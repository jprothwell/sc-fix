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
 *  Wrapper.c
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

	FILENAME	: Wrapper.c

  	PURPOSE		: OS wrapper functions to make the task structure functionality 
				independant of underlying operating system. 
 

 

	DATE		: Dec' 28, 2002

**************************************************************/

// Include files -----------------------------------
/* [001] */

#include "cswtype.h"
#include "wrappergprot.h"

#ifdef MMI_ON_WIN32
#include "taskinit.h"
#include "queuegprot.h"
#include "nvramprot.h"
#include "stdafx.h"
extern CRITICAL_SECTION TimerCS;
oslSoftTimerid TimerId[MAX_TIMERS];
oslTimerid TimerCallBackHdlr;
#endif /* MMI_ON_WIN32 */
#ifdef MMI_ON_HARDWARE_P
#include "l4dr.h"
#endif
#include "messagesmiscell.h"    
#include "adp_timer.h"   

#include "oslmemory.h"
#include "timerevents.h"
#include "mmi_trace.h"
// Macros and Data Structor Declerations --------------



//static oslTimerid TimerPtrs[MAX_TIMERS];
static U8 TimerExist[MAX_TIMERS];


/* kill all the semaphore and thread objects		 */

#ifdef MMI_ON_WIN32    
// ------Functions Decleration -------------------------------------

/*-------------------------------------------------------------------
Function Name	:	oslTaskid OslIntCreateTask
Description		:	This module creates the task 
Input			:	PS8 task_name,
					U8 priority,
					U32 stack_size,
					oslTaskFuncPtr entry_func,
					void * entry_param,
					U8 misc_options
Output			:	taskID
-------------------------------------------------------------------*/

oslTaskid OslIntCreateTask (
								PS8 task_name,
								U8 priority,
								U32 stack_size,
								oslTaskFuncPtr entry_func,
								void * entry_param,
								U8 misc_options)
{
#ifdef MMI_ON_WIN32
	U32 tThreadId;
	U32 win32_priority;
	oslTaskid taskid;
	taskid=CreateThread(NULL,stack_size,(LPTHREAD_START_ROUTINE)entry_func,
		entry_param,CREATE_SUSPENDED,&tThreadId);
	if(priority>=0 && priority<51) win32_priority= THREAD_PRIORITY_HIGHEST;
	else if(priority>=52 && priority<104) win32_priority= THREAD_PRIORITY_ABOVE_NORMAL;
	else if(priority>=104 && priority<156) win32_priority= THREAD_PRIORITY_NORMAL;
	else if(priority>=156 && priority<208) win32_priority= THREAD_PRIORITY_BELOW_NORMAL;
	else if(priority>=208 && priority<=255) win32_priority= THREAD_PRIORITY_LOWEST;
	SetThreadPriority(taskid,win32_priority);
	ResumeThread(taskid);
	return taskid;
#else
	return NULL;
#endif
}

/*-------------------------------------------------------------------
Function Name	:	OslIntDleteTask
Description		:	This module creates the task 
Input			:	TaskID
Output			:	TRYE or FALSE
-------------------------------------------------------------------*/
OSLSTATUS OslIntDleteTask(oslTaskid taskid)
{
#ifdef MMI_ON_WIN32
	CloseHandle(taskid);
#endif
	return OSL_SUCCESS;
}
#endif   

/*-------------------------------------------------------------------
Function Name	:	StartMyTimer
Description		:	This module starts the timer
Input			:	unsigned short nTimerId,
					unsigned short nTimeDuration,
					oslTimerFuncPtr TimerExpiry)
Output			:	BOOL
-------------------------------------------------------------------*/
U16 StartMyTimerInt(U16 nTimerId, U32 nTimeDuration, oslTimerFuncPtr TimerExpiry, U8 alignment)
{

	if(nTimerId>MAX_TIMERS_END)
	{
		while(1)
		{
			mmi_trace(1,"StartMyTimerInt,nTimerId is invalid!!!!,nTimerId is %d",nTimerId);

		}
	}
	if(0==nTimeDuration)
	{
		while(1)
		{
			mmi_trace(1,"StartMyTimerInt,nTimeDuration is invalid!!!!");

		}
	}
	if(TimerExist[nTimerId])
		OslStopSoftTimer(nTimerId);
	OslStartSoftTimer(nTimerId, TimerExpiry, nTimerId, nTimeDuration, alignment);
	TimerExist[nTimerId] = 1;
	return TRUE;
}
U16  StartMyTimer(U16 nTimerId, U32 nTimeDuration, oslTimerFuncPtr TimerExpiryFunction)
{
	return StartMyTimerInt(nTimerId, nTimeDuration, TimerExpiryFunction, 0);
}

/*-------------------------------------------------------------------
Function Name	:	StopMyTimer
Description		:	This module stops the timer
Input			:	timerID
Output			:	bool
-------------------------------------------------------------------*/
U16 StopMyTimerInt(U16 nTimerId)
{
	if(nTimerId>MAX_TIMERS_END)
	{
		while(1)
		{
			mmi_trace(1,"StopMyTimerInt,nTimerId is invalid!!!!,nTimerId is %d",nTimerId);

		}
	}

	if(TimerExist[nTimerId])
	{
		OslStopSoftTimer(nTimerId);
		TimerExist[nTimerId] = 0;
	}
	return TRUE;
}
U16 StopMyTimer(U16 nTimerId)
{
	return StopMyTimerInt(nTimerId);
}
/*-------------------------------------------------------------------
Function Name	:	IsMyTimerExist
Description		:	This module stops the timer
Input			:	timerID
Output			:	bool
-------------------------------------------------------------------*/
BOOL IsMyTimerExist(U16 nTimerId)
{
   if(TimerExist[nTimerId] == 1 )
   	return TRUE;
   else
      return FALSE;
}

#ifdef MMI_ON_WIN32	 
/*-------------------------------------------------------------------
Function Name	:	KillObjects
Description		:	This module kills the object but 
					currently of no use
Input			:	null
Output			:	bool
-------------------------------------------------------------------*/
U16 KillObjects(void)
{
#ifdef MMI_ON_WIN32	
	U16 i;
	for(i=9;i<WIN32_TOTAL_TASKS;i++)
	{
		OslIntDleteTask(task_info_g1[i].task_id);
		OslDeleteMsgQ(task_info_g1[i].task_ext_qid);
	}
	OslIntCancelTimer(TimerCallBackHdlr);
	OslIntDeleteTimer(TimerCallBackHdlr);
	for(i=0;i<MAX_TIMERS;i++)
		if(TimerExist[i]) OslStopSoftTimer(i);
	
#endif
	return TRUE;
}


/*-------------------------------------------------------------------
Function Name	:	OslIntCreateTimer
Description		:	This module creates the timer
Input			:	timer name
Output			:	timer ID
-------------------------------------------------------------------*/
oslTimerid OslIntCreateTimer(PS8 timer_name)
{
#ifdef MMI_ON_WIN32
	win32_timerid *tmrid;
	tmrid=(win32_timerid *)OslMalloc(sizeof(win32_timerid));
	strcpy(tmrid->tmr_name, timer_name);
	tmrid->tmr_handler=0;
	tmrid->tmr_timeout=0;
	tmrid->tmr_callback=NULL;
	tmrid->tmr_callback_arg=NULL;
	return tmrid;
#else
	return NULL;
#endif
}


/*-------------------------------------------------------------------
Function Name	:	OslIntSetTimer
Description		:	This module sets the timer
Input			:	oslTimerid timerid, 
					oslTimerFuncPtr func_ptr,
					void * funcArg,
					U32 timeDurationMsec
Output			:	bool
-------------------------------------------------------------------*/
OSLSTATUS OslIntSetTimer(oslTimerid timerid, 
						 oslTimerFuncPtr func_ptr,
						 void * funcArg,
						 U32 timeDurationMsec)
{
#ifdef MMI_ON_WIN32
extern HWND g_hSimWnd;
	timerid->tmr_callback=func_ptr;
	timerid->tmr_callback_arg=funcArg;
	timerid->tmr_timeout=timeDurationMsec;
	timerid->tmr_handler=SetTimer(g_hSimWnd,(UINT)timerid,timeDurationMsec,NULL);
#endif
	return OSL_SUCCESS;
}


/*-------------------------------------------------------------------
Function Name	:	OslIntCancelTimer
Description		:	This module cancels the timer
Input			:	timerID
Output			:	bool
-------------------------------------------------------------------*/
OSLSTATUS OslIntCancelTimer(oslTimerid timerid)
{
#ifdef MMI_ON_WIN32
	extern HWND g_hSimWnd;
	KillTimer(g_hSimWnd,(UINT)timerid);
#endif
	return OSL_SUCCESS;
}


/*-------------------------------------------------------------------
Function Name	:	OslIntDeleteTimer 
Description		:	This module delets the timer
Input			:	timerID
Output			:	bool
-------------------------------------------------------------------*/
OSLSTATUS OslIntDeleteTimer (oslTimerid timerid)
{
#ifdef MMI_ON_WIN32
	OslMfree(timerid);
#endif
	return OSL_SUCCESS;
}

/*-------------------------------------------------------------------
Function Name	:	OslIntStartSoftTimer 
Description		:	This module starts the soft timer
Input			:	timerID
Output			:	bool
-------------------------------------------------------------------*/
OSLSTATUS OslIntStartSoftTimer (unsigned short nTimerId, 
							oslTimerFuncPtr func_ptr,
							void * funcArg,
							U32 timeDurationMsec)
{
#ifdef MMI_ON_WIN32
	oslSoftTimerid TempTimerId;
	U32 nRoundOffMSec;
	if(timeDurationMsec%TIMER_CALLBACK_RESOLUTION)
		nRoundOffMSec=((timeDurationMsec/TIMER_CALLBACK_RESOLUTION)+1)*TIMER_CALLBACK_RESOLUTION;
	else 
		nRoundOffMSec=timeDurationMsec;
	TempTimerId=OslMalloc(sizeof(win32_soft_timerid));
	TempTimerId->tmr_callback=func_ptr;
	TempTimerId->tmr_callback_arg=funcArg;
	TempTimerId->tmr_timeout=nRoundOffMSec;
	TempTimerId->tmr_count=TempTimerId->tmr_timeout;
	TempTimerId->tmr_state=OSL_RUN;
	EnterCriticalSection(&TimerCS);
	TimerId[nTimerId]=TempTimerId;
	LeaveCriticalSection(&TimerCS);
#endif
	return OSL_SUCCESS;

}

/*-------------------------------------------------------------------
Function Name	:	OslIntStopSoftTimer 
Description		:	This module stops the soft timer
Input			:	timerID
Output			:	bool
-------------------------------------------------------------------*/

OSLSTATUS OslIntStopSoftTimer (unsigned short nTimerId)
{
#ifdef MMI_ON_WIN32
	EnterCriticalSection(&TimerCS);
	if (TimerId[nTimerId] != NULL)
	{
		OslMfree(TimerId[nTimerId]);
		TimerId[nTimerId]=NULL;
	}
	LeaveCriticalSection(&TimerCS);
#endif
	return OSL_SUCCESS;
}
#endif   


