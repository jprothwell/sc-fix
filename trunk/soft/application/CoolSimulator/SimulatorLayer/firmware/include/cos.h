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
/*      cos.h                                                                 */
/*                                                                            */
/* DESCRIPTION                                                                */
/*      The COS API definitions on CSW.                                       */
/*      See CSW developement Manual for more.                                 */
/*                                                                            */
/******************************************************************************/

#ifndef _COS_H_
#define _COS_H_

#include <cswtype.h>

#define COS_MAX_PRIORITY       20 
#define COS_CREATE_DEFAULT     0
#define COS_CREATE_SUSPENDED   1
#define COS_CREATE_PREEMPT     2
#define COS_CREATE_NO_PREEMPT  3
typedef struct _TaskDesc
{
 void (*TaskBody)(void *);
 void (*TaskExit)(void *);
 const char *Name;
 UINT16 nStackSize;
 UINT8 nPriority;
}TaskDesc;

typedef struct _TASK_HANDLE
{
 TaskDesc sTaskDesc;
 UINT8 nTaskId;
 UINT8 nMailBoxId;
 UINT8 padding[4];
}TASK_HANDLE;


typedef enum  {
      COS_MMI_HEAP = 0x00,
      COS_WAP_HEAP = 0x01, 
    	COS_MDI_HEAP = 0x02, 
    	COS_MMS_HEAP = 0x03, 
     	COS_HEAP_COUNT 
}COS_HEAP_ID;

typedef enum{
    COS_PAGE_NUM_4      = 0x04,
    COS_PAGE_NUM_5      = 0x05,
}COS_PAGE_NUM;

typedef enum{
    COS_PAGE_NO_TRIGGER        =0,      /// No interrupt is triggered
	  COS_PAGE_READ_TRIGGER      =1,      /// Triggered on reading of the protected page
	  COS_PAGE_WRITE_TRIGGER     =2,      /// Triggered on writing of the protected page
	  COS_PAGE_READWRITE_TRIGGER =3       /// Triggered on reading or writing of the protected page
}COS_PAGE_SPY_MODE;

typedef VOID (*PTASK_ENTRY)(PVOID);

#define COS_MMI_TASKS_PRIORITY_BASE 220 // 220-250 is assigned to MMI part.
#define COS_PRIORITY_NUM            30

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
);

BOOL SRVAPI COS_DeleteTask(
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hTask
#else
  HANDLE_WIN32 hTask
#endif
//luanyy modify end
);

UINT32 COS_SuspendTask(
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hTask
#else
  HANDLE_WIN32 hTask
#endif
//luanyy modify end
);

BOOL COS_ResumeTask (
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hTask
#else
  HANDLE_WIN32 hTask
#endif
//luanyy modify end
);

typedef struct _COS_EVENT {
  UINT32 nEventId;
  UINT32 nParam1;
  UINT32 nParam2;
  UINT32 nParam3;
} COS_EVENT;

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
);

BOOL COS_FreeEvent(COS_EVENT* pEvent);

#define COS_WAIT_FOREVER  0xFFFFFFFF
#define COS_NO_WAIT       0x0

#define COS_EVENT_PRI_NORMAL   0
#define COS_EVENT_PRI_URGENT   1

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
);

BOOL SRVAPI COS_ResetEventQueue(
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hTask
#else
  HANDLE_WIN32 hTask
#endif
//luanyy modify end
);

BOOL SRVAPI COS_IsEventAvailable(
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hTask
#else
  HANDLE_WIN32 hTask
#endif
//luanyy modify end
);

#define COS_TIMER_MODE_SINGLE     0
#define COS_TIMER_MODE_PERIODIC   1

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
);

BOOL SRVAPI COS_KillTimer (
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hTask,
#else
  HANDLE_WIN32 hTask,
#endif
//luanyy modify end
  UINT8 nTimerId 
);

BOOL SRVAPI COS_Sleep (
  UINT32 nMillisecondes
);

UINT32 SRVAPI COS_GetTickCount( VOID); 

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
);

BOOL SRVAPI COS_ExitCriticalSection (
//luanyy modify start
#ifndef MMI_ON_WIN32
  HANDLE hSection
#else
  HANDLE_WIN32 hSection
#endif
//luanyy modify end
);

UINT32 SRVAPI COS_GetLastError(VOID);

VOID SRVAPI COS_SetLastError(
  UINT32 nErrCode   // per-thread error code
);

#ifdef _DEBUG
PVOID  COS_MallocDbg (UINT32 nSize, COS_HEAP_ID nHeapID, CONST UINT8* pszFileName, UINT32 nLine);
BOOL COS_FreeDbg (VOID* pMemAddr, CONST UINT8* pszFileName, UINT32 nLine);

#define COS_MALLOC(size) COS_MallocDbg((UINT32)size, COS_MMI_HEAP,__FILE__, __LINE__)
#define COS_FREE(p)      COS_FreeDbg((PVOID)p, __FILE__, __LINE__)
#else
#define COS_MALLOC(size) COS_Malloc((UINT32)size,COS_MMI_HEAP)
#define COS_FREE(p)      COS_Free((PVOID)p)
#endif



PVOID SRVAPI COS_Malloc (
  UINT32 nSize,
  COS_HEAP_ID nHeapID
);

BOOL SRVAPI COS_Free (
  PVOID pMemBlock
);


#define COS_SHMEMMALLOC(size) COS_Malloc((UINT32)size,COS_WAP_HEAP)
#define COS_SHMEMFREE(p)      COS_Free((PVOID)p)


//luanyy modify start
#ifndef MMI_ON_WIN32
HANDLE SRVAPI COS_CreateSemaphore (
#else
  HANDLE_WIN32 SRVAPI COS_CreateSemaphore (
#endif
//luanyy modify end
    UINT32  nInitCount  // Specify the initial count of the semaphore
);

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
    HANDLE hSem,     // Specify the handle to a counting semaphore 
#else
	HANDLE_WIN32 hSem,
#endif
//luanyy modify end
    UINT32 nTimeOut  // the time-out value
);

BOOL SRVAPI COS_ReleaseSemaphore(
//luanyy modify start
#ifndef MMI_ON_WIN32
    HANDLE hSem //Specify the counting semaphore
#else
  HANDLE_WIN32 hSem
#endif
//luanyy modify end
);
#ifdef MMI_ON_WIN32
VOID  COS_UpdateMemCallAdd( VOID *pMemeAddr, UINT32 nCallAdd);
#define COS_GET_RA(store)  
#endif

#endif // H 

