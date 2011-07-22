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
/// @file tgt_rfd_cfg.h                                                       //
/// That file provides the TGT API related to RFD configuration.              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _TGT_RFD_CFG_H
#define _TGT_RFD_CFG_H

#include "cs_types.h"
#include "rfd_config.h"


// =============================================================================
// tgt_GetXcvConfig
// -----------------------------------------------------------------------------
/// This function is used by XCV to get access to its configuration structure.
/// This is the only way XCV can get this information.
// =============================================================================
PUBLIC CONST XCV_CONFIG_T* tgt_GetXcvConfig(VOID);


// =============================================================================
// tgt_GetPaConfig
// -----------------------------------------------------------------------------
/// This function is used by PA to get access to its configuration structure.
/// This is the only way PA can get this information.
// =============================================================================
PUBLIC CONST PA_CONFIG_T*  tgt_GetPaConfig(VOID);


// =============================================================================
// tgt_GetSwConfig
// -----------------------------------------------------------------------------
/// This function is used by SW to get access to its configuration structure.
/// This is the only way SW can get this information.
// =============================================================================
PUBLIC CONST SW_CONFIG_T*  tgt_GetSwConfig(VOID);

#endif // _TGT_RFD_CFG_H

