/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__18____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// ѹ��ģ�� /////////////////////////////////////////////////////////////
//ע�⣺�󲿷�ϵͳ�ж������� zlib �⣬���ϵͳ�в����� zlib ��Ļ����뽫�����룬��Լ��ռ�� 10k �ռ䡣
//�� COOLSAND ϵͳ�У�ϵͳ�ڲ���Ȼû������ȫ���� zlib�����ǽ�ѹ�������Ѿ�ȫ���������С�
//��˽�ѹ��ģ�飬ֻҪ���� uncompr.c ���ɡ�
//�� COOLSAND ϵͳ�У���ѹ���õĴ洢����Ϊϵͳ�ں˴洢�����������Ҳ����ע��洢ģ�顣

//#include "zlib/zlib.h"
//void* (*jmetoc_malloc)(int);
//void (*jmetoc_mfree)(void*);

/**
 * ע��洢��������
 * <p>
 * �� zlib �еĴ洢��������λ��ƽ̨�Ĵ洢��������
 * </p>
 * @param malloc_func �洢���䷽��
 * @param free_func �洢�ͷŷ���
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
