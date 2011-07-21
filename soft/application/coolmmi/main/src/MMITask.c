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
 * MMITask.C
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Protocol task functionality.
 *
 * Author:
 * -------
 * -------
 * * 修改记录:
 *       修改日期:20060512
 *       版 本 号:6206.v02
 *       修 改 人:苏世鹏
 *       修改内容:关闭include文件
 *------------------------------------------------------------------------------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: MMItask.c

  	PURPOSE		: Protocol task functionality 

 

 

	DATE		: Dec' 28, 2002

**************************************************************/

/***************************************************************************** 
* Include
*****************************************************************************/

#define __NEWSIMULATOR
#include "mmi_features.h"
#include "csw.h"
#include "stdc.h"
#include "l4dr1.h"

#include "datetimegprot.h"

 
#include "wgui_categories_inputs.h"
#include "imeres.h"
 
#include "globalconstants.h"
#include "taskinit.h"
#include "mmitaskprot.h"
#include "thrdosinterface.h"
#include "globalstruct.h"
#include "debuginitdef.h"
#include "protocolevents.h"
#include "asynceventsgprot.h"
#include "alarmframeworkprot.h"    
#include "callmanagementgprot.h"
#include "callmanagementstruct.h"
#include "settingprofile.h"
#include "profilegprots.h"
#include "service.h"
#include "satgprots.h"
#include "cphsprotocolhandlers.h"
#include "engineermodeprot.h"
#include "factorymodeprot.h"
#include "gpioinc.h"
#include "phonebookgprot.h"
#include "callhistorymain.h"
#include "funandgamesprots.h"
#include "messagesresourcedata.h"
#include "organizergprot.h"
#include "alarmgprot.h"
#include "scheduledpoweronoffgprot.h"
#include "unicodedcl.h"
#include "unicodedef.h"
#include "organizergprot.h"
#include "idleappprot.h"
#include "conversions.h"
#include "nvramprot.h"
#include "mmi.h"
#include "mmiapi.h"
#include "uim.h"
#include "adp_events.h"
#include "adp_eventsredef.h"
#include "adp_timer.h"
#include "protocolevents.h"

#include "mmi_data_types.h"

#ifdef __MMI_THEMES_V2_SUPPORT__
#include "thememanager.h"
#endif

#include "messagesexdcl.h"

#ifdef __MMI_USB_SUPPORT__
#include "usbdevicegprot.h"
   #ifdef MMI_ON_HARDWARE_P
   extern kal_bool INT_USBBoot(void);
   #endif
#endif

#ifdef __MMI_IRDA_SUPPORT__
#include "irdammigprots.h"
#endif


#ifdef __MMI_BT_SUPPORT__
#include "btmmiscrgprots.h"
#endif

#ifdef __MMI_FILE_MANAGER__
#include "filemanagergprot.h"
#include "msdc_def.h"
#endif

#ifdef __MMI_WEBCAM__
#include "mdi_datatype.h"
#include "mdi_webcam.h"
#endif /* __MMI_WEBCAM__ */

#include "soundeffect.h"

#include "shortcutsprots.h"
#include "sublcdhistorygprot.h"
#include "todolistdef.h"
#ifdef JATAAYU_SUPPORT
#endif
#include "dataaccountgprot.h"

#include "wpssprotos.h"
#include "calleridgprot.h"
#include "managecallmain.h"

#if defined(__MMI_STOPWATCH__)
#include "stopwatchgprot.h"
#endif
#if defined(__MMI_EMAIL__)		
#include "emailappgprot.h"
#endif // def __MMI_EMAIL__	
#include "osthreadinterfacegprot.h"

#ifdef MMI_ON_WIN32
extern BOOL flagMMIStart;
#include "buildcfg.h"
#endif
#include "eventsgprot.h"
#include "allappgprot.h"
#include "wrappergprot.h"
#include "queuegprot.h"
#include "keybrd.h"
#include "nvramexdcl.h"
#include "simdetectiongprot.h"
#include "wgui_status_icons.h"
#include "poweronchargerprot.h"
#include "settinggprots.h"
#include "simdetectiongexdcl.h"
#include "phonesetupgprots.h" /* phnsetreadnvramcalibrationdata */
#include "protocolevents.h"

 
#ifdef __MMI_POC__
#include "pocgprot.h"
#endif

#undef __NEWSIMULATOR

#ifdef MMI_ON_HARDWARE_P
//#include "lcd_sw.h"   
#include "lcd_sw_inc.h"
#include "sublcd.h"
#else
#include "lcd_if.h"
#endif
#include "composeringtoneprots.h"

#ifdef __MMI_IMPS__
#include "mmi_imps_gprot.h"
#endif

#include "usb_interface.h"

#ifdef MMI_ON_HARDWARE_P
#include "uctls_m.h"
#endif

 
#include "gdi_layer.h"
#include "gdi_lcd.h"
#include "string.h"
#include "mmitask.h"
#include "mmi_trace.h"
 
#include "lcd_sw.h"
#ifdef MMI_ON_HARDWARE_P
#include "nvram_user_defs.h"
#include "di.h"
#endif
#ifdef __SP__
#include "sp.h"
#endif
#include "gbhmi.h"
#ifdef __MMI_EBOOK_READER__
#include "ebookprot.h"
#endif
#ifdef MMI_ON_HARDWARE_P
#include "mci.h"
#endif
#include "vdoplygprot.h"
#include "cameraapp.h"
#include "imageviewer.h"
#include "fmradioprot.h"
#include "photoeditorgprot.h"
#include "vdorecgprot.h"
#include "vobjects.h"
#include "fmt_main.h"
#include "med_utility.h"
#include "mmithemes.h"
#include "usbdevicedefs.h"
#include "settingprot.h"
#include "eventsdef.h"
//#include "app_ltlcom.h"
#include "dual_sim.h"

#include "unit_test.h"

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
#include "testtype2control.h"
#endif
#ifdef JATAAYU_SUPPORT

#include "jwap_init.h"


extern void initJWap () ;
extern void initJMms () ;
#endif
#include "drv_mmc.h"

#if defined(__MMI_ANALOGTV__)
#include "analogtvapp.h"
extern void InitAnalogTV(void);
#endif
#if defined(MMI_2D_SENSOR_SUPPORT)	
extern void gpio_2d_Sensor_init(void);
extern void csd_init_2d_Sensor_when_system_boot_up(void);
#endif

#if defined(__J2ME__)
#include "javaprot.h"
#endif /* defined(__J2ME__) */ 

#if defined __NATIVE_JAVA__
extern void mmi_native_java_qq_highlight_menu(void);
extern void mmi_native_java_msn_highlight_menu(void);

extern void gcj_TraceOut(int index,char * fmt,...);
#include "java.h"
#include "jam_msg_handler.h"
#include "javadef.h"
#ifdef __MMI_MULTI_SIM__
extern U8 g_mmi_java_sim_id;
#endif
unsigned char gcj_IsJavaOn();
unsigned char gcj_IsForceToQuit=0;
#endif

#if  defined(_KK_SUPPORT_) && defined(__KK_UI_V2__)
extern void mmi_kk_init_app();
#endif	

void *sxr_Wait (u32 *Evt, u8 Sbx);

extern void med_main( ilm_struct *ilm_ptr);
extern kal_bool med_init( task_indx_type task_indx );
extern void SetHandsetInPhone(U8 h_status); 
extern void MMICheckDiskDisplay(void);
extern void NITZHandlerInit(void);
extern VOID DRV_SetStatusSleepIn(BOOL bStatus);
extern void gpio_hall_Sensor_init(void);


#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
extern void CSD_UI_init_bmpbuf();
#endif

//2009-6-20 fengzb add begin
typedef void (* WAP_TIMER_CALLBACK) (UINT32 uiCallbackArg, UINT32 uiTimerID) ; 
typedef struct WAP_TIMER_EVT_
{
	WAP_TIMER_CALLBACK 	cbTimerCallback ; /* Application Callback Function */
	UINT32 			uiCallbackArg ;   /* Argument for cbTimerCallback */
	UINT32 			uiTimerId ;		  /* Platform Specific TimerID */ 
       UINT32 			uiTimerValueMilliSec ; /* Expiry Time in millisecond */
       UINT8  		      bIsSingleShotTimer; /* Used if platform can handle
											 * both single shot and recurrent
											 * timers */
	/* Platform Specific Variables Start */
	/* Platform Specific Variables End */
} WAP_TIMER_EVT ;
//fengzb add end



U8 isIdleScreenReached=0;
U8 IsBeforePowerOn=1;
U8 gInitAllAppFlag=0;
U8 gInsertSimAppFlag=0;
BOOL gAlreadyInitialized = FALSE;
MYTIME StartUpTime, LastDuration;

#if (CSW_EXTENDED_API_PMD_USB == 0)
extern VOID    USB_PreClose(VOID);
extern VOID MCI_USBInit(UINT8 mailBoxId);
#else
extern VOID pm_ForceUpdateChargerStatus(VOID);
extern void SendMessageForUSB(UINT32 action);
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */

extern UINT8 g_Flag0 ;
// Adapter task of MMI layer.
static HANDLE g_AdpTaskHandle = HNULL;
// Main task of MMI layer.
static HANDLE g_MmiTaskHandle = HNULL;
// Multimedia task of MMI layer for support MP3 and MP4 playing.
static HANDLE g_MultiMediaTaskHandle = HNULL;
//Advanced FS task of MMI layer for support FS
static HANDLE g_FmtTaskHandle = HNULL;
static HANDLE g_UsbTaskHandle = HNULL;
#ifdef __J2ME__
static HANDLE g_hJ2MEHandle = HNULL;
#endif
static HANDLE g_BtTaskHandle = HNULL;
static HANDLE g_hTPTask = HNULL;
static HANDLE g_TransportHandle = HNULL;
/**************************************************************

	FUNCTION NAME		: GetMmiTaskHandle

  	PURPOSE				: Get    Task   Handle

	INPUT PARAMETERS  	: module_type mod

	OUTPUT PARAMETERS	: void

	RETURNS				: HANDLE 
	
	REMARKS				: 

**************************************************************/
HANDLE GetMmiTaskHandle(module_type mod)
{
	switch (mod)
	{
		case MOD_MMI:
			return g_MmiTaskHandle;
		case MOD_L4C: // MOD_ADP
#ifdef __MMI_MULTI_SIM__
		case MOD_L4C_2:
		case MOD_L4C_3:
		case MOD_L4C_4:
#endif
			return g_AdpTaskHandle;
		case MOD_MED: // MOD_AUD
			return g_MultiMediaTaskHandle;
		case MOD_TP_TASK:
			return g_hTPTask;			
        case MOD_FMT:
            return g_FmtTaskHandle;
	    case MOD_USB:
            return g_UsbTaskHandle;
#ifdef __J2ME__
		case MOD_J2ME:
      		return g_hJ2MEHandle;
#endif
	    case MOD_BT:
      		return g_BtTaskHandle;
        case MOD_TRANSPORT:
            return g_TransportHandle;
		default:
    		ASSERT(0);
		    return g_MmiTaskHandle;
	}
}


/**************************************************************

	FUNCTION NAME		: SetMmiTaskHandle

  	PURPOSE				: Set  MmiTask  Handle

	INPUT PARAMETERS  	: module_type mod, HANDLE hTask

	OUTPUT PARAMETERS	: void

	RETURNS				: BOOL 
	
	REMARKS				: 

**************************************************************/
BOOL SetMmiTaskHandle(module_type mod, HANDLE hTask)
{
	switch (mod)
	{
		case MOD_MMI:
			g_MmiTaskHandle = hTask;
			return TRUE;
		case MOD_L4C: // MOD_ADP
#ifdef __MMI_MULTI_SIM__
		case MOD_L4C_2:
		case MOD_L4C_3:
		case MOD_L4C_4:
#endif
			g_AdpTaskHandle = hTask;
			return TRUE;
		case MOD_MED: // MOD_AUD
			g_MultiMediaTaskHandle = hTask;
			return TRUE;
		case MOD_FMT:
			g_FmtTaskHandle = hTask;
			return TRUE;
			
		case MOD_USB:
			g_UsbTaskHandle = hTask;
			return TRUE;
			
#ifdef __J2ME__
		case MOD_J2ME:
		  g_hJ2MEHandle = hTask;
		  return TRUE;
#endif
		  
		case MOD_TP_TASK:
			g_hTPTask = hTask;
			return TRUE;
		case MOD_BT:
			g_BtTaskHandle = hTask;
			return TRUE;

        case MOD_TRANSPORT:
            g_TransportHandle = hTask;
            return TRUE;
		default:
			ASSERT(0);
			return FALSE;
	}
}


extern void InitAllApplications(void);
extern void InitializeChargingScr(void);
extern void QuitSystemOperation(void);
extern void mmi_pwron_entry_animation_screen(void);
/* 
 * for MDI 
 */
extern void mdi_audio_init(void);

extern void setup_UI_wrappers(void);

 
extern void InitNvramData(void);
extern void PhnsetInitSetTimeNDateNvram(void);
extern void ReadValueCityNVRAM(void);
 

 
extern void InitAudioPlayerApp();

/* 
 * For Input method
 */
#ifdef __MMI_T9__
   extern void InitInputMethod(void);
#elif defined __MMI_ZI__
   extern void InitZiInputMethod(void);
#elif defined(__MMI_KA__)
   extern void InitKonkaInputMethod(void);
#elif defined(__MMI_ITAP__)
   extern void InitItapInputMethod(void);
#endif

//tangjie add begin 20060802
#ifdef __MMI_GB__
extern void InitGbInputMethod(void);
#endif
//tangjie add end 20060802

 
extern U8 MMI_english_input_mode_set[INPUT_MODE_MAX_NUM];
extern U8 MMI_qsearch_input_mode_set[INPUT_MODE_MAX_NUM];
extern U8 MMI_all_input_mode_set[INPUT_MODE_MAX_NUM];
#ifdef __MMI_PREFER_INPUT_METHOD__
extern U16 MMI_all_prefered_input_mode_set[INPUT_MODE_MAX_NUM];
#endif
extern const sIMEModeDetails *gIMEModeArray;
extern const sIMEModeDetails *gIMEQSearchModeArray;
extern const U8 MMI_implement_input_mode_set[];
 
extern void mmiapi_init_engine(void);

/* 
 * Move InitPhoneSetupApp() to InitializeAll() for text encoding 
 */
extern void InitPhoneSetupApp(void);  
extern void DTGetRTCTime(MYTIME *t);
extern void InitPhoneSetupCntx(void);
extern void InitAppMem(void);
extern void CheckAndPrintMsgId(U16 Id);
extern void mmi_fng_highlight_setting_game(void);
extern void mmi_game_highlight_app(void);
#if defined(__MMI_NOKIA_STYLE_N800__)
//extern void mmi_java_game_highlight_app(void);
#endif
#ifdef EARPHONE_DETECT_BY_INTERRUPT
extern void EarphoneDetectGpioInit(void);
#endif

#if defined(__MMI_VRSD__)
extern void mmi_vrsd_init(void);
#endif

#if defined(__MMI_VRSI__)
extern	mmi_vrsi_init(void);
#endif

#if defined( __MMI_SOUND_RECORDER__)
extern void InitSoundRecorderApp(void);
#endif

extern void mmi_phb_sos_list_init(void);
#ifdef __MMI_MESSAGES_CHAT__	
extern void InitializeChatApp(void);
#endif
#ifdef __MMI_IRDA_SUPPORT__
extern void mmi_irda_init_menu_hdlr(void);
#endif
#if (defined (__MMI_BT_SUPPORT__))
extern void mmi_bt_scr_init(void);
extern void mmi_bt_init_menu_highlight_hdlr(void);
#endif
extern void ShctInit(void);
extern void mmi_frm_sms_init(void);  

 
#ifdef __FLIGHT_MODE_SUPPORT__
extern void PowerOnFlightMode(void);
extern void PowerOnNormalMode(void);
extern void EntryQueryFlightModeScreen(void);
extern void PhnsetFlightModeQueryBackground(void);  
extern void PhnsetFlightModePowerOnException(void);
#endif
 
extern void HalfModeBacklight(void);//modify by dongwg for backlight when press keypad 20070626

#include "l4dr.h"
#include "mmitaskprot.h"
#include "protocolevents.h"
extern void InitializeResourceVariables(void);
/* 
 * CSD added for two kinds of timer, one is for exactly time, another allow to delay.
 */
kal_uint8 				keypad_flag = 0;

//add by wuzc at 20061031
extern void FlightModeCheckBeforeAnimation(U8 flightmode);
//add by wuzc at 20061031
extern void ShutdownSystemOperation(void);
//extern void SetHandsetInPhone(U8);
void ChargePowerOn();
extern BOOL g_BeforeEntryIdleScr;
extern BOOL g_UsbChargerPowerOn;
extern HANDLE protectKeyCnt;
extern 	UINT32 COS_SendEventToJataayu( COS_EVENT* pCosEvent ) ;

kal_uint32  timer_count=0;
#ifndef MMI_ON_WIN32
#include "uctls_m.h"
extern volatile BOOL gUsbDetected;
extern void SendMessageForUSB(UINT32 action);
#ifdef __MMI_USB_DOWNLOAD_FR__
extern BOOL pm_GetUsbDownLoadStatus(VOID);
#endif
INT32 mmi_getCurrentPowerMode(void)
{
       mmi_trace(1," $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$mmi_getCurrentPowerMode = %d   ",g_pwr_context[0].PowerOnMode);
	return g_pwr_context[0].PowerOnMode;
}
extern void DateTimerProc(void);
extern u8 lcd_backlight;
extern void CloseBacklight(void);
extern void mmi_init_pwr_context(void);
#if defined(__PROJECT_GALLITE_C01__)
BOOL g_poweron_alarm = 0;
#endif
#ifdef __NATIVE_JAVA__
void gcj_DispatchMsg(COS_EVENT ev)
{
	
	if (ev.nEventId == MSG_ID_MMI_J2ME_EXIT_SCREEN_REQ) 
	{
		if(GetActiveScreenId() == SCR_JAVA)
			GoBackHistory();
		HideStatusIcon(STATUS_ICON_JAVA_PAUSE);
		UpdateStatusIcons();		
		gcj_IsForceToQuit=0;
	}
	else if (gcj_StubIsForceToQuit()  || !gcj_IsJavaOn())
	{
			return;
	}
	else if (ev.nEventId == MSG_ID_MMI_JAVA_PLAY_TONE_REQ)
	{
		unsigned int tmp = MSG_ID_MMI_JAVA_PLAY_TONE_REQ;
		mmi_java_play_tone_req_struct *data =  (mmi_java_play_tone_req_struct *)ev.nParam1;
		gcj_TraceOut(0,"MSG_ID_MMI_JAVA_PLAY_TONE_REQ =%d,len=%d\n",tmp,data->data_len);
		if (data->tone_or_audio == 1)
		{
			int ret =MDI_AUDIO_NO_SOUND;
			if( !IsSilentModeActivated() 
				#ifdef __MMI_SILENT_MEETING_PROFILE__
				&&(!IsMeetingModeActivated())
				#endif
				)
			{

				ret = mdi_audio_play_string_with_vol_path(data->audio_date,data->data_len, MDI_FORMAT_MID, DEVICE_AUDIO_PLAY_ONCE, NULL, data->handler, data->volume, 0);
			}
			else
				PlayVibratorOnce();
			if (data->data_len == 54 && gcj_IsJavaOn() == 1 && GetActiveScreenId() != SCR_JAVA)
			{
				BlinkStatusIcon(STATUS_ICON_JAVA_PAUSE);
				UpdateStatusIcons();
			}
                     if(!ret)
			    data->handler(ret);
		}	
		else if (data->tone_or_audio == 0)
		{
			playRequestedTone(data->tone_type);
			data->handler(1);
		}
		else
			abort();
	}
#ifdef JATAAYU_SUPPORT	
	else if(ev.nEventId == MSG_ID_MMI_JAVA_RUN_WAP_REQ)
	{
		//char *unicode_url= COS_MALLOC(256);
		//AnsiiToUnicodeString((S8 *)unicode_url, asiicc_url);
		//JC_CHAR *url	= jdi_CUtilsTcsDuplicate(JC_NULL,(const JC_CHAR *)unicode_url) ;

		JC_CHAR *url  = jdi_CUtilsTcsDuplicate(JC_NULL,(const JC_CHAR *)ev.nParam1) ;
	#ifdef __MMI_MULTI_SIM__
			jWap_LaunchFromIdle(1, g_mmi_java_sim_id, (JC_CHAR *)url);
	#else
			jWap_LaunchFromIdle(1, 0, (JC_CHAR *)url);
	#endif
	}
#endif	
	else if (ev.nEventId == MSG_ID_GCJ_ENTRY_NATIVE_EDITER_REQ) 
	{
		_NATIVE_EDITER_T *data = (_NATIVE_EDITER_T *)ev.nParam1;
		gcj_entry_native_input(data->maxLen);
	}
	else if (ev.nEventId == MSG_ID_GCJ_PREPARE_GPRS_REQ)
	{
		int ret = gcj_PrepareNetwork();
		_NET_EVENT_T *data = (_NET_EVENT_T *)ev.nParam1;
		////*(data->result) = ret;
		//data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_STOP_GPRS_REQ)
	{
		int ret = gcj_StopGprs();
		_NET_EVENT_T *data = (_NET_EVENT_T *)ev.nParam1;
		////*(data->result) = ret;
		//data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_GET_TCPIP_SOCKET_REQ)
	{
		int ret = gcj_GetTcpipSocket();
		_NET_EVENT_T *data = (_NET_EVENT_T *)ev.nParam1;
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_SET_SOCKET_OPT_REQ)
	{
		_NET_EVENT_SET_OPT_T *data = (_NET_EVENT_SET_OPT_T *)ev.nParam1;
		int ret = gcj_SetSocketOpt(data->socketId,data->option,data->value,data->value_size);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_GET_HOST_NAME_REQ)
	{
		_NET_EVENT_GET_HOST_T *data = (_NET_EVENT_GET_HOST_T *)ev.nParam1;
		int ret = gcj_GetHostName((const kal_char *)data->domain, data->address, data->adress_len);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_SOCKET_CONNECT_REQ)
	{
		_NET_EVENT_SOC_CON_T *data = (_NET_EVENT_SOC_CON_T *)ev.nParam1;
		int ret =  CFW_TcpipSocketConnect (data->nSocket, data->pName, data->nNameLen);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_SOCKET_READ_REQ)
	{
		_NET_EVENT_SOC_READ_T *data = (_NET_EVENT_SOC_READ_T *)ev.nParam1;
		int ret =  CFW_TcpipSocketRead(data->nSocket, data->inBuf, data->maxLen);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_SOCKET_WRITE_REQ)
	{
		_NET_EVENT_SOC_WRITE_T *data = (_NET_EVENT_SOC_WRITE_T *)ev.nParam1;
		int ret =  CFW_TcpipSocketWrite (data->nSocket, data->outBuf, data->writeLen);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_SOCKET_CLOSE_REQ)
	{
		_NET_EVENT_SOC_CLOSE_T *data = (_NET_EVENT_SOC_CLOSE_T *)ev.nParam1;
		int ret =  CFW_TcpipSocketClose(data->nSocket);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else
		abort();
}

void gcj_DispatchFileMsg(COS_EVENT ev)
{
	if (gcj_StubIsForceToQuit() || !gcj_IsJavaOn())
	{
		return;
	}
	if (ev.nEventId == MSG_ID_GCJ_FILE_OPEN_REQ)
	{
		_FILE_EVENT_OPEN_T *data = (_FILE_EVENT_OPEN_T *)ev.nParam1;
		int ret = gcj_CheckDir();
		if (ret != -1)
			ret =  MMI_FS_Open(data->unicodeName,data->flag);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_FILE_CLOSE_REQ)
	{
		_FILE_EVENT_CLOSE_T *data = (_FILE_EVENT_CLOSE_T *)ev.nParam1;
		int ret = gcj_CheckDir();
		if (ret != -1)
			ret =  MMI_FS_Close(data->file);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_FILE_READ_REQ)
	{
		_FILE_EVENT_READ_T *data = (_FILE_EVENT_READ_T *)ev.nParam1;
		int ret = gcj_CheckDir();
		if (ret != -1)
			ret =  MMI_FS_Read(data->FileHandle,data->DataPtr,data->Lenth,data->ReadLen);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_FILE_WRITE_REQ)
	{
		_FILE_EVENT_WRITE_T *data = (_FILE_EVENT_WRITE_T *)ev.nParam1;
		int ret = gcj_CheckDir();
		if (ret != -1)
			 ret =  MMI_FS_Write(data->FileHandle,data->DataPtr,data->Lenth,data->Written);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_FILE_SEEK_REQ)
	{
		_FILE_EVENT_SEEK_T *data = (_FILE_EVENT_SEEK_T *)ev.nParam1;
		int ret = gcj_CheckDir();
		if (ret != -1)		
			ret =  MMI_FS_Seek(data->FileHandle,data->Offset,data->Whence);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_FILE_CHANGE_SZIE_REQ)
	{
		_FILE_EVENT_CHANGE_SIZE_T *data = (_FILE_EVENT_CHANGE_SIZE_T *)ev.nParam1;
		int ret = gcj_CheckDir();
		if (ret != -1)
			ret =  FS_ChangeSize(data->FileHandle,data->nFileSize);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_FILE_GET_FILE_SZIE_REQ)
	{
		_FILE_EVENT_GET_FILE_SIZE_T *data = (_FILE_EVENT_GET_FILE_SIZE_T *)ev.nParam1;
		int ret = gcj_CheckDir();
		if (ret != -1)
			ret =  MMI_FS_GetFileSize(data->FileHandle,data->nFileSize);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_FILE_GET_FILE_NAME_REQ)
	{
		_FILE_EVENT_GET_FILE_NAME_T *data = (_FILE_EVENT_GET_FILE_NAME_T *)ev.nParam1;
		int ret = gcj_CheckDir();
		if (ret != -1)
			ret =  MMI_FS_GetFileName(data->FileHandle,data->iNameBufferLen,data->FileName);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_FILE_DISK_INFO_REQ)
	{
		_FILE_EVENT_DISK_INFO_T *data = (_FILE_EVENT_DISK_INFO_T *)ev.nParam1;
		int ret = gcj_CheckDir();
		if (ret != -1)
			ret =  MMI_FS_GetDiskInfo(data->DriveName,data->DiskInfo,data->Flags);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_FILE_GET_FIRST_REQ)
	{
		_FILE_EVENT_FIND_FIRST_T *data = (_FILE_EVENT_FIND_FIRST_T *)ev.nParam1;
		int ret = gcj_CheckDir();
		if (ret != -1)
			ret =  MMI_FS_FindFirst(data->NamePattern,data->Attr,data->AttrMask,data->FileInfo,data->FileName,data->MaxLength);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_FILE_GET_NEXT_REQ)
	{
		_FILE_EVENT_FIND_NEXT_T *data = (_FILE_EVENT_FIND_NEXT_T *)ev.nParam1;
		int ret = gcj_CheckDir();
		if (ret != -1)
			ret =  MMI_FS_FindNext(data->FileHandle,data->FileInfo,data->FileName,data->MaxLength);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_FILE_GET_CLOSE_REQ)
	{
		_FILE_EVENT_FIND_CLOSE_T *data = (_FILE_EVENT_FIND_CLOSE_T *)ev.nParam1;
		int ret = gcj_CheckDir();
		if (ret != -1)
			ret =  MMI_FS_FindClose(data->FileHandle);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_FILE_DELETE_REQ)
	{
		_FILE_EVENT_DELETE_T *data = (_FILE_EVENT_DELETE_T *)ev.nParam1;
		int ret = gcj_CheckDir();
		if (ret != -1)
			ret =  MMI_FS_Delete(data->unicodeName);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else if (ev.nEventId == MSG_ID_GCJ_FILE_RENAME_REQ)
	{
		_FILE_EVENT_RENAME_T *data = (_FILE_EVENT_RENAME_T *)ev.nParam1;
		int ret = gcj_CheckDir();
		if (ret != -1)
			ret =  MMI_FS_Rename(data->unicodeOName,data->unicodeNName);
		//*(data->result) = ret;
		data->finish(data->semp,data->result,ret);
	}
	else
		abort();
}
#endif
COS_EVENT mmi_ev_debug;

void BAL_MmiMainTaskEntry(void *pData)
{
	COS_EVENT ev;
    BOOL bRet;
    MMI_PROC_INFO Info;
    MMI_PROC_INFO* pInfo=NULL;
    ilm_struct* Message=NULL;
    bRet = TRUE;
#if (CSW_EXTENDED_API_PMD_USB == 0)
    USB_PreClose();
#endif // ! CSW_EXTENDED_API_PMD_USB
    SUL_ZeroMemory32( &ev, sizeof(COS_EVENT) );
    SUL_ZeroMemory8( &Info, sizeof(MMI_PROC_INFO) );
    pInfo = &Info;

#ifdef MMI_MESSAGE_TRACE

	MMIMessageNameCheck();                                    // add by elisa to check it message name returned by geteventname are right 2009-6-9

#endif //MMI_MESSAGE_TRACE
 	
 	mmi_init_pwr_context();

	//protectKeyCnt = COS_CreateSemaphore(1);
  
    for(;;) 
    {
		MMI_FreeEvent(&ev);   
		GET_COSMSG(MOD_MMI, &ev);
		mmi_ev_debug = ev;
		
#ifdef __NATIVE_JAVA__
		CheckAndRemoveAllPending();
		if (ev.nEventId >= MSG_ID_J2ME_CODE_BEGIN && ev.nEventId < MSG_ID_J2ME_CODE_CHECK_POINT && ev.nEventId != MSG_ID_MMI_JAVA_SHOW_ALERT_USER_DIALOG_REQ )
		{
			gcj_DispatchMsg(ev);
			continue;
		}
#endif
#ifdef JATAAYU_SUPPORT
            //2009-6-19 add
             if (ev.nEventId  >=MSG_ID_JAT_TIMER_IND_0 && ev.nEventId <=MSG_ID_JAT_TIMER_IND_9)
		{
			WAP_TIMER_EVT *timer;
			timer = (WAP_TIMER_EVT *)ev.nParam1;
                     timer_count++;
			//mmi_trace(1,"jdd_Timer MMI Get wap timer event,  timerID is %d  timer_count=%d cbTimerCallback=%x uiCallbackArg=0x%x \n",timer->uiTimerId,timer_count,timer->cbTimerCallback,timer->uiCallbackArg);
                     timer->cbTimerCallback(timer->uiCallbackArg,timer->uiTimerId);
		}


            if ( MSG_ID_TRANSP_TOMMI_IND == ev.nEventId  )
            {
                UINT32 nRet = COS_SendEventToJataayu( &ev);
            	if( ERR_SUCCESS != nRet )
            	{
            		COS_SetLastError( nRet );
            //			return FALSE;
            	}
            	
            	ev.nParam1 = 0;
            	continue;
            }
#endif

		Message = (ilm_struct*)ev.nParam1;

			
		if(ev.nEventId == EV_TIMER)
		{
			UINT16    timerid;
			timerid = (UINT16) ev.nParam1;

			if (timerid ==TIMER_REAL_TIME_CLOCK)
			{
		                TM_SYSTEMTIME pSystemTime;
			          SUL_ZeroMemory8(&pSystemTime, SIZEOF(TM_SYSTEMTIME));
		                ilm_struct  ilm_ptr_REAL_TIME; 
                        
			          bRet = TM_GetSystemTime(&pSystemTime);
			          if(FALSE == bRet)
			          {
						StartTimer(TIMER_REAL_TIME_CLOCK, 1000, DateTimerProc);  
						return;
			          }
				  mmi_eq_clock_tick_ind_struct *pREAL_TIME=OslMalloc(sizeof(mmi_eq_clock_tick_ind_struct));
		                ilm_ptr_REAL_TIME.local_para_ptr = (local_para_struct *)pREAL_TIME;    
                        
		                pREAL_TIME->rtc_time.rtc_year = pSystemTime.uYear - 2000;
		                pREAL_TIME->rtc_time.rtc_mon =pSystemTime.uMonth;
		                pREAL_TIME->rtc_time.rtc_day =pSystemTime.uDay;
		                pREAL_TIME->rtc_time.rtc_hour =pSystemTime.uHour;
		                pREAL_TIME->rtc_time.rtc_min =pSystemTime.uMinute;
		                pREAL_TIME->rtc_time.rtc_sec =pSystemTime.uSecond;
		                pREAL_TIME->rtc_time.rtc_wday =pSystemTime.uDayOfWeek;
		                ilm_ptr_REAL_TIME.dest_mod_id = MOD_MMI;
		                ilm_ptr_REAL_TIME.src_mod_id = MOD_MMI;
		                ilm_ptr_REAL_TIME.msg_id = MSG_ID_MMI_EQ_CLOCK_TICK_IND;
		                OslMsgSendExtQueue(&ilm_ptr_REAL_TIME);
			}

			ExecCurrTimerHandler(timerid);
			ev.nParam1 = 0;
			continue;
		}

#ifdef MMI_MESSAGE_TRACE
		mmi_trace(g_sw_DEBUG, TSTXT("GET A MESSAGE, TYPE:MMI,direction:%s -> %s, ID:%d  [%s]"),
			GetModName((UINT32)(Message->src_mod_id)),GetModName((UINT32)(Message->dest_mod_id)),
			Message->msg_id, GetMEGName(Message->msg_id));
#endif
		switch (Message->msg_id)
		{
	       	case MSG_ID_MMI_EQ_POWER_ON_IND:
			{
#ifdef MMI_MESSAGE_TRACE
                         mmi_trace(g_sw_trace_check, TSTXT("#####  001 %s[%d\n]\n"), EVID_CASE(MSG_ID_MMI_EQ_POWER_ON_IND), MSG_ID_MMI_EQ_POWER_ON_IND);
#endif
				mmi_eq_power_on_ind_struct *ppower_on = (mmi_eq_power_on_ind_struct *)Message->local_para_ptr;
				SetDateTime((void*) &(ppower_on->rtc_time)); 
				 
				SetDateTime((void*) &(ppower_on->rtc_time));
				IsBeforePowerOn = 0;
				
		#ifdef __MMI_MULTI_SIM__
			MTPNP_AD_Init_BootupDM(ppower_on->dm_setting);
			if(MTPNP_AD_Is_Flight_Mode())
				g_pwr_context[0].actFlightMode = FLIGHTMODE_SILENT;
			else
				g_pwr_context[0].actFlightMode = FLIGHTMODE_OFF;
		#else
			#ifdef __ADP_MULTI_SIM__
			g_pwr_context[0].actFlightMode = ppower_on->flightmode_state;
			#endif
		#endif
#if (CSW_EXTENDED_API_PMD_USB != 0)
            if(ppower_on->poweron_mode == POWER_ON_KEYPAD)
            {
                uctls_SetMode(UCTLS_ID_1);
            }
            else
            {
                uctls_SetMode(UCTLS_ID_3);
            }
            if(uctls_GetStatus() != UCTLS_STATUS_CLOSE)
            {
                pm_ForceUpdateChargerStatus();
            }
#endif /* (CSW_EXTENDED_API_PMD_USB != 0) */

            mmi_trace(TRUE, "POWER_ON mode = %d", ppower_on->poweron_mode);
				switch (ppower_on->poweron_mode)
				{
					case POWER_ON_KEYPAD:
						g_charbat_context.PowerOnCharger = 0;/*qiff modify for greenstone,avoid display "charger complete" windows when KBD power on*/
						mmi_trace(g_sw_SYSTEM, ">>>>> come here POWER_ON_KEYPAD>>>>\n");
						g_pwr_context[0].PowerOnMode=POWER_ON_KEYPAD;
						g_pwr_context[0].PowerOnPeriod=1;
						
						/*add by chenqiang for power on when Charger Connected to display Charging B*/
						{
						  BOOL ret;
						  UINT8 pBcs;
						  UINT8 pBcl;
						  UINT16 pMpc;

						  ret = PM_GetBatteryInfo(&pBcs, &pBcl, &pMpc);
						  if (ret == TRUE)
						  {
						    if (pBcs == 1)
						    {
						      g_charbat_context.isChargerConnected = 1;
							
						    }

						  }
						}
						/*add by chenqiang for power on when Charger Connected to display Charging E*/
						
						/*Guojian Add Start For 10810 Ver: GreenStone  on 2008-12-19 13:59 */
						InitializeResourceVariables(); 
						MMICheckDiskDisplay();  //display logo
						/*Guojian Add End  For 10810 Ver: GreenStone  on 2008-12-19 13:59 */
						ResetAbnormalReset();
						InitializeAll();   
						/************************************** 
				 		*   To initialize nvram data
						***************************************/
						InitNvramData();

						#if 1
						UINT8 marker;
						FM_GetValidMarker(&marker);
						if(5 == marker)
						{
							EntryAutoTest();
							return;
						}
						#endif
                        
                                        {          
                                        #if 0
                                        	U8 data[NVRAM_EF_SYS_CACHE_OCTET_SIZE];
                                    	S16 error;
                                        ReadRecord(NVRAM_EF_SYS_CACHE_OCTET_LID, 2 , &data, NVRAM_EF_SYS_CACHE_OCTET_SIZE, &error);
                                        mmi_trace(1,",power on,FlightMode is %d,data[1] is %d",mmi_bootup_get_active_flight_mode(),data[1]);
						FlightModeCheckBeforeAnimation(data[0]);
						#else
#if 0

					if (CFW_GetFlightMode(&flightModeSet) == ERR_SUCCESS)
					{
						FlightModeCheckBeforeAnimation(flightModeSet);
					}
					else
#endif
						{
							mmi_trace(g_sw_SYSTEM,"get flight mode error");
							#ifdef __MMI_TOUCH_SCREEN__
							if (PhnsetNeedFactoryDefaultCalibration())
							{
								EntryPhnseFactoryDefaultCalibrationScr(); /* need to calibrate for touch screen */
							}
							else
							#endif
							FlightModeCheckBeforeAnimation(FLIGHTMODE_OFF);
						}
						#endif
                                        }

						TurnOnBacklight(1);
						break;
						
					case POWER_ON_CHARGER_IN:
						/************************************** 
						* Always send charger-in indication
						* to avoid fast repeating charger 
						* in-out b4 power-on completes
						* Lisen 04/13/2004
						***************************************/
						mmi_trace(g_sw_SYSTEM, ">>>>> come here POWER_ON_CHARGER_IN, g_charbat_context.isChargerConnected = %d>>>>\n", g_charbat_context.isChargerConnected);
#if (CSW_EXTENDED_API_PMD_USB == 0)
#if 0
						if(	USB_DETECTED_OK == MCI_USBDetect() )
						{
							goto usb_case;
						}
						else
#endif
#endif /* (CSW_EXTENDED_API_PMD_USB != 0) */
						{
							g_UsbChargerPowerOn = 1;
							g_pwr_context[0].PowerOnMode = POWER_ON_CHARGER_IN;
							g_charbat_context.isChargerConnected = 1;
							g_charbat_context.PowerOnCharger=1;
							ChargePowerOn();
							TurnOnBacklight(1);
#ifdef __MMI_USB_DOWNLOAD_FR__
						if(USB_DETECTED_OK == MCI_USBDetect())
						{
						if(pm_GetUsbDownLoadStatus())
							{
							SendMessageForUSB(DEVUSB_DETECT_ACTION_PLUGIN);
							}
						}
#endif
							//delete by chenqiang for bug 12534(led flash when begain charging with power off) B
							//lcd_screen_on(MAIN_LCD,TRUE);
	                      			//lcd_bright_level(MAIN_LCD,7);
							//delete by chenqiang for bug 12534(led flash when begain charging with power off) E
						}
						break;
				
					case POWER_ON_ALARM: 
#if defined(__PROJECT_GALLITE_C01__)
						g_poweron_alarm = 1;
#endif
						mmi_trace(1, ">>>>> come here POWER_ON_ALARM>>>>\n");
						g_pwr_context[0].PowerOnMode = POWER_ON_ALARM;
						InitializeResourceVariables(); //add by panxu 2007-8-10
						MMICheckDiskDisplay();  //display logo
						AlmInitRTCPwron(); 
						TurnOnBacklight(1); //qiff modify for m301

						PRINT_INFORMATION((" ALARM POWER ON SUCCESS!!!!"));
						break;
			
					case POWER_ON_EXCEPTION:
						mmi_trace(1, ">>>>> come here POWER_ON_EXCEPTION>>>>\n");
						g_pwr_context[0].PowerOnMode = POWER_ON_EXCEPTION;
						InitializeResourceVariables(); 
					//	MMICheckDiskDisplay();  //display logo
						InitializeAll();
						SetAbnormalReset();

						ClearAllKeyHandler();
						ClearKeyHandler(KEY_END, KEY_LONG_PRESS);	
			
						/**************************************
						* Please note that the order of the following actions matters. 
						* NVRAM data must be read before InitAllApplications, or there might be 
						* unexpected errors. 
						***************************************/
						InitNvramData();	
						AlmReInitialize();
//						InitAllApplications();

#if 0
						if(CFW_GetFlightMode( &flightModeSet ) == ERR_SUCCESS)
						{
							FlightModeCheckBeforeAnimation(flightModeSet);
						}
						else
#endif
						{
							mmi_trace(g_sw_SYSTEM,"get flight mode error");
#if 0
							#ifdef __MMI_TOUCH_SCREEN__
							if (PhnsetNeedFactoryDefaultCalibration())
							{
								EntryPhnseFactoryDefaultCalibrationScr(); /* need to calibrate for touch screen */
							}
							else
							#endif
#endif /* Guoj delete. It's not used now at 2009-12-11 */
							FlightModeCheckBeforeAnimation(FLIGHTMODE_OFF);
						}					

						/***************************************
						* To check power on display if need restore 
						***************************************/
						mmi_pwron_exception_check_display();
// The MMI shall call the function DRV_SetStatusSleepIn(TRUE) to declare that the LCD is closed when LCD close,
// so the g_bSleepIn change to TRUE, and then res SDMMC could be released.
						DRV_SetStatusSleepIn(TRUE);
// Release MMI LP resource, otherwise, system will not go into LPM after pwron_exception
						{
						// Assume LCD backlight is half-on
						lcd_backlight = 1;
						// Tell MMI to close backlight (and turn it on later if there is a wakeup event)
						CloseBacklight();
						}
						csw_SetResourceActivity(CSW_LP_RESOURCE_MMIACTIVE, CSW_SYS_FREQ_32K);
						break;
						
						#ifdef __MMI_USB_SUPPORT__
#if (CSW_EXTENDED_API_PMD_USB == 0)
#if 0
	usb_case:
#endif
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */
				case  POWER_ON_USB:
						g_pwr_context[0].PowerOnMode=POWER_ON_USB;
						/***************************************
						* Because Aux task will not init in USB boot mode 
						* Interrupt service routine for clam detection CLAM_EINT_HISR() 
						* is not register, force the clam state to open 
						* If Aux task is necessary in USB mode, this tircky could be removed 
 
						***************************************/
						gMMIClamState=1;
						InitializeResourceVariables(); 
						InitializeUsbScr();
						/***************************************
						* To disable keypad tone state 
						***************************************/
						mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);
						mmi_trace(1,"POWER_ON_USB");
						break;
						#endif	

					default:
						break;
				}
				break;
	       	}	
//wug modify used to cs200 start 20070821
			case MSG_ID_MMI_EQ_GPIO_DETECT_IND:     //Added by jinzh start:20070730
			/* Shall uss protocol event handler after received power on indication */
			//GpioDetectInd((void*)Message.oslDataPtr);
#ifdef MMI_MESSAGE_TRACE
			   mmi_trace(g_sw_trace_check, TSTXT("#####   002 %s[%d\n]\n"), EVID_CASE(MSG_ID_MMI_EQ_GPIO_DETECT_IND), MSG_ID_MMI_EQ_GPIO_DETECT_IND);
#endif
			if(IsBeforePowerOn)
			{TRACE_EF(g_sw_MIDI,"初始化耳机");
				mmi_eq_gpio_detect_ind_struct *gpioDetectInd = (mmi_eq_gpio_detect_ind_struct *) Message->local_para_ptr;
				
				switch(gpioDetectInd->gpio_device)
				{
					case EXT_DEV_EARPHONE:   
						if(gpioDetectInd->on_off==1)
						{                  

							SetHandsetInPhone(1);
							#if (defined (__MMI_STATUS_ICON_EN_EARPHONE__)|| defined(__MMI_BT_SUPPORT__))
							TRACE_EF(g_sw_MIDI,"初始化耳机KKKKKKKKKKKK");
							IdleSetStatusIcon(STATUS_ICON_EARPHONE_INDICATOR);
							#endif
						}
						else
						{
							SetHandsetInPhone(0);
						}
						//SendAudioModeRequest( isEarphonePlugged );
					/*Jinzh Modify Start For 6424 Ver: TBM780  on 2007-8-28 17:37 */
					SendAudioModeRequest( isEarphonePlugged_1 );
					/*Jinzh Modify End  For 6424 Ver: TBM780  on 2007-8-28 17:37 */
						break;

					#ifdef __MMI_CLAMSHELL__
					case EXT_DEV_CLAM_OPEN:
						gMMIClamState=1;
						break;
					case EXT_DEV_CLAM_CLOSE:
						gMMIClamState=0;
						break;
					#endif /* __MMI_CLAMSHELL__ */   

					default:
					break;
				}
			}
			else
			{
			TRACE_EF(g_sw_MIDI,"初始化耳机");
				ProtocolEventHandler(Message->msg_id,(void*)Message->local_para_ptr,(int)Message->src_mod_id, (void*) Message->peer_buff_ptr);
			}

		break;			//Added by jinzh end:20070730
//wug modify used to cs200 end 20070821
	    	case MSG_ID_MMI_EQ_KEYPAD_DETECT_IND:
				//COS_WaitForSemaphore(protectKeyCnt,COS_WAIT_FOREVER); 
#ifdef MMI_MESSAGE_TRACE
				mmi_trace(g_sw_trace_check, TSTXT("#####  003 %s[%d\n]\n"), EVID_CASE(MSG_ID_MMI_EQ_KEYPAD_DETECT_IND), MSG_ID_MMI_EQ_KEYPAD_DETECT_IND);
#endif
				dm_dealedKey();
				//COS_ReleaseSemaphore(protectKeyCnt);
				
				//HalfModeBacklight();//modify by dongwg for backlight when press keypad 20070626
				COS_KeyEventHandler(Message);
				break;	
/******************chenhe add for usb app+++**************************/
//#define PRT_EQ_USB_DETECT_IND 1725
#ifdef __MMI_USB_SUPPORT__
		case PRT_EQ_USB_DETECT_IND:
		{
			mmi_trace(1,"mmi task receive,PRT_EQ_USB_DETECT_IND");
			mmi_eq_usbdetect_ind_struct *p=(mmi_eq_usbdetect_ind_struct*) Message->oslDataPtr;
			if( IsBeforePowerOn )
			{
				if( p->action == DEVUSB_DETECT_ACTION_PLUGIN ) 
					SetUsbBeforePowerOnFlag(MMI_TRUE);
				else if( p->action == DEVUSB_DETECT_ACTION_PLUGOUT )
					SetUsbBeforePowerOnFlag(MMI_FALSE);
			}
			else
			{
				ProtocolEventHandler(Message->msg_id,(void*)Message->local_para_ptr,(int)Message->src_mod_id, (void*) Message->peer_buff_ptr);		}
			}
		break;
#endif	
/******************chenhe add for usb app---**************************/
		
			
	    	default:
			{
				ProtocolEventHandler(Message->msg_id,(void*)Message->local_para_ptr,(int)Message->src_mod_id, (void*) Message);
	          	break;
			}
	}

	MMI_Free_msg_buf(Message);
	ev.nParam1 = 0;
    }
}
#else
void BAL_MmiMainTaskEntry(void *pData)
{
	MYQUEUE Message;
	oslMsgqid qid;

	extern MYQUEUE* qPtr;
	while (1) 
	{
		
		qid=task_info_g1[(U16)pData].task_ext_qid;
		OslReceiveMsgExtQ(qid, &Message);
		if (!ProcessPCKeyEvent(Message.oslMsgId, (KEYBRD_MESSAGE *)Message.oslDataPtr))
		{
			switch(Message.oslMsgId)
			{
			case 1399:
				{
					mmi_eq_power_on_ind_struct *p=(mmi_eq_power_on_ind_struct*) Message.oslDataPtr;
					gdi_init();
					IsBeforePowerOn=0;
					switch(p->poweron_mode)
					{
					case POWER_ON_KEYPAD:
						OslMemoryStart(TRUE);
#ifdef MMI_ON_WIN32
						if (flagMMIStart) break;	
						flagMMIStart=TRUE;
#endif
						g_charbat_context.PowerOnCharger=0;
						InitializeAll();
						OslDumpDataInFile();
						/***************************************
						* To initialize nvram data
						***************************************/
						InitNvramData();							
						
						 
						//#ifdef __FLIGHT_MODE_SUPPORT__
						//EntryQueryFlightModeScreen();
						//#else

            
						mmi_pwron_entry_animation_screen(); /* to display power on animation */
//		ClearAllKeyHandler();
	//	ClearKeyHandler(KEY_END,KEY_LONG_PRESS);
    //
	//	InitAllApplications();
//exit_custom_startup_screen();
	//					EntryIdleScreen();
						//#endif
						
						PRINT_INFORMATION((" NORMAL POWER ON SUCCESS!!!!"));
						break;
					case POWER_ON_CHARGER_IN:
						InitializeChargingScr();
						break;
					case POWER_ON_ALARM:
						AlmInitRTCPwron();
						break;
						/***************************************
						* For abnormal reset when invalid SIM 
						***************************************/
					case POWER_ON_EXCEPTION:
						OslMemoryStart(TRUE);
						SetAbnormalReset();
						InitializeAll();
						OslDumpDataInFile();
						PRINT_INFORMATION((" Exception POWER ON SUCCESS!!!!"));
						//goto_opening_screen();
						ClearAllKeyHandler();
						ClearKeyHandler(KEY_END,KEY_LONG_PRESS);	
						InitNvramData();	
						AlmReInitialize();
						InitAllApplications();
						
						/***************************************
						* To check power on display if need restore 
						***************************************/
						mmi_pwron_exception_check_display();
						break;
					}
				}
				break;
				
			default:
				ProtocolEventHandler((U16)Message.oslMsgId,(void*)Message.oslDataPtr,(int)Message.oslSrcId, (void*) Message.oslPeerBuffPtr );
				break;
			}
		}
		//OslFreeInterTaskMsg(&Message);
		
		qPtr = (MYQUEUE*)&Message;
		if(qPtr->oslSrcId==MOD_L4C)
		{
				qPtr->oslSrcId=MOD_PRT;
				qPtr->oslDestId=MOD_PST;
				qPtr->oslMsgId = 0;
				OslMsgSendExtQueue(qPtr);
		}
		if(qPtr->oslSrcId==MOD_L4C_2)
		{
				qPtr->oslSrcId=MOD_PRT;
				qPtr->oslDestId=MOD_PST_2;
				qPtr->oslMsgId = 0;
				OslMsgSendExtQueue(qPtr);
		}
		else
		{

		}
	}
		
		
}
#endif

#ifdef __TFLASH_MOUNT_DYNAMIC__
extern void McdSetCardDetectHandler(void);
#endif

void BAL_MmiTask(void *pData)
{
	COS_EVENT ev;
#ifdef MMI_MESSAGE_TRACE
	ilm_struct* Message = NULL;
#endif
    #if (CSW_EXTENDED_API_PMD_USB == 0)
    USB_PreClose();
    #endif // ! CSW_EXTENDED_API_PMD_USB

	SUL_ZeroMemory32( &ev, sizeof(COS_EVENT) );

	mmi_trace(g_sw_MAINENTRY, TSTXT("BAL_MmiTask: Start.\n") );     

#ifdef MMI_ON_HARDWARE_P	
#ifdef __TFLASH_MOUNT_DYNAMIC__
     McdSetCardDetectHandler();
#endif
#endif
	MMI_InitCfw(); //TBM780 hansl 2007/06/15 
#ifndef MMI_ON_WIN32

	 ML_Init();
#if (defined(MMI_2D_SENSOR_SUPPORT)&&(!defined(MMI_ON_WIN32)))	
	gpio_2d_Sensor_init();
#endif

	gpio_hall_Sensor_init();

	#ifdef __MMI_LANG_ARABIC__
	ML_SetCodePage("iso8859-6");
	#endif

	#ifdef __MMI_LANG_FRENCH__
	ML_SetCodePage("iso8859-1");
	#endif

	#ifdef __MMI_LANG_THAI__
	ML_SetCodePage("CP874");
	#endif

	#ifdef __MMI_LANG_RUSSIAN__
	ML_SetCodePage("ISO8859-5");
	#endif

	#ifdef __MMI_LANG_VIETNAMESE__
	ML_SetCodePage(ML_CP1258);
	#endif
	#endif

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	simulator_init_sms_storage();

	simulator_init_phb_storage();

#endif

	initNvramFolder();

#ifdef MMI_ON_HARDWARE_P
	for(;;) 
	{
		MMI_FreeEvent(&ev);   

		GET_COSMSG(MOD_ADP, &ev);

   //add by elisa for add tast message trace 2009-6-4     	
#ifdef MMI_MESSAGE_TRACE
       if(ev.nEventId>=EV_MMI_EV_BASE && ev.nEventId<=EV_MMI_EV_END_ ) //mmi message 
		{
            Message = (ilm_struct*)ev.nParam1;
			mmi_trace(g_sw_DEBUG, TSTXT("GET A MESSAGE, TYPE:MMI,direction:%s -> %s, ID:%d  [%s]"),
					GetModName((UINT32)(Message->src_mod_id)),GetModName((UINT32)(Message->dest_mod_id)),
					Message->msg_id, GetMEGName(Message->msg_id));
		}
	   else if IS_CFW_EVNET(ev.nEventId)					//cfw message
	   	{
			mmi_trace(g_sw_DEBUG, TSTXT("GET A EVENT, TYPE :CSW,direction:unknown->MOD_ADP, ID:%d [unknown]"),ev.nEventId);
			//Test_PrintfGetEventText(ev.nEventId );
	   	}
	   else if (ev.nEventId>=EV_DM_BASE && ev.nEventId<=EV_DM_END_ )   //DM message
	   	{
			mmi_trace(g_sw_DEBUG, TSTXT("GET A EVENT, TYPE :CSW, direction:unknown->MOD_ADP, ID:%d [unknown]"),ev.nEventId);
	   	}
	   else if (ev.nEventId>=EV_PM_BASE && ev.nEventId<=EV_PM_END_ )   //PM message
	   	{
			mmi_trace(g_sw_DEBUG, TSTXT("GET A EVENT, TYPE :CSW, direction:unknown->MOD_ADP, ID:%d [unknown]"),ev.nEventId);
	   	}
	   else 
	   	{
			mmi_trace(g_sw_DEBUG, TSTXT("GET A EVENT, TYPE :unknown, direction:unknown->MOD_ADP, ID:%d [unknown]"),ev.nEventId);
	   	}
	   
		if (ev.nEventId != 0x62 && ev.nEventId != 0x5d && ev.nEventId != 0x138b)		
			mmi_trace(g_sw_DEBUG, TSTXT("%s[0x%x]\n"), TS_GetEventName(ev.nEventId), ev.nEventId);
#endif

		ADP_DispatchMessage(&ev);
	}
#endif /*#ifdef MMI_ON_HARDWARE_P*/
}

INT32 MMC_GetLcdWidth()
{
     return LCD_WIDTH;
}

INT32 MMC_GetLcdHeight()
{
    return LCD_HEIGHT;
}

// We will use VGA sensor output as default. 
// If QVGA output is required, the Macro could be moved to target.
#ifndef MEM_ULC_3216
#define SENSOR_USE_VGA_OUTPUT
#endif
INT32 MMC_GetSensorSize()
{
#ifdef SENSOR_USE_VGA_OUTPUT
	return  1;
#else
	return  0;
#endif
}
//add by panxu 20070113
/**************************************************************
	FUNCTION NAME		:BAL_MultimediaTaskEntry
	PURPOSE		       : The task is for mp4
	INPUT PARAMETERS	: VOID *pData
	OUTPUT PARAMETERS	: void 
	RETURNS			       : 
	REMARKS			:
**************************************************************/
void BAL_MultimediaTaskEntry(void *pData)
{
	COS_EVENT ev;
	memset(&ev, 0, sizeof(ev));
	
	/* media task module init. */
#ifndef MMI_ON_WIN32
	med_init(0);
#endif

	/* 创建Media Task的消息循环 */
	while (1)
	{
		MMI_FreeEvent(&ev);
		GET_COSMSG(MOD_MED, &ev);

		if(ev.nEventId == EV_TIMER)
		{
			mmi_trace(1,"media task timer event coming! timer_id = %d", ev.nParam1);
      #ifndef MMI_ON_WIN32
			med_timer_expiry_callback((void *)ev.nParam1);
      #endif
			continue;
		}
    #ifndef MMI_ON_WIN32

#ifdef MMI_MESSAGE_TRACE

	mmi_trace(g_sw_DEBUG, TSTXT("GET A MESSAGE, TYPE:MMI,direction:%s -> %s, ID:%d  [%s]"),
		GetModName((UINT32)(((ilm_struct *)ev.nParam1)->src_mod_id)),GetModName((UINT32)(((ilm_struct *)ev.nParam1)->dest_mod_id)),
		((ilm_struct *)ev.nParam1)->msg_id, GetMEGName(((ilm_struct *)ev.nParam1)->msg_id));

#endif
		 med_main(( ilm_struct *)ev.nParam1);
    #endif
	}
}
#ifndef MMI_ON_WIN32
#include "cs_types.h"
#include "hal_usb.h"
#include "hal_sys.h"
#include "uctls_m.h"
#include "sxs_io.h"
#include "sxr_mem.h"
#include "pmd_m.h"
typedef UCTLS_BODY_MSG_T MsgBody_t;
#include "itf_msg.h"

#if (CSW_EXTENDED_API_PMD_USB == 0)
extern volatile BOOL gUsbDetected;
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */

#if (CSW_EXTENDED_API_PMD_USB == 0)
/* FS Task任务的入口函数 */
void BAL_FSTaskEntry(void *pData)
{
    COS_EVENT ev ;
    TASK_HANDLE * hTask;
    hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_FMT);
    TRACE_AUD_FUNCTION();
    SUL_ZeroMemory32(&ev, sizeof(COS_EVENT) );
    fmt_init(0);
    mmi_trace(1,"----------------------BAL_FSTaskEntry Task Begin---------------------------------\n");
    /* 创建FS Task的主循环，等待和处理消息 */
    while (1)
    {
    
	MMI_FreeEvent(&ev);
        GET_COSMSG(MOD_FMT, &ev);

        //add by elisa for add tast message trace 2009-6-4  
#ifdef MMI_MESSAGE_TRACE
        mmi_trace(g_sw_DEBUG, TSTXT("GET A MESSAGE, TYPE:MMI,direction:%s -> %s, ID:%d  [%s]"),
                  GetModName((UINT32)(((ilm_struct *)ev.nParam1)->src_mod_id)),GetModName((UINT32)(((ilm_struct *)ev.nParam1)->dest_mod_id)),
                  ((ilm_struct *)ev.nParam1)->msg_id, GetMEGName(((ilm_struct *)ev.nParam1)->msg_id));
    
#endif
#ifdef __NATIVE_JAVA__
	if (ev.nEventId >= MSG_ID_J2ME_CODE_BEGIN && ev.nEventId < MSG_ID_J2ME_CODE_CHECK_POINT)
	{
		gcj_DispatchFileMsg(ev);
		continue;
	}
#endif
        //COS_WaitEvent(g_hFsTask, &ev, COS_WAIT_FOREVER);
        fmt_main((ilm_struct *)ev.nParam1);
    }

}
#else
#include "bootup.h"


void BAL_FSTaskEntry(void *pData)
{
    COS_EVENT      ev ;
    TASK_HANDLE*   hTask;
    Msg_t*         usbMsg;
    UCTLS_STATUS_T usbStatus = UCTLS_STATUS_OPEN;

    hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_FMT);
    TRACE_AUD_FUNCTION();
    SUL_ZeroMemory32(&ev, sizeof(COS_EVENT) );
    fmt_init(0);
    mmi_trace(1,"----------------------BAL_FSTaskEntry Task Begin---------------------------------\n");
    uctls_Open(hTask->nMailBoxId, 0x0000, 0x0000, 0x0000, "USB Controller 1.0");
    pm_ForceUpdateChargerStatus();
    while (1)
    {
        usbMsg = sxr_Wait((u32 *)&ev, hTask->nMailBoxId);
        if(usbMsg)
        {
            if(usbMsg->H.Id == UCTLS_HEADER_ID)
            {
                mmi_trace(1,"---------------------------------USB mci ev.nEventId = %d , ev.nParam1 = %d-------------------------------\n",usbMsg->H.Id,usbMsg->B);
                switch (usbMsg->B)
                {
                case UCTLS_STATUS_OPEN:
                    if(usbStatus == UCTLS_STATUS_USB)
                    {
                        BatteryIndicationPopup(STR_USB_CHARGER_REMOVED);
                    }
                    else
                    {
                        BatteryIndicationPopup(STR_CHARGER_REMOVED);
                    }
                case UCTLS_STATUS_CLOSE:
                case UCTLS_STATUS_DISABLED:
                case UCTLS_STATUS_UNKNOWN:
                    if(usbStatus == UCTLS_STATUS_USB)
                    {
                        MCI_USBDisconnect();
                        mmi_trace(1,"----------------------USB PlugOut---------------------------------\n");
                        SendMessageForUSB(DEVUSB_DETECT_ACTION_PLUGOUT);
                    }
                    break;
                case UCTLS_STATUS_CHARGER:
                    BatteryIndicationPopup(STR_CHARGER_CONNECTED);
                    break;
                case UCTLS_STATUS_USB:
                    // USB is now connected
                    if(uctls_GetMode() != UCTLS_ID_3)
                    {
                        // Report to MMI only if not dummy mode
                        MCI_USBConnect();
                        mmi_trace(1,"----------------------USB Plugin---------------------------------\n");
                        BatteryIndicationPopup(STR_USB_CHARGER_CONNECTED);
                        SendMessageForUSB(DEVUSB_DETECT_ACTION_PLUGIN);
                    }
                    break;
                }
                usbStatus = usbMsg->B;
            }
            sxr_Free(usbMsg);
        }
        else
        {
            mmi_trace(1,"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");  
        //add by elisa for add tast message trace 2009-6-4  
#ifdef MMI_MESSAGE_TRACE
            mmi_trace(g_sw_DEBUG, TSTXT("GET A MESSAGE, TYPE:MMI,direction:%s -> %s, ID:%d  [%s]"),
                      GetModName((UINT32)(((ilm_struct *)ev.nParam1)->src_mod_id)),GetModName((UINT32)(((ilm_struct *)ev.nParam1)->dest_mod_id)),
                      ((ilm_struct *)ev.nParam1)->msg_id, GetMEGName(((ilm_struct *)ev.nParam1)->msg_id));
    
#endif
            //COS_WaitEvent(g_hFsTask, &ev, COS_WAIT_FOREVER);
            fmt_main((ilm_struct *)ev.nParam1);
            MMI_FreeEvent(&ev);
        }
    }
}
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */

void usb_main( ilm_struct *ilm_ptr)
{
}

void BAL_USBTaskEntry(void *pData)
{
    COS_EVENT ev ;
    TASK_HANDLE * hTask;
    Msg_t *usbMsg;
    hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_USB);
    SUL_ZeroMemory32(&ev, sizeof(COS_EVENT) );
    mmi_trace(1,"--------------BAL_USBTaskEntry Task Begin---------\n");
    MCI_USBInit(hTask->nMailBoxId);
    /* 创建FS Task的主循环，等待和处理消息 */
    while (1)
    {
        usbMsg = sxr_Wait((u32 *)&ev, hTask->nMailBoxId);
        if (usbMsg!= NULL && usbMsg->H.Id ==UCTLS_HEADER_ID)
        {
            mmi_trace(1,"---------------------------------USB mci ev.nEventId = %d , ev.nParam1 = %d-------------------------------\n",usbMsg->H.Id,usbMsg->B);
            switch (usbMsg->B)
            {
            case UCTLS_MSG_USB_DEVICE_CONNECTED:
                if(GetUsbStatus() == MMI_USBSTATUS_PLUGIN)
                {
                    sxr_Free(usbMsg);
                    continue;
                }
                gUsbDetected = 1;
                break;
            case UCTLS_MSG_USB_DEVICE_DISCONNECTED:
                gUsbDetected = 0;
                break;
            }
            sxr_Free(usbMsg);
            continue ;
        }
        else if(usbMsg==NULL)
        {
			usb_main((ilm_struct *)ev.nParam1);
			MMI_FreeEvent(&ev);
        }

    }
}

#ifdef __MMI_BT_SUPPORT__ //ACCELL BT doesn't use this MACRO?
extern void rdabt_main();

void BAL_BtTaskEntry(void *pData)
{

	rdabt_main();
}
#endif //__MMI_BT_SUPPORT__

#endif



/**************************************************************

	FUNCTION NAME		: FlightModeCheckBeforeAnimation

  	PURPOSE			: Flight mode query screen before animation

 

	OUTPUT PARAMETERS	: nil

	RETURNS			: nil

	REMARKS			: nil
**************************************************************/
extern void mmi_pwron_setaudiomode();
void FlightModeCheckBeforeAnimation(U8 flightmode)
{
	mmi_trace(1,"FlightModeCheckBeforeAnimation,flightmode is %d",flightmode);
	mmi_pwron_setaudiomode();
	 
	#ifdef __FLIGHT_MODE_SUPPORT__
	switch(flightmode)
	{
		case FLIGHTMODE_QUERY:
			TurnOnBacklight(1);
			PhnsetFlightModeQueryBackground();  
			EntryQueryFlightModeScreen();
		break;

		case FLIGHTMODE_SILENT:
			PowerOnFlightMode();
		break;

		case FLIGHTMODE_OFF:
		default:
			PowerOnNormalMode();
		break;
	}
	#else					
	mmi_pwron_entry_animation_screen(); /*  to display power on animation */
	#endif
	 
}


#ifdef MMI_ON_HARDWARE_P

/**************************************************************

	FUNCTION NAME		: mmi_frm_fetch_msg_from_extQ_to_circularQ

  	PURPOSE			: fetch the message from external queue and put
  	                 in the circular queue.
  	                 (*) Need to set my_index & mmi_ext_qid before using
  	                 this function

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: nil

	RETURNS			: 1 - the message is invalid, and discard the message
	                 0 - the message is vaild

	REMARKS			: nil
**************************************************************/
void mmi_frm_fetch_msg_from_extQ_to_circularQ(void)
{
// TODO: 为何要屏蔽以下代码?	
 
#if 0
	MYQUEUE Message;
	UINT my_index;
	U32 queue_node_number=0;
	MYQUEUE checkBaseNode = {0};
	U8 flag = 0;
	ilm_struct ilm_ptr;

	if (IsInNVRAMProcedure() == MMI_FALSE)
	{
		/*
		* If not in NVRAM access procedure,
		* receive the message from MMI queue and put in circular queue 
		*/
		msg_get_ext_queue_info(mmi_ext_qid, &queue_node_number);
		
		while ((queue_node_number > 0) && !OslIsCircularQFull())
		{
			OslReceiveMsgExtQ(mmi_ext_qid, &Message);
			OslGetMyTaskIndex( &my_index );
			OslStackSetActiveModuleID( my_index, MOD_MMI );
			OslLookUpLastCircularQMsg((void*)&checkBaseNode);

			if (mmi_frm_check_is_valid_msg(&checkBaseNode, &Message) == 1)
			{
				/* put Message in circular queue */
				ilm_ptr.src_mod_id = Message.src_mod_id;
				ilm_ptr.dest_mod_id = Message.dest_mod_id;
				ilm_ptr.msg_id = Message.msg_id;
				ilm_ptr.sap_id = Message.sap_id;
				ilm_ptr.local_para_ptr = Message.local_para_ptr;
				ilm_ptr.peer_buff_ptr = Message.peer_buff_ptr;

				flag = OslWriteCircularQ(&ilm_ptr);
				MMI_ASSERT(flag == 1);
				
				if (Message.src_mod_id != MOD_TIMER)
				{
					hold_local_para(ilm_ptr.local_para_ptr);
					hold_peer_buff(ilm_ptr.peer_buff_ptr);
				}
			}
			if (Message.src_mod_id != MOD_TIMER)
			{
				OslFreeInterTaskMsg(&Message);
			}

			msg_get_ext_queue_info(mmi_ext_qid, &queue_node_number);
		}
	}
 #endif
  
}

#endif /* MMI_ON_HARDWARE_P */


/**************************************************************

	FUNCTION NAME		: IMECheck()

  	PURPOSE				: To Check IME,

	INPUT PARAMETERS	: const U8 all_implement_IME[],U8 IME_Mode

	OUTPUT PARAMETERS	: 

	RETURNS				: MMI_TRUE : if the IME_Mode is in 
	                                                  MMI_FALSE : if the IME_Mode is not in

	REMARKS				: This is used to To Check IME,.

**************************************************************/
BOOL IMECheck(U8 IME_Mode,const U8 all_implement_IME[])
{
	int i = 0;
	for (;;)
	{
		if ((0xff) != all_implement_IME[i])
		{
			if (IME_Mode == all_implement_IME[i])
			{
				return MMI_TRUE;
			}
		}
		else
		{
			return MMI_FALSE;
		}
		i++;
		if (i > INPUT_MODE_MAX_NUM)
		{
			ASSERT(0);
		}
	}
}

/*Guojian Add Start For bluetooth tmp Ver: Jessamine  on 2008-6-27 13:56 */
void mmi_buletooth_highlight_app(void)
{
	SetLeftSoftkeyFunction(NULL,KEY_EVENT_UP);
	SetKeyHandler(NULL, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
/*Guojian Add End  For bluetooth tmp Ver: Jessamine  on 2008-6-27 13:56 */


/**************************************************************

	FUNCTION NAME		: mmi_fng_init_app()

  	PURPOSE				: To Initialize app relative fng

	INPUT PARAMETERS	: NULL

	OUTPUT PARAMETERS	: NULL

	RETURNS				: void

	REMARKS			: This is used to To Initialize fng,.

**************************************************************/
void mmi_fng_init_app()
{
	#if defined(__MMI_THEMES_APPLICATION__) //added by lin for theme init @20070620
		InitThemes();
	#endif
	
	#if defined(__MMI_AUDIO_PLAYER__)
		InitAudioPlayerApp();
	#endif /* __MMI_AUDIO_PLAYER__ */

	#if defined(__MMI_VIDEO__)
		mmi_vdoply_init_app();
	#endif /* __MMI_VIDEO__ */

    #if defined( __MMI_CAMERA__)
	mmi_trace(TRUE, "before Excute mmi_camera_init_app() is true");
        mmi_camera_init_app();
        mmi_imgview_init_app();
    #endif /* __MMI_CAMERA__ */
	
	#if defined(__MMI_GAME__)
		SetHiliteHandler(MENU3105_GAMES, mmi_game_highlight_app);
	#if defined(__MMI_NOKIA_STYLE_N800__)
		//SetHiliteHandler(MENU_FUNANDGAMES_JME_MENUID,mmi_java_game_highlight_app);
	#endif
	#endif /* __MMI_GAME__ */
	#if defined(__J2ME__) && !defined(__NATIVE_JAVA__)
	SetHiliteHandler(MENU3108_JAVA, mmi_java_highlight_menu);
	InitJAVA();
	#endif
	#if defined(__NATIVE_JAVA__)
	SetHiliteHandler(MENU_FUNANDGAMES_TENCENT_MENUID, mmi_native_java_qq_highlight_menu);
	SetHiliteHandler(MENU_FUNANDGAMES_MSN_MENUID, mmi_native_java_msn_highlight_menu);	
	#endif

    #if defined (__J2ME__)
    SetHiliteHandler(MENU_ID_FNG_SETTING_JAVA, mmi_fng_highlight_setting_java);
    #endif 
	
	#if defined(__MMI_GAME__)
		SetHiliteHandler(MENU_ID_FNG_SETTING_GAME, mmi_fng_highlight_setting_game);
	#endif /* __MMI_GAME__ */
    #if defined (__MMI_SUPPORT_JME__) 
		{
			extern void mmi_jme_highlight_app();
#if defined(__QQ_MENU_SEPERATE__)    
			extern void mmi_jme_highlight_sim(void);
			extern void mmi_jme_highlight_apn(void);
			extern void mmi_jme_highlight_proxy(void);
#endif
			mmi_trace(1,"jmetoc_showlist initial\n");
			SetHiliteHandler(MENU_FUNANDGAMES_JME_MENUID,mmi_jme_highlight_app);
#if defined(__QQ_MENU_SEPERATE__)    
			SetHiliteHandler(MENU_FUNANDGAMES_JME_SIM_MENUID,mmi_jme_highlight_sim);
			SetHiliteHandler(MENU_FUNANDGAMES_JME_APN_MENUID,mmi_jme_highlight_apn);
			SetHiliteHandler(MENU_FUNANDGAMES_JME_PROXY_MENUID,mmi_jme_highlight_proxy);
#endif
    		}    
     #endif
    
	#ifdef __MMI_SOUND_RECORDER__
	InitSoundRecorderApp();
	#endif

#ifdef __MMI_FM_RADIO__ //shizhenghui
	mmi_fmrdo_init_app();
#endif
	#ifdef __MMI_USB_SUPPORT__
	//InitMMIUsbContext();
	InitUsbModeEventHandler();
	#endif

	#if defined(__MMI_PHOTOEDITOR__)
		mmi_phoedt_init_app();
	#endif /* __MMI_PHOTOEDITOR__ */
	
#ifdef __MMI_VIDEO_RECORDER__
	mmi_vdorec_init_app();
#endif   //chenhe add for jasperII
  #if defined (__ZYX_PLATFORM_SUPPORT__) 
	{
	extern void mmi_zyx_highlight_app();
	mmi_trace(g_sw_SYSTEM,"zyx highlight\n");
    SetHiliteHandler(MENU_FUNANDGAMES_ZYX_MENUID, mmi_zyx_highlight_app);
	}    
	#endif

  #ifdef __SP__
  Init_SP();
  #endif
}


/**************************************************************

	FUNCTION NAME		: InitIMESetting()

  	PURPOSE				: To Initialize IME,

	INPUT PARAMETERS	: const sIMEDetails* IMEArray,U8* allIME,U8* englishOnlyIME,U16* PreferedIME

	OUTPUT PARAMETERS	: U8* allIME,U8* englishOnlyIME,U16* PreferedIME

	RETURNS				: void

	REMARKS				: This is used to To Initialize IME,.

**************************************************************/
void InitIMESetting(const sIMEModeDetails* IMEArray,U8* allIME,U8* englishOnlyIME,U16* PreferedIME)
{
	int i = 0,j=0,k=0;
	for (;;)
	{
		if ((0xff) != IMEArray[i].IME_Mode_ID)
		{
		       
			if (!IMECheck((U8)IMEArray[i].IME_Mode_ID,MMI_implement_input_mode_set))
			{
				//ASSERT(0);   
				/*Special IME mode ID not defined in  MMI_implement_input_mode_set, report assertion failure when power up*/
				 
			}
			allIME[i] = IMEArray[i].IME_Mode_ID;
#ifdef __MMI_PREFER_INPUT_METHOD__
			if ((NULL != PreferedIME) && (0 != IMEArray[i].IME_Type_ID))
			{
				PreferedIME[k] = IMEArray[i].IME_Type_ID;
				k++;
			}
#endif
			if (IMEArray[i].English_Only_Flag)
			{
				englishOnlyIME[j] = IMEArray[i].IME_Mode_ID;
				j++;
			}
			i++;
		}
		else
		{
			break;
		}
		if (i > INPUT_MODE_MAX_NUM)
		{
			ASSERT(0);
		}
	}
	allIME[i] = 0xff;
	PreferedIME[k]=0xffff;
	englishOnlyIME[j]=0xff;
}

/**************************************************************

	FUNCTION NAME		: InitIMEQSearch()

  	PURPOSE				: To Initialize IME,

	INPUT PARAMETERS	: const sIMEDetails* IMEQArray

	OUTPUT PARAMETERS	: U8* QuickIME

	RETURNS				: void

	REMARKS				: This is used to To Initialize IME,.

**************************************************************/
void InitIMEQSearch(const sIMEModeDetails* IMEQArray,U8* QuickIME)
{
	int i = 0,j=0;
	for (;;)
	{
		if ((0xff) != IMEQArray[i].IME_Mode_ID)
		{
			i++;
			QuickIME[j] = IMEQArray[i-1].IME_Mode_ID;
			j++;
		}
		else
		{
			break;
		}
		if (i > INPUT_MODE_MAX_NUM)
		{
			ASSERT(0);
		}
	}
	QuickIME[j] = 0xff;
}

/**************************************************************

	FUNCTION NAME		: InitIME()

  	PURPOSE				: To Initialize IME,

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: This is used to To Initialize IME,.

**************************************************************/
void InitIME(void)
{
// TODO: 为何要屏蔽以下代码?	
//#if defined(__MMI_MULTIPLE_BIN_IME__)||defined(__MMI_MULTIPLE_BIN_LANG_PACK__)
#ifdef __MMI_PREFER_INPUT_METHOD__
	InitIMESetting(gIMEModeArray,MMI_all_input_mode_set,MMI_english_input_mode_set,MMI_all_prefered_input_mode_set);
#else
	InitIMESetting(gIMEModeArray,MMI_all_input_mode_set,MMI_english_input_mode_set,NULL);
#endif
	InitIMEQSearch(gIMEQSearchModeArray,MMI_qsearch_input_mode_set);
//#endif
#ifdef __MMI_T9__
	InitInputMethod();
#elif defined __MMI_ZI__
	InitZiInputMethod();
#elif defined __MMI_KA__
	InitKonkaInputMethod();
#elif defined __MMI_ITAP__
	InitItapInputMethod();
#elif defined __MMI_GB__
	InitGbInputMethod();
#endif	
}
 

/**************************************************************

	FUNCTION NAME		: InitializeAll()

  	PURPOSE				: To Initialize framework,

	INPUT PARAMETERS	: U16 eventID

	OUTPUT PARAMETERS	: nil

	RETURNS				: static void

	REMARKS				: This is used to To Initialize framework,.

**************************************************************/
void InitFramework(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   /* 
    * Initialize events handler  
    */
   InitEvents();
   /* 
    * Initialize keyprocess 
    */
   InitProcessKeyEvent();
   /* 
    * Initialize history 
    */
   InitHistory(NULL, SubLCDIdleScreen);
}

/**************************************************************

	FUNCTION NAME		: InitializeAll()

  	PURPOSE				: To Initialize framework,

	INPUT PARAMETERS	: U16 eventID

	OUTPUT PARAMETERS	: nil

	RETURNS				: static void

	REMARKS				: This is used to To Initialize framework,.

**************************************************************/

extern void mdi_audio_init_event_hdlrs(void);
#if defined(__MMI_FM_RADIO_SCHEDULE_REC__)
extern void mmi_fmsr_init_app(void);
#endif

#ifdef __MMI_TTS_FEATURES__ 
extern int ejTTS_Init();
#endif

void InitializeAll()
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   U8 i;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	InitUnicodeSupport();  
	/* 
	* Init graphic lib 
	*/
	gdi_init();              
	/* 
	* Init media driver lib 
	*/
	// TODO: 为何要屏蔽以下代码?
	mdi_audio_init();        
	//jdi_audio_init();         

	#ifdef __MMI_WEBCAM__
	#ifdef MMI_ON_HARDWARE_P
	
		mdi_webcam_init();
	#endif
	#endif 
    
    
	#ifdef __MMI_USB_SUPPORT__
	{
   		//InitMMIUsbContext();
	}
	#endif

	#ifdef __MMI_IRDA_SUPPORT__
	{
         mmi_irda_init_cntx();   
         mmi_irda_set_recv_files_cnt(0);  
        }
	#endif

	/* 
	* Initialize the ProtocolEvents 
	*/
	// TODO: 为何要屏蔽以下代码?
	 
	/* 
	* Initialize framework 
	*/
	InitFramework();  
	InitHardwareEvents();   
	InitTime();    
	// TODO: 为何要屏蔽以下代码?
	 
	setup_UI_wrappers();  
	InitializeResourceVariables();   
	PopulateResData();    


	// TODO: 为何要屏蔽以下代码?
	//SetCurrentLanguage(0);  //tangjie del 20060629
	PopulateResGetBaseIDTable();    
	initialize_category_screens();   
	// TODO: 为何要屏蔽以下代码?
   
  PowerAndEndKeyHandler();     
	#ifdef __MOD_SMSAL__
	{
    InitMessageProtocolEventHandlers();
    mmi_frm_sms_init();
	}
	#endif
	/* 
	 * Data Account Application 
	 */
	#ifdef __GPRS_MODE__
	{
		InitDataAccountApp();  //JIASHUO FOR WAP/MMS,20090212
	}
	#endif
	/* 
	 * Call manahement needs to be initialized here because of SOS calls concept
	 */
	InitCallManagement();  // zrx open 20060620
	initialize_UI_demo();
	InitProfileApp();           // zrx open 20060620
	InitSimDetection();
	InitService();// wangbei open 20060724
	InitCPHS();
#ifndef __MMI_MULTI_SIM__
	SECSETGetFdlBdlModeReq();   
#endif
	InitNetWorkBootUpEventsCallBack();
	 
#ifdef __ADP_MULTI_SIM__
	for(i = 0; i < MMI_SIM_NUMBER;i++)
	{
		CFW_SimGetPrefOperatorListMaxNum(FREE_UTI, i);
	}
#else
	CFW_SimGetPrefOperatorListMaxNum(FREE_UTI);
#endif
	/*
	 * For engineer mode
	 */
	#ifdef __MMI_ENGINEER_MODE__ /* For saperate MMI feature by project */
	{
		InitEngineerMode();
	}
	#endif

	#ifdef __MMI_FACTORY_MODE__ /* For saperate MMI feature by project */
	{
		InitFactoryMode();
	}
	#endif


	#ifdef __MMI_FILE_MANAGER__
	{
		/*WUZC Modify Start For FMGR Ver:    on 2007-1-17 11:14 */
		mmi_fmgr_init();   
		/*WUZC Modify End  For FMGR Ver:    on 2007-1-17 11:14 */
	}
	#endif

	#ifdef __MMI_SOUND_RECORDER__
	{
		// TODO: 为何要屏蔽以下代码?
		 
	}
	#endif

#ifdef __MMI_SOUND_EFFECT__
	InitSoundEffect();  
#endif

#ifdef __MMI_BT_SUPPORT__
         mdi_audio_init_event_hdlrs(); //added by zhujianguo. 2009.6.16
#endif
	/* 
	* To initialize T9 before power on for To Do List
	*/
 
	InitIME();   
 
 	/* 
 
	 */
	// TODO: 为何要屏蔽以下代码?
	 

	InitCallLogs();  //zrx open20060620
	// TODO: 为何要屏蔽以下代码?
	//ATHandlerInit();   //dyj del 20060515
	#ifdef MMI_ON_HARDWARE_P
	{
		// TODO: 为何要屏蔽以下代码?
		 
	}
	#endif

   /*
 
    */
	mmi_phb_init_protocol();  //zrx open 20060620
	
   /* 
    * Move from InitSettingApp() for text encoding 
    */
	InitPhoneSetupCntx();   //zrx open 20060620
	InitPhoneSetupApp();   //zrx open 20060620

	
	#ifdef __MMI_BT_SUPPORT__
	{
         	mmi_bt_init();//licheng add for RDA bt project   
	}
      #endif

	#ifdef APPMEM_SUPPORT
	{
	   	InitAppMem();
	}
	#endif
	
	#if defined(__MMI_FM_RADIO_SCHEDULE_REC__)
		mmi_fmsr_init_app();   //jiashuo add for jessamine
	#endif

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_Init();
	MTPNP_PFAL_Init();
#endif
	#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
		CSD_UI_init_bmpbuf();
	#endif

#ifdef JATAAYU_SUPPORT

#ifdef __MMI_MULTI_SIM__
	MTPNP_mmi_ph_init();
#else
	mmi_ph_init () ;
#endif

#ifdef MMI_ON_HARDWARE_P
#ifdef JATAAYU_SUPPORT_WAP
	initJWap () ;
#endif
#ifdef JATAAYU_SUPPORT_MMS
	initJMms () ;
#endif
#endif
#endif
#if defined(MMI_2D_SENSOR_SUPPORT)
	csd_init_2d_Sensor_when_system_boot_up();
#endif
#ifdef MMI_ON_WIN32
  InitAllocMediaMemory();
#endif

#ifdef __MMI_TTS_FEATURES__ 
  ejTTS_Init();
#endif

}

/**************************br************************************

	FUNCTION NAME		: ProtocolEventHandler(PRT_MESSAGE*)

  	PURPOSE				: To handle protocol events.

	INPUT PARAMETERS	: U16 eventID

	OUTPUT PARAMETERS	: nil

	RETURNS				: static void

	REMARKS				: This is used to handle protocol events .

**************************************************************/

void ProtocolEventHandler(U32 eventID,void* MsgStruct,int mod_src, void *peerBuf)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mmi_trace(g_sw_SYSTEM,"\n MMItask   -> protocol event <id %d>\n",eventID);

	ExecuteCurrProtocolHandler(eventID,MsgStruct,mod_src, peerBuf);  
}

/**************************************************************

	FUNCTION NAME		: InitUnicodeSupport

  	PURPOSE				: Initalizes string functions for UCS2 support

	INPUT PARAMETERS	: nil
`
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: nil

**************************************************************/

U8  (*pfnUnicodeToEncodingScheme)( U16 Unicode ,U8 *CharacterLength , U8 *CharBuffer );

U8  (*pfnEncodingSchemeToUnicode)( PU16 pUnicode,PU8 arrOut);

S32 (*pfnUnicodeStrlen)(const S8 *arrOut);

PS8 (*pfnUnicodeStrcpy)(S8 *strDestination, const S8 *strSource );

PS8 (*pfnUnicodeStrncpy)(S8 *strDestination, const S8 *strSource , U32 size );

S32 (*pfnUnicodeStrcmp)( const S8 *string1, const S8 *string2  );

S32 (*pfnUnicodeStrncmp)( const S8 *string1, const S8 *string2, U32 size  );

PS8 (*pfnUnicodeStrcat)(S8 *strDestination, const S8 *strSource );

PS8 (*pfnUnicodeStrncat)(S8 *strDestination, const S8 *strSource, U32 size );

char *(*pfnUnicodeStrtok)( char *strToken, const char *strDelimit );

void InitUnicodeSupport(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#ifdef __UCS2_ENCODING
	{
	   pfnUnicodeToEncodingScheme = UnicodeToUCS2Encoding;
	   pfnEncodingSchemeToUnicode = UCS2EncodingToUnicode;
	   pfnUnicodeStrlen           = UCS2Strlen;
	   pfnUnicodeStrcpy           = UCS2Strcpy;
	   pfnUnicodeStrcmp           = UCS2Strcmp;
	   pfnUnicodeStrncpy          = UCS2Strncpy;
	   pfnUnicodeStrncmp          = UCS2Strncmp;
	   pfnUnicodeStrcat           = UCS2Strcat;
	   pfnUnicodeStrncat          = UCS2Strncat;
	}
	#endif
}
/**************************************************************

	FUNCTION NAME		: InitApplication

  	PURPOSE				: Initialize all application

	INPUT PARAMETERS	: void
`
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: nil

**************************************************************/
#if defined(MOTION_SENSOR_SUPPORT)
extern VOID gsensor_Init(BOOL enable_int);
#endif
S32 volume_level_UI = 0;
void InitAllApplications(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	gInitAllAppFlag=1;
	gInsertSimAppFlag = 0;
	mmi_phb_sos_list_init();
	initalize_main_menu_application();
	/* 
	* Initialize Setting application 
	*/
	InitSettingApp(); 
	InitCallCost();
	/* 
 
	*/
	mmi_fng_init_app();      


	/* 
	* Initialize phone book aplication 
	*/
	mmi_phb_init();       

	 
	#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
	mmi_init_themearchiver();
	#endif

	#ifdef __MOD_SMSAL__
	{
		InitMessagesApp();   // sms-code
		#ifdef __MMI_MESSAGES_CHAT__   
		{
			InitializeChatApp();
		}
		#endif
	}
	#endif

	InitCallHistory();
	#ifdef __MMI_STOPWATCH__
	{
		InitStopwatchApp();
	}
	#endif

	#ifdef __MMI_POC__
	{
		mmi_poc_init();
	}
	#endif

	#ifdef __MMI_EMAIL__
	{
		mmi_email_init ();
	}
	#endif // #ifdef __MMI_EMAIL__
	if (!gAlreadyInitialized) 
	{
		OrgInit();
		gAlreadyInitialized = TRUE;
		#if defined(__MMI_NITZ__)
		{
			NITZHandlerInit();
		}
		#endif
	}

	#ifdef __MMI_IRDA_SUPPORT__
	{
		mmi_irda_init_menu_hdlr();
	}
	#endif

	#if (defined(__MMI_VCARD__) || defined(__MMI_VCALENDAR__)) && defined(MMI_ON_HARDWARE_P)
	mmi_vobj_init(); /*Initial after mmiapi*/
	#endif

	#if defined(__MMI_VRSD__)
	mmi_vrsd_init();
	#endif

	#if defined(__MMI_VRSI__)
	mmi_vrsi_init();
	#endif

	#ifdef __MMI_IMPS__
	mmi_imps_init();
	#endif
	/*****************************************************************************************************/
	 
	/*****************************************************************************************************/
	ShctInit();   /*Added by Jinzh:2007-5-14*/

#ifdef __MMI_EBOOK_READER__
	InitEbookApp();
#endif /* defined (__MMI_EBOOK_READER__) */ 

#if defined(__MMI_ANALOGTV__)
	InitAnalogTV(); 
#endif
#if  defined(_KK_SUPPORT_) && defined(__KK_UI_V2__)
mmi_kk_init_app();
#endif	

#if defined(MOTION_SENSOR_SUPPORT)
	gsensor_Init(FALSE);
#endif
	volume_level_UI = GetRingVolumeLevel();
}

/**************************************************************

	FUNCTION NAME		: InitInsertSimApp

  	PURPOSE				: Initialize few applications which do not require SIM to be present

	INPUT PARAMETERS	: void
`
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: nil

**************************************************************/
void InitInsertSimApp(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
#ifdef __MMI_MULTI_SIM__
	static U8 fAbsent = 0;  
#endif /* __MMI_MULTI_SIM__ */
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

#ifdef __MMI_MULTI_SIM__
	// It means one card is absent when enter this function, so increase counter first. If not all cards is absent, just return.
	if (++fAbsent < MMI_SIM_NUMBER)
		return;
#else	
	gInsertSimAppFlag = 1;
#endif /* __MMI_MULTI_SIM__ */
	DinitSubLCDHistory();
	initalize_main_menu_application();
	InitSettingApp();//added by lin for swtich sim card @20070705
	mmi_fng_init_app();
	mmi_phb_init();
	OrgInit();

#ifdef __MMI_STOPWATCH__
	{
		InitStopwatchApp();
	}
#endif
#if defined (__MOD_SMSAL__) && defined (__DOWNLOAD__)   
	{
		InitMessagesApp();   // sms-code	
	}
#endif

#ifndef MMI_ON_WIN32
	{
		// TODO: 为何要屏蔽以下代码?
		//mmiapi_init_engine(); deleted by zhoumn for bug 10287 @2007/03/19	
	}
#endif	

	// TODO: 为何要屏蔽以下代码?
	//	AlarmInitNvRam();
	
	ShctInit();   /*qiff added 2008/10/15*/

#ifdef __MMI_EBOOK_READER__
	InitEbookApp();
#endif /* defined (__MMI_EBOOK_READER__) */ 

#if defined(__MMI_ANALOGTV__)
	InitAnalogTV();
#endif
#if  defined(_KK_SUPPORT_) && defined(__KK_UI_V2__)
  mmi_kk_init_app();
#endif	

#if defined(MOTION_SENSOR_SUPPORT)
	gsensor_Init(FALSE);
#endif

}

/**************************************************************

	FUNCTION NAME		: InitNvramData()

  	PURPOSE				: Allows the applications to read their NVRAM data

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: nil

**************************************************************/
extern void InitStopwatchNVRAM(void);//qiff add for 10708

void InitNvramData(void)
{
	#ifdef __IP_NUMBER__   
	   LoadIPNumbers();
	#endif
	/*
	 * Call Management:load black list
	 */
	#ifdef __MMI_CM_BLACK_LIST__
	   LoadBlackListNum();
	#endif
	//add by wuzc at 20061101
	// TODO: 为何要屏蔽以下代码?
	//InitDownloadNVRAM();	//to call
	InitNvramCallerId();		//to call
	InitAlarmFrameWork();		//to call
/*Jinzh Del Start For 6249,6246 Ver: TBM780  on 2007-8-17 11:28 */
	#if 0
	SetModeSpeechVolume();	//to call
	#endif
/*Jinzh Del End  For 6249,6246 Ver: TBM780  on 2007-8-17 11:28 */

#ifdef __MOD_SMSAL__
	{
		InitCBNVRAMData();	//to call
	}
#endif
	//add by wuzc at 20061101

	readWallpaperSettings();

	PhnsetInitSetTimeNDateNvram();
	PhnsetReadNvramScrSvrSettings();

	readPwrOnOffDisplaySettings();

	initnvramprofapp();
	InitSettingNVRAM();
	ReadValueCityNVRAM();
#ifdef __MMI_STOPWATCH__	
	 InitStopwatchNVRAM();//qiff add for 10708
#endif
#if defined(__MMI_SAVE_CURRENT_TIME__)
	InitSystemTimeNVRAM();
#endif
}


/**************************************************************

	FUNCTION NAME		: InitPCSimulatorBootup()

  	PURPOSE				: Automatically bootup the PC simulator only if level1

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: nil

**************************************************************/
#if defined(MMI_ON_WIN32)
void InitPCSimulatorBootup(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   /* 
    * For level 1 auto bootup
    */
	{
	MYQUEUE Message;
	mmi_eq_power_on_ind_struct *BootupReq;

	BootupReq = OslConstructDataPtr(sizeof (mmi_eq_power_on_ind_struct));
	BootupReq->poweron_mode       = (U8)0;
	BootupReq->battery_level      = (U8)0;
	BootupReq->build_label        = (U8)0;
	BootupReq->rtc_time.rtc_sec   = (U8)253;
	BootupReq->rtc_time.rtc_min   = (U8)253;
	BootupReq->rtc_time.rtc_hour  = (U8)253;
	BootupReq->rtc_time.rtc_day   = (U8)253;
	BootupReq->rtc_time.rtc_mon   = (U8)253;
	BootupReq->rtc_time.rtc_wday  = (U8)253;
	BootupReq->rtc_time.rtc_year  = (U8)253;

	Message.oslMsgId       = MMI_EQ_POWER_ON_IND;
	Message.oslDataPtr     = (oslParaType *)BootupReq;
	Message.oslPeerBuffPtr = NULL;
	Message.oslSrcId       = MOD_L4C;
	Message.oslDestId      = MOD_MMI;
	OslMsgSendExtQueue(&Message);
	}
   
	{
	MYQUEUE Message;
	mmi_smu_password_required_ind_struct *SIMInd;

	SIMInd = OslConstructDataPtr(sizeof (mmi_smu_password_required_ind_struct));
	SIMInd->type                      = 0;
	SIMInd->is_sim_change             = 0;
	SIMInd->chv_info.chv1_count       = 13;
	SIMInd->chv_info.chv2_count       = 240;
	SIMInd->chv_info.ubchv1_count     = 173;
	SIMInd->chv_info.ubchv2_count     = 186;
	SIMInd->chv_info.is_chv1_enabled  = 171;
	SIMInd->chv_info.is_chv1_verified = 171;

	Message.oslMsgId       = MSG_ID_GET_PIN_RES;
	Message.oslDataPtr     = (oslParaType *)SIMInd;
	Message.oslPeerBuffPtr = NULL;
	Message.oslSrcId       = MOD_L4C;
	Message.oslDestId      = MOD_MMI;
	OslMsgSendExtQueue(&Message);
	}

	{
	MYQUEUE Message;
	mmi_nw_attach_ind_struct *NetworkInd;

	NetworkInd = OslConstructDataPtr(sizeof (mmi_nw_attach_ind_struct));
	NetworkInd->status     = 0;
	sprintf(NetworkInd->plmn,"46688");
	NetworkInd->gsm_state  = 0;
	NetworkInd->gprs_state = 0;

	Message.oslMsgId       = PRT_NW_ATTACH_IND;	
	Message.oslDataPtr     = (oslParaType *)NetworkInd;
	Message.oslPeerBuffPtr = NULL;
	Message.oslSrcId       = MOD_L4C;
	Message.oslDestId      = MOD_MMI;
	OslMsgSendExtQueue(&Message);
	}
}
#endif

void ChargePowerOn()
{
	if (g_charbat_context.isChargerConnected == 1)
	{
		InitializeResourceVariables();
		InitializeChargingScr();
#if defined(__MMI_BT_SUPPORT__)&&defined(TGT_GALLITE_G800)
		mdi_audio_init_event_hdlrs(); 
		mmi_bt_init();
#endif
	}
	else 
		DM_DeviceSwithOff();
}

	extern s8 vm_status;
extern BOOL jmetoc_app_bg;


BOOL checkVMOn(void)
{
#ifdef __MMI_SUPPORT_JME__

	if( vm_status)
           return TRUE;
	else
		return FALSE;
#endif
#ifdef __NATIVE_JAVA__
	return gcj_IsJavaOn();
#endif
	 return FALSE;
}

BOOL checkVmBackGround(void)
{
#ifdef __MMI_SUPPORT_JME__
	#ifdef MMI_ON_HARDWARE_P

return jmetoc_app_bg;
	#else
	return 1;
#endif

#else
return FALSE;
#endif
}
