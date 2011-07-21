
#include "mmi_features.h"
#ifdef _KK_SUPPORT_ 
//#include "wepzone.h"
//#include "eventsgprot.h"
//#include "historygprot.h"

//#include "commonscreens.h"
//#include "todolistdef.h"
//#include "todolistenum.h"
  
//#include "custmenures.h"
#include "wgui_categories.h"
#include "globaldefs.h"
#include "wgui_categories_inputs.h"
#include "nvramprot.h"
#include "nvramenum.h" 
#include "settingprofile.h"
#include "lcd_if.h"
#include "lcd_sw.h"
#include "device.h"
#include "gdi_include.h"                /* graphic library */
#include "gdi_image_jpeg.h"             /* jpeg decoder */
#include "mdi_datatype.h"
#include "mdi_camera.h"
#include "mdi_audio.h"


#include "devcfg.h"
#include "devdecl.h"
#include "interface.h"

#include "wepcam.h"
#include "mmi_features_camera.h"        /* features */
//#include "cameraapp.h"
#include "gdi_include.h"
#include "timerevents.h"

#include "kal_release.h" 
//#include "jpeg_sw.h"
//#include "jpeg.h"
#include "app_ltlcom.h"

#include "med_utility.h"
#include "med_main.h"

//#include "funandgamesresdef.h"

#include "med_smalloc.h"
#include "dual_sim.h"
#include "wgui_softkeys.h"
#include "gui_data_types.h"  

#ifndef WIN32
//#include "mmc_vdochat.h"
//extern void vdochat_previewStart(UINT32 startX,UINT32 startY,UINT32 preW,UINT32 preH);
extern void mmc_vdochat_previewStart(UINT32 startX,UINT32 startY,UINT32 preW,UINT32 preH);
extern void mmi_camera_load_setting(void);
#endif

extern void TurnOnBacklight(U8 TimerFlag);

//bruce0912010
#ifdef __KK_UI_V2__
#define WEP_CAPTURE_WIDTH  96
#define WEP_CAPTURE_HEIGHT 96

#define WEP_TRANSFER_WIDTH   96
#define WEP_TRANSFER_HEIGHT  96

#define WEP_LOCAL_VIRGIN 10
#define WEP_REMOTE_VIRGIN 	10

#define VDO_AREA_OUTLINE_THICKNESS 		1
#define VDO_PORTRAIT_OUTLINE_THICKNESS 	2

#define VDO_INFO_ICON_STR_GAP 			4
#define VDO_OUTLINE_AREA_PORTRAIT_GAP 	1

#define WEP_COLOR_TEXT          (gui_color(255, 255, 255))
#define WEP_COLOR_PORTRAIT_BOARD          (gui_color(134, 134, 134))
#define WEP_COLOR_PORTRAIT_BG          (gui_color(255, 255, 255))

uint32_t	g_wep_start_time = 0;

extern void UI_GetCurrScreenTitle(uint16_t *str, int16_t maxlen);
extern uint8_t LUI_GetCacheSignalStrength(void);

static void WepCamInitParam( void );
//static void WepDrawVideoPortraitOutLine(void);
static void WepUpdateRoomInfo(void);

//static void WepRGB16ViewFinder(uint8_t *input, uint32_t in_w, uint32_t in_h, uint8_t *output, uint32_t out_w,uint32_t out_h);
static void WepRGB16ViewFinder2BaseLayer(uint8_t *input, uint32_t in_w, uint32_t in_h, uint8_t *output, uint32_t out_w,uint32_t out_h);

int32_t KK_title_height = 0;
int32_t KK_title_width = 0;
int32_t KK_title_x = 0;
int32_t KK_title_y = 0;

int32_t KK_video_title_height = 0;
int32_t KK_video_title_width = 0;
int32_t KK_video_title_x = 0;
int32_t KK_video_title_y = 0;

int32_t KK_content_x = 0;
int32_t KK_content_y = 0;
int32_t KK_content_width = 0;
int32_t KK_content_height = 0;

int32_t KK_video_content_x = 0;
int32_t KK_video_content_y = 0;
int32_t KK_video_content_width = 0;
int32_t KK_video_content_height = 0;

//bruce091210 for modis usb webcam
#ifdef WIN32
//BOOL VideoCaptureAPI_Initilization( int param ); //bruce0909
//BOOL VideoCaptureAPI_StartCapture( int param );
//void VideoCaptureAPI_StopCapture( int param );
void WepWin32_RGB24To16ViewFinder(uint8_t *input, uint32_t in_w, uint32_t in_h, uint16_t *output, uint32_t out_w,uint32_t out_h);
#endif

#endif /* __KK_UI_V2__ */

wepcam_struct  g_wepcam;

extern mdi_camera_context_struct g_mdi_camera_cntx;

//extern int WEP_JpegEncodeRgb2Jpeg(unsigned char  * RgbData,int RgbDataSize,int image_width,int image_height,int out_jpeg_width, int out_jpeg_height, unsigned char *p_Out,int bufsize,int q);
//extern int Dev_UJEncodeFrame(unsigned char  * RgbData,int RgbDataSize,int image_width,int image_height,int out_jpeg_width, int out_jpeg_height, unsigned char *p_Out,int bufsize,int q);

#if defined(_KK_UJ_DEC_) || defined(WIN32)
//extern int WEP_JpegDecodeJpeg2Rgb(unsigned char  * inputData, int inputDataSize,int image_width,int image_height,unsigned char *p_Out,int bufsize);
#endif
#if	1//def _KK_MTKCODEC_ //WIN32
extern void WepCamReInitMemory( void );
extern void WepCamRestoreMemory( void );
extern int WepCamResampleRGB16To16(unsigned char *src, int imgWidth, int imgHeight,  unsigned char *des, int newWidth, int newHeight );
extern GDI_RESULT WEP_CSDEncRgbFrame2Jpg( U8 *src_ptr, U32 src_size, 	S32 img_width, S32 img_height, \
													U8 *buf_ptr, U32 buf_size_max, U32 *out_file_size);
#endif

//extern void mmi_camera_get_preview_settings_for_wep(mdi_camera_setting_struct *camera_setting_data);

//extern int jpgEncodeWidth=80;
//extern int jpgEncodeHeight=80;
extern S32 UI_device_width;
extern S32 UI_device_height;
//bruce100113 b
MDI_RESULT DEV_CameraPowerOn(void);
MDI_RESULT DEV_CameraPreviewStart(  mdi_camera_preview_struct *preview_p,  mdi_camera_setting_struct *setting_p);
void DEV_CameraCaptureFromDsp(unsigned  char *buf, unsigned  int *buf_size);
void DEV_CameraPowerOff(void);
uint8_t *DEV_CameraGetFrameSrc(void);
//bruce100113 e

/* camera state machine */
typedef enum
{
    CAMERA_STATE_POWER_OFF = 0,
    CAMERA_STATE_IDLE,
    CAMERA_STATE_PREVIEWING,
    CAMERA_STATE_BARCODE_CAPTURING
} mdi_camera_state_enum;

void WepGetPreviewLayerBuf(kal_uint8 ** buf_p,kal_uint32 * buf_Len)
{
	*buf_p= g_wepcam.preview_layer_buf_ptr1;/*pangpeng add for fix the bad image problem*/
	*buf_Len = g_wepcam.preview_layer_buf_size;
}


kal_uint8  WepIsEncoding(void)
{
	return g_wepcam.isencoding;
} 
/*pangpeng add for solving the bad image while quickly moving begin */
kal_uint8 WepIsGetImgStatus(void)
{
	return g_wepcam.isGetImg;
}
void WepSetImgStatus(kal_uint8 status)
{
	g_wepcam.isGetImg = status;
}
/*pangpeng add for solving the bad image while quickly moving end */
void  VideoChatCameraPreview(void)
{
	return;
}

kal_int32  LocalPicGet(IUI * pIUI,kal_uint8 *picData, int maxSize, PFN_SETPICCB pfn_Wep_set_picdata)
{
	return 1;
}


kal_int8 VideoChatStart(void)
{
#if 0	
	if(WepCamStart())
		return 1;
	else
		return 0;
#endif
	return 1;
}


kal_int8 VideoChatStop(void)
{
	WepCamStop();
	return 1;
}


int32_t cameraIntervalGet(void)
{
 return 3000;  // msecond  
}
/*pangpeng add @090226 for wepcam SW refresh begin*/
void WepCamBltHandler(void)
{
	gui_cancel_timer(WepCamBltHandler);	
	{
		PU8 preiew_ptr;	
	//bruce0912010
#ifdef __KK_UI_V2__
		PU8 src_ptr;	

		WepUpdateRoomInfo();

//bruce100113 b
		DEV_CameraCaptureFromDsp(g_wepcam.preview_layer_buf_ptr, &g_wepcam.preview_layer_buf_size);
//bruce100113 e
//BN10112 b
#if 1
		DEV_BitBlt(
			g_wepcam.local_offset_x,								/* x1 */
			g_wepcam.local_offset_y,								/* y1 */
			UI_device_width,										/* x2 */
			g_wepcam.local_offset_y+g_wepcam.local_display_height	/* y2 */
			);	
#else
		gdi_layer_blt(GDI_LCD->blt_handle[0],	
					g_wepcam.g_wepcam_base_layer,			/* layer 0 */
					//GDI_LCD->blt_handle[1],//g_wepcam.g_wepcam_image_layer,	      /* layer 1 */
					//(gdi_handle)& GDI_LAYERS[5],
					GDI_LCD->blt_handle[2],
					0 /*g_wepcam.g_wepcam_preview_layer*/,			/* layer 2 */
					//0,									/* layer 3 */

					g_wepcam.local_offset_x,								/* x1 */
					g_wepcam.local_offset_y,								/* y1 */
					UI_device_width,		/* x2 */
					g_wepcam.local_offset_y+g_wepcam.local_display_height		/* y2 */
					);	
#endif
//BN10112 e

		gdi_layer_push_and_set_active(g_wepcam.g_wepcam_base_layer);
		gdi_layer_get_buffer_ptr(&preiew_ptr);	

		preiew_ptr += (UI_device_width * g_wepcam.local_offset_y + g_wepcam.local_offset_x) * 2;

//bruce100113 b
		src_ptr = DEV_CameraGetFrameSrc();
//bruce100113 e
		if(!DEV_IsNoneCamera()) //BN10117
			WepRGB16ViewFinder2BaseLayer(src_ptr, g_wepcam.capture_width, g_wepcam.capture_height, \
				preiew_ptr, g_wepcam.local_display_width, g_wepcam.local_display_height);

		gdi_layer_pop_and_restore_active();

#else /* !__KK_UI_V2__ */
		gdi_layer_push_and_set_active(g_wepcam.g_wepcam_preview_layer);
		gdi_layer_get_buffer_ptr(&preiew_ptr);	
		
		gdi_layer_blt(GDI_LCD->blt_handle[0],	
					g_wepcam.g_wepcam_base_layer,			/* layer 0 */
					//GDI_LCD->blt_handle[1],//g_wepcam.g_wepcam_image_layer,	      /* layer 1 */
					//(gdi_handle)& GDI_LAYERS[5],
					GDI_LCD->blt_handle[2],
					g_wepcam.g_wepcam_preview_layer,			/* layer 2 */
					//0,									/* layer 3 */

					g_wepcam.preview_offset_x,								/* x1 */
					g_wepcam.preview_offset_y,								/* y1 */
					g_wepcam.preview_width+g_wepcam.preview_offset_x,		/* x2 */
					g_wepcam.preview_height+g_wepcam.preview_offset_y		/* y2 */
					);	

		gdi_layer_push_and_set_active(g_wepcam.g_wepcam_preview_layer);
		gdi_layer_get_buffer_ptr(&preiew_ptr);	
		if(!DEV_IsNoneCamera()) //BN10117
			memcpy(preiew_ptr,(PU8)isp_preview_config_data.frame_buffer_address,isp_preview_config_data.frame_buffer_size);	
		gdi_layer_pop_and_restore_active();
#endif /* __KK_UI_V2__ */
	}
	gui_start_timer(10,WepCamBltHandler);
}
/*pangpeng add @090226 for wepcam SW refresh end*/

//#define GDI_MAINLCD_BIT_PER_PIXEL 16

kal_uint8 DrawWepVideoImage(kal_uint8  *p_Data,kal_uint32 dataLen)
{
//    S32 image_width;
//    S32 image_height;
    PU8 image_ptr,RGBdata_ptr;
    kal_uint32 RGBdata_len=0;

//bruce0912010
#ifdef __KK_UI_V2__
   if (g_wepcam.wait_video_anim != GDI_NULL_HANDLE)
    {
        gdi_image_stop_animation(g_wepcam.wait_video_anim);
       g_wepcam.wait_video_anim = GDI_NULL_HANDLE;
    }
#endif


	if(p_Data==NULL || dataLen==0)
		return 0;

	if(g_wepcam.isWepCamWindow == KAL_FALSE)
		return 0;

//#if defined(_KK_UJ_DEC_) || defined(WIN32)
	if(DEV_IsKKDec() == 1)
	{
		RGBdata_len= (g_wepcam.image_width * g_wepcam.image_height * 16) >> 3;
		RGBdata_ptr =(PU8) DEV_Malloc(MALLOC_MODULE_WEPCAM,RGBdata_len);
		DEV_MemSet(RGBdata_ptr,0,RGBdata_len);
		if(RGBdata_ptr == NULL)
		{
		#ifdef WEP_DEBUG		
			WM_DebugPrint( "DrawWepVideoImage MALLOC FAIL");
		#endif		
			return 0;
		}

		if(RGBdata_ptr !=NULL)
		{
			Dev_DecodeFrame( /*WEP_JpegDecodeJpeg2Rgb(*/
						p_Data,
						dataLen,
						g_wepcam.image_width,//image_width,
						g_wepcam.image_height,//image_height,
						RGBdata_ptr,
						RGBdata_len);
		}
		//gdi_layer_push_and_set_active(g_wepcam.g_wepcam_image_layer);	
		gdi_layer_push_and_set_active(g_wepcam.g_wepcam_base_layer);
		gdi_layer_lock_frame_buffer();
	//   	gdi_layer_get_dimension(&image_width, &image_height);
	   	gdi_layer_get_buffer_ptr(&image_ptr);
	//	memcpy(image_ptr,RGBdata_ptr,RGBdata_len);

		{
			S32 i,j;
	 		for (j = 0; j < g_wepcam.image_height; j++)
			{
				for (i = 0; i < g_wepcam.image_width; i++)
				{
					*(U16*) (image_ptr + (((j+ g_wepcam.image_offset_y) * UI_device_width+ i + g_wepcam.image_offset_x) << 1)) = *(U16*) (RGBdata_ptr + ((j * g_wepcam.image_width + i) << 1));
				}
			}

		}

		DEV_Free(RGBdata_ptr);
		gdi_layer_unlock_frame_buffer();
		gdi_layer_pop_and_restore_active();
	}
//#else /*!WIN32||  !_KK_UJ_DEC_*/
	else
	{
		if(p_Data != NULL)
		{
			GDI_RESULT gdi_ret;
			
			gdi_layer_lock_frame_buffer();
			gdi_layer_push_and_set_active(g_wepcam.g_wepcam_base_layer);	
			gdi_layer_push_clip();
		   	//gdi_layer_get_dimension(&image_width, &image_height);
		   	//gdi_layer_get_buffer_ptr(&image_ptr);
		
			//gdi_ret = gdi_image_jpeg_draw(g_wepcam.image_offset_x, g_wepcam.image_offset_y, p_Data, dataLen);
			//gdi_ret = gdi_image_jpeg_draw_resized(g_wepcam.image_offset_x, g_wepcam.image_offset_y, g_wepcam.image_width, g_wepcam.image_height, p_Data, dataLen);

			gdi_ret = gdi_image_jpeg_draw_resized(g_wepcam.image_offset_x, g_wepcam.image_offset_y, g_wepcam.image_width, g_wepcam.image_height, p_Data, dataLen);

		#ifdef WEP_DEBUG
			kal_prompt_trace(MOD_MED, "DrawWepVideoImage: gdi_image_jpeg_draw_resized return: %d", gdi_ret);
		#endif
		
			gdi_layer_pop_clip();
			gdi_layer_pop_and_restore_active();
			gdi_layer_unlock_frame_buffer();
		}
		else
		{	
		#ifdef WEP_DEBUG
			WM_DebugPrint("DrawWepVideoImage p_Data is NULL");
		#endif
			return 0;
		}
	}
//#endif /*_KK_UJ_DEC_ || WIN32*/

//bruce100105 for BN10069
#ifdef __KK_UI_V2__
//BN10112 b
	DEV_BitBlt(		g_wepcam.image_offset_x,								/* x1 */
					g_wepcam.image_offset_y,								/* y1 */
					g_wepcam.image_width+g_wepcam.image_offset_x,								/* x2 */
					g_wepcam.image_height+g_wepcam.image_offset_y);		
//BN10112 e
#else /* ! __KK_UI_V2__*/
	gdi_layer_blt(	GDI_LCD->blt_handle[0],
					g_wepcam.g_wepcam_base_layer,			/* layer 0 */
					//GDI_LCD->blt_handle[1],//g_wepcam.g_wepcam_image_layer,	      /* layer 1 */
					//(gdi_handle)& GDI_LAYERS[5],
					GDI_LCD->blt_handle[2],					/* layer 2 */
					g_wepcam.g_wepcam_preview_layer,			/* layer 3 */
					g_wepcam.image_offset_x,								/* x1 */
					g_wepcam.image_offset_y,								/* y1 */
					g_wepcam.image_width+g_wepcam.image_offset_x,								/* x2 */
					g_wepcam.image_height+g_wepcam.image_offset_y);		

#endif /* __KK_UI_V2__ */

#ifdef WEP_DEBUG
	WM_DebugPrint( "DrawWepVideoImage end");
#endif
	return 1;
}


kal_int32 CameraPicGet (ILD * pILd, kal_uint8 buffer[], kal_int32 maxSize)
{
	//bruce0912010
#ifdef __KK_UI_V2__
//    S32 image_width=g_wepcam.capture_width;
//    S32 image_height=g_wepcam.capture_height;
#else /* !__KK_UI_V2__ */
    S32 image_width=g_wepcam.preview_width;
    S32 image_height=g_wepcam.preview_height;
#endif /* __KK_UI_V2__ */
    PU8 image_ptr; 
    int quality=40;

 //   return 0 ; 

#if 0//def WIN32
	memset(buffer,0x66,1500);
	return 1500;
#else
//	image_ptr = (PU8)DEV_CameraGetFrameSrc();

#ifdef WEP_DEBUG
	WEP_DebugPrint("$$$$$$$$ CameraPicGet begin ");
#endif

	if(g_wepcam.isWepCamWindow == KAL_FALSE || !g_wepcam.isPreviewing)
		return 0;


	image_ptr = (PU8)DEV_CameraGetFrameSrc();
	
	if(image_ptr == NULL)
		return 0 ;
		
	WepSetImgStatus(FALSE);/*pangpeng add for solving the bad image while quickly moving  */
		
	DEV_MemSet(g_wepcam.capture_buf_ptr,0,WEP_MAX_PIC_SIZE);
	g_wepcam.capture_size=0;

//	g_wepcam.capture_size = previewPicGet(g_wepcam.capture_buf_ptr, WEP_MAX_PIC_SIZE);
#ifdef WEP_DEBUG
	WEP_DebugPrint("$$$$$$$$ JpegEncodeRgb2Jpeg begin g_wepcam.preview_layer_buf_size=%d",g_wepcam.preview_layer_buf_size);
#endif

	do
	{	
//#ifdef MT6230
if(!g_wepcam.isencoding)
		memcpy(g_wepcam.preview_layer_buf_ptr1,(PU8)image_ptr,g_wepcam.preview_layer_buf_size);
//#endif

		g_wepcam.isencoding = KAL_TRUE;

//bruce100113 b
#ifndef WIN32	
 
//		memcpy(g_wepcam.preview_layer_buf_ptr1,(PU8)g_wepcam.preview_layer_buf_ptr, g_wepcam.capture_width * g_wepcam.capture_height * 2);
#endif
//bruce100113 e

		 
		if(DEV_IsMtkCodec() == 0)
		{

		
			g_wepcam.capture_size = Dev_EncodeFrame( /*100113 Dev_UJEncodeFrame(*/	/*WEP_JpegEncodeRgb2Jpeg( */
										g_wepcam.preview_layer_buf_ptr1,
										g_wepcam.preview_layer_buf_size,
	//bruce0912010
#ifdef __KK_UI_V2__
									g_wepcam.capture_width,
									g_wepcam.capture_height,
#else /* !__KK_UI_V2__ */
									g_wepcam.preview_width,
									g_wepcam.preview_height,
#endif /* __KK_UI_V2__ */
										g_wepcam.transfer_width,
										g_wepcam.transfer_height,
										g_wepcam.capture_buf_ptr,
										WEP_MAX_PIC_SIZE,
										0);
			g_wepcam.isencoding = KAL_FALSE;
#ifdef WEP_DEBUG
	WEP_DebugPrint("$$$$$$$$ CameraPicGet g_wepcam.capture_size = %d ",g_wepcam.capture_size);
#endif

			if(g_wepcam.capture_size > 3000)
				return 0;
			else
			{
				memcpy(buffer,g_wepcam.capture_buf_ptr,g_wepcam.capture_size);
				return g_wepcam.capture_size;
			}
			if(quality ==0)
				return 0;
		}
		 
		else
		{
			GDI_RESULT result = GDI_FAILED;
			
			#ifdef WEP_DEBUG
			WM_DebugPrint("^^^CameraPicGet: resample begin\r\n"); 
			#endif

			g_wepcam.resample_width = WEP_TRANSFER_WIDTH;	//WEP_RESAMPLE_ENC_WIDTH;
			g_wepcam.resample_height= WEP_TRANSFER_HEIGHT;	//WEP_RESAMPLE_ENC_HEIGHT;
			g_wepcam.resample_buf_size= (g_wepcam.resample_width * g_wepcam.resample_height * 16) >> 3;;

			memset(g_wepcam.resample_buf_des_ptr, 0, g_wepcam.resample_buf_size);
			
			WepCamResampleRGB16To16(g_wepcam.preview_layer_buf_ptr1, 
	//bruce0912010
#ifdef __KK_UI_V2__
									g_wepcam.capture_width,
									g_wepcam.capture_height,
#else /* !__KK_UI_V2__ */
									g_wepcam.preview_width,
									g_wepcam.preview_height,
#endif /* __KK_UI_V2__ */

				g_wepcam.resample_buf_des_ptr, 
				g_wepcam.resample_width, 
				g_wepcam.resample_height);
			
			#ifdef WEP_DEBUG
			WM_DebugPrint("^^^CameraPicGet: resample end\r\n");
			#endif

				result = WEP_CSDEncRgbFrame2Jpg( 
											g_wepcam.resample_buf_des_ptr,
											g_wepcam.resample_buf_size,
											g_wepcam.resample_width,
											g_wepcam.resample_height,
											g_wepcam.capture_buf_ptr,
											WEP_MAX_PIC_SIZE,
											&(g_wepcam.capture_size)
											);

			#ifdef WEP_DEBUG
			{ 
				unsigned char *pBuf = (unsigned char *)(g_wepcam.capture_buf_ptr);
				
				kal_prompt_trace(MOD_MED, "CameraPicGet: (after)  capture_buf_ptr: 0x%08X, , capture_buf_ptr [data]: %d, %d, %d, %d,,,,%d, %d, %d, %d,,,,, %d, %d ", \
					(U32)pBuf, *pBuf, *(pBuf+1), *(pBuf+2), *(pBuf+3), *(pBuf+4), *(pBuf+5), *(pBuf+6), *(pBuf+7), *(pBuf+8), *(pBuf+9) );
				kal_prompt_trace(MOD_MED, "^^^CameraPicGet: (after)  g_wepcam.capture_size = %d\r\n, result=%d ", g_wepcam.capture_size, result);
			}
			#endif

			g_wepcam.isencoding = KAL_FALSE;

			if(g_wepcam.capture_size > 3000 || result != GDI_SUCCEED)
				return 0;
			else
			{
				memcpy(buffer,g_wepcam.capture_buf_ptr,g_wepcam.capture_size);
				return g_wepcam.capture_size;
			}
		}
		 
	}while(1);
#endif
}

MDI_RESULT mmi_wepcam_entry_preview_state(kal_bool status)	
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
   /************ init global var ************/
#if 0
    mdi_camera_setting_struct camera_setting_data;
    mdi_camera_preview_struct camera_preview_data;
	
    GDI_HANDLE preview_layer_handle = 0;
    U32 preview_layer_flag = 0;
    U32 blt_layer_flag = 0;
    S32 preview_wnd_offset_x = 0;
    S32 preview_wnd_offset_y = 0;
    U16 preview_wnd_width = 0;
    U16 preview_wnd_height = 0;
    MDI_RESULT  result;

#ifdef WEP_DEBUG
WM_DebugPrint("$$$$$$$$ mmi_wepcam_entry_preview_state begin");
#endif

	if(DEV_IsNoneCamera()) //BN10117
		return MDI_RES_CAMERA_SUCCEED;
//	mmi_camera_get_preview_settings_for_wep(&camera_setting_data);
	
#ifdef __CAMERA_AUTOFOCUS__
#ifdef HORIZONTAL_CAMERA
        {
            S32 tmp;

            if (g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
            {
                camera_setting_data.af_zone0_x = CAMERA_AF_POSITION_1_X;
                camera_setting_data.af_zone0_y = CAMERA_AF_POSITION_1_Y;
                camera_setting_data.af_zone1_x = CAMERA_AF_POSITION_4_X;
                camera_setting_data.af_zone1_y = CAMERA_AF_POSITION_4_Y;
                camera_setting_data.af_zone2_x = CAMERA_AF_POSITION_5_X;
                camera_setting_data.af_zone2_y = CAMERA_AF_POSITION_5_Y;
                camera_setting_data.af_zone3_x = CAMERA_AF_POSITION_3_X;
                camera_setting_data.af_zone3_y = CAMERA_AF_POSITION_3_Y;
                camera_setting_data.af_zone4_x = CAMERA_AF_POSITION_2_X;
                camera_setting_data.af_zone4_y = CAMERA_AF_POSITION_2_Y;

                TRANSLATE_COORDINATE_TO_270(camera_setting_data.af_zone0_x, camera_setting_data.af_zone0_y, tmp);
                TRANSLATE_COORDINATE_TO_270(camera_setting_data.af_zone1_x, camera_setting_data.af_zone1_y, tmp);
                TRANSLATE_COORDINATE_TO_270(camera_setting_data.af_zone2_x, camera_setting_data.af_zone2_y, tmp);
                TRANSLATE_COORDINATE_TO_270(camera_setting_data.af_zone3_x, camera_setting_data.af_zone3_y, tmp);
                TRANSLATE_COORDINATE_TO_270(camera_setting_data.af_zone4_x, camera_setting_data.af_zone4_y, tmp);

                g_camera_cntx.af_zone0_x = camera_setting_data.af_zone0_x;
                g_camera_cntx.af_zone1_x = camera_setting_data.af_zone1_x;
                g_camera_cntx.af_zone2_x = camera_setting_data.af_zone2_x;
                g_camera_cntx.af_zone3_x = camera_setting_data.af_zone3_x;
                g_camera_cntx.af_zone4_x = camera_setting_data.af_zone4_x;

                g_camera_cntx.af_zone0_y = camera_setting_data.af_zone0_y;
                g_camera_cntx.af_zone1_y = camera_setting_data.af_zone1_y;
                g_camera_cntx.af_zone2_y = camera_setting_data.af_zone2_y;
                g_camera_cntx.af_zone3_y = camera_setting_data.af_zone3_y;
                g_camera_cntx.af_zone4_y = camera_setting_data.af_zone4_y;
            }
            else
            {
                g_camera_cntx.af_zone0_x = camera_setting_data.af_zone0_x = CAMERA_AF_POSITION_1_X;
                g_camera_cntx.af_zone0_y = camera_setting_data.af_zone0_y = CAMERA_AF_POSITION_1_Y;
                g_camera_cntx.af_zone1_x = camera_setting_data.af_zone1_x = CAMERA_AF_POSITION_2_X;
                g_camera_cntx.af_zone1_y = camera_setting_data.af_zone1_y = CAMERA_AF_POSITION_2_Y;
                g_camera_cntx.af_zone2_x = camera_setting_data.af_zone2_x = CAMERA_AF_POSITION_3_X;
                g_camera_cntx.af_zone2_y = camera_setting_data.af_zone2_y = CAMERA_AF_POSITION_3_Y;
                g_camera_cntx.af_zone3_x = camera_setting_data.af_zone3_x = CAMERA_AF_POSITION_4_X;
                g_camera_cntx.af_zone3_y = camera_setting_data.af_zone3_y = CAMERA_AF_POSITION_4_Y;
                g_camera_cntx.af_zone4_x = camera_setting_data.af_zone4_x = CAMERA_AF_POSITION_5_X;
                g_camera_cntx.af_zone4_y = camera_setting_data.af_zone4_y = CAMERA_AF_POSITION_5_Y;

                TRANSLATE_COORDINATE_TO_270(camera_setting_data.af_zone0_x, camera_setting_data.af_zone0_y, tmp);
                TRANSLATE_COORDINATE_TO_270(camera_setting_data.af_zone1_x, camera_setting_data.af_zone1_y, tmp);
                TRANSLATE_COORDINATE_TO_270(camera_setting_data.af_zone2_x, camera_setting_data.af_zone2_y, tmp);
                TRANSLATE_COORDINATE_TO_270(camera_setting_data.af_zone3_x, camera_setting_data.af_zone3_y, tmp);
                TRANSLATE_COORDINATE_TO_270(camera_setting_data.af_zone4_x, camera_setting_data.af_zone4_y, tmp);
            }
        }
#else /* HORIZONTAL_CAMERA */ 
        g_camera_cntx.af_zone0_x = camera_setting_data.af_zone0_x = CAMERA_AF_POSITION_1_X;
        g_camera_cntx.af_zone0_y = camera_setting_data.af_zone0_y = CAMERA_AF_POSITION_1_Y;
        g_camera_cntx.af_zone1_x = camera_setting_data.af_zone1_x = CAMERA_AF_POSITION_2_X;
        g_camera_cntx.af_zone1_y = camera_setting_data.af_zone1_y = CAMERA_AF_POSITION_2_Y;
        g_camera_cntx.af_zone2_x = camera_setting_data.af_zone2_x = CAMERA_AF_POSITION_3_X;
        g_camera_cntx.af_zone2_y = camera_setting_data.af_zone2_y = CAMERA_AF_POSITION_3_Y;
        g_camera_cntx.af_zone3_x = camera_setting_data.af_zone3_x = CAMERA_AF_POSITION_4_X;
        g_camera_cntx.af_zone3_y = camera_setting_data.af_zone3_y = CAMERA_AF_POSITION_4_Y;
        g_camera_cntx.af_zone4_x = camera_setting_data.af_zone4_x = CAMERA_AF_POSITION_5_X;
        g_camera_cntx.af_zone4_y = camera_setting_data.af_zone4_y = CAMERA_AF_POSITION_5_Y;

#endif /* HORIZONTAL_CAMERA */ 
#endif /* __CAMERA_AUTOFOCUS__ */ 
	/*pangpeng add for solving the garbage preview problem while handwriting begin */
        blt_layer_flag =   GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1 |GDI_LAYER_ENABLE_LAYER_2| GDI_LAYER_ENABLE_LAYER_3;
        preview_layer_flag = GDI_LAYER_ENABLE_LAYER_3;
	/*pangpeng add for solving the garbage preview problem while handwriting end */	
        preview_layer_handle = g_wepcam.g_wepcam_preview_layer;

        /* use preview wnd's position */
        preview_wnd_offset_x = 0;
        preview_wnd_offset_y = 0;
        preview_wnd_width = camera_setting_data.preview_width;
        preview_wnd_height = camera_setting_data.preview_height;


   //     gdi_layer_push_and_set_active(g_wepcam.g_wepcam_preview_layer);
   //     gdi_layer_pop_and_restore_active();


	    /* preview data for preview start */
	    camera_preview_data.preview_layer_handle = preview_layer_handle;
	    camera_preview_data.preview_wnd_offset_x = preview_wnd_offset_x;
	    camera_preview_data.preview_wnd_offset_y = preview_wnd_offset_y;
	    camera_preview_data.preview_wnd_width = preview_wnd_width;
	    camera_preview_data.preview_wnd_height = preview_wnd_height;
	    camera_preview_data.blt_layer_flag = blt_layer_flag;
	    camera_preview_data.preview_layer_flag = preview_layer_flag;
//	    camera_preview_data.is_lcd_update = status;/*pangpeng add @090227 for wepcam refresh*/
	    camera_preview_data.src_key_color = GDI_COLOR_TRANSPARENT;
//	    camera_preview_data.is_tvout = FALSE;
#ifdef WEP_DEBUG
WM_DebugPrint("mmi_wepcam_entry_preview_state test1");
#endif

//bruce091210 for modis simulator capture from USB camera
#if defined(WIN32) && defined(__KK_UI_V2__)
{
	// Prepare for capture video from webcam...
	//VideoCaptureAPI_Initilization(0);

	//if(VideoCaptureAPI_StartCapture(0))
		result = MED_RES_OK;

	//g_wepcam.isPreviewing = KAL_TRUE;

	gui_start_timer(100,WepCamBltHandler);  
}
#else

//bruce100113 b
#if 0
        result = mdi_camera_preview_start(
            &camera_preview_data,
            &camera_setting_data);
		g_wepcam.isPreviewing = result<0 ? KAL_FALSE : KAL_TRUE;
#else
	
        result = DEV_CameraPreviewStart(
            &camera_preview_data,
            &camera_setting_data);
	g_wepcam.isPreviewing = result<0 ? KAL_FALSE : KAL_TRUE;
#endif
//bruce100113 e
		
#endif
//bruce0909 e for modis

#ifdef WEP_DEBUG
WM_DebugPrint("$$$$$$$$ mmi_wepcam_entry_preview_state result=%d", result);
#endif
	return result;
#endif
	return 0;
}

// int32_t previewPicGet(byte_t * imgBuff, int32_t maxSize)
// {
 
//     /*----------------------------------------------------------------*/
//     /* Local Variables                                                */
//     /*----------------------------------------------------------------*/
//     img_encode_struct encode_data;
//     S32 image_width;
//     S32 image_height;
//     PU8 image_ptr;
//     U32 imageSize;
//     S32 ret;
// 
//     /*----------------------------------------------------------------*/
//     /* Code Body                                                      */
//     /*----------------------------------------------------------------*/
//     gdi_layer_push_and_set_active(g_wepcam.g_wepcam_preview_layer);
//     gdi_layer_get_dimension(&image_width, &image_height);
//     gdi_layer_get_buffer_ptr(&image_ptr);
//     gdi_layer_pop_and_restore_active();
// 
//     encode_data.image_width = image_width;
//     encode_data.image_height = image_height;
// 
//     encode_data.image_quality = CAM_JPG_QTY_LOW;
//     encode_data.image_buffer_p = (void*)image_ptr;
//     encode_data.image_buffer_size = (image_width * image_height * GDI_MAINLCD_BIT_PER_PIXEL) >> 3;
// 
//     encode_data.target_width = image_width;
//     encode_data.target_height = image_height;
//     encode_data.media_mode = MED_MODE_STREAM;//MED_MODE_ARRAY;
//     encode_data.media_type = MED_TYPE_JPG;
// 
//     encode_data.data = (void*)imgBuff;
//     encode_data.file_size_p = (kal_uint32*) &imageSize;
//     encode_data.blocking = (kal_bool) TRUE;         /* blocking */
//     encode_data.seq_num = (kal_uint16) 0;           /* not used */
//     encode_data.file_buffer_len = (kal_uint32) maxSize;   /* not used */
//     encode_data.gray_mode = 0;
// 
// #if (MDI_CAMERA_MT6228_SERIES)
//     encode_data.thumbnail_mode = FALSE;
//     encode_data.overlay_frame_mode = FALSE;
// #endif /* MDI_CAMERA_MT6228_SERIES */ 
// 
//     ret = media_img_encode(stack_int_get_active_module_id(), &encode_data);
// 
// #ifdef WEP_DEBUG
// WM_DebugPrint("************ media_img_encode=%d",ret);
// #endif
//     if (ret == MED_RES_OK)
//     {
//         return imageSize;
//     }
//     else 
// 	 return 0;
// 
// //#endif
// }


//kal_uint32 WEP_CameraPhotoGet(kal_uint8  * pPhotoData, kal_uint32 maxSize)
// void  WEP_CameraPhotoGet(void)
// {
// //    S32 image_width=g_wepcam.preview_width;
// //    S32 image_height=g_wepcam.preview_height;
//     PU8 image_ptr;
// 
// #ifdef WIN32
// 	return;
// #endif
// 	g_wepcam.isencoding = KAL_TRUE;
// 
// if(g_wepcam.capture_buf_ptr==NULL)
// 	return;
// 
// 	DEV_MemSet(g_wepcam.capture_buf_ptr,0,WEP_MAX_PIC_SIZE);
// 	g_wepcam.capture_size=0;
// //	g_wepcam.capture_size = previewPicGet(g_wepcam.capture_buf_ptr, WEP_MAX_PIC_SIZE);
 
// #ifdef WEP_DEBUG
// 	WM_DebugPrint("$$$$$$$$ JpegEncodeRgb2Jpeg begin g_wepcam.preview_layer_buf_size=%d",g_wepcam.preview_layer_buf_size);
// #endif	
// 	g_wepcam.capture_size=WEP_JpegEncodeRgb2Jpeg( 
//  								g_wepcam.preview_layer_buf_ptr1,
//  								g_wepcam.preview_layer_buf_size,
//  								g_wepcam.transfer_width,
// 								g_wepcam.transfer_height,
//  								g_wepcam.capture_buf_ptr,
//  								WEP_MAX_PIC_SIZE,
//  								40);
// 
// //#endif
// #ifdef WEP_DEBUG
// WM_DebugPrint("$$$$$$$$ camera pic get size=%d",g_wepcam.capture_size);
// #endif		
// 
// 		g_wepcam.isencoding = KAL_FALSE;
// 
// 
// DrawWepVideoImage(g_wepcam.capture_buf_ptr,g_wepcam.capture_size);
// 
// 
// 
// 	StartTimer(ALARM_TIMER_WAIT_FOR_CALL, 2500, WEP_CameraPhotoGet);	
// 
// }

kal_uint8 g_wepcam_first_entry ;
void  WepVideoChat(void)
{
//	mdi_camera_setting_struct camera_setting_data;

	if(DEV_IsNoneCamera())//BN10117
	{
		gui_start_timer(20, WepCamBltHandler);	
		return;
	}
//bruce091210 for modis simulator capture from USB camera
#if defined(WIN32) && defined(__KK_UI_V2__)
{
	// Prepare for capture video from webcam...
	VideoCaptureAPI_Initilization(0);

	VideoCaptureAPI_StartCapture(0);

	g_wepcam.isPreviewing = KAL_TRUE;

	gui_start_timer(100,WepCamBltHandler);  
}

#else

#ifdef WEP_DEBUG
WM_DebugPrint("$$$$$$$$ WepVideoChat begin");
#endif	

	gui_cancel_timer(WepVideoChat);	//BN10013:	system pop menu trigger, then bg is white
	
//	if(mdi_get_camera_status() ==CAMERA_STATE_POWER_OFF)
	if(g_mdi_camera_cntx.state == CAMERA_STATE_POWER_OFF && g_wepcam.g_wepcam_preview_layer != 0) //BN10062		
	{
		mmi_camera_load_setting();
#ifdef WEP_DEBUG
WM_DebugPrint("$$$$$$$$ WepVideoChat test1");
#endif

//bruce100113 b
#if 0
		if(mdi_camera_power_on()==MDI_RES_CAMERA_SUCCEED)
#else
		if(DEV_CameraPowerOn() == MDI_RES_CAMERA_SUCCEED)
#endif			
			
//bruce100113 e

		{
#ifndef KK_CAMERA_REFRESH_SOFTWARE
			mmi_wepcam_entry_preview_state(TRUE) ;
#else
//			DrawWepVideoImage(g_wepcam.image_layer_buf_ptr,g_wepcam.image_layer_buf_size);
 
//			mmi_wepcam_entry_preview_state(FALSE) ;
//			gui_start_timer(20, WepCamBltHandler);
			g_wepcam.isPreviewing = KAL_TRUE;
			if(g_wepcam_first_entry)
			{
			DEV_BitBlt(
				0,//g_wepcam.local_offset_x,								/* x1 */
				0,//MMI_title_y,//g_wepcam.local_offset_y,								/* y1 */
				UI_device_width,										/* x2 */
				g_wepcam.local_offset_y+g_wepcam.local_display_height	/* y2 */
				);
			g_wepcam_first_entry = KAL_FALSE;
			}
			mmc_vdochat_previewStart(g_wepcam.local_offset_x , g_wepcam.local_offset_y,
							g_wepcam.local_display_width , g_wepcam.local_display_height);
		//	gui_start_timer(200, WepCamBltHandler);
#endif			
		}
		else
		{
       		 	DisplayPopup((PU8) GetString(15), IMG_GLOBAL_WARNING, 1, 20, WARNING_TONE);
		}
	}
#endif /* defined(WIN32) && defined(__KK_UI_V2__) */
}


void WepCamInit(void)
{
	DEV_MemSet(&g_wepcam,0,sizeof(wepcam_struct));

	//bruce0912010
#ifdef __KK_UI_V2__
	WepCamInitParam( );

	WM_PreviewCoordinateGet(&g_wepcam.preview_offset_x, &g_wepcam.preview_offset_y, &g_wepcam.capture_width, &g_wepcam.capture_height);

	g_wepcam.preview_layer_buf_size= (g_wepcam.capture_width * g_wepcam.capture_height * 16) >> 3;
	g_wepcam.preview_layer_buf_ptr =(PU8) DEV_Malloc(MALLOC_MODULE_WEPCAM,g_wepcam.preview_layer_buf_size);
	g_wepcam.preview_layer_buf_ptr1 =(PU8) DEV_Malloc(MALLOC_MODULE_WEPCAM,g_wepcam.preview_layer_buf_size);// double buffer
#else /* !__KK_UI_V2__ */
	WM_PreviewCoordinateGet(&g_wepcam.preview_offset_x, &g_wepcam.preview_offset_y, &g_wepcam.preview_width, &g_wepcam.preview_height);

	g_wepcam.preview_layer_buf_size= (g_wepcam.preview_width * g_wepcam.preview_height * 16) >> 3;
	g_wepcam.preview_layer_buf_ptr =(PU8) DEV_Malloc(MALLOC_MODULE_WEPCAM,g_wepcam.preview_layer_buf_size);
	g_wepcam.preview_layer_buf_ptr1 =(PU8) DEV_Malloc(MALLOC_MODULE_WEPCAM,g_wepcam.preview_layer_buf_size);// double buffer
#endif /* __KK_UI_V2__ */

	WM_ImageCoordinateGet(&g_wepcam.image_offset_x, &g_wepcam.image_offset_y, &g_wepcam.image_width, &g_wepcam.image_height);
	WM_TransferSizeGet(&g_wepcam.transfer_width, &g_wepcam.transfer_height);

//	g_wepcam.image_layer_buf_size= (g_wepcam.image_width * g_wepcam.image_height * GDI_MAINLCD_BIT_PER_PIXEL) >> 3;
//	g_wepcam.image_layer_buf_ptr =(PU8) DEV_Malloc(g_wepcam.image_layer_buf_size);
	
	if(g_wepcam.preview_layer_buf_ptr == NULL || g_wepcam.preview_layer_buf_ptr1==NULL)
	{
#ifdef WEP_DEBUG
		WM_DebugPrint(">>>>> WepCamInit FAIL <<<<<<<<<<");
#endif		
		DEV_Free((void *)g_wepcam.preview_layer_buf_ptr);
		g_wepcam.preview_layer_buf_ptr=NULL;
		DEV_Free((void *)g_wepcam.preview_layer_buf_ptr1);
		g_wepcam.preview_layer_buf_ptr1=NULL;
	//	DEV_Free((void *)g_wepcam.image_layer_buf_ptr);
	//	g_wepcam.image_layer_buf_ptr=NULL;
		return ;
	}

	g_wepcam.capture_buf_ptr =(PU8) DEV_Malloc(MALLOC_MODULE_WEPCAM,WEP_MAX_PIC_SIZE);

	if (g_wepcam.capture_buf_ptr == NULL)
	{
#ifdef WEP_DEBUG
		WM_DebugPrint(">>>>> WepCamInit FAIL <<<<<<<<<<");
#endif	
	}

 
	if(DEV_IsMtkCodec() == 1)
	{
	//bruce0912010
#ifdef __KK_UI_V2__
		g_wepcam.resample_buf_src_ptr =(PU8) DEV_Malloc(MALLOC_MODULE_WEPCAM,g_wepcam.capture_width * g_wepcam.capture_height * 3);
#else /* !__KK_UI_V2__ */
		g_wepcam.resample_buf_src_ptr =(PU8) DEV_Malloc(MALLOC_MODULE_WEPCAM,g_wepcam.preview_width * g_wepcam.preview_height * 3);
#endif /* __KK_UI_V2__ */

		g_wepcam.resample_buf_size= g_wepcam.preview_layer_buf_size;
		g_wepcam.resample_buf_des_ptr =(PU8) DEV_Malloc(MALLOC_MODULE_WEPCAM,g_wepcam.resample_buf_size);

		if(g_wepcam.resample_buf_src_ptr == NULL || g_wepcam.resample_buf_des_ptr==NULL)
		{
		#ifdef WEP_DEBUG
			WM_DebugPrint(">>>>> WepCamInit FAIL alloc for resample <<<<<<<<<<");
		#endif		
			DEV_Free((void *)g_wepcam.resample_buf_src_ptr);
			g_wepcam.resample_buf_src_ptr=NULL;
			DEV_Free((void *)g_wepcam.resample_buf_des_ptr);
			g_wepcam.resample_buf_des_ptr=NULL;
			return;
		}
		
		WepCamReInitMemory();
	}
//#endif
	return;	
}


bool_t WepCamStatusGet()
{
	return g_wepcam.isWepCamWindow;
}

bool_t WepCamStart(void)
{
	GDI_RESULT ret=0;
	int32_t img_x, img_y, img_w, img_h;
#ifdef WEP_DEBUG
WM_DebugPrint("&&&&&&&&& WepCamStart");
#endif

	if(g_wepcam.isWepCamWindow  == KAL_TRUE)
		return TRUE;

	if(g_wepcam.preview_layer_buf_ptr == NULL || g_wepcam.preview_layer_buf_ptr1 == NULL )
		return FALSE;
#ifdef WEP_DEBUG
WM_DebugPrint("&&&&&&&&& WepCamStart test1");
#endif	
	g_wepcam.isWepCamWindow = KAL_TRUE;
	g_wepcam.isencoding = KAL_FALSE;

        TurnOnBacklight(0);
	gdi_image_get_dimension(get_image(DEV_GetImgId(KK_IMG_VIEWCHAT_LOGO)), &img_w, &img_h);
	img_x = g_wepcam.image_offset_x + (g_wepcam.image_width-img_w)/2 ;
	img_y = g_wepcam.image_offset_y + (g_wepcam.image_height-img_h)/2 ;
	gdi_layer_multi_layer_enable();
    /* get mainlcd based layer handle */
    	gdi_layer_get_base_handle(&g_wepcam.g_wepcam_base_layer);
	 	gdi_layer_push_and_set_active(g_wepcam.g_wepcam_base_layer);
     	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
 	    gdi_layer_clear(GDI_COLOR_TRANSPARENT);
//bruce0912010
#ifndef __KK_UI_V2__		
		gdi_image_draw_id(img_x,img_y, IMG_VIEWCHAT_LOGO); // 居中显示
#endif		
		gdi_layer_pop_and_restore_active();

//bruce0912010
#ifdef __KK_UI_V2__

	ret=gdi_layer_create_cf_using_outside_memory
		(
			GDI_COLOR_FORMAT_16,
			g_wepcam.local_offset_x,
	       	g_wepcam.local_offset_y,
	       	g_wepcam.capture_width, //preview_display_width	capture_width	//must be actual preview display width!!! //local_display_width
	       	g_wepcam.capture_height, //preview_display_height	capture_height	//must be actual preview display height!!! //preview_display_height
	       	&g_wepcam.g_wepcam_preview_layer,
	       	g_wepcam.preview_layer_buf_ptr,
	       	g_wepcam.preview_layer_buf_size
        	);

#else /* !__KK_UI_V2__ */
	ret=gdi_layer_create_cf_using_outside_memory
		(
			GDI_COLOR_FORMAT_16,
			g_wepcam.preview_offset_x,
	       	g_wepcam.preview_offset_y,
	       	g_wepcam.preview_width,
	       	g_wepcam.preview_height,
	       	&g_wepcam.g_wepcam_preview_layer,
	       	g_wepcam.preview_layer_buf_ptr,
	       	g_wepcam.preview_layer_buf_size
        	);
#endif  /* __KK_UI_V2__ */

	if (ret<0)
	{
#ifdef WEP_DEBUG
		WM_DebugPrint(">>>>> WepCamStart layer create fail");
#endif		
		return FALSE;
	}

//bruce0912010
#ifdef __KK_UI_V2__	
	img_x =  (g_wepcam.local_display_width - img_w)/2 ;
	img_y =  (g_wepcam.local_display_height - img_h)/2 ;
#else /* !__KK_UI_V2__ */
	img_x =  (g_wepcam.preview_width-img_w)/2 ;
	img_y =  (g_wepcam.preview_height-img_h)/2 ;
#endif /* __KK_UI_V2__ */
 	gdi_layer_push_and_set_active(g_wepcam.g_wepcam_preview_layer);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_BLUE);
 	gdi_layer_clear(GDI_COLOR_BLUE);
//bruce0912010
#ifndef __KK_UI_V2__	
	gdi_image_draw_id(img_x,img_y, IMG_VIEWCHAT_LOGO); // 居中显示
#endif	
	gdi_layer_pop_and_restore_active();
#ifdef WEP_DEBUG
WM_DebugPrint("&&&&&&&&& WepCamStart test3");
#endif	
#if 0
    	ret=gdi_layer_create_using_outside_memory(//X, Y, WIDTH, HEIGHT, HANDLE_PTR, OUTMEM_PTR, OUTMEM_SIZE)
		g_wepcam.image_offset_x,
       	g_wepcam.image_offset_y,
       	g_wepcam.image_width,
       	g_wepcam.image_height,
       	&g_wepcam.g_wepcam_image_layer,
       	g_wepcam.image_layer_buf_ptr,
       	g_wepcam.image_layer_buf_size
        );
#ifdef WEP_DEBUG
WM_DebugPrint("&&&&&&&&& WepCamStart test4");
#endif	
	if (ret<0)
	{
#ifdef WEP_DEBUG
		WM_DebugPrint(">>>>> WepCamStart layer create fail");
#endif		
		return FALSE;
	}
		
	gdi_layer_push_and_set_active(g_wepcam.g_wepcam_image_layer);
	//gdi_layer_set_opacity(TRUE, 255);		// 255 is no transparent, 0 is fully transparnet 
	//gdi_layer_set_source_key(TRUE, GDI_COLOR_BLUE);	
	gdi_layer_set_opacity(TRUE, 255);		/* 255 is no transparent, 0 is fully transparnet */
	gdi_layer_set_source_key(TRUE, GDI_COLOR_BLUE);	
	gdi_layer_clear(GDI_COLOR_BLUE);
	gdi_image_draw_id(28,28, IMG_VIEWCHAT_LOGO); // 居中显示
	gdi_layer_pop_and_restore_active();


#ifdef WEP_DEBUG
WM_DebugPrint("&&&&&&&&& WepCamStart test2");
#endif
#endif

//bruce0912010
#ifdef __KK_UI_V2__
	//WepDrawVideoPortraitOutLine();
	//bruce100105 for BN10070	g_wep_start_time = app_getcurrtime();
#endif

	//WepVideoChat();
	//bruce091210	gui_start_timer(200, WepVideoChat); //BN10013:	system pop menu trigger, then bg is white
	WepVideoChat();

#ifdef WEP_DEBUG
WM_DebugPrint("&&&&&&&&& WepCamStart end");
#endif	
	return TRUE;
}

void WepCamPause(void)
{
	g_wepcam.isPreviewing = KAL_FALSE;
	mdi_camera_preview_stop();/*pangpeng add for blocking the preview while handwrite*/
}

void WepCamStop(void)
{
#ifdef WEP_DEBUG
	WM_DebugPrint("&&&&&&&&& WepCamStop");
#endif
	gui_cancel_timer(WepCamBltHandler);	/*pangpeng add @090226 for wepcam SW refresh */

	if(g_wepcam.isWepCamWindow == KAL_TRUE)
	{
		g_wepcam.isWepCamWindow = KAL_FALSE;
		g_wepcam.isPreviewing = KAL_FALSE;

//bruce100113 b
#if 0
		mdi_camera_preview_stop();
		mdi_camera_power_off();
#else
		DEV_CameraPowerOff();
#endif

//bruce100113 e

		gdi_layer_multi_layer_disable();

		gdi_layer_free(g_wepcam.g_wepcam_preview_layer);
		g_wepcam.g_wepcam_preview_layer = 0;
	//	gdi_layer_free(g_wepcam.g_wepcam_image_layer);
	//	g_wepcam.g_wepcam_image_layer = NULL;

//bruce091210 for modis simulator capture from USB camera
#if defined(WIN32) && defined(__KK_UI_V2__)
	VideoCaptureAPI_StopCapture(0);
#endif

		TurnOffBacklight();/*pangpeng add solving the backlight problem while exit preview*/
	}
}
void WepCamExit(void)
{
#ifdef WEP_DEBUG
	WM_DebugPrint("&&&&&&&&& WepCamExit");
#endif
	if(g_wepcam.capture_buf_ptr!= NULL)
	{
		DEV_Free((void *)g_wepcam.capture_buf_ptr );
		g_wepcam.capture_buf_ptr = NULL;
	}

	if(g_wepcam.preview_layer_buf_ptr != NULL)
	{
		DEV_Free((void *)g_wepcam.preview_layer_buf_ptr );
		g_wepcam.preview_layer_buf_ptr = NULL;
	}

	if(g_wepcam.preview_layer_buf_ptr1 != NULL)
	{
		DEV_Free((void *)g_wepcam.preview_layer_buf_ptr1 );
		g_wepcam.preview_layer_buf_ptr1 = NULL;
	}
/*	if(g_wepcam.image_layer_buf_ptr != NULL)
	{
		DEV_Free((void *)g_wepcam.image_layer_buf_ptr );
		g_wepcam.image_layer_buf_ptr = NULL;
	}*/

 
	if(DEV_IsMtkCodec() == 1)
	{
		if(g_wepcam.resample_buf_src_ptr!= NULL)
		{
			DEV_Free((void *)g_wepcam.resample_buf_src_ptr );
			g_wepcam.resample_buf_src_ptr = NULL;
		}
		if(g_wepcam.resample_buf_des_ptr!= NULL)
		{
			DEV_Free((void *)g_wepcam.resample_buf_des_ptr );
			g_wepcam.resample_buf_des_ptr = NULL;
		}

		WepCamRestoreMemory();
	}
//#endif	
}

 
#define GDI_RETURN return //for reserved from gdi
GDI_RESULT WEP_CSDEncRgbRaw2Jpg(
            U8 *src_ptr,
            U32 src_size,
            S32 src_width,
            S32 src_height,
            U8 *buf_ptr,
            U32 buf_size_max,
            U32 *out_file_size)
{
#if defined(GDI_USING_JPEG_ENCODE)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    img_encode_struct encode_data;
    S32 image_width;
    S32 image_height;
    PU8 image_ptr;
    U32 file_size;
    S32 ret;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
   image_ptr = (PU8)src_ptr;
   image_width = src_width;
   image_height = src_height;

    encode_data.image_width = image_width;
    encode_data.image_height = image_height;

    encode_data.image_quality = CAM_JPG_QTY_LOW;
    encode_data.image_buffer_p = (void*)image_ptr;
    encode_data.image_buffer_size = (image_width * image_height * 16) >> 3;	//src_size

    encode_data.target_width = image_width;
    encode_data.target_height = image_height;
    encode_data.media_mode = MED_MODE_ARRAY;
    encode_data.media_type = MED_TYPE_JPG;

    encode_data.data = (void*)buf_ptr;
    encode_data.file_size_p = (kal_uint32*) & file_size;
    encode_data.blocking = (kal_bool) TRUE;         /* blocking */
    encode_data.seq_num = (kal_uint16) 0;           /* not used */
    encode_data.file_buffer_len = (kal_uint32) buf_size_max;
    encode_data.gray_mode = 0;

#ifdef JPEG_ENCODE_THUMBNAIL_MODE

    if(gdi_image_encoding_flag & GDI_IMAGE_ENCODING_FLAG_JPEG_YUV_GRAY)
        encode_data.jpeg_yuv_mode = JPEG_FORMAT_GRAY;
    else if(gdi_image_encoding_flag & GDI_IMAGE_ENCODING_FLAG_JPEG_YUV_442)
        encode_data.jpeg_yuv_mode = JPEG_FORMAT_YUV422;
    else
        encode_data.jpeg_yuv_mode = JPEG_FORMAT_YUV420;
    
    gdi_image_encoding_flag = 0; // reset to default flag
    
    encode_data.thumbnail_mode = FALSE;
    encode_data.overlay_frame_mode = FALSE;
#endif 

#ifdef WEP_DEBUG
   WM_DebugPrint( "^^^WEP_CSDEncRgbRaw2Jpg: encode_data.data ptr = %d\r\n, encode_data.file_buffer_len = %d, ", \
   					(U32)encode_data.data, encode_data.file_buffer_len);
#endif

    ret = media_img_encode(stack_int_get_active_module_id(), &encode_data);

   *out_file_size = file_size;

#ifdef WEP_DEBUG
   WM_DebugPrint("^^^WEP_CSDEncRgbRaw2Jpg: ret = %d\r\n, out_file_size= %d", ret, file_size);
#endif

    if (ret == MED_RES_OK)
    {
        GDI_RETURN(GDI_SUCCEED);
    }
    else if (ret == MED_RES_DISC_FULL)
    {
        GDI_RETURN(GDI_IMAGE_ENCODER_ERR_DISK_FULL);
    }
    else if (ret == MED_RES_WRITE_PROTECTION)
    {
        GDI_RETURN(GDI_IMAGE_ENCODER_ERR_WRITE_PROTECTION);
    }
    else if (ret == MED_RES_NO_DISC)
    {
        GDI_RETURN(GDI_IMAGE_ENCODER_ERR_NO_DISK);
    }
    else
    {
        GDI_RETURN(GDI_FAILED);
    }

#else
    GDI_RETURN(GDI_FAILED);
#endif
}

GDI_RESULT WEP_CSDEncRgbFrame2Jpg(
            U8 *src_ptr,
            U32 src_size,
            S32 img_width,
            S32 img_height,
            U8 *buf_ptr,
            U32 buf_size_max,
            U32 *out_file_size)
{
    GDI_RESULT result = GDI_FAILED;
#if defined(GDI_USING_JPEG_ENCODE)
    do
    {
        result = WEP_CSDEncRgbRaw2Jpg( src_ptr, src_size, img_width, img_height,  buf_ptr, buf_size_max, out_file_size);//
    } while(0);

#endif
    GDI_RETURN(result);
}

//resample
#define RGB24ToRGB16(r,g,b) \
	(((r&0x00f8)<<8)|((g&0x00fc)<<3)|(b&0xf8)>>3)

int WepCamResampleRGB16To16(unsigned char *src, int imgWidth, int imgHeight,
					  unsigned char *des, int newWidth, int newHeight )
{       
	int i,j;
	unsigned char *r=NULL, *g, *b;
	int xx, yy, xp, yp, dx, dy, iw, yw;
	unsigned char *pp/*, *dp*/;
	unsigned char p00, p01, p10, p11;
	unsigned char kr, kg, kb;
	kal_uint16 *ptemp = (kal_uint16 *)src;
	int panSize;

	panSize = imgWidth*imgHeight;
	//r = med_alloc_ext_mem(panSize*3);
	//bruce0912010
#ifdef __KK_UI_V2__
	memset(g_wepcam.resample_buf_src_ptr, 0, g_wepcam.capture_width * g_wepcam.capture_height * 3);
#else /* !__KK_UI_V2__ */
	memset(g_wepcam.resample_buf_src_ptr, 0, g_wepcam.preview_width * g_wepcam.preview_height * 3);
#endif /* __KK_UI_V2__ */	
	r = (unsigned char *)g_wepcam.resample_buf_src_ptr;
	if(!r) return 0;

	g = r + panSize;
	b = g + panSize;

	for( i = 0; i < panSize; i ++, ptemp ++){
		kb=*ptemp<<3;
		kg=*ptemp>>3;kg&=0xfc;
		kr=*ptemp>>8;kb&=0xf8;
		r[i] = kr; g[i] = kg; b[i] = kb;
	}

	ptemp = (kal_uint16 *)des;
	for(j=0; j<newHeight; j++) {
		//缩放后的y位置
		yy = ((j*imgHeight)<<8)/newHeight;
		yp = yy>>8;  dy = yy&0xff; 

		yw = yp * imgWidth;
		for(i=0, iw = 0; i<newWidth; i++, iw += imgWidth,ptemp ++) {
			//缩放后的x位置
			xx = (iw<<8)/newWidth;
			xp = xx>>8; dx = xx&0xff;
			//r
			pp = r + xp+yw; 
			p00 = *pp;                 p10 = *(pp+1); 
			p01 = *(pp+imgWidth);      p11 = *(pp+imgWidth+1);
			kr = (unsigned char)(((p11*dx+p01*(256-dx))*dy + (p10*dx+p00*(256-dx))*(256-dy))>>16);

			//g
			pp = g + xp+yw; 
			p00 = *pp;                 p10 = *(pp+1); 
			p01 = *(pp+imgWidth);      p11 = *(pp+imgWidth+1);
			kg = (unsigned char)(((p11*dx+p01*(256-dx))*dy + (p10*dx+p00*(256-dx))*(256-dy))>>16);

			//b
			pp = b + xp+yw; 
			p00 = *pp;                 p10 = *(pp+1); 
			p01 = *(pp+imgWidth);      p11 = *(pp+imgWidth+1);
			kb = (unsigned char)(((p11*dx+p01*(256-dx))*dy + (p10*dx+p00*(256-dx))*(256-dy))>>16);

			*ptemp = RGB24ToRGB16(kr,kg,kb);

		}
	}  
	//med_free_ext_mem((void **)&r);
	return 1;
}

//re-config camera preview mem
#define MAX_WEPCAM_PREVIEW_MEM_SIZE_A 	(30 * 1024)
#define MAX_WEPCAM_PREVIEW_MEM_SIZE_B 	(40 * 1024)
//extern kal_int32 cam_preview_mem[2];
kal_int32 wepcam_preview_mem[2] = {0, 0};

void WepCamReInitMemory( void )
{
	#if 0
	wepcam_preview_mem[0] = cam_preview_mem[0];
	wepcam_preview_mem[1] = cam_preview_mem[1];

	if(DEV_GetPlatSWVer() == KK_MTK6235)
		cam_preview_mem[0] = MAX_WEPCAM_PREVIEW_MEM_SIZE_B;
	else
		cam_preview_mem[0] = MAX_WEPCAM_PREVIEW_MEM_SIZE_A;
	#endif
}

void WepCamRestoreMemory( void )
{
	#if 0
	cam_preview_mem[0] = wepcam_preview_mem[0];
	cam_preview_mem[1] = wepcam_preview_mem[1];
	#endif
}
 

void KKCamUpdateFrameData(uint8_t *src_buf_p, uint32_t src_data_len)
{
	extern BOOL	gFlagEntryWep;
	uint8_t *dst_buf_p= g_wepcam.preview_layer_buf_ptr1;/*pangpeng add for fix the bad image problem*/
	uint32_t dst_buf_len = g_wepcam.preview_layer_buf_size;
	uint32_t valid_data_len = 0;

	if(gFlagEntryWep)
	{
		if(src_data_len > 0)
		{
			//WepGetPreviewLayerBuf(&pData,&srcDataLen);

			if(dst_buf_len > src_data_len)
				valid_data_len = src_data_len;
			else
				valid_data_len = dst_buf_len;
			
			if(kk_is_support_horizon_cam() > 0) //BN 10122
			{
				if((dst_buf_p != NULL) && (src_buf_p != NULL) && (WepIsEncoding() == KAL_FALSE)/*&&(!WepIsGetImgStatus())*/)
				{
					S32 i,j;
					for(i = 0; i < g_wepcam.capture_width; i++)
						for(j = 0; j < g_wepcam.capture_height; j++)	
							memcpy(dst_buf_p+((j)*g_wepcam.capture_width+(g_wepcam.capture_height-1-i))*2,(PU8)src_buf_p+(i*g_wepcam.capture_width+j)*2,2);
				}
			}
			else
			{
				if((dst_buf_p != NULL) && (src_buf_p != NULL) && (WepIsEncoding() == KAL_FALSE)&&(!WepIsGetImgStatus()))
				{
				  	memcpy((void *)dst_buf_p, (void *)src_buf_p, valid_data_len);
					WepSetImgStatus(KAL_TRUE);
				}
			}
		}
	}
}

//BN10010 e

//bruce0912010
#ifdef __KK_UI_V2__
void WepEntryInitParam( void )
{
	int32_t img_w=0, img_h=0;

	//comoon title 
	KK_title_x = 0;
	KK_title_y = 0;
	KK_title_width = UI_device_width;
	if(UI_device_height < 240)
	{
		gui_measure_image((S8 *)GetImage(DEV_GetImgId(KK_IMG_VIEWCHAT_V2_KK_TITLE_MINI)),&img_w,&img_h);
		KK_title_height = img_h;
	}
	else if(UI_device_height >= 240 && UI_device_height < 320)
	{
		gui_measure_image((S8 *)GetImage(DEV_GetImgId(KK_IMG_VIEWCHAT_V2_KK_TITLE_MIDDLE)),&img_w,&img_h);
		KK_title_height = img_h;

		//
		//gui_measure_image((UI_image_type)GetImage(IMG_VIEWCHAT_V2_KK_TITLE_MIDDLE),&img_w,&img_h);
		//KK_title_height = img_h;
	}
	else		
	{
		gui_measure_image((S8 *)GetImage(DEV_GetImgId(KK_IMG_VIEWCHAT_V2_KK_TITLE)),&img_w,&img_h);
		KK_title_height = img_h;
	}
	
	//video title 
	KK_video_title_x = 0;
	KK_video_title_y = 0;
	KK_video_title_width = UI_device_width;
	if(UI_device_height <= 240)
	{
		KK_video_title_height = 5;
	}
	else
	{
		gui_measure_image((S8 *)GetImage(DEV_GetImgId(KK_IMG_VIEWCHAT_V2_KK_VIDEO_TITLE_CH)),&img_w,&img_h);
		KK_video_title_height = img_h;
	}

	//common content
	KK_content_x = 0;
	KK_content_y = KK_title_height;
	KK_content_width = UI_device_width;
	KK_content_height = UI_device_height - MMI_softkey_height - KK_title_height;

	//
	MMI_content_x = KK_content_x;
	MMI_content_y = KK_content_y;
	MMI_content_width = KK_content_width;
	MMI_content_height = KK_content_height;

	MMI_title_x = 0;
	MMI_title_y = 0;
	MMI_title_width = KK_content_width;
	MMI_title_height = KK_content_height;

#ifdef WEP_DEBUG
   WM_DebugPrint( "->WepEntryInitParam: KK_title_x = %d, KK_title_y = %d, KK_title_width = %d, MMI_title_height = %d,", \
   					KK_title_x, KK_title_y, KK_title_width, KK_title_height);
   WM_DebugPrint( "->WepEntryInitParam: KK_video_title_x = %d, KK_video_title_y = %d, KK_video_title_width = %d, KK_video_title_height = %d,", \
   					KK_video_title_x, KK_video_title_y, KK_video_title_width, KK_video_title_height);
   WM_DebugPrint( "->WepEntryInitParam: KK_content_x = %d, KK_content_y = %d, KK_content_width = %d, KK_content_height = %d,", \
   					KK_content_x, KK_content_y, KK_content_width, KK_content_height);
#endif
	
	//video content: init by WepCamInitParam, because remote image height is variable
}

void WepCamInitParam( void )
{
	video_resol_id local;
	video_resol_id remote;
	//video_resol_id transfer;
	//video_resol_id capture;
	uint8_t ts[32];
	UI_character_type s[64];
	int32_t str_w=0, str_h=0, img_w=0, img_h=0;
	int16_t adjust_horizontal_x=0;

	DEV_GeVideoResolution(&local, &remote);

	 
	if(remote > VIDEO_RESOL_VER1_MAX)
		remote = VIDEO_RESOL_VER1_MAX;
		
	if(local > remote)
		local = remote;

	//remote display resolution
	switch(remote)
	{
		case VIDEO_RESOL00_48X48:
			g_wepcam.remote_width= 48;
			g_wepcam.remote_height = 48;
			break;
		case VIDEO_RESOL01_64X64:
			g_wepcam.remote_width= 64;
			g_wepcam.remote_height = 64;
			break;
		case VIDEO_RESOL02_80X80:
			g_wepcam.remote_width = 80;
			g_wepcam.remote_height = 80;
			break;
		case VIDEO_RESOL03_96X96:
			g_wepcam.remote_width = 96;
			g_wepcam.remote_height = 96;
			break;
		case VIDEO_RESOL04_112X112:
			g_wepcam.remote_width = 112;
			g_wepcam.remote_height = 112;
			break;
		case VIDEO_RESOL05_128X96:
			g_wepcam.remote_width = 128;
			g_wepcam.remote_height = 96;
			break;
		case VIDEO_RESOL06_128X128:
			g_wepcam.remote_width = 128;
			g_wepcam.remote_height = 128;
			break;
#if 0			
		case VIDEO_RESOL07_160X120:
			g_wepcam.remote_width = 160;
			g_wepcam.remote_height = 120;
			break;
		case VIDEO_RESOL08_176X144:
			g_wepcam.remote_width = 176;
			g_wepcam.remote_height = 144;
			break;
		case VIDEO_RESOL09_320X240:
		case VIDEO_RESOL10_352X288:
			g_wepcam.remote_width = 320;
			g_wepcam.remote_height = 240;
#endif		
		default:
			g_wepcam.remote_width = 96;
			g_wepcam.remote_height = 96;
			break;
	}	

	//preview display resolution
	switch(local)
	{
		case VIDEO_RESOL00_48X48:
			g_wepcam.local_display_width = 48;
			g_wepcam.local_display_height = 48;
			break;
		case VIDEO_RESOL01_64X64:
			g_wepcam.local_display_width = 64;
			g_wepcam.local_display_height = 64;
			break;
		case VIDEO_RESOL02_80X80:
			g_wepcam.local_display_width = 80;
			g_wepcam.local_display_height = 80;
			break;
		case VIDEO_RESOL03_96X96:
			g_wepcam.local_display_width = 96;
			g_wepcam.local_display_height = 96;
			break;
		case VIDEO_RESOL04_112X112:
			g_wepcam.local_display_width = 112;
			g_wepcam.local_display_height = 112;
			break;
		case VIDEO_RESOL05_128X96:
			g_wepcam.local_display_width = 128;
			g_wepcam.local_display_height = 96;
			break;
		case VIDEO_RESOL06_128X128:
			g_wepcam.local_display_width = 128;
			g_wepcam.local_display_height = 128;
			break;
#if 0			
		case VIDEO_RESOL07_160X120:
			g_wepcam.local_display_width = 160;
			g_wepcam.local_display_height = 120;
			break;
		case VIDEO_RESOL08_176X144:
			g_wepcam.local_display_width = 176;
			g_wepcam.local_display_height = 144;
			break;
		case VIDEO_RESOL09_320X240:
		case VIDEO_RESOL10_352X288:
			g_wepcam.local_display_width = 320;
			g_wepcam.local_display_height = 240;
#endif		
		default:
			g_wepcam.local_display_width = 64;
			g_wepcam.local_display_height = 64;
			break;
	}	

	/*----------------------------------------------------------------*/
	//bruce100113 b
	if(DEV_GetPlatBBVer()!=KK_CSD6223)
	{
		//capture and sensor preview resolution 
		g_wepcam.capture_width = 96;//WEP_CAPTURE_WIDTH;
		g_wepcam.capture_height = 96;//WEP_CAPTURE_HEIGHT;

		//encode resolution
		g_wepcam.transfer_width = 96;//WEP_TRANSFER_WIDTH;
		g_wepcam.transfer_height = 96;//WEP_TRANSFER_HEIGHT;
	}
	else
	{
		uint16_t cap_width=0, cap_height=0, transf_width=0, transf_height=0;
		DEV_CameraGetResolution(&cap_width, &cap_height, &transf_width, &transf_height);
			
		//capture and sensor actual preview resolution, not local display resolution
		g_wepcam.capture_width = cap_width;
		g_wepcam.capture_height = cap_height;

		//encode resolution
		g_wepcam.transfer_width = transf_width;
		g_wepcam.transfer_height = transf_height;
	}
	//bruce100113 e
	
	/*----------------------------------------------------------------*/
	//local display < capture
	//check preview display resolution: display width & height must less(<) than caputre width & height
	if(g_wepcam.local_display_width > g_wepcam.capture_width || \
		g_wepcam.local_display_height > g_wepcam.capture_height)
	{
		g_wepcam.local_display_width = g_wepcam.capture_width;
		g_wepcam.local_display_height = g_wepcam.capture_height;
	}

	//local display < remote
	if((g_wepcam.local_display_width > g_wepcam.remote_width) ||\
		(g_wepcam.local_display_height > g_wepcam.remote_height) )
	{
		g_wepcam.local_display_width = 64;
		g_wepcam.local_display_height = 64;

		g_wepcam.remote_width = 96;
		g_wepcam.remote_height = 96;
	}

	/*----------------------------------------------------------------*/
	//outer line coordinagte
	g_wepcam.vdoarea_outline_x = 0;
	if(UI_device_height > 240)
	{
		gui_measure_image((S8 *)GetImage(DEV_GetImgId(KK_IMG_VIEWCHAT_V2_KK_VIDEO_TITLE_CH)),&img_w,&img_h);
		g_wepcam.vdoarea_outline_y = img_h;	//MMI_status_bar_height;
	}
	else
	{
		g_wepcam.vdoarea_outline_y = KK_video_title_height;	//MMI_status_bar_height;
	}
	
	g_wepcam.vdoarea_outline_y = MMI_status_bar_height;
		
	//adjust horizontal coordinate to left
	adjust_horizontal_x = (UI_device_width / 2 - g_wepcam.remote_width) / 2;
	if(DEV_SetVideoOffsetX() > 0 && DEV_SetVideoOffsetX() < adjust_horizontal_x)
		adjust_horizontal_x += DEV_SetVideoOffsetX();

	
	//remote coordinagte as it's outline reference
	g_wepcam.remote_virgin = WEP_REMOTE_VIRGIN;
	g_wepcam.remote_offset_x = UI_device_width / 2 - g_wepcam.remote_width - adjust_horizontal_x;
	if(g_wepcam.remote_offset_x <= VDO_AREA_OUTLINE_THICKNESS) //bruce
		g_wepcam.remote_offset_x = VDO_AREA_OUTLINE_THICKNESS + 1;
	g_wepcam.remote_offset_y = g_wepcam.vdoarea_outline_y + VDO_AREA_OUTLINE_THICKNESS + \
				VDO_OUTLINE_AREA_PORTRAIT_GAP + VDO_PORTRAIT_OUTLINE_THICKNESS;	

	g_wepcam.remote_outline_x = g_wepcam.remote_offset_x - VDO_PORTRAIT_OUTLINE_THICKNESS;
	g_wepcam.remote_outline_y = g_wepcam.remote_offset_y - VDO_PORTRAIT_OUTLINE_THICKNESS;

	//local preview coordinate as it's outline reference
	g_wepcam.local_virgin = WEP_LOCAL_VIRGIN;
	g_wepcam.local_offset_x = UI_device_width / 2 + WEP_LOCAL_VIRGIN - adjust_horizontal_x;
	g_wepcam.local_offset_y = g_wepcam.remote_offset_y + g_wepcam.remote_height - g_wepcam.local_display_height;

	g_wepcam.local_outline_x = g_wepcam.local_offset_x - VDO_PORTRAIT_OUTLINE_THICKNESS;
	g_wepcam.local_outline_y = g_wepcam.local_offset_y - VDO_PORTRAIT_OUTLINE_THICKNESS - VDO_OUTLINE_AREA_PORTRAIT_GAP;
	
	//content coordinate
	g_wepcam.content_y = g_wepcam.vdoarea_outline_y + g_wepcam.remote_height \
		+ (VDO_PORTRAIT_OUTLINE_THICKNESS+VDO_OUTLINE_AREA_PORTRAIT_GAP+VDO_AREA_OUTLINE_THICKNESS);
	g_wepcam.outline_height = g_wepcam.remote_height \
		+ (VDO_PORTRAIT_OUTLINE_THICKNESS+VDO_OUTLINE_AREA_PORTRAIT_GAP+VDO_AREA_OUTLINE_THICKNESS)*2;
	g_wepcam.outline_height = g_wepcam.outline_height;

	//kk video content when video chatting or room
	KK_video_content_x = 0;
	KK_video_content_y = g_wepcam.content_y;
	KK_video_content_width = UI_device_width;
	KK_video_content_height = UI_device_height - MMI_softkey_height - KK_video_content_y;
	

	//check if show room/buddy name
	if(g_wepcam.remote_height - g_wepcam.local_display_height > WM_FontHeightGet())
	{
	//	int32_t img_w = 0;
	//	int32_t img_h = 0;
		int32_t buddy_middle_y = 0;
	
		g_wepcam.has_buddy_name = TRUE;	
		
		gui_measure_image((S8 *)GetImage(DEV_GetImgId(KK_IMG_VIEWCHAT_V2_BUDDY_NAME)),&img_w,&img_h);
		buddy_middle_y = g_wepcam.local_offset_y - \
			(g_wepcam.remote_height - g_wepcam.local_display_height ) /2 - VDO_PORTRAIT_OUTLINE_THICKNESS;
		
		g_wepcam.buddy_icon_x = g_wepcam.local_offset_x;
		g_wepcam.buddy_icon_y = buddy_middle_y - img_h / 2;

		g_wepcam.buddy_name_x = g_wepcam.local_offset_x + img_w + VDO_INFO_ICON_STR_GAP;
		g_wepcam.buddy_name_y= buddy_middle_y - WM_FontHeightGet() /2 - 2;

		g_wepcam.buddy_name_max= (UI_device_width -g_wepcam.buddy_name_x -\
			VDO_AREA_OUTLINE_THICKNESS) / WM_FontHeightGet();		
	}
	else
		g_wepcam.has_buddy_name = FALSE;

	//during time and signal level
	sprintf((char_t *) ts, "%02d:%02d", 59, 59);
	mmi_asc_to_ucs2((char_t *) s, (char_t *)ts);
	gui_measure_string(s, &str_w, &str_h);
	gdi_image_get_dimension(get_image(DEV_GetImgId(KK_IMG_VIEWCHAT_V2_DURING_TIME)), &img_w, &img_h);
	
	if( (UI_device_width-(g_wepcam.local_offset_x+g_wepcam.local_display_width+VDO_PORTRAIT_OUTLINE_THICKNESS)) \
		>  (img_w+str_w+VDO_INFO_ICON_STR_GAP) )
	{
		uint16_t preview_right_middle_y = g_wepcam.local_offset_y + g_wepcam.local_display_height / 2;
		
		g_wepcam.has_during_time = TRUE;
		g_wepcam.has_during_time_only_text = FALSE;

		//during time coordinate
		g_wepcam.during_icon_x = g_wepcam.local_offset_x+g_wepcam.local_display_width+\
			VDO_PORTRAIT_OUTLINE_THICKNESS+VDO_INFO_ICON_STR_GAP;
		g_wepcam.during_icon_y =  g_wepcam.local_offset_y + (preview_right_middle_y - g_wepcam.local_offset_y)/2 - img_h / 2 + 5;

		g_wepcam.during_time_x = g_wepcam.during_icon_x + img_w + VDO_INFO_ICON_STR_GAP;
		g_wepcam.during_time_y = g_wepcam.local_offset_y + (preview_right_middle_y - g_wepcam.local_offset_y)/2 - str_h / 2 + 5 + 2;

		//signal coordinate
		gdi_image_get_dimension(get_image(DEV_GetImgId(KK_IMG_VIEWCHAT_V2_GPRS_SIG44)), &img_w, &img_h);
		g_wepcam.sig_level_x=  g_wepcam.during_icon_x;
		g_wepcam.sig_level_y = preview_right_middle_y + img_h / 2 -5;
	}
	else if( (UI_device_width-(g_wepcam.local_offset_x+g_wepcam.local_display_width+VDO_PORTRAIT_OUTLINE_THICKNESS)) \
		>  (str_w) )
	{
		uint16_t preview_right_middle_y = g_wepcam.local_offset_y + g_wepcam.local_display_height / 2;
		
		g_wepcam.has_during_time_only_text = TRUE;
		g_wepcam.has_during_time = FALSE;

		//during time coordinate
		g_wepcam.during_icon_x = -1;
		g_wepcam.during_icon_y =  -1;

		g_wepcam.during_time_x = g_wepcam.local_offset_x+g_wepcam.local_display_width+\
			VDO_PORTRAIT_OUTLINE_THICKNESS+VDO_INFO_ICON_STR_GAP;
		g_wepcam.during_time_y = g_wepcam.local_offset_y + (preview_right_middle_y - g_wepcam.local_offset_y)/2 - str_h / 2 + 5 + 2;

		//signal coordinate
		gdi_image_get_dimension(get_image(DEV_GetImgId(KK_IMG_VIEWCHAT_V2_GPRS_SIG44)), &img_w, &img_h);
		g_wepcam.sig_level_x=  g_wepcam.during_time_x;
		g_wepcam.sig_level_y = preview_right_middle_y + img_h / 2 -5;
	}
	else
	{
		g_wepcam.has_during_time = FALSE;
		g_wepcam.has_during_time_only_text = FALSE;
	}

	//re-init during time and signal coordinate if none camera
	if(DEV_IsNoneCamera() && (g_wepcam.has_buddy_name)) //BN10117
	{
		g_wepcam.has_during_time_only_text = FALSE;
		g_wepcam.has_during_time = TRUE;
		
		g_wepcam.during_icon_x = g_wepcam.buddy_icon_x;
		g_wepcam.during_icon_y = g_wepcam.during_time_y;

		g_wepcam.during_time_x = g_wepcam.buddy_name_x;
		//g_wepcam.during_time_y = g_wepcam.buddy_name_y;
		
		g_wepcam.sig_level_x=  g_wepcam.buddy_icon_x;
	}


#ifdef WEP_DEBUG
	WM_DebugPrint("->WepCamInitParam: c_w=%d, c_h=%d; l_w=%d, l_h=%d;r_w=%d, r_h=%d, t_w=%d, t_h=%d", \
		g_wepcam.capture_width, g_wepcam.capture_height, \
		g_wepcam.local_display_width, g_wepcam.local_display_height, \
		g_wepcam.remote_width, g_wepcam.remote_height, \
		g_wepcam.transfer_width, g_wepcam.transfer_height);
#endif

		
}


void WepGetCameraSettings( uint16_t *preview_w,  uint16_t *preview_h, uint16_t *img_w, uint16_t *img_h )
{
	*preview_w = g_wepcam.capture_width;
	*preview_h = g_wepcam.capture_height;
	
	*img_w = g_wepcam.capture_width;
	*img_h = g_wepcam.capture_height;
}



void WepUpdateCat57VideoCoordinate( int16_t *s16Y,  int16_t *s16Height )
{


	if( g_dm_data.S32CatId == MMI_CATEGORY57_WEP_VIDEO_CHAT_ID )
	{
		*s16Y +=  g_wepcam.content_y;//g_wepcam.outline_height;//bug 10109
		*s16Height -= g_wepcam.content_y;//g_wepcam.outline_height;
	}
}


#if 0
//bruce091210
void WepDrawVideoPortraitOutLine(void)
{
	//color_t bg_color = {226, 152, 29, 100};//{230, 180, 250, 100};//{58, 94, 235, 100};//{194, 216, 254, 100}; 
	//color board_bg_color = {230, 180, 250, 100};//{251, 13, 221, 100}; //{98, 196, 186, 100};//{74, 127, 181, 100};

//	gdi_color cust_start_color = 111;//gdi_act_color_from_rgb(255, 80, 133, 187);
//	gdi_color cust_end_color = 111;//gdi_act_color_from_rgb(255, 24, 37, 48);

	gdi_layer_push_and_set_active(g_wepcam.g_wepcam_base_layer);
#if 1
	//outer line
	gui_draw_rectangle(g_wepcam.vdoarea_outline_x, g_wepcam.vdoarea_outline_y, \
		UI_device_width-1, g_wepcam.content_y-1, WEP_COLOR_PORTRAIT_BOARD);

	//local portrait boardline
	if(!DEV_IsNoneCamera())//BN10117
	{
		gui_draw_rectangle(g_wepcam.local_outline_x, g_wepcam.local_outline_y, \
			g_wepcam.local_offset_x + g_wepcam.local_display_width, g_wepcam.local_offset_y+ g_wepcam.local_display_height, \
			WEP_COLOR_PORTRAIT_BOARD);
			
		gui_draw_rectangle(g_wepcam.local_outline_x+1, g_wepcam.local_outline_y + 1, \
			g_wepcam.local_offset_x + g_wepcam.local_display_width + 1, g_wepcam.local_offset_y + g_wepcam.local_display_height + 1, \
			WEP_COLOR_PORTRAIT_BOARD);
	}
	
	//remote portrait boardline
	gui_draw_rectangle(g_wepcam.remote_outline_x, g_wepcam.remote_outline_y, \
		g_wepcam.remote_offset_x + g_wepcam.remote_width, g_wepcam.remote_offset_y + g_wepcam.remote_height, \
		WEP_COLOR_PORTRAIT_BOARD);
	gui_draw_rectangle(g_wepcam.remote_outline_x+1, g_wepcam.remote_outline_y+1, \
		g_wepcam.remote_offset_x + g_wepcam.remote_width + 1, g_wepcam.remote_offset_y + g_wepcam.remote_height + 1, \
		WEP_COLOR_PORTRAIT_BOARD);
	
#endif


	gdi_layer_pop_and_restore_active();

	WepUpdateRoomInfo(); //bruce090930
#if 0
	gdi_layer_blt( GDI_LCD->blt_handle[0],/*0,*/
		g_wepcam.g_wepcam_base_layer,			/* layer 0 */
		//0,////g_wepcam.g_wepcam_preview_layer,		/* layer 1 */
		//g_wepcam.g_wepcam_image_layer,			/* layer 2 */
		0,									/* layer 3 */
		g_wepcam.g_wepcam_preview_layer,
		0,								/* x1 */
		offset_y,									/* y1 */
		UI_device_width,								/* x2 */
		offset_y+WEP_COMMON_VIDEO_BG_HEIGHT-1);	
#endif

}
#endif


extern kal_uint32 app_getcurrtime(void);

void WepUpdateRoomInfo(void)
{
/*	S32 img_x, img_y, img_w, img_h;
	S32 str_x, str_y, str_w, str_h;
	S8 ts[32];
	UI_character_type s[64];
	U32 s_len = 0;
	UI_time t;
	U32 diff_sec = 0;
	U32 during_hour = 0;
	U32 during_min = 0;
	U32 during_sec = 0;*/
#if 0
	color_t bg_color = {0, 0, 0, 255}; //{226, 152, 29, 100};

//	gdi_color cust_start_color = gdi_act_color_from_rgb(255, 80, 133, 187);
//	gdi_color cust_end_color = gdi_act_color_from_rgb(255, 24, 37, 48);
	
	U8 board_widht = 2;
	U8 board_gap= 4;
	S32 offset_x = 0;
	uint8_t signal_level = 0;

  	///WEPAppData * pWep = g_Builder->pWep;
	///UI * pUI = (UI *)(pWep->pIUI);

	if(g_wepcam.isWepCamWindow == KAL_FALSE)
		return;

	//update the bg of info window
/*        offset_x = WEP_VIDEO_INFO_ICON_OFFSETX + WEP_PREVIEW_ZOOMIN_WIDTH + WEP_VIDEOCHAT_BOARD_GAP;
        offset_y = WEP_VIDEOCHAT_TITLE_HEIGHT + WEP_VIDEOCHAT_BOARD_WIDTH;
	    gdi_draw_gradient_rect(
        offset_x,
        offset_y,
        UI_device_width-1,
        offset_y+WEP_COMMON_VIDEO_BG_HEIGHT-WEP_VIDEOCHAT_BOARD_WIDTH * 2 - 1,
        cust_start_color,
        cust_end_color,
        0,
        0,
        GDI_GRADIENT_RECT_VER);
*/	

	gdi_layer_push_and_set_active(g_wepcam.g_wepcam_base_layer);

//	gui_set_font(&MMI_medium_font);
//	gui_set_text_color(WEP_COLOR_TEXT);
	
	if(g_wepcam.has_buddy_name)
	{
		//check the string height
		sprintf((S8*) ts, "%d", 123);
		mmi_asc_to_ucs2((S8*) s, ts);
		gui_measure_string(s, &str_w, &str_h);

		//draw title
	///	gdi_image_get_dimension(get_image(DEV_GetImgId(KK_IMG_VIEWCHAT_V2_BUDDY_NAME)), &img_w, &img_h);
//		sprintf((S8*) ts, "%d", 12345678);
//		mmi_asc_to_ucs2((S8*) s, ts);
//		gui_measure_string(s, &str_w, &str_h);
	///	gui_measure_string(pUI->screenTitle, &str_w, &str_h);

		UI_GetCurrScreenTitle((uint16_t *)s, g_wepcam.buddy_name_max);//32
		
//		gui_measure_string(ts, &str_w, &str_h);

		//s_len = UTL_UniStrLen(pLui->subItem[pLui->subItemNum].strLabel);
		//DEV_NetUniByte2LocStr(s, pLui->subItem[pLui->subItemNum].strLabel, s_len);

	//	gui_measure_string(s, &str_w, &str_h);

		if(str_w > 0)
		{
			gdi_image_draw_id(g_wepcam.buddy_icon_x, g_wepcam.buddy_icon_y, DEV_GetImgId(KK_IMG_VIEWCHAT_V2_BUDDY_NAME)); 
			gui_move_text_cursor(g_wepcam.buddy_name_x, g_wepcam.buddy_name_y+2);
		//	gui_print_text(s);
		//	gui_print_text(pUI->screenTitle);
			gui_print_text(s);
		}
	}
	
	//gdi_layer_set_source_key(TRUE, GDI_COLOR_BLUE);
 	//gdi_layer_clear(GDI_COLOR_BLUE);

//	diff_sec = applib_dt_get_rtc_time()/*app_getcurrtime() */- g_wep_start_time;
	//signal strength
	signal_level = LUI_GetCacheSignalStrength();
	if(signal_level<0 || signal_level > SIG_LEVEL_MAX)
		signal_level = 0;		
	
	if(g_wepcam.has_during_time || g_wepcam.has_during_time_only_text)
	{
		//during time icon if need
		if(g_wepcam.has_during_time && !(g_wepcam.has_during_time_only_text))
			gdi_image_draw_id(g_wepcam.during_icon_x, g_wepcam.during_icon_y, DEV_GetImgId(KK_IMG_VIEWCHAT_V2_DURING_TIME)); 

		//during time string
		gdi_image_get_dimension(get_image(DEV_GetImgId(KK_IMG_VIEWCHAT_V2_DURING_TIME)), &img_w, &img_h);
		//GetDateTime(&t);
		//sprintf((S8*) ts, "%02d:%02d:%02d", t.nHour, t.nMin, t.nSec);
		//sprintf((S8*) ts, "%08d", msec);
		during_hour = diff_sec / 3600;
		during_min = (diff_sec - during_hour * 3600) / 60;
		during_sec = diff_sec - during_hour * 3600 -  during_min * 60;
		sprintf((S8*) ts, "%02d:%02d", during_min, during_sec);
		mmi_asc_to_ucs2((S8*) s, ts);
		gui_measure_string(s, &str_w, &str_h);
		gui_move_text_cursor(g_wepcam.during_time_x, g_wepcam.during_time_y);
		gdi_draw_solid_rect(g_wepcam.during_time_x, g_wepcam.during_time_y, \
			UI_device_width-VDO_OUTLINE_AREA_PORTRAIT_GAP-1, g_wepcam.during_time_y+str_h, GDI_COLOR_TRANSPARENT);
		gui_print_text(s);
		

		//signal icon
		gdi_image_get_dimension(get_image(DEV_GetImgId(KK_IMG_VIEWCHAT_V2_GPRS_SIG44)), &img_w, &img_h);
		gdi_draw_solid_rect(g_wepcam.sig_level_x, g_wepcam.sig_level_y, \
			g_wepcam.sig_level_x+img_w, g_wepcam.sig_level_y+img_h, GDI_COLOR_TRANSPARENT);
		
		gdi_image_draw_id(g_wepcam.sig_level_x, g_wepcam.sig_level_y, (DEV_GetImgId(KK_IMG_VIEWCHAT_V2_GPRS_SIG00)+signal_level)); 
	}

	//remote area info: waiting animation before incoming first frame image
	if(signal_level==0 && diff_sec < 2)
	{
		if (g_wepcam.wait_video_anim != GDI_NULL_HANDLE)
		{
			gdi_image_stop_animation(g_wepcam.wait_video_anim);
			g_wepcam.wait_video_anim = GDI_NULL_HANDLE;
		}

		if(g_wepcam.remote_width >= 80)
		{
			gdi_image_get_dimension(get_image(DEV_GetImgId(KK_IMG_VIEWCHAT_V2_CHAT_WAITING)), &img_w, &img_h);
		
			gdi_image_draw_animation_id(
				g_wepcam.remote_offset_x+(g_wepcam.remote_width-img_w) / 2, 
				g_wepcam.remote_offset_y + (g_wepcam.remote_height-img_h) / 2, 
				DEV_GetImgId(KK_IMG_VIEWCHAT_V2_CHAT_WAITING),
				&g_wepcam.wait_video_anim);
		}
		else
		{
			gdi_image_get_dimension(get_image(DEV_GetImgId(KK_IMG_VIEWCHAT_V2_CHAT_WAITING_MINI)), &img_w, &img_h);

			gdi_image_draw_animation_id(
				g_wepcam.remote_offset_x+(g_wepcam.remote_width-img_w) / 2, 
				g_wepcam.remote_offset_y + (g_wepcam.remote_height-img_h) / 2, 
				DEV_GetImgId(KK_IMG_VIEWCHAT_V2_CHAT_WAITING_MINI),
				&g_wepcam.wait_video_anim);
		}
	}

	//remote area info: waiting animation before incoming first frame image
	if(signal_level > 0)
	{	
		if (g_wepcam.wait_video_anim != GDI_NULL_HANDLE)
		{
			gdi_image_stop_animation(g_wepcam.wait_video_anim);
			g_wepcam.wait_video_anim = GDI_NULL_HANDLE;
		}
	}
	//online user number
#if 0
	gdi_image_get_dimension(get_image(DEV_GetImgId(KK_IMG_VIEWCHAT_V2_GPRS_SIG00)), &img_w, &img_h);
	img_y += img_h + WEP_VIDEOCHAT_BOARD_GAP;	
	gdi_image_draw_id(img_x,img_y, DEV_GetImgId(KK_IMG_VIEWCHAT_V2_BUDDY_NAME)); 

	gdi_image_get_dimension(get_image(DEV_GetImgId(KK_IMG_VIEWCHAT_V2_BUDDY_NAME)), &img_w, &img_h);
	sprintf((S8*) ts, "%d", 288);
	mmi_asc_to_ucs2((S8*) s, ts);
	gui_measure_string(s, &str_w, &str_h);
	gui_move_text_cursor(img_x+img_w+WEP_VIDEOCHAT_BOARD_GAP, img_y+img_h/2-str_h/2+2);
	gui_print_text(s);
#endif

	gdi_layer_pop_and_restore_active();
#endif
}

//scan source line data from up to down
#if 0
void WepRGB16ViewFinder(uint8_t *input, uint32_t in_w, uint32_t in_h, uint8_t *output, uint32_t out_w,uint32_t out_h)
{
	uint32_t i;
	uint32_t offset_x=0, offset_y=0;
	uint16_t line_len = out_w * 2;
	
	uint8_t *output_ptr = output;
	uint8_t *input_ptr = input;
	uint8_t *pin, *pout;

#ifdef WEP_DEBUG
	WM_DebugPrint("->RGB16ViewFinder: in_w=%d, in_h=%d; out_w=%d, out_h=%d", in_w, in_h, out_w, out_h);
#endif

	if(out_w > in_w || out_h > in_h || !input || !output)
		return;

	offset_x = (in_w - out_w) / 2;
	offset_y = (in_h - out_h) / 2;
	
	for(i=0;i<in_h;i++)
	{
		if((i >= offset_y) && (i < (offset_y+out_h)))
		{
			pin = input_ptr + (i * in_w + offset_x) * 2;
			pout = output_ptr + (i - offset_y) * out_w * 2;

			DEV_MemCpy(pout , pin, line_len);
		}
	}
}
#endif
//scan source line data from up to down
void WepRGB16ViewFinder2BaseLayer(uint8_t *input, uint32_t in_w, uint32_t in_h, uint8_t *output, uint32_t out_w,uint32_t out_h)
{
	uint32_t i ;
	uint32_t offset_x=0, offset_y=0;
	uint16_t line_len = out_w * 2;
	
	uint8_t *output_ptr = output;
	uint8_t *input_ptr = input;
	uint8_t *pin, *pout;

#ifdef WEP_DEBUG
	WM_DebugPrint("->RGB16ViewFinder: in_w=%d, in_h=%d; out_w=%d, out_h=%d", in_w, in_h, out_w, out_h);
#endif

	if(out_w > in_w || out_h > in_h || !input || !output)
		return;

	offset_x = (in_w - out_w) / 2;
	offset_y = (in_h - out_h) / 2;
	
	for(i=0;i<in_h;i++)
	{
		if((i >= offset_y) && (i < (offset_y+out_h)))
		{
			pin = input_ptr + (i * in_w + offset_x) * 2;
			//pout = output_ptr + (i - offset_y) * out_w * 2;
			pout = output_ptr + (i - offset_y) * UI_device_width * 2;

			DEV_MemCpy(pout , pin, line_len);
		}
	}
}
#endif


//bruce091210 for modis simulator capture from USB camera
#if defined(WIN32) && defined(__KK_UI_V2__)
U32  g_PreviewFrameIdx = 0;

//scan source line data from down to up when win32 sensor
void WepWin32RGB24To16(uint8_t *input,uint16_t *output0,uint32_t width,uint32_t height)
{
	uint32_t i,j;
	uint8_t r,g,b;	
	uint8_t *rp,*gp,*bp,*pin;

	uint16_t* output=output0;
	uint16_t output_r=0, output_g=0, output_b=0;

	rp=input;
	gp=input+1;
	bp=input+2;

	for(i=0;i<height;i++)
	{
		//output = (height-i-1) * width;
		pin = input + (height-i-1) * width * 3;
		rp=pin;
		gp=pin+1;
		bp=pin+2;

		for(j=0;j<width;j++)
		{
			r=*rp;
			g=*gp;
			b=*bp;
			*output++ =(((b&0x00f8)<<8)|((g&0x00fc)<<3)|(r&0xf8)>>3);		 
                                
			rp+=3;
			gp+=3;
			bp+=3;
		}
	}
}

//scan source line data from down to up when win32 sensor
void WepWin32_RGB24To16ViewFinder(uint8_t *input, uint32_t in_w, uint32_t in_h, uint16_t *output, uint32_t out_w,uint32_t out_h)
{
	uint32_t i,j;
	uint8_t r,g,b;	
	uint8_t *rp,*gp,*bp,*pin;
	uint32_t offset_x=0, offset_y=0;
	
	uint16_t *output_ptr = output;
	uint8_t *input_ptr = input;
	uint16_t output_r=0, output_g=0, output_b=0;

	if(out_w > in_w || out_h > in_h || !input || !output)
		return;

	offset_x = (in_w - out_w) / 2;
	offset_y = (in_h - out_h) / 2;
	
	rp=input_ptr;
	gp=input_ptr+1;
	bp=input_ptr+2;

	for(i=0;i<in_h;i++)
	{
		//output = (height-i-1) * width;
		if((i >= offset_y) && (i < (in_h-offset_y)))
		{
			pin = input_ptr + ((in_h-i-1) * in_w + offset_x)* 3;
			rp=pin;
			gp=pin+1;
			bp=pin+2;

			for(j=0;j<out_w;j++)
			{
				r=*rp;
				g=*gp;
				b=*bp;
				*output_ptr++ =(((b&0x00f8)<<8)|((g&0x00fc)<<3)|(r&0xf8)>>3);		 
	                                
				rp+=3;
				gp+=3;
				bp+=3;
			}
		}
	}
}

void GetWebCamPreviewFrame(uint8_t  *p_Data,kal_uint32 dataLen)
{
    S32 image_width;
    S32 image_height;
    PU8 image_ptr,RGBdata_ptr;
    kal_uint32 RGBdata_len=0;
	GDI_RESULT gdi_ret; //bruce090608


	if(p_Data==NULL || dataLen==0)
		return 0;

    if(g_wepcam.isWepCamWindow == KAL_FALSE)
		return 0;

//bruce090608 b
#if 0 //def WEP_DEBUG		
	{
	unsigned char *pBuf = (unsigned char *)(p_Data);

	WM_DebugPrint("GetWebCamPreviewFrame: Frame idx =%d, [image layer] width =%d, height = %d", \
		g_PreviewFrameIdx, g_wepcam.image_width, g_wepcam.image_height);
	WM_DebugPrint( "GetWebCamPreviewFrame: p_Data []: 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x ", \
		*pBuf, *(pBuf+1), *(pBuf+2), *(pBuf+3), *(pBuf+4), *(pBuf+5), *(pBuf+6), *(pBuf+7), *(pBuf+8), *(pBuf+9) );
	}
#endif	
		
	g_PreviewFrameIdx++;

	//if(g_PreviewFrameIdx % 20 == 0) //skip frames
	if(g_PreviewFrameIdx % 5 == 0) //skip frames
	{
///	WepWin32RGB24To16(p_Data, g_wepcam.preview_layer_buf_ptr, g_wepcam.preview_width, g_wepcam.preview_height);
///	memcpy(g_wepcam.preview_layer_buf_ptr, p_Data, g_wepcam.preview_layer_buf_size);
		//DrawPreviewWebCamImage(g_wepcam.preview_layer_buf_ptr, g_wepcam.preview_layer_buf_size);
		
//bruce090909
#if 0
	WepWin32_RGB24To16ViewFinder(p_Data, g_wepcam.preview_width, g_wepcam.preview_height, \
			g_wepcam.preview_layer_buf_ptr1, g_wepcam.local_display_width, g_wepcam.local_display_height);

	DEV_MemCpy(g_wepcam.preview_layer_buf_ptr, g_wepcam.preview_layer_buf_ptr1, \
		g_wepcam.local_display_width * g_wepcam.local_display_height * 2);
#else
	WepWin32_RGB24To16ViewFinder(p_Data, g_wepcam.capture_width, g_wepcam.capture_height, \
			g_wepcam.preview_layer_buf_ptr1, g_wepcam.capture_width, g_wepcam.capture_height);



#endif
	
#if 1//def WIN32
//		DEV_MemCpy(g_wepcam.preview_layer_buf_ptr1, g_wepcam.preview_layer_buf_ptr, g_wepcam.preview_layer_buf_size);
#elif 1
{
		S32 i,j;
		S32 offset_x = 0;//(g_wepcam.capture_width - g_wepcam.local_display_width) / 2;
		S32 offset_y = 0;//(g_wepcam.capture_height- g_wepcam.local_display_height) / 2;
		U8 *pSrcBaseOffset = NULL;
		U8 *pDstBaseOffset = NULL;
		U16 line_len = g_wepcam.local_display_width*2;
		
 		for (j = 0; j < 96; j++)
		{
			pSrcBaseOffset = (U8 *) (g_wepcam.preview_layer_buf_ptr + ((j * g_wepcam.capture_width + offset_x) ));
			pDstBaseOffset = (U8 *) (g_wepcam.preview_layer_buf_ptr1 + (((j+ g_wepcam.local_offset_y+offset_y) * UI_device_width + g_wepcam.local_offset_x) ));

			DEV_MemCpy(pDstBaseOffset ,pSrcBaseOffset, line_len);			
		}

}
#else
{
		S32 i,j;
		S32 offset_x = (g_wepcam.capture_width - g_wepcam.local_display_width) / 2;
		S32 offset_y = (g_wepcam.capture_height- g_wepcam.local_display_height) / 2;
		
 		for (j = 0; j < g_wepcam.local_display_height; j++)
		{
			for (i = 0; i < g_wepcam.local_display_width; i++)
			{
				*(U16*) (g_wepcam.preview_layer_buf_ptr1 + (((j+ g_wepcam.local_offset_y+offset_y) * UI_device_width+ i + g_wepcam.local_offset_x) << 1)) = \
					*(U16*) (g_wepcam.preview_layer_buf_ptr + ((j * g_wepcam.capture_width + offset_x + i) << 1));
			}
		}

}

#endif
	}

	//wep_video_encode_data_req(NULL, 0, 0);

	if(g_PreviewFrameIdx >= 0xFFFFFFFF)
		g_PreviewFrameIdx = 0;
	return;
}

#endif /* WIN32 */
#endif

