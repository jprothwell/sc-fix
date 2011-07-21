/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__11____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// ��Ļ���� /////////////////////////////////////////////////////////////
/**
 * ��Ļ��ա�
 * <p>
 * ʵ����������ð�ɫ��Ϊ��������Ļ��䡣
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
extern void clear_screen();

#if __SUPPORT_SOFTKEY__
#include "____SMARCHBOX__COOLSAND__V1_0_0__26____.h"
#endif

/**
 * ˢ����Ļ��
 * <p>
 * ����ʾ����ˢ��ʵ���Դ棬ʵ����Ļˢ�¡�
 * </p>
 * @param x ��Ļˢ�µ����� X ����
 * @param y ��Ļˢ�µ����� Y ����
 * @param width ˢ��������
 * @param height ˢ������߶�
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
extern void jmetoc_gdi_refresh_buffer(s32 x, s32 y, s32 width, s32 height); //����������ֲģ���ж���
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
