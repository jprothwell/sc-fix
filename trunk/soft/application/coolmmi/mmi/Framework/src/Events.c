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
 *	Events.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for protocol/keypad/interrupt event handler .
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
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#define __NEWSIMULATOR 
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "globalconstants.h"
#include "debuginitdef.h"
#include "mmi_data_types.h"
#include "frameworkstruct.h"
#include "eventsgprot.h"
#include "wrappergprot.h"
#include "eventsdcl.h"
#include "eventsdef.h"
#include "custdataprots.h"
#include "satgprots.h"
#include "idleappprot.h"
#include "idleappdef.h"
#include "sublcdhistorygprot.h"
#include "simdetectiongexdcl.h"
#include "settingdefs.h"
#include "settingsgdcl.h"
#include "callmanagementgprot.h"
#include "volumehandler.h"
#ifdef __MMI_THEMES_V2_SUPPORT__
	#include "thememanager.h"
#endif
#include <csw.h>  
#include "mmi.h"   
#include "adp_timer.h"
#undef __NEWSIMULATOR 
#include "l4dr.h"
#include "quickaccessmenuitem_def.h"
#ifdef __MMI_WGUI_MINI_TAB_BAR__
#include "wgui_tab_bars.h"
#endif
/*add start by wuzc at 20061125*/
#include "mmi_trace.h"
/*add end by wuzc at 20061125*/
// added by MK
#ifdef __MMI_EMAIL__
#include "emailappgprot.h"
#endif // #ifdef __MMI_EMAIL__
#ifdef __MMI_SCREEN_ROTATE__
#include "mmi_features_camera.h"
#include "mmi_features_video.h"
#include "cameraresdefs.h"
#include "vdoplyscrenum.h"
#include "vdorecscrenum.h"
#include "wallpaperdefs.h"
#include "servicedefs.h"
#include "phonebookdef.h"
#include "callmanagementiddef.h"
#include "stopwatchdef.h"
#include "callhistoryenum.h"
#include "fmradiodef.h"
#include "gameframework.h"  //ugrec_tky 
#include "calculator.h" //ugrec_tky
#include "worldclock.h"

#endif
/* 
**   Extern Varibales 
*/

#ifdef __DIRECT_ENTRY_FACTORY_MODE_ON_BOOTUP__
#include "miscfunctions.h"
#endif

#ifdef __J2ME__
#include "javadef.h"
#endif

#ifdef JATAAYU_SUPPORT
#include "jmms_inc.h"
#include "jwap_inc.h"
#include "messagingapi.h"
#include "jdi_communicator.h"
#include "jmms_mmi_cb.h"
#include "jmms_settings.h"
#include "phonebookgprot.h"
#include "jmms_struct_list.h"
#include "custom_nvram_editor_data_item.h"
#include "jprofilehandlertypes.h"
#endif
#include "nvram_user_defs.h"
#if defined(__MMI_MAINLCD_220X176__)
extern void cat222_title_text_stop_scrolling(void);
#endif
extern pBOOL IsBackHistory;
extern U8   gKeyPadLockFlag;
extern U8 gIsHelpEnabled;
extern U8 gInitAllAppFlag;
extern U8 gInsertSimAppFlag;
extern U8 gEmerencyNoDialed;  
extern U8 gIsInSATCall;

#ifdef __MMI_TTS_FEATURES__ 
extern nvram_tts_setting_struct g_tts_setting;
#endif

FuncPtr currKeyFuncPtrs[MAX_KEYS][MAX_KEY_TYPE];		/* available key func ptrs to current screen  */
hiliteInfo maxHiliteInfo[MAX_HILITE_HANDLER];			/* available hilite func ptrs  */
U8 hintData[MAX_SUB_MENUS][MAX_SUB_MENU_HINT_SIZE];
U8 subMenuData[MAX_SUB_MENUS][MAX_SUB_MENU_SIZE];
PU8 subMenuDataPtrs[MAX_SUB_MENUS];
PU8 hintDataPtrs[MAX_SUB_MENUS];
U8 processUpRepeatKey[MAX_KEYS];

/* 
**   Local Variables 
*/
static U16 currMaxHiliteInfo;									 
static U16 maxProtocolEvent[MMI_SIM_NUMBER];
static U16 maxInterruptEvent;
static U16 currParentID;										/* current parent item id */
static U16 currKeyCode;
static U16 currKeyType;
static U16 currHiliteID;
static U16 currEntryScrnID;										/* curr entry func ptr with scrn id */
static FuncPtr currEntryFuncPtr;
static U16 currExitScrnID;
U16 currTopScrnID;//niej by tc9751 and cs621									/* curr exit func ptr with scrn id */
static FuncPtr currExitFuncPtr;
static PseventInfo protocolEventHandler[MMI_SIM_NUMBER][MAX_PROTOCOL_EVENT];
static PsInteventInfo interruptEventHandler[MAX_INTERRUPT_EVENT];
static FuncPtr currSubLcdExitFuncPtr=NULL;

/*
**   Local Functions
*/
static void ExecuteCurrExitHandler(void);								/* execute current exit func handler, before execute current event handler */
static void ExecuteCurrExitHandler_Ext(void);								/* execute current exit func handler, before execute current event handler */
static void SetGenericExitHandler(U16 scrnID, FuncPtr exitFuncPtr, FuncPtr entryFuncPtr);
 
//static void SetExitHandler(U16 scrnID, FuncPtr exitFuncPtr);
 

/* 
**   Global Functions 
*/
extern void EmergencyCallCheck(S16 keyCode, S16 keyType);
extern void InitKeypadBeforePWRON(void);
extern U16 GetTotalCallCount(void);

#ifdef __MMI_SCREEN_ROTATE__
extern void mmi_frm_screen_rotate_handler(void);
#endif


#ifdef __MMI_SCREEN_SNAPSHOT__
	extern void mmi_camera_scr_snapshot(void);
#endif

/**************************************************************

	FUNCTION NAME		: InitEvents()

  	PURPOSE				: Initialization

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InitEvents(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 keyCode = 0;
	U16 keyType = 0;
	U16 count = 0;
	U16 i;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_FUN_EVENT_INIT));

	for(keyCode = 0; keyCode < MAX_KEYS; keyCode++)
	{
		for(keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
		{
			currKeyFuncPtrs[keyCode][keyType] = NULL;
		}
	}

	currEntryScrnID = 0;
	currEntryFuncPtr = NULL;

	currExitScrnID = 0;
	currTopScrnID = 0;
	currExitFuncPtr = NULL;

	currSubLcdExitFuncPtr=NULL;

	//JL remove to outside init InitHistory();

	currMaxHiliteInfo = 0;
	currParentID = 0;

	//CSD added by JL for PS interrupt
	maxInterruptEvent = 0;

	/* //////// initilaize the timer, protocol & hardware events with func ptrs /////// */
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		maxProtocolEvent[i] = 0;
		for(count = 0; count < MAX_PROTOCOL_EVENT; count++)
		{
			protocolEventHandler[i][count].eventID = 0;
			protocolEventHandler[i][count].entryFuncPtr = NULL;
		}
	}

	
	for (count = 0; count < MAX_HILITE_HANDLER; count++)
	{
		maxHiliteInfo[count].entryFuncPtr = NULL;
        maxHiliteInfo[count].hintFuncPtr = NULL;
	}
	for(count = 0; count < MAX_SUB_MENUS; count++)
			subMenuDataPtrs[count]=subMenuData[count];

	//CSD added by JL for PS interrupt
	for(count = 0; count < MAX_INTERRUPT_EVENT; count++)
	{
		interruptEventHandler[count].eventID =  0;
		interruptEventHandler[count].entryIntFuncPtr = NULL;
		interruptEventHandler[count].postIntFuncPtr = NULL;
	}

	////////////////////////////////////////////////////////////////////////////////////////
}

/**************************************************************

	FUNCTION NAME		: DeInitFramework_KeyAndEventHandler()

  	PURPOSE				: DeInitialization

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void DeInitFramework_KeyAndEventHandler(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 keyCode = 0;
	U16 keyType = 0;
	U16 count = 0;
	U16 i;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_FUN_EVENT_DEINIT_KEY_EVENT));

	for(keyCode = 0; keyCode < MAX_KEYS; keyCode++)
	{
		for(keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
		{
			currKeyFuncPtrs[keyCode][keyType] = NULL;
		}
	}

	currEntryScrnID = 0;
	currEntryFuncPtr = NULL;

	currExitScrnID = 0;
	currTopScrnID = 0;
	currExitFuncPtr = NULL;

	currSubLcdExitFuncPtr=NULL;

	currMaxHiliteInfo = 0;
	currParentID = 0;

	memset(maxProtocolEvent, 0 ,sizeof(maxProtocolEvent));

	/* //////// initilaize the timer, protocol & hardware events with func ptrs /////// */
	for(i = 0; i < MMI_SIM_NUMBER; i ++)
	{
		for(count = 0; count < MAX_PROTOCOL_EVENT; count++)
		{
			protocolEventHandler[i][count].eventID = 0;
			protocolEventHandler[i][count].entryFuncPtr = NULL;
		}
	}

	for(count = 0; count < MAX_HILITE_HANDLER; count++)
	{
		maxHiliteInfo[count].entryFuncPtr = NULL;
        maxHiliteInfo[count].hintFuncPtr = NULL;
	}

}

/**************************************************************

	FUNCTION NAME		: DeInitFramework_History()

  	PURPOSE				: DeInitialization

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void DeInitFramework_History(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_FUN_EVENT_DEINIT_HISTORY));

#ifdef MMI_ON_HARDWARE_P

	 
	InitKeypadBeforePWRON();   
	// CSD end
#endif

	DinitHistory();  
	DinitSubLCDHistory();  
   //JL remove to ShutDownApp.c
}

/**************************************************************

	FUNCTION NAME		: DeInitFramework()

  	PURPOSE				: DeInitialization

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DeInitFramework(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_FUN_EVENT_DEINIT));
	
    // lisen 03/27/2004
    DeInitFramework_KeyAndEventHandler();
    DeInitFramework_History();
}

/**************************************************************

	FUNCTION NAME		: ExecuteCurrKeyHandler()

  	PURPOSE				: executes current key func handler

	INPUT PARAMETERS	: S16 keyCode, S16 keyType

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
/*
	we don't support multi-click key's feature,On full keypad project, there will be some keys' confilect when double press operation
	To solve the problem, do the modify blew.log the key's state, when key up ,clear the flag,if the flag still, don't execute next key's event.
*/
#ifdef __MMI_ENGINEER_MODE__
#if defined(AUTO_TEST_KEYPRESS_RECORD_SUPPORT)
extern void RecordCurrKeyHandler(S16 keyCode, S16 keyType);
#endif 
#endif

#ifdef __MMI_TOUCH_SCREEN__
extern BOOL mmi_pen_in_handwriting_process(void);
#endif

void ExecuteCurrKeyHandler(S16 keyCode, S16 keyType)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	FuncPtr currFuncPtr = NULL;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

#if (defined(__MMI_PROJECT_T33__) || defined(__MMI_SHARE_KEY_LSK_WITH_SEND2__))&&defined(__SENDKEY2_SUPPORT__)  
	if(keyCode == KEY_LSK )
     {
		if(IDLE_SCREEN_ID==GetActiveScreenId())
		keyCode = KEY_SEND2;
	}
#endif

    
	#ifdef MMI_ON_WIN32
	#ifdef __MMI_ENGINEER_MODE__
#if defined(AUTO_TEST_KEYPRESS_RECORD_SUPPORT)
	RecordCurrKeyHandler(keyCode,keyType);
#endif 
	#endif
	#endif

#ifdef __MMI_TOUCH_SCREEN__
        if(mmi_pen_in_handwriting_process()&&(keyCode != KEY_POUND))
            return ;
#endif

	currKeyCode = keyCode;
	currKeyType = keyType;
	if(!gEmerencyNoDialed)
	{
		switch(keyType)
		{
			case KEY_EVENT_DOWN:
				processUpRepeatKey[keyCode]=1;
				break;
			case KEY_REPEAT:
				if(!processUpRepeatKey[keyCode]) return;
				break;
			case KEY_EVENT_UP:
				if(!processUpRepeatKey[keyCode]) return;
				else
					processUpRepeatKey[keyCode]=0;
			break;

		}
		currFuncPtr = currKeyFuncPtrs[keyCode][keyType];

		 
		if ( keyCode == KEY_ENTER && currFuncPtr == NULL )
		{
			/* If any one type of the key eventis registered by APP, we assume APP want to control this key by itself. */
			if ( currKeyFuncPtrs[KEY_ENTER][KEY_EVENT_UP]==NULL &&
				currKeyFuncPtrs[KEY_ENTER][KEY_EVENT_DOWN]==NULL &&
				currKeyFuncPtrs[KEY_ENTER][KEY_REPEAT] == NULL &&
				currKeyFuncPtrs[KEY_ENTER][KEY_LONG_PRESS] == NULL )

			currFuncPtr = currKeyFuncPtrs[KEY_LSK][keyType];
		}
		 

		if((keyType==KEY_REPEAT) && (currFuncPtr==NULL) && (keyCode!=KEY_RSK) && (keyCode!=KEY_LSK)) /*20040906 Disable repeat for LSK and RSK*/
			currFuncPtr = currKeyFuncPtrs[keyCode][KEY_EVENT_DOWN];

		#ifdef __MMI_WGUI_MINI_TAB_BAR__
			if ( mmi_wgui_if_mini_tab_bar_enable()==MMI_TRUE && keyCode==KEY_LEFT_ARROW && keyType==KEY_EVENT_DOWN)
				currFuncPtr=mmi_wgui_mini_tab_bar_goto_prev;
			
			if ( mmi_wgui_if_mini_tab_bar_enable()==MMI_TRUE && keyCode==KEY_RIGHT_ARROW && keyType==KEY_EVENT_DOWN )
				currFuncPtr=mmi_wgui_mini_tab_bar_goto_next;
		#endif

		#ifdef __MMI_SCREEN_SNAPSHOT__ 
			if((keyType == KEY_LONG_PRESS) && (keyCode == KEY_CAMERA) && (currFuncPtr == NULL))
			{
				currFuncPtr = mmi_camera_scr_snapshot;
			}
		#endif /* __MMI_SCREEN_SNAPSHOT__ */

		
		Trace( "Key_Event: >>>keyCode=%d, keyType=%d, currFuncPtr=%p >>>>>> \n", keyCode, keyType, currFuncPtr);

#ifdef __ZYX_PLATFORM_SUPPORT__
        {
            extern void Zyx_key_handler(S16 keycode,S16 keytype);
            Zyx_key_handler(keyCode,keyType);
        }
#endif
		if(currFuncPtr)
		{
			(*currFuncPtr)();
		 }
	}
	else
		gEmerencyNoDialed=0;
}

/**************************************************************

	FUNCTION NAME		: ExecuteCurrProtocolHandler()

  	PURPOSE				: executes current protocol func handler

	INPUT PARAMETERS	: U16 eventID, void* MsgStruct

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
static U8 gProtocolEvnetSrcModeIndex;
void MTPNP_AD_set_protocol_event_src_mode(U8 nIndex)
{
	gProtocolEvnetSrcModeIndex = nIndex;
}

U8 MTPNP_AD_get_protocol_event_src_mode(void)
{
	return gProtocolEvnetSrcModeIndex;
}
void ExecuteCurrProtocolHandler(U32 eventID,void* MsgStruct,int mod_src, void* peerBuf)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 count = 0;
	//PsExtFuncPtr currFuncPtr = NULL;
	PsExtPeerFuncPtr currFuncPtr = NULL;
	PsExtPeerIntFuncPtr currInterruptFuncPtr = NULL, currPostInterruptFuncPtr = NULL;
	U8	interrup_result=MMI_FALSE; //False not handle interrupt, True will handle
	U8 mod_src_index = mod_src - MOD_L4C;


	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	if(ADP_SIM_NUMBER > MMI_SIM_NUMBER &&(mod_src_index == 1 ||mod_src_index == 2||mod_src_index == 3))
	{
		mmi_trace(1, "ExecuteCurrProtocolHandler, give up event %d for sim%d",eventID, mod_src_index);
		return;
	}
	if(mod_src_index >= MMI_SIM_NUMBER)
		mod_src_index = 0;
	
	MTPNP_AD_set_protocol_event_src_mode(mod_src_index);
	
	mmi_trace(1, "ExecuteCurrProtocolHandler, mod_src_index is %d",mod_src_index);
	
	for (count = 0; count < maxProtocolEvent[mod_src_index]; count++)
	{
		if (protocolEventHandler[mod_src_index][count].eventID == eventID)
		{
			currFuncPtr = (PsExtPeerFuncPtr)protocolEventHandler[mod_src_index][count].entryFuncPtr;
			break;
		}
	}


	//CSD added by JL 040302 for interrupt handle preprocess
	for(count = 0; count < maxInterruptEvent; count++)
	{
		if(interruptEventHandler[count].eventID == eventID)
		{
			currInterruptFuncPtr = (PsExtPeerIntFuncPtr)interruptEventHandler[count].entryIntFuncPtr;
			currPostInterruptFuncPtr = (PsExtPeerIntFuncPtr)interruptEventHandler[count].postIntFuncPtr;
			break;
		}
	}
	//CSD added by JL 040302 for interrupt handle preprocess

	if (currInterruptFuncPtr)
	{
		MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_EXECURPTO_INTP_HDLR, eventID));
		interrup_result = (*currInterruptFuncPtr)(MsgStruct,mod_src, peerBuf);
	}
	//if(currFuncPtr)
	mmi_trace(1, "ExecuteCurrProtocolHandler, Fun is 0x%x, MsgStruct is 0x%x",currFuncPtr, MsgStruct);
	if ( (currFuncPtr) && (!interrup_result) )
	{
		MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_EXECURPTO_HDLR, eventID));
		//(*currFuncPtr)(MsgStruct,mod_src);
		(*currFuncPtr)(MsgStruct, mod_src, peerBuf);
	}
	else
	{
		MMI_TRACE( (MMI_TRACE_WARNING, MMI_FRM_INFO_EVENT_EXECURPTO_NO_HDLR, eventID));
	}

	if (currPostInterruptFuncPtr)
	{
		(*currPostInterruptFuncPtr)(MsgStruct,mod_src, peerBuf);
	}
}

/**************************************************************

	FUNCTION NAME		: ExecuteCurrExitHandler()

  	PURPOSE				: executes current exit func handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern void UI_common_screen_exit(void);
extern void UI_common_screen_pre_exit(void);

static void ExecuteCurrExitHandler(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	//CSD added by JL 040115 for create a new function without key clear if need.
	ExecuteCurrExitHandler_Ext();
	/* it clears all the key handlers, registered by the current screen */
	ClearAllKeyHandler();
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
	tm_reset_imageid_image_header_pool();
#endif
	//CSD end
}

/**************************************************************

	FUNCTION NAME		: ReplaceNewScreenHandler()

  	PURPOSE				: Replace current new screen handler

	INPUT PARAMETERS	: U16 scrnID, FuncPtr entryFuncPtr

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReplaceNewScreenHandler(U16 scrnID, FuncPtr exitFuncPtr, FuncPtr entryFuncPtr)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_SETEXIT_HDLR, scrnID));
	currExitScrnID = scrnID;
	currExitFuncPtr = exitFuncPtr;
	currEntryFuncPtr = entryFuncPtr;
}

/**************************************************************

	FUNCTION NAME		: EntryNewScreen()

  	PURPOSE				: To set the current topest screen ID for entry screen function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
#if defined(CAM_MD_SUPPORT)
 extern void    csd_stop_sw_motionsensor(void);
#endif
S16 g_dm_pre_scr = -1;//dial_pop
U8 EntryNewScreen(U16 newscrnID, FuncPtr newExitHandler, FuncPtr newEntryHandler, void  *peerBuf)
{
     /*----------------------------------------------------------------*/
     /* Local Variables                                                								*/
     /*----------------------------------------------------------------*/
     /*----------------------------------------------------------------*/
     /* Code Body                                                      								*/
     /*----------------------------------------------------------------*/
     /* store the current Top entry screen */
 
	g_dm_pre_scr = g_dm_data.S32ScrId;//dial_pop
	enable_check_for_small_screen();  
 
	currTopScrnID = newscrnID;
     /* entry the new screen */
	ExecuteCurrExitHandler();
     /* store the exit current Top entry screen */
	currExitScrnID = newscrnID;
     if ( (newExitHandler!=NULL) || (newEntryHandler!=NULL) )
		SetGenericExitHandler(newscrnID, newExitHandler, newEntryHandler);
 
	disable_check_for_small_screen();  
 
#ifdef __MMI_SCREEN_ROTATE__
	 if((newscrnID != SCR_ID_CAMERA_APP)
		 &&(newscrnID != SCR1004_CM_DISPENDCALLDURATION)
		 &&(newscrnID != SCR_ID_VDOPLY_APP)
		 &&(newscrnID != SCR_ID_VDOREC_APP)
		 &&(newscrnID != SCR_ID_VDOPLY_PLAYER)
		 &&(newscrnID != SCR_WALLPAPER_VIEW)
#ifdef FM_RADIO_ENABLE
		 &&(newscrnID != SCR_ID_FMRDO_MAIN)
#endif
		 &&(newscrnID != ITEMSCR_INCOMING_CALL)
		 &&(newscrnID != SCR_MULTIPURPOSE_STOPWATCH)
		 &&(newscrnID != SCR_SPLIT_TIMING)
		 &&(!isInCall())
		 &&(!isIncomingCall())
		 &&(newscrnID != IDLE_SCREEN_DIGIT_HANDLER_ID)
		 &&(newscrnID != GFX_GAME_SCREEN)  //ugrec_tky 
		 &&(newscrnID != SCR_ID_CALC_APP)  //ugrec_tky 
		 &&(newscrnID != SCR_ID_WC_BROWSE_CITY)
#if defined(JATAAYU_SUPPORT) // wangmg@20091013
		 &&!jWap_GetWapAppStatus()
#endif
		 &&(newscrnID != SCR_ID_CAMERA_ADD_FRAME_VIEW)
		 )
		 {
			 mmi_frm_screen_rotate_handler();
		 }
#if defined(__MMI_SUPPORT_JME__)
		 if(newscrnID == 50000){
			 if(mmi_frm_is_screen_width_height_swapped()){
				 mmi_frm_reset_screen_rotation();
			 }
		 }
#endif
	 
#endif

#if defined(CAM_MD_SUPPORT)
     csd_stop_sw_motionsensor();
#endif

     return  MMI_TRUE;
}

/**************************************************************

	FUNCTION NAME		: EntryNewScreen_Ext()

  	PURPOSE				: To set the current topest screen ID for entry screen function, without clear keyhandler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U8 EntryNewScreen_Ext(U16 newscrnID, FuncPtr newExitHandler, FuncPtr newEntryHandler, void  *peerBuf)
{
     /*----------------------------------------------------------------*/
     /* Local Variables                                                								*/
     /*----------------------------------------------------------------*/
     /*----------------------------------------------------------------*/
     /* Code Body                                                      								*/
     /*----------------------------------------------------------------*/
     /* store the current Top entry screen */
 
	enable_check_for_small_screen(); 
 
	currTopScrnID = newscrnID;
     /* entry the new screen */
	ExecuteCurrExitHandler_Ext();
     /* store the exit current Top entry screen */
	currExitScrnID = newscrnID;
     if ( (newExitHandler!=NULL) || (newEntryHandler!=NULL) )
		SetGenericExitHandler(newscrnID, newExitHandler, newEntryHandler);
 
	disable_check_for_small_screen();  
 
     return  MMI_TRUE;
}



/**************************************************************

	FUNCTION NAME		: ExecuteCurrExitHandler_Ext()

  	PURPOSE				: executes current exit func handler without clear keys

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
//CSD added by JL for keypad without clearall requirement
void ExecuteCurrExitHandler_Ext(void)
{
#if 1   
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8 count=0;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	ADD_SCREEN_STRINGS((0));
	ADD_SCREEN_IMAGES((0));
	ADD_SCREEN_MENUS((0));

	/*	Failsafe common function before exit screen */
	UI_common_screen_pre_exit();
	
	for(count = 0; count < MAX_SUB_MENUS; count++)
			subMenuDataPtrs[count]=subMenuData[count];


	if(currEntryFuncPtr || currExitFuncPtr)
		ClearAllInterruptEventHandler();

	 
	if(currEntryFuncPtr)
	{
		GenericExitScreen(currExitScrnID, currEntryFuncPtr);
	}

	if(currExitFuncPtr)
	{
		mmi_trace(1,"ExecuteCurrExitHandler_Ext ---->  0x%x ", currExitFuncPtr);
		(*currExitFuncPtr)();
	}
	
	 
	if(ExitCategoryFunction != NULL)
		ExitCategoryFunction();
	//CSD end
	
       currEntryFuncPtr = NULL;
	currExitFuncPtr = NULL;
#if defined(__MMI_MAINLCD_220X176__)
	cat222_title_text_stop_scrolling();
#endif
	/*	Failsafe common screen exit function	*/
	UI_common_screen_exit();

	/* it clears all the key handlers, registered by the current screen */
	IsBackHistory = FALSE;

	// To dump the history memory
	HistoryDump();
#endif
}


/**************************************************************

	FUNCTION NAME		: ExecuteCurrSubLcdExitHandler()

  	PURPOSE				: executes current exit func handler for the Sub-LCD

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExecuteCurrSubLcdExitHandler(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(currSubLcdExitFuncPtr)
	{
		MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_EXIT_SUBLCD_HDLR));

		(*currSubLcdExitFuncPtr) ();
		currSubLcdExitFuncPtr=NULL;
	}
}
/**************************************************************

	FUNCTION NAME		: ExecuteCurrHiliteHandler()

  	PURPOSE				: executes current hilite func handler

	INPUT PARAMETERS	: S32 hiliteid

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
#include "callhistoryenum.h"
#include "globalmenuitems.h"
extern  void Left_Arrow_Simulate_With_Lsk(void);

void ExecuteCurrHiliteHandler(S32 hiliteid)
{
//mmi_trace(1,"ExecuteCurrHiliteHandler###############################################################\n");
#if 0
  S32 test_value;
  test_value = MAX_MENU_ITEMS;
  mmi_trace(1,"MAX_MENU_ITEMS = %d\n",test_value);
   test_value = MENU8101_RECEIVED_CALLS;
  mmi_trace(1,"MENU8101_RECEIVED_CALLS = %d\n",test_value);
     test_value = MENU8102_DIALED_CALLS;
  mmi_trace(1,"MENU8102_DIALED_CALLS = %d\n",test_value);

   test_value = MENU8103_MISSED_CALLS;
  mmi_trace(1,"MENU8103_MISSED_CALLS = %d\n",test_value);

   test_value = MENU_DIALED_CALLS;
  mmi_trace(1,"MENU_DIALED_CALLS = %d\n",test_value);
   test_value = MENU_RECEIVED_CALLS;
  mmi_trace(1,"MENU_RECEIVED_CALLS = %d\n",test_value);

   test_value = MENU_MISSED_CALLS;
  mmi_trace(1,"MENU_MISSED_CALLS = %d\n",test_value);

#endif 
	//JL: To avoid the arrow left key to entry unexpected function, while other APP need they shall register again as BY request. 
    #ifndef __MMI_MAINLCD_128X128__
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(Left_Arrow_Simulate_With_Lsk, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    #endif
	ExecuteCurrHiliteHandler_Ext(hiliteid);

}

void ExecuteCurrHiliteHandler_Ext(S32 hiliteid)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	FuncPtr currFuncPtr = NULL;
	S32 hiliteItemID = 0;
#ifdef __MMI_TTS_FEATURES__ 
	U16 strId;
	char* strMenuName;
	S32 nLen;
#endif
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_EXECURHILIHTE_HDLR, hiliteid));

	currHiliteID=(U16)hiliteid;
	if(currParentID==0)
	{
		MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_ERROR_EVENT_EXECURHILIHTE_HDLR));
		return;
	}

 
	hiliteItemID = GetSeqItemId_Ext((U16)currParentID, (S16)hiliteid);
//	hiliteItemID = GetSeqItemId((U16)currParentID, (S16)hiliteid);
	currFuncPtr = maxHiliteInfo[hiliteItemID].entryFuncPtr;

#ifdef __MMI_TTS_FEATURES__ 

	
	//如果开启则直接播放，否则不进行播放。
	if(g_tts_setting.curTTSMenuReadStatus == 0)
	{
		strId = GetStringIdOfItem((U16)hiliteItemID);
		strMenuName = GetString(strId);
		nLen = UCS2Strlen(strMenuName);
		ejTTS_PlayToPCM(strMenuName, nLen*2);
	}
#endif

	if(currFuncPtr)
	{
		(*currFuncPtr)();
	}
}


/**************************************************************

	FUNCTION NAME		: GetCurrHiliteID()

  	PURPOSE				: Returns the index of the currently highlighted menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U16 GetCurrHiliteID(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	return currHiliteID;
}

/**************************************************************

	FUNCTION NAME		: SetHiliteHandler()

  	PURPOSE				: Sets hilite func handlers

	INPUT PARAMETERS	: U16 itemid , FuncPtr hiliteFuncPtr

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetHiliteHandler(U16 itemid, FuncPtr hiliteFuncPtr)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_SETCURHILIHTE_HDLR, itemid));

	maxHiliteInfo[itemid].entryFuncPtr = hiliteFuncPtr;
}

/**************************************************************

	FUNCTION NAME		: ClearHiliteHandler()

  	PURPOSE				: Clear hilite handler

	INPUT PARAMETERS	: U16 itemid

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ClearHiliteHandler(U16 itemid)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_CLRCURHILIHTE_HDLR, itemid));

	maxHiliteInfo[itemid].entryFuncPtr = NULL;
}

/**************************************************************

	FUNCTION NAME		: ConstructHintsList()

  	PURPOSE				: Constructs Hint List for a static menu screen

	INPUT PARAMETERS	: U16 currParentID, U8 **hintArray

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern MMI_BOOL mmi_frm_test_menu_item_hide(U16 menu_item_id);
void ConstructHintsList(U16 curr_parent_ID, U8 **hintArray)
{

   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8 i,noOfChild;
	U16 hiliteItemID[MAX_SUB_MENUS];
	U8 idx=0;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	noOfChild=(U8)GetNumOfChild(curr_parent_ID);
	for(i=0;i<noOfChild;i++)
	{
		hiliteItemID[i]=GetSeqItemId((U16)curr_parent_ID, (S16)i);
 
		if ( !mmi_frm_test_menu_item_hide(hiliteItemID[i])) // the menuitem is not hidden
		{
			/*Maybe some menu items have been hidden, but users don't know this. 
			So we only count the unhidden menu items for hint items.*/
			if(maxHiliteInfo[hiliteItemID[i]].hintFuncPtr)
			{
				(*maxHiliteInfo[hiliteItemID[i]].hintFuncPtr)(idx); // only count the hint items for the unhidden menu
				hintArray[idx]=hintData[idx];
				idx++;
			}
			else
			{
				hintArray[idx++]=NULL;
			} 
		}
	}

}

/**************************************************************

	FUNCTION NAME		: SetHintHandler()

  	PURPOSE				: Sets Hint Func Handlers

	INPUT PARAMETERS	: U16 itemid , FuncPtr hiliteFuncPtr

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetHintHandler(U16 itemid, FuncPtrShort hintFuncPtr)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_SETHINT_HDLR, itemid));

	maxHiliteInfo[itemid].hintFuncPtr = hintFuncPtr;
}


/**************************************************************

	FUNCTION NAME		: ClearHintHandler()

  	PURPOSE				: clears hint handlers

	INPUT PARAMETERS	: U16 itemid

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ClearHintHandler(U16 itemid)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_CLRHINT_HDLR, itemid));
	
	maxHiliteInfo[itemid].hintFuncPtr = NULL;
}

/**************************************************************

	FUNCTION NAME		: ClearAllHiliteHandler(void)

  	PURPOSE				: clears all the hilite handlers

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ClearAllHiliteHandler(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 count = 0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_CLRALLHILIHTE_HDLR));

	for(count = 0; count < currMaxHiliteInfo; count++)
	{
		maxHiliteInfo[count].entryFuncPtr = NULL;
	}
}


/**************************************************************

	FUNCTION NAME		: SetParentHandler()

  	PURPOSE				: sets current parent item id

	INPUT PARAMETERS	: U16 parentid

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void SetParentHandler(U16 parentid)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_SETPARENT_HDLR, parentid));
	
	currParentID = parentid;
}

//CSD Brian added for AT+CVIB, 2003/11/23

U16 GetParentHandler(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	return currParentID;
}

/**************************************************************

	FUNCTION NAME		: SetKeyHandler()

  	PURPOSE				: sets the curent screen key handlers

	INPUT PARAMETERS	: FuncPtr funcPtr, U16 keyCode, U16 keyType

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetKeyHandler(FuncPtr funcPtr, U16 keyCode, U16 keyType)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
  
    #ifdef __MMI_WITH_IME_KEY__
	extern void handle_category28_change_input_mode(void);
	extern void wgui_handle_qsearch_singleline_inputbox_change_input_mode(void);
	extern void wgui_handle_singleline_inputbox_change_input_mode(void);
	extern void wgui_handle_inline_singleline_inputbox_change_input_mode(void);
      if(KEY_POUND ==  keyCode&& KEY_EVENT_DOWN == keyType)
      {
      	if(wgui_handle_inputbox_change_input_mode == funcPtr||
   	   handle_category28_change_input_mode == funcPtr||
   	   wgui_handle_qsearch_singleline_inputbox_change_input_mode == funcPtr||
   	   wgui_handle_singleline_inputbox_change_input_mode == funcPtr||
   	   wgui_handle_inline_singleline_inputbox_change_input_mode == funcPtr
   	   )
   	   	keyCode = KEY_SHIFT_INPUTM;

	#ifdef __MMI_BLACKBERRY_QWERTY__
		if(wgui_handle_inputbox_change_input_mode == funcPtr||
   	   		handle_category28_change_input_mode == funcPtr||
   	   		wgui_handle_singleline_inputbox_change_input_mode == funcPtr||
   	   		wgui_handle_inline_singleline_inputbox_change_input_mode == funcPtr
   	   	)
		currKeyFuncPtrs[KEY_CALENDER][keyType] = funcPtr;
	#endif
		
      }
      #endif
   
    currKeyFuncPtrs[keyCode][keyType] = funcPtr;
}

/**************************************************************

	FUNCTION NAME		: GetKeyHandler()

  	PURPOSE				: return the curent screen key handlers

	INPUT PARAMETERS	: U16 keyCode, U16 keyType

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
FuncPtr GetKeyHandler(U16 keyCode, U16 keyType)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_ASSERT((keyCode<MAX_KEYS) && (keyType<MAX_KEY_TYPE));

	return(currKeyFuncPtrs[keyCode][keyType]);
}

/**************************************************************

	FUNCTION NAME		: void SetGroupKeyHandler()

  	PURPOSE				: sets the curent screen same key handler to the group pf keycodes

	INPUT PARAMETERS	: FuncPtr funcPtr, PU16 keyCodes, U8 len,U16 keyType

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetGroupKeyHandler(FuncPtr funcPtr, PU16 keyCodes, U8 len,U16 keyType)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8 count;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_ASSERT((len<=MAX_KEYS) && (keyType<MAX_KEY_TYPE));  

	for(count=0;count<len;count++)
	{
		MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_SETGROUPKEY_HDLR, keyCodes[count], keyType));
		currKeyFuncPtrs[keyCodes[count]][keyType] = funcPtr;
	}
}

/**************************************************************

	FUNCTION NAME		: ClearKeyHandler()

  	PURPOSE				: clears the current screen key handlers

	INPUT PARAMETERS	: FuncPtr keyCode, U16 keyType

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ClearKeyHandler(U16 keyCode, U16 keyType)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_CLRKEY_HDLR, keyCode, keyType));

	MMI_ASSERT((keyCode<MAX_KEYS) && (keyType<MAX_KEY_TYPE));

	currKeyFuncPtrs[keyCode][keyType] = NULL;
}

/**************************************************************

	FUNCTION NAME		: ClearOneKeyAllHandler()

  	PURPOSE				: clears the current screen key handlers

	INPUT PARAMETERS	: FuncPtr keyCode, U16 keyType

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ClearOneKeyAllHandler(U16 keyCode)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   U16 keyType = 0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_ASSERT((keyCode<MAX_KEYS));

	for(keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
	{
		currKeyFuncPtrs[keyCode][keyType] = NULL;
	}
}

/**************************************************************

	FUNCTION NAME		: ClearAllKeyHandler()

  	PURPOSE				: clears all the current screen key handlers

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ClearAllKeyHandler(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 keyCode = 0;
	U16 keyType = 0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_CLRALLKEY_HDLR));

	for(keyCode = 0; keyCode < MAX_KEYS; keyCode++)
	{
		for(keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
		{
			currKeyFuncPtrs[keyCode][keyType] = NULL;
		}
	}

	PowerAndEndKeyHandler();

	SetDefaultVolumeKeyHandlers();   
}

/**************************************************************

	FUNCTION NAME		: ClearInputEventHandler

  	PURPOSE				: Clear input device event handlers

	INPUT PARAMETERS	: input device type

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ClearInputEventHandler(U16 device)
{
	if (device & MMI_DEVICE_KEY )
	{
		ClearAllKeyHandler();
	}

#if (0)
#ifdef  __MMI_TOUCH_SCREEN__
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
#endif
}

/**************************************************************

	FUNCTION NAME		: PowerAndEndKeyHandler(void )

  	PURPOSE				: sets power key & end key handlers

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
#if defined(__DIRECT_ENTRY_FACTORY_MODE_ON_BOOTUP__)
extern void CallBackPowerOnAnimationCompleteWrapper(void);
#endif
extern pBOOL GetCMScrnFlag(void);
extern BOOL checkVMOn(void);
BOOL checkVmBackGround(void);
#ifdef __NATIVE_JAVA__
unsigned char gcj_IsJavaOn();
void GoBackJavaEditer();
unsigned char gcj_IsFormJavaNativeEditer();
#endif
void PowerAndEndKeyHandler(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	/**
	 * XXX Ashish - The end key must not be registered to
	 * Go to Idle screen in case any call exists on the
	 * the phone. This is a temporary solution for fixing the
	 * CM bugs
	 */
	if(!g_pwr_context[0].PowerOnTillIdleApp)
	{
		/**
		 * XXX Shweta - The end key should end all the existing calls
		 *  and screen should be the last screen thru call menu.
		 */
		if (!isInCall()) 
		{
			
			#ifdef __MMI_SUPPORT_JME__
			if(checkVMOn()&&(!checkVmBackGround()))
			{
				currKeyFuncPtrs[KEY_END][KEY_EVENT_DOWN] = GoBackHistory;
			}
			else
		    #endif
#ifdef __J2ME__
#ifdef __NATIVE_JAVA__
		     if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
             {
                 currKeyFuncPtrs[KEY_END][KEY_EVENT_DOWN] = GoBackHistory;
             }
             else
#else
             if(g_jam_vm_state == JVM_PAUSED_STATE)
			 {
				 currKeyFuncPtrs[KEY_END][KEY_EVENT_DOWN] = GoBackHistory;
			 }
			 else if(g_jam_vm_state == JVM_BG_BLOCKING_STATE)
             {
				 currKeyFuncPtrs[KEY_END][KEY_EVENT_DOWN] = CancelGotoJavaSreen;
             }
             else if(g_jam_vm_state == JVM_BG_RUNNING_STATE)
             {
                 currKeyFuncPtrs[KEY_END][KEY_EVENT_DOWN] = GoBackHistory;
             }
             else
#endif
#endif
		
		#ifdef JATAAYU_SUPPORT
		#ifdef JATAAYU_SUPPORT_WAP
			if(jWap_GetWapAppStatus())
			{
				currKeyFuncPtrs[KEY_END][KEY_EVENT_DOWN] = jWap_keyEndHandler;
			}
			else
		#endif	
		#endif
		#ifdef __MMI_EMAIL__
			if (mmi_email_util_get_stop_cause()==EMAIL_MODULE_ACTIVE_NOW)
			{
				currKeyFuncPtrs[KEY_END][KEY_EVENT_DOWN] = mmi_email_main_goto_idle;
			}
			#ifdef __MMI_MULTI_SIM__
			// TODO:
			else
			#else				
			else if (!g_SecuritySetupContext.PINBlocked[0])
			#endif				
		#else
			#ifdef __MMI_MULTI_SIM__
			// TODO:
			#else
			if (!g_SecuritySetupContext.PINBlocked[0])
			#endif				
		#endif
			{
				/*gaosh Modify Start For 6435 and 6437 Ver: TBM780_T_20070823_2145  on 2007-8-24 14:24 */
				currKeyFuncPtrs[KEY_END][KEY_EVENT_DOWN] = DisplayIdleScreen;
				currKeyFuncPtrs[KEY_END][KEY_EVENT_UP] = DisplayLockUnlockPopupScreen;
				/*gaosh Modify End  For 6435 and 6437 Ver: TBM780_T_20070823_2145  on 2007-8-24 14:24 */
			}
		}
		else 
		{
			currKeyFuncPtrs[KEY_END][KEY_EVENT_DOWN] = HangupAllCalls;
		}
	}
	
#if defined(__DIRECT_ENTRY_FACTORY_MODE_ON_BOOTUP__)       
	if (g_DirectMode)
		currKeyFuncPtrs[KEY_END][KEY_EVENT_DOWN] = CallBackPowerOnAnimationCompleteWrapper;
#endif

	if(!gKeyPadLockFlag)
	{
#if defined(TGT_USING_POWER_KEY)
			currKeyFuncPtrs[KEY_POWER][KEY_LONG_PRESS] = ShutdownSystemOperation;
		#else

			currKeyFuncPtrs[KEY_END][KEY_LONG_PRESS] = ShutdownSystemOperation;
		#endif
	}
}

/**************************************************************

	FUNCTION NAME		: SetProtocolEventHandler()

  	PURPOSE				: sets protocol event handler

	INPUT PARAMETERS	: FuncPtr funcPtr, U16 eventID

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetProtocolEventHandler(PsFuncPtr funcPtr, U32 eventID)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 count = 0;
	pBOOL isNewEvent = TRUE;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_SETPTO_HDLR, eventID));

	for(count = 0; count < maxProtocolEvent[0]; count++)
	{
		if(protocolEventHandler[0][count].eventID == eventID)
		{
			isNewEvent = FALSE;
			break;
		}
	}

	if(isNewEvent)
	{
		maxProtocolEvent[0]++;
		MMI_ASSERT(maxProtocolEvent[0]<MAX_PROTOCOL_EVENT);
	}

	protocolEventHandler[0][count].eventID = eventID;
	protocolEventHandler[0][count].entryFuncPtr = funcPtr;
}

/**************************************************************

	FUNCTION NAME		: SetProtocolEventHandlerExt()

  	PURPOSE				: sets protocol event handler

	INPUT PARAMETERS	: FuncPtr funcPtr, U16 eventID

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetProtocolEventHandlerExt(PsFuncPtr funcPtr, U32 eventID, U8 nSimID)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 count = 0;
	pBOOL isNewEvent = TRUE;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_SETPTO_HDLR, eventID));

	for(count = 0; count < maxProtocolEvent[nSimID]; count++)
	{
		if(protocolEventHandler[nSimID][count].eventID == eventID)
		{
			isNewEvent = FALSE;
			break;
		}
	}

	if(isNewEvent)
	{
		maxProtocolEvent[nSimID]++;
		MMI_ASSERT(maxProtocolEvent[nSimID]<MAX_PROTOCOL_EVENT);
	}

	protocolEventHandler[nSimID][count].eventID = eventID;
	protocolEventHandler[nSimID][count].entryFuncPtr = funcPtr;
}
/**************************************************************

	FUNCTION NAME		: ClearProtocolEventHandler( )

  	PURPOSE				: clears protocol event handler

	INPUT PARAMETERS	: U16 eventID

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ClearProtocolEventHandler(U32 eventID)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_CLRPTO_HDLR, eventID));
	SetProtocolEventHandler(NULL, eventID);
}

/**************************************************************

	FUNCTION NAME		: ClearProtocolEventHandlerExt( )

  	PURPOSE				: clears protocol event handler

	INPUT PARAMETERS	: U16 eventID

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ClearProtocolEventHandlerExt(U32 eventID, U8 nSimID)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_CLRPTO_HDLR, eventID));
	SetProtocolEventHandlerExt(NULL, eventID, nSimID);
}

/**************************************************************

	FUNCTION NAME		: ClearAllProtocolEventHandler(void)

  	PURPOSE				: clears all the protocol event handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ClearAllProtocolEventHandler(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 count = 0;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_CLRALLPTO_HDLR));
	
	for(count = 0; count < maxProtocolEvent[0]; count++)
	{
		protocolEventHandler[0][count].eventID = 0;
		protocolEventHandler[0][count].entryFuncPtr = NULL;
	}

}

/**************************************************************

	FUNCTION NAME		: ClearAllProtocolEventHandlerExt(void)

  	PURPOSE				: clears all the protocol event handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ClearAllProtocolEventHandlerExt(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 count = 0;
   	U16 i;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_CLRALLPTO_HDLR));
	for(i = 0; i < MMI_SIM_NUMBER; i ++)
	{
		for(count = 0; count < maxProtocolEvent[i]; count++)
		{
			protocolEventHandler[i][count].eventID = 0;
			protocolEventHandler[i][count].entryFuncPtr = NULL;
		}
	}

}

#ifdef __MMI_MULTI_SIM__
void SetSlaveProtocolEventHandler(PsFuncPtr funcPtr, U32 eventID)
{
	SetProtocolEventHandlerExt(funcPtr, eventID, 1);
}

void ClearSlaveProtocolEventHandler(U32 eventID)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_CLRPTO_HDLR, eventID));
	ClearProtocolEventHandlerExt(eventID,1);
}
#endif

/**************************************************************

	FUNCTION NAME		: SetEntryHandler()

  	PURPOSE				: sets current screen entry handlers

	INPUT PARAMETERS	: U16 scrnID, FuncPtr entryFuncPtr

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetEntryHandler(U16 scrnID, FuncPtr entryFuncPtr)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_SETENTRY_HDLR, scrnID));
	currEntryScrnID = scrnID;
	currEntryFuncPtr = entryFuncPtr;
}

/**************************************************************

	FUNCTION NAME		: SetEntryHandler()

  	PURPOSE				: sets current screen exit handlers

	INPUT PARAMETERS	: U16 scrnID, FuncPtr entryFuncPtr

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
 
//static void SetExitHandler(U16 scrnID, FuncPtr exitFuncPtr)
//{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	//MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_SETEXIT_HDLR, scrnID));
	//currExitScrnID = scrnID;
	//currExitFuncPtr = exitFuncPtr;

	 
	//currEntryFuncPtr = NULL;
	
//}
 

/**************************************************************

	FUNCTION NAME		: ClearEntryHandler()

  	PURPOSE				: clears current screen entry handlers

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ClearEntryHandler(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_CLRENTRY_HDLR));
	currEntryScrnID = 0;
	currEntryFuncPtr = NULL;
	
}

 
/*****************************************************************************
* FUNCTION
*	SetGenericExitHandler
* DESCRIPTION
*   Generic function to set exit screen handler.
*	Store current screen id, exit function, and entry function.
* PARAMETERS
*	scrnID			IN		Screen ID
*	exitFuncPtr		IN		Exit function pointer
*	entryFuncPtr	IN		Entry function pointer
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
static void SetGenericExitHandler(U16 scrnID, FuncPtr exitFuncPtr, FuncPtr entryFuncPtr)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_SETGEN_EXIT_HDLR, scrnID));
   
	currExitScrnID = scrnID;
	currExitFuncPtr = exitFuncPtr;
	currEntryFuncPtr = entryFuncPtr;
}

 
/*****************************************************************************
* FUNCTION
*	GenericExitInlineScreen
* DESCRIPTION
*  Generic exit inline editor screen handler.
*  Add screen related data to histoy.
* PARAMETERS
*	scrnID			IN		Screen ID
*	entryFuncPtr	IN		Entry function pointer
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void GenericExitInlineScreen(U16 scrnID,  FuncPtr entryFuncPtr)
{
#if 1  //ss del 20060516
	history_t	h;
	U16 inputBufferSize;

	CloseCategory57Screen();
	h.scrnID = scrnID;
	h.entryFuncPtr = entryFuncPtr;
	GetCategoryHistory(h.guiBuffer);
	inputBufferSize =(U16)GetCategory57DataSize( ); 
	GetCategory57Data ((U8*) h.inputBuffer );  
	AddNHistory(h, inputBufferSize);     
#endif
}

/*****************************************************************************
* FUNCTION
*	GenericExitScreen
* DESCRIPTION
*   Generic exit screen handler.
*	Add screen related data to histoy.
* PARAMETERS
*	scrnID			IN		Screen ID
*	entryFuncPtr	IN		Entry function pointer
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void GenericExitScreen(U16 scrnID,  FuncPtr entryFuncPtr)
{
#if 1   
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	history_t h;
	U16 nHistory = 0;
  
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_EXEGEN_EXIT_HDLR, scrnID));
   
	h.scrnID = scrnID;
	h.entryFuncPtr = entryFuncPtr;
	pfnUnicodeStrcpy((S8*)h.inputBuffer, (S8*)&nHistory);
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
 
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	mmi_wgui_disable_mini_tab_bar();
#endif
 
#endif
}

U16 GetExitScrnID(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_GETGEN_EXIT_HDLR));
   
	return currExitScrnID;
}

void SetSubLcdExitHandler(FuncPtr exitSubLcdFuncPtr)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_SETSUBLCD_EXIT_HDLR, exitSubLcdFuncPtr));
   
	currSubLcdExitFuncPtr = exitSubLcdFuncPtr;

}

/**************************************************************

	FUNCTION NAME		: ClearExitHandler()

  	PURPOSE				: clears current screen exit handlers

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ClearExitHandler(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_CLREXIT_HDLR));
	currExitScrnID = 0;
	currExitFuncPtr = NULL;
//micha0601, fix generic exit function bug
	currEntryFuncPtr = NULL;
	
}
void ClearSubLcdExitHandler(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_CLRSUBLCD_EXIT_HDLR));
	currSubLcdExitFuncPtr = NULL;
	
}


/**************************************************************

	FUNCTION NAME		: GetkeyInfo()

  	PURPOSE				: gives key code & key type of last key event.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: U16 *keyCode, U16 *keyType

	RETURNS				: void

 

**************************************************************/
void GetkeyInfo(PU16 keyCode, PU16 keyType)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	*keyCode = currKeyCode;
	*keyType = currKeyType;
}

/**************************************************************

	FUNCTION NAME		: SetkeyInfo()

  	PURPOSE				: Set key code & key type of last key event.

	INPUT PARAMETERS	: U16 keyCode, U16 keyType

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetkeyInfo(U16 keyCode, U16 keyType)
{
	currKeyCode = keyCode;
	currKeyType = keyType;
}

/**************************************************************

	FUNCTION NAME		: StartTimer()

  	PURPOSE				: to set a timer

	INPUT PARAMETERS	: U16 timerid, U16 delay, FuncPtr funcPtr

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void StartTimer(U16 timerid, U32 delay, FuncPtr funcPtr)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	ASSERT(delay != 0);
	StartMyTimerInt(timerid, delay, (oslTimerFuncPtr)funcPtr, 0);   
}

/**************************************************************

	FUNCTION NAME		: StopTimer()

  	PURPOSE				: to reset a timer

	INPUT PARAMETERS	: U16 timerid

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void StopTimer(U16 timerid)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	StopMyTimerInt(timerid);     
}

/*****************************************************************
	FUNCTION NAME		: MaskItems()

	PURPOSE				:

	INPUT PARAMETERS	: U16 *maskList, U8 noOfItems, U32 maskingWord

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8

 

*******************************************************************/
U8 MaskItems(U16 *maskList, U8 noOfItems, U32 maskingWord)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U32 count=0,i;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	for(i=0;i<noOfItems;i++)
		if(IsBitSet(maskingWord, i)) maskList[count++]=maskList[i];
	return (U8)count;
}

/*****************************************************************
	FUNCTION NAME		: MaskItemsU8()

	PURPOSE				:

	INPUT PARAMETERS	: U8 *maskList, U8 noOfItems, U32 maskingWord

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8

 

*******************************************************************/
U8 MaskItemsU8(U8 *maskList, U8 noOfItems, U32 maskingWord)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U32 count=0,i;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	for(i=0;i<noOfItems;i++)
		if(IsBitSet(maskingWord, i)) maskList[count++]=maskList[i];
	return (U8)count;
}

/*****************************************************************
	FUNCTION NAME		: MaskStringItems()

	PURPOSE				:

	INPUT PARAMETERS	: U8 **maskList, U8 noOfItems, U32 maskingWord

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8

 

*******************************************************************/
U8 MaskStringItems(U8 **maskList, U8 noOfItems, U32 maskingWord)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U32 count=0,i;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	for(i=0;i<noOfItems;i++)
		if(IsBitSet(maskingWord, i)) maskList[count++]=maskList[i];
	return (U8)count;
}

/*****************************************************************
	FUNCTION NAME		: MaskHiliteItems()

	PURPOSE				:

	INPUT PARAMETERS	: U16 currParentID, U32 maskingWord

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8

 

*******************************************************************/
U8 MaskHiliteItems(U16 curr_parent_ID, U32 maskingWord)
{

   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8 count=0,noOfChild,i;
	U16 hiliteItemID[32];
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	noOfChild=(U8)GetNumOfChild(curr_parent_ID);
	for(i=0;i<noOfChild;i++)
		hiliteItemID[i]=GetSeqItemId((U16)curr_parent_ID, (S16)i);
	for(i=0;i<noOfChild;i++)
		if(IsBitSet(maskingWord, i))
			maxHiliteInfo[hiliteItemID[count++]].entryFuncPtr=maxHiliteInfo[hiliteItemID[i]].entryFuncPtr;
	return (U8)count;

}

//CSD added by JL 040302 for interrupt handle preprocess
/**************************************************************

	FUNCTION NAME		: SetInterruptEventHandler()

  	PURPOSE				: sets interrupt event handler

	INPUT PARAMETERS	: FuncPtr funcPtr, U16 eventID

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetInterruptEventHandler(PsIntFuncPtr funcPtr, PsIntFuncPtr postfuncPtr, U32 eventID)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 count = 0;
	pBOOL isNewEvent = TRUE;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_SETINTP_HDLR, eventID));

	for(count = 0; count < maxInterruptEvent; count++)
	{
		if(interruptEventHandler[count].eventID == eventID)
		{
			isNewEvent = FALSE;
			break;
		}
	}

	if(isNewEvent)
	{
		maxInterruptEvent++;
		MMI_ASSERT(maxInterruptEvent<MAX_INTERRUPT_EVENT);
	}

	interruptEventHandler[count].eventID = eventID;
	interruptEventHandler[count].entryIntFuncPtr = funcPtr;
	interruptEventHandler[count].postIntFuncPtr = postfuncPtr;	
}

/**************************************************************

	FUNCTION NAME		: void SetGroupInterruptEventHandler()

  	PURPOSE				: sets the curent screen same group handler to the group pf events

	INPUT PARAMETERS	: FuncPtr funcPtr, PU16 eventsID, U8 len

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetGroupInterruptEventHandler(PsIntFuncPtr funcPtr, PsIntFuncPtr postfuncPtr, PU16 eventsID, U8 len)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8 count;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	for(count=0;count<len;count++)
	{
            MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_SETGRPINTP_HDLR, eventsID[count]));
	    SetInterruptEventHandler(funcPtr, postfuncPtr, eventsID[count]);
	}
}

/**************************************************************

	FUNCTION NAME		: ClearInterruptEventHandler( )

  	PURPOSE				: clears interrupt event handler

	INPUT PARAMETERS	: U16 eventID

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ClearInterruptEventHandler(U32 eventID)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_CLRINTP_HDLR, eventID));
	SetInterruptEventHandler(NULL, NULL, eventID);
}

/**************************************************************

	FUNCTION NAME		: ClearAllInterruptEventHandler(void)

  	PURPOSE				: clears all the interrupt event handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ClearAllInterruptEventHandler(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 count = 0;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EVENT_CLRALLINTP_HDLR));
	
	for(count = 0; count < maxInterruptEvent; count++)
	{
		interruptEventHandler[count].eventID = 0;
		interruptEventHandler[count].entryIntFuncPtr = NULL;
		interruptEventHandler[count].postIntFuncPtr = NULL;
	}
	maxInterruptEvent = 0;

}

/**************************************************************

	FUNCTION NAME		: GetActiveScreenId(void)

  	PURPOSE				: Get current new screen ID

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U16 GetActiveScreenId (void)
{
	 
	//return currExitScrnID;
	return currTopScrnID;
}

void SetActiveScreenId (U16 scrnId)
{
	currTopScrnID = scrnId;
}
#ifdef __MMI_UI_TAB_PANE__
/**************************************************************

	FUNCTION NAME		: GetCurrEntryFuncPtr(void)

  	PURPOSE				: Get current entry function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
FuncPtr GetCurrEntryFuncPtr(void)
{
	return currEntryFuncPtr;
}
/**************************************************************

	FUNCTION NAME		: SetCurrEntryFuncPtr(void)

  	PURPOSE				: Sets current entry function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetCurrEntryFuncPtr(FuncPtr ptr)
{
	currEntryFuncPtr=ptr;
}
/**************************************************************

	FUNCTION NAME		: GetCurrExitFuncPtr(void)

  	PURPOSE				: Get current exit function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
FuncPtr GetCurrExitFuncPtr(void)
{
	return currExitFuncPtr;
}
/**************************************************************

	FUNCTION NAME		: SetCurrExitFuncPtr(void)

  	PURPOSE				: Sets current exit function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetCurrExitFuncPtr(FuncPtr ptr)
{
	currExitFuncPtr=ptr;
}
#endif

/**************************************************************

	FUNCTION NAME		: ExecuteItemHiliteHandler

  	PURPOSE				: Execute the hilite handler of specified menu item

	INPUT PARAMETERS	: hiliteItemID

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExecuteItemHiliteHandler(U16 hiliteItemID)
{
	if ( maxHiliteInfo[hiliteItemID].entryFuncPtr != NULL )
		maxHiliteInfo[hiliteItemID].entryFuncPtr();
}

