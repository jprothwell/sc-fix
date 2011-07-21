#ifndef __ZYX_SYS_H__
#define __ZYX_SYS_H__

#include "zyxtypes.h"

typedef enum
{
    ZYX_PAUSE, //���ⲿ�¼���ϣ�Ӧ��Ӧ���𲢱���״̬
    ZYX_RESUME, //�ⲿ�¼�������ϣ�����Ӧ��
    ZYX_QUIT //Ӧ���˳���Ϣ
}ZYX_SYS_EVENT;

enum
{
    ZYX_KEY_0,									/**<���ּ� 0*/
    ZYX_KEY_1,								/**<���ּ� 1*/
    ZYX_KEY_2,								/**<���ּ� 2*/
    ZYX_KEY_3,								/**<���ּ� 3*/
    ZYX_KEY_4,								/**<���ּ� 4*/
    ZYX_KEY_5,								/**<���ּ� 5*/
    ZYX_KEY_6,								/**<���ּ� 6*/
    ZYX_KEY_7,								/**<���ּ� 7*/
    ZYX_KEY_8,								/**<���ּ� 8*/
    ZYX_KEY_9,								/**<���ּ� 9*/
    ZYX_KEY_LEFT_SOFTKEY,					/**<�����*/
    ZYX_KEY_RIGHT_SOFTKEY,					/**<�����*/
    ZYX_KEY_CENTER_SOFTKEY,					/**<�м����*/
    ZYX_KEY_UP,								/**<����� ��*/
    ZYX_KEY_DOWN,							/**<����� ��*/
    ZYX_KEY_LEFT,							/**<����� ��*/
    ZYX_KEY_RIGHT,							/**<����� ��*/
    ZYX_KEY_OK,								/**<ȷ����*/
    ZYX_KEY_CALL,							/**<ͨ����*/
    ZYX_KEY_END,								/**<������*/
    ZYX_KEY_CLEAR,							/**<�����*/
    ZYX_KEY_STAR,							/**<�Ǻż�*/
    ZYX_KEY_POUND,							/**<���ż�*/
    ZYX_KEY_VOL_UP,							/**<��������*/
    ZYX_KEY_VOL_DOWN,						/**<��������*/
    ZYX_KEY_POWER,    						/**<��Դ��*/
    ZYX_KEY_CAMERA,							/**<�����*/
    ZYX_KEY_BACK,							/**<���ؼ�*/
    ZYX_KEY_UNKOWN
};

/**ö�� �����¼���*/
enum
{
    ZYX_KEY_EVENT_DOWN,								/**<��������*/
    ZYX_KEY_EVENT_UP,								/**<�����ſ�*/
    ZYX_KEY_EVENT_REPEAT,							/**<�����ظ�*/
    ZYX_KEY_EVENT_UNKOWN
};

/**ö�� �������¼���*/
enum
{
    ZYX_PEN_EVENT_DOWN,								/**<�����ʵ���*/
    ZYX_PEN_EVENT_UP,								/**<�����ʷſ�*/
    ZYX_PEN_EVENT_MOVE,								/**<�������϶�*/
    ZYX_PEN_EVENT_UNKOWN
};

typedef int (*ZyxSysevtHandler)( U16 message, U16 param);
typedef int (*ZyxKeyevtHandler) (U16 keycode ,U16 keytype);
typedef int (*ZyxPenevtHandler)( U16 event, U16 x, U16 y);

extern void ZyxRegSysevtCallback (ZyxSysevtHandler handle_sysevt);
extern void ZyxRegKeyboardCallback (ZyxKeyevtHandler handle_keyevt);
extern void ZyxRegPenevtCallback (ZyxPenevtHandler handle_penevt);
extern void ZyxAppQuit(void);
extern void ZyxAppAddTask(char *path,char *param);


////////////////////////////////////////////////////////////////////////
typedef struct
{
	short year;
	char month;
	char day;
	char hour;
	char min;
	char sec;
	char weekday; /* 0=Sunday */
}ZyxTime,*ZyxTimePtr;


typedef void (*ZyxTimerHandler) (int timer_id);

extern void ZyxTrace(char const *fmt,...);

extern void* ZyxAlloc(int size);
extern void ZyxFree(void* mem);

extern int ZyxStartTimer(unsigned int ms,ZyxTimerHandler cb);
extern int ZyxStopTimer(int timer_id);
void ZyxStartSetData(int timer_id, U32 data1, U32 data2, U32 data3, U32 data4);
void ZyxStartGetData(int timer_id, U32* data1, U32* data2, U32* data3, U32* data4);

extern void* ZyxCreateMutex(char *name);
extern void ZyxTakeMutex(void *handle);
extern void ZyxGiveMutex(void *handle);

extern void ZyxSysLockInit();
extern void ZyxSysLockExit();
extern void ZyxSysLockEnter();
extern void ZyxSysLockLeave();

extern void ZyxSRand(int seed);
extern int ZyxRand(void);

extern void ZyxSleep(ULONG mil);

extern U32 ZyxGetSysTicks(void);
extern int ZyxGetSysTime(ZyxTimePtr time);

extern int ZyxGetLCDWidth(void);
extern int ZyxGetLCDHeight(void);
extern int ZyxGetLCDDepth(void);
extern char* ZyxGetDisplayBuf();
extern void ZyxFlush(S32 x1, S32 y1, S32 x2, S32 y2);

extern void ZyxDummy0(void);
extern void ZyxDummy1(void);
extern void ZyxDummy2(void);
extern void ZyxDummy3(void);
extern void ZyxDummy4(void);
extern void ZyxDummy5(void);
extern void ZyxDummy6(void);

#endif

