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
 *	 mdi_camera.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Camera interfce header file
 *
 * Author:
 * -------
 *	
 *                      
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _MDI_CAMERA_H_
#define _MDI_CAMERA_H_

#ifdef __MMI_CAMERA__

#ifndef _MDI_DATATYPE_H_
	#error "Please include mdi_datatype.h before mdi_camera.h"
#endif

#include "mmi_data_types.h" /* wufasong added 2007.06.13 */

#ifdef MMI_ON_HARDWARE_P
	#include "med_api.h"
#endif /* MMI_ON_HARDWARE_P */
 
/***************************************************************************** 
* Define
*****************************************************************************/
/* Parameters settings - mapping to media(driver's setting) */
#ifdef MMI_ON_HARDWARE_P
	/* On Target */
	#define MDI_CAMERA_EV_P4								CAM_EV_POS_4_3
	#define MDI_CAMERA_EV_P3								CAM_EV_POS_3_3
	#define MDI_CAMERA_EV_P2								CAM_EV_POS_2_3
	#define MDI_CAMERA_EV_P1								CAM_EV_POS_1_3
	#define MDI_CAMERA_EV_0									CAM_EV_ZERO
	#define MDI_CAMERA_EV_N1								CAM_EV_NEG_1_3
	#define MDI_CAMERA_EV_N2								CAM_EV_NEG_2_3
	#define MDI_CAMERA_EV_N3								CAM_EV_NEG_3_3
	#define MDI_CAMERA_EV_N4								CAM_EV_NEG_4_3	
	#define MDI_CAMERA_EV_COUNT							CAM_NO_OF_EV	

	#define MDI_CAMERA_EFFECT_NORMAL						CAM_EFFECT_ENC_NORMAL
	#define MDI_CAMERA_EFFECT_GRAYSCALE 				CAM_EFFECT_ENC_GRAYSCALE	
	#define MDI_CAMERA_EFFECT_SEPIA						CAM_EFFECT_ENC_SEPIA
	#define MDI_CAMERA_EFFECT_SEPIA_GREEN				CAM_EFFECT_ENC_SEPIAGREEN
	#define MDI_CAMERA_EFFECT_SEPIA_BLUE				CAM_EFFECT_ENC_SEPIABLUE
	#define MDI_CAMERA_EFFECT_COLOR_INVERT				CAM_EFFECT_ENC_COLORINV
	#define MDI_CAMERA_EFFECT_GRAY_INVERT				CAM_EFFECT_ENC_GRAYINV
	#define MDI_CAMERA_EFFECT_BLACKBOARD				CAM_EFFECT_ENC_BLACKBOARD
	#define MDI_CAMERA_EFFECT_WHITEBOARD				CAM_EFFECT_ENC_WHITEBOARD
	#define MDI_CAMERA_EFFECT_COPPER_CARVING			CAM_EFFECT_ENC_COPPERCARVING
	#define MDI_CAMERA_EFFECT_BLUE_CARVING				CAM_EFFECT_ENC_BLUECARVING
	#define MDI_CAMERA_EFFECT_EMBOSSMENT				CAM_EFFECT_ENC_EMBOSSMENT
	#define MDI_CAMERA_EFFECT_CONTRAST					CAM_EFFECT_ENC_CONTRAST
	#define MDI_CAMERA_EFFECT_JEAN						CAM_EFFECT_ENC_JEAN
	#define MDI_CAMERA_EFFECT_SKETCH						CAM_EFFECT_ENC_SKETCH
	#define MDI_CAMERA_EFFECT_OIL							CAM_EFFECT_ENC_OIL
	#define MDI_CAMERA_EFFECT_COUNT						CAM_NO_OF_EFFECT_ENC
	//add by gugogi
	#define MDI_CAMERA_EFFECT_ANTIQUE                                  CAM_EFFECT_ENC_ANTIQUE
       #define  MDI_CAMERA_EFFECT_REDRISH                                 CAM_EFFECT_ENC_REDRISH
      #define MDI_CAMERA_EFFECT_GREENRISH                              CAM_EFFECT_ENC_GREENRISH
      #define MDI_CAMERA_EFFECT_BLUERISH                                 CAM_EFFECT_ENC_BLUERISH
      #define MDI_CAMERA_EFFECT_BLACKWHITE                            CAM_EFFECT_ENC_BLACKWHITE
      #define MDI_CAMERA_EFFECT_NEGATIVENESS                        CAM_EFFECT_ENC_NEGATIVENESS

	#define MDI_CAMERA_EFFECT_MONOCHROME   				VIM_CAM_EFFECT_ENC_MONOCHROME
	#define MDI_CAMERA_EFFECT_NEGATIVE     				VIM_CAM_EFFECT_ENC_NEGATIVE
       #define MDI_CAMERA_EFFECT_REDONLY      					VIM_CAM_EFFECT_ENC_REDONLY


	#define MDI_CAMERA_WB_AUTO								CAM_WB_AUTO	
	#define MDI_CAMERA_WB_DAYLIGHT						CAM_WB_DAYLIGHT		
	#define MDI_CAMERA_WB_TUNGSTEN						CAM_WB_TUNGSTEN	
	#define MDI_CAMERA_WB_FLUORESCENT					CAM_WB_FLUORESCENT	
	#define MDI_CAMERA_WB_CLOUD							CAM_WB_CLOUD
	#define MDI_CAMERA_WB_INCANDESCENCE 				CAM_WB_INCANDESCENCE
	#define MDI_CAMERA_WB_COUNT							CAM_NO_OF_WB

	#define MDI_CAMERA_BANDING_50HZ 						CAM_BANDING_50HZ
	#define MDI_CAMERA_BANDING_60HZ 						CAM_BANDING_60HZ
	#define MDI_CAMERA_BANDING_COUNT 					CAM_NO_OF_BANDING

	#define MDI_CAMERA_JPG_QTY_FINE						CAM_JPG_QTY_FINE
	#define MDI_CAMERA_JPG_QTY_HIGH						CAM_JPG_QTY_HIGH
	#define MDI_CAMERA_JPG_QTY_NORMAL					CAM_JPG_QTY_NORMAL
	#define MDI_CAMERA_JPG_QTY_LOW						CAM_JPG_QTY_LOW
	#define MDI_CAMERA_JPG_QTY_COUNT						CAM_NO_OF_JPG_QTY

	#define MDI_CAMERA_FLASH_OFF							CAM_FLASH_OFF
	#define MDI_CAMERA_FLASH_ON							CAM_FLASH_ON
	#define MDI_CAMERA_FLASH_AUTO							CAM_FLASH_AUTO
	#define MDI_CAMERA_FLASH_REDEYE						CAM_FLASH_REDEYE

	#define MDI_CAMERA_AE_METER_AUTO						CAM_AE_METER_AUTO
	#define MDI_CAMERA_AE_METER_SPOT						CAM_AE_METER_SPOT
	#define MDI_CAMERA_AE_METER_CENTER					CAM_AE_METER_CENTRAL
	#define MDI_CAMERA_AE_METER_AVERAGE					CAM_AE_METER_AVERAGE

	#define MDI_CAMERA_ISO_AUTO							CAM_ISO_AUTO
	#define MDI_CAMERA_ISO_100								CAM_ISO_100
	#define MDI_CAMERA_ISO_200								CAM_ISO_200
	#define MDI_CAMERA_ISO_400								CAM_ISO_400

	#define MDI_CAMERA_SHOT_MODE_AUTO					(0)
	#define MDI_CAMERA_SHOT_MODE_SHUTTER_PRI			(1)
	#define MDI_CAMERA_SHOT_MODE_APERTURE_PRI			(2)
	#define MDI_CAMERA_SHOT_MODE_PORTRAIT				(3)
	#define MDI_CAMERA_SHOT_MODE_LANDSCAPE				(4)
	#define MDI_CAMERA_SHOT_MODE_SPORT					(5)
	#define MDI_CAMERA_SHOT_MODE_FLOWER					(6)
	#define MDI_CAMERA_SHOT_MODE_NIGHT					(7)

	#define MDI_CAMERA_PREVIEW_ROTATE_0					CAM_IMAGE_NORMAL
	#define MDI_CAMERA_PREVIEW_ROTATE_90				CAM_IMAGE_ROTATE_90
	#define MDI_CAMERA_PREVIEW_ROTATE_180				CAM_IMAGE_ROTATE_180
	#define MDI_CAMERA_PREVIEW_ROTATE_270				CAM_IMAGE_ROTATE_270
	#define MDI_CAMERA_PREVIEW_ROTATE_0_MIRROR		CAM_IMAGE_MIRROR
	#define MDI_CAMERA_PREVIEW_ROTATE_90_MIRROR		CAM_IMAGE_MIRROR_ROTATE_90
	#define MDI_CAMERA_PREVIEW_ROTATE_180_MIRROR		CAM_IMAGE_MIRROR_ROTATE_180
	#define MDI_CAMERA_PREVIEW_ROTATE_270_MIRROR		CAM_IMAGE_MIRROR_ROTATE_270

	#if defined(ISP_SUPPORT)
		#define MDI_CAMERA_LCM_ROTATE_0					LCD_LAYER_ROTATE_NORMAL
		#define MDI_CAMERA_LCM_ROTATE_90					LCD_LAYER_ROTATE_90
		#define MDI_CAMERA_LCM_ROTATE_180				LCD_LAYER_ROTATE_180
		#define MDI_CAMERA_LCM_ROTATE_270				LCD_LAYER_ROTATE_270
		#define MDI_CAMERA_LCM_ROTATE_0_MIRROR			LCD_LAYER_MIRROR
		#define MDI_CAMERA_LCM_ROTATE_90_MIRROR		LCD_LAYER_MIRROR_ROTATE_90
		#define MDI_CAMERA_LCM_ROTATE_180_MIRROR		LCD_LAYER_MIRROR_ROTATE_180
		#define MDI_CAMERA_LCM_ROTATE_270_MIRROR		LCD_LAYER_MIRROR_ROTATE_270

		#define MDI_CAMERA_POST_EFFECT_NORMAL			CAM_EFFECT_DEC_NORMAL
		#define MDI_CAMERA_POST_EFFECT_GRAYSCALE		CAM_EFFECT_DEC_GRAYSCALE
		#define MDI_CAMERA_POST_EFFECT_SEPIA			CAM_EFFECT_DEC_SEPIA
		#define MDI_CAMERA_POST_EFFECT_SEPIAGREEN		CAM_EFFECT_DEC_SEPIAGREEN
		#define MDI_CAMERA_POST_EFFECT_SEPIABLUE		CAM_EFFECT_DEC_SEPIABLUE
		#define MDI_CAMERA_POST_EFFECT_COLORINV		CAM_EFFECT_DEC_COLORINV
		#define MDI_CAMERA_POST_EFFECT_GRAYINV			CAM_EFFECT_DEC_GRAYINV
		#define MDI_CAMERA_POST_EFFECT_WATERCOLOR		CAM_EFFECT_DEC_WATERCOLOR
		#define MDI_CAMERA_POST_EFFECT_LIGHTBLUR		CAM_EFFECT_DEC_LIGHTBLUR
		#define MDI_CAMERA_POST_EFFECT_BLUR				CAM_EFFECT_DEC_BLUR
		#define MDI_CAMERA_POST_EFFECT_STRONGBLUR		CAM_EFFECT_DEC_STRONGBLUR
		#define MDI_CAMERA_POST_EFFECT_UNSHARP			CAM_EFFECT_DEC_UNSHARP
		#define MDI_CAMERA_POST_EFFECT_SHARPEN			CAM_EFFECT_DEC_SHARPEN
		#define MDI_CAMERA_POST_EFFECT_MORESHARPEN	CAM_EFFECT_DEC_MORESHARPEN
		#define MDI_CAMERA_POST_EFFECT_MEDIAN			CAM_EFFECT_DEC_MEDIAN
		#define MDI_CAMERA_POST_EFFECT_DILATION		CAM_EFFECT_DEC_DILATION
		#define MDI_CAMERA_POST_EFFECT_EROSION			CAM_EFFECT_DEC_EROSION
		
		#define MDI_CAMERA_ADJUSMENT_BRIGHTNESS		CAM_ADJ_DEC_BRIGHTNESS
		#define MDI_CAMERA_ADJUSMENT_CONTRAST			CAM_ADJ_DEC_CONTRAST
		#define MDI_CAMERA_ADJUSMENT_SATURATION		CAM_ADJ_DEC_SATURATION
		#define MDI_CAMERA_ADJUSMENT_HUE					CAM_ADJ_DEC_HUE
		#define MDI_CAMERA_ADJUSMENT_COLOR_R			CAM_ADJ_DEC_ADJR
		#define MDI_CAMERA_ADJUSMENT_COLOR_G			CAM_ADJ_DEC_ADJG
		#define MDI_CAMERA_ADJUSMENT_COLOR_B			CAM_ADJ_DEC_ADJB
	#else
		#define MDI_CAMERA_LCM_ROTATE_0					0
		#define MDI_CAMERA_LCM_ROTATE_90					1
		#define MDI_CAMERA_LCM_ROTATE_180				2
		#define MDI_CAMERA_LCM_ROTATE_270				3
		#define MDI_CAMERA_LCM_ROTATE_0_MIRROR			4
		#define MDI_CAMERA_LCM_ROTATE_90_MIRROR		5
		#define MDI_CAMERA_LCM_ROTATE_180_MIRROR		6
		#define MDI_CAMERA_LCM_ROTATE_270_MIRROR		7

		#define MDI_CAMERA_POST_EFFECT_NORMAL			0
		#define MDI_CAMERA_POST_EFFECT_GRAYSCALE		1
		#define MDI_CAMERA_POST_EFFECT_SEPIA			2
		#define MDI_CAMERA_POST_EFFECT_SEPIAGREEN		3
		#define MDI_CAMERA_POST_EFFECT_SEPIABLUE		4
		#define MDI_CAMERA_POST_EFFECT_COLORINV		5
		#define MDI_CAMERA_POST_EFFECT_GRAYINV			6
		#define MDI_CAMERA_POST_EFFECT_WATERCOLOR		7
		#define MDI_CAMERA_POST_EFFECT_LIGHTBLUR		8
		#define MDI_CAMERA_POST_EFFECT_BLUR				9
		#define MDI_CAMERA_POST_EFFECT_STRONGBLUR		10
		#define MDI_CAMERA_POST_EFFECT_UNSHARP			11
		#define MDI_CAMERA_POST_EFFECT_SHARPEN			12
		#define MDI_CAMERA_POST_EFFECT_MORESHARPEN	13
		#define MDI_CAMERA_POST_EFFECT_MEDIAN			14
		#define MDI_CAMERA_POST_EFFECT_DILATION		15
		#define MDI_CAMERA_POST_EFFECT_EROSION			16	

		#define MDI_CAMERA_ADJUSMENT_BRIGHTNESS		0
		#define MDI_CAMERA_ADJUSMENT_CONTRAST			1
		#define MDI_CAMERA_ADJUSMENT_SATURATION		2
		#define MDI_CAMERA_ADJUSMENT_HUE					3
		#define MDI_CAMERA_ADJUSMENT_COLOR_R			4
		#define MDI_CAMERA_ADJUSMENT_COLOR_G			5
		#define MDI_CAMERA_ADJUSMENT_COLOR_B			6	
	#endif

#else
	/* define for PC_Simulator, totaly abstract from media task */
	#define MDI_CAMERA_EV_P4								0
	#define MDI_CAMERA_EV_P3								1
	#define MDI_CAMERA_EV_P2								2
	#define MDI_CAMERA_EV_P1								3
	#define MDI_CAMERA_EV_0									4
	#define MDI_CAMERA_EV_N1								5
	#define MDI_CAMERA_EV_N2								6
	#define MDI_CAMERA_EV_N3								7
	#define MDI_CAMERA_EV_N4								8	
	#define MDI_CAMERA_EV_COUNT							9	

	#define MDI_CAMERA_EFFECT_NORMAL						0
	#define MDI_CAMERA_EFFECT_GRAYSCALE 				1	
	#define MDI_CAMERA_EFFECT_SEPIA						2
	#define MDI_CAMERA_EFFECT_SEPIA_GREEN				3
	#define MDI_CAMERA_EFFECT_SEPIA_BLUE				4
	#define MDI_CAMERA_EFFECT_COLOR_INVERT				5
	#define MDI_CAMERA_EFFECT_GRAY_INVERT				6
	#define MDI_CAMERA_EFFECT_BLACKBOARD				7
	#define MDI_CAMERA_EFFECT_WHITEBOARD				8
	#define MDI_CAMERA_EFFECT_COPPER_CARVING			8
	#define MDI_CAMERA_EFFECT_BLUE_CARVING				10
	#define MDI_CAMERA_EFFECT_EMBOSSMENT				11
	#define MDI_CAMERA_EFFECT_CONTRAST					12
	#define MDI_CAMERA_EFFECT_JEAN						13
	#define MDI_CAMERA_EFFECT_SKETCH						14
	#define MDI_CAMERA_EFFECT_OIL							15
	#define MDI_CAMERA_EFFECT_COUNT						16

	#define MDI_CAMERA_EFFECT_ANTIQUE                                  17
       #define  MDI_CAMERA_EFFECT_REDRISH                                 18
      #define MDI_CAMERA_EFFECT_GREENRISH                              19
      #define MDI_CAMERA_EFFECT_BLUERISH                                 20
      #define MDI_CAMERA_EFFECT_BLACKWHITE                            21
      #define MDI_CAMERA_EFFECT_NEGATIVENESS                        22	

	#define MDI_CAMERA_WB_AUTO								0	
	#define MDI_CAMERA_WB_DAYLIGHT						1		
	#define MDI_CAMERA_WB_TUNGSTEN						2	
	#define MDI_CAMERA_WB_FLUORESCENT					3	
	#define MDI_CAMERA_WB_CLOUD							4
	#define MDI_CAMERA_WB_INCANDESCENCE 				5
	#define MDI_CAMERA_WB_COUNT							6

	#define MDI_CAMERA_BANDING_50HZ 						0
	#define MDI_CAMERA_BANDING_60HZ 						1
	#define MDI_CAMERA_BANDING_COUNT 					2

	#define MDI_CAMERA_JPG_QTY_FINE						0
	#define MDI_CAMERA_JPG_QTY_HIGH						1
	#define MDI_CAMERA_JPG_QTY_NORMAL					2
	#define MDI_CAMERA_JPG_QTY_LOW						3
	#define MDI_CAMERA_JPG_QTY_COUNT						4

	#define MDI_CAMERA_FLASH_OFF							0
	#define MDI_CAMERA_FLASH_ON							1
	#define MDI_CAMERA_FLASH_AUTO							2
	#define MDI_CAMERA_FLASH_REDEYE						3

	#define MDI_CAMERA_AE_METER_AUTO						0
	#define MDI_CAMERA_AE_METER_SPOT						1
	#define MDI_CAMERA_AE_METER_CENTER					2
	#define MDI_CAMERA_AE_METER_AVERAGE					3

	#define MDI_CAMERA_ISO_AUTO							0
	#define MDI_CAMERA_ISO_100								1
	#define MDI_CAMERA_ISO_200								2
	#define MDI_CAMERA_ISO_400								3

	#define MDI_CAMERA_SHOT_MODE_AUTO					0
	#define MDI_CAMERA_SHOT_MODE_SHUTTER_PRI			1
	#define MDI_CAMERA_SHOT_MODE_APERTURE_PRI			2
	#define MDI_CAMERA_SHOT_MODE_PORTRAIT				3
	#define MDI_CAMERA_SHOT_MODE_LANDSCAPE				4
	#define MDI_CAMERA_SHOT_MODE_SPORT					5
	#define MDI_CAMERA_SHOT_MODE_FLOWER					6
	#define MDI_CAMERA_SHOT_MODE_NIGHT					7

	#define MDI_CAMERA_PREVIEW_ROTATE_0					0
	#define MDI_CAMERA_PREVIEW_ROTATE_90				1
	#define MDI_CAMERA_PREVIEW_ROTATE_180				2
	#define MDI_CAMERA_PREVIEW_ROTATE_270				3
	#define MDI_CAMERA_PREVIEW_ROTATE_0_MIRROR		4
	#define MDI_CAMERA_PREVIEW_ROTATE_90_MIRROR		5
	#define MDI_CAMERA_PREVIEW_ROTATE_180_MIRROR		6
	#define MDI_CAMERA_PREVIEW_ROTATE_270_MIRROR		7

	#define MDI_CAMERA_LCM_ROTATE_0					0
	#define MDI_CAMERA_LCM_ROTATE_90					1
	#define MDI_CAMERA_LCM_ROTATE_180				2
	#define MDI_CAMERA_LCM_ROTATE_270				3
	#define MDI_CAMERA_LCM_ROTATE_0_MIRROR			4
	#define MDI_CAMERA_LCM_ROTATE_90_MIRROR		5
	#define MDI_CAMERA_LCM_ROTATE_180_MIRROR		6
	#define MDI_CAMERA_LCM_ROTATE_270_MIRROR		7

	#define MDI_CAMERA_POST_EFFECT_NORMAL			0
	#define MDI_CAMERA_POST_EFFECT_GRAYSCALE		1
	#define MDI_CAMERA_POST_EFFECT_SEPIA			2
	#define MDI_CAMERA_POST_EFFECT_SEPIAGREEN		3
	#define MDI_CAMERA_POST_EFFECT_SEPIABLUE		4
	#define MDI_CAMERA_POST_EFFECT_COLORINV		5
	#define MDI_CAMERA_POST_EFFECT_GRAYINV			6
	#define MDI_CAMERA_POST_EFFECT_WATERCOLOR		7
	#define MDI_CAMERA_POST_EFFECT_LIGHTBLUR		8
	#define MDI_CAMERA_POST_EFFECT_BLUR				9
	#define MDI_CAMERA_POST_EFFECT_STRONGBLUR		10
	#define MDI_CAMERA_POST_EFFECT_UNSHARP			11
	#define MDI_CAMERA_POST_EFFECT_SHARPEN			12
	#define MDI_CAMERA_POST_EFFECT_MORESHARPEN	13
	#define MDI_CAMERA_POST_EFFECT_MEDIAN			14
	#define MDI_CAMERA_POST_EFFECT_DILATION		15
	#define MDI_CAMERA_POST_EFFECT_EROSION			16	

	#define MDI_CAMERA_ADJUSMENT_BRIGHTNESS		0
	#define MDI_CAMERA_ADJUSMENT_CONTRAST			1
	#define MDI_CAMERA_ADJUSMENT_SATURATION		2
	#define MDI_CAMERA_ADJUSMENT_HUE					3
	#define MDI_CAMERA_ADJUSMENT_COLOR_R			4
	#define MDI_CAMERA_ADJUSMENT_COLOR_G			5
	#define MDI_CAMERA_ADJUSMENT_COLOR_B			6	

#endif /* MMI_ON_HARDWARE_P */


/***************************************************************************** 
* Typedef
*****************************************************************************/
/* preview on which LCM */
typedef enum {
	MDI_CAMERA_PREVIEW_LCM_MAINLCD = 0,
	MDI_CAMERA_PREVIEW_LCM_SUBLCD,
	MDI_CAMERA_PREVIEW_LCM_COUNT
} mdi_camera_preview_lcm_enum;

/* flash, led highlight */
typedef struct {
	U8 r;
	U8 g;
	U8 b;
} mdi_camera_flash_struct;


/* setting */
typedef struct {
	U16	wb;
	U16	ev;	
	U16	zoom;
	U16	effect;
	U16	image_qty;
	U16	image_width;
	U16	image_height;
	U16	preview_width;
	U16	preview_height;
	U16	night;
	U16	banding;
	U16	lcm;
	U16	preview_rotate;
	U16	lcm_rotate;
	U16	brightness;
	U16	saturation;
	U16	contrast;
	U16	hue;
	U16	flash;
	U16	ae_meter;
	U16	iso;
	U16	shot_mode;
	U16 	osd_rotate;	

} mdi_camera_setting_struct;


/* preview window data */ 
typedef struct {
	gdi_handle	preview_layer_handle;
	S32			preview_wnd_offset_x;
	S32			preview_wnd_offset_y;
	U16			preview_wnd_width;
	U16			preview_wnd_height;
	U32			blt_layer_flag;
	U32			preview_layer_flag;
	U16			src_key_color;
} mdi_camera_preview_struct;


/* mdi camera context */
typedef struct {
	mdi_camera_setting_struct			setting;
	mdi_camera_preview_struct			preview;
	
	U16	state;
	U16	seq_num;
	BOOL	is_preview_stopped;
	BOOL	is_tvout;
	U16	burst_cap_count;
	U16	tvout_prev_owner;

 	#if defined(MT6228) || defined(MT6229)
		 
		U8 	lcd_id;
		U16	lcm_start_x;
		U16 	lcm_start_y;				
		U16 	lcm_end_x;					
		U16 	lcm_end_y;
		U16	roi_offset_x;
		U16	roi_offset_y;		
		U32	update_layer;				
		U32 	hw_update_layer;	
		void	*frame_buf_ptr;
		U32	frame_buf_size;
	#endif  

} mdi_camera_context_struct;


#ifdef MMI_ON_HARDWARE_P
	/* define as media stask's structure */
	#define mdi_camera_jpegs_struct		jpegs_struct
#else
	/* for PC Simulator, create its own */
	typedef struct
	{
		U32	file_size[20];
		U32	file_address[20];
	}	
	mdi_camera_jpegs_struct;
#endif /* MMI_ON_HARDWARE_P */


/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
/* init */
extern MDI_RESULT mdi_camera_init(void);

/* power on/off camera module */
extern MDI_RESULT	mdi_camera_power_on(void);
extern MDI_RESULT	mdi_camera_power_off(void);


/* start/stop  preview */
extern MDI_RESULT mdi_camera_preview_start(	gdi_handle						preview_layer_handle,
														 	S32								preview_wnd_offset_x,	/* offset from lcd's left up corner */
															S32								preview_wnd_offset_y,	/* offset from lcd's left up corner */
															U16								preview_wnd_width,		/* preview window width */
															U16								preview_wnd_height,		/* preview window height */
															U32								blt_layer_flag,			/* which layer shall be blt by driver */
															U32								preview_layer_flag,		/* preview on which layer */
															gdi_color						src_key_color,				/* src key color */
															BOOL								is_tvout,					/* is display preview on tvout */
															mdi_camera_setting_struct	*setting_p);				
															
															
extern MDI_RESULT	mdi_camera_preview_stop(void);


/* capture */
extern MDI_RESULT	mdi_camera_capture_to_file(S8 *file_name,U8 continue_capture);
extern MDI_RESULT	mdi_camera_capture_to_memory(U8 **buf_pp, U32 *captured_size);

extern MDI_RESULT mdi_camera_capture_layer_to_file(U32 capture_layer_flag,	
																	S8 *filename, 
																	S32 offset_x, 
																	S32 offset_y,	
																	S32 width, 
																	S32 height,
																	U8 continue_capture);

extern MDI_RESULT mdi_camera_capture_layer_to_memory(	U32 capture_layer_flag,	
																		U8 **buf_pp, 
																		U32 *captured_size,
																		S32 offset_x, 
																		S32 offset_y,
																		S32 width,
																		S32 height);

extern MDI_RESULT mdi_camera_burst_capture_to_memory(	U32 								capture_count,																
																		mdi_camera_jpegs_struct 	*jpgs_ptr);


extern MDI_RESULT mdi_camera_non_block_burst_capture_to_memory(	U32 								capture_count,																
																						mdi_camera_jpegs_struct 	*jpgs_ptr,
																						void (*capture_result_callback)(MDI_RESULT, U16));

extern MDI_RESULT mdi_camera_stop_non_block_burst_capture(void);

/* parameters */
extern void			mdi_camera_update_para_ev(U16 cam_para);
extern void			mdi_camera_update_para_zoom(U16 cam_para);
extern void			mdi_camera_update_para_effect(U16 cam_para);
extern void			mdi_camera_update_para_wb(U16 cam_para);
extern void			mdi_camera_update_para_brightness(U16 cam_para);
extern void			mdi_camera_update_para_saturation(U16 cam_para);
extern void			mdi_camera_update_para_contrast(U16 cam_para);
extern void			mdi_camera_update_para_hue(U16 cam_para);
extern void 		mdi_camera_update_para_night(U16 cam_pra);
extern void 		mdi_camera_update_para_ev_adjust(U16 cam_pra);

extern void			mdi_camera_update_para_image_qty(U16 cam_para);
extern void			mdi_camera_update_para_image_size(U16 image_width, U16 image_height);
extern void			mdi_camera_update_para_flash(U16 cam_para);
extern void			mdi_camera_update_para_iso(U16 cam_para);
extern void			mdi_camera_update_para_ae_meter(U16 cam_para);
extern void			mdi_camera_update_para_shot_mode(U16 cam_para);

/* check state */
extern BOOL			mdi_camera_is_ready_to_capture(void);

/* post-effect */
extern void 		mdi_camera_add_post_effect(gdi_handle layer_handle, U16 effect_id);
extern void 		mdi_camera_add_post_adjustment(gdi_handle layer_handle, U16 adjustment_id, S32 value);


extern MDI_RESULT mdi_camera_encode_layer_to_jpeg(gdi_handle layer_handle, PS8 file_name);
void mdi_camera_update_para_bt(U16 cam_para);
void mdi_camera_update_para_ct(U16 cam_para);
	
#endif /* __MMI_CAMERA__ */
#endif /* _CAMERA_API_H_ */


