#ifndef _WEPCAM_H
#define _WEPCAM_H

#include "mmi_features.h"
#include "interface.h"
#include "mmi_features.h"
//#include "WepZone.h"
#if (KK_MTKXXXX_SWSUBVER<KK_MTKXXXX_V09A52)	
#include "eventsgprot.h"
#endif
#include "historygprot.h"

#include "commonscreens.h"
#include "todolistdef.h"
#include "todolistenum.h"

#include "custmenures.h"
#include "wgui_categories.h"
#include "globaldefs.h"
#include "wgui_categories_inputs.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "settingprofile.h"
#include "lcd_if.h"
#include "lcd_sw.h"
#include "device.h"
#include "debuginitdef.h"
#include "wgui.h"                       /* for GetDateTime() */

typedef struct
{
	gdi_handle  g_wepcam_base_layer;
	gdi_handle  g_wepcam_preview_layer;
	gdi_handle  g_wepcam_image_layer;
    	PU8 capture_buf_ptr;
    	U32 capture_size;
    	PU8 preview_layer_buf_ptr;  // preview
	PU8 preview_layer_buf_ptr1; // pic get
    	U32 preview_layer_buf_size;
	PU8 image_layer_buf_ptr;
	U32 image_layer_buf_size;
	kal_uint8 isWepCamWindow;
	kal_uint8  isencoding;
	kal_uint8 isGetImg;/*pangpeng add for solving the bad image problem while quickly moving */
	kal_uint8  isPreviewing;


	kal_uint16 preview_offset_x;
	kal_uint16 preview_offset_y;
//bruce0912010
#ifndef __KK_UI_V2__
	kal_uint16 preview_width;
	kal_uint16 preview_height;
#endif

	kal_uint16 image_offset_x;
	kal_uint16 image_offset_y;
	kal_uint16 image_width;
	kal_uint16 image_height;

	kal_uint16 transfer_width;
	kal_uint16 transfer_height;

 
	PU8 resample_buf_src_ptr; 
	PU8 resample_buf_des_ptr;
	kal_uint16 resample_buf_size;
	kal_uint16 resample_width;
	kal_uint16 resample_height;
//#endif
//#endif

//bruce0912010
#ifdef __KK_UI_V2__
	kal_uint16 outline_height;

	//local portrait
	int16_t local_display_width;
	int16_t local_display_height;
	int16_t local_virgin;
	int16_t local_offset_x;
	int16_t local_offset_y;

	//remote portrait
	int16_t remote_width;
	int16_t remote_height;
	int16_t remote_virgin;
	int16_t remote_offset_x;
	int16_t remote_offset_y;

	//capture portrait
	int16_t capture_width;
	int16_t capture_height;

	//int16_t transfer_width;
	//int16_t transfer_height;

	//content
	int16_t content_y;

	//info
	bool_t has_buddy_name;
	bool_t has_during_time;
	bool_t has_during_time_only_text;
	int16_t buddy_icon_x;
	int16_t buddy_icon_y;
	int16_t buddy_name_x;
	int16_t buddy_name_y;
	int16_t buddy_name_max;	
	int16_t during_icon_x;
	int16_t during_icon_y;
	int16_t during_time_x;
	int16_t during_time_y;
	int16_t sig_level_x;
	int16_t sig_level_y;
	int16_t online_icon_x;
	int16_t online_icon_y;
	int16_t online_num_x;
	int16_t online_num_y;

	//outline
	int16_t vdoarea_outline_x;
	int16_t vdoarea_outline_y;
	
	int16_t local_outline_x;
	int16_t local_outline_y;

	int16_t remote_outline_x;
	int16_t remote_outline_y;

	//wait
	gdi_handle wait_video_anim;

#endif

}wepcam_struct;


//bruce0912010
#ifdef __KK_UI_V2__

extern int32_t KK_title_height;
extern int32_t KK_title_width;
extern int32_t KK_title_x;
extern int32_t KK_title_y;

extern int32_t KK_video_title_height;
extern int32_t KK_video_title_width;
extern int32_t KK_video_title_x;
extern int32_t KK_video_title_y;

extern int32_t KK_content_x;
extern int32_t KK_content_y;
extern int32_t KK_content_width;
extern int32_t KK_content_height;

extern int32_t KK_video_content_x;
extern int32_t KK_video_content_y;
extern int32_t KK_video_content_width;
extern int32_t KK_video_content_height;

#else /* !__KK_UI_V2__ */

#define WEP_IMAGE_WIDTH 	96	 //MMI_WEP_IMAGE_HEIGHT
#define WEP_IMAGE_HEIGHT 	96	 //MMI_WEP_IMAGE_HEIGHT
#define WEP_IMAGE_VIRGIN 	10
#define WEP_IMAGE_OFFSETX   (UI_device_width/2 +WEP_IMAGE_VIRGIN)
#define WEP_IMAGE_OFFSETY MMI_COMMON_TITLE_Y// + MMI_COMMON_TITLE_HEIGHT
//#define WEP_IMAGE_OFFSETX (UI_device_width -WEP_IMAGE_WIDTH -16) // czm test
//#define WEP_IMAGE_OFFSETY MMI_COMMON_TITLE_Y + MMI_COMMON_TITLE_HEIGHT

#define WEP_TRANSFER_WIDTH   96
#define WEP_TRANSFER_HEIGHT  96

#define WEP_PREVIEW_WIDTH  96
#define WEP_PREVIEW_HEIGHT 96
#define WEP_PREVIEW_VIRGIN 10
#define WEP_PREVIEW_OFFSETX  WEP_IMAGE_OFFSETX - WEP_PREVIEW_VIRGIN - WEP_PREVIEW_WIDTH
#define WEP_PREVIEW_OFFSETY  MMI_COMMON_TITLE_Y// + MMI_COMMON_TITLE_HEIGHT
//#define WEP_PREVIEW_OFFSETX (UI_device_width -WEP_IMAGE_WIDTH -16) // czm test
//#define WEP_PREVIEW_OFFSETY  MMI_COMMON_TITLE_Y + MMI_COMMON_TITLE_HEIGHT+WEP_IMAGE_HEIGHT +16

#endif /*__KK_UI_V2__ */



extern wepcam_struct  g_wepcam;

extern void  WepVideoChat(void);
extern void  WEP_CameraPhotoGet(void);
extern bool_t WepCamStart(void);
extern void WepCamStop(void);
extern void WepCamPause(void);

extern kal_int32 CameraPicGet (ILD * pILd, kal_uint8 buffer[], kal_int32 maxSize);
extern kal_uint8 DrawWepVideoImage(kal_uint8 * p_Data,kal_uint32 dataLen);
extern kal_int32  LocalPicGet(IUI * pIUI,kal_uint8 *picData, int maxSize, PFN_SETPICCB pfn_Wep_set_picdata);
extern 	void  VideoChatCameraPreview(void);
extern kal_int8 VideoChatStart(void);
extern kal_int8 VideoChatStop(void);
extern int32_t cameraIntervalGet(void);

extern bool_t WepCamStatusGet();


extern void WepCamInit(void);
extern void WepCamExit(void);

extern void WM_PreviewCoordinateGet(int16_t * preview_offset_x, int16_t * preview_offset_y, int16_t * preview_width, int16_t * preview_height);
extern void WM_ImageCoordinateGet(int16_t * image_offset_x, int16_t * image_offset_y, int16_t * image_width, int16_t * image_height);
extern void WM_TransferSizeGet(int16_t * transfer_width, int16_t * transfer_height);

#endif
