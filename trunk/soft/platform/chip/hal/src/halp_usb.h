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
//
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/halp_usb.h $
//  $Author: admin $
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $
//  $Revision: 269 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file halp_usb.h
///
/// This document describes the HAL USB private function
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _HALP_USB_H_
#define _HALP_USB_H_

#include "cs_types.h"

#if CHIP_HAS_USB == 1
#include "boot_usb.h"
#endif

// =============================================================================
// MACROS
// =============================================================================


// =============================================================================
// TYPES
// =============================================================================


// =============================================================================
// GLOBAL VARIABLES
// =============================================================================


// =============================================================================
// FUNCTIONS
// =============================================================================

#if CHIP_HAS_USB == 1
#if (HAL_USB_DRIVER_DONT_USE_ROMED_CODE == 1)
PROTECTED VOID hal_UsbIrqHandler(UINT8 interruptId);
#else
#define hal_UsbIrqHandler boot_UsbIrqHandler
#endif // (HAL_USB_DRIVER_DONT_USE_ROMED_CODE == 1)
#endif /* CHIP_HAS_USB */

#endif // _HALP_USB_H_

