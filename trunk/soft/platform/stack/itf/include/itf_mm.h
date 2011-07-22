//------------------------------------------------------------------------------
//  $Log: itf_mm.h,v $
//  Revision 1.4  2006/03/27 09:58:54  oth
//  Add a comment
//
//  Revision 1.3  2006/02/28 10:48:18  oth
//  Update the mm_PDTIInd_t comments
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
  File       : ITF_MM.H
--------------------------------------------------------------------------------

  Scope      : Interface Mesages provided by MM                       

  History    :
--------------------------------------------------------------------------------
  Oct 04 05  |  OTH   | Add the mm_ReleaseInd_t struct            
  Aug 06 03  |  OTH   | Creation            
================================================================================
*/

#ifndef __ITF_MM_H__
#define __ITF_MM_H__
 
#include "sxs_type.h"
#include "cmn_defs.h"
#include "cmn_dec.h"

#include "itf_api.h"


/*
** Primitive ID definitions
** Circuit switch
*/
#define MM_ESTAB_REQ        ( HVY_MM + 1  )
#define MM_ESTAB_CNF        ( HVY_MM + 2  )
#define MM_ESTAB_IND        ( HVY_MM + 3  )
#define MM_ESTAB_ERR        ( HVY_MM + 4  )
#define MM_ABORT_REQ        ( HVY_MM + 5  )
#define MM_RELEASE_REQ      ( HVY_MM + 6  )
#define MM_RELEASE_IND      ( HVY_MM + 7  )
#define MM_REESTAB_REQ      ( HVY_MM + 8  )
#define MM_REESTAB_CNF      ( HVY_MM + 9  )
#define MM_REESTAB_ERR      ( HVY_MM + 10 )
#define MM_SERVREQ_ACK      ( HVY_MM + 11 )
#define MM_DATA_REQ         ( HVY_MM + 12 )
#define MM_DATA_IND         ( HVY_MM + 13 )
#define MM_NWKINFO_IND      ( HVY_MM + 14 )

#define HVY_MM_GPRS_BASE ( HVY_MM + 0x20 )

#ifdef __GPRS__
/*
** Packet switch
*/
#define MMP_ATTACH_REQ      ( HVY_MM_GPRS_BASE + 1 )
#define MMP_DETACH_REQ      ( HVY_MM_GPRS_BASE + 2 )
#define MMP_DATA_REQ        ( HVY_MM_GPRS_BASE + 3 )
#define MMP_DATA_IND        ( HVY_MM_GPRS_BASE + 4 )
#define MMP_ABORT_IND       ( HVY_MM_GPRS_BASE + 5 )
#define MMP_PDPCTXSTATUS_IND  ( HVY_MM_GPRS_BASE + 6 )
#define MMP_SEQUENCE_IND      ( HVY_MM_GPRS_BASE + 7 )
#define MMP_SEQUENCE_RSP      ( HVY_MM_GPRS_BASE + 8 )
#endif


#define MM_CC_PD  CMN_CC_PD
#define MM_SS_PD  CMN_SS_PD
#define MM_SMS_PD CMN_SMS_PD
#define MM_SM_PD  CMN_SM_PD

/*
** Primitive Content definitions
*/
typedef struct 
{
 u8 PDTI;
 u8 CallType;
 // These values respect the 24.008, 10.5.3.3
#define MM_REGULAR_CALL   1
#define MM_EMERGENCY_CALL 2
#define MM_SMS            4
#define MM_SS_ACTIVATION  8
} mm_EstabReq_t;

typedef struct 
{
 u8 NbReest;
} mm_ReestabReq_t;

typedef struct 
{
 u8 PDTI;  // if PDTI=0xFF, Release everything
 u8 ReleaseCause;
#define MM_LOWLAYER_FAILURE 0
#define MM_OTHER_RELCAUSE   1
#define MM_COLLISION        2
} mm_ReleaseInd_t;

#if !(STK_MAX_SIM==1)
typedef struct 
{
 u8 PDTI;  // if PDTI=0xFF, Release everything
 u8 EstabErrCause; // establish error cause
#define MM_OTHER_ERRCAUSE       0
#define MM_ANOTHER_SIM_BUSY     1
} mm_EstabErrInd_t;
#endif

/*
================================================================================
  Structure  : mm_PDTIInd_t
--------------------------------------------------------------------------------
  Direction  : MMC->CM
  Message    : MM_ESTAB_IND, MM_RELEASE_REQ, MM_ESTAB_ERR, MM_ESTAB_CNF,
               MM_SERVREQ_ACK
  Scope      : 
================================================================================
*/
typedef struct 
{
 u8 PDTI;
} mm_PDTIInd_t;

/*
================================================================================
  Structure  : mm_StopReq_t
--------------------------------------------------------------------------------
  Direction  : API -> MM
  Scope      : Stops the protocol stack
================================================================================
*/
typedef api_StopReq_t mm_StopReq_t;

/*
================================================================================
  Structure  : mm_NwkInfoInd_t
--------------------------------------------------------------------------------
  Direction  : MM -> API
  Scope      : Gives informations about the network
================================================================================
*/
typedef api_NwkInfoInd_t mm_NwkInfoInd_t;

#ifdef __GPRS__
/*
================================================================================
  Structure  : mmp_DataReq_t
--------------------------------------------------------------------------------
  Direction  : SM -> MMP or SMS -> MMP
  Message    : MMP_DATA_REQ
  Scope      : InterLayer message
================================================================================
*/
typedef struct
{
  u8  Id;
} mmp_DataReq_t;


// Possible values for Id
// DO NOT CHANGE
// These values MUST MATCH the itf_llc.h Sapi value
#define MMP_SM_ID   1
#define MMP_SMS_ID  7


/*
================================================================================
  Structure  : mmp_DataInd_t
--------------------------------------------------------------------------------
  Direction  : MMP -> SMS or SM
  Message    : MMP_DATA_IND
  Scope      : InterLayer message
================================================================================
*/
typedef struct
{
  u8  Pad;
} mmp_DataInd_t;

/*
================================================================================
  Structure  : mmp_AbortInd_t
--------------------------------------------------------------------------------
  Direction  : MMP -> SM
  Message    : MMP_ABORT_IND
  Scope      :
================================================================================
*/
typedef struct
{
  u8  Cause;
} mmp_AbortInd_t;

/*
** Possible values for Cause
*/
  /*
      The cause value must be checked after discussion with the MMI for
      the interface between SM and MMI for the PDP deactivation due to MMP
  */
#define MMP_PDP_TO_REACTIVATE     36  // Regular PDP context deactivation from 24.008
#define MMP_PDP_NOT_TO_REACTIVATE 36  // Regular PDP context deactivation from 24.008


/*
================================================================================
  Structure  : mmp_PdpCtxStatusInd_t
--------------------------------------------------------------------------------
  Direction  : SM -> MMP
  Message    : MMP_PDPCTXSTATUS_IND
  Scope      :
================================================================================
*/
typedef struct
{
  u8    Nsapi;    // Concerned NSAPI (5 to 15)
  bool  Status; // TRUE = Active / FALSE = Inactive
} mmp_PdpCtxStatusInd_t;


/*
================================================================================
  Structure  : mmp_SequenceInd_t
--------------------------------------------------------------------------------
  Direction  : MMP -> SNDCP
  Message    : MMP_SEQUENCE_IND
  Scope      : MMP provides SNDCP with the list of the received N-PDU number in
               case of inter SGSN routing area updating
================================================================================
*/
typedef struct
{
  u8  Nsapi;
  u8  NpduNum;
} mmp_NpduElmt_t;

typedef struct
{
  u8  MbxRsp;     // Mbx to send the response
  u8  NbItems;    // Nb elements in the list
  mmp_NpduElmt_t NpduList[11];
} mmp_SequenceInd_t;


/*
================================================================================
  Structure  : mmp_SequenceRsp_t
--------------------------------------------------------------------------------
  Direction  : SNDCP -> MMP
  Message    : MMP_SEQUENCE_RSP
  Scope      : SNDCP provides MMP with the list of received N-PDU number
               (response to the MMP_SEQUENCE_IND)
================================================================================
*/
typedef struct
{
  u8  NbItems;    // Nb elements in the list
  mmp_NpduElmt_t NpduList[11];
} mmp_SequenceRsp_t;
#endif // __GPRS__




#endif
