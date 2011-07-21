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
 *	SAT.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   SIM application toolkit related functions
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
 *
 *
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

/**************************************************************

	FILENAME	: SAT.c

  	PURPOSE		: SAT Application

 

 

	DATE		: Mar 24,03

**************************************************************/
#ifndef __SAT_MMI_UNIT_TEST__
//For Icon Debug
//#define __SAT_MMI_UNIT_TEST__
#endif

#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "l4dr.h"

#include "frameworkstruct.h"
//micha0125,  modify key event handler, change on all IsInSATScreen.
#include"keybrd.h"
#include "satdefs.h"
#include "satstruct.h"
#include "satgprots.h"
#include "satprots.h"
#include "protocolevents.h"
#include "timerevents.h"
#include "callmanagementstruct.h"
#include "callmanagementgprot.h"
#include "gsm7bitdefaultalphabet.h"
#include "commonscreens.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "sscstringhandle.h"
#include "gdi_include.h"
#include "lcd_sw_rnd.h"
#include "queuegprot.h"
#include "bootup.h"
#include "poweronchargerprot.h"
#include "simdetectiongexdcl.h"
#include "sat_def.h"
#include "wgui_categories_inputs.h"
//micha0901
#include "wgui_categories_popup.h"
#include "callmanagementstruct.h"
#include "callstructuremanagementprot.h"
//micha1012
#include "alarmframeworkprot.h"
#include "gpioinc.h"
#include "stack_ltlcom.h"             //tangjie add begin 20060801
#include "mmiapi.h"
#include "servicedefs.h"

#include "gbhmi.h"  //added by zhoumn for the bug of editor full @2007/06/16

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif

#undef __NEWSIMULATOR
#include "mmi_trace.h"
#include "simdetectiongprot.h"
#include "debuginitdef.h"
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
#include "wap_ps_struct.h"
#endif
#define SAT_OS_DEST_ID MOD_L4C + gSATDisplayText.CMDfromSIM - SAT_CMD_SIM1

SATSETUPMENU gSATMainMenu[MMI_SIM_NUMBER];
SATSUBMENU gSATSubMenu;
SATDISPLAYTEXT gSATDisplayText;
SATGETINPUT gSATGetInput;
SATGETINKEY gSATGetInkey;
SATSENDSMS gSATSendSMS;
SATCALLSETUP gSATCallSetup;
SATIMAGERECORD gSATImageRecord;
SATSENDSS gSATSendSS;
SATSENDUSSD gSATSendUSSD;
SATCONTEXT gSATContext;

SATSETUPMENU *g_pSATMainMemu = gSATMainMenu;
#if 0

//SATPLAYTONE gSATPlayTone;
//SATRUNATCOMMAND gSATRunATCommand;
//SATLAUNCHBROWSER gSATLaunchBrowser;
//SATSENDDTMF gSATSendDTMF;
//micha1016
//SATDISPLAYIDLETEXT gSATDisplayIdleText;
#endif

SATLAUNCHBROWSER gSATLaunchBrowser;
extern U8 isIdleScreenReached;
extern s16 MMI_current_input_ext_type;  

void mmi_sat_release_menu_alpha(U8 *palpha, U8 *pDestalpha, U16 *npDestlen);

extern void GBSetInputboxLSKFunction(FuncPtr f); 		//yuxiang 07.6.22
extern void mmi_pen_editor_switch_input_method(void);
extern void PsCBackSetCallFeatures(void* info);
#if 0		// yuxiang B 07.6.23
#ifdef __SATCE__
SATOPENCHANNEL satOpenChannel;
SATCLOSECHANNEL satCloseChannel;
SATSENDDATA satSendData;
SATRECVDATA satRecvData;
#endif
#endif 	// yuxiang E 07.6.23
#ifdef __SAT_MMI_UNIT_TEST__
mmi_smu_read_sim_rsp_struct EFback1a = {0,0,{L4C_OK,0}, 
							10, 
							{0x01,	0x08,	0x08,	0x11,	0x4F,	0x04,	0x00,	0x00,	0x00,	0x0A}};

mmi_smu_read_sim_rsp_struct EFback2a = {0,0,{L4C_OK,0}, 
							10, 
							{0x08,	0x08,	0xFF,	0x03,	0xA5,	0x99,	0x99,	0xA5,	0xC3,	0xFF}};


mmi_smu_read_sim_rsp_struct EFback1 = {0,0,{L4C_OK,0}, 
							10, 
							{0x01,	0x08,	0x08,	0x21,	0x4F,	0x02,	0x00,	0x00,	0x00,	0x1F}};

mmi_smu_read_sim_rsp_struct EFback2 = {0,0,{L4C_OK,0}, 
							31, 
							{0x08,	0x08,	0x02,	0x03,	0x00,	0x16,	0xAA,	0xAA,	0x80,	0x02,	0x85,	0x42,
							0x81,	0x42,	0x81,	0x42,	0x81,	0x52,	0x80,	0x02,	0xAA,	0xAA,	0xFF,	0x00,
							0x00,	0x00,	0xFF,	0x00,	0x00,	0x00,	0xFF}};
#endif

#ifdef __MMI_MULTI_SIM__
U8 gSATActiveSimID = 0;
U8 gSATIndSimID = 0;
void SATSetActiveSimID(U8 nSimID)
{
	mmi_trace(g_sw_SAT, "SAT SATSetActiveSimID, nSimID is %d",nSimID);
	MMI_ASSERT(nSimID < MMI_SIM_NUMBER);
	gSATActiveSimID = nSimID;
	g_pSATMainMemu = &(gSATMainMenu[nSimID]);
}

U8 SATGetActiveSimID(void)
{
	return gSATActiveSimID;
}

void SATSetIndSimID(U8 nSimID)
{
	gSATIndSimID = nSimID;
	g_pSATMainMemu = &(gSATMainMenu[nSimID]);
}
#endif
static void SAT3000Timer(void);
static void SAT500Timer(void);

/*****************************************************************************
 * FUNCTION
 *  InitSimAppToolkit
 * DESCRIPTION
 *  Initialises variables & Protocol Handlers for SAT
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_frm_sms_sim_refresh_ind(U16);
void InitSimAppToolkit(void)
{
    U8 i = 0;

#ifdef __SAT_MMI_UNIT_TEST__
    SATSetupIdleDisplayTest();
#endif

#ifdef __MMI_MULTI_SIM__
    memset(gSATMainMenu, 0x00, sizeof(gSATMainMenu));
#endif

    for(i = 0; i < MMI_SIM_NUMBER; i++)
    {
        SetProtocolEventHandlerExt(SATSetupMenuInd,PRT_MMI_SAT_SETUP_MENU_IND, i);
        SetProtocolEventHandlerExt(SATSetupMenuRsp, PRT_MMI_SAT_SETUP_MENU_RES_RSP, i);
        SetProtocolEventHandlerExt(SATMenuSelectRsp, PRT_MMI_SAT_MENU_SELECT_RSP, i);
        SetProtocolEventHandlerExt(SATSelectItemInd, PRT_MMI_SAT_SELECT_ITEM_IND, i);
        SetProtocolEventHandlerExt(SATSelectItemRsp, PRT_MMI_SAT_SELECT_ITEM_RES_RSP, i);
        SetProtocolEventHandlerExt(SATDisplayTextInd,PRT_MMI_SAT_DISPLAY_TEXT_IND, i);
        SetProtocolEventHandlerExt(SATDisplayTextRsp, PRT_MMI_SAT_DISPLAY_TEXT_RES_RSP,i);
        SetProtocolEventHandlerExt(SATGetInputInd, PRT_MMI_SAT_GET_INPUT_IND,i);
        SetProtocolEventHandlerExt(SATGetInputRsp, PRT_MMI_SAT_GET_INPUT_RES_RSP, i);
        SetProtocolEventHandlerExt(SATGetInkeyInd, PRT_MMI_SAT_GET_INKEY_IND, i);
        SetProtocolEventHandlerExt(SATGetInkeyRsp, PRT_MMI_SAT_GET_INKEY_RES_RSP, i);
        SetProtocolEventHandlerExt(SATCallSetupInd,PRT_MMI_SAT_CALL_SETUP_IND, i);
        SetProtocolEventHandlerExt(SATCallSetupStage1Rsp, PRT_MMI_SAT_CALL_SETUP_STAGE1_RSP, i);
        SetProtocolEventHandlerExt(SATCallSetupStage2Rsp, PRT_MMI_SAT_CALL_SETUP_STAGE2_RSP, i);
        SetProtocolEventHandlerExt(SATSendSMSInd,PRT_MMI_SAT_SEND_SMS_IND, i);
        SetProtocolEventHandlerExt(SATSendSMSStage1Rsp, PRT_MMI_SAT_SEND_SMS_STAGE1_RSP, i);
        SetProtocolEventHandlerExt(SATSendSMSStage2Rsp, PRT_MMI_SAT_SEND_SMS_STAGE2_RSP, i);
        SetProtocolEventHandlerExt(SATNoFurtherCommandInd,PRT_MMI_SAT_NO_OTHER_CMD_IND, i);
        SetProtocolEventHandlerExt(SATSendSSInd,PRT_MMI_SAT_SEND_SS_IND, i);
        SetProtocolEventHandlerExt(SATSendSSStage1Rsp,PRT_MMI_SAT_SEND_SS_STAGE1_RSP, i);
        SetProtocolEventHandlerExt(SATSendSSStage2Rsp,PRT_MMI_SAT_SEND_SS_STAGE2_RSP, i);
        SetProtocolEventHandlerExt(SATSendUSSDInd,PRT_MMI_SAT_SEND_USSD_IND, i);
        SetProtocolEventHandlerExt(SATSendUSSDStage1Rsp,PRT_MMI_SAT_SEND_USSD_STAGE1_RSP, i);
        SetProtocolEventHandlerExt(SATSendUSSDStage2Rsp,PRT_MMI_SAT_SEND_USSD_STAGE2_RSP, i);
        SetProtocolEventHandlerExt(SATLaunchBrowserInd, PRT_MMI_SAT_LAUNCH_BROWSER_IND,i);
        SetProtocolEventHandlerExt(SATLaunchBrowserRsp, PRT_MMI_SAT_LAUNCH_BROWSER_RES_RSP,i);
    }

#ifdef __MMI_MULTI_SIM__
    gSATContext.CMDfromSIM = SAT_CMD_NONE;
    gSATContext.satIndex = SAT_CMD_NONE;	
    gSATDisplayText.CMDfromSIM = SAT_CMD_NONE;
    gSATSubMenu.CMDfromSIM = SAT_CMD_NONE;
#endif

#if 0
    SetProtocolEventHandler(SATMMIInfoInd,PRT_MMI_SAT_MMI_INFO_IND);
    SetProtocolEventHandler(SATPlayToneInd,PRT_MMI_SAT_PLAY_TONE_IND);
    SetProtocolEventHandler(SATPlayToneRsp,PRT_MMI_SAT_PLAY_TONE_RES_RSP);
    SetProtocolEventHandler(SATSendReadSimFileRsp,PRT_MMI_SMU_READ_SIM_RSP);
    SetProtocolEventHandler(SATLangNotifyInd,PRT_MMI_SAT_LANG_NOTIFY_IND);
    SetProtocolEventHandler(SATLaunchBrowserInd,PRT_MMI_SAT_LAUNCH_BROWSER_IND);
    SetProtocolEventHandler(WAPLaunchBrowserRsp,PRT_WAP_MMI_LAUNCH_BROWSER_RSP);	
    SetProtocolEventHandler(SATLaunchBrowserRsp,PRT_MMI_SAT_LAUNCH_BROWSER_RES_RSP);
    SetProtocolEventHandler(SATEVDLBrowserTerminationInd,PRT_WAP_MMI_BROWSER_TERMINATION_IND);
    SetProtocolEventHandler(SATSendDTMFInd,PRT_MMI_SAT_SEND_DTMF_IND);
    SetProtocolEventHandler(SATSendDTMFStage1Rsp,PRT_MMI_SAT_SEND_DTMF_STAGE1_RSP);
    SetProtocolEventHandler(SATSendDTMFStage2Rsp,PRT_MMI_SAT_SEND_DTMF_STAGE2_RSP);
    SetProtocolEventHandler(SATSetupIdleDisplayInd,PRT_MMI_SAT_SETUP_IDLE_DISPLAY_IND);
    SetProtocolEventHandler(SATRunATCommandInd,PRT_MMI_SAT_RUN_AT_COMMAND_IND);

    SetProtocolEventHandler(SATNoFurtherCommandInd,PRT_MMI_SAT_NO_OTHER_CMD_IND);

    SetProtocolEventHandler(SATSIMFileChangeInd,PRT_MMI_SAT_SIM_FILE_CHANGE_IND);

#ifdef __SATCE__
    SetProtocolEventHandler(SATOpenChannelInd,MSG_ID_MMI_SAT_OPEN_CHANNEL_IND);
    SetProtocolEventHandler(SATOpenChannelRes,MSG_ID_MMI_SAT_OPEN_CHANNEL_RSP);
    SetProtocolEventHandler(SATCloseChannelInd,MSG_ID_MMI_SAT_CLOSE_CHANNEL_IND);
    SetProtocolEventHandler(SATSendDataInd,MSG_ID_MMI_SAT_SEND_DATA_IND);
    SetProtocolEventHandler(SATSendDataRes,MSG_ID_MMI_SAT_SEND_DATA_RSP);
    SetProtocolEventHandler(SATRecvDataInd,MSG_ID_MMI_SAT_RECV_DATA_IND);
    SetProtocolEventHandler(SATRecvDataRes,MSG_ID_MMI_SAT_RECV_DATA_RSP);
#endif	
#endif 

    gSATContext.getImageStage = (U16)SAT_IMAGE_IDLE_CMD;
    gSATContext.gotStage1Rsp = 1;
    gSATContext.lastSATCmd = SAT_NONE_CMD;
    gSATContext.simFuncPtr[0] = mmi_frm_sms_sim_refresh_ind;  
}

/*****************************************************************************
 * FUNCTION
 *  ExitVariables
 * DESCRIPTION
 *  Reinitialises the global varibales of SAT while
 *  exiting
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitVariables(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gSATContext.isInSATMenu = 0;
    
    for(i = 0; i < MMI_SIM_NUMBER; i++)
    {    
        gSATMainMenu[i].selectedMenuItem = 0; 
    }
    
    ClearIsInSATScreen();
}

/**************************************************************

	FUNCTION NAME		: SATNoFurtherCommandInd

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SATNoFurtherCommandInd(void *msgPtr)
{
    SATMessageWaitTimer();
}

/**************************************************************

	FUNCTION NAME		: SATFileChangeSmsCheck

  	PURPOSE			: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS			: nil

 

**************************************************************/
static pBOOL SATFileChangeSmsCheck(mmi_sat_sim_file_change_ind_struct *simChange)
{
	U8 i;
	pBOOL rtn = MMI_FALSE;

	if(simChange->is_full_changed)
		return MMI_TRUE;

	for(i=0; i<simChange->num_of_file; i++)
	{
		if(simChange->file_list[i] == FILE_SMSS_IDX
		|| simChange->file_list[i] == FILE_SMSP_IDX
		|| simChange->file_list[i] == FILE_SMS_IDX
		|| simChange->file_list[i] == FILE_CBMI_IDX
		|| simChange->file_list[i] == FILE_CBMID_IDX
		|| simChange->file_list[i] == FILE_CBMIR_IDX
	#ifdef __CPHS__
		|| simChange->file_list[i] == FILE_VM_WAIT_IDX
		|| simChange->file_list[i] == FILE_MAILBOX_NUM_IDX
	#endif
		|| simChange->file_list[i] == FILE_ELP_IDX
		|| simChange->file_list[i] == FILE_LP_IDX)
		{
			rtn = MMI_TRUE;
			break;
		}
	}

	return rtn;
}

//micha0201
/**************************************************************

	FUNCTION NAME		: SATSIMFileChangeInd

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SATSIMFileChangeInd(void *msgPtr)
{
        U8 i;
	mmi_sat_sim_file_change_ind_struct *simChange;

	simChange = ( mmi_sat_sim_file_change_ind_struct*)msgPtr;

	if (simChange->refresh_type == TYPE_SIM_RESET)
	{
		GoToSATRebootScreen();
	}
	 
	else if( SATFileChangeSmsCheck(simChange) )
	{
		gSATContext.simFuncPtr[0](simChange->refresh_type); /* SMS */
	}
	 
	else
	{
		for(i=1; i<MAX_FILE_CHANGE_NOTIFY_CALLBACK; i++)
		{
			if (gSATContext.simFuncPtr[i] != NULL)
				gSATContext.simFuncPtr[i](simChange->refresh_type);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: GoToSATRebootScreen

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void GoToSATRebootScreen(void)
{
	EntryNewScreen(SCR_ID_SAT_WAITSCREEN, NULL, GoToSATRebootScreen, NULL);
	ShowCategory65Screen((PU8)GetString(SAT_WAIT_STRING_ID), SAT_WAIT_IMAGE_ID, NULL);
	ClearAllKeyHandler();
	SetKeyHandler(0,KEY_END,KEY_EVENT_DOWN);
	SetKeyHandler(0,KEY_END,KEY_EVENT_UP);	

	StartTimer(SAT_REFRESH_REBOOT_TIMER, 1000, SATRefreshRebootTimeOut);
}

/**************************************************************

	FUNCTION NAME		: SATRefreshRebootTimeOut

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SATRefreshRebootTimeOut(void)
{
	//AlmATPowerReset( 0, 0);  // zrx del 20060612
}


/**************************************************************

	FUNCTION NAME		: IsSATPresent

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
U8 IsSATPresent(U8 nSimID)
{
	#ifdef __MMI_MULTI_SIM__
	if(MTPNP_PFAL_Is_Card_Usable(nSimID))
	#endif
	{
		return(gSATContext.isSATPresent[nSimID]);
	}
	
	return 0;
}



/**************************************************************

	FUNCTION NAME		: IsInSATScreen

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
U8 IsInSATScreen(void)
{
	return(gSATContext.isInSATScreen);
}


/**************************************************************

	FUNCTION NAME		: IsInSATCall

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
U8 IsInSATCall(void)
{
	return(gSATContext.isInSATCall);
}


//micha0831
/**************************************************************

	FUNCTION NAME		: SATCallTerminate

  	PURPOSE				: Called by Call Manage

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SATCallTerminate(void)
{
	if (gSATContext.isInSATCall) {
		gSATContext.isInSATCall = 0;
		if(!gSATCallSetup.isSentRsp) {
			if(gSATCallSetup.exitCause==SAT_CALL_DISCONNECT_BEFORE_CONNECT)
			{
				TerminalResponseCallSetup(SAT_USER_CLEAR_DOWN_CALL_BEFORE_CONNECTION_RELEASE, 0,gSATCallSetup.cmdDetail);
			}
			else
			{
				TerminalResponseCallSetup(SAT_CMD_PERFORMED_SUCCESSFULLY, 0, gSATCallSetup.cmdDetail);
			}
		}
	}
}

/**************************************************************

	FUNCTION NAME		: GetSATIdleText

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
U8 *GetSATIdleText(void)
{
	return(gSATContext.idleTextPtr);
}


/**************************************************************

	FUNCTION NAME		: GetSATIdleIcon

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
U8 *GetSATIdleIcon(void)
{
	return(gSATContext.idleIconPtr);
}


/**************************************************************

	FUNCTION NAME		: GetPostponedSATScreen

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
FuncPtr GetPostponedSATScreen(void)
{
	return(gSATContext.postponedSATScreen);
}


/**************************************************************

	FUNCTION NAME		: ClearPostponedSATScreen

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void ClearPostponedSATScreen(void)
{
	gSATContext.postponedSATScreen = NULL;
}

/**************************************************************

	FUNCTION NAME		: GetSATMainMenuAlphaID

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
U8 *GetSATMainMenuAlphaID(void)
{
	return(g_pSATMainMemu->alphaId);
}


/*****************************************************************************
 * FUNCTION
 *  SATHistoryHandler
 * DESCRIPTION
 *  Calls Corresponding Function in History
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SATHistoryHandler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetIsInSATScreen();
    gSATContext.guiBufferPointer = GetCurrGuiBuffer(SCR_ID_SAT_HISTORY);
    (*gSATContext.historyFuncPtr) ();
}

/*****************************************************************************
 * FUNCTION
 *  SATSaveHistory
 * DESCRIPTION
 *  Saves SAT in History
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SATSaveHistory(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GenericExitScreen(SCR_ID_SAT_HISTORY, SATHistoryHandler);
    ClearIsInSATScreen();
}

/*****************************************************************************
 * FUNCTION
 *  SATMessageWaitTimer
 * DESCRIPTION
 *  Timer CallBack Function called by when the SAT dows not recieve any
 *  command from SIM for 3 secs
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SATMessageWaitTimer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SATStopWaitTimer();

    mmi_trace(g_sw_SAT, "SAT Func: %s gSATContext.isInSATScreen=%d, gSATContext.isInSATMenu=%d", __FUNCTION__, gSATContext.isInSATScreen, gSATContext.isInSATMenu);

	if(gSATContext.isInSATScreen || gSATContext.isInSATMenu)
    {
        gSATContext.isInSATScreen = 1;
        
#ifdef __MMI_CLAMCLOSE_BACK2IDLE__
        if (IsClamClose()) 
        {
            DisplayIdleScreen();
            return;
        }
#endif

        if (gSATContext.isInSATMenu)
        {
#ifdef __MMI_MULTI_SIM__
            SATSetActiveSimID(gSATContext.satIndex - SAT_CMD_SIM1);
#endif
            gSATContext.historyFuncPtr = GoToSATMainMenu;

            if(IsScreenPresent(SCR_ID_SAT_HISTORY)) 
            {
                GoBackToHistory(SCR_ID_SAT_HISTORY);
            }
            else 
            {
#ifdef __MMI_MULTI_SIM__
                SATSetActiveSimID(gSATContext.satIndex - SAT_CMD_SIM1);
#endif
                GoToSATMainMenu();
            }
        }
        else
        {
            if(IsScreenPresent(SCR_ID_SAT_HISTORY)) 
            {
                mmi_trace(g_sw_SAT, "SAT Func: %s GoBackToHistory(SCR_ID_SAT_HISTORY)", __FUNCTION__);
                GoBackToHistory(SCR_ID_SAT_HISTORY);
            }
            else 
            {
                mmi_trace(g_sw_SAT, "SAT Func: %s GoBackToHistory()", __FUNCTION__);
                ClearIsInSATScreen();
                GoBackHistory();
            }
        }
    }
    else
    {
        gSATContext.historyFuncPtr = GoToSATMainMenu;

        if(!gSATContext.isInSATMenu)
        {
            ClearIsInSATScreen();
            DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  StartSATTimerNoResponsefromUser
 * DESCRIPTION
 *  Starts No Response Timer and calls the passed
 *  function if no key is pressed in that duration
 * PARAMETERS
 *  FuncPtr     [IN]    To be called on Timer Expiry
 * RETURNS
 *  void
 *****************************************************************************/
void StartSATTimerNoResponsefromUser(FuncPtr pfunc)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_SAT," StartSATTimerNoResponsefromUser gSATContext.lastSATCmd=%d", gSATContext.lastSATCmd);
   
    gSATContext.noResponseFuncPtr = pfunc;

    if( gSATContext.lastSATCmd == SAT_DISPLAY_TEXT_CMD && gSATDisplayText.clearTextType == SAT_CLEAR_AFTER_DELAY)
    {
        StartTimer(SAT_TERMINAL_RESP_TIMER, SAT_DISPLAY_TEXT_DELAY_DURATION, gSATContext.noResponseFuncPtr);
        return;
    }

#ifdef MMI_ON_HARDWARE_P
    if (is_test_sim()) 
    {
        StartTimer(SAT_TERMINAL_RESP_TIMER, 60000, gSATContext.noResponseFuncPtr);
    }
    else 
    {
        StartTimer(SAT_TERMINAL_RESP_TIMER, SAT_NO_RESPONSE_TIMER_DURATION, gSATContext.noResponseFuncPtr);
    }
#else
    StartTimer(SAT_TERMINAL_RESP_TIMER, SAT_NO_RESPONSE_TIMER_DURATION, gSATContext.noResponseFuncPtr);
#endif
}

/*****************************************************************************
 * FUNCTION
 *  StopSATTimerNoResponsefromUser
 * DESCRIPTION
 *  Stops No Response Timer
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void StopSATTimerNoResponsefromUser(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    StopTimer(SAT_TERMINAL_RESP_TIMER);
}

/*****************************************************************************
 * FUNCTION
 *  TemporaryStopSATTimerNoResponsefromUser
 * DESCRIPTION
 *  Stops No Response Timer when any key is pressed
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void TemporaryStopSATTimerNoResponsefromUser(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    StopTimer(SAT_TERMINAL_RESP_TIMER);
}

/*****************************************************************************
 * FUNCTION
 *  RestartSATTimerNoResponsefromUser
 * DESCRIPTION
 *  ResStarts No Response Timer when any key is released
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RestartSATTimerNoResponsefromUser(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(gSATContext.noResponseFuncPtr != NULL )
    {
        if( gSATContext.lastSATCmd == SAT_DISPLAY_TEXT_CMD && gSATDisplayText.clearTextType == SAT_CLEAR_AFTER_DELAY )
        {
            StartTimer(SAT_TERMINAL_RESP_TIMER, SAT_DISPLAY_TEXT_DELAY_DURATION, gSATContext.noResponseFuncPtr);
            return;
        }

#ifdef MMI_ON_HARDWARE_P
        if (is_test_sim()) 
        {
            StartTimer(SAT_TERMINAL_RESP_TIMER, 60000, gSATContext.noResponseFuncPtr);
        }
        else 
        {
            StartTimer(SAT_TERMINAL_RESP_TIMER, SAT_NO_RESPONSE_TIMER_DURATION, gSATContext.noResponseFuncPtr);
        }
#else
        StartTimer(SAT_TERMINAL_RESP_TIMER, SAT_NO_RESPONSE_TIMER_DURATION, gSATContext.noResponseFuncPtr);
#endif
    }
}

/************************************************************

  	FUNCTION NAME		: StartSATTimerHelp

  	PURPOSE				: Starts Help Timer and calls the passed
							function if no key is pressed in that duration
	
	INPUT PARAMETERS	: FuncPtr: function to be called on Timer Expiry

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
void StartSATTimerHelp(FuncPtr pfunc)
{
	gSATContext.helpFuncPtr=pfunc;
	StartTimer(SAT_HELP_TIMER,SAT_HELP_TIMER_DURATION,gSATContext.helpFuncPtr);
}

/************************************************************

  	FUNCTION NAME		: StopSATTimerHelp

  	PURPOSE				: Stops Help Timer
	
	INPUT PARAMETERS	: FuncPtr: function to be called on Timer Expiry

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
void StopSATTimerHelp(void)
{
	StopTimer(SAT_HELP_TIMER);
}

/*****************************************************************************
 * FUNCTION
 *  TemporaryStopSATTimerHelp
 * DESCRIPTION
 *  Stops Help Timer when any key is pressed
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void TemporaryStopSATTimerHelp(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    StopTimer(SAT_HELP_TIMER);
}

/*****************************************************************************
 * FUNCTION
 *  RestartSATTimerHelp
 * DESCRIPTION
 *  Restarts Help Timer when any key is released
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RestartSATTimerHelp(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(gSATContext.helpFuncPtr != NULL)
    {
        StartTimer(SAT_HELP_TIMER, SAT_HELP_TIMER_DURATION, gSATContext.helpFuncPtr);
    }
}

/*****************************************************************************
 * FUNCTION
 *  ExitSATToIdle
 * DESCRIPTION
 *  Takes the user back to Idle Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitSATToIdle(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_SAT,"SAT FUNC:%s, isInSATMenu = %d", __FUNCTION__, gSATContext.isInSATMenu);

    if(isInCall())
    {
        HangupAllCalls();
    }
    else
    {

        gSATContext.isInSATMenu = 0;
        ClearIsInSATScreen();
        SATStopWaitTimer();
        StopTimer(J2ME_TIMEALARM_TIMER);
        DisplayIdleScreen();
    }
}

/*****************************************************************************
 * FUNCTION
 *  SATPleaseWaitScreen
 * DESCRIPTION
 *  Displays Please Wait Screen just sets the variables
 * PARAMETERS
 *  string      [IN]    String to be displayed in Please Wait Screen
 *  image       [IN]    Image to be displayed in Please Wait Screen
 * RETURNS
 *  void
 *****************************************************************************/
void SATPleaseWaitScreen(U16 string, U16 image)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gSATContext.waitString = string;
    gSATContext.waitImage = image;
    PleaseWaitActualScreen();
}

/**************************************************************

	FUNCTION NAME		: SATMessageWaitExit

  	PURPOSE				: Wait too long and return to previous menu

	INPUT PARAMETERS	: NULL

	OUTPUT PARAMETERS	: NULL

	RETURNS				: NULL

 

**************************************************************/
// yuxiang B	07.6.19
void SATMessageWaitExit(void)
{
	TRACE_EF(g_sw_SAT," SATMessageWaitExit");
	StopTimer(J2ME_NOT_READY_TIMER);
#ifdef __MMI_MULTI_SIM__
	SATSetActiveSimID(gSATContext.satIndex - SAT_CMD_SIM1);
#endif
	GoToSATMainMenu();
}

/*****************************************************************************
 * FUNCTION
 *  PleaseWaitActualScreen
 * DESCRIPTION
 *  Displays Wait Screen on MMI, Entry function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PleaseWaitActualScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_SAT," PleaseWaitActualScreen isInSATScreen = %d", gSATContext.isInSATScreen);

    if (gSATContext.isInSATScreen)
    {	
        gSATContext.exitCondition = &gSATContext.waitTimeExpired;
        EntryNewScreen(SCR_ID_SAT_WAITSCREEN, SATGenericExitHandler, NULL, NULL);
        ClearHighlightHandler();
        ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);	

#ifdef __MMI_SAT_PLEASE_WAIT_ONLY_TEXT__
        ShowCategory151Screen(0, 0, 0, 0, 0, 0, (PU8)GetString(gSATContext.waitString), 0, NULL);
#else
        if(gSATContext.waitImage == 0)
        {
            ShowCategory151Screen(0, 0, 0, 0, 0, 0, (PU8)GetString(gSATContext.waitString), 0, NULL);
        }
        else
        {
            ShowCategory9Screen(gSATContext.waitString, gSATContext.waitImage, NULL);
        }
#endif /* __MMI_SAT_PLEASE_WAIT_ONLY_TEXT__ */

        SetKeyHandler(NULL, KEY_END, KEY_EVENT_DOWN);
    }
    
    gSATContext.historyFuncPtr = PleaseWaitActualScreen;
    AlmDisableExpiryHandler();
    StartTimer(SAT_MESSAGE_WAIT_TIMER, SAT_MESSAGE_WAIT_TIMER_DURATION, SATMessageWaitTimer);

    gSATContext.waitTimeExpired = 0;
}

/*****************************************************************************
 * FUNCTION
 *  SATStopWaitTimer
 * DESCRIPTION
 *  Stops Please Wait Timer, Timer is started when Please
 *  Wait Screen is called
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SATStopWaitTimer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    AlmEnableExpiryHandler();
    StopTimer(SAT_MESSAGE_WAIT_TIMER);
    StopTimer(SAT_TERMINAL_RESP_TIMER);	
    StopTimer(J2ME_NOT_READY_TIMER);
    gSATContext.waitTimeExpired = 1;
}

/*****************************************************************************
 * FUNCTION
 *  SATSetupMenuInd
 * DESCRIPTION
 *  Protocol Handler for SETUP MENU IND
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATSetupMenuInd(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i;
    U8 *tempDataPtr;
    SATMENUITEM *menuItem;
    mmi_sat_setup_menu_ind_struct *setupMenu;
    U16 convertedLen;
    U8 *palpha = NULL;
    U8 nSimID;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    nSimID = MTPNP_AD_get_protocol_event_src_mode();
#ifdef __MMI_MULTI_SIM__  
    SATSetIndSimID(nSimID);
    gSATContext.CMDfromSIM = SAT_CMD_SIM1 + nSimID;
#endif

    mmi_trace(g_sw_SAT, "SAT SATSetupMenuInd(%d)", nSimID);

    setupMenu = (mmi_sat_setup_menu_ind_struct *)msgPtr;

    if(gSATContext.isSATPresent[nSimID]) 
    {
        FreeMainMenu(g_pSATMainMemu);
    }
    
    memset(g_pSATMainMemu, 0, sizeof(SATSETUPMENU));	
    
    memcpy(g_pSATMainMemu->cmdDetail, setupMenu->cmd_detail, 5*sizeof(U8));
    g_pSATMainMemu->selectedMenuItem = 0;
    g_pSATMainMemu->isHelpInfoAvailable = setupMenu->is_help_info_available;
    g_pSATMainMemu->numOfItem = setupMenu->num_of_item;
    
    g_pSATMainMemu->noItemIconIdList = setupMenu->no_item_icon_id_list;
    g_pSATMainMemu->itemIconListAttr = setupMenu->item_icon_list_attr;
    
#ifdef __SAT_MMI_UNIT_TEST__
    g_pSATMainMemu->isIconAvailable = 1;
    if (g_pSATMainMemu->isIconAvailable) 
    {
        g_pSATMainMemu->IconID = 1;
        g_pSATMainMemu->iconAttr = SAT_ICON_NOT_SELF_EX;
    }
    g_pSATMainMemu->noItemIconIdList = 1;
    g_pSATMainMemu->itemIconListAttr = 1;
    g_pSATMainMemu->noNextActionIndList = 1;
    g_pSATMainMemu->dcsOfAlphaId = MMI_DEFAULT_DCS;
    if (g_pSATMainMemu->isIconAvailable && g_pSATMainMemu->iconAttr == SAT_ICON_SELF_EX) 
    {
        g_pSATMainMemu->alphaIdLen = 0;
    }						
    else 
    {
        g_pSATMainMemu->alphaIdLen = 20;
        palpha=CovertStringForPlatform((PU8) "abcdefghji1234567890", g_pSATMainMemu->alphaIdLen,g_pSATMainMemu->dcsOfAlphaId,&g_pSATMainMemu->alphaIdLen);
        mmi_sat_release_menu_alpha(palpha, g_pSATMainMemu->alphaId, &g_pSATMainMemu->alphaIdLen);
    }
#else
    g_pSATMainMemu->isIconAvailable = setupMenu->is_icon_available;
    if (g_pSATMainMemu->isIconAvailable) 
    {
        g_pSATMainMemu->IconID = setupMenu->icon_info.icon_id;
        g_pSATMainMemu->iconAttr = setupMenu->icon_info.icon_attr;
    }
    
    g_pSATMainMemu->noItemIconIdList = setupMenu->no_item_icon_id_list;
    g_pSATMainMemu->itemIconListAttr = setupMenu->item_icon_list_attr;
    g_pSATMainMemu->noNextActionIndList = setupMenu->no_next_action_ind_list;

    g_pSATMainMemu->dcsOfAlphaId = setupMenu->dcs_of_alpha_id;
    if (g_pSATMainMemu->isIconAvailable && g_pSATMainMemu->iconAttr == SAT_ICON_SELF_EX) 
    {
        g_pSATMainMemu->alphaIdLen = 0;
    }						
    else 
    {
        g_pSATMainMemu->alphaIdLen = setupMenu->no_alpha_id;
        palpha = CovertStringForPlatform(setupMenu->alpha_id, g_pSATMainMemu->alphaIdLen, g_pSATMainMemu->dcsOfAlphaId, &g_pSATMainMemu->alphaIdLen);
        mmi_sat_release_menu_alpha(palpha, g_pSATMainMemu->alphaId, &g_pSATMainMemu->alphaIdLen);
    }
#endif

    if(g_pSATMainMemu->numOfItem == 0)
    {
        SendTerminalResponseSetupMenuEx(MOD_L4C + nSimID, SAT_CMD_PERFORMED_SUCCESSFULLY, g_pSATMainMemu->cmdDetail);
        FreeMainMenu(g_pSATMainMemu);
        gSATContext.isSATPresent[nSimID]=0;
        if(gSATContext.isInSATMenu) 
        {
            SATStopWaitTimer();
            DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);		
            if (GetExitScrnID() == SCR_ID_SAT_HISTORY) 
            {
                ClearExitHandler();
            }
            GoToSATMainMenu();
        }
        return;
    }
    
    if (g_pSATMainMemu->numOfItem > 0)
    {
        g_pSATMainMemu->menuItems = OslMalloc(g_pSATMainMemu->numOfItem * sizeof(SATMENUITEM));
        memset(g_pSATMainMemu->menuItems, 0, g_pSATMainMemu->numOfItem * sizeof(SATMENUITEM));

        menuItem = g_pSATMainMemu->menuItems;
        tempDataPtr = setupMenu->item_data;

        for (i = 0; i < g_pSATMainMemu->numOfItem; i++, menuItem++)
        {
            menuItem->itemLen = (U16) (*(tempDataPtr+1));
            if (menuItem->itemLen) 
            {
                tempDataPtr += 2;
                menuItem->itemId = *tempDataPtr++;
                menuItem->itemDcs = *tempDataPtr++;
                palpha = CovertStringForPlatform(tempDataPtr,menuItem->itemLen, menuItem->itemDcs, &convertedLen);
                mmi_sat_release_menu_alpha(palpha, menuItem->itemData, &convertedLen);

                tempDataPtr += menuItem->itemLen;
                menuItem->itemLen=convertedLen;
            }
            else 
            {
                menuItem->itemLen = 0;
            }

            if (g_pSATMainMemu->noItemIconIdList)
            {         
#ifdef __SAT_MMI_UNIT_TEST__
                menuItem->IconID = 1;
#else
                menuItem->IconID = *(setupMenu->item_icon_id_list + i);
#endif
            }
            
            if (g_pSATMainMemu->noNextActionIndList)
            {
#ifdef __SAT_MMI_UNIT_TEST__
                menuItem->nextAction= SAT_NEXT_ACTION_RESERVED_FOR_TIA;
#else
                menuItem->nextAction= setupMenu->next_action_ind_list[i];
#endif
            }
        }
    }

    SendTerminalResponseSetupMenuEx(MOD_L4C + nSimID, SAT_CMD_PERFORMED_SUCCESSFULLY, g_pSATMainMemu->cmdDetail);

    gSATContext.isSATPresent[nSimID] = 1;
    
    OslMfree(setupMenu->alpha_id);
    OslMfree(setupMenu->item_data);
}

/**************************************************************

	FUNCTION NAME		: SATReturnNextAction

  	PURPOSE				: SAT next action indication

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
PU8 SATReturnNextAction( U8 NextActionID) {

	switch (NextActionID) {
		case SAT_NEXT_ACTION_SETUP_CALL:
			return((PU8)GetString(SAT_NEXT_ACTION_SETUP_CALL_STRING_ID));

		case SAT_NEXT_ACTION_SEND_SS:
			return((PU8)GetString(SAT_NEXT_ACTION_SEND_SS_STRING_ID));

		case SAT_NEXT_ACTION_SEND_USSD:
			return((PU8)GetString(SAT_NEXT_ACTION_SEND_USSD_STRING_ID));

		case SAT_NEXT_ACTION_SEND_SMS:
			return((PU8)GetString(SAT_NEXT_ACTION_SEND_SMS_STRING_ID));

		case SAT_NEXT_ACTION_PLAY_TONE:
			return((PU8)GetString(SAT_NEXT_ACTION_PLAY_TONE_STRING_ID));

		case SAT_NEXT_ACTION_DISPLAY_TEXT:
			return((PU8)GetString(SAT_NEXT_ACTION_DISPLAY_TEXT_STRING_ID));

		case SAT_NEXT_ACTION_GET_INKEY:
			return((PU8)GetString(SAT_NEXT_ACTION_GET_INKEY_STRING_ID));

		case SAT_NEXT_ACTION_GET_INPUT:
			return((PU8)GetString(SAT_NEXT_ACTION_GET_INPUT_STRING_ID));

		case SAT_NEXT_ACTION_SELECT_ITEM:
			return((PU8)GetString(SAT_NEXT_ACTION_SELECT_ITEM_STRING_ID));

		case SAT_NEXT_ACTION_SETUP_MENU:
			return((PU8)GetString(SAT_NEXT_ACTION_SETUP_MENU_STRING_ID));

		case SAT_NEXT_ACTION_SETUP_IDLE_TEXT:
			return((PU8)GetString(SAT_NEXT_ACTION_SETUP_IDLE_TEXT_STRING_ID));

		case SAT_NEXT_ACTION_PERFORM_CARD_APDU:
			return((PU8)GetString(SAT_NEXT_ACTION_PERFORM_CARD_APDU_STRING_ID));

		case SAT_NEXT_ACTION_POWER_ON_CARD:
			return((PU8)GetString(SAT_NEXT_ACTION_POWER_ON_CARD_STRING_ID));

		case SAT_NEXT_ACTION_POWER_OFF_CARD:
			return((PU8)GetString(SAT_NEXT_ACTION_POWER_OFF_CARD_STRING_ID));

		case SAT_NEXT_ACTION_GET_READER_STATUS:
			return((PU8)GetString(SAT_NEXT_ACTION_GET_READER_STATUS_STRING_ID));

		case SAT_NEXT_ACTION_OPEN_CHANNEL:
			return((PU8)GetString(SAT_NEXT_ACTION_OPEN_CHANNEL_STRING_ID));

		case SAT_NEXT_ACTION_CLOSE_CHANNEL:
			return((PU8)GetString(SAT_NEXT_ACTION_CLOSE_CHANNEL_STRING_ID));

		case SAT_NEXT_ACTION_RECEIVE_DATA:
			return((PU8)GetString(SAT_NEXT_ACTION_RECEIVE_DATA_STRING_ID));

		case SAT_NEXT_ACTION_SEND_DATA:
			return((PU8)GetString(SAT_NEXT_ACTION_SEND_DATA_STRING_ID));

		case SAT_NEXT_ACTION_GET_CHANNEL_STATUS:
			return((PU8)GetString(SAT_NEXT_ACTION_GET_CHANNEL_STATUS_STRING_ID));

		case SAT_NEXT_ACTION_RESERVED_FOR_TIA:
			return((PU8)GetString(SAT_NEXT_ACTION_RESERVED_FOR_TIA_STRING_ID));

		case SAT_NEXT_ACTION_END_SESSION:
			return((PU8)GetString(SAT_NEXT_ACTION_END_SESSION_STRING_ID));

		default:
			return(NULL);
	}
}


/*****************************************************************************
 * FUNCTION
 *  SATAddIconIDToList
 * DESCRIPTION
 *  
 * PARAMETERS
 *  IconID      [IN]    
 * RETURNS
 *  void
 *****************************************************************************/
void SATAddIconIDToList(U8 IconID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_ICON_SUPPORT; i++)
    {
        if (gSATContext.iconList[i].iconID == IconID)
        {
            return;
        }
        
        if (gSATContext.iconList[i].iconID == 0)
        {
            gSATContext.iconList[i].iconID = IconID;
            return;
        }
    }
}

/**************************************************************

	FUNCTION NAME		: SATGetIconFromList

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
U8* SATGetIconFromList(U8 IconID)
{
	U8 i;
	for ( i = 0; i < MAX_ICON_SUPPORT; i++) {
		if (gSATContext.iconList[i].iconID == IconID && gSATContext.iconList[i].gotFromSIM == MMI_TRUE)
		{
			gSATContext.isCMDIconSupported = (gSATContext.iconList[i].iconAvailable==MMI_TRUE) ? MMI_TRUE : MMI_FALSE;  
			return(gSATContext.iconList[i].iconData);
		}
		if (gSATContext.iconList[i].iconID == 0)
		{
			return(NULL);
		}
	}
	return(NULL);
}


/*****************************************************************************
 * FUNCTION
 *  SATCheckAllIconGot
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  MMI_BOOL
 *****************************************************************************/
MMI_BOOL SATCheckAllIconGot(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_ICON_SUPPORT; i++)
    {
        if (gSATContext.iconList[i].iconID == 0)
        {
            return (MMI_TRUE);
        }

        if (gSATContext.iconList[i].gotFromSIM != MMI_TRUE)
        {
            return (MMI_FALSE);
        }
    }
    return (MMI_TRUE);
}



/*****************************************************************************
 * FUNCTION
 *  SATClearAllIcons
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SATClearAllIcons(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for ( i = 0; i < MAX_ICON_SUPPORT; i++)
    {
        if (gSATContext.iconList[i].iconID == 0)
        {
            break;
        }
        if (gSATContext.iconList[i].gotFromSIM == MMI_TRUE)
        {
            if(gSATContext.iconList[i].iconData != NULL)
            {
                if(gSATContext.iconList[i].iconAvailable == MMI_TRUE)
                {
                    OslMfree(gSATContext.iconList[i].iconData);
                }
                gSATContext.iconList[i].iconData = NULL;
            }
            gSATContext.iconList[i].iconID = 0;
            gSATContext.iconList[i].gotFromSIM = 0;
            gSATContext.iconList[i].iconAvailable = 0;  
        }
    }
}



/*****************************************************************************
 * FUNCTION
 *  SATGetIconDataToList
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SATGetIconDataToList(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for ( i = 0; i < MAX_ICON_SUPPORT; i++) 
    {
        if (gSATContext.iconList[i].iconID == 0)
        {
            gSATContext.currGetIconIndex = 0;
            SATGetIconCompleteCallBack();
            return;
        }
        
        if (gSATContext.iconList[i].gotFromSIM == MMI_FALSE) 
        {
            gSATContext.currGetIconIndex = i;

#ifdef __SAT_MMI_UNIT_TEST__
            SendImageDataRecordReq(&EFback1);
#else
            SendImageLenRecordReq( gSATContext.iconList[i].iconID);
#endif
            return;
        }
    }
}


/**************************************************************

	FUNCTION NAME		: SATGetIconDataCallBack

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SATGetIconDataCallBack(void)
{
	U32 dataSize, totalImageSize, rawBytes;
	U8 i, j, bitPerRasterPoint, currBitIndex, bitMask, bitMask1, bitMask2, CLUTIndex;
	PU8 dataPointer, currColorImgByte;
	U16 c_black, c_white, pixData;

	gSATContext.iconList[gSATContext.currGetIconIndex].gotFromSIM = MMI_TRUE;

	if (gSATImageRecord.noOfImages && ( gSATImageRecord.imageData != NULL))
	{
		totalImageSize = (U32) (gSATImageRecord.imageInst->width * gSATImageRecord.imageInst->height * 2 + 19);
		
		 
		gSATContext.iconList[gSATContext.currGetIconIndex].iconData=NULL;
		
		if(totalImageSize <= 2048 &&  
		   gSATImageRecord.imageInst->width == gSATImageRecord.imageData[0] &&
		   gSATImageRecord.imageInst->height == gSATImageRecord.imageData[1] )
		{
			gSATContext.iconList[gSATContext.currGetIconIndex].iconAvailable=MMI_TRUE;
		 

			gSATContext.iconList[gSATContext.currGetIconIndex].iconData = (U8 *) OslMalloc(totalImageSize*sizeof(U8));
			memset(gSATContext.iconList[gSATContext.currGetIconIndex].iconData, 0, totalImageSize*sizeof(U8));	
		
			gSATContext.iconList[gSATContext.currGetIconIndex].iconData[0] = 4 /*IMAGE_TYPE_DEVICE_BITMAP*/;
			gSATContext.iconList[gSATContext.currGetIconIndex].iconData[1] = 0;
			gSATContext.iconList[gSATContext.currGetIconIndex].iconData[2] = (U8) ((totalImageSize-6) & 0x000000ff);
			gSATContext.iconList[gSATContext.currGetIconIndex].iconData[3] = (U8) (((totalImageSize-6) >> 8) & 0x000000ff);
			gSATContext.iconList[gSATContext.currGetIconIndex].iconData[4] = (U8) (((totalImageSize-6) >> 16) & 0x000000ff);
			gSATContext.iconList[gSATContext.currGetIconIndex].iconData[5] = (U8) (((totalImageSize-6) >> 24) & 0x000000ff);		
			gSATContext.iconList[gSATContext.currGetIconIndex].iconData[6] = gSATImageRecord.imageInst->width;
			gSATContext.iconList[gSATContext.currGetIconIndex].iconData[10] = gSATImageRecord.imageInst->height;
			gSATContext.iconList[gSATContext.currGetIconIndex].iconData[14] = 0x10;
			rawBytes = (U32) ( gSATImageRecord.imageInst->width * 2);
			gSATContext.iconList[gSATContext.currGetIconIndex].iconData[15] = (U8) (rawBytes & 0x000000ff);
			gSATContext.iconList[gSATContext.currGetIconIndex].iconData[16] = (U8) ((rawBytes>>8) & 0x000000ff);
		
			dataPointer = gSATContext.iconList[gSATContext.currGetIconIndex].iconData + 19;
		
			dataSize = (U32) gSATImageRecord.imageInst->width * (U32) gSATImageRecord.imageInst->height;
		
			if (gSATImageRecord.imageInst->coding == SAT_BASIC_IMAGE) {
				c_white = GDI_RGB_TO_BUFFER_FORMAT(255,255,255);
				c_black = GDI_RGB_TO_BUFFER_FORMAT(0,0,0);
				for ( i = 0; i < dataSize; i++) {
					pixData = (*(gSATImageRecord.imageData +2 + i/8) >> (7-(i%8))) & 0x01 ? c_black : c_white;
					*(dataPointer + 2*i) = (U8) (pixData & 0x00ff);
					*(dataPointer + 2*i + 1) = (U8) ( (pixData >> 8) & 0x00ff);
				}
			}
			else if (gSATImageRecord.imageInst->coding == SAT_COLORR_IMAGE) {
				bitPerRasterPoint = (U8) *(gSATImageRecord.imageData+2);
				currBitIndex = 0;
				currColorImgByte = gSATImageRecord.imageData+6;
				bitMask = 0x01;
				for ( i = 0; i < bitPerRasterPoint; i++)
					bitMask = 2 * bitMask;
				bitMask = bitMask - 1;
		
				for ( i = 0; i < dataSize; i++) {
					if ((currBitIndex + bitPerRasterPoint) < 8) {
						CLUTIndex = ((*currColorImgByte) >> (8 - currBitIndex - bitPerRasterPoint)) & bitMask;
						currBitIndex = currBitIndex + bitPerRasterPoint;
					}
					else {
						bitMask1 = 0x01;
						for ( j = 0; j < 8 -currBitIndex; j++)
							bitMask1 = 2 * bitMask1;
						bitMask1 = bitMask1 - 1;
	
						if ((8-bitPerRasterPoint-currBitIndex) == 0) {
							CLUTIndex = (*currColorImgByte) & bitMask1;
						}
						else {
							bitMask2 = 0x01;
							for ( j = 0; j < bitPerRasterPoint+currBitIndex-8; j++)
								bitMask2 = 2 * bitMask2;
							bitMask2 = bitMask2 - 1;
							CLUTIndex = ((*currColorImgByte) & bitMask1) +
										(((*currColorImgByte+1) >> (8-(bitPerRasterPoint+currBitIndex-8))) &
										bitMask2);
						}
						currColorImgByte++;
						currBitIndex = (currBitIndex + bitPerRasterPoint) % 8;
					}
		
					pixData = GDI_RGB_TO_BUFFER_FORMAT( gSATImageRecord.CLUTData[CLUTIndex].red, 
												gSATImageRecord.CLUTData[CLUTIndex].green,
												gSATImageRecord.CLUTData[CLUTIndex].blue) ;
		
				*(dataPointer + 2*i) = (U8) (pixData & 0x00ff);
						*(dataPointer + 2*i + 1) = (U8) ( (pixData >> 8) & 0x00ff);
				}
			}
		}
		 
		else
		{
			gSATContext.iconList[gSATContext.currGetIconIndex].iconAvailable=MMI_FALSE;
			gSATContext.iconList[gSATContext.currGetIconIndex].iconData = (PU8)GetImage(IMG_GLOBAL_DEFAULT);
		}
		 
	}
	
	gSATImageRecord.iconId = 0;
	gSATImageRecord.noOfImages = 0;
	gSATImageRecord.noOfCLUT = 0;
	gSATImageRecord.locOfCLUT = 0;
	if (gSATImageRecord.imageInst != NULL) {
		OslMfree(gSATImageRecord.imageInst);
		gSATImageRecord.imageInst = NULL;
	}
	if (gSATImageRecord.imageData != NULL) {
		OslMfree(gSATImageRecord.imageData);
		gSATImageRecord.imageData = NULL;
	}
	gSATImageRecord.CLUTData = NULL;
	SATGetIconDataToList();
}



/*****************************************************************************
 * FUNCTION
 *  SendTerminalResponseSetupMenu
 * DESCRIPTION
 *  Sends SETUP MENU REQ to ADP
 * PARAMETERS
 *  res         [IN]    Response type
 *  cmdDetail   [IN]    Cmd detail to be sent
 * RETURNS
 *  void
 *****************************************************************************/
void SendTerminalResponseSetupMenu(U8 res, U8 *cmdDetail)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    COS_EVENT ev; 
    mmi_sat_setup_menu_res_req_struct   *dataPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    Message=ALLOCATE_ITM(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
#ifdef __MMI_MULTI_SIM__
    Message->oslDestId = MOD_L4C + SATGetActiveSimID();
#else
    Message->oslDestId = MOD_L4C;
#endif
    Message->oslMsgId = PRT_MMI_SAT_SETUP_MENU_RES_REQ;
    dataPtr =  (mmi_sat_setup_menu_res_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_setup_menu_res_req_struct));
    memcpy(dataPtr->cmd_detail, (void *)cmdDetail, 5 * sizeof(U8));
    dataPtr->res = res;
    Message->oslDataPtr = (oslParaType *)dataPtr;
    Message->oslPeerBuffPtr = NULL;

    ev.nEventId = Message->oslMsgId; 
    ev.nParam1 = (UINT32)Message; 
    ev.nParam3 = Message->oslDestId; 
    SEND_COSMSG(Message->oslDestId, &ev); 

    gSATContext.lastSATCmd = SAT_NONE_CMD;
}

/*****************************************************************************
 * FUNCTION
 *  SendTerminalResponseSetupMenuEx
 * DESCRIPTION
 *  Sends SETUP MENU REQ to ADP
 * PARAMETERS
 *  res         [IN]    Response type
 *  cmdDetail   [IN]    Cmd detail to be sent
 * RETURNS
 *  void
 *****************************************************************************/
void SendTerminalResponseSetupMenuEx(U8 destId, U8 res, U8* cmdDetail)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_setup_menu_res_req_struct *dataPtr;
    COS_EVENT ev;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Message = ALLOCATE_ITM(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
    Message->oslDestId = destId;
    Message->oslMsgId = PRT_MMI_SAT_SETUP_MENU_RES_REQ;
    dataPtr = (mmi_sat_setup_menu_res_req_struct*) OslConstructDataPtr(sizeof(mmi_sat_setup_menu_res_req_struct));
    dataPtr->res = res;
    memcpy(dataPtr->cmd_detail, (void*)cmdDetail, 5 * sizeof(U8));
    Message->oslDataPtr = (oslParaType*) dataPtr;
    Message->oslPeerBuffPtr = NULL;
    Message->oslSapId = INVALID_SAP;

    mmi_trace(g_sw_SAT,"SAT Func: %s dest sim id = %d", __FUNCTION__, destId - MOD_L4C);

    ev.nEventId = Message->oslMsgId; 
    ev.nParam1 = (UINT32)Message; 
    ev.nParam3 = Message->oslDestId; 
    SEND_COSMSG(Message->oslDestId, &ev); 

    gSATContext.lastSATCmd = SAT_NONE_CMD;
}

/*****************************************************************************
 * FUNCTION
 *  SATSetupMenuRsp
 * DESCRIPTION
 *  Protocol Handler for SETUP MENU RES
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATSetupMenuRsp(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mmi_sat_setup_menu_res_rsp_struct *setupMenuRes;
    U8 nSimID;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    mmi_trace(g_sw_SAT, "SAT FUNC:%s, isInSATMenu = %d", __FUNCTION__, gSATContext.isInSATMenu);

    setupMenuRes = (mmi_sat_setup_menu_res_rsp_struct *)msgPtr;

    nSimID = MTPNP_AD_get_protocol_event_src_mode();

    if (setupMenuRes->result == MMI_TRUE)
    {
        if(gSATContext.isInSATMenu)
        {
            SATStopWaitTimer();
            DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);		
            if (GetExitScrnID() == SCR_ID_SAT_HISTORY) 
            {
                ClearExitHandler();
            }
#ifdef __MMI_MULTI_SIM__
            SATSetActiveSimID(nSimID);
#endif
            GoToSATMainMenu();
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  SimBusyPleaseWait
 * DESCRIPTION
 *  Display SIM busy screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SimBusyPleaseWait(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup((U8*)GetString(SAT_WAIT_STRING_ID), IMG_GLOBAL_UNFINISHED,1, SSC_POP_UP_TIME, (U8)ERROR_TONE);
}

/*****************************************************************************
 * FUNCTION
 *  GoToSATMainMenu
 * DESCRIPTION
 *  Displays SAT Main Menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void GoToSATMainMenu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *listItems[64];
    U8 *listItemsIcons[64];
    U8 *listNextAction[64];
    U16 i;
    U8 nSimIDUsed = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    memset(listItems, 0, sizeof(listItems));
    memset(listItemsIcons, 0, sizeof(listItemsIcons));
    memset(listNextAction, 0, sizeof(listNextAction));

#ifdef __MMI_MULTI_SIM__
    nSimIDUsed = SATGetActiveSimID();

    if (!MTPNP_PFAL_Is_Card_Usable(nSimIDUsed))
    {
        DisplayPopup(
            (PU8) GetString(SERVICES_NOT_PROVIDED_STRING_ID),
            IMG_GLOBAL_WARNING,
            0,
            UI_POPUP_NOTIFYDURATION_TIME,
            (U8) WARNING_TONE);
        return;
    }
    
    SATSetIndSimID(nSimIDUsed);
    
    mmi_trace(g_sw_SAT,"SAT Func: %s nSimIDUsed = %d", __FUNCTION__, nSimIDUsed);    
#endif /* __MMI_MULTI_SIM__ */

    if ( g_pSATMainMemu->isIconAvailable)
    {
        SATAddIconIDToList( g_pSATMainMemu->IconID);
    }

    for ( i = 0; i < g_pSATMainMemu->noItemIconIdList; i++)
    {
        SATAddIconIDToList(g_pSATMainMemu->menuItems[i].IconID);
    }

    if (g_pSATMainMemu->isIconAvailable || g_pSATMainMemu->noItemIconIdList) 
    {
        if (!SATCheckAllIconGot()) 
        {
            SATGetIconCompleteCallBack = GoToSATMainMenu;
            SATGetIconDataToList();
            return;
        }
    }

    for(i=0; i<g_pSATMainMemu->numOfItem; i++)
    {
        listItems[i] = g_pSATMainMemu->menuItems[i].itemData;
        
        if (g_pSATMainMemu->noItemIconIdList)
        {
            listItemsIcons[i] = SATGetIconFromList(g_pSATMainMemu->menuItems[i].IconID);
        }
        if (g_pSATMainMemu->noNextActionIndList)
        {
            listNextAction[i] = SATReturnNextAction(g_pSATMainMemu->menuItems[i].nextAction);
        }
    }
    
    if(!gSATContext.isSATPresent[nSimIDUsed])
    {
        DeleteScreenIfPresent(SCR_ID_SAT_MAINMENU);
        ExitSATMainMenu();
        return;
    }

    gSATContext.exitCondition = &g_pSATMainMemu->isSentRsp;
    EntryNewScreen(SCR_ID_SAT_MAINMENU, SATGenericExitHandler, NULL, NULL);

    SetIsInSATScreen();
    g_pSATMainMemu->isSentRsp = 0;
    gSATContext.historyFuncPtr = GoToSATMainMenu;

    ClearAllKeyHandler();
    RegisterHighlightHandler(SATMenuIndex);
    
    ShowCategory174Screen(
        g_pSATMainMemu->alphaId, 
        SATGetIconFromList(g_pSATMainMemu->IconID),
        (PU8)GetString(STR_GLOBAL_OK),(PU8)GetImage(IMG_GLOBAL_OK),
        (PU8)GetString(STR_GLOBAL_BACK),(PU8)GetImage(IMG_GLOBAL_BACK),
        g_pSATMainMemu->numOfItem,listItems,
        listItemsIcons,
        listNextAction,
        0,
        g_pSATMainMemu->selectedMenuItem,
        gSATContext.guiBufferPointer);

    SetKeyHandler(ExitSATToIdle, KEY_END, KEY_EVENT_DOWN);

    SATCompletePreviousTransaction();

    if(g_pSATMainMemu->isHelpInfoAvailable) 
    {
        SetKeyHandler(EnvelopeSetupMenuHelp, KEY_SEND, KEY_EVENT_UP);
    }

    if (gSATContext.gotStage1Rsp) 
    {
#ifdef __MMI_MULTI_SIM__
        gSATContext.CMDfromSIM = SAT_CMD_SIM1 + nSimIDUsed;		
        gSATContext.satIndex = SAT_CMD_SIM1 + nSimIDUsed;	
#endif
        SetLeftSoftkeyFunction(EnvelopeSetupMenuOK,KEY_EVENT_UP);
        SetRightSoftkeyFunction(ExitSATMainMenu,KEY_EVENT_UP);
        SetKeyHandler(EnvelopeSetupMenuOK,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
        SetKeyHandler(ExitSATMainMenu,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
    }
    else 
    {
        SetLeftSoftkeyFunction(SimBusyPleaseWait,KEY_EVENT_UP);
        SetRightSoftkeyFunction(ExitSATMainMenu,KEY_EVENT_UP);
        SetKeyHandler(SimBusyPleaseWait,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
        SetKeyHandler(ExitSATMainMenu,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
    }
    
    gSATContext.isInSATMenu = 1;
    gSATContext.waitTimeExpired = 1;
}

/*****************************************************************************
 * FUNCTION
 *  SATCompletePreviousTransaction
 * DESCRIPTION
 *  Prevent screen delete by others
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SATCompletePreviousTransaction(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	switch (gSATContext.lastSATCmd) {
		case SAT_SELECT_ITEM_CMD:
			TerminalResponseSelectItem(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, gSATSubMenu.cmdDetail);
		break;
		case SAT_DISPLAY_TEXT_CMD:
			TerminalResponseDisplayText(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, gSATDisplayText.cmdDetail);
		break;
		case SAT_GET_INPUT:
			TerminalResponseGetInput(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, gSATGetInput.cmdDetail);
		break;
		case SAT_GET_INKEY:
			TerminalResponseGetInkey(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, gSATGetInkey.cmdDetail);
		break;
		case SAT_CALL_SETUP_CMD:
			TerminalResponseCallSetup(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, gSATCallSetup.cmdDetail);
		break;
//		case SAT_SEND_SMS_CMD:
//			TerminalResponseSendSMS(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, gSATSendSMS.cmdDetail);
//		break;
#if 0		//yuxiang 07.6.23
		case SAT_PLAY_TONE_CMD:
			TerminalResponsePlayTone(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, gSATPlayTone.cmdDetail);
		break;
		case SAT_LAUNCH_BROWSER_CMD:
			TerminalResponseLaunchBrowser(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, gSATLaunchBrowser.cmdDetail);
		break;
		case SAT_SEND_DTMF_CMD:
			TerminalResponseSendDTMF(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, gSATSendDTMF.cmdDetail);
		break;
#endif 		//yuxiang 07.6.23
//		case SAT_SEND_SS_CMD:
//			TerminalResponseSendSS(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0,NULL,gSATSendSS.cmdDetail);
//		break;
//		case SAT_SEND_USSD_CMD:
//			TerminalResponseSendUSSD(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0,NULL,gSATSendUSSD.cmdDetail);
//		break;
	}
}


/*****************************************************************************
 * FUNCTION
 *  ExitSATMainMenu
 * DESCRIPTION
 *  Exit Handler for SAT Main Menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitSATMainMenu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ExitVariables();
    GoBackHistory();
}

/*****************************************************************************
 * FUNCTION
 *  FreeMainMenu
 * DESCRIPTION
 *  Frees memory of various elements of SATSETUPMENU struct
 * PARAMETERS
 *  mainMenu    [IN]    Pointer to SATSETUPMENU struct to be freed
 * RETURNS
 *  void
 *****************************************************************************/
void FreeMainMenu(SATSETUPMENU *mainMenu)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    SATClearAllIcons();

    for (i=0; i<mainMenu->numOfItem; i++)
    {
        mainMenu->menuItems[i].itemLen=0;
    }
    
    mainMenu->numOfItem=0;
    if (mainMenu->menuItems != NULL)
    {
        OslMfree(mainMenu->menuItems);
        mainMenu->menuItems = NULL;
    }

    mainMenu->alphaIdLen=0;
}

/*****************************************************************************
 * FUNCTION
 *  SATMenuIndex
 * DESCRIPTION
 *  Highlight Handler for various menus in SAT
 * PARAMETERS
 *  index       [IN]    Index of item highlighted
 * RETURNS
 *  void
 *****************************************************************************/
void SATMenuIndex(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gSATContext.highlightedIndex =(U8)index;
}

/*****************************************************************************
 * FUNCTION
 *  EnvelopeSetupMenuOK
 * DESCRIPTION
 *  Key Handler for LSK of Main Menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EnvelopeSetupMenuOK(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!gSATContext.gotStage1Rsp)
    {
        /* prevent from entry SAT screen first then get SEND SMS indication */
        SimBusyPleaseWait();
        return;
    }
    
    mmi_trace(g_sw_SAT," EnvelopeSetupMenuOK   gSATContext.highlightedIndex=%d",  gSATContext.highlightedIndex);
    g_pSATMainMemu->selectedMenuItem = gSATContext.highlightedIndex;	  

    EnvelopeSetupMenu(g_pSATMainMemu->menuItems[  gSATContext.highlightedIndex].itemId, FALSE);
    SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
}

/*****************************************************************************
 * FUNCTION
 *  EnvelopeSetupMenuHelp
 * DESCRIPTION
 *  Key Handler for Hotkey of Main Menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EnvelopeSetupMenuHelp(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EnvelopeSetupMenu(g_pSATMainMemu->menuItems[gSATContext.highlightedIndex].itemId, TRUE);
    SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
}

/*****************************************************************************
 * FUNCTION
 *  EnvelopeSetupMenu
 * DESCRIPTION
 
 * PARAMETERS
 *  itemId                  [IN]    Item id of selcted item
 *  isHelpInfoRequested     [IN]    Is help Rquested (TRUE/FALSE)
 * RETURNS
 *  void
 *****************************************************************************/
void EnvelopeSetupMenu(U8 itemId, U8 isHelpInfoRequested)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_menu_select_req_struct *dataPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gSATContext.gotStage1Rsp = 0;
    g_pSATMainMemu->isSentRsp = 1;
    
    Message = ALLOCATE_ITM(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
#ifdef __MMI_MULTI_SIM__
    Message->oslDestId = MOD_L4C + SATGetActiveSimID();
#else
    Message->oslDestId = MOD_L4C;
#endif
    Message->oslMsgId =  PRT_MMI_SAT_MENU_SELECT_REQ;
    dataPtr =  (mmi_sat_menu_select_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_menu_select_req_struct));
    dataPtr->item_id = itemId;
    dataPtr->is_help_info_requested = isHelpInfoRequested;
    Message->oslDataPtr = (oslParaType *)dataPtr;
    Message->oslPeerBuffPtr = NULL;

#ifdef __MMI_MULTI_SIM__
    mmi_sat_dualsim_sendmsg(Message);
#else
    SEND_MSG(Message);
#endif

    gSATContext.lastSATCmd = SAT_NONE_CMD;
}

/*****************************************************************************
 * FUNCTION
 *  SATMenuSelectRsp
 * DESCRIPTION
 *  Protocol Handler for menu select
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATMenuSelectRsp(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gSATContext.gotStage1Rsp = 1;
}

/*****************************************************************************
 * FUNCTION
 *  SATSelectItemInd
 * DESCRIPTION
 *  Protocol Handler for SELECT ITEM IND
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATSelectItemInd(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i;
    U8 *tempDataPtr;
    SATMENUITEM *menuItem;
    mmi_sat_select_item_ind_struct *selectItem;
    U16 convertLen;
    U8 *palpha = NULL;
    U8 nSimID = 0;    
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/	
    selectItem = (mmi_sat_select_item_ind_struct *)msgPtr;

    nSimID = MTPNP_AD_get_protocol_event_src_mode();

#ifdef __MMI_MULTI_SIM__
    if (gSATSubMenu.CMDfromSIM != (nSimID+SAT_CMD_SIM1) && gSATSubMenu.CMDfromSIM != SAT_CMD_NONE)
    {
        TerminalResponseSelectItemEx(MOD_L4C + nSimID, SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 1, selectItem->cmd_detail);
        if(selectItem->alpha_id != NULL)
        {
            OslMfree(selectItem->alpha_id);
        }
        if(selectItem->item_data != NULL)
        {
            OslMfree(selectItem->item_data);
        }
        return;
    }
    else
    {
        gSATSubMenu.CMDfromSIM = SAT_CMD_SIM1 + nSimID;
    }
#endif
		
    FreeSubMenu();
    gSATContext.lastSATCmd = SAT_SELECT_ITEM_CMD;
    gSATContext.guiBufferPointer=NULL;
    gSATSubMenu.isSentRsp=0;

    if( GetCurrentState() == CM_OUTGOING_STATE || GetCurrentState() == CM_INCOMING_STATE )
    {
        TerminalResponseSelectItem(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, selectItem->cmd_detail);
        if(selectItem->alpha_id != NULL)
        {
            OslMfree(selectItem->alpha_id);
        }
        if(selectItem->item_data != NULL)
        {
            OslMfree(selectItem->item_data);
        }
        return;
    }	

#ifdef __FLIGHT_MODE_SUPPORT__
    if(mmi_bootup_get_active_flight_mode() == 1)
    {
        TerminalResponseSelectItem(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, selectItem->cmd_detail);
        if(selectItem->alpha_id != NULL)
        {
            OslMfree(selectItem->alpha_id);
        }
        if(selectItem->item_data != NULL)
        {
            OslMfree(selectItem->item_data);
        }
        return;
    }
#endif

    memcpy(gSATSubMenu.cmdDetail, selectItem->cmd_detail, 5 * sizeof(U8));
    gSATSubMenu.isHelpInfoAvailable = selectItem->is_help_info_available;
    gSATSubMenu.numOfItem = selectItem->num_of_item;

#ifdef __SAT_MMI_UNIT_TEST__
    gSATSubMenu.isIconAvailable = 1;
    if (gSATSubMenu.isIconAvailable) 
    {
        gSATSubMenu.IconID = 1;
        gSATSubMenu.iconAttr = SAT_ICON_NOT_SELF_EX;
    }
    gSATSubMenu.noItemIconIdList = 1;
    gSATSubMenu.itemIconListAttr = 1;
    gSATSubMenu.noNextActionIndList = 1;
    gSATSubMenu.dcsOfAlphaId = selectItem->dcs_of_alpha_id;
    if (gSATSubMenu.isIconAvailable && gSATSubMenu.iconAttr == SAT_ICON_SELF_EX) 
    {
        gSATSubMenu.alphaIdLen = 0;
        gSATSubMenu.alphaId=NULL;
    }						
    else 
    {
        gSATSubMenu.alphaIdLen = selectItem->no_alpha_id;
        gSATSubMenu.alphaId=CovertStringForPlatform(selectItem->alpha_id, gSATSubMenu.alphaIdLen,gSATSubMenu.dcsOfAlphaId,&gSATSubMenu.alphaIdLen);
    }
#else
    gSATSubMenu.isIconAvailable = selectItem->is_icon_available;
    if (gSATSubMenu.isIconAvailable)
    {
        gSATSubMenu.IconID = selectItem->icon_info.icon_id;
        gSATSubMenu.iconAttr = selectItem->icon_info.icon_attr;
    }
    
    gSATSubMenu.noItemIconIdList = selectItem->no_item_icon_id_list;
    gSATSubMenu.itemIconListAttr = selectItem->item_icon_list_attr;
    gSATSubMenu.noNextActionIndList = selectItem->no_next_action_ind_list;

    gSATSubMenu.dcsOfAlphaId = selectItem->dcs_of_alpha_id;
    if( (gSATSubMenu.isIconAvailable && gSATSubMenu.iconAttr == SAT_ICON_SELF_EX) || (selectItem->no_alpha_id == 0))
    {
        gSATSubMenu.alphaIdLen = 0;
        gSATSubMenu.alphaId=NULL;
    }
    else
    {
        gSATSubMenu.alphaIdLen = selectItem->no_alpha_id;
        if(gSATSubMenu.alphaIdLen != 0)
        {
            gSATSubMenu.alphaId = CovertStringForPlatform(selectItem->alpha_id, 
                                                                                            gSATSubMenu.alphaIdLen, 
                                                                                            gSATSubMenu.dcsOfAlphaId, 
                                                                                            &gSATSubMenu.alphaIdLen);
        }
        else
        {
            gSATSubMenu.alphaIdLen = g_pSATMainMemu->alphaIdLen;
            pfnUnicodeStrcpy(gSATSubMenu.alphaId, g_pSATMainMemu->alphaId);
        }
    }
#endif

    if(gSATSubMenu.numOfItem==0)
    {
        SelectItemWrongData();
        return;
    }
    
    if (gSATSubMenu.numOfItem > 0)
    {
        gSATSubMenu.menuItems = OslMalloc(gSATSubMenu.numOfItem * sizeof(SATMENUITEM));
        memset(gSATSubMenu.menuItems, 0, gSATSubMenu.numOfItem * sizeof(SATMENUITEM));

        menuItem = gSATSubMenu.menuItems;
        tempDataPtr = selectItem->item_data;
        
        for (i=0; i<gSATSubMenu.numOfItem; i++, menuItem++)
        {
            menuItem->itemLen = (U16) (*(tempDataPtr+1));
            tempDataPtr += 2;
            menuItem->itemId = *tempDataPtr++;
            menuItem->itemDcs = *tempDataPtr++;
            
            if(menuItem->itemLen!=0)
            {
                palpha = CovertStringForPlatform(tempDataPtr,menuItem->itemLen, menuItem->itemDcs,&convertLen);
                mmi_sat_release_menu_alpha(palpha, menuItem->itemData, &convertLen);
                tempDataPtr += menuItem->itemLen;
                menuItem->itemLen=convertLen;
            }
            else 
            {
                menuItem->itemLen=0;
            }

            if (gSATSubMenu.noItemIconIdList)
            {
#ifdef __SAT_MMI_UNIT_TEST__
                menuItem->IconID = 1;
#else
                menuItem->IconID = *(selectItem->item_icon_id_list + i);
#endif
            }
            
            if (gSATSubMenu.noItemIconIdList && gSATSubMenu.itemIconListAttr == SAT_ICON_SELF_EX) 
            {
                menuItem->itemLen = 0;
            }
                
            if (gSATSubMenu.noNextActionIndList)
#ifdef __SAT_MMI_UNIT_TEST__
            menuItem->nextAction= SAT_NEXT_ACTION_RESERVED_FOR_TIA_STRING_ID;
#else
            menuItem->nextAction= selectItem->next_action_ind_list[i];
#endif
        }
    }

    gSATSubMenu.defaultIndex=0;
    if(selectItem->is_item_id_available)
    {
        for(i=0; i<gSATSubMenu.numOfItem;i++)
        {
            if(gSATSubMenu.menuItems[i].itemId == selectItem->item_id)
            {
                gSATSubMenu.defaultIndex=i;
                break;
            }
        }
    }
    
    SATStopWaitTimer();
    DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);		
    if (GetExitScrnID() == SCR_ID_SAT_HISTORY) 
    {
        ClearExitHandler();
    }
    
    GoToSATSubMenu();

    if(selectItem->alpha_id != NULL)
    {
        OslMfree(selectItem->alpha_id);
    }
    if(selectItem->item_data != NULL)
    {
        OslMfree(selectItem->item_data);
    }
}

/*****************************************************************************
 * FUNCTION
 *  SelectItemWrongData
 * DESCRIPTION
 *  Displays SAT Sub Menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SelectItemWrongData(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TerminalResponseSelectItem(SAT_CMD_DATA_NOT_UNDERSTAND_BY_ME, 
                                                        gSATSubMenu.menuItems[gSATContext.highlightedIndex].itemId, 
                                                        gSATSubMenu.cmdDetail);
}

/*****************************************************************************
 * FUNCTION
 *  GoToSATSubMenu
 * DESCRIPTION
 *  Displays SAT Sub Menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void GoToSATSubMenu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *listItems[64];
    U8 *listItemsIcons[64];
    U8 *listNextAction[64];
    U16 i;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    memset(listItems,0,sizeof(listItems));
    memset(listItemsIcons,0,sizeof(listItemsIcons));
    memset(listNextAction,0,sizeof(listNextAction));

    if ( gSATSubMenu.isIconAvailable)
    {   
        SATAddIconIDToList( gSATSubMenu.IconID);
    }

    for ( i = 0; i < gSATSubMenu.noItemIconIdList; i++)
    {   
        SATAddIconIDToList( gSATSubMenu.menuItems[i].IconID);
    }

    if (gSATSubMenu.isIconAvailable || gSATSubMenu.noItemIconIdList) 
    {
        if ( !SATCheckAllIconGot())
        {
            SATGetIconCompleteCallBack = GoToSATSubMenu;
            SATGetIconDataToList();
            return;
        }
    }

    for(i=0;i<gSATSubMenu.numOfItem;i++)
    {
        listItems[i]=gSATSubMenu.menuItems[i].itemData;
        if (gSATSubMenu.noItemIconIdList)
        {      
            listItemsIcons[i] = SATGetIconFromList(gSATSubMenu.menuItems[i].IconID);
        }
        if (gSATSubMenu.noNextActionIndList)
        {      
            listNextAction[i] = SATReturnNextAction(gSATSubMenu.menuItems[i].nextAction);
        }
    }

    gSATContext.exitCondition = &gSATSubMenu.isSentRsp;
    EntryNewScreen(SCR_ID_SAT_SUBMENU, SATGenericExitHandler, NULL, NULL);

    SetIsInSATScreen();
    gSATContext.historyFuncPtr=GoToSATSubMenu;
    ClearAllKeyHandler();
    RegisterHighlightHandler(SATMenuIndex);

#ifdef __MMI_MULTI_SIM__
    SATSetIndSimID(gSATSubMenu.CMDfromSIM - SAT_CMD_SIM1);
#endif
    ShowCategory174Screen(
        gSATSubMenu.alphaId, 
        SATGetIconFromList(gSATSubMenu.IconID),
        (PU8)GetString(STR_GLOBAL_OK),
        (PU8)GetImage(IMG_GLOBAL_OK),
        (PU8)GetString(STR_GLOBAL_BACK),
        (PU8)GetImage(IMG_GLOBAL_BACK),
        gSATSubMenu.numOfItem,
        listItems,
        listItemsIcons,
        listNextAction,
        0,
        gSATSubMenu.defaultIndex,  
        gSATContext.guiBufferPointer);

    if(gSATSubMenu.isHelpInfoAvailable) 
    {
        SetKeyHandler(TerminalResponseSelectItemHelp,KEY_SEND,KEY_EVENT_UP);
    }

    SetKeyHandler(TerminalResponseSelectItemExpire,KEY_END,KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction(TerminalResponseSelectItemOK,KEY_EVENT_UP);
    SetRightSoftkeyFunction(TerminalResponseSelectItemMoveBack,KEY_EVENT_UP);
    SetKeyHandler(TerminalResponseSelectItemOK,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
    SetKeyHandler(TerminalResponseSelectItemMoveBack,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
    StartSATTimerNoResponsefromUser(SelectItemTimer);
}


/*****************************************************************************
 * FUNCTION
 *  SelectItemTimer
 * DESCRIPTION
 *  Callback function called whn the user shows no activity on
 *  Select Item Screen for 30 secs
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SelectItemTimer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_SAT," SelectItemTimer   gSATContext.isInSATMenu=%d",  gSATContext.isInSATMenu);
    
    TerminalResponseSelectItem(SAT_NO_RESPONSE_FROM_USER,
                                                    gSATSubMenu.menuItems[gSATContext.highlightedIndex].itemId, 
                                                    gSATSubMenu.cmdDetail);
    
    if (gSATContext.isInSATMenu)
    {
#ifdef __MMI_MULTI_SIM__
        SATSetActiveSimID(  gSATContext.satIndex - SAT_CMD_SIM1);
#endif
        gSATContext.historyFuncPtr = GoToSATMainMenu;

        if(  gSATContext.isInSATScreen)
        {
            if(IsScreenPresent(SCR_ID_SAT_HISTORY))
            {
                GoBackToHistory(SCR_ID_SAT_HISTORY);
            }
            else
            {
#ifdef __MMI_MULTI_SIM__
                SATSetActiveSimID(gSATContext.satIndex - SAT_CMD_SIM1);
#endif
                GoToSATMainMenu();
            }
        }
    }
    else 
    {
        if(gSATContext.isInSATScreen) 
        {
            GoBackHistory();
        }
        else
        {
            DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  FreeSubMenu
 * DESCRIPTION
 *  Frees memory of varios elements of SAT Sub Menu Structure
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void FreeSubMenu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    SATClearAllIcons();

    for (i=0; i<gSATSubMenu.numOfItem; i++)
    {
        gSATSubMenu.menuItems[i].itemLen=0;
    }
    
    gSATSubMenu.numOfItem=0;
    if (gSATSubMenu.menuItems != NULL)
    {
        OslMfree(gSATSubMenu.menuItems);
        gSATSubMenu.menuItems = NULL;
    }

    if (gSATSubMenu.alphaId)
    {
        OslMfree(gSATSubMenu.alphaId);
    }
    
    gSATSubMenu.alphaId = NULL;
    gSATSubMenu.alphaIdLen=0;
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseSelectItemOK
 * DESCRIPTION
 *  Key Handler for LSK of SAT Sub Menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseSelectItemOK(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TerminalResponseSelectItem(
        SAT_CMD_PERFORMED_SUCCESSFULLY,
        gSATSubMenu.menuItems[gSATContext.highlightedIndex].itemId,
        gSATSubMenu.cmdDetail);
    SATPleaseWaitScreen(STR_GLOBAL_PLEASE_WAIT, SAT_WAIT_IMAGE_ID);
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseSelectItemMoveBack
 * DESCRIPTION
 *  Key Handler for RSK of SAT Sub Menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseSelectItemMoveBack(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TerminalResponseSelectItem(
        SAT_BACKWARD_MOVE_IN_PROACTIVE_SESSION_BY_USER,
        gSATSubMenu.menuItems[gSATContext.highlightedIndex].itemId,
        gSATSubMenu.cmdDetail);
    SATPleaseWaitScreen(STR_GLOBAL_PLEASE_WAIT, SAT_WAIT_IMAGE_ID);
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseSelectItemHelp
 * DESCRIPTION
 *  Key Handler for Hotkey of SAT Sub Menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseSelectItemHelp(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TerminalResponseSelectItem(
        SAT_HELP_INFO_REQUEST_BY_USER, 
        gSATSubMenu.menuItems[gSATContext.highlightedIndex].itemId, 
        gSATSubMenu.cmdDetail);
    
    SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseSelectItemExpire
 * DESCRIPTION
 *  Key Handler for End Key of SAT Sub Menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseSelectItemExpire(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TerminalResponseSelectItem(
        SAT_PROACTIVE_CMD_TERMINATED_BY_USER,
        gSATSubMenu.menuItems[gSATContext.highlightedIndex].itemId,
        gSATSubMenu.cmdDetail);
    ExitSATToIdle();
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseSelectItem
 * DESCRIPTION
 
 * PARAMETERS
 *  res         [IN]    Response type
 *  itemId      [IN]    Item id of selcted item
 *  cmdDetail   [IN]    Cmd detail to be sent
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseSelectItem(U8 res, U8 itemId, U8 *cmdDetail)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_select_item_res_req_struct *dataPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(!gSATSubMenu.isSentRsp)
    {
        StopSATTimerNoResponsefromUser();
        gSATContext.noResponseFuncPtr = NULL; 
        gSATSubMenu.isSentRsp=1;
        
        Message=ALLOCATE_ITM(MOD_MMI);
        Message->oslSrcId = MOD_MMI;
#ifdef __MMI_MULTI_SIM__
        Message->oslDestId =  MOD_L4C + gSATSubMenu.CMDfromSIM - SAT_CMD_SIM1; 
#else
        Message->oslDestId = MOD_L4C;
#endif
        Message->oslMsgId =  PRT_MMI_SAT_SELECT_ITEM_RES_REQ;
        dataPtr =  (mmi_sat_select_item_res_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_select_item_res_req_struct));
        dataPtr->item_id = itemId;

        if(res == SAT_CMD_PERFORMED_SUCCESSFULLY && gSATSubMenu.isIconAvailable==1 &&   gSATContext.isCMDIconSupported==MMI_FALSE)
        {
            dataPtr->res = SAT_CMD_PERFORMED_BUT_ICON_COULD_NOT_DSPL;
        }
        else
        {
            dataPtr->res = res;
        }

        memcpy(dataPtr->cmd_detail, (void *)cmdDetail, 5*sizeof(U8));
        Message->oslDataPtr = (oslParaType *)dataPtr;
        Message->oslPeerBuffPtr = NULL;

#ifdef __MMI_MULTI_SIM__
        mmi_sat_dualsim_sendmsg(Message);
        gSATSubMenu.CMDfromSIM = SAT_CMD_NONE;
#else
        SEND_MSG(Message);
#endif

        gSATContext.lastSATCmd = SAT_NONE_CMD;
    }
}

/*****************************************************************************
 * FUNCTION
 *  SATSelectItemRsp
 * DESCRIPTION
 *  Protocol Handler for SELECT ITEM RES
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATSelectItemRsp(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_sat_select_item_res_rsp_struct *sat_rsp = (mmi_sat_select_item_res_rsp_struct *)msgPtr;

#ifdef __MMI_MULTI_SIM__
    if(sat_rsp->sim_id == gSATSubMenu.CMDfromSIM)
#endif
    {
        FreeSubMenu();
    }
}

/*****************************************************************************
 * FUNCTION
 *  SAT3000Timer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void SAT3000Timer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    StopTimer(SAT_3000_TIMER);
    if (isIncomingCall() || isOutgoingCall())
    {
        StartTimer(SAT_500_TIMER, 500, SAT500Timer);
    }
    else
    {
    	SATStopWaitTimer();
        GoToSATDisplayTextScreen();
    }
}


/*****************************************************************************
 * FUNCTION
 *  SAT500Timer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void SAT500Timer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    StopTimer(SAT_500_TIMER);
    if (isIncomingCall() || isOutgoingCall())
    {
        StartTimer(SAT_500_TIMER, 500, SAT500Timer);
    }
    else
    {
        StartTimer(SAT_3000_TIMER, 3000, SAT3000Timer);
    }
}

/*****************************************************************************
 * FUNCTION
 *  SATDisplayTextInd
 * DESCRIPTION
 *  Protocol Handler for DISPLAY TEXT IND
 * PARAMETERS
 *  msgPtr      [IN]    Pointer to data from ADP
 * RETURNS
 *  void
 *****************************************************************************/
void SATDisplayTextInd(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MULTI_SIM__
    U8 nSimID = 0;
#endif

    mmi_sat_display_text_ind_struct *displayText;
    displayText=(mmi_sat_display_text_ind_struct *)msgPtr;


    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MULTI_SIM__
    nSimID = MTPNP_AD_get_protocol_event_src_mode();

    if (gSATSubMenu.CMDfromSIM != (nSimID+SAT_CMD_SIM1) && gSATSubMenu.CMDfromSIM != SAT_CMD_NONE)
    {
        if(displayText->text_string != NULL)
        {
            OslMfree(displayText->text_string);
            displayText->text_string = NULL;
        }    
    
        TerminalResponseDisplayTextEx(MOD_L4C + nSimID, SAT_BACKWARD_MOVE_IN_PROACTIVE_SESSION_BY_USER, displayText->cmd_detail);
        return;
    }
    else
    {
        gSATDisplayText.CMDfromSIM = SAT_CMD_SIM1 + nSimID;
    }
#endif

    FreeDisplayText();

    gSATContext.lastSATCmd = SAT_DISPLAY_TEXT_CMD;
    gSATContext.guiBufferPointer = NULL;
    gSATDisplayText.sentRsp = 0;

    if(((!gSATContext.isInSATMenu && !g_idle_context.IsOnIdleScreen && !gSATContext.isInSATScreen) && displayText->priority==SAT_PRI_NORMAL)
    || ( GetCurrentState() == CM_OUTGOING_STATE) || ( GetCurrentState() == CM_INCOMING_STATE))
    {
        if(displayText->text_string != NULL)
        {
            OslMfree(displayText->text_string);
            displayText->text_string = NULL;
        }    

        TerminalResponseDisplayText(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, displayText->cmd_detail);    
        return;
    }

    if ( displayText->is_icon_available && !displayText->no_text_string) 
    {
        if(displayText->text_string != NULL)
        {
            OslMfree(displayText->text_string);
            displayText->text_string = NULL;
        }    
    
        TerminalResponseDisplayText(SAT_CMD_DATA_NOT_UNDERSTAND_BY_ME,displayText->cmd_detail);
        return;
    }
    else if (displayText->no_text_string == 0 && !gSATContext.isInSATMenu)
    {
        if(displayText->text_string != NULL)
        {
            OslMfree(displayText->text_string);
            displayText->text_string = NULL;
        }    
    
        TerminalResponseDisplayText(SAT_CMD_PERFORMED_SUCCESSFULLY, displayText->cmd_detail);
        return;
    }
    
#ifdef __MMI_MULTI_SIM__
    if (!MTPNP_PFAL_Is_Card_Usable(gSATDisplayText.CMDfromSIM - SAT_CMD_SIM1))
    {
        if(displayText->text_string != NULL)
        {
            OslMfree(displayText->text_string);
            displayText->text_string = NULL;
        }    
    
        TerminalResponseDisplayText(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, displayText->cmd_detail);
        return;
    }
#endif

    memcpy(gSATDisplayText.cmdDetail, (void *)displayText->cmd_detail, 5 * sizeof(U8));
    gSATDisplayText.clearTextType = displayText->clear_text_type;
    gSATDisplayText.immediateRes = displayText->immediate_res;

#ifdef __SAT_MMI_UNIT_TEST__
    gSATDisplayText.dcsOfTextString=MMI_DEFAULT_DCS;
    gSATDisplayText.textStringLen=50;
    gSATDisplayText.textString=CovertStringForPlatform((PU8) "01234567890123456789012345678901234567890123456789", gSATDisplayText.textStringLen, gSATDisplayText.dcsOfTextString,&gSATDisplayText.textStringLen);

    gSATDisplayText.isIconAvailable = 1;
    if (gSATDisplayText.isIconAvailable) 
    {
        gSATDisplayText.iconInfo.iconId= 1;
        gSATDisplayText.iconInfo.iconAttr= SAT_ICON_NOT_SELF_EX;
    }
#else
    gSATDisplayText.dcsOfTextString = displayText->dcs_of_text_string;
    if (gSATDisplayText.dcsOfTextString == MMI_8BIT_DCS)
    {
         gSATDisplayText.dcsOfTextString = MMI_DEFAULT_DCS;
    }
    gSATDisplayText.textStringLen = displayText->no_text_string;
    
    gSATDisplayText.textString = CovertStringForPlatform(
                                                        displayText->text_string, 
                                                        gSATDisplayText.textStringLen,
                                                        gSATDisplayText.dcsOfTextString,
                                                        &gSATDisplayText.textStringLen);
    
    gSATDisplayText.isIconAvailable = displayText->is_icon_available;
    if (gSATDisplayText.isIconAvailable) 
    {
        gSATDisplayText.iconInfo.iconId = displayText->icon_info.icon_id;
        gSATDisplayText.iconInfo.iconAttr = displayText->icon_info.icon_attr;
    }
#endif

    if (gSATDisplayText.isIconAvailable && gSATDisplayText.iconInfo.iconAttr == SAT_ICON_SELF_EX)
    {
        if (gSATDisplayText.textString)
        {
            OslMfree(gSATDisplayText.textString);
        }
        gSATDisplayText.textString = NULL;
        gSATDisplayText.textStringLen = 0;
    }

    if (IS_IDLE_ARRIVED) 
    {
        DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);
        if (GetExitScrnID() == SCR_ID_SAT_HISTORY)
        {
            ClearExitHandler();
        }
        
        if (isIncomingCall() || isOutgoingCall())
        {
            StartTimer(SAT_500_TIMER, 500, SAT500Timer);
        }
        else
        {
            SATStopWaitTimer();
            GoToSATDisplayTextScreen();
        }
    }
    else if (gSATContext.postponedSATScreen == NULL) 
    {
        SATStopWaitTimer();
        gSATContext.postponedSATScreen = GoToSATDisplayTextScreen;
    }
    else 
    {
        TerminalResponseDisplayText(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, displayText->cmd_detail);
        ClearPostponedSATScreen(); 
    }

    if(displayText->text_string != NULL)
    {
        OslMfree(displayText->text_string);
        displayText->text_string = NULL;
    }    
}


/*****************************************************************************
 * FUNCTION
 *  GoToSATDisplayTextScreen
 * DESCRIPTION
 *  Displays DISPLAY TEXT Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void GoToSATDisplayTextScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *iconData = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gSATDisplayText.isIconAvailable)
    {   
        SATAddIconIDToList( gSATDisplayText.iconInfo.iconId);
    }

    if (gSATDisplayText.isIconAvailable) 
    {
        if (!SATCheckAllIconGot()) 
        {
            SATGetIconCompleteCallBack = GoToSATDisplayTextScreen;
            SATGetIconDataToList();
            return;
        }
    }
    
    if (gSATDisplayText.isIconAvailable && gSATDisplayText.iconInfo.iconAttr == SAT_ICON_SELF_EX &&
        SATGetIconFromList(gSATDisplayText.iconInfo.iconId) != NULL)
    {
        if (gSATDisplayText.textString)
        {
            OslMfree(gSATDisplayText.textString);
            gSATDisplayText.textString = NULL;
        }
    }
    
    iconData = SATGetIconFromList(gSATDisplayText.iconInfo.iconId);
    gSATContext.exitCondition = &gSATDisplayText.sentRsp;
    EntryNewScreen(SCR_ID_SAT_DISPLAYTEXT, SATGenericExitHandler, NULL, NULL);

    SetIsInSATScreen();
    gSATContext.historyFuncPtr = GoToSATDisplayTextScreen;
    ClearAllKeyHandler();
    
    if(gSATDisplayText.textStringLen < 90 * ENCODING_LENGTH)
    {
        ShowCategory151Screen( 0, 0, 
            STR_GLOBAL_OK,IMG_GLOBAL_OK,
            STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
            gSATDisplayText.textString, iconData,   gSATContext.guiBufferPointer);
    }
    else
    {
        ShowCategory151Screen_ex( 0, 0, 
            STR_GLOBAL_OK,IMG_GLOBAL_OK,
            STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
            gSATDisplayText.textString, iconData,   gSATContext.guiBufferPointer);
    }
    
    if(gSATDisplayText.immediateRes) 
    {
        TerminalResponseDisplayText(SAT_CMD_PERFORMED_SUCCESSFULLY,gSATDisplayText.cmdDetail);
    }

    SetKeyHandler(TerminalResponseDisplayTextExpire, KEY_END, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction(TerminalResponseDisplayTextOK, KEY_EVENT_UP);
    SetRightSoftkeyFunction(TerminalResponseDisplayTextMoveBack, KEY_EVENT_UP);

    StartSATTimerNoResponsefromUser(DisplayTextTimer);
}

/*****************************************************************************
 * FUNCTION
 *  DisplayTextTimer
 * DESCRIPTION
 *  Callback function called whn the user shows no activity on
 *  Display Text Screen for 30 secs
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void DisplayTextTimer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(TRUE," Func: %s gSATDisplayText.clearTextType=%d", __FUNCTION__, gSATDisplayText.clearTextType);
    
    if(gSATDisplayText.clearTextType == SAT_CLEAR_AFTER_DELAY)
    {
        TerminalResponseDisplayText(SAT_CMD_PERFORMED_SUCCESSFULLY, gSATDisplayText.cmdDetail);
    }
    else
    {
        TerminalResponseDisplayText(SAT_NO_RESPONSE_FROM_USER, gSATDisplayText.cmdDetail);

        if (gSATContext.isInSATMenu)
        {
#ifdef __MMI_MULTI_SIM__
            SATSetActiveSimID(gSATContext.satIndex - SAT_CMD_SIM1);
#endif
            gSATContext.historyFuncPtr = GoToSATMainMenu;
            if(gSATContext.isInSATScreen)
            {
                if(IsScreenPresent(SCR_ID_SAT_HISTORY))
                {
                    GoBackToHistory(SCR_ID_SAT_HISTORY);
                }
                else
                {
#ifdef __MMI_MULTI_SIM__
                    SATSetActiveSimID(gSATContext.satIndex - SAT_CMD_SIM1);
#endif
                    GoToSATMainMenu();
                }
            }
        }
        else
        { 
#if defined(__MMI_NOKIA_STYLE_N800__)&&defined(JATAAYU_SUPPORT)
            if(gSATContext.isInSATScreen&&(!IsScreenPresent(SCR_ID_MMS_VIEW)) )
#else
            if(gSATContext.isInSATScreen) 
#endif
            {
                GoBackHistory();
            }
            else
            {
                DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);
            }
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  FreeDisplayText
 * DESCRIPTION
 *  Frees memory of varios elements of Display Text
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void FreeDisplayText(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SATClearAllIcons();

    if(gSATDisplayText.textString)
    {
        OslMfree(gSATDisplayText.textString);
        gSATDisplayText.textString = NULL;        
    }
    
    gSATDisplayText.textStringLen=0;
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseDisplayTextOK
 * DESCRIPTION
 *  Key Handler for LSK of Display Text Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseDisplayTextOK(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TerminalResponseDisplayText(SAT_CMD_PERFORMED_SUCCESSFULLY, gSATDisplayText.cmdDetail);
    SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseDisplayTextMoveBack
 * DESCRIPTION
 *  Key Handler for RSK of Display Text Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseDisplayTextMoveBack(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TerminalResponseDisplayText(SAT_BACKWARD_MOVE_IN_PROACTIVE_SESSION_BY_USER, gSATDisplayText.cmdDetail);
    SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseDisplayTextExpire
 * DESCRIPTION
 *  Key Handler for End Key of Display Text Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseDisplayTextExpire(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TRACE_EF(g_sw_SAT, " TerminalResponseDisplayTextExpire");
    TerminalResponseDisplayText(SAT_PROACTIVE_CMD_TERMINATED_BY_USER, gSATDisplayText.cmdDetail);
    ExitSATToIdle();
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseDisplayText
 * DESCRIPTION
 
 * PARAMETERS
 *  res         [IN]    Response type
 *  cmdDetail   [IN]    Cmd detail to be sent
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseDisplayText(U8 res, U8 *cmdDetail)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_display_text_res_req_struct *dataPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    StopSATTimerNoResponsefromUser();
    gSATContext.noResponseFuncPtr = NULL;
    
#ifdef __MMI_SAT_DISPLAY_TEXT_WITH_KEYPAD_LOCK__
    gSATContext.idleSATCmd = 0;
#endif

    if(!gSATDisplayText.sentRsp)
    {
        gSATDisplayText.sentRsp=1;
        Message = ALLOCATE_ITM(MOD_MMI);
        Message->oslSrcId = MOD_MMI;
#ifdef __MMI_MULTI_SIM__
        Message->oslDestId =  SAT_OS_DEST_ID; 
#else
        Message->oslDestId = MOD_L4C;
#endif

        Message->oslMsgId =  PRT_MMI_SAT_DISPLAY_TEXT_RES_REQ;
        dataPtr =  (mmi_sat_display_text_res_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_display_text_res_req_struct));

        if(res == SAT_CMD_PERFORMED_SUCCESSFULLY && gSATDisplayText.isIconAvailable == 1 && gSATContext.isCMDIconSupported == MMI_FALSE)
        {
            dataPtr->res = SAT_CMD_PERFORMED_BUT_ICON_COULD_NOT_DSPL;
        }
        else
        {
            dataPtr->res = res;
        }

        if (dataPtr->res == SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD)
        {
            dataPtr->additional_info = SAT_SCREEN_BUSY;
        }
        
        memcpy(dataPtr->cmd_detail, (void *)cmdDetail, 5 * sizeof(U8));
        Message->oslDataPtr = (oslParaType *)dataPtr;
        Message->oslPeerBuffPtr = NULL;

#ifdef __MMI_MULTI_SIM__
        mmi_sat_dualsim_sendmsg(Message);
        gSATDisplayText.CMDfromSIM = SAT_CMD_NONE;
#else
	SEND_MSG(Message);
#endif
        gSATContext.lastSATCmd = SAT_NONE_CMD;
    }
}

/*****************************************************************************
 * FUNCTION
 *  SATDisplayTextRsp
 * DESCRIPTION
 *  Protocol Handler for DISPLAY TEXT RES
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATDisplayTextRsp(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_sat_display_text_res_rsp_struct *text_rsp = (mmi_sat_display_text_res_rsp_struct *)msgPtr;
#ifdef __MMI_MULTI_SIM__
    if(text_rsp->sim_id == gSATDisplayText.CMDfromSIM)
#endif
    {
        FreeDisplayText();
    }
}

/*****************************************************************************
 * FUNCTION
 *  SATGetInputInd
 * DESCRIPTION
 *  Protocol Handler for GET INPUT IND
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATGetInputInd(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mmi_sat_get_input_ind_struct *satGetInputInd;
    U16 defaultLen;
    U8 nSimID = 0;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	
    satGetInputInd=(mmi_sat_get_input_ind_struct *) msgPtr;

    nSimID = MTPNP_AD_get_protocol_event_src_mode();

#ifdef __MMI_MULTI_SIM__
    if (gSATSubMenu.CMDfromSIM != (nSimID+SAT_CMD_SIM1) && gSATSubMenu.CMDfromSIM != SAT_CMD_NONE)
    {
        TerminalResponseGetInputEx(MOD_L4C + nSimID, SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, satGetInputInd->cmd_detail);
        if(satGetInputInd->default_text != NULL)
        {
            OslMfree(satGetInputInd->default_text);
        }
        if(satGetInputInd->text_string!= NULL)
        {
            OslMfree(satGetInputInd->text_string);
        }
        return;
    }
    else
    {
        gSATContext.CMDfromSIM = SAT_CMD_SIM1 + nSimID;
    }
#endif
	
    FreeGetInput();
    gSATContext.lastSATCmd = SAT_GET_INPUT;	
    gSATContext.guiBufferPointer = NULL;
    gSATGetInput.sentRsp=0;

    if( GetCurrentState() == CM_OUTGOING_STATE || GetCurrentState() == CM_INCOMING_STATE )
    {
        TerminalResponseGetInput(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, satGetInputInd->cmd_detail);
        if(satGetInputInd->default_text != NULL)
        {
            OslMfree(satGetInputInd->default_text);
        }
        if(satGetInputInd->text_string!= NULL)
        {
            OslMfree(satGetInputInd->text_string);
        }
        return;
    }

    memcpy(gSATGetInput.cmdDetail,satGetInputInd->cmd_detail, 5 * sizeof(U8));
    gSATGetInput.isHelpInfoAvailable = satGetInputInd->is_help_info_available;

    if(satGetInputInd->type_of_input == TYPE_ONLY_DIGIT)
    {
        if(satGetInputInd->is_input_revealed_to_user)
        {
            gSATGetInput.typeOfInput = INPUT_TYPE_PHONE_NUMBER;
        }
        else
        {
            gSATGetInput.typeOfInput = INPUT_TYPE_SAT_NUMERIC_PASSWORD;
        }
    }
    else
    {
        if(satGetInputInd->is_input_revealed_to_user)
        {
            gSATGetInput.typeOfInput = INPUT_TYPE_ALPHANUMERIC_LOWERCASE;
        }
        else
        {
            gSATGetInput.typeOfInput = INPUT_TYPE_SAT_NUMERIC_PASSWORD;
        }
    }

    gSATGetInput.lenOfMinInput=satGetInputInd->len_of_min_input;
    if(satGetInputInd->type_of_input == TYPE_UCS2 && satGetInputInd->len_of_max_input > 70)
    {   
        gSATGetInput.lenOfMaxInput=satGetInputInd->len_of_max_input/2; /* conflict with spec 11.10-4, regard it as byte length */
    }
    else
    {
        gSATGetInput.lenOfMaxInput=satGetInputInd->len_of_max_input; /* according to spec 11.10-4, it is character length */
    }
    
    gSATGetInput.lenOfTextString=satGetInputInd->no_text_string;
    gSATGetInput.dcsOfTextString=satGetInputInd->dcs_of_text_string;
    
    if (gSATGetInput.dcsOfTextString == MMI_8BIT_DCS)
    {
        gSATGetInput.dcsOfTextString = MMI_DEFAULT_DCS;
    }
    gSATGetInput.textString = CovertStringForPlatform(satGetInputInd->text_string, 
                                                                                        gSATGetInput.lenOfTextString, 
                                                                                        gSATGetInput.dcsOfTextString,
                                                                                        &gSATGetInput.lenOfTextString);

    gSATGetInput.lenOfDefaultText = satGetInputInd->no_default_text;
    gSATGetInput.dcsOfDefaultText = satGetInputInd->dcs_of_default_text;
    gSATGetInput.defaultText = CovertStringForPlatform(satGetInputInd->default_text, 
                                                                                            gSATGetInput.lenOfDefaultText, 
                                                                                            gSATGetInput.dcsOfDefaultText,
                                                                                            &defaultLen);
    
    gSATGetInput.lenOfDefaultText = defaultLen;
    if(gSATGetInput.defaultText == NULL)
    {   
        gSATGetInput.dcsOfDefaultText = gSATGetInput.dcsOfTextString;
    }
    
    gSATGetInput.lenOfInputString = gSATGetInput.lenOfMaxInput * ENCODING_LENGTH+ENCODING_LENGTH;
    gSATGetInput.inputString = (U8 *)OslMalloc(gSATGetInput.lenOfInputString);
    if(gSATGetInput.defaultText!=NULL && gSATGetInput.lenOfDefaultText<=gSATGetInput.lenOfInputString)
    {
        memcpy(gSATGetInput.inputString, gSATGetInput.defaultText, gSATGetInput.lenOfDefaultText);
    }
    else
    {   
        memset((void *)gSATGetInput.inputString,0,ENCODING_LENGTH);
    }
    
    if (!(gSATGetInput.cmdDetail[4] & 0x01))                    /* Bit 1 is 0 */
    {
        if (!(gSATGetInput.cmdDetail[4] & 0x02))                /* Bit 2 is 0 */
        {
            if (!(gSATGetInput.cmdDetail[4] & 0x08))
            {
                gSATGetInput.dcsOfInputString = MMI_8BIT_DCS;   /* user input to be in unpacked format */
            }
            else
            {
                gSATGetInput.dcsOfInputString = MMI_DEFAULT_DCS;
            }
        }
        else    /* 010b */
        {
            gSATGetInput.dcsOfInputString = MMI_UCS2_DCS;       /* input UCS2 chars */
        }
    }
    else
    {
        if (!(gSATGetInput.cmdDetail[4] & 0x02))                /* Bit 2 is 0 */
        {
            if (!(gSATGetInput.cmdDetail[4] & 0x08))
            {
                gSATGetInput.dcsOfInputString = MMI_8BIT_DCS;   /* user input to be in unpacked format */
            }
            else
            {
                gSATGetInput.dcsOfInputString = MMI_DEFAULT_DCS;
            }
        }
        else    /* 011b */
        {
            gSATGetInput.dcsOfInputString = MMI_UCS2_DCS;       /* input UCS2 chars */
        }
    }

#ifdef __SAT_MMI_UNIT_TEST__
    gSATGetInput.isIconAvailable = 1;
    if (gSATGetInput.isIconAvailable) 
    {
        gSATGetInput.iconInfo.iconId= 1;
        gSATGetInput.iconInfo.iconAttr= SAT_ICON_NOT_SELF_EX;
    }
#else
    gSATGetInput.isIconAvailable = satGetInputInd->is_icon_available;
    if (gSATGetInput.isIconAvailable)
    {
        gSATGetInput.iconInfo.iconId = satGetInputInd->icon_info.icon_id;
        gSATGetInput.iconInfo.iconAttr = satGetInputInd->icon_info.icon_attr;
    }
#endif

    if (gSATGetInput.isIconAvailable && gSATGetInput.iconInfo.iconAttr == SAT_ICON_SELF_EX) 
    {
        if (gSATGetInput.textString)
        {
            OslMfree(gSATGetInput.textString);
        }
        
        gSATGetInput.textString = NULL;
        gSATGetInput.lenOfTextString = 0;
    }
    
    SATStopWaitTimer();
    DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);		
    if (GetExitScrnID() == SCR_ID_SAT_HISTORY) 
    {
        ClearExitHandler();
    }
    
    if(satGetInputInd->default_text != NULL)
    {
        OslMfree(satGetInputInd->default_text);
    }
    if(satGetInputInd->text_string!= NULL)
    {
        OslMfree(satGetInputInd->text_string);
    }
    
    GoToGetInput();
}

/*****************************************************************************
 * FUNCTION
 *  GoToGetInput
 * DESCRIPTION
 *  Displays Get Input Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void GoToGetInput(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    U8 *iconData = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/	
    if ( gSATGetInput.isIconAvailable)
    {   
        SATAddIconIDToList( gSATGetInput.iconInfo.iconId);
    }
    if (gSATGetInput.isIconAvailable)
    {
        if ( !SATCheckAllIconGot()) 
        {
            SATGetIconCompleteCallBack = GoToGetInput;
            SATGetIconDataToList();
            return;
        }
    }

    iconData = SATGetIconFromList(gSATGetInput.iconInfo.iconId);

    gSATContext.exitCondition = &gSATGetInput.sentRsp;
    EntryNewScreen(SCR_ID_SAT_GETINPUT, SATGenericExitHandler, NULL, NULL);

    SetIsInSATScreen();
    gSATContext.historyFuncPtr = GoToGetInput;
    ClearAllKeyHandler();

    guiBuffer	= GetCurrGuiBuffer(SCR_ID_SAT_GETINPUT);
    if (guiBuffer == NULL)
    {
        memset((void *)gSATGetInput.inputString, 0, gSATGetInput.lenOfMaxInput);
    }
    
    if( gSATGetInput.dcsOfInputString != MMI_UCS2_DCS) 
    {
        gSATGetInput.typeOfInput |= INPUT_TYPE_USE_ONLY_ENGLISH_MODES;
    }

    if((gSATGetInput.typeOfInput & INPUT_TYPE_MASK) == INPUT_TYPE_PHONE_NUMBER || (gSATGetInput.typeOfInput & INPUT_TYPE_MASK) == INPUT_TYPE_SAT_NUMERIC_PASSWORD)
    {
         
        if( (gSATGetInput.typeOfInput & INPUT_TYPE_MASK) == INPUT_TYPE_PHONE_NUMBER )
        {      
            MMI_current_input_ext_type |= INPUT_TYPE_EXT_SIM_NUMERIC;
        }
         

        ShowCategory171Screen((PU8)GetString(STR_GLOBAL_OK),
                                                        (PU8)GetImage(IMG_GLOBAL_OK),
                                                        (PU8)GetString(STR_GLOBAL_BACK),
                                                        (PU8)GetImage(IMG_GLOBAL_BACK),
                                                        gSATGetInput.textString, iconData,
                                                        gSATGetInput.typeOfInput,
                                                        gSATGetInput.inputString,
                                                        gSATGetInput.lenOfMaxInput+1, 
                                                        gSATContext.guiBufferPointer);
        
        SetLeftSoftkeyFunction(TerminalResponseGetInputOK,KEY_EVENT_UP);
    }
    else
    {
        ShowCategory171Screen((PU8)GetString(STR_GLOBAL_OPTIONS),
                                                            (PU8)GetImage(IMG_GLOBAL_OPTIONS),
                                                            (PU8)GetString(STR_GLOBAL_BACK),
                                                            (PU8)GetImage(IMG_GLOBAL_BACK),
                                                            gSATGetInput.textString, 
                                                            iconData,
                                                            gSATGetInput.typeOfInput,
                                                            gSATGetInput.inputString,
                                                            gSATGetInput.lenOfMaxInput+1, 
                                                            gSATContext.guiBufferPointer);
        
        SetLeftSoftkeyFunction(SATEntryScrForInputMethodAndDone, KEY_EVENT_UP);
#if defined(__MMI_GB__)                                                                        
        MMI_allow_only_english_input_modes = 0 ;
        GBSetInputboxLSKFunction(SATEntryScrForInputMethodAndDone);  
#if defined(__MMI_TOUCH_SCREEN__)||defined(__MMI_HANDWRITING_PAD__)
        SetKeyHandler(/*wgui_handle_inputbox_change_input_mode*/mmi_pen_editor_switch_input_method, KEY_POUND, KEY_EVENT_DOWN);
#else
        SetKeyHandler(wgui_handle_inputbox_change_input_mode/*mmi_pen_editor_switch_input_method*/, KEY_POUND, KEY_EVENT_DOWN);
#endif
        SetKeyHandler(GoBackHistory, KEY_END, KEY_EVENT_UP);
#endif	
    }

    if(gSATGetInput.isHelpInfoAvailable) 
    {
        SetKeyHandler(TerminalResponseGetInputHelp, KEY_SEND, KEY_EVENT_UP);
    }
    SetKeyHandler(TerminalResponseGetInputExpire, KEY_END, KEY_EVENT_DOWN);

    StartSATTimerNoResponsefromUser(GetInputTimer);

    SetCategory171RightSoftkeyFunction(TerminalResponseGetInputMoveBack, KEY_EVENT_UP);
}

/*****************************************************************************
 * FUNCTION
 *  SATEntryScrForInputMethodAndDone
 * DESCRIPTION
 *  Displays Input Method And Done Screen from Get Input Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SATEntryScrForInputMethodAndDone(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8* guiBuffer;
    U16 nStrItemList[10];
    U16 numItems;
    U16 itemIcons[10];
    U8 i;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(SAT_SCR_INPUT_METHOD_AND_DONE, NULL, SATEntryScrForInputMethodAndDone, NULL);

    guiBuffer = GetCurrGuiBuffer (SAT_SCR_INPUT_METHOD_AND_DONE);		 
    SetParentHandler (MENU_INPUT_METHOD_AND_DONE_OPTION_ID);
    RegisterHighlightHandler (SATInputMethodAndDoneHiliteHandler);
    numItems = GetNumOfChild (MENU_INPUT_METHOD_AND_DONE_OPTION_ID);
    GetSequenceStringIds(MENU_INPUT_METHOD_AND_DONE_OPTION_ID, nStrItemList);

    for(i=0; ((i<numItems)&&(i<10)); i++)
    {
        itemIcons[i] = IMG_GLOBAL_L1 + i;
    }

    ShowCategory52Screen (STR_GLOBAL_OPTIONS,  
        GetRootTitleIcon(MAIN_MENU_SERVICES_MENUID), 
        STR_GLOBAL_OK, IMG_GLOBAL_OK,
        STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
        numItems, nStrItemList, itemIcons,
        NULL , 0, 0, guiBuffer );
    
    SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  SATInputMethodAndDoneHiliteHandler
 * DESCRIPTION
 *  Hilite Handler for Input Method And Done Screen
 * PARAMETERS
 *  index       [IN]    
 * RETURNS
 *  void
 *****************************************************************************/
void SATInputMethodAndDoneHiliteHandler(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(index==0)
    {
        SetLeftSoftkeyFunction (TerminalResponseGetInputOKDone, KEY_EVENT_UP);
        SetKeyHandler (TerminalResponseGetInputOK, KEY_RIGHT_ARROW, KEY_EVENT_UP);
    }
    else
    {
#if defined(__MMI_GB__)
        MMI_english_input_mode_flag = 0;                                
#endif
        SetLeftSoftkeyFunction (EntryInputMethodScreen, KEY_EVENT_UP);
        SetKeyHandler (EntryInputMethodScreen, KEY_RIGHT_ARROW, KEY_EVENT_UP);
        RegisterInputMethodScreenCloseFunction (SATGoBacknHistoryFromInputMethod);
    }
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseGetInputOKDone
 * DESCRIPTION
 *  Function for sending Terminal Response when Done is pressed
 *  from Input Method & Done Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseGetInputOKDone(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetIsInSATScreen();
    if((U16)(pfnUnicodeStrlen((PS8)gSATGetInput.inputString)) >= gSATGetInput.lenOfMinInput) 
    {
        ClearExitHandler();
    }
    
    TerminalResponseGetInputOK();
    
    if(gSATGetInput.sentRsp)
    {
        DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);	
    }
}

/*****************************************************************************
 * FUNCTION
 *  SATGoBacknHistoryFromInputMethod
 * DESCRIPTION
 *  Takes back the user from Input Method & Done Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SATGoBacknHistoryFromInputMethod(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GoBacknHistory(1);
}

/*****************************************************************************
 * FUNCTION
 *  GetInputTimer
 * DESCRIPTION
 *  Callback function called whn the user shows no activity on
 *  Select Item Screen for 30 secs
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void GetInputTimer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TerminalResponseGetInput(SAT_NO_RESPONSE_FROM_USER,gSATGetInput.cmdDetail);
    
    if (gSATContext.isInSATMenu)
    {
#ifdef __MMI_MULTI_SIM__
        SATSetActiveSimID(gSATContext.satIndex - SAT_CMD_SIM1);
#endif
        gSATContext.historyFuncPtr=GoToSATMainMenu;
        if(gSATContext.isInSATScreen)
        {
            if(IsScreenPresent(SCR_ID_SAT_HISTORY))
            {
                GoBackToHistory(SCR_ID_SAT_HISTORY);
            }
            else
            {
#ifdef __MMI_MULTI_SIM__
                SATSetActiveSimID(gSATContext.satIndex - SAT_CMD_SIM1);
#endif
                GoToSATMainMenu();
            }
        }
    }
    else 
    {
        if(gSATContext.isInSATScreen) 
        {
            GoBackHistory();
        }
        else
        {
            DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  FreeGetInput
 * DESCRIPTION
 *  Frees various elements of Get Input Structure
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void FreeGetInput(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SATClearAllIcons();

    if(gSATGetInput.textString)
    {
        OslMfree(gSATGetInput.textString);
    }
    gSATGetInput.lenOfTextString=0;
    gSATGetInput.textString=NULL;

    if(gSATGetInput.defaultText)
    {
        OslMfree(gSATGetInput.defaultText);
    }		
    gSATGetInput.lenOfDefaultText=0;
    gSATGetInput.defaultText=NULL;

    if(gSATGetInput.inputString)
    {
        OslMfree(gSATGetInput.inputString);
    }		
    gSATGetInput.lenOfInputString=0;
    gSATGetInput.inputString=NULL;
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseGetInputOK
 * DESCRIPTION
 *  Key Handler of LSK of Get Input Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseGetInputOK(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_get_input_res_req_struct    *dataPtr;
    U16 len;
    U16 inputLen;
    U8 *input = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    len=(U16)(pfnUnicodeStrlen((PS8)gSATGetInput.inputString));

    if(len >= gSATGetInput.lenOfMinInput)
    {
        gSATGetInput.sentRsp = 1;
        StopSATTimerNoResponsefromUser();
        gSATContext.noResponseFuncPtr = NULL; 
        
        Message = ALLOCATE_ITM(MOD_MMI);
        Message->oslSrcId = MOD_MMI;
#ifdef __MMI_MULTI_SIM__
        Message->oslDestId = MOD_L4C + gSATContext.CMDfromSIM - SAT_CMD_SIM1;
#else
        Message->oslDestId = MOD_L4C;
#endif
        Message->oslMsgId =  PRT_MMI_SAT_GET_INPUT_RES_REQ;
        dataPtr =  (mmi_sat_get_input_res_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_get_input_res_req_struct)+ (len * ENCODING_LENGTH) - 1);

        if(gSATGetInput.isIconAvailable==1 && gSATContext.isCMDIconSupported==MMI_FALSE)
        {
            dataPtr->res=SAT_CMD_PERFORMED_BUT_ICON_COULD_NOT_DSPL;
        }
        else
        {
            dataPtr->res=SAT_CMD_PERFORMED_SUCCESSFULLY;
        }

        memcpy(dataPtr->cmd_detail, (void *)gSATGetInput.cmdDetail, 5 * sizeof(U8));
        dataPtr->dcs_of_text_string = gSATGetInput.dcsOfInputString;
        
        if (gSATGetInput.dcsOfInputString == MMI_8BIT_DCS)  
        {
            input = CovertStringForSIM(
                            gSATGetInput.inputString,
                            (U16)(len*ENCODING_LENGTH),
                            MMI_DEFAULT_DCS,
                            &inputLen);
        }
        else 
        {
            input = CovertStringForSIM(
                            gSATGetInput.inputString,
                            (U16)(len * ENCODING_LENGTH),
                            gSATGetInput.dcsOfInputString,
                            &inputLen);
        }

        if(dataPtr->dcs_of_text_string == MMI_UCS2_DCS) 
        {
            dataPtr->no_text_string=(U8)inputLen - 2;
        }
        else 
        {
            dataPtr->no_text_string=(U8)inputLen - 1;
        }

        if (dataPtr->no_text_string >= 0xFF) 					//yuxiang 07.6.22
        {
            dataPtr->no_text_string = 0xFF;
            memcpy(dataPtr->text_string, input, 0xFF);	
        }
        else 
        {
            memcpy(dataPtr->text_string, input, dataPtr->no_text_string);
        }

        if (input) 
        {
            OslMfree(input);
        }
        
        Message->oslDataPtr = (oslParaType *)dataPtr;
        Message->oslPeerBuffPtr = NULL;
#ifdef __MMI_MULTI_SIM__
        mmi_sat_dualsim_sendmsg(Message);
        gSATContext.CMDfromSIM = SAT_CMD_NONE;
#else
        SEND_MSG(Message);
#endif          
        gSATContext.lastSATCmd = SAT_NONE_CMD;
        SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
    }
    else
    {
        DisplayPopup((U8*)GetString(SAT_GET_INPUT_LENGTH_NOT_ENOUGH), IMG_GLOBAL_UNFINISHED, 1, SSC_POP_UP_TIME, (U8)ERROR_TONE);
        DeleteScreenIfPresent(SAT_SCR_INPUT_METHOD_AND_DONE);
    }
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseGetInputMoveBack
 * DESCRIPTION
 *  Key Handler of RSK of Get Input Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseGetInputMoveBack(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TerminalResponseGetInput(SAT_BACKWARD_MOVE_IN_PROACTIVE_SESSION_BY_USER,gSATGetInput.cmdDetail);
    SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseGetInputHelp
 * DESCRIPTION
 *  Key Handler of Hot Key of Get Input Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseGetInputHelp(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TerminalResponseGetInput(SAT_HELP_INFO_REQUEST_BY_USER, gSATGetInput.cmdDetail);
    SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseGetInputExpire
 * DESCRIPTION
 *  Key Handler of End Key of Get Input Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseGetInputExpire(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TerminalResponseGetInput(SAT_PROACTIVE_CMD_TERMINATED_BY_USER,gSATGetInput.cmdDetail);
    ExitSATToIdle();
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseGetInput
 * DESCRIPTION
 
 * PARAMETERS
 *  res         [IN]    Response type
 *  cmdDetail   [IN]    Cmd detail to be sent
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseGetInput(U8 res, U8 *cmdDetail)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	MYQUEUE *Message;
	mmi_sat_get_input_res_req_struct    *dataPtr;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gSATGetInput.sentRsp=1;
    StopSATTimerNoResponsefromUser();
    gSATContext.noResponseFuncPtr = NULL;  

    Message=ALLOCATE_ITM(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
#ifdef __MMI_MULTI_SIM__
    Message->oslDestId = MOD_L4C + gSATContext.CMDfromSIM -SAT_CMD_SIM1;
#else
    Message->oslDestId = MOD_L4C;
#endif
    Message->oslMsgId =  PRT_MMI_SAT_GET_INPUT_RES_REQ;
    dataPtr =  (mmi_sat_get_input_res_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_get_input_res_req_struct));
    dataPtr->res=res;
    memcpy(dataPtr->cmd_detail, (void *)cmdDetail, 5*sizeof(U8));
    dataPtr->no_text_string=0;
    dataPtr->dcs_of_text_string=0;	
    dataPtr->text_string[0]=0;
    Message->oslDataPtr = (oslParaType *) dataPtr;
    Message->oslPeerBuffPtr = NULL;
#ifdef __MMI_MULTI_SIM__
    mmi_sat_dualsim_sendmsg(Message);
    gSATContext.CMDfromSIM = SAT_CMD_NONE;
#else
    SEND_MSG(Message);
#endif          
    gSATContext.lastSATCmd = SAT_NONE_CMD;
}

/*****************************************************************************
 * FUNCTION
 *  SATGetInputRsp
 * DESCRIPTION
 *  Protocol Handler for GET INPUT RES
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATGetInputRsp(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    FreeGetInput();
}

/**************************************************************

	FUNCTION NAME		: SATGetInkeyInd

  	PURPOSE				: Protocol Handler for GET INKEY IND

 

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SATGetInkeyInd(void * msgPtr)
{
	mmi_sat_get_inkey_ind_struct *satGetInkeyInd;

	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();

	satGetInkeyInd=(mmi_sat_get_inkey_ind_struct *) msgPtr;
	
	#ifdef __MMI_MULTI_SIM__
	if (gSATSubMenu.CMDfromSIM != (nSimID+SAT_CMD_SIM1) && gSATSubMenu.CMDfromSIM != SAT_CMD_NONE)
	{
		TerminalResponseGetInkeyEx(MOD_L4C + nSimID, SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, satGetInkeyInd->cmd_detail);
		return;
	}
	else
	{
		gSATContext.CMDfromSIM = SAT_CMD_SIM1 + nSimID;
		SATGetInkeyInd(msgPtr);
	}
	#endif
	
	FreeGetInkey();	
	gSATContext.lastSATCmd = SAT_GET_INKEY;	
	gSATContext.guiBufferPointer=NULL;
	gSATGetInkey.sentRsp=0;

	 
	if( GetCurrentState() == CM_OUTGOING_STATE || GetCurrentState() == CM_INCOMING_STATE )
	{
		TerminalResponseGetInkey(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, satGetInkeyInd->cmd_detail);
		return;
	}

	memcpy(gSATGetInkey.cmdDetail,satGetInkeyInd->cmd_detail, 5 * sizeof(U8));
	gSATGetInkey.isHelpInfoAvailable=satGetInkeyInd->is_help_info_available;
	if(satGetInkeyInd->type_of_input == TYPE_ONLY_DIGIT)
		gSATGetInkey.typeOfInput = INPUT_TYPE_PHONE_NUMBER;
	else if(satGetInkeyInd->type_of_input == TYPE_SMS_DEFAULT_SET)
		gSATGetInkey.typeOfInput = INPUT_TYPE_ALPHANUMERIC_LOWERCASE;
	else if(satGetInkeyInd->type_of_input == TYPE_YES_NO)
		gSATGetInkey.typeOfInput = INPUT_TYPE_YES_NO;
	gSATGetInkey.lenOfTextString=satGetInkeyInd->no_text_string;
	gSATGetInkey.dcsOfTextString=satGetInkeyInd->dcs_of_text_string;
//micha0418, spec 12.15,  always gsm default encoding
	if (gSATGetInkey.dcsOfTextString == MMI_8BIT_DCS) {
		gSATGetInkey.dcsOfTextString = MMI_DEFAULT_DCS;
	}
	gSATGetInkey.textString=CovertStringForPlatform(satGetInkeyInd->text_string, gSATGetInkey.lenOfTextString, gSATGetInkey.dcsOfTextString,&gSATGetInkey.lenOfTextString);
	memset(gSATGetInkey.inputString,0,ENCODING_LENGTH);
	if (!(gSATGetInkey.cmdDetail[4] & 0x04)) /*Bit 1 is 0 i.e. character sets defined by bit 1 and bit 2 are enabled*/
	{
		if (!(gSATGetInkey.cmdDetail[4] & 0x01)) /*Bit 1 is 0 */
		{
			gSATGetInkey.dcsOfInputString = MMI_8BIT_DCS; // input digits (0-9, *, #, +) only			
		}
		else
		{
			if (!(gSATGetInkey.cmdDetail[4] & 0x02)) /* Bit 2 is 0 */
			{
				gSATGetInkey.dcsOfInputString = MMI_8BIT_DCS; // input  ascii chars
			}
			else // 011b
			{
				gSATGetInkey.dcsOfInputString= MMI_UCS2_DCS; // input UCS2 chars
			}
		}
	}
	else
	{
		gSATGetInkey.dcsOfInputString = MMI_8BIT_DCS; // input 0x01 or 0x00 by ACCEPT key as "Yes" and END key as "No" }
	}


#ifdef __SAT_MMI_UNIT_TEST__
	gSATGetInkey.isIconAvailable = 1;
	if (gSATGetInkey.isIconAvailable) {
		gSATGetInkey.iconInfo.iconId= 1;
		gSATGetInkey.iconInfo.iconAttr= SAT_ICON_NOT_SELF_EX;
	}
#else
	gSATGetInkey.isIconAvailable = satGetInkeyInd->is_icon_available;
	if (gSATGetInkey.isIconAvailable) {
		gSATGetInkey.iconInfo.iconId = satGetInkeyInd->icon_info.icon_id;
		gSATGetInkey.iconInfo.iconAttr = satGetInkeyInd->icon_info.icon_attr;
	}
#endif

	if (gSATGetInkey.isIconAvailable && gSATGetInkey.iconInfo.iconAttr == SAT_ICON_SELF_EX) {
		if (gSATGetInkey.textString) {
			OslMfree(gSATGetInkey.textString);
		}
		gSATGetInkey.textString = NULL;
		gSATGetInkey.lenOfTextString = 0;
	}
	SATStopWaitTimer();
	DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);		
	if (GetExitScrnID() == SCR_ID_SAT_HISTORY) {
		ClearExitHandler();
	}
	GoToGetInkey();
}

/**************************************************************

	FUNCTION NAME		: GoToGetInkey

  	PURPOSE				: Displays Get Inkey Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void GoToGetInkey(void)
{
	U8* iconData = NULL;

	if ( gSATGetInkey.isIconAvailable)
		SATAddIconIDToList( gSATGetInkey.iconInfo.iconId);

	if (gSATGetInkey.isIconAvailable) {
		if ( !SATCheckAllIconGot()) {
			SATGetIconCompleteCallBack = GoToGetInkey;
			SATGetIconDataToList();
		return;
		}
	}

	iconData = SATGetIconFromList(gSATGetInkey.iconInfo.iconId);

	gSATContext.exitCondition = &gSATGetInkey.sentRsp;
	EntryNewScreen(SCR_ID_SAT_GETINKEY, SATGenericExitHandler, NULL, NULL);
	
	SetIsInSATScreen();
	gSATContext.historyFuncPtr=GoToGetInkey;
	ClearAllKeyHandler();
	if(gSATGetInkey.typeOfInput == INPUT_TYPE_YES_NO)
	{
		U8	*option_list[2];
		option_list[0]=(PU8)GetString(STR_GLOBAL_YES);
		option_list[1]=(PU8)GetString(STR_GLOBAL_NO);

		RegisterHighlightHandler(SATHiliteYesNo);
		ShowCategory172Screen(gSATGetInkey.textString,iconData,
		(U8*)GetString(STR_GLOBAL_OK),NULL,
		(U8*)GetString(STR_GLOBAL_BACK),NULL,
		2,option_list,0,0,gSATContext.guiBufferPointer);
#if (0)  
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif		
		SetLeftSoftkeyFunction(TerminalResponseGetInkeyYesNo,KEY_EVENT_UP);
		SetRightSoftkeyFunction(TerminalResponseGetInkeyMoveBack,KEY_EVENT_UP);
		SetKeyHandler(TerminalResponseGetInkeyExpire,KEY_END,KEY_EVENT_DOWN);
	}
	else
	{
		if( gSATGetInkey.dcsOfInputString != MMI_UCS2_DCS) {
			gSATGetInkey.typeOfInput |= INPUT_TYPE_USE_ONLY_ENGLISH_MODES;
		}

		ShowCategory171Screen((PU8)GetString(STR_GLOBAL_OK),(PU8)GetImage(IMG_GLOBAL_OK),
							(PU8)GetString(STR_GLOBAL_BACK),(PU8)GetImage(IMG_GLOBAL_BACK),
							gSATGetInkey.textString, iconData,
							gSATGetInkey.typeOfInput,
							gSATGetInkey.inputString,2, gSATContext.guiBufferPointer);
			
		SetLeftSoftkeyFunction(TerminalResponseGetInkeyOK,KEY_EVENT_UP);

		SetCategory171RightSoftkeyFunction(TerminalResponseGetInkeyMoveBack,KEY_EVENT_UP);
		SetKeyHandler(TerminalResponseGetInkeyExpire,KEY_END,KEY_EVENT_DOWN);
	}

//micha0328
	if(gSATGetInkey.isHelpInfoAvailable) {
		SetKeyHandler(TerminalResponseGetInkeyHelp,KEY_SEND,KEY_EVENT_UP);
	}
	StartSATTimerNoResponsefromUser(GetInkeyTimer);
}

/**************************************************************

	FUNCTION NAME		: GetInkeyTimer

  	PURPOSE				: Callback function called whn the user shows no activity on
							Select Item Screen for 30 secs

	INPUT PARAMETERS	: id: timer id of the timer

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void GetInkeyTimer(void)
{
	TerminalResponseGetInkey(SAT_NO_RESPONSE_FROM_USER,FALSE, gSATGetInkey.cmdDetail);
	if (gSATContext.isInSATMenu)
	{
	#ifdef __MMI_MULTI_SIM__
		SATSetActiveSimID(gSATContext.satIndex - SAT_CMD_SIM1);
	#endif
		gSATContext.historyFuncPtr=GoToSATMainMenu;
		if(gSATContext.isInSATScreen)
		{
			if(IsScreenPresent(SCR_ID_SAT_HISTORY))
				GoBackToHistory(SCR_ID_SAT_HISTORY);
			else
			{
			#ifdef __MMI_MULTI_SIM__
				SATSetActiveSimID(gSATContext.satIndex - SAT_CMD_SIM1);
			#endif
				GoToSATMainMenu();
			}
		}
	}
	else 
	{
		if(gSATContext.isInSATScreen) 
			GoBackHistory();
		else
			DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);
	}
}

/**************************************************************

	FUNCTION NAME		: FreeGetInkey

  	PURPOSE				: Frees various elements of Get Inkey Structure.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void FreeGetInkey(void)
{
	SATClearAllIcons();

//micha1008
	if(gSATGetInkey.textString)
	{
		OslMfree(gSATGetInkey.textString);
	}
//micha1222
		gSATGetInkey.textString = NULL;
		gSATGetInkey.lenOfTextString=0;
}

/**************************************************************

	FUNCTION NAME		: SATHiliteYesNo

  	PURPOSE				: 
  	
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SATHiliteYesNo(S32 index)
{
	gSATContext.highlightedIndex=(U8)index;	
}

/**************************************************************

	FUNCTION NAME		: TerminalResponseGetInkeyYesNo

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void TerminalResponseGetInkeyYesNo(void)
{
	if(gSATContext.highlightedIndex)
		TerminalResponseGetInkeyNo();
	else
		TerminalResponseGetInkeyYes();
}
/**************************************************************

	FUNCTION NAME		: TerminalResponseGetInkeyOK

  	PURPOSE				: Key Handler of LSK of Get Inkey Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void TerminalResponseGetInkeyOK(void)
{
	MYQUEUE *Message;
	mmi_sat_get_inkey_res_req_struct    *dataPtr;
	U16 len;
	U16 inputLen;
	U8 *input;
	len=(U16)pfnUnicodeStrlen((PS8)gSATGetInkey.inputString);
	if(len == 1)
	{
		gSATGetInkey.sentRsp=1;
		StopSATTimerNoResponsefromUser();
		gSATContext.noResponseFuncPtr = NULL;  

		Message=ALLOCATE_ITM(MOD_MMI);
		Message->oslSrcId = MOD_MMI;
#ifdef __MMI_MULTI_SIM__
	Message->oslDestId = MOD_L4C + gSATContext.CMDfromSIM - SAT_CMD_SIM1;
#else
	Message->oslDestId = MOD_L4C;
#endif
		Message->oslMsgId =  PRT_MMI_SAT_GET_INKEY_RES_REQ;
		dataPtr =  (mmi_sat_get_inkey_res_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_get_inkey_res_req_struct)+len);

		 
		if(gSATGetInkey.isIconAvailable==1 && gSATContext.isCMDIconSupported==MMI_FALSE)
		{
			dataPtr->res=SAT_CMD_PERFORMED_BUT_ICON_COULD_NOT_DSPL;
		}
		else
		{
		 
			dataPtr->res=SAT_CMD_PERFORMED_SUCCESSFULLY;
		}

		memcpy(dataPtr->cmd_detail, (void *)gSATGetInkey.cmdDetail, 5*sizeof(U8));
		dataPtr->is_yes_selected=FALSE;

		dataPtr->dcs_of_text_string=gSATGetInkey.dcsOfInputString;

		if (gSATGetInkey.dcsOfInputString == MMI_8BIT_DCS) 
			input=CovertStringForSIM(gSATGetInkey.inputString,(U16)(len*ENCODING_LENGTH),MMI_DEFAULT_DCS,&inputLen);
		else
			input=CovertStringForSIM(gSATGetInkey.inputString,(U16)(len*ENCODING_LENGTH),dataPtr->dcs_of_text_string,&inputLen);

		if(dataPtr->dcs_of_text_string==MMI_UCS2_DCS)
			dataPtr->no_text_string=(U8)inputLen-2;
		else
			dataPtr->no_text_string=(U8)inputLen-1;
		memcpy(dataPtr->text_string,input,dataPtr->no_text_string);
			OslMfree(input);

		Message->oslDataPtr = (oslParaType *)dataPtr;
		Message->oslPeerBuffPtr = NULL;
#ifdef __MMI_MULTI_SIM__
		mmi_sat_dualsim_sendmsg(Message);
#else
		SEND_MSG(Message);
#endif          
		gSATContext.lastSATCmd = SAT_NONE_CMD;
		SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
	}
}

/**************************************************************

	FUNCTION NAME		: TerminalResponseGetInkeyYes

  	PURPOSE				: Key Handler of Send Key of Get Inkey Screen if Yes/No Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void TerminalResponseGetInkeyYes(void)
{
	TerminalResponseGetInkey(SAT_CMD_PERFORMED_SUCCESSFULLY,TRUE, gSATGetInkey.cmdDetail);
	SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
}


/**************************************************************

	FUNCTION NAME		: TerminalResponseGetInkeyNo

  	PURPOSE				: Key Handler of Send Key of Get Inkey Screen id Yes/No Scree

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void TerminalResponseGetInkeyNo(void)
{
	TerminalResponseGetInkey(SAT_CMD_PERFORMED_SUCCESSFULLY,FALSE, gSATGetInkey.cmdDetail);
	SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
}


/**************************************************************

	FUNCTION NAME		: TerminalResponseGetInkeyMoveBack

  	PURPOSE				: Key Handler of RSK of Get Inkey Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void TerminalResponseGetInkeyMoveBack(void)
{
	TerminalResponseGetInkey(SAT_BACKWARD_MOVE_IN_PROACTIVE_SESSION_BY_USER,FALSE, gSATGetInkey.cmdDetail);
	SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
}


/**************************************************************

	FUNCTION NAME		: TerminalResponseGetInkeyHelp

  	PURPOSE				: Key Handler of Hot Key of Get Inkey Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void TerminalResponseGetInkeyHelp(void)
{
	TerminalResponseGetInkey(SAT_HELP_INFO_REQUEST_BY_USER,FALSE, gSATGetInkey.cmdDetail);
	SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
}


/**************************************************************

	FUNCTION NAME		: TerminalResponseGetInkeyExpire

  	PURPOSE				: Key Handler of End Key of Get Inkey Screen if noy Yes/No Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void TerminalResponseGetInkeyExpire(void)
{
	TerminalResponseGetInkey(SAT_PROACTIVE_CMD_TERMINATED_BY_USER,FALSE, gSATGetInkey.cmdDetail);
	ExitSATToIdle();
}

/**************************************************************

	FUNCTION NAME		: TerminalResponseGetInkey

 

	INPUT PARAMETERS	: Res: response type
							isYesSelected: Is Yes Selected on Get Inkey Screen (TRUE/FALSE)
							cmdDetail: cmd detail to be sent

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void TerminalResponseGetInkey(U8 res, U8 isYesSelected, U8 *cmdDetail)
{
	MYQUEUE *Message;
	mmi_sat_get_inkey_res_req_struct    *dataPtr;
	gSATGetInkey.sentRsp=1;

	StopSATTimerNoResponsefromUser();
	gSATContext.noResponseFuncPtr = NULL;  

	Message=ALLOCATE_ITM(MOD_MMI);
	Message->oslSrcId = MOD_MMI;
#ifdef __MMI_MULTI_SIM__
	Message->oslDestId = MOD_L4C + gSATContext.CMDfromSIM - SAT_CMD_SIM1;
#else
	Message->oslDestId = MOD_L4C;
#endif
	Message->oslMsgId =  PRT_MMI_SAT_GET_INKEY_RES_REQ;
	dataPtr =  (mmi_sat_get_inkey_res_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_get_inkey_res_req_struct));
	dataPtr->res=res;
	dataPtr->is_yes_selected=isYesSelected;
	memcpy(dataPtr->cmd_detail, (void *)cmdDetail, 5*sizeof(U8));
	dataPtr->no_text_string=0;
	dataPtr->dcs_of_text_string=0;
	dataPtr->text_string[0]=0;
	Message->oslDataPtr = (oslParaType *)dataPtr;
	Message->oslPeerBuffPtr = NULL;
#ifdef __MMI_MULTI_SIM__
		mmi_sat_dualsim_sendmsg(Message);
#else
		SEND_MSG(Message);
#endif          
	gSATContext.lastSATCmd = SAT_NONE_CMD;
}

/**************************************************************

	FUNCTION NAME		: SATGetInkeyRsp

  	PURPOSE				: Protocol Handler for GET INKEY RES

 

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SATGetInkeyRsp(void *msgPtr)
{
	FreeGetInkey();
}

/*****************************************************************************
 * FUNCTION
 *  SATCallSetupInd
 * DESCRIPTION
 *  Protocol Handler for CALL SETUP IND
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATCallSetupInd(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mmi_sat_call_setup_ind_struct *callSetupInd;
    U8 *asciiNumber;
    U8 *number;
    U16 numLength;
    U8 nSimID = 0;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/	
    callSetupInd = (mmi_sat_call_setup_ind_struct *) msgPtr;

    nSimID = MTPNP_AD_get_protocol_event_src_mode();
#ifdef __MMI_MULTI_SIM__
    if (gSATSubMenu.CMDfromSIM != (nSimID+SAT_CMD_SIM1) && gSATSubMenu.CMDfromSIM != SAT_CMD_NONE)
    {
        TerminalResponseCallSetupEx(MOD_L4C + nSimID, SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, callSetupInd->cmd_detail);
        return;
    }
    else
    {
        gSATContext.CMDfromSIM = SAT_CMD_SIM1 + nSimID;
    }
#endif
    FreeCallSetup();	
    gSATContext.lastSATCmd = SAT_CALL_SETUP_CMD;		
    gSATContext.guiBufferPointer = NULL;
	
    if(( GetCurrentState() == CM_OUTGOING_STATE) || ( GetCurrentState() == CM_INCOMING_STATE) || isInCall())
    {
        TerminalResponseCallSetup(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, callSetupInd->cmd_detail);
        return;
    }

    if (isInCall() && (callSetupInd->condition == SETUP_CALL_IF_NO_ANOTHER_CALL || callSetupInd->condition == SETUP_CALL_WITH_REDIAL_IF_NO_ANOTHER_CALL))
    {
        TerminalResponseCallSetup(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, SAT_ME_CURRENTLY_BUSY_ON_CALL, callSetupInd->cmd_detail);
        return;
    }
    
    if( !CHECK_MO_ALLOW() )
    {
        ChgrPlayLowBatTone();
        BatteryIndicationPopup(STR_LOW_BATTERY_MT_PROHIBIT);
        TerminalResponseCallSetup(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, callSetupInd->cmd_detail);
        return;
    }
    else 
    {
        SATStopWaitTimer();
    }
    
    if( IS_LOW_BATTERY() )
    {
        ChgrPlayLowBatTone();
        if( g_charbat_context.LowBatteryTxProhibitFlag )
        {
            SET_LOW_BATTERY_ALARM_THRESHOLD( LBAT_TALK_DURCNT_PROHIBIT);
        }
        else
        {
            SET_LOW_BATTERY_ALARM_THRESHOLD( LBAT_TALK_DURCNT_NORMAL);
        }
    }

    memcpy(gSATCallSetup.cmdDetail, callSetupInd->cmd_detail, 5 * sizeof(U8));
    gSATCallSetup.isChangedFromSS = callSetupInd->is_changed_from_ss;
    gSATCallSetup.duration = callSetupInd->duration;
    gSATCallSetup.condition = callSetupInd->condition;
    gSATCallSetup.noAddr = callSetupInd->no_addr;

    if(gSATCallSetup.noAddr > 20) 
    {
        gSATCallSetup.noAddr = 20;
    }

    memcpy(gSATCallSetup.addr, callSetupInd->addr, gSATCallSetup.noAddr);

    gSATCallSetup.noSubaddr=callSetupInd->size_of_subaddr;

    if(gSATCallSetup.noSubaddr > 21) 
    {
        gSATCallSetup.noSubaddr = 21;
    }
	
    memcpy(gSATCallSetup.subAddr,callSetupInd->subaddr,gSATCallSetup.noSubaddr);

    gSATCallSetup.noCcp=callSetupInd->size_of_ccp;
    if(gSATCallSetup.noCcp > 15) 
    {
        gSATCallSetup.noCcp = 15;
    }
    memcpy(gSATCallSetup.ccp, callSetupInd->ccp, gSATCallSetup.noCcp);

    asciiNumber = ConvertBCDToString(gSATCallSetup.addr, (U16)gSATCallSetup.noAddr, &numLength);

    number = CovertStringForPlatform(asciiNumber,(U16)strlen((PS8)asciiNumber),MMI_8BIT_DCS,NULL);
    if(asciiNumber != NULL)
    {
        OslMfree(asciiNumber);
    }
    gSATCallSetup.number = number;
    
    gSATCallSetup.alphaId1Present = 0;
    if (callSetupInd->is_alpha_id1_present)
    {
        if (callSetupInd->no_alpha_id1)
        {
            gSATCallSetup.noAlphaId1 = callSetupInd->no_alpha_id1;
            gSATCallSetup.dcsOfAlphaId1 = callSetupInd->dcs_of_alpha_id1;
            gSATCallSetup.alphaId1 = CovertStringForPlatform(callSetupInd->alpha_id1,
                                                                                                gSATCallSetup.noAlphaId1,
                                                                                                gSATCallSetup.dcsOfAlphaId1,
                                                                                                &gSATCallSetup.noAlphaId1);
            gSATCallSetup.alphaId1Present = 1;
        }
    }
    if(!gSATCallSetup.alphaId1Present)
    {
        gSATCallSetup.noAlphaId1 = pfnUnicodeStrlen((PS8)(PU8)GetString(SAT_SETUP_CALL_CONF_STRING_ID)) * ENCODING_LENGTH;
        gSATCallSetup.alphaId1 = (PU8)OslMalloc(gSATCallSetup.noAlphaId1 + ENCODING_LENGTH);
        memcpy(gSATCallSetup.alphaId1, (PU8)GetString(SAT_SETUP_CALL_CONF_STRING_ID), gSATCallSetup.noAlphaId1 + ENCODING_LENGTH);
    }
    
    gSATCallSetup.alphaId2Present = 0;
    if(callSetupInd->is_alpha_id2_present)
    {
        if(callSetupInd->no_alpha_id2)
        {
            gSATCallSetup.noAlphaId2 = callSetupInd->no_alpha_id2;
            gSATCallSetup.dcsOfAlphaId2 = callSetupInd->dcs_of_alpha_id2;
            gSATCallSetup.alphaId2 = CovertStringForPlatform(callSetupInd->alpha_id2, 
            gSATCallSetup.noAlphaId2,
            gSATCallSetup.dcsOfAlphaId2,
            &gSATCallSetup.noAlphaId2);
            gSATCallSetup.alphaId2Present = 1;
        }
    }
    if(!gSATCallSetup.alphaId2Present)
    {
        gSATCallSetup.noAlphaId2 = 0;
        gSATCallSetup.dcsOfAlphaId2 = 0;
        gSATCallSetup.alphaId2=NULL;
    }

#ifdef __SAT_MMI_UNIT_TEST__
    gSATCallSetup.isIcon1Available = 1;
    if (gSATCallSetup.isIcon1Available) 
    {
        gSATCallSetup.iconInfo1.iconId= 1;
        gSATCallSetup.iconInfo1.iconAttr= SAT_ICON_NOT_SELF_EX;
    }
    gSATCallSetup.isIcon2Available = 1;
    if (gSATCallSetup.isIcon2Available) 
    {
        gSATCallSetup.iconInfo2.iconId= 1;
        gSATCallSetup.iconInfo2.iconAttr= SAT_ICON_NOT_SELF_EX;
    }
#else
    gSATCallSetup.isIcon1Available = callSetupInd->is_icon1_available;
    if (gSATCallSetup.isIcon1Available) 
    {
        gSATCallSetup.iconInfo1.iconId = callSetupInd->icon_info1.icon_id;
        gSATCallSetup.iconInfo1.iconAttr = callSetupInd->icon_info1.icon_attr;
    }
    gSATCallSetup.isIcon2Available = callSetupInd->is_icon2_available;
    if (gSATCallSetup.isIcon2Available) 
    {
        gSATCallSetup.iconInfo2.iconId = callSetupInd->icon_info2.icon_id;
        gSATCallSetup.iconInfo2.iconAttr = callSetupInd->icon_info2.icon_attr;
    }
#endif
    if (gSATCallSetup.isIcon1Available && gSATCallSetup.iconInfo1.iconAttr == SAT_ICON_SELF_EX) 
    {
        if (gSATCallSetup.alphaId1)
        {      
            OslMfree(gSATCallSetup.alphaId1);
        }
        gSATCallSetup.alphaId1 = NULL;
        gSATCallSetup.noAlphaId1 = 0;
    }

    if (gSATCallSetup.isIcon2Available && gSATCallSetup.iconInfo2.iconAttr == SAT_ICON_SELF_EX) 
    {
        if (gSATCallSetup.alphaId2)
        {      
            OslMfree(gSATCallSetup.alphaId2);
        }
        gSATCallSetup.alphaId2 = NULL;
        gSATCallSetup.noAlphaId2 = 0;
    }
    
    DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);		
    if (GetExitScrnID() == SCR_ID_SAT_HISTORY) 
    {
        ClearExitHandler();
    }
    GoToCallConformationScreen();
}

/*****************************************************************************
 * FUNCTION
 *  GoToCallConformationScreen
 * DESCRIPTION
 *  Displays Call Conformation Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void GoToCallConformationScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *iconData = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ( gSATCallSetup.isIcon1Available)
    {
        SATAddIconIDToList( gSATCallSetup.iconInfo1.iconId);
    }
    
    if (gSATCallSetup.isIcon1Available) 
    {
        if ( !SATCheckAllIconGot()) 
        {
            SATGetIconCompleteCallBack = GoToCallConformationScreen;
            SATGetIconDataToList();
            return;
        }
    }

    iconData = SATGetIconFromList(gSATCallSetup.iconInfo1.iconId);

    gSATContext.exitCondition = &gSATCallSetup.passedConfirmScreen;
    EntryNewScreen(SCR_ID_SAT_CMCALLBACK, SATGenericExitHandler, NULL, NULL);

    SetIsInSATScreen();
    gSATContext.historyFuncPtr = GoToCallConformationScreen;
    gSATCallSetup.passedConfirmScreen = 0;
    ClearAllKeyHandler();
    
    ShowCategory151Screen( 0,0,
                                                STR_GLOBAL_YES,0,
                                                STR_GLOBAL_NO,0,
                                                gSATCallSetup.alphaId1, iconData, gSATContext.guiBufferPointer);
    
    SetKeyHandler(TerminalResponseCallSetupExpire,KEY_END,KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction(SATCallSetupStage1Req,KEY_EVENT_UP);
    
    SetKeyHandler(SATCallSetupStage1Req,KEY_SEND,KEY_EVENT_UP);
    SetRightSoftkeyFunction(SATCallSetupTerminalResponseMoveBack,KEY_EVENT_UP);
}



/*****************************************************************************
 * FUNCTION
 *  TerminalResponseCallSetupExpire
 * DESCRIPTION
 *  Key Handler of End Key of Call Conformation Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseCallSetupExpire(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TerminalResponseCallSetup(SAT_PROACTIVE_CMD_TERMINATED_BY_USER, 0, gSATCallSetup.cmdDetail);
    ExitSATToIdle();
}

/**************************************************************

	FUNCTION NAME		: SATCallSetupTerminalResponseMoveBack

  	PURPOSE				: Key Handler of RSK Key of Call Conformation Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/


void SATCallSetupTerminalResponseMoveBack(void)
{
	TerminalResponseCallSetup(SAT_USER_DID_NOT_ACCEPT_THE_PROACTIVE_CMD, 0, gSATCallSetup.cmdDetail);
	SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseCallSetup
 * DESCRIPTION
 
 * PARAMETERS
 *  res         [IN]    Response type
 *  addInfo     [IN]    Additional info
 *  cmdDetail   [IN]    Cmd detail to be sent
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseCallSetup(U8 res, U8 addInfo, U8 *cmdDetail)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_call_setup_stage2_req_struct *dataPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gSATCallSetup.isSentRsp = 1;
    gSATCallSetup.passedConfirmScreen = 1;
    
    Message = ALLOCATE_ITM(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
#ifdef __MMI_MULTI_SIM__
    Message->oslDestId = MOD_L4C + gSATContext.CMDfromSIM - SAT_CMD_SIM1;
#else
    Message->oslDestId = MOD_L4C;
#endif
    Message->oslMsgId =  PRT_MMI_SAT_CALL_SETUP_STAGE2_REQ;
    dataPtr =  (mmi_sat_call_setup_stage2_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_call_setup_stage2_req_struct));

    if(res == SAT_CMD_PERFORMED_SUCCESSFULLY && gSATCallSetup.isIcon1Available==1 
        && gSATContext.isCMDIconSupported==MMI_FALSE)
    {
        dataPtr->res = SAT_CMD_PERFORMED_BUT_ICON_COULD_NOT_DSPL;
    }
    else
    {
        dataPtr->res = res;
    }

    dataPtr->cause = addInfo;
    memcpy(dataPtr->cmd_detail, (void *)cmdDetail, 5 * sizeof(U8));
    Message->oslDataPtr = (oslParaType *)dataPtr;
    Message->oslPeerBuffPtr = NULL;
#ifdef __MMI_MULTI_SIM__
    mmi_sat_dualsim_sendmsg(Message);
    gSATContext.CMDfromSIM = SAT_CMD_NONE;
#else
    SEND_MSG(Message);
#endif          

    gSATContext.lastSATCmd = SAT_NONE_CMD;
}

/*****************************************************************************
 * FUNCTION
 *  SATCallSetupStage1Req
 * DESCRIPTION
 
 *  Key Handler of LSK Key of Call Conformation Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SATCallSetupStage1Req(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mmi_sat_call_setup_stage1_req_struct *dataPtr;
    MYQUEUE *Message;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gSATCallSetup.passedConfirmScreen=1;
    gSATContext.isInSATCall = 1;
    
    Message=ALLOCATE_ITM(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
#ifdef __MMI_MULTI_SIM__
    Message->oslDestId = MOD_L4C + gSATContext.CMDfromSIM - SAT_CMD_SIM1;
#else
    Message->oslDestId = MOD_L4C;
#endif
    Message->oslMsgId =  PRT_MMI_SAT_CALL_SETUP_STAGE1_REQ;
    dataPtr =  (mmi_sat_call_setup_stage1_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_call_setup_stage1_req_struct));
    dataPtr->is_changed_from_ss = gSATCallSetup.isChangedFromSS;
    dataPtr->sat_call_type = gSATCallSetup.condition;
    dataPtr->no_addr = gSATCallSetup.noAddr;
    memcpy(dataPtr->addr, gSATCallSetup.addr, gSATCallSetup.noAddr);

    dataPtr->no_ccp = gSATCallSetup.noCcp;
    memcpy(dataPtr->ccp, gSATCallSetup.ccp, gSATCallSetup.noCcp);

    dataPtr->no_sub_addr = gSATCallSetup.noSubaddr;
    memcpy(dataPtr->sub_addr, gSATCallSetup.subAddr, gSATCallSetup.noSubaddr);

    Message->oslDataPtr = (oslParaType *)dataPtr;
    Message->oslPeerBuffPtr = NULL;
#ifdef __MMI_MULTI_SIM__
    MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, gSATContext.CMDfromSIM - SAT_CMD_SIM1);	
    mmi_sat_dualsim_sendmsg(Message);
#else
    SEND_MSG(Message);
#endif          
    gSATCallSetup.isFromHistory = 0;
    gSATCallSetup.isSentRsp = 0;
    if(gSATCallSetup.condition==SETUP_CALL_PUT_ANOTHER_HOLD || gSATCallSetup.condition==SETUP_CALL_WITH_REDIAL_PUT_ANOTHER_HOLD)
    {
        MakeHold();
        SetCurrentState(CM_HOLD_STATE);
    }
    
    SATCMCallBackScreen();
}

/*****************************************************************************
 * FUNCTION
 *  SATCMCallBackScreen
 * DESCRIPTION
 *  Screen called from history when the contol returns to
 *  SAT Application from CM.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SATCMCallBackScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetIsInSATScreen();
    
    if(!gSATCallSetup.isFromHistory)
    {
        EntryNewScreen(SCR_ID_SAT_CMCALLBACK, SATCMCallBackScreenExitHandler, SATCMCallBackScreen, NULL);

        ClearAllKeyHandler();
        ClearHighlightHandler();
        ShowCategory9Screen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID, NULL);
        ShowOutgoingIndicationScreen(gSATCallSetup.number);
        gSATCallSetup.isFromHistory = 1;
    }
    else
    {
        gSATContext.isInSATCall = 0;

        if(gSATCallSetup.isSentRsp) 
        {
            gSATContext.waitString = SAT_WAIT_STRING_ID;
            gSATContext.waitImage = SAT_WAIT_IMAGE_ID;			
            gSATContext.exitCondition = &gSATContext.waitTimeExpired;
            EntryNewScreen(SCR_ID_SAT_WAITSCREEN, SATGenericExitHandler, NULL, NULL);
            
            ClearHighlightHandler();
            ShowCategory9Screen(gSATContext.waitString, gSATContext.waitImage, NULL);
            SetKeyHandler(0, KEY_END, KEY_EVENT_DOWN);
            gSATContext.historyFuncPtr = PleaseWaitActualScreen;
            StartTimer(SAT_MESSAGE_WAIT_TIMER, 2000, SATMessageWaitTimer);
            gSATContext.waitTimeExpired =0;		
        }
        else
        {
            if(gSATCallSetup.exitCause == SAT_CALL_DISCONNECT_BEFORE_CONNECT)
            {
                TerminalResponseCallSetup(SAT_USER_CLEAR_DOWN_CALL_BEFORE_CONNECTION_RELEASE, 0, gSATCallSetup.cmdDetail);
                SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
            }
            else
            {
                TerminalResponseCallSetup(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, gSATCallSetup.cmdDetail);
                SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
            }
        }
        gSATCallSetup.isFromHistory = 0;
    }
}

/*****************************************************************************
 * FUNCTION
 *  SATCMCallBackScreenExitHandler
 * DESCRIPTION
 *  Exit Handler of CallBack Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SATCMCallBackScreenExitHandler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearIsInSATScreen();
}

/**************************************************************

	FUNCTION NAME		: SATCallExitCause

  	PURPOSE				: Function called by Call Management Application to
							convet to SAT Application the cause of Call End

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SATCallExitCause(U8 cause)
{
	gSATCallSetup.exitCause=cause;
}

/**************************************************************

	FUNCTION NAME		: SATGetCallDisplay

  	PURPOSE				: This is used by CM to get
							Display Information while in Call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

PU8 SATGetCallDisplay(PU8 dcs)
{
	*dcs = gSATCallSetup.dcsOfAlphaId2;
	return gSATCallSetup.alphaId2;
}

/**************************************************************

	FUNCTION NAME		: SATCallRedialStatus

  	PURPOSE				: This is used by CM to indicate to Redial Status of SAT Call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

U8 SATCallRedialStatus(void)
{
	if(gSATCallSetup.condition==SETUP_CALL_WITH_REDIAL_IF_NO_ANOTHER_CALL ||
		gSATCallSetup.condition==SETUP_CALL_WITH_REDIAL_PUT_ANOTHER_HOLD ||
		gSATCallSetup.condition==SETUP_CALL_WITH_REDIAL_DISCON_ANOTHER )
		return TRUE;
	else 
        return FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  SATCallSetupStage1Rsp
 * DESCRIPTION
 *  Protocol Handler for SETUP CALL STAGE 1 RES
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATCallSetupStage1Rsp(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(!gSATCallSetup.isSentRsp)
    {
        mmi_sat_call_setup_stage1_rsp_struct *callSetupStage1Res;
        
        callSetupStage1Res=(mmi_sat_call_setup_stage1_rsp_struct *)msgPtr;
        
        TerminalResponseCallSetup(
            callSetupStage1Res->terminal_res, 
            callSetupStage1Res->additional_res, 
            gSATCallSetup.cmdDetail);
        
        if ( (callSetupStage1Res->terminal_res != SAT_CMD_PERFORMED_SUCCESSFULLY &&
            callSetupStage1Res->terminal_res != SAT_CMD_PERFORMED_WITH_PARTIAL_COMPREHENSION &&
            callSetupStage1Res->terminal_res != SAT_CMD_PERFORMED_WITH_MISSING_INFO)
            ||callSetupStage1Res->result.flag == L4C_ERROR)
        {
            RemoveOutgoingIndicationScreen();   
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  SATCallSetupStage2Rsp
 * DESCRIPTION
 *  Protocol Handler for SETUP CALL STAGE 2 RES
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATCallSetupStage2Rsp(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    FreeCallSetup();
}

/*****************************************************************************
 * FUNCTION
 *  FreeCallSetup
 * DESCRIPTION
 *  Protocol Handler for SETUP CALL STAGE 2 RES
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void FreeCallSetup(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SATClearAllIcons();

    if(gSATCallSetup.alphaId1)
    {
        OslMfree(gSATCallSetup.alphaId1);
    }
    gSATCallSetup.noAlphaId1=0;
    gSATCallSetup.alphaId1=NULL;
    
    if(gSATCallSetup.number)
    {
        OslMfree(gSATCallSetup.number);
        gSATCallSetup.number=NULL;
    }
    
    if(gSATCallSetup.alphaId2)
    {
        OslMfree(gSATCallSetup.alphaId2);
    }
    gSATCallSetup.noAlphaId2=0;
    gSATCallSetup.alphaId2=NULL;
}

/*****************************************************************************
 * FUNCTION
 *  SATSendSMSInd
 * DESCRIPTION
 *  Protocol Handler for SEND SMS IND
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATSendSMSInd(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mmi_sat_send_sms_ind_struct *sendSMSInd;
    U8 nSimID = 0;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    sendSMSInd = (mmi_sat_send_sms_ind_struct *) msgPtr;
    
    nSimID = MTPNP_AD_get_protocol_event_src_mode();
#ifdef __MMI_MULTI_SIM__
    if (gSATSubMenu.CMDfromSIM != (nSimID+SAT_CMD_SIM1) && gSATSubMenu.CMDfromSIM != SAT_CMD_NONE)
    {
        TerminalResponseSendSMS_2(MOD_L4C+nSimID, SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, sendSMSInd->cmd_detail);
        return;
    }
    else
    {
        gSATContext.CMDfromSIM = SAT_CMD_SIM1 + nSimID;
    }
#endif

#ifdef __MMI_MULTI_SIM__
    if(MTPNP_AD_Is_Flight_Mode())
#else
    if(mmi_bootup_get_active_flight_mode() == FLIGHTMODE_SILENT)
#endif
    {
    #ifdef __MMI_MULTI_SIM__
        TerminalResponseSendSMS_2(MOD_L4C+nSimID, SAT_NET_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, sendSMSInd->cmd_detail);
    #else
	TerminalResponseSendSMS(SAT_NET_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, sendSMSInd->cmd_detail);
    #endif
        return;
    }

    FreeSendSms();	
    gSATContext.lastSATCmd = SAT_SEND_SMS_CMD;			
    gSATContext.guiBufferPointer = NULL;
    gSATSendSMS.stage1Res = 0;    
    
    if (isIncomingCall() || isOutgoingCall())
    {
    #ifdef __MMI_MULTI_SIM__
        TerminalResponseSendSMS_2(MOD_L4C+nSimID, SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, sendSMSInd->cmd_detail);
    #else
	TerminalResponseSendSMS(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, sendSMSInd->cmd_detail);
    #endif
        return;
    }    
    
    gSATSendSMS.dcsOfAlphaId = sendSMSInd->dcs_of_alpha_id;
    gSATSendSMS.noAlphaId=sendSMSInd->no_alpha_id;
    
    if(sendSMSInd->is_alpha_id_present)
    {
        if(gSATSendSMS.noAlphaId)
        {
            gSATSendSMS.alphaId = CovertStringForPlatform(
                                                        sendSMSInd->alpha_id, gSATSendSMS.noAlphaId,
                                                        gSATSendSMS.dcsOfAlphaId,
                                                        &gSATSendSMS.noAlphaId);
            gSATSendSMS.informUser = 1;
        }
        else
        {
            gSATSendSMS.informUser = 0;
        }
    }
    else
    {
        gSATSendSMS.informUser=0;
    }
    
    memcpy(gSATSendSMS.cmdDetail,sendSMSInd->cmd_detail, 5 * sizeof(U8));
    
    gSATSendSMS.isSmsPackedRequired = sendSMSInd->is_sms_packed_required;
    
    gSATSendSMS.noAddr = sendSMSInd->size_of_addr;
    if(gSATSendSMS.noAddr<=41) 
    {
        memcpy(gSATSendSMS.addr,sendSMSInd->addr, gSATSendSMS.noAddr);
    }
    else 
    {
        memcpy(gSATSendSMS.addr,sendSMSInd->addr, 41);
    }

    gSATSendSMS.tpduLength=sendSMSInd->no_sms_tpdu;
    if(gSATSendSMS.tpduLength<=175) 
    {
        memcpy(gSATSendSMS.tpdu, sendSMSInd->sms_tpdu, gSATSendSMS.tpduLength);
    }
    else 
    {
        memcpy(gSATSendSMS.tpdu, sendSMSInd->sms_tpdu, 175);
    }

#ifdef __SAT_MMI_UNIT_TEST__
    gSATSendSMS.isIconAvailable = 1;
    if (gSATSendSMS.isIconAvailable) 
    {
        gSATSendSMS.iconInfo.iconId= 1;
        gSATSendSMS.iconInfo.iconAttr= SAT_ICON_NOT_SELF_EX;
    }
#else
    gSATSendSMS.isIconAvailable = sendSMSInd->is_icon_available;
    if (gSATSendSMS.isIconAvailable) 
    {
        gSATSendSMS.iconInfo.iconId = sendSMSInd->icon_info.icon_id;
        gSATSendSMS.iconInfo.iconAttr = sendSMSInd->icon_info.icon_attr;
    }
#endif

    if (gSATSendSMS.isIconAvailable && gSATSendSMS.iconInfo.iconAttr == SAT_ICON_SELF_EX) 
    {
        if (gSATSendSMS.alphaId)
        {      
            OslMfree(gSATSendSMS.alphaId);
        }
        gSATSendSMS.alphaId = NULL;
        gSATSendSMS.noAlphaId = 0;
    }

    if (isIdleScreenReached == 0) 
    {
        gSATSendSMS.informUser = 0;
    }

    if(gSATSendSMS.informUser)
    {
        SATStopWaitTimer();	
        DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);		
        if (GetExitScrnID() == SCR_ID_SAT_HISTORY) 
        {
            ClearExitHandler();
        }
        SATSendSMSStage1Req();
    }
    else
    {
        mmi_sat_restart_please_wait_timer(); /* restart please wait timer if SEND SMS is running in the background */
        SATSendSMSStage1Req();
    }
}

/**************************************************************

	FUNCTION NAME		: GoToSMSConformationScreen

  	PURPOSE				: Displays Send SMS Conformation Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void GoToSMSConformationScreen(void)
{
	gSATContext.exitCondition = &gSATSendSMS.passedConf;
	EntryNewScreen(SCR_ID_SAT_SEND_SMS_CONF, SATGenericExitHandler, NULL, NULL);

	SetIsInSATScreen();
	gSATContext.historyFuncPtr=GoToSMSConformationScreen;
	gSATSendSMS.passedConf=0;
	ClearAllKeyHandler();
	ShowCategory151Screen(0,0,
						 STR_GLOBAL_OK,IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
						 (PU8)GetString(SAT_SEND_SMS_CONF_STRING_ID), 0, gSATContext.guiBufferPointer
						);
	SetKeyHandler(SATSendSmsTerminalResponseExpire,KEY_END,KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(SATSendSMSStage1Req,KEY_EVENT_UP);
	SetRightSoftkeyFunction(SATSendSmsTerminalResponseUnable,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: SATSendSmsTerminalResponseExpire

  	PURPOSE				: Send Terminal Response  for Send SMS 
							if END Key pressed

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SATSendSmsTerminalResponseExpire(void)
{
	TerminalResponseSendSMS(SAT_PROACTIVE_CMD_TERMINATED_BY_USER, 0,gSATSendSMS.cmdDetail);
	ExitSATToIdle();
}

/**************************************************************

	FUNCTION NAME		: void SATSendSmsTerminalResponseUnable(void)

  	PURPOSE				: Send Terminal Response for Send SMS 
							when Unable to process command

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SATSendSmsTerminalResponseUnable(void)
{
	TerminalResponseSendSMS(SAT_BACKWARD_MOVE_IN_PROACTIVE_SESSION_BY_USER, 0,gSATSendSMS.cmdDetail);
	SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
}

/*****************************************************************************
 * FUNCTION
 *  SATSendSMSStage1Req
 * DESCRIPTION
 *  Send SEND SMS STAGE1 REQ
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SATSendSMSStage1Req(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_send_sms_stage1_req_struct *dataPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gSATSendSMS.passedConf = 1;
    if(gSATSendSMS.informUser)
    {
        DisplaySMSWindow();
    }

    gSATContext.gotStage1Rsp = 0;
    
    Message=ALLOCATE_ITM(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
#ifdef __MMI_MULTI_SIM__
    Message->oslDestId = MOD_L4C + gSATContext.CMDfromSIM - SAT_CMD_SIM1;
#else
    Message->oslDestId = MOD_L4C;
#endif
    Message->oslMsgId =  PRT_MMI_SAT_SEND_SMS_STAGE1_REQ;
    dataPtr =  (mmi_sat_send_sms_stage1_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_send_sms_stage1_req_struct));
    dataPtr->is_sms_packed_required = gSATSendSMS.isSmsPackedRequired;
    dataPtr->no_addr = gSATSendSMS.noAddr;
    if(dataPtr->no_addr <= 41) 
    {
        memcpy(dataPtr->addr, gSATSendSMS.addr, dataPtr->no_addr);
    }
    else 
    {
        memcpy(dataPtr->addr, gSATSendSMS.addr, 41);
    }
    dataPtr->tpdu_length = gSATSendSMS.tpduLength;
    if(dataPtr->tpdu_length <= 175) 
    {
        memcpy(dataPtr->tpdu, gSATSendSMS.tpdu, dataPtr->tpdu_length);
    }
    else 
    {
        memcpy(dataPtr->tpdu, gSATSendSMS.tpdu, 175);
    }

    Message->oslDataPtr = (oslParaType *)dataPtr;
    Message->oslPeerBuffPtr = NULL;
#ifdef __MMI_MULTI_SIM__
    mmi_sat_dualsim_sendmsg(Message);
#else
    SEND_MSG(Message);
#endif      
}

/*****************************************************************************
 * FUNCTION
 *  DisplaySMSWindow
 * DESCRIPTION
 *  Displays Sending SMS Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void DisplaySMSWindow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *iconData = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ( gSATSendSMS.isIconAvailable)
    {   
        SATAddIconIDToList( gSATSendSMS.iconInfo.iconId);
    }

    if (gSATSendSMS.isIconAvailable) 
    {
        if ( !SATCheckAllIconGot()) 
        {
            SATGetIconCompleteCallBack = DisplaySMSWindow;
            SATGetIconDataToList();
            return;
        }
    }

    TurnOnBacklight(1);

    iconData = SATGetIconFromList(gSATSendSMS.iconInfo.iconId);

    gSATContext.exitCondition = &gSATSendSMS.stage1Res;
    EntryNewScreen(SCR_ID_SAT_SENDSMS, SATGenericExitHandler, NULL, NULL);

    SetIsInSATScreen();
    gSATContext.historyFuncPtr = DisplaySMSWindow;
    ClearHighlightHandler();
    ShowCategory151Screen(0,0,0,0,0,0,gSATSendSMS.alphaId, iconData, gSATContext.guiBufferPointer);

    ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  SATSendSMSStage1Rsp
 * DESCRIPTION
 *  Protocol Handler for SEND SMS STAGE 1 RES
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATSendSMSStage1Rsp(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mmi_sat_send_sms_stage1_rsp_struct *sendSMSStage1Res;
    sendSMSStage1Res = (mmi_sat_send_sms_stage1_rsp_struct	*) msgPtr;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    gSATSendSMS.stage1Res=1;
    gSATContext.gotStage1Rsp = 1;
	
    if (gSATContext.lastSATCmd == SAT_NONE_CMD) 
    {
        return;
    }
	
    TerminalResponseSendSMS(sendSMSStage1Res->res, (U8)sendSMSStage1Res->cause, gSATSendSMS.cmdDetail);
    if(gSATSendSMS.informUser)
    {
        if ( sendSMSStage1Res->res == SAT_CMD_PERFORMED_SUCCESSFULLY ||
            sendSMSStage1Res->res == SAT_CMD_PERFORMED_WITH_PARTIAL_COMPREHENSION ||
            sendSMSStage1Res->res == SAT_CMD_PERFORMED_WITH_MISSING_INFO)
        {
            SATPleaseWaitScreen(SAT_SMS_SUCCESS_STRING_ID,0);
        }
        else
        {
            SATPleaseWaitScreen(SAT_SMS_ERROR_STRING_ID, 0);
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseSendSMS
 * DESCRIPTION
 
 * PARAMETERS
 *  res         [IN]    Response type
 *  cause       [IN]    Returned in SEND SMS STAGE 1 RES
 *  cmdDetail   [IN]    Cmd detail to be sent
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseSendSMS(U8 res, U8 cause, U8 *cmdDetail)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_send_sms_stage2_req_struct *dataPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gSATSendSMS.passedConf = 1;

    Message = ALLOCATE_ITM(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
#ifdef __MMI_MULTI_SIM__
    Message->oslDestId = MOD_L4C + gSATContext.CMDfromSIM - SAT_CMD_SIM1;
#else
    Message->oslDestId = MOD_L4C;
#endif
    Message->oslMsgId =  PRT_MMI_SAT_SEND_SMS_STAGE2_REQ;
    dataPtr =  (mmi_sat_send_sms_stage2_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_send_sms_stage2_req_struct));

     
    /* if(res==SAT_CMD_PERFORMED_SUCCESSFULLY && gSATSendSMS.isIconAvailable==1 && gSATContext.isCMDIconSupported==MMI_FALSE)
    dataPtr->res=SAT_CMD_PERFORMED_BUT_ICON_COULD_NOT_DSPL;
    else */
    dataPtr->res = res;
    dataPtr->cause = cause;
    memcpy(dataPtr->cmd_detail, cmdDetail, 5*sizeof(U8));
    Message->oslDataPtr = (oslParaType *)dataPtr;
    Message->oslPeerBuffPtr = NULL;
#ifdef __MMI_MULTI_SIM__
    mmi_sat_dualsim_sendmsg(Message);
    gSATContext.CMDfromSIM = SAT_CMD_NONE;
#else
    SEND_MSG(Message);
#endif      
    gSATContext.lastSATCmd = SAT_NONE_CMD;
}

/*****************************************************************************
 * FUNCTION
 *  SATSendSMSStage2Rsp
 * DESCRIPTION
 *  Protocol Handler for SEND SMS STAGE 2 RES
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATSendSMSStage2Rsp(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    FreeSendSms();
}

/*****************************************************************************
 * FUNCTION
 *  FreeSendSms
 * DESCRIPTION
 *  Frees various elements of Send SMS Structure
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void FreeSendSms(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SATClearAllIcons();
    if (gSATSendSMS.alphaId)
    {
        OslMfree(gSATSendSMS.alphaId);
        gSATSendSMS.alphaId = NULL;
    }
}

/**************************************************************

	FUNCTION NAME		: SATSendSSInd

  	PURPOSE				: Protocol Handler for SEND SS IND

 

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SATSendSSInd(void *msgPtr)
{
	mmi_sat_send_ss_ind_struct *sendSSInd;
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();

	sendSSInd=(mmi_sat_send_ss_ind_struct *) msgPtr;
	
	
	#ifdef __MMI_MULTI_SIM__
	if (gSATSubMenu.CMDfromSIM != (nSimID+SAT_CMD_SIM1) && gSATSubMenu.CMDfromSIM != SAT_CMD_NONE)
	{
		TerminalResponseSendSS_2(MOD_L4C+ nSimID, SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, NULL, sendSSInd->cmd_detail);
		return;
	}
	else
	{
		gSATContext.CMDfromSIM = SAT_CMD_SIM1 + nSimID;
	}
	#endif
	gSATContext.lastSATCmd = SAT_SEND_SS_CMD;
	gSATContext.guiBufferPointer=NULL;
	SATSendSSStage1Req(sendSSInd);
}

/**************************************************************

	FUNCTION NAME		: SATSendSSStage1Req

  	PURPOSE				: Send SEND SS STAGE1 REQ

 

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SATSendSSStage1Req(mmi_sat_send_ss_ind_struct *sendSSInd)
{
	MYQUEUE *Message;
	mmi_sat_send_ss_stage1_req_struct    *dataPtr;
	FreeSendSS();	
	gSATSendSS.dcsOfAlphaId=sendSSInd->dcs_of_alpha_id;
	gSATSendSS.noAlphaId=sendSSInd->no_alpha_id;
	gSATSendSS.stage1Res=0;
	if(sendSSInd->is_alpha_id_present)
	{
		if(gSATSendSS.noAlphaId)
		{
			gSATSendSS.alphaId=CovertStringForPlatform(sendSSInd->alpha_id, gSATSendSS.noAlphaId,gSATSendSS.dcsOfAlphaId,&gSATSendSS.noAlphaId);
			gSATSendSS.informUser=1;
		}
		else
		{
			gSATSendSS.informUser=0;
		}
	}
	else
	{
		gSATSendSS.noAlphaId=pfnUnicodeStrlen((PS8)(PU8)GetString(SAT_SEND_SS_STRING_ID))*ENCODING_LENGTH;
		gSATSendSS.alphaId=(PU8)OslMalloc(gSATSendSS.noAlphaId+ENCODING_LENGTH);
		memcpy(gSATSendSS.alphaId,(PU8)GetString(SAT_SEND_SS_STRING_ID),gSATSendSS.noAlphaId+ENCODING_LENGTH);
		gSATSendSS.informUser=1;
	}

#ifdef __SAT_MMI_UNIT_TEST__
	gSATSendSS.isIconAvailable = 1;
	if (gSATSendSS.isIconAvailable) {
		gSATSendSS.iconInfo.iconId= 1;
		gSATSendSS.iconInfo.iconAttr= SAT_ICON_NOT_SELF_EX;
	}
#else
	gSATSendSS.isIconAvailable = sendSSInd->is_icon_available;
	if (gSATSendSS.isIconAvailable) {
		gSATSendSS.iconInfo.iconId = sendSSInd->icon_info.icon_id;
		gSATSendSS.iconInfo.iconAttr = sendSSInd->icon_info.icon_attr;
	}
#endif

	if (gSATSendSS.isIconAvailable && gSATSendSS.iconInfo.iconAttr == SAT_ICON_SELF_EX) {
		if (gSATSendSS.alphaId) {
			OslMfree(gSATSendSS.alphaId);
		}
		gSATSendSS.alphaId = NULL;
		gSATSendSS.noAlphaId = 0;
	}
	
	if(gSATSendSS.informUser)
	{
		SATStopWaitTimer();
		DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);		
		if (GetExitScrnID() == SCR_ID_SAT_HISTORY) {
			ClearExitHandler();
		}
		DisplaySSWindow();
	}

	gSATContext.gotStage1Rsp = 0;
	
#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackSetCallFeatures, PRT_USSDVSS_RSP, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler((PsFuncPtr)PsCBackSetCallFeatures, PRT_USSDVSS_RSP);
#endif
	
	memcpy(gSATSendSS.cmdDetail,sendSSInd->cmd_detail, 5 * sizeof(U8));
	Message=ALLOCATE_ITM(MOD_MMI);
	Message->oslSrcId = MOD_MMI;
#ifdef __MMI_MULTI_SIM__
	Message->oslDestId = MOD_L4C + gSATContext.CMDfromSIM - SAT_CMD_SIM1;
#else
	Message->oslDestId = MOD_L4C;
#endif
	Message->oslMsgId =  PRT_MMI_SAT_SEND_SS_STAGE1_REQ;
	dataPtr =  (mmi_sat_send_ss_stage1_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_send_ss_stage1_req_struct));
	dataPtr->length=sendSSInd->no_ss_string;
	dataPtr->dcs=0x1;
	if(dataPtr->length<=MAX_DIGITS_USSD) {
		memcpy(dataPtr->input,sendSSInd->ss_string, dataPtr->length);
	}
	else {
		memcpy(dataPtr->input,sendSSInd->ss_string, MAX_DIGITS_USSD);
	}
	Message->oslDataPtr = (oslParaType *)dataPtr;
	Message->oslPeerBuffPtr = NULL;
    
#ifdef __MMI_MULTI_SIM__
		mmi_sat_dualsim_sendmsg(Message);
#else
		SEND_MSG(Message);
#endif    
}

/**************************************************************

	FUNCTION NAME		: DisplaySSWindow

  	PURPOSE				: Displays Sending SS Screen

	INPUT PARAMETERS	: alphaId: pointer to string to be displayed in Send SS Screen

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void DisplaySSWindow(void)
{
	U8* iconData = NULL;

	if ( gSATSendSS.isIconAvailable)
		SATAddIconIDToList( gSATSendSS.iconInfo.iconId);

	if (gSATSendSS.isIconAvailable) {
		if ( !SATCheckAllIconGot()) {
			SATGetIconCompleteCallBack = DisplaySSWindow;
			SATGetIconDataToList();
		return;
		}
	}
	iconData = SATGetIconFromList(gSATSendSS.iconInfo.iconId);

	gSATContext.exitCondition = &gSATSendSS.stage1Res;
	EntryNewScreen(SCR_ID_SAT_SENDSS, SATGenericExitHandler, NULL, NULL);

	SetIsInSATScreen();
	gSATContext.historyFuncPtr=DisplaySSWindow;
	ClearHighlightHandler();
	ShowCategory151Screen(0,0,0,0,0,0,
							gSATSendSS.alphaId, iconData, gSATContext.guiBufferPointer);

	SetKeyHandler(ExitSATToIdle,KEY_END,KEY_EVENT_DOWN);
}

/*****************************************************************************
 * FUNCTION
 *  SATSendSSUSSDError
 * DESCRIPTION
 *  Sends USSD Error
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SATSendSSUSSDError(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (gSATContext.lastSATCmd)
    {
        case SAT_SEND_SS_CMD:
            TerminalResponseSendSS(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, NULL, gSATSendSS.cmdDetail);
            if (gSATSendSS.informUser)
            {
                SATPleaseWaitScreen(SAT_SS_ERROR_STRING_ID, 0);
            }
            break;
    }
}

/**************************************************************

	FUNCTION NAME		: TerminalResponseSATDisplaySSStage1MoveBack

  	PURPOSE			: 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: nil

	RETURNS			: nil

 

**************************************************************/
void TerminalResponseSATDisplaySSStage1MoveBack(void)
{
	if(!gSATSendSS.sentRsp)
	{
		gSATSendSS.sentRsp = 1;
		TerminalResponseSendSS(SAT_CMD_PERFORMED_SUCCESSFULLY, gSATSendSS.ssStringLen, gSATSendSS.ssString, gSATSendSS.cmdDetail);
	}

	if(gSATSendSS.textString)
		OslMfree(gSATSendSS.textString);
	if(gSATSendSS.ssString)
		OslMfree(gSATSendSS.ssString);
	gSATSendSS.textString = NULL;
	gSATSendSS.ssString = NULL;

//	if(gSATSendSS.informUser)
		SetIsInSATScreen();
		SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
}

/**************************************************************

	FUNCTION NAME		: TerminalResponseSATDisplaySSStage1Expire

  	PURPOSE			: 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: nil

	RETURNS			: nil

 

**************************************************************/
void TerminalResponseSATDisplaySSStage1Expire(void)
{
	if(!gSATSendSS.sentRsp)
	{
		gSATSendSS.sentRsp = 1;
		TerminalResponseSendSS(SAT_CMD_PERFORMED_SUCCESSFULLY, gSATSendSS.ssStringLen, gSATSendSS.ssString, gSATSendSS.cmdDetail);
	}

	if(gSATSendSS.textString)
		OslMfree(gSATSendSS.textString);
	if(gSATSendSS.ssString)
		OslMfree(gSATSendSS.ssString);

	gSATSendSS.textString = NULL;
	gSATSendSS.ssString = NULL;

	ExitSATToIdle();
}

/**************************************************************

	FUNCTION NAME		: GoToSATDisplaySSStage1Rsp

  	PURPOSE			: 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: nil

	RETURNS			: nil

 

**************************************************************/
void GoToSATDisplaySSStage1Rsp(void)
{
	gSATContext.exitCondition = &gSATSendSS.sentRsp;
	EntryNewScreen(SCR_ID_SAT_DISPLAY_SS, SATGenericExitHandler, NULL, NULL);

	SetIsInSATScreen();
	gSATContext.historyFuncPtr=GoToSATDisplaySSStage1Rsp;
	ClearAllKeyHandler();

	ShowCategory151Screen(  0, 0, 
				STR_GLOBAL_OK, IMG_GLOBAL_OK,
				0, 0, //STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				gSATSendSS.textString, NULL, gSATContext.guiBufferPointer);

	SetKeyHandler(TerminalResponseSATDisplaySSStage1Expire, KEY_END, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(TerminalResponseSATDisplaySSStage1MoveBack, KEY_EVENT_UP);
	//SetRightSoftkeyFunction(TerminalResponseSATDisplaySSStage1MoveBack, KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: SATSendSSStage1Rsp

  	PURPOSE				: Protocol Handler for SEND SS STAGE 1 RES

 

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SATSendSSStage1Rsp(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mmi_sat_send_ss_stage1_rsp_struct *sendSSStage1Res;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    sendSSStage1Res = (mmi_sat_send_ss_stage1_rsp_struct*) msgPtr;

    gSATContext.gotStage1Rsp = 1;

    if (gSATContext.lastSATCmd == SAT_NONE_CMD)
    {
        return;
    }

    if (gSATContext.lastSATCmd != SAT_SEND_SS_CMD)
    {
        SATSendSSUSSDError();
        gSATContext.lastSATCmd = SAT_NONE_CMD;
        return;
    }
    else
    {
        gSATContext.lastSATCmd = SAT_NONE_CMD;
    }

    gSATSendSS.stage1Res = 1;

     
    if ((sendSSStage1Res->res == SAT_CMD_PERFORMED_SUCCESSFULLY)
        && (sendSSStage1Res->length > 8) && (sendSSStage1Res->addition_info[0] == 0x3B))
    {
        /* expected sms 7-bit packed ss string: 0x3B 0x.. (0x81) 0x.. 0x.. 0x.. 0x.. 0x.. length data...... */
        U8 tempss[256];

        gSATSendSS.ssString = OslMalloc(sendSSStage1Res->length);
        gSATSendSS.textString = OslMalloc(512);
        memset(gSATSendSS.textString, 0, 512);
        memset(tempss, 0, 256);

        /* copy the sms 7-bit packed ss for terminal response */
        gSATSendSS.ssStringLen = sendSSStage1Res->length;
        memcpy(gSATSendSS.ssString, sendSSStage1Res->addition_info, sendSSStage1Res->length);

        /* ss format check */
        /* unpack it and then transfer to unicode for display */
        #if 0
        if (sendSSStage1Res->addition_info[2] == 0x81)
        {
            smsal_gsm7_unpack(
                (PU8) (&sendSSStage1Res->addition_info[9]),
                tempss,
                smsal_msg_len_in_character(SMSAL_DEFAULT_DCS, sendSSStage1Res->addition_info[8], sendSSStage1Res->addition_info[8 + sendSSStage1Res-> addition_info[8]]),
                0);
        }
        else
        {
            smsal_gsm7_unpack(
                (PU8) (&sendSSStage1Res->addition_info[8]),
                tempss,
                smsal_msg_len_in_character(SMSAL_DEFAULT_DCS, sendSSStage1Res->addition_info[7], sendSSStage1Res->addition_info[7 + sendSSStage1Res-> addition_info[7]]),
                0);
        }
        #endif
        mmi_asc_to_ucs2((PS8) gSATSendSS.textString, (PS8) tempss);

        /* show the unpacked ss string */
        gSATSendSS.sentRsp = 0;
        SATStopWaitTimer();
        GoToSATDisplaySSStage1Rsp();
    }
    else
    {
        TerminalResponseSendSS(
            sendSSStage1Res->res,
            sendSSStage1Res->length,
            sendSSStage1Res->addition_info,
            gSATSendSS.cmdDetail);
        if (gSATSendSS.informUser)
        {
            if (sendSSStage1Res->res == SAT_CMD_PERFORMED_SUCCESSFULLY ||
                sendSSStage1Res->res == SAT_CMD_PERFORMED_WITH_PARTIAL_COMPREHENSION ||
                sendSSStage1Res->res == SAT_CMD_PERFORMED_WITH_MISSING_INFO)
            {
                SATPleaseWaitScreen(SAT_SS_SUCCESS_STRING_ID, 0);
            }
            else
            {
                SATPleaseWaitScreen(SAT_SS_ERROR_STRING_ID, 0);
            }
        }
    }
     
}

/**************************************************************

	FUNCTION NAME		: TerminalResponseSendSS

 

	INPUT PARAMETERS	: Res: response type
							len: len of additional info, returned in SEND SS STAGE 1 RES
							info: additional info, returned in SEND SS STAGE 1 RES
							cmdDetail: cmd detail to be sent

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void TerminalResponseSendSS(U8 res, U8 len, U8* info, U8 *cmdDetail)
{
	MYQUEUE *Message;
	mmi_sat_send_ss_stage2_req_struct    *dataPtr;
	Message=ALLOCATE_ITM(MOD_MMI);
	Message->oslSrcId = MOD_MMI;
#ifdef __MMI_MULTI_SIM__
	Message->oslDestId = MOD_L4C + gSATContext.CMDfromSIM - SAT_CMD_SIM1;
#else
	Message->oslDestId = MOD_L4C;
#endif
	Message->oslMsgId =  PRT_MMI_SAT_SEND_SS_STAGE2_REQ;
	dataPtr =  (mmi_sat_send_ss_stage2_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_send_ss_stage2_req_struct));

	 
	if(res==SAT_CMD_PERFORMED_SUCCESSFULLY && gSATSendSS.isIconAvailable==1 && gSATContext.isCMDIconSupported==MMI_FALSE)
	{
		dataPtr->res=SAT_CMD_PERFORMED_BUT_ICON_COULD_NOT_DSPL;
	}
	else
	{
	 
		dataPtr->res=res;
	}

	dataPtr->size_of_additional_info=len;

	if(dataPtr->size_of_additional_info>=255) {
		dataPtr->size_of_additional_info=255;
	}

	memcpy(dataPtr->additional_info,info, dataPtr->size_of_additional_info);
	memcpy(dataPtr->cmd_detail, (void *)cmdDetail, 5*sizeof(U8));
	Message->oslDataPtr = (oslParaType *)dataPtr;
	Message->oslPeerBuffPtr = NULL;
#ifdef __MMI_MULTI_SIM__
		mmi_sat_dualsim_sendmsg(Message);
#else
		SEND_MSG(Message);
#endif   
        gSATContext.lastSATCmd = SAT_NONE_CMD;
}


/**************************************************************

	FUNCTION NAME		: SATSendSSStage2Rsp

  	PURPOSE				: Protocol Handler for SEND SS STAGE 2 RES

 

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SATSendSSStage2Rsp(void *msgPtr)
{
	FreeSendSS();
	
}

/**************************************************************

	FUNCTION NAME		: FreeSendSS

  	PURPOSE				: Frees various elements of Send SS Structure

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void FreeSendSS(void)
{
	SATClearAllIcons();
//micha1008
	if(gSATSendSS.alphaId)
	{
		OslMfree(gSATSendSS.alphaId);
	}
//micha1222
		gSATSendSS.alphaId = NULL;
		gSATSendSS.noAlphaId=0;
}

/*****************************************************************************
 * FUNCTION
 *  SATSendUSSDInd
 * DESCRIPTION
 *  Protocol Handler for SEND USSD IND
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATSendUSSDInd(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mmi_sat_send_ussd_ind_struct *sendUSSDInd;
    U8 nSimID;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/	
    sendUSSDInd=(mmi_sat_send_ussd_ind_struct *) msgPtr;
    SetAbortCallRequestedFlag(FALSE);
   
    nSimID = MTPNP_AD_get_protocol_event_src_mode();
#ifdef __MMI_MULTI_SIM__
   if (gSATSubMenu.CMDfromSIM != (nSimID+SAT_CMD_SIM1) && gSATSubMenu.CMDfromSIM != SAT_CMD_NONE)
    {
        TerminalResponseSendUSSD_2(MOD_L4C + nSimID, SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, NULL, sendUSSDInd->cmd_detail);
        return;
    }
    else
    {
        gSATContext.CMDfromSIM = SAT_CMD_SIM1 + nSimID;
    }
#endif

    FreeSendUSSD();	

    gSATContext.lastSATCmd = SAT_SEND_USSD_CMD;
    gSATContext.guiBufferPointer = NULL;
    gSATSendUSSD.stage1Res = 0;
    
    if (sendUSSDInd->is_icon_available && sendUSSDInd->icon_info.icon_attr != SAT_ICON_SELF_EX && !sendUSSDInd->is_alpha_id_present)
    {
        TerminalResponseSendUSSD(SAT_CMD_DATA_NOT_UNDERSTAND_BY_ME, 0, NULL, sendUSSDInd->cmd_detail);
        return;
    }

    gSATSendUSSD.dcsOfAlphaId = sendUSSDInd->dcs_of_alpha_id;
    gSATSendUSSD.noAlphaId = sendUSSDInd->no_alpha_id;
    if(sendUSSDInd->is_alpha_id_present)
    {
        if(gSATSendUSSD.noAlphaId)
        {
            gSATSendUSSD.alphaId = CovertStringForPlatform(
                                                        sendUSSDInd->alpha_id, 
                                                        gSATSendUSSD.noAlphaId,
                                                        gSATSendUSSD.dcsOfAlphaId,
                                                        &gSATSendUSSD.noAlphaId);
            gSATSendUSSD.informUser = 1;
        }
        else
        {
            gSATSendUSSD.informUser = 0;
        }
    }
    else
    {
        gSATSendUSSD.noAlphaId = pfnUnicodeStrlen((PS8)(PU8)GetString(SAT_SEND_USSD_STRING_ID))*ENCODING_LENGTH;
        gSATSendUSSD.alphaId = (PU8)OslMalloc(gSATSendUSSD.noAlphaId+ENCODING_LENGTH);
        memcpy(gSATSendUSSD.alphaId, (PU8)GetString(SAT_SEND_USSD_STRING_ID), gSATSendUSSD.noAlphaId+ENCODING_LENGTH);
        gSATSendUSSD.informUser=1;
    }

#ifdef __SAT_MMI_UNIT_TEST__
    gSATSendUSSD.isIconAvailable = 1;
    if (gSATSendUSSD.isIconAvailable) 
    {
        gSATSendUSSD.iconInfo.iconId= 1;
        gSATSendUSSD.iconInfo.iconAttr= SAT_ICON_NOT_SELF_EX;
    }
#else
    gSATSendUSSD.isIconAvailable = sendUSSDInd->is_icon_available;
    if (gSATSendUSSD.isIconAvailable) 
    {
        gSATSendUSSD.iconInfo.iconId = sendUSSDInd->icon_info.icon_id;
        gSATSendUSSD.iconInfo.iconAttr = sendUSSDInd->icon_info.icon_attr;
    }
#endif
    if (gSATSendUSSD.isIconAvailable && gSATSendUSSD.iconInfo.iconAttr == SAT_ICON_SELF_EX) 
    {
        if (gSATSendUSSD.alphaId) 
        {
            OslMfree(gSATSendSS.alphaId);
        }
        gSATSendUSSD.alphaId = NULL;
        gSATSendUSSD.noAlphaId = 0;
    }
		
    if(gSATSendUSSD.informUser)
    {
        
        DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);		
        if (GetExitScrnID() == SCR_ID_SAT_HISTORY) 
        {
            ClearExitHandler();
        }
        DisplayUSSDWindow();
    }
    SATStopWaitTimer();
    SATSendUSSDStage1Req(sendUSSDInd);
}

/*****************************************************************************
 * FUNCTION
 *  SATSendUSSDStage1Req
 * DESCRIPTION
 *  Send SEND USSD STAGE1 REQ
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATSendUSSDStage1Req(mmi_sat_send_ussd_ind_struct *sendUSSDInd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_send_ussd_stage1_req_struct *dataPtr;
    int len = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* remove any USSD screen in the history to ensure that the coming USSD is displayed in the top screen */
    RemoveSSReqScr();
    	
#ifdef __MMI_MULTI_SIM__
    MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, gSATContext.CMDfromSIM - SAT_CMD_SIM1);
    MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackSetCallFeatures, PRT_USSDVSS_RSP, MTPNP_AD_CALL_CHANNEL);
#else
    SetProtocolEventHandler((PsFuncPtr)PsCBackSetCallFeatures, PRT_USSDVSS_RSP);
#endif

    gSATContext.gotStage1Rsp = 0;
    memcpy(gSATSendUSSD.cmdDetail,sendUSSDInd->cmd_detail, 5 * sizeof(U8));
    Message=ALLOCATE_ITM(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
#ifdef __MMI_MULTI_SIM__
    Message->oslDestId = MOD_L4C + gSATContext.CMDfromSIM - SAT_CMD_SIM1;
#else
    Message->oslDestId = MOD_L4C;
#endif
    Message->oslMsgId =  PRT_MMI_SAT_SEND_USSD_STAGE1_REQ;
    dataPtr =  (mmi_sat_send_ussd_stage1_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_send_ussd_stage1_req_struct));
    if(sendUSSDInd->no_ussd_string>=MAX_DIGITS_USSD) 
    {
        len=MAX_DIGITS_USSD-1;
    }
    else 
    {
        len=sendUSSDInd->no_ussd_string;
    }
    memcpy(dataPtr->input,sendUSSDInd->ussd_string, len);
    dataPtr->input[len] = 0;
    dataPtr->dcs = sendUSSDInd->dcs_of_ussd_string;
    dataPtr->length = len;
    Message->oslDataPtr = (oslParaType *)dataPtr;
    Message->oslPeerBuffPtr = NULL;
#ifdef __MMI_MULTI_SIM__
    mmi_sat_dualsim_sendmsg(Message);
#else
    SEND_MSG(Message);
#endif    
}

/*****************************************************************************
 * FUNCTION
 *  DisplayUSSDWindow
 * DESCRIPTION
 *  Displays Sending USSD Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void DisplayUSSDWindow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *iconData = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ( gSATSendUSSD.isIconAvailable)
    {   
        SATAddIconIDToList( gSATSendUSSD.iconInfo.iconId);
    }
    
    if (gSATSendUSSD.isIconAvailable) 
    {
        if ( !SATCheckAllIconGot())
        {
            SATGetIconCompleteCallBack = DisplayUSSDWindow;
            SATGetIconDataToList();
            return;
        }
    }
    
    if (gSATSendUSSD.isIconAvailable && gSATSendUSSD.iconInfo.iconAttr == SAT_ICON_SELF_EX &&
        SATGetIconFromList(gSATSendUSSD.iconInfo.iconId) != NULL)
    {
        if (gSATSendUSSD.alphaId)
        {
            OslMfree(gSATSendUSSD.alphaId);
            gSATSendUSSD.alphaId = NULL;
        }
    }    
    
    iconData = SATGetIconFromList(gSATSendUSSD.iconInfo.iconId);

    gSATContext.exitCondition = &gSATSendUSSD.stage1Res;
    EntryNewScreen(SCR_ID_SAT_SENDUSSD, SATGenericExitHandler, NULL, NULL);

    SetIsInSATScreen();
    gSATContext.historyFuncPtr = DisplayUSSDWindow;
    ClearHighlightHandler();
    ShowCategory151Screen(0,0,0,0,0,0,
                gSATSendUSSD.alphaId, iconData, gSATContext.guiBufferPointer);

    ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseSATDisplayUSSDStage1MoveBack
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseSATDisplayUSSDStage1MoveBack(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!gSATSendUSSD.sentRsp)
    {
        gSATSendUSSD.sentRsp = 1;

        TerminalResponseSendUSSD(
            SAT_CMD_PERFORMED_SUCCESSFULLY,
            gSATSendUSSD.ussdStringLen,
            gSATSendUSSD.ussdString,
            gSATSendUSSD.cmdDetail);
    }

    if (gSATSendUSSD.textString)
    {
        OslMfree(gSATSendUSSD.textString);
        gSATSendUSSD.textString = NULL;
    }
    if (gSATSendUSSD.ussdString)
    {
        OslMfree(gSATSendUSSD.ussdString);
        gSATSendUSSD.ussdString = NULL;
    }

    RemoveSSReqScr();
    SetIsInSATScreen();
    SATPleaseWaitScreen(STR_GLOBAL_PLEASE_WAIT, SAT_WAIT_IMAGE_ID);
}


/*****************************************************************************
 * FUNCTION
 *  TerminalResponseSATDisplayUSSDStage1Expire
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseSATDisplayUSSDStage1Expire(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!gSATSendUSSD.sentRsp)
    {
        gSATSendUSSD.sentRsp = 1;
        TerminalResponseSendUSSD(
            SAT_CMD_PERFORMED_SUCCESSFULLY,
            gSATSendUSSD.ussdStringLen,
            gSATSendUSSD.ussdString,
            gSATSendUSSD.cmdDetail);
    }

    if (gSATSendUSSD.textString)
    {
        OslMfree(gSATSendUSSD.textString);
        gSATSendUSSD.textString = NULL;
    }
    if (gSATSendUSSD.ussdString)
    {
        OslMfree(gSATSendUSSD.ussdString);
        gSATSendUSSD.ussdString = NULL;
    }

    ExitSATToIdle();
}

/*****************************************************************************
 * FUNCTION
 *  GoToSATDisplayUSSDStage1Rsp
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void GoToSATDisplayUSSDStage1Rsp(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gSATContext.exitCondition = &gSATSendUSSD.sentRsp;
    EntryNewScreen(SCR_ID_SAT_DISPLAY_USSD, SATGenericExitHandler, NULL, NULL);

    SetIsInSATScreen();
    gSATContext.historyFuncPtr = GoToSATDisplayUSSDStage1Rsp;

    ShowCategory151Screen_ex(
        0,
        0,
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        0,
        0,  /* STR_GLOBAL_BACK, IMG_GLOBAL_BACK, */
        gSATSendUSSD.textString,
        NULL,
        gSATContext.guiBufferPointer);

    SetKeyHandler(TerminalResponseSATDisplayUSSDStage1Expire, KEY_END, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction(TerminalResponseSATDisplayUSSDStage1MoveBack, KEY_EVENT_UP);
    /* SetRightSoftkeyFunction(TerminalResponseSATDisplayUSSDStage1MoveBack, KEY_EVENT_UP); */
}


/*****************************************************************************
 * FUNCTION
 *  SATSendUSSDStage1Rsp
 * DESCRIPTION
 *  Protocol Handler for SEND USSD STAGE 1 RES
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATSendUSSDStage1Rsp(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mmi_sat_send_ussd_stage1_rsp_struct	*sendUSSDStage1Res;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    sendUSSDStage1Res = (mmi_sat_send_ussd_stage1_rsp_struct *) msgPtr;

    gSATContext.gotStage1Rsp = 1;
    
    if (gSATContext.lastSATCmd == SAT_NONE_CMD) 
    {
        return;
    }

    if (gSATContext.lastSATCmd != SAT_SEND_USSD_CMD) 
    {
        SATSendSSUSSDError();
        gSATContext.lastSATCmd = SAT_NONE_CMD;
        return;
    }		
    else
    {
        gSATContext.lastSATCmd = SAT_NONE_CMD;
    }	

    gSATSendUSSD.stage1Res=1;

#if 0
    /* Display USSD response */
    if (sendUSSDStage1Res->res == SAT_CMD_PERFORMED_SUCCESSFULLY && sendUSSDStage1Res->addition_info[0] == 0x11)
    {
        U16 noAlphaId;

        /* reject display text when send USSD response display */
        gSATContext.lastSATCmd = 0xFF;

        /* expected ussd string: 0x11 (0x.. 0x..) 0x.. 0x.. 0x.. 0x.. */
        gSATSendUSSD.ussdString = OslMalloc(sendUSSDStage1Res->length);

        /* copy the ussd for terminal response */
        gSATSendUSSD.ussdStringLen = sendUSSDStage1Res->length - 1;
        memcpy(gSATSendUSSD.ussdString, sendUSSDStage1Res->addition_info - 1, sendUSSDStage1Res->length - 1);
        gSATSendUSSD.ussdString[0] = 0x04; /* Use 0x04 to response */

        /* ignore 0x11 & the following two bytes */
        gSATSendUSSD.textString = CovertStringForPlatform(
                                                        sendUSSDStage1Res->addition_info+ 2,
                                                        (U16) (sendUSSDStage1Res->length - 2),
                                                        sendUSSDStage1Res->addition_info[1],
                                                        &noAlphaId);

        /* show the ussd string */
        gSATSendUSSD.sentRsp = 0;
        SATStopWaitTimer();
        GoToSATDisplayUSSDStage1Rsp();
        return;
    }
#endif
    TerminalResponseSendUSSD(
        sendUSSDStage1Res->res, 
        sendUSSDStage1Res->length, 
        sendUSSDStage1Res->addition_info, 
        gSATSendUSSD.cmdDetail);
    
    if(gSATSendUSSD.informUser)
    {
        if ( sendUSSDStage1Res->res==SAT_CMD_PERFORMED_SUCCESSFULLY ||
        sendUSSDStage1Res->res==SAT_CMD_PERFORMED_WITH_PARTIAL_COMPREHENSION ||
        sendUSSDStage1Res->res==SAT_CMD_PERFORMED_WITH_MISSING_INFO)
        {
            SATPleaseWaitScreen(SAT_USSD_SUCCESS_STRING_ID, 0);
        }
        else if (sendUSSDStage1Res->res == SAT_USSD_SS_TRANSACTION_TERMINATION_BY_USER)
        {
            SATPleaseWaitScreen(SAT_ABORT_STRING_ID, 0);
        }
        else
        {
            SATPleaseWaitScreen(SAT_USSD_ERROR_STRING_ID, 0);
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseSendUSSD
 * DESCRIPTION
 
 * PARAMETERS
 *  res         [IN]    Response type
 *  len         [IN]    Len of additional info, returned in SEND USSD STAGE 1 RES
 *  info        [IN]    Additional info, returned in SEND USSD STAGE 1 RES
 *  cmdDetail   [IN]    Cmd detail to be sent
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseSendUSSD(U8 res, U8 len, U8 *info, U8 *cmdDetail)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_send_ussd_stage2_req_struct *dataPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Message=ALLOCATE_ITM(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
#ifdef __MMI_MULTI_SIM__
    Message->oslDestId = MOD_L4C + gSATContext.CMDfromSIM - SAT_CMD_SIM1;
#else
    Message->oslDestId = MOD_L4C;
#endif
    Message->oslMsgId =  PRT_MMI_SAT_SEND_USSD_STAGE2_REQ;
    dataPtr =  (mmi_sat_send_ussd_stage2_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_send_ussd_stage2_req_struct));

     
    if(res == SAT_CMD_PERFORMED_SUCCESSFULLY && gSATSendUSSD.isIconAvailable==1 && gSATContext.isCMDIconSupported==MMI_FALSE)
    {
        dataPtr->res=SAT_CMD_PERFORMED_BUT_ICON_COULD_NOT_DSPL;
    }
    else
    {
     
        dataPtr->res=res;
    }

    dataPtr->size_of_additional_info = len;
    if(dataPtr->size_of_additional_info >= 255) 
    {           
        dataPtr->size_of_additional_info = 255;
    }
    memcpy(dataPtr->additional_info, info,dataPtr->size_of_additional_info);
    
    memcpy(dataPtr->cmd_detail, (void *)cmdDetail, 5 * sizeof(U8));
    Message->oslDataPtr = (oslParaType *)dataPtr;
    Message->oslPeerBuffPtr = NULL;
#ifdef __MMI_MULTI_SIM__
    mmi_sat_dualsim_sendmsg(Message);
    gSATContext.CMDfromSIM = SAT_CMD_NONE;
#else
    SEND_MSG(Message);
#endif   

    gSATContext.lastSATCmd = SAT_NONE_CMD;
}

/*****************************************************************************
 * FUNCTION
 *  SATSendUSSDStage2Rsp
 * DESCRIPTION
 *  Protocol Handler for SEND USSD STAGE 2 RES
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATSendUSSDStage2Rsp(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    FreeSendUSSD();
}

/*****************************************************************************
 * FUNCTION
 *  FreeSendUSSD
 * DESCRIPTION
 *  Frees various elements of Send USSD Structure
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void FreeSendUSSD(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SATClearAllIcons();
    if (gSATSendUSSD.alphaId)
    {
        OslMfree(gSATSendUSSD.alphaId);
        gSATSendUSSD.alphaId = NULL;
    }
}
#if 0			// yuxiang B 07.6.23

/**************************************************************

	FUNCTION NAME		: SATMMIInfoInd

  	PURPOSE				: Protocol Handler for MMI INFO IND

 

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SATMMIInfoInd(void *msgPtr)
{
	mmi_sat_mmi_info_ind_struct *mmiInfoInd;
	U8 *Isd;
	U8 *message;
	U8 *number;
	U8 *displayNumber;
	U16 numberLength=0;
	U16 messageLen;
	U16 finalLen;
	U8 *finalMessage;
	gSATContext.guiBufferPointer=NULL;
	mmiInfoInd=(mmi_sat_mmi_info_ind_struct *) msgPtr;
	if(mmiInfoInd->is_alpha_id_present)
	{
		message=CovertStringForPlatform(mmiInfoInd->alpha_id, mmiInfoInd->no_alpha_id,mmiInfoInd->dcs_of_alpha_id,&messageLen);
	}
	else
	{
		if(!mmiInfoInd->len_of_address) return;
		messageLen=(pfnUnicodeStrlen((PS8)(PU8)GetString(SAT_NUM_CHANGED_BY_SIM_STRING_ID))*ENCODING_LENGTH) + ENCODING_LENGTH;
		message=(PU8)OslMalloc(messageLen);
		memcpy(message,(PU8)GetString(SAT_NUM_CHANGED_BY_SIM_STRING_ID),messageLen);
	}

	number=CovertStringForPlatform(mmiInfoInd->address,(U16)mmiInfoInd->len_of_address,MMI_8BIT_DCS,&numberLength);

	if (mmiInfoInd->ton == 0x91 && mmiInfoInd->len_of_address)
	{
		Isd=CovertStringForPlatform((PU8)"+",1,MMI_8BIT_DCS,NULL);
		displayNumber=OslMalloc(numberLength+ENCODING_LENGTH);
		pfnUnicodeStrcpy((PS8)displayNumber,(PS8)Isd);
		pfnUnicodeStrcat((PS8)displayNumber,(PS8)number);
		finalLen=messageLen + numberLength;
		OslMfree(Isd);
	}
	else
	{
		displayNumber=OslMalloc(numberLength+ENCODING_LENGTH);
		pfnUnicodeStrcpy((PS8)displayNumber,(PS8)number);
		finalLen=messageLen +  numberLength - ENCODING_LENGTH;
	}

	finalMessage=OslMalloc(finalLen);
	pfnUnicodeStrcpy((PS8)finalMessage,(PS8)message);
	pfnUnicodeStrcat((PS8)finalMessage,(PS8)displayNumber);

	OslMfree(displayNumber);
	OslMfree(message);
	NotifyMONumberChange(number,finalMessage);
	OslMfree(number);
	OslMfree(finalMessage);
}


/**************************************************************

	FUNCTION NAME		: SATPlayToneInd

  	PURPOSE				: Protocol Handler for PLAY TONE IND

 

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SATPlayToneInd(void *msgPtr)
{
	mmi_sat_play_tone_ind_struct *playToneInd;
	U8 callState;
	gSATContext.guiBufferPointer=NULL;
	playToneInd=(mmi_sat_play_tone_ind_struct *) msgPtr;
	callState=FALSE;
	FreePlayTone();	
	gSATContext.lastSATCmd = SAT_PLAY_TONE_CMD;	
	gSATPlayTone.sentRsp=0;
	if(GetCurrentState() == CM_ACTIVE_STATE)
	{
		callState=TRUE;
	}
	memcpy(gSATPlayTone.cmdDetail,playToneInd->cmd_detail, 5 * sizeof(U8));

#ifdef MMI_ON_HARDWARE_P
	if(callState)
	{
		switch(playToneInd->tone_id)
		{
			case SAT_TONE_DIAL:
				gSATPlayTone.toneId=TONE_DIAL_CALL_GSM;
			break;
			case SAT_TONE_CALLED_SUBSCRIBER_BUSY:
				gSATPlayTone.toneId=TONE_BUSY_CALL_GSM;
			break;
			case SAT_TONE_CONGESTION:
				gSATPlayTone.toneId=TONE_CONGESTION_GSM;
			break;
			case SAT_TONE_RADIO_PATH_ACK:
				gSATPlayTone.toneId=TONE_RADIO_PATH;
			break;
			case SAT_TONE_RADIO_NO_PATH:
				gSATPlayTone.toneId=TONE_RADIO_NA;
			break;
			case SAT_TONE_ERROR:
				gSATPlayTone.toneId=TONE_ERROR_INFO;
			break;
			case SAT_TONE_CALL_WAITING:
				gSATPlayTone.toneId=TONE_CALL_WAITING;
			break;
			case SAT_TONE_RING:
				gSATPlayTone.toneId=TONE_RINGING_CALL_GSM;
			break;
			case SAT_TONE_GENERAL_BEEP:
				gSATPlayTone.toneId=TONE_KEY_NORMAL;
			break;
			case SAT_TONE_POS_ACK:
				gSATPlayTone.toneId=TONE_POSITIVE_ACK;
			break;
			case SAT_TONE_NEG_ACK:
				gSATPlayTone.toneId=TONE_NEGATIVE_ACK;
			break;
		}
	}
	else
	{
		switch(playToneInd->tone_id)
		{
			case SAT_TONE_DIAL:
				gSATPlayTone.toneId=TONE_DIAL_CALL_GSM_STK;
			break;
			case SAT_TONE_CALLED_SUBSCRIBER_BUSY:
				gSATPlayTone.toneId=TONE_BUSY_CALL_GSM_STK;
			break;
			case SAT_TONE_CONGESTION:
				gSATPlayTone.toneId=TONE_CONGESTION_GSM_STK;
			break;
			case SAT_TONE_RADIO_PATH_ACK:
				gSATPlayTone.toneId=TONE_RADIO_PATH_STK;
			break;
			case SAT_TONE_RADIO_NO_PATH:
				gSATPlayTone.toneId=TONE_RADIO_NA_STK;
			break;
			case SAT_TONE_ERROR:
				gSATPlayTone.toneId=TONE_ERROR_INFO_STK;
			break;
			case SAT_TONE_CALL_WAITING:
				gSATPlayTone.toneId=TONE_CALL_WAITING_STK;
			break;
			case SAT_TONE_RING:
				gSATPlayTone.toneId=TONE_RINGING_CALL_GSM_STK;
			break;
			case SAT_TONE_GENERAL_BEEP:
				gSATPlayTone.toneId=TONE_KEY_NORMAL;
			break;
			case SAT_TONE_POS_ACK:
				gSATPlayTone.toneId=TONE_POSITIVE_ACK_STK;
			break;
			case SAT_TONE_NEG_ACK:
				gSATPlayTone.toneId=TONE_NEGATIVE_ACK_STK;
			break;
		}
	}
#endif

	gSATPlayTone.dcsOfAlphaId=playToneInd->dcs_of_alpha_id;
	gSATPlayTone.noAlphaId=playToneInd->no_alpha_id;

	if(playToneInd->is_alpha_id_present)
	{
		if(gSATPlayTone.noAlphaId)
		{
			gSATPlayTone.alphaId=CovertStringForPlatform(playToneInd->alpha_id, gSATPlayTone.noAlphaId,gSATPlayTone.dcsOfAlphaId,&gSATPlayTone.noAlphaId);
			gSATPlayTone.informUser=1;
		}
		else {
			gSATPlayTone.informUser=0;
		}
	}
	else
	{
		gSATPlayTone.noAlphaId=pfnUnicodeStrlen((PS8)(PU8)GetString(SAT_PLAY_TONE_STRING_ID))*ENCODING_LENGTH;
		gSATPlayTone.alphaId=(PU8)OslMalloc(gSATPlayTone.noAlphaId+ENCODING_LENGTH);
		memcpy(gSATPlayTone.alphaId,(PU8)GetString(SAT_PLAY_TONE_STRING_ID),gSATPlayTone.noAlphaId+ENCODING_LENGTH);
		gSATPlayTone.informUser=1;
	}

#ifdef __SAT_MMI_UNIT_TEST__
	gSATPlayTone.isIconAvailable = 1;
	if (gSATPlayTone.isIconAvailable) {
		gSATPlayTone.iconInfo.iconId= 1;
		gSATPlayTone.iconInfo.iconAttr= SAT_ICON_NOT_SELF_EX;
	}
#else
	gSATPlayTone.isIconAvailable = playToneInd->is_icon_available;
	if (gSATPlayTone.isIconAvailable) {
		gSATPlayTone.iconInfo.iconId = playToneInd->icon_info.icon_id;
		gSATPlayTone.iconInfo.iconAttr = playToneInd->icon_info.icon_attr;
	}
#endif

	if (gSATPlayTone.isIconAvailable && gSATPlayTone.iconInfo.iconAttr == SAT_ICON_SELF_EX) {
		if (gSATPlayTone.alphaId) {
			OslMfree(gSATPlayTone.alphaId);
		}
		gSATPlayTone.alphaId = NULL;
		gSATPlayTone.noAlphaId = 0;
	}

	if(gSATPlayTone.informUser) {
		SATStopWaitTimer();		
		DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);		
		if (GetExitScrnID() == SCR_ID_SAT_HISTORY) {
			ClearExitHandler();
		}
		DisplayPlayToneWindow();
	}
	else
	{
		AudioPlayReq(gSATPlayTone.toneId,DEVICE_AUDIO_PLAY_INFINITE);
	}

//micha1108
	StartTimer(SAT_PLAY_TONE_TIMER,playToneInd->duration,SATPlayToneTimer);
}


/**************************************************************

	FUNCTION NAME		: SATPlayToneTimer

  	PURPOSE				: CallBAck Function called when the timer to Plat the Tone Expires

	INPUT PARAMETERS	: ptr: pointer timerid

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SATPlayToneTimer(void)
{
#ifdef MMI_ON_WIN32
//micha1108
	StopTimer(SAT_PLAY_TONE_TIMER);
#endif
	AudioStopReq(gSATPlayTone.toneId);
	TerminalResponsePlayTone(SAT_CMD_PERFORMED_SUCCESSFULLY,gSATPlayTone.cmdDetail);
	if(gSATPlayTone.informUser) {
		SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
	}
}

/**************************************************************

	FUNCTION NAME		: DisplayPlayToneWindow

  	PURPOSE				: Display Play Tone Window

	INPUT PARAMETERS	: alphaId: pointer to string to be displayed in Play Tone Window

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/


void DisplayPlayToneWindow(void)
{
	U8* iconData = NULL;

	if ( gSATPlayTone.isIconAvailable) {
		SATAddIconIDToList( gSATPlayTone.iconInfo.iconId);
	}

	if (gSATPlayTone.isIconAvailable) {
		if ( !SATCheckAllIconGot()) {
			SATGetIconCompleteCallBack = DisplayPlayToneWindow;
			SATGetIconDataToList();
			return;
		}
	}

	iconData = SATGetIconFromList(gSATPlayTone.iconInfo.iconId);

	EntryNewScreen(SCR_ID_SAT_PLAYTONE, ExitPlayTone, NULL, NULL);

	SetIsInSATScreen();
	gSATContext.historyFuncPtr=DisplayPlayToneWindow;
	AudioPlayReq(gSATPlayTone.toneId,DEVICE_AUDIO_PLAY_INFINITE);
	ClearHighlightHandler();
//micha0726, add abort for test case	
	ShowCategory151Screen(0,0,0,0,STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							gSATPlayTone.alphaId, iconData, gSATContext.guiBufferPointer);

//	SetLeftSoftkeyFunction(TerminalResponsePlayToneOK,KEY_EVENT_UP);
	SetRightSoftkeyFunction(TerminalResponsePlayToneBackward,KEY_EVENT_UP);
	SetKeyHandler(TerminalResponsePlayToneExpire,KEY_END,KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: ExitPlayTone

  	PURPOSE				: Exit Handler of Play Tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void ExitPlayTone(void)
{
	if(!gSATPlayTone.sentRsp)
	{
		AudioStopReq(gSATPlayTone.toneId);
		SATSaveHistory();
	}
}


/**************************************************************

	FUNCTION NAME		: TerminalResponsePlayToneBackward

  	PURPOSE				: Key Handler of RSK of Play Tone Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void TerminalResponsePlayToneBackward(void)
{
	AudioStopReq(gSATPlayTone.toneId);
//micha1108
	StopTimer(SAT_PLAY_TONE_TIMER);
	TerminalResponsePlayTone(SAT_PROACTIVE_CMD_TERMINATED_BY_USER,gSATPlayTone.cmdDetail);
	SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
}

/**************************************************************

	FUNCTION NAME		: TerminalResponsePlayToneOK

  	PURPOSE				: Key Handler of RSK of Play Tone Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void TerminalResponsePlayToneOK(void)
{
	AudioStopReq(gSATPlayTone.toneId);
//micha1108
	StopTimer(SAT_PLAY_TONE_TIMER);
	TerminalResponsePlayTone(SAT_CMD_PERFORMED_SUCCESSFULLY,gSATPlayTone.cmdDetail);
	SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
}

/**************************************************************

	FUNCTION NAME		: TerminalResponsePlayToneExpire

  	PURPOSE				: Key Handler of End Key of Play Tone Screen

	INPUT PARAMETERS	: alphaId: pointer to string to be displayed in Play Tone Window

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void TerminalResponsePlayToneExpire(void)
{
	AudioStopReq(gSATPlayTone.toneId);
//micha1108
	StopTimer(SAT_PLAY_TONE_TIMER);
	TerminalResponsePlayTone(SAT_PROACTIVE_CMD_TERMINATED_BY_USER,gSATPlayTone.cmdDetail);
	ExitSATToIdle();
}

/**************************************************************

	FUNCTION NAME		: TerminalResponsePlayTone

 

	INPUT PARAMETERS	: Res: response type
							cmdDetail: cmd detail to be sent

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void TerminalResponsePlayTone(U8 res, U8 *cmdDetail)
{
	MYQUEUE *Message;
	mmi_sat_play_tone_res_req_struct    *dataPtr;
	gSATPlayTone.sentRsp=1;
	Message=ALLOCATE_ITM(MOD_MMI);
	Message->oslSrcId = MOD_MMI;
	Message->oslDestId = MOD_L4C;
	Message->oslMsgId =  PRT_MMI_SAT_PLAY_TONE_RES_REQ;
	dataPtr =  (mmi_sat_play_tone_res_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_play_tone_res_req_struct));
	
	 
	if(res==SAT_CMD_PERFORMED_SUCCESSFULLY && gSATPlayTone.isIconAvailable==1 && gSATContext.isCMDIconSupported==MMI_FALSE)
	{
		dataPtr->res=SAT_CMD_PERFORMED_BUT_ICON_COULD_NOT_DSPL;
	}
	else
	{
	 
		dataPtr->res=res;
	}

	memcpy(dataPtr->cmd_detail, (void *)cmdDetail, 5*sizeof(U8));
	Message->oslDataPtr = (oslParaType *)dataPtr;
	Message->oslPeerBuffPtr = NULL;
	SEND_MSG(Message);                     
	gSATContext.lastSATCmd = SAT_NONE_CMD;
}

/**************************************************************

	FUNCTION NAME		: FreePlayTone

  	PURPOSE				: Frees various elements of Play Tone structure

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void FreePlayTone(void)
{
	SATClearAllIcons();
//micha1008	
	if(gSATPlayTone.alphaId)
	{
		OslMfree(gSATPlayTone.alphaId);
	}
		gSATPlayTone.noAlphaId=0;
		gSATPlayTone.alphaId=NULL;
}

/**************************************************************

	FUNCTION NAME		: SATPlayToneRsp

  	PURPOSE				: Protocol Handler for PLAY TONE RES

 

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SATPlayToneRsp(void *msgPtr)
{
	FreePlayTone();
}
#endif 		//yuxiang E 07.6.23
#if 1 		//yuxiang 07.6.23
/**************************************************************

	FUNCTION NAME		: SendImageLenRecordReq

 

	INPUT PARAMETERS	: iconId: icon data to be read

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SendImageLenRecordReq(U8 iconId)
{
	gSATContext.getImageStage=SAT_IMAGE_READLEN_CMD;
	gSATImageRecord.iconId=iconId;
	SATSendReadSimFileReq(SAT_FILE_IMG_IDX, iconId, 1);
}

/**************************************************************

	FUNCTION NAME		: SendImageInstRecordReq

  	PURPOSE				: Reads Image Instance from EFIMG

	INPUT PARAMETERS	: satReadSimRes: pointer to SATREADSIMFILERES structure
 

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SendImageInstRecordReq(mmi_smu_read_sim_rsp_struct *satReadSimRes)
{
	if(satReadSimRes->result.flag==L4C_OK)
	{
		if(!satReadSimRes->length)
			gSATImageRecord.noOfImages=0;
		else
			gSATImageRecord.noOfImages=satReadSimRes->data[0];
		if(gSATImageRecord.noOfImages)
		{
			gSATContext.getImageStage=SAT_IMAGE_READINST_CMD;
			SATSendReadSimFileReq(SAT_FILE_IMG_IDX, gSATImageRecord.iconId, 10);
		}
		else
		{
			gSATContext.getImageStage=SAT_IMAGE_IDLE_CMD;
			SATGetIconDataCallBack();
		}
	}
	else
	{
//micha1021
		gSATImageRecord.noOfImages=0;
		gSATContext.getImageStage=SAT_IMAGE_IDLE_CMD;
		SATGetIconDataCallBack();
	}
}

/**************************************************************

	FUNCTION NAME		: SendImageDataRecordReq

  	PURPOSE				: Reads Image Data from Actual Image File

	INPUT PARAMETERS	: satReadSimRes: pointer to SATREADSIMFILERES structure
 

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SendImageDataRecordReq(mmi_smu_read_sim_rsp_struct *satReadSimRes)
{
	if(satReadSimRes->result.flag==L4C_OK)
	{
		if(!satReadSimRes->length)
			gSATImageRecord.noOfImages=0;
		else
			gSATImageRecord.noOfImages=satReadSimRes->data[0];
		
		if(gSATImageRecord.noOfImages)
		{
			gSATImageRecord.imageInst=(SATIMAGEINSTANCE *)OslMalloc(sizeof(SATIMAGEINSTANCE));
			gSATImageRecord.noOfImages=1;			//currently get only the first image
			gSATImageRecord.imageInst->width=satReadSimRes->data[1];
			gSATImageRecord.imageInst->height=satReadSimRes->data[2];
			gSATImageRecord.imageInst->coding=satReadSimRes->data[3];

			gSATImageRecord.imageInst->instance= satReadSimRes->data[4];
			gSATImageRecord.imageInst->instance=(gSATImageRecord.imageInst->instance << 8) + satReadSimRes->data[5];

			gSATImageRecord.imageInst->offset=satReadSimRes->data[6];
			gSATImageRecord.imageInst->offset=(gSATImageRecord.imageInst->offset << 8) + satReadSimRes->data[7];

			gSATImageRecord.imageInst->length=satReadSimRes->data[8];
			gSATImageRecord.imageInst->length=(gSATImageRecord.imageInst->length << 8)+ satReadSimRes->data[9];

			if ((gSATImageRecord.imageInst->length % 256) == 0 &&  (gSATImageRecord.imageInst->length/256) > 0)
				gSATImageRecord.imageInst->cycle	= gSATImageRecord.imageInst->length/256 - 1;
			else
				gSATImageRecord.imageInst->cycle	= gSATImageRecord.imageInst->length/256;
			
			if(gSATImageRecord.imageInst->cycle==0)
				gSATImageRecord.imageInst->reqlength=gSATImageRecord.imageInst->length;
			else
				gSATImageRecord.imageInst->reqlength=256;

			gSATImageRecord.imageInst->curcycle=0;

			gSATContext.getImageStage=SAT_IMAGE_READDATA_CMD;

#ifdef __SAT_MMI_UNIT_TEST__
			SendImageDataCallBack(&EFback2);
#else
			SATSendReadSimFileReq((U16) (FILE_IMG_1_IDX + (gSATImageRecord.imageInst->instance - 0x4F01)), gSATImageRecord.imageInst->offset, gSATImageRecord.imageInst->reqlength);
#endif
		}
		else
		{
			gSATContext.getImageStage=SAT_IMAGE_IDLE_CMD;
			SATGetIconDataCallBack();
		}
	}
	else
	{
//micha1021
		gSATImageRecord.noOfImages=0;
		gSATContext.getImageStage=SAT_IMAGE_IDLE_CMD;
		SATGetIconDataCallBack();
	}
}

/**************************************************************

	FUNCTION NAME		: SendImageDataCallBack

  	PURPOSE				: CallBack function called when the Image Data has been read &
							sends request for color data

	INPUT PARAMETERS	: satReadSimRes: pointer to SATREADSIMFILERES structure
 

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SendImageDataCallBack(mmi_smu_read_sim_rsp_struct *satReadSimRes)
{
	if(satReadSimRes->result.flag==L4C_OK)
	{
		if(gSATImageRecord.imageInst->cycle == gSATImageRecord.imageInst->curcycle)
		{
			gSATImageRecord.imageData=OslMalloc(satReadSimRes->length);
			memcpy(gSATImageRecord.imageData+((gSATImageRecord.imageInst->curcycle)*256),satReadSimRes->data,satReadSimRes->length);
			if(gSATImageRecord.imageInst->coding != SAT_BASIC_IMAGE)

			{
				gSATImageRecord.noOfCLUT = gSATImageRecord.imageData[3];
				gSATImageRecord.locOfCLUT = gSATImageRecord.imageData[4];
				gSATImageRecord.locOfCLUT = (gSATImageRecord.locOfCLUT << 8) + gSATImageRecord.imageData[5];
				gSATImageRecord.CLUTData= (SATCOLORDATA *) (gSATImageRecord.imageData + gSATImageRecord.locOfCLUT);
			}

			gSATContext.getImageStage=SAT_IMAGE_IDLE_CMD;
			SATGetIconDataCallBack();
		}
		else
		{
			memcpy(gSATImageRecord.imageData+((gSATImageRecord.imageInst->curcycle)*256),satReadSimRes->data,satReadSimRes->length);
			gSATImageRecord.imageInst->reqlength=((gSATImageRecord.imageInst->length)-(256 * gSATImageRecord.imageInst->curcycle + 1)) % 256;
			gSATImageRecord.imageInst->curcycle = gSATImageRecord.imageInst->curcycle + 1;

			gSATContext.getImageStage=SAT_IMAGE_READDATA_CMD;
			SATSendReadSimFileReq( (U16) (FILE_IMG_1_IDX + (gSATImageRecord.imageInst->instance - 0x4F01)), 
									(U16) (gSATImageRecord.imageInst->offset + (gSATImageRecord.imageInst->curcycle)*256),
									gSATImageRecord.imageInst->reqlength);
		}
	}
	else
	{
//micha1021
		gSATImageRecord.noOfImages=0;
		gSATContext.getImageStage=SAT_IMAGE_IDLE_CMD;
		SATGetIconDataCallBack();
	}
}


/*****************************************************************************
 * FUNCTION
 *  SATSendReadSimInfoReq
 * DESCRIPTION
 *  Send READ SIM INFO REQ
 * PARAMETERS
 *  fileIdx     [IN]    File Id to be read
 * RETURNS
 *  void
 *****************************************************************************/
void SATSendReadSimFileReq(U16 fileIdx, U16 recordNo, U16 length)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_smu_read_sim_req_struct *dataPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Message=ALLOCATE_ITM(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
#ifdef __MMI_MULTI_SIM__
    Message->oslDestId = MOD_L4C + gSATContext.CMDfromSIM - SAT_CMD_SIM1;
#else
    Message->oslDestId = MOD_L4C;
#endif
    Message->oslMsgId =  PRT_MMI_SMU_READ_SIM_REQ;
    dataPtr =  (mmi_smu_read_sim_req_struct *) OslConstructDataPtr(sizeof(mmi_smu_read_sim_req_struct));
    dataPtr->file_idx=(U8)fileIdx;
    dataPtr->para=(U8)recordNo;
    dataPtr->length=length;
    Message->oslDataPtr = (oslParaType *)dataPtr;
    Message->oslPeerBuffPtr = NULL;
#ifdef __MMI_MULTI_SIM__
    mmi_sat_dualsim_sendmsg(Message);
#else
    SEND_MSG(Message);
#endif      
}

/**************************************************************

	FUNCTION NAME		: SATSendReadSimFileRsp

  	PURPOSE				: Protocol Handler for SEND SIM RES

 

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SATSendReadSimFileRsp(void *msgPtr)
{
	mmi_smu_read_sim_rsp_struct *satReadSimRes;
	satReadSimRes=(mmi_smu_read_sim_rsp_struct *)msgPtr;
	switch(gSATContext.getImageStage)
	{
		case SAT_IMAGE_READLEN_CMD:
			SendImageInstRecordReq(satReadSimRes);
		break;
		case SAT_IMAGE_READINST_CMD:
			SendImageDataRecordReq(satReadSimRes);
		break;
		case SAT_IMAGE_READDATA_CMD:
			SendImageDataCallBack(satReadSimRes);
		break;
	}
}
#endif 		//yuxiang 07.6.23
#if 0		//yuxiang B 07.6.23
/**************************************************************

	FUNCTION NAME		: SATSendSSUSSDError

  	PURPOSE				: Sends USSD Error

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/


void SATSendSSUSSDError(void)
{
	switch (gSATContext.lastSATCmd) {
		case SAT_SEND_SS_CMD:
			TerminalResponseSendSS(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0,NULL,gSATSendSS.cmdDetail);
			if(gSATSendSS.informUser) {
				SATPleaseWaitScreen(SAT_SS_ERROR_STRING_ID, 0);
			}
			break;
		case SAT_SEND_USSD_CMD:
			TerminalResponseSendUSSD(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0,NULL,gSATSendUSSD.cmdDetail);
			if(gSATSendUSSD.informUser) {
				SATPleaseWaitScreen(SAT_USSD_ERROR_STRING_ID, 0);
			}
		break;
	}
}


/**************************************************************

	FUNCTION NAME		: SATEVDLIdleScreenAvailable

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SATEVDLIdleScreenAvaliable(void)
{
	MYQUEUE *Message;
	Message=ALLOCATE_ITM(MOD_MMI);
	Message->oslSrcId = MOD_MMI;
	Message->oslDestId = MOD_L4C;
	Message->oslMsgId =  PRT_MMI_SAT_EVDL_IDLE_SCREEN_AVAILABLE_REQ;
	Message->oslDataPtr = NULL;
	Message->oslPeerBuffPtr = NULL;
	SEND_MSG(Message);

	 
	g_pSATMainMemu->selectedMenuItem = 0;
}

/**************************************************************

	FUNCTION NAME		: SATEVDLLangSel

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SATEVDLLangSel(U8 *lang)
{
	MYQUEUE *Message;
	mmi_sat_language_selection_req_struct   *dataPtr;
	Message=ALLOCATE_ITM(MOD_MMI);
	Message->oslSrcId = MOD_MMI;
	Message->oslDestId = MOD_L4C;
	Message->oslMsgId = PRT_MMI_SAT_LANGUAGE_SELECTION_REQ;
	dataPtr =  (mmi_sat_language_selection_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_language_selection_req_struct));
	memcpy(dataPtr->iso639_language, (void *)lang, 2*sizeof(U8));
	Message->oslDataPtr = (oslParaType *)dataPtr;
	Message->oslPeerBuffPtr = NULL;
	SEND_MSG(Message);
}

/**************************************************************

	FUNCTION NAME		: SATEVDLBrowserTerminationInd

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SATEVDLBrowserTerminationInd(void * msgPtr)
{
	mmi_sat_evdl_browser_termination_rsp_struct *BrowserTerminationInd;
	BrowserTerminationInd=(mmi_sat_evdl_browser_termination_rsp_struct *) msgPtr;
	SATEVDLBrowserTermination((U8)BrowserTerminationInd->result);
}


/**************************************************************

	FUNCTION NAME		: SATEVDLBrowserTermination

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SATEVDLBrowserTermination(U8 res)
{
	MYQUEUE *Message;
	mmi_sat_evdl_browser_termination_req_struct   *dataPtr;
	Message=ALLOCATE_ITM(MOD_MMI);
	Message->oslSrcId = MOD_MMI;
	Message->oslDestId = MOD_L4C;
	Message->oslMsgId = PRT_MMI_SAT_EVDL_BROWSER_TERMINATION_REQ;
	dataPtr =  (mmi_sat_evdl_browser_termination_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_evdl_browser_termination_req_struct));
	dataPtr->browser_termination_cause = res;
	Message->oslDataPtr = (oslParaType *)dataPtr;
	Message->oslPeerBuffPtr = NULL;
	SEND_MSG(Message);
}

/**************************************************************

	FUNCTION NAME		: SATLangNotifyInd

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SATLangNotifyInd(void * msgPtr)
{
	mmi_sat_lang_notify_ind_struct *LangNotifyInd;
	LangNotifyInd=(mmi_sat_lang_notify_ind_struct *) msgPtr;
	if (LangNotifyInd->is_language_specified == MMI_TRUE ) {
		SATSetLanguage(LangNotifyInd->iso639_language);
	}
	TerminalResponseLangNotify(SAT_CMD_PERFORMED_SUCCESSFULLY,LangNotifyInd->cmd_detail);
}

/**************************************************************

	FUNCTION NAME		: SATSetLanguage

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SATSetLanguage(PU8 Lang)
{
	S16 error;
	U8 lang_sel = 0xff;
	if(strncmp((PS8) Lang, SAT_LANG_ENGLISH, 2) == 0 && gCurrLangIndex != 0) {
		lang_sel = 0;
	   	SetCurrentLanguage(lang_sel);
		WriteValue( NVRAM_SETTING_LANG, &lang_sel, DS_BYTE , &error);
		RefreshNwProviderName(gPLMN);
		DisplayPopup((U8*)GetString(SSC_SCR_LANGUAGE_ENGLISH_STR_ID),IMG_GLOBAL_ACTIVATED,1,SSC_POP_UP_TIME,(U8)SUCCESS_TONE);
	}
	else if (strncmp((PS8) Lang, SAT_LANG_CHINESE, 2) == 0 && gCurrLangIndex == 0) {
		lang_sel = 1;
	   	SetCurrentLanguage(lang_sel);
		WriteValue( NVRAM_SETTING_LANG, &lang_sel, DS_BYTE , &error);
		RefreshNwProviderName(gPLMN);
		DisplayPopup((U8*)GetString(SSC_SCR_LANGUAGE_TCHINESE_STR_ID),IMG_GLOBAL_ACTIVATED,1,SSC_POP_UP_TIME,(U8)SUCCESS_TONE);
	}
}


/**************************************************************

	FUNCTION NAME		: TerminalResponseLangNotify

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void TerminalResponseLangNotify(U8 res, U8 *cmdDetail)
{
	MYQUEUE *Message;
	mmi_sat_lang_notify_res_req_struct *dataPtr;
	Message=ALLOCATE_ITM(MOD_MMI);
	Message->oslSrcId = MOD_MMI;
	Message->oslDestId = MOD_L4C;
	Message->oslMsgId =  PRT_MMI_SAT_LANG_NOTIFY_RES_REQ;
	dataPtr =  (mmi_sat_lang_notify_res_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_lang_notify_res_req_struct));
	dataPtr->res=res;
	memcpy(dataPtr->cmd_detail, (void *)cmdDetail, 5*sizeof(U8));
	Message->oslDataPtr = (oslParaType *)dataPtr;
	Message->oslPeerBuffPtr = NULL;
	SEND_MSG(Message);
}
#endif

#ifdef __MMI_MULTI_SIM__
/*****************************************************************************
 * FUNCTION
 *  TerminalResponseLaunchBrowserEx
 * DESCRIPTION
 *  
 * PARAMETERS
 
 *  res         [IN]    Response type
 *  add_info    [IN]    Additional info
 *  cmdDetail   [IN]    Cmd detail to be sent      
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseLaunchBrowserEx(U8 destId, U8 res, U8 add_info, U8 *cmdDetail)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_launch_browser_res_req_struct *dataPtr;
    COS_EVENT ev ;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Message = allocate_ilm(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
    Message->oslDestId = destId;
    Message->oslMsgId = PRT_MMI_SAT_LAUNCH_BROWSER_RES_REQ;
    dataPtr =
        (mmi_sat_launch_browser_res_req_struct*) OslConstructDataPtr(sizeof(mmi_sat_launch_browser_res_req_struct));
    dataPtr->res = res;
    dataPtr->size_of_additional_info = add_info;
    memcpy(dataPtr->cmd_detail, (void*)cmdDetail, 5 * sizeof(U8));
    Message->oslDataPtr = (oslParaType*) dataPtr;
    Message->oslPeerBuffPtr = NULL;
#ifdef MMI_ON_HARDWARE_P
    Message->sap_id = INVALID_SAP;
    #endif
    ev.nEventId = Message->oslMsgId; 
    ev.nParam1 = (UINT32)Message; 
    ev.nParam3 = Message->oslDestId; 
    SEND_COSMSG(Message->oslDestId, &ev); 
}
#endif /*__MMI_MULTI_SIM__*/

/*****************************************************************************
 * FUNCTION
 *  SATLaunchBrowserInd
 * DESCRIPTION
 *  
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
 #ifdef JATAAYU_SUPPORT
extern BOOL isInWapApp(void);
#endif
void SATLaunchBrowserInd(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mmi_sat_launch_browser_ind_struct *LaunchBrowserInd;
    U8 nSimID;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    nSimID = MTPNP_AD_get_protocol_event_src_mode();

#ifdef __MMI_MULTI_SIM__
    if (gSATContext.CMDfromSIM != (nSimID+SAT_CMD_SIM1) && gSATContext.CMDfromSIM != SAT_CMD_NONE)
    {
        TerminalResponseLaunchBrowserEx(MOD_L4C + nSimID, SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 1, LaunchBrowserInd->cmd_detail);
        return;
    }
    else
    {
        gSATContext.CMDfromSIM = SAT_CMD_SIM1 + nSimID;
    }
#endif
    LaunchBrowserInd = (mmi_sat_launch_browser_ind_struct*) msgPtr;
    memcpy(gSATLaunchBrowser.cmdDetail, LaunchBrowserInd->cmd_detail, 5 * sizeof(U8));
#ifdef JATAAYU_SUPPORT
#ifdef JATAAYU_SUPPORT_WAP

    FreeLaunchBrowser();

    gSATContext.lastSATCmd = SAT_LAUNCH_BROWSER_CMD;
    gSATContext.guiBufferPointer = NULL;

    if (LaunchBrowserInd->browser_mode == SAT_LAUNCH_BROWSER_IF_NOT_ALREADY_LAUNCHED && isInWapApp())
    {
        TerminalResponseLaunchBrowser(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, SAT_LAUNCH_BROWSER_BROWSER_UNAVALIABLE, LaunchBrowserInd->cmd_detail);
        return;
    }

    gSATLaunchBrowser.browserMode = LaunchBrowserInd->browser_mode;
    gSATLaunchBrowser.browserIdentity = LaunchBrowserInd->browser_identity;
    memcpy(gSATLaunchBrowser.provisionFileId, LaunchBrowserInd->provision_file_id, 10 * sizeof(U8));

    gSATLaunchBrowser.noAlphaId = LaunchBrowserInd->no_alpha_id;
    gSATLaunchBrowser.dcsOfAlphaId = LaunchBrowserInd->dcs_of_alpha_id;

    gSATLaunchBrowser.noUrl = LaunchBrowserInd->no_url;
    gSATLaunchBrowser.noBearer = LaunchBrowserInd->no_bearer;
    gSATLaunchBrowser.noGateway = LaunchBrowserInd->no_gateway;
    gSATLaunchBrowser.dcsOfGateway = LaunchBrowserInd->dcs_of_gateway;

    gSATLaunchBrowser.isAlphaIdPresent = LaunchBrowserInd->is_alpha_id_present;
    if (gSATLaunchBrowser.isAlphaIdPresent && gSATLaunchBrowser.noAlphaId)
    {
        gSATLaunchBrowser.alphaId = CovertStringForPlatform(
                                        LaunchBrowserInd->alpha_id,
                                        gSATLaunchBrowser.noAlphaId,
                                        gSATLaunchBrowser.dcsOfAlphaId,
                                        &gSATLaunchBrowser.noAlphaId);
    }
    else
    {
         
        gSATLaunchBrowser.noAlphaId = mmi_ucs2strlen((PS8) (PU8) GetString(SAT_LAUNCH_BROWSER_CONF_STRING_ID)) * ENCODING_LENGTH;
        gSATLaunchBrowser.alphaId = (PU8) OslMalloc(gSATLaunchBrowser.noAlphaId + ENCODING_LENGTH);
        memcpy(
            gSATLaunchBrowser.alphaId,
            (PU8) GetString(SAT_LAUNCH_BROWSER_CONF_STRING_ID),
            gSATLaunchBrowser.noAlphaId + ENCODING_LENGTH);
    }

    gSATLaunchBrowser.isIconAvailable = LaunchBrowserInd->is_icon_available;
    if (gSATLaunchBrowser.isIconAvailable)
    {
        gSATLaunchBrowser.iconInfo.iconId = LaunchBrowserInd->icon_info.icon_id;
        gSATLaunchBrowser.iconInfo.iconAttr = LaunchBrowserInd->icon_info.icon_attr;
    }

    if (gSATLaunchBrowser.noUrl)
    {
        gSATLaunchBrowser.url = CovertStringForPlatform(
                                    LaunchBrowserInd->url,
                                    gSATLaunchBrowser.noUrl,
                                    MMI_DEFAULT_DCS,
                                    &gSATLaunchBrowser.noUrl);
    }

    if (gSATLaunchBrowser.noBearer)
    {
        gSATLaunchBrowser.bearer = (U8*) OslMalloc(LaunchBrowserInd->no_bearer);
        memcpy(gSATLaunchBrowser.bearer, LaunchBrowserInd->bearer, LaunchBrowserInd->no_bearer);
    }

    if (gSATLaunchBrowser.noGateway)
    {
        gSATLaunchBrowser.gateway = CovertStringForPlatform(
                                        LaunchBrowserInd->gateway,
                                        gSATLaunchBrowser.noGateway,
                                        gSATLaunchBrowser.dcsOfGateway,
                                        &gSATLaunchBrowser.noGateway);
    }

    SATStopWaitTimer();
    DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);
    if (GetExitScrnID() == SCR_ID_SAT_HISTORY)
    {
        ClearExitHandler();
    }
    GoToLaunchBrowserConformationScreen();
#endif	
#else
    /* ME cannot support WAP */
    TerminalResponseLaunchBrowser(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, SAT_LAUNCH_BROWSER_BEARER_UNAVALIABLE, gSATLaunchBrowser.cmdDetail);
#endif
}

#ifdef JATAAYU_SUPPORT

/*****************************************************************************
 * FUNCTION
 *  GoToLaunchBrowserConformationScreen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void GoToLaunchBrowserConformationScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *iconData = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gSATLaunchBrowser.isIconAvailable)
    {
        SATAddIconIDToList(gSATLaunchBrowser.iconInfo.iconId);
    }

    if (gSATLaunchBrowser.isIconAvailable)
    {
        if (!SATCheckAllIconGot())
        {
            SATGetIconCompleteCallBack = GoToLaunchBrowserConformationScreen;
            SATGetIconDataToList();
            return;
        }
    }

    if (gSATLaunchBrowser.isIconAvailable && gSATLaunchBrowser.iconInfo.iconAttr == SAT_ICON_SELF_EX &&
        SATGetIconFromList(gSATLaunchBrowser.iconInfo.iconId) != NULL)
    {
        if (gSATLaunchBrowser.alphaId)
        {
            OslMfree(gSATLaunchBrowser.alphaId);
            gSATLaunchBrowser.alphaId = NULL;
        }
    }

    iconData = SATGetIconFromList(gSATLaunchBrowser.iconInfo.iconId);
    gSATContext.exitCondition = &gSATLaunchBrowser.passedConf;
    EntryNewScreen(SCR_ID_SAT_LAUNCH_BROWSER_CONF, SATGenericExitHandler, NULL, NULL);

    SetIsInSATScreen();
    gSATContext.historyFuncPtr = GoToLaunchBrowserConformationScreen;
    gSATLaunchBrowser.passedConf = 0;
    ShowCategory151Screen(
        0,
        0,
        STR_GLOBAL_YES,
        0,
        STR_GLOBAL_NO,
        0,
        gSATLaunchBrowser.alphaId,
        iconData,
        gSATContext.guiBufferPointer);

    SetKeyHandler(SATSendLaunchBrowserTerminalResponseExpire, KEY_END, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction(LaunchBrowserConformationOK, KEY_EVENT_UP);
    SetRightSoftkeyFunction(SATSendLaunchBrowserTerminalResponseUnable, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  LaunchBrowserConformationOK
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 extern JWAP_APP_INFO *g_pstJWapAppInfo ;
extern void jWap_SetWapAppStatus(JC_BOOLEAN bIsActive);
extern void jWap_new_mainMenu_entry_handler ();
#ifdef JWAP_PUSH_ENABLE
extern void jWap_push_entry_handler (void);
#endif //JWAP_PUSH_ENABLE

 INT jWap_SAT_entry_handler (void)
{
	INT iErr ;

	mmi_trace (1, "JDD_LOG: jWap_mainMenu_entry_handler with Launch Mode [%d]",
		g_pstJWapAppInfo->eWapLaunchMode) ;
	mmi_show_in_progress(STR_GLOBAL_PLEASE_WAIT, IMG_GLOBAL_PROGRESS);
	jWap_SetWapAppStatus(E_TRUE); 
	jc_memset (jWap_getInputBuff(), 0, jWAP_MAX_INPUT_SIZE) ;
	jWap_initAppSettingsInfo () ;
	iErr = jWap_init_engine() ; 
	if (!iErr)
	{
		switch (g_pstJWapAppInfo->eWapLaunchMode)
		{
			case E_WAP_LAUNCH_NORMAL:
				jWap_new_mainMenu_entry_handler () ;
				break ;

			case E_WAP_LAUNCH_URL:
				if (g_pstJWapAppInfo->pmPushURL)
				{
					iErr = jWap_FetchPushUrl () ;
					if (JC_OK != iErr)
					{
						mmi_trace (1, "JDD_LOG: jWap_mainMenu_entry_handler >> jWap_FetchPushUrl failed (%X)", -iErr) ;
						jdd_MemFree (g_pstJWapAppInfo->pmPushURL) ;
						g_pstJWapAppInfo->pmPushURL = JC_NULL ;
						#ifdef __MMI_MULTI_SIM__
						g_pstJWapAppInfo->simid = g_pstJWapAppInfo->original_simid ;
						#endif
					}
				}
				break ;
#ifdef JWAP_PUSH_ENABLE 

			case E_WAP_LAUNCH_PUSH_ITEM:
				//ranjeet: code is commented since this api will launch latest push message
				//jWap_ShowLatestItemDetails () ;
				//use below api to lauch push
				jWap_push_entry_handler () ;
				break ;

			case E_WAP_LAUNCH_PUSH_INBOX:
				break;
#endif //JWAP_PUSH_ENABLE				
			default:
				break ;
		}
	}
	mmi_trace (1, "JDD_LOG: Browser Init Status: %X", JRET(iErr)) ;
    return iErr;
}


void LaunchBrowserConformationOK(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    INT iErr ;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gSATLaunchBrowser.passedConf = 1;

    g_pstJWapAppInfo->eWapLaunchMode = E_WAP_LAUNCH_URL;
    g_pstJWapAppInfo->pmPushURL = jdi_CUtilsTcsDuplicate(JC_NULL,(const JC_CHAR *)gSATLaunchBrowser.url) ;

#ifdef __MMI_MULTI_SIM__
	g_pstJWapAppInfo->simid = gSATContext.CMDfromSIM - 1;
#endif
    iErr = jWap_SAT_entry_handler();
    if(iErr)
    {
        SATSendLaunchBrowserTerminalResponseExpire();
    }
}

void ClearWapSrc(void)
{
	U16 screen_id = SCR_ID_WAP_MAIN_SCREEN;

	for(;screen_id<= SCR_ID_WAP_END;screen_id++)
	{
	    DeleteScreenIfPresent(screen_id);
	}
}

/*****************************************************************************
 * FUNCTION
 *  WAPLaunchBrowserRsp
 * DESCRIPTION
 *  
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void WAPLaunchBrowserRsp(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wap_mmi_lauch_browser_rsp_struct *wapLaunchBrowserRes;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_SAT, "func :%s gSATContext.isInSATMenu = %d", __FUNCTION__, gSATContext.isInSATMenu);
    if(msgPtr == NULL && gSATContext.isInSATMenu)
    {
        ClearWapSrc();
        
        #if (defined __MMI_DUAL_SIM_SINGLE_CALL__) || (defined __MMI_DUAL_SIM_MASTER__)
            if (gSATContext.satIndex == SAT_CMD_SIM2)
            {
                gSATContext.historyFuncPtr = GoToSATMainMenu_2;
            }
            else
            {
                gSATContext.historyFuncPtr = GoToSATMainMenu;
            }
        #else /* (defined __MMI_DUAL_SIM_SINGLE_CALL__) || (defined __MMI_DUAL_SIM_MASTER__) */
            gSATContext.historyFuncPtr = GoToSATMainMenu;
        #endif /* (defined __MMI_DUAL_SIM_SINGLE_CALL__) || (defined __MMI_DUAL_SIM_MASTER__) */
            GenericExitScreen(SCR_ID_SAT_HISTORY, SATHistoryHandler); /* there should be a WAP browser on top */
    
        TerminalResponseLaunchBrowser(
            SAT_CMD_PERFORMED_SUCCESSFULLY,
            0,
            gSATLaunchBrowser.cmdDetail);
        SATPleaseWaitScreen(STR_GLOBAL_PLEASE_WAIT, SAT_WAIT_IMAGE_ID);
        return;
    }
    else if(msgPtr == NULL)
    {
        return;
    }
    
    wapLaunchBrowserRes = (wap_mmi_lauch_browser_rsp_struct*) msgPtr;

    if (wapLaunchBrowserRes->res != SAT_CMD_PERFORMED_SUCCESSFULLY)
    {
        SetIsInSATScreen();
    }
    else
    {
    #if (defined __MMI_DUAL_SIM_SINGLE_CALL__) || (defined __MMI_DUAL_SIM_MASTER__)
        if (gSATContext.satIndex == SAT_CMD_SIM2)
        {
            gSATContext.historyFuncPtr = GoToSATMainMenu_2;
        }
        else
        {
            gSATContext.historyFuncPtr = GoToSATMainMenu;
        }
    #else /* (defined __MMI_DUAL_SIM_SINGLE_CALL__) || (defined __MMI_DUAL_SIM_MASTER__) */
        gSATContext.historyFuncPtr = GoToSATMainMenu;
    #endif /* (defined __MMI_DUAL_SIM_SINGLE_CALL__) || (defined __MMI_DUAL_SIM_MASTER__) */
        GenericExitScreen(SCR_ID_SAT_HISTORY, SATHistoryHandler); /* there should be a WAP browser on top */
    }

    TerminalResponseLaunchBrowser(
        wapLaunchBrowserRes->res,
        wapLaunchBrowserRes->additionalInfo,
        gSATLaunchBrowser.cmdDetail);
    SATPleaseWaitScreen(STR_GLOBAL_PLEASE_WAIT, SAT_WAIT_IMAGE_ID);
}

/*****************************************************************************
 * FUNCTION
 *  SATSendLaunchBrowserTerminalResponseExpire
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SATSendLaunchBrowserTerminalResponseExpire(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TerminalResponseLaunchBrowser(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, gSATLaunchBrowser.cmdDetail);
    ExitSATToIdle();
}


/*****************************************************************************
 * FUNCTION
 *  SATSendLaunchBrowserTerminalResponseUnable
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SATSendLaunchBrowserTerminalResponseUnable(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TerminalResponseLaunchBrowser(SAT_USER_DID_NOT_ACCEPT_THE_PROACTIVE_CMD, 0, gSATLaunchBrowser.cmdDetail);
    SATPleaseWaitScreen(STR_GLOBAL_PLEASE_WAIT, SAT_WAIT_IMAGE_ID);
}
#endif

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseLaunchBrowser
 * DESCRIPTION
 *  
 * PARAMETERS
 *  res         [IN]    Response type
 *  add_info    [IN]    Additional info
 *  cmdDetail   [IN]    Cmd detail to be sent      
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseLaunchBrowser(U8 res, U8 add_info, U8 *cmdDetail)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_launch_browser_res_req_struct *dataPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gSATLaunchBrowser.passedConf = 1;

    Message = ALLOCATE_ITM(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
    Message->oslMsgId = PRT_MMI_SAT_LAUNCH_BROWSER_RES_REQ;
#ifdef __MMI_MULTI_SIM__
    Message->oslDestId = MOD_L4C + gSATContext.CMDfromSIM - SAT_CMD_SIM1;
#else
    Message->oslDestId = MOD_L4C;
#endif
    dataPtr =
        (mmi_sat_launch_browser_res_req_struct*) OslConstructDataPtr(sizeof(mmi_sat_launch_browser_res_req_struct));

     
    if (res == SAT_CMD_PERFORMED_SUCCESSFULLY && gSATLaunchBrowser.isIconAvailable == 1 &&
        gSATContext.isCMDIconSupported == MMI_FALSE)
    {
        dataPtr->res = SAT_CMD_PERFORMED_BUT_ICON_COULD_NOT_DSPL;
    }
    else
    {
         
        dataPtr->res = res;
    }
    dataPtr->size_of_additional_info = add_info;

    memcpy(dataPtr->cmd_detail, (void*)cmdDetail, 5 * sizeof(U8));
    Message->oslDataPtr = (oslParaType*) dataPtr;
    Message->oslPeerBuffPtr = NULL;
#ifdef __MMI_MULTI_SIM__
    mmi_sat_dualsim_sendmsg(Message);
    gSATContext.CMDfromSIM = SAT_CMD_NONE;
#else
    SEND_MSG(Message);
#endif          
    gSATContext.lastSATCmd = SAT_NONE_CMD;
}


/*****************************************************************************
 * FUNCTION
 *  SATLaunchBrowserRsp
 * DESCRIPTION
 *  
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
void SATLaunchBrowserRsp(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    FreeLaunchBrowser();
}


/*****************************************************************************
 * FUNCTION
 *  FreeLaunchBrowser
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void FreeLaunchBrowser(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SATClearAllIcons();
    if (gSATLaunchBrowser.alphaId)
    {
        OslMfree(gSATLaunchBrowser.alphaId);
        gSATLaunchBrowser.alphaId = NULL;
    }

    if (gSATLaunchBrowser.url)
    {
        OslMfree(gSATLaunchBrowser.url);
        gSATLaunchBrowser.url = NULL;
    }

    if (gSATLaunchBrowser.bearer)
    {
        OslMfree(gSATLaunchBrowser.bearer);
        gSATLaunchBrowser.bearer = NULL;
    }

    if (gSATLaunchBrowser.gateway)
    {
        OslMfree(gSATLaunchBrowser.gateway);
        gSATLaunchBrowser.gateway = NULL;
    }
}



#if 0
/**************************************************************

	FUNCTION NAME		: SATSendDTMFInd

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SATSendDTMFInd(void *msgPtr)
{
	mmi_sat_send_dtmf_ind_struct *sendDTMFInd;
	gSATContext.guiBufferPointer=NULL;
	sendDTMFInd=(mmi_sat_send_dtmf_ind_struct *) msgPtr;

	FreeSendDTMF();
	gSATContext.lastSATCmd = SAT_SEND_DTMF_CMD;			
	if(GetCurrentState() != CM_ACTIVE_STATE)
	{
		TerminalResponseSendDTMF(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, SAT_NOT_IN_SPEECH_MODE,sendDTMFInd->cmd_detail);
		return;
	}
	gSATSendDTMF.dcsOfAlphaId=sendDTMFInd->dcs_of_alpha_id;
	gSATSendDTMF.noAlphaId=sendDTMFInd->no_alpha_id;
	gSATSendDTMF.stage1Res=0;

	if(sendDTMFInd->is_alpha_id_present && gSATSendDTMF.noAlphaId)
	{
		gSATSendDTMF.alphaId=CovertStringForPlatform(sendDTMFInd->alpha_id, gSATSendDTMF.noAlphaId,gSATSendDTMF.dcsOfAlphaId,&gSATSendDTMF.noAlphaId);
	}
	else
	{
		gSATSendDTMF.noAlphaId=pfnUnicodeStrlen((PS8)(PU8)GetString(SAT_SEND_DTMF_STRING_ID))*ENCODING_LENGTH;
		gSATSendDTMF.alphaId=(PU8)OslMalloc(gSATSendDTMF.noAlphaId+ENCODING_LENGTH);
		memcpy(gSATSendDTMF.alphaId,(PU8)GetString(SAT_SEND_DTMF_STRING_ID),gSATSendDTMF.noAlphaId+ENCODING_LENGTH);
	}

	gSATSendDTMF.noDtmf = sendDTMFInd->no_dtmf;
	gSATSendDTMF.dtmf=(PU8) OslMalloc(gSATSendDTMF.noDtmf*sizeof(U8) + 1);
	memcpy(gSATSendDTMF.dtmf,sendDTMFInd->dtmf,gSATSendDTMF.noDtmf*sizeof(U8) + 1);

	memcpy(gSATSendDTMF.cmdDetail,sendDTMFInd->cmd_detail, 5 * sizeof(U8));

#ifdef __SAT_MMI_UNIT_TEST__
	gSATSendDTMF.isIconAvailable = 1;
	if (gSATSendDTMF.isIconAvailable) {
		gSATSendDTMF.iconInfo.iconId= 1;
		gSATSendDTMF.iconInfo.iconAttr= SAT_ICON_NOT_SELF_EX;
	}
#else
	gSATSendDTMF.isIconAvailable = sendDTMFInd->is_icon_available;
	if (gSATSendDTMF.isIconAvailable) {
		gSATSendDTMF.iconInfo.iconId = sendDTMFInd->icon_info.icon_id;
		gSATSendDTMF.iconInfo.iconAttr = sendDTMFInd->icon_info.icon_attr;
	}
#endif

	if (gSATSendDTMF.isIconAvailable && gSATSendDTMF.iconInfo.iconAttr == SAT_ICON_SELF_EX) {
		if (gSATSendDTMF.alphaId) {
			OslMfree(gSATSendDTMF.alphaId);
		}
		gSATSendDTMF.alphaId = NULL;
		gSATSendDTMF.noAlphaId = 0;
	}
	SATStopWaitTimer();
	DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);		
	if (GetExitScrnID() == SCR_ID_SAT_HISTORY) {
		ClearExitHandler();
	}
	GoToDTMFConformationScreen();
}

/**************************************************************

	FUNCTION NAME		: GoToDTMFConformationScreen

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void GoToDTMFConformationScreen(void)
{
	gSATContext.exitCondition = &gSATSendDTMF.passedConf;
	EntryNewScreen(SCR_ID_SAT_SEND_DTMF_CONF, SATGenericExitHandler, NULL, NULL);

	SetIsInSATScreen();
	gSATContext.historyFuncPtr=GoToDTMFConformationScreen;
	gSATSendDTMF.passedConf=0;
	ClearAllKeyHandler();
	ShowCategory151Screen(0,0,
							STR_GLOBAL_OK,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							(PU8)GetString(SAT_SEND_DTMF_CONF_STRING_ID), 0, gSATContext.guiBufferPointer);
	SetKeyHandler(SATSendDTMFTerminalResponseExpire,KEY_END,KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(SATSendDTMFStage1Req,KEY_EVENT_UP);
	SetRightSoftkeyFunction(SATSendDTMFTerminalResponseUnable,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: SATSendDTMFTerminalResponseExpire

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SATSendDTMFTerminalResponseExpire(void)
{
	TerminalResponseSendDTMF(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0,gSATSendDTMF.cmdDetail);
	ExitSATToIdle();
}

/**************************************************************

	FUNCTION NAME		: SATSendDTMFTerminalResponseUnable

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SATSendDTMFTerminalResponseUnable(void)
{
	TerminalResponseSendDTMF(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0,gSATSendDTMF.cmdDetail);
	SATPleaseWaitScreen(SAT_WAIT_STRING_ID, SAT_WAIT_IMAGE_ID);
}


/**************************************************************

	FUNCTION NAME		: SATSendDTMFStage1Req

  	PURPOSE				: Send SEND DTMF STAGE1 REQ

 

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SATSendDTMFStage1Req(void)
{
	MYQUEUE *Message;
	mmi_sat_send_dtmf_stage1_req_struct    *dataPtr;
	gSATSendDTMF.passedConf=1;
	gSATContext.gotStage1Rsp = 0;
	DisplayDTMFWindow();
	Message=ALLOCATE_ITM(MOD_MMI);
	Message->oslSrcId = MOD_MMI;
	Message->oslDestId = MOD_L4C;
	Message->oslMsgId =  PRT_MMI_SAT_SEND_DTMF_STAGE1_REQ;
	dataPtr =  (mmi_sat_send_dtmf_stage1_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_send_dtmf_stage1_req_struct));
	dataPtr->no_digit=gSATSendDTMF.noDtmf;
	if(dataPtr->no_digit<=MAX_CC_ADDR_BCD_LEN) {
		memcpy(dataPtr->dtmf_digit,gSATSendDTMF.dtmf, dataPtr->no_digit);
	}
	else {
		memcpy(dataPtr->dtmf_digit,gSATSendDTMF.dtmf, MAX_CC_ADDR_BCD_LEN);
	}
	Message->oslDataPtr = (oslParaType *)dataPtr;
	Message->oslPeerBuffPtr = NULL;
	SEND_MSG(Message);                        
}

/**************************************************************

	FUNCTION NAME		: DisplayDTMFWindow

  	PURPOSE				: Displays Sending DTMF Screen

	INPUT PARAMETERS	: alphaId: pointer to string to be displayed in Send DTMF Screen

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void DisplayDTMFWindow(void)
{
	U8* iconData = NULL;

	if ( gSATSendDTMF.isIconAvailable) {
		SATAddIconIDToList( gSATSendDTMF.iconInfo.iconId);
	}

	if (gSATSendDTMF.isIconAvailable) {
		if ( !SATCheckAllIconGot()) {
			SATGetIconCompleteCallBack = DisplayDTMFWindow;
			SATGetIconDataToList();
			return;
		}
	}
	
	iconData = SATGetIconFromList(gSATSendDTMF.iconInfo.iconId);

	gSATContext.exitCondition = &gSATSendDTMF.stage1Res;
	EntryNewScreen(SCR_ID_SAT_SENDDTMF, SATGenericExitHandler, NULL, NULL);

	SetIsInSATScreen();
	gSATContext.historyFuncPtr=DisplayDTMFWindow;
	ClearHighlightHandler();
	ShowCategory151Screen(0,0,0,0,STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							gSATSendDTMF.alphaId, iconData, gSATContext.guiBufferPointer);
	
	SetRightSoftkeyFunction(SATSendDTMFAbortReq,KEY_EVENT_UP);
	SetKeyHandler(SATSendDTMFAbortReq,KEY_END,KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: SATSendDTMFAbortReq

  	PURPOSE				: Send SEND DTMF Abort REQ

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/


void SATSendDTMFAbortReq(void)
{
	MYQUEUE *Message;
	Message=ALLOCATE_ITM(MOD_MMI);
	Message->oslSrcId = MOD_MMI;
	Message->oslDestId = MOD_L4C;
	Message->oslMsgId =  PRT_MMI_SAT_ABORT_DTMF_REQ;
	Message->oslDataPtr = NULL;
	Message->oslPeerBuffPtr = NULL;
	SEND_MSG(Message);
}



/**************************************************************

	FUNCTION NAME		: SATSendDTMFStage1Rsp

  	PURPOSE				: Protocol Handler for SEND DTMF STAGE 1 RES

 

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SATSendDTMFStage1Rsp(void *msgPtr)
{
	mmi_sat_send_dtmf_stage1_rsp_struct	*sendDTMFStage1Res;
	sendDTMFStage1Res=(mmi_sat_send_dtmf_stage1_rsp_struct	*) msgPtr;
	gSATSendDTMF.stage1Res=1;
	gSATContext.gotStage1Rsp = 1;

//micha0714
	if (gSATContext.lastSATCmd == SAT_NONE_CMD) {
		return;
	}

	
	TerminalResponseSendDTMF(sendDTMFStage1Res->terminal_res, (U8)sendDTMFStage1Res->cause,gSATSendDTMF.cmdDetail);

	if ( sendDTMFStage1Res->terminal_res==SAT_CMD_PERFORMED_SUCCESSFULLY ||
		 sendDTMFStage1Res->terminal_res==SAT_CMD_PERFORMED_WITH_PARTIAL_COMPREHENSION ||
		 sendDTMFStage1Res->terminal_res==SAT_CMD_PERFORMED_WITH_MISSING_INFO)
	{
		SATPleaseWaitScreen(SAT_DTMF_SUCCESS_STRING_ID,0);
	}
	else
	{
		SATPleaseWaitScreen(SAT_DTMF_ERROR_STRING_ID, 0);
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
#endif
}

/**************************************************************

	FUNCTION NAME		: TerminalResponseSendDTMF

 

	INPUT PARAMETERS	: Res: response type
							cause: Returned in SEND DTMF STAGE 1 RES
							cmdDetail: cmd detail to be sent

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/


void TerminalResponseSendDTMF(U8 res, U8 addInfo, U8 *cmdDetail)
{
	MYQUEUE *Message;
	mmi_sat_send_dtmf_stage2_req_struct    *dataPtr;
	gSATSendDTMF.passedConf=1;
	Message=ALLOCATE_ITM(MOD_MMI);
	Message->oslSrcId = MOD_MMI;
	Message->oslDestId = MOD_L4C;
	Message->oslMsgId =  PRT_MMI_SAT_SEND_DTMF_STAGE2_REQ;
	dataPtr =  (mmi_sat_send_dtmf_stage2_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_send_dtmf_stage2_req_struct));

	 
	if(res==SAT_CMD_PERFORMED_SUCCESSFULLY && gSATSendDTMF.isIconAvailable==1 && gSATContext.isCMDIconSupported==MMI_FALSE)
	{
		dataPtr->res=SAT_CMD_PERFORMED_BUT_ICON_COULD_NOT_DSPL;
	}
	else
	{
	 
		dataPtr->res=res;
	}

	dataPtr->additional_info=addInfo;
	memcpy(dataPtr->cmd_detail, cmdDetail, 5*sizeof(U8));
	Message->oslDataPtr = (oslParaType *)dataPtr;
	Message->oslPeerBuffPtr = NULL;
	SEND_MSG(Message);           
	gSATContext.lastSATCmd = SAT_NONE_CMD;
}


/**************************************************************

	FUNCTION NAME		: SATSendDTMFStage2Rsp

  	PURPOSE				: Protocol Handler for SEND DTMF STAGE 2 RES

 

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void SATSendDTMFStage2Rsp(void *msgPtr)
{
	FreeSendDTMF();
}

/**************************************************************

	FUNCTION NAME		: FreeSendDTMF

  	PURPOSE				: Frees various elements of Send DTMF Structure

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void FreeSendDTMF(void)
{
	SATClearAllIcons();
//micha1008
	if(gSATSendDTMF.alphaId)
	{
		OslMfree(gSATSendDTMF.alphaId);
	}
		gSATSendDTMF.noAlphaId=0;
		gSATSendDTMF.alphaId=NULL;

	if(gSATSendDTMF.dtmf)
	{
		OslMfree(gSATSendDTMF.dtmf);
	}
		gSATSendDTMF.noDtmf=0;
		gSATSendDTMF.dtmf=NULL;
}



/**************************************************************

	FUNCTION NAME		: SATSetupIdleDisplayInd
	
  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

//micha1016
void SATSetupIdleDisplayInd(void *msgPtr)
{
	mmi_sat_setup_idle_display_ind_struct* setupIdleDisplayInd;
	setupIdleDisplayInd=(mmi_sat_setup_idle_display_ind_struct *) msgPtr;


	if (g_pSATMainMemu->idleTextPtr != NULL) {
		OslMfree(g_pSATMainMemu->idleTextPtr);
		g_pSATMainMemu->idleTextPtr = NULL;
	}

	if (g_pSATMainMemu->idleIconPtr!= NULL) {
		OslMfree(g_pSATMainMemu->idleIconPtr);
		g_pSATMainMemu->idleIconPtr = NULL;
	}


	memcpy(gSATDisplayIdleText.cmdDetail,(void *)setupIdleDisplayInd->cmd_detail,5*sizeof(U8));
	gSATDisplayIdleText.noTextString = setupIdleDisplayInd->no_text_string;
	gSATDisplayIdleText.dcsOfTextString= setupIdleDisplayInd->dcs_of_text_string;

//micha0418, spec 12.15,  always gsm default encoding
	if (gSATDisplayIdleText.dcsOfTextString == MMI_8BIT_DCS) {
		gSATDisplayIdleText.dcsOfTextString = MMI_DEFAULT_DCS;
	}

	gSATDisplayIdleText.textString = CovertStringForPlatform(setupIdleDisplayInd->text_string, 
										setupIdleDisplayInd->no_text_string, gSATDisplayIdleText.dcsOfTextString,
										&setupIdleDisplayInd->no_text_string);

	gSATDisplayIdleText.isIconAvailable = setupIdleDisplayInd->is_icon_available;

	if (gSATDisplayIdleText.isIconAvailable) {
		gSATDisplayIdleText.iconInfo.iconId = setupIdleDisplayInd->icon_info.icon_id;
		gSATDisplayIdleText.iconInfo.iconAttr= setupIdleDisplayInd->icon_info.icon_attr;
	}

	GoToSATSetupIdleDisplay();
}


#ifdef __SAT_MMI_UNIT_TEST__

void SATSetupIdleDisplayTest(void)
{
	U8 test_string[]="micha test";

	if (g_pSATMainMemu->idleTextPtr != NULL) {
		OslMfree(g_pSATMainMemu->idleTextPtr);
		g_pSATMainMemu->idleTextPtr = NULL;
	}

	if (g_pSATMainMemu->idleIconPtr!= NULL) {
		OslMfree(g_pSATMainMemu->idleIconPtr);
		g_pSATMainMemu->idleIconPtr = NULL;
	}

	gSATDisplayIdleText.noTextString = 10;
	gSATDisplayIdleText.dcsOfTextString= 0;

	gSATDisplayIdleText.textString = CovertStringForPlatform(test_string, 
										gSATDisplayIdleText.noTextString, gSATDisplayIdleText.dcsOfTextString,
										&gSATDisplayIdleText.noTextString);

	gSATDisplayIdleText.isIconAvailable = 1;

	if (gSATDisplayIdleText.isIconAvailable) {
		gSATDisplayIdleText.iconInfo.iconId = 2;
		gSATDisplayIdleText.iconInfo.iconAttr= 1;
	}

	GoToSATSetupIdleDisplay();
}

#endif


/**************************************************************

	FUNCTION NAME		: GoToSATSetupIdleDisplay
	
  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void GoToSATSetupIdleDisplay(void)
{

	U8* iconData = NULL;
	U32 iconSize;

	if ( gSATDisplayIdleText.isIconAvailable)
		SATAddIconIDToList( gSATDisplayIdleText.iconInfo.iconId);


	if (gSATDisplayIdleText.isIconAvailable) {
		if ( !SATCheckAllIconGot()) {
			SATGetIconCompleteCallBack = GoToSATSetupIdleDisplay;
			SATGetIconDataToList();
		return;
		}
	}

	if(gSATDisplayIdleText.noTextString< 300) {
		g_pSATMainMemu->idleTextPtr = OslMalloc( sizeof(U8) * gSATDisplayIdleText.noTextString* ENCODING_LENGTH + 4);
		memset(g_pSATMainMemu->idleTextPtr, 0, gSATDisplayIdleText.noTextString * ENCODING_LENGTH + 4);
		pfnUnicodeStrcpy((PS8)g_pSATMainMemu->idleTextPtr, (PS8)gSATDisplayIdleText.textString);
		}
		else {
		TerminalResponseSetupIdleDisplay(SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, gSATDisplayIdleText.cmdDetail);
		return;
		}

	if (gSATDisplayIdleText.isIconAvailable) {
		iconData = SATGetIconFromList(gSATDisplayIdleText.iconInfo.iconId);
		iconSize = (U32) iconData[2] + (((U32) iconData[3]) << 8) + (((U32) iconData[4]) << 16) + (((U32) iconData[5]) << 24) + 20;
		g_pSATMainMemu->idleIconPtr = OslMalloc( sizeof(U8) * iconSize);
		memcpy( g_pSATMainMemu->idleIconPtr, iconData, iconSize);
	}

	if (gSATDisplayIdleText.isIconAvailable && gSATDisplayIdleText.iconInfo.iconAttr== SAT_ICON_SELF_EX) {
		if (g_pSATMainMemu->idleTextPtr)
			OslMfree(g_pSATMainMemu->idleTextPtr);
		g_pSATMainMemu->idleTextPtr = NULL;
	}

	if(g_idle_context.IsOnIdleScreen) {
		EntryIdleScreen();
	}
	
	TerminalResponseSetupIdleDisplay(SAT_CMD_PERFORMED_SUCCESSFULLY, gSATDisplayIdleText.cmdDetail);

	if (gSATDisplayIdleText.textString) {
		OslMfree(gSATDisplayIdleText.textString);
		gSATDisplayIdleText.textString = NULL;
}
	SATClearAllIcons();
}



/**************************************************************

	FUNCTION NAME		: TerminalResponseSetupIdleDisplay

  	PURPOSE				: 
  	
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void TerminalResponseSetupIdleDisplay(U8 res, U8 *cmdDetail)
{
	MYQUEUE *Message;
	mmi_sat_setup_idle_display_res_req_struct    *dataPtr;
	Message=ALLOCATE_ITM(MOD_MMI);
	Message->oslSrcId = MOD_MMI;
	Message->oslDestId = MOD_L4C;
	Message->oslMsgId =  PRT_MMI_SAT_SETUP_IDLE_DISPLAY_RES_REQ;
	dataPtr =  (mmi_sat_setup_idle_display_res_req_struct *) OslConstructDataPtr(sizeof(mmi_sat_setup_idle_display_res_req_struct));
	dataPtr->res=res;
	memcpy(dataPtr->cmd_detail, cmdDetail, 5*sizeof(U8));
	Message->oslDataPtr = (oslParaType *)dataPtr;
	Message->oslPeerBuffPtr = NULL;
	SEND_MSG(Message);
}


/**************************************************************

	FUNCTION NAME		: SATRunATCommandInd

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SATRunATCommandInd(void *msgPtr)
{
	mmi_sat_run_at_command_ind_struct *displayATCommand;
	displayATCommand=(mmi_sat_run_at_command_ind_struct *)msgPtr;

	if(( GetCurrentState() == CM_OUTGOING_STATE) || ( GetCurrentState() == CM_INCOMING_STATE))
	{
		return;
	}

	SATClearAllIcons();
	if (gSATRunATCommand.alphaID) {
		OslMfree(gSATRunATCommand.alphaID);
		gSATRunATCommand.alphaID= NULL;
	}
	
	memcpy(gSATRunATCommand.cmdDetail,(void *)displayATCommand->cmd_detail,5*sizeof(U8));

	gSATRunATCommand.isIconAvailable = displayATCommand->is_icon_available;

	if (gSATRunATCommand.isIconAvailable) {
		gSATRunATCommand.iconInfo.iconId = displayATCommand->icon_info.icon_id;
		gSATRunATCommand.iconInfo.iconAttr = displayATCommand->icon_info.icon_attr;
	}

	gSATRunATCommand.noAlphaId = displayATCommand->no_alpha_id;
	gSATRunATCommand.isAlphaIdPresent = displayATCommand->is_alpha_id_present;

	gSATRunATCommand.dcsOfAlphaId= displayATCommand->dcs_of_alpha_id;

	if(gSATRunATCommand.isAlphaIdPresent)
	{
		if(gSATRunATCommand.noAlphaId)
		{
			gSATRunATCommand.alphaID = CovertStringForPlatform(displayATCommand->alpha_id, gSATRunATCommand.noAlphaId, gSATRunATCommand.dcsOfAlphaId,&gSATRunATCommand.noAlphaId);
			gSATRunATCommand.informUser=1;
		}
		else
		{
			gSATRunATCommand.informUser=0;
		}
	}
	else
	{
			gSATRunATCommand.informUser=0;
	}

	if (gSATRunATCommand.isIconAvailable && gSATRunATCommand.iconInfo.iconAttr == SAT_ICON_SELF_EX) {
		if (gSATRunATCommand.alphaID) {
			OslMfree(gSATRunATCommand.alphaID);
		}
		gSATRunATCommand.alphaID= NULL;
		gSATRunATCommand.noAlphaId= 0;
	}

	if (gSATRunATCommand.informUser) {
		SATStopWaitTimer();
		DeleteScreenIfPresent(SCR_ID_SAT_HISTORY);		
		if (GetExitScrnID() == SCR_ID_SAT_HISTORY) {
			ClearExitHandler();
		}
		DisplayATCommandScreen();
	}
	else {
		if (gSATRunATCommand.alphaID) {
			OslMfree(gSATRunATCommand.alphaID);
			gSATRunATCommand.alphaID= NULL;
		}
	}
}


/**************************************************************

	FUNCTION NAME		: DisplayATCommandScreen

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void DisplayATCommandScreen(void)
{
	U8* iconData = NULL;

	if ( gSATRunATCommand.isIconAvailable) {
		SATAddIconIDToList( gSATRunATCommand.iconInfo.iconId);
	}

	if (gSATRunATCommand.isIconAvailable) {
		if ( !SATCheckAllIconGot()) {
			SATGetIconCompleteCallBack = DisplayATCommandScreen;
			SATGetIconDataToList();
			return;
		}
	}
	iconData = SATGetIconFromList(gSATRunATCommand.iconInfo.iconId);
	EntryNewScreen(SCR_ID_SAT_SEND_AT_CMD, ExitDisplayATCommandScreen, NULL, NULL);
	
	ShowCategory151Screen(0,0,0,0,0,0,
							gSATRunATCommand.alphaID, iconData, NULL);
	StartTimer(POPUP_TIMER,1000,DisplayATCommandTimerOver);
}


/**************************************************************

	FUNCTION NAME		: ExitDisplayATCommandScreen

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void ExitDisplayATCommandScreen(void)
{
	if (gSATRunATCommand.alphaID) {
		OslMfree(gSATRunATCommand.alphaID);
		gSATRunATCommand.alphaID= NULL;
	}
	SATClearAllIcons();
	StopTimer(POPUP_TIMER);
}

/**************************************************************

	FUNCTION NAME		: DisplayATCommandTimerOver

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void DisplayATCommandTimerOver(void)
{
	StopTimer(POPUP_TIMER);
	if (gSATRunATCommand.alphaID) {
		OslMfree(gSATRunATCommand.alphaID);
		gSATRunATCommand.alphaID= NULL;
	}
	SATClearAllIcons();	
	GoBackHistory();
}
#endif 		//yuxiang E 07.6.23
/*****************************************************************************
 * FUNCTION
 *  SATGenericExitHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SATGenericExitHandler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_current_input_ext_type &= ~INPUT_TYPE_EXT_SIM_NUMERIC;

    if(!(*gSATContext.exitCondition))
    {
        SATSaveHistory();
    }
}


/*****************************************************************************
 * FUNCTION
 *  SATScrKeyHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  eventKey    [?]     
 * RETURNS
 *  void
 *****************************************************************************/
U8 SATScrKeyHandler(KEYBRD_MESSAGE *eventKey)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(eventKey->nMsgType == KEY_EVENT_DOWN) 
    {
        TemporaryStopSATTimerNoResponsefromUser();
        TemporaryStopSATTimerHelp();
    }
    
    if(eventKey->nMsgType == KEY_EVENT_UP) 
    {
        RestartSATTimerNoResponsefromUser();
        RestartSATTimerHelp();
    }
    
    return(MMI_TRUE);
}

/*****************************************************************************
 * FUNCTION
 *  SetIsInSATScreen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SetIsInSATScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gSATContext.isInSATScreen = 1;
    mmi_frm_kbd_reg_pre_key_hdlr(SATScrKeyHandler);
}

/*****************************************************************************
 * FUNCTION
 *  ClearIsInSATScreen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ClearIsInSATScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gSATContext.isInSATScreen = 0;
    mmi_frm_kbd_reg_pre_key_hdlr(NULL);
}


//micha0125 end

/*****************************************************************************
 * FUNCTION
 *  mmi_sat_restart_please_wait_timer
 * DESCRIPTION
 *  Restart please wait timer
 * PARAMETERS
 *  please_wait_time    [IN]    
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_sat_restart_please_wait_timer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(gSATContext.waitTimeExpired == 0)
    {
        StopTimer(SAT_MESSAGE_WAIT_TIMER);
        //StartTimer(SAT_MESSAGE_WAIT_TIMER, SAT_MESSAGE_WAIT_TIMER_DURATION, SATMessageWaitTimer);
    }
}

/*****************************************************************************
* FUNCTION
*	mmi_sat_release_menu_alpha
* DESCRIPTION
*	
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	void
*****************************************************************************/
void mmi_sat_release_menu_alpha(U8 *palpha, U8 *pDestalpha, U16 *npDestlen)
{
	memset(pDestalpha, 0x00, MAX_SAT_ITEMDATA_LEN);
	
	if(palpha != NULL && *npDestlen < MAX_SAT_ITEMDATA_LEN)
	{
		memcpy(pDestalpha, palpha, *npDestlen + 1);
	}
	else
	{
		*npDestlen = 0;
	}
	
	if(palpha != NULL)
	{
		OslMfree(palpha);
	}	
}

void mmi_sat_dualsim_sendmsg(MYQUEUE *Msg)
{
	COS_EVENT ev; 
	ev.nEventId = Msg->oslMsgId; 
	ev.nParam1 = (UINT32)Msg; 
	ev.nParam3 = Msg->oslDestId; 
	SEND_COSMSG(Msg->oslDestId, &ev); 	
}

#ifdef __MMI_MULTI_SIM__
/*****************************************************************************
 * FUNCTION
 *  TerminalResponseSelectItemEx
 * DESCRIPTION
 
 * PARAMETERS
 
 *  res         [IN]    Response type
 *  itemId      [IN]    Item id of selcted item
 *  cmdDetail   [IN]    Cmd detail to be sent
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseSelectItemEx(U8 destId, U8 res, U8 itemId, U8 *cmdDetail)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_select_item_res_req_struct *dataPtr;
  COS_EVENT ev;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Message = allocate_ilm(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
    Message->oslDestId = destId;
    Message->oslMsgId = PRT_MMI_SAT_SELECT_ITEM_RES_REQ;
    dataPtr =
        (mmi_sat_select_item_res_req_struct*) OslConstructDataPtr(sizeof(mmi_sat_select_item_res_req_struct));
    dataPtr->res = res;
    dataPtr->item_id = itemId;
    memcpy(dataPtr->cmd_detail, (void*)cmdDetail, 5 * sizeof(U8));
    Message->oslDataPtr = (oslParaType*) dataPtr;
    Message->oslPeerBuffPtr = NULL;
  Message->oslSapId = INVALID_SAP;
	
	//msg_send_ext_queue(Message);    
  ev.nEventId = Message->oslMsgId; 
	ev.nParam1 = (UINT32)Message; 
  ev.nParam3 = Message->oslDestId; 
  SEND_COSMSG(Message->oslDestId, &ev); 
	
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseDisplayTextEx
 * DESCRIPTION
 
 * PARAMETERS
 
 *  res         [IN]    Response type
 *  cmdDetail   [IN]    Cmd detail to be sent
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseDisplayTextEx(U8 destId, U8 res, U8 *cmdDetail)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_display_text_res_req_struct *dataPtr;
    COS_EVENT ev;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Message = allocate_ilm(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
    Message->oslDestId = destId;
    Message->oslMsgId = PRT_MMI_SAT_DISPLAY_TEXT_RES_REQ;
    dataPtr =
        (mmi_sat_display_text_res_req_struct*) OslConstructDataPtr(sizeof(mmi_sat_display_text_res_req_struct));
    dataPtr->res = res;
    if (dataPtr->res == SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD)
    {
        dataPtr->additional_info = SAT_SCREEN_BUSY;
    }
    memcpy(dataPtr->cmd_detail, (void*)cmdDetail, 5 * sizeof(U8));
    Message->oslDataPtr = (oslParaType*) dataPtr;
    Message->oslPeerBuffPtr = NULL;
    Message->oslSapId = INVALID_SAP;

    ev.nEventId = Message->oslMsgId; 
    ev.nParam1 = (UINT32)Message; 
    ev.nParam3 = Message->oslDestId; 
    SEND_COSMSG(Message->oslDestId, &ev); 
	
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseGetInputEx
 * DESCRIPTION
 
 * PARAMETERS
 
 *  res         [IN]    Response type
 *  cmdDetail   [IN]    Cmd detail to be sent
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseGetInputEx(U8 destId, U8 res, U8 *cmdDetail)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_get_input_res_req_struct *dataPtr;
    COS_EVENT ev;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Message = allocate_ilm(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
    Message->oslDestId = destId;
    Message->oslMsgId = PRT_MMI_SAT_GET_INPUT_RES_REQ;
    dataPtr = (mmi_sat_get_input_res_req_struct*) OslConstructDataPtr(sizeof(mmi_sat_get_input_res_req_struct));
    dataPtr->res = res;
    dataPtr->no_text_string = 0;
    dataPtr->dcs_of_text_string = 0;
    dataPtr->text_string[0] = 0;
    memcpy(dataPtr->cmd_detail, (void*)cmdDetail, 5 * sizeof(U8));
    Message->oslDataPtr = (oslParaType*) dataPtr;
    Message->oslPeerBuffPtr = NULL;
    Message->oslSapId = INVALID_SAP;

    //msg_send_ext_queue(Message);    
    ev.nEventId = Message->oslMsgId; 
    ev.nParam1 = (UINT32)Message; 
    ev.nParam3 = Message->oslDestId; 
    SEND_COSMSG(Message->oslDestId, &ev); 
	
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseGetInkeyEx
 * DESCRIPTION
 
 * PARAMETERS
 
 *  res             [IN]    Response type
 *  isYesSelected   [IN]    Is Yes Selected on Get Inkey Screen (TRUE/FALSE)
 *  cmdDetail       [IN]    Cmd detail to be sent
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseGetInkeyEx(U8 destId, U8 res, U8 isYesSelected, U8 *cmdDetail)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_get_inkey_res_req_struct *dataPtr;
    COS_EVENT ev;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Message = allocate_ilm(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
    Message->oslDestId = destId;
    Message->oslMsgId = PRT_MMI_SAT_GET_INKEY_RES_REQ;
    dataPtr = (mmi_sat_get_inkey_res_req_struct*) OslConstructDataPtr(sizeof(mmi_sat_get_inkey_res_req_struct));
    dataPtr->res = res;
    dataPtr->is_yes_selected = isYesSelected;
    dataPtr->no_text_string = 0;
    dataPtr->dcs_of_text_string = 0;
    dataPtr->text_string[0] = 0;
#ifdef __USAT__
    dataPtr->duration = mmi_sat_count_get_inkey_user_rsp();
#endif 
    memcpy(dataPtr->cmd_detail, (void*)cmdDetail, 5 * sizeof(U8));
    Message->oslDataPtr = (oslParaType*) dataPtr;
    Message->oslPeerBuffPtr = NULL;
    Message->oslSapId = INVALID_SAP;

    //msg_send_ext_queue(Message);    
    ev.nEventId = Message->oslMsgId; 
    ev.nParam1 = (UINT32)Message; 
    ev.nParam3 = Message->oslDestId; 
    SEND_COSMSG(Message->oslDestId, &ev); 
	
}
/*****************************************************************************
 * FUNCTION
 *  TerminalResponseCallSetupEx
 * DESCRIPTION
 
 * PARAMETERS
 
 *  res         [IN]    Response type
 *  addInfo     [IN]    Additional info
 *  cmdDetail   [IN]    Cmd detail to be sent
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseCallSetupEx(U8 destId, U8 res, U8 addInfo, U8 *cmdDetail)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_call_setup_stage2_req_struct *dataPtr;
    COS_EVENT ev;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Message = allocate_ilm(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
    Message->oslDestId = destId;
    Message->oslMsgId = PRT_MMI_SAT_CALL_SETUP_STAGE2_REQ;
    dataPtr =
        (mmi_sat_call_setup_stage2_req_struct*) OslConstructDataPtr(sizeof(mmi_sat_call_setup_stage2_req_struct));
    dataPtr->res = res;
    dataPtr->cause = addInfo;
    memcpy(dataPtr->cmd_detail, (void*)cmdDetail, 5 * sizeof(U8));
    Message->oslDataPtr = (oslParaType*) dataPtr;
    Message->oslPeerBuffPtr = NULL;
    Message->oslSapId = INVALID_SAP;

	//msg_send_ext_queue(Message);    
	ev.nEventId = Message->oslMsgId; 
	ev.nParam1 = (UINT32)Message; 
	ev.nParam3 = Message->oslDestId; 
	SEND_COSMSG(Message->oslDestId, &ev); 
	
}


/*****************************************************************************
 * FUNCTION
 *  TerminalResponseSendSMS_2
 * DESCRIPTION
 
 * PARAMETERS
 
 *  res         [IN]    Response type
 *  cause       [IN]    Returned in SEND SMS STAGE 1 RES
 *  cmdDetail   [IN]    Cmd detail to be sent
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseSendSMS_2(U8 destId, U8 res, U8 cause, U8 *cmdDetail)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_send_sms_stage2_req_struct *dataPtr;
    COS_EVENT ev;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Message = allocate_ilm(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
    Message->oslDestId = destId;
    Message->oslMsgId = PRT_MMI_SAT_SEND_SMS_STAGE2_REQ;
    dataPtr = (mmi_sat_send_sms_stage2_req_struct*) OslConstructDataPtr(sizeof(mmi_sat_send_sms_stage2_req_struct));
    dataPtr->res = res;
    dataPtr->cause = cause;
    memcpy(dataPtr->cmd_detail, cmdDetail, 5 * sizeof(U8));
    Message->oslDataPtr = (oslParaType*) dataPtr;
    Message->oslPeerBuffPtr = NULL;
    Message->oslSapId = INVALID_SAP;

	//msg_send_ext_queue(Message);    
	ev.nEventId = Message->oslMsgId; 
	ev.nParam1 = (UINT32)Message; 
	ev.nParam3 = Message->oslDestId; 
	SEND_COSMSG(Message->oslDestId, &ev); 
	
}

/*****************************************************************************
 * FUNCTION
 *  TerminalResponseSendSS_2
 * DESCRIPTION
 
 * PARAMETERS
 
 *  res         [IN]    Response type
 *  len         [IN]    Len of additional info, returned in SEND SS STAGE 1 RES
 *  info        [IN]    Additional info, returned in SEND SS STAGE 1 RES
 *  cmdDetail   [IN]    Cmd detail to be sent
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseSendSS_2(U8 destId, U8 res, U8 len, U8 *info, U8 *cmdDetail)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_send_ss_stage2_req_struct *dataPtr;
    COS_EVENT ev;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Message = allocate_ilm(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
    Message->oslDestId = destId;
    Message->oslMsgId = PRT_MMI_SAT_SEND_SS_STAGE2_REQ;
    dataPtr = (mmi_sat_send_ss_stage2_req_struct*) OslConstructDataPtr(sizeof(mmi_sat_send_ss_stage2_req_struct));
    dataPtr->res = res;
    dataPtr->size_of_additional_info = len;
    dataPtr->size_of_additional_info = 255;
    memcpy(dataPtr->additional_info, info, dataPtr->size_of_additional_info);
    memcpy(dataPtr->cmd_detail, (void*)cmdDetail, 5 * sizeof(U8));
    Message->oslDataPtr = (oslParaType*) dataPtr;
    Message->oslPeerBuffPtr = NULL;
    //Message->sap_id = INVALID_SAP;
    
    //msg_send_ext_queue(Message);
    ev.nEventId = Message->oslMsgId; 
    ev.nParam1 = (UINT32)Message; 
    ev.nParam3 = Message->oslDestId; 
    SEND_COSMSG(Message->oslDestId, &ev);     
}


/*****************************************************************************
 * FUNCTION
 *  TerminalResponseSendUSSD_2
 * DESCRIPTION
 
 * PARAMETERS
 
 *  res         [IN]    Response type
 *  len         [IN]    Len of additional info, returned in SEND USSD STAGE 1 RES
 *  info        [IN]    Additional info, returned in SEND USSD STAGE 1 RES
 *  cmdDetail   [IN]    Cmd detail to be sent
 * RETURNS
 *  void
 *****************************************************************************/
void TerminalResponseSendUSSD_2(U8 destId, U8 res, U8 len, U8 *info, U8 *cmdDetail)
{
	#if defined(MMI_ON_HARDWARE_P)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE *Message;
    mmi_sat_send_ussd_stage2_req_struct *dataPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Message = allocate_ilm(MOD_MMI);
    Message->oslSrcId = MOD_MMI;
    Message->oslDestId = destId;
    Message->oslMsgId = PRT_MMI_SAT_SEND_USSD_STAGE2_REQ;
    dataPtr = (mmi_sat_send_ussd_stage2_req_struct*) OslConstructDataPtr(sizeof(mmi_sat_send_ussd_stage2_req_struct));
    dataPtr->res = res;
    dataPtr->size_of_additional_info = len;

    memcpy(dataPtr->additional_info, info, dataPtr->size_of_additional_info);
    memcpy(dataPtr->cmd_detail, (void*)cmdDetail, 5 * sizeof(U8));
    Message->oslDataPtr = (oslParaType*) dataPtr;
    Message->oslPeerBuffPtr = NULL;
    Message->sap_id = INVALID_SAP;
    msg_send_ext_queue(Message);
	#endif
}

#endif

