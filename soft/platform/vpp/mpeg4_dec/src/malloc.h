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
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/vpp/branches/Total_BJ/mpeg4_dec/src/malloc.h $
//  $Author: lilian $
//  $Date$
//  $Revision: 22942 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file malloc.h
///
/// Malloc with garbage colector
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MALLOC_H_
#define _MALLOC_H_

#include "cs_types.h"

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

// =============================================================================
// xvid_myMalloc
// -----------------------------------------------------------------------------
/// This function alloc memory and register the memory allocated in internal
/// list
///
/// @param size Size of memory allocated
/// @return memory allocated
// =============================================================================
VOID* xvid_myMalloc(UINT32 size);

// =============================================================================
// xvid_myFree
// -----------------------------------------------------------------------------
/// This function free memory allocated by xvid_malloc and unregister the memory
/// allocated in internal list
///
/// @param buffer Memory buffer, you want free
// =============================================================================
VOID xvid_myFree(VOID* buffer);

// =============================================================================
// xvid_freeAll
// -----------------------------------------------------------------------------
/// This function free all memory allocated by xvid_malloc and already register
/// in internal list
// =============================================================================
VOID xvid_freeAll(VOID);

#endif // _MALLOC_H_



