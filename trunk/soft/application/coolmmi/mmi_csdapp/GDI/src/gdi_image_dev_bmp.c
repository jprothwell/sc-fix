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
 *	 gdi_image_bmp.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  device bmp api
 * Author:
 * -------
 * -------
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#include "gdi_internal.h"
#include "gd_primitive.h"
#include "drm_gprot.h"
#if 0
#if defined(CSD_TARGET)
#pragma Otime
#pragma no_check_stack
#endif
#endif

/****************************************************************************
* Define
*****************************************************************************/
#define	DEVICE_BMP_TRANSPARENT_COLOR_16 (0x3412)
#define	DEVICE_BMP_TRANSPARENT_COLOR_24 (0x123456)
#define	DEVICE_BMP_TRANSPARENT_COLOR_32 (0x12345678)
/****************************************************************************
* Device Bitmap
*****************************************************************************/

/**************************************************************

	FUNCTION NAME		: gdi_image_dev_bmp_draw()

  	PURPOSE				: Draw device bitmap

	INPUT PARAMETERS	: S32 offset_x
							  S32 offset_y
							  U8  *dev_bmp_src
							  U16 size

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
GDI_RESULT gdi_image_dev_bmp_draw_handler(U32 flag,U32 frame_pos,S32 x,S32 y,S32 w,S32 h,U8* data_ptr,U32 img_size)
{
	if((flag&GDI_IMAGE_CODEC_FLAG_IS_FILE)==0)
		return gdi_image_dev_bmp_draw(x,y, data_ptr, img_size);
	else
//#ifdef GDI_USING_LAYER
		return gdi_image_device_bmp_draw_file(x, y, (S8 *)data_ptr);
//#else
//		return GDI_FAILED;
//#endif
}
GDI_RESULT gdi_image_dev_bmp_get_dimension_handler(U32 flag,U8* data_ptr,U32 img_size,S32 *width,S32 *height)
{
	if(flag&GDI_IMAGE_CODEC_FLAG_IS_FILE)
	{
		return gdi_image_device_bmp_get_dimension_file((S8*)data_ptr,width,height);
	}
	else
	{
		return gdi_image_dev_bmp_get_dimension(data_ptr, width, height);
	}
}

GDI_RESULT gdi_image_dev_bmp_draw(S32 offset_x, S32 offset_y, U8 *dev_bmp_src, U32 size)
{
		U32			dev_bmp_width;
		U32			dev_bmp_height;
		U32			dev_bmp_depth;
		U32			dev_bmp_row_byte;
		S32			i,j;
		S32			y_jump, x_jump;
		S32			jump_size;
		S32			y_draw_count;
		S32			x_draw_count;
		S32			start, end;
		U8			*dest_ptr;
		U8			*dest_ptr_line_start;
		U8				*dev_bmp_ptr_line_start;
		U8				*dev_bmp_ptr;
		U32			U8_per_pixel;
		gdi_color	pixel_color;
		gd_color_to_rgb_func color_to_rgb;
		gdi_color transparent_color;

		/* TODO: add size chcek */
		
		dev_bmp_width	= dev_bmp_src[0]|(dev_bmp_src[1]<<8)|(dev_bmp_src[2]<<16)|(dev_bmp_src[3]<<24);
		dev_bmp_src		+= 4;
		dev_bmp_height	= dev_bmp_src[0]|(dev_bmp_src[1]<<8)|(dev_bmp_src[2]<<16)|(dev_bmp_src[3]<<24);
		dev_bmp_src		+= 4;
		dev_bmp_depth	= dev_bmp_src[0];
		dev_bmp_src		+= 1;
		dev_bmp_row_byte = dev_bmp_src[0]|(dev_bmp_src[1]<<8)|(dev_bmp_src[2]<<16)|(dev_bmp_src[3]<<24);
		dev_bmp_src		+= 4;

        if (dev_bmp_row_byte * dev_bmp_height + 13 != size)
        {
            return GDI_FAILED;
        }

		U8_per_pixel	= (GDI_LAYER.bits_per_pixel>>3);
		dev_bmp_depth = dev_bmp_depth >>3;

	    if (4 == dev_bmp_depth)
	    {
		    color_to_rgb	= gd_color_to_rgb_32;
		    transparent_color = DEVICE_BMP_TRANSPARENT_COLOR_32;
		}
		else if (3 == dev_bmp_depth)
		{
		    color_to_rgb	= gd_color_to_rgb_24;
		    transparent_color = DEVICE_BMP_TRANSPARENT_COLOR_24;
		}
		else
		{
		    color_to_rgb	= gd_color_to_rgb_16;
				//qiff modify 09.05.13 , for pbm icon display with transprant color
		    transparent_color = (dev_bmp_src[0]<<8)|(dev_bmp_src[1]);//DEVICE_BMP_TRANSPARENT_COLOR_16;
            dev_bmp_depth = 2;
		}

		/* adjust source ptr to the start of drawing point */
		/* jump to start y line */
		y_jump = 0;
		if(offset_y < GDI_LAYER.clipy1)
		{
			y_jump		= GDI_LAYER.clipy1-offset_y;
			dev_bmp_src	+=(y_jump*dev_bmp_width*dev_bmp_depth);
		}

		/* jump to start x */
		x_jump = 0;
		if(offset_x < GDI_LAYER.clipx1)
		{
			x_jump = GDI_LAYER.clipx1-offset_x;
			dev_bmp_src += x_jump*dev_bmp_depth;
		}

		dev_bmp_ptr_line_start = dev_bmp_src;


		/* adjust dest ptr to the start of drawing point */
		/* find start ptr for actual drawing */
		if(offset_x < GDI_LAYER.clipx1)
		{
			jump_size = ((y_jump+offset_y)*GDI_LAYER.width+GDI_LAYER.clipx1)*U8_per_pixel;
			dest_ptr_line_start = (U8*)(GDI_LAYER.buf_ptr+jump_size);
		}
		else
		{
			jump_size = ((y_jump+offset_y)*GDI_LAYER.width+offset_x)*U8_per_pixel;
			dest_ptr_line_start = (U8*)(GDI_LAYER.buf_ptr+jump_size);
		}

		/* find x_draw_count and y_draw_count */
		start	= (offset_x < GDI_LAYER.clipx1) ? GDI_LAYER.clipx1 : offset_x;
		end	= (offset_x+(S32)dev_bmp_width-1 > GDI_LAYER.clipx2) ? GDI_LAYER.clipx2 : offset_x+(S32)dev_bmp_width-1;
		x_draw_count = end-start+1;


		start	= (offset_y < GDI_LAYER.clipy1) ? GDI_LAYER.clipy1 : offset_y;
		end	= (offset_y+(S32)dev_bmp_height-1 > GDI_LAYER.clipy2) ? GDI_LAYER.clipy2 : offset_y+(S32)dev_bmp_height-1;
		y_draw_count = end-start+1;


		for(i = 0; i < y_draw_count; i++)
		{
			dest_ptr	= dest_ptr_line_start+GDI_LAYER.width*i*U8_per_pixel;
			dev_bmp_ptr	= dev_bmp_ptr_line_start+dev_bmp_width*i*dev_bmp_depth;

			for(j = 0; j < x_draw_count; j++)
			{
			    if (4 == dev_bmp_depth)
			    {
				    pixel_color	= (dev_bmp_ptr[0]) | (dev_bmp_ptr[1]<<8)| (dev_bmp_ptr[2]<<16)| (dev_bmp_ptr[3]<<24);
				}
				else if (3 == dev_bmp_depth)
				{
				    pixel_color	= (dev_bmp_ptr[0]) | (dev_bmp_ptr[1]<<8)| (dev_bmp_ptr[2]<<16);
				}
				else
				{
					pixel_color	= (dev_bmp_ptr[0]<< 8) | (dev_bmp_ptr[1]);
				}

				if(pixel_color != transparent_color)
					{
                    U32 alpha_value, red_value, green_value, blue_value;
                    gdi_color pixel_color_to_layer;
                    if (color_to_rgb != gdi_act_color_to_rgb)
                    {
                    color_to_rgb(&alpha_value, &red_value, &green_value, &blue_value, pixel_color);
                    pixel_color_to_layer = gdi_act_color_from_rgb(alpha_value, red_value, green_value, blue_value);
                    }
                    else
                    {
                        pixel_color_to_layer = pixel_color;
                    }
					if(gdi_act_layer_info->source_key_enable== TRUE)
					{
						if(pixel_color_to_layer == gdi_act_layer_info->source_key)
							pixel_color_to_layer = pixel_color_to_layer ^ 1;
					}
						
                    memcpy(dest_ptr,&pixel_color_to_layer,U8_per_pixel);
				}

				dev_bmp_ptr	+= dev_bmp_depth;
				dest_ptr += U8_per_pixel;
					}
		}

	return GDI_SUCCEED;
}


/**************************************************************

	FUNCTION NAME		: gdi_image_dev_bmp_get_dimension()

  	PURPOSE				: get device bitmap dimension

	INPUT PARAMETERS	: U8	*dev_bmp_src

	OUTPUT PARAMETERS	: S32 *width
							  S32 *height

	RETURNS				: void

 

***************************************************************/
GDI_RESULT gdi_image_dev_bmp_get_dimension(U8 *dev_bmp_src, S32 *width, S32 *height)
{
		*width		= dev_bmp_src[0]|(dev_bmp_src[1]<<8)|(dev_bmp_src[2]<<16)|(dev_bmp_src[3]<<24);
		dev_bmp_src	+= 4;
		*height		= dev_bmp_src[0]|(dev_bmp_src[1]<<8)|(dev_bmp_src[2]<<16)|(dev_bmp_src[3]<<24);

	return GDI_SUCCEED;
}

//#ifdef GDI_USING_LAYER
extern U32	gdi_image_decoder_frame_buf[(((GDI_LCD_WIDTH+16)*(GDI_LCD_HEIGHT+16)*GDI_MAINLCD_BIT_PER_PIXEL)>>3)/4];

GDI_RESULT gdi_image_device_bmp_draw_file(S32 offset_x, S32 offset_y, S8 *filename)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_image_device_bmp_draw_file)
		FS_HANDLE pbm_file_handle;
		INT32       read_len;
		U32			dev_bmp_width;
		U32			dev_bmp_height;
		U32			dev_bmp_depth;
		U32			dev_bmp_row_byte;
		gdi_rect_struct		clip;
		GDI_RESULT err;

		pbm_file_handle = MMI_FS_Open((UINT8*)filename, FS_READ_ONLY|FS_OPEN_NO_DIR);

		if (pbm_file_handle < 0)
			GDI_RETURN(GDI_FAILED);

		MMI_FS_Read(pbm_file_handle,&dev_bmp_width,4,(INT32*)&read_len);
		mmi_trace(1, "dev_bmp_width = %x, GDI_DEV_BMP_FILE_MARK = %x ", dev_bmp_width, GDI_DEV_BMP_FILE_MARK);
		if(dev_bmp_width == GDI_DEV_BMP_FILE_MARK)
		{
			MMI_FS_Read(pbm_file_handle,&dev_bmp_width,4,(INT32*)&read_len);
			MMI_FS_Read(pbm_file_handle,&dev_bmp_height,4,(INT32*)&read_len);
			MMI_FS_Read(pbm_file_handle,&dev_bmp_depth,1,(INT32*)&read_len);
			MMI_FS_Read(pbm_file_handle,&dev_bmp_row_byte,4,(INT32*)&read_len);
			MMI_FS_Read(pbm_file_handle,gdi_image_decoder_frame_buf,dev_bmp_row_byte*dev_bmp_height,(INT32*)&read_len);
			dev_bmp_depth = (dev_bmp_depth&0x000000FF);
			clip.x1 = gdi_act_layer->clipx1;
			clip.y1 = gdi_act_layer->clipy1;
			clip.x2 = gdi_act_layer->clipx2;
			clip.y2 = gdi_act_layer->clipy2;
			gdi_2d_memory_blt(	(U8*)gdi_image_decoder_frame_buf,	// src_ptr
						dev_bmp_width,
						0,								// src_offset_x
						0,								// src_offset_y 
						dev_bmp_width,	// src_width 
						dev_bmp_height,	// src_height 
						gdi_act_layer->buf_ptr,			// dest_ptr 
						gdi_act_layer->width,				// dest_pitch 
						offset_x,
						offset_y,
						clip);							// dest_clip 
			err = GDI_SUCCEED;
		}
		else
		{
			err = GDI_FAILED;
		}
		MMI_FS_Close(pbm_file_handle);
		GDI_RETURN(err);
	GDI_EXIT_CRITICAL_SECTION(gdi_image_device_bmp_draw_file)
}
//#endif
GDI_RESULT gdi_image_device_bmp_encode_file(S32 width, S32 height, U8 depth, PU8 buf_ptr, PS8 file_name)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_image_device_bmp_encode_file)
		FS_HANDLE pbm_file_handle;
		INT32 write_len;
		S32 image_width,image_height;
		PU8 image_ptr;
		U8 image_depth;
		S32 image_row_byte;
		S32 ret;
		U32 dev_bmp_mark = GDI_DEV_BMP_FILE_MARK;

		image_width = width;
		image_height = height;
		image_ptr = buf_ptr;

		image_depth = depth;
		image_row_byte = image_depth * image_width;

		pbm_file_handle = MMI_FS_Open((UINT8*)file_name, FS_CREATE_ALWAYS);
		if (pbm_file_handle < 0)
			GDI_RETURN(GDI_FAILED);
		ret = MMI_FS_Write(pbm_file_handle,&dev_bmp_mark,sizeof(dev_bmp_mark),&write_len);
		if (ret < 0)
		{
			MMI_FS_Close(pbm_file_handle);
			GDI_RETURN(GDI_FAILED);
		}
		ret = MMI_FS_Write(pbm_file_handle,&image_width,sizeof(image_width),&write_len);
		if (ret < 0)
		{
			MMI_FS_Close(pbm_file_handle);
			GDI_RETURN(GDI_FAILED);
		}
		ret = MMI_FS_Write(pbm_file_handle,&image_height,sizeof(image_height),&write_len);
		if (ret < 0)
		{
			MMI_FS_Close(pbm_file_handle);
			GDI_RETURN(GDI_FAILED);
		}
		ret = MMI_FS_Write(pbm_file_handle,&image_depth,sizeof(image_depth),&write_len);
		if (ret < 0)
		{
			MMI_FS_Close(pbm_file_handle);
			GDI_RETURN(GDI_FAILED);
		}
		ret = MMI_FS_Write(pbm_file_handle,&image_row_byte,sizeof(image_row_byte),&write_len);
		if (ret < 0)
		{
			MMI_FS_Close(pbm_file_handle);
			GDI_RETURN(GDI_FAILED);
		}
		ret = MMI_FS_Write(pbm_file_handle,image_ptr,image_width*image_height*image_depth,&write_len);
		if (ret < 0)
		{
			MMI_FS_Close(pbm_file_handle);
			GDI_RETURN(GDI_FAILED);
		}
		MMI_FS_Close(pbm_file_handle);
		GDI_RETURN(GDI_SUCCEED);
	GDI_EXIT_CRITICAL_SECTION(gdi_image_device_bmp_encode_file)
}
GDI_RESULT gdi_image_device_bmp_get_dimension_file(S8 *filename, S32 *width, S32 *height)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_image_device_bmp_get_dimension_file)
		FS_HANDLE pbm_file_handle;
		INT32 read_len;
		GDI_RESULT err;
		U32 mark;

		pbm_file_handle = MMI_FS_Open((UINT8*)filename, FS_READ_ONLY|FS_OPEN_NO_DIR);

		if (pbm_file_handle < 0)
			GDI_RETURN(GDI_FAILED);
		
		MMI_FS_Read(pbm_file_handle,&mark,4,&read_len);
		if(mark == GDI_DEV_BMP_FILE_MARK)
		{
			MMI_FS_Read(pbm_file_handle,width,4,&read_len);
			MMI_FS_Read(pbm_file_handle,height,4,&read_len);
			err = GDI_SUCCEED;
		}
		else
			err = GDI_FAILED;
		MMI_FS_Close(pbm_file_handle);
		GDI_RETURN(err);
	GDI_EXIT_CRITICAL_SECTION(gdi_image_device_bmp_get_dimension_file)
}

BOOL gdi_image_is_device_bmp_file(U8 *file_name)
{
	FS_HANDLE f;
	U32 mark;
	INT32 len;
	U32 result;
	BOOL err;
	
	f=DRM_open_file((UINT8*)file_name,FS_READ_ONLY|FS_OPEN_NO_DIR,0);
	if(f<0) return FALSE;

	result = DRM_read_file(f,&mark,sizeof(mark),&len); 

	err = MMI_FALSE;
	if(result == FS_NO_ERROR)
		if(mark == GDI_DEV_BMP_FILE_MARK)
			err = MMI_TRUE;

	DRM_close_file(f);
	return err;
	
}


