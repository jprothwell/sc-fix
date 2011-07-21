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
 *  FlightMode.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  This file is intends for phone setup application
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

#define __NEWSIMULATOR 

#include "mmi_features.h"   
#include "stdc.h"
#include "l4dr1.h"

#include "mmi_data_types.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_popup.h"
#include "custdatares.h"
#include "custmenures.h"
#include "eventsgprot.h"
#include "debuginitdef.h"
#include "historygprot.h"
#include "protocolevents.h"
#include "commonscreens.h"
#include "globaldefs.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "settingsgexdcl.h"
#include "settinggprots.h"
#include "settingdefs.h"
#include "mainmenudef.h"			/* main_menu_title_settings_icon */
#include "queuegprot.h"				/* osl send message */
#include "settingprot.h"			/* phonesetup prototype */
#include "settingprofile.h"		/* warning_tone */
#include "messagesexdcl.h"			/* getchildmenuidindexbyparentmenuid()  */
#include "wallpaperdefs.h"
#include "downloaddefs.h"
#include "phonesetup.h"
#include "phonesetupgprots.h"
#include "simdetectiondef.h"		/* for g_pwr_context */
#include "simdetectiongprot.h"
#include "wgui_status_icons.h"

#ifdef __FLIGHT_MODE_SUPPORT__
#include "simdetectiongexdcl.h"
#include "networksetupgprot.h"
#include "idleappprot.h"  
#include "callsetup.h"  
#include "globalscrenum.h"  
#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__
#include "statusiconres.h"
#endif

#ifdef __MMI_BT_SUPPORT__
#include "btmmiscrgprots.h"
#endif

#ifdef __MMI_IRDA_SUPPORT__
#include "irdammigprots.h"
#include "irdammiscr.h"
extern void mmi_irda_exec_close_menu(void);
#endif
#include "messagesmiscell.h"
#include "smsapi.h"
#include "smsstruct.h"
#ifdef MMI_ON_HARDWARE_P
//#include "wapadp.h"
#endif
#include "mmi_trace.h"
#endif /* __FLIGHT_MODE_SUPPORT__ */

#undef __NEWSIMULATOR

 

extern PHNSET_CNTX g_phnset_cntx;
extern U8 gSignalStrengthLevel[];

U32 StatusIconFlags[30]; /* Need to sync with RemoveStatusIconsForSimRemoved in SimDetection.c */

/*****************************************************************************
* FUNCTION
*   PhnsetFlightModeHideButSaveStatusIcon
*****************************************************************************/

MMI_BOOL PhnsetFlightModeHideButSaveStatusIcon(S16 icon_ID, U32 icon_flag)
{
	MMI_BOOL rtn = MMI_TRUE;
	switch(icon_ID)
	{
		case STATUS_ICON_ROAMING:
			StatusIconFlags[0] = icon_flag;
		break;
		case STATUS_ICON_CALL_DIVERT_L1L2:
			StatusIconFlags[1] = icon_flag;
		break;
#ifdef __MMI_CFU_SEPARATE_LINE__
		case STATUS_ICON_CALL_DIVERT_L1:
			StatusIconFlags[2] = icon_flag;
		break;
		case STATUS_ICON_CALL_DIVERT_L2:
			StatusIconFlags[3] = icon_flag;
		break;
#endif
		case STATUS_ICON_INCOMING_SMS:
			StatusIconFlags[4] = icon_flag;
		break;
		case STATUS_ICON_MISSED_CALL:
			StatusIconFlags[5] = icon_flag;
		break;
#ifndef __MMI_SUBLCD_SHORT_STATUSICON_LIST__
		case STATUS_ICON_SUBLCD_MISSED_CALL:
			StatusIconFlags[6] = icon_flag;
		break;
#endif
		case STATUS_ICON_UNREAD_VOICE_L1L2:
			StatusIconFlags[7] = icon_flag;
		break;
		case STATUS_ICON_UNREAD_VOICE_L1:
			StatusIconFlags[8] = icon_flag;
		break;
		case STATUS_ICON_UNREAD_VOICE_L2:
			StatusIconFlags[9] = icon_flag;
		break;
		case STATUS_ICON_UNREAD_FAX_L1L2:
			StatusIconFlags[10] = icon_flag;
		break;
		case STATUS_ICON_UNREAD_FAX_L1:
			StatusIconFlags[11] = icon_flag;
		break;
		case STATUS_ICON_UNREAD_FAX_L2:
			StatusIconFlags[12] = icon_flag;
		break;
		case STATUS_ICON_UNREAD_EMAIL_L1L2:
			StatusIconFlags[13] = icon_flag;
		break;
		case STATUS_ICON_UNREAD_EMAIL_L1:
			StatusIconFlags[14] = icon_flag;
		break;
		case STATUS_ICON_UNREAD_EMAIL_L2:
			StatusIconFlags[15] = icon_flag;
		break;
		case STATUS_ICON_SUBLCD_SMS:
			StatusIconFlags[16] = icon_flag;
		break;
		case STATUS_ICON_MMS_UNREAD:
			StatusIconFlags[17] = icon_flag;
		break;
		case STATUS_ICON_MMS_BUSY:
			StatusIconFlags[18] = icon_flag;
		break;
		case STATUS_ICON_CHAT_INDICATION_ROOM1:
			StatusIconFlags[19] = icon_flag;
		break;
		case STATUS_ICON_CHAT_INDICATION_ROOM2:
			StatusIconFlags[20] = icon_flag;
		break;
		case STATUS_ICON_WAP:
			StatusIconFlags[21] = icon_flag;
		break;
//		case STATUS_ICON_NETWORK_CIPHER_GSM:
//			StatusIconFlags[22] = icon_flag;
//		break;
//		case STATUS_ICON_NETWORK_CIPHER_GPRS:
//			StatusIconFlags[23] = icon_flag;
//		break;
#ifndef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__
		case STATUS_ICON_ALTERNATE_LINE_L1:
			StatusIconFlags[24] = icon_flag;
		break;
		case STATUS_ICON_ALTERNATE_LINE_L2:
			StatusIconFlags[25] = icon_flag;
		break;
#endif
#ifdef __MMI_POC__
		case STATUS_ICON_POC:
			StatusIconFlags[26] = icon_flag;
		break;
#endif
#ifdef __MMI_IMPS__
		case STATUS_ICON_IMPS_MSG:
			StatusIconFlags[27] = icon_flag;
		break;
		case STATUS_ICON_IMPS_LOGIN:
			StatusIconFlags[28] = icon_flag;
		break;
#endif

		default:
			rtn = MMI_FALSE;
		break;
	}

	return rtn;
}


#ifdef __FLIGHT_MODE_SUPPORT__

void mmi_msg_set_msg_icon (U8, U8);
extern void mmi_pwron_entry_animation_screen(void);
extern void SetCat165TimerExpireFunc(FuncPtr);
extern U32 get_status_icon_flag(S16 icon_ID);
extern void set_status_icon_flag(S16 icon_ID, U32 value);
extern void RemoveStatusIconsForSimRemoved(void);

#ifndef MMI_ON_HARDWARE_P
#define NVRAM_EF_SYS_CACHE_OCTET_SIZE 8
#endif

pBOOL gPowerOnWithFlightMode = MMI_FALSE;

/*****************************************************************************
* FUNCTION
*   PhnsetFlightModeSaveStatusIconFlags
*****************************************************************************/

void PhnsetFlightModeSaveStatusIconFlags(void)
{
	StatusIconFlags[0] = get_status_icon_flag(STATUS_ICON_ROAMING);
	StatusIconFlags[1] = get_status_icon_flag(STATUS_ICON_CALL_DIVERT_L1L2);
#ifdef __MMI_CFU_SEPARATE_LINE__
	StatusIconFlags[2] = get_status_icon_flag(STATUS_ICON_CALL_DIVERT_L1);
	StatusIconFlags[3] = get_status_icon_flag(STATUS_ICON_CALL_DIVERT_L2);
#endif
	StatusIconFlags[4] = get_status_icon_flag(STATUS_ICON_INCOMING_SMS);
	StatusIconFlags[5] = get_status_icon_flag(STATUS_ICON_MISSED_CALL);
#ifndef __MMI_SUBLCD_SHORT_STATUSICON_LIST__	
	StatusIconFlags[6] = get_status_icon_flag(STATUS_ICON_SUBLCD_MISSED_CALL);
#endif
	StatusIconFlags[7] = get_status_icon_flag(STATUS_ICON_UNREAD_VOICE_L1L2);
	StatusIconFlags[8] = get_status_icon_flag(STATUS_ICON_UNREAD_VOICE_L1);
	StatusIconFlags[9] = get_status_icon_flag(STATUS_ICON_UNREAD_VOICE_L2);
	StatusIconFlags[10] = get_status_icon_flag(STATUS_ICON_UNREAD_FAX_L1L2);
	StatusIconFlags[11] = get_status_icon_flag(STATUS_ICON_UNREAD_FAX_L1);
	StatusIconFlags[12] = get_status_icon_flag(STATUS_ICON_UNREAD_FAX_L2);
	StatusIconFlags[13] = get_status_icon_flag(STATUS_ICON_UNREAD_EMAIL_L1L2);
	StatusIconFlags[14] = get_status_icon_flag(STATUS_ICON_UNREAD_EMAIL_L1);
	StatusIconFlags[15] = get_status_icon_flag(STATUS_ICON_UNREAD_EMAIL_L2);
	StatusIconFlags[16] = get_status_icon_flag(STATUS_ICON_SUBLCD_SMS);
	StatusIconFlags[17] = get_status_icon_flag(STATUS_ICON_MMS_UNREAD);
	StatusIconFlags[18] = get_status_icon_flag(STATUS_ICON_MMS_BUSY);
	StatusIconFlags[19] = get_status_icon_flag(STATUS_ICON_CHAT_INDICATION_ROOM1);
	StatusIconFlags[20] = get_status_icon_flag(STATUS_ICON_CHAT_INDICATION_ROOM2);
	StatusIconFlags[21] = get_status_icon_flag(STATUS_ICON_WAP);
//	StatusIconFlags[22] = get_status_icon_flag(STATUS_ICON_NETWORK_CIPHER_GSM);
//	StatusIconFlags[23] = get_status_icon_flag(STATUS_ICON_NETWORK_CIPHER_GPRS);
#ifndef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__
	StatusIconFlags[24] = get_status_icon_flag(STATUS_ICON_ALTERNATE_LINE_L1);
	StatusIconFlags[25] = get_status_icon_flag(STATUS_ICON_ALTERNATE_LINE_L2);
#endif
#ifdef __MMI_POC__
	StatusIconFlags[26] = get_status_icon_flag(STATUS_ICON_POC);
#endif
#ifdef __MMI_IMPS__
	StatusIconFlags[27] = get_status_icon_flag(STATUS_ICON_IMPS_MSG);
	StatusIconFlags[28] = get_status_icon_flag(STATUS_ICON_IMPS_LOGIN);
#endif

}

/*****************************************************************************
* FUNCTION
*   PhnsetFlightModeRestoreStatusIconFlags
*****************************************************************************/

void PhnsetFlightModeRestoreStatusIconFlags(void)
{
	set_status_icon_flag(STATUS_ICON_ROAMING,		StatusIconFlags[0]);
	set_status_icon_flag(STATUS_ICON_CALL_DIVERT_L1L2,	StatusIconFlags[1]);
#ifdef __MMI_CFU_SEPARATE_LINE__
	set_status_icon_flag(STATUS_ICON_CALL_DIVERT_L1,	StatusIconFlags[2]);
	set_status_icon_flag(STATUS_ICON_CALL_DIVERT_L2,	StatusIconFlags[3]);
#endif
	set_status_icon_flag(STATUS_ICON_INCOMING_SMS,		StatusIconFlags[4]);
	set_status_icon_flag(STATUS_ICON_MISSED_CALL,		StatusIconFlags[5]);
#ifndef __MMI_SUBLCD_SHORT_STATUSICON_LIST__	
	set_status_icon_flag(STATUS_ICON_SUBLCD_MISSED_CALL,	StatusIconFlags[6]);
#endif
	set_status_icon_flag(STATUS_ICON_UNREAD_VOICE_L1L2,	StatusIconFlags[7]);
	set_status_icon_flag(STATUS_ICON_UNREAD_VOICE_L1,	StatusIconFlags[8]);
	set_status_icon_flag(STATUS_ICON_UNREAD_VOICE_L2,	StatusIconFlags[9]);
	set_status_icon_flag(STATUS_ICON_UNREAD_FAX_L1L2,	StatusIconFlags[10]);
	set_status_icon_flag(STATUS_ICON_UNREAD_FAX_L1,		StatusIconFlags[11]);
	set_status_icon_flag(STATUS_ICON_UNREAD_FAX_L2,		StatusIconFlags[12]);
	set_status_icon_flag(STATUS_ICON_UNREAD_EMAIL_L1L2,	StatusIconFlags[13]);
	set_status_icon_flag(STATUS_ICON_UNREAD_EMAIL_L1,	StatusIconFlags[14]);
	set_status_icon_flag(STATUS_ICON_UNREAD_EMAIL_L2,	StatusIconFlags[15]);
	set_status_icon_flag(STATUS_ICON_SUBLCD_SMS,		StatusIconFlags[16]);
	set_status_icon_flag(STATUS_ICON_MMS_UNREAD,		StatusIconFlags[17]);
	set_status_icon_flag(STATUS_ICON_MMS_BUSY,		StatusIconFlags[18]);
	set_status_icon_flag(STATUS_ICON_CHAT_INDICATION_ROOM1,	StatusIconFlags[19]);
	set_status_icon_flag(STATUS_ICON_CHAT_INDICATION_ROOM2,	StatusIconFlags[20]);
	set_status_icon_flag(STATUS_ICON_WAP,			StatusIconFlags[21]);
//	set_status_icon_flag(STATUS_ICON_NETWORK_CIPHER_GSM,	StatusIconFlags[22]);
//	set_status_icon_flag(STATUS_ICON_NETWORK_CIPHER_GPRS,	StatusIconFlags[23]);
#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__
 
	if(g_callset_cntx_p->LineID == LINE1)
		ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH, IMG_SI_SIGNAL_STRENGTH_L1); 
	else
		ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH, IMG_SI_SIGNAL_STRENGTH_L2); 
	UpdateStatusIcons(); 
#else
	set_status_icon_flag(STATUS_ICON_ALTERNATE_LINE_L1,	StatusIconFlags[24]);
	set_status_icon_flag(STATUS_ICON_ALTERNATE_LINE_L2,	StatusIconFlags[25]);
#endif
#ifdef __MMI_POC__
	set_status_icon_flag(STATUS_ICON_POC,			StatusIconFlags[26]);
#endif
#ifdef __MMI_IMPS__
	set_status_icon_flag(STATUS_ICON_IMPS_MSG,		StatusIconFlags[27]);
	set_status_icon_flag(STATUS_ICON_IMPS_LOGIN,		StatusIconFlags[28]);
#endif

}

/*****************************************************************************
* FUNCTION
*   HighlightPhnsetFlightMode
* DESCRIPTION
*   This function is Highlight handler for Phone Setup -> Flight Mode
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightPhnsetFlightMode(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
//	mmi_trace(0,"MENU_SETTING_FLIGHT_MODE is %d,STR_MENU_FLIGHT_MODE IS %d,MENU9102_INITIAL_SETUP is %d",MENU_SETTING_FLIGHT_MODE,STR_MENU_FLIGHT_MODE,MENU9102_INITIAL_SETUP);

	SetLeftSoftkeyFunction(EntryPhnsetFlightMode, KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetFlightMode, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*   HintPhnsetFlightMode
* DESCRIPTION
*   This function is hint handling function for Phone Setup -> Flight Mode
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HintPhnsetFlightMode(U16 index)
{
	S16 error;
	U8 data[NVRAM_EF_SYS_CACHE_OCTET_SIZE];
	
	ReadRecord(NVRAM_EF_SYS_CACHE_OCTET_LID, 2 , &data, NVRAM_EF_SYS_CACHE_OCTET_SIZE, &error);
	g_phnset_cntx.curFlightMode = data[0];

	switch(g_phnset_cntx.curFlightMode)
	{
   		case FLIGHTMODE_OFF: /* 0x00 */
			pfnUnicodeStrcpy( (PS8)hintData[index], (PS8)GetString(STR_FLIGHT_MODE_NORMAL) );
		break;

		case FLIGHTMODE_SILENT: /* 0x01 */
			pfnUnicodeStrcpy( (PS8)hintData[index], (PS8)GetString(STR_FLIGHT_MODE_SILENT) );
		break;

		case FLIGHTMODE_QUERY: /* 0x02 */
			pfnUnicodeStrcpy( (PS8)hintData[index], (PS8)GetString(STR_FLIGHT_MODE_QUERY) );
		break;

		default:
			data[0] = FLIGHTMODE_OFF;
			data[1] = FLIGHTMODE_OFF;
			g_phnset_cntx.curFlightMode = 0;
			WriteRecord(NVRAM_EF_SYS_CACHE_OCTET_LID, 2 , &data, NVRAM_EF_SYS_CACHE_OCTET_SIZE, &error);
			pfnUnicodeStrcpy( (PS8)hintData[index], (PS8)GetString(STR_FLIGHT_MODE_NORMAL) );
		break;
	}
}

/*****************************************************************************
* FUNCTION
*   HiliteHandlerBandSelection
*****************************************************************************/

void HiliteHandlerFlightMode(S32 index) 
{
	g_phnset_cntx.selFlightMode = (U8) index;
	return;
}

//extern void EntryFlightModeIdleScreen(void);

/*****************************************************************************
* FUNCTION
*   EntryFlightModeProgress
*****************************************************************************/

void EntryFlightModeProgress(void)
{
	EntryNewScreen(SCR_FLIGHT_MODE_PROGRESS, NULL, EntryFlightModeProgress, NULL);
	SetKeyHandler(NULL, KEY_END, KEY_EVENT_DOWN);
	SetKeyHandler(NULL, KEY_END, KEY_EVENT_UP);
	ShowCategory66Screen(STR_MENU_FLIGHT_MODE, 0, 0, 0, 0, 0, (PU8)GetString(STR_FLIGHT_MODE_PROGRESS), IMG_FLIGHT_MODE_PROGRESS, NULL);
}

/*****************************************************************************
* FUNCTION
*   PhnsetFlightModeSelectionRsp
*****************************************************************************/

void PhnsetFlightModeSelectionRsp(void *info) 
{
	U8 data[NVRAM_EF_SYS_CACHE_OCTET_SIZE];
	S16 error;
	mmi_nw_cfun_state_rsp_struct *cfun_state_rsp;
	U16 i;

	cfun_state_rsp = (mmi_nw_cfun_state_rsp_struct *)info;
  mmi_trace(1,"PhnsetFlightModeSelectionRsp");
	ClearProtocolEventHandler(PRT_NW_CFUN_STATE_RSP);

#ifdef MMI_ON_HARDWARE_P
	if(cfun_state_rsp->result == KAL_TRUE)
	{
#endif
		 
		//if(GetActiveScreenId() == SCR_ID_ALM_INDICATION)
		if(GetActiveScreenId() != SCR_FLIGHT_MODE_PROGRESS)
		{
			DeleteScreenIfPresent(SCR_FLIGHT_MODE_PROGRESS);  /* progressing */
			DeleteScreenIfPresent(SCR_FLIGHT_MODE_SELECTION); /* flight mode menu */
			DeleteScreenIfPresent(SCR9003_INITIALSETUP);	  /* phone setup */
			DeleteScreenIfPresent(SCR9000_SETTINGS);	  /* setting */
			DeleteScreenIfPresent(MAIN_MENU_SCREENID);	  /* main menu */
		}
		else
		{
			DisplayPopupCallBack((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED,
					     1, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE, EntryIdleScreen);
			DeleteUptoScrID(IDLE_SCREEN_ID);
		}

		g_phnset_cntx.curFlightMode = g_phnset_cntx.selFlightMode; /* update flight mode if it is successfully changed */

		if(mmi_bootup_get_active_flight_mode() == FLIGHTMODE_OFF && // normal mode to flight mode
#ifdef MMI_ON_HARDWARE_P
	#ifdef MMS_SUPPORT
		  (mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX) == MMI_FRM_SMS_INVALID_INDEX || !MSF_wapIsReady() ) )
	#else
		   mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX) == MMI_FRM_SMS_INVALID_INDEX )
	#endif
#else
		   mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX) == MMI_FRM_SMS_INVALID_INDEX )
#endif
			gPowerOnWithFlightMode = MMI_TRUE;
		else
			gPowerOnWithFlightMode = MMI_FALSE;

		 
		if(mmi_bootup_get_active_flight_mode() == FLIGHTMODE_OFF) // normal mode to flight mode
		{
			PhnsetFlightModeSaveStatusIconFlags();
			RemoveStatusIconsForSimRemoved();
			UpdateStatusIcons();
		}

		g_pwr_context[0].actFlightMode = g_phnset_cntx.selFlightMode;
		data[0] = g_phnset_cntx.curFlightMode;
		data[1] = mmi_bootup_get_active_flight_mode();
		WriteRecord(NVRAM_EF_SYS_CACHE_OCTET_LID, 2 , &data, NVRAM_EF_SYS_CACHE_OCTET_SIZE, &error); /* save it to NVRAM */
        	mmi_trace(1,"PhnsetFlightModeSelectionRsp,FlightMode is %d,gSignalStrengthLevel is %d",mmi_bootup_get_active_flight_mode(),gSignalStrengthLevel[0]);

		if(mmi_bootup_get_active_flight_mode() == FLIGHTMODE_OFF) // flight mode to normal mode
		{
			PhnsetFlightModeRestoreStatusIconFlags();
			UpdateStatusIcons();
		}

/*qiff add to refresh network name's display when switch phone mode on GS PRJ.2008/9/8*/			
#if 0
		 
		ChangeStatusIconLevel(STATUS_ICON_SIGNAL_STRENGTH, gSignalStrengthLevel);
#ifdef __MMI_SUBLCD__
		ChangeStatusIconLevel(STATUS_ICON_SUBLCD_SIGNAL_STRENGTH, gSignalStrengthLevel);
#endif
#endif
		for(i = 0; i < MMI_SIM_NUMBER; i++)
		{
			g_pwr_context[i].IsCampOnTonePlayed = 0;
			g_pwr_context[i].InvalidSimShowed = 1;
		}

		/* bluetooth power on/off */
		#if defined(__MMI_BT_SUPPORT__)
		if(mmi_bootup_get_active_flight_mode() == FLIGHTMODE_OFF)
			mmi_bt_reset_hw_on();
		else
			mmi_bt_reset_hw_off();
		#endif

		#ifdef __MMI_IRDA_SUPPORT__
		if(MMI_IRDASCR_CNTX(irda_state) != MMIR_SCR_IRCLOSE && mmi_bootup_get_active_flight_mode() == FLIGHTMODE_SILENT)
		{
			if(mmi_irda_is_ircomm_active() == MMI_TRUE)
				mmi_irda_ircomm_disconnect_request();
			mmi_irda_exec_close_menu();
		}
		#endif

#ifdef MMI_ON_HARDWARE_P
	}
	else
	{
		DisplayPopup((U8*)GetString(STR_GLOBAL_NOT_DONE), IMG_GLOBAL_UNFINISHED, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
		DeleteScreenIfPresent(SCR_FLIGHT_MODE_PROGRESS);
	}
#endif
}

/*****************************************************************************
* FUNCTION
*   PhnsetFlightModeSelectionReq
*****************************************************************************/

void PhnsetFlightModeSelectionReq(void) 
{
	U8 data[NVRAM_EF_SYS_CACHE_OCTET_SIZE];
	S16 error;
#if 0 
	MYQUEUE Message;
#endif	
	mmi_nw_cfun_state_req_struct *cfun_state_req;

	struct _fake
	{
	    UINT8 a;
	    UINT16 b;
	    BOOL result;
	}rsp;
			
mmi_trace(1,"PhnsetFlightModeSelectionReq,g_phnset_cntx.selFlightMode is %d",g_phnset_cntx.selFlightMode);
	if(g_phnset_cntx.selFlightMode == FLIGHTMODE_QUERY )
	{
		DisplayPopup((U8*)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)SUCCESS_TONE);
		DeleteScreenIfPresent(SCR_FLIGHT_MODE_SELECTION); /* flight mode menu */
		/* save it to NVRAM only and not necessary to send request to L4C */
		g_phnset_cntx.curFlightMode = g_phnset_cntx.selFlightMode;
		data[0] = g_phnset_cntx.curFlightMode;
		data[1] = mmi_bootup_get_active_flight_mode();
		WriteRecord(NVRAM_EF_SYS_CACHE_OCTET_LID, 2 , &data, NVRAM_EF_SYS_CACHE_OCTET_SIZE, &error);
        mmi_trace(1,"PhnsetFlightModeSelectionReq,FLIGHTMODE_QUERY,0 is %d,1 is %d,error is %d",data[0],data[1],error);
	        //CFW_SetFlightMode(  FLIGHTMODE_QUERY, 1);
		return;
	}

	if((g_phnset_cntx.selFlightMode != FLIGHTMODE_QUERY) && 
	   (g_phnset_cntx.curFlightMode == FLIGHTMODE_QUERY) &&
	   (mmi_bootup_get_active_flight_mode() == g_phnset_cntx.selFlightMode))
	{
		DisplayPopup ((U8*)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)SUCCESS_TONE);
		DeleteScreenIfPresent(SCR_FLIGHT_MODE_SELECTION); /* flight mode menu */
		/* save it to NVRAM only and not necessary to send request to L4C */
		g_phnset_cntx.curFlightMode = g_phnset_cntx.selFlightMode;
		data[0] = g_phnset_cntx.curFlightMode;
		data[1] = mmi_bootup_get_active_flight_mode();
		WriteRecord(NVRAM_EF_SYS_CACHE_OCTET_LID, 2 , &data, NVRAM_EF_SYS_CACHE_OCTET_SIZE, &error);
		
		if(g_phnset_cntx.selFlightMode == FLIGHTMODE_OFF ) 
		{
			//CFW_SetFlightMode(  FLIGHTMODE_OFF, 1);//normal mode
		}
		else
		{
			//CFW_SetFlightMode(  FLIGHTMODE_SILENT, 1);//flight mode
		}
		return;
	}

	cfun_state_req = OslConstructDataPtr( sizeof(mmi_nw_cfun_state_req_struct) );

	if(g_phnset_cntx.selFlightMode == FLIGHTMODE_OFF ) /* normal mode */
		cfun_state_req->cfun_state = 1;
	else /* flight mode */
		cfun_state_req->cfun_state = 4;

	SetProtocolEventHandler(PhnsetFlightModeSelectionRsp, PRT_NW_CFUN_STATE_RSP);
	EntryFlightModeProgress(); /* show searching screen */
    /****chenhe for jasper flight mode+*******/
//COS_Sleep(1000);
mmi_trace(1,"PhnsetFlightModeSelectionReq,cfun_state_req->cfun_state is %d",cfun_state_req->cfun_state);
if(cfun_state_req->cfun_state == 4)
{ //chenhe test

	#if 0
        rsp.result = (CFW_ShellControl(CFW_CONTROL_CMD_POWER_OFF) == ERR_SUCCESS)?TRUE:FALSE;
        PhnsetFlightModeSelectionRsp(&rsp);
	#else
		//CFW_SetFlightMode(  FLIGHTMODE_SILENT, 1);//flight mode

	#endif
    /****chenhe for jasper flight mode*******/
}
else if(cfun_state_req->cfun_state == 1)
{
	#if 0
	rsp.result = (CFW_ShellControl(CFW_CONTROL_CMD_POWER_ON) == ERR_SUCCESS)?TRUE:FALSE;
	PhnsetFlightModeSelectionRsp(&rsp);
	#else
			//CFW_SetFlightMode(  FLIGHTMODE_OFF, 1);//normal mode
	#endif
}
rsp.result = 1;
 PhnsetFlightModeSelectionRsp(&rsp);
/****chenhe for jasper flight mode-*******/
    #if 0 //chenhe
	Message.oslMsgId = PRT_NW_CFUN_STATE_REQ;	
	Message.oslDataPtr = (oslParaType *)cfun_state_req;
	Message.oslPeerBuffPtr= NULL;
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C;

	OslMsgSendExtQueue(&Message);
    #endif
}

/*****************************************************************************
* FUNCTION
*   EntryFlightModeSelectionCnf
*****************************************************************************/

void EntryFlightModeSelectionCnf(void) 
{
	/* Buffer holding history data */
	U8* guiBuffer;

	/*Call Exit Handler*/
	EntryNewScreen(SCR_FLIGHT_MODE_CONFIRM, NULL, NULL, NULL);

	/*Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_FLIGHT_MODE_CONFIRM);	

	/*Display Category Screen */
	ShowCategory162Screen(	STR_GLOBAL_YES, IMG_GLOBAL_YES,
				STR_GLOBAL_NO,  IMG_GLOBAL_NO, STR_FLIGHT_MODE_CONFIRM, IMG_GLOBAL_QUESTION, guiBuffer);


	SetLeftSoftkeyFunction(PhnsetFlightModeSelectionReq,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* Play warning tone */
	playRequestedTone(WARNING_TONE);
}

/*****************************************************************************
* FUNCTION
*   PhnsetFlightModeSelectionOK
*****************************************************************************/
//now use newly flight control function for multi sim support
#if 0
void PhnsetFlightModeSelectionOK(void) 
{
	if(g_phnset_cntx.selFlightMode == g_phnset_cntx.curFlightMode)
	{
		/* select mode is the same as current mode, so not necessary to send request to L4C */
		DisplayPopup ((U8*)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)SUCCESS_TONE);
		DeleteScreenIfPresent(SCR_FLIGHT_MODE_SELECTION); /* flight mode menu */
	}
	else
	{
		EntryFlightModeSelectionCnf();
	}
}
#else
/*****************************************************************************
* FUNCTION
*   EntryPhnsetFlightMode
* DESCRIPTION
*   This function is Entry function for Phone Setup -> Flight Mode
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
#ifdef __MMI_MULTI_SIM__
extern U8 DMModeEnabled[];
#else
U8 FlightModeEnabled;
/*
#if (defined (__NATIVE_JAVA__))    
void gcj_NativeJavaQuit(unsigned char isQuit);
#endif
*/
void GoBackDMSetHistory(void)
{
	GoBacknHistory(1);
}

void FlightModeSwitchComplete(void)
{
	if(FlightModeEnabled)
		g_pwr_context[0].actFlightMode = FLIGHTMODE_SILENT;
	else
		g_pwr_context[0].actFlightMode = FLIGHTMODE_OFF;

	//MTPNP_PFAL_Rearrange_StatusBar();

#if defined(__MMI_BT_SUPPORT__)
	if(mmi_bootup_get_active_flight_mode() == FLIGHTMODE_OFF)
	{
		mmi_trace(g_sw_BT, "DualModeSwitchComplete::mmi_bt_reset_hw_on");
		if(!mmi_bt_is_power_on(FALSE))
			mmi_bt_reset_hw_on();
	}
	else
	{
		mmi_trace(g_sw_BT, "DualModeSwitchComplete::mmi_bt_reset_hw_off");
		mmi_bt_reset_hw_off();
	}
#endif
/*
#if (defined (__NATIVE_JAVA__))    
	gcj_NativeJavaQuit(g_pwr_context[0].actFlightMode);
#endif
*/
	DisplayPopup((UINT8 *) GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	DeleteUptoScrID(IDLE_SCREEN_ID);
}

void FlightModeSwitchFailed(void)
{
	DisplayPopup((UINT8 *) GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1, ST_NOTIFYDURATION, ERROR_TONE);
	DeleteScreenIfPresent(SCR_FLIGHT_MODE_PROGRESS);
}

void mmi_flightmode_set_rsp_hdlr(void *msg_ptr)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	mmi_nw_cfun_state_rsp_struct *p = (mmi_nw_cfun_state_rsp_struct *)msg_ptr;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	ClearProtocolEventHandler(PRT_NW_CFUN_STATE_RSP);

	if (p->result == KAL_TRUE)
		FlightModeSwitchComplete();
	else
		FlightModeSwitchFailed();
}


void SaveFlightModeSet(void)
{
	mmi_nw_cfun_state_req_struct *p = NULL;
	ilm_struct ilm = {0};
	
	if (FlightModeEnabled == mmi_bootup_get_active_flight_mode())
	{
		DisplayPopup((UINT8 *) GetString(STR_GLOBAL_DONE),
					 IMG_GLOBAL_INFO, 0, ST_NOTIFYDURATION, WARNING_TONE);
		DeleteUptoScrID(IDLE_SCREEN_ID);
		return;
	}

	/* entry switch progress screen */
	EntryFlightModeProgress();

	p = (mmi_nw_cfun_state_req_struct *)OslMalloc(sizeof(mmi_nw_cfun_state_req_struct));
	MMI_ASSERT(p != NULL);
	memset(p, 0, sizeof(mmi_nw_cfun_state_req_struct));

	if(FlightModeEnabled)
		p->cfun_state = 0;
	else
		p->cfun_state = 1;

	memset(&ilm, 0, sizeof(ilm_struct));

	ilm.dest_mod_id = MOD_L4C;
	ilm.src_mod_id = MOD_MMI;
	ilm.sap_id = CSM_L4C_SAP;
	ilm.local_para_ptr = (local_para_struct *)p;
	ilm.msg_id = PRT_NW_CFUN_STATE_REQ;

	SetProtocolEventHandler(mmi_flightmode_set_rsp_hdlr, PRT_NW_CFUN_STATE_RSP);

	OslMsgSendExtQueue(&ilm);
}

void CheckFlightMode(void)
{

	DisplayConfirm(STR_GLOBAL_YES, 0, STR_GLOBAL_NO, 0, get_string(STR_GLOBAL_SAVE), IMG_GLOBAL_QUESTION, 0);

	SetLeftSoftkeyFunction (SaveFlightModeSet, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackDMSetHistory, KEY_EVENT_UP);
   
}
#endif
void PhnsetFlightModeSelectionOK(void)
{
	U8 i =0;
#ifdef __MMI_MULTI_SIM__
	if(g_phnset_cntx.selFlightMode)//normal
	{
		for(i=0;i<MMI_SIM_NUMBER;i++)
			DMModeEnabled[i]=1;

	}
	else //flight
	{
		for(i=0;i<MMI_SIM_NUMBER;i++)
			DMModeEnabled[i]=0;

	}
	CheckDmMode();
#else
	if(g_phnset_cntx.selFlightMode)//normal
	{
		FlightModeEnabled = 0;
	}
	else //flight
	{
		FlightModeEnabled = 1;
	}
	CheckFlightMode();
#endif
	
}
#endif
extern void mmi_show_qq_select_confirm(void (*default_entry) (void));

void EntryPhnsetFlightMode(void)
{
	U8 nNumofItem = 2;
	U8* guiBuffer;
	U16 nStrItemList[2] = {STR_FLIGHT_MODE_SILENT , STR_FLIGHT_MODE_NORMAL};

	if(checkVMOn())
	{
	    mmi_show_qq_select_confirm( EntryPhnsetFlightMode);
		return;
	}

	EntryNewScreen(SCR_FLIGHT_MODE_SELECTION, NULL, EntryPhnsetFlightMode, NULL);
    

	guiBuffer = GetCurrGuiBuffer(SCR_FLIGHT_MODE_SELECTION);

#ifdef __MMI_MULTI_SIM__
       if(MTPNP_AD_Is_Flight_Mode())
#else
	if(mmi_bootup_get_active_flight_mode() == FLIGHTMODE_SILENT)
#endif
            g_phnset_cntx.curFlightMode=0;
        else
             g_phnset_cntx.curFlightMode=1;
        
	RegisterHighlightHandler(HiliteHandlerFlightMode);
	ShowCategory11Screen(STR_MENU_FLIGHT_MODE, MAIN_MENU_TITLE_SETTINGS_ICON,
						    STR_GLOBAL_OK, IMG_GLOBAL_OK,
						    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						    nNumofItem, nStrItemList, g_phnset_cntx.curFlightMode, guiBuffer);

	SetLeftSoftkeyFunction(PhnsetFlightModeSelectionOK, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(0, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

 
#ifndef __MMI_TOUCH_SCREEN__
#if (defined(__MMI_KEYPAD_LOCK_PATTERN_1__) && !defined(__MMI_DISABLE_KEYPAD_LOCK__))
	//if(g_idle_context.ToNameScrFromIdleApp==1) 
	
/*zhangm Add Start For 10821 Ver: GreenStone  on 2008-12-19 16:23 */
//	if(g_idle_context.RskPressedFromIdleApp == 1)
//	{
//		StartTimer(KEYPAD_LOCK_TIMER, KEYPAD_LOCK_TIMEOUT, IdleHandleKeypadLockProcess);
//		SetKeyHandler(IdleHandlePoundKeyForKeypadLock,KEY_POUND,KEY_EVENT_UP);
//	}
	SetKeyHandler(IdleHandleStarKeyForKeypadLock,KEY_STAR,KEY_LONG_PRESS);
/*zhangm Add End  For 10821 Ver: GreenStone  on 2008-12-19 16:23 */
#endif
#endif

	return;
}

/*****************************************************************************
* FUNCTION
*   PhnsetFlightModeSaveMode
*****************************************************************************/
void PhnsetFlightModeSaveMode(void)
{
	U8 data[NVRAM_EF_SYS_CACHE_OCTET_SIZE];
	S16 error;
	data[0] = g_phnset_cntx.curFlightMode;
	data[1] = mmi_bootup_get_active_flight_mode();
	WriteRecord(NVRAM_EF_SYS_CACHE_OCTET_LID, 2 , &data, NVRAM_EF_SYS_CACHE_OCTET_SIZE, &error);
}

/*****************************************************************************
* FUNCTION
*   PowerOnFlightMode
*****************************************************************************/

void PowerOnFlightMode(void)
{
#if 0 
	MYQUEUE Message;
#endif
	mmi_nw_cfun_state_req_struct *cfun_state_req;

	cfun_state_req = OslConstructDataPtr( sizeof(mmi_nw_cfun_state_req_struct) );
	mmi_trace(1,"PowerOnFlightMode,g_pwr_context.PowerOnMode is %d",g_pwr_context[0].PowerOnMode);
	#ifdef __FLIGHT_MODE_SUPPORT__
	PhnsetFlightModeSaveStatusIconFlags();
	RemoveStatusIconsForSimRemoved();
	UpdateStatusIcons();
	#endif

	cfun_state_req->cfun_state = 4;
	g_pwr_context[0].actFlightMode = FLIGHTMODE_SILENT;
	if(g_phnset_cntx.curFlightMode != FLIGHTMODE_QUERY)
		g_phnset_cntx.curFlightMode = FLIGHTMODE_SILENT;
#if 0
	Message.oslMsgId = PRT_NW_CFUN_STATE_REQ;	
	Message.oslDataPtr = (oslParaType *)cfun_state_req;
	Message.oslPeerBuffPtr= NULL;
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C;

	OslMsgSendExtQueue(&Message);
#else
    { //chenhe for jasper

      //CFW_ShellControl(CFW_CONTROL_CMD_POWER_OFF);
       
    /****chenhe for jasper flight mode*******/
}
#endif
	StopTimer(POPUP_TIMER);
	ClearAllKeyHandler(); /* prevent user press key after mode selection in qurey screen */
	
	#if defined(__MMI_BT_SUPPORT__)
	mmi_bt_reset_hw_off();
	#endif
	
	gPowerOnWithFlightMode = MMI_TRUE;

	DeleteScreenIfPresent(GLOBAL_SCR_DUMMY);

	if(g_pwr_context[0].PowerOnMode == POWER_ON_KEYPAD||g_pwr_context[0].PowerOnMode == POWER_ON_EXCEPTION)  
	{
		PhnsetFlightModeSaveMode();
		mmi_pwron_entry_animation_screen();
	}
}

/*****************************************************************************
* FUNCTION
*   PowerOnNormalMode
*****************************************************************************/

void PowerOnNormalMode(void)
{
#if 0 
	MYQUEUE Message;
	mmi_nw_cfun_state_req_struct *cfun_state_req;
    mmi_trace(1,"PowerOnNormalMode");
	cfun_state_req = OslConstructDataPtr( sizeof(mmi_nw_cfun_state_req_struct) );

	cfun_state_req->cfun_state = 1;
#endif
	//g_pwr_context[0].actFlightMode = FLIGHTMODE_OFF;
	if(g_phnset_cntx.curFlightMode != FLIGHTMODE_QUERY)
		g_phnset_cntx.curFlightMode = FLIGHTMODE_OFF;
#if 0 //chenhe for jasper
	Message.oslMsgId = PRT_NW_CFUN_STATE_REQ;	
	Message.oslDataPtr = (oslParaType *)cfun_state_req;
	Message.oslPeerBuffPtr= NULL;
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C;

	OslMsgSendExtQueue(&Message);
#else
  // CFW_ShellControl(CFW_CONTROL_CMD_POWER_ON);
#endif
	StopTimer(POPUP_TIMER);
	ClearAllKeyHandler(); /* prevent user press key after mode selection in qurey screen */
	
	DeleteScreenIfPresent(GLOBAL_SCR_DUMMY);
	if(g_pwr_context[0].PowerOnMode == POWER_ON_KEYPAD)  
	{
		PhnsetFlightModeSaveMode();
		mmi_pwron_entry_animation_screen();
	}
}



void powerNomalAfterQuery(void)
{
	mmi_trace(1,"powerNomalAfterQuery");
	//CFW_SetFlightMode(  FLIGHTMODE_QUERY, 1);//query mode next time 
	PowerOnNormalMode();

}
void PhnsetFlightModeQueryBackground(void)
{
	 
	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, PhnsetFlightModeQueryBackground, NULL);
	ShowCategory61Screen(NULL, IMG_BOOTUP_SCANNING_LOGO, NULL);
}

/*****************************************************************************
* FUNCTION
*   EntryQueryFlightModeScreen
*****************************************************************************/

void EntryQueryFlightModeScreen(void)
{
	U8* guiBuffer;

	guiBuffer = GetCurrGuiBuffer(SCR_FLIGHT_MODE_QUERY);

   	EntryNewScreen(SCR_FLIGHT_MODE_QUERY, NULL, EntryQueryFlightModeScreen, NULL);

	#ifdef __FLIGHT_MODE_RENAME_TO_ANTENNA_MODE__
	ShowCategory165Screen_ext( STR_GLOBAL_YES, IMG_GLOBAL_YES, 
				   STR_GLOBAL_NO , IMG_GLOBAL_NO ,
				   get_string(STR_FLIGHT_MODE_POWERON), IMG_GLOBAL_QUESTION, 10, guiBuffer);
	#else
	ShowCategory165Screen_ext( STR_GLOBAL_YES, IMG_GLOBAL_YES, 
				   STR_GLOBAL_NO , IMG_GLOBAL_NO ,
				   get_string(STR_MENU_FLIGHT_MODE), IMG_GLOBAL_QUESTION, 10, guiBuffer);
	#endif

	//SetCat165TimerExpireFunc(PowerOnNormalMode);//chenhe instead it as following
	SetCat165TimerExpireFunc(PowerOnNormalMode);


	playRequestedTone(WARNING_TONE);

	//ShowSubLCDScreen(ShowIdleAppSubLcd);
	/* no sub-lcd exit handler because of no interrupt in this screen */
	#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
		if ( IsClamClose() )
			ShowSubLCDAnalogClockScreen();
		else
			ShowSubLCDLogoString();
	#else
		ShowCategory301Screen((PU8)GetString(STR_MENU_FLIGHT_MODE),0,NULL);
	#endif

	g_phnset_cntx.curFlightMode = FLIGHTMODE_QUERY;

	/* No history to go back */
	/* Always enter normal mode unless the flight mode is selected by pressing LSK (Yes) */
	ClearAllKeyHandler();
	//SetKeyHandler(PowerOnNormalMode, KEY_END, KEY_EVENT_DOWN);//chenhe instead it as following
	SetKeyHandler(PowerOnNormalMode, KEY_END, KEY_EVENT_DOWN);
	
	#ifdef __FLIGHT_MODE_RENAME_TO_ANTENNA_MODE__	
	/* use flight mode as title */
	SetLeftSoftkeyFunction(PowerOnNormalMode, KEY_EVENT_UP);
	
	//SetRightSoftkeyFunction(PowerOnFlightMode, KEY_EVENT_UP);//chenhe instead it as following
	SetRightSoftkeyFunction(PowerOnFlightMode, KEY_EVENT_UP);
	#else
	/* use antenna mode as title */
	SetLeftSoftkeyFunction(PowerOnFlightMode, KEY_EVENT_UP);
	
	//SetRightSoftkeyFunction(PowerOnFlightMode, KEY_EVENT_UP);//chenhe instead it as following
	SetRightSoftkeyFunction(PowerOnNormalMode, KEY_EVENT_UP);
	#endif
}

/*****************************************************************************
* FUNCTION
*   PhnsetFlightModeGetLatestActionMode
*****************************************************************************/
U8 PhnsetFlightModeGetLatestActionMode(void)
{
	U8 data[NVRAM_EF_SYS_CACHE_OCTET_SIZE];
	S16 error;
	ReadRecord(NVRAM_EF_SYS_CACHE_OCTET_LID, 2 , &data, NVRAM_EF_SYS_CACHE_OCTET_SIZE, &error);

	/*if(data[1] != FLIGHTMODE_OFF && data[1] != FLIGHTMODE_SILENT )
	{
		// NVRAM initialization for flight mode settings
		data[0] = FLIGHTMODE_OFF;
		data[1] = FLIGHTMODE_OFF;
		WriteRecord(NVRAM_EF_SYS_CACHE_OCTET_LID, 2 , &data, NVRAM_EF_SYS_CACHE_OCTET_SIZE, &error);
	}*/

	return data[1];
}

void PhnsetFlightModePowerOnException(void)
{
	if(PhnsetFlightModeGetLatestActionMode() == FLIGHTMODE_OFF )
		PowerOnNormalMode();
	else
		PowerOnFlightMode();
}

#endif /* __FLIGHT_MODE_SUPPORT__ */
