/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__3____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// 系统入口 /////////////////////////////////////////////////////////////
extern void regShell(unsigned short* name);
/**
 * 退出百宝箱。
 * <p>
 * 这个方法在系统完全退出后调用。留给外部扩展。
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-03-02
 */
extern void smarchbox_quit(void); //本方法在移植模块中定义

/**
 * 进入百宝箱。
 * <p>
 * 这个方法在系统进入时，未做任何初始化操作时调用。留给外部扩展。
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-03-02
 */
extern void smarchbox_entry(void); //本方法在移植模块中定义

/**
 * 设备智能适配器机器人。
 * <p>
 * 配套平台移植人员，对具体设备所有接口进行检测。
 * </p>
 * @return 是否启动机器人，并且退出当前百宝箱
 * @author runasea
 * @version 1.0.0.1, 2009-09-02
 */
extern signed char jmetocSmartAdaptRobot(void); //本方法在移植模块中定义

/**
 * 系统启动入口。
 * <p>
 * regShell 方法用于注册客户端进入的 UI 列表。例如：list进入游戏，app进入应用等。
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-03-02
 */
void jmetoc_mtk_entry(){
  smarchbox_entry();
  regShell((unsigned short*)L"");
  startMIDlet();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
