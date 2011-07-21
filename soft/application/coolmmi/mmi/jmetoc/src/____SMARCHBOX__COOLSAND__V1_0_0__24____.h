/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__24____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// GUI 模块 /////////////////////////////////////////////////////////////

/**
 * 显示提示框。
 * <p>
 * 提示框在一般手机系统中都存在，表现为一个跳出的非全屏的窗口。
 * 一半在该窗口中添加一些信息来给用户传达一些提示信息。
 * </p>
 * @param info 提示信息
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
extern void dm_reset_context(void);
JMETOCAPI void showAlter(u16* info) {
  dm_reset_context();
  ShowCategory165Screen(null, null, null, null, info, null, null);
}

/**
 * 去掉 GPRS 图标。
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
extern void hideGIcon();                    //在 jmetoc.c 定义

/**
 * 确定输入返回。
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
extern void jmetoc_confirm_inputForm();     //在 jmetoc.c 定义

/**
 * 取消输入返回。
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
extern void jmetoc_cancel_inputForm();      //在 jmetoc.c 定义

/**
 * 进入输入法窗口。
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
extern void jmetoc_entry_inputForm();       //在 jmetoc.c 定义
extern void jmetoc_entry_inputFormPW();     //在 jmetoc.c 定义
extern void jmetoc_entry_inputFormNum();    //在 jmetoc.c 定义
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
