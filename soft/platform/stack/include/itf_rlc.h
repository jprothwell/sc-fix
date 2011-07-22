//------------------------------------------------------------------------------
//  $Log: itf_rlc.h,v $
//  Revision 1.5  2006/05/09 17:46:48  fch
//  Update with Bob REF v2.22
//
//  Revision 1.4  2006/03/02 17:30:59  fch
//  Comment of OTH
//
//  Revision 1.3  2006/02/28 10:49:07  oth
//  Increase the RLD_SDU_MAX_SIZE for a CRTU test
//
//  Revision 1.2  2005/12/29 17:38:44  pca
//  Log keyword added to all files
//
//------------------------------------------------------------------------------
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
  File       : ITF_RLC.H
--------------------------------------------------------------------------------

  Scope      : Interface Messages provided by the RLU and RLD entity
  History    :
--------------------------------------------------------------------------------
  Jui 16 06  |  FCH   | Added RLU_LLC_CTRL_FLG, RLU_LLC_CTRL, RLU_SUSPEND_REQ,
             |        | RLU_RESUME_REQ, RLU_PURGE_REQ
  Mar 30 06  |  MCE   | Added rlu_CongestInd_t and RLU_ULCONGEST_IND
  Feb 14 06  |  OTH   | Increase RLD max size to 2500
  Oct 29 04  |  FCH   | Added Ref field, RLU_INFORM_ON_TX and RLU_INFORM_ON_ACK
             |        | values for PAction field in rlu_DataReq_t
             |        | Added RLU_SYNCH_IND and rlu_SynchInd_t
  Oct 05 04  |  FCH   | Added TReady interface with MMP and added field 
             |        | PAction in rlu_DataReq_t
  Sep 23 04  |  FCH   | Added RLU_TESTMODE_REQ and rlu_TestModeReq_t
  Sep 09 04  |  MCE   | Suppression of rlu_Semaphore
  Feb 06 04  |  MCE   | Creation            
================================================================================
*/

#ifndef __ITF_RLC_H__
#define __ITF_RLC_H__
 
#include "sxs_type.h"
#include "cmn_defs.h"
#include "itf_msgc.h"
#include "itf_api.h"

/*
** RLU Internal msg Start at RLU_INTRA_BASE + 0 
*/
#define RLU_INTRA_BASE ( HVY_RLU + 0x8000 )

/*
** Primitive ID definitions
*/
// RLU
#define RLU_DATA_REQ            ( HVY_RLU + 1 )
#define RLU_QUEUECREATE_IND     ( HVY_RLU + 2 )
#define RLU_QUEUEDATA_REQ       ( HVY_RLU + 3 + SXS_TIMER_EVT_ID )
#define RLU_TESTMODE_REQ        ( HVY_RLU + 4 )
#define RLU_TREADY_REQ          ( HVY_RLU + 5 )
#define RLU_TREADY_IND          ( HVY_RLU + 6 )
#define RLU_SYNCH_IND           ( HVY_RLU + 7 )
#define RLU_ULCONGEST_IND       ( HVY_RLU + 8 )
#define RLU_SUSPEND_REQ         ( HVY_RLU + 9 )
#define RLU_RESUME_REQ          ( HVY_RLU + 10 )
#define RLU_PURGE_REQ           ( HVY_RLU + 11 )

// RLD
#define RLD_DATA_IND            ( HVY_RLD + 1 ) // Interlayer message, no specific
                                                // fields -> no rld_DataInd_t
#define RLD_TEST_DATA_REQ  ( HVY_RLD + 2)
#define RLD_TEST_MODE_REQ  (HVY_RLD + 3)

// RLD defines
#define RLD_MAX_SDU 2500

/*
================================================================================
  Structure  : rlu_DataReq_t
--------------------------------
  Direction  : LLC -> RLU
  Scope      : asks for the transmission of an LLC PDU
  Comment    : InterLayer message
================================================================================
*/
typedef struct 
{
 u8   Cause     ; // Purpose of the LLC PDU
 u8   Mode      ; // RLC mode
 u8   Prio      ; // Radio priority; as in Channel Request Description IE in 
                  // 04.060; value in [0..3]
 u8   Class     ; // Peak throughput class; as in Channel Request Description IE
                  // in 04.060; value in [1..9]
 u8   Pfi       ; // Packet Flow Identifier
 u8   PType     ; // LLC PDU type, in 04.060 section 12.7
 u8   PAction   ; // Supplementary action requested for the PDU (Start TReady
                  // when sent on air interface, inform when sent on air
                  // interface, inform when ack from peer)
 u8   Ref       ; // Reference to use with MMP when information about sending is
                  // requested
} rlu_DataReq_t ;

// Possible values for Cause
// DO NOT CHANGE; 
// These values MUST MATCH the itf_rr.h cause values 
// for the rrp_ConnectReq_t struct 
#define RLU_USER_DATA    0
#define RLU_PAGE_RSP     1
#define RLU_CELL_UPDATE  2
#define RLU_MM_PROCEDURE 3

  // special cause for some LLC frame (XID by example)
#define RLU_LLC_CTRL_FLG 0x80
#define RLU_LLC_CTRL     (RLU_USER_DATA | RLU_LLC_CTRL_FLG)

// Possible values for Mode
// DO NOT CHANGE; 
// These values MUST MATCH the itf_rr.h RlcMode values 
// for the rrp_ConnectReq_t struct 
#define RLU_ACK_MODE     0
#define RLU_UACK_MODE    1
#define RLU_NO_MODE      0xff 

// Possible values for Pfi
// other valid values range from 0 to 127
#define RLU_NO_PFI       0x80

// Possible values for PType
#define RLU_SACK         0    // LLC frame is ACK or SACK
#define RLU_NOT_SACK     1    // LLC frame is other than ACK or SACK

// Possible values for PAction
#define RLU_NO_ACTION     0
#define RLU_START_TREADY  ( 1 << 0 )
#define RLU_INFORM_ON_TX  ( 1 << 1 )
#define RLU_INFORM_ON_ACK ( 1 << 2 )


/*
================================================================================
  Structure  : rlu_QueueCreateInd_t
--------------------------------------------------------------------------------
  Direction  : RLU -> LLC
  Message    : RLU_QUEUECREATE_IND
  Scope      : RLU gives the Queue Id of the shared queue between RLU and LLC 
               The queue is used to pass RluDataReq from LLC to RLC
  Comment    : 
================================================================================
*/
typedef struct 
{
  u8    QueueId;
} rlu_QueueCreateInd_t;

/*
================================================================================
  Structure  : rlu_TestModeReq_t
--------------------------------------------------------------------------------
  Direction  : MMP -> RLU
  Message    : RLU_TESTMODE_REQ
  Scope      : MMP provides RLU with the applicable test mode parameters
  Comment    : 
================================================================================
*/
typedef struct 
{
  u16   PduNumber;    // Number of LLC PDUs (0 to 4095)
  u8    Mode;         // Test mode
  u8    DlTsOff;      // Downlink Timeslot Offset (0 to 7)
} rlu_TestModeReq_t;


// Possible values for Mode
#define RLU_TST_MODE_A  0
#define RLU_TST_MODE_B  1

typedef struct
{
    u8 TestMode;
}rld_TestModeReq_t;


/*
================================================================================
  Structure  : rlu_TReadyReq_t
--------------------------------------------------------------------------------
  Direction  : MMP -> RLU
  Message    : RLU_TREADY_REQ
  Scope      : MMP configures RLU with value and status of the READY Timer
               or MMP orders RLU to stop the READY timer
  Comment    : 
================================================================================
*/
typedef struct 
{
  u8    Cmd;      // Command about the timer READY (bitmap)
  bool  TActive;  // Activated (TRUE) /deactivated (FALSE)
  u32   TValue;   // Value to use when starting the timer (in GSM frames)
}rlu_TReadyReq_t;


// Possible values for Cmd
#define RLU_TREADY_CFG    ( 1 << 0 )
#define RLU_TREADY_STOP   ( 1 << 1 )


/*
================================================================================
  Structure  : rlu_TReadyInd_t
--------------------------------------------------------------------------------
  Direction  : RLU -> MMP
  Message    : RLU_TREADY_IND
  Scope      : RLU provides MMP with information about READY timer
  Comment    : 
================================================================================
*/
typedef struct 
{
  u8    Status; // Status about the timer READY
}rlu_TReadyInd_t;


// Possible values for Status
#define RLU_TREADY_STARTED  0
#define RLU_TREADY_EXPIRED  1


/*
================================================================================
  Structure  : rlu_SynchInd_t
--------------------------------------------------------------------------------
  Direction  : RLU -> MMP
  Message    : RLU_SYNCH_IND
  Scope      : RLU informs MMP that the action requested for a PDU (MMP message)
               has happened
  Comment    : 
================================================================================
*/
typedef struct 
{
  u8    Ref;  // Reference of the PDU
}rlu_SynchInd_t;


/*
================================================================================
  Structure  : rlu_CongestInd_t
--------------------------------------------------------------------------------
  Direction  : RLU -> API
  Message    : RLU_ULCONGEST_IND
  Scope      : RLU informs API that the uplink contestion status has changed 
  Comment    : 
================================================================================
*/
typedef api_UlCongestInd_t rlu_UlCongestInd_t ;

// Possible values for Status
#define RLU_ULCONG_OFF  API_PS_ULCONG_OFF // No congestion on the uplink
#define RLU_ULCONG_ON   API_PS_ULCONG_ON  // Uplink radio path congested




/*
================================================================================
  Structure  : rld_DataInd_t
--------------------------------
  Direction  : RLD -> LLC
  Scope      : RLD is forwarding a LLC PDU to LLC
  Comment    : InterLayer message
================================================================================
*/
typedef struct 
{
 u32 TLLI;                      /* TLLI used when receiving this PDU */
} rld_DataInd_t ;

#endif  // __ITF_RLU_H__

