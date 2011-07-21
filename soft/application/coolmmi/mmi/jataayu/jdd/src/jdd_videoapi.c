//#define MODULE_NAME "JPROV"
#define FILE_NAME "jdd_videoapi.c"

/***************************************************************************
 *
 * File Name : jdd_videoapi.c
 *
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software Ltd., Bangalore ("Jataayu") 
 * or Jataayu's suppliers as the case may be. The Software is protected by 
 * copyright, including without limitation by applicable copyright laws, 
 * international treaty provisions, other intellectual property laws and 
 * applicable laws in the country in which the Software is being used. 
 * You shall not modify, adapt or translate the Software, without prior 
 * express written consent from Jataayu. You shall not reverse engineer, 
 * decompile, disassemble or otherwise alter the Software, except and 
 * only to the extent that such activity is expressly permitted by 
 * applicable law notwithstanding this limitation. Unauthorized reproduction 
 * or redistribution of this program or any portion of it may result in severe 
 * civil and criminal penalties and will be prosecuted to the maximum extent 
 * possible under the law. Jataayu reserves all rights not expressly granted. 
 * 
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND 
 * AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, AND/OR FITNESS 
 * FOR A PARTICULAR PURPOSE OR USE, TITLE OR INFRINGEMENT ARE EXPRESSLY 
 * DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. YOU ASSUME THE 
 * ENTIRE RISK AS TO THE ACCURACY AND THE USE OF THIS SOFTWARE. JATAAYU 
 * SHALL NOT BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT, 
 * EXEMPLARY, SPECIAL OR PUNITIVE DAMAGES INCLUDING WITHOUT LIMITATION 
 * ANY LOSS OF DATA, OR; LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL 
 * OR OTHER SIMILAR LOSS RESULTING FROM OR OUT OF THE USE OR INABILITY 
 * TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 *
 ***************************************************************************/

/** 
 * @file		jdd_videoapi.c
 * @ingroup		
 * @brief	
 *
 **/
#ifdef JATAAYU_SUPPORT
#ifdef MMI_ON_HARDWARE_P
#include "cs_types.h"
#endif
#include "mmi_trace.h"
#include "historygprot.h"
#include "wgui_categories.h"
#include "globaldefs.h"
#include "eventsgprot.h"
#include "wgui_status_icons.h"
#include "commonscreens.h"
#include "ddl.h"
#include "jcal.h"
#include "med_main.h"
#include "med_api.h"
#include "mdi_audio.h"
#include "med_struct.h"
#include "profilegprots.h"
#include "jddex_wnddefaults.h"

static U32 lcdBuf;
extern kal_uint32 get_lcd_frame_buffer_address(void);
extern kal_uint32 set_lcd_frame_buffer_address(kal_uint32 new_addr);
extern  void mdi_audio_resume_background_play(void);
extern  void mdi_audio_suspend_background_play(void);

JC_RETCODE jddex_PlatformGetVideoType(JC_INT8 *pcVideoMimeType, JC_UINT8 *uiVideoFormat)
{
     
	if(jc_strstr(pcVideoMimeType , "video/mp4"))
	{
        
        *uiVideoFormat= MED_TYPE_MP4 ;
	}
	else if(jc_strstr(pcVideoMimeType , "video/3gpp"))
	{
        *uiVideoFormat = MED_TYPE_3GP ;
	}
	else if(jc_strstr(pcVideoMimeType , "video/avi"))
	{
        *uiVideoFormat = MED_TYPE_MJPG ;
	}
	else
	{
    	*uiVideoFormat= MDI_FORMAT_NONE ;
	}

	return JC_OK ;
}


JC_RETCODE  CS_CreateVideoHandleFromBuffer(JC_INT8*szbuffer, int len, JC_INT8* szContentType)
{
#if !defined(__MMI_VIDEO_PLAYER__)
return JC_OK;
#else
	S32 ret;
	vid_open_struct       open_data;       
	module_type module_id = MOD_MMI;
	JC_UINT8 uiVideoFormat = 0;
	U16 dx=0;
	U16 dy=0;
	U64 totaltime=0;
      mdi_audio_suspend_background_play();
	lcdBuf=get_lcd_frame_buffer_address();

	open_data.media_mode          = MED_MODE_CLIP_ARRAY;
	open_data.data                                     = (void*)szbuffer;
	open_data.data_size                  =len;
	open_data.play_audio               = 1;
	open_data.image_width=&dx;
	open_data.image_height=&dy;
	open_data.total_time=&totaltime;

	if(JC_NULL != szContentType)
	jddex_PlatformGetVideoType(szContentType, &uiVideoFormat);
	open_data.media_type =  uiVideoFormat;//MED_TYPE_3GP/MED_TYPE_MP4/MED_TYPE_MJPG
	ret = media_vid_open(module_id, &open_data);
	mmi_trace(12,"cs_log: media_vid_open rCode[%d] for[%d] %d %d", ret, uiVideoFormat, dx, dy);

	
		
	if (MED_RES_OK == ret)
	{
	 return JC_OK;
	}
	else
	{
	 media_vid_close(module_id);
	 mdi_audio_resume_background_play();
	 return JC_ERR_FILE_OPEN;
	}
#endif
}
JC_RETCODE  CS_CreateVideoHandleFromFile(JC_CHAR *pmPath)
{
#if !defined(__MMI_VIDEO_PLAYER__)
	return JC_OK;
#else
         S32 ret;
         vid_open_struct       open_data;       
         module_type module_id = MOD_MMI;
	  U16 dx=0;
	  U16 dy=0;
    	  U64 totaltime=0;
         mdi_audio_suspend_background_play();
	  lcdBuf=get_lcd_frame_buffer_address();

         open_data.media_mode          = MED_MODE_FILE;
         open_data.data                                     = (void*)pmPath;
         open_data.play_audio               = 1;
         open_data.image_width           = &dx;
         open_data.image_height         = &dy;
         open_data.total_time=&totaltime;
         
         ret = media_vid_open(module_id, &open_data);
         mmi_trace(TRUE,"JDD_LOG Func: %s ret: 0x%x", __FUNCTION__, ret);
 
         if (MED_RES_OK == ret)
         {
                   return JC_OK;
         }
         else
         {
         	     media_vid_close(module_id);
		mdi_audio_resume_background_play();
                   return JC_ERR_FILE_OPEN;
         }
#endif
}

 
 JC_RETCODE cs_Relocate ( int x , int y , int dx, int dy)
 {
#if !defined(__MMI_VIDEO_PLAYER__)
		 return JC_ERR_UNKNOWN;
#else
 	module_type module_id = MOD_MMI;
	S32 clipX1,clipX2,clipY1,clipY2;
	S16 cutX,cutY,cutW,cutH;

	gui_get_clip(&clipX1, &clipY1, &clipX2, &clipY2);

	if(clipX1 > x)
	{
		cutX=clipX1-x;
		if(cutX>dx)
			cutX=dx;
	}
	else
		cutX=0;
	
	if(clipY1 > y)
	{
		cutY=clipY1-y;
		if(cutY>dy)
			cutY=dy;
	}
	else
		cutY=0;

	if(dx-cutX > clipX2-clipX1)
		cutW= clipX2-clipX1;
	else
		cutW=dx-cutX;
	
	if(dy-cutY > clipY2-clipY1)
		cutH= clipY2-clipY1;
	else
		cutH=dy-cutY;
		
	media_vid_display_set(module_id, dx, dy, clipX1, clipY1, cutX, cutY, cutW, cutH, 0);

	mmi_trace(TRUE,"JDD_LOG dx= %d dy=%d x=%d y=%d ", dx, dy, x, y);
	mmi_trace(TRUE,"JDD_LOG cutX=%d cutY=%d cutW=%d cutH=%d ",  cutX, cutY, cutW, cutH);
	mmi_trace(TRUE,"JDD_LOG clipX1=%d clipY1=%d clipX2=%d clipY2=%d ", clipX1, clipY1, clipX2, clipY2);
	
	return JC_OK;
#endif
 }

 JC_RETCODE jddex_PlatformDrawRect(const JC_RECT * pstRect, 
			  JC_INT32 iBorderColor, JC_INT32 iFillColor);

 JC_RETCODE cs_RelocateAndDisplay ( int x , int y , int dx, int dy, U64 time)
 {
#if !defined(__MMI_VIDEO_PLAYER__)
		 return JC_ERR_UNKNOWN;
#else
 	S32 ret;
 	module_type module_id = MOD_MMI;
	vid_seek_struct 	seek_data;	
	S32 clipX1,clipX2,clipY1,clipY2;
	S16 cutX,cutY,cutW,cutH;
  	U8 *layerBuf;

	gdi_layer_get_buffer_ptr(&layerBuf);
	set_lcd_frame_buffer_address((U32)layerBuf);

	gui_get_clip(&clipX1, &clipY1, &clipX2, &clipY2);

	if(clipX1 > x)
	{
		cutX=clipX1-x;
		if(cutX>dx)
			cutX=dx;
	}
	else
		cutX=0;
	
	if(clipY1 > y)
	{
		cutY=clipY1-y;
		if(cutY>dy)
			cutY=dy;
	}
	else
		cutY=0;

	if(dx-cutX > clipX2-clipX1)
		cutW= clipX2-clipX1;
	else
		cutW=dx-cutX;
	
	if(dy-cutY > clipY2-clipY1)
		cutH= clipY2-clipY1;
	else
		cutH=dy-cutY;
		
	media_vid_display_set(module_id, dx, dy, clipX1, clipY1, cutX, cutY, cutW, cutH, 0);

	mmi_trace(TRUE,"JDD_LOG dx= %d dy=%d x=%d y=%d ", dx, dy, x, y);
	mmi_trace(TRUE,"JDD_LOG cutX=%d cutY=%d cutW=%d cutH=%d ",  cutX, cutY, cutW, cutH);
	mmi_trace(TRUE,"JDD_LOG clipX1=%d clipY1=%d clipX2=%d clipY2=%d ", clipX1, clipY1, clipX2, clipY2);

	memset(&seek_data, 0, sizeof(vid_seek_struct));
	seek_data.time						= time;
	seek_data.get_frame				= TRUE;
	seek_data.display_width			= dx;
	seek_data.display_height		= dy;
	seek_data.display_offset_x		= clipX1;
	seek_data.display_offset_y		= clipY1;
	
	ret = media_vid_seek(module_id, &seek_data);

	mmi_trace(TRUE,"JDD_LOG Func: %s seek ret: 0x%x", __FUNCTION__, ret);
	
	if (MED_RES_OK != ret)
	{
		return JC_ERR_FILE_SEEK;  
	}
	else
		return JC_OK;
#endif
 }


 JC_RETCODE cs_GetCurrentTime(U64 *time)
 {
#if !defined(__MMI_VIDEO_PLAYER__)
		 return JC_ERR_UNKNOWN;
#else
 	module_type module_id = MOD_MMI;

	media_vid_get_current_play_time_block_req(module_id, time);

	mmi_trace(TRUE,"JDD_LOG curTime= %d",  *time);
	return JC_OK;
#endif
 }

JC_RETCODE cs_Playvideo ( int x , int y , int dx, int dy, PsFuncPtr PlayFinishCallBackFunc)
{
#if !defined(__MMI_VIDEO_PLAYER__)
	return JC_ERR_UNKNOWN;
#else
	S32 ret;
	vid_play_struct 	play_data;	
	module_type module_id = MOD_MMI;
	U8 *layerBuf;

	gdi_layer_get_buffer_ptr(&layerBuf);
	set_lcd_frame_buffer_address((U32)layerBuf);


	SetProtocolEventHandler(PlayFinishCallBackFunc,	MSG_ID_MEDIA_VID_PLAY_FINISH_IND);
	mdi_audio_set_volume(MDI_VOLUME_MEDIA, GetRingVolumeLevel());
	memset(&play_data, 0, sizeof(vid_play_struct));

	ret=cs_RelocateAndDisplay ( x ,  y ,  dx,  dy, 0);
	if(ret!=JC_OK) return ret;

	play_data.play_audio	= 1;
	ret = media_vid_play(module_id, &play_data); 

	mmi_trace(TRUE,"JDD_LOG Func: %s play ret: 0x%x", __FUNCTION__, ret);

	if (MED_RES_OK == ret)
	{
		return JC_OK;
	}
	else
	{
		return JC_ERR_FILE_OPEN;  //to be modified
	}
#endif
}

JC_RETCODE cs_Stopvideo ()
{
#if !defined(__MMI_VIDEO_PLAYER__)
		return JC_ERR_UNKNOWN;
#else
	module_type module_id = MOD_MMI;

	mmi_trace(TRUE,"JDD_LOG: Func: %s ", __FUNCTION__);
	media_vid_stop(module_id);

	return JC_OK;
#endif
}

JC_RETCODE cs_Deletevideo ()
{
#if !defined(__MMI_VIDEO_PLAYER__)
		return JC_ERR_UNKNOWN;
#else
	module_type module_id = MOD_MMI;
	mmi_trace(TRUE,"JDD_LOG Func: %s ", __FUNCTION__);
	media_vid_stop(module_id);
	media_vid_close(module_id);
	set_lcd_frame_buffer_address(lcdBuf);
	mdi_audio_resume_background_play();
	return JC_OK;
#endif
}


#if 0
void cs_VideoTest()
{
	JC_CHAR path[50];
	JC_RETCODE ret;
	U16 width = 0, height = 0;
	AnsiiToUnicodeString((S8*)path, "D:/Video/3.avi");
	CS_CreateVideoHandleFromFile(path, &width, &height);
	ret = cs_Playvideo ( 0, 60, width, height, PlayFinishCallBackFunc);
	if (ret != JC_OK)
	{
		cs_Stopvideo();
		cs_Deletevideo();
	}
}
#endif
#endif
