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
//                                                                            //
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/patch/include/patch_m.h $ //
//    $Author: huazeng $                                                        // 
//    $Date: 2010-09-16 18:26:51 +0800 (Thu, 16 Sep 2010) $                     //   
//    $Revision: 2299 $                                                        //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
///
/// @file patch_m.h
/// That file describes the patch interface.
///
/// The PATCH module gives the interface to setup a patch and also provides
/// as submodules the current set of patch applyed to a particular chip's ROM
///
////////////////////////////////////////////////////////////////////////////////


#ifndef _PATCH_M_H_
#define _PATCH_M_H_

#include "cs_types.h"

/// @defgroup PATCH
/// @{


// =============================================================================
//  MACROS
// =============================================================================

#define PATCH_FAILED    (-1)

// =============================================================================
//  TYPES
// =============================================================================


// =============================================================================
//  FUNCTIONS
// =============================================================================


// =============================================================================
// patch_Open
// -----------------------------------------------------------------------------
/// This function install all the hardware patch available for the current chip
// =============================================================================
PUBLIC VOID patch_Open(VOID);


// =============================================================================
// patch_ReplaceFunction
// -----------------------------------------------------------------------------
/// This function install and enable a patch to replace a function.
/// The function in ROM must be aligned on 16 bytes boundaries.
/// The two functions must have the exact same prototype !
/// The patch will actually replace \p oldFunction by a direct jump 
/// to \p newFunction.
/// @param oldFunction  the address/symbol of the function in ROM to replace.
/// @param newFunction  the address/symbol of the new implementation of the 
///                     function.
/// @return             the number of the allocated hardware patch 
///                     or \c PATCH_FAILED
// =============================================================================
PUBLIC INT32 patch_ReplaceFunction(CONST VOID (*oldFunction)(VOID),
                                   CONST VOID (*newFunction)(VOID));

// =============================================================================
// patch_GetLocation
// -----------------------------------------------------------------------------
/// This function allocate a hardware patch, it then need to be enabled with
/// #patch_Enable.
/// Use this function to get a patch location when #patch_ReplaceFunction cannot
/// be used for your patch (patch data or patch assembly code)
/// @param patchArea    pointer to recieve the address of the patch area
///                     It can be null.
/// @return             the number of the hardware patch or \c PATCH_FAILED
// =============================================================================
PUBLIC INT32 patch_GetLocation(UINT32** patchArea);


// =============================================================================
// patch_Enable
// -----------------------------------------------------------------------------
/// This function enables a hardware patch.
/// Once the replacement code is placed in internal sram, use this function to
/// enable the hardware patch.
/// @param patchNumber  the number of the patch as returned by #patch_GetLocation
/// @param patchAddress the address in the rom to patch (will be 16 bytes aligned)
// =============================================================================
PUBLIC VOID patch_Enable(UINT32 patchNumber, UINT32 patchAddress);


///  @} <- End of the PATCH group 

#endif // _PATCH_M_H_

