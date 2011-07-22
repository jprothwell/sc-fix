////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2009, Coolsand Technologies, Inc.            //
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
/// @file tgt_app_cfg.h
/// That file describes the configuration of the application for this target.
//
////////////////////////////////////////////////////////////////////////////////



// This target is not a full plain target it's just a change to the 
// gallite_fpga target


// make sure the define used here is not the one of the included file
#ifndef _TGT_APP_CFG_H_SOFTROM_
#define _TGT_APP_CFG_H_SOFTROM_

// First overload some paramters if needed

// here none, only the target.def is changed for this target

// then include the original header
#include "../../gallite_fpga/include/tgt_app_cfg.h"


#endif //_TGT_APP_CFG_H_SOFTROM_

