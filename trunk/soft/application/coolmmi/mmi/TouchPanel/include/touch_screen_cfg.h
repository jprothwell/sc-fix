///////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2009, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL:                                                                 //
 
//  $Date:                                                                    //
//  $Revision:                                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
///                                                                           //
///     @file touch_screen_cfg.h                                              //
///                                                                           //
///     Touch panel config for MMI.                                           //
///                                                                           //
//////////////////////////////////////////////////////////////////////////////// 
#ifndef __TOUCH_SCREEN_CFG_H__
#define __TOUCH_SCREEN_CFG_H__

#include "miscfunctions.h"
#include "smsguiinterfaceprot.h"
#include "phonebookgprot.h"
#include "idleappprot.h"
#include "mainmenuprot.h"
#include "cameraapp.h"

//!Shortcuts entries defination.
//!Shortcuts order is from left to right.
#define TP_SHORTCUT_APP_ARRAY \
    EntryMainMenuFromIdleScreen, \
    mmi_phb_idle_enter_phb_list, \
    EntrySMSMainMenuList, \
    mmi_camera_entry_app_screen, \
    EntryMainMultimedia

typedef enum{
	SHORTCUT_MAINMENU,
	SHORTCUT_PHONEBOOK,
	SHORTCUT_MESSAGES,
	SHORTCUT_CAMERA,
	SHORTCUT_MULTIMEDIA	,
	SHORTCUT_MAX,
}mmi_pen_shorcut_enum;

//!How many handwriting areas supported.
#define TP_MAX_HANDWRITING_AREA_NUM 3

#ifdef LCDSIZE_128_160
//! TP_SHORTCUT_AREA
//!-------------------------------------------------------------------------------------
//!Shortcuts area defination. startX, endX, startY, endY, widthOfEveryEntryIcon
#define TP_SHORTCUT_AREA    0, 125, 160, 175, 21 
//! TP_CALIB
//!-------------------------------------------------------------------------------------
//!We need 4 points to calibrate. the first three is for calibrating, the last one is for
//!verification. So please choose some representative points. eg: top left, top right, bottom left,
//!then use the center point for verification.
#define TP_CALIB_1_X            20
#define TP_CALIB_1_Y            20
#define TP_CALIB_2_X            108
#define TP_CALIB_2_Y            140
#define TP_CALIB_3_X            40
#define TP_CALIB_3_Y            130
#define TP_CALIB_VERIFY_X       88
#define TP_CALIB_VERIFY_Y       30
#elif defined(LCDSIZE_176_220)
#define TP_SHORTCUT_AREA    0, 175, 220, 240, 35
#define TP_CALIB_1_X            20
#define TP_CALIB_1_Y            20
#define TP_CALIB_2_X            156
#define TP_CALIB_2_Y            200
#define TP_CALIB_3_X            50
#define TP_CALIB_3_Y            160
#define TP_CALIB_VERIFY_X       126
#define TP_CALIB_VERIFY_Y       60
#elif defined(LCDSIZE_220_176)/*cong.li added for product QPAD[220*176] on 2009.08.11. */
#define TP_SHORTCUT_AREA    0, 220, 0, 176, 35
#define TP_CALIB_1_X            20
#define TP_CALIB_1_Y            20
#define TP_CALIB_2_X            200
#define TP_CALIB_2_Y            156
#define TP_CALIB_3_X            20
#define TP_CALIB_3_Y            156
#define TP_CALIB_VERIFY_X       200
#define TP_CALIB_VERIFY_Y       20
#elif defined(LCDSIZE_240_320)
#define TP_SHORTCUT_AREA    0, 239, 320, 340, 48
#define TP_CALIB_1_X            20
#define TP_CALIB_1_Y            20
#define TP_CALIB_2_X            220
#define TP_CALIB_2_Y            300
#define TP_CALIB_3_X            60
#define TP_CALIB_3_Y            260
#define TP_CALIB_VERIFY_X       160
#define TP_CALIB_VERIFY_Y       100
/*+zhouqin 2011-02-16 modify for q6 320X240 */
#elif defined(LCDSIZE_320_240)
#define TP_SHORTCUT_AREA    0, 319, 240, 230, 48
#define TP_CALIB_1_X            20
#define TP_CALIB_1_Y            20
#define TP_CALIB_2_X            300
#define TP_CALIB_2_Y            220
#define TP_CALIB_3_X            60
#define TP_CALIB_3_Y            220
#define TP_CALIB_VERIFY_X       160
#define TP_CALIB_VERIFY_Y       100
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#endif 

//!Handwriting setting
#ifdef __MMI_PENPOWER__
#define TP_HANDWRITING_STROKE_END_X (-1)
#define TP_HANDWRITING_STROKE_END_Y (-1)
#define TP_HANDWRITING_CHAR_END_X   (-1)
#define TP_HANDWRITING_CHAR_END_Y   (0)
#else
#error You must choose a handwriting method!
#endif

#endif //__TOUCH_SCREEN_CFG_H__
