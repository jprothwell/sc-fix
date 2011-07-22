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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/asicsimu/src/asicsimu_boot_sector.c $
//  $Author: admin $
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $
//  $Revision: 269 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file boot_sector.c
///
/// Boot sector replacement for simulation (need the special linker script)
//
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "global_macros.h"
#include "debug_host.h"
#include "boot.h"
#include "cfg_regs.h"
#include "sys_ctrl.h"
#include "tcu.h"
#include "chip_id.h"


// =============================================================================
// MACROS                                                                       
// =============================================================================


/// Linker section where the Boot Sector main function must be mapped:
/// Beginning of the first sector of the flash.
#define BOOT_SECTOR_SECTION_START  __attribute__((section (".boot_sector_start")))


// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================


/// This is defined in "bootp_loader.h" but, this header is not accessible
/// from the ASIC Simu module.
EXPORT PROTECTED VOID boot_LoaderEnter(UINT32 param);


PROTECTED VOID BOOT_SECTOR_SECTION_START asicsimu_BootSector(UINT32 param)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK;

    // we boot with the boot mode "bypass power up sequence"
    // so we need to switch to the RF clock now !

    // enable RF power & clock
    hwp_extApb->GPIO_Mode   &= ~REGS_MODE_PIN_LPSCO_1;
    hwp_tcu->LPS_PU_Ctrl    |= TCU_LPS_PU_CO0_ON | TCU_LPS_PU_CO1_ON | TCU_LPS_PU_CLK_RF_ON | TCU_LPS_PU_SPI_ON | TCU_LPS_PU_CO1_POL;
    // switch to RF clock
    hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF | SYS_CTRL_SYS_SEL_FAST_SLOW | SYS_CTRL_SYS_SEL_PLL_EXT | SYS_CTRL_CLOCKOUT_SEL_RF_OSCILLATOR | SYS_CTRL_CLOCK_PLL_UART_SEL_52MHZ;

    hwp_sysCtrl->REG_DBG    = SYS_CTRL_PROTECT_LOCK;

#elif ((CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE))

    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK;

    // we boot with the boot mode "bypass power up sequence"
    // so we need to switch to the RF clock now !

    // enable RF power & clock
    hwp_configRegs->GPIO_Mode   &= ~CFG_REGS_MODE_PIN_LPSCO_1;
    hwp_tcu->LPS_PU_Ctrl    |= TCU_LPS_PU_CO0_ON | TCU_LPS_PU_CO1_ON | TCU_LPS_PU_CLK_RF_ON | TCU_LPS_PU_SPI_ON | TCU_LPS_PU_CO1_POL;

    // switch to RF clock
    hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF | SYS_CTRL_SYS_SEL_FAST_SLOW;

    hwp_sysCtrl->REG_DBG    = SYS_CTRL_PROTECT_LOCK;

#else

    #error "asicsimu boot sector is not implemented for your chip!"

#endif

    boot_LoaderEnter(param);
}

