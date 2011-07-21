/******************************************************************************/
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    uim.h                                                         */
/*                                                                            */
/* Description:                                                               */
/*     UI management for MMI Demo.                                            */
/*     @SHK 2006-04-04.                                                       */
/******************************************************************************/
#ifndef _UIM_H
#define _UIM_H

#include <csw.h>

#include "mmi.h"

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

extern const UINT16 power_on_logo_1[];
extern const UINT16 power_on_logo_2[];
extern const UINT16 idle_screen[];
extern const UINT16 call_screen[];
extern const UINT16 call_mo_screen[];
extern const UINT16 call_progress_screen[];
extern const UINT16 call_err_screen[];
extern const UINT16 call_end_screen[];
extern const UINT16 call_mt_screen[];
extern const UINT16 sms_screen[];
extern const UINT16 init_note[];
extern const UINT16 init_note_sim[];
extern const UINT16 sms_new_screen[];
extern const UINT16 sms_mo_progress_screen[];
extern const UINT16 sms_err_note_screen[];
extern const UINT16 sms_ok_note_screen[];
extern const UINT16 sms_status_report_screen[];
extern const UINT16 pbk_screen[];
extern const UINT16 bmp_signal0[];
extern const UINT16 bmp_signal1[];
extern const UINT16 bmp_signal2[];
extern const UINT16 bmp_signal3[];
extern const UINT16 bmp_signal4[];
extern const UINT16 bmp_signal5[];
extern const UINT16 init_note_nw[];
extern const UINT16 init_note_enter_pin[];

#define IS_46000_OPERATOR(OperatorId) (OperatorId[0] == 0x04 && OperatorId[1] == 0x06 && OperatorId[2] == 0x00 && \
                                       OperatorId[3] == 0x00 && OperatorId[4] == 0x00 &&  OperatorId[5] == 0x0F  )
#define IS_46001_OPERATOR(OperatorId) (OperatorId[0] == 0x04 && OperatorId[1] == 0x06 && OperatorId[2] == 0x00 && \
                                       OperatorId[3] == 0x00 && OperatorId[4] == 0x01 &&  OperatorId[5] == 0x0F  )
#define DISP_OPERATOR() GDI_TextOut(55, 16, pInfo->OperatorName, 0)
#define DISP_SMS_IND_CNT(s) GDI_TextOut(55, 4, s, 0)
#define IS_CALL_NUMBER(n) (n==SK_NUM0 || n==SK_NUM1 ||n==SK_NUM2 || n==SK_NUM3|| n==SK_NUM4|| n==SK_NUM5|| n==SK_NUM6|| n==SK_NUM7|| n==SK_NUM8|| n==SK_NUM9 )
#define SPACE_LINE "                                                                                    "

UINT32 CFW_DemoMainProc(CFW_EVENT* pEvent, VOID* pUserData);
UINT32 NewSms(CFW_EVENT* pEvent);
BOOL GetReportInfo(CFW_EVENT* pEvent, UINT8* pNumber, UINT8* pScts, UINT8* pDt);
BOOL MySendSmsMessage(UINT8* pNumber, UINT8 nNumberSize);
UINT8 GetSignalLevel(UINT32 nParam1);
INT8 UTIL_GetKeyNumberChar(UINT8 sk);
BOOL CALL_MoKeyHandler(CFW_EVENT* pEvent, MMI_PROC_INFO* pAppInfo);

BOOL IND_SignalHandler(CFW_EVENT* pEvent, MMI_PROC_INFO* pAppInfo);

BOOL VM_ShowSigBatOper(MMI_PROC_INFO* pAppInfo);
BOOL MMI_CfwIndEventProc(CFW_EVENT* pEvent, MMI_PROC_INFO* pInfo);
BOOL VM_ShowIdleScreen(MMI_PROC_INFO* pAppInfo);
BOOL VM_ShowCallMoScreen(MMI_PROC_INFO* pAppInfo, UINT8 nOption);
BOOL VM_ShowSmsNoteScreen(MMI_PROC_INFO* pAppInfo, UINT8 nOption);
BOOL VM_ShowCallMtScreen(MMI_PROC_INFO* pAppInfo, UINT8 nOption);
BOOL VM_ShowSmsScreen(MMI_PROC_INFO* pAppInfo);
BOOL VM_ShowPbkScreen(MMI_PROC_INFO* pAppInfo);
BOOL VM_DrawCallMtNumber(MMI_PROC_INFO* pAppInfo);
BOOL VM_BackToIdle(MMI_PROC_INFO* pAppInfo, UINT8 nOption);

#define CLS_LINES "                                                                                 "
#define CLR_NUM( )             GDI_TextOut(5, 80, SPACE_LINE, 21)
#define DRAW_NUM(number, size) CLR_NUM( ); GDI_TextOut(5, 80, number, size)

#endif // _H_ 

