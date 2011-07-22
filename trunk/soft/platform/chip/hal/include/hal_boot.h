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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/include/hal_boot.h $ //
//    $Author: admin $                                                        // 
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                     //   
//    $Revision: 269 $                                                         //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_host.h                                                          //
/// That file contains types and defines used to exchange with the            //
/// host for monitoring and remote execution.                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _HAL_BOOT_H_
#define _HAL_BOOT_H_

/// @defgroup boot HAL Boot and Monitoring Driver
///
/// Tha group describes the API allowing for monitoring stuff to be done through
/// and by the HAL layer.
///
/// @{
///


#include "cs_types.h"



// =============================================================================
// hal_BootEnterMonitor
// -----------------------------------------------------------------------------
/// Call this function to enter the boot monitor.
// =============================================================================
PUBLIC VOID hal_BootEnterMonitor(VOID);


// @} // <-- End of the boot group.

#endif // _HAL_BOOT_H_




