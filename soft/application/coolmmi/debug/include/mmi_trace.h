/******************************************************************************************
	Header file of mmi_trace.c. Includes declaration of trace switches and functions.
*******************************************************************************************/

#ifndef _MMI_TRACE_H_
#define _MMI_TRACE_H_

#include "cswtype.h"
#include "coolprofile.h"
#ifndef MMI_ON_WIN32
#include "dbg.h"
#endif

/////////////////////////////////////////
// Trace switch for MMI entry module

extern BOOL g_sw_MAINENTRY;
/////////////////////////////////////////
// Trace switch for MMI debug module

extern BOOL g_sw_DEBUG;
extern BOOL g_sw_trace_check;

/////////////////////////////////////////
// Trace switch for Adaptation Layer

extern BOOL g_sw_ADP_COMMON;
extern BOOL g_sw_ADP_POWERON;
extern BOOL g_sw_ADP_POWEROFF;
extern BOOL g_sw_ADP_CC;
extern BOOL g_sw_ADP_SMS;
extern BOOL g_sw_ADP_PHB;
extern BOOL g_sw_ADP_SIM;
extern BOOL g_sw_ADP_SS;
extern BOOL g_sw_ADP_CALLLOG;
extern BOOL g_sw_ADP_SAT;
extern BOOL g_sw_ADP_NW;
extern BOOL g_sw_ADP_SMU;
extern BOOL g_sw_ADP_AUD;
extern BOOL g_sw_ADP_BT;//Renwy add for BT debug 2008/06/16
extern BOOL g_sw_ADP_GPRS;

/////////////////////////////////////////
// Trace switch for MMI Layer

extern BOOL g_sw_CC;
extern BOOL g_sw_SMS;
extern BOOL g_sw_PHB;
extern BOOL g_sw_SS;
extern BOOL g_sw_CALLLOG;
extern BOOL g_sw_SAT;
extern BOOL g_sw_NW;
extern BOOL g_sw_SMU;
extern BOOL g_sw_MMS;
extern BOOL g_sw_WAP;

extern BOOL g_sw_GDI;
extern BOOL g_sw_GUI;
extern BOOL g_sw_FRAMEWORK;
extern BOOL g_sw_SETTING;
extern BOOL g_sw_CALCULATOR;
extern BOOL g_sw_POWERON;
extern BOOL g_sw_POWEROFF;
extern BOOL g_sw_IDLE;
extern BOOL g_sw_PM;
extern BOOL g_sw_IM;
extern BOOL g_sw_ORGANIZER;
extern BOOL g_sw_NVRAM;
extern BOOL g_sw_CAMERA;
extern BOOL g_sw_MED;
extern BOOL g_sw_MED_Aud;
extern BOOL g_sw_MED_Vid;
extern BOOL g_sw_MED_Img;
extern BOOL g_sw_BT;//Renwy add for BT debug 2008/06/16
extern BOOL g_sw_Vidrec;//changed by zhangl

extern BOOL g_sw_DUALSIM;

/////////////////////////////////////////
// Trace switch for Platform Layer

extern BOOL g_sw_MIDI;
extern BOOL g_sw_LCD;
extern BOOL g_sw_AUDIO;
extern BOOL g_sw_SYSTEM;
extern BOOL g_sw_FMGR;
extern BOOL g_sw_CAM;
extern BOOL g_sw_MP3;
extern BOOL g_sw_MP4;
//chenqiang add 070613 B
extern BOOL g_sw_TPL;
//chenqiang add 070613 E
//wangzl:070615 add B
extern BOOL g_sw_HandW;
//wangzl:070615 add E
//chenhe++
extern BOOL g_sw_SREC;
//chenhe--
extern BOOL g_sw_SP;
extern BOOL g_sw_VdoRec;//chenhe,jasper
extern BOOL g_sw_Ebr;  //ebook reader
extern BOOL g_sw_WpSsPwon;//gdm , jessamine
extern BOOL g_sw_DataMgr;//guojian jassmine 20080624
extern BOOL g_sw_FMR;
extern BOOL g_sw_notepad;
extern BOOL g_sw_ALARM;//gdm 090605
extern BOOL g_sw_GAME;//kecx added on 090817

/////////////////////////////////////////
// Prototype definition of trace functions


enum MMI_TRACE_LEVEL
{
	MMI_TRACE_LEVEL_FORCE_OUTPUT,
	MMI_TRACE_LEVEL_1,
	MMI_TRACE_LEVEL_2,
	MMI_TRACE_LEVEL_3,
	MMI_TRACE_LEVEL_4,
	MMI_TRACE_LEVEL_5,
	MMI_TRACE_LEVEL_6,
	MMI_TRACE_LEVEL_7,
	MMI_TRACE_LEVEL_8,
	MMI_TRACE_LEVEL_9,
	MMI_TRACE_LEVEL_10,
	MMI_TRACE_LEVEL_11,
	MMI_TRACE_LEVEL_12,
	MMI_TRACE_LEVEL_13,
	MMI_TRACE_LEVEL_14,
	MMI_TRACE_LEVEL_15,
	MMI_TRACE_LEVEL_16
};

// FIXME: Use the same level for all MMI traces 1
#define MMI_TRACE_LEVEL   1
#ifdef MMI_ON_WIN32
	#define __FUNCTION__ __FILE__
  #define __func__ NULL
#endif
/*
FUNC: 
	Intialization for MMI trace function.
**/
/*
FUNC:
	Output trace information to UART.
PARAM:
[bSwtich]
	Output switch. TRUE, means output; FALSE, means DO NOT output.
[fmt]
	The trace information that you want to output. Refer the usage of ANSI C printf().
**/
#ifdef MMI_ON_WIN32

#define WIN32TRACE1           \
  do                          \
	{                           \
    if (bSwitch)              \
    {                         \
      char tch[1024];         \
      va_list var;            \
      va_start(var, fmt);     \
      vsprintf(tch,fmt,var);  \
      va_end(var);            \
                              \
      strncat(tch,"\n",1024); \
      WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),tch,strlen(tch),NULL,NULL);  \
    }                         \
  } while (0);

#define WIN32TRACE0           \
  do                          \
  {                           \
    BOOL bSwitch = TRUE;      \
    WIN32TRACE1               \
  } while(0);                 \

void dbg_TraceOpen(BOOL bTrace);
INLINE VOID mmi_trace_init()
{
  dbg_TraceOpen(TRUE);
}

__inline void mmi_trace(BOOL bSwitch, char* fmt,...)
{
	WIN32TRACE1;
}

#else
INLINE VOID mmi_trace_init()
{
    dbg_TraceOpen(TRUE);
    dbg_TraceSetOutputMask(MMI_TRACE_LEVEL, 1);
}

/*
FUNC:
	Output trace information to UART.
PARAM:
[bSwtich]
	Output switch. TRUE, means output; FALSE, means DO NOT output.
[fmt]
	The trace information that you want to output. Refer the usage of ANSI C printf().
**/
#ifndef MMI_NO_TRACE
#define mmi_trace(trace_level, fmt, ...) \
	do \
	{ \
			dbg_TraceOutputText(trace_level, (fmt), ##__VA_ARGS__); \
	} while (0)
#else
#define mmi_trace(trace_level, fmt, ...)
#endif // MMI_NO_TRACE
#endif

// Add 2 new trace macros for print the function name when enter or leave a function.
#define TRC_ENTER_FUNC(sw) mmi_trace((sw), "[221]Enter %s()", __FUNCTION__)
#define TRC_LEAVE_FUNC(sw) mmi_trace((sw), "[221]Leave %s()", __FUNCTION__)

PUBLIC void mmi_trace_hex(BOOL flag, int size, void *data); //wufasong added 2007.01.19
/*
MACRO:
	This strengthen mmi_trace function, which can print the trace information include function name, file name and line number.
PARAM:
[bSwtich]
	Output switch. TRUE, means output; FALSE, means DO NOT output.
[FMT]
	The trace information that you want to output. Refer the usage of ANSI C printf().
**/
//This is the pre identifier for format the trace information, which just used in TRACE_EF.
//When you use TRACE_EF to print the trace information, it start by PRE_IDENTIFIER. It's help us to read the trace information.
//And you can modify this for your own's moudle.
#define PRE_IDENTIFIER ">>>>>> "

/*
MACRO:
	This print the formated information using PRE_IDENTIFIER, to be easy to get the information.
PARAM:
[bSwtich]
	Output switch. TRUE, means output; FALSE, means DO NOT output.
[FMT]
	The trace information that you want to output. Refer the usage of ANSI C printf().
**/
#ifdef MMI_ON_WIN32
	__inline void TRACE_EF(BOOL bSwitch, char* fmt, ...)
{
  WIN32TRACE1;
}
#else
#define TRACE_EF(bSwitch,FMT,mmi_args...)\
	do {\
	mmi_trace((bSwitch), (FMT), ##mmi_args);\
	mmi_trace(bSwitch, "~~ trace information is in Func: %s, File: %s, Line: %d\n\n",   __FUNCTION__, __FILE__, __LINE__);\
	}while (0)
#endif

/*Guojian Add Start Ver: GreenStone  on 2009-2-10 19:44 */
#ifdef MMI_ON_WIN32 /* zhuoxz,2009-4-22 */

	INLINE void Trace(char* fmt, ...)
	{
	  WIN32TRACE0;
	}

#define Trace2
#define Trace3

#define kal_prompt_trace mmi_trace

#else /* #ifdef MMI_ON_WIN32 */

#define Trace(fmt, ...) \
	do \
	{ \
			dbg_TraceOutputText(g_sw_DEBUG, (fmt), ##__VA_ARGS__); \
	} while (0)

#define Trace2(fmt, arg_type,  ...) \
	do \
	{ \
			dbg_TraceOutputText(g_sw_DEBUG, (arg_type), ##__VA_ARGS__); \
	} while (0)

#define Trace3(fmt, arg_type,  ...) \
	do \
	{ \
			dbg_TraceOutputText(g_sw_DEBUG, (arg_type), ##__VA_ARGS__); \
	} while (0)

#define kal_prompt_trace(fmt, arg_type,  ...) \
	do \
	{ \
			dbg_TraceOutputText(g_sw_DEBUG, (arg_type), ##__VA_ARGS__); \
	} while (0)
#endif /* #ifdef MMI_ON_WIN32 */
/*Guojian Add End  Ver: GreenStone  on 2009-2-10 19:44 */

/*
MACRO:
	The macro supplies a convnient way to know where the function is running.
PARAM:
[message]
	The information you want to print.
**/
#define TRACE_SMU_FUNC(message)\
	mmi_trace(g_sw_ADP_SMU || g_sw_SMU, "%s %s, Func: %s, File: %s, Line: %d", PRE_IDENTIFIER, (message), __FUNCTION__, __FILE__, __LINE__)

/*
MACRO:
	The macro supplies a convnient way to know whether the function is running.
PARAM:
	nil	
**/
#define TRACE_SMU_FUNCTION()\
	mmi_trace(g_sw_SMU, " %s entry %s function, File: %s, Line: %d", PRE_IDENTIFIER, __FUNCTION__, __FILE__, __LINE__)


#define TRACE_SYS_FUNC(message)\
	mmi_trace(g_sw_SYSTEM, "%s %s, Func: %s, File: %s, Line: %d", PRE_IDENTIFIER, (message), __FUNCTION__, __FILE__, __LINE__)

#define TRACE_SYS_FUNCTION()\
	mmi_trace(g_sw_SYSTEM, " %s entry %s function, File: %s, Line: %d", PRE_IDENTIFIER, __FUNCTION__, __FILE__, __LINE__)
/*******************used by system****************************/

/*******************used by Audio****************************/
#define TRACE_AUD_FUNC(message)\
	mmi_trace(g_sw_AUDIO, "%s %s, Func: %s, File: %s, Line: %d", PRE_IDENTIFIER, (message), __FUNCTION__, __FILE__, __LINE__)

#define TRACE_AUD_FUNCTION()\
	mmi_trace(g_sw_AUDIO, " %s entry %s function, File: %s, Line: %d", PRE_IDENTIFIER, __FUNCTION__, __FILE__, __LINE__)
/*******************used by Audio**************************/
/*WUZC Add Start For MMIFS Ver:    on 2007-1-24 11:3 */
#define TRACE_FMGR_FUNC(message)\
	mmi_trace(g_sw_FMGR, "%s %s, Func: %s, File: %s, Line: %d", PRE_IDENTIFIER, (message), __FUNCTION__, __FILE__, __LINE__)

#define TRACE_FMGR_FUNCTION()\
	mmi_trace(g_sw_FMGR, " %s entry %s function, File: %s, Line: %d", PRE_IDENTIFIER, __FUNCTION__, __FILE__, __LINE__)

#define TRACE_CAM_FUNCTION()\
	mmi_trace(g_sw_CAM, " %s entry %s function, File: %s, Line: %d", PRE_IDENTIFIER, __FUNCTION__, __FILE__, __LINE__)


/*-----------------------------------------------------*/
#define   FMR_TRACE()\
    mmi_trace(g_sw_FMR, "[########FMRadio] %s() %s; %d;",__FUNCTION__, __FILE__, __LINE__)
    
/*-----------------------------------------------------*/    
#ifdef MMI_ON_WIN32
 __inline void FMR_TRACE_ARG(char* fmt,...)
{
  WIN32TRACE0;
}
#else

#define   FMR_TRACE_ARG(FMT,mmi_args...)\
	mmi_trace(g_sw_FMR, (FMT), ##mmi_args)
#endif	
#define kal_mem_cpy(a,b,c) memcpy(a,b,c)
#define kal_mem_set(a,b,c) memset(a,b,c)
	
/*-----------------------------------------------------*/	
#if 0
extern void MMI_Sleep(UINT32 um);
#endif //0

//add by elisa for add msg trace on 2009-6-2

//#define MMI_MESSAGE_TRACE                         // the key to contr if or not open the mmi message trace by elisa 2009.6.10
#ifdef   MMI_MESSAGE_TRACE

#define EVID_CASE(nEvId) #nEvId 
extern 	PCSTR GetMEGName(UINT32 nEvId);
extern	PCSTR GetModName(UINT32 modID);
extern 	void MMIMessageNameCheck();
#endif  //MMI_MESSAGE_TRACE

//this switch to control memory leak check
//#define MMI_MEMLEAK_CHK


#ifdef MMI_MEMLEAK_CHK
void addMemMalloc(char *function,int line,void * addr,UINT32 size);
void delMemMalloc(void *addr);
void mmi_memleakCHK(void);

void *MMI_Malloc(char *file,char *function,int line,UINT32 size);
void MMI_Free(void *addr);
#endif




#endif //_MMI_TRACE_H_

