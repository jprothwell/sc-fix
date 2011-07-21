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
 *	CallsStruct.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for .
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: Calls_Defs.h

  	PURPOSE		: Call application 

 

	DATE		: Jan 15,02

**************************************************************/

#ifndef _PIXCOM_CALLSTRUCT_H
#define _PIXCOM_CALLSTRUCT_H

#include "callsdefs.h"
 
#ifdef MMI_ON_HARDWARE_P
#include "l4c_common_enum.h"
#endif
 

#define CALL_LOG_NUMBER_STRUCT   l4c_number_struct
#define CALL_LOG_NAME_STRUCT     l4_name_struct

#ifdef __MMI_MULTI_SIM__
#define MMI_CHIS_DEST_MOD   (MOD_L4C + chis_p->nSimId)
#else
#define MMI_CHIS_DEST_MOD   MOD_L4C
#endif


typedef struct {
	U8	numType;
	U8	numLength;
	U8	number[(MAX_CC_ADDR_LEN+1)*ENCODING_LENGTH];
	U8	nameLength;
	U8	nameDCS;
	S8 pbName[(MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH];
	MYTIME	startTime;  
	MYTIME duration;
	U16 no_of_times;
} LOG_CALL;

typedef LOG_CALL RECEIVED_CALL;
typedef LOG_CALL DIALED_CALL;
typedef LOG_CALL MISSED_CALL;
typedef LOG_CALL REJECTED_CALL;
typedef struct
{
   U8 nSimId;
   U8 curCallLogType;
   U8 currDialedCallIndex;
   U8 currMissedCallIndex;
   U8 currRecvdCallIndex;
   U8 currRejectedCallIndex;
   U8 nDialedCalls;
   U8 nMissedCalls;
   U8 nRecvdCalls;
   U8 nRejectedCalls;
   U8 nMissedCallBeforeView;
   U8 nRejectedCallBeforeView;
   U8 dialFromCallLog;
   U8 process;
   U8 sequenceId;
   BOOL isFirstTime;
   BOOL isDeleteAll;
   BOOL isCallLogReady;
   BOOL isReqSent;
   BOOL saveMissedScrn;   
#ifdef __MMI_CH_MIXED_CALL_LOG__
   U8 currMixedCallIndex;
   U8 nMixedCalls;
   BOOL isMixedStyle;
  LOG_CALL dialedCalls[TOT_SIZE_OF_DIALED_LIST];  
  LOG_CALL missedCalls[TOT_SIZE_OF_MISSED_LIST];  
   LOG_CALL recvdCalls[TOT_SIZE_OF_RECVD_LIST];
   LOG_CALL rejectedCalls[TOT_SIZE_OF_REJECTED_LIST];
   LOG_CALL *mixedCalls[TOT_SIZE_OF_MIXED_LIST];  
 
#else
   LOG_CALL *dialedCalls;
   LOG_CALL *missedCalls;
   LOG_CALL *recvdCalls;
   LOG_CALL *rejectedCalls;
   LOG_CALL callsLog[TOT_SIZE_OF_CALLS_LIST];
#endif

   U16 nSendSMSCount;
   U16 nRecvSMSCount;

   U8 currCostIndex;
   U16 currCostItem;
   U16 nQuickEndTime;
   S32 nQuickEndFlag;

   //modified by zhoumn @2006//12/05>>>start 
   //U32 lastCallCost;
   //U32 allCallCost;
#ifdef _MMI_OPERATOR_
   U8 lastCallCost[4 + 1 + 2];		// 4:整数位数(1 day); 1:小数点; 2:小数位数
   U8 allCallCost[6 + 1 + 2];		// 6:整数位数(365 day); 1:小数点; 2:小数位数
#else
   double lastCallCost;
   double allCallCost;
#endif
   //modified by zhoumn @2006//12/05<<<end
   
   U32 maxCallCost;
   U8 newMaxCostStr[10*ENCODING_LENGTH];   

   double ppu;     /* hold PS return ppu */
   U8 PPUStr[20*ENCODING_LENGTH];  /* hold PS return currency*/
   U8 newPPUStr[20*ENCODING_LENGTH];   /* for edit currency */

   U8 currencyStr[4*ENCODING_LENGTH];  /* hold PS return currency*/
   U8 newCurrencyStr[4*ENCODING_LENGTH];   /* for edit currency */

   MYTIME total_out_time;
   MYTIME total_in_time;
   MYTIME last_call_time;

}call_history_context_struct;
extern call_history_context_struct g_CallHistory_cntxt[];
extern call_history_context_struct *chis_p;

#endif


