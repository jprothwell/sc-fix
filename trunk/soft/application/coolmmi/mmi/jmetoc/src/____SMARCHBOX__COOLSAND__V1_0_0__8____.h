/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__8____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// �޷��ҵ�t����Դ /////////////////////////////////////////////////////////////
u16 SMARCH_AUTO_BIN[] = {
    'D', ':', '/', 'S', 'm', 'a', 'r', 'c', 'h', 'B', 'o', 'x', '/',
    'j', 'm', 'e', 't', 'o', 'c', '_', 'a', 'u', 't', 'o', '.', 'b', 'i', 'n', 0};
u16 SMARCH_AUTO_SYM[] = {
    'D', ':', '/', 'S', 'm', 'a', 'r', 'c', 'h', 'B', 'o', 'x', '/',
    'j', 'm', 'e', 't', 'o', 'c', '_', 'a', 'u', 't', 'o', '.', 's', 'y', 'm', 0};

#ifndef ____NO_AUTO_DOWNLOAD____
#include "../jmetoc_auto_bin.h"
#include "../jmetoc_auto_sym.h"
#endif

#ifndef ____NO_AUTO_DOWNLOAD____
void uncompressData(char* compressData, char* uncompressData, u32 len, s32 handle) {
  u32 buffersize = jmetoc_unzip(compressData, uncompressData, len);
  writeFile(handle, uncompressData, buffersize);
  closeFile(handle);
}
s32 checkFile(u16* path) {
  s32 handle;
  handle = openFile(path);
  if (handle > 0) {
    closeFile(handle);
    deleteFile(path);
  }
  handle = createFile(path);
  return handle;
}
#endif

/**
 * �޷��ҵ������Դ��ʱ��
 * <p>
 * ����Դ��Ч��ʱ��ϵͳ�����ѹ�����õ�С�γ���T�������������ó���
 * �ó������Զ��������������ذٱ���ͻ��˵ġ�
 * ���ҵ��ͻ���������Ϻ󣬽��Զ������ٱ��䡣
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-04-28
 */
static void notifyNoResourceFinded() {
  jmetoc_trace("no sd");
  reportNoSDCard();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
