/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__2____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// ƽ̨�������� /////////////////////////////////////////////////////////////
#define JMETOC_TIMER_1                  JMETOC_TIMER_BASE             //��һ��ʱ�ӣ���Ҫ��ϵͳ�������˳����ʱ�����
#define JMETOC_TIMER_2                  JMETOC_TIMER_BASE + 1         //�ڶ���ʱ�ӣ���Ҫ��ϵͳ�ڲ�ģ��ʹ��
#define JMETOC_TIMER_3                  JMETOC_TIMER_BASE + 2         //������ʱ�ӣ���Ҫ��ϵͳʱ�ӵ���
#define JMETOC_TIMER_4                  JMETOC_TIMER_BASE + 3         //���ĸ�ʱ�ӣ����ڶ����շ�
#define JMETOC_TIMER_5                  JMETOC_TIMER_BASE + 4         //�����ʱ�ӣ��𶯣��ж�Idle
#define SRC_JMETOC_SMARTBOX_MAIN        50000                         //��ĻID
#define JMETOC_LOGO_DELAY               100                           //LOGO ͣ��ʱ�䣬��ʱ�䲢���Ǿ�ȷ��ʾ LOGO ��ʱ�䡣��Ϊ���� LOGO �Ժ󣬵�����ϵͳ��ϣ��������� LOGO ��������ʧ��
#define JMETOC_TIMER_ADPT               8                             //ʱ�����䣬��ʱ�ӽ��Ȳ�����ϵͳ��ͨ���������������ָ��5�����ʵ�ʾ���
#define JMETOC_INTERNAL_DELAY2          2                             //ϵͳ�ڶ�ʱ�Ӽ�����Ժ���ƣ���� JMETOC_TIMER_ADPT ��������
#include "../config.h"                                                //��̬���ã����ⲿ���ϵͳ��ϣ�����Ҫ����
#if __SCREEN_HEIGHT__ == 480
#define LCD_WIDTH                       240                           //��Ļ���
#define LCD_HEIGHT                      320                           //��Ļ�߶�
#else
#define LCD_WIDTH                       __SCREEN_WIDTH__              //��Ļ���
#define LCD_HEIGHT                      __SCREEN_HEIGHT__             //��Ļ�߶�
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
