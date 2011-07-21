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
 *	CallsLog.h
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

	FILENAME	: Calls_GProts.h

  	PURPOSE		: Call application Global Prototypes

 

 

	DATE		: Oct 4,02

**************************************************************/
/************************************************************************
  Change Histroy
  Version      Date             Author    Description
  02.01.001  2007-06-14, Li Tao      implement the new feature "SendSMS"
  02.01.001  2007-06-26, Li Tao      modify the menuID DIALED_CALL_MENU_SENDSMS to 1879
 ************************************************************************/
 
#ifndef _COOLSAND_CALLSLOG_H
#define _COOLSAND_CALLSLOG_H

	
void HighlightCHISTDialedCalls( void );
void HighlightCHISTMissedCalls( void );
void HighlightCHISTRecvdCalls( void );
void HighlightCHISTRejectedCalls( void );

void HighlightCHISTCallDialedNum( void );
void HighlightCHISTSaveDialedNum( void );
void HighlightCHISTDeleteDialedNum( void );
void HighlightCHISTSendSMSDialedNum( void );	//2007-06-14, Li Tao
void HighlightCHISTEditDialedNum( void );

void HighlightCHISTCallMissedNum( void );
void HighlightCHISTCallRejectedNum( void );
void HighlightCHISTSaveMissedNum( void );
void HighlightCHISTDeleteMissedNum( void );
void HighlightCHISTSendSMSMissedNum( void );	//2007-06-14, Li Tao
void HighlightCHISTEditMissedNum( void );
#ifdef __IP_NUMBER__
void HighlightMissedIpDial( void );
void HighlightReceivedIpDial( void );
void HighlightRejectedIpDial( void );
void HighlightDialedIpDial( void );
#endif

void HighlightCHISTCallRecvdNum( void );
void HighlightCHISTSaveRecvdNum( void );
void HighlightCHISTSaveRejectedNum( void );
void HighlightCHISTDeleteRecvdNum( void );
void HighlightCHISTDeleteRejectedNum( void );
void HighlightCHISTSendSMSRecvdNum( void );	//2007-06-14, Li Tao
void HighlightCHISTSendSMSRejectedNum( void );
void HighlightCHISTEditRecvdNum( void );
void HighlightCHISTEditRejectedNum( void );
#if defined(__MMI_PHB_SEND_MMS_FROM_PHB__)
void HighlightCHISTSendMMSRecvdNum( void );
void HighlightCHISTSendMMSRejectedNum( void );
void HighlightCHISTSendMMSMissedNum( void );
void HighlightCHISTSendMMSDialedNum( void );
#endif
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
void HighlightCHISTInfoDialedNum(void);
void HighlightCHISTInfoMissedNum(void);
void HighlightCHISTInfoRecvdNum(void);
#endif
void HintCHISTDialedCalls(U16 index);
void HintCHISTMissedCalls(U16 index);
void HintCHISTRecvdCalls(U16 index);
void HintCHISTRejectedCalls(U16 index);

void EntryCHISTViewDialedCallList( void );
void ExitCHISTViewDialedCallList( void );
void EntryCHISTViewDialedCall( void );
void ExitCHISTViewDialedCall( void );
void EntryCHISTDialedListOptions( void );
void ExitCHISTDialedListOptions( void );
void EntryCHISTDeleteDialedCallConfirm( void );
void ExitCHISTDeleteDialedCallConfirm( void );

void EntryCHISTViewMissedCallList( void );
void ExitCHISTViewMissedCallList( void );
void EntryCHISTViewMissedCall( void );
void EntryCHISTViewRejectedCall( void );
void ExitCHISTViewMissedCall( void );
void ExitCHISTViewRejectedCall( void );
void EntryCHISTMissedListOptions( void );
void EntryCHISTRejectedListOptions( void );
void ExitCHISTMissedListOptions( void );
void ExitCHISTRejectedListOptions( void );
void EntryCHISTDeleteMissedCallConfirm( void );
void ExitCHISTDeleteMissedCallConfirm( void );
void EntrySubCHISTNotifyMissedCall(void);
void ExitSubCHISTNotifyMissedCall(void);

void EntryCHISTViewRecvdCallList( void );
void ExitCHISTViewRecvdCallList( void );
void EntryCHISTViewRecvdCall( void );
void ExitCHISTViewRecvdCall( void );
void EntryCHISTRecvdListOptions( void );
void ExitCHISTRecvdListOptions( void );
void EntryCHISTDeleteRecvdCallConfirm( void );
void EntryCHISTDeleteRejectedCallConfirm( void );
void ExitCHISTDeleteRecvdCallConfirm( void );

void CHISTAnimationTimeOutHdlr(void);
void ExitCHISTProcessingAnimation(void);

void ReqReadDialedNumByKey(void);
void ReqReadDialedNum(void);
void RspReadDialedNum(void *info);
void ReqWriteCallLog(CALL_INFO *dailCall, U8 type);
void RspWriteDialedNum(void *info);
void ReqDeleteCallLog(U8 type, U8 index, U8 no_data);
void RspDeleteDialedNum(void *info);

void ReqReadRejectedNumByKey(void);
void ReqReadRejectedNum(void);
void RspReadRejectedNum(void *info);
void RspWriteRejectedNum(void *info);
void RspDeleteRejectedNum(void *info);

void ReqReadMissedNumByKey(void);
void ReqReadMissedNum(void);
void RspReadMissedNum(void *info);
void RspWriteMissedNum(void *info);
void RspDeleteMissedNum(void *info);

void ReqReadRecvdNumByKey(void);
void ReqReadRecvdNum(void);
void RspReadRecvdNum(void *info);
void RspWriteRecvdNum(void *info);
void RspDeleteRecvdNum(void *info);
void RspDeleteRejectedNum(void *info);

void ReadyLogIndication(void *info);

#ifdef __IP_NUMBER__
void SetTempUseIPNumber(pBOOL flag);
void CHISTCallDialedNumByIP( void );
void CHISTCallMissedNumByIP( void );
void CHISTCallRecvdNumByIP( void );
void CHISTCallRejectedNumByIP(void);
#endif

void CHISTDeleteDialedNum( void );
void CHISTSaveDialedNum( void );
void CHISTCallDialedNum( void );
void CHISTSendSMSDialedNum( void );	//2007-06-14, Li Tao
void CHISTEditDialedNum( void );

void CHISTDeleteMissedNum( void );
void CHISTSaveMissedNum( void );
void CHISTCallMissedNum( void );
void CHISTCallRejectedNum( void );
void CHISTSendSMSMissedNum( void );	//2007-06-14, Li Tao
void CHISTEditMissedNum( void );

void CHISTDeleteRecvdNum( void );
void CHISTDeleteRejectedNum( void );
void CHISTSaveRecvdNum( void );
void CHISTSaveRejectedNum( void );
void CHISTCallRecvdNum( void );
void CHISTSendSMSRecvdNum( void );	//2007-06-14, Li Tao
void CHISTSendSMSRejectedNum( void );
void CHISTEditRecvdNum( void );
void CHISTEditRejectedNum( void );
#if defined(__MMI_PHB_SEND_MMS_FROM_PHB__)
void CHISTSendMMSRecvdNum( void );
void CHISTSendMMSRejectedNum( void );
void CHISTSendMMSMissedNum( void );
void CHISTSendMMSDialedNum( void );
#endif

void CHISTPopulateDialedCallList( U8* numberList[], U8* phoneList[] );
void CHISTPopulateMissedCallList( U8* numberList[], U8* phoneList[] );
void CHISTPopulateRecvdCallList( U8* numberList[], U8* phoneList[] );
void CHISTPopulateRejectedCallList( U8* numberList[], U8* phoneList[] );

void CHISTGetCurrDialedCallIndex( S32 ind );
void CHISTGetCurrMissedCallIndex( S32 ind );
void CHISTGetCurrRecvdCallIndex( S32 ind );
void CHISTGetCurrRejectedCallIndex( S32 ind );

void CHISTExtractPSCallLog( LOG_CALL *logCall,
                               l4c_phb_ln_entry_struct* phbLastNumber);

#ifdef __MMI_CH_MIXED_CALL_LOG__
void EntryCHISTViewMixedCallList(void);
void ExitCHISTViewMixedCallList(void);
 
#ifdef __MMI_CH_OPTIONS_INCLUDE_INFO__
void EntryCHISTMixedListOptions( void );
#else
void EntryCHISTViewMixedCall( void );
#endif
 
void CHISTCallMixedNum( void );
#ifdef __IP_NUMBER__
void CHISTCallMixedNumByIP( void );
#endif
void CHISTPopulateMixedCallList( U8* numberList[], U8* phoneList[] );
void CHISTGetCurrMixedCallIndex( S32 ind );
void CHISTSortCallLogsByTime(void);
BOOL CHISTIsDuplicatedNumber(U8 index);
#endif

void CHISTIgnoreMissedCallScreen(void);
void CHISTReadMissedNum (void);
void ConvertRTC2MYTIME(MYTIME * time, CALL_LOG_RTCTIMEFORMAT * rtc);
void ConvertMYTIME2RTC(CALL_LOG_RTCTIMEFORMAT * rtc, MYTIME * time);

#endif

