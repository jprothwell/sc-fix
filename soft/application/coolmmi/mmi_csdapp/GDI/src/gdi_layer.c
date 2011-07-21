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

#if defined(GDI_USING_LCD_ROTATE) || defined(GDI_USING_LAYER_ROTATE)
#include "screenrotationgprot.h"
#endif /* defined(GDI_USING_LCD_ROTATE) */ 

#if GDI_MAIN_BASE_LAYER_FORMAT==GDI_COLOR_FORMAT_32
	#define GDI_MAINLCD_BUFFER_SIZE	((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*32+7)>>3)
#elif GDI_MAIN_BASE_LAYER_FORMAT==GDI_COLOR_FORMAT_24
	#define GDI_MAINLCD_BUFFER_SIZE	((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*24+7)>>3)
#elif GDI_MAIN_BASE_LAYER_FORMAT==GDI_COLOR_FORMAT_16
	#define GDI_MAINLCD_BUFFER_SIZE	((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*16+7)>>3)
#else
    #define GDI_MAINLCD_BUFFER_SIZE  ((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL+7)>>3)
#endif


#ifdef __MMI_SUBLCD__
	#if GDI_SUB_BASE_LAYER_FORMAT== GDI_COLOR_FORMAT_32
		#define GDI_SUBLCD_BUFFER_SIZE	((GDI_SUBLCD_WIDTH*GDI_SUBLCD_HEIGHT*32+7)>>3)
    #elif GDI_SUB_BASE_LAYER_FORMAT==GDI_COLOR_FORMAT_24
	    #define GDI_SUBLCD_BUFFER_SIZE	((GDI_SUBLCD_WIDTH*GDI_SUBLCD_HEIGHT*24+7)>>3)
    #elif GDI_SUB_BASE_LAYER_FORMAT==GDI_COLOR_FORMAT_16
	    #define GDI_SUBLCD_BUFFER_SIZE	((GDI_SUBLCD_WIDTH*GDI_SUBLCD_HEIGHT*16+7)>>3)
	#else
        #define GDI_SUBLCD_BUFFER_SIZE   ((GDI_SUBLCD_WIDTH*GDI_SUBLCD_HEIGHT*GDI_SUBLCD_BIT_PER_PIXEL+7)>>3)
	#endif
#else 
#define GDI_SUBLCD_BUFFER_SIZE   0
#endif 

#define GDI_LAYER_BUFFER_SIZE    ((GDI_MAINLCD_BUFFER_SIZE*(GDI_MAINLCD_MAX_LAYERS-1)) \
                             +(GDI_SUBLCD_BUFFER_SIZE* GDI_SUBLCD_MAX_LAYERS))

#define GDI_GRAY_BOUNDARY 210

#define	BUF_PIXEL_DATA(buff_ptr,x , y, width)				(*(buff_ptr + ((y) * width + (x))))
#define	RGB_TO_HW(R, G, B)								((((B) & 0xf8) >> 3) | (((G) & 0xfc) << 3) | (((R) & 0xf8) << 8))
#define	HW_TO_RGB_R(VALUE)							((U8)(((VALUE) & 0xf800) >> 8))
#define	HW_TO_RGB_G(VALUE)							((U8)(((VALUE) & 0x07e0) >> 3))
#define	HW_TO_RGB_B(VALUE)							((U8)(((VALUE) & 0x001f) << 3))

/****************************************************************************
* MACRO
*****************************************************************************/
/* Config hardware layers - only used by gdi_layer_blt() */
#define CONFIG_HARDWARE_LAYER(ID)                                                                       \
   do                                                                                                   \
   {                                                                                                    \
      if(handle##ID >= GDI_LAYER_VALID_HANDLE_START )                                                   \
      {                                                                                                 \
         GDI_DEBUG_ASSERT(GDI_LAYER_GET_FLAG(handle##ID,GDI_LAYER_FLAG_USED));                          \
         config_lcd_layer_window(LCD_LAYER##ID, &(gdi_layer_info[((gdi_layer_struct*)handle##ID)->id]));\
         background = ((gdi_layer_struct*)handle##ID)->background;                                      \
         layer_flag |= LCD_LAYER##ID##_ENABLE;                                                          \
      }                                                                                                 \
   }while(0)

#define SHIFT_LAYER_OFFSET(ID,X,Y)                                         \
   do{                                                                     \
      if(handle##ID >= GDI_LAYER_VALID_HANDLE_START )                      \
      {                                                                    \
         gdi_layer_info[((gdi_layer_struct*)handle##ID)->id].x_offset += X;\
         gdi_layer_info[((gdi_layer_struct*)handle##ID)->id].y_offset += Y;\
      }                                                                    \
   }while(0)

#define LIMIT_BOUND(pos, min_value, max_value) \
   do{                                         \
      if(pos < min_value)                      \
         pos = min_value;                      \
      else if(pos > max_value)                 \
         pos = max_value;                      \
   }while(0)

#define SET_HARDWARE_LAYER_ROTATE(ID) \
	do \
	{\
		if(handle##ID >= GDI_LAYER_VALID_HANDLE_START )\
		{\
			U8 layer_actual_rotate_value = (U8)((((GDI_LCD_LAYER_ROTATE_MIRROR_MASK & gdi_act_layer->rotate_value) | \
							(0x04 - GDI_LCD_LAYER_ROTATE_ANGLE_MASK & gdi_act_layer->rotate_value)) + \
							((gdi_layer_struct*)handle##ID)->rotate_value) & \
							GDI_LCD_LAYER_ROTATE_ALL_MASK); \
			GDI_DEBUG_ASSERT(GDI_LAYER_GET_FLAG(handle##ID,GDI_LAYER_FLAG_USED));\
			gdi_layer_set_flatten_rotate(handle##ID,layer_actual_rotate_value); \
		}\
	}while(0)
	
#ifdef MMI_ON_WIN32
#define WIN32_UI_SET_LAYER_DATA(id)         \
   {                                           \
      if (handle##id != GDI_LAYER_EMPTY_HANDLE)\
      {                                        \
         win32_ui_set_layer_data(              \
            id,                                \
            lcd_update_frame.lcd_id,           \
            GDI_LAYER.clipx1,                  \
            GDI_LAYER.clipy1,                  \
            GDI_LAYER.clipx2,                  \
            GDI_LAYER.clipy2,                  \
            GDI_LAYER.clipx1,                  \
            GDI_LAYER.clipy1,                  \
            GDI_LAYER.clipx2,                  \
            GDI_LAYER.clipy2,                  \
            handle##id,                        \
            GDI_LCD->act_handle                \
         );                                    \
      }                                        \
   }
#endif /* MMI_ON_WIN32 */ 
///////////////////////////////////////////
// buffer pool 

#if 0
/* the base layer should be in internal ram */
#ifdef CSD_TARGET
#pragma arm section rwdata = "INTERNRW1", zidata = "INTERNZI1"
#endif 
#endif

U32 gdi_base_layer_buffer_pool[(GDI_MAINLCD_BUFFER_SIZE + 3) / 4];
#if 0
#ifdef CSD_TARGET
#pragma arm section rwdata , zidata
#endif 
#endif
/* others layers should be external ram */
U32 gdi_layer_buffer_pool[((GDI_LAYER_BUFFER_SIZE) / 4) + 1];

#if defined GDI_SUPPORT_PALETTES
gdi_color gdi_layer_palette[GDI_SUPPORT_PALETTES][256];
#endif 

static U8 *gdi_layer_buf_pool_start_ptr = NULL;
static U8 *gdi_layer_buf_pool_end_ptr = NULL;

static BOOL gdi_is_layer_enable = FALSE;
static gdi_handle gdi_layer_stack[GDI_LAYER_ACT_LAYER_STACK_COUNT];
static S32 gdi_layer_stack_index;
//static gdi_layer_struct *gdi_layer_last_root = NULL;

gdi_layer_struct GDI_LAYERS[GDI_LAYER_TOTAL_LAYER_COUNT];
gdi_layer_struct *gdi_act_layer=NULL;
gd_color_from_rgb_func gdi_act_color_from_rgb;
gd_color_to_rgb_func gdi_act_color_to_rgb;
gd_put_pixel_func gdi_act_put_pixel;
gd_get_pixel_func gdi_act_get_pixel;
gd_replace_src_key_func gdi_act_replace_src_key;

lcd_layer_struct gdi_layer_info[GDI_LAYER_TOTAL_LAYER_COUNT];   /* this struct is used for output lcd */
lcd_layer_struct *gdi_act_layer_info=NULL;
static int gdi_palette_used;
static BOOL gdi_is_non_block_blt;
static S32 gdi_draw_section_level;
static BOOL gdi_is_blting;
gdi_handle gdi_work_buffer_handle;

/****************************************************************************
* Function Pre-declare
*****************************************************************************/

#ifdef MMI_ON_WIN32
extern void win32_ui_set_layer_data(
                U8 layer_index,
                U8 lcd_id,
                S32 text_clip_left,
                S32 text_clip_top,
                S32 text_clip_right,
                S32 text_clip_bottom,
                S32 gfx_clip_left,
                S32 gfx_clip_top,
                S32 gfx_clip_right,
                S32 gfx_clip_bottom,
                S32 current_handle,
                S32 active_handle);

extern void win32_ui_set_new_application_flag();
#endif /* MMI_ON_WIN32 */ 


/*wangrui and fengzb add 20070614 begin for support  simu multi layer by soft*/
#define LCD_MAX_OPACITY          0x1f
#if defined(__PRJ_WITH_SPILCD__) || (!defined(CHIP_HAS_GOUDA))
U16			simulator_Merge_buffer[GDI_LCD_WIDTH * GDI_LCD_HEIGHT];    
#endif
#ifdef __MMI_SUBLCD__
U16			simulator_sub_Merge_buffer[GDI_SUBLCD_WIDTH * GDI_SUBLCD_HEIGHT];
#endif
lcd_layer_struct	lcd_layer_data[4];
//U16					layer_sim_LUT0[256];
//U16					layer_sim_LUT1[256];
extern U32 lcd_frame_buffer_address;

extern void gdi_layer_Merge_frame_buffer(lcd_frame_update_struct *);
/**************************************************************

	FUNCTION NAME		: gdi_layer_Merge_frame_buffer_forPartVideo()

  	PURPOSE				: bitBlt layers.

	INPUT PARAMETERS	: lcd_para

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

***************************************************************/

void gdi_layer_Merge_frame_buffer_forPartVideo(U16 lcd_y)
{
//#ifndef CHIP_HAS_GOUDA
#if defined(__PRJ_WITH_SPILCD__) || (!defined(CHIP_HAS_GOUDA))

		U16		*buffer_ptr;
#if defined(GDI_USING_LAYER_ROTATE) || defined(GDI_USING_LCD_ROTATE)
	if(mmi_frm_is_screen_width_height_swapped())
	{
		buffer_ptr		= (U16 *)lcd_layer_data[0].frame_buffer_address+ GDI_LCD_HEIGHT*lcd_y;
		
	 	memcpy(simulator_Merge_buffer + GDI_LCD_HEIGHT*lcd_y,(const void *) buffer_ptr, 
	 		(GDI_LCD_WIDTH * GDI_LCD_HEIGHT -GDI_LCD_HEIGHT*lcd_y)*2 );
	}
    else
#endif
	{
		buffer_ptr		= (U16 *)lcd_layer_data[0].frame_buffer_address+ GDI_LCD_WIDTH*lcd_y;
		
	     memcpy(simulator_Merge_buffer + GDI_LCD_WIDTH*lcd_y,(const void *) buffer_ptr, 
		 	(GDI_LCD_WIDTH * GDI_LCD_HEIGHT -GDI_LCD_WIDTH*lcd_y)*2 );
	}
#ifndef MMI_ON_WIN32
	     lcd_frame_buffer_address = (U32)simulator_Merge_buffer;
#endif
#endif
} 

/**************************************************************

	FUNCTION NAME		: gdi_layer_Merge_frame_buffer()

  	PURPOSE				: bitBlt layers.

	INPUT PARAMETERS	: lcd_para

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

***************************************************************/
#if 0
#include "hal_debug.h"
//API for cool profile

#define CP_Merge_ENTRY(id) hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_MMI, id)
#define CP_Merge_EXIT(id) hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_MMI, id)


void gdi_layer_Merge_frame_buffer(lcd_frame_update_struct *lcd_para)
{
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

//	hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_HAL, 0x29b0);

CP_Merge_ENTRY(0x1f20);
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

	/* create this table, so we can use for loop to prcess each layer */
	layer_flag_table[0] = LCD_LAYER0_ENABLE;
	layer_flag_table[1] = LCD_LAYER1_ENABLE;
	layer_flag_table[2] = LCD_LAYER2_ENABLE;
	layer_flag_table[3] = LCD_LAYER3_ENABLE;

	roi_width	= end_x - start_x;
	roi_height	= end_y - start_y;
	
	is_first_layer = 1;
	for (layer_id = 0 ; layer_id <GDI_LAYER_TOTAL_LAYER_COUNT; layer_id++)
	{

		/* check if this layer need to merge or not */
		if ((update_layer & layer_flag_table[layer_id]) == 0) continue;

		shadow_ptr = (U16 *)simulator_Merge_buffer + start_y *LCD_WIDTH- roi_offset_x + start_x;
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
				
					shadow_ptr = shadow_ptr + LCD_WIDTH;
					buffer_ptr = buffer_ptr + layer_ptr->column_number;

			} /* for loop - y */
			is_first_layer = 0;
	} /* for each layer */

#ifndef MMI_ON_WIN32
//	     lcd_frame_buffer_address = (U32)simulator_Merge_buffer;
#endif
	
CP_Merge_EXIT(0x1f20);
} 
/*wangrui and fengzb add 20070614 end*/
#endif


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_find_valid_handle
 * DESCRIPTION
 *  Init the Frame Buffer Manager
 *  
 *  Will return handle of a valid layer.
 *  If no valid layer, will retrun GDI_LAYER_ERROR_HANDLE.
 * PARAMETERS
 *  void
 * RETURNS
 *  gdi_handle
 *****************************************************************************/
static gdi_handle gdi_layer_find_valid_handle(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = GDI_LAYER_USER_ALLOCATE_HANDLE_START; i < GDI_LAYER_TOTAL_LAYER_COUNT; i++)
        if (!GDI_LAYER_GET_FLAG(&GDI_LAYERS[i], GDI_LAYER_FLAG_USED))
        {
            return (gdi_handle) & GDI_LAYERS[i];
        }

    return GDI_LAYER_EMPTY_HANDLE;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_allocate_buffer
 * DESCRIPTION
 *  assign buffer of the pass in size to a layerr
 *  
 *  return alloacted buffer ptr if succeed, otherwise return NULL
 * PARAMETERS
 *  cf              [IN]        
 *  width           [IN]        
 *  height          [IN]        
 *  ret_size        [?]         
 *  size(?)         [IN]        
 * RETURNS
 *  U8*
 *****************************************************************************/
static U8 *gdi_layer_allocate_buffer(gdi_color_format cf, S32 width, S32 height, S32 *ret_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;
    U8 *next_available_ptr;
    U8 *tmp_ptr;
    S32 size;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    size = gdi_sizeof_pixels(cf, width, height);

    /* allcoate buffer from pool, align buffer_size to 4 U8 alignment */
    size += 3;
    size &= ~3;

    next_available_ptr = (U8*) gdi_layer_buffer_pool;   /* point to start of the pool */

    /* iterate all layer, find last allocated position */
    for (i = 0; i < GDI_LAYER_TOTAL_LAYER_COUNT; i++)
    {
        U32 x = GDI_LAYER_GET_FLAG(&GDI_LAYERS[i], GDI_LAYER_FLAG_USED | GDI_LAYER_FLAG_USE_OUTSIDE_MEMORY);

        if (x == GDI_LAYER_FLAG_USED)   /* used & !outside memory */
        {
            tmp_ptr = GDI_LAYERS[i].buf_ptr + GDI_LAYERS[i].layer_size;
            if (tmp_ptr > next_available_ptr)
            {
                next_available_ptr = tmp_ptr;
            }

            if (GDI_LAYER_GET_FLAG(&(GDI_LAYERS[i]), GDI_LAYER_FLAG_DOUBLE_LAYER))
            {
                tmp_ptr = GDI_LAYERS[i].buf_ptr1 + GDI_LAYERS[i].layer_size;
                if (tmp_ptr > next_available_ptr)
                {
                    next_available_ptr = tmp_ptr;
                }
            }
        }
    }
    if (next_available_ptr + size - 1 > gdi_layer_buf_pool_end_ptr)     /* out of valid buffer pool region */
    {
        /* no more buffer */
        GDI_ASSERT(0);
        return (NULL);
    }

    *ret_size = size;
    /* if within valid region, assign buffer */
    return (next_available_ptr);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_create_internal
 * DESCRIPTION
 *  Init a layer.
 *  
 *  Init layer as most basic layer, which does not
 *  support src_key, opacity, etc.
 *  This function will retrun FALSE if it cant allocate buffer.
 * PARAMETERS
 *  handle              [IN]        
 *  offset_x            [IN]        
 *  offset_y            [IN]        
 *  width               [IN]        
 *  height              [IN]        
 *  cf                  [IN]        
 *  is_double_layer     [IN]        
 *  buf_ptr             [?]         
 *  buf_ptr1            [?]         
 *  buf_size            [IN]        
 *  lcd_handle(?)       [IN]        
 * RETURNS
 *  void
 * BOOL
 *  
 *****************************************************************************/
static void gdi_layer_create_internal(
                gdi_handle handle,
                S32 offset_x,
                S32 offset_y,
                S32 width,
                S32 height,
                gdi_color_format cf,
                BOOL is_double_layer,
                U8 *buf_ptr,
                U8 *buf_ptr1,
                U32 buf_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gdi_layer_struct *layer = (gdi_layer_struct*) handle;
    int id = layer->id;
    lcd_layer_struct *layer_info = &gdi_layer_info[id];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    memset(layer, 0, sizeof(gdi_layer_struct));
    memset(layer_info, 0, sizeof(lcd_layer_struct));
    layer->id = id;

    GDI_LAYER_SET_FLAG(layer, GDI_LAYER_FLAG_USED);

    layer->cf = cf;
    layer->vcf = gdi_get_color_format(gdi_bits_per_pixel(cf));  /* convert to standard color format */
    if (GDI_COLOR_FORMAT_32 == layer->vcf)
    {
        layer_info->opacity_enable = TRUE;
    }
    GDI_DEBUG_ASSERT(layer->vcf < GDI_COLOR_FORMAT_STANDARD_END);
#ifdef GDI_SUPPORT_PALETTES
    if (layer->vcf == GDI_COLOR_FORMAT_8)   /* use index color */
    {
        int i;

        /* we should check there are palette available or not ! */
        for (i = 0; i < GDI_SUPPORT_PALETTES; i++)
            if ((gdi_palette_used & (1 << i)) == 0)
            {
                break;
            }

        if (i >= GDI_SUPPORT_PALETTES)
        {
            i = GDI_SUPPORT_PALETTES - 1;
        }

        gdi_palette_used |= (1 << i);

        layer_info->color_palette_enable = TRUE;
        layer_info->color_palette_select = i;
    }
#endif /* GDI_SUPPORT_PALETTES */ 
    layer->buf_ptr = buf_ptr;
    GDI_ASSERT(layer->buf_ptr); /* buffer not available */
    if (is_double_layer)
    {
        GDI_LAYER_SET_FLAG(layer, GDI_LAYER_FLAG_DOUBLE_LAYER);
        layer->buf_ptr1 = buf_ptr1;
        GDI_ASSERT(layer->buf_ptr1);    /* buffer not available */
    }

    layer->width = (U16) width;
    layer->height = (U16) height;
    layer->bits_per_pixel = (U8) gdi_bits_per_pixel(cf);
    layer->layer_size = (U32) buf_size;

    layer->offset_x = offset_x;
    layer->offset_y = offset_y;

    /* set color format function */
    GDI_DEBUG_ASSERT(cf < GDI_COLOR_FORMAT_END);

    /* set clipping */
    GDI_LAYER_RESET_CLIPS(layer);
    GDI_LAYER_SET_CLIP(layer, 0, 0, width - 1, height - 1);

    // default layer background is WHITE
     
    layer->background = gd_color_from_rgb_24(255, 255, 255, 255);

    /* init lcd_layer */
    layer_info->column_number = (U16) width;
    layer_info->frame_buffer_address = (U32) layer->buf_ptr;
    //              layer_info->layer_update_queue          = FALSE;
    //              layer_info->opacity_enable                      = FALSE;
    //              layer_info->opacity_value                       = 0;
#if defined(GDI_USING_LAYER_ROTATE)
    layer_info->rotate_value = LCD_LAYER_ROTATE_NORMAL;
#endif 
    layer_info->row_number = height;
    //              layer_info->source_key                          = 0;
    //              layer_info->source_key_enable           = FALSE;
    layer_info->x_offset = (U16) (offset_x + GDI_LAYER_OFFSET);
    layer_info->y_offset = (U16) (offset_y + GDI_LAYER_OFFSET);

    {
        U8 hw_cf;

        switch (layer->vcf)
        {
            case GDI_COLOR_FORMAT_8:
                hw_cf = GDI_HW_COLOR_FORMAT_8;
                break;
            case GDI_COLOR_FORMAT_16:
                hw_cf = GDI_HW_COLOR_FORMAT_16;
                break;
            case GDI_COLOR_FORMAT_24:
                hw_cf = GDI_HW_COLOR_FORMAT_24;
                break;
            case GDI_COLOR_FORMAT_32:
                hw_cf = GDI_HW_COLOR_FORMAT_32;
                break;
            default:
                hw_cf = GDI_HW_COLOR_FORMAT_16; /* /TODO: This layer should not used to bitblt !! */
        }
        layer_info->source_color_format = hw_cf;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_init
 * DESCRIPTION
 *  Init the Frame Buffer Manager
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
GDI_RESULT gdi_layer_init(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_init)
    gdi_color_format cf;

#ifdef __MMI_SUBLCD__
    U8 *buf_ptr;
    S32 buf_size;
#endif /* __MMI_SUBLCD__ */ 
    int i;

    memset(&GDI_LAYERS, 0, sizeof(GDI_LAYERS));
    memset(&gdi_layer_info, 0, sizeof(gdi_layer_info));
    for (i = 0; i < GDI_LAYER_TOTAL_LAYER_COUNT; i++)
    {
        GDI_LAYERS[i].id = i;
    }

    /* init layer stack */
    gdi_layer_stack_index = 0;

    /* init memory pool ptr */
    gdi_layer_buf_pool_start_ptr = (U8*) gdi_layer_buffer_pool;
    gdi_layer_buf_pool_end_ptr = ((U8*) gdi_layer_buffer_pool);
    gdi_layer_buf_pool_end_ptr += GDI_LAYER_BUFFER_SIZE - 1;    /* point to last valid U8 */

    /* non_block blt */
    gdi_is_non_block_blt = FALSE;
    gdi_draw_section_level = 0;
    gdi_is_blting = FALSE;

#ifdef __MMI_SUBLCD__
    /* init sub lcd base layer */
    cf = GDI_SUB_BASE_LAYER_FORMAT;
    switch(cf)
    {
    case GDI_COLOR_FORMAT_16:
        if(GDI_SUBLCD_BUFFER_FORMAT(0x12,0x34,0x56) != gd_color_from_rgb_16(0x00,0x12,0x34,0x56))
            cf = GDI_COLOR_FORMAT_SUBLCD;
        break;
    }

    buf_ptr = gdi_layer_allocate_buffer(cf, GDI_SUBLCD_WIDTH, GDI_SUBLCD_HEIGHT, &buf_size);
    gdi_layer_create_internal(
        GDI_LAYER_SUB_BASE_LAYER_HANDLE,
        0,
        0,
        GDI_SUBLCD_WIDTH,
        GDI_SUBLCD_HEIGHT,
        cf,
        FALSE,
        buf_ptr,
        NULL,
        buf_size);
    gdi_layer_set_active(GDI_LAYER_MAIN_BASE_LAYER_HANDLE);
    gdi_layer_clear(DRV_SUBLCD_INIT_COLOR);
#endif /* __MMI_SUBLCD__ */ 

    /* init mian lcd base layer */
    /*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-12 15:6 */
    #if 0
    cf = GDI_MAIN_BASE_LAYER_FORMAT;
    #else
    cf = GDI_COLOR_FORMAT_MAINLCD;
    #endif 
    /*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-12 15:6 */
    switch(cf)
    {
    case GDI_COLOR_FORMAT_16:
        if(GDI_RGB_TO_BUFFER_FORMAT(0x12,0x34,0x56) != gd_color_from_rgb_16(0x00,0x12,0x34,0x56))
            cf = GDI_COLOR_FORMAT_MAINLCD;
        break;
    }
    gdi_layer_create_internal(
        GDI_LAYER_MAIN_BASE_LAYER_HANDLE,
        0,
        0,
        GDI_LCD_WIDTH,
        GDI_LCD_HEIGHT,
        cf,
        FALSE,
        (U8*) gdi_base_layer_buffer_pool,
        NULL,
        sizeof(gdi_base_layer_buffer_pool));
    GDI_LAYER_SET_FLAG(GDI_LAYER_MAIN_BASE_LAYER_HANDLE, GDI_LAYER_FLAG_USE_OUTSIDE_MEMORY);
    gdi_layer_set_active(GDI_LAYER_MAIN_BASE_LAYER_HANDLE);
    gdi_layer_clear(DRV_MAINLCD_INIT_COLOR);

    /* set main LCD active */
    gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);

#if defined(GDI_USING_LAYER)
    if (gdi_layer_create_cf_using_outside_memory
        (GDI_WORK_BUFFER_FORMAT, 0, 0, GDI_LCD_WIDTH, GDI_LCD_HEIGHT, &gdi_work_buffer_handle, (U8*) gdi_work_buffer,
         gdi_work_buffer_size) < 0)
    {
        GDI_ASSERT(0);  /* can't create working buffer handle */
    }
#endif /* defined(GDI_USING_LAYER) */ 

    /* misc, some flags */
    gdi_is_layer_enable = FALSE;

    /* no palette is allocated */
    gdi_palette_used = 0;

    GDI_RETURN(GDI_SUCCEED);

    GDI_EXIT_CRITICAL_SECTION(gdi_layer_init)
}

/**************************************************************
   Create layer with color format
   INPUT PARAMETERS  :  
      cf
      offset_x
      offset_y
      width
      height
***************************************************************/


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_create_cf
 * DESCRIPTION
 *  
 * PARAMETERS
 *  cf              [IN]        
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  width           [IN]        
 *  height          [IN]        
 *  handle_ptr      [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_layer_create_cf(
            gdi_color_format cf,
            S32 offset_x,
            S32 offset_y,
            S32 width,
            S32 height,
            gdi_handle *handle_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_create_cf)
    gdi_handle handle;

    *handle_ptr = handle = gdi_layer_find_valid_handle();

    if (handle == GDI_LAYER_EMPTY_HANDLE)
    {
        /* cant create layer */
        GDI_ASSERT(0);
        GDI_RETURN(GDI_LAYER_ERR_NO_VALID_LAYER_HANDLE);
    }
    else
    {
        U8 *buf;
        S32 buf_size;

        buf = gdi_layer_allocate_buffer(cf, width, height, &buf_size);

        gdi_layer_create_internal(handle, offset_x, offset_y, width, height, cf, FALSE, buf, NULL, buf_size);
        GDI_RETURN(GDI_SUCCEED);
    }
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_create_cf)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_create_cf_double
 * DESCRIPTION
 *  Create a layer
 *  
 *  Will return handle of the layer.
 *  If no valid layer, will retrun GDI_LAYER_ERROR_HANDLE.
 * PARAMETERS
 *  cf              [IN]        
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  width           [IN]        
 *  height          [IN]        
 *  handle_ptr      [?]         
 * RETURNS
 *  gdi_handle
 *****************************************************************************/
GDI_RESULT gdi_layer_create_cf_double(
            gdi_color_format cf,
            S32 offset_x,
            S32 offset_y,
            S32 width,
            S32 height,
            gdi_handle *handle_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_create_double)
    gdi_handle handle;

    *handle_ptr = handle = gdi_layer_find_valid_handle();

    if (handle == GDI_LAYER_EMPTY_HANDLE)
    {
        /* cant create layer */
        GDI_ASSERT(0);
        GDI_RETURN(GDI_LAYER_ERR_NO_VALID_LAYER_HANDLE);
    }
    else
    {
        U8 *buf, *buf1;
        S32 buf_size;

        buf = gdi_layer_allocate_buffer(cf, width, height * 2, &buf_size);
        buf_size /= 2;
        buf1 = buf + buf_size;
        /* buf1 = gdi_layer_allocate_buffer(cf,width,height,&buf_size); */

        gdi_layer_create_internal(handle, offset_x, offset_y, width, height, cf, TRUE, buf, buf1, buf_size);
        GDI_RETURN(GDI_SUCCEED);
    }
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_create_double)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_create_cf_double_using_outside_memory
 * DESCRIPTION
 *  Create a layer
 *  
 *  Will return handle of the layer.
 *  If no valid layer, will retrun GDI_LAYER_ERROR_HANDLE.
 * PARAMETERS
 *  cf                          [IN]        
 *  offset_x                    [IN]        
 *  offset_y                    [IN]        
 *  width                       [IN]        
 *  height                      [IN]        
 *  handle_ptr                  [IN]        
 *  outside_memory1             [?]         
 *  outside_memory_size_1       [IN]        
 *  outside_memory2             [?]         
 *  outside_memory_size_2       [IN]        
 *  outside_memory_size(?)      [IN]        
 *  outside_memory(?)           [IN]        
 * RETURNS
 *  gdi_handle
 *****************************************************************************/
GDI_RESULT gdi_layer_create_cf_double_using_outside_memory(
            gdi_color_format cf,
            S32 offset_x,
            S32 offset_y,
            S32 width,
            S32 height,
            gdi_handle *handle_ptr,
            U8 *outside_memory1,
            S32 outside_memory_size_1,
            U8 *outside_memory2,
            S32 outside_memory_size_2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_create_double_using_outside_memory)
    gdi_handle handle;

    *handle_ptr = handle = gdi_layer_find_valid_handle();

    if (handle == GDI_LAYER_EMPTY_HANDLE)
    {
        /* cant create layer */
        GDI_ASSERT(0);
        GDI_RETURN(GDI_LAYER_ERR_NO_VALID_LAYER_HANDLE);
    }
    else
    {
        U8 *buf, *buf1;
        S32 buffer_size;

        buffer_size = gdi_sizeof_pixels(cf, width, height);
        /* allcoate buffer from pool, align buffer_size to 4 U8 alignment */
        buffer_size += 3;
        buffer_size &= ~3;

        //buf = outside_memory;
        //buf1 = (U8*)&outside_memory[offset];
        buf = outside_memory1;
        buf1 = outside_memory2;

        GDI_ASSERT((buffer_size <= outside_memory_size_1) && (buffer_size <= outside_memory_size_2));

        gdi_layer_create_internal(handle, offset_x, offset_y, width, height, cf, TRUE, buf, buf1, buffer_size);
        GDI_LAYER_SET_FLAG(handle, GDI_LAYER_FLAG_USE_OUTSIDE_MEMORY);
        GDI_RETURN(GDI_SUCCEED);
    }
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_create_double_using_outside_memory)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_create_cf_using_outside_memory
 * DESCRIPTION
 *  Create a layer
 *  
 *  Will return handle of the layer.
 *  If no valid layer, will retrun GDI_LAYER_ERROR_HANDLE.
 * PARAMETERS
 *  cf                      [IN]        
 *  offset_x                [IN]        
 *  offset_y                [IN]        
 *  width                   [IN]        
 *  height                  [IN]        
 *  handle_ptr              [?]         
 *  outside_memory          [?]         
 *  outside_memory_size     [IN]        
 * RETURNS
 *  gdi_handle
 *****************************************************************************/
GDI_RESULT gdi_layer_create_cf_using_outside_memory(
            gdi_color_format cf,
            S32 offset_x,
            S32 offset_y,
            S32 width,
            S32 height,
            gdi_handle *handle_ptr,
            U8 *outside_memory,
            S32 outside_memory_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_create_using_outside_memory)
    gdi_handle handle;

    *handle_ptr = handle = gdi_layer_find_valid_handle();

    if (handle == GDI_LAYER_EMPTY_HANDLE)
    {
        /* cant create layer */
        GDI_ASSERT(0);
        GDI_RETURN(GDI_LAYER_ERR_NO_VALID_LAYER_HANDLE);
    }
    else
    {
        U8 *buf;
        S32 buffer_size;

        buffer_size = gdi_sizeof_pixels(cf, width, height);
        GDI_ASSERT(buffer_size <= outside_memory_size);

        buf = outside_memory;

        gdi_layer_create_internal(handle, offset_x, offset_y, width, height, cf, FALSE, buf, NULL, buffer_size);
        GDI_LAYER_SET_FLAG(handle, GDI_LAYER_FLAG_USE_OUTSIDE_MEMORY);
        GDI_RETURN(GDI_SUCCEED);
    }
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_create_using_outside_memory)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_set_active
 * DESCRIPTION
 *  Set a layer as active layer.
 *  
 *  Will set some golbal variable, for faster reference.
 * PARAMETERS
 *  handle      [IN]        
 * RETURNS
 *  void
 * BOOL
 *  
 *****************************************************************************/
void gdi_layer_set_active(gdi_handle handle)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_set_active)
    gdi_act_layer = (gdi_layer_struct*) handle;
    gdi_act_layer_info = &(gdi_layer_info[gdi_act_layer->id]);

    gdi_act_color_from_rgb = gd_color_from_rgb[gdi_act_layer->cf];
    gdi_act_color_to_rgb = gd_color_to_rgb[gdi_act_layer->cf];

    gdi_act_put_pixel = gd_put_pixel[gdi_act_layer->cf];
    gdi_act_get_pixel = gd_get_pixel[gdi_act_layer->cf];
    gdi_act_replace_src_key = gd_replace_src_key[gdi_act_layer->cf];

    GDI_EXIT_CRITICAL_SECTION(gdi_layer_set_active)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_push_and_set_active
 * DESCRIPTION
 *  push current layer into stack and set passin layer active
 * PARAMETERS
 *  handle      [IN]        
 * RETURNS
 *  GDI_RESULT(?)
 *****************************************************************************/
void gdi_layer_push_and_set_active(gdi_handle handle)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_LOCK;
    /* no more stack entry available */
    GDI_ASSERT(gdi_layer_stack_index < GDI_LAYER_ACT_LAYER_STACK_COUNT - 1);

    gdi_layer_stack[gdi_layer_stack_index++] = (gdi_handle) gdi_act_layer;
    gdi_layer_set_active(handle);
    //gdi_act_layer = (gdi_layer_struct*) handle;
    //gdi_act_layer_info = &(gdi_layer_info[gdi_act_layer->id]);

    //gdi_act_color_from_rgb  = gd_color_from_rgb[gdi_act_layer->cf];
    //gdi_act_color_to_rgb    = gd_color_to_rgb[gdi_act_layer->cf];

    //gdi_act_put_pixel = gd_put_pixel[gdi_act_layer->cf];
    //gdi_act_get_pixel = gd_get_pixel[gdi_act_layer->cf];            
    //gdi_act_replace_src_key = gd_replace_src_key[gdi_act_layer->cf];
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_pop_and_restore_active
 * DESCRIPTION
 *  pop the layer in stack and make it active
 * PARAMETERS
 *  void
 * RETURNS
 *  GDI_RESULT(?)
 *****************************************************************************/
void gdi_layer_pop_and_restore_active(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ASSERT(gdi_layer_stack_index > 0);
    gdi_layer_set_active(gdi_layer_stack[--gdi_layer_stack_index]);
    //gdi_act_layer = (gdi_layer_struct*) gdi_layer_stack[--gdi_layer_stack_index];
    //gdi_act_layer_info = &(gdi_layer_info[gdi_act_layer->id]);              

    //gdi_act_color_from_rgb  = gd_color_from_rgb[gdi_act_layer->cf];
    //gdi_act_color_to_rgb    = gd_color_to_rgb[gdi_act_layer->cf];

    //gdi_act_put_pixel = gd_put_pixel[gdi_act_layer->cf];
    //gdi_act_get_pixel = gd_get_pixel[gdi_act_layer->cf];
    //gdi_act_replace_src_key = gd_replace_src_key[gdi_act_layer->cf];
    GDI_UNLOCK;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_get_active
 * DESCRIPTION
 *  get currently active layer handle
 * PARAMETERS
 *  handle_ptr      [OUT]       
 * RETURNS
 *  GDI_RESULT(?)
 *****************************************************************************/
void gdi_layer_get_active(gdi_handle *handle_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_get_active)
    *handle_ptr = (gdi_handle) gdi_act_layer;
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_get_active)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_toggle_double
 * DESCRIPTION
 *  Toggle double buf
 *  
 *  return FALSE if not valid handle.
 * PARAMETERS
 *  void
 * RETURNS
 * BOOL
 *  
 *****************************************************************************/
GDI_RESULT gdi_layer_toggle_double(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_toggle_double)

    /* active layer should be doubled layer */
    if(GDI_LAYER_GET_FLAG(gdi_act_layer, GDI_LAYER_FLAG_DOUBLE_LAYER))
    {
        U8 *tmp;
        /* swap those two buffer */
        tmp = gdi_act_layer->buf_ptr;
        gdi_act_layer->buf_ptr = gdi_act_layer->buf_ptr1;
        gdi_act_layer->buf_ptr1 = tmp;
        
        gdi_act_layer_info->frame_buffer_address = (U32) gdi_act_layer->buf_ptr;
        
        GDI_RETURN(GDI_SUCCEED);
    }
    else
    {
        GDI_RETURN(GDI_FAILED);
    }

    GDI_EXIT_CRITICAL_SECTION(gdi_layer_toggle_double)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_free
 * DESCRIPTION
 *  Release layer
 *  
 *  return FALSE if not valid handle.
 * PARAMETERS
 *  handle      [IN]        
 * RETURNS
 * BOOL
 *  
 *****************************************************************************/
GDI_RESULT gdi_layer_free(gdi_handle handle)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_free)
    gdi_layer_struct *layer = (gdi_layer_struct*) handle;

    GDI_DEBUG_ASSERT(handle >= (gdi_handle) & (GDI_LAYERS[GDI_LAYER_USER_ALLOCATE_HANDLE_START]));

    if (layer->vcf == GDI_COLOR_FORMAT_8)   /* free the palette */
    {
        gdi_palette_used &= ~(1 << gdi_layer_info[layer->id].color_palette_select);
    }
    gdi_anim_stop_all_by_layer((gdi_handle)layer);

    GDI_ASSERT(GDI_LAYER_GET_FLAG(layer, GDI_LAYER_FLAG_USED));
    layer->flag = 0;    /* clear all flag */

    GDI_DEBUG_ASSERT(((gdi_layer_struct*) GDI_LAYER_MAIN_BASE_LAYER_HANDLE)->child != (gdi_layer_struct*) handle);
#ifdef __MMI_SUBLCD__
    GDI_DEBUG_ASSERT(((gdi_layer_struct*) GDI_LAYER_SUB_BASE_LAYER_HANDLE)->child != (gdi_layer_struct*) handle);
#endif 

    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_free)
}


//follow cuijian's suggust
/*****************************************************************************
 * FUNCTION
 *  gdi_layer_free
 * DESCRIPTION
 *  Release layer
 *  
 *  return FALSE if not valid handle.
 * PARAMETERS
 *  handle      [IN]        
 * RETURNS
 * BOOL
 *  
 *****************************************************************************/
GDI_RESULT gdi_layers_free(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
   /* free layer */
   gdi_layer_flatten_previous_to_base();

    for (i = 0; i < GDI_LAYER_TOTAL_LAYER_COUNT; i++)
    {
       GDI_LAYER_SET_FLAG(&GDI_LAYERS[1], GDI_LAYER_FLAG_USED);
	gdi_layer_free(&GDI_LAYERS[1]);
    }

    gdi_layer_multi_layer_disable();	
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_set_position
 * DESCRIPTION
 *  Move the layer to its desired position.
 * PARAMETERS
 *  offset_x            [IN]        
 *  offset_y            [IN]        
 *  gdi_handle(?)       [IN]        Handle
 * RETURNS
 *  void
 *****************************************************************************/
GDI_RESULT gdi_layer_set_position(S32 offset_x, S32 offset_y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#if defined(GDI_USING_LAYER_ROTATE)
    /* S32 x1, y1, x2, y2; */
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_set_position)
    gdi_act_layer->offset_x = offset_x;
    gdi_act_layer->offset_y = offset_y;
    // #if !defined(GDI_USING_LAYER_ROTATE)
    gdi_act_layer_info->x_offset = (U16) (offset_x + GDI_LAYER_OFFSET);
    gdi_act_layer_info->y_offset = (U16) (offset_y + GDI_LAYER_OFFSET);
    //#else
    //x1 = gdi_act_layer->offset_x;
    //y1 = gdi_act_layer->offset_y;
    //x2 = gdi_act_layer->offset_x + gdi_act_layer->width - 1;
    //y2 = gdi_act_layer->offset_y + gdi_act_layer->height - 1;
    //gdi_layer_inverse_map_rotated_region(gdi_act_layer_info->rotate_value, &x1, &y1, &x2, &y2);
    //gdi_act_layer_info->x_offset = x1 + GDI_LAYER_OFFSET;
    //gdi_act_layer_info->y_offset = y1 + GDI_LAYER_OFFSET;
    //#endif

    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_set_position)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_set_clip
 * DESCRIPTION
 *  Set clip region.
 *  
 *  Will set clip region, use layer's own coordinate.
 * PARAMETERS
 *  x1                  [IN]        
 *  y1                  [IN]        
 *  x2                  [IN]        
 *  y2                  [IN]        
 *  gdi_handle(?)       [IN]        Handle
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_set_clip(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_set_clip)
    S16 x_max_limit;
    S16 y_max_limit;

    if (x2 < x1)
    {
        x2 = x1;
    }
    if (y2 < y1)
    {
        y2 = y1;
    }

    x_max_limit = gdi_act_layer->width - 1;
    y_max_limit = gdi_act_layer->height - 1;

    LIMIT_BOUND(x1, 0, x_max_limit);
    LIMIT_BOUND(x2, 0, x_max_limit);
    LIMIT_BOUND(y1, 0, y_max_limit);
    LIMIT_BOUND(y2, 0, y_max_limit);

    GDI_LAYER_SET_CLIP(gdi_act_layer, x1, y1, x2, y2);

    GDI_EXIT_CRITICAL_SECTION(gdi_layer_set_clip)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_reset_clip
 * DESCRIPTION
 *  Reset clip region to layer size
 *  
 *  Will reset clip region to layer's width/height.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_reset_clip(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_reset_clip)
    GDI_LAYER_SET_CLIP(gdi_act_layer, 0, 0, gdi_act_layer->width - 1, gdi_act_layer->height - 1);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_reset_clip)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_get_clip
 * DESCRIPTION
 *  Get current clip region of a layer.
 * PARAMETERS
 *  x1                  [IN]        
 *  y1                  [IN]        
 *  x2                  [IN]        
 *  y2                  [IN]        
 *  gdi_handle(?)       [IN]        Handle
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_get_clip(S32 *x1, S32 *y1, S32 *x2, S32 *y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_get_clip)
    *x1 = gdi_act_layer->clipx1;
    *y1 = gdi_act_layer->clipy1;
    *x2 = gdi_act_layer->clipx2;
    *y2 = gdi_act_layer->clipy2;
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_get_clip)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_push_clip
 * DESCRIPTION
 *  Push current clip region into stack.
 *  
 *  JF's opnion is the merge the clip region
 *  to get final clip region - need more study
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_push_clip(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gdi_rect_struct *rect;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_LOCK;
    GDI_ASSERT(gdi_act_layer->clips_top < GDI_LAYER_CLIP_STACK_COUNT);

    rect = &gdi_act_layer->clips[gdi_act_layer->clips_top];

    rect->x1 = gdi_act_layer->clipx1;
    rect->y1 = gdi_act_layer->clipy1;
    rect->x2 = gdi_act_layer->clipx2;
    rect->y2 = gdi_act_layer->clipy2;

    gdi_act_layer->clips_top++;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_pop_clip
 * DESCRIPTION
 *  Pop current clip region into stack.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_pop_clip(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gdi_rect_struct *rect;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ASSERT(gdi_act_layer->clips_top > 0);
    rect = &gdi_act_layer->clips[--(gdi_act_layer->clips_top)];

    gdi_act_layer->clipx1 = rect->x1;
    gdi_act_layer->clipy1 = rect->y1;
    gdi_act_layer->clipx2 = rect->x2;
    gdi_act_layer->clipy2 = rect->y2;

    GDI_UNLOCK;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_set_clip_preset
 * DESCRIPTION
 *  Set clip region.
 *  
 *  Will set clip region, use layer's own coordinate.
 * PARAMETERS
 *  x1                  [IN]        
 *  y1                  [IN]        
 *  x2                  [IN]        
 *  y2                  [IN]        
 *  gdi_handle(?)       [IN]        Handle
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_set_clip_preset(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_set_clip_preset)

    if (GDI_LAYER.clipx1 < x1)
    {
        x1 = GDI_LAYER.clipx1;
    }
    if (GDI_LAYER.clipy1 < y1)
    {
        y1 = GDI_LAYER.clipy1;
    }
    if (GDI_LAYER.clipx2 > x2)
    {
        x2 = GDI_LAYER.clipx2;
    }
    if (GDI_LAYER.clipy2 > y2)
    {
        y2 = GDI_LAYER.clipy2;
    }

    if (x2 < x1)
    {
        x2 = x1;
    }
    if (y2 < y1)
    {
        y2 = y1;
    }

    /* the previous clip area is already in lcd area */

    GDI_LAYER_SET_CLIP(gdi_act_layer, x1, y1, x2, y2);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_set_clip_preset)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_set_opacity
 * DESCRIPTION
 *  Set opacity value.
 *  
 *  Set opacity.
 * PARAMETERS
 *  opacity_enable      [IN]        
 *  opacity_value       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
GDI_RESULT gdi_layer_set_opacity(BOOL opacity_enable, U8 opacity_value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_set_opacity)
        if (GDI_COLOR_FORMAT_32 == gdi_act_layer->vcf)
        {
            gdi_act_layer_info->opacity_enable = TRUE;
            GDI_RETURN(GDI_SUCCEED);
        }
    gdi_act_layer_info->opacity_enable = opacity_enable;
#ifdef LCD_MAX_OPACITY
    gdi_act_layer_info->opacity_value = opacity_value * LCD_MAX_OPACITY / 255;  /* shift 3, hw only support 5 bit */
#else 
    gdi_act_layer_info->opacity_value = opacity_value;
#endif 

    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_set_opacity)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_set_source_key
 * DESCRIPTION
 *  Set opacity value.
 *  
 *  Set opacity.
 * PARAMETERS
 *  source_key_enable       [IN]        
 *  source_key_value        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
GDI_RESULT gdi_layer_set_source_key(BOOL source_key_enable, gdi_color source_key_value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_set_source_key)
    gdi_act_layer_info->source_key_enable = source_key_enable;
    gdi_act_layer_info->source_key = source_key_value;
    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_set_source_key)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_get_source_key
 * DESCRIPTION
 *  
 * PARAMETERS
 *  source_key_enable       [?]     
 *  source_key_value        [?]     
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_layer_get_source_key(BOOL *source_key_enable, gdi_color *source_key_value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_set_source_key)
    *source_key_enable = gdi_act_layer_info->source_key_enable;
    *source_key_value = gdi_act_layer_info->source_key;
    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_set_source_key)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_lock_frame_buffer
 * DESCRIPTION
 *  Blt layers.
 *  
 *  Lock blt. Avoid other to blt this layer.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_lock_frame_buffer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_LOCK;
    //              if(GDI_LCD->blt_lock == 0)
    //                      GDI_LCD->blt_is_first_time_used = TRUE;

    GDI_LCD->blt_lock++;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_unlock_frame_buffer
 * DESCRIPTION
 *  Blt layers.
 *  
 *  Unlock layer.
 * PARAMETERS
 *  void
 *  gdi_handle(?)       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_unlock_frame_buffer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* unlock buffer error */
    GDI_ASSERT(GDI_LCD->blt_lock > 0);
    GDI_LCD->blt_lock--;
    GDI_UNLOCK;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_get_rotate
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
U8 gdi_layer_get_rotate(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_get_rotate)
    GDI_RETURN(gdi_act_layer->rotate_value);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_get_rotate)
}

U8 gdi_layer_get_actual_rotate(void)
{
//	U8 actual_rotate_value;
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_get_actual_rotate)
	GDI_RETURN( gdi_layer_info[gdi_act_layer->id].rotate_value );
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_get_actual_rotate)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_rotate_map_region_internal
 * DESCRIPTION
 *  
 * PARAMETERS
 *  rotate_value        [IN]        
 *  x1                  [?]         
 *  y1                  [?]         
 *  x2                  [?]         
 *  y2                  [?]         
 *  dest_width          [IN]        
 *  dest_height         [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_rotate_map_region_internal(
        U8 rotate_value,
        S32 *x1,
        S32 *y1,
        S32 *x2,
        S32 *y2,
        S32 dest_width,
        S32 dest_height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_rotate_map_region_internal)
    S32 tx1, ty1, tx2, ty2;

    switch (rotate_value)
    {
        case GDI_LCD_LAYER_ROTATE_0:
        default:
            break;
        case GDI_LCD_LAYER_ROTATE_90:
            tx1 = dest_width - *y2 - 1;
            ty1 = *x1;
            tx2 = dest_width - *y1 - 1;
            ty2 = *x2;
            *x1 = tx1;
            *y1 = ty1;
            *x2 = tx2;
            *y2 = ty2;
            break;
        case GDI_LCD_LAYER_ROTATE_180:
            tx1 = dest_width - *x2 - 1;
            ty1 = dest_height - *y2 - 1;
            tx2 = dest_width - *x1 - 1;
            ty2 = dest_height - *y1 - 1;
            *x1 = tx1;
            *y1 = ty1;
            *x2 = tx2;
            *y2 = ty2;
            break;
        case GDI_LCD_LAYER_ROTATE_270:
            tx1 = *y1;
            ty1 = dest_height - *x2 - 1;
            tx2 = *y2;
            ty2 = dest_height - *x1 - 1;
            *x1 = tx1;
            *y1 = ty1;
            *x2 = tx2;
            *y2 = ty2;
            break;
        case GDI_LCD_LAYER_ROTATE_0_MIRROR:
            tx1 = dest_width - *x2 - 1;
            tx2 = dest_width - *x1 - 1;
            *x1 = tx1;
            *x2 = tx2;
            break;
        case GDI_LCD_LAYER_ROTATE_90_MIRROR:
            tx1 = dest_width - *y2 - 1;
            ty1 = dest_height - *x2 - 1;
            tx2 = dest_width - *y1 - 1;
            ty2 = dest_height - *x1 - 1;
            *x1 = tx1;
            *y1 = ty1;
            *x2 = tx2;
            *y2 = ty2;
            break;
        case GDI_LCD_LAYER_ROTATE_180_MIRROR:
            ty1 = dest_height - *y2 - 1;
            ty2 = dest_height - *y1 - 1;
            *y1 = ty1;
            *y2 = ty2;
            break;
        case GDI_LCD_LAYER_ROTATE_270_MIRROR:
            tx1 = *y1;
            ty1 = *x1;
            tx2 = *y2;
            ty2 = *x2;
            *x1 = tx1;
            *y1 = ty1;
            *x2 = tx2;
            *y2 = ty2;
            break;
    }
    GDI_EXIT_CRITICAL_SECTION(gdi_rotate_map_region_internal)
}
/*****************************************************************************
 * FUNCTION
 *  gdi_rotate_map_layer_to_lcd_region
 * DESCRIPTION
 *  
 * PARAMETERS
 *  rotate_value        [IN]        
 *  x1                  [?]         
 *  y1                  [?]         
 *  x2                  [?]         
 *  y2                  [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_rotate_map_layer_to_lcd_region(U8 rotate_value, S32 *x1, S32 *y1, S32 *x2, S32 *y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_rotate_map_layer_to_lcd_region)
    S32 lcd_width, lcd_height;

#if defined(GDI_USING_LCD_ROTATE)
    if (!GDI_LCD->rotate_by_layer)
    {
        gdi_lcd_get_dimension(&lcd_width, &lcd_height);
    }
    else
#endif /* defined(GDI_USING_LCD_ROTATE) */ 
    {
        lcd_width = GDI_LCD->act_width;
        lcd_height = GDI_LCD->act_height;
    }

    gdi_rotate_map_region_internal(rotate_value, x1, y1, x2, y2, lcd_width, lcd_height);
    GDI_EXIT_CRITICAL_SECTION(gdi_rotate_map_layer_to_lcd_region)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_rotate_map_layer_to_act_layer_region
 * DESCRIPTION
 *  
 * PARAMETERS
 *  rotate_value        [IN]        
 *  x1                  [?]         
 *  y1                  [?]         
 *  x2                  [?]         
 *  y2                  [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_rotate_map_layer_to_act_layer_region(U8 rotate_value,S32 *x1,S32 *y1,S32 *x2,S32 *y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	GDI_ENTER_CRITICAL_SECTION(gdi_rotate_map_layer_to_act_layer_region)
	gdi_rotate_map_region_internal(rotate_value, x1, y1, x2, y2, gdi_act_layer->width, gdi_act_layer->height);
	GDI_EXIT_CRITICAL_SECTION(gdi_rotate_map_layer_to_act_layer_region)
}	


/*****************************************************************************
 * FUNCTION
 *  gdi_rotate_map_layer_to_lcd
 * DESCRIPTION
 *  
 * PARAMETERS
 *  rotate_value        [IN]        
 *  x                   [?]         
 *  y                   [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_rotate_map_layer_to_lcd(U8 rotate_value, S32 *x, S32 *y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_rotate_map_layer_to_lcd)
    S32 tx, ty;
    S32 lcd_width, lcd_height;

    gdi_lcd_get_dimension(&lcd_width, &lcd_height);

    switch (rotate_value)
    {
        case GDI_LCD_LAYER_ROTATE_0:
        default:
            break;
        case GDI_LCD_LAYER_ROTATE_90:
            tx = lcd_width - *y - 1;
            ty = *x;
            *x = tx;
            *y = ty;
            break;
        case GDI_LCD_LAYER_ROTATE_180:
            tx = lcd_width - *x - 1;
            ty = lcd_height - *y - 1;
            *x = tx;
            *y = ty;
            break;
        case GDI_LCD_LAYER_ROTATE_270:
            tx = *y;
            ty = lcd_height - *x - 1;
            *x = tx;
            *y = ty;
            break;
        case GDI_LCD_LAYER_ROTATE_0_MIRROR:
            tx = lcd_width - *x - 1;
            *x = tx;
            break;
        case GDI_LCD_LAYER_ROTATE_90_MIRROR:
            tx = lcd_width - *y - 1;
            ty = lcd_height - *x - 1;
            *x = tx;
            *y = ty;
            break;
        case GDI_LCD_LAYER_ROTATE_180_MIRROR:
            ty = lcd_height - *y - 1;
            *y = ty;
            break;
        case GDI_LCD_LAYER_ROTATE_270_MIRROR:
            tx = *y;
            ty = *x;
            *x = tx;
            *y = ty;
            break;
    }
    GDI_EXIT_CRITICAL_SECTION(gdi_rotate_map_layer_to_lcd)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_rotate_map_lcd_to_layer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  rotate_value        [IN]        
 *  x                   [?]         
 *  y                   [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_rotate_map_lcd_to_layer(U8 rotate_value, S32 *x, S32 *y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_rotate_map_lcd_to_layer)
    S32 tx, ty;
    S32 lcd_width, lcd_height;

    gdi_lcd_get_dimension(&lcd_width, &lcd_height);

    switch (rotate_value)
    {
        case GDI_LCD_LAYER_ROTATE_0:
        default:
            break;
        case GDI_LCD_LAYER_ROTATE_90:
            tx = lcd_height - *y - 1;
            ty = *x;
            *x = tx;
            *y = ty;
            break;
        case GDI_LCD_LAYER_ROTATE_180:
            tx = lcd_width - *x - 1;
            ty = lcd_height - *y - 1;
            *x = tx;
            *y = ty;
            break;
        case GDI_LCD_LAYER_ROTATE_270:
            tx = *y;
            ty = lcd_width - *x - 1;
            *x = tx;
            *y = ty;
            break;
        case GDI_LCD_LAYER_ROTATE_0_MIRROR:
            tx = lcd_width - *x - 1;
            *x = tx;
            break;
        case GDI_LCD_LAYER_ROTATE_90_MIRROR:
            tx = *y;
            ty = *x;
            *x = tx;
            *y = ty;
            break;
        case GDI_LCD_LAYER_ROTATE_180_MIRROR:
            ty = lcd_height - *y - 1;
            *y = ty;
            break;
        case GDI_LCD_LAYER_ROTATE_270_MIRROR:
            tx = lcd_width - *y - 1;
            ty = lcd_height - *x - 1;
            *x = tx;
            *y = ty;
            break;
    }
    GDI_EXIT_CRITICAL_SECTION(gdi_rotate_map_lcd_to_layer)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_set_layer_info_rotate_value
 * DESCRIPTION
 *  
 * PARAMETERS
 *  layer_id            [IN]        
 *  rotate_value        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_set_layer_info_rotate_value(S32 layer_id,U8 rotate_value)
{
#if defined(GDI_USING_LAYER_ROTATE)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_set_layer_info_rotate_value)

    switch (rotate_value)
    {
        case GDI_LCD_LAYER_ROTATE_0:
        default:
            gdi_layer_info[layer_id].rotate_value = LCD_LAYER_ROTATE_NORMAL;
            break;
        case GDI_LCD_LAYER_ROTATE_90:
            gdi_layer_info[layer_id].rotate_value = LCD_LAYER_ROTATE_270;
            break;
        case GDI_LCD_LAYER_ROTATE_180:
            gdi_layer_info[layer_id].rotate_value = LCD_LAYER_ROTATE_180;
            break;
        case GDI_LCD_LAYER_ROTATE_270:
            gdi_layer_info[layer_id].rotate_value = LCD_LAYER_ROTATE_90;
            break;
        case GDI_LCD_LAYER_ROTATE_0_MIRROR:
            gdi_layer_info[layer_id].rotate_value = LCD_LAYER_MIRROR;
            break;
        case GDI_LCD_LAYER_ROTATE_90_MIRROR:
            gdi_layer_info[layer_id].rotate_value = LCD_LAYER_MIRROR_ROTATE_270;
            break;
        case GDI_LCD_LAYER_ROTATE_180_MIRROR:
            gdi_layer_info[layer_id].rotate_value = LCD_LAYER_MIRROR_ROTATE_180;
            break;
        case GDI_LCD_LAYER_ROTATE_270_MIRROR:
            gdi_layer_info[layer_id].rotate_value = LCD_LAYER_MIRROR_ROTATE_90;
            break;
    }
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_set_layer_info_rotate_value)
#endif /* defined(GDI_USING_LAYER_ROTATE) */ 
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_set_actual_rotate
 * DESCRIPTION
 *  
 * PARAMETERS
 *  layer_handle            [IN]        
 *  actual_rotate_value     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_set_actual_rotate(gdi_handle layer_handle, U8 actual_rotate_value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_set_actual_rotate)
#if defined(GDI_USING_LAYER_ROTATE)
    S32 x1, y1, x2, y2;
    S32 layer_id = ((gdi_layer_struct*) layer_handle)->id;

    gdi_layer_set_layer_info_rotate_value(layer_id, actual_rotate_value);
    /* Change device position */
    x1 = GDI_LAYERS[layer_id].offset_x;
    y1 = GDI_LAYERS[layer_id].offset_y;
    x2 = GDI_LAYERS[layer_id].offset_x + GDI_LAYERS[layer_id].width - 1;
    y2 = GDI_LAYERS[layer_id].offset_y + GDI_LAYERS[layer_id].height - 1;
    gdi_rotate_map_layer_to_lcd_region(actual_rotate_value, &x1, &y1, &x2, &y2);
    gdi_layer_info[layer_id].x_offset = x1 + GDI_LAYER_OFFSET;
    gdi_layer_info[layer_id].y_offset = y1 + GDI_LAYER_OFFSET;
#endif /* defined(GDI_USING_LAYER_ROTATE) */ 

    GDI_EXIT_CRITICAL_SECTION(gdi_layer_set_actual_rotate)
}

/*****************************************************************************
 * FUNCTION
 *  gdi_layer_set_flatten_rotate
 * DESCRIPTION
 *  
 * PARAMETERS
 *  layer_handle            [IN]        
 *  actual_rotate_value     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_set_flatten_rotate(gdi_handle layer_handle, U8 actual_rotate_value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	GDI_ENTER_CRITICAL_SECTION(gdi_layer_set_flatten_rotate)
	#if defined(GDI_USING_LAYER_ROTATE)
		S32 x1, y1, x2, y2;
		S32 layer_id = ((gdi_layer_struct*)layer_handle)->id;

		gdi_layer_set_layer_info_rotate_value(layer_id, actual_rotate_value);
		/* Change device position */
		x1 = GDI_LAYERS[layer_id].offset_x;
		y1 = GDI_LAYERS[layer_id].offset_y;
		x2 = GDI_LAYERS[layer_id].offset_x + GDI_LAYERS[layer_id].width - 1;
		y2 = GDI_LAYERS[layer_id].offset_y + GDI_LAYERS[layer_id].height - 1;
		gdi_rotate_map_layer_to_act_layer_region(actual_rotate_value, &x1, &y1, &x2, &y2);
		gdi_layer_info[layer_id].x_offset = x1 + GDI_LAYER_OFFSET;
		gdi_layer_info[layer_id].y_offset = y1 + GDI_LAYER_OFFSET;
#endif /* defined(GDI_USING_LAYER_ROTATE) */ 

	GDI_EXIT_CRITICAL_SECTION(gdi_layer_set_flatten_rotate)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_blt_ext
 * DESCRIPTION
 *  Blt layers.
 *  
 *  The last four parameters are the region which to blt to LCD.
 * PARAMETERS
 *  handle0     [IN]        
 *  handle1     [IN]        
 *  handle2     [IN]        
 *  handle3     [IN]        
 *  handle4     [IN]        
 *  handle5     [IN]        
 *  x1          [IN]        
 *  y1          [IN]        
 *  x2          [IN]        
 *  y2          [IN]        
 * RETURNS
 * BOOL
 *  
 *****************************************************************************/

GDI_RESULT gdi_Merge_layer(gdi_handle handle0, gdi_handle handle1, gdi_handle handle2, gdi_handle handle3,
#ifdef GDI_6_LAYERS
                             gdi_handle handle4, gdi_handle handle5,
#endif 
                             S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_blt)
    U32 layer_flag;
    gdi_color background;
    lcd_frame_update_struct lcd_update_frame;

    //#ifdef GDI_USING_LAYER_ROTATE
    //   gdi_layer_inverse_map_rotated_region(gdi_layer_get_base_layer_rotation(), &x1, &y1, &x2, &y2);
    //#endif /* GDI_USING_LAYER_ROTATE */
#if defined(GDI_USING_LAYER_ROTATE) || defined(GDI_USING_LCD_ROTATE)
    gdi_rotate_map_lcd_to_absolute_hw_region(GDI_LCD->rotate_value, &x1, &y1, &x2, &y2);
#endif 

    if (y2 < y1 || x2 < x1 || x1 >= GDI_LCD->act_width || x2 < 0 || y1 >= GDI_LCD->act_height || y2 < 0)
    {
        GDI_RETURN(GDI_LAYER_ERR_INVALID_BLT_REGION);
    }

/*wuzc Add Start For GDI/GUI Portting crash    on 2008-3-13 17:18 */
	if(handle0 == -1) handle0 = 0;
	if(handle1 == -1) handle1 = 0;
	if(handle2 == -1) handle2 = 0;
	if(handle3 == -1) handle3 = 0;
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-13 17:18 */
	
    /* store current blt layers, for gdi_layer_blt_previous uses */
    GDI_LCD->blt_handle[0] = handle0;
    GDI_LCD->blt_handle[1] = handle1;
    GDI_LCD->blt_handle[2] = handle2;
    GDI_LCD->blt_handle[3] = handle3;
#ifdef GDI_6_LAYERS
    GDI_LCD->blt_handle[4] = handle4;
    GDI_LCD->blt_handle[5] = handle5;
#endif /* GDI_6_LAYERS */ 

    /* if the layer is locked and someone call blt. merge the maxium blt region */
    if (GDI_LCD->blt_lock > 0)
    {
        if (!GDI_LCD->blt_rect_is_used)
        {
            GDI_LCD->blt_rect.x1 = x1;
            GDI_LCD->blt_rect.y1 = y1;
            GDI_LCD->blt_rect.x2 = x2;
            GDI_LCD->blt_rect.y2 = y2;
            GDI_LCD->blt_rect_is_used = TRUE;
        }
        else
        {
            if (x1 < GDI_LCD->blt_rect.x1)
            {
                GDI_LCD->blt_rect.x1 = x1;
            }
            if (y1 < GDI_LCD->blt_rect.y1)
            {
                GDI_LCD->blt_rect.y1 = y1;
            }
            if (x2 > GDI_LCD->blt_rect.x2)
            {
                GDI_LCD->blt_rect.x2 = x2;
            }
            if (y2 > GDI_LCD->blt_rect.y2)
            {
                GDI_LCD->blt_rect.y2 = y2;
            }
        }
        GDI_RETURN(GDI_LAYER_DOUBLE_BUFFER_LOCKED);
    }

    if (GDI_LCD->blt_rect_is_used)
    {
        /* merge and calculate maxium blt region */
        if (x1 > GDI_LCD->blt_rect.x1)
        {
            x1 = GDI_LCD->blt_rect.x1;
        }
        if (y1 > GDI_LCD->blt_rect.y1)
        {
            y1 = GDI_LCD->blt_rect.y1;
        }
        if (x2 < GDI_LCD->blt_rect.x2)
        {
            x2 = GDI_LCD->blt_rect.x2;
        }
        if (y2 < GDI_LCD->blt_rect.y2)
        {
            y2 = GDI_LCD->blt_rect.y2;
        }

        GDI_LCD->blt_rect_is_used = FALSE;
    }
    /* adjust region to fit valid region */
    if (x1 < 0)
    {
        x1 = 0;
    }
    if (y1 < 0)
    {
        y1 = 0;
    }
    if (x2 > GDI_LCD->act_width - 1)
    {
        x2 = GDI_LCD->act_width - 1;
    }
    if (y2 > GDI_LCD->act_height - 1)
    {
        y2 = GDI_LCD->act_height - 1;
    }

    /* config hardware register for each layer */
    layer_flag = 0;
    background = 0;

    gdi_lcd_set_blt_handle_rotate();

#ifdef GDI_6_LAYERS
    CONFIG_HARDWARE_LAYER(5);
    CONFIG_HARDWARE_LAYER(4);
#endif /* GDI_6_LAYERS */ 
if (handle3 != 0)
    CONFIG_HARDWARE_LAYER(3);
if (handle2 != 0)
    CONFIG_HARDWARE_LAYER(2);
if (handle1 != 0)
    CONFIG_HARDWARE_LAYER(1);
if (handle0 != 0)
    CONFIG_HARDWARE_LAYER(0);

    /* BLOCK BLT */
    if (!gdi_is_non_block_blt
        || (handle0 && GDI_LAYER_GET_FLAG(handle0, GDI_LAYER_FLAG_DOUBLE_LAYER | GDI_LAYER_FLAG_FROZEN) == 0)
        || (handle1 && GDI_LAYER_GET_FLAG(handle1, GDI_LAYER_FLAG_DOUBLE_LAYER | GDI_LAYER_FLAG_FROZEN) == 0)
        || (handle2 && GDI_LAYER_GET_FLAG(handle2, GDI_LAYER_FLAG_DOUBLE_LAYER | GDI_LAYER_FLAG_FROZEN) == 0)
        || (handle3 && GDI_LAYER_GET_FLAG(handle3, GDI_LAYER_FLAG_DOUBLE_LAYER | GDI_LAYER_FLAG_FROZEN) == 0)
#ifdef GDI_6_LAYERS
        || (handle4 && GDI_LAYER_GET_FLAG(handle4, GDI_LAYER_FLAG_DOUBLE_LAYER | GDI_LAYER_FLAG_FROZEN) == 0)
        || (handle5 && GDI_LAYER_GET_FLAG(handle5, GDI_LAYER_FLAG_DOUBLE_LAYER | GDI_LAYER_FLAG_FROZEN) == 0)
#endif /* GDI_6_LAYERS */ 
        )
    {
        lcd_update_frame.block_mode_flag = TRUE;
        lcd_update_frame.lcd_block_mode_cb = NULL;
    }
    else
    {
        lcd_update_frame.block_mode_flag = FALSE;
        lcd_update_frame.lcd_block_mode_cb = gdi_layer_blt_finish_handler;
        gdi_is_blting = TRUE;
    }

#ifdef GDI_USING_LAYER_OUTPUT_ANOTHER_MEMORY
    lcd_update_frame.memory_output = KAL_FALSE;
#endif 

    lcd_update_frame.module_id = LCD_UPDATE_MODULE_MMI;
    lcd_update_frame.fb_update_mode = LCD_SW_TRIGGER_MODE;
    lcd_update_frame.lcm_start_x = (kal_uint16) x1; /* LCM's start x */
    lcd_update_frame.lcm_start_y = (kal_uint16) y1; /* LCM's start y */
    lcd_update_frame.lcm_end_x = (kal_uint16) x2;   /* LCM's end x */
    lcd_update_frame.lcm_end_y = (kal_uint16) y2;   /* LCM's width */
#if defined(GDI_USING_LCD_ROTATE)
    //GDI_USING_LCD_ROTATE
     
    if (!GDI_LCD->rotate_by_layer && GDI_LCD->rotate_value)
    {
        gdi_rotate_map_absolute_hw_to_lcd_region(GDI_LCD->rotate_value, &x1, &y1, &x2, &y2);
    }
#endif /* defined(GDI_USING_LCD_ROTATE) */ 
    lcd_update_frame.roi_offset_x = (kal_uint16) (x1 + GDI_LAYER_OFFSET);       /* ROI offset x */
    lcd_update_frame.roi_offset_y = (kal_uint16) (y1 + GDI_LAYER_OFFSET);       /* ROI offset y */
    lcd_update_frame.update_layer = (kal_uint32) layer_flag;
    lcd_update_frame.hw_update_layer = 0;   /* not used in software mode */

#ifdef GDI_USING_LAYER_BACKGROUND
    lcd_update_frame.roi_background_color = background;
#endif 


#ifdef __MMI_SUBLCD__   /* actully have sub lcd - blt to it */
    if (GDI_LCD->act_handle == GDI_LCD_SUB_LCD_HANDLE)
    {
        lcd_update_frame.lcd_id = SUB_LCD;
    }
    else
#endif /* __MMI_SUBLCD__ */ 
        lcd_update_frame.lcd_id = MAIN_LCD;

#ifdef MMI_ON_WIN32
    /* pass necessary data to PC Simulator */
    WIN32_UI_SET_LAYER_DATA(0);
    WIN32_UI_SET_LAYER_DATA(1);
    WIN32_UI_SET_LAYER_DATA(2);
    WIN32_UI_SET_LAYER_DATA(3);
#endif /* MMI_ON_WIN32 */ 

    if (!GDI_LCD->is_freeze)
    {
        gdi_layer_Merge_frame_buffer(&lcd_update_frame);
    }

    /* reset blt lock value */
    GDI_LCD->blt_lock = 0;
    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_blt)
}

/*****************************************************************************
 * FUNCTION
 *  gdi_layer_blt_previous
 * DESCRIPTION
 *  Blt previous blt layers.
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_Merge_layer_previous(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_blt_previous)
    GDI_RESULT result =0;

    if (GDI_LCD->blt_handle[0] || GDI_LCD->blt_handle[1] || GDI_LCD->blt_handle[2] || GDI_LCD->blt_handle[3]
#ifdef GDI_6_LAYERS
        || GDI_LCD->blt_handle[4] || GDI_LCD->blt_handle[5]
#endif 
        )
    {
        result = gdi_Merge_layer(GDI_LCD->blt_handle[0],
                                   GDI_LCD->blt_handle[1], GDI_LCD->blt_handle[2], GDI_LCD->blt_handle[3],
    #ifdef GDI_6_LAYERS
                                   GDI_LCD->blt_handle[4], GDI_LCD->blt_handle[5],
    #endif 
                                   x1, y1, x2, y2);
    }
    GDI_RETURN(result);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_blt_previous)
}

/*****************************************************************************
 * FUNCTION
 *  gdi_layer_blt_ext
 * DESCRIPTION
 *  Blt layers.
 *  
 *  The last four parameters are the region which to blt to LCD.
 * PARAMETERS
 *  handle0     [IN]        
 *  handle1     [IN]        
 *  handle2     [IN]        
 *  handle3     [IN]        
 *  handle4     [IN]        
 *  handle5     [IN]        
 *  x1          [IN]        
 *  y1          [IN]        
 *  x2          [IN]        
 *  y2          [IN]        
 * RETURNS
 * BOOL
 *  
 *****************************************************************************/
 extern U8 gdi_lcd_get_rotate(void);
GDI_RESULT gdi_layer_blt_ext(gdi_handle handle0, gdi_handle handle1, gdi_handle handle2, gdi_handle handle3,
#ifdef GDI_6_LAYERS
                             gdi_handle handle4, gdi_handle handle5,
#endif 
                             S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_blt)
    U32 layer_flag;
    gdi_color background;
    lcd_frame_update_struct lcd_update_frame;

#ifdef __MMI_TVOUT__
    MDI_RESULT ret;
    U16 tv_owner;
    U16 tvout_width;
    U16 tvout_height;
    S32 tvout_x1;
    S32 tvout_y1;
    S32 tvout_x2;
    S32 tvout_y2;
#endif /* __MMI_TVOUT__ */ 

    //#ifdef GDI_USING_LAYER_ROTATE
    //   gdi_layer_inverse_map_rotated_region(gdi_layer_get_base_layer_rotation(), &x1, &y1, &x2, &y2);
    //#endif /* GDI_USING_LAYER_ROTATE */
#if defined(GDI_USING_LAYER_ROTATE) || defined(GDI_USING_LCD_ROTATE)
  //  lcd_update_frame.rotate_value = GDI_LCD->rotate_value;
  //  gdi_rotate_map_lcd_to_absolute_hw_region(GDI_LCD->rotate_value, &x1, &y1, &x2, &y2);
#endif 

    if (y2 < y1 || x2 < x1 || x1 >= GDI_LCD->act_width || x2 < 0 || y1 >= GDI_LCD->act_height || y2 < 0)
    {
        GDI_RETURN(GDI_LAYER_ERR_INVALID_BLT_REGION);
    }

/*wuzc Add Start For GDI/GUI Portting crash    on 2008-3-13 17:18 */
	if(handle0 == -1) handle0 = 0;
	if(handle1 == -1) handle1 = 0;
	if(handle2 == -1) handle2 = 0;
	if(handle3 == -1) handle3 = 0;
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-13 17:18 */
	
    /* store current blt layers, for gdi_layer_blt_previous uses */
    GDI_LCD->blt_handle[0] = handle0;
    GDI_LCD->blt_handle[1] = handle1;
    GDI_LCD->blt_handle[2] = handle2;
    GDI_LCD->blt_handle[3] = handle3;
#ifdef GDI_6_LAYERS
    GDI_LCD->blt_handle[4] = handle4;
    GDI_LCD->blt_handle[5] = handle5;
#endif /* GDI_6_LAYERS */ 

    /* if the layer is locked and someone call blt. merge the maxium blt region */
    if (GDI_LCD->blt_lock > 0)
    {
        if (!GDI_LCD->blt_rect_is_used)
        {
            GDI_LCD->blt_rect.x1 = x1;
            GDI_LCD->blt_rect.y1 = y1;
            GDI_LCD->blt_rect.x2 = x2;
            GDI_LCD->blt_rect.y2 = y2;
            GDI_LCD->blt_rect_is_used = TRUE;
        }
        else
        {
            if (x1 < GDI_LCD->blt_rect.x1)
            {
                GDI_LCD->blt_rect.x1 = x1;
            }
            if (y1 < GDI_LCD->blt_rect.y1)
            {
                GDI_LCD->blt_rect.y1 = y1;
            }
            if (x2 > GDI_LCD->blt_rect.x2)
            {
                GDI_LCD->blt_rect.x2 = x2;
            }
            if (y2 > GDI_LCD->blt_rect.y2)
            {
                GDI_LCD->blt_rect.y2 = y2;
            }
        }
        GDI_RETURN(GDI_LAYER_DOUBLE_BUFFER_LOCKED);
    }

    if (GDI_LCD->blt_rect_is_used)
    {
        /* merge and calculate maxium blt region */
        if (x1 > GDI_LCD->blt_rect.x1)
        {
            x1 = GDI_LCD->blt_rect.x1;
        }
        if (y1 > GDI_LCD->blt_rect.y1)
        {
            y1 = GDI_LCD->blt_rect.y1;
        }
        if (x2 < GDI_LCD->blt_rect.x2)
        {
            x2 = GDI_LCD->blt_rect.x2;
        }
        if (y2 < GDI_LCD->blt_rect.y2)
        {
            y2 = GDI_LCD->blt_rect.y2;
        }

        GDI_LCD->blt_rect_is_used = FALSE;
    }

    /* adjust region to fit valid region */
    if (x1 < 0)
    {
        x1 = 0;
    }
    if (y1 < 0)
    {
        y1 = 0;
    }

    switch(gdi_lcd_get_rotate())
    {
        case 0:
        case 2:
        default:
                if (x2 > GDI_LCD->act_width - 1)
                {
                    x2 = GDI_LCD->act_width - 1;
                }
                if (y2 > GDI_LCD->act_height - 1)
                {
                    y2 = GDI_LCD->act_height - 1;
                }
                break;
        case 1:
        case 3:    
                if (x2 > GDI_LCD->act_height - 1)
                {
                    x2 = GDI_LCD->act_height - 1;
                }
                if (y2 > GDI_LCD->act_width - 1)
                {
                    y2 = GDI_LCD->act_width - 1;
                }
            break;
    }

    /* config hardware register for each layer */
    layer_flag = 0;
    background = 0;

    gdi_lcd_set_blt_handle_rotate();

#ifdef GDI_6_LAYERS
    CONFIG_HARDWARE_LAYER(5);
    CONFIG_HARDWARE_LAYER(4);
#endif /* GDI_6_LAYERS */ 
if (handle3 != 0)
    CONFIG_HARDWARE_LAYER(3);
if (handle2 != 0)
    CONFIG_HARDWARE_LAYER(2);
if (handle1 != 0)
    CONFIG_HARDWARE_LAYER(1);
if (handle0 != 0)
    CONFIG_HARDWARE_LAYER(0);

    /* BLOCK BLT */
    if (!gdi_is_non_block_blt
        || (handle0 && GDI_LAYER_GET_FLAG(handle0, GDI_LAYER_FLAG_DOUBLE_LAYER | GDI_LAYER_FLAG_FROZEN) == 0)
        || (handle1 && GDI_LAYER_GET_FLAG(handle1, GDI_LAYER_FLAG_DOUBLE_LAYER | GDI_LAYER_FLAG_FROZEN) == 0)
        || (handle2 && GDI_LAYER_GET_FLAG(handle2, GDI_LAYER_FLAG_DOUBLE_LAYER | GDI_LAYER_FLAG_FROZEN) == 0)
        || (handle3 && GDI_LAYER_GET_FLAG(handle3, GDI_LAYER_FLAG_DOUBLE_LAYER | GDI_LAYER_FLAG_FROZEN) == 0)
#ifdef GDI_6_LAYERS
        || (handle4 && GDI_LAYER_GET_FLAG(handle4, GDI_LAYER_FLAG_DOUBLE_LAYER | GDI_LAYER_FLAG_FROZEN) == 0)
        || (handle5 && GDI_LAYER_GET_FLAG(handle5, GDI_LAYER_FLAG_DOUBLE_LAYER | GDI_LAYER_FLAG_FROZEN) == 0)
#endif /* GDI_6_LAYERS */ 
        )
    {
        lcd_update_frame.block_mode_flag = TRUE;
        lcd_update_frame.lcd_block_mode_cb = NULL;
    }
    else
    {
        lcd_update_frame.block_mode_flag = FALSE;
        lcd_update_frame.lcd_block_mode_cb = gdi_layer_blt_finish_handler;
        gdi_is_blting = TRUE;
    }

#ifdef __MMI_TVOUT__
    /* 
     * if tvout is enable, we will get buffer from mdi_tv and send to lcd_fb_update function,
     * this function will merge layers and blt to this buffer.
     * 
     * afterwards, we shall call mdi_tv_blt to blt this buffer to TV 
     */

    /* init */
    lcd_update_frame.tv_output = FALSE;
    lcd_update_frame.memory_output = FALSE;

    if (GDI_LCD->act_handle != GDI_LCD_SUB_LCD_HANDLE)
    {
        if (mdi_tvout_is_enable() &&
            ((GDI_LCD->lcd_type == GDI_LCD_TYPE_TVOUT) || (GDI_LCD->bind_handle == GDI_LCD_TVOUT_HANDLE)))
        {
            tv_owner = mdi_tvout_get_owner();

            if (tv_owner == MDI_TV_OWNER_GDI)
            {
                ret = mdi_tvout_get_para(
                        MDI_TV_OWNER_GDI,       /* owner */
                        NULL,                   /* mode_ptr */
                        (PU16) & tvout_width,   /* buf_width_ptr */
                        (PU16) & tvout_height,  /* buf_height_ptr */
                        (PU32) & lcd_update_frame.memory_output_buffer_size,   /* buf_size_ptr */
                        (PU8*) & lcd_update_frame.memory_output_buffer_address,       /* buf_1_pp */
                        NULL,                   /* buf_2_pp */
                        NULL,                   /* rotate_buf_1_pp */
                        NULL);                  /* rotate_buf_2_pp */

                tvout_x1 = x1;
                tvout_y1 = y1;
                tvout_x2 = x2;
                tvout_y2 = y2;

            #if defined(GDI_USING_LCD_ROTATE)
                if (!GDI_LCD->rotate_by_layer && GDI_LCD->rotate_value)
                {
                    gdi_rotate_map_absolute_hw_to_lcd_region(
                        GDI_LCD->rotate_value,
                        &tvout_x1,
                        &tvout_y1,
                        &tvout_x2,
                        &tvout_y2);
                }
            #endif /* defined(GDI_USING_LCD_ROTATE) */ 

                if (ret == MDI_RES_TV_ENABLE)
                {
                    /* if tvout is active, need to update full screen */
                    lcd_update_frame.block_mode = LCD_WM_BLOCK_MODE;
                    lcd_update_frame.memory_block_width = tvout_width;
                    lcd_update_frame.memory_data_format = LCD_WMEM_RGB565;

                    lcd_update_frame.memory_output = TRUE;
                    lcd_update_frame.tv_output = FALSE;

                    lcd_update_frame.memory_output_buffer_address =
                        lcd_update_frame.memory_output_buffer_address +
                        ((tvout_x1 + (tvout_y1 * tvout_width)) * MDI_TV_BUFFER_DEPTH);

                }
            }
            else if ((tv_owner == MDI_TV_OWNER_CAMERA) || (tv_owner == MDI_TV_OWNER_VIDEO))
            {

                ret = mdi_tvout_get_para(
                        MDI_TV_OWNER_CAMERA,    /* owner */
                        NULL,                   /* mode_ptr */
                        (PU16) & tvout_width,   /* buf_width_ptr */
                        (PU16) & tvout_height,  /* buf_height_ptr */
                        NULL,                   /* buf_size_ptr */
                        NULL,                   /* buf_1_pp */
                        NULL,                   /* buf_2_pp */
                        NULL,                   /* rotate_buf_1_pp */
                        NULL);                  /* rotate_buf_2_pp */

                /* full screen update */
                x1 = 0;
                y1 = 0;
                x2 = GDI_LCD->act_width - 1;
                y2 = GDI_LCD->act_height - 1;

                lcd_update_frame.tv_output_width = tvout_width;
                lcd_update_frame.tv_output_height = tvout_height;

                lcd_update_frame.tv_output_offset_x = 0;
                lcd_update_frame.tv_output_offset_y = 0;
            }
        }
    }

#else /* __MMI_TVOUT__ */ 
#ifdef GDI_USING_LAYER_OUTPUT_ANOTHER_MEMORY
    lcd_update_frame.memory_output = KAL_FALSE;
#endif 
#endif /* __MMI_TVOUT__ */ 

#if defined(GDI_USING_LCD_ROTATE)
    //GDI_USING_LCD_ROTATE
     
    if (!GDI_LCD->rotate_by_layer && GDI_LCD->rotate_value)
    {
     //   gdi_rotate_map_absolute_hw_to_lcd_region(GDI_LCD->rotate_value, &x1, &y1, &x2, &y2);
    }
#endif /* defined(GDI_USING_LCD_ROTATE) */ 
    lcd_update_frame.module_id = LCD_UPDATE_MODULE_MMI;
    lcd_update_frame.fb_update_mode = LCD_SW_TRIGGER_MODE;
    lcd_update_frame.lcm_start_x = (kal_uint16) x1; /* LCM's start x */
    lcd_update_frame.lcm_start_y = (kal_uint16) y1; /* LCM's start y */
    lcd_update_frame.lcm_end_x = (kal_uint16) x2;   /* LCM's end x */
    lcd_update_frame.lcm_end_y = (kal_uint16) y2;   /* LCM's width */
    lcd_update_frame.roi_offset_x = (kal_uint16) (x1 + GDI_LAYER_OFFSET);       /* ROI offset x */
    lcd_update_frame.roi_offset_y = (kal_uint16) (y1 + GDI_LAYER_OFFSET);       /* ROI offset y */
    lcd_update_frame.update_layer = (kal_uint32) layer_flag;
    lcd_update_frame.hw_update_layer = 0;   /* not used in software mode */

#ifdef GDI_USING_LAYER_BACKGROUND
    lcd_update_frame.roi_background_color = background;
#endif 

#ifdef GDI_USING_TV_OUTPUT
    lcd_update_frame.tv_output = KAL_FALSE;
#endif 

#ifdef __MMI_SUBLCD__   /* actully have sub lcd - blt to it */
    if (GDI_LCD->act_handle == GDI_LCD_SUB_LCD_HANDLE)
    {
        lcd_update_frame.lcd_id = SUB_LCD;
    }
    else
#endif /* __MMI_SUBLCD__ */ 
        lcd_update_frame.lcd_id = MAIN_LCD;

#ifdef MMI_ON_WIN32
    /* pass necessary data to PC Simulator */
    WIN32_UI_SET_LAYER_DATA(0);
    WIN32_UI_SET_LAYER_DATA(1);
    WIN32_UI_SET_LAYER_DATA(2);
    WIN32_UI_SET_LAYER_DATA(3);
#endif /* MMI_ON_WIN32 */ 

#ifdef __MMI_TVOUT__
    if (GDI_LCD->lcd_type == GDI_LCD_TYPE_TVOUT)
    {
        config_lcd_output(KAL_FALSE);
    }
    else
    {
        config_lcd_output(KAL_TRUE);
    }
#endif /* __MMI_TVOUT__ */ 
    if (!GDI_LCD->is_freeze)
    {
        lcd_fb_update(&lcd_update_frame);
    }

#ifdef __MMI_TVOUT__
    if (mdi_tvout_is_enable() &&
        ((GDI_LCD->lcd_type == GDI_LCD_TYPE_TVOUT) || (GDI_LCD->bind_handle == GDI_LCD_TVOUT_HANDLE)))
    {
        if ((GDI_LCD->act_handle != GDI_LCD_SUB_LCD_HANDLE) && lcd_update_frame.memory_output)
        {
            mdi_tvout_blt();
        }
    }
#endif /* __MMI_TVOUT__ */ 

    /* reset blt lock value */
    GDI_LCD->blt_lock = 0;
    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_blt)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_set_blt_layer_ext
 * DESCRIPTION
 *  Set layer to GDI and config driver's setting
 * PARAMETERS
 *  handle0     [IN]        
 *  handle1     [IN]        
 *  handle2     [IN]        
 *  handle3     [IN]        
 *  handle4     [IN]        
 *  handle5     [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_layer_set_blt_layer_ext(gdi_handle handle0, gdi_handle handle1, gdi_handle handle2, gdi_handle handle3
#ifdef GDI_6_LAYERS
                                       , gdi_handle handle4, gdi_handle handle5
#endif 
    )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	GDI_ENTER_CRITICAL_SECTION(gdi_layer_set_blt_layer)
	U32 layer_flag = 0;         /* variable not used, just for CONFIG_HARDWARE_LAYER MACRO */
	gdi_color background = 0;   /* variable not used, just for CONFIG_HARDWARE_LAYER MACRO */

	/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-4-22 17:56 */
	if(handle0 == -1) handle0 = 0;
	if(handle1 == -1) handle1 = 0;
	if(handle2 == -1) handle2 = 0;
	if(handle3 == -1) handle3 = 0;
	/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-4-22 17:56 */

	/* store current blt layers, for gdi_layer_blt_previous uses */
	GDI_LCD->blt_handle[0] = handle0;
	GDI_LCD->blt_handle[1] = handle1;
	GDI_LCD->blt_handle[2] = handle2;
	GDI_LCD->blt_handle[3] = handle3;
	#ifdef GDI_6_LAYERS
	GDI_LCD->blt_handle[4] = handle4;
	GDI_LCD->blt_handle[5] = handle5;
	#endif /* GDI_6_LAYERS */ 

    gdi_lcd_set_blt_handle_rotate();

	//config to driver
	/*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-4-22 17:57 */
	if(handle3 != 0)
		CONFIG_HARDWARE_LAYER(3);
	if(handle2 != 0)
		CONFIG_HARDWARE_LAYER(2);
	if(handle1 != 0)
		CONFIG_HARDWARE_LAYER(1);
	if(handle0 != 0)
		CONFIG_HARDWARE_LAYER(0);
	#ifdef GDI_6_LAYERS
	CONFIG_HARDWARE_LAYER(4);
	CONFIG_HARDWARE_LAYER(5);
	#endif
	/*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-4-22 17:57 */

    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_set_blt_layer)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_get_blt_layer_ext
 * DESCRIPTION
 *  
 * PARAMETERS
 *  handle0     [?]     
 *  handle1     [?]     
 *  handle2     [?]     
 *  handle3     [?]     
 *  handle4     [?]     
 *  handle5     [?]     
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_layer_get_blt_layer_ext(gdi_handle *handle0, gdi_handle *handle1, gdi_handle *handle2,
                                       gdi_handle *handle3
#ifdef GDI_6_LAYERS
                                       , gdi_handle *handle4, gdi_handle *handle5
#endif 
    )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_get_blt_layer)
    *handle0 = GDI_LCD->blt_handle[0];
    *handle1 = GDI_LCD->blt_handle[1];
    *handle2 = GDI_LCD->blt_handle[2];
    *handle3 = GDI_LCD->blt_handle[3];
#ifdef GDI_6_LAYERS
    *handle4 = GDI_LCD->blt_handle[4];
    *handle5 = GDI_LCD->blt_handle[5];
#endif /* GDI_6_LAYERS */ 
    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_get_blt_layer)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_blt_base_layer
 * DESCRIPTION
 *  Blt base layer
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_layer_blt_base_layer(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_blt_base_layer)

#ifdef __MMI_SUBLCD__
    if (GDI_LCD->act_handle == GDI_LCD_SUB_LCD_HANDLE)
    {
        GDI_RETURN(gdi_layer_blt(GDI_LAYER_SUB_BASE_LAYER_HANDLE, 0, 0, 0, x1, y1, x2, y2));
    }
    else
#endif /* __MMI_SUBLCD__ */ 
        GDI_RETURN(gdi_layer_blt(GDI_LAYER_MAIN_BASE_LAYER_HANDLE, 0, 0, 0, x1, y1, x2, y2));
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_blt_base_layer)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_blt_previous
 * DESCRIPTION
 *  Blt previous blt layers.
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_layer_blt_previous(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_blt_previous)
    GDI_RESULT result;

    if (GDI_LCD->blt_handle[0] || GDI_LCD->blt_handle[1] || GDI_LCD->blt_handle[2] || GDI_LCD->blt_handle[3]
#ifdef GDI_6_LAYERS
        || GDI_LCD->blt_handle[4] || GDI_LCD->blt_handle[5]
#endif 
        )
    {
        result = gdi_layer_blt_ext(GDI_LCD->blt_handle[0],
                                   GDI_LCD->blt_handle[1], GDI_LCD->blt_handle[2], GDI_LCD->blt_handle[3],
    #ifdef GDI_6_LAYERS
                                   GDI_LCD->blt_handle[4], GDI_LCD->blt_handle[5],
    #endif 
                                   x1, y1, x2, y2);
    }
    else
    {
        result = gdi_layer_blt_ext(GDI_LCD->act_layer_handle, 0, 0, 0,
    #ifdef GDI_6_LAYERS
                                   0, 0,
    #endif 
                                   x1, y1, x2, y2);
    }
    GDI_RETURN(result);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_blt_previous)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_multi_layer_enable
 * DESCRIPTION
 *  Enable multi-layer functions
 * PARAMETERS
 *  void
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_layer_multi_layer_enable(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_multi_layer_enable)

#ifdef MMI_ON_WIN32
    win32_ui_set_new_application_flag();
#endif 

#if defined(GDI_USING_LAYER)
    gdi_is_layer_enable = TRUE;
#else 
//    GDI_ASSERT(0);  /* 6205B and 6218 should not call multi-layer function */
#endif 

    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_multi_layer_enable)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_multi_layer_disable
 * DESCRIPTION
 *  Disable multi-layer functions
 * PARAMETERS
 *  void
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_layer_multi_layer_disable(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_multi_layer_disable)
#if defined(GDI_USING_LAYER)
    gdi_handle lcd_handle;

    gdi_lcd_get_active(&lcd_handle);

    if (lcd_handle == GDI_LCD_MAIN_LCD_HANDLE)
    {
        gdi_layer_set_blt_layer(GDI_LAYER_MAIN_BASE_LAYER_HANDLE, 0, 0, 0);
    }
#ifdef __MMI_SUBLCD__
    else if (lcd_handle == GDI_LCD_SUB_LCD_HANDLE)
    {
        gdi_layer_set_blt_layer(GDI_LAYER_SUB_BASE_LAYER_HANDLE, 0, 0, 0);
    }
#endif /* __MMI_SUBLCD__ */ 
    else
    {
        GDI_ASSERT(0);
        GDI_RETURN(GDI_FAILED);
    }

    gdi_is_layer_enable = FALSE;
#else /* defined(GDI_USING_LAYER) */ 
//    GDI_ASSERT(0);  /* 6205B and 6218 should not call multi-layer function */
#endif /* defined(GDI_USING_LAYER) */ 
    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_multi_layer_disable)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_is_multi_layer_enable
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
BOOL gdi_layer_is_multi_layer_enable(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_is_multi_layer_enable)
    GDI_RETURN(gdi_is_layer_enable);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_is_multi_layer_enable)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_get_base_handle
 * DESCRIPTION
 *  Get handle of base layer.
 * PARAMETERS
 *  handle_ptr      [?]     
 * RETURNS
 *  gdi_handle
 *****************************************************************************/
GDI_RESULT gdi_layer_get_base_handle(gdi_handle *handle_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_get_base_handle)

#ifdef __MMI_SUBLCD__
    if (GDI_LCD->act_handle == GDI_LCD_SUB_LCD_HANDLE)
    {
        *handle_ptr = GDI_LAYER_SUB_BASE_LAYER_HANDLE;
    }
    else
#endif /* __MMI_SUBLCD__ */ 
        *handle_ptr = GDI_LAYER_MAIN_BASE_LAYER_HANDLE;

    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_get_base_handle)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_restore_base_active
 * DESCRIPTION
 *  Restore base layer as active layer
 * PARAMETERS
 *  void
 * RETURNS
 *  GDI_RESULT(?)
 *****************************************************************************/
void gdi_layer_restore_base_active(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_restore_base_active)
#ifdef __MMI_SUBLCD__
    if (GDI_LCD->act_handle == GDI_LCD_SUB_LCD_HANDLE)
    {
        gdi_layer_set_active(GDI_LAYER_SUB_BASE_LAYER_HANDLE);
    }
    /* gdi_act_layer = (gdi_layer_struct*) GDI_LAYER_SUB_BASE_LAYER_HANDLE; */
    else
#endif /* __MMI_SUBLCD__ */ 
        gdi_layer_set_active(GDI_LAYER_MAIN_BASE_LAYER_HANDLE);
    //gdi_act_layer = (gdi_layer_struct*) GDI_LAYER_MAIN_BASE_LAYER_HANDLE;
    //gdi_act_layer_info = &(gdi_layer_info[gdi_act_layer->id]);

    //gdi_act_color_from_rgb  = gd_color_from_rgb[gdi_act_layer->cf];
    //gdi_act_color_to_rgb    = gd_color_to_rgb[gdi_act_layer->cf];

    //gdi_act_put_pixel = gd_put_pixel[gdi_act_layer->cf];
    //gdi_act_get_pixel = gd_get_pixel[gdi_act_layer->cf];
    //gdi_act_replace_src_key = gd_replace_src_key[gdi_act_layer->cf];

    GDI_EXIT_CRITICAL_SECTION(gdi_layer_restore_base_active)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_clear
 * DESCRIPTION
 *  draw currently active layer's color
 * PARAMETERS
 *  bg_color        [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_layer_clear(gdi_color color)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_clear)

    if (GDI_LAYER.bits_per_pixel == 16)
    {
        gdi_memset16((U8*) gdi_act_layer->buf_ptr, (U16) color, gdi_act_layer->layer_size);
    }
    else
    {
        gdi_layer_push_clip();
        gdi_layer_reset_clip();
        gdi_draw_solid_rect(0, 0, gdi_act_layer->width - 1, gdi_act_layer->height - 1, color);
        gdi_layer_pop_clip();
    }

    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_clear)
}

/**************************************************************

   FUNCTION NAME     : gdi_layer_rotate()

   PURPOSE           : rotate the layer content

   INPUT PARAMETERS  :
                  GDI_LAYER_ROTATE_0         
                  GDI_LAYER_ROTATE_90        
                  GDI_LAYER_ROTATE_180       
                  GDI_LAYER_ROTATE_270    
                  GDI_LAYER_ROTATE_0_MIRROR  
                  GDI_LAYER_ROTATE_90_MIRROR 
                  GDI_LAYER_ROTATE_180_MIRROR
                  GDI_LAYER_ROTATE_270_MIRROR

   OUTPUT PARAMETERS : nil

   RETURNS           : void
***************************************************************/
/*
 * void gdi_layer_rotate(U8 rotate)
 * {
 * GDI_ENTER_CRITICAL_SECTION(gdi_layer_rotate)
 * switch(rotate)
 * {
 * case GDI_LAYER_ROTATE_0: //nothing to do
 * break;
 * case GDI_LAYER_ROTATE_90:
 * }
 * GDI_EXIT_CRITICAL_SECTION(gdi_layer_rotate)
 * }
 */


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_get_buffer_ptr
 * DESCRIPTION
 *  get currently active layer's frame buffer pointer.
 * PARAMETERS
 *  buf_ptr     [OUT]       
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_layer_get_buffer_ptr(U8 **buf_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_get_buffer_ptr)
    *buf_ptr = gdi_act_layer->buf_ptr;
    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_get_buffer_ptr)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_get_dimension
 * DESCRIPTION
 *  get currently active layer's size.
 * PARAMETERS
 *  width       [OUT]       
 *  height      [OUT]       
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_layer_get_dimension(S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_get_dimension)
    *width = gdi_act_layer->width;
    *height = gdi_act_layer->height;
    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_get_dimension)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_get_position
 * DESCRIPTION
 *  get currently active layer's position.
 * PARAMETERS
 *  offset_x        [OUT]       
 *  offset_y        [OUT]       
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_layer_get_position(S32 *offset_x, S32 *offset_y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_get_position)
    *offset_x = (S32) gdi_act_layer->offset_x;
    *offset_y = (S32) gdi_act_layer->offset_y;
    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_get_position)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_debug_blt
 * DESCRIPTION
 *  use when ASSERT or Fatal Error happen to print debug info.
 * PARAMETERS
 *  void
 * RETURNS
 *  GDI_RESULT(?)
 *****************************************************************************/
void gdi_layer_debug_blt(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_debug_blt)
    /* config hardware register for base layer */
    reset_lcd_if();
    config_lcd_layer_window(LCD_LAYER0, &(gdi_layer_info[((gdi_layer_struct*) GDI_LAYER_MAIN_BASE_LAYER_HANDLE)->id]));

    assert_lcd_fb_update(
        MAIN_LCD,
        0,                  /* LCM's start x */
        0,                  /* LCM's start y */
        GDI_LCD_WIDTH - 1,  /* LCM's width */
        GDI_LCD_HEIGHT - 1, /* LCM's width */
        GDI_LAYER_OFFSET,   /* ROI offset x */
        GDI_LAYER_OFFSET,   /* ROI offset y */
        (kal_uint32) LCD_LAYER0_ENABLE);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_debug_blt)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_flatten_ext
 * DESCRIPTION
 *  flatten previous blt layers to base layer
 * PARAMETERS
 *  handle0     [IN]        
 *  handle1     [IN]        
 *  handle2     [IN]        
 *  handle3     [IN]        
 *  handle4     [IN]        
 *  handle5     [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_layer_flatten_ext(gdi_handle handle0, gdi_handle handle1, gdi_handle handle2, gdi_handle handle3
#ifdef GDI_6_LAYERS
                                 , gdi_handle handle4, gdi_handle handle5
#endif 
    )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_flatten_to_base)
#if defined(GDI_USING_LAYER)

    lcd_frame_update_to_mem_struct update_mem_data;
    U32 layer_flag = 0;
    gdi_color background = 0;

    /* S32 handle_xor_value = 0; */

     
    //If only one handle is valid and it is the active layer handle, we won't flatten it
    //We use some tricky way to check this.
    //We use XOR to check if there is one layer handle equal to the active layer handle.(0)
    //Other layer handles are equal to GDI_ERROR_HANDLE or GDI_NULL_HANDLE.
    //handle_xor_value = handle0 ^ handle1 ^ handle2 ^ handle3 
    //#ifdef GDI_6_LAYERS
    //                ^ handle4 ^ handle5
    //#endif
    //                ^ (S32)gdi_act_layer
    //                ;
    //if ((handle_xor_value== GDI_ERROR_HANDLE)||(handle_xor_value== GDI_NULL_HANDLE))
    //        GDI_RETURN(GDI_SUCCEED);

#ifdef GDI_6_LAYERS
			SET_HARDWARE_LAYER_ROTATE(5);
			SET_HARDWARE_LAYER_ROTATE(4);
    SHIFT_LAYER_OFFSET(5, -GDI_LAYER_OFFSET, -GDI_LAYER_OFFSET);
    SHIFT_LAYER_OFFSET(4, -GDI_LAYER_OFFSET, -GDI_LAYER_OFFSET);
    CONFIG_HARDWARE_LAYER(5);
    CONFIG_HARDWARE_LAYER(4);
    SHIFT_LAYER_OFFSET(5, GDI_LAYER_OFFSET, GDI_LAYER_OFFSET);
    SHIFT_LAYER_OFFSET(4, GDI_LAYER_OFFSET, GDI_LAYER_OFFSET);
#endif /* GDI_6_LAYERS */ 
			SET_HARDWARE_LAYER_ROTATE(3);
			SET_HARDWARE_LAYER_ROTATE(2);
			SET_HARDWARE_LAYER_ROTATE(1);
			SET_HARDWARE_LAYER_ROTATE(0);
    SHIFT_LAYER_OFFSET(3, -GDI_LAYER_OFFSET, -GDI_LAYER_OFFSET);
    SHIFT_LAYER_OFFSET(2, -GDI_LAYER_OFFSET, -GDI_LAYER_OFFSET);
    SHIFT_LAYER_OFFSET(1, -GDI_LAYER_OFFSET, -GDI_LAYER_OFFSET);
    SHIFT_LAYER_OFFSET(0, -GDI_LAYER_OFFSET, -GDI_LAYER_OFFSET);
    CONFIG_HARDWARE_LAYER(3);
    CONFIG_HARDWARE_LAYER(2);
    CONFIG_HARDWARE_LAYER(1);
    CONFIG_HARDWARE_LAYER(0);
    SHIFT_LAYER_OFFSET(3, GDI_LAYER_OFFSET, GDI_LAYER_OFFSET);
    SHIFT_LAYER_OFFSET(2, GDI_LAYER_OFFSET, GDI_LAYER_OFFSET);
    SHIFT_LAYER_OFFSET(1, GDI_LAYER_OFFSET, GDI_LAYER_OFFSET);
    SHIFT_LAYER_OFFSET(0, GDI_LAYER_OFFSET, GDI_LAYER_OFFSET);

    update_mem_data.fb_update_mode = LCD_SW_TRIGGER_MODE;
    update_mem_data.block_mode = LCD_WM_BLOCK_MODE;
    update_mem_data.dest_block_width = gdi_act_layer->width;
    update_mem_data.dest_buffer_address = (kal_uint32) gdi_act_layer->buf_ptr;
    update_mem_data.dest_buffer_size = gdi_act_layer->layer_size;
    update_mem_data.roi_offset_x = 0;
    update_mem_data.roi_offset_y = 0;
    update_mem_data.roi_width = gdi_act_layer->width;
    update_mem_data.roi_height = gdi_act_layer->height;
    update_mem_data.update_layer = layer_flag;
    update_mem_data.hw_update_layer = 0;

    update_mem_data.roi_background_color = background;

#ifdef LCD_WMEM_RGB565
    switch (gdi_act_layer->vcf)
    {
        case GDI_COLOR_FORMAT_16:
            update_mem_data.memory_data_format = LCD_WMEM_RGB565;
            break;
        case GDI_COLOR_FORMAT_24:
        case GDI_COLOR_FORMAT_32:
            update_mem_data.memory_data_format = LCD_WMEM_RGB888;
            break;
        default:
            GDI_DEBUG_ASSERT(0);    /* didn't support this color format */
    }
#endif /* LCD_WMEM_RGB565 */ 

    if (!GDI_LCD->is_freeze)
    {
        lcd_fb_update_to_memory(&update_mem_data);
    }
#if defined(GDI_USING_2D_ENGINE_V2)
	if(gdi_act_layer->vcf == GDI_COLOR_FORMAT_32)
	{
		gdi_bitblt_internal(gdi_act_layer->buf_ptr, gdi_act_layer->width, 
			0, 0, 
			gdi_act_layer->width, gdi_act_layer->height,
			GDI_COLOR_FORMAT_24,
			gdi_act_layer->buf_ptr, gdi_act_layer->width,
			0, 0,
			0, 0, gdi_act_layer->width - 1, gdi_act_layer->height - 1,
			gdi_act_layer->vcf, gdi_act_layer->layer_size,
			FALSE,
			FALSE, 0,
			FALSE, 0,
			FALSE, 0,
			GDI_TRANSFORM_NONE,
			GDI_TRANSFORM_DIRECTION_RB);
	}
#endif

#endif /* defined(GDI_USING_LAYER) */ 
    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_flatten_to_base)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_flatten_with_clipping_ext
 * DESCRIPTION
 *  
 * PARAMETERS
 *  handle0     [IN]        
 *  handle1     [IN]        
 *  handle2     [IN]        
 *  handle3     [IN]        
 *  handle4     [IN]        
 *  handle5     [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_layer_flatten_with_clipping_ext(gdi_handle handle0, gdi_handle handle1, gdi_handle handle2,
                                               gdi_handle handle3
#ifdef GDI_6_LAYERS
                                               , gdi_handle handle4, gdi_handle handle5
#endif 
    )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_flatten_with_clipping)
#if defined(GDI_USING_LAYER)

    lcd_frame_update_to_mem_struct update_mem_data;
    U32 layer_flag = 0;
    gdi_color background = 0;

    /* S32 handle_xor_value = 0; */

    //If only one handle is valid and it is the active layer handle, we won't flatten it
    //We use some tricky way to check this.
    //We use XOR to check if there is one layer handle equal to the active layer handle.(0)
    //Other layer handles are equal to GDI_ERROR_HANDLE or GDI_NULL_HANDLE.
    //handle_xor_value = handle0 ^ handle1 ^ handle2 ^ handle3 
    //#ifdef GDI_6_LAYERS
    //                ^ handle4 ^ handle5
    //#endif
    //                ^ (S32)gdi_act_layer
    //                ;
    //if ((handle_xor_value== GDI_ERROR_HANDLE)||(handle_xor_value== GDI_NULL_HANDLE))
    //        GDI_RETURN(GDI_SUCCEED);

#ifdef GDI_6_LAYERS
    SHIFT_LAYER_OFFSET(5, -GDI_LAYER_OFFSET, -GDI_LAYER_OFFSET);
    SHIFT_LAYER_OFFSET(4, -GDI_LAYER_OFFSET, -GDI_LAYER_OFFSET);
    CONFIG_HARDWARE_LAYER(5);
    CONFIG_HARDWARE_LAYER(4);
    SHIFT_LAYER_OFFSET(5, GDI_LAYER_OFFSET, GDI_LAYER_OFFSET);
    SHIFT_LAYER_OFFSET(4, GDI_LAYER_OFFSET, GDI_LAYER_OFFSET);
#endif /* GDI_6_LAYERS */ 
    SHIFT_LAYER_OFFSET(3, -GDI_LAYER_OFFSET, -GDI_LAYER_OFFSET);
    SHIFT_LAYER_OFFSET(2, -GDI_LAYER_OFFSET, -GDI_LAYER_OFFSET);
    SHIFT_LAYER_OFFSET(1, -GDI_LAYER_OFFSET, -GDI_LAYER_OFFSET);
    SHIFT_LAYER_OFFSET(0, -GDI_LAYER_OFFSET, -GDI_LAYER_OFFSET);
    CONFIG_HARDWARE_LAYER(3);
    CONFIG_HARDWARE_LAYER(2);
    CONFIG_HARDWARE_LAYER(1);
    CONFIG_HARDWARE_LAYER(0);
    SHIFT_LAYER_OFFSET(3, GDI_LAYER_OFFSET, GDI_LAYER_OFFSET);
    SHIFT_LAYER_OFFSET(2, GDI_LAYER_OFFSET, GDI_LAYER_OFFSET);
    SHIFT_LAYER_OFFSET(1, GDI_LAYER_OFFSET, GDI_LAYER_OFFSET);
    SHIFT_LAYER_OFFSET(0, GDI_LAYER_OFFSET, GDI_LAYER_OFFSET);

    update_mem_data.fb_update_mode = LCD_SW_TRIGGER_MODE;
    update_mem_data.block_mode = LCD_WM_BLOCK_MODE;
    update_mem_data.dest_block_width = gdi_act_layer->width;
    update_mem_data.dest_buffer_address = (kal_uint32) gdi_act_layer->buf_ptr;
    update_mem_data.dest_buffer_size = gdi_act_layer->layer_size;
    update_mem_data.roi_offset_x = gdi_act_layer->clipx1;
    update_mem_data.roi_offset_y = gdi_act_layer->clipy1;
    update_mem_data.roi_width = gdi_act_layer->clipx2 - gdi_act_layer->clipx1 + 1;
    update_mem_data.roi_height = gdi_act_layer->clipy2 - gdi_act_layer->clipy1 + 1;
    update_mem_data.update_layer = layer_flag;
    update_mem_data.hw_update_layer = 0;

#ifdef LCD_WMEM_RGB565
    switch (gdi_act_layer->vcf)
    {
        case GDI_COLOR_FORMAT_16:
            update_mem_data.memory_data_format = LCD_WMEM_RGB565;
            break;
        case GDI_COLOR_FORMAT_24:
        case GDI_COLOR_FORMAT_32:
            update_mem_data.memory_data_format = LCD_WMEM_RGB888;
            break;
        default:
            GDI_DEBUG_ASSERT(0);    /* didn't support this color format */
    }
#endif /* LCD_WMEM_RGB565 */ 

    if (!GDI_LCD->is_freeze)
    {
        lcd_fb_update_to_memory(&update_mem_data);
    }

#if defined(GDI_USING_2D_ENGINE_V2)
	if(gdi_act_layer->vcf == GDI_COLOR_FORMAT_32)
	{
		gdi_bitblt_internal(gdi_act_layer->buf_ptr, gdi_act_layer->width, 
			0, 0, 
			gdi_act_layer->width, gdi_act_layer->height,
			gdi_act_layer->vcf,
			gdi_act_layer->buf_ptr, gdi_act_layer->width,
			0, 0,
			0, 0, gdi_act_layer->width - 1, gdi_act_layer->height - 1,
			gdi_act_layer->vcf, gdi_act_layer->layer_size,
			FALSE,
			FALSE, 0,
			FALSE, 0,
			FALSE, 0,
			GDI_TRANSFORM_NONE,
			GDI_TRANSFORM_DIRECTION_RB);
	}
#endif
#endif /* defined(GDI_USING_LAYER) */ 
    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_flatten_with_clipping)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_flatten_to_base_ext
 * DESCRIPTION
 *  
 * PARAMETERS
 *  handle0     [IN]        
 *  handle1     [IN]        
 *  handle2     [IN]        
 *  handle3     [IN]        
 *  handle4     [IN]        
 *  handle5     [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_layer_flatten_to_base_ext(gdi_handle handle0, gdi_handle handle1, gdi_handle handle2, gdi_handle handle3
#ifdef GDI_6_LAYERS
                                         , gdi_handle handle4, gdi_handle handle5
#endif 
    )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_flatten_to_base)
    GDI_RESULT ret;
    gdi_handle base_handle;

    gdi_layer_get_base_handle(&base_handle);

    gdi_layer_push_and_set_active(base_handle);
    ret = gdi_layer_flatten_ext(handle0, handle1, handle2, handle3
#ifdef GDI_6_LAYERS
                                , handle4, handle5
#endif 
        );
    gdi_layer_pop_and_restore_active();
    GDI_RETURN(ret);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_flatten_to_base)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_flatten_previous_to_base
 * DESCRIPTION
 *  flatten previous blt layers to base layer
 * PARAMETERS
 *  void
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_layer_flatten_previous_to_base(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_flatten_previous_to_base)
  //  GDI_RESULT result;

#if defined(GDI_USING_LAYER)
    result = gdi_layer_flatten_to_base_ext(GDI_LCD->blt_handle[0], GDI_LCD->blt_handle[1], GDI_LCD->blt_handle[2],
                                      GDI_LCD->blt_handle[3]
#ifdef GDI_6_LAYERS
                                      , GDI_LCD->blt_handle[4], GDI_LCD->blt_handle[5]
#endif 
        );
    GDI_RETURN(result);
#else /* defined(GDI_USING_LAYER) */ 
    GDI_RETURN(GDI_SUCCEED);
#endif /* defined(GDI_USING_LAYER) */ 
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_flatten_previous_to_base)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_save_jpeg_file
 * DESCRIPTION
 *  snapshot video and save as jpeg file
 * PARAMETERS
 *  layer_handle        [IN]        Video play layer
 *  file_name           [IN]        Filename for jpeg
 * RETURNS
 *  MDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_layer_save_jpeg_file(gdi_handle layer_handle, PS8 file_name)
{
#ifdef GDI_USING_HW_JPEG
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    img_encode_struct encode_data;
    S32 image_width;
    S32 image_height;
    PU8 image_ptr;
    U32 file_size;
    S32 ret;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_and_set_active(layer_handle);
    gdi_layer_get_dimension(&image_width, &image_height);
    gdi_layer_get_buffer_ptr(&image_ptr);
    gdi_layer_pop_and_restore_active();

    encode_data.image_width = image_width;
    encode_data.image_height = image_height;

    encode_data.image_quality = 2;
    encode_data.image_buffer_p = (void*)image_ptr;
    encode_data.image_buffer_size = (image_width * image_height * GDI_MAINLCD_BIT_PER_PIXEL) >> 3;

    encode_data.target_width = image_width;
    encode_data.target_height = image_height;
    encode_data.media_mode = MED_MODE_FILE;
    encode_data.media_type = MED_TYPE_JPG;

    encode_data.data = (void*)file_name;
    encode_data.file_size_p = (kal_uint32*) & file_size;
    encode_data.blocking = (kal_bool) TRUE;         /* blocking */
    encode_data.seq_num = (kal_uint16) 0;           /* not used */
    encode_data.file_buffer_len = (kal_uint32) 0;   /* not used */
    encode_data.gray_mode = 0;

	
	image_width 		= image_width/8*8;
	image_height 		= image_height/8*8;

  	ret = MMC_JpegEncode(file_name,1, image_width, image_height,(short*) image_ptr,
							(void*)&file_size);

//    ret = media_img_encode(stack_get_active_module_id(), &encode_data);

    if (ret == MED_RES_OK)
    {
        return GDI_SUCCEED;
    }
#endif /* GDI_USING_HW_JPEG */ 
    return GDI_FAILED;
//    GDI_RETURN(ret);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_save_pbm_file
 * DESCRIPTION
 *  
 * PARAMETERS
 *  layer_handle        [IN]        
 *  file_name           [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_layer_save_pbm_file(gdi_handle layer_handle, PS8 file_name)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_save_pbm_file)
    S32 image_width, image_height;
    PU8 image_ptr;
    U8 image_depth;

    image_width = ((gdi_layer_struct*) layer_handle)->width;
    image_height = ((gdi_layer_struct*) layer_handle)->height;
    image_ptr = ((gdi_layer_struct*) layer_handle)->buf_ptr;
    image_depth = (((gdi_layer_struct*) layer_handle)->bits_per_pixel) >> 3;

    GDI_RETURN(gdi_image_device_bmp_encode_file(image_width, image_height, image_depth, image_ptr, file_name));
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_save_pbm_file)
}

/*****************************************************************************
* DESCRIPTION
*  get the active layer pixel size (in bits)
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_get_bit_per_pixel
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
int gdi_layer_get_bit_per_pixel(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return GDI_LAYER.bits_per_pixel;
}

/*****************************************************************************
* DESCRIPTION
*  resize the active layer , the new size should not large then the layer_size
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_resize
 * DESCRIPTION
 *  
 * PARAMETERS
 *  width       [IN]        
 *  height      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_resize(S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_resize)
    GDI_DEBUG_ASSERT(gdi_sizeof_pixels(gdi_act_layer->cf, width, height) <= gdi_act_layer->layer_size);
    gdi_act_layer->width = (U16) width;
    gdi_act_layer->height = (U16) height;
    gdi_act_layer_info->column_number = (U16) width;
    gdi_act_layer_info->row_number = (U16) height;

    //#if defined(GDI_USING_LAYER_ROTATE)
    //if (gdi_act_layer_info->rotate_value != GDI_LAYER_ROTATE_0)
    //{
    //        gdi_layer_set_rotate(gdi_act_layer_info->rotate_value);
    //}
    //#endif

    GDI_LAYER_RESET_CLIPS(gdi_act_layer);
    GDI_LAYER_SET_CLIP(gdi_act_layer, 0, 0, width - 1, height - 1);

    GDI_EXIT_CRITICAL_SECTION(gdi_layer_resize)
}

/*****************************************************************************
* DESCRIPTION
*  set the base/active layer rotate value.
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_set_rotate
 * DESCRIPTION
 *  
 * PARAMETERS
 *  rotate_value        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_set_rotate(U8 rotate_value)
{
#if defined(GDI_USING_LAYER_ROTATE)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* S32 x1, y1, x2, y2; */

    GDI_ENTER_CRITICAL_SECTION(gdi_layer_set_rotate)
    gdi_act_layer->rotate_value = rotate_value;
    /* gdi_act_layer_info->rotate_value                             = rotate_value; */

    /* Change device position */
    //x1 = gdi_act_layer->offset_x;
    //y1 = gdi_act_layer->offset_y;
    //x2 = gdi_act_layer->offset_x + gdi_act_layer->width - 1;
    //y2 = gdi_act_layer->offset_y + gdi_act_layer->height - 1;
    //gdi_layer_inverse_map_rotated_region(rotate_value, &x1, &y1, &x2, &y2);
    //gdi_act_layer_info->x_offset = x1 + GDI_LAYER_OFFSET;
    //gdi_act_layer_info->y_offset = y1 + GDI_LAYER_OFFSET;
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_set_rotate)
#endif /* defined(GDI_USING_LAYER_ROTATE) */ 
}

/*****************************************************************************
* DESCRIPTION
*  copy the active buffer to another buffer
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_copy_double
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_copy_double(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_copy_double)

    /* active layer should be doubled layer */
    GDI_DEBUG_ASSERT(GDI_LAYER_GET_FLAG(gdi_act_layer, GDI_LAYER_FLAG_DOUBLE_LAYER));

    /* swap those two buffer */
    memcpy(gdi_act_layer->buf_ptr, gdi_act_layer->buf_ptr1, gdi_act_layer->layer_size);

    GDI_EXIT_CRITICAL_SECTION(gdi_layer_copy_double)
}

/*****************************************************************************
* DESCRIPTION
*  set the palette
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_select_palette
 * DESCRIPTION
 *  
 * PARAMETERS
 *  palette_number      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_select_palette(U8 palette_number)
{
#ifdef GDI_SUPPORT_PALETTES
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_select_palette)
    gdi_act_layer_info->color_palette_select = palette_number;
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_select_palette)
#else /* GDI_SUPPORT_PALETTES */ 
    GDI_ASSERT(0);
#endif /* GDI_SUPPORT_PALETTES */ 
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_set_palette
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]        
 *  color       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_set_palette(U8 index, gdi_color color)
{
#ifdef GDI_SUPPORT_PALETTES
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_set_palette)
    set_lcd_color_palette(gdi_act_layer_info->color_palette_select, ((U32*) & color) - index, index, 1);
    gdi_layer_palette[gdi_act_layer_info->color_palette_select][index] = color;
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_set_palette)
#else /* GDI_SUPPORT_PALETTES */ 
    GDI_ASSERT(0);
#endif /* GDI_SUPPORT_PALETTES */ 
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_get_palette
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
gdi_color *gdi_layer_get_palette(void)
{
#ifdef GDI_SUPPORT_PALETTES
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_get_palette)
    GDI_RETURN_TYPE(gdi_color *, &gdi_layer_palette[gdi_act_layer_info->color_palette_select][0]);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_get_palette)
#else /* GDI_SUPPORT_PALETTES */ 
    GDI_ASSERT(0);
    GDI_RETURN_TYPE(gdi_color *, 0);   //null
#endif /* GDI_SUPPORT_PALETTES */ 
}


/*****************************************************************************
 * FUNCTION
 *  gdi_waiting_blt_finish
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_waiting_blt_finish(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    do
    {
        GDI_LOCK;
        if (gdi_is_blting == FALSE)
        {
            break;
        }
        GDI_UNLOCK;
    #ifndef MMI_ON_WIN32
        kal_sleep_task(1);
    #endif 
    } while (1);
    GDI_UNLOCK;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_enable_non_block_blt
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_enable_non_block_blt(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_enable_non_block_blt)
    gdi_is_non_block_blt = TRUE;
    GDI_EXIT_CRITICAL_SECTION(gdi_enable_non_block_blt)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_disable_non_block_blt
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_disable_non_block_blt(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_waiting_blt_finish();
    GDI_ENTER_CRITICAL_SECTION(gdi_disable_non_block_blt)
    gdi_is_non_block_blt = FALSE;
    GDI_EXIT_CRITICAL_SECTION(gdi_disable_non_block_blt)

}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_enter_draw_section
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_enter_draw_section(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_waiting_blt_finish();
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_enter_draw_section)
    if (gdi_draw_section_level == 0)
    {
        GDI_LAYER_CLEAR_FLAG(gdi_act_layer, GDI_LAYER_FLAG_FROZEN);
    }
    gdi_draw_section_level++;
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_enter_draw_section)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_exit_draw_section
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_exit_draw_section(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_exit_draw_section)
    gdi_draw_section_level--;
    GDI_DEBUG_ASSERT(gdi_draw_section_level >= 0);
    if (gdi_draw_section_level == 0)
    {
        GDI_LAYER_SET_FLAG(gdi_act_layer, GDI_LAYER_FLAG_FROZEN);
    }

    GDI_EXIT_CRITICAL_SECTION(gdi_layer_exit_draw_section)
}

/* This function is called by LCD driver HISR */


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_blt_finish_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_blt_finish_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    //GDI_ENTER_CRITICAL_SECTION(gdi_layer_blt_finish_handler)
    gdi_is_blting = FALSE;
    /* GDI_EXIT_CRITICAL_SECTION(gdi_layer_blt_finish_handler) */
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   GDI Layer Manager
////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_add
 * DESCRIPTION
 *  
 * PARAMETERS
 *  parent          [IN]        
 *  child           [IN]        
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_add(gdi_handle parent, gdi_handle child, S32 offset_x, S32 offset_y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_add)
    gdi_layer_struct *p = (gdi_layer_struct*) parent;

    ((gdi_layer_struct*) child)->offset_x = offset_x;
    ((gdi_layer_struct*) child)->offset_y = offset_y;
    ((gdi_layer_struct*) child)->sib = NULL;    /* just make sure sib list is clear */

    if (p->child == NULL)   /* first child */
    {
        p->child = (gdi_layer_struct*) child;
    }
    else
    {
        p = p->child;
        while (p->sib != NULL)
        {
            p = p->sib;
        }
        p->sib = (gdi_layer_struct*) child;
    }

    GDI_EXIT_CRITICAL_SECTION(gdi_layer_add)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_del
 * DESCRIPTION
 *  
 * PARAMETERS
 *  parent      [IN]        
 *  child       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
BOOL gdi_layer_del(gdi_handle parent, gdi_handle child)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_add)
    BOOL ret = FALSE;
    gdi_layer_struct *p = (gdi_layer_struct*) parent;
    gdi_layer_struct *c = (gdi_layer_struct*) child;

    if (p->child == NULL)
    {
        /* nothing to do */
    }
    else if (p->child == c) /* first child */
    {
        p->child = c->sib;  /* remove it */
        c->sib = NULL;
        ret = TRUE;
    }
    else
    {
        p = p->child;
        while (p->sib != NULL && p->sib != c)
        {
            p = p->sib;
        }

        if (p->sib == c)    /* in sib child */
        {
            p->sib = c->sib;    /* remove it */
            c->sib = NULL;
            ret = TRUE;
        }
    }
    GDI_RETURN(ret);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_add)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_del_all_child
 * DESCRIPTION
 *  
 * PARAMETERS
 *  parent      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_del_all_child(gdi_handle parent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gdi_layer_struct *root = (gdi_layer_struct*) parent;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (root->child)
    {
        root->child = NULL;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_blt_tree
 * DESCRIPTION
 *  
 * PARAMETERS
 *  root        [IN]        
 *  x1          [IN]        
 *  y1          [IN]        
 *  x2          [IN]        
 *  y2          [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_blt_tree(gdi_handle root, S32 x1, S32 y1, S32 x2, S32 y2)
{
#ifndef GDI_USING_LAYER
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_blt(root, 0, 0, 0, x1, y1, x2, y2);
#else /* GDI_USING_LAYER */ 
    GDI_ENTER_CRITICAL_SECTION(gdi_lyaer_blt_tree)
    gdi_layer_struct *node = (gdi_layer_struct*) root;
    static gdi_handle visit_list[GDI_LAYER_HW_LAYER_COUNT + 1];

    if (gdi_layer_last_root != node)
    {
        gdi_layer_struct *stack[GDI_LAYER_TREE_MAX_DEPTH];
        gdi_layer_struct **stack_end = stack;
        gdi_handle *visit_list_end = visit_list;
        S32 parent_x, parent_y;

#define PUSH(NODE)   *stack_end++ = NODE;GDI_DEBUG_ASSERT(stack_end< stack+GDI_LAYER_TREE_MAX_DEPTH)
#define POP()        ((stack<stack_end)?*(--stack_end):NULL)

        gdi_layer_last_root = node; /* cache this root */

        PUSH(node);
        while ((node = POP()) != NULL)
        {
            *visit_list_end++ = (gdi_handle) node;  /* store to visit list */

            if (visit_list_end > visit_list + GDI_LAYER_HW_LAYER_COUNT) /* current visit_list is GDI_LAYER_HW_LAYER_COUNT+1 items */
            {
                gdi_layer_push_and_set_active(gdi_work_buffer_handle);
            #ifdef GDI_4_LAYERS
                gdi_layer_flatten_ext(visit_list[0], visit_list[1], visit_list[2], visit_list[3]);
            #else /* GDI_4_LAYERS */ 
                gdi_layer_flatten_ext(
                    visit_list[0],
                    visit_list[1],
                    visit_list[2],
                    visit_list[3],
                    visit_list[4],
                    visit_list[5]);
            #endif /* GDI_4_LAYERS */ 
                visit_list[0] = gdi_work_buffer_handle;
                visit_list[1] = visit_list[GDI_LAYER_HW_LAYER_COUNT];
                visit_list_end = visit_list + 2;
                gdi_layer_pop_and_restore_active();
            }

            parent_x = (S32) gdi_layer_info[node->id].x_offset;
            parent_y = (S32) gdi_layer_info[node->id].y_offset;

            if (node->sib && node->sib != (gdi_layer_struct*) root)
            {
                gdi_layer_info[node->sib->id].x_offset = (U16) (parent_x + node->sib->offset_x);
                gdi_layer_info[node->sib->id].y_offset = (U16) (parent_y + node->sib->offset_y);
                PUSH(node->sib);
            }
            if (node->child)
            {
                gdi_layer_info[node->child->id].x_offset = (U16) (parent_x + node->child->offset_x);
                gdi_layer_info[node->child->id].y_offset = (U16) (parent_y + node->child->offset_y);
                PUSH(node->child);
            }

        }
#undef PUSH
#undef POP

        /* clear remind list */
        for (; visit_list_end < visit_list + GDI_LAYER_HW_LAYER_COUNT;)
        {
            *visit_list_end++ = 0;
        }
    }

    /* blt those layer */
    gdi_layer_blt_ext(visit_list[0], visit_list[1], visit_list[2], visit_list[3],
#ifdef GDI_6_LAYERS
                      visit_list[4], visit_list[5],
#endif 
                      x1, y1, x2, y2);
    GDI_EXIT_CRITICAL_SECTION(gdi_lyaer_blt_tree)
#endif /* GDI_USING_LAYER */ 
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_set_background
 * DESCRIPTION
 *  
 * PARAMETERS
 *  color       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
BOOL gdi_layer_set_background(gdi_color color)
{
#ifdef GDI_USING_LAYER_BACKGROUND
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 A, R, G, B;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_set_background)
    gdi_act_color_to_rgb(&A, &R, &G, &B, color);
    gdi_act_layer->background = gd_color_from_rgb_24(A, R, G, B);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_set_background)
    return TRUE;
#else /* GDI_USING_LAYER_BACKGROUND */ 
    return FALSE;
#endif /* GDI_USING_LAYER_BACKGROUND */ 
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_get_background
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
gdi_color gdi_layer_get_background(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_act_layer->background;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_apply_gray_effect
 * DESCRIPTION
 *  apply gray scale effect to the active layer
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_layer_apply_gray_effect(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_apply_gray_effect)
#if defined(CSD_TARGET)
    if (GDI_COLOR_FORMAT_16 == gdi_act_layer->vcf && 
        !(gdi_act_layer->layer_size & 0xf) && !((S32) gdi_act_layer->buf_ptr & 0x3))
    {
        gd_gray_16(gdi_act_layer->buf_ptr, gdi_act_layer->layer_size);
    }
    else
#endif /* defined(CSD_TARGET) */ 
    {
        S32 x = 0;
        S32 y = 0;
        gdi_color color_t;
        U32 a, r, g, b; 
        gd_get_pixel_func get_ptr = gd_get_pixel[gdi_act_layer->cf];
        gd_put_pixel_func put_ptr = gd_put_pixel[gdi_act_layer->cf];

        for (y = 0; y < gdi_act_layer->height; y++)
        {
            for (x = 0; x < gdi_act_layer->width; x++)
            {
                color_t = get_ptr(x,y);
                gdi_act_color_to_rgb(&a,&r,&g,&b,color_t);
                r = (r + (g << 1) + b)>>2;
                if (r > GDI_GRAY_BOUNDARY)
                {
                    r = GDI_GRAY_BOUNDARY;
                }
                put_ptr(x,y,gdi_act_color_from_rgb(a,r,r,r));
            }
        }
    }
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_apply_gray_effect)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_add_post_effect
 * DESCRIPTION
 *  add post effect to a layer
 * PARAMETERS
 *  effect_id           [IN]        Effect type
 *  layer_handle(?)     [IN]        Layer handle
 * RETURNS
 *  void
 *****************************************************************************/
GDI_RESULT gdi_layer_add_post_effect(U16 effect_id)
{
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_add_post_effect)
#if defined(ISP_SUPPORT) && defined(CSD_TARGET)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 layer_width;
    S32 layer_height;
    S32 bits_per_pixel;
    PU8 buf_ptr;
    S32 buf_size;
    PU8 work_buf_ptr_1;
    PU8 work_buf_ptr_2;
    U16 drv_effect_id;

    IMAGE_EFFECT_DEC_STRUCT effect_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    drv_effect_id = 0;

#define POST_EFFECT_MAPPING_SWITCH(__effect__)      \
      case GDI_LAYER_POST_EFFECT_##__effect__##:       \
         drv_effect_id = CAM_EFFECT_DEC_##__effect__##;\
         break;                                        \

    switch (effect_id)
    {
            POST_EFFECT_MAPPING_SWITCH(NORMAL);
            POST_EFFECT_MAPPING_SWITCH(GRAYSCALE);
            POST_EFFECT_MAPPING_SWITCH(SEPIA);
            POST_EFFECT_MAPPING_SWITCH(SEPIAGREEN);
            POST_EFFECT_MAPPING_SWITCH(SEPIABLUE);
            POST_EFFECT_MAPPING_SWITCH(COLORINV);
            POST_EFFECT_MAPPING_SWITCH(GRAYINV);
            POST_EFFECT_MAPPING_SWITCH(WATERCOLOR);
            POST_EFFECT_MAPPING_SWITCH(LIGHTBLUR);
            POST_EFFECT_MAPPING_SWITCH(BLUR);
            POST_EFFECT_MAPPING_SWITCH(STRONGBLUR);
            POST_EFFECT_MAPPING_SWITCH(UNSHARP);
            POST_EFFECT_MAPPING_SWITCH(SHARPEN);
            POST_EFFECT_MAPPING_SWITCH(MORESHARPEN);
            POST_EFFECT_MAPPING_SWITCH(MEDIAN);
            POST_EFFECT_MAPPING_SWITCH(DILATION);
            POST_EFFECT_MAPPING_SWITCH(EROSION);
        default:
            /* shall not enter here */
            MMI_ASSERT(0);
            break;
    }

    layer_width = gdi_act_layer->width;
    layer_height = gdi_act_layer->height;
    buf_ptr = gdi_act_layer->buf_ptr;
    bits_per_pixel = gdi_act_layer->bits_per_pixel;

    buf_size = (layer_width * layer_height * bits_per_pixel) >> 3;

    /* allocate from media task */
    media_get_ext_buffer(MOD_MMI, (void **)&work_buf_ptr_1, ((layer_width * layer_height * bits_per_pixel) >> 3));

    media_get_ext_buffer(MOD_MMI, (void **)&work_buf_ptr_2, ((layer_width * layer_height * bits_per_pixel) >> 3));

    /* NOTE: only some effect needs two buffer, like "water paint", etc */

    /* TODO: fix when support 888 format */
    switch (gdi_act_layer->vcf)
    {
        case GDI_COLOR_FORMAT_16:
            effect_data.data_type = IBR1_TYPE_RGB565;
            break;
        case GDI_COLOR_FORMAT_24:
            effect_data.data_type = IBR1_TYPE_RGB888;
            break;
        default:
            GDI_ASSERT(0);
    }
    effect_data.data_order = IBR1_ORDER_BGR888; /* not used - just give it a value */
    effect_data.source_buffer_address = (kal_uint32) buf_ptr;
    effect_data.dest1_buffer_address = (kal_uint32) work_buf_ptr_1;
    effect_data.dest2_buffer_address = (kal_uint32) work_buf_ptr_2;
    effect_data.image_width = layer_width;
    effect_data.image_height = layer_height;
    effect_data.effect_index = drv_effect_id;
    effect_data.adj_level = 0;                  /* not used */
    effect_data.image_effect_dec_cb = NULL;     /* not used */

    image_effect_dec_process(&effect_data);

    /* copy result to soruce layer */
    memcpy(buf_ptr, work_buf_ptr_1, buf_size);

    /* free buffer */
    media_free_ext_buffer(MOD_MMI, (void **)&work_buf_ptr_1);
    media_free_ext_buffer(MOD_MMI, (void **)&work_buf_ptr_2);

    GDI_RETURN(GDI_SUCCEED);

#else /* defined(ISP_SUPPORT) && defined(CSD_TARGET) */ 
    GDI_RETURN(GDI_FAILED);
#endif /* defined(ISP_SUPPORT) && defined(CSD_TARGET) */ /* ISP_SUPPORT && MMI_ON_HARDWARE_P */
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_add_post_effect)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_add_post_adjustment
 * DESCRIPTION
 *  preview fail handler (driver error)
 * PARAMETERS
 *  adjustment_id       [IN]        Adjustment type
 *  value               [IN]        Adjustment value
 *  layer_handle(?)     [IN]        Layer handle
 * RETURNS
 *  void
 *****************************************************************************/
GDI_RESULT gdi_layer_add_post_adjustment(U16 adjustment_id, S32 value)
{
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_add_post_adjustment)
#if defined(ISP_SUPPORT) && defined(CSD_TARGET)

    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 layer_width;
    S32 layer_height;
    S32 bits_per_pixel;
    PU8 buf_ptr;
    S32 buf_size;
    PU8 work_buf_ptr_1;
    PU8 work_buf_ptr_2;
    U16 drv_adj_id;

    IMAGE_EFFECT_DEC_STRUCT effect_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    drv_adj_id = 0;

#define POST_ADJUSTMENT_MAPPING_SWITCH(__adj__)\
      case GDI_LAYER_ADJUSMENT_##__adj__##:       \
         drv_adj_id = CAM_ADJ_DEC_##__adj__##;    \
         break;                                   \

    switch (drv_adj_id)
    {
            POST_ADJUSTMENT_MAPPING_SWITCH(BRIGHTNESS);
            POST_ADJUSTMENT_MAPPING_SWITCH(CONTRAST);
            POST_ADJUSTMENT_MAPPING_SWITCH(SATURATION);
            POST_ADJUSTMENT_MAPPING_SWITCH(HUE);
            POST_ADJUSTMENT_MAPPING_SWITCH(ADJR);
            POST_ADJUSTMENT_MAPPING_SWITCH(ADJG);
            POST_ADJUSTMENT_MAPPING_SWITCH(ADJB);
        default:
            /* shall not enter here */
            MMI_ASSERT(0);
            break;
    }

    layer_width = gdi_act_layer->width;
    layer_height = gdi_act_layer->height;
    buf_ptr = gdi_act_layer->buf_ptr;
    bits_per_pixel = gdi_act_layer->bits_per_pixel;

    buf_size = (layer_width * layer_height * bits_per_pixel) >> 3;

    /* allocate from media task */
    media_get_ext_buffer(MOD_MMI, (void **)&work_buf_ptr_1, ((layer_width * layer_height * bits_per_pixel) >> 3));

    media_get_ext_buffer(MOD_MMI, (void **)&work_buf_ptr_2, ((layer_width * layer_height * bits_per_pixel) >> 3));

     
    memcpy(work_buf_ptr_2, buf_ptr, buf_size);
    memset(work_buf_ptr_1, 0, buf_size);

    /* TODO: fix when support 888 format */
    switch (gdi_act_layer->vcf)
    {
        case GDI_COLOR_FORMAT_16:
            effect_data.data_type = IBR1_TYPE_RGB565;
            break;
        case GDI_COLOR_FORMAT_24:
            effect_data.data_type = IBR1_TYPE_RGB888;
            break;
        default:
            GDI_ASSERT(0);
    }
    effect_data.data_order = IBR1_ORDER_BGR888;             /* not used - just give it a value */
    effect_data.source_buffer_address = (kal_uint32) work_buf_ptr_2;
    effect_data.dest1_buffer_address = (kal_uint32) work_buf_ptr_1;
    effect_data.dest2_buffer_address = (kal_uint32) NULL;   /* not used */
    effect_data.image_width = layer_width;
    effect_data.image_height = layer_height;
    effect_data.effect_index = drv_adj_id;
    effect_data.adj_level = value;
    effect_data.image_effect_dec_cb = NULL;                 /* not used */

    image_adj_dec_process(&effect_data);

    /* copy result to soruce layer */
    memcpy(buf_ptr, work_buf_ptr_1, buf_size);

    /* free buffer */
    media_free_ext_buffer(MOD_MMI, (void **)&work_buf_ptr_1);
    media_free_ext_buffer(MOD_MMI, (void **)&work_buf_ptr_2);

    GDI_RETURN(GDI_SUCCEED);
#else /* defined(ISP_SUPPORT) && defined(CSD_TARGET) */ 
    GDI_RETURN(GDI_FAILED);
#endif /* defined(ISP_SUPPORT) && defined(CSD_TARGET) */ /* ISP_SUPPORT && MMI_ON_HARDWARE_P */
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_add_post_adjustment)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_layer_is_double
 * DESCRIPTION
 *  
 * PARAMETERS
 *  layer       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
BOOL gdi_layer_is_double(gdi_handle layer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(GDI_LAYER_GET_FLAG(layer, GDI_LAYER_FLAG_DOUBLE_LAYER))
        return TRUE;
    else
        return FALSE;
}







/*****************************************************************************
 * FUNCTION
 *  gdi_layer_merge_for_video
 * DESCRIPTION
 *  merge layers.
 *  
 *  The last four parameters are the region which to blt to LCD.
 * PARAMETERS
 *  handle0     [IN]        
 *  handle1     [IN]        
 *  handle2     [IN]        
 *  handle3     [IN]        
 *  handle4     [IN]        
 *  handle5     [IN]        
 *  x1          [IN]        
 *  y1          [IN]        
 *  x2          [IN]        
 *  y2          [IN]        
 * RETURNS
 * BOOL
 *  
 *****************************************************************************/
GDI_RESULT gdi_layer_merge_for_video(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
 #ifdef MMI_ON_HARDWARE_P
#ifdef CHIP_HAS_GOUDA
        GDI_RETURN(GDI_SUCCEED);
#endif
#endif

	
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_blt)
    U32 layer_flag;
    gdi_color background;
    lcd_frame_update_struct lcd_update_frame;
	gdi_handle handle0 = GDI_LCD->blt_handle[0];
	gdi_handle handle1 = GDI_LCD->blt_handle[1];
	gdi_handle handle2 = GDI_LCD->blt_handle[2];
	gdi_handle handle3 = GDI_LCD->blt_handle[3];

#if defined(GDI_USING_LAYER_ROTATE) || defined(GDI_USING_LCD_ROTATE)
	S32 tempWidth;
	S32 tempHeight;
	
	if(mmi_frm_is_screen_width_height_swapped())
	{
		tempWidth = GDI_LCD->act_height;
		tempHeight = GDI_LCD->act_width;
		
		if (y2 < y1 || x2 < x1 || x1 >= tempWidth || x2 < 0 || y1 >= tempHeight || y2 < 0)
		{
			GDI_RETURN(GDI_LAYER_ERR_INVALID_BLT_REGION);
		}
	}
	else
#endif
    if (y2 < y1 || x2 < x1 || x1 >= GDI_LCD->act_width || x2 < 0 || y1 >= GDI_LCD->act_height || y2 < 0)
    {
        GDI_RETURN(GDI_LAYER_ERR_INVALID_BLT_REGION);
    }

/*wuzc Add Start For GDI/GUI Portting crash    on 2008-3-13 17:18 */
	if(handle0 == -1) handle0 = 0;
	if(handle1 == -1) handle1 = 0;
	if(handle2 == -1) handle2 = 0;
	if(handle3 == -1) handle3 = 0;
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-13 17:18 */
	
    /* store current blt layers, for gdi_layer_blt_previous uses */
    GDI_LCD->blt_handle[0] = handle0;
    GDI_LCD->blt_handle[1] = handle1;
    GDI_LCD->blt_handle[2] = handle2;
    GDI_LCD->blt_handle[3] = handle3;

    /* if the layer is locked and someone call blt. merge the maxium blt region */
    if (GDI_LCD->blt_lock > 0)
    {
        if (!GDI_LCD->blt_rect_is_used)
        {
            GDI_LCD->blt_rect.x1 = x1;
            GDI_LCD->blt_rect.y1 = y1;
            GDI_LCD->blt_rect.x2 = x2;
            GDI_LCD->blt_rect.y2 = y2;
            GDI_LCD->blt_rect_is_used = TRUE;
        }
        else
        {
            if (x1 < GDI_LCD->blt_rect.x1)
            {
                GDI_LCD->blt_rect.x1 = x1;
            }
            if (y1 < GDI_LCD->blt_rect.y1)
            {
                GDI_LCD->blt_rect.y1 = y1;
            }
            if (x2 > GDI_LCD->blt_rect.x2)
            {
                GDI_LCD->blt_rect.x2 = x2;
            }
            if (y2 > GDI_LCD->blt_rect.y2)
            {
                GDI_LCD->blt_rect.y2 = y2;
            }
        }
        GDI_RETURN(GDI_LAYER_DOUBLE_BUFFER_LOCKED);
    }

    if (GDI_LCD->blt_rect_is_used)
    {
        /* merge and calculate maxium blt region */
        if (x1 > GDI_LCD->blt_rect.x1)
        {
            x1 = GDI_LCD->blt_rect.x1;
        }
        if (y1 > GDI_LCD->blt_rect.y1)
        {
            y1 = GDI_LCD->blt_rect.y1;
        }
        if (x2 < GDI_LCD->blt_rect.x2)
        {
            x2 = GDI_LCD->blt_rect.x2;
        }
        if (y2 < GDI_LCD->blt_rect.y2)
        {
            y2 = GDI_LCD->blt_rect.y2;
        }

        GDI_LCD->blt_rect_is_used = FALSE;
    }
    /* adjust region to fit valid region */
#if defined(GDI_USING_LAYER_ROTATE) || defined(GDI_USING_LCD_ROTATE)
	if(mmi_frm_is_screen_width_height_swapped())
	{
		tempWidth = GDI_LCD->act_height;
		tempHeight = GDI_LCD->act_width;
		
		if (x1 < 0)
	    {
	        x1 = 0;
	    }
	    if (y1 < 0)
	    {
	        y1 = 0;
	    }
	    if (x2 > tempWidth - 1)
	    {
	        x2 = tempWidth - 1;
	    }
	    if (y2 > tempHeight - 1)
	    {
	        y2 = tempHeight - 1;
	    }
	}
	else
#endif
	{
    if (x1 < 0)
    {
        x1 = 0;
    }
    if (y1 < 0)
    {
        y1 = 0;
    }
    if (x2 > GDI_LCD->act_width - 1)
    {
        x2 = GDI_LCD->act_width - 1;
    }
    if (y2 > GDI_LCD->act_height - 1)
    {
        y2 = GDI_LCD->act_height - 1;
    }
	}
    /* config hardware register for each layer */
    layer_flag = 0;
    background = 0;

    gdi_lcd_set_blt_handle_rotate();

if (handle3 != 0)
    CONFIG_HARDWARE_LAYER(3);
if (handle2 != 0)
    CONFIG_HARDWARE_LAYER(2);
if (handle1 != 0)
    CONFIG_HARDWARE_LAYER(1);
if (handle0 != 0)
    CONFIG_HARDWARE_LAYER(0);

    /* BLOCK BLT */
    if (!gdi_is_non_block_blt
        || (handle0 && GDI_LAYER_GET_FLAG(handle0, GDI_LAYER_FLAG_DOUBLE_LAYER | GDI_LAYER_FLAG_FROZEN) == 0)
        || (handle1 && GDI_LAYER_GET_FLAG(handle1, GDI_LAYER_FLAG_DOUBLE_LAYER | GDI_LAYER_FLAG_FROZEN) == 0)
        || (handle2 && GDI_LAYER_GET_FLAG(handle2, GDI_LAYER_FLAG_DOUBLE_LAYER | GDI_LAYER_FLAG_FROZEN) == 0)
        || (handle3 && GDI_LAYER_GET_FLAG(handle3, GDI_LAYER_FLAG_DOUBLE_LAYER | GDI_LAYER_FLAG_FROZEN) == 0)
        )
    {
        lcd_update_frame.block_mode_flag = TRUE;
        lcd_update_frame.lcd_block_mode_cb = NULL;
    }
    else
    {
        lcd_update_frame.block_mode_flag = FALSE;
        lcd_update_frame.lcd_block_mode_cb = gdi_layer_blt_finish_handler;
        gdi_is_blting = TRUE;
    }


#ifdef GDI_USING_LAYER_OUTPUT_ANOTHER_MEMORY
    lcd_update_frame.memory_output = KAL_FALSE;
#endif 

    lcd_update_frame.module_id = LCD_UPDATE_MODULE_MMI;
    lcd_update_frame.fb_update_mode = LCD_SW_TRIGGER_MODE;
    lcd_update_frame.lcm_start_x = (kal_uint16) x1; /* LCM's start x */
    lcd_update_frame.lcm_start_y = (kal_uint16) y1; /* LCM's start y */
    lcd_update_frame.lcm_end_x = (kal_uint16) x2;   /* LCM's end x */
    lcd_update_frame.lcm_end_y = (kal_uint16) y2;   /* LCM's width */
    lcd_update_frame.roi_offset_x = (kal_uint16) (x1 + GDI_LAYER_OFFSET);       /* ROI offset x */
    lcd_update_frame.roi_offset_y = (kal_uint16) (y1 + GDI_LAYER_OFFSET);       /* ROI offset y */
    lcd_update_frame.update_layer = (kal_uint32) layer_flag;
    lcd_update_frame.hw_update_layer = 0;   /* not used in software mode */

#ifdef GDI_USING_LAYER_BACKGROUND
    lcd_update_frame.roi_background_color = background;
#endif 


#ifdef __MMI_SUBLCD__   /* actully have sub lcd - blt to it */
    if (GDI_LCD->act_handle == GDI_LCD_SUB_LCD_HANDLE)
    {
        lcd_update_frame.lcd_id = SUB_LCD;
    }
    else
#endif /* __MMI_SUBLCD__ */ 
        lcd_update_frame.lcd_id = MAIN_LCD;

#ifdef MMI_ON_WIN32
    /* pass necessary data to PC Simulator */
    WIN32_UI_SET_LAYER_DATA(0);
    WIN32_UI_SET_LAYER_DATA(1);
    WIN32_UI_SET_LAYER_DATA(2);
    WIN32_UI_SET_LAYER_DATA(3);
#endif /* MMI_ON_WIN32 */ 

    if (!GDI_LCD->is_freeze)
    {
        
		gdi_layer_Merge_frame_buffer(&lcd_update_frame);
    }


    /* reset blt lock value */
    GDI_LCD->blt_lock = 0;
    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_blt)
}
#define GDI_ALPHA_BLENDING_SOURCE_LAYER_STACK_COUNT      (8)
static gdi_handle gdi_alpha_blending_source_layer_stack[GDI_ALPHA_BLENDING_SOURCE_LAYER_STACK_COUNT];
static S32 gdi_alpha_blending_source_layer_stack_index = 0;
static gdi_handle g_alpha_blending_source_layer = GDI_ERROR_HANDLE;

/*****************************************************************************
 * FUNCTION
 *  gdi_set_alpha_blending_source_layer
 * DESCRIPTION
 *  set alpha source blending source layer
 * PARAMETERS
 *  src_layer_handle       [IN] gdi handle of the alpha source layer
 * RETURNS
 *  gdi_handle  old alpha source layer handle
 *****************************************************************************/
gdi_handle gdi_set_alpha_blending_source_layer(gdi_handle src_layer_handle)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gdi_handle  old_src_layer_handle;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_LOCK;
    
    old_src_layer_handle = g_alpha_blending_source_layer;
    g_alpha_blending_source_layer = src_layer_handle;  
    
    GDI_UNLOCK;

    return old_src_layer_handle;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_get_alpha_blending_source_layer
 * DESCRIPTION
 *  get alpha blending source layer
 * PARAMETERS
 *  src_layer_handle       [OUT] gdi handle of the alpha source layer
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_get_alpha_blending_source_layer(gdi_handle *src_layer_handle)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_LOCK;
    
    *src_layer_handle = g_alpha_blending_source_layer;
    
    GDI_UNLOCK;
}

/*****************************************************************************
 * FUNCTION
 *  gdi_push_and_set_alpha_blending_source_layer
 * DESCRIPTION
 *  push current alpha source layer into stack and set passin layer as alpha source layer
 * PARAMETERS
 *  handle      [IN]        
 * RETURNS
 *  GDI_RESULT(?)
 *****************************************************************************/
void gdi_push_and_set_alpha_blending_source_layer(gdi_handle handle)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_LOCK;
    /* no more stack entry available */
    GDI_ASSERT(gdi_alpha_blending_source_layer_stack_index <= GDI_ALPHA_BLENDING_SOURCE_LAYER_STACK_COUNT - 1);

    gdi_alpha_blending_source_layer_stack[gdi_alpha_blending_source_layer_stack_index++] = g_alpha_blending_source_layer;
    g_alpha_blending_source_layer = handle;

}


/*****************************************************************************
 * FUNCTION
 *  gdi_pop_and_restore_alpha_blending_source_layer
 * DESCRIPTION
 *  pop the layer in stack and make it as alpha source layer
 * PARAMETERS
 *  void
 * RETURNS
 *  GDI_RESULT(?)
 *****************************************************************************/
void gdi_pop_and_restore_alpha_blending_source_layer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ASSERT(gdi_alpha_blending_source_layer_stack_index > 0);
    g_alpha_blending_source_layer = gdi_alpha_blending_source_layer_stack[--gdi_alpha_blending_source_layer_stack_index];
    GDI_UNLOCK;
}

#define INVALID_RECT    (-1)
typedef struct 
{
    S32 x1, y1; /* top-left point */
    S32 x2, y2; /* bottom-right point */
} rect_struct;
//#include"gdi_image_alpha_bmp_internal.h"
/*****************************************************************************
 * FUNCTION
 *  get_draw_rect_in_layer_clip
 * DESCRIPTION
 *  calculate the overlapped rect between the image rect and layer clip rect
 * PARAMETERS
 *  x1          [IN]    x1 of the draw rect top-left point (x1, y1)
 *  y1          [IN]    y1 of the draw rect top-left point (x1, y1)
 *  x2          [IN]    x2 of the draw rect bottom-right point (x2, y2)
 *  y2          [IN]    y2 of the draw rect bottom-right point (x2, y2)
 *  layer       [IN]    layer handle
 * RETURNS
 *  clipped rect, if out of clip, x1 is INVALID_RECT (-1)
 *****************************************************************************/
static  rect_struct get_draw_rect_in_layer_clip(S32 x1, S32 y1, S32 x2, S32 y2, gdi_handle layer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32                 start_x, start_y, end_x, end_y;
    S32                 layer_clip_x1, layer_clip_y1, layer_clip_x2, layer_clip_y2;
    rect_struct         clip_rect = {INVALID_RECT, INVALID_RECT, INVALID_RECT, INVALID_RECT};
    gdi_layer_struct    *layer_struct;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    layer_struct = (gdi_layer_struct*)layer;

    /* get active layer clip */
    layer_clip_x1 = (S32)layer_struct->clipx1;
    layer_clip_y1 = (S32)layer_struct->clipy1;
    layer_clip_x2 = (S32)layer_struct->clipx2;
    layer_clip_y2 = (S32)layer_struct->clipy2;

    /* check if the draw rect is out of the clip */
    if ((x1 > layer_clip_x2) || (y1 > layer_clip_y2) || (x2 < layer_clip_x1) || (y2 < layer_clip_y1))
    {
        return clip_rect;
    }

    /* calculate the overlapped rect */
    start_y = MAX(y1, layer_clip_y1);
    end_y = MIN(y2, layer_clip_y2);
    start_x = MAX(x1, layer_clip_x1);
    end_x = MIN(x2, layer_clip_x2);

    clip_rect.x1 = start_x;
    clip_rect.y1 = start_y;
    clip_rect.x2 = end_x;
    clip_rect.y2 = end_y;

    return clip_rect;
}

void gdi_image_layer_copy(S32 x1, S32 y1, S32 x2, S32 y2, gdi_handle dst_layer, gdi_handle src_layer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gdi_layer_struct *dst, *src;
    S32     y;
    S32     src_line_jump_bytes, dst_line_jump_bytes;
    S32     line_size;
    S32     lcd_size;
    S32     src_bpp, dst_bpp;
    S32     src_pixel_size, dst_pixel_size; 
    U8      *dst_line_p, *src_line_p;
    S32     line_width;
    U32     color_888;
    rect_struct clip_rect;
    S32     src_layer_offset_x, src_layer_offset_y;
    S32     blending_layer_offset_x, blending_layer_offset_y;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* get destination layer offset */
    gdi_layer_get_position(&src_layer_offset_x, &src_layer_offset_y);
    
    /* get source layer offset */
    gdi_layer_push_and_set_active(src_layer);
    gdi_layer_get_position(&blending_layer_offset_x, &blending_layer_offset_y);
    gdi_layer_pop_and_restore_active();    
    src_layer_offset_x -= blending_layer_offset_x;
    src_layer_offset_y -= blending_layer_offset_y;
    
    /* remove a silly compiler warning */
    color_888 = 0;
    
    /* get clip rect */
    clip_rect = get_draw_rect_in_layer_clip(x1, y1, x2, y2, dst_layer);

    if (clip_rect.x1 == INVALID_RECT)
    {
        return;
    }

    x1 = clip_rect.x1;
    y1 = clip_rect.y1;
    x2 = clip_rect.x2;
    y2 = clip_rect.y2;

    /* get layer info */
    src = (gdi_layer_struct*)src_layer;
    dst = (gdi_layer_struct*)dst_layer;

    src_bpp = src->bits_per_pixel;
    dst_bpp = dst->bits_per_pixel;

    src_pixel_size = src_bpp / 8;
    dst_pixel_size = dst_bpp / 8;
if((0 == x1)&&(0 == y1)&&(x2 == (dst->width-1))&&(y2 == (dst->height-1)))
{
	lcd_size = dst->width * dst->height*src_pixel_size;
	memcpy(dst->buf_ptr, src->buf_ptr, lcd_size);
}
else
{

    src_line_jump_bytes = src->width * src_pixel_size;
    dst_line_jump_bytes = dst->width * dst_pixel_size;

    src_line_p = src->buf_ptr + src_line_jump_bytes * (y1 + src_layer_offset_y) + (x1 + src_layer_offset_x) * src_pixel_size;
    dst_line_p = dst->buf_ptr + dst_line_jump_bytes * y1 + x1 * dst_pixel_size;

    line_width = x2 - x1 + 1;
    
    /* do layer copy */
    if (src->bits_per_pixel == dst->bits_per_pixel)
    {   /* same pixel format, memcpy line by line */
        line_size = line_width * src_pixel_size;

        for (y = y2 - y1; y >= 0; y--)
        {
            memcpy(dst_line_p, src_line_p, line_size);

            src_line_p += src_line_jump_bytes;
            dst_line_p += dst_line_jump_bytes;            
        }
    }
}

}

