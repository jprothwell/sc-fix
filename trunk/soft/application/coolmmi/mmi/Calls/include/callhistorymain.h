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
 *	CallHistoryMain.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for ¡K.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
 
/**************************************************************

	FILENAME	: Callorwarding.h

  	PURPOSE		: Call application

 

	 

	DATE		: 29th April 2003

**************************************************************/


#ifndef _COOLSAND_CALLHISTORYMAIN_H
#define _COOLSAND_CALLHISTORYMAIN_H

void InitCallHistory( void );
void InitCallHistoryContext( void );
void InitCallLogs( void );
void InitCallCost( void );
void InitCallsReset( void );
void InitSMSCounter( void );
void InitCallTimes( void );

void EntryCHISTMainMenu( void );
void ExitCHISTMainMenu( void );

void CHISTResetCallLogIndex (void);

#ifdef __MMI_GPRS_FEATURES__
void InitGPRSCounter(void);
void HighlightGPRSCount( void );
void HighlightGPRSCountLastSent(void);
void HighlightGPRSCountLastRecieved(void);
void HighlightGPRSCountAllSent(void);
void HighlightGPRSCountAllReceived(void);
void HighlightGPRSCountCounterReset(void);
void HintGPRSCountLastSent(U16);
void HintGPRSCountLastRecieved(U16);
void HintGPRSCountAllSent(U16);
void HintGPRSCountAllReceived(U16);
void GetGPRSCountersRequest(void);
void GetGPRSCountersResponse(void *info);
void EntryScrGPRSRecvCounterResetConfirmation( void );
void ExitScrGPRSRecvCounterResetConfirmation( void );
void ResetGPRSCounterReq( void );
void ResetGPRSCounterRsp( void *info );
void EntryScrGPRSCounter( void );
void ExitGPRSCounter( void );
#endif

#endif 


