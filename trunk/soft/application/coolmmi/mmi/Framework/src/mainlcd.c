 
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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *  mainlcd.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  define LCD and sub-LCD routines.
 *
 * Author:
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/Framework/src/mainlcd.c-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:57:50   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:38:18   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:24:26   zhangxb
 * Initial revision.
 * 
 *    Rev 1.2   Sep 19 2006 18:59:18   zhoumn
 * No change.
 * 
 *    Rev 1.1   Aug 28 2006 17:19:46   zhangxb
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _MAINLCD_C
#define _MAINLCD_C

#ifdef MMI_ON_WIN32
#include "mmi_features.h"
#include "gdi_features.h"
#include "kal_non_specific_general_types.h"
#include "gui.h"

/* target    : custom/drv/LCD/XXXXLCM/lcd_sw_inc.h */
/* simulator : plutommi/mmi/DrvSim/DrvSimInc/lcd_sw_inc.h */
#include "lcd_sw.h"

/* interface/Include/lcd_if.h */
#include "lcd_if.h"			
#include "gdi_include.h"
#include "lcd_sw_rnd.h"

/***************************************************************************** 
* Define
*****************************************************************************/

#define SIMULATOR_LAYER_COUNT			(4)

#define	BUF_PIXEL_DATA(buff_ptr,x , y, width)	(*(buff_ptr + ((y) * width + (x))))
#define	RGB_TO_HW(R, G, B)							((((B) & 0xf8) >> 3) | (((G) & 0xfc) << 3) | (((R) & 0xf8) << 8))
#define	HW_TO_RGB_R(VALUE)							((U8)(((VALUE) & 0xf800) >> 8))
#define	HW_TO_RGB_G(VALUE)							((U8)(((VALUE) & 0x07e0) >> 3))
#define	HW_TO_RGB_B(VALUE)							((U8)(((VALUE) & 0x001f) << 3))
//#define  DRV_HW_TO_RGB_R(VALUE)  ((U8)(((VALUE) & 0xf800) >> 8))
//#define  DRV_HW_TO_RGB_G(VALUE)  ((U8)(((VALUE) & 0x07e0) >> 3))
//#define  DRV_HW_TO_RGB_B(VALUE)  ((U8)(((VALUE) & 0x001f) << 3))

#define	LCD_LAYER0_ENABLE		0x80000000
#define	LCD_LAYER1_ENABLE		0x40000000
#define	LCD_LAYER2_ENABLE		0x20000000
#define	LCD_LAYER3_ENABLE		0x10000000


/***************************************************************************** 
* Typedef 
*****************************************************************************/
/* it must be identical to what was defined in MNT_UI.h */
typedef struct {
	unsigned int	layer_enable;
	unsigned int	frame_buffer_address;

	int				text_clip_left;
	int				text_clip_top;
	int				text_clip_right;
	int				text_clip_bottom;

	int				gfx_clip_left;
	int				gfx_clip_top;
	int				gfx_clip_right;
	int				gfx_clip_bottom;

	int				active_layer;

	unsigned short	layer_x_offset;
	unsigned short	layer_y_offset;
	unsigned short	layer_width;
	unsigned short	layer_height;

	unsigned short	roi_x_offset;
	unsigned short	roi_y_offset;
	unsigned short	roi_width;
	unsigned short	roi_height;

	unsigned char	source_key_enable;
	unsigned char	opacity_enable;
	unsigned short	source_key;
	unsigned char	opacity_value;
} MNT_UI_layer_info_struct;


/***************************************************************************** 
* Local Variable
*****************************************************************************/
/* Main LCD simulator shadow buffer */
U16						simulator_shadow_buffer[LCD_WIDTH * LCD_HEIGHT];

/* Watch LCD simulator watch layer buffer, Add by zhuoxz, 2009-9-21 */
U16 SimLayerWatchView[SIMULATOR_LAYER_COUNT][LCD_WIDTH * LCD_HEIGHT];

/* Sub LCD simulator shadow buffer */
#ifdef DUAL_LCD
U16						simulator_sub_shadow_buffer[SUBLCD_WIDTH * SUBLCD_HEIGHT];
#endif

/* Main LCD frame buffer for bitmap */
unsigned int			simulator_frame_buffer[LCD_WIDTH * LCD_HEIGHT];

#ifdef DUAL_LCD
/* Sub LCD frame buffer for bitmap */
unsigned int			sub_simulator_frame_buffer[SUBLCD_WIDTH * SUBLCD_HEIGHT];
#endif

BITMAPINFO				bitmap_info;
BITMAPINFO				sub_bitmap_info;



#ifdef __MMI_SUBLCD__
	kal_uint8 SubLCDSetArray[]=
		{
			0x01,	/* 0000 0001 */
			0x02,	/* 0000 0010 */
			0x04,	/* 0000 0100 */
			0x08,	/* 0000 1000 */
			0x10,	/* 0001 0000 */
			0x20,	/* 0010 0000 */
			0x40,	/* 0100 0000 */
			0x80	/* 1000 0000 */
		};

	kal_uint8 SubLCDReSetArray[]=
		{
			0xFE,	/* 1111 1110 */
			0xFD,	/* 1111 1101 */
			0xFB,	/* 1111 1011 */
			0xF7,	/* 1111 0111 */
			0xEF,	/* 1110 1111 */
			0xDF,	/* 1101 1111 */
			0xBF,	/* 1011 1111 */
			0x7F	/* 0111 1111 */
		};
#endif

/* main window */
/*	layer_sim_size		layer_sim_main_window; */

/* ROI */
/* layer_sim_offset	layer_sim_ROI_offset; */
/* layer_sim_size		layer_sim_ROI_size; */

/* layers */
lcd_layer_struct	lcd_layer_data[SIMULATOR_LAYER_COUNT];

/* palette look up table */
U16					layer_sim_LUT0[256];
U16					layer_sim_LUT1[256];

BOOL					IsLcdFirstInit = TRUE;


MNT_UI_layer_info_struct		MNT_UI_layer[SIMULATOR_LAYER_COUNT];
MNT_UI_layer_info_struct		**MNT_UI_layer_ptr;
MNT_UI_layer_info_struct		*uil_ptr;

unsigned char						new_application_flag = 0;
S32									active_layer;


/***************************************************************************** 
* Local Function
*****************************************************************************/
UINT32 lcd_get_actual_width();
UINT32 lcd_get_actual_height();

/***************************************************************************** 
* Global Variable
*****************************************************************************/


/***************************************************************************** 
* Global Function
*****************************************************************************/
extern void UpdateScreenFun(unsigned int *simulator_buffer, BITMAPINFO *lpbitmap_info);
void UpdateLayerFun(int nLayer, unsigned int *simulator_buffer, BITMAPINFO *lpbitmap_info);

/*****************************************************************************
* FUNCTION
*	assert_lcd_fb_update
* DESCRIPTION
*	nil
*
* PARAMETERS
*  lcd_id			IN		U8
*	start_x			IN		U16
*	start_y			IN		U16
*	end_x				IN		U16
*	end_y				IN		U16
*	roi_offset_x	IN		U16
*	roi_offset_y	IN		U16
*	update_layer	IN		U32
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void assert_lcd_fb_update(	kal_uint8 	lcd_id, 
									kal_uint16 	start_x, 
									kal_uint16 	start_y, 
									kal_uint16 	end_x,
									kal_uint16 	end_y,
									kal_uint16 	roi_offset_x, 
									kal_uint16 	roi_offset_y, 
									kal_uint32 	update_layer)
{
	
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

} /* end of assert_lcd_fb_update */


/*****************************************************************************
* FUNCTION
*	reset_lcd_if
* DESCRIPTION
*	nil
*
* PARAMETERS
*	nil
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/				   	  				
void reset_lcd_if(void)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

} /* end of reset_lcd_if */


/*****************************************************************************
* FUNCTION
*	config_lcd_layer_window
* DESCRIPTION
*	nil
*
* PARAMETERS
*	lcd_layer	IN		unsigned char
*	layer_data	IN		lcd_layer_struct
* RETURNS
*	TRUE
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
BOOL config_lcd_layer_window(unsigned char lcd_layer, lcd_layer_struct *layer_data)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	lcd_layer_data[lcd_layer] = *layer_data;

	return TRUE;
} /* end of config_lcd_layer_window */


/*****************************************************************************
* FUNCTION
*	lcd_dspl_init
* DESCRIPTION
*	screen initialization
*
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void lcd_dspl_init()
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	int	i;


	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MNT_UI_layer_ptr = (MNT_UI_layer_info_struct **)malloc(SIMULATOR_LAYER_COUNT * sizeof(MNT_UI_layer_info_struct *));

	/* initialize pointer allocation for MNT_UI_layer */
	for (i = 0; i < SIMULATOR_LAYER_COUNT; i++)
	{
		MNT_UI_layer_ptr[i] = &MNT_UI_layer[i];
	}

	/* initialize layout data */

	/* set up the bitmap infomation */
	bitmap_info.bmiHeader.biSize				= sizeof(bitmap_info.bmiHeader);
	bitmap_info.bmiHeader.biWidth				= LCD_WIDTH;
	bitmap_info.bmiHeader.biHeight			= LCD_HEIGHT;
	bitmap_info.bmiHeader.biPlanes			= 1;
	bitmap_info.bmiHeader.biBitCount			= 32;
	bitmap_info.bmiHeader.biCompression		= BI_RGB;
	bitmap_info.bmiHeader.biSizeImage		= LCD_WIDTH * LCD_HEIGHT * 4;
	bitmap_info.bmiHeader.biClrUsed			= 0;
	bitmap_info.bmiHeader.biClrImportant	= 0;

#ifdef __MMI_SUBLCD__
	/* set up the sub-bitmap infomation */
	sub_bitmap_info.bmiHeader.biSize				= sizeof(sub_bitmap_info.bmiHeader);
	sub_bitmap_info.bmiHeader.biWidth			= SUBLCD_WIDTH;
	sub_bitmap_info.bmiHeader.biHeight			= SUBLCD_HEIGHT;
	sub_bitmap_info.bmiHeader.biPlanes			= 1;
	sub_bitmap_info.bmiHeader.biBitCount		= 32;
	sub_bitmap_info.bmiHeader.biCompression	= BI_RGB;
	sub_bitmap_info.bmiHeader.biSizeImage		= SUBLCD_WIDTH * SUBLCD_HEIGHT * 4;
	sub_bitmap_info.bmiHeader.biClrUsed			= 0;
	sub_bitmap_info.bmiHeader.biClrImportant	= 0;
#endif /* __MMI_SUBLCD__ */

	/* draw to simulator's main LCD region */


} /* end of lcd_dspl_init */


/*****************************************************************************
* FUNCTION
*	clear_layer_watch_view
* DESCRIPTION
*	clear the single layer view
*
* PARAMETERS
* nLayer        IN      the layer index
*	pBuffer				IN			the bitmap buffer
* RETURNS
*	void
* GLOBALS AFFECTED
*	xzzhuo,200909-21
*****************************************************************************/
void clear_layer_watch_view(int nLayer, U16* pBuffer)
{
  U32 i,j;

  for (i = 0 ; i < LCD_HEIGHT; i++)
	{
    for (j = 0 ; j < LCD_WIDTH; j++)
    {
      BUF_PIXEL_DATA(pBuffer, j, i, LCD_WIDTH) = 0xffff;
    }
	}
}

/*****************************************************************************
* FUNCTION
*	update_layer_watch_view
* DESCRIPTION
*	set update the single layer view
*
* PARAMETERS
* nLayer        IN      the layer index
*	pBuffer				IN			the bitmap buffer
* RETURNS
*	void
* GLOBALS AFFECTED
*	xzzhuo,200909-21
*****************************************************************************/
void update_layer_watch_view(int nLayer, U16* pBuffer)
{
  U32 i,j;
  color_t c;
  U32 sim_layer_watch_buffer[LCD_WIDTH * LCD_HEIGHT];

  for (i = 0 ; i < LCD_HEIGHT; i++)
	{
    for (j = 0 ; j < LCD_WIDTH; j++)
    {
      c.r		= HW_TO_RGB_R(pBuffer[i * LCD_WIDTH + j]);
      c.g		= HW_TO_RGB_G(pBuffer[i * LCD_WIDTH + j]);
      c.b		= HW_TO_RGB_B(pBuffer[i * LCD_WIDTH + j]);
      c.alpha	= 100;
      
      //SetPixel(hdcMem, j, i ,RGB(c.r, c.g, c.b));
      BUF_PIXEL_DATA(sim_layer_watch_buffer, j, LCD_HEIGHT - 1 - i, LCD_WIDTH) = RGB(c.b, c.g, c.r);
    }
	}
  UpdateLayerFun(nLayer,sim_layer_watch_buffer, &bitmap_info);
}


/*****************************************************************************
* FUNCTION
*	set_global_window_handle
* DESCRIPTION
*	set global the window handle
*
* PARAMETERS
*	hwnd				IN			the input window handle
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void update_screen()
{
	if(simulator_frame_buffer != NULL)
	{
     UpdateScreenFun(simulator_frame_buffer, &bitmap_info);
	}
#ifdef __MMI_SUBLCD__
	
	if (sub_simulator_frame_buffer != NULL)
	{
		UpdateScreenFun(sub_simulator_frame_buffer, sub_bitmap_info);
	}
#endif /* end of __MMI_SUBLCD__ */
} /* end of update_screen */

/*****************************************************************************
* FUNCTION
*	lcd_screen_init
* DESCRIPTION
*	screen initialization
*
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void lcd_screen_init()
{
  
	lcd_dspl_init();



} /* end of lcd_screen_init */




/*****************************************************************************
* FUNCTION
*  win32_ui_set_layer_data
* DESCRIPTION
*  set the flag to indicate that a new application starts
*
* PARAMETERS
*	layer_index			IN		index for the layer
*	lcd_id				IN		lcd id
*	text_clip_left		IN		text clipping region info
*	text_clip_top		IN		text clipping region info
*	text_clip_right	IN		text clipping region info
*	text_clip_bottom	IN		text clipping region info
*	gfx_clip_left		IN		clipping region info
*	gfx_clip_top		IN		clipping region info
*	gfx_clip_right		IN		clipping region info
*	gfx_clip_bottom	IN		clipping region info
*	current_handle		IN		current lcd handle
*	active_handle		IN		active lcd handle
* RETURNS
*  void
* GLOBALS AFFECTED
*  void
*****************************************************************************/
void win32_ui_set_layer_data(
	U8			layer_index,
	U8			lcd_id,
	S32		text_clip_left,
	S32		text_clip_top,
	S32		text_clip_right,
	S32		text_clip_bottom,

	S32		gfx_clip_left,
	S32		gfx_clip_top,
	S32		gfx_clip_right,
	S32		gfx_clip_bottom,

	S32		current_handle,
	S32		active_handle
)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/


	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if (lcd_id == MAIN_LCD)
	{
		/* UIL := UI Layer */
		uil_ptr = MNT_UI_layer_ptr[layer_index];

		uil_ptr->text_clip_left = text_clip_left;
		uil_ptr->text_clip_top = text_clip_top;
		uil_ptr->text_clip_right = text_clip_right;
		uil_ptr->text_clip_bottom = text_clip_bottom;

		uil_ptr->gfx_clip_left = gfx_clip_left;
		uil_ptr->gfx_clip_top = gfx_clip_top;
		uil_ptr->gfx_clip_right = gfx_clip_right;
		uil_ptr->gfx_clip_bottom = gfx_clip_bottom;

		if (current_handle == active_handle)
		{
			active_layer = layer_index;
		}
	}
} /* end of win32_ui_set_layer_data */


/*****************************************************************************
* FUNCTION
*	lcd_fb_update
* DESCRIPTION
*	nil
*
* PARAMETERS
*	lcd_para	IN		lcd_frame_update_struct
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef MMI_ON_HARDWARE_P
extern MMI_BOOL mmi_frm_is_screen_width_height_swapped(void);
#else
MMI_BOOL mmi_frm_is_screen_width_height_swapped(void)
{
	return FALSE;
}
#endif
void lcd_fb_update(lcd_frame_update_struct *lcd_para)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16		x, y,_LCD_W_;
	U16		x_layer, y_layer;
	U16		origin_pixel;
	U16		layer_pixel;
	U16		layer_pixel_index;		/* used for LUT */	
	U8			*index_ptr;					/* used for LUT, get index value */
	U16		*buffer_ptr;
	U16		*shadow_ptr;
	BOOL		source_enable;
	BOOL		opacity_enable;
	BOOL		LUT_enable;
	U32		r, g, b;
	U8			op_value;					/* opacity value*/
	U8			op_value_c;					/* opacity value complement */
	U32		layer_flag_table[4];
	U8			layer_id;
	U8			skip;
	U32		i, j;
	color_t		c;
//	U16		y1, y2, x1, x2;
	S32		roi_width;
	S32		roi_height;

	U8			lcd_id;
	U16		start_x;
	U16		start_y; 
	U16		end_x;
	U16		end_y;
	U16		roi_offset_x; 
	U16		roi_offset_y; 
	DWORD		update_layer;
	U8			is_first_layer;

	lcd_layer_struct *layer_ptr;		/* use this ptr to make code shorter */

	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	if (IsLcdFirstInit != FALSE)
	{
		lcd_screen_init();

		IsLcdFirstInit = FALSE;
	}

	lcd_id			= lcd_para->lcd_id;
	start_x			= lcd_para->lcm_start_x;
	start_y			= lcd_para->lcm_start_y;
	end_x				= lcd_para->lcm_end_x;
	end_y				= lcd_para->lcm_end_y;
	roi_offset_x	= lcd_para->roi_offset_x;
	roi_offset_y	= lcd_para->roi_offset_y;
	update_layer	= lcd_para->update_layer;


	if (lcd_id == MAIN_LCD)
	{
		shadow_ptr = (U16 *)simulator_shadow_buffer;
	}
#ifdef __MMI_SUBLCD__	
	else if (lcd_id == SUB_LCD)
	{
		shadow_ptr = (U16 *)simulator_sub_shadow_buffer;
	}
#endif /* __MMI_SUBLCD__ */

	/* create this table, so we can use for loop to prcess each layer */
	layer_flag_table[0] = LCD_LAYER0_ENABLE;
	layer_flag_table[1] = LCD_LAYER1_ENABLE;
	layer_flag_table[2] = LCD_LAYER2_ENABLE;
	layer_flag_table[3] = LCD_LAYER3_ENABLE;

	roi_width	= end_x - start_x;
	roi_height	= end_y - start_y;
	/* copy base layer to shadow buffer directly for faster speed */
/*	lcd_layer_data[0].offset_x */

/*	
	buffer_ptr = (U16 *)lcd_layer_data[0].frame_buffer_address;
	memcpy(shadow_ptr, buffer_ptr, LCD_WIDTH * LCD_HEIGHT * 2);
*/
	
	is_first_layer = 1;
                if(mmi_frm_is_screen_width_height_swapped())
                    {
                        _LCD_W_ = lcd_get_actual_height();
                }
                else
                    {
                       _LCD_W_ =lcd_get_actual_width();
                }
	for (layer_id = 0 ; layer_id < SIMULATOR_LAYER_COUNT; layer_id++)
	{
		/* set necessary data for MNT UI displaying */
		/* UIL := UI Layer */
		if (lcd_id == MAIN_LCD)
		{
			uil_ptr = MNT_UI_layer_ptr[layer_id];

			uil_ptr->layer_x_offset			= lcd_layer_data[layer_id].x_offset;
			uil_ptr->layer_y_offset			= lcd_layer_data[layer_id].y_offset;
			uil_ptr->layer_width				= lcd_layer_data[layer_id].column_number;
			uil_ptr->layer_height			= lcd_layer_data[layer_id].row_number;

			uil_ptr->roi_x_offset			= roi_offset_x;
			uil_ptr->roi_y_offset			= roi_offset_y;
			uil_ptr->roi_width				= roi_width;
			uil_ptr->roi_height				= roi_height;

			uil_ptr->source_key_enable		= lcd_layer_data[layer_id].source_key_enable;
			uil_ptr->opacity_enable			= lcd_layer_data[layer_id].opacity_enable;
			uil_ptr->source_key				= lcd_layer_data[layer_id].source_key;
			uil_ptr->opacity_value			= lcd_layer_data[layer_id].opacity_value;
			
			uil_ptr->layer_enable			= (update_layer & layer_flag_table[layer_id]);
			uil_ptr->frame_buffer_address	= lcd_layer_data[layer_id].frame_buffer_address;

			uil_ptr->active_layer			= active_layer;
		}

		/* check if this layer need to merge or not */
		if ((update_layer & layer_flag_table[layer_id]) != 0)
		{

			buffer_ptr		= (U16 *)lcd_layer_data[layer_id].frame_buffer_address;
			index_ptr		= (U8 *)lcd_layer_data[layer_id].frame_buffer_address;
			layer_ptr		= &lcd_layer_data[layer_id];
			source_enable	= layer_ptr->source_key_enable;
			opacity_enable	= layer_ptr->opacity_enable;
			LUT_enable		= layer_ptr->color_palette_enable;

			/* for each pixel within region */
			for (y = roi_offset_y ; y <= roi_offset_y+roi_height; y++)
			{
				for (x = roi_offset_x; x <= roi_offset_x+roi_width; x++)
				{
					int x_rotate, y_rotate;
					
					/* transform to layer's local coordinate */
					x_rotate = x-GDI_LAYER_OFFSET;
					y_rotate = y-GDI_LAYER_OFFSET;
					//gdi_rotate_map_lcd_to_layer(gdi_lcd_get_rotate(), &x_rotate, &y_rotate);
					x_layer = x_rotate+GDI_LAYER_OFFSET - layer_ptr->x_offset;
					y_layer = y_rotate+GDI_LAYER_OFFSET - layer_ptr->y_offset;
					/***********************************************************/
					/* test if within region */
					if (	(x_layer >= 0) &&
							(y_layer >= 0) &&
							(x_layer <= layer_ptr->column_number  - 1) &&
							(y_layer <= layer_ptr->row_number - 1) )
					{	
						
						skip = FALSE;
						/* GDI_ASSERT(x - layer_ptr->x_offset>=0); */
								
						/* get original's pixel color */
						if (is_first_layer)
						{
							origin_pixel = 0xffff;
						}
						else
						{
							if (lcd_id == MAIN_LCD)
								origin_pixel = BUF_PIXEL_DATA(shadow_ptr, x- roi_offset_x + start_x, y- roi_offset_y + start_y, _LCD_W_ );		
						#ifdef __MMI_SUBLCD__
							else
								origin_pixel = BUF_PIXEL_DATA(shadow_ptr, x- roi_offset_x + start_x, y- roi_offset_y + start_y, SUBLCD_WIDTH);		
						#endif
						}	
						/* get layer's pixel color, check if it use LUT */
						if (layer_ptr->color_palette_enable == KAL_TRUE)
						{
							/* get index, (U8) */
							layer_pixel_index = BUF_PIXEL_DATA(index_ptr, x_layer, y_layer, layer_ptr->column_number);

							/* ingore this error currently. just set index = 0 */
							if ( (layer_pixel_index < 0) ||  (layer_pixel_index > 255) )
								layer_pixel_index = 0;
							
							/* this might contain error. but ignore. palette must be 0 or 1 */
							if ( layer_ptr->color_palette_select == 0)
								layer_pixel = layer_sim_LUT0[layer_pixel_index];
							else 
								layer_pixel = layer_sim_LUT1[layer_pixel_index];	
						}
						else
						{
							U8* b1 = GDI_LAYER.act_buf_ptr;
							S32 b2 = GDI_LAYER.act_pitch;

							GDI_LAYER.act_buf_ptr = (U8 *)layer_ptr->frame_buffer_address;
							GDI_LAYER.act_pitch = layer_ptr->column_number;

							if (lcd_id == MAIN_LCD)
								DRV_MAINLCD_GET_BUFFER_PIXEL(x_layer, y_layer, layer_pixel);
						#ifdef __MMI_SUBLCD__	
							else
								DRV_SUBLCD_GET_BUFFER_PIXEL(x_layer, y_layer, layer_pixel);
						#endif
							GDI_LAYER.act_buf_ptr = b1;
							GDI_LAYER.act_pitch = b2;
						}
						
						
						/* source key check - tansparency */
						if (source_enable == KAL_TRUE)
						{
							if (layer_ptr->source_key == layer_pixel)
								skip = KAL_TRUE;
						}

                                        // Add by zhuoxz,2009-9-21,Output to layer watch.
                                        if (lcd_id == MAIN_LCD)
                                        {
                                                U32 w,h,tx,ty;
                                                w = lcd_get_actual_width();
                                                h = lcd_get_actual_height();
                                                if(mmi_frm_is_screen_width_height_swapped())
                                                {
                                                    h = lcd_get_actual_width();
                                                    w = lcd_get_actual_height();
                                                }

                                                    switch(gdi_lcd_get_rotate())
                                                    {
                                                        case 0:
                                                        default:
                                                            break;
                                                        case 1:
                                                        tx = lcd_get_actual_width() - y_layer - 1;
                                                        ty = x_layer;
                                                        x_layer = tx;
                                                        y_layer = ty;
                                                            break;
                                                        case 2:
                                                        tx = lcd_get_actual_width() - x_layer - 1;
                                                        ty = lcd_get_actual_height() - y_layer - 1;
                                                        x_layer = tx;
                                                        y_layer = ty;
                                                            break;
                                                        case 3:
                                                        tx = y_layer ;
                                                        ty = lcd_get_actual_height() - x_layer - 1;
                                                        x_layer = tx;
                                                        y_layer = ty;
                                                            break;
                                                    }   

                                                   // y = lcd_get_actual_height() - y - 1;
                                                    x_layer=( x_layer>0?x_layer:0);
                                                    y_layer =( y_layer>0?y_layer:0);
                                                    
                                              if (skip == FALSE)
                                              {
                                                
                                                BUF_PIXEL_DATA(SimLayerWatchView[layer_id], x_layer, y_layer, LCD_WIDTH) = layer_pixel;
                                              }
                                              else
                                              {
                                                BUF_PIXEL_DATA(SimLayerWatchView[layer_id], x_layer, y_layer, LCD_WIDTH) = 0xffff;
                                              }
                                            }

						/* opacity check */
						if (skip == FALSE)
						{
              if (opacity_enable == KAL_TRUE)
							{
								/* calc opacity ratio */
								op_value = layer_ptr->opacity_value << 3;
								op_value_c = ~op_value;
								
								if (op_value!=0)
								{
									if (lcd_id == MAIN_LCD)
									{
										r = HW_TO_RGB_R(origin_pixel) * op_value_c + DRV_HW_TO_RGB_R(layer_pixel) * op_value;
										g = HW_TO_RGB_G(origin_pixel) * op_value_c + DRV_HW_TO_RGB_G(layer_pixel) * op_value;
										b = HW_TO_RGB_B(origin_pixel) * op_value_c + DRV_HW_TO_RGB_B(layer_pixel) * op_value;	
									}
								#ifdef __MMI_SUBLCD__	
									else
									{
										r = HW_TO_RGB_R(origin_pixel) * op_value_c + HW_FORMAT_TO_SUBMMI_R(layer_pixel) * op_value;
										g = HW_TO_RGB_G(origin_pixel) * op_value_c + HW_FORMAT_TO_SUBMMI_G(layer_pixel) * op_value;
										b = HW_TO_RGB_B(origin_pixel) * op_value_c + HW_FORMAT_TO_SUBMMI_B(layer_pixel) * op_value;
									}
								#endif
									r >>= 8;
									g >>= 8;
									b >>= 8;
									origin_pixel = (U16)RGB_TO_HW(r, g, b);
								}
							}
							else
							{
								if (lcd_id == MAIN_LCD)
								{
									r = DRV_HW_TO_RGB_R(layer_pixel);
									g = DRV_HW_TO_RGB_G(layer_pixel);
									b = DRV_HW_TO_RGB_B(layer_pixel);	
								}
							#ifdef __MMI_SUBLCD__	
								else
								{
									r = HW_FORMAT_TO_SUBMMI_R(layer_pixel);
									g = HW_FORMAT_TO_SUBMMI_G(layer_pixel);
									b = HW_FORMAT_TO_SUBMMI_B(layer_pixel);
								}
							#endif
								origin_pixel = (U16)RGB_TO_HW(r, g, b);
							}
						}

						/* write back to shadow buffer */
						if (lcd_id == MAIN_LCD)
							BUF_PIXEL_DATA(shadow_ptr, x - roi_offset_x + start_x, y - roi_offset_y + start_y, _LCD_W_) = origin_pixel;//040105 JF modified
					#ifdef __MMI_SUBLCD__	
						else
							BUF_PIXEL_DATA(shadow_ptr, x - roi_offset_x + start_x, y - roi_offset_y + start_y, SUBLCD_WIDTH) = origin_pixel;//040105 JF modified
					#endif
						/***********************************************************/

					} /* if within region */
				} /* for loop  x */
			} /* for loop - y */
			is_first_layer = 0;
      /* update the watch lyaer view, zhuoxz, 2009-9-21 */
      update_layer_watch_view(layer_id, SimLayerWatchView[layer_id]);
		} /* if flag */
	} /* for each layer */

	/* send the main-lcd information to MNT_UI for displaying */
	if (lcd_id == MAIN_LCD)
	{
		//set_MNT_UI_layer_data(MNT_UI_layer_ptr, LCD_WIDTH, LCD_HEIGHT, SIMULATOR_LAYER_COUNT, &new_application_flag);
	}
	
	/* draw to dc */
	if (lcd_id == MAIN_LCD)
	{	
		/* draw to simulator's main LCD region */

            U32 w,h,x,y,tx,ty;
            w = lcd_get_actual_width();
            h = lcd_get_actual_height();
            if(mmi_frm_is_screen_width_height_swapped())
            {
                    h = lcd_get_actual_width();
                    w = lcd_get_actual_height();
            }
		for (i = 0 ; i < h; i++)
		{
			for (j = 0 ; j < w ; j++)
			{
				c.r		= HW_TO_RGB_R(simulator_shadow_buffer[i * w + j]);
				c.g		= HW_TO_RGB_G(simulator_shadow_buffer[i * w + j]);
				c.b		= HW_TO_RGB_B(simulator_shadow_buffer[i * w + j]);
				c.alpha	= 100;

					//SetPixel(hdcMem, j, i ,RGB(c.r, c.g, c.b));
			     

                           x = j;
                           y = i;
                	      switch(gdi_lcd_get_rotate())
                           {
                                case 0:
                                default:
                                        break;
                                case 1:
                                    tx = lcd_get_actual_width() - y - 1;
                                    ty = x;
                                    x = tx;
                                    y = ty;
                                          break;
                                case 2:
                                    tx = lcd_get_actual_width() - x - 1;
                                    ty = lcd_get_actual_height() - y - 1;
                                    x = tx;
                                    y = ty;
                                           break;
                                case 3:
                                    tx = y ;
                                    ty = lcd_get_actual_height() - x - 1;
                                    x = tx;
                                    y = ty;
                                           break;
                           }   
                          
				y = lcd_get_actual_height() - y - 1;
                            x =( x>0?x:0);
                            y =( y>0?y:0);


                    
				BUF_PIXEL_DATA(simulator_frame_buffer, x, y, lcd_get_actual_width()) = RGB(c.b, c.g, c.r);
			}
		}
    update_screen();



		
	}
#ifdef __MMI_SUBLCD__	
	else if (lcd_id == SUB_LCD)
	{
		/* draw to simulator's sub LCD region */

		y1 = 0;
		y2 = SUBLCD_WIDTH - 1;
		x1 = 0;
		x2 = SUBLCD_HEIGHT - 1;

		for (i = 0 ; i < SUBLCD_HEIGHT; i++)
		{
			for (j = 0 ; j < SUBLCD_WIDTH; j++)
			{
				c.r		= HW_TO_RGB_R(simulator_sub_shadow_buffer[i * SUBLCD_WIDTH + j]);
				c.g		= HW_TO_RGB_G(simulator_sub_shadow_buffer[i * SUBLCD_WIDTH + j]);
				c.b		= HW_TO_RGB_B(simulator_sub_shadow_buffer[i * SUBLCD_WIDTH + j]);
				c.alpha	= 100;

				//SetPixel(sub_hdcMem, j, i ,RGB(c.r, c.g, c.b));
				BUF_PIXEL_DATA(sub_simulator_frame_buffer, j, SUBLCD_HEIGHT - 1 - i, SUBLCD_WIDTH) = RGB(c.b, c.g, c.r);
			}
		}

    update_screen();
	}
#endif /* __MMI_SUBLCD__ */
} /* end of lcd_fb_update */


/*****************************************************************************
* FUNCTION
*	config_lcd_roi_window
* DESCRIPTION
*	nil
*
* PARAMETERS
*	roi_x_offset	IN		U16
*	roi_y_offset	IN		kal_uint16
*	roi_column		IN		kal_uint16
*	roi_row			IN		kal_uint16
* RETURNS
*	TRUE
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
kal_bool config_lcd_roi_window(U16 roi_x_offset, 
							   kal_uint16 roi_y_offset, 
							   kal_uint16 roi_column,
							   kal_uint16 roi_row)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	return	KAL_TRUE;

} /* end of config_lcd_roi_window */


/*****************************************************************************
* FUNCTION
*	lcd_init
* DESCRIPTION
*	nil
*
* PARAMETERS
*	lcd_id				IN		kal_uint8
*	background_color	IN		kal_uint16
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void lcd_init(kal_uint8 lcd_id, kal_uint16 background_color)
{

} /* end of lcd_init */


/*****************************************************************************
* FUNCTION
*	lcd_sleep_in
* DESCRIPTION
*	nil
*
* PARAMETERS
*	lcd_id	IN		kal_uint8
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void lcd_sleep_in(kal_uint8 lcd_id)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

} /* end of lcd_sleep_in */


/*****************************************************************************
* FUNCTION
*	lcd_sleep_out
* DESCRIPTION
*	nil
*
* PARAMETERS
*	lcd_id	IN		kal_uint8
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void lcd_sleep_out(kal_uint8 lcd_id)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

} /* end of lcd_sleep_out */


/*****************************************************************************
* FUNCTION
*	lcd_partial_on
* DESCRIPTION
*	nil
*
* PARAMETERS
*	lcd_id		IN		kal_uint8
*	start_line	IN		kal_uint16
*	end_line		IN		kal_uint16
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void lcd_partial_on(kal_uint8 lcd_id, kal_uint16 start_line, kal_uint16 end_line)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

} /* end of lcd_partial_on */


/*****************************************************************************
* FUNCTION
*	lcd_partial_off
* DESCRIPTION
*	nil
*
* PARAMETERS
*	lcd_id		IN		kal_uint8
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void lcd_partial_off(kal_uint8 lcd_id)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

} /* end of lcd_partial_off */


/*****************************************************************************
* FUNCTION
*	lcd_bright_level
* DESCRIPTION
*	nil
*
* PARAMETERS
*	lcd_id			IN		kal_uint8
*	bright_level	IN		kal_uint8
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void lcd_bright_level(kal_uint8 lcd_id, kal_uint8 bright_level)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

} /* end of lcd_bright_level */


/*****************************************************************************
* FUNCTION
*	lcd_power_on
* DESCRIPTION
*	nil
*
* PARAMETERS
*	lcd_id	IN		kal_uint8
*	on			IN		kal_bool
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void lcd_power_on(kal_uint8 lcd_id, kal_bool on)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

} /* end of lcd_power_on */


/*****************************************************************************
* FUNCTION
*	lcd_screen_on
* DESCRIPTION
*	nil
*
* PARAMETERS
*	lcd_id	IN		kal_uint8
*	on			IN		kal_bool
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void lcd_screen_on(kal_uint8 lcd_id, kal_bool on)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

} /* end of lcd_screen_on */


/*****************************************************************************
* FUNCTION
*	lcd_get_parameter
* DESCRIPTION
*	nil
*
* PARAMETERS
*	lcd_id	IN		kal_uint8
*	type		IN		lcd_func_type
* RETURNS
*	0
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
kal_uint8 lcd_get_parameter(kal_uint8 lcd_id, lcd_func_type type)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	return 0;
} /* end of lcd_get_parameter */


/*****************************************************************************
* FUNCTION
*	lcd_get_size
* DESCRIPTION
*	nil
*
* PARAMETERS
*	lcd_id		IN		kal_uint8
*	lcd_width	IN		kal_uint16 *
*	lcd_height	IN		kal_uint16 *
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void lcd_get_size(kal_uint8 lcd_id, kal_uint16 *lcd_width, kal_uint16 *lcd_height)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

} /* end of lcd_get_size */


/*****************************************************************************
* FUNCTION
*	lcd_set_bias
* DESCRIPTION
*	nil
*
* PARAMETERS
*	lcd_id		IN		kal_uint8
*	bias			IN		kal_uint8 *
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void lcd_set_bias(kal_uint8 lcd_id, kal_uint8 *bias)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

} /* end of lcd_set_bias */


/*****************************************************************************
* FUNCTION
*	lcd_set_contrast
* DESCRIPTION
*	nil
*
* PARAMETERS
*	lcd_id		IN		kal_uint8
*	contrast		IN		kal_uint8 *
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void lcd_set_contrast(kal_uint8 lcd_id, kal_uint8 *contrast)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

} /* end of lcd_set_contrast */


/*****************************************************************************
* FUNCTION
*	lcd_set_linerate
* DESCRIPTION
*	nil
*
* PARAMETERS
*	lcd_id		IN		kal_uint8
*	linereate	IN		kal_uint8 *
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void lcd_set_linerate(kal_uint8 lcd_id, kal_uint8 *linereate)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

} /* end of lcd_set_linerate */


/*****************************************************************************
* FUNCTION
*	lcd_set_temp_compensate
* DESCRIPTION
*	nil
*
* PARAMETERS
*	lcd_id		IN		kal_uint8
*	compensate	IN		kal_uint8 *
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void lcd_set_temp_compensate(kal_uint8 lcd_id, kal_uint8 *compensate)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

} /* end of lcd_set_temp_compensate */


/*****************************************************************************
* FUNCTION
*	lcd_cmd_update
* DESCRIPTION
*	nil
*
* PARAMETERS
*	lcd_id			IN		kal_uint8
*	start_x			IN		kal_uint16
*	start_y			IN		kal_uint16
*	end_x				IN		kal_uint16
*	end_y				IN		kal_uint16
*	roi_offset_x	IN		kal_uint16
*	roi_offset_y	IN		kal_uint16
*	update_layer	IN		kal_uint32
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void lcd_cmd_update(	kal_uint8 lcd_id, 
							kal_uint16 start_x,
							kal_uint16 start_y, 
							kal_uint16 end_x,
							kal_uint16 end_y,
							kal_uint16 roi_offset_x, 
							kal_uint16 roi_offset_y,
							kal_uint32 update_layer)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	
} /* end of lcd_cmd_update */


/*****************************************************************************
* FUNCTION
*  win32_ui_set_new_application_flag
* DESCRIPTION
*  set the flag to indicate that a new application starts
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*  void
*****************************************************************************/
void win32_ui_set_new_application_flag()
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/


	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	new_application_flag = 1;
} /* end of win32_ui_set_new_application_flag */


void set_lcd_color_palette(kal_uint8 color_palette_select,kal_uint32 *color_palette_addr_ptr,
								   kal_uint8 start_index, kal_uint8 number_of_color)
{

}

void config_lcd_output(kal_bool lcd_output)
{
}

void lcd_set_rotate(kal_uint8 lcd_id,kal_uint8 rotate_value)
{
	switch(rotate_value)
    { 
        case LCD_LAYER_ROTATE_270:
		case LCD_LAYER_ROTATE_90:
            bitmap_info.bmiHeader.biWidth			= LCD_HEIGHT;
            bitmap_info.bmiHeader.biHeight			= LCD_WIDTH;
            break;
        case 0:
        default:
            bitmap_info.bmiHeader.biWidth			= LCD_WIDTH;
            bitmap_info.bmiHeader.biHeight			= LCD_HEIGHT;
            break;

    }
}

UINT32 lcd_get_actual_width()
{ 
    return bitmap_info.bmiHeader.biWidth;
}

UINT32 lcd_get_actual_height()
{ 
    return bitmap_info.bmiHeader.biHeight;
}

void lcd_lock_all_blt_operation(void)
{
}

void lcd_unlock_all_blt_operation(void)
{
}

#endif
#endif /* _MAINLCD_C */

