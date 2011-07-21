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
 *  CallStructureManagementProt.h
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
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: CallStructureManagementProt.h

  	PURPOSE		: Call Strucutre Management API

 

	AUTHOR		: Ashish

	DATE		: Nov 8,02

**************************************************************/
#ifndef CALLSTRUCTUREMANAGEMENT_PROT_H
#define CALLSTRUCTUREMANAGEMENT_PROT_H

typedef struct {
	CALL_INFO		AllCalls[MAX_CALLS];
	CALL_INFO		CallStructureForCallLog;
	CALL_STATE 		CurrentState;
	CALL_STATE		PrevState;
	U8 				NumofActivecalls;
	U8					NumofHeldcalls;
	U8					TotalCallCount;
	U8					OldestCallIndex;
} cm_state_struct;

void ReinitalizeCallManagement(void);
void ResetGlobalValues(void);
void InitCallInfo(void);
void ResetCallInfo(CM_CALL_HANDLE nCallHandle,pBOOL bIsHandle);
void PurgeOutgoingCallsStructure(void);
void PurgeIncomingCallStructure(void);
void PurgeAllCalls(void);

void AddNewCallInfo(PU8 Number,CALL_STATE currentstate,
					CALL_STATE prevstate,CALL_DIR MOflag,
					CM_CALL_HANDLE currentCallHandle, U8 callType);

void SetIncomingCallStateActive (void);
void SetCallState(CM_CALL_HANDLE nCallHandle,CALL_STATE state,pBOOL bIsHandle);
 
void SetAllCallState(CALL_STATE state);
void SetAllExceptWaitingCallState(CALL_STATE state);
void SetAllActiveCallState(CALL_STATE state);
void SetAllHeldCallState(CALL_STATE state);
 
CALL_STATE GetCallState (CM_CALL_HANDLE handle);
void PsCbackSyncCallList(void *MsgStruct);
void UpdateStateMachine(void);

void SetHoldFlag(void);
void ResetHoldFlag(void);
void SetRetrieveFlag(void);
void ResetRetrieveFlag(void);
void SetSwapFlag(void);
void ResetSwapFlag(void);
void SetConfFlag(void);
void ResetConfFlag(void);
void ResetSplitFlag(void);
void SetAllHangupFlag(void);
void SetAllCallsHangupFlag(void);
void SetAllActHangupFlag(void);
void SetAllHldHangupFlag(void);
void ResetAllHangupFlag(void);
BOOL CheckAllCallflag(void);
U8 GetAllCallFlags (void);

CHLD_REQ_ACTION GetChldReqSent(void);
void SetChldReqSent(CHLD_REQ_ACTION action);

void MakeHold(void);
void MakeRetrieve(void);
void MakeSwap(void);
void MakeConf(void);
void MakeAllActHangup(void);
void MakeSplitSucess(void);

pBOOL AllowMoreCalls(void);
CALL_STATE GetPreviousState(void);
void SetCurrentState(CALL_STATE state);
void SetPreviousState(CALL_STATE state);
void SetCallflag(CM_CALL_HANDLE nCallHandle,U8 flag,pBOOL bIsHandle);
U8 GetCallflag(CM_CALL_HANDLE nCallHandle,pBOOL bIsHandle);
void ResetCallflag(CM_CALL_HANDLE nCallHandle,U8 flag,pBOOL bIsHandle);
U16 GetCallDirFlag(CM_CALL_HANDLE handle);

CM_CALL_HANDLE MapItemId2CallHandle(U16 idx);
S16 GetMMIStructIndexof(CM_CALL_HANDLE nCallIndex);
S16 GetFirstFreeIndex(void);
 
CM_CALL_HANDLE GetDisconnectingCallHandle(void);
 
CM_CALL_HANDLE GetIncomingCallHandle(void);
void SetOutgoingCallHandle (CM_CALL_HANDLE handle);
CM_CALL_HANDLE GetOutgoingCallHandle(void);
CALL_DIR GetOutgoingCallOrigin(void);
CM_CALL_HANDLE GetCallHandle(PU8 Dispname);
CM_CALL_HANDLE GetHungUpHandle(void);
S8 GetOutgoingCallIndex(void);
U8 GetAllDispNameorNum(PU8 *Dispnames,U16	*DispStrids,S16 *HighlightIndex);
U8 GetAllActDispNameorNum(PU8 *Dispnames);
U16 GetImgIdofState(CALL_STATE tmpState);

void SetCalledNumber(S16 index,U8* msg);
void SetCalledNumWithType(S16 index,void *msg);
void SetOutgoingNamefromPhonebook(void);
PU8 GetOutgoingNumber(void);
PU8 GetOutgoingName(void);
U8 GetOutgoingCallType(void);
PU8 GetCallName(CM_CALL_HANDLE handle);
PU8 GetCallNumber(CM_CALL_HANDLE handle);
U8 GetCallType(CM_CALL_HANDLE handle);
U16 GetCallImgId(CM_CALL_HANDLE handle);
#ifdef __MMI_NUMBER_LOCATION__

PU8 GetCallNameWithAddress(CM_CALL_HANDLE handle);
PU8 GetDispNameorNumWithAddress(void);
#endif
#ifdef __MMI_PHB_CALL_SHOW_PICTURE_FROM_FILE__
S8* GetCallImgPath(CM_CALL_HANDLE handle) ;
#endif
#ifdef __MMI_INCOMING_CALL_VIDEO__
S8* GetCallVideoPath(CM_CALL_HANDLE handle) ;
#endif
U8 GetActiveCallType(void);
PU8 GetDispNameorNum(void);
#ifdef __MMI_CM_BLACK_LIST__
pBOOL BlockedHandle(CM_CALL_HANDLE handle);
void SetClearKeyFlag(pBOOL value);
pBOOL GetClearKeyFlag(void);
#endif
void SetCCFlag(pBOOL value);
pBOOL GetCCFlag(void);
void SetTransitionToActiveFlag (pBOOL flag);
void SetAnswerMode (void *mode);
void *GetAnswerMode(void);
void SetAutoAnswerFlag(pBOOL flag);
pBOOL GetAutoAnswerFlag(void);
 
void SetAutoReleaseFlag(pBOOL flag);
pBOOL GetAutoReleaseFlag(void);
 
void SetAbortCallRequestedFlag(pBOOL flag);
pBOOL GetAbortCallRequestedFlag(void);
void SetCallAbortReqSentFlag (pBOOL flag);
pBOOL GetCallAbortReqSentFlag(void);
void SetDropRequestFlag(pBOOL flag);
pBOOL GetDropRequestFlag(void);
void SetIncomingCallDroppedFlag(pBOOL flag);
pBOOL GetIncomingCallDroppedFlag(void);
void SetCallWaitFlag(pBOOL flag);
pBOOL GetCallWaitFlag(void);
void SetNoTwiceRingFlag(pBOOL flag);
pBOOL GetNoTwiceRingFlag(void);
void SetWait2RingCallFlag(pBOOL flag);
pBOOL GetWait2RingCallFlag(void);
void SetCallEndedBeforeConnFlag(pBOOL flag);
pBOOL GetCallEndedBeforeConnFlag(void);
void SetErrorToneFlag(pBOOL flag);
pBOOL GetErrorToneFlag(void);
void SetShowAOC(pBOOL flag);
pBOOL GetShowAOC(void);
void SetDTMFKeyUpFlag(pBOOL flag);
pBOOL GetDTMFKeyUpFlag(void);
void SetCMScrnFlag(pBOOL flag);
pBOOL GetCMScrnFlag(void);

void UpdateActiveCallScrnIfPresent(void);
pBOOL EmergencyCallPresent(void);

void SetWapCallPresent(pBOOL flag);
void SetPreviousCallWAP(pBOOL flag);
pBOOL GetPreviousCallWAP (void);
U16 CMAllowInterrupt(void);

void SetCallEndCause (U16 cause);
U16 GetCallEndCause(void);
void SetTempUseIPNumber(pBOOL flag);
pBOOL GetTempUseIPNumber (void);
void SetCMPhoneBookStruct(PHB_CM_INTERFACE *pNumberStruct);
PHB_CM_INTERFACE *GetCMPhoneBookStruct(void);
void SetCallDeflectFlag(pBOOL flag);
pBOOL GetCallDeflectFlag(void);
void SetActivatedCcbsFlag(pBOOL ccbsFlag);
pBOOL GetActivatedCcbsFlag(void);
void SetCCBSFlag(pBOOL ccbsFlag);
pBOOL GetCCBSFlag(void);
void CCBSUpdateCallInfo(void);

void LogCall(void);
void LogCallWithStartTime(CALL_INFO *log);
void LogCallInfoForCallHistory(CM_CALL_HANDLE handle);
void UpdateCallStartTimeAndLogCall(CM_CALL_HANDLE handle,MYTIME *t);
void SetCallHandleForStartTimeUpdate(CM_CALL_HANDLE handle);
CM_CALL_HANDLE GetCallHandleForStartTimeUpdate(void);
void GetEndTimeAndNotifyCallAborted(void);
void GetDateTimeAndNotifyCallAbortedCBack(MYTIME *t);
void GetEndTimeAndNotifyCallMissed(void);
void GetEndTimeAndNotifyCallRejected(void);
void GetDateTimeAndNotifyCallMissedCBack(MYTIME *t);
void GetDateTimeAndNotifyCallRejectedCBack(MYTIME *t);
void GetEndTimeAndLogUnconnectedMOCall(void);
void GetDateTimeAndLogUnconnectedMOCallCBack(MYTIME *t);
void GetEndTimeAndNotifyEndCallDuration(CM_CALL_HANDLE handle);
void GetDateTimeAndNotifyEndCallDurationCBack(MYTIME *t);
void UpdateCallStartTimeforWapCall(MYTIME *t);

void InitializeTimeStructure (TIMEANDINDEX *time,MYTIME *t);
MYTIME * GetStartCallTime(void);

void SetPriorityCall(pBOOL priority);
pBOOL GetPriorityCall(void);
void SetSATScrnSavedFlag(pBOOL flag);
pBOOL GetSATScrnSavedFlag(void);
pBOOL isInCall(void);


#endif//CALLSTRUCTUREMANAGEMENT_PROT_H


