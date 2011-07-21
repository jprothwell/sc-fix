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
 *  CallManagementGprot.h
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

	FILENAME	: CallManagementGprot.h

  	PURPOSE		: Call Management Global Prototypes

 

	AUTHOR		: Ashish

	DATE		: Nov 8,02

**************************************************************/
#ifndef _INCOMING_CALL_MANGEMENT_GPROT_H
#define _INCOMING_CALL_MANGEMENT_GPROT_H

#include "callmanagementstruct.h"

void InitCallManagement (void);
void DeInitCM(void);
pBOOL isInCall(void);
extern pBOOL isOutgoingCall(void);
extern pBOOL isIncomingCall(void);
pBOOL IsWapCallPresent (void);
void CheckAndPrintMsgId	(U16 Id);

CALL_STATE GetCurrentState(void);
U16 GetTotalCallCount(void);
S16 GetTotalActiveCallCount(void);
U16 GetTotalHoldCallCount(void);
void SyncCallList(void);
void SetMissedCallFlag(pBOOL flag);
void SetRejectedCallFlag(pBOOL flag);
pBOOL GetMissedCallFlag(void);
pBOOL GetRejectedCallFlag(void);
void SetSyncCallFromATFlag(pBOOL flag);
pBOOL GetSyncCallFromATFlag(void);

pBOOL GetTransitionToActiveFlag (void);
pBOOL GetWapCallPresent(void);

void MakeMOForAT(PS8 num, U8 length, U8 callType);
void AcceptCallForAT(S16 handle);
void HangupAllCalls(void);

U16 IsPhoneInMute(void);
void SetLSpkNowOff(pBOOL flag);
pBOOL GetDTMFFlag(void);
U8 MapDtmfEnum2Ascii(U16 digit);
void EnterScreenAndHandleDigit(void);

U16 GetCmMarkerScrnID (void);

void NotifyCMClampOpen(void);
void NotifyCMClampClose(void);
void ProcessHeadsetKeyPress(void);

void CBackCallDeflectNumberFromPB(PS8 number);
void MakeCall(PS8 strNumber);
void ShowOutgoingIndicationScreen(PU8 number);
void RemoveOutgoingIndicationScreen(void);
void NotifyMONumberChange(U8* newNumber,U8* ptrDisplayString);

//micha0915
void RemoveSSReqScr( void);


#endif //_INCOMING_CALL_MANGEMENT_GPROT_H


