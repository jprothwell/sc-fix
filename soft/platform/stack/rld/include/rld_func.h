//------------------------------------------------------------------------------
//  $Log: rld_func.h,v $
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

  Copyright StackCom (c) 2003

================================================================================
*/
/*
================================================================================
  File       : rld_func.h
--------------------------------------------------------------------------------

  Scope      : Protoypes of rld functions

  History    :
--------------------------------------------------------------------------------
  Mar 04 04  |  OTH   | Creation 
================================================================================
*/

#ifndef __RLD_FUNC_H__
#define __RLD_FUNC_H__

#include "sxs_type.h"    // System includes

#include "rld_msg.h"
#include "itf_msg.h"

/*
================================================================================
  Common prototypes for RLD
--------------------------------------------------------------------------------

  Scope      : Functions used by core RLD task

================================================================================
*/

//           rld_evt.c
InEvtDsc_t * rld_GiveDsc ( u32 ) ;

//           rld_out.c
void         rld_PrepMsg ( OutEvtDsc_t *, u16 ) ;
void         rld_SendMsg ( u8 ) ;

//           rld_trsf.c
void         rld_ConInd  ( void ) ;
void         rld_DataInd ( void ) ;
void         rld_RelInd  ( void ) ;
void         rld_TestModeSet(void);

//           rld_tool.c
void         rld_BuildAndFwdSdu( Msg_t * );
#endif
