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
 *  PsCallHelperGprot.h
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

	FILENAME	: PsCallHelperGprot.h

  	PURPOSE		: Call Management application

 

	AUTHOR		: Subramanya BG

	DATE		: Nov 16,02

**************************************************************/
#ifndef _PIXCOM_PSCALLHELPER_GPROT_H
#define _PIXCOM_PSCALLHELPER_GPROT_H

void MakePsInitiateCall(U8* MsgStruct, void* callBack);
void MakePsSSVUSSDReq(U8* MsgStruct, U8* IPNum, 
                              U16 length, void* callBack);
void MakePsCallAcceptIncoming(void* callBack);
void MakePsAthRequest(void* callBack);
 
void MakePsReleaseCompleteRequest(CM_CALL_HANDLE handle);
 
void MakePsSendUDUB(void* callBack);
void MakePsHangupallActiveAcceptOtherRequest(void* callBack);
void MakePsEndSelectiveCall(void* callBack, CM_CALL_HANDLE handle);
void MakePsActiveHold(void* callBack);
void MakePsSplitRequest(CM_CALL_HANDLE nCallHandle);
void MakePsConfCallRequest(void);
void MakePsEctRequest(void);
void MakePsActivateCCBS(void* callBack);
void MakePsHangupallRequest(void* callBack);
void MakePsSwapCallRequest(void* callBack);
void MakePsSendRelHeld(void* callBack);
void MakePsHangupallActiveRequest(void* callBack);
void MakePsHangupallCallsRequest(void* callBack);
void MakePsCallDeflect(void* callBack,PU8 CallDeflectNumber);
void MakePsGetCallList(void* callBack);
void MakePsDropUSSDReq(PsFuncPtr callBack);
void MakePsSendUSSDReq(S8 *pSrc);
void MakePsSendDTMFTone(U16* MsgStruct);
void MakePsStopDTMFTone(void);
void MakePsSendDTMFString(U8* MsgStruct);
void MakePsReverseSpeechStatus(pBOOL status);
void MakePsStartVoiceRecord(void);
void MakePsStopVoiceRecord(void);
void StartVoiceRecordReqSucess(void* MsgStruct);
void VoiceRecordFinishHandler(void* MsgStruct);

pBOOL DeriveAndSetCallFeatures(void* info);
S16 DeriveOutgoingCallIdRsp(void* info, U16 *cause);
CM_CALL_HANDLE DeriveCallHandle(void* info);
void DeriveCallInfo(void* info, IncomingCall* incmg);
void DeriveCallInfoFromWait(void* info, IncomingCall* incmg);
void DeriveSSEvent(void* info);
U16 GetStrDisplayForId(U16 event);
U32 DeriveCcmValue(void* info);
U8 DeriveInternalAlrtIndVal(void* info);
pBOOL CheckStatusOfSpeech(void* info);
void DeriveUSSDData (U8 dcs, U8* ussd_string);
void DeriveUSSNbuf(void* info);
void DeriveUSSRbuf(void* info);
void DeriveUSSDbuf(void* info);

pBOOL CheckChldRequestSuccess(void* info, U16 *cause);
void CheckFailureChld(void* info);
U16 CheckCallDeflectSuccess(void* info, U16* cause);
U16 CheckAtaSuccess(void* info,U16 *cause);
pBOOL CheckRspCcbsFlag(void* info);

void CBackDropUSSDReq(void *info);
void CBackDropUSSDReqEND(void *info);
void CBackDropUSSDSatReq(void *info);

#endif //_PIXCOM_PSCALLHELPER_GPROT_H


