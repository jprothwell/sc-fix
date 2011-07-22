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
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/svc/branches/Total_BJ/umss/storage/ram/src/umss_storageram.c $
//  $Author: lilian $
//  $Date$
//  $Revision: 13445 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file umss_storageram.c
///
/// Storage ram function
//
////////////////////////////////////////////////////////////////////////////////


// =============================================================================
// HEADERS                                                                      
// =============================================================================

#include "umss_config.h"

#include "sxr_mem.h"
#include "sxs_io.h"

#include <string.h>

// =============================================================================
// MACROS                                                                       
// =============================================================================


// =============================================================================
// TYPES                                                                        
// =============================================================================

// =============================================================================
// UMSS_STORAGE_RAM_HANDLER_T
// -----------------------------------------------------------------------------
/// Ram handler
// =============================================================================
typedef struct
{
    UMSS_STORAGE_SIZE_T  size;
    VOID*                ram;
} UMSS_STORAGE_RAM_HANDLER_T;

// =============================================================================
// PROTOTYPE FUNCTIONS                                                          
// =============================================================================

PRIVATE VOID*
umss_StorageRamOpen   (CONST UMSS_STORAGE_PARAMETERS_T* cfg);

PRIVATE VOID
umss_StorageRamClose  (VOID* handler);

PRIVATE INT32
umss_StorageRamRead   (VOID* handler, VOID* data, UINT32 lba, UINT32 size);

PRIVATE INT32
umss_StorageRamWrite  (VOID* handler, VOID* data, UINT32 lba, UINT32 size);

PRIVATE UMSS_STORAGE_SIZE_T*
umss_StorageRamGetsize(VOID* handler);

PRIVATE BOOL
umss_StorageRamPresent(VOID* handler);

// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================

PUBLIC CONST UMSS_STORAGE_CALLBACK_T g_umssStorageRamCallback =
{
    .open    = umss_StorageRamOpen,
    .close   = umss_StorageRamClose,
    .read    = umss_StorageRamRead,
    .write   = umss_StorageRamWrite,
    .getsize = umss_StorageRamGetsize,
    .present = umss_StorageRamPresent
};

// =============================================================================
// PRIVATE FUNCTIONS                                                            
// =============================================================================

PRIVATE BOOL umss_StorageRamPresent(VOID* handler)
{
    UMSS_STORAGE_RAM_HANDLER_T* handlerRam;

    handlerRam = (UMSS_STORAGE_RAM_HANDLER_T*) handler;

    return(TRUE);
}

PRIVATE VOID*  umss_StorageRamOpen(CONST UMSS_STORAGE_PARAMETERS_T* cfg)
{
    UMSS_STORAGE_RAM_HANDLER_T* handlerRam;
    UINT32                      size;

    if(cfg == 0)
    {
        return 0;
    }

    handlerRam = (UMSS_STORAGE_RAM_HANDLER_T*)
        sxr_Malloc(sizeof(UMSS_STORAGE_RAM_HANDLER_T));

    handlerRam->size.nbBlock   = cfg->size/512;
    handlerRam->size.sizeBlock = 512;
    size                       =
        handlerRam->size.sizeBlock * handlerRam->size.nbBlock;
    handlerRam->ram            = sxr_Malloc(size);

    if(handlerRam->ram == 0)
    {
        sxr_Free(handlerRam);
        handlerRam = 0;
    }

    return((VOID*)(handlerRam));
}

PRIVATE VOID   umss_StorageRamClose(VOID* handler)
{
    UMSS_STORAGE_RAM_HANDLER_T* handlerRam;

    handlerRam = (UMSS_STORAGE_RAM_HANDLER_T*) handler;

    if(handler)
    {
        sxr_Free(handlerRam->ram);
        sxr_Free(handler);
    }
}

PRIVATE INT32 umss_StorageRamRead(VOID* handler, VOID* data, UINT32 lba, UINT32 nbBlock)
{
    UMSS_STORAGE_RAM_HANDLER_T* handlerRam;
    VOID*                       dataMem;
    UINT32                      size;

    handlerRam = (UMSS_STORAGE_RAM_HANDLER_T*) handler;

    size             = handlerRam->size.sizeBlock*nbBlock;

    dataMem = &((UINT8*)handlerRam->ram)[lba*handlerRam->size.sizeBlock];

    memcpy(data, dataMem, size);

    return(nbBlock);
}

PRIVATE INT32 umss_StorageRamWrite(VOID* handler, VOID* data, UINT32 lba, UINT32 nbBlock)
{
    UMSS_STORAGE_RAM_HANDLER_T* handlerRam;
    VOID*                       dataMem;
    UINT32                      size;

    handlerRam = (UMSS_STORAGE_RAM_HANDLER_T*) handler;

    size             = handlerRam->size.sizeBlock*nbBlock;

    dataMem = &((UINT8*)handlerRam->ram)[lba*handlerRam->size.sizeBlock];

    memcpy(dataMem, data, size);

    return(nbBlock);
}

PRIVATE UMSS_STORAGE_SIZE_T* umss_StorageRamGetsize(VOID* handler)
{
    UMSS_STORAGE_RAM_HANDLER_T* handlerRam;

    handlerRam = (UMSS_STORAGE_RAM_HANDLER_T*) handler;

    return(&handlerRam->size);
}

// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================
