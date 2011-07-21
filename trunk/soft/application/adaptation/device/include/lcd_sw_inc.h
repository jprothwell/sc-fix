////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2007, Coolsand Technologies, Inc.
//                       All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmisison of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: lcd_sw_inc.h
// 
// DESCRIPTION:
//   TODO... 
//   LCD dimension definition.
// 
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   TODO... 
//   Dingjian         070604         modify LCD Framework
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef __LCD_SW_INC_H__
#define __LCD_SW_INC_H__
#include "mmi_features.h"
////////////////////////////////////////////////////////////////////////////////
// DECLARE HEAD FILES
////////////////////////////////////////////////////////////////////////////////
// TODO... 

////////////////////////////////////////////////////////////////////////////////
// DEFINE LOCAL CONSTANTS 
////////////////////////////////////////////////////////////////////////////////
// TODO.. 

////////////////////////////////////////////////////////////////////////////////
// DEFINE LOCAL MACROS 
////////////////////////////////////////////////////////////////////////////////
// TODO... 

 /*+ zhouqin 2011-02-16 modify for q6 320X240 */
#ifdef LCDSIZE_320_240
#define LCD_WIDTH     320
#define LCD_HEIGHT    240  										/* Detail Info: [Main LCD is 220x176]*/
#endif // LCDSIZE_320_240    
/*- zhouqin 2011-02-16 modify for q6 320X240 */
// modify for lcd r61502u 
// dingjian 20071026
#ifdef    LCDSIZE_240_320
#define LCD_WIDTH    240
#define LCD_HEIGHT    320   
#endif

#ifdef    LCDSIZE_176_220
#define LCD_WIDTH    176
#define LCD_HEIGHT    220   
#endif

/*added for product QPAD[220*176] */
#ifdef    LCDSIZE_220_176
#define LCD_WIDTH    220
#define LCD_HEIGHT    176   
#endif

#ifdef    LCDSIZE_160_128
#define LCD_WIDTH    160
#define LCD_HEIGHT    128   
#endif

#ifdef    LCDSIZE_128_160
#define LCD_WIDTH    128
#define LCD_HEIGHT    160   
#endif
     
#ifdef    LCDSIZE_128_128
#define LCD_WIDTH    128
#define LCD_HEIGHT    128 
#endif
 
#define SUBLCD_WIDTH          96
#define SUBLCD_HEIGHT         64

#define UI_DEVICE_WIDTH             LCD_WIDTH
#define UI_DEVICE_HEIGHT            LCD_HEIGHT

////////////////////////////////////////////////////////////////////////////////
// DECLARE GLOBAL VARIABLES
////////////////////////////////////////////////////////////////////////////////
// TODO... 

////////////////////////////////////////////////////////////////////////////////
// DECLARE GLOBAL FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
// TODO... 

#endif // __LCD_SW_INC_H__ 

