/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__13____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// ������� /////////////////////////////////////////////////////////////
//������Ϣ
#define JMETOC_KEY_STATUS_PRESS   0x10000
#define JMETOC_KEY_STATUS_RELEASE 0x20000
#define JMETOC_KEY_STATUS_REPEAT  0x40000
//���̶��в���
#define jmetocAddKeyPressEvent(keyCode)         if (jmetoc_key_cursor < 5) jmetoc_key_queue[jmetoc_key_cursor++] = (keyCode) | JMETOC_KEY_STATUS_PRESS
#define jmetocAddKeyReleaseEvent(keyCode)         if (jmetoc_key_cursor < 5) jmetoc_key_queue[jmetoc_key_cursor++] = (keyCode) | JMETOC_KEY_STATUS_RELEASE
#define jmetocAddKeyRepeatEvent(keyCode)         if (jmetoc_key_cursor < 5) jmetoc_key_queue[jmetoc_key_cursor++] = (keyCode) | JMETOC_KEY_STATUS_REPEAT

/**
 * �����󰴼�ֵ��
 * <p>
 * ��������ɽ��ڼ��̻����еļ�ֵת����ƽ̨�ı�׼��ֵ��
 * �ڼ�����Ϣ���������������ñ���������ñ�׼��ֵ��
 * </p>
 * <p>
 * ������������ GetkeyInfo(u16*,u16*) ������
 * �÷�����������û����еļ����룬��ϵͳ��ͬʵ�֡�
 * enum KEYS {
 * KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
 * KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW,
 * KEY_ENTER, KEY_LSK, KEY_RSK, KEY_STAR, KEY_POUND
 * };
 * </p>
 * @return ���ռ�����
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
 * ע�����м��̵Ļص�������
 * <p>
 * ������ὫҪ���� POWER �� END ����������м��ͷŸ�Ӧ��ʹ�á�
 * ������������������Լ�������Ϊǿ���˳�����
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
extern void jmetoc_regkeyhandle();  //����������ֲģ���ж���

/**
 * ��������ص����á�
 * <p>
 * ����������������У�����ñ��������������а�����
 * �������������Ϻ󣬽������ jmetoc_regkeyhandle ���������ð�������ص���
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
extern void jmetoc_initkey();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
