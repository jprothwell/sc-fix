////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2007, Coolsand Technologies, Inc.
//                       All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmisison of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: lcd_sw_rnd.h
// 
// DESCRIPTION:
//   TODO... 
//   The abstract LCD driver function external declaration, shadow buffer macros and definition 
//   for MMI.
// 
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   TODO... 
//   Dingjian         070604          modify LCD Framework
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef __LCD_SW_RND_H__
#define __LCD_SW_RND_H__

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
//#define WHITE_BACKLIGHT
#define BLACK_BACKLIGHT
//#define BLUE_BACKLIGHT

// Color Transform 
#define  DRV_RGB_TO_HW(R,G,B)\
	( (kal_uint16)((((R) & 0xf8) << 8) | (((G) & 0xfc) << 3) | (((B) & 0xf8) >> 3) ))
	
//2007-7-3 fengzb
#define  DRV_HW_TO_RGB_R(VALUE)	( (kal_uint8)(((VALUE)&0xf800)>>8))
#define  DRV_HW_TO_RGB_G(VALUE)	( (kal_uint8)(((VALUE)&0x07e0)>>3))
#define  DRV_HW_TO_RGB_B(VALUE)	( (kal_uint8)(((VALUE)&0x001f)<<3))

// Frame Buffer 
#define DRV_MAINLCD_BIT_PER_PIXEL	(16)
#define DRV_MAINLCD_GET_BUFFER_PIXEL(X,Y,VALUE)\
	(VALUE=(*((U16*)GDI_LAYER.act_buf_ptr+((Y)*(GDI_LAYER.act_pitch)+(X)))))
#define DRV_MAINLCD_SET_BUFFER_PIXEL(X,Y,VALUE)\
	((*((U16*)GDI_LAYER.act_buf_ptr+((Y)*(GDI_LAYER.act_pitch)+(X))))=VALUE)	

#define DRV_MAINLCD_COLOR_TYPE	UINT16
#define DRV_MAINLCD_ACCESS_BEGIN(ADDR,PITCH)	{ \
							_lcd_address=(DRV_MAINLCD_COLOR_TYPE*)ADDR; \
							_lcd_pitch  =(int) PITCH; \
							{
						
#define DRV_MAINLCD_GOTO_XY(X,Y)\
	_lcd_current_address = _lcd_address + (Y)*_lcd_pitch + (X)

#define DRV_MAINLCD_SET_PIXEL(COLOR)		do{ \
							*_lcd_current_address = (DRV_MAINLCD_COLOR_TYPE) COLOR; \
							_lcd_current_address++; \
							}while(0)
						
#define DRV_MAINLCD_GET_PIXEL(COLOR)		do{ \
							COLOR = *_lcd_current_address; \
							_lcd_current_address++; \
							}while(0)

#define DRV_MAINLCD_JUMP_PIXELS(PIXELS)	do{ \
							_lcd_current_address+=PIXELS; \
							}while(0)
						
#define DRV_MAINLCD_ACCESS_END			} }

// Init Color 
#ifdef __MMI_NOKIA_STYLE_N800__
#define DRV_MAINLCD_INIT_COLOR	(0x0000)	// 16bit 
#else
#define DRV_MAINLCD_INIT_COLOR	(0xffff)	// 16bit 
#endif

////////////////////////////////////////////////////////////////////////////////
// DECLARE GLOBAL VARIABLES
////////////////////////////////////////////////////////////////////////////////
// TODO... 
//extern kal_uint8 SubLCDSetArray[];
//extern kal_uint8 SubLCDReSetArray[];

////////////////////////////////////////////////////////////////////////////////
// DECLARE GLOBAL FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
// TODO... 

#endif   //  __LCD_SW_RND_H__

