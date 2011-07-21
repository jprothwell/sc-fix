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
 *	 CameraApp.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Camera Applications v0.3
 *
 * Author:
 * -------
 * -------
 *                      
 *============================================================================
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
  *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/****************************************************************************
* Include Files                                                                
*****************************************************************************/
#include "mmi_features.h"
#ifdef __MMI_CAMERA__

#define __NEWSIMULATOR 


#include "stdc.h"
#include "l4dr1.h"
#include "globaldefs.h"

#include "lcd_if.h"							/* lcd layer enable flag */
#include "lcd_sw_rnd.h"						/* for lcd size */
//#define __MMI_CAMERA_ONLY__ //wufasong deleted 2007.01.08
#include "mmi_features_camera.h"			/* features */

#include "nvramtype.h"						/* nvram data type */
#include "nvramprot.h"						/* nvram access fucntions */	
#include "nvramenum.h"						/* nvram id define */

#ifdef MMI_ON_HARDWARE_P
	#include "nvram_data_items.h" 		/* id to store file path */
#endif

/* framework related headers */
#include "custmenures.h"
#include "globalconstants.h"				/* key id define */
#include "globaldefs.h"						/* golbal image and string id. */
#include "gui.h"								/* gui functions */
#include "wgui_categories.h"
#include "historygprot.h"					/* screen history */
#include "unicodexdcl.h"					/* unicode transform functions */
#include "eventsgprot.h"		
#include "settingdefs.h"					/* st_notifyduration define */
#include "settingprofile.h"				/* success_tone define */
#include "wgui.h"								/* for getdatetime() */
#include "commonscreens.h"					/* displaypopup() */
#include "gpioinc.h"							/* led */
#include "debuginitdef.h"					/* mmi_assert */
#include "settingprofile.h"
#include "wgui_categories_multimedia.h"
#include "wgui_categories_popup.h"
#include "wgui_status_icons.h"
#include "wgui_softkeys.h"
#include "sublcdhistorygprot.h"
#include "fat_fs.h"
#include "filemanagerdef.h"
#include "filemanagergprot.h"				/* file path */
#include "filemgr.h"
#include "mmiapi_enum.h"					/* stop led */
#include "mainmenudef.h"					/* multimedia icon */
#include "profilegprots.h"					/* playrequestedtone */
#include "keybrd.h"							/* keypad tone related interface */

/* gdi, mid, camera app headers */
#include "gdi_include.h"					/* graphic library */
#include "gdi_image_jpeg.h"				/* jpeg decoder */
#include	"mdi_datatype.h"
#include "mdi_camera.h"
#include "mdi_audio.h"

#ifdef __CAMERA_FEATURE_TOUCH_SCREEN__
	#include "wgui_touch_screen.h"
#endif /* __CAMERA_FEATURE_TOUCH_SCREEN__ */
#include "vdoplystrenum.h"
#include "resource_camera_skins.h"		/* ui custom config */
#include "cameraapp.h"						/* camera app data */
#include "cameramenu.h"						/* function declare */
#include "cameraresdefs.h"					/* resource id def */

#ifdef __MMI_TVOUT__
	#include "mdi_tv.h"						/* mdi tvout */
	#include "wallpaperdefs.h"				/* some header for phonsetup.h */
	#include "downloaddefs.h"				/* some header for phonsetup.h */
	#include "phonesetup.h"					/* tvout display style */
#endif /* __MMI_TVOUT__ */


	#include "gui_setting.h"
	#include "wgui.h"
#if defined(__CAMERA_OSD_HORIZONTAL__) || defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	#include "screenrotationgprot.h"
#endif
#include "mmi_trace.h"
#include "multimemmng.h"

#include "phonebooktypes.h"
#include "simdetectiondef.h"
#include "simdetectiongexdcl.h"
#include "lcd_sw_inc.h"
#include "di.h"
/*WeiD Add Start For 6314 Ver: TBM780  on 2007-8-17 15:6 */
#include "settingsgdcl.h"
/*WeiD Add End  For 6314 Ver: TBM780  on 2007-8-17 15:6 */
//#include "smsstruct.h"

#include "globalscrenum.h"

#ifdef JATAAYU_SUPPORT
#ifdef JATAAYU_SUPPORT_MMS 
	extern INT8 jMMS_IsMMSAppOrDownloadActive(void);
#endif
#ifdef JATAAYU_SUPPORT_WAP
extern  INT8 jMMS_IsWapConnected (void);
#endif
#endif
extern void mmi_phb_entry_not_ready(void);
extern mmi_phb_context_struct g_phb_cntx;
/*WeiD Add Start For 6314 Ver: TBM780  on 2007-8-17 15:7 */
extern SecuritySetupContext g_SecuritySetupContext;
/*WeiD Add End  For 6314 Ver: TBM780  on 2007-8-17 15:7 */
/*WeiD Add Start For 6269 Ver: TBM780  on 2007-8-20 14:31 */
extern FuncPtr currKeyFuncPtrs[MAX_KEYS][MAX_KEY_TYPE];
/*WeiD Add End  For 6269 Ver: TBM780  on 2007-8-20 14:31 */
#if defined(__MMI_MAINLCD_220X176__)
void mmi_revert_nokia_popup_dialog_component();
#endif

void mmi_camera_preview_wait_key7_hdlr(void);

extern void set_mmc_camera_preview();
extern void set_multicaputure(int i);
extern   UINT32 MCI_CamPlayBack ();
extern UINT32 MCI_CamCancelLastSave(VOID);
#if (CSW_EXTENDED_API_CAMERA == 1)
extern INT32  MCI_CamSavePhoto (UINT8*);
#else
extern INT32  MCI_CamSavePhoto (INT32 filehandle);
#endif /* CSW_EXTENDED_API_CAMERA */

#ifdef __MMI_STOP_SIGNAL_REPORT__
extern void NetWorkSignalStopReport(U8 nStart);
#endif
//#include "adp_filesystem.h" //wufasong added 2007.01.08
/*Jinzh Add Start For 6225 Ver: TBM780  on 2007-8-14 10:2 */
#ifdef __FF_AUDIO_SET__
extern BOOL cameraAppOnOff;
#endif
#if !(defined(MMI_ON_HARDWARE_P))
kal_int32 media_cam_save_photo(UINT32 fhd )
{
}
#endif

extern void mmi_show_qq_select_confirm(void (*default_entry) (void));

/*Jinzh Add End  For 6225 Ver: TBM780  on 2007-8-14 10:2 */
/***************************************************************************** 
* Define
*****************************************************************************/
#define DUMMY_COMMAND 						(0)
#define HINT_POPUP_FADE_TIME				(1200)
#define TOGGLE_POPUP_FADE_TIME			(800)
#define DONE_POPUP_FADE_TIME				(1000)
#define CAMERA_OSD_BUFFER_SIZE 			(((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3)*2) /* double buffer */
#define CAMERA_TILE_RESULT_BUFFER_SIZE	(((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3))
#define CAMERA_ERR_CORRUPT_FAT			(-1)
#define CAMERA_ERR_PHONE_DISK_FULL		(-2)
#define CAMERA_ERR_CARD_DISK_FULL		(-3)
#define CAMERA_ERR_WRITE_PROTECT			(-4)
#define CAMERA_ERR_ROOT_DIR_FULL			(-5)

#define POPLIST_LIST_SPACING				(1)

#if 1
	//#define CAMEAR_BG_TRASN_COLOR			(0x0821) //2007-7-3 fengzb
	#define CAMEAR_BG_TRASN_COLOR			(0x001f) 
#else
	#define CAMEAR_BG_TRASN_COLOR			(GDI_COLOR_TRANSPARENT)
#endif 

/***************************************************************************** 
* Typedef 
*****************************************************************************/


/* wufasong added 2007.01.22 */
const U8 CAMERA_STORAGE_FILEPATH_PHONE[]       = {0x50,0x00,0x68,0x00,0x6F,0x00,0x74,0x00,0x6F,0x00,0x73,0x00,0x2F,0x00,0x00,0x00,};
const U8 CAMERA_STORAGE_FILEPATH_MEMORY_CARD[] = {0x50,0x00,0x68,0x00,0x6F,0x00,0x74,0x00,0x6F,0x00,0x73,0x00,0x2F,0x00,0x00,0x00,};
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
const U8 CAMERA_STORAGE_FILEPATH_MEMORY_SEC_CARD[] = {0x50,0x00,0x68,0x00,0x6F,0x00,0x74,0x00,0x6F,0x00,0x73,0x00,0x2F,0x00,0x00,0x00,};
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
//U8 CAMERA_STORAGE_UNSAVE_FILENAME[]      = {0x7E,0x00,0x63,0x00,0x61,0x00,0x6D,0x00,0x65,0x00,0x72,0x00,0x61,0x00,0x2E,0x00,0x6A,0x00,0x70,0x00,0x67,0x00,0x00,0x00,};

/****************************************************************************
* Setting command map                                                                
*****************************************************************************/
/* mapping mmi settings to mdi command. */
/*********** EV ***********/
const U8	camera_ev_command_map[CAMERA_SETTING_EV_COUNT+1] = 
{
//#ifdef __CAMERA_FEATURE_EV_4__
//	MDI_CAMERA_EV_N4,				/* CAMERA_SETTING_EV_N4 */
//#endif
		
//#ifdef __CAMERA_FEATURE_EV_3__	
//	MDI_CAMERA_EV_N3,				/* CAMERA_SETTING_EV_N3 */
//#endif 

//#ifdef __CAMERA_FEATURE_EV_2__
	MDI_CAMERA_EV_N2,				/* CAMERA_SETTING_EV_N2 */
//#endif

	MDI_CAMERA_EV_N1,				/* CAMERA_SETTING_EV_N1 */
	MDI_CAMERA_EV_0,				/* CAMERA_SETTING_EV_0 */
	MDI_CAMERA_EV_P1,				/* CAMERA_SETTING_EV_P1 */
	
//#ifdef __CAMERA_FEATURE_EV_2__	
	MDI_CAMERA_EV_P2,				/* CAMERA_SETTING_EV_P2 */
//#endif

//#ifdef __CAMERA_FEATURE_EV_3__
//	MDI_CAMERA_EV_P3,				/* CAMERA_SETTING_EV_P3	*/
//#endif

//#ifdef __CAMERA_FEATURE_EV_4__
//	MDI_CAMERA_EV_P4,				/* CAMERA_SETTING_EV_P4 */
//#endif

	DUMMY_COMMAND
};


/*********** EFFECT ***********/
const U8 camera_effect_command_map[CAMERA_SETTING_EFFECT_COUNT+1] = 
{

#ifdef __CAMERA_FEATURE_EFFECT_NORMAL__
	MDI_CAMERA_EFFECT_NORMAL,			/* CAMERA_SETTING_EFFECT_NOMRAL */
#endif

#ifdef __CAMERA_FEATURE_EFFECT_GRAYSCALE__
	MDI_CAMERA_EFFECT_GRAYSCALE,			/* CAMERA_SETTING_EFFECT_GRAYSCALE */
#endif 

#ifdef __CAMERA_FEATURE_EFFECT_SEPIA__
	MDI_CAMERA_EFFECT_SEPIA,				/* CAMERA_SETTING_EFFECT_SEPIA */
#endif

#ifdef  __CAMERA_FEATURE_EFFECT_SEPIA_GREEN__
	MDI_CAMERA_EFFECT_SEPIA_GREEN,		/* CAMERA_SETTING_EFFECT_SEPIA_GREEN */	
#endif 

#ifdef __CAMERA_FEATURE_EFFECT_SEPIA_BLUE__
	MDI_CAMERA_EFFECT_SEPIA_BLUE,			/* CAMERA_SETTING_EFFECT_SEPIA_BLUE */	
#endif

#ifdef __CAMERA_FEATURE_EFFECT_COLOR_INVERT__
	MDI_CAMERA_EFFECT_COLOR_INVERT,		/* CAMERA_SETTING_EFFECT_COLOR_INVERT */	
#endif 

#ifdef __CAMERA_FEATURE_EFFECT_GRAY_INVERT__
	MDI_CAMERA_EFFECT_GRAY_INVERT,		/* CAMERA_SETTING_EFFECT_GRAY_INVERT */	
#endif

#ifdef __CAMERA_FEATURE_EFFECT_BLACKBOARD__
	MDI_CAMERA_EFFECT_BLACKBOARD,			/* CAMERA_SETTING_EFFECT_BLACKBOARD */	
#endif 

#ifdef __CAMERA_FEATURE_EFFECT_WHITEBOARD__
	MDI_CAMERA_EFFECT_WHITEBOARD,			/* CAMERA_SETTING_EFFECT_WHITEBOARD */	
#endif 

#ifdef __CAMERA_FEATURE_EFFECT_COPPER_CARVING__
	MDI_CAMERA_EFFECT_COPPER_CARVING,	/* CAMERA_SETTING_EFFECT_COPPER_CARVING */	
#endif 

#ifdef __CAMERA_FEATURE_EFFECT_BLUE_CARVING__
	MDI_CAMERA_EFFECT_BLUE_CARVING,		/* CAMERA_SETTING_EFFECT_BLUE_CARVING */		
#endif 

#ifdef __CAMERA_FEATURE_EFFECT_EMBOSSMENT__
	MDI_CAMERA_EFFECT_EMBOSSMENT,			/* CAMERA_SETTING_EFFECT_EMBOSSMENT */		
#endif 

#ifdef __CAMERA_FEATURE_EFFECT_CONTRAST__
	MDI_CAMERA_EFFECT_CONTRAST,			/* CAMERA_SETTING_EFFECT_CONTRAST */		
#endif 

#ifdef __CAMERA_FEATURE_EFFECT_JEAN__
	MDI_CAMERA_EFFECT_JEAN,					/* CAMERA_SETTING_EFFECT_JEAN */	
#endif

#ifdef __CAMERA_FEATURE_EFFECT_SKETCH__
	MDI_CAMERA_EFFECT_SKETCH,				/* CAMERA_SETTING_EFFECT_SKETCH */	
#endif

#ifdef __CAMERA_FEATURE_EFFECT_OIL__
	MDI_CAMERA_EFFECT_OIL,					/* CAMERA_SETTING_EFFECT_OIL */	
#endif


/////added  by gugogi
#ifdef  __CAMERA_FEATURE_EFFECT_ANTIQUE__
     MDI_CAMERA_EFFECT_ANTIQUE,
#endif

#ifdef  __CAMERA_FEATURE_EFFECT_REDRISH__
    MDI_CAMERA_EFFECT_REDRISH,
#endif

#ifdef  __CAMERA_FEATURE_EFFECT_GREENRISH__
    MDI_CAMERA_EFFECT_GREENRISH,
#endif

#ifdef  __CAMERA_FEATURE_EFFECT_BLUERISH__
    MDI_CAMERA_EFFECT_BLUERISH,
#endif


#ifdef __CAMERA_FEATURE_EFFECT_BLACKWHITE__
     MDI_CAMERA_EFFECT_BLACKWHITE,
#endif


#ifdef  __CAMERA_FEATURE_EFFECT_NEGATIVE__
    MDI_CAMERA_EFFECT_NEGATIVENESS,
#endif

	DUMMY_COMMAND
};


/*********** WB ***********/
const U8 camera_wb_command_map[CAMERA_SETTING_WB_COUNT+1] = 
{
#if 1 //chenhe
	0,1,2,3,4,
#else
//#ifdef __CAMERA_FEATURE_WB_AUTO__
	MDI_CAMERA_WB_AUTO,								/*	CAMREA_SETTING_WB_AUTO */
//#endif 

//#ifdef __CAMERA_FEATURE_WB_DAYLIGHT__
	MDI_CAMERA_WB_DAYLIGHT,							/* CAMERA_SETTING_WB_DAYLIGHT */
//#endif 

//#ifdef __CAMERA_FEATURE_WB_TUNGSTEN__
	MDI_CAMERA_WB_TUNGSTEN,							/* CAMERA_SETTING_WB_TUNGSTEN */ 
//#endif 

//#ifdef __CAMERA_FEATURE_WB_FLUORESCENT__
	MDI_CAMERA_WB_FLUORESCENT,						/* CAMERA_SETTING_WB_FLUORESCENT */
//#endif  

//#ifdef __CAMERA_FEATURE_WB_CLOUD__
	MDI_CAMERA_WB_CLOUD,								/* CAMERA_SETTING_WB_CLOUD */
//#endif  

//#ifdef __CAMERA_FEATURE_WB_INCANDESCENCE__
	MDI_CAMERA_WB_INCANDESCENCE,					/* CAMERA_SETTING_WB_INCANDESCENCE */
//#endif  
#endif
	DUMMY_COMMAND
};


/*********** Image Qty ***********/
const U8 camera_image_qty_command_map[CAMERA_SETTING_IMAGE_QTY_COUNT+1] =
{
//#ifdef __CAMERA_FEATURE_IMAGE_QTY_LOW__
	//MDI_CAMERA_JPG_QTY_LOW,							/* CAMERA_SETTING_IMAGE_QTY_LOW */
//#endif  /* CAMERA_FEATURE_JPG_QTY_LOW */

//#ifdef __CAMERA_FEATURE_IMAGE_QTY_NORMAL__
	MDI_CAMERA_JPG_QTY_NORMAL,						/* CAMERA_SETTING_IMAGE_QTY_NORMAL */
//#endif 

//#ifdef __CAMERA_FEATURE_IMAGE_QTY_HIGH__
	MDI_CAMERA_JPG_QTY_HIGH,						/* CAMERA_SETTING_IMAGE_QTY_HIGH */
//#endif

//#ifdef __CAMERA_FEATURE_IMAGE_QTY_FINE__
	//MDI_CAMERA_JPG_QTY_FINE,						/* CAMREA_SETTING_IMAGE_QTY_FINE */
//#endif

	DUMMY_COMMAND
};
//add by WeiD
const U8 cool_camera_brightness_command_map[CAMERA_SETTING_BT_COUNT+1] =
{
	CAMERA_SETTING_BRIGHT_1,
	CAMERA_SETTING_BRIGHT_2,
	CAMERA_SETTING_BRIGHT_3,
	CAMERA_SETTING_BRIGHT_4,
	CAMERA_SETTING_BRIGHT_5,
	DUMMY_COMMAND
};

const U8 cool_camera_contrast_command_map[CAMERA_SETTING_CT_COUNT+1] =
{
	CAMERA_SETTING_CONTRAST_1,
	CAMERA_SETTING_CONTRAST_2,
	CAMERA_SETTING_CONTRAST_3,
	CAMERA_SETTING_CONTRAST_4,
	CAMERA_SETTING_CONTRAST_5,
	DUMMY_COMMAND
};

//add by WeiD
/*********** Banding ***********/
const U8 camera_banding_command_map[CAMERA_SETTING_BANDING_COUNT+1] =
{
	MDI_CAMERA_BANDING_50HZ,
	MDI_CAMERA_BANDING_60HZ,
	DUMMY_COMMAND
};


/*********** AE meter ***********/
const U8 camera_ae_meter_command_map[CAMERA_SETTING_AE_METER_COUNT+1] =
{
//#ifdef __CAMERA_FEATURE_AE_METER_AUTO__
	MDI_CAMERA_AE_METER_AUTO,						/* CAMERA_SETTING_AE_METER_AUTO */
//#endif  

//#ifdef __CAMERA_FEATURE_AE_METER_SPOT__
	MDI_CAMERA_AE_METER_SPOT,						/* CAMERA_SETTING_AE_METER_SPOT */
//#endif 

//#ifdef __CAMERA_FEATURE_AE_METER_CENTER__
	MDI_CAMERA_AE_METER_CENTER,					/* CAMERA_SETTING_AE_METER_CENTER */
//#endif

//#ifdef __CAMERA_FEATURE_AE_METER_AVERAGE__
	MDI_CAMERA_AE_METER_AVERAGE,					/* CAMERA_SETTING_AE_METER_AVERAGE */
//#endif

	DUMMY_COMMAND
};


/*********** Flash ***********/
const U8 camera_flash_command_map[CAMERA_SETTING_FLASH_COUNT+1] =
{
	MDI_CAMERA_FLASH_OFF,							/* CAMERA_SETTING_FLASH_OFF */
	MDI_CAMERA_FLASH_ON,								/* CAMERA_SETTING_FLASH_ON */

#ifdef __CAMERA_FEATURE_FLASH_AUTO__
	MDI_CAMERA_FLASH_AUTO,							/* CAMERA_SETTING_FLASH_AUTO */
#endif

#ifdef __CAMERA_FEATURE_FLASH_REDEYE__
	MDI_CAMERA_FLASH_REDEYE,						/* CAMERA_SETTING_FLASH_REDEYE */
#endif

	DUMMY_COMMAND
};



/*********** ISO ***********/
const U8 camera_iso_command_map[CAMERA_SETTING_ISO_COUNT+1] =
{
#ifdef __CAEMRA_FEATURE_ISO_AUTO__
	MDI_CAMERA_ISO_AUTO,					/* CAMERA_SETTING_ISO_AUTO */
#endif  

#ifdef __CAEMRA_FEATURE_ISO_100__
	MDI_CAMERA_ISO_100,					/* CAMERA_SETTING_ISO_100 */
#endif

#ifdef __CAEMRA_FEATURE_ISO_200__
	MDI_CAMERA_ISO_200,					/* CAMERA_SETTING_ISO_200 */
#endif

#ifdef __CAEMRA_FEATURE_ISO_400__
	MDI_CAMERA_ISO_400,					/* CAMERA_SETTING_ISO_400 */
#endif

	DUMMY_COMMAND
};


/*********** Shot Mode ***********/
const U8 camera_shot_mode_command_map[CAMERA_SETTING_SHOT_MODE_COUNT+1] =
{
#ifdef __CAMERA_FEATURE_SHOT_MODE_AUTO__
	MDI_CAMERA_SHOT_MODE_AUTO,						/* CAMERA_SETTING_SHOT_MODE_AUTO */
#endif  

#ifdef __CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI__
	MDI_CAMERA_SHOT_MODE_SHUTTER_PRI,			/* CAMERA_SETTING_SHOT_MODE_SHUTTER_PRI */
#endif 

#ifdef __CAMERA_FEATURE_SHOT_MODE_APERTURE_PRI__
	MDI_CAMERA_SHOT_MODE_APERTURE_PRI,			/* CAMERA_SETTING_SHOT_MODE_APERTURE_PRI */
#endif

#ifdef __CAMERA_FEATURE_SHOT_MODE_PORTRAIT__
	MDI_CAMERA_SHOT_MODE_PORTRAIT,				/* CAMERA_SETTING_SHOT_MODE_PORTRAIT */
#endif

#ifdef __CAMERA_FEATURE_SHOT_MODE_LANDSCAPE__
	MDI_CAMERA_SHOT_MODE_LANDSCAPE,				/* CAMERA_SETTING_SHOT_MODE_LANDSCAPE */
#endif

#ifdef __CAMERA_FEATURE_SHOT_MODE_SPORT__
	MDI_CAMERA_SHOT_MODE_SPORT,					/* CAMERA_SETTING_SHOT_MODE_SPORT */
#endif

#ifdef __CAMERA_FEATURE_SHOT_MODE_FLOWER__
	MDI_CAMERA_SHOT_MODE_FLOWER,					/* CAMERA_SETTING_SHOT_MODE_FLOWER */
#endif

#ifdef __CAMERA_FEATURE_SHOT_MODE_NIGHT__
	MDI_CAMERA_SHOT_MODE_NIGHT,					/* CAMERA_SETTING_SHOT_MODE_NIGHT */
#endif

	DUMMY_COMMAND
};


/****************************************************************************
* Global Variable - Extern                                                                 
*****************************************************************************/
extern BOOL	r2lMMIFlag;


/****************************************************************************
* Global Function                                                           
*****************************************************************************/
#ifdef MMI_ON_HARDWARE_P
	extern void 	custom_start_flashlight(kal_uint8 red_level, kal_uint8 green_level, kal_uint8 blue_level, kal_uint8 duty);
	extern void 	custom_stop_flashlight(void);
#endif

extern kal_uint8	GpioCtrlReq(kal_uint8 action);
extern BOOL 		IsClamClose(void);
extern void 		SetClamState(U16 clam_state);
extern void 		AudioStopReq(U16 soundId);
extern U16 			GetCurKeypadTone(void);
extern void 		GpioDetectInd(void *info);

#ifdef __MMI_TOUCH_SCREEN__
	MMI_BOOL mmi_wgui_test_lsk_position(mmi_pen_point_struct pos);
#endif /* __MMI_TOUCH_SCREEN__ */


/***************************************************************************** 
* Local Variable
*****************************************************************************/
/* camera contex */
camera_context_struct 	g_camera_cntx;

#ifdef __CAMERA_FEATURE_OSD_MENU__
	/* camera UI */
	CAM_UI_POPLIST			g_cam_ui_poplist;
	CAM_UI_ICONLIST		g_cam_ui_iconlist;
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


#ifndef APPMEM_SUPPORT
	/* if we need add osd layer but no appmem not supported, we will use static buffer */
	U32 camera_osd_layer_buffer[(CAMERA_OSD_BUFFER_SIZE)/4];

	#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
		U32 camera_tile_result_buffer[CAMERA_TILE_RESULT_BUFFER_SIZE/4];
	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */
	
#endif /* APPMEM_SUPPORT */

BOOL g_camera_image_is_convert = FALSE;

/*****************************************************************************
* Local Function 
*****************************************************************************/
/* setting / path / stroage */
static BOOL mmi_camera_get_storage_disk_path(PS8 drv_buf);
void mmi_camera_get_next_filename(PS8 file_buf_p);
static void mmi_camera_display_error_popup(void);

#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
	static void mmi_camera_display_sublcd_error_popup(void);
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */

static void mmi_camera_dummy_func(void);

/* app */
void mmi_camera_hilight_app(void);
static U16 	mmi_camera_get_img_size_str_id(S32 image_width, S32 image_height);
static void mmi_camera_exit_app_screen(void);
static void mmi_camera_enter_state(camera_state_enum state);
static void mmi_camera_exit_current_state(void);
		 U8 	mmi_camera_clam_close_event_hdlr(void* ptr);
static void mmi_camera_calc_remain_storage(void);

#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
	static void mmi_camera_toggle_self_capture(void);
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */

static void mmi_camera_toggle_hide_osd(void);

static void mmi_camera_turn_on_preview_led_highlight(void);
static void mmi_camera_turn_on_capture_led_highlight(void);
static void mmi_camera_turn_off_led_highlight(void);


/* mainlcd preview state */
static void mmi_camera_entry_preview_state(void);
static void mmi_camera_exit_preview_state(void);
static void mmi_camera_preview_set_key_hdlr(void);
static void mmi_camera_preview_create_resource(void);
static void mmi_camera_preview_free_resource(void);

static void mmi_camera_preview_create_preview_resource(S32 offset_x,S32 offset_y,S32 width,S32 height,U8 rotate);
static void mmi_camera_preview_free_preview_resource(void);

static void mmi_camera_preview_start(void);
static void mmi_camera_preview_stop(void);
static void mmi_camera_preview_set_blt_layer(BOOL is_previewing);
static void mmi_camera_preview_clear_background(void);

static void mmi_camera_preview_flatten_layer(BOOL is_previewing);

static void mmi_camera_preview_draw_osd(void);
static void mmi_camera_preview_draw_title(void);
static void mmi_camera_preview_draw_softkey(void);
static void mmi_camera_preview_draw_hint(void);
static void mmi_camera_preview_draw_icon(void);

#ifdef __CAMERA_FEATURE_CONT_SHOT_DRAW_COUNTBAR__
	static void mmi_camera_preview_draw_contshot_countbar(void);
#endif /* __CAMERA_FEATURE_CONT_SHOT_DRAW_COUNTBAR__ */

static void mmi_camera_preview_set_hint(PS8 hint_str1, PS8 hint_str2, U16 fade_time);
static void mmi_camera_preview_hide_hint(void);

static U16 	mmi_camera_preview_get_zoom_factor(void);
#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__

static void mmi_camera_preview_ev_inc_key_press(void);
#endif
#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
static void mmi_camera_preview_ev_dec_key_press(void);
#endif

//add by WeiD

static void mmi_camera_preview_bt_inc_key_press(void);
static void mmi_camera_preview_bt_inc_key_release(void);
static void mmi_camera_preview_bt_dec_key_press(void);
static void mmi_camera_preview_bt_dec_key_release(void);

static void mmi_camera_preview_contrast_hotkey_press(void);




//add by WeiD

#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
	static void mmi_camera_iconbar_right_arrow_press(void);
	static void mmi_camera_iconbar_right_arrow_release(void);
	static void mmi_camera_iconbar_left_arrow_press(void);
	static void mmi_camera_iconbar_left_arrow_releas(void);
	static void mmi_camera_iconbar_up_arrow_press(void);
	static void mmi_camera_iconbar_up_arrow_release(void);
	static void mmi_camera_iconbar_down_arrow_press(void);
	static void mmi_camera_iconbar_down_arrow_release(void);
#endif /* __CAMERA_FEATURE_ICONBAR_PANEL__ */


//#ifdef __CAMERA_FEATURE_ZOOM__   //deleted by WeiD
	static void mmi_camera_preview_zoom_in(void);
	static void mmi_camera_preview_zoom_out(void);
	#ifndef __CAMERA_FEATURE_ICONBAR_PANEL__
		static void mmi_camera_preview_up_arrow_press(void);
		static void mmi_camera_preview_up_arrow_release(void);
		static void mmi_camera_preview_down_arrow_press(void);
		static void mmi_camera_preview_down_arrow_release(void);
	#endif /* !__CAMERA_FEATURE_ICONBAR_PANEL__ */ 
//#endif /* __CAMERA_FEATURE_ZOOM__ */

/* hotkeys */
#ifndef __CAMERA_FEATURE_ICONBAR_PANEL__
	#ifdef __CAMERA_FEATURE_EFFECT__
		static void mmi_camera_preview_effect_hotkey_press(void);
	#endif

	#ifdef __CAMERA_FEATURE_WB__
		static void mmi_camera_preview_wb_hotkey_press(void);
	#endif

	#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT__
		static void mmi_camera_preview_led_highlight_hotkey_press(void);
	#endif

	#ifdef __CAMERA_FEATURE_FLASH__
		static void mmi_camera_preview_flash_hotkey_press(void);
	#endif

#ifndef __MMI_DISABLE_DUAL_SENSOR__
		 void mmi_camera_preview_switch_sensor_hotkey_press(void);
	#endif

	#ifdef __CAMERA_FEATURE_NIGHT__
		static void mmi_camera_preview_night_hotkey_press(void);
	#endif

	#ifdef __CAMERA_FEATURE_CONT_SHOT__
		static void mmi_camera_preview_cont_shot_hotkey_press(void);
	#endif

	#ifdef __CAMERA_FEATURE_DELAY_TIMER__
		static void mmi_camera_preview_delay_timer_hotkey_press(void);
	#endif

	static void mmi_camera_preview_image_qty_hotkey_press(void);
	static void mmi_camera_preview_image_size_hotkey_press(void);

	#ifdef __CAMERA_FEATURE_STORAGE__
		static void mmi_camera_preview_storage_hotkey_press(void);
	#endif 

   #ifdef __CAMERA_FEATURE_SHOT_MODE__
	   static void mmi_camera_preview_shot_mode_hotkey_press(void);
   #endif 
   
   #ifdef __CAMERA_FEATURE_ISO__
	   static void mmi_camera_preview_iso_hotkey_press(void);
   #endif 
   
   #ifdef __CAMERA_FEATURE_AE_METER__
	   static void mmi_camera_preview_ae_meter_hotkey_press(void);
   #endif 

#endif /* __CAMERA_FEATURE_ICONBAR_PANEL__ */



#ifdef __CAMERA_FEATURE_EFFECT__
	static void mmi_camera_preview_effect_change(BOOL is_next);
#endif

#ifdef __CAMERA_FEATURE_WB__
	static void mmi_camera_preview_wb_change(BOOL is_next);	
#endif

#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT__
	static void mmi_camera_preview_led_highlight_change(BOOL is_next);	
#endif

#ifdef __CAMERA_FEATURE_FLASH__
	static void mmi_camera_preview_flash_change(BOOL is_next);	
#endif

#ifdef __CAMERA_FEATURE_NIGHT__
	static void mmi_camera_preview_night_change(BOOL is_next);	
#endif

#ifdef __CAMERA_FEATURE_CONT_SHOT__
	static void mmi_camera_preview_cont_shot_change(BOOL is_next);	
#endif

#ifdef __CAMERA_FEATURE_DELAY_TIMER__
	static void mmi_camera_preview_delay_timer_change(BOOL is_next);
#endif

static void mmi_camera_preview_image_size_change(BOOL is_next);

static void mmi_camera_preview_image_qty_change(BOOL is_next);

#ifdef __CAMERA_FEATURE_STORAGE__
	static void mmi_camera_preview_storage_change(BOOL is_next);	
#endif 

#ifdef __CAMERA_FEATURE_SHOT_MODE__
	static void mmi_camera_preview_shot_mode_change(BOOL is_next);	
	static U16	mmi_camera_get_shutter_pri_str_id(S32 feature_id);
	static U16	mmi_camera_get_aperture_pri_str_id(S32 feature_id);
#endif 

#ifdef __CAMERA_FEATURE_ISO__
	static void mmi_camera_preview_iso_change(BOOL is_next);	
#endif 

#ifdef __CAMERA_FEATURE_AE_METER__
	static void mmi_camera_preview_ae_meter_change(BOOL is_next);
#endif 

#ifdef __CAMERA_FEATURE_ADD_FRAME__
	static void mmi_camera_draw_add_frame(void);
//	static void mmi_camera_preview_change_frame_up(void);
//	static void mmi_camera_preview_change_frame_down(void);
#endif 

static void mmi_camera_preview_lsk_press(void);
static void mmi_camera_preview_lsk_release(void);
static void mmi_camera_preview_rsk_press(void);
static void mmi_camera_preview_rsk_release(void);
//static void mmi_camera_preview_ind_handler(void *data);

/* countdown state */
static void mmi_camera_entry_countdown_state(void);
static void mmi_camera_exit_countdown_state(void);
static void mmi_camera_countdown_cyclic(void);
static void mmi_camera_countdown_stop_LED(void);

/* contshot state */
static void mmi_camera_entry_contshot_state(void);
static void mmi_camera_exit_contshot_state(void);
static void mmi_camera_contshot_cyclic(void);

#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
	/* burstshot state */
	static void mmi_camera_entry_burstshot_capture_state(void);
	static void mmi_camera_exit_burstshot_capture_state(void);

#if defined(__CAMERA_FEATURE_CONT_SHOT_BURST__) && defined(__CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__)
	static void mmi_camera_burstshot_capture_result_hdlr(MDI_RESULT ret, U16 captured_count);
	static void mmi_camera_burstshot_close_multi_sound(void);
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ && __CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__ */

	/* burstshot save confirm state */
	static void mmi_camera_entry_burstshot_save_confirm_state(void);
	static void mmi_camera_exit_burstshot_save_confirm_state(void);
	static void mmi_camera_burstshot_save_confirm_lsk_press(void);
	static void mmi_camera_burstshot_save_confirm_lsk_release(void);
	static void mmi_camera_burstshot_save_confirm_rsk_press(void);
	static void mmi_camera_burstshot_save_confirm_rsk_release(void);

	/* burstshot saving state */
	static void mmi_camera_entry_burstshot_saving_state(void);
	static void mmi_camera_exit_burstshot_saving_state(void);
	static void mmi_camera_burstshot_saving_cyclic(void);
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */

/* save confirm state */
static void mmi_camera_entry_save_confirm_state(void);
static void mmi_camera_exit_save_confirm_state(void);
static void mmi_camera_save_confirm_lsk_press(void);
static void mmi_camera_save_confirm_lsk_release(void);
static void mmi_camera_save_confirm_rsk_press(void);
static void mmi_camera_save_confirm_rsk_release(void);
static void mmi_camera_save_confirm_cancel_file(void);
static void mmi_camera_save_confirm_save_file(void);

/* OSD menu state */
#ifdef __CAMERA_FEATURE_OSD_MENU__
	static void mmi_camera_entry_osd_menu_state(void);
	static void mmi_camera_exit_osd_menu_state(void);
	static void mmi_camera_osd_menu_draw_osd(void);
	static void mmi_camera_osd_menu_lsk_press(void);	
	static void mmi_camera_osd_menu_rsk_release(void);	

	static void mmi_camera_osd_menu_iconlist_move_in(void);
	static void mmi_camera_osd_menu_iconlist_move_out(void);	

	static void mmi_camera_osd_menu_left_arrow_press(void);
	static void mmi_camera_osd_menu_left_arrow_release(void);		
	static void mmi_camera_osd_menu_left_arrow_release_no_action(void);

	static void mmi_camera_osd_menu_right_arrow_press(void);
	static void mmi_camera_osd_menu_right_arrow_release(void);	
	static void mmi_camera_osd_menu_right_arrow_release_no_action(void);		

	static void mmi_camera_osd_menu_highlight_image_size(S32 index);
	static void mmi_camera_osd_menu_highlight_image_qty(S32 index);
	static void mmi_camera_osd_menu_highlight_wb(S32 index);
	static void mmi_camera_osd_menu_highlight_effect(S32 index);
	static void mmi_camera_osd_menu_highlight_night(S32 index);
	static void mmi_camera_osd_menu_highlight_add_frame(S32 index);
	static void mmi_camera_osd_menu_highlight_cont_shot(S32 index);
	static void mmi_camera_osd_menu_highlight_delay_timer(S32 index);
	static void mmi_camera_osd_menu_highlight_led_highlight(S32 index);
	static void mmi_camera_osd_menu_highlight_shutter_sound(S32 index);
	static void mmi_camera_osd_menu_highlight_banding(S32 index);
	static void mmi_camera_osd_menu_highlight_self_capture(S32 index);
	static void mmi_camera_osd_menu_highlight_storage(S32 index);	

	#ifdef __CAMERA_FEATURE_TOUCH_SCREEN__
		static void mmi_camera_osd_menu_touch_scr_pen_down(S32 x, S32 y);
		static void mmi_camera_osd_menu_touch_scr_pen_up(S32 x, S32 y);	
	#endif /* __CAMERA_FEATURE_TOUCH_SCREEN__ */
#if defined(__CAMERA_OSD_HORIZONTAL__) || defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__) && defined(__CAMERA_FEATURE_TOUCH_SCREEN__)
	static void mmi_camera_update_osd_touch_icon();
#endif



	/* UI */
	/* poplist */
	static void mmi_camera_osd_menu_draw_image(CAM_UI_IMG *img_p);
	static void mmi_camera_osd_menu_draw_image_tile(CAM_UI_IMG_TILE *img_tile_p);

	static void mmi_camera_osd_menu_init_poplist(S32 mark_offset_x);
	static void mmi_camera_osd_menu_draw_poplist(void);
	static void mmi_camera_osd_menu_draw_poplist_title(void);
	static void mmi_camera_osd_menu_draw_poplist_bottom(void);
	static void mmi_camera_osd_menu_draw_poplist_body(void);
	static void mmi_camera_osd_menu_draw_poplist_body_list(void);

	static void mmi_camera_osd_menu_poplist_up_arrow_press(void);
	static void mmi_camera_osd_menu_poplist_down_arrow_press(void);
	
	static void mmi_camera_osd_menu_poplist_up_arrow_release(void);
	static void mmi_camera_osd_menu_poplist_up_arrow_release_no_action(void);
	static void mmi_camera_osd_menu_poplist_down_arrow_release(void);		
	static void mmi_camera_osd_menu_poplist_down_arrow_release_no_action(void);	
	
	static void mmi_camera_osd_menu_poplist_goto_item(S32 index);

	/* iconlist */
	static void mmi_camera_osd_menu_init_iconlist(void);
	static void mmi_camera_osd_menu_draw_iconlist(void);
	static void mmi_camera_osd_menu_draw_iconlist_left_arrow(void);
	static void mmi_camera_osd_menu_draw_iconlist_right_arrow(void);		
	static void mmi_camera_osd_menu_iconlist_move_next(void);	/* selection move to right */
	static void mmi_camera_osd_menu_iconlist_move_prev(void);	/* selection move to left */	
	
#endif /* __CAMERA_FEATURE_OSD_MENU__ */

/* capture */
static void mmi_camera_capture(void);
static void mmi_camera_capture_button_press(void);
static void mmi_camera_capture_button_release(void);

#if defined(__CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__)
static void mmi_camera_capture_play_multi_channel_sound(void);
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__ */

/* save done */
static void mmi_camera_entry_save_done_state(void);
static void mmi_camera_exit_save_done_state(void);
static void mmi_camera_save_done_timeout(void);

#ifdef __MMI_SUBLCD__
	/* sub screen */
	static void mmi_camera_entry_sublcd_screen(void);
	static void mmi_camera_exit_sublcd_screen(void);
	static void mmi_camera_sublcd_capture(void);
#endif /* __MMI_SUBLCD__ */


#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
	/* toggle to sub state */
	static void mmi_camera_entry_toggle_to_sub_state(void);
	static void mmi_camera_exit_toggle_to_sub_state(void);
	static void mmi_camera_toggle_to_sub_timeout(void);

	/* toggle to main state */
	static void mmi_camera_entry_toggle_to_main_state(void);
	static void mmi_camera_exit_toggle_to_main_state(void);
	static void mmi_camera_toggle_to_main_timeout(void);

	/* sublcd preview state */
	static void mmi_camera_entry_sublcd_preview_state(void);
	static void mmi_camera_exit_sublcd_preview_state(void);
	static void mmi_camera_sublcd_preview_set_key_hdlr(void);
	static void mmi_camera_sublcd_preview_start(void);
	static void mmi_camera_sublcd_preview_stop(void);
	static void mmi_camera_sublcd_preview_set_blt_layer(BOOL is_previewing);
	static void mmi_camera_sublcd_preview_lsk_press(void);
	static void mmi_camera_sublcd_preview_rsk_press(void);
	static void mmi_camera_sublcd_preview_lsk_release(void);
	static void mmi_camera_sublcd_preview_rsk_release(void);

	/* sublcd countdown state */
	static void mmi_camera_entry_sublcd_countdown_state(void);
	static void mmi_camera_exit_sublcd_countdown_state(void);
	static void mmi_camera_sublcd_countdown_cyclic(void);
	static void mmi_camera_sublcd_countdown_stop_LED(void);

	/* sublcd contshot state */
	static void mmi_camera_entry_sublcd_contshot_state(void);
	static void mmi_camera_exit_sublcd_contshot_state(void);
	static void mmi_camera_sublcd_contshot_cyclic(void);

 	#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
		/* sublcd burstshot state */
		static void mmi_camera_entry_sublcd_burstshot_state(void);
		static void mmi_camera_exit_sublcd_burstshot_state(void);

		#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__
			static void mmi_camera_sublcd_burstshot_capture_result_hdlr(MDI_RESULT ret, U16 captured_count);
		#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__ */
		
	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */

	/* sublcd burstshot saving state */
	#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__ 
		static void mmi_camera_entry_sublcd_burstshot_saving_state(void);
		static void mmi_camera_exit_sublcd_burstshot_saving_state(void);
		static void mmi_camera_sublcd_burstshot_saving_cyclic(void);
	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */

	/* sublcd capture */
	static void mmi_camera_sublcd_capture_button_press(void);
	static void mmi_camera_sublcd_capture(void);

	#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
		static void mmi_camera_tileshot_capture(void);
		#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
			static void mmi_camera_sublcd_tileshot_capture(void);
		#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */
	#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */ 

#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */



/* util draw */
static void mmi_camera_draw_style_text(PS8 str, S32 offset_x, S32 offset_y, camera_style_text_struct *style_txt);

#if defined(__CAMERA_FEATURE_TOUCH_SCREEN__) && defined(__MMI_TOUCH_SCREEN__)
	static void mmi_camera_touch_scr_pen_down_hdlr(mmi_pen_point_struct pos);
	static void mmi_camera_touch_scr_pen_up_hdlr(mmi_pen_point_struct pos);
	static void mmi_camera_touch_scr_pen_move_hdlr(mmi_pen_point_struct pos);
	static BOOL mmi_camera_touch_scr_hit_test(S32 pos_x, S32 pos_y, camera_ui_touch_struct *touch_obj);
#endif /* __CAMERA_FEATURE_TOUCH_SCREEN__ && __MMI_TOUCH_SCREEN__ */

/* screen snapshot */
#ifdef __MMI_SCREEN_SNAPSHOT__
	static void mmi_camera_entry_scr_snapshot_screen(void);
	static void mmi_camera_exit_scr_snapshot_screen(void);
#endif /* __MMI_SCREEN_SNAPSHOT__ */

#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
void mmi_camera_restart_app_watchdog(void);
void mmi_camera_stop_app_watchdog(void);
void mmi_camera_app_watchdog_callback(void);
void mmi_camera_stop_delete_confirm_watchdog(void);
void mmi_camera_delete_confirm_watchdog_timer_callback(void);
void mmi_camera_start_delete_confirm_watchdog(void);
#endif /* __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__ */

extern BOOL HandleLowBatteryProcess(void);

/***************************************************************************
*
*	Settings / File Path / Storage handling                                                     
*
***************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_camera_check_and_restore_valid_drv()
* DESCRIPTION
*   Check if desired path is ready or not. If not, will restore to valid path.
* PARAMETERS
*	 nil
* RETURNS
*   BOOL
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if 0
BOOL mmi_get_Camera_running_state(void)
{
	return ( !(g_camera_cntx.app_state == CAMERA_STATE_EXIT));
}
#endif
BOOL mmi_camera_check_and_restore_valid_drv(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32			ret_drv;
	S8				buf[64];
	S8				UCS2_buf[128];
	FS_HANDLE	file_handle;


    mmi_trace(g_sw_CAMERA, "mmi_camera_check_and_restore_valid_drv()");

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   
   mmi_trace(g_sw_CAMERA, "g_camera_cntx.setting.storage = %d", g_camera_cntx.setting.storage);
   
	/* Get storage location */
	if(g_camera_cntx.setting.storage == CAMERA_SETTING_STORAGE_PHONE)
	{
		/* path is phone */
		ret_drv = MMI_FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM); //LICHENG opend 2009-8-20
		//ret_drv = 'E'; //add by panxu 2007-8-10
	}
	else if(g_camera_cntx.setting.storage == CAMERA_SETTING_STORAGE_MEMORY_CARD)
	{
		/* path is memory card */
		ret_drv 	= MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE);
	}
    /*+ zhouqin modify for dual t-flash card 20110503*/
    #if defined(DUAL_TFLASH_SUPPORT)
    else if(g_camera_cntx.setting.storage == CAMERA_SETTING_STORAGE_MEMORY_SEC_CARD)
	{
		/* path is memory card 2 */
		ret_drv 	= MMI_FS_GetDrive(FS_DRIVE_TFCARD_SECOND, 1, FS_NO_ALT_DRIVE);
	}
    #endif
    /*- zhouqin modify for dual t-flash card 20110503*/
	else
	{
		ASSERT(0);
		return FALSE;
	}

	mmi_trace(g_sw_CAMERA, "ret_drv = %d", ret_drv);
	
	if(ret_drv >= 0)
	{
		/* wufasong updated 2007.01.23 */
		/* drive available */
		//sprintf(buf, "%c:\\", (U8)ret_drv);
		//AnsiiToUnicodeString(UCS2_buf, buf);
		sprintf(buf, "%c:/", (U8)ret_drv);
		AnsiiToUnicodeString(UCS2_buf, buf);
		//pfnUnicodeStrcat((PS8)UCS2_buf, list_separator);

		file_handle = MMI_FS_Open((const UINT8 *)UCS2_buf, FS_OPEN_DIR | FS_READ_ONLY);
		//file_handle=0;//chenhe
		mmi_trace(g_sw_CAMERA, "file_handle = %d", file_handle);
		/* memory card not available */
		if(file_handle >= 0 || file_handle == FS_FOLDER_HANDLE)
			MMI_FS_Close(file_handle);
		else
		{
			/* card not available error */
			g_camera_cntx.setting.storage				= CAMERA_SETTING_STORAGE_PHONE;
			g_camera_cntx.is_storage_path_changed	= TRUE;
			mmi_camera_store_setting();
			
			return FALSE;
		}
	}
	else
	{
		/* drive not available error */
		g_camera_cntx.setting.storage				= CAMERA_SETTING_STORAGE_PHONE;	
		g_camera_cntx.is_storage_path_changed 	= TRUE;
		mmi_camera_store_setting();
		 
		return FALSE;
	}

	return TRUE;
}




/*****************************************************************************
* FUNCTION
*	 mmi_camera_get_storage_disk_path()
* DESCRIPTION
*   get current active storage disk path.
* PARAMETERS
*	 a  OUT	fill disk's name string.      
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static BOOL mmi_camera_get_storage_disk_path(PS8 drv_buf)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32	ret_drv;
	S8		buf[64];


    mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_get_storage_disk_path(),g_camera_cntx.setting.storage is %d",g_camera_cntx.setting.storage);
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_camera_cntx.setting.storage == CAMERA_SETTING_STORAGE_PHONE)
	{
		ret_drv	= MMI_FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM);

		if(ret_drv >=0)
		{
			/* wufasong updated 2007.01.23 */
			//sprintf(buf, "%c:\\", (U8)ret_drv); 
			//AnsiiToUnicodeString(drv_buf, buf);
			sprintf(buf, "%c:/", (U8)ret_drv); 
			AnsiiToUnicodeString(drv_buf, buf);
			//pfnUnicodeStrcat((PS8)drv_buf, list_separator);
			return TRUE;
		}
		else
		{
			/* phone drive should not fail */
            ASSERT(ret_drv >=0);
			return FALSE;
		}
	}
	else if(g_camera_cntx.setting.storage == CAMERA_SETTING_STORAGE_MEMORY_CARD)
	{
		ret_drv	= MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE , 1, FS_NO_ALT_DRIVE);

		if(ret_drv >=0)
		{
			sprintf(buf, "%c:/", (U8)ret_drv); 
			AnsiiToUnicodeString(drv_buf, buf);
			return TRUE;
		}
		else
		{
			#if 1//gdm 090630 fix bug12768 
			ret_drv	= MMI_FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM);
			
			if(ret_drv >=0)
			{
				sprintf(buf, "%c:/", (U8)ret_drv); 
				AnsiiToUnicodeString(drv_buf, buf);
				return TRUE;
			}
			#endif 
			return FALSE;
		}
	}
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
	else if(g_camera_cntx.setting.storage == CAMERA_SETTING_STORAGE_MEMORY_SEC_CARD)
	{
		ret_drv	= MMI_FS_GetDrive(FS_DRIVE_TFCARD_SECOND , 1, FS_NO_ALT_DRIVE);
		if(ret_drv >=0)
		{
			sprintf(buf, "%c:/", (U8)ret_drv); 
			AnsiiToUnicodeString(drv_buf, buf);
			return TRUE;
		}
		else
		{
			#if 1
			ret_drv	= MMI_FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM);
			
			if(ret_drv >=0)
			{
				sprintf(buf, "%c:/", (U8)ret_drv); 
				AnsiiToUnicodeString(drv_buf, buf);
				return TRUE;
			}
			#endif 
			return FALSE;
		}
	}
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
	return FALSE;
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_get_storage_file_path()
* DESCRIPTION
*   get current active storage file path.
* PARAMETERS
*	 a  OUT			fill storage full path     
* RETURNS
*   BOOL
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
BOOL mmi_camera_get_storage_file_path(PS8 filepath)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8		drv_buf[8];

	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_get_storage_file_path() status = %d",g_camera_cntx.setting.storage);
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   mmi_camera_load_setting();
   if(g_camera_cntx.setting.storage == CAMERA_SETTING_STORAGE_PHONE)
	{
//		mmi_trace(g_sw_CAMERA, "");
		if(mmi_camera_get_storage_disk_path(drv_buf))
		{
			S8 temp[256] = {0x00};
			mmi_trace_hex(g_sw_CAMERA, UCS2Strlen(drv_buf) * 2, drv_buf);
			pfnUnicodeStrcpy((PS8)filepath, (PS8)drv_buf);
			pfnUnicodeStrcat((PS8)filepath, (PS8)CAMERA_STORAGE_FILEPATH_PHONE);
			mmi_trace_hex(g_sw_CAMERA, UCS2Strlen(filepath) * 2, filepath);
			memset(temp, 0x00, sizeof(temp));
			UnicodeToAnsii(temp, (S8 *)filepath);
			mmi_trace(g_sw_CAMERA, "file path = %s", temp);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else if(g_camera_cntx.setting.storage == CAMERA_SETTING_STORAGE_MEMORY_CARD)
	{
		if(mmi_camera_get_storage_disk_path(drv_buf))
		{
			pfnUnicodeStrcpy((PS8)filepath, (PS8)drv_buf);
			pfnUnicodeStrcat((PS8)filepath, (PS8)CAMERA_STORAGE_FILEPATH_MEMORY_CARD);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
	else if(g_camera_cntx.setting.storage == CAMERA_SETTING_STORAGE_MEMORY_SEC_CARD)
	{
		if(mmi_camera_get_storage_disk_path(drv_buf))
		{
			pfnUnicodeStrcpy((PS8)filepath, (PS8)drv_buf);
			pfnUnicodeStrcat((PS8)filepath, (PS8)CAMERA_STORAGE_FILEPATH_MEMORY_SEC_CARD);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
	else
	{
		ASSERT(0);
	}
	return FALSE;
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_load_setting()
* DESCRIPTION
*   load camera settings from NVRAM
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_load_setting(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16	error;
        /*+ zhouqin modify for dual t-flash card 20110503*/
        #ifdef DUAL_TFLASH_SUPPORT
        S16 cardDrvLetter;
        S16 secardDrvLetter;
        #endif
        /*+ zhouqin modify for dual t-flash card 20110503*/

   mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_load_setting()");


   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* if setting is already loaded from NVRAM, it is already exist in memory, do not loaded again */
	if(!g_camera_cntx.is_setting_loaded)
	{
   		ReadRecord(NVRAM_EF_CAMERA_SETTING_LID, 1, (void*)&g_camera_cntx.setting, NVRAM_EF_CAMERA_SETTING_SIZE, &error);

		//g_camera_cntx.setting.storage =CAMERA_SETTING_STORAGE_MEMORY_CARD;  //add by WeiD for cann't save in phone
		/* first time init */
		/*gaosh modify for the default feature did not be loaded when down "lod".*/
		if((U8)g_camera_cntx.setting.wb==0xff )
		/*gaosh modify for the default feature did not be loaded when down "lod".*/	
		{
			mmi_camera_restore_setting();
		}

		g_camera_cntx.setting.preview_led_value = CAMERA_DEFAULT_PREVIEW_LED_VALUE;
		g_camera_cntx.setting.capture_led_value = CAMERA_DEFAULT_CAPTURE_LED_VALUE;		

		/* init preview size based on LCD size */
		mmi_camera_preview_update_capture_size();

		g_camera_cntx.is_setting_loaded = TRUE;
	}
    /*+ zhouqin modify for dual t-flash card 20110503*/
#ifndef DUAL_TFLASH_SUPPORT
    #ifndef 	COOLSAND_MULTIMEDIA_USE_FLASHFS
        g_camera_cntx.setting.storage = CAMERA_SETTING_STORAGE_MEMORY_CARD;
    #endif
#else		
	//if storage is t-flash but it does not exist,
	//reset storage before enter setting screen.
	if(g_camera_cntx.setting.storage== CAMERA_SETTING_STORAGE_MEMORY_CARD && 
		((cardDrvLetter = MMI_CARD_DRV) < 0))
	{		
	    g_camera_cntx.setting.storage= CAMERA_SETTING_STORAGE_PHONE;
	    mmi_camera_store_setting();
	}
	else if(g_camera_cntx.setting.storage== CAMERA_SETTING_STORAGE_MEMORY_SEC_CARD && 
		((secardDrvLetter = MMI_SEC_CARD_DRV) < 0))
	{		
            g_camera_cntx.setting.storage= CAMERA_SETTING_STORAGE_PHONE;
            mmi_camera_store_setting();
	}		         
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
  	
}




/*****************************************************************************
* FUNCTION
*	 mmi_camera_store_setting() 
* DESCRIPTION
*   store camera settings to NVRAM
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_store_setting(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16	error;

   mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_store_setting()");

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/ 
	WriteRecord(NVRAM_EF_CAMERA_SETTING_LID, 1, (void*)&g_camera_cntx.setting, NVRAM_EF_CAMERA_SETTING_SIZE, &error);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_restore_setting()
* DESCRIPTION
*   restore camera setting to default value
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_restore_setting(void)
{
	mmi_trace(g_sw_CAMERA, "Excute func  : mmi_camera_restore_setting()");
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_camera_cntx.setting.wb					= CAMERA_DEFAULT_SETTING_WB;
	g_camera_cntx.setting.flash				= CAMERA_DEFAULT_SETTING_FLASH;
	g_camera_cntx.setting.led_highlight		= CAMERA_DEFAULT_SETTING_LED_HIGHLIGHT;
	g_camera_cntx.setting.shutter_sound		= CAMERA_DEFAULT_SETTING_SHUTTER_SOUND;
	g_camera_cntx.setting.ev					= CAMERA_DEFAULT_SETTING_EV;
	g_camera_cntx.setting.night				= CAMERA_DEFAULT_SETTING_NIGHT;	
	g_camera_cntx.setting.closeup				= CAMERA_DEFAULT_SETTING_CLOSEUP;
	g_camera_cntx.setting.banding				= CAMERA_DEFAULT_SETTING_BANDING;
	g_camera_cntx.setting.image_size			= CAMERA_DEFAULT_SETTING_IMAGE_SIZE;
	g_camera_cntx.setting.image_qty			= CAMERA_DEFAULT_SETTING_IMAGE_QTY;
	g_camera_cntx.setting.iso					= CAMERA_DEFAULT_SETTING_ISO;
	g_camera_cntx.setting.ae_meter			= CAMERA_DEFAULT_SETTING_AE_METER;	
	
	g_camera_cntx.setting.zoom_idx			= CAMERA_DEFAULT_SETTING_ZOOM_IDX;
	g_camera_cntx.setting.effect				= CAMERA_DEFAULT_SETTING_EFFECT;
	g_camera_cntx.setting.shot_mode			= CAMERA_DEFAULT_SETTING_SHOT_MODE;	
	g_camera_cntx.setting.add_frame			= CAMERA_DEFAULT_SETTING_ADD_FRAME;
	g_camera_cntx.setting.cont_shot			= CAMERA_DEFAULT_SETTING_CONT_SHOT;
	g_camera_cntx.setting.delay_timer		= CAMERA_DEFAULT_SETTING_DELAY_TIMER;
	#ifdef COOLSAND_MULTIMEDIA_USE_FLASHFS
	g_camera_cntx.setting.storage				= CAMERA_DEFAULT_SETTING_STORAGE;
	#else
	g_camera_cntx.setting.storage				= CAMERA_SETTING_STORAGE_MEMORY_CARD;  //modified by WeiD
	#endif
	 if(MMI_CARD_DRV > 0)
	{
		g_camera_cntx.setting.storage = CAMERA_SETTING_STORAGE_MEMORY_CARD;
	}
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    else if(MMI_SEC_CARD_DRV > 0)
	{
		g_camera_cntx.setting.storage = CAMERA_SETTING_STORAGE_MEMORY_SEC_CARD;
	}
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
	//add by WeiD
	g_camera_cntx.setting.reserve5         = CAMERA_DEFAULT_SETTING_BRIGHTNESS;  //for brightness
#if defined(__MMI_MAINLCD_220X176__)
	g_camera_cntx.setting.contrast         = CAMERA_DEFAULT_SETTING_CONTRAST;
#else
	g_camera_cntx.setting.reserve6         = CAMERA_DEFAULT_SETTING_CONTRAST;  //for contrast
	//add by WeiD
#endif
	
	g_camera_cntx.setting.iso					= CAMERA_DEFAULT_SETTING_ISO;
	g_camera_cntx.setting.ae_meter			= CAMERA_DEFAULT_SETTING_AE_METER;	
	g_camera_cntx.setting.shutter_pri 		= 0;
	g_camera_cntx.setting.aperture_pri		= 0;

	g_camera_cntx.setting.fx_brightness[0]	= CAMERA_DEFAULT_SETTING_BRIGHTNESS;
	g_camera_cntx.setting.fx_brightness[1]	= CAMERA_DEFAULT_SETTING_BRIGHTNESS;	
	g_camera_cntx.setting.fx_brightness[2]	= CAMERA_DEFAULT_SETTING_BRIGHTNESS;	
	g_camera_cntx.setting.fx_saturation[0]	= CAMERA_DEFAULT_SETTING_SATURATION;	
	g_camera_cntx.setting.fx_saturation[1]	= CAMERA_DEFAULT_SETTING_SATURATION;	
	g_camera_cntx.setting.fx_saturation[2]	= CAMERA_DEFAULT_SETTING_SATURATION;	
	g_camera_cntx.setting.fx_contrast[0]	= CAMERA_DEFAULT_SETTING_CONTRAST;	
	g_camera_cntx.setting.fx_contrast[1]	= CAMERA_DEFAULT_SETTING_CONTRAST;	
	g_camera_cntx.setting.fx_contrast[2]	= CAMERA_DEFAULT_SETTING_CONTRAST;		
	g_camera_cntx.setting.fx_hue[0]			= CAMERA_DEFAULT_SETTING_HUE;	
	g_camera_cntx.setting.fx_hue[1]			= CAMERA_DEFAULT_SETTING_HUE;	
	g_camera_cntx.setting.fx_hue[2]			= CAMERA_DEFAULT_SETTING_HUE;	
	
	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270 )
	{
		g_camera_cntx.osd_rotate = CAMERA_DISPLAY_ROTATE_0;
		g_camera_cntx.previous_osd_rotate = CAMERA_DISPLAY_ROTATE_RESET;
	}
	#endif
	
	mmi_camera_preview_update_capture_size();

	mmi_camera_store_setting();
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_create_file_dir()
* DESCRIPTION
*   create directly from a file path (resursivly)
* PARAMETERS
*	 a  IN    		file path to create    	
* RETURNS
*   S32
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
S32 mmi_camera_create_file_dir(PS8 filepath)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8				buf_createpath[128];
	S8				buf_filepath[FMGR_PATH_BUFFER_SIZE];
	S32			fs_ret;
	S32			str_len;
	S32			cmp_ret;
	S32			i;
	FS_HANDLE	file_handle;

	S8 list_separator[] = {0x2F,0x00,0x00,0x00,};


    mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_create_file_dir()");
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	fs_ret = 0;
	
	file_handle = MMI_FS_Open((const UINT8 *)filepath, FS_OPEN_DIR | FS_READ_ONLY);
	mmi_trace(g_sw_CAMERA, "file_handle = %d", file_handle);
	/* path already exist */
	if(file_handle>=0 || file_handle == FS_FOLDER_HANDLE)
	{
		MMI_FS_Close(file_handle);
		return 0; /* already exist , 0 measn success in FS */
	}
	
	/* create directory if it is not there */
	pfnUnicodeStrcpy((PS8)buf_filepath, (PS8)filepath);
	mmi_trace(g_sw_CAMERA, "pfnUnicodeStrcpy((PS8)buf_filepath, (PS8)filepath)");


	str_len = pfnUnicodeStrlen((PS8)buf_filepath);
	str_len *= ENCODING_LENGTH;
	mmi_trace(g_sw_CAMERA, "str_len = %d", str_len);

	/* skip drive. "C:\" */
	for(i = 3*ENCODING_LENGTH ; i < str_len; i+=2)
	{
		/* wufasong updated 2007.01.23 */
		//cmp_ret = pfnUnicodeStrncmp((PS8)&buf_filepath[i], (PS8)L"\\", 1);
		cmp_ret = pfnUnicodeStrncmp((PS8)&buf_filepath[i], (PS8)list_separator, 1);
		if(cmp_ret == 0)
		{
			/* create dir */

			pfnUnicodeStrncpy((PS8)buf_createpath, (PS8)buf_filepath, i/ENCODING_LENGTH);
			buf_createpath[i]		= '\0';
			buf_createpath[i+1]	= '\0';
			fs_ret = MMI_FS_CreateDir((const UINT8 *)buf_createpath);	
			mmi_trace(g_sw_CAMERA,"mmi_camera_create_file_dir,fs_ret is %d",fs_ret);

			if(fs_ret < 0)
				return fs_ret;
		}
	}

	return fs_ret;
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_get_next_filename()
* DESCRIPTION
*   get next file name for jpeg image
* PARAMETERS
*	 a  OUT    		next filename
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_get_next_filename(PS8 file_buf_p)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8					buffer[25];
	S8					UCS2_buffer[50];
	S8					UCS2_testname[50];
	S8					UCS2_alpha[10];
	S16				error = 0;
	U16				filename_seq_no;
	U16				alpha_count;
	U16				alpha_index;
	FS_HANDLE		file_handle;

	//wufasong added 2007.01.19
	S8 filename_ext_jpg[] = {0x2E,0x00,0x6A,0x00,0x70,0x00,0x67,0x00,0x00,0x00};

    mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_get_next_filename()");
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	

	/* get/store sequentail number */
	ReadValue(NVRAM_CAM_FILENAME_SEQ_NO, &filename_seq_no, DS_SHORT , &error);
	if(filename_seq_no == 0xffff)
	{
		filename_seq_no = 1;	/* init value */
	}
	else 
	{
		filename_seq_no++;
		if(filename_seq_no > 9999)	/* more than 4 digits */
			filename_seq_no = 1;
	}
	WriteValue(NVRAM_CAM_FILENAME_SEQ_NO, &filename_seq_no, DS_SHORT , &error);

	alpha_count		= 'Z'-'A'+1 ;
	alpha_index		= 0;

	sprintf((PS8)buffer, "IMG%04d", filename_seq_no);
	mmi_trace(g_sw_CAMERA, "filename_seq_no = %d", filename_seq_no);
	mmi_trace(g_sw_CAMERA, "buffer = %s", buffer);
	AnsiiToUnicodeString((PS8)UCS2_buffer, (PS8)buffer);	

	while(1)
	{
		sprintf((PS8)buffer, "%c", 'A'+alpha_index);
		mmi_trace(g_sw_CAMERA, "-- %s --", buffer);
		AnsiiToUnicodeString((PS8)UCS2_alpha, (PS8)buffer);

		pfnUnicodeStrcpy(UCS2_testname, UCS2_buffer);
		pfnUnicodeStrcat(UCS2_testname, UCS2_alpha);
		//pfnUnicodeStrcat(UCS2_testname, (PS8)L".jpg"); /* wufasong updated 2007.01.19 */
		pfnUnicodeStrcat(UCS2_testname, filename_ext_jpg);

        mmi_trace(g_sw_CAMERA, "g_camera_cntx.storage_filepath = .....");
//		mmi_trace_hex(g_sw_CAMERA, UCS2Strlen(g_camera_cntx.storage_filepath) * 2, g_camera_cntx.storage_filepath);
//		mmi_trace_hex(g_sw_CAMERA, UCS2Strlen(UCS2_testname) * 2, UCS2_testname);
		//wufasong updated since it is only PHONE storage 2007.01.19
		pfnUnicodeStrcpy((PS8)file_buf_p, (PS8)g_camera_cntx.storage_filepath);
		pfnUnicodeStrcat((PS8)file_buf_p, (PS8)UCS2_testname);
		//pfnUnicodeStrcpy((PS8)file_buf_p, (PS8)UCS2_testname);

		file_handle = MMI_FS_Open((const UINT8 *)file_buf_p, FS_READ_ONLY);
	
		if(file_handle >= 0)
		{
			mmi_trace(g_sw_CAMERA, "file exist");
			//mmi_trace_hex(g_sw_CAMERA, UCS2Strlen(UCS2_testname) * 2, UCS2_testname);
			/* file exist */
			MMI_FS_Close(file_handle);
			alpha_index++;	
		}
		else
		{
			mmi_trace(g_sw_CAMERA, "file not exist");
			/* file not exit. return */
			return;
		}

		/* exit condition - will get a IMGXXXXZ.jpg as file name */
		if(alpha_index >= alpha_count-1)
			return;
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_display_error_popup()
* DESCRIPTION
*   display a popup error message 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
VOID mmi_display_usb_busy(void) 
{
	U16 string_id=0;	
	ClearAllKeyHandler();
	string_id=STR_ID_CAMERA_NOTIFY_WRITE_PROTECTION;
	DisplayPopup((PU8)GetString(string_id), IMG_GLOBAL_WARNING, CAMERA_WARNING_POPUPDURATION, ST_NOTIFYDURATION, WARNING_TONE);


}
VOID mmi_display_gprs_active(void) 
{
	U16 string_id=0;	
	ClearAllKeyHandler();
	string_id=STR_ID_CAMERA_NOTIFY_QQ_BACKGROUND;
	DisplayPopup((PU8)GetString(string_id), IMG_GLOBAL_WARNING, CAMERA_WARNING_POPUPDURATION, ST_NOTIFYDURATION, WARNING_TONE);


}
static void mmi_camera_display_error_popup(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 string_id=0;	

    mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_display_error_popup(error = %d)", g_camera_cntx.last_error);
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* if has error */
	if( g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
	{
		ClearAllKeyHandler();
		switch(g_camera_cntx.last_error)
		{
		case MDI_RES_CAMERA_ERR_DISK_FULL:
			string_id = STR_ID_CAMERA_NOTIFY_DISK_FULL;
			break;
		case MDI_RES_CAMERA_ERR_NO_DISK:
			string_id = STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD;
			break;
		case MDI_RES_CAMERA_ERR_WRITE_PROTECTION:
			string_id = STR_ID_CAMERA_NOTIFY_WRITE_PROTECTION;
			break;
		case MDI_RES_CAMERA_ERR_CAPTURE_FAILED:
			string_id = STR_ID_CAMERA_NOTIFY_CAPTURE_FAILED;
			break;
		case CAMERA_ERR_CARD_DISK_FULL:
			string_id = FMGR_FS_DISK_FULL_TEXT;
			break;
		case CAMERA_ERR_ROOT_DIR_FULL:
			string_id = FMGR_FS_ROOT_DIR_FULL_TEXT;
			break;
		case CAMERA_ERR_WRITE_PROTECT:
			string_id = FMGR_FS_WRITE_PROTECTION_TEXT;
			break;
		case MDI_RES_CAMERA_ERR_HW_NOT_READY:
			string_id = STR_ID_CAMERA_NOTIFY_HW_NOT_READY;
			break;
		case CAMERA_ERR_CORRUPT_FAT:
            #if 1//def TGT_GALLITE_G800,other prj also need this
			string_id =FMGR_FS_DISK_FULL_TEXT;//the fs not return disk full,tmpl solve the warning error FMGR_FS_FAT_ALLOC_ERROR_TEXT;
	     #else
			string_id =FMGR_FS_FAT_ALLOC_ERROR_TEXT;
            #endif
			break;
		case CAMERA_ERR_PHONE_DISK_FULL:
			/* phone disk full, cant create "Photos" folder */
			string_id = FMGR_FS_DISK_FULL_TEXT;
			break;			
		default:
            #ifdef TGT_GALLITE_G800
			string_id =FMGR_FS_DISK_FULL_TEXT;//the fs not return disk full,tmpl solve the warning error FMGR_FS_FAT_ALLOC_ERROR_TEXT;
	     #else
			string_id = STR_ID_CAMERA_NOTIFY_ERROR;
            #endif

			break;
		}

		#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
		DisplayPopupRotated((PU8)GetString(string_id), IMG_GLOBAL_WARNING, CAMERA_WARNING_POPUPDURATION, ST_NOTIFYDURATION, WARNING_TONE,g_camera_cntx.osd_rotate);
		#else
		DisplayPopup((PU8)GetString(string_id), IMG_GLOBAL_WARNING, CAMERA_WARNING_POPUPDURATION, ST_NOTIFYDURATION, WARNING_TONE);
		#endif

		if((g_camera_cntx.last_error ==CAMERA_ERR_PHONE_DISK_FULL)
			||(g_camera_cntx.last_error ==CAMERA_ERR_CORRUPT_FAT)
			||(g_camera_cntx.last_error ==MDI_RES_CAMERA_ERR_HW_NOT_READY)
			||(g_camera_cntx.last_error ==CAMERA_ERR_ROOT_DIR_FULL))
		{
			DeleteNHistory(1);
		}

		g_camera_cntx.last_error = MDI_RES_CAMERA_SUCCEED;
	}	
}




/*****************************************************************************
* FUNCTION
*	 mmi_camera_display_sublcd_error_popup()
* DESCRIPTION
*   display a popup error message 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_display_sublcd_error_popup(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	#ifdef MMI_ON_HARDWARE_P
		mmi_eq_gpio_detect_ind_struct	clam_event;
	#endif /* MMI_ON_HARDWARE_P */

	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   
	/* if has error */
	if( g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
	{
		ClearAllKeyHandler();
		switch(g_camera_cntx.last_error)
		{
			/* these are recoverable errors */
			/* show on sublcd screen and keep previewing */
			case MDI_RES_CAMERA_ERR_DISK_FULL:
			case MDI_RES_CAMERA_ERR_NO_DISK:
			case MDI_RES_CAMERA_ERR_WRITE_PROTECTION:
			case MDI_RES_CAMERA_ERR_CAPTURE_FAILED:	
				/* play warning tone and re-entry preview state */
				playRequestedTone(WARNING_TONE);
				CAMERA_ENTER_STATE(CAMERA_STATE_SUB_PREVIEW);
				break;

			default:
				/* serious error, will exit camera app */
				GoBackHistory();

				/* 
					since clam close if aready intercept by camera app,
					we shall notify framework (gpio.c) to process clam close action 
				*/
				if(g_camera_cntx.is_clam_close)
				{
					#ifdef MMI_ON_HARDWARE_P
						clam_event.gpio_device = EXT_DEV_CLAM_CLOSE;	
						GpioDetectInd(&clam_event);
					#endif /* MMI_ON_HARDWARE_P */
				}
				
				break;
		}
	}
	
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_dummy_func()
* DESCRIPTION
*   dummy function 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_dummy_func(void)
{
	/* empty function */
}


/****************************************************************************
*
*	Camera App                                                   
*
****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_camera_hilight_app()
* DESCRIPTION
*   app menu item hilight hdlr
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __PRJ_WITH_SPICAM__
void mmi_camera_capture_hilight_app(void)
{
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
   
    SetLeftSoftkeyFunction(mmi_camera_entry_app_screen,KEY_EVENT_UP);
}
#endif

void mmi_camera_hilight_app(void)
{
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
   
   
#ifndef __MMI_DISABLE_DUAL_SENSOR__
    extern void camera_SwitchSensorReset();
    camera_SwitchSensorReset();
#endif
    mmi_trace(g_sw_CAMERA,"Excute funs: mmi_camera_hilight_app() is TRUE");

#ifdef __PRJ_WITH_SPICAM__
    SetLeftSoftkeyFunction(mmi_camera_entry_option_menu_screen,KEY_EVENT_UP);
#else
    SetLeftSoftkeyFunction(mmi_camera_entry_app_screen,KEY_EVENT_UP);
#endif

#ifndef __MMI_PROJECT_GS_HK603__
#if defined __MMI_MAINLCD_240X320__  //for greenstone new image resource ,081128. and then modified by kecx on 081211.
#ifdef __PRJ_WITH_SPICAM__
 	SetKeyHandler(mmi_camera_entry_option_menu_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
#else
    SetKeyHandler(mmi_camera_entry_app_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);  	
#endif
#endif
#endif
}
/*****************************************************************************
* FUNCTION
*	 mmi_camera_get_img_size_img_id()
* DESCRIPTION
*   get image string id based on customization 
* PARAMETERS
*	 image_width 	IN			capture image width
*	 image_height 	IN			capture image height
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static U16 mmi_camera_get_img_size_img_id(S32 image_width, S32 image_height)
{
	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_get_img_size_img_id(width = %d, height = %d)", image_width, image_height);
	
#ifdef __MMI_MAINLCD_160X128__
   #if defined (MEM_3216_CAMERA_LARGE_MODE) 
   if(image_width == 1280 && image_height == 1024)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_1280x1024;
	}
	else if(image_width == 640 && image_height == 480)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_640x480;
	}
	else 
    #endif
     if(image_width == 320 && image_height == 240)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_320x240;
	}
	else if(image_width == 220 && image_height == 176)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_176x220;
	}
	else if(image_width == 160 && image_height == 128)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_128x160;
	}		
	else
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_UNDEFINE;
	}
#else
    if(image_width == 2048 && image_height == 1536)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_2048x1536;
	}
	else if(image_width == 1600 && image_height == 1200)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_1600x1200;
	}	
	else if(image_width == 1280 && image_height == 1024)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_1280x1024;
	}
	else if(image_width == 1280 && image_height == 960)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_1280x960;
	}
	else if(image_width == 640 && image_height == 480)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_640x480;
	}
	else if(image_width == 320 && image_height == 240)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_320x240;
	}
	else if(image_width == 160 && image_height == 120)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_160x120;
	}
	else if(image_width == 80 && image_height == 60)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_80x60;
	}
	else if(image_width == 80 && image_height == 56)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_80x56;
	}
	else if(image_width == 240 && image_height == 320)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_240x320;
	}	
	else if((image_width == 176 && image_height == 220) || (image_width == 220 && image_height == 176))
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_176x220;
	}
	else if(image_width == 128 && image_height == 160)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_128x160;
	}
	else if(image_width == 128 && image_height == 128)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_128x128;
	}
	else if(image_width == 96 && image_height == 64)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_96x64;
	}	
	else if(image_width == 128 && image_height == 85)
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_128x85;
	}		
	else
	{
		return IMG_ID_CAMERA_IMAGE_SIZE_UNDEFINE;
	}
 #endif
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_get_shutter_pri_str_id()
* DESCRIPTION
*   get image string id based on customization 
* PARAMETERS
*	 feature_id 	IN			shutter feature id
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SHOT_MODE__
U16	mmi_camera_get_shutter_pri_str_id(S32 feature_id)
{
	switch(feature_id)
	{
		case 4000:
			return STR_ID_CAMERA_SHOT_MODE_SHUTTER_PRI_4S;
		break;

		case 2000:
			return STR_ID_CAMERA_SHOT_MODE_SHUTTER_PRI_2S;
		break;
			
		case 1000:
			return STR_ID_CAMERA_SHOT_MODE_SHUTTER_PRI_1S;
		break;

		case 500:
			return STR_ID_CAMERA_SHOT_MODE_SHUTTER_PRI_05S;
		break;
		case 250:
			return STR_ID_CAMERA_SHOT_MODE_SHUTTER_PRI_025S;
		break;
		default:
			return STR_ID_CAMERA_SHOT_MODE_SHUTTER_PRI_UNDEFINED;
		break;

	}
}
#endif
/*****************************************************************************
* FUNCTION
*	 mmi_camera_get_aperture_pri_str_id()
* DESCRIPTION
*   get image string id based on customization 
* PARAMETERS
*	 feature_id 	IN			aperture feature id
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SHOT_MODE__
U16	mmi_camera_get_aperture_pri_str_id(S32 feature_id)
{
	switch(feature_id)
	{
		case 28:
			return STR_ID_CAMERA_SHOT_MODE_APERTURE_PRI_F28;
		break;

		case 40:
			return STR_ID_CAMERA_SHOT_MODE_APERTURE_PRI_F40;
		break;
			
		case 56:
			return STR_ID_CAMERA_SHOT_MODE_APERTURE_PRI_F56;
		break;

		default:
			return STR_ID_CAMERA_SHOT_MODE_APERTURE_PRI_UNDEFINED;
		break;
	}
}
#endif

/*****************************************************************************
* FUNCTION
*	 mmi_camera_get_img_size_str_id()
* DESCRIPTION
*   get image string id based on customization 
* PARAMETERS
*	 image_width 	IN			capture image width
*	 image_height 	IN			capture image height
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static U16 mmi_camera_get_img_size_str_id(S32 image_width, S32 image_height)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef __MMI_MAINLCD_160X128__
    #if defined (MEM_3216_CAMERA_LARGE_MODE) 
    if(image_width == 1280 && image_height == 1024)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_1280x1024;
	}	
	else if(image_width == 640 && image_height == 480)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_640x480;
	}
	else 
    #endif
     if(image_width == 320 && image_height == 240)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_320x240;
	}	
	else if(image_width == 240 && image_height == 320)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_240x320;
	}	
	else if(image_width == 220 && image_height == 176)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_176x220;
	}
	else if(image_width == 160 && image_height == 128)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_128x160;
	}	
	else
	{
		return STR_ID_CAMERA_IMAGE_SIZE_UNDEFINE;
	}
#else
    if(image_width == 2048 && image_height == 1536)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_2048x1536;
	}
	else if(image_width == 1600 && image_height == 1200)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_1600x1200;
	}	
	else if(image_width == 1280 && image_height == 1024)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_1280x1024;
	}
	else if(image_width == 1280 && image_height == 960)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_1280x960;
	}
	else if(image_width == 640 && image_height == 480)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_640x480;
	}
	else if(image_width == 320 && image_height == 240)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_320x240;
	}
	else if(image_width == 160 && image_height == 120)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_160x120;
	}
	else if(image_width == 80 && image_height == 60)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_80x60;
	}
	else if(image_width == 80 && image_height == 56)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_80x56;
	}
	else if(image_width == 240 && image_height == 320)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_240x320;
	}	
	else if(image_width == 176 && image_height == 220)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_176x220;
	}
	else if(image_width == 128 && image_height == 160)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_128x160;
	}
	else if(image_width == 128 && image_height == 128)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_128x128;
	}
	else if(image_width == 96 && image_height == 64)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_96x64;
	}	
	else if(image_width == 128 && image_height == 85)
	{
		return STR_ID_CAMERA_IMAGE_SIZE_128x85;
	}		
	else
	{
		return STR_ID_CAMERA_IMAGE_SIZE_UNDEFINE;
	}
#endif
}



#if (defined(__CAMERA_OSD_HORIZONTAL__) || defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)) && defined(__CAMERA_FEATURE_TOUCH_SCREEN__)

/*****************************************************************************
* FUNCTION
*	 mmi_camera_update_osd_touch_icon()
* DESCRIPTION
*   update osd touch icons' positions
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_update_osd_touch_icon()
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

		#ifdef __CAMERA_FEATURE_TOUCH_CLICKICON__
			#define INIT_OSD_TOUCH_ICON(a,A)\
			do {\
				if(g_camera_cntx.active_camera_osd_ptr->##a.is_show)\
				{\
					gdi_image_get_dimension_id((U16)(IMG_ID_CAMERA_OSD_##A##_START+1),\
														&g_camera_cntx.touch_##a.width,\
														&g_camera_cntx.touch_##a.height);\
					g_camera_cntx.touch_##a.offset_x = g_camera_cntx.active_camera_osd_ptr->##a.offset_x;\
					g_camera_cntx.touch_##a.offset_y = g_camera_cntx.active_camera_osd_ptr->##a.offset_y;\
					g_camera_cntx.touch_##a.is_press = FALSE;\
				}\
			} while(0);
		#else

			/* 
				if touch screen is enable, but do not want to enable icon click, 
				set icon offset to -1 and size to 0 
			*/
			#define INIT_OSD_TOUCH_ICON(a,A)\
			do {\
				if(g_camera_cntx.active_camera_osd_ptr->##a##.is_show)\
				{\
					g_camera_cntx.touch_##a.width 		= 0;\
					g_camera_cntx.touch_##a.height 	= 0;\
					g_camera_cntx.touch_##a.offset_x 	= -1;\
					g_camera_cntx.touch_##a.offset_y 	= -1;\
					g_camera_cntx.touch_##a.is_press 	= FALSE;\
				}\
			} while(0);
		#endif 

		/* draw icon */
		INIT_OSD_TOUCH_ICON(effect,EFFECT);		
		INIT_OSD_TOUCH_ICON(wb,WB);
		INIT_OSD_TOUCH_ICON(flash,FLASH);
		INIT_OSD_TOUCH_ICON(led_highlight,LED_HIGHLIGHT);
		INIT_OSD_TOUCH_ICON(night,NIGHT);
		if(g_camera_cntx.active_camera_osd_ptr->image_size.is_show)
		{
			gdi_image_get_dimension_id((U16)(IMG_ID_CAMERA_IMAGE_SIZE_320x240),
												&g_camera_cntx.touch_image_size.width,
												&g_camera_cntx.touch_image_size.height);
			g_camera_cntx.touch_image_size.offset_x = g_camera_cntx.active_camera_osd_ptr->image_size.offset_x;
			g_camera_cntx.touch_image_size.offset_y = g_camera_cntx.active_camera_osd_ptr->image_size.offset_y;
			g_camera_cntx.touch_image_size.is_press = FALSE;
		}

		INIT_OSD_TOUCH_ICON(image_qty,IMAGE_QTY);
		INIT_OSD_TOUCH_ICON(cont_shot,CONT_SHOT);
		INIT_OSD_TOUCH_ICON(delay_timer,DELAY_TIMER);
		INIT_OSD_TOUCH_ICON(shot_mode,SHOT_MODE);
		INIT_OSD_TOUCH_ICON(iso,ISO);		
		INIT_OSD_TOUCH_ICON(ae_meter,AE_METER);		
		INIT_OSD_TOUCH_ICON(storage,STORAGE);	

		/* ev inc */
		gdi_image_get_dimension_id((U16)(IMG_ID_CAMERA_OSD_EV_INC),
											&g_camera_cntx.touch_ev_inc.width,
											&g_camera_cntx.touch_ev_inc.height);
		
		g_camera_cntx.touch_ev_inc.offset_x = g_camera_cntx.active_camera_osd_ptr->ev_inc.offset_x;
		g_camera_cntx.touch_ev_inc.offset_y = g_camera_cntx.active_camera_osd_ptr->ev_inc.offset_y;
		g_camera_cntx.touch_ev_inc.is_press = FALSE;

		/* ev dec */
		gdi_image_get_dimension_id((U16)(IMG_ID_CAMERA_OSD_EV_DEC),
											&g_camera_cntx.touch_ev_dec.width,
											&g_camera_cntx.touch_ev_dec.height);
		
		g_camera_cntx.touch_ev_dec.offset_x = g_camera_cntx.active_camera_osd_ptr->ev_dec.offset_x;
		g_camera_cntx.touch_ev_dec.offset_y = g_camera_cntx.active_camera_osd_ptr->ev_dec.offset_y;
		g_camera_cntx.touch_ev_dec.is_press = FALSE;

		/* zoom inc */
		gdi_image_get_dimension_id((U16)(IMG_ID_CAMERA_OSD_ZOOM_INC),
											&g_camera_cntx.touch_zoom_inc.width,
											&g_camera_cntx.touch_zoom_inc.height);
		
		g_camera_cntx.touch_zoom_inc.offset_x = g_camera_cntx.active_camera_osd_ptr->zoom_inc.offset_x;
		g_camera_cntx.touch_zoom_inc.offset_y = g_camera_cntx.active_camera_osd_ptr->zoom_inc.offset_y;
		g_camera_cntx.touch_zoom_inc.is_press = FALSE;

		/* zoom dec */
		gdi_image_get_dimension_id((U16)(IMG_ID_CAMERA_OSD_ZOOM_DEC),
											&g_camera_cntx.touch_zoom_dec.width,
											&g_camera_cntx.touch_zoom_dec.height);
		
		g_camera_cntx.touch_zoom_dec.offset_x = g_camera_cntx.active_camera_osd_ptr->zoom_dec.offset_x;
		g_camera_cntx.touch_zoom_dec.offset_y = g_camera_cntx.active_camera_osd_ptr->zoom_dec.offset_y;
		g_camera_cntx.touch_zoom_dec.is_press = FALSE;

		/* capture */
		gdi_image_get_dimension_id((U16)(IMG_ID_CAMERA_OSD_CAPTURE),
											&g_camera_cntx.touch_capture.width,
											&g_camera_cntx.touch_capture.height);
		
		g_camera_cntx.touch_capture.offset_x = g_camera_cntx.active_camera_osd_ptr->capture.offset_x;
		g_camera_cntx.touch_capture.offset_y = g_camera_cntx.active_camera_osd_ptr->capture.offset_y;
		g_camera_cntx.touch_capture.is_press = FALSE;

}
#endif /* __CAMERA_FEATURE_TOUCH_SCREEN__ */

/*****************************************************************************
* FUNCTION
*	 mmi_camera_init_app()
* DESCRIPTION
*   nit camera application
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_init_app(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/	
	S32 index;
	U16 str_id;
	U16 img_id;
	#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
		S32 	image_height;
	#endif 
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_init_app()");
	
	/* print trace Info */
	CAMERA_TRACE_FUNC_INIT();
	
	SetHiliteHandler(MENU_ID_CAMERA_APP, mmi_camera_hilight_app);
#ifdef __MMI_PROJECT_GS_HK603__
    SetHiliteHandler(MAIN_MENU_CAMERA_MENUID, mmi_camera_hilight_app);
#endif

	#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
		SetHiliteHandler(MAIN_MENU_OPTION_CAMERA_APP, mmi_camera_hilight_app);
	#endif
	
	g_camera_image_is_convert = camera_ImageIsConvert();
	/* init Option Menu */
	mmi_camera_init_option_menu();

	/* init context */
	g_camera_cntx.app_state 		= CAMERA_STATE_EXIT;
	g_camera_cntx.app_prev_state 	= CAMERA_STATE_EXIT;
	g_camera_cntx.app_next_state 	= CAMERA_STATE_EXIT; 

	g_camera_cntx.is_storage_path_changed	= FALSE;	
	g_camera_cntx.is_force_self_capture		= FALSE;
	g_camera_cntx.is_setting_loaded			= FALSE;
	g_camera_cntx.is_mainlcd_resource		= FALSE;
	g_camera_cntx.is_sublcd_resource			= FALSE;	
	g_camera_cntx.is_sublcd_display			= FALSE;
	g_camera_cntx.is_tileshot 					= FALSE;
	g_camera_cntx.is_fullscreen				= FALSE;	
	g_camera_cntx.is_dis_clam_open			= FALSE;
	g_camera_cntx.is_cat65_show				= FALSE;
	g_camera_cntx.is_multi_sound_open		= FALSE;

	g_camera_cntx.base_layer_handle					= GDI_LAYER_EMPTY_HANDLE;
	g_camera_cntx.add_frame_layer_handle			= GDI_LAYER_EMPTY_HANDLE;	
	g_camera_cntx.osd_layer_handle					= GDI_LAYER_EMPTY_HANDLE;
	g_camera_cntx.sublcd_base_layer_handle			= GDI_LAYER_EMPTY_HANDLE;	
	g_camera_cntx.tileshot_result_layer_handle	= GDI_LAYER_EMPTY_HANDLE;
	g_camera_cntx.tileshot_preview_layer_handle	= GDI_LAYER_EMPTY_HANDLE;
	g_camera_cntx.preview_layer_handle				= GDI_LAYER_EMPTY_HANDLE;

	g_camera_cntx.tile_cur_num	= 0;

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	g_camera_cntx.osd_rotate = CAMERA_DISPLAY_ROTATE_270 ;
	#else
	g_camera_cntx.osd_rotate = CAMERA_DISPLAY_ROTATE_0 ;
	#endif

	g_camera_cntx.capture_path = CAPTURE_FROM_MEMORY;
	g_camera_cntx.active_camera_osd_ptr = &g_camera_osd_cntx;
	
	g_camera_cntx.base_UI_device_width  = LCD_WIDTH;
	g_camera_cntx.base_UI_device_height = LCD_HEIGHT; 

	g_camera_cntx.osd_UI_device_width  = LCD_WIDTH;
	g_camera_cntx.osd_UI_device_height = LCD_HEIGHT; 

	mmi_camera_preview_update_capture_size();
	g_camera_cntx.multi_sound_dummy_bg 	= 0;
	g_camera_cntx.multi_sound_capture0 	= 0;
	g_camera_cntx.multi_sound_capture1 	= 0;	
	g_camera_cntx.multi_sound_capture2 	= 0;
	
	#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
		g_camera_cntx.image_size_before_burst = CAMERA_FEATURE_CONT_SHOT_BURST_IMAGE_IDX;
	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */

	memset(g_camera_cntx.hint1_buf, 0, CAMERA_HINT_BUF_SIZE);
	memset(g_camera_cntx.hint2_buf, 0, CAMERA_HINT_BUF_SIZE);	

	#ifdef __CAMERA_FEATURE_SHOT_MODE__
		/* init shot mode shutter priority */
		index = 0;
		#ifdef __CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI_0__
			str_id = mmi_camera_get_shutter_pri_str_id(CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI_0);
			g_camera_cntx.shot_mode_shutter_pri_str_id[index++] = str_id;
		#endif

		#ifdef __CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI_1__
			str_id = mmi_camera_get_shutter_pri_str_id(CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI_1);
			g_camera_cntx.shot_mode_shutter_pri_str_id[index++] = str_id;
		#endif

		#ifdef __CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI_2__
			str_id = mmi_camera_get_shutter_pri_str_id(CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI_2);
			g_camera_cntx.shot_mode_shutter_pri_str_id[index++] = str_id;
		#endif
		
		#ifdef __CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI_3__
			str_id = mmi_camera_get_shutter_pri_str_id(CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI_3);
			g_camera_cntx.shot_mode_shutter_pri_str_id[index++] = str_id;
		#endif

		#ifdef __CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI_4__
			str_id = mmi_camera_get_shutter_pri_str_id(CAMERA_FEATURE_SHOT_MODE_SHUTTER_PRI_4);
			g_camera_cntx.shot_mode_shutter_pri_str_id[index++] = str_id;
		#endif

		/* init shot mode aperture priority */
		index = 0;

		#ifdef __CAMERA_FEATURE_SHOT_MODE_APERTURE_PRI_0__
			str_id = mmi_camera_get_aperture_pri_str_id(CAMERA_FEATURE_SHOT_MODE_APERTURE_PRI_0);
			g_camera_cntx.shot_mode_aperture_pri_str_id[index++] = str_id;
		#endif

		#ifdef __CAMERA_FEATURE_SHOT_MODE_APERTURE_PRI_1__
			str_id = mmi_camera_get_aperture_pri_str_id(CAMERA_FEATURE_SHOT_MODE_APERTURE_PRI_1);
			g_camera_cntx.shot_mode_aperture_pri_str_id[index++] = str_id;
		#endif

		#ifdef __CAMERA_FEATURE_SHOT_MODE_APERTURE_PRI_2__
			str_id = mmi_camera_get_aperture_pri_str_id(CAMERA_FEATURE_SHOT_MODE_APERTURE_PRI_2);
			g_camera_cntx.shot_mode_aperture_pri_str_id[index++] = str_id;
		#endif
	#endif
	
	/* init image size string */
	index = 0;
	
	#ifdef __CAMERA_FEATURE_IMAGE_SIZE_SS__
		str_id = mmi_camera_get_img_size_str_id(CAMERA_FEATURE_IMAGE_SIZE_SS_WIDTH, 
															 CAMERA_FEATURE_IMAGE_SIZE_SS_HEIGHT);
#if defined(MEM_ULC_3216)
		g_camera_cntx.image_size_img_id[index] = IMG_ID_CAMERA_OSD_IMAGE_SIZE_SS;
#else
		img_id = mmi_camera_get_img_size_img_id(CAMERA_FEATURE_IMAGE_SIZE_SS_WIDTH, 
															 CAMERA_FEATURE_IMAGE_SIZE_SS_HEIGHT);
		g_camera_cntx.image_size_img_id[index] = img_id;
#endif
		g_camera_cntx.image_size_str_id[index++] = str_id;

	#endif /* __CAMERA_FEATURE_IMAGE_SIZE_SS__ */

    #ifdef __CAMERA_FEATURE_IMAGE_SIZE_S__
    #if defined(MEM_ULC_3216)
		str_id = mmi_camera_get_img_size_str_id(CAMERA_FEATURE_IMAGE_SIZE_S_WIDTH, 
															 CAMERA_FEATURE_IMAGE_SIZE_S_HEIGHT);
#if defined(MEM_ULC_3216)
		g_camera_cntx.image_size_img_id[index] = IMG_ID_CAMERA_OSD_IMAGE_SIZE_S;
#else
		img_id = mmi_camera_get_img_size_img_id(CAMERA_FEATURE_IMAGE_SIZE_SS_WIDTH, 
															 CAMERA_FEATURE_IMAGE_SIZE_SS_HEIGHT);
		g_camera_cntx.image_size_img_id[index] = img_id;
#endif
		g_camera_cntx.image_size_str_id[index++] = str_id;
    #endif
    #endif /* __CAMERA_FEATURE_IMAGE_SIZE_S__ */

	#ifdef __CAMERA_FEATURE_IMAGE_SIZE_M__
		str_id = mmi_camera_get_img_size_str_id(CAMERA_FEATURE_IMAGE_SIZE_M_WIDTH, 
															 CAMERA_FEATURE_IMAGE_SIZE_M_HEIGHT);
#if defined(MEM_ULC_3216)
		g_camera_cntx.image_size_img_id[index] = IMG_ID_CAMERA_OSD_IMAGE_SIZE_M;
#else
		img_id = mmi_camera_get_img_size_img_id(CAMERA_FEATURE_IMAGE_SIZE_SS_WIDTH, 
															 CAMERA_FEATURE_IMAGE_SIZE_SS_HEIGHT);
		g_camera_cntx.image_size_img_id[index] = img_id;
#endif
		g_camera_cntx.image_size_str_id[index++] = str_id;
	#endif /* __CAMERA_FEATURE_IMAGE_SIZE_M__ */

	#ifdef __CAMERA_FEATURE_IMAGE_SIZE_L__
		str_id = mmi_camera_get_img_size_str_id(CAMERA_FEATURE_IMAGE_SIZE_L_WIDTH, 
															 CAMERA_FEATURE_IMAGE_SIZE_L_HEIGHT);
#if defined(MEM_ULC_3216)
		g_camera_cntx.image_size_img_id[index] = IMG_ID_CAMERA_OSD_IMAGE_SIZE_L;
#else
		img_id = mmi_camera_get_img_size_img_id(CAMERA_FEATURE_IMAGE_SIZE_SS_WIDTH, 
															 CAMERA_FEATURE_IMAGE_SIZE_SS_HEIGHT);
		g_camera_cntx.image_size_img_id[index] = img_id;
#endif
		g_camera_cntx.image_size_str_id[index++] = str_id;
	#endif /* __CAMERA_FEATURE_IMAGE_SIZE_L__ */

	#ifdef __CAMERA_FEATURE_IMAGE_SIZE_LL__
		str_id = mmi_camera_get_img_size_str_id(CAMERA_FEATURE_IMAGE_SIZE_LL_WIDTH, 
															 CAMERA_FEATURE_IMAGE_SIZE_LL_HEIGHT);
	#if defined(MEM_ULC_3216)
		g_camera_cntx.image_size_img_id[index] = IMG_ID_CAMERA_OSD_IMAGE_SIZE_LL;
	#else
		img_id = mmi_camera_get_img_size_img_id(CAMERA_FEATURE_IMAGE_SIZE_SS_WIDTH, 
															 CAMERA_FEATURE_IMAGE_SIZE_SS_HEIGHT);

		g_camera_cntx.image_size_img_id[index] = img_id;
	#endif
		g_camera_cntx.image_size_str_id[index++] = str_id;
	#endif /* __CAMERA_FEATURE_IMAGE_SIZE_LL__ */


	/* init OSD menu filler */
	#ifdef  __CAMERA_FEATURE_OSD_MENU__

		g_cam_ui_poplist.highlight_color = 
			GDI_RGB_TO_BUFFER_FORMAT(	g_camera_osd_menu_cntx.poplist_highlight_color.r,\
												g_camera_osd_menu_cntx.poplist_highlight_color.g,\
												g_camera_osd_menu_cntx.poplist_highlight_color.b);
		
		g_cam_ui_poplist.bg_color = 
			GDI_RGB_TO_BUFFER_FORMAT(	g_camera_osd_menu_cntx.poplist_bg_color.r,\
												g_camera_osd_menu_cntx.poplist_bg_color.g,\
												g_camera_osd_menu_cntx.poplist_bg_color.b);

		
		g_camera_cntx.is_iconlist_init 	= FALSE;
		g_camera_cntx.is_poplist_init 	= FALSE;
		g_camera_cntx.is_card_present		= TRUE;
	
	#endif /* __CAMERA_FEATURE_OSD_MENU__ */


	/* init touch screen related items */
	#ifdef __CAMERA_FEATURE_TOUCH_SCREEN__

		#ifdef __CAMERA_FEATURE_TOUCH_CLICKICON__
			#define INIT_OSD_TOUCH_ICON(a,A)\
			do {\
				if(g_camera_cntx.active_camera_osd_ptr->a.is_show)\
				{\
					gdi_image_get_dimension_id((U16)(IMG_ID_CAMERA_OSD_##A##_START+1),\
														&g_camera_cntx.touch_##a.width,\
														&g_camera_cntx.touch_##a.height);\
					g_camera_cntx.touch_##a.offset_x = g_camera_cntx.active_camera_osd_ptr->a.offset_x;\
					g_camera_cntx.touch_##a.offset_y = g_camera_cntx.active_camera_osd_ptr->a.offset_y;\
					g_camera_cntx.touch_##a.is_press = FALSE;\
				}\
			} while(0);
		#else
			/* 
				if touch screen is enable, but do not want to enable icon click, 
				set icon offset to -1 and size to 0 
			*/
			#define INIT_OSD_TOUCH_ICON(a,A)\
			do {\
				if(g_camera_cntx.active_camera_osd_ptr->a.is_show)\
				{\
					g_camera_cntx.touch_##a.width 		= 0;\
					g_camera_cntx.touch_##a.height 	= 0;\
					g_camera_cntx.touch_##a.offset_x 	= -1;\
					g_camera_cntx.touch_##a.offset_y 	= -1;\
					g_camera_cntx.touch_##a.is_press 	= FALSE;\
				}\
			} while(0);
		#endif 

		/* draw icon */
		INIT_OSD_TOUCH_ICON(effect,EFFECT);		
		INIT_OSD_TOUCH_ICON(wb,WB);
		INIT_OSD_TOUCH_ICON(flash,FLASH);
		INIT_OSD_TOUCH_ICON(led_highlight,LED_HIGHLIGHT);
		INIT_OSD_TOUCH_ICON(night,NIGHT);
		if(g_camera_cntx.active_camera_osd_ptr->image_size.is_show)
		{
			gdi_image_get_dimension_id((U16)(IMG_ID_CAMERA_IMAGE_SIZE_320x240),
												&g_camera_cntx.touch_image_size.width,
												&g_camera_cntx.touch_image_size.height);
			g_camera_cntx.touch_image_size.offset_x = g_camera_cntx.active_camera_osd_ptr->image_size.offset_x;
			g_camera_cntx.touch_image_size.offset_y = g_camera_cntx.active_camera_osd_ptr->image_size.offset_y;
			g_camera_cntx.touch_image_size.is_press = FALSE;
		}

		INIT_OSD_TOUCH_ICON(image_qty,IMAGE_QTY);
		INIT_OSD_TOUCH_ICON(cont_shot,CONT_SHOT);
		INIT_OSD_TOUCH_ICON(delay_timer,DELAY_TIMER);
		INIT_OSD_TOUCH_ICON(shot_mode,SHOT_MODE);
		INIT_OSD_TOUCH_ICON(iso,ISO);		
		INIT_OSD_TOUCH_ICON(ae_meter,AE_METER);		
		INIT_OSD_TOUCH_ICON(storage,STORAGE);	
#if defined(__MMI_MAINLCD_220X176__)
		INIT_OSD_TOUCH_ICON(contrast,CONTRAST);
		g_camera_cntx.touch_contrast.width = 13;//delete this line when image resource added.
		g_camera_cntx.touch_contrast.height = 13;//delete this line when image resource added.
#endif
		/* ev inc */
		gdi_image_get_dimension_id((U16)(IMG_ID_CAMERA_OSD_EV_INC),
											&g_camera_cntx.touch_ev_inc.width,
											&g_camera_cntx.touch_ev_inc.height);
		
		g_camera_cntx.touch_ev_inc.offset_x = g_camera_cntx.active_camera_osd_ptr->ev_inc.offset_x;
		g_camera_cntx.touch_ev_inc.offset_y = g_camera_cntx.active_camera_osd_ptr->ev_inc.offset_y;
		g_camera_cntx.touch_ev_inc.is_press = FALSE;

		/* ev dec */
		gdi_image_get_dimension_id((U16)(IMG_ID_CAMERA_OSD_EV_DEC),
											&g_camera_cntx.touch_ev_dec.width,
											&g_camera_cntx.touch_ev_dec.height);
		
		g_camera_cntx.touch_ev_dec.offset_x = g_camera_cntx.active_camera_osd_ptr->ev_dec.offset_x;
		g_camera_cntx.touch_ev_dec.offset_y = g_camera_cntx.active_camera_osd_ptr->ev_dec.offset_y;
		g_camera_cntx.touch_ev_dec.is_press = FALSE;

		/* zoom inc */
		gdi_image_get_dimension_id((U16)(IMG_ID_CAMERA_OSD_ZOOM_INC),
											&g_camera_cntx.touch_zoom_inc.width,
											&g_camera_cntx.touch_zoom_inc.height);
		
		g_camera_cntx.touch_zoom_inc.offset_x = g_camera_cntx.active_camera_osd_ptr->zoom_inc.offset_x;
		g_camera_cntx.touch_zoom_inc.offset_y = g_camera_cntx.active_camera_osd_ptr->zoom_inc.offset_y;
		g_camera_cntx.touch_zoom_inc.is_press = FALSE;

		/* zoom dec */
		gdi_image_get_dimension_id((U16)(IMG_ID_CAMERA_OSD_ZOOM_DEC),
											&g_camera_cntx.touch_zoom_dec.width,
											&g_camera_cntx.touch_zoom_dec.height);
		
		g_camera_cntx.touch_zoom_dec.offset_x = g_camera_cntx.active_camera_osd_ptr->zoom_dec.offset_x;
		g_camera_cntx.touch_zoom_dec.offset_y = g_camera_cntx.active_camera_osd_ptr->zoom_dec.offset_y;
		g_camera_cntx.touch_zoom_dec.is_press = FALSE;

		/* capture */
		gdi_image_get_dimension_id((U16)(IMG_ID_CAMERA_OSD_CAPTURE),
											&g_camera_cntx.touch_capture.width,
											&g_camera_cntx.touch_capture.height);
		
		g_camera_cntx.touch_capture.offset_x = g_camera_cntx.active_camera_osd_ptr->capture.offset_x;
		g_camera_cntx.touch_capture.offset_y = g_camera_cntx.active_camera_osd_ptr->capture.offset_y;
		g_camera_cntx.touch_capture.is_press = FALSE;

	#endif /* __CAMERA_FEATURE_TOUCH_SCREEN__ */
	

	#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
		g_camera_cntx.iconbar_start_idx		= 0;
		g_camera_cntx.iconbar_selected_idx	= (g_camera_cntx.active_camera_osd_ptr->iconbar.icon_per_page>>1);
		g_camera_cntx.iconbar_item_per_page = g_camera_cntx.active_camera_osd_ptr->iconbar.icon_per_page;

		gdi_image_get_dimension_id(IMG_ID_CAMERA_ICONBAR_LEFT_ARROW,
											&g_camera_cntx.iconbar_arrow_width,
											&image_height);

		gdi_image_get_dimension_id(IMG_ID_CAMERA_ICONBAR_ITEM,
											&g_camera_cntx.iconbar_item_width,
											&image_height);

		index = 0;

		#define INIT_ICONBAR_ITEM(__a__, __A__)\
		do {\
			if(g_camera_cntx.active_camera_osd_ptr->##__a__##.is_show)\
			{\
				g_camera_cntx.iconbar_item[index] = CAMERA_ICONBAR_##__A__;\
				index++;\
			}\
		} while(0);

		INIT_ICONBAR_ITEM(zoom,ZOOM);
		INIT_ICONBAR_ITEM(ev,EV);
		INIT_ICONBAR_ITEM(effect,EFFECT);		
		INIT_ICONBAR_ITEM(wb,WB);
		INIT_ICONBAR_ITEM(flash,FLASH);
		INIT_ICONBAR_ITEM(led_highlight,LED_HIGHLIGHT);
		INIT_ICONBAR_ITEM(night,NIGHT);
		INIT_ICONBAR_ITEM(closeup,CLOSEUP);		
		INIT_ICONBAR_ITEM(cont_shot,CONT_SHOT);		
		INIT_ICONBAR_ITEM(delay_timer,DELAY_TIMER);
		INIT_ICONBAR_ITEM(add_frame,ADD_FRAME);
		INIT_ICONBAR_ITEM(image_size,IMAGE_SIZE);
		INIT_ICONBAR_ITEM(image_qty,IMAGE_QTY);
		INIT_ICONBAR_ITEM(storage,STORAGE);

		g_camera_cntx.iconbar_item_count = index;
			
	#endif /* __CAMERA_FEATURE_ICONBAR_PANEL__ */

	#ifdef __MMI_SCREEN_SNAPSHOT__
		g_camera_cntx.is_screen_snapshot = FALSE;
	#endif /* __MMI_SCREEN_SNAPSHOT__ */

}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_app_screen()
* DESCRIPTION
*   entry camera application screen
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
extern int checkUsbstate(void);
extern VOID mmi_display_usb_busy(void) ;
extern BOOL is_phb_init_done(void);

#ifdef __MMI_ACCEL_OPP_SUPPORT__
extern BOOL mmi_opp_is_connected(void);
#endif
//add for QQ 2010-0203 
BOOL  check_AppGprsStatus(void)
{
#ifdef JATAAYU_SUPPORT
if(jMMS_IsMMSAppOrDownloadActive()||jMMS_IsWapConnected())
	{
		DisplayPopup((PU8)GetString(STR_ID_CAMERA_NOTIFY_GPRS_ACTIVE) ,
			IMG_GLOBAL_PROGRESS, CAMERA_WARNING_POPUPDURATION,
			UI_POPUP_NOTIFYDURATION_TIME,
			WARNING_TONE);
				return FALSE;
	}
#endif
return TRUE;
}
//add end
extern BOOL checkVMOn(void);
void smarchbox_bg_askquit(void);
extern VOID mmi_display_gprs_active(void) ;
extern BOOL mmi_frm_sms_is_ready(void);
void mmi_camera_entry_app_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8				*guiBuffer;
	U16			cur_storage;
	S32			create_result;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

    if(HandleLowBatteryProcess())
    {
        mmi_trace(1,"low battery in camera");
        return ;
    }
   
#ifndef MEMD_FLASH_IS_ADMUX //add  by licheng for t08 target,because the normal falsh is slowly,and camera can't work after poweron
#if defined(__PROJECT_GALLITE_C01__)
	if(mmi_frm_sms_is_ready() == FALSE)
#else
	if(!is_phb_init_done())
#endif
	{
	    //开机初始化 stringid =25803  modified by renyh  2008/8/1
		DisplayPopup((PU8)GetString(STR_GLOBAL_POWER_ON_PROCESSING) ,IMG_GLOBAL_PROGRESS/*IMG_GLOBAL_ACTIVATED*/, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);//modified by lin for bug 6761@20070929
		return;
	}
#endif

 #ifdef __MMI_ACCEL_OPP_SUPPORT__
	if(mmi_opp_is_connected())
	{
		DisplayPopup((PU8)GetString(STR_ID_CAMERA_NOTIFY_WRITE_PROTECTION) ,
			IMG_GLOBAL_PROGRESS, CAMERA_WARNING_POPUPDURATION,
			UI_POPUP_NOTIFYDURATION_TIME,
			WARNING_TONE);
		return;
	}
#endif

#if !defined(MMI_ON_WIN32)
 #ifdef JATAAYU_SUPPORT
	if(
	#ifdef JATAAYU_SUPPORT_MMS
		jMMS_IsMMSAppOrDownloadActive()
	#endif	
	#ifdef JATAAYU_SUPPORT_WAP
	#ifdef JATAAYU_SUPPORT_MMS
		||
	#endif
		jMMS_IsWapConnected()
	#endif
		)
	{
		DisplayPopup((PU8)GetString(STR_ID_CAMERA_NOTIFY_GPRS_ACTIVE) ,
			IMG_GLOBAL_PROGRESS, CAMERA_WARNING_POPUPDURATION,
			UI_POPUP_NOTIFYDURATION_TIME,
			WARNING_TONE);
				return;
	}

#endif
#endif
#if 0
	if(!is_phb_init_done())
	{
	    //开机初始化 stringid =25803  modified by renyh  2008/8/1
		DisplayPopup((PU8)GetString(STR_GLOBAL_POWER_ON_PROCESSING) ,IMG_GLOBAL_PROGRESS/*IMG_GLOBAL_ACTIVATED*/, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);//modified by lin for bug 6761@20070929
		return;
	}
#endif
	if(checkVMOn())
	{
	    mmi_show_qq_select_confirm(mmi_camera_entry_app_screen);

		return;
	}
	if(checkUsbstate())
	{
		mmi_display_usb_busy();
		return;
	}
	#if 0
	/* user press camera key when clam close in Idle screen, ignore it */
	#if defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__) && defined(MMI_ON_HARDWARE_P)

		if(IsClamClose())
		{
			if(GetActiveScreenId() == IDLE_SCREEN_ID)
			{
				return;
			}
			else
			{
				DisplayIdleScreen();
				return;
			}
		}
		else if(GetActiveScreenId() != SCR_ID_CAMERA_OPTION )
			g_camera_cntx.is_clam_close=FALSE;
	#endif /* __MMI_CLAMSHELL__ && !__MMI_SLIDE__ && MMI_ON_HARDWARE_P */
	#endif

	/* if clam is opened and not enter from camera option menu */
	if(GetActiveScreenId() != SCR_ID_CAMERA_OPTION && g_camera_cntx.is_clam_close==FALSE)
	{
		/* reset force enter sub flag */
		g_camera_cntx.is_force_self_capture	= FALSE;
	}

	/* set TV out mode */
	/* if is full screen multimedia mode, change to tvout mode2 */
	#ifdef __MMI_TVOUT__
		if(mdi_tvout_is_enable())
		{
			if(mmi_phnset_tvout_get_style() == SETTING_TVOUT_STYLE_FULLSCREEN)
			{
				/* if is fullscreen, we shall set to owner to camera here, to avoid GDI's update */
				mdi_tvout_set_owner(MDI_TV_OWNER_CAMERA);				

				/* start mode 2 - full screen preview update */
				mdi_tvout_set_mode(	MDI_TV_MODE_CAMERA_PREVIEW,
											0,						/* layer_width */
											0);					/* layer_height */
			}
		}
	#endif /* __MMI_TVOUT__ */

	/* debug info */
	CAMERA_TRACE_FUNC_ENTER_APP();
	if(g_camera_cntx.app_state !=CAMERA_STATE_EXIT)
	{
		mmi_trace(g_sw_CAMERA,"mmi_camera_entry_app_screen,state is not CAMERA_STATE_EXIT,return");
		return ;
	}
	CAMERA_CHECK_STATE(CAMERA_STATE_EXIT);

	/************ entry screen ************/
	
	/* entry new screen */
	EntryNewScreen(SCR_ID_CAMERA_APP, mmi_camera_exit_app_screen, mmi_camera_entry_app_screen, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_CAMERA_APP);

#ifdef __MMI_STOP_SIGNAL_REPORT__
	NetWorkSignalStopReport(0); //Stop the signal report for "No more Envolope" issue
#endif

	/************ init/config framework's environment  ************/
	
	/* entry full screen app */
	entry_full_screen();
	
	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();
	
	/* stop bg music */
	mdi_audio_suspend_background_play();

	/* stop MMI sleep */
	TurnOnBacklight(0);

	/* force all playing keypad tone off */
	AudioStopReq(GetCurKeypadTone());

	/* disable key pad tone */
	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);

	/* disalbe align timer  */
	UI_disable_alignment_timers();

	/* stop LED patten */
	GpioCtrlReq(MMIAPI_DEV_CTRL_GET);

	/* enable multi-layer */
	gdi_layer_multi_layer_enable();	


	/*********** init layer var ************/
	/* get mainlcd based layer handle */
	gdi_layer_get_base_handle(&g_camera_cntx.base_layer_handle);

	/* get sublcd base layer handle */
	
	#ifdef __MMI_SUBLCD__
		gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
		gdi_layer_get_base_handle(&g_camera_cntx.sublcd_base_layer_handle);
		gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
	#endif 

	/************ draw sublcd and hook msg hdlr ************/

	#ifdef __MMI_SUBLCD__
		/* entry sublcd */
		if(!g_camera_cntx.is_sublcd_display)
		{
			ForceSubLCDScreen(mmi_camera_entry_sublcd_screen);
		}
	#endif 

	#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
		SetKeyHandler(mmi_camera_toggle_self_capture, CAMERA_TOGGLE_SELF_CAPTURE_KEY, KEY_EVENT_DOWN);
	#endif 

	#if defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__) && defined(MMI_ON_HARDWARE_P)
		/* register event hdlr */
		SetInterruptEventHandler(mmi_camera_clam_close_event_hdlr, NULL, MSG_ID_MMI_EQ_GPIO_DETECT_IND);
	#endif /* __MMI_CLAMSHELL__ !__MMI_SLIDE__ && MMI_ON_HARDWARE_P */

	/************ init camera app environment ************/

	/* init some flags */
	g_camera_cntx.last_error 			= MDI_RES_CAMERA_SUCCEED;
	g_camera_cntx.is_dis_clam_open	= FALSE;
	g_camera_cntx.tile_cur_num			= 0;

	g_camera_cntx.lcm_rotate         = CAMERA_LCM_MAINLCD_ROTATE;

	if(!g_camera_cntx.is_force_self_capture)
	{
		g_camera_cntx.is_clam_close = FALSE;
	}

	memset(g_camera_cntx.hint1_buf, 0, CAMERA_HINT_BUF_SIZE);
	memset(g_camera_cntx.hint2_buf, 0, CAMERA_HINT_BUF_SIZE);	

	/* load Setting */
	mmi_camera_load_setting();
//add by WeiD
	{
		S32 judge_drv = 0;
		judge_drv 	= MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE);
		mmi_trace(g_sw_CAMERA,"mmi_camera_entry_app_screen,judge_drv is %d",judge_drv);
		if(judge_drv < 0)
		{
			if(g_camera_cntx.setting.storage == CAMERA_SETTING_STORAGE_PHONE)
			{
			}
			else if(g_camera_cntx.setting.storage == CAMERA_SETTING_STORAGE_MEMORY_CARD)
			{
				g_camera_cntx.setting.storage =CAMERA_SETTING_STORAGE_PHONE;
			}
		}
	}	
//add by WeiD
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    {
		S32 judge_sec_drv = 0;
		judge_sec_drv 	= MMI_FS_GetDrive(FS_DRIVE_TFCARD_SECOND, 1, FS_NO_ALT_DRIVE);
		mmi_trace(g_sw_CAMERA,"mmi_camera_entry_app_screen,judge_sec_drv is %d",judge_sec_drv);
		if(judge_sec_drv < 0)
		{
			if(g_camera_cntx.setting.storage == CAMERA_SETTING_STORAGE_PHONE)
			{
			}
			else if(g_camera_cntx.setting.storage == CAMERA_SETTING_STORAGE_MEMORY_SEC_CARD)
			{
				g_camera_cntx.setting.storage =CAMERA_SETTING_STORAGE_PHONE;
			}
		}
	}
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
	/* allocate storage path buffer */
	g_camera_cntx.storage_filepath = coolsand_UI_malloc(FMGR_PATH_BUFFER_SIZE);
	memset(g_camera_cntx.storage_filepath, 0, FMGR_PATH_BUFFER_SIZE);
	
	/* check if path is valid or not. if not will pop up a screen after the preview screen is show */
	cur_storage = g_camera_cntx.setting.storage;
	mmi_trace(g_sw_CAMERA,"mmi_camera_entry_app_screen,cur_storage is %d",cur_storage);
	if(mmi_camera_check_and_restore_valid_drv())
	{
		mmi_trace(g_sw_CAMERA,"g_camera_cntx.storage_filepath = %p",g_camera_cntx.storage_filepath);
		/* drv is valid, create directory */
		mmi_camera_get_storage_file_path(g_camera_cntx.storage_filepath);
		create_result = mmi_camera_create_file_dir(g_camera_cntx.storage_filepath);

		mmi_trace(g_sw_CAMERA,"Create_result = %ld",create_result);
		/* create directory failed */
		if(create_result == FS_WRITE_PROTECTION)
		{
			g_camera_cntx.last_error 					= CAMERA_ERR_WRITE_PROTECT;

			/* has error - restore to phone */
			g_camera_cntx.setting.storage				= CAMERA_SETTING_STORAGE_PHONE;	
			g_camera_cntx.is_storage_path_changed 	= TRUE;
		}
		else if(create_result == FS_DISK_FULL)
		{
			if(cur_storage == CAMERA_SETTING_STORAGE_PHONE)
			{
				g_camera_cntx.last_error 					= CAMERA_ERR_PHONE_DISK_FULL;
			}
			else
			{
				/* storage is card */
				g_camera_cntx.last_error 					= CAMERA_ERR_CARD_DISK_FULL;

				/* has error - restore to phone */
				g_camera_cntx.setting.storage				= CAMERA_SETTING_STORAGE_PHONE;	
				g_camera_cntx.is_storage_path_changed 	= TRUE;
			}
		}
		else if(create_result == FS_ROOT_DIR_FULL)
		{
			if(cur_storage == CAMERA_SETTING_STORAGE_PHONE)
			{
				g_camera_cntx.last_error 					= CAMERA_ERR_ROOT_DIR_FULL;
			}
			else
			{
				/* storage is card */
				g_camera_cntx.last_error 					= CAMERA_ERR_ROOT_DIR_FULL;

				/* has error - restore to phone */
				g_camera_cntx.setting.storage				= CAMERA_SETTING_STORAGE_PHONE;	
				g_camera_cntx.is_storage_path_changed 	= TRUE;
			}
		}
		else if(create_result<0)
		{
			if(cur_storage == CAMERA_SETTING_STORAGE_PHONE)
			{
				g_camera_cntx.last_error 					= CAMERA_ERR_CORRUPT_FAT;
			}
			else
			{
			/* storage is card */
				g_camera_cntx.last_error 					= CAMERA_ERR_CORRUPT_FAT;

				/* has error - restore to phone */
				g_camera_cntx.setting.storage				= CAMERA_SETTING_STORAGE_PHONE;	
				g_camera_cntx.is_storage_path_changed 	= TRUE;
			}
			
		}
		
	}
	else
	{
		/* drv no valid, means card is plugged out (or phone FAT corrupted) */
		
		/* if storage before restoration is already in phone, means FAT is corrupted */
		if(cur_storage == CAMERA_SETTING_STORAGE_PHONE) 
		{
			g_camera_cntx.last_error = CAMERA_ERR_CORRUPT_FAT;
		}
		else
		{
			g_camera_cntx.last_error = MDI_RES_CAMERA_ERR_NO_DISK;
		}
	}

	mmi_trace(g_sw_CAMERA,"g_camera_cnx.last_error = %ld",g_camera_cntx.last_error);

	/* each time newly enter camera, zoom will be reset to default */
	if(guiBuffer == NULL)
	{
		g_camera_cntx.setting.zoom_idx = CAMERA_DEFAULT_SETTING_ZOOM_IDX;
	}

	/* power on and init hardware module */
	if(g_camera_cntx.last_error == MDI_RES_CAMERA_SUCCEED)
	{
            mmi_trace(g_sw_CAMERA,"Excute funs: mdi_camera_power_on() is true");
#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
            CSD_UI_Free_bmpbuf();
#endif        
		g_camera_cntx.last_error = mdi_camera_power_on();
		mmi_trace(g_sw_CAMERA," mdi_camera_power_on() g_camera_cntx.last_error = %ld",g_camera_cntx.last_error);
		//SetProtocolEventHandler(mmi_camera_preview_ind_handler, MSG_ID_MEDIA_CAM_MD_WORK_IND);
	}

	/* create preview resource */
	mmi_camera_preview_create_resource();

	
	#if defined(__CAMERA_OSD_HORIZONTAL__) 
	not use 
	/* configuare camera*/
		g_camera_cntx.previous_osd_rotate = CAMERA_DISPLAY_ROTATE_RESET;
	/* check orientation between sensor and lcm */
		mmi_camera_check_sensor_orientation();
	/* check osd orientation */
		mmi_camera_check_osd_orientation();

	#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
		if(g_camera_cntx.setting.cont_shot == CAMERA_SETTING_CONT_SHOT_TILE)
			mmi_camera_preview_update_capture_size();
	#endif
	#endif

	/************ enter next state ************/
	/* entry preview */
	mmi_trace(g_sw_CAMERA,"Camera after last_error = %d",g_camera_cntx.last_error);

	if( (g_camera_cntx.is_force_self_capture) && 
		 (g_camera_cntx.last_error == MDI_RES_CAMERA_SUCCEED)) /* no error, if has error, will enter main then show error popup */
	{
		/* force to enter sublcd preview from option menu */
		#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
			g_camera_cntx.is_force_self_capture = FALSE;
			CAMERA_ENTER_STATE(CAMERA_STATE_SUB_PREVIEW);
		#else
			MMI_ASSERT(0);
		#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */
	}
	else if(g_camera_cntx.last_error == MDI_RES_CAMERA_SUCCEED) //chenhe add
	{
		CAMERA_ENTER_STATE(CAMERA_STATE_PREVIEW);
	}
	else
       {
		mmi_camera_display_error_popup();
       }

	
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_app_screen()
* DESCRIPTION
*   exit camera application screen
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_exit_app_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_exit_app_screen()");
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* print debug info */
    /*Jinzh Add Start For 6225 Ver: TBM780  on 2007-8-14 10:7 */
#ifdef __FF_AUDIO_SET__
    cameraAppOnOff = FALSE;
#endif
    /*Jinzh Add End  For 6225 Ver: TBM780  on 2007-8-14 10:7 */
	CAMERA_TRACE_FUNC_EXIT_APP();
	ClearProtocolEventHandler(MSG_ID_MEDIA_CAM_MD_WORK_IND);

	/* exit camera state */
	CAMERA_ENTER_STATE(CAMERA_STATE_EXIT);

	/* stop capture and stop dummy bg midi for multi-channel sound */
	#if defined(__CAMERA_FEATURE_CONT_SHOT_BURST__) && defined(__CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__)
		mmi_camera_burstshot_close_multi_sound();
		coolsand_UI_cancel_timer(mmi_camera_burstshot_close_multi_sound);
	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ && __CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__ */

	/* stop hide hint timer */
	coolsand_UI_cancel_timer(mmi_camera_preview_hide_hint);
	
#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_stop_app_watchdog();
    mmi_camera_stop_delete_confirm_watchdog();
#endif /* __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__ */

	/* shut down camera */
	mdi_camera_power_off();
	/*stop continus shot timer*/
#if defined(__CAMERA_FEATURE_CONT_SHOT__)
	coolsand_UI_cancel_timer(mmi_camera_contshot_cyclic);
#endif
	set_multicaputure(0);
	/* resume alignment timer */
	UI_enable_alignment_timers();

	/* resume LED patten */
	GpioCtrlReq(MMIAPI_DEV_CTRL_RELEASE);

	/* let MMI can sleep */
	TurnOffBacklight();

	/* enable multi-layer */
	gdi_layer_multi_layer_disable();	

	/* resume background audio */
	mdi_audio_resume_background_play();

	/* re-enable keypad tone */
	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);

	/* free buffer */
	coolsand_UI_free(g_camera_cntx.storage_filepath);
	g_camera_cntx.storage_filepath = NULL;

	#ifdef __MMI_SUBLCD__
		/* exit sublcd */
		if(g_camera_cntx.is_sublcd_display)
		{
			GoBackSubLCDHistory();
			g_camera_cntx.is_sublcd_display = FALSE;
		}
	#endif /* __MMI_SUBLCD__ */

	#if defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__) && defined(MMI_ON_HARDWARE_P)
		/* un-hook clam close msg */
		ClearInterruptEventHandler(MSG_ID_MMI_EQ_GPIO_DETECT_IND);
	#endif /* __MMI_CLAMSHELL__ && !__MMI_SLIDE__ && MMI_ON_HARDWARE_P */	


	#if defined(__CAMERA_OSD_HORIZONTAL__) 
	if(g_camera_cntx.lcm_rotate == CAMERA_DISPLAY_ROTATE_90||  g_camera_cntx.lcm_rotate == CAMERA_DISPLAY_ROTATE_270)
	{
		gdi_layer_push_and_set_active(g_camera_cntx.base_layer_handle);
		gdi_layer_clear_background(GDI_COLOR_BLACK);
		gdi_layer_set_rotate(CAMERA_DISPLAY_ROTATE_0);
		gdi_layer_resize(LCD_WIDTH, LCD_HEIGHT);
		gdi_layer_pop_and_restore_active();
	}
	#endif
	mmi_camera_preview_free_preview_resource();

	/* free preview resource */
	mmi_camera_preview_free_resource();

	/* store camera setting back to NVRAM */
	mmi_camera_store_setting();

	/* this will force title status to redraw */
	entry_full_screen();


	/* restore TV out mode */
	#ifdef __MMI_TVOUT__
		if(mdi_tvout_is_enable())
		{
			if(mmi_phnset_tvout_get_style() == SETTING_TVOUT_STYLE_FULLSCREEN)
			{
				/* restore back to GDI */
				mdi_tvout_set_owner(MDI_TV_OWNER_GDI);			
					
				/* restore back to start mode1 */
				mdi_tvout_set_mode(	MDI_TV_MODE_LCD_SCREEN,
											0,						/* layer_width */
											0);					/* layer_height */
			}
		}
	#endif /* __MMI_TVOUT__ */

#ifdef __MMI_STOP_SIGNAL_REPORT__
	NetWorkSignalStopReport(1); //Start the signal report because it was stoped at entry function
#endif

}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_enter_state()
* DESCRIPTION
*   enter next state 
* PARAMETERS
*	 state 	IN		state to enter
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_enter_state(camera_state_enum state)
{
	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_enter_state(%d)", state);
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_camera_cntx.app_prev_state 	= g_camera_cntx.app_state;
	g_camera_cntx.app_state 		= state; 
	
	switch(state)
	{
		case CAMERA_STATE_EXIT:
			/* do nothing */
        coolsand_UI_cancel_timer(mmi_camera_preview_wait_key7_hdlr);  
            ClearAllKeyHandler();
			break;
	
		/* main lcd */
		case CAMERA_STATE_PREVIEW:
			mmi_camera_entry_preview_state();
			break;

#if defined(__CAMERA_FEATURE_DELAY_TIMER__)
		case CAMERA_STATE_COUNTDOWN:
			mmi_camera_entry_countdown_state();
			break;
#endif

#if defined(__CAMERA_FEATURE_CONT_SHOT__)
		case CAMERA_STATE_CONTSHOT:
			mmi_camera_entry_contshot_state();
			break;
#endif

			
	#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
	
		case CAMERA_STATE_BURSTSHOT:
			mmi_camera_entry_burstshot_capture_state();
			break;
			
		case CAMERA_STATE_BURSTSHOT_SAVE_CONFIRM:
			mmi_camera_entry_burstshot_save_confirm_state();
			break;

		case CAMERA_STATE_BURSTSHOT_SAVING:
			mmi_camera_entry_burstshot_saving_state();
			break;
			
	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */

		case CAMERA_STATE_SAVE_DONE:
			mmi_camera_entry_save_done_state();			
			break;

		case CAMERA_STATE_SAVE_CONFIRM:
			mmi_camera_entry_save_confirm_state();
			break;		
			
	#ifdef __CAMERA_FEATURE_OSD_MENU__
	
		case CAMERA_STATE_OSD_MENU:
			mmi_camera_entry_osd_menu_state();
			break;
			
	#endif /* __CAMERA_FEATURE_OSD_MENU__ */
		
	#ifdef __CAMERA_FEATURE_SELF_CAPTURE__

		/* toggle popup */
		case CAMERA_STATE_TOGGLE_TO_SUB:
			mmi_camera_entry_toggle_to_sub_state();
			break;	

		case CAMERA_STATE_TOGGLE_TO_MAIN:
			mmi_camera_entry_toggle_to_main_state();			
			break;	

		/* sub lcd */
		case CAMERA_STATE_SUB_PREVIEW:
			mmi_camera_entry_sublcd_preview_state();						
			break;		

		case CAMERA_STATE_SUB_COUNTDOWN:
			mmi_camera_entry_sublcd_countdown_state();
			break;

		case CAMERA_STATE_SUB_CONTSHOT:
			mmi_camera_entry_sublcd_contshot_state();
			break;		

		#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
			case CAMERA_STATE_SUB_BURSTSHOT:
				mmi_camera_entry_sublcd_burstshot_state();
				break;		

			case CAMERA_STATE_SUB_BURSTSHOT_SAVING:
				mmi_camera_entry_sublcd_burstshot_saving_state();
				break;		
		#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */

	#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */
	    default:
	    break;
			
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_current_state()
* DESCRIPTION
*   exit camera current state
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_exit_current_state(void)
{
	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_exit_current_state(%d)", g_camera_cntx.app_state);
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	switch(g_camera_cntx.app_state)
	{
		case CAMERA_STATE_EXIT:
			/* do nothing */
			break;
	
		/* main lcd */
		case CAMERA_STATE_PREVIEW:
			mmi_camera_exit_preview_state();
			break;

#if defined(__CAMERA_FEATURE_DELAY_TIMER__)
		case CAMERA_STATE_COUNTDOWN:
			mmi_camera_exit_countdown_state();
			break;
#endif
#if defined(__CAMERA_FEATURE_CONT_SHOT__)
		case CAMERA_STATE_CONTSHOT:
			mmi_camera_exit_contshot_state();
			break;
#endif

	#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
	
		case CAMERA_STATE_BURSTSHOT:
			mmi_camera_exit_burstshot_capture_state();
			break;

		case CAMERA_STATE_BURSTSHOT_SAVE_CONFIRM:
			mmi_camera_exit_burstshot_save_confirm_state();
			break;

		case CAMERA_STATE_BURSTSHOT_SAVING:
			mmi_camera_exit_burstshot_saving_state();
			break;

	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */

		case CAMERA_STATE_SAVE_DONE:
			mmi_camera_exit_save_done_state();			
			break;

		case CAMERA_STATE_SAVE_CONFIRM: 
			mmi_camera_exit_save_confirm_state();
			break;		

	#ifdef __CAMERA_FEATURE_OSD_MENU__
	
		case CAMERA_STATE_OSD_MENU:
			mmi_camera_exit_osd_menu_state();
			break;
			
	#endif /* __CAMERA_FEATURE_OSD_MENU__ */


#ifdef __CAMERA_FEATURE_SELF_CAPTURE__

		/* toggle popup */
		case CAMERA_STATE_TOGGLE_TO_SUB:
			mmi_camera_exit_toggle_to_sub_state();
			break;	

		case CAMERA_STATE_TOGGLE_TO_MAIN:
			mmi_camera_exit_toggle_to_main_state();
			break;	

		/* sub lcd */
		case CAMERA_STATE_SUB_PREVIEW:
			mmi_camera_exit_sublcd_preview_state();					
			break;		

		case CAMERA_STATE_SUB_COUNTDOWN:
			mmi_camera_exit_sublcd_countdown_state();
			break;
 
		case CAMERA_STATE_SUB_CONTSHOT: 
			mmi_camera_exit_sublcd_contshot_state();	
			break;		

	#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
		case CAMERA_STATE_SUB_BURSTSHOT:
			mmi_camera_exit_sublcd_burstshot_state();
			break;		

		case CAMERA_STATE_SUB_BURSTSHOT_SAVING:
			mmi_camera_exit_sublcd_burstshot_saving_state();
			break;		
	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */

#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */

	}
}


 
/*****************************************************************************
* FUNCTION
*	 mmi_camera_clam_close_event_hdlr()
* DESCRIPTION
*   handle clam close event 
* PARAMETERS
*	 ptr   IN	    message data ptr
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__)
U8 mmi_camera_clam_close_event_hdlr(void* ptr)
{
	#ifdef MMI_ON_HARDWARE_P
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	mmi_eq_gpio_detect_ind_struct *gpioDetectInd;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	gpioDetectInd = (mmi_eq_gpio_detect_ind_struct *)ptr;
	
	if(gpioDetectInd->gpio_device == EXT_DEV_CLAM_CLOSE)
	{
		g_camera_cntx.is_clam_close = TRUE;
		SetClamState(EXT_DEV_CLAM_CLOSE);
			
		/* print debug info */
		CAMERA_TRACE_CLAM_CLOSE_EVENT();

		switch(g_camera_cntx.app_state)
		{
			/* main lcd */
			case CAMERA_STATE_PREVIEW:
			case CAMERA_STATE_COUNTDOWN:				
			case CAMERA_STATE_CONTSHOT:
			case CAMERA_STATE_BURSTSHOT:
			case CAMERA_STATE_BURSTSHOT_SAVE_CONFIRM:
			case CAMERA_STATE_BURSTSHOT_SAVING:
			case CAMERA_STATE_SAVE_DONE:
			case CAMERA_STATE_SAVE_CONFIRM:
			case CAMERA_STATE_TOGGLE_TO_MAIN:
			case CAMERA_STATE_TOGGLE_TO_SUB:
			case CAMERA_STATE_OSD_MENU:
				/* if clam close will not back to idle, goback to previous screen */				
				#ifndef __MMI_CLAMCLOSE_BACK2IDLE__
					GoBackHistory();
				#endif 
				
				return FALSE;	/* pass this interrupt to framwork's handler */
				break;
				
			/* sub lcd */
			case CAMERA_STATE_SUB_COUNTDOWN:
			case CAMERA_STATE_SUB_CONTSHOT:
			case CAMERA_STATE_SUB_BURSTSHOT:
			case CAMERA_STATE_SUB_BURSTSHOT_SAVING:
				CAMERA_ENTER_STATE(CAMERA_STATE_SUB_PREVIEW);
				return TRUE; 	/* block this event - do not call default handler */ 
				break;		

			case CAMERA_STATE_SUB_PREVIEW:	
				#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
					/* stop and restart sublcd preview, will display correcly according to clam close/open */
					mmi_camera_sublcd_preview_stop();	

					/* config rotate of preview layer, only ISP_SUPPORT need */
					#if defined(ISP_SUPPORT)
					gdi_layer_push_and_set_active(g_camera_cntx.sublcd_base_layer_handle);
					gdi_layer_set_rotate(CAMERA_LCM_SUBLCD_CLAM_CLOSE_ROTATE);
					gdi_layer_pop_and_restore_active();
					#endif

					mmi_camera_sublcd_preview_start();
					return TRUE; 	/* block this event - do not call default handler */ 
				#else
					MMI_ASSERT(0);
				#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */
				break;

			default:
				MMI_ASSERT(0);	/* make sure all state is handled */				
				break;	
		}
	}
	else if(gpioDetectInd->gpio_device == EXT_DEV_CLAM_OPEN)
	{
		g_camera_cntx.is_clam_close = FALSE;
		SetClamState(EXT_DEV_CLAM_OPEN);	
		/* print debug info */
		CAMERA_TRACE_CLAM_OPEN_EVENT();

		switch(g_camera_cntx.app_state)
		{
			/* main lcd */
			case CAMERA_STATE_PREVIEW:
			case CAMERA_STATE_COUNTDOWN:					
			case CAMERA_STATE_CONTSHOT:
			case CAMERA_STATE_BURSTSHOT:
			case CAMERA_STATE_BURSTSHOT_SAVING:
			case CAMERA_STATE_SAVE_DONE:
			case CAMERA_STATE_TOGGLE_TO_MAIN:
			case CAMERA_STATE_TOGGLE_TO_SUB: 
				/* clam open do nothing */
				return FALSE;	/* pass this interrupt to framwork's handler */
				break;

			case CAMERA_STATE_SAVE_CONFIRM:
				/* 
					this may happened when user capture tile image on sub. and all images
					are captured and enter save confirm state 
				*/
				if(g_camera_cntx.is_dis_clam_open)
				{
					/* block this event - do not call default handler */ 
					g_camera_cntx.is_dis_clam_open = FALSE;
					return TRUE;
				}
				else
				{
					return FALSE;
				}
				break;
								
			/* sub lcd */
			case CAMERA_STATE_SUB_COUNTDOWN:			
			case CAMERA_STATE_SUB_CONTSHOT:
			case CAMERA_STATE_SUB_BURSTSHOT:
			case CAMERA_STATE_SUB_BURSTSHOT_SAVING:
				CAMERA_ENTER_STATE(CAMERA_STATE_SUB_PREVIEW);
				return TRUE; 	/* block this event - do not call default handler */ 
				break;		
				
			case CAMERA_STATE_SUB_PREVIEW:	
				#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
					/* stop and restart sublcd preview, will display correcly according to clam close/open */
					mmi_camera_sublcd_preview_stop();	

					/* config rotate of preview layer, only ISP_SUPPORT need */
					#if defined(ISP_SUPPORT)
					gdi_layer_push_and_set_active(g_camera_cntx.sublcd_base_layer_handle);
					gdi_layer_set_rotate(CAMERA_LCM_SUBLCD_CLAM_OPEN_ROTATE);
					gdi_layer_pop_and_restore_active();
					#endif

					mmi_camera_sublcd_preview_start();
					return TRUE; 	/* block this event - do not call default handler */ 
				#else
					MMI_ASSERT(0);
				#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */
				break;
				
			default:
				MMI_ASSERT(0);	/* make sure all state is handled */
				break;	
		}
	}

	#endif /* MMI_ON_HARDWARE_P */	

	/* return false means will pass this interrut to default handler */
	return FALSE;	
}
#endif /* __MMI_CLAMSHELL__ && !__MMI_SLIDE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_calc_remain_storage()
* DESCRIPTION
*   calculate remain storage precent
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_calc_remain_storage(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	FS_DiskInfo	disk_info;
	S8				disk_path[10];
	U64			disk_free_space;
	U64			disk_total_space;
	S32			fs_ret;

	mmi_trace(g_sw_CAMERA, "Excute funs: mmi_camera_calc_remain_storage() is TRUE");
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	/* Get disk name, calculate available disk space */
	memset(disk_path, 0, sizeof(disk_path));
	
	mmi_camera_get_storage_disk_path(disk_path);
	fs_ret = MMI_FS_GetDiskInfo((const UINT8 *)disk_path, &disk_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);
		
	g_camera_cntx.remain_percent = 0;

	if(fs_ret >= 0)
	{
		disk_free_space	= disk_info.FreeClusters;
		disk_total_space  = disk_info.TotalClusters;

		if(disk_total_space >0)
		{
//			g_camera_cntx.remain_percent = (U8)((disk_free_space*100)/disk_total_space);
//			g_camera_cntx.remain_percent = (U8)((float32)((float32)disk_free_space/(float32)disk_total_space)*100); //modified by WeiD
			g_camera_cntx.remain_percent = (U8)(disk_free_space/(disk_total_space/100)); //modified by gaodm 091019
		}
		else
			g_camera_cntx.remain_percent = 0;
	}	
}


/*****************************************************************************
* FUNCTION
*	 mmi_camera_toggle_hide_osd()
* DESCRIPTION
*   toggle hide osd
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_toggle_hide_osd(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_camera_cntx.is_hide_osd = (g_camera_cntx.is_hide_osd)?FALSE:TRUE;
   mmi_camera_preview_draw_osd();
   gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_toggle_self_capture()
* DESCRIPTION
*   toggle self capture (Main/Sub)
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_toggle_self_capture(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_camera_cntx.app_state == CAMERA_STATE_PREVIEW)
	{
		CAMERA_ENTER_STATE(CAMERA_STATE_TOGGLE_TO_SUB);
	}
	else if(g_camera_cntx.app_state == CAMERA_STATE_SUB_PREVIEW)
	{
		/* only when clam is open may toggle to main */
		if(!g_camera_cntx.is_clam_close)
			CAMERA_ENTER_STATE(CAMERA_STATE_TOGGLE_TO_MAIN);
	}
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_turn_on_preview_led_highlight()
* DESCRIPTION
*   turn on preview led highlght
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_turn_on_preview_led_highlight(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8 duty;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	duty = (U8)g_camera_cntx.setting.preview_led_value;
	return;//chenhe,
	if(duty == 0)
	{
		custom_stop_flashlight();
		return;
	}

#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT_7_COLOR__
	switch(g_camera_cntx.setting.led_highlight)
	{
		case CAMERA_SETTING_LED_HIGHLIGHT_WHITE:
			custom_start_flashlight(1,1,1, duty);
			break;
			
		case 	CAMERA_SETTING_LED_HIGHLIGHT_RED:
			custom_start_flashlight(1,0,0, duty);	
			break;

		case 	CAMERA_SETTING_LED_HIGHLIGHT_GREEN:
			custom_start_flashlight(0,1,0, duty);	
			break;		

		case 	CAMERA_SETTING_LED_HIGHLIGHT_BLUE:
			custom_start_flashlight(0,0,1, duty);	
			break;			

		case 	CAMERA_SETTING_LED_HIGHLIGHT_YELLOW:
			custom_start_flashlight(1,1,0, duty);	
			break;	

		case 	CAMERA_SETTING_LED_HIGHLIGHT_PURPLE:
			custom_start_flashlight(1,0,1, duty);	
			break;	

		case 	CAMERA_SETTING_LED_HIGHLIGHT_CYAN:
			custom_start_flashlight(0,1,1, duty);	
			break;	

		case CAMERA_SETTING_LED_HIGHLIGHT_OFF:
			custom_stop_flashlight();
			break;
			
		default:
			/* restore default value */
			mmi_camera_restore_setting();
			break;
	}
#endif /* __CAMERA_FEATURE_LED_HIGHLIGHT_7_COLOR__ */

#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT_ON_OFF__  
	switch(g_camera_cntx.setting.led_highlight)
	{
		case CAMERA_SETTING_LED_HIGHLIGHT_ON:
			custom_start_flashlight(1,1,1, duty);
			break;

		case CAMERA_SETTING_LED_HIGHLIGHT_OFF:
			custom_stop_flashlight();
			break;

		default:
			/* restore default value */
			mmi_camera_restore_setting();
			break;			
	}
#endif /* __CAMERA_FEATURE_LED_HIGHLIGHT_ON_OFF__ */


}




/*****************************************************************************
* FUNCTION
*	 mmi_camera_turn_on_capture_led_highlight()
* DESCRIPTION
*   turn on preview led highlght
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_turn_on_capture_led_highlight(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8 duty;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	duty = (U8)g_camera_cntx.setting.capture_led_value;
	mmi_trace(1,"chenhe,mmi_camera_turn_on_capture_led_highlight,g_camera_cntx.setting.led_highlight) is %d,duty is %d",g_camera_cntx.setting.flash,duty);
	if(duty == 0)
	{
		custom_stop_flashlight();
		return;
	}
	
#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT_7_COLOR__
	switch(g_camera_cntx.setting.led_highlight)
	{
		case CAMERA_SETTING_LED_HIGHLIGHT_WHITE:
			custom_start_flashlight(1,1,1, duty);
			break;
			
		case 	CAMERA_SETTING_LED_HIGHLIGHT_RED:
			custom_start_flashlight(1,0,0, duty);	
			break;

		case 	CAMERA_SETTING_LED_HIGHLIGHT_GREEN:
			custom_start_flashlight(0,1,0, duty);	
			break;		

		case 	CAMERA_SETTING_LED_HIGHLIGHT_BLUE:
			custom_start_flashlight(0,0,1, duty);	
			break;			

		case 	CAMERA_SETTING_LED_HIGHLIGHT_YELLOW:
			custom_start_flashlight(1,1,0, duty);	
			break;	

		case 	CAMERA_SETTING_LED_HIGHLIGHT_PURPLE:
			custom_start_flashlight(1,0,1, duty);	
			break;	

		case 	CAMERA_SETTING_LED_HIGHLIGHT_CYAN:
			custom_start_flashlight(0,1,1, duty);	
			break;	
			
		case CAMERA_SETTING_LED_HIGHLIGHT_OFF:
			custom_stop_flashlight();
			break;			
			
		default:
			/* restore default value */
			mmi_camera_restore_setting();
			break;					
	}
#endif /* __CAMERA_FEATURE_LED_HIGHLIGHT_7_COLOR__ */


#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT_ON_OFF__
	//switch(g_camera_cntx.setting.led_highlight)
	switch(g_camera_cntx.setting.flash)
	{
		case CAMERA_SETTING_LED_HIGHLIGHT_ON:
			custom_start_flashlight(1,1,1, duty);
			break;

		case CAMERA_SETTING_LED_HIGHLIGHT_OFF:
			custom_stop_flashlight();
			break;

		default:
			/* restore default value */
			mmi_camera_restore_setting();
			break;			
	}
#endif /* __CAMERA_FEATURE_LED_HIGHLIGHT_ON_OFF__ */


}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_turn_off_led_highlight()
* DESCRIPTION
*   turn off preview led highlght
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_turn_off_led_highlight(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	custom_stop_flashlight();
}





/*****************************************************************************
*
*	Camera Main LCD Preview State
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_preview_state()
* DESCRIPTION
*	 entry mainlcd preview state
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
/*WeiD Add Start For 6068 Ver: TBM780  on 2007-8-3 19:33 */

/*WeiD Add End  For 6068 Ver: TBM780  on 2007-8-3 19:33 */

static void mmi_camera_entry_preview_state(void)
{
    /*WeiD Add Start For 6068 Ver: TBM780  on 2007-8-3 19:33 */
    S8 			str_buf[256];
    /*WeiD Add End  For 6068 Ver: TBM780  on 2007-8-3 19:33 */
    /*----------------------------------------------------------------*/
    /* Code Body																		*/
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_entry_preview_state()");

    TBM_ENTRY(0x2AA1);

#if (CSW_EXTENDED_API_CAMERA == 1)
    // set_mmc_camera_preview set a global
    // variable in MDI that is not used for 
    // anything. The statement in the #else
    // part is useless.
#else
    set_mmc_camera_preview();
#endif

    /************ init global var ************/
    g_camera_cntx.is_clam_close				= FALSE;
    g_camera_cntx.is_lsk_pressed				= FALSE;
    g_camera_cntx.is_rsk_pressed				= FALSE;
    g_camera_cntx.is_up_arrow_pressed		= FALSE;
    g_camera_cntx.is_down_arrow_pressed 	= FALSE; 
    g_camera_cntx.is_capturing 				= FALSE;

    g_camera_cntx.touch_effect.is_press 			= FALSE;
    g_camera_cntx.touch_wb.is_press 					= FALSE;
    g_camera_cntx.touch_flash.is_press 				= FALSE;
    g_camera_cntx.touch_led_highlight.is_press 	= FALSE;
    g_camera_cntx.touch_night.is_press 				= FALSE;
    g_camera_cntx.touch_image_size.is_press 		= FALSE;
    g_camera_cntx.touch_image_qty.is_press 		= FALSE;
    g_camera_cntx.touch_cont_shot.is_press 		= FALSE;
    g_camera_cntx.touch_delay_timer.is_press 		= FALSE;
    g_camera_cntx.touch_storage.is_press 			= FALSE;
    g_camera_cntx.touch_ev_inc.is_press 			= FALSE;
    g_camera_cntx.touch_ev_dec.is_press 			= FALSE;
    g_camera_cntx.touch_zoom_inc.is_press 			= FALSE;
    g_camera_cntx.touch_zoom_dec.is_press			= FALSE;
    g_camera_cntx.touch_capture.is_press 			= FALSE;

    /* calculate remain storage */
    mmi_camera_calc_remain_storage();

    /* update preview/capture size para */
    mmi_camera_preview_update_capture_size();

#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
    /* draw sublcd icon */
    mmi_camera_sublcd_draw_icon();
#endif 

    if(g_camera_cntx.app_prev_state != CAMERA_STATE_CONTSHOT)
    {
        S32 offset_x = 0;
        S32 offset_y = 0;

#ifdef HORIZONTAL_CAMERA
        offset_y = (LCD_WIDTH - g_camera_cntx.preview_height)>>1;
#else
        offset_y = (LCD_HEIGHT - g_camera_cntx.preview_height)>>1;
#endif
        mmi_camera_preview_free_preview_resource();
        mmi_camera_preview_create_preview_resource(offset_x,offset_y, 
                    g_camera_cntx.preview_width,g_camera_cntx.preview_height,g_camera_cntx.lcm_rotate);
    }

    /* init title bar */
    if(g_camera_cntx.active_camera_osd_ptr->title.is_draw_mmi_title_bar)
    {
#if defined( __MMI_UI_STYLE_3__) || defined(__MMI_UI_STYLE_4__)
        reset_title_status_icon();
        show_status_icon_bar(0);
#endif /* __MMI_UI_STYLE_3__ */

        /* hide short cut */
        MMI_menu_shortcut_number = -1;

        if(g_camera_cntx.active_camera_osd_ptr->title.is_draw_mmi_caption)
        {
            ChangeTitle(MAIN_MENU_TITLE_MULTIMEDIA_ICON, (PU8)GetString(STR_ID_CAMERA_APP_NAME));
        }
        else
        {
            ChangeTitle(0, (PU8)GetString(STR_ID_CAMERA_APP_NAME));
        }
    }
    else
    {
#if defined( __MMI_UI_STYLE_3__) || defined(__MMI_UI_STYLE_4__)
        hide_status_icon_bar(0);
#endif /* __MMI_UI_STYLE_3__ */
    }

    /*WeiD Add Start For 6068 Ver: TBM780  on 2007-8-3 19:34 */
    //coolsand_UI_start_timer(1000, mmi_camera_preview_wait_hdlr);  //add by WeiD
    ClearAllKeyHandler();
    /*WeiD Add End  For 6068 Ver: TBM780  on 2007-8-3 19:34 */
    /* register keys */
    /*WeiD Del Start For 6068 Ver: TBM780  on 2007-8-3 19:34 */
#if 0
    mmi_camera_preview_set_key_hdlr();  
#endif
    /*WeiD Del End  For 6068 Ver: TBM780  on 2007-8-3 19:34 */

    /* clear base layer bg */
    if( (g_camera_cntx.app_prev_state != CAMERA_STATE_CONTSHOT) && 
             (g_camera_cntx.app_prev_state != CAMERA_STATE_OSD_MENU))
    {
        mmi_camera_preview_clear_background();
    }

#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
    if(g_camera_cntx.is_tileshot)
    { 
        /* is in tile shot, we do not clear bg when from countdown and sub preview state */
        if( !(g_camera_cntx.app_prev_state == CAMERA_STATE_COUNTDOWN) && 
                 !(g_camera_cntx.app_prev_state == CAMERA_STATE_TOGGLE_TO_MAIN) )
        {
#if defined(__CAMERA_OSD_HORIZONTAL__) 
            mmi_camera_check_tileshot_orientation();
#endif

            /* clear tile result backgroud */
            gdi_layer_push_and_set_active(g_camera_cntx.tileshot_result_layer_handle);
            gdi_layer_clear(GDI_COLOR_BLACK);
            gdi_layer_pop_and_restore_active();
        }
    }
    else
    {
        /* not tile shot */
        /* clear tile result backgroud */
        gdi_layer_push_and_set_active(g_camera_cntx.tileshot_result_layer_handle);
        gdi_layer_clear(GDI_COLOR_BLACK);
        gdi_layer_pop_and_restore_active();
    }
#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */			

#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
    /* check if is burst shot, if yes, change and store previous capture size */
    if(g_camera_cntx.setting.cont_shot == CAMERA_SETTING_CONT_SHOT_BURST)
    {
        /* if not correct image size, change to it */
        if(g_camera_cntx.setting.image_size != CAMERA_FEATURE_CONT_SHOT_BURST_IMAGE_IDX)
        {
            g_camera_cntx.image_size_before_burst = g_camera_cntx.setting.image_size;
            g_camera_cntx.setting.image_size = CAMERA_FEATURE_CONT_SHOT_BURST_IMAGE_IDX;

            /* image size changed, update capture size and preview size */
            mmi_camera_preview_update_capture_size();
        }
    }
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */

    /* draw panel and softkey */
    /*WeiD Del Start For 6068 Ver: TBM780  on 2007-8-3 19:37 */
#if 0
    mmi_camera_preview_set_hint(NULL, NULL, 0);
#endif
    /*WeiD Del End  For 6068 Ver: TBM780  on 2007-8-3 19:37 */
	
    /*WeiD Add Start For 6068 Ver: TBM780  on 2007-8-3 19:37 */
#ifndef __CAMERA_FEATURE_HIDE_HINT__
    pfnUnicodeStrcpy((PS8)str_buf, (PS8)GetString(STR_ID_CAMERA_PLEASEWAIT));    //Please Wait
    mmi_camera_preview_set_hint( (PS8)str_buf,
                                            NULL,
                                            HINT_POPUP_FADE_TIME);
#endif /* __CAMERA_FEATURE_HIDE_HINT__ */
    /*WeiD Add End  For 6068 Ver: TBM780  on 2007-8-3 19:37 */
	
    mmi_camera_preview_draw_osd();

    /* set blt layer for non-previwing, and blt first skin */
    mmi_camera_preview_set_blt_layer(FALSE);

#if defined(__CAMERA_OSD_HORIZONTAL__) 
    if(g_camera_cntx.app_prev_state == CAMERA_STATE_CONTSHOT)
        gdi_layer_set_blt_layer(g_camera_cntx.preview_layer_handle,
                                                    g_camera_cntx.osd_layer_handle,
                                                    0,
                                                    0);
#endif
	
    gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
    mmi_trace(g_sw_CAMERA,"before preview start g_camera_cntx.last_error = %ld",g_camera_cntx.last_error);
    /* check if any error occurs in mmi_camera_entry_app_screen function */
    if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
    {
        mmi_trace(g_sw_CAMERA,"mmi_camera_entry_preview_state 1");
        mmi_camera_display_error_popup();
        TBM_EXIT(0x2AA1);
        return;
    }
    else
    {
        /* if no error occurs, start preview */  
        mmi_camera_preview_set_blt_layer(TRUE);
        mmi_camera_preview_start();
    }
    mmi_camera_preview_set_key_hdlr();
    /* check if preview success */
    if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
    {
        mmi_trace(g_sw_CAMERA,"mmi_camera_entry_preview_state 2");
        mmi_camera_display_error_popup();
    }
    TBM_EXIT(0x2AA1);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_preview_state()
* DESCRIPTION
*	 exit preview state
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
static void mmi_camera_exit_preview_state(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_exit_preview_state()");
   
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	/* stop timer */
	if(g_camera_cntx.app_next_state != CAMERA_STATE_COUNTDOWN)
	{
		coolsand_UI_cancel_timer(mmi_camera_preview_hide_hint);
	}


	
#ifdef __CAMERA_FEATURE_ZOOM__	
	coolsand_UI_cancel_timer(mmi_camera_preview_zoom_out);
	coolsand_UI_cancel_timer(mmi_camera_preview_zoom_in);	
#endif /* __CAMERA_FEATURE_ZOOM__ */

	/* there are some setting changed by using hotkey or preview panel, save them to NVRAM */
	mmi_camera_store_setting();

	/* exit app */
	if(g_camera_cntx.app_next_state == CAMERA_STATE_EXIT)
	{
		/* turn off led highlight */
		mmi_camera_turn_off_led_highlight();
	
		/* stop previewing */
		mmi_camera_preview_stop();

		#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
			/* copy tiled picture from tile result layer to base layer */
			if(g_camera_cntx.is_tileshot)
			{
				#ifdef __CAMERA_FEATURE_ADD_FRAME__
					/* if has add frame, merge layers to base  */
					gdi_layer_flatten_to_base(	g_camera_cntx.tileshot_result_layer_handle, 
														g_camera_cntx.add_frame_layer_handle,
														0,	0);
				#else
					/* no add frame */
					gdi_layer_flatten_to_base(	g_camera_cntx.tileshot_result_layer_handle, 
														0, 0,	0);
				#endif 
			}
		#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */

		/* flatten layer */
		mmi_camera_preview_flatten_layer(TRUE);
	}

	/* toggle to sub */
	if(g_camera_cntx.app_next_state == CAMERA_STATE_TOGGLE_TO_SUB)
	{
		/* turn off led highlight */
		mmi_camera_turn_off_led_highlight();
	
		/* stop previewing */
		mmi_camera_preview_stop();

		#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
			/* copy tiled picture from tile result layer to base layer */
			if(g_camera_cntx.is_tileshot)
			{
				#ifdef __CAMERA_FEATURE_ADD_FRAME__
					/* if has add frame, merge layers to base  */
					gdi_layer_flatten_to_base(	g_camera_cntx.tileshot_result_layer_handle, 
														g_camera_cntx.add_frame_layer_handle,
														0,	0);
				#else
					/* no add frame */
					gdi_layer_flatten_to_base(	g_camera_cntx.tileshot_result_layer_handle, 
														0, 0,	0);
				#endif 
			}
		#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */

		mmi_camera_preview_free_preview_resource();

		if(g_camera_cntx.is_fullscreen)
		{
			gdi_layer_set_blt_layer(g_camera_cntx.base_layer_handle, 
											g_camera_cntx.add_frame_layer_handle, 
											0, 0);
		}
		else
		{
			gdi_layer_set_blt_layer(g_camera_cntx.base_layer_handle, 0, 0, 0);
		}
		
		#if defined( __MMI_UI_STYLE_3__) || defined(__MMI_UI_STYLE_4__)
			hide_status_icon_bar(0);
		#endif /* __MMI_UI_STYLE_3__ */		
	}

}

void mmi_camera_sensorPwr_autoStart(void)
{

}
/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_set_key_hdlr()
* DESCRIPTION
*   set key handler 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_set_key_hdlr(void)
{
    mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_preview_set_key_hdlr()");
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearAllKeyHandler();

#if defined(__CAMERA_FEATURE_TOUCH_SCREEN__) && defined(__MMI_TOUCH_SCREEN__)
    mmi_wgui_register_pen_down_handler(mmi_camera_touch_scr_pen_down_hdlr);
    mmi_wgui_register_pen_up_handler(mmi_camera_touch_scr_pen_up_hdlr);
    mmi_wgui_register_pen_move_handler(mmi_camera_touch_scr_pen_move_hdlr);
#endif /* __CAMERA_FEATURE_TOUCH_SCREEN__ && __MMI_TOUCH_SCREEN__ */

    /* register soft keys */	
#ifndef __PRJ_WITH_SPICAM__
    SetKeyHandler(mmi_camera_preview_lsk_press, KEY_LSK, KEY_EVENT_DOWN);	
    SetKeyHandler(mmi_camera_preview_lsk_release, KEY_LSK, KEY_EVENT_UP);		
#endif

    SetKeyHandler(mmi_camera_preview_rsk_press, KEY_RSK, KEY_EVENT_DOWN);	
    SetKeyHandler(mmi_camera_preview_rsk_release, KEY_RSK, KEY_EVENT_UP);		

#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
    /*************************************/
    /* IconBar Panel Key 					 */
    /*************************************/
    SetKeyHandler(mmi_camera_iconbar_right_arrow_press,	KEY_RIGHT_ARROW,	KEY_EVENT_DOWN);
    SetKeyHandler(mmi_camera_iconbar_right_arrow_release,	KEY_RIGHT_ARROW,	KEY_EVENT_UP);
    SetKeyHandler(mmi_camera_iconbar_left_arrow_press,		KEY_LEFT_ARROW, 	KEY_EVENT_DOWN);
    SetKeyHandler(mmi_camera_iconbar_left_arrow_releas,	KEY_LEFT_ARROW, 	KEY_EVENT_UP);		
    SetKeyHandler(mmi_camera_iconbar_up_arrow_press,		KEY_UP_ARROW,		KEY_EVENT_DOWN);
    SetKeyHandler(mmi_camera_iconbar_up_arrow_release,		KEY_UP_ARROW,		KEY_EVENT_UP);		
    SetKeyHandler(mmi_camera_iconbar_down_arrow_press,		KEY_DOWN_ARROW,	KEY_EVENT_DOWN);
    SetKeyHandler(mmi_camera_iconbar_down_arrow_release,	KEY_DOWN_ARROW,	KEY_EVENT_UP);		

    /* register capture and control keys */
#ifndef __MMI_ANALOGTV__
    if(CAMERA_CAPTURE_KEY!=-1)
    {
        SetKeyHandler(mmi_camera_capture_button_press, CAMERA_CAPTURE_KEY, KEY_EVENT_DOWN);	
        SetKeyHandler(mmi_camera_capture_button_release, CAMERA_CAPTURE_KEY, KEY_EVENT_UP);				
    }
#endif
    if(CAMERA_CAPTURE_EXTRA_KEY!=-1)
    {
        SetKeyHandler(mmi_camera_capture_button_press, CAMERA_CAPTURE_EXTRA_KEY, KEY_EVENT_DOWN);
        SetKeyHandler(mmi_camera_capture_button_release, CAMERA_CAPTURE_EXTRA_KEY, KEY_EVENT_UP);	
    }
	 
#ifdef WIN32 
    /* Simulator debug */
    SetKeyHandler(mmi_camera_capture_button_press, KEY_0, KEY_EVENT_DOWN);
#endif 
			
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
    SetKeyHandler(mmi_camera_toggle_self_capture,	(U16)CAMREA_TOGGLE_SELF_CAPTURE_KEY, KEY_EVENT_DOWN);
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */
		
#else
    /*************************************/
    /* Normal Key			 					 */
    /*************************************/
		
    /* change add frame hotkey - side keys */
#if 0   //chenqiang delete temp fix 12920
#ifdef __CAMERA_FEATURE_ADD_FRAME__    
    SetKeyHandler(mmi_camera_preview_change_frame_up,	KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(mmi_camera_preview_change_frame_down, KEY_VOL_DOWN, KEY_EVENT_DOWN);
#endif 
#endif
    /* register capture and control keys */
#ifndef __MMI_ANALOGTV__
    if(CAMERA_CAPTURE_KEY!=-1)
    {
        mmi_trace(g_sw_CAMERA, "Excute funs: CAMERA_CAPTURE_KEY is TRUE");
#if defined(MEM_ULC_3216)
        SetKeyHandler(mmi_camera_capture_button_press, CAMERA_CAPTURE_KEY, KEY_EVENT_UP);
#else
        SetKeyHandler(mmi_camera_capture_button_press, CAMERA_CAPTURE_KEY, KEY_EVENT_DOWN);	
        SetKeyHandler(mmi_camera_capture_button_release, CAMERA_CAPTURE_KEY, KEY_EVENT_UP);				
#endif
    }
#endif
    if(CAMERA_CAPTURE_EXTRA_KEY!=-1)
    {
#if defined(MEM_ULC_3216)
        SetKeyHandler(mmi_camera_capture_button_press, CAMERA_CAPTURE_EXTRA_KEY, KEY_EVENT_UP);
#else
        SetKeyHandler(mmi_camera_capture_button_press, CAMERA_CAPTURE_EXTRA_KEY, KEY_EVENT_DOWN);
        SetKeyHandler(mmi_camera_capture_button_release, CAMERA_CAPTURE_EXTRA_KEY, KEY_EVENT_UP);	
#endif
    }
    
#ifndef __MMI_ANALOGTV__
    if(CAMERA_CAPTURE_OK_KEY!=-1)
    {
        mmi_trace(g_sw_CAMERA, "Excute funs: CAMERA_CAPTURE_OK_KEY is TRUE");
#if defined(MEM_ULC_3216)
        SetKeyHandler(mmi_camera_capture_button_press, CAMERA_CAPTURE_OK_KEY, KEY_EVENT_UP);
#else
        SetKeyHandler(mmi_camera_capture_button_press, CAMERA_CAPTURE_OK_KEY, KEY_EVENT_DOWN);
        SetKeyHandler(mmi_camera_capture_button_release, CAMERA_CAPTURE_OK_KEY, KEY_EVENT_UP);	
#endif
    }
#endif	 
#ifdef WIN32 
    /* Simulator debug */
#if defined(MEM_ULC_3216)
    SetKeyHandler(mmi_camera_capture_button_press, KEY_0, KEY_EVENT_UP);
#else
    SetKeyHandler(mmi_camera_capture_button_press, KEY_0, KEY_EVENT_DOWN);
#endif
#endif 

#if 0
    /* ev / zooming */
    if(g_camera_cntx.osd_rotate==CAMERA_DISPLAY_ROTATE_90 || g_camera_cntx.osd_rotate ==CAMERA_DISPLAY_ROTATE_270)
    {
        SetKeyHandler(mmi_camera_preview_ev_inc_key_press,		KEY_DOWN_ARROW,	KEY_EVENT_DOWN);
        SetKeyHandler(mmi_camera_preview_ev_inc_key_release,	KEY_DOWN_ARROW,	KEY_EVENT_UP);
		
        SetKeyHandler(mmi_camera_preview_ev_dec_key_press,		KEY_UP_ARROW,	KEY_EVENT_DOWN);
        SetKeyHandler(mmi_camera_preview_ev_dec_key_release,	KEY_UP_ARROW,	KEY_EVENT_UP);
    }
    else
    {
        SetKeyHandler(mmi_camera_preview_ev_inc_key_press,		KEY_RIGHT_ARROW,	KEY_EVENT_DOWN);
        SetKeyHandler(mmi_camera_preview_ev_inc_key_release,	KEY_RIGHT_ARROW,	KEY_EVENT_UP);
		
        SetKeyHandler(mmi_camera_preview_ev_dec_key_press,		KEY_LEFT_ARROW,	KEY_EVENT_DOWN);
        SetKeyHandler(mmi_camera_preview_ev_dec_key_release,	KEY_LEFT_ARROW,	KEY_EVENT_UP);
    }
#endif

#ifndef __PRJ_WITH_SPICAM__
#if defined(__CAMERA_FEATURE_BRIGHTNESS__)
//add by WeiD
//for brightness
    {
        SetKeyHandler(mmi_camera_preview_bt_inc_key_press,		KEY_RIGHT_ARROW ,	KEY_EVENT_DOWN);   
        SetKeyHandler(mmi_camera_preview_bt_inc_key_release,	KEY_RIGHT_ARROW ,	KEY_EVENT_UP);
		
        SetKeyHandler(mmi_camera_preview_bt_dec_key_press,		KEY_LEFT_ARROW,	KEY_EVENT_DOWN);
        SetKeyHandler(mmi_camera_preview_bt_dec_key_release,	KEY_LEFT_ARROW,	KEY_EVENT_UP);
    }
#endif
#ifdef __CAMERA_FEATURE_ZOOM__
    if(g_camera_cntx.osd_rotate==CAMERA_DISPLAY_ROTATE_90 || g_camera_cntx.osd_rotate ==CAMERA_DISPLAY_ROTATE_270)
    {
        SetKeyHandler(mmi_camera_preview_down_arrow_press,		KEY_LEFT_ARROW,	KEY_EVENT_DOWN);
        SetKeyHandler(mmi_camera_preview_down_arrow_release,	KEY_LEFT_ARROW,	KEY_EVENT_UP);
		
        SetKeyHandler(mmi_camera_preview_up_arrow_press,		KEY_RIGHT_ARROW,		KEY_EVENT_DOWN);
        SetKeyHandler(mmi_camera_preview_up_arrow_release,		KEY_RIGHT_ARROW,		KEY_EVENT_UP);
    }		
    else
    {
        SetKeyHandler(mmi_camera_preview_down_arrow_press,		KEY_DOWN_ARROW,	KEY_EVENT_DOWN);
        SetKeyHandler(mmi_camera_preview_down_arrow_release,	KEY_DOWN_ARROW,	KEY_EVENT_UP);
			
        SetKeyHandler(mmi_camera_preview_up_arrow_press,		KEY_UP_ARROW,		KEY_EVENT_DOWN);
        SetKeyHandler(mmi_camera_preview_up_arrow_release,		KEY_UP_ARROW,		KEY_EVENT_UP);
    }
#endif /* __CAMERA_FEATURE_ZOOM__ */

#if defined(__CAMERA_FEATURE_CONTRAST__)
//add by WeiD
//for contrast
    SetKeyHandler(mmi_camera_preview_contrast_hotkey_press, CAMERA_CT_KEY, KEY_EVENT_DOWN);  //2//2
#endif

    /* hot keys */
#ifdef __CAMERA_FEATURE_EFFECT__
    if(CAMERA_EFFECT_KEY != -1)
        SetKeyHandler(mmi_camera_preview_effect_hotkey_press, (U16)CAMERA_EFFECT_KEY,	KEY_EVENT_DOWN);
#endif /* __CAMERA_FEATURE_EFFECT__ */

#ifdef __CAMERA_FEATURE_WB__
    if(CAMERA_WB_KEY != -1)
        SetKeyHandler(mmi_camera_preview_wb_hotkey_press, (U16)CAMERA_WB_KEY, KEY_EVENT_DOWN);
#endif /* __CAMERA_FEATURE_WB__ */

#ifdef __CAMERA_FEATURE_NIGHT__
    if(CAMERA_NIGHT_KEY != -1)
        SetKeyHandler(mmi_camera_preview_night_hotkey_press, (U16)CAMERA_NIGHT_KEY, KEY_EVENT_DOWN);	
#endif /* __CAMERA_FEATURE_NIGHT__ */

#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT__
    if(CAMERA_LED_HIGHLIGHT_KEY != -1)
        SetKeyHandler(mmi_camera_preview_led_highlight_hotkey_press, (U16)CAMERA_LED_HIGHLIGHT_KEY, KEY_EVENT_DOWN);		
#endif /* __CAMERA_FEATURE_LED_HIGHLIGHT__ */

#ifdef __CAMERA_FEATURE_FLASH__
    if(CAMERA_FLASH_KEY != -1)
        SetKeyHandler(mmi_camera_preview_flash_hotkey_press,	(U16)CAMERA_FLASH_KEY, KEY_EVENT_DOWN);
#endif /* __CAMERA_FEATURE_FLASH__ */
#endif

#ifndef __MMI_DISABLE_DUAL_SENSOR__
    if (CAMERA_SWITCH_SENSOR != -1)
        SetKeyHandler(mmi_camera_preview_switch_sensor_hotkey_press, (U16)CAMERA_SWITCH_SENSOR, KEY_EVENT_DOWN);
#endif /* __CAMERA_FEATURE_FLASH__ */

#ifndef __PRJ_WITH_SPICAM__
#ifdef __CAMERA_FEATURE_CONT_SHOT__
    if(CAMERA_CONT_SHOT_KEY != -1)
        SetKeyHandler(mmi_camera_preview_cont_shot_hotkey_press,	(U16)CAMERA_CONT_SHOT_KEY, KEY_EVENT_DOWN);
#endif /* __CAMERA_FEATURE_CONT_SHOT__ */

#ifdef __CAMERA_FEATURE_DELAY_TIMER__
    if(CAMERA_DELAY_TIMER_KEY != -1)
        SetKeyHandler(mmi_camera_preview_delay_timer_hotkey_press, (U16)CAMERA_DELAY_TIMER_KEY, KEY_EVENT_DOWN);	
#endif /* __CAMERA_FEATURE_DELAY_TIMER__ */

    if(CAMERA_IMAGE_SIZE_KEY != -1)
        SetKeyHandler(mmi_camera_preview_image_size_hotkey_press, (U16)CAMERA_IMAGE_SIZE_KEY, KEY_EVENT_DOWN);	

    if(CAMERA_IMAGE_QTY_KEY != -1)
        SetKeyHandler(mmi_camera_preview_image_qty_hotkey_press, (U16)CAMERA_IMAGE_QTY_KEY, KEY_EVENT_DOWN);	

#ifdef __CAMERA_FEATURE_SHOT_MODE__
    if(CAMERA_SHOT_MODE_KEY != -1)
        SetKeyHandler(mmi_camera_preview_shot_mode_hotkey_press, (U16)CAMERA_SHOT_MODE_KEY, KEY_EVENT_DOWN);	
#endif /* __CAMERA_FEATURE_SHOT_MODE__ */

#ifdef __CAMERA_FEATURE_ISO__
    if(CAMERA_ISO_KEY != -1)
        SetKeyHandler(mmi_camera_preview_iso_hotkey_press, (U16)CAMERA_ISO_KEY, KEY_EVENT_DOWN);	
#endif /* __CAMERA_FEATURE_ISO__ */

#ifdef __CAMERA_FEATURE_AE_METER__
    if(CAMERA_AE_METER_KEY != -1)
        SetKeyHandler(mmi_camera_preview_ae_meter_hotkey_press, (U16)CAMERA_AE_METER_KEY, KEY_EVENT_DOWN);	
#endif /* __CAMERA_FEATURE_AE_METER__ */

#ifdef __CAMERA_FEATURE_STORAGE__
    if(CAMERA_STORAGE_KEY != -1)
        SetKeyHandler(mmi_camera_preview_storage_hotkey_press, (U16)CAMERA_STORAGE_KEY, KEY_EVENT_DOWN);	
#endif /* __CAMERA_FEATURE_STORAGE__ */

#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
    SetKeyHandler(mmi_camera_toggle_self_capture,	(U16)CAMERA_TOGGLE_SELF_CAPTURE_KEY, KEY_EVENT_DOWN);
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */
				
    /* toggle hide OSD */
    SetKeyHandler(mmi_camera_toggle_hide_osd, CAMERA_TOGGLE_HIDE_OSD_KEY, KEY_EVENT_DOWN);
    //SetKeyHandler(mmi_camera_capture_button_press, KEY_UP_ARROW, KEY_EVENT_DOWN);	//chenhe,test
#endif
		
#endif /* __CAMERA_FEATURE_ICONBAR_PANEL__ */

}

/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_create_preview_resource()
* DESCRIPTION
*   free main lcd's preview layer resource
* PARAMETERS
*	 offset_x	IN		preview layer offset x
*	 offset_y	IN		preview layer offset y
*	 width 		IN		preview layer width
*	 height		IN		preview_layer height
*	 rotate		IN		lcm rotate
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_create_preview_resource(S32 offset_x,S32 offset_y,S32 width,S32 height,U8 rotate)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	PU8	buf_ptr;
	S32 	buf_size;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_preview_create_preview_resource()");
   
	ASSERT(width*height<=LCD_WIDTH*LCD_HEIGHT);
	
	gdi_layer_push_and_set_active(g_camera_cntx.base_layer_handle);
	gdi_layer_get_buffer_ptr(&buf_ptr);

	/* if < MT 6227 *****************************************************************/
	/*  calculate the right buf_ptr position.													  */
	/********************************************************************************/
	if(offset_x !=0 || offset_y !=0)
	{
		#ifdef HORIZONTAL_CAMERA
			buf_ptr += ((offset_y*LCD_HEIGHT*GDI_LAYER.bits_per_pixel)>>3);
		#else
			buf_ptr += ((offset_y*LCD_WIDTH*GDI_LAYER.bits_per_pixel)>>3);
		#endif
		buf_ptr += ((offset_x*GDI_LAYER.bits_per_pixel)>>3);
	}
	buf_size = ((width*height*GDI_MAINLCD_BIT_PER_PIXEL)>>3);
	gdi_layer_create_using_outside_memory(	0,
														0,
														width,
														height,
														&g_camera_cntx.preview_layer_handle,
														(PU8)buf_ptr, 
														buf_size);	

	gdi_layer_set_active(g_camera_cntx.preview_layer_handle);
	#if defined(MT6227)||defined(MT6226)||defined(MT6226M)||defined(MT6228)||defined(MT6229)
	//gdi_layer_set_background(GDI_COLOR_BLACK);//fengzb
	#endif
	gdi_layer_set_rotate(rotate);
	gdi_layer_set_position(offset_x,offset_y);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);	
	gdi_layer_pop_and_restore_active();
	
}


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_free_preview_resource()
* DESCRIPTION
*   free main lcd's preview layer resource
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_free_preview_resource(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_camera_cntx.preview_layer_handle==GDI_LAYER_EMPTY_HANDLE)
		return;
	gdi_layer_free(g_camera_cntx.preview_layer_handle);
	g_camera_cntx.preview_layer_handle	= GDI_LAYER_EMPTY_HANDLE;
}

/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_create_resource()
* DESCRIPTION
*   create main lcd's preview layer resource
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_create_resource(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_preview_create_resource()");

    MMI_ASSERT(g_camera_cntx.is_mainlcd_resource == FALSE);
    /* if main lcd resoruce is not created */

    /************ base layer ************/
    /* get both MainLCD and SubLCD base handle */
    gdi_layer_get_base_handle(&g_camera_cntx.base_layer_handle);

    /* clear fullscreen */

    gdi_layer_clear(GDI_COLOR_BLACK);


#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
    /************ tile shot ************/
    /* create tile result buffer */
#ifdef APPMEM_SUPPORT
    /* with ASM */
    //g_camera_cntx.tileshot_result_buf_ptr = kal_app_mem_alloc(CAMERA_TILE_RESULT_BUFFER_SIZE, KAL_APPMEM_APP, MOD_MMI); 
    g_camera_cntx.tileshot_result_buf_ptr = AllocMediaMemory(CAMERA_TILE_RESULT_BUFFER_SIZE);
#else
    /* no ASM */
    g_camera_cntx.tileshot_result_buf_ptr = (PU8)camera_tile_result_buffer;
#endif /* APPMEM_SUPPORT */

    gdi_layer_create_using_outside_memory( 0,
											0,
											LCD_WIDTH,
											LCD_HEIGHT,
											&g_camera_cntx.tileshot_result_layer_handle,
											(PU8)g_camera_cntx.tileshot_result_buf_ptr, 
											(S32)CAMERA_TILE_RESULT_BUFFER_SIZE);

    gdi_layer_push_and_set_active(g_camera_cntx.tileshot_result_layer_handle);
    gdi_layer_clear(GDI_COLOR_BLACK);
    gdi_layer_pop_and_restore_active();		
	
#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */

#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
    /* make sublcd active and get it's base layer handle */
    gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
    gdi_layer_get_base_handle(&g_camera_cntx.sublcd_base_layer_handle);

    gdi_layer_clear(GDI_COLOR_BLACK);

    gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
#endif 

    /************ frame layer ************/
#ifdef __CAMERA_FEATURE_ADD_FRAME__

    /* create a singe layer frame for add frame layer */
    gdi_layer_create(0, 
                                    0, 
                                    LCD_WIDTH, 
                                    LCD_HEIGHT, 
                                    &g_camera_cntx.add_frame_layer_handle);
							
    gdi_layer_push_and_set_active(g_camera_cntx.add_frame_layer_handle);
    gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);				
    gdi_layer_clear(GDI_COLOR_TRANSPARENT);
    gdi_layer_pop_and_restore_active();		

    /* update frame */
    mmi_camera_draw_add_frame();

#endif /* __CAMERA_FEATURE_ADD_FRAME__ */

    /************ osd layer ************/
    /* create a double layer frame from outside memory */

#ifdef APPMEM_SUPPORT
    /* with ASM */
    //g_camera_cntx.osd_layer_buf_ptr = kal_app_mem_alloc((CAMERA_OSD_BUFFER_SIZE), KAL_APPMEM_APP, MOD_MMI); 
#ifdef MMI_ON_HARDWARE_P //chenhe for simulator
#if 1
    g_camera_cntx.osd_layer_buf_ptr = (PU8)AllocMediaMemory(CAMERA_OSD_BUFFER_SIZE>>1);
#else
    g_camera_cntx.osd_layer_buf_ptr = (PU8)AllocMediaMemory(CAMERA_OSD_BUFFER_SIZE);
#endif
#else
    g_camera_cntx.osd_layer_buf_ptr = (PU8)OslMalloc(CAMERA_OSD_BUFFER_SIZE);		
#endif
#else
    /* no ASM */
    g_camera_cntx.osd_layer_buf_ptr = (PU8)camera_osd_layer_buffer;
#endif /* APPMEM_SUPPORT */
#if 1
    gdi_layer_create_using_outside_memory(0,
                                                            0,
                                                            LCD_WIDTH,
                                                            LCD_HEIGHT,
                                                            &g_camera_cntx.osd_layer_handle,
                                                            (PU8)g_camera_cntx.osd_layer_buf_ptr, 
                                                            (S32)(CAMERA_OSD_BUFFER_SIZE)>>1);
#else
    gdi_layer_create_double_using_outside_memory( 0,
                                                            0,
                                                            LCD_WIDTH,
                                                            LCD_HEIGHT,
                                                            &g_camera_cntx.osd_layer_handle,
                                                            (PU8)g_camera_cntx.osd_layer_buf_ptr, 
                                                            (S32)(CAMERA_OSD_BUFFER_SIZE));
#endif
    gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);
    gdi_layer_clear_background(GDI_COLOR_TRANSPARENT);
    gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);	
    gdi_layer_pop_and_restore_active();		

    g_camera_cntx.is_mainlcd_resource = TRUE;

}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_free_resource()
* DESCRIPTION
*   free main lcd's preview layer resource
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_free_resource(void)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/****** free add frame layer ******/
	#ifdef __CAMERA_FEATURE_ADD_FRAME__
	if(g_camera_cntx.add_frame_layer_handle != GDI_LAYER_EMPTY_HANDLE)
	{
		gdi_layer_free(g_camera_cntx.add_frame_layer_handle);
		g_camera_cntx.add_frame_layer_handle = GDI_LAYER_EMPTY_HANDLE;
	}
	#endif /* __CAMERA_FEATURE_ADD_FRAME__ */


	/****** free osd layer ******/
	if(g_camera_cntx.osd_layer_handle	!= GDI_LAYER_EMPTY_HANDLE)
	{
		gdi_layer_free(g_camera_cntx.osd_layer_handle);
		g_camera_cntx.osd_layer_handle	= GDI_LAYER_EMPTY_HANDLE;
	}

	#ifdef APPMEM_SUPPORT
		/* with ASM */
		FreeMediaMemory((unsigned long)g_camera_cntx.osd_layer_buf_ptr); 
		g_camera_cntx.osd_layer_buf_ptr = NULL;
	#endif /* APPMEM_SUPPORT */


	#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
		/************ free tile shot ************/
		/* free tile shot result layer */
	if(g_camera_cntx.tileshot_result_layer_handle	!= GDI_LAYER_EMPTY_HANDLE)
	{
		gdi_layer_free(g_camera_cntx.tileshot_result_layer_handle);
		g_camera_cntx.tileshot_result_layer_handle	= GDI_LAYER_EMPTY_HANDLE;
	}

		#ifdef APPMEM_SUPPORT
			/* with ASM */
			FreeMediaMemory(g_camera_cntx.tileshot_result_buf_ptr); 
			g_camera_cntx.tileshot_result_buf_ptr = NULL;
		#endif /* APPMEM_SUPPORT */

	#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */

	/* restore only base layer active */
	gdi_layer_set_blt_layer(g_camera_cntx.base_layer_handle, 0, 0, 0);

	g_camera_cntx.is_mainlcd_resource = FALSE;
	
}


/*****************************************************************************
* FUNCTION
*	mmi_camera_check_tileshot_orientation()
* DESCRIPTION
*   update display rotate (0 | 90 | 180 | 270 )
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_OSD_HORIZONTAL__) 
void 	mmi_camera_check_tileshot_orientation(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* rotate tileshot result layer */
	gdi_layer_push_and_set_active(g_camera_cntx.tileshot_result_layer_handle);
	gdi_layer_set_rotate(g_camera_cntx.osd_rotate);

	if(g_camera_cntx.osd_rotate==CAMERA_DISPLAY_ROTATE_90 || g_camera_cntx.osd_rotate ==CAMERA_DISPLAY_ROTATE_270)
	{
		gdi_layer_resize(LCD_HEIGHT,LCD_WIDTH);
	}
	else 
	{
		gdi_layer_resize(LCD_WIDTH,LCD_HEIGHT);
	}

	gdi_layer_clear(GDI_COLOR_BLACK);
	gdi_layer_pop_and_restore_active();


}
#endif
/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_start()
* DESCRIPTION
*   start previewing of main lcd
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_start(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	mdi_camera_setting_struct	camera_setting_data;
	GDI_HANDLE						preview_layer_handle = -1;
	U32								preview_layer_flag = 0;
	U32								blt_layer_flag = 0;
	S32								preview_wnd_offset_x = 0;
	S32								preview_wnd_offset_y = 0;
	U16								preview_wnd_width = 0;
	U16								preview_wnd_height = 0;

   #ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
		S32							offset_x;
		S32							offset_y;
	#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */

	
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_preview_start()");

	/* preview parametes */
	camera_setting_data.zoom				= mmi_camera_preview_get_zoom_factor();
	camera_setting_data.wb					= camera_wb_command_map[g_camera_cntx.setting.wb];	
	camera_setting_data.ev					= camera_ev_command_map[g_camera_cntx.setting.ev];
	camera_setting_data.banding 			= camera_banding_command_map[g_camera_cntx.setting.banding];
	camera_setting_data.flash				= camera_flash_command_map[g_camera_cntx.setting.flash];
	camera_setting_data.ae_meter			= camera_ae_meter_command_map[g_camera_cntx.setting.ae_meter];
	camera_setting_data.iso					= camera_iso_command_map[g_camera_cntx.setting.iso];
	camera_setting_data.night				= g_camera_cntx.setting.night;
	camera_setting_data.shot_mode			= g_camera_cntx.setting.shot_mode;	/* TODO */
	camera_setting_data.osd_rotate		= g_camera_cntx.osd_rotate;
	/* default effect */
	camera_setting_data.effect			= camera_effect_command_map[g_camera_cntx.setting.effect];//chenhe for jasper
	//camera_setting_data.effect			= g_camera_cntx.setting.effect;
	camera_setting_data.brightness 	= cool_camera_brightness_command_map[g_camera_cntx.setting.reserve5];
	camera_setting_data.saturation 	= 128;
#if defined(__MMI_MAINLCD_220X176__)
	camera_setting_data.contrast 		= cool_camera_contrast_command_map[g_camera_cntx.setting.contrast];
#else
	camera_setting_data.contrast 		= cool_camera_contrast_command_map[g_camera_cntx.setting.reserve6];
#endif
	camera_setting_data.hue		 		= 0;

#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
	if(g_camera_cntx.setting.effect == CAMERA_SETTING_EFFECT_CUSTOM_FX_1)
	{
		camera_setting_data.effect			= camera_effect_command_map[CAMERA_SETTING_EFFECT_NORMAL];
		camera_setting_data.brightness 	= g_camera_cntx.setting.fx_brightness[0];
		camera_setting_data.saturation 	= g_camera_cntx.setting.fx_saturation[0];	
		camera_setting_data.contrast 		= g_camera_cntx.setting.fx_contrast[0];
		camera_setting_data.hue		 		= g_camera_cntx.setting.fx_hue[0];		
	}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */

#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_2__
	if(g_camera_cntx.setting.effect == CAMERA_SETTING_EFFECT_CUSTOM_FX_2)
	{
		camera_setting_data.effect			= camera_effect_command_map[CAMERA_SETTING_EFFECT_NORMAL];
		camera_setting_data.brightness 	= g_camera_cntx.setting.fx_brightness[1];
		camera_setting_data.saturation 	= g_camera_cntx.setting.fx_saturation[1];	
		camera_setting_data.contrast 		= g_camera_cntx.setting.fx_contrast[1];
		camera_setting_data.hue		 		= g_camera_cntx.setting.fx_hue[1];	
	}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_2__ */

#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_3__
	if(g_camera_cntx.setting.effect == CAMERA_SETTING_EFFECT_CUSTOM_FX_3)
	{
		camera_setting_data.effect			= camera_effect_command_map[CAMERA_SETTING_EFFECT_NORMAL];
		camera_setting_data.brightness 	= g_camera_cntx.setting.fx_brightness[2];
		camera_setting_data.saturation 	= g_camera_cntx.setting.fx_saturation[2];	
		camera_setting_data.contrast 		= g_camera_cntx.setting.fx_contrast[2];
		camera_setting_data.hue		 		= g_camera_cntx.setting.fx_hue[2];	
	}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_3__ */

	
	camera_setting_data.preview_width	= g_camera_cntx.preview_width;
	camera_setting_data.preview_height	= g_camera_cntx.preview_height;	
	
	/* capture parameters */
	camera_setting_data.image_qty			= camera_image_qty_command_map[g_camera_cntx.setting.image_qty];
	if(g_camera_cntx.setting.image_qty == 0) //chenhe
	{
		camera_setting_data.image_qty = 0;
	}
	else
	{
		camera_setting_data.image_qty = 1;
	}
	camera_setting_data.image_width		= g_camera_cntx.image_width;
	camera_setting_data.image_height		= g_camera_cntx.image_height;	

	camera_setting_data.lcm					= MDI_CAMERA_PREVIEW_LCM_MAINLCD;
	camera_setting_data.preview_rotate	= CAMERA_PREVIEW_MAINLCD_ROTATE;
	camera_setting_data.lcm_rotate		= CAMERA_LCM_MAINLCD_ROTATE;

	/* turn on led highlight */
	mmi_camera_turn_on_preview_led_highlight();

	/* 
 
		we have to clear the preview layer's background to transparent,
		and we have to resoret it after preview stop (stopped by capture command)
	*/
		
	#ifdef CAMERA_MODULE_WITH_LCD
		gdi_layer_clear(CAMEAR_BG_TRASN_COLOR);
	#endif /* CAMERA_MODULE_WITH_LCD */


	mmi_trace(g_sw_CAMERA, "g_camera_cntx.is_tileshot = %d", g_camera_cntx.is_tileshot);
	
	if(g_camera_cntx.is_tileshot)
	{
		#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
			preview_layer_handle	= g_camera_cntx.preview_layer_handle;

		#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
			if(g_camera_cntx.osd_rotate ==CAMERA_DISPLAY_ROTATE_270)
			{
				offset_x = (g_camera_cntx.tile_cur_num%2)*(LCD_HEIGHT>>1);
				offset_y = (g_camera_cntx.tile_cur_num/2)*(LCD_WIDTH>>1);			
			}
			else if (g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_0)
			{
				switch(g_camera_cntx.tile_cur_num)
				{
					case 0:
						offset_x = 0;
						offset_y = LCD_WIDTH>>1;;
						break;
					case 1:
						offset_x = 0;
						offset_y = 0;
						break;
					case 2:
						offset_x = LCD_HEIGHT>>1;
						offset_y = LCD_WIDTH>>1;
						break;
					case 3:
						offset_x = LCD_HEIGHT>>1;
						offset_y = 0;
						break;
					default:
						break;
				}
			}
#else
				offset_x = (g_camera_cntx.tile_cur_num%2)*(LCD_WIDTH>>1);
				offset_y = (g_camera_cntx.tile_cur_num/2)*(LCD_HEIGHT>>1);
#endif
			mmi_trace(g_sw_CAMERA, "tile_cur_num = %d", g_camera_cntx.tile_cur_num);
			mmi_trace(g_sw_CAMERA, "offset_x = %d", offset_x);
			mmi_trace(g_sw_CAMERA, "offset_y = %d", offset_y);

			gdi_layer_push_and_set_active(g_camera_cntx.preview_layer_handle);
			gdi_layer_set_position(offset_x, offset_y);
			gdi_layer_pop_and_restore_active();
			
			/* tile shot, will draw on full tile preview layer, start from 0, 0 */
			preview_wnd_offset_x = 0;
			preview_wnd_offset_y = 0;
			preview_wnd_width		= g_camera_cntx.preview_width;	
			preview_wnd_height	= g_camera_cntx.preview_height;	
			
			#if defined(CAMERA_MODULE)	
				/* not support now */
				MMI_ASSERT(0);						
			#elif defined(ISP_SUPPORT)			
				/* tile result, preview, frame, OSD */
				blt_layer_flag			= GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1|GDI_LAYER_ENABLE_LAYER_2;
				preview_layer_flag 	= GDI_LAYER_ENABLE_LAYER_1;				
			#endif		
		#else
			MMI_ASSERT(0);
		#endif
	}
	else
	{
		preview_layer_handle	= g_camera_cntx.preview_layer_handle;
		
		/* use preview wnd's position */
		preview_wnd_offset_x	 =	g_camera_cntx.active_camera_osd_ptr->preview_wnd.offset_x;
		preview_wnd_offset_y	 =	g_camera_cntx.active_camera_osd_ptr->preview_wnd.offset_y;
		preview_wnd_width		 =	g_camera_cntx.preview_width;
		preview_wnd_height	 =	g_camera_cntx.preview_height;

		if(!((preview_wnd_width==LCD_WIDTH) && (preview_wnd_height==LCD_HEIGHT)))
		{
			preview_wnd_offset_x = 0;
			preview_wnd_offset_y = LCD_HEIGHT - LCD_WIDTH;
		}


		mmi_trace(g_sw_CAMERA, "offset_x = %ld,offset_y = %ld,width = %ld,height = %ld",g_camera_cntx.active_camera_osd_ptr->preview_wnd.offset_x,g_camera_cntx.active_camera_osd_ptr->preview_wnd.offset_y,g_camera_cntx.preview_width,g_camera_cntx.preview_height);
				
		/* preview (hw), OSD */
		#if (defined(MT6227)||defined(MT6226)||defined(MT6226M)||defined(MT6228)||defined(MT6229))
			blt_layer_flag 		=	GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1;
			preview_layer_flag	= 	GDI_LAYER_ENABLE_LAYER_0;
		#else
			blt_layer_flag 		=	GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1|GDI_LAYER_ENABLE_LAYER_2;
			preview_layer_flag	= 	GDI_LAYER_ENABLE_LAYER_1;
		#endif				
	}

	/* TODO: should cal a most large size to use buffer */
	#ifdef __MMI_TVOUT__
		if(mdi_tvout_is_enable())
		{
			if(mmi_phnset_tvout_get_style() == SETTING_TVOUT_STYLE_FULLSCREEN)
			{
				mdi_tvout_resize_buffer(g_camera_cntx.preview_width, g_camera_cntx.preview_height);
			}
		}
	#endif /* __MMI_TVOUT__ */	
	
#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

	g_camera_cntx.last_error 
		= mdi_camera_preview_start(preview_layer_handle,
											preview_wnd_offset_x,
											preview_wnd_offset_y,
											preview_wnd_width,
											preview_wnd_height,
											blt_layer_flag,
											preview_layer_flag,
											GDI_COLOR_TRANSPARENT,
											TRUE,
											&camera_setting_data);

}

/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_stop()
* DESCRIPTION
*   stop previewing of main lcd
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_stop(void)
{
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* stop previewing */
    mdi_camera_preview_stop();

    /* restore black background */
#ifdef CAMERA_MODULE_WITH_LCD
    mmi_camera_preview_clear_background();
#endif /* CAMERA_MODULE_WITH_LCD */	
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_set_blt_layer()
* DESCRIPTION
*   set blt layer based on current status (ex: is previwing, has add frame, etc )
* PARAMETERS
*	 is_previewing 	IN 	is preview flag
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_set_blt_layer(BOOL is_previewing)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#ifdef WIN32 
		/* for simulators, do not have hw update layer */
		is_previewing = FALSE;
	#endif 
   
	#if defined(CAMERA_MODULE)
		gdi_layer_set_blt_layer(g_camera_cntx.base_layer_handle,
											g_camera_cntx.preview_layer_handle,
											g_camera_cntx.osd_layer_handle,
											0);
	#elif defined(ISP_SUPPORT)
		if(is_previewing)
		{
			if(g_camera_cntx.is_tileshot)
			{
				#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
					/* (1) previewing (2) tile shot */
					gdi_layer_set_blt_layer(g_camera_cntx.tileshot_result_layer_handle,
													g_camera_cntx.preview_layer_handle,														
													g_camera_cntx.osd_layer_handle,
													0);
				#else
					MMI_ASSERT(0);
				#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */
			}
			else
			{

				/* (1) previwing */
				#if (defined(MT6227)||defined(MT6226)||defined(MT6226M)||defined(MT6228)||defined(MT6229))
				gdi_layer_set_blt_layer(g_camera_cntx.preview_layer_handle,
												g_camera_cntx.osd_layer_handle,
												0,
												0);
				#else
				gdi_layer_set_blt_layer(g_camera_cntx.base_layer_handle,
												g_camera_cntx.preview_layer_handle,
												g_camera_cntx.osd_layer_handle,
												0);
				#endif
			}
		}
		else
		{
			if(g_camera_cntx.is_tileshot)
			{
				#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__			
					/* (1) not previewing (2) tile shot */
					gdi_layer_set_blt_layer(g_camera_cntx.tileshot_result_layer_handle,
													g_camera_cntx.osd_layer_handle,
													0,
													0);
				#else
					MMI_ASSERT(0);
				#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */			
			}
			else
			{
				/*  not previewing.*/
				#if (defined(MT6227)||defined(MT6226)||defined(MT6226M)||defined(MT6228)||defined(MT6229))
				gdi_layer_set_blt_layer(g_camera_cntx.base_layer_handle,
												g_camera_cntx.osd_layer_handle,
												0,
												0);
				#else
				gdi_layer_set_blt_layer(g_camera_cntx.base_layer_handle,
												g_camera_cntx.preview_layer_handle,
												g_camera_cntx.osd_layer_handle,
												0);

				#endif
			}
		}
	#endif /* chip version */

}


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_clear_background()
* DESCRIPTION
*   clear backgroud of preview screen
* PARAMETERS
*   nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_clear_background(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
	/*----------------------------------------------------------------*/
	gdi_color 	bg_color;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	gdi_layer_push_and_set_active(g_camera_cntx.base_layer_handle);
   
	if(g_camera_cntx.active_camera_osd_ptr->bg.is_draw_bg_color)
	{
		bg_color = GDI_RGB_TO_BUFFER_FORMAT(g_camera_cntx.active_camera_osd_ptr->bg.r, g_camera_cntx.active_camera_osd_ptr->bg.g, g_camera_cntx.active_camera_osd_ptr->bg.b);

		gdi_layer_clear(bg_color);
	}

	if(g_camera_cntx.active_camera_osd_ptr->bg.is_draw_bg_image)
	{
		gdi_image_draw_id(0, 0, IMG_ID_CAMERA_PREVIEW_BACKGROUND);
	}

	if(!g_camera_cntx.active_camera_osd_ptr->bg.is_draw_bg_color && !g_camera_cntx.active_camera_osd_ptr->bg.is_draw_bg_image)
	{
		gdi_layer_clear(GDI_COLOR_BLACK);
	}

	gdi_layer_pop_and_restore_active();

	#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
		/* tile preview back groud */
	if(g_camera_cntx.preview_layer_handle!=GDI_LAYER_EMPTY_HANDLE)
	{
		gdi_layer_push_and_set_active(g_camera_cntx.preview_layer_handle);
		gdi_layer_clear(GDI_COLOR_BLACK);
		gdi_layer_pop_and_restore_active();
	}
	#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */

}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_flatten_blt_layer()
* DESCRIPTION
*   flatten layers to base
* PARAMETERS
*   nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_flatten_layer(BOOL is_previewing)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#if defined(CAMERA_MODULE)

	if(is_previewing)
	{
		gdi_layer_flatten_to_base(	g_camera_cntx.base_layer_handle,
											g_camera_cntx.preview_layer_handle, 
											g_camera_cntx.osd_layer_handle,
											0);
	}
	else
	{
		gdi_layer_flatten_to_base(	g_camera_cntx.base_layer_handle, 
											g_camera_cntx.osd_layer_handle,
											0, 
											0);
	}
#elif defined(ISP_SUPPORT)

	if(is_previewing)
	{
		if(g_camera_cntx.is_tileshot)
		{
			gdi_layer_flatten_to_base(	g_camera_cntx.tileshot_result_layer_handle,
												g_camera_cntx.osd_layer_handle, 
												0,
												0);
		}
		else
		{
			#if (defined(MT6226)||defined(MT6227)||defined(MT6226M)||defined(MT6228)||defined(MT6229))
			gdi_layer_flatten_to_base(	g_camera_cntx.preview_layer_handle, 
												g_camera_cntx.osd_layer_handle,
												0,
												0);
			#else
			gdi_layer_flatten_to_base(	g_camera_cntx.base_layer_handle,
												g_camera_cntx.preview_layer_handle, 
												g_camera_cntx.osd_layer_handle, 
												0);
			#endif
		}
	}
	else
	{
	
		gdi_layer_flatten_to_base(	g_camera_cntx.base_layer_handle, 
											g_camera_cntx.osd_layer_handle,
											0, 
											0);
	}

#endif /* chip version */

}

/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_hide_hint()
* DESCRIPTION
*   hide hint
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_hide_hint(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	mmi_camera_preview_set_hint(NULL, NULL, 255);
 	mmi_camera_preview_draw_osd();
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
}


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_draw_osd()
* DESCRIPTION
*   draw preview title
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
INT16 mmi_getcameraFrame(void) //chenhe add
{
	return g_camera_cntx.setting.add_frame;
}
static void mmi_camera_preview_draw_osd(void)
{
    mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_preview_draw_osd()");
  	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
    lcd_lock_all_blt_operation();
    
	gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);

	/* toggle double buffer */
	gdi_layer_toggle_double();

	
	gdi_layer_clear(GDI_COLOR_TRANSPARENT);//fengzb
	mmi_trace(g_sw_CAMERA,"g_camera_cntx.is_fullscreen=%d,g_camera_cntx.setting.add_frame=%d",g_camera_cntx.is_fullscreen,g_camera_cntx.setting.add_frame);
//  gdi_layer_clear(GDI_COLOR_BLACK);//fengzb
	if(/* (g_camera_cntx.app_state != CAMERA_STATE_SAVE_CONFIRM) && 
		 (g_camera_cntx.app_state != CAMERA_STATE_BURSTSHOT_SAVE_CONFIRM)&& *///chenhe for jasper
		 (g_camera_cntx.is_fullscreen) &&
		 (g_camera_cntx.setting.add_frame != 0))
	{
			PU8 src;
			PU8 dest;
			U16 src_width,src_height,src_pitch;
			S32 src_offset_x,src_offset_y;
			S32 dest_pitch, dest_offset_x, dest_offset_y	;
			gdi_rect_struct dest_clip;
            mmi_trace(g_sw_CAMERA,"draw frame****************");
			gdi_layer_get_buffer_ptr(&dest);

			gdi_layer_push_and_set_active(g_camera_cntx.add_frame_layer_handle);
			gdi_layer_get_buffer_ptr(&src);

			src_pitch = g_camera_cntx.osd_UI_device_width;
			dest_pitch = g_camera_cntx.osd_UI_device_width;
			dest_clip.x1 = 0;
			dest_clip.y1 = 0;
			dest_clip.x2 = g_camera_cntx.osd_UI_device_width-1;
			dest_clip.y2 = g_camera_cntx.osd_UI_device_height-1;
			src_offset_x = 0;
			src_offset_y = 0;
			src_width =  g_camera_cntx.osd_UI_device_width;
			src_height = g_camera_cntx.osd_UI_device_height;
			dest_offset_y = 0;
			dest_offset_x = 0;
			gdi_layer_pop_and_restore_active();

			gdi_2d_memory_blt_without_transpant_check(src, src_pitch, src_offset_x, src_offset_y, src_width, src_height,
				dest, dest_pitch, dest_offset_x, dest_offset_y,dest_clip);
		}

		mmi_camera_preview_draw_softkey();
		mmi_camera_preview_draw_title();

	if(g_camera_cntx.fade_time !=255)
	{
		mmi_camera_preview_draw_hint();
		g_camera_cntx.fade_time = 0;
	}
		
	if( (g_camera_cntx.app_state != CAMERA_STATE_SAVE_CONFIRM) && 
		 (g_camera_cntx.app_state != CAMERA_STATE_BURSTSHOT_SAVE_CONFIRM))	
	{
		mmi_camera_preview_draw_icon();	

		if(g_camera_cntx.app_state == CAMERA_STATE_CONTSHOT)
		{
			#ifdef __CAMERA_FEATURE_CONT_SHOT_DRAW_COUNTBAR__
				mmi_camera_preview_draw_contshot_countbar();
			#endif /* __CAMERA_FEATURE_CONT_SHOT_DRAW_COUNTBAR__ */
		}
	}

	gdi_layer_pop_and_restore_active();
    
    lcd_unlock_all_blt_operation();
}


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_draw_title()
* DESCRIPTION
*   draw preview title
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_draw_title(void)
{
	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_preview_draw_title()");
  	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	/* if is fullscreen display, do not draw title */
	if(g_camera_cntx.is_fullscreen)
	{
		return;
	}

	/* if not fullscreen and need title */
	if(g_camera_cntx.active_camera_osd_ptr->title.is_draw_mmi_title_bar && 
		g_camera_cntx.active_camera_osd_ptr->title.is_draw_mmi_caption)
	{
		gdi_layer_lock_frame_buffer();
		
		show_title_status_icon();
		draw_title();

		gdi_layer_unlock_frame_buffer();
		gdi_layer_reset_clip();
		gdi_layer_reset_text_clip();	
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_draw_softkey()
* DESCRIPTION
*   draw preview softkey
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_draw_softkey(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 lsk_offset_x = 0;
	S32 lsk_offset_y = 0;	
	S32 rsk_offset_x = 0;
	S32 rsk_offset_y = 0;	
	S32 str_width = 0;
	S32 str_height = 0;
	PS8 str_ptr;
	U16 lsk_id;
	U16 rsk_id;

    mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_preview_draw_softkey()");
  	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	
	if( (g_camera_cntx.app_state == CAMERA_STATE_SAVE_CONFIRM) || 
		 (g_camera_cntx.app_state == CAMERA_STATE_BURSTSHOT_SAVE_CONFIRM) )
	{
		lsk_id = STR_GLOBAL_SAVE;
		rsk_id = STR_GLOBAL_CLEAR;
	}
	else
	{
		lsk_id = STR_GLOBAL_OPTIONS;

		/* if only have 23key, use rsk to capture */
		#ifdef __MMI_CAMERA_23KEY__
			rsk_id = STR_ID_CAMERA_RSK_CAPTURE;
		#else
			rsk_id = STR_GLOBAL_BACK;
		#endif 

		if( (g_camera_cntx.app_state == CAMERA_STATE_PREVIEW) &&
			 (g_camera_cntx.tile_cur_num >= 1) )
		{
			rsk_id = STR_GLOBAL_CLEAR;
		}
	}

	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();

	/* if is fullscreen display, or normal display but without mmi_softkey */
	if( (g_camera_cntx.is_fullscreen) || (!g_camera_cntx.active_camera_osd_ptr->softkey.is_draw_mmi_softkey) )
	{
		coolsand_UI_set_font(&MMI_medium_font);

		/* lsk */
		str_ptr = (PS8)GetString(lsk_id);

		coolsand_UI_measure_string((UI_string_type)str_ptr, &str_width, &str_height);

		if(g_camera_cntx.osd_rotate ==CAMERA_DISPLAY_ROTATE_270)
		{
#if defined(__CAMERA_OSD_HORIZONTAL__) || defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
			lsk_offset_x = MMI_ROTATED_BUTTON_BAR_X + (MMI_ROTATED_BUTTON_BAR_WIDTH-str_width)-1;
			lsk_offset_y = MMI_ROTATED_BUTTON_BAR_Y + MMI_ROTATED_BUTTON_BAR_HEIGHT - str_height - 2;
#endif
		}
		else
		{
		lsk_offset_x = 2;
		lsk_offset_y = (g_camera_cntx.osd_UI_device_height-MMI_button_bar_height)+((MMI_button_bar_height-str_height)>>1)+1;
		}

		if(g_camera_cntx.is_lsk_pressed)
		{
			lsk_offset_x++;
			lsk_offset_y++;	
		}

		mmi_camera_draw_style_text(str_ptr, 
											lsk_offset_x,
											lsk_offset_y,
											&g_camera_cntx.active_camera_osd_ptr->softkey.lsk);
		
		g_camera_cntx.touch_lsk.offset_x = lsk_offset_x;
		g_camera_cntx.touch_lsk.offset_y = lsk_offset_y;
		g_camera_cntx.touch_lsk.width 	= str_width;
		g_camera_cntx.touch_lsk.height 	= str_height;

		/* rsk */
		str_ptr = (PS8)GetString(rsk_id); 

		coolsand_UI_measure_string((UI_string_type)str_ptr, &str_width, &str_height);
		if(g_camera_cntx.osd_rotate ==CAMERA_DISPLAY_ROTATE_270)
		{
		#if defined(__CAMERA_OSD_HORIZONTAL__) || defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
			rsk_offset_x = MMI_ROTATED_BUTTON_BAR_X + (MMI_ROTATED_BUTTON_BAR_WIDTH-str_width)-1;
			rsk_offset_y = 2;
		#endif
		}
		else
		{
		rsk_offset_x = (g_camera_cntx.osd_UI_device_width-str_width-2);
		rsk_offset_y = (g_camera_cntx.osd_UI_device_height-MMI_button_bar_height)+((MMI_button_bar_height-str_height)>>1)+1;
		}
		if(g_camera_cntx.is_rsk_pressed)
		{
			rsk_offset_x++;
			rsk_offset_y++;	
		}
		
		mmi_camera_draw_style_text(str_ptr, 
											rsk_offset_x,
											rsk_offset_y,
											&g_camera_cntx.active_camera_osd_ptr->softkey.rsk);
		
		g_camera_cntx.touch_rsk.offset_x	= rsk_offset_x;
		g_camera_cntx.touch_rsk.offset_y	= rsk_offset_y;
		g_camera_cntx.touch_rsk.width		= str_width;
		g_camera_cntx.touch_rsk.height	= str_height;
		mmi_trace(g_sw_CAMERA, "funs: is full screen!!");

	}
	else
	{
		if(g_camera_cntx.is_rsk_pressed)
		{
			set_softkey_flags_on(UI_BUTTON_STATE_CLICKED|UI_BUTTON_STATE_DOWN, MMI_RIGHT_SOFTKEY);
		}
		else
		{
			set_softkey_flags_off(UI_BUTTON_STATE_CLICKED,MMI_RIGHT_SOFTKEY);
			set_softkey_flags_off(UI_BUTTON_STATE_DOWN,MMI_RIGHT_SOFTKEY);
		}

		if(g_camera_cntx.is_lsk_pressed)
		{
			set_softkey_flags_on(UI_BUTTON_STATE_CLICKED|UI_BUTTON_STATE_DOWN, MMI_LEFT_SOFTKEY);
		}
		else
		{
			set_softkey_flags_off(UI_BUTTON_STATE_CLICKED,MMI_LEFT_SOFTKEY);
			set_softkey_flags_off(UI_BUTTON_STATE_DOWN,MMI_LEFT_SOFTKEY);
		}
		
		change_left_softkey(lsk_id, IMG_GLOBAL_OK);
		change_right_softkey(rsk_id, IMG_GLOBAL_BACK);

		/* draw MMI standard softkey */
		gdi_layer_lock_frame_buffer();
		
		show_softkey_background();
		
		show_left_softkey(); 
		show_right_softkey();

		gdi_layer_unlock_frame_buffer();


	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_draw_hint()
* DESCRIPTION
*   draw preview OSD panel
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_draw_hint(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 	str1_width; 
	S32 	str1_height;
	S32 	str2_width; 
	S32 	str2_height;
	S32 	str1_offset_x;
	S32 	str1_offset_y;
	S32 	str2_offset_x;
	S32 	str2_offset_y;
	S32 	spacing;
	PS8	hint_str1;
	PS8	hint_str2;


    mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_preview_draw_hint()");
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	
	/* clear hint background region */

	/* draw hint */
	if(g_camera_cntx.active_camera_osd_ptr->hint_box.is_show)
	{
		hint_str1 = g_camera_cntx.hint1_buf;
		hint_str2 = g_camera_cntx.hint2_buf;
	
		/* be removed : clear hint backgounrd */

		coolsand_UI_set_text_clip(g_camera_cntx.active_camera_osd_ptr->hint_box.offset_x, 
										g_camera_cntx.active_camera_osd_ptr->hint_box.offset_y,
										g_camera_cntx.active_camera_osd_ptr->hint_box.offset_x+g_camera_cntx.active_camera_osd_ptr->hint_box.width-1,
										g_camera_cntx.active_camera_osd_ptr->hint_box.offset_y+g_camera_cntx.active_camera_osd_ptr->hint_box.height-1);
				
		coolsand_UI_measure_string((UI_string_type)hint_str1, &str1_width, &str1_height); 
		coolsand_UI_measure_string((UI_string_type)hint_str2, &str2_width, &str2_height); 

		str1_offset_x = (g_camera_cntx.active_camera_osd_ptr->hint_box.width - str1_width)>>1;

		spacing = g_camera_cntx.active_camera_osd_ptr->hint_box.height-str1_height-str2_height;
		spacing = (spacing > 0) ? (spacing/3+1) : 0;
		
		str1_offset_y 	= spacing;	
		str2_offset_y	= str1_offset_y+spacing+str1_height;
		str2_offset_x 	= (g_camera_cntx.active_camera_osd_ptr->hint_box.width - str2_width)>>1;

		gdi_layer_reset_clip();
		coolsand_UI_reset_text_clip();

		if( (g_camera_cntx.app_state == CAMERA_STATE_SAVE_CONFIRM) ||
 			 (g_camera_cntx.app_state == CAMERA_STATE_BURSTSHOT_SAVE_CONFIRM))
		{
			mmi_camera_draw_style_text(hint_str1, 
												g_camera_cntx.active_camera_osd_ptr->hint_box.offset_x+str1_offset_x,
												(MMI_title_height-str1_height)>>1,
												&g_camera_cntx.active_camera_osd_ptr->hint_box.style_text);		
		}
		else
		{
			mmi_camera_draw_style_text(hint_str1, 
												g_camera_cntx.active_camera_osd_ptr->hint_box.offset_x+str1_offset_x,
												g_camera_cntx.active_camera_osd_ptr->hint_box.offset_y+str1_offset_y,
												&g_camera_cntx.active_camera_osd_ptr->hint_box.style_text);		
											
			mmi_camera_draw_style_text(hint_str2, 
												g_camera_cntx.active_camera_osd_ptr->hint_box.offset_x+str2_offset_x,
												g_camera_cntx.active_camera_osd_ptr->hint_box.offset_y+str2_offset_y,
												&g_camera_cntx.active_camera_osd_ptr->hint_box.style_text);	
		}
			

		if(g_camera_cntx.fade_time != 0 && g_camera_cntx.fade_time !=255)
		{
			coolsand_UI_start_timer(HINT_POPUP_FADE_TIME, mmi_camera_preview_hide_hint);
			g_camera_cntx.fade_time = 0;
		}
	}
	
}

S32 mmi_camera_get_zoom_total_level_cnt()
{
	S32 i;
	for (i=g_camera_cntx.setting.zoom_idx;i<CAMERA_FEATURE_ZOOM_COUNT;i++)
	{
		if(g_camera_feature_zoom[i] == g_camera_cntx.zoom_limit)
			break;
	}
	return i;
}

/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_draw_icon()
* DESCRIPTION
*   draw preview OSD panel
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_draw_icon(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8 	str_buf[256];
	U8		str_buf_UCS2[512];	
	S32 	str_width;
	S32 	str_height;	
	
	#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
		S32	icon_offset_x;	
		S32	icon_offset_y;	
		S32	image_width;
		S32	image_height;		
		S32 	icon_bar_width;
		S32	icon_bar_height;		
		S32	index;		
		S32	offset_x;
		S32	offset_y;
		S32	i;		
	#endif /* __CAMERA_FEATURE_ICONBAR_PANEL__ */

	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_previewer_draw_icon()");

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__

		/******************************************************/
		/*						Draw Icon List 							*/
		/******************************************************/
		/****** draw icon MACRO ******/
		#define ICONBAR_DRAW_SWITCH(__a__,__A__)\
		{\
			case CAMERA_ICONBAR_##__A__##:\
				gdi_image_get_dimension_id((U16)(IMG_ID_CAMERA_OSD_##__A__##_START+g_camera_cntx.setting.##__a__##+1),\
													&image_width,\
													&image_height);\
				icon_offset_x = ((icon_bar_width-image_width)>>1);\
				icon_offset_y = ((icon_bar_height-image_height)>>1);\
				icon_offset_x += offset_x;\
				icon_offset_y += offset_y;\
				if(index != g_camera_cntx.iconbar_selected_idx)\
				{\
					gdi_image_draw_id(offset_x, \
											offset_y, \
											IMG_ID_CAMERA_ICONBAR_ITEM);\
				}\
				else\
				{\
					gdi_image_draw_id(offset_x, \
											offset_y, \
											IMG_ID_CAMERA_ICONBAR_ITEM_SEL);\
				}\
				gdi_image_draw_id(icon_offset_x, \
										icon_offset_y, \
										(U16)(IMG_ID_CAMERA_OSD_##__A__##_START+g_camera_cntx.setting.##__a__##+1));\
				offset_x += g_camera_cntx.iconbar_item_width;\
				break;\
		}
		/* draw left arrow */
		offset_x = g_camera_cntx.active_camera_osd_ptr->iconbar.offset_x;
		offset_y = g_camera_cntx.active_camera_osd_ptr->iconbar.offset_y;	
		gdi_image_draw_id(offset_x, offset_y, IMG_ID_CAMERA_ICONBAR_LEFT_ARROW);
		gdi_image_get_dimension_id(IMG_ID_CAMERA_ICONBAR_LEFT_ARROW, &image_width, &image_height);
		
		offset_x += image_width;
		
		/* get iconbar width*/	
		gdi_image_get_dimension_id((U16)(IMG_ID_CAMERA_ICONBAR_ITEM), &icon_bar_width, &icon_bar_height);


		for(	i = g_camera_cntx.iconbar_start_idx; 
				i < g_camera_cntx.iconbar_start_idx+g_camera_cntx.iconbar_item_per_page; 
				i++)
		{
			if(i < g_camera_cntx.iconbar_item_count)
			{
				index = i;
			}
			else
			{
				index = i-g_camera_cntx.iconbar_item_count;
			}
		
			switch(g_camera_cntx.iconbar_item[index])
			{
				ICONBAR_DRAW_SWITCH(zoom_idx,ZOOM);
				ICONBAR_DRAW_SWITCH(ev,EV);				
				ICONBAR_DRAW_SWITCH(effect,EFFECT);		
				ICONBAR_DRAW_SWITCH(shot_mode,SHOT_MODE);				
				ICONBAR_DRAW_SWITCH(wb,WB);
				ICONBAR_DRAW_SWITCH(flash,FLASH);
				ICONBAR_DRAW_SWITCH(led_highlight,LED_HIGHLIGHT);
				ICONBAR_DRAW_SWITCH(night,NIGHT);
				ICONBAR_DRAW_SWITCH(closeup,CLOSEUP);
				ICONBAR_DRAW_SWITCH(iso,ISO);
				ICONBAR_DRAW_SWITCH(ae_meter,AE_METER);
				ICONBAR_DRAW_SWITCH(cont_shot,CONT_SHOT);		
				ICONBAR_DRAW_SWITCH(delay_timer,DELAY_TIMER);		
				ICONBAR_DRAW_SWITCH(image_size,IMAGE_SIZE);
				ICONBAR_DRAW_SWITCH(image_qty,IMAGE_QTY);
				ICONBAR_DRAW_SWITCH(storage,STORAGE);
				case CAMERA_ICONBAR_IMAGE_SIZE:
				if(g_camera_cntx.is_tileshot == TRUE)
				gdi_image_get_dimension_id((U16)mmi_camera_get_img_size_img_id(LCD_WIDTH,LCD_HEIGHT),
													&image_width,
													&image_height);
				else
				gdi_image_get_dimension_id((U16)mmi_camera_get_img_size_img_id(g_camera_cntx.image_width,g_camera_cntx.image_height),
													&image_width,
													&image_height);

				icon_offset_x = ((icon_bar_width-image_width)>>1);
				icon_offset_y = ((icon_bar_height-image_height)>>1);
				icon_offset_x += offset_x;
				icon_offset_y += offset_y;
				if(index != g_camera_cntx.iconbar_selected_idx)
				{
					gdi_image_draw_id(offset_x, 
											offset_y, 
											IMG_ID_CAMERA_ICONBAR_ITEM);
				}
				else
				{
					gdi_image_draw_id(offset_x, 
											offset_y, 
											IMG_ID_CAMERA_ICONBAR_ITEM_SEL);
				}
				if(g_camera_cntx.is_tileshot == TRUE)
				gdi_image_draw_id(icon_offset_x,
										icon_offset_y,
										(U16)mmi_camera_get_img_size_img_id(LCD_WIDTH,LCD_HEIGHT));
				else
				gdi_image_draw_id(icon_offset_x, 
										icon_offset_y, 
										(U16)mmi_camera_get_img_size_img_id(g_camera_cntx.image_width,g_camera_cntx.image_height));
				offset_x += g_camera_cntx.iconbar_item_width;
				break;


				case CAMERA_ICONBAR_ADD_FRAME:
					gdi_image_get_dimension_id((U16)(IMG_ID_CAMERA_OSD_ADD_FRAME_START+g_camera_cntx.setting.add_frame+1),
														&image_width,
														&image_height);
					
					icon_offset_x = ((icon_bar_width-image_width)>>1);
					icon_offset_y = ((icon_bar_height-image_height)>>1);
					icon_offset_x += offset_x;
					icon_offset_y += offset_y;

					if(g_camera_cntx.is_fullscreen)
					{
						if(index != g_camera_cntx.iconbar_selected_idx)
						{
							gdi_image_draw_id(offset_x, 
													offset_y,
													IMG_ID_CAMERA_ICONBAR_ITEM);
						}
						else
						{
							gdi_image_draw_id(offset_x, 
													offset_y,
													IMG_ID_CAMERA_ICONBAR_ITEM_SEL);
						}
					}
					else
					{
						gdi_image_draw_id(offset_x, 
												offset_y,
												IMG_ID_CAMERA_ICONBAR_ITEM_DIS);
					}
					
					gdi_image_draw_id(icon_offset_x,
											icon_offset_y,
											(U16)(IMG_ID_CAMERA_OSD_ADD_FRAME_START+g_camera_cntx.setting.add_frame+1));
					
					offset_x += g_camera_cntx.iconbar_item_width;
					break;
				}
			}
		/* draw right arrow */
		gdi_image_draw_id(offset_x, offset_y, IMG_ID_CAMERA_ICONBAR_RIGHT_ARROW);
		
	#else
	
		/******************************************************/
		/*						Draw normal OSD icons 					*/
		/******************************************************/
	
		/****** draw icon MACRO ******/
/*		#define DRAW_OSD_ICON(a,A)\
		do {\
			if(g_camera_cntx.active_camera_osd_ptr->##a##.is_show)\
			{\
				gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->##a##.offset_x, \
										g_camera_cntx.active_camera_osd_ptr->##a##.offset_y, \
										(U16)(IMG_ID_CAMERA_OSD_##A##_START+g_camera_cntx.setting.##a##+1));\
			}\
		} while(0);*/
		#define DRAW_OSD_ICON(a,A)\
		do {\
			if(g_camera_cntx.active_camera_osd_ptr->a.is_show)\
			{\
				gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->a.offset_x, \
										g_camera_cntx.active_camera_osd_ptr->a.offset_y, \
										(U16)(IMG_ID_CAMERA_OSD_##A##_START+g_camera_cntx.setting.a+1));\
			}\
		} while(0);

		mmi_trace(g_sw_CAMERA, "IMG_ID_CAMERA_OSD_EV_INC = %d", IMG_ID_CAMERA_OSD_EV_INC);
		mmi_trace(g_sw_CAMERA, "IMG_ID_CAMERA_APP_ICON   = %d", IMG_ID_CAMERA_APP_ICON);
		mmi_trace(g_sw_CAMERA, "draw osd icon -------------");
		if(!g_camera_cntx.is_hide_osd)
		{
			/* draw icon */
			#ifdef __CAMERA_FEATURE_EFFECT__
			DRAW_OSD_ICON(effect,EFFECT);	
			#endif

			#ifdef __CAMERA_FEATURE_WB__
			DRAW_OSD_ICON(wb,WB);
			#endif

			#ifdef __CAMERA_FEATURE_FLASH__			
			DRAW_OSD_ICON(flash,FLASH);
			#endif

			#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT__
			DRAW_OSD_ICON(led_highlight,LED_HIGHLIGHT);
			#endif

			#ifdef __CAMERA_FEATURE_NIGHT__
			DRAW_OSD_ICON(night,NIGHT);
			#endif


			DRAW_OSD_ICON(closeup,CLOSEUP);	
		
		
			DRAW_OSD_ICON(image_qty,IMAGE_QTY);

			DRAW_OSD_ICON(image_size,IMAGE_SIZE); //add by WeiD


#ifndef MEM_ULC_3216
mmi_trace(g_sw_CAMERA,"effect.offset_x = %d, effect.offset_y = %d",g_camera_cntx.active_camera_osd_ptr->effect.offset_x,g_camera_cntx.active_camera_osd_ptr->effect.offset_y);
mmi_trace(g_sw_CAMERA, "start_effect = %ld,EFFECT_1 = %ld,g_camera_cntx.setting.effect = %d",IMG_ID_CAMERA_OSD_EFFECT_START,IMG_ID_CAMERA_OSD_EFFECT_NORMAL,g_camera_cntx.setting.effect);
mmi_trace(g_sw_CAMERA, "start = %ld,ZOOM_1 = %ld,g_camera_cntx.setting.zoom_idx = %d",IMG_ID_CAMERA_OSD_ZOOM_START,IMG_ID_CAMERA_OSD_ZOOM_1,g_camera_cntx.setting.zoom_idx);
#endif



			#ifdef __CAMERA_FEATURE_CONT_SHOT__
			DRAW_OSD_ICON(cont_shot,CONT_SHOT);
			#endif

			#ifdef __CAMERA_FEATURE_DELAY_TIMER__			
			DRAW_OSD_ICON(delay_timer,DELAY_TIMER);
			#endif

			#ifdef __CAMERA_FEATURE_STORAGE__			
			DRAW_OSD_ICON(storage,STORAGE);	
			#endif

			//for brightness //it's just for brightness,not ev BEGIN
			if(g_camera_cntx.active_camera_osd_ptr->ev.is_show)
			{
				gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->ev.offset_x, 
								g_camera_cntx.active_camera_osd_ptr->ev.offset_y, 
								(U16)(IMG_ID_CAMERA_OSD_BRIGHT_1+g_camera_cntx.setting.reserve5));
			}  
			if(g_camera_cntx.active_camera_osd_ptr->ev_inc.is_show)
			{
				if(g_camera_cntx.touch_ev_inc.is_press)
				{
					gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->ev_inc.offset_x, 
											g_camera_cntx.active_camera_osd_ptr->ev_inc.offset_y,
											(U16)(IMG_ID_CAMERA_OSD_EV_INC_SEL));
				}
				else
				{
					gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->ev_inc.offset_x, 
											g_camera_cntx.active_camera_osd_ptr->ev_inc.offset_y,
											(U16)(IMG_ID_CAMERA_OSD_EV_INC));
				}
			}
			
			if(g_camera_cntx.active_camera_osd_ptr->ev_dec.is_show)
			{
				if(g_camera_cntx.touch_ev_dec.is_press)
				{
					gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->ev_dec.offset_x, 
											g_camera_cntx.active_camera_osd_ptr->ev_dec.offset_y,
											(U16)(IMG_ID_CAMERA_OSD_EV_DEC_SEL));
				}
				else
				{
					gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->ev_dec.offset_x, 
											g_camera_cntx.active_camera_osd_ptr->ev_dec.offset_y,
											(U16)(IMG_ID_CAMERA_OSD_EV_DEC));
				}
			}
//it's just for brightness,not ev END


			#if 0
			/* ev */
			#ifdef __CAMERA_FEATURE_EV__
				DRAW_OSD_ICON(ev,EV);

			
				if(g_camera_cntx.active_camera_osd_ptr->ev_inc.is_show)
				{
					if(g_camera_cntx.touch_ev_inc.is_press)
					{
						gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->ev_inc.offset_x, 
												g_camera_cntx.active_camera_osd_ptr->ev_inc.offset_y,
												(U16)(IMG_ID_CAMERA_OSD_EV_INC_SEL));
					}
					else
					{
						gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->ev_inc.offset_x, 
												g_camera_cntx.active_camera_osd_ptr->ev_inc.offset_y,
												(U16)(IMG_ID_CAMERA_OSD_EV_INC));
					}
				}
				
				if(g_camera_cntx.active_camera_osd_ptr->ev_dec.is_show)
				{
					if(g_camera_cntx.touch_ev_dec.is_press)
					{
						gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->ev_dec.offset_x, 
												g_camera_cntx.active_camera_osd_ptr->ev_dec.offset_y,
												(U16)(IMG_ID_CAMERA_OSD_EV_DEC_SEL));
					}
					else
					{
						gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->ev_dec.offset_x, 
												g_camera_cntx.active_camera_osd_ptr->ev_dec.offset_y,
												(U16)(IMG_ID_CAMERA_OSD_EV_DEC));
					}
				}
							
				#endif
		#endif
		
	#ifdef __CAMERA_FEATURE_ZOOM__
	
		#if 1 //qiff remvoe for bug : 13750 def LCDSIZE_240_320	//added by zhangl for cs2324	
			/* zooming icon */
			if(g_camera_cntx.active_camera_osd_ptr->zoom.is_show && g_camera_cntx.zoom_limit != 10)
			{
				S32 total_idx, current_idx, bar_width, bar_height,slider_x,slider_y,slider_width,slider_height;

				gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->zoom.offset_x, 
										g_camera_cntx.active_camera_osd_ptr->zoom.offset_y,
										IMG_ID_CAMERA_OSD_ZOOM_BAR);

				// CALCULATE THE SLIDER LOACTION AND DRAW
				total_idx = mmi_camera_get_zoom_total_level_cnt()-1;
				current_idx = g_camera_cntx.setting.zoom_idx;
				gdi_image_get_dimension_id(IMG_ID_CAMERA_OSD_ZOOM_BAR, &bar_width, &bar_height);
				gdi_image_get_dimension_id(IMG_ID_CAMERA_OSD_ZOOM_SLIDER, &slider_width, &slider_height);

				slider_x = g_camera_cntx.active_camera_osd_ptr->zoom.offset_x - (S32)(slider_width-bar_width)/2 ;

				if(current_idx == total_idx)
				{
					slider_y = g_camera_cntx.active_camera_osd_ptr->zoom.offset_y;
				}
				else if (current_idx == 0)
				{
					slider_y = g_camera_cntx.active_camera_osd_ptr->zoom.offset_y + bar_height - slider_height;
				}
				else
				{
					slider_y = g_camera_cntx.active_camera_osd_ptr->zoom.offset_y + (total_idx - current_idx) * (bar_height-slider_height)/total_idx;

				}	

				gdi_image_draw_id(slider_x, 
										slider_y,
										IMG_ID_CAMERA_OSD_ZOOM_SLIDER);


				
				if(g_camera_cntx.active_camera_osd_ptr->zoom_inc.is_show)
				{
					if(g_camera_cntx.touch_zoom_inc.is_press)
					{
						gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->zoom_inc.offset_x, 
												g_camera_cntx.active_camera_osd_ptr->zoom_inc.offset_y,
												(U16)(IMG_ID_CAMERA_OSD_ZOOM_INC_SEL));
					}
					else
					{
						gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->zoom_inc.offset_x, 
												g_camera_cntx.active_camera_osd_ptr->zoom_inc.offset_y,
												(U16)(IMG_ID_CAMERA_OSD_ZOOM_INC));
					}
				}
					
				if(g_camera_cntx.active_camera_osd_ptr->zoom_dec.is_show)
				{
					if(g_camera_cntx.touch_zoom_dec.is_press)
					{
						gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->zoom_dec.offset_x, 
												g_camera_cntx.active_camera_osd_ptr->zoom_dec.offset_y,
												(U16)(IMG_ID_CAMERA_OSD_ZOOM_DEC_SEL));
					}
					else
					{
						gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->zoom_dec.offset_x, 
												g_camera_cntx.active_camera_osd_ptr->zoom_dec.offset_y,
												(U16)(IMG_ID_CAMERA_OSD_ZOOM_DEC));
					}
				}
			}			


			  
                /*draw image size */
// Beginning--modified by kecx for fixing bug 12324 on 20090526
				if(g_camera_cntx.is_tileshot == TRUE)
				gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->image_size.offset_x,
										g_camera_cntx.active_camera_osd_ptr->image_size.offset_y,
										(U16)mmi_camera_get_img_size_img_id(LCD_WIDTH,LCD_HEIGHT));
				else
				gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->image_size.offset_x, 
										g_camera_cntx.active_camera_osd_ptr->image_size.offset_y, 
										(U16)mmi_camera_get_img_size_img_id(g_camera_cntx.image_width,g_camera_cntx.image_height));
// end--modified by kecx for fixing bug 12324 on 20090526
		#else
			//This Draw for ZOOM  //add by WeiD not QVGA SCREEN
			if(g_camera_cntx.active_camera_osd_ptr->zoom.is_show)
			{
				gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->zoom.offset_x, 
								g_camera_cntx.active_camera_osd_ptr->zoom.offset_y,
								(U16)(IMG_ID_CAMERA_OSD_ZOOM_START+g_camera_cntx.setting.zoom_idx+1));
			}
			if(g_camera_cntx.active_camera_osd_ptr->zoom_inc.is_show)
			{
				if(g_camera_cntx.touch_zoom_inc.is_press)
				{
					gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->zoom_inc.offset_x, 
											g_camera_cntx.active_camera_osd_ptr->zoom_inc.offset_y,
											(U16)(IMG_ID_CAMERA_OSD_ZOOM_INC_SEL));
				}
				else
				{
					gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->zoom_inc.offset_x, 
											g_camera_cntx.active_camera_osd_ptr->zoom_inc.offset_y,
											(U16)(IMG_ID_CAMERA_OSD_ZOOM_INC));
				}
			}
				
			if(g_camera_cntx.active_camera_osd_ptr->zoom_dec.is_show)
			{
				if(g_camera_cntx.touch_zoom_dec.is_press)
				{
					gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->zoom_dec.offset_x, 
											g_camera_cntx.active_camera_osd_ptr->zoom_dec.offset_y,
											(U16)(IMG_ID_CAMERA_OSD_ZOOM_DEC_SEL));
				}
				else
				{
					gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->zoom_dec.offset_x, 
											g_camera_cntx.active_camera_osd_ptr->zoom_dec.offset_y,
											(U16)(IMG_ID_CAMERA_OSD_ZOOM_DEC));
				}
			}

			#endif
			
		#endif /* __CAMERA_FEATURE_ZOOM__ */

		/* remain storage string */
		if(g_camera_cntx.active_camera_osd_ptr->remain_storage.is_show)
		{
			coolsand_UI_reset_text_clip();
			coolsand_UI_set_font(&MMI_small_font);
			mmi_trace(g_sw_CAMERA, "The remain_percent = %d",g_camera_cntx.remain_percent);
		
//			sprintf(str_buf, "%d%%", g_camera_cntx.remain_percent);
			sprintf(str_buf, "%d%c", g_camera_cntx.remain_percent,'%'); //modified by chenhe
			AnsiiToUnicodeString((PS8)str_buf_UCS2, (PS8)str_buf);
		
			coolsand_UI_measure_string((UI_string_type)str_buf_UCS2, &str_width, &str_height);

			if(g_camera_cntx.active_camera_osd_ptr->remain_storage.is_right_align)
			{
				mmi_camera_draw_style_text((PS8)str_buf_UCS2,
													g_camera_cntx.active_camera_osd_ptr->remain_storage.offset_x-str_width, 
													g_camera_cntx.active_camera_osd_ptr->remain_storage.offset_y,
													&g_camera_cntx.active_camera_osd_ptr->remain_storage.style_text);
			}
			else
			{
				mmi_camera_draw_style_text((PS8)str_buf_UCS2,
													g_camera_cntx.active_camera_osd_ptr->remain_storage.offset_x, 
													g_camera_cntx.active_camera_osd_ptr->remain_storage.offset_y,
													&g_camera_cntx.active_camera_osd_ptr->remain_storage.style_text);
			}
		}
	}

		/* capture icon */
		if(g_camera_cntx.active_camera_osd_ptr->capture.is_show)
		{
			if(g_camera_cntx.is_capturing)
			{
				gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->capture.offset_x,
										g_camera_cntx.active_camera_osd_ptr->capture.offset_y,
										(U16)(IMG_ID_CAMERA_OSD_CAPTURE_SEL));
			}
			else
			{
				gdi_image_draw_id(g_camera_cntx.active_camera_osd_ptr->capture.offset_x,
										g_camera_cntx.active_camera_osd_ptr->capture.offset_y,
										(U16)(IMG_ID_CAMERA_OSD_CAPTURE));
			}
		}

	#endif /* __CAMERA_FEATURE_ICONBAR_PANEL__ */


}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_draw_contshot_countbar()
* DESCRIPTION
*   draw contshot contbar
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_CONT_SHOT_DRAW_COUNTBAR__
static void mmi_camera_preview_draw_contshot_countbar(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16	img_id;
	S32	image_width;
	S32	image_height;
	S32	box_height;
	S32	offset_x;
	S32 	offset_y;
	S32	remain_shot;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	switch(g_camera_cntx.setting.cont_shot)
	{
		case CAMERA_SETTING_CONT_SHOT_1:
			img_id = IMG_ID_CAMERA_OSD_CONT_SHOT_BAR_1;
			break;

		case CAMERA_SETTING_CONT_SHOT_2:
			img_id = IMG_ID_CAMERA_OSD_CONT_SHOT_BAR_2;
			break;

		case CAMERA_SETTING_CONT_SHOT_3:
			img_id = IMG_ID_CAMERA_OSD_CONT_SHOT_BAR_3;
			break;

		default:
			img_id = 0;
	}

	gdi_image_get_dimension_id(img_id, &image_width, &image_height);
		
	box_height 	= image_height/g_camera_cntx.cont_shot_total_count;
	remain_shot = g_camera_cntx.cont_shot_total_count-g_camera_cntx.cont_shot_count;

	if(g_camera_osd_cntx.cont_count_bar.is_top_align)
	{
		/* top align */
		offset_x = g_camera_osd_cntx.cont_count_bar.offset_x;
		offset_y = g_camera_osd_cntx.cont_count_bar.offset_y;



		gdi_layer_push_clip();
		gdi_layer_set_clip(	offset_x,
									offset_y,
									offset_x+image_width-1,
									offset_y+box_height*remain_shot-1);
											
		gdi_image_draw_id(offset_x, offset_y, img_id);
		gdi_layer_pop_clip();
	}
	else
	{
		/* bottom align */
		offset_x = g_camera_osd_cntx.cont_count_bar.offset_x;
		offset_y = g_camera_osd_cntx.cont_count_bar.offset_y;
				
		gdi_layer_push_clip();
		gdi_layer_set_clip(	offset_x,
									offset_y-box_height*remain_shot+1,
									offset_x+image_width-1,
									offset_y);
								
		gdi_image_draw_id(offset_x, offset_y-image_height+1, img_id);
		gdi_layer_pop_clip();
	}

}
#endif /* __CAMERA_FEATURE_CONT_SHOT_DRAW_COUNTBAR__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_set_hint()
* DESCRIPTION
*   set hint string
* PARAMETERS
*	 hint_str1		IN		string 1
*	 hint_str2		IN 	string 2
*	 fade_time 		IN 	hint fade out time
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_set_hint(PS8 hint_str1, PS8 hint_str2, U16 fade_time)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* clear buffer */
	memset(g_camera_cntx.hint1_buf, 0, CAMERA_HINT_BUF_SIZE);
	memset(g_camera_cntx.hint2_buf, 0, CAMERA_HINT_BUF_SIZE);
	
	if(hint_str1 != NULL)
	{
		pfnUnicodeStrncpy((PS8)g_camera_cntx.hint1_buf, (PS8)hint_str1, CAMERA_HINT_CHAR_COUNT);
	}

	if(hint_str2 != NULL)
	{
		pfnUnicodeStrncpy((PS8)g_camera_cntx.hint2_buf, (PS8)hint_str2, CAMERA_HINT_CHAR_COUNT);
	}

	g_camera_cntx.fade_time = fade_time;

}


/*****************************************************************************
* FUNCTION
*	mmi_camera_check_sensor_orientation()
* DESCRIPTION
*   update orientation between sensor and lcm
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_OSD_HORIZONTAL__)
void 	mmi_camera_check_sensor_orientation(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	/* rotate base layer */
   if(g_camera_cntx.osd_rotate != gdi_layer_get_base_layer_rotation() ||g_camera_cntx.previous_osd_rotate == CAMERA_DISPLAY_ROTATE_RESET)
	{
		gdi_layer_push_and_set_active(g_camera_cntx.base_layer_handle);
		gdi_layer_set_rotate(g_camera_cntx.osd_rotate);
		if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
		{
			mmi_frm_set_self_rotation_flag(MMI_FRM_SCREEN_ROTATE_270);
			gdi_layer_resize(LCD_HEIGHT, LCD_WIDTH);
			g_camera_cntx.base_UI_device_height = LCD_WIDTH; 
			g_camera_cntx.base_UI_device_width  = LCD_HEIGHT;

		}
		else
		{
			mmi_frm_set_self_rotation_flag(MMI_FRM_SCREEN_ROTATE_0);
			gdi_layer_resize(LCD_WIDTH, LCD_HEIGHT);
			g_camera_cntx.base_UI_device_height = LCD_HEIGHT; 
			g_camera_cntx.base_UI_device_width  = LCD_WIDTH;

		}
		gdi_layer_clear_background(GDI_COLOR_BLACK);
		gdi_layer_pop_and_restore_active();
   }
}
#endif

/*****************************************************************************
* FUNCTION
*	mmi_camera_check_osd_orientation()
* DESCRIPTION
*   update display rotate (0 | 90 | 180 | 270 )
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_OSD_HORIZONTAL__) 
void 	mmi_camera_check_osd_orientation(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_camera_cntx.previous_osd_rotate != g_camera_cntx.osd_rotate)
	{
		/* rotate osd layer */
		gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);
		gdi_layer_toggle_double();
		gdi_layer_set_rotate(g_camera_cntx.osd_rotate);

		if(g_camera_cntx.osd_rotate==CAMERA_DISPLAY_ROTATE_90 || g_camera_cntx.osd_rotate ==CAMERA_DISPLAY_ROTATE_270)
		{
			gdi_layer_resize(LCD_HEIGHT, LCD_WIDTH);
			g_camera_cntx.osd_UI_device_height = LCD_WIDTH; 
			g_camera_cntx.osd_UI_device_width  = LCD_HEIGHT;
		}
		else 
		{
			gdi_layer_resize(LCD_WIDTH, LCD_HEIGHT);
			g_camera_cntx.osd_UI_device_height = LCD_HEIGHT; 
			g_camera_cntx.osd_UI_device_width  = LCD_WIDTH;

		}

		gdi_layer_clear_background(GDI_COLOR_TRANSPARENT);
		gdi_layer_pop_and_restore_active();

		switch(g_camera_cntx.osd_rotate)
		{
			case CAMERA_DISPLAY_ROTATE_0:
				g_camera_cntx.active_camera_osd_ptr =  &g_camera_osd_cntx;
			break;
		
			case CAMERA_DISPLAY_ROTATE_270:
				g_camera_cntx.active_camera_osd_ptr =  &g_camera_osd_cntx_270;
			break;
		
			default:
				g_camera_cntx.active_camera_osd_ptr =  &g_camera_osd_cntx;
			break;
	}

	mmi_camera_preview_set_key_hdlr();
		
	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__) && defined(__CAMERA_FEATURE_TOUCH_SCREEN__)
		mmi_camera_update_osd_touch_icon();
	#endif

	g_camera_cntx.previous_osd_rotate = g_camera_cntx.osd_rotate;

	}
}

#endif
/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_update_capture_size()
* DESCRIPTION
*   update capture size and preview size due to setting change
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_preview_update_capture_size(void)
{
	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_preview_update_capture_size()");
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
		
	#ifdef __CAMERA_FEATURE_IMAGE_SIZE_LL__
		if(g_camera_cntx.setting.image_size == CAMERA_SETTING_IMAGE_SIZE_LL)
		{
			g_camera_cntx.preview_width	= CAMERA_FEATURE_PREVIEW_SIZE_LL_WIDTH;
			g_camera_cntx.preview_height 	= CAMERA_FEATURE_PREVIEW_SIZE_LL_HEIGHT;
			
			g_camera_cntx.image_width		= CAMERA_FEATURE_IMAGE_SIZE_LL_WIDTH;
			g_camera_cntx.image_height 	= CAMERA_FEATURE_IMAGE_SIZE_LL_HEIGHT;

			g_camera_cntx.zoom_limit		= CAMERA_FEATURE_PREVIEW_LL_ZOOM_LIMIT;
		}
	#endif 

	#ifdef __CAMERA_FEATURE_IMAGE_SIZE_L__
		if(g_camera_cntx.setting.image_size == CAMERA_SETTING_IMAGE_SIZE_L)
		{
			g_camera_cntx.preview_width	= CAMERA_FEATURE_PREVIEW_SIZE_L_WIDTH;
			g_camera_cntx.preview_height 	= CAMERA_FEATURE_PREVIEW_SIZE_L_HEIGHT;

			g_camera_cntx.image_width		= CAMERA_FEATURE_IMAGE_SIZE_L_WIDTH;
			g_camera_cntx.image_height 	= CAMERA_FEATURE_IMAGE_SIZE_L_HEIGHT;

			g_camera_cntx.zoom_limit		= CAMERA_FEATURE_PREVIEW_L_ZOOM_LIMIT;		
		}
	#endif 

	#ifdef __CAMERA_FEATURE_IMAGE_SIZE_M__
		if(g_camera_cntx.setting.image_size == CAMERA_SETTING_IMAGE_SIZE_M)
		{
			g_camera_cntx.preview_width	= CAMERA_FEATURE_PREVIEW_SIZE_M_WIDTH;
			g_camera_cntx.preview_height 	= CAMERA_FEATURE_PREVIEW_SIZE_M_HEIGHT;

			g_camera_cntx.image_width		= CAMERA_FEATURE_IMAGE_SIZE_M_WIDTH;
			g_camera_cntx.image_height 	= CAMERA_FEATURE_IMAGE_SIZE_M_HEIGHT;

			g_camera_cntx.zoom_limit		= CAMERA_FEATURE_PREVIEW_M_ZOOM_LIMIT;
		}
	#endif 

    #ifdef __CAMERA_FEATURE_IMAGE_SIZE_S__
    #if defined(MEM_ULC_3216)
		if(g_camera_cntx.setting.image_size == CAMERA_SETTING_IMAGE_SIZE_S)
		{
			g_camera_cntx.preview_width	= CAMERA_FEATURE_PREVIEW_SIZE_S_WIDTH;
			g_camera_cntx.preview_height	= CAMERA_FEATURE_PREVIEW_SIZE_S_HEIGHT;
			
			g_camera_cntx.image_width		= CAMERA_FEATURE_IMAGE_SIZE_S_WIDTH;
			g_camera_cntx.image_height 	= CAMERA_FEATURE_IMAGE_SIZE_S_HEIGHT;

			g_camera_cntx.zoom_limit		= CAMERA_FEATURE_PREVIEW_S_ZOOM_LIMIT;	
		}
    #endif
    #endif 

	#ifdef __CAMERA_FEATURE_IMAGE_SIZE_SS__
		if(g_camera_cntx.setting.image_size == CAMERA_SETTING_IMAGE_SIZE_SS)
		{
			g_camera_cntx.preview_width	= CAMERA_FEATURE_PREVIEW_SIZE_SS_WIDTH;
			g_camera_cntx.preview_height 	= CAMERA_FEATURE_PREVIEW_SIZE_SS_HEIGHT;

			g_camera_cntx.image_width		= CAMERA_FEATURE_IMAGE_SIZE_SS_WIDTH;
			g_camera_cntx.image_height 	= CAMERA_FEATURE_IMAGE_SIZE_SS_HEIGHT;		

			g_camera_cntx.zoom_limit		= CAMERA_FEATURE_PREVIEW_SS_ZOOM_LIMIT;	
		}
	#endif
	mmi_trace(g_sw_CAMERA, "g_camera_cntx.setting.image_size = %d", g_camera_cntx.setting.image_size);
	mmi_trace(g_sw_CAMERA, "g_camera_cntx.preview_width      = %d", g_camera_cntx.preview_width);
	mmi_trace(g_sw_CAMERA, "g_camera_cntx.preview_height     = %d", g_camera_cntx.preview_height);
	mmi_trace(g_sw_CAMERA, "g_camera_cntx.image_width        = %d", g_camera_cntx.image_width);
	mmi_trace(g_sw_CAMERA, "g_camera_cntx.image_height       = %d", g_camera_cntx.image_height);

	if( ((g_camera_cntx.image_width == LCD_WIDTH) &&(g_camera_cntx.image_height == LCD_HEIGHT)))
	{
		g_camera_cntx.is_fullscreen = TRUE;

		#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
		if(g_camera_cntx.is_tileshot!=TRUE)
		{
			g_camera_cntx.previous_osd_rotate  = g_camera_cntx.osd_rotate;
			g_camera_cntx.osd_rotate = CAMERA_DISPLAY_ROTATE_0;
				g_camera_cntx.capture_path = CAPTURE_FROM_LAYER;
		}
		#else
				g_camera_cntx.capture_path = CAPTURE_FROM_LAYER;
		#endif
	}
	else
	{
	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
		if(g_camera_cntx.is_fullscreen == TRUE)
		{
			g_camera_cntx.previous_osd_rotate  = g_camera_cntx.osd_rotate;
			g_camera_cntx.osd_rotate = CAMERA_DISPLAY_ROTATE_270;
				g_camera_cntx.capture_path = CAPTURE_FROM_MEMORY;
		}
	#else
		g_camera_cntx.capture_path = CAPTURE_FROM_MEMORY;
	#endif
		g_camera_cntx.is_fullscreen = FALSE;
	}

	#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__  
		if(g_camera_cntx.setting.cont_shot == CAMERA_SETTING_CONT_SHOT_TILE)
		{
			g_camera_cntx.is_tileshot 		= TRUE;
			
			g_camera_cntx.image_width 		= (g_camera_cntx.osd_UI_device_width>>1);
			g_camera_cntx.image_height 	= (g_camera_cntx.osd_UI_device_height>>1);


		#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
			if(g_camera_cntx.osd_UI_device_width == LCD_WIDTH)
			{
				g_camera_cntx.capture_path = CAPTURE_FROM_LAYER;
				g_camera_cntx.is_fullscreen  	= TRUE;
			}
			else
			{
				g_camera_cntx.capture_path = CAPTURE_FROM_MEMORY;
				g_camera_cntx.is_fullscreen  	= FALSE;
			}
			g_camera_cntx.preview_width	= (LCD_HEIGHT>>1);
			g_camera_cntx.preview_height 	= (LCD_WIDTH>>1);
		#else
				g_camera_cntx.capture_path = CAPTURE_FROM_LAYER;
			/* tile shot also is a kind of fullscreen shot */			
				g_camera_cntx.is_fullscreen  	= TRUE;

			g_camera_cntx.preview_width	= (LCD_WIDTH>>1);
			g_camera_cntx.preview_height 	= (LCD_HEIGHT>>1);

		#endif
			/* set tileshot's zoom limit */
			g_camera_cntx.zoom_limit		= CAMERA_FEATURE_PREVIEW_TILE_ZOOM_LIMIT;
		}
		else
		{
			g_camera_cntx.is_tileshot = FALSE;
			#if !defined(__CAMERA_OSD_HORIZONTAL__) || !defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
			if(g_camera_cntx.is_fullscreen)
				g_camera_cntx.capture_path = CAPTURE_FROM_LAYER;		
			else 
				g_camera_cntx.capture_path = CAPTURE_FROM_MEMORY;
			#endif
		}
	#endif



}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_get_zoom_factor()
* DESCRIPTION
*   get zoom factor based on feature define
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static U16 mmi_camera_preview_get_zoom_factor(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_camera_cntx.setting.zoom_idx >= CAMERA_FEATURE_ZOOM_COUNT	)
	{
			mmi_camera_restore_setting();
			return 0;
	}
	else
	   return g_camera_feature_zoom[g_camera_cntx.setting.zoom_idx];
}


/*****************************************************************************
* FUNCTION
*	 mmi_camera_iconbar_right_arrow_press()
* DESCRIPTION
*   iconbar right arrow press
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
static void mmi_camera_iconbar_right_arrow_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_camera_cntx.iconbar_selected_idx++;
	if(g_camera_cntx.iconbar_selected_idx >= g_camera_cntx.iconbar_item_count)
	{
		g_camera_cntx.iconbar_selected_idx = 0;
	}

	g_camera_cntx.iconbar_start_idx++;
	if(g_camera_cntx.iconbar_start_idx >= g_camera_cntx.iconbar_item_count)
	{
		g_camera_cntx.iconbar_start_idx = 0;
	}

	mmi_camera_preview_draw_osd();
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
		
}
#endif /* __CAMERA_FEATURE_ICONBAR_PANEL__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_iconbar_right_arrow_release()
* DESCRIPTION
*   iconbar right arrow rlease
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
static void mmi_camera_iconbar_right_arrow_release(void)
{

}
#endif /* __CAMERA_FEATURE_ICONBAR_PANEL__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_iconbar_left_arrow_press()
* DESCRIPTION
*   iconbar left arrow press
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
static void mmi_camera_iconbar_left_arrow_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_camera_cntx.iconbar_selected_idx--;
	if(g_camera_cntx.iconbar_selected_idx < 0 )
	{
		g_camera_cntx.iconbar_selected_idx = g_camera_cntx.iconbar_item_count-1;
	}

	g_camera_cntx.iconbar_start_idx--;
	if(g_camera_cntx.iconbar_start_idx < 0)
	{
		g_camera_cntx.iconbar_start_idx = g_camera_cntx.iconbar_item_count-1;
	}

	mmi_camera_preview_draw_osd();
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
}
#endif /* __CAMERA_FEATURE_ICONBAR_PANEL__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_iconbar_left_arrow_releas()
* DESCRIPTION
*   iconbar left arrow release
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
static void mmi_camera_iconbar_left_arrow_releas(void)
{

}
#endif /* __CAMERA_FEATURE_ICONBAR_PANEL__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_iconbar_up_arrow_press()
* DESCRIPTION
*   iconbar up arrow press
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
static void mmi_camera_iconbar_up_arrow_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#define ICONBAR_SETTING_INC(__a__,__A__)\
	{\
		case CAMERA_ICONBAR_##__A__##:\
			mmi_camera_preview_##__a__##_change(TRUE);\
			break;\
	}
	
	switch(g_camera_cntx.iconbar_item[g_camera_cntx.iconbar_selected_idx])
	{
		case CAMERA_ICONBAR_ZOOM:
			#ifdef __CAMERA_FEATURE_ZOOM__
				mmi_camera_preview_zoom_in();
			#endif 
			break;

		case CAMERA_ICONBAR_EV:
			#ifdef __CAMERA_FEATURE_EV__
				mmi_camera_preview_ev_inc_key_press();
			#endif
			break;

		case CAMERA_ICONBAR_ADD_FRAME:
			#ifdef __CAMERA_FEATURE_ADD_FRAME__
				mmi_camera_preview_change_frame_up();
			#endif
			break;

		#ifdef __CAMERA_FEATURE_EFFECT__
			ICONBAR_SETTING_INC(effect,EFFECT);
		#endif 

		#ifdef __CAMERA_FEATURE_WB__		
			ICONBAR_SETTING_INC(wb,WB);
		#endif 
		
		#ifdef __CAMERA_FEATURE_FLASH__
			ICONBAR_SETTING_INC(flash,FLASH);
		#endif

		#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT__
			ICONBAR_SETTING_INC(led_highlight,LED_HIGHLIGHT);
		#endif 

		#ifdef __CAMERA_FEATURE_NIGHT__
			ICONBAR_SETTING_INC(night,NIGHT);
		#endif 

		#ifdef __CAMERA_FEATURE_CONT_SHOT__
			ICONBAR_SETTING_INC(cont_shot,CONT_SHOT);		
		#endif

		#ifdef __CAMERA_FEATURE_DELAY_TIMER__
			ICONBAR_SETTING_INC(delay_timer,DELAY_TIMER);		
		#endif
		
		ICONBAR_SETTING_INC(image_size,IMAGE_SIZE);
		ICONBAR_SETTING_INC(image_qty,IMAGE_QTY);
		
		#ifdef __CAMERA_FEATURE_STORAGE__
			ICONBAR_SETTING_INC(storage,STORAGE);
		#endif
	}	
	
}
#endif /* __CAMERA_FEATURE_ICONBAR_PANEL__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_iconbar_up_arrow_release()
* DESCRIPTION
*   icon bar up arrow release
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
static void mmi_camera_iconbar_up_arrow_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	switch(g_camera_cntx.iconbar_item[g_camera_cntx.iconbar_selected_idx])
	{
		case CAMERA_ICONBAR_ZOOM:
			coolsand_UI_cancel_timer(mmi_camera_preview_zoom_in);
			break;

		case CAMERA_ICONBAR_EV:
			mmi_camera_preview_ev_inc_key_release();
			break;
	}	
}
#endif /* __CAMERA_FEATURE_ICONBAR_PANEL__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_iconbar_down_arrow_press()
* DESCRIPTION
*   iconbar down arrow press
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
static void mmi_camera_iconbar_down_arrow_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#define ICONBAR_SETTING_DEC(__a__,__A__)\
	{\
		case CAMERA_ICONBAR_##__A__##:\
			mmi_camera_preview_##__a__##_change(FALSE);\
			break;\
	}
	
	switch(g_camera_cntx.iconbar_item[g_camera_cntx.iconbar_selected_idx])
	{
		case CAMERA_ICONBAR_ZOOM:
			#ifdef __CAMERA_FEATURE_ZOOM__
				mmi_camera_preview_zoom_out();
			#endif
			break;

		case CAMERA_ICONBAR_EV:
			#ifdef __CAMERA_FEATURE_EV__
				mmi_camera_preview_ev_dec_key_press();
			#endif
			break;

		case CAMERA_ICONBAR_ADD_FRAME:
			#ifdef __CAMERA_FEATURE_ADD_FRAME__
				mmi_camera_preview_change_frame_down();
			#endif
			break;


		#ifdef __CAMERA_FEATURE_EFFECT__
			ICONBAR_SETTING_DEC(effect,EFFECT);
		#endif 

		#ifdef __CAMERA_FEATURE_WB__		
			ICONBAR_SETTING_DEC(wb,WB);
		#endif 
		
		#ifdef __CAMERA_FEATURE_FLASH__
			ICONBAR_SETTING_DEC(flash,FLASH);
		#endif

		#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT__
			ICONBAR_SETTING_DEC(led_highlight,LED_HIGHLIGHT);
		#endif 

		#ifdef __CAMERA_FEATURE_NIGHT__
			ICONBAR_SETTING_DEC(night,NIGHT);
		#endif 

		#ifdef __CAMERA_FEATURE_CONT_SHOT__
			ICONBAR_SETTING_DEC(cont_shot,CONT_SHOT);		
		#endif

		#ifdef __CAMERA_FEATURE_DELAY_TIMER__
			ICONBAR_SETTING_DEC(delay_timer,DELAY_TIMER);		
		#endif
		
		ICONBAR_SETTING_DEC(image_size,IMAGE_SIZE);
		ICONBAR_SETTING_DEC(image_qty,IMAGE_QTY);
		
		#ifdef __CAMERA_FEATURE_STORAGE__
			ICONBAR_SETTING_DEC(storage,STORAGE);
		#endif
	}	
}
#endif /* __CAMERA_FEATURE_ICONBAR_PANEL__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_iconbar_down_arrow_release()
* DESCRIPTION
*   iconbar down arrow release
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
static void mmi_camera_iconbar_down_arrow_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	switch(g_camera_cntx.iconbar_item[g_camera_cntx.iconbar_selected_idx])
	{
		case CAMERA_ICONBAR_ZOOM:
			coolsand_UI_cancel_timer(mmi_camera_preview_zoom_out);
			break;

		case CAMERA_ICONBAR_EV:
			mmi_camera_preview_ev_dec_key_release();
			break;
	}	
}
#endif /* __CAMERA_FEATURE_ICONBAR_PANEL__ */


//add by WeiD

static void mmi_camera_preview_bt_inc_key_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
  	if(g_camera_cntx.app_state == CAMERA_STATE_PREVIEW)
	{
		if(g_camera_cntx.setting.reserve5< CAMERA_SETTING_BT_COUNT -1) //new add by WeiD
		{

//			g_camera_cntx.touch_ev_inc.is_press = TRUE;  //deleted by WeiD
			
			g_camera_cntx.setting.reserve5 ++;

			mdi_camera_update_para_bt(cool_camera_brightness_command_map[g_camera_cntx.setting.reserve5]);

			mmi_camera_preview_set_hint(NULL, NULL, 0);
			mmi_camera_preview_draw_osd();
			
			gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
		}
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_ev_inc_key_release()
* DESCRIPTION
*   preview right arrow release 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_bt_inc_key_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
  	if(g_camera_cntx.app_state == CAMERA_STATE_PREVIEW)
	{
//		g_camera_cntx.touch_ev_inc.is_press = FALSE;  //deleted by WeiD
		mmi_camera_preview_draw_osd();
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	}
}

#if 0
static void mmi_camera_preview_ct_inc_key_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
  	if(g_camera_cntx.app_state == CAMERA_STATE_PREVIEW)
	{
		if(g_camera_cntx.setting.reserve6< CAMERA_SETTING_CT_COUNT-1)
		{

//			g_camera_cntx.touch_ev_inc.is_press = TRUE;  //deleted by WeiD
			
			g_camera_cntx.setting.reserve6 ++;

			mdi_camera_update_para_ct(cool_camera_contrast_command_map[g_camera_cntx.setting.reserve6]);

			mmi_camera_preview_set_hint(NULL, NULL, 0);
			mmi_camera_preview_draw_osd();
			
			gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
		}
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_ev_inc_key_release()
* DESCRIPTION
*   preview right arrow release 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_ct_inc_key_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
  	if(g_camera_cntx.app_state == CAMERA_STATE_PREVIEW)
	{
//		g_camera_cntx.touch_ev_inc.is_press = FALSE;  //deleted by WeiD
		mmi_camera_preview_draw_osd();
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	}
}
#endif

static void mmi_camera_preview_bt_dec_key_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_camera_cntx.app_state == CAMERA_STATE_PREVIEW)
	{
		if(g_camera_cntx.setting.reserve5 > 0)
		{
//			g_camera_cntx.touch_ev_dec.is_press = TRUE;
			
			g_camera_cntx.setting.reserve5 --;
		
			mdi_camera_update_para_bt(cool_camera_brightness_command_map[g_camera_cntx.setting.reserve5]);

			mmi_camera_preview_set_hint(NULL, NULL, 0);
			mmi_camera_preview_draw_osd();
			gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
		}	
	}
}


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_ev_dec_key_release()
* DESCRIPTION
*   preview left arrow release
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_bt_dec_key_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_camera_cntx.app_state == CAMERA_STATE_PREVIEW)
	{
//		g_camera_cntx.touch_ev_dec.is_press = FALSE;
		mmi_camera_preview_draw_osd();
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	}
}
#if 0
static void mmi_camera_preview_ct_dec_key_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_camera_cntx.app_state == CAMERA_STATE_PREVIEW)
	{
		if(g_camera_cntx.setting.reserve6 > 0)
		{
//			g_camera_cntx.touch_ev_dec.is_press = TRUE;
			
			g_camera_cntx.setting.reserve6 --;
		
			mdi_camera_update_para_ct(cool_camera_contrast_command_map[g_camera_cntx.setting.reserve6]);

			mmi_camera_preview_set_hint(NULL, NULL, 0);
			mmi_camera_preview_draw_osd();
			gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
		}	
	}
}


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_ev_dec_key_release()
* DESCRIPTION
*   preview left arrow release
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_ct_dec_key_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_camera_cntx.app_state == CAMERA_STATE_PREVIEW)
	{
//		g_camera_cntx.touch_ev_dec.is_press = FALSE;
		mmi_camera_preview_draw_osd();
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	}
}


//add by WeiD
#endif

static void mmi_camera_preview_contrast_change(BOOL is_next)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8 	str_buf[256];
//   BOOL	is_custom_fx = FALSE;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   if(is_next)
   {
#if defined(__MMI_MAINLCD_220X176__)
		if(g_camera_cntx.setting.contrast < 4)  //modified by WeiD
			g_camera_cntx.setting.contrast++;
		else
			g_camera_cntx.setting.contrast = 0;
#else		
		if(g_camera_cntx.setting.reserve6 < 4)	//modified by WeiD
			g_camera_cntx.setting.reserve6++;
		else
			g_camera_cntx.setting.reserve6 = 0;
#endif
	}
	else
	{
#if 0	
		if(g_camera_cntx.setting.effect > 1)
			g_camera_cntx.setting.effect--;
		else
			g_camera_cntx.setting.effect = 5;  //modified by WeiD
#endif		
	}
#if defined(__MMI_MAINLCD_220X176__)
	mmi_trace(g_sw_CAMERA, "Excute funs: mmi_camera_preview_contrast_change() status = %d",cool_camera_contrast_command_map[g_camera_cntx.setting.contrast]);
	mdi_camera_update_para_ct(cool_camera_contrast_command_map[g_camera_cntx.setting.contrast]);
#else
	mmi_trace(g_sw_CAMERA, "Excute funs: mmi_camera_preview_contrast_change() status = %d",cool_camera_contrast_command_map[g_camera_cntx.setting.reserve6]);
	mdi_camera_update_para_ct(cool_camera_contrast_command_map[g_camera_cntx.setting.reserve6]);
#endif
//add by WeiD
	#ifndef __CAMERA_CONTRAST_HIDE_HINT__
		pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
		pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_CAMERA_CONTRAST));
		pfnUnicodeStrcat((PS8)str_buf, (PS8)L"]");
	
		mmi_camera_preview_set_hint(	(PS8)str_buf,
#if defined(__MMI_MAINLCD_220X176__)
												(PS8)GetString((U16)(g_camera_cntx.setting.contrast + STR_ID_CAMERA_CT_START)),
#else
												(PS8)GetString((U16)(g_camera_cntx.setting.reserve6 + STR_ID_CAMERA_CT_START)),
#endif
												HINT_POPUP_FADE_TIME);
	#endif /* __CAMERA_FEATURE_HIDE_HINT__ */
	
	mmi_camera_preview_draw_osd();
	
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

}


static void mmi_camera_preview_contrast_hotkey_press(void)
{
	mmi_trace(g_sw_CAMERA, "Excute funs: mmi_camera_preview_effect_hotkey_press() is TRUE");
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   mmi_camera_preview_contrast_change(TRUE);
}


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_ev_inc_key_press()
* DESCRIPTION
*   preview right arrow pressed, adjust EV 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__

static void mmi_camera_preview_ev_inc_key_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_stop_app_watchdog();
#endif

  	if(g_camera_cntx.app_state == CAMERA_STATE_PREVIEW)
	{
		if(g_camera_cntx.setting.ev < CAMERA_SETTING_EV_COUNT-1)
		{
			g_camera_cntx.touch_ev_inc.is_press = TRUE;
			
			g_camera_cntx.setting.ev++;

			mdi_camera_update_para_ev(camera_ev_command_map[g_camera_cntx.setting.ev]);

			mmi_camera_preview_set_hint(NULL, NULL, 0);
			mmi_camera_preview_draw_osd();
			
			gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
		}
	}
}
#endif


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_ev_inc_key_release()
* DESCRIPTION
*   preview right arrow release 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__

static void mmi_camera_preview_ev_inc_key_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

  	if(g_camera_cntx.app_state == CAMERA_STATE_PREVIEW)
	{
		g_camera_cntx.touch_ev_inc.is_press = FALSE;
		mmi_camera_preview_draw_osd();
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	}
}
#endif


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_ev_dec_key_press()
* DESCRIPTION
*   preview left arrow pressed, adjust EV 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__


static void mmi_camera_preview_ev_dec_key_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_stop_app_watchdog();
#endif

	if(g_camera_cntx.app_state == CAMERA_STATE_PREVIEW)
	{
		if(g_camera_cntx.setting.ev > 0)
		{
			g_camera_cntx.touch_ev_dec.is_press = TRUE;
			
			g_camera_cntx.setting.ev--;
		
			mdi_camera_update_para_ev(camera_ev_command_map[g_camera_cntx.setting.ev]);

			mmi_camera_preview_set_hint(NULL, NULL, 0);
			mmi_camera_preview_draw_osd();
			gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
		}	
	}
}
#endif

/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_ev_dec_key_release()
* DESCRIPTION
*   preview left arrow release
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__

static void mmi_camera_preview_ev_dec_key_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

	if(g_camera_cntx.app_state == CAMERA_STATE_PREVIEW)
	{
		g_camera_cntx.touch_ev_dec.is_press = FALSE;
		mmi_camera_preview_draw_osd();
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	}
}
#endif


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_zoom_in()
* DESCRIPTION
*   zoom out
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_ZOOM__
static void mmi_camera_preview_zoom_in(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 zoom_factor;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_camera_cntx.app_state == CAMERA_STATE_PREVIEW)
	{
		if(g_camera_cntx.setting.zoom_idx < CAMERA_FEATURE_ZOOM_COUNT-1)
		{
			if(g_camera_cntx.zoom_limit > mmi_camera_preview_get_zoom_factor())
			{
				g_camera_cntx.setting.zoom_idx += 1;

				g_camera_cntx.touch_zoom_inc.is_press = TRUE;

				/* get zoom factor based on features define */
				zoom_factor = mmi_camera_preview_get_zoom_factor();	
				
				mdi_camera_update_para_zoom(zoom_factor);
				
				mmi_camera_preview_set_hint(NULL, NULL, 0);
				mmi_camera_preview_draw_osd();
				gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

				#if defined(ISP_SUPPORT)
					/* only build-in camera use long pressed zooming feature */
					/* since backend module's zooming time is not stable, will cause zooming jump two level */
					if(g_camera_cntx.is_up_arrow_pressed)
					{
						coolsand_UI_start_timer(100, mmi_camera_preview_zoom_in);
					}
					else
					{
						/* first time press up arrow, use longer time */
						coolsand_UI_start_timer(300, mmi_camera_preview_zoom_in);
					}
				#endif /* ISP_SUPPORT */
				
			}
		}
		else
		{
			g_camera_cntx.touch_zoom_inc.is_press = FALSE;
			mmi_camera_preview_draw_osd();
			gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
		}
	}

}
#endif /* __CAMERA_FEATURE_ZOOM__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_zoom_out()
* DESCRIPTION
*   zoom out
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_ZOOM__
static void mmi_camera_preview_zoom_out(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 zoom_factor;
	mmi_trace(g_sw_CAMERA, "Excute funs: mmi_camera_preview_zoom_out() is TRUE");
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_camera_cntx.app_state == CAMERA_STATE_PREVIEW)
	{
		if(g_camera_cntx.setting.zoom_idx > 0)
		{
			g_camera_cntx.touch_zoom_dec.is_press = TRUE;
			
			g_camera_cntx.setting.zoom_idx -= 1;

			/* get zoom factor based on features define */

			zoom_factor = mmi_camera_preview_get_zoom_factor();
			
			mdi_camera_update_para_zoom(zoom_factor);

			mmi_camera_preview_set_hint(NULL, NULL, 0);
			mmi_camera_preview_draw_osd();
			gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

			#if defined(ISP_SUPPORT)
				/* only build-in camera use long pressed zooming feature */
				/* since backend module's zooming time is not stable, will cause zooming jump two level */
				if(g_camera_cntx.is_down_arrow_pressed)
				{
					coolsand_UI_start_timer(100, mmi_camera_preview_zoom_out);
				}
				else
				{
					/* first time press down arrow, use longer time */
					coolsand_UI_start_timer(300, mmi_camera_preview_zoom_out);
				}			
			#endif /* ISP_SUPPORT */
			
		}
		else
		{
			g_camera_cntx.touch_zoom_dec.is_press = FALSE;
			mmi_camera_preview_draw_osd();
			gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
		}
	}
}
#endif /* __CAMERA_FEATURE_ZOOM__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_up_arrow_press()
* DESCRIPTION
*   preview up arrow pressed, adjust zooming 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_ZOOM__) && !defined(__CAMERA_FEATURE_ICONBAR_PANEL__)
static void mmi_camera_preview_up_arrow_press(void)
{
	mmi_trace(g_sw_CAMERA, "Excute funs: mmi_camera_preview_up_arrow_press() is TRUE");
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_stop_app_watchdog();
#endif

	{
		if(!g_camera_cntx.is_up_arrow_pressed)
		{
			mmi_camera_preview_zoom_in();
		}

		g_camera_cntx.is_up_arrow_pressed = TRUE;
	}

}
#endif /* __CAMERA_FEATURE_ZOOM__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_up_arrow_release()
* DESCRIPTION
*   preview up arrow release, stop adjust zooming 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_ZOOM__) && !defined(__CAMERA_FEATURE_ICONBAR_PANEL__)
static void mmi_camera_preview_up_arrow_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

		g_camera_cntx.is_up_arrow_pressed = FALSE;

		g_camera_cntx.touch_zoom_inc.is_press = FALSE;
		mmi_camera_preview_draw_osd();
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

		coolsand_UI_cancel_timer(mmi_camera_preview_zoom_in);

}
#endif /* __CAMERA_FEATURE_ZOOM__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_down_arrow_press()
* DESCRIPTION
*   preview down arrow pressed, adjust zooming 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_ZOOM__) && !defined(__CAMERA_FEATURE_ICONBAR_PANEL__)
static void mmi_camera_preview_down_arrow_press(void)
{
	mmi_trace(g_sw_CAMERA, "Excute funs: mmi_camera_preview_down_arrow_press() is TRUE");
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_stop_app_watchdog();
#endif

		if(!g_camera_cntx.is_down_arrow_pressed)
		{
			mmi_camera_preview_zoom_out();
		}

		g_camera_cntx.is_down_arrow_pressed = TRUE;
}
#endif /* __CAMERA_FEATURE_ZOOM__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_down_arrow_release()
* DESCRIPTION
*   preview down arrow release, stop adjust zooming 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_ZOOM__) && !defined(__CAMERA_FEATURE_ICONBAR_PANEL__)
static void mmi_camera_preview_down_arrow_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

	{
		g_camera_cntx.is_down_arrow_pressed = FALSE;

		g_camera_cntx.touch_zoom_dec.is_press = FALSE;
		mmi_camera_preview_draw_osd();
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

		coolsand_UI_cancel_timer(mmi_camera_preview_zoom_out);
	}
}
#endif /* __CAMERA_FEATURE_ZOOM__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_effect_hotkey_press()
* DESCRIPTION
*   effect setting hotkey pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_EFFECT__) && !defined(__CAMERA_FEATURE_ICONBAR_PANEL__)
static void mmi_camera_preview_effect_hotkey_press(void)
{
	mmi_trace(g_sw_CAMERA, "Excute funs: mmi_camera_preview_effect_hotkey_press() is TRUE");
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

	mmi_camera_preview_effect_change(TRUE);
}
#endif /* __CAMERA_FEATURE_EFFECT__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_effect_change()
* DESCRIPTION
*   chage effect
* PARAMETERS
*	 is_next		IN		change effect to next, otherwise is previous
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_EFFECT__
static void mmi_camera_preview_effect_change(BOOL is_next)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8 	str_buf[256];
   BOOL	is_custom_fx = FALSE;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   if(is_next)
   {
              mmi_trace(1,"*******************CAMERA_SETTING_EFFECT_COUNT=%d***",CAMERA_SETTING_EFFECT_COUNT);
		if(g_camera_cntx.setting.effect < CAMERA_SETTING_EFFECT_COUNT-1)
		//if(g_camera_cntx.setting.effect < 6)  //modified by WeiD
			g_camera_cntx.setting.effect++;
		else
			g_camera_cntx.setting.effect = 0;
	}
	else
	{
		if(g_camera_cntx.setting.effect >= 1)
			g_camera_cntx.setting.effect--;
		else
			g_camera_cntx.setting.effect = CAMERA_SETTING_EFFECT_COUNT-1;
//			g_camera_cntx.setting.effect = 6;  //modified by WeiD
	}
			

	#ifndef __CAMERA_FEATURE_HIDE_HINT__
		pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
		pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_CAMERA_EFFECT_SETTING));
		pfnUnicodeStrcat((PS8)str_buf, (PS8)L"]");
	
		mmi_camera_preview_set_hint(	(PS8)str_buf,
												(PS8)GetString((U16)(g_camera_cntx.setting.effect + STR_ID_CAMERA_EFFECT_START+1)),
												HINT_POPUP_FADE_TIME);
	#endif /* __CAMERA_FEATURE_HIDE_HINT__ */
	
	mmi_camera_preview_draw_osd();

	#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
		if(g_camera_cntx.setting.effect == CAMERA_SETTING_EFFECT_CUSTOM_FX_1)
		{
			mdi_camera_update_para_effect(camera_effect_command_map[CAMERA_SETTING_EFFECT_NORMAL]);
			mdi_camera_update_para_brightness(g_camera_cntx.setting.fx_brightness[0]);
			mdi_camera_update_para_saturation(g_camera_cntx.setting.fx_saturation[0]);	
			mdi_camera_update_para_contrast(g_camera_cntx.setting.fx_contrast[0]);	
			mdi_camera_update_para_hue(g_camera_cntx.setting.fx_hue[0]);	
			
			is_custom_fx = TRUE;
		}
	#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */


	#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_2__	
		if(g_camera_cntx.setting.effect == CAMERA_SETTING_EFFECT_CUSTOM_FX_2)
		{
			mdi_camera_update_para_effect(camera_effect_command_map[CAMERA_SETTING_EFFECT_NORMAL]);
			mdi_camera_update_para_brightness(g_camera_cntx.setting.fx_brightness[1]);
			mdi_camera_update_para_saturation(g_camera_cntx.setting.fx_saturation[1]);	
			mdi_camera_update_para_contrast(g_camera_cntx.setting.fx_contrast[1]);	
			mdi_camera_update_para_hue(g_camera_cntx.setting.fx_hue[1]);	
			
			is_custom_fx = TRUE;
		}
	#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_2__ */


	#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_3__	
		if(g_camera_cntx.setting.effect == CAMERA_SETTING_EFFECT_CUSTOM_FX_3)
		{
			mdi_camera_update_para_effect(camera_effect_command_map[CAMERA_SETTING_EFFECT_NORMAL]);
			mdi_camera_update_para_brightness(g_camera_cntx.setting.fx_brightness[2]);
			mdi_camera_update_para_saturation(g_camera_cntx.setting.fx_saturation[2]);	
			mdi_camera_update_para_contrast(g_camera_cntx.setting.fx_contrast[2]);	
			mdi_camera_update_para_hue(g_camera_cntx.setting.fx_hue[2]);	

			is_custom_fx = TRUE;
		}
	#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_3__ */

	if(!is_custom_fx)
	{
		mdi_camera_update_para_effect(camera_effect_command_map[g_camera_cntx.setting.effect]);
		//mdi_camera_update_para_effect(g_camera_cntx.setting.effect);//chenhe modify
		#if 0 //chenhe delete
		mdi_camera_update_para_brightness(128);
		mdi_camera_update_para_saturation(128);	
		mdi_camera_update_para_contrast(128);	
		mdi_camera_update_para_hue(0);	
		#endif
	}
	
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

}
#endif /* __CAMERA_FEATURE_EFFECT__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_wb_hotkey_press()
* DESCRIPTION
*   wb hotkey pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_WB__) && !defined(__CAMERA_FEATURE_ICONBAR_PANEL__)
static void mmi_camera_preview_wb_hotkey_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
 #ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

	mmi_camera_preview_wb_change(TRUE);
}
#endif /* __CAMERA_FEATURE_WB__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_wb_change()
* DESCRIPTION
*   change wb
* PARAMETERS
*	 is_next		IN		change wb to next, otherwise is previous
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_WB__
static void mmi_camera_preview_wb_change(BOOL is_next)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8 str_buf[256];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   if(is_next)
   {
   	if(g_camera_cntx.setting.wb < CAMERA_SETTING_WB_COUNT-1)
			g_camera_cntx.setting.wb++;
		else
			g_camera_cntx.setting.wb = 0;
	}
	else
	{
		if(g_camera_cntx.setting.wb >= 1)
			g_camera_cntx.setting.wb--;
		else
			g_camera_cntx.setting.wb = CAMERA_SETTING_WB_COUNT-1;
	}
	
	#ifndef __CAMERA_FEATURE_HIDE_HINT__
		pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
		pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_CAMERA_WB));
		pfnUnicodeStrcat((PS8)str_buf, (PS8)L"]");		

		mmi_camera_preview_set_hint(	(PS8)str_buf, 
												(PS8)GetString((U16)(g_camera_cntx.setting.wb + STR_ID_CAMERA_WB_START+1)),
												HINT_POPUP_FADE_TIME);
	#endif /* __CAMERA_FEATURE_HIDE_HINT__ */

	mdi_camera_update_para_wb(camera_wb_command_map[g_camera_cntx.setting.wb]);
	
	mmi_camera_preview_draw_osd();

	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);	

}
#endif /* __CAMERA_FEATURE_WB__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_led_highlight_hotkey_press()
* DESCRIPTION
*   led highlight hotkey pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_LED_HIGHLIGHT__) && !defined(__CAMERA_FEATURE_ICONBAR_PANEL__)
static void mmi_camera_preview_led_highlight_hotkey_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

   mmi_camera_preview_led_highlight_change(TRUE);
}
#endif /* __CAMERA_FEATURE_LED_HIGHLIGHT__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_led_highlight_change()
* DESCRIPTION
*   led highlight change
* PARAMETERS
*	 is_next		IN		change led highlight to next, otherwise is previous
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT__
static void mmi_camera_preview_led_highlight_change(BOOL is_next)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8 str_buf[256];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   mmi_trace(1,"chenhe,mmi_camera_preview_led_highlight_change,g_camera_cntx.setting.led_highlight is %d",g_camera_cntx.setting.led_highlight);
   if(is_next)
   {
		if(g_camera_cntx.setting.led_highlight < CAMERA_SETTING_LED_HIGHLIGHT_COUNT-1)
			g_camera_cntx.setting.led_highlight++;
		else
			g_camera_cntx.setting.led_highlight = 0;
	}
	else
	{
		if(g_camera_cntx.setting.led_highlight >= 1)
			g_camera_cntx.setting.led_highlight--;
		else
			g_camera_cntx.setting.led_highlight = CAMERA_SETTING_LED_HIGHLIGHT_COUNT-1;
	}

	#ifndef __CAMERA_FEATURE_HIDE_HINT__	
		pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
		pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_CAMERA_LED_HIGHLIGHT));
		pfnUnicodeStrcat((PS8)str_buf, (PS8)L"]");
	
		mmi_camera_preview_set_hint(	(PS8)str_buf, 
												(PS8)GetString((U16)(g_camera_cntx.setting.led_highlight + STR_ID_CAMERA_LED_HIGHLIGHT_START+1)),
												HINT_POPUP_FADE_TIME);
	#endif /* __CAMERA_FEATURE_HIDE_HINT__ */
	
	mmi_camera_preview_draw_osd();

	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

	/* trun on/off led highlight */
	mmi_camera_turn_on_preview_led_highlight();

}
#endif /* __CAMERA_FEATURE_LED_HIGHLIGHT__ */




/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_flash_hotkey_press()
* DESCRIPTION
*   flash hotkey pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_FLASH__) && !defined(__CAMERA_FEATURE_ICONBAR_PANEL__)
static void mmi_camera_preview_flash_hotkey_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

   mmi_camera_preview_flash_change(TRUE);
}
#endif /* __CAMERA_FEATURE_FLASH__ */

#ifndef __MMI_DISABLE_DUAL_SENSOR__
 void mmi_camera_preview_switch_sensor_hotkey_press(void)
{
#ifdef MMI_ON_HARDWARE_P
#ifndef __MMI_DISABLE_DUAL_SENSOR__
    extern void camera_SwitchSensor();
    camera_SwitchSensor();
#endif
#endif

    EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
    GoBackHistory();
}
#endif


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_flash_change()
* DESCRIPTION
*   flash change
* PARAMETERS
*	 is_next		IN		change led highlight to next, otherwise is previous
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_FLASH__
static void mmi_camera_preview_flash_change(BOOL is_next)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/	
	S8 str_buf[256];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
   if(is_next)
   {
		if(g_camera_cntx.setting.flash < CAMERA_SETTING_FLASH_COUNT-1)
		{
			g_camera_cntx.setting.flash++;
		}
		else
		{
			g_camera_cntx.setting.flash = 0;
		}
	}
	else
	{
		if(g_camera_cntx.setting.flash >= 1)
		{
			g_camera_cntx.setting.flash--;
		}
		else
		{
			g_camera_cntx.setting.flash = CAMERA_SETTING_FLASH_COUNT-1;
		}
	}

	#ifndef __CAMERA_FEATURE_HIDE_HINT__
		pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
		pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_CAMERA_FLASH));
		pfnUnicodeStrcat((PS8)str_buf, (PS8)L"]");
	
		mmi_camera_preview_set_hint((PS8)str_buf, (PS8)GetString((U16)(g_camera_cntx.setting.flash + STR_ID_CAMERA_FLASH_START+1)),HINT_POPUP_FADE_TIME);
	#endif /* __CAMERA_FEATURE_HIDE_HINT__ */

	/* update quality parameter to mdi_camera */
	mdi_camera_update_para_flash(camera_flash_command_map[g_camera_cntx.setting.flash]);
	
	mmi_camera_preview_draw_osd();
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	
}
#endif /* __CAMERA_FEATURE_FLASH__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_night_hotkey_press()
* DESCRIPTION
*   night hotkey pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_NIGHT__) && !defined(__CAMERA_FEATURE_ICONBAR_PANEL__)
static void mmi_camera_preview_night_hotkey_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
 #ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

   mmi_camera_preview_night_change(TRUE);
}
#endif /* __CAMERA_FEATURE_NIGHT__ */




/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_night_change()
* DESCRIPTION
*   night change
* PARAMETERS
*	 is_next		IN		change night to next, otherwise is previous
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_NIGHT__
static void mmi_camera_preview_night_change(BOOL is_next)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8 str_buf[256];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   if(is_next)
   {
		if(g_camera_cntx.setting.night < CAMERA_SETTING_NIGHT_COUNT-1)
			g_camera_cntx.setting.night++;
		else
			g_camera_cntx.setting.night = 0;
	}
	else
	{
		if(g_camera_cntx.setting.night >= 1)
			g_camera_cntx.setting.night--;
		else
			g_camera_cntx.setting.night = CAMERA_SETTING_NIGHT_COUNT-1;
	}

	#ifndef __CAMERA_FEATURE_HIDE_HINT__
		pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
		pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_CAMERA_NIGHT));
		pfnUnicodeStrcat((PS8)str_buf, (PS8)L"]");
		
		mmi_camera_preview_set_hint(	(PS8)str_buf, 
												(PS8)GetString((U16)(g_camera_cntx.setting.night + STR_ID_CAMERA_NIGHT_START+1)),
												HINT_POPUP_FADE_TIME);
	#endif /* __CAMERA_FEATURE_HIDE_HINT__ */
	
	mmi_camera_preview_draw_osd();
	
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	
	mdi_camera_update_para_night(g_camera_cntx.setting.night);

}
#endif /* __CAMERA_FEATURE_NIGHT__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_image_size_hotkey_press()
* DESCRIPTION
*   image size hotkey pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/

/*WeiD Add Start For 6269 Ver: TBM780  on 2007-8-20 14:31 */
void mmi_camera_preview_wait_key7_hdlr()
{
	SetKeyHandler(mmi_camera_preview_image_size_hotkey_press, (U16)CAMERA_IMAGE_SIZE_KEY, KEY_EVENT_DOWN);	
}
/*WeiD Add End  For 6269 Ver: TBM780  on 2007-8-20 14:31 */

#if !defined(__CAMERA_FEATURE_ICONBAR_PANEL__)
static void mmi_camera_preview_image_size_hotkey_press(void)
{
	mmi_trace(g_sw_CAMERA, "Excute funs: mmi_camera_preview_image_size_hotkey_press() is TRUE");
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	

#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

   mmi_camera_preview_image_size_change(TRUE);
   /*WeiD Add Start For 6269 Ver: TBM780  on 2007-8-20 14:31 */
   currKeyFuncPtrs[KEY_7][KEY_EVENT_DOWN] = NULL;
    coolsand_UI_start_timer(300, mmi_camera_preview_wait_key7_hdlr);  
   /*WeiD Add End  For 6269 Ver: TBM780  on 2007-8-20 14:31 */
}
#endif

/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_image_size_change()
* DESCRIPTION
*   image size change
* PARAMETERS
*	 is_next		IN		change image size to next, otherwise is previous
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_image_size_change(BOOL is_next)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 			str_buf[256];
    S32 			prev_preview_width;
    S32 			prev_preview_height;
    BOOL			prev_is_fullscreen;
    BOOL			prev_is_tileshot;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/		
    /* change size */
    if(is_next)
    {
        if(g_camera_cntx.setting.image_size < CAMERA_SETTING_IMAGE_SIZE_COUNT-1)
            g_camera_cntx.setting.image_size++;
        else
            g_camera_cntx.setting.image_size = 0;
    }
    else
    {
        if(g_camera_cntx.setting.image_size >= 1)
            g_camera_cntx.setting.image_size--;
        else
            g_camera_cntx.setting.image_size = CAMERA_SETTING_IMAGE_SIZE_COUNT-1;
    }

    /* restore zoom setting */
    g_camera_cntx.setting.zoom_idx = CAMERA_DEFAULT_SETTING_ZOOM_IDX;

#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
    if(g_camera_cntx.is_tileshot)
    {
        g_camera_cntx.setting.cont_shot = CAMERA_SETTING_CONT_SHOT_OFF;
        g_camera_cntx.tile_cur_num 			= 0;
    }
#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */
	

#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
    /* disable burst shot if size can not support */
    if( (g_camera_cntx.setting.image_size != CAMERA_FEATURE_CONT_SHOT_BURST_IMAGE_IDX) &&
        (g_camera_cntx.setting.cont_shot == CAMERA_SETTING_CONT_SHOT_BURST) )
    {
        g_camera_cntx.setting.cont_shot = CAMERA_DEFAULT_SETTING_CONT_SHOT;
    }
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */
	
    /* backup previous */
    prev_preview_width 	= g_camera_cntx.preview_width;
    prev_preview_height	= g_camera_cntx.preview_height;
    prev_is_fullscreen	= g_camera_cntx.is_fullscreen;
    prev_is_tileshot  	= g_camera_cntx.is_tileshot;

#ifndef __CAMERA_FEATURE_HIDE_HINT__
    pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
    pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_CAMERA_IMAGE_SIZE));
    pfnUnicodeStrcat((PS8)str_buf, (PS8)L"]");		

    mmi_camera_preview_set_hint((PS8)str_buf,
            (PS8)GetString((U16)(g_camera_cntx.image_size_str_id[g_camera_cntx.setting.image_size])),
            HINT_POPUP_FADE_TIME);
#endif /* __CAMERA_FEATURE_HIDE_HINT__ */

    /* update parameters */	
    mmi_camera_preview_update_capture_size();
	
#if defined(__CAMERA_OSD_HORIZONTAL__) 
    mmi_camera_check_osd_orientation();
    #endif

#if 1
    if( (prev_preview_width != g_camera_cntx.preview_width) || 
         (prev_preview_height != g_camera_cntx.preview_height) ||
         (prev_is_fullscreen != g_camera_cntx.is_fullscreen) ||
         (g_camera_cntx.setting.zoom_idx != CAMERA_DEFAULT_SETTING_ZOOM_IDX) )
#else		 
    if(0)//chenhe for jasper
#endif	 
    {
        mmi_trace(g_sw_CAMERA, "CAM_Set setting.zoom_idx = %d",g_camera_cntx.setting.zoom_idx);
        /* since preview para is changed, stop and restart preview */
        mmi_camera_preview_stop();

        /* set blt */
        mmi_camera_preview_set_blt_layer(FALSE);

        /* update add frame */
#ifdef __CAMERA_FEATURE_ADD_FRAME__
        mmi_camera_draw_add_frame();
#endif

        /* clear background color */
        mmi_camera_preview_clear_background();
        mmi_camera_preview_draw_osd();
#if 1		
        gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
#endif		
#if defined(__CAMERA_OSD_HORIZONTAL__) 
        mmi_camera_check_sensor_orientation();
#endif
        mmi_camera_preview_free_preview_resource();
        {
            S32 offset_x = 0;
            S32 offset_y = 0;
#ifdef HORIZONTAL_CAMERA
            offset_y = (LCD_WIDTH - g_camera_cntx.preview_height)>>1;
#else
            offset_y = (LCD_HEIGHT - g_camera_cntx.preview_height)>>1;
#endif

            mmi_camera_preview_create_preview_resource(offset_x,offset_y, 
                    g_camera_cntx.preview_width,g_camera_cntx.preview_height,g_camera_cntx.lcm_rotate);
        }

        /* set blt layer for previwing */
        mmi_camera_preview_set_blt_layer(TRUE);
	
        mmi_camera_preview_start();

        /* check if preview success */
        if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
        {
            mmi_trace(g_sw_CAMERA,"mmi_camera_preview_image_size_change 1");
            mmi_camera_display_error_popup();
        }	
    }
    else
    {
        //mmi_trace(g_sw_CAMERA, "image_size_change image_width = %d,image_height = %d,addframe=%d",g_camera_cntx.image_width,g_camera_cntx.image_height,mmi_getcameraFrame());
        //MCI_CamSetPara(CAM_PARAM_ADD_FRAME,mmi_getcameraFrame());//add by gugogi
        
        mdi_camera_update_para_zoom(mmi_camera_preview_get_zoom_factor());
        mdi_camera_update_para_image_size(g_camera_cntx.image_width, g_camera_cntx.image_height);
        /* update add frame */
#ifdef __CAMERA_FEATURE_ADD_FRAME__
        mmi_camera_draw_add_frame();
#endif

        /* clear background color */
        mmi_camera_preview_clear_background();
		
        mmi_camera_preview_draw_osd();
        gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
    }
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_image_qty_hotkey_press()
* DESCRIPTION
*   imaeg qty hotkey pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if !defined(__CAMERA_FEATURE_ICONBAR_PANEL__)
static void mmi_camera_preview_image_qty_hotkey_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
 #ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif
   mmi_camera_preview_image_qty_change(TRUE);
}
#endif


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_image_qty_change()
* DESCRIPTION
*   image qty hotkey pressed
* PARAMETERS
*	 is_next		IN		change image qty to next, otherwise is previous
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_image_qty_change(BOOL is_next)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/	
	S8 str_buf[256];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
   if(is_next)
   {
		if(g_camera_cntx.setting.image_qty < CAMERA_SETTING_IMAGE_QTY_COUNT-1)
		{
			g_camera_cntx.setting.image_qty++;
		}
		else
		{
			g_camera_cntx.setting.image_qty = 0;
		}
	}
	else
	{
		if(g_camera_cntx.setting.image_qty >= 1)
		{
			g_camera_cntx.setting.image_qty--;
		}
		else
		{
			g_camera_cntx.setting.image_qty = CAMERA_SETTING_IMAGE_QTY_COUNT-1;
		}
	}

	#ifndef __CAMERA_FEATURE_HIDE_HINT__	
		pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
		pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_CAMERA_IMAGE_QTY));
		pfnUnicodeStrcat((PS8)str_buf, (PS8)L"]");
		
		mmi_camera_preview_set_hint((PS8)str_buf, (PS8)GetString((U16)(g_camera_cntx.setting.image_qty + STR_ID_CAMERA_IMAGE_QTY_START+1/*chenhe,for our only support normal or high*/)),HINT_POPUP_FADE_TIME);
	#endif /* __CAMERA_FEATURE_HIDE_HINT__ */

	/* update quality parameter to mdi_camera */
	//mdi_camera_update_para_image_qty(camera_image_qty_command_map[g_camera_cntx.setting.image_qty]);
	mdi_camera_update_para_image_qty(g_camera_cntx.setting.image_qty);//chenhe for jessamine
	mmi_trace(g_sw_CAMERA, "change_img_qty image_qty = %d",g_camera_cntx.setting.image_qty);

	mmi_camera_preview_draw_osd();
	
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
}


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_cont_shot_hotkey_press()
* DESCRIPTION
*   cont shot hotkey pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_CONT_SHOT__) && !defined(__CAMERA_FEATURE_ICONBAR_PANEL__)
static void mmi_camera_preview_cont_shot_hotkey_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
 #ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

   mmi_camera_preview_cont_shot_change(TRUE);
}
#endif /* __CAMERA_FEATURE_CONT_SHOT__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_cont_shot_change()
* DESCRIPTION
*   cont shot change
* PARAMETERS
*	 is_next		IN		change cont shot to next, otherwise is previous
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_CONT_SHOT__
static void mmi_camera_preview_cont_shot_change(BOOL is_next)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8 	str_buf[256];
	BOOL	need_restart_preview;
	
	#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
	   S32 	prev_preview_width;
  		S32 	prev_preview_height; 
		BOOL	need_restore_image_size;
		BOOL	need_change_preview;
	#endif

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	need_restart_preview = FALSE;
   
	#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
		/* inti var */
		need_restore_image_size = FALSE;
	
		/* leaving burst shot */
		if(g_camera_cntx.setting.cont_shot == CAMERA_SETTING_CONT_SHOT_BURST)
		{	
			need_restore_image_size = TRUE;
		}

	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */


	#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
		/* Before change is tile shot */
		if(g_camera_cntx.is_tileshot)
		{
			g_camera_cntx.tile_cur_num 			= 0;
			g_camera_cntx.setting.image_size = g_camera_cntx.previous_image_size;
			need_restart_preview = TRUE;
		}
	#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */


	if(is_next)
	{
		if(g_camera_cntx.setting.cont_shot < CAMERA_SETTING_CONT_SHOT_COUNT-1)
			g_camera_cntx.setting.cont_shot++;
		else
			g_camera_cntx.setting.cont_shot = 0;
	}
	else
	{
		if(g_camera_cntx.setting.cont_shot >= 1 )
			g_camera_cntx.setting.cont_shot--;
		else
			g_camera_cntx.setting.cont_shot = CAMERA_SETTING_CONT_SHOT_COUNT-1;
	}
			


	#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
		/* AFTER change is tile shot */
		if(g_camera_cntx.setting.cont_shot == CAMERA_SETTING_CONT_SHOT_TILE)
		{
			g_camera_cntx.is_tileshot 				= TRUE;
			g_camera_cntx.tile_cur_num 			= 0;
			g_camera_cntx.previous_image_size 	= g_camera_cntx.setting.image_size;
			g_camera_cntx.setting.image_size 	= CAMERA_FEATURE_CONT_SHOT_TILE_IMAGE_IDX;
			need_restart_preview 					= TRUE;
		}
		else
		{
			g_camera_cntx.is_tileshot 	= FALSE;
		}
			
	#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */

	#ifndef __CAMERA_FEATURE_HIDE_HINT__
		pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
		pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_CAMERA_CONT_SHOT));
		pfnUnicodeStrcat((PS8)str_buf, (PS8)L"]");

		mmi_camera_preview_set_hint(	(PS8)str_buf, 
												(PS8)GetString((U16)(g_camera_cntx.setting.cont_shot + STR_ID_CAMERA_CONT_SHOT_START+1)),
												HINT_POPUP_FADE_TIME);	
	#endif /* __CAMERA_FEATURE_HIDE_HINT__ */

	/* update capture size - for tile shot */
	mmi_camera_preview_update_capture_size();

	#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__

		/* check if change to burst need to change image and preview size */
		if(g_camera_cntx.setting.cont_shot == CAMERA_SETTING_CONT_SHOT_BURST)
		{
			/* store image size before enter burst shot */
			g_camera_cntx.image_size_before_burst = g_camera_cntx.setting.image_size;
		
			/* if not correct image size, change to it */
			if(g_camera_cntx.setting.image_size != CAMERA_FEATURE_CONT_SHOT_BURST_IMAGE_IDX)
			{
				g_camera_cntx.setting.image_size = CAMERA_FEATURE_CONT_SHOT_BURST_IMAGE_IDX;
				need_change_preview = TRUE;
			}
		}

		/* check change out from burst need to change image and preview size */
		if(need_restore_image_size)
		{
			if(g_camera_cntx.image_size_before_burst != CAMERA_FEATURE_CONT_SHOT_BURST_IMAGE_IDX )
			{
				/* restore preview */
				g_camera_cntx.setting.image_size = g_camera_cntx.image_size_before_burst;
				need_change_preview = TRUE;
			}
		}

		if(need_change_preview)
		{
			prev_preview_width 	= g_camera_cntx.preview_width;
			prev_preview_height	= g_camera_cntx.preview_height;

			mmi_camera_preview_update_capture_size();
			mdi_camera_update_para_image_size(g_camera_cntx.image_width, g_camera_cntx.image_height);

			if( (prev_preview_width != g_camera_cntx.preview_width) || 
				 (prev_preview_height != g_camera_cntx.preview_height) ||
				 (g_camera_cntx.setting.zoom_idx != CAMERA_DEFAULT_SETTING_ZOOM_IDX) )
			{
				g_camera_cntx.setting.zoom_idx = CAMERA_DEFAULT_SETTING_ZOOM_IDX;
				need_restart_preview = TRUE;
			}
		}		
		
	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */

	if(need_restart_preview)
	{
		/* since preview para is changed, stop and restart preview */
		mmi_camera_preview_stop();

		/* restore zoom setting */
		g_camera_cntx.setting.zoom_idx = CAMERA_DEFAULT_SETTING_ZOOM_IDX;

		/* update add frame */
		#ifdef __CAMERA_FEATURE_ADD_FRAME__
			mmi_camera_draw_add_frame();
		#endif
 
		/* set blt */
		mmi_camera_preview_set_blt_layer(FALSE);

		/* clear background color */
		mmi_camera_preview_clear_background();

		#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
			/* tile result back groud */
			#if defined(__CAMERA_OSD_HORIZONTAL__) 
			mmi_camera_check_tileshot_orientation();
			#else
				gdi_layer_push_and_set_active(g_camera_cntx.tileshot_result_layer_handle);
				gdi_layer_clear(GDI_COLOR_BLACK);
				gdi_layer_pop_and_restore_active();
			#endif 
		#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */


		mmi_camera_preview_set_hint(	(PS8)str_buf, 
												(PS8)GetString((U16)(g_camera_cntx.setting.cont_shot + STR_ID_CAMERA_CONT_SHOT_START+1)),
												HINT_POPUP_FADE_TIME);					
		mmi_camera_preview_draw_osd();

		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

		{
			S32 offset_x = 0;
			S32 offset_y = 0;

			mmi_camera_preview_free_preview_resource();
		#ifdef HORIZONTAL_CAMERA
			offset_y = (LCD_WIDTH - g_camera_cntx.preview_height)>>1;
		#else
			offset_y = (LCD_HEIGHT - g_camera_cntx.preview_height)>>1;
		#endif
			mmi_camera_preview_create_preview_resource(offset_x,offset_y, 
				g_camera_cntx.preview_width,g_camera_cntx.preview_height,g_camera_cntx.lcm_rotate);
		}


		/* set blt layer for previwing */
		mmi_camera_preview_set_blt_layer(TRUE);
		mmi_camera_preview_start();

		/* check if preview success */
		if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
		{
			mmi_trace(g_sw_CAMERA,"mmi_camera_preview_cont_shot_change 1");
			mmi_camera_display_error_popup();
			return;		
		}
	}
	else
	{
		mmi_camera_preview_set_hint(	(PS8)str_buf, 
												(PS8)GetString((U16)(g_camera_cntx.setting.cont_shot + STR_ID_CAMERA_CONT_SHOT_START+1)),
												HINT_POPUP_FADE_TIME);
		mmi_camera_preview_draw_osd();

		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	}
	
}
#endif /* __CAMERA_FEATURE_CONT_SHOT__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_delay_timer_hotkey_press()
* DESCRIPTION
*   delay_timer hotkey pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_DELAY_TIMER__) && !defined(__CAMERA_FEATURE_ICONBAR_PANEL__)
static void mmi_camera_preview_delay_timer_hotkey_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
 #ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

	mmi_camera_preview_delay_timer_change(TRUE);
}
#endif /* __CAMERA_FEATURE_DELAY_TIMER__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_delay_timer_change()
* DESCRIPTION
*   delay_timer change
* PARAMETERS
*	 is_next		IN		change delay timer to next, otherwise is previous
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_DELAY_TIMER__
static void mmi_camera_preview_delay_timer_change(BOOL is_next)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/	
	S8 str_buf[256];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
   if(is_next)
   {
		if(g_camera_cntx.setting.delay_timer < CAMERA_SETTING_DELAY_TIMER_COUNT-1)
			g_camera_cntx.setting.delay_timer++;
		else
			g_camera_cntx.setting.delay_timer = 0;
	}
	else
	{
		if(g_camera_cntx.setting.delay_timer >= 1)
			g_camera_cntx.setting.delay_timer--;
		else
			g_camera_cntx.setting.delay_timer = CAMERA_SETTING_DELAY_TIMER_COUNT-1;
	}

	#ifndef __CAMERA_FEATURE_HIDE_HINT__
		pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
		pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_CAMERA_DELAY_TIMER));
		pfnUnicodeStrcat((PS8)str_buf, (PS8)L"]");
	
		mmi_camera_preview_set_hint((PS8)str_buf, (PS8)GetString((U16)(g_camera_cntx.setting.delay_timer + STR_ID_CAMERA_DELAY_TIMER_START+1)),HINT_POPUP_FADE_TIME);
	#endif /* __CAMERA_FEATURE_HIDE_HINT__ */
		
	mmi_camera_preview_draw_osd();
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	
}
#endif /* __CAMERA_FEATURE_DELAY_TIMER__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_shot_mode_hotkey_press()
* DESCRIPTION
*   shot mode hotkey pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SHOT_MODE__
static void mmi_camera_preview_shot_mode_hotkey_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
   mmi_camera_preview_shot_mode_change(TRUE);
}
#endif /* __CAMERA_FEATURE_SHOT_MODE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_shot_mode_change()
* DESCRIPTION
*   shot mode hotkey pressed
* PARAMETERS
*	 is_next		IN		change shot mode to next, otherwise is previous
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SHOT_MODE__
static void mmi_camera_preview_shot_mode_change(BOOL is_next)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/	
	S8 str_buf[256];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
   if(is_next)
   {
		if(g_camera_cntx.setting.shot_mode < CAMERA_SETTING_SHOT_MODE_COUNT-1)
		{
			g_camera_cntx.setting.shot_mode++;
		}
		else
		{
			g_camera_cntx.setting.shot_mode = 0;
		}
	}
	else
	{
		if(g_camera_cntx.setting.shot_mode >= 1)
		{
			g_camera_cntx.setting.shot_mode--;
		}
		else
		{
			g_camera_cntx.setting.shot_mode = CAMERA_SETTING_SHOT_MODE_COUNT-1;
		}
	}
	
	pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
	pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_CAMERA_SHOT_MODE));
	pfnUnicodeStrcat((PS8)str_buf, (PS8)L"]");

	/* update quality parameter to mdi_camera */
	mdi_camera_update_para_shot_mode(camera_shot_mode_command_map[g_camera_cntx.setting.shot_mode]);
	
	mmi_camera_preview_set_hint((PS8)str_buf, (PS8)GetString((U16)(g_camera_cntx.setting.shot_mode + STR_ID_CAMERA_SHOT_MODE_START+1)),HINT_POPUP_FADE_TIME);
	mmi_camera_preview_draw_osd();
	
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
}
#endif /* __CAMERA_FEATURE_SHOT_MODE__ */




/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_iso_hotkey_press()
* DESCRIPTION
*   shot mode hotkey pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_ISO__
static void mmi_camera_preview_iso_hotkey_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

   mmi_camera_preview_iso_change(TRUE);
}
#endif /* __CAMERA_FEATURE_ISO__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_iso_change()
* DESCRIPTION
*   shot mode hotkey pressed
* PARAMETERS
*	 is_next		IN		change shot mode to next, otherwise is previous
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_ISO__
static void mmi_camera_preview_iso_change(BOOL is_next)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/	
	S8 str_buf[256];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
   if(is_next)
   {
		if(g_camera_cntx.setting.iso < CAMERA_SETTING_ISO_COUNT-1)
		{
			g_camera_cntx.setting.iso++;
		}
		else
		{
			g_camera_cntx.setting.iso = 0;
		}
	}
	else
	{
		if(g_camera_cntx.setting.iso >= 1)
		{
			g_camera_cntx.setting.iso--;
		}
		else
		{
			g_camera_cntx.setting.iso = CAMERA_SETTING_ISO_COUNT-1;
		}
	}
	
	pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
	pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_CAMERA_ISO));
	pfnUnicodeStrcat((PS8)str_buf, (PS8)L"]");

	/* update quality parameter to mdi_camera */
	mdi_camera_update_para_iso(camera_iso_command_map[g_camera_cntx.setting.iso]);
	
	mmi_camera_preview_set_hint((PS8)str_buf, (PS8)GetString((U16)(g_camera_cntx.setting.iso + STR_ID_CAMERA_ISO_START+1)),HINT_POPUP_FADE_TIME);
	mmi_camera_preview_draw_osd();
	
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
}
#endif /* __CAMERA_FEATURE_ISO__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_ae_meter_hotkey_press()
* DESCRIPTION
*   shot mode hotkey pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_AE_METER__
static void mmi_camera_preview_ae_meter_hotkey_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
 #ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

   mmi_camera_preview_ae_meter_change(TRUE);
}
#endif /* __CAMERA_FEATURE_AE_METER__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_ae_meter_change()
* DESCRIPTION
*   shot mode hotkey pressed
* PARAMETERS
*	 is_next		IN		change shot mode to next, otherwise is previous
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_AE_METER__
static void mmi_camera_preview_ae_meter_change(BOOL is_next)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/	
	S8 str_buf[256];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
   if(is_next)
   {
		if(g_camera_cntx.setting.ae_meter < CAMERA_SETTING_AE_METER_COUNT-1)
		{
			g_camera_cntx.setting.ae_meter++;
		}
		else
		{
			g_camera_cntx.setting.ae_meter = 0;
		}
	}
	else
	{
		if(g_camera_cntx.setting.ae_meter >= 1)
		{
			g_camera_cntx.setting.ae_meter--;
		}
		else
		{
			g_camera_cntx.setting.ae_meter = CAMERA_SETTING_AE_METER_COUNT-1;
		}
	}
	
	pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
	pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_CAMERA_AE_METER));
	pfnUnicodeStrcat((PS8)str_buf, (PS8)L"]");

	/* update quality parameter to mdi_camera */
	mdi_camera_update_para_ae_meter(camera_ae_meter_command_map[g_camera_cntx.setting.ae_meter]);
	
	mmi_camera_preview_set_hint((PS8)str_buf, (PS8)GetString((U16)(g_camera_cntx.setting.ae_meter + STR_ID_CAMERA_AE_METER_START+1)),HINT_POPUP_FADE_TIME);
	mmi_camera_preview_draw_osd();
	
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
}
#endif /* __CAMERA_FEATURE_AE_METER__ */





/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_storage_hotkey_press()
* DESCRIPTION
*   delay_timer hotkey pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_STORAGE__) && !defined(__CAMERA_FEATURE_ICONBAR_PANEL__)
static void mmi_camera_preview_storage_hotkey_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
 #ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

	mmi_camera_preview_storage_change(TRUE);
}
#endif /* __CAMERA_FEATURE_STORAGE__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_storage_change()
* DESCRIPTION
*   storage change
* PARAMETERS
*	 is_next		IN		change storage to next, otherwise is previous
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_STORAGE__
static void mmi_camera_preview_storage_change(BOOL is_next)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/	
	S8 		str_buf[256];
	U16		cur_storage;
	S32		create_result;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		   

	if(is_next)
	{
		if(g_camera_cntx.setting.storage < CAMERA_SETTING_STORAGE_COUNT-1)
			g_camera_cntx.setting.storage++;
		else
			g_camera_cntx.setting.storage = 0;
	}
	else
	{
		if(g_camera_cntx.setting.storage >= 1)
			g_camera_cntx.setting.storage--;
		else
			g_camera_cntx.setting.storage = CAMERA_SETTING_STORAGE_COUNT-1;

	}

  	#ifndef __CAMERA_FEATURE_HIDE_HINT__ 
		pfnUnicodeStrcpy((PS8)str_buf, (PS8)L"[ ");
		pfnUnicodeStrcat((PS8)str_buf, (PS8)GetString(STR_ID_CAMERA_STORAGE));
		pfnUnicodeStrcat((PS8)str_buf, (PS8)L"]");
	
		mmi_camera_preview_set_hint((PS8)str_buf, (PS8)GetString((U16)(g_camera_cntx.setting.storage + STR_ID_CAMERA_STORAGE_START+1)),HINT_POPUP_FADE_TIME);
	#endif /*  __CAMERA_FEATURE_HIDE_HINT__  */
		
	mmi_camera_preview_draw_osd();
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

	mmi_camera_set_storage_path_check(g_camera_cntx.setting.storage, FALSE);

	/* storage changed */
	if(g_camera_cntx.is_storage_path_changed)
	{
		/* check if path is valid or not. if not will pop up a screen after the preview screen is show */
		cur_storage = g_camera_cntx.setting.storage;
		
		if(mmi_camera_check_and_restore_valid_drv())
		{
			/* drv is valid, create directory */
			mmi_camera_get_storage_file_path(g_camera_cntx.storage_filepath);
			create_result = mmi_camera_create_file_dir(g_camera_cntx.storage_filepath);

			/* create directory failed */
			if(create_result == FS_WRITE_PROTECTION)
			{
				g_camera_cntx.last_error 					= CAMERA_ERR_WRITE_PROTECT;

				/* has error - restore to phone */
				g_camera_cntx.setting.storage				= CAMERA_SETTING_STORAGE_PHONE;	
				g_camera_cntx.is_storage_path_changed 	= TRUE;
			}
			else if(create_result == FS_DISK_FULL)
			{
				if(cur_storage == CAMERA_SETTING_STORAGE_PHONE)
				{
					g_camera_cntx.last_error 					= CAMERA_ERR_PHONE_DISK_FULL;
				}
				else
				{
					/* storage is card */
					g_camera_cntx.last_error 					= CAMERA_ERR_CARD_DISK_FULL;

					/* has error - restore to phone */
					g_camera_cntx.setting.storage				= CAMERA_SETTING_STORAGE_PHONE;	
					g_camera_cntx.is_storage_path_changed 	= TRUE;
				}
			}
			else if(create_result == FS_ROOT_DIR_FULL)
			{
				if(cur_storage == CAMERA_SETTING_STORAGE_PHONE)
				{
					g_camera_cntx.last_error 					= CAMERA_ERR_ROOT_DIR_FULL;
				}
				else
				{
					/* storage is card */
					g_camera_cntx.last_error 					= CAMERA_ERR_ROOT_DIR_FULL;

					/* has error - restore to phone */
					g_camera_cntx.setting.storage				= CAMERA_SETTING_STORAGE_PHONE;	
					g_camera_cntx.is_storage_path_changed 	= TRUE;
				}
			}
			
		}
		else
		{
			/* drv no valid, means card is plugged out (or phone FAT corrupted) */
			
			/* if storage before restoration is already in phone, means FAT is corrupted */
			if(cur_storage == CAMERA_SETTING_STORAGE_PHONE) 
			{
				g_camera_cntx.last_error = CAMERA_ERR_CORRUPT_FAT;
			}
			else
			{
				g_camera_cntx.last_error = MDI_RES_CAMERA_ERR_NO_DISK;
			}
		}

		/* check if preview success */
		if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
			mmi_trace(g_sw_CAMERA,"mmi_camera_preview_storage_change 1");
			mmi_camera_display_error_popup();
		
	}

	
}
#endif /* __CAMERA_FEATURE_STORAGE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_draw_add_frame()
* DESCRIPTION
*   exit preview state
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_ADD_FRAME__
static void mmi_camera_draw_add_frame(void)
{
	U16 image_idx;
	
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	
	gdi_layer_push_and_set_active(g_camera_cntx.add_frame_layer_handle);

	//gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
	//gdi_layer_clear_background(GDI_COLOR_TRANSPARENT);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_BLUE);//2007-7-6 fengzb
	gdi_layer_clear_background(GDI_COLOR_BLACK);//2007-7-6 fengzb
	/* if frame is set and preview size is LCD size, draw frame */
	if(g_camera_cntx.setting.add_frame != CAMERA_SETTING_ADD_FRAME_OFF)
	{
		if(g_camera_cntx.is_fullscreen)
		{
			if(g_camera_cntx.is_tileshot)
			{
				image_idx = (U16)(IMG_ID_CAMERA_TILE_ADD_FRAME_START+g_camera_cntx.setting.add_frame);
			}
			else
			{
				image_idx = (U16)(IMG_ID_CAMERA_ADD_FRAME_START+g_camera_cntx.setting.add_frame);
			}
			
			gdi_image_draw_id(0, 0, (U16)(image_idx));
		}
	}	
	
	gdi_layer_pop_and_restore_active();		
}
#endif /* __CAMERA_FEATURE_ADD_FRAME__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_change_frame_up()
* DESCRIPTION
*   change add frame 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if 0   //chenqiang delete temp fix 12920
#ifdef __CAMERA_FEATURE_ADD_FRAME__
static void mmi_camera_preview_change_frame_up(void)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(g_camera_cntx.is_fullscreen)
	{
		if(g_camera_cntx.setting.add_frame >= CAMERA_SETTING_ADD_FRAME_COUNT-1)
		{
			g_camera_cntx.setting.add_frame = 0;
		}
		else
		{
			g_camera_cntx.setting.add_frame++;
		}


		if(g_camera_cntx.setting.add_frame == 0 && g_camera_cntx.is_tileshot==FALSE && g_camera_cntx.is_fullscreen == FALSE)
			g_camera_cntx.capture_path = CAPTURE_FROM_MEMORY;
		else 
			g_camera_cntx.capture_path = CAPTURE_FROM_LAYER;

		mmi_camera_preview_stop();

		mmi_camera_draw_add_frame();
		
		gdi_layer_push_and_set_active(g_camera_cntx.preview_layer_handle);
		gdi_layer_clear(GDI_COLOR_BLACK);
		gdi_layer_pop_and_restore_active();
		
		mmi_camera_preview_draw_osd();
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

		mmi_camera_preview_start();

		if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
			mmi_trace(g_sw_CAMERA,"mmi_camera_preview_change_frame_up 1");
			mmi_camera_display_error_popup();		
	
	}
}
#endif /*  __CAMERA_FEATURE_ADD_FRAME__ */





/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_change_frame_down()
* DESCRIPTION
*   change add frame 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_ADD_FRAME__
static void mmi_camera_preview_change_frame_down(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(g_camera_cntx.is_fullscreen)
	{
   	if(g_camera_cntx.setting.add_frame == 0)
		{
			g_camera_cntx.setting.add_frame = CAMERA_SETTING_ADD_FRAME_COUNT-1;
		}
		else 
		{
			g_camera_cntx.setting.add_frame--;
		}

		if(g_camera_cntx.setting.add_frame == 0 && g_camera_cntx.is_tileshot==FALSE && g_camera_cntx.is_fullscreen == FALSE)
			g_camera_cntx.capture_path = CAPTURE_FROM_MEMORY;
		else 
			g_camera_cntx.capture_path = CAPTURE_FROM_LAYER;

		mmi_camera_preview_stop();

		mmi_camera_draw_add_frame();
		gdi_layer_push_and_set_active(g_camera_cntx.preview_layer_handle);
		gdi_layer_clear(GDI_COLOR_BLACK);
		gdi_layer_pop_and_restore_active();
		
		mmi_camera_preview_draw_osd();
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

		mmi_camera_preview_start();

		if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
			mmi_trace(g_sw_CAMERA,"mmi_camera_preview_change_frame_down 1");
			mmi_camera_display_error_popup();		
	}
}
#endif /*  __CAMERA_FEATURE_ADD_FRAME__ */


#endif
/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_lsk_press()
* DESCRIPTION
*   previwe lsk pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_lsk_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_camera_cntx.is_lsk_pressed = TRUE;

	#ifdef __CAMERA_FEATURE_OSD_MENU__
		CAMERA_ENTER_STATE(CAMERA_STATE_OSD_MENU);
	#else
		mmi_camera_preview_draw_osd();
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	#endif /* __CAMERA_FEATURE_OSD_MENU__ */

}

 

/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_lsk_release()
* DESCRIPTION
*   previwe lsk released
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_lsk_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_camera_cntx.is_lsk_pressed = FALSE;

	#ifdef __CAMERA_FEATURE_OSD_MENU__
		/* do nothing */
	#else
		mmi_camera_preview_draw_osd();
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	
		mmi_camera_entry_option_menu_screen();
	#endif /* __CAMERA_FEATURE_OSD_MENU__ */
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_rsk_press()
* DESCRIPTION
*   previwe rsk pressed
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_preview_rsk_press(void)
{
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_camera_cntx.is_rsk_pressed = TRUE;
    mmi_trace(1,"chenhe,mmi_camera_preview_rsk_press");
    mmi_camera_preview_draw_osd();
    gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_preview_rsk_release()
* DESCRIPTION
*   previwe rsk released
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/

static void mmi_camera_preview_rsk_release(void)
{	
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
    S32 offset_x;
    S32 offset_y;
#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */

    mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_preview_rsk_release()");
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_camera_cntx.is_rsk_pressed = FALSE;
    set_multicaputure(0);
#if defined(__CAMERA_FEATURE_CONT_SHOT__)
    coolsand_UI_cancel_timer(mmi_camera_contshot_cyclic);
#endif

    mmi_camera_preview_draw_osd();

    if( (g_camera_cntx.app_state == CAMERA_STATE_PREVIEW) &&
         (g_camera_cntx.is_tileshot) &&
         (g_camera_cntx.tile_cur_num != 0) )
    {
#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
        mmi_camera_preview_stop();

        /* if is tile shot, and alreay captured tile image, clear it and re-capture */
        offset_x = (g_camera_cntx.tile_cur_num%2)*(g_camera_cntx.osd_UI_device_width>>1);
        offset_y = (g_camera_cntx.tile_cur_num/2)*(g_camera_cntx.osd_UI_device_height>>1);

        mmi_camera_preview_set_blt_layer(FALSE);
		
        gdi_layer_push_and_set_active(g_camera_cntx.tileshot_result_layer_handle);

        gdi_draw_solid_rect(	offset_x, offset_y, 
                offset_x+(g_camera_cntx.osd_UI_device_width>>1)-1, offset_y+(g_camera_cntx.osd_UI_device_height>>1)-1,
                GDI_COLOR_BLACK);

        gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
			
        gdi_layer_pop_and_restore_active();

        g_camera_cntx.tile_cur_num--;

        if(g_camera_cntx.tile_cur_num == 0)
        {
            /* redraw OSD, this will change RSK from Clear->Back when clear to tile's 1st image */
            mmi_camera_preview_draw_osd();
        }

        gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
        mmi_camera_preview_set_blt_layer(TRUE);
        mmi_camera_preview_start();

        if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
            mmi_trace(g_sw_CAMERA,"mmi_camera_preview_rsk_release 1");
        mmi_camera_display_error_popup();			

        return;
#else
        MMI_ASSERT(0);
#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */
		
    }
    else
    {
        gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
    }
    /* camera with only 23key, will use RSK to capture */
#ifdef __MMI_CAMERA_23KEY__
    mmi_camera_capture_button_press();
#else
    GoBackHistory();
#endif /* __MMI_CAMERA_23KEY__ */		
}

#if 0
static void mmi_camera_preview_ind_handler(void *data)
{
	mmi_trace(1,"chenhe,mmi_camera_preview_ind_handler");
	mmi_camera_preview_rsk_release();
//	DM_Reset();
}
#endif
#if defined(__CAMERA_FEATURE_DELAY_TIMER__)

/*****************************************************************************
*
*	Main LCD Countdown State
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_countdown_state()
* DESCRIPTION
*	 entry count down state 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
static void mmi_camera_entry_countdown_state(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	TBM_ENTRY(0x2AA3);
	/* register soft keys */	
	ClearAllKeyHandler();

	SetKeyHandler(mmi_camera_preview_lsk_press, KEY_LSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_camera_preview_lsk_release, KEY_LSK, KEY_EVENT_UP);		
	
	SetKeyHandler(mmi_camera_preview_rsk_press, KEY_RSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_camera_preview_rsk_release, KEY_RSK, KEY_EVENT_UP);		

	/* clearn center key - since if not clear, this key will mappign to LSK */
#ifndef __MMI_ANALOGTV__
	if(CAMERA_CAPTURE_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_KEY, KEY_EVENT_DOWN);	
#endif
	if(CAMERA_CAPTURE_EXTRA_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_EXTRA_KEY, KEY_EVENT_DOWN);

	/* init countdown para */
	switch(g_camera_cntx.setting.delay_timer)
	{
		case CAMERA_SETTING_DELAY_TIMER_1:
			{
				g_camera_cntx.capture_countdown = CAMERA_FEATURE_DELAY_TIMER_1_DURATION*1000;
			}
			break;

	
		case CAMERA_SETTING_DELAY_TIMER_2:
			{
				g_camera_cntx.capture_countdown = CAMERA_FEATURE_DELAY_TIMER_2_DURATION*1000;
			}
			break;
		
		case CAMERA_SETTING_DELAY_TIMER_3:
			{
				g_camera_cntx.capture_countdown = CAMERA_FEATURE_DELAY_TIMER_3_DURATION*1000;
			}
			break;
			
		default:
			/* restore default value */
			mmi_camera_restore_setting();
			break;
	}

	/* start count down */
	mmi_camera_countdown_cyclic();
	TBM_EXIT(0x2AA3);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_countdown_state()
* DESCRIPTION
*	 entry count down state 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
static void mmi_camera_exit_countdown_state(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* stop countdown timer */
	coolsand_UI_cancel_timer(mmi_camera_countdown_stop_LED);
	coolsand_UI_cancel_timer(mmi_camera_countdown_cyclic);
	
	/* free preview resource */
	if(g_camera_cntx.app_next_state == CAMERA_STATE_EXIT)
	{
		/* turn off led highlight */
		mmi_camera_turn_off_led_highlight();
	
		/* stop previewing */
		mmi_camera_preview_stop();

		/* flatten layer */
		mmi_camera_preview_flatten_layer(TRUE);
	}

}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_countdown_cyclic()
* DESCRIPTION
*   count down delay timer cyclic loop
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static UINT32 camera_lastCd_times = 0;//chenhe add for count down capture,20091014
static void mmi_camera_countdown_cyclic(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* countdonw duration and freq */
#if 0   
	if(g_camera_cntx.capture_countdown > 6000)
	{
		if(g_camera_cntx.setting.led_highlight == CAMERA_SETTING_LED_HIGHLIGHT_OFF)
		{
			SetGPIOLevelReq(GPIO_DEV_LED_STATUS_1, LED_LIGHT_LEVEL5);
		}
		
		coolsand_UI_start_timer(100, mmi_camera_countdown_stop_LED);
		coolsand_UI_start_timer(1000, mmi_camera_countdown_cyclic);
		g_camera_cntx.capture_countdown-=1000;
	#ifdef __MMI_SILENT_MEETING_PROFILE__
		if( !IsSilentModeActivated() && !IsMeetingModeActivated() )
/*Jinzh Del Start For 5754 Ver: TBM780  on 2007-8-8 16:55 */
			#if 0
			PlayAudioDviStream((PU8)camera_sound_countdown, CAMREA_SOUND_COUNTDOWN_LEN, DEVICE_AUDIO_PLAY_ONCE);
			#endif
/*Jinzh Del End  For 5754 Ver: TBM780  on 2007-8-8 16:55 */
			/*Jinzh Add Start For 5754 Ver: TBM780  on 2007-8-8 16:55 */
			AudioPlayReq(CAMERA_DAOJISHI,DEVICE_AUDIO_PLAY_ONCE);
			/*Jinzh Add End  For 5754 Ver: TBM780  on 2007-8-8 16:55 */
	#else
		if( !IsSilentModeActivated() )
/*Jinzh Del Start For 5754 Ver: TBM780  on 2007-8-8 16:55 */
			#if 0
			PlayAudioDviStream((PU8)camera_sound_countdown, CAMREA_SOUND_COUNTDOWN_LEN, DEVICE_AUDIO_PLAY_ONCE);
			#endif
/*Jinzh Del End  For 5754 Ver: TBM780  on 2007-8-8 16:55 */
			/*Jinzh Add Start For 5754 Ver: TBM780  on 2007-8-8 16:55 */
			AudioPlayReq(CAMERA_DAOJISHI,DEVICE_AUDIO_PLAY_ONCE);
			/*Jinzh Add End  For 5754 Ver: TBM780  on 2007-8-8 16:55 */
	#endif /* __MMI_SILENT_MEETING_PROFILE__ */
	}
#endif	
	/*Jinzh Add Start For 6225 Ver: TBM780  on 2007-8-14 10:9 */
#ifdef __FF_AUDIO_SET__
	cameraAppOnOff = TRUE;
#endif

	if(g_camera_cntx.capture_countdown > 0) //chenhe &&0
	{
	     camera_lastCd_times = 0;
		if(g_camera_cntx.setting.led_highlight == CAMERA_SETTING_LED_HIGHLIGHT_OFF)
		{
			SetGPIOLevelReq(GPIO_DEV_LED_STATUS_1, LED_LIGHT_LEVEL5);
		}
		mmi_trace(1,"mmi_camera_countdown_cyclic,g_camera_cntx.capture_countdown  is %d,camera_lastCd_times is %d",g_camera_cntx.capture_countdown,camera_lastCd_times );

		coolsand_UI_start_timer(100, mmi_camera_countdown_stop_LED);
		coolsand_UI_start_timer(1000, mmi_camera_countdown_cyclic);
		g_camera_cntx.capture_countdown-=1000;

#ifdef __MMI_SILENT_MEETING_PROFILE__
		if( !IsSilentModeActivated() && !IsMeetingModeActivated() )
			#if 0
			PlayAudioDviStream((PU8)camera_sound_countdown, CAMREA_SOUND_COUNTDOWN_LEN, DEVICE_AUDIO_PLAY_ONCE);
			#endif
#else
		if( !IsSilentModeActivated() )
		{
			AudioPlayReq(CAMERA_DAOJISHI,DEVICE_AUDIO_PLAY_ONCE);
		}
#endif

	}
	else if(g_camera_cntx.capture_countdown == 0)
	{
		if(g_camera_cntx.setting.led_highlight == CAMERA_SETTING_LED_HIGHLIGHT_OFF)
		{
			SetGPIOLevelReq(GPIO_DEV_LED_STATUS_1, LED_LIGHT_LEVEL5);
		}
		if(camera_lastCd_times++>=5)
		{
			g_camera_cntx.capture_countdown =-1;
		}
		mmi_trace(1,"mmi_camera_countdown_cyclic,g_camera_cntx.capture_countdown  is %d,camera_lastCd_times is %d",g_camera_cntx.capture_countdown,camera_lastCd_times );

		coolsand_UI_start_timer(100, mmi_camera_countdown_stop_LED);
		coolsand_UI_start_timer(300, mmi_camera_countdown_cyclic);

		#ifdef __MMI_SILENT_MEETING_PROFILE__
		if( !IsSilentModeActivated() && !IsMeetingModeActivated() )
		{
		}
		#else
		if( !IsSilentModeActivated() )
		{
			AudioPlayReq(CAMERA_DAOJISHI,DEVICE_AUDIO_PLAY_ONCE);
		}
			
	#endif /* __MMI_SILENT_MEETING_PROFILE__ */
	
	}
	else
	{
		mmi_trace(1,"else mmi_camera_countdown_cyclic %d,mmi_camera_capture,%d",g_camera_cntx.capture_countdown	,	camera_lastCd_times);
              mmi_camera_capture();
	
		return;
	}
}


/*****************************************************************************
* FUNCTION
*	 mmi_camera_countdown_stop_LED()
* DESCRIPTION
*	 stop count down LED
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
static void mmi_camera_countdown_stop_LED(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	if(g_camera_cntx.setting.led_highlight == CAMERA_SETTING_LED_HIGHLIGHT_OFF)
	{
		SetGPIOLevelReq(GPIO_DEV_LED_STATUS_1, LED_LIGHT_LEVEL0);	
	}
}


#endif



#if defined(__CAMERA_FEATURE_CONT_SHOT__)


/*****************************************************************************
*
*	Main LCD Cont Shot State
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_contshot_state()
* DESCRIPTION
*	 entry contshot state 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
static void mmi_camera_entry_contshot_state(void)
{
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TBM_ENTRY(0x2AA4);
    /* register soft keys */	
    ClearAllKeyHandler();

    SetKeyHandler(mmi_camera_preview_lsk_press, KEY_LSK, KEY_EVENT_DOWN);	
    SetKeyHandler(mmi_camera_preview_lsk_release, KEY_LSK, KEY_EVENT_UP);		
	
    SetKeyHandler(mmi_camera_preview_rsk_press, KEY_RSK, KEY_EVENT_DOWN);	
    SetKeyHandler(mmi_camera_preview_rsk_release, KEY_RSK, KEY_EVENT_UP);	

    /* clearn center key - since if not clear, this key will mappign to LSK */
#ifndef __MMI_ANALOGTV__    
    if(CAMERA_CAPTURE_KEY!=-1)
        SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_KEY, KEY_EVENT_DOWN);	
#endif
    if(CAMERA_CAPTURE_EXTRA_KEY!=-1)
        SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_EXTRA_KEY, KEY_EVENT_DOWN);

    /* init count down para */
    switch(g_camera_cntx.setting.cont_shot)
    {
        case CAMERA_SETTING_CONT_SHOT_1:
        {
            g_camera_cntx.cont_shot_total_count = CAMERA_FEATURE_CONT_SHOT_1_COUNT;
            g_camera_cntx.cont_shot_count			= 0;
        }
        break;
	
        case CAMERA_SETTING_CONT_SHOT_2:
        {
            g_camera_cntx.cont_shot_total_count = CAMERA_FEATURE_CONT_SHOT_2_COUNT;
            g_camera_cntx.cont_shot_count			= 0;
        }
        break;
		
        case CAMERA_SETTING_CONT_SHOT_3:
        {
            g_camera_cntx.cont_shot_total_count = CAMERA_FEATURE_CONT_SHOT_3_COUNT;
            g_camera_cntx.cont_shot_count			= 0;
        }
        break;
    }

    {
        mmi_camera_contshot_cyclic();
    }
	
    TBM_EXIT(0x2AA4);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_contshot_state()
* DESCRIPTION
*	 entry contshot state 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
static void mmi_camera_exit_contshot_state(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* cancel countdown timer */
//	coolsand_UI_cancel_timer(mmi_camera_contshot_cyclic);

	/* free preview resource */
	if(g_camera_cntx.app_next_state == CAMERA_STATE_EXIT)
	{
		/* turn off led highlight */
		mmi_camera_turn_off_led_highlight();

		/* stop previewing */
		mmi_camera_preview_stop();

		/* flatten layer */
		mmi_camera_preview_flatten_layer(TRUE);	
	}
	
}
#ifdef MMI_ON_HARDWARE_P
extern kal_int32 media_cam_save_photo(UINT32 fhd );
#endif
/*****************************************************************************
* FUNCTION
*	 mmi_camera_contshot_cyclic()
* DESCRIPTION
*   cont shot cyclic loop
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_contshot_cyclic(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8	buf_filename[FMGR_PATH_BUFFER_SIZE];
    U8 continue_capture = TRUE;
    INT32  file_handle;

#ifndef __CAMERA_FEATURE_CONT_SHOT_DRAW_COUNTBAR__
    S8		buffer[100];
    S8		UCS2_buffer[100];
#endif /* __CAMERA_FEATURE_CONT_SHOT_DRAW_COUNTBAR__ */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* set capturing flag */
    g_camera_cntx.is_capturing = TRUE;

    if (g_camera_cntx.cont_shot_count+1 == g_camera_cntx.cont_shot_total_count)
        continue_capture = FALSE;

    if(g_camera_cntx.cont_shot_total_count != 1)
    {
#ifndef __CAMERA_FEATURE_CONT_SHOT_DRAW_COUNTBAR__
        sprintf(buffer, "%d/%d",g_camera_cntx.cont_shot_count+1, g_camera_cntx.cont_shot_total_count);
        AnsiiToUnicodeString((PS8)UCS2_buffer, (PS8)buffer);
        pfnUnicodeStrcpy((PS8)buffer, (PS8)GetString(STR_ID_CAMERA_NOFIFY_CAPTURING));
        mmi_camera_preview_set_hint((PS8)buffer, UCS2_buffer, 0);
#else
        mmi_camera_preview_set_hint(NULL, NULL, 0);
#endif /* __CAMERA_FEATURE_CONT_SHOT_DRAW_COUNTBAR__ */

        set_multicaputure(1);
        mmi_camera_preview_draw_osd();
        gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);	
    }
    else
    {
        /* if only 1 image, draw "Capture.." string */
        set_multicaputure(1);
        mmi_camera_preview_set_hint((PS8)GetString(STR_ID_CAMERA_NOFIFY_CAPTURING), NULL, 0);
        mmi_camera_preview_draw_osd();
        gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);	
    }

    /* will capture then save to file */
    //mmi_camera_get_next_filename(buf_filename);
    mmi_camera_turn_on_capture_led_highlight();
    mmi_trace(g_sw_CAMERA,"mmi_camera_contshot_cyclic capture_path = %d",g_camera_cntx.capture_path);

    COS_Sleep(1000);
    if(g_camera_cntx.setting.add_frame != CAMERA_SETTING_ADD_FRAME_OFF&&g_camera_cntx.image_width==LCD_WIDTH&&g_camera_cntx.preview_width==LCD_WIDTH&& g_camera_cntx.preview_height==LCD_HEIGHT)
    {	
        gdi_layer_set_blt_layer(g_camera_cntx.preview_layer_handle,
                        g_camera_cntx.add_frame_layer_handle,
                        0,
                        0);
        gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);//2007-7-6 fengzb
    }     

    //modified by gugogi
    g_camera_cntx.last_error = mdi_camera_capture_to_file(buf_filename,continue_capture);

    mmi_camera_get_next_filename(buf_filename);
    file_handle = MMI_FS_Open(buf_filename, FS_CREATE_ALWAYS);
    mmi_trace(g_sw_CAMERA,"const save photo by gugogi  buf_filename=%s,filehandle=%d",buf_filename,file_handle);
#ifndef MMI_ON_WIN32
	 g_camera_cntx.last_error = media_cam_save_photo(file_handle);
#endif


    MMI_FS_Close(file_handle);
		
    /* start timer for next shot */
    //we have to wait for its confirm then start the timer,very in jasper
    //coolsand_UI_start_timer(2500, mmi_camera_contshot_cyclic);
	
    /* play capture sound after capture */
    mmi_camera_capture_play_sound();
		
    /* clearn capturing flag */
    g_camera_cntx.is_capturing = TRUE;

    /* after captured restore black background */
#ifdef CAMERA_MODULE_WITH_LCD
    if(!g_camera_cntx.is_fullscreen)
        mmi_camera_preview_clear_background();
#endif /* CAMERA_MODULE_WITH_LCD */	

    mmi_camera_turn_off_led_highlight();

    /* captured, previwing is stopped, change blt layer */
    mmi_camera_preview_set_blt_layer(FALSE);

    if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
    {
        coolsand_UI_cancel_timer(mmi_camera_contshot_cyclic);
        mmi_trace(g_sw_CAMERA,"***delete the 0 size file***");
        MMI_FS_Delete((const UINT8 *)buf_filename);
        mmi_trace(g_sw_CAMERA,"mmi_camera_contshot_cyclic 1");
        mmi_camera_display_error_popup();
        return;
    }

    g_camera_cntx.cont_shot_count++;

    mmi_trace(g_sw_CAMERA, "mmi_camera_contshot_cyclic shot_count = %d",g_camera_cntx.cont_shot_count);

    /* terminate condition */
    if(g_camera_cntx.cont_shot_count >= g_camera_cntx.cont_shot_total_count)
    {
#if defined(__CAMERA_OSD_HORIZONTAL__) 
#if defined(CAMERA_MODULE)
        /* (0) base(as prevew buffer) , (1) add frame  */
        gdi_layer_set_blt_layer(g_camera_cntx.base_layer_handle,
                            g_camera_cntx.preview_layer_handle,
                            g_camera_cntx.osd_layer_handle,
                            g_camera_cntx.add_frame_layer_handle);
#elif (defined(MT6226)||defined(MT6226M)||defined(MT6227)||defined(MT6228)||defined(MT6229))
        /* (0) Base-as preview buffer, (2) Add frame layer */
        gdi_layer_set_blt_layer(g_camera_cntx.preview_layer_handle,
                            g_camera_cntx.add_frame_layer_handle,
                            g_camera_cntx.osd_layer_handle,
                            0);
#else
        gdi_layer_set_blt_layer(g_camera_cntx.base_layer_handle,
                            g_camera_cntx.preview_layer_handle,
                            g_camera_cntx.osd_layer_handle,
                            g_camera_cntx.add_frame_layer_handle);
#endif	
#endif
           
        set_mmc_camera_preview();
		
        mmi_camera_preview_draw_osd();
        gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
		
        /* stop timer */
        coolsand_UI_cancel_timer(mmi_camera_contshot_cyclic);
        set_multicaputure(0);
        ClearAllKeyHandler();   //add by WeiD
        CAMERA_ENTER_STATE(CAMERA_STATE_PREVIEW);
	
        return;
    }
    else
    {
        /* change blt layer for previwing */
        set_mmc_camera_preview();
        mmi_camera_preview_set_blt_layer(TRUE);		
        mmi_camera_preview_start();
	
        if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
        {
            /* stop timer */
            coolsand_UI_cancel_timer(mmi_camera_contshot_cyclic);
            mmi_trace(g_sw_CAMERA,"mmi_camera_contshot_cyclic 2");
            mmi_camera_display_error_popup();	
            return;
        }

        //coolsand_UI_start_timer(500,mmi_camera_contshot_cyclic);
        coolsand_UI_start_timer(1500, mmi_camera_contshot_cyclic);
    }
}

#endif

/*****************************************************************************
*
*	Burstshot State
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_burstshot_capture_state()
* DESCRIPTION
*	 entry burstshot state 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
static void mmi_camera_entry_burstshot_capture_state(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	/* register softkeys */	
	ClearAllKeyHandler();

	SetKeyHandler(mmi_camera_preview_lsk_press, KEY_LSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_camera_preview_lsk_release, KEY_LSK, KEY_EVENT_UP);		
	
	SetKeyHandler(mmi_camera_preview_rsk_press, KEY_RSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_camera_preview_rsk_release, KEY_RSK, KEY_EVENT_UP);	

	/* clearn center key - since if not clear, this key will mapping to LSK */
#ifndef __MMI_ANALOGTV__    
	if(CAMERA_CAPTURE_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_KEY, KEY_EVENT_DOWN);	
#endif
	if(CAMERA_CAPTURE_EXTRA_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_EXTRA_KEY, KEY_EVENT_DOWN);

	mmi_camera_turn_on_capture_led_highlight();

	/* set capturing flag */
	g_camera_cntx.is_capturing = TRUE;
	
	mmi_camera_preview_set_hint((PS8)GetString(STR_ID_CAMERA_NOFIFY_BURST_CAPTURING), NULL, 0);
	mmi_camera_preview_draw_osd();
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);


	#if defined(__CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__)

		#ifdef __CAMERA_FEATURE_SHUTTER_SOUND_OFF__
		if(g_camera_cntx.setting.shutter_sound != CAMERA_SETTING_SHUTTER_SOUND_OFF)
		#endif
		{
			/* start multi_channel sound, for capture sound effect */

			if(g_camera_cntx.is_multi_sound_open == FALSE)
			{
				/* multi_sound still not opened, open them */
				
				/* set flag */
				g_camera_cntx.is_multi_sound_open = TRUE;

				/* open and play a dummy midi at backgroup - this is a must do to play multi-channel sound */
				g_camera_cntx.multi_sound_dummy_bg = 
					mdi_audio_mma_open_midi((void*)camera_sound_dummy_bg, CAMERA_SOUND_DUMMY_BG_LEN, 0, NULL);

				mdi_audio_mma_play_midi(g_camera_cntx.multi_sound_dummy_bg);

				g_camera_cntx.multi_sound_cap_idx = 0;

				switch(g_camera_cntx.setting.shutter_sound)
				{
					case CAMERA_SETTING_SHUTTER_SOUND_1:
					{
						g_camera_cntx.multi_sound_capture0 = 
							mdi_audio_mma_open_wav(	(void*)camera_sound_shutter_1,
															(U32)CAMERA_SOUND_SHUTTER_1_LEN,
						                           1,
				   	   	                     NULL);	

						g_camera_cntx.multi_sound_capture1 = 
							mdi_audio_mma_open_wav(	(void*)camera_sound_shutter_1,
															(U32)CAMERA_SOUND_SHUTTER_1_LEN,
						                           1,
				   	   	                     NULL);	

						g_camera_cntx.multi_sound_capture2 = 
							mdi_audio_mma_open_wav(	(void*)camera_sound_shutter_1,
															(U32)CAMERA_SOUND_SHUTTER_1_LEN,
						                           1,
				   	   	                     NULL);						
					}
					break;

					case CAMERA_SETTING_SHUTTER_SOUND_2:
					{
						g_camera_cntx.multi_sound_capture0 = 				
							mdi_audio_mma_open_wav(	(void*)camera_sound_shutter_2,
															(U32)CAMERA_SOUND_SHUTTER_2_LEN,
						                           1,
				   	   	                     NULL);	

						g_camera_cntx.multi_sound_capture1 = 				
							mdi_audio_mma_open_wav(	(void*)camera_sound_shutter_2,
															(U32)CAMERA_SOUND_SHUTTER_2_LEN,
						                           1,
				   	   	                     NULL);		

						g_camera_cntx.multi_sound_capture2 = 				
							mdi_audio_mma_open_wav(	(void*)camera_sound_shutter_2,
															(U32)CAMERA_SOUND_SHUTTER_2_LEN,
						                           1,
				   	   	                     NULL);						
					}
					break;

					case CAMERA_SETTING_SHUTTER_SOUND_3:
					{
						g_camera_cntx.multi_sound_capture0 = 				
							mdi_audio_mma_open_wav(	(void*)camera_sound_shutter_3,
															(U32)CAMERA_SOUND_SHUTTER_3_LEN,
						                           1,
				   	   	                     NULL);
						
						g_camera_cntx.multi_sound_capture1 = 				
							mdi_audio_mma_open_wav(	(void*)camera_sound_shutter_3,
															(U32)CAMERA_SOUND_SHUTTER_3_LEN,
						                           1,
				   	   	                     NULL);		

						g_camera_cntx.multi_sound_capture2 = 				
							mdi_audio_mma_open_wav(	(void*)camera_sound_shutter_3,
															(U32)CAMERA_SOUND_SHUTTER_3_LEN,
						                           1,
				   	   	                     NULL);							
					}
					break;
				}
			}
			else
			{
				/* multi_sound is alrady opened, stop the timer that will close it */
				coolsand_UI_cancel_timer(mmi_camera_burstshot_close_multi_sound);
			}
			
		}
	
		 
		/* call mdi to capture and save */
		g_camera_cntx.last_error
			= mdi_camera_non_block_burst_capture_to_memory(	CAMERA_FEATURE_CONT_SHOT_BURST_COUNT,
																			&g_camera_cntx.burst_jpg_data,
																			mmi_camera_burstshot_capture_result_hdlr);
		
	#else
		/* burst shot shall play before capture */
		mmi_camera_capture_play_sound(); 
	
		/* call mdi to capture and save */
		g_camera_cntx.last_error
			= mdi_camera_burst_capture_to_memory(	CAMERA_FEATURE_CONT_SHOT_BURST_COUNT,
																&g_camera_cntx.burst_jpg_data);
																				

		/* clear capturing flag */
		g_camera_cntx.is_capturing = FALSE;

		/* restore black background */
		#ifdef CAMERA_MODULE_WITH_LCD
			mmi_camera_preview_clear_background();
		#endif /* CAMERA_MODULE_WITH_LCD */
		
		/* will enter save confirm, close led highlight */
		mmi_camera_turn_off_led_highlight();
		
		/* captured, previewing is stopped, change blt layer */
		mmi_camera_preview_set_blt_layer(FALSE);
		
		if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
		{
			mmi_trace(g_sw_CAMERA,"mmi_camera_entry_burstshot_capture_state 1");
			mmi_camera_display_error_popup();
			return;
		}
		else
		{
			CAMERA_ENTER_STATE(CAMERA_STATE_BURSTSHOT_SAVE_CONFIRM);
		}
		
	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__ */
	
}
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_burstshot_capture_result_hdlr()
* DESCRIPTION
*	 no block burst shot capture result handler
* PARAMETERS
*	 ret			IN				burst shot result
*	 count		IN				how many image is captured, count start from 1
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_CONT_SHOT_BURST__) && defined(__CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__)
static void mmi_camera_burstshot_capture_result_hdlr(MDI_RESULT ret, U16 captured_count)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* capture count start from 1 */

	mmi_camera_capture_play_multi_channel_sound(); 
	
	if(ret >= 0)
	{
		/* no error happened */
		if(captured_count == CAMERA_FEATURE_CONT_SHOT_BURST_COUNT)
		{
			g_camera_cntx.is_capturing = FALSE;
			CAMERA_ENTER_STATE(CAMERA_STATE_BURSTSHOT_SAVE_CONFIRM);
			return;
		}
	}
	else
	{
		g_camera_cntx.is_capturing = FALSE;
		g_camera_cntx.last_error = ret;
		mmi_trace(g_sw_CAMERA,"mmi_camera_burstshot_capture_result_hdlr 1");
		mmi_camera_display_error_popup();
	}
}
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ & __CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__*/



/*****************************************************************************
* FUNCTION
*	 mmi_camera_burstshot_close_multi_sound()
* DESCRIPTION
*	 no block burst shot capture result handler
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_CONT_SHOT_BURST__) && defined(__CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__)
static void mmi_camera_burstshot_close_multi_sound(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* capture count start from 1 */
	if(g_camera_cntx.is_multi_sound_open)
	{
		mdi_audio_mma_stop_midi(g_camera_cntx.multi_sound_dummy_bg);
		mdi_audio_mma_close_midi(g_camera_cntx.multi_sound_dummy_bg);

		mdi_audio_mma_stop_wav(g_camera_cntx.multi_sound_capture0);
		mdi_audio_mma_close_wav(g_camera_cntx.multi_sound_capture0);

		mdi_audio_mma_stop_wav(g_camera_cntx.multi_sound_capture1);
		mdi_audio_mma_close_wav(g_camera_cntx.multi_sound_capture1);

		mdi_audio_mma_stop_wav(g_camera_cntx.multi_sound_capture2);
		mdi_audio_mma_close_wav(g_camera_cntx.multi_sound_capture2);
			
		g_camera_cntx.multi_sound_capture0 	= 0;
		g_camera_cntx.multi_sound_capture1 	= 0;			
		g_camera_cntx.multi_sound_capture2 	= 0;			
		g_camera_cntx.multi_sound_dummy_bg 	= 0;

		g_camera_cntx.is_multi_sound_open = FALSE;
	}
	
}
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ & __CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__*/




/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_burstshot_capture_state()
* DESCRIPTION
*	 entry burstshot state 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
static void mmi_camera_exit_burstshot_capture_state(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* hide hint on panel if it is not fade out */
	mmi_camera_preview_hide_hint();

	/* free preview resource */
	if(g_camera_cntx.app_next_state == CAMERA_STATE_EXIT)
	{
		/* turn off led highlight */
		mmi_camera_turn_off_led_highlight();

		/* mdi_camera_burst_shot_stop */
		/* need this function if burstshot is non-blocking */
		
		/* flatten layer */
		mmi_camera_preview_flatten_layer(TRUE);
	}

	/* stop capture and stop dummy bg midi for multi-channel sound */
	#if defined(__CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__)

		if(g_camera_cntx.is_capturing)
		{
			mdi_camera_stop_non_block_burst_capture();
		}

		/*
			close multi-channel sound after 500 ms, this is used to 
			avoid last sound be terminate when play to half 
		*/
		coolsand_UI_start_timer(500, mmi_camera_burstshot_close_multi_sound);
		
	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__ */

	
}
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */





/*****************************************************************************
*
*	BurstShot Save Confirm State
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_burstshot_save_confirm_state()
* DESCRIPTION
*	 enter save confirm state
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
static void mmi_camera_entry_burstshot_save_confirm_state(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	S32				offset_x;
	S32				offset_y;
	S32				resized_offset_x;
	S32				resized_offset_y;
	S32				resized_width;
	S32				resized_height;
	S32				image_region_width;
	S32				image_region_height;	
	S32				image_region_offset_x;
	S32				image_region_offset_y;	
	S32				i, j;
	S32				image_box_width;
	S32				image_box_height;
	S32				image_box_offset_x;
	S32				image_box_offset_y;
	S32				spacing;
	S32 				index;
	PU8				image_buf_ptr;
	U32				image_buf_size;
	S32				image_width = 0;
	S32				image_height = 0;

	#ifdef __MMI_TVOUT__
		GDI_HANDLE	tvout_layer;		
	#endif /* __MMI_TVOUT__ */

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	/* init var */
	g_camera_cntx.is_lsk_pressed = FALSE;
	g_camera_cntx.is_rsk_pressed = FALSE;	

	spacing = 2;
	
	/* register soft keys */	
	ClearAllKeyHandler();
	
	SetKeyHandler(mmi_camera_burstshot_save_confirm_lsk_press, KEY_LSK, KEY_EVENT_DOWN); 
	SetKeyHandler(mmi_camera_burstshot_save_confirm_lsk_release, KEY_LSK, KEY_EVENT_UP); 	
	
	SetKeyHandler(mmi_camera_burstshot_save_confirm_rsk_press, KEY_RSK, KEY_EVENT_DOWN); 
	SetKeyHandler(mmi_camera_burstshot_save_confirm_rsk_release, KEY_RSK, KEY_EVENT_UP); 

	/* clearn center key - since if not clear, this key will mappign to LSK */
#ifndef __MMI_ANALOGTV__    
	if(CAMERA_CAPTURE_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_KEY, KEY_EVENT_DOWN);	
#endif
	if(CAMERA_CAPTURE_EXTRA_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_EXTRA_KEY, KEY_EVENT_DOWN);

	/* clear base layer background */
	mmi_camera_preview_clear_background();

	gdi_layer_set_blt_layer(g_camera_cntx.base_layer_handle,
									g_camera_cntx.osd_layer_handle, 
									0,
									0);
	
	/* hide icons  */
	mmi_camera_preview_set_hint(NULL, NULL, 0);
	mmi_camera_preview_draw_osd();
	

	#ifndef WIN32
		gdi_image_jpeg_get_dimension(	(PU8)g_camera_cntx.burst_jpg_data.file_address[0], 
												g_camera_cntx.burst_jpg_data.file_size[0],
												&image_width, 
												&image_height);
	#endif /* WIN32 */
	
	/* set clip region as content region */
	gdi_layer_reset_clip();

	/* calc image region */
	image_region_width 		= LCD_WIDTH;
	image_region_height 		= LCD_HEIGHT;
	image_region_offset_x 	= 0;
	image_region_offset_y	= 0;
	
	if(g_camera_cntx.active_camera_osd_ptr->title.is_draw_mmi_title_bar)
	{
		image_region_height 		-= (MMI_title_height+MMI_title_y); 
		image_region_offset_y	+= (MMI_title_height+MMI_title_y);
	}

	if(g_camera_cntx.active_camera_osd_ptr->softkey.is_draw_mmi_softkey)
	{
		image_region_height -= (MMI_button_bar_height); 
	}

	/* TODO: check image_width & image_height no Zero */
	
	image_box_width 	= image_region_width-spacing*(CAMERA_FEATURE_CONT_SHOT_BURST_SAVE_COL*2);
	image_box_width 	/= CAMERA_FEATURE_CONT_SHOT_BURST_SAVE_COL;

	image_box_height 	= image_region_height-spacing*(CAMERA_FEATURE_CONT_SHOT_BURST_SAVE_ROW*2);
	image_box_height 	/= CAMERA_FEATURE_CONT_SHOT_BURST_SAVE_ROW;

	index = 0;
	for(i = 0; i < CAMERA_FEATURE_CONT_SHOT_BURST_SAVE_ROW; i++)
	{
		for(j = 0 ; j < CAMERA_FEATURE_CONT_SHOT_BURST_SAVE_COL; j++)
		{
			image_buf_ptr 	= (PU8)g_camera_cntx.burst_jpg_data.file_address[index];
			image_buf_size = (U32)g_camera_cntx.burst_jpg_data.file_size[index];
		
			image_box_offset_x = image_region_offset_x+(j*2+1)*spacing+j*image_box_width;
			image_box_offset_y = image_region_offset_y+(i*2+1)*spacing+i*image_box_height;
		
			if( (image_box_width >= image_width) && (image_box_height >= image_height))
			{
				/* align preview window at the center of box */
				offset_x = image_box_offset_x+((image_box_width-image_width)>>1);
				offset_y = image_box_offset_y+((image_box_height-image_height)>>1);
				
				#ifndef WIN32
					gdi_image_jpeg_draw(	offset_x, 
												offset_y, 
												image_buf_ptr, 
												image_buf_size);
				#endif /* WIN32 */

				gdi_draw_rect(	image_box_offset_x+offset_x-1, 
									image_box_offset_y+offset_y-1, 
									image_box_offset_x+offset_x+image_width, 
									image_box_offset_y+offset_y+image_height, 
									GDI_COLOR_WHITE);
			}
			else
			{
				gdi_image_util_fit_bbox(image_box_width, 
												image_box_height,
												image_width, image_height,
												&resized_offset_x, &resized_offset_y,
												&resized_width, &resized_height);
				#ifndef WIN32
					gdi_image_jpeg_draw_resized(	resized_offset_x+image_box_offset_x, 
															resized_offset_y+image_box_offset_y, 
															resized_width,
															resized_height,
															image_buf_ptr, 
															image_buf_size);
				#endif /* WIN32 */
				
				gdi_draw_rect(	image_box_offset_x+resized_offset_x-1,
									image_box_offset_y+resized_offset_y-1,
									image_box_offset_x+resized_offset_x+resized_width, 
									image_box_offset_y+resized_offset_y+resized_height, 
									GDI_COLOR_WHITE);

			}

			/* blt */
			gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
			
			/* next image */
			index++;	
		}
	}


	#ifdef __MMI_TVOUT__
		if(mdi_tvout_is_enable())
		{
			if(mmi_phnset_tvout_get_style() == SETTING_TVOUT_STYLE_FULLSCREEN)
			{
				/* if is fullscreen, we shall set to owner to camera here, to avoid GDI's update */
				mdi_tvout_set_owner(MDI_TV_OWNER_MMI);			

				/* start MMI owner draw mode */
				mdi_tvout_set_mode(	MDI_TV_MODE_OWNER_DRAW,
											UI_device_width,			/* layer_width */
											UI_device_height);		/* layer_height */

				tvout_layer = mdi_tvout_get_active_layer();

				gdi_layer_push_and_set_active(tvout_layer);
				gdi_layer_reset_clip();
				gdi_layer_flatten(g_camera_cntx.base_layer_handle, 0, 0, 0);
				
				gdi_layer_pop_and_restore_active();

				/* blt to TV */
				mdi_tvout_blt();

				/* if is fullscreen, we shall set to owner to camera here, to avoid GDI's update */
				mdi_tvout_set_owner(MDI_TV_OWNER_CAMERA);				

				/* start mode 2 - full screen preview update */
				mdi_tvout_set_mode(	MDI_TV_MODE_CAMERA_PREVIEW,
											0,						/* layer_width */
											0);					/* layer_height */

			}
		}
	#endif /* __MMI_TVOUT__ */


	/* draw save notify text */
	mmi_camera_preview_set_hint((PS8)GetString(STR_ID_CAMERA_NOTIFY_SAVE_CONFIRM), NULL, 0);
	mmi_camera_preview_draw_osd();

	/* blt */
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

}
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_burstshot_save_confirm_state()
* DESCRIPTION
*	 exit save confirm state
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
static void mmi_camera_exit_burstshot_save_confirm_state(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	
	/* clear panel */
	mmi_camera_preview_set_hint(NULL, NULL, 0);
	mmi_camera_preview_draw_osd();
	
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

	/* free preview resource */
	if(g_camera_cntx.app_next_state == CAMERA_STATE_EXIT)
	{
		/* flatten layer */
		mmi_camera_preview_flatten_layer(FALSE);
	}

}
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_burstshot_save_confirm_lsk_press()
* DESCRIPTION
*   draw save confirm softkey
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
static void mmi_camera_burstshot_save_confirm_lsk_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_camera_cntx.is_lsk_pressed = TRUE;
	mmi_camera_preview_draw_osd();

	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
}
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_save_confirm_lsk_release()
* DESCRIPTION
*   lsk release
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
static void mmi_camera_burstshot_save_confirm_lsk_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_camera_cntx.is_lsk_pressed = FALSE;
  	mmi_camera_preview_draw_osd();	 
	
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

	/* enter burstshot saving to save file */
	CAMERA_ENTER_STATE(CAMERA_STATE_BURSTSHOT_SAVING);
}
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_burstshot_save_confirm_rsk_press()
* DESCRIPTION
*   draw save confirm softkey
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
static void mmi_camera_burstshot_save_confirm_rsk_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_camera_cntx.is_rsk_pressed = TRUE;
	mmi_camera_preview_draw_osd();	
	
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
}
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_burstshot_save_confirm_rsk_release()
* DESCRIPTION
*   draw save confirm softkey
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
static void mmi_camera_burstshot_save_confirm_rsk_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_camera_cntx.is_rsk_pressed = FALSE;
	
   mmi_camera_preview_draw_osd();	 
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	
	CAMERA_ENTER_STATE(CAMERA_STATE_PREVIEW);
}
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */



/*****************************************************************************
*
*	Save Burstshot State
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_burstshot_saving_state()
* DESCRIPTION
*	 entry save burstshot state 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
static void mmi_camera_entry_burstshot_saving_state(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	/* register softkeys */	
	ClearAllKeyHandler();
	
	SetKeyHandler(GoBackHistory, KEY_LSK, KEY_EVENT_UP);		
	SetKeyHandler(mmi_camera_entry_option_menu_screen, KEY_RSK, KEY_EVENT_UP);	

	/* clearn center key - since if not clear, this key will mappign to LSK */
#ifndef __MMI_ANALOGTV__    
	if(CAMERA_CAPTURE_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_KEY, KEY_EVENT_DOWN);	
#endif
	if(CAMERA_CAPTURE_EXTRA_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_EXTRA_KEY, KEY_EVENT_DOWN);

	/* flatten to base */
	gdi_layer_flatten_previous_to_base();

	/* draw base layer only */
	gdi_layer_set_blt_layer(g_camera_cntx.base_layer_handle,0 ,0 ,0);
	#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
	force_no_small_screen();
	#endif
	ShowCategory65Screen((PU8)GetString(STR_GLOBAL_SAVING), IMG_GLOBAL_SAVE, NULL);
	g_camera_cntx.is_cat65_show = TRUE;	

	/* start save burst shot */
	g_camera_cntx.burst_save_index = 0;
	mmi_camera_burstshot_saving_cyclic();
}
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_burstshot_saving_state()
* DESCRIPTION
*	 exit save burstshot state
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
static void mmi_camera_exit_burstshot_saving_state(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	/* exit popup screen to free drawmanager's resource */
	if(g_camera_cntx.is_cat65_show)
	{
		g_camera_cntx.is_cat65_show = FALSE;
		ExitCategoryFunction();
	}

	coolsand_UI_cancel_timer(mmi_camera_burstshot_saving_cyclic);
	gdi_image_stop_animation_all();
	
	if(g_camera_cntx.app_next_state == CAMERA_STATE_EXIT)
	{
		/* flatten layer */
		mmi_camera_preview_flatten_layer(FALSE);
	}
}
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_burstshot_saving_cyclic()
* DESCRIPTION
*	 loop to save burstshot image
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
static void mmi_camera_burstshot_saving_cyclic(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	S8 			buf_filename[FMGR_PATH_BUFFER_SIZE];
	PU8			image_buf_ptr;	
	U32			image_buf_size;
	U32			written;
	FS_HANDLE 	file_handle;
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	
	/* this function has nvram access */
	mmi_camera_get_next_filename(buf_filename);

	/* start timer for next shot */
	coolsand_UI_start_timer(100, mmi_camera_burstshot_saving_cyclic);

	/* save burst shot image */

	image_buf_ptr 	= (PU8)g_camera_cntx.burst_jpg_data.file_address[g_camera_cntx.burst_save_index];
	image_buf_size = (U32)g_camera_cntx.burst_jpg_data.file_size[g_camera_cntx.burst_save_index];

	file_handle = MMI_FS_Open((U16*)buf_filename, FS_CREATE | FS_READ_WRITE);

	if(file_handle >= 0)
	{
		MMI_FS_Write(file_handle, (void*)image_buf_ptr, image_buf_size, (UINT*)&written);
		MMI_FS_Close(file_handle);

		/* cant write in whole file, must be disk full */
		if(image_buf_size != written)
		{
			/* delete the file that is not save complete */
			MMI_FS_Delete((U16*)buf_filename);
			
			g_camera_cntx.last_error = MDI_RES_CAMERA_ERR_DISK_FULL;

			/* will display a popup, go out application and re-enter */
			mmi_trace(g_sw_CAMERA,"mmi_camera_burstshot_saving_cyclic 1");
			mmi_camera_display_error_popup();
			return;
		}
	}
	else
	{
		if(file_handle == FS_DISK_FULL)
		{
			g_camera_cntx.last_error = MDI_RES_CAMERA_ERR_DISK_FULL;
		}
		else if(file_handle == FS_ROOT_DIR_FULL)
		{
			g_camera_cntx.last_error = CAMERA_ERR_ROOT_DIR_FULL;
		}
		else if(file_handle == FS_WRITE_PROTECTION)
		{
			g_camera_cntx.last_error = MDI_RES_CAMERA_ERR_WRITE_PROTECTION;
		}
		else
		{
			g_camera_cntx.last_error = MDI_RES_CAMERA_ERR_CAPTURE_FAILED;
		}

		/* will display a popup, go out application and re-enter */
		mmi_trace(g_sw_CAMERA,"mmi_camera_burstshot_saving_cyclic 2");
		mmi_camera_display_error_popup();
		return;
	}

	/* teminate condition */
	if(g_camera_cntx.burst_save_index >= CAMERA_FEATURE_CONT_SHOT_BURST_COUNT-1)
	{	
		CAMERA_ENTER_STATE(CAMERA_STATE_SAVE_DONE);
		return;
	}

	/* increase index */
	g_camera_cntx.burst_save_index++;
	
}
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */




/*****************************************************************************
*
*	TileShot State
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_tileshot_capture()
* DESCRIPTION
*	 tileshot capture
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
static void mmi_camera_tileshot_capture(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	S32 			offset_x;
	S32 			offset_y;

	#ifdef MMI_ON_HARDWARE_P
		GDI_RESULT	result;
	#endif /* MMI_ON_HARDWARE_P */


	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/	

	/* if needs to show capture icon, update it */
	if(g_camera_cntx.active_camera_osd_ptr->capture.is_show)
	{
		g_camera_cntx.is_capturing = TRUE;
		mmi_camera_preview_draw_osd();
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	}
		

	#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
		if(g_camera_cntx.osd_rotate ==CAMERA_DISPLAY_ROTATE_270)
		{
			offset_x = (g_camera_cntx.tile_cur_num%2)*(LCD_HEIGHT>>1);
			offset_y = (g_camera_cntx.tile_cur_num/2)*(LCD_WIDTH>>1);			
		}
		else if (g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_0)
		{
			offset_x = (g_camera_cntx.tile_cur_num%2)*(LCD_WIDTH>>1);
			offset_y = (g_camera_cntx.tile_cur_num/2)*(LCD_HEIGHT>>1);
		}
	#else
			offset_x = (g_camera_cntx.tile_cur_num%2)*(LCD_WIDTH>>1);
			offset_y = (g_camera_cntx.tile_cur_num/2)*(LCD_HEIGHT>>1);

	#endif
	
	mmi_camera_turn_on_capture_led_highlight();
	switch(g_camera_cntx.capture_path)
	{
		case 0:
		g_camera_cntx.last_error
			= mdi_camera_capture_to_memory(&g_camera_cntx.capture_buf_ptr, &g_camera_cntx.capture_size);
		break;
		case 1:
		gdi_layer_set_blt_layer(g_camera_cntx.preview_layer_handle,
														GDI_LAYER_MEDIA_HW_TRIGGER_LAYER,
														g_camera_cntx.tileshot_result_layer_handle,
														g_camera_cntx.add_frame_layer_handle);


		
				

			g_camera_cntx.last_error 
					= mdi_camera_capture_layer_to_memory(	GDI_LAYER_ENABLE_LAYER_0,
																		&g_camera_cntx.capture_buf_ptr,
																		&g_camera_cntx.capture_size,
																		offset_x,
																		offset_y,
																		g_camera_cntx.osd_UI_device_width>>1,
																		g_camera_cntx.osd_UI_device_height>>1);


		gdi_layer_set_blt_layer(g_camera_cntx.tileshot_result_layer_handle,
											GDI_LAYER_MEDIA_HW_TRIGGER_LAYER,
											g_camera_cntx.add_frame_layer_handle,
										g_camera_cntx.osd_layer_handle);
				
		break;
		default:
		g_camera_cntx.last_error
			= mdi_camera_capture_to_memory(&g_camera_cntx.capture_buf_ptr, &g_camera_cntx.capture_size);
		break;
	}

	/* play capture sound after capture */
	mmi_camera_capture_play_sound();
	mmi_camera_turn_off_led_highlight();

	gdi_layer_push_and_set_active(g_camera_cntx.tileshot_result_layer_handle);

	offset_x = (g_camera_cntx.tile_cur_num%2)*(g_camera_cntx.osd_UI_device_width>>1);
	offset_y = (g_camera_cntx.tile_cur_num/2)*(g_camera_cntx.osd_UI_device_height>>1);	

	#ifdef MMI_ON_HARDWARE_P
		/* TODO: add error handling */
		result = gdi_image_jpeg_draw(offset_x, offset_y, g_camera_cntx.capture_buf_ptr, g_camera_cntx.capture_size);
	#endif /* MMI_ON_HARDWARE_P */
		
	gdi_layer_pop_and_restore_active();

	/* set hint to NULL */
	mmi_camera_preview_set_hint(NULL, NULL, 0);

	/* if needs to show capture icon, update it */
	g_camera_cntx.is_capturing = FALSE;
	mmi_camera_preview_draw_osd();

	/* change blt layer for not previwing */
	mmi_camera_preview_set_blt_layer(FALSE);

	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

	g_camera_cntx.tile_cur_num++;
	
	if(g_camera_cntx.tile_cur_num == 1)
	{
		/* first tile shot, change softkey from BACK->Clear */
		mmi_camera_preview_draw_osd();
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	}

	if(g_camera_cntx.tile_cur_num < 4)
	{
		/* change blt layer for previwing */
		mmi_camera_preview_set_blt_layer(TRUE);

		if(g_camera_cntx.app_state == CAMERA_STATE_PREVIEW)
		{
			mmi_camera_preview_start();
			
			if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
				mmi_trace(g_sw_CAMERA,"mmi_camera_tileshot_capture 1");
				mmi_camera_display_error_popup();
		}
		else if(g_camera_cntx.app_state == CAMERA_STATE_COUNTDOWN)
		{
			CAMERA_ENTER_STATE(CAMERA_STATE_PREVIEW);
		}
		else
		{
			MMI_ASSERT(0);
		}
	}
	else
	{
		g_camera_cntx.tile_cur_num = 0;
		
		/* sicne the tile preview buffer is using base layer's buffer, we have to clear it */
		gdi_layer_push_and_set_active(g_camera_cntx.preview_layer_handle);
		gdi_layer_clear(GDI_COLOR_BLACK);
		gdi_layer_pop_and_restore_active();
		
		CAMERA_ENTER_STATE(CAMERA_STATE_SAVE_CONFIRM);
	}
	
}
#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_sublcd_tileshot_capture()
* DESCRIPTION
*	 sublcd tileshot capture
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_CONT_SHOT_TILE__) && defined(__CAMERA_FEATURE_SELF_CAPTURE__)
static void mmi_camera_sublcd_tileshot_capture(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	S32 			offset_x;
	S32 			offset_y;
	
	#ifdef MMI_ON_HARDWARE_P
		GDI_RESULT	result;
	#endif /* MMI_ON_HARDWARE_P */


	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/	
	mmi_camera_turn_on_capture_led_highlight();

	g_camera_cntx.last_error
		= mdi_camera_capture_to_memory(&g_camera_cntx.capture_buf_ptr, &g_camera_cntx.capture_size);

	/* play capture sound after capture */
	mmi_camera_capture_play_sound();

	mmi_camera_turn_off_led_highlight();

	gdi_layer_push_and_set_active(g_camera_cntx.tileshot_result_layer_handle);

	offset_x = (g_camera_cntx.tile_cur_num%2)*(UI_device_width>>1);
	offset_y = (g_camera_cntx.tile_cur_num/2)*(UI_device_height>>1);	

	#ifdef MMI_ON_HARDWARE_P
		result = gdi_image_jpeg_draw(offset_x, offset_y, g_camera_cntx.capture_buf_ptr, g_camera_cntx.capture_size);
	#endif /* MMI_ON_HARDWARE_P */

	gdi_layer_pop_and_restore_active();

	g_camera_cntx.tile_cur_num++;

	if(g_camera_cntx.tile_cur_num < 4)
	{
		/* set maninlcd's blt layer, to display tile images */
		gdi_layer_set_blt_layer(g_camera_cntx.tileshot_result_layer_handle, 
										g_camera_cntx.add_frame_layer_handle,
										0, 0);
		
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
		if(g_camera_cntx.app_state == CAMERA_STATE_SUB_PREVIEW)
		{
			mmi_camera_sublcd_preview_start();
			
			if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
				mmi_trace(g_sw_CAMERA,"mmi_camera_tileshot_capture 2");
				mmi_camera_display_error_popup();
		}
		else if(g_camera_cntx.app_state == CAMERA_STATE_SUB_COUNTDOWN)
		{
			CAMERA_ENTER_STATE(CAMERA_STATE_SUB_PREVIEW);
		}
	}
	else
	{
		/* do not need blt, enter save confirm will blt */
		
		g_camera_cntx.is_dis_clam_open = FALSE;
		
		g_camera_cntx.tile_cur_num = 0;
		CAMERA_ENTER_STATE(CAMERA_STATE_SAVE_CONFIRM);
	}

}
#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ && __CAMERA_FEATURE_SELF_CAPTURE__ */



/*****************************************************************************
*
*	Save Confirm State
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_save_confirm_state()
* DESCRIPTION
*	 enter save confirm state
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
static void mmi_camera_entry_save_confirm_state(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	S32	image_width;
	S32	image_height;
	S32	offset_x;
	S32	offset_y;
	S32	resized_offset_x;
	S32	resized_offset_y;
	S32	resized_width;
	S32	resized_height;
	S32	result;
	S32	image_region_width;
	S32	image_region_height;	
	S32	image_region_offset_x;
	S32	image_region_offset_y;	

		
	#ifdef __MMI_TVOUT__
		S32 tvout_max_width;
		S32 tvout_max_height;
		S32 tvout_resized_width;
		S32 tvout_resized_height;
		S32 tvout_resized_offset_x;		
		S32 tvout_resized_offset_y;		
		GDI_HANDLE	tvout_layer;		
	#endif /* __MMI_TVOUT__ */

	TBM_ENTRY(0x2AA5);
    mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_entry_save_confirm_state()");
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	/* init var */
	g_camera_cntx.is_lsk_pressed = FALSE;
	g_camera_cntx.is_rsk_pressed = FALSE;	
	

	/* register soft keys */	
	ClearAllKeyHandler();
	
	SetKeyHandler(mmi_camera_save_confirm_lsk_press, KEY_LSK, KEY_EVENT_DOWN); 
	SetKeyHandler(mmi_camera_save_confirm_lsk_release, KEY_LSK, KEY_EVENT_UP); 	
#if defined(MEM_ULC_3216) // enable OK to save image
	SetKeyHandler(mmi_camera_save_confirm_lsk_press, KEY_ENTER, KEY_EVENT_DOWN); 
	SetKeyHandler(mmi_camera_save_confirm_lsk_release, KEY_ENTER, KEY_EVENT_UP); 	
#endif	
	SetKeyHandler(mmi_camera_save_confirm_rsk_press, KEY_RSK, KEY_EVENT_DOWN); 
	SetKeyHandler(mmi_camera_save_confirm_rsk_release, KEY_RSK, KEY_EVENT_UP); 

	SetKeyHandler(mmi_camera_save_confirm_lsk_release, CAMERA_CAPTURE_EXTRA_KEY, KEY_EVENT_DOWN); 
	
	/* clearn center key - since if not clear, this key will mappign to LSK */
#ifndef __MMI_ANALOGTV__    
	if(CAMERA_CAPTURE_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_KEY, KEY_EVENT_DOWN);	
#endif
	mmi_camera_preview_free_preview_resource();
	gdi_layer_push_and_set_active(g_camera_cntx.base_layer_handle);
	gdi_layer_clear_background(GDI_COLOR_BLACK);
	gdi_layer_pop_and_restore_active();	
			
	/* init title bar */
	if(g_camera_cntx.active_camera_osd_ptr->title.is_draw_mmi_title_bar)
	{
		#if defined( __MMI_UI_STYLE_3__) || defined(__MMI_UI_STYLE_4__)
			reset_title_status_icon();
			show_status_icon_bar(0);
		#endif /* __MMI_UI_STYLE_3__ */

		/* hide short cut */
		MMI_menu_shortcut_number = -1;

		if(g_camera_cntx.active_camera_osd_ptr->title.is_draw_mmi_caption)
		{
			ChangeTitle(MAIN_MENU_TITLE_MULTIMEDIA_ICON, (PU8)GetString(STR_ID_CAMERA_APP_NAME));
		}
		else
		{
			ChangeTitle(0, (PU8)GetString(STR_ID_CAMERA_APP_NAME));
		}
	}
	else
	{
		#if defined( __MMI_UI_STYLE_3__) || defined(__MMI_UI_STYLE_4__)
			hide_status_icon_bar(0);
		#endif /* __MMI_UI_STYLE_3__ */
	}

	if(g_camera_cntx.is_tileshot)
	{
		#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
		
			#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
				/* draw sublcd icon */
				mmi_camera_sublcd_draw_icon();
			#endif 

			gdi_layer_set_blt_layer(g_camera_cntx.tileshot_result_layer_handle,
											g_camera_cntx.add_frame_layer_handle,
											g_camera_cntx.osd_layer_handle, 
											0);

			/* set TV out mode */
			/* if is full screen multimedia mode, we will decode jpeg to a buffer, then blt to TV */
			#ifdef __MMI_TVOUT__
				if(mdi_tvout_is_enable())
				{
					if(mmi_phnset_tvout_get_style() == SETTING_TVOUT_STYLE_FULLSCREEN)
					{

						/* if is fullscreen, we shall set to owner to camera here, to avoid GDI's update */
						mdi_tvout_set_owner(MDI_TV_OWNER_MMI);			

						/* start MMI owner draw mode */
						mdi_tvout_set_mode(	MDI_TV_MODE_OWNER_DRAW,
													UI_device_width,			/* layer_width */
													UI_device_height);		/* layer_height */

						tvout_layer = mdi_tvout_get_active_layer();

						gdi_layer_push_and_set_active(tvout_layer);
						gdi_layer_reset_clip();
						gdi_layer_flatten(g_camera_cntx.tileshot_result_layer_handle, 0, 0, 0);
						
						gdi_layer_pop_and_restore_active();

						/* blt to TV */
						mdi_tvout_blt();
		
					}
				}
			#endif /* __MMI_TVOUT__ */
			
		#else
			MMI_ASSERT(0);
		#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */
	}
	else
	{

		/* clear base layer background */
		//mmi_camera_preview_clear_background();

		gdi_layer_set_blt_layer(g_camera_cntx.base_layer_handle,
										0,
										g_camera_cntx.osd_layer_handle, 
										0);
	
		#ifdef MMI_ON_HARDWARE_P
			/* get file dimension */
			result =	gdi_image_jpeg_get_dimension(	g_camera_cntx.capture_buf_ptr, 
																g_camera_cntx.capture_size,
																&image_width, 
																&image_height);
		#else 
			/* remove compile warning */
			result = 0;
			image_width = 0;
			image_height = 0;
		#endif /* MMI_ON_HARDWARE_P */

		/* set clip region as content region */
		gdi_layer_reset_clip();

		/* calc image region */
		image_region_width 		= g_camera_cntx.osd_UI_device_width;
		image_region_height 		= g_camera_cntx.osd_UI_device_height;
		image_region_offset_x 	= 0;
		image_region_offset_y	= 0;
		
		if(g_camera_cntx.active_camera_osd_ptr->title.is_draw_mmi_title_bar)
		{
			image_region_height 		-= (MMI_title_height+MMI_title_y); 
			image_region_offset_y	+= (MMI_title_height+MMI_title_y);
		}

		if(g_camera_cntx.active_camera_osd_ptr->softkey.is_draw_mmi_softkey)
		{
			image_region_height -= (MMI_button_bar_height); 
		}

		mmi_trace(g_sw_CAMERA, "Excute funs: mmi_camera_entry_save_confirm_state() result = %ld",result);
		if( result>= 0 )
		{
			if( (image_region_width >= image_width) && (image_region_height >= image_height))
			{
				/* align preview window at the center of screen */
				offset_x = image_region_offset_x+((image_region_width-image_width)>>1);
				offset_y = image_region_offset_y+((image_region_height-image_height)>>1);
#if 0		
				#ifndef WIN32
					gdi_image_jpeg_draw(	offset_x,
												offset_y, 
												g_camera_cntx.capture_buf_ptr, 
												g_camera_cntx.capture_size);

				#endif /* WIN32 */
#endif				
				resized_width  = image_width;
				resized_height = image_height;
			}
			else
			{
				gdi_image_util_fit_bbox(	image_region_width, 
													image_region_height,
													image_width, image_height,
													&resized_offset_x, &resized_offset_y,
													&resized_width, &resized_height);

#if 0			
				#ifndef WIN32
					gdi_image_jpeg_draw_resized(	resized_offset_x+image_region_offset_x, 
															resized_offset_y+image_region_offset_y, 
															resized_width,
															resized_height,
															g_camera_cntx.capture_buf_ptr, 
															g_camera_cntx.capture_size);
				
				#endif /* WIN32 */
#endif				
				offset_x = resized_offset_x+image_region_offset_x;
				offset_y = resized_offset_y+image_region_offset_y;
			}


			/* set TV out mode */
			/* if is full screen multimedia mode, we will decode jpeg to a buffer, then blt to TV */
			#ifdef __MMI_TVOUT__
				if(mdi_tvout_is_enable())
				{
					if(mmi_phnset_tvout_get_style() == SETTING_TVOUT_STYLE_FULLSCREEN)
					{
						tvout_max_width = 320;
						tvout_max_height = 240;		

						if( (tvout_max_width >= image_width) && (tvout_max_height >= image_height))
						{
							tvout_resized_width 	= image_width;
							tvout_resized_height = image_height;								
						}
						else
						{
							gdi_image_util_fit_bbox(tvout_max_width,
															tvout_max_height,
															image_width, image_height,
															&tvout_resized_offset_x, &tvout_resized_offset_y,
															&tvout_resized_width, &tvout_resized_height);
						}

						/* if is fullscreen, we shall set to owner to camera here, to avoid GDI's update */
						mdi_tvout_set_owner(MDI_TV_OWNER_MMI);			

						/* start MMI owner draw mode */
						mdi_tvout_set_mode(	MDI_TV_MODE_OWNER_DRAW,
													tvout_resized_width,			/* layer_width */
													tvout_resized_height);		/* layer_height */

						tvout_layer = mdi_tvout_get_active_layer();

						gdi_layer_push_and_set_active(tvout_layer);
						gdi_layer_reset_clip();
						gdi_layer_clear(GDI_COLOR_BLACK);
#if 0						
						#ifndef WIN32
							gdi_image_jpeg_draw_resized(	0, 
																	0, 
																	tvout_resized_width,
																	tvout_resized_height,
																	g_camera_cntx.capture_buf_ptr, 
																	g_camera_cntx.capture_size);						
						#endif
#endif						
						gdi_layer_pop_and_restore_active();

						/* blt to TV */
						mdi_tvout_blt();
					}
				}
			#endif /* __MMI_TVOUT__ */
			
		}
	}

	
	#ifdef __MMI_TVOUT__
		/* restore owner to camera */
		if(mdi_tvout_is_enable())
		{
			if(mmi_phnset_tvout_get_style() == SETTING_TVOUT_STYLE_FULLSCREEN)
			{
				/* if is fullscreen, we shall set to owner to camera here, to avoid GDI's update */
				mdi_tvout_set_owner(MDI_TV_OWNER_CAMERA);				

				/* start mode 2 - full screen preview update */
				mdi_tvout_set_mode(	MDI_TV_MODE_CAMERA_PREVIEW,
											0,						/* layer_width */
											0);					/* layer_height */
			}
		}
	#endif /* __MMI_TVOUT__ */


	/* draw save notify text */
	set_mmc_camera_preview();
	mmi_camera_preview_set_hint((PS8)GetString(STR_ID_CAMERA_NOTIFY_SAVE_CONFIRM), NULL, 0);
	mmi_camera_preview_draw_osd();

	/* blt */
	//gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	/*WeiD Add Start For 6195 Ver: TBM780  on 2007-8-10 18:32 */
	COS_Sleep(10);
	/*WeiD Add End  For 6195 Ver: TBM780  on 2007-8-10 18:32 */
	//offset_x = 0;
	//offset_y = 16;
	//resized_width = 128;
	//resized_height = 128;
	//MCI_CamPlayBack(offset_x,offset_y,resized_width,resized_height);//chenhe for jasper playback
       MCI_CamPlayBack();
	/*gaosh Modify End  For 6270  Ver: TBM780_T_20070814_1929  on 2007-8-15 16:42 */

// Beginning--modified by kecx for fixing bug 14537 on 20090928
       coolsand_UI_cancel_timer(mmi_camera_preview_wait_key7_hdlr);  
       ClearKeyHandler(CAMERA_IMAGE_SIZE_KEY, KEY_EVENT_DOWN);
// end--modified by kecx for fixing bug 14537 on 20090928
    
	TBM_EXIT(0x2AA5);
}




/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_save_confirm_state()
* DESCRIPTION
*	 exit save confirm state
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
static void mmi_camera_exit_save_confirm_state(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
}


/*****************************************************************************
* FUNCTION
*	 mmi_camera_save_confirm_lsk_press()
* DESCRIPTION
*   draw save confirm softkey
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_save_confirm_lsk_press(void)
{
	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_save_confirm_lsk_press()");
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#if 0
	g_camera_cntx.is_lsk_pressed = TRUE;
	mmi_camera_preview_draw_osd();

	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
#endif	
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_save_confirm_lsk_release()
* DESCRIPTION
*   lsk release
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/

static void mmi_camera_save_confirm_lsk_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#if 0
	g_camera_cntx.is_lsk_pressed = FALSE;
  	mmi_camera_preview_draw_osd();	 
	
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
#endif
	mmi_camera_save_confirm_save_file();
#if defined(__MMI_MAINLCD_220X176__)
	mmi_revert_nokia_popup_dialog_component();
#endif
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_save_confirm_rsk_press()
* DESCRIPTION
*   draw save confirm softkey
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_save_confirm_rsk_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#if 0
	g_camera_cntx.is_rsk_pressed = TRUE;
	mmi_camera_preview_draw_osd();	
	
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
#endif	
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_save_confirm_rsk_release()
* DESCRIPTION
*   draw save confirm softkey
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_save_confirm_rsk_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#if 0
	g_camera_cntx.is_rsk_pressed = FALSE;
	
   mmi_camera_preview_draw_osd();	 
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
#endif	
	mmi_camera_save_confirm_cancel_file();
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_save_confirm_cancel_file()
* DESCRIPTION
*   cancel file, do not save
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_save_confirm_cancel_file(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	if(g_camera_cntx.capture_buf_ptr != NULL)
	{
		mmi_trace(g_sw_CAMERA, "Excute funs: mmi_camera_save_confirm_cancel_file Save_File is TRUE");
		FreeMediaMemory((unsigned long)g_camera_cntx.capture_buf_ptr);
		g_camera_cntx.capture_buf_ptr = NULL;
	}
	MCI_CamCancelLastSave();
	mmi_trace(1,"mmi_camera_save_confirm_cancel_file,cancle last camera file save222");
	/* enter preview state */
	CAMERA_ENTER_STATE(CAMERA_STATE_PREVIEW);
}

#if defined(__MMI_MAINLCD_220X176__)
extern U8 nokia_popup_dialog[];
extern s16 coordinate_nokia_popup_dialog_set_nosoftkey[];
U8 nokia_popup_dialog_camera[] = 
{
	5,
	DM_BASE_LAYER_START,
	DM_POPUP_BACKGROUND,
	DM_IMAGE,
	DM_MULTILINE_INPUTBOX1, 
	DM_BUTTON_BAR1
};
s16 coordinate_nokia_popup_dialog_set_nosoftkey_camera[]=
{
    DM_FULL_SCREEN_COORDINATE_FLAG,
    0,(UI_DEVICE_HEIGHT>>1),UI_DEVICE_WIDTH,(UI_DEVICE_HEIGHT>>1)-MMI_BUTTON_BAR_HEIGHT,0,
    180,(UI_DEVICE_HEIGHT>>1)+10,60,60,DM_NO_FLAGS,
    14,(UI_DEVICE_HEIGHT>>1)+10,UI_DEVICE_WIDTH-(UI_DEVICE_WIDTH>>2),(UI_DEVICE_HEIGHT>>1)-10-MMI_BUTTON_BAR_HEIGHT,DM_NO_FLAGS,
    DM_DEFAULT_BUTTON_BAR_FLAG, MMI_SOFTKEY_WIDTH
};
U8 g_nokia_popup_dialog_tmp[] = 
{
	6,
	DM_NEW_LAYER_START,
	DM_CATEGORY_CONTROLLED_AREA,
	DM_BASE_LAYER_START,
	DM_POPUP_BACKGROUND,	
	DM_IMAGE,
	DM_MULTILINE_INPUTBOX1,  
};
S16 g_coordinate_nokia_popup_dialog_set_nosoftkey_tmp[]=
{
	DM_FULL_SCREEN_COORDINATE_FLAG,
	DM_POPUP_SCREEN_COORDINATE,DM_DRAW_POPUP_BACKGROUND_3D,
	DM_FULL_SCREEN_COORDINATE_FLAG,
	MMI_POP_UP_DIALOG_X,MMI_POP_UP_DIALOG_Y,UI_DEVICE_WIDTH,MMI_POP_UP_DIALOG_Y + 78,DM_NO_FLAGS,
	180,MMI_POP_UP_DIALOG_Y+10,40,(MMI_POP_UP_DIALOG_HEIGHT>>1),DM_NO_FLAGS,
	MMI_POP_UP_DIALOG_X,MMI_POP_UP_DIALOG_Y+20,180,(MMI_POP_UP_DIALOG_HEIGHT>>1),DM_LEFT_ALIGN_X
};
/********************************************************************************************
*DESCRIPTION:This component,DM_NEW_LAYER_START,can not create a new layer in camera screen for memory
*				size restrict(GDI_ASSERT(0),gdi_layer.c line655).modify category component to avoid collapse.
*********************************************************************************************/
void mmi_modify_nokia_popup_dialog_component_avoid_camera_collapse(void)
{
	memcpy(nokia_popup_dialog,nokia_popup_dialog_camera,sizeof(nokia_popup_dialog_camera));
	memcpy(coordinate_nokia_popup_dialog_set_nosoftkey,coordinate_nokia_popup_dialog_set_nosoftkey_camera,sizeof(coordinate_nokia_popup_dialog_set_nosoftkey_camera));
}

/********************************************************************************************
*DESCRIPTION:nokia_popup_dialog[] in custcoordinates.c can work well except camera.
*********************************************************************************************/
void mmi_revert_nokia_popup_dialog_component(void)
{
	mmi_modify_nokia_popup_dialog_component_avoid_camera_collapse();
	memcpy(nokia_popup_dialog,g_nokia_popup_dialog_tmp,sizeof(g_nokia_popup_dialog_tmp));
	memcpy(coordinate_nokia_popup_dialog_set_nosoftkey,g_coordinate_nokia_popup_dialog_set_nosoftkey_tmp,sizeof(g_coordinate_nokia_popup_dialog_set_nosoftkey_tmp));
}
#endif



/*****************************************************************************
* FUNCTION
*	 mmi_camera_save_confirm_save_file()
* DESCRIPTION
*   save file
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
extern BOOL AudioStopStream();

static void mmi_camera_save_confirm_save_file(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

       INT32  filehandle=0;
	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_save_confirm_save_file()");
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__

		/* copy tiled picture from tile result layer to base layer */
		if(g_camera_cntx.is_tileshot)
		{
	
			#ifdef __CAMERA_FEATURE_ADD_FRAME__
			if(g_camera_cntx.osd_rotate==CAMERA_DISPLAY_ROTATE_0 && g_camera_cntx.setting.add_frame != 0)
			{
				gdi_layer_push_and_set_active(g_camera_cntx.tileshot_result_layer_handle);

				/* if has add frame, merge layers */
				gdi_layer_flatten(g_camera_cntx.tileshot_result_layer_handle, 
										g_camera_cntx.add_frame_layer_handle,
										0,	0);
				gdi_layer_pop_and_restore_active();
			}
			#endif /* __CAMERA_FEATURE_ADD_FRAME__ */

				/* also copy result to base - this is for when popup show, the result will show at bg */
				gdi_layer_flatten(g_camera_cntx.tileshot_result_layer_handle, 0, 0, 0);
				
		}
	#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */

	/* blt base only */
	gdi_layer_set_blt_layer(g_camera_cntx.base_layer_handle, 0, 0, 0);

	
   gdi_layer_lock_frame_buffer();   

	/* show popup category */
	#if defined(__CAMERA_OSD_HORIZONTAL__) || defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270 && mmi_frm_get_screen_rotate() !=CAMERA_DISPLAY_ROTATE_270 )
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);
	#endif

	#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
	force_no_small_screen();
	#endif
#if defined(__MMI_MAINLCD_220X176__)
	mmi_modify_nokia_popup_dialog_component_avoid_camera_collapse();
#endif
//	ShowCategory65Screen((PU8)GetString(STR_GLOBAL_SAVE), IMG_GLOBAL_SAVE, NULL);
//	ShowCategory65Screen((PU8)GetString(STR_GLOBAL_SAVE), NULL, NULL);
//	g_camera_cntx.is_cat65_show = TRUE;

	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
	
	if(g_camera_cntx.is_tileshot)
	{
		#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__		
			mmi_camera_get_next_filename(buf_filename);
			
			if(g_camera_cntx.setting.add_frame != CAMERA_SETTING_ADD_FRAME_OFF)
			{
				
				if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
				{
					g_camera_cntx.last_error
						= mdi_camera_encode_layer_to_jpeg(g_camera_cntx.tileshot_result_layer_handle, buf_filename);
				}
				else
				{
					g_camera_cntx.last_error
						= mdi_camera_encode_layer_to_jpeg(g_camera_cntx.tileshot_result_layer_handle, buf_filename);			
				}
			}
			else
			{
				if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
				{
					g_camera_cntx.last_error
						= mdi_camera_encode_layer_to_jpeg(g_camera_cntx.tileshot_result_layer_handle, buf_filename);
				}
				else
				{
					g_camera_cntx.last_error
						= mdi_camera_encode_layer_to_jpeg(g_camera_cntx.tileshot_result_layer_handle, buf_filename);			
				}
				
			}

			if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
			{
				/* will display a popup, go out application and re-enter */
				if(g_camera_cntx.is_cat65_show)
				{
					g_camera_cntx.is_cat65_show = FALSE;
					ExitCategoryFunction();
				}
				mmi_trace(g_sw_CAMERA,"mmi_camera_save_confirm_save_file 0");
				mmi_camera_display_error_popup();
				return;
			}
		#else
			MMI_ASSERT(0);
		#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */
		
	}
	else
	{
			if(g_camera_cntx.capture_buf_ptr != NULL)
			{
				mmi_trace(g_sw_CAMERA, "Excute funs: Save_File is TRUE");
				FreeMediaMemory((unsigned long)g_camera_cntx.capture_buf_ptr);
				g_camera_cntx.capture_buf_ptr = NULL;
			}

		//AudioStopStream();//chenhe add to avoid of using voc confilctly by amr decode and jpg encode
		mdi_audio_stop_all();//chenhe 
		
#if (CSW_EXTENDED_API_CAMERA == 0)
{
	char  buf_filename[ 520 ];
	
       mmi_camera_get_next_filename(buf_filename);
	filehandle = MMI_FS_Open(buf_filename, FS_CREATE_ALWAYS);
	mmi_trace(g_sw_CAMERA,"save confirm save photo by gugogi  buf_filename=%s,filehandle=%d",buf_filename,filehandle);


	if(filehandle>=0)
	{
	#ifndef MMI_ON_WIN32
		g_camera_cntx.last_error	= media_cam_save_photo(filehandle);
	#endif
		 MMI_FS_Close(filehandle);

	}
	else
	{
		if(FS_DISK_FULL == filehandle)
		{
			g_camera_cntx.last_error = MDI_RES_CAMERA_ERR_DISK_FULL;
		}
		else
		{
			g_camera_cntx.last_error = filehandle;

		}
	}
	 

	 if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
	 	{
	 	    mmi_trace(g_sw_CAMERA,"***delete the 0 size file***");
	           MMI_FS_Delete((const UINT8 *)buf_filename);
	 	}

       
}
#else
            {
                char buf_filename[ 520 ];
                //char filename_xuml[520];
                //uint8 tmp[128]="/t";

                mmi_camera_get_next_filename(buf_filename);
		  #if 0
                UnicodeToAnsii(filename_xuml, buf_filename);
                strcat(tmp,filename_xuml+2);
                AnsiiToUnicodeString(buf_filename,tmp);
		  #endif
		  #ifndef MMI_ON_WIN32
		  g_camera_cntx.last_error = media_cam_save_photo(buf_filename);
		  #endif
            }
#endif
			
#if 0 //chenhe for jasper
		INT32 retdisk = 0;
		/* this function has nvram access */
		mmi_camera_get_next_filename(buf_filename);
		mmi_trace_hex(g_sw_CAMERA, UCS2Strlen(buf_filename) * 2, buf_filename);



		file_handle = MMI_FS_Open((U16*)buf_filename, FS_CREATE);
		file_handle = 0;//chenhe for jasper
		mmi_trace(g_sw_CAMERA, "file_handle = %d", file_handle);
		if(file_handle >= 0)
		{		
			mmi_trace(g_sw_CAMERA, "capture_size = %d", g_camera_cntx.capture_size);
			if(g_camera_cntx.capture_buf_ptr == NULL)
			{
				mmi_trace(g_sw_CAMERA, "before write capture_buf_ptr is NULL");
			}
			MMI_FS_Write(file_handle, (void*)g_camera_cntx.capture_buf_ptr, g_camera_cntx.capture_size, (UINT*)&written);
			mmi_trace(g_sw_CAMERA, "written = %d", written);
			MMI_FS_Close(file_handle);
			if(g_camera_cntx.capture_buf_ptr != NULL)
			{
				mmi_trace(g_sw_CAMERA, "Excute funs: Save_File is TRUE");
				FreeMediaMemory((unsigned long)g_camera_cntx.capture_buf_ptr);
				g_camera_cntx.capture_buf_ptr = NULL;
			}
			/* cant write in whole file, must be disk full */
			if(g_camera_cntx.capture_size != written)
			{
				/* delete the file that is not save complete */
				MMI_FS_Delete((U16*)buf_filename);
				
				g_camera_cntx.last_error = MDI_RES_CAMERA_ERR_DISK_FULL;

				if(g_camera_cntx.is_cat65_show)
				{
					g_camera_cntx.is_cat65_show = FALSE;
					ExitCategoryFunction();
				}

				if(g_camera_cntx.capture_buf_ptr != NULL)
				{
					mmi_trace(g_sw_CAMERA, "Excute funs: Next Save_File is TRUE");
					FreeMediaMemory((unsigned long)g_camera_cntx.capture_buf_ptr);
					g_camera_cntx.capture_buf_ptr = NULL;
				}
				/* will display a popup, go out application and re-enter */
				mmi_trace(g_sw_CAMERA,"mmi_camera_save_confirm_save_file 1");
				mmi_camera_display_error_popup();
				return;
			}
		}
		else
		{
			if(file_handle == FS_DISK_FULL)
			{
				g_camera_cntx.last_error = MDI_RES_CAMERA_ERR_DISK_FULL;
			}
			else if(file_handle == FS_ROOT_DIR_FULL)
			{
				g_camera_cntx.last_error = CAMERA_ERR_ROOT_DIR_FULL;
			}			
			else if(file_handle == FS_WRITE_PROTECTION)
			{
				g_camera_cntx.last_error = MDI_RES_CAMERA_ERR_WRITE_PROTECTION;
			}
			else
			{
				g_camera_cntx.last_error = MDI_RES_CAMERA_ERR_CAPTURE_FAILED;
			}

			if(g_camera_cntx.is_cat65_show)
			{
				g_camera_cntx.is_cat65_show = FALSE;
				ExitCategoryFunction();
			}

			/* will display a popup, go out application and re-enter */
			mmi_trace(g_sw_CAMERA,"mmi_camera_save_confirm_save_file 2");
			mmi_camera_display_error_popup();
			return;
		}
#endif
	}

	if(g_camera_cntx.is_cat65_show)
	{
		g_camera_cntx.is_cat65_show = FALSE;
		ExitCategoryFunction();
	}

	/* enter save done */
	if(g_camera_cntx.last_error!=ERR_SUCCESS)
		mmi_camera_display_error_popup();
	else
	       CAMERA_ENTER_STATE(CAMERA_STATE_SAVE_DONE);

}




/*****************************************************************************
*
*	Save Done State 
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_save_done_state()
* DESCRIPTION
*	 enter save done state, show a save done popup 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
static void mmi_camera_entry_save_done_state(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* clear all keys */
	ClearAllKeyHandler();
	mmi_trace(g_sw_CAMERA, "Excute funs: mmi_camera_entry_save_done_state() is TRUE");
	/* draw base layer only */
	gdi_layer_set_blt_layer(g_camera_cntx.base_layer_handle,0 ,0 ,0);

	gdi_layer_lock_frame_buffer();
	
	/* show popup category */
	#if defined(__CAMERA_OSD_HORIZONTAL__) || defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270 && mmi_frm_get_screen_rotate() !=CAMERA_DISPLAY_ROTATE_270 )
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_270);
	#endif

	#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
	force_no_small_screen();
	#endif

	ShowCategory65Screen((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, NULL);
//	ShowCategory65Screen((PU8)GetString(STR_GLOBAL_DONE), NULL, NULL);
	g_camera_cntx.is_cat65_show = TRUE;	

	gdi_layer_unlock_frame_buffer();

	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
	
	coolsand_UI_start_timer(DONE_POPUP_FADE_TIME, mmi_camera_save_done_timeout);
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_save_done_state()
* DESCRIPTION
*	 exit save done state
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
static void mmi_camera_exit_save_done_state(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	/* will display a popup, go out application and re-enter */
	if(g_camera_cntx.is_cat65_show)
	{
		g_camera_cntx.is_cat65_show = FALSE;
		ExitCategoryFunction();
	}

	coolsand_UI_cancel_timer(mmi_camera_save_done_timeout);

	/* stop anmation on popup category */
	gdi_image_stop_animation_all();
	#if defined(__CAMERA_OSD_HORIZONTAL__) || defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
	{
		mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_0);
		gdi_layer_push_and_set_active(g_camera_cntx.base_layer_handle);
		gdi_layer_set_rotate(g_camera_cntx.osd_rotate);
		if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
		{
			mmi_frm_set_self_rotation_flag(MMI_FRM_SCREEN_ROTATE_270);
			gdi_layer_resize(LCD_HEIGHT, LCD_WIDTH);
			g_camera_cntx.base_UI_device_height = LCD_WIDTH; 
			g_camera_cntx.base_UI_device_width  = LCD_HEIGHT;

		}
		else
		{
			mmi_frm_set_self_rotation_flag(MMI_FRM_SCREEN_ROTATE_0);
			gdi_layer_resize(LCD_WIDTH, LCD_HEIGHT);
			g_camera_cntx.base_UI_device_height = LCD_HEIGHT; 
			g_camera_cntx.base_UI_device_width  = LCD_WIDTH;

		}
		gdi_layer_clear_background(GDI_COLOR_BLACK);
		gdi_layer_pop_and_restore_active();

	}
	#endif


}


/*****************************************************************************
* FUNCTION
*	 mmi_camera_save_done_timeout()
* DESCRIPTION
*	 save done popup timeout
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
static void mmi_camera_save_done_timeout(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* enter sub preview state */
	CAMERA_ENTER_STATE(CAMERA_STATE_PREVIEW);
}




/*****************************************************************************
*
*	OSD Menu State
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_osd_menu_state()
* DESCRIPTION
*	 enter osd menu state
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_entry_osd_menu_state(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	S32				offset_x;
	S32				i;

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* init var */
	g_camera_cntx.is_left_arrow_pressed 	= FALSE;
	g_camera_cntx.is_right_arrow_pressed 	= FALSE;
	g_camera_cntx.is_up_arrow_pressed 		= FALSE;
	g_camera_cntx.is_down_arrow_pressed 	= FALSE;		
	g_camera_cntx.is_iconlist_moving			= FALSE;


	/* register soft keys */	
	ClearAllKeyHandler();
	
	SetKeyHandler(mmi_camera_osd_menu_lsk_press, 	KEY_LSK, KEY_EVENT_DOWN); 	
	SetKeyHandler(mmi_camera_osd_menu_rsk_release, 	KEY_RSK, KEY_EVENT_UP); 

	SetKeyHandler(mmi_camera_osd_menu_poplist_down_arrow_press,			KEY_DOWN_ARROW,	KEY_EVENT_DOWN);
	SetKeyHandler(mmi_camera_osd_menu_poplist_up_arrow_press,			KEY_UP_ARROW,		KEY_EVENT_DOWN);

	SetKeyHandler(mmi_camera_osd_menu_poplist_down_arrow_release,		KEY_DOWN_ARROW,	KEY_EVENT_UP);
	SetKeyHandler(mmi_camera_osd_menu_poplist_up_arrow_release,			KEY_UP_ARROW,		KEY_EVENT_UP);

	SetKeyHandler(mmi_camera_osd_menu_left_arrow_press,		KEY_LEFT_ARROW,	KEY_EVENT_DOWN);
	SetKeyHandler(mmi_camera_osd_menu_right_arrow_press,		KEY_RIGHT_ARROW,	KEY_EVENT_DOWN);

	SetKeyHandler(mmi_camera_osd_menu_left_arrow_release,		KEY_LEFT_ARROW,	KEY_EVENT_UP);
	SetKeyHandler(mmi_camera_osd_menu_right_arrow_release,	KEY_RIGHT_ARROW,	KEY_EVENT_UP);

	/* clearn center key - since if not clear, this key will mappign to LSK */
#ifndef __MMI_ANALOGTV__    
	if(CAMERA_CAPTURE_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_KEY, KEY_EVENT_DOWN);	
#endif
	if(CAMERA_CAPTURE_EXTRA_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_EXTRA_KEY, KEY_EVENT_DOWN);

	/* stop hide hint timer, avoid timeout and redraw */
	coolsand_UI_cancel_timer(mmi_camera_preview_hide_hint);

	/* icon list shall init first, before pop list */
	mmi_camera_osd_menu_init_iconlist();

	i = g_cam_ui_iconlist.highlighted_index;
		
	offset_x = g_cam_ui_iconlist.img_icon[i].offset_x+(g_cam_ui_iconlist.img_icon[i].width>>1);
	offset_x -= g_camera_osd_menu_cntx.poplist_left_margin;
			
	mmi_camera_osd_menu_init_poplist(offset_x);

	/* draw first iconlist */
	gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);
	gdi_layer_toggle_double();
	gdi_layer_clear(GDI_COLOR_TRANSPARENT);
	mmi_camera_osd_menu_draw_iconlist();
	gdi_layer_pop_and_restore_active();
	
	/* set icon list shift and start to move in */
	g_camera_cntx.iconlist_offset_y  = g_cam_ui_iconlist.height;
	g_camera_cntx.is_need_clear_all	= TRUE;

	mmi_camera_osd_menu_iconlist_move_in();	

}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_osd_menu_state()
* DESCRIPTION
*	 exit osd menu state
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_exit_osd_menu_state(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_exit_osd_menu_state()");
	
	/* stop timer */
	coolsand_UI_cancel_timer(mmi_camera_osd_menu_iconlist_move_in);
	coolsand_UI_cancel_timer(mmi_camera_osd_menu_iconlist_move_out);
	coolsand_UI_cancel_timer(mmi_camera_capture_play_sound);
	
	/* save changed settings to NVRAM */
	mmi_camera_store_setting();

	/* stop previewing */
	mmi_camera_preview_stop();

	/* toggle to sub */
	if(g_camera_cntx.app_next_state == CAMERA_STATE_TOGGLE_TO_SUB)
	{
		/* turn off led highlight */
		mmi_camera_turn_off_led_highlight();

		#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
			/* copy tiled picture from tile result layer to base layer */
			if(g_camera_cntx.is_tileshot)
			{
				#ifdef __CAMERA_FEATURE_ADD_FRAME__
					/* if has add frame, merge layers to base  */
					gdi_layer_flatten_to_base(	g_camera_cntx.tileshot_result_layer_handle, 
														g_camera_cntx.add_frame_layer_handle,
														0,	0);
				#else
					/* no add frame */
					gdi_layer_flatten_to_base(	g_camera_cntx.tileshot_result_layer_handle, 
														0, 0,	0);
				#endif 
			}
		#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */

		if(g_camera_cntx.is_fullscreen)
		{
			gdi_layer_set_blt_layer(g_camera_cntx.base_layer_handle, 
											g_camera_cntx.add_frame_layer_handle, 
											0, 0);
		}
		else
		{
			gdi_layer_set_blt_layer(g_camera_cntx.base_layer_handle, 0, 0, 0);
		}
		
		#if defined( __MMI_UI_STYLE_3__) || defined(__MMI_UI_STYLE_4__)
			hide_status_icon_bar(0);
		#endif /* __MMI_UI_STYLE_3__ */	
	}
	
	/* free preview resource */
	if(g_camera_cntx.app_next_state == CAMERA_STATE_EXIT)
	{
		/* turn off led highlight */
		mmi_camera_turn_off_led_highlight();

		/* flatten layer */
		mmi_camera_preview_flatten_layer(TRUE);	

	}
	
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_draw_osd()
* DESCRIPTION
*	 draw menu osd 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_draw_osd(void)
{
	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_osd_menu_draw_osd()");
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);

	/* toggle double buffer */
	gdi_layer_toggle_double();
	
	gdi_layer_clear(GDI_COLOR_TRANSPARENT);

	mmi_camera_osd_menu_draw_poplist();
	mmi_camera_osd_menu_draw_iconlist();

	gdi_layer_pop_and_restore_active();
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */




/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_lsk_press()
* DESCRIPTION
*	 osd menu state lsk pressed hdlr 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_lsk_press(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* if not moving, move it */
	if(!g_camera_cntx.is_iconlist_moving)
	{
		g_camera_cntx.is_need_clear_all 	= TRUE;
		g_camera_cntx.iconlist_offset_y	= 0;

		gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);

		gdi_layer_toggle_double();
		
		/* clear whole screen */
		gdi_layer_clear(GDI_COLOR_TRANSPARENT);

		/* draw icon list */
		mmi_camera_osd_menu_draw_iconlist();
		
		gdi_layer_pop_and_restore_active();
	
		/* moving out */
		mmi_camera_osd_menu_iconlist_move_out();
	}
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_rsk_release()
* DESCRIPTION
*	 osd menu state rsk release hdlr 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_rsk_release(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	GoBackHistory();
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_iconlist_move_in()
* DESCRIPTION
*	 icon list move in
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_iconlist_move_in(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	g_camera_cntx.is_iconlist_moving = TRUE;

	/* moving up 10 pixel */
	g_camera_cntx.iconlist_offset_y -= 10;

	if(g_camera_cntx.iconlist_offset_y < 0)
	{
		g_camera_cntx.iconlist_offset_y = 0;
	}

	if(g_camera_cntx.iconlist_offset_y > 0)
	{
		coolsand_UI_start_timer(100, mmi_camera_osd_menu_iconlist_move_in);
	}

	gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);
	gdi_layer_set_position(0, g_camera_cntx.iconlist_offset_y);
	gdi_layer_pop_and_restore_active();
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

	if(g_camera_cntx.iconlist_offset_y <= 0)
	{
		/* move to up, draw osd */
		g_camera_cntx.is_iconlist_moving = FALSE;
		mmi_camera_osd_menu_draw_osd();

		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	}

}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_iconlist_move_out()
* DESCRIPTION
*	 icon list move in
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_iconlist_move_out(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	g_camera_cntx.is_iconlist_moving = TRUE;

	/* moving up 10 pixel */
	g_camera_cntx.iconlist_offset_y += 10;

	if(g_camera_cntx.iconlist_offset_y > g_cam_ui_iconlist.height)
	{
		g_camera_cntx.iconlist_offset_y = g_cam_ui_iconlist.height;
	}

	/* start loop or teminate move out */
	if(g_camera_cntx.iconlist_offset_y < g_cam_ui_iconlist.height)
	{
		coolsand_UI_start_timer(100, mmi_camera_osd_menu_iconlist_move_out);
	}

	gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);
	gdi_layer_set_position(0, g_camera_cntx.iconlist_offset_y);
	gdi_layer_pop_and_restore_active();
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

	/* start loop or teminate move out */
	if(g_camera_cntx.iconlist_offset_y >= g_cam_ui_iconlist.height)
	{
		g_camera_cntx.is_iconlist_moving = FALSE;	

		gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);
		gdi_layer_set_position(0, 0);
		gdi_layer_pop_and_restore_active();
		
		/* move to down, enter normal preview state */
		CAMERA_ENTER_STATE(CAMERA_STATE_PREVIEW);
	}

}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_right_arrow_press()
* DESCRIPTION
*	 right arrow pressed
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_right_arrow_press(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	PU8	blt_buf_ptr;		/* buf used by driver */
	PU8	draw_buf_ptr;
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	g_camera_cntx.is_right_arrow_pressed = TRUE;

	if(g_cam_ui_iconlist.highlighted_index >= g_cam_ui_iconlist.total_icon_count-1)
		return;	
	
	gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);
	
	gdi_layer_get_buffer_ptr(&blt_buf_ptr);
	gdi_layer_toggle_double();
	gdi_layer_get_buffer_ptr(&draw_buf_ptr);

	memcpy(draw_buf_ptr, blt_buf_ptr, CAMERA_OSD_BUFFER_SIZE>>1);

	mmi_camera_osd_menu_draw_iconlist_right_arrow();

	gdi_layer_pop_and_restore_active();
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_right_arrow_release()
* DESCRIPTION
*	 right arrow released
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_right_arrow_release(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	g_camera_cntx.is_right_arrow_pressed = FALSE;	

	if(g_cam_ui_iconlist.highlighted_index >= g_cam_ui_iconlist.total_icon_count-1)
		return;
	
	mmi_camera_osd_menu_iconlist_move_next();	
}

#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_right_arrow_release_no_action()
* DESCRIPTION
*	  arrow released no action, will only draw 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_right_arrow_release_no_action(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	PU8	blt_buf_ptr;		/* buf used by driver */
	PU8	draw_buf_ptr;
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	g_camera_cntx.is_right_arrow_pressed = FALSE;
 	
	if(g_cam_ui_iconlist.highlighted_index >= g_cam_ui_iconlist.total_icon_count-1)
		return;
	
	gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);
	
	gdi_layer_get_buffer_ptr(&blt_buf_ptr);
	gdi_layer_toggle_double();
	gdi_layer_get_buffer_ptr(&draw_buf_ptr);

	memcpy(draw_buf_ptr, blt_buf_ptr, CAMERA_OSD_BUFFER_SIZE>>1);

	mmi_camera_osd_menu_draw_iconlist_right_arrow();

	gdi_layer_pop_and_restore_active();
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_left_arrow_press()
* DESCRIPTION
*	 right arrow pressed
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_left_arrow_press(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	PU8	blt_buf_ptr;		/* buf used by driver */
	PU8	draw_buf_ptr;
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	g_camera_cntx.is_left_arrow_pressed = TRUE;

	if(g_cam_ui_iconlist.highlighted_index == 0)
		return;
	
	gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);
	
	gdi_layer_get_buffer_ptr(&blt_buf_ptr);
	gdi_layer_toggle_double();
	gdi_layer_get_buffer_ptr(&draw_buf_ptr);

	memcpy(draw_buf_ptr, blt_buf_ptr, CAMERA_OSD_BUFFER_SIZE>>1);

	mmi_camera_osd_menu_draw_iconlist_left_arrow();
	
	gdi_layer_pop_and_restore_active();
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_left_arrow_release()
* DESCRIPTION
*	 left arrow released
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_left_arrow_release(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	g_camera_cntx.is_left_arrow_pressed = FALSE;

	if(g_cam_ui_iconlist.highlighted_index == 0)
		return;
	
	mmi_camera_osd_menu_iconlist_move_prev();
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_left_arrow_release_no_action()
* DESCRIPTION
*	 left arrow released no action, will only draw 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_left_arrow_release_no_action(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	PU8	blt_buf_ptr;		/* buf used by driver */
	PU8	draw_buf_ptr;
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	g_camera_cntx.is_left_arrow_pressed = FALSE;

	if(g_cam_ui_iconlist.highlighted_index == 0)
		return;
	
	gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);
	
	gdi_layer_get_buffer_ptr(&blt_buf_ptr);
	gdi_layer_toggle_double();
	gdi_layer_get_buffer_ptr(&draw_buf_ptr);

	memcpy(draw_buf_ptr, blt_buf_ptr, CAMERA_OSD_BUFFER_SIZE>>1);

	mmi_camera_osd_menu_draw_iconlist_left_arrow();
	
	gdi_layer_pop_and_restore_active();
	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_highlight_image_size()
* DESCRIPTION
*  osd menu image size highlight hdlr
* PARAMETERS
*	 index 		IN			selected index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_highlight_image_size(S32 index)
{
   S32 	prev_preview_width;
   S32 	prev_preview_height; 

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
   
	/* change size */
	g_camera_cntx.setting.image_size = index;
	g_camera_cntx.setting.zoom_idx 		= CAMERA_DEFAULT_SETTING_ZOOM_IDX; 	/* restore zoom setting */

	#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
		/* disable burst shot if size cant not support */
		if( (g_camera_cntx.setting.image_size != CAMERA_FEATURE_CONT_SHOT_BURST_IMAGE_IDX) &&
			 (g_camera_cntx.setting.cont_shot == CAMERA_SETTING_CONT_SHOT_BURST) )
		{
			g_camera_cntx.setting.cont_shot = CAMERA_DEFAULT_SETTING_CONT_SHOT;
		}
	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */

	
	/* backup previous */
	prev_preview_width 	= g_camera_cntx.preview_width;
	prev_preview_height	= g_camera_cntx.preview_height;
	
	/* update parameters */	
	mmi_camera_preview_update_capture_size();
	
	if( (prev_preview_width != g_camera_cntx.preview_width) || 
		 (prev_preview_height != g_camera_cntx.preview_height) )
	{
		/* since preview para is changed, stop and restart preview */
		mmi_camera_preview_stop();

		/* update add frame */
		#ifdef __CAMERA_FEATURE_ADD_FRAME__
			mmi_camera_draw_add_frame();
		#endif
 
		/* set blt */
		mmi_camera_preview_set_blt_layer(FALSE);

		/* clear preview buffer background color */ /* can skip this ?? */
		mmi_camera_preview_clear_background();

		/* blt cleard bg */
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

		/* set blt for previewing */
		mmi_camera_preview_set_blt_layer(TRUE);
		
		/* restart */
		mmi_camera_preview_start();

		/* check if preview success */
		if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
			mmi_trace(g_sw_CAMERA,"mmi_camera_osd_menu_highlight_image_size 1");
			mmi_camera_display_error_popup();
		
	}
	else
	{
		mdi_camera_update_para_zoom(mmi_camera_preview_get_zoom_factor());//
		mdi_camera_update_para_image_size(g_camera_cntx.image_width, g_camera_cntx.image_height);
	
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	}
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_highlight_image_qty()
* DESCRIPTION
*  osd menu image qty highlight hdlr
* PARAMETERS
*	 index 		IN			selected index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_highlight_image_qty(S32 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	g_camera_cntx.setting.image_qty = index;

	/* update quality parameter to mdi_camera */
	mdi_camera_update_para_image_qty(camera_image_qty_command_map[g_camera_cntx.setting.image_qty]);
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_highlight_wb()
* DESCRIPTION
*  osd menu wb highlight hdlr
* PARAMETERS
*	 index 		IN			selected index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_highlight_wb(S32 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_camera_cntx.setting.wb = index;

	/* update quality parameter to mdi_camera */
	mdi_camera_update_para_wb(camera_wb_command_map[g_camera_cntx.setting.wb]);
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_highlight_effect()
* DESCRIPTION
*  osd menu effect highlight hdlr
* PARAMETERS
*	 index 		IN			selected index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_highlight_effect(S32 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* PLZ NOTED!!when using OSD Menu, shall turn On "custom FX"  */
   g_camera_cntx.setting.effect = index;
   
	mdi_camera_update_para_effect(camera_effect_command_map[g_camera_cntx.setting.effect]);
	mdi_camera_update_para_brightness(128);
	mdi_camera_update_para_saturation(128);	
	mdi_camera_update_para_contrast(128);	
	mdi_camera_update_para_hue(0);	
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_highlight_night()
* DESCRIPTION
*  osd menu night highlight hdlr
* PARAMETERS
*	 index 		IN			selected index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_highlight_night(S32 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_camera_cntx.setting.night = index;

	/* update quality parameter to mdi_camera */
	mdi_camera_update_para_night(g_camera_cntx.setting.night);
	
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_highlight_add_frame()
* DESCRIPTION
*  osd menu add frame highlight hdlr
* PARAMETERS
*	 index 		IN			selected index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_OSD_MENU__) && defined(__CAMERA_FEATURE_ADD_FRAME__)
static void mmi_camera_osd_menu_highlight_add_frame(S32 index)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_camera_cntx.setting.add_frame = index;

	if(g_camera_cntx.is_fullscreen)
	{
		/* since add frame layer is a single buffer, we have to stop preview and draw frame */
		mmi_camera_preview_stop();

		/* update add frame */
		mmi_camera_draw_add_frame();

		mmi_camera_preview_start();	
		
		if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
			mmi_trace(g_sw_CAMERA,"mmi_camera_osd_menu_highlight_add_frame 1");
			mmi_camera_display_error_popup();		
	}
}
#endif /* __CAMERA_FEATURE_ADD_FRAME__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_highlight_cont_shot()
* DESCRIPTION
*  osd menu cont shot highlight hdlr
* PARAMETERS
*	 index 		IN			selected index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_highlight_cont_shot(S32 index)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	BOOL need_restart_preview;
	
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	need_restart_preview = FALSE;

	#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
		/* Before change is tile shot */
		if(g_camera_cntx.setting.cont_shot == CAMERA_SETTING_CONT_SHOT_TILE)
		{
			need_restart_preview	= TRUE;
		}
	#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */

	
	g_camera_cntx.setting.cont_shot = index;
	
	#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
		/* AFTER change is tile shot */
		if(g_camera_cntx.setting.cont_shot == CAMERA_SETTING_CONT_SHOT_TILE)
		{
			g_camera_cntx.is_tileshot 			= TRUE;
			g_camera_cntx.tile_cur_num 		= 0;
			g_camera_cntx.setting.image_size = CAMERA_FEATURE_CONT_SHOT_TILE_IMAGE_IDX;
			need_restart_preview 				= TRUE;
		}
		else
		{
			g_camera_cntx.is_tileshot 	= FALSE;
		}
			
	#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */

	/* restart preview */
	if(need_restart_preview)
	{
		mmi_camera_preview_update_capture_size();
		
		mmi_camera_preview_stop();

		/* clear backgroud */
		mmi_camera_preview_set_blt_layer(FALSE);
		gdi_layer_clear(GDI_COLOR_BLACK);
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
		mmi_camera_preview_set_blt_layer(TRUE);		
		mmi_camera_preview_start();

		if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
			mmi_trace(g_sw_CAMERA,"mmi_camera_osd_menu_highlight_cont_shot 1");
			mmi_camera_display_error_popup();		
	}
	
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_highlight_delay_timer()
* DESCRIPTION
*  osd menu delay timer highlight hdlr
* PARAMETERS
*	 index 		IN			selected index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_highlight_delay_timer(S32 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_camera_cntx.setting.delay_timer = index;
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_highlight_led_highlight()
* DESCRIPTION
*  osd menu led highlight highlight hdlr
* PARAMETERS
*	 index 		IN			selected index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_highlight_led_highlight(S32 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_camera_cntx.setting.led_highlight = index;
   
  	/* trun on/off led highlight */
	mmi_camera_turn_on_preview_led_highlight();
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_highlight_shutter_sound()
* DESCRIPTION
*  osd menu shutter sound highlight hdlr
* PARAMETERS
*	 index 		IN			selected index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_highlight_shutter_sound(S32 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_camera_cntx.setting.shutter_sound = index;

   coolsand_UI_start_timer(200, mmi_camera_capture_play_sound);
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_highlight_banding()
* DESCRIPTION
*  osd menu banding highlight hdlr
* PARAMETERS
*	 index 		IN			selected index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_highlight_banding(S32 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_camera_cntx.setting.banding = index;

	/* stop and start to make the banding effect works */
	mmi_camera_preview_stop();
	mmi_camera_preview_start();	

	if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
		mmi_trace(g_sw_CAMERA,"mmi_camera_osd_menu_highlight_banding 1");
		mmi_camera_display_error_popup();	

}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_highlight_self_capture()
* DESCRIPTION
*  osd menu self capture highlight hdlr
* PARAMETERS
*	 index 		IN			selected index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_highlight_self_capture(S32 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* 0 = Off, 1 = ON */
	if(index == 1) 
	{
		g_camera_cntx.is_force_self_capture = TRUE;
	}
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_highlight_storage()
* DESCRIPTION
*  osd menu storage highlight hdlr
* PARAMETERS
*	 index 		IN			selected index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_highlight_storage(S32 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* 0 = Phone, 1 = Card */
	g_camera_cntx.is_card_present = TRUE;
   
	if(index == 1) 
	{
		if(!fmgr_is_msdc_present())
		{
			g_camera_cntx.is_card_present	= FALSE;
		}
	}
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
*
*	Camera OSD Menu UI
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_draw_image()
* DESCRIPTION
*   draw image 
* PARAMETERS
*   img_p	IN		ptr to a camera image object
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_draw_image(CAM_UI_IMG *img_p)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* check image is valid */	
	if(img_p->image_id == 0)
		return;

	gdi_image_draw_id(img_p->offset_x, img_p->offset_y, img_p->image_id);
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_draw_image_tile()
* DESCRIPTION
*   draw image 
* PARAMETERS
*   img_tile_p		IN		ptr to image tile object
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_draw_image_tile(CAM_UI_IMG_TILE *img_tile_p)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	S32 	x_loop;
	S32 	y_loop;
	S32 	i;
	S32 	j;
	S32	offset_x;
	S32	offset_y;
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* check image is valid */	
	if(img_tile_p->image_id == 0)
		return;
	
	gdi_layer_push_clip();
	gdi_layer_set_clip(	img_tile_p->tile_offset_x,
								img_tile_p->tile_offset_y, 
								img_tile_p->tile_offset_x+img_tile_p->tile_width-1, 
								img_tile_p->tile_offset_y+img_tile_p->tile_height-1);
		
	/* check if is valid image */
	if(img_tile_p->image_width == 0 || img_tile_p->image_height == 0)
	{
		gdi_layer_pop_clip();
		return;
	}

	x_loop = (img_tile_p->tile_width/img_tile_p->image_width);
	y_loop = (img_tile_p->tile_height/img_tile_p->image_height);

	if(img_tile_p->tile_width%img_tile_p->image_width != 0)
	{
		x_loop++;
	}

	if(img_tile_p->tile_height%img_tile_p->image_height != 0)
	{
		y_loop++;
	}


	offset_y = img_tile_p->tile_offset_y;

	for(i = 0 ; i < y_loop ; i++)
	{
		offset_x = img_tile_p->tile_offset_x;

		for(j = 0; j < x_loop; j++)
		{
			gdi_image_draw_id(offset_x, offset_y, img_tile_p->image_id);
			offset_x += img_tile_p->image_width;
		}
		offset_y += img_tile_p->image_height;
	}

	gdi_layer_pop_clip();

}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_init_poplist()
* DESCRIPTION
*   init a poplist for drawing
* PARAMETERS
*   void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_init_poplist(S32 mark_offset_x)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	CAM_UI_POPLIST 	*pop_p;
	CAM_UI_IMG			*left_img_p;	
	CAM_UI_IMG			*right_img_p;	
	CAM_UI_IMG			*img_p;
	CAM_UI_IMG_TILE	*img_tile_p;
	S32					center_width;
	S32					body_height;
	S32 					img_width;
	S32					img_height;
	S32					i;
	S32					item_count;
	S32					highlighted_index;
	S32					content_region;
	void					(*highlight_hdlr_ptr)(S32);

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	#define CAM_UI_CONFIG_SETTING_ITEM(__NAME__,__name__)\
	{\
		for(i = 0; i < item_count; i++)\
		{\
			g_camera_cntx.list_str_ptr[i] = get_string((U16)(STR_ID_CAMERA_##__NAME__##_START+1+i));\
		}\
		highlighted_index 	= g_camera_cntx.setting.##__name__;\
		highlight_hdlr_ptr	= mmi_camera_osd_menu_highlight_##__name__;\
	}
	
	/* get how many item count */
	switch(g_cam_ui_iconlist.highlighted_index)
	{
		case CAMERA_OSD_MENU_IMAGE_SIZE:
			item_count = CAMERA_IMAGE_SIZE_ITEM_COUNT;
			
			for(i = 0; i < item_count; i++)
			{
				g_camera_cntx.list_str_ptr[i] = get_string(g_camera_cntx.image_size_str_id[i]);
			}

			highlighted_index 	= g_camera_cntx.setting.image_size;
			highlight_hdlr_ptr	= mmi_camera_osd_menu_highlight_image_size;
			
			break;

		case CAMERA_OSD_MENU_IMAGE_QTY:
			item_count = CAMERA_IMAGE_QTY_ITEM_COUNT;
			CAM_UI_CONFIG_SETTING_ITEM(IMAGE_QTY,image_qty);			
			break;
				
	#ifdef __CAMERA_FEATURE_WB__
		case CAMERA_OSD_MENU_WB:
			item_count = CAMERA_WB_ITEM_COUNT;
			CAM_UI_CONFIG_SETTING_ITEM(WB,wb);	
			break;
	#endif 

	#ifdef __CAMERA_FEATURE_EFFECT__
		case CAMERA_OSD_MENU_EFFECT:
			item_count = CAMERA_EFFECT_SETTING_ITEM_COUNT;
			CAM_UI_CONFIG_SETTING_ITEM(EFFECT,effect);
			break;		
	#endif 	

	#ifdef __CAMERA_FEATURE_NIGHT__
		case CAMERA_OSD_MENU_NIGHT:
			item_count = CAMERA_NIGHT_ITEM_COUNT;
			CAM_UI_CONFIG_SETTING_ITEM(NIGHT,night);
			break;		
	#endif 	

	#ifdef __CAMERA_FEATURE_ADD_FRAME__
		case CAMERA_OSD_MENU_ADD_FRAME:
			item_count = CAMERA_ADD_FRAME_ITEM_COUNT;
			CAM_UI_CONFIG_SETTING_ITEM(ADD_FRAME,add_frame);
			break;				
	#endif 	

	#ifdef __CAMERA_FEATURE_CONT_SHOT__
		case CAMERA_OSD_MENU_CONT_SHOT:
			item_count = CAMERA_CONT_SHOT_ITEM_COUNT;
			CAM_UI_CONFIG_SETTING_ITEM(CONT_SHOT,cont_shot);
			break;				
	#endif
	
	#ifdef __CAMERA_FEATURE_DELAY_TIMER__
		case CAMERA_OSD_MENU_DELAY_TIMER:
			item_count = CAMERA_DELAY_TIMER_ITEM_COUNT;
			CAM_UI_CONFIG_SETTING_ITEM(DELAY_TIMER,delay_timer);
			break;				
	#endif 	

	#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT__
		case CAMERA_OSD_MENU_LED_HIGHLIGHT:
			item_count = CAMERA_LCD_HIGHLIGHT_ITEM_COUNT;
			CAM_UI_CONFIG_SETTING_ITEM(LED_HIGHLIGHT,led_highlight);
			break;
	#endif 

	#ifdef __CAMERA_FEATURE_SHUTTER_SOUND__
		case CAMERA_OSD_MENU_SHUTTER_SOUND:
			item_count = CAMERA_SHUTTER_SOUND_ITEM_COUNT;
			CAM_UI_CONFIG_SETTING_ITEM(SHUTTER_SOUND,shutter_sound);
			break;		
	#endif 

	#ifdef __CAMERA_FEATURE_BANDING__
		case CAMERA_OSD_MENU_BANDING:
			item_count = CAMERA_BANDING_ITEM_COUNT;
			CAM_UI_CONFIG_SETTING_ITEM(BANDING, banding);
			break;		
	#endif 

	#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
		case CAMERA_OSD_MENU_SELF_CAPTURE:
			item_count = 2;	/* Off / On */
			g_camera_cntx.list_str_ptr[0] = get_string(STR_GLOBAL_OFF);
			g_camera_cntx.list_str_ptr[1] = get_string(STR_GLOBAL_ON);

			highlight_hdlr_ptr	= mmi_camera_osd_menu_highlight_self_capture;
			highlighted_index 	= 0;
			break;		
	#endif 

	#ifdef __CAMERA_FEATURE_STORAGE__
		case CAMERA_OSD_MENU_STORAGE:
			item_count = 2; /* Phone / Card */
			g_camera_cntx.list_str_ptr[0] = get_string(STR_ID_CAMERA_STORAGE_PHONE);
			g_camera_cntx.list_str_ptr[1] = get_string(STR_ID_CAMERA_STORAGE_MEMORY_CARD);	

			highlight_hdlr_ptr	= mmi_camera_osd_menu_highlight_storage;	
			highlighted_index 	= g_camera_cntx.setting.storage;
			break;		
	#endif 	

		default:
			MMI_ASSERT(0);
			
	}


	pop_p = &g_cam_ui_poplist;

	/* cacl popuplist hieght */
	pop_p->height = item_count*MMI_MENUITEM_HEIGHT;

	if(!g_camera_cntx.is_poplist_init)
		gdi_image_get_dimension_id(IMG_ID_CAMERA_POPLIST_TITLE_LEFT, &img_width, &img_height);
	else
		img_height = pop_p->title.left.height;
	
	pop_p->height += img_height;

	if(!g_camera_cntx.is_poplist_init)
		gdi_image_get_dimension_id(IMG_ID_CAMERA_POPLIST_BOTTOM_LEFT, &img_width, &img_height);	
	else
		img_height = pop_p->bottom.left.height;

	pop_p->height += img_height;


	/* fix it into content region */
	content_region = 	UI_device_height-g_cam_ui_iconlist.height-
							g_camera_osd_menu_cntx.poplist_bottom_margin-g_camera_osd_menu_cntx.poplist_top_margin;
	
	while(pop_p->height > content_region)
	{
		pop_p->height -= MMI_MENUITEM_HEIGHT;
	}

	pop_p->height +=2 ;	/* for spacing between menu list and border */

	pop_p->offset_x	= g_camera_osd_menu_cntx.poplist_left_margin;
	pop_p->offset_y	= g_cam_ui_iconlist.offset_y-g_camera_osd_menu_cntx.poplist_bottom_margin-pop_p->height;
	pop_p->width		= UI_device_width-g_camera_osd_menu_cntx.poplist_left_margin-g_camera_osd_menu_cntx.poplist_right_margin;

	body_height = pop_p->height;

	/************************/
	/*			 title 			*/
	/************************/
	/* use left image to measure height */
	if(!g_camera_cntx.is_poplist_init)
	{
		gdi_image_get_dimension_id(IMG_ID_CAMERA_POPLIST_TITLE_LEFT, &img_width, &img_height);
	}
	else
	{
		img_height 	= pop_p->title.left.height;
		img_width 	= pop_p->title.left.width;
	}
	
	pop_p->title.offset_x 	= pop_p->offset_x;
	pop_p->title.offset_y 	= pop_p->offset_y;	
	pop_p->title.width	 	= pop_p->width;
	pop_p->title.height 		= img_height;	
	
	center_width			 	= pop_p->title.width;	
	body_height 				-= pop_p->title.height;	

		
	/**** left ****/
	left_img_p = &pop_p->title.left;
	
	if(!g_camera_cntx.is_poplist_init)
	{
		left_img_p->image_id = IMG_ID_CAMERA_POPLIST_TITLE_LEFT;		
		gdi_image_get_dimension_id(left_img_p->image_id, &left_img_p->width, &left_img_p->height);
	}
	
	left_img_p->offset_x = pop_p->title.offset_x;
	left_img_p->offset_y = pop_p->title.offset_y;

	center_width -= left_img_p->width;



	/**** right ****/
	right_img_p = &pop_p->title.right;
	
	if(!g_camera_cntx.is_poplist_init)
	{
		right_img_p->image_id = IMG_ID_CAMERA_POPLIST_TITLE_RIGHT;
		gdi_image_get_dimension_id(right_img_p->image_id, &right_img_p->width, &right_img_p->height);
	}
	
	right_img_p->offset_x = pop_p->title.offset_x+pop_p->title.width-right_img_p->width;
	right_img_p->offset_y = pop_p->title.offset_y;

	center_width -= right_img_p->width;
		
	/* center */
	img_tile_p = &pop_p->title.center;

	if(!g_camera_cntx.is_poplist_init)
	{
		img_tile_p->image_id = IMG_ID_CAMERA_POPLIST_TITLE_CENTER;
		gdi_image_get_dimension_id(img_tile_p->image_id, &img_tile_p->image_width, &img_tile_p->image_height);
	}

	img_tile_p->tile_width 		= center_width;
	img_tile_p->tile_height 	= pop_p->title.height;	
	img_tile_p->tile_offset_x	= pop_p->title.offset_x+left_img_p->width;
	img_tile_p->tile_offset_y	= pop_p->title.offset_y;
	

	/************************/
	/*			 bottom 			*/
	/************************/
	/* use left image to measure height */
	if(!g_camera_cntx.is_poplist_init)
	{
		gdi_image_get_dimension_id(IMG_ID_CAMERA_POPLIST_BOTTOM_LEFT, &img_width, &img_height);	
	}
	else
	{
		img_height 	= pop_p->bottom.left.height;
		img_width 	= pop_p->bottom.left.width;
	}
	
	pop_p->bottom.offset_x 	= pop_p->offset_x;
	pop_p->bottom.offset_y 	= pop_p->offset_y+pop_p->height-img_height;
	pop_p->bottom.width	 	= pop_p->width;
	pop_p->bottom.height 	= img_height;	
	
	center_width			 	= pop_p->bottom.width;
	body_height 				-= pop_p->bottom.height;

		
	/**** left ****/
	left_img_p = &pop_p->bottom.left;

	if(!g_camera_cntx.is_poplist_init)
	{
		left_img_p->image_id = IMG_ID_CAMERA_POPLIST_BOTTOM_LEFT;
		gdi_image_get_dimension_id(left_img_p->image_id, &left_img_p->width, &left_img_p->height);
	}
	
	left_img_p->offset_x = pop_p->bottom.offset_x;
	left_img_p->offset_y = pop_p->bottom.offset_y;

	center_width -= left_img_p->width;


	/**** right ****/
	right_img_p = &pop_p->bottom.right;
	
	if(!g_camera_cntx.is_poplist_init)
	{
		right_img_p->image_id = IMG_ID_CAMERA_POPLIST_BOTTOM_RIGHT;
		gdi_image_get_dimension_id(right_img_p->image_id, &right_img_p->width, &right_img_p->height);
	}
	
	right_img_p->offset_x = pop_p->bottom.offset_x+pop_p->bottom.width-right_img_p->width;
	right_img_p->offset_y = pop_p->bottom.offset_y;

	center_width -= right_img_p->width;

		
	/**** center ****/	
	img_tile_p = &pop_p->bottom.center;

	img_tile_p->image_id = IMG_ID_CAMERA_POPLIST_BOTTOM_CENTER;

	gdi_image_get_dimension_id(img_tile_p->image_id, &img_tile_p->image_width, &img_tile_p->image_height);

	img_tile_p->tile_width 		= center_width;
	img_tile_p->tile_height 	= pop_p->bottom.height;	
	img_tile_p->tile_offset_x	= pop_p->bottom.offset_x+left_img_p->width;
	img_tile_p->tile_offset_y	= pop_p->bottom.offset_y;


	 		
	img_p = &pop_p->bottom.chat_mark;
	
	img_p->image_id = IMG_ID_CAMERA_POPLIST_BOTTOM_CHAT_MARK;

	gdi_image_get_dimension_id(img_p->image_id, &img_p->width, &img_p->height);
	
	img_p->offset_x = pop_p->bottom.offset_x+mark_offset_x-(img_p->width>>1);
	img_p->offset_y = pop_p->bottom.offset_y+pop_p->bottom.height-img_p->height;

	/**** up arrow ****/			
	img_p = &pop_p->bottom.up_arrow;
	
	if(!g_camera_cntx.is_poplist_init)
	{
		img_p->image_id = IMG_ID_CAMERA_POPLIST_BOTTOM_U_ARROW;
		gdi_image_get_dimension_id(img_p->image_id, &img_p->width, &img_p->height);
	}
	
	img_p->offset_x = pop_p->bottom.center.tile_offset_x;
	img_p->offset_y = pop_p->bottom.center.tile_offset_y;

	/* selected */
	memcpy(&pop_p->bottom.up_arrow_sel, &pop_p->bottom.up_arrow, sizeof(CAM_UI_IMG));
	pop_p->bottom.up_arrow_sel.image_id = IMG_ID_CAMERA_POPLIST_BOTTOM_U_ARROW_SEL;	

	/* disable */
	memcpy(&pop_p->bottom.up_arrow_dis, &pop_p->bottom.up_arrow, sizeof(CAM_UI_IMG));
	pop_p->bottom.up_arrow_dis.image_id = IMG_ID_CAMERA_POPLIST_BOTTOM_U_ARROW_DIS;	


	/**** down arrow ****/			
	img_p = &pop_p->bottom.down_arrow;
	
	if(!g_camera_cntx.is_poplist_init)
	{
		img_p->image_id = IMG_ID_CAMERA_POPLIST_BOTTOM_D_ARROW;
		gdi_image_get_dimension_id(img_p->image_id, &img_p->width, &img_p->height);
	}
	
	img_p->offset_x = pop_p->bottom.center.tile_offset_x+pop_p->bottom.center.tile_width-img_p->width;
	img_p->offset_y = pop_p->bottom.center.tile_offset_y;

	/* selected */
	memcpy(&pop_p->bottom.down_arrow_sel, &pop_p->bottom.down_arrow, sizeof(CAM_UI_IMG));
	pop_p->bottom.down_arrow_sel.image_id = IMG_ID_CAMERA_POPLIST_BOTTOM_D_ARROW_SEL;	

	/* disable */
	memcpy(&pop_p->bottom.down_arrow_dis, &pop_p->bottom.down_arrow, sizeof(CAM_UI_IMG));
	pop_p->bottom.down_arrow_dis.image_id = IMG_ID_CAMERA_POPLIST_BOTTOM_D_ARROW_DIS;	

	

	/************************/
	/*			 body 			*/
	/************************/
	pop_p->body.offset_x = pop_p->offset_x;
	pop_p->body.offset_y = pop_p->offset_y+pop_p->title.height;
	pop_p->body.width	 	= pop_p->width;
	pop_p->body.height 	= body_height;
	center_width 			= pop_p->body.width;

	/* left */
	img_tile_p = &pop_p->body.left;

	if(!g_camera_cntx.is_poplist_init)
	{
		img_tile_p->image_id = IMG_ID_CAMERA_POPLIST_BODY_LEFT;
		gdi_image_get_dimension_id(img_tile_p->image_id, &img_tile_p->image_width, &img_tile_p->image_height);
	}

	img_tile_p->tile_width 		= img_tile_p->image_width;
	img_tile_p->tile_height 	= pop_p->body.height;	
	img_tile_p->tile_offset_x	= pop_p->body.offset_x;
	img_tile_p->tile_offset_y	= pop_p->body.offset_y;

	
	center_width -= img_tile_p->tile_width;

	/* right */
	img_tile_p = &pop_p->body.right;

	if(!g_camera_cntx.is_poplist_init)
	{
		img_tile_p->image_id = IMG_ID_CAMERA_POPLIST_BODY_RIGHT;
		gdi_image_get_dimension_id(img_tile_p->image_id, &img_tile_p->image_width, &img_tile_p->image_height);
	}

	img_tile_p->tile_width 		= img_tile_p->image_width;
	img_tile_p->tile_height 	= pop_p->body.height;	
	img_tile_p->tile_offset_x	= pop_p->body.offset_x+pop_p->body.width-img_tile_p->image_width;
	img_tile_p->tile_offset_y	= pop_p->body.offset_y;
	
	center_width -= img_tile_p->tile_width;	

	/* center */
	img_tile_p = &pop_p->body.center;

	if(!g_camera_cntx.is_poplist_init)
	{
		img_tile_p->image_id = IMG_ID_CAMERA_POPLIST_BODY_CENTER;
		gdi_image_get_dimension_id(img_tile_p->image_id, &img_tile_p->image_width, &img_tile_p->image_height);
	}

	img_tile_p->tile_width 		= center_width;
	img_tile_p->tile_height 	= pop_p->body.height;	
	img_tile_p->tile_offset_x	= pop_p->body.offset_x+pop_p->body.left.image_width;
	img_tile_p->tile_offset_y	= pop_p->body.offset_y;

	/* list */
	g_cam_ui_poplist.highlighted_index 		= highlighted_index;
	g_cam_ui_poplist.total_item_count	= item_count;
	g_cam_ui_poplist.item_per_page 		= (pop_p->body.center.tile_height-POPLIST_LIST_SPACING*2)/MMI_MENUITEM_HEIGHT;

	if(g_cam_ui_poplist.total_item_count < g_cam_ui_poplist.item_per_page)
	{
		g_cam_ui_poplist.item_per_page = g_cam_ui_poplist.total_item_count;
	}

	if(g_cam_ui_poplist.total_item_count-g_cam_ui_poplist.highlighted_index < g_cam_ui_poplist.item_per_page)
	{
		g_cam_ui_poplist.page_start_index = g_cam_ui_poplist.total_item_count-g_cam_ui_poplist.item_per_page;
	}
	else
	{
		g_cam_ui_poplist.page_start_index = highlighted_index;
	}

	g_cam_ui_poplist.highlight_hdlr = highlight_hdlr_ptr;

}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_draw_poplist()
* DESCRIPTION
*   draw pop list 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_draw_poplist(void)
{
	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_osd_menu_draw_poplist()");
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);
	
	mmi_camera_osd_menu_draw_poplist_title();
	mmi_camera_osd_menu_draw_poplist_body();	
	mmi_camera_osd_menu_draw_poplist_bottom();		

	gdi_layer_pop_and_restore_active();
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_draw_poplist_title()
* DESCRIPTION
*   draw title
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_draw_poplist_title(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	U16 	str_id;
	PS8	str_ptr;
	S32	str_width;
	S32	str_height;
	S32	offset_x;
	S32 	offset_y;

	CAM_UI_TITLE *title_p;

		mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_osd_menu_draw_poplist_title()");
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	title_p = &g_cam_ui_poplist.title;
	
	gdi_layer_push_clip();
	gdi_layer_push_text_clip();
	
	gdi_layer_set_clip(	title_p->offset_x,
								title_p->offset_y, 
								title_p->offset_x+title_p->width-1, 
								title_p->offset_y+title_p->height-1);
	
	gdi_layer_set_text_clip(title_p->offset_x,
									title_p->offset_y, 
									title_p->offset_x+title_p->width-1, 
									title_p->offset_y+title_p->height-1);


	mmi_camera_osd_menu_draw_image(&title_p->left);
	mmi_camera_osd_menu_draw_image(&title_p->right);
	mmi_camera_osd_menu_draw_image_tile(&title_p->center);


	/* get how many item count */
	switch(g_cam_ui_iconlist.highlighted_index)
	{
		case CAMERA_OSD_MENU_IMAGE_SIZE:
			str_id = STR_ID_CAMERA_IMAGE_SIZE;
			break;

		case CAMERA_OSD_MENU_IMAGE_QTY:
			str_id = STR_ID_CAMERA_IMAGE_QTY;		
			break;
				
	#ifdef __CAMERA_FEATURE_WB__
		case CAMERA_OSD_MENU_WB:
			str_id = STR_ID_CAMERA_WB;
			break;
	#endif 

	#ifdef __CAMERA_FEATURE_EFFECT__
		case CAMERA_OSD_MENU_EFFECT:
			str_id = STR_ID_CAMERA_EFFECT_SETTING;
			break;		
	#endif 	

	#ifdef __CAMERA_FEATURE_NIGHT__
		case CAMERA_OSD_MENU_NIGHT:
			str_id = STR_ID_CAMERA_NIGHT;
			break;		
	#endif 	

	#ifdef __CAMERA_FEATURE_ADD_FRAME__
		case CAMERA_OSD_MENU_ADD_FRAME:
			str_id = STR_ID_CAMERA_ADD_FRAME;
			break;				
	#endif 	

	#ifdef __CAMERA_FEATURE_CONT_SHOT__
		case CAMERA_OSD_MENU_CONT_SHOT:
			str_id = STR_ID_CAMERA_CONT_SHOT;
			break;				
	#endif
	
	#ifdef __CAMERA_FEATURE_DELAY_TIMER__
		case CAMERA_OSD_MENU_DELAY_TIMER:
			str_id = STR_ID_CAMERA_DELAY_TIMER;
			break;				
	#endif 	

	#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT__
		case CAMERA_OSD_MENU_LED_HIGHLIGHT:
			str_id = STR_ID_CAMERA_LED_HIGHLIGHT;
			break;
	#endif 

	#ifdef __CAMERA_FEATURE_SHUTTER_SOUND__
		case CAMERA_OSD_MENU_SHUTTER_SOUND:
			str_id = STR_ID_CAMERA_SHUTTER_SOUND;
			break;		
	#endif 

	#ifdef __CAMERA_FEATURE_BANDING__
		case CAMERA_OSD_MENU_BANDING:
			str_id = STR_ID_CAMERA_BANDING;
			break;		
	#endif 

	#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
		case CAMERA_OSD_MENU_SELF_CAPTURE:
			str_id = STR_ID_CAMERA_SELF_CAPTURE;
			break;		
	#endif 

	#ifdef __CAMERA_FEATURE_STORAGE__
		case CAMERA_OSD_MENU_STORAGE:
			str_id = STR_ID_CAMERA_STORAGE;
			break;		
	#endif 	

		default:
			MMI_ASSERT(0);
			
	}

	coolsand_UI_set_font(&MMI_medium_font);

	str_ptr = GetString(str_id);
	coolsand_UI_measure_string((UI_string_type)str_ptr, &str_width, &str_height);

	offset_x = title_p->offset_x+((title_p->width-str_width)>>1);
	offset_y = title_p->offset_y+((title_p->height-str_height)>>1);

	mmi_camera_draw_style_text(str_ptr, offset_x, offset_y, &g_camera_osd_menu_cntx.poplist_title_text);

	gdi_layer_pop_text_clip();
	gdi_layer_pop_clip();
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_draw_poplist_bottom()
* DESCRIPTION
*   draw poplist bottom object
* PARAMETERS
*   void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_draw_poplist_bottom(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	CAM_UI_BOTTOM *bottom_p;
		
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	bottom_p = &g_cam_ui_poplist.bottom;
	
	gdi_layer_push_clip();
	gdi_layer_set_clip(	bottom_p->offset_x,
								bottom_p->offset_y, 
								bottom_p->offset_x+bottom_p->width-1, 
								bottom_p->offset_y+bottom_p->height-1);
	

	mmi_camera_osd_menu_draw_image(&bottom_p->left);
	mmi_camera_osd_menu_draw_image(&bottom_p->right);
	mmi_camera_osd_menu_draw_image_tile(&bottom_p->center);

	if(g_cam_ui_poplist.highlighted_index > 0)
	{
		if(g_camera_cntx.is_up_arrow_pressed)
		{
			mmi_camera_osd_menu_draw_image(&bottom_p->up_arrow_sel);
		}
		else
		{
			mmi_camera_osd_menu_draw_image(&bottom_p->up_arrow);
		}	
	}

	if(g_cam_ui_poplist.highlighted_index < g_cam_ui_poplist.total_item_count-1)
	{
		if(g_camera_cntx.is_down_arrow_pressed)
		{
			mmi_camera_osd_menu_draw_image(&bottom_p->down_arrow_sel);
		}
		else
		{
			mmi_camera_osd_menu_draw_image(&bottom_p->down_arrow);
		}		
	}

	 
	mmi_camera_osd_menu_draw_image(&bottom_p->chat_mark);

	gdi_layer_pop_clip();
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_draw_poplist_body()
* DESCRIPTION
*   draw poplist body object
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_draw_poplist_body(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/	
	CAM_UI_BODY *body_p;
	

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/	
	body_p = &g_cam_ui_poplist.body;
	
	gdi_layer_push_clip();
	gdi_layer_set_clip(	body_p->offset_x,
								body_p->offset_y, 
								body_p->offset_x+body_p->width-1, 
								body_p->offset_y+body_p->height-1);
	

	mmi_camera_osd_menu_draw_image_tile(&body_p->left);
	mmi_camera_osd_menu_draw_image_tile(&body_p->right);
	mmi_camera_osd_menu_draw_image_tile(&body_p->center);

	/* draw list */
	mmi_camera_osd_menu_draw_poplist_body_list();

	gdi_layer_pop_clip();
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_draw_poplist_body_list()
* DESCRIPTION
*   draw poplist body object's list
* PARAMETERS
*   void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_draw_poplist_body_list(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/	
	CAM_UI_BODY *body_p;
	U16			i;
	U16			idx;
	S32			offset_x;
	S32			offset_y;
	S32			str_width;
	S32			str_height;
	S32			str_offset_x;
	S32			str_offset_y;

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/	
	gdi_layer_reset_text_clip();
	gdi_layer_reset_clip();
	
	body_p 	= &g_cam_ui_poplist.body;
	offset_x = body_p->center.tile_offset_x+POPLIST_LIST_SPACING;
	offset_y = body_p->center.tile_offset_y+POPLIST_LIST_SPACING;

	gdi_draw_solid_rect(	body_p->center.tile_offset_x, 
								body_p->center.tile_offset_y,
								body_p->center.tile_offset_x+body_p->center.tile_width-1, 
								body_p->center.tile_offset_y+body_p->center.tile_height-1, 
								g_cam_ui_poplist.bg_color);
	
	coolsand_UI_set_font(&MMI_medium_font);

	for(i = 0 ; i < g_cam_ui_poplist.item_per_page; i++)
	{	
		idx = i+g_cam_ui_poplist.page_start_index;

		coolsand_UI_measure_string(g_camera_cntx.list_str_ptr[idx], &str_width, &str_height);
	
		str_offset_x = offset_x+3;	/* 3 spacing from left margin */
		str_offset_y = offset_y+((MMI_MENUITEM_HEIGHT-str_height)>>1);

		if(g_cam_ui_poplist.highlighted_index == idx)
		{
			/* highlighted block */
			gdi_draw_solid_rect(	offset_x, 
										offset_y,
										offset_x+body_p->center.tile_width-(POPLIST_LIST_SPACING<<1)-1,
										offset_y+MMI_MENUITEM_HEIGHT-1,
										g_cam_ui_poplist.highlight_color);

			mmi_camera_draw_style_text((PS8)g_camera_cntx.list_str_ptr[idx],
												str_offset_x, 
												str_offset_y,
												&g_camera_osd_menu_cntx.poplist_list_highlight_text);
			
		}
		else
		{
			mmi_camera_draw_style_text((PS8)g_camera_cntx.list_str_ptr[idx],
												str_offset_x, 
												str_offset_y,
												&g_camera_osd_menu_cntx.poplist_list_text);
		}

		offset_y += MMI_MENUITEM_HEIGHT;		
	}
	

}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_poplist_up_arrow_press()
* DESCRIPTION
*   poplist up arrow press hdlr
* PARAMETERS
*   void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_poplist_up_arrow_press(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	PU8	blt_buf_ptr;		/* buf is updating by driver */
	PU8	draw_buf_ptr;

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	if(g_cam_ui_poplist.highlighted_index == 0)
		return;
	
	g_camera_cntx.is_up_arrow_pressed = TRUE;
	
	/* toggle and dupe buffer */
	gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);
	
	gdi_layer_get_buffer_ptr(&blt_buf_ptr);
	gdi_layer_toggle_double();
	gdi_layer_get_buffer_ptr(&draw_buf_ptr);
	memcpy(draw_buf_ptr, blt_buf_ptr, CAMERA_OSD_BUFFER_SIZE>>1);

	mmi_camera_osd_menu_draw_poplist_bottom();
	
	gdi_layer_pop_and_restore_active();

	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_poplist_up_arrow_release()
* DESCRIPTION
*   poplist up arrow release hdlr
* PARAMETERS
*   void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_poplist_up_arrow_release(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	PU8	blt_buf_ptr;		/* buf is updating by driver */
	PU8	draw_buf_ptr;
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	if(g_cam_ui_poplist.highlighted_index == 0)
		return;
	
	g_camera_cntx.is_up_arrow_pressed = FALSE;
	
	gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);
	gdi_layer_lock_frame_buffer();
	
	/* toggle and dupe buffer */
	gdi_layer_get_buffer_ptr(&blt_buf_ptr);
	gdi_layer_toggle_double();
	gdi_layer_get_buffer_ptr(&draw_buf_ptr);
	memcpy(draw_buf_ptr, blt_buf_ptr, CAMERA_OSD_BUFFER_SIZE>>1);

	/* move selection to previous */
	
	/* draw list */
	if(g_cam_ui_poplist.highlighted_index > 0)
	{
		g_cam_ui_poplist.highlighted_index--;
	}

	if(g_cam_ui_poplist.page_start_index > g_cam_ui_poplist.highlighted_index)
	{
		g_cam_ui_poplist.page_start_index--;
	}

	mmi_camera_osd_menu_draw_poplist_body_list();

	/* draw buttom */
	mmi_camera_osd_menu_draw_poplist_bottom();


	gdi_layer_unlock_frame_buffer();	
	gdi_layer_pop_and_restore_active();	

	/* call selected item's highlight handler */
	g_cam_ui_poplist.highlight_hdlr(g_cam_ui_poplist.highlighted_index);

	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	

	if(g_camera_cntx.is_force_self_capture)
	{
		g_camera_cntx.is_force_self_capture = FALSE;
		CAMERA_ENTER_STATE(CAMERA_STATE_TOGGLE_TO_SUB);
		return;
	}

	/* storage test */
	if( (g_cam_ui_iconlist.highlighted_index == CAMERA_OSD_MENU_STORAGE) && 
		 (!g_camera_cntx.is_card_present) )
	{
		g_camera_cntx.is_card_present = TRUE;
		
		/* no card */
		/* popup will exit camera app and re-enter. re-enter takes some time, so make the popup shorter */		
		DisplayPopup((PU8)GetString(STR_ID_CAMERA_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION>>1, WARNING_TONE);
	}
	
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_poplist_up_arrow_release_no_action()
* DESCRIPTION
*   poplist up arrow release hdlr, only draw no action
* PARAMETERS
*   void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_poplist_up_arrow_release_no_action(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	PU8	blt_buf_ptr;		/* buf is updating by driver */
	PU8	draw_buf_ptr;

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	if(g_cam_ui_poplist.highlighted_index == 0)
		return;
	
	g_camera_cntx.is_up_arrow_pressed = FALSE;
	
	/* toggle and dupe buffer */
	gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);
	
	gdi_layer_get_buffer_ptr(&blt_buf_ptr);
	gdi_layer_toggle_double();
	gdi_layer_get_buffer_ptr(&draw_buf_ptr);
	memcpy(draw_buf_ptr, blt_buf_ptr, CAMERA_OSD_BUFFER_SIZE>>1);

	mmi_camera_osd_menu_draw_poplist_bottom();
	
	gdi_layer_pop_and_restore_active();

	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_poplist_down_arrow_press()
* DESCRIPTION
*   poplist down arrow press hdlr
* PARAMETERS
*   void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_poplist_down_arrow_press(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	PU8	blt_buf_ptr;		/* buf is updating by driver */
	PU8	draw_buf_ptr;

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	if(g_cam_ui_poplist.highlighted_index == g_cam_ui_poplist.total_item_count-1)
		return;
	
	g_camera_cntx.is_down_arrow_pressed = TRUE;
	
	/* toggle and dupe buffer */
	gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);
	
	gdi_layer_get_buffer_ptr(&blt_buf_ptr);
	gdi_layer_toggle_double();
	gdi_layer_get_buffer_ptr(&draw_buf_ptr);
	memcpy(draw_buf_ptr, blt_buf_ptr, CAMERA_OSD_BUFFER_SIZE>>1);

	mmi_camera_osd_menu_draw_poplist_bottom();
	
	gdi_layer_pop_and_restore_active();

	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */




/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_poplist_down_arrow_release()
* DESCRIPTION
*   poplist down arrow release hdlr
* PARAMETERS
*   void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_poplist_down_arrow_release(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	PU8	blt_buf_ptr;		/* buf used by driver */
	PU8	draw_buf_ptr;
	 
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/	
	if(g_cam_ui_poplist.highlighted_index == g_cam_ui_poplist.total_item_count-1)
		return;
	
	g_camera_cntx.is_down_arrow_pressed 	= FALSE;
	
	gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);
	gdi_layer_lock_frame_buffer();

	/* toggle and dupe buffer */
	gdi_layer_get_buffer_ptr(&blt_buf_ptr);
	gdi_layer_toggle_double();
	gdi_layer_get_buffer_ptr(&draw_buf_ptr);
	memcpy(draw_buf_ptr, blt_buf_ptr, CAMERA_OSD_BUFFER_SIZE>>1);

	/* draw list */
	if(g_cam_ui_poplist.highlighted_index < g_cam_ui_poplist.total_item_count-1)
	{
		g_cam_ui_poplist.highlighted_index++;
	}

	if(g_cam_ui_poplist.highlighted_index - g_cam_ui_poplist.item_per_page >= g_cam_ui_poplist.page_start_index )
	{
		g_cam_ui_poplist.page_start_index++;
	}

	mmi_camera_osd_menu_draw_poplist_body_list();
	
	/* draw buttom */
	mmi_camera_osd_menu_draw_poplist_bottom();

	gdi_layer_unlock_frame_buffer();	
	gdi_layer_pop_and_restore_active();	

	/* call selected item's highlight handler */
	g_cam_ui_poplist.highlight_hdlr(g_cam_ui_poplist.highlighted_index);

	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	
	/* self capture test */
	#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
		if( (g_cam_ui_iconlist.highlighted_index == CAMERA_OSD_MENU_SELF_CAPTURE) && 
			 (g_camera_cntx.is_force_self_capture) )
		{
			g_camera_cntx.is_force_self_capture = FALSE;	
			CAMERA_ENTER_STATE(CAMERA_STATE_TOGGLE_TO_SUB);
			return;
		}
	#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */

	#ifdef __CAMERA_FEATURE_STORAGE__
		/* storage test */
		if( (g_cam_ui_iconlist.highlighted_index == CAMERA_OSD_MENU_STORAGE) && 
			 (!g_camera_cntx.is_card_present) )
		{
			g_camera_cntx.is_card_present = TRUE;
			
			/* no card */
			/* popup will exit camera app and re-enter. re-enter takes some time, so make the popup shorter */			
			DisplayPopup((PU8)GetString(STR_ID_CAMERA_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION>>1, WARNING_TONE);
		}
	#endif /* __CAMERA_FEATURE_STORAGE__ */
	
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_poplist_down_arrow_release_no_action()
* DESCRIPTION
*   poplist down arrow release hdlr, only draw no action
* PARAMETERS
*   void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_poplist_down_arrow_release_no_action(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	PU8	blt_buf_ptr;		/* buf is updating by driver */
	PU8	draw_buf_ptr;

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	if(g_cam_ui_poplist.highlighted_index == g_cam_ui_poplist.total_item_count-1)
		return;
	
	g_camera_cntx.is_down_arrow_pressed = FALSE;
	
	/* toggle and dupe buffer */
	gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);
	
	gdi_layer_get_buffer_ptr(&blt_buf_ptr);
	gdi_layer_toggle_double();
	gdi_layer_get_buffer_ptr(&draw_buf_ptr);
	memcpy(draw_buf_ptr, blt_buf_ptr, CAMERA_OSD_BUFFER_SIZE>>1);

	mmi_camera_osd_menu_draw_poplist_bottom();
	
	gdi_layer_pop_and_restore_active();

	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_poplist_goto_item()
* DESCRIPTION
*   goto index
* PARAMETERS
*   void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_poplist_goto_item(S32 index)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	PU8	blt_buf_ptr;		/* buf is updating by driver */
	PU8	draw_buf_ptr;
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);
	gdi_layer_lock_frame_buffer();
	
	/* toggle and dupe buffer */
	gdi_layer_get_buffer_ptr(&blt_buf_ptr);
	gdi_layer_toggle_double();
	gdi_layer_get_buffer_ptr(&draw_buf_ptr);
	memcpy(draw_buf_ptr, blt_buf_ptr, CAMERA_OSD_BUFFER_SIZE>>1);

	/* move selection */
	g_cam_ui_poplist.highlighted_index = index;

	mmi_camera_osd_menu_draw_poplist_body_list();

	gdi_layer_unlock_frame_buffer();	
	gdi_layer_pop_and_restore_active();	

	/* call selected item's highlight handler */
	g_cam_ui_poplist.highlight_hdlr(g_cam_ui_poplist.highlighted_index);

	gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

	#ifdef __CAMERA_FEATURE_SELF_CAPTURE__	
		if(g_camera_cntx.is_force_self_capture)
		{
			g_camera_cntx.is_force_self_capture = FALSE;
			CAMERA_ENTER_STATE(CAMERA_STATE_TOGGLE_TO_SUB);
			return;
		}
	#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */

	#ifdef __CAMERA_FEATURE_STORAGE__
		/* storage test */
		if( (g_cam_ui_iconlist.highlighted_index == CAMERA_OSD_MENU_STORAGE) && 
			 (!g_camera_cntx.is_card_present) )
		{
			g_camera_cntx.is_card_present = TRUE;
			
			/* no card */
			/* popup will exit camera app and re-enter. re-enter takes some time, so make the popup shorter */
			DisplayPopup((PU8)GetString(STR_ID_CAMERA_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION>>1, WARNING_TONE);
		}
	#endif /* __CAMERA_FEATURE_STORAGE__ */
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */






/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_init_iconlist()
* DESCRIPTION
*   init icon list
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_init_iconlist(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	CAM_UI_IMG	*img_p;
	S32			center_width;
	S32			img_width;
	S32			img_height;
	S32 			index;
	S32			shift;
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	/* check if is already init	*/
	if(g_camera_cntx.is_iconlist_init)
	{
		return;
	}

	/* start to init */
	
	gdi_image_get_dimension_id(IMG_ID_CAMERA_ICONLIST_LEFT_ARROW, &img_width, &img_height);

	g_cam_ui_iconlist.width 	= UI_device_width;
	g_cam_ui_iconlist.height 	= img_height;	
	g_cam_ui_iconlist.offset_x	= 0;	
	g_cam_ui_iconlist.offset_y	= UI_device_height-img_height;		

	center_width = g_cam_ui_iconlist.width;

	/***************************/
	/*			left arrow			*/
	/***************************/

	/* normal */
	img_p = &g_cam_ui_iconlist.img_l_arrow;
	img_p->image_id = IMG_ID_CAMERA_ICONLIST_LEFT_ARROW;

	gdi_image_get_dimension_id(img_p->image_id, &img_p->width, &img_p->height);
	
	img_p->offset_x = g_cam_ui_iconlist.offset_x;
	img_p->offset_y = g_cam_ui_iconlist.offset_y;

	center_width -= img_p->width;
	shift			 =	img_p->width;

	/* selected */
	memcpy(&g_cam_ui_iconlist.img_l_arrow_sel, &g_cam_ui_iconlist.img_l_arrow, sizeof(CAM_UI_IMG));
	g_cam_ui_iconlist.img_l_arrow_sel.image_id = IMG_ID_CAMERA_ICONLIST_LEFT_ARROW_SEL;	
	
	/* disable */
	memcpy(&g_cam_ui_iconlist.img_l_arrow_dis, &g_cam_ui_iconlist.img_l_arrow, sizeof(CAM_UI_IMG));
	g_cam_ui_iconlist.img_l_arrow_dis.image_id = IMG_ID_CAMERA_ICONLIST_LEFT_ARROW_DIS;	
		

	/***************************/
	/*			right arrow			*/
	/***************************/

	/* normal */
	img_p = &g_cam_ui_iconlist.img_r_arrow;
	img_p->image_id = IMG_ID_CAMERA_ICONLIST_RIGHT_ARROW;

	gdi_image_get_dimension_id(img_p->image_id, &img_p->width, &img_p->height);
	
	img_p->offset_x = g_cam_ui_iconlist.offset_x+g_cam_ui_iconlist.width-img_p->width;
	img_p->offset_y = g_cam_ui_iconlist.offset_y;

	center_width -= img_p->width;

	/* selected */
	memcpy(&g_cam_ui_iconlist.img_r_arrow_sel, &g_cam_ui_iconlist.img_r_arrow, sizeof(CAM_UI_IMG));
	g_cam_ui_iconlist.img_r_arrow_sel.image_id = IMG_ID_CAMERA_ICONLIST_RIGHT_ARROW_SEL;	

	/* disable */
	memcpy(&g_cam_ui_iconlist.img_r_arrow_dis, &g_cam_ui_iconlist.img_r_arrow, sizeof(CAM_UI_IMG));
	g_cam_ui_iconlist.img_r_arrow_dis.image_id = IMG_ID_CAMERA_ICONLIST_RIGHT_ARROW_DIS;		


	/***************************/
	/*			hide tap				*/
	/***************************/

	/* normal */
	img_p = &g_cam_ui_iconlist.img_hide_tap;
	img_p->image_id = IMG_ID_CAMERA_ICONLIST_HIDE;

	gdi_image_get_dimension_id(img_p->image_id, &img_p->width, &img_p->height);
	
	img_p->offset_x = g_cam_ui_iconlist.offset_x;
	img_p->offset_y = g_cam_ui_iconlist.offset_y-img_p->height;


	/***************************/
	/*			center icons		*/
	/***************************/	
	index = 0;
 
	#define CAMERA_OSD_INIT_ICON(__NAME__)\
	{\
		img_p = &g_cam_ui_iconlist.img_icon[index];\
		img_p->image_id = IMG_ID_CAMERA_ICONLIST_##__NAME__##;\
		gdi_image_get_dimension_id(img_p->image_id, &img_p->width, &img_p->height);\
		img_p->offset_x = shift;\
		img_p->offset_y = g_cam_ui_iconlist.offset_y;\
		memcpy(&g_cam_ui_iconlist.img_icon_sel[index], &g_cam_ui_iconlist.img_icon[index], sizeof(CAM_UI_IMG));\
		g_cam_ui_iconlist.img_icon_sel[index].image_id = IMG_ID_CAMERA_ICONLIST_##__NAME__##_SEL;\
		g_cam_ui_iconlist.is_image_icon_update[index] = TRUE;\
		index++;\
		shift+=img_p->width;\
	}

	CAMERA_OSD_INIT_ICON(IMAGE_SIZE);
	CAMERA_OSD_INIT_ICON(IMAGE_QTY);	
			
	#ifdef __CAMERA_FEATURE_WB__
		CAMERA_OSD_INIT_ICON(WB);
	#endif 

	#ifdef __CAMERA_FEATURE_EFFECT__
		CAMERA_OSD_INIT_ICON(EFFECT);		
	#endif 	

	#ifdef __CAMERA_FEATURE_NIGHT__
		CAMERA_OSD_INIT_ICON(NIGHT);			
	#endif 	

	#ifdef __CAMERA_FEATURE_ADD_FRAME__
		CAMERA_OSD_INIT_ICON(ADD_FRAME);		
	#endif 	

	#ifdef __CAMERA_FEATURE_CONT_SHOT__
		CAMERA_OSD_INIT_ICON(CONT_SHOT);			
	#endif
	
	#ifdef __CAMERA_FEATURE_DELAY_TIMER__
		CAMERA_OSD_INIT_ICON(DELAY_TIMER);		
	#endif 	

	#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT__
		CAMERA_OSD_INIT_ICON(LED_HIGHLIGHT);	
	#endif 

	#ifdef __CAMERA_FEATURE_SHUTTER_SOUND__
		CAMERA_OSD_INIT_ICON(SHUTTER_SOUND);		
	#endif 

	#ifdef __CAMERA_FEATURE_BANDING__
		CAMERA_OSD_INIT_ICON(BANDING);		
	#endif 

	#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
		CAMERA_OSD_INIT_ICON(SELF_CAPTURE);	
	#endif 

	#ifdef __CAMERA_FEATURE_STORAGE__
		CAMERA_OSD_INIT_ICON(STORAGE);		
	#endif 	

	/* check if out of bound */
	MMI_ASSERT(index < 15);

	g_cam_ui_iconlist.total_icon_count 	= index;
	g_cam_ui_iconlist.icon_per_page 		= center_width/img_p->width;
	g_cam_ui_iconlist.highlighted_index = 0;
	g_cam_ui_iconlist.page_start_index 	= 0;	

	/* set is inited flag */
	g_camera_cntx.is_iconlist_init = TRUE;
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_draw_iconlist()
* DESCRIPTION
*   draw icon list
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_draw_iconlist(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	S32 i;
	S32 draw_index;

	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_osd_menu_draw_iconlist()");
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/	

	gdi_layer_push_and_set_active(g_camera_cntx.osd_layer_handle);

	/* arrow */
	mmi_camera_osd_menu_draw_iconlist_left_arrow();
	mmi_camera_osd_menu_draw_iconlist_right_arrow();	
	
	/* icons */
	for(i = 0; i < g_cam_ui_iconlist.icon_per_page; i++)
	{
		draw_index = i + g_cam_ui_iconlist.page_start_index;

		/* if icon need update */
		if(g_cam_ui_iconlist.is_image_icon_update[draw_index])
		{
			if(draw_index == g_cam_ui_iconlist.highlighted_index)
			{
				mmi_camera_osd_menu_draw_image(&g_cam_ui_iconlist.img_icon_sel[draw_index]);
			}
			else
			{
				mmi_camera_osd_menu_draw_image(&g_cam_ui_iconlist.img_icon[draw_index]);
			}
		}
	}


	#ifdef __CAMERA_FEATURE_TOUCH_SCREEN__
		/* draw hide tap */
		mmi_camera_osd_menu_draw_image(&g_cam_ui_iconlist.img_hide_tap);
	#endif /* __CAMERA_FEATURE_TOUCH_SCREEN__ */
	
	
	gdi_layer_pop_and_restore_active();
	
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_draw_iconlist_left_arrow()
* DESCRIPTION
*   draw icon list left arrow
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_draw_iconlist_left_arrow(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/	
	
	/* left arrow */
	if(g_camera_cntx.is_left_arrow_pressed)
	{
		mmi_camera_osd_menu_draw_image(&g_cam_ui_iconlist.img_l_arrow_sel);
	}
	else
	{
		if(g_cam_ui_iconlist.highlighted_index != 0)
		{
			mmi_camera_osd_menu_draw_image(&g_cam_ui_iconlist.img_l_arrow);
		}
		else
		{
			mmi_camera_osd_menu_draw_image(&g_cam_ui_iconlist.img_l_arrow_dis);
		}
	}
}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_draw_iconlist_right_arrow()
* DESCRIPTION
*   draw icon list right arrow
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__
static void mmi_camera_osd_menu_draw_iconlist_right_arrow(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/	
	
	/* right arrow */
	if(g_camera_cntx.is_right_arrow_pressed)
	{
		mmi_camera_osd_menu_draw_image(&g_cam_ui_iconlist.img_r_arrow_sel);
	}
	else
	{
		if(g_cam_ui_iconlist.highlighted_index != g_cam_ui_iconlist.total_icon_count-1)
		{
			mmi_camera_osd_menu_draw_image(&g_cam_ui_iconlist.img_r_arrow);
		}
		else
		{
			mmi_camera_osd_menu_draw_image(&g_cam_ui_iconlist.img_r_arrow_dis);
		}
	}

}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_iconlist_move_next()
* DESCRIPTION
*   icon list move next
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__	
static void mmi_camera_osd_menu_iconlist_move_next(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	S32	i;
	BOOL	need_redraw;
	S32	offset_x;
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/	
	need_redraw = FALSE;

	if(g_cam_ui_iconlist.highlighted_index+1 < 
		g_cam_ui_iconlist.page_start_index+g_cam_ui_iconlist.icon_per_page)
	{
		/* still within this page */
		g_cam_ui_iconlist.is_image_icon_update[g_cam_ui_iconlist.highlighted_index] = TRUE;		
		g_cam_ui_iconlist.highlighted_index++;
		g_cam_ui_iconlist.is_image_icon_update[g_cam_ui_iconlist.highlighted_index] = TRUE;

		need_redraw = TRUE;
	}
	else
	{
		if(g_cam_ui_iconlist.highlighted_index+1 <= g_cam_ui_iconlist.total_icon_count-1)
		{
			g_cam_ui_iconlist.highlighted_index++;	
			g_cam_ui_iconlist.page_start_index++;

			/* reset all icons' pos */
			for(i = 0; i < g_cam_ui_iconlist.total_icon_count; i++)
			{
				g_cam_ui_iconlist.img_icon[i].offset_x 		-= g_cam_ui_iconlist.img_icon[i].width;
				g_cam_ui_iconlist.img_icon_sel[i].offset_x 	-= g_cam_ui_iconlist.img_icon_sel[i].width;
				g_cam_ui_iconlist.is_image_icon_update[i] 	= TRUE;
			}			

			need_redraw = TRUE;
		}
	}


	if(need_redraw)
	{
		i = g_cam_ui_iconlist.highlighted_index;
		
		offset_x = g_cam_ui_iconlist.img_icon[i].offset_x+(g_cam_ui_iconlist.img_icon[i].width>>1);
		offset_x -= g_cam_ui_poplist.offset_x;
			
		mmi_camera_osd_menu_init_poplist(offset_x);
		
		mmi_camera_osd_menu_draw_osd();
		
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	}

}
#endif /* __CAMERA_FEATURE_OSD_MENU__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_iconlist_move_prev()
* DESCRIPTION
*   icon list move previous
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_OSD_MENU__	
static void mmi_camera_osd_menu_iconlist_move_prev(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	S32	i;
	BOOL	need_redraw;
	S32	offset_x;
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/	
	need_redraw = FALSE;
	
	if(g_cam_ui_iconlist.highlighted_index-1 >= g_cam_ui_iconlist.page_start_index)
	{
		/* still within this page */
		g_cam_ui_iconlist.is_image_icon_update[g_cam_ui_iconlist.highlighted_index] = TRUE;		
		g_cam_ui_iconlist.highlighted_index--;
		g_cam_ui_iconlist.is_image_icon_update[g_cam_ui_iconlist.highlighted_index] = TRUE;
		need_redraw = TRUE;		
	}
	else
	{
		if(g_cam_ui_iconlist.highlighted_index-1 >= 0)
		{
			g_cam_ui_iconlist.highlighted_index--;	
			g_cam_ui_iconlist.page_start_index--;

			/* reset all icons' pos */
			for(i = 0; i < g_cam_ui_iconlist.total_icon_count; i++)
			{
				g_cam_ui_iconlist.img_icon[i].offset_x 		+= g_cam_ui_iconlist.img_icon[i].width;
				g_cam_ui_iconlist.img_icon_sel[i].offset_x 	+= g_cam_ui_iconlist.img_icon_sel[i].width;
				g_cam_ui_iconlist.is_image_icon_update[i] 	= TRUE;		
			}			
			
			need_redraw = TRUE;
		}
	}

	if(need_redraw)
	{
		i = g_cam_ui_iconlist.highlighted_index;
		
		offset_x = g_cam_ui_iconlist.img_icon[i].offset_x+(g_cam_ui_iconlist.img_icon[i].width>>1);
		offset_x -= g_cam_ui_poplist.offset_x;
		
		mmi_camera_osd_menu_init_poplist(offset_x);
		
		mmi_camera_osd_menu_draw_osd();
		
		gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
	}	
}
	
#endif /* __CAMERA_FEATURE_OSD_MENU__ */




/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_touch_scr_pen_down()
* DESCRIPTION
*   touch screen down hdlr for osd menu state 
* PARAMETERS
*	 x				IN				position x
*	 y				IN				position y
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_TOUCH_SCREEN__) && defined(__CAMERA_FEATURE_OSD_MENU__)
static void mmi_camera_osd_menu_touch_scr_pen_down(S32 x, S32 y)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	CAM_UI_IMG	*img_p;
	S32			i;
	S32 			index;
	S32			offset_x;
	S32			offset_y;
	CAM_UI_BODY *body_p;	
	S32			x1, y1, x2, y2;
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	g_camera_cntx.is_touch_pen_down 	= TRUE;
	g_camera_cntx.last_touch_img_ptr	= NULL;
	
	/* not moving, means poplist is show */
	if(!g_camera_cntx.is_iconlist_moving)
	{
		/***************************/
		/* hide tap			 			*/
		/***************************/
		
		img_p = &g_cam_ui_iconlist.img_hide_tap;
		
		if((x >= img_p->offset_x) &&
			(x < 	img_p->offset_x + img_p->width) &&
			(y >= img_p->offset_y) &&
			(y < 	img_p->offset_y + img_p->height))
		{
			/* call lsk - move out function */
			mmi_camera_osd_menu_lsk_press();

			g_camera_cntx.last_touch_img_ptr = img_p;				
			return;						
		}
		

		/***************************/
		/* icon list test 			*/
		/***************************/
		
		/* left_arrow */
		img_p = &g_cam_ui_iconlist.img_l_arrow;
		
		if((x >= img_p->offset_x) &&
			(x < 	img_p->offset_x + img_p->width) &&
			(y >= img_p->offset_y) &&
			(y < 	img_p->offset_y + img_p->height))
		{
			mmi_camera_osd_menu_left_arrow_press();
			
			g_camera_cntx.last_touch_img_ptr = img_p;				
			return;						
		}

		/* right */
		img_p = &g_cam_ui_iconlist.img_r_arrow;
		
		if((x >= img_p->offset_x) &&
			(x < 	img_p->offset_x + img_p->width) &&
			(y >= img_p->offset_y) &&
			(y < 	img_p->offset_y + img_p->height))
		{
			mmi_camera_osd_menu_right_arrow_press();
			
			g_camera_cntx.last_touch_img_ptr = img_p;		
			return;			
		}

		/* icons */
		for(i = 0 ; i < g_cam_ui_iconlist.icon_per_page; i++)
		{
			index = i+g_cam_ui_iconlist.page_start_index;

			img_p = &g_cam_ui_iconlist.img_icon[index];

			if((x >= img_p->offset_x) &&
				(x < 	img_p->offset_x + img_p->width) &&
				(y >= img_p->offset_y) &&
				(y < 	img_p->offset_y + img_p->height))
			{
				if(g_cam_ui_iconlist.highlighted_index != index)
				{
					g_cam_ui_iconlist.highlighted_index = index;

					offset_x = g_cam_ui_iconlist.img_icon[index].offset_x+(g_cam_ui_iconlist.img_icon[index].width>>1);
					offset_x -= g_cam_ui_poplist.offset_x;
					
					mmi_camera_osd_menu_init_poplist(offset_x);
					
					mmi_camera_osd_menu_draw_osd();
					gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

					g_camera_cntx.last_touch_img_ptr = img_p;
				}
				return;
			}			
			
		}


		/***************************/
		/* pop list test 				*/
		/***************************/
		
		/* up arrow */
		img_p = &g_cam_ui_poplist.bottom.up_arrow;
		
		if((x >= img_p->offset_x) &&
			(x < 	img_p->offset_x + img_p->width) &&
			(y >= img_p->offset_y) &&
			(y < 	img_p->offset_y + img_p->height))
		{
			mmi_camera_osd_menu_poplist_up_arrow_press();
			
			g_camera_cntx.last_touch_img_ptr = img_p;			
			return;						
		}

		/* down arrow  */
		img_p = &g_cam_ui_poplist.bottom.down_arrow;
		
		if((x >= img_p->offset_x) &&
			(x < 	img_p->offset_x + img_p->width) &&
			(y >= img_p->offset_y) &&
			(y < 	img_p->offset_y + img_p->height))
		{
			mmi_camera_osd_menu_poplist_down_arrow_press();

			g_camera_cntx.last_touch_img_ptr = img_p;			
			return;			
		}		

		/* list item */

		body_p 	= &g_cam_ui_poplist.body;
		offset_x = body_p->center.tile_offset_x+POPLIST_LIST_SPACING;
		offset_y = body_p->center.tile_offset_y+POPLIST_LIST_SPACING;

		for(i = 0 ; i < g_cam_ui_poplist.item_per_page; i++)
		{	
			index = i+g_cam_ui_poplist.page_start_index;

			x1 = offset_x;
			x2 = offset_x+body_p->center.tile_width-(POPLIST_LIST_SPACING<<1)-1;
			y1 = offset_y;
			y2 = offset_y+MMI_MENUITEM_HEIGHT-1;

			if((x >= x1) && (x <= x2) && (y >= y1) && (y <= y2))
			{
				if(index != g_cam_ui_poplist.highlighted_index)
				{
					mmi_camera_osd_menu_poplist_goto_item(index);
				}
				return;
			}

			offset_y += MMI_MENUITEM_HEIGHT;		
		}
		
	}

}
#endif /* __CAMERA_FEATURE_TOUCH_SCREEN__ && __CAMERA_FEATURE_OSD_MENU__ */




/*****************************************************************************
* FUNCTION
*	 mmi_camera_osd_menu_touch_scr_pen_up()
* DESCRIPTION
*   touch screen down hdlr for osd menu state 
* PARAMETERS
*	 x				IN				position x
*	 y				IN				position y
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_TOUCH_SCREEN__) && defined(__CAMERA_FEATURE_OSD_MENU__)
static void mmi_camera_osd_menu_touch_scr_pen_up(S32 x, S32 y)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	CAM_UI_IMG	*img_p;
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	g_camera_cntx.is_touch_pen_down = FALSE;

	/* not moving, means poplist is show */
	if(!g_camera_cntx.is_iconlist_moving)
	{
		/***************************/
		/* icon list test 			*/
		/***************************/
		
		/* left_arrow */
		img_p = &g_cam_ui_iconlist.img_l_arrow;

		if(g_camera_cntx.last_touch_img_ptr == img_p)
		{
			if((x >= img_p->offset_x) &&
				(x < 	img_p->offset_x + img_p->width) &&
				(y >= img_p->offset_y) &&
				(y < 	img_p->offset_y + img_p->height))
			{
				mmi_camera_osd_menu_left_arrow_release();
			}
			else
			{
				mmi_camera_osd_menu_left_arrow_release_no_action();
			}

			return;
		}

		/* right_arrow */
		img_p = &g_cam_ui_iconlist.img_r_arrow;

		if(g_camera_cntx.last_touch_img_ptr == img_p)
		{
			if((x >= img_p->offset_x) &&
				(x < 	img_p->offset_x + img_p->width) &&
				(y >= img_p->offset_y) &&
				(y < 	img_p->offset_y + img_p->height))
			{
				mmi_camera_osd_menu_right_arrow_release();
			}
			else
			{
				mmi_camera_osd_menu_right_arrow_release_no_action();
			}

			return;
		}


		/***************************/
		/* pop list test 				*/
		/***************************/
		
		/* up arrow */
		img_p = &g_cam_ui_poplist.bottom.up_arrow;
		
		if(g_camera_cntx.last_touch_img_ptr == img_p)
		{
			if((x >= img_p->offset_x) &&
				(x < 	img_p->offset_x + img_p->width) &&
				(y >= img_p->offset_y) &&
				(y < 	img_p->offset_y + img_p->height))
			{
				mmi_camera_osd_menu_poplist_up_arrow_release();
			}
			else
			{
				mmi_camera_osd_menu_poplist_up_arrow_release_no_action();
			}

			return;
		}

		/* down arrow */
		img_p = &g_cam_ui_poplist.bottom.down_arrow;
		
		if(g_camera_cntx.last_touch_img_ptr == img_p)
		{
			if((x >= img_p->offset_x) &&
				(x < 	img_p->offset_x + img_p->width) &&
				(y >= img_p->offset_y) &&
				(y < 	img_p->offset_y + img_p->height))
			{
				mmi_camera_osd_menu_poplist_down_arrow_release();
			}
			else
			{
				mmi_camera_osd_menu_poplist_down_arrow_release_no_action();
			}

			return;
		}
	}

}
#endif /* __CAMERA_FEATURE_TOUCH_SCREEN__ && __CAMERA_FEATURE_OSD_MENU__ */





/***************************************************************************
*
*	Mainlcd Capture 
*
***************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_capture()
* DESCRIPTION
*	 capture photo
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
static void mmi_camera_capture(void)
{
    mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_capture()");
	
    /*----------------------------------------------------------------*/
    /* Code Body																		*/
    /*----------------------------------------------------------------*/
    switch(g_camera_cntx.setting.cont_shot)
    {
        case CAMERA_SETTING_CONT_SHOT_OFF:
        {
            mmi_trace(g_sw_CAMERA, "Excute funs: mmi_camera_capture() is CAMERA_SETTING_CONT_SHOT_OFF");
            /* capture button pressed */
            g_camera_cntx.is_capturing = TRUE;

            mmi_camera_turn_on_capture_led_highlight();

            mmi_camera_preview_set_hint((PS8)GetString(STR_ID_CAMERA_NOFIFY_CAPTURING), NULL, 0);
            mmi_camera_preview_draw_osd();

            /* call mdi to capture and save */
            if(g_camera_cntx.setting.add_frame == CAMERA_SETTING_ADD_FRAME_OFF)
            {
                switch(g_camera_cntx.capture_path)
                {
                    case CAPTURE_FROM_MEMORY:
                        g_camera_cntx.last_error
                                = mdi_camera_capture_to_memory( &g_camera_cntx.capture_buf_ptr,
                                                                                    &g_camera_cntx.capture_size);
                        break;
#if defined(__CAMERA_OSD_HORIZONTAL__) && defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
                    case CAPTURE_FROM_LAYER: /* capture layer to memory */
                        g_camera_cntx.last_error 
                                = mdi_camera_capture_layer_to_memory(	
 #if (defined(MT6227)||defined(MT6226)||defined(MT6226M)||defined(MT6228)||defined(MT6229))
                                                            GDI_LAYER_ENABLE_LAYER_0, 
#else
                                                            GDI_LAYER_ENABLE_LAYER_1,
#endif
                                                            &g_camera_cntx.capture_buf_ptr,
                                                            &g_camera_cntx.capture_size,
                                                            0,
                                                            0,
                                                            g_camera_cntx.osd_UI_device_width,
                                                            g_camera_cntx.osd_UI_device_height);
                        break;
#endif
                    default:
                        g_camera_cntx.last_error
                                = mdi_camera_capture_to_memory(&g_camera_cntx.capture_buf_ptr,
                                                            &g_camera_cntx.capture_size);
                        break;
                }

                //gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);

            }
            else
            {
                switch(g_camera_cntx.capture_path)
                {
                    case CAPTURE_FROM_MEMORY:
                        g_camera_cntx.last_error
                                = mdi_camera_capture_to_memory(&g_camera_cntx.capture_buf_ptr,
                                                &g_camera_cntx.capture_size);
                        break;
#if 0				
                    case CAPTURE_FROM_LAYER:
                        gdi_layer_set_blt_layer(g_camera_cntx.preview_layer_handle,
                                    g_camera_cntx.osd_layer_handle,
                                    g_camera_cntx.add_frame_layer_handle,
                                    0);

#if defined(CAMERA_MODULE)
                        /* (0) Base-as preview buffer, (1) Add frame layer */
                        g_camera_cntx.last_error 
                                = mdi_camera_capture_layer_to_memory(GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_2, 
                                                &g_camera_cntx.capture_buf_ptr,
                                                &g_camera_cntx.capture_size,
                                                0,
                                                0,
                                                UI_device_width,
                                                UI_device_height);
#else /* ISP */
                        /* (0) Base-as preview buffer, (2) Add frame layer */
                        g_camera_cntx.last_error 
                                = mdi_camera_capture_layer_to_memory(GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_2, 
                                                &g_camera_cntx.capture_buf_ptr,
                                                &g_camera_cntx.capture_size,
                                                0,
                                                0,
                                                g_camera_cntx.osd_UI_device_width,
                                                g_camera_cntx.osd_UI_device_height);
#endif 
                        break;
#endif					
                    default:
                        if(g_camera_cntx.preview_width==LCD_WIDTH&& g_camera_cntx.preview_height==LCD_HEIGHT)
                        {
                            mmi_trace(g_sw_CAMERA, "have frame blt_layer");
                            gdi_layer_set_blt_layer(g_camera_cntx.preview_layer_handle,
                                                    g_camera_cntx.add_frame_layer_handle,
                                                    0,
                                                    0);
                            gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);//2007-7-6 fengzb
                        }
//2007-7-6 fengzb add for frame capture
                        g_camera_cntx.last_error
                                = mdi_camera_capture_to_memory(&g_camera_cntx.capture_buf_ptr,
                                                    &g_camera_cntx.capture_size);
                        break;
                    }
                }

                /* play capture sound after capture */
                mmi_camera_capture_play_sound(); 
                /*Jinzh Add Start For 6225 Ver: TBM780  on 2007-8-14 10:23 */
#ifdef __FF_AUDIO_SET__
                cameraAppOnOff = FALSE;
#endif
                /*Jinzh Add End  For 6225 Ver: TBM780  on 2007-8-14 10:23 */
                g_camera_cntx.is_capturing = FALSE;

                /* restore background */
#ifdef CAMERA_MODULE_WITH_LCD
                if(!g_camera_cntx.is_fullscreen)
                    mmi_camera_preview_clear_background();
#endif /* CAMERA_MODULE_WITH_LCD */	

                /* will enter save confirm, close led highlight */
                mmi_camera_turn_off_led_highlight();

                /* captured, previewing is stopped, change blt layer */
                mmi_camera_preview_set_blt_layer(FALSE);
			
                if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
                {
                    mmi_trace(g_sw_CAMERA,"mmi_camera_capture 1");
                    mmi_camera_display_error_popup();
                    return;
                }
                else
                {
                    CAMERA_ENTER_STATE(CAMERA_STATE_SAVE_CONFIRM);
                }
            }
            break;

        case CAMERA_SETTING_CONT_SHOT_1:
#ifdef __CAMERA_FEATURE_CONT_SHOT_2__
        case CAMERA_SETTING_CONT_SHOT_2: 
#endif
#ifdef __CAMERA_FEATURE_CONT_SHOT_3__
        case CAMERA_SETTING_CONT_SHOT_3:
#endif
        {
            CAMERA_ENTER_STATE(CAMERA_STATE_CONTSHOT);
            return;
        }
        break;

#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
        case CAMERA_SETTING_CONT_SHOT_BURST:
        {
            CAMERA_ENTER_STATE(CAMERA_STATE_BURSTSHOT);
            return;
        }
        break;
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */

#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
        case CAMERA_SETTING_CONT_SHOT_TILE:
        {
            mmi_camera_preview_set_hint((PS8)GetString(STR_ID_CAMERA_NOFIFY_CAPTURING), NULL, 0);
            mmi_camera_preview_draw_osd();
            gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
            mmi_camera_tileshot_capture();
            return;
        }
        break;
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */
	
        default:
            /* restore default value */
            mmi_camera_restore_setting();
            break;		
    }
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_capture_button_press()
* DESCRIPTION
*   capture button presse
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_capture_button_press(void)
{
#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

#ifndef COOLSAND_MULTIMEDIA_USE_FLASHFS	
	S32 ret_drv =0;
	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_capture_button_press()");

// Beginning--modified by kecx for fixing bug 14537 on 20090928
        coolsand_UI_cancel_timer(mmi_camera_preview_wait_key7_hdlr);  
        ClearKeyHandler(CAMERA_IMAGE_SIZE_KEY, KEY_EVENT_DOWN);
// end--modified by kecx for fixing bug 14537 on 20090928

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* capture only works if camera is ready, otherwise will ignore this action */
//add by WeiD for no memory card can't capture
	ret_drv 	= MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE);
	mmi_trace(g_sw_CAMERA, "Excute func : mmi_camera_capture_button_press(),ret_drv is %d",ret_drv);
    if(ret_drv>=0)
    {
    }
	else
	{
		DisplayPopup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, CAMERA_WARNING_POPUPDURATION, ST_NOTIFYDURATION, WARNING_TONE);
		return;
	}
//add by WeiD   
#endif

// Beginning--added by kecx for fixing bug 13248 on 20090925
    g_camera_cntx.is_capturing = TRUE;
    mmi_camera_preview_draw_osd();
    gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
 //   COS_Sleep(100);
// end--added by kecx for fixing bug 13248 on 20090925

	if(mdi_camera_is_ready_to_capture() == TRUE)
	{
		mmi_trace(g_sw_CAMERA, "capture_btn_press g_camera_cntx.setting.delay_timer = %ld",g_camera_cntx.setting.delay_timer);
		/* if not self timer, capture the image */
		if(g_camera_cntx.setting.delay_timer == CAMERA_SETTING_DELAY_TIMER_OFF)
		{
			mmi_camera_capture();
		}
		else
		{
			CAMERA_ENTER_STATE(CAMERA_STATE_COUNTDOWN);
		}
	}
	/* flush key event */
	ClearKeyEvents();

}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_capture_button_release()
* DESCRIPTION
*   capture button release
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_capture_button_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* flush all key event */
  #ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

	ClearKeyEvents();
}



/*****************************************************************************
* FUNCTION
*	 mmi_camera_capture_play_sound()
* DESCRIPTION
*   play capture shutter sound effect 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_camera_capture_play_sound(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef __MMI_SILENT_MEETING_PROFILE__
	if( IsSilentModeActivated() || IsMeetingModeActivated() )
		return;
#else
	if( IsSilentModeActivated() )
		return;
#endif /* __MMI_SILENT_MEETING_PROFILE__ */
   
   /* force to close multi channel sound */
	#if defined(__CAMERA_FEATURE_CONT_SHOT_BURST__) && defined(__CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__)
		mmi_camera_burstshot_close_multi_sound();
		coolsand_UI_cancel_timer(mmi_camera_burstshot_close_multi_sound);
	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ && __CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__ */
   
	/* play capture sound */
	switch(g_camera_cntx.setting.shutter_sound)
	{
		case CAMERA_SETTING_SHUTTER_SOUND_1:
		{
			/*Jinzh Modify Start For 5754,5846 Ver: TBM780  on 2007-8-9 10:3 */
#ifdef __FF_AUDIO_SET__
			cameraAppOnOff = TRUE;
#endif
			AudioPlayReq(CAMERA_CNT_SOUND1,DEVICE_AUDIO_PLAY_ONCE);
			/*Jinzh Modify End  For 5754,5846 Ver: TBM780  on 2007-8-9 10:3 */
#if 0
			mdi_audio_play_string_with_vol_path((void*)camera_sound_shutter_1,
															(U32)CAMERA_SOUND_SHUTTER_1_LEN,
														#if defined(MT6228) || defined(MT6229)
															MDI_FORMAT_WAV,
														#else
															MDI_FORMAT_DVI_ADPCM,
															#endif
															DEVICE_AUDIO_PLAY_ONCE,
															NULL,
															NULL,
															CAMERA_SHUTTER_SOUND_VOLUMN,
															MDI_DEVICE_SPEAKER_BOTH);
#endif
		}
		break;
#ifdef __CAMERA_FEATURE_SHUTTER_SOUND_2__	

		case CAMERA_SETTING_SHUTTER_SOUND_2:
		{

			/*Jinzh Modify Start For 5754,5846 Ver: TBM780  on 2007-8-9 10:3 */
#ifdef __FF_AUDIO_SET__
			cameraAppOnOff = TRUE;
#endif
			AudioPlayReq(CAMERA_CNT_SOUND2,DEVICE_AUDIO_PLAY_ONCE);
			/*Jinzh Modify End  For 5754,5846 Ver: TBM780  on 2007-8-9 10:3 */

#if 0
			mdi_audio_play_string_with_vol_path((void*)camera_sound_shutter_2,
															(U32)CAMERA_SOUND_SHUTTER_2_LEN,
														#if defined(MT6228) || defined(MT6229)
															MDI_FORMAT_WAV,
														#else
															MDI_FORMAT_DVI_ADPCM,
															#endif
															DEVICE_AUDIO_PLAY_ONCE,
															NULL,
															NULL,
															CAMERA_SHUTTER_SOUND_VOLUMN,
															MDI_DEVICE_SPEAKER_BOTH);
#endif
		}
		break;
#endif
#ifdef __CAMERA_FEATURE_SHUTTER_SOUND_3__	

		case CAMERA_SETTING_SHUTTER_SOUND_3:
		{

			/*Jinzh Modify Start For 5754,5846 Ver: TBM780  on 2007-8-9 10:3 */
#ifdef __FF_AUDIO_SET__
			cameraAppOnOff = TRUE;
#endif
			AudioPlayReq(CAMERA_CNT_SOUND3,DEVICE_AUDIO_PLAY_ONCE);
			/*Jinzh Modify End  For 5754,5846 Ver: TBM780  on 2007-8-9 10:3 */
#if 0
			mdi_audio_play_string_with_vol_path((void*)camera_sound_shutter_3,
															(U32)CAMERA_SOUND_SHUTTER_3_LEN,
														#if defined(MT6228) || defined(MT6229)
															MDI_FORMAT_WAV,
														#else
															MDI_FORMAT_DVI_ADPCM,
															#endif
															DEVICE_AUDIO_PLAY_ONCE,
															NULL,
															NULL,
															CAMERA_SHUTTER_SOUND_VOLUMN,
															MDI_DEVICE_SPEAKER_BOTH);			
#endif
		}
		break;
#endif
#ifdef __CAMERA_FEATURE_SHUTTER_SOUND_OFF__
		case CAMERA_SETTING_SHUTTER_SOUND_OFF:
#endif
		default:
			/* do not play sound */
			break;
	}	
}





/*****************************************************************************
* FUNCTION
*	 mmi_camera_capture_play_multi_channel_sound()
* DESCRIPTION
*   play capture shutter sound effect when support multi-channel
 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__)
static void mmi_camera_capture_play_multi_channel_sound(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* play capture sound */
	switch(g_camera_cntx.setting.shutter_sound)
	{
		case CAMERA_SETTING_SHUTTER_SOUND_1:
#if !defined(FLASH_OPT) 			
		case CAMERA_SETTING_SHUTTER_SOUND_2:			
		case CAMERA_SETTING_SHUTTER_SOUND_3:			
#endif			
		{
			if(g_camera_cntx.multi_sound_cap_idx == 0)
			{
				mdi_audio_mma_stop_wav(g_camera_cntx.multi_sound_capture0);
				mdi_audio_mma_play_wav(g_camera_cntx.multi_sound_capture0);
			}
			else if(g_camera_cntx.multi_sound_cap_idx == 1)
			{
				mdi_audio_mma_stop_wav(g_camera_cntx.multi_sound_capture1);
				mdi_audio_mma_play_wav(g_camera_cntx.multi_sound_capture1);
			}
			else
			{
				mdi_audio_mma_stop_wav(g_camera_cntx.multi_sound_capture2);
				mdi_audio_mma_play_wav(g_camera_cntx.multi_sound_capture2);
			}

			g_camera_cntx.multi_sound_cap_idx++;
			
			if(g_camera_cntx.multi_sound_cap_idx >=3)
			{
				g_camera_cntx.multi_sound_cap_idx = 0;
			}

		}
		break;

#ifdef __CAMERA_FEATURE_SHUTTER_SOUND_OFF__
		case CAMERA_SETTING_SHUTTER_SOUND_OFF:
#endif
		default:
			/* do not play sound */
			break;
	}
	
}
#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__ */



/*****************************************************************************
*
*	Toggle To Sub State 
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_toggle_to_sub_state()
* DESCRIPTION
*	 enter toggle to sub state
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_entry_toggle_to_sub_state(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* clear all keys */
	ClearAllKeyHandler();

	gdi_layer_set_blt_layer(g_camera_cntx.base_layer_handle, 0, 0, 0);	

	#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
	force_no_small_screen();
	#endif

	ShowCategory65Screen((PU8)GetString(STR_ID_CAMERA_NOTIFY_SELF_CAPTURE_ON), IMG_GLOBAL_ACTIVATED, NULL);
	g_camera_cntx.is_cat65_show = TRUE;	
	
	coolsand_UI_start_timer(TOGGLE_POPUP_FADE_TIME, mmi_camera_toggle_to_sub_timeout);
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_toggle_to_sub_state()
* DESCRIPTION
*	 exit save confirm state
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_exit_toggle_to_sub_state(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* will display a popup, go out application and re-enter */
	if(g_camera_cntx.is_cat65_show)
	{
		g_camera_cntx.is_cat65_show = FALSE;
		ExitCategoryFunction();
	}

	coolsand_UI_cancel_timer(mmi_camera_toggle_to_sub_timeout);

	/* stop anmation on popup category */
	gdi_image_stop_animation_all();

}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_toggle_to_sub_timeout()
* DESCRIPTION
*	 exit save confirm state
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_toggle_to_sub_timeout(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* enter sub preview state */
	CAMERA_ENTER_STATE(CAMERA_STATE_SUB_PREVIEW);
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */



/*****************************************************************************
*
*	Toggle To Main State 
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_toggle_to_main_state()
* DESCRIPTION
*	 enter toggle to main state
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_entry_toggle_to_main_state(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* clear all keys */
	ClearAllKeyHandler();
		
	/* show popup category */
	gdi_layer_set_blt_layer(g_camera_cntx.base_layer_handle, 0, 0, 0);

	#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
	force_no_small_screen();
	#endif

	ShowCategory65Screen((PU8)GetString(STR_ID_CAMERA_NOTIFY_SELF_CAPTURE_OFF), IMG_GLOBAL_ACTIVATED, NULL);
	g_camera_cntx.is_cat65_show = TRUE;
	
	coolsand_UI_start_timer(TOGGLE_POPUP_FADE_TIME, mmi_camera_toggle_to_main_timeout);
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_toggle_to_main_state()
* DESCRIPTION
*	 exit toggle to main state
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_exit_toggle_to_main_state(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* will display a popup, go out application and re-enter */
	if(g_camera_cntx.is_cat65_show)
	{
		g_camera_cntx.is_cat65_show = FALSE;
		ExitCategoryFunction();
	}

	coolsand_UI_cancel_timer(mmi_camera_toggle_to_main_timeout);

	/* stop anmation on popup category */
	gdi_image_stop_animation_all();

}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_toggle_to_main_timeout()
* DESCRIPTION
*	 toggle to main popup timeout
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_toggle_to_main_timeout(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* enter sub preview state */
	CAMERA_ENTER_STATE(CAMERA_STATE_PREVIEW);
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */






/*****************************************************************************
*
*	Sublcd Screen
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_sublcd_screen()
* DESCRIPTION
*   entry sublcd screen, if still remain on standard MMI sublcd screen, time update will
*   redraw sublcd while mainlcd is previewing, will cause driver assert.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_SUBLCD__
static void mmi_camera_entry_sublcd_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* print debug info */
	CAMERA_TRACE_FUNC_ENTER_SUBLCD();
	
	ExecSubLCDCurrExitHandler();
	ShowCategory331Screen();
	SetSubLCDExitHandler(mmi_camera_exit_sublcd_screen);

	g_camera_cntx.is_sublcd_display = TRUE;
}
#endif /* __MMI_SUBLCD__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_sublcd_screen()
* DESCRIPTION
*   exit sublcd screen
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_SUBLCD__
static void mmi_camera_exit_sublcd_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/	
   SubLCDHistoryNode sub_history;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	
	/* print debug info */
	CAMERA_TRACE_FUNC_EXIT_SUBLCD();
	
	#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
		/* someone may exit/draw sublcd first then exit mainlcd, this may cause driver assert */
  		/* also stop preview here */
		mmi_camera_sublcd_preview_stop();
	#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */

	g_camera_cntx.is_sublcd_display = FALSE;

	sub_history.entryFuncPtr = mmi_camera_entry_sublcd_screen;
	AddSubLCDHistory(&sub_history);
	
}
#endif /* __MMI_SUBLCD__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_sublcd_draw_icon()
* DESCRIPTION
*   draw a sublcd icon.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_SUBLCD__
void mmi_camera_sublcd_draw_icon(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 image_width;
	S32 image_height;
	S32 lcd_width;
	S32 lcd_height;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
	gdi_lcd_get_dimension(&lcd_width, &lcd_height);

	gdi_layer_reset_clip();

	gdi_layer_clear_background(GDI_COLOR_WHITE);
	gdi_image_get_dimension_id(IMG_ID_CAMERA_PREVIEW_SUBLCD_ICON, &image_width, &image_height);
	gdi_image_draw_id((lcd_width-image_width)>>1,(lcd_height-image_height)>>1,IMG_ID_CAMERA_PREVIEW_SUBLCD_ICON);
	gdi_layer_blt_base_layer(0,0, lcd_width-1, lcd_height-1);

	gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
}
#endif /* __MMI_SUBLCD__ */





/*****************************************************************************
*
*	Sublcd Preview State 
*
*****************************************************************************/
	
/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_sublcd_preview_state()
* DESCRIPTION
*	 entry sublcd preview state 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_entry_sublcd_preview_state(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	/* draw a popup on mainlcd  */
	/* only first time enter sub preview needs to draw main lcd */
	if((g_camera_cntx.app_prev_state == CAMERA_STATE_EXIT) ||
		(g_camera_cntx.app_prev_state == CAMERA_STATE_TOGGLE_TO_SUB) )
	{
		gdi_layer_lock_frame_buffer();

		/* clear fullscreen */
		#ifdef CAMERA_MODULE_WITH_LCD
			gdi_layer_clear(CAMEAR_BG_TRASN_COLOR);
		#else
			gdi_layer_clear(GDI_COLOR_BLACK);
		#endif 
		
		#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
			force_no_small_screen();
		#endif
		
		ShowCategory65Screen((PU8)GetString(STR_ID_CAMERA_NOFITY_SELF_CAPTURE_SUBLCD_PREVIEW), IMG_GLOBAL_INFO, NULL);
		g_camera_cntx.is_cat65_show = TRUE;
			
		/* IMG_GLOBAL_INFO may be an animation, we have to stop it avoid both lcd updating */
		gdi_image_stop_animation_all();
		gdi_layer_unlock_frame_buffer();

		mmi_camera_sublcd_preview_set_key_hdlr();
		gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
	}

	/* turn on led highlight */
	mmi_camera_turn_on_preview_led_highlight();

	/* set key handler */
	mmi_camera_sublcd_preview_set_key_hdlr();

	/* set correct lcm rotate */
	/* config rotate of preview layer, only ISP_SUPPORT need */
	#if defined(ISP_SUPPORT)
	gdi_layer_push_and_set_active(g_camera_cntx.sublcd_base_layer_handle);
	if(g_camera_cntx.is_clam_close==TRUE)
	gdi_layer_set_rotate(CAMERA_LCM_SUBLCD_CLAM_CLOSE_ROTATE);
	else
	gdi_layer_set_rotate(CAMERA_LCM_SUBLCD_CLAM_OPEN_ROTATE);
	gdi_layer_pop_and_restore_active();
	#endif
	/* preview start */
	mmi_camera_sublcd_preview_start();
	
	/* check if preview function has error */
	if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
		mmi_camera_display_sublcd_error_popup();


}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_sublcd_preview_state()
* DESCRIPTION
*	 exit sublcd preview state 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_exit_sublcd_preview_state(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* exit app || toggle to main */

	/* config rotate of preview layer, only ISP_SUPPORT need */
	#if defined(ISP_SUPPORT)
	gdi_layer_push_and_set_active(g_camera_cntx.sublcd_base_layer_handle);
	gdi_layer_set_rotate(CAMERA_LCM_ROTATE_0);
	gdi_layer_pop_and_restore_active();
	#endif
	if((g_camera_cntx.app_next_state == CAMERA_STATE_EXIT) ||
		(g_camera_cntx.app_next_state == CAMERA_STATE_TOGGLE_TO_MAIN))
	{
		/* turn off led highlight */
		mmi_camera_turn_off_led_highlight();
	
		/* stop previewing */
		mmi_camera_sublcd_preview_stop();

		/* exit popup screen to free drawmanager's resource */
		/* this shall call after preview is stopped, or will cause multiple lcd access */
		if(g_camera_cntx.is_cat65_show)
		{
			ExitCategoryFunction();
		}
	}

	
	if(g_camera_cntx.app_next_state == CAMERA_STATE_EXIT)
	{
		/* 
			when interrupt happened while clam is close (preview on sub)
			we will go handling interrupt, when back from interrupt, 
			we shall force enter sub again to keep clam open close state correct 
		*/
		
		if(g_camera_cntx.is_clam_close)
			g_camera_cntx.is_force_self_capture = TRUE;
	}
	

}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_sublcd_preview_set_key_hdlr()
* DESCRIPTION
*   set sublcd key handler 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_sublcd_preview_set_key_hdlr()
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	#ifndef __CAMERA_FEATURE_OSD_MENU__
		SetKeyHandler(mmi_camera_sublcd_preview_lsk_release, KEY_LSK, KEY_EVENT_UP);
		SetKeyHandler(mmi_camera_sublcd_preview_lsk_press, KEY_LSK, KEY_EVENT_DOWN);
	#endif /* __CAMERA_FEATURE_OSD_MENU__ */

	SetKeyHandler(mmi_camera_sublcd_preview_rsk_release, KEY_RSK, KEY_EVENT_UP);
   SetKeyHandler(mmi_camera_sublcd_preview_rsk_press, KEY_RSK, KEY_EVENT_DOWN);
#ifndef __MMI_ANALOGTV__    	
	SetKeyHandler(mmi_camera_sublcd_capture_button_press, CAMERA_CAPTURE_KEY, KEY_EVENT_DOWN);	
#endif
	if(CAMERA_CAPTURE_EXTRA_KEY!=-1)
		SetKeyHandler(mmi_camera_sublcd_capture_button_press, CAMERA_CAPTURE_EXTRA_KEY, KEY_EVENT_DOWN);
		 
#ifdef WIN32 
		/* Simulator debug */
		SetKeyHandler(mmi_camera_sublcd_capture_button_press, KEY_0, KEY_EVENT_DOWN);
#endif 

	SetKeyHandler(mmi_camera_toggle_self_capture, CAMERA_TOGGLE_SELF_CAPTURE_KEY, KEY_EVENT_DOWN);
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_sublcd_preview_start()
* DESCRIPTION
*   start previewing on sublcd 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_sublcd_preview_start(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	mdi_camera_setting_struct	camera_setting_data;
	S32								sublcd_width;
	S32								sublcd_height;
	BOOL								is_tvout;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* get sublcd base layer handle */
	gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
	gdi_layer_get_dimension(&sublcd_width, &sublcd_height);

	#ifdef CAMERA_MODULE_WITH_LCD
		gdi_layer_clear(CAMEAR_BG_TRASN_COLOR);
	#endif /* CAMERA_MODULE_WITH_LCD */
	
	gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);

	/* preview parametes */
	camera_setting_data.zoom				= mmi_camera_preview_get_zoom_factor();
	camera_setting_data.wb					= camera_wb_command_map[g_camera_cntx.setting.wb];	
	camera_setting_data.ev					= camera_ev_command_map[g_camera_cntx.setting.ev];
	camera_setting_data.banding 			= camera_banding_command_map[g_camera_cntx.setting.banding];
	camera_setting_data.flash				= camera_flash_command_map[g_camera_cntx.setting.flash];
	camera_setting_data.ae_meter			= camera_ae_meter_command_map[g_camera_cntx.setting.ae_meter];
	camera_setting_data.iso					= camera_iso_command_map[g_camera_cntx.setting.iso];
	camera_setting_data.night				= g_camera_cntx.setting.night;
	camera_setting_data.shot_mode			= g_camera_cntx.setting.shot_mode;

	/* effect */
	camera_setting_data.effect			= camera_effect_command_map[g_camera_cntx.setting.effect];
	camera_setting_data.brightness 	= 128;
	camera_setting_data.saturation 	= 128;
	camera_setting_data.contrast 		= 128;
	camera_setting_data.hue		 		= 0;

#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__
	if(g_camera_cntx.setting.effect == CAMERA_SETTING_EFFECT_CUSTOM_FX_1)
	{
		camera_setting_data.effect			= camera_effect_command_map[CAMERA_SETTING_EFFECT_NORMAL];
		camera_setting_data.brightness 	= g_camera_cntx.setting.fx_brightness[0];
		camera_setting_data.saturation 	= g_camera_cntx.setting.fx_saturation[0];	
		camera_setting_data.contrast 		= g_camera_cntx.setting.fx_contrast[0];
		camera_setting_data.hue		 		= g_camera_cntx.setting.fx_hue[0];		
	}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_1__ */

#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_2__
	if(g_camera_cntx.setting.effect == CAMERA_SETTING_EFFECT_CUSTOM_FX_2)
	{
		camera_setting_data.effect			= camera_effect_command_map[CAMERA_SETTING_EFFECT_NORMAL];
		camera_setting_data.brightness 	= g_camera_cntx.setting.fx_brightness[1];
		camera_setting_data.saturation 	= g_camera_cntx.setting.fx_saturation[1];	
		camera_setting_data.contrast 		= g_camera_cntx.setting.fx_contrast[1];
		camera_setting_data.hue		 		= g_camera_cntx.setting.fx_hue[1];	
	}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_2__ */

#ifdef __CAMERA_FEATURE_EFFECT_CUSTOM_FX_3__
	if(g_camera_cntx.setting.effect == CAMERA_SETTING_EFFECT_CUSTOM_FX_3)
	{
		camera_setting_data.effect			= camera_effect_command_map[CAMERA_SETTING_EFFECT_NORMAL];
		camera_setting_data.brightness 	= g_camera_cntx.setting.fx_brightness[2];
		camera_setting_data.saturation 	= g_camera_cntx.setting.fx_saturation[2];	
		camera_setting_data.contrast 		= g_camera_cntx.setting.fx_contrast[2];
		camera_setting_data.hue		 		= g_camera_cntx.setting.fx_hue[2];	
	}
#endif /* __CAMERA_FEATURE_EFFECT_CUSTOM_FX_32__ */

	/* preview size is sublcd size */
	camera_setting_data.preview_width	= CAMERA_FEATURE_SUBLCD_PREVIEW_SIZE_WIDTH;
	camera_setting_data.preview_height	= CAMERA_FEATURE_SUBLCD_PREVIEW_SIZE_HEIGHT;
	
	/* capture parameters */
	camera_setting_data.image_qty			= camera_image_qty_command_map[g_camera_cntx.setting.image_qty];
	camera_setting_data.image_width		= g_camera_cntx.image_width;
	camera_setting_data.image_height		= g_camera_cntx.image_height;	

	/* preview on sublcd */
	camera_setting_data.lcm = MDI_CAMERA_PREVIEW_LCM_SUBLCD;
	
	if(g_camera_cntx.is_clam_close == TRUE)
	{
		camera_setting_data.preview_rotate	= CAMERA_PREVIEW_SUBLCD_CLAM_CLOSE_ROTATE;
		camera_setting_data.lcm_rotate		= CAMERA_LCM_SUBLCD_CLAM_CLOSE_ROTATE;
	}
	else
	{
		camera_setting_data.preview_rotate 	= CAMERA_PREVIEW_SUBLCD_CLAM_OPEN_ROTATE;
		camera_setting_data.lcm_rotate		= CAMERA_LCM_SUBLCD_CLAM_OPEN_ROTATE;
	}

	camera_setting_data.osd_rotate		= CAMERA_DISPLAY_ROTATE_0;
	/* set sublcd's layer info to lcd driver by calling gdi_layer_set_blt_layer()*/
	/* set layer for preview */
	mmi_camera_sublcd_preview_set_blt_layer(TRUE);	/* TRUE == Preview */

	/* turn on led highlight */
	mmi_camera_turn_on_preview_led_highlight();


	is_tvout = FALSE;
	
	#ifdef __MMI_TVOUT__
		/* when preview on sublcd, only fullscreen style willupdate to TV */
		if(mdi_tvout_is_enable())
		{
			if(mmi_phnset_tvout_get_style() == SETTING_TVOUT_STYLE_FULLSCREEN)
			{		
				is_tvout = TRUE;
			}
		}
	#endif /* __MMI_TVOUT__ */
	
	/* start preview */
	g_camera_cntx.last_error 
		= mdi_camera_preview_start(	g_camera_cntx.sublcd_base_layer_handle,
												(S32)0,
												(S32)0,
												(U16)sublcd_width,
												(U16)sublcd_height, 
												GDI_LAYER_ENABLE_LAYER_0,
												GDI_LAYER_ENABLE_LAYER_0,
												CAMEAR_BG_TRASN_COLOR,		/* this value is use only CAMERA_MODULE_WITH_LCD */
												is_tvout,
												&camera_setting_data);
	
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */




/*****************************************************************************
* FUNCTION
*	 mmi_camera_sublcd_preview_stop()
* DESCRIPTION
*   stop previewing on sublcd 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_sublcd_preview_stop(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mdi_camera_preview_stop();

	/* restore black background */
	#ifdef CAMERA_MODULE_WITH_LCD
		gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
		gdi_layer_clear(GDI_COLOR_BLACK);
		gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
	#endif /* CAMERA_MODULE_WITH_LCD */
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_sublcd_preview_set_blt_layer()
* DESCRIPTION
*   set blt layer based on current status
* PARAMETERS
*	 is_previewing 	IN 	is preview flag
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_sublcd_preview_set_blt_layer(BOOL is_previewing)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* currently no function, reserve it when sublcd will use multi-layer */
	
#if defined(CAMERA_MODULE)

	gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
	gdi_layer_set_blt_layer(g_camera_cntx.sublcd_base_layer_handle,0,0,0);
	gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);

#elif defined(ISP_SUPPORT)

	gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
	gdi_layer_set_blt_layer(g_camera_cntx.sublcd_base_layer_handle,0,0,0);
	gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
	
#endif /* chip version */

}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */




/*****************************************************************************
* FUNCTION
*	 mmi_camera_sublcd_preview_lsk_press()
* DESCRIPTION
*   stop sublcd preview then mimic softkey press.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_sublcd_preview_lsk_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   
	/* this function will stop sublcd preview when soft key is down */
	/* if not stop sublcd preview first, mainlcd will draw softkey down and cause driver assert */
	
	if((g_camera_cntx.app_state == CAMERA_STATE_SUB_PREVIEW) ||
		(g_camera_cntx.app_state == CAMERA_STATE_SUB_CONTSHOT) || 
		(g_camera_cntx.app_state == CAMERA_STATE_SUB_COUNTDOWN))
	{
		mmi_camera_sublcd_preview_stop();
		coolsand_UI_cancel_timer(mmi_camera_sublcd_contshot_cyclic);
		coolsand_UI_cancel_timer(mmi_camera_sublcd_countdown_cyclic);		
	}

}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */




/*****************************************************************************
* FUNCTION
*	 mmi_camera_sublcd_preview_rsk_press()
* DESCRIPTION
*   stop sublcd preview then mimic softkey press.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_sublcd_preview_rsk_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
  
	/* this function will stop sublcd preview when soft key is down */
	/* if not stop sublcd preview first, mainlcd will draw softkey down and cause driver assert */
	/* if is preview on sub and RSK pressm, has to stop preview first then darw on Main */
	
	if((g_camera_cntx.app_state == CAMERA_STATE_SUB_PREVIEW) ||
		(g_camera_cntx.app_state == CAMERA_STATE_SUB_CONTSHOT) || 
		(g_camera_cntx.app_state == CAMERA_STATE_SUB_COUNTDOWN))
	{
		mmi_camera_sublcd_preview_stop();
		coolsand_UI_cancel_timer(mmi_camera_sublcd_contshot_cyclic);
		coolsand_UI_cancel_timer(mmi_camera_sublcd_countdown_cyclic);				
	}
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_sublcd_preview_lsk_release()
* DESCRIPTION
*   stop sublcd preview then mimic softkey press.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_sublcd_preview_lsk_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mmi_camera_entry_option_menu_screen();
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_sublcd_preview_rsk_release()
* DESCRIPTION
*   stop sublcd preview then mimic softkey press.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_sublcd_preview_rsk_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	GoBackHistory();
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */




/*****************************************************************************
*
*	Sublcd Countdown State
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_sublcd_countdown_state()
* DESCRIPTION
*	 entry count down state 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_entry_sublcd_countdown_state(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* register soft keys */	
	ClearAllKeyHandler();

	SetKeyHandler(mmi_camera_sublcd_preview_lsk_release, KEY_LSK, KEY_EVENT_UP);
	SetKeyHandler(mmi_camera_sublcd_preview_lsk_press, KEY_LSK, KEY_EVENT_DOWN);

	SetKeyHandler(mmi_camera_sublcd_preview_rsk_release, KEY_RSK, KEY_EVENT_UP);
   SetKeyHandler(mmi_camera_sublcd_preview_rsk_press, KEY_RSK, KEY_EVENT_DOWN);	

	/* clearn center key - since if not clear, this key will mappign to LSK */
#ifndef __MMI_ANALOGTV__    
	if(CAMERA_CAPTURE_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_KEY, KEY_EVENT_DOWN);	
#endif
	if(CAMERA_CAPTURE_EXTRA_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_EXTRA_KEY, KEY_EVENT_DOWN);


	/* init countdown para */
	switch(g_camera_cntx.setting.delay_timer)
	{
		case CAMERA_SETTING_DELAY_TIMER_1:
			{
				g_camera_cntx.capture_countdown = CAMERA_FEATURE_DELAY_TIMER_1_DURATION*1000;
			}
			break;

	
		case CAMERA_SETTING_DELAY_TIMER_2:
			{
				g_camera_cntx.capture_countdown = CAMERA_FEATURE_DELAY_TIMER_2_DURATION*1000;
			}
			break;
		
		case CAMERA_SETTING_DELAY_TIMER_3:
			{
				g_camera_cntx.capture_countdown = CAMERA_FEATURE_DELAY_TIMER_3_DURATION*1000;
			}
			break;
			
		default:
			/* restore default value */
			mmi_camera_restore_setting();
			break;
	}

	/* start count down */
	mmi_camera_sublcd_countdown_cyclic();
	
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_sublcd_countdown_state()
* DESCRIPTION
*	 entry count down state 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_exit_sublcd_countdown_state(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* stop countdown timer */
	coolsand_UI_cancel_timer(mmi_camera_sublcd_countdown_stop_LED);
	coolsand_UI_cancel_timer(mmi_camera_sublcd_countdown_cyclic);
	
	/* free preview resource */
	if(g_camera_cntx.app_next_state == CAMERA_STATE_EXIT)
	{
		/* turn off led highlight */
		mmi_camera_turn_off_led_highlight();
	
		/* stop previewing */
		mmi_camera_sublcd_preview_stop();
	}

	if(g_camera_cntx.app_next_state == CAMERA_STATE_EXIT)
	{
		/* 
			when interrupt happened while clam is close (preview on sub)
			we will go handling interrupt, when back from interrupt, 
			we shall force enter sub again to keep clam open close state correct 
		*/
		
		if(g_camera_cntx.is_clam_close)
			g_camera_cntx.is_force_self_capture = TRUE;
	}
	
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_countdown_cyclic()
* DESCRIPTION
*   count down delay timer cyclic loop
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_sublcd_countdown_cyclic(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* countdonw duration and freq */
	if(g_camera_cntx.capture_countdown > 6000)
	{
		if(g_camera_cntx.setting.led_highlight == CAMERA_SETTING_LED_HIGHLIGHT_OFF)
		{
			SetGPIOLevelReq(GPIO_DEV_LED_STATUS_1, LED_LIGHT_LEVEL5);
		}
		
		coolsand_UI_start_timer(100, mmi_camera_sublcd_countdown_stop_LED);
		coolsand_UI_start_timer(1000, mmi_camera_sublcd_countdown_cyclic);
		g_camera_cntx.capture_countdown -= 1000;

	#ifdef __MMI_SILENT_MEETING_PROFILE__
		if( !IsSilentModeActivated() && !IsMeetingModeActivated() )
			PlayAudioDviStream((PU8)camera_sound_countdown, CAMREA_SOUND_COUNTDOWN_LEN, DEVICE_AUDIO_PLAY_ONCE);
	#else
		if( !IsSilentModeActivated() )
			PlayAudioDviStream((PU8)camera_sound_countdown, CAMREA_SOUND_COUNTDOWN_LEN, DEVICE_AUDIO_PLAY_ONCE);
	#endif /* __MMI_SILENT_MEETING_PROFILE__ */

	}
	else if(g_camera_cntx.capture_countdown > 2000)
	{
		if(g_camera_cntx.setting.led_highlight == CAMERA_SETTING_LED_HIGHLIGHT_OFF)
		{
			SetGPIOLevelReq(GPIO_DEV_LED_STATUS_1, LED_LIGHT_LEVEL5);
		}
		
		coolsand_UI_start_timer(100, mmi_camera_sublcd_countdown_stop_LED);
		coolsand_UI_start_timer(500, mmi_camera_sublcd_countdown_cyclic);
		g_camera_cntx.capture_countdown -= 500;

	#ifdef __MMI_SILENT_MEETING_PROFILE__
		if( !IsSilentModeActivated() && !IsMeetingModeActivated() )
			PlayAudioDviStream((PU8)camera_sound_countdown, CAMREA_SOUND_COUNTDOWN_LEN, DEVICE_AUDIO_PLAY_ONCE);
	#else
		if( !IsSilentModeActivated() )
			PlayAudioDviStream((PU8)camera_sound_countdown, CAMREA_SOUND_COUNTDOWN_LEN, DEVICE_AUDIO_PLAY_ONCE);
	#endif /* __MMI_SILENT_MEETING_PROFILE__ */

	}
	else if(g_camera_cntx.capture_countdown >= 0)
	{
		if(g_camera_cntx.setting.led_highlight == CAMERA_SETTING_LED_HIGHLIGHT_OFF)
		{
			SetGPIOLevelReq(GPIO_DEV_LED_STATUS_1, LED_LIGHT_LEVEL5);
		}
		
		coolsand_UI_start_timer(100, mmi_camera_sublcd_countdown_stop_LED);
		coolsand_UI_start_timer(200, mmi_camera_sublcd_countdown_cyclic);
		g_camera_cntx.capture_countdown -= 200;

	#ifdef __MMI_SILENT_MEETING_PROFILE__
		if( !IsSilentModeActivated() && !IsMeetingModeActivated() )
			PlayAudioDviStream((PU8)camera_sound_countdown, CAMREA_SOUND_COUNTDOWN_LEN, DEVICE_AUDIO_PLAY_ONCE);
	#else
		if( !IsSilentModeActivated() )
			PlayAudioDviStream((PU8)camera_sound_countdown, CAMREA_SOUND_COUNTDOWN_LEN, DEVICE_AUDIO_PLAY_ONCE);
	#endif /* __MMI_SILENT_MEETING_PROFILE__ */

	}
	else
	{
		mmi_camera_sublcd_capture();
		return;
	}
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_sublcd_countdown_stop_LED()
* DESCRIPTION
*	 stop count down LED
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_sublcd_countdown_stop_LED(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	if(g_camera_cntx.setting.led_highlight == CAMERA_SETTING_LED_HIGHLIGHT_OFF)
	{
		SetGPIOLevelReq(GPIO_DEV_LED_STATUS_1, LED_LIGHT_LEVEL0);	
	}
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */







/*****************************************************************************
*
*	Sublcd Cont Shot State
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_sublcd_contshot_state()
* DESCRIPTION
*	 entry contshot state 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_entry_sublcd_contshot_state(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	
	/* register soft keys */	
	ClearAllKeyHandler();

	SetKeyHandler(mmi_camera_sublcd_preview_lsk_press, KEY_LSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_camera_sublcd_preview_lsk_release, KEY_LSK, KEY_EVENT_UP);		
	
	SetKeyHandler(mmi_camera_sublcd_preview_rsk_press, KEY_RSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_camera_sublcd_preview_rsk_release, KEY_RSK, KEY_EVENT_UP);	

	/* clearn center key - since if not clear, this key will mappign to LSK */
#ifndef __MMI_ANALOGTV__    
	if(CAMERA_CAPTURE_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_KEY, KEY_EVENT_DOWN);	
#endif
	if(CAMERA_CAPTURE_EXTRA_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_EXTRA_KEY, KEY_EVENT_DOWN);


	/* init count down para */
	switch(g_camera_cntx.setting.cont_shot)
	{
		case CAMERA_SETTING_CONT_SHOT_OFF:
			{
				g_camera_cntx.cont_shot_total_count = 1;
				g_camera_cntx.cont_shot_count			= 0;
			}
			break;
	
		case CAMERA_SETTING_CONT_SHOT_1:
			{
				g_camera_cntx.cont_shot_total_count = CAMERA_FEATURE_CONT_SHOT_1_COUNT;
				g_camera_cntx.cont_shot_count			= 0;
			}
			break;

	
		case CAMERA_SETTING_CONT_SHOT_2:
			{
				g_camera_cntx.cont_shot_total_count = CAMERA_FEATURE_CONT_SHOT_2_COUNT;
				g_camera_cntx.cont_shot_count			= 0;
			}
			break;
		
		case CAMERA_SETTING_CONT_SHOT_3:
			{
				g_camera_cntx.cont_shot_total_count = CAMERA_FEATURE_CONT_SHOT_3_COUNT;
				g_camera_cntx.cont_shot_count			= 0;
			}
			break;
	}

	mmi_camera_sublcd_contshot_cyclic();
	
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_sublcd_contshot_state()
* DESCRIPTION
*	 entry contshot state 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_exit_sublcd_contshot_state(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* cancel contshot timer */
	coolsand_UI_cancel_timer(mmi_camera_sublcd_contshot_cyclic);

	/* free preview resource */
	if(g_camera_cntx.app_next_state == CAMERA_STATE_EXIT)
	{
		/* turn off led highlight */
		mmi_camera_turn_off_led_highlight();
	
		/* stop previewing */
		mmi_camera_sublcd_preview_stop();
	}

	if(g_camera_cntx.app_next_state == CAMERA_STATE_EXIT)
	{
		/* 
			when interrupt happened while clam is close (preview on sub)
			we will go handling interrupt, when back from interrupt, 
			we shall force enter sub again to keep clam open close state correct 
		*/
		
		if(g_camera_cntx.is_clam_close)
			g_camera_cntx.is_force_self_capture = TRUE;
	}
	
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_sublcd_contshot_cyclic()
* DESCRIPTION
*   cont shot cyclic loop
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_sublcd_contshot_cyclic(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8 	buf_filename[FMGR_PATH_BUFFER_SIZE];
	U8 	continue_capture = TRUE;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	
	/* this function has nvram access */
	mmi_camera_get_next_filename(buf_filename);

	/* start timer for next shot */
	coolsand_UI_start_timer(100, mmi_camera_sublcd_contshot_cyclic);
	
	mmi_camera_turn_on_capture_led_highlight();
	if (g_camera_cntx.cont_shot_count+1 == g_camera_cntx.cont_shot_total_count)
		continue_capture = FALSE;

	g_camera_cntx.last_error = mdi_camera_capture_to_file(buf_filename,continue_capture);
	
	/* play capture sound after capture */
	mmi_camera_capture_play_sound();

	/* restore black background */
	#ifdef CAMERA_MODULE_WITH_LCD
		gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
		gdi_layer_clear(GDI_COLOR_BLACK);
		gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
	#endif /* CAMERA_MODULE_WITH_LCD */
	
	mmi_camera_turn_off_led_highlight();

	/* captured, previwing is stopped, change blt layer */
	mmi_camera_sublcd_preview_set_blt_layer(FALSE);

	if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
	{
		coolsand_UI_cancel_timer(mmi_camera_sublcd_contshot_cyclic);
		MMI_FS_Delete((U16*)buf_filename);
		mmi_camera_display_sublcd_error_popup();
		return;
	}

	g_camera_cntx.cont_shot_count++;

	/* terminate condition */
	if(g_camera_cntx.cont_shot_count >= g_camera_cntx.cont_shot_total_count)
	{
		/* stop timer */
		coolsand_UI_cancel_timer(mmi_camera_sublcd_contshot_cyclic);
		CAMERA_ENTER_STATE(CAMERA_STATE_SUB_PREVIEW);
		return;
	}
	else
	{
		/* change blt layer for previwing */
		mmi_camera_sublcd_preview_set_blt_layer(TRUE);		

		/* config rotate of preview layer, only ISP_SUPPORT need */
		#if defined(ISP_SUPPORT)
		gdi_layer_push_and_set_active(g_camera_cntx.sublcd_base_layer_handle);

		if(g_camera_cntx.is_clam_close==TRUE)
		gdi_layer_set_rotate(CAMERA_LCM_SUBLCD_CLAM_CLOSE_ROTATE);
		else
		gdi_layer_set_rotate(CAMERA_LCM_SUBLCD_CLAM_OPEN_ROTATE);

		gdi_layer_pop_and_restore_active();
		#endif

		mmi_camera_sublcd_preview_start();
	
		if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
		{
			/* stop timer */
			coolsand_UI_cancel_timer(mmi_camera_sublcd_contshot_cyclic);
			mmi_camera_display_sublcd_error_popup();	
			return;
		}
	}

}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */




/*****************************************************************************
*
*	Sublcd Capture Burst State
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_sublcd_burstshot_state()
* DESCRIPTION
*	 entry contshot state 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_SELF_CAPTURE__) && defined(__CAMERA_FEATURE_CONT_SHOT_BURST__)
static void mmi_camera_entry_sublcd_burstshot_state(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	/* register softkeys */	
	ClearAllKeyHandler();
	
	SetKeyHandler(mmi_camera_sublcd_preview_lsk_press, KEY_LSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_camera_sublcd_preview_lsk_release, KEY_LSK, KEY_EVENT_UP);		
	
	SetKeyHandler(mmi_camera_sublcd_preview_rsk_press, KEY_RSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_camera_sublcd_preview_rsk_release, KEY_RSK, KEY_EVENT_UP);	

	/* clearn center key - since if not clear, this key will mappign to LSK */
#ifndef __MMI_ANALOGTV__    
	if(CAMERA_CAPTURE_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_KEY, KEY_EVENT_DOWN);	
#endif
	if(CAMERA_CAPTURE_EXTRA_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_EXTRA_KEY, KEY_EVENT_DOWN);

	mmi_camera_turn_on_capture_led_highlight();

	g_camera_cntx.is_capturing = TRUE;

	#if defined(__CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__)

		#ifdef __CAMERA_FEATURE_SHUTTER_SOUND_OFF__
		if(g_camera_cntx.setting.shutter_sound != CAMERA_SETTING_SHUTTER_SOUND_OFF)
		#endif
		{
			/* start multi_channel sound, for capture sound effect */

			/* open and play a dummy midi at backgroup - this is a must do to play multi-channel sound */
			g_camera_cntx.multi_sound_dummy_bg = 
				mdi_audio_mma_open_midi((void*)camera_sound_dummy_bg, CAMERA_SOUND_DUMMY_BG_LEN, 0, NULL);

			mdi_audio_mma_play_midi(g_camera_cntx.multi_sound_dummy_bg);

			g_camera_cntx.multi_sound_cap_idx = 0;

			switch(g_camera_cntx.setting.shutter_sound)
			{
				case CAMERA_SETTING_SHUTTER_SOUND_1:
				{
					g_camera_cntx.multi_sound_capture0 = 
						mdi_audio_mma_open_wav(	(void*)camera_sound_shutter_1,
														(U32)CAMERA_SOUND_SHUTTER_1_LEN,
					                           1,
			   	   	                     NULL);	

					g_camera_cntx.multi_sound_capture1 = 
						mdi_audio_mma_open_wav(	(void*)camera_sound_shutter_1,
														(U32)CAMERA_SOUND_SHUTTER_1_LEN,
					                           1,
			   	   	                     NULL);						

					g_camera_cntx.multi_sound_capture2 = 
						mdi_audio_mma_open_wav(	(void*)camera_sound_shutter_1,
														(U32)CAMERA_SOUND_SHUTTER_1_LEN,
					                           1,
			   	   	                     NULL);						
				}

				
				break;
#if !defined(FLASH_OPT) 
				case CAMERA_SETTING_SHUTTER_SOUND_2:
				{
					g_camera_cntx.multi_sound_capture0 = 				
						mdi_audio_mma_open_wav(	(void*)camera_sound_shutter_2,
														(U32)CAMERA_SOUND_SHUTTER_2_LEN,
					                           1,
			   	   	                     NULL);	
					
					g_camera_cntx.multi_sound_capture1 = 				
						mdi_audio_mma_open_wav(	(void*)camera_sound_shutter_2,
														(U32)CAMERA_SOUND_SHUTTER_2_LEN,
					                           1,
			   	   	                     NULL);	
					
					g_camera_cntx.multi_sound_capture2 = 				
						mdi_audio_mma_open_wav(	(void*)camera_sound_shutter_2,
														(U32)CAMERA_SOUND_SHUTTER_2_LEN,
					                           1,
			   	   	                     NULL);						
				}
				break;

				case CAMERA_SETTING_SHUTTER_SOUND_3:
				{
					g_camera_cntx.multi_sound_capture0 = 				
						mdi_audio_mma_open_wav(	(void*)camera_sound_shutter_3,
														(U32)CAMERA_SOUND_SHUTTER_3_LEN,
					                           1,
			   	   	                     NULL);

					g_camera_cntx.multi_sound_capture1 = 				
						mdi_audio_mma_open_wav(	(void*)camera_sound_shutter_3,
														(U32)CAMERA_SOUND_SHUTTER_3_LEN,
					                           1,
			   	   	                     NULL);		

					g_camera_cntx.multi_sound_capture2 = 				
						mdi_audio_mma_open_wav(	(void*)camera_sound_shutter_3,
														(U32)CAMERA_SOUND_SHUTTER_3_LEN,
					                           1,
			   	   	                     NULL);							
				}
				break;
#endif				
			}
		}

		 
		/* call mdi to capture and save */
		g_camera_cntx.last_error
			= mdi_camera_non_block_burst_capture_to_memory(	CAMERA_FEATURE_CONT_SHOT_BURST_COUNT,
																			&g_camera_cntx.burst_jpg_data,
																			mmi_camera_sublcd_burstshot_capture_result_hdlr);
		
	#else
		/* burst shot shall play before capture */
		mmi_camera_capture_play_sound(); 
	
		/* call mdi to capture and save */
		g_camera_cntx.last_error
			= mdi_camera_burst_capture_to_memory(	CAMERA_FEATURE_CONT_SHOT_BURST_COUNT,
																&g_camera_cntx.burst_jpg_data);

		/* restore black background */
		#ifdef CAMERA_MODULE_WITH_LCD
			gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
			gdi_layer_clear(GDI_COLOR_BLACK);
			gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
		#endif /* CAMERA_MODULE_WITH_LCD */


		/* stop led */
		mmi_camera_turn_off_led_highlight();
		
		if(g_camera_cntx.last_error != MDI_RES_CAMERA_SUCCEED)
		{
			playRequestedTone(WARNING_TONE);
			CAMERA_ENTER_STATE(CAMERA_STATE_SUB_PREVIEW);
		}
		else
		{
			CAMERA_ENTER_STATE(CAMERA_STATE_SUB_BURSTSHOT_SAVING);
		}

	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__ */

	


}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ && __CAMERA_FEATURE_CONT_SHOT_BURST__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_sublcd_burstshot_state()
* DESCRIPTION
*	 entry contshot state 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_SELF_CAPTURE__) && defined(__CAMERA_FEATURE_CONT_SHOT_BURST__)
static void mmi_camera_exit_sublcd_burstshot_state(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	/* stop dummy bg midi for multi-channel sound */
	#if defined(__CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__)

		if(g_camera_cntx.is_capturing)
		{
			mdi_camera_stop_non_block_burst_capture();
		}
	#ifdef __CAMERA_FEATURE_SHUTTER_SOUND_OFF__
		if(g_camera_cntx.setting.shutter_sound != CAMERA_SETTING_SHUTTER_SOUND_OFF)
	#endif
		{
			mdi_audio_mma_stop_midi(g_camera_cntx.multi_sound_dummy_bg);
			mdi_audio_mma_close_midi(g_camera_cntx.multi_sound_dummy_bg);

			mdi_audio_mma_stop_wav(g_camera_cntx.multi_sound_capture0);
			mdi_audio_mma_close_wav(g_camera_cntx.multi_sound_capture0);

			mdi_audio_mma_stop_wav(g_camera_cntx.multi_sound_capture1);
			mdi_audio_mma_close_wav(g_camera_cntx.multi_sound_capture1);
			
			mdi_audio_mma_stop_wav(g_camera_cntx.multi_sound_capture2);
			mdi_audio_mma_close_wav(g_camera_cntx.multi_sound_capture2);

			g_camera_cntx.multi_sound_capture0 	= 0;
			g_camera_cntx.multi_sound_capture1 	= 0;			
			g_camera_cntx.multi_sound_capture2 	= 0;	
			g_camera_cntx.multi_sound_dummy_bg 	= 0;
		}
		
	#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__ */

	if(g_camera_cntx.app_next_state == CAMERA_STATE_EXIT)
	{
		/* turn off led highlight */
		mmi_camera_turn_off_led_highlight();
	}

	if(g_camera_cntx.app_next_state == CAMERA_STATE_EXIT)
	{
		/* 
			when interrupt happened while clam is close (preview on sub)
			we will go handling interrupt, when back from interrupt, 
			we shall force enter sub again to keep clam open close state correct 
		*/
		
		if(g_camera_cntx.is_clam_close)
			g_camera_cntx.is_force_self_capture = TRUE;
	}
	
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ && __CAMERA_FEATURE_CONT_SHOT_BURST__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_sublcd_burstshot_capture_result_hdlr()
* DESCRIPTION
*	 no block burst shot capture result handler
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_SELF_CAPTURE__) && defined(__CAMERA_FEATURE_CONT_SHOT_BURST__) && defined(__CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__)
static void mmi_camera_sublcd_burstshot_capture_result_hdlr(MDI_RESULT ret, U16 captured_count)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* capture count start from 1 */

	mmi_camera_capture_play_multi_channel_sound(); 
	
	if(ret >= 0)
	{
		/* no error happened */
		if(captured_count == CAMERA_FEATURE_CONT_SHOT_BURST_COUNT)
		{
			g_camera_cntx.is_capturing = FALSE;
			CAMERA_ENTER_STATE(CAMERA_STATE_SUB_BURSTSHOT_SAVING);
			return;
		}
	}
	else
	{
		g_camera_cntx.is_capturing = FALSE;		
		playRequestedTone(WARNING_TONE);
		CAMERA_ENTER_STATE(CAMERA_STATE_SUB_PREVIEW);
	}
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ & __CAMERA_FEATURE_CONT_SHOT_BURST__ & __CAMERA_FEATURE_CONT_SHOT_BURST_NON_BLOCK__*/





/*****************************************************************************
*
*	Sublcd Save Burstshot State
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_sublcd_burstshot_saving_state()
* DESCRIPTION
*	 entry sublcd save burstshot state 
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_SELF_CAPTURE__) && defined(__CAMERA_FEATURE_CONT_SHOT_BURST__)
static void mmi_camera_entry_sublcd_burstshot_saving_state(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 			str_width;
	S32 			str_height;
	PS8			str_ptr;
	S32			lcd_width;
	S32 			lcd_height;
	gdi_handle	layer_handle;
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	/* register softkeys */	
	ClearAllKeyHandler();
	
	SetKeyHandler(mmi_camera_sublcd_preview_lsk_press, KEY_LSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_camera_sublcd_preview_lsk_release, KEY_LSK, KEY_EVENT_UP);		
	
	SetKeyHandler(mmi_camera_sublcd_preview_rsk_press, KEY_RSK, KEY_EVENT_DOWN);	
	SetKeyHandler(mmi_camera_sublcd_preview_rsk_release, KEY_RSK, KEY_EVENT_UP);	

	/* clearn center key - since if not clear, this key will mappign to LSK */
#ifndef __MMI_ANALOGTV__    
	if(CAMERA_CAPTURE_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_KEY, KEY_EVENT_DOWN);	
#endif
	if(CAMERA_CAPTURE_EXTRA_KEY!=-1)
		SetKeyHandler(mmi_camera_dummy_func, CAMERA_CAPTURE_EXTRA_KEY, KEY_EVENT_DOWN);

	/* drwa saving string */
	gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
	gdi_layer_reset_text_clip();

	gdi_lcd_get_dimension(&lcd_width, &lcd_height);
	gdi_layer_get_active(&layer_handle);
	
	str_ptr = (PS8)GetString(STR_ID_CAMERA_NOTIFY_SAVING);
	
	coolsand_UI_measure_string((UI_string_type)str_ptr, &str_width, &str_height);

	mmi_camera_draw_style_text(str_ptr, 
										(lcd_width-str_width)>>1,
										(lcd_height-str_height)>>1,
										&g_camera_cntx.active_camera_osd_ptr->hint_box.style_text);	
	
	gdi_layer_blt(layer_handle, 0, 0, 0, 0, 0, lcd_width-1, lcd_height-1);

	/* restore to main */
	gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
	
	/* start save burst shot */
	g_camera_cntx.burst_save_index = 0;
	mmi_camera_sublcd_burstshot_saving_cyclic();
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ && __CAMERA_FEATURE_CONT_SHOT_BURST__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_sub_burstshot_saving_state()
* DESCRIPTION
*	 exit sublcd save burstshot state
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_SELF_CAPTURE__) && defined(__CAMERA_FEATURE_CONT_SHOT_BURST__)
static void mmi_camera_exit_sublcd_burstshot_saving_state(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	coolsand_UI_cancel_timer(mmi_camera_sublcd_burstshot_saving_cyclic);
	gdi_image_stop_animation_all();
	
	if(g_camera_cntx.app_next_state == CAMERA_STATE_EXIT)
	{
		/* 
			when interrupt happened while clam is close (preview on sub)
			we will go handling interrupt, when back from interrupt, 
			we shall force enter sub again to keep clam open close state correct 
		*/
		
		if(g_camera_cntx.is_clam_close)
			g_camera_cntx.is_force_self_capture = TRUE;
	}
	
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ && __CAMERA_FEATURE_CONT_SHOT_BURST__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_sublcd_burstshot_saving_cyclic()
* DESCRIPTION
*	 loop to save sublcd burstshot image
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_SELF_CAPTURE__) && defined(__CAMERA_FEATURE_CONT_SHOT_BURST__)
static void mmi_camera_sublcd_burstshot_saving_cyclic(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	S8 			buf_filename[FMGR_PATH_BUFFER_SIZE];
	PU8			image_buf_ptr;	
	U32			image_buf_size;
	U32			written;
	FS_HANDLE 	file_handle;
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* this function has nvram access */
	mmi_camera_get_next_filename(buf_filename);

	/* start timer for next shot */
	coolsand_UI_start_timer(100, mmi_camera_sublcd_burstshot_saving_cyclic);

	/* save burst shot image */
	image_buf_ptr 	= (PU8)g_camera_cntx.burst_jpg_data.file_address[g_camera_cntx.burst_save_index];
	image_buf_size = (U32)g_camera_cntx.burst_jpg_data.file_size[g_camera_cntx.burst_save_index];

	file_handle = MMI_FS_Open((U16*)buf_filename, FS_CREATE | FS_READ_WRITE);

	if(file_handle >= 0)
	{
		MMI_FS_Write(file_handle, (void*)image_buf_ptr, image_buf_size, (UINT*)&written);
		MMI_FS_Close(file_handle);

		/* cant write in whole file, must be disk full */
		if(image_buf_size != written)
		{
			/* delete the file that is not save complete */
			MMI_FS_Delete((U16*)buf_filename);
			
			g_camera_cntx.last_error = MDI_RES_CAMERA_ERR_DISK_FULL;		

			/* this is not serious error, will play a notify tone */
			/* it will directly re-entry sub preview state */
			mmi_camera_display_sublcd_error_popup();

			return;
		}
	}
	else
	{
		if(file_handle == FS_DISK_FULL)
		{
			g_camera_cntx.last_error = MDI_RES_CAMERA_ERR_DISK_FULL;
		}
		else if(file_handle == FS_ROOT_DIR_FULL)
		{
			g_camera_cntx.last_error = CAMERA_ERR_ROOT_DIR_FULL;
		}			
		else if(file_handle == FS_WRITE_PROTECTION)
		{
			g_camera_cntx.last_error = MDI_RES_CAMERA_ERR_WRITE_PROTECTION;
		}
		else
		{
			g_camera_cntx.last_error = MDI_RES_CAMERA_ERR_CAPTURE_FAILED;
		}

		/* this is not serious error, will play a notify tone */
		/* it will directly re-entry sub preview state */
		mmi_camera_display_sublcd_error_popup();
		return;
	}

	/* teminate condition */
	if(g_camera_cntx.burst_save_index >= CAMERA_FEATURE_CONT_SHOT_BURST_COUNT-1)
	{	
		CAMERA_ENTER_STATE(CAMERA_STATE_SUB_PREVIEW);
		return;
	}

	/* increase index */
	g_camera_cntx.burst_save_index++;
	
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ && __CAMERA_FEATURE_CONT_SHOT_BURST__ */





/***************************************************************************
*
*	Sublcd Capture 
*
***************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_sublcd_capture()
* DESCRIPTION
*	 capture photo
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_sublcd_capture(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	switch(g_camera_cntx.setting.cont_shot)
	{

		case CAMERA_SETTING_CONT_SHOT_OFF:	
			mmi_camera_turn_on_capture_led_highlight();

			/* since user may capture when clam close, and open clam to view the captured image */
			/* disable clam open action */
			g_camera_cntx.is_dis_clam_open = TRUE;

			g_camera_cntx.last_error
				= mdi_camera_capture_to_memory(&g_camera_cntx.capture_buf_ptr, &g_camera_cntx.capture_size);

			/* play capture sound after capture */
			mmi_camera_capture_play_sound();

			mmi_camera_sublcd_draw_icon();

			mmi_camera_turn_off_led_highlight();
			CAMERA_ENTER_STATE(CAMERA_STATE_SAVE_CONFIRM);

			break;
		
		case CAMERA_SETTING_CONT_SHOT_1:

		#ifdef __CAMERA_FEATURE_CONT_SHOT_2__
			case CAMERA_SETTING_CONT_SHOT_2: 
		#endif
		
		#ifdef __CAMERA_FEATURE_CONT_SHOT_3__
			case CAMERA_SETTING_CONT_SHOT_3:
		#endif
			{
				CAMERA_ENTER_STATE(CAMERA_STATE_SUB_CONTSHOT);
				return;
			}
			break;
			
		#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
			case CAMERA_SETTING_CONT_SHOT_BURST:
			{
				CAMERA_ENTER_STATE(CAMERA_STATE_SUB_BURSTSHOT);
				return;
			}
			break;
		#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */
				
		#ifdef __CAMERA_FEATURE_CONT_SHOT_TILE__
			case CAMERA_SETTING_CONT_SHOT_TILE:
			{
				mmi_camera_sublcd_tileshot_capture();
				return;
			}
			break;
		#endif /* __CAMERA_FEATURE_CONT_SHOT_TILE__ */

		default:
			/* restore default value */
			mmi_camera_restore_setting();
			break;	
		
	}

}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_sublcd_capture_button_press()
* DESCRIPTION
*   capture button presse
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
static void mmi_camera_sublcd_capture_button_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
 #ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

	/* capture only works is camera is ready, otherwise will ignore this action */
	if(mdi_camera_is_ready_to_capture() == TRUE)
	{
		/* if not self timer, capture the image */
		if(g_camera_cntx.setting.delay_timer == CAMERA_SETTING_DELAY_TIMER_OFF)
		{
			mmi_camera_sublcd_capture();
		}
		else
		{
			CAMERA_ENTER_STATE(CAMERA_STATE_SUB_COUNTDOWN);
		}
	}

	/* flush key event */
	ClearKeyEvents();
}
#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */




/*****************************************************************************
*
*	Utility drawing functions
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_camera_draw_style_text()
* DESCRIPTION
*   draw style text
* PARAMETERS
*	 str				IN		string
*	 offset_x	 	IN	 	offset x
*	 offset_y		IN		offset y
*	 style_txt		IN    text style
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_camera_draw_style_text(PS8 str, S32 offset_x, S32 offset_y, camera_style_text_struct *style_txt)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	if(r2lMMIFlag)
	{
		offset_x += coolsand_UI_get_string_width((UI_string_type)str);
	}
	
	if(style_txt->style == CAMERA_TEXT_STYLE_NORMAL)
	{

		coolsand_UI_move_text_cursor(offset_x, offset_y);
		coolsand_UI_set_text_color(coolsand_UI_color(style_txt->r, style_txt->g ,style_txt->b));
	  	coolsand_UI_print_text((UI_string_type)str);	
	}
	else if(style_txt->style == CAMERA_TEXT_STYLE_BORDER)
	{
		coolsand_UI_move_text_cursor(offset_x, offset_y);
		coolsand_UI_set_text_color(coolsand_UI_color(style_txt->r, style_txt->g, style_txt->b));
		coolsand_UI_set_text_border_color(coolsand_UI_color(style_txt->style_r, style_txt->style_g, style_txt->style_b));
	   coolsand_UI_print_bordered_text((UI_string_type)str);
	}
	else if(style_txt->style == CAMERA_TEXT_STYLE_SHADOW)
	{
		coolsand_UI_move_text_cursor(offset_x+1, offset_y+1);
		coolsand_UI_set_text_color(coolsand_UI_color(style_txt->style_r, style_txt->style_g ,style_txt->style_b));
	   coolsand_UI_print_text((UI_string_type)str);	

		coolsand_UI_move_text_cursor(offset_x, offset_y);	
		coolsand_UI_set_text_color(coolsand_UI_color(style_txt->r, style_txt->g ,style_txt->b));
	   coolsand_UI_print_text((UI_string_type)str);	
	}
	else
	{
		MMI_ASSERT(0);
	} 
	
}




/*****************************************************************************
*
*	Touch Screen Support
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_touch_scr_pen_down_hdlr()
* DESCRIPTION
*   touch screen hdlr when pen down
* PARAMETERS
*	 pos		IN			pen down position
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_TOUCH_SCREEN__) && defined(__MMI_TOUCH_SCREEN__)
static void mmi_camera_touch_scr_pen_down_hdlr(mmi_pen_point_struct pos)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
	{
		S32 tmp;
		tmp = pos.x;
		pos.x= pos.y;
		pos.y = (LCD_WIDTH-1) - tmp;
	}

		mmi_trace(g_sw_CAMERA, "  mmi_camera_touch_scr_pen_down_hdlr() g_camera_cntx.app_state=%d",g_camera_cntx.app_state);

	if(g_camera_cntx.app_state == CAMERA_STATE_OSD_MENU)
	{
		#ifdef __CAMERA_FEATURE_OSD_MENU__
			/* call osd menu to handle all touch screen action */
			mmi_camera_osd_menu_touch_scr_pen_down(pos.x, pos.y);
			g_camera_cntx.touch_object = CAMERA_TOUCH_NONE;
			return;
		#else
			MMI_ASSERT(0);
		#endif /* __CAMERA_FEATURE_OSD_MENU__ */
	}
	else
	{
		mmi_trace(g_sw_CAMERA, "  mmi_camera_touch_scr_pen_down_hdlr() g_camera_cntx.(g_camera_cntx.is_fullscreen) || (!g_camera_cntx.active_camera_osd_ptr->softkey.is_draw_mmi_softkey)=%d",(g_camera_cntx.is_fullscreen) || (!g_camera_cntx.active_camera_osd_ptr->softkey.is_draw_mmi_softkey));
#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

		/* other screen use generic touch screen handler */
		if( (g_camera_cntx.is_fullscreen) || (!g_camera_cntx.active_camera_osd_ptr->softkey.is_draw_mmi_softkey) )
		{
			/* LSK */
			if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_lsk) == MMI_TRUE)
			{
				switch(g_camera_cntx.app_state)
				{
					/* main lcd */
					case CAMERA_STATE_PREVIEW:
					case CAMERA_STATE_COUNTDOWN:					
					case CAMERA_STATE_CONTSHOT:
					case CAMERA_STATE_BURSTSHOT:
						mmi_camera_preview_lsk_press();
						break;

					case CAMERA_STATE_SAVE_CONFIRM:
						mmi_camera_save_confirm_lsk_press();
						break;

					case CAMERA_STATE_BURSTSHOT_SAVE_CONFIRM:
						#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
							mmi_camera_burstshot_save_confirm_lsk_press();
						#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */
						break;
							
					case CAMERA_STATE_OSD_MENU:
						#ifdef __CAMERA_FEATURE_OSD_MENU__
							mmi_camera_osd_menu_lsk_press();		
						#endif /* __CAMERA_FEATURE_OSD_MENU__ */
						break;						
						
					/* sub lcd */
					case CAMERA_STATE_SUB_PREVIEW:	
					case CAMERA_STATE_SUB_COUNTDOWN:			
					case CAMERA_STATE_SUB_CONTSHOT:
					case CAMERA_STATE_SUB_BURSTSHOT:
					case CAMERA_STATE_SUB_BURSTSHOT_SAVING:
						#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
							mmi_camera_sublcd_preview_lsk_press();
						#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */							
						break;		
				}

				g_camera_cntx.touch_object = CAMERA_TOUCH_LSK;
				return;
			}
			
			/* RSK */
			if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_rsk) == MMI_TRUE)
			{
				switch(g_camera_cntx.app_state)
				{
					/* main lcd */
					case CAMERA_STATE_PREVIEW:
					case CAMERA_STATE_COUNTDOWN:					
					case CAMERA_STATE_CONTSHOT:
					case CAMERA_STATE_BURSTSHOT:
						mmi_camera_preview_rsk_press();
						break;

					case CAMERA_STATE_SAVE_CONFIRM:
						mmi_camera_save_confirm_rsk_press();
						break;

					
					case CAMERA_STATE_BURSTSHOT_SAVE_CONFIRM:
						#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
							mmi_camera_burstshot_save_confirm_rsk_press();
						#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */
						break;
					
					/* sub lcd */
					case CAMERA_STATE_SUB_PREVIEW:	
					case CAMERA_STATE_SUB_COUNTDOWN:			
					case CAMERA_STATE_SUB_CONTSHOT:
					case CAMERA_STATE_SUB_BURSTSHOT:
					case CAMERA_STATE_SUB_BURSTSHOT_SAVING:
						#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
							mmi_camera_sublcd_preview_rsk_press();
						#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */								
						break;		
				}
				g_camera_cntx.touch_object = CAMERA_TOUCH_RSK;
				return;
			}
			
		}
		else
		{
			if(mmi_wgui_test_lsk_position(pos) == MMI_TRUE)
			{
				/* draw clicked lsk */
				mmi_camera_preview_lsk_press();
				g_camera_cntx.touch_object = CAMERA_TOUCH_LSK;
				return;
			}
			if(mmi_wgui_test_rsk_position(pos) == MMI_TRUE)
			{
				/* draw clicked rsk */
				mmi_camera_preview_rsk_press();
				g_camera_cntx.touch_object = CAMERA_TOUCH_RSK;
				return;
			}
		}

		if(g_camera_cntx.app_state ==CAMERA_STATE_PREVIEW && g_camera_cntx.is_capturing==FALSE)
		{
			/* osd test */
			#define OSD_ICON_TOUCH_TEST(a,A)\
			do {\
				if(g_camera_cntx.active_camera_osd_ptr->a.is_show)\
				{\
					mmi_trace(g_sw_CAMERA, "  mmi_camera_touch_scr_pen_down_hdlr() pos(%d,%d) (x1,y1,x2,y2)(%d,%d,%d,%d)",pos.x,pos.y,g_camera_cntx.touch_##a.offset_x,g_camera_cntx.touch_##a.offset_y,g_camera_cntx.touch_##a.offset_x +g_camera_cntx.touch_##a.width,g_camera_cntx.touch_##a.offset_y + g_camera_cntx.touch_##a.height);\
					if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_##a) == MMI_TRUE)\
					{\
						mmi_camera_preview_##a##_hotkey_press();\
						g_camera_cntx.touch_object = CAMERA_TOUCH_##A;\
						return;\
					}\
				}\
			} while(0);
	
			if(!g_camera_cntx.is_hide_osd)
			{

				#ifdef __CAMERA_FEATURE_EFFECT__
					OSD_ICON_TOUCH_TEST(effect,EFFECT);		
				#endif

				#ifdef __CAMERA_FEATURE_WB__
					OSD_ICON_TOUCH_TEST(wb,WB);
				#endif

				#ifdef __CAMERA_FEATURE_FLASH__
					OSD_ICON_TOUCH_TEST(flash,FLASH);
				#endif

				#ifdef __CAMERA_FEATURE_LED_HIGHLIGHT__
					OSD_ICON_TOUCH_TEST(led_highlight,LED_HIGHLIGHT);
				#endif

				#ifdef __CAMERA_FEATURE_NIGHT__
					mmi_trace(g_sw_CAMERA," mmi_camera_touch_scr_pen_down_hdlr__CAMERA_FEATURE_NIGHT__");
					OSD_ICON_TOUCH_TEST(night,NIGHT);
				#endif 

				mmi_trace(g_sw_CAMERA," mmi_camera_touch_scr_pen_down_hdlr image size");
				OSD_ICON_TOUCH_TEST(image_size,IMAGE_SIZE);
				mmi_trace(g_sw_CAMERA," mmi_camera_touch_scr_pen_down_hdlr image quality");
				OSD_ICON_TOUCH_TEST(image_qty,IMAGE_QTY);

				#ifdef __CAMERA_FEATURE_CONT_SHOT__
					mmi_trace(g_sw_CAMERA," mmi_camera_touch_scr_pen_down_hdlr __CAMERA_FEATURE_CONT_SHOT__");
					OSD_ICON_TOUCH_TEST(cont_shot,CONT_SHOT);
				#endif

				#ifdef __CAMERA_FEATURE_DELAY_TIMER__
					mmi_trace(g_sw_CAMERA," mmi_camera_touch_scr_pen_down_hdlr __CAMERA_FEATURE_DELAY_TIMER__");
					OSD_ICON_TOUCH_TEST(delay_timer,DELAY_TIMER);
				#endif

				#ifdef __CAMERA_FEATURE_SHOT_MODE__
					OSD_ICON_TOUCH_TEST(shot_mode,SHOT_MODE);
				#endif

				#ifdef __CAMERA_FEATURE_ISO__
					OSD_ICON_TOUCH_TEST(iso,ISO);
				#endif

				#ifdef __CAMERA_FEATURE_AE_METER__
					OSD_ICON_TOUCH_TEST(ae_meter,AE_METER);		
				#endif

				#ifdef __CAMERA_FEATURE_STORAGE__
					OSD_ICON_TOUCH_TEST(storage,STORAGE);
				#endif
#if defined(__MMI_MAINLCD_220X176__)
					OSD_ICON_TOUCH_TEST(contrast,CONTRAST);

#endif
				mmi_trace(g_sw_CAMERA," mmi_camera_touch_scr_pen_down_hdlr ev inc");
				mmi_trace(g_sw_CAMERA, "  mmi_camera_touch_scr_pen_down_hdlr() pos(%d,%d) (x1,y1,x2,y2)(%d,%d,%d,%d)",pos.x,pos.y,g_camera_cntx.touch_ev_inc.offset_x,g_camera_cntx.touch_ev_inc.offset_y,g_camera_cntx.touch_ev_inc.offset_x +g_camera_cntx.touch_ev_inc.width,g_camera_cntx.touch_ev_inc.offset_y + g_camera_cntx.touch_ev_inc.height);

				#if 0
				/* ev inc test */
				if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_ev_inc) == MMI_TRUE)
				{
					mmi_camera_preview_ev_inc_key_press();
					g_camera_cntx.touch_object = CAMERA_TOUCH_EV_INC;
					return;
				}		

				/* ev dec test */
				if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_ev_dec) == MMI_TRUE)
				{
					mmi_camera_preview_ev_dec_key_press();
					g_camera_cntx.touch_object = CAMERA_TOUCH_EV_DEC;
					return;
				}		
				#else //it's just for brightness,not ev
				/* ev inc test */
				if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_ev_inc) == MMI_TRUE)
				{
					mmi_camera_preview_bt_inc_key_press();
					g_camera_cntx.touch_object = CAMERA_TOUCH_EV_INC;
					return;
				}		

				/* ev dec test */
				if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_ev_dec) == MMI_TRUE)
				{
					mmi_camera_preview_bt_dec_key_press();
					g_camera_cntx.touch_object = CAMERA_TOUCH_EV_DEC;
					return;
				}		
				#endif
				
				/* zoom inc test */
				if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_zoom_inc) == MMI_TRUE)
				{
					mmi_camera_preview_up_arrow_press();
					g_camera_cntx.touch_object = CAMERA_TOUCH_ZOOM_INC;
					return;
				}		

				/* zoom dec test */
				if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_zoom_dec) == MMI_TRUE)
				{
					mmi_camera_preview_down_arrow_press();
					g_camera_cntx.touch_object = CAMERA_TOUCH_ZOOM_DEC;
					return;
				}		
			}
			
			/* capture */
			if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_capture) == MMI_TRUE)
			{
				mmi_camera_capture_button_press();
				g_camera_cntx.touch_object = CAMERA_TOUCH_CAPTURE;
				return;
			}		
		}		
	}
	
	g_camera_cntx.touch_object = CAMERA_TOUCH_NONE;

}
#endif /* __CAMERA_FEATURE_TOUCH_SCREEN__ && __MMI_TOUCH_SCREEN__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_touch_scr_pen_up_hdlr()
* DESCRIPTION
*   touch screen hdlr when pen up
* PARAMETERS
*	 pos		IN			pen up position
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_TOUCH_SCREEN__) && defined(__MMI_TOUCH_SCREEN__)
static void mmi_camera_touch_scr_pen_up_hdlr(mmi_pen_point_struct pos)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
	{
		S32 tmp;
		tmp = pos.x;
		pos.x= pos.y;
		pos.y = (LCD_WIDTH-1) - tmp;
	}

	
	if(g_camera_cntx.app_state == CAMERA_STATE_OSD_MENU)
	{
		#ifdef __CAMERA_FEATURE_OSD_MENU__
			if(g_camera_cntx.touch_object == CAMERA_TOUCH_NONE)
			{
				mmi_camera_osd_menu_touch_scr_pen_up(pos.x, pos.y);
			}
		#else
			MMI_ASSERT(0);
		#endif /* __CAMERA_FEATURE_OSD_MENU__ */
	}
	else
	{
#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

		switch (g_camera_cntx.touch_object)
		{
			case	CAMERA_TOUCH_LSK:
				if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_lsk) == MMI_TRUE)
				{
					switch(g_camera_cntx.app_state)
					{
						/* main lcd */
						case CAMERA_STATE_PREVIEW:
						case CAMERA_STATE_COUNTDOWN:					
						case CAMERA_STATE_CONTSHOT:
						case CAMERA_STATE_BURSTSHOT:
							mmi_camera_preview_lsk_release();
							break;

						case CAMERA_STATE_SAVE_CONFIRM:
							mmi_camera_save_confirm_lsk_release();
							break;
						
						case CAMERA_STATE_BURSTSHOT_SAVE_CONFIRM:
							#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
								mmi_camera_burstshot_save_confirm_lsk_release();
							#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */							
							break;

						/* sub lcd */
						case CAMERA_STATE_SUB_PREVIEW:	
						case CAMERA_STATE_SUB_COUNTDOWN:			
						case CAMERA_STATE_SUB_CONTSHOT:
						case CAMERA_STATE_SUB_BURSTSHOT:
						case CAMERA_STATE_SUB_BURSTSHOT_SAVING:
							#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
								mmi_camera_sublcd_preview_rsk_release();
							#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */							
							break;		
					}
				}
				break;

			case	CAMERA_TOUCH_RSK:
				if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_rsk) == MMI_TRUE)
				{
					switch(g_camera_cntx.app_state)
					{
						/* main lcd */
						case CAMERA_STATE_PREVIEW:
						case CAMERA_STATE_COUNTDOWN:					
						case CAMERA_STATE_CONTSHOT:
						case CAMERA_STATE_BURSTSHOT:
							mmi_camera_preview_rsk_release();
							break;

						case CAMERA_STATE_SAVE_CONFIRM:
							mmi_camera_save_confirm_rsk_release();
							break;

						case CAMERA_STATE_BURSTSHOT_SAVE_CONFIRM:
							#ifdef __CAMERA_FEATURE_CONT_SHOT_BURST__
								mmi_camera_burstshot_save_confirm_rsk_release();
							#endif /* __CAMERA_FEATURE_CONT_SHOT_BURST__ */							
							break;

						case CAMERA_STATE_OSD_MENU:
							#ifdef __CAMERA_FEATURE_OSD_MENU__
								mmi_camera_osd_menu_rsk_release();	
							#endif /* __CAMERA_FEATURE_OSD_MENU__ */
							break;
							
						/* sub lcd */
						case CAMERA_STATE_SUB_PREVIEW:	
						case CAMERA_STATE_SUB_COUNTDOWN:			
						case CAMERA_STATE_SUB_CONTSHOT:
						case CAMERA_STATE_SUB_BURSTSHOT:
						case CAMERA_STATE_SUB_BURSTSHOT_SAVING:
							#ifdef __CAMERA_FEATURE_SELF_CAPTURE__
								mmi_camera_sublcd_preview_lsk_release();
							#endif /* __CAMERA_FEATURE_SELF_CAPTURE__ */
							break;		
					}
				}
				break;

#if 0
			case	CAMERA_TOUCH_EV_INC:
				if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_ev_inc) == MMI_TRUE)
				{
					mmi_camera_preview_ev_inc_key_release();
				}
				break;

			case	CAMERA_TOUCH_EV_DEC:
				if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_ev_dec) == MMI_TRUE)
				{
					mmi_camera_preview_ev_dec_key_release();
				}
				break;
#else   //it's just for brightness,not ev
			case	CAMERA_TOUCH_EV_INC:
				if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_ev_inc) == MMI_TRUE)
				{
					mmi_camera_preview_bt_inc_key_release();
				}
				break;

			case	CAMERA_TOUCH_EV_DEC:
				if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_ev_dec) == MMI_TRUE)
				{
					mmi_camera_preview_bt_dec_key_release();
				}
				break;
#endif
			case	CAMERA_TOUCH_ZOOM_INC:
				if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_zoom_inc) == MMI_TRUE)
				{
					mmi_camera_preview_up_arrow_release();
				}
				break;

			case	CAMERA_TOUCH_ZOOM_DEC:
				if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_zoom_dec) == MMI_TRUE)
				{
					mmi_camera_preview_down_arrow_release();
				}
				break;				

			default:
				break;
		}
	}
}
#endif /* __CAMERA_FEATURE_TOUCH_SCREEN__ && __MMI_TOUCH_SCREEN__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_touch_scr_pen_move_hdlr()
* DESCRIPTION
*   touch screen hdlr when pen move
* PARAMETERS
*	 pos		IN			pen move position
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_TOUCH_SCREEN__) && defined(__MMI_TOUCH_SCREEN__)
static void mmi_camera_touch_scr_pen_move_hdlr(mmi_pen_point_struct pos)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	BOOL softkey_pressed;
   
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	softkey_pressed = FALSE;
	if(g_camera_cntx.osd_rotate == CAMERA_DISPLAY_ROTATE_270)
	{
		S32 tmp;
		tmp = pos.x;
		pos.x= pos.y;
		pos.y = (LCD_WIDTH-1) - tmp;
	}
#ifdef __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__
    mmi_camera_restart_app_watchdog();
#endif

	switch(g_camera_cntx.touch_object)
	{
		case CAMERA_TOUCH_LSK:
			if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_lsk))
			{
				softkey_pressed = TRUE;
			}
			else
			{
				softkey_pressed = FALSE;
			}
			
			if(softkey_pressed != g_camera_cntx.is_lsk_pressed)
			{
				g_camera_cntx.is_lsk_pressed = softkey_pressed;
				mmi_camera_preview_draw_osd();
				gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
			}
			break;
			
		case CAMERA_TOUCH_RSK:
			if(mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_rsk))
			{
				softkey_pressed = TRUE;
			}
			else
			{
				softkey_pressed = FALSE;
			}
			
			if(softkey_pressed != g_camera_cntx.is_rsk_pressed)
			{
				g_camera_cntx.is_rsk_pressed = softkey_pressed;
				mmi_camera_preview_draw_osd();
				gdi_layer_blt_previous(0, 0, g_camera_cntx.base_UI_device_width-1, g_camera_cntx.base_UI_device_height-1);
			}
			break;

		case	CAMERA_TOUCH_EV_INC:
			if(!mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_ev_inc))
			{
				/* move out, treat as key release */				
#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
				mmi_camera_preview_ev_inc_key_release();
#endif
			}
			break;

		case	CAMERA_TOUCH_EV_DEC:
			if(!mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_ev_dec))
			{
				/* move out, treat as key release */				
#ifdef __CAMERA_FEATURE_ICONBAR_PANEL__
				mmi_camera_preview_ev_dec_key_release();
#endif
			}
			break;

		case	CAMERA_TOUCH_ZOOM_INC:
			if(!mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_zoom_inc))
			{
				/* move out, treat as key release */				
				mmi_camera_preview_up_arrow_release();
			}
			break;

		case	CAMERA_TOUCH_ZOOM_DEC:
			if(!mmi_camera_touch_scr_hit_test(pos.x, pos.y, &g_camera_cntx.touch_zoom_dec))
			{
				/* move out, treat as key release */
				mmi_camera_preview_down_arrow_release();
			}
			break;	

			
		
	}
}
#endif /* __CAMERA_FEATURE_TOUCH_SCREEN__ && __MMI_TOUCH_SCREEN__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_touch_scr_hit_test()
* DESCRIPTION
*   test if is within touch region
* PARAMETERS
*	 pos_x		IN			position x
*	 pos_y		IN			position y
*	 touch		IN			touch object structure
* RETURNS
*   BOOL	
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__CAMERA_FEATURE_TOUCH_SCREEN__) && defined(__MMI_TOUCH_SCREEN__)
static BOOL mmi_camera_touch_scr_hit_test(S32 pos_x, S32 pos_y, camera_ui_touch_struct *touch_obj)
{
 	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	if((pos_x >= touch_obj->offset_x) &&
		(pos_x <  touch_obj->offset_x + touch_obj->width) &&
		(pos_y >= touch_obj->offset_y) &&
		(pos_y <  touch_obj->offset_y + touch_obj->height))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
#endif /* __CAMERA_FEATURE_TOUCH_SCREEN__ && __MMI_TOUCH_SCREEN__ */




/*****************************************************************************
*
*	Screen Snapshot
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_camera_screen_snapshot()
* DESCRIPTION
*   snapshot screen
* PARAMETERS
*	 void
* RETURNS
*   void 	
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_SCREEN_SNAPSHOT__
void mmi_camera_scr_snapshot(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
   if((g_camera_cntx.is_screen_snapshot) || 						/* capture screen shot */
   	(g_camera_cntx.app_state != CAMERA_STATE_EXIT) 		/* in camera app */
   	#if defined(__MMI_VIDEO_RECORDER__)
	 ||	(mmi_vdorec_is_in_app())									/* in video recorder */
	 #endif
	 #if defined(__MMI_VIDEO_PLAYER__)
	 ||	(mmi_vdoply_is_in_app())										/* in video player */	
	 #endif
	 )
   {
		return;
   }

	mmi_camera_entry_scr_snapshot_screen();
   
	
}
#endif /* __MMI_SCREEN_SNAPSHOT__ */



/*****************************************************************************
* FUNCTION
*	 mmi_camera_entry_scr_snapshot_screen()
* DESCRIPTION
*   entry  screen snapshot screen
* PARAMETERS
*	 void
* RETURNS
*   void 	
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_SCREEN_SNAPSHOT__
static void mmi_camera_entry_scr_snapshot_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   gdi_handle 		base_layer;
  	S8					storage_filepath[FMGR_PATH_BUFFER_SIZE];
  	S8					storage_filename[FMGR_PATH_BUFFER_SIZE];  	
  	U16				cur_storage;
  	S32				create_result;
  	MDI_RESULT  	result;
	S8					buffer[25];
	S8					UCS2_buffer[50];
	S8					UCS2_testname[50];
	S8					UCS2_alpha[10];
	S16				error = 0;
	U16				filename_seq_no;
	U16				alpha_count;
	U16				alpha_index;
	FS_HANDLE		file_handle;


   
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	g_camera_cntx.is_screen_snapshot = TRUE;

   /* enter a new screen, force previous screen leave */
	EntryNewScreen(SCR_ID_CAMERA_SCREEN_SNAPSHOT, 
						mmi_camera_exit_scr_snapshot_screen, 
						NULL,  /* entry function is null, will not add to history by framework */
						NULL);

	/* load Setting */
	mmi_camera_load_setting();

	/* allocate storage path buffer */
	memset(storage_filepath, 0, FMGR_PATH_BUFFER_SIZE);
	
	/* check if path is valid or not. if not will pop up a screen after the preview screen is show */
	cur_storage = g_camera_cntx.setting.storage;
	
	/* drv is valid, create directory */
	mmi_camera_get_storage_file_path(storage_filepath);
	create_result = mmi_camera_create_file_dir(storage_filepath);

	/* create directory error check */
	if(create_result == FS_WRITE_PROTECTION)
	{
		DisplayPopup((PU8)GetString(STR_ID_CAMERA_NOTIFY_WRITE_PROTECTION), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
		return;
	}
	else if(create_result == FS_DISK_FULL)
	{
		DisplayPopup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
		return;		
	}
	else if(create_result == FS_ROOT_DIR_FULL)
	{
		DisplayPopup((PU8)GetString(FMGR_FS_ROOT_DIR_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
		return;		
	}	
	else if(create_result == FS_DRIVE_NOT_FOUND)
	{
		DisplayPopup((PU8)GetString(FMGR_FS_DRIVE_NOT_FOUND_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
		return;		
	}
	else if(create_result < 0)
	{
		DisplayPopup((PU8)GetString(STR_ID_CAMERA_NOTIFY_ERROR), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
		return;		
	}


	/* get/store sequentail number */
	ReadValue(NVRAM_SCR_SNAPSHOT_SEQ_NO, &filename_seq_no, DS_SHORT , &error);
	if(filename_seq_no == 0xffff)
	{
		filename_seq_no = 1;	/* init value */
	}
	else 
	{
		filename_seq_no++;
		if(filename_seq_no > 9999)	/* more than 4 digits */
			filename_seq_no = 1;
	}
	WriteValue(NVRAM_SCR_SNAPSHOT_SEQ_NO, &filename_seq_no, DS_SHORT , &error);

	alpha_count		= 'Z'-'A'+1 ;
	alpha_index		= 0;

	sprintf((PS8)buffer, "SCR%04d", filename_seq_no);
	AnsiiToUnicodeString((PS8)UCS2_buffer, (PS8)buffer);	

	while(1)
	{
		sprintf((PS8)buffer, "%c", 'A'+alpha_index);
		AnsiiToUnicodeString((PS8)UCS2_alpha, (PS8)buffer);

		pfnUnicodeStrcpy(UCS2_testname, UCS2_buffer);
		pfnUnicodeStrcat(UCS2_testname, UCS2_alpha);
		pfnUnicodeStrcat(UCS2_testname, (PS8)L".jpg");

		pfnUnicodeStrcpy((PS8)storage_filename, (PS8)storage_filepath);
		pfnUnicodeStrcat((PS8)storage_filename, (PS8)UCS2_testname);

		file_handle = MMI_FS_Open((U16*)storage_filename, FS_READ_ONLY);
	
		if(file_handle >= 0)
		{
			/* file exist */
			MMI_FS_Close(file_handle);
			alpha_index++;	
		}
		else
		{
			break;
		}

		/* exit condition - will get a IMGXXXXZ.jpg as file name */
		if(alpha_index >= alpha_count-1)
			break;
	}

	/* play capture sound */
	mmi_camera_capture_play_sound();

	/* encode layer */
	gdi_layer_get_base_handle(&base_layer);
	result = mdi_camera_encode_layer_to_jpeg(base_layer, storage_filename);

	switch(result)
	{
		case MDI_RES_CAMERA_SUCCEED:
			/* use storage_filepath as tmp buffer */
			pfnUnicodeStrcpy(storage_filepath, (PS8)GetString(STR_ID_CAMERA_NOTIFY_SCREEN_CAPTURED));
			pfnUnicodeStrcat(storage_filepath, (PS8)L"\n");	
			pfnUnicodeStrcat(storage_filepath, (PS8)UCS2_testname);		
			
			DisplayPopup((PU8)storage_filepath, IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
			return;
			break;

		case MDI_RES_CAMERA_ERR_DISK_FULL:
			DisplayPopup((PU8)GetString(STR_ID_CAMERA_NOTIFY_DISK_FULL), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			return;				
			break;

		case MDI_RES_CAMERA_ERR_WRITE_PROTECTION:
			DisplayPopup((PU8)GetString(STR_ID_CAMERA_NOTIFY_WRITE_PROTECTION), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			return;				
			break;
			
		case MDI_RES_CAMERA_ERR_NO_DISK:
			DisplayPopup((PU8)GetString(STR_ID_CAMERA_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			return;				
			break;

		default:
			DisplayPopup((PU8)GetString(STR_GLOBAL_NOT_DONE), IMG_GLOBAL_ERROR, 1, ST_NOTIFYDURATION, WARNING_TONE);
			return;				
			break;
	}

}
#endif /* __MMI_SCREEN_SNAPSHOT__ */


/*****************************************************************************
* FUNCTION
*	 mmi_camera_exit_scr_snapshot_screen()
* DESCRIPTION
*   exit  screen snapshot screen
* PARAMETERS
*	 void
* RETURNS
*   void 	
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_SCREEN_SNAPSHOT__
static void mmi_camera_exit_scr_snapshot_screen(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	g_camera_cntx.is_screen_snapshot = FALSE;
}
#endif /* __MMI_SCREEN_SNAPSHOT__ */


#if defined(__MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__)
/*****************************************************************************
 * FUNCTION
 *  mmi_camera_restart_app_watchdog
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_camera_restart_app_watchdog(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_cancel_timer(mmi_camera_app_watchdog_callback);
    g_camera_cntx.app_timer_count = 0;
    gui_start_timer(MMI_CAMERA_APP_POLLING_DURATION,mmi_camera_app_watchdog_callback);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_camera_stop_app_watchdog
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_camera_stop_app_watchdog(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_cancel_timer(mmi_camera_app_watchdog_callback);
    g_camera_cntx.app_timer_count = 0;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_camera_app_watchdog_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_camera_app_watchdog_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_camera_cntx.app_timer_count += MMI_CAMERA_APP_POLLING_DURATION;

    if (g_camera_cntx.app_timer_count < MMI_CAMERA_APP_WATCHDOG_DURATION)
    {
        gui_start_timer(MMI_CAMERA_APP_POLLING_DURATION, mmi_camera_app_watchdog_callback);        
    }
    else
    {
        GoBackHistory();
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_camera_start_delete_confirm_watchdog
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_camera_start_delete_confirm_watchdog(void)
{   
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_camera_cntx.delete_confirm_timer_count = 0;
    gui_start_timer(MMI_CAMERA_APP_POLLING_DURATION,mmi_camera_delete_confirm_watchdog_timer_callback);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_camera_delete_confirm_watchdog_timer_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_camera_delete_confirm_watchdog_timer_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_camera_cntx.delete_confirm_timer_count += MMI_CAMERA_DELETE_CONFIRM_POLLING_DURATION ;

    if (g_camera_cntx.delete_confirm_timer_count < MMI_CAMERA_DELETE_CONFIRM_WATCHDOG_DURATION )
    {
        gui_start_timer(MMI_CAMERA_DELETE_CONFIRM_POLLING_DURATION , mmi_camera_delete_confirm_watchdog_timer_callback);        
    }
    else
    {
//        if (g_camera_cntx.is_external_request == FALSE)
//        {
//            CAMERA_ENTER_STATE(CAMERA_STATE_PREVIEW);
//        }
//        else 
        {
            /* for External Request */
 //           g_camera_cntx.callback_ptr(MMI_TRUE,g_camera_cntx.captured_filepath);
            GoBackHistory();
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_camera_stop_delete_confirm_watchdog
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_camera_stop_delete_confirm_watchdog(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_cancel_timer(mmi_camera_delete_confirm_watchdog_timer_callback);
}
#endif /* __MMI_CAMERA_FEATURE_WATCHDOG_TIMEOUT__ */

void mmi_camapp_clean_app(void)
{
    //   DeleteScreenIfPresent(SCR_ID_ANALOGTV_APP);
    if(IsScreenPresent( SCR_ID_CAMERA_APP))
    {
        DeleteUptoScrID(IDLE_SCREEN_ID);
    }
}

static void (*mmi_current_app_entry_function) (void)=NULL;
void mmi_exit_bg_qq_and_entry_app(void)
{
    gdi_layer_lock_frame_buffer();
	clear_left_softkey();
	clear_right_softkey();
#if defined( __MMI_SUPPORT_JME__)
      exitSmarchBox();
#else
	#ifdef MMI_ON_HARDWARE_P
       gcj_NativeJavaQuit(1);

	while (gcj_isJavaLife())
   		COS_Sleep(1000);
	#endif
  #endif

    gdi_layer_unlock_frame_buffer();
	
    if(mmi_current_app_entry_function)
        mmi_current_app_entry_function();

    mmi_current_app_entry_function = NULL;   

}
void mmi_show_qq_select_confirm(void (*default_entry) (void))
{
#if defined( __MMI_SUPPORT_JME__) 

            smarchbox_bg_askquit();
 SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

#elif defined (__NATIVE_JAVA__)

	DisplayConfirm(STR_GLOBAL_OK,
						0,
						STR_GLOBAL_BACK,
						0,
						get_string(STR_ID_CAMERA_NOTIFY_QQ_QUIT),
						IMG_GLOBAL_QUESTION,
						WARNING_TONE);
    mmi_current_app_entry_function = default_entry;
	SetLeftSoftkeyFunction(mmi_exit_bg_qq_and_entry_app,KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
   
#endif    
}
#endif /* __MMI_CAMERA__ */


