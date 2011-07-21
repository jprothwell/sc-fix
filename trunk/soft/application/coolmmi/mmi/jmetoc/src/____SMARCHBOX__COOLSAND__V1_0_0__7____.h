/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__7____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// 环境初始化 /////////////////////////////////////////////////////////////
extern unsigned short idle_sid;
extern JMETOC_BOOL loadFromLink;
extern javax_microedition_lcdui_Display* javax_microedition_lcdui_Display_disp;
extern U16 GetActiveScreenId(void);
extern JMETOC_BOOL vm_status;
extern JMETOC_BOOL jmetoc_app_plug;
extern JMETOC_BOOL jmetoc_app_bg;
extern void jmetoc_show_icon();

#ifndef WIN32
extern int dbg_TraceOutputText(u16 nIndex, const char* fmt, ...);
#define kal_prompt_trace(trace_level, fmt, ...) \
			dbg_TraceOutputText(1, (fmt), ##__VA_ARGS__);
extern int mod_trace;
#endif

#define SMARCHBOX_NO_SDCARD_SCREEN_ID 5001
/**
 * <p>
 * 这个时候需要有一个后台来判断是否发生了退出。
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-02-28
 */
void checkIdle() {
  u16 sid = GetActiveScreenId();
  //kal_prompt_trace(1, "aid %d,%d", sid, idle_sid);
  if (sid == idle_sid || sid == SMARCHBOX_NO_SDCARD_SCREEN_ID) {
    if (vm_status) {
      exitSmarchBox();
    }
  } else {
    StartTimer(JMETOC_TIMER_5,JMETOC_IDLE_CHECK,checkIdle);
  }
}

/**
 * 系统暂停回调。
 * <p>
 * 当系统被外部中断所通知的时候，进入这里。
 * 主要完成时钟暂停，挂起应用的任务。
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-02-28
 */
void jmetoc_onstop() {
	#ifdef MMI_ON_HARDWARE_P

  jmetoc_trace("int");
#ifdef __JMETOC_INTERRUPT_QUIT__
  exitSmarchBox();
#else
  if (!jmetoc_app_bg) {
    StopTimer(JMETOC_TIMER_3);
  } else {
    jmetoc_hide_icon();
    jmetoc_show_icon();
  }
  if (!quit_vm) {
    javax_microedition_lcdui_Display_disp->javax_microedition_lcdui_Display_paintSuspended = true;
    if (!jmetoc_app_bg) {
      MIDLET->javax_microedition_midlet_MIDlet_pauseApp(MIDLET);
      StartTimer(JMETOC_TIMER_5,JMETOC_IDLE_CHECK,checkIdle);
    }
  }
#endif

#endif
}

/**
 * 虚拟机入口。
 * @author runasea
 * @version 1.0.0.1, 2009-02-28
 */
void jmetoc_onentry() {
  EntryNewScreen(SRC_JMETOC_SMARTBOX_MAIN,jmetoc_onstop,jmetoc_onentry,NULL);
  entry_full_screen();
  StopTimer(JMETOC_TIMER_5);
  javax_microedition_lcdui_Display_disp->javax_microedition_lcdui_Display_paintSuspended = false;
  jmetoc_onstart();
  javax_microedition_lcdui_Display_repaint(javax_microedition_lcdui_Display_disp, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__);
}

extern void kal_get_time(U32 *ticks_ptr);
extern int smarchbox_attrsp;
extern u8 jmetoc_SIM;
/**
 * 初始化系统环境。
 * <p>
 * 根据各个系统的不同来进行系统配置。
 * </p>
 * <p>
 * 1)屏幕注册；
 * 2)进入全屏；
 * 3)关闭多层；
 * 4)开启背光；
 * 5)获得当前时间点；
 * 6)注册默认上网帐号；
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-02-28
 */
static void initSysEnvironment() {
  extern void setNetworkAPN(const char*);
  u32 ticker;
  u8 status;
  loadFromLink = false;
  EntryNewScreen(SRC_JMETOC_SMARTBOX_MAIN,jmetoc_onstop,jmetoc_onentry,NULL); //屏幕注册
  entry_full_screen();  //进入全屏
  gdi_layer_multi_layer_disable();  //关闭多层
#ifdef ____JMETOC_LIGHT_ON____
  TurnOnBacklight(0); //背光永远开着
#endif
  kal_get_time(&ticker);  //获得当前时间点
  jmetoc_tickers = (s64)ticker;
  setNetworkAPN("cmnet"); //注册默认上网帐号
  //check attach
  #if __SUPPORT_SLAVESIM__
    CFW_GetGprsAttState(&status, jmetoc_SIM);
  #else
    CFW_GetGprsAttState(&status);
  #endif
  if (status == 1) {
    smarchbox_attrsp = 1;
  }
}

/**
 * 强制返回方法。
 * <p>
 * 当虚拟机强制退出后，通过本方法返回到上一个场景。
 * </p>
 */
extern void GoBackHistory(void);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
