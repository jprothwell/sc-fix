//==============================================================================
//  File       : SMR_PROC.H
//------------------------------------------------------------------------------
//  Scope      :
//  History    :
//------------------------------------------------------------------------------
//  2006/05/22 | aba | Copyrigth is updated
//  2005/12/29 | pca | Log keyword added to all files
//  Feb 22 04  | ABA | Creation
//==============================================================================


#ifndef __SMR_PROC_H__
#define __SMR_PROC_H__

#include "sxr_ops.h"
#include "cmn_defs.h"

#define TR1M_DURATION (45 SECOND)
#define TR2M_DURATION (20 SECOND)
#define TRAM_DURATION (35 SECOND)

#define M_GET_MSG_REF(_PtSmrHead) (*(_PtSmrHead+1))

#endif  // __SMR_PROC_H__
