/******************************************************************************/
/*                                                                            */
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* This source code is property of Coolsand. The information contained in     */
/* this file is confidential. Distribution, reproduction, as well as          */
/* exploitation,or transmisison of any content of this file is not            */
/* allowed except if expressly permitted.                                     */
/* Infringements result in damage claims!                                     */
/*                                                                            */
/* FILE NAME                                                                  */
/*      ts_win32.c                                                                  */
/*                                                                            */
/* DESCRIPTION                                                                */
/*    Trace Service service declarations, constant definitions and macros     */
/*                                                                            */
/******************************************************************************/

#include "ts.h" //simon add



#ifndef HAVE_NO_TS_IF
UINT32 SRVAPI TS_SetOutputMask(UINT16 nIndex, BOOL bMode);






UINT32 SRVAPI TS_OutputText(UINT16 nModuleIdx, PCSTR pFormat, ...){}
VOID TS_TraceEnable(BOOL bEnable) ;
PCSTR SRVAPI TS_GetEventName(UINT32 nEvId){return "";}
PUBLIC UINT32 hal_TimGetUpTime(VOID)
{
    return 0;
}
//////////////////////////////////////////////////////////////////////////

//
// Redefine the compatible debug API to trace.
// 

#endif  //HAVE_NO_TS_IF

