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
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_lcdc.c
/// This file contains the lcdc module driver 
/// implementation.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#if (CHIP_HAS_LCDC==1)

#include "chip_id.h"

#include "cs_types.h"

#include "global_macros.h"
#include "lcdc.h"
#include "cfg_regs.h"
#include "sys_irq.h"

#include "hal_slcdc.h"
#include "hal_sys.h"
#include "halp_sys.h"
#include "hal_error.h"

#include "halp_debug.h"
#include "halp_sys_ifc.h"
#include "halp_lcdc.h"


// ============================================================================
//  DEFINES
// ============================================================================


// ============================================================================
//  GLOBAL VARIABLES
// ============================================================================
PRIVATE UINT8 g_halSlcdcIfcChan = HAL_UNKNOWN_CHANNEL;

// =============================================================================
// g_halSlcdcClkDiv
// -----------------------------------------------------------------------------
/// Table to store the dividers for the serial clock
// =============================================================================
UINT32 g_halSlcdcClkReq = 13000000;

// =============================================================================
// g_halSlcdcIrqHandler
// -----------------------------------------------------------------------------
/// Variable to store the user irq handler for the lcdc
/// interruption.
// =============================================================================
PRIVATE HAL_SLCDC_IRQ_HANDLER_T g_halSlcdcIrqHandler = NULL ;

/// data size, in bytes, ie the number of bytes sent 
/// anytime a data is sent to the screen in fifo mode.
PRIVATE UINT32  g_halSlcdcDataSize = 0;

// ============================================================================
//  FUNCTIONS
// ============================================================================

// =============================================================================
// hal_SlcdcUpdateTimings
// -----------------------------------------------------------------------------
/// Update the divider used by the SLCDC module to fit optimally with the 
/// system frequency. After the call to the #hal_SlcdcOpen function, an 
/// array of timings, one for each system frequency, is registered in to the 
/// HAL LCDC driver.
/// @param sysFreq (New) System frequency for which new timings must be 
/// configured.
// =============================================================================
PRIVATE VOID hal_SlcdcUpdateDivider(HAL_SYS_FREQ_T sysFreq)
{
    HAL_ASSERT(g_halSlcdcClkReq!= 0, "HAL: SLCDC: Serial Clock requested frequency not defined");

    // Calculate divider
    // SCLK = PCLK / (2*(clkDiv+1))
    UINT32 clkDiv=sysFreq/(2*g_halSlcdcClkReq);


    if (clkDiv>1)
    {
        clkDiv--;
    }

    // Setup divider
    hwp_lcdc->SLCD_DIVCLK = LCDC_DIV_CLK(clkDiv);
}

// =============================================================================
// hal_SlcdcOpen
// -----------------------------------------------------------------------------
/// Open the LCDC controller.
/// Parameters are used to describe the screen connexion configuration and 
/// the controller timings to use depending on the system clock frequency.
/// @param config Pointer to the controller configuration.
/// @param timings Controller timing array..
// =============================================================================
PUBLIC VOID hal_SlcdcOpen(CONST HAL_SLCDC_CONFIG_T* config)
{
    // Check that the muxing allow SLCDC
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    HAL_ASSERT((hwp_extApb->Alt_mux_select & REGS_LSB_MEMORY_ADDR_BUS_MASK)
            == REGS_LSB_MEMORY_ADDR_BUS_MEMORY,
            "Muxing does not allow serial LCD.");
#else
#error HAL SLCDC driver is not supported for this architecture
#endif
    // Set configuration register
    // This describes the way the LCD is plugged to the 
    // controller.
    hwp_lcdc->SLCD_CFG = ((config->busKind == HAL_SLCDC_BUS_KIND_3)?LCDC_LINE:0)
        | (config->csPolarity?LCDC_CE:0);
    
    // SLCDC works only in write mode
    hwp_lcdc->SLCD_RW=0;

    // Apply divider for the current system clok frequency:
    g_halSlcdcClkReq=config->SclkFrequencyReq;
    hal_SlcdcUpdateDivider(hal_SysGetFreq());

    // do not request any frequency, but register a clock update function
    hal_SysRequestFreq(HAL_SYS_FREQ_LCD,HAL_SYS_FREQ_32K,hal_SlcdcUpdateDivider);
}


// =============================================================================
// hal_SlcdcClose
// -----------------------------------------------------------------------------
/// Close the LCDC controller.
// =============================================================================
PUBLIC VOID hal_SlcdcClose(VOID)
{
    // FIXME Is this enough ?
    hwp_lcdc->SLCD_CTRL  = 0;
    g_halSlcdcIrqHandler = NULL;
    g_halSlcdcIfcChan    = HAL_UNKNOWN_CHANNEL;
    g_halSlcdcDataSize   = 0;
    hal_SysRequestFreq(HAL_SYS_FREQ_LCD,HAL_SYS_FREQ_32K,NULL);
}


// =============================================================================
// hal_SlcdcWriteReg
// -----------------------------------------------------------------------------
/// Write a command and then a data to the LCD screen (Or a value at a register
/// address, depending on the point of view).
/// 
/// @param addr Address of the register to write, or command to send
/// to the LCD screen
/// @param data Corresponding data to write to the LCD.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_SlcdcWriteReg(UINT16 addr, UINT16 data)
{
    HAL_ERR_T errorStatus = HAL_ERR_NO;
    
    UINT32 status = hal_SysEnterCriticalSection();
    
    if ((hwp_lcdc->SLCD_STATUS & LCDC_READY) == 0)
    {
        // LCDC is busy
        errorStatus = HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        // LCDC is available, we use it
        hwp_lcdc->SLCD_CDOUT = (REG32)addr;
        hwp_lcdc->SLCD_CTRL = LCDC_ENABLE | LCDC_SCL_ENABLE | 0;

        // Unavoidable wait for the previous
        // command to be sent.
        while((hwp_lcdc->SLCD_STATUS & LCDC_READY) == 0);
        hwp_lcdc->SLCD_CDOUT = (REG32)data;
        hwp_lcdc->SLCD_CTRL = LCDC_ENABLE | LCDC_SCL_ENABLE | 0 ;
        
        errorStatus = HAL_ERR_NO;
    }
    
    hal_SysExitCriticalSection(status);
    return errorStatus;
}


// =============================================================================
// hal_SlcdcWriteCmd
// -----------------------------------------------------------------------------
/// Write a command to the LCD.
///
/// @param addr Command to write.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_SlcdcWriteCmd(UINT16 addr)
{
    HAL_ERR_T errorStatus = HAL_ERR_NO;
    
    UINT32 status = hal_SysEnterCriticalSection();
    
    if ((hwp_lcdc->SLCD_STATUS & LCDC_READY) == 0)
    {
        // LCDC is busy
        errorStatus = HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        // LCDC is available, we use it
        hwp_lcdc->SLCD_CDOUT = (REG32)addr;
        hwp_lcdc->SLCD_CTRL = LCDC_ENABLE | LCDC_SCL_ENABLE | 0;

        errorStatus = HAL_ERR_NO;
    }
    
    hal_SysExitCriticalSection(status);
    return errorStatus;
}

// =============================================================================
// hal_SlcdcWriteData
// -----------------------------------------------------------------------------
/// Write a data to the LCD screen.
///
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_SlcdcWriteData(UINT16 data)
{
    HAL_ERR_T errorStatus = HAL_ERR_NO;
    
    UINT32 status = hal_SysEnterCriticalSection();
    
    if ((hwp_lcdc->SLCD_STATUS & LCDC_READY) == 0)
    {
        // LCDC is busy
        errorStatus = HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        // LCDC is available, we use it
        hwp_lcdc->SLCD_CDOUT = (REG32)data;
        hwp_lcdc->SLCD_CTRL = LCDC_ENABLE | LCDC_SCL_ENABLE | LCDC_CD ;
    }

    hal_SysExitCriticalSection(status);
    return errorStatus;
}


// =============================================================================
// hal_SlcdcWriteDataBlock
// -----------------------------------------------------------------------------
/// Send a block of pixel data to the LCD screen.
/// WARNING: only even pixel number are supported, that is to say size must
/// be a mutiple of 4 bytes.
/// 
/// @param pData Pointer to the pixel buffer, to send to the screen.
/// @param size Size of the pixel buffer, in bytes. (Multiple of 4)
/// @param handler If not NULL, user function that is called when the transfer
/// is finished.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_SlcdcWriteDataBlock(CONST UINT16* pData, UINT32 size, HAL_SLCDC_IRQ_HANDLER_T handler)
{
    HAL_ASSERT((size&3) == 0, "HAL: SLCDC: Attempt to write a block whose "
            " size is not a multiple of 32 bits ");
    HAL_ASSERT(((((UINT32)pData)&3) == 0), "HAL: SLCDC: Attempt to write a block whose "
            " start address is not a multiple of 32 bits ");
    UINT32 status = hal_SysEnterCriticalSection();
    if ((hwp_lcdc->SLCD_STATUS & LCDC_READY) == 0)
    {
        // LCDC is busy, abort
        hal_SysExitCriticalSection(status);
        return HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        // Resource is free, we take it by starting
        // the transfer.
        
        // I counted 8 register access to start the DMA
        // transfer. We will assume that transfer of less than
        // 17 pixels are best sent manually.
        // (2 bytes per pixel)
        if (size<17*2) 
        {
            // FIXME: Exit the critical section here,
            // assuming the lock in the LCDD driver.
            hal_SysExitCriticalSection(status);
            UINT32 index;
            for (index=0 ; index<size/2 ; index++)
            {
                // Wait screen ready
                while ((hwp_lcdc->SLCD_STATUS & LCDC_READY) == 0);
                hwp_lcdc->SLCD_CDOUT = (REG32)(pData[index]);
                hwp_lcdc->SLCD_CTRL = LCDC_ENABLE | LCDC_SCL_ENABLE | LCDC_CD ;
            }
            if (handler)
            {
                handler();
            }
            return HAL_ERR_NO;
        }
        else
        {
            // Record handler and enable interrupt if needed.
            g_halSlcdcIrqHandler = handler;
            if (g_halSlcdcIrqHandler != NULL)
            {
                // Set Mask
                hwp_lcdc->SLCD_CFG |= LCDC_SLCD_INT_MASK;
            }
            else
            {
                // Disable IRQ
                hwp_lcdc->SLCD_CFG &= ~LCDC_SLCD_INT_MASK;
            }

            // Start transfer
            g_halSlcdcIfcChan = hal_IfcTransferStart(HAL_IFC_LCDC_TX, (UINT8*)pData, size, HAL_IFC_SIZE_32_MODE_AUTO);
            
            // Enable module
            hwp_lcdc->SLCD_CTRL = LCDC_ENABLE | LCDC_SCL_ENABLE |
                LCDC_APB_FF | LCDC_CD | LCDC_DMA_CNT(size);
   
            hal_SysExitCriticalSection(status);
            return HAL_ERR_NO;
        }
    }
}


// =============================================================================
// hal_SlcdcIrqHandler
// -----------------------------------------------------------------------------
/// Handles IRQ.
// =============================================================================
PROTECTED VOID hal_SlcdcIrqHandler(UINT8 interruptId)
{
    UINT32 tmpRead;
    hwp_lcdc->SLCD_INT= LCDC_INT_CLEAR;
    tmpRead = hwp_lcdc->SLCD_INT;

    // Call user handler
    if (g_halSlcdcIrqHandler)
    {
        g_halSlcdcIrqHandler();
    }
}























#endif  // CHIP_HAS_LCDC


