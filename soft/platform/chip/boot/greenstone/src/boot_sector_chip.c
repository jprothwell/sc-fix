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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/boot/greenstone/src/boot_sector_chip.c $
//  $Author: admin $
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $
//  $Revision: 269 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file boot_sector_chip.c
///
/// Contains the implementation of the chip dependent function of
/// the Boot Sector.
/// 
/// Important: all the source of this file is mapped in the Boot Sector (i.e.
/// first section of the Flash).
//
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"

#include "chip_id.h"
#include "global_macros.h"
#include "cfg_regs.h"
#include "sys_ctrl.h"
#include "tcu.h"
#include "ana_acco.h"
#include "mem_bridge.h"

#include "bootp_sector.h"
#include "boot.h"


// =============================================================================
// MACROS                                                                       
// =============================================================================


// Used for the patch functions.
#define PATCH_FAILED -1


// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================


/// Entry of the Boot Monitor in ROM.
EXPORT PROTECTED VOID boot_monitor(VOID);


// =============================================================================
// boot_Memcpy
// -----------------------------------------------------------------------------
/// Documentation of the function.
// =============================================================================
PUBLIC VOID *boot_Memcpy(VOID *dest, CONST VOID *src, UINT32 n)
{
    INT32                     i;
    INT32                     destOffset;
    INT32                     srcOffset;
    INT32                     offset;
    INT32                     offset0;
    INT32                     offset1;
    UINT32                    v;
    UINT32*                   destAddr;
    UINT32*                   srcAddr;
    UINT32                    fifo[2];
    CONST INT32               addrMask = sizeof(UINT32)-1;

    destOffset  = (((UINT32)dest) &  addrMask) * 8;
    srcOffset   = (((UINT32)src)  &  addrMask) * 8;
    offset      = srcOffset - destOffset;

    destAddr    = (UINT32*) (((UINT32)dest| 0x80000000) & ~addrMask);
    srcAddr     = (UINT32*) (((UINT32)src | 0x80000000) & ~addrMask);

    if(n == 0)
    {
        return destAddr;
    }

    if(offset == 0)
    {
        if(destOffset)
        {
            fifo[0]   = *srcAddr;
            v         = fifo[0];

            for(i = destOffset/8; i < (INT32)sizeof(UINT32) && n; ++i, --n)
            {
                ((UINT8*)destAddr)[i] = (v >> (i*8)) & 0xFF;
            }

            ++srcAddr;
            ++destAddr;
        }

        for(; n >= sizeof(UINT32); n -= sizeof(UINT32))
        {
            fifo[0]   = *srcAddr;
            v         = fifo[0];
            *destAddr = v;

            ++srcAddr;
            ++destAddr;
        }

        if(n != 0)
        {
            fifo[0]   = *srcAddr;
            v         = fifo[0];

            for(i = 0; n; ++i, --n)
            {
                ((UINT8*)destAddr)[i] = (v >> (i*8)) & 0xFF;
            }

            ++srcAddr;
            ++destAddr;
        }
    }
    else
    {
        if(offset <= 0)
        {
            offset0 = -offset;
            offset1 = (sizeof(UINT32) * 8) + offset;
        }
        else
        {
            offset1 = offset;
            offset0 = (sizeof(UINT32) * 8) - offset;
        }

        fifo[1] = *srcAddr;
        ++srcAddr;

        if(destOffset)
        {
            if(offset > 0)
            {
                fifo[0] = fifo[1];
                fifo[1] = *srcAddr;
                ++srcAddr;
            }

            v         = (fifo[1] << offset0) | (fifo[0] >> offset1);

            for(i = destOffset/8; i < (INT32)sizeof(UINT32) && n; ++i, --n)
            {
                ((UINT8*)destAddr)[i] = (v >> (i*8)) & 0xFF;
            }

            ++destAddr;
        }

        for(i = 0; n >= sizeof(UINT32); n -= sizeof(UINT32), i ^= 1)
        {
            fifo[i]   = *srcAddr;
            v         = (fifo[i] << offset0) | (fifo[i^1] >> offset1);
            *destAddr = v;

            ++srcAddr;
            ++destAddr;
        }

        if(n != 0)
        {
            fifo[i]   = *srcAddr;
            v         = (fifo[i] << offset0) | (fifo[i^1] >> offset1);

            for(i = 0; n; ++i, --n)
            {
                ((UINT8*)destAddr)[i] = (v >> (i*8)) & 0xFF;
            }

            ++srcAddr;
            ++destAddr;
        }
    }

    return (VOID*)destAddr;
}


// =============================================================================
// boot_HostUsbClockEnable
// -----------------------------------------------------------------------------
/// Start the USB clock as it is not done by the ROMed monitor in Greenstone.
// =============================================================================
PRIVATE VOID boot_HostUsbClockEnable(VOID)
{
    UINT32 i;

    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK;

    // Enable V_ANA on the CT1010.
    hwp_extApb->GPIO_Mode   &= ~REGS_MODE_PIN_LPSCO_1;
    hwp_tcu->LPS_PU_Ctrl    |= TCU_LPS_PU_CO1_ON | TCU_LPS_PU_CO1_POL;

    // Rx control.
    hwp_anaAccoFe->rf_rx_calib = 0x0;
    hwp_anaAccoFe->rf_rx_calib = ANA_ACCO_RX_BG_ENABLE
        | ANA_ACCO_RX_BG_CTRL(0x25)
        | ANA_ACCO_RX_REF_CAL(3);

    hwp_anaAccoFe->rf_ctrl_set = ANA_ACCO_RF_DEEP_PD_RELEASE;

    hwp_extApb->ExtPll_Config = REGS_EXTPLL_INPUT_DIVIDER(0)
        | REGS_EXTPLL_FEEDBACK_DIVIDER(22)
        | REGS_EXTPLL_OUTPUT_DIVIDER_DIV_BY_1;
    hwp_anaAccoFe->pll_ctrl = ANA_ACCO_PLL_PD_N |
        ANA_ACCO_PLL_IN_SOURCE_EXT |
        ANA_ACCO_PLL_CP_TUNE_100_PERCENT |
        ANA_ACCO_PLL_SYS_EON_ENABLE |
        ANA_ACCO_PLL_EXT_EON_ENABLE |
        ANA_ACCO_PLL_RST_LOCK;
    hwp_extApb->ExtPll_Ctrl = REGS_EXTPLL_ENABLE_ENABLE |
        REGS_EXTPLL_CLK_FAST_ENABLE_ENABLE |
        REGS_EXTPLL_OUTPUT_ENABLE_ENABLE   |
        REGS_EXTPLL_LOCK_CTRL_RST_RESET;

    // Wait PLL lock
    for(i = 0; i < 100; ++i);
    hwp_extApb->ExtPll_Ctrl &= ~REGS_EXTPLL_LOCK_CTRL_RST_RESET;
    while(!(hwp_extApb->ExtPll_Ctrl & REGS_EXTPLL_LOCKED_LOCKED));

    hwp_anaAccoFe->pll_ctrl &= ~ANA_ACCO_PLL_RST_LOCK;
    while(!(hwp_anaAccoFe->pll_ctrl & ANA_ACCO_PLL_LOCKED));

    hwp_sysCtrl->REG_DBG    = SYS_CTRL_PROTECT_LOCK;
}


// =============================================================================
// boot_PatchGetLocation
// -----------------------------------------------------------------------------
/// This function allocate a hardware patch, it then need to be enabled with
/// #patch_Enable.
/// Use this function to get a patch location when #patch_ReplaceFunction cannot
/// be used for your patch (patch data or patch assembly code)
/// @param patchNumber  location of the patch to use.
/// @param patchArea    pointer to recieve the address of the patch area
///                     It can be null.
/// @return             the number of the hardware patch or \c PATCH_FAILED
// =============================================================================
PRIVATE INT32 boot_PatchGetLocation(UINT32 patchNumber,
                                    UINT32** patchArea)
{
    UINT32 patch = patchNumber;

    // reserve one patch for GDB
    // and reserve the last patch for the Boot Sector structure.
    if (CHIP_NUMBER_OF_PATCHES-1 < patch)
    {
        return PATCH_FAILED;
    }

    if(NULL != patchArea)
    {
        *patchArea = (UINT32*)(KSEG0(REG_INT_SRAM_BASE)) + 4*patch;
    }

    return patch;
}


// =============================================================================
// boot_PatchEnable
// -----------------------------------------------------------------------------
/// This function enables a hardware patch.
/// Once the replacement code is placed in internal sram, use this function to
/// enable the hardware patch.
/// @param patchNumber  the number of the patch as returned by #patch_GetLocation
/// @param patchAddress the address in the rom to patch (will be 16 bytes aligned)
// =============================================================================
PRIVATE VOID boot_PatchEnable(UINT32 patchNumber, UINT32 patchAddress)
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
// boot_PatchReplaceFunction
// -----------------------------------------------------------------------------
/// This function install and enable a patch to replace a function.
/// The function in ROM must be aligned on 16 bytes boundaries.
/// The two functions must have the exact same prototype !
/// The patch will actually replace \p oldFunction by a direct jump 
/// to \p newFunction.
/// @param oldFunction the address/symbol of the function in ROM to replace.
/// @param newFunction the address/symbol of the new implementation of the 
///                    function.
/// @param patchNumber location of the patch to use.
// =============================================================================
PRIVATE INT32 boot_PatchReplaceFunction(CONST VOID (*oldFunction)(VOID),
                                        CONST VOID (*newFunction)(VOID),
                                        UINT32 patchNumber)
{
    UINT32  oldFunctionPtr = (UINT32)oldFunction;
    UINT32  newFunctionPtr = (UINT32)newFunction;

    UINT32 *patchArea;
    INT32   patch;

    patch = boot_PatchGetLocation(patchNumber, &patchArea);

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
    boot_PatchEnable(patch, oldFunctionPtr);

    return patch;
}


// =============================================================================
// boot_SectorEnterBootMonitor
// -----------------------------------------------------------------------------
/// From the Boot Sector, enter in the Boot Monitor.
/// And, depending on the chip, make the modifications required to have a
/// fully working monitor.
/// This function is implemented in "boot/CT_ASIC/src/boot_sector_chip.c".
// =============================================================================
PROTECTED VOID boot_SectorEnterBootMonitor(VOID)
{
    // Patch the ROMed "memcpy" function, because it is used by the USB monitor
    // and the ROMed version does not handle the non-aligned buffers properly.
    // Address in ROM of memcpy = 0x80e00790
    boot_PatchReplaceFunction((CONST VOID (*)(VOID))0x80e00791,
                              (CONST VOID (*)(VOID))boot_Memcpy, 0);

    /// Force entering the boot monitor.
    /// This function is at a fixed location, in ROM.
    g_bootBootMode |= 2 /* BOOT_MODE_FORCE_MONITOR */;
    boot_monitor();
}

// =============================================================================
// boot_SectorFixRom
// -----------------------------------------------------------------------------
/// Use to fix some startup missing feature:
/// * usb clock on greenstone
/// This function is implemented in "boot/CT_ASIC/src/boot_sector_chip.c".
// =============================================================================
PROTECTED VOID boot_SectorFixRom(VOID)
{
    // Start the USB clock as it is not done by the ROMed monitor in Greenstone.
    boot_HostUsbClockEnable();
}
