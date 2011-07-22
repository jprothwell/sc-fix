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
/// @file tgt_cams_cfg.h                                                      //
/// That file provides the TGT API related to CAMS configuration.             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _TGT_CAMS_CFG_H_
#define _TGT_CAMS_CFG_H_

#include "cs_types.h"
#include "cams_config.h"


// =============================================================================
// tgt_GetCamsConfig
// -----------------------------------------------------------------------------
/// This function is used by CAMS to get access to its configuration structure.
/// This is the only way CAMS can get this information.
// =============================================================================
PUBLIC CONST CAMS_CONFIG_T* tgt_GetCamsConfig(VOID);


#endif // _TGT_CAMS_CFG_H_

