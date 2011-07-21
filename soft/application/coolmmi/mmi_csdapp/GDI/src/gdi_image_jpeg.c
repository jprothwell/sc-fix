/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2001
*
*******************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *  gdi_image_jpeg.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI Image Jpeg decoder.
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#include "gdi_features.h"
#include "gdi_internal.h"
/* DRM_REPLACE */
#include "drm_gprot.h"


/* JPEG only support in GDI_MEMORY_PROFILE_2 */
#ifdef GDI_USING_HW_JPEG

#include "gui.h"        /* for timer */
#include "unicodexdcl.h"

#include "med_api.h"
#include "med_main.h"
#include "med_struct.h"

#ifdef GDI_HW_JPEG_SUPPORT_COLOR_FORMAT
#include "imgdma.h"
#endif 

#include "debuginitdef.h"
#include "mmi_trace.h"
#include "di.h"
#ifdef GDI_USING_SW_JPEG
#include "mmi_jpeg_dec.h"
#endif

#if !defined (MMI_ON_WIN32)
#include "hal_timers.h"
#endif   

extern U8 gdi_image_decoder_frame_buf[((GDI_LCD_WIDTH+16)*(GDI_LCD_HEIGHT+16)*GDI_MAINLCD_BIT_PER_PIXEL)>>3];

extern BOOL		gdi_memory_output;
extern U16		gdi_memory_output_width;
extern U16		gdi_memory_output_height;
extern U32 		gdi_memory_output_buffer_address;
extern U32		gdi_memory_output_buffer_size;

#define MAX_IMG_GDI_DEC_FILE_BUFFER_LEN   (200*1024) /* Buffer size for multi-run JPG decode. */

 
#define JPEG_MARKER_SOF0      0xC0
#define JPEG_MARKER_SOF1      0xC1
#define JPEG_MARKER_SOF2      0xC2
#define JPEG_MARKER_SOF3      0xC3
#define JPEG_MARKER_SOF15     0xCF
#define JPEG_MARKER_SOS       0xDA
#define JPEG_MARKER_DQT       0xDB
#define JPEG_MARKER_DNL       0xDC
#define JPEG_MARKER_DRI       0xDD
#define JPEG_MARKER_DHP       0xDE
#define JPEG_MARKER_EXP       0xDF
#define JPEG_MARKER_APP0      0xE0
#define JPEG_MARKER_APP15     0xEF
#define JPEG_MARKER_COM       0xFE

#define ROUND16(x)   (((x) + 15)&(~0xf))


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_get_dimension_internal
 * DESCRIPTION
 *  get jpeg dimension inetrnal function
 * PARAMETERS
 *  width       [OUT]       
 *  height      [OUT]       
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
static GDI_RESULT gdi_image_jpeg_get_dimension_internal(S32 *width, S32 *height)
{
		U32	index,index_offset;
		U16	marker_length;

		U8		jpg_byte1, jpg_byte2, tmp_byte;

		index		= 0;
		jpg_byte1	= gdi_bytestream_get_byte();
		jpg_byte2	= gdi_bytestream_get_byte();

		/* check first two byte to see if is valid jpeg file */
		if ((jpg_byte1==0xFF) && (jpg_byte2==0xD8))
		{

			/* parse bytestream */
			do
			{
				if (gdi_bytestream_is_eof() == TRUE)
					break;

				do
				{
					tmp_byte = gdi_bytestream_get_byte();
				} while (tmp_byte!=0xFF);

				jpg_byte1 = 0xFF;
				jpg_byte2 = gdi_bytestream_get_byte();

				switch (jpg_byte2)
				{
					case JPEG_MARKER_SOF0:
					case JPEG_MARKER_SOF2:

						marker_length =  gdi_bytestream_get_byte()<<8;
						marker_length |= gdi_bytestream_get_byte();
						marker_length -= 2;

						gdi_bytestream_get_byte(); /* precision */
						*height		=	gdi_bytestream_get_byte()<<8;
						*height		|= gdi_bytestream_get_byte();
						*width		=	gdi_bytestream_get_byte()<<8;
						*width		|= gdi_bytestream_get_byte();

						/* set terminate byte */
						jpg_byte2=0xD9;

					break;
                                     
                                    case JPEG_MARKER_SOS:
					break;
					case 0xC8:
					case 0x00:
					break;
					default:
						if ((jpg_byte2==JPEG_MARKER_SOF1) ||
							((jpg_byte2>=JPEG_MARKER_SOF3) && (jpg_byte2<=JPEG_MARKER_SOF15))||
							 (jpg_byte2==JPEG_MARKER_DQT) || (jpg_byte2==JPEG_MARKER_DNL) ||
							 (jpg_byte2==JPEG_MARKER_DRI) || (jpg_byte2==JPEG_MARKER_DHP) ||
							 (jpg_byte2==JPEG_MARKER_EXP) || (jpg_byte2==JPEG_MARKER_COM) ||
							 ((jpg_byte2>=JPEG_MARKER_APP0) && (jpg_byte2<=JPEG_MARKER_APP15)))
						{
							index_offset =	 gdi_bytestream_get_byte()<<8;
							index_offset |= gdi_bytestream_get_byte();
							index_offset -= 2;
							gdi_bytestream_flush(index_offset);
						}
					break;
				}


			} while (!((jpg_byte1==0xFF) && (jpg_byte2==0xD9)));

		return GDI_SUCCEED;
		}
		else /* not valid jpeg file */
		{
		return GDI_IMAGE_DECODER_ERR_INVALID_IMG_TYPE;
		}

}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_get_dimension
 * DESCRIPTION
 *  get jpeg dimension, source is from memory
 * PARAMETERS
 *  jpeg_src        [IN]        
 *  size            [IN]        
 *  width           [OUT]       
 *  height          [OUT]       
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
 extern S32 UI_device_width	;
extern S32 UI_device_height;

GDI_RESULT gdi_image_jpeg_get_dimension(U8 *jpeg_src, U32 size, S32 *width, S32 *height)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_image_jpeg_get_dimension)
		GDI_RESULT ret;

		*width	= 0;
		*height	= 0;

		if(jpeg_src==NULL || gdi_bytestream_create((U8 *)jpeg_src, size) != GDI_SUCCEED)
			GDI_RETURN(GDI_IMAGE_DECODER_ERR_OPEN_FILE_FAILED);
#if 0//#ifdef GDI_USING_SW_JPEG
    *width = UI_device_width ;
    *height = UI_device_height;
    ret = GDI_SUCCEED;
#else

		GDI_TRY
		{
			ret = gdi_image_jpeg_get_dimension_internal(width, height);
		}
#endif
		GDI_CATCH_BEGIN
		{
			*width	= 0;
			*height	= 0;
			ret = GDI_IMAGE_DECODER_ERR_INVALID_IMG_TYPE;
		}
		GDI_CATCH_END

		gdi_bytestream_free();

		GDI_RETURN(ret);
	GDI_EXIT_CRITICAL_SECTION(gdi_image_jpeg_get_dimension)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_get_dimension_file
 * DESCRIPTION
 *  get jpeg dimension, source is from file
 * PARAMETERS
 *  image_name      [IN]        
 *  width           [OUT]       
 *  height          [OUT]       
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_get_dimension_file(S8 *image_name, S32 *width, S32 *height)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_image_jpeg_get_dimension_file)
		GDI_RESULT ret;

		*width	= 0;
		*height	= 0;

		if(image_name==NULL || gdi_bytestream_create_file(image_name) != GDI_SUCCEED)
			GDI_RETURN(GDI_FAILED);

		GDI_TRY
//#ifdef GDI_USING_SW_JPEG
#if 0
    *width = UI_device_width;
    *height = UI_device_height;
    ret = GDI_SUCCEED;
#else
		{
			ret = gdi_image_jpeg_get_dimension_internal(width, height);
		}
#endif
		GDI_CATCH_BEGIN
		{
			*width	= 0;
			*height	= 0;
			ret = GDI_IMAGE_DECODER_ERR_INVALID_IMG_TYPE;
		}
		GDI_CATCH_END

		gdi_bytestream_free();

		GDI_RETURN(ret);
	GDI_EXIT_CRITICAL_SECTION(gdi_image_jpeg_get_dimension_file)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_draw_to_buffer
 * DESCRIPTION
 *  draw resized jpeg
 * PARAMETERS
 *  width                   [IN]        
 *  height                  [IN]        
 *  jpeg_src                [IN]        
 *  size                    [IN]        
 *  buf                     [?]         
 *  buf_size                [IN]        
 *  decoded_image_width     [?]         
 *  oy(?)                   [IN]        
 *  resized_width(?)        [IN]        
 *  resized_height(?)       [IN]        
 *  ox(?)                   [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_draw_to_buffer(
            S32 width,
            S32 height,
            U8 *jpeg_src,
            U32 size,
            U8 *buf,
            S32 buf_size,
            S32 *decoded_image_width)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_image_jpeg_draw_to_buffer)
	//#ifndef WIN32
		S32					ret;
		media_img_decode_req_struct img_decode_data;

		img_decode_data.display_width			= (kal_uint16)width;
		img_decode_data.display_height		= (kal_uint16)height;
		img_decode_data.image_buffer_p		= (void*)buf;
		img_decode_data.image_buffer_size	= (kal_uint32)buf_size;
		img_decode_data.media_mode				= (kal_uint16)MED_MODE_ARRAY;
		img_decode_data.media_type				= (kal_uint16)MED_TYPE_JPG;
		img_decode_data.data						= (void*)jpeg_src;
		img_decode_data.file_size				= (kal_uint32)size;
		img_decode_data.seq_num					= (kal_uint16)0;
		img_decode_data.blocking				= (kal_bool)TRUE;	/* blocking */
		img_decode_data.image_clip_x1			=0;
		img_decode_data.image_clip_y1			=0;
		img_decode_data.image_clip_x2			=width-1;
		img_decode_data.image_clip_y2			=height-1;
		img_decode_data.callback				= NULL;

		ret = media_img_decode(MOD_MMI, &img_decode_data);

		if(ret != MED_RES_OK)
		{
			if(ret == MED_RES_MEM_INSUFFICIENT || ret == MED_RES_BUFFER_INSUFFICIENT)
				GDI_RETURN(GDI_JPEG_ERR_IMAGE_TOO_LARGE);
			else if(ret == MED_RES_OPEN_FILE_FAIL || ret == MED_RES_INVALID_HANDLE)
				GDI_RETURN(GDI_JPEG_ERR_FILE_FORMAT_WRONG);
			else
				GDI_RETURN(GDI_FAILED);
		}

		*decoded_image_width = img_decode_data.decoded_image_width;
		GDI_RETURN(GDI_SUCCEED);
	GDI_EXIT_CRITICAL_SECTION(gdi_image_jpeg_draw_to_buffer)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_draw_file_to_buffer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  width                   [IN]        
 *  height                  [IN]        
 *  jpeg_file               [?]         
 *  buf                     [?]         
 *  buf_size                [IN]        
 *  decoded_image_width     [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_draw_file_to_buffer(
            S32 width,
            S32 height,
            U8 *jpeg_file,
            U8 *buf,
            S32 buf_size,
            S32 *decoded_image_width)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_jpeg_draw_to_buffer)
    GDI_RETURN(gdi_image_jpeg_draw_internal(
                0,
                0,
                width,
                height,
                -1,
                -1,
                -1,
                -1,
                jpeg_file,
                0,
                TRUE,
                TRUE,
                TRUE,
                buf,
                buf_size,
                decoded_image_width,
                GDI_COLOR_FORMAT_16));
    GDI_EXIT_CRITICAL_SECTION(gdi_image_jpeg_draw_to_buffer)
}

#ifdef JPG_DECODE
extern U16			simulator_Merge_buffer[GDI_LCD_WIDTH * GDI_LCD_HEIGHT];
kal_int32 image_decoder_jpeg_req(media_img_decode_req_struct *req_p)
{
   return 0;
}
#endif


GDI_RESULT gdi_image_jpeg_draw_screen(S32 src_x,S32 src_y, S32 clip_x1,S32 clip_x2,S32 clip_y1,S32 clip_y2,S32 show_width,S32 show_height)
{
	gdi_rect_struct		clip;
	clip.x1 = clip_x1;
	clip.y1 = clip_x2;
	clip.x2 = clip_y1;
	clip.y2 = clip_y2;
	
	mmi_trace(1," gdi_image_jpeg_draw_screen");
	#if 1
	gdi_2d_memory_blt(	(U8*)gdi_image_decoder_frame_buf,	// src_ptr
						show_width,
						src_x,								// src_offset_x
						 src_y,								// src_offset_y 
						show_width,	// src_width 
						show_height,	// src_height 
						gdi_act_layer->buf_ptr,			// dest_ptr 
						gdi_act_layer->width,				// dest_pitch 
						clip.x1,
						clip.y1,
						clip);							// dest_clip 
#else
		clip.x1 = gdi_act_layer->clipx1;
		clip.y1 = gdi_act_layer->clipy1;
		clip.x2 = gdi_act_layer->clipx2;
		clip.y2 = gdi_act_layer->clipy2;
		
			gdi_2d_memory_blt(	(U8*)gdi_image_decoder_frame_buf,	// src_ptr
									decUnit.width,
									0,								// src_offset_x
									0,								// src_offset_y 
									160,	// src_width 
									120,	// src_height 
									gdi_act_layer->buf_ptr,			// dest_ptr 
									gdi_act_layer->width,				// dest_pitch 
									clip.x1,
									clip.y1,
									clip);							// dest_clip 		
#endif
GDI_RETURN(  GDI_SUCCEED );	
//GDI_EXIT_CRITICAL_SECTION(gdi_image_jpeg_get_dimension)
}

/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_draw_internal
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ox                      [IN]        
 *  oy                      [IN]        
 *  resized_width           [IN]        
 *  resized_height          [IN]        
 *  output_clipx1           [IN]        
 *  output_clipy1           [IN]        
 *  output_clipx2           [IN]        
 *  output_clipy2           [IN]        
 *  src                     [?]         
 *  size                    [IN]        
 *  is_file                 [IN]        
 *  is_resized              [IN]        
 *  dest_buf                [?]         
 *  dest_buf_size           [IN]        
 *  decoded_image_width     [?]         
 *  dest_buf_cf             [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_draw_internal(
            S32 ox,
            S32 oy,
            S32 resized_width,
            S32 resized_height,
            S32 output_clipx1,
            S32 output_clipy1,
            S32 output_clipx2,
            S32 output_clipy2,
            U8 *src,
            U32 size,
            BOOL is_file,
            BOOL is_resized,
            BOOL retry_thumbnail,
            U8 *dest_buf,
            S32 dest_buf_size,
            S32 *decoded_image_width,
            gdi_color_format dest_buf_cf)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_image_jpeg_draw_internal)
	/*#ifndef WIN32*/
		
		GDI_RESULT ret=MED_RES_OK;
		gdi_rect_struct		clip;
              DEC_UNIT  decUnit;
			  uint32  c1;
		FS_HANDLE input_file = -1;
		if (is_resized && (resized_width  == 0 || resized_height==0))
		{
			GDI_RETURN(GDI_SUCCEED);
		}
		
		if(!is_resized && resized_width  == 0 && resized_height==0)
		{
			if(is_file)
			{
				if(gdi_image_jpeg_get_dimension_file((S8 *)src, &resized_width, &resized_height)!=GDI_SUCCEED)
					GDI_RETURN(GDI_FAILED);
			}
			else
			{
				if(gdi_image_jpeg_get_dimension(src, size,&resized_width, &resized_height)!=GDI_SUCCEED)
					GDI_RETURN(GDI_FAILED);
			}
		}

   //     mmi_trace(g_sw_CAMERA, " resized_width = %d, resized_height = %d", resized_width , resized_height);
		
		if(output_clipx1==-1 && output_clipy1==-1 && output_clipx2==-1 && output_clipy2==-1)
		{
			output_clipx1 = 0;
			output_clipy1 = 0;
			output_clipx2 = resized_width-1;
			output_clipy2 = resized_height-1;
		}


		ox += output_clipx1;
		oy += output_clipy1;
		
	
	//	mmi_trace(g_sw_CAMERA, "MMI::Begin media_img_decode");
		//ret =  Image_DI->img_jpegdecode(src,gdi_image_decoder_frame_buf,0,resized_width,resized_height);
        //ret = MMF_JpegDecodePicture(src,gdi_image_decoder_frame_buf,sizeof(gdi_image_decoder_frame_buf),resized_width,resized_height,&decUnit,is_file);
	    if(is_file)
	    {
			input_file = MMI_FS_Open((U8*)src, FS_READ_ONLY);
			if (input_file < 0)
				GDI_RETURN( GDI_FAILED );
	    }
 	   else
 	  {
	 	   input_file = -1;
 	  }
            
	   GDI_TRY
	    {
	    c1 = hal_TimGetUpTime();
	    if(is_file)
	    {
			ret = MMF_JpegDecodePicture(input_file,gdi_image_decoder_frame_buf,sizeof(gdi_image_decoder_frame_buf),
										resized_width,resized_height,
										0,0,
										resized_width-1,resized_height-1,
										&decUnit,is_file,0);
	    }
	    else
	    {
			ret = MMF_JpegDecodePicture((long)src,gdi_image_decoder_frame_buf,sizeof(gdi_image_decoder_frame_buf),
										resized_width,resized_height,
										0,0,
										resized_width-1,resized_height-1,
										&decUnit,is_file,0);
	    }
      //      mmi_trace(1,"*****JPEG DECODER OK ,RET=%d  consume time=%d*******",ret,hal_TimGetUpTime()-c1);
	    }
	    GDI_CATCH_BEGIN
	    {
	       mmi_trace(1,"*****JPEG DECODER ERROR HAPPEND*******");
	        ret = GDI_FAILED;
	    }
	    GDI_CATCH_END 
			
//	       mmi_trace(1,"chenhe,jpeg close input_file is %d,is_file is %d",input_file,is_file);
		 if(is_file)
	 	{
			MMI_FS_Close(input_file);
	 	}
		ox = ox + (resized_width - decUnit.width)/2;
		oy = oy + (resized_height - decUnit.height)/2;
		resized_width = decUnit.width;
		resized_height = decUnit.height;
		output_clipx1 = 0;
		output_clipy1 = 0;
		output_clipx2 = resized_width-1;
		output_clipy2 = resized_height-1;
//		mmi_trace(g_sw_CAMERA, "MMI::Dsiplay and put hex, resized_width = %d, resized_height = %d", resized_width, resized_height);
//		mmi_trace(g_sw_CAMERA, "MMI::Dsiplay and put hex, output_clipx1 = %d, output_clipy1 = %d", output_clipx1, output_clipy1);
//		mmi_trace(g_sw_CAMERA, "MMI::Finish media_img_decode  media_img_decode ret = %d", ret);
             

		if(ret != MED_RES_OK)
		{
			if(ret == MED_RES_MEM_INSUFFICIENT || ret == MED_RES_BUFFER_INSUFFICIENT)
				GDI_RETURN( GDI_JPEG_ERR_IMAGE_TOO_LARGE);
			else if(ret == MED_RES_OPEN_FILE_FAIL || ret == MED_RES_INVALID_HANDLE)
				GDI_RETURN( GDI_JPEG_ERR_FILE_FORMAT_WRONG);
			else if(ret == MED_RES_NOT_ENOUGH_SPACE || ret == MED_RES_END_OF_FILE)
			  	GDI_RETURN( GDI_JPEG_ERR_FRAME_BUFFER_NOT_ENOUGH);
			else
				GDI_RETURN( GDI_FAILED );
		}
		#ifdef GDI_HW_JPEG_SUPPORT_DIRECT_OUTPUT_BUFFER		
		else  // the result already output to layer buffer
		{
			GDI_RETURN(GDI_SUCCEED);
		}
		#endif

		clip.x1 = gdi_act_layer->clipx1;
		clip.y1 = gdi_act_layer->clipy1;
		clip.x2 = gdi_act_layer->clipx2;
		clip.y2 = gdi_act_layer->clipy2;
        //mmi_trace(1,"x1=%d,y1=%d,x2=%d,y2=%d gdi_act_layer->width=%d",clip.x1,clip.y1,clip.x2,clip.y2,gdi_act_layer->width);
		
		/* return ok, copy from intermeidate buffer to destination buffer */
		{
		gdi_2d_memory_blt(	(U8*)gdi_image_decoder_frame_buf,	// src_ptr
            						resized_width,
							
									0,								// src_offset_x
									0,								// src_offset_y 
						
									resized_width,	// src_width 
									resized_height,	// src_height 
									gdi_act_layer->buf_ptr,			// dest_ptr 
									gdi_act_layer->width,				// dest_pitch 
									ox,
									oy,
									clip);							// dest_clip 
	}
	/*#endif //!WIN32*/
	
	GDI_RETURN(  GDI_SUCCEED );
	GDI_EXIT_CRITICAL_SECTION(gdi_image_jpeg_draw_internal)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_get_exif_info
 * DESCRIPTION
 *  
 * PARAMETERS
 *  filename        [?]     
 *  info            [?]     
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_get_exif_info(S8 *filename, gdi_image_jpeg_exif_info_struct *info)
{
/*wuzc Del Start For GDI/GUI Portting Ver:    on 2008-3-11 11:33 */
    #if 0
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_img_exif_info_struct minfo;
    S32 ret;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    minfo.filename = filename;
    ret = media_img_exif_decode(MOD_MMI, &minfo);
    PRINT_INFORMATION_2((MMI_TRACE_G8_MEDIA, "*---[GDI_IMAGE_JPEG.c] filename = %s---*\n", filename));
    memset(info, 0, sizeof(*info));

    if (ret != MED_RES_OK)
    {
        return GDI_FAILED;
    }
    else
    {
#define FIELD_COPY(FIELD) memcpy(info->FIELD,minfo.FIELD,sizeof(info->FIELD));                    \
      PRINT_INFORMATION_2 ((MMI_TRACE_G8_MEDIA, "*---[GDI_IMAGE_JPEG.c] %s---*\n",(char*)(info->FIELD)))\

        FIELD_COPY(make);
        FIELD_COPY(model);
        FIELD_COPY(orientation);
        FIELD_COPY(x_resolution);
        FIELD_COPY(y_resolution);
        FIELD_COPY(resolution_unit);
        FIELD_COPY(software);
        FIELD_COPY(date_time);
        FIELD_COPY(ycbcr_positioning);
        FIELD_COPY(exif_ifd);

        FIELD_COPY(exposure_time);
        FIELD_COPY(fnumber);
        FIELD_COPY(exposure_program);
        FIELD_COPY(iso_speed_ratings);
        FIELD_COPY(exif_version);
        FIELD_COPY(date_time_original);
        FIELD_COPY(date_time_digitized);
        FIELD_COPY(components_config);
        FIELD_COPY(exposure_bias_value);
        FIELD_COPY(metering_mode);
        FIELD_COPY(light_source);
        FIELD_COPY(flash);
        FIELD_COPY(maker_note);
        FIELD_COPY(flashpix_version);
        FIELD_COPY(color_space);
        FIELD_COPY(pixel_x_dimension);
        FIELD_COPY(pixel_y_dimension);
        FIELD_COPY(exposure_mode);
        FIELD_COPY(digital_zoom_ratio);
        FIELD_COPY(scene_capture_type);
#undef FIELD_COPY
        return GDI_SUCCEED;
    }
    #else
	return GDI_FAILED;
    #endif
/*wuzc Del End  For GDI/GUI Portting Ver:    on 2008-3-11 11:33 */
}

#elif defined(GDI_USING_W32_JPEG)
extern int w32_image_get_dimension(int type, char *src, int size, int *width, int *height);
extern char *w32_image_get_data(
                int type,
                char *src,
                int size,
                int bits_per_pixel,
                int resized_width,
                int resized_height,
                int *width,
                int *height,
                int *pitch);
extern void w32_image_free_data(void);


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_get_dimension
 * DESCRIPTION
 *  
 * PARAMETERS
 *  jpeg_src        [?]         
 *  size            [IN]        
 *  width           [?]         
 *  height          [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_get_dimension(U8 *jpeg_src, U32 size, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_jpeg_get_dimension)
    if (w32_image_get_dimension(GDI_IMAGE_TYPE_JPG, (char*)jpeg_src, size, width, height))
    {
        GDI_RETURN(GDI_SUCCEED);
    }
    else
    {
        GDI_RETURN(GDI_FAILED);
    }
    GDI_EXIT_CRITICAL_SECTION(gdi_image_jpeg_get_dimension)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_get_dimension_file
 * DESCRIPTION
 *  
 * PARAMETERS
 *  image_name      [?]     
 *  width           [?]     
 *  height          [?]     
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_get_dimension_file(S8 *image_name, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_jpeg_get_dimension_file)
    if (w32_image_get_dimension(GDI_IMAGE_TYPE_JPG, (char*)image_name, 0, width, height))
    {
        GDI_RETURN(GDI_SUCCEED);
    }
    else
    {
        GDI_RETURN(GDI_FAILED);
    }
    GDI_EXIT_CRITICAL_SECTION(gdi_image_jpeg_get_dimension_file)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_draw_to_buffer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  width                   [IN]        
 *  height                  [IN]        
 *  jpeg_src                [?]         
 *  size                    [IN]        
 *  buf                     [?]         
 *  buf_size                [IN]        
 *  decoded_image_width     [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_draw_to_buffer(
            S32 width,
            S32 height,
            U8 *jpeg_src,
            U32 size,
            U8 *buf,
            S32 buf_size,
            S32 *decoded_image_width)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_jpeg_draw_to_buffer)
    GDI_RETURN(gdi_image_jpeg_draw_internal(
                0,
                0,
                width,
                height,
                -1,
                -1,
                -1,
                -1,
                jpeg_src,
                size,
                FALSE,
                TRUE,
                TRUE,
                buf,
                buf_size,
                decoded_image_width,
                GDI_COLOR_FORMAT_16));
    GDI_EXIT_CRITICAL_SECTION(gdi_image_jpeg_draw_to_buffer)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_draw_file_to_buffer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  width                   [IN]        
 *  height                  [IN]        
 *  jpeg_file               [?]         
 *  buf                     [?]         
 *  buf_size                [IN]        
 *  decoded_image_width     [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_draw_file_to_buffer(
            S32 width,
            S32 height,
            U8 *jpeg_file,
            U8 *buf,
            S32 buf_size,
            S32 *decoded_image_width)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_jpeg_draw_to_buffer)
    GDI_RETURN(gdi_image_jpeg_draw_internal(
                0,
                0,
                width,
                height,
                -1,
                -1,
                -1,
                -1,
                jpeg_file,
                0,
                TRUE,
                TRUE,
                TRUE,
                buf,
                buf_size,
                decoded_image_width,
                GDI_COLOR_FORMAT_16));
    GDI_EXIT_CRITICAL_SECTION(gdi_image_jpeg_draw_to_buffer)
}

/* /TO DO : Need to modify this for gdi_image_jpeg_draw_to_buffer and gdi_image_jpeg_draw_file_to_buffer */


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_draw_internal
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ox                      [IN]        
 *  oy                      [IN]        
 *  resized_width           [IN]        
 *  resized_height          [IN]        
 *  output_clipx1           [IN]        
 *  output_clipy1           [IN]        
 *  output_clipx2           [IN]        
 *  output_clipy2           [IN]        
 *  src                     [?]         
 *  size                    [IN]        
 *  is_file                 [IN]        
 *  is_resized              [IN]        
 *  dest_buf                [?]         
 *  dest_buf_size           [IN]        
 *  decoded_image_width     [?]         
 *  dest_buf_cf             [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_draw_internal(
            S32 ox,
            S32 oy,
            S32 resized_width,
            S32 resized_height,
            S32 output_clipx1,
            S32 output_clipy1,
            S32 output_clipx2,
            S32 output_clipy2,
            U8 *src,
            U32 size,
            BOOL is_file,
            BOOL is_resized,
            BOOL retry_thumbnail,
            U8 *dest_buf,
            S32 dest_buf_size,
            S32 *decoded_image_width,
            gdi_color_format dest_buf_cf)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_jpeg_draw_internal)

    U8 *s;
    int width, height, pitch;

    if (!is_resized && resized_width == 0 && resized_height == 0)
    {
        if (is_file)
        {
            if (gdi_image_jpeg_get_dimension_file((S8*) src, &resized_width, &resized_height) != GDI_SUCCEED)
            {
                GDI_RETURN(GDI_FAILED);
            }
        }
        else
        {
            if (gdi_image_jpeg_get_dimension(src, size, &resized_width, &resized_height) != GDI_SUCCEED)
            {
                GDI_RETURN(GDI_FAILED);
            }
        }
    }

    if (output_clipx1 == -1 && output_clipy1 == -1 && output_clipx2 == -1 && output_clipy2 == -1)
    {
        output_clipx1 = 0;
        output_clipy1 = 0;
        output_clipx2 = resized_width - 1;
        output_clipy2 = resized_height - 1;
    }
    /* output clipping should consider the layer clipping area */
    {
        int tx, ty;

        tx = output_clipx1;
        ty = output_clipy1;

        output_clipx1 += ox;
        output_clipy1 += oy;
        output_clipx2 += ox;
        output_clipy2 += oy;
        GDI_CLIP_RECT_TEST(output_clipx1, output_clipy1, output_clipx2, output_clipy2, GDI_RETURN(GDI_SUCCEED));
        output_clipx1 -= ox;
        output_clipy1 -= oy;
        output_clipx2 -= ox;
        output_clipy2 -= oy;

        /* adjust (ox,oy) to correct position, because output clipping window may change. */
        //ox += tx - output_clipx1;
        //oy += ty - output_clipy1;
    }

    if (is_file)
    {
        s = w32_image_get_data(
                GDI_IMAGE_TYPE_JPG,
                (char*)src,
                0,
                gdi_act_layer->bits_per_pixel,
                resized_width,
                resized_height,
                &width,
                &height,
                &pitch);
    }
    else
    {
        s = w32_image_get_data(
                GDI_IMAGE_TYPE_JPG,
                (char*)src,
                size,
                gdi_act_layer->bits_per_pixel,
                resized_width,
                resized_height,
                &width,
                &height,
                &pitch);
    }

    if (s)
    {
        gdi_rect_struct clip;

        if (resized_width == 0 && resized_height == 0)
        {
            resized_width = width;
            resized_height = height;
        }

        clip.x1 = gdi_act_layer->clipx1;
        clip.y1 = gdi_act_layer->clipy1;
        clip.x2 = gdi_act_layer->clipx2;
        clip.y2 = gdi_act_layer->clipy2;
        /* return ok, copy from intermeidate buffer to destination buffer */
        gdi_2d_memory_blt(
            (U8*) s,
            pitch * 8 / GDI_LAYER.bits_per_pixel,
            0,                      /* src_offset_x */
            0,                      /* src_offset_y */
            resized_width,          /* src_width */
            resized_height,         /* src_height */
            gdi_act_layer->buf_ptr, /* dest_ptr */
            gdi_act_layer->width,   /* dest_pitch */
            ox,                     /* dest_offset_x */
            oy,                     /* dest_offset_y */
            clip);                  /* dest_clip */

        w32_image_free_data();
        GDI_RETURN(GDI_SUCCEED);
    }
    else
    {
        GDI_RETURN(GDI_FAILED);
    }
    GDI_EXIT_CRITICAL_SECTION(gdi_image_jpeg_draw_internal)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_get_exif_info
 * DESCRIPTION
 *  
 * PARAMETERS
 *  filename        [?]     
 *  info            [?]     
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_get_exif_info(S8 *filename, gdi_image_jpeg_exif_info_struct *info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return GDI_FAILED;
}
#else /* not support jpeg format */


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_get_dimension
 * DESCRIPTION
 *  
 * PARAMETERS
 *  jpeg_src        [?]         
 *  size            [IN]        
 *  width           [?]         
 *  height          [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_get_dimension(U8 *jpeg_src, U32 size, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return GDI_FAILED;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_get_dimension_file
 * DESCRIPTION
 *  
 * PARAMETERS
 *  image_name      [?]     
 *  width           [?]     
 *  height          [?]     
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_get_dimension_file(S8 *image_name, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return GDI_FAILED;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_draw_to_buffer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  width                   [IN]        
 *  height                  [IN]        
 *  jpeg_src                [?]         
 *  size                    [IN]        
 *  buf                     [?]         
 *  buf_size                [IN]        
 *  decoded_image_width     [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_draw_to_buffer(
            S32 width,
            S32 height,
            U8 *jpeg_src,
            U32 size,
            U8 *buf,
            S32 buf_size,
            S32 *decoded_image_width)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return GDI_FAILED;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_draw_file_to_buffer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  width                   [IN]        
 *  height                  [IN]        
 *  jpeg_file               [?]         
 *  buf                     [?]         
 *  buf_size                [IN]        
 *  decoded_image_width     [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_draw_file_to_buffer(
            S32 width,
            S32 height,
            U8 *jpeg_file,
            U8 *buf,
            S32 buf_size,
            S32 *decoded_image_width)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return GDI_FAILED;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_draw_internal
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ox                      [IN]        
 *  oy                      [IN]        
 *  resized_width           [IN]        
 *  resized_height          [IN]        
 *  output_clipx1           [IN]        
 *  output_clipy1           [IN]        
 *  output_clipx2           [IN]        
 *  output_clipy2           [IN]        
 *  src                     [?]         
 *  size                    [IN]        
 *  is_file                 [IN]        
 *  is_resized              [IN]        
 *  dest_buf                [?]         
 *  dest_buf_size           [IN]        
 *  decoded_image_width     [?]         
 *  dest_buf_cf             [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_draw_internal(
            S32 ox,
            S32 oy,
            S32 resized_width,
            S32 resized_height,
            S32 output_clipx1,
            S32 output_clipy1,
            S32 output_clipx2,
            S32 output_clipy2,
            U8 *src,
            U32 size,
            BOOL is_file,
            BOOL is_resized,
            BOOL retry_thumbnail,
            U8 *dest_buf,
            S32 dest_buf_size,
            S32 *decoded_image_width,
            gdi_color_format dest_buf_cf)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return GDI_FAILED;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_get_exif_info
 * DESCRIPTION
 *  
 * PARAMETERS
 *  filename        [?]     
 *  info            [?]     
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_get_exif_info(S8 *filename, gdi_image_jpeg_exif_info_struct *info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return GDI_FAILED;
}

#endif 


/*****************************************************************************
 * FUNCTION
 *  gdi_image_is_jpeg_file
 * DESCRIPTION
 *  
 * PARAMETERS
 *  file_name       [?]     
 * RETURNS
 *  
 *****************************************************************************/
BOOL gdi_image_is_jpeg_file(U8 *file_name)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FS_HANDLE f;
    U8 buf[3];
    INT32 len;
    U32 result;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    f = DRM_open_file((U8*) file_name, FS_READ_ONLY | FS_OPEN_NO_DIR, 0);
    if (f < 0)
    {
        return FALSE;
    }

    result = DRM_read_file(f, buf, 3, &len);

    DRM_close_file(f);

    if (result == FS_NO_ERROR)
        if (len == 3)
            if (buf[0] == 0xff && buf[1] == 0xd8 && buf[2] == 0xff)
            {
                return TRUE;
            }
    return FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_draw_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  flag            [IN]        
 *  frame_pos       [IN]        
 *  x               [IN]        
 *  y               [IN]        
 *  w               [IN]        
 *  h               [IN]        
 *  data_ptr        [?]         
 *  img_size        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_draw_handler(U32 flag, U32 frame_pos, S32 x, S32 y, S32 w, S32 h, U8 *data_ptr, U32 img_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_jpeg_draw_internal(
            x,
            y,
            w,
            h,
            -1,
            -1,
            -1,
            -1,
            data_ptr,
            img_size,
            TOBOOL(flag & GDI_IMAGE_CODEC_FLAG_IS_FILE),
            (BOOL) (w && h),
            TRUE,
            NULL,
            0,
            NULL,
            0);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_jpeg_get_dimension_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  flag            [IN]        
 *  data_ptr        [?]         
 *  img_size        [IN]        
 *  width           [?]         
 *  height          [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_jpeg_get_dimension_handler(U32 flag, U8 *data_ptr, U32 img_size, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (flag & GDI_IMAGE_CODEC_FLAG_IS_FILE)
    {
        return gdi_image_jpeg_get_dimension_file((S8*) data_ptr, width, height);
    }
    else
    {
        return gdi_image_jpeg_get_dimension(data_ptr, img_size, width, height);
    }
}

