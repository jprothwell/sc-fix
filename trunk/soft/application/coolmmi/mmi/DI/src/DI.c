////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2007, Coolsand Technologies, Inc.
//                       All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmisison of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: MediaChip_DI_Vimicro0528.c
// 
// DESCRIPTION:
//   TODO... 
// 
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   TODO... 
//   Dingjian        20070726      build
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// DESIGN THOUGHT DESCRIPTOIN 
////////////////////////////////////////////////////////////////////////////////
// TODO.. 

////////////////////////////////////////////////////////////////////////////////
// DECLARE HEAD FILES
////////////////////////////////////////////////////////////////////////////////
// TODO... 
#include <cswtype.h>
#include <mmi_trace.h>
#include <ts.h>
#include "di.h"

////////////////////////////////////////////////////////////////////////////////
// LOCAL FUNCTION PROTOTYPES
////////////////////////////////////////////////////////////////////////////////
// TODO.. 

////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
////////////////////////////////////////////////////////////////////////////////
// TODO... 

// this variable only used DI Module !!!
// g_sw_DI_FLAG define
// 8 LCD Chip Init(1/0)
// 7 Camera open/close(1/0)
// 6 Image open/close(1/0)
// 5 Vibrator open/close(1/0)
// 4 LED open/close(1/0)
// 3
// 2
// 1
UINT8 g_sw_DI_FLAG = 0;
// this variable only used DI Module !!!
// get preview width and height for update_image update area
UINT16 g_sw_DI_PreviewWidth =0 , g_sw_DI_PreviewHeight = 0;
// this variable only used DI Module !!!
UINT32 g_sw_DI_CaptureImageSize = 0xFFFFFFFF;

// this variable be used DI Module + LCD Driver + Vimicro Driver
UINT32 drv_camera_main_cs_base = 0;
UINT32 drv_camera_main_cs_offset = 0;

extern Image_Funcs_DI Image_Funcs_Instance_DI;
extern Camera_Funcs_DI Camera_Funcs_Instance_DI;


////////////////////////////////////////////////////////////////////////////////
// FUNCTION IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////
// TODO... 

Image_Funcs_DI * Image_DI = &Image_Funcs_Instance_DI;
Camera_Funcs_DI * Camera_DI = &Camera_Funcs_Instance_DI;



