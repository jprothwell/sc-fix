////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            //
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
/// @file tgt_btd_cfg.h.h
/// That file provides the TGT API related to BTD configuration.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _TGT_SPILCD_CFG_H_
#define _TGT_SPILCD_CFG_H_

#include "cs_types.h"

#ifdef __PRJ_WITH_SPILCD__
// =============================================================================
// tgt_GetSpiLcdConfig
// -----------------------------------------------------------------------------
/// This function is used by SPILCD to get access to its configuration structure.
/// This is the only way SPILCD can get this information.
// =============================================================================
PUBLIC CONST SPILCD_CONFIG_STRUCT_T* tgt_GetSpiLcdConfig(VOID);
#endif //__PRJ_WITH_SPILCD__
#endif //
