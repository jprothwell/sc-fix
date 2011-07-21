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
 *   cam_main.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes primary functions of camera module.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
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
#ifdef CAMERA_MODULE
#include "ext_camera.h"
//#include "cam_module.h" /* wufasong deleted 2007.06.13 since no this file. */
#endif
/* local includes */
#include "med_global.h"
#include "med_main.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "cam_main.h"

#include "mmi_trace.h"

#ifdef __MED_CAM_MOD__

cam_context_struct*   cam_context_p = &(med_context.cam_context);

HANDLE  Cam_Semaphore[MAX_CAM_SEM] = {0}; //new add 

/*==== FUNCTIONS ===========*/

/******************************************************************
 * FUNCTION
 *    cam_startup_hdlr
 * DESCRIPTION
 *    This function is handle startup procedure of camera module.
 * PARAMETERS
 *    
 *    
 * RETURNS
 *
 * GLOBALS AFFECTED
 *  
 *  
 ******************************************************************/
void cam_startup_hdlr( ilm_struct *ilm_ptr)
{
   cam_context_p->src_mod = ilm_ptr->src_mod_id;
#if defined (CAMERA_MODULE) && !defined(WIN32)
   ext_camera_init();
#endif
}

/******************************************************************
 * FUNCTION
 *    cam_main
 * DESCRIPTION
 *    This function is main message dispatching function of camera module.
 * PARAMETERS
 *    
 *    
 * RETURNS
 *
 * GLOBALS AFFECTED
 *  
 *  
 ******************************************************************/
void cam_main( ilm_struct *ilm_ptr)
{

	//mmi_trace(g_sw_CAM, "Excute func : cam_main(msg_id = %d)", ilm_ptr->msg_id);

   switch( ilm_ptr->msg_id )
   {
      case MSG_ID_MEDIA_CAM_POWER_UP_REQ:
	  	 mmi_trace(g_sw_CAM, "Excute cam_main MSG_ID_MEDIA_CAM_POWER_UP_REQ is TRUE");
         cam_power_up_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_MEDIA_CAM_POWER_DOWN_REQ:
	  	 mmi_trace(g_sw_CAM, "Excute cam_main MSG_ID_MEDIA_CAM_POWER_DOWN_REQ is TRUE");
         cam_power_down_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_MEDIA_CAM_PREVIEW_REQ:
	  	mmi_trace(g_sw_CAM, "Excute cam_main MSG_ID_MEDIA_CAM_PREVIEW_REQ is TRUE");
         cam_preview_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_MEDIA_CAM_STOP_REQ:
	  	mmi_trace(g_sw_CAM, "Excute cam_main MSG_ID_MEDIA_CAM_STOP_REQ is TRUE");
         cam_stop_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_MEDIA_CAM_CAPTURE_REQ:
	  	mmi_trace(g_sw_CAM, "Excute cam_main MSG_ID_MEDIA_CAM_CAPTURE_REQ is TRUE");
         cam_capture_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_MEDIA_CAM_SET_PARAM_REQ:
	  	mmi_trace(g_sw_CAM, "Excute cam_main MSG_ID_MEDIA_CAM_SET_PARAM_REQ is TRUE");
         cam_set_param_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_MEDIA_CAM_SET_FLASH_REQ:
	  	mmi_trace(g_sw_CAM, "Excute cam_main MSG_ID_MEDIA_CAM_SET_FLASH_REQ is TRUE");
         cam_set_flash_req_hdlr(ilm_ptr);
         break;
 	case MSG_ID_MEDIA_CAM_MD_REQ:
		cam_md_hdlr(ilm_ptr);
		break;
 	case MSG_ID_MEDIA_CAM_MD_OPEN_REQ:
	  	mmi_trace(g_sw_CAM, "Excute cam_main MSG_ID_MEDIA_CAM_MD_OPEN_REQ is TRUE");
		cam_md_open_hdlr(ilm_ptr);
		break;
 	case MSG_ID_MEDIA_CAM_MD_CLOSE_REQ:
	  	mmi_trace(g_sw_CAM, "Excute cam_main MSG_ID_MEDIA_CAM_MD_CLOSE_REQ is TRUE");
		cam_md_close_hdlr(ilm_ptr);
		break;
	case MSG_ID_MEDIA_CAM_SAVE_PHOTO:
	  	mmi_trace(g_sw_CAM, "Excute cam_main MSG_ID_MEDIA_CAM_SAVE_PHOTO is TRUE");
		cam_save_photo_hdlr(ilm_ptr);
		break;
      default:
         break;
   }
}



/******************************************************************
 * FUNCTION
 *    cam_init
 * DESCRIPTION
 *    This function is used to init camera module of media task.
 * PARAMETERS
 *    
 *    
 * RETURNS
 *
 * GLOBALS AFFECTED
 *  
 *  
 ******************************************************************/
kal_bool cam_init(void)
{
	mmi_trace(g_sw_CAMERA, "Excute func : cam_init()");
   cam_context_p->state = CAM_IDLE;
   cam_context_p->capture_buffer_p=0;
   cam_context_p->cam_event = kal_create_event_group("cam_events");

   cam_context_p->intmem_start_address=0;
   cam_context_p->extmem_start_address=0;
   cam_context_p->int_frame_buffer_p=0;
   return KAL_TRUE;
}

#endif /* __MED_CAM_MOD__ */
#endif /* MED_NOT_PRESENT */


