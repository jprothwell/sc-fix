/******************************************************************************/
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    adp_Timer.h                                                         */
/*                                                                            */
/* Description:                                                               */
/*     timer management for MMI .                                            */
/*     @SHK 2006-04-04.                                                       */
/******************************************************************************/
#ifndef _ADP_TIMER_H
#define _ADP_TIMER_H

#include <csw.h>
#include "frameworkstruct.h"  // zrx add 200060612

void StartCosTimerInt(U16 nTimerId,U32 nTimeDuration,oslTimerFuncPtr TimerExpiryFunction, U8 alignment);
void StopCosTimerInt(U16 nTimerId);
void SetTimerHandler(U16 TimerID,FuncPtr funcPtr);
void ExecCurrTimerHandler(U16 TimerId);     //tangjie add 20060621
void ResumeAllNoAlignedCosTimer();
void SuspendAllNoAlignedCosTimer();
#ifndef MMI_ON_WIN32  //luanyy modify
#define OslStartSoftTimer(nTimerId,TimerExpiry, nTimerId1, nTimeDuration,alignment)\
		      StartCosTimerInt(nTimerId,nTimeDuration,TimerExpiry,alignment)
#define OslStopSoftTimer	StopCosTimerInt
#endif


#endif // _H_ 

