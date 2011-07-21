/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   usbvideo_camera_if.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file implements web camera module interface function (API)
 *
 * Author:
 * -------
 * -------
 ****************************************************************************/

#include "kal_release.h"
//#include "drv_features.h"
#if (defined(WEBCAM_SUPPORT))
#include "ts.h"
#include "usbvideo_cam_if.h"
#include "uvideos_stream_m.h"

kal_uint8 *g_usbvideo_cam_buff;

USBVIDEO_CAM_OPERATION_STATE_ENUM usbvideo_cam_operation_state = USBVIDEO_CAM_OFF_STATE;
/*************************************************************************
* FUNCTION
*	usbvideo_camera_init
*
* DESCRIPTION
*	This function init web camera module status.
*
* PARAMETERS
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void usbvideo_camera_init(void)
{
#if 0//JiaShuo Delete for webcam
	usbvideo_cam_operation_state=USBVIDEO_CAM_OFF_STATE;

	usbvideo_camera_init_attr();

	/* device function mapping */
	usbvideo_camera_func_config();
	usbvideo_image_sensor_func->usbvideo_cmd_mapping();
#if (defined(YUV_SENSOR_SUPPORT))		
    image_sensor_func_config();
#endif    
#if  (!defined(DRV_ISP_6225_SERIES))
	GPTI_GetHandle(&usbvideo_camera_jpeg_timer_handle);   
#endif	
#endif
}	/* usbvideo_camera_init() */

/*************************************************************************
* FUNCTION
*	usbvideo_camera_power_on
*
* DESCRIPTION
*	This function turn the power of web camera module on.
*
* PARAMETERS
*	None
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_bool usbvideo_camera_power_on(void)
{
	if (usbvideo_cam_operation_state==USBVIDEO_CAM_OFF_STATE)
	{
		usbvideo_cam_operation_state=USBVIDEO_CAM_STANDBY_STATE;
		if(uvideos_CameraPowerUp())
			return KAL_TRUE;
		else
			return KAL_FALSE;
	}
	else
		ASSERT(0);
	return KAL_FALSE;		
}	/* usbvideo_camera_power_on() */


/*************************************************************************
* FUNCTION
*	usbvideo_camera_power_off
*
* DESCRIPTION
*	This function turn off the power of web camera module.
*  complete or not.
*
* PARAMETERS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void usbvideo_camera_power_off(void)
{
	usbvideo_cam_operation_state=USBVIDEO_CAM_OFF_STATE;
	uvideos_CameraPowerdown();
}	/* usbvideo_camera_power_off() */

void init_working_mem(void)
{
#if (defined(DRV_ISP_6219_SERIES)||defined(DRV_ISP_6227_SERIES)||defined(DRV_ISP_6225_SERIES))	
	intmem_init((kal_uint32 *) g_usbvideo_camera_data.intmem_start_address,
					g_usbvideo_camera_data.intmem_size);
	extmem_init((kal_uint32 *) g_usbvideo_camera_data.extmem_start_address,
					g_usbvideo_camera_data.extmem_size);
#endif					
}

void deinit_working_mem(void)
{
#if (defined(DRV_ISP_6219_SERIES)||defined(DRV_ISP_6227_SERIES)||defined(DRV_ISP_6225_SERIES))		
	intmem_deinit();
	extmem_deinit();
#endif	
}

void config_usbvideo_preview_jpeg(usbvideo_camera_process_struct* usbvideo_param)
{
}/* config_usbvideo_preview_jpeg() */

/*************************************************************************
* FUNCTION
*	usbvideo_camera_preview
*
* DESCRIPTION
*	This function starts camera preview
*
* PARAMETERS
*	usbvideo_data : the parameters for camera preview process
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_bool usbvideo_camera_preview(usbvideo_camera_process_struct* usbvideo_param)
{

	if (usbvideo_cam_operation_state != USBVIDEO_CAM_STANDBY_STATE)
		ASSERT(0);

	uvideos_CameraPreview();
	return KAL_TRUE;						
}

/*************************************************************************
* FUNCTION
*	exit_usbvideo_camera_preview
*
* DESCRIPTION
*	This function exit camera preview
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void exit_usbvideo_camera_preview(void)
{
	uvideos_CameraStopPreview();
#if 0//JiaShuo Delete for webcam
	if (sensor_err_check<0)
		return ;
	
	if (usbvideo_cam_operation_state==USBVIDEO_CAM_PREVIEW_CHECK_STATE)
	{
   	kal_uint32 save_irq_mask;

     	save_irq_mask=SaveAndSetIRQMask();
   	usbvideo_cam_operation_state=USBVIDEO_CAM_STANDBY_STATE;
  	   RestoreIRQMask(save_irq_mask);
#if (!defined(YUV_SENSOR_SUPPORT))	
   	ae_on_off(KAL_FALSE);
   	awb_on_off(KAL_FALSE);
#endif   	
#if  (!defined(DRV_ISP_6225_SERIES))
      if(jpeg_encoder_operation_state==JPEG_ENCODER_ENCODING_STATE)
      {
   		kal_uint32 i;      	
   		/* wait until JPEG encode completely */
   		for (i=0;i<20;i++)
   		{
   			kal_sleep_task(20);
				if ((jpeg_encoder_encode_result & JPEG_ENCODE_STALL)||
					 (jpeg_encoder_encode_result & JPEG_ENCODE_DONE))
   				i=100;
   		}
   		DISABLE_CMOS_SESNOR;
   		DISABLE_VIEW_FINDER_MODE;
#if (defined(DRV_ISP_6219_SERIES)||defined(DRV_ISP_6227_SERIES))
   		RESZ_Stop(scene_id);
   		RESZ_Close(scene_id);

   		clean_imgprc_coefficient(scene_id);
   		IMGPROC_Stop(scene_id);
   		IMGPROC_Close(scene_id);

   		stop_image_dma(scene_id);
   		IMGDMA_Close(scene_id);

   		intmem_deinit();
   		extmem_deinit();
#elif (defined(DRV_ISP_6228_SERIES)||defined(DRV_ISP_6229_SERIES))      
			close_image_data_path(scene_id);
#endif      
      }
#elif (defined(DRV_ISP_6225_SERIES))	
       		DISABLE_CMOS_SESNOR;
       		DISABLE_VIEW_FINDER_MODE;		           
            RESZ_Stop(scene_id); 
            RESZ_Close(scene_id);
            
            intmem_deinit();
            extmem_deinit();
#endif      
      
   }
   else if (usbvideo_cam_operation_state!=USBVIDEO_CAM_STANDBY_STATE)
      ASSERT(0);
#if (!defined(DRV_ISP_6225_SERIES))			                 
   jpeg_codec_power_down();
#endif
#endif
}	/* exit_usbvideo_camera_preview() */

/*************************************************************************
* FUNCTION
*	usbvideo_camera_capture
*
* DESCRIPTION
*	This function start camera capture
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_bool usbvideo_camera_capture(usbvideo_camera_process_struct *usbvideo_param)
{
#if 0//JiaShuo Delete for webcam
	if (sensor_err_check<0)
		return KAL_FALSE;
	
	if (usbvideo_cam_operation_state != USBVIDEO_CAM_STANDBY_STATE)
		ASSERT(0);

	g_usbvideo_cam_buff = USBVideo_Get_Still_Buffer();		
	if(g_usbvideo_cam_buff!=NULL)
	{	
   	g_usbvideo_cam_size = usbvideo_param->target_size_enum;
   	g_usbvideo_camera_data.target_width = STILL_SIZE[g_usbvideo_cam_size-1].width;
   	g_usbvideo_camera_data.target_height = STILL_SIZE[g_usbvideo_cam_size-1].height;
   	g_usbvideo_camera_data.target_buffer_size = STILL_SIZE[g_usbvideo_cam_size-1].max_frame_size;

		ENABLE_CMOS_SESNOR;
		config_usbvideo_preview_jpeg(usbvideo_param);
		usbvideo_cam_operation_state=USBVIDEO_CAM_CAPTURE_STATE;
		// Update attr
		usbvideo_camera_update_attr();	
		SET_CAMERA_CAPTURE_MODE;
		SET_CAMERA_FRAME_RATE(0);
		ENABLE_VIEW_FINDER_MODE;
		return KAL_TRUE;		
	}
#endif	
	return KAL_TRUE;
}	/* usbvideo_camera_capture() */

/*************************************************************************
* FUNCTION
*	exit_usbvideo_camera_capture
*
* DESCRIPTION
*	This function exit camera capture
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void exit_usbvideo_camera_capture(void)
{
}/* usbvideo_camera_capture() */

#if  (!defined(DRV_ISP_6225_SERIES))
/*************************************************************************
* FUNCTION
*	usbvideo_camera_frame_ready
*
* DESCRIPTION
*	This function complete frame(HISR) and get next video buffer
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void usbvideo_camera_frame_ready(void)
{
}

#elif (defined(DRV_ISP_6225_SERIES))
void usbvideo_camera_frame_ready(void)
{
}
#endif  // MT6225 usbvideo_camera_frame_ready
    
/*************************************************************************
 * Init / Update Attribute for ISP
 *************************************************************************/
void usbvideo_camera_init_attr(void)
{
#if 0//JiaShuo Delete for webcam
	kal_uint32 i;
	for (i=0;i<GET_ATTR_PU_IDX(USBVIDEO_ATTRIBUTE_PU_MAX);i++)		
	{
		usbvideo_attr_PU_value_list[i] = USBVIDEO_ATTR_PARA.attr_info[i].def;
	}
	/* CT Part , Fix(Default) Attributes */
	usbvideo_cam_command_CT_map[GET_ATTR_CT_IDX(USBVIDEO_ATTRIBUTE_FOCAL_MIN)] = USBVIDEO_CAM_ATTR_FIX;
	usbvideo_cam_command_CT_map[GET_ATTR_CT_IDX(USBVIDEO_ATTRIBUTE_FOCAL_MAX)] = USBVIDEO_CAM_ATTR_FIX;
	usbvideo_cam_command_CT_map[GET_ATTR_CT_IDX(USBVIDEO_ATTRIBUTE_FOCAL_LENGTH)] = USBVIDEO_CAM_ATTR_FIX;
#endif
}

kal_uint8 usbvideo_camera_update_attr(void)
{
#if 0//JiaShuo Delete for webcam
	kal_uint32 i;
	for (i=0;i<GET_ATTR_PU_IDX(USBVIDEO_ATTRIBUTE_PU_MAX);i++)		
	{
		if(usbvideo_cam_command_PU_map[i]==USBVIDEO_CAM_ATTR_ISP)
			usbvideo_attr_setting_PU_index |= (1<<i); 	
	}
	usbvideo_camera_effect_background_setting_attr();
#endif
	return KAL_TRUE;
}

/*************************************************************************
 * Set / Get Attribute for ISP
 *************************************************************************/
void usbvideo_camera_effect_background_setting_attr(void)
{
}

kal_uint8 usbvideo_camera_effect_setting_attr(USBVideo_Attr_Value *attr)
{
#if 0//JiaShuo Delete for webcam
	USBVIDEO_ATTRIBUTE_TYPE attr_type = attr->type;
	kal_uint32 attr_value = attr->value;

	// update attr PU current value
	if(attr_type>USBVIDEO_ATTRIBUTE_CT_MAX)
	{
		usbvideo_attr_PU_value_list[GET_ATTR_PU_IDX(attr_type)] = attr_value;
		usbvideo_attr_setting_PU_index |= 1<<GET_ATTR_PU_IDX(attr_type);
	}
#endif
	return KAL_TRUE;
}

kal_uint8 usbvideo_camera_effect_getting_attr(USBVideo_Attr_Value *attr)
{
#if 0//JiaShuo Delete for webcam
	USBVIDEO_ATTRIBUTE_TYPE attr_type = attr->type;
	if(attr_type>USBVIDEO_ATTRIBUTE_CT_MAX)
	{
		switch(attr->req_type)
		{
			case USBVIDEO_REQUEST_GET_CUR:
				attr->value = usbvideo_attr_PU_value_list[GET_ATTR_PU_IDX(attr_type)];
				break;
			case USBVIDEO_REQUEST_GET_MIN:
				attr->value = USBVIDEO_ATTR_PARA.attr_info[GET_ATTR_PU_IDX(attr->type)].min;
				break;
			case USBVIDEO_REQUEST_GET_MAX:
				attr->value = USBVIDEO_ATTR_PARA.attr_info[GET_ATTR_PU_IDX(attr->type)].max;
				break;
			case USBVIDEO_REQUEST_GET_DEF:
				attr->value = USBVIDEO_ATTR_PARA.attr_info[GET_ATTR_PU_IDX(attr->type)].def;
				break;
			case USBVIDEO_REQUEST_GET_RES:
				attr->value = USBVIDEO_ATTR_PARA.attr_info[GET_ATTR_PU_IDX(attr->type)].res;
				break;
			default:
				EXT_ASSERT(0, (kal_uint32)attr->req_type, 0, 0);
				break;
		}
		return KAL_TRUE;		
	}
	else	// CT Part
	{	
		return KAL_FALSE;		
	}
#endif
	return KAL_TRUE;
}

#endif  // END_OF_WEBCAM_SUPPORT

