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
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file halp_lcdc.h
/// This file contains the lcdc private header.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#if CHIP_HAS_LCDC

#ifndef _HALP_LCDC_H_
#define _HALP_LCDC_H_

#include "hal_sys.h"

// =============================================================================
// hal_LcdcIrqHandler
// -----------------------------------------------------------------------------
// =============================================================================
PROTECTED VOID hal_LcdcIrqHandler(UINT8 interruptId);


#endif // _HALP_LCDC_H_

#endif // CHIP_HAS_LCDC

