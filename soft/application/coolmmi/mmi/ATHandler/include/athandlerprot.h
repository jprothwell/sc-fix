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
 *  ATHandlerProt.h
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

/**********************************************************************************
	Filename:		ATHandlerProt.h
	Author:			Gautam
	Date Created:	
	Contains:	AT comamnd handler
**********************************************************************************/
#ifndef __ATPROT_H
#define __ATPROT_H
#include "mmi_data_types.h"

void ATSetVolume(void *msg);
void ATSimulateKeyBoard(void *msg);
void ATTextOnLCD(void *msg);
void AtShowStatusIcon(void *msg);
void ATSetAlarm(void *msg) ;
void ATCancelAlarm(void *msg);
void ATSetRTC(void *msg) ;
void ATQueryAlarm(void *msg);
void AtSetGreetingTextHdlr(void *msg);
void ATMakeOutgoingCall(void *msg);
void ATAnswerIncomingCall(void *msg);
void ATHangupCall(void *msg) ;
void ATSyncCallTable(void *msg) ;
void ATTextOnLCDExit(void);
void ATSetDateTime(void *msg);
void ATSetMute(void *msg);
void ATSetSilentMode(void *msg);
void ATSetVibrationMode(void *msg);
void ATSetDateandTimeFormat(void *msg);
void ATSetLanguage(void *msg);
void SendLangChange2AT( U8 lang );

void AtQueryGreetingTextIndHdlr(void *msg);
void AtQueryLangIndHdlr(void *msg);
void ATSetProfileHandler( void *msg );
void AtQuerySilentModeIndHdlr(void *msg);
void AtQueryVibratorModeIndHdlr(void *msg);
void AtQueryDTFmtIndHdlr(void *msg);
void AtQueryMuteModeIndHdlr(void *msg);
extern void ATPowerReset(void*);
void SendMsg2AT(U16 msg_id, void *local_param_ptr, void *peer_buf_ptr);


#endif


