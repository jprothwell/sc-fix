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
 *   cam_api.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes primary exported functions of image module.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef MED_NOT_PRESENT


/*==== INCLUDES =========*/

/* system includes */
#include "kal_release.h" 
#include "stack_common.h"  
#include "stack_msgs.h"
#include "app_ltlcom.h"          /* task message communiction */
#include "stacklib.h"           /* basic type for dll, evshed, stacktimer */
#include "event_shed.h"          /* event scheduler */
#include "stack_timer.h"         /* stack timer */



/* global includes */
#include "l1audio.h"
#include "device.h"
#include "resource_audio.h"
#include "nvram_struct.h"
#include "nvram_user_defs.h"
#include "nvram_data_items.h"
#include "custom_nvram_editor_data_item.h"


/* local includes */
#include "med_global.h"
#include "med_main.h"
#include "aud_defs.h"   //add by weid
#include "med_struct.h"
#include "med_api.h"

#include "med_context.h"  //add by weid

#include "med_api.h"
#include "cam_main.h"

#include "mmi_trace.h"

#ifdef __MED_CAM_MOD__

static kal_int32 cam_result;

/*==== FUNCTIONS ===========*/

/*************************************************************************
* FUNCTION
*	cam_set_result
*
* DESCRIPTION
*	This function is to set return result. 
*
* PARAMETERS
*	kal_int32 result
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void cam_set_result(kal_int32 result)
{
   cam_result=result;
}

/*************************************************************************
* FUNCTION
*	media_cam_power_up
*
* DESCRIPTION
*	This function is to power on camera. 
*
* PARAMETERS
*	module_type src_mod_id
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_cam_power_up(module_type src_mod_id, kal_uint16  seq_num)
{
   cam_send_power_up_req(src_mod_id, seq_num);
//   mmi_trace(0,"before CAM_EVT_POWER_UP is TRUE");
   CAM_WAIT_EVENT(CAM_EVT_POWER_UP);
   return cam_result;
}

/*************************************************************************
* FUNCTION
*	media_cam_power_down
*
* DESCRIPTION
*	This function is to power down camera. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_int16 delay_time
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_cam_power_down(module_type src_mod_id, kal_int16 delay_time)
{
   cam_send_power_down_req(src_mod_id, delay_time);
   CAM_WAIT_EVENT(CAM_EVT_POWER_DOWN);
   return MED_RES_OK;
}

/*************************************************************************
* FUNCTION
*	media_cam_preview
*
* DESCRIPTION
*	This function is to start camera preview. 
*
* PARAMETERS
*	module_type src_mod_id
*  cam_preview_struct *preview
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void mmi_camera_sensorPwr_autoStart(void);

kal_int32 media_cam_preview(module_type src_mod_id, cam_preview_struct *preview)
{
   TBM_ENTRY(0x2AA2);
   cam_result=MED_RES_OK;
   mmi_camera_sensorPwr_autoStart();//chenhe for jessamine 2 sensor
   cam_send_preview_req(src_mod_id, preview);
 //  mmi_trace(0, "Before wait_event CAM_EVT_PREVIEW is TRUE");
   CAM_WAIT_EVENT(CAM_EVT_PREVIEW); 
	TBM_EXIT(0x2AA2);
   return cam_result;
}

/*************************************************************************
* FUNCTION
*	media_cam_stop
*
* DESCRIPTION
*	This function is to stop camera preview. 
*
* PARAMETERS
*	module_type src_mod_id
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_cam_stop(module_type src_mod_id)
{
//   mmi_trace(0, "Excute funs: media_cam_stop() is TRUE");
   cam_send_stop_req(src_mod_id);
   CAM_WAIT_EVENT(CAM_EVT_STOP);  
}

/*************************************************************************
* FUNCTION
*	media_cam_capture
*
* DESCRIPTION
*	This function is to capture the image. 
*
* PARAMETERS
*	module_type src_mod_id
*  cam_capture_struct *capture
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_cam_capture(module_type src_mod_id, cam_capture_struct *capture)
{
   TBM_ENTRY(0x2AA8);
   cam_result=MED_RES_OK;
//   mmi_trace(0,"Excute funs: media_cam_capture() is TRUE");
   cam_send_capture_req(src_mod_id, capture);
   CAM_WAIT_EVENT(CAM_EVT_CAPTURE);
   TBM_EXIT(0x2AA8);
   return cam_result;
}


/*************************************************************************
* FUNCTION
*	media_cam_set_param
*
* DESCRIPTION
*	This function is to set the parameter of camera. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint16 param_id
*  kal_int16 value
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_cam_set_param(module_type src_mod_id, kal_uint16 param_id, kal_int16 value)
{
   mmi_trace(1, "Excute funs: media_cam_set_param() is TRUE param_id = %ld ,value = %ld",param_id, value);
   cam_send_set_param_req(src_mod_id, param_id, value);
   CAM_WAIT_EVENT(CAM_EVT_SET_PARAM);  
}

/*************************************************************************
* FUNCTION
*	media_cam_set_flash
*
* DESCRIPTION
*	This function is to set the flash for camera. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_int16 flash_mode
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_cam_set_flash(module_type src_mod_id, kal_int16 flash_mode)
{
   cam_send_set_flash_req(src_mod_id, flash_mode);
}

//open camera motion detect
kal_int32 media_cam_md_open(module_type src_mod_id, kal_uint32 sensLevel, void(*md_callback)(int32))
{
 	cam_send_md_open_req( src_mod_id, sensLevel, md_callback);
	CAM_WAIT_EVENT(CAM_EVT_POWER_UP);
	return cam_result;
}
//close camera motion detect
kal_int32 media_cam_md_close(module_type src_mod_id)
{
	cam_send_md_close_req(src_mod_id);
    CAM_WAIT_EVENT(CAM_EVT_POWER_DOWN);
	return MED_RES_OK;
}
kal_int32 media_cam_save_photo(UINT32 fhd )
{
	cam_send_cam_save_photo_req(MOD_MMI,fhd);
	CAM_WAIT_EVENT(CAM_EVT_SAVE_PHOTO);
	return cam_result;
}

#endif /* __MED_CAM_MOD__ */

#endif /* MED_NOT_PRESENT */



