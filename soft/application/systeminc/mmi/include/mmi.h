/******************************************************************************/
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    mmi.h                                                         */
/*                                                                            */
/* Description:                                                               */
/*     MMI task entry based on CSW platform.                                  */
/*     @SHK 2006-04-04.                                                       */
/******************************************************************************/
#ifndef _MMI_H_
#define _MMI_H_

#include <csw.h>

#define APP_STATE_IDLE          0x00
#define APP_STATE_CALL          0x01 
#define APP_STATE_INCOMING_CALL 0x02
#define APP_STATE_INCOMING_SMS  0x03
#define APP_STATE_SMS           0x04
#define APP_STATE_PBK           0x05
#define APP_STATE_CALL_PROGRESS 0x06
#define APP_STATE_SMS_MO_PROGRESS  0x07
#define APP_STATE_CALL_MO_PROGRESS 0x08
#define APP_STATE_CALL_MT_PROGRESS 0x09

#define MMI_INIT_STATUS_START           0x00
#define MMI_INIT_STATUS_NW_REG_READY    0x02
#define MMI_INIT_STATUS_SIM_READY       0x04
#define MMI_INIT_STATUS_NOSIM           0x08
#define MMI_INIT_STATUS_NW_REG_FAIL     0x10
#define MMI_INIT_STATUS_SIMPIN          0x20

typedef struct _MMI_PROC_INFO {
    UINT32 nCurrentState;
    UINT32 nNextState;    
    INT8 OperatorName[40];
    INT8 nMode;
    UINT8 nSignalVal;
    UINT8 nSignalValUpdatStatus; // 1: need to update when back to IDLE.
    INT8 nCfwInit;
    UINT8 nDialNumber[21];
    UINT8 nDialNumberPos;
    UINT8 bMTSendMessage;
    UINT8 padding1[1];
    UINT8 nMtNumber[21];
    UINT8 nMtNumberPos;
    UINT8 nListCnt;
    UINT8 nNewSms;  
    UINT8 nSmsTelNumber[21];
    UINT8 nSmsTelPos;
    UINT8 padding2[2];
}MMI_PROC_INFO;

#define MMI_MALLOC(p, BLOCK_TYPE) \
	do \
	{ \
		p = OslMalloc(sizeof(BLOCK_TYPE)); \
		memset((p), 0, sizeof(BLOCK_TYPE)); \
		MMI_ASSERT(p != NULL); \
	} while (0)

#define MMI_FREE(p) \
	do \
	{ \
		OslMfree(p); \
		p = NULL; \
	} while (0)

#define MMI_FAULT_FORK \
	do { \
		mmi_trace(1, "[221]Program enter a fault fork!"); \
		MMI_ASSERT(0); \
	} while (0)

BOOL VW_PowerOnLogo(VOID);

BOOL MMI_InitCfw(VOID);
BOOL MMI_CosEvent2CfwEvent(COS_EVENT* pCosEvent, CFW_EVENT* pCfwEvent);
BOOL MMI_CfwEvProc(CFW_EVENT* pEv);
BOOL MMI_GenernalEventsProc( COS_EVENT* ev, MMI_PROC_INFO* pInfo );


#endif // _H_ 

