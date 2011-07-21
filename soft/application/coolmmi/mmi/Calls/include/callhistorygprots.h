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
 *	CallHistoryGProts.h
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
#ifndef _COOLSAND_CALLSHISTORYGPROT_H
#define _COOLSAND_CALLSHISTORYGPROT_H

void EntryCHISTNotifyMissedCall( void );
void ExitCHISTNotifyMissedCall( void );

void CHISTIncSentSMS( U8 nsimID );
void CHISTIncRecvdSMS( U8 nsimID );
void CHISTRefreshSMSCount(void);

void CHISTGetCallLogBySENDKey(void);
#ifdef __MMI_UI_CALL_LOG_IN_TAB_PANE__
void EntryCHISTTabPaneIdleScreen(void);
#endif
U8 CHISTGetMissedCallCountBeforeView( U8 nSimID );

void ReqUpdateCallLogName(U8 length, U8 dcs, PU8 name);
void RspUpdateCallLogName(void *info);
pBOOL CHISTSyncCallLogNeeded (U8 length, U8 type, PU8 number);
void mmi_chist_rearrange_missed_call_status_icon(void);
void mmi_chist_rearrange_rejected_call_status_icon(void);
#ifdef __MMI_CHIST_ACCU_NUM_MISSED_CALL_BEFORE_VIEW_AFTER_REBOOT__
extern void mmi_chist_set_num_missed_call_before_view(void);
extern U16 CHISGetUnreadMissedCallNum();
#endif

#ifdef __MMI_MULTI_SIM__
void SetCHISContext(U8 nSimId);
#endif
#endif


