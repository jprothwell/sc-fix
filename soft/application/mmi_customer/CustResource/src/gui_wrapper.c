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

#include "mmi_features.h"
/* Target & MNT */
#ifdef MMI_ON_HARDWARE_P
#include "mmi_data_types.h"
#include "gdi_include.h"

S32 UI_device_width			= GDI_LCD_WIDTH;
S32 UI_device_height			= GDI_LCD_HEIGHT;

S32 MAIN_LCD_device_width	= GDI_LCD_WIDTH;
S32 MAIN_LCD_device_height = GDI_LCD_HEIGHT;

	#ifdef __MMI_SUBLCD__
S32 SUB_LCD_device_width	= GDI_SUBLCD_WIDTH;
S32 SUB_LCD_device_height	= GDI_SUBLCD_HEIGHT;
	#endif /* __MMI_SUBLCD__ */

#else

/* PC_Simulator */
#include "gui_setting.h"

S32 UI_device_width			= LCD_WIDTH;
S32 UI_device_height			= LCD_HEIGHT;

S32 MAIN_LCD_device_width	= LCD_WIDTH;
S32 MAIN_LCD_device_height = LCD_HEIGHT;

	#ifdef __MMI_SUBLCD__
S32 SUB_LCD_device_width	= SUBLCD_WIDTH;
S32 SUB_LCD_device_height	= SUBLCD_HEIGHT;
	#endif /* __MMI_SUBLCD__ */

#endif

/* Bear 040228 - remove unused code for PC_simulator */
//
//#if(WINGUI_SIMULATE_HW)
//
//word simulator_lcd_double_buffer[GDI_LCD_WIDTH*GDI_LCD_HEIGHT];
//byte simulator_sub_lcd_double_buffer[(SUB_LCD_DEVICE_WIDTH/8)*SUB_LCD_DEVICE_HEIGHT];
//
//#elif(BUILD_TYPE_X86WIN32_DOUBLE_BUFFERED)
//
//byte simulator_lcd_double_buffer[GDI_LCD_WIDTH*3*GDI_LCD_HEIGHT];
//byte simulator_sub_lcd_double_buffer[(SUB_LCD_DEVICE_WIDTH/8)*SUB_LCD_DEVICE_HEIGHT];
//
//#endif

