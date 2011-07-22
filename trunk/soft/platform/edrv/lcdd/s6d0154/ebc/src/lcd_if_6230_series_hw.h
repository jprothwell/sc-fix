/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2001-2007
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
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
 * lcd_if.h
 *
 * Project:
 * --------
 *   Maui
 *
 * Description:
 * ------------
 *   This file is to define LCD HW register in 6225, 6226, 6227 chips.
 *
 * Author:
 * -------
 * -------
 * -------
 * -------
 *
 *------------------------------------------------------------------------------
 * $Log$
 *
 * removed!
 * removed!
 * 
 *
 * removed!
 * removed!
 * 
 *
 * removed!
 * removed!
 * 
 *
 * removed!
 * removed!
 * 
 *
 * removed!
 * removed!
 * 
 *
 * removed!
 * removed!
 * 
 *
 * removed!
 * removed!
 * 
 * 
 *
 *
 *
 *******************************************************************************/
 #ifdef __ADS__
	typedef unsigned short UINT16;
	typedef unsigned int UINT32;
	#define KSEG1(addr) (addr)
#else
#include "cs_types.h"
#include "global_macros.h"
#endif

//#if (defined(MT6225)||defined(MT6226)||defined(MT6226M)||defined(MT6227)||defined(MT6227D)||defined(MT6226D))
	  #define LCD_base KSEG1(0x01A24000)
	  #define GPIO 	   KSEG1(0x01A03000)
	  #define REG_LCD_RESET_CLR      KSEG1(0x01A03014)
	  #define REG_LCD_RESET_SET      KSEG1(0x01A03010)
	   
                                            
      #define GD_COMMAND                    (LCD_base+0x0000)
      #define GD_STATUS                     (LCD_base+0x0004)
      #define GD_EOF_IRQ                    (LCD_base+0x0008)
      #define GD_EOF_IRQ_MASK               (LCD_base+0x000C)
      
      #define GD_ROI_TL_PPOS                (LCD_base+0x0010)
      #define GD_ROI_BR_PPOS                (LCD_base+0x0014)
      #define GD_ROI_BG_COLOR               (LCD_base+0x0018)   
      
      #define GD_VL_INPUT_FMT               (LCD_base+0x001C)
      #define GD_VL_TL_PPOS                 (LCD_base+0x0020)
      #define GD_VL_BR_PPOS                 (LCD_base+0x0024)
      #define GD_VL_EXTENTS                 (LCD_base+0x0028)
      #define GD_VL_BLEND_OPT               (LCD_base+0x002C)
      #define GD_VL_Y_SRC                   (LCD_base+0x0030)
      #define GD_VL_U_SRC                   (LCD_base+0x0036)
      #define GD_VL_V_SRC                   (LCD_base+0x0038)
      #define GD_VL_RESC_RATIO              (LCD_base+0x003C)
      
      #define GD_L1_INPUT_FMT               (LCD_base+0x0040)
      #define GD_L1_TL_PPOS                 (LCD_base+0x0044)
      #define GD_L1_BR_PPOS                 (LCD_base+0x0048)
      #define GD_L1_BLEND_OPT               (LCD_base+0x004C)
      #define GD_L1_RGB_SRC                 (LCD_base+0x0050)
      
      #define GD_L2_INPUT_FMT               (LCD_base+0x0054)
      #define GD_L2_TL_PPOS                 (LCD_base+0x0058)
      #define GD_L2_BR_PPOS                 (LCD_base+0x005C)
      #define GD_L2_BLEND_OPT               (LCD_base+0x0060)
      #define GD_L2_RGB_SRC                 (LCD_base+0x0064)
      
      #define GD_L3_INPUT_FMT               (LCD_base+0x0068)
      #define GD_L3_TL_PPOS                 (LCD_base+0x006C)
      #define GD_L3_BR_PPOS                 (LCD_base+0x0070)
      #define GD_L3_BLEND_OPT               (LCD_base+0x0074)
      #define GD_L3_RGB_SRC                 (LCD_base+0x0078)
      
      #define GD_LCD_CTRL                 	(LCD_base+0x007C)
      #define GD_LCD_TIMING                	(LCD_base+0x0080)
      #define GD_LCD_MEM_ADDRESS            (LCD_base+0x0084)
      #define GD_LCD_SINGLE_ACCESS          (LCD_base+0x0088)




      #define REG_GD_COMMAND                    *((volatile unsigned int *)(LCD_base+0x0000))
      #define REG_GD_STATUS                     *((volatile unsigned int *)(LCD_base+0x0004))
      #define REG_GD_EOF_IRQ                    *((volatile unsigned int *)(LCD_base+0x0008))
      #define REG_GD_EOF_IRQ_MASK               *((volatile unsigned int *)(LCD_base+0x000C))
                                                
      #define REG_GD_ROI_TL_PPOS                *((volatile unsigned int *)(LCD_base+0x0010))
      #define REG_GD_ROI_BR_PPOS                *((volatile unsigned int *)(LCD_base+0x0014))
      #define REG_GD_ROI_BG_COLOR               *((volatile unsigned int *)(LCD_base+0x0018))   
                                                
      #define REG_GD_VL_INPUT_FMT               *((volatile unsigned int *)(LCD_base+0x001C))
      #define REG_GD_VL_TL_PPOS                 *((volatile unsigned int *)(LCD_base+0x0020))
      #define REG_GD_VL_BR_PPOS                 *((volatile unsigned int *)(LCD_base+0x0024))
      #define REG_GD_VL_EXTENTS                 *((volatile unsigned int *)(LCD_base+0x0028))
      #define REG_GD_VL_BLEND_OPT               *((volatile unsigned int *)(LCD_base+0x002C))
      #define REG_GD_VL_Y_SRC                   *((volatile unsigned int *)(LCD_base+0x0030))
      #define REG_GD_VL_U_SRC                   *((volatile unsigned int *)(LCD_base+0x0036))
      #define REG_GD_VL_V_SRC                   *((volatile unsigned int *)(LCD_base+0x0038))
      #define REG_GD_VL_RESC_RATIO              *((volatile unsigned int *)(LCD_base+0x003C))
                                                
      #define REG_GD_L1_INPUT_FMT               *((volatile unsigned int *)(LCD_base+0x0040))
      #define REG_GD_L1_TL_PPOS                 *((volatile unsigned int *)(LCD_base+0x0044))
      #define REG_GD_L1_BR_PPOS                 *((volatile unsigned int *)(LCD_base+0x0048))
      #define REG_GD_L1_BLEND_OPT               *((volatile unsigned int *)(LCD_base+0x004C))
      #define REG_GD_L1_RGB_SRC                 *((volatile unsigned int *)(LCD_base+0x0050))
                                                
      #define REG_GD_L2_INPUT_FMT               *((volatile unsigned int *)(LCD_base+0x0054))
      #define REG_GD_L2_TL_PPOS                 *((volatile unsigned int *)(LCD_base+0x0058))
      #define REG_GD_L2_BR_PPOS                 *((volatile unsigned int *)(LCD_base+0x005C))
      #define REG_GD_L2_BLEND_OPT               *((volatile unsigned int *)(LCD_base+0x0060))
      #define REG_GD_L2_RGB_SRC                 *((volatile unsigned int *)(LCD_base+0x0064))
                                                
      #define REG_GD_L3_INPUT_FMT               *((volatile unsigned int *)(LCD_base+0x0068))
      #define REG_GD_L3_TL_PPOS                 *((volatile unsigned int *)(LCD_base+0x006C))
      #define REG_GD_L3_BR_PPOS                 *((volatile unsigned int *)(LCD_base+0x0070))
      #define REG_GD_L3_BLEND_OPT               *((volatile unsigned int *)(LCD_base+0x0074))
      #define REG_GD_L3_RGB_SRC                 *((volatile unsigned int *)(LCD_base+0x0078))
      
      #define REG_GD_LCD_CTRL                 	*((volatile unsigned int *)(LCD_base+0x007C))
      #define REG_GD_LCD_TIMING                	*((volatile unsigned int *)(LCD_base+0x0080))
      #define REG_GD_LCD_MEM_ADDRESS            *((volatile unsigned int *)(LCD_base+0x0084))
      #define REG_GD_LCD_SINGLE_ACCESS          *((volatile unsigned int *)(LCD_base+0x0088))


                                            
      /// bit mapping of Gouda command register
      #define GD_COMMAND_START_BIT						0x00000001
      
      /// bit mapping of Gouda status register
      #define GD_STATUS_IA_BUSY_BIT						0x00000001
      
      #define GD_STATUS_LCD_BUSY_BIT					0x00000010
      
      /// bit mapping of Gouda EOF IRQ register
      #define GD_EOF_CAUSE_BIT							0x00000001
      
      #define GD_EOF_STATUS_BIT							0x00010000
      
      /// bit mapping of Gouda EOF IRQ MASK register
      #define GD_EOF_MASK_BIT							0x00000001
      
      /// bit mapping of roi tl ppos register
      #define GD_ROI_TL_PPOS_X0_BIT						0x000003ff
      #define GD_ROI_TL_PPOS_Y0_BIT						0x03ff0000
      
      /// bit mapping of roi br ppos register
      #define GD_ROI_BR_PPOS_X1_BIT						0x000003ff
      #define GD_ROI_BR_PPOS_Y1_BIT						0x03ff0000
      
      /// bit mapping of roi bg color register
      #define GD_ROI_BG_COLOR_B_BIT						0x0000001f
      #define GD_ROI_BG_COLOR_G_BIT						0x000007e0
      #define GD_ROI_BG_COLOR_R_BIT						0x0000f800
      
      /// bit mapping of video layer  register
      #define GD_VL_INPUT_FMT_RGB565_BIT				0x00000000
      #define GD_VL_INPUT_FMT_UYVY_BIT					0x00000001
      #define GD_VL_INPUT_FMT_YUYV_BIT				    0x00000010
      #define GD_VL_INPUT_FMT_IYUV_BIT					0x00000011
      
      #define GD_VL_ACTIVE								0x80000000
      
      #define GD_VL_TL_PPOS_X0_BIT						0x000003ff
      #define GD_VL_TL_PPOS_Y0_BIT						0x03ff0000
      
      #define GD_VL_BR_PPOS_X0_BIT						0x000003ff
      #define GD_VL_BR_PPOS_Y0_BIT						0x03ff0000
      
      #define GD_VL_EXTENTS_MAX_LINE_BIT				0x000003ff
      #define GD_VL_EXTENTS_MAX_COL_BIT					0x03ff0000
      
      #define GD_VL_BLEND_CKEY_B_BIT					0x0000001f
      #define GD_VL_BLEND_CKEY_G_BIT					0x000007e0
      #define GD_VL_BLEND_CKEY_R_BIT					0x0000f800
      #define GD_VL_BLEND_CKEY_ENABLE_BIT				0x00010000
      #define GD_VL_BLEND_CKEY_MASK_BIT					0x000e0000
      #define GD_VL_BLEND_ALPHA_BIT						0x0ff00000
      #define GD_VL_BLEND_ROTATE_BIT					0x30000000
      #define GD_VL_BLEND_ROTATE_90_BIT					0x10000000
      #define GD_VL_BLEND_DEPTH_BIT						0xc0000000
      
      #define GD_VL_Y_SRC_BIT							0x03fffffc
      #define GD_VL_U_SRC_BIT							0x03fffffc
      #define GD_VL_V_SRC_BIT							0x03fffffc
      
      #define GD_VL_REST_RATIO_X_BIT					0x000003ff
      #define GD_VL_REST_RATIO_Y_BIT					0x03ff0000
      
      /// bit mapping of overlay layer  register
      #define GD_L1_INPUT_FMT_ARGB_BIT					0x00000001
      #define GD_L1_STRIDE_BIT							0x00007ffc
      #define GD_L1_ACTIVE								0x80000000
      
      #define GD_L1_TL_PPOS_X0_BIT						0x000003ff
      #define GD_L1_TL_PPOS_Y0_BIT						0x03ff0000
      
      #define GD_L1_BR_PPOS_X0_BIT						0x000003ff
      #define GD_L1_BR_PPOS_Y0_BIT						0x03ff0000
      
      #define GD_L1_BLEND_CKEY_B_BIT					0x0000001f
      #define GD_L1_BLEND_CKEY_G_BIT					0x000007e0
      #define GD_L1_BLEND_CKEY_R_BIT					0x0000f800
      #define GD_L1_BLEND_CKEY_ENABLE_BIT				0x00010000
      #define GD_L1_BLEND_CKEY_MASK_BIT					0x000e0000
      #define GD_L1_BLEND_ALPHA_BIT						0x0ff00000
      
      #define GD_L1_SRC_BIT								0x03fffffc
      
      #define GD_L2_INPUT_FMT_ARGB_BIT					0x00000001
      #define GD_L2_STRIDE_BIT							0x00007ffc
      #define GD_L2_ACTIVE								0x80000000
      
      #define GD_L2_TL_PPOS_X0_BIT						0x000003ff
      #define GD_L2_TL_PPOS_Y0_BIT						0x03ff0000
      
      #define GD_L2_BR_PPOS_X0_BIT						0x000003ff
      #define GD_L2_BR_PPOS_Y0_BIT						0x03ff0000
      
      #define GD_L2_BLEND_CKEY_B_BIT					0x0000001f
      #define GD_L2_BLEND_CKEY_G_BIT					0x000007e0
      #define GD_L2_BLEND_CKEY_R_BIT					0x0000f800
      #define GD_L2_BLEND_CKEY_ENABLE_BIT				0x00010000
      #define GD_L2_BLEND_CKEY_MASK_BIT					0x000e0000
      #define GD_L2_BLEND_ALPHA_BIT						0x0ff00000
      
      #define GD_L2_SRC_BIT								0x03fffffc
      
      #define GD_L3_INPUT_FMT_ARGB_BIT					0x00000001
      #define GD_L3_STRIDE_BIT							0x00007ffc
      #define GD_L3_ACTIVE								0x80000000
      
      #define GD_L3_TL_PPOS_X0_BIT						0x000003ff
      #define GD_L3_TL_PPOS_Y0_BIT						0x03ff0000
      
      #define GD_L3_BR_PPOS_X0_BIT						0x000003ff
      #define GD_L3_BR_PPOS_Y0_BIT						0x03ff0000
      
      #define GD_L3_BLEND_CKEY_B_BIT					0x0000001f
      #define GD_L3_BLEND_CKEY_G_BIT					0x000007e0
      #define GD_L3_BLEND_CKEY_R_BIT					0x0000f800
      #define GD_L3_BLEND_CKEY_ENABLE_BIT				0x00010000
      #define GD_L3_BLEND_CKEY_MASK_BIT					0x000e0000
      #define GD_L3_BLEND_ALPHA_BIT						0x0ff00000
      
      #define GD_L3_SRC_BIT								0x03fffffc
      
      /// bit mapping of lcd control register
      #define GD_LCD_DEST_LCD_CS0_BIT					0x00000000 
      #define GD_LCD_DEST_LCD_CS1_BIT					0x00000001
      #define GD_LCD_DEST_LCD_mem_BIT					0x00000002
      #define GD_LCD_DEST_mem_BIT						0x00000003
      
      #define GD_LCD_OUT_FMT_8BIT_332_BIT				0x00000000
      #define GD_LCD_OUT_FMT_8BIT_444_BIT				0x00000010
      #define GD_LCD_OUT_FMT_8BIT_565_BIT				0x00000020
      #define GD_LCD_OUT_FMT_16BIT_332_BIT				0x00000040
      #define GD_LCD_OUT_FMT_16BIT_444_BIT				0x00000050
      #define GD_LCD_OUT_FMT_16BIT_565_BIT				0x00000060
      
      #define GD_LCD_CS0_POLARITY__INV_BIT				0x00000100
      #define GD_LCD_CS1_POLARITY__INV_BIT				0x00000200
      #define GD_LCD_RS_POLARITY__INV_BIT				0x00000400
      #define GD_LCD_WR_POLARITY__INV_BIT				0x00000800
      #define GD_LCD_RD_POLARITY__INV_BIT				0x00001000
      
      #define GD_LCD_NB_COMMAND_BIT						0x001f0000
      #define GD_LCD_START_COMMAND_BIT					0x01000000
      
      /// bit mapping of lcd timing register
      #define GD_LCD_TAS_BIT							0x00000007
      #define GD_LCD_TAH_BIT							0x00000070
      #define GD_LCD_PWL_BIT							0x00003f00
      #define GD_LCD_PWH_BIT							0x003f0000
      
      /// bit mapping of lcd memroy address register
      #define GD_LCD_MEM_ADDR_BIT						0x03fffffc
      
      /// bit mapping of lcd single access register
      #define GD_LCD_DATA_BIT							0x000000ff
      #define GD_LCD_YTPE_BIT							0x00010000
      #define GD_LCD_START_WRITE_BIT					0x00020000
      #define GD_LCD_START_READ_BIT						0x00040000
      
      
      
      
      #define GD_START_RUN			REG_GD_COMMAND = GD_COMMAND_START_BIT;
      
      #define SET_LCD_ROI_WINDOW_TL(x,y)        REG_GD_ROI_TL_PPOS = ((y<<16) | x);
      #define SET_LCD_ROI_WINDOW_BR(x,y)        REG_GD_ROI_BR_PPOS = ((y<<16) | x);
      #define SET_LCD_BG_COLOR(r,g,b)			REG_GD_ROI_BG_COLOR = ((r<<11)|(g<<5)|b);
      
      #define SET_LCD_VL_TL(x,y)        REG_GD_VL_TL_PPOS = ((y<<16) | x);
      #define SET_LCD_VL_BR(x,y)        REG_GD_VL_BR_PPOS = ((y<<16) | x);
      
      #define SET_LCD_VL_EXTENTS(x,y)   REG_GD_VL_EXTENTS = ((x<<16) | y);
      
      #define SET_LCD_TIMING(tas,tah,pwl,pwh)    REG_GD_LCD_TIMING = ((pwh<<16)|(pwl<<8)|(tah<<4)|tas);
      
      #define LCD_RESET_CLR 		*((volatile unsigned int *) REG_LCD_RESET_CLR) = 4
      #define LCD_RESET_SET 		*((volatile unsigned int *) REG_LCD_RESET_SET) = 4


