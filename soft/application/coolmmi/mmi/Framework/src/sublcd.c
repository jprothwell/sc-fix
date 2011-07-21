/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *  sublcd.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

 
//#include "lcd_sw.h"   
 

//#ifndef DUAL_LCD
////CSD added, for dummy use
//	LCD_Funcs_DI   *SubLCD;
//	kal_uint8 sublcd_shadow_buffer[SUB_LCD_HEIGHT][SUB_LCD_WIDTH/8];
//
//	LCD_Funcs_DI LCD1_func_S6B1713 = {
//   LCD1_Init_S6B1713,
//   LCD1_PWRON_S6B1713,
//   LCD1_SetVoltLevel_S6B1713,
//   LCD1_ON_S6B1713,
//   LCD1_BlockWrite_Rotate90_S6B1713,
//   LCD1_Size_S6B1713,
//
 
//   LCD1_EnterSleep_S6B1713,
//   LCD1_ExitSleep_S6B1713,
//   /*Engineering mode*/
//   LCD1_GetParm_S6B1713,
//   LCD1_SetBias_S6B1713,
//   LCD1_Contrast_S6B1713,
//   LCD1_LineRate_S6B1713,
//   LCD1_Temp_Compensate_S6B1713
//   //CSD end
//};
//
//#endif
//
//
//kal_uint8 SubLCDSetArray[]=
//{
//  0x80,
//  0x40,
//  0x20,
//  0x10,
//  0x08,
//  0x04,
//  0x02,
//  0x01
//};
//
//
//kal_uint8 SubLCDReSetArray[]=
//{
//  0x7F,
//  0xBF,
//  0xDF,
//  0xEF,
//  0xF7,
//  0xFB,
//  0xFD,
//  0xFE
//};
//
///*
//kal_uint8 sub_lcd_shadow_buffer[SUB_LCD_HEIGHT][SUB_LCD_WIDTH/8];
//*/
//
////Dummy function, if S6B1713 driver is not available	
//void LCD1_Init_S6B1713(kal_uint32 bkground, void **buf_addr)		//Validate
//{
//}
//void LCD1_PWRON_S6B1713(kal_bool on)
//{
//}
//void LCD1_SetVoltLevel_S6B1713(kal_uint8 voltage)
//{
//}
//void LCD1_ON_S6B1713(kal_bool on)
//{
//}
//void LCD1_BlockWrite_Rotate90_S6B1713(kal_uint16 startx,kal_uint16 starty,kal_uint16 endx,kal_uint16 endy)
//{
//}
//void LCD1_Size_S6B1713(kal_uint16 *out_LCD_width,kal_uint16 *out_LCD_height)
//{
//}
////Dummy function, if S6B1713 driver is not available
//void LCD1_EnterSleep_S6B1713(void)
//{
//}
//void LCD1_ExitSleep_S6B1713(void)
//{
//}
//kal_uint8 LCD1_GetParm_S6B1713(lcd_func_type type)
//{
//	return 0;
//}
//void LCD1_SetBias_S6B1713(kal_uint8 *bias)
//{
//}
//void LCD1_Contrast_S6B1713(kal_uint8 *contrast)
//{
//}
//void LCD1_LineRate_S6B1713(kal_uint8 *linerate)
//{
//}
//void LCD1_Temp_Compensate_S6B1713(kal_uint8 *compensate)
//{
//}



