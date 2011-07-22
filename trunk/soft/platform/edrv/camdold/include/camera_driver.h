//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
//            Copyright (C) 2003-2009, Coolsand Technologies, Inc.              //
//                            All Rights Reserved                               //
//                                                                              //
//      This source code is the property of Coolsand Technologies and is        //
//      confidential.  Any  modification, distribution,  reproduction or        //
//      exploitation  of  any content of this file is totally forbidden,        //
//      except  with the  written permission  of  Coolsand Technologies.        //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
//  $HeadURL:                                                                   //
//	$Author:                                                                    //
//	$Date: 2009-11-24                                                           //
//	$Revision:                                                                  //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
///  @file camera_driver.h                                                        //
///  This file contains the driver for camera      //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#ifndef CAMERA_DRIVER_H_
#define CAMERA_DRIVER_H_

// =============================================================================
// CAM_DRV_T
// -----------------------------------------------------------------------------
/// This types describes all functions for camera driver.
// =============================================================================
typedef struct 
{
    VOID (*camera_PowerOn)(CAM_SIZE_T CamOutSize, UINT16 IspOutWidth, UINT16 IspOutHeight, CAM_FORMAT_T Format);
    VOID (*camera_PowerOff)(VOID);		 
    VOID (*camera_Reserve)(CAM_IRQ_HANDLER_T FrameReady);
    VOID (*camera_Release)(VOID);
    VOID (*camera_PrepareImageCaptureL)(CAM_FORMAT_T ImageFormat, UINT32 SizeIndex, CAM_IMAGE_T Buffer);
    VOID(* camera_CaptureImage)(VOID);
    VOID(* camera_SetZoomFactorL)(UINT32 ZoomFactor);
    UINT32 (*camera_ZoomFactor)(VOID);
    VOID (*camera_SetDigitalZoomFactorL)(UINT32 DigZoomFactor);
    UINT32 (*camera_DigitalZoomFactor)(VOID);
    VOID (*camera_SetContrastL)(UINT32 Contrast);
    UINT32 (*camera_Contrast)(VOID);
    VOID(* camera_SetBrightnessL)(UINT32 Brightness);
    UINT32(* camera_Brightness)(VOID);
    VOID (*camera_SetFlashL)(CAM_FLASH_T Flash);
    CAM_FLASH_T (*camera_Flash)(VOID);
    VOID (*camera_SetExposureL)(CAM_EXPOSURE_T Exposure);
    CAM_EXPOSURE_T (*camera_Exposure)(VOID);
    VOID (*camera_SetWhiteBalanceL)(CAM_WHITEBALANCE_T WhiteBalance);
    CAM_WHITEBALANCE_T (*camera_WhiteBalance)(VOID);
    VOID (*camera_SetSpecialEffect)(CAM_SPECIALEFFECT_T SpecialEffect);
    CAM_SPECIALEFFECT_T (*camera_SpecialEffect)(VOID);
    VOID(* camera_SetNightMode)(UINT32 NightMode);
    UINT32 (*camera_NightMode)(VOID);
    VOID (*camera_StartViewFinderDirectL)();
    VOID (*camera_StopViewFinder)(VOID);
    BOOL(* camera_ViewFinderActive)(VOID);

    VOID (*camera_SetViewFinderMirror)(BOOL Mirror);
    BOOL (*camera_ViewFinderMirror)(VOID);
    VOID(* camera_CancelCaptureImage)(VOID);
    VOID(* camera_EnumerateCaptureSizes)(CAM_SIZE_T *Size, UINT32 SizeIndex, CAM_FORMAT_T Format);
    VOID(* camera_PrepareVideoCaptureL)(CAM_FORMAT_T Format, UINT32 SizeIndex, UINT32 RateIndex, UINT32 BuffersToUse, UINT32 FramesPerBuffer);
    VOID(* camera_StartVideoCapture)(VOID);
    VOID (*camera_StopVideoCapture)(VOID);
    BOOL (*camera_VideoCaptureActive)(VOID);
    VOID(* camera_EnumerateVideoFrameSizes)(CAM_SIZE_T Size, UINT32 SizeIndex, CAM_FORMAT_T Format);
    VOID (*camera_EnumerateVideoFrameRates)();
    VOID (*camera_GetFrameSize)(CAM_SIZE_T Size);
    VOID (*camera_SetFrameRate)(UINT32 FrameRate);
    UINT32 (*camera_FrameRate)(VOID);
    UINT32 (*camera_BuffersInUse)(VOID);
    UINT32 (*camera_FramesPerBuffer)(VOID);
    UINT8 (*camera_GetId)(VOID);
    char* (*camera_GetStringId)(VOID);
    VOID (* camera_GetImageInfo)(UINT32 *pcamout,UINT16 *pwidth,UINT16 *pheight);	
}CAM_DRV_T;

// ============================================================================
// camera_**_RegInit
// ----------------------------------------------------------------------------
// This function register the right camera driver
//  When added new camera dirver,must add camera_**_RegInit function on here
// ============================================================================
extern BOOL camera_siv100b_RegInit(CAM_DRV_T  *pCamDrv);
extern BOOL camera_gc0306_RegInit(CAM_DRV_T  *pCamDrv);
extern BOOL camera_gc0307_RegInit(CAM_DRV_T  *pCamDrv);
extern BOOL camera_ov7670_RegInit(CAM_DRV_T  *pCamDrv);
extern BOOL camera_ov7675_RegInit(CAM_DRV_T  *pCamDrv);
extern BOOL camera_byd3603_RegInit(CAM_DRV_T  *pCamDrv);
extern BOOL camera_byd3703_RegInit(CAM_DRV_T  *pCamDrv);
extern BOOL camera_sp80818_RegInit(CAM_DRV_T  *pCamDrv);
extern BOOL camera_ov7660_RegInit(CAM_DRV_T  *pCamDrv);
extern BOOL camera_bg0313_RegInit(CAM_DRV_T  *pCamDrv);
extern BOOL camera_yacbaa0sddas_RegInit(CAM_DRV_T  *pCamDrv);
extern BOOL camera_hya10s_RegInit(CAM_DRV_T  *pCamDrv);
extern BOOL camera_gc0309_RegInit(CAM_DRV_T  *pCamDrv);
extern BOOL camera_hi704_RegInit(CAM_DRV_T  *pCamDrv);

extern BOOL camera_pas6175_RegInit(CAM_DRV_T  * pCamDrv);
extern BOOL camera_sp0828_RegInit(CAM_DRV_T  * pCamDrv);

#endif 

