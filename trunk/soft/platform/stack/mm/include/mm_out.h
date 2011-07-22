//------------------------------------------------------------------------------
//  $Log: mm_out.h,v $
//  Revision 1.3  2006/05/23 10:51:30  fch
//  Update Copyright
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
  File       : mm_out.h
--------------------------------------------------------------------------------

  Scope      : Header file for mm_out.c

  History    :
--------------------------------------------------------------------------------
  Aug 29 03  |  OTH   | Creation            
================================================================================
*/

#ifndef __MM_OUT_H__
#define __MM_OUT_H__

// System includes
#include "sxs_type.h"
#include "fsm.h"

void  mmc_PrepMsg (OutEvtDsc_t * Evt, bool IsIL);
void  mmp_PrepMsg (OutEvtDsc_t * Evt, u16  MsgLen);
void  mm_SendMsg (u8 Mbx);


#endif // __MM_OUT_H__
