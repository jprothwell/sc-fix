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
 *  gdi_layer.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI Layer related.
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#include "gdi_internal.h"
#include "gd_primitive.h"
#include "mmc_jpeg_enc.h"
#ifdef GDI_USING_HW_JPEG
#include "med_api.h"
#include "med_main.h"
#endif /* GDI_USING_HW_JPEG */ 
#include "debuginitdef.h"
#ifdef __MMI_TVOUT__
// #include "mmi_features.h"
#include "mdi_datatype.h"
#include "mdi_tv.h"
#endif /* __MMI_TVOUT__ */ 
#if defined(ISP_SUPPORT)
   /* for visual effect */
#ifdef MMI_ON_HARDWARE_P
#include "intrctrl.h"
#include "drv_comm.h"
//#include "drvpdn.h"
#include "jpeg.h"
#include "visualhisr.h"
#include "visual_comm.h"
#include "fat_fs.h"
#include "resizer.h"
#include "isp_if.h"
#include "imgproc.h"
#include "img_comm.h"
#include "imgdma.h"
#include "image_effect.h"
#endif /* MMI_ON_HARDWARE_P */ /* ISP_SUPPORT */
#endif /* defined(ISP_SUPPORT) */ 

#if defined(__PRJ_WITH_SPILCD__) || (!defined(CHIP_HAS_GOUDA))
 extern U16			simulator_Merge_buffer[GDI_LCD_WIDTH * GDI_LCD_HEIGHT];
#else
 extern U16*		simulator_Merge_buffer ;
#endif

extern lcd_layer_struct	lcd_layer_data[4];
U16					layer_sim_LUT0[256];
U16					layer_sim_LUT1[256];




/**************************************************************

	FUNCTION NAME		: gdi_layer_Merge_frame_buffer()

  	PURPOSE				: bitBlt layers.

	INPUT PARAMETERS	: lcd_para

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

***************************************************************/
#ifdef MMI_ON_HARDWARE_P
#include "hal_debug.h"
#endif
//API for cool profile

#define CP_Merge_ENTRY(id) hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_MMI, id)
#define CP_Merge_EXIT(id) hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_MMI, id)
extern U8 gdi_lcd_get_rotate(void);

#if defined(GDI_USING_LAYER_ROTATE) || defined(GDI_USING_LCD_ROTATE)
extern kal_bool mmi_frm_is_screen_width_height_swapped(void);
#endif

void gdi_layer_Merge_frame_buffer(lcd_frame_update_struct *lcd_para)
{
#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__

	/*----------------------------------------------------------------*/
  /* Local Variables                                                */
  /*----------------------------------------------------------------*/
	U16		x, y;
	U16		y_layer;
	U16		origin_pixel1,origin_pixel2;
	U16		layer_pixel1,layer_pixel2;
	U16		layer_pixel_index;		/* used for LUT */	
	U8			*index_ptr;					/* used for LUT, get index value */
	U8 			*layer_ycol_ptr;
	U16		*buffer_ptr;
	U16		*shadow_ptr;
	U16		*layer_sim_LUT_tmp;
	BOOL		source_enable;
	BOOL		opacity_enable;
	BOOL		LUT_enable;
	U32		color_r=0, color_g=0, color_b=0;
	U8			op_value;					/* opacity value*/
	U8			op_value_c;					/* opacity value complement */
	U32		layer_flag_table[4];
	U8			layer_id;
	S32		roi_width;
	S32		roi_height;

	U8		lcd_id;
	U16		start_x;
	U16		start_y; 
	U16		end_x;
	U16		end_y;
	U16		roi_offset_x; 
	U16		roi_offset_y; 
	U16		roi_offset_x_end; 
	U16		roi_offset_y_end; 
	U16		layer_x_end; 
	U16		layer_y_end; 

	U16		*shadow_ptr_t;
	U16		*buffer_ptr_t;
	U16		*layer_ycol_ptr_t;
	U8		  Opacity_merge_flag;
	
	DWORD		update_layer;
	U8			is_first_layer;
	UINT16  xEnd=0,yEnd=0;    //xStart=0,yStart=0,
	lcd_layer_struct *layer_ptr;		/* use this ptr to make code shorter */

        U16 lcd_width = 0;

//	hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_HAL, 0x29b0);
#ifdef MMI_ON_HARDWARE_P
CP_Merge_ENTRY(0x1f20);
#endif
#if defined(GDI_USING_LAYER_ROTATE) || defined(GDI_USING_LCD_ROTATE)
    if(mmi_frm_is_screen_width_height_swapped())
    { 
  //      mmi_trace(1," gdi_layer_Merge_frame_buffer  SCREEN ROTATED !!!  ");
        lcd_width = LCD_HEIGHT;
    }
    else
#endif
    { 
   //     mmi_trace(1," gdi_layer_Merge_frame_buffer  SCREEN NOT ROTATED  !!!! ");

        lcd_width = LCD_WIDTH;
    }
  /*----------------------------------------------------------------*/
  /* Code Body                                                      */
  /*----------------------------------------------------------------*/


	lcd_id			= lcd_para->lcd_id;
	start_x			= lcd_para->lcm_start_x;
	start_y			= lcd_para->lcm_start_y;
	end_x				= lcd_para->lcm_end_x;
	end_y				= lcd_para->lcm_end_y;
	roi_offset_x	= lcd_para->roi_offset_x;
	roi_offset_y	= lcd_para->roi_offset_y;
	update_layer      	= lcd_para->update_layer;

//	  if(roi_offset_x > lcd_width) MMI_ASSERT(0);
//	  if(roi_offset_y > LCD_HEIGHT) MMI_ASSERT(0);
	  
	/* create this table, so we can use for loop to prcess each layer */
	layer_flag_table[0] = LCD_LAYER0_ENABLE;
	layer_flag_table[1] = LCD_LAYER1_ENABLE;
	layer_flag_table[2] = LCD_LAYER2_ENABLE;
	layer_flag_table[3] = LCD_LAYER3_ENABLE;

	roi_width	= end_x - start_x;
	roi_height	= end_y - start_y;
	
	is_first_layer = 1;

   //     mmi_trace(1," gdi_layer_Merge_frame_buffer  roi_offset_y = %d    !!!! ",roi_offset_y);

	for (layer_id = 0 ; layer_id <GDI_LAYER_TOTAL_LAYER_COUNT; layer_id++)
	{

		/* check if this layer need to merge or not */
		if ((update_layer & layer_flag_table[layer_id]) == 0) continue;

		shadow_ptr = (U16 *)simulator_Merge_buffer + start_y * lcd_width- roi_offset_x + start_x;
		index_ptr		= (U8 *)lcd_layer_data[layer_id].frame_buffer_address;
		layer_ptr		= &lcd_layer_data[layer_id];
		source_enable	  = layer_ptr->source_key_enable;
		opacity_enable = layer_ptr->opacity_enable;
		LUT_enable	  = layer_ptr->color_palette_enable;
		buffer_ptr		= (U16 *)lcd_layer_data[layer_id].frame_buffer_address+( roi_offset_y - layer_ptr->y_offset)*layer_ptr->column_number;

	
			roi_offset_x_end = roi_offset_x+roi_width+1; 
			roi_offset_y_end = roi_offset_y+roi_height+1; 
			layer_x_end = layer_ptr->x_offset + layer_ptr->column_number; 
			layer_y_end = layer_ptr->y_offset + layer_ptr->row_number; 
			
			/*chenhe:pixtel would be blend base on 2 condition:
			   1,during the matrix roi_offset_x,roi_offset_x_end,roi_offset_y,roi_offset_y_end
			   2,during the matrix layer_ptr->x_offset,,layer_x_end,layer_ptr->y_offset,layer_y_end

			   so that we only count out a smaller matrix which is the 2 matrix overlap
			*/
			xEnd = roi_offset_x_end<layer_x_end?roi_offset_x_end:layer_x_end;
			yEnd = roi_offset_y_end<layer_y_end?roi_offset_y_end:layer_y_end;
			//mmi_trace(1,"chenhe, %d,%d", xEnd,yEnd );
       //             mmi_trace(1," gdi_layer_Merge_frame_buffer  yEnd = %d    !!!! ",yEnd);

			if(layer_ptr->x_offset>=xEnd || layer_ptr->y_offset>=yEnd)
			continue;

			//caculate op_value and op_value_c
			
			/* calc opacity ratio */
			op_value   = layer_ptr->opacity_value << 3;
			op_value_c = ~op_value;
			
			if ( layer_ptr->color_palette_select == 0)
				layer_sim_LUT_tmp = (U16*)layer_sim_LUT0;
			else 
				layer_sim_LUT_tmp = (U16*)layer_sim_LUT1;	
				
			origin_pixel1 = 0xffff;
			origin_pixel2 = 0xffff;
			
			if(opacity_enable == KAL_TRUE && op_value!=0 && lcd_id == MAIN_LCD)
				Opacity_merge_flag = 1;
			else 
				Opacity_merge_flag = 0;

			for (y = roi_offset_y ; y <  yEnd; y++)
			{
					if (y >= layer_ptr->y_offset)
					{
						y_layer = y - layer_ptr->y_offset;
						
						layer_ycol_ptr = index_ptr + (y_layer * layer_ptr->column_number);
						
						/*convert to 32 bit address*/
						shadow_ptr_t = (U16*)(shadow_ptr + roi_offset_x);
						buffer_ptr_t   = (U16*)(buffer_ptr + roi_offset_x - layer_ptr->x_offset);
						layer_ycol_ptr_t = (U16*)(layer_ycol_ptr + roi_offset_x - layer_ptr->x_offset);
						
					
						for (x = roi_offset_x; x < xEnd; x+=2)
						{					
							if (x >= layer_ptr->x_offset)
							{		
								if (!is_first_layer)
								{
									origin_pixel1 = *shadow_ptr_t;
									origin_pixel2 = *(shadow_ptr_t+1);
								}
					
								if (layer_ptr->color_palette_enable == KAL_TRUE)
								{
									layer_pixel_index = *(layer_ycol_ptr_t++);
									layer_pixel1= layer_sim_LUT_tmp[layer_pixel_index];
									layer_pixel_index = *(layer_ycol_ptr_t++);
									layer_pixel2= layer_sim_LUT_tmp[layer_pixel_index];
								}
								else
								{
									layer_pixel1 = *(buffer_ptr_t++);
									layer_pixel2 = *(buffer_ptr_t++);
								}
								
								if (source_enable == KAL_TRUE && layer_ptr->source_key == layer_pixel1)
								{
									*(shadow_ptr_t++) = origin_pixel1;
								}
								/* opacity check */
								else
								{
									if (Opacity_merge_flag)
									{
										color_r = (origin_pixel1& 0xf800) *op_value_c + (layer_pixel1 & 0xf800) * op_value;
										color_g = (origin_pixel1& 0x07e0) *op_value_c + (layer_pixel1 & 0x07e0) * op_value;
										color_b = (origin_pixel1& 0x001f) *op_value_c + (layer_pixel1 & 0x001f) * op_value;	
 										layer_pixel1   = ((color_r>>8) & 0xf800 )| ((color_g>>8) & 0x07e0)|(color_b>>8);
									}
									*(shadow_ptr_t++) = layer_pixel1;
								}/* for opacity check */
						
								if (source_enable == KAL_TRUE && layer_ptr->source_key == layer_pixel2)
								{
									*(shadow_ptr_t++) = origin_pixel2;
								}
								/* opacity check */
								else
								{
									if (Opacity_merge_flag)
									{
										color_r = (origin_pixel2& 0xf800) *op_value_c + (layer_pixel2 & 0xf800) * op_value;
										color_g = (origin_pixel2& 0x07e0) *op_value_c + (layer_pixel2 & 0x07e0) * op_value;
										color_b = (origin_pixel2& 0x001f) *op_value_c + (layer_pixel2 & 0x001f) * op_value;	
 										layer_pixel2   = ((color_r>>8) & 0xf800 )| ((color_g>>8) & 0x07e0)|(color_b>>8);
									}
									*(shadow_ptr_t++) = layer_pixel2;
								}/* for opacity check */
								
							} /* if within x region */			
							else
							{
								shadow_ptr_t +=2;
								buffer_ptr_t +=2;
								layer_ycol_ptr_t+=2;	/* increment address */
							}
						} /* for loop  x */
					}/* if within y region  */
				
				shadow_ptr = shadow_ptr + lcd_width;
					buffer_ptr = buffer_ptr + layer_ptr->column_number;

			} /* for loop - y */
			is_first_layer = 0;
	} /* for each layer */

#ifndef MMI_ON_WIN32
//	     lcd_frame_buffer_address = (U32)simulator_Merge_buffer;
#endif
#ifdef MMI_ON_HARDWARE_P
CP_Merge_EXIT(0x1f20);
#endif
#endif

} 
/*wangrui and fengzb add 20070614 end*/




