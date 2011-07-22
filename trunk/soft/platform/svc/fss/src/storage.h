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
/// @file storage.h
///
/// Fat32 storage interface
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "cs_types.h"

#include "umss_config.h"


// =============================================================================
// MACROS                                                                       
// =============================================================================

#define NR_DEVICE_TYPE 4

#define INVALID_DEVICE_NUMBER 0

#define DEV_STATE_UNFOUND 0

#define DRV_BLOCK_READ(dev_nr, blk_nr, buf)                             \
    g_device_info_table[dev_nr].storage->read((VOID*)g_device_info_table[dev_nr].dev_handle, buf, blk_nr, 1)

#define DRV_BLOCK_WRITE(dev_nr, blk_nr, buf) \
    g_device_info_table[dev_nr].storage->write((VOID*)g_device_info_table[dev_nr].dev_handle, buf, blk_nr, 1)


// =============================================================================
// TYPES                                                                        
// =============================================================================

struct device_info
{
    UINT8*                     dev_name;
    INT32                      dev_handle;
    UINT32                     dev_state; /*used, OR deleted*/
    UMSS_STORAGE_CALLBACK_T*   storage;
};

// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================

EXPORT struct device_info g_device_info_table[NR_DEVICE_TYPE];

// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================


#endif // _STORAGE_H_
