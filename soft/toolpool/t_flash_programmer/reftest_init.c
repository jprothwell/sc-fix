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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/toolpool/trunk/t_flash_programmer/reftest_init.c $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2010-09-28 17:31:33 +0800 (Tue, 28 Sep 2010) $                     //   
//	$Revision: 2609 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file util_reftest.c                                                      //
/// That file implements common initialization functions for the reftest      //
///  related to the peripherals.                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#include "cs_types.h"

#include "reftest_m.h"

#include "memd_m.h"
#include "lcdd_m.h"
#include "fmg_m.h"

#include "tgt_memd_cfg.h"

#define NCHAN_16                1
#define IMAGE_WIDTH             128
#define IMAGE_HEIGHT            60

UINT16 g_logoRgb[(IMAGE_WIDTH*IMAGE_HEIGHT)*NCHAN_16] =
{
#include "logo_128x60_16.dat"
};




//--------- initialises LCD + keypad for reference tests
PUBLIC VOID reftest_Init(VOID)
{
    HAL_KEY_IRQ_MASK_T keyMask;
    LCDD_SCREEN_INFO_T screenInfo;
    LCDD_ROI_T roi;
    LCDD_FBW_T frameBufferWin;

    // Setup the display 
    lcdd_Open();
    lcdd_SetStandbyMode(FALSE);
    lcdd_GetScreenInfo(&screenInfo);

    roi.x=0;
    roi.y=0;
    roi.width=screenInfo.width;
    roi.height=screenInfo.height;

    while (LCDD_ERR_NO != lcdd_FillRect16(&roi, 0xffff));

    // Draw a cool logo
    frameBufferWin.fb.buffer = (UINT16*)  g_logoRgb; 
    frameBufferWin.fb.width =  IMAGE_WIDTH;
    frameBufferWin.fb.height = IMAGE_HEIGHT;
    frameBufferWin.fb.colorFormat = LCDD_COLOR_FORMAT_RGB_565;

    frameBufferWin.roi.x=0;
    frameBufferWin.roi.y=0;
    frameBufferWin.roi.width= IMAGE_WIDTH;
    frameBufferWin.roi.height= IMAGE_HEIGHT;

    while (LCDD_ERR_NO != lcdd_Blit16(&frameBufferWin,0,0));

    lcdd_SetBrightness(6);

    fmg_PrintfInit(0, 8, screenInfo.width, screenInfo.height);

    // Setup the keypad
    hal_KeyOpen(240, 16); //8); // dbn = 15ms, itl = 120 ms
    hal_KeyIrqSetHandler(reftest_KeyCallback);
    keyMask.onUp = 1;
    keyMask.onPressed = 1;
    keyMask.onDown = 1;
    hal_KeyResetIrq();
    hal_KeyIrqSetMask(keyMask);
}

// This function waits until the OK key is pressed
PUBLIC VOID reftest_WaitAnyKey()
{
    BOOL testStarted = 0;
	UINT32     evt[4];
	Msg_t	*msg;
    
    fmg_PrintfSetXY(2,8);
    fmg_Printf("Press any key\n");

    while (testStarted==0)
    {
        msg = sxr_Wait(evt, testMbx);
        if (msg->H.Id == MMI_KEY_DOWN)
        {
                testStarted = 1;
                sxs_fprintf(TSTDOUT,"starting test");
                fmg_PrintfSetXY(2,8);
                fmg_Printf("Starting test\n");
        }
	sxr_Free(msg);
    }

}



