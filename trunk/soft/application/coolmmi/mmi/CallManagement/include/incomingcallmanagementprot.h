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
 *  IncomingCallManagementProt.h
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: IncomingCallManagementProt.h

  	PURPOSE		: Call Management application

 

 

	DATE		: Nov 8,02

**************************************************************/

#ifndef _INCOMINGCALLMANAGEMENT_PROT_H
#define _INCOMINGCALLMANAGEMENT_PROT_H


void ReinitalizeIncomingCall (void);
void InitIncomingCall (void);

void HiliteMenuIncomingAnswer(void);
void HiliteMenuIncomingCallDeflection(void);
void HiliteMenuCCBSDial(void);
void HiliteMenuIncomingEndActive(void);
void HiliteMenuEndAllCalls(void);
void HiliteCallToEnd(S32 itemIdx);

void EntryScrIncomingCallEvent(void);
void ExitScrIncomingCallEvent(void);
void EntrySubLcdScrIncomingCallEvent(void);
void ExitSubLcdScrIncomingCallEvent(void);
void EntrySubLcdScrStaticIncomingCallEvent(void);
void ExitSubLcdScrStaticIncomingCallEvent(void);
void EntryScrIncomingMultipleOptions(void);
void EntryScrIncomingOptions(void);
void EntryScrCcbsActivate(void);
void ExitScrCcbsActivate(void);
void EntryScrNotifyCCBSSuccess(void);
void EntryScrCCBSInvokeEvent(void);
void EntryScrCCBSInvokeMultipleOptions(void);
void EntryScrShowSSEvent(void);
void ExitScrShowSSEvent(void);
void EntryScrShowActiveCallToEnd(void);
void HandleCallDelfectDigitEntry(void);
void EntryCallDeflectDigitHandler(void);
void ExitCallDeflectDigitHandler(void);
void EntryScrConfirmDTMFString(void);
void ExitScrConfirmDTMFString(void);
void ExitScrShowNumberChangeNotification(void);

void PsCBackCallIncoming (void* info);
void PsCBackCallWait (void* info);
void PsCBackNetworkCallDropped(void* info);
void PsCBackCcbsActivate (void* info);
void PsCBackActivatedCCBS(void* info);
void PsCBackCCBSInvoked (void* info);
void PsCBackNotifySSInd(void* info);
void PsCBackSpeechIndication(void* info);
void PsCBackAocIndication(void* info);
void PsCBackConfirmDTMFString(void* info);

void KbCBackCallIncomingAccepted (void);
void KbCBackCallIncomingRejected (void);
#ifdef __MMI_SIDEKEY_SUPPRESS_RING__
void KbCBackCallIncomingRejectedNoTone (void);
void KbCBackCallIncomingSuppressTone (void);
#endif
void KbCBackEndActiveAcceptIncomingCall (void);
void KbCBackEndSelectiveActiveCall(void);
void KbCBackEndAllCallsExceptIncoming(void);
void KbCBackCCBSDialCall (void);
void KbCBackCCBSInvokeRejected (void);
void IgnoreCCBS(void);
void ActivateCCBS(void);

void ProcessIncomingEvents (CM_EVENT_TYPES event, void* info);
ACTION_RESULT ProcessIncomingCallEvent (void* info);
ACTION_RESULT ProcessCallWait(void* info);
ACTION_RESULT ProcessInvokedCcbs (void* info);
ACTION_RESULT ProcessIncomingCallRejected(void* info);
ACTION_RESULT ProcessEndCallIdAndGoBack(void* info);
void ProcessShowSSEventNotification(void* info);
ACTION_RESULT ProcessAocIndication(void* info);
void ReverseSpeechIndication(void* info);
void ProcessStateCheckIncomingCall(void);

void PsCBackIncomingCallAccepted(void* info);
void PsCBackCheckChldIncomingCallAccepted(void* info);
void PsCBackActiveCallsHeld(void* info);
ACTION_RESULT ProcessCallsHeld(void* info);
void PsCBackIncomingCallRejected (void* info);
ACTION_RESULT ProcessAcceptIncomingCall(void* info);
void PsCBackselectedCallEnded(void* info);
void PsCBackActiveCallEndedHeldRetrieved(void *info);
ACTION_RESULT ProcessEndAllActiveCall(void* info);
void CBackHangupAllActive(void);
void CBackHangupAllActiveAcceptOther(void* info);
ACTION_RESULT ProcessHold2Active(void* info);
ACTION_RESULT ProcessCCBSDialRequest(void* info);
ACTION_RESULT ProcessCCBSRejectRequest(void* info);
void PsCBackCCBSRejectResponse(void* info);
void CCBSDialResponse (void* info);

void CallDeflectScreenEmpty (void);
void CallDeflectScreenNotEmpty (void);
void CallDeflectDigitCall(void);
void CheckBeforeGoBackHistory(void);

void ConfirmDTMFString(void);
void IgnoreDTMFStringAndGoBackHistory(void);

void SetKeysForPOPUp(void);
void POPUPHandler(void);

void InvisibleDigitHandler(void);
void AuxInvisibleDigitHandler(void);

/**************************** CISS ********************************/

void PsCBackUSSNInd(void* info);
void PsCBackUSSRInd(void* info);
void PsCBackUSSDInd(void* info);

void EntryScrDisplayUSSNMsg(void);
//micha0317
void GoBackFromUSSNMsg(void);
void EntryScrDisplayUSSRMsg(void);
void ExitScrDisplayUSSRMsg(void);
void EntryScrDisplayUSSDRes(void);
void EntryScrDisplayUSSDOkEnd(void);
void EntryScrDisplayUSSDEnd(void);
void EntryScrEditUSSRMsg(void);

void SendUSSRMsg(void);
void AbortUSSDResultRequest(void);
void AbortUSSDResultRequestEND(void);
//micha0610
void ResumeSSScrInCall(void);

//micha0110
U8 USSRDelHistoryHdlr( void* in_param );
void EditUSSRGoBack(void);

void CallFactoryAutoDailInd();
#endif

