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

#include "hal_lcdc.h"
#include "hal_sys.h"
#include "halp_sys.h"
#include "hal_error.h"
#include "hal_timers.h"

#include "halp_debug.h"
#include "halp_sys_ifc.h"
#include "halp_lcdc.h"


// ============================================================================
//  DEFINES
// ============================================================================
#define LCDC_ENDIAN 0


// ============================================================================
//  GLOBAL VARIABLES
// ============================================================================
PRIVATE UINT8 g_halLcdcIfcChan = HAL_UNKNOWN_CHANNEL;


// =============================================================================
// g_halLcdcIrqHandler
// -----------------------------------------------------------------------------
/// Variable to store the user irq handler for the lcdc
/// interruption.
// =============================================================================
PRIVATE HAL_LCDC_IRQ_HANDLER_T g_halLcdcIrqHandler = NULL ;

/// Store the timings.
PRIVATE CONST HAL_LCDC_TIMINGS_T* g_halLcdcTimings = NULL;

/// data size, in bytes, ie the number of bytes sent 
/// anytime a data is sent to the screen in fifo mode.
PRIVATE UINT32  g_halLcdcDataSize = 0;

// ============================================================================
//  FUNCTIONS
// ============================================================================

// =============================================================================
// hal_LcdcUpdateTimings
// -----------------------------------------------------------------------------
/// Update the timings used by the LCDC module to fit optimally with the 
/// system frequency. After the call to the #hal_LcdcOpen function, an 
/// array of timings, one for each system frequency, is registered in to the 
/// HAL LCDC driver.
/// @param sysFreq (New) System frequency for which new timings must be 
/// configured.
// =============================================================================
PROTECTED VOID hal_LcdcUpdateTimings(HAL_SYS_FREQ_T sysFreq)
{
    // Index in the timing registry as to where to find
    // the settings to apply at that given frequency.
    UINT32 index=0;
    switch (sysFreq)
    {
        case HAL_SYS_FREQ_32K  :
            index = 0;
            break;

        case HAL_SYS_FREQ_13M  :
            index = 1;
            break;

        case HAL_SYS_FREQ_26M  :
            index = 2;
            break;

        case HAL_SYS_FREQ_39M  :
            index = 3;
            break;

        case HAL_SYS_FREQ_52M  :
            index = 4;
            break;

        case HAL_SYS_FREQ_78M  :
            index = 5;
            break;

        case HAL_SYS_FREQ_104M :
            index = 6;
            break;

        case HAL_SYS_FREQ_156M :
            index = 7;
            break;

        default:
            HAL_ASSERT(FALSE, "HAL: LCDC: Unsupported system "
                    "frequency %d", sysFreq);
    }

    HAL_ASSERT(index<HAL_LCDC_TIMINGS_QTY, "HAL: LCDC: Timing "
            "For this index don't exist: %d", index);

    HAL_ASSERT(g_halLcdcTimings!= NULL, "HAL: LCDC: No Timings"
            "registered yet");

    // Timings are cleverly represented by
    // bitfields:
    hwp_lcdc->LCD_TIMING = g_halLcdcTimings[index].reg;
}

// =============================================================================
// hal_LcdcOpen
// -----------------------------------------------------------------------------
/// Open the LCDC controller.
/// Parameters are used to describe the screen connexion configuration and 
/// the controller timings to use depending on the system clock frequency.
/// @param config Pointer to the controller configuration.
/// @param timings Controller timing array..
// =============================================================================
PUBLIC VOID hal_LcdcOpen(CONST HAL_LCDC_CONFIG_T* config, CONST HAL_LCDC_TIMINGS_T* timings)
{
    // Check that the muxing allow LCDC
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    HAL_ASSERT((hwp_extApb->Alt_mux_select & REGS_LSB_MEMORY_ADDR_BUS_MASK)
            == REGS_LSB_MEMORY_ADDR_BUS_PARALLEL_LCD,
            "Muxing does not allow parallel LCD.");
#else
#error HAL LCDC driver is not supported for this architecture
#endif
    // TODO Add parameter validity check.
    
    // Set configuration register
    // This describes the way the LCD is plugged to the 
    // controller.
    hwp_lcdc->LCD_CFG = LCDC_LCD_CFG_DBS(config->busWidth) 
        | LCDC_LCD_CFG_CS(config->cs) 
        | ((config->busKind == HAL_LCDC_BUS_KIND_68)?LCDC_LCD_CFG_PIS:0)
        | (config->csPolarity?LCDC_LCD_CS_POL(config->cs):LCDC_LCD_CS_POL(0));
    
    // Register the timings
    g_halLcdcTimings = timings;

    // Apply timings for the current system clok frequency:
    hal_LcdcUpdateTimings(hal_SysGetFreq());
    
    // Do not request any frequency (yet ...), but register a clock update function.
    hal_SysRequestFreq(HAL_SYS_FREQ_LCD, HAL_SYS_FREQ_32K, hal_LcdcUpdateTimings);


    // Set the data size.
    switch (config->busWidth)
    {
        case HAL_LCDC_BUS_WIDTH_8_BITS:
            g_halLcdcDataSize = 1;
            break;

        case HAL_LCDC_BUS_WIDTH_9_BITS:
        case HAL_LCDC_BUS_WIDTH_16_BITS:
            g_halLcdcDataSize = 2;
            break;

        case HAL_LCDC_BUS_WIDTH_18_BITS:
            g_halLcdcDataSize = 4;
            break;

        default:
            HAL_ASSERT(FALSE, "HAL: LCDC: Unsupported bus width.");
    }

//  Leave default value
//  hwp_lcdc->apbi_ctrl_lcd = LCDC_SOFT_RST_L | LCDC_L_ENDIAN(LCDC_ENDIAN);
}


// =============================================================================
// hal_LcdcClose
// -----------------------------------------------------------------------------
/// Close the LCDC controller.
// =============================================================================
PUBLIC VOID hal_LcdcClose(VOID)
{
    // FIXME Is this enough ?
    hwp_lcdc->LCD_CTRL  = 0;
    g_halLcdcIrqHandler = NULL;
    g_halLcdcIfcChan    = HAL_UNKNOWN_CHANNEL;
    g_halLcdcDataSize   = 0;
    hal_SysRequestFreq(HAL_SYS_FREQ_LCD,HAL_SYS_FREQ_32K,NULL);
}


// =============================================================================
// hal_LcdcWriteReg
// -----------------------------------------------------------------------------
/// Write a command and then a data to the LCD screen (Or a value at a register
/// address, depending on the point of view).
/// 
/// @param addr Address of the register to write, or command to send
/// to the LCD screen.
/// @param data Corresponding data to write to the LCD.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_LcdcWriteReg(UINT16 addr, UINT16 data)
{
    HAL_ERR_T errorStatus = HAL_ERR_NO;
    
    UINT32 status = hal_SysEnterCriticalSection();
    
    if ((hwp_lcdc->LCD_STATUS & LCDC_LCD_RDY) == 0)
    {
        // LCDC is busy
        errorStatus = HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        // LCDC is available, we use it
        hwp_lcdc->LCD_DATA = (REG32)addr;
        hwp_lcdc->LCD_CTRL = LCDC_LCD_ENABLE | LCDC_LCD_CTRL_RW | 0;

        // Unavoidable wait for the previous
        // command to be sent.
        while((hwp_lcdc->LCD_STATUS & LCDC_LCD_RDY) == 0);
        hwp_lcdc->LCD_DATA = (REG32)data;
        hwp_lcdc->LCD_CTRL = LCDC_LCD_ENABLE | LCDC_LCD_CTRL_RW | LCDC_LCD_CTRL_RS ;
        
        errorStatus = HAL_ERR_NO;
    }
    
    hal_SysExitCriticalSection(status);
    return errorStatus;
}


// =============================================================================
// hal_LcdcReadReg
// -----------------------------------------------------------------------------
/// Write the address and then read a data from the LCD register
/// 
/// @param addr Address of the register to read.
/// @param pData Pointer to where to store the read data.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_LcdcReadReg(UINT16 addr, UINT16* pData)
{
    HAL_ERR_T errorStatus = HAL_ERR_NO;

    UINT32 status = hal_SysEnterCriticalSection();
    
    if ((hwp_lcdc->LCD_STATUS & LCDC_LCD_RDY) == 0)
    {
        // LCDC is busy
        errorStatus = HAL_ERR_RESOURCE_BUSY;
    }
    else // LCDC is available, we use it
    {
        // Write the adress    
        hwp_lcdc->LCD_DATA = (REG32)addr;
        hwp_lcdc->LCD_CTRL = LCDC_LCD_ENABLE | LCDC_LCD_CTRL_RW | 0;

        while((hwp_lcdc->LCD_STATUS & LCDC_LCD_RDY) == 0);
        // Ask for a read
        hwp_lcdc->LCD_CTRL = LCDC_LCD_ENABLE | 0 | LCDC_LCD_CTRL_RS ;
        while((hwp_lcdc->LCD_STATUS & LCDC_LCD_RDY) == 0);
        // Get the data
        *pData = hwp_lcdc->LCD_DATA;

        errorStatus = HAL_ERR_NO;
    }

    hal_SysExitCriticalSection(status);
    return errorStatus;
}


// =============================================================================
// hal_LcdcWriteCmd
// -----------------------------------------------------------------------------
/// Write a command to the LCD.
///
/// @param addr Command to write.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_LcdcWriteCmd(UINT16 addr)
{
    HAL_ERR_T errorStatus = HAL_ERR_NO;
    
    UINT32 status = hal_SysEnterCriticalSection();
    
    if ((hwp_lcdc->LCD_STATUS & LCDC_LCD_RDY) == 0)
    {
        // LCDC is busy
        errorStatus = HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        // LCDC is available, we use it
        hwp_lcdc->LCD_DATA = (REG32)addr;
        hwp_lcdc->LCD_CTRL = LCDC_LCD_ENABLE | LCDC_LCD_CTRL_RW | 0;
        errorStatus = HAL_ERR_NO;
    }
    
    hal_SysExitCriticalSection(status);
    return errorStatus;
}

// =============================================================================
// hal_LcdcWriteData
// -----------------------------------------------------------------------------
/// Write a data to the LCD screen.
///
/// @param data Data to write.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_LcdcWriteData(UINT16 data)
{
    HAL_ERR_T errorStatus = HAL_ERR_NO;
    
    UINT32 status = hal_SysEnterCriticalSection();
    
    if ((hwp_lcdc->LCD_STATUS & LCDC_LCD_RDY) == 0)
    {
        // LCDC is busy
        errorStatus = HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        // LCDC is available, we use it
        hwp_lcdc->LCD_DATA = (REG32)data;
        hwp_lcdc->LCD_CTRL = LCDC_LCD_ENABLE | LCDC_LCD_CTRL_RW | LCDC_LCD_CTRL_RS ;
    }

    hal_SysExitCriticalSection(status);
    return errorStatus;
}


// =============================================================================
// hal_LcdcReadData
// -----------------------------------------------------------------------------
/// Read a data to the LCD screen.
///
/// @param pData Pointer to where to store the read data.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_LcdcReadData(UINT16* pData)
{
    HAL_ERR_T errorStatus = HAL_ERR_NO;
    
    UINT32 status = hal_SysEnterCriticalSection();

    if ((hwp_lcdc->LCD_STATUS & LCDC_LCD_RDY) == 0)
    {
        // LCDC is busy
        errorStatus = HAL_ERR_RESOURCE_BUSY;
    }
    else // LCDC is available, we use it
    {
        // Ask for a read    
        hwp_lcdc->LCD_CTRL = LCDC_LCD_ENABLE | 0 | LCDC_LCD_CTRL_RS ;
        while((hwp_lcdc->LCD_STATUS & LCDC_LCD_RDY) == 0);
        // Get the data
        *pData = hwp_lcdc->LCD_DATA;

        errorStatus = HAL_ERR_NO;
    }


    hal_SysExitCriticalSection(status);
    return errorStatus;
}


// =============================================================================
// hal_LcdcWriteDataBlock
// -----------------------------------------------------------------------------
/// Send a block of pixel data to the LCD screen.
/// WARNING: only even pixel number are supported, that is to say size must
/// be a multiple of 4 bytes.
/// 
/// @param pData Pointer to the pixel buffer, to send to the screen.
/// @param size Size of the pixel buffer, in bytes. (Multiple of 4)
/// @param handler If not NULL, user function that is called when the transfer
/// is finished.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_LcdcWriteDataBlock(CONST UINT16* pData, UINT32 size, HAL_LCDC_IRQ_HANDLER_T handler)
{
    HAL_ASSERT((size&3) == 0, "HAL: LCDC: Attempt to write a block whose "
            " size is not a multiple of 32 bits ");
    HAL_ASSERT(((((UINT32)pData)&3) == 0), "HAL: LCDC: Attempt to write a block whose "
            " start address is not a multiple of 32 bits ");
    UINT32 status = hal_SysEnterCriticalSection();
    if ((hwp_lcdc->LCD_STATUS & LCDC_LCD_RDY) == 0)
    {
        // LCDC is busy, abort
        hal_SysExitCriticalSection(status);
        return HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        hal_SysRequestFreq(HAL_SYS_FREQ_LCD, HAL_SYS_FREQ_104M, hal_LcdcUpdateTimings);
        // Resource is free, we take it by starting
        // the transfer.
        
        // I counted 8 register access to start the DMA
        // transfer. We will assume that transfer of less than
        // 17 pixels are best sent manually.
        // (2 bytes per pixel)
        
        // BUT: There is a context switch to execute the interrupt 
        // handler that we can count as a cost of
        // - 22 instruction cache lines fetch for hal_irq_handler.S
        // - 43 instruction cache lines fetch for hal_IrqDispatch
        // -  9 instruction cache lines fetch for the SX schedule function
        // - 26 instruction cache lines fetch for sxr_RunNextTask
        // - 13 instruction cache lines fetch for sxr_TaskSwap
        //
        // That is to say there are 113 lines to fetch of CPU times just to 
        // allow for the execution of the recorded handler.
        // This 113 bust access can be thought to be equivalent to 113 write
        // accesses (Lines are fetch as 4 words burst, and a pixel write is 
        // a single 16 bits access). That is to say that writing 113*2 = 226 bytes
        // 'by hand' would take as much CPU time.
        //
        // Measurements (See the wiki page http://atlas/twiki/bin/view/CoolWiki/Modem2GLcdcBenchmarking ), by defining
        // LCDC_BENCHMARK and using the reftest_lcdc test) shows that IFC/DMA becomes faster
        // than by CPU copies for a buffer of about 220 bytes (using the cache) or 304 bytes
        // (when the cache is completely flushed before each buffer copy on the LCDC).
        // Assuming we're lucky enough to have half the stuff in cache, we aims at the 
        // middle and get 260 bytes as a criterion to switch from manual (CPU) copy 
        // to DMA.
        if (size<260) 
        {
            // FIXME: Exit the critical section here,
            // assuming the lock in the LCDD driver.
            hal_SysExitCriticalSection(status);
            UINT32 index;
            for (index=0 ; index<size/2 ; index++)
            {
                // Wait screen ready
                while ((hwp_lcdc->LCD_STATUS & LCDC_LCD_RDY) == 0);
                hwp_lcdc->LCD_DATA = (REG32)(pData[index]);
                hwp_lcdc->LCD_CTRL = LCDC_LCD_ENABLE | LCDC_LCD_CTRL_RW | LCDC_LCD_CTRL_RS ;
            }
            if (handler)
            {
                handler();
            }
            hal_SysRequestFreq(HAL_SYS_FREQ_LCD, HAL_SYS_FREQ_32K, hal_LcdcUpdateTimings);
            return HAL_ERR_NO;
        }
        else
        {
            // Record handler and enable interrupt if needed.
            g_halLcdcIrqHandler = handler;
            if (g_halLcdcIrqHandler != NULL)
            {
                // Set Mask
                hwp_sysIrq->Pulse_Mask_Set = SYS_IRQ_SYS_IRQ_DMA_TX_LCDC;
            }
            else
            {
                // Disable IRQ
                hwp_sysIrq->Pulse_Mask_Clr = SYS_IRQ_SYS_IRQ_DMA_TX_LCDC;
            }


            HAL_PROFILE_PULSE(HAL_LCDC_START_BLOCK_PULSE);
            // Start transfer
            g_halLcdcIfcChan = hal_IfcTransferStart(HAL_IFC_LCDC_TX, (UINT8*)pData, size, HAL_IFC_SIZE_32_MODE_AUTO);
            
            // Enable module
            hwp_lcdc->LCD_CTRL = LCDC_LCD_ENABLE | LCDC_LCD_CTRL_RW
                        | LCDC_LCD_CTRL_RS | LCDC_LCD_CTRL_AF
                        | LCDC_LCD_CTRL_CNT(size/g_halLcdcDataSize);
   
            hal_SysExitCriticalSection(status);
            return HAL_ERR_NO;
        }
    }
}


#if (LCDC_BENCHMARK == 1)

#include "halp_gdb_stub.h"

PROTECTED UINT32 g_halLcdcBenchSize         = 0;
PROTECTED UINT32 g_halLcdcBenchStartTime    = 0;
PROTECTED UINT32 g_halLcdcBenchCpuDuration  = 0;
PROTECTED UINT32 g_halLcdcBenchDmaDuration  = 0;
PROTECTED UINT32 g_halLcdcBenchLocked       = FALSE;


// =============================================================================
// hal_LcdcWriteDataBlockBench
// -----------------------------------------------------------------------------
/// Benching version of Send a block of pixel data to the LCD screen.
/// WARNING: only even pixel number are supported, that is to say size must
/// be a multiple of 4 bytes.
/// 
/// @param pData Pointer to the pixel buffer, to send to the screen.
/// @param size Size of the pixel buffer, in bytes. (Multiple of 4)
/// @param handler If not NULL, user function that is called when the transfer
/// is finished.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_LcdcWriteDataBlockBench(CONST UINT16* pData, UINT32 size, HAL_LCDC_IRQ_HANDLER_T handler, BOOL flush)
{
    HAL_ASSERT((size&3) == 0, "HAL: LCDC: Attempt to write a block whose "
            " size is not a multiple of 32 bits ");
    HAL_ASSERT(((((UINT32)pData)&3) == 0), "HAL: LCDC: Attempt to write a block whose "
            " start address is not a multiple of 32 bits ");

    UINT32 status = hal_SysEnterCriticalSection();
    if ((hwp_lcdc->LCD_STATUS & LCDC_LCD_RDY) == 0 || g_halLcdcBenchLocked)
    {
        // LCDC is busy, abort
        hal_SysExitCriticalSection(status);
        return HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        hal_SysRequestFreq(HAL_SYS_FREQ_LCD, HAL_SYS_FREQ_104M, hal_LcdcUpdateTimings);
        if (flush)
        {
            hal_GdbFlushCache();
        }

        g_halLcdcBenchLocked = TRUE;
        
        g_halLcdcBenchSize = size;

        // Resource is free, we take it by starting
        // the transfer.
        
        // I counted 8 register access to start the DMA
        // transfer. We will assume that transfer of less than
        // 17 pixels are best sent manually.
        // (2 bytes per pixel)
        
        // First Run with the CPU
        HAL_PROFILE_WINDOW_ENTER(hal_LcdcWriteDataBlockBenchCpu);
        g_halLcdcBenchStartTime = hal_TimGetUpTime();
        {
            // FIXME: Exit the critical section here,
            // assuming the lock in the LCDD driver.
            hal_SysExitCriticalSection(status);
            UINT32 index;
            for (index=0 ; index<size/2 ; index++)
            {
                // Wait screen ready
                while ((hwp_lcdc->LCD_STATUS & LCDC_LCD_RDY) == 0);
                hwp_lcdc->LCD_DATA = (REG32)(pData[index]);
                hwp_lcdc->LCD_CTRL = LCDC_LCD_ENABLE | LCDC_LCD_CTRL_RW | LCDC_LCD_CTRL_RS ;
            }
            
            g_halLcdcBenchCpuDuration = hal_TimGetUpTime() - g_halLcdcBenchStartTime;

        }
        HAL_PROFILE_WINDOW_EXIT(hal_LcdcWriteDataBlockBenchCpu);
        
        if (flush)
        {
            hal_GdbFlushCache();
        }
        
        // Second Run, With DMA.
        HAL_PROFILE_WINDOW_ENTER(hal_LcdcWriteDataBlockBenchDma);
        g_halLcdcBenchStartTime = hal_TimGetUpTime();
        {
            // Record handler and enable interrupt if needed.
            g_halLcdcIrqHandler = handler;
            if (g_halLcdcIrqHandler != NULL)
            {
                // Set Mask
                hwp_sysIrq->Pulse_Mask_Set = SYS_IRQ_SYS_IRQ_DMA_TX_LCDC;
            }
            else
            {
                // Disable IRQ
                hwp_sysIrq->Pulse_Mask_Clr = SYS_IRQ_SYS_IRQ_DMA_TX_LCDC;
            }


            // Start transfer
            g_halLcdcIfcChan = hal_IfcTransferStart(HAL_IFC_LCDC_TX, (UINT8*)pData, size, HAL_IFC_SIZE_32_MODE_AUTO);
            
            // Enable module
            hwp_lcdc->LCD_CTRL = LCDC_LCD_ENABLE | LCDC_LCD_CTRL_RW
                        | LCDC_LCD_CTRL_RS | LCDC_LCD_CTRL_AF
                        | LCDC_LCD_CTRL_CNT(size/g_halLcdcDataSize);
   
            hal_SysExitCriticalSection(status);
            return HAL_ERR_NO;
        }
    }
}

PUBLIC VOID hal_LcdcBenchHandler(VOID)
{
    g_halLcdcBenchDmaDuration = hal_TimGetUpTime() - g_halLcdcBenchStartTime;
    HAL_PROFILE_WINDOW_EXIT(hal_LcdcWriteDataBlockBenchDma);

    HAL_TRACE(HAL_LCD_TRC, 0, "Size: %d, CPU duration:%d, DMA duration:%d", 
              g_halLcdcBenchSize, g_halLcdcBenchCpuDuration, g_halLcdcBenchDmaDuration);

    g_halLcdcBenchLocked = FALSE;
}

#endif


// =============================================================================
// hal_LcdcIrqHandler
// -----------------------------------------------------------------------------
/// Handles IRQ.
// =============================================================================
PROTECTED VOID hal_LcdcIrqHandler(UINT8 interruptId)
{
    UINT32 tmpRead;
    hwp_sysIrq->Pulse_Clear = SYS_IRQ_SYS_IRQ_DMA_TX_LCDC;
    tmpRead = hwp_sysIrq->Pulse_Clear;

    // Call user handler
    if (g_halLcdcIrqHandler)
    {
        g_halLcdcIrqHandler();
    }
    
    hal_SysRequestFreq(HAL_SYS_FREQ_LCD, HAL_SYS_FREQ_32K, hal_LcdcUpdateTimings);
    
}























#endif  // CHIP_HAS_LCDC


