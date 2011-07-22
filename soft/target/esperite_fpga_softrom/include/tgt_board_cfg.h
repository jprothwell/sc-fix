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
/// @file tgt_board_config.h
/// That file describes the configuration of the board drivers for the specific
/// Gallite FPGA target with rom regenerated each time
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



// This target is not a full plain target it's just a change to the 
// esperite_fpga target


// make sure the define used here is not the one of the included file
#ifndef _TGT_BOARD_CFG_H_SOFTROM_
#define _TGT_BOARD_CFG_H_SOFTROM_

// First overload some paramters if needed

// here none, only the target.def is changed for this target

// then include the original header
#include "../../esperite_fpga/include/tgt_board_cfg.h"

#endif //_TGT_BOARD_CFG_H_SOFTROM_


