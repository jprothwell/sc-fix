/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__14____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// 鼠标相关 /////////////////////////////////////////////////////////////
#if __SUPPORT_CURSOR__
//鼠标消息
#define JMETOC_CURSOR_STATUS_DOWN 0x1000000
#define JMETOC_CURSOR_STATUS_UP   0x2000000
#define JMETOC_CURSOR_STATUS_MOVE 0x4000000
//鼠标队列操作
#define jmetocAddCursorDownEvent(x, y)         if (jmetoc_cursor_cursor < 5) jmetoc_cursor_queue[jmetoc_cursor_cursor++] = (x & 0xfff) | ((y & 0xfff) << 12) | JMETOC_CURSOR_STATUS_DOWN;
#define jmetocAddCursorUpEvent(x, y)         if (jmetoc_cursor_cursor < 5) jmetoc_cursor_queue[jmetoc_cursor_cursor++] = (x & 0xfff) | ((y & 0xfff) << 12) | JMETOC_CURSOR_STATUS_UP;
#define jmetocAddCursorMoveEvent(x, y)         if (jmetoc_cursor_cursor < 5) jmetoc_cursor_queue[jmetoc_cursor_cursor++] = (x & 0xfff) | ((y & 0xfff) << 12) | JMETOC_CURSOR_STATUS_MOVE;
//支持方法
static void jmetocCursorDownHandler(jmetoc_point pos);
static void jmetocCursorUpHandler(jmetoc_point pos);
static void jmetocCursorMoveHandler(jmetoc_point pos);

extern void mmi_pen_register_down_handler(void*);
extern void mmi_pen_register_move_handler(void*);
extern void mmi_pen_register_up_handler(void*);

/**
 * 注册所有鼠标的回调函数。
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
void jmetoc_regcursorhandle() {
  mmi_pen_register_down_handler((void*)jmetocCursorDownHandler);
  mmi_pen_register_up_handler((void*)jmetocCursorUpHandler);
  mmi_pen_register_move_handler((void*)jmetocCursorMoveHandler);
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
