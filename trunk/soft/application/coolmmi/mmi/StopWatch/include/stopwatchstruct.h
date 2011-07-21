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
 *  StopwatchStruct.h
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
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/StopWatch/include/StopwatchStruct.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 11:03:32   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:43:54   lin
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

	FILENAME	: StopwatchStruct.h

  	PURPOSE		: Contains declaration of all global variables

 

 

	DATE		: 05/01/2004

**************************************************************/

#ifndef __STOPWATCH_STRUCT_H__
#define __STOPWATCH_STRUCT_H__

#include "mmi_features.h"
#ifdef __MMI_STOPWATCH__

/***************************************************************************** 
* All structures Typedef Delaration
*****************************************************************************/
typedef struct
	{
			U8 recHr;
			U8 recMin;
			U8 recSec;
			U8 recMsec;
		}nRecordDetail;

typedef struct
	{
		S8 name[(MAX_RECORD_NAME_LENGTH+1)*ENCODING_LENGTH];
		nRecordDetail nRecordData[MAX_TIMER_RECORDS];
		U8 numOfRecords;
	}stopwatchRecord;

/*For global variables of stopwatch*/
typedef struct { 

					S32 numofCurrTimerRecords;      
					U8 numOfSavedTimerRecords;
					U8 splitLapTimer;
					U8 recordName[(MAX_RECORD_NAME_LENGTH +1) * ENCODING_LENGTH];
					S32 currHilitedRecord;
					pBOOL replaceRecord;
					pBOOL isRecordPresent;
					U8 TimerState;
/*For MultiPurpose Stopwatch*/
					U8 MPTimerState;
					U8 currentTimerIndex;
					S32 currentHiliteTimer;
				}wtchCntxStruct;

typedef struct{
				U8 timerIndex[MAX_MULTIPURPOSE_TIMER];
				U16 assignedKey[MAX_MULTIPURPOSE_TIMER];
    		  }nWayTimerStruct;

typedef struct {
	U16 x;
	U16 y;
} wtch_coor_type;

typedef struct {

	/* typical stopwatch */
	wtch_coor_type typ_timer;
	wtch_coor_type typ_milsec;
	U16 typ_history_height;

	/* nWay stopwatch */
//	U16 maintimer_height;
	U16 separator_height;
	wtch_coor_type nWay_maintimer_start;
	wtch_coor_type nWay_maintimer_end;
	wtch_coor_type nWay_dialog_start;
	wtch_coor_type nWay_dialog_end;
	
}wtchSkinResStruct;

#endif
#endif

