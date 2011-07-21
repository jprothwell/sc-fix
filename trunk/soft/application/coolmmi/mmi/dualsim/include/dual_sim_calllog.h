/******************************************************************************/
/*                                                                            */
/*              Copyright (C) 2009, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* This source code is property of Coolsand. The information contained in     */
/* this file is confidential. Distribution, reproduction, as well as          */
/* exploitation,or transmisison of any content of this file is not            */
/* allowed except if expressly permitted.                                     */
/* Infringements result in damage claims!                                     */
/*                                                                            */
/* FILE NAME                                                                  */
/*       dual_sim_calllog.h                                                           */
/*                                                                            */
/* DESCRIPTION                                                                */
/*   MTPNP layer header file, includes definition of macros and               */
/*   data structures and API functions.                                       */
/*                                                                            */
/******************************************************************************/

#ifndef __DUAL_SIM__CALLLOG_H__
#define __DUAL_SIM__CALLLOG_H__


#ifdef __MMI_MULTI_SIM__
#include "dual_sim_call.h"


////////////////////////////////////////
// CHIS APIs
extern void HighlightDMCHISTMainMenu(void);
extern void HighlightSlaveCHISTDialedCalls(void);
extern void HighlightSlaveCHISTRecvdCalls(void);
extern void HighlightSlaveCHISTMissedCalls(void);
extern void HintSlaveCHISTDialedCalls(U16 index);
extern void HintSlaveCHISTRecvdCalls(U16 index);
extern void HintSlaveCHISTMissedCalls(U16 index);
extern void MTPNP_PFAL_ReadDualMixedCallLog(void);
extern void HighlightCHISTDualMixedCallLog(void);
extern BOOL GetSCHISReady(U8 nSimId );
extern void SetSCHISReady(U8 nSimId, BOOL bReady);
extern void SetCHISContext(U8 nSimId);
extern void MTPNP_CHISSetProtocolEventHandler(PsFuncPtr funcPtr, U32 eventID);
extern void SetCHISToCall();
extern void RestoreCHISContext();
extern void EntryCHISTSlaveSMSCount(void);
extern void HighlightSlaveCHISTSlaveSMSCount(void);
extern void MTPNP_PFAL_Call_Rejected(void);
extern void MTPNP_PFAL_Call_Missed( void );
extern void MTPNP_PFAL_Call_Recved( void );
extern void MTPNP_PFAL_Call_Dialed( void );
extern void mmi_ucm_app_entry_dial_option(U8 *num_uri, mmi_ucm_callback_func_ptr callback_after_dial, void* callback_para);
extern MTPNP_UINT8 MTPNP_PFAL_chist_get_num_rejected_call_before_view(void);
extern void MTPNP_PFAL_ReadDualModeMissedNum(void);

extern void CHISTGetCallLogBySENDKey_SIM1();
extern void CHISTGetCallLogBySENDKey_SIM2();
extern void CHISTGetCallLogBySENDKey_SIM3();
#endif

#endif //__DUAL_SIM__CALLLOG_H__

