/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifdef CFW_TCPIP_SUPPORT
//#include <cswtype.h>
#include "tcpip_sys.h"
#include "base_prv.h"
//#include <csw.h>
//#include "tcpip_opt.h"
#include "tcpip_def.h"
//#include "tcpip_memp.h"
//#include <cos.h>

extern BOOL BAL_SendTaskEvent(COS_EVENT* pEvent, UINT8 nTaskId);

#if (NO_SYS == 0)

struct sswt_cb
{
	sys_sem_t *psem;
	INT16 timeflag;
	UINT8 padding[2];
};

VOID *sys_mbox_fetch(sys_mbox_t mbox, VOID **Msg)
{
///     CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("%d wait msg\n"),mbox);
    //COS_WaitEvent(  mbox,  *Msg, COS_WAIT_FOREVER);
    return *Msg;
}

VOID sys_sem_wait(sys_sem_t sem)
{
#if 0
	UINT32 time;
	struct sys_timeouts *timeouts;
	struct sys_timeout *tmptimeout;
	sys_timeout_handler h;
	VOID *arg;

	/*  while (sys_arch_sem_wait(sem, 1000) == 0);
	   return; */

      again:

	timeouts = sys_arch_timeouts();

	if (!timeouts || !timeouts->next)
	{
		sys_arch_sem_wait(sem, 0);
	}
	else
	{
		if (timeouts->next->time > 0)
		{
			time = sys_arch_sem_wait(sem, timeouts->next->time);
		}
		else
		{
			time = SYS_ARCH_TIMEOUT;
		}

		if (time == SYS_ARCH_TIMEOUT)
		{
			/* If time == SYS_ARCH_TIMEOUT, a timeout occured before a message
			   could be fetched. We should now call the timeout handler and
			   deallocate the memory allocated for the timeout. */
			tmptimeout = timeouts->next;
			timeouts->next = tmptimeout->next;
			h = tmptimeout->h;
			arg = tmptimeout->arg;
			memp_free(MEMP_SYS_TIMEOUT, tmptimeout);
			if (h != NULL)
			{
				CSW_TRACE(CFW_TCPIP_TS_ID_SXS,TSTXT("ssw h=%p(%p)\n"), (VOID *) h, (VOID *) arg);
				h(arg);
			}

			/* We try again to fetch a message from the mbox. */
			goto again;
		}
		else
		{
			/* If time != SYS_ARCH_TIMEOUT, a message was received before the timeout
			   occured. The time variable is set to the number of
			   milliseconds we waited for the message. */
			if (time <= timeouts->next->time)
			{
				timeouts->next->time -= time;
			}
			else
			{
				timeouts->next->time = 0;
			}
		}

	}
#endif
}

VOID sys_timeout(UINT32 msecs, sys_timeout_handler h, VOID *arg)
{
/*
	struct sys_timeouts *timeouts;
	struct sys_timeout *timeout, *t;

	timeout = memp_malloc(MEMP_SYS_TIMEOUT);
	if (timeout == NULL)
	{
		return;
	}
	timeout->next = NULL;
	timeout->h = h;
	timeout->arg = arg;
	timeout->time = msecs;

	timeouts = sys_arch_timeouts();

	CSW_TRACE(CFW_TCPIP_TS_ID_SXS,TSTXT("sys_timeout: %p msecs=0x%x h=%p arg=%p\n"), (VOID *) timeout, msecs, (VOID *) h, (VOID *) arg);

	LWIP_ASSERT("sys_timeout: timeouts != NULL", timeouts != NULL);

	if (timeouts->next == NULL)
	{
		timeouts->next = timeout;
		return;
	}

	if (timeouts->next->time > msecs)
	{
		timeouts->next->time -= msecs;
		timeout->next = timeouts->next;
		timeouts->next = timeout;
	}
	else
	{
		for (t = timeouts->next; t != NULL; t = t->next)
		{
			timeout->time -= t->time;
			if (t->next == NULL || t->next->time > timeout->time)
			{
				if (t->next != NULL)
				{
					t->next->time -= timeout->time;
				}
				timeout->next = t->next;
				t->next = timeout;
				break;
			}
		}
	}
*/
}

/* Go through timeout list (for this task only) and remove the first matching entry,
   even though the timeout has not triggered yet.
*/

VOID sys_untimeout(sys_timeout_handler h, VOID *arg)
{
#if 0	
struct sys_timeouts *timeouts;
	struct sys_timeout *prev_t, *t;

	timeouts = sys_arch_timeouts();

	if (timeouts->next == NULL)
		return;

	for (t = timeouts->next, prev_t = NULL; t != NULL; prev_t = t, t = t->next)
	{
		if ((t->h == h) && (t->arg == arg))
		{
			/* We have a match */
			/* Unlink from previous in list */
			if (prev_t == NULL)
				timeouts->next = t->next;
			else
				prev_t->next = t->next;
			/* If not the last one, add time of this one back to next */
			if (t->next != NULL)
				t->next->time += t->time;
			memp_free(MEMP_SYS_TIMEOUT, t);
			return;
		}
	}
#endif
	return;
}
#if 0
static VOID sswt_handler(VOID *arg)
{
	struct sswt_cb *sswt_cb = (struct sswt_cb *) arg;

	/* Timeout. Set flag to TRUE and signal semaphore */
	sswt_cb->timeflag = 1;
	sys_sem_signal(*(sswt_cb->psem));
}
#endif
/* Wait for a semaphore with timeout (specified in ms) */
/* timeout = 0: wait forever */
/* Returns 0 on timeout. 1 otherwise */

INT32 sys_sem_wait_timeout(sys_sem_t sem, UINT32 timeout)
{
#if 0
	struct sswt_cb sswt_cb;

	sswt_cb.psem = &sem;
	sswt_cb.timeflag = 0;

	/* If timeout is zero, then just wait forever */
	if (timeout > 0)
		/* Create a timer and pass it the address of our flag */
		sys_timeout(timeout, sswt_handler, &sswt_cb);
	sys_sem_wait(sem);
	/* Was it a timeout? */
	if (sswt_cb.timeflag)
	{
		/* timeout */
		return 0;
	}
	else
	{
		/* Not a timeout. Remove timeout entry */
		sys_untimeout(sswt_handler, &sswt_cb);
		return 1;
	}
#endif
return 1;

}

VOID sys_msleep(UINT32 ms)
{
#if 0
	sys_sem_t delaysem = sys_sem_new(0);

	sys_sem_wait_timeout(delaysem, ms);

	sys_sem_free(delaysem);
#endif
}

#if 1
UINT32 sys_arch_mbox_fetch(UINT8 mbox, VOID **msg, UINT32 timeout)
{
    return 0;
}
UINT32 sys_arch_sem_wait(sys_sem_t sem, UINT32 timeout)
{
    return 0;
}
#if 0
//UINT32 sys_arch_mbox_fetch(UINT8 mbox, VOID **msg, UINT32 timeout,UINT32 Id)
{

    UINT32 Evt[2];
    COS_EVENT* pEvent;
    
    COS_SetTimer ( mbox, Id,  COS_TIMER_MODE_SINGLE, timeout);    
    //sxs_StartTimer(timeout, Id, 0, FALSE, mbox);
    
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS,TSTXT("sys_arch_mbox_fetch:%d\n"),mbox);
    
    COS_WaitEvent(  mbox,  *msg, COS_WAIT_FOREVER);
    //*msg = sxr_Wait( Evt, mbox);
    pEvent = *msg;
    
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS,TSTXT("sys_arch_mbox_fetch***,Evt[0]:%x,msg ID:%x\n"),Evt[0],(VOID *)msg[0]);

    if( pEvent->nEventId == Id )
    {
      CSW_TRACE(CFW_TCPIP_TS_ID_SXS,TSTXT("receive:timeout\n"));
        return SYS_ARCH_TIMEOUT;
    }
    else
    {
        COS_KillTimerEX ( mbox,  Id );       
        //sxs_StopTimer(Id, 0, mbox);
     CSW_TRACE(CFW_TCPIP_TS_ID_SXS,TSTXT("receive:no timeout\n"));
        return TRUE;
    }

    //return 0;
}


#endif 
struct sys_timeouts *sys_arch_timeouts(VOID)
{
	return 0;
}

VOID sys_sem_free(sys_sem_t sem)
{
 //   CSW_TRACE(CFW_TCPIP_TS_ID_SXS,TSTXT("Free sem ID: %d\n"),sem);
//    COS_DeleteSemaphore (sem);
}

UINT8 sys_sem_new(sys_sem_t sem)
{
	//return (UINT8)COS_CreateSemaphore(sem);	
	return 1;
}

VOID sys_sem_signal(sys_sem_t sem)
{
//    CSW_TRACE(CFW_TCPIP_TS_ID_SXS,TSTXT("Release sem ID: %d\n"),sem);
//    CSW_TRACE(_TCP,TSTXT"Release sem ID: %d\n",sem);
//COS_ReleaseSemaphore (sem);
}

UINT8 sys_mbox_new(VOID)
{
#if 0
	return (UINT8)sxr_NewMailBox();
#else	
	//return (UINT8)COS_CreateSemaphore(0);	
	return 0;	
#endif
}

VOID sys_mbox_post(UINT8 mbox, VOID *msg)
{

//   CSW_TRACE(CFW_TCPIP_TS_ID_SXS,TSTXT("send msg to %d\n"),mbox);
   // COS_SendEvent( mbox, msg, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
//    sxr_Send (msg, mbox, 0);
}

VOID sys_mbox_free(UINT8 mbox)
{
  //  UINT32 Evt[2];
  //  VOID *msg;

#if 0    
//    while (sxr_SbxHot (mbox))
    while (COS_IsEventAvailable (mbox))
    {
        COS_WaitEvent(  mbox,  msg, COS_WAIT_FOREVER);
//        msg = sxr_Wait (Evt, mbox);
        if(msg != NULL )
        {
            CSW_TCPIP_FREE ((VOID *)msg);
        }
    }
    
	sxr_FreeMailBox (mbox);
#else
   // COS_DeleteSemaphore (mbox);
#endif
}

VOID sys_post_event_to_APS(VOID *msg)
{
     COS_EVENT *pcos_ev = (COS_EVENT *)msg;

//     CSW_TRACE(CFW_TCPIP_TS_ID_SXS,TSTXT("send msg to APS \n"));
    
     BAL_SendTaskEvent(pcos_ev, CSW_TASK_ID_APS);
 
    return;
}  

VOID sys_post_event_to_APP(UINT32 nEventId, UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT8 nFlag, UINT16 reserved)
{
#if 1
    COS_EVENT ev;

    UINT16 n = MAKEUINT16(nFlag, nType); 
    
    ev.nEventId =  nEventId;
    ev.nParam1  = nParam1;
    ev.nParam2  = nParam2;

    ev.nParam3 =  MAKEUINT32(n, reserved);
        
    COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
#endif        

    return;
} 
#endif



#endif /* NO_SYS */
#endif

