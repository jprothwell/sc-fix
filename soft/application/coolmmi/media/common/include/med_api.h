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
 *	med_api.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes exported apis of media task.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef MED_NOT_PRESENT

#ifndef _MED_API_H
#define _MED_API_H

/*==== CONSTANT ====*/

enum {
   AUD_BLOCK_ALL=0,
   AUD_BLOCK_ALL_EXCEPT_TONE,
   AUD_NO_OF_BLOCK_LEVEL
};

/* MMAPI media type */
enum {
   MMA_TYPE_MIDI=0,
   MMA_TYPE_TONE,
   MMA_TYPE_WAV,
   MMA_TYPE_IMY,
   MMA_TYPE_AMR,
   MMA_TYPE_DAF,
   MMA_TYPE_AAC,
   MMA_TYPE_AU,
   MMA_NO_OF_TYPE
};



typedef struct
{
   kal_uint8   audio_id;
   kal_uint8   play_style;
   kal_uint8   volume;
   kal_uint8   output_path;
   kal_uint16  identifier;
}
aud_play_id_struct;

typedef struct
{
   kal_uint8   src_id;
   kal_uint8   *data_p;
   kal_uint32  len;
   kal_uint8   format;
   kal_uint8   play_style;
   kal_uint8   volume;
   kal_uint8   output_path;
   kal_uint16  identifier;
   kal_uint8   blocking;
}
aud_play_string_struct;

typedef struct
{
   kal_uint8   src_id;
   kal_wchar  *file_name_p;
   kal_uint8   play_style;
   kal_uint8   volume;
   kal_uint8   output_path;
   kal_uint16  identifier;
// set both start_offset and end_offset to 0 for backward compatible
   kal_uint32   start_offset; /* 0 : file begining */
   kal_uint32   end_offset;  /* 0 or file data len-1: file end */
}
aud_play_file_struct;
typedef struct {
    kal_uint16 *text_string;
    kal_uint16 str_type;    /* String Type */
    kal_uint16 lang;        /* Language */
    kal_uint16 app_type;    /* Application Type */
    kal_uint16 gander;      /* Gender */
    kal_uint16 identifier;  /* id for the request */
    kal_uint8 volume;       /* Volume, from 0~6 */
    kal_uint8 path;         /* Audio Path */
    kal_uint8 pitch;        /* Audio Pitch, from 0~100 */
    kal_uint8 speed;        /* Audio Speed, from 0~100 */
} aud_tts_struct;


/************  MMA  ***********/
/* This must be sync with those defined in l1audio.h */
typedef enum {
   MED_SUCCESS,
   MED_FAIL,
   MED_REENTRY,
   MED_NOT_INITIALIZED,
   MED_BAD_FORMAT,
   MED_BAD_PARAMETER,
   MED_BAD_COMMAND,
   MED_NO_HANDLER,
   MED_UNSUPPORTED_CHANNEL,
   MED_UNSUPPORTED_FREQ,
   MED_UNSUPPORTED_TYPE
} med_status_enum;

/* This must be sync with those defined in l1audio.h */
typedef enum {
   MED_NONE,
   MED_DATA_REQUEST,
   MED_DATA_NOTIFICATION,
   MED_END,
   MED_ERROR,
   MED_REPEATED,
   MED_TERMINATED,
   MED_LED_ON,
   MED_LED_OFF,
   MED_VIBRATOR_ON,
   MED_VIBRATOR_OFF,
   MED_BACKLIGHT_ON,
   MED_BACKLIGHT_OFF
} med_event_enum;

typedef  kal_int32 med_handle;
typedef  kal_int32 med_event;
typedef  kal_int32 med_status;
typedef  void (*med_handler)( kal_int32 handle, kal_int32 event );

typedef struct
{
   kal_uint8   *data;
   kal_int32   size;           
   kal_int16   repeats;           
}
mma_player_struct;



#define GET_ARGB_A(_x_) (kal_uint8)((((kal_uint32)_x_)&0xff000000)>>24)
#define GET_ARGB_R(_x_) (kal_uint8)((((kal_uint32)_x_)&0x00ff0000)>>16)
#define GET_ARGB_G(_x_) (kal_uint8)((((kal_uint32)_x_)&0x0000ff00)>>8)
#define GET_ARGB_B(_x_) (kal_uint8)((((kal_uint32)_x_)&0x000000ff))

#define SET_ARGB(_a_,_r_,_g_,_b_) ((((kal_uint32)_a_)<<24)|\
                                    (((kal_uint32)_r_)<<16)|\
                                    (((kal_uint32)_g_)<<8)|\
                                    (((kal_uint32)_b_)))

enum {
   CAM_SRC_ISP=0,
   CAM_SRC_MEM,
   CAM_NO_OF_SRC
};

/* camera param enum */
enum {
   CAM_PARAM_NONE=0,
   CAM_PARAM_ZOOM_FACTOR,
   CAM_PARAM_CONTRAST,
   CAM_PARAM_BRIGHTNESS,
   CAM_PARAM_HUE,
   CAM_PARAM_GAMMA,
   CAM_PARAM_WB,
   CAM_PARAM_EXPOSURE,
   CAM_PARAM_EFFECT,
   CAM_PARAM_BANDING,    //9  //add by WeiD
   CAM_PARAM_SATURATION,
   CAM_PARAM_NIGHT_MODE,
   CAM_PARAM_EV_VALUE,
   CAM_PARAM_FLASH,
   CAM_PARAM_FLASH_MODE=CAM_PARAM_FLASH,
   CAM_PARAM_AE_METERING,
   CAM_PARAM_AF_KEY,
   CAM_PARAM_AF_METERING,
   CAM_PARAM_AF_MODE,
   CAM_PARAM_MANUAL_FOCUS_DIR,
   CAM_PARAM_SHUTTER_PRIORITY,
   CAM_PARAM_APERTURE_PRIORITY,
   CAM_PARAM_ISO,
   CAM_PARAM_SCENE_MODE,
   CAM_PARAM_QUALITY, //2//23  //add by WeiD
   CAM_PARAM_IMAGESIZE, //24 add by WeiD
   CAM_PARAM_ADD_FRAME,//25,chenhe add
   CAM_NO_OF_PARAM
};

/* zoom factor enum */
enum {
	CAM_ZOOM_1X= 0,
	CAM_ZOOM_2X,
	CAM_ZOOM_4X,
	CAM_NO_OF_ZOOM
};

/* exposure compensation value enum */
enum {
	CAM_EV_NEG_4_3 = 0,
	CAM_EV_NEG_3_3,
	CAM_EV_NEG_2_3,
	CAM_EV_NEG_1_3,
	CAM_EV_ZERO,
	CAM_EV_POS_1_3,
	CAM_EV_POS_2_3,
	CAM_EV_POS_3_3,
	CAM_EV_POS_4_3,
	CAM_EV_NIGHT_SHOT,
	CAM_NO_OF_EV	
};

/* Image effect enum */
enum {
	CAM_EFFECT_NOMRAL = 0,
	CAM_EFFECT_SEPIA,
	CAM_EFFECT_WHITELINE,
	CAM_EFFECT_BLACKLINE,
	CAM_EFFECT_BW,
	CAM_EFFECT_GRAYEDGE,
	CAM_EFFECT_FILM,
	CAM_NO_OF_EFFECT
};

/* White balance enum */
enum {
	CAM_WB_AUTO = 0,
	CAM_WB_CLOUD,
	CAM_WB_DAYLIGHT,
	CAM_WB_INCANDESCENCE,
	CAM_WB_FLUORESCENT,
	CAM_WB_TUNGSTEN,
	CAM_WB_MANUAL,
	CAM_NO_OF_WB
};

/* Camera- Jpeg encode quality enum */
enum {
	CAM_JPG_QTY_LOW = 20,  //modified by WeiD
	CAM_JPG_QTY_NORMAL = 14,
	CAM_JPG_QTY_HIGH = 10,
	CAM_JPG_QTY_FINE = 7,
	CAM_NO_OF_JPG_QTY
};

/* camera image rotate/mirror enum */
enum
{
	CAM_IMAGE_NORMAL=0,
	CAM_IMAGE_ROTATE_90,
	CAM_IMAGE_ROTATE_180,
	CAM_IMAGE_ROTATE_270,
	CAM_IMAGE_MIRROR,
	CAM_IMAGE_MIRROR_ROTATE_90,
	CAM_IMAGE_MIRROR_ROTATE_180,
	CAM_IMAGE_MIRROR_ROTATE_270,
	CAM_NO_OF_IMAGE_ROTATE
};

enum
{
	CAM_BANDING_50HZ=0,
	CAM_BANDING_60HZ,
	CAM_NO_OF_BANDING
};

 

/* Special Effect for Preview, Capture and MPEG4 Encode */
enum {
	CAM_EFFECT_ENC_NORMAL=0,
	CAM_EFFECT_ENC_GRAYSCALE,
	CAM_EFFECT_ENC_SEPIA,
	CAM_EFFECT_ENC_SEPIAGREEN,
	CAM_EFFECT_ENC_SEPIABLUE,
	CAM_EFFECT_ENC_COLORINV,
	CAM_EFFECT_ENC_GRAYINV,
	CAM_EFFECT_ENC_BLACKBOARD,
	CAM_EFFECT_ENC_WHITEBOARD,
	CAM_EFFECT_ENC_COPPERCARVING,
	CAM_EFFECT_ENC_EMBOSSMENT,
	CAM_EFFECT_ENC_BLUECARVING,
	CAM_EFFECT_ENC_CONTRAST,
	CAM_EFFECT_ENC_JEAN,
	CAM_EFFECT_ENC_SKETCH,
	CAM_EFFECT_ENC_OIL,
	CAM_NO_OF_EFFECT_ENC,
	
      //add by gugogi
      CAM_EFFECT_ENC_ANTIQUE,
      CAM_EFFECT_ENC_REDRISH,
      CAM_EFFECT_ENC_GREENRISH,
      CAM_EFFECT_ENC_BLUERISH,
      CAM_EFFECT_ENC_BLACKWHITE,
      CAM_EFFECT_ENC_NEGATIVENESS,
      
      
	
};

typedef enum _VIM_CAM_EFFECT
{
	VIM_CAM_EFFECT_ENC_NORMAL=0,
	VIM_CAM_EFFECT_ENC_MONOCHROME,
	VIM_CAM_EFFECT_ENC_SEPIA,
	VIM_CAM_EFFECT_ENC_NEGATIVE,
	VIM_CAM_EFFECT_ENC_REDONLY
}VIM_CAM_EFFECT;

/* Special Effect for Playback and Post-processing */
enum {
	CAM_EFFECT_DEC_NORMAL=0,
	CAM_EFFECT_DEC_GRAYSCALE,
	CAM_EFFECT_DEC_SEPIA,
	CAM_EFFECT_DEC_SEPIAGREEN,
	CAM_EFFECT_DEC_SEPIABLUE,
	CAM_EFFECT_DEC_COLORINV,
	CAM_EFFECT_DEC_GRAYINV,
	CAM_EFFECT_DEC_WATERCOLOR,
	CAM_EFFECT_DEC_LIGHTBLUR,
	CAM_EFFECT_DEC_BLUR,
	CAM_EFFECT_DEC_STRONGBLUR,
	CAM_EFFECT_DEC_UNSHARP,
	CAM_EFFECT_DEC_SHARPEN,
	CAM_EFFECT_DEC_MORESHARPEN,
	CAM_EFFECT_DEC_MEDIAN,
	CAM_EFFECT_DEC_DILATION,
	CAM_EFFECT_DEC_EROSION,
	CAM_NO_OF_EFFECT_DEC
};

/* Adjustment Effect for Preview, Capture and MPEG4 Encode */
enum {
   CAM_ADJ_ENC_NONE=0,
   CAM_ADJ_ENC_HUE,
   CAM_ADJ_ENC_SATURATION,
   CAM_ADJ_ENC_GAMMA,
   CAM_ADJ_ENC_ADJR,
   CAM_ADJ_ENC_ADJG,
   CAM_ADJ_ENC_ADJB,
   CAM_ADJ_ENC_CONTRAST,
   CAM_ADJ_ENC_BRIGHTNESS,
   CAM_NO_OF_ADJ_ENC
};

/* Adjustment Effect for Playback and Post-processing */
enum {
   CAM_ADJ_DEC_NONE=0,
   CAM_ADJ_DEC_HUE,
   CAM_ADJ_DEC_SATURATION,
   CAM_ADJ_DEC_GAMMA,
   CAM_ADJ_DEC_ADJR,
   CAM_ADJ_DEC_ADJG,
   CAM_ADJ_DEC_ADJB,
   CAM_ADJ_DEC_CONTRAST,
   CAM_ADJ_DEC_BRIGHTNESS,
   CAM_NO_OF_ADJ_DEC
};


/* Image - Jpeg encode quality enum */
enum {
	IMG_JPG_QTY_LOW = 0,
	IMG_JPG_QTY_NORMAL,
	IMG_JPG_QTY_HIGH,
	IMG_JPG_QTY_FINE,
	IMG_NO_OF_JPG_QTY
};

/* Display output device */
enum {
	MED_DISPLAY_NONE = 0,
	MED_DISPLAY_TO_MAIN_LCD,
	MED_NO_OF_DISPLAY_DEVICE
};

/* flash mode enum */
enum {
	CAM_FLASH_OFF= 0,
	CAM_FLASH_ON,
	CAM_FLASH_AUTO,
	CAM_FLASH_REDEYE,
	CAM_NO_OF_FLASH
};

/* preview frame rate enum */
enum {
	CAMERA_PERVIEW_FRAME_RATE= 0,
	VIDEO_PERVIEW_FRAME_RATE=1
};

/* camera operation enum */
enum{
	CAM_AE_METERING=0,
	CAM_AF_KEY,
	CAM_AF_METERING,
	CAM_AF_MODE,
	CAM_MANUAL_FOCUS_DIR,
	CAM_SHUTTER_PRIORITY,
	CAM_APERTURE_PRIORITY,
	CAM_ISO,
	CAM_SCENE_MODE,
	CAM_FLASH_MODE,
	CAM_NO_OF_CAMERA_OPERATION
};

/* ae metering enum */
enum{
	CAM_AE_METER_AUTO=0,
	CAM_AE_METER_SPOT,
	CAM_AE_METER_CENTRAL,
	CAM_AE_METER_AVERAGE,
	CAM_NO_OF_AE_METERING
};

/* af key enum */
enum{
	CAM_AF_RELEASE=0,
	CAM_AF_ONE_TIME,
	CAM_AF_CONTINUE,
	CAM_NO_OF_AF_KEY
};

/* af metering enum */
enum{
	CAM_AF_SINGLE_ZONE=0,
	CAM_AF_MULTI3_ZONE,
	CAM_AF_MULTI5_ZONE,
	CAM_NO_OF_AF_METERING
};

/* af mode enum */
enum{
	CAM_AF_AUTO_MODE=0,
	CAM_AF_MACRO_MODE,
	CAM_AF_INFINITE_MODE,
	CAM_AF_MANUAL_MODE,
	CAM_NO_OF_AF_MODE
};

/* menual focus direction enum */
enum{
	CAM_FOCUS_STOP=0,
	CAM_FOCUS_FORWARD,
	CAM_FOCUS_BACKWARD,
	CAM_FOCUS_FORWARD_CONTINUE,
	CAM_FOCUS_BACKWARD_CONTINUE,
	CAM_NO_OF_FOCUS_DIR
};

/* iso number enum */
enum{
	CAM_ISO_AUTO=0,
	CAM_ISO_100,
	CAM_ISO_200,
	CAM_ISO_400,
	CAM_NO_OF_ISO
};

/* scene mode enum */
enum{
	CAM_NORMALSCENE=0,
	CAM_PORTRAIT,
	CAM_LANDSCAPE,
	CAM_SPORT,
	CAM_FLOWER,
	CAM_NIGHTSCENE,
	CAM_NO_OF_SCENE_MODE
};

/*==== DATATYPE ====*/

typedef struct {
   kal_uint16   image_width;
   kal_uint16   image_height;
   kal_uint16   buffer_width;
   kal_uint16   buffer_height;
   kal_uint16   preview_width;
   kal_uint16   preview_height;
   kal_uint16   preview_offset_x;
   kal_uint16   preview_offset_y;
   void*      image_buffer_p;
   kal_uint32  image_buffer_size;
   kal_int16   frame_rate;
   kal_int16   contrast;
   kal_int16   brightness;
   kal_int16   saturation;
   kal_int16   hue;
   kal_int16   gamma;
   kal_int16   WB;
   kal_int16   exposure;
   kal_int16   effect;
   kal_int16   zoom_factor;
   kal_int16   rotate;
   kal_int16   lcd_rotate;
   kal_int16   flash_mode;
   kal_uint8   snapshot_number;
   kal_bool   night_mode;
   kal_uint8   banding_freq;

   kal_uint8   lcd_id;
   kal_uint16  lcd_start_x;
   kal_uint16  lcd_start_y;
   kal_uint16  lcd_end_x;
   kal_uint16  lcd_end_y;
   kal_uint16  roi_offset_x;
   kal_uint16  roi_offset_y;
   kal_uint32   update_layer;
   kal_uint32   hw_update_layer;
	kal_uint32  src_key_color;	

   /* LCD Setting */
   kal_uint32   roi_background_color;         /* set the background color */

#if (defined(MT6228)||defined(MT6229))
   /* ISP Setting */
   kal_bool    manual_wb;                     /* KAL_TRUE or KAL_FALSE to enable or disable manual white balance */
   kal_uint8   ae_metering_mode;               /* AUTO, SPOT, CENTRAL WEIGHT, AVERAGE */
   kal_uint8   iso_setting;                  /* AUTO, 100, 200, 400 */
   kal_uint8	scene_mode;							/* NORMALSCENE, PORTRAIT, LANDSCAPE, SPORT, FLOWER, NIGHTSCENE */
   /* IPP Setting */
   kal_bool   image_pitch_mode;               /* KAL_TRUE or KAL_FALSE for block mode of decoded image */
   kal_uint8   image_data_format;            /* IMGDMA_IBW_OUTPUT_RGB565 or IMGDMA_IBW_OUTPUT_RGB888 */
   kal_uint16   image_pitch_bytes;            /* pitch width */

   kal_bool    overlay_frame_mode;            /* KAL_TRUE or KAL_FALSE to enable or disable overlay function */
   kal_uint8   overlay_color_depth;            /* The color depth of overlay frame buffer */
   kal_uint8   overlay_frame_source_key;
   kal_uint16  overlay_frame_width;            /* Image width of overlay frame buffer */
   kal_uint16  overlay_frame_height;         /* Image height of overlay frame buffer */
   kal_uint32  overlay_frame_buffer_address;   /* The starting address of overlay frame buffer */
   /* TV Setting */
   kal_uint8     tv_output_mode;
   kal_bool     tv_output;
   kal_uint16   tv_output_width;               /* image width for TV output */
   kal_uint16   tv_output_height;               /* image height for TV output */
   kal_uint32    tv_output_buffer1_address;      /* image buffer1 for TV source buffer */
   kal_uint32    tv_output_buffer2_address;      /* image buffer2 for TV source buffer */
   kal_uint32    tv_output_buffer_size;         /* image buffer size for TV source buffer */
#endif
} 
cam_preview_struct;

typedef struct
{
	kal_uint32	file_size[20];
	kal_uint32	file_address[20];
}	
jpegs_struct;


typedef struct {
   kal_uint8	media_mode;
   kal_uint8    source_device;
   kal_uint16    image_width;
   kal_uint16    image_height;
   void*       data;
   void**       capture_buffer_p;
   kal_uint32   file_buffer_len;
   kal_uint32   *file_size_p;
   kal_int16  flash_mode;
   kal_uint8   image_quality;
   kal_uint8   gray_mode;
   kal_uint8   snapshot_number;
   kal_uint32  capture_layer;                /* capture layer for capture from mem(lcd) case */
   kal_uint16  roi_offset_x;                 /* offset x for capture from mem(lcd) case */
   kal_uint16  roi_offset_y;                 /* offset y for capture from mem(lcd) case */
   jpegs_struct *jpegs_p;
   kal_uint8   continue_capture;

#if (defined(MT6228)||defined(MT6229))
	
	/* JPEG Setting */
	kal_uint8	jpeg_yuv_mode;
	
	kal_bool		thumbnail_mode;					/* add thumbnail in JPEG file or not */
	kal_uint16	thumbnail_width;
	kal_uint16 	thumbnail_height;
	/* IPP Setting */
	kal_bool 	overlay_frame_mode;				/* KAL_TRUE or KAL_FALSE */
	kal_uint8	overlay_color_depth;				/* 1/2/4/8 bpp */
	kal_uint8	overlay_frame_source_key;		/* source key of the overlay frame buffer */
	kal_uint16  overlay_frame_width;				/* overlay frame width */
	kal_uint16	overlay_frame_height;			/* overlay frame height */
	kal_uint32  overlay_frame_buffer_address;	/* bsae address of the overlay frame buffer */

	kal_bool		image_pitch_mode;					/* KAL_TRUE or KAL_FALSE for block mode of decoded image */
	kal_uint8	image_data_format;				/* IMGDMA_IBW_OUTPUT_RGB565 or IMGDMA_IBW_OUTPUT_RGB888 */
	kal_uint16	image_pitch_bytes;				/* pitch width */

	/* Display Setting */
	kal_uint8 	lcd_id;
	kal_uint16 	lcm_start_x;				/* the start x axis of LCM to be updated*/
	kal_uint16 	lcm_start_y;				/* the start y axis of LCM to be updated*/
	kal_uint16 	lcm_end_x;					/* the end x axis of LCM to be updated*/
	kal_uint16 	lcm_end_y;					/* the end y axis of LCM to be updated*/
//	kal_uint16 	roi_offset_x;				/* the ROI window offset x from main window */
//	kal_uint16 	roi_offset_y;				/* the ROI window offset y from main window */
	kal_uint32 	update_layer;				/* the updated layer */
	kal_uint32 	hw_update_layer;			 

	kal_bool		display;								/* output to LCM or TV */
	kal_uint32	frame_buffer_address;
	kal_uint32 	frame_buffer_size;

	kal_bool		tv_output;							/* works when display item is KAL_TRUE */
   kal_uint32 	tv_output_buffer1_address;		/* image buffer1 for TV source buffer */
	kal_uint32 	tv_output_buffer2_address;		/* image buffer2 for TV source buffer, if snapshot_number==1, this buffer is not necessary */
   kal_uint32 	tv_output_buffer_size;			/* image buffer size for TV source buffer */
#endif
} 
cam_capture_struct;


/* wufasong updated 2007.06.16 for compile pass. */
#if 1 //#if defined (JPG_DECODE) || defined (JPG_ENCODE) || defined (GIF_DECODE)
/* image */
typedef struct
{
   kal_uint16   display_width;	
   kal_uint16   display_height;	
	kal_uint16  image_clip_x1;						/* clip window start x for clipping decoded image */
	kal_uint16  image_clip_x2;						/* clip window end x for clipping decoded image */
	kal_uint16	image_clip_y1;						/* clip window start y for clipping decoded image */
	kal_uint16	image_clip_y2;						/* clip window end y for clipping decoded image */
   void*      image_buffer_p;	
   kal_uint32   image_buffer_size;
   kal_uint8   media_mode;	
   kal_uint8   media_type;	
   void*      data;
   kal_uint32   file_size;
   kal_uint16  seq_num;
   kal_uint8   blocking;
   kal_uint16  decoded_image_width;	
	kal_bool  	jpeg_thumbnail_mode;				/* decode thumbnail or not */
#if (defined(MT6228)||defined(MT6229))
	kal_bool		image_pitch_mode;					/* KAL_TRUE or KAL_FALSE for block mode of decoded image */
	kal_uint8	image_data_format;				/* IMGDMA_IBW_OUTPUT_RGB565 or IMGDMA_IBW_OUTPUT_RGB888 */
	kal_uint16	image_pitch_bytes;				/* pitch width */

	kal_bool		memory_output;						/* output another image for TV output */
	kal_uint16	memory_output_width;				/* image width for TV output */
	kal_uint16	memory_output_height;			/* image height for TV output */
	kal_uint32 	memory_output_buffer_address;
	kal_uint32 	memory_output_buffer_size;
#endif
   void      (*callback)(kal_int32 result);
}
img_decode_struct;

typedef struct
{
   kal_uint16   image_width;
   kal_uint16   image_height;
   kal_uint8   image_quality;
   void*      image_buffer_p;
   kal_uint32   image_buffer_size;
   kal_uint16   target_width;
   kal_uint16   target_height;
   kal_uint8   media_mode;
   kal_uint8   media_type;
   void*      data;
   kal_uint32   file_buffer_len;
   kal_uint32   *file_size_p;
   kal_uint16  seq_num;
   kal_uint8   blocking;
   kal_uint8   gray_mode;   
#if (defined(MT6228)||defined(MT6229))
	kal_uint8	jpeg_yuv_mode;						/* YUV420, YUV422 or Gray mode*/

	kal_bool		thumbnail_mode;					/* add thumbnail in JPEG file or not */
	kal_uint16	thumbnail_width;
	kal_uint16 	thumbnail_height;

	kal_bool 	overlay_frame_mode;				/* KAL_TRUE or KAL_FALSE */
	kal_uint8	overlay_color_depth;				/* 1/2/4/8 bpp */
	kal_uint8	overlay_frame_source_key;		/* source key of the overlay frame buffer */
	kal_uint16  overlay_frame_width;				/* overlay frame width */
	kal_uint16	overlay_frame_height;			/* overlay frame height */
	kal_uint32  overlay_frame_buffer_address;	/* bsae address of the overlay frame buffer */
#endif
}
img_encode_struct;

typedef struct
{
	kal_uint32 src_image_address;				/* the source image buffer start address */
	kal_uint32 dest_image_address;			/* the destination image buffer start address */
	kal_uint32 dest_buffer_size;				/* the buffer size of destination image buffer size */
	kal_uint16 src_width;						/* the width of image source */
	kal_uint16 src_height;						/* the height of image source */
	kal_uint16 src_pitch;						/* the pitch width of image source */
	kal_uint16 target_width;					/* the width of target image */
	kal_uint16 target_height;					/* the height of target image */
	kal_uint16 image_clip_x1;					/* image clip window start x */
	kal_uint16 image_clip_x2;					/* image clip window end x */
	kal_uint16 image_clip_y1;					/* image clip window start y */
	kal_uint16 image_clip_y2;					/* image clip window end x */
   kal_uint16  seq_num;                   /* use for non-blocking mode */
   kal_uint8   blocking;
#if (defined(MT6228)||defined(MT6229))
	kal_bool		image_pitch_mode;					/* KAL_TRUE or KAL_FALSE for block mode of decoded image */
	kal_uint8	image_data_format;				/* IMGDMA_IBW_OUTPUT_RGB565 or IMGDMA_IBW_OUTPUT_RGB888 */
	kal_uint16	image_pitch_bytes;				/* pitch width */
#endif
}
img_resize_struct;

#endif

/* wufasong updated 2007.06.16 for compile pass. */
#if 1 //#if defined (MP4_CODEC) || defined (MJPG_SUPPORT)
/* video */

/* Video encoding quality */
enum {
	VID_REC_QTY_LOW = 0,
	VID_REC_QTY_NORMAL,
	VID_REC_QTY_HIGH,
	VID_REC_QTY_FINE,
	VID_NO_OF_REC_QTY
};

/* Bit stream type, not the file format */
enum {
	VID_3GP_BIT_STREAM = 0,
	VID_MP4_BIT_STREAM,
	VID_NO_OF_BIT_STREAM
};

/* video em mode enum */
enum {
	VID_EM_MODE_DISABLED = 0,
	VID_EM_MODE_ULTRA_HIGH_BITRATE,
	VID_NO_OF_EM_MODE
};


typedef struct media_vid_record_req_struct vid_record_struct; 


typedef struct
{
   kal_wchar* file_name_p;

   kal_uint16  image_x;
   kal_uint16  image_y;
   kal_uint32  total_frame_num;
}
vid_open_file_struct;

typedef struct {
   kal_uint8   media_mode;
   kal_uint8   media_type;
   void*      data;
   kal_uint32  data_size;
   kal_uint16  seq_num;
   kal_uint16  max_width;
   kal_uint16  max_height;
   kal_uint16  *image_width;
   kal_uint16  *image_height;
   kal_uint64  *total_time;
   kal_uint8   blocking;
   kal_uint8   play_audio;
}
vid_open_struct;

typedef struct
{
   kal_uint16	display_width;
   kal_uint16	display_height;
   kal_uint16   display_offset_x;
   kal_uint16   display_offset_y;
   void*	      image_buffer_p;
   kal_uint32    image_buffer_size;
   kal_int16   play_speed;
   kal_uint8   play_audio;
   kal_uint8   display_device;  /* display device, MED_DISPLAY_NONE, MED_DISPLAY_TO_MAIN_LCD, ... */
   kal_uint8   audio_path;   /* audio path for headset mode */
   kal_uint16  seq_num;
   kal_int16   lcd_rotate;
   kal_uint16  repeats;    /* repeat count, 0xffff: infinite */

   kal_uint8   lcd_id;
   kal_uint16  lcd_start_x;
   kal_uint16  lcd_start_y;
   kal_uint16  lcd_end_x;
   kal_uint16  lcd_end_y;
   kal_uint16  roi_offset_x;
   kal_uint16  roi_offset_y;
   kal_uint32   update_layer;
   kal_uint32   hw_update_layer;

   kal_uint16   video_display_offset_x;
   kal_uint16   video_display_offset_y;

#if (defined(MT6228)||defined(MT6229))
	kal_bool		image_pitch_mode;							/* KAL_TRUE or KAL_FALSE for block mode of decoded image */
	kal_uint8	image_data_format;						/* IMGDMA_IBW_OUTPUT_RGB565 or IMGDMA_IBW_OUTPUT_RGB888 */
	kal_uint16	image_pitch_bytes;						/* pitch width */
	/* TV Setting */
	kal_bool		tv_output;
   kal_uint32 	tv_output_buffer1_address;		/* image buffer1 for TV source buffer */
   kal_uint32 	tv_output_buffer2_address;		/* image buffer2 for TV source buffer */
   kal_uint32 	tv_output_buffer_size;			/* image buffer size for TV source buffer */
#endif
}
vid_play_struct;

typedef struct
{
   kal_uint8     get_frame;
   kal_uint32    frame_num;
   kal_uint64	time;
   kal_uint16	display_width;
   kal_uint16	display_height;
   kal_int16   display_offset_x;
   kal_int16   display_offset_y;
   void*	      image_buffer_p;
   kal_uint32    image_buffer_size;
   kal_uint8   blocking;
   kal_uint16  seq_num;
#if (defined(MT6228)||defined(MT6229))
   kal_bool		image_pitch_mode;							/* KAL_TRUE or KAL_FALSE for block mode of decoded image */
   kal_uint8	image_data_format;						/* IMGDMA_IBW_OUTPUT_RGB565 or IMGDMA_IBW_OUTPUT_RGB888 */
   kal_uint16	image_pitch_bytes;						/* pitch width */
   /* TV Setting */
   kal_bool		tv_output;
   kal_uint32 	tv_output_buffer1_address;		/* image buffer1 for TV source buffer */
   kal_uint32 	tv_output_buffer2_address;		/* image buffer2 for TV source buffer */
   kal_uint32 	tv_output_buffer_size;			/* image buffer size for TV source buffer */
#endif
}
vid_seek_struct;

typedef struct {
   void*	      image_buffer_p;
   kal_uint32    image_buffer_size;
} 
vid_snapshot_struct;


typedef struct
{
   kal_uint8	media_mode;
   kal_uint8	media_type;
   void*       data;
   kal_uint16	display_width;
   kal_uint16	display_height;
   void*	      image_buffer_p;
   kal_uint32    image_buffer_size;
   kal_uint16   frame_num;
   kal_uint16  seq_num;
   kal_uint8   blocking;
}
vid_get_iframe_struct;

#endif /* MP4_CODEC */

/* TV output mode */
enum
{
   MED_TV_OUT_DISABLE=0,
   MED_TV_OUT_MODE1,
   MED_TV_OUT_MODE2,
   MED_TV_OUT_MODE3,
   MED_TV_OUT_MODE4   
};

/* TV output format */
enum
{
   MED_TV_FORMAT_NTSC=0,
   MED_TV_FORMAT_PALM,
   MED_TV_FORMAT_PALC,
   MED_TV_FORMAT_PAL	
};

#if (defined(MT6228)||defined(MT6229))   /* will change to TV_OUT_SUPPORT */
/* tv out */
typedef struct
{
   kal_uint8   tv_output_mode;
   kal_uint8   tv_output_format;
   kal_uint16   tv_output_width;               /* image width for TV output */
   kal_uint16   tv_output_height;               /* image height for TV output */
}
tvo_mode_struct;


#endif

/*==== PROTOTYPE ====*/
/* audio */
kal_int32 media_aud_play_id(module_type src_mod_id, void *id_param);
kal_int32 media_aud_stop_id(module_type src_mod_id, kal_uint8 audio_id);
kal_int32 media_aud_play_string(module_type src_mod_id, void *string_param);
kal_int32 media_aud_stop_string(module_type src_mod_id, kal_uint8 src_id);
kal_int32 media_aud_play_file(module_type src_mod_id, void *file_param);
kal_int32 media_aud_stop_file(module_type src_mod_id, kal_uint8 src_id);
//kal_int32 media_aud_start_record(module_type src_mod_id, kal_wchar* file_name, kal_uint8 format, kal_bool default_input,kal_uint8 input_source);
kal_int32 media_aud_start_record( module_type src_mod_id,kal_wchar *file_name,  kal_uint8 format,kal_uint8 quality,    kal_bool default_input,  kal_uint8 input_source);                    

kal_int32 media_aud_stop_record(module_type src_mod_id, kal_uint8 src_id);
kal_int32 media_aud_pause(module_type src_mod_id, kal_uint8 src_id);
kal_int32 media_aud_resume(module_type src_mod_id, kal_uint8 src_id);
kal_int32 media_aud_block(module_type src_mod_id, kal_uint16 mod_id, kal_uint8 level);
kal_int32 media_aud_set_headset_mode_output_path(module_type src_mod_id, kal_uint8 device);
kal_int32 media_aud_set_vibrator_enabled( module_type src_mod_id, kal_uint8 enable );
kal_int32 media_aud_store(module_type src_mod_id, kal_uint8 src_id);
kal_int32 media_aud_restore(module_type src_mod_id, void *file_param);
kal_int32 media_aud_set_equalizer(module_type src_mod_id, kal_int8 *magnitude);
kal_int32 media_aud_get_file_data_length( module_type src_mod_id, void* file_name,kal_uint32 PlayProgress,kal_uint32* length );
kal_int32 media_aud_get_file_data_progress( module_type src_mod_id, kal_uint32* progress );
kal_int32 media_aud_set_file_data_progress( module_type src_mod_id, kal_uint32 progress ,kal_uint32* getDuration);

kal_int32 media_aud_fmr_power_on(module_type src_mod_id, kal_uint8 output_path);
kal_int32 media_aud_fmr_power_off(module_type src_mod_id);
kal_int32 media_aud_fmr_set_freq(module_type src_mod_id, kal_uint16 freq);
kal_int32 media_aud_fmr_mute(module_type src_mod_id, kal_uint8 mute);

#ifdef DSP_WT_SYN
/* mma */
kal_int32 media_mma_midi_open(med_handler handler, mma_player_struct* param);
kal_int32 media_mma_midi_close(kal_int32 handle);
kal_int32 media_mma_midi_play(kal_int32 handle);
kal_int32 media_mma_midi_stop(kal_int32 handle);
kal_int32 media_mma_midi_pause(kal_int32 handle);
kal_int32 media_mma_midi_resume(kal_int32 handle);
kal_int32 media_mma_midi_set_start_time(kal_int32 handle, kal_int32 start_time);
kal_int32 media_mma_midi_set_stop_time(kal_int32 handle, kal_int32 stop_time);
kal_int32 media_mma_midi_get_current_time(kal_int32 handle);
kal_int32 media_mma_midi_get_duration(kal_int32 handle);
kal_int32 media_mma_midi_get_duration_by_string(const kal_uint8 *data);
kal_uint8 media_mma_midi_get_volume(kal_int32 handle);
kal_int32 media_mma_midi_set_volume(kal_int32 handle, kal_uint8 volume);
kal_int32 media_mma_midi_get_rate(kal_int32 handle);
kal_int32 media_mma_midi_set_rate(kal_int32 handle, kal_int32 rate);
kal_int32 media_mma_midi_get_tempo(kal_int32 handle);
kal_int32 media_mma_midi_set_tempo(kal_int32 handle, kal_int32 tempo);
kal_int32 media_mma_midi_get_pitch(kal_int32 handle);
kal_int32 media_mma_midi_set_pitch(kal_int32 handle, kal_int32 pitch);
kal_int32 media_mma_midi_open_device(void);
kal_int32 media_mma_midi_close_device(kal_int32 handle);
kal_int32 media_mma_midi_send_long_msg(kal_int32 handle, kal_uint8* buf, kal_uint16 len);
kal_int32 media_mma_midi_send_short_msg(kal_int32 handle, kal_uint8 type, kal_uint8 data1, kal_uint8 data2);
kal_uint8 media_mma_midi_get_channel_volume(kal_int32 handle, kal_uint8 channel);
kal_int32 media_mma_midi_set_channel_volume(kal_int32 handle, kal_uint8 channel, kal_uint8 level);
void media_mma_midi_get_program(kal_int32 handle, kal_uint8 channel, kal_int16* bank, kal_int8* program);
kal_int32 media_mma_midi_set_program(kal_int32 handle, kal_uint8 channel, kal_uint8 bank, kal_uint8 program);
kal_bool media_mma_midi_bank_support(kal_int32 handle);
void media_mma_midi_get_bank_list(kal_int32 handle, kal_bool custom, const kal_int16 **buf, kal_int16 *len);
void media_mma_midi_get_program_list(kal_int32 handle, kal_int16 bank, const kal_int8 **buf, kal_int16 *len);
const kal_uint8* media_mma_midi_get_program_name(kal_int32 handle, kal_int16 bank, kal_int8 prog);
const kal_uint8* media_mma_midi_get_key_name(kal_int32 handle, kal_int16 bank, kal_int8 prog, kal_int8 key);

kal_int32 media_mma_tone_open(med_handler handler, mma_player_struct* param);
kal_int32 media_mma_tone_close(kal_int32 handle);
kal_int32 media_mma_tone_play(kal_int32 handle);
kal_int32 media_mma_tone_stop(kal_int32 handle);
kal_int32 media_mma_tone_pause(kal_int32 handle);
kal_int32 media_mma_tone_resume(kal_int32 handle);
kal_int32 media_mma_tone_set_data(kal_int32 handle, mma_player_struct* param);
kal_int32 media_mma_tone_set_start_time(kal_int32 handle, kal_int32 start_time);
kal_int32 media_mma_tone_set_stop_time(kal_int32 handle, kal_int32 stop_time);
kal_int32 media_mma_tone_get_current_time(kal_int32 handle);
kal_int32 media_mma_tone_get_duration(kal_int32 handle);
kal_uint8 media_mma_tone_get_volume(kal_int32 handle);
kal_int32 media_mma_tone_set_volume(kal_int32 handle, kal_uint8 volume);
kal_int8 media_mma_tone_get_pitch(kal_int32 handle);
kal_int32 media_mma_tone_set_pitch(kal_int32 handle, kal_int32 pitch);


kal_int32 media_mma_wav_open(med_handler handler, mma_player_struct* param);
kal_int32 media_mma_wav_close(kal_int32 handle);
kal_int32 media_mma_wav_play(kal_int32 handle);
kal_int32 media_mma_wav_stop(kal_int32 handle);
kal_int32 media_mma_wav_pause(kal_int32 handle);
kal_int32 media_mma_wav_resume(kal_int32 handle);
kal_int32 media_mma_wav_set_start_time(kal_int32 handle, kal_int32 start_time);
kal_int32 media_mma_wav_set_stop_time(kal_int32 handle, kal_int32 stop_time);
kal_int32 media_mma_wav_get_current_time(kal_int32 handle);
kal_int32 media_mma_wav_get_duration(kal_int32 handle);
kal_uint8 media_mma_wav_get_volume(kal_int32 handle);
kal_int32 media_mma_wav_set_volume(kal_int32 handle, kal_uint8 volume);

kal_int32 media_mma_imy_open(med_handler handler, mma_player_struct* param);
kal_int32 media_mma_imy_close(kal_int32 handle);
kal_int32 media_mma_imy_play(kal_int32 handle);
kal_int32 media_mma_imy_stop(kal_int32 handle);
kal_int32 media_mma_imy_pause(kal_int32 handle);
kal_int32 media_mma_imy_resume(kal_int32 handle);
kal_int32 media_mma_imy_set_start_time(kal_int32 handle, kal_int32 start_time);
kal_int32 media_mma_imy_set_stop_time(kal_int32 handle, kal_int32 stop_time);
kal_int32 media_mma_imy_get_current_time(kal_int32 handle);
kal_int32 media_mma_imy_get_duration(kal_int32 handle);
kal_uint8 media_mma_imy_get_volume(kal_int32 handle);
kal_int32 media_mma_imy_set_volume(kal_int32 handle, kal_uint8 volume);

kal_int32 media_mma_amr_open(med_handler handler, mma_player_struct* param);
kal_int32 media_mma_amr_close(kal_int32 handle);
kal_int32 media_mma_amr_play(kal_int32 handle);
kal_int32 media_mma_amr_stop(kal_int32 handle);
kal_int32 media_mma_amr_pause(kal_int32 handle);
kal_int32 media_mma_amr_resume(kal_int32 handle);
kal_int32 media_mma_amr_set_start_time(kal_int32 handle, kal_int32 start_time);
kal_int32 media_mma_amr_set_stop_time(kal_int32 handle, kal_int32 stop_time);
kal_int32 media_mma_amr_get_current_time(kal_int32 handle);
kal_int32 media_mma_amr_get_duration(const kal_uint8 *data, kal_int32 size, kal_int32 *duration);
kal_uint8 media_mma_amr_get_volume(kal_int32 handle);
kal_int32 media_mma_amr_set_volume(kal_int32 handle, kal_uint8 volume);

kal_int32 media_mma_daf_open(med_handler handler, mma_player_struct* param);
kal_int32 media_mma_daf_close(kal_int32 handle);
kal_int32 media_mma_daf_play(kal_int32 handle);
kal_int32 media_mma_daf_stop(kal_int32 handle);
kal_int32 media_mma_daf_pause(kal_int32 handle);
kal_int32 media_mma_daf_resume(kal_int32 handle);

kal_int32 media_mma_aac_open(med_handler handler, mma_player_struct* param);
kal_int32 media_mma_aac_close(kal_int32 handle);
kal_int32 media_mma_aac_play(kal_int32 handle);
kal_int32 media_mma_aac_stop(kal_int32 handle);
kal_int32 media_mma_aac_pause(kal_int32 handle);
kal_int32 media_mma_aac_resume(kal_int32 handle);

kal_int32 media_mma_au_open(med_handler handler, mma_player_struct* param);
kal_int32 media_mma_au_close(kal_int32 handle);
kal_int32 media_mma_au_play(kal_int32 handle);
kal_int32 media_mma_au_stop(kal_int32 handle);
kal_int32 media_mma_au_pause(kal_int32 handle);
kal_int32 media_mma_au_resume(kal_int32 handle);

#endif /* DSP_WT_SYN */

#if defined (CAMERA_MODULE) || defined (ISP_SUPPORT)

/* camera */
kal_int32 media_cam_power_up(module_type src_mod_id, kal_uint16  seq_num);
kal_int32 media_cam_power_down(module_type src_mod_id, kal_int16 delay_time);
kal_int32 media_cam_preview(module_type src_mod_id, cam_preview_struct *preview);
void media_cam_stop(module_type src_mod_id);
kal_int32 media_cam_capture(module_type src_mod_id, cam_capture_struct *capture);
void media_cam_set_param(module_type src_mod_id, kal_uint16 param_id, kal_int16 value);
void media_cam_set_flash(module_type src_mod_id, kal_int16 flash_mode);
kal_int32 media_cam_md_open(module_type src_mod_id, kal_uint32 sensLevel, void(*md_callback)(int32));
kal_int32 media_cam_md_close(module_type src_mod_id);
kal_int32 media_cam_save_photo(UINT32 fhd );

#endif


#if defined (JPG_DECODE) || defined (JPG_ENCODE) || defined (GIF_DECODE)

/* image */
kal_int32 media_img_decode(module_type src_mod_id, void *decode);
kal_int32 media_img_encode(module_type src_mod_id, img_encode_struct *encode);
void media_img_stop(module_type src_mod_id);
kal_int32 media_img_resize(module_type src_mod_id, img_resize_struct *resize);

#endif


#if defined (MP4_CODEC) || defined (MJPG_SUPPORT)
/* video */
kal_int32 media_vid_preview(module_type src_mod_id, void* preview);
kal_int32 media_vid_record(module_type src_mod_id, void* record);
void media_vid_stop(module_type src_mod_id);
void media_vid_file_merge(module_type src_mod_id, kal_wchar* path);
kal_int32 media_vid_open_file(module_type src_mod_id, void* file_name, kal_uint16 seq_num,void (*videoplayer_drawtime_hdlr)(kal_int32));
void media_vid_close_file(module_type src_mod_id);
kal_int32 media_vid_open(module_type src_mod_id, vid_open_struct* open);
void media_vid_close(module_type src_mod_id);
kal_int32 media_vid_play(module_type src_mod_id, vid_play_struct* play);
kal_int32 media_vid_seek(module_type src_mod_id, vid_seek_struct* seek);
kal_int32 media_vid_pause(module_type src_mod_id);
kal_int32 media_vid_resume(module_type src_mod_id);
kal_int32 media_vid_snapshot(module_type src_mod_id, vid_snapshot_struct* snapshot);
kal_int32 media_vid_get_iframe(module_type src_mod_id, vid_get_iframe_struct* iframe);
kal_int32 media_vid_set_display(module_type src_mod_id, kal_uint8 device);
void media_vid_get_current_play_time(module_type src_mod_id, kal_uint64* time);
kal_int32 media_vid_get_decode_time(module_type src_mod_id, kal_uint32 frame_no, kal_uint64* time);
void media_vid_get_current_record_time(module_type src_mod_id, kal_uint64* time);
void media_vid_get_current_frame_num(module_type src_mod_id, kal_uint32* frame_num);
kal_int32 media_vid_frame_to_time(kal_uint32 frame, kal_uint32 *time);
kal_int32 media_vid_time_to_frame(kal_uint32 time, kal_uint32 *frame);
kal_bool media_vid_check_unfinished_file(module_type src_mod_id);
void media_vid_clean_temp_files(module_type src_mod_id);
void media_vid_set_em_mode(kal_uint8 mode);
kal_uint8 media_vid_get_em_mode(void);
kal_int32 media_vid_display_interface(module_type src_mod_id);
void media_vid_display_set(module_type src_mod_id,kal_uint16 ZoomWidth, kal_uint16 ZoomHeight, kal_int16 startX, kal_int16 startY,  kal_int16 cutX, kal_int16 cutY, kal_int16 cutW, kal_int16 cutH, kal_uint16 rotate);
void media_vid_get_current_play_time_block_req(module_type src_mod_id, kal_uint64 * time);
#endif /* MP4_CODEC or MJPG_SUUPORT */

#if (defined(MT6228)||defined(MT6229))

void tvo_set_result(kal_int32 result);
kal_int32 media_tvo_start(module_type src_mod_id, tvo_mode_struct *mode);
void media_tvo_stop(module_type src_mod_id);
kal_int32 media_tvo_set_mode(module_type src_mod_id, tvo_mode_struct *mode);
kal_uint32 media_tvo_get_current_buffer(module_type src_mod_id);

#endif

#ifdef __MED_VR_MOD__
kal_uint32 media_aud_vr_new_session_id( void );
kal_int32 media_aud_vr_get_version( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang );
kal_int32 media_aud_vr_get_param( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_int16* sim, kal_int16* diff, kal_int16* rej );
kal_int32 media_aud_vr_set_param( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_int16 sim, kal_int16 diff, kal_int16 rej );
kal_int32 media_aud_vr_init_rcg( module_type src_mod_id, kal_uint32 session_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id );
kal_int32 media_aud_vr_init_trn( module_type src_mod_id, kal_uint32 session_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 word_id );
kal_int32 media_aud_vr_voice_in( module_type src_mod_id, kal_uint32 session_id, kal_uint8 seq_no );
kal_int32 media_aud_vr_del_tag( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 word_id );
kal_int32 media_aud_vr_check_tag( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 word_id );
kal_int32 media_aud_vr_sync_db( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16* id_length_p, kal_uint16* id_array );
kal_int32 media_aud_vr_play_tag( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 word_id, kal_uint16 identifier );
kal_int32 media_aud_vr_get_dir( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16* folder_name );
kal_int32 media_aud_vr_add_tag( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 id_length, kal_uint16* id_array );
void media_aud_vr_abort( module_type src_mod_id );
#endif // __MED_VR_MOD__

#ifdef __MED_VRSI_MOD__        

kal_uint32 media_aud_vrsi_new_session_id(void); 
void media_aud_vrsi_prompt_cnf(module_type src_mod_id, kal_uint32 session_id);  
void media_aud_vrsi_read_dgt_adp_prompt(kal_uint8 *pmp_len, const kal_uint16 **pmp_text);    
kal_uint16 media_aud_vrsi_read_cmd_trn_result(void);
void media_aud_vrsi_read_cmd_rcg_result(media_aud_vrsi_cmd_rcg_result_struct *vrsi_rcg_res);
void media_aud_vrsi_read_dgt_rcg_result(kal_uint16 *digit_length, kal_uint16 **digit_list); 

kal_int32 media_aud_vrsi_init(module_type src_mod_id); 
kal_int32 media_aud_vrsi_add_tags(module_type src_mod_id, kal_uint32 session_id, kal_uint8 *grammar, kal_uint16 tag_num, kal_uint16** name_list, kal_int32 *long_list, kal_uint8 *is_name_list, kal_uint16 *id_list);
kal_int32 media_aud_vrsi_start_cmd_trn(module_type src_mod_id, kal_uint32 session_id, kal_uint8 *grammar, kal_uint16 *tag_name, kal_int32 tag_long);
kal_int32 media_aud_vrsi_start_cmd_rcg(module_type src_mod_id, kal_uint32 session_id, kal_uint8 *grammar);
kal_int32 media_aud_vrsi_start_digit_rcg(module_type src_mod_id, kal_uint32 session_id, kal_uint8 lang, kal_uint16 limit);
kal_int32 media_aud_vrsi_start_digit_adapt(module_type src_mod_id, kal_uint32 session_id, kal_uint8 lang);
kal_int32 media_aud_vrsi_play_tag(module_type src_mod_id, kal_uint32 session_id, kal_uint8 *grammar, kal_uint16 tag_id, kal_uint8 volume, kal_uint8 output_path);
kal_int32 media_aud_vrsi_play_TTS(module_type src_mod_id, kal_uint32 session_id, kal_uint16 *text, kal_uint8 volume, kal_uint8 output_path, kal_uint8 lang);
kal_int32 media_aud_vrsi_get_tag_num(module_type src_mod_id, kal_uint8 *grammar, kal_int32 *tag_num);
kal_int32 media_aud_vrsi_get_tag_info(module_type src_mod_id, kal_uint8 *grammar, kal_int32 tag_num, kal_uint16 *id_list, kal_uint16 **name_list, kal_int32 *long_list);
kal_int32 media_aud_vrsi_del_tags(module_type src_mod_id, kal_uint8 *grammar, kal_int32 tag_num, kal_uint16 *id_list);
kal_int32 media_aud_vrsi_reset_tags(module_type src_mod_id, kal_uint8 *grammar);
kal_int32 media_aud_vrsi_sync_db(module_type src_mod_id, kal_uint8 *grammar, kal_int32 tag_num, kal_uint16 *id_list, kal_uint16 **name_list, kal_uint8 *sync_flag);
void media_aud_vrsi_abort(module_type src_mod_id, kal_uint32 session_id);
void media_aud_vrsi_close(module_type src_mod_id);
kal_int32 media_aud_vrsi_get_error_msg(void); 
kal_int32 media_aud_vrsi_query_tag_id(kal_uint8 *grammar, kal_int32 tag_long, kal_uint16 *tag_id); 
kal_int32 media_aud_vrsi_query_next_tag_id(kal_uint8 *grammar, kal_uint16 *tag_id);
kal_int32 media_aud_vrsi_query_all_tag_ids(kal_uint8 *grammar, kal_int32 tag_long, kal_uint16 buf_size, kal_uint16 *id_buf, kal_uint16 *tag_num, kal_bool *getAll) ; 


#endif //__MED_VRSI_MOD__
#ifdef __MED_GENERAL_TTS__
extern kal_int32 media_aud_tts_set_attr_req(module_type src_mod_id, kal_uint32 attr_id, kal_uint32 attr_value);
extern kal_int32 media_aud_tts_play_req(module_type src_mod_id, void *tts_param);
extern kal_int32 media_aud_tts_stop_req(module_type src_mod_id);
extern kal_int32 media_aud_tts_pause_req(module_type src_mod_id);
extern kal_int32 media_aud_tts_resume_req(module_type src_mod_id);
#endif /* #ifdef __MED_GENERAL_TTS__ */
#ifdef __MED_BT_MOD__
extern void media_aud_bt_audio_close(kal_uint8 profile);
extern void media_aud_bt_turn_off_audio(kal_uint8 profile);
#endif /*#ifdef __MED_BT_MOD__*/

#ifdef __MED_WEBCAM_MOD__
extern void media_webcam_set_default_jpeg_size(kal_uint8 index, kal_uint32 max_size);
extern kal_int32 media_webcam_start(module_type src_mod_id, void *start);
extern void media_webcam_stop(module_type src_mod_id);
extern kal_int32 media_webcam_pause(module_type src_mod_id, void *pause);
extern kal_int32 media_webcam_resume(module_type src_mod_id, void *resume);
extern kal_int32 media_webcam_capture(module_type src_mod_id);
extern kal_int32 media_webcam_rotate(module_type src_mod_id, void *rotate);
extern kal_uint8 media_webcam_get_resolution_num(void);
extern void media_webcam_get_resolution_dimension(kal_uint8 index, kal_uint16 *pwidth, kal_uint16 *pheight);
#endif /* __MED_WEBCAM_MOD__ */ 

kal_int32 media_get_ext_buffer(module_type src_mod_id, void** buffer_p, kal_uint32 buffer_size);
kal_int32 media_free_ext_buffer(module_type src_mod_id, void** buffer_p);
kal_int32 media_get_int_buffer(module_type src_mod_id, void** buffer_p, kal_uint32 buffer_size);
kal_int32 media_free_int_buffer(module_type src_mod_id, void** buffer_p);

#endif /* _MED_API_H */

#endif /* MED_NOT_PRESENT */


