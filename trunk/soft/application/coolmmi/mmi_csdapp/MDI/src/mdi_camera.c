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
 *	 mdi_camera.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	 Interface to access camera related functions.
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
/***************************************************************************** 
* Include
*****************************************************************************/
#include "mmi_features.h"
#ifdef __MMI_CAMERA__

#define __NEWSIMULATOR 
#include "stdc.h"
#include "l4dr1.h"

#include "mmi_data_types.h"
#include "globalconstants.h"
#include "queuegprot.h"			/* message handling */
#include "fat_fs.h"
#include "gdi_include.h"		/* include data type */	
#include "gui.h"
#include "med_api.h"
#include "mdi_datatype.h"
#include "med_api.h"				/* media task */
#include "mdi_camera.h"			/* include camera related define */	
#include "eventsgprot.h"		/* event hadnling - get message return from camera driver */
#include "gpioinc.h"				/* avoid sleep when using camera */
#include "fat_fs.h"

#include "lcd_if.h"
#include "lcd_sw_rnd.h"

#include "med_api.h"
#include "med_main.h"
#include "med_struct.h"	
#include "lcd_sw_inc.h"


#if defined(ISP_SUPPORT)
	/* for visual effect */
	#ifdef MMI_ON_HARDWARE_P
		#include "intrctrl.h"
		#include "drv_comm.h"
//		#include "drvpdn.h"
		#include "jpeg.h"
		#include "visualhisr.h"
		#include "visual_comm.h"
		#include "fat_fs.h"
		#include "resizer.h"
		#include "isp_if.h"
		#include "imgproc.h"
		#include "img_comm.h"
		#include "imgdma.h"
		#include "image_effect.h"	
	#endif /* ISP_SUPPORT */
#endif

#ifdef __MMI_TVOUT__
	#include "mdi_tv.h"
#endif 

#include "mmi_trace.h"
/*gaosh Add Start For 6116 Ver: TBM780_T_20070813_1919  on 2007-8-14 14:11 */
#include "mmi_features_camera.h"
/*gaosh Add End  For 6116 Ver: TBM780_T_20070813_1919  on 2007-8-14 14:11 */
#include "cameraapp.h"
 
/***************************************************************************** 
* Typedef
*****************************************************************************/
/* camera state machine */
typedef enum {
	CAMERA_STATE_POWER_OFF = 0,
	CAMERA_STATE_IDLE,
	CAMERA_STATE_PREVIEWING
} mdi_camera_state_enum;


/***************************************************************************** 
* Local Variable
*****************************************************************************/
mdi_camera_context_struct		g_mdi_camera_cntx;
mdi_camera_context_struct		*mdi_camera_p				= &g_mdi_camera_cntx;
mdi_camera_setting_struct		*camera_setting_p			= &g_mdi_camera_cntx.setting;
mdi_camera_preview_struct		*camera_preview_p			= &g_mdi_camera_cntx.preview;

/* callback function */
void (*mdi_camera_non_block_burst_shot_result_callback)(MDI_RESULT, U16) = NULL;

#ifndef APPMEM_SUPPORT  
	U32 mdi_camera_post_effect_buf_1[((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3)/4];
	U32 mdi_camera_post_effect_buf_2[((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3)/4];
#endif

//API for cool profile
#define u16 UINT16
#define u8   UINT8
#if 1
#define CPMASK 0x3fff
#define CPEXITFLAG 0x8000
#define CP_ENTRY(id) 
#define CP_EXIT(id) 
#else
extern void hal_PXTS_SendProfilingCode(u16 code);
#define CPMASK 0x3fff
#define CPEXITFLAG 0x8000
#define CP_ENTRY(id) hal_PXTS_SendProfilingCode((id) & CPMASK)
#define CP_EXIT(id) hal_PXTS_SendProfilingCode(((id) & CPMASK) | CPEXITFLAG)
#endif


/***************************************************************************** 
* Local Function
*****************************************************************************/
static MDI_RESULT	mdi_camera_preview_start_internal(void);

#if (defined(MT6228)||defined(MT6229))
	static void mdi_camera_non_block_burst_capture_result_hdlr(void *msg_ptr);
#endif  

#ifdef MMI_ON_WIN32
 void media_cam_set_param(module_type src_mod_id, kal_uint16 param_id, kal_int16 value)
{}
#endif


/*****************************************************************************
* FUNCTION
*	mdi_camera_init
* DESCRIPTION
*	init MDI Camera
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_camera_init(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mdi_camera_p->state 				= CAMERA_STATE_POWER_OFF;
	mdi_camera_p->seq_num			= 0;
	mdi_camera_p->burst_cap_count = 0;
	
	return MDI_RES_CAMERA_SUCCEED;
}



/*****************************************************************************
* FUNCTION
*	mdi_camera_power_on
* DESCRIPTION
*	power on the camera module
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_camera_power_on()
{
#ifdef MMI_ON_HARDWARE_P
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 ret;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* check if state is right */
	ASSERT(mdi_camera_p->state == CAMERA_STATE_POWER_OFF);
	mmi_trace(g_sw_CAMERA,"Excute funs: mdi_camera_power_on() is TRUE");
	ret = media_cam_power_up(MOD_MMI, mdi_camera_p->seq_num);

	if(ret == MED_RES_OK)
	{
		/* Power On succeed */
		mdi_camera_p->state = CAMERA_STATE_IDLE;
		return MDI_RES_CAMERA_SUCCEED;
	}
	else
	{
		return ret;
	}
		

#else 
	/* avoid compile warning in MNT */
	return MDI_RES_CAMERA_SUCCEED;
#endif /* MMI_ON_HARDWARE_P */
}



/*****************************************************************************
* FUNCTION
*	mdi_camera_power_off
* DESCRIPTION
*	power off the camera module
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_camera_power_off(void)
{
#ifdef MMI_ON_HARDWARE_P
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
//	media_cam_power_down(MOD_MMI, 100);
	media_cam_power_down(MOD_MMI, 0);//fengzb
	mdi_camera_p->state = CAMERA_STATE_POWER_OFF;
	mdi_camera_p->seq_num++;
	
	return MDI_RES_CAMERA_SUCCEED;

#else 
	/* avoid compile warning in MNT */
	return MDI_RES_CAMERA_SUCCEED;
#endif /* MMI_ON_HARDWARE_P */

}



/*****************************************************************************
* FUNCTION
*	mdi_camera_preview_start
* DESCRIPTION
*	start preview
* PARAMETERS
*	preview_layer_handle			IN		layer for preview
*	preview_wnd_offset_x			IN		window offset x
*	preview_wnd_offset_y			IN		window offset y
*	preview_wnd_width				IN		window width
*	preview_wnd_height			IN		window height
*	blt_layer_flag					IN		blt out layer flag
*	preview_layer_flag			IN		preview layer flag
*  is_tvout							IN		is display preview on tvout
*	setting_p						IN		setting data
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_camera_preview_start(	gdi_handle						preview_layer_handle,
													S32								preview_wnd_offset_x,	
													S32								preview_wnd_offset_y,	
													U16								preview_wnd_width,		/* preview window width */
													U16								preview_wnd_height,		/* preview window height */
													U32								blt_layer_flag,
													U32								preview_layer_flag,
													gdi_color						src_key_color,
													BOOL								is_tvout,
													mdi_camera_setting_struct	*setting_p)
{	
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 ret;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* arg check */
	ASSERT(blt_layer_flag > 0);
	ASSERT(preview_layer_flag > 0);

	g_mdi_camera_cntx.is_tvout = is_tvout;

	/* set preview wnd parameters */				
	camera_preview_p->preview_layer_handle	= preview_layer_handle;
	camera_preview_p->preview_wnd_offset_x	= preview_wnd_offset_x;
	camera_preview_p->preview_wnd_offset_y	= preview_wnd_offset_y;
	camera_preview_p->preview_wnd_width		= preview_wnd_width;
	camera_preview_p->preview_wnd_height	= preview_wnd_height;

	camera_preview_p->blt_layer_flag			= blt_layer_flag;
	camera_preview_p->preview_layer_flag	= preview_layer_flag;

	/* 
 
		we only support 16 bit color depth, so, U16 is enoguth 
	*/
	camera_preview_p->src_key_color			= (U16)src_key_color;

	/* init camera parameters */
	memcpy(camera_setting_p, setting_p, sizeof(mdi_camera_setting_struct));
	mmi_trace(g_sw_CAMERA,"before start_internal mdi_camera_p->state = %d",mdi_camera_p->state);
	//if(mdi_camera_p->state == CAMERA_STATE_IDLE)
	if(1)
	{
		ret = mdi_camera_preview_start_internal();
		mmi_trace(g_sw_CAMERA, "Excute funs: mdi_camera_preview_start() ret = %d",ret);
		if(ret >= 0)
		{
			/* preview successfully */
			mdi_camera_p->state = CAMERA_STATE_PREVIEWING;
		}
		
		return  ret;
	}
	else
	{
		/* start preview at wrong state */
#ifdef WIN32 /* MNT Simulator */

		/* on MNT, direct call internal function for debugging */
		mdi_camera_preview_start_internal();
		return MDI_RES_CAMERA_SUCCEED;

#else 	
		/* Target */
		return MDI_RES_CAMERA_ERR_FAILED;
#endif 
	}
}




/*****************************************************************************
* FUNCTION
*	mdi_camera_preview_start_internal
* DESCRIPTION
*	init MDI Camera
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/

extern camera_context_struct g_camera_cntx;
#ifdef __MMI_ANALOGTV__ 
extern uint8  atvif_GetCurModule();
extern BOOL atvif_IsFullScreen(void);
#endif

static MDI_RESULT mdi_camera_preview_start_internal(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    cam_preview_struct	cam_preview_data;
    U8				*buf_ptr;	
    S32				layer_width;
    S32				layer_height;
    S32				layer_offset_x;
    S32				layer_offset_y;
    MDI_RESULT		ret;
    S32				lcd_width;
    S32				lcd_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    /* if preview on sublcd, set sublcd active, so we may get correct paramater from following functions */
    if(camera_setting_p->lcm == MDI_CAMERA_PREVIEW_LCM_SUBLCD)
        gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);

    /* get layer info */
    gdi_layer_push_and_set_active(camera_preview_p->preview_layer_handle);

    gdi_layer_get_buffer_ptr(&buf_ptr);
    gdi_layer_get_dimension(&layer_width, &layer_height);
    gdi_layer_get_position(&layer_offset_x, &layer_offset_y);

    gdi_layer_pop_and_restore_active();
		
    if(camera_setting_p->lcm == MDI_CAMERA_PREVIEW_LCM_SUBLCD)
        gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);

    cam_preview_data.preview_width      = camera_setting_p->preview_width;
    cam_preview_data.preview_height     = camera_setting_p->preview_height;


    if(camera_setting_p->lcm_rotate==MDI_CAMERA_LCM_ROTATE_270) 
    {
        cam_preview_data.lcd_start_x       = LCD_WIDTH - layer_offset_y - camera_setting_p->preview_height;	
        cam_preview_data.lcd_start_y       = layer_offset_x;

        cam_preview_data.lcd_end_x        = cam_preview_data.lcd_start_x + cam_preview_data.preview_height -1;
        cam_preview_data.lcd_end_y        = cam_preview_data.lcd_start_y + cam_preview_data.preview_width -1;

        cam_preview_data.roi_offset_x      = GDI_LAYER_OFFSET+cam_preview_data.lcd_start_x;
        cam_preview_data.roi_offset_y      = GDI_LAYER_OFFSET+cam_preview_data.lcd_start_y;
    }
    else
    {
        cam_preview_data.lcd_start_x	= layer_offset_x;	
        cam_preview_data.lcd_start_y	= layer_offset_y;

        cam_preview_data.lcd_end_x	= cam_preview_data.lcd_start_x + cam_preview_data.preview_width -1;
        cam_preview_data.lcd_end_y	= cam_preview_data.lcd_start_y + cam_preview_data.preview_height -1;

        cam_preview_data.roi_offset_x	= GDI_LAYER_OFFSET+cam_preview_data.lcd_start_x;
        cam_preview_data.roi_offset_y	= GDI_LAYER_OFFSET+cam_preview_data.lcd_start_y;
    }

    /* force update full screen */
    cam_preview_data.preview_offset_x	= camera_preview_p->preview_wnd_offset_x;
    cam_preview_data.preview_offset_y	= camera_preview_p->preview_wnd_offset_y;

    cam_preview_data.image_buffer_p	= (void*)buf_ptr;
    cam_preview_data.image_width		= (kal_uint16)camera_setting_p->image_width;
    cam_preview_data.image_height		= (kal_uint16)camera_setting_p->image_height;

    cam_preview_data.buffer_width		= (kal_uint16)layer_width;
    cam_preview_data.buffer_height		= (kal_uint16)layer_height;
    cam_preview_data.image_buffer_size = (kal_uint32)((camera_setting_p->preview_width*camera_setting_p->preview_height*GDI_LAYER.act_bit_per_pixel)>>3);

    /* set settings parameters */
    cam_preview_data.effect			= (kal_uint16)camera_setting_p->effect;
    cam_preview_data.WB			= (kal_uint16)camera_setting_p->wb;
    cam_preview_data.exposure		= (kal_uint16)camera_setting_p->ev;
    cam_preview_data.zoom_factor	= (kal_uint16)camera_setting_p->zoom;
    cam_preview_data.banding_freq	= (kal_uint8)camera_setting_p->banding;

    cam_preview_data.brightness		= (kal_uint16)camera_setting_p->brightness;
    cam_preview_data.contrast		= (kal_uint16)camera_setting_p->contrast;
    cam_preview_data.saturation		= (kal_uint16)camera_setting_p->saturation;
    cam_preview_data.hue			= (kal_uint16)camera_setting_p->hue;
    cam_preview_data.flash_mode		= (kal_uint16)camera_setting_p->flash;
    //cam_preview_data.night_mode	= camera_setting_p->shot_mode;/*(kal_bool)camera_setting_p->night;*/
    cam_preview_data.night_mode		= (kal_bool)camera_setting_p->night;
    cam_preview_data.snapshot_number	= 1;

#if (defined(MT6228)||defined(MT6229))
    cam_preview_data.ae_metering_mode	= (kal_uint16)camera_setting_p->ae_meter;
    cam_preview_data.iso_setting 		= (kal_uint16)camera_setting_p->iso;
#endif
			
    /* rotate */

#ifdef __MMI_ANALOGTV__ 
    if(atvif_GetCurModule() == 1)
    {
        cam_preview_data.rotate 		= atvif_IsFullScreen();
        cam_preview_data.lcd_rotate 	= camera_setting_p->lcm_rotate;
    }
    else
    {
        cam_preview_data.rotate 		= camera_setting_p->preview_rotate;
        cam_preview_data.lcd_rotate 	= camera_setting_p->lcm_rotate;
    }
#else
    cam_preview_data.rotate 		= camera_setting_p->preview_rotate;
    cam_preview_data.lcd_rotate 	= camera_setting_p->lcm_rotate;
#endif
    /* fixed para */
    //cam_preview_data.gamma				= 0;  //deleted by WeiD
    cam_preview_data.gamma      = camera_setting_p->image_qty; //use gamma for image_qty add by WeiD

    /*gaosh Add Start For 6116 Ver: TBM780_T_20070813_1919  on 2007-8-14 14:13 */
#ifndef __PRJ_WITH_SPICAM__    
    if((cam_preview_data.image_width == CAMERA_FEATURE_IMAGE_SIZE_LL_WIDTH)
            &&(cam_preview_data.image_height ==CAMERA_FEATURE_IMAGE_SIZE_LL_HEIGHT)
            &&(cam_preview_data.gamma!=MDI_CAMERA_JPG_QTY_LOW))
    {
        cam_preview_data.gamma  = MDI_CAMERA_JPG_QTY_LOW;
    }
#endif    
    /*gaosh Add End  For 6116 Ver: TBM780_T_20070813_1919  on 2007-8-14 14:13 */

    //cam_preview_data.frame_rate		= CAMERA_PERVIEW_FRAME_RATE;
    cam_preview_data.frame_rate		= g_camera_cntx.setting.add_frame; //add by WeiD for frame

    /*WeiD Add Start For 6195 Ver: TBM780  on 2007-8-10 18:33 */
    if((cam_preview_data.image_width==LCD_WIDTH) && (cam_preview_data.image_height==LCD_HEIGHT) && cam_preview_data.frame_rate)
    {
        cam_preview_data.gamma = 1;     //for Vimicro need quality high when have frame ,so we use 3 level.(3 level is high)
    }

    /*WeiD Add End  For 6195 Ver: TBM780  on 2007-8-10 18:33 */

#if (defined(MT6228)||defined(MT6229))
    cam_preview_data.image_pitch_mode 	= FALSE;
    cam_preview_data.overlay_frame_mode = FALSE;
    cam_preview_data.manual_wb				= FALSE;
    cam_preview_data.tv_output				= FALSE;
#endif

    /* preview on whihc lcm */
    if(camera_setting_p->lcm == MDI_CAMERA_PREVIEW_LCM_MAINLCD)
    {
        cam_preview_data.lcd_id	= MAIN_LCD;
        lcd_width = LCD_WIDTH;
        lcd_height = LCD_HEIGHT;
    }
    else 
    {
        cam_preview_data.lcd_id	= SUB_LCD;
        gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
        gdi_lcd_get_dimension(&lcd_width, &lcd_height);
        gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
    }

    cam_preview_data.update_layer		= camera_preview_p->blt_layer_flag;
    cam_preview_data.hw_update_layer	= camera_preview_p->preview_layer_flag;
    //cam_preview_data.src_key_color 	= GDI_COLOR_TRANSPARENT;//camera_preview_p->src_key_color;//fengzb
    cam_preview_data.src_key_color 	= camera_preview_p->src_key_color;	//fengzb
    cam_preview_data.roi_background_color = GDI_COLOR_BLACK;

#if defined(MT6228)|| defined(MT6229)
     
    g_mdi_camera_cntx.lcd_id			= cam_preview_data.lcd_id;
    g_mdi_camera_cntx.lcm_start_x		= cam_preview_data.lcd_start_x;
    g_mdi_camera_cntx.lcm_start_y 		= cam_preview_data.lcd_start_y;				
    g_mdi_camera_cntx.lcm_end_x		= cam_preview_data.lcd_end_x;					
    g_mdi_camera_cntx.lcm_end_y		= cam_preview_data.lcd_end_y;					
    g_mdi_camera_cntx.update_layer	= cam_preview_data.update_layer;				
    g_mdi_camera_cntx.hw_update_layer	= cam_preview_data.hw_update_layer;	
    g_mdi_camera_cntx.frame_buf_size	= cam_preview_data.image_buffer_size;
    g_mdi_camera_cntx.frame_buf_ptr	= cam_preview_data.image_buffer_p;
    g_mdi_camera_cntx.roi_offset_x		= cam_preview_data.roi_offset_x;
    g_mdi_camera_cntx.roi_offset_y		= cam_preview_data.roi_offset_y;	
#endif  

    /* TV out */
#ifdef __MMI_TVOUT__	
    cam_preview_data.tv_output = FALSE;

    if(g_mdi_camera_cntx.is_tvout)
    {
        if(mdi_tvout_is_enable())
        {
            /* set tvout owner to Camera */
            g_mdi_camera_cntx.tvout_prev_owner = mdi_tvout_get_owner();	
            if(g_mdi_camera_cntx.tvout_prev_owner != MDI_TV_OWNER_CAMERA)
            {
                mdi_tvout_set_owner(MDI_TV_OWNER_CAMERA);
            }
            ret = mdi_tvout_get_para(	MDI_TV_OWNER_CAMERA, 
                                (PU16)&cam_preview_data.tv_output_mode,					/* mode_ptr */
                                (PU16)&cam_preview_data.tv_output_width,					/* buf_width_ptr */
                                (PU16)&cam_preview_data.tv_output_height,					/* buf_height_ptr */
                                (PU32)&cam_preview_data.tv_output_buffer_size,			/* buf size */
                                (PU8*)&cam_preview_data.tv_output_buffer1_address, 	/* buf1 pp*/
                                (PU8*)&cam_preview_data.tv_output_buffer2_address);  	/* buf2 pp*/

            if(ret == MDI_RES_TV_ENABLE)
            {
                cam_preview_data.tv_output = TRUE;
            }
            else
            {
                /* tvout not enable , restore owner to GDI */
                if(g_mdi_camera_cntx.tvout_prev_owner != MDI_TV_OWNER_CAMERA)
                {
                    mdi_tvout_set_owner(g_mdi_camera_cntx.tvout_prev_owner);	
                }
            }
        } /* tvout is enable or not */
    }
#endif /* __MMI_TVOUT__ */

    mdi_camera_p->is_preview_stopped = FALSE;

#ifdef MMI_ON_HARDWARE_P
    ret = media_cam_preview(MOD_MMI, &cam_preview_data);

    mmi_trace(g_sw_CAMERA, "media_cam_preview() return now. ret = %d", ret);
	
    if(ret != MED_RES_OK)
    {
        return MDI_RES_CAMERA_ERR_HW_NOT_READY;
    }
    else
    {
        mdi_camera_p->state = CAMERA_STATE_PREVIEWING;
        return MDI_RES_CAMERA_SUCCEED;
    }
#else
    ret = MED_RES_OK;
    return ret;
#endif /* MMI_ON_HARDWARE_P */

}



/*****************************************************************************
* FUNCTION
*	mdi_camera_preview_stop
* DESCRIPTION
*	shutdown the camer module
* PARAMETERS
*	void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_camera_preview_stop(void)
{
#ifdef MMI_ON_HARDWARE_P
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mmi_trace(g_sw_CAMERA, "Excute funs: mdi_camera_preview_stop() is TRUE,mdi_camera_p->is_preview_stopped = %d",mdi_camera_p->is_preview_stopped);
	if(mdi_camera_p->is_preview_stopped == FALSE)
		media_cam_stop(MOD_MMI);
 
	#ifdef __MMI_TVOUT__
		/* tvout not enable , restore owner to GDI */
		if(g_mdi_camera_cntx.tvout_prev_owner == MDI_TV_OWNER_GDI)
		{
			mdi_tvout_set_owner(MDI_TV_OWNER_GDI);	
		}
	#endif /* __MMI_TVOUT__ */

	/* already powered off */
	if(mdi_camera_p->state == CAMERA_STATE_POWER_OFF)
		return MDI_RES_CAMERA_SUCCEED;

	mdi_camera_p->state = CAMERA_STATE_IDLE;
	mdi_camera_p->is_preview_stopped = TRUE;

	return MDI_RES_CAMERA_SUCCEED;

#else 
	/* avoid compile warning in MNT */
	return MDI_RES_CAMERA_SUCCEED;
#endif /* MMI_ON_HARDWARE_P */
}


/*****************************************************************************
* FUNCTION
*	mdi_camera_capture_to_file
* DESCRIPTION
*	capture jpeg
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_camera_capture_to_file(S8 *file_name,U8 continue_capture)
{
#ifdef MMI_ON_HARDWARE_P
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    cam_capture_struct	capture_data;
    S32						result;
    TBM_ENTRY(0x2AA7);
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(mdi_camera_p->state != CAMERA_STATE_PREVIEWING)
        ASSERT(0);

    capture_data.image_width 	= camera_setting_p->image_width;
    capture_data.image_height 	= camera_setting_p->image_height;

    capture_data.media_mode	= MED_MODE_FILE;
    capture_data.image_quality	= (U8)camera_setting_p->image_qty;
    capture_data.source_device	= CAM_SRC_ISP;		/* ISP */
    capture_data.data			= (void*)file_name;
    capture_data.flash_mode		= (kal_uint16)camera_setting_p->flash;
    capture_data.snapshot_number= 1;
    capture_data.gray_mode		= 0;
    capture_data.continue_capture = continue_capture;
#if (defined(MT6228)||defined(MT6229))
    capture_data.thumbnail_mode		= FALSE;
    capture_data.overlay_frame_mode	= FALSE;
    capture_data.image_pitch_mode		= FALSE;
    capture_data.tv_output				= FALSE;		
#endif  

    /* capture jpeg */
    result = media_cam_capture(MOD_MMI, &capture_data);

    /* capture will also stop preview */
    mdi_camera_p->is_preview_stopped = TRUE;

    /* after captured. back to idle state */
    mdi_camera_p->state = CAMERA_STATE_IDLE;

    /* capture will also stop preview TV-Out */
#ifdef __MMI_TVOUT__
    /* tvout not enable , restore owner to GDI */
    if(g_mdi_camera_cntx.tvout_prev_owner == MDI_TV_OWNER_GDI)
    {
        mdi_tvout_set_owner(MDI_TV_OWNER_GDI);	
    }
#endif /* __MMI_TVOUT__ */

    if(result == MED_RES_OK)
    {
        TBM_EXIT(0x2AA7);
        return MDI_RES_CAMERA_SUCCEED;
    }
    else if(result == MED_RES_DISC_FULL||MDI_RES_CAMERA_ERR_DISK_FULL ==result )
    {
        TBM_EXIT(0x2AA7);
        return MDI_RES_CAMERA_ERR_DISK_FULL;
    }
    else if(result == MED_RES_WRITE_PROTECTION)
    {
        TBM_EXIT(0x2AA7);
        return MDI_RES_CAMERA_ERR_WRITE_PROTECTION;
    }
    else if(result == MED_RES_NO_DISC)
    {
        TBM_EXIT(0x2AA7);
        return MDI_RES_CAMERA_ERR_NO_DISK;
    }
    else
    {
        TBM_EXIT(0x2AA7);
        return MDI_RES_CAMERA_ERR_FAILED;
    }
#else 
    /* avoid compile warning in MNT */
    return MDI_RES_CAMERA_SUCCEED;
#endif /* MMI_ON_HARDWARE_P */
}



/*****************************************************************************
* FUNCTION
*	mdi_camera_capture_layer_to_file
* DESCRIPTION
*	capture from layers to a jpeg file
* PARAMETERS
*	capture_layer_flag	IN		layer configuration
*	file_name				IN		file_name
*	offset_x					IN		capture region's offset x
*	offset_y					IN		capture region's offset y
*	width						IN		capture region's width
*	height					IN		capture region's height
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_camera_capture_layer_to_file(	U32 capture_layer_flag,	
															S8  *file_name, 
															S32 offset_x, 
															S32 offset_y,
															S32 width,
															S32 height,
															U8 continue_capture)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    cam_capture_struct	capture_data;
    S32				result;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_CAMERA, "Excute funs: mdi_camera_capture_layer_to_file() is TRUE");
    if(mdi_camera_p->state != CAMERA_STATE_PREVIEWING)
        ASSERT(0);

    capture_data.image_width 		= width;
    capture_data.image_height 		= height;
	
    capture_data.media_mode		= MED_MODE_FILE;
    capture_data.image_quality		= (U8)camera_setting_p->image_qty;
    capture_data.source_device		= CAM_SRC_MEM;		/* ISP */
    capture_data.data				= (void*)file_name;
    capture_data.capture_layer		= capture_layer_flag;
    capture_data.roi_offset_x			= GDI_LAYER_OFFSET+offset_x;
    capture_data.roi_offset_y			= GDI_LAYER_OFFSET+offset_y;
    capture_data.flash_mode			= (kal_uint16)camera_setting_p->flash;	
    capture_data.snapshot_number	= 1;
    capture_data.gray_mode			= 0;
    capture_data.continue_capture 	= continue_capture;

#if (defined(MT6228)||defined(MT6229))
    capture_data.thumbnail_mode		= FALSE;
    capture_data.overlay_frame_mode	= FALSE;
    capture_data.image_pitch_mode	= FALSE;
    capture_data.tv_output			= FALSE;				
#endif  

    /* capture jpeg */
#ifdef MMI_ON_HARDWARE_P
    result = media_cam_capture(MOD_MMI, &capture_data);
#else
    /* avoid simulaotr compile warning */
    result = MED_RES_OK;
#endif /* MMI_ON_HARDWARE_P */
	
    /* capture will also stop preview */
    mdi_camera_p->is_preview_stopped = TRUE;

    /* after captured. back to idle state */
    mdi_camera_p->state = CAMERA_STATE_IDLE;

    /* capture will also stop preview TV-Out */
#ifdef __MMI_TVOUT__
    /* tvout not enable , restore owner to GDI */
    if(g_mdi_camera_cntx.tvout_prev_owner == MDI_TV_OWNER_GDI)
    {
        mdi_tvout_set_owner(MDI_TV_OWNER_GDI);	
    }
#endif /* __MMI_TVOUT__ */

    if(result == MED_RES_OK)
        return MDI_RES_CAMERA_SUCCEED;
    else if(result == MED_RES_DISC_FULL)
        return MDI_RES_CAMERA_ERR_DISK_FULL;
    else if(result == MED_RES_WRITE_PROTECTION)
        return MDI_RES_CAMERA_ERR_WRITE_PROTECTION;
    else if(result == MED_RES_NO_DISC)
        return MDI_RES_CAMERA_ERR_NO_DISK;
    else
        return MDI_RES_CAMERA_ERR_FAILED;

}



/*****************************************************************************
* FUNCTION
*	mdi_camera_capture_to_memory
* DESCRIPTION
*	capture jpeg
* PARAMETERS
*	buf_pp					IN		pointer to a buffer pointer for jpeg file
*	captured_size			OUT	captured size
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT	mdi_camera_capture_to_memory(U8 **buf_pp, U32 *captured_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    cam_capture_struct	capture_data;
    S32						result;
    TBM_ENTRY(0x2AA6);
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(mdi_camera_p->state != CAMERA_STATE_PREVIEWING)
        ASSERT(0);

    mmi_trace(g_sw_CAMERA,"Excute funs: mdi_camera_capture_to_memory() is TRUE");
    capture_data.image_width 	= camera_setting_p->image_width;
    capture_data.image_height 	= camera_setting_p->image_height;

    capture_data.media_mode			= MED_MODE_BUFFER;
    capture_data.image_quality		= (U8)camera_setting_p->image_qty;
    capture_data.source_device		= CAM_SRC_ISP;		/* ISP */
    capture_data.capture_buffer_p = (void**)buf_pp;
    capture_data.data					= NULL;				/* not used */
    capture_data.file_buffer_len	= 0;					/* not used */
    capture_data.file_size_p		= (kal_uint32*)captured_size;
    capture_data.flash_mode			= (kal_uint16)camera_setting_p->flash;	
    capture_data.snapshot_number 	= 1;
    capture_data.gray_mode			= 0;

#if (defined(MT6228)||defined(MT6229))
    capture_data.thumbnail_mode		= FALSE;
    capture_data.overlay_frame_mode	= FALSE;
    capture_data.image_pitch_mode		= FALSE;
    capture_data.tv_output				= FALSE;		
#endif  
	
    /* capture jpeg */
#ifdef MMI_ON_HARDWARE_P
    result = media_cam_capture(MOD_MMI, &capture_data);
#else
    /* avoid simulaotr compile warning */
    result = MED_RES_OK;
#endif /* MMI_ON_HARDWARE_P */
	
    /* capture will also stop preview */
    mdi_camera_p->is_preview_stopped = TRUE;

    /* after captured. back to idle state */
    mdi_camera_p->state = CAMERA_STATE_IDLE;

    /* capture will also stop preview TV-Out */
#ifdef __MMI_TVOUT__
    /* tvout not enable , restore owner to GDI */
    if(g_mdi_camera_cntx.tvout_prev_owner == MDI_TV_OWNER_GDI)
    {
        mdi_tvout_set_owner(MDI_TV_OWNER_GDI);	
    }
#endif /* __MMI_TVOUT__ */

    if(result == MED_RES_OK)
    {
        TBM_EXIT(0x2AA6);
        return MDI_RES_CAMERA_SUCCEED;
    }
    else if(result == MED_RES_DISC_FULL)
    {
        TBM_EXIT(0x2AA6);
        return MDI_RES_CAMERA_ERR_DISK_FULL;
    }
    else if(result == MED_RES_WRITE_PROTECTION)
    {
        TBM_EXIT(0x2AA6);
        return MDI_RES_CAMERA_ERR_WRITE_PROTECTION;
    }
    else if(result == MED_RES_NO_DISC)
    {
        TBM_EXIT(0x2AA6);
        return MDI_RES_CAMERA_ERR_NO_DISK;
    }
    else
    {
        TBM_EXIT(0x2AA6);
        return MDI_RES_CAMERA_ERR_FAILED;
    }

}



/*****************************************************************************
* FUNCTION
*	mdi_camera_capture_layer_to_memory
* DESCRIPTION
*	capture from layers to a memory, this is used for add frame
* PARAMETERS
*	capture_layer_flag	IN		layer configuration
*	buf_pp					IN		pointer to a buffer pointer for jpeg file
*	captured_size			OUT	captured size
*	offset_x					IN		capture region's offset x
*	offset_y					IN		capture region's offset y
*	width						IN		capture region's width
*	height					IN		capture region's height
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_camera_capture_layer_to_memory(	U32 capture_layer_flag,	
																U8  **buf_pp, 
																U32 *captured_size,
																S32 offset_x, 
																S32 offset_y,
																S32 width,
																S32 height)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	cam_capture_struct	capture_data;
	S32						result;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(mdi_camera_p->state != CAMERA_STATE_PREVIEWING)
		ASSERT(0);

	capture_data.image_width 			= width;
	capture_data.image_height 			= height;
	
	capture_data.media_mode			= MED_MODE_BUFFER;
	capture_data.image_quality		= (U8)camera_setting_p->image_qty;
	capture_data.source_device		= CAM_SRC_MEM;		/* ISP */
	capture_data.capture_buffer_p = (void**)buf_pp;
	capture_data.data					= NULL;				/* not used */
	capture_data.file_buffer_len	= 0;					/* not used */
	capture_data.file_size_p		= (kal_uint32*)captured_size;
	capture_data.capture_layer		= capture_layer_flag;
	capture_data.roi_offset_x		= GDI_LAYER_OFFSET+offset_x+camera_preview_p->preview_wnd_offset_x;
	capture_data.roi_offset_y		= GDI_LAYER_OFFSET+offset_y+camera_preview_p->preview_wnd_offset_y;

	capture_data.flash_mode			= (kal_uint16)camera_setting_p->flash;	
	capture_data.snapshot_number 	= 1;
	capture_data.gray_mode			= 0;

	#if (defined(MT6228)||defined(MT6229))
		capture_data.thumbnail_mode		= FALSE;
		capture_data.overlay_frame_mode	= FALSE;
		capture_data.image_pitch_mode		= FALSE;
		capture_data.tv_output				= FALSE;				
	#endif  

	/* capture jpeg */
	#ifdef MMI_ON_HARDWARE_P
		result = media_cam_capture(MOD_MMI, &capture_data);
	#else
		/* avoid simulaotr compile warning */
		result = MED_RES_OK;
	#endif /* MMI_ON_HARDWARE_P */

	/* capture will also stop preview */
	mdi_camera_p->is_preview_stopped = TRUE;

	/* after captured. back to idle state */
	mdi_camera_p->state = CAMERA_STATE_IDLE;

	if(result == MED_RES_OK)
		return MDI_RES_CAMERA_SUCCEED;
	else if(result == MED_RES_NOT_ENOUGH_SPACE)
		return MDI_RES_CAMERA_ERR_MEMORY_NOT_ENOUGH;
	else
		return MDI_RES_CAMERA_ERR_FAILED;

}



/*****************************************************************************
* FUNCTION
*	mdi_camera_burst_capture_to_memory
* DESCRIPTION
*	burst shot capture 
* PARAMETERS
* 	capture_count			IN		capture counter
*	jpg_ptr					OUT	captured jpeg info
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_camera_burst_capture_to_memory(	U32 								capture_count,																
																mdi_camera_jpegs_struct 	*jpgs_ptr)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	cam_capture_struct	capture_data;
	S32						result;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(mdi_camera_p->state != CAMERA_STATE_PREVIEWING)
		ASSERT(0);

	capture_data.image_width 			= camera_setting_p->image_width;
	capture_data.image_height 			= camera_setting_p->image_height;
	capture_data.media_mode				= MED_MODE_BUFFER;
	capture_data.image_quality			= (U8)camera_setting_p->image_qty;
	capture_data.source_device			= CAM_SRC_ISP;		/* ISP */
	
	capture_data.data						= NULL;				/* not used */
	capture_data.file_buffer_len		= 0;					/* not used */
	capture_data.file_size_p			= NULL;				/* not used */
	capture_data.capture_buffer_p 	= NULL;				/* not used */

	capture_data.snapshot_number		= capture_count;
	capture_data.flash_mode				= (kal_uint16)camera_setting_p->flash;	
	capture_data.gray_mode				= 0;	
	#ifdef MMI_ON_HARDWARE_P	
		capture_data.jpegs_p					= jpgs_ptr;
	#endif /* MMI_ON_HARDWARE_P */

	#if (defined(MT6228)||defined(MT6229))
		capture_data.thumbnail_mode		= FALSE;
		capture_data.overlay_frame_mode	= FALSE;
		capture_data.image_pitch_mode		= FALSE;
		capture_data.tv_output				= FALSE;				
	#endif  


	/* capture jpeg */
	#ifdef MMI_ON_HARDWARE_P
		result = media_cam_capture(MOD_MMI, &capture_data);
	#else
		/* avoid simulaotr compile warning */
		result = MED_RES_OK;
	#endif /* MMI_ON_HARDWARE_P */


	/* capture will also stop preview */
	mdi_camera_p->is_preview_stopped = TRUE;

	/* after captured. back to idle state */
	mdi_camera_p->state = CAMERA_STATE_IDLE;

	/* capture will also stop preview TV-Out */
	#ifdef __MMI_TVOUT__
		/* tvout not enable , restore owner to GDI */
		if(g_mdi_camera_cntx.tvout_prev_owner == MDI_TV_OWNER_GDI)
		{
			mdi_tvout_set_owner(MDI_TV_OWNER_GDI);	
		}
	#endif /* __MMI_TVOUT__ */

	if(result == MED_RES_OK)
		return MDI_RES_CAMERA_SUCCEED;
	else if(result == MED_RES_NOT_ENOUGH_SPACE)
		return MDI_RES_CAMERA_ERR_MEMORY_NOT_ENOUGH;
	else
		return MDI_RES_CAMERA_ERR_FAILED;

}



/*****************************************************************************
* FUNCTION
*	mdi_camera_non_block_burst_capture_to_memory
* DESCRIPTION
*	non block burst shot capture 
* PARAMETERS
* 	capture_count					IN		capture counter
*	jpg_ptr							OUT	captured jpeg info
*  capture_result_callback		IN		capture result callback
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_camera_non_block_burst_capture_to_memory(	U32 								capture_count,																
																			mdi_camera_jpegs_struct 	*jpgs_ptr,
																			void 								(*capture_result_callback)(MDI_RESULT,U16))
{
 	#if defined(MT6228)||defined(MT6229)
	   /*----------------------------------------------------------------*/
	   /* Local Variables                                                */
	   /*----------------------------------------------------------------*/
		cam_capture_struct	capture_data;
		S32						result;
			
	   /*----------------------------------------------------------------*/
	   /* Code Body                                                      */
	   /*----------------------------------------------------------------*/
	   
		if(mdi_camera_p->state != CAMERA_STATE_PREVIEWING)
			ASSERT(0);

		ASSERT(capture_result_callback != NULL);

		g_mdi_camera_cntx.burst_cap_count = capture_count;

		mdi_camera_non_block_burst_shot_result_callback = capture_result_callback;
		SetProtocolEventHandler(mdi_camera_non_block_burst_capture_result_hdlr, MSG_ID_MEDIA_CAM_CAPTURE_EVENT_IND);	

		capture_data.image_width 			= camera_setting_p->image_width;
		capture_data.image_height 			= camera_setting_p->image_height;
		capture_data.media_mode				= MED_MODE_BUFFER;
		capture_data.image_quality			= (U8)camera_setting_p->image_qty;
		capture_data.source_device			= CAM_SRC_ISP;		/* ISP */
		
		capture_data.data						= NULL;				/* not used */
		capture_data.file_buffer_len		= 0;					/* not used */
		capture_data.file_size_p			= NULL;				/* not used */
		capture_data.capture_buffer_p 	= NULL;				/* not used */

		capture_data.snapshot_number		= capture_count;
		capture_data.flash_mode				= (kal_uint16)camera_setting_p->flash;		
		capture_data.gray_mode				= 0;	
		
		#ifdef MMI_ON_HARDWARE_P
			capture_data.jpegs_p					= jpgs_ptr;
		#endif /* MMI_ON_HARDWARE_P */

		capture_data.thumbnail_mode		= FALSE;
		capture_data.overlay_frame_mode	= FALSE;
		capture_data.image_pitch_mode		= FALSE;
		capture_data.tv_output				= FALSE;		
		
		capture_data.display						= TRUE;	/* diplay to LCM */
		capture_data.lcd_id 						= g_mdi_camera_cntx.lcd_id;
		capture_data.lcm_start_x 				= g_mdi_camera_cntx.lcm_start_x;
		capture_data.lcm_start_y				= g_mdi_camera_cntx.lcm_start_y;
		capture_data.lcm_end_x					= g_mdi_camera_cntx.lcm_end_x;
		capture_data.lcm_end_y					= g_mdi_camera_cntx.lcm_end_y;
		capture_data.update_layer				= g_mdi_camera_cntx.update_layer;
		capture_data.hw_update_layer			= g_mdi_camera_cntx.hw_update_layer;
		capture_data.frame_buffer_address	= (kal_uint32)g_mdi_camera_cntx.frame_buf_ptr;
		capture_data.frame_buffer_size		= g_mdi_camera_cntx.frame_buf_size;
		capture_data.roi_offset_x				= g_mdi_camera_cntx.roi_offset_x;
		capture_data.roi_offset_y				= g_mdi_camera_cntx.roi_offset_y;

		#if (defined(MT6228)||defined(MT6229))
			capture_data.thumbnail_mode		= FALSE;
			capture_data.overlay_frame_mode	= FALSE;
			capture_data.image_pitch_mode		= FALSE;
			capture_data.tv_output				= FALSE;				
		#endif  

		/* capture jpeg */
		#ifdef MMI_ON_HARDWARE_P
			result = media_cam_capture(MOD_MMI, &capture_data);
		#else
			/* avoid simulaotr compile warning */
			result = MED_RES_OK;
		#endif /* MMI_ON_HARDWARE_P */


		/* capture will also stop preview */
		mdi_camera_p->is_preview_stopped = TRUE;

		/* after captured. back to idle state */
		mdi_camera_p->state = CAMERA_STATE_IDLE;

		if(result == MED_RES_OK)
			return MDI_RES_CAMERA_SUCCEED;
		else if(result == MED_RES_NOT_ENOUGH_SPACE)
			return MDI_RES_CAMERA_ERR_MEMORY_NOT_ENOUGH;
		else
			return MDI_RES_CAMERA_ERR_FAILED;
	#else
		 
		ASSERT(0);
		return MDI_RES_CAMERA_ERR_FAILED;
	#endif  

}


/*****************************************************************************
* FUNCTION
*	mdi_camera_stop_non_block_burst_capture
* DESCRIPTION
*	stop non blocking burst capture
* PARAMETERS
*  void
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_camera_stop_non_block_burst_capture(void)
{
	#ifdef MMI_ON_HARDWARE_P
	 	#if defined(MT6228) ||defined(MT6229)
			media_cam_stop(MOD_MMI);
		#endif  
	#endif 

	return MDI_RES_CAMERA_SUCCEED;	
}



/*****************************************************************************
* FUNCTION
*	mdi_camera_non_block_burst_capture_result_hdlr
* DESCRIPTION
*	burst capture result handler
* PARAMETERS
*	void
* RETURNS
*	BOOL
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
#if (defined(MT6228)||defined(MT6229))
static void mdi_camera_non_block_burst_capture_result_hdlr(void *msg_ptr)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	media_cam_capture_event_ind_struct *ind_p;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/	
	ind_p = (media_cam_capture_event_ind_struct*)msg_ptr;

	/* note:capture count start from 1 */

	if( (ind_p->result != MED_RES_OK) ||
		 ( (ind_p->result == MED_RES_OK) && ((ind_p->count) == g_mdi_camera_cntx.burst_cap_count)) )
	{
		/* if failed, or if it is last capture - will enter here */
		
		g_mdi_camera_cntx.burst_cap_count = 0;
		
		/* capture will also stop preview TV-Out */
		#ifdef __MMI_TVOUT__
			/* tvout not enable , restore owner to GDI */
			if(g_mdi_camera_cntx.tvout_prev_owner == MDI_TV_OWNER_GDI)
			{
				mdi_tvout_set_owner(MDI_TV_OWNER_GDI);	
			}
		#endif /* __MMI_TVOUT__ */
	}

	
	/* successful save file */
	if(ind_p->result == MED_RES_OK)
		mdi_camera_non_block_burst_shot_result_callback(MDI_RES_CAMERA_SUCCEED, ind_p->count);
	else if(ind_p->result == MED_RES_BUFFER_INSUFFICIENT)
		mdi_camera_non_block_burst_shot_result_callback(MDI_RES_CAMERA_ERR_MEMORY_NOT_ENOUGH, ind_p->count);
	else
		mdi_camera_non_block_burst_shot_result_callback(MDI_RES_CAMERA_ERR_FAILED, ind_p->count);

}
#endif  


/*****************************************************************************
* FUNCTION
*	mdi_camera_is_ready_to_capture
* DESCRIPTION
*	check hw module is ready to capture (some module requires some time to power on and init)
* PARAMETERS
*	void
* RETURNS
*	BOOL
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
BOOL mdi_camera_is_ready_to_capture(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   mmi_trace(g_sw_CAMERA, "Excute funs: mdi_camera_is_ready_to_capture() mdi_camera_p->state = %ld",mdi_camera_p->state);
	if(mdi_camera_p->state == CAMERA_STATE_PREVIEWING)
		return TRUE;
	else
		return FALSE;
}

//add by WeiD
#define MDI_CAMERA_BRIGHTNESS 5
#define MDI_CAMERA_CONTRAST   5

void mdi_camera_update_para_bt(U16 cam_para)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	ASSERT(cam_para < MDI_CAMERA_BRIGHTNESS);

	camera_setting_p->brightness = cam_para;
#ifdef MMI_ON_HARDWARE_P
	if(mdi_camera_p->state == CAMERA_STATE_PREVIEWING)
		media_cam_set_param(MOD_MMI, CAM_PARAM_BRIGHTNESS, cam_para);
#endif /* MMI_ON_HARDWARE_P */
}


void mdi_camera_update_para_ct(U16 cam_para)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	ASSERT(cam_para < MDI_CAMERA_CONTRAST);

	camera_setting_p->contrast= cam_para;
#ifdef MMI_ON_HARDWARE_P
	if(mdi_camera_p->state == CAMERA_STATE_PREVIEWING)
		media_cam_set_param(MOD_MMI, CAM_PARAM_CONTRAST, cam_para);
#endif /* MMI_ON_HARDWARE_P */
}
//add by WeiD

/*****************************************************************************
* FUNCTION
*	mdi_camera_update_para_ev
* DESCRIPTION
*	update ev setting value 
* PARAMETERS
*	cam_para		IN		ev setting value
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_camera_update_para_ev(U16 cam_para)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	ASSERT(cam_para < MDI_CAMERA_EV_COUNT);

	camera_setting_p->ev = cam_para;

#ifdef MMI_ON_HARDWARE_P
	if(mdi_camera_p->state == CAMERA_STATE_PREVIEWING)
		media_cam_set_param(MOD_MMI, CAM_PARAM_EXPOSURE, cam_para);
#endif /* MMI_ON_HARDWARE_P */
}


/*****************************************************************************
* FUNCTION
*	mdi_camera_update_para_ev_adjust
* DESCRIPTION
*	update ev setting value - for factory mode to adjust ev balance
* PARAMETERS
*	cam_para		IN		ev adjust value
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_camera_update_para_ev_adjust(U16 cam_para)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

#ifdef MMI_ON_HARDWARE_P
	if(mdi_camera_p->state == CAMERA_STATE_PREVIEWING)
		media_cam_set_param(MOD_MMI, CAM_PARAM_EV_VALUE, cam_para);
#endif /* MMI_ON_HARDWARE_P */
}


/*****************************************************************************
* FUNCTION
*	mdi_camera_update_para_zoom
* DESCRIPTION
*	update zoom setting value 
* PARAMETERS
*	cam_para		IN		zoom setting value (10 measn 1x, 40 means 4x, etc)
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_camera_update_para_zoom(U16 cam_para)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	camera_setting_p->zoom = cam_para;

#ifdef MMI_ON_HARDWARE_P
	if(mdi_camera_p->state == CAMERA_STATE_PREVIEWING)
		media_cam_set_param(MOD_MMI, CAM_PARAM_ZOOM_FACTOR, cam_para);
#endif /* MMI_ON_HARDWARE_P */
}



/*****************************************************************************
* FUNCTION
*	mdi_camera_update_para_effect
* DESCRIPTION
*	update effect setting value 
* PARAMETERS
*	cam_para		IN		effect setting value 
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_camera_update_para_effect(U16 cam_para)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	ASSERT(cam_para < MDI_CAMERA_EFFECT_COUNT);

	camera_setting_p->effect = cam_para;

#ifdef MMI_ON_HARDWARE_P
	if(mdi_camera_p->state == CAMERA_STATE_PREVIEWING)
		media_cam_set_param(MOD_MMI, CAM_PARAM_EFFECT, cam_para);
#endif /* MMI_ON_HARDWARE_P */
}



/*****************************************************************************
* FUNCTION
*	mdi_camera_update_para_wb
* DESCRIPTION
*	update wb setting value 
* PARAMETERS
*	cam_para		IN		wb setting value 
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_camera_update_para_wb(U16 cam_para)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	ASSERT(cam_para < MDI_CAMERA_WB_COUNT);
	
	camera_setting_p->wb = cam_para;

#ifdef MMI_ON_HARDWARE_P
	if(mdi_camera_p->state == CAMERA_STATE_PREVIEWING)
		media_cam_set_param(MOD_MMI, CAM_PARAM_WB, cam_para);
#endif /* MMI_ON_HARDWARE_P */
}



/*****************************************************************************
* FUNCTION
*	mdi_camera_update_para_brightness
* DESCRIPTION
*	update brightness setting value 
* PARAMETERS
*	cam_para		IN		brightness setting value 
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_camera_update_para_brightness(U16 cam_para)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	camera_setting_p->brightness = cam_para;

#ifdef MMI_ON_HARDWARE_P
	if(mdi_camera_p->state == CAMERA_STATE_PREVIEWING)
		media_cam_set_param(MOD_MMI, CAM_PARAM_BRIGHTNESS, cam_para);
#endif /* MMI_ON_HARDWARE_P */
}



/*****************************************************************************
* FUNCTION
*	mdi_camera_update_para_saturation
* DESCRIPTION
*	update saturation setting value 
* PARAMETERS
*	cam_para		IN		saturation setting value 
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_camera_update_para_saturation(U16 cam_para)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	camera_setting_p->saturation = cam_para;

#ifdef MMI_ON_HARDWARE_P
	if(mdi_camera_p->state == CAMERA_STATE_PREVIEWING)
		media_cam_set_param(MOD_MMI, CAM_PARAM_SATURATION, cam_para);
#endif /* MMI_ON_HARDWARE_P */
}


/*****************************************************************************
* FUNCTION
*	mdi_camera_update_para_contrast
* DESCRIPTION
*	update contrast setting value 
* PARAMETERS
*	cam_para		IN		contrast setting value 
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_camera_update_para_contrast(U16 cam_para)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	camera_setting_p->contrast = cam_para;

#ifdef MMI_ON_HARDWARE_P
	if(mdi_camera_p->state == CAMERA_STATE_PREVIEWING)
		media_cam_set_param(MOD_MMI, CAM_PARAM_CONTRAST, cam_para);
#endif /* MMI_ON_HARDWARE_P */
}


/*****************************************************************************
* FUNCTION
*	mdi_camera_update_para_hue
* DESCRIPTION
*	update hue setting value 
* PARAMETERS
*	cam_para		IN		hue setting value 
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_camera_update_para_hue(U16 cam_para)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	camera_setting_p->hue = cam_para;

#ifdef MMI_ON_HARDWARE_P
	if(mdi_camera_p->state == CAMERA_STATE_PREVIEWING)
		media_cam_set_param(MOD_MMI, CAM_PARAM_HUE, cam_para);
#endif /* MMI_ON_HARDWARE_P */
}

/*****************************************************************************
* FUNCTION
*	mdi_camera_update_para_night
* DESCRIPTION
*	update night mode setting value 
* PARAMETERS
*	cam_para		IN		night mode setting value 
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_camera_update_para_night(U16 cam_para)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	camera_setting_p->night = cam_para;

#ifdef MMI_ON_HARDWARE_P
	if(mdi_camera_p->state == CAMERA_STATE_PREVIEWING)
		media_cam_set_param(MOD_MMI, CAM_PARAM_NIGHT_MODE, cam_para);
#endif
}

/*****************************************************************************
* FUNCTION
*	mdi_camera_update_para_image_qty
* DESCRIPTION
*	update image quality setting value 
* PARAMETERS
*	cam_para 	IN 		image qty setting value
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_camera_update_para_image_qty(U16 cam_para)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
  
	ASSERT(cam_para < MDI_CAMERA_JPG_QTY_COUNT);

	/*gaosh Add Start For 6116 Ver: TBM780_T_20070813_1919  on 2007-8-14 14:17 */
	camera_setting_p->image_qty	= cam_para;
/*	if((g_camera_cntx.setting.image_size == CAMERA_SETTING_IMAGE_SIZE_LL)
		&&(cam_para!=MDI_CAMERA_JPG_QTY_LOW))
	{
		cam_para=MDI_CAMERA_JPG_QTY_LOW;
	}
*/	
	/*gaosh Add End  For 6116 Ver: TBM780_T_20070813_1919  on 2007-8-14 14:17 */
	
	 if(mdi_camera_p->state == CAMERA_STATE_PREVIEWING)
	{
		/*WeiD Add Start For 6195 Ver: TBM780  on 2007-8-10 18:32 */
		if((g_camera_cntx.preview_width==LCD_WIDTH) && (g_camera_cntx.preview_height==LCD_HEIGHT) && g_camera_cntx.setting.add_frame)
		{
			cam_para = 1;     //for Vimicro need quality high when have frame ,so we use 3 level.(3 level is high)
		}
		/*WeiD Add End  For 6195 Ver: TBM780  on 2007-8-10 18:32 */
		media_cam_set_param(MOD_MMI, CAM_PARAM_QUALITY, cam_para);
	}
}



/*****************************************************************************
* FUNCTION
*	mdi_camera_update_para_image_size
* DESCRIPTION
*	update image size setting value 
* PARAMETERS
*	image_width		IN		captured iamge width
*	image_height	IN	   captured	iamge height
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/

void mdi_camera_update_para_image_size(U16 image_width, U16 image_height)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
    kal_int16 value = 0;
	camera_setting_p->image_width 	= image_width;
	camera_setting_p->image_height 	= image_height;
/*
	if(image_width==80 && image_height==56)
		value = 1;
	else if(image_width==160 && image_height==120)
		value = 2;
	else if(image_width==LCD_WIDTH&& image_height==LCD_HEIGHT)
		value = 3;
	else if(image_width==320 && image_height==240)
		value = 4;
	else if(image_width==640 && image_height==480)
		value = 5;
	else
		value = 3;
	*/
	if(image_width==LCD_WIDTH && image_height==LCD_HEIGHT)
		value = 1;
	else if(image_width==176 && image_height==220)
		value = 2;
	else if(image_width==320&& image_height==240)
		value = 3;
	else if(image_width==640 && image_height==480)
		value = 4;
	else if(image_width==1280 && image_height==1024)
		value = 5;
	else
		value = 1;
	mmi_trace(g_sw_CAMERA, "Excute : mdi_camera_update_para_image_size() value = %d",value);
	media_cam_set_param(MOD_MMI, CAM_PARAM_IMAGESIZE, value);
}


/*****************************************************************************
* FUNCTION
*	mdi_camera_update_para_flash
* DESCRIPTION
*	update flash setting value 
* PARAMETERS
*	cam_para 	IN 		flash setting value
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_camera_update_para_flash(U16 cam_para)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	camera_setting_p->iso = cam_para;
   
#ifdef MMI_ON_HARDWARE_P
	media_cam_set_param(MOD_MMI, CAM_PARAM_FLASH, cam_para);
#endif /* MMI_ON_HARDWARE_P */

}



/*****************************************************************************
* FUNCTION
*	mdi_camera_update_para_iso
* DESCRIPTION
*	update iso setting value 
* PARAMETERS
*	cam_para 	IN 		iso setting value
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_camera_update_para_iso(U16 cam_para)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	camera_setting_p->iso = cam_para;

#ifdef MMI_ON_HARDWARE_P
	media_cam_set_param(MOD_MMI, CAM_PARAM_ISO, cam_para);
#endif
}


/*****************************************************************************
* FUNCTION
*	mdi_camera_update_para_ae_meter
* DESCRIPTION
*	update ae meter setting value 
* PARAMETERS
*	cam_para 	IN 		ae meter setting value
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_camera_update_para_ae_meter(U16 cam_para)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	camera_setting_p->ae_meter = cam_para;

#ifdef MMI_ON_HARDWARE_P
	media_cam_set_param(MOD_MMI, CAM_PARAM_AE_METERING, cam_para);
#endif /* MMI_ON_HARDWARE_P */
   
}



/*****************************************************************************
* FUNCTION
*	mdi_camera_update_para_shot_mode
* DESCRIPTION
*	update shot(scene) mode value 
* PARAMETERS
*	cam_para 	IN 		shot mode setting value
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_camera_update_para_shot_mode(U16 cam_para)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
}






/*****************************************************************************
* FUNCTION
*	mdi_camera_encode_layer_to_jpeg
* DESCRIPTION
*	encode a layer and save as file
* PARAMETERS
*	layer_handle	IN	 	layer handle
*	file_name		IN		filename for jpeg	
* RETURNS
*	MDI_RESULT
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
MDI_RESULT mdi_camera_encode_layer_to_jpeg(gdi_handle layer_handle, PS8 file_name)
{
#if defined(JPG_ENCODE) && defined(MMI_ON_HARDWARE_P)
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

	#if (defined(MT6228)||defined(MT6229))
		encode_data.thumbnail_mode			= FALSE;
		encode_data.overlay_frame_mode	= FALSE;
	#endif

   ret = media_img_encode(MOD_MMI, &encode_data);

	if(ret == MED_RES_OK)
		return MDI_RES_CAMERA_SUCCEED;
	else if(ret == MED_RES_DISC_FULL)
		return MDI_RES_CAMERA_ERR_DISK_FULL;
	else if(ret == MED_RES_WRITE_PROTECTION)
		return MDI_RES_CAMERA_ERR_WRITE_PROTECTION;
	else if(ret == MED_RES_NO_DISC)
		return MDI_RES_CAMERA_ERR_NO_DISK;
	else
		return MDI_RES_CAMERA_ERR_FAILED;

#else
	return MDI_RES_CAMERA_ERR_FAILED;
#endif /* JPG_ENCODE */
	
}



/*****************************************************************************
* FUNCTION
*	mdi_camera_add_post_effect
* DESCRIPTION
*	preview fail handler (driver error)
* PARAMETERS
*	msgPtr	IN 	ready msg
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mdi_camera_add_post_effect(gdi_handle layer_handle, U16 effect_id)
{
#if defined(ISP_SUPPORT) && defined(MMI_ON_HARDWARE_P)
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	S32 layer_width;
	S32 layer_height;
	PU8 buf_ptr;
	U32 buf_size;
	PU8 work_buf_ptr_1;
	PU8 work_buf_ptr_2;
	
	IMAGE_EFFECT_DEC_STRUCT effect_data;

	
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	gdi_layer_push_and_set_active(layer_handle);
   gdi_layer_get_dimension(&layer_width, &layer_height);
	gdi_layer_get_buffer_ptr(&buf_ptr);
	gdi_layer_pop_and_restore_active();

	buf_size = (layer_width*layer_height*GDI_MAINLCD_BIT_PER_PIXEL)>>3;

	#ifdef APPMEM_SUPPORT  
		/* allocate intermediate working buffer */
		work_buf_ptr_1 = AllocMediaMemory(buf_size); 
		work_buf_ptr_2 = AllocMediaMemory(buf_size); 	/* waterpaint effect needs second working buffer */
	#else
		/* if no ASM support, allocate a static memory */
		work_buf_ptr_1 = (PU8)mdi_camera_post_effect_buf_1;
		work_buf_ptr_2 = (PU8)mdi_camera_post_effect_buf_2;
	#endif

	/* TODO: fix when support 888 format */
	effect_data.data_type 					= IBR1_TYPE_RGB565;
	effect_data.data_order 					= IBR1_ORDER_BGR888;		/* not used - just give it a value */
	effect_data.source_buffer_address	= (kal_uint32)buf_ptr;
	effect_data.dest1_buffer_address		= (kal_uint32)work_buf_ptr_1;
	effect_data.dest2_buffer_address		= (kal_uint32)work_buf_ptr_2;
	effect_data.image_width					= layer_width;
	effect_data.image_height				= layer_height;
	effect_data.effect_index				= effect_id;
	effect_data.adj_level					= 0;							/* not used */
	effect_data.image_effect_dec_cb		= NULL;						/* not used */

	image_effect_dec_process(&effect_data);

	/* copy result to soruce layer */
	memcpy(buf_ptr, work_buf_ptr_1, buf_size);


	#ifdef APPMEM_SUPPORT  
		/* allocate intermediate working buffer */
		FreeMediaMemory(work_buf_ptr_1); 
		FreeMediaMemory(work_buf_ptr_2); 	/* waterpaint effect needs second working buffer */
	#endif 	
	
#endif /* ISP_SUPPORT && MMI_ON_HARDWARE_P */
}



/*****************************************************************************
* FUNCTION
*	mdi_camera_add_post_adjustment
* DESCRIPTION
*	preview fail handler (driver error)
* PARAMETERS
*	msgPtr	IN 	ready msg
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/

void mdi_camera_add_post_adjustment(gdi_handle layer_handle, U16 adjustment_id, S32 value)
{
#if defined(ISP_SUPPORT) && defined(MMI_ON_HARDWARE_P)
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	S32 layer_width;
	S32 layer_height;
	PU8 buf_ptr;
	S32 buf_size;
	PU8 work_buf_ptr_1;
	PU8 work_buf_ptr_2;
	
	IMAGE_EFFECT_DEC_STRUCT effect_data;

	
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	gdi_layer_push_and_set_active(layer_handle);
   gdi_layer_get_dimension(&layer_width, &layer_height);
	gdi_layer_get_buffer_ptr(&buf_ptr);
	gdi_layer_pop_and_restore_active();

	buf_size = (layer_width*layer_height*GDI_MAINLCD_BIT_PER_PIXEL)>>3;

	#ifdef APPMEM_SUPPORT  
		/* allocate intermediate working buffer */
		work_buf_ptr_1 = AllocMediaMemory(buf_size); 
		work_buf_ptr_2 = AllocMediaMemory(buf_size); 	/* waterpaint effect needs second working buffer */
	#else
		/* if no ASM support, allocate a static memory */
		work_buf_ptr_1 = (PU8)mdi_camera_post_effect_buf_1;
		work_buf_ptr_2 = (PU8)mdi_camera_post_effect_buf_2;
	#endif 

	/* TEMP: HW BUGS - source cant be internal SRAM*/
	memcpy(work_buf_ptr_2, buf_ptr, buf_size);
	memset(work_buf_ptr_1, 0, buf_size);
	
	/* TODO: fix when support 888 format */
	effect_data.data_type 					= IBR1_TYPE_RGB565;
	effect_data.data_order 					= IBR1_ORDER_BGR888;		/* not used - just give it a value */
	effect_data.source_buffer_address	= (kal_uint32)work_buf_ptr_2;
	effect_data.dest1_buffer_address		= (kal_uint32)work_buf_ptr_1;
	effect_data.dest2_buffer_address		= (kal_uint32)NULL;		/* not used */
	effect_data.image_width					= layer_width;
	effect_data.image_height				= layer_height;
	effect_data.effect_index				= adjustment_id;
	effect_data.adj_level					= value;							
	effect_data.image_effect_dec_cb		= NULL;						/* not used */

	image_adj_dec_process(&effect_data);

	/* copy result to soruce layer */
	memcpy(buf_ptr, work_buf_ptr_1, buf_size);

	#ifdef APPMEM_SUPPORT  
		/* allocate intermediate working buffer */
		FreeMediaMemory(work_buf_ptr_1); 
		FreeMediaMemory(work_buf_ptr_2); 	/* waterpaint effect needs second working buffer */
	#endif 	
	
#endif /* ISP_SUPPORT && MMI_ON_HARDWARE_P */
}


#endif /* __MMI_CAMERA__ */

