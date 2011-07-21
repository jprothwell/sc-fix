/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__18____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// 压缩模块 /////////////////////////////////////////////////////////////
//注意：大部分系统中都包含了 zlib 库，如果系统中不包含 zlib 库的话，请将其引入，大约多占用 10k 空间。
//在 COOLSAND 系统中，系统内部虽然没有引入全部的 zlib，但是解压缩核心已经全部引入其中。
//因此解压缩模块，只要引入 uncompr.c 即可。
//在 COOLSAND 系统中，解压缩用的存储管理为系统内核存储区。因此这里也无需注册存储模块。

//#include "zlib/zlib.h"
//void* (*jmetoc_malloc)(int);
//void (*jmetoc_mfree)(void*);

/**
 * 注册存储管理方法。
 * <p>
 * 将 zlib 中的存储管理方法定位到平台的存储管理方法。
 * </p>
 * @param malloc_func 存储分配方法
 * @param free_func 存储释放方法
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
//typedef void* (*_alloc_func)(int);
//typedef void (*_free_func)(void*);
void appzlib_register(_alloc_func alloc_hdlr, _free_func free_hdlr) {
  //jmetoc_malloc = alloc_hdlr;
  //jmetoc_mfree = free_hdlr;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
