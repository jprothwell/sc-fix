//------------------------------------------------------------------------------
//  $Header: /usr/local/cvsroot/rlu/inc/rlc_defs.h,v 1.2 2006/03/08 00:18:10 mce Exp $
//------------------------------------------------------------------------------
/*
================================================================================

  This source code is property of StackCom. The information contained in this
  file is confidential. Distribution, reproduction, as well as exploitation,or 
  transmission of any content of this file is not allowed except if expressly
  permitted. Infringements result in damage claims.

  Copyright StackCom (C) 2003-2006

================================================================================
*/

/*
================================================================================
  File       : rlc_defs.h 
--------------------------------------------------------------------------------

  Scope      : Defintion of Constants and Macros for the RLC entities

  History    :
--------------------------------------------------------------------------------
  Feb 09 04  |  MCE   | Creation            
================================================================================
*/
#ifndef __RLC_DEFS_H__
#define __RLC_DEFS_H__

#include "rlu_msg.h"
#include "itf_il.h"

/* 
** Size of the RLC-MAC header
*/
#define RLC_MAC_HDR_SIZE ( RAD_HDR_MAC_SIZE + 1 )

/*
** SNS value and its modulo
*/
#define RLC_SNS            128
#define RLC_MOD_SNS(_val)  ((_val) & (RLC_SNS - 1))

/*
** Window Size
*/
#define RLC_WS             64

#endif	

