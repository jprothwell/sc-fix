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
 *  Modification Notice:
 *  --------------------------
 *  This software is modified by COOLSAND Inc. and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of COOLSAND Inc. (C) 2001
 *
 *******************************************************************************/

 /*******************************************************************************
 * Filename:
 * ---------
 *   HistoryHelper.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  .......
 *
 * Author:
 * -------
 * -------
 *
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/CallManagement/src/HistoryHelper.c-arc  $
 * 
 *    Rev 1.4   Mar 26 2007 17:25:46   lin
 * 通话后闹钟能正常运行
 * 
 *    Rev 1.3   Nov 30 2006 10:57:00   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:37:28   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:23:46   zhangxb
 * Initial revision.
 * 
 *    Rev 1.0   Nov 06 2006 17:32:54   zhangxb
 * Initial revision.
 * 
 *    Rev 1.2   Sep 19 2006 19:10:50   zhoumn
 * No change.
 * 
 *    Rev 1.1   Aug 28 2006 17:38:42   zhangxb
 * 合并了PSI和Simon修改的一些代码.
 * 
 *    Rev 1.0   Jul 06 2006 17:42:38   zhangxb
 * 
 *
 *******************************************************************************/

/**************************************************************

	FILENAME	: HistoryHelper.c

  	PURPOSE		: Call Structure Management API

 

	AUTHOR		: Ashish

	DATE		: Nov 8,02

**************************************************************/
#define __NEWSIMULATOR
#include "stdc.h"
#include "mmi_features.h"
#include "l4dr.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "debuginitdef.h"
#include "globalscrenum.h"
#include "historygprot.h"
#include "sublcdhistorygprot.h"
#include "wgui_categories.h"
#include "timerevents.h"
#include "keybrd.h"

#include "bootup.h"
#include "poweronchargerprot.h"
//#include "usbdevicegprot.h"   // zrx del 20050512
#include "satgprots.h"
#include "simdetectiongexdcl.h"
#include "alarmframeworkprot.h"
#include "messagesmiscell.h"
#include "phonebookdef.h"
#include "phonebookgprot.h"
#include "gpioinc.h"
#include "profilegprots.h"
#include "callhistoryenum.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#ifdef WAP_SUPPORT
#include "wapprot.h"
#endif

#include "callmanagementiddef.h"
#include "callmanagementstruct.h"
#include "callstructuremanagementprot.h"
#include "callsgprots.h"
#include "commonstubsprot.h"
#include "redialhelpergprot.h"
#include "callmanagementgprot.h"
#include "incomingcallmanagementprot.h"
#include "outgoingcallhelperprot.h"
#include "outgoingcallmanagementprot.h"
#include "idleappdef.h"

#include "historyhelperprot.h"

#if defined(__MMI_WEBCAM__)
#include "extdevicedefs.h"
#endif

#undef __NEWSIMULATOR
#ifdef __MMI_MULTI_SIM__
#include "dual_sim_calllog.h"
#endif
//static pBOOL			gPOPUpFlag=FALSE;
//static pBOOL			gNotifySSFlag= FALSE;
//static FuncPtr			gCbackAfterSS=NULL;

//pBOOL noSave = TRUE;
PU8 gdispnameForSubLCD=NULL;
#ifdef __MMI_SUPPORT_JME__
extern s8 vm_status;
#endif
#ifdef __MMI_CLAMCLOSE_BACK2IDLE__
extern BOOL IsClamClose(void);
//micha0601
//extern U8 gIsInSATCall;
#endif
#ifdef __MMI_MT_CALL_SPEAKER_MUTE__
extern BOOL GetSpeakerMuteFlag();
extern void SetSpeakerMuteFlag(BOOL bMute);
extern void ResumeSpeaker();
#endif
extern void mmi_msg_reset_new_msg_ind_call_end(void);
extern U8 mmi_msg_need_new_msg_ind_call_end (void);
extern void PendingUsbDetectionHdlr(void);

/**************************************************************

 

  	PURPOSE				: This function is the error screen which should never be
						  called.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrMarkerError(void)
{
	//U8* guiBuffer;

   EntryNewScreen(CM_SCR_MARKER, NULL, NULL, NULL);

	 

	SetLeftSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	StartTimer(CM_NOTIFYDURATION_TIMER, 1, GoBackfromNotifyScr);
}

/**************************************************************

 

  

	I 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void AddMarkerToHistory(void)
{
	history_t   scr101;
	U16 nHistory = 0;

	pfnUnicodeStrcpy((S8*)scr101.inputBuffer, (PS8)&nHistory);
	scr101.scrnID = CM_SCR_MARKER;
	scr101.entryFuncPtr = EntryScrMarkerError;
	AddHistory(scr101);
}

/**************************************************************

	FUNCTION NAME		: DeleteUptoCmScreen(void)

 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DeleteUptoCmScreen(void)
{
	DeleteUptoScrID(CM_SCR_MARKER);
}

/**************************************************************

	FUNCTION NAME		: DeleteAllCMSubLCDScreens(void)

  	PURPOSE				: Deletes All Sub LCD Screens in history

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DeleteAllCMSubLCDScreens(void)
{
	while(IsSubLCDScreenPresent(SUB_LCD_SCR_CM_ACTIVECALLSCREEN))
	{
		DeleteSubLCDScreen(SUB_LCD_SCR_CM_ACTIVECALLSCREEN);
	}

	while(IsSubLCDScreenPresent(SUB_LCD_SCR1001_CM_OUTGOINGCALLSCREEN))
	{
		DeleteSubLCDScreen(SUB_LCD_SCR1001_CM_OUTGOINGCALLSCREEN);
	}

	while(IsSubLCDScreenPresent(SUB_LCD_ITEMSCR_INCOMING_CALL))
	{
		DeleteSubLCDScreen(SUB_LCD_ITEMSCR_INCOMING_CALL);
	}
}

/**************************************************************

	FUNCTION NAME		: DeleteIncomingCallOptionScreen(void)

  	PURPOSE				: Deletes All incoming call option Screens in history

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DeleteIncomingCallOptionScreen(void)
{
	U16 scrnId = 0;

	GetPreviousScrnIdOf(SCR1009_CM_CALLLISTSCREEN, &scrnId);
	if (scrnId == ITEM_SCR_INC_MACT_OPN)
		DeleteScreenIfPresent(SCR1009_CM_CALLLISTSCREEN);

	/* deflect need to delete dial pad and phb list if any */
	GetPreviousScrnIdOf(SCR_PBOOK_LIST, &scrnId);
	if (scrnId == ITEM_SCR_CALL_DEFLECT_DIGIT_HANDLER_ID)
	{
		DeleteScreenIfPresent(SCR_PBOOK_LIST);
		DeleteScreenIfPresent(ITEM_SCR_CALL_DEFLECT_DIGIT_HANDLER_ID);
	}
	
   DeleteScreenIfPresent(ITEM_SCR_INC_MACT_OPN);
   DeleteScreenIfPresent(ITEM_SCR_INCOMING_OPTIONS);
}

/**************************************************************

	FUNCTION NAME		: SetNotifySSFlag

  	PURPOSE				: Sets Notify SS Flag dpeneding on flag

	INPUT PARAMETERS	: flag

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetNotifySSFlag(pBOOL flag)
{
	cm_p->history_info.IsNotifySS=flag;
}

/**************************************************************

	FUNCTION NAME		: GetNotifySSFlag

  	PURPOSE				: Returns Notify SS Flag

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: flag

 

**************************************************************/
pBOOL GetNotifySSFlag(void)
{
	return cm_p->history_info.IsNotifySS;
}

/**************************************************************

	FUNCTION NAME		: SetCbackAfterSS

  	PURPOSE				: Sets Function to be called after SS

	INPUT PARAMETERS	: Function

	OUTPUT PARAMETERS	: void

	RETURNS				: nil

 

**************************************************************/
void SetCbackAfterSS(FuncPtr fptr)
{
	cm_p->history_info.CbackAfterNotifySS=fptr;
}

/**************************************************************

	FUNCTION NAME		: GetCbackAfterSS

  	PURPOSE				: Returns Function to be called after SS

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: void

	RETURNS				: nil

 

**************************************************************/
FuncPtr GetCbackAfterSS(void)
{

	return cm_p->history_info.CbackAfterNotifySS;
}

/**************************************************************

	FUNCTION NAME		: SetPOPUpFlag

  	PURPOSE				:

	INPUT PARAMETERS	: TRUE for set, FALSE for not set

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void SetPOPUpFlag(pBOOL flag)
{
	cm_p->history_info.IsPopUp = flag;
}

/**************************************************************

	FUNCTION NAME		: GetPOPUpFlag

  	PURPOSE				: Gets the LoudSpFromOption Flag Status

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: TRUE for set, FALSE for not set

	RETURNS				: nil

 

**************************************************************/
pBOOL GetPOPUpFlag(void)
{
	return cm_p->history_info.IsPopUp;
}

/**************************************************************

 

  

 INPUT PARAMETERS : nil

 OUTPUT PARAMETERS :void

 RETURNS    : U16

 
**************************************************************/
U16 GetCmMarkerScrnID (void)
{
 	return CM_SCR_MARKER;
}

/**************************************************************

	FUNCTION NAME		: SetNoSave

  	PURPOSE				: Controls access to global history store buffer

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetNoSave(void)
{
	cm_p->history_info.IsSave = FALSE;
}

/**************************************************************

	FUNCTION NAME		: GetOutofApplication(void)

  	PURPOSE				: Helper to perform last rites and exit application

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CMHistoryHandler(void)
{
#ifdef __MMI_CLAMCLOSE_BACK2IDLE__
     if (IsClamClose() == MMI_TRUE && !IsInSATCall())
     {
       DeleteScreenIfPresent(SCR8055_CALLS_LIST_DIALED);
       DeleteScreenIfPresent(SCR8056_CALLS_LIST_MISSED);
       DeleteScreenIfPresent(SCR8054_CALLS_LIST_RECEIVED);
       DeleteScreenIfPresent(SCR8057_CALLS_LIST_MIXED);
     }
#endif

    mmi_trace(g_sw_CC, "MMI_Call: Func: %s Goback History", __FUNCTION__);
    GoBackHistory();

#ifdef __MMI_WEBCAM__
	if (GetExitScrnID() == SCR_ID_USB_WEBCAM)
	{
		/* if is in webcam screen, do not proess End key handler */
		return;
	}
#endif /* __MMI_WEBCAM__ */


#ifdef __MMI_CLAMCLOSE_BACK2IDLE__
   if ((IsClamClose() == MMI_TRUE && !IsInSATCall()) &&
       (GetExitScrnID() != ITEM_SCR_MISSED_CALL))
   {
		ExecuteCurrKeyHandler( KEY_END, KEY_EVENT_DOWN );
   }
#endif
}

/**************************************************************

	FUNCTION NAME		: GetOutofApplication(void)

  	PURPOSE				: Helper to perform last rites and exit application

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
 void GetOutOfCMApplication (void)
{
	// XXX CAUTION: If any changes are being done here,
	// corresponding changes have to be there in GetOutOfCMforAdjustHistory()

     mmi_trace(g_sw_CC, "MMI_Call: Func: %s ", __FUNCTION__);
	     
	StopRingtoneOrVibrator(); //fix bug 14240

   /* low battery */
   if( g_charbat_context.LowBatteryTxProhibitFlag == 1)
      SET_LOW_BATTERY_ALARM_THRESHOLD( LBAT_IDLE_DURCNT_PROHIBIT );
   else
      SET_LOW_BATTERY_ALARM_THRESHOLD( LBAT_IDLE_DURCNT_NORMAL );

	if(GetCallDeflectFlag()==TRUE)
	{
		DeleteScreenIfPresent(ITEM_SCR_CALL_DEFLECT_DIGIT_HANDLER_ID);
		SetCallDeflectFlag(FALSE);
	}

   /* reset redial data */
	if(IsRedialSet())
	{
		ResetRedialAttempts();
	}

	mmi_phb_call_set_dial_from_list(MMI_PHB_NONE);
	CHISTSetDialFromCallLogFlag (0);
	
	/* stop voice recording */
	if(GetVoiceRecordingFlag())
	{
		SetKeyPadStopVoiceRecordLongPress();
	}

	/* unmute phone */
	if(TRUE==IsPhoneInMute())
	{
		UnMutethePhone();
	}
#ifdef __MMI_MT_CALL_SPEAKER_MUTE__
	ResumeSpeaker();
#endif
     /* turn off loud speaker */
	if (GetHFreeHeld()==TRUE)
	{
		SetHFreeHeld(FALSE);
		DisbleHandsFree();
		SetLoudSpeaker(FALSE);
	}

	/* resume audio */
	mdi_audio_resume_background_play();  // zrx del 20060612//chenhe enable for jesmmine

//micha0831
	SATCallTerminate();

	/* reset DTMF flag */
	SetDTMFFlag(TRUE);

	/* restore CHIS setting */
#ifdef __MMI_MULTI_SIM__
	RestoreCHISContext();
#endif

	/* reset state and exit */
	ReinitalizeCallManagement();
   SetCallAbortReqSentFlag(FALSE);

	if(GetAbortCallRequestedFlag()==TRUE || GetTotalCallCount()==0)
	{
		DeleteAllCMSubLCDScreens();
		GoBackSubLCDHistory();
		SetAbortCallRequestedFlag(FALSE);
	}

	mmi_trace(g_sw_CC,"MMI_Call: Func: %s IsScreenPresent(CM_SCR_MARKER)=%d, GetCMScrnFlag()=%d", __FUNCTION__, IsScreenPresent(CM_SCR_MARKER), GetCMScrnFlag());
	if (IsScreenPresent(CM_SCR_MARKER) && GetCMScrnFlag()==FALSE)
	{
	   /* enter in-call option menu to go to other applications
         delete CM screens only and go back to that application */
           MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_GetOutOfCMApplication1));

		if(IsScreenPresent(SCR1003_CM_ACTIVECALLOPTIONS))
		{
			DeleteBetweenScreen(CM_SCR_MARKER,SCR1003_CM_ACTIVECALLOPTIONS);
		}
		else
		{
			if(!DeleteBetweenScreen(CM_SCR_MARKER,SCR_CM_ACTIVECALLSCREEN))
			{
				DeleteScreenIfPresent(CM_SCR_MARKER);
			}
		}

        CMHistoryHandler();
		SetCMScrnFlag(TRUE);
	}
	else
	{
	   /* all CM screens, delete all CM screen and go back */
           MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_GetOutOfCMApplication2));

		DeleteNScrId(CM_SCR_MARKER);
                CMHistoryHandler();
	}

	 
	DeleteScreenIfPresent(CM_SCR_MARKER);
	DeleteIncomingCallOptionScreen();
	

#ifdef __MMI_USB_SUPPORT__
   PendingUsbDetectionHdlr();
#endif
	AlmEnableExpiryHandler();   // zrx del 20060612  // opened yaosq 20070320 for alarm when calling
	PendingAlarmReminder();
	#if 0   // zrx del 20060612
	if(mmi_msg_need_new_msg_ind_call_end ())
	{
		mmi_msg_reset_new_msg_ind_call_end();
//		PlayMessageArrivalTone();
	}
	#endif

#ifdef WAP_SUPPORT
	/* Callback for WAP widget */
	widget_MMI_on_exit_CM();
#endif
	return;
}

/**************************************************************

	FUNCTION NAME		: GetOutOfCMforAdjustHistory(void)

  	PURPOSE				: Helper to perform last rites and exit application

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GetOutOfCMforAdjustHistory (void)
{
     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_GetOutOfCMforAdjustHistory));

   /* low battery */
   if( g_charbat_context.LowBatteryTxProhibitFlag == 1)
      SET_LOW_BATTERY_ALARM_THRESHOLD( LBAT_IDLE_DURCNT_PROHIBIT );
   else
      SET_LOW_BATTERY_ALARM_THRESHOLD( LBAT_IDLE_DURCNT_NORMAL );

	if(GetCallDeflectFlag()==TRUE)
	{
		DeleteScreenIfPresent(ITEM_SCR_CALL_DEFLECT_DIGIT_HANDLER_ID);
		SetCallDeflectFlag(FALSE);
	}

   /* reset redial data */
	if(IsRedialSet())
	{
		ResetRedialAttempts();
	}

	mmi_phb_call_set_dial_from_list(MMI_PHB_NONE);
	CHISTSetDialFromCallLogFlag (0);

   /* stop voice recording */
   if(GetVoiceRecordingFlag())
	{
		SetKeyPadStopVoiceRecordLongPress();
	}

   /* unmute phone */
	if(TRUE==IsPhoneInMute())
	{
		UnMutethePhone();
	}

     /* turn off loud speaker */
	if (GetHFreeHeld()==TRUE)
	{
		SetHFreeHeld(FALSE);
		DisbleHandsFree();
		SetLoudSpeaker(FALSE);
	}

	/* resume audio */
	mdi_audio_resume_background_play();  // zrx del 20060612 //chenhe enable for jesmmine

//micha0831
	SATCallTerminate();

	/* reset DTMF flag */
	SetDTMFFlag(TRUE);

	// reset state and exit
	ReinitalizeCallManagement();
   SetCallAbortReqSentFlag(FALSE);

	if(GetAbortCallRequestedFlag()==TRUE || GetTotalCallCount()==0)
  	{
		DeleteAllCMSubLCDScreens();
		GoBackSubLCDHistory();
		SetAbortCallRequestedFlag(FALSE);
	}

	if (IsScreenPresent(CM_SCR_MARKER) && GetCMScrnFlag()==FALSE)
	{
	   /* enter in-call option menu to go to other applications
         delete CM screens only */
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_GetOutOfCMforAdjustHistory1));

		if(IsScreenPresent(SCR1003_CM_ACTIVECALLOPTIONS))
		{
			DeleteBetweenScreen(CM_SCR_MARKER,SCR1003_CM_ACTIVECALLOPTIONS);
			DeleteIncomingCallOptionScreen();
		}
		else
		{
			if(!DeleteBetweenScreen(CM_SCR_MARKER,SCR_CM_ACTIVECALLSCREEN))
			{
				DeleteScreenIfPresent(CM_SCR_MARKER);
			}
		}

		SetCMScrnFlag(TRUE);
	}else
	{
	   /* all CM screens, delete all CM screen */
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_GetOutOfCMforAdjustHistory2));

		DeleteNScrId(CM_SCR_MARKER);
	}

	 
	DeleteScreenIfPresent(CM_SCR_MARKER);

/*
#ifdef __MMI_CLAMCLOSE_BACK2IDLE__
	if (IsClamClose())
         ExecuteCurrKeyHandler( KEY_END, KEY_EVENT_DOWN );
#endif
*/

#ifdef __MMI_USB_SUPPORT__
	PendingUsbDetectionHdlr();
#endif
#if 0 // zrx del 20060612
	AlmEnableExpiryHandler();
	//PendingAlarmReminder();
	if(mmi_msg_need_new_msg_ind_call_end ())
	{
		mmi_msg_reset_new_msg_ind_call_end();
//		PlayMessageArrivalTone();
	}
	#endif

#ifdef WAP_SUPPORT
	/* Callback for WAP widget */
	widget_MMI_on_exit_CM();
#endif
	return;
}

/**************************************************************

	FUNCTION NAME		: GoBackfromNotifyScr(void)

  	PURPOSE				: This is the function which is invoked when there is a timeout
						  or user presses the LSK of the notification screen.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GoBackfromNotifyScr(void)
{
     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_GoBackfromNotifyScr));

	StopTimer(CM_NOTIFYDURATION_TIMER);
	SetPOPUpFlag(FALSE);

	mmi_trace(g_sw_CC,"MMI_Call: Func: %s GetCbackAfterSS()=%d", __FUNCTION__, GetCbackAfterSS());
	if(GetCbackAfterSS())
	{
		ExitScrShowSSEvent();
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_GoBackfromNotifyScr1));
		return;
	}
	SetNotifySSFlag(FALSE);

	AdjustSubLcd();

	mmi_trace(g_sw_CC,"MMI_Call: Func: %s GetTotalCallCount()=%d", __FUNCTION__, GetTotalCallCount());
	if(GetTotalCallCount()==0)
	{
		mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);
		if (GetHFreeHeld()==TRUE)
		{
			SetHFreeHeld(FALSE);
			DisbleHandsFree();
			SetLoudSpeaker(FALSE);
		}
        DeleteScreenIfPresent(IDLE_SCREEN_DIGIT_HANDLER_ID);

	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_GoBackfromNotifyScr2));
		GetOutOfCMApplication();
		return;
	}

	if (GetCurrentState() == CM_OUTGOING_STATE && GetActivatedCcbsFlag()!=TRUE)
	{
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_GoBackfromNotifyScr3));
	   if (!IsScreenPresent(SCR1001_CM_OUTGOINGCALLSCREEN))
		{
			EntryScr1001OutgoingCall();
			if (IsScreenPresent(CM_SCR_MARKER) == FALSE)
			{
				AddMarkerToHistory();
			}
		}
		else
		GoBackToHistory(SCR1001_CM_OUTGOINGCALLSCREEN);
		return;
	}

		/* MT call caller picture, ringtone...will be overwrite by MO call, 
		it needs to be restored afterwards */
	if (GetCurrentState() == CM_INCOMING_STATE && GetPreviousState() == CM_OUTGOING_STATE)
		{
			CM_CALL_HANDLE handle;
			PHB_CM_INTERFACE phb_data;
			
			handle = GetIncomingCallHandle();
#ifdef __MMI_MULTI_SIM__
			phb_data = mmi_phb_call_get_data_for_call_mgnt(GetCallNumber(handle), MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL));
#else
			phb_data = mmi_phb_call_get_data_for_call_mgnt(GetCallNumber(handle));
#endif	

#ifdef __MMI_INCOMING_CALL_VIDEO__
         /* waiting call and ccbs invoke can't use video for display */
#ifdef  __MMI_SUPPORT_JME__
            if ((GetTotalCallCount() > 0 || GetCCBSFlag() == TRUE)  || (vm_status))
#else
            if (GetTotalCallCount() > 0 || GetCCBSFlag() == TRUE)
#endif
         {
         	phb_data.videoId = 0;
         }
#endif
			SetCMPhoneBookStruct(&phb_data);
		}

	if(GetCurrScrnId()==DTMF_SCR_DISPLAY_ID)
	{
		if (GetCurrentState() == CM_INCOMING_STATE && 1==GetTotalCallCount())
		{
			if (GetCallWaitFlag()&&!GetNoTwiceRingFlag()&&
				mdi_audio_is_speech_mode()== FALSE)
			{
				SetWait2RingCallFlag(TRUE);
				SetNoTwiceRingFlag(TRUE);
				ShowIncomingCallIndication();
				DeleteScreenIfPresent(DTMF_SCR_DISPLAY_ID);
		   	}
		}
		
		GoBackHistory();
		return;
	}

	if (GetCurrentState() == CM_INCOMING_STATE)
	{
		if((1==GetTotalCallCount())&&(!IsScreenPresent(ITEMSCR_INCOMING_CALL)))
		{
			DeleteNScrId(CM_SCR_MARKER);
		}

      if (IsScreenPresent(ITEMSCR_INCOMING_CALL))
      {
			GoBackToHistory(ITEMSCR_INCOMING_CALL);
      }
      else
      {
         EntryScrIncomingCallEvent();
      }
      
      if ((GetTotalCallCount() == 1) && (GetRingingFlag() == FALSE))
      	ShowIncomingCallIndication();
      
           MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_GoBackfromNotifyScr4));
		return;
	}

	DeleteIncomingCallOptionScreen();
	DeleteNScrId(ITEM_SCR_CALL_DEFLECT_DIGIT_HANDLER_ID);

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_GoBackfromNotifyScr6));

	if(GetCMScrnFlag()==FALSE || IsScreenPresent(ITEM_SCR_USSN_MSG))
	{
		/* SS present, active call present, go back to SS screen */
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_GoBackfromNotifyScr7));
		GoBackHistory();
		return;
	}

	if(GetWapCallPresent())
	{
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_GoBackfromNotifyScr8));
	       
	      DeleteScreenIfPresent(CM_SCR_MARKER);
		GoBackHistory();
		return;
	}



	if (IsScreenPresent(DTMF_SCR_DISPLAY_ID))
	{
	   MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_GoBackfromNotifyScr5));
		GoBackToHistory(DTMF_SCR_DISPLAY_ID);
		return;
	}
	
	if(IsScreenPresent(SCR_CM_ACTIVECALLSCREEN))
	{
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_GoBackfromNotifyScr9));
		GoBackToHistory(SCR_CM_ACTIVECALLSCREEN);
	}
	else
	{
		if(GetExitScrnID() == SCR_CM_ACTIVECALLSCREEN)
		{
	     		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_GoBackfromNotifyScr10));
			return;
		}
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_GoBackfromNotifyScr11));
		EntryScr1002ActiveCall();
		if (!IsScreenPresent(CM_SCR_MARKER))
		AddMarkerToHistory();
	}
}

/**************************************************************

	FUNCTION NAME		: AdjustHistoryForCM(void)

  	PURPOSE				: This is the function which is invoked when there is a timeout
						  or user presses the LSK of the notification screen.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void AdjustHistoryForCM(void)
{
     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_AdjustHistoryForCM));

	if(GetCbackAfterSS())
	{
		ExitScrShowSSEvent();
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_AdjustHistoryForCM1));
		return;
	}
	SetNotifySSFlag(FALSE);

	AdjustSubLcd();

	if(GetTotalCallCount()==0)
	{
		mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);
	   // side effect: sound recorder in call, the loud speaker cannot be turned off
		if (GetHFreeHeld()==TRUE)
		{
			SetHFreeHeld(FALSE);
			DisbleHandsFree();
			SetLoudSpeaker(FALSE);
		}
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_AdjustHistoryForCM2));
		GetOutOfCMforAdjustHistory();
		return;
	}

	if (GetCurrentState() == CM_OUTGOING_STATE && GetActivatedCcbsFlag()!=TRUE)
	{
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_AdjustHistoryForCM3));
		DeleteUptoScrID(SCR1001_CM_OUTGOINGCALLSCREEN);
		return;
	}
	if (GetCurrentState() == CM_INCOMING_STATE)
	{
		if((1==GetTotalCallCount())&&(!IsScreenPresent(ITEMSCR_INCOMING_CALL)))
		{
			DeleteNScrId(CM_SCR_MARKER);
		}

		DeleteUptoScrID(ITEMSCR_INCOMING_CALL);

		/* MT call caller picture, ringtone...will be overwrite by MO call, 
		it needs to be restored afterwards */
		if (GetPreviousState() == CM_OUTGOING_STATE)
		{
			CM_CALL_HANDLE handle;
			PHB_CM_INTERFACE phb_data;
			
			handle = GetIncomingCallHandle();
#ifdef __MMI_MULTI_SIM__
			phb_data = mmi_phb_call_get_data_for_call_mgnt(GetCallNumber(handle), MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL));
#else
			phb_data = mmi_phb_call_get_data_for_call_mgnt(GetCallNumber(handle));
#endif				

#ifdef __MMI_INCOMING_CALL_VIDEO__
         /* waiting call and ccbs invoke can't use video for display */
#ifdef  __MMI_SUPPORT_JME__
            if ((GetTotalCallCount() > 0 || GetCCBSFlag() == TRUE)  || (vm_status))
#else
            if (GetTotalCallCount() > 0 || GetCCBSFlag() == TRUE)
#endif         
         {
         	phb_data.videoId = 0;
         }
#endif
			SetCMPhoneBookStruct(&phb_data);
		}
		
		if (1==GetTotalCallCount())
		{
			if (GetCallWaitFlag()&&!GetNoTwiceRingFlag()&& 
				mdi_audio_is_speech_mode() == FALSE)
			{
				SetWait2RingCallFlag(TRUE);
				SetNoTwiceRingFlag(TRUE);
				ShowIncomingCallIndication();
	   	}
		}
		
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_AdjustHistoryForCM4));
		return;
	}

	if (IsScreenPresent(DTMF_SCR_DISPLAY_ID))
	{
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_AdjustHistoryForCM5));
		DeleteUptoScrID(DTMF_SCR_DISPLAY_ID);
		return;
	}
     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_AdjustHistoryForCM6));

	if(GetCMScrnFlag()==FALSE || IsScreenPresent(ITEM_SCR_USSN_MSG))
	{
		/* SS present, active call present, go back to SS screen */
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_AdjustHistoryForCM7));
		return;
	}

	if(IsScreenPresent(SCR_CM_ACTIVECALLSCREEN))
	{
		DeleteUptoScrID(SCR_CM_ACTIVECALLSCREEN);
	}
	else
	{
		if (GetWapCallPresent())
		   return;

		if(GetExitScrnID() == SCR_CM_ACTIVECALLSCREEN)
		{
		     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_AdjustHistoryForCM8));
			return;
		}
#if 1		
		EntryScr1002ActiveCall();
		if (!IsScreenPresent(CM_SCR_MARKER))
			AddMarkerToHistory();
#else
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
#endif		
	}

}

/**************************************************************

	FUNCTION NAME		: AdjustSubLcd

	PURPOSE				: Adjusts sub-lcd scrn according to state

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void AdjustSubLcd(void)
{
	ExecSubLCDCurrExitHandler();

	if((GetCurrentState()==CM_ACTIVE_STATE)||(GetCurrentState()==CM_HOLD_STATE))
	{
		ShowCategory304Screen(IMG_CM_STATE_CONNECTED,NULL);
		SetSubLCDExitHandler(ExitSubLcdScr1002ActiveCall);
		return;
	}

	if((GetCurrentState()==CM_OUTGOING_STATE))
	{
		ShowCategory311Screen(gdispnameForSubLCD,IMG_CM_SUBLCD_CALLING,NULL);
		SetSubLCDExitHandler(ExitSubLcdScr1001OutgoingCall);
		return;
	}

	if((GetCurrentState()==CM_INCOMING_STATE))
	{
		if (GetTotalCallCount() == 1)
			DeleteSubLCDScreen(SUB_LCD_SCR_CM_ACTIVECALLSCREEN);
		
		ShowCategory311Screen(gdispnameForSubLCD,IMG_CM_SUBLCD_CALLING,NULL);
		SetSubLCDExitHandler(ExitSubLcdScrIncomingCallEvent);
		return;
	}
}


