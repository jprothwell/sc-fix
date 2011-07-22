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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/hal_open.c $ //
//    $Author: huazeng $                                                        // 
//    $Date: 2011-07-12 11:53:33 +0800 (Tue, 12 Jul 2011) $                     //   
//    $Revision: 8975 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
///     @file   hal_open.c 
///     Implementation of the HAL layer initialization.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#include "cs_types.h"

#include "chip_id.h"

#include "global_macros.h"
#include "page_spy.h"
#include "sys_ctrl.h"
#include "sys_irq.h"
#include "cfg_regs.h"

#if ((CHIP_HAS_ACCO_PLL) || (CHIP_HAS_ACCO_AUDIO))
#include "ana_acco.h"
#endif

#if (CHIP_HAS_ASYNC_BCPU)
#include "bb_ctrl.h"
#endif

#include "tcu.h"

#include "tgt_m.h"
#include "tgt_hal_cfg.h"
#include "tgt_pmd_cfg.h"

#include "pmd_m.h"
#include "calib_m.h"

#include "hal_rfspi.h"
#include "hal_timers.h"
#include "hal_ana_gpadc.h"
#include "halp_ana_audio.h"
#include "hal_debug.h"
#include "hal_pwm.h"
#include "hal_mem_map.h"
#include "hal_gpio.h"
#include "hal_sys.h"
#include "hal_config.h"
#include "hal_map_engine.h"
#include "hal_speech.h"
#include "hal_usb.h"
#include "hal_camera.h"
#include "hal_ispi.h"
#include "hal_rda_abb.h"

#include "halp_irq.h"
#include "halp_page_spy.h"
#include "halp_sys.h"
#include "halp_sys_ifc.h"
#include "halp_timers.h"
#include "halp_calib.h"
#include "halp_config.h"
#include "halp_ana_gpadc.h"
#include "halp_debug.h"
#include "halp_speech.h"
#include "halp_ebc.h"
#include "halp_proxy_memory.h"
#include "halp_profile.h"

#include "boot_usb_monitor.h"
#include "halp_usb_monitor.h"


#include "patch_m.h"

#include "stdio.h"


#if (ASICSIMU == 1)
#include "asicsimu_m.h"
#endif

#include "pal_gsm.h"

// =============================================================================
//  MACROS
// =============================================================================

#if defined(_FLASH_PROGRAMMER) || defined(_T_UPGRADE_PROGRAMMER)
#undef EARLY_TRACE_OPEN
#endif


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

// =============================================================================
// g_halCalibration
// -----------------------------------------------------------------------------
/// Since the calibration is managed by the Calib module, we need to use
/// an accessor function to get the pointer to the calibration structure.
/// To avoid to call that function numerous times, the g_halCalibration is
/// used as a pointer to the calibration structure, and is initialized by
/// hal_Open. g_halCalibration can be used by all driver of HAL.
// =============================================================================
PROTECTED CALIB_CALIBRATION_T*  g_halCalibration    
     = NULL;



#if (PAGE_SPY_LOG == 1)
// In 32 bits words, we keep the last 100 writes plus a stamp
#define SP_BUFFER_SIZE  (4*2048)



// =============================================================================
// g_SpBuffer
// -----------------------------------------------------------------------------
/// Page spy writes buffer
// =============================================================================
PROTECTED UINT32 g_SpBuffer[SP_BUFFER_SIZE];


// =============================================================================
// g_SpBufferTop
// -----------------------------------------------------------------------------
/// Pointer to the top position in the Page spy writes buffer
// =============================================================================
extern UINT32* g_SpBufferTop;


// =============================================================================
// g_SpBufferPtr
// -----------------------------------------------------------------------------
/// Pointer to the current position in the Page spy writes buffer
// =============================================================================
extern UINT32* g_SpBufferPtr;
#endif

#if (PAGE_SPY_AT_ZERO == 1)
// placeholder for protecting arrea at the beginning of ram (addr 0 and above)
PROTECTED UINT32 g_SpZero[256] __attribute__((section (".ram")));
#endif

// =============================================================================
//  FUNCTIONS
// =============================================================================

// =============================================================================
// hal_Open
// -----------------------------------------------------------------------------
/// Initializes the HAL layer. 
/// This function also do the open of the PMD driver and set the idle-mode.
// =============================================================================
PUBLIC VOID hal_Open(CONST HAL_CFG_CONFIG_T* halCfg)
{
#ifdef EARLY_TRACE_OPEN
    // Enable trace before OS is ready, for the convenience of debugging in hal_Open().
    extern void sxs_HostTraceEarlyOpen (void);
    sxs_HostTraceEarlyOpen ();
#endif // EARLY_TRACE_OPEN

    // Prevent from going into test mode
    // through the test mode pin.

    // FIX for stereo DAC pop noise
    // todo as soon as possible
#if (CHIP_HAS_STEREO_DAC == 1)
// will hopefully be fixed on Gallite
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    hal_AnaAudioStereoDacPreOpen();
#endif
#endif

    // Checkers to ensures that defines used to publicly access registers are
    // coherent with the register map.
    HAL_ASSERT((UINT32)&(hwp_sysIrq->SC) == HAL_SYS_CRITICAL_SECTION_ADDRESS,
            "The critical section public access is not up to date with"
            "the memory map. Edit hal_sys.h and update "
            "HAL_SYS_CRITICAL_SECTION_ADDRESS.%08X %08X",
            (UINT32)&(hwp_sysIrq->SC),
            HAL_SYS_CRITICAL_SECTION_ADDRESS);

    // Install all patches
    patch_Open();

    // Initialize the map engine.
    hal_MapEngineOpen();

#if (CHIP_HAS_USB == 1)
    // Avoid crashing when calling hal_UsbClose() without hal_UsbOpen() before.
    boot_UsbInitVar();
#endif // (CHIP_HAS_USB == 1)

#if defined(HAL_PROFILE_NOT_ON_ROMULATOR) || defined(HAL_PROFILE_ON_BUFFER)
    // Configure PXTS to enable record in RAM
    hal_DbgPxtsOpen();
#endif// defined(HAL_PROFILE_NOT_ON_ROMULATOR)

    // Initialize the System IFC.
    hal_IfcOpen();

#if (CHIP_HAS_PROXY_MEMORY == 1)
    // Disable access to the proxy memory from the external
    // bus. (ie. Enable the access at cpu speed)
    hal_PMemEnableExternalAccess(FALSE);
#endif

    // Setup the debug mode pins configured by IOMUX
    // as debug output
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    // TODO This registers holds a lot of config
    // done in hal_BoardSetup
    hwp_extApb->Alt_mux_select = REGS_DEBUG_PORT_DEBUG;
#endif

    // Unlock following registers.
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    // cache ram automatic disable mode
    hwp_sysCtrl->Cfg_Cpus_Cache_Ram_Disable|=
                 SYS_CTRL_XCPU_CACHE_RAM_DISABLE(1)
               | SYS_CTRL_BCPU_CACHE_RAM_DISABLE(1);
    // automatic clock gating enable
    hwp_sysCtrl->Clk_Sys_Mode = 0;
    hwp_sysCtrl->Clk_BB_Mode = 0;
    hwp_sysCtrl->Clk_Other_Mode = SYS_CTRL_MODE_OC_VOC_CORE_MANUAL;
    hwp_sysCtrl->Clk_Other_Disable = SYS_CTRL_DISABLE_OC_VOC_CORE;
#elif ((CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE))
    // cache ram automatic disable mode
    hwp_sysCtrl->Cfg_Cpus_Cache_Ram_Disable|=
                 SYS_CTRL_XCPU_CACHE_RAM_DISABLE(1)
               | SYS_CTRL_BCPU_CACHE_RAM_DISABLE(1);
    // automatic clock gating enable
    hwp_sysCtrl->Clk_Sys_Mode = 0;
    hwp_sysCtrl->Clk_BB_Mode = 0;
    hwp_sysCtrl->Clk_Other_Mode = 0;
#else
#error "No implementation for this CHIP_ASIC_ID"
#endif

    hwp_sysCtrl->Clk_Per_Mode = 0;
    
#if (CHIP_HAS_ACCO_PLL)
    // automatic clock gating enable for analog
    // WARNING also destroys clock polarity (non-inverted)
    // and AFC divider, but they will be set by hal_SysSetupSystemClock
    // AFC clock needs to be manual.
    hwp_anaAccoFe->ana_clk_ctrl = ANA_ACCO_AFC_CLK_EN_MODE
                                | ANA_ACCO_PA_CLK_1M08_EN_MODE;
#endif
    
    // Lock registers.
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;

    // Set the board specific configuration, depending on 
    // the target.
    hal_BoardSetup(halCfg);

    // for asic simulation configure the print after the board setup is done
#if (ASICSIMU == 1)
    asicsimu_Open();
#endif

// ********************
//   Calibration Init  
// ********************

// This function check if the platform has been calibrated. If not,
// it uses the default calibration parameters that have been written
// in the Flash, with the code. If the platform has been calibrated,
// it uses the values stored in the calibration dedicated Flash sector. 
        
// If we are in PAL_CALIB mode, the global calibration buffer will 
// be stored in Ram. Jade Debug Server or the CalibTool will be able to
// write in an other buffer and swap it with the global calibration
// buffer. After the calibration is done one of these buffer will be
// burnt in Flash, in the calibration dedicated Flash sector.
    calib_DaemonInit();
    
    /// Init HAL internal pointer to the calibration structure.
    g_halCalibration = calib_GetPointers();
    
#ifndef EXTERNAL_ANALOG
//  Calibration of the bandgap. 
    hal_AnaGpadcCalibBandgap();
#endif
    // (Un)Initialize the battery voltage value to 0xffff
    // to indicate that the first measurement hasn't been done yet
    
    // FIXME implement
    //hal_GpadcSetBatVolt(GPADC_BAT_VOLT_UNINIT);

    // Set the sys clock, and tcu clock, and analog clocks
    // In FPGA, the frequency doesn't change after the initial setup
    // (hal_SysChangeSystemClock is empty)
    // With the Chip, the frequency is set according to the users
    // need. For example, PAL asks for at least 39 Mhz.
#ifdef FPGA
#ifndef FPGA_SLOW
    hal_SysSetupSystemClock(HAL_SYS_FREQ_39M);
#else
    hal_SysSetupSystemClock(HAL_SYS_FREQ_26M);
#endif
#else
    hal_SysSetupSystemClock(HAL_SYS_FREQ_26M);
#endif

    // ??? No effect unless PLL is lock ???
    // Set the Membridge clock
    // FIXME: should be done by a call to FlashOpen
    // before hal_Open
#ifdef RAM_CLK_IS_52M
    hal_EbcCsSetup(HAL_SYS_FREQ_52M);
#else
#ifdef RAM_CLK_IS_78M
    hal_EbcCsSetup(HAL_SYS_FREQ_78M);
#else
#ifdef RAM_CLK_IS_104M
    hal_EbcCsSetup(HAL_SYS_FREQ_104M);
#else
    hal_EbcCsSetup(HAL_SYS_FREQ_52M); // EBC use 52M clk in default.
#endif//RAM_CLK_IS_104M
#endif//RAM_CLK_IS_78M
#endif//RAM_CLK_IS_52M

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
#ifdef FPGA
    hwp_sysCtrl->Cfg_Clk_Voc = SYS_CTRL_VOC_SEL_PLL_PLL | SYS_CTRL_FREQ_13M;
#else
    hwp_sysCtrl->Cfg_Clk_Voc = SYS_CTRL_VOC_SEL_PLL_PLL | SYS_CTRL_FREQ_52M;
#endif
#elif ((CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE))
#ifdef FPGA
    hwp_sysCtrl->Cfg_Clk_Voc = SYS_CTRL_VOC_SEL_PLL_PLL | SYS_CTRL_VOC_FREQ_13M;
#else
#ifdef USE_VOC_IS_78M
    hwp_sysCtrl->Cfg_Clk_Voc = SYS_CTRL_VOC_SEL_PLL_PLL | SYS_CTRL_VOC_FREQ_78M;
#else
    hwp_sysCtrl->Cfg_Clk_Voc = SYS_CTRL_VOC_SEL_PLL_PLL | SYS_CTRL_VOC_FREQ_52M;
#endif
#endif
#endif // CHIP_ASIC_ID

    // By default, turn the CLK_OUT   *** OFF ***    The clock can be reconfigured
    // i.e. set to 32k or 26M / enabled or disabled later.
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
    hwp_sysCtrl->Clk_Other_Disable = SYS_CTRL_DISABLE_OC_CLK_OUT;
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;


    // OS timer init. 
    hal_TimTickOpen();

//    hal_TimWatchDogOpen(5*HAL_TICK1S); // 5s default watchdog value

    // Enable and configure the page spies. 
    hal_DbgPageProtectSetup (HAL_DBG_PAGE_IRQ_STACKWATCH, HAL_DBG_PAGE_WRITE_TRIGGER, 
                    (UINT32) &_sys_sram_irq_stackwatch_start, (UINT32)&_sys_sram_irq_stackwatch_end);

    hal_DbgPageProtectSetup (HAL_DBG_PAGE_CODEWATCH, HAL_DBG_PAGE_WRITE_TRIGGER, 
                    (UINT32) &_ram_txt_start, (UINT32)&_ram_txt_end);

    hal_DbgPageProtectSetup (HAL_DBG_PAGE_INT_CODEWATCH, HAL_DBG_PAGE_WRITE_TRIGGER,  
                        (UINT32)&_sys_sram_txt_start, (UINT32)&_sys_sram_txt_end);

    hal_DbgPageProtectEnable (HAL_DBG_PAGE_IRQ_STACKWATCH);

    hal_DbgPageProtectEnable (HAL_DBG_PAGE_CODEWATCH);

    hal_DbgPageProtectEnable (HAL_DBG_PAGE_INT_CODEWATCH);

#if (PAGE_SPY_AT_ZERO == 1)
    hal_DbgPageProtectSetup(HAL_DBG_PAGE_4, HAL_DBG_PAGE_READWRITE_TRIGGER, (UINT32)g_SpZero, (UINT32)g_SpZero + sizeof(g_SpZero)-1);
    hal_DbgPageProtectEnable(HAL_DBG_PAGE_4);
#endif

#if (PAGE_SPY_LOG == 1)
    g_SpBufferTop = g_SpBuffer;
    g_SpBufferPtr = g_SpBufferTop;
    // The one useful page spy is set in the job thing.
#endif

    // reset the speech fifo
    hal_SpeechFifoReset();

    // Configure debugging
#if ENABLE_PXTS
#if ENABLE_EXL
    hal_DbgPortSetup(HAL_DBG_EXL_AND_PXTS);
#else
    // Enable PXTS
    hal_DbgPortSetup(HAL_DBG_PXTS);
#endif
    // if not extra lvl selected set no extra lvl
#ifndef ENABLE_PXTS_EXTRA_LVL
#define ENABLE_PXTS_EXTRA_LVL 0
#endif /* ENABLE_PXTS_EXTRA_LVL */
    // Enable some interesting levels by default
    hal_DbgPxtsSetup( 1<<HAL_DBG_PXTS_HAL
                    | 1<<HAL_DBG_PXTS_SX
                    | 1<<HAL_DBG_PXTS_PAL
                    | 1<<HAL_DBG_PXTS_BB_IRQ
                    | 1<<HAL_DBG_PXTS_SPC
                    | ENABLE_PXTS_EXTRA_LVL);
#else
#if ENABLE_EXL
    hal_DbgPortSetup(HAL_DBG_EXL);
#endif
#endif
    
    // Open ISPI with default (romed) config
    // (must be done before hal_AbbOpen/pmd_Open)
    hal_IspiOpen(NULL);
    // Initialize ABB (must be done before pmd_Open, as the ABB settings
    // in pmd_Open cannot be overridden or reset to defaults)
    hal_AbbOpen();

    // Initializes Power Management Driver and set
    // it as no-eco mode.
    pmd_Open(tgt_GetPmdConfig());
    pmd_SetPowerMode(PMD_IDLEPOWER);
    // initialise pal and rf stuff (transceiver, lps).
    pal_Open();
    // Ask Target to request every modules to register itself.
    tgt_RegisterAllModules();
    
    // Camera: Configure the camera RST and PDN lines so that
    // the camera is off by now.
    // We trick this by opening and closing the camera drivers
    // with dummy configurations.
    if (halCfg->camCfg.camUsed)
    {
        HAL_CAMERA_CFG_T cameraConfig;

        // Camera 0
        cameraConfig.rstActiveH = halCfg->camCfg.camRstActiveH;
        cameraConfig.pdnActiveH = halCfg->camCfg.camPdnActiveH;
        cameraConfig.dropFrame  = FALSE; // Irrelevant parameter.
        cameraConfig.camClkDiv  = 0xFF; // Irrelevant parameter.
        cameraConfig.endianess  = NO_SWAP; // Irrelevant parameter.
        cameraConfig.colRatio   = COL_RATIO_1_1; // Irrelevant parameter.
        cameraConfig.rowRatio   = ROW_RATIO_1_1; // Irrelevant parameter.
        cameraConfig.camId      = 0;

        hal_CameraOpen(&cameraConfig);
        hal_CameraClose();
    }
	if (halCfg->camCfg.cam1Used)
	{
        HAL_CAMERA_CFG_T cameraConfig;

        // Camera 1		
        cameraConfig.rstActiveH = halCfg->camCfg.cam1RstActiveH;
        cameraConfig.pdnActiveH = halCfg->camCfg.cam1PdnActiveH;
        cameraConfig.dropFrame  = FALSE; // Irrelevant parameter.
        cameraConfig.camClkDiv  = 0xFF; // Irrelevant parameter.
        cameraConfig.endianess  = NO_SWAP; // Irrelevant parameter.
        cameraConfig.colRatio   = COL_RATIO_1_1; // Irrelevant parameter.
        cameraConfig.rowRatio   = ROW_RATIO_1_1; // Irrelevant parameter.
        cameraConfig.camId      = 1;

        hal_CameraOpen(&cameraConfig);
        hal_CameraClose();
     }

#ifdef EARLY_TRACE_OPEN
    extern void sxs_HostTraceEarlyOpenDone (void);
    sxs_HostTraceEarlyOpenDone ();
#endif // EARLY_TRACE_OPEN
}




