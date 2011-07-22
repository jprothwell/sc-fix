/***********************************************************************
 *
 * MODULE NAME:    host_timer.c
 * DESCRIPTION:    Host Side Timers
 * MAINTAINER:     Dave Airlie
 *
 * SOURCE CONTROL: $Id: host_timer.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies.
 *     All rights reserved.
 *
 *    
 ***********************************************************************/

#include "host_config.h"

#include "host_types.h"

#if (PRH_HOST_UNIT_TEST==1)
#include "stubs/unit_stub_papi.h"
#else
#include "papi.h"
#endif

#include "host_error.h"
#include "host_timer.h"
#include "bt_timer.h"

#if COMBINED_HOST==1
#define MAX_HOST_TIMERS		15 //orignal value was 20, but 10 is enough for now, changed by xzc
#else
#define MAX_HOST_TIMERS		20    
#endif

#define HOST_TIMER_UNUSED		0

struct t_timerInfo {	
    u_int32 timeoutSlots;
    u_int32 timeToExpiry;
    void (*callbackFunc)(void *);
    void *callbackParam;
    u_int8 flags;
} _host_timers[MAX_HOST_TIMERS];
u_int8 _curr_process_timer;

t_pTimer host_timer_create(u_int32 timeoutTenths, void(*callbackFunc)(void *), void *callbackArg, u_int8 flags)
{
    /* Timer handles must start at 1 - timer handle 0 is unused */
    struct t_timerInfo *newEntry;
    t_pTimer retVal;
	int i;

	for(i=0; i<MAX_HOST_TIMERS; i++)
	{
		if(_host_timers[i].flags == HOST_TIMER_UNUSED)
		{
			newEntry = &(_host_timers[i]);
			retVal = i+1;
			break;
		}
	}
    //newEntry=pMalloc(sizeof(struct t_timerInfo));
    if (i==MAX_HOST_TIMERS) {
        return 0;
    }
    
#if pDEBUG
    if (!(flags&pTIMER_ONESHOT||flags&pTIMER_PERIODIC)) {
        pDebugPrintfEX((pLOGERROR,pLOGSYSTEM,"PAPI: pTimerCreate(): Invalid flags value 0x%02X specified\n",flags));
        {   
            int PAPI_pTimerCreate__Invalid_Flags=0;
            assert(PAPI_pTimerCreate__Invalid_Flags);
        }
    }
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    if (!(flags&pTIMER_ONESHOT||flags&pTIMER_PERIODIC)) {       
        SYSDebug_PrintEx(pLOGDEBUG,pLOGSYSTEM, LOG_SYS_HOST_TIME_INDEX, &flags, 1,BT_CODE_BYTE);
       
        {   
            int PAPI_pTimerCreate__Invalid_Flags=0;
        }
    }
#endif

#if pDEBUG
    if (timeoutTenths==0) {
        pDebugPrintfEX((pLOGERROR,pLOGSYSTEM,"PAPI: pTimerCreate(): Invalid timeout value 0x%02X specified\n",0));
        {   
            int PAPI_pTimerCreate__Invalid_Duration=0;
            assert(PAPI_pTimerCreate__Invalid_Duration);
        }
    }
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    if (timeoutTenths==0) {
        flags=0;
        SYSDebug_PrintEx(pLOGERROR,pLOGSYSTEM,(LOG_SYS_HOST_TIME_INDEX+1),&flags,1,BT_CODE_BYTE);
        {   
            int PAPI_pTimerCreate__Invalid_Duration=0;
        }
    }
#endif

    newEntry->callbackFunc=callbackFunc;
    newEntry->timeoutSlots=timeoutTenths*160;
    newEntry->timeToExpiry=BTtimer_Set_Slots(newEntry->timeoutSlots);
    newEntry->callbackParam=callbackArg;
    newEntry->flags=flags;

    return retVal;
}

int host_timer_cancel(t_pTimer timerHandle)
{
    if(timerHandle <= MAX_HOST_TIMERS)
    {
    	_host_timers[timerHandle-1].flags = HOST_TIMER_UNUSED;
        return pENOERROR;
    }
    else
	    return pEINVALIDPARM;
}

void host_handle_expired_timers(void)
{
	struct t_timerInfo *current;

	current = &(_host_timers[_curr_process_timer]);
       _curr_process_timer ++;
	if(_curr_process_timer == MAX_HOST_TIMERS)
		_curr_process_timer = 0;
	if(current->flags == HOST_TIMER_UNUSED)
		return;

	/* Decrement the counter and check for expiry */
	if (BTtimer_Is_Expired(current->timeToExpiry)) 
	{
#if pDEBUG
		pDebugPrintfEX((pLOGDEBUG,pLOGSYSTEM,"PAPI: About to fire timer 0x%02x", _curr_process_timer));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)           
              SYSDebug_PrintEx(pLOGDEBUG,pLOGSYSTEM, (LOG_SYS_HOST_TIME_INDEX+2), &_curr_process_timer, 1,BT_CODE_BYTE);
#endif
		if (current->flags&pTIMER_ONESHOT)
		{
			current->flags = HOST_TIMER_UNUSED; // disable the timer
		} 
		else if (current->flags&pTIMER_PERIODIC) 
		{
			current->timeToExpiry=BTtimer_Set_Slots(current->timeoutSlots);
		} 
		
		current->callbackFunc(current->callbackParam);
	}
}


