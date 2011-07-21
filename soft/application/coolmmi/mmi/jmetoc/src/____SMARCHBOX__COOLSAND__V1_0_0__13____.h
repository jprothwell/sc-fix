/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__13____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// 键盘相关 /////////////////////////////////////////////////////////////
//键盘消息
#define JMETOC_KEY_STATUS_PRESS   0x10000
#define JMETOC_KEY_STATUS_RELEASE 0x20000
#define JMETOC_KEY_STATUS_REPEAT  0x40000
//键盘队列操作
#define jmetocAddKeyPressEvent(keyCode)         if (jmetoc_key_cursor < 5) jmetoc_key_queue[jmetoc_key_cursor++] = (keyCode) | JMETOC_KEY_STATUS_PRESS
#define jmetocAddKeyReleaseEvent(keyCode)         if (jmetoc_key_cursor < 5) jmetoc_key_queue[jmetoc_key_cursor++] = (keyCode) | JMETOC_KEY_STATUS_RELEASE
#define jmetocAddKeyRepeatEvent(keyCode)         if (jmetoc_key_cursor < 5) jmetoc_key_queue[jmetoc_key_cursor++] = (keyCode) | JMETOC_KEY_STATUS_REPEAT

/**
 * 获得最后按键值。
 * <p>
 * 本方法完成将在键盘缓存中的键值转化成平台的标准键值。
 * 在键盘消息发生后，虚拟机会调用本函数来获得标准键值。
 * </p>
 * <p>
 * 本方法调用了 GetkeyInfo(u16*,u16*) 方法，
 * 该方法是用来获得缓存中的键盘码，据系统不同实现。
 * enum KEYS {
 * KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
 * KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW,
 * KEY_ENTER, KEY_LSK, KEY_RSK, KEY_STAR, KEY_POUND
 * };
 * </p>
 * @return 最终键盘码
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
static S16 getKeyCode() {
  u16 keyCode, keyType;
  S16 keyC = 0;
  GetkeyInfo(&keyCode, &keyType);
  switch (keyCode) {
  case KEY_0:
    keyC = javax_microedition_lcdui_Canvas_KEYNUM0;
    break;
  case KEY_1:
    keyC = javax_microedition_lcdui_Canvas_KEYNUM1;
    break;
  case KEY_2:
    keyC = javax_microedition_lcdui_Canvas_KEYNUM2;
    break;
  case KEY_3:
    keyC = javax_microedition_lcdui_Canvas_KEYNUM3;
    break;
  case KEY_4:
    keyC = javax_microedition_lcdui_Canvas_KEYNUM4;
    break;
  case KEY_5:
    keyC = javax_microedition_lcdui_Canvas_KEYNUM5;
    break;
  case KEY_6:
    keyC = javax_microedition_lcdui_Canvas_KEYNUM6;
    break;
  case KEY_7:
    keyC = javax_microedition_lcdui_Canvas_KEYNUM7;
    break;
  case KEY_8:
    keyC = javax_microedition_lcdui_Canvas_KEYNUM8;
    break;
  case KEY_9:
    keyC = javax_microedition_lcdui_Canvas_KEYNUM9;
    break;
  case KEY_UP_ARROW:
    keyC = -1;
    break;
  case KEY_DOWN_ARROW:
    keyC = -2;
    break;
  case KEY_LEFT_ARROW:
    keyC = -3;
    break;
  case KEY_RIGHT_ARROW:
    keyC = -4;
    break;
  case KEY_ENTER:
  case KEY_CSK:
    keyC = -5;
    break;
  case KEY_LSK:
  case KEY_SEND1:
  case KEY_SEND2:
    keyC = -6;
    break;
  case KEY_RSK:
    keyC = -7;
    break;
  case KEY_STAR:
    keyC = javax_microedition_lcdui_Canvas_KEYSTAR;
    break;
  case KEY_POUND:
    keyC = javax_microedition_lcdui_Canvas_KEYPOUND;
    break;
  }
  return keyC;
}

/**
 * 注册所有键盘的回调函数。
 * <p>
 * 虚拟机会将要除了 POWER 和 END 键以外的所有键释放给应用使用。
 * 而这两个键由虚拟机自己处理，作为强制退出键。
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
extern void jmetoc_regkeyhandle();  //本方法在移植模块中定义

/**
 * 键盘最初回调设置。
 * <p>
 * 在虚拟机启动过程中，会调用本方法来屏蔽所有按键。
 * 当虚拟机启动完毕后，将会调用 jmetoc_regkeyhandle 方法来设置按键处理回调。
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
extern void jmetoc_initkey();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
