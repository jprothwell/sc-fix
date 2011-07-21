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
 *   vid_msg_handler.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes message handle functions of video module.
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
#include "fat_fs.h"
#include "custom_equipment.h"
#include "custom_video.h"

#include "rtc_sw.h"
#include "lcd_if.h"
#include "lcd_sw_inc.h"
/* local includes */
#include "med_global.h"
#include "med_status.h"
#include "med_main.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "med_utility.h"
//#include "med_smalloc.h"
#include "cam_main.h"
#include "vid_main.h"
#include "aud_main.h"
#include "img_main.h"

#include "di.h"

#include "gdi_include.h"			/* graphic library */

#include "stdc.h"
#include "l4dr1.h"
#include "globaldefs.h"

#include "mmi_features_video.h"	/* features */
#include "lcd_sw_rnd.h"
#include "custmenures.h"
#include "custdatares.h"			/* res handle functions */
#include "globalconstants.h"		/* key id define */
#include "fat_fs.h"					/* must after "globalconstants.h" */
#include "globaldefs.h"				/* golbal image and string id. */
#include "gui.h"						/* gui functions */
#include "wgui_categories.h"
#include "historygprot.h"			/* screen history */
#include "unicodexdcl.h"			/* unicode transform functions */
#include "eventsgprot.h"		
#include "filesystemdef.h"			/* file function */
#include "gpioinc.h"					/* led */
#include "settingdefs.h"			/* popup sound */
#include "debuginitdef.h"			/* mmi_assert */	
#include "commonscreens.h"			/* popup */
#include "keybrd.h"					/* clear key function */
#include "sublcdhistorygprot.h"	/* sublcd */
#include "mainmenudef.h"			/* multi-media icon */
#include "frameworkstruct.h"		/* for psextfuncptr */
#include "conversions.h"			/* char set conversion */
#include "phonesetupgprots.h"		/* phnsetgetdefencodingtype() */

#include "nvramtype.h"				/* nvram data type */
#include "nvramprot.h"				/* nvram access fucntions */	
#include "nvramenum.h"				/* nvram id define */

#include "med_api.h"					/* media task */
#include "med_main.h"				/* media task */

#include "lcd_sw_rnd.h"
#include "lcd_if.h"
#include "gdi_include.h"			/* graphic library */

#include "mdi_datatype.h"
#include "mdi_audio.h"				/* audio lib */
#include "mdi_video.h"				/* video lib */
#include "mdi_camera.h"				/* camera lib */
#include "cameraapp.h"		

#include "resource_video_skins.h"		/* ui custom config */
#include "vdorecstrenum.h"
#include "vdorecimgenum.h"
#include "vdoplyimgenum.h"
#include "vdoplystrenum.h"
#include "vdoplyscrenum.h"

#include "vdorecapp.h"
#include "vdorecgprot.h"
#include "vdoplyapp.h"
#include "vdoplygprot.h"     
//Added by jinzh end : 20070731
		
#include "mci.h"
extern ext_camera_para_struct_di cam_param;
extern kal_uint16 ring_buffer[AUD_RING_BUFFER_LEN];

#if (CSW_EXTENDED_API_AUDIO_VIDEO == 1)
VOID ConvertRelative2Full(UINT8 *pFullName, UINT8 *pRelativeName);
INT32 ConvertDosName2Unix(UINT8 *pUnixName, CONST UINT8 *pDosName);
#endif /* (CSW_EXTENDED_API_AUDIO_VIDEO == 1) */

#ifdef __MED_MJPG_MOD__
	#define MJPEGTRACE    1 // MJPEG trace power
#ifdef CAMERA_MODULE
#include "ext_camera.h"
//#include "cam_module.h"
#endif
/* wufasong added 2007.06.19 */
//#if defined(JPG_DECODE) || defined(JPG_ENCODE)
//#include "jpeg.h"
//#endif
#endif

#include "media_others.h"
#include "mmi_trace.h"
#include "mdi_audio.h"   //added by jinzh:20070731

#ifdef __MED_MJPG_MOD__

//lcd_frame_update_struct lcd_data;
//extern ext_camera_para_struct cam_param;
extern lcd_frame_update_struct lcd_param;

extern U8 GetHandsetInPhone();  
#ifdef __FF_AUDIO_SET__
extern void GpioSpeakerModeEnd(void);
#endif
image_para_di MJPEG;


/*************************************************************************
* FUNCTION
*   vid_stop_unfinished_audio
*
* DESCRIPTION
*   This function is to stop unfinished audio.
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_stop_unfinished_audio(void)
{
   /* if keytone is playing, stop it */
   aud_keytone_stop();

   /* if tone is playing, stop it */
   if (aud_context_p->tone_playing)
      aud_tone_stop();
   
#ifdef __MED_MMA_MOD__
   /* close all mma tasks */
   aud_mma_close_all();
#endif

   aud_stop_unfinished_process();

}



/*************************************************************************
* added by elisa for coolprofile tools to analyse the bandwidth
*/
//API for cool profile
//#define u16 UINT16
//#define u8   UINT8
//extern void hal_PXTS_SendProfilingCode(u16 code);
//#define CPMASK 0x3fff
//#define CPEXITFLAG 0x8000
//#define CP_ENTRY(id) hal_PXTS_SendProfilingCode((id) & CPMASK)
//#define CP_EXIT(id) hal_PXTS_SendProfilingCode(((id) & CPMASK) | CPEXITFLAG)

/* GLOBALS AFFECTED
*
*************************************************************************/




typedef  struct 
{
	int audio_buffer_remain_len;
	int fist_buffer_flag;
	int audio_buffer_fill_len;
	
}audio_play_control;
audio_play_control audio_buffer_control;
audio_play_control* pcontrol =&audio_buffer_control ;

/*************************************************************************
* FUNCTION
*	vid_preview_check
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
void vid_preview_check(void* arg)
{ 
   switch(vid_context_p->state)
   {
      case VID_START_PREVIEW:
         if(ext_camera_ready_check()!=0xffffffff)
         {
            /* preview ok */
            VID_ENTER_STATE(VID_PREVIEW);
            VID_SET_EVENT(VID_EVT_PREVIEW);
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
                  vid_set_result(MED_RES_FAIL);
                  VID_ENTER_STATE(VID_IDLE);
                  CAM_ENTER_STATE(CAM_IDLE);
                  VID_SET_EVENT(VID_EVT_PREVIEW);
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
         med_start_timer(CAM_READY_POLLING_TIMER, CAM_PREVIEW_POLLING_TIME, vid_preview_check, 0);
         break;

#ifdef CAMERA_MODULE_WITH_LCD
      case VID_RECORD:   
#endif
      case VID_PREVIEW:
      case VID_RECORD_PAUSED:
         ext_camera_ready_check();
         med_start_timer(CAM_READY_POLLING_TIMER, CAM_PREVIEW_POLLING_TIME, vid_preview_check, 0);
         break;
      default:
         break;

   }
}

/*************************************************************************
* FUNCTION
*	vid_rec_finish_ind
*
* DESCRIPTION
*	This function is to send video recorder finish indication to mmi
*
* PARAMETERS
*	int16 result
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/

void vid_rec_finish_ind(uint16 msg_result){
    media_vid_record_finish_ind_struct *ind_p;
    ilm_struct       *local_ilm_ptr = NULL;
   
    ind_p = (media_vid_record_finish_ind_struct*)
        construct_local_para(sizeof(media_vid_record_finish_ind_struct), TD_CTRL);  

    ind_p->result = msg_result;
    ind_p->seq_num = 0;

    local_ilm_ptr = allocate_ilm(MOD_MED);
    local_ilm_ptr->src_mod_id = MOD_MED;
    local_ilm_ptr->dest_mod_id = MOD_MMI;
    local_ilm_ptr->sap_id = MED_SAP;
   
    local_ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_RECORD_FINISH_IND;
    local_ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
    local_ilm_ptr->peer_buff_ptr = NULL;    


    msg_send_ext_queue(local_ilm_ptr);
    mmi_trace(1,"call back vid_rec_finish_ind  MSG_ID_MEDIA_VID_RECORD_FINISH_IND ");
	

//mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MMC_MODE_PRE_CP);
        
}


/*************************************************************************
* FUNCTION
*	vid_preview_req_hdlr
*
* DESCRIPTION
*	This function is to handle video preview request. 
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

void vid_preview_req_hdlr(ilm_struct *ilm_ptr)
{
	int32 result = -1;
	media_vid_preview_req_struct *req_p;
	MMC_VDOREC_SETTING_STRUCT previewParam;

  req_p = (media_vid_preview_req_struct*)ilm_ptr->local_para_ptr;
  vid_context_p->src_mod = ilm_ptr->src_mod_id;
  
  previewParam.previewStartX= req_p->lcd_start_x;
  previewParam.previewStartY = req_p->lcd_start_y;
  previewParam.previewWidth = cam_context_p->preview_width=req_p->preview_width;
  previewParam.previewHeight = cam_context_p->preview_height=req_p->preview_height;
  previewParam.imageWidth=req_p->image_width;
  previewParam.imageHeight=req_p->image_height;
  previewParam.contrast=req_p->contrast;
  previewParam.brightnessLevel=req_p->exposure;//req_p->brightness;
  previewParam.whiteBalance = req_p->WB;
  previewParam.exposureVerify = req_p->exposure;
  previewParam.specialEffect = req_p->effect;
  previewParam.nightMode = req_p->night_mode;
  previewParam.recordTimePermit = req_p->time_limit;
  previewParam.fileSizePermit = req_p->size_limit;
  previewParam.IfRecordAudio = req_p->record_aud;
  previewParam.encodeQuality = req_p->encode_quality;
  previewParam.zoomFactor = req_p->zoom_factor;
  previewParam.lcdrotate = 0;
  
  mmi_trace( g_sw_Vidrec,"vid_preview_req_hdlr,,previewParam.start_x is %d,previewParam.start_y is %d", previewParam.previewStartX, previewParam.previewStartY );

  result=MCI_VideoRecordPreviewStart(&previewParam,vid_rec_finish_ind);

  mmi_trace( g_sw_Vidrec,"vid_preview_req_hdlr,, previewParam.whiteBalance is %d,previewParam.exposureVerify is %d", previewParam.whiteBalance, previewParam.exposureVerify );
  
  mmi_trace( g_sw_Vidrec,"vid_preview_req_hdlr,, previewParam.specialEffect  is %d,previewParam.exposureVerify is %d", previewParam.specialEffect , previewParam.nightMode );
	//result=MCI_VideoRecordPreviewStart(&previewParam);
  mmi_trace(g_sw_Vidrec,"vid_preview_req_hdlr,result is %d",result);

  
	if(MED_RES_OK == result)
	{
		VID_ENTER_STATE(VID_START_PREVIEW);
		vid_set_result(MED_RES_OK);      
		VID_SET_EVENT(VID_EVT_PREVIEW);
	}
	else
	{
	      vid_set_result(MED_RES_BUSY);      
	      VID_SET_EVENT(VID_EVT_PREVIEW);
	}
  
#if 0
   if(CAM_IN_STATE(CAM_READY) && VID_IN_STATE(VID_IDLE))
   {
#ifndef WIN32
   
      req_p = (media_vid_preview_req_struct*)ilm_ptr->local_para_ptr;
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
      cam_param.video_mode=KAL_TRUE; /* video preview */

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

      cam_context_p->preview_check_conut=0;
      cam_context_p->preview_retry_conut=0;
      cam_context_p->zoom_factor=req_p->zoom_factor;
      cam_context_p->flash_mode=req_p->flash_mode;
      cam_context_p->night_mode=req_p->night_mode;
      cam_set_night_mode((kal_int16)cam_context_p->night_mode);
      ext_camera_preview(&cam_param, &lcd_param);
      cam_set_zoom_factor(cam_context_p->zoom_factor);
      cam_set_flash(cam_context_p->flash_mode);
#if defined(VC0558)
      cam_context_p->image_effect = req_p->effect;	  
      cam_set_effect(cam_context_p->image_effect);	 
#endif
      med_start_timer(CAM_READY_POLLING_TIMER, CAM_PREVIEW_INIT_TIME, vid_preview_check, 0);
#endif
      VID_ENTER_STATE(VID_START_PREVIEW);
   }
   else if(VID_IN_STATE(VID_PREVIEW) || VID_IN_STATE(VID_RECORD_PAUSED))
   {
      //med_start_timer(CAM_READY_POLLING_TIMER, CAM_PREVIEW_POLLING_TIME, vid_preview_check, 0);
      VID_SET_EVENT(VID_EVT_PREVIEW);
   }
   else
   {
      vid_set_result(MED_RES_BUSY);      
      VID_SET_EVENT(VID_EVT_PREVIEW);
   }
#endif
}



/*************************************************************************
* FUNCTION
*   vid_record_req_hdlr
*
* DESCRIPTION
*   This function is to handle video record request
*
* PARAMETERS
*   ilm_ptr
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_record_req_hdlr(ilm_struct *ilm_ptr)
{

   media_vid_record_req_struct *req_p;
   kal_int32 result;
   
   vid_context_p->src_mod = ilm_ptr->src_mod_id;

   req_p = (media_vid_record_req_struct*)ilm_ptr->local_para_ptr;
	mmi_trace(1," vid_record_req_hdlr");

    // FIXME 
    // Added parameter to match the function with the same name in mdi
    // The parameter should be fixed
	vid_context_p->file_handle = (HANDLE)req_p->data;
	result = MCI_VideoRecordStart((HANDLE)req_p->data);// the original value is NULL

	vid_set_result(result);
	VID_SET_EVENT(VID_EVT_RECORD);
        VID_ENTER_STATE(VID_RECORD);

}




/*************************************************************************
* FUNCTION
*   vid_stop_req_hdlr
*
* DESCRIPTION
*   This function is to handle video stop request
*
* PARAMETERS
*   ilm_ptr
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
#ifdef __FF_AUDIO_SET__
void MusicEndAudioMode(); //Added by Jinzh:20070616
#endif
void vid_stop_req_hdlr(ilm_struct *ilm_ptr)
{
   vid_context_p->src_mod = ilm_ptr->src_mod_id;
   	MCI_VideoStop();
	#ifdef __FF_AUDIO_SET__
	MusicEndAudioMode();
	#endif
	//MMI_FS_Close(vid_context_p->file_handle);//chenhe add 080924
	VID_ENTER_STATE(VID_FILE_READY);
	   VID_AUD_ENTER_STATE(VID_AUDIO_IDLE);

	VID_SET_EVENT(VID_EVT_STOP);  

}

#ifdef VIDEO_RECORDER_SUPPORT

void vid_Rec_stop_req_hdlr(ilm_struct *ilm_ptr) //chenhe for jasperII

{
   	vid_context_p->src_mod = ilm_ptr->src_mod_id;
	mmi_trace(1,"vid_Rec_stop_req_hdlr");
   	MCI_VideoRecordStop();
	
	MMI_FS_Close(vid_context_p->file_handle);//zhangl
	VID_ENTER_STATE(VID_FILE_READY);
	 VID_AUD_ENTER_STATE(VID_AUDIO_IDLE);
	VID_SET_EVENT(VID_EVT_STOP);  

}

void vid_Rec_previewStop_req_hdlr(ilm_struct *ilm_ptr) //chenhe for jasperII

{
   vid_context_p->src_mod = ilm_ptr->src_mod_id;
	mmi_trace(1,"vid_Rec_previewStop_req_hdlr");
   	MCI_VideoRecordPreviewStop();
	VID_ENTER_STATE(VID_FILE_READY);
	   VID_AUD_ENTER_STATE(VID_AUDIO_IDLE);

	VID_SET_EVENT(VID_EVT_STOP);  

}

void vid_Rec_pause_req_hdlr(ilm_struct *ilm_ptr) //chenhe for jasperII

{
   vid_context_p->src_mod = ilm_ptr->src_mod_id;
	mmi_trace(1,"vid_Rec_pause_req_hdlr");

   	MCI_VideoRecordPause();
	VID_ENTER_STATE(VID_PLAY_PAUSED);
	VID_SET_EVENT(VID_EVT_PAUSE);  

}
void vid_Rec_resume_req_hdlr(ilm_struct *ilm_ptr) //chenhe for jasperII

{
	mmi_trace(1,"vid_Rec_resume_req_hdlr");

   vid_context_p->src_mod = ilm_ptr->src_mod_id;

   	MCI_VideoRecordResume();
	VID_ENTER_STATE(VID_PLAY);
	VID_SET_EVENT(VID_EVT_RESUME);  

}

void vid_Rec_set_param_req_hdlr(ilm_struct *ilm_ptr)
{
   media_cam_set_param_req_struct *req_p;

   cam_context_p->src_mod = ilm_ptr->src_mod_id;

   req_p = (media_cam_set_param_req_struct*)ilm_ptr->local_para_ptr;
   MCI_VideoRecordAdjustSetting(req_p->param_id, req_p->value);
}
#endif
/*************************************************************************
* FUNCTION
*   vid_file_merge_req_hdlr
*
* DESCRIPTION
*   This function is to handle video file merge request
*
* PARAMETERS
*   ilm_ptr
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_file_merge_req_hdlr(ilm_struct *ilm_ptr)
{
   media_vid_file_merge_req_struct *req_p;
   
   vid_context_p->src_mod = ilm_ptr->src_mod_id;

   req_p = (media_vid_file_merge_req_struct*)ilm_ptr->local_para_ptr;

   VID_SET_EVENT(VID_EVT_FILE_MERGE);  
   vid_send_file_merge_done_ind(MED_RES_OK);
}

/*************************************************************************
* FUNCTION
*   vid_open_req_result
*
* DESCRIPTION
*   This function is to send back open video file result to mmi
*
* PARAMETERS
*  result
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_open_req_result(int16 result)
{
        media_vid_file_ready_ind_struct *ind_p=NULL ;
        ilm_struct       *local_ilm_ptr = NULL;
	 INT16 img_width = 0;
	 INT16 img_height = 0;
	 INT32 totaltime = 0;

       kal_uint16  aud_channel;
       kal_uint16  aud_sample_rate;
      kal_uint16  track;



	if(result == MED_RES_OK)
	{
	   MCI_VideoGetInfo(&img_width,&img_height, &totaltime,&aud_channel,&aud_sample_rate,& track);


	
           ind_p = (media_vid_file_ready_ind_struct*)
                    construct_local_para(sizeof(media_vid_file_ready_ind_struct), TD_CTRL);  
           if(!ind_p)return ;
           ind_p->result = (int16)result;
           ind_p->image_width=img_width;
           ind_p->image_height=img_height;
           ind_p->total_frame_num=0;
           ind_p->total_time=totaltime;
           ind_p->seq_num=0;
           ind_p->media_type =0;
           ind_p->aud_channel = aud_channel;
           ind_p->aud_sample_rate = aud_sample_rate;
           ind_p->track  = track;
  

           local_ilm_ptr = allocate_ilm(MOD_MED);
           if(!local_ilm_ptr)return;
           local_ilm_ptr->src_mod_id = MOD_MED;
           local_ilm_ptr->dest_mod_id = MOD_MMI;
           local_ilm_ptr->sap_id = MED_SAP;
           
           local_ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_FILE_READY_IND;
           local_ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
           local_ilm_ptr->peer_buff_ptr = NULL;

	   msg_send_ext_queue(local_ilm_ptr);
	   }
}

/*************************************************************************
* FUNCTION
*	vid_play_finish_ind
*
* DESCRIPTION
*	This function is to send video play finish indication to mmi
*
* PARAMETERS
*	int16 result
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_play_finish_ind(int32 result)
{
	media_vid_play_finish_ind_struct *ind_p;
	ilm_struct       *local_ilm_ptr = (ilm_struct *)NULL;

	ind_p = (media_vid_play_finish_ind_struct*)
	construct_local_para(sizeof(media_vid_play_finish_ind_struct), TD_CTRL);  
	if(!ind_p)return ;
	ind_p->result = (int16)result;
	ind_p->seq_num = 0;

	local_ilm_ptr = allocate_ilm(MOD_MED);
	if(!local_ilm_ptr)return;
	local_ilm_ptr->src_mod_id = MOD_MED;
	local_ilm_ptr->dest_mod_id = MOD_MMI;
	local_ilm_ptr->sap_id = MED_SAP;

	local_ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_PLAY_FINISH_IND;
	local_ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
	local_ilm_ptr->peer_buff_ptr = NULL;

	msg_send_ext_queue(local_ilm_ptr);
	mmi_trace(1,"call back vid_play_finish_ind   MSG_ID_MEDIA_VID_PLAY_FINISH_IND ");
}


/*************************************************************************
* FUNCTION
*   vid_open_file_req_hdlr
*
* DESCRIPTION
*   This function is to handle open file request
*
* PARAMETERS
*   ilm_ptr
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/

void vid_open_file_req_hdlr(ilm_struct *ilm_ptr)
{
  // int Result;
   //INT32 iResult,size;
   int32 result = -1;
#if (CSW_EXTENDED_API_AUDIO_VIDEO == 0)
   med_type_enum vdoType=0;
   FS_HANDLE fhd=-1;
#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */
   mmi_trace(MJPEGTRACE, "Excute func : vid_open_file_req_hdlr");//elisa 
	
   media_vid_open_file_req_struct *req_p;
   
   vid_context_p->src_mod = ilm_ptr->src_mod_id;

   req_p = (media_vid_open_file_req_struct*)ilm_ptr->local_para_ptr;

   vid_context_p->seq_num=req_p->seq_num;

	vid_stop_unfinished_audio();
	cam_abort_to_idle();
//jiashuo change 	
#if (CSW_EXTENDED_API_AUDIO_VIDEO==1)
    {
        UINT8* fileName; 	 	 
        UINT8* unixFileName; 	 	 

        fileName = OslMalloc(MAX_MEDIA_FILE_NAME);  
        unixFileName =  OslMalloc(MAX_MEDIA_FILE_NAME);
        ConvertRelative2Full(fileName, (UINT8*)req_p->file_name_p);
        ConvertDosName2Unix(unixFileName, fileName);
        result=MCI_VideoOpenFile(2, unixFileName);
        OslMfree(fileName);
        OslMfree(unixFileName);
    }
#else
    if( (fhd=MMI_FS_Open((UINT8 *)req_p->file_name_p,FS_READ_ONLY))<0)
    {
        result = -1;
        goto vid_play_end;
    }
	vid_context_p->file_handle = fhd;//chenhe add 080924
    vdoType = med_get_media_type(req_p->file_name_p);


{
    mci_type_enum video_fromat_type;

    switch (vdoType)
    {
    
        case MED_TYPE_MP4:
		   video_fromat_type=MCI_TYPE_MP4;
        break;

        case MED_TYPE_3GP:
		   video_fromat_type=MCI_TYPE_3GP;
        break;

	 case MED_TYPE_MJPG:
		   video_fromat_type=MCI_TYPE_MJPG;
        break;
		
	 case MED_TYPE_RM:
		   video_fromat_type=MCI_TYPE_RM;
        break;
		
        default:
		video_fromat_type=MCI_TYPE_NONE;
    		mmi_trace(1,"vid_open_file_req_hdlr: Unknown Multimedia Type %d",vdoType);
        break;
    }

    result=MCI_VideoOpenFile(2, 1, fhd, 0, 0, video_fromat_type, vid_play_finish_ind,req_p->videoplayer_drawtime_hdlr);

}
//jiashuo change
vid_play_end:
#endif
	if(result == MED_RES_OK)
	{
//		vid_send_file_ready_ind(result);
		vid_set_result(MED_RES_OK);     
		VID_SET_EVENT(VID_EVT_OPEN_FILE);
		vid_open_req_result( MED_RES_OK);
	}
	else
	{
		vid_set_result(MED_RES_BUSY);     
#if (CSW_EXTENDED_API_AUDIO_VIDEO == 0)
		MMI_FS_Close(fhd);//chenhe for bug 9572 
#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */
		VID_SET_EVENT(VID_EVT_OPEN_FILE);
	}
   	mmi_trace(MJPEGTRACE,"vid_open_file_req_hdlr,tempbuffer success,result is %d", result);

}

/*************************************************************************
* FUNCTION
*   vid_close_file_req_hdlr
*
* DESCRIPTION
*   This function is to handle close file request
*
* PARAMETERS
*   ilm_ptr
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_close_file_req_hdlr(ilm_struct *ilm_ptr)
{

MCI_VideoClose();
MMI_FS_Close(vid_context_p->file_handle);//shenh
 VID_ENTER_STATE(VID_IDLE); 
   VID_SET_EVENT(VID_EVT_CLOSE_FILE);

}
void vid_dummy_function()
{
}
/*************************************************************************
* FUNCTION
*   vid_open_req_hdlr
*
* DESCRIPTION
*   This function is to handle openrequest
*
* PARAMETERS
*   ilm_ptr
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_open_req_hdlr(ilm_struct *ilm_ptr)
{
   int32 result = -1;
   med_type_enum vdoType=0;
   HANDLE fhd=-1;
   UINT8 file_mode=0;
   INT16 img_width, img_height;
   INT32 totaltime;

       kal_uint16  aud_channel;
       kal_uint16  aud_sample_rate;
      kal_uint16  track;


   mmi_trace(MJPEGTRACE, "Excute func : vid_open_req_hdlr");//elisa 
	
      media_vid_open_req_struct *req_p;
   
   vid_context_p->src_mod = ilm_ptr->src_mod_id;

   req_p = (media_vid_open_req_struct*)ilm_ptr->local_para_ptr;

   vid_context_p->seq_num=req_p->seq_num;

	vid_stop_unfinished_audio();
	cam_abort_to_idle();
//jiashuo change 	
	mmi_trace(TRUE,"-----++Func: %s;File: %s;Line: %d++-----play_audio=%d", __FUNCTION__, __FILE__, __LINE__,req_p->play_audio);

	if(req_p->media_mode==MED_MODE_FILE)//only for file mode.sheen
	{
		mmi_trace(TRUE,"sheen. vid file mode\n");
		if( (fhd=MMI_FS_Open(req_p->data,FS_READ_ONLY))<0)
		{
			result = -1;
			goto vid_play_end;
		}

		file_mode=0;
		vdoType = med_get_media_type(req_p->data);
	}
	else
	{
		fhd=(HANDLE)req_p->data;
		file_mode=1;
		vdoType=req_p->media_type;
	}
	
	//vdoType = med_get_media_type(req_p->data);
#if (CSW_EXTENDED_API_AUDIO_VIDEO==1)
	if((result=MCI_VideoOpenFile(2, req_p->data)) == MED_RES_OK)
#else
//qiff modify for bug:10630



    mci_type_enum video_fromat_type;

    switch (vdoType)
    {
    
        case MED_TYPE_MP4:
		   video_fromat_type=MCI_TYPE_MP4;
        break;

        case MED_TYPE_3GP:
		   video_fromat_type=MCI_TYPE_3GP;
        break;

	 case MED_TYPE_MJPG:
		   video_fromat_type=MCI_TYPE_MJPG;
        break;
		
	 case MED_TYPE_RM:
		   video_fromat_type=MCI_TYPE_RM;
        break;

        default:
		video_fromat_type=MCI_TYPE_NONE;
    		mmi_trace(1,"vid_open_req_hdlr: Unknown Multimedia Type %d",vdoType);
        break;
    }
 

	if((result=MCI_VideoOpenFile(2, req_p->play_audio, fhd, file_mode, req_p->data_size,video_fromat_type, vid_play_finish_ind,vid_dummy_function)) == MED_RES_OK)
#endif
	{
	      MCI_VideoGetInfo(&img_width,&img_height, &totaltime,&aud_channel,&aud_sample_rate,& track);
//		MCI_VideoGetInfo(&img_width,&img_height, &totaltime);
		vid_set_image_size(img_width,img_height);
	    vid_set_total_time(totaltime);
	}
//jiashuo change
vid_play_end:
	mmi_trace(TRUE,"Func: %s result:%d", __FUNCTION__, result);
	if(result == MED_RES_OK)
	{
//		vid_send_file_ready_ind(result);
		vid_set_result(MED_RES_OK);     
		VID_SET_EVENT(VID_EVT_OPEN);
		vid_open_req_result( MED_RES_OK);
		VID_ENTER_STATE(VID_OPEN_FILE);

	}
	else
	{
		vid_set_result(MED_RES_BUSY);     
		if(req_p->media_mode==MED_MODE_FILE)
			MMI_FS_Close(fhd);
		VID_SET_EVENT(VID_EVT_OPEN);
		VID_ENTER_STATE(VID_IDLE);

	}
}

/*************************************************************************
* FUNCTION
*   vid_close_req_hdlr
*
* DESCRIPTION
*   This function is to handle close request
*
* PARAMETERS
*   ilm_ptr
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_close_req_hdlr(ilm_struct *ilm_ptr)
{

MCI_VideoClose();
MMI_FS_Close(vid_context_p->file_handle);
 VID_ENTER_STATE(VID_IDLE); 
   VID_SET_EVENT(VID_EVT_CLOSE);
}


/*************************************************************************
* FUNCTION
*   vid_play_req_hdlr
*
* DESCRIPTION
*   This function is to handle video play request
*
* PARAMETERS
*   ilm_ptr
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
extern BOOL get_vdo_audioPlay(void);

#ifdef __FF_AUDIO_SET__
extern void MusicStartAudioMode(); //Added by Jinzh:20070616
#endif

//extern vdoply_context_struct	g_vdoply_cntx;
void vid_play_req_hdlr(ilm_struct *ilm_ptr)
{
   media_vid_play_req_struct *req_p;
   //int read_size;
   //memset(aud_context_p,0x0,sizeof(aud_context_struct));//chenhe add,070731
   vid_context_p->src_mod = ilm_ptr->src_mod_id;

   req_p=(media_vid_play_req_struct*)ilm_ptr->local_para_ptr;

   mmi_trace(MJPEGTRACE,"elisa_vid_play_req_hdlr req_p->lcd_start_x = %d,req_p->lcd_start_y = %d",
   						req_p->lcd_start_x,req_p->lcd_start_y);
   
  	MCI_VideoPlay(req_p->lcd_start_x,req_p->lcd_start_y);
//	if(get_vdo_audioPlay())
	if(req_p->play_audio)
	{
	#ifdef __FF_AUDIO_SET__
		MusicStartAudioMode();
	#endif
	}
	VID_AUD_ENTER_STATE(VID_AUDIO_PLAY_NORMAL);
	vid_set_result(MED_RES_OK);
	VID_SET_EVENT(VID_EVT_PLAY);
	VID_ENTER_STATE(VID_PLAY);

}

/*************************************************************************
* FUNCTION
*   vid_seek_req_result
*
* DESCRIPTION
*   This function is to send back seek video file result to mmi
*
* PARAMETERS
*  result
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_seek_req_result(int16 result)
{
    media_vid_seek_done_ind_struct *ind_p=NULL;
    ilm_struct       *local_ilm_ptr = NULL;
    ind_p = (media_vid_seek_done_ind_struct*)
        construct_local_para(sizeof(media_vid_seek_done_ind_struct), TD_CTRL);  
    if(!ind_p)return;
    ind_p->result = (int16)result;
    local_ilm_ptr = allocate_ilm(MOD_MED);
    if(!local_ilm_ptr)return;
    local_ilm_ptr->src_mod_id = MOD_MED;
    local_ilm_ptr->dest_mod_id = MOD_MMI;
    ind_p->seq_num = 0;
    local_ilm_ptr->sap_id = MED_SAP;

    local_ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_SEEK_DONE_IND;
    local_ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
    local_ilm_ptr->peer_buff_ptr = NULL;
    
	   msg_send_ext_queue(local_ilm_ptr);
}
/*************************************************************************
* FUNCTION
*   vid_seek_req_hdlr
*
* DESCRIPTION
*   This function is to handle video seek request
*
* PARAMETERS
*   ilm_ptr
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_seek_req_hdlr(ilm_struct *ilm_ptr)
{
   //CP_ENTRY(10006);
   int32 result = -1;
   media_vid_seek_req_struct *req_p;
   
	vid_context_p->src_mod = ilm_ptr->src_mod_id;

	req_p=(media_vid_seek_req_struct*)ilm_ptr->local_para_ptr;

	//lcd_data.lcm_start_x = req_p->display_offset_x;
	//lcd_data.lcm_start_y = req_p->display_offset_y;

	mmi_trace(MJPEGTRACE,"chenhe, _ vid_seek_req_hdlr req_p->display_offset_x = %d,req_p->display_offset_y = %d",
			req_p->display_offset_x,req_p->display_offset_y);
	mmi_trace(1," vid_seek_req_hdlr seektime = %d",req_p->time);
	result = MCI_VideoSeek(req_p->time,0,req_p->display_offset_x,req_p->display_offset_y);
		if(result == MED_RES_OK)
	{
		vid_set_result(MED_RES_OK);
		VID_SET_EVENT(VID_EVT_SEEK);
		vid_seek_req_result( MED_RES_OK);
	}
	else
	{
		vid_set_result(MED_RES_BUSY);
		VID_SET_EVENT(VID_EVT_SEEK);
		vid_seek_req_result( MED_RES_BUSY);
	}


}


/*************************************************************************
* FUNCTION
*   vid_pause_req_hdlr
*
* DESCRIPTION
*   This function is to handle video pause request
*
* PARAMETERS
*   ilm_ptr
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_pause_req_hdlr(ilm_struct *ilm_ptr)
{
   	MCI_VideoPause();
	//close audio pa when pause in speaker mode,if don't,there is noise on the speaker
	if(GetHandsetInPhone()!=1)
		{
		#ifdef __FF_AUDIO_SET__
		//GpioSpeakerModeEnd();
		MusicEndAudioMode();
		#endif
		}
	
	VID_ENTER_STATE(VID_FILE_READY);
	VID_SET_EVENT(VID_EVT_PAUSE);  
}

void vid_display_set_req_hdlr(ilm_struct *ilm_ptr)
{
	media_vid_display_set_req_struct *req_p;
	req_p = (media_vid_display_set_req_struct*)ilm_ptr->local_para_ptr;
		mmi_trace(1," vid_display_set_req_hdlr"); 

	MCI_VideoSet(req_p->ZoomWidth, req_p->ZoomHeight, req_p->startX, req_p->startY, req_p->cutX,req_p->cutY,req_p->cutW,req_p->cutH,req_p->rotate) ;

}


/*************************************************************************
* FUNCTION
*   vid_set_display_req_hdlr
*
* DESCRIPTION
*   This function is to handle video set display request
*
* PARAMETERS
*   ilm_ptr
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_set_display_req_hdlr(ilm_struct *ilm_ptr)
{
   media_vid_set_display_req_struct *req_p;
   
   vid_context_p->src_mod = ilm_ptr->src_mod_id;

   req_p = (media_vid_set_display_req_struct*)ilm_ptr->local_para_ptr;

   if(vid_context_p->display_device!=req_p->device)
   {
      vid_context_p->display_device=req_p->device;
      if(vid_context_p->display_device==MED_DISPLAY_TO_MAIN_LCD)
      {
         if(VID_IN_STATE(VID_PLAY) || VID_IN_STATE(VID_AUDIO_PLAY_FINISH))
         {
            if(vid_context_p->image_path_open!=KAL_TRUE)
            {
  //             start_jpeg_video_play(&lcd_data);
               vid_context_p->image_path_open=KAL_TRUE;
            }
         }   
      }
      else if(vid_context_p->display_device==MED_DISPLAY_NONE)
      {
         if((VID_IN_STATE(VID_PLAY) || VID_IN_STATE(VID_AUDIO_PLAY_FINISH))
            && vid_context_p->image_path_open==KAL_TRUE)
         {
  //          stop_jpeg_video_play();
            vid_context_p->image_path_open=KAL_FALSE;
         }   
      }
   }
   VID_SET_EVENT(VID_EVT_SET_DISPLAY);
}


void vid_send_play_time(UINT32 playtime)
{
   media_vid_play_time_ind_struct *ind_p;
   ilm_struct       *ilm_ptr = NULL;
   
   ind_p = (media_vid_play_time_ind_struct*)
            construct_local_para(sizeof(media_vid_play_time_ind_struct), TD_CTRL);  

   ind_p->playtime= playtime;

   ilm_ptr = allocate_ilm(MOD_MED);
   ilm_ptr->src_mod_id = MOD_MED;
   ilm_ptr->dest_mod_id = MOD_MMI;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_VID_PLAY_TIME_IND;
   ilm_ptr->local_para_ptr = (local_para_struct*)ind_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);
}

void vid_get_play_time_req_hdlr(ilm_struct *ilm_ptr)
{
   UINT32 playtime;
   
   playtime = MCI_VideoGetPlayTime();

   vid_send_play_time(playtime);
   
}

void vid_get_play_time_block_req_hdlr(ilm_struct *ilm_ptr)
{
   UINT32 playtime;
   
   playtime = MCI_VideoGetPlayTime();

   vid_set_current_time((kal_uint64)playtime);
   VID_SET_EVENT(VID_EVT_GET_INFO);
}

void vid_display_interface_req_hdlr(ilm_struct *ilm_ptr)
{ 
	MCI_DisplayVideoInterface(); 
}

#ifdef __MED_MJPG_AUDIO__            
/*************************************************************************
* FUNCTION
*   vid_audio_play_finish_ind_hdlr
*
* DESCRIPTION
*   This function is to handle audio play finish indication message. 
*   
* PARAMETERS
*   ilm_ptr
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_audio_play_finish_ind_hdlr(ilm_struct *ilm_ptr)
{
   if(vid_context_p->play_audio)
   {
      aud_player_struct *player_p;
      player_p=&(vid_context_p->aud_player);
      if (player_p->used && VID_AUD_IN_STATE(VID_AUDIO_PLAY_NORMAL)) 
      {
         Media_Status status;
         status=JAmr_Stop((Media_Handle*)player_p->handle);
      }
   }   

   VID_AUD_ENTER_STATE(VID_AUDIO_IDLE);
   
}
#endif



/*************************************************************************
* FUNCTION
*   vid_is_audio_channel_available
*
* DESCRIPTION
*   This function is to check if the audio channel is avaiable. 
*
* PARAMETERS
*   None
*
* RETURNS
*   kal_bool
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_bool vid_is_audio_channel_available(void)
{
   mmi_trace(1,"chenhe,vid_is_audio_channel_available,vid_context_p->state is %d",vid_context_p->state);
   if(VID_IN_STATE(VID_IDLE))
   {
      return KAL_TRUE;
   }
   else if(VID_IN_STATE(VID_PLAY) || 
         VID_IN_STATE(VID_PLAY_PAUSED) || 
         VID_IN_STATE(VID_AUDIO_PLAY_FINISH) ||
         VID_IN_STATE(VID_AUDIO_PLAY_ERROR) ||
         VID_IN_STATE(VID_VISUAL_PLAY_FINISH) ||
         VID_IN_STATE(VID_FILE_READY) ||
         VID_IN_STATE(VID_OPEN_FILE) ||
         VID_IN_STATE(VID_SEEK) ||
         VID_IN_STATE(VID_GET_IFRAME))
   {
      if(VID_AUD_IN_STATE(VID_AUDIO_IDLE))
         return KAL_TRUE;
      else
         return KAL_FALSE;
   }
   else if(VID_IN_STATE(VID_RECORD) || 
         VID_IN_STATE(VID_RECORD_FINISH) ||
         VID_IN_STATE(VID_RECORD_PAUSED) ||
         VID_IN_STATE(VID_RECORD_STOP_VISUAL))
   {
      return KAL_FALSE;
   }
   else
      return KAL_TRUE;
}

#endif

#endif /* MED_NOT_PRESENT */

