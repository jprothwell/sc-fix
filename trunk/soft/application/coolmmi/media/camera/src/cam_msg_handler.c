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
 *   cam_msg_handler.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes message handle functions of image module.
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
#include "lcd_if.h"
#include "lcd_sw_inc.h"
#include "fat_fs.h"
#include "custom_equipment.h"
#ifdef CAMERA_MODULE
#include "ext_camera.h"
//#include "cam_module.h" /* wufasong delete 2007.06.13 */
#endif
#ifdef JPG_ENCODE
#include "jpeg.h"
#endif

 
/* local includes */
#include "med_global.h"
#include "med_main.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "med_utility.h"
#include "cam_main.h"
#include "img_main.h"
#include "cam_motion_detect.h"

#if defined(__MED_VID_MOD__) || defined(__MED_MJPG_MOD__)
#include "vid_main.h"
#endif


#ifdef __MED_TVO_MOD__
#include "tv_out.h"
#include "tvo_main.h"
#endif

#include "mmi_trace.h"
#include "media_others.h"
#include "cpu_share.h"
#include "mci.h"

#ifdef __MMI_ANALOGTV__ 
extern uint8 atvif_GetCurModule(void);
#ifdef __FF_AUDIO_SET__
extern void MusicStartAudioMode();
extern void MusicEndAudioMode();
#endif	

#endif


#ifdef __MED_CAM_MOD__

#define CAM_FREE_CAPTURE_BUFFER()       if (cam_context_p->capture_buffer_p)  \
                                             med_free_ext_mem((void**)&cam_context_p->capture_buffer_p)

#define CAM_FREE_INT_BUFFER()       if (cam_context_p->intmem_start_address)  \
                                             med_free_int_mem((void**)&cam_context_p->intmem_start_address)

#define CAM_FREE_EXT_BUFFER()       if (cam_context_p->extmem_start_address)  \
                                             med_free_ext_mem((void**)&cam_context_p->extmem_start_address)


#if defined(ISP_SUPPORT)
#if MT6219_SERIES
kal_int32 cam_capture_mem[2] = {MAX_CAM_CAP_INT_MEM_SIZE, MAX_CAM_CAP_EXT_MEM_SIZE};
kal_int32 cam_preview_mem[2] = {3840, 0};
#else
kal_int32 cam_capture_mem[2] = {MAX_CAM_CAP_INT_MEM_SIZE, MAX_CAM_CAP_EXT_MEM_SIZE};
kal_int32 cam_preview_mem[2] = {46080, 0};
#endif
 
kal_uint8 cam_rotate_map[CAM_NO_OF_IMAGE_ROTATE]=
{
   IMAGE_NORMAL,
   IMAGE_NORMAL,
   IMAGE_HV_MIRROR,
   IMAGE_NORMAL,
   IMAGE_H_MIRROR,
   IMAGE_NORMAL,
   IMAGE_V_MIRROR,
   IMAGE_NORMAL
};

 
kal_uint8 cam_wb_map[CAM_NO_OF_WB]=
{
   0,
   DAYLIGHT,
   TUNGSTEN,
   FLUORESCENT,
   CLOUD,
   INCANDESCENCE
};

#endif


#if defined (CAMERA_MODULE)

ext_camera_para_struct cam_param;
CAM_MSG_STRUC cam_param_lily;//chenhe add for jasperII
lcd_frame_update_struct lcd_param;

#elif defined (ISP_SUPPORT)

camera_capture_mem_struct capture_mem_param;
jpeg_encode_process_struct jpg_encode;

#endif



/*************************************************************************
* FUNCTION
*	cam_set_flash
*
* DESCRIPTION
*	This function is to set the flash. 
*
* PARAMETERS
*	kal_int16 value
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int16 cam_set_flash(kal_int16 flash_mode)
{
#if defined (CAMERA_MODULE)
      ext_camera_set_parameter(cam_command_map[CAM_PARAM_FLASH],
                              flash_mode);
#endif

   return MED_RES_OK;
}

/*************************************************************************
* FUNCTION
*	cam_set_flash_req_hdlr
*
* DESCRIPTION
*	This function is to handle camera set flash request. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void cam_set_flash_req_hdlr(ilm_struct *ilm_ptr)
{
   media_cam_set_flash_req_struct *req_p;

   cam_context_p->src_mod = ilm_ptr->src_mod_id;

   req_p = (media_cam_set_flash_req_struct*)ilm_ptr->local_para_ptr;

   cam_set_flash(req_p->flash_mode);
      
}


#if defined (CAMERA_MODULE)

void cam_preview_check(void* arg);


//This function is used for touchpanel
BOOL GetCameraPreviewState(void)
{
	if(CAM_IN_STATE(CAM_PREVIEW))
		return TRUE;
	else
		return FALSE;
}


/*************************************************************************
* FUNCTION
*	cam_power_up_check
*
* DESCRIPTION
*	This function is to check the power up state. 
*
* PARAMETERS
*	void* arg
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void FreeMediaMemory(unsigned long base);

void cam_power_up_check(void* arg)
{ 
	mmi_trace(g_sw_CAM, "Excute funs:cam_power_up_check() cam_context_p->state =%d",cam_context_p->state);
   if(CAM_IN_STATE(CAM_POWER_UP))
   {
      if(ext_camera_reset_status())
      {
         CAM_ENTER_STATE(CAM_READY);
         cam_send_ready_ind(MED_RES_OK);
         CAM_SET_EVENT(CAM_EVT_POWER_UP);
      }
      else
      {
         ext_camera_power_off();
#ifdef CAMERA_MODULE_WITH_LCD
         if(cam_context_p->osd_buffer_p)
         {
      	   med_free_ext_mem((void**)&cam_context_p->osd_buffer_p);
         }
#endif

         cam_context_p->power_up_check_conut++;
         if(cam_context_p->power_up_check_conut>MAX_POWER_UP_CHECK_COUNT)
         {
            /* to release the memory allocated in MED_MODE_BUFFER mode */
//            CAM_FREE_CAPTURE_BUFFER();
			if((unsigned long *)cam_context_p->capture_buffer_p != NULL)
			{
				FreeMediaMemory((unsigned long)cam_context_p->capture_buffer_p); //add by WeiD
				cam_context_p->capture_buffer_p = (kal_uint32)NULL;
			}
			
            CAM_SET_EVENT(CAM_EVT_POWER_UP);
            /* power up failed */
            CAM_ENTER_STATE(CAM_IDLE);
            cam_send_ready_ind(MED_RES_FAIL);
         }
         else
         {
#ifdef CAMERA_MODULE_WITH_LCD
            cam_context_p->osd_buffer_p=(kal_uint32)med_alloc_ext_mem(LCD_WIDTH*LCD_HEIGHT*2);
            if(cam_context_p->osd_buffer_p)
            {
            	ext_camera_set_osd_buffer(cam_context_p->osd_buffer_p, LCD_WIDTH*LCD_HEIGHT*2);
            }
#endif
            ext_camera_power_on();
            med_start_timer(CAM_POWER_UP_TIMER, CAM_POWER_UP_INIT_TIME, cam_power_up_check, 0);
         }
      }
   }
   else if(CAM_IN_STATE(CAM_START_PREVIEW))
   {
      if(ext_camera_reset_status())
      {
         cam_context_p->preview_check_conut=0;
         cam_context_p->preview_retry_conut++;
         cam_set_night_mode((kal_int16)cam_context_p->night_mode);
		 //add by WeiD

	  cam_set_quality(cam_context_p->image_quality);
//add by WeiD	 
         ext_camera_preview(&cam_param, &lcd_param);
	  cam_set_zoom_factor(cam_context_p->zoom_factor);
	  cam_set_contrast(cam_context_p->contrast_level);
	  cam_set_effect(cam_context_p->image_effect);
	  cam_set_brightness(cam_context_p->brightness_level);
	  cam_set_banding(cam_context_p->banding_freq);
//add by WeiD	 
         cam_set_flash(cam_context_p->flash_mode);
         med_start_timer(CAM_READY_POLLING_TIMER, CAM_PREVIEW_POLLING_TIME, cam_preview_check, 0);
      }
      else
      {
         ext_camera_power_off();
#ifdef CAMERA_MODULE_WITH_LCD
         if(cam_context_p->osd_buffer_p)
         {
      	   med_free_ext_mem((void**)&cam_context_p->osd_buffer_p);
         }
#endif
         cam_context_p->power_up_check_conut++;
         if(cam_context_p->power_up_check_conut>MAX_POWER_UP_CHECK_COUNT)
         {
            /* to release the memory allocated in MED_MODE_BUFFER mode */
//            CAM_FREE_CAPTURE_BUFFER();
			if((unsigned long *)cam_context_p->capture_buffer_p != NULL)
			{
				FreeMediaMemory((unsigned long)cam_context_p->capture_buffer_p); //add by WeiD
				cam_context_p->capture_buffer_p = (kal_uint32)NULL;
			}
            /* preview really fail, return to MMI */
            cam_set_result(MED_RES_FAIL);
            CAM_ENTER_STATE(CAM_IDLE);
            CAM_SET_EVENT(CAM_EVT_PREVIEW);
         }
         else
         {
#ifdef CAMERA_MODULE_WITH_LCD
            cam_context_p->osd_buffer_p=(kal_uint32)med_alloc_ext_mem(LCD_WIDTH*LCD_HEIGHT*2);
            if(cam_context_p->osd_buffer_p)
            {
            	ext_camera_set_osd_buffer(cam_context_p->osd_buffer_p, LCD_WIDTH*LCD_HEIGHT*2);
            }
#endif
            ext_camera_power_on();
            med_start_timer(CAM_POWER_UP_TIMER, CAM_POWER_UP_INIT_TIME, cam_power_up_check, 0);
         }
      }
   }
#ifdef  __MED_MJPG_MOD__
   else if(VID_IN_STATE(VID_START_PREVIEW))
   {
      if(ext_camera_reset_status())
      {
         cam_context_p->preview_check_conut=0;
         cam_context_p->preview_retry_conut++;
         cam_set_night_mode((kal_int16)cam_context_p->night_mode);

//add by WeiD
	  cam_set_quality(cam_context_p->image_quality);
//add by WeiD	 
         ext_camera_preview(&cam_param, &lcd_param);
	  cam_set_zoom_factor(cam_context_p->zoom_factor);
	  cam_set_banding(cam_context_p->banding_freq);
	  cam_set_contrast(cam_context_p->contrast_level);
	  cam_set_effect(cam_context_p->image_effect);
	  cam_set_brightness(cam_context_p->brightness_level);
//add by WeiD	 
		 
         cam_set_flash(cam_context_p->flash_mode);
         med_start_timer(CAM_READY_POLLING_TIMER, CAM_PREVIEW_POLLING_TIME, vid_preview_check, 0);
      }
      else
      {
         ext_camera_power_off();
#ifdef CAMERA_MODULE_WITH_LCD
         if(cam_context_p->osd_buffer_p)
         {
      	   med_free_ext_mem((void**)&cam_context_p->osd_buffer_p);
         }
#endif
         cam_context_p->power_up_check_conut++;
         if(cam_context_p->power_up_check_conut>MAX_POWER_UP_CHECK_COUNT)
         {
            /* preview really fail, return to MMI */
            vid_set_result(MED_RES_FAIL);
            VID_ENTER_STATE(VID_IDLE);
            CAM_ENTER_STATE(CAM_IDLE);
            VID_SET_EVENT(VID_EVT_PREVIEW);
         }
         else
         {
#ifdef CAMERA_MODULE_WITH_LCD
            cam_context_p->osd_buffer_p=(kal_uint32)med_alloc_ext_mem(LCD_WIDTH*LCD_HEIGHT*2);
            if(cam_context_p->osd_buffer_p)
            {
            	ext_camera_set_osd_buffer(cam_context_p->osd_buffer_p, LCD_WIDTH*LCD_HEIGHT*2);
            }
#endif
            ext_camera_power_on();
            med_start_timer(CAM_POWER_UP_TIMER, CAM_POWER_UP_INIT_TIME, cam_power_up_check, 0);
         }
      }
   }

#endif
      
}

/*************************************************************************
* FUNCTION
*	cam_power_up_req_hdlr
*
* DESCRIPTION
*	This function is to handle power up request. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void cam_power_up_req_hdlr(ilm_struct *ilm_ptr)
{
	INT32 result=0;
	CAM_ENTER_STATE(CAM_IDLE);
	if (1)
	{
#ifdef __MMI_ANALOGTV__  
//xiaoyifeng for atv
    if (atvif_GetCurModule() == 1) //analogTV
    {
        //MCI_AnalogTvPowerUp();//open the analogTV function
        result=MCI_CamPowerUp(0,cam_send_md_work_ind);
    	#ifdef __FF_AUDIO_SET__
//    	MusicStartAudioMode();
    	#endif	
    }
    else				
        result=MCI_CamPowerUp(0,cam_send_md_work_ind);
#else            
        result=MCI_CamPowerUp(0,cam_send_md_work_ind);
#endif
		CAM_ENTER_STATE(CAM_POWER_UP);
		CAM_SET_EVENT(CAM_EVT_POWER_UP);
		mmi_trace(1,"cam_power_up_req_hdlr,1111111,result is %d",result);

	}
	else if(CAM_IN_STATE(CAM_READY))
	{
		cam_send_ready_ind(MED_RES_OK);
		CAM_SET_EVENT(CAM_EVT_POWER_UP);
		CAM_ENTER_STATE(CAM_POWER_UP);
	}
	
	cam_set_result(result);
	cam_context_p->delay_power_down=KAL_FALSE;
	mmi_trace(1,"cam_power_up_req_hdlr,CAM_IN_STATE(CAM_READY) is %d,CAM_IN_STATE(CAM_IDLE) is %d",CAM_IN_STATE(CAM_READY),CAM_IN_STATE(CAM_IDLE));

}

/*************************************************************************
* FUNCTION
*	cam_power_down
*
* DESCRIPTION
*	This function is to power down extern camera module. 
*
* PARAMETERS
*	void* arg
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void cam_power_down(void* arg)
{
	mmi_trace(1,"cam_power_down");
#ifndef WIN32
   if(CAM_IN_STATE(CAM_READY) || CAM_IN_STATE(CAM_POWER_UP)|| CAM_IN_STATE(CAM_CAPTURE))
   {
      ext_camera_power_off();
#ifdef CAMERA_MODULE_WITH_LCD
      if(cam_context_p->osd_buffer_p)
      {
      	med_free_ext_mem((void**)&cam_context_p->osd_buffer_p);
      }
#endif
      /* to release the memory allocated in MED_MODE_BUFFER mode */
//      CAM_FREE_CAPTURE_BUFFER();
		if((unsigned long *)cam_context_p->capture_buffer_p != NULL)
		{
			FreeMediaMemory((unsigned long)cam_context_p->capture_buffer_p); //add by WeiD
			cam_context_p->capture_buffer_p = (kal_uint32)NULL;
		}
      CAM_ENTER_STATE(CAM_IDLE);
      cam_context_p->delay_power_down=KAL_FALSE;
   }
#endif
}

/*************************************************************************
* FUNCTION
*	cam_power_down_req_hdlr
*
* DESCRIPTION
*	This function is to handle power down request. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void cam_power_down_req_hdlr(ilm_struct *ilm_ptr)
{
   media_cam_power_down_req_struct *req_p;

   cam_context_p->src_mod = ilm_ptr->src_mod_id;

   req_p = (media_cam_power_down_req_struct*)ilm_ptr->local_para_ptr;

   if(CAM_IN_STATE(CAM_PREVIEW)||CAM_IN_STATE(CAM_START_PREVIEW))
   {
#ifndef WIN32
      med_stop_timer(CAM_READY_POLLING_TIMER);
#ifdef __MMI_ANALOGTV__ 
//xiaoyifeng for atv
    if (atvif_GetCurModule() == 1) //analogTV
    {
#ifdef __FF_AUDIO_SET__
    //    MusicEndAudioMode();
#endif	
		exit_ext_camera_preview();
//           MCI_AnalogTvPowerDown();//exit analogTV preview state
    }
    else		
    {
          exit_ext_camera_preview();
    }
#else
      exit_ext_camera_preview();
#endif
#endif
      CAM_ENTER_STATE(CAM_READY);
   }

   if(!CAM_IN_STATE(CAM_IDLE))
   {
	if((unsigned long *)cam_context_p->capture_buffer_p != NULL)
	{
		FreeMediaMemory((unsigned long)cam_context_p->capture_buffer_p); //add by WeiD
		cam_context_p->capture_buffer_p = (kal_uint32)NULL;
	}
      med_stop_timer(CAM_POWER_UP_TIMER);
      if (req_p->delay_time>0)
      {
         //cam_context_p->delay_power_down=KAL_TRUE;
         med_start_timer(CAM_POWER_DOWN_DELAY_TIMER, req_p->delay_time, cam_power_down, 0);
      }
      else
      {
         cam_power_down(0);
      }   
#ifdef __MMI_ANALOGTV__ 
//xiaoyifeng for atv
	if (atvif_GetCurModule() == 1) //analogTV
	{
	//power down analogTV 
#ifdef __FF_AUDIO_SET__
	//	MusicEndAudioMode();
#endif	
	//			 MCI_AnalogTvPowerDown();
	}  
#endif
   }   
  CAM_SET_EVENT(CAM_EVT_POWER_DOWN);
 //  MCI_CamPowerDown();//chenhe add
}

/*************************************************************************
* FUNCTION
*	cam_preview_check
*
* DESCRIPTION
*	This function is to check the preview state. 
*
* PARAMETERS
*	void* arg
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void cam_preview_check(void* arg)
{ 
   TBM_ENTRY(0x2AAC);
   switch(cam_context_p->state)
   {
      case CAM_START_PREVIEW:
         if(ext_camera_ready_check()!=0xffffffff)
         {
            /* preview ok */
            CAM_ENTER_STATE(CAM_PREVIEW);
            CAM_SET_EVENT(CAM_EVT_PREVIEW);
         }
         else
         {
            /* preview fail, retry */
            cam_context_p->preview_check_conut++;
            if(cam_context_p->preview_check_conut>MAX_PREVIEW_CHECK_COUNT)
            {
               if(cam_context_p->preview_retry_conut>MAX_PREVIEW_RETRY_COUNT)
               {
                  /* preview really fail, return to MMI */
                  exit_ext_camera_preview();
                  ext_camera_power_off();
#ifdef CAMERA_MODULE_WITH_LCD
                  if(cam_context_p->osd_buffer_p)
                  {
                  	med_free_ext_mem((void**)&cam_context_p->osd_buffer_p);
                  }
#endif
                  /* to release the memory allocated in MED_MODE_BUFFER mode */
//                  CAM_FREE_CAPTURE_BUFFER();
          				if((unsigned long *)cam_context_p->capture_buffer_p != NULL)
          				{
          					FreeMediaMemory((unsigned long)cam_context_p->capture_buffer_p); //add by WeiD
          					cam_context_p->capture_buffer_p = (kal_uint32)NULL;
          				}
                  cam_set_result(MED_RES_FAIL);
                  CAM_ENTER_STATE(CAM_IDLE);
                  CAM_SET_EVENT(CAM_EVT_PREVIEW);
                  break;
               }
               else
               {
                  ext_camera_power_off();
#ifdef CAMERA_MODULE_WITH_LCD
                  if(cam_context_p->osd_buffer_p)
                  {
                  	med_free_ext_mem((void**)&cam_context_p->osd_buffer_p);
                  }
#endif
                  ext_camera_power_on();
                  cam_context_p->power_up_check_conut=0;
                  med_start_timer(CAM_POWER_UP_TIMER, CAM_POWER_UP_INIT_TIME, cam_power_up_check, 0);
                  break;
               }
            }
         
         }
         med_start_timer(CAM_READY_POLLING_TIMER, CAM_PREVIEW_POLLING_TIME, cam_preview_check, 0);
         break;

      case CAM_PREVIEW:
         //ext_camera_ready_check();//fengzb
        // med_start_timer(CAM_READY_POLLING_TIMER, CAM_PREVIEW_POLLING_TIME, cam_preview_check, 0);//fengzb
         break;
      default:
         break;

   }
   TBM_EXIT(0x2AAC);
}

/*************************************************************************
* FUNCTION
*	cam_preview_req_hdlr
*
* DESCRIPTION
*	This function is to handle camera preview request. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
//chenhe 071225 +
kal_int16 cam_set_frame(kal_int16 value)
{
	MCI_CamSetPara(CAM_PARAM_ADD_FRAME,value);
      return MED_RES_OK;                  
}
//chenhe 071225 -
INT16 mmi_getcameraFrame(void); //chenhe add
void cam_preview_req_hdlr(ilm_struct *ilm_ptr)
{
	media_cam_preview_req_struct *req_p;
	CAM_PREVIEW_STRUCT preStru;
	int32 result=-1;
		mmi_trace(g_sw_CAMERA, "cam_preview_req_hdlr1111111111111");
	if(1)
	{
	       req_p = (media_cam_preview_req_struct*)ilm_ptr->local_para_ptr;
		cam_param.image_width=req_p->image_width;
		cam_param.image_height=req_p->image_height;

		cam_param.buffer_width=req_p->buffer_width;
		cam_param.buffer_height=req_p->buffer_height;
		cam_param.preview_width=cam_context_p->preview_width=req_p->preview_width;
		cam_param.preview_height=cam_context_p->preview_height=req_p->preview_height;
		cam_param.preview_offset_x=req_p->preview_offset_x;
		cam_param.preview_offset_y=req_p->preview_offset_y;
		cam_param.white_balance_mode1=(kal_uint8)cam_wb_param_map[req_p->WB];
		cam_param.ev_value=(kal_uint8)cam_ev_param_map[req_p->exposure];
		cam_param.image_effect1=(kal_uint8)cam_effect_param_map[req_p->effect];
#ifdef CAMERA_MODULE_WITH_LCD
		cam_param.image_buffer_ptr=(kal_uint16*)cam_context_p->osd_buffer_p;
		cam_param.image_buffer_size=LCD_WIDTH*LCD_HEIGHT*2;
#else
		cam_param.image_buffer_ptr=req_p->image_buffer_p;
#endif
		cam_param.preview_rotate=req_p->rotate;
		cam_param.banding_freq=(kal_uint8) cam_banding_param_map[req_p->banding_freq];
		cam_param.snapshot_number=req_p->snapshot_number;
		cam_param.src_key_color=req_p->src_key_color;	
		cam_param.zoom_factor=req_p->zoom_factor;
		cam_param.video_mode=KAL_FALSE; /* camera preview */
		cam_param.frame_rate =req_p->frame_rate; //add  by WeiD

		lcd_param.module_id=LCD_UPDATE_MODULE_MEDIA;
		lcd_param.lcd_id=req_p->lcd_id;
		lcd_param.fb_update_mode=LCD_SW_TRIGGER_MODE;
		lcd_param.lcm_start_x=req_p->lcd_start_x;
		lcd_param.lcm_start_y=req_p->lcd_start_y;
		lcd_param.lcm_end_x=req_p->lcd_end_x;
		lcd_param.lcm_end_y=req_p->lcd_end_y;
		lcd_param.roi_offset_x=req_p->roi_offset_x;
		lcd_param.roi_offset_y=req_p->roi_offset_y;
		lcd_param.update_layer=req_p->update_layer;
		lcd_param.hw_update_layer=0;

		cam_context_p->brightness_level = req_p->brightness;
		cam_context_p->contrast_level = req_p->contrast;

		cam_context_p->preview_check_conut=0;
		cam_context_p->preview_retry_conut=0;
		cam_context_p->zoom_factor=req_p->zoom_factor;
		cam_context_p->flash_mode=req_p->flash_mode;
		cam_context_p->night_mode=req_p->night_mode;
		cam_context_p->banding_freq = req_p->banding_freq;
		cam_context_p->image_effect = req_p->effect;
		cam_context_p->wb_mode = req_p->WB;
		cam_context_p->ev_value = req_p->exposure;
		cam_context_p->image_quality = req_p->gamma; 



		cam_param_lily.Camera_struct.preview_width= cam_param.preview_width;
		cam_param_lily.Camera_struct.preview_height = cam_param.preview_height;
		cam_param_lily.Camera_struct.frame_rate =  CAMERA_NO_BLEND_PREVIEW_FRAME_DEFAULT;
		cam_param_lily.Camera_struct.only1frameflag = FALSE;

		//store info to our used message struct for sending to lily
		preStru.start_x = lcd_param.lcm_start_x;
		preStru.start_y = lcd_param.lcm_start_y;		
		preStru.end_x = lcd_param.lcm_end_x;
		preStru.end_y = lcd_param.lcm_end_y;
		preStru.image_width = cam_param.image_width;
		preStru.image_height = cam_param.image_height;	
#ifdef __MMI_ANALOGTV__ 
		//xiaoyifeng for atv		
		preStru.preview_rotate = cam_param.preview_rotate;	//xiaoyifeng add  10/11/1
#endif				
		preStru.nightmode = ((kal_int16)cam_context_p->night_mode);
		preStru.imageQuality = (cam_context_p->image_quality);
		preStru.contrast = (cam_context_p->contrast_level);
		preStru.specialEffect = (cam_context_p->image_effect);
		preStru.brightNess = (cam_context_p->brightness_level);
		preStru.whiteBlance = (cam_context_p->wb_mode);
		preStru.exposure =(cam_context_p->ev_value);
		preStru.addFrame =(mmi_getcameraFrame());
		preStru.factor = cam_param.zoom_factor;
        preStru.flashenable = cam_context_p->flash_mode;
		result=MCI_CamPreviewOpen(&preStru);
	
#if 0
		//after preview open,wo should set some parameters+
		cam_set_night_mode((kal_int16)cam_context_p->night_mode);
		cam_set_quality(cam_context_p->image_quality);
		//ext_camera_preview(&cam_param, &lcd_param);
		cam_set_zoom_factor(cam_context_p->zoom_factor);
		//cam_set_banding(cam_context_p->banding_freq);
		cam_set_contrast(cam_context_p->contrast_level);
		cam_set_effect(cam_context_p->image_effect);
		cam_set_brightness(cam_context_p->brightness_level);
		cam_set_wb(cam_context_p->wb_mode);
		cam_set_exposure(cam_context_p->ev_value);
		cam_set_frame(mmi_getcameraFrame());
		//after preview open,wo should set some parameters-
#endif
		if(result == MED_RES_OK)
		{
			CAM_ENTER_STATE(CAM_START_PREVIEW);
			CAM_SET_EVENT(CAM_EVT_PREVIEW);
			cam_set_result(MED_RES_OK);
		}
		else
		{
			CAM_SET_EVENT(CAM_EVT_PREVIEW);
			cam_set_result(MED_RES_FAIL);
		}
		
	}
}

/*************************************************************************
* FUNCTION
*	cam_stop_req_hdlr
*
* DESCRIPTION
*	This function is to handle camera stop request. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void cam_stop_req_hdlr(ilm_struct *ilm_ptr)
{
   cam_context_p->src_mod = ilm_ptr->src_mod_id;
	mmi_trace(g_sw_CAMERA, "cam_stop_req_hdlr() is status = %d",cam_context_p->state);

	MCI_CamPreviewClose();

   CAM_ENTER_STATE(CAM_READY);
   CAM_SET_EVENT(CAM_EVT_STOP);
      
}

/*************************************************************************
* FUNCTION
*	cam_capture_req_hdlr
*
* DESCRIPTION
*	This function is to handle camera capture request. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void cam_capture_req_hdlr(ilm_struct *ilm_ptr)
{
    media_cam_capture_req_struct *req_p;
    CAM_CAPTURE_STRUCT capStruc;
    //ext_camera_para_struct cam_param;
    kal_int32 result = MED_RES_OK;
    TBM_ENTRY(0x2AAA);
    cam_context_p->src_mod = ilm_ptr->src_mod_id;
    mmi_trace(1,"cam_capture_req_hdlr");
    req_p = (media_cam_capture_req_struct*)ilm_ptr->local_para_ptr;
    //store the used information into the messag struct for sending to lily
    capStruc.image_width = req_p->image_width;
    capStruc.image_height = req_p->image_height;//need to modify
    capStruc.media_mode = req_p->media_mode;
    //capStruc.image_width = 240;
    //capStruc.image_height = 320;
    result = MCI_CamCapture(&capStruc);
    mmi_trace(1,"cam_capture_req_hdlr,222,%d,%d,%d,%d",capStruc.image_width,capStruc.image_height);
    if(result == MED_RES_OK)
    {
        CAM_ENTER_STATE(CAM_CAPTURE);
        CAM_SET_EVENT(CAM_EVT_CAPTURE);
    }
    else
    {
        cam_set_result(result);
        CAM_SET_EVENT(CAM_EVT_CAPTURE);      
    }
    TBM_EXIT(0x2AAA);
}

/*************************************************************************
* FUNCTION
*	cam_capture_req_hdlr
*
* DESCRIPTION
*	This function is to handle camera capture request. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
extern INT32  MCI_CamSavePhoto (INT32  filehandle) ; //MCI_CAM_SAVE_PHOTO

void cam_save_photo_hdlr(ilm_struct *ilm_ptr)
{
	UINT32 fhd = -1;
   //ext_camera_para_struct cam_param;
	kal_int32 result = MED_RES_OK;
	cam_context_p->src_mod = ilm_ptr->src_mod_id;
	mmi_trace(1,"cam_save_photo_hdlr");
	fhd = (UINT32)ilm_ptr->local_para_ptr;

	result = MCI_CamSavePhoto (fhd);
   	mmi_trace(1,"cam_save_photo_hdlr,fhd is %d",fhd);
	if(result == MED_RES_OK)
	{
		cam_set_result(result);

		CAM_SET_EVENT(CAM_EVT_SAVE_PHOTO);      
	}
	else
	{
			cam_set_result(result);

		CAM_SET_EVENT(CAM_EVT_SAVE_PHOTO);      
	}
   TBM_EXIT(0x2AAA);
}

/*************************************************************************
* FUNCTION
*	cam_set_zoom_factor
*
* DESCRIPTION
*	This function is to set the camera zoom factor. 
*
* PARAMETERS
*	kal_int16 value
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int16 cam_set_zoom_factor(kal_int16 value)
{
	MCI_CamSetPara(CAM_PARAM_ZOOM_FACTOR,value);
	   return MED_RES_OK;
}

/*************************************************************************
* FUNCTION
*	cam_set_contrast
*
* DESCRIPTION
*	This function is to set the contrast. 
*
* PARAMETERS
*	kal_int16 value
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int16 cam_set_contrast(kal_int16 value)
{
	MCI_CamSetPara(CAM_PARAM_CONTRAST,value);
	return MED_RES_OK;
}

/*************************************************************************
* FUNCTION
*	cam_set_brightness
*
* DESCRIPTION
*	This function is to set the brightness. 
*
* PARAMETERS
*	kal_int16 value
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int16 cam_set_brightness(kal_int16 value)
{
	MCI_CamSetPara(CAM_PARAM_BRIGHTNESS,value);
	return MED_RES_OK;
}


/*************************************************************************
* FUNCTION
*	cam_set_wb
*
* DESCRIPTION
*	This function is to set the white balance value. 
*
* PARAMETERS
*	kal_int16 value
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int16 cam_set_wb(kal_int16 value)
{
	MCI_CamSetPara(CAM_PARAM_WB,value);
      return MED_RES_OK;                        
}



/*************************************************************************
* FUNCTION
*	cam_set_exposure
*
* DESCRIPTION
*	This function is to set the exposure compensation value. 
*
* PARAMETERS
*	kal_int16 value
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int16 cam_set_exposure(kal_int16 value)
{
	MCI_CamSetPara(CAM_PARAM_EXPOSURE,value);
      return MED_RES_OK;                        
}

/*************************************************************************
* FUNCTION
*	cam_set_effect
*
* DESCRIPTION
*	This function is to set the image effect. 
*
* PARAMETERS
*	kal_int16 value
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int16 cam_set_effect(kal_int16 value)
{
	MCI_CamSetPara(CAM_PARAM_EFFECT,value);
	return MED_RES_OK;
}


kal_int16 cam_set_quality(kal_int16 value)
{
   mmi_trace(g_sw_CAMERA, "Excute funs: cam_set_quality() value = %d",value);
   MCI_CamSetPara(CAM_PARAM_QUALITY,value);
	 return MED_RES_OK;
}

kal_int16 cam_set_imagesize(kal_int16 value)
{
	//MCI_CamSetPara(CAM_PARAM_IMAGESIZE,value);
	return MED_RES_OK;
}

/*************************************************************************
* FUNCTION
*	cam_set_banding
*
* DESCRIPTION
*	This function is to set the banding. 
*
* PARAMETERS
*	kal_int16 value
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int16 cam_set_banding(kal_int16 value)
{
	MCI_CamSetPara(CAM_PARAM_BANDING,value);
	return MED_RES_OK;

}

/*************************************************************************
* FUNCTION
*	cam_set_night_mode
*
* DESCRIPTION
*	This function is to set the night mode. 
*
* PARAMETERS
*	kal_int16 value
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int16 cam_set_night_mode(kal_int16 value)
{
	MCI_CamSetPara(CAM_PARAM_NIGHT_MODE,  value);
      return MED_RES_OK;                        
}

/*************************************************************************
* FUNCTION
*	cam_set_param_req_hdlr
*
* DESCRIPTION
*	This function is to handle camera set parameter request. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void cam_set_param_req_hdlr(ilm_struct *ilm_ptr)
{
   media_cam_set_param_req_struct *req_p;

   cam_context_p->src_mod = ilm_ptr->src_mod_id;

   req_p = (media_cam_set_param_req_struct*)ilm_ptr->local_para_ptr;
   MCI_CamSetPara(req_p->param_id, req_p->value);
   CAM_SET_EVENT(CAM_EVT_SET_PARAM);
      
}

/*************************************************************************
* FUNCTION
*	cam_abort_to_idle
*
* DESCRIPTION
*	This function is to abort all operations and go to idle state . 
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
void cam_abort_to_idle(void)
{
   if(!CAM_IN_STATE(CAM_IDLE))
   {
      if(CAM_IN_STATE(CAM_PREVIEW))
      {
#ifndef WIN32
         med_stop_timer(CAM_READY_POLLING_TIMER);
         exit_ext_camera_preview();
#endif
         CAM_ENTER_STATE(CAM_READY);
      }
      med_stop_timer(CAM_POWER_DOWN_DELAY_TIMER);
      cam_power_down(0);
      CAM_ENTER_STATE(CAM_IDLE);
   }   
}

void cam_md_hdlr(ilm_struct *ilm_ptr)
{
	MCI_CamDoMd();
}
// open camera motion detect
void cam_md_open_hdlr(ilm_struct *ilm_ptr)
{
	int res=-1;
	media_cam_md_open_req_struct *req_p;
	req_p = (media_cam_md_open_req_struct*)ilm_ptr->local_para_ptr;
	
	res=MCI_CamMdOpen(req_p->sensLevel, cam_send_md_work_ind);
	if(res==0)
	{
		cam_set_result(MED_RES_OK);
		CAM_SET_EVENT(CAM_EVT_POWER_UP);
	}
	else
	{
		cam_set_result(MED_RES_FAIL);
		CAM_SET_EVENT(CAM_EVT_POWER_UP);
	}
}
// close camera motion detect
void cam_md_close_hdlr(ilm_struct *ilm_ptr)
{
	MCI_CamMdClose();
    CAM_SET_EVENT(CAM_EVT_POWER_DOWN);
}

#else
#error "no such combination"
#endif

#endif /* __MED_CAM_MOD__ */

#endif /* MED_NOT_PRESENT */

