#ifndef _CAMERA_M_H_
#define _CAMERA_M_H_

// =============================================================================
// CAM_CONFIG_T
// -----------------------------------------------------------------------------
/// That type is used to describe the configuration specific to a given target
/// for the CAM driver. It defines all the control signals for each 
/// target and the GPIO assignments.
// =============================================================================
//typedef struct
//{
//
//} CAM_CONFIG_T;




typedef enum
{
    CAM_FLASH_NONE = 0x00,
    CAM_FLASH_AUTO = 0x01,
    CAM_FLASH_FORCED = 0x02,
    CAM_FLASH_FILLIN = 0x04,
    CAM_FLASH_REDEYEREDUCE = 0x08
} CAM_FLASH_T;

typedef enum
{
    CAM_EXPOSURE_AUTO = 0x00,
    CAM_EXPOSURE_NEGATIVE_2 = 0x01,
    CAM_EXPOSURE_NEGATIVE_1 = 0x02,
    CAM_EXPOSURE_POSITIVE_1 = 0x04,
    CAM_EXPOSURE_POSITIVE_2 = 0x08
} CAM_EXPOSURE_T;

typedef enum
{
    CAM_WB_AUTO = 0x00,
    CAM_WB_DAYLIGHT= 0x01,
    CAM_WB_CLOUDY= 0x02,
    CAM_WB_TUNGSTEN = 0x04,
    CAM_WB_OFFICE = 0x08
} CAM_WHITEBALANCE_T;

typedef enum
{
    CAM_SPE_NORMAL = 0x00,
    CAM_SPE_ANTIQUE= 0x01,
    CAM_SPE_REDISH= 0x02,
    CAM_SPE_GREENISH = 0x04,
    CAM_SPE_BLUEISH = 0x08,
    CAM_SPE_BLACKWHITE = 0x10,
    CAM_SPE_NEGATIVE =0x11,
    CAM_SPE_BLACKWHITE_NEGATIVE =0x12
} CAM_SPECIALEFFECT_T;

// For now only support RGB565 format
typedef enum
{
//    CAM_FORMAT_RGBNORM,
    CAM_FORMAT_RGB565
//    CAM_FORMAT_RGB555,
//    CAM_FORMAT_YUV,
} CAM_FORMAT_T;

// For now, don't do CIF
typedef enum
{
    CAM_NPIX_VGA = (640*480),       // 640x480
    CAM_NPIX_QVGA = (320*240),      // 320x240
    CAM_NPIX_QQVGA = (160*120)      // 160x120
} CAM_SIZE_T;

typedef UINT16  *CAM_IMAGE_T;

typedef VOID (*CAM_IRQ_HANDLER_T)(UINT8 id, UINT8 ActiveBufNum);

//PUBLIC VOID camera_CameraInfo(CAM_INFO_T *Info);
//

PUBLIC VOID camera_Reserve(CAM_IRQ_HANDLER_T FrameReady);
// =============================================================================
// camera_Reserve(CAM_FRAME_READY_P FrameReady)
// =============================================================================
/// Reserve the camera resource.  Opens the I2C and puts the camera module into
/// power down and reset.  Enables the camera interface.  Must be called before
/// camera_PowerOn()  FrameReady is the callback function that will be called
/// with the frame number parameter of the last frame that was received
// =============================================================================


PUBLIC VOID camera_Release(VOID);


// =============================================================================
// camera_PowerOn(VOID)
// =============================================================================
/// Power on the camera.  Camera must have first been initialized and
/// reserved by calling camera_Reserve()
// =============================================================================
PUBLIC VOID camera_PowerOn(CAM_SIZE_T FrameSize, CAM_FORMAT_T Format);

PUBLIC VOID camera_PowerOff(VOID);


// =============================================================================
// camera_PrepareImageCaptureL(VOID)
// =============================================================================
/// Configures the camera to capture a single image with the format described
/// in the ImageFormat structure.
// =============================================================================
PUBLIC void camera_PrepareImageCaptureL(CAM_FORMAT_T ImageFormat, CAM_SIZE_T SizeIndex, CAM_IMAGE_T Buffer);

PUBLIC VOID camera_CaptureImage(VOID);


PUBLIC VOID camera_SetZoomFactorL(UINT32 ZoomFactor);
PUBLIC UINT32 camera_ZoomFactor(VOID);
PUBLIC VOID camera_SetDigitalZoomFactorL(UINT32 DigZoomFactor);
PUBLIC UINT32 camera_DigitalZoomFactor(VOID);
PUBLIC VOID camera_SetContrastL(UINT32 Contrast);
PUBLIC UINT32 camera_Contrast(VOID);
PUBLIC VOID camera_SetBrightnessL(UINT32 Brightness);
PUBLIC UINT32 camera_Brightness(VOID);
PUBLIC VOID camera_SetFlashL(CAM_FLASH_T Flash);
PUBLIC CAM_FLASH_T camera_Flash(VOID);
PUBLIC VOID camera_SetExposureL(CAM_EXPOSURE_T Exposure);
PUBLIC CAM_EXPOSURE_T camera_Exposure(VOID);
PUBLIC VOID camera_SetWhiteBalanceL(CAM_WHITEBALANCE_T WhiteBalance);
PUBLIC CAM_WHITEBALANCE_T camera_WhiteBalance(VOID);
PUBLIC VOID camera_SetSpecialEffect(CAM_SPECIALEFFECT_T SpecialEffect);
PUBLIC CAM_SPECIALEFFECT_T camera_SpecialEffect(VOID);
PUBLIC VOID camera_SetNightMode(UINT32 NightMode);
PUBLIC UINT32 camera_NightMode(VOID);

// This is not _really_ Direct, but it does implement a viewfinder function
PUBLIC VOID camera_StartViewFinderDirectL();
PUBLIC VOID camera_StopViewFinder(VOID);
PUBLIC BOOL camera_ViewFinderActive(VOID);

PUBLIC VOID camera_SetViewFinderMirror(BOOL Mirror);
PUBLIC BOOL camera_ViewFinderMirror(VOID);
PUBLIC VOID camera_CancelCaptureImage(VOID);
PUBLIC VOID camera_EnumerateCaptureSizes(CAM_SIZE_T *Size, UINT32 SizeIndex, CAM_FORMAT_T Format);
PUBLIC VOID camera_PrepareVideoCaptureL(CAM_FORMAT_T Format, UINT32 SizeIndex, UINT32 RateIndex, UINT32 BuffersToUse, UINT32 FramesPerBuffer);
PUBLIC VOID camera_StartVideoCapture(VOID);
PUBLIC VOID camera_StopVideoCapture(VOID);
PUBLIC BOOL camera_VideoCaptureActive(VOID);
PUBLIC VOID camera_EnumerateVideoFrameSizes(CAM_SIZE_T Size, UINT32 SizeIndex, CAM_FORMAT_T Format);
PUBLIC VOID camera_EnumerateVideoFrameRates();
PUBLIC VOID camera_GetFrameSize(CAM_SIZE_T Size);
PUBLIC VOID camera_SetFrameRate(UINT32 FrameRate);
PUBLIC UINT32 camera_FrameRate(VOID);
PUBLIC UINT32 camera_BuffersInUse(VOID);
PUBLIC UINT32 camera_FramesPerBuffer(VOID);
PUBLIC UINT8 camera_GetId(VOID);

#endif // _CAMERA_M_H_
