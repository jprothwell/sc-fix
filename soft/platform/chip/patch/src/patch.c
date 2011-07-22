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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/patch/src/patch.c $ //
//    $Author: huazeng $                                                        // 
//    $Date: 2010-09-16 18:26:51 +0800 (Thu, 16 Sep 2010) $                     //   
//    $Revision: 2299 $                                                        //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//       
/// @file patch.c
/// That file implement the patch functions
//
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "patch_m.h"

#include "global_macros.h"
#include "mem_bridge.h"


/// Only used by patch_GetLocation().
PRIVATE UINT32 g_patchUsed = 0;


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
PUBLIC INT32 patch_GetLocation(UINT32** patchArea)
{
    UINT32 patch = g_patchUsed;

    // reserve one patch for GDB
    // and reserve the last patch for the Boot Sector structure.
    if (CHIP_NUMBER_OF_PATCHES-1 < g_patchUsed)
    {
        return PATCH_FAILED;
    }

    if(NULL != patchArea)
    {
        *patchArea = (UINT32*)(KSEG0(REG_INT_SRAM_BASE)) + 4*patch;
    }

    g_patchUsed = g_patchUsed + 1;

    return patch;
}

// =============================================================================
// patch_Enable
// -----------------------------------------------------------------------------
/// This function enables a hardware patch.
/// Once the replacement code is placed in internal sram, use this function to
/// enable the hardware patch.
/// @param patchNumber  the number of the patch as returned by #patch_GetLocation
/// @param patchAddress the address in the rom to patch (will be 16 bytes aligned)
// =============================================================================
PUBLIC VOID patch_Enable(UINT32 patchNumber, UINT32 patchAddress)
{
    // assert patchAddress is in rom !
    // ((patchAddress & 0x00F00000) == REG_INT_ROM_BASE)
    //
    // assert patch exist
    // (patchNumber < NB_ROM_PACHT)

    // enable patch
    hwp_memBridge->Rom_Patch[patchNumber] = MEM_BRIDGE_PATCH_ENABLE
            | ((UINT32)patchAddress & MEM_BRIDGE_BLOCK_ADDR_MASK);

}



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
                                   CONST VOID (*newFunction)(VOID))
{
    UINT32  oldFunctionPtr = (UINT32)oldFunction;
    UINT32  newFunctionPtr = (UINT32)newFunction;

    UINT32 *patchArea = NULL;
    INT32   patch;

    patch = patch_GetLocation(&patchArea);

    if(PATCH_FAILED == patch)
    {
        return PATCH_FAILED;
    }

    // assert oldFunction is in rom !
    // ((oldFunctionPtr & 0x00F00000) == REG_INT_ROM_BASE)
    //
    // assert oldFunction must be alligned on 16 bytes boundaries (patch area)
    // ((oldFunctionPtr & 0xe) == 0)

    // newFunction can be on any ISA Mode (Mips 16 or Mips 32)
    // as we use a jr in the patch code

    if ((oldFunctionPtr & 1) == 1)
    {
        // mips16 old function

        // assemble: lw v0, 12(pc)
        ((UINT16*)patchArea)[0]=0xb203;
        // assemble: nop (lw delay before v0 use)
        ((UINT16*)patchArea)[1]=0x6500;
        // assemble: jr v0
        ((UINT16*)patchArea)[2]=0xea00;
        // assemble: nop (jr delay slot)
        ((UINT16*)patchArea)[3]=0x6500;

        // place data for lw instruction
        patchArea[3] = newFunctionPtr;

    }
    else
    {
        // mips32 old function

        // assemble: lui at, hi(newFunctionPtr)
        patchArea[0] = 0x3c010000 + (newFunctionPtr >> 16);
        // assemble: ori at, at, lo(newFunctionPtr)
        patchArea[1] = 0x34210000 + (newFunctionPtr & 0xffff);
        // assemble: jr at
        patchArea[2] = 0x00200008;
        // assemble: nop
        patchArea[3] = 0;

    }
    // enable patch
    patch_Enable(patch, oldFunctionPtr);

    return patch;
}
