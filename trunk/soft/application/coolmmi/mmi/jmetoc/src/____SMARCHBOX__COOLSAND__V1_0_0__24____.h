/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__24____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// GUI ģ�� /////////////////////////////////////////////////////////////

/**
 * ��ʾ��ʾ��
 * <p>
 * ��ʾ����һ���ֻ�ϵͳ�ж����ڣ�����Ϊһ�������ķ�ȫ���Ĵ��ڡ�
 * һ���ڸô��������һЩ��Ϣ�����û�����һЩ��ʾ��Ϣ��
 * </p>
 * @param info ��ʾ��Ϣ
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
extern void dm_reset_context(void);
JMETOCAPI void showAlter(u16* info) {
  dm_reset_context();
  ShowCategory165Screen(null, null, null, null, info, null, null);
}

/**
 * ȥ�� GPRS ͼ�ꡣ
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
extern void hideGIcon();                    //�� jmetoc.c ����

/**
 * ȷ�����뷵�ء�
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
extern void jmetoc_confirm_inputForm();     //�� jmetoc.c ����

/**
 * ȡ�����뷵�ء�
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
extern void jmetoc_cancel_inputForm();      //�� jmetoc.c ����

/**
 * �������뷨���ڡ�
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
extern void jmetoc_entry_inputForm();       //�� jmetoc.c ����
extern void jmetoc_entry_inputFormPW();     //�� jmetoc.c ����
extern void jmetoc_entry_inputFormNum();    //�� jmetoc.c ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
