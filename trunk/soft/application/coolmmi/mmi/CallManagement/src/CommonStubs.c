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
 *   CommonStubs.c
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
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/CallManagement/src/CommonStubs.c-arc  $
 * 
 *    Rev 1.8   Jan 09 2007 11:05:34   lin
 * 1）打开void VibratorThenRing(void)函数中注释掉的部分
 * 2）打开被注释掉的VibratorOff()函数
 * 
 *    Rev 1.7   Dec 29 2006 21:15:54   lin
 * 合入audio的代码
 * 
 *    Rev 1.6   Dec 28 2006 11:07:12   lin
 * 打开call模块的来电播放铃声代码
 * 
 *    Rev 1.5   Dec 26 2006 17:59:16   zhangxb
 * 提交Audio代码。
 * 
 *    Rev 1.4   Dec 06 2006 12:01:10   lin
 * 修改LoudSpeakerFunction（）函数
 * 
 *    Rev 1.3   Nov 30 2006 10:57:00   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:37:26   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:23:44   zhangxb
 * Initial revision.
 * 
 *    Rev 1.0   Nov 06 2006 17:32:54   zhangxb
 * Initial revision.
 * 
 *    Rev 1.1   Sep 19 2006 19:10:50   zhoumn
 * No change.
 * 
 *    Rev 1.0   Jul 06 2006 17:42:38   zhangxb
 * Initial revision.
 * 
 *    Rev 1.0   Jul 04 2006 22:56:46   zhangx
 *
 *******************************************************************************/

#define __NEWSIMULATOR
#include "stdc.h"
#include "mmi_features.h"
#include "l4dr.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globaldefs.h"
#include "debuginitdef.h"
#include "wgui_status_icons.h"
#include "timerevents.h"

#include "gpioinc.h"
#include "profilesenum.h"
#include "profilesdefs.h"
#include "profilegprots.h"
#include "settingprofile.h"
#include "settinggprots.h"
#include "phonebookdef.h"
#include "phonebooktypes.h"
#include "phonebookgprot.h"
#include "volumehandler.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "audioinc.h"

#include "wgui_categories_cm.h"
#include "callmanagementiddef.h"
#include "callmanagementstruct.h"
#include "callmanagementgprot.h"
#include "callstructuremanagementprot.h"
#include "pscallhelpergprot.h"
#include "outgoingcallhelperprot.h"
#include "outgoingcallmanagementprot.h"

#include "commonstubsprot.h"
#include "outgoingcmscriddef.h"//niej
#include "mmi_trace.h"

#ifdef __MMI_BT_SUPPORT__
#include "btmmiscrgprots.h"
#endif

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_call.h"
#endif

#undef __NEWSIMULATOR

#include "mmi_trace.h"

 
#ifdef __MMI_SOUND_RECORDER__
extern void mmi_sndrec_entry_from_call(void);
#endif // __MMI_SOUND_RECORDER__
 

extern U16 CMIdleScreenDigits[];
extern U16 currTopScrnID;
//static U16 gMyRing;
//static U16 gBackLight;
//static U16 gHFreeHeldFlag=FALSE;
//static pBOOL gDtmfFlag = TRUE;
//static pBOOL gIsSilent=FALSE;
//static pBOOL gPhoneinMute=FALSE;
//static pBOOL gLSpkNowOff=FALSE;
//static pBOOL gIsInternalAlert=FALSE;
//static pBOOL gVoiceRecordingFlag=FALSE;
//static MMI_ALERT_TYPE gAlertType;

/**************************************************************

	FUNCTION NAME		: IsPhoneInMute(void)

  	PURPOSE				: This function informs whethere the phone is in mute or not.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: U16

	RETURNS				: returns TRUE if muted else FALSE

 
**************************************************************/
U16 IsPhoneInMute(void)
{
	return cm_p->alert_info.IsMute;
}

/**************************************************************

	FUNCTION NAME		: MutethePhone(void)

  	PURPOSE				: This function mutes the phone i,e currently changing the global flag.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: changes the value of gPhoneinMute.

	RETURNS				: void

 
**************************************************************/
void MutethePhone(void)
{
	cm_p->alert_info.IsMute=TRUE;
	MuteOnMicrophone();
	ShowStatusIcon(STATUS_ICON_MUTE);
	UpdateStatusIcons();
}

/**************************************************************

	FUNCTION NAME		: UnMutethePhone(void)

  	PURPOSE				: This function informs whethere the phone is in mute or not.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: U16

	RETURNS				: returns TRUE if muted else FALSE

 
**************************************************************/
void UnMutethePhone(void)
{
	cm_p->alert_info.IsMute=FALSE;
	MuteOffMicrophone();
	HideStatusIcon(STATUS_ICON_MUTE);
	UpdateStatusIcons();
}

/**************************************************************

	FUNCTION NAME		: IsPhoneSilent(void)

  	PURPOSE				: This function informs whethere the phone is silent or not.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: U16

	RETURNS				: returns TRUE if silent else FALSE

 
**************************************************************/
pBOOL IsPhoneInSilent(void)
{
	return cm_p->alert_info.IsSilent;
}

/**************************************************************

	FUNCTION NAME		: UnSilencethePhone(void)

  	PURPOSE				: This function silences the phone

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	:

	RETURNS				: void

 
**************************************************************/
void UnSilencethePhone(void)
{
	cm_p->alert_info.IsSilent=FALSE;
}

/**************************************************************

	FUNCTION NAME		: SetLSpkNowOff

	PURPOSE				:

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
void SetLSpkNowOff(pBOOL flag)
{
	cm_p->alert_info.IsLoudSpk = flag;
}

/**************************************************************

	FUNCTION NAME		: GetLSpkNowOff

	PURPOSE				:

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
pBOOL GetLSpkNowOff(void)
{
	return cm_p->alert_info.IsLoudSpk;
}

/**************************************************************

	FUNCTION NAME		: SetVoiceRecordingFlag

	PURPOSE				:

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
void SetVoiceRecordingFlag(pBOOL flag)
{
	cm_p->alert_info.IsRecord = flag;
}
/**************************************************************

	FUNCTION NAME		: GetVoiceRecordingFlag

	PURPOSE				:

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
pBOOL GetVoiceRecordingFlag(void)
{
	return cm_p->alert_info.IsRecord;
}

/**************************************************************

	FUNCTION NAME		: CMSetupAlertForMT(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CMSetupAlertForMT (void)
{
   PHB_CM_INTERFACE *PhoneNumberStruct;
   CM_CALL_HANDLE tmp;
#ifdef __MMI_INCOMING_CALL_VIDEO__   
   U16 imgId;
#endif
#ifdef __MMI_PROFILE_ALERT_TYPE_DOMINANT__
   MMI_ALERT_TYPE alert_type;
#endif

   tmp = GetIncomingCallHandle();
   PhoneNumberStruct=GetCMPhoneBookStruct();
               
   cm_p->alert_info.RingToneId = PhoneNumberStruct->ringtoneId;
   cm_p->alert_info.BacklightPatternId = PhoneNumberStruct->backlightId;
   cm_p->alert_info.AlertType = PhoneNumberStruct->alertType;

#ifdef __MMI_INCOMING_CALL_VIDEO__
   imgId = GetCallImgId(tmp);
   if (((imgId & 0x8000) || ((imgId & 0x3fff) >= VDO_ID_PHB_MTCALL_1)) && (imgId & 0x4000))
      /* user defined video file or system default video resource */
      /* use video sound as ring tone */
      cm_p->alert_info.RingToneId = CM_RINGTONE_VIDEO;
   else
#endif
   /* phb not set ring tone => use profile default */
	if (cm_p->alert_info.RingToneId==0)
	{
#ifdef __MMI_DUAL_PROFILE_SUPPORT__
		if (MTPNP_AD_IsMasterChannel(MTPNP_AD_CALL_CHANNEL))
		{
		      cm_p->alert_info.RingToneId = GetRingToneID();
		}
		else
		{
		      cm_p->alert_info.RingToneId = GetCard2RingToneID();
		}
#else  /* No Defined __MMI_DUAL_PROFILE_SUPPORT__ */
      cm_p->alert_info.RingToneId = GetRingToneID();
#endif /* __MMI_DUAL_PROFILE_SUPPORT__ */
	}
	//added by PanXu on 20061226
   #if 1  // zrx del 20060612
   /* phb not set backlight pattern => use profile default */
   if(cm_p->alert_info.BacklightPatternId==0)
      cm_p->alert_info.BacklightPatternId=GetLightStatus();
   
   /* phb not set alert type => use profile default */
   if (cm_p->alert_info.AlertType == MMI_ALERT_NONE)
      cm_p->alert_info.AlertType = GetMtCallAlertTypeEnum();
#endif
#ifdef __MMI_PROFILE_ALERT_TYPE_DOMINANT__
   // get alert type from profile
   alert_type = GetMtCallAlertTypeEnum();
   // if alert type is vibration only or silent, do not ring according to the caller group
   if( alert_type == MMI_VIBRATION_ONLY || alert_type == MMI_SILENT )
      cm_p->alert_info.AlertType = alert_type;
#endif

#ifdef __MMI_CM_DATA_CALL_ONLY_PLAY_TONE__
   if (GetCallType(tmp) == CM_DATA_CALL)
      cm_p->alert_info.RingToneId = TONE_DATA_CALL;

   if (GetPreviousCallWAP() == TRUE)
   {
      SetPreviousCallWAP(FALSE);
      cm_p->alert_info.RingToneId = TONE_DATA_CALL;
   }
#endif
}

/**************************************************************

	FUNCTION NAME		: ShowIncomingCallIndication(void)

  	PURPOSE				: This function is called to Show incoming call indication

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ShowIncomingCallIndication(void)
{
    /* before show incoming call indication, it must entered incoming call screen 
    which will already suspend audio, no need to stop again */
    //StopRingtoneOrVibrator();

    mmi_trace(g_sw_CC,"ShowIncomingCallIndication\n");

    CMSetupAlertForMT();
    aud_speech_set_mode(KAL_FALSE);
    StartRingtoneOrVibrator(); //changed by panxu 2007-7-6
    SetRingingFlag(TRUE);

    if(GetCCBSFlag()==TRUE)
        StartTimer(CM_CCBS_NOTIFYDURATION_TIMER,
        CM_CCBS_NOTIFY_TIMEOUT,StopIncomingCallIndication);
}

/**************************************************************

	FUNCTION NAME		: StopIncomingCallIndication(void)

  	PURPOSE				: This function is called to Stop incoming call indication

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
// zrx add end
void StopIncomingCallIndication(void)
{
	SetRingingFlag(FALSE);

	if (GetCallWaitFlag()&&(!GetWait2RingCallFlag()))
	{
	   /* stop waiting call indication */
		SetCallWaitFlag(FALSE);
	   MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_StopRing1));
		AudioStopReq (TONE_CALL_WAITING);


		SetNoTwiceRingFlag(FALSE);
	}
	else
	{
	   /* stop incoming call indication */
		if(GetWait2RingCallFlag())
		{
			SetWait2RingCallFlag(FALSE);
			SetNoTwiceRingFlag(FALSE);
		}
	   MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_StopRing2));
		StopRingtoneOrVibrator();
	}

	if(GetAutoAnswerFlag()==TRUE)
	{
		StopTimer(CM_AUTOANSWER_NOTIFYDURATION_TIMER);
		SetAutoAnswerFlag(FALSE);
	}

	if(GetCCBSFlag()==TRUE)
	{
		StopTimer(CM_CCBS_NOTIFYDURATION_TIMER);
	}

	SetRingAfterVibFlag(FALSE);
	
   if (GetExitScrnID() ==  ITEMSCR_INCOMING_CALL && GetTotalCallCount() == 1)
   {
#ifdef __MMI_INCOMING_CALL_VIDEO__
      CM_CALL_HANDLE handle;
      U16 imgId;
      
      handle = GetIncomingCallHandle();
      imgId = GetCallImgId(handle);
         
      /* pause video */
      if ((imgId & 0x8000) || ((imgId & 0x3fff) >= VDO_ID_PHB_MTCALL_1))
         StopCategory17Video();
      else
#endif
      MOMTHideAnimation();
   }
	
}

/**************************************************************

	FUNCTION NAME		: CMPlayMTRingTone(void)

  	PURPOSE				: This function is called to play ring or vibrator
  							  according to current setting

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CMPlayMTRingTone (U16 tone)
{
#ifdef __MMI_INCOMING_CALL_VIDEO__
   if (tone != CM_RINGTONE_VIDEO)
#endif      
      StartRingTone(tone);
}

/**************************************************************

	FUNCTION NAME		: StartRingtoneOrVibrator(void)

  	PURPOSE				: This function is called to play ring or vibrator
  							  according to current setting

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void StartRingtoneOrVibrator(void)
{
	TRACE_SYS_FUNCTION();
	TRACE_EF(g_sw_AUDIO, "cm_p->alert_info.AlertType = %d, cm_p->alert_info.RingToneId = %d", cm_p->alert_info.AlertType, cm_p->alert_info.RingToneId);

#if ( defined(__MMI_BT_SUPPORT__))

   if( mmi_bt_is_aud2hf() == MMI_TRUE )
   {
	  mmi_bt_start_hf_ring_req();
   }
   //else /* Play bluetooth headset in-band tone and loud-speaker ring tone at the same time */
#endif
#if 1 // zrx del 20060612 //added by panxu 20061226
   switch (cm_p->alert_info.AlertType)
	{
	case MMI_RING:
         //StartRingTone (cm_p->alert_info.RingToneId);
         CMPlayMTRingTone(cm_p->alert_info.RingToneId);
		break;
	case MMI_VIBRATION_ONLY:
		VibratorOn();
		break;
	case MMI_VIBRATION_AND_RING:
		VibratorOn();

		/************************************************/
		 
		/* AT command will query MMI when needed                   */
		/************************************************/
		/* NSC This API is not correct not integrated wit Profiles Silent Mode*/
		/*
		if(IsPhoneInSilent())
			SilentModeOff();
		*/
         //StartRingTone (cm_p->alert_info.RingToneId);
         CMPlayMTRingTone(cm_p->alert_info.RingToneId);
		break;
	case MMI_VIBRATION_THEN_RING:
		VibratorThenRing();
		break;
	case MMI_SILENT:
		break;
	case MMI_ALERT_NONE:
		break;
	default:
         //StartRingTone (cm_p->alert_info.RingToneId);
         CMPlayMTRingTone(cm_p->alert_info.RingToneId);
		break;
	}
   #endif
  	//PlayPatternCallerGroup((MMI_LEDLIGHT_STATUS_ENUM)cm_p->alert_info.BacklightPatternId);   // zrx del 20060612
}

BOOL IsVibratorEnable(void)
{
        U8 curr_alert_type;
	 curr_alert_type = (U8)GetMtCallAlertTypeEnum();
     
    mmi_trace(1," cm_p->alert_info.AlertType %d  ",curr_alert_type);
    switch (curr_alert_type)
    {
        case MMI_VIBRATION_ONLY:
        case MMI_VIBRATION_AND_RING:
        case MMI_VIBRATION_THEN_RING:
            return TRUE;
        break;
        default:
            return FALSE;
        break;
    }
}
BOOL IsRingEnable(void)
{
        U8 curr_alert_type;
	 curr_alert_type = (U8)GetMtCallAlertTypeEnum();
     
    mmi_trace(1," cm_p->alert_info.AlertType %d  ",curr_alert_type);
    switch (curr_alert_type)
    {
        case MMI_VIBRATION_ONLY:
        case MMI_SILENT:
            return FALSE;
        break;
        default:
            return TRUE;
        break;
    }
}

/**************************************************************

	FUNCTION NAME		: StopRingtoneOrVibrator

  	PURPOSE				: This function stops RingTone or Vibrarator depending on Alert Type

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void StopRingtoneOrVibrator(void)
{

#if  defined(__MMI_BT_SUPPORT__)
   if( mmi_bt_is_aud2hf() == MMI_TRUE )
   {
	  mmi_bt_stop_hf_ring_req();
   }
   //else /* Play bluetooth headset in-band tone and loud-speaker ring tone at the same time */
#endif

	mmi_trace(g_sw_CC,"MMI_Call Func: %s AlertType:%d", __FUNCTION__, cm_p->alert_info.AlertType);
	switch (cm_p->alert_info.AlertType)
	{
		case MMI_RING:
			StopRingTone(cm_p->alert_info.RingToneId);
			break;
		case MMI_VIBRATION_ONLY:
			VibratorOff();
			break;
		case MMI_VIBRATION_AND_RING:
			StopRingTone(cm_p->alert_info.RingToneId);
			VibratorOff();
			break;
		case MMI_VIBRATION_THEN_RING:
			/* NSC Does not make the mobile go back into Silent Mode */
			StopTimer(CM_ALERT_NOTIFYDURATION_TIMER);
			VibratorOff();
			StopRingTone(cm_p->alert_info.RingToneId);
			break;
		case MMI_SILENT:
			break;
		case MMI_ALERT_NONE:
			break;
		default:
			break;
	}
	StopPatternCallerGroup((MMI_LEDLIGHT_STATUS_ENUM)cm_p->alert_info.BacklightPatternId);
}

#ifdef __MMI_INTELLIGENT_CALL_ALERT__
U16 intelligent_call_alert_ring_tone;
U8 is_intelligent_call_alert_playing_text = 0;
void PlayIncomingCallTextHandler( mdi_result result )
{
	is_intelligent_call_alert_playing_text = 0;
	if( result == MDI_AUDIO_END_OF_FILE )
		AudioPlayToneWithCallBack(intelligent_call_alert_ring_tone,GetRingTypeEnum(), INCOMING_CALL_TONE);
}
#endif


/**************************************************************

	FUNCTION NAME		: StartRingTone

  	PURPOSE				: This function statrs the ringtone

	INPUT PARAMETERS	: ringtone index

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
extern S32 volume_level_UI;

void StartRingTone (U16 ringTone)
{
#if 1   // zrx del 20060612

	if( ringTone == TONE_CALL_WAITING )
		AudioPlayToneWithCallBack( ringTone, GetRingTypeEnum(), INCOMING_CALL_TONE );
	else if ( !IsSilentModeActivated() )
	{
#ifdef __MMI_INTELLIGENT_CALL_ALERT__
		U16 text[(MAX_NAME+MAX_CM_NUMBER)/2+1];
		U16 count;
		S32 text_len;

		MMI_INTELLIGENT_CALL_ALERT_TYPE isIntelligentCallAlertOn = GetIntelligentCallAert();

		if( isIntelligentCallAlertOn == MMI_INTELLIGENT_CALL_ALERT_ON )
		{
			for (count=0;count<MAX_CALLS;count++)
			{
				if (CM_INCOMING_STATE==cm_p->state_info.AllCalls[count].curr_state)
				{
					text[0] = 0;
#ifdef DIGIT_TONE_SUPPORT
					pfnUnicodeStrcpy( (S8*)text, (S8*)cm_p->state_info.AllCalls[count].number );
#endif
#if defined( SIMPLE_TTS ) || defined( DIGIT_TONE_SUPPORT )
					pfnUnicodeStrcat( (S8*)text, (S8*)cm_p->state_info.AllCalls[count].pBname );
#endif
					text[(MAX_NAME+MAX_CM_NUMBER)/2] = 0;
					break;
				}
			}

			text_len = pfnUnicodeStrlen((S8*)text);
			if( text_len > 0 && mdi_audio_play_text_with_vol_path((U8*)text, pfnUnicodeStrlen((S8*)text), 100, NULL, PlayIncomingCallTextHandler,
				volume_level_UI, MDI_DEVICE_SPEAKER_BOTH ) == MDI_AUDIO_SUCCESS )
			{
				intelligent_call_alert_ring_tone = ringTone;
				is_intelligent_call_alert_playing_text = 1;
			}
			else
				AudioPlayToneWithCallBack(ringTone,GetRingTypeEnum(), INCOMING_CALL_TONE);
		}
		else
#endif
		AudioPlayToneWithCallBack(ringTone,GetRingTypeEnum(), INCOMING_CALL_TONE);
		//AudioPlayReq(ringTone,GetRingTypeEnum());
	}
	#endif
}

/**************************************************************

	FUNCTION NAME		: StopRingTone

  	PURPOSE				: This function stops the ringtone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
void StopRingTone(U16 ringTone)
{
	if (ringTone < MAX_TONE_ID)
	{
#ifdef __MMI_INTELLIGENT_CALL_ALERT__
		if( is_intelligent_call_alert_playing_text )
			mdi_audio_stop_all();
		else
#endif
		AudioStopReq(ringTone);
	}
	else
		mdi_audio_stop_all();
}

/**************************************************************

	FUNCTION NAME		: VibratorThenRing(void)

  	PURPOSE				: This function is called for Vibrator Then Ring indication

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void VibratorThenRing(void)
{	
	//added by Laurence on 20061230
	TRACE_AUD_FUNCTION();
	VibratorOn();  
	StartTimer(CM_ALERT_NOTIFYDURATION_TIMER, VIB_THEN_RING_DURATION, StopVibratorPlayRing);  // zrx del 20060612
}

/**************************************************************

	FUNCTION NAME		: StopVibratorPlayRing(void)

  	PURPOSE				: This function is called for StopVibrator PlayRing

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
#ifdef __MMI_INCOMING_CALL_VIDEO__
	extern void EnableCategory17Audio(void);
#endif      	
void StopVibratorPlayRing(void)
{
	TRACE_AUD_FUNCTION();
	StopTimer(CM_ALERT_NOTIFYDURATION_TIMER);
	 

	VibratorOff();   
	//StartRingTone (cm_p->alert_info.RingToneId);

#ifdef __MMI_INCOMING_CALL_VIDEO__
         if (cm_p->alert_info.RingToneId == CM_RINGTONE_VIDEO)
         {
            /* turn on audio path */
            EnableCategory17Audio();
   	SetRingAfterVibFlag (TRUE);
         }
#endif
	CMPlayMTRingTone(cm_p->alert_info.RingToneId);
}

/**************************************************************

	FUNCTION NAME		: PlayConnectNotice(void)

  	PURPOSE				: This function is wrapper to Play Ring Tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
#ifdef __MMI_CONNECT_NOTICE__
void PlayConnectNotice(void)
{
	switch (GetConnectNoticeType())
	{
   	case MMI_NOTICE_NONE:
   		break;
   	case MMI_NOTICE_TONE_ONLY:
   	   playRequestedTone(CONNECT_TONE);
   		break;
   	case MMI_NOTICE_VIB_ONLY:
   		PlayVibratorOnce();
   		break;
   	case MMI_NOTICE_TONE_AND_VIB:
   	   playRequestedTone(CONNECT_TONE);
   		PlayVibratorOnce();
   		break;
   }
}
#endif

/**************************************************************

	FUNCTION NAME		: GetRingAfterVibFlag

  	PURPOSE				: This function returns state of alerts

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: TRUE or FALSE

	RETURNS				: pBOOL

 
**************************************************************/
pBOOL GetRingAfterVibFlag(void)
{
	return cm_p->alert_info.IsRingAfterVib;
}

/**************************************************************

	FUNCTION NAME		: SetRingAfterVibFlag

  	PURPOSE				: This function checks to see if need to ring again

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: TRUE or FALSE

	RETURNS				: pBOOL

 
**************************************************************/
void SetRingAfterVibFlag(pBOOL truth)
{
	cm_p->alert_info.IsRingAfterVib = truth;
}

/**************************************************************

	FUNCTION NAME		: GetAlertingFlag

  	PURPOSE				: This function returns state of alerts

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: TRUE or FALSE

	RETURNS				: pBOOL

 
**************************************************************/
pBOOL GetAlertingFlag(void)
{
	return cm_p->alert_info.IsAlerting;
}

/**************************************************************

	FUNCTION NAME		: SetAlertingFlag

  	PURPOSE				: This function checks to see if remote is alerting

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: TRUE or FALSE

	RETURNS				: pBOOL

 
**************************************************************/
void SetAlertingFlag(pBOOL truth)
{
	cm_p->alert_info.IsAlerting = truth;
}

/**************************************************************

	FUNCTION NAME		: SetRingingFlag

  	PURPOSE				: This function checks to see if remote is alerting

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: TRUE or FALSE

	RETURNS				: pBOOL

 
**************************************************************/
void SetRingingFlag(pBOOL truth)
{
	PRINT_INFORMATION(("SetRingingFlag: %d\n", truth));
	
	cm_p->alert_info.IsRinging = truth;
}

/**************************************************************

	FUNCTION NAME		: GetRingingFlag

  	PURPOSE				: This function returns state of internal alerts indication

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: TRUE or FALSE

	RETURNS				: pBOOL

 
**************************************************************/
pBOOL GetRingingFlag(void)
{
	return cm_p->alert_info.IsRinging;
}

/**************************************************************

	FUNCTION NAME		: GetInternalAlert

  	PURPOSE				: This function returns state of internal alerts indication

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: TRUE or FALSE

	RETURNS				: pBOOL

 
**************************************************************/
pBOOL GetInternalAlert(void)
{
	return cm_p->alert_info.IsInternalAlert;
}

/**************************************************************

	FUNCTION NAME		: SetInternalAlert

  	PURPOSE				: This function checks to see if USSD is supported in the phone/network

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: TRUE or FALSE

	RETURNS				: pBOOL

 
**************************************************************/
void SetInternalAlert(pBOOL truth)
{
	cm_p->alert_info.IsInternalAlert = truth;
}

/**************************************************************

	FUNCTION NAME		: PlayInternalOutgoingTone

  	PURPOSE				: Stops playing waiting call tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
void PlayInternalOutgoingTone(void)
{
	SetInternalAlert(TRUE);
	StartRingTone (TONE_RINGING_CALL_GSM);
}

/**************************************************************

	FUNCTION NAME		: StopInternalOutgoingTone

  	PURPOSE				: Stops playing waiting call tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
void StopInternalOutgoingTone(void)
{
	SetInternalAlert(FALSE);
	AudioStopReq (TONE_RINGING_CALL_GSM);
}

/**************************************************************

	FUNCTION NAME		: PsCBackPlayInternalTone(void*)

  	PURPOSE				: This function plays GSMtone

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackPlayInternalTone(void* info)
{
	U8 flag=DeriveInternalAlrtIndVal(info);

	SetAlertingFlag (TRUE);
	if (GetExitScrnID() == SCR1001_CM_OUTGOINGCALLSCREEN)
		SetGroupKeyHandler(EnterScreenAndHandleDigit,(PU16)CMIdleScreenDigits,
								MAX_CM_SCREEN_DIGITS-1,KEY_EVENT_DOWN);
		
	if(flag)
	{
		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_PlayInternalTone));
//		PlayInternalOutgoingTone();
	}
	SetSpeechVolume(GetModeSpeechVolume());
}

/**************************************************************

	FUNCTION NAME		: SetDTMFFlag

  	PURPOSE				: Sets the DTMF Flag

	INPUT PARAMETERS	: TRUE for set, FALSE for not set

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void SetDTMFFlag(pBOOL dtmfFlag)
{
	cm_p->alert_info.DTMF = dtmfFlag;
}

/**************************************************************

	FUNCTION NAME		: GetDTMFFlag

  	PURPOSE				: Gets the DTMF Flag Status

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: TRUE for set, FALSE for not set

	RETURNS				: nil

 

**************************************************************/
pBOOL GetDTMFFlag(void)
{
	return cm_p->alert_info.DTMF;
}

/**************************************************************

	FUNCTION NAME		: DTMF

  	PURPOSE				: This function enables or Disables the DTMF

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil
**************************************************************/
void DTMFFunction (void)
{
	if(GetDTMFFlag()==TRUE)
	{
		SetDTMFFlag(FALSE);
		ChangeLeftSoftkey(STR_GLOBAL_OFF,IMG_SCR1002_OFF_LSK);
	}else
	{
		SetDTMFFlag(TRUE);
		ChangeLeftSoftkey(STR_GLOBAL_ON,IMG_SCR1002_ON_LSK);
	}
	Hint2024DTMF(GetCurrHiliteID());
	RedrawCategoryFunction();
	HiliteM2024DTMF();
}

/**************************************************************

	FUNCTION NAME		: SetHFreeHeld

  	PURPOSE				: Sets the HFreeHeld Flag

	INPUT PARAMETERS	: Flag value to set

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void SetHFreeHeld(BOOL HFreeHeldFlag)
{
	/* TRUE: current LSpk, show HHeld as RSK,
		FALSE: current no LSpk, show HFree as RSK */
	mmi_trace(g_sw_CC," Func: %s HFreeHeldFlag=%d", __FUNCTION__,HFreeHeldFlag);
	cm_p->alert_info.IsHFree = HFreeHeldFlag;
}

/**************************************************************

	FUNCTION NAME		: GetHFreeHeld

  	PURPOSE				: Gets the HFreeHeld Flag

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: Flag Value

	RETURNS				: nil

 

**************************************************************/
U16 GetHFreeHeld(void)
{
	return cm_p->alert_info.IsHFree;
}

/**************************************************************

	FUNCTION NAME		: LoudSpeakerFunction

  	PURPOSE				: This function enables the Loud Speaker

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil
**************************************************************/
void LoudSpeakerFunction (void)
{

	if (GetHFreeHeld()==FALSE)
	{
		SetHFreeHeld(TRUE);
		if (GetExitScrnID() == SCR_CM_ACTIVECALLSCREEN)
		{
		   /* in active screen */
		   ChangeRightSoftkey(STR_SCR1002_LOUDSP_H_HELD_RSK,
		                      IMG_SCR1002_LOUDSP_RSK);
		}

		EnableHandsFree();
		SetLoudSpeaker(TRUE);
	}
	else
	{
		SetHFreeHeld(FALSE);
		if (GetExitScrnID() == SCR_CM_ACTIVECALLSCREEN)
		{
		   /* in active screen */
		   ChangeRightSoftkey(STR_SCR1002_LOUDSP_H_FREE_RSK,
		                      IMG_SCR1002_LOUDSP_RSK);
	   }

		DisbleHandsFree();
		SetLoudSpeaker(FALSE);
	}

	EntryScrLoudSpNotifyScreen();

}

/**************************************************************

	FUNCTION NAME		: OptionMuteorUnMutetheCall(void)

  	PURPOSE				: This handles the Mute functionality in Options

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void OptionMuteorUnMutetheCall(void)
{
	if(IsPhoneInMute())
	{
		UnMutethePhone();
		ChangeLeftSoftkey(STR_GLOBAL_ON,IMG_SCR1002_ON_LSK);
	}else
	{
		MutethePhone();
		ChangeLeftSoftkey(STR_GLOBAL_OFF,IMG_SCR1002_OFF_LSK);
	}
	Hint2020Mute(GetCurrHiliteID());
	RedrawCategoryFunction();
	HiliteM2020Mute();
}

/**************************************************************

	FUNCTION NAME		: IsSimControlProcedure

  	PURPOSE				: This function is a stub for checking sim control proc.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
pBOOL IsSimControlProcedure(PU8 buf)
{
	return FALSE;
}

/**************************************************************

	FUNCTION NAME		: IsManufactuerSpecific

  	PURPOSE				: This function is a stub for checking manufactuer specific string

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
pBOOL IsManufactuerSpecific(PU8 buf)
{
	return FALSE;
}

/**************************************************************

	FUNCTION NAME		: SetSideVolumeKeysForCM(void)

  	PURPOSE				: Sets Key Handlers for Volume Key in CM

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetSideVolumeKeysForCM(void)
{
#if (defined(__NO_SIDE_VOLUME_KEY_56__))||(defined(__NO_SIDE_VOLUME_KEY_46__)||(defined(__NO_SIDE_VOLUME_KEY_STAR_POUND__)))	//niej begin

	if(SCR_CM_ACTIVECALLSCREEN == currTopScrnID)//niej
	{
		SetKeyHandler(DummyAforSideKeyHandling,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(DummyBforSideKeyHandling,KEY_RIGHT_ARROW,KEY_LONG_PRESS);
	#ifdef __MMI_IN_CALL_CAMERA_KEY_START_SOUND_RECORD__
		SetKeyHandler(DummyBforSideKeyHandling,KEY_CAMERA,KEY_EVENT_UP);
	#endif

		SetKeyHandler(MMI_dummy_function,KEY_RIGHT_ARROW,KEY_REPEAT);

		SetKeyHandler(DummyCforSideKeyHandling,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(DummyDforSideKeyHandling,KEY_LEFT_ARROW,KEY_LONG_PRESS);
		SetKeyHandler(MMI_dummy_function,KEY_LEFT_ARROW,KEY_REPEAT);
	}
#else
	SetKeyHandler(DummyAforSideKeyHandling,KEY_VOL_UP,KEY_EVENT_DOWN);
	SetKeyHandler(DummyBforSideKeyHandling,KEY_VOL_UP,KEY_LONG_PRESS);
#ifdef __MMI_IN_CALL_CAMERA_KEY_START_SOUND_RECORD__
	SetKeyHandler(DummyBforSideKeyHandling,KEY_CAMERA,KEY_EVENT_UP);
#endif

	SetKeyHandler(MMI_dummy_function,KEY_VOL_UP,KEY_REPEAT);

	SetKeyHandler(DummyCforSideKeyHandling,KEY_VOL_DOWN,KEY_EVENT_DOWN);
	SetKeyHandler(DummyDforSideKeyHandling,KEY_VOL_DOWN,KEY_LONG_PRESS);
	SetKeyHandler(MMI_dummy_function,KEY_VOL_DOWN,KEY_REPEAT);
#endif
//niej end
}

/**************************************************************

	FUNCTION NAME		: DummyAforSideKeyHandling

  	PURPOSE				: Sets Key Handler for Volume Up Key in CM

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void DummyAforSideKeyHandling(void)
{
#if (defined(__NO_SIDE_VOLUME_KEY_56__))||(defined(__NO_SIDE_VOLUME_KEY_46__)||(defined(__NO_SIDE_VOLUME_KEY_STAR_POUND__))	)//niej begin
	//niej begin
	SetKeyHandler(SetKeyPadVolUp,KEY_RIGHT_ARROW,KEY_EVENT_UP);
	#else
	SetKeyHandler(SetKeyPadVolUp,KEY_VOL_UP,KEY_EVENT_UP);
	#endif//niej End
}

/**************************************************************

	FUNCTION NAME		: DummyBforSideKeyHandling

  	PURPOSE				: Sets Key Handler for Volume Up Key in CM

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DummyBforSideKeyHandling(void)
{

#if (defined(__NO_SIDE_VOLUME_KEY_56__))||(defined(__NO_SIDE_VOLUME_KEY_46__)||(defined(__NO_SIDE_VOLUME_KEY_STAR_POUND__)))	//niej begin
//niej begin

	ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_UP);
#ifdef __MMI_IN_CALL_CAMERA_KEY_START_SOUND_RECORD__
	ClearKeyHandler(KEY_RIGHT_ARROW,KEY_LONG_PRESS);
	ClearKeyHandler(KEY_CAMERA,KEY_EVENT_UP);
#endif
	#else
	ClearKeyHandler(KEY_VOL_UP,KEY_EVENT_UP);
#ifdef __MMI_IN_CALL_CAMERA_KEY_START_SOUND_RECORD__
	ClearKeyHandler(KEY_VOL_UP,KEY_LONG_PRESS);
	ClearKeyHandler(KEY_CAMERA,KEY_EVENT_UP);
#endif
	#endif//niej end
	SetKeyPadStartVoiceRecordLongPress();
}

/**************************************************************

	FUNCTION NAME		: DummyCforSideKeyHandling

  	PURPOSE				: Sets Key Handler for Volume Down Key in CM

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DummyCforSideKeyHandling(void)
{
#if (defined(__NO_SIDE_VOLUME_KEY_56__))||(defined(__NO_SIDE_VOLUME_KEY_46__)||(defined(__NO_SIDE_VOLUME_KEY_STAR_POUND__)))	//niej begin
	//niej begin

	SetKeyHandler(SetKeyPadVolDown,KEY_LEFT_ARROW,KEY_EVENT_UP);
	#else
	SetKeyHandler(SetKeyPadVolDown,KEY_VOL_DOWN,KEY_EVENT_UP);
	#endif		//niej End
	
}

/**************************************************************

	FUNCTION NAME		: DummyDforSideKeyHandling

  	PURPOSE				: Sets Key Handler for Volume Down Key in CM

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DummyDforSideKeyHandling(void)
{
	//niej Begin
#if (defined(__NO_SIDE_VOLUME_KEY_56__))||(defined(__NO_SIDE_VOLUME_KEY_46__)||(defined(__NO_SIDE_VOLUME_KEY_STAR_POUND__)))	//niej begin
	//niej begin

	ClearKeyHandler(KEY_LEFT_ARROW,KEY_EVENT_UP);
	#else
	ClearKeyHandler(KEY_VOL_DOWN,KEY_EVENT_UP);
	#endif
	//niej End
	SetKeyPadStopVoiceRecordLongPress();
}

/**************************************************************

	FUNCTION NAME		: SetKeyPadStartVoiceRecordLongPress

  	PURPOSE				:  To stop voice memo recording

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetKeyPadStartVoiceRecordLongPress(void)
{
	if( GetTotalActiveCallCount() > 0 )
	{
#ifdef __MMI_SOUND_RECORDER__
 
	mmi_sndrec_entry_from_call();
 
#else
	MakePsStartVoiceRecord();
#endif
        }
}

/**************************************************************

	FUNCTION NAME		: SetKeyPadStopVoiceRecordLongPress

  	PURPOSE				:  To stop voice memo recording

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetKeyPadStopVoiceRecordLongPress(void)
{
	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_StopRecord));

	SetVoiceRecordingFlag(FALSE);
	HideStatusIcon(STATUS_ICON_VOICE_RECORD);
	UpdateStatusIcons();
#ifdef __MMI_SOUND_RECORDER__
 
//	mdi_audio_stop_record();
 
#else
	MakePsStopVoiceRecord();
#endif
}

