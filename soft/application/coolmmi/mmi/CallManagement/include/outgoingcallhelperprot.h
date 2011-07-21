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
 *  OutgoingCallHelperProt.h
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

	FILENAME	: OutgoingCallHelperProt.h

  	PURPOSE		: Call Management application

 

	AUTHOR		: Subramanya BG 

	DATE		: Nov 16,02

**************************************************************/
#ifndef _PIXCOM_OUTGOINGCALLHELPER_PROT_H
#define _PIXCOM_OUTGOINGCALLHELPER_PROT_H


void MakeMyCall(PS8 strNumber);
void CheckShortCutOrCall(void);
void MakeOutgoingcall(OUTGOING_CALL MsgStruct);
pBOOL IsStandardGsmSS(PU8 dtmfBuf);
pBOOL isHashString(PU8 buf);
pBOOL isShortString(PU8 buf);
pBOOL UCS2StrNoFirstPositionSearch(PS8 strNumber,S8 charToSearch);
pBOOL UCS2StrFirstPositionSearch(PS8 strNumber);
void PsCBackSetCallFeatures(void* info);
void PsCbackOutgoingCallIdSync(void* MsgStruct);
void OutgoingCallConnected(void* MsgStruct);
void PsCBackOutgoingCallEnded(void* MsgStruct);
void PsCBackUSSDResponse(void* info);
 
void ReleaseCall(void);
 
void DropRequest(void);
void HangupCall(CM_CALL_HANDLE CallHandle);
void HangupReqSucess(void* MsgStruct);

void HangupAll(void);
void HangupallReqSucess(void* MsgStruct);
void HangupallActive(void);
void HangupallActReqSucess(void* MsgStruct);
void HangupallHeld(void);
void HangupallHldReqSucess(void* MsgStruct);
void MakeActiveCallHold(void);
void HoldReqSucess(void* MsgStruct);
void RetrieveHeldCall(void);
void RetrieveReqSucess(void* MsgStruct);
void SwapCall(void);
void SwapReqSucess(void* MsgStruct);
void ConferenceCall(void);
void ConfReqSucess(void* MsgStruct);
void SplitCall(CM_CALL_HANDLE CallHandle);
void SplitReqSucess(void* MsgStruct);
void TransferCall(void);
void EctReqSucess(void* MsgStruct);

void EntryScrDisplayDialedDigits(void); 
void ExitScrDisplayDialedDigits(void);
void InCallSSCParsing(void);
void HandleDTMFScreenDigits(void);
void UpdateDialedDigitDisplay(void);
void HandleDTMFScreenStopTone(void);
void HandleDisplayDialedDigits(void);
void MakeCallFromDTMFScr(void);
#ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
void MakeSim1CallFromDTMFScr(void);
void MakeSim2CallFromDTMFScr(void);
#endif
void CallMgmtSavePhoneBook(void);

void ClearDtmfBuf(void);
void FillDtmfBuff(PU8 buf);
PU8 GetDtmfBuffer(void);

/**********************************************************************/

//void ProcessAcceptIncomingForAT(void *MsgStruct);
//void AcceptIncomingSuc(void* MsgStruct);

// void StoreAndKeepKeyCode (U16 keycode, U16 keytype);
void SetDialIndication(pBOOL truth);
pBOOL GetDialIndication(void);

#endif //_PIXCOM_OUTGOINGCALLHELPER_PROT_H


