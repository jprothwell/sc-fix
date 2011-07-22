#include "cs_types.h"
#include "sxs_io.h"

#include "hal_camera.h"
#include "hal_gpio.h"
#include "drv_camera.h"
#include "pmd_m.h"

volatile UINT8 gIfcChan = 0xff;


VOID nhal_CameraOpen(BOOL rstActiveH, BOOL dropFrame)
{
    HAL_GPIO_OR_GPO_ID_T StupidThing;

    // Configure GPIOs (Powerdown = GPIO1, Reset = GPIO0
    // Powerdown -> 1
    // Reset -> 1
    StupidThing.type = HAL_GPIO_TYPE_IO;
    StupidThing.id.gpioId = HAL_GPIO_0 | HAL_GPIO_1;
    hal_GpioSet(StupidThing);
    hal_GpioSetOut(HAL_GPIO_0 | HAL_GPIO_1);
    if (gSensorInfo.rstactiveh)
    {
        hal_CameraOpen(TRUE, FALSE);
    }
}

VOID nhal_CameraReset(BOOL rstActiveH, BOOL InReset)
{
    HAL_GPIO_OR_GPO_ID_T StupidThing;

    // Reset -> 0
    StupidThing.type = HAL_GPIO_TYPE_IO;
    StupidThing.id.gpioId = HAL_GPIO_0;

    if (InReset)
    {
        hal_GpioSet(StupidThing);
    }
    else
    {
        hal_GpioClr(StupidThing);
    }
}

VOID nhal_CameraPowerOn()
{
    HAL_GPIO_OR_GPO_ID_T StupidThing;

    // Powerdown => 0
    StupidThing.type = HAL_GPIO_TYPE_IO;
    StupidThing.id.gpioId = HAL_GPIO_1;
    hal_GpioClr(StupidThing);
}

// ############
// Public
// ############

//PUBLIC void camera_CameraInfo(CAM_INFO_T *Info)
//{
//}

// The camera will be powered on in a particular mode specified here
// Later, when the video capture or image capture is 'prepared', the frame buffer size
// must correspond to the size here unless the format is changed.
PUBLIC void camera_PowerOn(CAM_SIZE_T FrameSize, CAM_FORMAT_T Format)
{
    // Turn on any power required for camera
    // Enable the Camera Interface Module
    // Turn on the Camera
    // Reset & initialize the Camera


    SXS_TRACE(TSTDOUT,"DRV_CAM: Power On");
    // Sets the PowerDown bit of the Camera Control IF to power on the camera
    //
    // Mask all interrupts
    HAL_CAMERA_IRQ_MASK_T mask = {0,0,0,0};
    hal_CameraIrqSetMask(mask);

    // Enable Camera LDO
    pmd_EnablePower(PMD_POWER_CAMERA,TRUE);

    hal_CameraPowerOn();
    if (gSensorInfo.snrrst)
    {
        // Toggle the reset bit
    }
    else
    {
        // Just bring the sensor out of reset
        hal_CameraReset(TRUE, FALSE);
    }


    // Should use FrameSize and Format to decide which sequence to use
    // FrameSize and Format may be changed later but the camera and IFC need
    // to be stopped and the IFC and camera re-enabled in the correct order
    // Power On seq
    // After this, the camera will start sending data to the camera module.  The IFC will
    // need to be started at the correct time (typically after the end of frame interrupt
    // so the data will be properly synchronized in the buffer
    if (FrameSize == CAM_NPIX_VGA)
    {
        camera_SendRgSeq(RG_InitPowerOnVga,  (sizeof(RG_InitPowerOnVga) / sizeof(CAM_REG_T)));
    }
    else
    {
        camera_SendRgSeq(RG_InitPowerOnQvga,  (sizeof(RG_InitPowerOnQvga) / sizeof(CAM_REG_T)));
    }

    // Sensor Init seq
}

PUBLIC void camera_PowerOff(void)
{
    // Turn off the Camera
    // Disable Camera Interface Module
    hal_CameraClose();
    // Disable the LDO for the camera
    pmd_EnablePower(PMD_POWER_CAMERA,FALSE);
}

/**
 * This must be called first before camer_PowerOn()
 */
PUBLIC void camera_Reserve(CAM_IRQ_HANDLER_T FrameReady)
{

    // Take a resource?
    // Initialize SCCB (I2C) Interface
    // Initialize Camera Interface, but does not turn on any power
    // Camera is left in reset state
    

    hal_CameraOpen(gSensorInfo.rstactiveh, FALSE);
    gSensorInfo.camirqhandler = FrameReady;

    hal_CameraIrqSetHandler(camera_InteruptHandler);
    if (gIfcChan != 0xff)
    {
        // Force reset of channel
        hal_CameraIfcPrepare();
        gIfcChan = 0xff;
    }
}

PUBLIC void camera_Release(void)
{
    // 'De-initialize' camera
    // Release resource
}


PUBLIC void camera_SetZoomFactorL(UINT32 ZoomFactor)
{
}

PUBLIC UINT32 camera_ZoomFactor(void)
{
    return(0);
}

PUBLIC void camera_SetDigitalZoomFactorL(UINT32 DigZoomFactor)
{
}

PUBLIC UINT32 camera_DigitalZoomFactor(void)
{
    return(0);
}

PUBLIC void camera_SetContrastL(UINT32 Contrast)
{
}

PUBLIC UINT32 camera_Contrast(void)
{
    return(0);
}

PUBLIC void camera_SetBrightnessL(UINT32 Brightness)
{
}

PUBLIC UINT32 camera_Brightness(void)
{
    return(0);
}

PUBLIC void camera_SetFlashL(CAM_FLASH_T Flash)
{
}

PUBLIC CAM_FLASH_T camera_Flash(void)
{
    return(CAM_FLASH_NONE);
}

PUBLIC void camera_SetExposureL(CAM_EXPOSURE_T Exposure)
{
}

PUBLIC CAM_EXPOSURE_T camera_Exposure(void)
{
    return(CAM_EXPOSURE_AUTO);
}

PUBLIC void camera_SetWhiteBalanceL(CAM_WHITEBALANCE_T WhiteBalance)
{
}

PUBLIC CAM_WHITEBALANCE_T camera_WhiteBalance(void)
{
    return(CAM_WB_AUTO);
}

PUBLIC VOID camera_SetSpecialEffect(CAM_SPECIALEFFECT_T SpecialEffect)
{
}

PUBLIC CAM_SPECIALEFFECT_T camera_SpecialEffect(VOID)
{
    return(CAM_SPE_NORMAL);
 
}

// NightMode : 1: NightModeEN; 0:NightModeDis
PUBLIC VOID camera_SetNightMode(UINT32 NightMode)
{
}
PUBLIC UINT32 camera_NightMode(VOID)
{
     return(FALSE);

}
// =============================================================================
// camera_StartViewFinderDirectL(some parameters)
// -----------------------------------------------------------------------------
/// Configure the camera to capture frames for viewfinder.  This should be
/// not the maximum resolution of the camera, but a resolution as close to
/// the LCD as possible.  The parameters for the screen should be passed here
/// and this function should automatically 'calculate' the best size based on
/// the possible output formatter configurations.
/// 
/// @param Some stuff to be defined
// =============================================================================

    PUBLIC void camera_StartViewFinderDirectL()
{
    // Not really direct, but sets the camera into viewfinder mode (lower resolution to fit the screen)
    // Needs
}

    PUBLIC void camera_StopViewFinder(void)
{
}

PUBLIC BOOL camera_ViewFinderActive(void)
{
    return(FALSE);
}

PUBLIC void camera_SetViewFinderMirror(BOOL Mirror)
{
}

PUBLIC BOOL camera_ViewFinderMirror(void)
{
    return(FALSE);
}

PUBLIC void camera_PrepareImageCaptureL(CAM_FORMAT_T ImageFormat, CAM_SIZE_T SizeIndex, CAM_IMAGE_T Buffer)
{
//    SXS_TRACE(TSTDOUT,"DRV_CAM: PrepareCapture %x", Buffer);
    // ImageBuf must be allocated by the upper level and should be large enough for the 
    // TODO: Configure the camera to the new image format and size
    // For now, just keep VGA RGB565
    //camera_SendRgSeq(RG_VidFormatxxx,  (sizeof(RG_VidFormatxxx,) / sizeof(CAM_REG_T)));
    gSensorInfo.format = ImageFormat;
    gSensorInfo.npixels = SizeIndex;

    gSensorInfo.vidbuffer = Buffer;

    // TODO: Handle the error case.  This just asks hal_Camera to get the IFC Channel
//    IfcChan = hal_CameraIfcPrepare();
//    SXS_TRACE(TSTDOUT,"DRV_CAM: Got IFC Chan %d", IfcChan);

    // When the next interrupt arrives (end of frame), the handler will
    // check the cammode and start the IFC and switch to CAM_MODE_IMG_CAPTURE
    gSensorInfo.cammode = CAM_MODE_PREP_IMG_CAPTURE;
}

PUBLIC void camera_CaptureImage()
{
    // The cammode should now be CAM_MODE_PREP_IMG_CAPTURE.  Can trigger the interrupt
    // to start the capture on the next frame (actually the end of the current frame)
    // Clear all the camera interrupts and enable only the end of frame
    // Stupid.  Only VSYNC interrupt is available in Mode=1
//    SXS_TRACE(TSTDOUT,"DRV_CAM: Capture");
    HAL_CAMERA_IRQ_MASK_T mask = {0,0,0,1};
    hal_CameraIrqSetMask(mask);

}

PUBLIC void camera_CancelCaptureImage(void)
{
}

PUBLIC void camera_EnumerateCaptureSizes(CAM_SIZE_T *Size,UINT32 SizeIndex, CAM_FORMAT_T Format)
{
}

PUBLIC void camera_PrepareVideoCaptureL(CAM_FORMAT_T Format, UINT32 SizeIndex, UINT32 RateIndex, UINT32 BuffersToUse, UINT32 FramesPerBuffer)
{
}

PUBLIC void camera_StartVideoCapture(void)
{
}

PUBLIC void camera_StopVideoCapture(void)
{
    if (gSensorInfo.capturevideo)
    {
    }
}

PUBLIC BOOL camera_VideoCaptureActive(void)
{
    return(gSensorInfo.capturevideo);
}

PUBLIC void camera_EnumerateVideoFrameSizes(
        CAM_SIZE_T Size,
        UINT32 SizeIndex,
        CAM_FORMAT_T Format)
{
}

PUBLIC void camera_EnumerateVideoFrameRates()
{
}

PUBLIC void camera_GetFrameSize(CAM_SIZE_T Size)
{
}

PUBLIC VOID camera_SetFrameRate(UINT32 FrameRate)
{
}

PUBLIC UINT32 camera_FrameRate(void)
{
    return(0);
}

PUBLIC UINT32 camera_BuffersInUse(void)
{
    return(0);
}

PUBLIC UINT32 camera_FramesPerBuffer(void)
{
    return(0);
}

PUBLIC UINT8 camera_GetId(VOID)
{
    return(camera_ReadReg(0x0A));
}

// ############
// Private
// ############
VOID camera_Delay(UINT16 Wait_mS)
{
    UINT32 Ticks_16K;

    // Here is the delay function used by the camera driver.  It can be changed to use a non-blocking
    // wait if necessary, but for now, just use hal_TimDelay
    Ticks_16K = (16348 * Wait_mS)/1000;
    hal_TimDelay(Ticks_16K);
}

VOID camera_DbgSendSequence(UINT8 SeqNum)
{
    UINT16 RegCount;
    UINT16 Len;
    CAM_REG_T *RegVal;

    switch (SeqNum)
    {
        case (1) :
            Len = sizeof(RG_InitPowerOnVga) / sizeof(CAM_REG_T);
            RegVal =  (CAM_REG_T*)&RG_InitPowerOnVga;
        break;
        case (2) :
            Len = sizeof(RG_VidFormatVga) / sizeof(CAM_REG_T);
            RegVal =  (CAM_REG_T*)&RG_VidFormatVga;
        break;
        case (3) :
            Len = sizeof(RG_VidFormatQvga) / sizeof(CAM_REG_T);
            RegVal =  (CAM_REG_T*)&RG_VidFormatQvga;
        break;
        case (4) :
            Len = sizeof(RG_VidFormatQqvga) / sizeof(CAM_REG_T);
            RegVal =  (CAM_REG_T*)&RG_VidFormatQqvga;
        break;
        default :
            Len = 0;
            RegVal = NULL;
        break;
    }
    for (RegCount = 0; RegCount < Len; ++RegCount)
    {
        camera_WriteReg(RegVal[RegCount]);
    }
}

VOID camera_SendRgSeq(const CAM_REG_T *RegList, UINT16 Len)
{
    UINT16 RegCount;

    for (RegCount = 0; RegCount < Len; ++RegCount)
    {
        camera_WriteReg(RegList[RegCount]);
    }
}

VOID camera_WriteReg(CAM_REG_T RegWrite)
{
    hal_I2cSendRawByte(((gSensorInfo.i2caddress<<1) & 0xfe), PHASE3_WR_PH1);
    hal_I2cSendRawByte(RegWrite.Addr, PHASE3_WR_PH2);
    hal_I2cSendRawByte(RegWrite.Data, PHASE3_WR_PH3);

    // Do delay here using RegWrite.Wait mS
    camera_Delay(RegWrite.Wait);
}

UINT8 camera_ReadReg(UINT8 Addr)
{
    if (hal_I2cSendRawByte(((gSensorInfo.i2caddress<<1) & 0xfe), PHASE2_WR_PH1) == HAL_ERR_RESOURCE_BUSY)
    {
        return(0x1f);
    }
    if (hal_I2cSendRawByte(Addr, PHASE2_WR_PH2) == HAL_ERR_RESOURCE_BUSY)
    {
        return(0x1e);
    }

    if (hal_I2cSendRawByte(((gSensorInfo.i2caddress<<1) | 0x01), PHASE2_RD_PH1) == HAL_ERR_RESOURCE_BUSY)
    {
        return(0x1d);
    }

    return(hal_I2cReadRawByte(PHASE2_RD_PH2));
}

VOID camera_InteruptHandler()
{
    HAL_CAMERA_IRQ_MASK_T mask = {0,0,0,0};

//    SXS_TRACE(TSTDOUT,"DRV_CAM: IRQ");
    switch (gSensorInfo.cammode)
    {
        case CAM_MODE_PREP_IMG_CAPTURE:
            // Configure IFC
            // TODO: Not very clean here, should find better way to calculate xfer size since
            // we know it takes 16 bits / pixel...
            gIfcChan = hal_CameraIfcStart((gSensorInfo.npixels*2), (UINT8*)gSensorInfo.vidbuffer);
//            SXS_TRACE(TSTDOUT,"DRV_CAM: Start IFC %d, %x on %d", gSensorInfo.npixels, gSensorInfo.vidbuffer, gIfcChan);
            // Reset interrupt
            mask.vsync= 1;
            hal_CameraIrqSetMask(mask);
            gSensorInfo.cammode = CAM_MODE_IMG_CAPTURE;
//            gSensorInfo.camirqhandler(7, 0);        // Send debug signal
            break;
        case CAM_MODE_IMG_CAPTURE:
            // TODO: When capture is done, should go back to viewfinder mode, or just shut down the camera?
//            SXS_TRACE(TSTDOUT,"DRV_CAM: End of Image Capture");
            hal_CameraIfcPrepare();     // This is really debug stuff
            //hal_CameraReset(TRUE, TRUE);
            hal_CameraIrqSetMask(mask);
            gSensorInfo.cammode = CAM_MODE_IDLE;
            gSensorInfo.camirqhandler(0, gSensorInfo.currentbuf);
            break;
        default:
            gSensorInfo.camirqhandler(255, 255);
            break;
    }

    //if (gSensorInfo.currentbuf == gSensorInfo.numactivebuffers)
    //{
    //    gSensorInfo.currentbuf = 0;
    //}
}
