/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__10____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// �洢���� /////////////////////////////////////////////////////////////
#ifndef WIN32
void* COS_Malloc (
  u32 nSize,
  int nHeapID
);
u8 COS_Free (
  void* pMemBlock
);
#endif
#define JMETOC_MALLOC(size) COS_Malloc((u32)size,1)                         //��̬�洢���䣬Ҫ��֤�ܹ�������洢������������й���
#define JMETOC_FREE(p) COS_Free((void*)p)                                   //��̬�洢�ͷ�
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
