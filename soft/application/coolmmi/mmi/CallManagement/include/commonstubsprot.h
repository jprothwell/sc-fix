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
 *  CommonStubsProt.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef _PIXCOM_COMMON_STUB_H
#define _PIXCOM_COMMON_STUB_H

#ifdef __MMI_INCOMING_CALL_VIDEO__
#define CM_RINGTONE_VIDEO 0xffff
#endif

typedef struct {
	S8 		RedialNum[MAX_NUMBER];
	U8 		MaxAttempts;
	U8 		CurrentAttempt;
	BOOL 		ErrRedialScreen;				/* guard cm error pop up timeout or not */
	BOOL		RedialTimer;		/* guard redial timer expire or not  */
	U8  DialFromPhb;
} cm_redial_struct;

typedef struct {	
	U16 		RingToneId;
	U16 		BacklightPatternId;
	U16		QuickEndTime;
	MYTIME     LastCTRTime;
	U8			AlertType;
	BOOL		DTMF;							/* default ON */
	BOOL		IsSilent;
	BOOL		IsMute;
	BOOL		IsLoudSpk;
	BOOL		IsHFree;
	BOOL		IsInternalAlert;
	BOOL		IsAlerting;
	BOOL		IsRinging;
	BOOL		IsRecord;
	BOOL        IsCTRStart;
	BOOL		IsRingAfterVib;
} cm_alert_struct;

typedef struct {
	FuncPtr	CbackAfterNotifySS;
	BOOL		IsPopUp;
	BOOL		IsNotifySS;
	BOOL		IsSave;
} cm_history_struct;

typedef struct {
	cm_state_struct	state_info;
	cm_redial_struct 	redial_info;
	cm_alert_struct	alert_info;
	cm_history_struct	history_info;
} cm_context_struct;


#ifdef __MMI_MULTI_SIM__
#define MMI_CC_DEST_MOD   (MOD_L4C + MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL))
#else  /* No Defined __MMI_MULTI_SIM__ */
#define MMI_CC_DEST_MOD   MOD_L4C
#endif /* __MMI_MULTI_SIM__ */

extern cm_context_struct *cm_p;

void MutethePhone(void);
void UnMutethePhone(void);
void OptionMuteorUnMutetheCall(void);

pBOOL IsPhoneInSilent(void);
void UnSilencethePhone(void);

pBOOL GetLSpkNowOff(void);

void SetVoiceRecordingFlag(pBOOL flag);
pBOOL GetVoiceRecordingFlag(void);

void CMSetupAlertForMT(void);
   
void ShowIncomingCallIndication(void);
void StopIncomingCallIndication(void);
void StartRingtoneOrVibrator(void);
void StopRingtoneOrVibrator(void);

void StartRingTone (U16 ringTone);
void StopRingTone(U16 ringTone);
void VibratorThenRing(void);
void StopVibratorPlayRing(void);

void PlayConnectNotice(void);

pBOOL GetRingAfterVibFlag(void);
void SetRingAfterVibFlag(pBOOL truth);
pBOOL GetRingingFlag(void);
void SetRingingFlag(pBOOL truth);
pBOOL GetAlertingFlag(void);
void SetAlertingFlag(pBOOL truth);
pBOOL GetInternalAlert(void);
void SetInternalAlert(pBOOL truth);
void PlayInternalOutgoingTone(void);
void StopInternalOutgoingTone(void);
void PsCBackPlayInternalTone(void* info);

void SetDTMFFlag(pBOOL dtmfFlag);
void DTMFFunction (void);
U16 GetHFreeHeld(void);
void SetHFreeHeld(BOOL HFreeHeldFlag);
void LoudSpeakerFunction (void);

void SetSideVolumeKeysForCM(void);
void SetKeyPadStartVoiceRecordLongPress(void);
void SetKeyPadStopVoiceRecordLongPress(void);
void DummyAforSideKeyHandling(void);
void DummyBforSideKeyHandling(void);
void DummyCforSideKeyHandling(void);
void DummyDforSideKeyHandling(void);

pBOOL IsSimControlProcedure(PU8 buf);
pBOOL IsManufactuerSpecific(PU8 buf);

#endif //_PIXCOM_COMMON_STUB_H


