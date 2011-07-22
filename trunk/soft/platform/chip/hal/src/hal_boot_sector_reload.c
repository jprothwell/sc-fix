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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/hal_boot_sector_reload.c $
//  $Author: lijy $
//  $Date: 2010-11-03 15:57:15 +0800 (Wed, 03 Nov 2010) $
//  $Revision: 3366 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file hal_boot_sector_driver.c
///
/// HAL Boot Sector driver.
/// 
/// Provides functions to allow HAL EBC and HAL Host Monitor to write some
/// informations in the Boot Sector structure and to validate this structure.
/// 
/// The name is very long to avoid to be cached by the *boot_sector.o pattern
/// of the linker script.
//
////////////////////////////////////////////////////////////////////////////////

#include "chip_id.h"
#include "global_macros.h"

#include "cs_types.h"

#include "hal_mem_map.h"

#include "boot_sector.h"
#include "halp_debug.h"

// =============================================================================
// MACROS                                                                       
// =============================================================================


// =============================================================================
// FUNCTIONS                                                            
// =============================================================================



// =============================================================================
// hal_BootSectorFillReloadCtx
// -----------------------------------------------------------------------------
/// Fill the boot sector reload context.
/// ctx: pointer to the boot sector reload context.
/// id: boot sector reload id
// =============================================================================
PUBLIC BOOL hal_BootSectorFillReloadCtx(BOOT_SECTOR_RELOAD_CTX_T *ctx, BOOT_SECTOR_RELOAD_ID_T id)
{
    BOOT_SECTOR_RELOAD_STRUCT_T *halBootSectorReloadStructPtr;

    if(id >= BOOT_SECTOR_RELOAD_MAX_NUM){
        HAL_ASSERT(FALSE,"boot sector reload: unkown id %d",id);
        return FALSE;
    }
    halBootSectorReloadStructPtr = boot_BootSectorGetReloadStructPointer();

    halBootSectorReloadStructPtr->ctx[id].storeAddress = ctx->storeAddress;
    halBootSectorReloadStructPtr->ctx[id].mapAddress = ctx->mapAddress;
    halBootSectorReloadStructPtr->ctx[id].size = ctx->size;
    
    return TRUE;
}

// =============================================================================
// hal_BootSectorReloadEnable
// -----------------------------------------------------------------------------
/// Validate the Boot Sector reload structure. This is a way to tell the Boot Sector
/// that the structure contains correct ram image and that it must be loaded
/// and executed when restart.
/// 
/// This is done by putting the address of the Boot Sector reload structure in
/// the Boot Sector reload structure pointer and enabling a flag in it.
// =============================================================================
PUBLIC VOID hal_BootSectorReloadEnable(VOID)
{
    BOOT_SECTOR_RELOAD_STRUCT_T *halBootSectorReloadStructPtr = boot_BootSectorGetReloadStructPointer();
    halBootSectorReloadStructPtr->validTag = BOOT_SECTOR_RELOAD_VALID_TAG;
    halBootSectorReloadStructPtr->checkSum = ~BOOT_SECTOR_RELOAD_VALID_TAG;
}


