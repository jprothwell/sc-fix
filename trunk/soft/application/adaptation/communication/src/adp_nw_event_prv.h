/******************************************************************************/
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    adp_NW_Event.h                                                */
/*                                                                            */
/* Description:                                                               */
/*     Add this header file for solving.                                      */
/*     @ZXB 2009-02-06                                                        */
/******************************************************************************/

#ifndef _ADP_NW_EVENT_PRV_H
#define _ADP_NW_EVENT_PRV_H

#include "mmi_features.h"
#include <csw.h>
#include "mmi.h"
#include "app_ltlcom.h"

// Force to include adp_events.h if not.
#ifndef _ADP_EVENTS_H
#include "adp_events.h"
#endif

static UINT8 SelModeorRegiFlag = 0;

extern kal_bool INT_USBBoot(void);

static void AdpNW_SendMessageToMMI(local_para_struct *msg_ptr,  msg_type_t msg_id);
static void CswBand2CSDBand(UINT8 *CSDBand,  UINT8 cswBand);
static void CSDBand2CswBand(UINT8 *cswBand,  UINT8 CSDBand);
static UINT8 AdaptAbortGetPLMNListToIdle(COS_EVENT* pCosEvent);
static UINT8 AdaptEntryNETSETSelectionModeReq(COS_EVENT* pCosEvent);
static UINT8 AdaptManuAutoNetworkRregisterReq(COS_EVENT* pCosEvent);
static UINT8 AdaptNETGetImeiReq(COS_EVENT *pCosEvent);
static UINT8 AdaptNETSETGetBandSelectionReq(COS_EVENT *pCosEvent);
static UINT8 AdaptNETSETGetPLMNListReq(COS_EVENT *pCosEvent);
static UINT8 AdaptNETSETSetBandSelectionReq(COS_EVENT *pCosEvent);
#ifdef __ADP_MULTI_SIM__
static UINT8 AdaptNwProcCfunReq(COS_EVENT *pCosEvent);
#endif
static UINT8 AdaptRequestRacDetach(COS_EVENT *pCosEvent);
static void AdpNW_SendMessageToMMI(local_para_struct *msg_ptr,	msg_type_t msg_id);
static BOOL ADP_DispatchNwIndMsg(COS_EVENT* pCosEvent);
static BOOL ADP_DispatchNwReqMsg(COS_EVENT *pCosEvent);
static BOOL ADP_DispatchNwRspMsg(COS_EVENT* pCosEvent);
static void CSDBand2CswBand(UINT8 *cswBand,  UINT8 CSDBand);
static void CswBand2CSDBand(UINT8 *CSDBand,  UINT8 cswBand);
static UINT8 DoEntryNETSETSelectionModeRsp(COS_EVENT * pCosEvent);
static UINT8 DoNETSETAbortPLMNListRsp(COS_EVENT * pCosEvent);
static UINT8 DoNETSETGetPLMNListRsp(COS_EVENT* pCosEvent);
static UINT8 DoNETSETManuAutoSelectNetworkRsp(COS_EVENT* pCosEvent);
static UINT8 DoNETSETSetBandSelectionRsp(COS_EVENT *pCosEvent);
static UINT8 DoNITZHandler(COS_EVENT *pCosEvent);
#ifdef __ADP_MULTI_SIM__
static UINT8 DoNwCfunRsp(COS_EVENT *pCosEvent);
#endif
static UINT8 DoNwPlmnSetRSP(COS_EVENT *pCosEvent);
#ifdef __ADP_MULTI_SIM__
static void ProcCfunFailed(void);
#endif
#ifdef __ADP_MULTI_SIM__
/*
Low 4 bits for SIM-1:
----------------------------------------
bits set (binary system)	meaning
----------------------------------------
0001						card enabled
0100						card disabled
others						reserved.

High 4 bits for SIM-2:
----------------------------------------
bits set (binary system)	meaning
----------------------------------------
0001						card enabled
0100						card disabled
others						reserved.

All 8 bits for SIM-1&2:
----------------------------------------
bits set	decimal	meaning
----------------------------------------
0001 0001	17		dual sim card
0100 0001	65		sim card1 only
0001 0100	20		sim card2 only
0100 0100	68		flight mode
**/
// Dual mode state.
static UINT8 g_DmState = 0;
// Previous dual mode state.
static UINT8 g_DmPrevState = 0;

// Get high and low 4 bits.
#define GET_H4(b) ((b) >> 4)
#define GET_L4(b) ((b) & 0x0F)

static void ProcCfunFailed(void);
static void ProcCfunSuccess(void);

#endif

#endif

