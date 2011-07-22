//------------------------------------------------------------------------------
//  $Log: mmc_ctxt.h,v $
//  Revision 1.5  2006/05/23 10:51:30  fch
//  Update Copyright
//
//  Revision 1.4  2006/02/27 13:12:09  oth
//  Handle Assignment Reject and Access barred in case of Loc Up
//
//  Revision 1.3  2006/02/06 03:01:37  oth
//  Addition of a flag MMC_WAIT_RELEASE in case of abort from MM
//
//  Revision 1.2  2005/12/29 17:38:47  pca
//  Log keyword added to all files
//
//------------------------------------------------------------------------------
/*
================================================================================

  This source code is property of Coolsand. The information contained in this
  file is confidential. Distribution, reproduction, as well as exploitation,or 
  transmisison of any content of this file is not allowed except if expressly
  permitted. Infringements result in damage claims.

  Copyright Coolsand (C) 2003-2006

================================================================================
*/

/*
================================================================================
  File       : mmc_ctxt.h
--------------------------------------------------------------------------------

  Scope      : Context for the Circuit Switch MM layer

  History    :
--------------------------------------------------------------------------------
  Aug 06 03  |  OTH   | Creation            
================================================================================
*/

#ifndef __MMC_CTXT_H__
#define __MMC_CTXT_H__

// System includes
#include "sxs_type.h"
#include "cmn_defs.h"

// Interface includes
#include "mm_msg.h"
#include "itf_msg.h"
#include "itf_rr.h"

// MM specific includes
#include "mm_def.h"


#define MMC_CM_CALL_BIT ( 1 << 7 )
/*
================================================================================
  Data       : mmc_Ctxt_t
--------------------------------------------------------------------------------

  Scope      : Global Circuit Switch MM context

================================================================================
*/
typedef struct
{
 u8  CxNb;
 u8  CurPDTI;
 u8  CurProc;
#define MMC_NO_PROC           0
#define MMC_LOC_UP            1
#define MMC_IMSI_DETACH       2
#define MMC_REGULAR_CALL      ( 1 | MMC_CM_CALL_BIT )
#define MMC_EMERGENCY_CALL    ( 2 | MMC_CM_CALL_BIT )
#define MMC_REESTAB           ( 3 | MMC_CM_CALL_BIT )
#define MMC_PAGING_RESP       ( 4 | MMC_CM_CALL_BIT )
 u8  CurLocUp;
#define MMC_REGULAR_LOCUP     0 /* The 2 locup values are linked with the */
#define MMC_PERIODIC_LOCUP    1 /* values in the mmc_SavedEvt_t struct    */
#define MMC_IMSI_ATTACH       2
#define MMC_NO_LOCUP          3
 u8  LUAttemptCter;
 u8  LURandAccFailure;
 u16 Flags;
#define MMC_NO_T3212          ( 1 << 0  )
#define MMC_FOLLOW_ON_REQUEST ( 1 << 1  )
#define MMC_LOCUP_NEEDED      ( 1 << 2  )
#define MMC_ATTACHED          ( 1 << 3  )
#define MMC_RETRIEVING_CALL   ( 1 << 4  )
#define MMC_NORMAL_T3212      ( 1 << 5  )
#define MMC_START_T3211       ( 1 << 6  )
#define MMC_T3212_REG_TIMEOUT ( 1 << 7  )
#define MMC_STK_STOPPED       ( 1 << 8  )
#define MMC_RELEASE_RCVD      ( 1 << 9  )
#define MMC_LAUNCH_COMBATT    ( 1 << 10 )
#define MMC_FIRST_MM_MSG      ( 1 << 11 )
#define MMC_COMB_DETACH       ( 1 << 12 )
#define MMC_WAIT_RELEASE      ( 1 << 13 )
#define MMC_STOP_LOCUP        ( 1 << 14 )

 Msg_t * InitialMsg;
} mmc_Ctxt_t ;


#endif // __MMC_CTXT_H__
