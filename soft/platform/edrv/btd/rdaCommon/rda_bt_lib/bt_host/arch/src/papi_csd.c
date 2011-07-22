/***********************************************************************
 *
 * MODULE NAME:    papi_uitron.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    PAPI for UITRON port
 * MAINTAINER:     John Sheehy
 * CREATION DATE:  1 May 2000
 *
 * SOURCE CONTROL: $Id: papi_csd.c 4147 2010-11-30 10:03:46Z shug $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    1.May.2000 - JS - first import
 *   03.Jul.2000 - DA - Updated with timer/semaphores
 *
 *
 * ISSUES:
 *       
 ***********************************************************************/

#include "host_config.h"
#include "host_types.h"
#include "host_error.h"
#include "papi.h"
#include "mmi_trace.h"
typedef UINT32 MsgBody_t;
#include "itf_msg.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define PAPI_TIMER 0
#define PAPI_SEM 0

#ifndef SXR_CSP_TK_STCK_HEAP
#define SXR_CSP_TK_STCK_HEAP 0
#endif
#ifndef SXR_CSP_JB_STCK_HEAP
#define SXR_CSP_JB_STCK_HEAP 0
#endif
#ifndef SXR_CSP_JB_CTX_HEAP
#define SXR_CSP_JB_CTX_HEAP 0
#endif

#include "sxr_mem.h"
#include "sxr_mem.hp"

#define PAPI_EVENT_NUM		16
/*
static const char* LOGLAYER[]=
{
    "pLOGSYSTEM",
    "pLOGHCI",
    "pLOGMANAGER",
    "pLOGL2CAP",
    "pLOGRFCOMM",
    "pLOGSDP",
    "pLOGOBEX",
    "pLOGTCS",
    "pLOGA2DP",
    "pALWAYS"
};

static const char* LOGLEVEL[]=
{
    "pLOGFATAL",
    "pLOGCRITICAL",
    "pLOGERROR",	
    "pLOGWARNING",
    "pLOGNOTICE",
    "pLOGDEBUG"	
};

*/
/*
 */
/*************************************************************************
 * Type Definitions
 *************************************************************************/
typedef void                   *kal_semid;
typedef void                   *kal_mutexid;
typedef void                   *kal_timerid;
typedef char                    kal_char;
typedef unsigned char           kal_uint8;
typedef unsigned int            kal_uint32;
typedef void (* kal_timer_func_ptr)(void *);


typedef enum {
    KAL_NO_WAIT,
    KAL_INFINITE_WAIT
} kal_wait_mode;

typedef enum {
    KAL_SUCCESS,
    KAL_ERROR,
    KAL_Q_FULL,
    KAL_Q_EMPTY,
    KAL_SEM_NOT_AVAILABLE,
    KAL_WOULD_BLOCK,
    KAL_MESSAGE_TOO_BIG,
    KAL_INVALID_ID,
    KAL_NOT_INITIALIZED,
    KAL_INVALID_LENGHT,
    KAL_NULL_ADDRESS,
    KAL_NOT_RECEIVE,
    KAL_NOT_SEND,
    KAL_MEMORY_NOT_VALID,
    KAL_NOT_PRESENT,
    KAL_MEMORY_NOT_RELEASE
} kal_status;

typedef enum {
    KAL_SEM_GIVEN,
    KAL_SEM_TAKEN
} kal_sem_state;

typedef enum {
    KAL_MUTEX_GIVEN,
    KAL_MUTEX_TAKEN
} kal_mutex_state;

typedef enum
{
   TRACE_FUNC,
   TRACE_STATE,
   TRACE_INFO,
   TRACE_WARNING,
   TRACE_ERROR,
   TRACE_GROUP_1,
   TRACE_GROUP_2,
   TRACE_GROUP_3,
   TRACE_GROUP_4,
   TRACE_GROUP_5,
   TRACE_GROUP_6,
   TRACE_GROUP_7,
   TRACE_GROUP_8,
   TRACE_GROUP_9,
   TRACE_GROUP_10,
   TRACE_PEER
}trace_class_enum;

uint32 gCsdBtLogLevel=0;

#include "coolprofile.h"

void RDA_TBM_ENTRY(u_int32 id)
{
	TBM_ENTRY(id);
}

void RDA_TBM_EXIT(u_int32 id)
{
	TBM_EXIT(id);
}
int pInit(void *arg)
{
	gCsdBtLogLevel = pLOGERROR;
  return BT_NOERROR;
}

#if pDEBUG
u_int8 LogLayer_Encode(u_int8 loglayer)
{
    u_int8 i=0;
    if(loglayer==0)
        return 0;//error
    while(loglayer)
    {
         loglayer=loglayer>>1;
         i++;
    }        
    return (i-1);
    
}
#include "dbg.h"
#include "sxs_io.h"
int pDebugPrintEX(int logLevel,int logLayer, char *formatString, ... ) 
{
  va_list args;
  char buff[256];
  if (logLevel > gCsdBtLogLevel)
  	return 0;
  va_start(args,formatString);
  vsprintf(buff, formatString, args);
  va_end(args);
  //SXS_TRACE(1<<21,buff);
  SXS_TRACE(_SXR | TLEVEL(1), buff);		
  //SXS_TRACE(1<<21,"[%s]:%s:%s\n",LOGLEVEL[logLevel], LOGLAYER[LogLayer_Encode(logLayer)],buff);
  //dbg_TraceOutputText(0,"[%s]:%s:%s\n",LOGLEVEL[logLevel], LOGLAYER[LogLayer_Encode(logLayer)],buff2);
  return BT_NOERROR;
}


int pDebugPrint(int logLevel, char *formatString, ... )
{
  va_list args;
  char buff[256];
  if (logLevel > gCsdBtLogLevel)
  	return 0;
  va_start(args,formatString);
  vsprintf(buff, formatString, args);
  va_end(args);
  SXS_TRACE(_SXR | TLEVEL(1), buff);	
  //SXS_TRACE(1<<21,buff);
  //SXS_TRACE( 1<<21,"[%s]:%s:%s\n",LOGLEVEL[logLevel], LOGLAYER[LogLayer_Encode(9)],buff);
  //dbg_TraceOutputText(0,"[%s]:%s:%s\n",LOGLEVEL[logLevel], LOGLAYER[LogLayer_Encode(9)],buff2);
  return BT_NOERROR;
}

/*
int pDebugDumpFuncEX(int logLevel, int logLayer, u_int8 *buffer, u_int32 len) 
{
  return BT_NOERROR;
}

int pDebugDumpFunc(int logLevel, u_int8 *buffer, u_int32 len)
{
  return pDebugDumpFuncEX(logLevel,pLOGALWAYS,buffer,len);
}
*/
#endif
#include "hal_debug.h"
void rdabt_pageSpaySetup(u_int8 num,u_int8 trigger, u_int32 start_address, u_int32 end_address)
{
    hal_DbgPageProtectSetup (num, trigger,  
                        (UINT32)start_address, end_address);
}

void rdabt_pageSpayEnable(u_int8 num)
{
    hal_DbgPageProtectEnable (num);
}

void rdabt_pageSpayDisable(u_int8 num)
{
    hal_DbgPageProtectDisable(num);
}

#include "cos.h"
void *pMalloc(size_t size)
{
	  UINT32  nCallerAdd = 0x00;
	  COS_GET_RA(&nCallerAdd);

	if (size ==0)
	{
		mmi_trace(1,"#################pMalloc size = %p\n",size);
		return NULL;
	}
	
	void *p = COS_MALLOC(size);
	if (p == NULL)
	{
		mmi_trace(1,"#################pMalloc retuen address = %p\n",p);
		return NULL;
	}

	memset(p,0,size);
	  COS_UpdateMemCallAdd(p,nCallerAdd);

	//mmi_trace(1,"pMalloc retuen address = %p\n",p);
    return p;
}

#include "sxs_io.h"
/*
void checkMsg(void *_Ptr)
{
 	u32 Status;
 	sxr_HMemHead_t *HBlock = ((sxr_HMemHead_t *)_Ptr) - 1;
	 Status = sxr_EnterSc();

	 if (HBlock -> H.Index != SXR_HEAP_ALLOCATED)
	  SXS_RAISE ((_SXR|TNB_ARG(2)|TDB,TSTR("Not an allocated block 0x%x 0x%lxy\n",0x06a70006), _Ptr, HBlock -> H.CallerAdd));

	 if (!sxr_CheckSum (&HBlock -> H.CheckSum))
	  SXS_RAISE ((_SXR|TABORT|TDB|TNB_ARG(1),TSTR("Heap consistency failure %lx.\n",0x06a70007), _Ptr));

	 if (((u8 *)_Ptr) [(HBlock -> Size << 2) - HBlock -> H.DSize] != SXR_MEM_PATTERN)
	//  SXS_RAISE ((_SXR|TDB|TNB_ARG(2),TSTR("Memory deletion %lx offset %i\n",0x06a70008), _Ptr, HBlock -> Size));
	  SXS_RAISE ((_SXR|TABORT|TNB_ARG(3)|TDB,TSTR("Memory deletion (%lxy) %lx offset %i\n",0x06a70009), HBlock -> H.CallerAdd, _Ptr, HBlock -> Size));
	sxr_ExitSc(Status);
}
*/

#include "sxr_sbx.hp"

u8 checkIsNoEnv(void)
{
	return sxr_Sbx.EnvIdxFree == 0xffff;	 
}

u8 checkIsMemFreed(void *ptr)
{
 	sxr_HMemHead_t *HBlock = ((sxr_HMemHead_t *)ptr) - 1;
 	if (HBlock -> H.Index != SXR_HEAP_ALLOCATED)
 	{
 		return 0;
 	}
	return 1;
}


void pFree(void *buffer)
{
    //mmi_trace(1,"pFree retuen address = %p\n",buffer);
    if (0)//(!checkIsMemFreed(buffer))
    {
	mmi_trace(1,"#################pFree address has been freed = %p\n",buffer);
	return;
    }
    COS_FREE(buffer);
    //_sxr_HFree(&buffer);
    //sxr_Free(buffer);
}

void *pMemcpy(void *dest,  void *src, size_t n)
{
    return memcpy(dest,src,n);
}

void *pMemset(void *s, int c, size_t n)
{
    return memset(s,c,n);
}

int pMemcmp(void *s1, void *s2, size_t n)
{
  return memcmp(s1, s2, n);
}

size_t pStrlen(const char *s)
{
  return strlen(s);
}

int pStrncmp(const char *s1, const char *s2, size_t n)
{
    return strncmp(s1,s2,n);
}

int pStrcmp(const char *s1, const char *s2)
{
  return strcmp(s1,s2);
}

char *pStrcpy(char *s1, const char *s2)
{
  return strcpy(s1,s2);
}

char *pStrncpy(char *s1, const char *s2, size_t n)
{
    return strncpy(s1,s2,n);
}
/*
 * PAPI EXIT FUNCTION
 */ 
void pExit(int x)
{
  
    /* There's probably a nice cygnus call for it */
  
}

/* 
 * PAPI DEBUGGING SECTION
 */
#if pDEBUG

void pDebugCheck(void)
{
  
}
#endif
/*
 * PAPI SEMAPHORE SECTION
 */

#include "ds.h"

pSem_t pSemCreate(unsigned int initval)
{
	ASSERT(0);
    //return (pSem_t)kal_create_sem(NULL, initval);
    return 0;
}

void pSemFree(pSem_t sem)
{
	ASSERT(0);
    //???
}


int pSemLock(pSem_t sem, int flags)
{
	ASSERT(0);
  //return kal_take_sem((kal_semid)sem, (kal_wait_mode)flags);
  	return 0;
}


int pSemUnlock(pSem_t sem)
{
	ASSERT(0);
  //kal_give_sem((kal_semid)sem);
  return 0;
}

/*
 * PAPI TIMER SECTION
 */
#include "sxr_sbx.h"
#include "stack_config.h"
#include "mmitask.h"

#include "sxr_tim.h"

BT_TIMER_EVT bt_timer_evt[MAX_BT_TIMER]={{0}};
 /*

 #define SXR_REGULAR_TIMER          0

void _sxr_StartFunctionTimer (u32 Period, void (*Function)(void *), void *Param, u16 Id, u8 I2d);


#define sxr_StartFunctionTimer(Period, Function, Param, Id) \
 _sxr_StartFunctionTimer ((u32)Period, (void (*)(void *))Function, (void *)Param, (u16)Id, SXR_REGULAR_TIMER)

#define sxr_StopFunctionTimer(Function) \
 _sxr_StopFunctionTimer ((void (*)(void *))Function, SXR_REGULAR_TIMER)
*/
t_pTimer pTimerCreate(u_int32 timeoutTenths, void(*callbackFunc)(void *), void *callbackArg, u_int8 flags) 
{
    u_int32 i;
    u_int32 CallerAdd;
    SXS_GET_RA(&CallerAdd);
    //mmi_trace(1,"pTimerCreate -- callbackFunc = 0x%x, CallerAdd = 0x%x\n",callbackFunc,CallerAdd);
    for (i=0;i<MAX_BT_TIMER;i++)
    {
    	if (bt_timer_evt[i].callbackfun == NULL)
			break;
    }
    if (i>= MAX_BT_TIMER)
    {
    		mmi_trace(1,"No BT timer evt have to be alloc!!!!!!!!!!!!!111\n");
		return 0;
    }
    bt_timer_evt[i].callbackfun = callbackFunc;
    bt_timer_evt[i].argv = callbackArg;
    bt_timer_evt[i].timerHandle= i+1;
    //kal_timerid timer = (kal_timerid)callbackFunc;
    TASK_HANDLE * hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_BT);
    //sxr_StartFunctionTimer(((timeoutTenths<<14)>>3),callbackFunc,callbackArg,0); //timeoutTenths is tenth time,ie 30 -> 3 s
    sxs_StartTimer(((timeoutTenths<<14)/10), BT_TIMER_PAPI+i+1, &bt_timer_evt[i], TRUE, hTask->nMailBoxId);

     //mmi_trace(0,"pTimerCreate -- callbackFunc = 0x%x, timerHandle=%d, CallerAdd = 0x%x,cuttrent ticks=%d\n",callbackFunc,i+1,CallerAdd,hal_TimGetUpTime());

    return (t_pTimer)(i+1);
}

t_pTimer pTimerCreateMs(u_int32 timeoutTenths, void(*callbackFunc)(void *), void *callbackArg, u_int8 flags) 
{
    u_int32 i;
    u_int32 CallerAdd;
    SXS_GET_RA(&CallerAdd);
    //mmi_trace(1,"pTimerCreate -- callbackFunc = 0x%x, CallerAdd = 0x%x\n",callbackFunc,CallerAdd);
    for (i=0;i<MAX_BT_TIMER;i++)
    {
    	if (bt_timer_evt[i].callbackfun == NULL)
			break;
    }
    if (i>= MAX_BT_TIMER)
    {
    		mmi_trace(1,"No BT timer evt have to be alloc!!!!!!!!!!!!!111\n");
		return 0;
    }
    bt_timer_evt[i].callbackfun = callbackFunc;
    bt_timer_evt[i].argv = callbackArg;
    bt_timer_evt[i].timerHandle= i+1;
    //kal_timerid timer = (kal_timerid)callbackFunc;
    TASK_HANDLE * hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_BT);
    //sxr_StartFunctionTimer(((timeoutTenths<<14)),callbackFunc,callbackArg,0); //timeout ms
    sxs_StartTimer(((timeoutTenths<<14)/1000), BT_TIMER_PAPI+i+1, &bt_timer_evt[i], TRUE, hTask->nMailBoxId);

     //mmi_trace(0,"pTimerCreate -- callbackFunc = 0x%x, timerHandle=%d, CallerAdd = 0x%x,cuttrent ticks=%d\n",callbackFunc,i+1,CallerAdd,hal_TimGetUpTime());

    return (t_pTimer)(i+1);
}

int pTimerSet(unsigned int timeout, void(*callbackFunc)(void *), void *callbackParm, int *timerHandle)
{
    *timerHandle = pTimerCreate(timeout, callbackFunc, callbackParm, pTIMER_ONESHOT);
    return *timerHandle;
}

int pTimerCancel(t_pTimer timerHandle)
{
    u_int32 CallerAdd;
    SXS_GET_RA(&CallerAdd);
    if (timerHandle == 0)
		return 0;
    if (bt_timer_evt[(u_int32)timerHandle-1].timerHandle != timerHandle)
    {
    	mmi_trace(0,"pTimerCancel assert -- callbackFunc =0x%x,timerHandle =%d, bt_timer_evt[(u_int32)timerHandle-1].timerHandle=%d,CallerAdd = 0x%x,current ticks=%d\n",bt_timer_evt[timerHandle-1].callbackfun,timerHandle,bt_timer_evt[(u_int32)timerHandle-1].timerHandle,CallerAdd,hal_TimGetUpTime());
    }
    if (bt_timer_evt[(u_int32)timerHandle-1].timerHandle == 0)
		return 0;
    //mmi_trace(0,"pTimerCancel -- callbackFunc =0x%x,timerHandle =%d, CallerAdd = 0x%x,current ticks=%d\n",bt_timer_evt[timerHandle-1].callbackfun,timerHandle,CallerAdd,hal_TimGetUpTime());
    TASK_HANDLE * hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_BT);
    sxs_StopTimer(BT_TIMER_PAPI+timerHandle,&bt_timer_evt[timerHandle-1],hTask->nMailBoxId);
    bt_timer_evt[(u_int32)timerHandle-1].callbackfun = NULL;
    bt_timer_evt[(u_int32)timerHandle-1].argv = NULL;
    bt_timer_evt[(u_int32)timerHandle-1].timerHandle= 0;//-1;
    //sxr_StopFunctionTimer((void(*)(void *))timerHandle);
    return 0;
}

BOOL pIsTimerCreated(t_pTimer timerHandle)
{
	mmi_trace(1,"pIsTimerCreated is called timerHandle=%d",timerHandle);
	if (bt_timer_evt[(u_int32)timerHandle-1].callbackfun == NULL)
    	return FALSE;
    else 
	return TRUE;
}
t_pMutex pMutexCreate(u_int8 initialState)
{
//	return (t_pMutex)kal_create_mutex(NULL);
        return 0;

}

int pMutexLock(t_pMutex mutex)
{
	//kal_take_mutex((kal_mutexid)mutex);
    return 0;
}

int pMutexUnlock(t_pMutex mutex)
{
//	kal_give_mutex((kal_mutexid)mutex);
    return 0;
}

void pMutexFree(t_pMutex mutex)
{
    // ??
}

struct t_eventInfo {	
    int (*callbackFunc)(void *);
    void *callbackParam;
    u_int8 used;
    u_int8 state;
    u_int8 hand;
} papiEvents[PAPI_EVENT_NUM];

// event handle start at 1
t_pEvent pEventCreate(u_int8 initialState)
{
    u_int8 i;
//    u_int32 status;
    //status= sxr_EnterSc();
    // find unused event
    for(i=0; i<PAPI_EVENT_NUM; i++)
        if(papiEvents[i].used == FALSE)
            break;
    if(i<PAPI_EVENT_NUM)
    {
        papiEvents[i].used = TRUE;
        papiEvents[i].state = initialState;
	 papiEvents[i].callbackFunc = NULL;
    }
    //sxr_ExitSc(status);
    //mmi_trace(1,"pEventCreate at %d, event = %d \n",hal_TimGetUpTime(),i);
   //papiEvents[i].hand = sxr_NewSemaphore (initialState);
    return i+1;
}

int pEventWait(t_pEvent event, int (*callbackFunc)(void *), void *callbackParam)
{
 //   u_int32 status;
    u_int32 CallerAdd;	
    SXS_GET_RA(&CallerAdd);
    //mmi_trace(1,"pEventWait event=%d, CallerAddr = 0x%x\n",event,CallerAdd);
    event --;
    if(event>=PAPI_EVENT_NUM)
        return TRUE;
    //status= sxr_EnterSc();
    if(papiEvents[event].used == FALSE)
     {
     	//sxr_ExitSc(status);
     	return TRUE;
    }
    papiEvents[event].callbackFunc = callbackFunc;
    papiEvents[event].callbackParam = callbackParam;
    //sxr_ExitSc(status);
    //mmi_trace(1,"function %s:line %d,pEventWait at %d, event = %d \n",__FUNCTION__,__LINE__,hal_TimGetUpTime(),event);
    //sxr_TakeSemaphore(papiEvents[event].hand);
    return papiEvents[event].state;
}

//#define sxr_HMalloc(a)        _sxr_HMalloc(a,0);

int pEventSet(t_pEvent event)
{
//   u_int32 a;
//   Msg_t *msg;
   u_int32 CallerAdd;
   u_int32 tmp;
   SXS_GET_RA(&CallerAdd);
   TASK_HANDLE * hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_BT);
   tmp = (u_int32)hTask;
   //mmi_trace(1,"pEventSet at %d,event=%d,hTask=0x%x papiEvents[event].hand=%d, CallerAddr = 0x%x\n",hal_TimGetUpTime(),event,hTask,papiEvents[event].hand,CallerAdd);
   if (event == 0)
   	asm("break 1");
    event --;
    if(event>=PAPI_EVENT_NUM)
        return FALSE;
    //a = sxr_EnterSc();
    if(papiEvents[event].used == FALSE)
    {
    	//sxr_ExitSc(a);
   	return FALSE;
    }
    papiEvents[event].state = TRUE;
    //sxr_ExitSc(a);
    #if 0
    sxr_ReleaseSemaphore(papiEvents[event].hand);
    if (tmp != (u_int32)hTask)
		asm("break 1");
    mmi_trace(1,"pEventSet after sxr_ReleaseSemaphore event=%d,hTask=0x%x\n",event,hTask);
    msg = (Msg_t *)sxr_HMalloc(sizeof (Msg_t));
    msg->H.Id = BT_EVENT_SET;
    msg->B = event;
    mmi_trace(1,"pEventSet before sxr_Send event=%d,hTask=0x%x\n",event,hTask);
    sxr_Send(msg,hTask->nMailBoxId,SXR_SEND_MSG);
#endif
	return TRUE;
}

void pEventFree(t_pEvent event)
{
//    u_int32 a;
    event --;
    if(event<PAPI_EVENT_NUM)
    {
        //a = sxr_EnterSc();
        papiEvents[event].used = FALSE;
        papiEvents[event].callbackFunc = NULL;
       //sxr_ExitSc(a);
    }
    //mmi_trace(1,"pEventFree at %d, event = %d \n",hal_TimGetUpTime(),event);
   // sxr_FreeSemaphore(papiEvents[event].hand);
    papiEvents[event].hand = 0;
}


void papiSched(unsigned int dummy)
{
    u_int8 i;
//    u_int32 a;
    for(i=0; i<PAPI_EVENT_NUM; i++)
    {
        //a = sxr_EnterSc();
        if(papiEvents[i].used == TRUE)
        {
    	     //mmi_trace(1,"papiSched at %d, event = %d \n",hal_TimGetUpTime(),i);
            if(papiEvents[i].state == TRUE && papiEvents[i].callbackFunc != NULL)
            {
                papiEvents[i].state = FALSE;
    	     	  mmi_trace(1,"papiSched at %d, event = %d  callbackfuc = 0x%x\n",hal_TimGetUpTime(),i,papiEvents[i].callbackFunc);
		  //sxr_ExitSc(a);
                papiEvents[i].callbackFunc(papiEvents[i].callbackParam);
            }
	     else
		 	;//sxr_ExitSc(a);
        }
	else
		;//sxr_ExitSc(a);
    }
}

void pShutdown(void)
{
	u_int8 i;
	memset(papiEvents,0,sizeof(papiEvents));
       for (i=0;i<MAX_BT_TIMER;i++)
       {
       	pTimerCancel(bt_timer_evt[i].timerHandle);
       }
	memset(bt_timer_evt,0,sizeof(bt_timer_evt));
}

