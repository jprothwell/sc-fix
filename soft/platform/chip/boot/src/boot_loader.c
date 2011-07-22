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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/boot/src/boot_loader.c $ //
//  $Author: huazeng $                                                         //
//  $Date: 2011-04-08 15:23:06 +0800 (Fri, 08 Apr 2011) $                     //
//  $Revision: 6970 $                                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file xcpu_init.c                                                         //
/// That file contains the XCPU init function                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "chip_id.h"

#include "global_macros.h"
#include "sys_ctrl.h"
#include "bb_sram.h"
#include "mem_bridge.h"
#include "debug_port.h"
#include "cfg_regs.h"

#include "boot.h"
#include "boot_host.h"
#include "bootp_debug.h"
#include "bootp_loader.h"

#include "tgt_hal_cfg.h"

#include "hal_host.h"
#include "hal_mem_map.h"
#include "hal_open.h"
#include "hal_timers.h"

#include "hal_debug.h"

#include "calib_m.h"
#include "boot_map.h"

// For the opening of the flash
#include "memd_m.h"
#include "tgt_memd_cfg.h"

#ifdef DEPRECATED_DIRTY_WORKAROUND_FOR_USB_CT1010        
#include "gpio.h"
#include "hal_gpio.h"
#endif


// Boot loader entry point.
PROTECTED VOID boot_LoaderEnter(UINT32 param);


// Standard main entry, user function.
extern int main(int, char*[]);


// Calibration entry point.
extern int calib_StubMain(int, char*[]);

// =============================================================================
//  boot_LoaderEnter
// -----------------------------------------------------------------------------
/// Real implementation of the boot loader, loading code after the stack has 
/// been reseted in #boot_LoaderEnter();
///
/// @param param This parameter as a dummy value when we boot normally from ROM,
/// but it has a special value when the function is called by a jump command
/// of a Boot Monitor (Host, USB or UART). This is used to change the main of
/// the code (where we will jump at the end of the boot loading sequence).
// =============================================================================
PROTECTED VOID boot_Loader(UINT32 param);

// =============================================================================
//  boot_LoaderEnter
// -----------------------------------------------------------------------------
/// Entry point for the code out of the boot sector.
///
/// To compensate for the stack usage of the boot romed code, this function
/// resets the stack pointer at the usable top of the internal SRAM, and call
/// the 'real' boot loader without any local variable on the stack that could
/// thus get corrupted.
/// @param param This parameter as a dummy value when we boot normally from ROM,
/// but it has a special value when the function is called by a jump command
/// of a Boot Monitor (Host, USB or UART). This is used to change the main of
/// the code (where we will jump at the end of the boot loading sequence).
// =============================================================================
PROTECTED VOID __attribute__((section(".text.boot_loader_start"))) boot_LoaderEnter(UINT32 param)
{
    // Initializes IRQ stack top to the top of the available sram
    // and set here stack pointer to the desired location,
    // at he top of the int sram that is not preallocated for, eg,
    // the mailbox. This location is provided through the symbol
    // properly defined in the linker script _sys_sram_top.
    sxr_IrqStack_pointer = (UINT32) &_sys_sram_top;
    // $29 is the SP register.
    REG UINT32 stackPointer asm("$29") __attribute__((unused))= sxr_IrqStack_pointer ;

    boot_Loader(param);

    // Sep. 05, 2009 - Hua Zeng - Workaround for stack corruption issue after resetting SP.
    // If we are not using -mips16 (ISA is mips1 by default), with -O2 or more optimazation specified,
    // in case that the final statement is a function call, the compiler will pop the using stack first
    // and then branch to the final function call. Unfortunately, here we just reset the SP to
    // the top of the stack space, so the stack of the final function falls beyond the stack top.
    // The workaournd is to add a dummy statement as the final statement so as to avoid the optimization.
    asm volatile("nop");
}

// =============================================================================
//  boot_BreakBootMain
// -----------------------------------------------------------------------------
/// Called when launching the code in elfmaster debug mode. The purpose is to 
/// break when the code starts, to begin a debugging session.
///
/// Hi! This is Elf Master. You got here because you started me
/// with the "Run Program" option. Your code has been restarted,
/// and is currently stalled at the beginning. You can hit
/// "continue" to unstall it, or you can set some additional 
/// breakpoints where you need to.
// =============================================================================
PROTECTED VOID boot_BreakBootMain( VOID )
{
    // WELCOME TO ELFMASTER!
    BREAKPOINT;
    main(0, 0);
}





// =============================================================================
//  boot_Loader
// -----------------------------------------------------------------------------
/// Real implementation of the boot loader, loading code after the stack has 
/// been reseted in #boot_LoaderEnter();
///
/// @param param This parameter as a dummy value when we boot normally from ROM,
/// but it has a special value when the function is called by a jump command
/// of a Boot Monitor (Host, USB or UART). This is used to change the main of
/// the code (where we will jump at the end of the boot loading sequence).
// =============================================================================
PROTECTED VOID boot_Loader(UINT32 param)
{
    UINT32* src;
    UINT32* dst;

#ifdef DEPRECATED_DIRTY_WORKAROUND_FOR_USB_CT1010
    // Shut down the LDO of the USB to avoid the PC to detect the USB device.
    // This is very dirty fix. This won't be necessary with the new USB integration,
    // at boot, an empty config will be used by default and used for the first
    // USB enumeration.
    //hwp_gpio->gpo_clr = HAL_GPO_1;
// FIXME Handle that properly in the boot_sector.
    HAL_APO_ID_T apo;
    apo.gpoId = HAL_GPO_1;
    hwp_gpio->gpo_clr = 1 << apo.id;

#endif

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    // Force the flash remap so that by default the 16MB accessible 
    // address space is the first 16MB of the flash.
    hwp_memFifo->EBC_Flash_Block_Remap = MEM_FIFO_BLOCK_REMAP(3);
#endif


    // NO FUNCTION CALL ALLOWED AS LONG AS THE CODE HAS NOT BEEN
    // COPIED FROM FLASH TO RAM...

    // If a WD occured, treat it before initializing the bb SRAM.
    if (hwp_sysCtrl->Reset_Cause & SYS_CTRL_WATCHDOG_RESET_HAPPENED)
    {
        if  ((g_halWatchdogCtx.validityMarker == 0xC001C001) ||
            (g_halWatchdogCtx.validityMarker == 0xDEADBEEF) )
        {
            // This is a real watchdog and the structure is valid.
            UINT32* saved_context_ptr = (UINT32*) KSEG1(REG_BB_SRAM_BASE);
            UINT32 i;

            // Copy from BB_SRAM into External sram structure.
            for (i = 0; i < sizeof(HAL_DBG_ERROR_CTX_T)/4; i++)
            {
                *(&(g_halWatchdogCtx.errorContext.zero) + i) =
                    *(saved_context_ptr + i);
            }
            // reset the marker to normal
            g_halWatchdogCtx.validityMarker = 0xC001C001;
            hal_TimeWdRecover();
        }
        else
        {
            // The structure is not valid behave as in case of soft reset.
            g_halWatchdogCtx.validityMarker = 0xDEADBEEF;
            //hwp_sysCtrl->Sys_Rst_Clr = SYS_CTRL_WATCHDOG_STATUS;
        }
    }
    else
    {
        g_halWatchdogCtx.validityMarker = 0xC001C001;
    }

    // Set the global variables from ROM not set by the ROM code.
    xcpu_error_code   = 0;
    xcpu_error_status = 0;
    xcpu_sp_context   = 0;

    // Copy code & data from ROM space to RAM.
    for (src = (UINT32*) &_ram_flash_start_location,
             dst = (UINT32*) &_ram_start;
         src < (UINT32*) &_ram_flash_end_location;
         src++, dst++)
    {
        *dst = *src;
    }

    // Copy code & data from ROM space to internal SRAM.
    for (src = (UINT32*) &_sys_sram_flash_start_location,
             dst = (UINT32*) &_sys_sram_start;
         src < (UINT32*) &_sys_sram_flash_end_location;
         src++, dst++)
    {
        *dst = *src;
    }

    // Clear BSS in Internal SRAM.
    for (dst = (UINT32*) &_sys_sram_ucbss_start;
         dst < (UINT32*) &_sys_sram_ucbss_end;
         dst++)
    {
        *dst = 0;
    }
    for (dst = (UINT32*) &_sys_sram_bss_start;
         dst < (UINT32*) &_sys_sram_bss_end;
         dst++)
    {
        *dst = 0;
    }

    // Load target parameter to speed up the booting
    memd_RamOpen(tgt_GetMemdRamConfig());

    // clear BSS in External RAM
    for (dst = (UINT32*) &_ram_ucbss_start;
         dst < (UINT32*) &_ram_ucbss_end;
         dst++)
    {
        *dst = 0;
    }
    for (dst = (UINT32*) &_ram_bss_start;
         dst < (UINT32*) &_ram_bss_end;
         dst++)
    {
        *dst = 0;
    }

#ifdef REWRITE_ROM_AT_BOOT
#ifdef FPGA
    // Copy data from flash space to internal ROM.

    // Unlock.
    hwp_memBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_ENABLE;

    for (src = (UINT32*) &_int_rom_flash_start_location,
             dst = (UINT32*) &_int_rom_start;
         src < (UINT32*) &_int_rom_flash_end_location;
         src++, dst++)
    {
        *dst = *src;
    }

    // Lock.
    hwp_memBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_DISABLE;
#endif // FPGA
#else // REWRITE_ROM_AT_BOOT
    // Check that the ROM version in the chip is the same as the ROM version
    // of the ROM Library against which the code has been compiled.
    if(ROM_LINKED_AGAINST_VERSION != *(UINT32*)ROM_VERSION_LOCATION)
    {
        // Mismatch!
        hal_HstSendEvent(BOOT_LOADER_EVENT_ROM_ERROR);
        while(1);
    }
#endif // REWRITE_ROM_AT_BOOT


#if (CHIP_HAS_PROXY_MEMORY == 1)
    // Load proxy memory content.
    for (src = (UINT32*) &_proxy_memory_flash_start_location,
             dst = (UINT32*) &_proxy_memory_start;
         src < (UINT32*) &_proxy_memory_flash_end_location;
         src++, dst++)
    {
        *dst = *src;
    }

    // FIXME For test purpose, move the Irq Stack 
    // atop the proxy memory !
    sxr_IrqStack_pointer = (UINT32)&_proxy_memory_top;
#endif

    // Same thing for the bcpu. These are needed by GDB.
    bcpu_error_code   = HAL_DBG_ERROR_NO;
    bcpu_error_status = 0xc001c001;
    bcpu_sp_context   = (UINT32*)0x0db90db9;

    // FUNCTIONS CAN BE CALL NOW!
    // Enable exception (gdb breaks), Host, Debug and Page Spy external it,
    // but not regular external it.
    asm volatile("li    $27,    (0x0001 | 0xe000)\n\t"
                 "mtc0  $0, $13\n\t"
                 "nop\n\t"
                 "mtc0  $27, $12\n\t"
                 // k1 is status for os irq scheme.
                 "li    $27, 1");
    hal_HstSendEvent(BOOT_LOADER_EVENT_START_CODE);

    hal_Open(tgt_GetHalConfig());

    BOOT_PROFILE_PULSE(XCPU_MAIN);

    // Selects where we will boot: normal main or calib stub main.
    switch(param)
    {
        case CALIB_MAGIC_TAG:
            calib_StubMain(0, 0);
            break;
        case BOOT_MAIN_TAG_BREAKBOOT:
            // Gdb needs the flash to be open to be able to step in the romulator!!!
            // TODO: THIS CERTAINLY NEEDS TO BE DONE IN THE BOOT LOADER INSTEAD
            // of ALL MAINS. BECAUSE HERE, WE'RE OPENING TWICE THE FLASH
            memd_FlashOpen(tgt_GetMemdFlashConfig());
            boot_BreakBootMain();
            break;
        default:
            main(0, 0);
            break;
    }
}

