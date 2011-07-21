/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__11____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// 屏幕方法 /////////////////////////////////////////////////////////////
/**
 * 屏幕清空。
 * <p>
 * 实际上是完成用白色作为背景将屏幕填充。
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
extern void clear_screen();

#if __SUPPORT_SOFTKEY__
#include "____SMARCHBOX__COOLSAND__V1_0_0__26____.h"
#endif

/**
 * 刷新屏幕。
 * <p>
 * 将显示缓存刷入实际显存，实现屏幕刷新。
 * </p>
 * @param x 屏幕刷新点左上 X 坐标
 * @param y 屏幕刷新点左上 Y 坐标
 * @param width 刷新区域宽度
 * @param height 刷新区域高度
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
extern void jmetoc_gdi_refresh_buffer(s32 x, s32 y, s32 width, s32 height); //本方法在移植模块中定义
void jmetoc_refresh_buffer(S32 x, S32 y, S32 width, S32 height) {
  #if __SUPPORT_SOFTKEY__
    #if __SCREEN_HEIGHT__ > 320
    if (height > 320) {
      jmetocSKey_paint();
    }
    #else
    if (jmetoc_sk_show) {
      jmetocSKey_paint();
    }
    #endif
  #endif
  jmetoc_gdi_refresh_buffer(x, y, width, height);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
