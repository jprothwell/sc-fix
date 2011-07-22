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
//
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/hal_sys.c $
//  $Author: huazeng $
//  $Date: 2011-05-09 17:23:46 +0800 (Mon, 09 May 2011) $
//  $Revision: 7474 $
//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_sys.c
/// Contains the SysCtrl driver implementation.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"

#include "chip_id.h"

#include "tgt_hal_cfg.h"
#include "string.h"

#include "global_macros.h"
#include "sys_ctrl.h"
#include "tcu.h"

#if (CHIP_HAS_ACCO_PLL)
#include "ana_acco.h"
#endif

#if (CHIP_HAS_ASYNC_BCPU)
#include "bb_ctrl.h"
#endif

#include "cfg_regs.h"
#include "mem_bridge.h"
#include "calendar.h"
#include "sys_irq.h"

#include "hal_sys.h"
#include "hal_lps.h"
#include "halp_lps.h"
#include "hal_mem_map.h"

#include "halp_sys.h"
#include "halp_ebc.h"
#include "halp_host.h"
#include "halp_camera.h"
#include "hal_debug.h"

#include "halp_gdb_stub.h"

#include "halp_debug.h"
#include "boot.h"

#include "halp_usb_monitor.h"

#include "boot_usb_monitor.h"

#include "hal_host.h" // this is to send debug events
#include "pmd_m.h"


// =============================================================================
//  MACROS
// =============================================================================
#define HAL_SYS_MAX_SPEED   CHIP_MAX_SPEED

// Don't limit the clock if we use PXTS on RAM or TRACE.
#if (defined(PXTS_MAX_SUPPORTED_FREQUENCY) && !defined(HAL_PROFILE_NOT_ON_ROMULATOR))
#if (PXTS_MAX_SUPPORTED_FREQUENCY < HAL_SYS_MAX_SPEED)
#undef HAL_SYS_MAX_SPEED
#define HAL_SYS_MAX_SPEED PXTS_MAX_SUPPORTED_FREQUENCY
#endif
#endif

/// extern HAL_SYS_CLOCK_OUT_FREQ_T enum
#define HAL_SYS_CLOCK_OUT_OFF 0

#define HAL_DATA_BACKUP __attribute__((section (".backup")))

#define SYS_SHUTDOWN_MAGIC_NUM 0xa1b2c3fe

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

PRIVATE UINT32 HAL_DATA_BACKUP g_halSysStateShutdownFlag;

// =============================================================================
// g_halSysFreqScalRegistry
// -----------------------------------------------------------------------------
/// This array is used to store the frequency required by each frequency scaling
/// user, one per byte.
// =============================================================================
PROTECTED UINT32 g_halSysFreqScalRegistry[HAL_SYS_FREQ_MAX_RESOURCE_NB/4];

// =============================================================================
// g_halSysFreqHandlerRegistry
// -----------------------------------------------------------------------------
/// This array is used to store the user handlers to be called at frequency 
/// change.
// =============================================================================
PRIVATE HAL_SYS_FREQ_CHANGE_HANDLER_T g_halSysFreqHandlerRegistry[HAL_SYS_TOTAL_USERS_QTY];

// ============================================================================
// g_halSysSystemFreq
// ----------------------------------------------------------------------------
/// Global var to have a view of the system frequency.
/// At boot, we are first on the 26Mhz Rf clock.
// ============================================================================
PROTECTED HAL_SYS_FREQ_T g_halSysSystemFreq = HAL_SYS_FREQ_26M;

#if (CHIP_HAS_USB == 1)
PRIVATE BOOL g_halUsbHostEnable = FALSE;
#endif /* CHIP_HAS_USB */

// ============================================================================
// g_halNumberOfUserPerFreq
// ----------------------------------------------------------------------------
/// This array is used to calculate the minimum system frequency to set
/// (that is the maximum required by a module among all the module) in a 
/// constant time (relative to the number of users).
/// 
// ============================================================================
PROTECTED UINT32 g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_QTY];



// =============================================================================
// g_halSysClockOutRegistry
// -----------------------------------------------------------------------------
/// This array is used to store the frequency required by each users for clock out.
// =============================================================================
PROTECTED UINT32 g_halSysClockOutRegistry[HAL_SYS_CLOCK_OUT_ID_QTY] = {0};


PRIVATE HAL_SYS_CALLBACK_T *g_halSysSystemFreqCallback[HAL_SYS_FREQ_LEVEL_QTY] = {NULL,NULL};

PRIVATE BOOL g_halSysSystemFreqFallen = FALSE;

PRIVATE UINT32 g_halSysAuxClkUserNum = 0;

#if (!CHIP_HAS_ASYNC_TCU)
// =============================================================================
// g_halSysClockSwitchQBitError
// -----------------------------------------------------------------------------
/// To compensate error. Each sys clock switch induces a pll clock error on TCU qbit
//  The value counts in clocks at 312 Mhz (1 QBit = 288 clocks)
// =============================================================================
PRIVATE INT32 g_halSysClockSwitchQBitError = 0;

// =============================================================================
// g_halSysClockSwitchErrorTable312
// -----------------------------------------------------------------------------
/// Clock switch error compensation table (in 312 MHz clocks) when the PLL clock is 312 Mhz
// =============================================================================
CONST INT8 g_halSysClockSwitchQBitErrorCompensate312[6][6] =
{  //  26    39    52    78   104  156
    {   0 ,   8 ,   0 ,   8 ,  18,  20}, //   26 
    {  -4 ,   0 ,   4 ,   8 ,  14,  16}, //   39 
    {  -6 ,  -4 ,   0 ,   2 ,   6, 8}, //   52 
    {  -8 ,  -8 ,   0 ,   0 ,   2,  4 }, //   78 
    {  -9 , -10 ,   0 ,  -1 ,  0,  2 },  //  104 
    {  -12 , -12 ,   0 ,  -1 , -2, 0 }  //  156     
};

#if (CHIP_HAS_ACCO_PLL)
// =============================================================================
// g_halSysClockSwitchErrorTable312
// -----------------------------------------------------------------------------
/// Clock switch error compensation table (in 312 MHz clocks) when the PLL clock is 156 Mhz
// =============================================================================
CONST INT8 g_halSysClockSwitchQBitErrorCompensate156[4][4] =
{  //  26    39    52    78  
    {   0 ,   4 ,  -1 ,   0 }, //   26 
    {   0 ,   0 ,   3 ,   4 }, //   39 
    {   1 ,  -1 ,   0 ,   1 }, //   52 
    {   0 ,  -4 ,  -1 ,   0 }  //   78
};
#endif // (CHIP_HAS_ACCO_PLL)

#endif // (!CHIP_HAS_ASYNC_TCU)

// =============================================================================
//  FUNCTIONS
// =============================================================================

// =============================================================================
// hal_SysCallFreqChangeHandlers
// -----------------------------------------------------------------------------
/// Call all the registered frequency change handlers for the given frequency.
/// @param sysFreq target frequency of the current change.
// =============================================================================
PRIVATE VOID hal_SysCallFreqChangeHandlers(HAL_SYS_FREQ_T sysFreq);

// =============================================================================
// hal_SysChangeSystemClock
// -----------------------------------------------------------------------------
/// Change the system clock (previously started and configured) to a (possibly)
/// new value
/// @param fastClockSetup New System clock applied to the system.
// =============================================================================
PRIVATE VOID hal_SysChangeSystemClock(HAL_SYS_FREQ_T fastClockSetup);


// =============================================================================
// hal_SysStartBcpu
// -----------------------------------------------------------------------------
/// Start the BCPU with a stack to at the given address.
/// @param bcpuMain Main function to run on the BCPU.
/// @param stackStartAddr Stack top for the program running on the BCPU
// =============================================================================
PUBLIC VOID hal_SysStartBcpu(VOID* bcpuMain, VOID* stackStartAddr)
{
    UINT32 status = hal_SysEnterCriticalSection();
    bcpu_stack_base = (UINT32) stackStartAddr;
    bcpu_main_entry = (UINT32*) bcpuMain;
#if (!CHIP_HAS_ASYNC_BCPU)
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
    hwp_sysCtrl->Clk_BB_Enable = SYS_CTRL_ENABLE_BB_BCPU;
    hwp_sysCtrl->BB_Rst_Clr = SYS_CTRL_CLR_RST_BCPU;
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
#else // (CHIP_HAS_ASYNC_BCPU)
    hwp_bbCtrl->REG_DBG = BB_CTRL_PROTECT_UNLOCK;
    hwp_bbCtrl->Clk_BB_Enable = BB_CTRL_ENABLE_BB_BCPU;
    hwp_bbCtrl->BB_Rst_Clr = BB_CTRL_CLR_RST_BCPU;
    hwp_bbCtrl->REG_DBG = BB_CTRL_PROTECT_LOCK;
#endif // (!CHIP_HAS_ASYNC_BCPU)
    hal_SysExitCriticalSection(status);
}


// =============================================================================
// hal_SysStopBcpu
// -----------------------------------------------------------------------------
/// Stop the BCPU
// =============================================================================
PUBLIC VOID hal_SysStopBcpu(VOID)
{
    UINT32 status = hal_SysEnterCriticalSection();
#if (!CHIP_HAS_ASYNC_BCPU)
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
    hwp_sysCtrl->BB_Rst_Set = SYS_CTRL_SET_RST_BCPU;
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
#else // (CHIP_HAS_ASYNC_BCPU)
    hwp_bbCtrl->REG_DBG = BB_CTRL_PROTECT_UNLOCK;
    hwp_bbCtrl->BB_Rst_Set = BB_CTRL_SET_RST_BCPU;
    hwp_bbCtrl->REG_DBG = BB_CTRL_PROTECT_LOCK;
#endif // (!CHIP_HAS_ASYNC_BCPU)
    hal_SysExitCriticalSection(status);
}

// =============================================================================
// hal_SysRequestSetCallback
// -----------------------------------------------------------------------------
/// @parameter cbfunc function to be called at frequency request
// =============================================================================
PUBLIC VOID hal_SysRequestSetCallback(HAL_SYS_CALLBACK_T* cbFunc, HAL_SYS_FREQ_LEVEL_ID_T level)
{
	g_halSysSystemFreqCallback[level] = cbFunc;
}

// =============================================================================
// hal_SysSetRequestFreq
// -----------------------------------------------------------------------------
/// When a given users requires a minimum frequency to be able to execute a
/// task, this function must be called.
/// @param userId Identifier of the calling user.
/// @param minFreq Minimum required frequency.
/// @param handler User function registered for this userId 
/// The function is called when the frequency is switched by any following call
/// to hal_SysSetRequestFreq() including the current one.
/// If \c NULL, no function is registered.
// =============================================================================
PUBLIC VOID hal_SysRequestFreq(HAL_SYS_FREQ_USER_ID_T userId, HAL_SYS_FREQ_T minFreq,
                                HAL_SYS_FREQ_CHANGE_HANDLER_T handler)
{
    UINT32 status;
    UINT32 requiredFreq = minFreq;
//    UINT32 callerAddr;

//    SXS_GET_RA(&callerAddr);

    // Frequency clamping
    if (requiredFreq > HAL_SYS_MAX_SPEED)
    {
        HAL_TRACE(HAL_LPS_TRC, 0, "hal_SysRequestFreq clamped the request for a %d frequency. %d", requiredFreq, HAL_SYS_MAX_SPEED);
        requiredFreq = HAL_SYS_MAX_SPEED;
    }

//    HAL_TRACE(HAL_LPS_TRC,0,"hal_SysRequestFreq %d %d from 0x%lx",userId,minFreq,callerAddr); 

    // Keep a coherent table
    status = hal_SysEnterCriticalSection();

    // Convert freq and set in the registry holding the frequency
    // required by each module
    hal_SysFreqScalSet(userId, hal_SysSF2FSF(requiredFreq));

    g_halSysFreqHandlerRegistry[userId] = handler;

    hal_SysUpdateSystemFrequency();
    
    hal_SysExitCriticalSection(status);
}

// =============================================================================
// hal_SysCallFreqChangeHandlers
// -----------------------------------------------------------------------------
/// Call all the registered frequency change handlers for the given frequency.
/// @param sysFreq target frequency of the current change.
// =============================================================================
PRIVATE VOID hal_SysCallFreqChangeHandlers(HAL_SYS_FREQ_T sysFreq)
{
    UINT32 i;
    for(i=0; i<HAL_SYS_TOTAL_USERS_QTY; i++)
    {
        if(g_halSysFreqHandlerRegistry[i])
        {
            g_halSysFreqHandlerRegistry[i](sysFreq);
        }
    }
}

// only Greenstone has a 2-PLL clock scheme
#if (CHIP_HAS_ACCO_PLL)
/// put the variable pll selection variable in internal ram 
PROTECTED UINT32 HAL_DATA_INTERNAL g_sysCtrlSelPll = SYS_CTRL_SYS_SEL_PLL_EXT;

VOID hal_SysGreenstoneExtPLLCtrlBeforeSwitch(BOOL startExtPLL)
{
    if(startExtPLL && (g_sysCtrlSelPll == SYS_CTRL_SYS_SEL_PLL_ACCO))
    { // switch from ACCO to EXT PLL as clock sys source
        hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK;
        // Enable system PLL (ExtPll)
        hwp_extApb->ExtPll_Config = REGS_EXTPLL_INPUT_DIVIDER(0)
                                    | REGS_EXTPLL_FEEDBACK_DIVIDER(22)
                                    | REGS_EXTPLL_OUTPUT_DIVIDER_DIV_BY_1;

        hwp_extApb->ExtPll_Ctrl = REGS_EXTPLL_ENABLE_ENABLE
                                | REGS_EXTPLL_LOCK_CTRL_RST_RESET
                                | REGS_EXTPLL_CLK_FAST_ENABLE_ENABLE 
                                | REGS_EXTPLL_OUTPUT_ENABLE_ENABLE;
        // wait for lock
        hwp_extApb->ExtPll_Ctrl &= ~REGS_EXTPLL_LOCK_CTRL_RST_RESET;
        while(!(hwp_extApb->ExtPll_Ctrl & REGS_EXTPLL_LOCKED_LOCKED));
        // ensure Ext Pll Fast Dividers are updated
        hwp_sysCtrl->Cfg_Clk_Sys = hwp_sysCtrl->Cfg_Clk_Sys | SYS_CTRL_FORCE_DIV_UPDATE;
        hwp_sysCtrl->Cfg_Clk_Mem_Bridge = hwp_sysCtrl->Cfg_Clk_Mem_Bridge;
        hwp_sysCtrl->Cfg_Clk_Voc = hwp_sysCtrl->Cfg_Clk_Voc;
        hwp_sysCtrl->Cfg_Clk_AudioBCK_Div = hwp_sysCtrl->Cfg_Clk_AudioBCK_Div;
        
        // switch clock sys source
        g_sysCtrlSelPll = SYS_CTRL_SYS_SEL_PLL_EXT;
        hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF
                            | SYS_CTRL_SYS_SEL_FAST_FAST
                            | g_sysCtrlSelPll;
        // ensure switched
        while((hwp_sysCtrl->Sel_Clock&SYS_CTRL_SYS_PLL_SELECTED_ACCO));

        // EXT PLL is started, configure ACCO for USB
        // reset lock
        hwp_anaAccoFe->pll_ctrl |= ANA_ACCO_PLL_RST_LOCK;
        while(hwp_anaAccoFe->pll_ctrl & ANA_ACCO_PLL_LOCKED);
        // reconfigure
        hwp_anaAccoFe->pll_ctrl = ANA_ACCO_PLL_PD_N |
             ANA_ACCO_PLL_IN_SOURCE_EXT |
             ANA_ACCO_PLL_CP_TUNE_100_PERCENT |
             ANA_ACCO_PLL_SYS_EON_ENABLE |
             ANA_ACCO_PLL_EXT_EON_ENABLE;
        // wait for lock
        while(!(hwp_anaAccoFe->pll_ctrl & ANA_ACCO_PLL_LOCKED));
        //
        hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK;
    }
}

VOID hal_SysGreenstoneExtPLLCtrlAfterSwitch(BOOL startExtPLL)
{
    if(!startExtPLL && (g_sysCtrlSelPll == SYS_CTRL_SYS_SEL_PLL_EXT))
    { // switch from EXT PLL to ACCO as clock sys source
        hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK;
        // configure ACCO PLL for clock sys
        // reset lock
        hwp_anaAccoFe->pll_ctrl |= ANA_ACCO_PLL_RST_LOCK;
        while(hwp_anaAccoFe->pll_ctrl & ANA_ACCO_PLL_LOCKED);
        // reconfigure
        hwp_anaAccoFe->pll_ctrl = ANA_ACCO_PLL_PD_N |
             ANA_ACCO_PLL_IN_SOURCE_CLOCKSQUARER |
             ANA_ACCO_PLL_CP_TUNE_100_PERCENT |
             ANA_ACCO_PLL_SYS_EON_ENABLE |
             ANA_ACCO_PLL_EXT_EON_NO_CLOCK;
        // wait for lock
        while(!(hwp_anaAccoFe->pll_ctrl & ANA_ACCO_PLL_LOCKED));
        // switch clock sys source
        g_sysCtrlSelPll = SYS_CTRL_SYS_SEL_PLL_ACCO;
        hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF
                            | SYS_CTRL_SYS_SEL_FAST_FAST
                            | g_sysCtrlSelPll;
        // ensure switched
        while(!(hwp_sysCtrl->Sel_Clock&SYS_CTRL_SYS_PLL_SELECTED_ACCO));

        // stop EXT PLL
        hwp_extApb->ExtPll_Ctrl = REGS_EXTPLL_ENABLE_POWER_DOWN |
                                  REGS_EXTPLL_OUTPUT_ENABLE_DISABLE;
        hwp_extApb->ExtPll_Ctrl = REGS_EXTPLL_ENABLE_POWER_DOWN |
                                  REGS_EXTPLL_OUTPUT_ENABLE_DISABLE |
                                  REGS_EXTPLL_LOCK_CTRL_RST_RESET;
        //
        hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK;
    }
}

#endif // (CHIP_HAS_ACCO_PLL)

// =============================================================================
// hal_SysUpdateSystemFrequency
// -----------------------------------------------------------------------------
/// Set the system frequency to the highest minimal frequency required 
/// by the user of the system.
// =============================================================================
PROTECTED VOID hal_SysUpdateSystemFrequency(VOID)
{
    UINT32 i;
    HAL_SYS_FREQ_T newSysFreq;
    HAL_SYS_FREQ_T oldSysFreq = g_halSysSystemFreq;

    // Look for the min to meet all requirement (ie the biggest)
    i = HAL_SYS_FREQ_SCAL_QTY-1;

    // Check i>0 to avoid problems when running into here while
    // hal_SysSetupSystemClock() has not been called yet.
    // E.g., crash dump in hal_Open() -- pal_SleepSerial().
    while (g_halNumberOfUserPerFreq[i]==0 && i>0)
    {
        i--;
    }
    
    // i hold the highest frequency which is required by a least
    // on user, in the FSF (Frequency scaling Fequency format)
    newSysFreq = hal_SysFSF2SF(i);

    if (
        g_halSysSystemFreqFallen &&
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
        (userId !=HAL_SYS_FREQ_SPI) &&
#endif
        (oldSysFreq <= HAL_SYS_FREQ_39M) && (newSysFreq > HAL_SYS_FREQ_39M)
       )
    {
        g_halSysSystemFreqFallen = FALSE;
        if (g_halSysSystemFreqCallback[HAL_SYS_FREQ_LEVEL_RISE] != NULL)
        {
            g_halSysSystemFreqCallback[HAL_SYS_FREQ_LEVEL_RISE]();
        }
    }

// only Greenstone has a 2-PLL clock scheme
#if (CHIP_HAS_ACCO_PLL)

#if (HAL_ACCO_PLL_SCHEME == 1)
    BOOL startExtPLL = FALSE;
    // running at 104 ?
    if(i == HAL_SYS_FREQ_SCAL_104M)
    {
        startExtPLL = TRUE;
    }
    // any of the following resource is activated ?
    else if (  (hal_SysFreqScalGet(HAL_SYS_FREQ_STEREO) != HAL_SYS_FREQ_SCAL_32K)
            || (hal_SysFreqScalGet(HAL_SYS_FREQ_CAMCLK) != HAL_SYS_FREQ_SCAL_32K)
            || (hal_SysFreqScalGet(HAL_SYS_FREQ_USB)    != HAL_SYS_FREQ_SCAL_32K) )
    {
        startExtPLL = TRUE;
    }
#else // always use 2 PLLs
    BOOL startExtPLL = TRUE;
#endif
    
    hal_SysGreenstoneExtPLLCtrlBeforeSwitch(startExtPLL);
#endif // (CHIP_HAS_ACCO_PLL)

    // reconvert freq and apply change
    hal_SysChangeSystemClock(newSysFreq);
    
// only Greenstone has a 2-PLL clock scheme
#if (CHIP_HAS_ACCO_PLL)
    hal_SysGreenstoneExtPLLCtrlAfterSwitch(startExtPLL);
#endif // (CHIP_HAS_ACCO_PLL)

    if (
        !g_halSysSystemFreqFallen &&
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
        (userId !=HAL_SYS_FREQ_SPI) &&
#endif
        (oldSysFreq > HAL_SYS_FREQ_39M) && (g_halSysSystemFreq <=  HAL_SYS_FREQ_39M)
       )
    {
        g_halSysSystemFreqFallen = TRUE;
        if (g_halSysSystemFreqCallback[HAL_SYS_FREQ_LEVEL_FALL] != NULL)
        {
            g_halSysSystemFreqCallback[HAL_SYS_FREQ_LEVEL_FALL]();
        }
    }
}



// =============================================================================
// hal_SysSF2FSF
// -----------------------------------------------------------------------------
/// Convert a System Frequency into a Frequency Scaling Frequency
/// @todo Speed could be improved by ordering according to the the more
/// used HAL_SYS_FREQ_xxx values.
// =============================================================================
PROTECTED HAL_SYS_FREQ_SCAL_FREQ_T hal_SysSF2FSF(HAL_SYS_FREQ_T freqSetup)
{
    HAL_SYS_FREQ_SCAL_FREQ_T convValue = 0;
    // The freq parameter holds the required frequency
    // at the beginning, so that we can adujst dependending 
    // on the system limitation.
    HAL_SYS_FREQ_T freq = freqSetup;

    switch (freq)
    {
        case  HAL_SYS_FREQ_32K:
            convValue =  HAL_SYS_FREQ_SCAL_32K;
            break;

        case HAL_SYS_FREQ_13M:
            convValue =  HAL_SYS_FREQ_SCAL_13M;
            break;

        case HAL_SYS_FREQ_26M:
            convValue =  HAL_SYS_FREQ_SCAL_26M;
            break;

        case HAL_SYS_FREQ_39M:
            convValue =  HAL_SYS_FREQ_SCAL_39M;
            break;

        case HAL_SYS_FREQ_52M:
            convValue =  HAL_SYS_FREQ_SCAL_52M;
            break;

        case HAL_SYS_FREQ_78M:
            convValue =  HAL_SYS_FREQ_SCAL_78M;
            break;

        case HAL_SYS_FREQ_104M:
            convValue =  HAL_SYS_FREQ_SCAL_104M;
            break;

        case HAL_SYS_FREQ_156M:
            convValue =  HAL_SYS_FREQ_SCAL_156M;
            break;

        default:
            HAL_ASSERT(FALSE, "Improper system frequency: %d", freq);
            break;
    }

    return convValue;
}



// =============================================================================
// hal_SysFSF2SF
// -----------------------------------------------------------------------------
/// Convert a Frequency Scaling Frequency into a System Frequency
// =============================================================================
PROTECTED HAL_SYS_FREQ_T hal_SysFSF2SF(HAL_SYS_FREQ_SCAL_FREQ_T fsfFreq)
{
    HAL_SYS_FREQ_T convValue = 0;

    switch (fsfFreq)
    {
        case  HAL_SYS_FREQ_SCAL_32K:
            convValue =  HAL_SYS_FREQ_32K;
            break;

        case HAL_SYS_FREQ_SCAL_13M:
            convValue =  HAL_SYS_FREQ_13M;
            break;

        case HAL_SYS_FREQ_SCAL_26M:
            convValue =  HAL_SYS_FREQ_26M;
            break;

        case HAL_SYS_FREQ_SCAL_39M:
            convValue =  HAL_SYS_FREQ_39M;
            break;

        case HAL_SYS_FREQ_SCAL_52M:
            convValue =  HAL_SYS_FREQ_52M;
            break;

        case HAL_SYS_FREQ_SCAL_78M:
            convValue =  HAL_SYS_FREQ_78M;
            break;

        case HAL_SYS_FREQ_SCAL_104M:
            convValue =  HAL_SYS_FREQ_104M;
            break;

        case HAL_SYS_FREQ_SCAL_156M:
            convValue =  HAL_SYS_FREQ_156M;
            break;

        default:
            HAL_ASSERT(FALSE, "Improper Freq. scaling frequency: %d", fsfFreq);
            break;
    }

    return convValue;
}

// =============================================================================
// hal_SysGetRequestFreq
// -----------------------------------------------------------------------------
/// Get the minimum frequency required by a given user
/// @param userId Identifier of the calling user.
/// @return The minimum required frequency for this user.
// =============================================================================
PUBLIC HAL_SYS_FREQ_T hal_SysGetRequestFreq(HAL_SYS_FREQ_USER_ID_T userId)
{
    return (hal_SysFSF2SF(hal_SysFreqScalGet(userId)));
}



// =============================================================================
// hal_SysGetFreq
// -----------------------------------------------------------------------------
/// Get the current system clock frequency.
/// @return The current system clock frequency.
// =============================================================================
PUBLIC HAL_SYS_FREQ_T hal_SysGetFreq(VOID)
{
    return g_halSysSystemFreq;
}



// =============================================================================
// hal_SysShutdown
// -----------------------------------------------------------------------------
/// A call to this function shuts the system down.
// =============================================================================
PUBLIC BOOL hal_SysGetShutdownFlag(VOID)
{
    if(g_halSysStateShutdownFlag == SYS_SHUTDOWN_MAGIC_NUM)
    {
        return TRUE;
    }
    return FALSE;
}

PUBLIC VOID hal_SysResetShutdownFlag(VOID)
{
    g_halSysStateShutdownFlag = 0x0;
}

PUBLIC VOID hal_SysShutdown(VOID)
{
    // Check that the alarm has finished to be set
    // if an alarm has been programmed before shutting 
    // down
    while (hwp_calendar->Cmd & CALENDAR_ALARM_ENABLE_SET);

    UINT32 status __attribute__((unused))  = hal_SysEnterCriticalSection();

    hal_HstSendEvent(0x1a11dead);

    // Enter power-save mode
    pmd_Close();
    // Power down
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
    hwp_sysCtrl->WakeUp  = 0;

    // Force to flush the write buffer.
    // VOLATILE is needed as the compiler optimizes the while (TRUE)
    // too much.
    VOLATILE UINT32  dummyRead __attribute__((unused)) = hwp_sysCtrl->WakeUp;

    // set the shutdown flag here
    g_halSysStateShutdownFlag = SYS_SHUTDOWN_MAGIC_NUM;

    // wait 1 second ...
    hal_TimDelay(HAL_TICK1S);
    // we are still alive ?
    // just reset so we don't get stuck here!
    hal_SysRestart();

    while (TRUE);
}



// =============================================================================
// hal_SysSoftReset
// -----------------------------------------------------------------------------
/// Does a soft reset on the chip.
/// Generally, you should NOT use this function, use hal_SysRestart() instead.
// =============================================================================
PUBLIC VOID hal_SysSoftReset(VOID)
{
    UINT32 status __attribute__((unused))  = hal_SysEnterCriticalSection();

    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
#if (CHIP_HAS_ASYNC_BCPU)
    hwp_sysCtrl->Sys_Rst_Set= SYS_CTRL_SYS_SOFT_RST;
#else
    hwp_sysCtrl->Sys_Rst_Set= SYS_CTRL_SOFT_RST;
#endif
    while (1); // We die here ...
    // hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
    // hal_SysExitCriticalSection(status);
}



// =============================================================================
// hal_SysRestart
// -----------------------------------------------------------------------------
/// A call to this function reboots the system.
/// First, this function does a power cycle on USB
// =============================================================================
PUBLIC VOID hal_SysRestart(VOID)
{
#if (CHIP_HAS_USB == 1)
    UINT32 startTime;

    startTime = hal_TimGetUpTime();
    hal_UsbReset(TRUE);
    while(hal_TimGetUpTime() < startTime + 8000);
    hal_UsbReset(FALSE);
#endif // (CHIP_HAS_USB == 1)

    hal_SysSoftReset();
}



// =============================================================================
// hal_SysDisablePLL
// -----------------------------------------------------------------------------
/// Disable PLL and switch on low clock
// =============================================================================
PROTECTED VOID hal_SysDisablePLL(VOID)
{
    // FIXME Implement
    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK;
#if (!CHIP_HAS_ACCO_PLL)
    hwp_sysCtrl->Sel_Clock |= SYS_CTRL_SYS_SEL_FAST_SLOW;
    hwp_sysCtrl->Pll_Ctrl &= (~SYS_CTRL_PLL_ENABLE_POWER_DOWN);
#else // (CHIP_HAS_ACCO_PLL)
    hwp_anaAccoFe->pll_ctrl |= ANA_ACCO_PLL_RST_LOCK;
    while(hwp_anaAccoFe->pll_ctrl & ANA_ACCO_PLL_LOCKED);
    hwp_anaAccoFe->pll_ctrl &= (~ANA_ACCO_PLL_PD_N);
#endif // (!CHIP_HAS_ACCO_PLL)
    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK;
}



// =============================================================================
// hal_SysEnablePLL
// -----------------------------------------------------------------------------
/// Enable PLL
// =============================================================================
PROTECTED VOID hal_SysEnablePLL(VOID)
{
    // FIXME Implement
    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK;
#if (!CHIP_HAS_ACCO_PLL)
    hwp_sysCtrl->Pll_Ctrl |= SYS_CTRL_PLL_ENABLE_ENABLE;
#else // (CHIP_HAS_ACCO_PLL)
    hwp_anaAccoFe->pll_ctrl &= ~ANA_ACCO_PLL_RST_LOCK; 
    hwp_anaAccoFe->pll_ctrl |= ANA_ACCO_PLL_PD_N;
#endif // (!CHIP_HAS_ACCO_PLL)
    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK;
}



// =============================================================================
// hal_SysGetPLLLock
// -----------------------------------------------------------------------------
/// Return PLL lock 
/// @return PLL status
///         If \c TRUE, PLL locked.
///         If \c FALSE, PLL not locked.
// =============================================================================
PROTECTED BOOL hal_SysGetPLLLock(VOID)
{
#if (!CHIP_HAS_ACCO_PLL)
    if(hwp_sysCtrl->Sel_Clock & SYS_CTRL_PLL_LOCKED)
#else // (CHIP_HAS_ACCO_PLL)
    if(hwp_anaAccoFe->pll_ctrl & ANA_ACCO_PLL_LOCKED)
#endif // (!CHIP_HAS_ACCO_PLL)
    {
        return TRUE;
    }
    return FALSE;
}



// =============================================================================
// hal_SysGetFrequencyConfiguration
// -----------------------------------------------------------------------------
/// This function returns the frequency configuration to apply to the system 
/// when required to set a given frequency. If the system cannot produce that
/// frequency, the closest available higher frequency will be applied.
/// The real frequency to apply is returned. The second pointer parameter will
/// store the bitfield value to use in the chip register.
///
/// @param fastClock Minimum frequency the system must output.
/// @param sysCtrlRegConfig Value to set in the sysCtrl bitfield to get the 
/// configured frequency.
/// @param enableCpuTurbo Pointer to a variable set to TRUE if, and only if,
/// the X-CPU clock doubler needs to be enabled at that frequency. This is
/// enabled only on chip supporting that feature.
/// @return The actual frequency that is going to be applied with that 
/// sysCtrl bitfield value.
// =============================================================================
#if (((CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE)) && !defined(FPGA))
PRIVATE  HAL_SYS_FREQ_T hal_SysGetFrequencyConfiguration(HAL_SYS_FREQ_T fastClock,
                                                         UINT32*        sysCtrlRegConfig,
                                                         BOOL*          enableCpuTurbo)
{
    HAL_SYS_FREQ_T realFreq = fastClock;
    *enableCpuTurbo = FALSE;
    switch(fastClock)
    {

        case HAL_SYS_FREQ_32K:
            // Not relevant, as we stay on the slow clock.
            // No frequency requested, fall back to 26MHz, 32kHz is only switched
            // in hal_LpsDeepSleep()
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            break;

        case HAL_SYS_FREQ_13M:
        case HAL_SYS_FREQ_26M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            break;

        case HAL_SYS_FREQ_39M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_39M;
            break;

        case HAL_SYS_FREQ_52M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_52M;
            break;

        case HAL_SYS_FREQ_78M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_78M;
            break;

        case HAL_SYS_FREQ_104M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_104M;
            break;

        case HAL_SYS_FREQ_156M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_156M;
            break;

        case HAL_SYS_FREQ_208M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_104M;
            realFreq = HAL_SYS_FREQ_104M;
            HAL_TRACE(HAL_WARN_TRC, 0, "HAL SYS: Requested Freq %d unsupported, fall back to %d",
                                        fastClock, realFreq);
            break;
        default:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            HAL_TRACE(HAL_WARN_TRC, 0, "HAL SYS: Requested Freq %d unsupported, fall back to %d",
                                        fastClock, realFreq);
    }
    return realFreq;
}

#elif (((CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE)) && defined(FPGA))
PRIVATE  HAL_SYS_FREQ_T hal_SysGetFrequencyConfiguration(HAL_SYS_FREQ_T fastClock,
                                                         UINT32*        sysCtrlRegConfig,
                                                         BOOL*          enableCpuTurbo)
{
    HAL_SYS_FREQ_T realFreq = fastClock;
    *enableCpuTurbo = FALSE;
    
    switch(fastClock)
    {
        case HAL_SYS_FREQ_32K:
            // Not relevant, as we stay on the slow clock.
            // No frequency requested, fall back to 26MHz, 32kHz is only switched
            // in hal_LpsDeepSleep()
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            break;

        case HAL_SYS_FREQ_13M:
        case HAL_SYS_FREQ_26M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            break;

        case HAL_SYS_FREQ_39M:
        case HAL_SYS_FREQ_52M:
        case HAL_SYS_FREQ_78M:
        case HAL_SYS_FREQ_104M:
        case HAL_SYS_FREQ_156M:
        case HAL_SYS_FREQ_208M:
#if (FPGA_SLOW == 1)
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            break;
#else
            *sysCtrlRegConfig = SYS_CTRL_FREQ_52M;
            realFreq = HAL_SYS_FREQ_52M;
            break;
#endif
        default:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            HAL_TRACE(HAL_WARN_TRC, 0, "HAL SYS: Requested Freq %d unsupported, fall back to %d",
                                        fastClock, realFreq);
    }

    //*sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
    //realFreq = HAL_SYS_FREQ_26M;

    *sysCtrlRegConfig = SYS_CTRL_FREQ_52M;
    realFreq = HAL_SYS_FREQ_52M;
 
      

    return realFreq;
}

#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE) 
PRIVATE  HAL_SYS_FREQ_T hal_SysGetFrequencyConfiguration(HAL_SYS_FREQ_T fastClock,
                                                         UINT32*        sysCtrlRegConfig,
                                                         BOOL*          enableCpuTurbo)
{
    HAL_SYS_FREQ_T realFreq = fastClock;
    *enableCpuTurbo = FALSE;
    switch(fastClock)
    {
        case HAL_SYS_FREQ_13M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_13M;
            break;
        case HAL_SYS_FREQ_26M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            break;
        case HAL_SYS_FREQ_39M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_39M;
            break;
        case HAL_SYS_FREQ_52M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_52M;
            break;
        case HAL_SYS_FREQ_78M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_78M;
            break;
#if (CHIP_MAX_SPEED == 104000000)
        case HAL_SYS_FREQ_104M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_104M;
            break;
        case HAL_SYS_FREQ_156M:
        case HAL_SYS_FREQ_208M:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_104M;
            HAL_TRACE(HAL_WARN_TRC, 0, "HAL SYS: Requested Freq %d unsupported, fall back to %d",
                                        fastClock, HAL_SYS_FREQ_104M);
            break;
#endif // CHIP_MAX_SPEED
        case HAL_SYS_FREQ_32K:
            // No frequency requested, fall back to 26MHz, 32kHz is only switched
            // in hal_LpsDeepSleep()
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            break;
        default:
            *sysCtrlRegConfig = SYS_CTRL_FREQ_26M;
            realFreq = HAL_SYS_FREQ_26M;
            HAL_TRACE(HAL_WARN_TRC, 0, "HAL SYS: Requested Freq %d unsupported, fall back to %d",
                                        fastClock, realFreq);
            break;
    }
    return realFreq;
}
#else // CHIP_ASIC_ID
#error "hal_SysGetFrequencyConfiguration not implemented for your architecture."
#endif // CHIP_ASIC_ID



// =============================================================================
// hal_SysSetupSystemClock
// -----------------------------------------------------------------------------
/// Configure the initial settings of the system clocks.
/// This function is to be called only by hal_init.
/// It also checks the validity of the public enum for HAL_SYS_FREQ_T and 
/// the internal one HAL_SYS_FREQ_SCAL_USERS_T use for frequency scaling.
/// @param fastClockSetup Initial System Clock.
// =============================================================================
PROTECTED VOID hal_SysSetupSystemClock(HAL_SYS_FREQ_T fastClockSetup)
{
    // FIXME CLAMP
    UINT32         anaClkCtrl;
    HAL_SYS_FREQ_T fastClock      = fastClockSetup;
    UINT32         sysFreq        = 0;
    BOOL           enableCpuTurbo = FALSE;
    UINT32         i;
  
    // Frequency clamping
    if (fastClock > HAL_SYS_MAX_SPEED)
    {
        HAL_TRACE(HAL_WARN_TRC, 0, "hal_SysSetupSystemClock clamped the request for a %d frequency. %d", fastClock, HAL_SYS_MAX_SPEED);
        fastClock = HAL_SYS_MAX_SPEED;
    }

    // Check the validity of the two enums
    // The HAL user Id are encroaching the system ones
    HAL_ASSERT(HAL_SYS_FREQ_SYS_LAST_USER<HAL_SYS_USER_START,
        "Bad value for the HAL_SYS_FREQ_USER_ID_T enum\n  "
        "The first public user one,%d, is less than the "
        "last hal private one,%d.",HAL_SYS_USER_START,HAL_SYS_FREQ_SYS_LAST_USER);

    HAL_ASSERT(HAL_SYS_TOTAL_USERS_QTY<=HAL_SYS_FREQ_MAX_RESOURCE_NB,
        "Bad value for the HAL_SYS_FREQ_USER_ID_T enum\n  "
        "The last public user one,%d, is greater than the "
        "total available number in hal %d.",HAL_SYS_TOTAL_USERS_QTY,HAL_SYS_FREQ_MAX_RESOURCE_NB);

    // Init the freq scal registries
    g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_32K ] = HAL_SYS_TOTAL_USERS_QTY ;
    g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_13M ] = 0;
    g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_26M ] = 0;
    g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_39M ] = 0;
    g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_52M ] = 0;
    g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_78M ] = 0;
    g_halNumberOfUserPerFreq[HAL_SYS_FREQ_SCAL_104M] = 0;

    // Init the freq request list
    for (i = 0; i < HAL_SYS_FREQ_MAX_RESOURCE_NB/4; i++)
    {
        g_halSysFreqScalRegistry[i] = 0;
    }

    // Get the possible clock configuration for the chip.
    // sysFreq is later used to be written in hwp_sysCtrl.
    fastClock = hal_SysGetFrequencyConfiguration(fastClock, &sysFreq, &enableCpuTurbo);

    if (g_halSysSystemFreq < fastClock)
    {
        hal_SysCallFreqChangeHandlers(fastClock);
    }
    // Do not set VCORE here for PMU is initialized after setting up sys clk
    
    {
        UINT32 status = hal_SysEnterCriticalSection();
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
// ---------------------------------------
        // setup fast clock
        if (fastClock == HAL_SYS_FREQ_32K)
        {
            hwp_extApb->ExtPll_Ctrl = REGS_EXTPLL_ENABLE_POWER_DOWN |
                                      REGS_EXTPLL_OUTPUT_ENABLE_DISABLE;
        } else
        {
// Only 39Mhz available through PLL on FPGA    
#ifdef FPGA
            if (fastClock == HAL_SYS_FREQ_26M)
            {
                hwp_extApb->ExtPll_Ctrl = REGS_EXTPLL_ENABLE_POWER_DOWN |
                                          REGS_EXTPLL_OUTPUT_ENABLE_DISABLE;
                // XXX FIXME
                // Dirty hack, really ?
                // start the PLL whatever to have an audio clock, but don't
                // switch on it !
                hwp_extApb->ExtPll_Config = REGS_EXTPLL_INPUT_DIVIDER(0)
                                        | REGS_EXTPLL_FEEDBACK_DIVIDER(22)
                                        | REGS_EXTPLL_OUTPUT_DIVIDER_DIV_BY_1;

                hwp_extApb->ExtPll_Ctrl = REGS_EXTPLL_ENABLE_ENABLE |
                                          REGS_EXTPLL_CLK_FAST_ENABLE_ENABLE |
                                          REGS_EXTPLL_OUTPUT_ENABLE_ENABLE;
            }
            else
            {
                hwp_extApb->ExtPll_Config = REGS_EXTPLL_INPUT_DIVIDER(0)
                                        | REGS_EXTPLL_FEEDBACK_DIVIDER(22)
                                        | REGS_EXTPLL_OUTPUT_DIVIDER_DIV_BY_1;

                hwp_extApb->ExtPll_Ctrl = REGS_EXTPLL_ENABLE_ENABLE |
                                          REGS_EXTPLL_CLK_FAST_ENABLE_ENABLE |
                                          REGS_EXTPLL_OUTPUT_ENABLE_ENABLE;
                hwp_sysCtrl->Cfg_Clk_Sys = sysFreq | SYS_CTRL_FORCE_DIV_UPDATE; 
            }
#else // NOT FPGA

            // No need to start the pll's already started in boot sector or ROM
            // Just verify both pll's are enabled...
            HAL_ASSERT(((hwp_extApb->ExtPll_Ctrl & REGS_EXTPLL_ENABLE_POWER_DOWN) == REGS_EXTPLL_ENABLE_ENABLE) && ((hwp_anaAccoFe->pll_ctrl & ANA_ACCO_PLL_PD_N) == ANA_ACCO_PLL_PD_N_ENABLE), "PLL are not properly enabled, UPGRADE YOUR BOOT SECTOR!");
            hwp_sysCtrl->Cfg_Clk_Sys = sysFreq | SYS_CTRL_FORCE_DIV_UPDATE;

#endif // FPGA
        }
    // switch to pll
#ifdef FPGA
        if (fastClock == HAL_SYS_FREQ_26M)
        {
            hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF |SYS_CTRL_SYS_SEL_FAST_SLOW;
        }
        else
        {
            hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF |SYS_CTRL_SYS_SEL_FAST_FAST;
        }
#else
        hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF
                            | SYS_CTRL_SYS_SEL_FAST_FAST
                            | g_sysCtrlSelPll;
                            //| SYS_CTRL_SYS_SEL_PLL_ACCO;
                            //| SYS_CTRL_SYS_SEL_PLL_EXT;
#endif




#elif ((CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE))
// ---------------------------------------
        // setup fast clock
        if (fastClock == HAL_SYS_FREQ_32K)
        {
            // Turning off the Pll and reset of the Lock detector
            hwp_sysCtrl->Pll_Ctrl =     SYS_CTRL_PLL_ENABLE_POWER_DOWN
                                    |   SYS_CTRL_PLL_BYPASS_PASS
                                    |   SYS_CTRL_PLL_CLK_FAST_ENABLE_DISABLE
                                    |   SYS_CTRL_PLL_LOCK_RESET_RESET;
        } else
        {
             // Turning off the Pll and reset of the Lock detector
            hwp_sysCtrl->Pll_Ctrl =     SYS_CTRL_PLL_ENABLE_POWER_DOWN
                                    |   SYS_CTRL_PLL_BYPASS_PASS
                                    |   SYS_CTRL_PLL_CLK_FAST_ENABLE_DISABLE
                                    |   SYS_CTRL_PLL_LOCK_RESET_RESET;

            // TODO: Implement. PLL settings through SPI
           
            hwp_sysCtrl->Pll_Ctrl =     SYS_CTRL_PLL_ENABLE_ENABLE
                                    |   SYS_CTRL_PLL_BYPASS_PASS
                                    |   SYS_CTRL_PLL_CLK_FAST_ENABLE_DISABLE
                                    |   SYS_CTRL_PLL_LOCK_RESET_NO_RESET;
            // Wait for lock.
            while(!(hwp_sysCtrl->Sel_Clock & SYS_CTRL_PLL_LOCKED));

            // Allow clock through
            hwp_sysCtrl->Pll_Ctrl |= SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE;

            hwp_sysCtrl->Cfg_Clk_Sys = sysFreq | SYS_CTRL_FORCE_DIV_UPDATE; 
        }
        // switch to pll
#ifdef FPGA
        // Only 52Mhz available through PLL on FPGA:
        // - 26 MHz is done with the RF clock.
        // - 52 Mhz is done with the Pll.
        if (fastClock == HAL_SYS_FREQ_26M)
        {
            hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF | SYS_CTRL_SYS_SEL_FAST_SLOW;
        }
        else
        {
            hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF | SYS_CTRL_SYS_SEL_FAST_FAST;
        }
#else //FPGA
        hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF | SYS_CTRL_SYS_SEL_FAST_FAST;
#endif
#else
#error "hal_SysSetupSystemClock not implemented for this Chip"
#endif // CHIP_ASIC_ID

        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
        hal_SysExitCriticalSection(status);
    }

#if (!CHIP_HAS_ACCO_PLL)
    anaClkCtrl = 0;
    // setup gsm clocks: (TCU clock)
    hwp_tcu->Cfg_Clk_Div= TCU_ENABLE_QBIT_ENABLED;
#else // (CHIP_HAS_ACCO_PLL)
    // setup gsm clocks: (TCU clock)
    hwp_tcu->Cfg_Clk_Div= TCU_ENABLE_QBIT_ENABLED;

    // Setup analog clocks:
    anaClkCtrl = hwp_anaAccoFe->ana_clk_ctrl & ~( ANA_ACCO_GPADC_CLK_DIV_MASK| ANA_ACCO_AFC_CLK_DIV_MASK);
    
    hwp_anaAccoFe->ana_clk_ctrl = anaClkCtrl | ANA_CLOCKS_POLARITY |
                                  ANA_ACCO_AFC_CLK_DIV(AFC_DIVIDER_SETTING) |
                                  ANA_ACCO_GPADC_CLK_DIV(GPADC_DIVIDER_SETTING);
#endif // (!CHIP_HAS_ACCO_PLL)

    if (g_halSysSystemFreq > fastClock)
    {  // when going to a faster clock need to setup everything first
        hal_SysCallFreqChangeHandlers(fastClock);
    }
    // Do not set VCORE here for PMU is initialized after setting up sys clk

    // update global variables
    g_halSysSystemFreq = fastClock;
}



// =============================================================================
// hal_SysChangeSystemClock
// -----------------------------------------------------------------------------
/// Change the system clock (previously started and configured) to a (possibly)
/// new value
/// @param fastClockSetup New System clock applied to the system.
// =============================================================================
PRIVATE VOID hal_SysChangeSystemClock(HAL_SYS_FREQ_T fastClockSetup)
{
#ifndef SIMU
    HAL_SYS_FREQ_T fastClock;

    // assert setup already done (pll on / sys is fast)
    UINT32  sysFreq;
    BOOL    enableCpuTurbo;

    // Get the possible clock configuration for the chip.
    // sysFreq is later used to be written in hwp_sysCtrl.
    fastClock = hal_SysGetFrequencyConfiguration(fastClockSetup, &sysFreq, &enableCpuTurbo);

    // If the newly asked frequency is the same as before,
    // nevermind ...
    if (g_halSysSystemFreq == fastClock)
    {
        return;
    }

    // Check Wether LPS allows us to change the system frequency.
    // If we are lock by the LPS, we do not change the frequency here to keep 
    // synch'ed.
    // The LPS calibration depends on the system frequency, 
    // this function will invalidate it if needed.
    if (!hal_LpsClockSysChangeAllowed(fastClock))
    {
        return;
    }

    if (g_halSysSystemFreq < fastClock)
    {
        hal_SysCallFreqChangeHandlers(fastClock);
    }

    {
        UINT32 status = hal_SysEnterCriticalSection();
        UINT32 oldSysFreq = hwp_sysCtrl->Cfg_Clk_Sys & SYS_CTRL_FREQ_MASK;
        
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;

#if (!CHIP_HAS_ASYNC_TCU)
        // Accumulate the QBit error induced by the clock switch
#if (CHIP_HAS_ACCO_PLL)
        if (g_sysCtrlSelPll == SYS_CTRL_SYS_SEL_PLL_ACCO)
        {
            g_halSysClockSwitchQBitError += g_halSysClockSwitchQBitErrorCompensate156[sysFreq-SYS_CTRL_FREQ_26M][oldSysFreq-SYS_CTRL_FREQ_26M];
        }
        else
#endif // (CHIP_HAS_ACCO_PLL)
        {
            g_halSysClockSwitchQBitError += g_halSysClockSwitchQBitErrorCompensate312[sysFreq-SYS_CTRL_FREQ_26M][oldSysFreq-SYS_CTRL_FREQ_26M];
        }
#endif // (!CHIP_HAS_ASYNC_TCU)

        // Configure System Clock.
        hwp_sysCtrl->Cfg_Clk_Sys= SYS_CTRL_FREQ(sysFreq);

#ifdef FPGA
        // For FPGA, up to now, 26 Mhz can only be done
        // through the RF clock.
        if (fastClock == HAL_SYS_FREQ_26M)
        {
            hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF |SYS_CTRL_SYS_SEL_FAST_SLOW;
        }
        else
        {
#if (!CHIP_HAS_ACCO_PLL)
            hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF | SYS_CTRL_SYS_SEL_FAST_FAST;
#else // (CHIP_HAS_ACCO_PLL)
            hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF |SYS_CTRL_SYS_SEL_FAST_FAST| SYS_CTRL_SYS_PLL_SELECTED_EXT;
#endif // (!CHIP_HAS_ACCO_PLL)
        }
#endif // FPGA

        // Configure CPU clock.
#if (CHIP_HAS_CPU_CLOCK_DOUBLER == 1)
        // Double CPU clock if needed.
        if (enableCpuTurbo)
        {
            hwp_sysCtrl->Xcpu_Clock_Doubler = SYS_CTRL_XCPU_DOUBLE_CLK_EN_DOUBLE_CLK_ENABLE;
        }
        else
        {
            hwp_sysCtrl->Xcpu_Clock_Doubler = SYS_CTRL_XCPU_DOUBLE_CLK_EN_DOUBLE_CLK_DISABLE;
        }
#endif
        
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
        
        hal_SysExitCriticalSection(status);
    }

    if (g_halSysSystemFreq > fastClock)
    {
        hal_SysCallFreqChangeHandlers(fastClock);
    }

#ifdef HAL_CLK_PROFILING

        switch(g_halSysSystemFreq)
        {
            case HAL_SYS_FREQ_32K  : HAL_PROFILE_WINDOW_EXIT(SYS_CLK_IS_32K); break; 
            case HAL_SYS_FREQ_13M  : HAL_PROFILE_WINDOW_EXIT(SYS_CLK_IS_13M); break; 
            case HAL_SYS_FREQ_26M  : HAL_PROFILE_WINDOW_EXIT(SYS_CLK_IS_26M); break; 
            case HAL_SYS_FREQ_39M  : HAL_PROFILE_WINDOW_EXIT(SYS_CLK_IS_39M); break; 
            case HAL_SYS_FREQ_52M  : HAL_PROFILE_WINDOW_EXIT(SYS_CLK_IS_52M); break; 
            case HAL_SYS_FREQ_104M : HAL_PROFILE_WINDOW_EXIT(SYS_CLK_IS_104M); break;
            default                : HAL_PROFILE_WINDOW_EXIT(SYS_CLK_IS_OTHER); break;
        }

        switch(fastClock)
        {
            case HAL_SYS_FREQ_32K  : HAL_PROFILE_WINDOW_ENTER(SYS_CLK_IS_32K); break; 
            case HAL_SYS_FREQ_13M  : HAL_PROFILE_WINDOW_ENTER(SYS_CLK_IS_13M); break; 
            case HAL_SYS_FREQ_26M  : HAL_PROFILE_WINDOW_ENTER(SYS_CLK_IS_26M); break; 
            case HAL_SYS_FREQ_39M  : HAL_PROFILE_WINDOW_ENTER(SYS_CLK_IS_39M); break; 
            case HAL_SYS_FREQ_52M  : HAL_PROFILE_WINDOW_ENTER(SYS_CLK_IS_52M); break; 
            case HAL_SYS_FREQ_104M : HAL_PROFILE_WINDOW_ENTER(SYS_CLK_IS_104M); break;
            default                : HAL_PROFILE_WINDOW_ENTER(SYS_CLK_IS_OTHER); break;
        }

#endif // HAL_CLK_PROFILING

    g_halSysSystemFreq = fastClock;

#endif    
}




// =============================================================================
// hal_SysResetOut
// -----------------------------------------------------------------------------
/// A call to this function resets an external device through the reset-out
/// line, if the parameter is \c TRUE.
/// @param resetOut If \c TRUE, rises the reset-out line.
///                 If \c FALSE, pulls it down.
// =============================================================================
PUBLIC VOID hal_SysResetOut(BOOL resetOut)
{
    if (resetOut == FALSE)
    {
        hwp_sysCtrl->Sys_Rst_Clr = SYS_CTRL_CLR_RST_OUT;
    }
    else
    {
        UINT32 status = hal_SysEnterCriticalSection();
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
        hwp_sysCtrl->Sys_Rst_Set = SYS_CTRL_SET_RST_OUT;
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
        hal_SysExitCriticalSection(status);
    }
}



// =============================================================================
// hal_SysInvalidateCache
// -----------------------------------------------------------------------------
/// This function invalidates in the cache the line corresponding to the given 
/// memory area, referred by a pointer and a size.
/// It must be called with the destination buffer after a DMA copy.
/// @param buffer Pointer to the first address whose cache line must be 
/// invalidated
/// @param size Size of the buffer.
// =============================================================================
PUBLIC VOID hal_SysInvalidateCache(VOID* buffer, UINT32 size)
{
    if (size > 512)
    {
        // flush cache
        hal_GdbFlushCacheData();
    } else {
        // do a memcpy from uncached to cached to insure coherency of cache
        memcpy( buffer, // destination
                (VOID*) HAL_SYS_GET_UNCACHED_ADDR((UINT32)buffer), // source
                size); // size
    }
}



// =============================================================================
// hal_SysGetChipVersion
// -----------------------------------------------------------------------------
/// That function gives the version of the chip the software is running on.
/// @return The version of the chip.
// =============================================================================
PUBLIC UINT32 hal_SysGetChipVersion(VOID)
{

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    UINT32 tmp = hwp_extApb->CHIP_ID;
#elif ((CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE))
    UINT32 tmp = hwp_configRegs->CHIP_ID;
#else
#error "hal_SysGetChipVersion not implemented for this chip"
#endif
    return tmp; 
}


// =============================================================================
// hal_SysClkOut
// -----------------------------------------------------------------------------
/// Manage the output of the clock-out, to an external device
///
/// This function requests a resource of #HAL_SYS_FREQ_26M or release it 
/// to #HAL_SYS_FREQ_32K depending on the \p freq parameter value.
///
/// @param freq the required frequency or #HAL_SYS_CLOCK_OUT_OFF
// =============================================================================
PRIVATE VOID hal_SysClkOut(HAL_SYS_CLOCK_OUT_FREQ_T freq)
{

    if (freq == HAL_SYS_CLOCK_OUT_FREQ_26M)
    {
        // Need the RF clock
        hal_SysRequestFreq(HAL_SYS_FREQ_CLK_OUT, HAL_SYS_FREQ_26M, NULL);
    }
    {
        UINT32 status = hal_SysEnterCriticalSection();
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
        UINT32 myreg;
        myreg = hwp_sysCtrl->Sel_Clock;
        myreg &= ~SYS_CTRL_CLOCKOUT_SEL_RF_MASK;
        if (freq == HAL_SYS_CLOCK_OUT_FREQ_26M)
        {
            myreg |= SYS_CTRL_CLOCKOUT_SEL_RF_RF;
        }
        else
        {
            myreg |= SYS_CTRL_CLOCKOUT_SEL_RF_OSCILLATOR;
        }
        hwp_sysCtrl->Sel_Clock = myreg;
#elif ((CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE))
        // TODO Implement the divider
        // management.
        if (freq == HAL_SYS_CLOCK_OUT_FREQ_26M)
        {
            hwp_sysCtrl->Cfg_Clk_Out = SYS_CTRL_CLKOUT_SEL_RF;
        }
        else
        {
            hwp_sysCtrl->Cfg_Clk_Out = SYS_CTRL_CLKOUT_SEL_OSC;
        }
#else
#error "hal_SysClkOut not implemented for this Chip"
#endif

        if (freq != HAL_SYS_CLOCK_OUT_OFF)
        {
            hwp_sysCtrl->Clk_Other_Enable = SYS_CTRL_ENABLE_OC_CLK_OUT;
        }
        else
        {
            hwp_sysCtrl->Clk_Other_Disable = SYS_CTRL_DISABLE_OC_CLK_OUT;
        }
        hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
        hal_SysExitCriticalSection(status);
    }
        
    if (freq != HAL_SYS_CLOCK_OUT_FREQ_26M)
    {
        // Free the resource
        hal_SysRequestFreq(HAL_SYS_FREQ_CLK_OUT, HAL_SYS_FREQ_32K, NULL);
    }
}


// =============================================================================
// hal_SysClkOutOpen
// -----------------------------------------------------------------------------
/// Manage the output of the clock-out, to an external device
///
/// This function requests a resource of #HAL_SYS_FREQ_26M or not
/// (keep #HAL_SYS_FREQ_32K) depending on the \p freq parameter value.
///
/// @param freq the required frequency
/// @return the user id to be used for #hal_SysClkOutClose
///         or #HAL_SYS_CLOCK_OUT_RESOURCE_UNAVAILABLE when the clock is already
///         in use at a different frequency.
// =============================================================================
PUBLIC HAL_SYS_CLOCK_OUT_ID_T hal_SysClkOutOpen(HAL_SYS_CLOCK_OUT_FREQ_T freq)
{
    INT32 id = HAL_SYS_CLOCK_OUT_RESOURCE_UNAVAILABLE;
    INT32 loopId;
    UINT8 curFreq=HAL_SYS_CLOCK_OUT_OFF;
    UINT32 status;

    HAL_ASSERT(((freq <= HAL_SYS_CLOCK_OUT_FREQ_26M) 
                && ( freq >= HAL_SYS_CLOCK_OUT_FREQ_32K))
               , "Clock Out Frequency out of range: %d", freq);

    status = hal_SysEnterCriticalSection();

    for(loopId = 0; loopId < HAL_SYS_CLOCK_OUT_ID_QTY; loopId ++)
    {
        // empty spot?
        if (HAL_SYS_CLOCK_OUT_OFF == g_halSysClockOutRegistry[loopId])
        {
            id = loopId;
        }
        else
        {
            curFreq = g_halSysClockOutRegistry[loopId];
        }
    }
    // no more spot
    if (HAL_SYS_CLOCK_OUT_RESOURCE_UNAVAILABLE == id)
    {
        hal_SysExitCriticalSection(status);
        return HAL_SYS_CLOCK_OUT_RESOURCE_UNAVAILABLE;
    }
    // no freqency yet or same freq, ok open it !
    if ((HAL_SYS_CLOCK_OUT_OFF == curFreq) || (freq == curFreq))
    {
        g_halSysClockOutRegistry[id] = freq;
        if (HAL_SYS_CLOCK_OUT_OFF == curFreq)
        {
            hal_SysClkOut(freq);
        }
        hal_SysExitCriticalSection(status);
        return id;
    }
    // else an other frequency is already set
    hal_SysExitCriticalSection(status);
    return HAL_SYS_CLOCK_OUT_RESOURCE_UNAVAILABLE;
}



// =============================================================================
// hal_SysClkOutClose
// -----------------------------------------------------------------------------
/// Manage the output of the clock-out, to an external device
///
/// This function release a resource to #HAL_SYS_FREQ_32K if no more 
/// Clock Out user needs it.
///
/// @param id the user id returned by #hal_SysClkOutOpen
// =============================================================================
PUBLIC VOID hal_SysClkOutClose(HAL_SYS_CLOCK_OUT_ID_T id)
{
    INT32 loopId;
    UINT8 curFreq=HAL_SYS_CLOCK_OUT_OFF;
    UINT32 status;
    HAL_ASSERT(((id < HAL_SYS_CLOCK_OUT_ID_QTY) 
                && (id >= HAL_SYS_CLOCK_OUT_ID_1))
               , "Clock Out Id out of range: %d", id);
    HAL_ASSERT(g_halSysClockOutRegistry[id] != HAL_SYS_CLOCK_OUT_OFF, "Invalid clock out Id (not opened): %d", id);
    status = hal_SysEnterCriticalSection();
    // clear request
    g_halSysClockOutRegistry[id] = HAL_SYS_CLOCK_OUT_OFF;
    for(loopId = 0; loopId < HAL_SYS_CLOCK_OUT_ID_QTY; loopId ++)
    {
        // freq request?
        if (HAL_SYS_CLOCK_OUT_OFF != g_halSysClockOutRegistry[loopId])
        {
            curFreq = g_halSysClockOutRegistry[loopId];
            // one found, stop all must match or be off
            hal_SysExitCriticalSection(status);
            return ;
        }
    }
    // No clock out needed.
    hal_SysClkOut(HAL_SYS_CLOCK_OUT_OFF);
    hal_SysExitCriticalSection(status);
}



// ==========================================================================
// hal_SysAuxClkOut
// --------------------------------------------------------------------------
/// Manage the output of the 26M AUX Clock, to an external device
/// @param enable If \c TRUE, enable AuxClk output.
///               If \c FALSE, disable AuxClk output.
// ==========================================================================
PUBLIC VOID hal_SysAuxClkOut(BOOL enable)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
    UINT32 status = hal_SysEnterCriticalSection();
    if (enable)
    {
        if (g_halSysAuxClkUserNum == 0)
        {
            hwp_sysCtrl->Cfg_Clk_Auxclk = SYS_CTRL_AUXCLK_EN_ENABLE;
        }
        g_halSysAuxClkUserNum++;
    }
    else
    {
        if (g_halSysAuxClkUserNum > 0)
        {
            g_halSysAuxClkUserNum--;
            if (g_halSysAuxClkUserNum == 0)
            {
                hwp_sysCtrl->Cfg_Clk_Auxclk = SYS_CTRL_AUXCLK_EN_DISABLE;
            }
        }
    }
    hal_SysExitCriticalSection(status);
#endif // (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
}


// =============================================================================
// hal_SysIrqPending
// -----------------------------------------------------------------------------
/// This function checks the IRQ pending status. It should be called in critical
/// section to know if some IRQ are pending and then decide to release the 
/// critical section.
/// @return \c TRUE if IRQ are pending.
///         \c FALSE Otherwise.
// =============================================================================
PUBLIC BOOL hal_SysIrqPending(VOID)
{
    if (hwp_sysIrq->Cause != 0)
    {
        // IRQ are pending
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}



// =============================================================================
// hal_SysGetResetCause
// -----------------------------------------------------------------------------
/// Get the cause of the last reset.
/// This function only returns the reset cause. It does not proceed to any test
/// to now if the boot was due to a press on the Power On button or because
/// the charger was plugged. Those tests are to be done by the user code, 
/// when this function returns HAL_SYS_RESET_CAUSE_ALARM.
/// @return The last reset cause
// =============================================================================
PUBLIC HAL_SYS_RESET_CAUSE_T hal_SysGetResetCause(VOID)
{
    if (g_bootResetCause & SYS_CTRL_WATCHDOG_RESET_HAPPENED)
    {
        return HAL_SYS_RESET_CAUSE_WATCHDOG;
    }

    if (g_bootResetCause & SYS_CTRL_GLOBALSOFT_RESET_HAPPENED)
    {
        return HAL_SYS_RESET_CAUSE_RESTART;
    }

    if (g_bootResetCause & SYS_CTRL_HOSTDEBUG_RESET_HAPPENED)
    {
        return HAL_SYS_RESET_CAUSE_HOST_DEBUG;
    }

    if (g_bootResetCause & SYS_CTRL_ALARMCAUSE_HAPPENED)
    {
        return HAL_SYS_RESET_CAUSE_ALARM;
    }

    // else
    return HAL_SYS_RESET_CAUSE_NORMAL;

}



// =============================================================================
// hal_SysUsbHostEnable
// -----------------------------------------------------------------------------
/// Enable or disable usb host.
/// This is useful to inform hal_SysProcessIdle() that the USB Host functions
/// must be called to process the host commands.
/// @param enable \c TRUE to enable, \c FALSE to disable.
// =============================================================================
PROTECTED VOID hal_SysUsbHostEnable(BOOL enable)
{
#if (CHIP_HAS_USB == 1)
    g_halUsbHostEnable = enable;
#else
    enable             = enable;
#endif/* CHIP_HAS_USB */
}
// =============================================================================
// hal_SysProcessIdle
// -----------------------------------------------------------------------------
/// This function is called in the idle task. 
/// It is used to execute in idle the function processing  possible commands
/// from the Host, and thus, for example, call the executor...
// =============================================================================
PUBLIC VOID hal_SysProcessIdle(VOID)
{
    // Process commands from host
#if (CHIP_HAS_USB == 1)
    if(g_halUsbHostEnable)
    {
#if (HAL_USB_DRIVER_DONT_USE_ROMED_CODE == 1)
        hal_HostUsbRecv();
        hal_HostUsbSend();
#else
        boot_HostUsbRecv();
        boot_HostUsbSend();
#endif // HAL_USB_DRIVER_DONT_USE_ROMED_CODE == 1

    }
#endif

    hal_HstRamClientIdle();
}


#if (!CHIP_HAS_ASYNC_TCU)
// =============================================================================
// hal_SysQBitError
// -----------------------------------------------------------------------------
/// This function returns error on QBit due to clock switch mechanism.
/// If the error has to be compensated the error is cleared.
/// @param forceReset force to clear the error counter.
/// @return -1, 0 or 1 QBit error to be compensated.
// =============================================================================
PUBLIC INT32 hal_SysQBitError(BOOL forceReset)
{
    UINT32 status = hal_SysEnterCriticalSection();

    // return value
    INT32 returnVal = (g_halSysClockSwitchQBitError >=  288) ?  1 :
                      (g_halSysClockSwitchQBitError <= -288) ? -1 :
                      0;

    // update g_halSysClockSwitchQBitError
    if (forceReset)
    {
        // force reset
        // TODO : should we reset when the error is too big?
       g_halSysClockSwitchQBitError = 0;
    }
    else 
    {
        g_halSysClockSwitchQBitError -= (288*returnVal);
    }

    hal_SysExitCriticalSection(status);
    return returnVal;
}
#endif // (!CHIP_HAS_ASYNC_TCU)



PUBLIC VOLATILE UINT32* HAL_DATA_INTERNAL g_halSysCsAddress = (UINT32*)&hwp_sysIrq->SC;


