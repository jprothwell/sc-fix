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
 * SimDetection.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is for handle Normal Bootup
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
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: SimDetection.c

  	PURPOSE		: handle Normal Bootup

 

 

	DATE		: 12/03/03

**************************************************************/
#define __NEWSIMULATOR 
#include "mmi_features.h"   
#include "mmi_trace.h"

#include "stdc.h"
#include "l4dr1.h"

#include "historygprot.h"
#include "sublcdhistorygprot.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_popup.h"
#include "wgui_categories_cm.h"
#include "commonscreens.h"
#include "miscfunctions.h"
#include "custdatares.h"
#include "protocolevents.h"
#include "timerevents.h"
#include "simdetectiongprot.h"
//#include "simdetectiongdcl.h"
#include "simdetectiondef.h"
//#include "simdetectiontype.h"
#include "mmi_data_types.h"
#include "quickaccessmenuitem_def.h"
#include "quickaccessmenuitemprot.h"  
#include "simdetectiongexdcl.h"
#include "eventsdcl.h"
#include "queuegprot.h"
#include "unicodexdcl.h"
#include "protocolevents.h"
#include "debuginitdef.h"
#include "ucs2prot.h" 
#include "taskinit.h"
#include "wgui_status_icons.h"
#include "custmenures.h"
#include "globaldefs.h"
#include "idleappdef.h"
#include "alarmframeworkprot.h"     //dyj del 20060514
#include "sscstringhandle.h"
#include "phonebooktypes.h"
#include "settingprofile.h"
#include "settinggprots.h"
#include "profilegprots.h"
//#include "personalizationgprot.h"
#include "personalizationprot.h"
#include "settingsgexdcl.h"
#include "led.h"
#include "gpioinc.h"
#include "keybrd.h"
#include "mainmenudef.h"
#include "poweronchargerprot.h"
#include "profilesgexdcl.h"
#include "bootup.h"
#include "callsetup.h"
#include "phonebookgprot.h"
#include "idleappprot.h"
#include "funandgamesdefs.h"
#include "gsm7bitdefaultalphabet.h"
//micha0606
#include "securitysetup.h"
#include "satgprots.h"
#include "globalscrenum.h"

#include "unit_test.h"
#include "dual_sim.h"

#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__  
#include "statusiconres.h"
#endif

#ifdef MMS_SUPPORT
extern void setMmscVersion(const U8 *curPLMN);
extern void writeRoamingStatusForMMS(U32 isRoaming);
#endif

#ifdef __MOD_SMSAL__
#include "messagesexdcl.h"
#endif

#include "l4dr.h"
#include "simdetectionhwmapfile.h"
#include "custom_util.h"
#include "mci.h"
//micha0317
#include "networksetupgprot.h"
 
#include "networksetupdefs.h"

//micha0406
#include "custresdef.h"
#include "plmnenum.h"

//micha0420
#include "settingdefs.h"
#include "settingsgdcl.h"

#include "phonesetupgprots.h"

#ifdef __MMI_USB_SUPPORT__
#include "usbdevicegprot.h"
#endif

#ifdef _NETWORK_CIPHER_SUPPORT_
#include "mm_mmi_enums.h"
#endif

#ifdef __MMI_IMPS__
#include "mmi_imps_gprot.h"
#endif

 
#ifdef __MMI_POC__
#include "pocgprot.h"
#endif

#if defined(  __FS_CHECKDRIVE_SUPPORT__ )
#include "fmt_struct.h"
#ifdef __MMI_FILE_MANAGER__
#include "filemanagergprot.h"
#endif
#endif

#ifdef __MMI_MULTI_SIM__
#include "mtpnp_ad_resdef.h"
#include "dual_sim_call.h"
#endif
#include "usb_interface.h"
#undef __NEWSIMULATOR 

#if ( defined(__MMI_ENGINEER_MODE__) || defined(__MMI_FACTORY_MODE__) )
extern U8 gCurrentMode;
#endif

#ifdef __MMI_BT_SUPPORT__
extern MMI_BOOL mmi_bt_ps_stack_and_hw_init(MMI_BOOL flight_mode, MMI_BOOL sim_inserted);
extern void mmi_bt_pwroff(void);

#endif

extern U8 gIsOnSearchingScreen;

#ifdef __MMI_EONS_SUPPORT__
extern void eons_get_opn_file_info_req(void);
extern void eons_get_pnn_record_req(U8);
extern void eons_read_opn_from_nvram(void);
extern U8 eons_check_if_plmn_present(U8*);
extern kal_bool is_sim_replaced(void);
#endif

extern void idle_screen_show_network_name(void);

//micha0519, modify thie in whold file
extern void FillCountDownNumber( sim_chv_info_struct	chv_info, U8 nSimID);
extern void FillCountDownString(U8 TypeOfPass, U8 *PassCountdownDisp, U8 nSimID);
extern void CallBackPowerOnAnimationComplete(GDI_RESULT ret);
extern void DeInitSpecialMode(void);
extern void refresh_status_icon_display(void);
extern void goto_opening_screen(void);
extern void MakeCall(PS8 strBumber);
extern void IdleSetStatusIcon(S16);
extern void IdleResetStatusIcon(S16);
extern void BatteryStatusRsp(void*);
//micha0420
//extern void RstSmuVerifyPINRspHdlr(void *);
extern void InitInsertSimApp(void);
extern void RefreshDisplayOwnerNumber(U8 lineid);
extern pBOOL isInCall(void);
extern void SendMessageForUSBPlugIn();
extern void SendMessageForUSB(UINT32 action);
extern U8 GetHandsetInPhone();
extern int FS_SetDiskFlag(void);
#ifdef __J2ME__
extern void j2me_enter_idle_screen_notify(void);
#endif
extern void SearchingSrceenTimeOutHandler(void);
extern  pBOOL mmi_bootup_get_active_flight_mode();

void PowerOnBatteryIndicationComplete(void);
void InsertSimSetKeyPadLockFlag(void);
void mmi_sim_error_idle_toggle_profile(void);//qiff add for m301,BUG:10748
extern void StopPowerOnOffLED(void);
pBOOL mmi_bootup_is_roaming(UINT8 sim_id);

 
//extern U8 chargerLevel;
//extern U8 batteryStatus;
//extern U8 isChargerConnected;
//CSD end

extern charbat_context_struct g_charbat_context;
extern U8 gKeyPadLockFlag;
//micha0420
//extern U8 gSettings;
extern U8 isEarphonePlugged;
/*Jinzh Add Start For 6424 Ver: TBM780  on 2007-8-28 17:37 */
extern U8 isEarphonePlugged_1;  
/*Jinzh Add End  For 6424 Ver: TBM780  on 2007-8-28 17:37 */
//micha0309
//extern CallSetupContext g_callset_context;
//extern U16 gLineNo;
//cong.li adds for singleSimCardWapMMS on 2009.07.08
extern U8 gInsertSimAppFlag;

//micha0202
U8 gGPRSStatusModified = 0;
//micha0202, If PIN locked, shouldn't go back to idle screen
//extern U8 gPINLocked;


// Lisen reorged the code and moved variables here. 04/21/2004 
S8 gInputNewPinBufferSecondTime[MAX_PWD_LENGTH_MMI * ENCODING_LENGTH ];
S8 gInputNewPinBufferFirstTime[MAX_PWD_LENGTH_MMI * ENCODING_LENGTH ];
S8 gInputPwdBuffer[MAX_PWD_LENGTH_MMI * ENCODING_LENGTH ];

U8 gNWProviderName[MMI_SIM_NUMBER][MAX_LENGTH_DISPLAY_NAME * ENCODING_LENGTH];
U8 gSignalStrengthLevel[MMI_SIM_NUMBER] = {0,};
U8 gPLMN[MMI_SIM_NUMBER][MAX_PLMN_LEN_MMI+1];
MMI_SPN_INFO gSPNIfo[MMI_SIM_NUMBER];

/* CSD 1211:
 1: Never send get IMEI request
 2: Waiting get IMEI response
 3: IMEI response has came  */
//U8 testRoaming = 1;
/*Fix Bug#12330 for cs2324c by cong.li on 2009.06.24. 
   PLMN=MCC(3)+MNC(2), such as: 46000, 46001*/
S8 gHomePLMNNum[MMI_SIM_NUMBER][MAX_PLMN_LEN_MMI+1];
S8 gSimEccNumList[MAX_ECC_NUMBER_LIST_LENGTH][(EMERGENCY_NUMBER_LENGTH+1)];
S8 gServProvderName[MMI_SIM_NUMBER][MAX_LENGTH_SERVICE_PROVIDER*ENCODING_LENGTH];
S8 gOperatorName[MAX_LENGTH_OPERATOR_NAME*ENCODING_LENGTH];
S8 gShortOperatorName[MAX_LENGTH_SHORT_OPERATOR_NAME*ENCODING_LENGTH];
S8 gOwnerName[MMI_SIM_NUMBER][MAX_LENGTH_OWNER_NUMBER*ENCODING_LENGTH];
U8 gOpNameValid = 0;
U8 gShortOpnameValid = 0;
U8 gSpnValid[MMI_SIM_NUMBER] = {0};
extern U8 g_SimSuccessIndBuffered[MMI_SIM_NUMBER];
U8 gPhoneLockStore = 0;
MMISMUPASSWDREQIND gSMUPwdDataStruct[MMI_SIM_NUMBER], gSMUPwdDataStruct_phoneLock; //chenhe add phonelock variable to remember the very info
MMISMUFAILIND gSMUFailIndStruct[MMI_SIM_NUMBER];
MMI_OWNER_NUMBER_STRUCT *g_pDispOwnerNumber;
MMI_OWNER_NUMBER_STRUCT  gDispOwnerNumber[MMI_SIM_NUMBER][2];

U8 gSIMRefresh[MMI_SIM_NUMBER] = {0,};
U8 lastNwStatus[MMI_SIM_NUMBER];
U8 lastPLMN[MMI_SIM_NUMBER][MAX_PLMN_LEN_MMI+1];

//Lisen 04112005
U16 EmergencyNumber[MAX_EMERGENCYNUMBER][4] = {
      {0x0031, 0x0031, 0x0032, 0x00},
#if defined(__PCS1900__ )
	{
		0x0039, 0x0031, 0x0031, 0x00
	},

#endif
#if defined(__MEXICO_ECC__)
	{
		0x0030, 0x0038, 0x0030, 0x00
	},

	{0x0030, 0x0036, 0x0030, 0x00},
	{0x0030, 0x0038, 0x00}
#endif
};

//micha1230
U8 gCPHSRetryCount = 0;

 
extern U8 isIdleScreenReached;

typedef enum 
{
	SIM_ERROR_DISPLAY_INSERT_SIM,				
	SIM_ERROR_DISPLAY_SIM_ACCESS_ERROR
} SIM_ERROR_DISPLAY; 

SIM_ERROR_DISPLAY  simErrorDisplayString = SIM_ERROR_DISPLAY_INSERT_SIM;
	
//micha0406,  moved to customer folder
extern S8 sPlmnsToDisplayName[][MAX_PLMN_LEN_MMI+1];


#if defined(  __FS_CHECKDRIVE_SUPPORT__ )
// NOT DONE or NONE: 0
// SKIP: 1
// DONE: 2
// Processing: 3
U8   SanityCheckDone = 0; 
#else
U8   SanityCheckDone = 2; 
#endif

#define __MMI_ENTRY_IDLE_QUICKLY__

#if defined(TGT_GALLITE_G800)
#ifdef __MMI_ENTRY_IDLE_QUICKLY__
#undef __MMI_ENTRY_IDLE_QUICKLY__
#endif
#endif

// CSD 04/18/2004 Lisen : code re-org
pwr_context_struct g_pwr_context[MMI_SIM_NUMBER];

void mmi_init_pwr_context(void)
{
	U8 i;
	mmi_trace(1, "mmi_init_pwr_context");
	memset(g_pwr_context, 0, sizeof(g_pwr_context));

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		g_pwr_context[i].testRoaming = 1;
		g_pwr_context[i].CurrentServiceType = LIMITED_SERVICE;
		g_pwr_context[i].RequireRPLMN = 1;
		#ifdef __MANUAL_MODE_NW_SEL__
		g_pwr_context[i].PLMNSelectionMode = 1;
		#endif
	}
}

/**************************************************************

	FUNCTION NAME		: InitSimDetection()

  	PURPOSE				: Initialize all protocol handlers for SIM Messages

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: vod

 

**************************************************************/
void InitSimDetection(void)
{
	U16 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetProtocolEventHandlerExt(SimPasswdReqResponse, MSG_ID_GET_PIN_RES, i);
		SetProtocolEventHandlerExt(SimQueryFailureRsp, MSG_ID_SIM_CARD_FAILURE, i);
	}
	SetProtocolEventHandler(SimECCInfoSave, PRT_SMU_STARTUP_INFO_IND);
}

/**************************************************************


	FUNCTION NAME		: GetCurrentPLMNName

  	PURPOSE				: to get PLMN name of input PLMN No

	INPUT PARAMETERS	: PLMN Number

	OUTPUT PARAMETERS	: Nil

	RETURNS				: Pointer to PLMN Name

 

**************************************************************/
PS8 GetCurrentPLMNName(S8* PlmnNo)
{
	U16 count;

	for(count=0;count<MAX_PLMN_NO_LIST_LENGTH;count++)
	{
//micha0422
		if (strncmp(PlmnNo,sPlmnsToDisplayName[count],strlen(sPlmnsToDisplayName[count]))==0)
		{
			count=count+STR_PLMN_NAME_1;
			return GetString(count);
		}
	}
	return NULL;
}


/**************************************************************


	FUNCTION NAME		: InitNetWorkBootUpEventsCallBack

  	PURPOSE				: This function initializes all network related protocol handlers

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void
 

**************************************************************/


void InitNetWorkBootUpEventsCallBack(void)
{
	U16 i;
	
	for( i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetProtocolEventHandlerExt(NetWorkAttachedIndication, PRT_NW_ATTACH_IND, i);
		SetProtocolEventHandlerExt(RspToTestRoamingEnabled, PRT_GET_IMSI_RSP, i);
		SetProtocolEventHandlerExt(NumberOfPLMNSuported, PRT_PLMN_LIST_IND, i);
		SetProtocolEventHandlerExt(NetWorkSignalLevelIndication, PRT_NW_RX_LEVEL_IND,i);
		#ifdef __MMI_GPRS_FEATURES__
		SetProtocolEventHandlerExt(GPRSStatusUpdateIndication,PRT_MMI_PS_GPRS_STATUS_UPDATE_IND, i);
		#endif

		lastNwStatus[i] = MMI_RAC_LIMITED_SERVICE;
	}
	
	SetProtocolEventHandler(NetWorkServiceTypeIndication, PRT_SERVICE_TYPE_IND);
	
	
	SetProtocolEventHandler(BatteryStatusRsp, PRT_BATTERY_STATUS_IND);

#ifdef _NETWORK_CIPHER_SUPPORT_
	SetProtocolEventHandler(NetWorkCipherIndication, PRT_MMI_SMU_CIPHER_IND);
#endif

#ifdef __MANUAL_MODE_NW_SEL__
	SetProtocolEventHandler(PLMNSelectionModeIndication, MSG_ID_MMI_NW_SEL_MODE_IND);
#endif

/*Fix Bug#12330 for cs2324c by cong.li on 2009.06.24. 
   PLMN=MCC(3)+MNC(2), such as: 46000, 46001*/
	memset(gPLMN, 0, sizeof(gPLMN));
	memset(gHomePLMNNum, '\0', sizeof(gHomePLMNNum));
}
#ifdef _NETWORK_CIPHER_SUPPORT_
/**************************************************************

	FUNCTION NAME		: NetWorkCipherIndication

  	PURPOSE				: Call back function for ciphering indeication

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void NetWorkCipherIndication(void *info)
{
	mmi_smu_cipher_ind_struct *local_data = (
	mmi_smu_cipher_ind_struct *)info;
	PRINT_INFORMATION(("\n NetWorkCipherIndication\n"));
	if(local_data->cipher_ind && local_data->is_gsm_conn_exist)
	{
		if(local_data->gsm_cipher_cond==GMMREG_CIPHER_OFF)
		{
			ShowStatusIcon(STATUS_ICON_NETWORK_CIPHER_GSM);//warning for insecure network
		}
		else if(local_data->gsm_cipher_cond==GMMREG_CIPHER_ON
				||local_data->gsm_cipher_cond==GMMREG_CIPHER_INVALID)
		{
			HideStatusIcon(STATUS_ICON_NETWORK_CIPHER_GSM);//warning for insecure network
		}
		else if(local_data->gprs_cipher_cond==GMMREG_CIPHER_OFF)
		{
			ShowStatusIcon(STATUS_ICON_NETWORK_CIPHER_GPRS);//warning for insecure network
		}
		else if(local_data->gprs_cipher_cond==GMMREG_CIPHER_ON
				||local_data->gsm_cipher_cond==GMMREG_CIPHER_INVALID)
		{
			HideStatusIcon(STATUS_ICON_NETWORK_CIPHER_GPRS);//warning for insecure network
		}

	}
	else//cipher not supported
	{
		HideStatusIcon(STATUS_ICON_NETWORK_CIPHER_GSM);
		HideStatusIcon(STATUS_ICON_NETWORK_CIPHER_GPRS);
	}
		UpdateStatusIcons();
}
#endif

#ifdef __MANUAL_MODE_NW_SEL__
/*****************************************************************************
* FUNCTION
*	 PLMNSelectionModeIndication()
* DESCRIPTION
*  Call back function for PLMN Sel Mode ind
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void PLMNSelectionModeIndication(void *info)
{

	U8 nSimID;
	mmi_nw_sel_mode_ind_struct *local_data = (mmi_nw_sel_mode_ind_struct *)info;
	PRINT_INFORMATION(("\n NetworkSelectionModeIndication\n"));

	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	g_pwr_context[nSimID].PLMNSelectionMode = local_data->network_mode;
	IdleRefreshServiceIndicationArea();
	
	if(g_idle_context.IsOnIdleScreen)
		IdleSetLRKey();
}	
/*****************************************************************************
* FUNCTION
*	 PLMNGetSelectionModeRsp()
* DESCRIPTION
*  Call back function for PLMNGetSelectionModeReq
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void PLMNGetSelectionModeRsp(void *info)
{
	U8 nSimID;
	mmi_nw_get_plmn_select_mode_rsp_struct* local_data = (mmi_nw_get_plmn_select_mode_rsp_struct *)info;
	PRINT_INFORMATION(("\n PLMNGetSelectionModeRsp\n"));

	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	g_pwr_context[nSimID].PLMNSelectionMode = local_data->mode;
	ClearProtocolEventHandler(MSG_ID_MMI_NW_GET_PLMN_SELECT_MODE_RSP);
	return;
}	


/*****************************************************************************
* FUNCTION
*	 PLMNGetSelectionModeReq()
* DESCRIPTION
*  Ask the protocol to provide the selection mode
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void PLMNGetSelectionModeReq(void)
{
	MYQUEUE msgStruct;
	ClearAllKeyHandler();
	
	SetProtocolEventHandler(PLMNGetSelectionModeRsp, MSG_ID_MMI_NW_GET_PLMN_SELECT_MODE_RSP);
	/* format the message to get band selection */
	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MOD_L4C;
	msgStruct.oslMsgId = (unsigned int)MSG_ID_MMI_NW_GET_PLMN_SELECT_MODE_REQ;
	msgStruct.oslSapId = (unsigned int)0;
	msgStruct.oslDataPtr = NULL;
	msgStruct.oslPeerBuffPtr= NULL;
	 
	OslMsgSendExtQueue(&msgStruct);
	return;
}	

#endif /*__MANUAL_MODE_NW_SEL__*/

/**************************************************************

	FUNCTION NAME		: EntryScrnSimErrorScreen()

  	PURPOSE				: Entry screen when no SIM is inserted

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
#ifdef __J2ME__
extern BOOL java_is_launch_after_install(void);
extern void mmi_java_send_mid_start_req(void);
#endif

//#include "mci.h"
extern BOOL g_BeforeEntryIdleScr;
#if (CSW_EXTENDED_API_PMD_USB == 0)
extern BOOL g_NeedReDetectUsb;
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */
extern BOOL do_usbplugout_later;

extern void SendMessageForUSB(UINT32 action);
#include "bootup.h"
extern void BatteryIndicationPopup(U16 stringId);

extern BOOL Is_KeyEnd_Press_On_Sim_Error_Src(void);
extern BOOL g_endkey_is_pressed_on_sim_error_src;

#if defined __MMI_OPP_SUPPORT__ 
#include "btmmioppgprots.h"
#include "oppmmigprots.h"
#include "bluetooth_struct.h"
#include "filemgr.h"
#include "btmmiopp.h"
extern BOOL g_NeedRecheckOppRecv;
extern void mmi_opp_scr_recv_obj_ind(void);
#endif

void EntryScrnSimErrorScreen()
{

	U8* guiBuffer;       /* Buffer holding history data */
        U16 msgString; 
	U16 InsertSIMScreenDigits[MAX_INSERT_SCR_DIGITS] = {KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_STAR,KEY_POUND};
	
	g_BeforeEntryIdleScr = 0;
	
#if (CSW_EXTENDED_API_PMD_USB == 0)
	if(g_NeedReDetectUsb)
	{
		g_NeedReDetectUsb = 0;
		if (USB_DETECTED_OK == MCI_USBDetect())
			SendMessageForUSB(DEVUSB_DETECT_ACTION_PLUGIN); //chenhe,080607
		else
			BatteryIndicationPopup(STR_CHARGER_CONNECTED);
	}
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */
	if (do_usbplugout_later)
	{
		do_usbplugout_later = 0;
		SendMessageForUSB(DEVUSB_DETECT_ACTION_PLUGOUT);
	}
	

	EntryNewScreen(SCR_SIM_INSERTION, ExitScrnSimErrorScreen, EntryScrnSimErrorScreen, NULL);
	
//Dinit History is called to prevent memory leak.
	DinitHistory();
	DeInitSpecialMode(); /* Back from engineer mode or factory mode */
	
	/*zhangm Add Start For 10963 Ver: GreenStone  on 2008-12-22 11:21 */
	g_endkey_is_pressed_on_sim_error_src = Is_KeyEnd_Press_On_Sim_Error_Src();
	/*zhangm Add End  For 10963 Ver: GreenStone  on 2008-12-22 11:21 */
	g_pwr_context[0].SOSCallFlagTOAvoidHistorySave=0;
	g_pwr_context[0].PrevScreenIndicator = ENTER_SIM_INSERTION;
	g_idle_context.IsOnSimErrorScreen = 1;
        // Fix the case that caller group LED could not demo after entering FM/EM and then back to INSERT_SIM

#if ( defined(__MMI_ENGINEER_MODE__) || defined(__MMI_FACTORY_MODE__) )
        gCurrentMode =NORMAL_MODE;
#endif

	#ifdef __J2ME__
		j2me_enter_idle_screen_notify();
	#endif

	 
	if(!gKeyPadLockFlag)
	{
		SETTING_AKPL_TIME GetAutoLockTimeOut=GetAutoKeypadLockTime();
		switch(GetAutoLockTimeOut)
		{
			case TIME_0:
			g_idle_context.AutoLockTimeOut=0;
			break;
			case TIME_5:
			g_idle_context.AutoLockTimeOut=5000;
			break;
			case TIME_30:
			g_idle_context.AutoLockTimeOut=30000;
			break;
			case TIME_60:
			g_idle_context.AutoLockTimeOut=60000;
			break;
			case TIME_2:
			g_idle_context.AutoLockTimeOut=300000;
			break;
		}
		
		if(g_idle_context.AutoLockTimeOut!=0)
			StartTimer(IDLE_APP_AUTO_LOCK_TIMER_ID, g_idle_context.AutoLockTimeOut, InsertSimSetKeyPadLockFlag);
		#ifdef __MMI_TOUCH_SCREEN__
		mmi_wgui_register_status_icon_pen_event_hdlr(STATUS_ICON_KEYPAD_LOCK, WGUI_STATUS_ICON_BAR_PEN_LONG_PRESS_ICON,InsertSimSetKeyPadLockFlag);
		#endif
	}

	mmi_frm_kbd_reg_pre_key_hdlr(IdleScreenEmergencyCallCheckNKeypadLock);
	 

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_SIM_INSERTION);	

	ShowSubLCDScreen(ShowSubLCDSimError);
	DinitSubLCDHistory();
#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && (defined __MMI_TOUCH_SCREEN__) )
#ifdef __FLIGHT_MODE_SUPPORT__
	if (mmi_bootup_get_active_flight_mode() != 1)
#endif
		EnableCenterSoftkey(0, IMG_CSK_DIAL_ICON);
#endif

#ifdef __J2ME__			
	if(java_is_launch_after_install())
	{
		mmi_java_send_mid_start_req();
	}
	else
#endif				
	{
	
        msgString = (simErrorDisplayString == SIM_ERROR_DISPLAY_SIM_ACCESS_ERROR) ? STR_SIM_ACCESS_ERROR_MSG : STR_SIM_INSERTION_MSG;

		if(gKeyPadLockFlag == 0) /* keypad is not locked */
		{
			ShowCategory123Screen ( 0,0,
						  STR_GLOBAL_OPTIONS, 
						  IMG_GLOBAL_OPTIONS,
						  STR_RSK_SOS, 
						  0,
						  msgString,
						  IMG_GLOBAL_WARNING,NULL);
#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && (defined __MMI_TOUCH_SCREEN__) )
			SetCenterSoftkeyFunction(HandleInsertSIMScrDigitEntry, KEY_EVENT_UP);
#endif	
			g_idle_context.LongPressEnable = 0;
			SetLeftSoftkeyFunction(EntryScrSimOptionMenu, KEY_EVENT_UP);
			SetRightSoftkeyFunction(ShowMakeSOSCallScr, KEY_EVENT_UP);
			SetGroupKeyHandler(HandleInsertSIMScrDigitEntry,InsertSIMScreenDigits,MAX_INSERT_SCR_DIGITS-1,KEY_EVENT_DOWN);
		}
		else
		{
			U16 KeyLockLSKStringID=0, KeyLockRSKStringID=0, KeyLockLSKIconID=0, KeyLockRSKIconID=0;

			#ifdef __MMI_TOUCH_SCREEN__
				#if 0 //modified by zhoumn for keypad lock@2007/07/20
				KeyLockRSKStringID = SCREEN_LOCKED_TEXT;
				#else
				KeyLockRSKStringID = KEYPAD_LOCKED_RSK_TEXT;
				#endif
				KeyLockRSKIconID = 0;
			#else
				#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__
					KeyLockRSKStringID = KEYPAD_LOCKED_RSK_TEXT;
					KeyLockRSKIconID = 0;
				#endif
					
				#ifdef __MMI_KEYPAD_LOCK_PATTERN_2__
					KeyLockLSKStringID= KEYPAD_LOCKED_RSK_TEXT;
					KeyLockLSKIconID = 0;
				#endif
			#endif

			#if (defined  __MMI_TOUCH_SCREEN__ && defined __MMI_WGUI_CSK_ENABLE__ )
				ResetCenterSoftkey();
			#endif
			ShowCategory123Screen( 	0, 0,
					KeyLockLSKStringID, KeyLockLSKIconID, KeyLockRSKStringID, KeyLockRSKIconID,
					msgString, IMG_GLOBAL_WARNING,NULL);
			
			#if 0 //键盘锁时，按任意键都不响应 //deleted by zhoumn@2007/07/20
				#if defined(__MMI_KEYPAD_LOCK_PROMPT_TEXT__)
					SetGroupKeyHandler(EntryScrKeyPadLockOnPessingAnyKey,(PU16)PresentAllKeys,(U8)TOTAL_KEYS,(U16)KEY_EVENT_DOWN);
				#endif
			#endif
		
			#ifdef __MMI_TOUCH_SCREEN__
				mmi_pen_block();  

				#if 0 //modified by zhoumn for keypad lock@2007/07/20
				SetKeyHandler(EntryScrAfterPessingRSK, KEY_SEND, KEY_EVENT_DOWN); 
				#else
				SetKeyHandler(EntryScrAfterPessingRSK, KEY_RSK, KEY_EVENT_DOWN); 
				#endif

			#else /* __MMI_TOUCH_SCREEN__ */
				#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__  
					SetRightSoftkeyFunction(EntryScrAfterPessingRSK,KEY_EVENT_UP);
				#endif
			
				#ifdef __MMI_KEYPAD_LOCK_PATTERN_2__  
					SetLeftSoftkeyFunction(EntryScrAfterPessingRSK,KEY_EVENT_UP);
				#endif
			#endif

			if(g_idle_context.IsRSKPressOnScreenSaver)
			{
				EntryScrAfterPessingRSK();
				g_idle_context.IsRSKPressOnScreenSaver=0;
			}
		}

#ifdef __MMI_USB_SUPPORT__
		PendingUsbDetectionHdlr();
#endif
		if (g_charbat_context.isChargerConnected == 1)
			IdleScreenChargerConnected(KAL_FALSE);
	}
	
	 
	PendingAlarmReminder();
#ifdef __MMI_TOUCH_SCREEN__
    mmi_pen_enable_quick_launch( );
#endif

#if defined __MMI_OPP_SUPPORT__ 
	if ( g_NeedRecheckOppRecv && MMI_OPP_CTX(cur_recv_file_cnt) > 0)
	{
	     	mmi_opp_scr_recv_obj_ind();
		g_NeedRecheckOppRecv = 0;
       }
#endif

}

/**************************************************************

	FUNCTION NAME		: ExitScrnSimErrorScreen()

  	PURPOSE				: Exit screen when no SIM is inserted

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScrnSimErrorScreen()
{
	g_idle_context.IsOnSimErrorScreen = 0;
#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__) )
	ResetCenterSoftkey();
#endif
#if ( !(defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
  ClearSimulateCenterKeyDailhandlers();
#endif
#ifdef __MMI_TOUCH_SCREEN__
	if(gKeyPadLockFlag == 1)
		mmi_pen_unblock();  

#endif

	StopTimer(IDLE_APP_AUTO_LOCK_TIMER_ID);
}

/**************************************************************

	FUNCTION NAME		: ShowSubLCDSimError()

  	PURPOSE				: entry screen for sublcd when no SIM is inserted

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ShowSubLCDSimError(void)
{

        U16 stringID = (simErrorDisplayString == SIM_ERROR_DISPLAY_SIM_ACCESS_ERROR) ? STR_SUBLCD_SIM_ACCESS_ERROR_MSG : STR_SUBLCD_SIM_INSERTION_MSG;

	ExecSubLCDCurrExitHandler();
	#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
		if ( IsClamClose() )
		ShowSubLCDAnalogClockScreen();
		else
			ShowSubLCDLogoString();
	#else
	ShowCategory301Screen((PU8)GetString(stringID),0,NULL);
	#endif
	SetSubLCDExitHandler(ExitSubLCDSimError);
	g_idle_context.IsSubLcdOnIdleScreen = SUBLCD_ON_IDLE_INSERT_SIM;
}

/**************************************************************

	FUNCTION NAME		: ExitSubLCDSimError()

  	PURPOSE				: exit fn for sublcd when no SIM is inserted

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSubLCDSimError(void)
{
	SubLCDHistoryNode SubLCDHistory;
	SubLCDHistory.entryFuncPtr=ShowSubLCDSimError;
	AddSubLCDHistory(&SubLCDHistory);
	g_idle_context.IsSubLcdOnIdleScreen = SUBLCD_NOT_ON_IDLE;
}

/**************************************************************

	FUNCTION NAME		: EntryScrSimOptionMenu()

  	PURPOSE				: Entry fn for option button on insert sim/ blocked sim screen 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern U16 gMainMenuTitleIcon[];
extern void main_menu_custom_highlight_handler();

void EntryScrSimOptionMenu()
{

/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_220X176__) || defined(__MMI_MAINLCD_160X128__) || defined( __MMI_BLACK_BERRY_MAIN_MENU__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
    EntryMainMenuFromIdleScreen();	
#else
	U8* guiBuffer;       /* Buffer holding history data */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	UI_image_ID_type mm_nosim_iconIDs[MAX_MAIN_MENU_ITEMS];
#if !defined __MMI_MAINLCD_220X176__
	U8 i = 0;
#endif
	U16 title_str = 0;

#ifndef __MMI_TOUCH_SCREEN__
#ifdef	__MMI_KEYPAD_LOCK_PATTERN_2__ 
	g_idle_context.ToMainMenuScrFromIdleApp = 1;  
#endif
#endif

        EntryNewScreen(SCR_SIM_OPTION, NULL, EntryScrSimOptionMenu, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_SIM_OPTION);	

	SetParentHandler(MENU_SIM_OPTION_ID);

	nNumofItem = GetNumOfChild(MENU_SIM_OPTION_ID);

	nDispAttribute = GetDispAttributeOfItem(MENU_SIM_OPTION_ID);

	GetSequenceStringIds(MENU_SIM_OPTION_ID,nStrItemList);
	GetSequenceImageIds(MENU_SIM_OPTION_ID,mm_nosim_iconIDs);
	
	for( i=0;  i<nNumofItem; i++)
	{
		mm_nosim_iconIDs[i]   =  MAIN_MENU_TITLE_ICON_BASE+( mm_nosim_iconIDs[i] -MAIN_MENU_ICON_IMGBASE  ) ;
		//gMainMenuTitleIcon[i] =mm_nosim_iconIDs[i] ;
	}
	
	RegisterHighlightHandler(main_menu_custom_highlight_handler);
#ifdef __MMI_MULTI_SIM__
	title_str = IDLE_SCREEN_LSK_TEXT;
#else
	title_str = STR_SIM_OPTION_CAPTION;	
#endif
	ShowCategory15Screen( title_str,0,
						  STR_GLOBAL_OK,IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK ,IMG_GLOBAL_BACK ,
						  nNumofItem, nStrItemList, mm_nosim_iconIDs,
						  LIST_MENU, 0, guiBuffer);

	
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

 
#ifndef __MMI_TOUCH_SCREEN__
#if (defined(__MMI_KEYPAD_LOCK_PATTERN_2__) && !defined(__MMI_DISABLE_KEYPAD_LOCK__))
/*zhangm Add Start For 10821 Ver: GreenStone  on 2008-12-19 16:23 */
//	if(g_idle_context.ToMainMenuScrFromIdleApp == 1)
//	{
//		StartTimer(KEYPAD_LOCK_TIMER, KEYPAD_LOCK_TIMEOUT, IdleHandleKeypadLockProcess);
//		SetKeyHandler(IdleHandlePoundKeyForKeypadLock,KEY_POUND,KEY_EVENT_UP);
//	}
	SetKeyHandler(IdleHandleStarKeyForKeypadLock,KEY_STAR,KEY_LONG_PRESS);
/*zhangm Add end For 10821 Ver: GreenStone  on 2008-12-19 16:23 */
#endif
#endif

#endif
}

/**************************************************************

	FUNCTION NAME		: ExitScrnSimBlockRsp()

  	PURPOSE				: Exit screen when no SIM is blocked

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScrnSimBlockRsp(void)
{
	g_idle_context.IsOnSimErrorScreen = 0;
#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__) )
	ResetCenterSoftkey();
#endif
#if ( !(defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
  ClearSimulateCenterKeyDailhandlers();
#endif
#ifdef __MMI_TOUCH_SCREEN__
	if(gKeyPadLockFlag == 1)
		mmi_pen_unblock();  

#endif

	StopTimer(IDLE_APP_AUTO_LOCK_TIMER_ID);
}



/**************************************************************

	FUNCTION NAME		: EntryScrnSimBlockRsp()

  	PURPOSE				: entry fn when blocked sim is detected

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrnSimBlockRsp(void)
{
	U8* guiBuffer;       /* Buffer holding history data */
	U16 BlockSIMScreenDigits[MAX_INSERT_SCR_DIGITS] = {KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_STAR,KEY_POUND};

	EntryNewScreen(SCR_SIM_BLOCK, ExitScrnSimBlockRsp, EntryScrnSimBlockRsp, NULL);
//Dinit History is called to prevent memory leak.
	DinitHistory();
	DeInitSpecialMode(); /* Back from engineer mode or factory mode */

	g_pwr_context[0].SOSCallFlagTOAvoidHistorySave=0;
	g_pwr_context[0].PrevScreenIndicator = ENTER_SIM_BLOCK;
	g_idle_context.IsOnSimErrorScreen = 1;
        // Fix the case that caller group LED could not demo after entering FM/EM and then back to INSERT_SIM

#if ( defined(__MMI_ENGINEER_MODE__) || defined(__MMI_FACTORY_MODE__) )
        gCurrentMode =NORMAL_MODE;
#endif

	#ifdef __J2ME__
		j2me_enter_idle_screen_notify();
	#endif

	 
	if(!gKeyPadLockFlag)
	{
		SETTING_AKPL_TIME GetAutoLockTimeOut=GetAutoKeypadLockTime();
		switch(GetAutoLockTimeOut)
		{
			case TIME_0:
			g_idle_context.AutoLockTimeOut=0;
			break;
			case TIME_5:
			g_idle_context.AutoLockTimeOut=5000;
			break;
			case TIME_30:
			g_idle_context.AutoLockTimeOut=30000;
			break;
			case TIME_60:
			g_idle_context.AutoLockTimeOut=60000;
			break;
			case TIME_2:
			g_idle_context.AutoLockTimeOut=300000;
			break;
		}
		
		if(g_idle_context.AutoLockTimeOut!=0)
			StartTimer(IDLE_APP_AUTO_LOCK_TIMER_ID, g_idle_context.AutoLockTimeOut, InsertSimSetKeyPadLockFlag);
	}

	mmi_frm_kbd_reg_pre_key_hdlr(IdleScreenEmergencyCallCheckNKeypadLock);
	 

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_SIM_BLOCK);	

	ShowSubLCDScreen(ShowSubLCDBlockedSIM);
	DinitSubLCDHistory();
#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && (defined __MMI_TOUCH_SCREEN__) )
#ifdef __FLIGHT_MODE_SUPPORT__
	if (mmi_bootup_get_active_flight_mode() != 1)
#endif
		EnableCenterSoftkey(0, IMG_CSK_DIAL_ICON);
#endif
  #if ((!defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
		DrawSimulateCenterKey();	  
  #endif
#ifdef __J2ME__			
	if(java_is_launch_after_install())
	{
		mmi_java_send_mid_start_req();
	}
	else
#endif				
	{
		if(gKeyPadLockFlag == 0) /* keypad is not locked */
		{
			ShowCategory123Screen (0,0, STR_GLOBAL_OPTIONS, 
						  IMG_GLOBAL_OPTIONS,
						  STR_RSK_SOS, 
						  0,
						  STR_SIM_BLOCK_MSG,IMG_GLOBAL_WARNING,NULL);
	
#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && (defined __MMI_TOUCH_SCREEN__) )
			SetCenterSoftkeyFunction(HandleInsertSIMScrDigitEntry, KEY_EVENT_UP);
#endif	
#if ( !(defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
    SimulateCenterKeyDailFunction(HandleInsertSIMScrDigitEntry, KEY_EVENT_UP);
#endif
			SetLeftSoftkeyFunction(EntryScrSimOptionMenu, KEY_EVENT_UP);
			SetRightSoftkeyFunction(ShowMakeSOSCallScr, KEY_EVENT_UP);
			SetGroupKeyHandler(HandleInsertSIMScrDigitEntry,BlockSIMScreenDigits,MAX_INSERT_SCR_DIGITS-1,KEY_EVENT_DOWN);
		}
		else
		{
			U16 KeyLockLSKStringID=0, KeyLockRSKStringID=0, KeyLockLSKIconID=0, KeyLockRSKIconID=0;

			#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__
				KeyLockRSKStringID = KEYPAD_LOCKED_RSK_TEXT;
				KeyLockRSKIconID = 0;
			#endif
				
			#ifdef __MMI_KEYPAD_LOCK_PATTERN_2__
				KeyLockLSKStringID= KEYPAD_LOCKED_RSK_TEXT;
				KeyLockLSKIconID = 0;
			#endif

			ShowCategory123Screen (0,0,
						  KeyLockLSKStringID, KeyLockLSKIconID, KeyLockRSKStringID, KeyLockRSKIconID,
						  STR_SIM_BLOCK_MSG,IMG_GLOBAL_WARNING,NULL);

		#if 0 //键盘锁时，按任意键都不响应 //deleted by zhoumn@2007/07/20
			#if defined(__MMI_KEYPAD_LOCK_PROMPT_TEXT__)
				SetGroupKeyHandler(EntryScrKeyPadLockOnPessingAnyKey,(PU16)PresentAllKeys,(U8)TOTAL_KEYS,(U16)KEY_EVENT_DOWN);
			#endif
		#endif
		
			#ifdef __MMI_TOUCH_SCREEN__
				mmi_pen_block();  

				#if 0 //modified by zhoumn for keypad lock@2007/07/20
				SetKeyHandler(EntryScrAfterPessingRSK, KEY_SEND, KEY_EVENT_DOWN);
				#else
				SetKeyHandler(EntryScrAfterPessingRSK, KEY_RSK, KEY_EVENT_DOWN);
				#endif
				
			#else /* __MMI_TOUCH_SCREEN__ */
				#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__  
					SetRightSoftkeyFunction(EntryScrAfterPessingRSK,KEY_EVENT_UP);
				#endif
			
				#ifdef __MMI_KEYPAD_LOCK_PATTERN_2__  
					SetLeftSoftkeyFunction(EntryScrAfterPessingRSK,KEY_EVENT_UP);
				#endif
			#endif

			if(g_idle_context.IsRSKPressOnScreenSaver)
			{
				EntryScrAfterPessingRSK();
				g_idle_context.IsRSKPressOnScreenSaver=0;
			}
		}
#ifdef __MMI_USB_SUPPORT__
		PendingUsbDetectionHdlr();
	#endif	
		if( g_charbat_context.isChargerConnected == 1 )
			IdleScreenChargerConnected(KAL_FALSE);
	}
}

/**************************************************************

	FUNCTION NAME		: ShowSubLCDBlockedSIM()

  	PURPOSE				: entry fn for sulcd when blocked sim is detected

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ShowSubLCDBlockedSIM(void)
{
	ExecSubLCDCurrExitHandler();
	#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
		if ( IsClamClose() )
		ShowSubLCDAnalogClockScreen();
		else
			ShowSubLCDLogoString();
	#else
	ShowCategory301Screen((PU8)GetString(STR_SIM_BLOCK_MSG),0,NULL);
	#endif
	SetSubLCDExitHandler(ExitSubLCDBlockedSIM);
	g_idle_context.IsSubLcdOnIdleScreen = SUBLCD_ON_IDLE_SIM_BLOCKED;
}

/**************************************************************

	FUNCTION NAME		: ExitSubLCDBlockedSIM()

  	PURPOSE				: exit fn for sublcd when blocked sim is detected

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSubLCDBlockedSIM(void)
{
	SubLCDHistoryNode SubLCDHistory;
	SubLCDHistory.entryFuncPtr=ShowSubLCDBlockedSIM;
	AddSubLCDHistory(&SubLCDHistory);
	g_idle_context.IsSubLcdOnIdleScreen = SUBLCD_NOT_ON_IDLE;
}

/**************************************************************

	FUNCTION NAME		: EntryScrEnterPIN()

  	PURPOSE				: Function inputs PIN from the user 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U8 PinEnterSimID;
void EntryScrEnterPIN(void)
{
	U8 PassCountdownDisp[MAX_PASS_COUNTDOWN_DISP];
	U16 msg_str = STR_ENTER_PIN_MSG;
	
	
	EntryNewScreen(SCR_ENTER_PIN, NULL, EntryScrEnterPIN, NULL);
 	DinitHistory();
	
	mmi_trace(g_sw_SMU,"MMI_SIM: EntryScrEnterPIN");
	
	if (!IS_IDLE_ARRIVED)
		g_pwr_context[0].PowerOnPeriod = 0;
	
	g_pwr_context[0].PrevScreenIndicator = ENTER_PIN_SCR;

// If user is entring this screen after SOS call then memset the buffer
	if(g_pwr_context[0].SOSCallFlagTOAvoidHistorySave)
		memset((S8*)gInputPwdBuffer,0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);

//micha0606
	RegisterInputBoxValidationFunction(RstrSettingPasswdScrSoftKeyFunc);
	g_pwr_context[0].SOSCallFlagTOAvoidHistorySave=0;
//micha1126
	FillCountDownString(MMI_SETTING_TYPE_CHV1, PassCountdownDisp,PinEnterSimID);
	#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	msg_str = STR_ENTER_CARD1_PIN_MSG + PinEnterSimID;
	#endif
	ShowCategory111Screen(	STR_GLOBAL_OK , 
							IMG_GLOBAL_OK ,
							STR_RSK_SOS, 
							0,
							msg_str,
							INPUT_TYPE_NUMERIC_PASSWORD,
							(U8*)gInputPwdBuffer,
//micha0420
							MAX_PWD_LENGTH_MMI , PassCountdownDisp, NULL);

	ShowSubLCDScreen(ShowSubLCDEnterPIN);
	DinitSubLCDHistory();

	TRACE_SYS_FUNCTION();
	SetLeftSoftkeyFunction(ValidatePIN, KEY_EVENT_UP);
	SetCategory111RightSoftkeyFunction(ShowMakeSOSCallScr,KEY_EVENT_UP);

	SetKeyHandler(GoToSOSDialScr,KEY_SEND,KEY_EVENT_UP);
#ifdef __SENDKEY2_SUPPORT__
	SetKeyHandler(GoToSOSDialScr,KEY_SEND1, KEY_EVENT_DOWN);
	SetKeyHandler(GoToSOSDialScr,KEY_SEND2, KEY_EVENT_DOWN);
#endif

     
    SetKeyHandler(ValidatePIN,KEY_POUND,KEY_EVENT_UP);
    //CSD end
#if TGT_GALLITE_G800
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_UP);
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_DOWN);
#endif
//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = ValidatePIN;
	if(pfnUnicodeStrlen((S8*)gInputPwdBuffer)<MMI_MIN_PASSWORD_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
}

/**************************************************************

	FUNCTION NAME		: ShowSubLCDEnterPIN()

  	PURPOSE				: entry fn for sublcd for enter pin 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ShowSubLCDEnterPIN(void)
{
	ExecSubLCDCurrExitHandler();
	#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
		if ( IsClamClose() )
		ShowSubLCDAnalogClockScreen();
		else
			ShowSubLCDLogoString();
	#else
	ShowCategory301Screen((PU8)GetString(STR_SUBLCD_ENTER_PIN_MSG),0,NULL);
	#endif
	SetSubLCDExitHandler(ExitSubLCDEnterPIN);
}

/**************************************************************

	FUNCTION NAME		: ExitSubLCDEnterPIN()

  	PURPOSE				: exit fn for sublcd for enter pin screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSubLCDEnterPIN(void)
{
	SubLCDHistoryNode SubLCDHistory;
	SubLCDHistory.entryFuncPtr=ShowSubLCDEnterPIN;
	AddSubLCDHistory(&SubLCDHistory);
}

/*************************************************************

	FUNCTION NAME		: EntryScrEnterPhoneLock

  	PURPOSE				: Entry Scr for entring phone lock

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrEnterPhoneLock(void)
{
	EntryNewScreen(SCR_ENTER_PHONELOCK, NULL, EntryScrEnterPhoneLock, NULL);
	//Dinit History is called to prevent memory leak.
 	DinitHistory();
	g_pwr_context[0].PrevScreenIndicator = ENTER_PHONELOCK;
	g_pwr_context[0].PhoneLockProcesing = TRUE;/*wangrui add 2008.11.27 for fixbug10388  */

	if (!IS_IDLE_ARRIVED)
		g_pwr_context[0].PowerOnPeriod = 0;

	// If user is entring this screen after SOS call then memset the buffer
	if (g_pwr_context[0].SOSCallFlagTOAvoidHistorySave)
		memset((S8 *)gInputPwdBuffer, 0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
	
	g_pwr_context[0].SOSCallFlagTOAvoidHistorySave = 0;

	//micha0606
	RegisterInputBoxValidationFunction(RstrSettingPasswdScrSoftKeyFunc);

	ShowCategory111Screen(
						STR_GLOBAL_OK, 
						IMG_GLOBAL_OK,
						STR_RSK_SOS, /*SOS call */
						0, STR_ENTER_PHONELOCK_MSG,
						INPUT_TYPE_NUMERIC_PASSWORD,
						(U8*)gInputPwdBuffer,
						MAX_PWD_LENGTH_MMI, NULL, NULL);

	ShowSubLCDScreen(ShowSubLCDEnterPhoneLock);
	DinitSubLCDHistory();

	SetLeftSoftkeyFunction(ValidatePhoneLock, KEY_EVENT_UP);

	SetCategory111RightSoftkeyFunction(ShowMakeSOSCallScr, KEY_EVENT_UP);	
	SetKeyHandler(GoToSOSDialScr, KEY_SEND, KEY_EVENT_UP);
#ifdef __SENDKEY2_SUPPORT__
	SetKeyHandler(GoToSOSDialScr,KEY_SEND1, KEY_EVENT_DOWN);
	SetKeyHandler(GoToSOSDialScr,KEY_SEND2, KEY_EVENT_DOWN);
#endif

    SetKeyHandler(ValidatePhoneLock, KEY_POUND, KEY_EVENT_UP);
#if TGT_GALLITE_G800
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_UP);
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_DOWN);
#endif
//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = ValidatePhoneLock;
	if (pfnUnicodeStrlen((S8 *)gInputPwdBuffer) < MMI_MIN_PASSWORD_LENGTH) 
	{
		ChangeLeftSoftkey(0, 0);
		ClearKeyHandler(KEY_POUND, KEY_EVENT_UP);
	}
	 
}
/*************************************************************

	FUNCTION NAME		: ShowSubLCDEnterPhoneLock

  	PURPOSE				: Entry Scr of sublcd for entring phone lock

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ShowSubLCDEnterPhoneLock(void)
{
	ExecSubLCDCurrExitHandler();
	#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
		if ( IsClamClose() )
		ShowSubLCDAnalogClockScreen();
		else
			ShowSubLCDLogoString();
	#else
	ShowCategory301Screen((PU8)GetString(STR_SUBLCD_ENTER_PHONELOCK_MSG),0,NULL);
	#endif
	SetSubLCDExitHandler(ExitSubLCDEnterPhoneLock);
}
/*************************************************************

	FUNCTION NAME		: ExitSubLCDEnterPhoneLock

  	PURPOSE				: Exit fn of sublcd for enter phone lock screen 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSubLCDEnterPhoneLock(void)
{
	SubLCDHistoryNode SubLCDHistory;
	SubLCDHistory.entryFuncPtr=ShowSubLCDEnterPhoneLock;
	AddSubLCDHistory(&SubLCDHistory);
}

/**************************************************************

	FUNCTION NAME		: EntryScrEnterPUK()

  	PURPOSE				: Function inputs PUK from the user 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U8 PUKEnterSimID = 0;
void EntryScrEnterPUK(void)
{
	U8 PassCountdownDisp[MAX_PASS_COUNTDOWN_DISP];
	U16 msg_str = STR_ENTER_PUK_MSG;
	EntryNewScreen(SCR_ENTER_PUK, NULL, EntryScrEnterPUK, NULL);
//Dinit History is called to prevent memory leak.
 	DinitHistory();
	g_pwr_context[0].PrevScreenIndicator = ENTER_PUK_SCR;

	if (!IS_IDLE_ARRIVED)
		g_pwr_context[0].PowerOnPeriod = 0;

// If user is entring this screen after SOS call then memset the buffer
	if(g_pwr_context[0].SOSCallFlagTOAvoidHistorySave)
		memset((S8*)gInputPwdBuffer,0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);

	g_pwr_context[0].SOSCallFlagTOAvoidHistorySave=0;
//micha0606
	RegisterInputBoxValidationFunction(ScrSettingPukValidation);
//micha1126	
	FillCountDownString(MMI_SETTING_TYPE_UBCHV1, PassCountdownDisp,PUKEnterSimID);
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	msg_str = STR_ENTER_SIM1_PUK_MSG + PUKEnterSimID;
#endif
	ShowCategory111Screen(STR_GLOBAL_OK,
						  IMG_GLOBAL_OK,
						  STR_RSK_SOS,
						  0,
						  msg_str,
						  INPUT_TYPE_NUMERIC_PASSWORD,
						  (U8*)gInputPwdBuffer,
						  MAX_PWD_LENGTH_MMI , PassCountdownDisp, NULL);
	ShowSubLCDScreen(ShowSubLCDEnterPUK);
	DinitSubLCDHistory();


	SetLeftSoftkeyFunction(EntryScrEnterNewPINFirstTime, KEY_EVENT_UP);

	SetKeyHandler(GoToSOSDialScr,KEY_SEND,KEY_EVENT_UP);
#ifdef __SENDKEY2_SUPPORT__
	SetKeyHandler(GoToSOSDialScr,KEY_SEND1, KEY_EVENT_DOWN);
	SetKeyHandler(GoToSOSDialScr,KEY_SEND2, KEY_EVENT_DOWN);
#endif

    SetKeyHandler(EntryScrEnterNewPINFirstTime,KEY_POUND,KEY_EVENT_UP);
    //SetKeyHandler(SSCHandleEnterPUK,KEY_STAR,KEY_EVENT_DOWN);//man
	SetCategory111RightSoftkeyFunction(ShowMakeSOSCallScr, KEY_EVENT_UP);

//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = EntryScrEnterNewPINFirstTime;
	if(pfnUnicodeStrlen((S8*)gInputPwdBuffer)<MMI_MIN_PUK_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
}

/**************************************************************

	FUNCTION NAME		: ShowSubLCDEnterPUK()

  	PURPOSE				: entry fn for sublcd screen enter puk
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ShowSubLCDEnterPUK(void)
{
	ExecSubLCDCurrExitHandler();
	#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
		if ( IsClamClose() )
		ShowSubLCDAnalogClockScreen();
		else
			ShowSubLCDLogoString();
	#else
	ShowCategory301Screen((PU8)GetString(STR_SUBLCD_ENTER_PUK_MSG),0,NULL);
	#endif
	SetSubLCDExitHandler(ExitSubLCDEnterPUK);
}

/**************************************************************

	FUNCTION NAME		: ExitSubLCDEnterPUK()

  	PURPOSE				: exit fn for sublcd screen enter puk

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSubLCDEnterPUK(void)
{
	SubLCDHistoryNode SubLCDHistory;
	SubLCDHistory.entryFuncPtr=ShowSubLCDEnterPUK;
	AddSubLCDHistory(&SubLCDHistory);
}

/**************************************************************



	FUNCTION NAME		: EntryScrEnterNewPINFirstTime

  	PURPOSE				: Function inputs new PIN first time from the user 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  correctly 	

**************************************************************/
void EntryScrEnterNewPINFirstTime(void)
{
// If user is entring this screen after SOS call then memset the buffer

	if (!IS_IDLE_ARRIVED)
		g_pwr_context[0].PowerOnPeriod = 0;

	if(g_pwr_context[0].SOSCallFlagTOAvoidHistorySave)
		memset((S8*)gInputNewPinBufferFirstTime,0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
	else if(pfnUnicodeStrlen((S8*)gInputPwdBuffer) < MMI_MIN_PUK_LENGTH)
	{
		if(pfnUnicodeStrlen((S8*)gInputPwdBuffer) <=0)
		{
			return;
		}
		else
		{
			memset((S8*)gInputPwdBuffer,0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
			EntryScrInvalidPINPUKPLOCKLength();
			return;
		}
	}

	EntryNewScreen(SCR_ENTER_NEW_PIN, NULL, EntryScrEnterNewPINFirstTime, NULL);
	g_pwr_context[0].PrevScreenIndicator = ENTER_NEW_PIN;
	g_pwr_context[0].SOSCallFlagTOAvoidHistorySave=0;
//micha0606
	RegisterInputBoxValidationFunction(RstrSettingPasswdScrSoftKeyFunc);

	ShowCategory111Screen(	STR_GLOBAL_OK, /*OK*/
							IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, 
							IMG_GLOBAL_BACK,
							STR_ENTER_NEW_PIN_MSG,
							INPUT_TYPE_NUMERIC_PASSWORD,
							(U8*)gInputNewPinBufferFirstTime,
							MAX_PWD_LENGTH_MMI ,NULL, NULL);

	SetLeftSoftkeyFunction(EntryScrEnterNewPINSecondTime, KEY_EVENT_UP);

	SetKeyHandler(GoToSOSDialScr,KEY_SEND,KEY_EVENT_UP);
#ifdef __SENDKEY2_SUPPORT__
	SetKeyHandler(GoToSOSDialScr,KEY_SEND1, KEY_EVENT_DOWN);
	SetKeyHandler(GoToSOSDialScr,KEY_SEND2, KEY_EVENT_DOWN);
#endif

	SetCategory111RightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(EntryScrEnterNewPINSecondTime,KEY_POUND,KEY_EVENT_UP);
#if TGT_GALLITE_G800
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_UP);
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_DOWN);
#endif
//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = EntryScrEnterNewPINSecondTime;
	if(pfnUnicodeStrlen((S8*)gInputNewPinBufferFirstTime)<MMI_MIN_PASSWORD_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
	 
}

/**************************************************************

	FUNCTION NAME		: EntryScrEnterNewPINSecondTime

  	PURPOSE				: Function confirms new PIN from user 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  correctly and new pin is enerd first time
						  and second time pin is asked 

**************************************************************/
void EntryScrEnterNewPINSecondTime(void)
{
	if(g_pwr_context[0].SOSCallFlagTOAvoidHistorySave)
		memset((S8*)gInputNewPinBufferSecondTime,0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
	else if(pfnUnicodeStrlen((S8*)gInputNewPinBufferFirstTime) < MMI_MIN_PASSWORD_LENGTH) 
	{
		if(pfnUnicodeStrlen((S8*)gInputNewPinBufferFirstTime) <=0)
		{
			return;
		}
		else
		{
			memset((S8*)gInputNewPinBufferFirstTime,0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
			EntryScrInvalidPINPUKPLOCKLength();
			return;
		}
	}
	EntryNewScreen(SCR_ENTER_CONFIRM_PIN, NULL, EntryScrEnterNewPINSecondTime, NULL);
 	g_pwr_context[0].SOSCallFlagTOAvoidHistorySave=0;
          
	g_pwr_context[0].PrevScreenIndicator = ENTER_CONFIRM_PIN;

//micha0606
	RegisterInputBoxValidationFunction(RstrSettingPasswdScrSoftKeyFunc);

	ShowCategory111Screen(	STR_GLOBAL_OK, /*OK*/
							IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, 
							IMG_GLOBAL_BACK,
							STR_ENTER_PIN_AGAIN_MSG,
							INPUT_TYPE_NUMERIC_PASSWORD,
							(U8*)gInputNewPinBufferSecondTime,
							MAX_PWD_LENGTH_MMI ,NULL, NULL);

	SetLeftSoftkeyFunction(ValidateFirstAndSecondPin, KEY_EVENT_UP);

	SetKeyHandler(GoToSOSDialScr,KEY_SEND,KEY_EVENT_UP);
#ifdef __SENDKEY2_SUPPORT__
	SetKeyHandler(GoToSOSDialScr,KEY_SEND1, KEY_EVENT_DOWN);
	SetKeyHandler(GoToSOSDialScr,KEY_SEND2, KEY_EVENT_DOWN);
#endif

    SetKeyHandler(ValidateFirstAndSecondPin,KEY_POUND,KEY_EVENT_UP);
	SetCategory111RightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
#if TGT_GALLITE_G800
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_UP);
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_DOWN);
#endif
//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = ValidateFirstAndSecondPin;
	if(pfnUnicodeStrlen((S8*)gInputNewPinBufferSecondTime)<MMI_MIN_PASSWORD_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
	
}

/**************************************************************

	FUNCTION NAME		: EntryScrIncorrectPin

  	PURPOSE				: Flash Message Incorrect pin try again 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						 	
**************************************************************/
void EntryScrIncorrectPin(void)
{
	mmi_trace(g_sw_SMU, "MMI_SIM: EntryScrIncorrectPin");
	DisplayPopup((U8*)GetString(STR_INCORRECT_PIN),IMG_GLOBAL_ERROR,0,2000,(U8)ERROR_TONE);
}

/**************************************************************

	FUNCTION NAME		: EntryScrIncorrectPhoneLock

  	PURPOSE				: Flash Message Incorrect phonelock try again 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						 	
**************************************************************/
void EntryScrIncorrectPhoneLock(void)
{
	DisplayPopup((U8*)GetString(STR_INCORRECT_PHONELOCK),IMG_GLOBAL_ERROR,0,2000,(U8)ERROR_TONE);
}

	
/**************************************************************

	FUNCTION NAME		: EntryScrPinDoNotMatch

  	PURPOSE				: Flash Message 'pin do not match'  

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrPinDoNotMatch(void)
{
	memset((S8*)gInputNewPinBufferSecondTime,0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
	memset((S8*)gInputNewPinBufferFirstTime,0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
	DisplayPopup((U8*)GetString(STR_PIN_DONOT_MATCH),IMG_GLOBAL_ERROR,0,2000,(U8)ERROR_TONE);
	DeleteUptoScrID(SCR_ENTER_NEW_PIN);

}

/**************************************************************

	FUNCTION NAME		: EntryScrIncorrectPUK

  	PURPOSE				: Flash Message Incorrect PUK try again 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void EntryScrIncorrectPUK(void)
{
	DisplayPopup((U8*)GetString(STR_INCORRECT_PUK),IMG_GLOBAL_ERROR,0,2000,(U8)ERROR_TONE);
	DeleteUptoScrID(SCR_ENTER_PUK);
}

/**************************************************************

	FUNCTION NAME		: EntryScrPinBlocked

  	PURPOSE				: Flash Message Pin is blocked 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
							

**************************************************************/
void EntryScrPinBlocked(void)
{
	U16 msg_str_id = STR_PIN_BLOCKED_PUK;
	
	EntryNewScreen(SCR_PUK_BLOCKED, ExitScrPinBlocked, NULL, NULL);
	AddHistoryForBootUp(EntryScrEnterPUK);
	
	#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	msg_str_id = STR_SIM1_PIN_BLOCKED_PUK+ PUKEnterSimID;
	#endif

	ShowCategory123Screen(0,0,0,0,0,0,msg_str_id,IMG_GLOBAL_WARNING,NULL);
	
	playRequestedTone(WARNING_TONE);
	StartTimer(CODE_VERIFIED_TIMER,CODE_VERIFIED_TIMEOUT,EntryScrEnterPUK);
	SetGroupKeyHandler(EntryScrEnterPUK,(PU16)PresentAllKeys,(U8) TOTAL_KEYS,(U16)KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: ExitScrPinBlocked

  	PURPOSE				: exit fn for pin blocked screen 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
							

**************************************************************/
void ExitScrPinBlocked(void)
{
	stopRequestedTone(WARNING_TONE);
	StopTimer(CODE_VERIFIED_TIMER);
}
/**************************************************************

	FUNCTION NAME		: EntryScrInvalidPINPUKPLOCKLength

  	PURPOSE				: Entry Screen for when for error handling
						  when PIN , PUK and Phone Lock is incorrect	

	INPUT PARAMETERS	: void 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/

void EntryScrInvalidPINPUKPLOCKLength(void)
{
	DisplayPopup((U8*)GetString(STR_INVALID_PINPUKPLOCK_LENGTH_RUNNING_TEXT),IMG_GLOBAL_WARNING,0,2000,(U8)ERROR_TONE);
}

/**************************************************************

	FUNCTION NAME		: ValidatePIN()

  	PURPOSE				: Function to validate PIN 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void ValidatePIN(void)
{
  
	 
	MYQUEUE	Message; 
	MMISMUVERIFYPINREQ* local_data;
	TRACE_SYS_FUNCTION();

	if (!IS_IDLE_ARRIVED)
		g_pwr_context[0].PowerOnPeriod = 1;
	
	if(pfnUnicodeStrlen((S8*)gInputPwdBuffer) < MMI_MIN_PASSWORD_LENGTH)
	{
		if(pfnUnicodeStrlen((S8*)gInputPwdBuffer) <=0)
		{
			return;
		}
		else
		{
			memset((S8*)gInputPwdBuffer,0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
			EntryScrInvalidPINPUKPLOCKLength();
			return;
		}
	}
	ClearAllKeyHandler();
	PRINT_INFORMATION(("@@@@@ Sending request for verifying pin  @@@@@"));
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C + PinEnterSimID;
	Message.oslMsgId = PRT_MMI_SMU_VERIFY_PIN_REQ;  
	local_data = (MMISMUVERIFYPINREQ*) OslConstructDataPtr(sizeof(MMISMUVERIFYPINREQ));
	local_data->pin_type = MMI_TYPE_UNSPECIFIED;
	
	/*Change gInputPwdBuffer to ASCII then copy local_data->pin*/
	UnicodeToAnsii((S8*)local_data->pin,(S8*)gInputPwdBuffer );	
	
	mmi_trace(g_sw_SMU,"MMI_SIM: ValidatePIN, Sending request for verifying pin, user input pin is: %s", local_data->pin);
	
	Message.oslDataPtr = (oslParaType *)local_data;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
}

/**************************************************************

	FUNCTION NAME		: ValidatePhoneLock()

  	PURPOSE				: Function to validate Phonelock 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void ValidatePhoneLock(void)
{
 	MYQUEUE	Message; 
	MMISMUVERIFYPINREQ *local_data;
	PRINT_INFORMATION(("MMI_SIM: @@@@@ Sending request for verifying Phone Lock @@@@@"));

	if (!IS_IDLE_ARRIVED)
		g_pwr_context[0].PowerOnPeriod = 1;
	
	if (pfnUnicodeStrlen((S8 *)gInputPwdBuffer) < MMI_MIN_PASSWORD_LENGTH)
	{
		if (pfnUnicodeStrlen((S8 *)gInputPwdBuffer) <= 0)
		{
			return;
		}
		else
		{
			memset((S8 *)gInputPwdBuffer, 0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
			EntryScrInvalidPINPUKPLOCKLength();
			return;
		}
	}
	ClearAllKeyHandler();

	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C;
	Message.oslMsgId = PRT_MMI_SMU_VERIFY_PIN_REQ;  
	local_data = (MMISMUVERIFYPINREQ *)OslConstructDataPtr(sizeof(MMISMUVERIFYPINREQ));

	#if 0
	local_data->pin_type = MMI_TYPE_UNSPECIFIED;
	#else
	local_data->pin_type = MMI_SETTING_TYPE_PHONELOCK;
	#endif
	
	/* 1. Change gInputPwdBuffer to ASCII then copy local_data->pin*/
	UnicodeToAnsii((S8 *)local_data->pin, (S8 *)gInputPwdBuffer);		

	Message.oslDataPtr = (oslParaType *)local_data;
	Message.oslPeerBuffPtr = NULL;

	SetProtocolEventHandler(SECSETVerifyPINRsp, PRT_MMI_SMU_VERIFY_PIN_RSP);

	OslMsgSendExtQueue(&Message);

	PRINT_INFORMATION(("MMI_SIM: @@@@@ Exiting from  Phone Lock  Validate @@@@@"));
}

/**************************************************************

	FUNCTION NAME		: ValidatePUK()

  	PURPOSE				: Function to validate PUK 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void ValidatePUK(void)
{
 	 

	MYQUEUE	Message; 
	MMISMUVERIFYPINREQ* local_data;

	TRACE_SMU_FUNC("ValidatePUK");
	PRINT_INFORMATION(("@@@@@ Sending request for verifying pin  @@@@@"));

	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C + PUKEnterSimID;
	Message.oslMsgId = PRT_MMI_SMU_VERIFY_PIN_REQ;  
	local_data = (MMISMUVERIFYPINREQ*) OslConstructDataPtr(sizeof 
					(MMISMUVERIFYPINREQ));

	local_data->pin_type = MMI_TYPE_UBCHV1; 
	
		UnicodeToAnsii((S8*)local_data->pin,(S8*)gInputPwdBuffer);		
		UnicodeToAnsii((S8*)local_data->new_pin,(S8*)gInputNewPinBufferFirstTime);		
//micha0420
#if (0)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif		
	
	Message.oslDataPtr = (oslParaType *)local_data;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);

}

/**************************************************************

	FUNCTION NAME		: ValidateFirstAndSecondPin()

  	PURPOSE				: Function to validate First and Second PIN 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ValidateFirstAndSecondPin(void)
{

	if (!IS_IDLE_ARRIVED)
		g_pwr_context[0].PowerOnPeriod = 1;

	if(pfnUnicodeStrlen((S8*)gInputNewPinBufferSecondTime) < MMI_MIN_PASSWORD_LENGTH)
	{
		if(pfnUnicodeStrlen((S8*)gInputNewPinBufferSecondTime) <=0)
		{
			return;
		}
		else
		{
			memset((S8*)gInputNewPinBufferSecondTime,0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
			EntryScrInvalidPINPUKPLOCKLength();
			return;
		}
	}

	if( memcmp((S8*)gInputNewPinBufferSecondTime,
				(S8*)gInputNewPinBufferFirstTime,(MAX_PWD_LENGTH_MMI - 1) * ENCODING_LENGTH ) == 0)
	{
		PRINT_INFORMATION(("Both Pin matches , calling ValidatePUK"));	
		ClearAllKeyHandler();

		 
		ValidatePUK();
	}
	else
	{
		EntryScrPinDoNotMatch();
	}
}

/**************************************************************

	FUNCTION NAME		: GoToSOSDialScr()

  	PURPOSE				: Function takes care that from which buffer DialPadBuffer should be filled up. 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GoToSOSDialScr(void)
{
	U8 i;
	BOOL bPINBlocked = MMI_FALSE;
	
   if( g_pwr_context[0].PrevScreenIndicator == ENTER_NEW_PIN )
   {
	   pfnUnicodeStrcpy(g_idle_context.DialPadCallBuffer,(S8*)gInputNewPinBufferFirstTime);
   }
   else if( g_pwr_context[0].PrevScreenIndicator == ENTER_CONFIRM_PIN )
   {
	   pfnUnicodeStrcpy(g_idle_context.DialPadCallBuffer,(S8*)gInputNewPinBufferSecondTime);
   }
   else
   {
	   pfnUnicodeStrcpy(g_idle_context.DialPadCallBuffer,(S8*)gInputPwdBuffer);
   }
//micha0519
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_SecuritySetupContext.PINBlocked[i])
		{
			bPINBlocked = MMI_TRUE;
			break;
		}
	}
	if (bPINBlocked)
	{
		pfnUnicodeStrcpy(g_idle_context.DialPadCallBuffer, (S8*)g_SecuritySetupContext.NewPassword);
	}

   EntryScrEmergencyCallDialing();

}
/**************************************************************

	FUNCTION NAME		: HandleInsertSIMScrDigitEntry()

  	PURPOSE				: Handle Digit entry on Insert SIM

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
	
void HandleInsertSIMScrDigitEntry(void)
{
	U16 Keycode,Keytype;

	GetkeyInfo(&Keycode,&Keytype);
	#if 0
	sprintf(g_idle_context.DialPadCallBuffer,"");
	#else
	g_idle_context.DialPadCallBuffer[0] = 0;
	#endif
	g_idle_context.IdleScreenDigitKeyCode=Keycode;
	DialSOSNumber();

}
#ifdef __FT_IDLE_SMART_DIAL__ 
extern void ShowCategorySmartDialScreen(U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,S32 number_of_items,
	                                                                    GetItemFuncPtr get_item_func, GetHintFuncPtr	get_hint_func,
	                                                                         S32 highlighted_item,	U16 search_image,	U8 * buffer ,	U32 BufferLength,U8* history_buffer );

extern pBOOL mmi__smartdial_search_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask );  
extern U16 IsSmartDialStyle(void);
#endif

/*************************************************************

	FUNCTION NAME		: DialSOSNumber

  	PURPOSE				: Function to make SOS calls 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DialSOSNumber(void)
{
	U8* guiBuffer;       /* Buffer holding history data */

	g_idle_context.IsOnSimErrorDialerScreen = 1;
	
	EntryNewScreen(SCR_DIAL_SOS_NUM, ExitDialSOSNumber, NULL, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_SIM_INSERTION);	

#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__) && !defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)) 
#ifdef __FLIGHT_MODE_SUPPORT__
	if (mmi_bootup_get_active_flight_mode() != 1)
#endif
		EnableCenterSoftkey(0, IMG_CSK_DIAL_ICON);
#endif
#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__) )
	#if defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
#ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
    mmi_idle_set_dual_sim_dialing_keypad_call_handler(EntryScrEmergencyCallDialing, EntryScrEmergencyCallDialing,NULL,NULL);
#else  /* No Defined __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */
	SetDialingKeypadCallHandler(EntryScrEmergencyCallDialing);
#endif /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */
	#else
	SetCenterSoftkeyFunction(EntryScrEmergencyCallDialing, KEY_EVENT_UP);
	#endif
#endif	
#ifdef __FT_IDLE_SMART_DIAL__ 
	if(IsSmartDialStyle())
	{
		ShowCategorySmartDialScreen(STR_GLOBAL_SAVE,
													0,
													STR_GLOBAL_BACK,
													0,
													0,
													mmi__smartdial_search_list_get_item,
													NULL,
													0,
													0,
													(U8*)g_idle_context.DialPadCallBuffer, 
													MAX_DIAL_PAD ,
													guiBuffer );
	}
	else
#endif
	ShowCategory16Screen(STR_GLOBAL_SAVE, 0, STR_GLOBAL_BACK, 0, (U8*)g_idle_context.DialPadCallBuffer, MAX_DIAL_PAD, guiBuffer);
	if(g_idle_context.IdleScreenDigitKeyCode>=0)
		ExecuteCurrKeyHandler(g_idle_context.IdleScreenDigitKeyCode,KEY_EVENT_DOWN);


#ifdef __FLIGHT_MODE_SUPPORT__
	if( mmi_bootup_get_active_flight_mode() == 0 ) /* 2005/11/01 Cannot allow to dial in flight mode insert SIM screen */
#endif
	{
		SetKeyHandler(EntryScrEmergencyCallDialing,KEY_SEND,KEY_EVENT_UP);
#ifdef __SENDKEY2_SUPPORT__
		SetKeyHandler(EntryScrEmergencyCallDialing,KEY_SEND1, KEY_EVENT_DOWN);
		SetKeyHandler(EntryScrEmergencyCallDialing,KEY_SEND2, KEY_EVENT_DOWN);
#endif
	}

	SetCategory16RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	 
	SetLeftSoftkeyFunction(IdleDialPadSavePhoneBook,KEY_EVENT_UP);
#ifdef __MMI_STARKEY_TOGGLE_PROFILE__
		SetKeyHandler(mmi_sim_error_idle_toggle_profile,KEY_STAR,KEY_LONG_PRESS);////qiff add for m301,BUG:10748
#else
		SetKeyHandler(mmi_sim_error_idle_toggle_profile,KEY_POUND,KEY_LONG_PRESS);////qiff add for m301,BUG:10748
#endif
                                                                                                                               
/* For processing SSC */
	if ((g_pwr_context[0].PrevScreenIndicator == ENTER_SIM_INSERTION)||(g_pwr_context[0].PrevScreenIndicator == ENTER_SIM_BLOCK))
	{
		SetKeyHandler(SSCStringParsing1,KEY_POUND,KEY_EVENT_UP);
		
	}
	
	/*Zhangm Add Start For 10951 Ver: GreenStone  on 2008-12-22 9:54 */	
	if(g_idle_context.LongPressEnable==0 && (g_idle_context.IdleScreenDigitKeyCode==KEY_STAR) )
	{
		SetKeyHandler(IdleHandleStarKeyForKeypadLock,KEY_STAR,KEY_LONG_PRESS);
		g_idle_context.LongPressEnable = 1;
	}
	/*Zhangm Add End  For 10951 Ver: GreenStone  on 2008-12-22 9:54 */
}
/*************************************************************

	FUNCTION NAME		: ExitDialSOSNumber

  	PURPOSE				: exit Function for dial SOS screen 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitDialSOSNumber(void)
{
	if(g_pwr_context[0].SOSCallFlagTOAvoidHistorySave!=1)// to avoid history save
	{
		history_t   Scr; /* Variable to hold the history data*/
		s16 nHistory = 0;

		Scr.scrnID = SCR_DIAL_SOS_NUM;
		Scr.entryFuncPtr = DialSOSNumber;
		pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
		GetCategoryHistory(Scr.guiBuffer);
		AddHistory(Scr);
	}
	
	g_idle_context.IdleScreenDigitKeyCode=-1;
	g_idle_context.IsOnSimErrorDialerScreen = 0;
#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && (defined __MMI_TOUCH_SCREEN__) )
	ResetCenterSoftkey();
#endif
}

/*************************************************************

	FUNCTION NAME		: ShowMakeSOSCallScr

  	PURPOSE				: Function to make SOS calls 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ShowMakeSOSCallScr(void)
{
#ifndef __MMI_TOUCH_SCREEN__
#ifdef	__MMI_KEYPAD_LOCK_PATTERN_1__
	if(GetActiveScreenId() == SCR_SIM_INSERTION || GetActiveScreenId() == SCR_SIM_BLOCK)
		g_idle_context.RskPressedFromIdleApp = 1;  
#endif
#endif

	EntryNewScreen(SCR_MAKE_SOS_CALL, ExitShowMakeSOSCallScr, NULL, NULL);

#ifdef __MMI_TOUCH_SCREEN__
	if(gKeyPadLockFlag) 
		mmi_pen_unblock();
#endif
	AnsiiToUnicodeString(g_idle_context.DialPadCallBuffer,"112");	

	ShowCategory165Screen(STR_GLOBAL_YES,IMG_GLOBAL_YES,STR_GLOBAL_NO,IMG_GLOBAL_NO,get_string(STR_DIAL_EMERGENCY_NUMBER),IMG_GLOBAL_QUESTION,NULL);

	SetLeftSoftkeyFunction(EntryScrEmergencyCallDialing,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/*changed end key press from up to down to clear END key
	mapping to DisplayIdleScreen which is done at time of 
	ExecuteCurrExitHandler(). --vanita*/
	SetKeyHandler(GoBackHistory,KEY_END,KEY_EVENT_DOWN);
	playRequestedTone(WARNING_TONE);   //dyj del 20060514 //add by panxu 20061228

 
#ifndef __MMI_TOUCH_SCREEN__
#if (defined(__MMI_KEYPAD_LOCK_PATTERN_1__) && !defined(__MMI_DISABLE_KEYPAD_LOCK__))
/*zhangm Add Start For 10821 Ver: GreenStone  on 2008-12-19 16:23 */
	//if(g_idle_context.ToNameScrFromIdleApp==1) 
//	if(g_idle_context.RskPressedFromIdleApp == 1)
//	{
//		StartTimer(KEYPAD_LOCK_TIMER, KEYPAD_LOCK_TIMEOUT, IdleHandleKeypadLockProcess);
//		SetKeyHandler(IdleHandlePoundKeyForKeypadLock,KEY_POUND,KEY_EVENT_UP);
//	}
/*zhangm Add end For 10821 Ver: GreenStone  on 2008-12-19 16:23 */
#endif
#endif
}

/*************************************************************

	FUNCTION NAME		: ExitShowMakeSOSCallScr

  	PURPOSE				: exit Function for make SOS call screen 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitShowMakeSOSCallScr(void)
{
	if(g_pwr_context[0].SOSCallFlagTOAvoidHistorySave!=1)
	{
		history_t   Scr; /* Variable to hold the history data*/
		s16 nHistory = 0;

		Scr.scrnID = SCR_MAKE_SOS_CALL;
		Scr.entryFuncPtr = ShowMakeSOSCallScr;
		pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
		GetCategoryHistory(Scr.guiBuffer);
//		AddHistory(Scr);
	}
    stopRequestedTone(WARNING_TONE);
}

/*************************************************************

	FUNCTION NAME		: isSOSCall

  	PURPOSE				: Function for CM to notify is SOS 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
pBOOL IsSOSCall(void)
{
	return g_pwr_context[0].ForCMToIdentifySOSCall;
}

/*************************************************************

	FUNCTION NAME		: EntryScrFlightModeMessage

  	PURPOSE			: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS			: void

 

**************************************************************/
#ifdef __FLIGHT_MODE_SUPPORT__
void EntryScrFlightModeMessage(void)
{
	historyNode *History;

	DisplayPopup((U8*)GetString(STR_MENU_FLIGHT_MODE),IMG_GLOBAL_WARNING,0,2000,(U8)ERROR_TONE);

//micha0420
	if(IsScreenPresent(SCR_ID_SECSET_PUK1))
	{
		if(GetHistoryScrID(SCR_ID_SECSET_PUK1,&History)==ST_SUCCESS)
		{
			if(History->inputBuffer!=NULL)
				memset(History->inputBuffer,0,ENCODING_LENGTH);
			if(History->guiBuffer!=NULL)
				memset(History->guiBuffer,0,2);
		}
		 DeleteUptoScrID(SCR_ID_SECSET_PUK1);
	}
}
#endif

/*************************************************************

	FUNCTION NAME		: EntryScrEmergencyCallDialing

  	PURPOSE				: Show the message Emergency Call Dialing..... 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrEmergencyCallDialing(void)
{
	g_pwr_context[0].SOSCallFlagTOAvoidHistorySave=1;

	 
	if(gKeyPadLockFlag == 1)
	{
		gKeyPadLockFlag = 0;
#ifdef __MMI_TOUCH_SCREEN__
		ChangeStatusIconImage(STATUS_ICON_KEYPAD_LOCK, IMG_SI_KEYPAD_UNLOCK);
		UpdateStatusIcons();
#else
		IdleResetStatusIcon(STATUS_ICON_KEYPAD_LOCK);	
#endif
#ifndef __MMI_KEYPADLOCK_WITH_KEYTONE__
		mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);
#endif
	}

#ifdef __MMI_MULTI_SIM__
	mmi_sim_error_set_emergency_call_path();
#endif /* __MMI_MULTI_SIM__ */

	if(CheckValidEmergencyNo1(g_idle_context.DialPadCallBuffer))
	{
		g_pwr_context[0].ForCMToIdentifySOSCall = TRUE;

#ifdef __FLIGHT_MODE_SUPPORT__
		if(mmi_bootup_get_active_flight_mode() == 1) /* 2005/11/01 Cannot allow to dial in flight mode insert SIM screen */
			EntryScrFlightModeMessage();
		else
#endif
		if( IsSOSPossible() )
		{
#ifdef __MMI_Emergency_Call_911_Support__
		   /***  For Insert SIM, Dail 911 as emergency call ***/

		   /* MakeCall (g_idle_context.DialPadCallBuffer); */
		   MakeCall( (PS8) EmergencyNumber[0] );

#else
		   MakeCall (g_idle_context.DialPadCallBuffer);
#endif
		}
		 else
		{	
			EntryScrNoNetworkServiceMessage();
		}
	}
	else if(!SSCStringParsing2())
	{
#ifdef __FLIGHT_MODE_SUPPORT__
		if(mmi_bootup_get_active_flight_mode() == 1) /* 2005/11/01 Cannot allow to dial in flight mode insert SIM screen */
			EntryScrFlightModeMessage();
		else
#endif
			EntryScrNotSOSNumMessage();
	}
}

/**************************************************************

	FUNCTION NAME		: EntryScrNoNetworkServiceMessage

  	PURPOSE				: Entry Screen for no network service Message

	INPUT PARAMETERS	: void 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/

void EntryScrNoNetworkServiceMessage(void)
{
	historyNode *History;

	DisplayPopup((U8*)GetString(STR_NO_SERVICE_RUNNING_TEXT),IMG_GLOBAL_WARNING,0,2000,(U8)ERROR_TONE);

//micha0420
	if(IsScreenPresent(SCR_ID_SECSET_PUK1))
	{
		if(GetHistoryScrID(SCR_ID_SECSET_PUK1,&History)==ST_SUCCESS)
		{
			if(History->inputBuffer!=NULL)
				memset(History->inputBuffer,0,ENCODING_LENGTH);
			if(History->guiBuffer!=NULL)
				memset(History->guiBuffer,0,2);
		}
		 DeleteUptoScrID(SCR_ID_SECSET_PUK1);
	}
}

/**************************************************************

	FUNCTION NAME		: EntryScrNotSOSNumMessage

  	PURPOSE				: This function flash message that this one is not a vaild SOS number.

	INPUT PARAMETERS	: void 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/
void EntryScrNotSOSNumMessage(void)
{
	historyNode *History;
	DisplayPopup((U8*)GetString(STR_NOT_SOS_NUMBER),IMG_GLOBAL_WARNING,0,2000,(U8)ERROR_TONE);
//micha0420
	if(IsScreenPresent(SCR_ID_SECSET_PUK1))
	{
		if(GetHistoryScrID(SCR_ID_SECSET_PUK1,&History)==ST_SUCCESS)
		{
			if(History->inputBuffer!=NULL)
				memset(History->inputBuffer,0,ENCODING_LENGTH);
			if(History->guiBuffer!=NULL)
				memset(History->guiBuffer,0,2);
		}
		 DeleteUptoScrID(SCR_ID_SECSET_PUK1);
	}
}

/**************************************************************

	FUNCTION NAME		: IsSOSPossible

  	PURPOSE				: Return the possibility of SOS call

	INPUT PARAMETERS	: void 

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL 

 

**************************************************************/
pBOOL IsSOSPossible()
{
	// renwy del begin
	/*
	if(g_pwr_context.CurrentServiceType  == FULL_SERVICE || 
	   g_pwr_context.CurrentServiceType  == LIMITED_SERVICE || 
	   g_pwr_context.InvalidSim)
		return TRUE;
	else
		return FALSE;
	*/
	//renwy  del end
	
	return TRUE;//renwy modify
}
/**************************************************************


	FUNCTION NAME		: NumberOfPLMNSuported

  	PURPOSE				: This function is a call back for 
	                      MSG_ID_MMI_SMU_SUPPORT_PLMN_LIST_IND 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void *

	RETURNS				: void

 

**************************************************************/
void NumberOfPLMNSuported(void *iMsg)
{
	U8 nSimID;
	MMISMUSUPPORTPLMNLISTIND *local_data = (MMISMUSUPPORTPLMNLISTIND *)iMsg;
	PRINT_INFORMATION(("NumberOfPLMNSuported\n"));

	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	g_pwr_context[nSimID].NoOfPLMNSupported = local_data->length;
	PRINT_INFORMATION(("NumberOfPLMNSuported gNoOfPLMNSupported = %d\n",g_pwr_context[nSimID].NoOfPLMNSupported));
	
}
/**************************************************************


	FUNCTION NAME		: NetWorkServiceTypeIndication

  	PURPOSE				: This function is a call back for 
	                      PRT_BATTERY_STATUS_IND 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void *

	RETURNS				: void

 

**************************************************************/
void NetWorkServiceTypeIndication(void *iMsg)
{
//micha0120 begin
	
//	MMINWREGSTATEIND *local_data = (MMINWREGSTATEIND *)iMsg;
	mmi_nw_reg_state_ind_struct *local_data = (mmi_nw_reg_state_ind_struct *)iMsg;
	PRINT_INFORMATION(("\n NetWorkServiceTypeIndication\n"));
    if(local_data->status == MMI_RAC_OK)
    {

#if 0/* JIASHUO FOR WAP/MMS */
//micha0120
	UpdateGPRSStatusIcon(local_data->gprs_status);
#endif

//micha0423
		if( gGPRSStatusModified )
		{
			refresh_status_icon_display();
			gGPRSStatusModified = 0;
		}

//Unit test code
//		UpdateGPRSStatusIcon(L4C_GPRS_ATTACHED);
//		UpdateGPRSStatusIcon(L4C_GPRS_COVERAGE);
//		UpdateGPRSStatusIcon(L4C_PDP_ACTIVED);
     }
     else
     {
//Do nothing, keep original state     
//        ResetStatusIcon(STATUS_ICON_GPRS_SERVICE);
     }

//micha0120 end	 
}
/**************************************************************


	FUNCTION NAME		: ServiceIndicationStringDisplay

  	PURPOSE				: This function is a call back for
	                      MSG_ID_MMI_CPHS_MMI_INFO_IND
 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void *

	RETURNS				: void

 

**************************************************************/

void ServiceIndicationStringDisplay(void *inMsg)
{
#if 0
	mmi_cphs_mmi_info_ind_struct *local_data = (mmi_cphs_mmi_info_ind_struct *)inMsg;
	U8 i=0, len=0;
	PS8 pString;


	PRINT_INFORMATION(("MSG_ID_MMI_CPHS_MMI_INFO_IND"));

	gOpNameValid = local_data->is_opname_valid;
	gShortOpnameValid = local_data->is_sopname_valid;
	gSpnValid = local_data->is_spn_valid;

 
	if (gSpnValid == 1)
		g_pwr_context[0].RequireRPLMN = local_data->is_spn_RPLMN_required;
// CSD End

	if(gOpNameValid)
	{
		AnsiiToUnicodeString(gOperatorName,(S8*)local_data->opname);	
	}

	if(gShortOpnameValid)
	{
		AnsiiToUnicodeString(gShortOperatorName,(S8*)local_data->sopname);	
	}

	if(gSpnValid)
	{
		if(local_data->spn[0] >= 0x80 )  
		{
			U8 *convertedData=NULL;
			U8 dcs;
			switch(local_data->spn[0])
			{
				case 0x80:
					dcs = MMI_UCS2_DCS;
				break;
				case 0x81:
					dcs = MMI_UCS2_81;
				break;
				case 0x82:
					dcs = MMI_UCS2_82;
				break;
				default:
					dcs = 0;
				break;
			}
			if(dcs==0)
			{
				gSpnValid = 0;
			}
			else
			{
				convertedData = CovertStringForPlatform(local_data->spn+1, (U16)(strlen((PS8)(local_data->spn+1))), dcs, NULL);
				pfnUnicodeStrcpy((PS8)gServProvderName, (PS8)convertedData);
				OslMfree(convertedData);
			}
		}
		else
		{
			AnsiiToUnicodeString(gServProvderName,(S8*)local_data->spn);	
		}
	}

      
	memset( (void *)g_pDispOwnerNumber, 0, sizeof(MMI_OWNER_NUMBER_STRUCT) );
	if( local_data->no_msisdn > 0 )
	{
		for( i=0; ((i< MAX_LINE_NUM) && (i< local_data->no_msisdn)) ; i++ )
		{
			if( local_data->msisdn[i].alpha_id_size != 0)
			{
				/*
				if(local_data->msisdn[i].alpha_id_dcs == MMI_PHB_UCS2)
				{
                       //Prevent PS bug, Watch out!! sould sync with l4csmu_addr_struct
                   local_data->msisdn[i].alpha_id[MAX_PB_NAME_LENGTH]=0x0;
                   local_data->msisdn[i].alpha_id[MAX_PB_NAME_LENGTH-1]=0x0;
					//BigEndianToLittleEndian((PS8)gDispOwnerNumber[i].name, (PS8)local_data->msisdn[i].alpha_id); 
				}
				else
                {
					//len = (((local_data->msisdn[i].alpha_id_size) > MAX_PB_NAME_LENGTH) ?  MAX_PB_NAME_LENGTH : (local_data->msisdn[i].alpha_id_size) );
					//PhbGSM7BitToAscii((PU8)local_data->msisdn[i].alpha_id, local_data->msisdn[i].alpha_id_size );
					//AnsiiToUnicodeString((PS8)gDispOwnerNumber[i].name, (PS8)local_data->msisdn[i].alpha_id);
				}
				*/
				len = (((local_data->msisdn[i].alpha_id_size) > MAX_PB_NAME_LENGTH) ?  MAX_PB_NAME_LENGTH : (local_data->msisdn[i].alpha_id_size) );				
				InputboxConvertGSMToDeviceEncoding(local_data->msisdn[i].alpha_id, g_pDispOwnerNumber[i].name, len,(MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH, local_data->msisdn[i].alpha_id_dcs, 0, 1);
		    }

		    if( strlen((S8*)local_data->msisdn[i].digit )!=0 )
			{
	            //Prevent PS error. Watch out!! should sync with l4csmu_addr_struct
               local_data->msisdn[i].digit[MAX_PB_NUMBER_LENGTH]=0x0;
               local_data->msisdn[i].digit[MAX_PB_NUMBER_LENGTH-1]=0x0;
		       if( ((local_data->msisdn[i].ton_npi) & 0x10) != 0 )
		       {
		           len = MAX_PB_NUMBER_LENGTH;
	               g_pDispOwnerNumber[i].number[0] = '+';
                   pString = (PS8)g_pDispOwnerNumber[i].number+ENCODING_LENGTH;
	           }
		       else
		       {
		           len = MAX_PB_NUMBER_LENGTH+1;
                   pString = (PS8)g_pDispOwnerNumber[i].number;
		       }

 				    AnsiiNToUnicodeString((PS8)pString, (S8*)local_data->msisdn[i].digit, len );
			}
   		}
	}

//micha0309
	RefreshDisplayOwnerNumber((U8)g_callset_cntx_p->LineID);
//CSD end

	if(g_idle_context.IsOnIdleScreen)
	{
		EntryIdleScreen();
	}
#endif
}

#define MAX_SIGNALS 5
#ifdef __MMI_MULTI_SIM__
U8 GetAverageSignal(U8 signal_level, U8 nSimId)
#else  /* No Defined __MMI_MULTI_SIM__ */
U8 GetAverageSignal(U8 signal_level)
#endif /* __MMI_MULTI_SIM__ */
{
#ifdef __MMI_MULTI_SIM__
	static U8 g_signalarray[MMI_SIM_NUMBER][MAX_SIGNALS] = {{0}};
	static U8 signal_pointer[MMI_SIM_NUMBER] = {0};
#else  /* No Defined __MMI_MULTI_SIM__ */
	static U8 g_signalarray[MAX_SIGNALS] = {0};
	static U8 signal_pointer = 0;
#endif /* __MMI_MULTI_SIM__ */
	PU8 p_signal, p_signalpointer;
	U16 total_signal = 0;
	int i, count = 0;

#ifdef __MMI_MULTI_SIM__
	p_signal = g_signalarray[nSimId];	
	p_signalpointer = &signal_pointer[nSimId];
#else  /* No Defined __MMI_MULTI_SIM__ */
	p_signal = g_signalarray;
	p_signalpointer = &signal_pointer;
#endif /* __MMI_MULTI_SIM__ */
	p_signal[*p_signalpointer] = signal_level;
	if (*p_signalpointer < MAX_SIGNALS - 1)
	{
		(*p_signalpointer)++;
	}
	else
	{
		*p_signalpointer = 0;
	}

	for (i=0; i<MAX_SIGNALS; i++)
	{
		if (p_signal[i] == 0)
		{
			continue;
		}
		count++;
		total_signal += p_signal[i];
	}
	
	if (count == 0)
		return 0;
	
	return (total_signal / count);
	
}

#ifdef __MMI_STOP_SIGNAL_REPORT__
void NetWorkSignalStopReport(U8 nStart)
{

	mmi_nw_stop_signal_req_struct *myMsgPtr;
	MYQUEUE Message;

	mmi_trace(g_sw_NW, "MMI_NW: NetWorkSignalStopReport");
	myMsgPtr = (mmi_nw_stop_signal_req_struct*) OslConstructDataPtr(sizeof(mmi_nw_stop_signal_req_struct));
	myMsgPtr->nStart = nStart;
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C;
	Message.oslMsgId = MSG_ID_MMI_NW_RX_STOP_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);

}
#endif

/**************************************************************


	FUNCTION NAME		: NetWorkSignalLevelIndication

  	PURPOSE				: This function is a call back for 
 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void *

	RETURNS				: void

 

**************************************************************/

void NetWorkSignalLevelIndication(void *inMsg)
{
	// U8 old_gSignalStrengthLevel = gSignalStrengthLevel;

	MMINWRXLEVELIND *local_data = (MMINWRXLEVELIND *)inMsg;
	U8 level;
	U8 nSimID;
	
	#ifdef __MMI_MULTI_SIM__
	U8 sim_status;
	#endif

	nSimID = MTPNP_AD_get_protocol_event_src_mode();

      g_pwr_context[nSimID].CurrentCampOnBand = local_data->current_band;

#if 0
#ifdef __MMI_MULTI_SIM__
	level = GetAverageSignal(local_data->level, nSimID);
#else  /* No Defined __MMI_MULTI_SIM__ */
	level = GetAverageSignal(local_data->level);
#endif /* __MMI_MULTI_SIM__ */
#endif /* Guoj delete. It's not used now at 2010-3-26 */

	level = local_data->level;

	mmi_trace(g_sw_NW, "MMI_NW: NetWorkSignalLevelIndication(%d) local_data->level=%d, average level = %d", nSimID, local_data->level, level );
	 
	if (level == SIGNAL_STRENGTH_INVAILD)
	{
		PRINT_INFORMATION(("Invalid Signal Indication"));
	}
	else if(level == SIGNAL_STRENGTH_NONE)
	{
		PRINT_INFORMATION(("No Signal Indication"));
		gSignalStrengthLevel[nSimID] = 0;
	}
	else if( level < SIGNAL_STRENGTH_LEVEL1 )  
	{
		PRINT_INFORMATION(("No Signal Indication"));
		gSignalStrengthLevel[nSimID] = 0;
	}
	else if( (level >= SIGNAL_STRENGTH_LEVEL1) && (level < SIGNAL_STRENGTH_LEVEL2) )
	{

		PRINT_INFORMATION(("Show only one bar shaded "));
		gSignalStrengthLevel[nSimID] = 20;
	}
	else if( (level >= SIGNAL_STRENGTH_LEVEL2) && (level < SIGNAL_STRENGTH_LEVEL3) )
	{
		PRINT_INFORMATION(("Show two  bar shaded "));
		gSignalStrengthLevel[nSimID] = 40;
	}
	else if( (level >= SIGNAL_STRENGTH_LEVEL3) && (level < SIGNAL_STRENGTH_LEVEL4) )
	{
		PRINT_INFORMATION(("Show three  bar shaded " ));
		gSignalStrengthLevel[nSimID] = 60;
	}
	else if( (level >= SIGNAL_STRENGTH_LEVEL4)  && (level < SIGNAL_STRENGTH_LEVEL5))
	{

		PRINT_INFORMATION(("Show four bar shaded " ));
		gSignalStrengthLevel[nSimID] = 80; 
	}
	else if (level >= SIGNAL_STRENGTH_LEVEL5)
	{
		PRINT_INFORMATION(("Show five bar shaded " ));
		gSignalStrengthLevel[nSimID] = 100; 

	}

	#ifdef __MMI_MULTI_SIM__
	sim_status = MTPNP_AD_Get_Startup_Mode();
	mmi_trace(g_sw_NW, "MMI_NW: NetWorkSignalLevelIndication(%d), sim_status is 0x%x",nSimID, sim_status);
	if(!(sim_status&(DM_STATUS_MASK<<nSimID)))
	{
		gSignalStrengthLevel[nSimID] = 0;
	}
	#endif

	 
	mmi_trace(g_sw_NW, "MMI_NW: NetWorkSignalLevelIndication(%d), CurrentServiceType is %d",nSimID, g_pwr_context[nSimID].CurrentServiceType);
	if( g_pwr_context[nSimID].CurrentServiceType == NO_SERVICE )
		ChangeStatusIconLevel(STATUS_ICON_SIGNAL_STRENGTH + nSimID,SIGNAL_STRENGTH_NONE);     
	else
		ChangeStatusIconLevel(STATUS_ICON_SIGNAL_STRENGTH + nSimID, gSignalStrengthLevel[nSimID]);
#ifdef __MMI_SUBLCD__
     if( g_pwr_context[nSimID].CurrentServiceType == NO_SERVICE )
	 	ChangeStatusIconLevel(STATUS_ICON_SUBLCD_SIGNAL_STRENGTH,SIGNAL_STRENGTH_NONE);
     else
	 	ChangeStatusIconLevel(STATUS_ICON_SUBLCD_SIGNAL_STRENGTH,gSignalStrengthLevel[0]);
#endif
}

/**************************************************************


	FUNCTION NAME		: RefreshNwProviderName

  	PURPOSE				: This function refreshes the Nw provider name

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void *

	RETURNS				: void

 

**************************************************************/
void RefreshNwProviderName(U8 *UpdatePlmn, U8 simID)
{
	 U16	count;	     			   

#ifdef __MMI_EONS_SUPPORT__
	 
	U8 recordnum = eons_check_if_plmn_present(UpdatePlmn);
	if(recordnum)
	{
		eons_get_pnn_record_req(recordnum);
		return;
	}
#endif
    /*
    if(gSPNIfo[simID].nSPNDisplayFlag == 1 && gSPNIfo[simID].nSPNameLen > 0)
    {
        if(gSPNIfo[simID].nSpnName[0] > 0x7F )
        {
            memcpy(gNWProviderName, gSPNIfo[simID].nSpnName + 1, gSPNIfo[simID].nSPNameLen - 1);
        }
        else
        {
            AnsiiToUnicodeString((S8*)gNWProviderName, (S8*)gSPNIfo[simID].nSpnName);	
        }
        return;
    }*/
    if(!((memcmp(UpdatePlmn, "46002", 5) == 0) || (memcmp(UpdatePlmn, "46000", 5) == 0) || (memcmp(UpdatePlmn, "31017", 5) == 0)))
    {
        if(gSPNIfo[simID].nSPNDisplayFlag == 1 && gSPNIfo[simID].nSPNameLen > 0)
        {
            gSpnValid[simID] = 1;
            
            if(gSPNIfo[simID].nSpnName[0] > 0x7F )
            {
                memcpy((S8*) gServProvderName[simID], gSPNIfo[simID].nSpnName + 1, gSPNIfo[simID].nSPNameLen - 1);
            }
            else
            {
                AnsiiToUnicodeString((S8*) gServProvderName[simID], (S8*)gSPNIfo[simID].nSpnName);	
            }
        }
   }
/* Search the number from the plmn table and store in global variable to display on idle scren  */
	for(count = 0; count < MAX_PLMN_NO_LIST_LENGTH; ++count)
	{
//micha0816
		if(memcmp(sPlmnsToDisplayName[count],UpdatePlmn,strlen((PS8) sPlmnsToDisplayName[count])) == 0)
		{
			break;
		}
	}
	if(count < MAX_PLMN_NO_LIST_LENGTH)
	{
			memcpy((S8*)gNWProviderName[simID],GetCurrentPLMNName((S8*)sPlmnsToDisplayName[count]),MAX_LENGTH_DISPLAY_NAME * ENCODING_LENGTH);	
	}
	else
	{
			AnsiiToUnicodeString((S8*)gNWProviderName[simID],(S8*)UpdatePlmn);	
	}
}

/*****************************************************************************
 * FUNCTION
 *  mmi_bootup_sim_indication_bt_power_on
 * DESCRIPTION
 *  
 * PARAMETERS
 *  sim_ind         [IN]        
 *  msg_type        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_bootup_sim_indication_bt_power_on(U8 sim_ind, U16 msg_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifdef __MMI_BT_SUPPORT__
    static U8 BTPowerOnFlag = MMI_FALSE;
    pBOOL status;
#endif /* __MMI_BT_SUPPORT__ */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_IRDA_SUPPORT__
    if (mmi_bootup_is_sim_valid() == MMI_TRUE && sim_ind == 0 && msg_type != MMI_SIM_REFRESH &&
        MMI_IRDASCR_CNTX(irda_state) != MMIR_SCR_IRCLOSE)
    {
        if (mmi_irda_is_ircomm_active() == MMI_TRUE)
        {
            mmi_irda_ircomm_disconnect_request();
        }
        mmi_irda_exec_close_menu();
    }
#endif /* __MMI_IRDA_SUPPORT__ */ 

#ifdef __MMI_BT_SUPPORT__
    if (BTPowerOnFlag)
    {
        return;
    }

    if (sim_ind == 0)   /* sim failure indication */
    {
        status = MMI_FALSE;
    }
    else    /* sim password indication */
    {
        switch (msg_type)
        {
            case MMI_TYPE_PHONELOCK:
            case MMI_TYPE_CHV1:
            case MMI_TYPE_UBCHV1:
            case MMI_TYPE_NP:
            case MMI_TYPE_NSP:
            case MMI_TYPE_SP:
            case MMI_TYPE_CP:
            case MMI_TYPE_IMSI_LOCK:
//            case MMI_TYPE_LINK_NS_SP:
    //        case MMI_TYPE_LINK_SIM_C:
                return;
                break;

            case MMI_TYPE_SIM_CARD_BLOCKED:
                status = MMI_FALSE;
                break;

            case MMI_TYPE_NO_REQUIRED:
            default:
                status = MMI_TRUE;
                break;
        }
    }

    mmi_bt_ps_stack_and_hw_init(mmi_bootup_get_active_flight_mode(), status);
    BTPowerOnFlag = MMI_TRUE;
#endif /* __MMI_BT_SUPPORT__ */ 
}

/**************************************************************


	FUNCTION NAME		: NetWorkAttachedIndication

  	PURPOSE				: This function is a call back for 
	                      MSG_ID_MMI_NW_ATTACH_IND 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void *

	RETURNS				: void

 

**************************************************************/
void NetWorkAttachedIndication(void *inMsg)
{
	 
	MMINWATTACHIND *local_data = (mmi_nw_attach_ind_struct *)inMsg;
	U8 nSimID;
	U16 i;
	BOOL NeedCampOnTonePlay = MMI_FALSE;
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	Trace( "MMI_NW: NetWorkAttachedIndication(%d), local_data->status=%d", nSimID,local_data->status);

	// Do nothing because GET_L4 is searching network.
	if (local_data->status == MMI_RAC_SEARCHING)
		return;

/*Guojian Add Start For 13497 Ver: GreenStone  on 2009-9-2 16:45 */
#ifdef __MMI_MULTI_SIM__
	if ((MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL) == nSimID) && isInCall())
	{
		return;
	}
#endif /* __MMI_MULTI_SIM__ */
/*Guojian Add End  For 13497 Ver: GreenStone  on 2009-9-2 16:45 */

	//[221] Added by zhangxb 2009/04/17								[BEGIN]
	//如果正在显示"搜网中..."则立即关闭之.
	if (gIsOnSearchingScreen)
	{
		StopTimer(SEARCHING_SCREEN_TIMER);
//		playRequestedTone(CAMP_ON_TONE);
		SearchingSrceenTimeOutHandler();
	}
	//[221] Added by zhangxb 2009/04/17								[END]

#if 0/* JIASHUO FOR WAP/MMS */
//micha0120
	UpdateGPRSStatusIcon(local_data->gprs_status);
#endif

//micha0317
	if ((pfnUnicodeStrcmp((PS8)gNWProviderName[nSimID], (PS8)GetString(STR_ID_NETSET_SEARCHIN_NW)) == 0) ||
		(local_data->status != lastNwStatus[nSimID]) || memcmp(local_data->plmn,lastPLMN[nSimID],MAX_PLMN_LEN_MMI)) {
		gGPRSStatusModified = 1;
	}
	memset(gNWProviderName[nSimID],0,MAX_LENGTH_DISPLAY_NAME * ENCODING_LENGTH);
	memcpy(&gSPNIfo[nSimID], &local_data->spn_info, sizeof(MMI_SPN_INFO));

	switch(local_data->status) /* l4c_rac_response_enum */
	{
		/* normal service */
		case MMI_RAC_OK:
			if(local_data->gsm_state == 1)
			{
				g_pwr_context[nSimID].PowerOnSIMStatus |= MMI_SIM_STATUS_ROAMING;
			}
			else
			{
				g_pwr_context[nSimID].PowerOnSIMStatus &= ~MMI_SIM_STATUS_ROAMING;
			}
/* Required for othe applications to indicate them full service*/
			g_pwr_context[nSimID].CurrentServiceType = FULL_SERVICE;
			ChangeStatusIconLevel(STATUS_ICON_SIGNAL_STRENGTH + nSimID,gSignalStrengthLevel[nSimID]);
			ChangeStatusIconLevel(STATUS_ICON_SUBLCD_SIGNAL_STRENGTH, gSignalStrengthLevel[0]);

			/*Copy plmn to global variable to be checked later for displaying roaming signals*/
			memcpy(gPLMN[nSimID], local_data->plmn, MAX_PLMN_LEN_MMI);

			RefreshNwProviderName(gPLMN[nSimID], nSimID);
			if( g_pwr_context[nSimID].testRoaming == 3)  
			{
	 
#ifdef __MMI_EONS_SUPPORT__
				if(eons_check_if_plmn_present(gPLMN[nSimID]) == 0)
#endif
					ChangeRoamingState(nSimID);
			}
			else if ( g_pwr_context[nSimID].testRoaming == 1 ) /* No body send get IMSI req */
			{
				RequestToTestRoamingEnabled();     
			}

			/* Send message to check Roaming enabled or not signal*/

	//micha1111, #223
			lastNwStatus[nSimID] = MMI_RAC_OK;

   			memset(lastPLMN[nSimID], '\0', sizeof(lastPLMN[nSimID]));
			memcpy(lastPLMN[nSimID], gPLMN[nSimID], MAX_PLMN_LEN_MMI);


		g_pwr_context[nSimID].NetWorkFlag = 1;
		 
	/*qiff end*/			
#ifdef __MMI_IMPS__
               mmi_imps_notify_rac_ready();
#endif
 
#ifdef __MMI_POC__
			if (isIdleScreenReached) {
			mmi_poc_poweron_reg();
			}
#endif
			// TODO: 为了测试暂时屏蔽...
			#if 0
			EntryIdleScreen(void)();
			#endif
			break;

		/* no service */

	case MMI_RAC_NO_CELL:

		/* Required for othe applications to indicate them no service*/
		g_pwr_context[nSimID].CurrentServiceType  = NO_SERVICE;
		g_pwr_context[nSimID].NetWorkFlag = 0;

		IdleResetStatusIcon(STATUS_ICON_ROAMING + nSimID);

 
		PRINT_INFORMATION(("No Service"));


		if (IS_IDLE_ARRIVED) //1211
			StopLEDPatternNoService();  

 
           ChangeStatusIconLevel(STATUS_ICON_SIGNAL_STRENGTH + nSimID, 0); 
           ChangeStatusIconLevel(STATUS_ICON_SUBLCD_SIGNAL_STRENGTH, 0);

//micha1111, #223
		   lastNwStatus[nSimID] = MMI_RAC_NO_CELL;
 
			if(g_idle_context.IsOnIdleScreen)
	           IdleSetLRKey();
 
		break;

		/* limit service */

	case MMI_RAC_LIMITED_SERVICE:
		/* Required for othe applications to indicate them no service*/
		g_pwr_context[nSimID].CurrentServiceType  = LIMITED_SERVICE;
		g_pwr_context[nSimID].NetWorkFlag = 1;

		IdleResetStatusIcon(STATUS_ICON_ROAMING+ nSimID);
 
		ChangeStatusIconLevel(STATUS_ICON_SIGNAL_STRENGTH + nSimID, gSignalStrengthLevel[nSimID]);
		ChangeStatusIconLevel(STATUS_ICON_SUBLCD_SIGNAL_STRENGTH, gSignalStrengthLevel[0]);
 
 
		PRINT_INFORMATION(("Limited Service"));


		if (IS_IDLE_ARRIVED)  
			StopLEDPatternLimitService();  

//micha1111, #223
			lastNwStatus[nSimID] = MMI_RAC_LIMITED_SERVICE;
            
			if(g_idle_context.IsOnIdleScreen)
			IdleSetLRKey();  
            
		break;

//micha1119 vito request begin

	case MMI_RAC_ATTEMPT_TO_UPDATE:
		g_pwr_context[nSimID].CurrentServiceType  = LIMITED_SERVICE;
		g_pwr_context[nSimID].NetWorkFlag = 1;

		IdleResetStatusIcon(STATUS_ICON_ROAMING+ nSimID);

 
		PRINT_INFORMATION(("Limited Service"));


		if (IS_IDLE_ARRIVED)   
			StopLEDPatternLimitService();  

		lastNwStatus[nSimID] = MMI_RAC_ATTEMPT_TO_UPDATE;

			if(g_idle_context.IsOnIdleScreen)
			IdleSetLRKey(); 

		break;
//micha1119 vito request end		


//micha0923, #169 begin

	case MMI_RAC_INVALID_SIM:
		g_pwr_context[nSimID].InvalidSim = 1;
		g_pwr_context[nSimID].CurrentServiceType  = LIMITED_SERVICE;

		g_pwr_context[nSimID].NetWorkFlag = 1;

//micha0120
//		ResetStatusIcon(STATUS_ICON_GPRS_SERVICE);

 
		PRINT_INFORMATION(("Limited Service"));

		RefreshNwProviderName(gPLMN[nSimID], nSimID);

		if (IS_IDLE_ARRIVED)   
			StopLEDPatternNoService();  

//micha1111, #223
			lastNwStatus[nSimID] = MMI_RAC_INVALID_SIM;
			
			break;

//micha1111, #223
			case MMI_RAC_ERROR:

//micha0120
//			ResetStatusIcon(STATUS_ICON_GPRS_SERVICE);

		switch (lastNwStatus[nSimID])
		{

		case MMI_RAC_OK:
			g_pwr_context[nSimID].CurrentServiceType  = FULL_SERVICE;
			g_pwr_context[nSimID].NetWorkFlag = 1;

			memcpy(gPLMN[nSimID], lastPLMN[nSimID], MAX_PLMN_LEN_MMI);
			RefreshNwProviderName(gPLMN[nSimID],nSimID);
			 

			if (IS_IDLE_ARRIVED)
				StopLEDPatternNoService();

			break;

		case MMI_RAC_NO_CELL:
			g_pwr_context[nSimID].CurrentServiceType  = NO_SERVICE;
			g_pwr_context[nSimID].NetWorkFlag = 0;

			RefreshNwProviderName(gPLMN[nSimID],nSimID);

			 
			if (IS_IDLE_ARRIVED) //Idle screen have been reached
				StopLEDPatternNoService();  

			break;

		case MMI_RAC_LIMITED_SERVICE:
			g_pwr_context[nSimID].CurrentServiceType  = LIMITED_SERVICE;
			g_pwr_context[nSimID].NetWorkFlag = 1;

			RefreshNwProviderName(gPLMN[nSimID],nSimID);

			 
			if (IS_IDLE_ARRIVED)
				StopLEDPatternNoService();  

			break;

		case MMI_RAC_INVALID_SIM:
			g_pwr_context[nSimID].InvalidSim = 1;
			g_pwr_context[nSimID].CurrentServiceType  = LIMITED_SERVICE;
			g_pwr_context[nSimID].NetWorkFlag = 1;

			RefreshNwProviderName(gPLMN[nSimID],nSimID);

			 
			if (IS_IDLE_ARRIVED)  //Idle screen have been reached
				StopLEDPatternNoService();  

			break;

		case MMI_RAC_ATTEMPT_TO_UPDATE:
			g_pwr_context[nSimID].CurrentServiceType  = LIMITED_SERVICE;
			g_pwr_context[nSimID].NetWorkFlag = 1;

			RefreshNwProviderName(gPLMN[nSimID],nSimID);

			 
			if (IS_IDLE_ARRIVED) //Idle screen have been reached
				StopLEDPatternNoService();  

			break;

		default:
			break;
		}

//micha1111, #223 end
		break;
	}
    if(local_data->status <= MMI_RAC_ATTEMPT_TO_UPDATE )
    {
        IdleRefreshServiceIndicationArea();
        if (GetActiveScreenId() == IDLE_SCREEN_ID)
        {   
            idle_screen_show_network_name();       
        }
    }
#if 0
//micha0423
		if( gGPRSStatusModified )  
		{
			if (g_idle_context.IsOnIdleScreen && g_pwr_context.testRoaming == 3 )
			{
				 
				/* postpone EntryIdleScreen because MMI has to wait for eons_get_pnn_record_rsp() */
				#ifdef __MMI_EONS_SUPPORT__
				if(eons_check_if_plmn_present(gPLMN) == 0)
				#endif
				EntryIdleScreen();
			}
			else
			{
				refresh_status_icon_display();
			}
			gGPRSStatusModified = 0;			
		}
#endif
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(!g_pwr_context[i].IsCampOnTonePlayed && g_pwr_context[i].CurrentServiceType == FULL_SERVICE && g_idle_context.IsOnIdleScreen)
		{
			NeedCampOnTonePlay = MMI_TRUE;
			break;
		}
	}
	if(NeedCampOnTonePlay)
	{
		playRequestedTone(CAMP_ON_TONE);
		//only play once, no matter how much cards
		for(i = 0; i < MMI_SIM_NUMBER; i++)
		{
			g_pwr_context[i].IsCampOnTonePlayed = 1;
		}
	}

//micha0923, #169	
	if( g_pwr_context[nSimID].InvalidSim  && (g_idle_context.IsOnIdleScreen || IsScreenPresent(IDLE_SCREEN_ID)) &&  !g_pwr_context[nSimID].InvalidSimShowed) 
	{
		g_pwr_context[nSimID].InvalidSimShowed = 1;
		DisplayInvalidSimInfo();
	}

}

/**************************************************************


	FUNCTION NAME		: RequestToTestRoamingEnabled()

  	PURPOSE				: To send message MSG_ID_MMI_SMU_GET_IMSI_REQ
						  in response of this message imsi field is returned
						  compare first 5byte of this arr , if they are not same 
						  with the plmn , means roaming is enabled, display the 
						  roaming icon. 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void RequestToTestRoamingEnabled(void)
{
	MYQUEUE	Message; 
	U8 nSimID;

	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	SetProtocolEventHandlerExt(RspToTestRoamingEnabled,PRT_GET_IMSI_RSP, nSimID);
	g_pwr_context[nSimID].testRoaming = 2;

	PRINT_INFORMATION(("@@@@@ RequestToTestRoamingEnabled  @@@@@"));
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C + nSimID;
	Message.oslMsgId = PRT_GET_IMSI_REQ;
	Message.oslDataPtr = NULL;//(oslParaType *)local_data;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_netset_plmn_matching_criteria
 * DESCRIPTION
 *  Implementation of 23.122 for PLMN match criteria
 * PARAMETERS
 *  sim_plmn        [IN]
 *  bcch_plmn       [IN]
 * RETURNS
 *  MMI_BOOL
 *****************************************************************************/
MMI_BOOL mmi_netset_plmn_matching_criteria(const U8 *sim_plmn, const U8 *bcch_plmn, UINT8 sim_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_BOOL rtn;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_pwr_context[sim_id].CurrentCampOnBand == MMI_BAND_1900)
    {
        if (memcmp(sim_plmn, bcch_plmn, 3) == 0)
        {
            if (strlen((S8*) bcch_plmn) == 5)
            {
                if (*(bcch_plmn) == 0x33 && *(bcch_plmn + 1) == 0x31 && (*(bcch_plmn + 2) >= 0x30 && *(bcch_plmn + 2) <= 0x36))
                {
                    if (strlen((S8*) sim_plmn) == 5 || *(sim_plmn + 5) == 0x30)
                    {
                        rtn = (memcmp(sim_plmn + 3, bcch_plmn + 3, 2) == 0) ? 1 : 0;
                    }
                    else
                    {
                        rtn = 0;
                    }
                }
                else
                {
                    rtn = (memcmp(sim_plmn + 3, bcch_plmn + 3, 2) == 0) ? 1 : 0;
                }
            }
            else
            {
                rtn = (memcmp(sim_plmn + 3, bcch_plmn + 3, 3) == 0) ? 1 : 0;
            }
        }
        else
        {
            rtn = 0;
        }
    }
    else
    {
        if (memcmp(sim_plmn, bcch_plmn, 3) == 0)
        {
            if (strlen((S8*) bcch_plmn) == 5)
            {
                rtn = (memcmp(sim_plmn + 3, bcch_plmn + 3, 2) == 0) ? 1 : 0;
            }
            else
            {
                rtn = (memcmp(sim_plmn + 3, bcch_plmn + 3, 3) == 0) ? 1 : 0;
            }
        }
        else
        {
            rtn = 0;
        }
    }

    return (MMI_BOOL) rtn;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_netset_is_t_mobile_us_sim
 * DESCRIPTION
 *  Check if the inserted SIM is T-Mobile
 * PARAMETERS
 *  void
 * RETURNS
 *  pBOOL
 *****************************************************************************/
pBOOL mmi_netset_is_t_mobile_us_sim(UINT8 sim_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    PS8 p_current_plmn_name;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (memcmp(gHomePLMNNum[sim_id], "310", 3) != 0)
    {
        return MMI_FALSE; /* not in United States */
    }

    p_current_plmn_name = GetCurrentPLMNName((PU8) gHomePLMNNum[sim_id]);

    if(p_current_plmn_name == NULL)
    {
        return MMI_FALSE;
    }
    
    if (pfnUnicodeStrcmp(p_current_plmn_name, (PS8) L"T-Mobile") == 0)
    {
        return MMI_TRUE;
    }
    else
    {
        return MMI_FALSE; /* not T-Mobile SIM */
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_netset_equivalent_plmn_check
 * DESCRIPTION
 *  Check if VPLMN and HPLMN is mapping to the same operator name string
 * PARAMETERS
 *  void
 * RETURNS
 *  pBOOL
 *****************************************************************************/
pBOOL mmi_netset_equivalent_plmn_check(UINT8 sim_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* gHomePLMNNum (Home PLMN) and gPLMN (camp on PLMN) are equivalent */
    U8 plmn_str_1[MAX_PLMN_LEN_MMI], plmn_str_2[MAX_PLMN_LEN_MMI + 1];
    PS8 p_plmn_str1, p_plmn_str2, p_g_plmn_str;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    memcpy(plmn_str_1, gHomePLMNNum[sim_id], 5);    /* assumed that Home PLMN is 5-digit */
    plmn_str_1[5] = 0;
    memcpy(plmn_str_2, gHomePLMNNum[sim_id], 6);    /* assumed that Home PLMN is 6-digit */
    plmn_str_2[6] = 0;

    p_plmn_str1 = GetCurrentPLMNName(plmn_str_1);
    p_plmn_str2 = GetCurrentPLMNName(plmn_str_2);

    if ((p_g_plmn_str = GetCurrentPLMNName(gPLMN[sim_id])) == NULL)
    {
        return MMI_FALSE;
    }

    if ((p_plmn_str1 != NULL) && (pfnUnicodeStrcmp(p_plmn_str1, p_g_plmn_str) == 0))
    {
        return MMI_TRUE;
    }
    else if ((p_plmn_str2 != NULL) && (pfnUnicodeStrcmp(p_plmn_str2, p_g_plmn_str) == 0))
    {
        return MMI_TRUE;
    }
    else
    {
        return MMI_FALSE;
    }
}

/**************************************************************


	FUNCTION NAME		: ChangeRoamingState

  	PURPOSE				: Change Roaming state. 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 

**************************************************************/
void ChangeRoamingState(UINT8 sim_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_BOOL plmn_match_by_spec_criteria = 0;
    MMI_BOOL pnn_equals_to_opn = 0;
    MMI_BOOL cmcc_sim_in_cmcc_networks = 0;
    MMI_BOOL t_mobile_sim_in_t_mobile_network = 0;
    MMI_BOOL same_pnn_by_eons = 0;
    MMI_BOOL same_pnn_by_phone_table = 0;
    MMI_BOOL is_in_roaming = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    plmn_match_by_spec_criteria = mmi_netset_plmn_matching_criteria((U8*)gHomePLMNNum[sim_id], gPLMN[sim_id], sim_id);
     
    pnn_equals_to_opn = (pfnUnicodeStrcmp((S8*)gNWProviderName[sim_id], (S8*)gOperatorName) == 0);
    cmcc_sim_in_cmcc_networks =
        ((memcmp(gHomePLMNNum[sim_id], "46002", 5) == 0 
        || memcmp(gHomePLMNNum[sim_id], "46007", 5) == 0
        || memcmp(gHomePLMNNum[sim_id], "46000", 5) == 0) 
        && /* CMCC SIMs */
         ((memcmp(gPLMN[sim_id], "46000", 5) == 0)
         || memcmp(gPLMN[sim_id], "46002", 5) == 0)); /* camp on CMCC network */
    t_mobile_sim_in_t_mobile_network =
        (mmi_netset_is_t_mobile_us_sim(sim_id) == MMI_TRUE && memcmp(gPLMN[sim_id], "31017", 5) == 0);
#ifdef __MMI_EONS_SUPPORT__
    same_pnn_by_eons = mmi_eons_check_roaming();
#else
    same_pnn_by_eons = MMI_FALSE;
#endif
    //same_pnn_by_phone_table = mmi_netset_equivalent_plmn_check(sim_id);

    is_in_roaming =
        !plmn_match_by_spec_criteria &&
        !pnn_equals_to_opn &&
        !cmcc_sim_in_cmcc_networks &&
        !t_mobile_sim_in_t_mobile_network &&
        !same_pnn_by_eons &&
        !same_pnn_by_phone_table;

    mmi_trace(1, "func :%s is_in_roaming %d, %d, %d ,%d, %d, %d ,%d ,simid %d, gplmn %s, hplmn %s",
        __FUNCTION__,
        is_in_roaming,
        plmn_match_by_spec_criteria, pnn_equals_to_opn, cmcc_sim_in_cmcc_networks,
        t_mobile_sim_in_t_mobile_network, same_pnn_by_eons, same_pnn_by_phone_table, sim_id, gPLMN[sim_id], gHomePLMNNum[sim_id]);
    
    if (mmi_bootup_is_roaming(sim_id))
    {        
        g_pwr_context[sim_id].PowerOnSIMStatus |= MMI_SIM_STATUS_ROAMING;
        IdleSetStatusIcon(STATUS_ICON_ROAMING + sim_id);

        if (IS_IDLE_ARRIVED)
        {
            StopLEDPatternRoaming();     
        }
    }
    else
    {        
        g_pwr_context[sim_id].PowerOnSIMStatus &= ~MMI_SIM_STATUS_ROAMING;
        IdleResetStatusIcon(STATUS_ICON_ROAMING + sim_id);
        
        if (IS_IDLE_ARRIVED)
        {
            StopLEDPatternRoaming();     
        }
    }

#ifdef __MMI_OP01_DCD__
	if (0 != memcmp("460", gPLMN, 3))
	{
		mmi_dcd_network_indication(MMI_TRUE);
	}
	else
	{
		mmi_dcd_network_indication(MMI_FALSE);
	}
#endif /* __MMI_OP01_DCD__ */

}

pBOOL mmi_bootup_is_roaming(UINT8 sim_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* replace g_pwr_context.RoamingFlag */
    mmi_trace(1, "func:%s, sim_id = %d, PowerOnSIMStatus = %d", __FUNCTION__, sim_id, g_pwr_context[sim_id].PowerOnSIMStatus);
    if (g_pwr_context[sim_id].PowerOnSIMStatus & MMI_SIM_STATUS_ROAMING)
    {
        return MMI_TRUE;
    }
    else
    {
        return MMI_FALSE;
    }
}

/**************************************************************


	FUNCTION NAME		: RspToTestRoamingEnabled

  	PURPOSE				: To send message MSG_ID_MMI_SMU_GET_IMSI_REQ
						  in response of this message imsi field is returned
						  compare first 5byte of this arr , if they are not same 
						  with the plmn , means roaming is enabled, display the 
						  roaming icon. 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void *

	RETURNS				: void

 

**************************************************************/
extern VOID CFW_GetMNCLen(UINT8 *pLen,CFW_SIM_ID nSimID);
void RspToTestRoamingEnabled(void *inMsg)
{
#ifndef MMI_ON_WIN32
    U8 nSimID;
    MMISMUGETIMSIRSP *local_data = (MMISMUGETIMSIRSP*)inMsg;
    U8 plmnlen;
    
    //PRINT_INFORMATION(("Inside RspToTestRoamingEnabled"));

    nSimID = MTPNP_AD_get_protocol_event_src_mode();
    mmi_trace(g_sw_NW, "RspToTestRoamingEnabled result=%d nSimID = %d", local_data->result, nSimID);

    if(local_data->result == ST_SUCCESS )
    {
        g_pwr_context[nSimID].testRoaming = 3;

        memset(gHomePLMNNum[nSimID], '\0', sizeof(gHomePLMNNum[nSimID]));
        //modified by zhoumn for bug 10437 @2007/04/06>>>start
#if 0
        memcpy(gHomePLMNNum[nSimID],(local_data->imsi) +1, MAX_PLMN_LEN_MMI);
#else
        CFW_GetMNCLen(&plmnlen, nSimID);
        plmnlen+=3;
        if(plmnlen > MAX_PLMN_LEN_MMI)
        {
            plmnlen = MAX_PLMN_LEN_MMI;
        }
        memcpy(gHomePLMNNum[nSimID], local_data->imsi, plmnlen);
#endif
        //modified by zhoumn for bug 10437 @2007/04/06<<<end
        mmi_trace(g_sw_NW, "RspToTestRoamingEnabled->gHomePLMNNum=%s, plmnlen = %d", gHomePLMNNum[nSimID], plmnlen);
#ifdef MMS_SUPPORT
        setMmscVersion((const U8*) gHomePLMNNum[nSimID]);
#endif

#ifdef __MMI_EONS_SUPPORT__
         
        //memset(&gEONSPLMNInfo[0], 0, sizeof(EONSPLMNINFO)*MAX_OPN_PNN_RECORD);
        if( is_sim_replaced())
            eons_get_opn_file_info_req();
        else
            eons_read_opn_from_nvram();
#endif

        if( g_pwr_context[nSimID].CurrentServiceType == FULL_SERVICE )
        {   
            ChangeRoamingState(nSimID);
        }
    }
    else
    {
        g_pwr_context[nSimID].testRoaming = 1;
    }

    if(g_idle_context.IsOnIdleScreen)
    {
        EntryIdleScreen();
    }
#endif
}




/*****************************************************************************
* FUNCTION
*	 mmi_ready_to_idle_screen_ind_hdlr()
* DESCRIPTION
*   ready go to idle handler
* 	 we use this message to enter idle screen, this is used to solve NVRAM/timer
*	 interlace problem
* PARAMETERS
*	 mgs			IN					message
* RETURNS
*   BOOL
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_ready_to_idle_screen_ind_hdlr(void* msg)
{
	TRACE_SYS_FUNCTION();

	/* there might be an very critail timing issue here */
	
   

	/*
	   (PS. set Tab=3 to view the following diagram correctly)
	   
		(1) =======================================================

		[Sim ready, send SIM_READY_IND to MMI]
		

		PS			  ------------------------------------------------
	
									(SIM ready)
		SIM 		  ----------*-------------------------------------
									| SIM_RDY_IND
									v
		PWRON HDLR ------------------------------------------------
		                     

		MT SCREEN  ------------------------------------------------


		[MSG Queue: (1)SIM_RDY_INC]
	
		===========================================================


		(2) =======================================================

		[Before SIM_RDY_IND arrive MMI, MT Call happened]
                             (MT Call)
		PS			  ------------*-----------------------------------
									  | MT_CALL_IND
		                       |
		SIM 		  ----------*-|-----------------------------------
									| |
									v |
		PWRON HDLR ------------|-----------------------------------
									  |				               			
									  v				               			
		MT SCREEN  ------------------------------------------------	


		[MSG Queue: (1)SIM_RDY_INC, (2) MT_CALL_IND]
		===========================================================


		(3) =======================================================

		[Then we will process SIM_RDY_INC, it will call this function 
		 to send MMI_RDT_TO_IDLE_SCR_IND to it self]

		 		 
                             (MT Call)
		PS			  ------------*-----------------------------------
									  | MT_CALL_IND
		                       |
		SIM 		  ----------*-|-----------------------------------
									| |
									| |
		PWRON HDLR ----------#-|-----------------------------------
									  |				               			
									  v
		MT SCREEN  ------------------------------------------------	


		[MSG Queue: (1) MT_CALL_IND (2) MMI_RDT_TO_IDLE_SCR_IND ]
		===========================================================

		(4) Then will proceed MT_CALL_IND first, it will enter MT call screen.
		(5) But it will follow up by proceeding MMI_RDT_TO_IDLE_SCR_IND, it 
		    will force to enter idle screen. So the MT call screen will gone..

		[Soultion]
		We have to check if it is already in MT call screen. If yes, we do not send 
	 	MSG_ID_MMI_READY_TO_IDLE_SCREEN_IND.
		
	*/


	/* 
		the reason to send out this message, it to solve NVRAM/Timer interleaving problem 
	   while powering on.
	*/

	if(isInCall())
	{
		/* is in call, after call end, call management will lead to idle screen */
		MMI_TRACE((MMI_TRACE_G7_MISC, MMI_POWER_ON_CALL_BEFORE_IDLE));
		return;
	}
		
	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_POWER_ON_READY_TO_IDLE));

	ClearProtocolEventHandler(MSG_ID_MMI_READY_TO_IDLE_SCREEN_IND);	
	goto_opening_screen();
	
}

void mmi_ready_to_idle_screen_ind(void) // this function is call by MMI
{
#ifdef MMI_ON_HARDWARE_P
	MYQUEUE	Message;

	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_POWER_ON_SEND_MSG_2_MMI));

	mmi_trace(g_sw_MAINENTRY ,"MMI_IDLE: Func: %s ", __FUNCTION__);
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_MMI;
	Message.oslMsgId = MSG_ID_MMI_READY_TO_IDLE_SCREEN_IND;

	Message.oslDataPtr = (oslParaType *)NULL;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
	SetProtocolEventHandler(mmi_ready_to_idle_screen_ind_hdlr, MSG_ID_MMI_READY_TO_IDLE_SCREEN_IND);
#endif
}

#ifdef __MMI_MULTI_SIM__
void MTPNP_EntryScrnSimErrorScreen()
{
	U16 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_SimSuccessIndBuffered[i])
		{
			g_SimSuccessIndBuffered[i] = 0;
			MTPNP_AD_set_protocol_event_src_mode(i);
			if(g_pwr_context[i].SimQueryFailIndFlag)
			{
				SimQueryFailureRsp(&(gSMUFailIndStruct[i]));
			}
			else
			{
				SimPasswdReqResponse(&(gSMUPwdDataStruct[i]));
			}
			return;
		}
	}

	if(!MTPNP_PFAL_Is_All_SIM_Init())
		return;
	
	if(IS_IDLE_ARRIVED)
	{
		mmi_trace(g_sw_SMU,"MMI_SIM: Func: %s not first time to idle", __FUNCTION__);
		BeforeEntryIdleScr();
	}
	else
	{	
		mmi_trace(g_sw_SMU,"MMI_SIM: Func: %s first time to idle", __FUNCTION__);
		goto_opening_screen();
	}
}
#endif
/**************************************************************

	FUNCTION NAME		: SimPasswdReqResponse

  	PURPOSE				: This is called as a call back when 
						  MMI_SMU_PASSWORD_REQUIRED_IND is received
 

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/
void SimPasswdReqResponse(void *inMsg)
{
	static U8 CodeVerifyRequiredFlag[MMI_SIM_NUMBER] = {0,};
	U8 nSimID;
	U16 i;
	BOOL bFound = MMI_FALSE;

	mmi_smu_password_required_ind_struct *local_data = (MMISMUPASSWDREQIND *)inMsg;

	nSimID = MTPNP_AD_get_protocol_event_src_mode();

	mmi_trace(g_sw_SMU, "MMI_SIM: SimPasswdReqResponse(%d) local_data->type=%d", nSimID,local_data->type);

	////////////////////////////////////////
	// Update SIM lock status.
	switch (local_data->type)
	{
	case MMI_TYPE_CHV_PASSED:
		#ifdef __MMI_MULTI_SIM__
		MTPNP_AD_Set_CardStatus(MTPNP_AD_SIMCARD_VALID);
		#endif
		g_SecuritySetupContext.SimLockStatus[nSimID] = CHV_VERIFIED;
		break;
	// TODO: 这个地方根本就不应该有MMI_SETTING_XXX的情况,因此需要去掉.
	case MMI_SETTING_TYPE_NO_REQUIRED:
	case MMI_TYPE_NO_REQUIRED:
		#ifdef __MMI_MULTI_SIM__
		MTPNP_AD_Set_CardStatus(MTPNP_AD_SIMCARD_VALID);
		#endif

		g_SecuritySetupContext.SimLockStatus[nSimID] = CHV_DISABLED;
		break;
	case MMI_TYPE_CHV1:
	case MMI_TYPE_UBCHV1:
		g_SecuritySetupContext.SimLockStatus[nSimID] = CHV_RETRY_ALLOW;
		break;
	case MMI_TYPE_PHONELOCK:
		//////////////////////////////////////////////////
		// Read phone lock setting at this time because MMI maybe has not complete it's some initialization operation (e.g. phone lock 
		// setting has not been load from NVRAM) before ADP send phone-lock IND to MMI.
		// On the other hand, it's a patch before we solve the MMI initialization issue.
		// TODO: 二次读.
		if (!ReadPhoneLockSetting())
			return;
		
		// Phone is locked, continue to process.
		break;
		
	default:	
		break;
	}

	// SIM not inserted or blocked.
	if (g_pwr_context[0].PrevScreenIndicator == ENTER_SIM_INSERTION || g_pwr_context[0].PrevScreenIndicator == ENTER_SIM_BLOCK)
	{
		mmi_trace(g_sw_SMU,"MMI_SIM:(ERROR) Func: %s g_pwr_context.PrevScreenIndicator=%d", __FUNCTION__, g_pwr_context[0].PrevScreenIndicator);
		return;
	}

	FillCountDownNumber(local_data->chv_info, nSimID);

	//micha1230
	gCPHSRetryCount = local_data->cphs_retry_count;

	if ((!g_pwr_context[0].PowerOnAnimationComplete)
#if defined(  __FS_CHECKDRIVE_SUPPORT__ )
			|| (SanityCheckDone == 0) || (SanityCheckDone == 3)
#endif
	   )
	{
		MMI_TRACE((MMI_TRACE_G7_MISC, MMI_POWER_ON_RECV_SIM_PSWD));
		g_pwr_context[nSimID].PasswdReqIndFlag=1;
		
		mmi_trace(g_sw_SMU, "MMI_SIM: Is in poweron animation screen. delay to process this type: %d", local_data->type);
		mmi_trace(g_sw_SMU, "MMI_SIM: buffer gSMUPwdDataStruct");

		 
		/* Current, MMI will postpone handling SMU_PASSWORD_REQUIRED_IND to align with power on animation */
		/* So will delay the IMSI retrieve, cause realize roaming status too late. */
		/* Duplicate here to get IMSI immediately once SIM password verified done. */


		if (local_data->type == MMI_TYPE_NO_REQUIRED || local_data->type == MMI_TYPE_CHV_PASSED || local_data->type == MMI_TYPE_CHV1)
		{
			memcpy(&(gSMUPwdDataStruct[nSimID]), local_data, sizeof(MMISMUPASSWDREQIND));
			g_pwr_context[nSimID].IsSimValid = 1;
			g_SimSuccessIndBuffered[nSimID] = 1;

			if ((g_pwr_context[nSimID].testRoaming == 1) && (g_pwr_context[nSimID].CurrentServiceType == FULL_SERVICE)) //Shall send GET IMSI REQ once
				RequestToTestRoamingEnabled();
		}
		else if (local_data->type == MMI_TYPE_PHONELOCK)
		{
			memcpy(&gSMUPwdDataStruct_phoneLock, local_data, sizeof(MMISMUPASSWDREQIND));
			gPhoneLockStore = 1;
		}
		else
		{
			memcpy(&(gSMUPwdDataStruct[nSimID]), local_data, sizeof(MMISMUPASSWDREQIND));
			g_SimSuccessIndBuffered[nSimID] = 1;
		}

		return;
	}

	/*wangrui add 2008.11.27 begin for fixbug10388    */
	if (g_pwr_context[0].PhoneLockProcesing == TRUE)//(GetActiveScreenId() == SCR_ENTER_PHONELOCK)//man
	{
		mmi_trace(g_sw_SMU, "MMI_SIM: Is processing phone lock. delay to process this type: %d, buffer gSMUPwdDataStruct", local_data->type);
		memcpy(&(gSMUPwdDataStruct[nSimID]), local_data, sizeof(MMISMUPASSWDREQIND));
		g_SimSuccessIndBuffered[nSimID] = 1;
		return;
	}


	if (g_pwr_context[0].PrevScreenIndicator == ENTER_PIN_SCR && nSimID != PinEnterSimID)
	{
		mmi_trace(g_sw_SMU, "MMI_SIM: pin input for sim%d going on, delay to handle this", PinEnterSimID);
		memcpy(&(gSMUPwdDataStruct[nSimID]), local_data, sizeof(MMISMUPASSWDREQIND));
		g_SimSuccessIndBuffered[nSimID] = 1;
		return;
	}

	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_POWER_ON_PROC_SIM_PSWD));
    /* Notify BT to start init bt ps stack and hw */
    mmi_bootup_sim_indication_bt_power_on(1, local_data->type);
	g_pwr_context[nSimID].PasswdReqIndFlag = 0;

	 
	StopLEDPatternPowerOn();

	ClearKeyEvents();
	memset((S8 *)gInputPwdBuffer, 0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
	memset((S8 *)gInputNewPinBufferFirstTime, 0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
	memset((S8 *)gInputNewPinBufferSecondTime, 0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
	 
	
#ifdef MMI_ON_HARDWARE_P
	if (local_data->type != 0)
	{
		FirstTimeTurnOnBackLight();
	}

#endif
	 
	g_pwr_context[nSimID].SimRemoveFlag = 0;
	g_pwr_context[nSimID].PowerOnTillIdleApp = 1;
	gKeyPadLockFlag = 0;
	g_pwr_context[0].PowerOnPeriod = 0;



#ifdef MMI_ON_HARDWARE_P
	/* 
	call this stop animation to stop all play power display, 
	which will stop all timer and avoid NVRAM & timer interlace problem 
	*/
	//mmi_pwron_stop_animation();  // zrx del 20060612 //delete by panxu

	//when local_data->type == MMI_TYPE_NO_REQUIRED, the mmi_pwron_stop_animation is called in mmi_ready_to_idle_screen_ind function
	if (local_data->type != MMI_TYPE_NO_REQUIRED)
	{
		mmi_pwron_stop_animation();
	}
#endif

	switch (local_data->type)
	{
	//2 NO_REQUIRED or PASSED
	case MMI_TYPE_NO_REQUIRED:
	case MMI_TYPE_CHV_PASSED:
	{
		#ifdef __MMI_MULTI_SIM__
		MTPNP_PFAL_Rearrange_StatusBar();
		#endif

//[221] Added by ZXB 2009/02/19								[BEGIN]
#ifdef __UT_SIMU_CARD2_ABSENT__
		UT_SimuCfwMsg(EV_CFW_INIT_IND, 0, 0, ADP_SIM_2 << 8 | CFW_INIT_STATUS_NO_SIM);
#endif
#ifdef __UT_SIMU_CARD1_ABSENT__
		UT_SimuCfwMsg(EV_CFW_INIT_IND, 0, 0, ADP_SIM_1 << 8 | CFW_INIT_STATUS_NO_SIM);
#endif
//[221] Added by ZXB 2009/02/19								[END]
		
		if (gSIMRefresh[nSimID] == 1)
		{
			g_phb_cntx.phb_ready = FALSE;    //CSD 2003-12-22 Wilson, for SIM refresh
			g_pwr_context[nSimID].IsSimValid = 1;

			if (IsInSATScreen())
				ExitVariables();  

			EntryIdleScreen();

			gSIMRefresh[nSimID] = 0;
			return;
		}

		// CSD end
		g_pwr_context[0].PowerOnPeriod = 1;
		g_pwr_context[0].PowerOnTillIdleApp = 0;
		g_pwr_context[nSimID].SSCFromEnterPukAtBootUp = 0;
		// gTypeNoReq = 1;
		g_pwr_context[nSimID].IsSimValid = 1;
		g_pwr_context[0].PrevScreenIndicator = 0;

			 
			/* Current, MMI will postpone handling SMU_PASSWORD_REQUIRED_IND to align with power on animation */
			/* So will delay the IMSI retrieve, cause realize roaming status too late. */
			/* Will duplicate to the top of this fucntion to get IMSI immediately once SIM password verified done. */
			if ((g_pwr_context[nSimID].testRoaming == 1) && (g_pwr_context[nSimID].CurrentServiceType == FULL_SERVICE)) //Shall send GET IMSI REQ once
			{
				RequestToTestRoamingEnabled();
			}	

			if (g_charbat_context.isChargerConnected == 1)
			{
				//Hide status icon temporary, entry idle screen will start animation again
			    HideStatusIcon(STATUS_ICON_BATTERY_STRENGTH);
				ShowStatusIcon(STATUS_ICON_BATTERY_STRENGTH);
				UpdateStatusIcons();
			}

		if (CodeVerifyRequiredFlag[nSimID])
		{
			CodeVerifyRequiredFlag[nSimID] = 0;
			EntryScrCodeVerified();
		}
		else
		{
			//DinitHistory(); // Not necessary to deinit history because there might be an incoming call

			#ifdef MMI_ON_HARDWARE_P
			/* send message to it slef notify ready to go to idle */
			for(i = 0; i < MMI_SIM_NUMBER; i++)
			{
				if(g_SimSuccessIndBuffered[i])
				{
					g_SimSuccessIndBuffered[i] = 0;
					MTPNP_AD_set_protocol_event_src_mode(i);
					if(g_pwr_context[i].SimQueryFailIndFlag)
					{
						SimQueryFailureRsp(&(gSMUFailIndStruct[i]));
					}
					else
					{
						SimPasswdReqResponse(&(gSMUPwdDataStruct[i]));
					}
					bFound = MMI_TRUE;
					break;
				}
			}
			if(!bFound
				#ifdef __MMI_MULTI_SIM__
				&&MTPNP_PFAL_Is_All_SIM_Init()
				#endif
				)
				goto_opening_screen();
			#else
				goto_opening_screen();
			#endif            
			}
			break;
		}
		
		case MMI_TYPE_PHONELOCK:
			#if 0 //chenhe comment it
			if (g_pwr_context.PrevScreenIndicator == ENTER_PHONELOCK)
			{
				EntryScrIncorrectPhoneLock();
			}
			else
				#endif
			{
				EntryScrEnterPhoneLock();

			if (!IS_IDLE_ARRIVED)
			{
#ifdef __MMI_USB_SUPPORT__
				PendingUsbDetectionHdlr(); /* USB 02/20 */
#endif
				PendingAlarmInPwronPeriod();

					if (g_charbat_context.isChargerConnected == 1)
					IdleScreenChargerConnected(KAL_FALSE);
				}
			}
			break;
		
		/* Show Enter PIN Screen */
		case MMI_TYPE_CHV1:				
			mmi_trace(g_sw_SMU, "MMI_SIM: Need to enter PIN1, g_pwr_context.PrevScreenIndicator is:%d",g_pwr_context[0].PrevScreenIndicator);
			if (g_pwr_context[0].PrevScreenIndicator == ENTER_PIN_SCR)
			{
				EntryScrIncorrectPin();
			}
			else
			{
				CodeVerifyRequiredFlag[nSimID] = 1;
				PinEnterSimID = MTPNP_AD_get_protocol_event_src_mode();
				EntryScrEnterPIN();

				if (!IS_IDLE_ARRIVED)
				{
					#ifdef __MMI_USB_SUPPORT__
					PendingUsbDetectionHdlr(); /* USB 02/20 */
					#endif
					PendingAlarmInPwronPeriod();

					if (g_charbat_context.isChargerConnected == 1)
						IdleScreenChargerConnected(KAL_FALSE);
				}
			}
			break;
		
		case MMI_TYPE_CHV2:
			PRINT_INFORMATION(("MMI_SIM: Enter PIN [MMI_TYPE_CHV2]"));
			break;
		
		case MMI_TYPE_UBCHV1:
			PRINT_INFORMATION(("MMI_SIM: MMI_TYPE_UBCHV1"));
			
			if ((g_pwr_context[0].PrevScreenIndicator == ENTER_CONFIRM_PIN) || (g_pwr_context[nSimID].SSCFromEnterPukAtBootUp))
				EntryScrIncorrectPUK();
			else
			{
				PUKEnterSimID = MTPNP_AD_get_protocol_event_src_mode();
				EntryScrPinBlocked();
				CodeVerifyRequiredFlag[nSimID] = 1;
			}
			
			break;

		case MMI_TYPE_UBCHV2:
			PRINT_INFORMATION(("MMI_SIM: MMI_TYPE_UBCHV2"));
			break;

		case MMI_TYPE_CHV1_BLOCKED:
			PRINT_INFORMATION(("MMI_SIM: MMI_TYPE_CHV1_BLOCKED"));
			break;
		
		case MMI_TYPE_CHV2_BLOCKED:
			 
			PRINT_INFORMATION(("MMI_SIM: MMI_TYPE_CHV2_BLOCKED"));
			break;
		
		case MMI_TYPE_SIM_CARD_BLOCKED:
			PRINT_INFORMATION(("MMI_SIM: MMI_TYPE_SIM_CARD_BLOCKED"));
			g_pwr_context[0].PowerOnTillIdleApp=0;
			InitInsertSimApp();
			RemoveStatusIconsForSimRemoved();
			#ifdef __MMI_MULTI_SIM__
			MTPNP_AD_Set_CardStatus(MTPNP_AD_SIMCARD_CHVBLOCKED);
			MTPNP_AD_Set_CardServiceType(MTPNP_AD_LIMITED_SERVICE, nSimID);
			MTPNP_PFAL_Rearrange_StatusBar();
			for(i = 0; i < MMI_SIM_NUMBER; i++)
			{
				if(g_SimSuccessIndBuffered[i])
				{
					g_SimSuccessIndBuffered[i] = 0;
					MTPNP_AD_set_protocol_event_src_mode(i);
					if(g_pwr_context[i].SimQueryFailIndFlag)
					{
						SimQueryFailureRsp(&(gSMUFailIndStruct[i]));
					}
					else
					{
						SimPasswdReqResponse(&(gSMUPwdDataStruct[i]));
					}
					bFound = MMI_TRUE;
					break;
				}
			}
			if(!bFound)
			{
				if(IS_IDLE_ARRIVED)
				{
					mmi_trace(g_sw_SMU,"MMI_SIM: Func: %s not first time to idle", __FUNCTION__);
					BeforeEntryIdleScr();
				}
				else
				{	
					mmi_trace(g_sw_SMU,"MMI_SIM: Func: %s first time to idle", __FUNCTION__);
					goto_opening_screen();
				}
			}
			#else
			EntryScrnSimBlockRsp();
			#endif

#ifdef __MMI_USB_SUPPORT__

		if (!IS_IDLE_ARRIVED)
		{
			PendingUsbDetectionHdlr();

				if (g_charbat_context.isChargerConnected == 1)
					IdleScreenChargerConnected(KAL_FALSE);
			}
		#endif
		break;
		
		case MMI_TYPE_UBCHV2_BLOCKED:
			 
			PRINT_INFORMATION(("MMI_SIM: MMI_TYPE_UBCHV2_BLOCKED"));
		break;
		
		case MMI_TYPE_UBCHV1_CHV1_NOT_LOCK:
			 
			PRINT_INFORMATION(("MMI_SIM: MMI_TYPE_UBCHV1_CHV1_NOT_LOCK"));
		break;
		
		case MMI_TYPE_NP:
			 
			PRINT_INFORMATION(("MMI_SIM: MMI_TYPE_NP"));
			if (g_pwr_context[0].PrevScreenIndicator == ENTER_NP)
			{
				//micha1230
				if (gCPHSRetryCount == 0) 
				{
					g_pwr_context[0].PowerOnTillIdleApp = 0;
					InitInsertSimApp();
					RemoveStatusIconsForSimRemoved();
                mmi_bootup_sim_indication_bt_power_on(1, MMI_TYPE_SIM_CARD_BLOCKED);
					EntryScrnSimBlockRsp();
				}
				else 
				{
					EntryScrIncorrectNP();
				}
			}
			else
			{
				EntryScrEnterNP();

			if (!IS_IDLE_ARRIVED)
				{
				#ifdef __MMI_USB_SUPPORT__
					PendingUsbDetectionHdlr(); /* USB 02/20 */
				#endif
					PendingAlarmInPwronPeriod();				

					if (g_charbat_context.isChargerConnected == 1)
						IdleScreenChargerConnected(KAL_FALSE);
				}
			}
		break;
		
		case MMI_TYPE_NSP:
			 
			PRINT_INFORMATION(("MMI_SIM: MMI_TYPE_NSP"));
			if (g_pwr_context[0].PrevScreenIndicator == ENTER_NSP)
			{
				//micha1230
				if (gCPHSRetryCount == 0) 
				{
					g_pwr_context[0].PowerOnTillIdleApp = 0;
					InitInsertSimApp();
					RemoveStatusIconsForSimRemoved();
                mmi_bootup_sim_indication_bt_power_on(1, MMI_TYPE_SIM_CARD_BLOCKED);
					EntryScrnSimBlockRsp();
				}
				else 
				{
					EntryScrIncorrectNSP();
				}
			}
			else
			{
				EntryScrEnterNSP();

			if (!IS_IDLE_ARRIVED)
				{
				#ifdef __MMI_USB_SUPPORT__
					PendingUsbDetectionHdlr(); /* USB 02/20 */
				#endif
					PendingAlarmInPwronPeriod();				

					if (g_charbat_context.isChargerConnected == 1)
						IdleScreenChargerConnected(KAL_FALSE);
				}
			}
		break;
		
		case MMI_TYPE_SP:
			 
			PRINT_INFORMATION(("MMI_SIM: MMI_TYPE_SP"));
			if (g_pwr_context[0].PrevScreenIndicator == ENTER_SP)
			{
				//micha1230
				if (gCPHSRetryCount == 0) 
				{
					g_pwr_context[0].PowerOnTillIdleApp = 0;
					InitInsertSimApp();
					RemoveStatusIconsForSimRemoved();
                mmi_bootup_sim_indication_bt_power_on(1, MMI_TYPE_SIM_CARD_BLOCKED);
					EntryScrnSimBlockRsp();
				}
				else 
				{
					EntryScrIncorrectSP();
				}
			}
			else
			{
				EntryScrEnterSP();

			if (!IS_IDLE_ARRIVED)
				{
				#ifdef __MMI_USB_SUPPORT__
					PendingUsbDetectionHdlr(); /* USB 02/20 */
				#endif
					PendingAlarmInPwronPeriod();				

					if (g_charbat_context.isChargerConnected == 1)
						IdleScreenChargerConnected(KAL_FALSE);
				}

			}
		break;
		
		case MMI_TYPE_CP:
			 
			PRINT_INFORMATION(("MMI_SIM: MMI_TYPE_CP"));
			if (g_pwr_context[0].PrevScreenIndicator == ENTER_CP)
			{
				//micha1230
				if (gCPHSRetryCount == 0) 
				{
					g_pwr_context[0].PowerOnTillIdleApp = 0;
					InitInsertSimApp();
					RemoveStatusIconsForSimRemoved();
                mmi_bootup_sim_indication_bt_power_on(1, MMI_TYPE_SIM_CARD_BLOCKED);
					EntryScrnSimBlockRsp();
				}
				else 
				{
					EntryScrIncorrectCP();
				}
			}
			else
			{
				EntryScrEnterCP();

			if (!IS_IDLE_ARRIVED)
				{
				#ifdef __MMI_USB_SUPPORT__
					PendingUsbDetectionHdlr(); /* USB 02/20 */
				#endif
					PendingAlarmInPwronPeriod();				

					if (g_charbat_context.isChargerConnected == 1)
						IdleScreenChargerConnected(KAL_FALSE);
				}

			}
		break;
		
		case MMI_TYPE_IMSI_LOCK:
			 
			PRINT_INFORMATION(("MMI_SIM: MMI_TYPE_IMSI_LOCK"));
                mmi_bootup_sim_indication_bt_power_on(1, MMI_TYPE_SIM_CARD_BLOCKED);
			if (g_pwr_context[0].PrevScreenIndicator == ENTER_SIMP)
			{
				//micha1230
				if (gCPHSRetryCount == 0) 
				{
					g_pwr_context[0].PowerOnTillIdleApp = 0;
                mmi_bootup_sim_indication_bt_power_on(1, MMI_TYPE_SIM_CARD_BLOCKED);
					InitInsertSimApp();
					RemoveStatusIconsForSimRemoved();
					EntryScrnSimBlockRsp();
				}
				else 
				{
					EntryScrIncorrectSIMP();
				}
			}
			else
			{
				EntryScrEnterSIMP();

			if (!IS_IDLE_ARRIVED)
				{
				#ifdef __MMI_USB_SUPPORT__
					PendingUsbDetectionHdlr(); /* USB 02/20 */
				#endif
					PendingAlarmInPwronPeriod();				

					if (g_charbat_context.isChargerConnected == 1)
						IdleScreenChargerConnected(KAL_FALSE);

				}

			}
		break;
		
		case MMI_TYPE_PERSONALIZATION_KEY:
			 
			PRINT_INFORMATION(("MMI_SIM: MMI_TYPE_PERSONALIZATION_KEY"));
		break;
		
		case MMI_TYPE_UNSPECIFIED:
			 
			PRINT_INFORMATION(("MMI_SIM: MMI_TYPE_UNSPECIFIED"));
		break;
		
		default:
			PRINT_INFORMATION(("MMI_SIM: Unhandled enum type in SimPasswdReqResponse"));
		break;
	}
}

void MMI_notify_bootup_ready(void) // this function is call by MMI
{
#ifdef MMI_ON_HARDWARE_P
       static BOOL haveBeenCalled = FALSE;
	MYQUEUE	Message;

       if (haveBeenCalled == FALSE)
       {
         haveBeenCalled = TRUE;
	  Message.oslSrcId=MOD_MMI;
	  Message.oslDestId=MOD_L4C;
	  Message.oslMsgId = MSG_ID_MMI_READY_NOTIFY_REQ;

	  Message.oslDataPtr = (oslParaType *)NULL;
	  Message.oslPeerBuffPtr= NULL;
	  OslMsgSendExtQueue(&Message);
       }
#endif
}
#include "smsapi.h"
BOOL mmi_is_int_finish(void)
{
	if(g_phb_cntx.phb_ReadyFinishi == FALSE)
	{
		mmi_trace(g_sw_SMU, "MMI_SIM: mmi_is_int_finish,phb not init done");
		return FALSE;
	}
	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX)==MMI_FRM_SMS_INVALID_INDEX)
	{
		mmi_trace(g_sw_SMU, "MMI_SIM: mmi_is_int_finish,sms not init done");
		return FALSE;
	}
	return TRUE;
}
BOOL mmi_is_pin_needed(U8 nSimID)
{
	if(g_SecuritySetupContext.SimLockStatus[nSimID] != CHV_DISABLED)
	{
		mmi_trace(g_sw_SMU, "MMI_SIM: mmi_is_pin_needed,pin needed");
		return TRUE;
	}
	return FALSE;
}
S8 nIndexSimRemoved = -1;
void mmi_reset_sim_rsp(void *inmsg)
{
#ifndef MMI_ON_WIN32
	U8 nSimID;
	U8 *pResult = (U8*)inmsg;
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	mmi_trace(g_sw_SMU, "MMI_SIM: sim reset result is %d",*pResult);
#ifdef MMI_ON_HARDWARE_P
	ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_RESET_SIM_RSP, nSimID);
#endif
	if(*pResult != TRUE)
	{
		InitInsertSimApp();
		RemoveStatusIconsForSimRemoved();
		g_pwr_context[nSimID].SimRemoveFlag = 1;
		#ifdef __MMI_MULTI_SIM__
		MTPNP_AD_Set_CardStatus(MTPNP_AD_SIMCARD_ABSENT);
		MTPNP_EntryScrnSimErrorScreen();
		nIndexSimRemoved = nSimID;
		#else
		simErrorDisplayString = SIM_ERROR_DISPLAY_INSERT_SIM;
		EntryScrnSimErrorScreen();
		#endif
	}
#endif
}
void mmi_reset_sim_req(U8 nSimID)
{

	MYQUEUE		Message;	 
#ifndef MMI_ON_WIN32
	Message.oslSrcId	=	MOD_MMI;	
	Message.oslDestId=	MOD_L4C + nSimID;
	Message.oslMsgId	=	PRT_MSG_ID_MMI_RESET_SIM_REQ;	
	Message.oslDataPtr	=	NULL;	
	Message.oslPeerBuffPtr	=	NULL;	
	SetProtocolEventHandlerExt(mmi_reset_sim_rsp, PRT_MSG_ID_MMI_RESET_SIM_RSP, nSimID);
	mmi_trace(g_sw_SMU, "MMI_SIM: send sim reset req for sim%d", nSimID);
	OslMsgSendExtQueue(&Message);
#endif
}

/**************************************************************

	FUNCTION NAME		: SimQueryFailureRsp

  	PURPOSE				: Call back for MMI_SMU_FAIL_IND 

	INPUT PARAMETERS	: void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/
#define __MMI_RESET_WHEN_SIM_REMOVED__
void SimQueryFailureRsp(void *inMsg)
{
	MMISMUFAILIND *local_data = (MMISMUFAILIND *)inMsg;
	U8 nSimID;

	mmi_trace(g_sw_SMU,"MMI_SIM: Func: %s local_data->cause=%d", __FUNCTION__, local_data->cause);

	nSimID = MTPNP_AD_get_protocol_event_src_mode();

#ifdef __MMI_MULTI_SIM__ 
	//if (local_data->cause == MMI_SIM_NO_INSERTED)
	//{
	//	MTPNP_AD_Set_CardStatus(MTPNP_AD_SIMCARD_ABSENT);
	//}
#endif

	if ((!g_pwr_context[0].PowerOnAnimationComplete)
#if defined(  __FS_CHECKDRIVE_SUPPORT__ )
			|| (SanityCheckDone == 0) || (SanityCheckDone == 3)
#endif
	   )
	{
		mmi_trace(g_sw_SMU, "MMI_SIM: Is in poweron animation screen. delay to process this type: %d", local_data->cause);
		g_pwr_context[nSimID].SimQueryFailIndFlag = 1;
		g_SimSuccessIndBuffered[nSimID] = 1;
		memcpy(&(gSMUFailIndStruct[nSimID]), local_data, sizeof(MMISMUFAILIND));
		return;
	}

	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_POWER_ON_PROC_SIM_FAIL));

	if (!g_pwr_context[0].PowerOnAnimationComplete)
	{
		g_pwr_context[nSimID].SimQueryFailIndFlag = 1;
		g_SimSuccessIndBuffered[nSimID] = 1;
		memcpy(&(gSMUFailIndStruct[nSimID]), local_data, sizeof(MMISMUFAILIND));
		return;
	}
	
	if (g_pwr_context[0].PhoneLockProcesing == TRUE) //(GetActiveScreenId() == SCR_ENTER_PHONELOCK)//man
	{
		mmi_trace(g_sw_SMU, "MMI_SIM: Is processing phone lock. delay to process this type: %d", local_data->cause);

		g_pwr_context[nSimID].SimQueryFailIndFlag = 1;
		g_SimSuccessIndBuffered[nSimID] = 1;
		memcpy(&(gSMUFailIndStruct[nSimID]), local_data, sizeof(MMISMUFAILIND));
		return;
	}

    mmi_bootup_sim_indication_bt_power_on(0, local_data->cause);
	g_pwr_context[nSimID].SimQueryFailIndFlag=0;
	g_pwr_context[0].PowerOnPeriod=0;

	 
    #ifdef __MMI_RUNNING_LED__ 
	StopLEDPatternPowerOn();
    #endif
	// Lisen 0607 reset IsSimValid
	g_pwr_context[nSimID].IsSimValid = 0;

	gKeyPadLockFlag=0;
	g_pwr_context[0].PowerOnTillIdleApp=0;
	ClearKeyEvents();
	#ifdef MMI_ON_HARDWARE_P
		FirstTimeTurnOnBackLight();
	#endif

	 
	
	switch(local_data->cause)
	{

	case MMI_SIM_CARD_REMOVED:
		PRINT_INFORMATION(("MMI_SIM_CARD_REMOVED"));
		 

		if (g_pwr_context[nSimID].SimRemoveFlag == 0)
		{
		#ifdef __MMI_RESET_WHEN_SIM_REMOVED__
			if(!mmi_is_int_finish() || mmi_is_pin_needed(nSimID))
			{
#ifndef MMI_ON_WIN32
				//restart phone
				mmi_trace(g_sw_SMU, "restart phone for sim removed");
			#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)      
				VDS_FlushCacheAll(); 
			#endif   
				DM_Reset();
#endif
			}
			else
			{
				//reset this sim
				mmi_trace(g_sw_SMU, "reset sim for sim removed");
				mmi_reset_sim_req(nSimID);
			}
		#else
			#ifdef __MMI_MULTI_SIM__
			if(MTPNP_PFAL_Is_CardValid(nSimID))
			#else
			if(g_pwr_context[0].IsSimValid)
			#endif
			{
				nIndexSimRemoved = nSimID;
			}
			InitInsertSimApp();
			RemoveStatusIconsForSimRemoved();
			g_pwr_context[nSimID].SimRemoveFlag = 1;
			#ifdef __MMI_MULTI_SIM__
			MTPNP_EntryScrnSimErrorScreen();
			#else
			simErrorDisplayString = SIM_ERROR_DISPLAY_INSERT_SIM;
			EntryScrnSimErrorScreen();
			#endif
		#endif
		}

		break;

	case MMI_SIM_ACCESS_ERROR:
		PRINT_INFORMATION(("MMI_SIM_ACCESS_ERROR"));
		// Lisen added: to signal SIM ACCESS ERROR
		InitInsertSimApp();
		RemoveStatusIconsForSimRemoved();

#ifdef __MMI_MULTI_SIM__
		MTPNP_EntryScrnSimErrorScreen();
#else
		simErrorDisplayString = SIM_ERROR_DISPLAY_SIM_ACCESS_ERROR;
		EntryScrnSimErrorScreen();
#endif
		break;

	case MMI_SIM_CMD_SUCCESS:
		PRINT_INFORMATION(("MMI_SIM_CMD_SUCCESS"));
		break;

	case MMI_SIM_CMD_FAIL:
		PRINT_INFORMATION(("MMI_SIM_CMD_FAIL"));
		 
		g_pwr_context[nSimID].SimRemoveFlag = 1;
		InitInsertSimApp();
		RemoveStatusIconsForSimRemoved();

#ifdef __MMI_MULTI_SIM__
		MTPNP_EntryScrnSimErrorScreen();
#else
		simErrorDisplayString = SIM_ERROR_DISPLAY_INSERT_SIM;
		EntryScrnSimErrorScreen();
#endif
		break;

	case MMI_SIM_FATAL_ERROR:
		PRINT_INFORMATION(("MMI_SIM_FATAL_ERROR"));
		// Lisen added: to signal SIM FATAL ERROR
		InitInsertSimApp();
		RemoveStatusIconsForSimRemoved();

#ifdef __MMI_MULTI_SIM__
		MTPNP_EntryScrnSimErrorScreen();
#else
		simErrorDisplayString = SIM_ERROR_DISPLAY_SIM_ACCESS_ERROR;
		EntryScrnSimErrorScreen();
#endif

		break;

	case MMI_SIM_NO_INSERTED:
		#ifdef __MMI_RESET_WHEN_SIM_REMOVED__
		#ifdef __MMI_MULTI_SIM__
		if(MTPNP_PFAL_Is_CardValid(nSimID))
		#else
		if(g_pwr_context[0].IsSimValid)
		#endif
		{
			if(!mmi_is_int_finish() || mmi_is_pin_needed(nSimID))
			{
			#ifndef MMI_ON_WIN32
				//restart phone
				mmi_trace(g_sw_SMU, "restart phone for sim removed");
			#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)      
				VDS_FlushCacheAll(); 
			#endif   
				DM_Reset();
			#endif
			}
			else
			{
				//reset this sim
				mmi_trace(g_sw_SMU, "reset sim for sim removed");
				mmi_reset_sim_req(nSimID);
			}
		}
		else
		#endif
		{
			PRINT_INFORMATION(("MMI_SIM_NO_INSERTED"));
			 
			g_pwr_context[nSimID].SimRemoveFlag = 1;
			InitInsertSimApp();
			RemoveStatusIconsForSimRemoved();
#ifdef __MMI_MULTI_SIM__
		#ifdef __MMI_MULTI_SIM__
			if(MTPNP_PFAL_Is_CardValid(nSimID))
		#else
			if(g_pwr_context[0].IsSimValid)
		#endif
			{
				nIndexSimRemoved = nSimID;
			}
			MTPNP_AD_Set_CardStatus(MTPNP_AD_SIMCARD_ABSENT);
			MTPNP_AD_Set_CardServiceType(MTPNP_AD_LIMITED_SERVICE, nSimID);
			MTPNP_PFAL_Rearrange_StatusBar();

			MTPNP_EntryScrnSimErrorScreen();
#else
			simErrorDisplayString = SIM_ERROR_DISPLAY_INSERT_SIM;
			EntryScrnSimErrorScreen();
#endif
		}
		break;

	case MMI_SIM_CHV_NOT_INIT:
		PRINT_INFORMATION(("MMI_SIM_CHV_NOT_INIT"));
		break;

	case MMI_SIM_CHV_RETRY_ALLOW:
		PRINT_INFORMATION(("MMI_SIM_CHV_RETRY_ALLOW"));
		break;

	case MMI_SIM_CHV_BLOCK:
		PRINT_INFORMATION(("MMI_SIM_CHV_BLOCK"));
		 
		InitInsertSimApp();
		RemoveStatusIconsForSimRemoved();
#ifdef __MMI_MULTI_SIM__
		MTPNP_EntryScrnSimErrorScreen();
#else
		EntryScrnSimBlockRsp();
#endif

		break;

	case MMI_SIM_ACCESS_NOT_ALLOW:
		PRINT_INFORMATION(("MMI_SIM_ACCESS_NOT_ALLOW"));
		break;

	case MMI_SIM_SAT_CMD_BUSY:
		PRINT_INFORMATION(("MMI_SIM_SAT_CMD_BUSY"));
		break;

	case MMI_SIM_DL_ERROR:
		PRINT_INFORMATION(("MMI_SIM_DL_ERROR"));
		break;

	case MMI_SIM_MEMORY_PROBLEM:
		PRINT_INFORMATION(("MMI_SIM_MEMORY_PROBLEM"));
		break;

	case MMI_SIM_TECHNICAL_PROBLEM:
		PRINT_INFORMATION(("MMI_SIM_TECHNICAL_PROBLEM"));
		break;
		 

	case MMI_SIM_REFRESH:
		gSIMRefresh[nSimID] = 1;
		break;
		// CSD end
		default:
			PRINT_INFORMATION(("Invalid cause from SIM FAIL error"));
			break;
	}

}
void CodeVerifiedPopupCallback(void)
{
	U16 i;
	mmi_trace(g_sw_SMU, "MMI_SIM: CodeVerifiedPopupCallback");
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_SimSuccessIndBuffered[i])
		{
			g_SimSuccessIndBuffered[i] = 0;
			MTPNP_AD_set_protocol_event_src_mode(i);
			if(g_pwr_context[i].SimQueryFailIndFlag)
			{
				SimQueryFailureRsp(&(gSMUFailIndStruct[i]));
			}
			else
			{
				SimPasswdReqResponse(&(gSMUPwdDataStruct[i]));
			}
			break;
		}
	}
}
/**************************************************************

	FUNCTION NAME		: EntryScrCodeVerified

  	PURPOSE				: PopUp when code is Verified.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void EntryScrCodeVerified(void)
{
	U16 i;
	BOOL needCallBack = MMI_FALSE;
	
	EntryNewScreen(SCR_CODE_VERIFY, ExitCodeVerifiedScr, NULL, NULL);

	DinitHistory();
	mmi_trace(g_sw_SMU, "MMI_SIM: EntryScrCodeVerified");
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_SimSuccessIndBuffered[i] == 1)
		{
			needCallBack = MMI_TRUE;
			break;
		}
	}
	if (needCallBack)
	{
		DisplayPopupCallBack((U8*)GetString(STR_CODE_VERIFIED),  IMG_GLOBAL_ACTIVATED, 
							MMI_FALSE, CODE_VERIFIED_TIMEOUT, (U8)SUCCESS_TONE, 
							CodeVerifiedPopupCallback);
	}
	else
	{
		ShowCategory62Screen(STR_CODE_VERIFIED, IMG_GLOBAL_ACTIVATED, 0);
		playRequestedTone(SUCCESS_TONE);
		StartTimer(CODE_VERIFIED_TIMER, CODE_VERIFIED_TIMEOUT, CodeVerifiedTimeoutFn);
		SetGroupKeyHandler(CodeVerifiedTimeoutFn, (PU16)PresentAllKeys, (U8)TOTAL_KEYS, (U16)KEY_EVENT_DOWN);
	}
}

/**************************************************************

	FUNCTION NAME		: ExitCodeVerifiedScr

  	PURPOSE			: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void ExitCodeVerifiedScr(void)
{
	history_t h;

	StopTimer(CODE_VERIFIED_TIMER);
	stopRequestedTone(SUCCESS_TONE);

	/* Add idle screen to history because DinitHistory is just called */
	pfnUnicodeStrcpy((S8*)h.inputBuffer, (S8*)L"");
	h.scrnID=GLOBAL_SCR_DUMMY;
	h.entryFuncPtr=mmi_ready_to_idle_screen_ind;
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
}

/**************************************************************

	FUNCTION NAME		: CodeVerifiedTimeoutFn

  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void CodeVerifiedTimeoutFn(void)
{
 
	StopTimer(CODE_VERIFIED_TIMER);
// CSD end
	mmi_ready_to_idle_screen_ind(); //goto_opening_screen();
}

/**************************************************************

	FUNCTION NAME		: ShowWelcomeScr

  	PURPOSE				: show Welcome screen

	INPUT PARAMETERS	: void 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/
BOOL g_welcom_scr = FALSE;
void ShowWelcomeScr(void)
{
    S8* WelcomeString=NULL;
	
	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_POWER_ON_START_WELCOME));

	EntryNewScreen(WELCOME_SCREEN_ID, ExitWelcomeScreen, ShowWelcomeScr, NULL);

	DeleteScreenIfPresent(GLOBAL_SCR_DUMMY);

#ifdef __MMI_NOKIA_STYLE_N800__
       TurnOnBacklight(1);
	   #endif
	 
	if(IsShowWelcomeScr1() && !g_welcom_scr)
	{
	    g_welcom_scr = TRUE;
		WelcomeString=GetWelcomeText();

		mmi_trace(g_sw_MAINENTRY,"MMI_IDLE: Func: %s show welcome screen", __FUNCTION__);
		#if defined(__MMI_APHORISM__)
/* under construction !*/
		#else
			StartTimer(WELCOME_SCR_TIMER,WELCOME_SCR_TIME,	BeforeEntryIdleScr);
		#endif

		ShowCategory61Screen((U8*)WelcomeString,IMG_WELCOME_SCR,NULL);
		OslMfree(WelcomeString);

		 
		ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
	}
	else
	{
		#if defined(__MMI_APHORISM__)
/* under construction !*/
		#else
			BeforeEntryIdleScr();
		#endif	
	}

}

/**************************************************************

	FUNCTION NAME		: ExitWelcomeScreen

  	PURPOSE				: exit Welcome screen

	INPUT PARAMETERS	: void 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/
void ExitWelcomeScreen(void)
{
	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_POWER_ON_STOP_WELCOME));
	StopTimer(WELCOME_SCR_TIMER);
}

 
#if defined(__MMI_APHORISM__)
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
/* under construction !*/
#endif //#if defined(__MMI_APHORISM__)
//CSD end

/**************************************************************

	FUNCTION NAME		: BeforeEntryIdleScr

  	PURPOSE				: fn to be called before entry into idle screen

	INPUT PARAMETERS	: void 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/
#ifdef __MMI_ENGINEER_MODE__
extern void CheckWhetherNeedAutoRunTest(void);
#endif
void BeforeEntryIdleScr(void)
{
//micha0923, #169
	U16 i;
	BOOL bNeedShowInvalidSimInfo = MMI_FALSE;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if (g_pwr_context[i].InvalidSim && !g_pwr_context[i].InvalidSimShowed)
		{
			bNeedShowInvalidSimInfo = MMI_TRUE;
			break;
		}
	}
	if (bNeedShowInvalidSimInfo) 
	{
		mmi_trace(g_sw_MAINENTRY,"MMI_IDLE: Func: %s g_pwr_context.InvalidSim=%d, g_pwr_context.InvalidSimShowed=%d", __FUNCTION__, g_pwr_context[i].InvalidSim, g_pwr_context[i].InvalidSimShowed);
		g_pwr_context[i].InvalidSimShowed = 1;
		DisplayInvalidSimInfo();
		DinitHistory(); // do not go back to power on animation
		ExitIdleScreen();
	}
	else
	{

/*JINZH Del Start For 6249 Ver: TBM780  on 2007-8-17 16:19 */
		#if 0
		if( isEarphonePlugged )  
		{
	           prevMode=AUD_MODE_HEADSET; //BY 1211
			SetModeSpeechVolume();
			ActivateHeadset(); 
		} 
		else if(gactivatedprofile==4)
		{
 		     prevMode=AUD_MODE_NORMAL; //BY 1211
			SendAudioModeRequest(prevMode); 
			SetModeSpeechVolume();  
			DeactivateHeadset();  
		}
		#endif
/*JINZH Del End  For 6249 Ver: TBM780  on 2007-8-17 16:19 */
#ifdef __MMI_ENGINEER_MODE__
#if defined(AUTO_TEST_KEYPRESS_RECORD_SUPPORT)
		CheckWhetherNeedAutoRunTest();
#endif
#endif
    
		EntryIdleScreen();
//		mmi_msg_init_msg_process();	// added by magesh 
	}
}

/**************************************************************

	FUNCTION NAME		: CheckValidEmergencyNo1

  	PURPOSE				: Checks for Emergency Nos in Different conditions

	INPUT PARAMETERS	: input Dialled Number

	OUTPUT PARAMETERS	: void

	RETURNS				: Bool vaue 

 

**************************************************************/
pBOOL CheckValidEmergencyNo1(S8 *tempbuffer)
{
	U8 i;
	if(pfnUnicodeStrlen(tempbuffer)!=0)
	{
//Lisen 04112005
		for(i=0;i<MAX_EMERGENCYNUMBER;i++)
		{
			if(pfnUnicodeStrcmp(tempbuffer, (PS8)EmergencyNumber[i])==0)
				return 1;
		}   

		if((g_pwr_context[0].PrevScreenIndicator != ENTER_SIM_BLOCK)&&(g_pwr_context[0].PrevScreenIndicator != ENTER_SIM_INSERTION))
		{
			if(CheckSIMSOSEntries(tempbuffer))
				return 1;
			else
				return 0;
		}
	}
	return 0;
}

/**************************************************************

	FUNCTION NAME		: CheckValidEmergencyNo1

  	PURPOSE				: Checks for Emergency Nos in Different conditions

	INPUT PARAMETERS	: input Dialled Number

	OUTPUT PARAMETERS	: void

	RETURNS				: Bool vaue 

 

**************************************************************/
void SimECCInfoSave(void *inMsg)
{
	mmi_smu_startup_info_ind_struct *local_data = (mmi_smu_startup_info_ind_struct *)inMsg;
	U8 Count=0;
	U8 i=0;
	
	if(local_data->num>0)
	{
		for(Count=0;Count<local_data->num;Count++)
		{
			while(local_data->ecc[i]!='\0')
			{
				gSimEccNumList[Count][i]=local_data->ecc[i];
				i++;
			}
			gSimEccNumList[Count][i]='\0';
	
			i=0;
		}
	}
#ifdef __MMI_AUTO_LANG_EN__

	/* Process and store LP in context */
	PhnsetPLPInd(local_data->language);

	/* If UsrLangIdx != 0xFF, not necessary to handle it because */
	/* 1. PhnsetSetLangByNvramValue() has been executed --> Language has already set */
	/* 2. PhnsetSetLangByNvramValue() did not executed --> */
	/*    PhnsetSetLangByNvramValue() will be execute later and Language will be set at that time */
	if(PhnsetGetLang()== 0xFF )
	{
		SetCurrentLanguage((S8)PhnsetGetAutoLangIdx()); /* Func in ReadRes.c */
	}
#endif

#ifdef __MANUAL_MODE_NW_SEL__
	PLMNGetSelectionModeReq();
#endif

}
/**************************************************************

	FUNCTION NAME		: CheckValidEmergencyNo1

  	PURPOSE				: Checks for Emergency Nos in Different conditions

	INPUT PARAMETERS	: input Dialled Number

	OUTPUT PARAMETERS	: void

	RETURNS				: Bool vaue 
	
 

**************************************************************/
pBOOL CheckSIMSOSEntries(S8* tempbuffer)
{
	U8 count=0;
	S8 SimEccNum[(EMERGENCY_NUMBER_LENGTH+1)*ENCODING_LENGTH];
	
	while(count<MAX_ECC_NUMBER_LIST_LENGTH)
	{
	       AnsiiToUnicodeString((S8*)SimEccNum, (S8*) (gSimEccNumList[count]));
	       
		if (pfnUnicodeStrcmp(tempbuffer, (S8*)SimEccNum)==0)
		  return 1;

		count++;
	}

	return 0;
}


/************************************************************

	FUNCTION NAME		: GetAphorismScrText

  	PURPOSE				: Get Aphorism Scr Text from NVRAM

	INPUT PARAMETERS	: void 

	OUTPUT PARAMETERS	: 

	RETURNS				: Aphorism text

 

**************************************************************/

S8* GetAphorismScrText(void)
{
#if 0 // zrx del 20060612
	U16 AphorismNo=rand() % MAX_APHORISM_LIST_LENGTH;

	AphorismNo=STR_APHORISM_TEXT_1 + AphorismNo;

	return GetString(AphorismNo);
#else
	return NULL;
#endif
}

/**************************************************************


	FUNCTION NAME		: SSCHandleEnterPUK

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SSCHandleEnterPUK(void)
{
	U16 Keycode,Keytype;
	byte *history_buffer;
	if(pfnUnicodeStrlen((S8*)gInputPwdBuffer)==0)
	{
		EntryNewScreen(SCR_SSC_ENTER_PUK,ExitSSCHandleEnterPUK, NULL, NULL);
		GetkeyInfo(&Keycode,&Keytype);
		#if 0
		sprintf(g_idle_context.DialPadCallBuffer,"");
		#else
		g_idle_context.DialPadCallBuffer[0] = 0;
		#endif
		g_idle_context.IdleScreenDigitKeyCode=Keycode;
		// TODO: need do something for multi sim
		g_pwr_context[0].SSCFromEnterPukAtBootUp=0;
		history_buffer=GetCurrGuiBuffer(SCR_SSC_ENTER_PUK);
		ShowCategory16Screen(0,0,STR_GLOBAL_BACK,0,(U8*)g_idle_context.DialPadCallBuffer, MAX_DIAL_PAD ,history_buffer);
		if(g_idle_context.IdleScreenDigitKeyCode>=0)
			ExecuteCurrKeyHandler(g_idle_context.IdleScreenDigitKeyCode,KEY_EVENT_DOWN);
		/* if come from setting PUK scr then should not go to idle scr on press on END --vanita*/
//micha0420
		if(IsScreenPresent(SCR_ID_SECSET_PUK1))
		{
			ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
			ClearKeyHandler(KEY_END,KEY_EVENT_UP);
		}

	//	SetKeyHandler(ValidateSSCStringPUK,KEY_POUND,KEY_EVENT_UP);//man for *
		
		SetCategory16RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		
	}
}

/**************************************************************


	FUNCTION NAME		: ExitSSCHandleEnterPUK

  	PURPOSE				: exit fn for SSC screen from PUK

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitSSCHandleEnterPUK(void)
{
	if(!g_pwr_context[0].SSCFromEnterPukAtBootUp)
	{
		history_t   Scr; /* Variable to hold the history data*/
		s16 nHistory = 0;

		Scr.scrnID = SCR_SSC_ENTER_PUK;
		Scr.entryFuncPtr = SSCHandleEnterPUK;
		pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
		GetCategoryHistory(Scr.guiBuffer);
		AddHistory(Scr);
	}
		g_idle_context.IdleScreenDigitKeyCode=-1;
}

/**************************************************************


	FUNCTION NAME		: ValidateSSCStringPUK

  	PURPOSE				: Validate is it a valid SSC for puk screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ValidateSSCStringPUK(void)
{
	g_pwr_context[0].SSCFromEnterPukAtBootUp=1;
	if(!SSCStringParsing3())
	{
		DisplayPopup((U8*)GetString(STR_INCORRECT_SSC),IMG_GLOBAL_WARNING,0,2000,(U8)ERROR_TONE);
//micha0412
		g_pwr_context[0].SSCFromEnterPukAtBootUp=0;
		DeleteScreenIfPresent(SCR_SSC_ENTER_PUK);
	}
}
/**************************************************************


	FUNCTION NAME		: MakeCallSSCStringPUK

  	PURPOSE				: make call for PUK screen SSC

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void MakeCallSSCStringPUK(void)
{
   MakeCall(g_idle_context.DialPadCallBuffer);
}

/**************************************************************


	FUNCTION NAME		: AddHistoryForBootUp

  	PURPOSE				: to add history in bootup

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void AddHistoryForBootUp(FuncPtr funcPtr)
{
	history_t   Scr; /* Variable to hold the history data*/
	s16 nHistory = 0;

	Scr.scrnID = SCR_POP_UP_HISTORY;
	Scr.entryFuncPtr = funcPtr;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);

}

/**************************************************************


	FUNCTION NAME		: ScrBootUpPasswordaValidation

  	PURPOSE				: Disable OK key

	INPUT PARAMETERS	: U8*,U8*,S32

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void ScrBootUpPasswordaValidation(U8* text,U8* cursor,S32 length)
{
	if((length) < MMI_MIN_PASSWORD_LENGTH)
		ChangeLeftSoftkey(0,0);
	else
		EnableLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
}

/**************************************************************


	FUNCTION NAME		: PUKLengthValidation

  	PURPOSE				: Disable OK key for PUK screen

	INPUT PARAMETERS	: U8*,U8*,S32

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PUKLengthValidation(U8* text,U8* cursor,S32 length)
{
	if((length) < MMI_MIN_PUK_LENGTH)
		ChangeLeftSoftkey(0,0);
	else
		EnableLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
}

// CSD added, for SIM refresh and abnormal reset 0912

/**************************************************************


	FUNCTION NAME		: GetAbnormalReset

  	PURPOSE				: get status of abnormal reset

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U8 GetAbnormalReset(void)
{
	return g_pwr_context[0].AbnormalReset;
}
/**************************************************************


	FUNCTION NAME		: SetAbnormalReset

  	PURPOSE				: Set abnormal reset flag

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetAbnormalReset(void)
{
	U16 i;
	g_pwr_context[0].AbnormalReset = 1;
 
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		g_pwr_context[i].IsCampOnTonePlayed=1;
	}
	g_pwr_context[0].PowerOnAnimationComplete = 1;
	g_pwr_context[0].PowerOnPeriod = 1;
	SanityCheckDone = 2;  
// CSD end
}

/**************************************************************

	FUNCTION NAME		: ResetAbnormalReset

  	PURPOSE				: Reset abnormal reset flag

	INPUT PARAMETERS	: U8*,U8*,S32

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ResetAbnormalReset(void)
{
	g_pwr_context[0].AbnormalReset = 0;
}

/**************************************************************


	FUNCTION NAME		: DisplayInvalidSimInfo

  	PURPOSE				: Display Invalid Sim info Popup

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DisplayInvalidSimInfo(void)
{
	U8 i;
	BOOL bPINBlocked =MMI_TRUE;
	
	EntryNewScreen(SCR_INVALID_SIM_INFO, NULL, DisplayInvalidSimInfo, NULL);
	
	ShowCategory2Screen (STR_SIM_INVALID_SIM_TITLE, 
	  						0,
							STR_GLOBAL_OK,IMG_GLOBAL_OK, 
							0, 0,  STR_SIM_INVALID_SIM_TXT, NULL);

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(!g_SecuritySetupContext.PINBlocked[i])
		{
			bPINBlocked = MMI_FALSE;
			break;
		}
	}
	if(bPINBlocked|| IsScreenPresent(IDLE_SCREEN_ID))
	{
		SetLeftSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	}
	else 
	{
		SetLeftSoftkeyFunction(EntryIdleScreen,KEY_EVENT_UP);
	}
}

/**************************************************************


	FUNCTION NAME		: RemoveStatusIconsForSimRemoved

  	PURPOSE				: Remove invalid status icon when sim is removed or blocked in running condition

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void RemoveStatusIconsForSimRemoved(void)
{
	U8 i;
	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_POWER_ON_REMOVE_ICONS));

#ifdef __MMI_MULTI_SIM__
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(!MTPNP_PFAL_Is_Card_Usable(i))
		{
			HideStatusIcon(STATUS_ICON_CALL_DIVERT_L1L2+i);
			HideStatusIcon(STATUS_ICON_MISSED_CALL+i);
			HideStatusIcon(STATUS_ICON_REJECTED_CALL+i);
			HideStatusIcon(STATUS_ICON_ROAMING + i);
		}
			
	}
#else  /* No Defined __MMI_MULTI_SIM__ */
	HideStatusIcon(STATUS_ICON_CALL_DIVERT_L1L2);
	HideStatusIcon(STATUS_ICON_MISSED_CALL);
	HideStatusIcon(STATUS_ICON_REJECTED_CALL);
#ifdef __MMI_CFU_SEPARATE_LINE__  
	HideStatusIcon(STATUS_ICON_CALL_DIVERT_L1);
	HideStatusIcon(STATUS_ICON_CALL_DIVERT_L2);
#endif
#endif /* __MMI_MULTI_SIM__ */
	HideStatusIcon(STATUS_ICON_INCOMING_SMS);
#ifndef __MMI_TOUCH_SCREEN__
	HideStatusIcon(STATUS_ICON_KEYPAD_LOCK);
#endif

 
#ifndef __MMI_SUBLCD_SHORT_STATUSICON_LIST__	
	HideStatusIcon( STATUS_ICON_SUBLCD_MISSED_CALL );   
	//HideStatusIcon( STATUS_ICON_SUBLCD_REJECTED_CALL );  
#endif
 
	HideStatusIcon(STATUS_ICON_UNREAD_VOICE_L1L2);
	HideStatusIcon(STATUS_ICON_UNREAD_VOICE_L1);
	HideStatusIcon(STATUS_ICON_UNREAD_VOICE_L2);
	HideStatusIcon(STATUS_ICON_UNREAD_FAX_L1L2);
	HideStatusIcon(STATUS_ICON_UNREAD_FAX_L1);
	HideStatusIcon(STATUS_ICON_UNREAD_FAX_L2);
	HideStatusIcon(STATUS_ICON_UNREAD_EMAIL_L1L2);
	HideStatusIcon(STATUS_ICON_UNREAD_EMAIL_L1);
	HideStatusIcon(STATUS_ICON_UNREAD_EMAIL_L2);
	HideStatusIcon(STATUS_ICON_SUBLCD_SMS);
	HideStatusIcon(STATUS_ICON_MMS_UNREAD);
	HideStatusIcon(STATUS_ICON_MMS_BUSY);
	HideStatusIcon(STATUS_ICON_CHAT_INDICATION_ROOM1);
	HideStatusIcon(STATUS_ICON_CHAT_INDICATION_ROOM2);
	HideStatusIcon(STATUS_ICON_WAP);
	HideStatusIcon(STATUS_ICON_NETWORK_CIPHER_GSM); 
	HideStatusIcon(STATUS_ICON_NETWORK_CIPHER_GPRS); 

#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__  
	ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH, IMG_SI_SIGNAL_STRENGTH);
#else
	HideStatusIcon(STATUS_ICON_ALTERNATE_LINE_L1);
	HideStatusIcon(STATUS_ICON_ALTERNATE_LINE_L2);
#endif

#ifdef __MMI_POC__
	HideStatusIcon(STATUS_ICON_POC); 
#endif


#if defined (__MMI_BT_SUPPORT__)
	HideStatusIcon(STATUS_ICON_BT);
	HideStatusIcon(STATUS_ICON_BT_CON);
	HideStatusIcon(STATUS_ICON_BT_ND);
	mmi_bt_pwroff();
#endif

#ifdef __MMI_IMPS__
	HideStatusIcon(STATUS_ICON_IMPS_MSG);
	HideStatusIcon(STATUS_ICON_IMPS_LOGIN);
#endif
	/*Wangzl Add Start For  Ver: TBM_780  on 2007-8-8 10:8 */
//by niej  for CS200 the SIM  icon need not dislayed, because only support one SIM 2007/08/14 begin
#if defined(DOUBLE_SIM_CARD)
	HideStatusIcon(STATUS_ICON_SIM_SWITCH);
	/*Wangzl Add End  For  Ver: TBM_780  on 2007-8-8 10:8 */
	#endif//by niej  for CS200 the SIM  icon need not dislayed, because only support one SIM 2007/08/14 end
	UpdateStatusIcons();
}


/**************************************************************


	FUNCTION NAME		: FirstTimeTurnOnBackLight

 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void FirstTimeTurnOnBackLight(void)
{
#if 0 // zrx del 20060612
	static U8 FirstTime=0;
	if(!FirstTime)
	{
		FirstTime=1;
		TurnOffBacklight();

		if( 1 == GetHandsetInPhone())  
		{
			ActivateHeadset();
/*Jinzh Del Start For 6249,6246 Ver: TBM780  on 2007-8-17 11:26 */
			#if 0
			SetModeSpeechVolume();
			#endif
/*Jinzh Del End  For 6249,6246 Ver: TBM780  on 2007-8-17 11:26 */
		} 
	}
	else
		TurnOnBacklight(1);
	#endif
}

extern pBOOL mmi_bootup_is_sim_valid();
void PowerOnBatteryIndicationCompletePhase2(void)
{
	U16 i;
#ifdef MMI_ON_HARDWARE_P
	FS_SetDiskFlag(); 
#endif
	if (gPhoneLockStore)
	{
		SimPasswdReqResponse(&gSMUPwdDataStruct_phoneLock);
		gPhoneLockStore = 0;
	}
	else
	{
		for(i = 0; i < MMI_SIM_NUMBER; i++)
		{
			if(g_SimSuccessIndBuffered[i])
			{
				g_SimSuccessIndBuffered[i] = 0;
				MTPNP_AD_set_protocol_event_src_mode(i);
				mmi_trace(g_sw_SYSTEM, "PowerOnBatteryIndicationCompletePhase2, sim pin req for sim %d", i);
				if(g_pwr_context[i].SimQueryFailIndFlag)
				{
					SimQueryFailureRsp(&(gSMUFailIndStruct[i]));
				}
				else
				{
					SimPasswdReqResponse(&(gSMUPwdDataStruct[i]));
				}
				return;
			}
		}
		#ifndef __MMI_ENTRY_IDLE_QUICKLY__
		if (mmi_bootup_is_sim_valid() == MMI_TRUE)
		#endif
		{
			goto_opening_screen();
		}
	
	}

}

#if defined(  __FS_CHECKDRIVE_SUPPORT__ )
extern pBOOL isInCall(void);
static void PWR_NAND_SanityCheck_Popup(void* info)
{
	DisplayPopupCallBack((U8*)GetString(STR_GLOBAL_DONE),IMG_GLOBAL_ACTIVATED,1,2000,(U8)SUCCESS_TONE,PWR_NAND_SanityCheck_response_handler);
	ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
	SanityCheckDone = 2;
//	DeleteScreenIfPresent(SCR_NAND_FLASH_CHECK);
	DeleteScreenIfPresent(SCR_NAND_CHECK_PROCESSING);
	FS_ClearDiskFlag(); 

}
void PWR_NAND_SanityCheck_response_handler(void)
{
	
	if (!isInCall())
		PowerOnBatteryIndicationCompletePhase2();
	else
		GoBackHistory();			
}


static void PWR_NAND_SanityCheck_skip_check(void)
{
	SanityCheckDone = 1;
	PowerOnBatteryIndicationCompletePhase2();
}

static void PWR_NAND_SanityCheck_proceed_check(void)
{
#ifdef MMI_ON_HARDWARE_P
	MYQUEUE Message;

	mmi_fmt_check_drive_req_strcut *req = (mmi_fmt_check_drive_req_strcut*)OslConstructDataPtr(sizeof(mmi_fmt_check_drive_req_strcut));

	EntryNewScreen(SCR_NAND_CHECK_PROCESSING, NULL, PWR_NAND_SanityCheck_proceed_check, NULL);

#ifdef __MMI_FILE_MANAGER__
	req->drive = MMI_PUBLIC_DRV;
#endif
	if(SanityCheckDone != 3)
	{
		SanityCheckDone = 3;//processing
       // Send message to FMT
		Message.oslSrcId = MOD_MMI;
		Message.oslDestId = MOD_FMT;
		Message.oslMsgId = MSG_ID_MMI_FMT_CHECK_DRIVE_REQ;
       // Message.oslDataPtr = NULL;
		Message.oslDataPtr =  (oslParaType *)req;
		Message.oslPeerBuffPtr= NULL;

		OslMsgSendExtQueue(&Message);
		SetProtocolEventHandler (PWR_NAND_SanityCheck_Popup, PRT_MMI_FMGR_CHECK_DRIVE_RSP);
	}
	ShowCategory66Screen(STR_NAND_CHECK_TITLE,0, 0, 0, 0, 0, (PU8)GetString(STR_NAND_CHECK_PROCESSING), IMG_GLOBAL_PROGRESS, NULL);
	ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);

#else //for pc simulator
	extern void EntryScrDisplayUSSNMsg(void);
	EntryNewScreen(SCR_NAND_CHECK_PROCESSING, NULL, PWR_NAND_SanityCheck_proceed_check, NULL);

	StopTimer(WELCOME_SCR_TIMER);

	if(SanityCheckDone != 3)
	{
		SanityCheckDone = 3;//processing
		StartTimer(WELCOME_SCR_TIMER,5000,PWR_NAND_SanityCheck_Popup);
	}
	ShowCategory66Screen(STR_NAND_CHECK_TITLE,0, 0, 0, 0, 0, (PU8)GetString(STR_NAND_CHECK_PROCESSING), IMG_GLOBAL_PROGRESS, NULL);
	ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);	
#endif

}

void PWR_NAND_SanityCheckDisplay_Exit (void)
{
   stopRequestedTone(WARNING_TONE);
}

extern void SetCat165TimerExpireFunc(FuncPtr f);
/**************************************************************
	FUNCTION NAME		: PWR_NAND_SanityCheckDisplay
  	PURPOSE				: 	1. display NAND check operation screen
  								2. pressing End key will come back here except during a call
	INPUT PARAMETERS	: 
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
 
**************************************************************/
static void PWR_NAND_SanityCheckDisplay(void)
{ 
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
   U8* guiBuffer;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

   guiBuffer = GetCurrGuiBuffer(SCR_NAND_FLASH_CHECK);
	//Some application might be performed during NAND check
	//if the user press end key, here will be the root entry function
	if( GetActiveScreenId()!=SCR_NAND_FLASH_CHECK )//root and only nand check screen
	{
		if(SanityCheckDone==0)
		{
	   	EntryNewScreen(SCR_NAND_FLASH_CHECK,PWR_NAND_SanityCheckDisplay_Exit,
	   		PWR_NAND_SanityCheckDisplay,NULL);


	   	ShowCategory165Screen_ext(STR_GLOBAL_YES, IMG_GLOBAL_YES, 
	   		STR_GLOBAL_NO, IMG_GLOBAL_NO,get_string(STR_NAND_CHECK),
		   	IMG_GLOBAL_QUESTION,10,guiBuffer);

		
	   	SetCat165TimerExpireFunc(PWR_NAND_SanityCheck_proceed_check);

		  playRequestedTone(WARNING_TONE);  //dyj del 20060514 //add by panxu 20061228

    #ifdef __MMI_RUNNING_LED__ 
	   	StopLEDPatternPowerOn();
    #endif
			//ShowSubLCDScreen(ShowIdleAppSubLcd);
			ShowIdleAppSubLcd();
				SetKeyHandler(PWR_NAND_SanityCheck_skip_check, KEY_END, KEY_EVENT_DOWN);
	      SetLeftSoftkeyFunction(PWR_NAND_SanityCheck_proceed_check, KEY_EVENT_UP);
	      SetRightSoftkeyFunction(PWR_NAND_SanityCheck_skip_check, KEY_EVENT_UP);
		}
		else//skip or done or processing
			PowerOnBatteryIndicationComplete();
	}
}

#endif
/**************************************************************


	FUNCTION NAME		: PowerOnBatteryIndicationComplete

  	PURPOSE				: 	1. Fn to be called after power on animatin is completed.
  								2. pressing End key in a call will come back here
	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PowerOnBatteryIndicationComplete(void)
{
       /* Lisen 04/25/2004: handle failure message first */
       g_pwr_context[0].PowerOnAnimationComplete=1;

#if defined(  __FS_CHECKDRIVE_SUPPORT__ )
#ifdef MMI_ON_HARDWARE_P
       if (FS_CheckDiskFlag())
#else
		if (1)
#endif
       {
       	if ( (SanityCheckDone == 0) || (SanityCheckDone == 3))
       	{
         	if(IsScreenPresent(SCR_NAND_CHECK_PROCESSING))
         	{
          		GoBackToHistory(SCR_NAND_CHECK_PROCESSING);
          	}
          	else if(IsScreenPresent(SCR_NAND_FLASH_CHECK))
          	{
          		GoBackToHistory(SCR_NAND_FLASH_CHECK);
          	}
          	else
	          	PWR_NAND_SanityCheckDisplay();

          	return;//wait for PRT_MMI_FMGR_CHECK_DRIVE_RSP
       	}
       }
       else
       	SanityCheckDone = 2;
#endif       
         PowerOnBatteryIndicationCompletePhase2();

/*
       if(g_pwr_context.SimQueryFailIndFlag)
	{
		StopLEDPatternPowerOn();
		SimQueryFailureRsp(&gSMUFailIndStruct);
	}
       else if(g_pwr_context.PasswdReqIndFlag)
	{
		StopLEDPatternPowerOn();
		SimPasswdReqResponse(&gSMUPwdDataStruct);
	}
*/
}

/**************************************************************


	FUNCTION NAME		: CallBackPowerOnAnimationComplete

  	PURPOSE				: Call back function when power on animation is completed

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CallBackPowerOnAnimationComplete(GDI_RESULT ret)
{
	BOOL bSimIndFlag = MMI_FALSE;
	U16 i;
	TRACE_SYS_FUNCTION();
	/* Low battery shutdown */
	if( ((g_charbat_context.batteryStatus == PMIC_VBAT_STATUS || g_charbat_context.batteryStatus == PMIC_CHARGER_OUT ) && g_charbat_context.isChargerConnected == 0) /* Battery Status */
		&& (g_charbat_context.chargerLevel == BATTERY_LOW_POWEROFF) /* Battery Level */ )
        {
        	TRACE_SYS_FUNC("ShowLowPowerShutdownScr");
	   ShowLowPowerShutdownScr();
	   return;
	}

	 
	if( ret < 0 || ret == GDI_IMAGE_IS_STILL_IMAGE )
	{
		/* Broken animation image - force enter */
		TRACE_SYS_FUNC("PowerOnBatteryIndicationComplete");
 		PowerOnBatteryIndicationComplete();
		return;
	}

	
	/* SIM ready/fail indication has been arrival */
	#ifndef __MMI_ENTRY_IDLE_QUICKLY__
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if( g_pwr_context[i].PasswdReqIndFlag || g_pwr_context[i].SimQueryFailIndFlag )
		{
			bSimIndFlag = MMI_TRUE;
		}
	}
	#else
		bSimIndFlag = MMI_TRUE;
	#endif
	if(bSimIndFlag)
	{
		TRACE_SYS_FUNC("( g_pwr_context.PasswdReqIndFlag || g_pwr_context.SimQueryFailIndFlag )");
		/* Low battery warning */
		if( g_charbat_context.chargerLevel == BATTERY_LOW_TX_PROHIBIT || g_charbat_context.chargerLevel == BATTERY_LOW_WARNING )
		{
        	EntryNewScreen(SCR_CHARGER_POPUP,ExitLowBatteryScrAtPowerOn, NULL,NULL);
        	ClearKeyEvents();
        	ShowCategory64Screen(STR_LOW_BATTERY,IMG_GLOBAL_WARNING,0);
        	StartTimer(LOW_BATTERY_SCR_TIMER,LOW_BATTERY_SCR_TIMEOUT, PowerOnBatteryIndicationComplete );
			
		}
		else
		{
			TRACE_SYS_FUNC("PowerOnBatteryIndicationComplete();");
			PowerOnBatteryIndicationComplete();
		}
	}
	 
}
/**************************************************************

	FUNCTION NAME		: ExitLowBatteryScrAtPowerOn

  	PURPOSE				: Exit Low battery screen when it appear just after power on animation

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 

**************************************************************/
void ExitLowBatteryScrAtPowerOn(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	s16 nHistory = 0;
	Scr.scrnID = SCR_CHARGER_POPUP;
	Scr.entryFuncPtr = PowerOnBatteryIndicationComplete;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
	AddHistory(Scr);
	StopTimer(LOW_BATTERY_SCR_TIMER);
}
/*************************************************************
					API's To Others
 **************************************************************/
 
/**************************************************************


	FUNCTION NAME		: GetHomePLMNName

  	PURPOSE				: to get PLMN name of given PLMN No

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
PS8 GetHomePLMNName(void)
{
	return GetCurrentPLMNName(gHomePLMNNum[0]);
}


//micha0120 begin

void UpdateGPRSStatusIcon(U8 gprs_state)
{
#if 0/* JIASHUO FOR WAP/MMS */
	static U8 gGPRSAttachStatus = 0;
	static U8 gGPRSPDPStatus = 0;
	static U8 gGPRSCellStatus = 0;

//micha0202, vito G display issue
	static U8 LastGPRSStatus = 0;   // 0: No icon,  1:  only attach,  2: attach+pdp active

	switch(gprs_state)	// l4c_gprs_status_enum
	{
		case L4C_GPRS_ATTACHED:
			gGPRSAttachStatus = 1;
			break;
		case L4C_GPRS_DETACHED:
			gGPRSAttachStatus = 0;
			break;
		case L4C_GPRS_COVERAGE:
			gGPRSCellStatus = 1;
			break;
		case L4C_NONE_GPRS:
			gGPRSCellStatus = 0;
//micha0317, John 
			gGPRSAttachStatus = 0;
 			break;
		case L4C_PDP_ACTIVED:
			gGPRSPDPStatus = 1;
			break;
		case L4C_PDP_DEACTIVED:
			gGPRSPDPStatus = 0;
 
//			GPRSStatusGetGPRSModeReq();
			break;
		default :
			break;
	}

//micha0603
	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "[Update G] Att:%d,PDP:%d,Cov:%d,Last:%d", 
									gGPRSAttachStatus, gGPRSPDPStatus, gGPRSCellStatus, LastGPRSStatus));

//micha0202, micha0224, vito display issue
	if (gGPRSAttachStatus && gGPRSPDPStatus && gGPRSCellStatus) {
		if (LastGPRSStatus != 2) {
		HideStatusIcon(STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR);
		ShowStatusIcon(STATUS_ICON_GPRS_SERVICE);
		LastGPRSStatus = 2;
		gGPRSStatusModified = 1;
	}
	}
	else if (gGPRSAttachStatus && !gGPRSPDPStatus && gGPRSCellStatus) {
		if (LastGPRSStatus != 1) {
		HideStatusIcon(STATUS_ICON_GPRS_SERVICE);
		ShowStatusIcon(STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR);
		LastGPRSStatus = 1;
		gGPRSStatusModified = 1;
	}
	}
	else  {
		if ( LastGPRSStatus != 0){
		HideStatusIcon(STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR);
		HideStatusIcon(STATUS_ICON_GPRS_SERVICE);
		LastGPRSStatus = 0;
		gGPRSStatusModified = 1;
	}	
	UpdateStatusIcons();
}
#else
	mmi_trace(g_sw_NW, "UpdateGPRSStatusIcon->gprs_state:%d",gprs_state);
	switch(gprs_state)	// l4c_gprs_status_enum
	{
		case L4C_GPRS_ATTACHED:
			HideStatusIcon(STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR);
	//		ShowStatusIcon(STATUS_ICON_GPRS_SERVICE);
			break;
		case L4C_GPRS_DETACHED:
			HideStatusIcon(STATUS_ICON_GPRS_SERVICE);
			HideStatusIcon(STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR);
			break;
		case L4C_GPRS_COVERAGE:
			break;
		case L4C_NONE_GPRS:
 			break;
		case L4C_PDP_ACTIVED:
			HideStatusIcon(STATUS_ICON_GPRS_SERVICE);
			ShowStatusIcon(STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR);
			break;
		case L4C_PDP_DEACTIVED:
			HideStatusIcon(STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR);
			HideStatusIcon(STATUS_ICON_GPRS_SERVICE);            
			//ShowStatusIcon(STATUS_ICON_GPRS_SERVICE);
			break;
		default :
			break;
	}
	UpdateStatusIcons();
#endif
}

void UpdateGPRSStatusIconExt(U8 gprs_state, U8 nSimID)
{

	mmi_trace(g_sw_NW, "UpdateGPRSStatusIcon->gprs_state:%d",gprs_state);
   #if 0// #ifdef __MMI_NOKIA_STYLE_N800__
	switch(gprs_state)
	{
		case L4C_GPRS_ATTACHED:
			HideStatusIcon(STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR +1 );
			ShowStatusIcon(STATUS_ICON_GPRS_SERVICE);
			break;
		case L4C_GPRS_DETACHED:
		case L4C_NONE_GPRS:
			HideStatusIcon(STATUS_ICON_GPRS_SERVICE +1);
			HideStatusIcon(STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR +1);
			break;
		case L4C_GPRS_COVERAGE:
			break;
		case L4C_PDP_ACTIVED:
			HideStatusIcon(STATUS_ICON_GPRS_SERVICE +1);
			ShowStatusIcon(STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR +1);
			break;
		case L4C_PDP_DEACTIVED:
			HideStatusIcon(STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR +1);
			HideStatusIcon(STATUS_ICON_GPRS_SERVICE +1);            
			break;
		default :
			break;
	}

    #else
	switch(gprs_state)
	{
		case L4C_GPRS_ATTACHED:
			HideStatusIcon(STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR + nSimID);
			//ShowStatusIcon(STATUS_ICON_GPRS_SERVICE);
			break;
		case L4C_GPRS_DETACHED:
		case L4C_NONE_GPRS:
			HideStatusIcon(STATUS_ICON_GPRS_SERVICE + nSimID);
			HideStatusIcon(STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR + nSimID);
			break;
		case L4C_GPRS_COVERAGE:
			break;
		case L4C_PDP_ACTIVED:
			HideStatusIcon(STATUS_ICON_GPRS_SERVICE + nSimID);
			ShowStatusIcon(STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR + nSimID);
			break;
		case L4C_PDP_DEACTIVED:
			#ifdef CFW_MULTI_SIM
			if(!CFW_isPdpActiveAvailable(nSimID))
			#else
			if(!CFW_isPdpActiveAvailable())
			#endif
			{
			HideStatusIcon(STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR + nSimID);
			HideStatusIcon(STATUS_ICON_GPRS_SERVICE + nSimID);            
			}
			//ShowStatusIcon(STATUS_ICON_GPRS_SERVICE);
			break;
		default :
			break;
	}
    #endif
	UpdateStatusIcons();
}

void GPRSStatusUpdateIndication(void *info)
{
#ifdef __MMI_GPRS_FEATURES__
	U8 nSimID;
	mmi_ps_gprs_status_update_ind_struct *local_data = (mmi_ps_gprs_status_update_ind_struct *)info;
	PRINT_INFORMATION(("\n GPRSStatusUpdateIndication\n"));
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	UpdateGPRSStatusIconExt(local_data->status, nSimID);

	if(gGPRSStatusModified )
	{
		UpdateStatusIcons();
		gGPRSStatusModified = 0;
	}
	
#endif	
}

void GPRSStatusGetGPRSModeReq(void)
{
//micha1020
#ifdef __MMI_GPRS_FEATURES__
	MYQUEUE msgStruct;
	
	SetProtocolEventHandler(GPRSStatusGetGPRSModeRsp, MSG_ID_MMI_NW_GET_GPRS_CONNECT_TYPE_RSP);
	/* format the message to get band selection */
	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MOD_L4C;
	msgStruct.oslMsgId = (unsigned int)MSG_ID_MMI_NW_GET_GPRS_CONNECT_TYPE_REQ;
	msgStruct.oslSapId = (unsigned int)0;
	msgStruct.oslDataPtr = NULL;
	msgStruct.oslPeerBuffPtr= NULL;
	 
	OslMsgSendExtQueue(&msgStruct);
	return;
#endif
}

void GPRSStatusGetGPRSModeRsp(void *info) 
{
//micha1020
#ifdef __MMI_GPRS_FEATURES__
	mmi_nw_get_gprs_connect_type_rsp_struct* localPtr;

	localPtr = (mmi_nw_get_gprs_connect_type_rsp_struct *)info;

//micha0202 begin
	if(localPtr->result == MMI_TRUE)
	{
		if (localPtr->type == L4C_WHEN_NEEDED) {
			GPRSStatusDetachGPRSReq();
	}
	}
//micha0202 end	
#endif
}

void GPRSStatusDetachGPRSReq(void)
{
//micha1020
#ifdef __MMI_GPRS_FEATURES__
	MYQUEUE msgStruct;
	mmi_nw_set_attach_req_struct* local_data;
	
	SetProtocolEventHandler(GPRSStatusDetachGPRSRsp, MSG_ID_MMI_NW_SET_ATTACH_RSP);
	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MOD_L4C;
	msgStruct.oslMsgId = MSG_ID_MMI_NW_SET_ATTACH_REQ;  
	local_data = (mmi_nw_set_attach_req_struct*) OslConstructDataPtr(sizeof (mmi_nw_set_attach_req_struct));
	local_data->is_poweroff = MMI_FALSE;
	local_data->opcode = RAC_DETACH;
	local_data->type = RAC_PS;
	msgStruct.oslDataPtr = (oslParaType *)local_data;
	msgStruct.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&msgStruct);
	return;
#endif
}


void GPRSStatusDetachGPRSRsp(void *info)
{
	// do nothing
}

 //micha0120 end

/**************************************************************************
	FUNCTION NAME	: InsertSimRSKPressOnScreenSaver
  	PURPOSE		: Handle RSK pressed in keypad locked insert SIM screen
**************************************************************************/
void InsertSimRSKPressOnScreenSaver(void)
{
	g_idle_context.IsRSKPressOnScreenSaver=1;
	if( IsScreenPresent(SCR_SIM_BLOCK) || GetActiveScreenId() == SCR_SIM_BLOCK )
		EntryScrnSimBlockRsp();
	else
		EntryScrnSimErrorScreen();
}

/**************************************************************************
	FUNCTION NAME	: InsertSimSetKeyPadLockFlag
  	PURPOSE		: Timeout to lock keypad in insert SIM screen
**************************************************************************/
void InsertSimSetKeyPadLockFlag(void)
{
	gKeyPadLockFlag = 1;	

#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_block();
#endif

#ifdef __MMI_KEYPADLOCK_WITH_KEYTONE__
      
#else 
       mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);
#endif

	if( IsScreenPresent(SCR_SIM_BLOCK) || GetActiveScreenId() == SCR_SIM_BLOCK )
		EntryScrnSimBlockRsp();
	else
		EntryScrnSimErrorScreen();

#ifdef __MMI_TOUCH_SCREEN__
	SetKeyHandler(InsertSimRSKPressOnScreenSaver, KEY_SEND, KEY_EVENT_DOWN);
#else /* __MMI_TOUCH_SCREEN__ */
#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__  
	SetRightSoftkeyFunction(InsertSimRSKPressOnScreenSaver,KEY_EVENT_DOWN);
#endif
#ifdef __MMI_KEYPAD_LOCK_PATTERN_2__  
	SetLeftSoftkeyFunction(InsertSimRSKPressOnScreenSaver,KEY_EVENT_DOWN);
#endif
#endif /* __MMI_TOUCH_SCREEN__ */

#ifdef __MMI_TOUCH_SCREEN__
	ChangeStatusIconImage(STATUS_ICON_KEYPAD_LOCK, IMG_SI_KEYPAD_LOCK);
	UpdateStatusIcons();
#else
	IdleSetStatusIcon(STATUS_ICON_KEYPAD_LOCK);	
#endif

	StopTimer(IDLE_APP_AUTO_LOCK_TIMER_ID);
}
//qiff add for m301,BUG:10748
void mmi_sim_error_idle_toggle_profile(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_STARKEY_TOGGLE_PROFILE__
    if (UCS2Strlen(g_idle_context.DialPadCallBuffer) == 1 && UCS2Strncmp(g_idle_context.DialPadCallBuffer, (S8*) L"*", 1) == 0)
#else
    if (UCS2Strlen(g_idle_context.DialPadCallBuffer) == 1 && UCS2Strncmp(g_idle_context.DialPadCallBuffer, (S8*) L"#", 1) == 0)
#endif
    {
        IdleToggleProfile();
    }
}

BOOL HasValidSIM(void)
{
#ifdef __MMI_MULTI_SIM__
	return (MTPNP_PFAL_Has_CardValid());
#else
	return (gInsertSimAppFlag != 1);
#endif
}



/*****************************************************************************
 * FUNCTION
 *  mmi_bootup_get_active_flight_mode
 * DESCRIPTION
 *  Get active mode is flight or normal
 * PARAMETERS
 *  void
 * RETURNS
 *  pBOOL
 *****************************************************************************/
pBOOL mmi_bootup_get_active_flight_mode(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* replace g_pwr_context.actFlightMode */
#ifdef __FLIGHT_MODE_SUPPORT__
#ifdef __MMI_MULTI_SIM__
	if (MTPNP_AD_Get_Startup_Mode() == MTPNP_AD_NVRAM_FLIGHT) 
    {
        return FLIGHTMODE_SILENT;
    }
    else
    {
        return FLIGHTMODE_OFF;
    }
#else
    return g_pwr_context[0].actFlightMode;
#endif /* __MMI_MULTI_SIM__ */
#else /* __FLIGHT_MODE_SUPPORT__ */ 
    return 0;
#endif /* __FLIGHT_MODE_SUPPORT__ */ 
}


