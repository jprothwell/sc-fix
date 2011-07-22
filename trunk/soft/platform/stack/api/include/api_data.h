/*
================================================================================

  This source code is property of StackCom. The information contained in this
  file is confidential. Distribution, reproduction, as well as exploitation,or 
  transmisison of any content of this file is not allowed except if expressly
  permitted. Infringements result in damage claims.

  Copyright StackCom (c) 2003

================================================================================
*/

/*
================================================================================
  File       : api_data.h
--------------------------------------------------------------------------------

  Scope      : API Context

  History    :
--------------------------------------------------------------------------------
  May 12 06  |  FCH   | Added __GPRS__ flag for GSM only compilation
  Mar 31 06  |  MCE   | Addec UlCongStatus
  Mar 10 04  |  PCA   | Creation
================================================================================
*/

#ifndef __API_DATA_H__
#define __API_DATA_H__

// System includes
#include "sxs_type.h"
#include "fsm.h"

// Interface includes
#include "api_msg.h"


#ifdef __API_C__
#define DefExtern
#else
#define DefExtern extern
#endif


/*
================================================================================
  Data       : api_Ctxt_t
--------------------------------------------------------------------------------

  Scope      : Global API context

================================================================================
*/
#define API_MSG_MAX_SIZE 20
#define RAD_OFFS_API     10
typedef struct
{
 Msg_t       * RecMsg       ;   // Received message
 Msg_t       * OutMsg       ;   // Message to send

 Fsm_t       * Fsm          ;   // Current selected Fsm
 InEvtDsc_t  * IEvtDsc      ;   // Incoming Event
 OutEvtDsc_t * OEvtDsc      ;   // Outgoing Event
#if !(STK_MAX_SIM==1)
 u32           SimId        ;
#endif
 bool          KeepMsg      ;   // TRUE if RecMsg must not be deleted
 u8            Flags        ;   // API flags
 u8            UlCongStatus ;   // Bit map of Uplink Congestion Status

} api_Ctxt_t ;

/*
** Defines for the API flags
*/
#define API_FLG_MMC_STOPPED STK_CNFFSM_MMC
#define API_FLG_MMP_STOPPED STK_CNFFSM_MMP
#ifdef __GPRS__
#define API_FLG_STOP_MSK    ( API_FLG_MMC_STOPPED | API_FLG_MMP_STOPPED )
#else
#define API_FLG_STOP_MSK    ( API_FLG_MMC_STOPPED )
#endif

/*
** Bit definition for UlCongStatus
*/
#define API_NO_ULCONG     0
#define API_RLU_ULCONG   (1<<0)
#define API_RLU_NOULCONG (1<<1)
#define API_MEM_ALERT    (1<<2)

#if !(STK_MAX_SIM==1)
DefExtern api_Ctxt_t apiData[STK_MAX_SIM];
DefExtern api_Ctxt_t *pAPIData;
#else
DefExtern api_Ctxt_t apiData;
DefExtern api_Ctxt_t *pAPIData;
#endif

#undef DefExtern
#endif // __API_DATA_H__
