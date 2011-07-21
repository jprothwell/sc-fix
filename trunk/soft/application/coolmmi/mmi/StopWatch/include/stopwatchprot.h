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
 *  StopwatchProt.h
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
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/StopWatch/include/StopwatchProt.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 11:03:32   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:43:52   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:28:44   zhangxb
 * Initial revision.
 * Revision 1.1.1.1  2006/06/08 12:43:26  liuruijin
 * no message
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: StopwatchProt.h

  	PURPOSE		: It contains the prototype declaration of internally used functions

 

 

	DATE		: 05/01/2004

**************************************************************/
#ifndef __STOPWATCH_PROT_H__
#define __STOPWATCH_PROT_H__

#include "mmi_features.h"
#ifdef __MMI_STOPWATCH__

#include "stopwatchstruct.h"

/***************************************************************************** 
* Local Function Declaration 
*****************************************************************************/
void HighlightStopwatch(void);
void HighlightTypicalStopwatch(void);
void HighlightMultipurposeStopwatch(void);
void HighlightSplitTiming(void);
void HighlightLapTiming(void);
void HighlightViewRecord(void);
void HighlightOptionDone(void);
void HighlightOptionInputMethod(void);
void HighlightOptionView(void);
void HighlightOptionDelete(void);
void HighlightOptionDeleteAll(void);


void EntryWtchScrStopwatch(void);
void EntryWtchScrTypicalStopwatch(void);
void EntryWtchScrMultipurposeStopwatch(void);
void EntryWtchScrSplitLapTimingStopwatch(void);
void EntryWtchScrViewRecord(void);

void ExitWtchScrStopwatch(void);
void ExitWtchScrTypicalStopwatch(void);
void ExitWtchScrMultiPurposeStopwatch(void);
void ExitWtchScrSplitLapTimingStopwatch(void);
void ExitWtchScrViewRecord(void);


void StartSplitLapTimer(void);
void RecordSplitLapTime(void);
void ResumeSplitLapTimer(void);
void StopSplitLapTimer(void);
void ResetSplitLapTimer(void);
void SetLSKforSplitLapTimerResumeLongPress(void);
void SaveTimerRecord(void);
void ConfirmSaveTimerRecord(void);
void EntryWtchScrEnterRecordName(void);
void EntryWtchScrMemoryFull(void);
void ExitWtchScrRecordName(void);
void EntryWtchRecordSaveOption(void);
void ExitWtchScrRecordSaveOption(void);

void EntryWtchScrReplaceRecord(void);
void ExitWtchScrReplaceRecord(void);
void ReplaceSelectedRecord(void);

void GoBackHistoryFnFromInputMethod(void);

void GoBackToStopwatchMenu(void);
void EntryWtchScrViewRecordOptions(void);
void ExitWtchScrViewRecordOptions(void);
void EntryWtchScrViewSavedRecord(void);
void ExitWtchScrViewSavedRecord(void);

void GetSelectedItemIndex(S32);

void EntryWtchScrDeleteRecord(void);
void DeleteSelectedRecord(void);

void EntryWtchScrDeleteAllRecord(void);
void DeleteAllTimerRecord(void);
void DisplayDiscontinueTimerQuery(void);
void ResetTypicalStopwatch(void);
void DiscontinuePreviousStopwatch(void);

/****Multipurpose Stopwatch*****************/
void InitMultipurposeStopwatch(void);
void ResetAllTimer(void);
void StopCurrentTimer(void);
void SetLSKMultiPurposeTimerResumeLongPress(void);

void StartKeyTimer(void);
void StartIndexTimer(void);



#endif
#endif

