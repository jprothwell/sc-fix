/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__3____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// ϵͳ��� /////////////////////////////////////////////////////////////
extern void regShell(unsigned short* name);
/**
 * �˳��ٱ��䡣
 * <p>
 * ���������ϵͳ��ȫ�˳�����á������ⲿ��չ��
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-03-02
 */
extern void smarchbox_quit(void); //����������ֲģ���ж���

/**
 * ����ٱ��䡣
 * <p>
 * ���������ϵͳ����ʱ��δ���κγ�ʼ������ʱ���á������ⲿ��չ��
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-03-02
 */
extern void smarchbox_entry(void); //����������ֲģ���ж���

/**
 * �豸���������������ˡ�
 * <p>
 * ����ƽ̨��ֲ��Ա���Ծ����豸���нӿڽ��м�⡣
 * </p>
 * @return �Ƿ����������ˣ������˳���ǰ�ٱ���
 * @author runasea
 * @version 1.0.0.1, 2009-09-02
 */
extern signed char jmetocSmartAdaptRobot(void); //����������ֲģ���ж���

/**
 * ϵͳ������ڡ�
 * <p>
 * regShell ��������ע��ͻ��˽���� UI �б����磺list������Ϸ��app����Ӧ�õȡ�
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-03-02
 */
void jmetoc_mtk_entry(){
  smarchbox_entry();
  regShell((unsigned short*)L"");
  startMIDlet();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
