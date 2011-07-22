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
/// @file mpsp_medium.h
///
/// Protype of function for medium access
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MPSP_MEDIUM_H_
#define _MPSP_MEDIUM_H_

#include "cs_types.h"

// =============================================================================
// MACROS                                                                       
// =============================================================================


// =============================================================================
// TYPES                                                                        
// =============================================================================

// =============================================================================
// MPS_MEDIUM_TYPE_T
// -----------------------------------------------------------------------------
/// List of input medium
// =============================================================================
typedef enum
{
    MPS_MEDIUM_TYPE_INVALID,
    MPS_MEDIUM_TYPE_FS,
    MPS_MEDIUM_TYPE_MEM
} MPS_MEDIUM_TYPE_T;

// =============================================================================
// MPS_MEDIUM_DATA_FS_T
// -----------------------------------------------------------------------------
/// File descriptor for filesystem medium
// =============================================================================
typedef struct
{
    INT32 fd;
} MPS_MEDIUM_DATA_FS_T ;

// =============================================================================
// MPS_MEDIUM_DATA_MEM_T
// -----------------------------------------------------------------------------
/// Buffer description and position for memory medium
// =============================================================================
typedef struct
{
    UINT8* buffer;
    UINT32 size;

    UINT32 position;
} MPS_MEDIUM_DATA_MEM_T ;

// =============================================================================
// MPS_MEDIUM_DATA_T
// -----------------------------------------------------------------------------
/// Internal medium data
// =============================================================================
typedef union
{
    MPS_MEDIUM_DATA_FS_T  fs;
    MPS_MEDIUM_DATA_MEM_T mem;
} MPS_MEDIUM_DATA_T;

// =============================================================================
// MPS_MEDIUM_T
// -----------------------------------------------------------------------------
/// Medium description
// =============================================================================
typedef struct
{
    MPS_MEDIUM_TYPE_T type;
    MPS_MEDIUM_DATA_T data;
} MPS_MEDIUM_T ;

// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================


// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================

PROTECTED INT32 mps_MediumOpen(MPS_MEDIUM_T* medium, MPS_MEDIUM_TYPE_T mediumType,
                               UINT8* buffer, UINT32 size);

PROTECTED INT32 mps_MediumClose(MPS_MEDIUM_T* medium);

PROTECTED INT32 mps_MediumRead(MPS_MEDIUM_T* medium, VOID* buffer, UINT32 size);

PROTECTED INT32 mps_MediumSeek(MPS_MEDIUM_T* medium, INT32 offset, INT32 whence);

PROTECTED UINT32 mps_MediumTell(MPS_MEDIUM_T* medium);

PROTECTED BOOL mps_MediumIsEof(MPS_MEDIUM_T* medium);

PROTECTED UINT32 mps_MediumSize(MPS_MEDIUM_T* medium);

#endif // _MPSP_MEDIUM_H_
