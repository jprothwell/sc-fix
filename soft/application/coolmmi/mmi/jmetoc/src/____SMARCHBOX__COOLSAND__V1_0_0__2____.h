/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__2____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// 平台参数定义 /////////////////////////////////////////////////////////////
#define JMETOC_TIMER_1                  JMETOC_TIMER_BASE             //第一个时钟，主要给系统启动和退出相关时候调用
#define JMETOC_TIMER_2                  JMETOC_TIMER_BASE + 1         //第二个时钟，主要给系统内部模块使用
#define JMETOC_TIMER_3                  JMETOC_TIMER_BASE + 2         //第三个时钟，主要给系统时钟调用
#define JMETOC_TIMER_4                  JMETOC_TIMER_BASE + 3         //第四个时钟，用于短信收发
#define JMETOC_TIMER_5                  JMETOC_TIMER_BASE + 4         //第五个时钟，震动，判断Idle
#define SRC_JMETOC_SMARTBOX_MAIN        50000                         //屏幕ID
#define JMETOC_LOGO_DELAY               100                           //LOGO 停顿时间，该时间并不是精确显示 LOGO 的时间。因为启动 LOGO 以后，到加载系统完毕，整个过程 LOGO 都不会消失。
#define JMETOC_TIMER_ADPT               8                             //时钟适配，在时钟进度不够的系统中通过这个参数调整，指定5毫秒的实际精度
#define JMETOC_INTERNAL_DELAY2          2                             //系统内定时钟间隔，以毫秒计，配合 JMETOC_TIMER_ADPT 调整精度
#include "../config.h"                                                //动态配置，与外部打包系统配合，必须要引入
#if __SCREEN_HEIGHT__ == 480
#define LCD_WIDTH                       240                           //屏幕宽度
#define LCD_HEIGHT                      320                           //屏幕高度
#else
#define LCD_WIDTH                       __SCREEN_WIDTH__              //屏幕宽度
#define LCD_HEIGHT                      __SCREEN_HEIGHT__             //屏幕高度
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
