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
//                                                                            //
/// @file tgt_camd_cfg.h                                                      //
/// That file provides the TGT API related to CAMD configuration.             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _TGT_CAMD_CFG_H_
#define _TGT_CAMD_CFG_H_

#include "cs_types.h"
#include "camd_config.h"


// =============================================================================
// tgt_GetCamdConfig
// -----------------------------------------------------------------------------
/// This function is used by CAMD to get access to its configuration structure.
/// This is the only way CAMD can get this information.
// =============================================================================
PUBLIC CONST CAMD_CONFIG_T* tgt_GetCamdConfig(VOID);


#endif // _TGT_CAMD_CFG_H_

