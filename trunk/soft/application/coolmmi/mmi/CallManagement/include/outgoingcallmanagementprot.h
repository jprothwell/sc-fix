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
 *  OutgoingCallManagementProt.h
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
 
/**************************************************************

	FILENAME	: OutgoingCallManagementProt.h

  	PURPOSE		: Call Management application

 

	AUTHOR		: Subramanya BG 

	DATE		: Nov 16,02

**************************************************************/
#ifndef _PIXCOM_OUTGOINGCALLMANAGEMENT_PROT_H
#define _PIXCOM_OUTGOINGCALLMANAGEMENT_PROT_H

//#include "mmi_data_types.h"
//#include "callmanagementstruct.h"

void ReinitializeOutgoingCall(void);
void InitOutGoingCall(void);

void HiliteM2011EndSingle(void);
void HiliteM2011IncEndSingle(void);
void HiliteM2011SingleActiveEnd(void);
void HiliteM2019AllActiveEnd(void);
void HiliteM2020AllHeldEnd(void);
void HiliteM2016AllCallEnd(void);
void HiliteIncomingReject(void);
void HiliteM2010ActiveHold(void);
void HiliteM2012HeldRetrieve(void);
void HiliteM2014Swap(void);
void HiliteM2015Conference(void);
void HiliteM2017Transfer(void);
void HiliteM2018Split(void);
void HiliteM2031PBookAppEntry(void);
void HiliteM2032MsgAppEntry(void);
#ifdef __MMI_VOICE_MEMO__
void HiliteM2022VoiceMemo(void);
#endif
#ifdef __MMI_SOUND_RECORDER__
void HiliteM2026SoundRecorder(void);
#endif
#ifdef __MMI_AUDIO_PLAYER__
void HiliteM2025AudioPlayer(void);
#endif


#if  defined (__MMI_BT_SUPPORT__)

void HiliteBTCMSetPath(void);
void ProcessBTSetVoicePath(void);
/* Configure Default Headset */
extern void HiliteBTCMConnectLastBTHeadset(void);
extern void ProcessBTCMConnectLastBTHeadset(void);
extern void HintBTConnectLastBTHeadset(U16 index);
#endif

void HiliteM2020Mute(void);
void HiliteM2024DTMF(void);
void HiliteM2023LoudSp(void);
void HiliteM2021NewCall(void);

void Hint2020Mute(U16 index);
void Hint2024DTMF(U16 index);

void OutgoingProcessCMEvent(CM_EVENT_TYPES inEvenType,void* MsgStruct);
void ProcessShowOutgoingCallScreen(void);
ACTION_RESULT ProcessKBOutgoingEvent(void* MsgStruct);
ACTION_RESULT ProcessPSCallconnectedEvent(void* CallIndex);
ACTION_RESULT ProcessKBHangupReqEvent(void* CallIndex);
ACTION_RESULT ProcessPSHangupSucEvent(void* CallIndex);
ACTION_RESULT ProcessKBHangupallReqEvent(void* MsgStruct);
ACTION_RESULT ProcessPSHangupallSucEvent(void* MsgStruct);
ACTION_RESULT ProcessKBHangupallActReqEvent(void* MsgStruct);
ACTION_RESULT ProcessPSHangupallActSucEvent(void* MsgStruct);
ACTION_RESULT ProcessKBHangupallHldReqEvent(void* MsgStruct);
ACTION_RESULT ProcessPSHangupallHldSucEvent(void* MsgStruct);
ACTION_RESULT ProcessKBHoldReqEvent(void* MsgStruct);
ACTION_RESULT ProcessPSHoldSucEvent(void* MsgStruct);
ACTION_RESULT ProcessKBRetrieveReqEvent(void* MsgStruct);
ACTION_RESULT ProcessPSRetrieveSucEvent(void* MsgStruct);
ACTION_RESULT ProcessKBSwapReqEvent(void* MsgStruct);
ACTION_RESULT ProcessPSSwapSucEvent(void* MsgStruct);
ACTION_RESULT ProcessKBConfReqEvent(void* MsgStruct);
ACTION_RESULT ProcessPSConfSucEvent(void* MsgStruct);
ACTION_RESULT ProcessKBSplitReqEvent(void* MsgStruct);
ACTION_RESULT ProcessPSSplitSucEvent(void* MsgStruct);
ACTION_RESULT ProcessKBEctReqEvent(void* MsgStruct);
ACTION_RESULT ProcessPSEctSuccessful(void* MsgStruct);
void PsCBackCallDeflected(void* info);

void EndSingleAndGoBack(void);
void EndSingleActiveAndGoBack(void);
void SplitAndGoBack(void);
void SetCurrHiliteCall(S32 hiliteIndex);
void HangupHilited(void);
void SplitHilited(void);

void EntryScr1009CallListScreen(void);
void EntryScr1001OutgoingCall(void);
void ExitScr1001OutgoingCall(void);
void EntrySubLcdScr1001OutgoingCall(void);
void ExitSubLcdScr1001OutgoingCall(void);
void EntryScrOutgoingMultipleOptions(void);
void EntryScr1002ActiveCall(void);
void DummyScr1002ActiveCall(MYTIME *t);
void EntrySubLcdScr1002ActiveCall(void);
void ExitSubLcdScr1002ActiveCall(void);
void EntryScr1003CMActiveCallOptions(void);
void ExitScr1003CMActiveCallOptions(void);
void EntryScr1004NotifyEndCallDuration(void);
void ExitScr1004NotifyEndCallDuration(void);
void EntryScrNotifyCallAborted(void);
void EntryScr1005NotifyHoldSucess(void);
void EntryScr1006NotifyRetrieveSucess(void);
void EntryScr1007NotifySwapSucess(void);
void EntryScr1008NotifyConfSucess(void);
void EntryScr1010NotifySplitSucess(void);
void EntryScrNotifyCHLDSucess(U16 notify_string, pBOOL play_tone);
void ExitScrNotifyCMPopUpTone(void);
void EntryScrLoudSpNotifyScreen(void);
void ExitScrNotifyCMPopUp(void);

/************************** CISS *****************************/
void EntryScrUSSDReq(void);
void SendUSSDRequest(void);
void AbortUSSDRequest(void);
void AbortUSSDRequestEND(void);
void ProcessUSSDrecieved(void* info);

//micha0915
U8 SSReqHistoryDelHdlr( void* in_param );

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
#endif
#endif //_PIXCOM_OUTGOINGCALLMANAGEMENT_PROT_H


