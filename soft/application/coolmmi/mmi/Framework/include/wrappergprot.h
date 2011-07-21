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
 *	WrapperGprot.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for timer wrapper.
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


#ifndef _WRAPPER_GPROT1_H
#define _WRAPPER_GPROT1_H

#include "mmi_data_types.h"
#include "cswtype.h"
#include "timerevents.h"
/***************************************************************************** 
* Define
*****************************************************************************/
#define TIMER_CALLBACK_RESOLUTION 100

#ifdef MMI_ON_HARDWARE_P
	#include "kal_release.h"
	#include "stack_common.h"
	#include "stack_msgs.h"
	#include "app_ltlcom.h"
	#include "lcd_ip_cqueue.h"
	#include "stack_types.h"
	#include "stack_ltlcom.h"
	#include "mmi_data_types.h"// simon add
#else
	#define OslCreateTimer	OslIntCreateTimer
	#define OslSetTimer		OslIntSetTimer
	#define OslCancelTimer	OslIntCancelTimer
	#define OslDeleteTimer	OslIntDeleteTimer
	#define osl_create_task		OslIntCreateTask
	#define osl_delete_task		OslIntDleteTask
	#define OslStartSoftTimer(nTimerId,TimerExpiry, nTimerId1, nTimeDuration,alignment)\
	      OslIntStartSoftTimer(nTimerId,TimerExpiry, (void *)nTimerId1, nTimeDuration)
	#define OslStopSoftTimer	OslIntStopSoftTimer
#endif

/***************************************************************************** 
* Typedef 
*****************************************************************************/

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
#ifdef MMI_ON_WIN32     
oslTaskid OslIntCreateTask (
								PS8 task_name,
								U8 priority,
								U32 stack_size,
								oslTaskFuncPtr entry_func,
								void * entry_param,
								U8 misc_options);
								
OSLSTATUS OslIntDleteTask(oslTaskid taskid);
oslTimerid OslIntCreateTimer(PS8 timer_name);
OSLSTATUS OslIntSetTimer(oslTimerid timerid, 
						 oslTimerFuncPtr func_ptr,
						 void * funcArg,
						 U32 timeDurationMsec);
OSLSTATUS OslIntCancelTimer(oslTimerid timerid);
OSLSTATUS OslIntDeleteTimer (oslTimerid timerid);
OSLSTATUS OslIntStartSoftTimer (unsigned short nTimerId, 
							oslTimerFuncPtr func_ptr,
							void * funcArg,
							U32 timeDurationMsec);
OSLSTATUS OslIntStopSoftTimer (unsigned short nTimerId);
#endif     
U16 StartMyTimerInt(U16 nTimerId,U32 nTimeDuration,oslTimerFuncPtr TimerExpiryFunction, U8 alignment);
U16 StartMyTimer(U16 nTimerId, U32 nTimeDuration, oslTimerFuncPtr TimerExpiryFunction);

U16 StopMyTimerInt(U16 nTimerId);		
U16 StopMyTimer(U16 nTimerId);
BOOL IsMyTimerExist(U16 nTimerId);

#endif

