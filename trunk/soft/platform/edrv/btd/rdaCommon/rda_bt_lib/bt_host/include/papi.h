/*
 * MODULE NAME:    papi.h
 * DESCRIPTION:    PAPI Operating System Independent Header
 * AUTHOR:         Martin Mellody
 * DATE:           10 March 2000
 *
 * SOURCE CONTROL: $Id: papi.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 Parthus Technologies plc.
 *     All rights reserved.
 *
 */

#ifndef PRH_PAPI_H
#    define PRH_PAPI_H

#include "host_config.h"


#include <stdarg.h>
#include "cos.h"
/* 
	OS specific stuff is in here. The build mechanism 
	must make sure the correct one of these is found.
*/

#if COMBINED_HOST==0
#include "papi_impl.h"	
#endif

/* Error return codes */
#define pENOERROR		0 /* must be zero */
#define pENOMEMORY		1
#define pENORESOURCE	2
#define pEINVALIDPARM	3
#define pEWOULDBLOCK	4
#define pELOCKERROR		5
#define pEIOERROR		6	
#define pENOTSUPPORTED	7
#define pEUNKNOWNERROR	8

/* Logging levels for pDebugPrint */
#define pLOGFATAL		0
#define pLOGCRITICAL	1
#define pLOGERROR		2
#define pLOGWARNING	3
#define pLOGNOTICE		4
#define pLOGDEBUG		5


/*Logging layers for pDebug */
#define pLOGSYSTEM 1     /*0*/
#define pLOGHCI 2           /*1*/
#define pLOGMANAGER 4   /*2*/
#define pLOGL2CAP 8     /*3*/
#define pLOGRFCOMM 16   /*4*/
#define pLOGSDP 32  /*5*/
#define pLOGOBEX 64   /*6*/
#define pLOGTCS 128   /*7*/
#define pLOGA2DP 256  //Zhu Jianguo added it. 2008.9.18
#define pLOGHFP    512
#define pLOGALWAYS 0xffff

#define LOG_EVENT_STRING_L2_PSM_INDEX  0   /*13*/
#define LOG_EVENT_STRING_L2_CONFIG_SM_INDEX 13

/* the start index num for each file*/

#define LOG_L2CAP_L2CAP_PRIMITIVES_INDEX   0  /* 1*/
#define LOG_L2CAP_L2_CONFIG_SM_INDEX          1  /*4*/
#define LOG_L2CAP_L2_CONNCECT_SM_INDEX     5 /*11*/
#define LOG_L2CAP_L2_ENC_DEC_INDEX             16/*29*/
#define LOG_L2CAP_L2_GROUPS_INDEX               45 /*5*/
#define LOG_L2CAP_L2_PKT_HANDLER_INDEX      50/*1*/  
#define LOG_L2CAP_L2_PSM_INDEX      51/*8*/                
#define LOG_L2CAP_L2_SAR_INDEX    59 /**/  

#define LOG_SYS_HOST_TIME_INDEX   0  /*3*/
#define LOG_SYS_PAPI_WIN32_INDEX 3 /*14*/  

#define LOG_A2TP_AVDTP_CALLBACKS_INDEX 0 /*5*/
#define LOG_A2TP_AVRCP_CALLBACKS_INDEX 5 /*2*/
#define LOG_A2TP_AVRCP_CORE_INDEX 7 /*3*/

#define LOG_ALWAYS_AVRCP_CORE_INDEX 0 /*3*/
#define LOG_ALWAYS_PAPI_WIN32_INDEX 3  /*1*/
#define LOG_ALWAYS_RFCOMM_UNIT  4 /*1*/

#define LOG_RFC_RFCOMM_CTRL_CHAN_INDEX 0 /*7*/
#define LOG_RFC_RFCOMM_DLC_STATE_INDEX 7  /*11*/
#define LOG_RFC_RFCOMM_FRAMES_INDEX 18 /*3*/
#define LOG_RFC_RFCOMM_L2AUTO_STATE_INDEX  21  /*4*/
#define LOG_RFC_RFCOMM_L2CAP_INDEX 25 /*1*/
#define LOG_RFC_RFCOMM_UE_API_INDEX    26/*3*/
#define LOG_RFC_STACK_ENT_INDEX  29/*1*/


#define LOG_SDP_SDP_CALLBACKS_INDEX 0/*6*/
#define LOG_SDP_SDP_DB  6/*1*/
#define LOG_SDP_SDP_LE_REQUEST_DECODER   7/*3*/
#define LOG_SDP_SDP_LE_RESPONSE_DECODER  10/*4*/
#define LOG_SDP_UPF_5  14 /*2*/

#define BT_TIMER_PAPI	(HVY_TIMER_IN |COS_BT_TIMER_ID_BASE)

typedef void (*bt_timer_callback)(void *);

typedef struct BT_TIMER_EVT
{
	bt_timer_callback callbackfun;
	void *argv;
	u_int32 timerHandle;
//	u_int32 reverseD;
}BT_TIMER_EVT;

#define MAX_BT_TIMER	20

int pInit(void *platformConfig);
void pShutdown(void);

#define pTIMER_ONESHOT 1
#define pTIMER_PERIODIC 2
#if COMBINED_HOST==0
t_pTimer pTimerCreate(u_int32 timeoutTenths, void(*callbackFunc)(void *), void *callbackArg, u_int8 flags);
t_pTimer pTimerCreateMs(u_int32 timeoutTenths, void(*callbackFunc)(void *), void *callbackArg, u_int8 flags);
int pTimerCancel(t_pTimer timerHandle);
BOOL pIsTimerCreated(t_pTimer timerHandle);

/*obsolete:*/
int pTimerSet(unsigned int delay, void (*callbackFunc)(void *), void *callbackParm, int *timerHandle);

t_pMutex pMutexCreate(u_int8 initialState);
void pMutexFree(t_pMutex mutex);
int pMutexLock(t_pMutex mutex);
int pMutexUnlock(t_pMutex mutex);
#else
#define pTimerCreate(timeoutTenths, callbackFunc, callbackArg, flags)  host_timer_create(timeoutTenths, callbackFunc, callbackArg, flags)
#define pTimerCancel(timerHandle)                                      host_timer_cancel(timerHandle)
#define pMutexCreate(initialState)          (0)
#define pMutexFree(mutex) 
#define pMutexLock(mutex)
#define pMutexUnlock(mutex)
#define prh_hci_GetWriteBuffer              host_buf_GetWriteBuffer
#define prh_hci_FreeWriteBuffer             host_buf_FreeWriteBuffer
#endif
/* 
Notes:
    Events must retain the 'set' state until waited on, and then clear. 
       i.e. After any number of pEventSet(x) calls,  pEventWait(x) will not block, although a second pEVentWait(x) will block.
    Mutexes are always locked and unlocked by the same thread of execution. 
    Timer expiry callbacks may add and remove timers via pTimerCreate and pTimerCancel. 
    -MM
*/

#if COMBINED_HOST==0

t_pEvent pEventCreate(u_int8 initialState);
void pEventFree(t_pEvent event);
int pEventWait(t_pEvent event, int(*callbackFunc)(void *), void *callbackParam);
int pEventSet(t_pEvent event);


void *pMalloc(size_t size);
void pFree(void *buffer);
#endif


#if PRH_RFCOMM_RSE_DATA_PUMP_ENABLE==1
u_int32 pExGetCurrentTicks(void);
u_int32 pExGetTickDifference(u_int32 start, u_int32 end);
#endif

/* 
	The __cdecl modifier is required in the windows build for varargs functions.    
    -MM
*/
#if pDEBUG
#  ifdef _WINDOWS
	  int __cdecl pDebugPrintEX(int logLevel, int logLayer, char *formatString, ... );
	  int __cdecl pDebugPrint(int logLevel, char *formatString, ... );
#else
        extern int RDA_Debug_PrintEX(int logLevel,int logLayer, char *formatString, ...) ;
        extern int RDA_Debug_Print(int logLevel, char *formatString, ... );
	  int pDebugPrintEX(int logLevel, int loglayer,char *formatString, ... );
	  int pDebugPrint(int logLevel,char *formatString, ... );
#  endif
#endif

#if pDEBUG
    int pDebugSleep(int seconds);   
    void pDebugCheck(void);
    /* pDebugDumpFunc() should be called via the macro in host_config.h (like pDebugPrintf) */
//    int pDebugDumpFuncEX(int logLevel, int logLayer, u_int8 *buffer, u_int32 len); 
//	int pDebugDumpFunc(int logLevel, u_int8 *buffer, u_int32 len); 
#endif                                             

#ifndef NULL
#    define NULL ((void *)0)
#endif
#ifndef TRUE
#    define TRUE 1
#endif
#ifndef FALSE
#    define FALSE 0
#endif

#define pNULL NULL

/* This should be used in code and provided where available for specific patforms */
#ifndef pINLINEFUNC
#    define pINLINEFUNC		
#endif

/* If we have libc then papi_impl.h can just #define these so check before declaring the replacements */
#if COMBINED_HOST==0
#ifndef pMemcpy
	void *pMemcpy(void *dest, void *src, size_t numbytes);
#endif
#ifndef pMemset
	void *pMemset(void *buf, int m_byte, size_t numbytes);
#endif

#ifndef pMemcmp
	int pMemcmp(void *buf1, void *buf2, size_t numbytes);
#endif

#ifndef pMemchr
	void *pMemchr(void *buf, int m_byte, size_t numbytes);
#endif

#ifndef pStrlen
	size_t pStrlen(const char *s);
#endif

#ifndef pStrncmp
	int pStrncmp(const char *s1, const char *s2, size_t n);
#endif

#ifndef pStrcmp
	int pStrcmp(const char *s1, const char *s2);
#endif

#ifndef pStrcpy
	char *pStrcpy(char *s1, const char *s2);
#endif

#ifndef pStrncpy
	char *pStrncpy(char *s1, const char *s2, size_t n);
#endif
#else
#define pMemcpy memcpy
#define pMemset memset
#define pMemcmp memcmp
#define pMemchr memchr
#define pStrcmp strcmp
#define pStrncmp strncmp
#define pStrlen strlen
#define pStrcpy strcpy
#define pStrncpy strncpy
#define pMalloc (NULL)
#define pFree
#endif

#endif	/* PAPI_H */

