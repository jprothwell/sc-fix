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
/// @file hal_camera.c
/// This file contains the camera module driver 
/// implementation.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "chip_id.h"

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)

#include "cs_types.h"
#include "camera.h"
#include "hal_camera.h"
#include "hal_sys.h"
#include "halp_sys.h"
#include "global_macros.h"
#include "halp_camera.h"
#include "halp_config.h"
#include "hal_gpio.h"
#include "cfg_regs.h"

#include "halp_debug.h"
#include "halp_sys_ifc.h"

#include "hal_error.h"

// ============================================================================
// MACROS
// ============================================================================
//
/// Flag to enable the Camera Module test mode. This is useful when the
/// Debug Bus needs to be used: the Camera Bus and the Debug Bus are
/// multiplexed in the chip IOMux, so they cannot be used both at same
/// time. The idea of the Camera test mode is to displays a dummy image
/// instead of getting its data from the sensor.
/// So, when CT_RELEASE=cool_profile, the test mode is enabled: the PXTS,
/// EXL or signal spy cannot be used along with the Camera. When the camera
/// needs to be used, use CT_RELEASE=debug, for instance.
#ifdef ENABLE_PXTS
#define CAM_TEST_MODE     CAMERA_TEST
#else
#define CAM_TEST_MODE     0 //CAMERA_TEST
#endif

#ifdef HAL_CAMERA_PRINTF
#define HAL_CAMERA_TRACE(a, ...) HAL_TRACE(_PAL | TSTDOUT, 0, a, ##__VA_ARGS__)
#else
#define HAL_CAMERA_TRACE(a, ...)
#endif
// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

PRIVATE UINT8  g_halCameraIfcChan  = HAL_UNKNOWN_CHANNEL;

PRIVATE HAL_CAMERA_CFG_T g_halCameraConfig = {
    .rstActiveH = TRUE,
    .pdnActiveH = TRUE,
    .dropFrame  = FALSE,
    .camClkDiv  = 18,
    .endianess  = BYTE_SWAP,
    .camId      = 0
};


// =============================================================================
// g_halCameraIrqHandler
// -----------------------------------------------------------------------------
/// Variable to store the user irq handler for the camera
/// interruption.
// =============================================================================
PRIVATE HAL_CAMERA_IRQ_HANDLER_T g_halCameraIrqHandler;

// ============================================================================
// FUNCTIONS
// ============================================================================

// =============================================================================
// hal_CameraReset(BOOL InReset)
// -----------------------------------------------------------------------------
/// Puts the Camera sensor in Reset or out of Reset.
/// 
/// @param InReset if true, put the external camera sensor in reset
// =============================================================================
PUBLIC VOID hal_CameraReset(BOOL InReset)
{
    if (g_halCameraConfig.camId == 0)
    {
        // Use sensor 0
        if (g_halCfg->camCfg.camRstRemapFlag)
        {
            // Use GPIO
            if ((g_halCameraConfig.rstActiveH && InReset) || ((!g_halCameraConfig.rstActiveH) && (!InReset)))
            {
                hal_GpioSet(g_halCfg->camCfg.camRstRemap);
            }
            else
            {
                hal_GpioClr(g_halCfg->camCfg.camRstRemap);
            }
        }
        else
        {
            // Configures the Reset bit to the camera
            if ((g_halCameraConfig.rstActiveH && InReset) || ((!g_halCameraConfig.rstActiveH) && (!InReset)))
            {
                hwp_camera->Camera_CONTROL_register |= (CAMERA_RESET);
            }
            else
            {
                hwp_camera->Camera_CONTROL_register &= ~(CAMERA_RESET);
            }
        }
    }
    else
    {
        // Use sensor 1
        // Use GPIO
        if ((g_halCameraConfig.rstActiveH && InReset) || ((!g_halCameraConfig.rstActiveH) && (!InReset)))
        {
            hal_GpioSet(g_halCfg->camCfg.cam1RstPin);
        }
        else
        {
            hal_GpioClr(g_halCfg->camCfg.cam1RstPin);
        }

    }
}

// =============================================================================
// hal_CameraOpen
// -----------------------------------------------------------------------------
/// Enable the Camera Control module and set the default configuration
/// for the selected external camera module.  It is both held in reset
/// AND kept in power down.
///
/// In case of dual-sensor platform, the #camId field of the #camConfig
/// parameter is to be used to select which sensor is to be enabled.
/// 
/// @param camConfig Camera configuration. (cf #HAL_CAMERA_CFG_T).
// =============================================================================
PUBLIC VOID hal_CameraOpen(HAL_CAMERA_CFG_T* camConfig)
{
    HAL_ASSERT(g_halCameraIfcChan == HAL_UNKNOWN_CHANNEL,
               "hal_CameraOpen: Camera already open");

    g_halCameraConfig = *camConfig;

    hwp_camera->Camera_CONTROL_register = CAM_TEST_MODE;

    // Configure Camera Clock Divider
    hal_CameraSetupClockDivider(g_halCameraConfig.camClkDiv);

    // Set endianess also reset the apbi
    hwp_camera->apbi_ctrl_camera = (CAMERA_L_ENDIAN(g_halCameraConfig.endianess));

    if (!g_halCameraConfig.dropFrame)
    {
        hwp_camera->Camera_CONTROL_register |= CAMERA_MODE;
    }

    if (g_halCameraConfig.camId == 0)
    {
        // Use sensor 0
     
        // Check compatibility between sensor and HAL camera
        // module configuration.
        HAL_ASSERT(g_halCfg->camCfg.camPdnActiveH == g_halCameraConfig.pdnActiveH
                && g_halCfg->camCfg.camRstActiveH == g_halCameraConfig.rstActiveH,
                "Camera sensor and HAL camera module configurations incompatibles !");

        if (g_halCfg->camCfg.camRstRemapFlag)
        {
            if(g_halCfg->camCfg.camRstRemap.type == HAL_GPIO_TYPE_IO) //GPIO
            {
                hal_GpioSetOut(g_halCfg->camCfg.camRstRemap.gpioId);
            }
            else if (g_halCfg->camCfg.camRstRemap.type != HAL_GPIO_TYPE_O)
            {
                HAL_ASSERT(FALSE, "Should Use GPIO/GPO as CAM_RST");
            }
        }

        if (g_halCfg->camCfg.camPdnRemapFlag)
        {
            if (g_halCfg->camCfg.camPdnRemap.type == HAL_GPIO_TYPE_IO) //GPIO
            {
                hal_GpioSetOut(g_halCfg->camCfg.camPdnRemap.gpioId);
            }
            else if (g_halCfg->camCfg.camPdnRemap.type != HAL_GPIO_TYPE_O)
            {
                HAL_ASSERT(FALSE, "Should Use GPIO/GPO as CAM_PDN");
            }
        }
    }
    else
    {
        // Use sensor 1
        
        // Check compatibility between sensor and HAL camera
        // module configuration.
        HAL_ASSERT(g_halCfg->camCfg.cam1PdnActiveH == g_halCameraConfig.pdnActiveH
                && g_halCfg->camCfg.cam1RstActiveH == g_halCameraConfig.rstActiveH,
                "Camera sensor and HAL camera module configurations incompatibles !");

        if(g_halCfg->camCfg.cam1RstPin.type == HAL_GPIO_TYPE_IO) //GPIO
        {
            hal_GpioSetOut(g_halCfg->camCfg.cam1RstPin.gpioId);
        }
        else if (g_halCfg->camCfg.cam1RstPin.type != HAL_GPIO_TYPE_O)
        {
            HAL_ASSERT(FALSE, "Should Use GPIO/GPO as CAM_RST");
        }
        
        if (g_halCfg->camCfg.cam1PdnPin.type == HAL_GPIO_TYPE_IO) //GPIO
        {
            hal_GpioSetOut(g_halCfg->camCfg.cam1PdnPin.gpioId);
        }
        else if (g_halCfg->camCfg.cam1PdnPin.type != HAL_GPIO_TYPE_O)
        {
            HAL_ASSERT(FALSE, "Should Use GPIO/GPO as CAM_PDN");
        }
    
    }

    hal_CameraReset(FALSE);
    hal_CameraPowerDown(FALSE);
}


// =============================================================================
// hal_CameraClose
// -----------------------------------------------------------------------------
/// Power off the camera sensor by setting the PowerDown bit.
/// Resets the camera sensor by enabling the Camera Reset bit.
/// This function can only be called after the camera transfer has been stopped
/// by a call to #hal_CameraStopXfer().
// =============================================================================
PUBLIC VOID hal_CameraClose(VOID)
{
    HAL_ASSERT(g_halCameraIfcChan == HAL_UNKNOWN_CHANNEL,
               "HAL Camera: %s called while a transfer was in progress.",
               __func__);

    hal_CameraReset(TRUE);
    hal_CameraPowerDown(TRUE);
    hal_CameraControllerEnable(FALSE);

    g_halCameraIfcChan = HAL_UNKNOWN_CHANNEL;
}


// =============================================================================
// hal_CameraPowerDown(BOOL PowerDown)
// -----------------------------------------------------------------------------
/// Controls the Power Down line of the sensor, turns the clock on and off
/// The camera stays in reset.
/// @param PowerDown: if TRUE the sensor module is in Power Down and the clock is shut down
///                   if FALSE the sensor module is in Power Down and the clock is shut down
// =============================================================================
PUBLIC VOID hal_CameraPowerDown(BOOL PowerDown)
{
    if (g_halCameraConfig.camId == 0)
    {
        // Use sensor 0
        if(g_halCfg->camCfg.camPdnRemapFlag)
        {
            if(((TRUE ==PowerDown) && (TRUE ==g_halCameraConfig.pdnActiveH)) ||
               ((FALSE==PowerDown) && (FALSE==g_halCameraConfig.pdnActiveH)))
            {
                hal_GpioSet(g_halCfg->camCfg.camPdnRemap);
            }
            else
            {
                hal_GpioClr(g_halCfg->camCfg.camPdnRemap);
            }
        }
        else
        {
            if(((TRUE ==PowerDown) && (TRUE ==g_halCameraConfig.pdnActiveH)) ||
               ((FALSE==PowerDown) && (FALSE==g_halCameraConfig.pdnActiveH)))
            {
                hwp_camera->Camera_CONTROL_register |= (CAMERA_PWDON);
            }
            else
            {
                hwp_camera->Camera_CONTROL_register &= ~(CAMERA_PWDON);
            }
        }
    }
    else
    {
        // Use sensor 1
        if (((TRUE ==PowerDown) && (TRUE ==g_halCameraConfig.pdnActiveH)) ||
            ((FALSE==PowerDown) && (FALSE==g_halCameraConfig.pdnActiveH)))
        {
            hal_GpioSet(g_halCfg->camCfg.cam1PdnPin);
        }
        else
        {
            hal_GpioClr(g_halCfg->camCfg.cam1PdnPin);
        }
    }

    if (PowerDown)
    {
        hwp_extApb->CLK_CAMERA_OUT_CTRL = 0;
        hal_SysRequestFreq(HAL_SYS_FREQ_CAMCLK, HAL_SYS_FREQ_32K, NULL);
    }
    else
    {
        hal_SysRequestFreq(HAL_SYS_FREQ_CAMCLK, HAL_SYS_FREQ_26M, NULL);
        hwp_extApb->CLK_CAMERA_OUT_CTRL |= REGS_CLK_CAMERA_OUT_EN_ENABLE;
    }
    hwp_camera->Interrupt_CLEAR_register = 0x1f;
}


// =============================================================================
// hal_CameraControllerEnable(BOOL enable)
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID hal_CameraControllerEnable(BOOL enable)
{
    if (enable)
    {
        // Turn on controller and apbi
        hwp_camera->Camera_CONTROL_register |= CAMERA_ENCAMERA;
        //hwp_camera->apbi_ctrl_camera |= CAMERA_SOFT_RST_L;
    }
    else
    {
        // Turn off controller and apbi
        hwp_camera->Camera_CONTROL_register &= ~(CAMERA_ENCAMERA);
        hwp_camera->apbi_ctrl_camera &= ~(CAMERA_SOFT_RST_L);
    }
}


// ============================================================================
// hal_CameraIrqSetHandler
// ----------------------------------------------------------------------------
/// This function configures which user function will be called by the
/// camera driver when an interruption is generated.
/// 
/// @param handler Pointer to a user function called when an interruption
/// is generated by the camera driver.
// ============================================================================
PUBLIC VOID hal_CameraIrqSetHandler(HAL_CAMERA_IRQ_HANDLER_T handler)
{
    g_halCameraIrqHandler = handler;
}



// =============================================================================
// hal_CameraIrqConfig()
// -----------------------------------------------------------------------------
/// Configure the desired interrupts
/// 
/// @param mask Mask to enable specific interrupts.  Valid interrupts are
/// liste in HAL_CAMERA_IRQ_CAUSE_T.
// =============================================================================
PUBLIC VOID hal_CameraIrqSetMask(HAL_CAMERA_IRQ_CAUSE_T mask)
{
    // The real mask bits in the hardware register may not be the same as the 

    union
    {
        UINT32 reg;
        HAL_CAMERA_IRQ_CAUSE_T mask;
    } u;
    u.mask = mask;
    hwp_camera->Interrupt_CLEAR_register = u.reg;
    hwp_camera->Interrupt_MASK_register  = u.reg;
}


PROTECTED VOID hal_CameraIrqHandler(UINT8 interruptId)
{
    union
    {
        UINT32 reg;
        HAL_CAMERA_IRQ_CAUSE_T irq;
    } u;

    // Get cause
    u.reg = (UINT32) hwp_camera->Interrupt_CAUSE_register;

    // Clear IRQ
    hwp_camera->Interrupt_CLEAR_register = u.reg;

    // Call User handler
    if (g_halCameraIrqHandler)
    {
        g_halCameraIrqHandler(u.irq);
    }
}


// =============================================================================
// hal_CameraStopXfer
// -----------------------------------------------------------------------------
/// Must be called at the end of the Frame Capture
/// If an underflow occured and the IFC tranfer is not complete, 
/// this function will handle the channel release
///
/// @param stop If \c TRUE, stops the camera controller.
/// @return 0 when the IC transfer was complete
///         1 when the IFC transfer was not complete and the channel had to be released
// =============================================================================
PUBLIC HAL_CAMERA_XFER_STATUS_T hal_CameraStopXfer(BOOL stop)
{
    UINT32               status;

    // Disable the Camera controller in any case to avoid toggling
    if(stop)
    {
        hal_CameraControllerEnable(FALSE);
    }

    if(g_halCameraIfcChan != HAL_UNKNOWN_CHANNEL)
    {
        /// @todo This test should not be necessary with the new IFC driver behaviour.
        status = hal_SysEnterCriticalSection();
        if(hal_IfcGetOwner(g_halCameraIfcChan) == HAL_IFC_CAMERA_RX)
        {
            UINT32 pxl;
            if((pxl = hal_IfcGetTc(HAL_IFC_CAMERA_RX, g_halCameraIfcChan)) != 0)
            {
                if(!stop)
                {
                    hal_CameraControllerEnable(FALSE);
                }

                // Flush the remaining data in the IFC Fifo
                hal_IfcChannelFlush(HAL_IFC_CAMERA_RX, g_halCameraIfcChan);
                while(!hal_IfcChannelIsFifoEmpty(HAL_IFC_CAMERA_RX, g_halCameraIfcChan));
                // Release the channel
                hal_IfcChannelRelease(HAL_IFC_CAMERA_RX, g_halCameraIfcChan);

                // Channel is released
                g_halCameraIfcChan = HAL_UNKNOWN_CHANNEL;
                hal_SysExitCriticalSection(status);
                if(!stop)
                {
                    hal_CameraControllerEnable(TRUE);
                }
                // Try to determine why we missed data
                if(hwp_camera->Interrupt_STATUS_register & CAMERA_OVFL)
                {
                    HAL_CAMERA_TRACE("Overflow during transfer between camera module and IFC");
                }
                else
                {
                    HAL_CAMERA_TRACE("Missing data between external camera and camera module:%d",pxl);
                }
                return(XFER_NOT_FINISHED);
            }
        }

        // Channel has been released automatically
        g_halCameraIfcChan = HAL_UNKNOWN_CHANNEL;
        hal_SysExitCriticalSection(status);
        if (!(hwp_camera->Interrupt_STATUS_register & CAMERA_OVFL))
        {
            return(XFER_SUCCESS);
        }

        HAL_CAMERA_TRACE("IFC Transfer finished but Overflow bit is set - Transfer size not matching resolution");
        return(XFER_FINISHED_WITH_OVERFLOW);
    }

    HAL_ASSERT(FALSE, "hal_CameraStopXfer called with no if channel reserved for camera !");
    return(XFER_ERR_WRONG_PARAM);
}


// =============================================================================
// hal_CameraStartXfer
// -----------------------------------------------------------------------------
/// This function begins the IFC transfer of the camera data.  The camera
/// itself is reset and the camera module internal fifo is cleared.  The IFC
/// transfer is then started.
/// @param BufSize This is the size of the buffer in _bytes_
/// @param Buffer Pointer to the video buffer where the IFC will store the data.
/// @return IFC channel number.
// =============================================================================
PUBLIC UINT8 hal_CameraStartXfer(UINT32 bufSize, UINT8* buffer)
{
    hwp_camera->apbi_ctrl_camera         &= ~(CAMERA_SOFT_RST_L);
    hwp_camera->apbi_ctrl_camera         |= (CAMERA_SOFT_RST_L);
    hwp_camera->Interrupt_CLEAR_register  = CAMERA_OVFL;       // Clear the Overflow bit for checking later
    g_halCameraIfcChan                    = hal_IfcTransferStart(HAL_IFC_CAMERA_RX,
        buffer, bufSize, HAL_IFC_SIZE_32_MODE_AUTO);
    
    return(g_halCameraIfcChan);
}


// =============================================================================
// hal_CameraSetupClockDivider
// -----------------------------------------------------------------------------
// =============================================================================
PUBLIC VOID hal_CameraSetupClockDivider(UINT8 divider)
{
    UINT32 newClkScale;
    UINT32 ctrlReg;

    // Save enable bit
    ctrlReg = hwp_extApb->CLK_CAMERA_OUT_CTRL & REGS_CLK_CAMERA_OUT_EN_ENABLE;

    // Saturate the divider to the maximum value supported
    // by the hardware.
    if(divider-2 > (REGS_CLK_CAMERA_OUT_DIV_MASK>>REGS_CLK_CAMERA_OUT_DIV_SHIFT))
    {
        newClkScale = (REGS_CLK_CAMERA_OUT_DIV_MASK>>REGS_CLK_CAMERA_OUT_DIV_SHIFT) + 2;
    }
    else if(divider<2)
    {
        newClkScale = 2;
    }
    else
    {
        newClkScale = divider;
    }

    // Divider is register value+2, so we take off 2 here.
    ctrlReg |=  REGS_CLK_CAMERA_DIV_SRC_SEL_156_MHZ      |
                REGS_CLK_CAMERA_OUT_SRC_SEL_FROM_DIVIDER |
                REGS_CLK_CAMERA_OUT_DIV(newClkScale-2);

    // Restore initial config with new clock scal.
    hwp_extApb->CLK_CAMERA_OUT_CTRL = ctrlReg;
}


// ============================================================================
// hal_UartForcePinsLow
// ----------------------------------------------------------------------------
/// This function force all output pins of the camera interface to low.
///
/// This function must only be called when the camera interface is closed.
// ============================================================================
PUBLIC VOID hal_CameraForcePinsLow(VOID)
{
    // FIXME This doesn't actually ensure the camera is closed,
    // just that no transfer is in progress. It could be useful
    // to therefore change that test and actually check hal_CameraClose
    // is called before (A global variable ?)
    HAL_ASSERT(g_halCameraIfcChan == HAL_UNKNOWN_CHANNEL,
               "HAL Camera: %s called while the camera is not closed",
               __func__);

    // Force reset line low.
    if (g_halCameraConfig.camId == 0)
    {
        // Use sensor 0.
        if (g_halCfg->camCfg.camRstRemapFlag)
        {
            // Use GPIO
            // Force power down line low.
            hal_GpioClr(g_halCfg->camCfg.camPdnRemap);
            // Force reset line low.
            hal_GpioClr(g_halCfg->camCfg.camRstRemap);
        }
        else
        {
            // Use camera module bits.
            // Power Down line.
            hwp_camera->Camera_CONTROL_register &= ~(CAMERA_PWDON);
            // Reset line.
            hwp_camera->Camera_CONTROL_register &= ~(CAMERA_RESET);
        }
    }
    else
    {
        // Use sensor 1
        hal_GpioClr(g_halCfg->camCfg.cam1PdnPin);
        hal_GpioClr(g_halCfg->camCfg.cam1RstPin);
    }
}

#endif // CHIP_ASIC_ID_GREENSTONE


