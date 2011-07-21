#ifndef __ZYX_SYS_H__
#define __ZYX_SYS_H__

#include "zyxtypes.h"

typedef enum
{
    ZYX_PAUSE, //有外部事件打断，应用应挂起并保持状态
    ZYX_RESUME, //外部事件处理完毕，激活应用
    ZYX_QUIT //应用退出消息
}ZYX_SYS_EVENT;

enum
{
    ZYX_KEY_0,									/**<数字键 0*/
    ZYX_KEY_1,								/**<数字键 1*/
    ZYX_KEY_2,								/**<数字键 2*/
    ZYX_KEY_3,								/**<数字键 3*/
    ZYX_KEY_4,								/**<数字键 4*/
    ZYX_KEY_5,								/**<数字键 5*/
    ZYX_KEY_6,								/**<数字键 6*/
    ZYX_KEY_7,								/**<数字键 7*/
    ZYX_KEY_8,								/**<数字键 8*/
    ZYX_KEY_9,								/**<数字键 9*/
    ZYX_KEY_LEFT_SOFTKEY,					/**<左软键*/
    ZYX_KEY_RIGHT_SOFTKEY,					/**<右软键*/
    ZYX_KEY_CENTER_SOFTKEY,					/**<中间软键*/
    ZYX_KEY_UP,								/**<方向键 上*/
    ZYX_KEY_DOWN,							/**<方向键 下*/
    ZYX_KEY_LEFT,							/**<方向键 左*/
    ZYX_KEY_RIGHT,							/**<方向键 右*/
    ZYX_KEY_OK,								/**<确定键*/
    ZYX_KEY_CALL,							/**<通话键*/
    ZYX_KEY_END,								/**<结束键*/
    ZYX_KEY_CLEAR,							/**<清除键*/
    ZYX_KEY_STAR,							/**<星号键*/
    ZYX_KEY_POUND,							/**<井号键*/
    ZYX_KEY_VOL_UP,							/**<音量增加*/
    ZYX_KEY_VOL_DOWN,						/**<音量降低*/
    ZYX_KEY_POWER,    						/**<电源键*/
    ZYX_KEY_CAMERA,							/**<照相键*/
    ZYX_KEY_BACK,							/**<返回键*/
    ZYX_KEY_UNKOWN
};

/**枚举 按键事件。*/
enum
{
    ZYX_KEY_EVENT_DOWN,								/**<按键按下*/
    ZYX_KEY_EVENT_UP,								/**<按键放开*/
    ZYX_KEY_EVENT_REPEAT,							/**<按键重复*/
    ZYX_KEY_EVENT_UNKOWN
};

/**枚举 触摸屏事件。*/
enum
{
    ZYX_PEN_EVENT_DOWN,								/**<触摸笔点下*/
    ZYX_PEN_EVENT_UP,								/**<触摸笔放开*/
    ZYX_PEN_EVENT_MOVE,								/**<触摸笔拖动*/
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

