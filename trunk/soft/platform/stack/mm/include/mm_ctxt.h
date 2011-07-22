//------------------------------------------------------------------------------
//  $Log: mm_ctxt.h,v $
//  Revision 1.4  2006/05/23 10:51:30  fch
//  Update Copyright
//
//  Revision 1.3  2006/01/16 16:56:29  oth
//  Update from FCH
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
  File       : mm_ctxt.h
--------------------------------------------------------------------------------

  Scope      : General context for the MM layer

  History    :
--------------------------------------------------------------------------------
  Jan 13 06  |  FCH   | Added MM_TBF_ACTIF
  Aug 06 03  |  OTH   | Creation            
================================================================================
*/

#ifndef __MM_CTXT_H__
#define __MM_CTXT_H__

// System includes
#include "sxs_type.h"
#include "fsm.h"

// Interface includes
#include "itf_rr.h"

// MM internal includes
#include "mmc_ctxt.h"
#ifdef __GPRS__
#include "mmp_ctxt.h"
#endif

// EnDec includes
#include "edre.h"


#ifdef __MM_C__
#define DefExtern
u8 TmsiInvalid[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
u8 LaiInvalid[2]  = { 0xFF, 0xFE };
#ifdef __GPRS__
u8 PTmsiSignInvalid[3] = { 0xFF, 0xFF, 0xFF };
#endif
#else
#define DefExtern extern
extern u8 TmsiInvalid[4];
extern u8 LaiInvalid[2];
#ifdef __GPRS__
extern u8 PTmsiSignInvalid[3];
#endif
#endif


/*
================================================================================
  Data       : mm_sim_t
--------------------------------------------------------------------------------

  Scope      : Sim context for MM

================================================================================
*/

typedef struct
{
 u8 Imsi[9];              /* Encoded as in the SIM (MobileId 10.5.1.4) */
 u8 Tmsi[4];              /* These four first parameters are in the */
 u8 Lai[5];               /* exact same order than in the LOCI file */
 u8 T3212Duration;        /* Do not change it (see 11.11 par10.3.17 */
 u8 LocUpStatus;
 u8 SimStatus;
#define MM_SIM_KO           0 /*no SIM*/
#define MM_SIM_OK_CS        ( 1 << 0 ) /*SIM CS service is OK*/
#define MM_SIM_OK_PS        ( 1 << 1 ) /*SIM PS service is OK*/
#define MM_SIM_OK           ( MM_SIM_OK_CS | MM_SIM_OK_PS )
#define MM_SIM_KO_STK_STOP  1 << 3 /*no SIM and stack stop; added by liujg*/
 u8 CKSN;
#ifdef __GPRS__
 u8 PTmsi[4];             /* These four parameters are in the exact */ 
 u8 PTmsiSign[3];         /* same order than in the LOCI GPRS file  */
 u8 RAI[6];               /* Do not change it.                      */
 u8 RAUpStatus;           /* (see 11.11 par 10.3.33)                */
 u8 GPRSCKSN;
#endif
} mm_sim_t;


/*
================================================================================
  Data       : mm_Ctxt_t
--------------------------------------------------------------------------------

  Scope      : Global MM context

================================================================================
*/
typedef struct
{
 u8               Flags;
#define MM_KEEP_MSG ( 1 << 0 )
 TmmDl_Data     * IRadioMsg;
 Msg_t          * IMsg;
 OutEvtDsc_t    * OEvtDsc;
 InEvtDsc_t     * IEvtDsc;
 Msg_t          * OMsg;
 mmc_Ctxt_t       mmcCtx;
 rr_CellInd_t     CellInd;
 rr_CellInd_t     OldCellInd;
#if !(STK_MAX_SIM==1)
 u32              SimId;
 #endif
 u8               LaiQ;
#ifdef __GPRS__ 
 u8               SavedMsgQ;
 u8               mmActif;
#define MM_MMC_ACTIF ( 1 << 0 )
#define MM_MMP_ACTIF ( 1 << 1 )
#define MM_TBF_ACTIF ( 1 << 2 )
 mmp_Ctxt_t       mmpCtx;
#endif          
 mm_sim_t         mmsimIf;
} mm_Ctxt_t ;

#if !(STK_MAX_SIM==1)
DefExtern mm_Ctxt_t mmCtx[STK_MAX_SIM];
DefExtern mm_Ctxt_t *pMMCtx;

typedef struct
{
 u8               CurSimId;
 u8               SavedMsgQ; 
} mm_Dul_Ctxt_t ;

DefExtern mm_Dul_Ctxt_t  mmDulCtx;

#else
DefExtern mm_Ctxt_t mmCtx;
DefExtern mm_Ctxt_t *pMMCtx;
#endif


#undef DefExtern
#endif // __MM_CTXT_H__
