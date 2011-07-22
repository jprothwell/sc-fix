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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/lcdd/au/lcdc/src/drv_lcd.h $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2010-08-23 13:47:34 +0800 (Mon, 23 Aug 2010) $                     //   
//	$Revision: 1532 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file drv_lcd.h                                                           //
/// That file provides the private interface for the AU lcd driver.           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _DRV_LCD_H
#define _DRV_LCD_H
// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================



// =============================================================================
//  MACROS
// =============================================================================


// =============================================================================
//  TYPES
// =============================================================================

typedef struct {
    UINT8   hsa;
    UINT8   hea;
    UINT8   vsa;
    UINT8   vea;
} LCDD_ACTIVE_WIN_STATUS_T;


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================


// =============================================================================
//  FUNCTIONS
// =============================================================================

// =============================================================================
// lcdd_DisplayOn
// -----------------------------------------------------------------------------
/// Switch on the display of the LCD screen 
// =============================================================================
PRIVATE VOID lcdd_DisplayOn(VOID);

// =============================================================================
// lcdd_PowerOn
// -----------------------------------------------------------------------------
/// Switch ON the LCD screen power
// =============================================================================
PRIVATE VOID lcdd_PowerOn(VOID);

//PUBLIC MY_LCDD_ERR_T my_lcdd_Open(VOID);
//PUBLIC MY_LCDD_ERR_T my_lcdd_SetStandbyMode(BOOL standbyMode);
//PUBLIC MY_LCDD_ERR_T my_lcdd_SetActiveWindow(UINT8 hsa, UINT8 vsa, UINT8 hea, UINT8 vea);
//PUBLIC MY_LCDD_ERR_T my_lcdd_FillRect16(UINT16 bgColor);
//PUBLIC MY_LCDD_ERR_T my_lcdd_Blit16(CONST UINT16* pPixelData);
//PUBLIC MY_LCDD_ERR_T my_lcdd_Close(VOID);
//PUBLIC BOOL my_lcdd_Busy();

#endif // _DRV_LCD_H_


