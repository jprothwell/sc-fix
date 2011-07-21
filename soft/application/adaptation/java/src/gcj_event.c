#include "stddef.h"
#include "stdarg.h"
#include "cos.h"
#include "cswtype.h"
#include "fs_type.h"
#include "tm.h"
#include "mmi_trace.h"
#include "sxr_mem.h"
#include "internal.h"


#include "sxr_sbx.h"

typedef GCJ_BODY_MSG_T MsgBody_t;

#include "itf_msg.h"

#include "sxr_tim.h"
/*
int sigprocmask(int c, const void * a, void * b)
{
	asm ("break 1");
}
*/
#include "stack_config.h"

typedef void (*sighandler_t)(void);

void sxr_GcjInitSignalHandle();
void sxr_GcjSetBaseSbx(u8 Sbx);
void sxr_GcjRegSignalHandle(u8 Sbx,int sig,sighandler_t handle);

HANDLE GetMmiTaskHandle(module_type mod);


unsigned char getMainThreadMbox()
{
    TASK_HANDLE * hTask;
    hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_J2ME);
    return hTask->nMailBoxId;
}
unsigned char getMainThreadTskId()
{
    TASK_HANDLE * hTask;
    hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_J2ME);
    return hTask->nTaskId;
}

extern unsigned char _Jv_ThreadGetMailBoxId(unsigned char threadId);
COND_WAIT_RET_T gcj_StubCondWait(unsigned char mailBoxId,unsigned int timeout);

unsigned char getTaskMboxId(unsigned char taskId)
{
	return _Jv_ThreadGetMailBoxId(taskId);
}


unsigned char gcj_TaskGetMailBoxId(unsigned char threadId)
{
    TASK_HANDLE * hTask;
    hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_MMI);
    if (hTask->nTaskId == threadId)
		return hTask->nMailBoxId;
    hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_L4C);
    if (hTask->nTaskId == threadId)
		return hTask->nMailBoxId;
    hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_MED);
    if (hTask->nTaskId == threadId)
		return hTask->nMailBoxId;
    hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_USB);
    if (hTask->nTaskId == threadId)
		return hTask->nMailBoxId;
    hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_BT);
    if (hTask->nTaskId == threadId)
		return hTask->nMailBoxId;
    return 0;
}

void sleep(unsigned int misec)
{
	//abort();
	COS_Sleep(misec);
}
void gc_sleep(unsigned int misec)
{
	#if 1
	COS_Sleep(misec);
	#else
	unsigned char taskId = sxr_GetCurrentTaskId();
	unsigned char mailBoxId = getTaskMboxId(taskId);
	gcj_StubCondWait(mailBoxId,misec);
	#endif
	
}

void gcj_StubThreadSleep(unsigned long long millis)
{
	gcj_TraceOut(0,"gcj_StubThreadSleep taskid =%d,millis=%d\n",sxr_GetCurrentTaskId(),(unsigned int)millis);
	unsigned char taskId = sxr_GetCurrentTaskId();
	unsigned char mailBoxId = getTaskMboxId(taskId);
	COND_WAIT_RET_T ret;
retry:	ret = gcj_StubCondWait(mailBoxId,millis);
#ifndef USE_SYS_EVENT
		if (ret == EKILL)
			Jv_ThreadKillCurrent();
		while (ret != ENORMAL &&  ret != ETIMEDOUT)
			goto retry;
#endif		
}

void gcj_StubThreadYield()
{
	gcj_StubThreadSleep(200);
}

unsigned char gcj_StubNewSemphone()
{
	unsigned char taskId = sxr_GetCurrentTaskId();
	unsigned char mailBoxId = getTaskMboxId(taskId);
	return mailBoxId;
}
unsigned char gcj_StubTakeSemphone(unsigned char SemId)
{
#ifdef USE_SYS_EVENT
	gcj_StubCondWait(SemId,0);
#else
	COND_WAIT_RET_T ret;
	while ((ret =  gcj_StubCondWait(SemId,0)) != ESEMAPHONE)
	{
		gcj_TraceOut(0,"gcj_StubTakeSemphone SemId=%d,got ret=%d\n",SemId,ret);
		if (ret == EKILL)
			Jv_ThreadKillCurrent();
	}
#endif	
}

void gcj_StubReleaseSemphone(unsigned char SemId)
{
	gcj_StubCondNotifySemphone(SemId);
}

void gcj_StubInitMutex(_GCJ_MUTEX_T *mutex)
{
#ifndef USE_SYS_MUTEX
	memset(mutex,0,sizeof(_GCJ_MUTEX_T));
#else
	mutex->id = sxr_NewMutex();
#endif

}

sighandler_t gcj_pthread_handle=NULL;


void  GC_disable(void);
void  GC_enable(void);
#ifdef __mips16
#define COS_GET_RA(store) asm volatile("move $2, $31\n\tsw $2, 0(%0)"::"r"((store)):"$2")
#else
#define COS_GET_RA(store) asm volatile("sw $31, 0(%0)"::"r"((store)))
#endif

void gcj_StubTakeMutex(_GCJ_MUTEX_T *mutex)
{
#ifndef USE_SYS_MUTEX
	unsigned int nCallerAdd;
	COS_GET_RA(&nCallerAdd);
	unsigned char tmid = sxr_GetCurrentTaskId();
	//gcj_TraceOut(0,"gcj_StubTakeMutex current taskid =%d, index =%d,sign=%d\n",sxr_GetCurrentTaskId(),mutex->index,mutex->sign);
	int status = sxr_EnterSc();
	unsigned char sign = mutex->sign;
	mutex->sign = 1;
	if (sign)
	{
		unsigned char taskId = sxr_GetCurrentTaskId();
		unsigned char index = mutex->index;
		if (taskId == mutex->taskList[mutex->lockedId].taskId)
		{
			mutex->lockCount++;
			sxr_ExitSc(status);
			return;
		}
		int space=0;
		for (space=0;space<=index;space++)
		{
			if (mutex->taskList[space].taskId == 0)
				break;
		}
		
		if (space >= MAX_THREAD)
			abort();
		for (int i=0;i<index;i++)
		{
			if (mutex->taskList[i].taskId== taskId)
				abort();
		}
		mutex->taskList[space].taskId = taskId;;
		mutex->taskList[space].callAddr = nCallerAdd;
		if (space>index)
			mutex->index++;
		unsigned mailBox = getTaskMboxId(taskId);
		int ret;
		int isKill=0;
		while ( (ret =gcj_StubCondWait(mailBox,0)) != EMUTEX)
		{
			if (ret == EKILL)
			{
				gcj_TraceOut(1,"current Task =%d,Got a KILL",taskId);
				isKill = 1;
			}
		}
		mutex->lockedId = space;
		mutex->lockCount++;	
		mutex->isKill = isKill;
		sxr_ExitSc(status);
	}
	else
	{
		unsigned char taskId = sxr_GetCurrentTaskId();
		unsigned char index = mutex->index;
		if (index !=0)
			abort();
		if (mutex->taskList[index].taskId !=0)
			abort();
		mutex->taskList[index].taskId = taskId;;
		mutex->taskList[index].callAddr = nCallerAdd;
		mutex->lockedId = index;
		mutex->lockCount++;
		mutex->index++;
		sxr_ExitSc(status);
	}
#else
	mutex->taskId = sxr_TakeMutex(mutex->id);
#endif
}

void gcj_StubReleaseMutex(_GCJ_MUTEX_T *mutex)
{
#ifndef USE_SYS_MUTEX
	unsigned char tmid = sxr_GetCurrentTaskId();
	unsigned char isKill=0;
	//gcj_TraceOut(0,"gcj_StubReleaseMutex current taskid =%d, index =%d,sign=%d\n",sxr_GetCurrentTaskId(),mutex->index,mutex->sign);
	int status = sxr_EnterSc();
	//mutex->index--;
	if (mutex->sign)
	{
		//if (mutex->index>0)
		{
			unsigned char taskId = sxr_GetCurrentTaskId();
			unsigned char nextTsk=0;
			unsigned char index = mutex->index;
			/*
			for (int i=0;i<=index;i++)
			{
				if (taskId == mutex->taskList[i].taskId)
				{
					mutex->taskList[i].taskId = 0;
					break;
				}
			}
			*/
			mutex->lockCount--;
			if (mutex->lockCount != 0)
			{
				sxr_ExitSc(status);
				return;
			}
			if (mutex->lockCount <0)
				abort();
			if (mutex->taskList[mutex->lockedId].taskId != taskId)
				abort();
			mutex->taskList[mutex->lockedId].taskId = 0;
			mutex->taskList[mutex->lockedId].callAddr = 0;
			isKill = mutex->isKill;
			mutex->isKill = 0;
			for (int i=0;i<=index;i++)
			{
				if (mutex->taskList[i].taskId!=0)
				{
					nextTsk = mutex->taskList[i].taskId;
					break;
				}
			}
			if (nextTsk == 0)
			{
				mutex->index=0;
				mutex->sign=0;
				mutex->lockedId = 0;
				mutex->lockCount= 0;
				mutex->isKill = 0;
				mutex->taskList[mutex->lockedId].taskId = 0;
				mutex->taskList[mutex->lockedId].callAddr = 0;
			}
			else
			{
				 unsigned mailBox = getTaskMboxId(nextTsk);
				gcj_StubCondNotifyMutex(mailBox);
			}
		}
	}
	else
	{
		gcj_TraceOut(0,"Waring curretn taskid =%d, gcj_StubReleaseMutex index =%d\n",sxr_GetCurrentTaskId(),mutex->index);
		abort();
	}
	if (isKill && !mutex->killTaskId)
	{
		mutex->killTaskId = sxr_GetCurrentTaskId();		
		Jv_ThreadKillCurrent();
		mutex->killTaskId = 0;		
		sxr_ExitSc(status);
	}
	else		
		sxr_ExitSc(status);
#else
	sxr_ReleaseMutex(mutex->id,mutex->taskId);
#endif	
}

unsigned char checkIsCurrentWaitInMutex(_GCJ_MUTEX_T *mutex)
{
	unsigned char currentTaskId = sxr_GetCurrentTaskId();
	int status = sxr_EnterSc();
	if (mutex->sign)
	{
		if (mutex->taskList[mutex->lockedId].taskId != currentTaskId)
		{
			unsigned char index = mutex->index;
			for (int i=0;i<=index;i++)
			{
				if (currentTaskId == mutex->taskList[i].taskId)
				{
					sxr_ExitSc(status);
					return TRUE;
				}
			}
		}
	}
	sxr_ExitSc(status);
	return FALSE;
}

extern unsigned char checkIsCurrentWaitInGcMutex();
extern unsigned char checkIsCurrentWaitInLibMutex();
extern _GCJ_MUTEX_T memMutex;
extern _GCJ_MUTEX_T fileMutex;
extern _GCJ_MUTEX_T netMutex;


unsigned char checkIsCurrentWaitInMutexAll()
{
	if (checkIsCurrentWaitInGcMutex())
		return TRUE;
	if (checkIsCurrentWaitInLibMutex())
		return TRUE;
	if (checkIsCurrentWaitInMutex(&memMutex))
		return TRUE;
	if (checkIsCurrentWaitInMutex(&fileMutex))
		return TRUE;
	if (checkIsCurrentWaitInMutex(&netMutex))
		return TRUE;
	return FALSE;
}
void gcj_StubFreeMutex(_GCJ_MUTEX_T *mutex)
{
#ifndef USE_SYS_MUTEX
#else
	sxr_FreeMutex(mutex->id);
#endif
}

sighandler_t signal(int signum, sighandler_t handler)
{
	sighandler_t ret = gcj_pthread_handle;
	//gcj_TraceOut(1,"stub:task id=%d,current function=%s,signum=%d,handler=0x%x\n",sxr_GetCurrentTaskId(),__FUNCTION__,signum,handler);
	if (signum == GCJ_MSG_PTHREAD_SIG)
		gcj_pthread_handle = handler;
	else
		;//abort();
	return ret;
}

#define pthread_t unsigned char
unsigned char test_gc=0;
void sxr_RegisterSignalHandle(unsigned char mboxId,int sig);
int pthread_kill(pthread_t thread, int sig)
{
	unsigned char mailBoxId = getTaskMboxId(thread);	
	unsigned char cmailBoxId = 0;//sxr_GetSbxForTaskWating(thread);
	if (cmailBoxId !=0 && mailBoxId != cmailBoxId)
	{
		gcj_TraceOut(0,"pthread_kill Waring current task id=%d, thread id =%d,thread state =%d,mailBoxId=%d,CmailBoxId=%d\n",sxr_GetCurrentTaskId(),thread,sxr_GetTaskState(thread),mailBoxId,cmailBoxId);
		mailBoxId = cmailBoxId;
	}
	if (mailBoxId == 0)
		return 1;
	Msg_t *msg;
	msg 			 = (Msg_t*) sxr_Malloc(sizeof(Msg_t));
	msg->H.Id		 = 0;
	msg->B			 = GCJ_MSG_PTHREAD_SIG;
	gcj_TraceOut(0,"pthread_kill current task id=%d, thread id =%d,taskMbox=%d,cmailBoxId=%d,taskState=%d\n",sxr_GetCurrentTaskId(),thread,mailBoxId,cmailBoxId,sxr_GetTaskState(thread));
	sxr_RemoveCustMsgFromMbox(mailBoxId,GCJ_MSG_PTHREAD_SIG,0);
//	sxr_GcjRegSignalHandle(mailBoxId,GCJ_MSG_PTHREAD_SIG,gcj_pthread_handle);
	sxr_Send(msg,mailBoxId,SXR_SEND_MSG|SXR_QUEUE_FIRST);
	return 0;
}
int GC_isNeedToSuspend();
void Jv_ThreadKillCurrent();

void gcj_ThreadDestroy(unsigned char thread)
{
	unsigned char mailBoxId = getTaskMboxId(thread);
	Msg_t *msg;
	msg 			 = (Msg_t*) sxr_Malloc(sizeof(Msg_t));
	msg->H.Id		 = 0;
	msg->B			 = GCJ_MSG_GCJ_KILL;
	gcj_TraceOut(0,"gcj_ThreadDestroy current task id=%d, thread id =%d,taskMbox=%d\n",sxr_GetCurrentTaskId(),thread,mailBoxId);
//	sxr_GcjRegSignalHandle(mailBoxId,GCJ_MSG_GCJ_KILL,Jv_ThreadKillCurrent);
	sxr_Send(msg,mailBoxId,SXR_SEND_MSG|SXR_QUEUE_FIRST);
}
extern unsigned char QueueEventInputMbox;

void CondWaitTimeOut(unsigned char mailBox)
{
	if (mailBox == QueueEventInputMbox)
		gcj_TraceOut(0,"CondWaitTimeOut task id=%d taskMbox=%d\n,ticks=%d",sxr_GetCurrentTaskId(),mailBox,csw_TMGetTick());
	Msg_t *msg;
	msg 			 = (Msg_t*) sxr_Malloc(sizeof(Msg_t));
	msg->H.Id		 = 0;
	msg->B			 = GCJ_MSG_COND_WAIT_TIMEOUT;
	sxr_Send(msg,mailBox,SXR_SEND_MSG);
	
	//sxr_ReleaseSemaphore(mailBox);
}


COND_WAIT_RET_T gcj_StubCondWait(unsigned char mailBoxId,unsigned int timeout)
{
	COS_EVENT ev;
	Msg_t *Msg=NULL;
	
	sxr_StopFunctionTimer2(CondWaitTimeOut,mailBoxId);
	//gcj_TraceOut(0,"gcj_StubCondWait task id=%d taskMbox=%d,wiat time=%d ms,ticks=%d\n",sxr_GetCurrentTaskId(),mailBoxId,timeout,csw_TMGetTick());
	if (timeout !=0)
		sxr_StartFunctionTimer(16384*timeout/1000,CondWaitTimeOut,mailBoxId,0);
	memset(&ev,0,sizeof(ev));
	Msg = sxr_Wait(&ev,mailBoxId);
	//sxr_TakeSemaphore(mailBoxId);
	sxr_StopFunctionTimer2(CondWaitTimeOut,mailBoxId);
	if (Msg != NULL)
	{
		//gcj_TraceOut(0,"gcj_StubCondWait task id=%d taskMbox=%d,wiat time=%d ms,get msg id=%d,GC_isNeedToSuspend=0x%x\n",sxr_GetCurrentTaskId(),mailBoxId,timeout,Msg->B,GC_isNeedToSuspend());
		if (Msg->B == GCJ_MSG_COND_WAIT)
		{
#ifdef USE_SYS_EVENT
#else
		
			if (GC_isNeedToSuspend())
			{
				if (gcj_pthread_handle)
					gcj_pthread_handle(); 		
			}
#endif			
			sxr_Free(Msg);
			return ENORMAL;
		}
		else if (Msg->B == GCJ_MSG_COND_WAIT_TIMEOUT)
		{
#ifdef USE_SYS_EVENT
#else
			if (GC_isNeedToSuspend())
			{
				if (gcj_pthread_handle)
					gcj_pthread_handle(); 		
			}
#endif
			sxr_Free(Msg);
			return ETIMEDOUT;
		}
		else if (Msg->B == GCJ_MSG_PTHREAD_SIG)
		{
#ifdef USE_SYS_EVENT
			abort();
#else		
			if (gcj_pthread_handle)
				gcj_pthread_handle();			
			sxr_Free(Msg);
#endif			
			if (sxr_GetCurrentTaskId() == getMainThreadTskId())
				return EINTR;
			else
				return EINTR;//gcj_StubCondWait(mailBoxId,timeout);
		}
		else if (Msg->B == GCJ_MSG_MUTEX)
		{
			sxr_Free(Msg);
			return EMUTEX;
		}
		else if (Msg->B == GCJ_MSG_SEMPHONE)
		{
			sxr_Free(Msg);
			return ESEMAPHONE;
		}
		else if (Msg->B == GCJ_MSG_GCJ_KILL)
		{
#ifdef USE_SYS_EVENT
			abort();
#else		
			sxr_Free(Msg);
			if (checkIsCurrentWaitInMutexAll())
				return EKILL;
			else
				Jv_ThreadKillCurrent();
#endif			
			return EINTR;
		}
		else
			abort();
	}
	else
	{
		gcj_TraceOut(0,"gcj_StubCondWait Got Event task id=%d,mailBox=%d, event is %d,%d,%d,%d\n",sxr_GetCurrentTaskId(),mailBoxId,ev.nEventId,ev.nParam1,ev.nParam2,ev.nParam3);
		if (GC_isNeedToSuspend())
		{
			if (gcj_pthread_handle)
				gcj_pthread_handle(); 		
		}
		return GCJ_RETURN_EVENT;
	}
}

void gcj_StubCondNotifyMutex(unsigned char mailBoxId)
{
	//gcj_TraceOut(0,"gcj_StubCondNotifyMutex task id=%d taskMbox=%d\n",sxr_GetCurrentTaskId(),mailBoxId);


	Msg_t *msg;
	msg 			 = (Msg_t*) sxr_Malloc(sizeof(Msg_t));
	msg->H.Id		 = 0;
	msg->B			 = GCJ_MSG_MUTEX;
	sxr_Send(msg,mailBoxId,SXR_SEND_MSG|SXR_QUEUE_FIRST);
	
	//sxr_StopFunctionTimer2(CondWaitTimeOut,mailBoxId);
	//sxr_ReleaseSemaphore(mailBoxId);
}

void gcj_StubCondNotifySemphone(unsigned char mailBoxId)
{
	//gcj_TraceOut(0,"gcj_StubCondNotifySemphone task id=%d taskMbox=%d\n",sxr_GetCurrentTaskId(),mailBoxId);


	Msg_t *msg;
	msg 			 = (Msg_t*) sxr_Malloc(sizeof(Msg_t));
	msg->H.Id		 = 0;
	msg->B			 = GCJ_MSG_SEMPHONE;
	sxr_Send(msg,mailBoxId,SXR_SEND_MSG|SXR_QUEUE_FIRST);
	
	//sxr_StopFunctionTimer2(CondWaitTimeOut,mailBoxId);
	//sxr_ReleaseSemaphore(mailBoxId);
}


void gcj_StubCondNotify(unsigned char mailBoxId)
{
	//gcj_TraceOut(0,"gcj_StubCondNotify task id=%d taskMbox=%d\n",sxr_GetCurrentTaskId(),mailBoxId);


	Msg_t *msg;
	msg 			 = (Msg_t*) sxr_Malloc(sizeof(Msg_t));
	msg->H.Id		 = 0;
	msg->B			 = GCJ_MSG_COND_WAIT;
	sxr_StopFunctionTimer2(CondWaitTimeOut,mailBoxId);
	sxr_Send(msg,mailBoxId,SXR_SEND_MSG);
	
	//sxr_StopFunctionTimer2(CondWaitTimeOut,mailBoxId);
	//sxr_ReleaseSemaphore(mailBoxId);
}

void gcj_net_reset_mailId(unsigned char mailBoxId);

void gcj_NativeDestoryThread(unsigned char mailId)
{
	gcj_net_reset_mailId(mailId);
	sxr_StopFunctionTimer2(CondWaitTimeOut,mailId);
	sxr_RemoveCustMsgFromMbox(mailId,0,1);
}

typedef struct _sxr_sig_ {
	unsigned int sig;
	sighandler_t handle;
}_SXR_SIG_T;

_SXR_SIG_T sbxSigArray[30];

static u8 gcjBaseSbx;

void sxr_GcjInitSignalHandle()
{
	memset(sbxSigArray,0xff,sizeof(sbxSigArray));
	gcjBaseSbx=0;
}
void sxr_GcjSetBaseSbx(u8 Sbx)
{
	gcjBaseSbx = Sbx;
}

void sxr_GcjRegSignalHandle(u8 Sbx,int sig,sighandler_t handle)
{
	u8 index = Sbx%gcjBaseSbx;
	sbxSigArray[index].sig = sig;
	sbxSigArray[index].handle = handle;
}

static u8 sxr_GcjCatchSig(u8 Sbx,void* Msg)
{
	if (gcjBaseSbx==0)
		return 0;
	u8 index = Sbx%gcjBaseSbx;
	if (Sbx >= gcjBaseSbx && index < 30)
	{
		if (sbxSigArray[index].sig !=0xffffffff && sbxSigArray[index].handle != 0xffffffff && sbxSigArray[index].handle != 0 )
		{
			if (((int *)Msg)[2] == sbxSigArray[index].sig);
			{
				sbxSigArray[index].handle();
				return 1;
			}
		}
	}
	return 0;
}

