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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/boot/greenstone/src/boot_monitor.c $ //
//  $Author: admin $                                                         //
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                     //
//  $Revision: 269 $                                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file boot_monitor.c                                                      //
///     RMC monitor
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "boot_sector_chip.h"
#include "boot_sector.h"

#include "global_macros.h"
#include "timer.h"
#include "uart.h"
#include "debug_host.h"
#include "debug_uart.h"
#include "debug_port.h"
#include "cfg_regs.h"
#include "cp0.h"

#include "boot_host.h"
#include "boot.h"
#include "bootp_mode.h"
#include "bootp_host.h"
#include "boot_usb_monitor.h"
#include "bootp_uart_monitor.h"
#include "bootp_debug.h"

#include "hal_debug.h"




PROTECTED VOID boot_monitor(VOID)
{
    BOOT_HOST_USB_MONITOR_CTX_T hostUsbContext;

    VOID(*flashEntry)(UINT32) = (VOID(*)(UINT32)) BOOT_SECTOR_ENTRY_POINT;

    hostUsbContext.MonitorMode = BOOT_HOST_USB_MODE_BOOT;

    // Configure the IOMux to let the PXTS exit.
#ifdef BOOT_WITH_IOMUX_PROFILE
    hwp_debugPort->Debug_Port_Mode |= DEBUG_PORT_PXTS_EXL_CLOCK_POL
        | DEBUG_PORT_MODE_CONFIG_MODE_PXTS;
    hwp_debugPort->Pxts_Exl_Cfg    |= DEBUG_PORT_ENABLE_PXTS_TAG(1<<HAL_DBG_PXTS_BOOT);
    hwp_extApb->Alt_mux_select     &= ~REGS_DEBUG_PORT_DAI_SIMPLE;
#endif // BOOT_WITH_IOMUX_PROFILE

    BOOT_PROFILE_PULSE(BOOT_POWER_STATE_DONE);

    if ((g_bootBootMode & BOOT_MODE_FORCE_MONITOR) ||
       *(BOOT_FLASH_PROGRAMMED_PATTERN_ADDRESS) != BOOT_FLASH_PROGRAMMED_PATTERN)
    {
        BOOT_PROFILE_PULSE(BOOT_ENTER_MONITOR_LOOP);

        // Open the UART monitor, only when the boot mode requires it.
        if (g_bootBootMode & BOOT_MODE_UART_MONITOR_ENABLE)
        {
            boot_UartMonitorOpen();
        }
#if (CHIP_HAS_USB == 1)
        // Open the USB monitor, except when to boot mode prevents it.
        if (!(g_bootBootMode & BOOT_MODE_USB_MONITOR_DISABLE))
        {
            boot_UsbInitVar();
            boot_HostUsbClockEnable();
            boot_UsbOpen(boot_HostUsbDeviceCallback);
            boot_HostUsbOpen(&hostUsbContext, 0);
        }
#endif
        while (1)
        {
            // Host monitor always enabled. 
            if (boot_HstMonitor())
            {
                break;
            }
            
            // Process the UART monitor, only when to boot mode requires it.
            if (g_bootBootMode & BOOT_MODE_UART_MONITOR_ENABLE)
            {
                if(boot_UartMonitor())
                {
                    break;
                }
            }
            
#if (CHIP_HAS_USB == 1)
            // Process the USB monitor, except when to boot mode prevents it.
            if (!(g_bootBootMode & BOOT_MODE_USB_MONITOR_DISABLE))
            {
                if(boot_HostUsbRecv())
                {
                    break;
                }
                boot_HostUsbSend();
            }
#endif
        }

        // Close the UART monitor, only when the boot mode requires it.
        if (g_bootBootMode & BOOT_MODE_UART_MONITOR_ENABLE)
        {
            boot_UartMonitorClose();
        }

#if (CHIP_HAS_USB == 1)
        // Close the USB monitor, except when to boot mode prevents it.
        if (!(g_bootBootMode & BOOT_MODE_USB_MONITOR_DISABLE))
        {
            boot_HostUsbClose();
            boot_UsbClose();
        }
#endif
    }

    BOOT_PROFILE_PULSE(BOOT_DIRECT_IN_FLASH);

    flashEntry(BOOT_SECTOR_STANDARD_MAIN);
}















