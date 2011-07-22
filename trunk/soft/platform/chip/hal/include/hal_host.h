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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/include/hal_host.h $ //
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


#ifndef _HAL_HOST_H_
#define _HAL_HOST_H_

EXTERN_C_START

/// @defgroup host HAL Host Port Driver/Client
/// That group describes briefly the exchange media available
/// betweem HAL and a Host (Personal Computer, Mac, etc) 
/// connected through the Host Port.
///
/// @{
///


#include "cs_types.h"



// =============================================================================
// hal_HstSendEvent
// -----------------------------------------------------------------------------
/// Send an event through the host port
/// @param ch Event sent through the host port.
// =============================================================================
PUBLIC BOOL hal_HstSendEvent(UINT32 ch);



// =============================================================================
// hal_HstWaitLastEventSent
// -----------------------------------------------------------------------------
/// Check if the last event has been sent on the host port.
/// You can't send new event after this function
// =============================================================================
PUBLIC VOID hal_HstWaitLastEventSent(VOID);



// @} // <-- End of the host group.

EXTERN_C_END

#endif




