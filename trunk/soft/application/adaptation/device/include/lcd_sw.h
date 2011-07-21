////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2007, Coolsand Technologies, Inc.
//                       All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmisison of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: lcd_sw.h
// 
// DESCRIPTION:
//   TODO... 
//   All LCD interface configuration and LCD Function type definition struct declaration.
// 
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   TODO... 
//   Dingjian         070604         modify LCD Framework
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef __LCD_SW_H__
#define __LCD_SW_H__

#include "lcd_if.h"
#if 0
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
   void (* Init)(kal_uint32 background, void **buf_addr);
   void (* PWRON)(kal_bool on);
   void (* BrightLevel)(kal_uint8 level);
   void (* SCREENON)(kal_bool on);
   void (* BlockWrite)(kal_uint16 startx,kal_uint16 starty,kal_uint16 endx,kal_uint16 endy);
   void (* GetSize)(kal_uint16 *out_LCD_width,kal_uint16 *out_LCD_height);
   void (* SleepIn)(void);
   void (* SleepOut)(void);
   void (* PartialOn) (kal_uint16 start_page,kal_uint16 end_page);
   void (* PartialOff) (void);
   void (* ClearScreen)(kal_uint16 background);
   kal_uint8 (*partial_display_align) (void);
   /*Engineering mode*/
   kal_uint8 (* get_param_number)(lcd_func_type type);
   void (* set_bias)(kal_uint8 *bias);
   void (* set_contrast)(kal_uint8 *contrast);
   void (* set_linerate)(kal_uint8 *linerate);
   void (* set_temp_compensate)(kal_uint8 *compensate);
   void (* lcd_update_image)(kal_uint16 startx, kal_uint16 starty,kal_uint16 Width,kal_uint16 Height);
   void (* lcd_chip_init)(void);
}LCD_Funcs_DI;
////////////////////////////////////////////////////////////////////////////////
#endif
#endif // __LCD_SW_H__ 


