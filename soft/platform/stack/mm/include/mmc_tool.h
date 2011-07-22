//------------------------------------------------------------------------------
//  $Log: mmc_tool.h,v $
//  Revision 1.4  2006/05/23 10:51:30  fch
//  Update Copyright
//
//  Revision 1.3  2006/01/16 15:14:07  oth
//  Add the AbandonFOR (Follow on Proceed) procedure
//
//  Revision 1.2  2005/12/29 17:38:47  pca
//  Log keyword added to all files
//
//------------------------------------------------------------------------------
/*
================================================================================

  This source code is property of StackCom. The information contained in this
  file is confidential. Distribution, reproduction, as well as exploitation,or 
  transmisison of any content of this file is not allowed except if expressly
  permitted. Infringements result in damage claims.

  Copyright StackCom (C) 2003-2006

================================================================================
*/

/*
================================================================================
  File       : mmc_tool.h
--------------------------------------------------------------------------------

  Scope      : Header file for the generic Circuit Switch MM functions

  History    :
--------------------------------------------------------------------------------
  Aug 06 03  |  OTH   | Creation            
================================================================================
*/

#ifndef __MMC_TOOL_H__
#define __MMC_TOOL_H__

// System includes
#include "sxs_type.h"

// Interface includes
#include "itf_rr.h"


void mmc_LocUp               ( u8   );
void mmc_LocUpFailure        ( bool );
void mmc_AskForRrCx          ( u8   );
void mmc_CellIndGenProc      ( void );
void mmc_HandleT3212         ( void );
void mmc_ImsiDetach          ( bool );
void mmc_PrepareCmServReq    ( u8, bool );
void mmc_ResetLocUpParam     ( u8, bool );
void mmc_BuildTmsiReallocCplt( void );
void mmc_SdLRStatus          ( u8, bool, u8, u8* );
u8   mmc_FillUpMobId         ( u8*  );
void mmc_SdMmStatus          ( u8   );
void mmc_HandlePaging        ( bool );
void mmc_HandleCMAbort       ( void );
void mmc_ReleaseCM           ( u8 );
void mmc_CheckEncode         ( s16, u8, u8 );
#define MMC_RRCNX_NEEDED 0
#define MMC_RR_TRANSFER  1
#define MMC_NO_ACTION    2
void mmc_HandleOtherPD       ( void );
void mmc_StopLocUp           ( void );
void mmc_SdSelfLocUp         ( u8   );
void mmc_ActivateCnx         ( void );
u8   mmc_SdStopCnf           ( void );
void mmc_AbandonFOR          ( void );

// MMC macros
// This macro is waiting for a PDTI
#define WHICH_MBX_PDTI( _PDTI )                                 \
( _PDTI & 0x0F ) == MM_CC_PD  ? CC_MBX  :                       \
( _PDTI & 0x0F ) == MM_SMS_PD ? SMS_MBX : SS_MBX

#define WHICH_MBX_PD( _PD )                     \
 _PD == MM_CC_PD  ? CC_MBX  :                   \
 _PD == MM_SMS_PD ? SMS_MBX : SS_MBX

#define MMC_GET_TI( _PDTI ) (_PDTI & 0xF0) >> 4
#define MMC_GET_PD( _PDTI ) (_PDTI & 0x0F)


// Capabilities variables
extern bool e2p_HRSupported;
#define MMC_GET_HR_SUPPORTED e2p_HRSupported

#endif // __MMC_TOOL_H__
