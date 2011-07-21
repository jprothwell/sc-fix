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
*    ext_camera.c
*
*  Project:
* --------
 
*
* Description:
* ------------
*   This file implements external camera module interface function (API)
*
* Author:
* -------
*
*============================================================================
*             HISTORY
 
*------------------------------------------------------------------------------
* 
*------------------------------------------------------------------------------
 
*============================================================================
****************************************************************************/
#include "kal_release.h"
#include "stack_common.h"
#include "stack_msgs.h"
#include "app_ltlcom.h"
#include "data_buff_pool.h"
#include "stack_config.h"

#include "lcd_sw.h"
#include "lcd_sw_inc.h"
#include "lcd_sw_rnd.h"
#include "lcd_if.h"

#if (defined (CAMERA_MODULE))
//#include "jpeg.h"
//#include "cam_module.h"  //deleted by weid
#include "med_api.h"
#include "ext_camera.h"
#include "ts.h"
#include "mmi_trace.h"
#include "media_others.h"
#include "di.h"
#include "mci.h"

extern lcd_layer_struct	gdi_layer_info[]; 

kal_uint8 ext_camera_lcd;


lcd_frame_update_struct ext_camera_preview_lcd_data;
#ifdef DUAL_LCD
   extern lcd_frame_update_struct sub_lcd_fb_update_para;
#endif

#ifdef CAMERA_MODULE_WITH_LCD
lcd_frame_update_to_mem_struct ext_camera_lcd_menu_data;
ext_camera_osd_para_struct	ext_camera_osd_buffer_data;
#endif

kal_uint8 ext_camera_state;
kal_uint8 camera_sleep_mode_handler=0xFF;

kal_uint8 burst_snapshot_number;
ext_camera_para_struct ext_camera_config_data;
ext_cam_module_func *ext_cam_func;

kal_bool engineering_mode_setting=KAL_FALSE;
kal_uint8 ext_cam_reg_setting[0x100];
kal_uint16 ext_cam_reg_value[0x100];
kal_bool video_clip_mode=KAL_FALSE;

#if (!defined(ISP_SUPPORT))
kal_uint8 cam_command_map[CAM_NO_OF_PARAM];
kal_uint8 cam_ev_param_map[CAM_NO_OF_EV];
kal_uint8 cam_zoom_param_map[CAM_NO_OF_ZOOM];
kal_uint8 cam_effect_param_map[CAM_NO_OF_EFFECT_ENC];
kal_uint8 cam_jpg_qty_param_map[CAM_NO_OF_JPG_QTY];
kal_uint8 cam_wb_param_map[CAM_NO_OF_WB];
kal_uint8 cam_banding_param_map[CAM_NO_OF_BANDING];
#endif


/*************************************************************************
* FUNCTION
*	ext_camera_init
*
* DESCRIPTION
*	This function init external camera module status.
*
* PARAMETERS
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
extern void cam_module_func_config(void);  /* wufasong added 2007.06.14 */
extern ext_cam_module_func cam_func;
uint8 L1SM_GetHandle(void);
void get_NFI_bus(void)
{

};
void free_NFI_bus(void)
{

};
void L1SM_SleepEnable(uint8 handle);

void L1SM_SleepDisable(uint8 handle);

void cam_module_func_config(void);

void ext_camera_init(void)
{
	kal_uint16 i;

	mmi_trace(g_sw_CAMERA, "Excute func : ext_camera_init()");

//	ext_cam_func = &cam_func; /* wufasong added 2007.06.15 */

	ext_camera_state=EXT_CAMERA_POWER_OFF_STATE;
	if (camera_sleep_mode_handler==0xFF)
		camera_sleep_mode_handler = L1SM_GetHandle();  
	cam_module_func_config(); 
	if(kal_query_systemInit()==KAL_FALSE)
	{
		get_NFI_bus();
//		Camera_DI->cam_module_cmd_mapping();
		free_NFI_bus();
	}
	else	// System in initialization stage
//		Camera_DI->cam_module_cmd_mapping();

	video_clip_mode=KAL_FALSE;
	for (i=0;i<0x100;i++)
		ext_cam_reg_setting[i]=0;
}	/* ext_camera_init() */

/*************************************************************************
* FUNCTION
*	ext_camera_power_on
*
* DESCRIPTION
*	This function turn the power of external camera module on.
*
* PARAMETERS
*	None
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void ext_camera_power_on(void)
{
	mmi_trace(g_sw_CAMERA,"Excut funs: ext_camera_power_on is TRUE ext_camera_state = %d",ext_camera_state);
	if (ext_camera_state==EXT_CAMERA_POWER_OFF_STATE)
	{
		lcd_power_up();
#if (defined(CAMERA_MODULE_WITH_LCD))
	#ifdef DUAL_LCD
		if ((main_lcd_operation_state!=LCD_STANDBY_STATE)||
			 (sub_lcd_operation_state!=LCD_STANDBY_STATE))
			ASSERT(0);   
			main_lcd_operation_state=LCD_CAMERA_ACTIVE_STATE;
			sub_lcd_operation_state=LCD_CAMERA_ACTIVE_STATE;
	#else
		if (main_lcd_operation_state!=LCD_STANDBY_STATE)
			ASSERT(0);   
			main_lcd_operation_state=LCD_CAMERA_ACTIVE_STATE;
	#endif
#endif		
		get_NFI_bus();

		Camera_DI->cam_module_power_on();

		free_NFI_bus();
		ext_camera_state=EXT_CAMERA_INITIAL_STATE;
		lcd_power_down();
	}
	else if (ext_camera_state!=EXT_CAMERA_INITIAL_STATE)
		ASSERT(0);   
}	/* ext_camera_power_on() */

/*************************************************************************
* FUNCTION
*	ext_camera_reset_status
*
* DESCRIPTION
*	This function check if the external camera module power on procedure is
*  complete or not.
*
* PARAMETERS
*	None
* RETURNS
*	1 : external camera mdoule power is on
*  0 : external camera module is initializing.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_uint8 ext_camera_reset_status(void)
{
	kal_uint16 i;
	kal_uint16 data;

	lcd_power_up();
	get_NFI_bus();

	if(Camera_DI->cam_module_reset_status())
	{
		ext_camera_state=EXT_CAMERA_STANDBY_STATE;
		if (engineering_mode_setting==KAL_TRUE)
		{
			for (i=0;i<0x100;i++)
			{
				if (ext_cam_reg_setting[((kal_uint8)i)]==1)
				{
					do
					{

						Camera_DI->cam_module_write_para(i,ext_cam_reg_value[((kal_uint8) i)]);

						data=Camera_DI->cam_module_read_para(i);

					} while (data!=ext_cam_reg_value[((kal_uint8)i)]);
				}
			}
		}
		free_NFI_bus();
		lcd_power_down();
		return KAL_TRUE;
	}
	free_NFI_bus();
	lcd_power_down();
	return KAL_FALSE;
}	/* ext_camera_reset_status() */

#if (defined(CAMERA_MODULE_WITH_LCD))
void wait_lcd_update(void)
{
#ifdef DUAL_LCD
	while ((main_lcd_operation_state==LCD_CAMERA_ACTIVE_UPDATE_STATE)||
			 (sub_lcd_operation_state==LCD_CAMERA_ACTIVE_UPDATE_STATE))
#else
	while (main_lcd_operation_state==LCD_CAMERA_ACTIVE_UPDATE_STATE)
#endif
	{
		kal_sleep_task(1);
	}
}
#endif



/*************************************************************************
* FUNCTION
*	ext_camera_ready_check
*
* DESCRIPTION
*	This function check if the external camera module power on procedure is
*  complete or not.
*
* PARAMETERS
*	None
* RETURNS
*	1 : external camera mdoule power is on
*  0 : external camera module is initializing.
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_eventgrpid lcd_event_id;
//static U8 *pStrCRC = NULL;
kal_uint32 ext_camera_ready_check(void)
{
	kal_uint32 jpeg_file_size;

	get_NFI_bus();
	mmi_trace(g_sw_CAMERA,"Excut func: ext_camera_ready_check is %d",ext_camera_state);
#ifndef CAMERA_MODULE_WITH_LCD
	if (ext_camera_state==EXT_CAMERA_PREVIEW_STATE)
	{
		if(Camera_DI->cam_module_preview_ready_check())
		{
#ifndef DUAL_LCD
			if (main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)
#else
			if ((main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)||
		 		(sub_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE))
#endif	/* DUAL_LCD */
			{
				if (ext_camera_lcd==MAIN_LCD)
				{
						main_lcd_fb_update_para.module_id=LCD_UPDATE_MODULE_EXT_CAMERA;
						lcd_fb_update((lcd_frame_update_struct *) &main_lcd_fb_update_para);
						kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
					}
				#ifdef DUAL_LCD
					else if (ext_camera_lcd==SUB_LCD)
					{
						sub_lcd_fb_update_para.module_id=LCD_UPDATE_MODULE_EXT_CAMERA;
						lcd_fb_update((lcd_frame_update_struct *) &sub_lcd_fb_update_para);
						kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
				}
			#endif
			}
#ifndef DUAL_LCD
			else if (main_lcd_operation_state==LCD_CAMERA_UPDATE_STATE)
#else
			else if ((main_lcd_operation_state==LCD_CAMERA_UPDATE_STATE)||
		 				(sub_lcd_operation_state==LCD_CAMERA_UPDATE_STATE))
#endif	/* DUAL_LCD */
			{



     		lcd_fb_update((lcd_frame_update_struct *) &ext_camera_preview_lcd_data);
		
			}
			else
				ASSERT(0);  
			free_NFI_bus();
			return (0);
		}
		else
		{
#ifndef DUAL_LCD
			if (main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)
#else
			if ((main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)||
		 		(sub_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE))
#endif	/* DUAL_LCD */
			{
				if (ext_camera_lcd==MAIN_LCD)
				{
					main_lcd_fb_update_para.module_id=LCD_UPDATE_MODULE_EXT_CAMERA;
					lcd_fb_update((lcd_frame_update_struct *) &main_lcd_fb_update_para);
					kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
				}
			#ifdef DUAL_LCD
				else if (ext_camera_lcd==SUB_LCD)
				{
					sub_lcd_fb_update_para.module_id=LCD_UPDATE_MODULE_EXT_CAMERA;
					lcd_fb_update((lcd_frame_update_struct *) &sub_lcd_fb_update_para);
					kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
				}
			#endif
			}
		}
		free_NFI_bus();
		return (0xFFFFFFFF);
	}
	else if (ext_camera_state==EXT_CAMERA_CAPTURE_STATE)
	{

		jpeg_file_size=Camera_DI->cam_module_capture_ready_check();

        mmi_trace(g_sw_CAMERA,"Excute fun: ext_camera_state==EXT_CAMERA_CAPTURE_STATE is TRUE");
        mmi_trace(g_sw_CAMERA,"Excute fun: jpeg_file_size is %d",jpeg_file_size);

		
		if (jpeg_file_size!=NULL_JPEG_FILE)
		{
			if ((ext_camera_config_data.snapshot_number!=1)&&
				 (ext_camera_config_data.captured_picture!=burst_snapshot_number))
			{
				ext_camera_config_data.snapshot_number=burst_snapshot_number-ext_camera_config_data.captured_picture;
				ext_camera_config_data.jpeg_buffer_ptr=(kal_uint8 *) (((kal_uint32)ext_camera_config_data.jpeg_buffer_ptr)+jpeg_file_size);
				ext_camera_config_data.image_buffer_size=ext_camera_config_data.image_buffer_size-jpeg_file_size;
				ext_camera_config_data.total_file_size+=jpeg_file_size;

				Camera_DI->cam_module_capture((ext_camera_para_struct_di*)(&ext_camera_config_data));

				free_NFI_bus();
				return (NULL_JPEG_FILE);
			}
			else
			{
				L1SM_SleepEnable(camera_sleep_mode_handler);  
				ext_camera_state=EXT_CAMERA_STANDBY_STATE;
				ext_camera_config_data.total_file_size+=jpeg_file_size;
      	   if (ext_camera_lcd==MAIN_LCD)
		         main_lcd_operation_state=LCD_STANDBY_STATE;
         #ifdef DUAL_LCD
         	else if (ext_camera_lcd==SUB_LCD)
		         sub_lcd_operation_state=LCD_STANDBY_STATE;
         #endif
         	free_NFI_bus();
				return (ext_camera_config_data.total_file_size);
			}
		}
		else
		{
			free_NFI_bus();
			return (NULL_JPEG_FILE);
		}
	}
	else
		ASSERT(0);   

#else

	if ((ext_camera_state==EXT_CAMERA_PREVIEW_STATE) || (ext_camera_state==EXT_CAMERA_VIDEO_RECORD_STATE))
	{

		if(Camera_DI->cam_module_preview_ready_check())
		{
#ifndef DUAL_LCD
			if (main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)
#else
			if ((main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)||
		 		(sub_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE))
#endif	/* DUAL_LCD */
			{
				wait_lcd_update();
				
				if (ext_camera_state==EXT_CAMERA_PREVIEW_STATE)
				{
   				ext_camera_osd_buffer_data.lcd_id=ext_camera_preview_lcd_data.lcd_id;
   				ext_camera_osd_buffer_data.lcm_start_x=ext_camera_preview_lcd_data.lcm_start_x;
   				ext_camera_osd_buffer_data.lcm_start_y=ext_camera_preview_lcd_data.lcm_start_y;
   				ext_camera_osd_buffer_data.lcm_end_x=ext_camera_preview_lcd_data.lcm_end_x;
   				ext_camera_osd_buffer_data.lcm_end_y=ext_camera_preview_lcd_data.lcm_end_y;
   				ext_camera_osd_buffer_data.dest_buffer_address=(kal_uint32)ext_camera_config_data.image_buffer_ptr;
   				ext_camera_osd_buffer_data.dest_buffer_size=ext_camera_config_data.image_buffer_size;
   				ext_camera_osd_buffer_data.force_update=0;
   
   				if (main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)
   				{
   					lcd_fb_update_to_memory(&ext_camera_lcd_menu_data);

					Camera_DI->cam_module_write_lcdfb(&ext_camera_osd_buffer_data);

   					kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
   					
   				}
   				#ifdef DUAL_LCD
   					else if (sub_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)
   
   					{
   						lcd_fb_update_to_memory(&ext_camera_lcd_menu_data);

						Camera_DI->cam_module_write_lcdfb(&ext_camera_osd_buffer_data);

   						kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
   					}
   				#endif
				
			   }
			   else if (ext_camera_state==EXT_CAMERA_VIDEO_RECORD_STATE)
			   {
   				ext_camera_osd_buffer_data.lcm_start_x=main_lcd_fb_update_para.lcm_start_x;
   				ext_camera_osd_buffer_data.lcm_start_y=main_lcd_fb_update_para.lcm_start_y;
   				ext_camera_osd_buffer_data.lcm_end_x=main_lcd_fb_update_para.lcm_end_x;
   				ext_camera_osd_buffer_data.lcm_end_y=main_lcd_fb_update_para.lcm_end_y;
   				ext_camera_osd_buffer_data.dest_buffer_address=(kal_uint32)ext_camera_config_data.image_buffer_ptr;
   				ext_camera_osd_buffer_data.dest_buffer_size=ext_camera_config_data.image_buffer_size;
   				ext_camera_osd_buffer_data.force_update=1;
   				
   				ext_camera_lcd_menu_data.roi_width=main_lcd_fb_update_para.lcm_end_x-main_lcd_fb_update_para.lcm_start_x+1;
   				ext_camera_lcd_menu_data.roi_height=main_lcd_fb_update_para.lcm_end_y-main_lcd_fb_update_para.lcm_start_y+1;
   				ext_camera_lcd_menu_data.roi_offset_x=main_lcd_fb_update_para.roi_offset_x;
   				ext_camera_lcd_menu_data.roi_offset_y=main_lcd_fb_update_para.roi_offset_y;
      				
   				if (main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)
   				{
   					lcd_fb_update_to_memory(&ext_camera_lcd_menu_data);

					Camera_DI->cam_module_write_lcdfb(&ext_camera_osd_buffer_data);

   					kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
   					
   					ext_camera_osd_buffer_data.lcd_id=ext_camera_preview_lcd_data.lcd_id;
   					ext_camera_osd_buffer_data.lcm_start_x=ext_camera_preview_lcd_data.lcm_start_x;
   					ext_camera_osd_buffer_data.lcm_start_y=ext_camera_preview_lcd_data.lcm_start_y;
   					ext_camera_osd_buffer_data.lcm_end_x=ext_camera_preview_lcd_data.lcm_end_x;
   					ext_camera_osd_buffer_data.lcm_end_y=ext_camera_preview_lcd_data.lcm_end_y;
   					ext_camera_osd_buffer_data.dest_buffer_address=(kal_uint32)ext_camera_config_data.image_buffer_ptr;
      				ext_camera_osd_buffer_data.dest_buffer_size=ext_camera_config_data.image_buffer_size;
      				ext_camera_osd_buffer_data.force_update=0;
      				
      				ext_camera_lcd_menu_data.roi_width=ext_camera_preview_lcd_data.lcm_end_x-ext_camera_preview_lcd_data.lcm_start_x+1;
      				ext_camera_lcd_menu_data.roi_height=ext_camera_preview_lcd_data.lcm_end_y-ext_camera_preview_lcd_data.lcm_start_y+1;
      				ext_camera_lcd_menu_data.roi_offset_x=ext_camera_preview_lcd_data.roi_offset_x;
      				ext_camera_lcd_menu_data.roi_offset_y=ext_camera_preview_lcd_data.roi_offset_y;
      				
      				lcd_fb_update_to_memory(&ext_camera_lcd_menu_data);

					Camera_DI->cam_module_write_lcdfb(&ext_camera_osd_buffer_data);

   				}
   			}
			}
			free_NFI_bus();
			return (0);
		}
		free_NFI_bus();
		return (0xFFFFFFFF);
	}
	else if (ext_camera_state==EXT_CAMERA_CAPTURE_STATE)
	{

		jpeg_file_size=Camera_DI->cam_module_capture_ready_check();

		if (jpeg_file_size!=NULL_JPEG_FILE)
		{
			
				L1SM_SleepEnable(camera_sleep_mode_handler);
				ext_camera_state=EXT_CAMERA_STANDBY_STATE;
				ext_camera_config_data.total_file_size+=jpeg_file_size;

	         main_lcd_operation_state=LCD_CAMERA_ACTIVE_STATE;
         #ifdef DUAL_LCD
	         sub_lcd_operation_state=LCD_CAMERA_ACTIVE_STATE;
         #endif
         	free_NFI_bus();
				return (ext_camera_config_data.total_file_size);
		}
		else
		{
			free_NFI_bus();
			return (NULL_JPEG_FILE);
	}
	}
	else
		ASSERT(0);  
#endif	/* CAMERA_MODULE_WITH_LCD */
	free_NFI_bus();
	return 0;
}	/* ext_camera_ready_check() */

void ext_camera_power_off(void)
{
	lcd_power_up();	
#if (defined(CAMERA_MODULE_WITH_LCD))	
	if ((ext_camera_state==EXT_CAMERA_PREVIEW_STATE)||
		 (ext_camera_state==EXT_CAMERA_CAPTURE_STATE)||
		 (ext_camera_state==EXT_CAMERA_VIDEO_RECORD_STATE))
#else
	if ((ext_camera_state==EXT_CAMERA_PREVIEW_STATE)||
		 (ext_camera_state==EXT_CAMERA_CAPTURE_STATE))
#endif
	{
#ifndef DUAL_LCD
		if (main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)
#else
		if ((main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)||
	 		(sub_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE))
#endif	/* DUAL_LCD */
		{
#if (defined(CAMERA_MODULE_WITH_LCD))
			if (main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)
#else
			if (ext_camera_lcd==MAIN_LCD)
#endif
			{
				main_lcd_fb_update_para.module_id=LCD_UPDATE_MODULE_EXT_CAMERA;
				lcd_fb_update((lcd_frame_update_struct *) &main_lcd_fb_update_para);
				kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
			}
#ifdef DUAL_LCD
#if (defined(CAMERA_MODULE_WITH_LCD))
			else if (sub_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)
#else
			else if (ext_camera_lcd==SUB_LCD)
#endif
			{
				sub_lcd_fb_update_para.module_id=LCD_UPDATE_MODULE_EXT_CAMERA;
				lcd_fb_update((lcd_frame_update_struct *) &sub_lcd_fb_update_para);
				kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
			}
#endif
		}

		if (ext_camera_lcd==MAIN_LCD)
			main_lcd_operation_state=LCD_STANDBY_STATE;
#ifdef DUAL_LCD
		else if (ext_camera_lcd==SUB_LCD)
			sub_lcd_operation_state=LCD_STANDBY_STATE;
#endif
	}
#if (defined(CAMERA_MODULE_WITH_LCD))
	wait_lcd_update();
#endif
	get_NFI_bus();

	//Camera_DI->exit_cam_module_preview(); //add by WeiD  //2007-7-3 fengzb
	Camera_DI->cam_module_power_off();
	//chenhe for jasperII +
	MCI_CamPowerDown();
//	sleepLily();
	//chenhe for jasperII -
	free_NFI_bus();
#if (defined(CAMERA_MODULE_WITH_LCD))
		main_lcd_operation_state=LCD_STANDBY_STATE;
#ifdef DUAL_LCD
		sub_lcd_operation_state=LCD_STANDBY_STATE;
#endif
#endif
	lcd_power_down();
		
	L1SM_SleepEnable(camera_sleep_mode_handler);
	ext_camera_state=EXT_CAMERA_POWER_OFF_STATE;
}	/* ext_camera_power_off() */

//#include "vim_high_api.h"
//extern UINT16 VIM_HAPI_SetPreviewMode(VIM_HAPI_PREVIEW_MODE MODE);
void ext_camera_preview(ext_camera_para_struct *ext_cam_para, lcd_frame_update_struct *lcd_para)
{
	mmi_trace(g_sw_CAMERA,"Excute func: ext_camera_preview is TRUE");
	if (ext_camera_state!=EXT_CAMERA_STANDBY_STATE)
		ASSERT(0);  

	lcd_power_up();
	ext_camera_lcd=lcd_para->lcd_id;
	ext_camera_preview_lcd_data.module_id=LCD_UPDATE_MODULE_EXT_CAMERA;
	ext_camera_preview_lcd_data.lcd_id=ext_camera_lcd;
	ext_camera_preview_lcd_data.fb_update_mode=lcd_para->fb_update_mode;
	ext_camera_preview_lcd_data.lcm_start_x=lcd_para->lcm_start_x;
	ext_camera_preview_lcd_data.lcm_start_y=lcd_para->lcm_start_y;
	ext_camera_preview_lcd_data.lcm_end_x=lcd_para->lcm_end_x;
	ext_camera_preview_lcd_data.lcm_end_y=lcd_para->lcm_end_y;
	ext_camera_preview_lcd_data.roi_offset_x=lcd_para->roi_offset_x;
	ext_camera_preview_lcd_data.roi_offset_y=lcd_para->roi_offset_y;
	ext_camera_preview_lcd_data.update_layer=lcd_para->update_layer;

	if (ext_camera_lcd==MAIN_LCD)
		main_lcd_operation_state=LCD_CAMERA_UPDATE_STATE;
#ifdef DUAL_LCD
	else if (ext_camera_lcd==SUB_LCD)
		sub_lcd_operation_state=LCD_CAMERA_UPDATE_STATE;
#endif
	else
		ASSERT(0);   

	L1SM_SleepDisable(camera_sleep_mode_handler);
	/* 050309 MC Lin Add */
	ext_camera_config_data.image_width=ext_cam_para->image_width;
	ext_camera_config_data.image_height=ext_cam_para->image_height;
	/* 050309end of MC Lin Add */
	ext_camera_config_data.preview_width=ext_cam_para->preview_width;
	ext_camera_config_data.preview_height=ext_cam_para->preview_height;
	ext_camera_config_data.white_balance_mode1=ext_cam_para->white_balance_mode1;
	ext_camera_config_data.ev_value=ext_cam_para->ev_value;
	ext_camera_config_data.gamma_table=ext_cam_para->gamma_table;
	ext_camera_config_data.image_effect1=ext_cam_para->image_effect1;
	ext_camera_config_data.frame_rate=ext_cam_para->frame_rate;
	ext_camera_config_data.image_buffer_ptr=ext_cam_para->image_buffer_ptr;
	ext_camera_config_data.buffer_width=ext_cam_para->buffer_width;
	ext_camera_config_data.buffer_height=ext_cam_para->buffer_height;
	ext_camera_config_data.preview_offset_x = ext_cam_para->preview_offset_x;
	ext_camera_config_data.preview_offset_y = ext_cam_para->preview_offset_y;
	ext_camera_config_data.preview_rotate = ext_cam_para->preview_rotate;
	ext_camera_config_data.snapshot_number=ext_cam_para->snapshot_number;
	ext_camera_config_data.banding_freq=ext_cam_para->banding_freq;
	ext_camera_config_data.image_buffer_ptr=ext_cam_para->image_buffer_ptr;
	ext_camera_config_data.zoom_factor=ext_cam_para->zoom_factor;
	ext_camera_config_data.total_file_size=0;
	ext_camera_config_data.captured_picture=0;
	ext_camera_config_data.continue_capture = ext_cam_para->continue_capture;
    ext_camera_config_data.src_key_color = ext_cam_para->src_key_color;//2007-6-27 fengzb add
	mmi_trace(g_sw_CAMERA,"image_width = %d,image_height =%d,buffer_width =%d,buffer_height =%d,jpeg_buffer = %x,image_buffer =%x,image_size =%ld",ext_camera_config_data.image_width,ext_camera_config_data.image_height,
		ext_camera_config_data.buffer_width,ext_camera_config_data.buffer_height,ext_camera_config_data.jpeg_buffer_ptr,ext_camera_config_data.image_buffer_ptr,ext_camera_config_data.image_buffer_size);
	
	mmi_trace(g_sw_CAMERA,"ext_camera_preview preview_offset_x = %d,preview_offset_y =%d,preview_width =%d,preview_height =%d",ext_cam_para->preview_offset_x,ext_cam_para->preview_offset_y,ext_cam_para->preview_width,ext_cam_para->preview_height);
	mmi_trace(g_sw_CAMERA,"ext_camera_config_data preview_offset_x = %d,preview_offset_y =%d,preview_width =%d,preview_height =%d",ext_camera_config_data.preview_offset_x,ext_camera_config_data.preview_offset_y,ext_camera_config_data.preview_width,ext_camera_config_data.preview_height);
#ifdef CAMERA_MODULE_WITH_LCD
	
	ext_camera_config_data.lcd_id=lcd_para->lcd_id;	
	ext_camera_config_data.src_key_color=ext_cam_para->src_key_color;
	ext_camera_config_data.preview_offset_x = lcd_para->lcm_start_x;
	ext_camera_config_data.preview_offset_y = lcd_para->lcm_start_y;
	ext_camera_config_data.video_mode = ext_cam_para->video_mode;

	ext_camera_lcd_menu_data.fb_update_mode=LCD_SW_TRIGGER_MODE;		
	ext_camera_lcd_menu_data.block_mode=LCD_WM_CONT_MODE;				
	ext_camera_lcd_menu_data.dest_block_width=lcd_para->lcm_end_x-lcd_para->lcm_start_x+1;		
	ext_camera_lcd_menu_data.dest_buffer_address=(kal_uint32)ext_cam_para->image_buffer_ptr;	
	ext_camera_lcd_menu_data.dest_buffer_size=ext_cam_para->image_buffer_size;    
	ext_camera_lcd_menu_data.roi_offset_x=lcd_para->roi_offset_x;			
	ext_camera_lcd_menu_data.roi_offset_y=lcd_para->roi_offset_y;			
	ext_camera_lcd_menu_data.roi_width=lcd_para->lcm_end_x-lcd_para->lcm_start_x+1;				
	ext_camera_lcd_menu_data.roi_height=lcd_para->lcm_end_y-lcd_para->lcm_start_y+1;				
	ext_camera_lcd_menu_data.update_layer=lcd_para->update_layer;        
   ext_camera_lcd_menu_data.hw_update_layer=0;     

	wait_lcd_update();
	ext_camera_osd_buffer_data.lcd_id=ext_camera_preview_lcd_data.lcd_id;
	ext_camera_osd_buffer_data.lcm_start_x=ext_camera_preview_lcd_data.lcm_start_x;
	ext_camera_osd_buffer_data.lcm_start_y=ext_camera_preview_lcd_data.lcm_start_y;
	ext_camera_osd_buffer_data.lcm_end_x=ext_camera_preview_lcd_data.lcm_end_x;
	ext_camera_osd_buffer_data.lcm_end_y=ext_camera_preview_lcd_data.lcm_end_y;
	ext_camera_osd_buffer_data.dest_buffer_address=(kal_uint32)ext_camera_config_data.image_buffer_ptr;
	ext_camera_osd_buffer_data.dest_buffer_size=ext_camera_config_data.image_buffer_size;
	ext_camera_osd_buffer_data.src_key_color=ext_camera_config_data.src_key_color;
	ext_camera_osd_buffer_data.force_update=0;
	lcd_fb_update_to_memory(&ext_camera_lcd_menu_data);
	get_NFI_bus();

	Camera_DI->cam_module_write_lcdfb(&ext_camera_osd_buffer_data);

	free_NFI_bus();
#endif

#if (defined(CAMERA_MODULE_WITH_LCD))
	if(ext_cam_para->video_mode)
		ext_camera_state=EXT_CAMERA_VIDEO_RECORD_STATE;
	else
#else
		ext_camera_state=EXT_CAMERA_PREVIEW_STATE;
#endif
	get_NFI_bus();

//	Camera_DI->cam_module_power_on();//2007-7-3 fengzb
	Camera_DI->cam_module_preview((ext_camera_para_struct_di*)&ext_camera_config_data);


	free_NFI_bus();
	lcd_power_down();
}	/* ext_camera_preview() */

void exit_ext_camera_preview(void)
{
	lcd_power_up();
#if (defined(CAMERA_MODULE_WITH_LCD))	
	if ((ext_camera_state!=EXT_CAMERA_PREVIEW_STATE) && (ext_camera_state!=EXT_CAMERA_VIDEO_RECORD_STATE))
#else
	if (ext_camera_state!=EXT_CAMERA_PREVIEW_STATE)
#endif
		ASSERT(0);
	L1SM_SleepEnable(camera_sleep_mode_handler);
	ext_camera_state=EXT_CAMERA_STANDBY_STATE;

#if (defined(CAMERA_MODULE_WITH_LCD))
	wait_lcd_update();
#ifndef DUAL_LCD
			if (main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)
#else
			if ((main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)||
		 		(sub_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE))
#endif	/* DUAL_LCD */
			{
				ext_camera_osd_buffer_data.lcd_id=ext_camera_preview_lcd_data.lcd_id;
				ext_camera_osd_buffer_data.lcm_start_x=ext_camera_preview_lcd_data.lcm_start_x;
				ext_camera_osd_buffer_data.lcm_start_y=ext_camera_preview_lcd_data.lcm_start_y;
				ext_camera_osd_buffer_data.lcm_end_x=ext_camera_preview_lcd_data.lcm_end_x;
				ext_camera_osd_buffer_data.lcm_end_y=ext_camera_preview_lcd_data.lcm_end_y;
				ext_camera_osd_buffer_data.dest_buffer_address=(kal_uint32)ext_camera_config_data.image_buffer_ptr;
				ext_camera_osd_buffer_data.dest_buffer_size=ext_camera_config_data.image_buffer_size;
				ext_camera_osd_buffer_data.force_update=0;

//		if (ext_camera_lcd==MAIN_LCD)
		if (main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)
				{
					lcd_fb_update_to_memory(&ext_camera_lcd_menu_data);
					get_NFI_bus();
					Camera_DI->cam_module_write_lcdfb(&ext_camera_osd_buffer_data);
					free_NFI_bus();
					kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
				}
				#ifdef DUAL_LCD
//			else if (ext_camera_lcd==SUB_LCD)
			else if (sub_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)
					{
						lcd_fb_update_to_memory(&ext_camera_lcd_menu_data);
						get_NFI_bus();
						Camera_DI->cam_module_write_lcdfb(&ext_camera_osd_buffer_data);
						free_NFI_bus();
						kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
					}
				#endif
			}
		main_lcd_operation_state=LCD_CAMERA_ACTIVE_STATE;
#ifdef DUAL_LCD
		sub_lcd_operation_state=LCD_CAMERA_ACTIVE_STATE;
#endif

	if (Camera_DI->exit_cam_module_preview!=NULL)
	{
		get_NFI_bus();

		Camera_DI->exit_cam_module_preview();
		//Camera_DI->cam_module_power_off();  //new add by WeiD//fengzb
		free_NFI_bus();
	}

#else	// !CAMERA_MODULE_WITH_LCD

	if(Camera_DI->exit_cam_module_preview!=NULL)
	{
		mmi_trace(g_sw_CAMERA, "Exit exit_ext_camera_preview() is TRUE");
		get_NFI_bus();

		Camera_DI->exit_cam_module_preview();
		//Camera_DI->cam_module_power_off(); //new add by WeiD

		free_NFI_bus();
	}

#ifndef DUAL_LCD
			if (main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)
#else
			if ((main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)||
		 		(sub_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE))
#endif	/* DUAL_LCD */
			{
				if (ext_camera_lcd==MAIN_LCD)
				{
						main_lcd_fb_update_para.module_id=LCD_UPDATE_MODULE_EXT_CAMERA;
						lcd_fb_update((lcd_frame_update_struct *) &main_lcd_fb_update_para);
						kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
					}
				#ifdef DUAL_LCD
					else if (ext_camera_lcd==SUB_LCD)
					{
						sub_lcd_fb_update_para.module_id=LCD_UPDATE_MODULE_EXT_CAMERA;
						lcd_fb_update((lcd_frame_update_struct *) &sub_lcd_fb_update_para);
						kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
				}
			#endif
			}

	if (ext_camera_lcd==MAIN_LCD)
		main_lcd_operation_state=LCD_STANDBY_STATE;
#ifdef DUAL_LCD
	else if (ext_camera_lcd==SUB_LCD)
		sub_lcd_operation_state=LCD_STANDBY_STATE;
#endif
#endif
	lcd_power_down();
}	/* exit_ext_camera_preview() */

void ext_camera_capture(ext_camera_para_struct *ext_cam_para)
{
	mmi_trace(g_sw_CAMERA,"Excut funs: ext_camera_capture is TRUE");
	
	lcd_power_up();
	if (ext_camera_state!=EXT_CAMERA_PREVIEW_STATE)
		ASSERT(0);

	burst_snapshot_number=ext_cam_para->snapshot_number;

#if 0
	ext_camera_config_data.burst_capture_ptr=ext_cam_para->burst_capture_ptr;
#endif
	ext_camera_config_data.image_width=ext_cam_para->image_width;
	ext_camera_config_data.image_height=ext_cam_para->image_height;
	ext_camera_config_data.jpeg_compression_ratio=ext_cam_para->jpeg_compression_ratio;
	ext_camera_config_data.snapshot_number=ext_cam_para->snapshot_number;
	ext_camera_config_data.jpeg_buffer_ptr=ext_cam_para->jpeg_buffer_ptr;
	ext_camera_config_data.image_buffer_size=ext_cam_para->image_buffer_size;
	ext_camera_config_data.total_file_size=0;
	ext_camera_config_data.captured_picture=0;
	ext_camera_config_data.sticker_capture=0;
	ext_camera_config_data.continue_capture = ext_cam_para->continue_capture;

	get_NFI_bus();

	Camera_DI->cam_module_capture((ext_camera_para_struct_di*)(&ext_camera_config_data));

	free_NFI_bus();
	ext_camera_state=EXT_CAMERA_CAPTURE_STATE;
	lcd_power_down();
}	/* ext_camera_capture() */

kal_uint8 ext_camera_video_frame_rate(void)
{
	volatile kal_uint8 result;

	get_NFI_bus();

	result=Camera_DI->cam_module_frame_rate();

	free_NFI_bus();
	return (result);
//	return (ext_cam_func->cam_module_frame_rate());
}

kal_uint32 ext_camera_video_clip(ext_camera_para_struct *ext_cam_para)
{
	kal_uint32 jpeg_file_size;

#ifndef CAMERA_MODULE_WITH_LCD

	if (Camera_DI->cam_module_jpeg_encode!=NULL)
	{
		video_clip_mode=KAL_TRUE;
		lcd_power_up();
		if (ext_camera_state!=EXT_CAMERA_PREVIEW_STATE)
			ASSERT(0);

		get_NFI_bus();

		while (!Camera_DI->cam_module_preview_ready_check())
		{
			free_NFI_bus();
			kal_sleep_task(1);
			if (main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)
			{
				if (ext_camera_lcd==MAIN_LCD)
				{
					main_lcd_fb_update_para.module_id=LCD_UPDATE_MODULE_EXT_CAMERA;
					lcd_fb_update((lcd_frame_update_struct *) &main_lcd_fb_update_para);
					kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
				}
			}
			get_NFI_bus();
		};
		free_NFI_bus();

		ext_cam_para->image_width=ext_camera_config_data.preview_width;
		ext_cam_para->image_height=ext_camera_config_data.preview_height;
		ext_cam_para->buffer_width=ext_camera_config_data.buffer_width;

		get_NFI_bus();

		Camera_DI->cam_module_jpeg_encode((ext_camera_para_struct_di*)ext_cam_para,KAL_TRUE);

		free_NFI_bus();

	#ifndef DUAL_LCD
		if (main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)
	#else
		if ((main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)||
	 		(sub_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE))
	#endif	/* DUAL_LCD */
		{
			if (ext_camera_lcd==MAIN_LCD)
			{
				main_lcd_fb_update_para.module_id=LCD_UPDATE_MODULE_EXT_CAMERA;
				lcd_fb_update((lcd_frame_update_struct *) &main_lcd_fb_update_para);
				kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
			}
		#ifdef DUAL_LCD
			else if (ext_camera_lcd==SUB_LCD)
			{
				sub_lcd_fb_update_para.module_id=LCD_UPDATE_MODULE_EXT_CAMERA;
				lcd_fb_update((lcd_frame_update_struct *) &sub_lcd_fb_update_para);
				kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
			}
		#endif
		}
	#ifndef DUAL_LCD
		else if (main_lcd_operation_state==LCD_CAMERA_UPDATE_STATE)
	#else
		else if ((main_lcd_operation_state==LCD_CAMERA_UPDATE_STATE)||
	 				(sub_lcd_operation_state==LCD_CAMERA_UPDATE_STATE))
	#endif	/* DUAL_LCD */
		{
			lcd_fb_update((lcd_frame_update_struct *) &ext_camera_preview_lcd_data);
		}
		else
			ASSERT(0);

		do
		{
			kal_sleep_task(1);
			if (main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)
			{
				if (ext_camera_lcd==MAIN_LCD)
				{
					main_lcd_fb_update_para.module_id=LCD_UPDATE_MODULE_EXT_CAMERA;
					lcd_fb_update((lcd_frame_update_struct *) &main_lcd_fb_update_para);
					kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
				}
			}
			get_NFI_bus();

			jpeg_file_size=Camera_DI->cam_module_capture_ready_check();

			free_NFI_bus();
		}	while (jpeg_file_size==NULL_JPEG_FILE);
		kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
		lcd_power_down();
		video_clip_mode=KAL_FALSE;
		return jpeg_file_size;
	}
	else
		return NULL_JPEG_FILE;

#else		
		ext_camera_capture((ext_camera_para_struct *)&ext_cam_para);
		do
		{
			if (main_lcd_operation_state==LCD_CAMERA_CMD_QUEUE_STATE)
			{
				if (ext_camera_lcd==MAIN_LCD)
				{
					main_lcd_fb_update_para.module_id=LCD_UPDATE_MODULE_EXT_CAMERA;
					lcd_fb_update((lcd_frame_update_struct *) &main_lcd_fb_update_para);
					kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
				}
			}

			kal_sleep_task(1);
			get_NFI_bus();

			jpeg_file_size=Camera_DI->cam_module_capture_ready_check();

			free_NFI_bus();
		}	while (jpeg_file_size==NULL_JPEG_FILE);
		return jpeg_file_size;
#endif
}	/* ext_camera_video_clip() */
void lcd_fb_update_to_memory(lcd_frame_update_to_mem_struct *lcd_para);

kal_uint32 ext_camera_sticker_capture(ext_camera_para_struct *ext_cam_para, lcd_frame_update_to_mem_struct *lcd_para)
{
	kal_uint32 jpeg_file_size;

#ifndef CAMERA_MODULE_WITH_LCD

	if (Camera_DI->cam_module_jpeg_encode!=NULL)
	{
		lcd_power_up();
	/* for testing */
		lcd_para->fb_update_mode=LCD_SW_TRIGGER_MODE;
		lcd_para->block_mode=LCD_WM_CONT_MODE;
		lcd_para->hw_update_layer=0;

		get_NFI_bus();

		while (!Camera_DI->cam_module_preview_ready_check()) {};	/* wait until one picture is get from external camera */

		free_NFI_bus();

	   if (ext_camera_lcd==MAIN_LCD)
	      main_lcd_operation_state=LCD_STANDBY_STATE;
	#ifdef DUAL_LCD
		else if (ext_camera_lcd==SUB_LCD)
	      sub_lcd_operation_state=LCD_STANDBY_STATE;
	#endif
		lcd_fb_update_to_memory(lcd_para);
		lcd_power_up();

		ext_cam_para->image_buffer_ptr=(kal_uint16 *) lcd_para->dest_buffer_address;

		ext_cam_para->image_width=lcd_para->roi_width;
		ext_cam_para->image_height=lcd_para->roi_height;
		ext_cam_para->buffer_width=lcd_para->roi_width;

		get_NFI_bus();
		ext_cam_func->cam_module_jpeg_encode(ext_cam_para,KAL_FALSE);
		free_NFI_bus();

		do
		{
			get_NFI_bus();
			jpeg_file_size=ext_cam_func->cam_module_capture_ready_check();
			free_NFI_bus();
		}	while (jpeg_file_size==NULL_JPEG_FILE);

		lcd_power_down();
		L1SM_SleepEnable(camera_sleep_mode_handler);
		ext_camera_state=EXT_CAMERA_STANDBY_STATE;
		return jpeg_file_size;
	}
	else
		return NULL_JPEG_FILE;

#else		
		/* update sticker */
		wait_lcd_update();
		ext_camera_osd_buffer_data.lcm_start_x=lcd_para->roi_offset_x;
		ext_camera_osd_buffer_data.lcm_start_y=lcd_para->roi_offset_y;
		ext_camera_osd_buffer_data.lcm_end_x=lcd_para->roi_offset_x+lcd_para->roi_width-1;
		ext_camera_osd_buffer_data.lcm_end_y=lcd_para->roi_offset_y+lcd_para->roi_height-1;
		ext_camera_osd_buffer_data.dest_buffer_address=lcd_para->dest_buffer_address;
		ext_camera_osd_buffer_data.dest_buffer_size=lcd_para->dest_buffer_size;
		ext_camera_osd_buffer_data.force_update=0;
		lcd_fb_update_to_memory(lcd_para);
		get_NFI_bus();
		ext_cam_func->cam_module_write_lcdfb(&ext_camera_osd_buffer_data);
		free_NFI_bus();

		ext_camera_config_data.burst_capture_ptr=ext_cam_para->burst_capture_ptr;
		ext_camera_config_data.image_width=ext_cam_para->image_width;
		ext_camera_config_data.image_height=ext_cam_para->image_height;
		ext_camera_config_data.jpeg_compression_ratio=ext_cam_para->jpeg_compression_ratio;
		ext_camera_config_data.snapshot_number=ext_cam_para->snapshot_number;
		ext_camera_config_data.jpeg_buffer_ptr=ext_cam_para->jpeg_buffer_ptr;
		ext_camera_config_data.image_buffer_size=ext_cam_para->image_buffer_size;
		ext_camera_config_data.total_file_size=0;
		ext_camera_config_data.captured_picture=0;
		ext_camera_config_data.sticker_capture=1;
		ext_camera_config_data.continue_capture = ext_cam_para->continue_capture;

		get_NFI_bus();
		ext_cam_func->cam_module_capture(&ext_camera_config_data);
		free_NFI_bus();
		ext_camera_state=EXT_CAMERA_CAPTURE_STATE;
		//ext_camera_capture(ext_cam_para);
		do
		{
			get_NFI_bus();
			jpeg_file_size=ext_cam_func->cam_module_capture_ready_check();
			free_NFI_bus();
		}	while (jpeg_file_size==NULL_JPEG_FILE);
      main_lcd_operation_state=LCD_CAMERA_ACTIVE_STATE;
   #ifdef DUAL_LCD
      sub_lcd_operation_state=LCD_CAMERA_ACTIVE_STATE;
   #endif
		ext_camera_state = EXT_CAMERA_STANDBY_STATE;
		return jpeg_file_size;
#endif
}	/* ext_camera_sticker_capture() */
void ext_camera_set_parameter(kal_uint8 cmd, kal_uint8 para)
{
	lcd_power_up();
	get_NFI_bus();

	Camera_DI->cam_module_set_para(cmd,para);

	free_NFI_bus();
	lcd_power_down();
}	/* ext_camera_set_parameter() */

kal_bool write_ext_camera(kal_uint16 cmd, kal_uint16 para)
{
//	kal_uint8 current_lcd_state;
	engineering_mode_setting=KAL_TRUE;
	ext_cam_reg_setting[((kal_uint8)cmd)]=1;
	ext_cam_reg_value[((kal_uint8)cmd)]=para;
	return KAL_TRUE;

}

kal_bool read_ext_camera(kal_uint16 cmd, kal_uint16 *para)
{
	kal_uint8 current_lcd_state;

	current_lcd_state=main_lcd_operation_state;
	lcd_power_up();
	main_lcd_operation_state=LCD_CAMERA_UPDATE_STATE;	/* temp state to prevent lcd interface power down */

	if (ext_camera_state==EXT_CAMERA_POWER_OFF_STATE)
	{
		get_NFI_bus();
		ext_cam_func->cam_module_power_on();
		free_NFI_bus();
		ext_camera_state=EXT_CAMERA_INITIAL_STATE;
	}
	while (ext_camera_reset_status()!=KAL_TRUE) {};	/* wait until external camera ready */

	if (ext_cam_func->cam_module_read_para!=NULL)
	{
		get_NFI_bus();
		*para=ext_cam_func->cam_module_read_para(cmd);
		free_NFI_bus();
		main_lcd_operation_state=current_lcd_state;
		lcd_power_down();
		ext_camera_power_off();
	   return KAL_TRUE;
   }
	else
	{
		main_lcd_operation_state=current_lcd_state;
		lcd_power_down();
		ext_camera_power_off();
		return KAL_FALSE;
	}
}

void ext_camera_set_osd_buffer(kal_uint32 osd_addr, kal_uint32 osd_size)
{
#ifdef CAMERA_MODULE_WITH_LCD
	ext_camera_lcd_menu_data.dest_buffer_address=osd_addr;
	ext_camera_lcd_menu_data.dest_buffer_size=osd_size;
	ext_camera_osd_buffer_data.dest_buffer_address=osd_addr;
	ext_camera_osd_buffer_data.dest_buffer_size=osd_size;
#endif
}

void ext_camera_update_backend_lcd_buffer(lcd_frame_update_struct *ext_camera_osd_data)
{
#ifdef CAMERA_MODULE_WITH_LCD
	kal_uint32 save_irq_mask;

	if (ext_camera_osd_data->lcd_id==MAIN_LCD)
	{
		if ((main_lcd_operation_state!=LCD_CAMERA_ACTIVE_STATE)&&
			 (main_lcd_operation_state!=LCD_CAMERA_UPDATE_STATE))
		{
			ASSERT(0);
		}
	}
#if (defined(DUAL_LCD))
	else if (ext_camera_osd_data->lcd_id==SUB_LCD)
	{
		if ((sub_lcd_operation_state!=LCD_CAMERA_ACTIVE_STATE)&&
			 (sub_lcd_operation_state!=LCD_CAMERA_UPDATE_STATE))
		{
			ASSERT(0);
		}
	}
#endif

	save_irq_mask=SaveAndSetIRQMask();
	if (ext_camera_osd_data->lcd_id==MAIN_LCD)
		main_lcd_operation_state=LCD_CAMERA_ACTIVE_UPDATE_STATE;
#ifdef DUAL_LCD
	else if (ext_camera_osd_data->lcd_id==SUB_LCD)
		sub_lcd_operation_state=LCD_CAMERA_ACTIVE_UPDATE_STATE;
#endif
	RestoreIRQMask(save_irq_mask);

	ext_camera_lcd_menu_data.fb_update_mode=ext_camera_osd_data->fb_update_mode;
	ext_camera_lcd_menu_data.block_mode=LCD_WM_CONT_MODE;
	ext_camera_lcd_menu_data.roi_offset_x=ext_camera_osd_data->roi_offset_x;
	ext_camera_lcd_menu_data.roi_offset_y=ext_camera_osd_data->roi_offset_y;
	ext_camera_lcd_menu_data.roi_width=ext_camera_osd_data->lcm_end_x-ext_camera_osd_data->lcm_start_x+1;
	ext_camera_lcd_menu_data.roi_height=ext_camera_osd_data->lcm_end_y-ext_camera_osd_data->lcm_start_y+1;
	ext_camera_lcd_menu_data.update_layer=ext_camera_osd_data->update_layer;

	ext_camera_osd_buffer_data.lcd_id=ext_camera_osd_data->lcd_id;
	ext_camera_osd_buffer_data.lcm_start_x=ext_camera_osd_data->lcm_start_x;
	ext_camera_osd_buffer_data.lcm_start_y=ext_camera_osd_data->lcm_start_y;
	ext_camera_osd_buffer_data.lcm_end_x=ext_camera_osd_data->lcm_end_x;
	ext_camera_osd_buffer_data.lcm_end_y=ext_camera_osd_data->lcm_end_y;
	ext_camera_osd_buffer_data.force_update=1;
	
	ext_camera_osd_buffer_data.dest_buffer_address=ext_camera_lcd_menu_data.dest_buffer_address;

	lcd_fb_update_to_memory(&ext_camera_lcd_menu_data);
	get_NFI_bus();

	Camera_DI->cam_module_write_lcdfb(&ext_camera_osd_buffer_data);

	free_NFI_bus();

	save_irq_mask=SaveAndSetIRQMask();

	main_lcd_operation_state=LCD_CAMERA_ACTIVE_STATE;
#ifdef DUAL_LCD
	sub_lcd_operation_state=LCD_CAMERA_ACTIVE_STATE;
#endif
	RestoreIRQMask(save_irq_mask);

	kal_set_eg_events(lcd_event_id,LCD_CMD_COMPLETE_EVENT,KAL_OR);
#endif
}

void ext_camera_start_video_encode(ext_camera_retrieve_video_struct *ext_camera_retrieve_video_para)
{
#ifdef CAMERA_MODULE_WITH_LCD
   get_NFI_bus();
   ext_cam_func->cam_module_start_video_encode(ext_camera_retrieve_video_para);
   free_NFI_bus();
#endif
}

void ext_camera_retrieve_video_data(ext_camera_retrieve_video_struct *ext_camera_retrieve_video_para)
{
#ifdef CAMERA_MODULE_WITH_LCD
   get_NFI_bus();
   ext_cam_func->cam_module_retrieve_video_data(ext_camera_retrieve_video_para);
   free_NFI_bus();
#endif
}

void ext_camera_pause_video_encode(ext_camera_retrieve_video_struct *ext_camera_retrieve_video_para)
{
#ifdef CAMERA_MODULE_WITH_LCD
   get_NFI_bus();
   ext_cam_func->cam_module_pause_video_encode(ext_camera_retrieve_video_para);
   free_NFI_bus();
#endif
}

void ext_camera_resume_video_encode(void)
{
#ifdef CAMERA_MODULE_WITH_LCD
   get_NFI_bus();
   ext_cam_func->cam_module_resume_video_encode();
   free_NFI_bus();
#endif   
}

void ext_camera_stop_video_encode(ext_camera_retrieve_video_struct *ext_camera_retrieve_video_para)
{
#ifdef CAMERA_MODULE_WITH_LCD
   get_NFI_bus();
   ext_cam_func->cam_module_stop_video_encode(ext_camera_retrieve_video_para);
   free_NFI_bus();
#endif
}


#endif /* CAMERA_MODULE */

