#ifndef __NEW_EARPHONE_PLAN__
#define __NEW_EARPHONE_PLAN__

/* Modified MINVOLT_EARPHONE_NOT_IN_PHONE and MAXVOLT_EARPHONE_IN_PHONE */

#if defined(__PROJECT_GALLITE_C01__)
#define MINVOLT_EARPHONE_NOT_IN_PHONE  1600  //1500 ,�������û�����ֻ��ϣ���ƽֵ��Сֵ
#define MAXVOLT_EARPHONE_IN_PHONE  1400  //1500 ,���VMIC��,���������ֻ��ϣ�û����������������ƽֵ�ķ�Χ
#else
#define MINVOLT_EARPHONE_NOT_IN_PHONE  1500  //�������û�����ֻ��ϣ���ƽֵ��Сֵ

#define MAXVOLT_EARPHONE_IN_PHONE  1500  //���VMIC��,���������ֻ��ϣ�û����������������ƽֵ�ķ�Χ

#endif
#define MINVOLT_EARPHONE_IN_PHONE  300

#define MAXVOLT_FOR_EARPHONE_PRESSKEY  300  //���˶�������������ƽֵ�����ֵ
#endif
