////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2010, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
/// @file tgt_atvd_cfg.h
/// That file provides the TGT API related to ATVD configuration.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _TGT_ATVD_CFG_H_
#define _TGT_ATVD_CFG_H_

#include "cs_types.h"


typedef struct ATVD_CONFIG_STRUCT_T TGT_ATVD_CONFIG_T;

// =============================================================================
// tgt_GetATVDConfig
// -----------------------------------------------------------------------------
/// This function is used by ATVD to get access to its configuration structure.
/// This is the only way ATVD can get this information.
// =============================================================================
PUBLIC CONST TGT_ATVD_CONFIG_T* tgt_GetAtvdConfig(VOID);


#endif // _TGT_ATVD_CFG_H_
