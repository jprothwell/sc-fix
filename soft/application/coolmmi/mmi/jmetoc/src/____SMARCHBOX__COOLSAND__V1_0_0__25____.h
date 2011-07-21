/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__25____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

////////////////////////////////////////////////////////// 附加 API 模块 //////////////////////////////////////////////////////////
extern void applib_dt_get_date_time(applib_time_struct *t);
extern u32 kal_milli_secs_to_ticks(u32 milli_secs);
extern int mmi_bootup_is_sim_valid(void);
#if 0
extern void MMI_Sleep(u32 um);
#endif //0

#ifndef WIN32
extern int dbg_TraceOutputText(u16 nIndex, const char* fmt, ...);
#define kal_prompt_trace(trace_level, fmt, ...) \
			dbg_TraceOutputText(1, (fmt), ##__VA_ARGS__);
#endif

/**
 * 当前进程睡眠。
 * @param ms 睡眠时间
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI void sleep(u32 ms) {
  //kal_sleep_task(kal_milli_secs_to_ticks(ms));
  COS_Sleep(ms);
}

#ifndef WIN32
typedef struct _TM_SYSTEMTIME { 
    u16 uYear;
    u8  uMonth;
    u8  uDayOfWeek;
    u8  uDay;
    u8  uHour;
    u8  uMinute;
    u8  uSecond;
    u16 uMilliseconds;
} TM_SYSTEMTIME, *TM_PSYSTEMTIME;
u8 TM_GetSystemTime(
    TM_SYSTEMTIME* pSystemTime
);
#endif
/**
 * 获得当前时间数据。
 * <p>
 * 获取的时间数据存储与结构体 SYSTEMTIME 中。
 * </p>
 */
void getSystemDate() {
  TM_SYSTEMTIME tm;
  TM_GetSystemTime(&tm);
  jmetoc_time.nDay = tm.uDay;
  jmetoc_time.DayIndex = tm.uDayOfWeek;
  jmetoc_time.nHour = tm.uHour;
  jmetoc_time.nMin = tm.uMinute;
  jmetoc_time.nMonth = tm.uMonth;
  jmetoc_time.nSec = tm.uSecond;
  jmetoc_time.nYear = tm.uYear;
}

/**
 * 电话呼叫。
 * @param pNumber 号码
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI void jmetoc_call(u16* pNumber) {
	MakeCall((s8*)pNumber);
}

extern void kal_delete_task(kal_taskid task_id);
/**
 * 创建新线程。
 * <p>
 * 请注意：这个函数在现有的系统中不被使用。
 * 在一些可以支持线程的系统中可以考虑实现，以备将来使用。
 * </p>
 * @param run 线程回调
 * @param param 参数
 * @return 线程ID
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI thid newThread(void* run, void* param) {
  return null;
}

/**
 * 停止线程。
 * <p>
 * 请注意：这个函数在现有的系统中不被使用。
 * 在一些可以支持线程的系统中可以考虑实现，以备将来使用。
 * </p>
 * @param tid 线程ID
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI void stopThread(thid tid) {
  //kal_delete_task(tid);
}

/**
 * 信息跟踪。
 * <p>
 * 对各种设备环境的调试信息进行跟踪。输出跟踪信息。
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-05-01
 */
JMETOCAPI void jmetoc_trace(const char *fmt) {
#ifdef ____SMARCHBOX_DEBUG_ON____
  dbg_TraceOutputText(10, "SMARCH_BOX_LOG >> %s", fmt);
#else
  getSystemDate();
#endif
}

extern s32 jmetoc_mem_total;
/**
 * 有效存储报告。
 * <p>
 * 对初始化内存大小进行报告。
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-08-23
 */
void jmetoc_mem_report() {
  kal_prompt_trace(mod_trace, "mem total %dK\r\n", jmetoc_mem_total >> 10);
}

#include "____SMARCHBOX__COOLSAND__V1_0_0__29____.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
