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
 * PoweronCharger.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Power On when Charger connects 
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: PoweronCharger.c

  	PURPOSE		: Power On when Charger connects 

 

 

	DATE		: 12/03/03

**************************************************************/
#define __NEWSIMULATOR 
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"

#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

#include "statusiconres.h"
#include "custdatares.h"
#include "wgui_categories.h"
#include "wgui_categories_popup.h"
#include "eventsgprot.h"
#include "queuegprot.h"
#include "taskinit.h"
#include "bootup.h"
#include "keybrd.h"
#include "protocolevents.h"
#include "allappgprot.h"
#include "mmitaskprot.h"
#include "asynceventsgprot.h"
#include "debuginitdef.h"
#include "poweronchargerprot.h"
#include "wgui_status_icons.h"
#include "commonscreens.h"
#include "timerevents.h"
#include "historygprot.h"
#include "nvramprot.h"
#include "audioinc.h"
#include "idleappprot.h"
#include "simdetectiongexdcl.h"
#include "gpioinc.h"
#include "settinggprots.h"
#include "profilegprots.h"
#include "globaldefs.h"
#include "globalscrenum.h"
#include "alarmgprot.h"   //dyj del 20060514
#include "profilegprots.h"
#include "callmanagementgprot.h"
#include "sublcdhistorygprot.h"
#include "alarmframeworkprot.h"
#include "profilegprots.h"
#include "scheduledpoweronoffgprot.h"
#include "gdi_include.h"	/* bear - include for graphic lib */
#include "lcd_if.h"
#include "dm.h"
void ChgrUpdateScrCallback( U16 curr_v );
void ChgrResetChargeVariable(void);
#if defined(__MMI_ANALOGTV__)
extern void mmi_analogtv_clean_app(void); 
#endif

extern  void mmi_vdoply_clean_app(void);
extern  void mmi_vdorec_clean_app(void);
extern  void mmi_audply_clean_app(void);
extern  void mmi_fmradio_clean_app(void);
extern  void mmi_gameapp_clean_app(void);
extern  void mmi_camapp_clean_app(void);


#ifdef __MMI_USB_SUPPORT__
#include "usbdevicegprot.h"
#endif

#ifdef __MMI_USB_SUPPORT__
#include "mci.h"
#endif

#ifdef MMI_ON_HARDWARE_P
#include "lcd_sw.h" 
#include "l4dr.h"
#include "uctls_m.h"

/* Bear remove */
//extern kal_uint16 *MainLCD_ShadowBuf_p;
//extern kal_uint16 *SubLCD_ShadowBuf_p;

#include "lcd_sw.h" 
#include "lcd_sw_inc.h"
/* [Bear] 2004/3/28 - begin */
//#include "sublcd.h"
/* [Bear] 2004/3/28 - end */
#include "gpioinc.h"
#endif
#include "composeringtoneprots.h"
#include "mmi_trace.h"
#include "usb_interface.h"
#undef __NEWSIMULATOR 

// CSD 04/18/2004 Lisen : code re-org
charbat_context_struct g_charbat_context = {
   0, 					// 	g_charbat_context.PowerOnCharger;
   0, 					// 	g_charbat_context.PowerOnChargingPeriod
   0, 					// 	g_charbat_context.ChargingAbnormal
   0, 					// 	g_charbat_context.ChargingComplete
   0, 					// 	g_charbat_context.LowBatteryFlag;	
   0, 					// 	g_charbat_context.LowBatteryTxProhibitFlag
   BATTERY_LEVEL_0, 	//    g_charbat_context.chargerLevel
   PMIC_VBAT_STATUS, //    g_charbat_context.batteryStatus
   0, 					//   	g_charbat_context.isChargerConnected
   0  					// 	g_charbat_context.LowBatteryCutOffCallCnt
};

/******************Local Variables****************************/
//Tells how  many times Battery low ind has been received. after 10 count it reset to -1
 
static U8 batteryLowCount=0;
static U8 lowBatteryAlarmThreshold = LBAT_IDLE_DURCNT_NORMAL;  

/**************************************************************
		Functions and Variables imported from outside
***************************************************************/
extern void QuitSystemOperation(void);
#ifdef __MMI_THEMES_APPLICATION__
extern void ReadThemesFromNVRAM(void);
#endif
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
extern void PmgInitExtMelodyStruct(void);
#endif

extern void InitDownloadNVRAM(void);
extern void ShutdownSystemOperation(void);

//extern U8 gPowerOnPeriod;

/**************************************************************

	FUNCTION NAME		: InitializeChargingScr(void)

  	PURPOSE				: Initialization for power on charger.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void InitializeChargingScr(void)
{
	TRACE_EF(g_sw_SYSTEM," InitializeChargingScr");
#if 1 // zrx del 20060612
	InitUnicodeSupport();

	gdi_init();

#ifdef __MMI_USB_SUPPORT__
      InitMMIUsbContext();
#endif

	InitFramework(); /* Initialize framework */
	InitHardwareEvents();
	InitTime();
	InitMMI_App();
	setup_UI_wrappers();
	PopulateResData();
	initialize_UI_demo();
	ShowChargingScr();  
	InitSettingBootup();
	
	InitProfileApp();
//	TurnOnBacklight(1); //delete by chenqiang for bug 12534(led flash when begain charging with power off)

/******************* NVRAM initialisation when there is Power on ALARM**************/
	//InitDownloadNVRAM();
	//GetSettingOptionsStatus();
	initnvramprofapp();
	InitSettingNVRAM();
#if defined(__MMI_RING_COMPOSER__) && defined(__MMI_IMELODY_SUPPORT__)	
	InitRingToneComposer();
#endif
/***********************************************************************************/

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
	PmgInitExtMelodyStruct();
#endif

	 
	InitAlarmFrameWork();

	StartLEDPatternCharging();
	g_charbat_context.PowerOnCharger=1;		/****Deepali****/
	g_charbat_context.isChargerConnected=1;
	AnimateStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);	
	UpdateStatusIcons();
	InitChargerPwrOn();
	 
	PendingAlarmReminder();
	#endif 
}
	
 
/**************************************************************


	FUNCTION NAME		: AlarmDisplayCharging

  	PURPOSE				: Handling of Charger connected on Power on Alarm screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 

**************************************************************/
void AlarmDisplayCharging(void)
{
	StartLEDPatternCharging();
	g_charbat_context.PowerOnCharger=1;		/****Deepali****/
	g_charbat_context.isChargerConnected=1;
	ShowChargingScr();

	if(g_charbat_context.ChargingComplete)
	{
		HideStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);
		ShowStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);
	}
	else
		AnimateStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);	
	UpdateStatusIcons();
	InitChargerPwrOn();
}
// CSD end
/**************************************************************


	FUNCTION NAME		: ShowChargingScr

  	PURPOSE				: Show charging screen when phone bootup bcos of charger.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 

**************************************************************/
void ShowChargingScr(void)
{
	EntryNewScreen(SCR_CHARGER_POWER_ON, ExitChargingScr, NULL, NULL);
	entry_full_screen();
	TRACE_EF(g_sw_SYSTEM,"ShowChargingScr");
	DinitHistory();
    ShowCategory9Screen(0, IMG_CHARGER_POWER_ON, NULL);
	//ShowSubLCDScreen(ShowSubLCDChargingScr);
#ifndef __MMI_SUBLCD_CHARGING_HIDE_STATUS_BAR__
	ShowStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);
	UpdateStatusIcons();
#endif
	 
#if defined(TGT_USING_POWER_KEY)
     SetKeyHandler(ChargingPwnOnThanPwnOn,KEY_POWER,KEY_LONG_PRESS);
#else	
     SetKeyHandler(ChargingPwnOnThanPwnOn,KEY_END,KEY_LONG_PRESS);
#endif
	g_charbat_context.PowerOnChargingPeriod=1;

    {
        ChgrResetChargeVariable();
        ChgrUpdateScrCallback(0);
    }

}

/**************************************************************


	FUNCTION NAME		: ExitChargingScr

  	PURPOSE				: Exit power on charging screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 

**************************************************************/
void ExitChargingScr(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	s16 nHistory = 0;
	Scr.scrnID = SCR_CHARGER_POWER_ON;
	Scr.entryFuncPtr = ShowChargingScr;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
	AddHistory(Scr);
	leave_full_screen();
	g_charbat_context.PowerOnChargingPeriod=0;
}

/**************************************************************


	FUNCTION NAME		: ShowSubLCDChargingScr

  	PURPOSE				: Show power on charging screen on SUBLCD

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 

**************************************************************/
void ShowSubLCDChargingScr(void)
{
#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
	if (!IsClamClose())
	{
		ExecSubLCDCurrExitHandler();
		ShowCategory310Screen(get_string(STR_GLOBAL_LOGO));
		SetSubLCDExitHandler(ExitSubLCDChargingScr);
	}
	else
	{
		ExecSubLCDCurrExitHandler();
		ShowCategory304Screen(IMG_SUBLCD_CHARGER_POWER_ON,0);
		SetSubLCDExitHandler(ExitSubLCDChargingScr);
	}
#else
	ExecSubLCDCurrExitHandler();
	ShowCategory304Screen(IMG_SUBLCD_CHARGER_POWER_ON,0);
	SetSubLCDExitHandler(ExitSubLCDChargingScr);
#endif

}

/**************************************************************


	FUNCTION NAME		: ExitSubLCDChargingScr

  	PURPOSE				: exit power on charging screen from sublcd.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 

**************************************************************/
void ExitSubLCDChargingScr(void)
{
	SubLCDHistoryNode SubLCDHistory;
	SubLCDHistory.entryFuncPtr=ShowSubLCDChargingScr;
	AddSubLCDHistory(&SubLCDHistory);
}

/**************************************************************

	FUNCTION NAME		: InitChargerPwrOn(void)

  	PURPOSE				: Initialise protocol handlers for power on charger.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void InitChargerPwrOn()
{
	SetProtocolEventHandler(BatteryStatusRsp,PRT_BATTERY_STATUS_IND);
#ifdef MMI_ON_HARDWARE_P
	SetProtocolEventHandler(PowerOffRsp,MSG_ID_MMI_EQ_POWER_OFF_RSP);
#endif
      
	//SetKeyHandler(ChargingPwnOnThanPwnOn,KEY_END,KEY_LONG_PRESS);
}

// MMI should detect keypad power on itself when charging
/**************************************************************

	FUNCTION NAME		: ChargingPwnOnThanPwnOn(void)

  	PURPOSE				: Fn to be called to power on the phone for long press of End
						  key during power n charger.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ChargingPwnOnThanPwnOn()
{
	MYQUEUE Message;
	mmi_eq_power_on_req_struct *myMsgPtr;

	#if (defined(__MMI_SUBLCD__) && defined(MMI_ON_HARDWARE_P) )
		lcd_power_on(SUB_LCD, 0);
	#endif	
	
	myMsgPtr = (mmi_eq_power_on_req_struct*) OslConstructDataPtr(sizeof(mmi_eq_power_on_req_struct));
	myMsgPtr->fun = 1;
	myMsgPtr->rst = 0;
    	TRACE_EF(g_sw_SYSTEM," ChargingPwnOnThanPwnOn CHARGER POWERKEY_ON!!!!");
	Message.oslSrcId       = MOD_MMI;
	Message.oslDestId      = MOD_L4C;
	Message.oslMsgId       = MSG_ID_MMI_EQ_POWER_ON_REQ;
    Message.oslDataPtr     = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr = NULL;
	OslMsgSendExtQueue(&Message);
	g_charbat_context.PowerOnCharger=0;		/****Deepali****/
}


#include "mmi_msg_struct.h"
#include "protocolevents.h"
void SendMessageForUSB(UINT32 action)
{
	ilm_struct ptr_HandsetKey;
	mmi_eq_usbdetect_ind_struct *message=(mmi_eq_usbdetect_ind_struct *)OslMalloc(sizeof(mmi_eq_usbdetect_ind_struct));
	ASSERT(NULL != message);
	memset(message,0,sizeof(mmi_eq_usbdetect_ind_struct));
	
	message->action = action;

	ptr_HandsetKey.src_mod_id     = MOD_L4C;
	ptr_HandsetKey.dest_mod_id    = MOD_MMI;
	ptr_HandsetKey.msg_id         = PRT_EQ_USB_DETECT_IND;
	ptr_HandsetKey.local_para_ptr = (local_para_struct*)message;
	OslMsgSendExtQueue(&ptr_HandsetKey);

	//UsbDetectIndHdlr(message);
}



/**************************************************************

	FUNCTION NAME		: BatteryStatusRsp(void)

  	PURPOSE				: Call back function for battery status indication.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
//chenhe for jessamine charger detect to avoid pulse
//#define PMIC_CHARGER_DETECT_DELAY


extern BOOL g_UsbChargerIn;
extern BOOL g_BeforeEntryIdleScr;
#if (CSW_EXTENDED_API_PMD_USB == 0)
extern BOOL g_NeedReDetectUsb;
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */
extern BOOL do_usbplugout_later;
extern BOOL g_UsbChargerPowerOn;
UINT8 isChargerIn(void) //chenhe add
{
	return g_charbat_context.isChargerConnected;
}

void BatteryStatusRsp(void* info)
{
#if (CSW_EXTENDED_API_PMD_USB == 0)
	MYQUEUE Message;
	USB_DETECTED_STATUS usbDetectedResult;
	mmi_eq_battery_status_ind_struct *p=(mmi_eq_battery_status_ind_struct*) info;
	g_charbat_context.batteryStatus=p->battery_status;
	g_charbat_context.chargerLevel=p->battery_voltage;
	mmi_trace(g_sw_PM,"BatteryStatusRsp  g_charbat_context.batteryStatus %d",g_charbat_context.batteryStatus);
	switch(p->battery_status)
	{
		case PMIC_CHARGER_IN://charger connected

#ifdef __MMI_USB_SUPPORT__
			usbDetectedResult = MCI_USBDetect();
			mmi_trace(g_sw_DEBUG, "MMI_DEBUG: File: %s, Line: %d, entry %s function,usbDetectedResult=(%d)", __FILE__, __LINE__, __FUNCTION__,usbDetectedResult);
			if(USB_DETECTED_OK != usbDetectedResult) //LICHENG modify for bug:10679
			{
#endif


                		mmi_trace(g_sw_PM,"MCI_USBDetect is %d",usbDetectedResult);
				g_charbat_context.isChargerConnected=1;
			       g_charbat_context.ChargingComplete = 0;  
			       TRACE_EF(g_sw_PM,"BatteryStatusRsp PMIC_CHARGER_IN");
				TurnOnBacklight(1); 
				if(!g_charbat_context.PowerOnCharger && !IdleIsPowerOnPeriod())//to avoid popup on power on and charging animation screens
				{
					if (USB_DETECTED_BUSY == usbDetectedResult)
					{
						g_NeedReDetectUsb = 1;
					}
					else
						BatteryIndicationPopup(STR_CHARGER_CONNECTED);
					IdleScreenChargerConnected(KAL_TRUE);  
				}
				if(  g_charbat_context.LowBatteryFlag )  
				{
				    g_charbat_context.LowBatteryFlag = 0;
	  			    StopLEDPatternBatteryWarning();  
				 }

#ifdef __MMI_USB_SUPPORT__
			}
			else
			{
			    mmi_trace(g_sw_PM,"charger in for usb");
				g_charbat_context.isChargerConnected=1;
		       g_charbat_context.ChargingComplete = 0;  
		       TRACE_EF(g_sw_PM," BatteryStatusRsp PMIC_CHARGER_IN");
				TurnOnBacklight(1); 
				if(!g_charbat_context.PowerOnCharger && !IdleIsPowerOnPeriod())//to avoid popup on power on and charging animation screens
				{
					IdleScreenChargerConnected(KAL_TRUE);  
				}
				if(  g_charbat_context.LowBatteryFlag )  
				{
				    g_charbat_context.LowBatteryFlag = 0;
	  			    StopLEDPatternBatteryWarning();  
				 }
				mmi_trace(g_sw_PM,"g_BeforeEntryIdleScr = %d\n",g_BeforeEntryIdleScr);
				g_UsbChargerIn = 1;
				if (USB_DETECTED_OK==usbDetectedResult)
				{
					if (g_BeforeEntryIdleScr && !(g_UsbChargerPowerOn))
						g_NeedReDetectUsb = 1;
					else
						SendMessageForUSB(DEVUSB_DETECT_ACTION_PLUGIN);
				}
				else if (USB_DETECTED_BUSY == usbDetectedResult)
				{
					g_NeedReDetectUsb = 1;
				}
			}
#endif //__MMI_USB_SUPPORT__
// Reset low battery indication on charge-in
			batteryLowCount = 0;//reset low battery ind count
			g_charbat_context.LowBatteryFlag = 0;  
			g_charbat_context.LowBatteryTxProhibitFlag = 0;

			break;
		case PMIC_CHARGER_OUT://charger out

#ifdef __MMI_USB_SUPPORT__		
			if(g_charbat_context.isChargerConnected && !g_UsbChargerIn)
			{
#endif


			g_charbat_context.ChargingComplete = 0;  
			g_charbat_context.isChargerConnected=0;
			TRACE_EF(g_sw_PM,"BatteryStatusRsp PMIC_CHARGER_OUT g_charbat_context.PowerOnCharger=%d,AlmIsRTCPwron()=%d",g_charbat_context.PowerOnCharger,AlmIsRTCPwron());
			StopLEDPatternBatteryWarning();
			if(g_charbat_context.PowerOnCharger && (AlmIsRTCPwron() == 0))
			{
				Message.oslSrcId = MOD_MMI;
				Message.oslDestId = MOD_L4C;
				Message.oslMsgId = MSG_ID_MMI_EQ_POWER_OFF_REQ;
       			Message.oslDataPtr = NULL;
				Message.oslPeerBuffPtr= NULL;
#ifndef MMI_ON_WIN32				
        DM_SetPowerDown_Cause(1);
#endif
				OslMsgSendExtQueue(&Message);
			}
			else
			{
			    if(!IdleIsPowerOnPeriod()) 
				{
					TurnOnBacklight(1);
					if (g_NeedReDetectUsb)
						g_NeedReDetectUsb = 0;
					else 
						BatteryIndicationPopup(STR_CHARGER_REMOVED);
					//IdleScreenChargerDisconnected();
					 
 					StopLEDPatternCharging();
  			     }
			    
   			     IdleScreenChargerDisconnected();

				if( g_charbat_context.ChargingAbnormal )
				{
					g_charbat_context.ChargingAbnormal=0;
  			    }
                //CSD End.
		
			}
			//Lisen 0710
			g_charbat_context.PowerOnCharger = 0;
#ifdef __MMI_USB_SUPPORT__
			}
			else if (g_UsbChargerIn)
			{
				mmi_trace(g_sw_PM,"chenhe ,charger out for usb");
				if (!g_NeedReDetectUsb && !(g_UsbChargerPowerOn &&g_BeforeEntryIdleScr))
				{
					SendMessageForUSB(DEVUSB_DETECT_ACTION_PLUGOUT);
				}
				else if ((g_UsbChargerPowerOn &&g_BeforeEntryIdleScr))
				{
					do_usbplugout_later = 1;	
				}
				else
					g_NeedReDetectUsb = 0;
/*
				g_charbat_context.PowerOnCharger = 0;
				g_charbat_context.isChargerConnected=0;
				HideStatusIcon(STATUS_ICON_BATTERY_STRENGTH);
				HideStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);
				ShowStatusIcon(STATUS_ICON_BATTERY_STRENGTH);
				ShowStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);
				UpdateStatusIcons();
				BatteryStatusIndication(g_charbat_context.chargerLevel);
*/
			g_charbat_context.ChargingComplete = 0;  
			g_charbat_context.isChargerConnected=0;
			TRACE_EF(g_sw_PM,"BatteryStatusRsp PMIC_CHARGER_OUT g_charbat_context.PowerOnCharger=%d,AlmIsRTCPwron()=%d",g_charbat_context.PowerOnCharger,AlmIsRTCPwron());
			StopLEDPatternBatteryWarning();
			if(g_charbat_context.PowerOnCharger && (AlmIsRTCPwron() == 0))
			{
				Message.oslSrcId = MOD_MMI;
				Message.oslDestId = MOD_L4C;
				Message.oslMsgId = MSG_ID_MMI_EQ_POWER_OFF_REQ;
       			Message.oslDataPtr = NULL;
				Message.oslPeerBuffPtr= NULL;
#ifndef MMI_ON_WIN32				
        DM_SetPowerDown_Cause(1);
#endif
				OslMsgSendExtQueue(&Message);
			}
			else
			{
			    if(!IdleIsPowerOnPeriod()) 
				{
					TurnOnBacklight(1);
 					StopLEDPatternCharging();
  			     }
			    
   			     IdleScreenChargerDisconnected();

				if( g_charbat_context.ChargingAbnormal )
				{
					g_charbat_context.ChargingAbnormal=0;
  			    }
                //CSD End.
		
			}
			//Lisen 0710
			g_charbat_context.PowerOnCharger = 0;

			g_UsbChargerIn = 0;
			}
#endif
			break;
#else
    MYQUEUE Message;
    UCTLS_STATUS_T usbStatus;
    mmi_eq_battery_status_ind_struct *p=(mmi_eq_battery_status_ind_struct*) info;
    g_charbat_context.batteryStatus = p->battery_status;
    g_charbat_context.chargerLevel  = p->battery_voltage;
    mmi_trace(g_sw_PM,"BatteryStatusRsp  g_charbat_context.batteryStatus %d",g_charbat_context.batteryStatus);
    switch(p->battery_status)
    {
    case PMIC_CHARGER_IN://charger connected
        g_charbat_context.isChargerConnected = TRUE;
        g_charbat_context.ChargingComplete   = FALSE;  
        TurnOnBacklight(TRUE);

        TRACE_EF(g_sw_PM,"BatteryStatusRsp PMIC_CHARGER_IN");
#ifdef __MMI_USB_SUPPORT__
        usbStatus = uctls_GetStatus();
        if(usbStatus != UCTLS_STATUS_USB &&
           usbStatus != UCTLS_STATUS_DISABLED) //LICHENG modify for bug:10679
        {
#endif
            if(!g_charbat_context.PowerOnCharger && !IdleIsPowerOnPeriod())//to avoid popup on power on and charging animation screens
            {
                IdleScreenChargerConnected(KAL_TRUE);  
            }
#ifdef __MMI_USB_SUPPORT__
        }
        else
        {
            if(!g_charbat_context.PowerOnCharger && !IdleIsPowerOnPeriod())//to avoid popup on power on and charging animation screens
            {
                IdleScreenChargerConnected(KAL_TRUE);  
            }
            mmi_trace(g_sw_PM,"g_BeforeEntryIdleScr = %d\n",g_BeforeEntryIdleScr);
            g_UsbChargerIn = 1;
        }
#endif
        if(g_charbat_context.LowBatteryFlag)  
        {
            g_charbat_context.LowBatteryFlag = FALSE;
            StopLEDPatternBatteryWarning();  
        }
        break;
    case PMIC_CHARGER_OUT://charger out

#ifdef __MMI_USB_SUPPORT__
        if(g_charbat_context.isChargerConnected && !g_UsbChargerIn)
        {
#endif
            g_charbat_context.ChargingComplete   = FALSE;  
            g_charbat_context.isChargerConnected = FALSE;
            TRACE_EF(g_sw_PM,"BatteryStatusRsp PMIC_CHARGER_OUT g_charbat_context.PowerOnCharger=%d,AlmIsRTCPwron()=%d",g_charbat_context.PowerOnCharger,AlmIsRTCPwron());
            StopLEDPatternBatteryWarning();
            if(g_charbat_context.PowerOnCharger && (AlmIsRTCPwron() == 0))
            {
                Message.oslSrcId       = MOD_MMI;
                Message.oslDestId      = MOD_L4C;
                Message.oslMsgId       = MSG_ID_MMI_EQ_POWER_OFF_REQ;
                Message.oslDataPtr     = NULL;
                Message.oslPeerBuffPtr = NULL;
                OslMsgSendExtQueue(&Message);
            }
            else
            {
                if(!IdleIsPowerOnPeriod())
                {
                    TurnOnBacklight(TRUE);
                    //IdleScreenChargerDisconnected();
                     
                    StopLEDPatternCharging();
                 }

                 IdleScreenChargerDisconnected();

                if(g_charbat_context.ChargingAbnormal)
                {
                    g_charbat_context.ChargingAbnormal = 0;
                }
                //CSD End.
            }
            //Lisen 0710
            g_charbat_context.PowerOnCharger = 0;
#ifdef __MMI_USB_SUPPORT__
        }
        else if(g_UsbChargerIn)
        {
            if ((g_UsbChargerPowerOn &&g_BeforeEntryIdleScr))
            {
                do_usbplugout_later = 1;
            }
            g_charbat_context.ChargingComplete   = FALSE;  
            g_charbat_context.isChargerConnected = FALSE;
            TRACE_EF(g_sw_PM,"BatteryStatusRsp PMIC_CHARGER_OUT g_charbat_context.PowerOnCharger=%d,AlmIsRTCPwron()=%d",g_charbat_context.PowerOnCharger,AlmIsRTCPwron());
            StopLEDPatternBatteryWarning();
            if(g_charbat_context.PowerOnCharger && (AlmIsRTCPwron() == 0))
            {
                Message.oslSrcId       = MOD_MMI;
                Message.oslDestId      = MOD_L4C;
                Message.oslMsgId       = MSG_ID_MMI_EQ_POWER_OFF_REQ;
                Message.oslDataPtr     = NULL;
                Message.oslPeerBuffPtr = NULL;
                OslMsgSendExtQueue(&Message);
            }
            else
            {
                if(!IdleIsPowerOnPeriod())
                {
                    TurnOnBacklight(TRUE);
                    //BatteryIndicationPopup(STR_CHARGER_REMOVED);
                    //IdleScreenChargerDisconnected();
                     
                    StopLEDPatternCharging();
                 }
                IdleScreenChargerDisconnected();

                if( g_charbat_context.ChargingAbnormal )
                {
                    g_charbat_context.ChargingAbnormal = 0;
                }
                //CSD End.
            }
            //Lisen 0710
            g_charbat_context.PowerOnCharger = 0;

            g_UsbChargerIn = 0;
            }
#endif
			break;
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */

		case PMIC_CHARGE_COMPLETE://battery full charged.
			TRACE_EF(g_sw_PM,"BatteryStatusRsp PMIC_CHARGE_COMPLETE");
			if(g_charbat_context.PowerOnCharger)
			{
                 
				if( g_charbat_context.ChargingComplete == FALSE && (AlmIsRTCPwron() == 0))  
				{
					g_charbat_context.ChargingComplete = TRUE;
					if(!IdleIsPowerOnPeriod()) 
					{
						ShowChargingCompleteScr();
					}	
					StartLEDPatternFullCharging();
					HideStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);
					ShowStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);
					ChangeStatusIconLevel(STATUS_ICON_BATTERY_STRENGTH,100); //CSD 1107, Charging complete battery status icon full
					ChangeStatusIconLevel(STATUS_ICON_SUBLCD_BATTERY_STRENGTH,100);
					UpdateStatusIcons();
				}
                // CSD End
			}
			/* Message for Power off Device;Deepali*/
			else
			{
				g_charbat_context.ChargingComplete = TRUE;
#ifdef  __MMI_LED_POWERON_CHARGEFULL__	      
				StartLEDPatternFullCharging();
#endif			
#ifdef __MMI_USB_SUPPORT__		
			//if(!MCI_USBDetect())			
#endif
			{
				IdleScreenBatteryFullCharged();
				if(!IdleIsPowerOnPeriod()) 
				{
					DisplayPopup( (U8 *)GetString(STR_BATTERY_FULL_CHARGED), \
					IMG_GLOBAL_ACTIVATED, 0, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);//added by zhoumn for full charged @2007/07/28
				}	
			}
			}
			break;
		case PMIC_VBAT_STATUS:
		     BatteryStatusLowBatteryHandler(p->battery_voltage);
			 BatteryStatusIndication(p->battery_voltage);
		    break;
#ifdef __MMI_USB_SUPPORT__
		case PMIC_USB_CHARGER_IN:
 		    g_charbat_context.isChargerConnected = TRUE;
		if(!IdleIsPowerOnPeriod())
		    IdleScreenChargerConnected(KAL_TRUE);
		    break;

		case PMIC_USB_CHARGER_OUT:

			if(g_charbat_context.PowerOnCharger && (AlmIsRTCPwron() == 0))
			{
                Message.oslSrcId       = MOD_MMI;
                Message.oslDestId      = MOD_L4C;
                Message.oslMsgId       = MSG_ID_MMI_EQ_POWER_OFF_REQ;
                Message.oslDataPtr     = NULL;
                Message.oslPeerBuffPtr = NULL;
                OslMsgSendExtQueue(&Message);
				break;
			}

 		    g_charbat_context.isChargerConnected = FALSE;
		    TurnOnBacklight(TRUE);
		    if(!IdleIsPowerOnPeriod()) 
 		    {
#if (CSW_EXTENDED_API_PMD_USB == 0)
		       BatteryIndicationPopup(STR_USB_CHARGER_REMOVED);
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */
		      IdleScreenChargerDisconnected();
		    }
		    g_charbat_context.PowerOnCharger = FALSE;
		    break;
#endif

        case PMIC_INVALID_BATTERY:     ChargingAbnormalHandler(STR_INVALID_BATTERY);      break;
        case PMIC_OVERBATTEMP:         ChargingAbnormalHandler(STR_OVER_TEMP_BATTERY);    break;
        case PMIC_OVERVOLPROTECT:      ChargingAbnormalHandler(STR_OVER_VOLTAGE_BATTERY); break;
        case PMIC_OVERCHARGECURRENT:   ChargingAbnormalHandler(STR_OVER_CURRENT_BATTERY); break;
        case PMIC_LOWBATTEMP:          ChargingAbnormalHandler(STR_LOW_TEMP_BATTERY);     break;
        case PMIC_CHARGING_TIMEOUT:    ChargingAbnormalHandler(STR_CHARGING_TIMEOUT);     break;
        case PMIC_INVALID_CHARGER:     ChargingAbnormalHandler(STR_INVALID_CHARGER);      break;
        case PMIC_LOWCHARGECURRENT:    ChargingAbnormalHandler(STR_CHARGE_BAD_CONTACT);   break;
        case PMIC_CHARGE_BAD_CONTACT:  ChargingAbnormalHandler(STR_CHARGE_BAD_CONTACT);   break;
        case PMIC_BATTERY_BAD_CONTACT: ChargingAbnormalHandler(STR_BATTERY_BAD_CONTACT);  break;

	     default:
   	          break;
		    
	}

    
   /* Shall reset this flag in the end of msg handler func */
   SetInterruptPopupDisplay(POPUP_ALL_ON);
}


/**************************************************************


	FUNCTION NAME		: LowBatteryCommonAction

  	PURPOSE				: Handles the common action for low battery

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 


**************************************************************/
 
void LowBatteryCommonAction(void)
{
    if(IdleIsPowerOnPeriod())
    {
    		/* Reset low battery alarm counter,  low battery will alert right after finish power on procedure  */
		RESET_LOW_BATTERY_ALARM_CNT();
            if(!IS_IDLE_ARRIVED)
               DisplayPopup( (U8 *)GetString(STR_LOW_BATTERY), \
                 IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, 0);
    }
    else 
    {
	    if( !g_charbat_context.isChargerConnected &&  batteryLowCount >= lowBatteryAlarmThreshold )
	{
        batteryLowCount=0;
		BatteryIndicationPopup(STR_LOW_BATTERY);
		StartLEDPatternBatteryLow();
		ChgrPlayLowBatTone();
		BatteryStatusIndication(BATTERY_LOW_WARNING);
		PRINT_INFORMATION(("@@@@@ PMIC_LOW_BATTERY!!!! @@@@@"));
//process low battery applications action                
#if defined(__MMI_ANALOGTV__)                            
                        mmi_analogtv_clean_app();
#endif
#if defined(__MMI_VIDEO_PLAYER__)
                        mmi_vdoply_clean_app();
#endif
#if defined(__MMI_VIDEO_RECORDER__)
                        mmi_vdorec_clean_app();
#endif
#if defined(__MMI_AUDIO_PLAYER__)
                        mmi_audply_clean_app();
#endif
#ifdef	__MMI_FM_RADIO__
                        mmi_fmradio_clean_app();
#endif
 #ifdef __MMI_GAME__                       
                        mmi_gameapp_clean_app();
 #endif
                        mmi_camapp_clean_app();
	}

	mmi_trace(g_sw_ADP_NW, TSTXT("LowBat IND: CallCount is [%d], LowBatTxFlag is [%d], LowBatteryCutOffCallCnt is [%d]\n"), 
        GetTotalCallCount(),g_charbat_context.LowBatteryTxProhibitFlag, g_charbat_context.LowBatteryCutOffCallCnt);

 	if( GetTotalCallCount() >  0 && g_charbat_context.LowBatteryTxProhibitFlag )
	{
	       g_charbat_context.LowBatteryCutOffCallCnt++;
		if( g_charbat_context.LowBatteryCutOffCallCnt >= LBAT_CALLDROP_CNT  )
		{
		    g_charbat_context.LowBatteryCutOffCallCnt=0;
		    DeInitCM();
		}
	}
	else
 	    {
	   g_charbat_context.LowBatteryCutOffCallCnt=0;
	    }
    }
 
	g_charbat_context.LowBatteryFlag = 1;  
	batteryLowCount++;

}

/**************************************************************


	FUNCTION NAME		: BatteryStatusLowBatteryHandler

  	PURPOSE				: This function handle low battery case

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 


**************************************************************/
void BatteryStatusLowBatteryHandler(U8 battery_voltage)
{
	  mmi_trace(g_sw_PM, "=LOWBAT=[Thre %d] [Cnt %d] [LBflag %d] [Txflag %d]", lowBatteryAlarmThreshold,  batteryLowCount, g_charbat_context.LowBatteryFlag,  g_charbat_context.LowBatteryTxProhibitFlag );
	  mmi_trace(g_sw_PM, "battery_voltage =%d", battery_voltage);
      switch(battery_voltage)
      {
      		/*  1. After power on animation, will check the battery level again, if too low, will shutdown as well. */
		/*  2. When charger connected, shall ignore low battery indication  */
		case BATTERY_LOW_POWEROFF://shutdown bcos of low battery
		   if( !IdleIsPowerOnPeriod() && !g_charbat_context.isChargerConnected )
		     {
					PRINT_INFORMATION(("@@@@@ PMIC_LOW_BATTERY_POWER_OFF!!!! @@@@@"));
					TurnOnBacklight(1);
					ShowLowPowerShutdownScr();
		     }
		   break;
		case BATTERY_LOW_TX_PROHIBIT:
			if( g_charbat_context.LowBatteryTxProhibitFlag == 0 ) 
			{
	  		    /* Battery level changer, Change alert Threshold */
			    g_charbat_context.LowBatteryTxProhibitFlag = 1; // BATTERY_LOW_TX_PROHIBIT is a kind of low battery indication
			    if( GetTotalCallCount() == 0 )
			    {
					SET_LOW_BATTERY_ALARM_THRESHOLD( LBAT_IDLE_DURCNT_PROHIBIT );
			        RESET_LOW_BATTERY_ALARM_CNT();
			    }
			    else
			    {
			      SET_LOW_BATTERY_ALARM_THRESHOLD( LBAT_TALK_DURCNT_PROHIBIT );
			      RESET_LOW_BATTERY_ALARM_CNT();
			    }
  	        }
			LowBatteryCommonAction();
			break;
			
		case BATTERY_LOW_WARNING:
	     
		    if( g_charbat_context.LowBatteryFlag == 0 || g_charbat_context.LowBatteryTxProhibitFlag == 1)
			{
		     	    /* Battery level changer, Change alert Threshold */

		     	    /* Battery level coming up from BATTERY_LOW_TX_PROHIBIT, reset flag */
		              g_charbat_context.LowBatteryTxProhibitFlag = 0;
			    
				if( GetTotalCallCount() == 0 )
				{
  					SET_LOW_BATTERY_ALARM_THRESHOLD( LBAT_IDLE_DURCNT_NORMAL );
  		             RESET_LOW_BATTERY_ALARM_CNT();
				}
			    else
			    {
					SET_LOW_BATTERY_ALARM_THRESHOLD( LBAT_TALK_DURCNT_NORMAL );
			       RESET_LOW_BATTERY_ALARM_CNT();
			    }
			}

			 LowBatteryCommonAction();
		   break;

		case BATTERY_LEVEL_0:
		case BATTERY_LEVEL_1:
		case BATTERY_LEVEL_2:
		case BATTERY_LEVEL_3:
		case BATTERY_LEVEL_4:
		case BATTERY_LEVEL_5:
		default:
			batteryLowCount = 0;//reset low battery ind count
			g_charbat_context.LowBatteryFlag = 0;  
			g_charbat_context.LowBatteryTxProhibitFlag = 0;
		   break;
      }
}
//End of modify 1129
/**************************************************************


	FUNCTION NAME		: BatteryStatusIndication

  	PURPOSE				: This function shows the level on status icons of mainlcd nad sublcd
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 


**************************************************************/
 
void BatteryStatusIndication(U8 battery_voltage)
{
	switch(battery_voltage)
	{
		case BATTERY_LOW_POWEROFF:
		case BATTERY_LOW_TX_PROHIBIT:
		case BATTERY_LOW_WARNING:
			PRINT_INFORMATION(("BatteryStatusIndication Warning"));
			ChangeStatusIconLevel(STATUS_ICON_BATTERY_STRENGTH,0);
#ifdef __MMI_SUBLCD__
			ChangeStatusIconLevel(STATUS_ICON_SUBLCD_BATTERY_STRENGTH,0);
#endif
			break;		
		case BATTERY_LEVEL_0:
			PRINT_INFORMATION(("BatteryStatusIndication voltage 0"));
			ChangeStatusIconLevel(STATUS_ICON_BATTERY_STRENGTH,0);
#ifdef __MMI_SUBLCD__
			ChangeStatusIconLevel(STATUS_ICON_SUBLCD_BATTERY_STRENGTH,0);
#endif
			break;		
		case BATTERY_LEVEL_1:
			PRINT_INFORMATION(("BatteryStatusIndication voltage 1"));
			ChangeStatusIconLevel(STATUS_ICON_BATTERY_STRENGTH,20);
#ifdef __MMI_SUBLCD__
			ChangeStatusIconLevel(STATUS_ICON_SUBLCD_BATTERY_STRENGTH,20);
#endif
			break;		
		case BATTERY_LEVEL_2:
			PRINT_INFORMATION(("BatteryStatusIndication voltage 2"));
			ChangeStatusIconLevel(STATUS_ICON_BATTERY_STRENGTH,40);
#ifdef __MMI_SUBLCD__
			ChangeStatusIconLevel(STATUS_ICON_SUBLCD_BATTERY_STRENGTH,40);
#endif
			break;
		case BATTERY_LEVEL_3:
			PRINT_INFORMATION(("BatteryStatusIndication voltage 3"));
			ChangeStatusIconLevel(STATUS_ICON_BATTERY_STRENGTH,60);
#ifdef __MMI_SUBLCD__
			ChangeStatusIconLevel(STATUS_ICON_SUBLCD_BATTERY_STRENGTH,60);
#endif
			break;
		case BATTERY_LEVEL_4:
			PRINT_INFORMATION(("BatteryStatusIndication voltage 4"));
			ChangeStatusIconLevel(STATUS_ICON_BATTERY_STRENGTH,80);
#ifdef __MMI_SUBLCD__
			ChangeStatusIconLevel(STATUS_ICON_SUBLCD_BATTERY_STRENGTH,80);
#endif
			break;
		case BATTERY_LEVEL_5:
			PRINT_INFORMATION(("BatteryStatusIndication voltage 5"));
			ChangeStatusIconLevel(STATUS_ICON_BATTERY_STRENGTH,100);
#ifdef __MMI_SUBLCD__
			ChangeStatusIconLevel(STATUS_ICON_SUBLCD_BATTERY_STRENGTH,100);
#endif
			break;
		default:
			PRINT_INFORMATION(("Battery Status Indication unknown voltage "));
			ChangeStatusIconLevel(STATUS_ICON_BATTERY_STRENGTH,33);
#ifdef __MMI_SUBLCD__
			ChangeStatusIconLevel(STATUS_ICON_SUBLCD_BATTERY_STRENGTH,33);
#endif
			break;
	}
}
//CSD End

 
/**************************************************************


	FUNCTION NAME		: CHECK_MO_ALLOW

  	PURPOSE				: This function checks if outgoing call is allowed or not. 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: BOOL

	RETURNS				: void

 


**************************************************************/
BOOL CHECK_MO_ALLOW(void)
{
   if( g_charbat_context.LowBatteryTxProhibitFlag == 0 )
      return MMI_TRUE; //allow MT call
   else
      return MMI_FALSE;  //prohibit MT call 
}

/**************************************************************


	FUNCTION NAME		: IS_LOW_BATTERY

  	PURPOSE				: This function checks if battery is low. 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: BOOL

	RETURNS				: void

 


**************************************************************/
BOOL IS_LOW_BATTERY(void)
{
   if( g_charbat_context.LowBatteryFlag == 1 )
      return MMI_TRUE; //Low battery
   else
      return MMI_FALSE; 
}


/**************************************************************


	FUNCTION NAME		: SET_LOW_BATTERY_ALARM_THRESHOLD

  	PURPOSE				: This function sets the thresh hold for low battery alarm. 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: BOOL

	RETURNS				: void

 


**************************************************************/
void SET_LOW_BATTERY_ALARM_THRESHOLD(U8 cnt)
{
	/*********************************************************/
	/***** The threshold will be changed when:	*****/
	/*********************************************************/
	/* 1. MT Call Accepted	*/
	/* 2. Call END	*/
	/* 3. Make MO Call	*/
	/* 4. SAT Call setup	*/
	/*********************************************************/
	
     if( lowBatteryAlarmThreshold == cnt )
		 return;

     lowBatteryAlarmThreshold = cnt;
     batteryLowCount=lowBatteryAlarmThreshold;
}

/**************************************************************


	FUNCTION NAME		: RESET_LOW_BATTERY_ALARM_CNT

  	PURPOSE				: This function reset battery low alert count 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 


**************************************************************/
void RESET_LOW_BATTERY_ALARM_CNT(void)
{
     /* Set counter to threshold value so that MMI will alert at next indication */
     /* (the indication will comes to MMI by 1 minute ( depends on custom setting) */
     batteryLowCount=lowBatteryAlarmThreshold;
}

/**************************************************************


	FUNCTION NAME		: BatteryIndicationPopup

  	PURPOSE				: General popup fn for charger events.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 

**************************************************************/

void BatteryIndicationPopup(U16 stringId)
{
   U8 pp_flag, tone_id=0;

    
   /* Framework support Popup display permisstion flag */
   /* Application who wanna stop popup has to intercept related messages and set the flag */
   pp_flag = GetInterruptPopupDisplay();
   if( (pp_flag & POPUP_DSPL_ON) == 0 )
       return;
     
   if( pp_flag & POPUP_TONE_ON )
      tone_id = AUX_TONE;

    
   if( GetTransitionToActiveFlag() == TRUE||(!IS_IDLE_ARRIVED) )
      return;
   
   DisplayPopup( (U8 *)GetString(stringId), \
                 IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, tone_id);
}


/**************************************************************

	FUNCTION NAME		: ExitChargerPopUpScr

  	PURPOSE				: Exit Charger pop up screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void ExitChargerPopUpScr(void)
{
	StopTimer(LOW_BATTERY_SCR_TIMER);
}

/**************************************************************


	FUNCTION NAME		: ChargingAbnormalHandler

  	PURPOSE				: Handle General Abnormal battery ind

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 

**************************************************************/
 
void ChargingAbnormalHandler( U16 stringId )
{
	if(!IdleIsPowerOnPeriod() && g_charbat_context.isChargerConnected )
	{
		AbnormalBatteryIndPopup( stringId );
		StartLEDPatternBatteryWarning();
		playRequestedTone(BATTERY_WARNING_TONE);   
	}
}


/**************************************************************


	FUNCTION NAME		: AbnormalBatteryIndPopup

  	PURPOSE				: Exit General Abnormal battery ind popup

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 

**************************************************************/
void AbnormalBatteryIndPopup(U16 stringId)
{
	static U16 LocalStringId;
	g_charbat_context.ChargingAbnormal = 1;  
	EntryNewScreen(SCR_ABNORMAL_CHARGING, ExitAbnormalBatteryIndPopup, NULL, NULL);
	ClearKeyEvents();
	if(stringId!=0)
		LocalStringId=stringId;
	ShowCategory64Screen(LocalStringId,IMG_GLOBAL_WARNING,0);
	ShowSubLCDScreen(ShowSubLCDAbnormalPopup);
     
    HideStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);
    ShowStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);

	 ClearAllKeyHandler();
     if( g_charbat_context.PowerOnCharger == 0 )
      {
         HideStatusIcon(STATUS_ICON_BATTERY_STRENGTH);
         ShowStatusIcon(STATUS_ICON_BATTERY_STRENGTH);
      }
     else
      {
         ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
         ClearKeyHandler(KEY_END,KEY_LONG_PRESS);  
      }
     UpdateStatusIcons();

      // CSD End
}

/**************************************************************


	FUNCTION NAME		: ExitAbnormalBatteryIndPopup

  	PURPOSE				: Exit General Abnormal battery ind popup

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 

**************************************************************/
void ExitAbnormalBatteryIndPopup(void)
{
	if(GetInterruptPopupDisplay()==POPUP_ALL_ON)
		GoBackSubLCDHistory();
}

/**************************************************************

	FUNCTION NAME		: ShowSubLCDAbnormalPopup

  	PURPOSE				: General Abnormal battery ind popup for sublcd

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ShowSubLCDAbnormalPopup(void)
{
	ExecSubLCDCurrExitHandler();
	ShowCategory304Screen(IMG_SUBLCD_ABNORMAL_BATTERY,0);
}

/**************************************************************

	FUNCTION NAME		: ShowLowPowerShutdownScr

  	PURPOSE				: Show Low power shutdown screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ShowLowPowerShutdownScr(void)
{
	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
	// lisen: 03/27/2004 replace DeInitFramework();        
	DeInitFramework_KeyAndEventHandler();
	
    ShowCategory64Screen(STR_LOW_BATTERY_SHUTDOWN,IMG_GLOBAL_WARNING,NULL);
 	BatteryStatusIndication(0);
	StartTimer(LOW_BATTERY_SCR_TIMER,LBAT_SHUTDOWN_SCR_TIMER_DUR,LowBatteryShutDownScrTimeout);

	 
	ChgrPlayLowBatTone();

}

//CSD Modified: Low Battery Power off, shall notify user.
/**************************************************************

	FUNCTION NAME		: LowBatteryShutDownScrTimeout

  	PURPOSE				: Call back fn when Low Battery shutdown timer expired.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void LowBatteryShutDownScrTimeout(void)
{
	StopTimer(LOW_BATTERY_SCR_TIMER);
//qiff modify for low battery without power off animation
#if 1 // __MMI_LOW_BATTERY_SHUTDOWN_NO_ANIMATION__
    QuitSystemOperation();
#else
      
     if( IdleIsPowerOnPeriod() )
       QuitSystemOperation();
     else
     ShutdownSystemOperation();
#endif

}
//CSD End
   
/**************************************************************

	FUNCTION NAME		: IdleScreenChargerConnected

  	PURPOSE				: update status icon display

	INPUT PARAMETERS	: playtone (TRUE/FALSE)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void IdleScreenChargerConnected(U8 playtone)
{
        if (playtone == KAL_TRUE)
	  playRequestedTone(AUX_TONE);
	  
	ShowStatusIcon(STATUS_ICON_BATTERY_STRENGTH);
	ShowStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);
	if( g_charbat_context.ChargingComplete == 0)   //added by guojian for bug 5999
	{
		AnimateStatusIcon(STATUS_ICON_BATTERY_STRENGTH);
		AnimateStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);
	}
	UpdateStatusIcons();
	TRACE_EF(g_sw_PM,"@@@@@ CHARGER CONNECTED @@@@@@@@@");
}

/**************************************************************

	FUNCTION NAME		: IdleScreenChargerDisconnected

  	PURPOSE				: update status icon display


	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void IdleScreenChargerDisconnected(void)
{	
	playRequestedTone(AUX_TONE);  
	HideStatusIcon(STATUS_ICON_BATTERY_STRENGTH);
	HideStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);
	ShowStatusIcon(STATUS_ICON_BATTERY_STRENGTH);
	ShowStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);
	UpdateStatusIcons();
	BatteryStatusIndication(g_charbat_context.chargerLevel);
	TRACE_EF(g_sw_PM,"@@@@@@ CHARGER DISCONNECTED @@@@@@@@@ g_charbat_context.chargerLevel=%d",g_charbat_context.chargerLevel);
}


/**************************************************************

	FUNCTION NAME		: IdleScreenBatteryFullCharged

  	PURPOSE				: update status icon display						  

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void IdleScreenBatteryFullCharged(void)
{	
	HideStatusIcon(STATUS_ICON_BATTERY_STRENGTH);
	HideStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);
	ShowStatusIcon(STATUS_ICON_BATTERY_STRENGTH);
	ShowStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);
	ChangeStatusIconLevel(STATUS_ICON_BATTERY_STRENGTH,100);
	ChangeStatusIconLevel(STATUS_ICON_SUBLCD_BATTERY_STRENGTH,100);
	UpdateStatusIcons();
	TRACE_EF(g_sw_PM,"@@@@@@ BATTERY FULL CHARGED @@@@@@@@@");
}

/**************************************************************


	FUNCTION NAME		: ShowChargingCompleteScr

  	PURPOSE				: Show Charging Complete screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 

**************************************************************/

void ShowChargingCompleteScr(void)
{
	EntryNewScreen(SCR_CHARGING_COMPLETE, NULL, ShowChargingCompleteScr, NULL);
	DinitHistory();
	ShowCategory9Screen(0, IMG_BATTERY_FULL_CHARGED, NULL);
	//ShowSubLCDScreen(ShowSubLCDChargingCompleteScr);
	#if defined(TGT_USING_POWER_KEY)
    ClearKeyHandler(KEY_POWER,KEY_EVENT_DOWN);
    SetKeyHandler(ChargingPwnOnThanPwnOn,KEY_POWER,KEY_LONG_PRESS);
	
	#else
    ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
    SetKeyHandler(ChargingPwnOnThanPwnOn,KEY_END,KEY_LONG_PRESS);
	#endif

}

/**************************************************************


	FUNCTION NAME		: ShowSubLCDChargingCompleteScr

  	PURPOSE				: Show sublcd charging complete screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 

**************************************************************/
void ShowSubLCDChargingCompleteScr(void)
{
	ExecSubLCDCurrExitHandler();
#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
	if (!IsClamClose())
		ShowCategory310Screen(get_string(STR_GLOBAL_LOGO));
	else
#endif
		ShowCategory304Screen(IMG_SUBLCD_CHARGING_COMPLETE,0);
	SetSubLCDExitHandler(ExitSubLCDChargingCompleteScr);

}

/**************************************************************


	FUNCTION NAME		: ExitSubLCDChargingCompleteScr

  	PURPOSE				: exit from sublcd charging complete screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 

**************************************************************/
void ExitSubLCDChargingCompleteScr(void)
{
	SubLCDHistoryNode SubLCDHistory;
	SubLCDHistory.entryFuncPtr=ShowSubLCDChargingCompleteScr;
	AddSubLCDHistory(&SubLCDHistory);
}



/**************************************************************


	FUNCTION NAME		: ChargerDetectTimeoutHdlr

  	PURPOSE				: Display Charger status after power on enter idle screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 

**************************************************************/
void ChargerDetectTimeoutHdlr(void)
{
      StopTimer( BOOTUP_CHARGER_DETECT_TIMER );
      
	   if( g_charbat_context.isChargerConnected == 1)
	   	{
         IdleScreenChargerConnected(KAL_TRUE);
	   	}
}
/**************************************************************


	FUNCTION NAME		: ChgrPlayLowBatTone

  	PURPOSE				: Display Charger status after power on enter idle screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 

**************************************************************/
void ChgrPlayLowBatTone(void)
{
   if(g_charbat_context.isChargerConnected == FALSE)
       playRequestedTone(BATTERY_LOW_TONE);
}

#define CHARGE_IND_STR_X ((UI_DEVICE_WIDTH>>1) -10)
#define CHARGE_IND_STR_Y ((UI_DEVICE_HEIGHT>>1) - 65)
//this function is called by adp only
static S16 Battery_mmi_handle = 0;
static U8  already_full_charged = 0;
void ChgrResetChargeVariable(void)
{
  Battery_mmi_handle = 0;
  already_full_charged = 0;
}
void ChgrUpdateScrCallback( U16 curr_v )
{
    char batry_v[30] = {0x00};
    U16  batry_u[16]={0x00};
    U8 curr_vv =0;
    color_t c = {0,0,0,100};
   // U8 sv=0;
   // U16 mv=0;

//the msg can't update right V ,need do it here
 //   PM_GetBatteryInfo(&sv,&curr_vv,&mv);

	CFW_EMOD_BATTERY_INFO batteryinfo;
	CFW_EmodGetBatteryInfo(&batteryinfo);
    curr_vv = batteryinfo.nChargeInfo.nBcl ;

    Battery_mmi_handle =  (curr_vv > Battery_mmi_handle)?curr_vv : Battery_mmi_handle;

    if(Battery_mmi_handle<0 )
        Battery_mmi_handle = 0;

    if(Battery_mmi_handle>100)
		Battery_mmi_handle=100;

    if(Battery_mmi_handle==100&&already_full_charged==1 )
        return;

    if(Battery_mmi_handle==100)
        already_full_charged = 1;
    
    mmi_trace(1," ChgrUpdateScrCallback -v %d   ",Battery_mmi_handle);
    
    if(GetActiveScreenId() ==SCR_CHARGER_POWER_ON)
    {
        Trace(" ChgrUpdateScrCallback %d",Battery_mmi_handle);

        sprintf(batry_v, "%d",Battery_mmi_handle);
        strcat(batry_v, "\%");
        mmi_trace(1," ChgrUpdateScrCallback %s   ",batry_v);
        AnsiiToUnicodeString((S8 *) batry_u, (S8 * )batry_v);
    //update Chgr screen the battery state
        gdi_layer_lock_frame_buffer();
        gdi_draw_solid_rect(CHARGE_IND_STR_X, CHARGE_IND_STR_Y,
            CHARGE_IND_STR_X+40,
            CHARGE_IND_STR_Y+20, 
            GDI_COLOR_TRANSPARENT);
#if defined(__PROJECT_GALLITE_C01__)
        UI_set_text_color(c);
#endif
        gui_move_text_cursor(CHARGE_IND_STR_X, CHARGE_IND_STR_Y );
        gui_print_text(batry_u);

        gdi_layer_unlock_frame_buffer();
        gdi_layer_blt_previous(CHARGE_IND_STR_X, CHARGE_IND_STR_Y,
            CHARGE_IND_STR_X+40,
            CHARGE_IND_STR_Y+20);
        
    }

   // gui_start_timer(5000 ,ChgrUpdateScrCallback);
}
