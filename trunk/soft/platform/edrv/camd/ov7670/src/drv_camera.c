#include "cs_types.h"
#include "sxr_ops.h"
#include "sxs_io.h"

#include "hal_camera.h"
#include "hal_gpio.h"
#include "hal_host.h"
#include "drv_camera.h"
#include "pmd_m.h"

#include "camdp_debug.h"

/* MsgBody_t eeds to be defined before we include itf_msg.h */
typedef UINT8 MsgBody_t;
#include "itf_msg.h"

PRIVATE CAMD_INFO_T g_camdInfo =
{
    "OV7670 RGB Mode",      // char desc[40];   

    HAL_I2C_BPS_100K,       // UINT16 i2crate;
    0x21,                   // UINT8 i2caddress; this is the _REAL_ slave address.  
                            //  The value written in phase 1 of a write cycle will be 0x42
                            //  while the value just before the read will be 0x43
    0x21,                   // UINT8 i2cispaddress;
    0xff,                   // ifcChan
    0,                      // Mailbox to send Frame done message
    CAMD_MODE_IDLE,         // Camera Mode
    NULL,                   // No buffer yet
    0,                      // No pixels to put there
    FALSE,                  // No streaming.
    TRUE,                   // skipOddFrames.
    0,                      // dropped pictures before capture.
    0,                       // Size of the streaming buffer
    0                      // At the beginning of the (not yet existing) buffer.
};

// =============================================================================
// CAMD_PARAMETERS_T
// -----------------------------------------------------------------------------
/// This structure contains the camera configuration parameters.  These must be
/// set by the camd_SetParam() function.
// =============================================================================
typedef struct
{
    CAMD_PARAM_RESOLUTION_T         Resolution;
    CAMD_PARAM_FLASH_T              Flash;
    CAMD_PARAM_EXPOSURE_T           Exposure;
    CAMD_PARAM_WHITE_BALANCE_T      WhiteBalance;
    CAMD_PARAM_SPECIAL_EFFECT_T     SpecialEffect;
    CAMD_PARAM_FORMAT_T             Format;
    CAMD_PARAM_OPTICAL_ZOOM_T       OpticalZoom;
    CAMD_PARAM_DIGITAL_ZOOM_T       DigitalZoom;
    CAMD_PARAM_CONTRAST_T           Contrast;
    CAMD_PARAM_BRIGHTNESS_T         Brightness;
    CAMD_PARAM_NIGHT_MODE_T         NightMode;
    UINT32                          frameRate;
} CAMD_PARAMETERS_T;


PRIVATE CAMD_PARAMETERS_T g_CameraParams;


// ############
// Public
// ############


// =============================================================================
// camd_Open
// -----------------------------------------------------------------------------
/// Open the camera driver, and do all the initialization needed to use the 
/// functions of its API.
/// #camd_Open must be called before any other function of the CAMD API can 
/// be used.
/// @param mbx Mailbox. The CAMD driver will send its status message (like
/// capture done, etc ...) in as messages to this mailbox. The ID of CAMD
/// messages are #CAMD_MSG_HEADER_ID.
/// @return #CAMD_ERR_NO, or #CAMD_ERR_NO_DEVICE.
// =============================================================================
PUBLIC CAMD_ERR_T camd_Open(UINT8 mbx)
{
    CAMD_PARAM_RESOLUTION_T Resolution = CAMD_PARAM_RESOLUTION_QVGA;
    HAL_CAMERA_CFG_T CamCfg = {0,};

    CAMD_PROFILE_FUNCTION_ENTER(camd_Open);
    CAMD_TRACE(CAMD_INFO_TRC, 0, "camd_Open %d",mbx);

    g_camdInfo.frameDoneMbx = mbx;
    g_camdInfo.ifcChan = 0xff;
    
    
    CamCfg.rstActiveH = FALSE;
    CamCfg.pdnActiveH = TRUE;
    CamCfg.dropFrame  = FALSE;
    CamCfg.camClkDiv  = 9;
    CamCfg.endianess  = BYTE_SWAP;
    // Leave the camera sensor in reset, do not drop frames
    // Controler is off at this time
    hal_CameraOpen(&CamCfg);

    // Setup IRQ configuration
    // Mask all interrupts
    HAL_CAMERA_IRQ_CAUSE_T mask = {0,0,0,0};
    hal_CameraIrqSetMask(mask);
    hal_CameraIrqSetHandler(camd_InteruptHandler);
    
    // Enable Camera LDO
    pmd_EnablePower(PMD_POWER_CAMERA,TRUE); 

    // Sets the PowerDown bit of the Camera Control IF to power on the camera
    hal_CameraPowerDown(FALSE);

    // Bring the sensor out of reset
    hal_CameraReset(FALSE);

    // Power On seq:
    // Should use Resolution to decide which sequence to use
    // Resolution may be changed later but the camera and IFC need
    // to be stopped and the IFC and camera re-enabled in the correct order
    // After this, the camera will start sending data to the camera module.  The IFC will
    // need to be started at the correct time (typically after the end of frame interrupt
    // so the data will be properly synchronized in the buffer
    if (Resolution == CAMD_PARAM_RESOLUTION_VGA)
    {
    g_camdInfo.nPixels = 640*480;
        camd_SendRgSeq(RG_InitPowerOnVga,  (sizeof(RG_InitPowerOnVga) / sizeof(CAMD_REG_T)));
    }
    else if (Resolution == CAMD_PARAM_RESOLUTION_QVGA)
    {
        g_camdInfo.nPixels = 320*240;
        camd_SendRgSeq(RG_InitPowerOnQvga,  (sizeof(RG_InitPowerOnQvga) / sizeof(CAMD_REG_T)));
    }
    else
    {
        g_camdInfo.nPixels = 160*120;
        camd_SendRgSeq(RG_InitPowerOnQqvga,  (sizeof(RG_InitPowerOnQqvga) / sizeof(CAMD_REG_T)));
    }

    CAMD_PROFILE_FUNCTION_EXIT(camd_Open);
    return CAMD_ERR_NO;
}

// =============================================================================
// camd_Close
// -----------------------------------------------------------------------------
/// Close the CAMD driver. After a call to #camd_Close, a call to #camd_Open is
/// needed to be allowed to use CAMD functions.
/// @return #CAMD_ERR_NO.
// =============================================================================
PUBLIC CAMD_ERR_T camd_Close(VOID)
{
    CAMD_PROFILE_FUNCTION_ENTER(camd_Close);
    CAMD_TRACE(CAMD_INFO_TRC, 0, "camd_Close");
    // Turn off the Camera
    // Disable Camera Interface Module
    hal_CameraClose();

    // Set the sensor in reset
    hal_CameraReset(TRUE);
    hal_CameraPowerDown(FALSE);
    // Disable the LDO for the camera
    pmd_EnablePower(PMD_POWER_CAMERA,FALSE);

    hal_CameraIrqSetHandler(NULL);

    g_camdInfo.ifcChan = 0xff;
    CAMD_PROFILE_FUNCTION_EXIT(camd_Close);
    return CAMD_ERR_NO;
}


// =============================================================================
// camd_SetParam
// -----------------------------------------------------------------------------
/// Set a camera parameter to a given value.
/// The parameter to set is among the values defined by the type #CAMD_PARAM_T.
/// If this parameter is not available for a given implementation of CAMD, 
/// the #CAMD_ERR_UNSUPPORTED_PARAM error must be returned.
/// Depending on the CAMD_PARAM_XXX parameter to set, the value can either 
/// be a proper UINT32 number, or a value of the CAMD_PARAM_XXX_T enum, might 
/// it exists. This is detailed in the CAMD_PARAM_T definition.
/// If the value is not supported for this parameter, #CAMD_ERR_UNSUPPORTED_VALUE
/// is returned.
///
/// @param param Camera parameter to set.
/// @param value Value to set to the parameter to.
/// @return #CAMD_ERR_NO, #CAMD_ERR_UNSUPPORTED_PARAM, #CAMD_ERR_UNSUPPORTED_VALUE
/// ...
// =============================================================================
PUBLIC CAMD_ERR_T camd_SetParam(CAMD_PARAM_T param, UINT32 value)
{
    CAMD_PROFILE_FUNCTION_ENTER(camd_SetParam);
    if (param >= CAMD_PARAM_QTY)
    {
        CAMD_PROFILE_FUNCTION_EXIT(camd_SetParam);
        return CAMD_ERR_UNSUPPORTED_PARAM;
    }

    // By doing this, we force the next captured picture to be skipped and reduce the artefact
    g_camdInfo.camdMode         = CAMD_MODE_PREP_IMG_CAPTURE;
    switch (param)
    {
        case CAMD_PARAM_RESOLUTION:
            // Check valid value
            if (((CAMD_PARAM_RESOLUTION_T)value) >= CAMD_PARAM_RESOLUTION_QTY)
            {
                CAMD_PROFILE_FUNCTION_EXIT(camd_SetParam);
                return CAMD_ERR_UNSUPPORTED_VALUE;
            }
            g_CameraParams.Resolution = (CAMD_PARAM_RESOLUTION_T) value;
            camd_SetResolution(g_CameraParams.Resolution);
            break;

        case CAMD_PARAM_FLASH:
            g_CameraParams.Flash = (CAMD_PARAM_FLASH_T) value;
            camd_SetFlash(g_CameraParams.Flash);
            break;
        case CAMD_PARAM_EXPOSURE:
            g_CameraParams.Exposure = (CAMD_PARAM_EXPOSURE_T) value;
            camd_SetExposure(g_CameraParams.Exposure);
            break;
        case CAMD_PARAM_WHITE_BALANCE:
            g_CameraParams.WhiteBalance = (CAMD_PARAM_WHITE_BALANCE_T) value;
            camd_SetWhiteBalance(g_CameraParams.WhiteBalance);
            break;

        case CAMD_PARAM_SPECIAL_EFFECT:
            g_CameraParams.SpecialEffect = (CAMD_PARAM_SPECIAL_EFFECT_T) value;
            camd_SetSpecialEffect(g_CameraParams.SpecialEffect);
            break;

        case CAMD_PARAM_FORMAT:
            CAMD_ASSERT( value == CAMD_PARAM_FORMAT_RGB565,
                            "Only RGB 565 is supported by CAMD");
            g_CameraParams.Format = (CAMD_PARAM_FORMAT_T) value;
            break;

        case CAMD_PARAM_OPTICAL_ZOOM:
            CAMD_TRACE(CAMD_INFO_TRC, 0,"camd Set Optical Zoom %d : Unsupported",value);
            g_CameraParams.OpticalZoom = (CAMD_PARAM_OPTICAL_ZOOM_T) value;
            break;

        case CAMD_PARAM_DIGITAL_ZOOM:
            CAMD_TRACE(CAMD_INFO_TRC, 0,"camd Set Digital Zoom %d : Unsupported",value);
            g_CameraParams.DigitalZoom = (CAMD_PARAM_DIGITAL_ZOOM_T) value;
            break;

        case CAMD_PARAM_CONTRAST:
            g_CameraParams.Contrast = (CAMD_PARAM_CONTRAST_T) value;
            camd_SetContrast(g_CameraParams.Contrast);
            break;
        case CAMD_PARAM_BRIGHTNESS:
            g_CameraParams.Brightness = (CAMD_PARAM_BRIGHTNESS_T) value;
            camd_SetBrightness(g_CameraParams.Brightness);
            break;
        case CAMD_PARAM_NIGHT_MODE:
            g_CameraParams.NightMode = (CAMD_PARAM_NIGHT_MODE_T) value;
            camd_EnableNightMode(value);
            break;
        case CAMD_PARAM_FRAME_RATE:
            g_CameraParams.frameRate = value;
            camd_SetFrameRate(value);
            break;
        default:
            CAMD_TRACE(CAMD_INFO_TRC, 0,"camd SetParam: Unknown Param %d",param);
            CAMD_PROFILE_FUNCTION_EXIT(camd_SetParam);
            return CAMD_ERR_UNSUPPORTED_PARAM;
            break;
    }
    // reset it again in case a VSYNC occured during this function treatment
    g_camdInfo.camdMode         = CAMD_MODE_PREP_IMG_CAPTURE;
    CAMD_PROFILE_FUNCTION_EXIT(camd_SetParam);
    return CAMD_ERR_NO;
}


// =============================================================================
// camd_GetImage
// =============================================================================
/// Capture a single image.
/// @param buffer Buffer where the image data will be stored.
/// When the capture is done, a message with the Id #CAMD_MSG_HEADER_ID, and
/// the status field set to #CAMD_STATUS_CAPTURE_QTY is sent to the mailbox
/// passed as a parameter to the #camd_Open function.
/// @return #CAMD_ERR_NO, #CAMD_ERR_RESOURCE_BUSY.
// =============================================================================
PUBLIC CAMD_ERR_T camd_GetImage(CAMD_IMAGE_T buffer)
{
    HAL_CAMERA_IRQ_CAUSE_T mask = {0,0,0,0};

    CAMD_PROFILE_FUNCTION_ENTER(camd_GetImage);
    CAMD_PROFILE_WINDOW_ENTER(camd_GettingPictureWin);
    CAMD_PROFILE_WINDOW_ENTER(camd_SyncWaitWin);
    g_camdInfo.camdMode  = CAMD_MODE_PREP_IMG_CAPTURE;
    g_camdInfo.imgBuffer = buffer;
    g_camdInfo.streaming = FALSE;
    g_camdInfo.camdDrop  = CAMD_DROP_NB;

    // Clear pending VSYNC IRQ and then enable it
    mask.vsync=1;
    hal_CameraIrqSetMask(mask);

    // Enable Camera controller
    hal_CameraControllerEnable(TRUE);

    CAMD_PROFILE_FUNCTION_EXIT(camd_GetImage);
    CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_GetImage : Skip %d ",g_camdInfo.camdDrop);
    return CAMD_ERR_NO;
}


// =============================================================================
// camd_ImageStreamStart
// =============================================================================
/// Start to capture several images in a row in a buffer. The images are 
/// recorded side by side in the buffer. The size of the buffer must be an  
/// integer multiple of the size of the image. The recording wrap at the 
/// end of the buffer. Every time a picture is recorded, a message with the 
/// status CAMD_STATUS_CAPTURE_DONE and the pointer to the recorded image is
/// sent to the mailbox recorded with #camd_Open.
/// 
/// @param buffer Buffer where the images data will be stored
/// @param size Size of the buffer, in bytes. It must be a multiple of a single
/// picture size.
/// When the capture is done, a message with the Id #CAMD_MSG_HEADER_ID, and
/// the status field set to #CAMD_STATUS_CAPTURE_DONE is sent to the mailbox
/// passed as a parameter to the #camd_Open function.
/// @return #CAMD_ERR_NO, #CAMD_ERR_RESOURCE_BUSY.
// =============================================================================
PUBLIC CAMD_ERR_T camd_ImageStreamStart(CAMD_IMAGE_T buffer, UINT32 size,BOOL skipOddFrames)
{
    HAL_CAMERA_IRQ_CAUSE_T mask = {0,0,0,0};

    CAMD_PROFILE_FUNCTION_ENTER(camd_GetImage);
    CAMD_PROFILE_WINDOW_ENTER(camd_GettingPictureWin);
    CAMD_PROFILE_WINDOW_ENTER(camd_SyncWaitWin);
    g_camdInfo.camdMode         = CAMD_MODE_PREP_IMG_CAPTURE;
    g_camdInfo.imgBuffer        = buffer;
    g_camdInfo.imgBuffer        = buffer;
    g_camdInfo.streaming        = TRUE;
    g_camdInfo.skipOddFrames    = skipOddFrames;
    // TODO Assert sur la taille de la frame.
//    CAMD_ASSERT(size%g_camdInfo.nPixels == 0, "CAMD: buffer size %d is not a "
//            "mutliple of %d", size, g_camdInfo.nPixel);
    // Pixels are on 16 bits, we check the size of the buffer is big enough 
    // for at least two pictures.
    CAMD_ASSERT(size>=g_camdInfo.nPixels*2*2, "CAMD: buffer size %d is not a "
            "multiple of %d superior to 2", size, g_camdInfo.nPixels*2);
    g_camdInfo.bufferSize   = size;
    g_camdInfo.bufferOffset = 0;

    // Clear pending VSYNC IRQ and then enable it
    mask.vsync = 1;
    hal_CameraIrqSetMask(mask);

    // Enable Camera controller
    hal_CameraControllerEnable(TRUE);

    CAMD_PROFILE_FUNCTION_EXIT(camd_GetImage);
    return CAMD_ERR_NO;
}


// =============================================================================
// camd_ImageStreamStop
// =============================================================================
/// Stop the capture several images in a row in a buffer.
//
/// @return #CAMD_ERR_NO, #CAMD_ERR_RESOURCE_BUSY.
// =============================================================================
PUBLIC CAMD_ERR_T camd_ImageStreamStop(VOID)
{
    // Stop the streaming at the end of the current
    // frame.
    g_camdInfo.streaming = FALSE;
    return CAMD_ERR_NO;
}

// =============================================================================
// camd_GetInfo
// -----------------------------------------------------------------------------
/// Returns information from the camera as bitfields.
/// @param resolutions pointer to an integer where available resolutions will be
/// returned as bitfields: for example, if 
/// resolutions &(1<<CAMD_PARAM_RESOLUTION_QVGA)
/// is not 0, the QVGA resolution is supported.
/// @param parameters pointer to an integer where available parameters will be
/// returned as bitfields: for example, if parameters &(1<<CAMD_PARAM_EXPOSURE)
/// is not 0, the exposure can be configured on this camera.
/// @return #CAMD_ERR_NO, #CAMD_ERR_RESOURCE_BUSY.
// =============================================================================
PUBLIC CAMD_ERR_T camd_GetInfo(UINT32* resolutions, UINT32* parameters)
{
    CAMD_PROFILE_FUNCTION_ENTER(camd_GetInfo);
    *resolutions = (1<<CAMD_PARAM_RESOLUTION_VGA)
                | (1<<CAMD_PARAM_RESOLUTION_QVGA)
                | (1<<CAMD_PARAM_RESOLUTION_QQVGA);

    *parameters = 0;
    CAMD_PROFILE_FUNCTION_EXIT(camd_GetInfo);
    return CAMD_ERR_NO;
}

// ############
// Private
// ############

// =============================================================================
// camd_SetContrast
// -----------------------------------------------------------------------------
PRIVATE VOID camd_SetContrast(CAMD_PARAM_CONTRAST_T Contrast)
{
    CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetContrast %d ",Contrast);
    switch(Contrast)
    {
        case CAMD_PARAM_CONTRAST_0:
            camd_WriteOneReg(0x56, 0x60);      
            break;
        case CAMD_PARAM_CONTRAST_1:
            camd_WriteOneReg(0x56, 0x50);   
            break;
        case CAMD_PARAM_CONTRAST_2:
            camd_WriteOneReg(0x56, 0x40);      
            break;
        case CAMD_PARAM_CONTRAST_3:
            camd_WriteOneReg(0x56, 0x38);      
            break;
        case CAMD_PARAM_CONTRAST_4:
            camd_WriteOneReg(0x56, 0x30);     
            break;
        default:
            break;
    }
}
// =============================================================================
// camd_SetBrightness
// -----------------------------------------------------------------------------
PRIVATE VOID camd_SetBrightness(CAMD_PARAM_BRIGHTNESS_T Brightness)
{
    CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetBrightness %d ",Brightness);
    switch(Brightness)
    {
        case CAMD_PARAM_BRIGHTNESS_0:
            camd_WriteOneReg(0x55, 0xb0);
            break;
        case CAMD_PARAM_BRIGHTNESS_1:
            camd_WriteOneReg(0x55, 0x98);
            break;
        case CAMD_PARAM_BRIGHTNESS_2:
            camd_WriteOneReg(0x55, 0x00);
            break;
        case CAMD_PARAM_BRIGHTNESS_3:
            camd_WriteOneReg(0x55, 0x18);
            break;
        case CAMD_PARAM_BRIGHTNESS_4:
            camd_WriteOneReg(0x55, 0x30);
            break;
        default:
            camd_WriteOneReg(0x55, 0x30);
            break;
    }
}


// =============================================================================
// camd_SetFlash
// -----------------------------------------------------------------------------
/// Configure the camera flash light.
///
/// @param flash Flash configuration to apply.
// =============================================================================
PRIVATE VOID camd_SetFlash(CAMD_PARAM_FLASH_T flash)
{
    CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetFlash %d ", flash);
    switch(flash)
    {
        case CAMD_PARAM_FLASH_NONE:
            pmd_EnablePower(PMD_POWER_CAMERA_FLASH, FALSE);
            break;

        case CAMD_PARAM_FLASH_FORCED:
            pmd_EnablePower(PMD_POWER_CAMERA_FLASH, TRUE);
            break;
        
        case CAMD_PARAM_FLASH_AUTO:
        case CAMD_PARAM_FLASH_FILLIN:
        case CAMD_PARAM_FLASH_REDEYEREDUCE:
        default : 
            CAMD_TRACE(CAMD_INFO_TRC, 0,"camd Set Flash %d : Unsupported", flash);
            break;
    }
}

// =============================================================================
// camd_SetExposure
// -----------------------------------------------------------------------------
PRIVATE VOID camd_SetExposure(CAMD_PARAM_EXPOSURE_T Exposure)
{
    CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetExposure %d ",Exposure);
    switch(Exposure)
    {
       // Bits [15:10] are on register 0x7
       // Bits [9:2] are on register 0x10
       // Bits [1:0] are on register 0x4
       
       case  CAMD_PARAM_EXPOSURE_NEGATIVE_2:   //-2
            camd_WriteOneReg(0x7,0x04);
            camd_WriteOneReg(0x10,0x00);
            break;
       case   CAMD_PARAM_EXPOSURE_NEGATIVE_1: //-1
            camd_WriteOneReg(0x7,0x08);
            camd_WriteOneReg(0x10,0x00);
            break;
       case  CAMD_PARAM_EXPOSURE_AUTO:
            camd_WriteOneReg(0x7,0x10);
            camd_WriteOneReg(0x10,0x00);
            break;
       case   CAMD_PARAM_EXPOSURE_POSITIVE_1:  //+1
            camd_WriteOneReg(0x7,0x14);
            camd_WriteOneReg(0x10,0x00);
            break;
       case   CAMD_PARAM_EXPOSURE_POSITIVE_2:  //+2
            camd_WriteOneReg(0x7,0x18);
            camd_WriteOneReg(0x10,0x00);
            break; 
       default : 
            camd_WriteOneReg(0x7,0x18);
            camd_WriteOneReg(0x10,0x00);
            break;
    }
}
// =============================================================================
// camd_SetWhiteBalance
// -----------------------------------------------------------------------------
PRIVATE VOID camd_SetWhiteBalance(CAMD_PARAM_WHITE_BALANCE_T WhiteBalance)
{
    switch(WhiteBalance)
    {
        case CAMD_PARAM_WHITE_BALANCE_DAYLIGHT:   /*ÇçÌìÌ«Ñô¹â*/
            CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetWhiteBalance Daylight");
            camd_WriteOneReg(0x13,0xe5);
            camd_WriteOneReg(0x01,0x5a);
            camd_WriteOneReg(0x02,0x5c);
            camd_WriteOneReg(0x3b,0x0a);
            break;
        case CAMD_PARAM_WHITE_BALANCE_AUTO:  /*Auto */
            CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetWhiteBalance Auto");
            camd_WriteOneReg(0x13,0xe7);
            camd_WriteOneReg(0x3b,0x0a);
            camd_WriteOneReg(0x2d,0x00);
            camd_WriteOneReg(0x2e,0x00);
            break;
        case CAMD_PARAM_WHITE_BALANCE_OFFICE:   /*ÈÕ¹âµÆ*/
            CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetWhiteBalance Office");
            camd_WriteOneReg(0x13,0xe5);
            camd_WriteOneReg(0x01,0x84);
            camd_WriteOneReg(0x02,0x4c);
            camd_WriteOneReg(0x3b,0x0a);
            break;
        case CAMD_PARAM_WHITE_BALANCE_CLOUDY:  /*ÒõÌì*/
            CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetWhiteBalance Cloudy");
            camd_WriteOneReg(0x13,0xe5);
            camd_WriteOneReg(0x01,0x58);
            camd_WriteOneReg(0x02,0x60);
            camd_WriteOneReg(0x3b,0x0a);
            break;
        case CAMD_PARAM_WHITE_BALANCE_TUNGSTEN:  /*ÎÙË¿µÆ*/
            CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetWhiteBalance Tungsten");
            camd_WriteOneReg(0x13,0xe5);
            camd_WriteOneReg(0x01,0xcc);
            camd_WriteOneReg(0x02,0x40);
            camd_WriteOneReg(0x3b,0x0a);
            break;
        default : 
            CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetWhiteBalance Unkown");
            break;
    }
}
// =============================================================================
// camd_SetSpecialEffect
// -----------------------------------------------------------------------------
PRIVATE VOID camd_SetSpecialEffect(CAMD_PARAM_SPECIAL_EFFECT_T SpecialEffect)
{
    switch(SpecialEffect)
    {
        case CAMD_PARAM_SPECIAL_EFFECT_NORMAL:
            CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetSpecialEffect Normal");
            camd_WriteOneReg(0x3a,0x04);
            camd_WriteOneReg(0x67,0x80);
            camd_WriteOneReg(0x68,0x80);
            break;
        case CAMD_PARAM_SPECIAL_EFFECT_ANTIQUE:
            CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetSpecialEffect Antique");
            camd_WriteOneReg(0x3a,0x14);
            camd_WriteOneReg(0x67,0x04);
            camd_WriteOneReg(0x68,0xcf);
            break;
        case CAMD_PARAM_SPECIAL_EFFECT_REDISH:
            CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetSpecialEffect Redish");
            camd_WriteOneReg(0x3a,0x14);
            camd_WriteOneReg(0x67,0x8a);
            camd_WriteOneReg(0x68,0xf8);
            break;
        case CAMD_PARAM_SPECIAL_EFFECT_GREENISH:
            CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetSpecialEffect Greenish");
            camd_WriteOneReg(0x3a,0x14);
            camd_WriteOneReg(0x67,0x00);
            camd_WriteOneReg(0x68,0x40);
            break;
        case CAMD_PARAM_SPECIAL_EFFECT_BLUEISH:
            CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetSpecialEffect Blueish");
            camd_WriteOneReg(0x3a,0x14);
            camd_WriteOneReg(0x67,0xe0);
            camd_WriteOneReg(0x68,0x30);
            break;
        case CAMD_PARAM_SPECIAL_EFFECT_BLACKWHITE:
            CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetSpecialEffect BW");
            camd_WriteOneReg(0x3a,0x14);
            camd_WriteOneReg(0x67,0x80);
            camd_WriteOneReg(0x68,0x80);
            break;
        case CAMD_PARAM_SPECIAL_EFFECT_NEGATIVE:
            CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetSpecialEffect Negative");
            camd_WriteOneReg(0x3a,0x24);
            camd_WriteOneReg(0x67,0x80);
            camd_WriteOneReg(0x68,0x80);
            break;
        case CAMD_PARAM_SPECIAL_EFFECT_BLACKWHITE_NEGATIVE:
            CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetSpecialEffect BW Negative");
            camd_WriteOneReg(0x3a,0x34);
            camd_WriteOneReg(0x67,0x80);
            camd_WriteOneReg(0x68,0x80);
            break;
        default: 
            break;
    }
}
// =============================================================================
// camd_EnableNightMode
// -----------------------------------------------------------------------------
// =============================================================================
PRIVATE VOID camd_EnableNightMode(UINT16 NightMode)
{
    UINT8 readregval;
  
    CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_EnableNightMode %d",NightMode);
    if(NightMode==CAMD_PARAM_NIGHT_MODE_OFF)
    {
        readregval=camd_ReadReg(0x3b);
        camd_WriteOneReg(0x3b,readregval &(~0x80));
    }
    else
    {
        readregval=camd_ReadReg(0x3b);
        camd_WriteOneReg(0x3b,readregval |0x80);
    }
}
// =============================================================================
// camd_SetResolution
// -----------------------------------------------------------------------------
// =============================================================================
PRIVATE VOID camd_SetResolution(CAMD_PARAM_RESOLUTION_T Resolution)
{
    CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_SetResolution %d",Resolution);
    switch (Resolution)
    {
        case CAMD_PARAM_RESOLUTION_VGA:
            g_camdInfo.nPixels = 640*480;
            camd_SendRgSeq(RG_VidFormatVga,  (sizeof(RG_VidFormatVga) / sizeof(CAMD_REG_T)));
            break;

        case CAMD_PARAM_RESOLUTION_QVGA:
            g_camdInfo.nPixels = 320*240;
            camd_SendRgSeq(RG_VidFormatQvga,  (sizeof(RG_VidFormatQvga) / sizeof(CAMD_REG_T)));
            break;

        case CAMD_PARAM_RESOLUTION_QQVGA:
            g_camdInfo.nPixels = 160*120;
            camd_SendRgSeq(RG_VidFormatQqvga,  (sizeof(RG_VidFormatQqvga) / sizeof(CAMD_REG_T)));
            break;
        default:
            CAMD_ASSERT( FALSE, "CAMD unsupported resolution");
            break;
    }
}
// =============================================================================
// camd_SetFrameRate
// -----------------------------------------------------------------------------
// The Framerate is directly related to the PCLK
// For VGA the transfer of one picture is 710x784=799680 cycles of PCLK
// PCLK value is XCLK / 2 / (Reg11[5:0]+1)
// XCLK must be between 12 and 48Mhz for this sensor
// XCLK = 156 / (CLK_CAMERA_OUT_DIV+2)
//  2<=CLK_CAMERA_OUT_DIV<=11 to respect the range 12MHz - 48 MHz
// -----------------------------------------------------------------------------------
// Reg11=1
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (2 + 2) / 4 / 799680 =  12,19 Frame/s
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (3 + 2) / 4 / 799680 =   9,75 Frame/s
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (4 + 2) / 4 / 799680 =   8,13 Frame/s
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (5 + 2) / 4 / 799680 =   6.97 Frame/s
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (6 + 2) / 4 / 799680 =   6,10 Frame/s
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (7 + 2) / 4 / 799680 =   5,42 Frame/s
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (8 + 2) / 4 / 799680 =   4.88 Frame/s
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (9 + 2) / 4 / 799680 =   4,43 Frame/s
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (10 + 2)/ 4 / 799680 =   4,06 Frame/s
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (11 + 2)/ 4 / 799680 =   3,75 Frame/s
// -----------------------------------------------------------------------------------
// Reg11=0
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (2 + 2) / 2 / 799680 =  24,38 Frame/s
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (3 + 2) / 2 / 799680 =  19,51 Frame/s
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (4 + 2) / 2 / 799680 =  16,26 Frame/s
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (5 + 2) / 2 / 799680 =  13,93 Frame/s
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (6 + 2) / 2 / 799680 =  12,19 Frame/s
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (7 + 2) / 2 / 799680 =  10,84 Frame/s
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (8 + 2) / 2 / 799680 =  9,75 Frame/s
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (9 + 2) / 2 / 799680 =  8,87 Frame/s
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (10 + 2)/ 2 / 799680 =  8,13 Frame/s
//         - CLK_CAMERA_OUT_DIV=0 => 156000000 / (11 + 2)/ 2 / 799680 =  7,50 Frame/s
// =============================================================================
VOID camd_SetFrameRate(UINT16 FrameRate)
{
    UINT16 rate;

    if (g_camdInfo.nPixels == (640*480))
    {
        if (FrameRate>=12)
        {
            hal_CameraSetupClockDivider(4);
            camd_WriteOneReg(0x11,1);
            rate=12;
        }
        else if (FrameRate>=9)
        {
            hal_CameraSetupClockDivider(5);
            camd_WriteOneReg(0x11,1);
            rate=9;
        }
        else if (FrameRate>=8)
        {
            hal_CameraSetupClockDivider(6);
            camd_WriteOneReg(0x11,1);
            rate=8;
        }
        else if (FrameRate==7)
        {
            hal_CameraSetupClockDivider(7);
            camd_WriteOneReg(0x11,1);
            rate=7;
        }
        else if (FrameRate==6)
        {
            hal_CameraSetupClockDivider(8);
            camd_WriteOneReg(0x11,1);
            rate=6;
        }
        else if (FrameRate==5)
        {
            hal_CameraSetupClockDivider(9);
            camd_WriteOneReg(0x11,1);
            rate=6;
        }
        else 
        {
            hal_CameraSetupClockDivider(10);
            camd_WriteOneReg(0x11,1);
            rate=4;
        }
        CAMD_TRACE(CAMD_INFO_TRC, 0, "camd_SetFrameRate VGA : requested %dfps, actual %dfps",FrameRate,rate);
    }
    else if (g_camdInfo.nPixels == (320*240))
    {
        if (FrameRate>=25)
        {
            hal_CameraSetupClockDivider(7);
            camd_WriteOneReg(0x11,0x80);
            rate=27;
        }
        else if (FrameRate>=24)
        {
            hal_CameraSetupClockDivider(4);
            camd_WriteOneReg(0x11,0);
            rate=24;
        }
        else if (FrameRate>=19)
        {
            hal_CameraSetupClockDivider(5);
            camd_WriteOneReg(0x11,0);
            rate=19;
        }
        else if (FrameRate>=16)
        {
            hal_CameraSetupClockDivider(6);
            camd_WriteOneReg(0x11,0);
            rate=16;
        }
        else if (FrameRate>=13)
        {
            hal_CameraSetupClockDivider(7);
            camd_WriteOneReg(0x11,0);
            rate=14;
        }
        else if (FrameRate==12)
        {
            hal_CameraSetupClockDivider(8);
            camd_WriteOneReg(0x11,0);
            rate=12;
        }
        else if (FrameRate>=10)
        {
            hal_CameraSetupClockDivider(9);
            camd_WriteOneReg(0x11,0);
            rate=11;
        }
        else if (FrameRate==9)
        {
            hal_CameraSetupClockDivider(10);
            camd_WriteOneReg(0x11,0);
            rate=9;
        }
        else if (FrameRate==8)
        {
            hal_CameraSetupClockDivider(6);
            camd_WriteOneReg(0x11,1);
            rate=8;
        }
        else if (FrameRate==8)
        {
            hal_CameraSetupClockDivider(6);
            camd_WriteOneReg(0x11,1);
            rate=8;
        }
        else if (FrameRate==7)
        {
            hal_CameraSetupClockDivider(7);
            camd_WriteOneReg(0x11,1);
            rate=7;
        }
        else if (FrameRate==6)
        {
            hal_CameraSetupClockDivider(8);
            camd_WriteOneReg(0x11,1);
            rate=6;
        }
        else if (FrameRate==5)
        {
            hal_CameraSetupClockDivider(10);
            camd_WriteOneReg(0x11,1);
            rate=5;
        }
        else 
        {
            hal_CameraSetupClockDivider(12);
            camd_WriteOneReg(0x11,1);
            rate=4;
        }
        CAMD_TRACE(CAMD_INFO_TRC, 0, "camd_SetFrameRate QVGA : requested %dfps, actual %dfps",FrameRate,rate);
    }
    else
    {
#if 1
        if (FrameRate>=25)
        {
            hal_CameraSetupClockDivider(7);
            camd_WriteOneReg(0x11,0x80);
            rate=27;
        }
        else if (FrameRate>=24)
        {
            hal_CameraSetupClockDivider(4);
            camd_WriteOneReg(0x11,0);
            rate=24;
        }
        else if (FrameRate>=19)
        {
            hal_CameraSetupClockDivider(5);
            camd_WriteOneReg(0x11,0);
            rate=19;
        }
        else if (FrameRate>=16)
        {
            hal_CameraSetupClockDivider(6);
            camd_WriteOneReg(0x11,0);
            rate=16;
        }
        else if (FrameRate>=13)
        {
            hal_CameraSetupClockDivider(7);
            camd_WriteOneReg(0x11,0);
            rate=14;
        }
        else if (FrameRate==12)
        {
            hal_CameraSetupClockDivider(8);
            camd_WriteOneReg(0x11,0);
            rate=12;
        }
        else if (FrameRate>=10)
        {
            hal_CameraSetupClockDivider(9);
            camd_WriteOneReg(0x11,0);
            rate=11;
        }
        else if (FrameRate==9)
        {
            hal_CameraSetupClockDivider(10);
            camd_WriteOneReg(0x11,0);
            rate=9;
        }
        else if (FrameRate==8)
        {
            hal_CameraSetupClockDivider(6);
            camd_WriteOneReg(0x11,1);
            rate=8;
        }
        else if (FrameRate==7)
        {
            hal_CameraSetupClockDivider(7);
            camd_WriteOneReg(0x11,1);
            rate=7;
        }
        else if (FrameRate==6)
        {
            hal_CameraSetupClockDivider(8);
            camd_WriteOneReg(0x11,1);
            rate=6;
        }
        else if (FrameRate==5)
        {
            hal_CameraSetupClockDivider(10);
            camd_WriteOneReg(0x11,1);
            rate=5;
        }
        else 
        {
            hal_CameraSetupClockDivider(12);
            camd_WriteOneReg(0x11,1);
            rate=4;
        }
#else
            hal_CameraSetupClockDivider(12);
#endif
        CAMD_TRACE(CAMD_INFO_TRC, 0, "camd_SetFrameRate QQVGA : requested %dfps, actual %dfps",FrameRate,rate);
    }
    }

// =============================================================================
// camd_WriteOneReg
// -----------------------------------------------------------------------------
PRIVATE VOID  camd_WriteOneReg(UINT8 Addr, UINT8 Data)
{
    hal_I2cSendRawByte(((g_camdInfo.i2caddress<<1) & 0xfe), PHASE3_WR_PH1);
    hal_I2cSendRawByte(Addr, PHASE3_WR_PH2);
    hal_I2cSendRawByte(Data, PHASE3_WR_PH3);
}

// =============================================================================
// camd_SendRgSeq
// -----------------------------------------------------------------------------
PRIVATE VOID camd_SendRgSeq(const CAMD_REG_T *RegList, UINT16 Len)
{
    UINT16 RegCount;

    for (RegCount = 0; RegCount < Len; ++RegCount)
    {
        camd_WriteReg(RegList[RegCount]);
    }
}

// =============================================================================
// camd_WriteReg
// -----------------------------------------------------------------------------
VOID camd_WriteReg(CAMD_REG_T RegWrite)
{
    hal_I2cSendRawByte(((g_camdInfo.i2caddress<<1) & 0xfe), PHASE3_WR_PH1);
    hal_I2cSendRawByte(RegWrite.Addr, PHASE3_WR_PH2);
    hal_I2cSendRawByte(RegWrite.Data, PHASE3_WR_PH3);

    sxr_Sleep(RegWrite.Wait MILLI_SECOND);
}

// =============================================================================
// camd_ReadReg
// -----------------------------------------------------------------------------
UINT8 camd_ReadReg(UINT8 Addr)
{
   if (hal_I2cSendRawByte(((g_camdInfo.i2caddress<<1) & 0xfe), PHASE2_WR_PH1) == HAL_ERR_RESOURCE_BUSY)   
    {
        return(0x1f);
    }
    if (hal_I2cSendRawByte(Addr, PHASE2_WR_PH2) == HAL_ERR_RESOURCE_BUSY)
    {
        return(0x1e);
    }

   if (hal_I2cSendRawByte(((g_camdInfo.i2caddress<<1) | 0x01), PHASE2_RD_PH1) == HAL_ERR_RESOURCE_BUSY)
  
    {
        return(0x1d);
    }

    return(hal_I2cReadRawByte(PHASE2_RD_PH2));
}

// =============================================================================
// camd_InteruptHandler
// -----------------------------------------------------------------------------
VOID camd_InteruptHandler(HAL_CAMERA_IRQ_CAUSE_T cause)
{
    CAMD_MSG_T* msg;
    HAL_CAMERA_IRQ_CAUSE_T mask = {0,0,0,0};
    HAL_CAMERA_XFER_STATUS_T status;
    CAMD_MODE_T nextMode = CAMD_MODE_IDLE;

     switch (g_camdInfo.camdMode)
     {
        case CAMD_MODE_PREP_IMG_CAPTURE:
            // This is the first VSYNC interrupt
            if (g_camdInfo.camdDrop==0)
            {
                CAMD_PROFILE_WINDOW_EXIT(camd_SyncWaitWin);
                // Start IFC transfer
                // 16 bits per pixels
                g_camdInfo.ifcChan = hal_CameraStartXfer(g_camdInfo.nPixels*2, 
                                ((UINT8*)g_camdInfo.imgBuffer) + g_camdInfo.bufferOffset);
                if (0xFF == g_camdInfo.ifcChan)
                {
                    // No channel available at this time
                    // Wait for the next VSYNC to do the capture
                    CAMD_TRACE(CAMD_INFO_TRC, 0, 
                         "CAMD IRQ HANDLER START CAPTURE : NO FREE IFC CHANNEL, RETRYING @ NEXT VSYNC");
                    // Enable Vsync interrupt
                    mask.vsync= 1;
                    hal_CameraIrqSetMask(mask);
                }
                else
                {
                    CAMD_PROFILE_WINDOW_ENTER(camd_FetchData);
                    //CAMD_TRACE(CAMD_INFO_TRC, 0, "CAMD IRQ HANDLER START CAPTURE");

                    if (g_camdInfo.streaming == TRUE)
                    {
                        // We will handle the following vsync as a stream
                        // of pictures.
                        g_camdInfo.camdMode = CAMD_MODE_STREAM_CAPTURE;
                    }
                    else
                    {
                        g_camdInfo.camdMode = CAMD_MODE_IMG_CAPTURE;
                    }

                    // Enable Vsync and Overflow interrupt
                    mask.vsync=1;
                    mask.overflow=1;
                    hal_CameraIrqSetMask(mask);
                }
            }
            else
            {
                CAMD_TRACE(CAMD_INFO_TRC, 0,"camd_GetImage_Skip : Skip %d ",g_camdInfo.camdDrop);
                g_camdInfo.camdDrop--;
                // Enable Vsync interrupt
                mask.vsync=1;
                hal_CameraIrqSetMask(mask);
            }
            break;

        case CAMD_MODE_IMG_CAPTURE:
        case CAMD_MODE_STREAM_CAPTURE:
            // Disable all irqs
            hal_CameraIrqSetMask(mask);

            msg = (CAMD_MSG_T*) sxr_Malloc(sizeof(CAMD_MSG_T));
            msg->headerId = CAMD_MSG_HEADER_ID;

            // This will record that the transfer is finished and reset the channel
            // if needed. The controller is stopped if we don't want to stream 
            // the next image.
            status = hal_CameraStopXfer(!g_camdInfo.streaming);     

            if (cause.overflow)
            {
                // Show the overflow on the profiling.
                CAMD_PROFILE_PULSE(camd_OverflowPulse);
                if (status==XFER_NOT_FINISHED)
                {
                    CAMD_TRACE(CAMD_INFO_TRC, 0, "CAMD IRQ HANDLER OVERFLOW");
                    msg->status = CAMD_STATUS_CAPTURE_ERROR;
                    msg->imgPtr = (UINT32*)(((UINT8*)g_camdInfo.imgBuffer) + g_camdInfo.bufferOffset);
                    // Try again from the prepimage stage  
                    nextMode = CAMD_MODE_PREP_IMG_CAPTURE;
                }
                else
                {
                    CAMD_TRACE(CAMD_INFO_TRC, 0, "CAMD IRQ HANDLER OVERFLOW BUT IFC XFER COMPLETE");
                    CAMD_TRACE(CAMD_INFO_TRC, 0, "SENSOR RESOLUTION IS PROBABLY WRONG");
            msg->status = CAMD_STATUS_CAPTURE_DONE;
                    msg->imgPtr = (UINT32*)(((UINT8*)g_camdInfo.imgBuffer) + g_camdInfo.bufferOffset);
                    if (g_camdInfo.streaming)
                    {
                        if (g_camdInfo.skipOddFrames)
                        {
                            nextMode = CAMD_MODE_PREP_IMG_CAPTURE;
                        }
                        else
                        {
                            nextMode = CAMD_MODE_STREAM_CAPTURE;
                        }
                    }
                    else
                    {
                        nextMode = CAMD_MODE_IDLE;
                    }
                }
            } 
            else if (cause.vsync)
            {
                CAMD_TRACE(CAMD_INFO_TRC, 0, "CAMD IRQ HANDLER CAPTURE DONE");
                msg->status = CAMD_STATUS_CAPTURE_DONE;
                msg->imgPtr = (UINT32*)(((UINT8*)g_camdInfo.imgBuffer) + g_camdInfo.bufferOffset);
                if (g_camdInfo.streaming)
                {
                    if (g_camdInfo.skipOddFrames)
                    {
                        CAMD_TRACE(CAMD_INFO_TRC, 0, "SKIP NEXT");
                        nextMode = CAMD_MODE_PREP_IMG_CAPTURE;
                    }
                    else
                    {
                        nextMode = CAMD_MODE_STREAM_CAPTURE;
                    }
                }
                else
                {
                    nextMode = CAMD_MODE_IDLE;
                }
            }
            else
            {
                CAMD_TRACE(CAMD_INFO_TRC, 0, "CAMD IRQ HANDLER : Cause not handled");
                if (g_camdInfo.streaming)
                {
                    nextMode = CAMD_MODE_PREP_IMG_CAPTURE;
                }
                else
                {
                    nextMode = CAMD_MODE_IDLE;
                }
            }


            CAMD_PROFILE_WINDOW_EXIT(camd_FetchData);
            CAMD_PROFILE_WINDOW_EXIT(camd_GettingPictureWin);
            g_camdInfo.ifcChan = 0xff;

            sxr_Send(msg, g_camdInfo.frameDoneMbx, SXR_SEND_MSG);
        
            // Configure state during the next frame.
            CAMD_TRACE(CAMD_INFO_TRC, 0, "CAMD IRQ HANDLER: next mode:%d", nextMode);
            switch (nextMode)
            {
                case CAMD_MODE_IDLE:
                    // All irqs are disabled.
                    g_camdInfo.camdMode = CAMD_MODE_IDLE;
                    g_camdInfo.bufferOffset = 0;
            break;

                case CAMD_MODE_PREP_IMG_CAPTURE:
                    // Synchronize on the VSYNC and try again.
                    g_camdInfo.camdMode = CAMD_MODE_PREP_IMG_CAPTURE;
                    mask.vsync = 1;
                    hal_CameraIrqSetMask(mask);
                    break;

                case CAMD_MODE_STREAM_CAPTURE:
                    // Start fetching the next picture
                    // Start IFC transfer
                    // 16 bits per pixels
                    g_camdInfo.bufferOffset += g_camdInfo.nPixels*2;
                    // Check that a whole picture can fit in the buffer.
                    if (g_camdInfo.bufferOffset+g_camdInfo.nPixels*2 > g_camdInfo.bufferSize)
                    {
                        // Let's wrap in the picture stream buffer
                        g_camdInfo.bufferOffset = 0;
                    }

                    g_camdInfo.ifcChan = hal_CameraStartXfer(g_camdInfo.nPixels*2,
                                    ((UINT8*)g_camdInfo.imgBuffer) + g_camdInfo.bufferOffset);
                    
                    if (0xFF == g_camdInfo.ifcChan)
                    {
                        // No channel available at this time
                        // Wait for the next VSYNC to start the capture
                        CAMD_TRACE(CAMD_INFO_TRC, 0, 
                             "CAMD IRQ HANDLER START CAPTURE : NO FREE IFC CHANNEL, RETRYING @ NEXT VSYNC");
                        // Go back in the prep capture mode
                        g_camdInfo.camdMode = CAMD_MODE_PREP_IMG_CAPTURE;
                        // Enable Vsync interrupt
                        mask.vsync=1;
                        hal_CameraIrqSetMask(mask);
                    }
                    else
                    {
                        CAMD_PROFILE_WINDOW_ENTER(camd_FetchData);
                        CAMD_TRACE(CAMD_INFO_TRC, 0, "CAMD IRQ HANDLER START CAPTURE");
                        // We will handle the following vsync as a stream
                        // Enable Vsync and Overflow interrupt
                        g_camdInfo.camdMode = CAMD_MODE_STREAM_CAPTURE;
                        mask.vsync=1;
                        mask.overflow=1;
                        hal_CameraIrqSetMask(mask);
                    }
                    break;

                default:
                    break;

            }
            break;


        default:
            CAMD_TRACE(CAMD_INFO_TRC, 0, "CAMD IRQ HANDLER Unknown State");
            msg = (CAMD_MSG_T*) sxr_Malloc(sizeof(CAMD_MSG_T));
            msg->status = CAMD_STATUS_CAPTURE_ERROR;
            msg->imgPtr = NULL;
            sxr_Send(msg, g_camdInfo.frameDoneMbx, SXR_SEND_MSG);
            break;
     }

}



