/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__7____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// ������ʼ�� /////////////////////////////////////////////////////////////
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
 * ���ʱ����Ҫ��һ����̨���ж��Ƿ������˳���
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
 * ϵͳ��ͣ�ص���
 * <p>
 * ��ϵͳ���ⲿ�ж���֪ͨ��ʱ�򣬽������
 * ��Ҫ���ʱ����ͣ������Ӧ�õ�����
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
 * �������ڡ�
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
 * ��ʼ��ϵͳ������
 * <p>
 * ���ݸ���ϵͳ�Ĳ�ͬ������ϵͳ���á�
 * </p>
 * <p>
 * 1)��Ļע�᣻
 * 2)����ȫ����
 * 3)�رն�㣻
 * 4)�������⣻
 * 5)��õ�ǰʱ��㣻
 * 6)ע��Ĭ�������ʺţ�
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-02-28
 */
static void initSysEnvironment() {
  extern void setNetworkAPN(const char*);
  u32 ticker;
  u8 status;
  loadFromLink = false;
  EntryNewScreen(SRC_JMETOC_SMARTBOX_MAIN,jmetoc_onstop,jmetoc_onentry,NULL); //��Ļע��
  entry_full_screen();  //����ȫ��
  gdi_layer_multi_layer_disable();  //�رն��
#ifdef ____JMETOC_LIGHT_ON____
  TurnOnBacklight(0); //������Զ����
#endif
  kal_get_time(&ticker);  //��õ�ǰʱ���
  jmetoc_tickers = (s64)ticker;
  setNetworkAPN("cmnet"); //ע��Ĭ�������ʺ�
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
 * ǿ�Ʒ��ط�����
 * <p>
 * �������ǿ���˳���ͨ�����������ص���һ��������
 * </p>
 */
extern void GoBackHistory(void);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
