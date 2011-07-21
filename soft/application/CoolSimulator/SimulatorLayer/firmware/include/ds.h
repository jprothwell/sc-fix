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
/*      ds.h                                                                  */
/*                                                                            */
/* DESCRIPTION                                                                */
/*    Debug service declarations, constant definitions and macros             */
/*                                                                            */
/******************************************************************************/
#ifndef _DS_H_
#define _DS_H_
  
#ifdef __cplusplus
CPP_START
#endif

#include <ts.h>

//luanyy modify start
#ifndef MMI_ON_WIN32
UINT32 SRVAPI DS_FatalErr (
  PCSTR pszFileName,
  UINT32 nLineNum,
  PCSTR pszErrStr
); 
#endif
//luanyy modify end

//
// Redefine the Debug API to trace in default level.
// 
#if defined(DEBUG)
#if defined(_WIN32)
#define DS_ASSERT(x) if(!(x)) { TS_OutputText(0, "ASSERTION %s ", #x); __asm { int 3 }  }
#define DS_FATAL_ERR(x) if(!(x)) { TS_OutputText(0, "ASSERTION %s ", #x); __asm { int 3 }  }
#define DS_CHECKHANDLE(handle) if(!handle || !_CrtIsMemoryBlock(handle, sizeof(*handle), NULL, NULL, NULL)) { TS_OutLog(0, TS_ASSERT_LEVEL,"handle: %s is invalid", #handle); __asm { int 3 } }
#else // _WIN32
#define DS_CHECKHANDLE(handle)  if(!(handle)) {TS_OutputText(0, " >> HANDLE %s fail: %s, %s", (UINT32)handle,(UINT32)__FILE__, (UINT32)__LINE__); __asm__("break 5");}
#define DS_ASSERT(x) if (!(x)) \
	do { \
		TS_OutputText(0, "\n >> ASSERTION: %s \n",#x); \
		TS_OutputText(0, " >> File: %s \n",(UINT32)__FILE__); \
		TS_OutputText(0, " >> Function: %s \n",(UINT32)__FUNCTION__); \
		TS_OutputText(0,  " >> Line:%d \n\n",(UINT32)__LINE__); __asm__("break 5"); \
	} while(0) 
#endif
#else // DEBUG
#define DS_CHECKHANDLE(handle)      {	 }
#define DS_ASSERT(x)                {	 }
#endif // DEBUG

//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
CPP_END
#endif

#endif // _H_
