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
/// @file tgt_tsd_cfg.h                                                       //
/// That file provides the TGT API related to MCD configuration.              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _TGT_TSD_CFG_H_
#define _TGT_TSD_CFG_H_

#include "cs_types.h"
#include "hal_spi.h"


// =============================================================================
// tgt_GetTsdConfig
// -----------------------------------------------------------------------------
/// This function is used by MCD to get access to its configuration structure.
/// This is the only way TSD can get this information.
// =============================================================================
PUBLIC CONST TSD_CONFIG_T* tgt_GetTsdConfig(VOID);


#endif // _TGT_TSD_CFG_H_

