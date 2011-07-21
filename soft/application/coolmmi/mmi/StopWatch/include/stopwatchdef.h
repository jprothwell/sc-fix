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
 *  StopwatchDef.h
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
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/StopWatch/include/StopwatchDef.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 11:03:30   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:43:50   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:28:42   zhangxb
 * Initial revision.
 * Revision 1.1.1.1  2006/06/08 12:43:26  liuruijin
 * no message
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: StopwatchDef.h

  	PURPOSE		: contains macro definitions for stop watch application.

 

 

	DATE		: 05/01/2004

**************************************************************/


#ifndef _STOPWATCH_DEF_H
#define _STOPWATCH_DEF_H

#include "mmi_features.h"
#ifdef __MMI_STOPWATCH__

#include "mmi_data_types.h"
/******************************************************************/

/***************************************************************************** 
* Define
*****************************************************************************/
#define MAX_STOPWATCH_RECORDS		10

#define MAX_RECORD_NAME_LENGTH		16

#define MAX_TIMER_RECORD_DIGITS		11

#define POPUP_TIME					UI_POPUP_NOTIFYDURATION_TIME_2000

enum MP_TIMER_STATE
{
	TIMER_NOT_STARTED=0,
	TIMER_STARTED,
	TIMER_STOPPED,
	TIMER_RUNNING
};

enum TIMER_TYPE
{
	LAP_TIMER = 0,
	SPLIT_TIMER
};


#ifndef MMI_ON_HARDWARE_P
#ifndef NVRAM_STOPWATCH_RECORD_SIZE
	#ifdef __UCS2_ENCODING
		#define NVRAM_STOPWATCH_RECORD_SIZE  1152
	#else
		#define NVRAM_STOPWATCH_RECORD_SIZE  980
	#endif
#endif
#endif


enum STR_STOPWATCH_LIST
{
	STR_STOPWATCH_TITLE		=	STOPWATCH_BASE+1,
	STR_TYPICAL_STOPWATCH,  //39002
	STR_MULTIPURPOSE_STOPWATCH,
	STR_SPLIT_TIMING,
	STR_LAP_TIMING,
	STR_VIEW_RECORD,
	STR_STOPWATCH_START,
	STR_STOPWATCH_STOP,
	STR_STOPWATCH_SPLIT,
	STR_STOPWATCH_LAP,
	STR_STOPWATCH_RESUME, //39011
	STR_STOPWATCH_MEMORY_FULL,
	STR_STOPWATCH_NOT_SAVED,
	STR_STOPWATCH_REPLACE_RECORD,
	STR_STOPWATCH_SAVE_STOPWATCH,
	 
	//STR_STOPWATCH_LIST_EMPTY,
	//STR_VIEW_STOPWATCH_LIST_OPTION_VIEW,
	//STR_VIEW_STOPWATCH_LIST_OPTION_DELETEALL,
	//STR_CONFIRM_DELETE_ALL_RECORD,
	STR_EMPTY_NAME,  //39016
	STR_MUTIPURPOSE_TITLE_MSG,
	STR_DISCONTINUE_TIMER_QUERY,
	STR_MUTIPURPOSE_TITLE_SIDE_MSG

};

enum IMG_STOPWATCH_LIST
{
	IMG_MULTIPURPOSE_TIMER1		=		STOPWATCH_BASE+1,
	IMG_MULTIPURPOSE_TIMER2,
	IMG_MULTIPURPOSE_TIMER3,
	IMG_MULTIPURPOSE_TIMER4,
	IMG_TYPICALSW_BACKGROUND,  //39005
 
	IMG_TYPICALSW_LIST_BACKGROUND,
//	#endif
	IMG_STOPWATCH_ICON
};

enum SCR_STOPWATCH_LIST
{
	SCR_STOPWATCH			=	STOPWATCH_BASE+1,
	SCR_TYPICAL_STOPWATCH,
	SCR_MULTIPURPOSE_STOPWATCH,
	SCR_SPLIT_TIMING,
	SCR_VIEW_RECORD,
	SCR_ENTER_RECORD_NAME,
	SCR_SAVE_OPTIONS,
	SCR_VIEW_RECORD_OPTIONS,
	SCR_VIEW_SAVED_RECORD,
	SCR_REPLACE_RECORD
};


#endif
#endif

