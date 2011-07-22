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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/include/hal_camera.h $ //
//    $Author: huazeng $                                                       // 
//    $Date: 2011-01-11 20:30:56 +0800 (Tue, 11 Jan 2011) $                     //   
//    $Revision: 5305 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_camera.h                                                        //
///                                                                           //
/// This file decribes the camera device driver API.                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#ifndef _HAL_CAMERA_H_
#define _HAL_CAMERA_H_

#include "cs_types.h"

// =============================================================================
// HAL_CAMERA_IRQ_CAUSE_T
// -----------------------------------------------------------------------------
/// Describes the camera interrupt cause.
// =============================================================================
typedef struct
{
    /// An overflow occured.
    UINT8 overflow:1;

    /// Frame start interrupt cause.
    UINT8 fstart:1;

    /// Frame end interrupt cause.
    UINT8 fend:1;

    /// Vsync interrupt cause.
    UINT8 vsync:1;
} HAL_CAMERA_IRQ_CAUSE_T;


// =============================================================================
// HAL_CAMERA_ENDIANESS_T
// -----------------------------------------------------------------------------
/// Camera endiannes. Defines the swapping operation done on any 32 bits of 
/// received data from the camera data flow, to get pixels in the proper order.
// =============================================================================
typedef enum
{
    /// No reordering.
    NO_SWAP=0,

    /// Bytes order is inverted.
    BYTE_SWAP=1,

    /// 16 bits data are swapped.
    HALF_WORD_SWAP=2,

    /// Bits are swapped.
    BIT_SWAP=3
} HAL_CAMERA_ENDIANESS_T;


// =============================================================================
// HAL_CAMERA_XFER_STATUS_T
// -----------------------------------------------------------------------------
/// Describes the status returned by the #hal_CameraStopXfer() function.
// =============================================================================
typedef enum
{
    /// The transfer finished properly, the whole image 
    /// has been received.
    XFER_SUCCESS,
    
    /// The transfer has not finished.
    XFER_NOT_FINISHED,

    /// The transfer finished, but an overflow occured.
    /// There is probably a misconfiguration between
    /// the number of pixels expected and the camera
    /// resolution configuration.
    XFER_FINISHED_WITH_OVERFLOW,

    /// There was no IFC configured for a transfer.
    /// #hal_CameraStopXfer() was called before 
    /// #hal_CameraStartXfer() ?
    XFER_ERR_WRONG_PARAM
} HAL_CAMERA_XFER_STATUS_T;

// =============================================================================
// HAL_CAMERA_ROW_RATIO_T
// -----------------------------------------------------------------------------
/// Describes the row pixel decimation ratio.
// =============================================================================
typedef enum
{
    /// Original size, decimation disabled, 1/1.
    ROW_RATIO_1_1,
    
    /// Row pixel decimation 1/2.
    ROW_RATIO_1_2,

    /// Row pixel decimation 1/3.
    ROW_RATIO_1_3,

    /// Row pixel decimation 1/4.
    ROW_RATIO_1_4,
} HAL_CAMERA_ROW_RATIO_T;


// =============================================================================
// HAL_CAMERA_COL_RATIO_T
// -----------------------------------------------------------------------------
/// Describes the column pixel decimation ratio.
// =============================================================================
typedef enum
{
    /// Original size, decimation disabled, 1/1.
    COL_RATIO_1_1,
    
    /// Column pixel decimation 1/2.
    COL_RATIO_1_2,

    /// Column pixel decimation 1/3.
    COL_RATIO_1_3,

    /// Column pixel decimation 1/4.
    COL_RATIO_1_4,
} HAL_CAMERA_COL_RATIO_T;


// =============================================================================
// HAL_CAMERA_CFG_T
// -----------------------------------------------------------------------------
/// Camera configuration. This structure is used by #hal_CameraOpen() to 
/// configure how the chip and the camera sensors are connected, and which 
/// camera sensor to use in case there would be two.
// =============================================================================
typedef struct
{
    /// Reset line polarity. Active high if \c TRUE.
    BOOL rstActiveH;
    /// Power DOWN line polarity. Active high if \c TRUE.
    BOOL pdnActiveH;
    /// If \c TRUE, all even frame are skipped.
    BOOL dropFrame;
    // Enable window crop
    BOOL cropEnable;
    /// Clock divider to apply to produce the camera clock 
    /// from the 156 MHz clock.
    UINT8 camClkDiv;
    UINT8 reOrder;
    /// Describes the swapping operation to apply to data 
    /// received from the camera sensor.
    HAL_CAMERA_ENDIANESS_T endianess;
    /// Id (0 or 1) of the camera to use.
    UINT32 camId;
    /// Row pixel decimation ratio
    HAL_CAMERA_ROW_RATIO_T rowRatio;
    /// Column pixel decimation ratio
    HAL_CAMERA_COL_RATIO_T colRatio;
    // start pixel of cropped window
    UINT16 dstWinColStart;
    // end pixel of cropped window
    UINT16 dstWinColEnd;
    // start line of cropped window
    UINT16 dstWinRowStart;
    // end line of cropped window
    UINT16 dstWinRowEnd;
} HAL_CAMERA_CFG_T;



// =============================================================================
// HAL_CAMERA_IRQ_HANDLER_T
// -----------------------------------------------------------------------------
/// Type for the user function called when a camera interrupt occurs.
// =============================================================================
typedef VOID (*HAL_CAMERA_IRQ_HANDLER_T)(HAL_CAMERA_IRQ_CAUSE_T cause);

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
PUBLIC VOID hal_CameraOpen(HAL_CAMERA_CFG_T* camConfig);


// =============================================================================
// hal_CameraClose
// -----------------------------------------------------------------------------
/// Power off the camera sensor by setting the PowerDown bit.
/// Resets the camera sensor by enabling the Camera Reset bit.
/// This function can only be called after the camera transfer has been stopped
/// by a call to #hal_CameraStopXfer().
// =============================================================================
PUBLIC VOID hal_CameraClose(VOID);


// =============================================================================
// hal_CameraReset
// -----------------------------------------------------------------------------
/// Controls the Reset line going to the sensor
/// Uses the rstActiveH field of the configuration structure to know the polarity
/// Should not be called before the open
/// @param reset \c TRUE, the sensor is in reset
///              \c FALSE, the sensor is out of reset
// =============================================================================
PUBLIC VOID hal_CameraReset(BOOL reset);


// =============================================================================
// hal_CameraPowerOn
// -----------------------------------------------------------------------------
/// Controls the PDN line going to the sensor
/// Uses the pdnActiveH field of the configuration structure to know the polarity
/// Should not be called before the open
/// @param powerDown \c TRUE, the sensor is in Power Down
///              \c FALSE, the sensor is out of Power Down
// =============================================================================
PUBLIC VOID hal_CameraPowerDown(BOOL powerDown);


// =============================================================================
// hal_CameraControllerEnable
// -----------------------------------------------------------------------------
/// Turns the Camera controller module On and Off
/// Should not be called before the open
/// @param enable \cTRUE, the Controller is active
///               FALSE, the Controller is turned off
// =============================================================================
PUBLIC VOID hal_CameraControllerEnable(BOOL enable);


// ============================================================================
// hal_CameraIrqSetHandler
// ----------------------------------------------------------------------------
/// This function configures which user function will be called by the
/// camera driver when an interruption is generated.
/// 
/// @param handler Pointer to a user function called when an interruption
/// is generated by the camera driver.
// ============================================================================
PUBLIC VOID hal_CameraIrqSetHandler(HAL_CAMERA_IRQ_HANDLER_T handler);


// =============================================================================
// hal_CameraIrqConfig()
// -----------------------------------------------------------------------------
/// Configure the desired interrupts
/// 
/// @param mask Mask to enable specific interrupts.  Valid interrupts are
/// liste in HAL_CAMERA_IRQ_CAUSE_T.
// =============================================================================
PUBLIC VOID hal_CameraIrqSetMask(HAL_CAMERA_IRQ_CAUSE_T mask);


// =============================================================================
// hal_CameraStartXfer
// -----------------------------------------------------------------------------
/// This function begins the IFC transfer of the camera data.  The camera
/// itself is reset and the camera module internal fifo is cleared.  The IFC
/// transfer is then started.
/// @param bufSize This is the size of the buffer in _bytes_
/// @param buffer Pointer to the video buffer where the IFC will store the data.
/// @return IFC channel number.
// =============================================================================
PUBLIC UINT8 hal_CameraStartXfer(UINT32 bufSize, UINT8* buffer);



// =============================================================================
// hal_CameraStopXfer()
// -----------------------------------------------------------------------------
/// Must be called at the end of the Frame Capture
/// If an underflow occured and the IFC tranfer is not complete, 
/// this function will handle the channel release
///
/// @param stop If \c TRUE, stops the camera controller.
/// @return 0 when the IC transfer was complete
///         1 when the IFC transfer was not complete and the channel had to be released
// =============================================================================
PUBLIC HAL_CAMERA_XFER_STATUS_T hal_CameraStopXfer(BOOL stop);


// =============================================================================
// hal_CameraSetupClockDivider
// -----------------------------------------------------------------------------
/// setup the divider of the camera module to generate the required frequency
/// @param divider : the generated clock will be 156MHz/divider
/// divider must be between 2 and 18
// =============================================================================
PUBLIC VOID hal_CameraSetupClockDivider(UINT8 divider);


#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
// ============================================================================
// hal_UartForcePinsLow
// ----------------------------------------------------------------------------
/// This function force all output pins of the camera interface to low.
///
/// This function must only be called when the camera interface is closed.
// ============================================================================
PUBLIC VOID hal_CameraForcePinsLow(VOID);
#endif // CHIP_ASIC_ID_GREENSTONE

#endif // _HAL_CAMERA_H_


