////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2008, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//
//  $HeadURL$
//  $Author$
//  $Date$
//  $Revision$
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file utrace.c
///
/// Usb Trace service
//
////////////////////////////////////////////////////////////////////////////////


// =============================================================================
// HEADERS                                                                      
// =============================================================================

#include "hal_usb_monitor.h"
#include "uctls_m.h"
#include "hal_usb_monitor.h"

#include "sxr_ops.h"

// =============================================================================
// MACROS                                                                       
// =============================================================================


// =============================================================================
// TYPES                                                                        
// =============================================================================


// =============================================================================
// PRIVATE VARIABLES                                                            
// =============================================================================


// =============================================================================
// PRIVATE FUNCTIONS                                                            
// =============================================================================

PRIVATE HAL_USB_INTERFACE_DESCRIPTOR_T**
utraces_Open(CONST UCTLS_SERVICE_CFG_T* config);

PRIVATE VOID utraces_Close(VOID);

// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================

PUBLIC CONST UCTLS_SERVICE_CALLBACK_T g_utracesCallback =
{
   .open  =         utraces_Open,
   .close =         utraces_Close
};

// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================

PRIVATE HAL_USB_INTERFACE_DESCRIPTOR_T**
utraces_Open(CONST UCTLS_SERVICE_CFG_T* config)
{
    HAL_USB_INTERFACE_DESCRIPTOR_T** desc;

    desc = (HAL_USB_INTERFACE_DESCRIPTOR_T**) sxr_Malloc(sizeof(HAL_USB_INTERFACE_DESCRIPTOR_T*)*2);

    config   = config;
    desc[0]  = hal_HostUsbOpen();
    desc[1]  = 0;

    return desc;
}

PRIVATE VOID utraces_Close(VOID)
{
    hal_HostUsbClose();
}
