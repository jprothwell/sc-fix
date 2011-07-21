/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__1____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// 输出版本信息 /////////////////////////////////////////////////////////////
#include "../config.h"

#define ____PRINT_VERSION____

/**
 * 输出版本信息。
 * <p>
 * 当定义了宏 ____PRINT_VERSION____ 之后，系统在启动最初就会调用本方法。
 * 本方法打印一个百宝箱的标准版权信息。
 * </p>
 * <p>
 * 希望移植者在任何可以输出版权信息的平台输出以下版权信息，以示对版权和作者的尊重。
 * 版权信息如下：
 * =====================================================
 * = SmarchBox [platform]
 * = Version x.x.x"
 * = Copyright (c) Smarch 2007
 * =====================================================
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
void printVersionInfo() {
  jmetoc_trace("=====================================================");
  jmetoc_trace("= SmarchBox COOLSAND");
	jmetoc_trace("= Version 1.x.x");
	jmetoc_trace("= Copyright (c) Smarch 2007");
	jmetoc_trace("=====================================================");
}

/**
 * 获得系统版本。
 * @return 系统版本
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
JMETOCAPI u32 getSmarchBoxVersion() {
  return (VERSION1 << 24) | (VERSION2 << 16) | (VERSION3 << 8);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
