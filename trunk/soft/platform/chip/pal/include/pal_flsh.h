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
  File         pal_flsh.h
--------------------------------------------------------------------------------

  Scope      : PAL flash header file.

  History    :
--------------------------------------------------------------------------------
 Jun 13 2003 |  ADA  | Creation
================================================================================
*/

#ifndef __PAL_FLSH_H__
#define __PAL_FLSH_H__

#include "sxs_type.h"

// TODO : merged from old "palcflsh.h" see if it is relevant...

/* to be updated when */
/*#define PAL_FLASHSTACKDATASIZE (sizeof( <stackcom structure >))*/
/* NOTE: up to 8k (adding reserved data 4byte) will work with no code change,
  if more is needed please ask*/
#define PAL_FLASHSTACKDATASIZE 1020

#define PAL_FLASHSTACKDATALOCATION 0x7f0000

void* pal_ReadFlash( UINT16 Offset );
void  pal_WriteFlash( void* Data, UINT16 Offset, UINT16 Len );

#endif

