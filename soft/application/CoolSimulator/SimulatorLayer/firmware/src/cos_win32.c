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
/*      cos_win32.c                                                               */
/*                                                                            */
/* DESCRIPTION                                                                */
/*      The COS API definitions on CSW.                                       */
/*      See CSW developement Manual for more.                                 */
/*                                                                            */
/******************************************************************************/



#include "cos.h"
#if 0
const kal_uint8 g_dt_day_per_mon[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

kal_uint8 applib_dt_is_leap_year(kal_uint16 year)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((year % 400) == 0)
    {
        return 1;
    }
    else if ((year % 100) == 0)
    {
        return 0;
    }
    else if ((year % 4) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif 
#ifndef MMI_ON_WIN32
HANDLE SRVAPI COS_CreateTask(
#else	
HANDLE_WIN32 SRVAPI COS_CreateTask(
#endif
  PTASK_ENTRY pTaskEntry,
  PVOID pParameter,
  PVOID pStackAddr,
  UINT16 nStackSize,
  UINT8 nPriority,
  UINT16 nCreationFlags,
  UINT16 nTimeSlice,
  PCSTR pTaskName
)
{
  return 0;
}

BOOL SRVAPI COS_DeleteTask(
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hTask
#else
  HANDLE_WIN32 hTask
#endif
//luanyy modify end
)
{
  return FALSE;
}

UINT32 COS_SuspendTask(
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hTask
#else
  HANDLE_WIN32 hTask
#endif
//luanyy modify end
)
{
  return 0;
}

BOOL COS_ResumeTask (
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hTask
#else
  HANDLE_WIN32 hTask
#endif
//luanyy modify end
)
{
  return FALSE;
}

BOOL SRVAPI COS_WaitEvent(
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hTask,
#else
  HANDLE_WIN32 hTask,
#endif
//luanyy modify end
  COS_EVENT* pEvent,
  UINT32 nTimeOut
){}

BOOL COS_FreeEvent(COS_EVENT* pEvent);


BOOL SRVAPI COS_SendEvent(
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hTask, 
#else
  HANDLE_WIN32 hTask,
#endif
//luanyy modify end
  COS_EVENT* pEvent,
  UINT32 nTimeOut,
  UINT16 nOption
){}

BOOL SRVAPI COS_ResetEventQueue(
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hTask
#else
  HANDLE_WIN32 hTask
#endif
//luanyy modify end
)
{
  return FALSE;
}

BOOL SRVAPI COS_IsEventAvailable(
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hTask
#else
  HANDLE_WIN32 hTask
#endif
//luanyy modify end
);


BOOL SRVAPI COS_SetTimer (
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hTask, 
#else
  HANDLE_WIN32 hTask,
#endif
//luanyy modify end
  UINT8 nTimerId,
  UINT8 nMode,
  UINT32 nElapse
){}

BOOL SRVAPI COS_KillTimer (
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hTask, 
#else
  HANDLE_WIN32 hTask,
#endif
//luanyy modify end
  UINT8 nTimerId 
){}

BOOL SRVAPI COS_Sleep (
  UINT32 nMillisecondes
){}

UINT32 SRVAPI COS_GetTickCount( VOID)
{
  return -1;
}

BOOL SRVAPI COS_SetPriority (
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hTask, 
#else
  HANDLE_WIN32 hTask,
#endif
//luanyy modify end
  UINT8 nPriority
);

//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE SRVAPI COS_EnterCriticalSection (
#else
  HANDLE_WIN32 SRVAPI COS_EnterCriticalSection (
#endif
//luanyy modify end
  VOID
)
{
  return 0;
}

BOOL SRVAPI COS_ExitCriticalSection (
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hSection
#else
  HANDLE_WIN32 hSection
#endif
//luanyy modify end
)
{
  return FALSE;
}

UINT32 SRVAPI COS_GetLastError(VOID);

VOID SRVAPI COS_SetLastError(
  UINT32 nErrCode   // per-thread error code
);

#ifdef _DEBUG
PVOID COS_MallocDbg (UINT32 nSize, CONST UINT8* pszFileName, UINT32 nLine)
{
  /*mmi_trace(TRUE,_T("%s,%d\n"),pszFileName,nLine);*/
  return malloc(nSize);
}

BOOL COS_FreeDbg (VOID* pMemAddr, CONST UINT8* pszFileName, UINT32 nLine)
{
  /*mmi_trace(TRUE,_T("%s,%d\n"),pszFileName,nLine);*/
  if (pMemAddr)
  {
    free(pMemAddr);
  }
  return TRUE;
}

#define COS_MALLOC(size) COS_MallocDbg((UINT32)size, __FILE__, __LINE__)
#define COS_FREE(p)      COS_FreeDbg((PVOID)p, __FILE__, __LINE__)
#else
#define COS_MALLOC(size) COS_Malloc((UINT32)size)
#define COS_FREE(p)      COS_Free((PVOID)p)
#endif

PVOID SRVAPI COS_Malloc (
  UINT32 nSize
)
{
  return malloc(nSize);
}

BOOL SRVAPI COS_Free (
  PVOID pMemBlock
)
{
  if (pMemBlock)
  {
    free(pMemBlock);
  }
  return TRUE;
}

//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE SRVAPI COS_CreateSemaphore (
#else
  HANDLE_WIN32 SRVAPI COS_CreateSemaphore (
#endif
//luanyy modify end
    UINT32  nInitCount  // Specify the initial count of the semaphore
){}

BOOL SRVAPI COS_DeleteSemaphore(
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hSem
#else
  HANDLE_WIN32 hSem
#endif
//luanyy modify end
);

BOOL SRVAPI COS_WaitForSemaphore(
//luanyy modify start
#ifndef MMI_ON_WIN32
	HANDLE hSem, // Specify the handle to a counting semaphore 
#else
	HANDLE_WIN32 hSem,
#endif
//luanyy modify end
    UINT32 nTimeOut  // the time-out value
){}

BOOL SRVAPI COS_ReleaseSemaphore(
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hSem //Specify the counting semaphore
#else
  HANDLE_WIN32 hSem
#endif
//luanyy modify end
){}

BOOL  SRVAPI COS_KillTimerEX (
  HANDLE hTask,
  UINT16 nTimerId 
)
{
  return FALSE;
}

BOOL SRVAPI COS_SetTimerEX (
  HANDLE hTask,
  UINT16 nTimerId,
  UINT8 nMode,
  UINT32 nElapse
)
{
  return FALSE;
}

VOID  COS_UpdateMemCallAdd( VOID *pMemeAddr, UINT32 nCallAdd)
{
  return;
}

INT32 SUL_Wcsicmp(const UINT16* cs, const UINT16* ct)
{

  return (*cs) - (*ct);

}
 
UINT32 COS_PageProtectDisable( COS_PAGE_NUM nPageNum )
{
	return 0;
}
UINT32 COS_PageProtectEnable( COS_PAGE_NUM nPageNum )
{
	return 0;
}
UINT32 COS_PageProtectSetup( COS_PAGE_NUM nPageNum, COS_PAGE_SPY_MODE nMode, UINT32 nStartAddr, UINT32 nEndAddr )
{
	return 0;
}

