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
 *	 mdi_video.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	 Interface to access video related functions.
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
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#include "mmi_features.h"
#ifdef __MMI_VIDEO__
#define __NEWSIMULATOR 
#include "stdc.h"
#include "l4dr1.h"

#include "mmi_data_types.h"
#include "fat_fs.h"
#include "lcd_if.h"				/* lcd interface */
#include "gdi_include.h"		/* include data type */	
#include "lcd_sw_rnd.h"
#include "gui.h"
#include "queuegprot.h"			/* message handling */
#include "med_api.h"				/* media task */
#include "med_main.h"
#include "med_struct.h"
#include "mdi_datatype.h"
#include "mdi_video.h"	
#include "queuegprot.h"			/* message handling */
#include "eventsgprot.h"		/* event hadnling - get message return from camera driver */
#include "gpioinc.h"				/* avoid sleep when using camera */
#include "fat_fs.h"
#include "debuginitdef.h"		/* mmi_assert */
#ifdef __MMI_TVOUT__
	#include "mdi_tv.h"
#endif 
#include "vdoplyapp.h"
#include "resource_video_skins.h"
#include "mmi_trace.h"
#include "keybrd.h"   

/***************************************************************************** 
* Local Variable
*****************************************************************************/
mdi_video_context_struct	g_mdi_video_cntx;
mdi_video_context_struct	*mdi_video_p			= &g_mdi_video_cntx;
mdi_video_setting_struct	*video_setting_p		= &g_mdi_video_cntx.setting;

/* callback function */
void (*mdi_video_rec_save_result_callback)(MDI_RESULT) = NULL;
void (*mdi_video_rec_record_result_callback)(MDI_RESULT) = NULL;
void (*mdi_video_ply_open_file_result_callback)(MDI_RESULT, mdi_video_info_struct*) = NULL;
void (*mdi_video_ply_play_finish_callback)(MDI_RESULT) = NULL;
void (*mdi_video_play_seek_result_callback)(MDI_RESULT) = NULL;
void (*mdi_video_get_play_time_result_callback)(UINT32) = NULL;


/***************************************************************************** 
* Local Function
*****************************************************************************/
static void mdi_video_rec_save_result_hdlr(void *msg_ptr);
static void mdi_video_rec_record_result_hdlr(void *msg_ptr);
static void mdi_video_ply_play_finish_result_hdlr(void *msgPtr);
static void mdi_video_ply_open_file_result_hdlr(void *msgPtr);
static void mdi_video_ply_non_block_seek_result_hdlr(void *msgPtr);




/***************************************************************************** 
* Extern Variable
*****************************************************************************/
#if defined(__J2ME__) && !defined(__NATIVE_JAVA__)
	extern void (*mma_mpeg_ply_open_file_result_hdlr)(void*);
	extern void (*mma_mpeg_ply_play_finish_result_hdlr)(void*);
	extern void (*mma_mpeg_rec_save_result_hdlr)(void*);
#endif
U16 g_vdoply_width;
U16 g_vdoply_height;
U16 g_vdoply_totaltime;
U16 gZoomHeight, gZoomWidth;
extern BOOL b_video_playing;



/****to declare fucntions ***/
extern void media_vidrec_stop(module_type src_mod_id);
extern void media_vidrec_previewStop(module_type src_mod_id);
extern kal_int32 media_vidRec_pause(module_type src_mod_id);
extern kal_int32 media_vidRec_resume(module_type src_mod_id);
extern void media_vidRec_set_param(module_type src_mod_id, kal_uint16 param_id, kal_int16 value);
extern void media_vid_display_set(module_type src_mod_id,kal_uint16 ZoomWidth, kal_uint16 ZoomHeight, kal_int16 startX, kal_int16 startY, kal_int16 cutX, kal_int16 cutY, kal_int16 cutW, kal_int16 cutH, kal_uint16 rotate);
extern void media_vid_get_current_play_time_req(module_type src_mod_id);
/****************************************************************************
*
* RECORDER                                                                 
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	mdi_video_rec_init
* DESCRIPTION
*	Init mdi video recorder 
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_rec_init(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
 	return MDI_RES_VDOREC_SUCCEED;
}



/*****************************************************************************
* FUNCTION
*	mdi_video_rec_power_on
* DESCRIPTION
*	power on recorder hw
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_rec_power_on(void)
{
#ifdef MMI_ON_HARDWARE_P
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 ret;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/


	/* in 6219, power up will always success it is si fast that we may treat as blocking fucntion */
//		ret = media_cam_power_up(MOD_MMI, 0);
	ret = MED_RES_OK;//chenhe for jasperII

	if(ret != MED_RES_OK)
		return MDI_RES_VDOREC_ERR_POWER_ON_FAILED;
	
#endif /* MMI_ON_HARDWARE_P */

	return MDI_RES_VDOREC_SUCCEED;
}



/*****************************************************************************
* FUNCTION
*	mdi_video_rec_power_off
* DESCRIPTION
*	power off recorder hw
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_rec_power_off(void)
{
#ifdef MMI_ON_HARDWARE_P	
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	S32 ret;
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	ret = media_cam_power_down(MOD_MMI, 100);
	
#endif /* MMI_ON_HARDWARE_P */


	/* power off alwyas return succeed */
	return MDI_RES_VDOREC_SUCCEED;
}



/*****************************************************************************
* FUNCTION
*	mdi_video_rec_preview_start
* DESCRIPTION
*	preview start
* PARAMETERS
*	preview_layer_handle			IN		layer for preview
*	preview_wnd_offset_x			IN		window offset x
*	preview_wnd_offset_y			IN		window offset y
*	preview_wnd_width				IN		window width
*	preview_wnd_height			IN		window height
*	blt_layer_flag					IN		blt out layer flag
*	preview_layer_flag			IN		preview layer flag
*	setting_p						IN		setting data
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/

MDI_RESULT mdi_video_rec_preview_start(gdi_handle						preview_layer_handle,
													U32								blt_layer_flag,
													U32								preview_layer_flag,
													mdi_video_setting_struct	*setting_p)					/* video setting */
{

#ifdef MMI_ON_HARDWARE_P		
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	media_vid_preview_req_struct	cam_preview_data;

	U8							*buf_ptr;	
	S32						layer_width;
	S32						layer_height;
	S32						layer_offset_x;
	S32						layer_offset_y;
	S32						lcd_width;
	S32						lcd_height;
	S32 						temp_x;
	MDI_RESULT			ret;
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	/* store video's parameters */
	memset(video_setting_p,0,sizeof(mdi_video_setting_struct));
	memcpy(video_setting_p, setting_p, sizeof(mdi_video_setting_struct));
	

	gdi_layer_push_and_set_active(preview_layer_handle);
	gdi_layer_get_buffer_ptr(&buf_ptr);
	gdi_layer_get_dimension(&layer_width, &layer_height);
	gdi_layer_get_position(&layer_offset_x, &layer_offset_y);
	gdi_layer_pop_and_restore_active();

	gdi_lcd_get_dimension(&lcd_width, &lcd_height);
	if((setting_p->lcd_rotate == MDI_VIDEO_LCD_ROTATE_270) ||
		(setting_p->lcd_rotate == MDI_VIDEO_LCD_ROTATE_270_MIRROR))
	{
		temp_x			= layer_offset_x;
		layer_offset_x = GDI_LCD_WIDTH-layer_offset_y-layer_height;
		layer_offset_y = temp_x;
	}
	else if(	(setting_p->lcd_rotate == MDI_VIDEO_LCD_ROTATE_90)||
				(setting_p->lcd_rotate == MDI_VIDEO_LCD_ROTATE_90_MIRROR))
	{
		/* havent been tested */
		temp_x			= layer_offset_x;
		layer_offset_x = layer_offset_y;
		layer_offset_y = GDI_LCD_HEIGHT-temp_x-layer_width;
	}
	
	cam_preview_data.buffer_width			= (kal_uint16)layer_width;
	cam_preview_data.buffer_height		= (kal_uint16)layer_height;
	cam_preview_data.image_buffer_size  = (kal_uint32)((layer_width*layer_height*GDI_LAYER.act_bit_per_pixel)>>3);
   cam_preview_data.image_buffer_p		= (void*)buf_ptr;

	cam_preview_data.preview_width		= layer_width;
	cam_preview_data.preview_height		= layer_height;

	/* set settings parameters */ 
	cam_preview_data.WB				= video_setting_p->wb;
	cam_preview_data.exposure		= video_setting_p->ev;
	cam_preview_data.banding_freq	= (U8)video_setting_p->banding;	
	cam_preview_data.effect			= video_setting_p->effect;
	cam_preview_data.zoom_factor	= video_setting_p->zoom;
	cam_preview_data.size_limit  = video_setting_p->size_limit;
	cam_preview_data.time_limit  = video_setting_p->time_limit;
	cam_preview_data.record_aud = video_setting_p->record_aud;
//   	cam_preview_data.encode_quality 	= (U8)video_setting_p->video_qty;
#if 	1
		switch(video_setting_p->video_qty)
		{
			case 0:
			case 1:
				cam_preview_data.encode_quality=0;
				break;
			default:
				cam_preview_data.encode_quality=1;
				break;
		}
#endif	
	/* other para */
	cam_preview_data.brightness		= video_setting_p->brightness;
	cam_preview_data.contrast			= video_setting_p->contrast;
	cam_preview_data.saturation		= video_setting_p->saturation;	
	cam_preview_data.hue					= video_setting_p->hue;
	cam_preview_data.night_mode		= video_setting_p->night;	
	cam_preview_data.gamma				= 0;
	cam_preview_data.frame_rate		= VIDEO_PERVIEW_FRAME_RATE;

       mmi_trace(1,"setting mdi_video_rec_preview_start video_qty=%d,encode_quality=%d,",video_setting_p->video_qty,cam_preview_data.encode_quality);
       mmi_trace(1,"setting mdi_video_rec_preview_start wb=%d,ev=%d,",video_setting_p->wb,video_setting_p->ev);
       mmi_trace(1,"setting mdi_video_rec_preview_start effect=%d,zoom=%d,",video_setting_p->effect,video_setting_p->zoom);
       mmi_trace(1,"setting mdi_video_rec_preview_start night=%d,record_aud=%d,",video_setting_p->night,video_setting_p->record_aud);
       mmi_trace(1,"setting mdi_video_rec_preview_start size_limit=%d,time_limit=%d,",video_setting_p->size_limit,video_setting_p->time_limit);

	/* rotate */
	cam_preview_data.rotate 			= video_setting_p->rotate;
	cam_preview_data.lcd_rotate		= video_setting_p->lcd_rotate;

	#if (defined(MT6228)||defined(MT6229))
		cam_preview_data.image_pitch_mode 	= FALSE;
		cam_preview_data.overlay_frame_mode = FALSE;
		cam_preview_data.manual_wb				= FALSE;
		cam_preview_data.tv_output				= FALSE;
	#endif
	
	cam_preview_data.lcd_id				= MAIN_LCD;

	/* lcd start x, start y are coordinate on LCM, is the region will be update by driver */
	cam_preview_data.lcd_start_x		= layer_offset_x;
	cam_preview_data.lcd_start_y		= layer_offset_y;

	if( (setting_p->lcd_rotate == MDI_VIDEO_LCD_ROTATE_270) ||
		 (setting_p->lcd_rotate == MDI_VIDEO_LCD_ROTATE_90) ||
		 (setting_p->lcd_rotate == MDI_VIDEO_LCD_ROTATE_270_MIRROR) ||
		 (setting_p->lcd_rotate == MDI_VIDEO_LCD_ROTATE_90_MIRROR) )
	{
		/* rotate 90 or 270 for horizonal view */
		cam_preview_data.lcd_end_x	= layer_offset_x+layer_height-1;
		cam_preview_data.lcd_end_y	= layer_offset_y+layer_width-1;		
	}
	else
	{
		cam_preview_data.lcd_end_x	= layer_offset_x+layer_width-1;
		cam_preview_data.lcd_end_y	= layer_offset_y+layer_height-1;	
	}

	/* preview offset x, y are offset from lcd_start_x, lcd_start_y */
	cam_preview_data.preview_offset_y = 0;
	cam_preview_data.preview_offset_x = 0;	
	
	cam_preview_data.roi_offset_x	= (kal_uint16)GDI_LAYER_OFFSET+layer_offset_x;
	cam_preview_data.roi_offset_y	= (kal_uint16)GDI_LAYER_OFFSET+layer_offset_y;	

#if defined(CAMERA_MODULE)
	cam_preview_data.update_layer		= blt_layer_flag;
#elif defined(ISP_SUPPORT)
	cam_preview_data.update_layer		= blt_layer_flag^preview_layer_flag;
#endif /* chiip version */
	cam_preview_data.hw_update_layer	= preview_layer_flag;

//#ifdef ISP_SUPPORT
	switch(video_setting_p->video_size)
	{
		case MDI_VIDEO_VIDEO_SIZE_SQCIF:
			cam_preview_data.image_width 	= 128;
			cam_preview_data.image_height = 96;
			break;

		case MDI_VIDEO_VIDEO_SIZE_QQVGA:
			cam_preview_data.image_width 	= 160;
			cam_preview_data.image_height = 120;			
			break;

		case MDI_VIDEO_VIDEO_SIZE_QCIF:
			cam_preview_data.image_width 	= 176;
			cam_preview_data.image_height = 144;			
			break;

		case MDI_VIDEO_VIDEO_SIZE_CIF:
			cam_preview_data.image_width 	= 352;
			cam_preview_data.image_height = 288;			
			break; 			

		default:
			ASSERT(0);
	}
	
    cam_preview_data.image_width 	= 176;
    cam_preview_data.image_height = 144;	

    #if(LCD_WIDTH<LCD_HEIGHT)			
        cam_preview_data.preview_width=LCD_WIDTH;
        cam_preview_data.preview_height=(LCD_WIDTH*3)>>2;    
    #else    
        cam_preview_data.preview_width=LCD_HEIGHT;
        cam_preview_data.preview_height=(LCD_HEIGHT*3)>>2;    
    #endif
   
   //add sheen.
   if(cam_preview_data.preview_height > g_vdorec_osd_cntx.preview_wnd.rect.height)
   	cam_preview_data.preview_height = g_vdorec_osd_cntx.preview_wnd.rect.height;
   
   cam_preview_data.lcd_start_x		= g_vdorec_osd_cntx.preview_wnd.rect.offset_x;
   cam_preview_data.lcd_start_y		= g_vdorec_osd_cntx.preview_wnd.rect.offset_y+
   									((g_vdorec_osd_cntx.preview_wnd.rect.height-cam_preview_data.preview_height)/2);
  //add end.
   
   cam_preview_data.bit_stream_type	= (U8)video_setting_p->video_format;
   cam_preview_data.gray_mode			= 0;
	cam_preview_data.seq_num			= 0;
//#endif /* ISP_SUPPORT *///delete by zhangl 11-18

	/* TV out */
	#ifdef __MMI_TVOUT__

		if(mdi_tvout_is_enable())
		{
			g_mdi_video_cntx.tvout_prev_owner = mdi_tvout_get_owner();	

			if(g_mdi_video_cntx.tvout_prev_owner != MDI_TV_OWNER_VIDEO)
			{
				mdi_tvout_set_owner(MDI_TV_OWNER_VIDEO);
			}

			/* set tvout owner to Video */
			mdi_tvout_set_owner(MDI_TV_OWNER_VIDEO);

			ret = mdi_tvout_get_para(	MDI_TV_OWNER_VIDEO, 
												(PU16)&cam_preview_data.tv_output_mode,					/* mode_ptr */
												(PU16)&cam_preview_data.tv_output_width,					/* buf_width_ptr */
												(PU16)&cam_preview_data.tv_output_height,					/* buf_height_ptr */
												(PU32)&cam_preview_data.tv_output_buffer_size,			/* buf size */
												(PU8*)&cam_preview_data.tv_output_buffer1_address, 	/* buf1 pp*/
												(PU8*)&cam_preview_data.tv_output_buffer2_address);  	/* buf2 pp*/

			if(ret == MDI_RES_TV_ENABLE)
			{
				/* check if buffer is enough */
				if(cam_preview_data.tv_output_width*cam_preview_data.tv_output_height*2 > 
					cam_preview_data.tv_output_buffer_size)
				{
					MMI_ASSERT(0);
				}
				
				cam_preview_data.tv_output		= TRUE;

				/* TV out. force update full screen */
				cam_preview_data.lcd_start_x	= 0;
				cam_preview_data.lcd_start_y	= 0;
				cam_preview_data.lcd_end_x		= UI_device_width-1;
				cam_preview_data.lcd_end_y		= UI_device_height-1;
				cam_preview_data.roi_offset_x	= GDI_LAYER_OFFSET;
				cam_preview_data.roi_offset_y	= GDI_LAYER_OFFSET;
				
				cam_preview_data.preview_offset_x = layer_offset_x;
				cam_preview_data.preview_offset_y = layer_offset_y;				
			}
			else
			{
				/* tvout not enable , restore owner to previous owner */
				if(g_mdi_video_cntx.tvout_prev_owner != MDI_TV_OWNER_VIDEO)
				{
					mdi_tvout_set_owner(g_mdi_video_cntx.tvout_prev_owner);
				}

				cam_preview_data.tv_output = FALSE;
			}
		} /* is tv out enable or not */
	#endif /* __MMI_TVOUT__ */
/*
	mmi_trace(TRUE,"preview_offset_x:%d--preview_offset_y:%d",cam_preview_data.preview_offset_x,cam_preview_data.preview_offset_y);
	mmi_trace(TRUE,"preview_width:%d--preview_height:%d",cam_preview_data.preview_width,cam_preview_data.preview_height);
	mmi_trace(TRUE,"image_width:%d--image_height:%d",cam_preview_data.image_width,cam_preview_data.image_height);
	mmi_trace(TRUE,"night_mode:%d--encode_quality:%d",cam_preview_data.night_mode,cam_preview_data.encode_quality);
	mmi_trace(TRUE,"zoom_factor:%d--effect:%d",cam_preview_data.zoom_factor,cam_preview_data.effect);
	mmi_trace(TRUE,"WB:%d--contrast:%d--brightness:%d",cam_preview_data.WB,cam_preview_data.contrast,cam_preview_data.brightness);
*/

	ret = media_vid_preview(MOD_MMI, &cam_preview_data);


	if(ret != MED_RES_OK)
		return MDI_RES_VDOREC_ERR_PREVIEW_FAILED;
#endif /* MMI_ON_HARDWARE_P */

	return MDI_RES_VDOREC_SUCCEED;

}



/*****************************************************************************
* FUNCTION
*	mdi_video_rec_preview_stop
* DESCRIPTION
*	stop recorder preview
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_rec_preview_stop(void)
{
#ifdef MMI_ON_HARDWARE_P	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	media_vid_stop(MOD_MMI);

	#ifdef __MMI_TVOUT__
		/* restore to previous owner */
		if(g_mdi_video_cntx.tvout_prev_owner != MDI_TV_OWNER_VIDEO)
		{
			mdi_tvout_set_owner(g_mdi_video_cntx.tvout_prev_owner);
		}
	#endif /* __MMI_TVOUT__ */

#endif /* MMI_ON_HARDWARE_P */	
	/* stop alywas return succeed */
	return MDI_RES_CAMERA_SUCCEED;
}


/*****************************************************************************
* FUNCTION
*	clear_vdoRec_result_cb
* DESCRIPTION
*	clear the callback
* PARAMETERS
* null
* RETURNS
*	VOID
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void clear_vdoRec_result_cb(void)
{
	mdi_video_rec_record_result_callback = NULL;
}

/*****************************************************************************
* FUNCTION
*	mdi_video_rec_record_start
* DESCRIPTION
*	start recording
* PARAMETERS
*	filename							IN 	video clip's filename
*	record_result_callback		IN		record result callback function ptr
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_rec_record_start(FS_HANDLE filehandle, void (*record_result_callback)(MDI_RESULT) )
{
#ifdef MMI_ON_HARDWARE_P		
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
   media_vid_record_req_struct record;
	S32					ret;

   
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	ASSERT(record_result_callback != NULL);

	mdi_video_rec_record_result_callback = record_result_callback; 
	SetProtocolEventHandler(mdi_video_rec_record_result_hdlr, MSG_ID_MEDIA_VID_RECORD_FINISH_IND);	

   record.media_mode			= MED_MODE_FILE;
   record.data					= (kal_int32)filehandle;
   record.record_audio 		= (U8)video_setting_p->record_aud;
   record.file_size_limit 	= (U32)(video_setting_p->size_limit*1024); /* K */;
   record.seq_num				= g_mdi_video_cntx.record_seq_num;

   ret = media_vid_record(MOD_MMI, (void*)&record);
//mmi_trace(g_sw_VdoRec,"guoj++ mdi_video_rec_record_start filename = %s,ret is %d",tmp,ret);
	if(ret != MED_RES_OK)
	{
		switch(ret)
		{
			case MED_RES_DISC_FULL:
			case MDI_RES_VDOREC_DISK_FULL:
			case MDI_RES_VDOREC_ERR_DISK_FULL:
				return MDI_RES_VDOREC_ERR_DISK_FULL;
		
			case MED_RES_WRITE_PROTECTION:
				return MDI_RES_VDOREC_ERR_WRITE_PROTECTION;
				
			case MDI_RES_VDOREC_REACH_DISK_LIMIT:
				return MDI_RES_VDOREC_REACH_DISK_LIMIT;

			default:
		return MDI_RES_VDOREC_ERR_RECORD_FAILED;
		}
	}
#endif /* MMI_ON_HARDWARE_P */

	return MDI_RES_VDOREC_SUCCEED;
}



/*****************************************************************************
* FUNCTION
*	mdi_video_rec_record_stop
* DESCRIPTION
*	stop recording
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_rec_record_stop(void)
{
#ifdef MMI_ON_HARDWARE_P	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	mmi_trace(g_sw_VdoRec," mdi_video_rec_record_stop");
	g_mdi_video_cntx.record_seq_num++;
	media_vidrec_stop(MOD_MMI);


	#ifdef __MMI_TVOUT__
		/* restore to previous owner */
		if(g_mdi_video_cntx.tvout_prev_owner != MDI_TV_OWNER_VIDEO)
		{
			mdi_tvout_set_owner(g_mdi_video_cntx.tvout_prev_owner);
		}
	#endif /* __MMI_TVOUT__ */
	
#endif /* MMI_ON_HARDWARE_P */

	/* stop always return succeed */
	return MDI_RES_VDOREC_SUCCEED;
}


//chenhe for jasperII+
MDI_RESULT mdi_video_rec_record_previewStop(void)
{
#ifdef MMI_ON_HARDWARE_P	

	media_vidrec_previewStop(MOD_MMI);

#endif /* MMI_ON_HARDWARE_P */

	/* stop always return succeed */
	return MDI_RES_VDOREC_SUCCEED;
}
//chenhe for jasperII-



/*****************************************************************************
* FUNCTION
*	mdi_video_rec_record_pause
* DESCRIPTION
*	pause recording
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_rec_record_pause(void)
{
#ifdef MMI_ON_HARDWARE_P		
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 ret;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	//ret = media_vid_pause(stack_int_get_active_module_id());
	ret = media_vidRec_pause(MOD_MMI);

	if(ret == MED_RES_BUSY)
	{
		/* 
			ret ==  MED_RES_BUSY many happend at one cirtical situation.
			When driver recorde almost reach time limit, and user keep pause-resume,

			So the driver will send play finish msg to MDI as follow timing.
			
			ResumeA   PauseA	 ResumeB	 PauseB
  			  |			|			|			|
			--|----x----|--------|---x----|--------            
			       |           		 |
					finish msg---------> 
											 MMI receive msg here 	

			so the PauseA and ResumeB will return MED_RES_BUSY and shall ignore. 
		*/

		/* increase the flag, will ignore recrod finish hdlr from media */
		g_mdi_video_cntx.record_seq_num++;

		/* 
			mmi see this return shall enter saving handling procedure, since 
		   the record finish callback will not be called 
		*/
		
		return MDI_RES_VDOREC_RECORD_ALREADY_STOP;
		
	}
	else if(ret == MED_RES_END_OF_FILE)
	{
		return MDI_RES_VDOREC_REACH_SIZE_LIMIT;
	}

	else if(ret == MED_RES_DISC_FULL)
	{
		return MDI_RES_VDOREC_ERR_DISK_FULL;
	}
	else if(ret != MED_RES_OK)
	{
		return MDI_RES_VDOREC_ERR_PAUSE_FAILED;
	}
	else
	{
		return MDI_RES_VDOREC_SUCCEED;
	}

#else
	return MDI_RES_VDOREC_SUCCEED;
#endif /* MMI_ON_HARDWARE  */

}



/*****************************************************************************
* FUNCTION
*	mdi_video_rec_record_resume
* DESCRIPTION
*	resume recording
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_rec_record_resume(void)
{
#ifdef MMI_ON_HARDWARE_P		
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 ret= 0;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	//ret = media_vid_resume(stack_int_get_active_module_id());
	media_vidRec_resume(MOD_MMI);
	mmi_trace(1,"mdi_video_rec_record_resume,ret is %d",ret);
	if(ret == MED_RES_BUSY)
	{
		/* 
			ret ==  MED_RES_BUSY many happend at one cirtical situation.
			When driver recorde almost reach time limit, and user keep pause-resume,

			So the driver will send play finish msg to MDI as follow timing.
			
			ResumeA   PauseA	 ResumeB	 PauseC
  			  |			|			|			|
			--|----x----|--------|---x----|--------            
			       |           		 |
					finish msg---------> 
											 MMI receive msg here 	

			so the PauseA and ResumeB will return MED_RES_BUSY and shall ignore. 
		*/

		/* increase the flag, will ignore recrod finish hdlr from media */
		g_mdi_video_cntx.record_seq_num++;

		/* 
			mmi see this return shall enter saving handling procedure, since 
		   the record finish callback will not be called 
		*/
		   
		return MDI_RES_VDOREC_RECORD_ALREADY_STOP;
		
	}
	else if(ret != MED_RES_OK)
	{
		return MDI_RES_VDOREC_ERR_RESUME_FAILED;
	}
	else
	{
		return MDI_RES_VDOREC_SUCCEED;
	}

#else
	return MDI_RES_VDOREC_SUCCEED;
#endif /* MMI_ON_HARDWARE */


}

 

/*****************************************************************************
* FUNCTION
*	mdi_video_rec_get_cur_record_time
* DESCRIPTION
*	get current recorded time
* PARAMETERS
*	cur_record_time	OUT	current recorded time
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_video_rec_get_cur_record_time(U32 *cur_record_time)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	/* time scale is [ms] */
//#ifdef   MMI_ON_HARDWARE_P	  //guojian delete for jasper 2007.12.11
#if 0	 
	media_vid_get_current_record_time(MOD_MMI, cur_record_time);   
#else
	*cur_record_time += 400; 
#endif 
}



/*****************************************************************************
* FUNCTION
*	mdi_video_rec_has_unsaved_file
* DESCRIPTION
*	check if there is an unsaved video clip
* PARAMETERS
*	filepath		IN		path to check
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
BOOL mdi_video_rec_has_unsaved_file(S8 *filepath)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
#ifdef MMI_ON_HARDWARE_P	
	return media_vid_check_unfinished_file(MOD_MMI);
#else
	return FALSE;
#endif /* MMI_ON_HARDWARE_P */
}



/*****************************************************************************
* FUNCTION
*	mdi_video_rec_delete_unsaved_file
* DESCRIPTION
*	delete unsaved video file
* PARAMETERS
*	filepath		IN		path of unsaved video temp file
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_video_rec_delete_unsaved_file(S8 *filepath)
{
#ifdef MMI_ON_HARDWARE_P	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	media_vid_clean_temp_files(MOD_MMI);
#endif /* MMI_ON_HARDWARE_P */
}



/*****************************************************************************
* FUNCTION
*	mdi_video_rec_delete_unsaved_file  
* DESCRIPTION 
*	merge and save file, [Non-Blocking]
* PARAMETERS
*	filepath						IN		path to merge file
*	save_result_callback		IN		merge and save result
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_rec_save_file(S8 *filepath, void (*save_result_callback)(MDI_RESULT))
{
#ifdef MMI_ON_HARDWARE_P	
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	module_type module_id=stack_int_get_active_module_id();

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	ASSERT(save_result_callback != NULL);
	module_id = MOD_MMI;
	mdi_video_rec_save_result_callback = save_result_callback;
	if (module_id == MOD_MMI)
		SetProtocolEventHandler(mdi_video_rec_save_result_hdlr, MSG_ID_MEDIA_VID_FILE_MERGE_DONE_IND);
#if defined(__J2ME__) && !defined(__NATIVE_JAVA__)
	else if (module_id == MOD_J2ME)
		mma_mpeg_rec_save_result_hdlr = mdi_video_rec_save_result_hdlr;		
#endif

	media_vid_file_merge(module_id, (kal_wchar*)filepath);
	
#endif /* MMI_ON_HARDWARE_P */

	return MDI_RES_VDOREC_SUCCEED;
}



/*****************************************************************************
* FUNCTION
*	mdi_video_rec_update_para_ev
* DESCRIPTION
*	update ev setting value 
* PARAMETERS
*	para	IN		ev setting parameter
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void 	mdi_video_rec_update_para_ev(U16 para)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	ASSERT(para < MDI_VIDEO_EV_COUNT);

	mdi_video_p->setting.ev = para;

#ifdef MMI_ON_HARDWARE_P	
//	media_vidRec_set_param(MOD_MMI, CAM_PARAM_EXPOSURE, para);
media_vidRec_set_param(MOD_MMI, CAM_PARAM_BRIGHTNESS, para);
#endif

}



/*****************************************************************************
* FUNCTION
*	mdi_video_rec_update_para_zoom
* DESCRIPTION
*	update zoom setting value 
* PARAMETERS
*	para	IN		zoom setting parameter
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void  mdi_video_rec_update_para_zoom(U16 para)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	mdi_video_p->setting.zoom = para;

#ifdef MMI_ON_HARDWARE_P	
	media_vidRec_set_param(MOD_MMI, CAM_PARAM_ZOOM_FACTOR, para);
#endif

}


/*****************************************************************************
* FUNCTION
*	mdi_video_rec_update_para_effect
* DESCRIPTION
*	update effect setting value 
* PARAMETERS
*	para	IN		effect setting parameter
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_video_rec_update_para_effect(U16 para)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	//ASSERT(para < MDI_VIDEO_EFFECT_COUNT);

	video_setting_p->effect = para;
	mmi_trace(1," mdi_video_rec_update_para_effect video_setting_p->effect  = %d",video_setting_p->effect );

#ifdef MMI_ON_HARDWARE_P	
	media_vidRec_set_param(MOD_MMI, CAM_PARAM_EFFECT, para);
#endif 
}

/*****************************************************************************
* FUNCTION
*	mdi_video_rec_update_para_wb
* DESCRIPTION
*	update wb setting value 
* PARAMETERS
*	para	IN		ev setting parameter
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_video_rec_update_para_wb(U16 para)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	ASSERT(para < MDI_VIDEO_WB_COUNT);
	
	video_setting_p->wb = para;

#ifdef MMI_ON_HARDWARE_P	
	media_vidRec_set_param(MOD_MMI, CAM_PARAM_WB, para);
#endif
}



/*****************************************************************************
* FUNCTION
*	mdi_video_rec_update_para_night
* DESCRIPTION
*	update night setting value 
* PARAMETERS
*	para	IN		ev setting parameter
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_video_rec_update_para_night(U16 para)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	video_setting_p->night = para;

#ifdef MMI_ON_HARDWARE_P	
	media_vidRec_set_param(MOD_MMI, CAM_PARAM_NIGHT_MODE, para);
#endif
}

/*****************************************************************************
* FUNCTION
*	mdi_video_rec_update_para_ct
* DESCRIPTION
*	update contrast setting value 
* PARAMETERS
*	para	IN		ct setting parameter
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_video_rec_update_para_ct(U16 para)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	video_setting_p->contrast= para;

#ifdef MMI_ON_HARDWARE_P	
	media_vidRec_set_param(MOD_MMI, CAM_PARAM_CONTRAST, para);
#endif
}



/*****************************************************************************
* FUNCTION
*	mdi_video_rec_save_result_hdlr
* DESCRIPTION
*	save result message hdlr
* PARAMETERS
*	msg_ptr	IN		message pointer
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
static void mdi_video_rec_save_result_hdlr(void *msg_ptr)
{
#ifdef MMI_ON_HARDWARE_P
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	media_vid_file_merge_done_ind_struct *ind_p;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	ind_p = (media_vid_file_merge_done_ind_struct*)msg_ptr;
	
	if(ind_p->result == MED_RES_OK)
		mdi_video_rec_save_result_callback(MDI_RES_VDOREC_SUCCEED);
	else if(ind_p->result == MED_RES_DISC_FULL)
		mdi_video_rec_save_result_callback(MDI_RES_VDOREC_ERR_DISK_FULL);	
	else
		mdi_video_rec_save_result_callback(MDI_RES_VDOREC_ERR_FAILED);
	
#endif /* MMI_ON_HARDWARE_P */
} 


/*****************************************************************************
* FUNCTION
*	mdi_video_rec_record_result_hdlr
* DESCRIPTION
*	record result message hdlr
* PARAMETERS
*	msg_ptr	IN		message pointer
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
static void mdi_video_rec_record_result_hdlr(void *msg_ptr)
{
#ifdef MMI_ON_HARDWARE_P	
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	media_vid_record_finish_ind_struct *ind_p;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	ind_p = (media_vid_record_finish_ind_struct*)msg_ptr;

  mmi_trace(1, "############## [MDI Vidrec]: MMI get the msg --- MSG_ID_MEDIA_VID_RECORD_FINISH_IND ############# \n");
#if 0
	/* seq number not match, measn it is already stopped, ignore this message */
	if(g_mdi_video_cntx.record_seq_num != ind_p->seq_num)
		return;
#endif
	#ifdef __MMI_TVOUT__
		/* restore to previous owner */
		if(g_mdi_video_cntx.tvout_prev_owner != MDI_TV_OWNER_VIDEO)
		{
			mdi_tvout_set_owner(g_mdi_video_cntx.tvout_prev_owner);
		}
	#endif /* __MMI_TVOUT__ */

	if(mdi_video_rec_record_result_callback)
	{
		/* successful save file */
		if(ind_p->result == MED_RES_END_OF_FILE)
		mdi_video_rec_record_result_callback(MDI_RES_VDOREC_REACH_SIZE_LIMIT);
		else if(ind_p->result == MED_RES_DISC_FULL||MDI_RES_VDOREC_DISK_FULL == ind_p->result)
		mdi_video_rec_record_result_callback(MDI_RES_VDOREC_DISK_FULL);
		else if(MDI_RES_VDOREC_ERR_FILE_SIZE == ind_p->result)
		mdi_video_rec_record_result_callback(MDI_RES_VDOREC_ERR_FILE_SIZE);	
		else 
		mdi_video_rec_record_result_callback(MDI_RES_VDOREC_ERR_FAILED);
	}

#endif /* MMI_ON_HARDWARE_P */
} 




/****************************************************************************
*
* Video Player
*
****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  mdi_video_is_playing
 * DESCRIPTION
 *  To check video is currently playing or not
 * PARAMETERS
 *  void
 * RETURN VALUES
 *  MMI_TRUE : Video is playing now.
 *  MMI_FALSE : Video is not playing now.
 *****************************************************************************/
BOOL mdi_video_is_playing(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
  
    return mmi_vdoply_is_in_playing();
  
}






/*****************************************************************************
* FUNCTION
*	mdi_video_ply_init
* DESCRIPTION
*	init mdi video player
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_ply_init(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	g_mdi_video_cntx.open_seq_num 	= 0;
	g_mdi_video_cntx.play_seq_num 	= 0;
	g_mdi_video_cntx.record_seq_num 	= 0;
	g_mdi_video_cntx.seek_seq_num		= 0;

	g_mdi_video_cntx.is_seeking		= FALSE;

	#ifdef __MMI_TVOUT__
		g_mdi_video_cntx.tvout_prev_owner = MDI_TV_OWNER_GDI;
 	#endif /* __MMI_TVOUT__ */	
 	
   /* init shall always succeed */	
 	return MDI_RES_VDOPLY_SUCCEED;
}


/*****************************************************************************
* FUNCTION
*	mdi_video_ply_open_file
* DESCRIPTION
*	open video file
* PARAMETERS
*	filename					IN 	videe clip's full file name
*	open_done_callback	IN		open file result  callback function
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_ply_open_file(	const S8 *filename,  
												void (*open_result_callback)(MDI_RESULT, mdi_video_info_struct*),
												void (*videoplayer_drawtime_hdlr)(U32))
{
#ifdef MMI_ON_HARDWARE_P		
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 ret;
	module_type module_id;
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/\
	TBM_ENTRY(0x2A71);
	ret = MED_RES_FAIL;	
	mdi_video_ply_open_file_result_callback = open_result_callback;

	//module_id = stack_int_get_active_module_id();
	
	module_id=MOD_MMI;
	if(MOD_MMI == module_id)
		SetProtocolEventHandler(mdi_video_ply_open_file_result_hdlr, MSG_ID_MEDIA_VID_FILE_READY_IND);
#if defined(__J2ME__) && !defined(__NATIVE_JAVA__)
	else if(MOD_J2ME == module_id)
		mma_mpeg_ply_open_file_result_hdlr = mdi_video_ply_open_file_result_hdlr;
#endif

   	ret = media_vid_open_file(module_id, (void*)filename, g_mdi_video_cntx.open_seq_num,(void*)videoplayer_drawtime_hdlr);
	//mdi_video_ply_open_file_result_hdlr(0);//chenhe for jasperII
	mmi_trace(1,"mdi_video_ply_open_file success");
	/* play from file alway enable audio */
	g_mdi_video_cntx.is_play_audio = TRUE;

	if(ret != MED_RES_OK)
	{
		TBM_EXIT(0x2A71);
		return MDI_RES_VDOPLY_ERR_OPEN_FILE_FAILED;
	}
	else
	{
		TBM_EXIT(0x2A71);
		return MDI_RES_VDOPLY_SUCCEED;
	}
#else
	{
		/* for WIN32 simulation */
		mdi_video_info_struct vdo_clip;
		
		vdo_clip.width						= 176;
		vdo_clip.height					= 144;
		vdo_clip.total_frame_count		= 1000;
		vdo_clip.total_time_duration	= 30;
		mdi_video_ply_open_file_result_callback = open_result_callback;
		mdi_video_ply_open_file_result_callback(MDI_RES_VDOPLY_SUCCEED, &vdo_clip);
		TBM_EXIT(0x2A71);
		return MDI_RES_VDOPLY_SUCCEED;		
	}
#endif /* MMI_ON_HARDWARE_P */

}



/*****************************************************************************
* FUNCTION
*	mdi_video_ply_close_file
* DESCRIPTION
*	close video file
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_ply_close_file(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
#ifdef MMI_ON_HARDWARE_P		
	module_type module_id = stack_int_get_active_module_id();

	TBM_ENTRY(0x2A72);
	module_id = MOD_MMI;
	g_mdi_video_cntx.open_seq_num++;
	//   media_vid_close_file(stack_int_get_active_module_id());
	media_vid_close_file(module_id);
#endif /* MMI_ON_HARDWARE_P */

	/* close file shall always succeed */
	TBM_EXIT(0x2A72);
	return MDI_RES_VDOPLY_SUCCEED;
}



/*****************************************************************************
* FUNCTION
*	mdi_video_ply_snapshot
* DESCRIPTION
*	snapshot video and save as jpeg file
* PARAMETERS
*	layer_handle	IN	 	video play layer
*	file_name		IN		filename for jpeg	
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_ply_snapshot(gdi_handle layer_handle, PS8 file_name)
{
#ifdef MMI_ON_HARDWARE_P	
		
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	img_encode_struct encode_data;
	S32 					image_width;
	S32					image_height;
	PU8					image_ptr;
	U32					file_size;
	S32 					ret;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/		
	gdi_layer_push_and_set_active(layer_handle);
	gdi_layer_get_dimension(&image_width, &image_height);
	gdi_layer_get_buffer_ptr(&image_ptr);
	gdi_layer_pop_and_restore_active();

	encode_data.image_width 		= image_width;
	encode_data.image_height 		= image_height;

	encode_data.image_quality 		= CAM_JPG_QTY_HIGH;
	encode_data.image_buffer_p		= (void*)image_ptr;
	encode_data.image_buffer_size = (image_width*image_height*GDI_MAINLCD_BIT_PER_PIXEL)>>3;

	encode_data.target_width 		= image_width;
	encode_data.target_height 		= image_height;
	encode_data.media_mode			= MED_MODE_FILE;
	encode_data.media_type 			= MED_TYPE_JPG;

   encode_data.data					= (void*)file_name;
   encode_data.file_size_p			= (kal_uint32*)&file_size;
   encode_data.blocking				= (kal_bool)TRUE;		/* blocking */
   encode_data.seq_num				= (kal_uint16)0;		/* not used */
   encode_data.file_buffer_len	= (kal_uint32)0; 		/* not used */
	encode_data.gray_mode			= 0;

   ret = media_img_encode(MOD_MMI, &encode_data);

	if(ret == MED_RES_OK)
		return MDI_RES_VDOPLY_SUCCEED;
	else if(ret == MED_RES_DISC_FULL)
		return MDI_RES_VDOPLY_ERR_SNAPSHOT_DISK_FULL;
	else if(ret == MED_RES_WRITE_PROTECTION)
		return MDI_RES_VDOPLY_ERR_SNAPSHOT_WRITE_PROTECTION;
	else if(ret == MED_RES_NO_DISC)
		return MDI_RES_VDOPLY_ERR_SNAPSHOT_NO_DISK;
	else
		return MDI_RES_VDOPLY_ERR_SNAPSHOT_FAILED;

#else
	return MDI_RES_VDOPLY_SUCCEED;
#endif /* MMI_ON_HARDWARE_P */
	
}



/*****************************************************************************
* FUNCTION
*	mdi_video_ply_snapshot
* DESCRIPTION
*	snapshot video and save as jpeg file
* PARAMETERS
*	player_layer_handle 	IN		play layer handle
*	blt_layer_flag			IN		layer to blt to LCM
*	repeat_count			IN  	repeat how many times
*	is_visual_update		IN 	update to LCM or not
*	audio_path				IN 	audio output path
*	play_layer_flag		IN		layer for HW to draw 
*  rotate 					IN 	rotate
*	play_speed				IN		play speed
*	play_finish_hdlr		IN		play finish callback
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_ply_play(gdi_handle	player_layer_handle,
										U32			blt_layer_flag,							/* which layer shall be blt by driver */
										U32			play_layer_flag,
										U16			repeat_count,
										BOOL			is_visual_update,
										U8				audio_path,
										U16			rotate,
										S16			play_speed,
										void			(*play_finish_callback)(MDI_RESULT))	/* call back when play finied */
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
#ifdef MMI_ON_HARDWARE_P		
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 layer_width=0;
	S32 layer_height = 0;
	S32 offset_x = 0;
	S32 offset_y = 0;
	U8	 *buf_ptr=NULL;
	S32 ret = 0;
	S32 temp_x = 0;
   vid_play_struct play_data;
	module_type module_id;
   
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	TBM_ENTRY(0x2A68);
	ret = MED_RES_FAIL;
	
	module_id = stack_int_get_active_module_id();
	module_id = MOD_MMI;

  	/* disable key pad tone */
  	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);

	/* Freeze sublcd */
	#ifdef __MMI_SUBLCD__
		if(is_visual_update)
		{
			gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
			gdi_lcd_freeze(TRUE);
			gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
		}
	#endif /* __MMI_SUBLCD__ */
#if 0
	gdi_layer_push_and_set_active(player_layer_handle);

	gdi_layer_get_buffer_ptr(&buf_ptr);
	gdi_layer_get_dimension(&layer_width, &layer_height);
	gdi_layer_get_position(&offset_x, &offset_y);

	gdi_layer_pop_and_restore_active();
#endif
	mdi_video_ply_play_finish_callback = play_finish_callback;

	if (module_id == MOD_MMI)
		SetProtocolEventHandler(mdi_video_ply_play_finish_result_hdlr,	MSG_ID_MEDIA_VID_PLAY_FINISH_IND);	
#if defined(__J2ME__) && !defined(__NATIVE_JAVA__)
	else if (module_id == MOD_J2ME)
		mma_mpeg_ply_play_finish_result_hdlr = mdi_video_ply_play_finish_result_hdlr;
#endif

	if((rotate == MDI_VIDEO_LCD_ROTATE_270) ||
		(rotate == MDI_VIDEO_LCD_ROTATE_270_MIRROR))
	{
		temp_x	= offset_x;
		offset_x = GDI_LCD_WIDTH-offset_y-layer_height;
		offset_y = temp_x;
	}
	else if(	(rotate == MDI_VIDEO_LCD_ROTATE_90)||
				(rotate == MDI_VIDEO_LCD_ROTATE_90_MIRROR))
	{
		/* havent been tested */
		temp_x	= offset_x;
		offset_x = offset_y;
		offset_y = GDI_LCD_HEIGHT-temp_x-layer_width;
	}

	play_data.lcd_id			= MAIN_LCD;
	play_data.lcd_start_x	= offset_x;
	play_data.lcd_start_y	= offset_y;

	/* if rotate 90 or 270 degree */
	if( (rotate == GDI_LAYER_ROTATE_90)  || (rotate == GDI_LAYER_ROTATE_90_MIRROR) ||
	    (rotate == GDI_LAYER_ROTATE_270) || (rotate == GDI_LAYER_ROTATE_270_MIRROR) )
	{
		play_data.lcd_end_x	= offset_x+layer_height-1;
		play_data.lcd_end_y	= offset_y+layer_width-1;
	}
	else
	{
		play_data.lcd_end_x	= offset_x+layer_width-1;
		play_data.lcd_end_y	= offset_y+layer_height-1;		
	}

	play_data.display_width			= layer_width;
	play_data.display_height		= layer_height;
	play_data.display_offset_x		= 0; 	/* not used */
	play_data.display_offset_y		= 0;	/* not used */
	play_data.roi_offset_x			= GDI_LAYER_OFFSET+offset_x;
	play_data.roi_offset_y			= GDI_LAYER_OFFSET+offset_y;	
	play_data.image_buffer_p		= (void*)buf_ptr;
	play_data.image_buffer_size	= (layer_width*layer_height*GDI_LAYER.act_bit_per_pixel)>>3;
	play_data.update_layer			= blt_layer_flag;
	play_data.hw_update_layer		= play_layer_flag;
	play_data.play_audio          = g_mdi_video_cntx.is_play_audio;
	play_data.play_speed          = play_speed;
	play_data.audio_path				= audio_path;
	play_data.display_device		= MED_DISPLAY_TO_MAIN_LCD;	
	play_data.seq_num					= g_mdi_video_cntx.play_seq_num;
	play_data.lcd_rotate				= rotate;

   play_data.video_display_offset_x = 0;
   play_data.video_display_offset_y = 0;
   	play_data.lcd_start_x	= 0;
	play_data.lcd_start_y	= 0;

   
	#if (defined(MT6228)||defined(MT6229))
	   play_data.image_pitch_mode = FALSE;
	#endif

	
	if(is_visual_update)
	{
		play_data.display_device = MED_DISPLAY_TO_MAIN_LCD;
	}
	else
	{
		play_data.display_device = MED_DISPLAY_NONE;
	}
	
	if(repeat_count == 0)
	{
		play_data.repeats	= 0xffff;	/* infinite play */
	}
	else
	{
		play_data.repeats	= repeat_count;
	}

	/* TV out */
	#ifdef __MMI_TVOUT__
		if(mdi_tvout_is_enable())
		{
			g_mdi_video_cntx.tvout_prev_owner = mdi_tvout_get_owner();	
						
			if(g_mdi_video_cntx.tvout_prev_owner != MDI_TV_OWNER_VIDEO)
			{
				mdi_tvout_set_owner(MDI_TV_OWNER_VIDEO);
			}

			ret = mdi_tvout_get_para(	MDI_TV_OWNER_VIDEO, 
												NULL,														/* mode_ptr */
												NULL,														/* buf_width_ptr */
												NULL,														/* buf_height_ptr */
												(PU32)&play_data.tv_output_buffer_size,		/* buf size */
												(PU8*)&play_data.tv_output_buffer1_address, 	/* buf1 pp*/
												(PU8*)&play_data.tv_output_buffer2_address); /* buf2 pp*/

			if(ret == MDI_RES_TV_ENABLE)
			{
				play_data.tv_output = TRUE;

				/* TV out. force update full screen */
				play_data.lcd_start_x	= 0;
				play_data.lcd_start_y	= 0;
				play_data.lcd_end_x		= UI_device_width-1;
				play_data.lcd_end_y		= UI_device_height-1;
				play_data.roi_offset_x	= GDI_LAYER_OFFSET;
				play_data.roi_offset_y	= GDI_LAYER_OFFSET;

	   		play_data.video_display_offset_x = offset_x;
			   play_data.video_display_offset_y = offset_y;			
			}
			else
			{
				/* tvout not enable , restore owner to GDI */
				if(g_mdi_video_cntx.tvout_prev_owner != MDI_TV_OWNER_VIDEO)
				{
					mdi_tvout_set_owner(g_mdi_video_cntx.tvout_prev_owner);
				}

				play_data.tv_output = FALSE;
			}
		}

	#endif /* __MMI_TVOUT__ */
	ret = media_vid_play(module_id, &play_data);

	if(ret != MED_RES_OK)
	{
		TBM_EXIT(0x2A68);
		return MDI_RES_VDOPLY_ERR_PLAY_FAILED;
	}
	b_video_playing = 1;
#endif 
	TBM_EXIT(0x2A68);
	return MDI_RES_VDOPLY_SUCCEED;
}







/*****************************************************************************
* FUNCTION
*	mdi_video_display_interface
* DESCRIPTION
*	display the video player interface
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_display_interface(void)
{
#ifdef MMI_ON_HARDWARE_P	
	module_type module_id;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	
	module_id = stack_int_get_active_module_id();
	module_id = MOD_MMI;
	media_vid_display_interface(module_id);




#endif /* MMI_ON_HARDWARE_P */

   /* stop always return succeed */
	return MDI_RES_VDOPLY_SUCCEED;
}



/*****************************************************************************
* FUNCTION
*	mdi_video_ply_stop
* DESCRIPTION
*	stop video playing
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_ply_pause(void)
{
#ifdef MMI_ON_HARDWARE_P	
	module_type module_id;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	
	module_id = stack_int_get_active_module_id();
	g_mdi_video_cntx.play_seq_num++;
//	media_vid_stop(stack_int_get_active_module_id());
	module_id = MOD_MMI;
	media_vid_pause(module_id);

	/* un-freeze sublcd */
	#ifdef __MMI_SUBLCD__	
		gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
		gdi_lcd_freeze(FALSE);
		gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
	#endif /* __MMI_SUBLCD__ */

	#ifdef __MMI_TVOUT__
		/* restore to previous owner */
		if(g_mdi_video_cntx.tvout_prev_owner != MDI_TV_OWNER_VIDEO)
		{
			mdi_tvout_set_owner(g_mdi_video_cntx.tvout_prev_owner);
		}
	#endif /* __MMI_TVOUT__ */

#endif /* MMI_ON_HARDWARE_P */

	b_video_playing = 0;

   /* stop always return succeed */
	return MDI_RES_VDOPLY_SUCCEED;
}


/*****************************************************************************
* FUNCTION
*	mdi_video_ply_stop
* DESCRIPTION
*	stop video playing
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_ply_stop(void)
{
#ifdef MMI_ON_HARDWARE_P	
	module_type module_id;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	
	TBM_ENTRY(0x2A67);
	module_id = stack_int_get_active_module_id();
	g_mdi_video_cntx.play_seq_num++;
//	media_vid_stop(stack_int_get_active_module_id());
	module_id = MOD_MMI;
	media_vid_stop(module_id);

	/* un-freeze sublcd */
	#ifdef __MMI_SUBLCD__	
		gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
		gdi_lcd_freeze(FALSE);
		gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
	#endif /* __MMI_SUBLCD__ */

	#ifdef __MMI_TVOUT__
		/* restore to previous owner */
		if(g_mdi_video_cntx.tvout_prev_owner != MDI_TV_OWNER_VIDEO)
		{
			mdi_tvout_set_owner(g_mdi_video_cntx.tvout_prev_owner);
		}
	#endif /* __MMI_TVOUT__ */

#endif /* MMI_ON_HARDWARE_P */

	b_video_playing = 0;

   /* stop always return succeed */
	TBM_EXIT(0x2A67);
	return MDI_RES_VDOPLY_SUCCEED;
}



/*****************************************************************************
* FUNCTION
*	mdi_video_ply_seek
* DESCRIPTION
*	seek video
* PARAMETERS
*	time		IN		seek time stamp
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_ply_seek(U64 time)
{
#ifdef MMI_ON_HARDWARE_P	
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	vid_seek_struct 	seek_data;
	S32					ret;
	module_type module_id;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	

	TBM_ENTRY(0x2A69);
	
	seek_data.time			= time;
	seek_data.get_frame	= FALSE;
	seek_data.blocking	= TRUE;	
	module_id = stack_int_get_active_module_id();
	
	#if (defined(MT6228)||defined(MT6229))
		seek_data.tv_output = FALSE;
	#endif 

	/* TV out */
	#ifdef __MMI_TVOUT__
		if(mdi_tvout_is_enable())
		{
			if(mdi_tvout_get_owner() == MDI_TV_OWNER_VIDEO)
	 		{
				ret = mdi_tvout_get_para(	MDI_TV_OWNER_VIDEO, 
													NULL,														/* mode_ptr */
													NULL,														/* buf_width_ptr */
													NULL,														/* buf_height_ptr */
													(PU32)&seek_data.tv_output_buffer_size,		/* buf size */
													(PU8*)&seek_data.tv_output_buffer1_address, 	/* buf1 pp*/
													(PU8*)&seek_data.tv_output_buffer2_address); /* buf2 pp*/

				if(ret == MDI_RES_TV_ENABLE)
				{
					seek_data.tv_output = TRUE;		
				}
				else
				{
					seek_data.tv_output = FALSE;
				}
			}
		}
  	#endif /* __MMI_TVOUT__ */

	module_id = MOD_MMI;
	ret = media_vid_seek(module_id, &seek_data);

	if(ret == MED_RES_VIDEO_SEEK_PARTIAL_DONE)
	{
		TBM_EXIT(0x2A69);
		return MDI_RES_VDOPLY_SEEK_PARTIAL_DONE;
	}
	else if(ret != MED_RES_OK)
	{
		TBM_EXIT(0x2A69);
		return MDI_RES_VDOPLY_ERR_SEEK_FAILED;
	}
	
#endif /* MMI_ON_HARDWARE_P */

	TBM_EXIT(0x2A69);
	return MDI_RES_VDOPLY_SUCCEED;
}



/*****************************************************************************
* FUNCTION
*	mdi_video_ply_seek_and_get_frame
* DESCRIPTION
*	seek video and get seek frame
* PARAMETERS
*	time						IN		seek time stamp
*	player_layer_handle	IN		get frame on this layer
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
extern const vdoply_context_struct	g_vdoply_cntx;
extern const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx;
MDI_RESULT mdi_video_ply_seek_and_get_frame(	U64			time,
															gdi_handle	player_layer_handle)
{
#ifdef MMI_ON_HARDWARE_P		
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 	layer_width = 0;
	S32 	layer_height = 0;
	U8	 	*buf_ptr = NULL;
	S32	ret = 0;
	S32 offset_x = 0, offset_y = 0;

	vid_seek_struct seek_data;
	module_type module_id = stack_int_get_active_module_id();

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	TBM_ENTRY(0x2A6E);
	#if 1
	gdi_layer_push_and_set_active(player_layer_handle);

	gdi_layer_get_buffer_ptr(&buf_ptr);
	gdi_layer_get_dimension(&layer_width, &layer_height);
	gdi_layer_get_position(&offset_x, &offset_y);

	gdi_layer_pop_and_restore_active();
	#endif
#ifdef NEVER
//	g_vdoply_cntx.video_info.width = g_vdoply_width;
//	g_vdoply_cntx.video_info.height = g_vdoply_height;
//	origin_offset_y = 75;
	origin_offset_y = 20;
	mmi_trace(TRUE,"guoj++ Func: %s ", __FUNCTION__);
	
	offset_x = g_vdoply_osd_cntx.play_wnd.rect.offset_x +(g_vdoply_osd_cntx.play_wnd.rect.width - g_vdoply_cntx.video_info.width) / 2;
	offset_y = origin_offset_y +(g_vdoply_osd_cntx.play_wnd.rect.height - g_vdoply_cntx.video_info.height) / 2;
#endif /* NEVER */
	mmi_trace(1," mdi_video_ply_seek_and_get_frame (%d,%d,%d,%d) rect(%d,%d,%d,%d)",offset_x,offset_y,g_vdoply_cntx.video_info.width,g_vdoply_cntx.video_info.height,g_vdoply_osd_cntx.play_wnd.rect.offset_x,g_vdoply_osd_cntx.play_wnd.rect.offset_y,g_vdoply_osd_cntx.play_wnd.rect.width,g_vdoply_osd_cntx.play_wnd.rect.height);
	
	seek_data.time						= time;
	seek_data.get_frame				= TRUE;
	seek_data.display_width			= g_vdoply_cntx.video_info.width;
	seek_data.display_height		= g_vdoply_cntx.video_info.height;
	seek_data.display_offset_x		= offset_x;
	seek_data.display_offset_y		= offset_y;
	seek_data.image_buffer_p		= (void*)buf_ptr;
	seek_data.image_buffer_size	= (layer_width*layer_height*GDI_LAYER.act_bit_per_pixel)>>3; 
	seek_data.blocking				= TRUE;	
	#if (defined(MT6228)||defined(MT6229))
      seek_data.tv_output = FALSE;
   #endif 

	/* TV out */
	#ifdef __MMI_TVOUT__
		if(mdi_tvout_get_owner() == MDI_TV_OWNER_VIDEO)
 		{
			/* if owner is VIDEO, means already in mode2 */
			ret = mdi_tvout_get_para(	MDI_TV_OWNER_VIDEO, 
												NULL,														/* mode_ptr */
												NULL,														/* buf_width_ptr */
												NULL,														/* buf_height_ptr */
												(PU32)&seek_data.tv_output_buffer_size,		/* buf size */
												(PU8*)&seek_data.tv_output_buffer1_address, 	/* buf1 pp*/
												(PU8*)&seek_data.tv_output_buffer2_address); /* buf2 pp*/

			if(ret == MDI_RES_TV_ENABLE)
			{
				seek_data.tv_output = TRUE;		
			}
			else
			{
				seek_data.tv_output = FALSE;
			}
		}
  	#endif /* __MMI_TVOUT__ */
	
	module_id = MOD_MMI;

 	ret = media_vid_seek(module_id, &seek_data);

	if(ret == MED_RES_VIDEO_SEEK_PARTIAL_DONE)
	{
		TBM_EXIT(0x2A6E);
		return MDI_RES_VDOPLY_SEEK_PARTIAL_DONE;
	}
	else if(ret != MED_RES_OK)
	{
		TBM_EXIT(0x2A6E);
		return MDI_RES_VDOPLY_ERR_SEEK_FAILED;
	}
		
#endif /* MMI_ON_HARDWARE_P */

	TBM_EXIT(0x2A6E);
	return MDI_RES_VDOREC_SUCCEED;
}



/*****************************************************************************
* FUNCTION
*	mdi_video_ply_non_block_seek
* DESCRIPTION
*	seek video
* PARAMETERS
*	time							IN		seek time stamp
*	seek_result_callback		IN 	seek result callback function
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_ply_non_block_seek(U64 time, void (*seek_result_callback)(MDI_RESULT))
{
#ifdef MMI_ON_HARDWARE_P		
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	vid_seek_struct 	seek_data;
	S32					ret;
	module_type module_id = stack_int_get_active_module_id();

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	TBM_ENTRY(0x2A70);
	SetProtocolEventHandler(mdi_video_ply_non_block_seek_result_hdlr, MSG_ID_MEDIA_VID_SEEK_DONE_IND);
	
	mdi_video_play_seek_result_callback = seek_result_callback;
	
	seek_data.time			= time;
	seek_data.get_frame	= FALSE;
	seek_data.blocking	= FALSE;
	seek_data.seq_num		= g_mdi_video_cntx.seek_seq_num;

	#if (defined(MT6228)||defined(MT6229))
		seek_data.tv_output = FALSE;
	#endif 

	/* TV out */
	#ifdef __MMI_TVOUT__
		if(mdi_tvout_get_owner() == MDI_TV_OWNER_VIDEO)
 		{
			/* if owner is VIDEO, means already in mode2 */
			ret = mdi_tvout_get_para(	MDI_TV_OWNER_VIDEO, 
												NULL,														/* mode_ptr */
												NULL,														/* buf_width_ptr */
												NULL,														/* buf_height_ptr */
												(PU32)&seek_data.tv_output_buffer_size,		/* buf size */
												(PU8*)&seek_data.tv_output_buffer1_address, 	/* buf1 pp*/
												(PU8*)&seek_data.tv_output_buffer2_address); /* buf2 pp*/

			if(ret == MDI_RES_TV_ENABLE)
			{
				seek_data.tv_output = TRUE;		
			}
			else
			{
				seek_data.tv_output = FALSE;
			}
		}
  	#endif /* __MMI_TVOUT__ */
	module_id = MOD_MMI;

	ret = media_vid_seek(module_id, &seek_data);

	if(ret != MED_RES_OK)
	{
		TBM_EXIT(0x2A70);
		return MDI_RES_VDOPLY_ERR_SEEK_FAILED;
	}

	g_mdi_video_cntx.is_seeking = TRUE;
	
#endif /* MMI_ON_HARDWARE_P */

	TBM_EXIT(0x2A70);
	return MDI_RES_VDOPLY_SUCCEED;
}



/*****************************************************************************
* FUNCTION
*	mdi_video_ply_non_block_seek_and_get_frame
* DESCRIPTION
*	seek video and get seek frame
* PARAMETERS
*	time						IN		seek time stamp
*	player_layer_handle	IN		get frame on this layer
*	seek_result_callback	IN		seek result callback function
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/

MDI_RESULT mdi_video_ply_non_block_seek_and_get_frame(U64			time,
																		gdi_handle	player_layer_handle,
																		void 			(*seek_result_callback)(MDI_RESULT))
																		
{
#ifdef MMI_ON_HARDWARE_P		
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 	layer_width = 0;
	S32 	layer_height = 0;
	U8	 	*buf_ptr=NULL;
	S32	ret = 0;
	S32 offset_x = 0, offset_y = 0;
	vid_seek_struct seek_data;
	module_type module_id = stack_int_get_active_module_id();

	TRACE_EF(g_sw_MP4," mdi_video_ply_non_block_seek_and_get_frame");
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	SetProtocolEventHandler(mdi_video_ply_non_block_seek_result_hdlr, MSG_ID_MEDIA_VID_SEEK_DONE_IND);
	
	mdi_video_play_seek_result_callback = seek_result_callback;
	#if 0
	gdi_layer_push_and_set_active(player_layer_handle);

	gdi_layer_get_buffer_ptr(&buf_ptr);
	gdi_layer_get_dimension(&layer_width, &layer_height);
	gdi_layer_get_position(&offset_x, &offset_y);

	gdi_layer_pop_and_restore_active();
#endif
		if(!((gZoomHeight == LCD_WIDTH) &&  (gZoomWidth == LCD_HEIGHT)) )//full screen
			{
				offset_x = g_vdoply_cntx.video_info.startx;
				offset_y = g_vdoply_cntx.video_info.starty ;
			}


	seek_data.time						= time;
	seek_data.get_frame				= TRUE;
	seek_data.display_width			= layer_width;
	seek_data.display_height		= layer_height;
	seek_data.display_offset_x		= offset_x;
	seek_data.display_offset_y		= offset_y;
	seek_data.image_buffer_p		= (void*)buf_ptr;
	seek_data.image_buffer_size	= (layer_width*layer_height*GDI_LAYER.act_bit_per_pixel)>>3; 
	seek_data.blocking				= FALSE;
	seek_data.seq_num					= g_mdi_video_cntx.seek_seq_num;
	
	#if (defined(MT6228)||defined(MT6229))
      seek_data.tv_output = FALSE;
   #endif 

	/* TV out */
	#ifdef __MMI_TVOUT__
		if(mdi_tvout_is_enable())
		{
			if(mdi_tvout_get_owner() == MDI_TV_OWNER_VIDEO)
	 		{
				ret = mdi_tvout_get_para(	MDI_TV_OWNER_VIDEO, 
													NULL,														/* mode_ptr */
													NULL,														/* buf_width_ptr */
													NULL,														/* buf_height_ptr */
													(PU32)&seek_data.tv_output_buffer_size,		/* buf size */
													(PU8*)&seek_data.tv_output_buffer1_address, 	/* buf1 pp*/
													(PU8*)&seek_data.tv_output_buffer2_address); /* buf2 pp*/

				if(ret == MDI_RES_TV_ENABLE)
				{
					seek_data.tv_output = TRUE;		
				}
				else
				{
					seek_data.tv_output = FALSE;
				}
			}
		}
  	#endif /* __MMI_TVOUT__ */

	module_id = MOD_MMI;
	ret = media_vid_seek(module_id, &seek_data); 
	if(ret != MED_RES_OK)
	   return MDI_RES_VDOPLY_ERR_SEEK_FAILED;

	g_mdi_video_cntx.is_seeking = TRUE;

   #endif /* MMI_ON_HARDWARE_P */
   


	return MDI_RES_VDOREC_SUCCEED;
}




/*****************************************************************************
* FUNCTION
*	mdi_video_ply_stop_non_block_seek
* DESCRIPTION
*	stop non block seeking
* PARAMETERS
*  void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
extern MDI_RESULT	mdi_video_ply_stop_non_block_seek(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	module_type module_id;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	TBM_ENTRY(0x2A73);
	TRACE_EF(g_sw_MP4," mdi_video_ply_stop_non_block_seek");
#ifdef MMI_ON_HARDWARE_P		
	g_mdi_video_cntx.seek_seq_num++;
//	if(g_mdi_video_cntx.is_seeking == TRUE)
	{
		module_id = MOD_MMI;
		media_vid_stop(module_id);
		g_mdi_video_cntx.is_seeking = FALSE;	
		b_video_playing = 0;
	}
#endif /* MMI_ON_HARDWARE_P */

	TBM_EXIT(0x2A73);
	return MDI_RES_VDOPLY_SUCCEED;
}


MDI_RESULT mdi_video_ply_fullscreen_display_set(U16 ZoomWidth, U16 ZoomHeight, S16 startX, S16 startY, U16 rotate)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	module_type module_id;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	TBM_ENTRY(0x2A73);
	TRACE_EF(g_sw_MP4,"mdi_video_ply_fullscreen_display_set ZoomWidth:%d, ZoomHeight:%d", ZoomWidth, ZoomHeight);
	gZoomWidth = ZoomWidth;
	gZoomHeight = ZoomHeight;
#ifdef MMI_ON_HARDWARE_P		

		module_id = MOD_MMI;
		media_vid_display_set(module_id, ZoomWidth, ZoomHeight, startX, startY, 0, 0, ZoomWidth, ZoomHeight, rotate);

#endif /* MMI_ON_HARDWARE_P */

	TBM_EXIT(0x2A73);
	return MDI_RES_VDOPLY_SUCCEED;
}


/*****************************************************************************
* FUNCTION
*	mdi_video_ply_non_block_seek_result_hdlr
* DESCRIPTION
*	seek result message hdlr
* PARAMETERS
*	msg_ptr	IN		message pointer
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
static void mdi_video_ply_non_block_seek_result_hdlr(void *msgPtr)
{
#ifdef MMI_ON_HARDWARE_P		
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	media_vid_seek_done_ind_struct	*ind_p;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	ind_p = (media_vid_seek_done_ind_struct*)msgPtr;

#if 0	
	if(ind_p->seq_num != g_mdi_video_cntx.seek_seq_num)
	{
		/* 
			this case will happened when meida already send out file_ready_ind_msg, 
		   but before MDI receive this msg, MDI already ask media to stop seeking. 
		   So the seq_num cant match.  
		*/
		return;
	}
#endif
	g_mdi_video_cntx.is_seeking = FALSE;

	if(mdi_video_play_seek_result_callback != NULL)
	{
		if(ind_p->result == MED_RES_OK)
			mdi_video_play_seek_result_callback(MDI_RES_VDOPLY_SUCCEED);
		else if(ind_p->result == MED_RES_VIDEO_SEEK_PARTIAL_DONE)
			mdi_video_play_seek_result_callback(MDI_RES_VDOPLY_SEEK_PARTIAL_DONE);
		else
			mdi_video_play_seek_result_callback(MDI_RES_VDOPLY_ERR_SEEK_FAILED);
	}
#endif /* MMI_ON_HARDWARE_P */
}



/*****************************************************************************
* FUNCTION
*	mdi_video_ply_get_cur_play_time
* DESCRIPTION
*	get current play time
* PARAMETERS
*	cur_play_time		OUT	current play time
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_video_ply_get_cur_play_time(U32 *cur_play_time)
{
	module_type module_id;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	TBM_ENTRY(0x2A66);

	module_id = stack_int_get_active_module_id();
#ifdef  MMI_ON_HARDWARE_P	  //guojian delete for jasper 2007.12.11
	module_id = MOD_MMI;

	media_vid_get_current_play_time(module_id, (U64 *)cur_play_time);
#else
	*cur_play_time += 400;
#endif /* MMI_ON_HARDWARE_P */
	TBM_EXIT(0x2A66);

}



/*****************************************************************************
* FUNCTION
*	mdi_video_ply_open_file_result_hdlr
* DESCRIPTION
*	record result message hdlr
* PARAMETERS
*	msg_ptr	IN		message pointer
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
static void mdi_video_get_play_time_result_hdlr(void *msgPtr)
{
#ifdef MMI_ON_HARDWARE_P		
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	media_vid_play_time_ind_struct	*ind_p;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	ind_p = (media_vid_play_time_ind_struct*)msgPtr;
	

	mdi_video_get_play_time_result_callback(ind_p->playtime);
#endif /* MMI_ON_HARDWARE_P */	
}



/*****************************************************************************
* FUNCTION
*	mdi_video_get_cur_play_time_req
* DESCRIPTION
*	get current play time
* PARAMETERS
*	cur_play_time		OUT	current play time
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_video_get_cur_play_time_req(void (*playtime_result_callback)(UINT32))
{
	module_type module_id;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	module_id = stack_int_get_active_module_id();
#ifdef  MMI_ON_HARDWARE_P	  
	module_id = MOD_MMI;
	mdi_video_get_play_time_result_callback = playtime_result_callback;
	SetProtocolEventHandler(mdi_video_get_play_time_result_hdlr, MSG_ID_MEDIA_VID_PLAY_TIME_IND);
	media_vid_get_current_play_time_req(module_id);
#else
#endif /* MMI_ON_HARDWARE_P */

}

/*****************************************************************************
* FUNCTION
*	mdi_video_get_cur_play_time_block_req
* DESCRIPTION
*	get current play time block
* PARAMETERS
*	cur_play_time		OUT	current play time
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_video_get_cur_play_time_block_req(void (*playtime_result_callback)(UINT32))
{
	module_type module_id;
	U64 time;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	module_id = stack_int_get_active_module_id();
#ifdef  MMI_ON_HARDWARE_P	  
	module_id = MOD_MMI;
	//mdi_video_get_play_time_result_callback = playtime_result_callback;
	//SetProtocolEventHandler(mdi_video_get_play_time_result_hdlr, MSG_ID_MEDIA_VID_PLAY_TIME_IND);
	media_vid_get_current_play_time_block_req(module_id, &time);
	playtime_result_callback(time);
#else
#endif /* MMI_ON_HARDWARE_P */

}


/*****************************************************************************
* FUNCTION
*	mdi_video_ply_play_finish_result_hdlr
* DESCRIPTION
*	play finish result message handler
* PARAMETERS
*	msgPtr	IN		message pointer
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_video_ply_play_finish_result_hdlr(void *msgPtr)
{
#ifdef MMI_ON_HARDWARE_P	
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	media_vid_play_finish_ind_struct	*ind_p;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	/* seq number not match, measn it is already stopped, ignore this message */
	ind_p = (media_vid_play_finish_ind_struct*)msgPtr;
#if 0	
	if(g_mdi_video_cntx.play_seq_num != ind_p->seq_num)
		return;
#endif

  	/* enable key pad tone */
  	//mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);//close for aud_StreamStart bug.sheen
	/* un-freeze sublcd */
	#ifdef __MMI_SUBLCD__	
		gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
		gdi_lcd_freeze(FALSE);
		gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
	#endif /* __MMI_SUBLCD__ */

	#ifdef __MMI_TVOUT__
		/* restore to previous owner */
		if(g_mdi_video_cntx.tvout_prev_owner != MDI_TV_OWNER_VIDEO)
		{
			mdi_tvout_set_owner(g_mdi_video_cntx.tvout_prev_owner);
		}
	#endif /* __MMI_TVOUT__ */

	TRACE_EF(g_sw_MP4," mdi_video_ply_play_finish_result_hdlr ind_p->result =%d",ind_p->result );

	if(mdi_video_ply_play_finish_callback != NULL)
	{
		b_video_playing = 0;
		if(ind_p->result == MED_RES_OK || ind_p->result ==MED_RES_END_OF_FILE)
			mdi_video_ply_play_finish_callback(MDI_RES_VDOPLY_SUCCEED);
		else
			mdi_video_ply_play_finish_callback(MDI_RES_VDOPLY_ERR_PLAY_FAILED);
	}
#endif /* MMI_ON_HARDWARE_P */	
}


/*****************************************************************************
* FUNCTION
*	mdi_video_ply_open_file_result_hdlr
* DESCRIPTION
*	record result message hdlr
* PARAMETERS
*	msg_ptr	IN		message pointer
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
static void mdi_video_ply_open_file_result_hdlr(void *msgPtr)
{
#ifdef MMI_ON_HARDWARE_P		
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	mdi_video_info_struct				vdo_clip;
	media_vid_file_ready_ind_struct	*ind_p;

//	TRACE_EF(g_sw_MP4,"guoj++ mdi_video_ply_open_file_result_hdlr,ind_p->total_time is %d ",ind_p->total_time);
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	ind_p = (media_vid_file_ready_ind_struct*)msgPtr;
#if 0
//chenhe test-

	if(ind_p->seq_num != g_mdi_video_cntx.open_seq_num)
	{
		/* 
			this case will happened when meida already send out file_ready_ind_msg, 
		   but before MDI receive this msg, MDI already ask media to close file. 
		   So the seq_num cant match.  
		*/
		return;
	}
#endif	
//chenhe test-
	if(ind_p->result == MED_RES_OK)
	{
		vdo_clip.width						= ind_p->image_width;
		vdo_clip.height					= ind_p->image_height;
		vdo_clip.total_frame_count		= ind_p->total_frame_num;
		vdo_clip.total_time_duration	= ind_p->total_time;
		vdo_clip.media_type	= ind_p->media_type;
              vdo_clip.aud_channel =  ind_p->aud_channel;
              vdo_clip.aud_sample_rate=  ind_p->aud_sample_rate;
              vdo_clip.track=  ind_p->track;
   
		mdi_video_ply_open_file_result_callback(MDI_RES_VDOPLY_SUCCEED, &vdo_clip);
	}
	else
	{
		vdo_clip.width						= 0;
		vdo_clip.height					= 0;
		vdo_clip.total_frame_count		= 0;
		vdo_clip.total_time_duration	= 0;
		vdo_clip.media_type	= MED_NO_OF_TYPE;

              vdo_clip.aud_channel =  0;
              vdo_clip.aud_sample_rate= 0;
              vdo_clip.track=  0;

		mdi_video_ply_open_file_result_callback(MDI_RES_VDOPLY_ERR_FAILED, &vdo_clip);
	}
#endif /* MMI_ON_HARDWARE_P */	
}



/*****************************************************************************
* FUNCTION
*	mdi_video_ply_set_lcm_update
* DESCRIPTION
*	set video update to lcm or not
* PARAMETERS
*	is_update	IN		update flag
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_ply_set_lcm_update(BOOL is_update)
{
#ifdef MMI_ON_HARDWARE_P		
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/	
	S32 ret;
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	/* Freeze sublcd */
	#ifdef __MMI_SUBLCD__
		if(is_update)
		{
			/* mainlcd will start to update, freeze sublcd */
			gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
			gdi_lcd_freeze(TRUE);
			gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
		}
		else /* __MMI_SUBLCD__ */
		{
			/* mainlcd do not update, unfreeze sublcd */
			gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
			gdi_lcd_freeze(FALSE);
			gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
		}
	#endif /* __MMI_SUBLCD__ */

	
	if(is_update)
	{
		ret = media_vid_set_display(MOD_MMI, MED_DISPLAY_TO_MAIN_LCD);
	}
	else
	{
		ret = media_vid_set_display(MOD_MMI, MED_DISPLAY_NONE);
	}
	
	if(ret != MED_RES_OK)
		return MDI_RES_VDOPLY_ERR_OPEN_FILE_FAILED;
	else
		return MDI_RES_VDOPLY_SUCCEED;
#else
	return MDI_RES_VDOPLY_SUCCEED;	
	
#endif /* MMI_ON_HARDWARE_P */	

}
	


/****************************************************************************
*
* VIDEO CLIP                                                                 
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	mdi_video_ply_open_clip_file
* DESCRIPTION
*	open video clip, this is a blocking function, will return untill file is opened
* PARAMETERS
*	filename					IN 	video clip's full file name
*	open_done_callback	IN		open file result  callback function
*	is_aud_on				IN 	play video or not
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_ply_open_clip_file(const S8 *filename, mdi_video_info_struct *info, BOOL is_aud_on)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
#ifdef MMI_ON_HARDWARE_P		
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 					ret;
	vid_open_struct 	open_data;
	module_type module_id=stack_int_get_active_module_id();
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	

	module_id = MOD_MMI;
	
	/* if open clip with audio on. play will also turn audio on. */
	g_mdi_video_cntx.is_play_audio = is_aud_on;

	/* un-hook open file done hdlr */
	if (module_id==MOD_MMI)
		SetProtocolEventHandler(NULL, MSG_ID_MEDIA_VID_FILE_READY_IND);

   //open_data.media_mode 	= MED_MODE_CLIP_FILE;
   open_data.media_mode 	= MED_MODE_FILE;//MED_MODE_FILE use for file mode, MED_MODE_CLIP_ARRAY for whole file in buffer mode, MED_MODE_BUFFER for ring buffer.sheen
   open_data.data				= (void*)filename;
   
   #if defined(MT6228) || defined(MT6229)
		open_data.max_width		= 352;		 
		open_data.max_height		= 288;		 
	#else
		 
		open_data.max_width		= 176;		 
		open_data.max_height		= 144;		 
	#endif /* chip */
	
   open_data.play_audio		= (U8)is_aud_on;
   open_data.seq_num			= g_mdi_video_cntx.open_seq_num;
   open_data.blocking		= TRUE;
	open_data.image_width 	= &info->width;
	open_data.image_height 	= &info->height;
	open_data.total_time		= &info->total_time_duration;
	
	ret = media_vid_open(module_id, &open_data);
//	ret = media_vid_open_file(module_id, filename,0);

	if(ret != MED_RES_OK)
		return MDI_RES_VDOPLY_ERR_OPEN_FILE_FAILED;
	else
		return MDI_RES_VDOPLY_SUCCEED;
#else
	{
		/* for WIN32 simulation */
		mdi_video_info_struct vdo_clip;
		
		vdo_clip.width						= 176;
		vdo_clip.height					= 144;
		vdo_clip.total_frame_count		= 1000;
		vdo_clip.total_time_duration	= 30;
	
		return MDI_RES_VDOPLY_SUCCEED;		
	}
#endif /* MMI_ON_HARDWARE_P */

}



/*****************************************************************************
* FUNCTION
*	mdi_video_ply_close_clip_file
* DESCRIPTION
*	close video clip
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_ply_close_clip_file(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	

#ifdef MMI_ON_HARDWARE_P		

	g_mdi_video_cntx.open_seq_num++;
	media_vid_close(MOD_MMI);
	
#endif /* MMI_ON_HARDWARE_P */

   /* close file shall always succeed */
	return MDI_RES_VDOPLY_SUCCEED;
}


/*****************************************************************************
* FUNCTION
*	mdi_video_clip_open_id
* DESCRIPTION
*	open video clip
* PARAMETERS
*	video_id					IN 	video clip resource id
*	open_done_callback	IN		open file result callback function
*	is_aud_on				IN 	play video or not
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
BOOL get_vdo_audioPlay(void)
{
	return g_mdi_video_cntx.is_play_audio;
}
MDI_RESULT mdi_video_ply_open_clip_id(U16 video_id, mdi_video_info_struct *info, BOOL is_aud_on)
{
#ifdef MMI_ON_HARDWARE_P	
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 					ret;
	vid_open_struct 	open_data;
	PS8					vdo_ptr;
	S32					vdo_len;

	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	

	/* if open clip with audio on. play will also turn audio on. */
	g_mdi_video_cntx.is_play_audio = is_aud_on;

	/* un-hook open file done hdlr */
	SetProtocolEventHandler(NULL, MSG_ID_MEDIA_VID_FILE_READY_IND);

	/* get raw data by ID */
	vdo_ptr = GetMedia(video_id);

	if(vdo_ptr == NULL)
	{
		return MDI_RES_VDOPLY_ERR_FAILED;
	}


	switch(*vdo_ptr)
	{
		case IMAGE_TYPE_AVI:
			 open_data.media_type = MED_TYPE_MJPG;
			break;

		case IMAGE_TYPE_3GP:
			 open_data.media_type = MED_TYPE_3GP;
			break;

		case IMAGE_TYPE_MP4:
			 open_data.media_type = MED_TYPE_MP4;
			break;

		default:
			return MDI_RES_VDOPLY_ERR_FAILED;
	}

	/* get video data length size */
	vdo_len = (S32)(vdo_ptr[2]);
	vdo_len |= (S32)((S32)vdo_ptr[3]<<8);
	vdo_len |= (S32)((S32)vdo_ptr[4]<<16);
	vdo_len |= (S32)((S32)vdo_ptr[5]<<24);
	
   //open_data.media_mode 	= MED_MODE_CLIP_ARRAY;
   open_data.media_mode 	= MED_MODE_FILE;//MED_MODE_FILE use for file mode, MED_MODE_CLIP_ARRAY for whole file in buffer mode, MED_MODE_BUFFER for ring buffer.sheen

	open_data.data				= (void*)(vdo_ptr+8);
	open_data.data_size		= vdo_len;
	
   #if defined(MT6228) || defined(MT6229)
		open_data.max_width		= 352;		 
		open_data.max_height		= 288;		 
	#else
		 
		open_data.max_width		= 176;		 
		open_data.max_height		= 144;		 
	#endif /* chip */
	
   open_data.play_audio		= (U8)is_aud_on;
   open_data.seq_num			= g_mdi_video_cntx.open_seq_num;
   open_data.blocking		= TRUE;
	open_data.image_width 	= &info->width;
	open_data.image_height 	= &info->height;
	open_data.total_time		= &info->total_time_duration;
	
  ret = media_vid_open(MOD_MMI, &open_data);

	if(ret != MED_RES_OK)
		return MDI_RES_VDOPLY_ERR_OPEN_FILE_FAILED;
	else
		return MDI_RES_VDOPLY_SUCCEED;

#endif /* MMI_ON_HARDWARE_P */

	return MDI_RES_VDOPLY_SUCCEED;
}



/*****************************************************************************
* FUNCTION
*	mdi_video_ply_close_clip_id
* DESCRIPTION
*	close video clip from id
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_video_ply_close_clip_id(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	
#ifdef MMI_ON_HARDWARE_P		

	g_mdi_video_cntx.open_seq_num++;
	media_vid_close(MOD_MMI);

#endif /* MMI_ON_HARDWARE_P */

   /* close file shall always succeed */
	return MDI_RES_VDOPLY_SUCCEED;
}


#endif /* __MMI_VIDEO__ */

