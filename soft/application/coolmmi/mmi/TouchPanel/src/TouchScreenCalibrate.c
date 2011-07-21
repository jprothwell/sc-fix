
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
///     @file TouchScreenCalibrate.c                                          //
///                                                                           //
///     Touch panel calibration procedure.                                    //
///                                                                           //
//////////////////////////////////////////////////////////////////////////////// 
#include "mmi_features.h"
#define __TOUCH_PANEL_CALIBRATE_C__
#if ( defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) ) && defined(MMI_ON_HARDWARE_P)

#include "mmi_trace.h"
#include "csw.h"
#include "tsd_m.h"
#include "kal_release.h"
#include "app_ltlcom.h"
#include "queuegprot.h"
#include "touchscreengprot.h"
#include "touch_screen_cfg.h"
#include "nvramprot.h"
#include "nvram_user_defs.h"

typedef struct
{ 
    TSD_CALIBRATION_POINT_T calibPoints[3];
    TSD_CALIBRATION_POINT_T defaultCalibPoints[3];
    BOOL defaultExsits;
    BOOL calibExsits;
} TP_CALIB_PARA_T;

PRIVATE TSD_CALIBRATION_POINT_T g_CalibPoints[3];
//!For calibration process.
PRIVATE UINT16 g_CalibFlag = 0-1;

/* --------------------------------------------------------------------*/
/** mmi_pen_start_calibration
 * @brief Start calibration process. 
 * Here is the calibration procedure:
 * 1, Call mmi_pen_start_calibration, and draw first point on the screen.
 * 2, After 1st point is touched, call mmi_pen_set_calibration_data( x, y, 0)
 * and then draw the 2nd point on the screen.
 * 3, Do steps 2 until all the 4 points is touched.
 * 4, Call mmi_pen_end_calibration to store the calibration data in to flash.
 *
 *
 * @return 
 */
/* --------------------------------------------------------------------*/
PUBLIC VOID mmi_pen_start_calibration( )
{ 
    mmi_trace( g_sw_TPL, "TP in mmi_pen_start_calibration.");
    //memset( g_CalibPoints, 0, sizeof( g_CalibPoints));
    g_CalibPoints[0].expected.x = TP_CALIB_1_X;
    g_CalibPoints[0].expected.y = TP_CALIB_1_Y;
    g_CalibPoints[1].expected.x = TP_CALIB_2_X;
    g_CalibPoints[1].expected.y = TP_CALIB_2_Y;
    g_CalibPoints[2].expected.x = TP_CALIB_3_X;
    g_CalibPoints[2].expected.y = TP_CALIB_3_Y;
    tsd_SetCalibStatus( FALSE);
    g_CalibFlag = 0;

    mmi_pen_unblock( );
}

/* --------------------------------------------------------------------*/
/** mmi_pen_set_calibration_data
 * @brief After user touch the calibration point on the screen, call this
 * function to pass the XY value back. This function will store and check
 * the given point coordinates.
 * 
 * @param x 
 * @param y
 * @param index Which point is touched.
 * 
 * @return If the given point coordinates is invalid, return FALSE.
 */
/* --------------------------------------------------------------------*/
PUBLIC BOOL mmi_pen_set_calibration_data( UINT16 x, UINT16 y, UINT16 index)
{ 
    ASSERT( index < 4);
    if( index < 3)
    { 
        g_CalibPoints[index].measured.x = x;
        g_CalibPoints[index].measured.y = y;
        mmi_trace( g_sw_TPL, "TP In mmi_pen_set_calibration_data. x: %d %d, y: %d %d",
            x, g_CalibPoints[index].expected.x,
            y, g_CalibPoints[index].expected.y);
        if( index == 2)
        { 
            if(tsd_SetCalibPoints( g_CalibPoints) == TSD_ERR_NO)
            { 
                tsd_SetCalibStatus( TRUE);
            }
        }
    }
    else
    { 
        mmi_trace( g_sw_TPL, "TP In mmi_pen_set_calibration_data index 3. x: %d y: %d",
                x, y);
        if( x > TP_CALIB_VERIFY_X + 5 || x < TP_CALIB_VERIFY_X- 5 ||
                y > TP_CALIB_VERIFY_Y + 5 || x < TP_CALIB_VERIFY_Y-5)
            return FALSE;
    }
    
    g_CalibFlag ++;
    return TRUE;
}

/* --------------------------------------------------------------------*/
/** mmi_pen_set_calibration_data
 * @brief If the calibration procedure is ended normally, this function 
 * will store it to flash. Otherwise, just return.
 * IMPORTANT: If there is no defualt calibration data stored in the flash,
 * the last calibration data will be stored as the defualt.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PUBLIC VOID mmi_pen_end_calibration( )
{ 
	mmi_trace( g_sw_TPL, "TP In mmi_pen_end_calibration.");
    if( g_CalibFlag != 4)
    { 
        g_CalibFlag = 0-1;
        return;
    }
    
    TP_CALIB_PARA_T calibPara;
    S16 error;
    S32 r = ReadRecord(
            NVRAM_EF_TOUCH_PANEL_SETTING_LID, 1,
            (void *) &calibPara,
            (U16)sizeof( calibPara), &error);
    memcpy( calibPara.calibPoints, g_CalibPoints,
            sizeof( calibPara.calibPoints));
    calibPara.calibExsits = TRUE;
    if( r != sizeof( calibPara) || !calibPara.defaultExsits)
    { 
        memcpy( calibPara.defaultCalibPoints, g_CalibPoints,
                sizeof( calibPara.defaultCalibPoints));
        calibPara.defaultExsits = TRUE;
    }

    r = WriteRecord( 
            NVRAM_EF_TOUCH_PANEL_SETTING_LID, 1,
            (void *) &calibPara,
            (U16)sizeof( calibPara), &error);
    mmi_trace( g_sw_TPL, "TP in mmi_pen_end_calibration. write r:%d, error:%d", 
            r, error);

    g_CalibFlag = 0-1;
}

/* --------------------------------------------------------------------*/
/** mmi_pen_restore_default_calibration
 * @brief If there is a defualt calibration, restore it as the current
 * calibration data.
 * 
 * @return TRUE if success.
 */
/* --------------------------------------------------------------------*/
PUBLIC BOOL mmi_pen_restore_default_calibration( )
{ 
    TP_CALIB_PARA_T calibPara;
    S16 error;
    S32 r = ReadRecord(
            NVRAM_EF_TOUCH_PANEL_SETTING_LID, 1,
            (void *) &calibPara,
            (U16)sizeof( calibPara), &error);
    mmi_trace( g_sw_TPL, "TP in mmi_pen_restore_default_calibration. r:%d, error:%d", 
            r, error);
    if( r != sizeof( calibPara))
    { 
        return FALSE;
    }

    if( calibPara.defaultExsits)
    { 
        memcpy( calibPara.calibPoints, calibPara.defaultCalibPoints, 
                sizeof( calibPara.calibPoints));
        calibPara.calibExsits = TRUE;
        r = WriteRecord( 
            NVRAM_EF_TOUCH_PANEL_SETTING_LID, 1,
            (void *) &calibPara,
            (U16)sizeof( calibPara), &error);
        mmi_trace( g_sw_TPL, "TP in mmi_pen_restore_default_calibration. write r:%d, error:%d", 
            r, error);
        tsd_SetCalibPoints( calibPara.calibPoints);
        tsd_SetCalibStatus( TRUE);
        return TRUE;
    }

    tsd_SetCalibStatus( FALSE);
    return FALSE;
}

/* --------------------------------------------------------------------*/
/** mmi_pen_read_calibration
 * @brief Read calibration data from flash.
 * 
 * @return If no data stored in the flash, return FALSE.
 */
/* --------------------------------------------------------------------*/
PUBLIC BOOL mmi_pen_read_calibration( )
{ 
    TP_CALIB_PARA_T calibPara;
    S16 error;
    S32 r = ReadRecord(
            NVRAM_EF_TOUCH_PANEL_SETTING_LID, 1,
            (void *) &calibPara,
            (U16)sizeof( calibPara), &error);
    mmi_trace( g_sw_TPL, "TP in mmi_pen_read_calibration. r:%d, error:%d", 
            r, error);
    if( r != sizeof( calibPara))
    { 
        return FALSE;
    }
    
    if( calibPara.calibExsits)
    { 
        tsd_SetCalibPoints( calibPara.calibPoints);
        tsd_SetCalibStatus( TRUE);
        return TRUE;
    }

    return mmi_pen_restore_default_calibration( );
}

/* --------------------------------------------------------------------*/
/** mmi_pen_is_in_calibration_process
 * @brief Check if it is in touch panel calibation process.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PUBLIC BOOL mmi_pen_is_in_calibration_process( )
{ 
    if( g_CalibFlag <= 4)
        return TRUE;

    return FALSE;
}

#endif

