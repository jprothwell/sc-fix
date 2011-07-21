/*
 *
 * Copyright  1990-2009 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */

#ifdef __cplusplus
extern "C" {
#endif
#include "tm.h"
#include "mmi_trace.h"
#include "sxr_tim.h"
#include "timerevents.h"
#include "hal_timers.h"

#include "javacall_time.h"
/**
 *
 * Create a native timer to expire in wakeupInSeconds or less seconds.
 *
 * @param wakeupInMilliSecondsFromNow time to wakeup in milli-seconds
 *                              relative to current time
 *                              if -1, then ignore the call
 * @param cyclic <tt>JAVACALL_TRUE</tt>  indicates that the timer should be
 *               repeated cuclically,
 *               <tt>JAVACALL_FALSE</tt> indicates that this is a one-shot
 *               timer that should call the callback function once
 * @param func callback function should be called in platform's context once the timer
 *			   expires
 * @param handle A pointer to the returned handle that on success will be
 *               associated with this timer.
 *
 * @return on success returns <tt>JAVACALL_OK</tt>,
 *         or <tt>JAVACALL_FAIL</tt> or negative value on failure
 */
static int javacall_wapper_time=0;
static javacall_callback_func g_TimerHandle;

static void javacall_timer_wapper(javacall_handle handle)
{
	g_TimerHandle(handle);
	sxr_StartFunctionTimer(javacall_wapper_time, javacall_timer_wapper, (VOID*)NULL, 0);
}

javacall_result javacall_time_initialize_timer(
                    int                      wakeupInMilliSecondsFromNow,
                    javacall_bool            cyclic,
                    javacall_callback_func   func,
					/*OUT*/ javacall_handle	*handle){
    mmi_trace(1,"time:function : %s, wakeupInMilliSecondsFromNow=%d,cyclic=%d\n",__FUNCTION__,wakeupInMilliSecondsFromNow,cyclic);
    if(cyclic == JAVACALL_FALSE)
        asm("break 1");//TODO:impl one shot timer
        
    *handle = (javacall_handle)JVM_TIMER;
    g_TimerHandle = func;
    javacall_wapper_time = 495;//16384*wakeupInMilliSecondsFromNow/1000;
    //javacall_timer_wapper(*handle);
    sxr_StartFunctionTimer(javacall_wapper_time, javacall_timer_wapper, (VOID*)NULL, 0);
    return JAVACALL_OK;
}

/**
 *
 * Disable a set native timer
 * @param handle The handle of the timer to be finalized
 *
 * @return on success returns <tt>JAVACALL_OK</tt>,
 *         <tt>JAVACALL_FAIL</tt> or negative value on failure
 */
javacall_result javacall_time_finalize_timer(javacall_handle handle){
    int i = (int)handle;
    mmi_trace(1,"time:current function : %s,handle=%d\n",__FUNCTION__,i);
	sxr_StopFunctionTimer(javacall_timer_wapper);
	return JAVACALL_OK;
}

/*
 *
 * Temporarily disable timer interrupts. This is called when
 * the VM is about to sleep (when there's no Java thread to execute)
 *
 * @param handle timer handle to suspend
 *
 */
void javacall_time_suspend_ticks(javacall_handle handle){
    int i = (int)handle;
    mmi_trace(1,"time:current function : %s,handle=%d\n",__FUNCTION__,i);
    sxr_StopFunctionTimer(javacall_timer_wapper);
    return;
}

/*
 *
 * Enable  timer interrupts. This is called when the VM
 * wakes up and continues executing Java threads.
 *
 * @param handle timer handle to resume
 *
 */
void javacall_time_resume_ticks(javacall_handle handle){
    int i = (int)handle;
	mmi_trace(1,"time:current function : %s,handle=%d\n",__FUNCTION__,i);
    sxr_StartFunctionTimer(javacall_wapper_time, javacall_timer_wapper, (VOID*)NULL, 0);
    return;
}

/*
 * Suspend the current process sleep for ms milliseconds
 */
void javacall_time_sleep(javacall_uint64 ms){
	mmi_trace(1,"time:current function : %s\n",__FUNCTION__);
	COS_Sleep(ms);
    return;
}

/**
 *
 * Create a native timer to expire in wakeupInSeconds or less seconds.
 * At least one native timer can be used concurrently.
 * If a later timer exists, cancel it and create a new timer
 *
 * @param type type of alarm to set, either JAVACALL_TIMER_PUSH, JAVACALL_TIMER_EVENT
 *                              or JAVACALL_TIMER_WATCHDOG
 * @param wakeupInMilliSecondsFromNow time to wakeup in milli-seconds
 *                              relative to current time
 *                              if -1, then ignore the call
 *
 * @return <tt>JAVACALL_OK</tt> on success, <tt>JAVACALL_FAIL</tt> on failure
 */
////javacall_result	javacall_time_create_timer(javacall_timer_type type, int wakeupInMilliSecondsFromNow){
//    return JAVACALL_FAIL;
//}

/**
 * Return local timezone ID string. This string is maintained by this
 * function internally. Caller must NOT try to free it.
 *
 * This function should handle daylight saving time properly. For example,
 * for time zone America/Los_Angeles, during summer time, this function
 * should return GMT-07:00 and GMT-08:00 during winter time.
 *
 * @return Local timezone ID string pointer. The ID string should be in the
 *         format of GMT+/-??:??. For example, GMT-08:00 for PST.
 */
char* javacall_time_get_local_timezone(void){mmi_trace(1,"time:current function : %s\n",__FUNCTION__);
    //return (char*) "GMT+08:00";
	  INT8 nzone=0;
    static UINT8  nGMT[20]={0};
    
	  nzone = TM_GetTimeZone();
    
    if((nzone >= 0) && (nzone < 10 ))       SUL_StrPrint(nGMT,"GMT+0%d", nzone);
    else if((nzone > 9 ))                   SUL_StrPrint(nGMT,"GMT+%d",  nzone);
    else if((nzone < 0) && (nzone > -10 ))  SUL_StrPrint(nGMT,"GMT-0%d", nzone);
    else if((nzone < -9 ))                  SUL_StrPrint(nGMT,"GMT-%d",  nzone);
    
    return (char*)nGMT;
}

/**
 * returns number of milliseconds elapsed since midnight(00:00:00), January 1, 1970,
 *
 * @return milliseconds elapsed since midnight (00:00:00), January 1, 1970
 */
 
extern UINT32 TM_GetTimeZoneTime (
                            INT8  nTimeZone,
                            TM_SYSTEMTIME* pSystemTime
                            );
extern U8 PhnsetGetHomeCity(void); 
volatile unsigned long long currentTImeMillis=0;
volatile unsigned long long prevTImeMillis=0;
const unsigned long long timeAfter2000 = (946684800)*1000LL;

static int GetLocalTimeZoneOffset()
{
	int izone=0;
	
	izone = TM_GetTimeZone();
	//izone = PhnsetGetHomeCity()+1;
	//mmi_trace(1,"gcj_StubGetLocalTimeZoneOffset izone=%d\n",izone);	
	return izone;
}

static unsigned long long getCurrentMillis()
{
	TM_SYSTEMTIME SystemTime = {0,} ;

	static unsigned int record_millis;
	static unsigned int record_time;
	volatile unsigned int ticks= hal_TimGetUpTime();
	ticks &=0x3fff;
	volatile unsigned int millis= (ticks*1000)>>14;// (ticks%16384 *1000)/16384;//(ticks*1000)>>14;
	volatile unsigned int time = TM_GetTimeZoneTime(0,&SystemTime);
	while (!time)
	{
		time = TM_GetTimeZoneTime(0,&SystemTime);
		mmi_trace(1,"getCurrentMillis time return null\n");
		COS_Sleep(50);
	}
	time -= GetLocalTimeZoneOffset()*60*60; //get UTC Time
	//millis = (millis+500)%1000;
	if (millis < record_millis && time == record_time)
	{
		millis = 1000+millis;
	}
	record_millis = millis;
	record_time = time;
	//mmi_trace(1,"getCurrentMillis current ticks=%d,millis=%d,time=%d",ticks,millis,time);
	return time*1000LL+millis;
}

javacall_int64 /*OPTIONAL*/ javacall_time_get_milliseconds_since_1970(void){
	unsigned long long ret=0;
	currentTImeMillis = ret =  getCurrentMillis();
	if (currentTImeMillis <  prevTImeMillis)
	{
		//COS_Sleep(10);
		mmi_trace(1,"currentTImeMillis current sec=%d,%d,millis=%d,ticks=%d",currentTImeMillis/1000,(unsigned int)currentTImeMillis%1000,hal_TimGetUpTime());
		mmi_trace(1,"prevTImeMillis current sec=%d,%d,millis=%d,ticks=%d",prevTImeMillis/1000,(unsigned int)prevTImeMillis%1000,hal_TimGetUpTime());
		currentTImeMillis = prevTImeMillis;//getCurrentMillis();
	}
	prevTImeMillis = ret;
	return ret+timeAfter2000;
}

/**
 * returns the number of seconds elapsed since midnight (00:00:00), January 1, 1970,
 *
 * @return seconds elapsed since midnight (00:00:00), January 1, 1970
 */
javacall_time_seconds /*OPTIONAL*/ javacall_time_get_seconds_since_1970(void){//mmi_trace(1,"time:current function : %s\n",__FUNCTION__);
    return JAVACALL_NOT_IMPLEMENTED;
}

/**
 * returns the milliseconds elapsed time counter
 *
 * @return elapsed time in milliseconds
 */
javacall_time_milliseconds /*OPTIONAL*/ javacall_time_get_clock_milliseconds(void){//mmi_trace(1,"time:current function : %s\n",__FUNCTION__);
    return JAVACALL_NOT_IMPLEMENTED;
}

#ifdef __cplusplus
}
#endif

