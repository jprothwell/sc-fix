//------------------------------------------------------------------------------
//  $Log: rrp_enc.h,v $
//  Revision 1.4  2006/05/22 14:39:08  oth
//  Update the copyright
//
//  Revision 1.3  2006/02/08 08:50:47  oth
//  Addition for the handling of the PCCO
//
//  Revision 1.2  2005/12/29 17:39:12  pca
//  Log keyword added to all files
//
//------------------------------------------------------------------------------
/*
================================================================================

  This source code is property of StackCom. The information contained in this
  file is confidential. Distribution, reproduction, as well as exploitation,or 
  transmisison of any content of this file is not allowed except if expressly
  permitted. Infringements result in damage claims.

  Copyright StackCom (C) 2003 - 2006

================================================================================
*/

/*
================================================================================
  File       : rrp_enc.h
--------------------------------------------------------------------------------

  Scope      : Encoding structures

  History    :
--------------------------------------------------------------------------------
  Jan 11 05  |  OTH   | Creation            
================================================================================
*/

#ifndef __RRP_ENC_H__
#define __RRP_ENC_H__

// System includes
#include "sxs_type.h"




#define RRP_PCCFAIL_MT 0
#define RRP_PRESREQ_MT 5




typedef struct
{
 bool IsUplink;
 u8   Value;
} rrp_GTFI_t;




typedef struct
{
 bool IsPresent;
 u8   Value;
} rrp_TN_t;




typedef struct
{
 u8  Value[52];
 u16 BitLen;
} rrp_MsRACap_t;




typedef struct
{
 bool          AccessType_Present;
 bool          TLLI_Present;
 bool          MsRACap2_Present;
 bool          ChangeMark_Present;
 bool          SignVar_Present;
 bool          PFI_Present;
 u8            AccessType;
 u8            ChangeMark;
 u8            CValue;
 u8            SignVar;
 u8            PFI;
 u8            ChannelReqDesc [3];
 u32           TLLI;
 rrp_GTFI_t    GTFI;
 rrp_TN_t      ILevelTN[8];
 rrp_MsRACap_t MsRACap2;

} rrp_PResReq_t;

s16 rrp_EncodePResReq( u8 * Buffer, const u32 BitOffset,
                       const rrp_PResReq_t* Source );




#define RRP_FREQ_NOT_IMPLEMENTED   0
#define RRP_NO_RESP_ON_TARGET_CELL 1
#define RRP_ACC_REJ_ON_TARGET_CELL 2
#define RRP_STDBY_STATE            5
#define RRP_FORCE_2_STDBY          6




typedef struct
{
 u8  Cause;
 u8  Bsic;
 u16 Arfcn;
 u32 TLLI;

} rrp_PCCFail_t;

s16 rrp_EncodePCCFail( u8 * Buffer, const u32 BitOffset,
                       const rrp_PCCFail_t* Source );

#endif // __RRP_ENC_H__
