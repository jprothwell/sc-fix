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
 * FactoryModeSrc.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is Factory Mode
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
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "mmi_features.h" 

#ifdef __MMI_FACTORY_MODE__ 


/*****************************************************************************
* Include Files                                                                
*****************************************************************************/
#define __NEWSIMULATOR 
#include "stdc.h"
#include "l4dr1.h"
#include "globaldefs.h"
#include "globalscrenum.h"
#include "custdatares.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "protocolevents.h"
#include "custmenures.h"
#include "wgui_categories.h"
#ifdef __MMI_SUBLCD__
#include "wgui_categories_sublcd.h"
#endif
#include "hardwareevents.h"
#include "thrdosinterface.h"
#include "osthreadinterfacegprot.h"
#include "allappgprot.h"
#include "pixcomfontengine.h" 
#include "timerevents.h"
#include "mainmenudef.h"
#include "mmi_data_types.h"
#include "customcfg.h"
#include "unicodexdcl.h"
#include "audioinc.h"
#include "idleappdef.h"
#include "factorymodedef.h"
#include "factorymodeprot.h"
#include "engineermodeprot.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "alarmdef.h"
#include "gpioinc.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "commonscreens.h"
#include "settingdefs.h"					/* st_notifyduration define */
#include "nvramenum.h"
#include "keybrd.h"							/* keypad tone related interface */
#include "fmt_struct.h"
#include "gdi_include.h"
#include "profilegprots.h"
#include "mmi_trace.h"
#ifdef __MMI_SUBLCD__
#include	"sublcdhistorygprot.h"
#endif
#ifdef MMI_ON_HARDWARE_P
#include "calib_m.h"
#include "pmd_m.h"
#endif
//wangrui delete the micro for fix bug 10484 	
//#if defined(__MSDC_MS__) || defined(__MSDC_SD_MMC__)|| defined(__MSDC_MSPRO__)
#include "msdc_def.h"
//#endif

#if defined(__MMI_CAMERA__)
	#ifdef MMI_ON_HARDWARE_P
		#include "kal_release.h"
	#else
		#include "kal_non_specific_general_types.h"
	#endif /* MMI_ON_HARDWARE_P */

	#include "lcd_if.h"		
	#include "lcd_sw_rnd.h"						/* lcd layer enable flag */
	#include "mmi_data_types.h"
	#include "gdi_include.h"					/* graphic library */
	#include	"mdi_datatype.h"
	#include "med_api.h"							/* media task camera module */
	#include "mmi_features_camera.h"	
	#include "resource_camera_skins.h"
	#include "mdi_camera.h"
	#include "cameraapp.h"
	#include "wgui_categories_multimedia.h"
	#include "wgui_categories_inputs.h"
	#include "sublcdhistorygprot.h"
//	#include "camera_para.h"

	#ifndef MMI_ON_WIN32
		#include "nvram_user_defs.h"
	#endif /* MMI_ON_WIN32 */		
	
	#if defined(ISP_SUPPORT)
		#include "image_sensor.h"
	#endif /* ISP_SUPPORT */
#endif


#ifdef MMI_ON_HARDWARE_P
	#include "device.h"
	#include "custom_em.h"
	#include "custom_equipment.h"
	#include "custom_hw_default.h"
	#include "nvram_data_items.h"
	#include "nvram_user_defs.h"
	#include "lcd_sw.h"
	#include "lcd_sw_inc.h"
	#include "custom_jump_tbl.h"
	#include "resource_verno.h"
	#include "uart_sw.h"    						/* uart_port type */
	#include "csw_ver.h"
	#ifndef WIN32
		#include "l1audio.h"
	#else
		#define L1SP_Tones kal_uint16
	#endif
      #include "tgt_m.h"
	#include "csw_csp.h"
	#include "csw.h"
	#include "drv_flash.h"

#else
	#include "custom_em.h"
	#include "custom_equipment.h"
#endif
#undef __NEWSIMULATOR 

 
#ifndef MMI_ON_WIN32   //luanyy modify
#include "engineermodepatch.h"
#include "kal_release.h"
#include "uart_sw.h"
#include "hal_mem_map.h"
#include "mci.h"
#else
//#include "engineermodedummyinc.h"
#include "kal_release.h"
#include "uart_sw.h"
#endif
#include "sscstringhandle.h"
extern kal_uint16 UART_PutBytes(UART_PORT port, 
                                                      kal_uint8 *Buffaddr, 
                                                      kal_uint16 Length, 
                                                      module_type ownerid);
extern kal_uint16 UART_GetBytes(UART_PORT port, 
	                                                 kal_uint8 *Buffaddr, 
	                                                 kal_uint16 Length, 
	                                                 kal_uint8 *status, 
	                                                 module_type ownerid);
extern module_type UART_GetOwnerID(UART_PORT port);
extern void UART_SetOwner (UART_PORT port, kal_uint8 owner);
extern void Media_Stop( void );
int PSI_LCDTpye = 0;
BOOL g_test_without_intervenor = FALSE;
/***************************************************************************** 
* Define
*****************************************************************************/

/***************************/
/* Headset                 */
/***************************/
#define HEADSET_MIC_TEST 1

/***************************/
/* Camera						*/
/***************************/
#define FM_CAMERA_OSD_BUFFER_SIZE	(((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3)) /* single buffer */ 

/* camera support for Backend module or ISP */
#if defined(__MMI_CAMERA__) && defined(MMI_ON_HARDWARE_P)
	#if defined(ISP_SUPPORT)
		/* using own ISP, support tunning */
		#define __FM_CAMERA_TUNNING__
		#define __FM_CAMERA_PREVIEW__
	#else
		/* using backend, only support preview */
		#define __FM_CAMERA_PREVIEW__
	#endif 
#endif /* __MMI_CAMERA__ && ISP_SUPPORT */

#if (UI_DEVICE_HEIGHT == 128)
	#define CAMTUNE_PREVIEW_HEIGHT_COUNT 3
#elif (UI_DEVICE_HEIGHT == 160)
	#define CAMTUNE_PREVIEW_HEIGHT_COUNT 4
#elif (UI_DEVICE_HEIGHT == 220)
	#define CAMTUNE_PREVIEW_HEIGHT_COUNT 5
#else
	#define CAMTUNE_PREVIEW_HEIGHT_COUNT 6
#endif 

#define CAMTUNE_GROUP_NAME_LEN			(20)			/* group name length */
#define CAMTUNE_GROUP_NAME_BUF_LEN		((CAMTUNE_GROUP_NAME_LEN-1)*ENCODING_LENGTH)

#define CAMTUNE_ITEM_PER_GROUP			(15)
#define CAMTUNE_ITEM_NAME_LEN				(20)
#define CAMTUNE_ITEM_NAME_BUF_LEN		((CAMTUNE_ITEM_NAME_LEN-1)*ENCODING_LENGTH)
#define CAMTUNE_ITEM_DIG_LEN				(15)
#define CAMTUNE_ITEM_DIG_BUF_LEN			((CAMTUNE_ITEM_DIG_LEN-1)*ENCODING_LENGTH)


/***************************/
/* KeyPad						*/
/***************************/
#define PHYSICAL_KEYs (sizeof(IdleScreenDigits)/sizeof(IdleScreenDigits[0]))
#define KEYNUMs (sizeof(keypad_value)/sizeof(keypad_value[0]))
#if defined(__MMI_FACTORY_MODE_NAV_KEY__) ||defined(__MMI_WITH_C_KEY__)||defined(__MMI_MAINLCD_220X176__) ||defined(__MMI_BLACKBERRY_QWERTY__)|| defined(__PROJECT_GALLITE_C01__)
	#define LINE_NUM     8
#else
	#define LINE_NUM     7
#endif
#if defined(TGT_GALLITE_G800)||defined(TGT_TD_4SIM)||defined(__MMI_PROJECT_GS_HK603__)
#ifdef LINE_NUM
#undef LINE_NUM
#define LINE_NUM 8
#endif
#endif
#ifdef __MMI_BLACKBERRY_QWERTY__
#define COLUMN_NUM   6
#else
#define COLUMN_NUM   4
#endif
#define KEY_NA       2
#define KEY_HIDE     1
#define KEY_SHOW     0
#define BLANK        "   "

#define KEY_NONE_NAME			BLANK
#define KEY_NONE_VALUE			0xFF
#define KEY_LSK_NAME			"LSK  "
#define KEY_LSK_VALUE			KEY_LSK
#define KEY_RSK_NAME			"RSK  "
#define KEY_RSK_VALUE			KEY_RSK
#define KEY_UP_ARROW_NAME		"Up  "
#define KEY_UP_ARROW_VALUE		KEY_UP_ARROW
#define KEY_DOWN_ARROW_NAME		"Dn  "
#define KEY_DOWN_ARROW_VALUE	KEY_DOWN_ARROW
#define KEY_LEFT_ARROW_NAME		"LF  "
#define KEY_LEFT_ARROW_VALUE	KEY_LEFT_ARROW
#define KEY_RIGHT_ARROW_NAME	"RT  "
#define KEY_RIGHT_ARROW_VALUE	KEY_RIGHT_ARROW
#define KEY_SEND_NAME			"Snd  "
#define KEY_SEND_VALUE			KEY_SEND
#ifdef __SENDKEY2_SUPPORT__
#define KEY_SEND1_NAME 			"Snd1  "
#define KEY_SEND1_VALUE 		KEY_SEND1
#define KEY_SEND2_NAME 			"Snd2  "
#define KEY_SEND2_VALUE 		KEY_SEND2
#define KEY_SEND3_NAME 			"Snd3  "
#define KEY_SEND3_VALUE 		KEY_SEND3
#endif


#define KEY_END_NAME				"End "
#define KEY_END_VALUE			KEY_END

#ifdef __MMI_NO_SIDEKEY__
	#define KEY_VOL_UP_NAME		KEY_NONE_NAME
	#define KEY_VOL_UP_VALUE	KEY_NONE_VALUE
	#define KEY_VOL_DOWN_NAME	KEY_NONE_NAME
	#define KEY_VOL_DOWN_VALUE	KEY_NONE_VALUE
#else
	#define KEY_VOL_UP_NAME		"Vup "
	#define KEY_VOL_UP_VALUE	KEY_VOL_UP
	#define KEY_VOL_DOWN_NAME	"Vdn "
	#define KEY_VOL_DOWN_VALUE	KEY_VOL_DOWN
#endif

#if defined(__MMI_IP_KEY__) || defined(__MMI_FACTORY_MODE_I_KEY__)
	#define KEY_IP_NAME		" I "
	#define KEY_IP_VALUE		KEY_ENTER
#else
	#define KEY_IP_NAME		KEY_NONE_NAME
	#define KEY_IP_VALUE		KEY_NONE_VALUE
#endif

#if 0//defined(__MMI_CAMERA_KEY__)//JIASHUO DELETE,because the camerkey is the same as okkey on hardware
	#define KEY_CAMERA_NAME		"CAM "
	#define KEY_CAMERA_VALUE	KEY_CAMERA
#else
	#define KEY_CAMERA_NAME		KEY_NONE_NAME
	#define KEY_CAMERA_VALUE	KEY_NONE_VALUE
#endif

#if defined(__MMI_FACTORY_MODE_NAV_KEY__)
	#define KEY_OK_NAME		"OK "
	#define KEY_OK_VALUE		KEY_LSK
#else
	/*	wangrui modify 2008.10.19 begin for fixbug 10083*/
	#define KEY_OK_NAME		"OK "
	#define KEY_OK_VALUE		KEY_ENTER
	/*	wangrui modify 2008.10.19 end*/
#endif

#if defined(__MMI_WITH_C_KEY__) 
	#define KEY_CLR_NAME		"CLR"
	#define KEY_CLR_VALUE	KEY_CLEAR
#else
	#define KEY_CLR_NAME		KEY_NONE_NAME
	#define KEY_CLR_VALUE	KEY_NONE_VALUE
#endif

#ifdef PRJ_F021
	#define KEY_FUNC_NAME  "  FUNC"
	#define KEY_FUNC_VALUE  KEY_FM
#endif
#if defined(__PROJECT_GALLITE_C01__)
const char sw_version[] = SW_VERSION_STR;
const char hw_version[] = HW_VERSION_STR;
#endif
/***************************************************************************** 
* Typedef 
*****************************************************************************/
typedef enum {
	FM_CAMERA_SENSOR,
	FM_CAMERA_ISP
} fm_camtune_cat_enum;


typedef struct {
	S32	value;
	S32	min_value;
	S32	max_value;
	BOOL	is_read_only;
	BOOL	is_true_false;
	BOOL  is_need_restart;
	S8	 	name_buf[CAMTUNE_ITEM_NAME_BUF_LEN];
	S8		digit_buf[CAMTUNE_ITEM_DIG_BUF_LEN];
} fm_camtune_item_struct;


typedef struct {
	S32							item_count;
	S32 							item_inx;
	S8								name_buf[CAMTUNE_GROUP_NAME_BUF_LEN];
	fm_camtune_item_struct 	items[CAMTUNE_ITEM_PER_GROUP];
} fm_camtune_group_struct;


typedef struct {
	S32	group_count;
	S32 	group_idx;
	
	S32 	category_idx;
	
	S32	hilight_idx;
	
	gdi_handle	preview_layer_handle;
	PU8			preview_layer_buf_ptr;

	BOOL			is_previewing;
	BOOL			is_fullscreen;

	UI_string_type					true_false_str[2];
	fm_camtune_group_struct 	group_data;
#ifdef __MMI_CAMERA__
	mdi_camera_setting_struct	camera_setting_data;
#endif
} fm_camtune_cntx_struct;



typedef enum {

      MAINLCD_BL = 0,

#ifdef __MMI_SUBLCD__
      SUBLCD_BL,
#endif

      KEYPAD_LED,

#ifndef __MMI_HIDE_STATUS_LED_R__
      STATUSLED_R,
#endif

#ifndef __MMI_HIDE_STATUS_LED_G__
      STATUSLED_G,
#endif

#ifndef __MMI_HIDE_STATUS_LED_B__
      STATUSLED_B,
#endif

      MAX_FM_LED_MENU_NUM
} FM_LED_MENU_ENUM;


/****************************************************************************
* Global Variable                                                           
*****************************************************************************/
extern U8 		subMenuData[MAX_SUB_MENUS][MAX_SUB_MENU_SIZE];
extern PU8 		subMenuDataPtrs[MAX_SUB_MENUS];
extern bitmap	*current_LCD_device_bitmap;
extern bitmap	main_LCD_device_bitmap;
extern bitmap	sub_LCD_device_bitmap;
extern U8 		gCurrentMode;
extern S32 		MAIN_LCD_device_width;
extern S32 		MAIN_LCD_device_height;
extern S16 		gAutoTestListSize;
extern PU8		AutoTestSelectedItems[MAX_AUTO_TEST_ITEMS];

extern rtc_format_struct 	myclocktime;
extern AutoTestResultArray nvramTestResultArray; 
extern AutoTestItemArray 	nvramTestItemArray;


#if defined(__FM_CAMERA_TUNNING__) || defined(__FM_CAMERA_PREVIEW__)
	extern camera_context_struct g_camera_cntx;
	extern const U8 camera_ev_command_map[];
	extern const U8 camera_zoom_command_map[];
	extern const U8 camera_effect_command_map[];
	extern const U8 camera_wb_command_map[];
	extern const U8 camera_banding_command_map[];
	extern const U8 camera_jpg_size_command_map[];
	extern const U8 camera_image_qty_command_map[]; 
/*wuzc Del Start For GDI/GUI Portting Ver:    on 2008-3-14 9:41 */
	#if 0
	extern gdi_color 			GDI_COLOR_BLACK;
	#endif
/*wuzc Del End  For GDI/GUI Portting Ver:    on 2008-3-14 9:41 */
	extern wgui_inline_item wgui_inline_items[];

	extern gdi_handle 		wgui_base_layer;
	extern gdi_handle 		wgui_layer_1;

	#ifndef APPMEM_SUPPORT
		extern U32 	camera_osd_layer_buffer[];
	#endif /* APPMEM_SUPPORT */

#endif /* __FM_CAMERA_TUNNING__ || __FM_CAMERA_PREVIEW__ */



/****************************************************************************
* Global Function                                                           
*****************************************************************************/
extern void UI_set_clip(s32 x1,s32 y1,s32 x2,s32 y2);
extern void UI_fill_rectangle(s32 x1,s32 y1,s32 x2,s32 y2,color_t c);
extern void UI_reset_text_clip(void);
extern void UI_set_font(UI_font_type f);
extern void UI_set_text_color(color_t c);

extern void ShowCategory205Screen(U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,UI_string_type message,U16 message_icon,U8* history_buffer);
extern void PhnsetMainLCDContrast(U8 level);
extern void PhnsetSubLCDContrast(U8 level);
extern void BatteryStatusRsp(void* );
extern void QuitSystemOperation(void);
extern void FactorySetAlarm(U8 sec);
extern void SetBacklightModeExe(U8 mode);
extern void GoBackBothLCDHistory(void);
extern void EMFMBatteryStatusMsgHandler( void  *inMsg );
extern void DTGetRTCTime(MYTIME *t);
extern module_type UART_GetOwnerID(UART_PORT port);
extern void	wgui_update_inline_data(void);		/* for camera tune screen */
extern U16 	GetCurKeypadTone(void);

#ifdef NAND_SUPPORT
	extern kal_bool NANDsanitycheck_Factory(void);
#endif

#if defined(__FM_CAMERA_TUNNING__) || defined(__FM_CAMERA_PREVIEW__)
	extern void mmi_fm_camera_entry_sublcd_screen(void);
#endif /* __FM_CAMERA_TUNNING__ ||  __FM_CAMERA_PREVIEW__ */

/***************************************************************************** 
* Local Variable
*****************************************************************************/
S8 	EMFMUnicodeDisplayBuf[MAX_TEST_STRING_BUF*2];	/* display buffer */
S8 	CardType[32];
U8 	EMFMAsciiDisplayBuf[MAX_TEST_STRING_BUF];
U8   	NeedExitFunc = TRUE;
S16 	currTestItem = 0;
BOOL 	TestExecuted[MAX_AUTO_TEST_ITEMS];

static U8   sequence_counter=0; /* for auto display action use */
static U16 	currentContrast=7;
 
//static U8	LCDType=0;
static U8 	autoTestMode = 0;
static U8 	currentTest = 0;
static U8 	gFM_HighlightIdx;
static U8 	gFM_MenuItemNum;
static U8  	gBarCode[MAX_SUB_MENU_SIZE];
static U8  	gBarCodeReadFlag=0;
static U32 	FM_LEDState;
static FM_ADC_TYPE_ENUM   adc_mode = FM_ADC_BATTERY;

static unsigned char keypad_status[LINE_NUM][COLUMN_NUM];
static U8 	InkeyTestingScreen=0;

/* ===== Loud Speak ============ */
static BOOL LoudSpkTestOn = FALSE;

/*=============== MTBF =============== */
static U32   MTBF_counter = 0; //for MTBF
static FM_MTBF_LCD_ENUM    MTBF_state_LCD = 0; 
static FM_MTBF_LED_ENUM     MTBF_state_LED = 0; 
static FM_MTBF_VIB_ENUM     MTBF_state_VIB = 0; 
static FM_MTBF_AUD_ALTERNATION_ENUM AUD_TEST_ON = 0;


#ifdef MMI_ON_HARDWARE_P
	const device_tones_struct test_tone[]           =  { { 1000, 0, 0,  0,   0 } };

	const device_tones_struct SPK_test_tone[FM_MTBF_SPK_MAX] = 
	{ 
		{ 400, 0, FM_MTBF_SPK_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 1 },
		{ 600, 0, FM_MTBF_SPK_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 2 },
		{ 800, 0, FM_MTBF_SPK_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 3 },
		{ 850, 0, FM_MTBF_SPK_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 4 },
		{ 850, 0, FM_MTBF_SPK_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 5 },
		{ 900, 0, FM_MTBF_SPK_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 6 },
		{ 1000, 0, FM_MTBF_SPK_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 7 },
		{ 2000, 0, FM_MTBF_SPK_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 8 },
		{ 4000, 0, FM_MTBF_SPK_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 9 },
		{ 8000, 0, FM_MTBF_SPK_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 0 }
	};

	const device_tones_struct REV_test_tone[FM_MTBF_REV_MAX] = 
	{ 
		{ 400, 0, FM_MTBF_REV_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 1 },
		{ 500, 0, FM_MTBF_REV_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 2 },
		{ 600, 0, FM_MTBF_REV_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 3 },
		{ 600, 0, FM_MTBF_REV_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 4 },
		{ 700, 0, FM_MTBF_REV_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 5 },
		{ 800, 0, FM_MTBF_REV_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 6 },
		{ 1000, 0, FM_MTBF_REV_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 7 },
		{ 1500, 0, FM_MTBF_REV_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 8 },
		{ 2000, 0, FM_MTBF_REV_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 9 },
		{ 3000, 0, FM_MTBF_REV_UNIT_CYCLE,  FM_MTBF_AUDIO_GAP, 0 }
	};
#endif

static BOOL EchoLoopTestOn = FALSE; 	/* Echo Loop */
static U8 vib, vib_status=KAL_FALSE;	/* Vibrator */
static BOOL ReceiverTestOn = FALSE;		/* Receiver */
static BOOL HeadsetTestOn = FALSE;		/* Headset Test */
static BOOL RingToneTestOn = FALSE;		/* RingTone */
U8 gFactoryAlarm = 0; /* Factory Alarm Flag for MMI alarm framework use */

mmi_fmt_get_msdc_status_rsp_strcut FM_Card_Status; 

BOOL Card_Response_Received = FALSE;
BOOL Card_Response_Sent = FALSE;
BOOL UART_Response_Received = FALSE;

#ifndef __MMI_DISABLE_DUAL_SENSOR__
static BOOL flash_status = FALSE;
 void FM_FlashOn(void);

 void FM_FlashOff(void);


#endif

void HightlightFMHwModuleNameHdlr(void);

#ifndef MEM_ULC_3216
U8 FM_LED_Menu[][MAX_SUB_MENU_SIZE] = {
#else
U8 FM_LED_Menu[][14] = {
#endif
		"Main LCM BL",
	#ifdef __MMI_SUBLCD__
		"Sub LCM BL",
	#endif
		"Keypad LED",

	#ifndef __MMI_HIDE_STATUS_LED_R__
		"Status LED R",
	#endif

	#ifndef __MMI_HIDE_STATUS_LED_G__
		"Status LED G",
	#endif

	#ifndef __MMI_HIDE_STATUS_LED_B__
		"Status LED B"
	#endif
};

/* camera */
fm_camtune_cntx_struct g_camtune_cntx;


/*=============== Auto Testing =============== */
const testlet Tests[MAX_AUTO_TEST_ITEMS] = 
{ //Maximum name length = MAX_TEST_ITEM_NAME_LEN*ENCODING_LENGTH
  {{'L', 0, 'E', 0, 'D', 0}, FM_AutoTest_LED}, 
  {{'C', 0, 'l', 0, 'a', 0, 'm', 0}, FM_AutoTest_CLAM}, 	
  {{'L', 0, 'C', 0, 'M', 0}, FM_AutoTest_SubLCM}, 
  {{'V', 0,'e', 0,'r', 0,'s', 0,'i', 0,'o', 0,'n', 0}, FM_AutoTest_Version}, 
  {{'B', 0,'a', 0,'c', 0,'k', 0,'L', 0,'i', 0,'g', 0,'h', 0,'t', 0}, FM_AutoTest_Backlight}, 
  {{'L', 0,'C', 0,'D', 0}, FM_AutoTest_LCD}, 
  {{'K', 0,'e', 0,'y', 0,'P', 0,'a', 0,'d', 0}, FM_AutoTest_Keypad}, 
  {{'R', 0,'e', 0,'c', 0,'e', 0,'i', 0,'v', 0,'e', 0,'r', 0}, FM_AutoTest_Receiver}, 
  {{'M', 0,'I', 0,'C', 0}, FM_AutoTest_MIC}, 
  {{'S', 0,'p', 0,'e', 0,'a', 0,'k', 0,'e', 0,'r', 0}, FM_AutoTest_Speaker}, 
  {{'H', 0,'e', 0,'a', 0,'d', 0,'s', 0,'e', 0,'t', 0}, FM_AutoTest_Headset}, 
  {{'B', 0,'a', 0,'t', 0,'t', 0,'e', 0,'r', 0,'y', 0}, FM_AutoTest_Battery}, 
  {{'M', 0,'e', 0,'l', 0,'o', 0,'d', 0,'y', 0}, FM_AutoTest_Melody}, 
  {{'V', 0,'i', 0,'b', 0,'r', 0,'a', 0,'t', 0,'o', 0,'r', 0}, FM_AutoTest_VIB}, 
  {{'C', 0,'h', 0,'a', 0,'r', 0,'g', 0,'e', 0,'r', 0}, FM_AutoTest_Charger},
  {{'N', 0,'A', 0,'N', 0,'D', 0}, FM_AutoTest_NAND}, 
  {{'U', 0,'A', 0,'R', 0,'T', 0}, FM_AutoTest_UART},
  {{'D',0,'o',0,'u',0,'b',0,'l',0,'e',0,'S', 0,'p', 0,'e', 0,'a', 0,'k', 0,'e', 0,'r', 0}, FM_AutoTest_Double_Speaker}, //17
#ifdef __MMI_CAMERA__   //luanyy modify for cs621
  {{'C', 0,'A', 0,'M', 0,'E', 0,'R', 0,'A', 0}, FM_AutoTest_CAMERA}, //18
#endif  
  {{'M',0,'e', 0,'m', 0,'o', 0,'r', 0,'y', 0,'C', 0,'a', 0,'r', 0,'d', 0}, FM_AutoTest_MemoryCard} //19
#ifdef __MMI_TOUCH_SCREEN__
  ,{{'P',0,'a',0,'r',0,'a',0,'l',0,'l',0,'e',0,'l',0,'L',0,'i',0,'n',0,'e',0},FM_AutoTest_Pen_Parallel_Test} //20
  ,{{'N',0,'x',0,'M',0,'P',0,'o',0,'i',0,'n',0,'t',0},FM_AutoTest_Pen_N_Cross_M_Test} //21
#endif /*__MMI_TOUCH_SCREEN__*/
};


/* ========== KeyPad test use ================ */

/*	wangrui modify 2008.10.19 begin for fixbug 10083*/

#ifdef __MMI_PROJECT_D700__
const char *keypad_layout[LINE_NUM][COLUMN_NUM]=  
{
	{KEY_LSK_NAME, 		KEY_OK_NAME, 		 KEY_CAMERA_NAME, 		KEY_RSK_NAME},
	{KEY_UP_ARROW_NAME, KEY_DOWN_ARROW_NAME, KEY_LEFT_ARROW_NAME,	KEY_RIGHT_ARROW_NAME},
	{KEY_SEND_NAME, 	KEY_END_NAME, 		 KEY_NONE_NAME, 		KEY_NONE_NAME},
	{"1       ", "2      ", "3     ", KEY_NONE_NAME},
	{"4       ", "5      ", "6     ", KEY_NONE_NAME},
	{"7       ", "8      ", "9     ", KEY_NONE_NAME},
	{"*       ", "0     ",  "#     ", KEY_NONE_NAME},
};

const U16 keypad_value[]=
{
	KEY_LSK_VALUE, 		KEY_OK_VALUE, 		  KEY_CAMERA_VALUE, 	KEY_RSK_VALUE,
	KEY_UP_ARROW_VALUE, KEY_DOWN_ARROW_VALUE, KEY_LEFT_ARROW_VALUE, KEY_RIGHT_ARROW_VALUE,
	KEY_SEND_VALUE, 	KEY_END_VALUE, 		  KEY_NONE_VALUE, 		KEY_NONE_VALUE, 
	KEY_1,KEY_2,KEY_3,KEY_NONE_VALUE,
	KEY_4,KEY_5,KEY_6,KEY_NONE_VALUE,
	KEY_7,KEY_8,KEY_9,KEY_NONE_VALUE,
	KEY_STAR,KEY_0,KEY_POUND,KEY_NONE_VALUE,
};
#elif defined (__MMI_PROJECT_T33__)
const char *keypad_layout[LINE_NUM][COLUMN_NUM]=
{
   {KEY_LSK_NAME, KEY_OK_NAME, KEY_NONE_NAME, KEY_RSK_NAME},
#if defined(__MMI_FACTORY_MODE_NAV_KEY__) || defined(__MMI_WITH_C_KEY__) 
   {KEY_NONE_NAME, KEY_CLR_NAME,    KEY_OK_NAME,    KEY_NONE_NAME},
#endif
   {KEY_UP_ARROW_NAME, KEY_DOWN_ARROW_NAME, KEY_LEFT_ARROW_NAME, KEY_RIGHT_ARROW_NAME},
   {KEY_NONE_NAME, KEY_END_NAME, KEY_VOL_UP_NAME, KEY_VOL_DOWN_NAME},
   {"   1","   2","   3",
#ifdef __SENDKEY2_SUPPORT__
KEY_SEND1_NAME
#else
KEY_NONE_NAME
#endif
},
   {"   4","   5","   6",
KEY_NONE_NAME
},
   {"   7","   8","   9",KEY_NONE_NAME},
   {"   *","   0","   #",KEY_NONE_NAME},

};

const U16 keypad_value[]=
{
   KEY_LSK_VALUE, KEY_OK_VALUE, KEY_NONE_VALUE, KEY_RSK_VALUE,
#if defined(__MMI_FACTORY_MODE_NAV_KEY__) || defined(__MMI_WITH_C_KEY__) 
   KEY_NONE_VALUE,  KEY_CLR_VALUE,  KEY_OK_VALUE,    KEY_NONE_VALUE,
#endif
   KEY_UP_ARROW_VALUE, KEY_DOWN_ARROW_VALUE, KEY_LEFT_ARROW_VALUE, KEY_RIGHT_ARROW_VALUE,
   KEY_NONE_VALUE, KEY_END_VALUE, KEY_VOL_UP_VALUE, KEY_VOL_DOWN_VALUE, 
   KEY_1,KEY_2,KEY_3,
   #ifdef __SENDKEY2_SUPPORT__
   KEY_SEND1_VALUE,
   #else
   KEY_NONE_VALUE,
   #endif
   KEY_4,KEY_5,KEY_6,
   KEY_NONE_VALUE,
   KEY_7,KEY_8,KEY_9,KEY_NONE_VALUE,
   KEY_STAR,KEY_0,KEY_POUND,KEY_NONE_VALUE,
};

#elif defined (__MMI_BLACKBERRY_QWERTY__)

const char *keypad_layout[LINE_NUM][COLUMN_NUM]=
{
   {"Sd1", " Sd2", " Lsk", " Rsk", " Mp3", " End"},
   {"Up", " Dn", " LF", " RT", " OK", KEY_NONE_NAME},
   {"  1","   2","   3", "  4","  5", "  6"},
   {"  7","   8","   9", " #", "  *", " alt"},
   {"  T","  Y","  U",  " I", " O", " P"},
   {" G","  H","  J",  " K", " L", " Del"},
   {"  V","  B","  N",  " M", " $", " <--"},
   {" aA", " 0", " [ ]", " sym", " Aa",  KEY_NONE_NAME}
};

const U16 keypad_value[]=
{
   KEY_SEND1,	 KEY_SEND2, 		    KEY_LSK,               KEY_RSK,                 KEY_MUSIC,       KEY_END,
   KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_ENTER,    KEY_NONE_VALUE,
   KEY_1,			 KEY_2,			    KEY_3, 		     KEY_4,			 KEY_5,	        KEY_6,
   KEY_7,			KEY_8,			    KEY_9,                   KEY_Q,			KEY_A,            KEY_SHIFT_INPUTM,
   KEY_T,			KEY_Y,			    KEY_U,		     KEY_I, 
			KEY_O, 	        KEY_P,
   KEY_G,			KEY_H,			    KEY_J,			     KEY_K,			KEY_L,	        KEY_BACKSPACE1,
   KEY_V,			KEY_B,			    KEY_N,		     KEY_M,			KEY_DOLLAR,  KEY_NEWLINE,
   KEY_SHIFT1,	KEY_0,			    KEY_SPACE1,	     KEY_SYM,		KEY_CALENDER,		KEY_NONE_VALUE
};

#elif defined(TGT_GALLITE_G800)

#undef KEY_CAMERA_NAME
#undef KEY_CAMERA_VALUE
#define KEY_CAMERA_NAME		"TV "
#define KEY_CAMERA_VALUE	KEY_CAMERA

const char *keypad_layout[LINE_NUM][COLUMN_NUM]=
{
   {KEY_LSK_NAME, KEY_OK_NAME, KEY_NONE_NAME, KEY_RSK_NAME},
#if defined(__MMI_FACTORY_MODE_NAV_KEY__) || defined(__MMI_WITH_C_KEY__) 
   {KEY_NONE_NAME, KEY_CLR_NAME,    KEY_OK_NAME,    KEY_NONE_NAME},
#endif
   {KEY_UP_ARROW_NAME, KEY_DOWN_ARROW_NAME, KEY_LEFT_ARROW_NAME, KEY_RIGHT_ARROW_NAME},
   {KEY_CAMERA_NAME, KEY_VOL_UP_NAME, KEY_VOL_DOWN_NAME,KEY_END_NAME},
   {
   #ifdef __SENDKEY2_SUPPORT__
KEY_SEND1_NAME,
#else
KEY_NONE_NAME,
#endif
   	KEY_NONE_NAME,KEY_NONE_NAME,KEY_SEND2_NAME},
   {"   1","   2","   3",KEY_NONE_NAME},
   {"   4","   5","   6",KEY_NONE_NAME},
   {"   7","   8","   9",KEY_NONE_NAME},
   {"   *","   0","   #",KEY_NONE_NAME}

};

const U16 keypad_value[]=
{
   KEY_LSK_VALUE, KEY_OK_VALUE, KEY_NONE_VALUE, KEY_RSK_VALUE,
#if defined(__MMI_FACTORY_MODE_NAV_KEY__) || defined(__MMI_WITH_C_KEY__) 
   KEY_NONE_VALUE,  KEY_CLR_VALUE,  KEY_OK_VALUE,    KEY_NONE_VALUE,
#endif
   KEY_UP_ARROW_VALUE, KEY_DOWN_ARROW_VALUE, KEY_LEFT_ARROW_VALUE, KEY_RIGHT_ARROW_VALUE,
   KEY_CAMERA_VALUE, KEY_VOL_UP_VALUE, KEY_VOL_DOWN_VALUE, KEY_END_VALUE, 
   KEY_SEND1_VALUE,KEY_NONE_VALUE,KEY_NONE_VALUE,KEY_SEND2_VALUE,
   KEY_1,KEY_2,KEY_3,KEY_NONE_VALUE,
   KEY_4,KEY_5,KEY_6,KEY_NONE_VALUE,
   KEY_7,KEY_8,KEY_9,KEY_NONE_VALUE,
   KEY_STAR,KEY_0,KEY_POUND,KEY_NONE_VALUE
};
#elif defined(TGT_TD_4SIM)

#undef KEY_CAMERA_NAME
#undef KEY_CAMERA_VALUE
#define KEY_CAMERA_NAME		"Snd4 "
#define KEY_CAMERA_VALUE	KEY_CAMERA

const char *keypad_layout[LINE_NUM][COLUMN_NUM]=
{
   {KEY_LSK_NAME, KEY_OK_NAME, KEY_NONE_NAME, KEY_RSK_NAME},
#if defined(__MMI_FACTORY_MODE_NAV_KEY__) || defined(__MMI_WITH_C_KEY__) 
   {KEY_NONE_NAME, KEY_CLR_NAME,    KEY_OK_NAME,    KEY_NONE_NAME},
#endif
   {KEY_UP_ARROW_NAME, KEY_DOWN_ARROW_NAME, KEY_LEFT_ARROW_NAME, KEY_RIGHT_ARROW_NAME},
   {KEY_CAMERA_NAME, KEY_VOL_UP_NAME, KEY_VOL_DOWN_NAME,KEY_END_NAME},
   {
   #ifdef __SENDKEY2_SUPPORT__
KEY_SEND1_NAME,
#else
KEY_NONE_NAME,
#endif
KEY_NONE_NAME,KEY_NONE_NAME,
#ifdef __SENDKEY2_SUPPORT__
KEY_SEND2_NAME
#else
KEY_NONE_NAME
#endif
},
   {"   1","   2","   3",KEY_NONE_NAME},
   {"   4","   5","   6",KEY_NONE_NAME},
   {"   7","   8","   9",KEY_NONE_NAME},
   {"   *","   0","   #",KEY_NONE_NAME}

};

const U16 keypad_value[]=
{
   KEY_LSK_VALUE, KEY_OK_VALUE, KEY_NONE_VALUE, KEY_RSK_VALUE,
#if defined(__MMI_FACTORY_MODE_NAV_KEY__) || defined(__MMI_WITH_C_KEY__) 
   KEY_NONE_VALUE,  KEY_CLR_VALUE,  KEY_OK_VALUE,    KEY_NONE_VALUE,
#endif
   KEY_UP_ARROW_VALUE, KEY_DOWN_ARROW_VALUE, KEY_LEFT_ARROW_VALUE, KEY_RIGHT_ARROW_VALUE,
   KEY_CAMERA_VALUE, KEY_VOL_UP_VALUE, KEY_VOL_DOWN_VALUE, KEY_END_VALUE, 
   KEY_SEND_VALUE,KEY_NONE_VALUE,KEY_NONE_VALUE,
#ifdef __SENDKEY2_SUPPORT__
	KEY_SEND2_VALUE,
#else
	KEY_NONE_VALUE,
#endif
   KEY_1,KEY_2,KEY_3,KEY_NONE_VALUE,
   KEY_4,KEY_5,KEY_6,KEY_NONE_VALUE,
   KEY_7,KEY_8,KEY_9,KEY_NONE_VALUE,
   KEY_STAR,KEY_0,KEY_POUND,KEY_NONE_VALUE
};
#elif defined(__MMI_MAINLCD_220X176__)
const char *keypad_layout[LINE_NUM][COLUMN_NUM]=
{
	{KEY_LSK_NAME,KEY_SEND_NAME,KEY_RSK_NAME,KEY_END_NAME},
	{KEY_UP_ARROW_NAME, KEY_DOWN_ARROW_NAME, KEY_LEFT_ARROW_NAME, KEY_RIGHT_ARROW_NAME},
	{KEY_OK_NAME,"   *","   #","   0"},
	{"   1","   2","   3",KEY_NONE_NAME},
	{"   4","   5","   6",KEY_NONE_NAME},
	{"   7","   8","   9",KEY_NONE_NAME},
	{"cln   ","Sms   ","Game  ","Mainmenu"},
	{"mp3   ","mp4   ","camr  ","qq     "}
};

const U16 keypad_value[]=
{
   KEY_LSK_VALUE, KEY_SEND_VALUE, KEY_RSK_VALUE, KEY_END_VALUE,
   KEY_UP_ARROW_VALUE, KEY_DOWN_ARROW_VALUE, KEY_LEFT_ARROW_VALUE, KEY_RIGHT_ARROW_VALUE,
   KEY_ENTER,KEY_STAR,KEY_POUND,KEY_0, 
   KEY_1,KEY_2,KEY_3,KEY_NONE_VALUE,
   KEY_4,KEY_5,KEY_6,KEY_NONE_VALUE,
   KEY_7,KEY_8,KEY_9,KEY_NONE_VALUE,
   KEY_EXTRA_2,KEY_SMS

};


#elif defined(__PROJECT_GALLITE_C01__)
const char *keypad_layout[LINE_NUM][COLUMN_NUM]=
{
   {"	Pre", "		Play", "	Next", "	Music"},
   {KEY_LSK_NAME, KEY_OK_NAME, KEY_CAMERA_NAME, KEY_RSK_NAME},
#if defined(__MMI_FACTORY_MODE_NAV_KEY__) || defined(__MMI_WITH_C_KEY__) 
   {KEY_NONE_NAME, KEY_CLR_NAME,    KEY_OK_NAME,    KEY_NONE_NAME},
#endif
   {KEY_UP_ARROW_NAME, KEY_DOWN_ARROW_NAME, KEY_LEFT_ARROW_NAME, KEY_RIGHT_ARROW_NAME},
   {KEY_SEND_NAME, KEY_END_NAME, KEY_NONE_NAME, "	QQ"},
   {"   1","   2","   3",
#ifdef __SENDKEY2_SUPPORT__
KEY_SEND1_NAME
#else
KEY_NONE_NAME
#endif
},
   {"   4","   5","   6",
#ifdef __SENDKEY2_SUPPORT__
KEY_SEND2_NAME
#else
KEY_NONE_NAME
#endif
},
   {"   7","   8","   9",KEY_NONE_NAME},
   {"   *","   0","   #",KEY_NONE_NAME},

};
const U16 keypad_value[]=
{
   KEY_MUSIC_PREV, KEY_MUSIC_PLAY, KEY_MUSIC_NEXT, KEY_MUSIC,
   KEY_LSK_VALUE, KEY_OK_VALUE, KEY_CAMERA_VALUE, KEY_RSK_VALUE,
#if defined(__MMI_FACTORY_MODE_NAV_KEY__) || defined(__MMI_WITH_C_KEY__) 
   KEY_NONE_VALUE,  KEY_CLR_VALUE,  KEY_OK_VALUE,    KEY_NONE_VALUE,
#endif
   KEY_UP_ARROW_VALUE, KEY_DOWN_ARROW_VALUE, KEY_LEFT_ARROW_VALUE, KEY_RIGHT_ARROW_VALUE,
   KEY_SEND_VALUE, KEY_END_VALUE, KEY_VOL_UP_VALUE, KEY_QQ, 
   KEY_1,KEY_2,KEY_3,
   #ifdef __SENDKEY2_SUPPORT__
   KEY_SEND1_VALUE,
   #else
   KEY_NONE_VALUE,
   #endif
   KEY_4,KEY_5,KEY_6,
   #ifdef __SENDKEY2_SUPPORT__
   KEY_SEND2_VALUE,
   #else
   KEY_NONE_VALUE,
   #endif
   KEY_7,KEY_8,KEY_9,KEY_NONE_VALUE,
   KEY_STAR,KEY_0,KEY_POUND,KEY_NONE_VALUE,
};
#elif defined(__MMI_PROJECT_GS_HK603__)

#undef KEY_MUSIC_NAME
#undef KEY_MUSIC_VALUE
#define KEY_MUSIC_NAME		"Music"
#define KEY_MUSIC_VALUE	KEY_MUSIC

const char *keypad_layout[LINE_NUM][COLUMN_NUM]=
{
   {KEY_LSK_NAME, KEY_OK_NAME, KEY_NONE_NAME, KEY_RSK_NAME},
#if defined(__MMI_FACTORY_MODE_NAV_KEY__) || defined(__MMI_WITH_C_KEY__) 
   {KEY_NONE_NAME, KEY_CLR_NAME,    KEY_OK_NAME,    KEY_NONE_NAME},
#endif
   {KEY_UP_ARROW_NAME, KEY_DOWN_ARROW_NAME, KEY_LEFT_ARROW_NAME, KEY_RIGHT_ARROW_NAME},
   {KEY_MUSIC_NAME, KEY_VOL_UP_NAME, KEY_VOL_DOWN_NAME,KEY_END_NAME},
   {
   #ifdef __SENDKEY2_SUPPORT__
KEY_SEND1_NAME,
#else
KEY_NONE_NAME,
#endif
   	KEY_NONE_NAME,KEY_NONE_NAME,KEY_SEND2_NAME},
   {"   1","   2","   3",KEY_NONE_NAME},
   {"   4","   5","   6",KEY_NONE_NAME},
   {"   7","   8","   9",KEY_NONE_NAME},
   {"   *","   0","   #",KEY_NONE_NAME}

};

const U16 keypad_value[]=
{
   KEY_LSK_VALUE, KEY_OK_VALUE, KEY_NONE_VALUE, KEY_RSK_VALUE,
#if defined(__MMI_FACTORY_MODE_NAV_KEY__) || defined(__MMI_WITH_C_KEY__) 
   KEY_NONE_VALUE,  KEY_CLR_VALUE,  KEY_OK_VALUE,    KEY_NONE_VALUE,
#endif
   KEY_UP_ARROW_VALUE, KEY_DOWN_ARROW_VALUE, KEY_LEFT_ARROW_VALUE, KEY_RIGHT_ARROW_VALUE,
   KEY_MUSIC_VALUE, KEY_VOL_UP_VALUE, KEY_VOL_DOWN_VALUE, KEY_END_VALUE, 
   KEY_SEND1_VALUE,KEY_NONE_VALUE,KEY_NONE_VALUE,KEY_SEND2_VALUE,
   KEY_1,KEY_2,KEY_3,KEY_NONE_VALUE,
   KEY_4,KEY_5,KEY_6,KEY_NONE_VALUE,
   KEY_7,KEY_8,KEY_9,KEY_NONE_VALUE,
   KEY_STAR,KEY_0,KEY_POUND,KEY_NONE_VALUE
};
#else
const char *keypad_layout[LINE_NUM][COLUMN_NUM]=
{
   {KEY_LSK_NAME, KEY_OK_NAME, KEY_CAMERA_NAME, KEY_RSK_NAME},
#if defined(__MMI_FACTORY_MODE_NAV_KEY__) || defined(__MMI_WITH_C_KEY__) 
   {KEY_NONE_NAME, KEY_CLR_NAME,    KEY_OK_NAME,    KEY_NONE_NAME},
#endif
   {KEY_UP_ARROW_NAME, KEY_DOWN_ARROW_NAME, KEY_LEFT_ARROW_NAME, KEY_RIGHT_ARROW_NAME},
   {KEY_SEND_NAME, KEY_END_NAME, KEY_VOL_UP_NAME, KEY_VOL_DOWN_NAME},
   {"   1","   2","   3",KEY_NONE_NAME},
   {"   4","   5","   6",KEY_NONE_NAME},
   {"   7","   8","   9",KEY_NONE_NAME},
   {"   *","   0","   #",KEY_NONE_NAME},

};

const U16 keypad_value[]=
{
   KEY_LSK_VALUE, KEY_OK_VALUE, KEY_CAMERA_VALUE, KEY_RSK_VALUE,
#if defined(__MMI_FACTORY_MODE_NAV_KEY__) || defined(__MMI_WITH_C_KEY__) 
   KEY_NONE_VALUE,  KEY_CLR_VALUE,  KEY_OK_VALUE,    KEY_NONE_VALUE,
#endif
   KEY_UP_ARROW_VALUE, KEY_DOWN_ARROW_VALUE, KEY_LEFT_ARROW_VALUE, KEY_RIGHT_ARROW_VALUE,
   KEY_SEND_VALUE, KEY_END_VALUE, KEY_VOL_UP_VALUE, KEY_VOL_DOWN_VALUE, 
   KEY_1,KEY_2,KEY_3,KEY_NONE_VALUE,
   KEY_4,KEY_5,KEY_6,KEY_NONE_VALUE,
   KEY_7,KEY_8,KEY_9,KEY_NONE_VALUE,
   KEY_STAR,KEY_0,KEY_POUND,KEY_NONE_VALUE,
};
#endif
#define N_MAX_KEY_TEST	((sizeof(keypad_value))/(sizeof(U16)))
/*****************************************************************************
* Local Function 
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
	static void mmi_fm_camtune_init(void);
	static void mmi_fm_entry_camera_screen(void);
	static void mmi_fm_camera_entry_restore_confirm_screen(void);
	static void mmi_fm_camera_restore_confirm_yes(void);
	static void mmi_fm_camera_entry_save_confirm_screen(void);
	static void mmi_fm_camera_save_confirm_yes(void);
	static void mmi_fm_camera_highlight_hdlr(S32 index);

	static void mmi_fm_camtune_set_value(void);
	static void mmi_fm_camtune_draw_info_callback(S32 index, S32 x1, S32 y1, S32 x2, S32 y2);
	static void mmi_fm_camtune_highlight_hdlr(S32 index);
	static void mmi_fm_camtune_get_category_info(U16 category_idx);
	static void mmi_fm_camtune_get_group_info(U16 category_idx, U16 group_idx);
	static void mmi_fm_camtune_change_group_fwd(void);
	static void mmi_fm_camtune_toggle_fullscreen(void);
	static void mmi_fm_camtune_show_group(void);
	static void mmi_fm_camtune_entry_screen(void);
	static void mmi_fm_camtune_exit_screen(void);
	
	static void mmi_fm_highlight_camera(void);
#endif /* __FM_CAMERA_TUNNING__ */

#if defined(__FM_CAMERA_PREVIEW__)
	void mmi_fm_camera_entry_preview_screen(void);
#endif /* __FM_CAMERA_PREVIEW__ */

#if defined(__FM_CAMERA_PREVIEW__) || defined(__FM_CAMERA_TUNNING__)
	static void mmi_fm_highlight_camera(void);
#endif /* __FM_CAMERA_PREVIEW__ || __FM_CAMERA_TUNNING__ */
#ifdef __MMI_FM_RADIO__
void HighlightFMFMRadiohdlr(void);
#endif
/***************************************************************************
*
*	Cody Body                                                   
*
***************************************************************************/
/**************************************************************
**	FUNCTION NAME		: FM_RestoreAudio
**
**  	PURPOSE				: Helper function for Restoring audio
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_RestoreAudio(void);
/**************************************************************
**************************************************************/
void HighlightFMIsCalibrated(void)
{
	SetLeftSoftkeyFunction(NULL, KEY_EVENT_UP);
	ChangeLeftSoftkey(0, 0);
}
/**************************************************************
**	FUNCTION NAME		: InitFactoryMode
**
**  	PURPOSE				: This function is to initialize hilite handlers for Factory mode
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void InitFactoryMode(void)
{
	SetHiliteHandler(MENU_ID_FM_VERSION, HightlightFMVersionHdlr);

	SetHiliteHandler(MENU_ID_FM_SW_SUMMARY, HightlightFMVersionSummaryHdlr);
#if defined(__PROJECT_GALLITE_C01__)
	SetHiliteHandler(MENU_ID_FM_SW_VER, HightlightFMVersionCommonHdlr);
#endif
	SetHiliteHandler(MENU_ID_FM_MELODY_VER, HightlightFMVersionCommonHdlr);
	SetHiliteHandler(MENU_ID_FM_ISN, HightlightFMVersionCommonHdlr);
	SetHiliteHandler(MENU_ID_FM_BBCHIP_VER, HightlightFMVersionCommonHdlr);
	SetHiliteHandler(MENU_ID_FM_DSPCODE_VER, HightlightFMVersionCommonHdlr);
	SetHiliteHandler(MENU_ID_FM_DSPATCH_VER, HightlightFMVersionCommonHdlr);
	#ifdef TGT_AUTOGEN_MODULE_NAME
	SetHiliteHandler(MENU_ID_FM_HW_VER, HightlightFMHwModuleNameHdlr);
	#endif
#if defined(__PROJECT_GALLITE_C01__)
	SetHiliteHandler(MENU_ID_FM_HW_VER, HightlightFMVersionHwHdlr);
#endif
	SetHiliteHandler(MENU_ID_FM_BUILD_TIME, HightlightFMVersionCommonHdlr);

	SetHiliteHandler(MENU_ID_FM_RESOURCE_BIN, HightlightFMResourceBinHdlr);
	SetHiliteHandler(MENU_ID_FM_LED, HightlightFMLEDHdlr);
	SetHiliteHandler(MENU_ID_FM_LCD, HightlightFMLCDHdlr);
	SetHiliteHandler(MENU_ID_FM_LCD_AUTO, HightlightFMLCDCertainHdlr);
	SetHiliteHandler(MENU_ID_FM_LCD_R, HightlightFMLCDCertainHdlr);
	SetHiliteHandler(MENU_ID_FM_LCD_G, HightlightFMLCDCertainHdlr);
	SetHiliteHandler(MENU_ID_FM_LCD_B, HightlightFMLCDCertainHdlr);
	SetHiliteHandler(MENU_ID_FM_LCD_W, HightlightFMLCDCertainHdlr);
	SetHiliteHandler(MENU_ID_FM_LCD_BLACK, HightlightFMLCDCertainHdlr);
	SetHiliteHandler(MENU_ID_FM_LCD_CONTRAST, HightlightFMLCDContrastHdlr);
	SetHiliteHandler(MENU_ID_FM_LCD_CONTRAST_MAINLCD, HightlightFMLCDContrastMainHdlr);
	SetHiliteHandler(MENU_ID_FM_LCD_CONTRAST_SUBLCD, HightlightFMLCDContrastSubHdlr);
	SetHiliteHandler(MENU_ID_FM_KEYPAD, HightlightFMKeypadHdlr);
	SetHiliteHandler(MENU_ID_FM_LOUDSPK, HightlightFMLoudSpkHdlr);
	SetHiliteHandler(MENU_ID_FM_EACHLOOP, HightlightFMEchoLoopHdlr);
	SetHiliteHandler(MENU_ID_FM_VIBRATOR, HightlightFMVibratorHdlr);
	SetHiliteHandler(MENU_ID_FM_ADC, HightlightFMADCHdlr);
	SetHiliteHandler(MENU_ID_FM_RECEIVER, HightlightFMReceiverHdlr);
	SetHiliteHandler(MENU_ID_FM_RTC, HightlightFMRTCHdlr);
	SetHiliteHandler(MENU_ID_FM_MTBF, HightlightFMMTBFHdlr);
	
//#if (defined(__MSDC_MS__) || defined(__MSDC_SD_MMC__) || defined(__MSDC_MSPRO__))							   
	SetHiliteHandler(MENU_ID_FM_MEMORY_CARD, HightlightFMMemoryCardHdlr);
//#endif
	SetHiliteHandler(MENU_ID_FM_UART, HightlightFMUARTHdlr);

#if defined(__FM_CAMERA_TUNNING__) || defined(__FM_CAMERA_PREVIEW__)
	SetHiliteHandler(MENU_ID_FM_CAMERA, mmi_fm_highlight_camera);
#endif

	SetHiliteHandler(MENU_ID_FM_NAND_FLASH, HightlightFMNandFlashHdlr);
	SetHiliteHandler(MENU_ID_FM_HEADSET, HightlightFMHeadsetHdlr);
	SetHiliteHandler(MENU_ID_FM_CHARGER, HightlightFMChargerHdlr);
	SetHiliteHandler(MENU_ID_FM_RINGTONE, HightlightFMRingToneHdlr );

#ifdef MMI_ON_HARDWARE_P
	 //Set protocol event handler
	SetProtocolEventHandler ( FM_ATEntryMenuHdlr, MSG_ID_MMI_FACTORY_TEST_IND  );
	SetProtocolEventHandler ( FM_ATKeypadTestMsgHdlr, MSG_ID_MMI_EM_KEYPAD_EVENT_ACT_IND );
	SetProtocolEventHandler ( FM_ATLCDTestMsgHdlr, MSG_ID_MMI_EM_LCM_TEST_IND );
//wangrui delete the micro for fix bug 10484 	
//	#if (defined(__MSDC_MS__) || defined(__MSDC_SD_MMC__) || defined(__MSDC_MSPRO__))							   
		SetProtocolEventHandler (FM_MemoryCardResponseHandler, MSG_ID_MMI_FMT_GET_MSDC_STATUS_RSP);
//	#endif
	//SetProtocolEventHandler (FM_UARTResponseHandler, MSG_ID_MMI_FMT_GET_MSDC_STATUS_RSP);
#endif
#ifdef __MMI_TOUCH_SCREEN__
	SetHiliteHandler(MENU_ID_FM_PARALLEL_LINE,HighlightFMParallelTestHdlr);
	SetHiliteHandler(MENU_ID_FM_N_CROSS_M,HighlightFMNCrossMTestHdlr);
#endif /*__MMI_TOUCH_SCREEN__*/

#if defined(__FM_CAMERA_TUNNING__)
	mmi_fm_camtune_init();
#endif /* __MMI_CAMERA__ */
#ifdef __MMI_FM_RADIO__
	SetHiliteHandler(MENU_ID_FM_FMRADIO,HighlightFMFMRadiohdlr);
#endif

	SetHiliteHandler(MENU_ID_FM_IS_CALIBRATED, HighlightFMIsCalibrated);
#if defined(__PROJECT_GALLITE_C01__)
	SetHiliteHandler(MENU_ID_FM_AUTO_TEST_ROOT, HighlightFactoryModeAutoTestHdlr);
#endif
}


/**************************************************************
**	FUNCTION NAME		: InitFactoryModeSetting
**
**  	PURPOSE				: This function is to initialize Factory mode
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void InitFactoryModeSetting(void)
{
     gCurrentMode = FACTORY_MODE;
     mdi_audio_suspend_background_play();
     //wangzl:modify B
     //mdi_audio_block( MOD_MMI );
     mdi_audio_stop_all();
     //wangzl:modify E
     StopLEDPatternBackGround();
}


/**************************************************************
**	FUNCTION NAME		: FM_ReTest
**
**  	PURPOSE				: ReTest for the current Auto Test item
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
static void FM_ReTest(void)
{
       //currTestItem--;
       sequence_counter=0; // Reset the sequence counter
       TestExecuted[currTestItem] = FALSE; 
       EntryFMAutoTestStart();
}


/**************************************************************
**	FUNCTION NAME		: FM_Autotest_test_done_USC2string
**
**  	PURPOSE				: Helper function for UCS2 string display
**
**	INPUT PARAMETERS	: name
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
static void FM_Autotest_test_done_USC2string(const S8* name)
{
       S8 name_ascii[MAX_TEST_ITEM_NAME_LEN];

       UnicodeToAnsii((S8*)name_ascii, (S8*)name);
       	
       sprintf((S8*)EMFMAsciiDisplayBuf, "%s (%d/%d)\n", name_ascii, currTestItem+1, gAutoTestListSize);

       AnsiiToUnicodeString((S8*)EMFMUnicodeDisplayBuf, (S8*)EMFMAsciiDisplayBuf);

//Lisen 01262005
       if(currentTest == FM_AUTOTEST_MEMORYCARD||currentTest == FM_TEST_MEMORYCARD)	//chenhe fix bug 11447 
	  UCS2Strcat((S8*)EMFMUnicodeDisplayBuf, CardType);
       else
          UCS2Strcat((S8*)EMFMUnicodeDisplayBuf, GetString(STR_GLOBAL_DONE));

       AnsiiToUnicodeString((S8*)EMFMAsciiDisplayBuf, ".\n\nSendkey: ");

       UCS2Strcat((S8*)EMFMUnicodeDisplayBuf, (S8*)EMFMAsciiDisplayBuf);
        
       UCS2Strcat((S8*)EMFMUnicodeDisplayBuf, GetString(STR_ID_FM_AUTO_TEST_RETEST));
        
       AnsiiToUnicodeString((S8*)EMFMAsciiDisplayBuf, "\nEndkey: ");

      	UCS2Strcat((S8*)EMFMUnicodeDisplayBuf, (S8*)EMFMAsciiDisplayBuf);
      	
       UCS2Strcat((S8*)EMFMUnicodeDisplayBuf, GetString(STR_ID_FM_AUTO_TEST_EXIT));
}


/**************************************************************
**	FUNCTION NAME		: FM_BACK_TO_NORMAL_STATE
**
**  	PURPOSE				: Resume normal state after each auto test item
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
	   /*wang rui modify begin 20081210 for fix bug 10693*/
extern void GpioTurnOffKeypadLight(void);
static void FM_BACK_TO_NORMAL_STATE(void)
{
       sequence_counter=0;
       StopTimer(FM_AUTO_TEST_COMMNON_TIMER);
       
       ReceiverTestOn = FALSE;
       EchoLoopTestOn = FALSE;       
       LoudSpkTestOn = FALSE;       
       HeadsetTestOn = FALSE;
       
       //LED
       FM_SetGpioReq( GPIO_DEV_LED_STATUS_1, LED_LIGHT_LEVEL0 );       
       FM_SetGpioReq( GPIO_DEV_LED_STATUS_2, LED_LIGHT_LEVEL0 );       
       FM_SetGpioReq( GPIO_DEV_LED_STATUS_3, LED_LIGHT_LEVEL0 );       

       //LCD
       FM_SetGpioReq( GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL5 );
	   /*wang rui modify begin 20081210 for fix bug 10693*/
	   GpioTurnOffKeypadLight();
//       FM_SetGpioReq( GPIO_DEV_LED_KEY, LED_LIGHT_LEVEL0 );
	   /*wang rui modify end*/

       //LCD contrast
       PhnsetMainLCDContrast(7);
       PhnsetSubLCDContrast(7);

   #ifdef MMI_ON_HARDWARE_P
       //Lisen 0702
       //Receiver : stop 1K tone
       TONE_Stop();

       // MIC: close loopback 
       custom_em_set_loopback(FALSE);
       
       // Ringtone
       RingToneTestOn = FALSE;
       /* stop MIDI */
       FM_SendStopAudioReq(0);
   	DM_StopTone();
       FM_SendSetAudioModeReq(AUD_MODE_NORMAL);
	/*Wangzl Add Start For 6495 Ver: TBM780  on 2007-8-27 16:29 */
	CFW_EmodAudioTestEnd();
	/*Wangzl Add End  For 6234 Ver: TBM780  on 2007-8-27 16:29 */

       //VIB
       FM_SetGpioReq( GPIO_DEV_VIBRATOR, VIBRATOR_OFF );
   #endif       
}


/**************************************************************
**	FUNCTION NAME		: FM_Handle_Pass_Key_Press
**
**  	PURPOSE				: Auto test Pass key handler
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
static void FM_Handle_Pass_Key_Press(void)
{
       FM_BACK_TO_NORMAL_STATE();
       
       nvramTestResultArray.result[currTestItem] = FM_TEST_PASSED;
       currTestItem++;
       WriteTestResultToNVRAM();
	EntryFMAutoTestStart();
}


/**************************************************************
**	FUNCTION NAME		: FM_Handle_Fail_Key_Press
**
**  	PURPOSE				: Auto test Fail key handler
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
static void FM_Handle_Fail_Key_Press(void)
{
       FM_BACK_TO_NORMAL_STATE();

       nvramTestResultArray.result[currTestItem] = FM_TEST_FAILED;
       currTestItem++;
       WriteTestResultToNVRAM();
	EntryFMAutoTestStart();
}


/**************************************************************
**	FUNCTION NAME		: FM_Autotest_set_key_handlers
**
**  	PURPOSE				: Auto test general key handler
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
static void FM_Autotest_endkey_handler(void)
{
    FM_BACK_TO_NORMAL_STATE();
    DisplayIdleScreen();
}
static void FM_Autotest_set_key_handlers(void)
{
      SetKeyHandler(FM_ReTest, KEY_SEND, KEY_EVENT_UP);//wangzl:modify from KEY_EVENT_DOWN for bug 6319
      #ifdef __SENDKEY2_SUPPORT__
      SetKeyHandler(FM_ReTest, KEY_SEND1, KEY_EVENT_UP);
      SetKeyHandler(FM_ReTest, KEY_SEND2, KEY_EVENT_UP);
      #endif
      PowerAndEndKeyHandler();
      SetLeftSoftkeyFunction(FM_Handle_Pass_Key_Press, KEY_EVENT_UP);
      SetRightSoftkeyFunction(FM_Handle_Fail_Key_Press, KEY_EVENT_UP);
      SetKeyHandler(FM_Autotest_endkey_handler,KEY_END,KEY_EVENT_DOWN);
}


/**************************************************************
**	FUNCTION NAME		: FM_Autotest_Stop_Test
**
**  	PURPOSE				: Auto test Stop test command
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
static void FM_Autotest_Stop_Test(void)
{
      sequence_counter=0; // Reset the sequence counter

      switch (currentTest)
      {
         case    FM_TEST_LED:
         	break;
         case    FM_TEST_CLAM_DETECT:		
         	break;
         case    FM_TEST_SUBLCM:
         	break;
         case    FM_TEST_SW_VERSION:
         	break;
         case    FM_TEST_BACKLIGHT:
         	break;
         case    FM_TEST_LCD:
         	break;
         case    FM_TEST_KEYPAD:
         	break;
         case    FM_TEST_RECEIVER:
         case    FM_TEST_MIC:
         case    FM_TEST_SPEAKER:
         case    FM_TEST_HEADSET:
#ifdef MMI_ON_HARDWARE_P        	
         	Media_Stop();
#endif
         	break;
         case    FM_TEST_VIB:
         	break;
         case    FM_TEST_BATTERY:
         	break;
         case    FM_TEST_MELODY:
#ifdef MMI_ON_HARDWARE_P        	
         	Media_Stop();
#endif
              FM_SendSetAudioModeReq(AUD_MODE_NORMAL);
              FM_SendStopAudioReq(0);
         	break;
         case    FM_TEST_CHARGER:
         	break;
         case    FM_TEST_NAND:
         	break;
         case    FM_TEST_UART:
         case    FM_TEST_DOUBLE_SPEAKER:
#ifdef MMI_ON_HARDWARE_P        	
         	Media_Stop();
#endif
         	break;
         case    FM_TEST_MEMORYCARD:
          	break;
#if defined(__MMI_CAMERA__)
//        case    FM_TEST_CAMERA:
    //     	break;
#endif
#ifdef __MMI_TOUCH_SCREEN__
		case	FM_TEST_PEN_PARALLEL_LINES:
			break;
		case	FM_TEST_PEN_N_CROSS_M_POINTS:
			break;
#endif /*__MMI_TOUCH_SCREEN__*/
         default:
         	ASSERT(0);
      	}
}

/*****************************************************************
 **  Root Menu
 *****************************************************************/

/**************************************************************
**	FUNCTION NAME		: mmi_fm_main_delete_history_hdlr
**  PURPOSE				: a exit function for go back history or along the path to root history (idle screen)
**	INPUT PARAMETERS	: void* param
**	OUTPUT PARAMETERS	: FALSE means no stop from go back history
**	RETURNS				: void
 
**************************************************************/
U8 mmi_fm_main_delete_history_hdlr( void* param )
{
	FM_RestoreAudio();
	gCurrentMode = NORMAL_MODE;
	mdi_audio_unblock();
	mdi_audio_resume_background_play();
	StartLEDPatternBackGround();
	mmi_profiles_restore_activated_profile();
	return FALSE;
}

/**************************************************************
**	FUNCTION NAME		: EntryFMMenu
**
**  	PURPOSE				: Entry function for Factory mode
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMMenu(void)
{
	U16				ItemList[32];
	U16				ItemIcons[32];
	U16				nItems;
	U8*				guiBuffer;
	S32				i;
	UI_string_type	PopUpList[32];

	EntryNewScreen(SCR_ID_FM_ROOT, ExitFMMenu, NULL, NULL);

       autoTestMode = 0; 
	guiBuffer = GetCurrGuiBuffer(SCR_ID_FM_ROOT);
	nItems = GetNumOfChild(MENU_ID_FM_ROOT);
	GetSequenceStringIds(MENU_ID_FM_ROOT,ItemList);
	//GetSequenceImageIds(SCR_ID_FM_ROOT,ItemIcons);
	SetParentHandler(MENU_ID_FM_ROOT);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		if (ItemList[i] == STR_ID_FM_UART)
		{
                   AnsiiToUnicodeString((S8*)subMenuData[i], (S8*)"Be sure that UART2 is not in use!");
  		   PopUpList[i] = (UI_string_type )subMenuData[i];        
		}
		#ifndef MMI_ON_WIN32
		else if(ItemList[i] == STR_ID_FM_IS_CALIBRATED)
		{
			#ifndef MMI_ON_WIN32
			CALIB_CALIBRATION_T* calibPtr = calib_GetPointers();
			if(calibPtr->paramStatus == CALIB_PARAM_CALIBRATED)
			{
				pfnUnicodeStrcpy((S8*)subMenuData[i],(S8*)get_string(STR_GLOBAL_YES));
			}
			else
			{
				pfnUnicodeStrcpy((S8*)subMenuData[i],(S8*)get_string(STR_GLOBAL_NO));
			}
			PopUpList[i] = (UI_string_type )subMenuData[i];
		#endif
		}
		#endif
		else
		   PopUpList[i]=NULL;
	}

       for(i=0;i<nItems&&i<22;i++)
	{
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}
       
	/*
	FM_PreparePopUp( PopUpList );
	ShowCategory52Screen(	STR_ID_FM_ROOT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);
	*/

	ShowCategory52Screen(	STR_ID_FM_ROOT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

     InitFactoryModeSetting();

     SetDelScrnIDCallbackHandler( SCR_ID_FM_ROOT, (HistoryDelCBPtr)mmi_fm_main_delete_history_hdlr );
}

void EntrySWFactoryModeMenu(void)
{
	U16				ItemList[32];
	U16				ItemIcons[32];
	U16				nItems;
	U8*				guiBuffer;
	S32				i;
	UI_string_type	PopUpList[32];

	EntryNewScreen(SCR_ID_SWFM_ROOT, ExitSWFMMenu, NULL, NULL);

    autoTestMode = 0; 
	guiBuffer = GetCurrGuiBuffer(SCR_ID_SWFM_ROOT);
	nItems = GetNumOfChild(MENU_ID_SWFM_ROOT);
	GetSequenceStringIds(MENU_ID_SWFM_ROOT,ItemList);
	//GetSequenceImageIds(SCR_ID_FM_ROOT,ItemIcons);
	SetParentHandler(MENU_ID_SWFM_ROOT);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);


    for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);
	}

	ShowCategory52Screen(	STR_ID_FM_ROOT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

     InitFactoryModeSetting();

     SetDelScrnIDCallbackHandler( SCR_ID_SWFM_ROOT, (HistoryDelCBPtr)mmi_fm_main_delete_history_hdlr );
}

/*Wangzl Add Start For 6321 Ver: TBM780  on 2007-8-20 11:22 */
void FM_Play_Tone(void)
{
	DM_StopTone();
	DM_PlayTone(24, 1, 3000,4);
	coolsand_UI_start_timer(3000,FM_Play_Tone);
}

void FM_Stop_Tone(void)
{
	DM_StopTone();
	coolsand_UI_cancel_timer(FM_Play_Tone);
}
/*Wangzl Add End  For 6321 Ver: TBM780  on 2007-8-20 11:22 */

/**************************************************************
**	FUNCTION NAME		: ExitFMMenu
**
**  	PURPOSE				: Exit function for Factory mode
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
extern void GpioSpeakerModeEnd(void);
void ExitFMMenu (void)
{
	history_t currHistory;

	//FM_RestoreAudio();
#ifdef MMI_ON_HARDWARE_P
        if(LoudSpkTestOn)
          //TONE_Stop();
        {
        	FM_Stop_Tone();
      		#ifdef __FF_AUDIO_SET__
      		GpioSpeakerModeEnd();
     		#endif
		FM_SendSetAudioModeReq(AUD_MODE_NORMAL);
        }
	if(ReceiverTestOn)
	{
		FM_Stop_Tone();
	}
        if(EchoLoopTestOn)
          //custom_em_set_loopback(FALSE);
        {
        	CFW_EmodAudioTestEnd();
        }
	if(HeadsetTestOn)
	{
		FM_Stop_Tone();
		FM_SendSetAudioModeReq(AUD_MODE_NORMAL);
	}
        if(RingToneTestOn)
          FM_SendStopAudioReq(0);   
#endif

	currHistory.scrnID = SCR_ID_FM_ROOT;
	currHistory.entryFuncPtr = EntryFMMenu;
	pfnUnicodeStrcpy((S8*)currHistory.inputBuffer, (S8*)L"");
	GetCategoryHistory (currHistory.guiBuffer);
	AddHistory (currHistory);
}

void ExitSWFMMenu (void)
{
	history_t currHistory;

	currHistory.scrnID = SCR_ID_SWFM_ROOT;
	currHistory.entryFuncPtr = EntrySWFactoryModeMenu;
	pfnUnicodeStrcpy((S8*)currHistory.inputBuffer, (S8*)L"");
	GetCategoryHistory (currHistory.guiBuffer);
	AddHistory (currHistory);
}
/**************************************************************
**	FUNCTION NAME		: HightlightFMVersionSummaryHdlr
**
**  	PURPOSE				: Highlight function for version summary
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMVersionSummaryHdlr(void)
{
	change_left_softkey(STR_GLOBAL_OK,0);
	redraw_left_softkey();
	SetLeftSoftkeyFunction(EntryFMVersionSummaryMenu,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************
**	FUNCTION NAME		: HightlightFMVersionCommonHdlr
**
**  	PURPOSE				: Common Highlight function for Version Menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMVersionCommonHdlr(void)
{
	clear_left_softkey_handler();
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	change_left_softkey(0,0);
	redraw_left_softkey();
	RedrawCategoryFunction();
}
#if defined(__PROJECT_GALLITE_C01__)

void HightlightFMVersionSwHdlr(void)
{
	change_left_softkey(STR_GLOBAL_OK,0);
	redraw_left_softkey();
	SetLeftSoftkeyFunction(EntryFMVersionSwMenu,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

void HightlightFMVersionHwHdlr(void)
{
	change_left_softkey(STR_GLOBAL_OK,0);
	redraw_left_softkey();
	SetLeftSoftkeyFunction(EntryFMVersionHwMenu,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
#endif
extern void SSCHandleFactoryModeAutoTest(void);
void HighlightFactoryModeAutoTestHdlr(void)
{
	change_left_softkey(STR_GLOBAL_OK,0);
	redraw_left_softkey();
	SetLeftSoftkeyFunction(SSCHandleFactoryModeAutoTest,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
/**************************************************************
**	FUNCTION NAME		: EntryFMVersionSummaryMenu
**
**  	PURPOSE				: Entry function for Version Summary info menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
S8   UnicodeDisplayBuf[MAX_TEST_STRING_BUF*10];

void EntryFMVersionSummaryMenu(void)	
{
    U8*	guiBuffer;
    U8   tempbuf[MAX_TEST_STRING_BUF*2];
    U8   AsciiDisplayBuf[MAX_TEST_STRING_BUF];
#ifndef COOLSAND_VERSION_DISP
#ifdef MMI_ON_HARDWARE_P
    S16   ErrorCode, Ret;
    CHAR* versionString;
#endif 
#endif 

    EntryNewScreen(SCR_ID_FM_VERSION_SUMMARY, NULL, EntryFMVersionSummaryMenu, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_ID_FM_VERSION_SUMMARY);
    SetParentHandler(MENU_ID_FM_SW_SUMMARY);
    RegisterHighlightHandler(ExecuteCurrHiliteHandler);

#ifdef COOLSAND_VERSION_DISP   
	memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
	sprintf((S8*) AsciiDisplayBuf, "[VERSION]\nMAUI.GEMINI.W09.03\n[BRANCH]:GEMINI\n");
	memset(tempbuf, 0, sizeof(tempbuf));
	AnsiiToUnicodeString((S8*)tempbuf, (S8*)AsciiDisplayBuf);
	UCS2Strcat((S8*)UnicodeDisplayBuf, (const S8*)tempbuf);
	
	memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
	sprintf((S8*) AsciiDisplayBuf, "BUILD:BUILD_NO\nSERIAL#:\nMT012345678901234567\n[BUILD TIME]\n2009/08/15");
	memset(tempbuf, 0, sizeof(tempbuf));
	AnsiiToUnicodeString((S8*)tempbuf, (S8*)AsciiDisplayBuf);
	UCS2Strcat((S8*)UnicodeDisplayBuf, (const S8*)tempbuf);
	
#else

#ifdef MMI_ON_HARDWARE_P
    /* Get Barcode from Nvram */
    if( gBarCodeReadFlag == 0 )
    {
        memset( gBarCode, 0, sizeof(gBarCode) );
        Ret = ReadRecord( NVRAM_EF_BARCODE_NUM_LID, 1, gBarCode, 64, &ErrorCode);
        if(!(ErrorCode == NVRAM_READ_SUCCESS && Ret == NVRAM_EF_BARCODE_NUM_SIZE))
        {
            strcpy((S8*)gBarCode, "SN001234567");
        }
        gBarCodeReadFlag = 1;
    }

    // Stack
    memset(UnicodeDisplayBuf, 0, sizeof(UnicodeDisplayBuf));
    memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
    AnsiiToUnicodeString((S8*)AsciiDisplayBuf, "[Stack]: ");
    UCS2Strcpy((S8*)UnicodeDisplayBuf, (const S8 *)AsciiDisplayBuf);

    memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
    versionString = csw_VerGetString(CSW_VER_MODULE_ID_STACK);
    if (versionString != NULL && strlen(versionString) < MAX_TEST_STRING_BUF)
    {
        sprintf((S8*) AsciiDisplayBuf, "%s - r%u\n", versionString,
            (unsigned int)csw_VerGetRevision(CSW_VER_MODULE_ID_STACK));
    }
    memset(tempbuf, 0, sizeof(tempbuf));
    AnsiiToUnicodeString((S8*)tempbuf, (S8*)AsciiDisplayBuf);
    UCS2Strcat((S8*)UnicodeDisplayBuf, (const S8*)tempbuf);

    // CSW
    memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
    AnsiiToUnicodeString((S8*)AsciiDisplayBuf, "[CSW]: ");
    UCS2Strcat((S8*)UnicodeDisplayBuf, (const S8 *)AsciiDisplayBuf);

    memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
    versionString = csw_VerGetString(CSW_VER_MODULE_ID_CSW);
    if (versionString != NULL && strlen(versionString) < MAX_TEST_STRING_BUF)
    {
        sprintf((S8*) AsciiDisplayBuf, "%s - r%u\n", versionString,
            (unsigned int)csw_VerGetRevision(CSW_VER_MODULE_ID_CSW));
    }
    memset(tempbuf, 0, sizeof(tempbuf));
    AnsiiToUnicodeString((S8 *)tempbuf, (S8 *) AsciiDisplayBuf);
    UCS2Strcat((S8 *) UnicodeDisplayBuf, (const S8 *)tempbuf);

    // MMI
    memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
    AnsiiToUnicodeString((S8*)AsciiDisplayBuf, "[MMI]: ");
    UCS2Strcat((S8*)UnicodeDisplayBuf, (const S8 *)AsciiDisplayBuf);

    memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
    versionString = csw_VerGetString(CSW_VER_MODULE_ID_MMI);
    if (versionString != NULL && strlen(versionString) < MAX_TEST_STRING_BUF)
    {
        sprintf((S8*) AsciiDisplayBuf, "%s - r%u\n", versionString,
            (unsigned int)csw_VerGetRevision(CSW_VER_MODULE_ID_MMI));
    }
    memset(tempbuf, 0, sizeof(tempbuf));
    AnsiiToUnicodeString((S8 *)tempbuf, (S8 *) AsciiDisplayBuf);
    UCS2Strcat((S8 *) UnicodeDisplayBuf, (const S8 *)tempbuf);

    // RFD (used to be PHY Config)
    memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
    AnsiiToUnicodeString((S8*)AsciiDisplayBuf, "[RFD]: ");
    UCS2Strcat((S8*)UnicodeDisplayBuf, (const S8 *)AsciiDisplayBuf);

    memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
    versionString = csw_VerGetString(CSW_VER_MODULE_ID_RFD);
    if (versionString != NULL && strlen(versionString) < MAX_TEST_STRING_BUF)
    {
        sprintf((S8*) AsciiDisplayBuf, "%s - r%u\n", versionString,
            (unsigned int)csw_VerGetRevision(CSW_VER_MODULE_ID_RFD));
    }
    memset(tempbuf, 0, sizeof(tempbuf));
    AnsiiToUnicodeString((S8 *)tempbuf, (S8 *) AsciiDisplayBuf);
    UCS2Strcat((S8 *) UnicodeDisplayBuf, (const S8 *)tempbuf);

    // HAL (used to be PHY)
    memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
    AnsiiToUnicodeString((S8*)AsciiDisplayBuf, "[HAL]: ");
    UCS2Strcat((S8*)UnicodeDisplayBuf, (const S8 *)AsciiDisplayBuf);

    memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
    versionString = csw_VerGetString(CSW_VER_MODULE_ID_HAL);
    if (versionString != NULL && strlen(versionString) < MAX_TEST_STRING_BUF)
    {
        sprintf((S8*) AsciiDisplayBuf, "%s - r%u\n", versionString,
            (unsigned int)csw_VerGetRevision(CSW_VER_MODULE_ID_HAL));
    }
    memset(tempbuf, 0, sizeof(tempbuf));
    AnsiiToUnicodeString((S8 *)tempbuf, (S8 *) AsciiDisplayBuf);
    UCS2Strcat((S8 *) UnicodeDisplayBuf, (const S8 *)tempbuf);
    
#else
    /*For PC Simulatior*/
    
    if( gBarCodeReadFlag == 0 )
    {
        memset( gBarCode, 0, sizeof(gBarCode) );
        strcpy((S8*)gBarCode, "SN001234567");
        gBarCodeReadFlag = 1;
    }

    memset(UnicodeDisplayBuf, 0, sizeof(UnicodeDisplayBuf));
    memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
    AnsiiToUnicodeString((S8*)AsciiDisplayBuf, "[VERSION] \n");
    UCS2Strcpy((S8*)UnicodeDisplayBuf, (const S8 *)AsciiDisplayBuf);

    //Lisen 04072005
    memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
    AnsiiToUnicodeString((S8*)AsciiDisplayBuf, "[BRANCH] \n");
    UCS2Strcat((S8*)UnicodeDisplayBuf, (const S8 *)AsciiDisplayBuf);

    memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
    sprintf((S8*) AsciiDisplayBuf, "SW: CSD.Wyy.ww\n");
    memset(tempbuf, 0, sizeof(tempbuf));
    AnsiiToUnicodeString((S8*)tempbuf, (S8*)AsciiDisplayBuf);
    UCS2Strcat((S8*)UnicodeDisplayBuf, (const S8*)tempbuf);

    memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
    sprintf((S8*) AsciiDisplayBuf, "BUILD: CSD.abcdefgh\n");
    memset(tempbuf, 0, sizeof(tempbuf));
    AnsiiToUnicodeString((S8*)tempbuf, (S8*)AsciiDisplayBuf);
    UCS2Strcat((S8*)UnicodeDisplayBuf, (const S8*)tempbuf);

    memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
    sprintf((S8*) AsciiDisplayBuf, "SERIAL#: %s\n", (S8*)gBarCode);
    memset(tempbuf, 0, sizeof(tempbuf));
    AnsiiToUnicodeString((S8*)tempbuf, (S8*)AsciiDisplayBuf);
    UCS2Strcat((S8*)UnicodeDisplayBuf, (const S8*)tempbuf);

    memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
    sprintf((S8*) AsciiDisplayBuf, "[BUILD TIME] \n");
    memset(tempbuf, 0, sizeof(tempbuf));
    AnsiiToUnicodeString((S8*)tempbuf, (S8*)AsciiDisplayBuf);
    UCS2Strcat((S8*)UnicodeDisplayBuf, (const S8*)tempbuf);

    memset(AsciiDisplayBuf, 0, sizeof(AsciiDisplayBuf));
    sprintf((S8*) AsciiDisplayBuf, "MMM DD HH:MM yyyy\n");
    memset(tempbuf, 0, sizeof(tempbuf));
    AnsiiToUnicodeString((S8*)tempbuf, (S8*)AsciiDisplayBuf);
    UCS2Strcat((S8*)UnicodeDisplayBuf, (const S8*)tempbuf);

#endif
#endif

    ShowCategory74Screen(STR_ID_FM_SW_SUMMARY, 0, 0, 0, STR_GLOBAL_BACK,0, 
        (PU8) UnicodeDisplayBuf, sizeof(UnicodeDisplayBuf), guiBuffer );

    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
#if defined(__PROJECT_GALLITE_C01__)

void EntryFMVersionSwMenu(void)	
{
	U8*	guiBuffer;
    U8   tempbuf[MAX_TEST_STRING_BUF*2];

    EntryNewScreen(SCR_ID_FM_SW_VER, NULL, EntryFMVersionSwMenu, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_ID_FM_SW_VER);
    SetParentHandler(MENU_ID_FM_VERSION);
    RegisterHighlightHandler(ExecuteCurrHiliteHandler);


	memset(UnicodeDisplayBuf, 0, sizeof(UnicodeDisplayBuf));
	memset(tempbuf, 0, sizeof(tempbuf));
	AnsiiToUnicodeString((S8*)tempbuf, (S8*)sw_version);
	UCS2Strcpy((S8*)UnicodeDisplayBuf, (const S8*)tempbuf);
	
    ShowCategory74Screen(STR_ID_FM_SW_VER, 0, 0, 0, STR_GLOBAL_BACK,0, 
        (PU8) UnicodeDisplayBuf, sizeof(UnicodeDisplayBuf), guiBuffer );

    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

void EntryFMVersionHwMenu(void)	
{
	U8*	guiBuffer;
    U8   tempbuf[MAX_TEST_STRING_BUF*4];

    EntryNewScreen(SCR_ID_FM_HW_VER, NULL, EntryFMVersionHwMenu, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_ID_FM_HW_VER);
    SetParentHandler(MENU_ID_FM_VERSION);
    RegisterHighlightHandler(ExecuteCurrHiliteHandler);


	memset(UnicodeDisplayBuf, 0, sizeof(UnicodeDisplayBuf));
	memset(tempbuf, 0, sizeof(tempbuf));
	AnsiiToUnicodeString((S8*)tempbuf, (S8*)hw_version);
	UCS2Strcpy((S8*)UnicodeDisplayBuf, (const S8*)tempbuf);
	
    ShowCategory74Screen(STR_ID_FM_HW_VER, 0, 0, 0, STR_GLOBAL_BACK,0, 
        (PU8) UnicodeDisplayBuf, sizeof(UnicodeDisplayBuf), guiBuffer );

    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
#endif
/*****************************************************************
 **  Version Number
 *****************************************************************/
/**************************************************************
**	FUNCTION NAME		: FM_PrepareVersionPopUp
**
**  	PURPOSE				: Helper function for version popup
**
**	INPUT PARAMETERS	: list
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_PrepareVersionPopUp(UI_string_type *list)
{
	U8      i=0;
	S8		TempString[MAX_SUB_MENU_SIZE];

#ifdef MMI_ON_HARDWARE_P
	S16     ErrorCode, Ret;
	version_struct ver_struct;
#endif

	memset( subMenuData, 0, sizeof(subMenuData) );

#if( defined(MMI_ON_HARDWARE_P) && !defined(WIN32) )

	/* Get Barcode from Nvram */
    if( gBarCodeReadFlag == 0 )
    {
       memset( gBarCode, 0, sizeof(gBarCode) );
       Ret = ReadRecord( NVRAM_EF_BARCODE_NUM_LID, 1, gBarCode, 64, &ErrorCode);
       if(!(ErrorCode == NVRAM_READ_SUCCESS && Ret == NVRAM_EF_BARCODE_NUM_SIZE))
       {
          strcpy((S8*)gBarCode, "SN001234567");
       }
       gBarCodeReadFlag = 1;
    }

	/* Get sturctured version number */
	INT_VersionNumbers(&ver_struct);

       // Lisen 04072005
	/* MCU SW Version*/
	memset(TempString, 0, sizeof(TempString) );
    memcpy(TempString, ver_struct.mcu_sw, MAX_SUB_MENU_SIZE-2);
    AnsiiToUnicodeString((S8*)subMenuData[1], (S8*)TempString);

	/* Build Version */
	memset(TempString, 0, sizeof(TempString) );
    memcpy(TempString, build_date_time() , MAX_SUB_MENU_SIZE-2);

    #ifdef TGT_AUTOGEN_MODULE_NAME
	    AnsiiToUnicodeString((S8*)subMenuData[3], (S8*)TempString);
    #else
	    AnsiiToUnicodeString((S8*)subMenuData[2], (S8*)TempString);
    #endif
	for( i=0; i<MAX_VER_NUM; i++)
		list[i] = (UI_string_type )subMenuData[i];

#else
	/*For PC Simulatior*/

    if( gBarCodeReadFlag == 0 )
    {
       memset( gBarCode, 0, sizeof(gBarCode) );
       strcpy((S8*)gBarCode, "SN001234567");
       gBarCodeReadFlag = 1;
    }

	AnsiiToUnicodeString((S8*)subMenuData[1],(S8*)"Test Version");
	AnsiiToUnicodeString((S8*)subMenuData[2],(S8*)"HW Module");
	AnsiiToUnicodeString((S8*)subMenuData[3],(S8*)"May 6 12:00  2011");

#if defined(__PROJECT_GALLITE_C01__)
	/* BB Chip Ver. */
	memset(TempString, 0, sizeof(TempString) );
	AnsiiToUnicodeString((S8*)subMenuData[1],(S8*)"CSD_BB.V0");

	/* DSP Code Ver. */
	memset(TempString, 0, sizeof(TempString) );
	AnsiiToUnicodeString((S8*)subMenuData[2],(S8*)"CSD_DSP.V0");

	/* DSP Patch Ver. */
	memset(TempString, 0, sizeof(TempString) );
	AnsiiToUnicodeString((S8*)subMenuData[3],(S8*)"CSD_DSP_P.V0");

	/* MCU (SW Ver.) */
	memset(TempString, 0, sizeof(TempString) );
	AnsiiToUnicodeString((S8*)subMenuData[4],(S8*)"Branch CSD_SW.V0");

	/* MS Board (HW Ver.) */
	memset(TempString, 0, sizeof(TempString) );
	AnsiiToUnicodeString((S8*)subMenuData[5],(S8*)"CSD_HW.V0");

	/* Melody Ver. */
	memset(TempString, 0, sizeof(TempString) );
	AnsiiToUnicodeString((S8*)subMenuData[6],(S8*)"Melody.V0");

	/* Serial No. */
	memset(TempString, 0, sizeof(TempString) );
    memcpy(TempString, (S8*)gBarCode, MAX_SUB_MENU_SIZE-2);
	AnsiiToUnicodeString((S8*)subMenuData[7], (S8*)TempString);

	/* Build Version */
	memset(TempString, 0, sizeof(TempString) );
	AnsiiToUnicodeString((S8*)subMenuData[8],(S8*)"Oct 25 19:08  2003");
#endif

	for( i=0; i<MAX_VER_NUM; i++)
		list[i] = (UI_string_type) subMenuData[i];
#endif
}


/**************************************************************
**	FUNCTION NAME		: HightlightFMVersionHdlr
**
**  	PURPOSE				: Highlight function for version 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMVersionHdlr(void)
{
	SetLeftSoftkeyFunction(EntryFMVersionMenu,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	change_left_softkey(STR_GLOBAL_OK,0);
	redraw_left_softkey();
}


/**************************************************************
**	FUNCTION NAME		: EntryFMVersionMenu
**
**  	PURPOSE				: Entry function for Version info menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMVersionMenu(void)
{
	U16				ItemList[32];
	U16				ItemIcons[32];
	U16				nItems;
	U8*				guiBuffer;
	S32				i;
	UI_string_type	PopUpList[32];

	EntryNewScreen(SCR_ID_FM_VERSION, NULL, EntryFMVersionMenu, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_FM_VERSION);
	nItems = GetNumOfChild(MENU_ID_FM_VERSION);
	GetSequenceStringIds(MENU_ID_FM_VERSION,ItemList);
	SetParentHandler(MENU_ID_FM_VERSION);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	FM_PrepareVersionPopUp(PopUpList);

	ShowCategory52Screen(	STR_ID_FM_VERSION,0,
							(U16)STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: CSD_itoa
**
**  	PURPOSE				: Helper function for integare to ascii transfer
**
**	INPUT PARAMETERS	: TempString, val
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
static void CSD_itoa(S8 *TempString, S32 val)
{
    if (val <= 0) 
    {
       TempString[0] = '0'; 
       return;
    }
    else if (val <= 9)
    {
       TempString[0] = (S8) ('0' + val); 
       return;
    }
    else
    {
       S32 v, digits, pos;
       for (v = val, digits = 0; v>0; v=v/10)
    	  digits++;
    
       for (v = val, pos=digits-1; 
              digits > 0; 
              v=v/10)
       {
          TempString[pos] = '0' + v%10;
          digits--; 
          pos--;
       }
    }
}


/**************************************************************
**	FUNCTION NAME		: FM_PrepareResourceBinPopUp
**
**  	PURPOSE				: Helper function for reosurce info popup
**
**	INPUT PARAMETERS	: list
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_PrepareResourceBinPopUp(UI_string_type *list)
{
	U8      i=0;
    S8     version_string[] = "version ";

#if( defined(MMI_ON_HARDWARE_P) && !defined(WIN32) )
	S8		TempString[MAX_SUB_MENU_SIZE];
	S8     *temp; 

	memset( subMenuData, 0, sizeof(subMenuData) );
//Lisen 06052005
	/* LangPack Ver# */
   	memset(TempString, 0, sizeof(TempString) );
       memcpy(TempString, version_string, sizeof(version_string));
       temp = TempString + sizeof(version_string) -1;
       CSD_itoa((S8*)temp, CURRENT_LANG_PACK_RES_VERSION);
       AnsiiToUnicodeString((S8*)subMenuData[0], (S8*)TempString);

	/* CustPack Ver# */
   	memset(TempString, 0, sizeof(TempString) );
       memcpy(TempString, version_string, sizeof(version_string));
       temp = TempString + sizeof(version_string) -1;

      //if (custpack_nvram_header_ptr->version == 0)
          CSD_itoa((S8*)temp, CURRENT_CUSTPACK_RES_VERSION);
       //else
      //memcpy(temp, custpack_nvram_header_ptr->version, NVRAM_EF_CUSTPACK_VERNO_SIZE);

       AnsiiToUnicodeString((S8*)subMenuData[1], (S8*)TempString);

	for( i=0; i<2; i++)
		list[i] = (UI_string_type )subMenuData[i];

#else
	memset( subMenuData, 0, sizeof(subMenuData) );
	/*For PC Simulatior*/

	/* LangPack Ver# */
	AnsiiToUnicodeString((S8*)subMenuData[0],(S8*)"version 0");

	/* CustPack Ver# */
	AnsiiToUnicodeString((S8*)subMenuData[1],(S8*)"version 0");

	for( i=0; i<2; i++)
		list[i] = (UI_string_type) subMenuData[i];
#endif
}


/**************************************************************
**	FUNCTION NAME		: HightlightFMResourceBinHdlr
**
**  	PURPOSE				: Highlight function for resource 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMResourceBinHdlr(void)
{
	SetLeftSoftkeyFunction(EntryFMResourceBinMenuHdlr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	change_left_softkey(STR_GLOBAL_OK,0);
	redraw_left_softkey();
}


/**************************************************************
**	FUNCTION NAME		: EntryFMResourceBinMenuHdlr
**
**  	PURPOSE				: Entry function for Resource BIN info menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMResourceBinMenuHdlr(void)
{
	U8*                      guiBuffer;
	UI_string_type	PopUpList[32];
	U16				ItemList[32];
	U16				ItemIcons[32];
	U16				nItems;
	S32				i;

	EntryNewScreen(SCR_ID_FM_RESOURCE_BIN, NULL, EntryFMResourceBinMenuHdlr, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_FM_RESOURCE_BIN);		 
	nItems = GetNumOfChild(MENU_ID_FM_RESOURCE_BIN);
	GetSequenceStringIds(MENU_ID_FM_RESOURCE_BIN,ItemList);
	SetParentHandler(MENU_ID_FM_RESOURCE_BIN);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	FM_PrepareResourceBinPopUp(PopUpList);
	
	ShowCategory52Screen(	STR_ID_FM_RESOURCE_BIN,0,
							0,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);

}

/*****************************************************************
 **  LED
 *****************************************************************/
/**************************************************************
**	FUNCTION NAME		: HightlightFMLEDHdlr
**
**  	PURPOSE				: Highlight function for LED
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMLEDHdlr(void)
{
	SetLeftSoftkeyFunction(EntryFMLEDMenuHdlr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	change_left_softkey(STR_GLOBAL_OK,0);
	redraw_left_softkey();
}


/**************************************************************
**	FUNCTION NAME		: HighlightFMOnCertainLED
**
**  	PURPOSE				: Highlight function for certain LED
**
**	INPUT PARAMETERS	: nIndex
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightFMOnCertainLED(S32 nIndex)
{
	gFM_HighlightIdx = (U8) nIndex;
   StopTimer(FM_AUTO_TEST_COMMNON_TIMER);
   
   if( FM_IS_SET(FM_LEDState, gFM_HighlightIdx) ) 
   {
      change_left_softkey(STR_GLOBAL_OFF,0);
   }
   else
   {
      change_left_softkey(STR_GLOBAL_ON,0);
   }
   redraw_left_softkey();

}


/**************************************************************
**	FUNCTION NAME		: FM_PrepareLEDList
**
**  	PURPOSE				: Helper function for LED info
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_PrepareLEDList(void)
{
   U16 i;
   S8 buffer[32];

   memset(subMenuData, 0, sizeof(subMenuData));
   memset(subMenuDataPtrs, 0, sizeof(subMenuDataPtrs));

   for(i=0; i<MAX_FM_LED_MENU_NUM;i++)
   {
      sprintf(buffer,"%s", FM_LED_Menu[i] );
      AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
      subMenuDataPtrs[i] = subMenuData[i];
   }
}


/**************************************************************
**	FUNCTION NAME		: EntryFMLEDMenuHdlr
**
**  	PURPOSE				: Entry function for LED menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMLEDMenuHdlr(void)
{
	U8* guiBuffer;

	EntryNewScreen(SCR_ID_FM_LED, NULL, EntryFMLEDMenuHdlr, NULL);

	guiBuffer = GetCurrGuiBuffer (SCR_ID_FM_LED);		 

	RegisterHighlightHandler (HighlightFMOnCertainLED);

       FM_PrepareLEDList();
      
       ShowCategory6Screen ( STR_ID_FM_LED, 0,
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0,
						  MAX_FM_LED_MENU_NUM, (PU8*)subMenuDataPtrs,NULL, 0,
						  guiBuffer); 
	SetLeftSoftkeyFunction (FM_ExecuteLEDFunc, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: FM_LEDToggleLSK
**
**  	PURPOSE				: LSK handler function for LED Toggle
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_LEDToggleLSK(void)
{
   FM_STATE_TOGGLE(FM_LEDState, gFM_HighlightIdx);
   if( FM_IS_SET(FM_LEDState, gFM_HighlightIdx) ) 
   {
      change_left_softkey(STR_GLOBAL_OFF,0);
   }
   else
   {
      change_left_softkey(STR_GLOBAL_ON,0);
   }

   redraw_left_softkey();
}


/**************************************************************
**	FUNCTION NAME		: FM_LEDStepFunc
**
**  	PURPOSE				: helper function for alternative LED display
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_LEDStepFunc(void)
{
   if (sequence_counter < LED_LIGHT_LEVEL_MAX)
   {
	   switch( gFM_HighlightIdx )
	   {
	   case MAINLCD_BL: /* Main LCM BL */
			   FM_SetGpioReq( GPIO_DEV_LED_MAINLCD, sequence_counter );
			   StartTimer(FM_AUTO_TEST_COMMNON_TIMER, 200, FM_LEDStepFunc);
		          break;
	#ifdef __MMI_SUBLCD__
	   case SUBLCD_BL: /* Sub LCM BL */
			   FM_SetGpioReq( GPIO_DEV_LED_SUBLCD, sequence_counter );
			   StartTimer(FM_AUTO_TEST_COMMNON_TIMER, 200, FM_LEDStepFunc);
		          break;
	#endif
	   case KEYPAD_LED: /* Keypad LED */
 			   FM_SetGpioReq( GPIO_DEV_LED_KEY, sequence_counter );
			   StartTimer(FM_AUTO_TEST_COMMNON_TIMER, 200, FM_LEDStepFunc);
		          break;

	#ifndef __MMI_HIDE_STATUS_LED_R__   
	   case STATUSLED_R: /* Status LED R */
			   FM_SetGpioReq( GPIO_DEV_LED_STATUS_1, sequence_counter );
			   StartTimer(FM_AUTO_TEST_COMMNON_TIMER, 200, FM_LEDStepFunc);
		         break;
	#endif

	#ifndef __MMI_HIDE_STATUS_LED_G__   
	   case STATUSLED_G: /* Status LED G */
			   FM_SetGpioReq( GPIO_DEV_LED_STATUS_2, sequence_counter );
			   StartTimer(FM_AUTO_TEST_COMMNON_TIMER, 200, FM_LEDStepFunc);
		          break;
	#endif

	#ifndef __MMI_HIDE_STATUS_LED_B__   
	   case STATUSLED_B: /* Status LED B */
			   FM_SetGpioReq( GPIO_DEV_LED_STATUS_3, sequence_counter );
			   StartTimer(FM_AUTO_TEST_COMMNON_TIMER, 200, FM_LEDStepFunc);			   
		          break; 
	#endif

	   default:
	                 sequence_counter=0;
                        StopTimer(FM_AUTO_TEST_COMMNON_TIMER);
		          break;
	   }

	   sequence_counter++;
   }
   else
   {
      sequence_counter=0;
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);

 
#if 0
/* under construction !*/
/* under construction !*/
	#ifndef __MMI_HIDE_STATUS_LED_R__   
/* under construction !*/
/* under construction !*/
/* under construction !*/
	#endif
/* under construction !*/
	#ifndef __MMI_HIDE_STATUS_LED_G__   
/* under construction !*/
/* under construction !*/
/* under construction !*/
	#endif
/* under construction !*/
	#ifndef __MMI_HIDE_STATUS_LED_B__   
/* under construction !*/
/* under construction !*/
/* under construction !*/
	#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif      
   }
}


/**************************************************************
**	FUNCTION NAME		: FM_ExecuteLEDFunc
**
**  	PURPOSE				: helper function for executinge LED function
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_ExecuteLEDFunc(void)
{
   FM_LEDToggleLSK();

#ifdef MMI_ON_HARDWARE_P

   sequence_counter=0;
   StopTimer(FM_AUTO_TEST_COMMNON_TIMER);

   switch( gFM_HighlightIdx )
   {
   case MAINLCD_BL: /* Main LCM BL */
		if( FM_IS_SET(FM_LEDState, gFM_HighlightIdx) ) 
		   FM_LEDStepFunc();
		else
	       FM_SetGpioReq( GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL0 );
	    break;
#ifdef __MMI_SUBLCD__
   case SUBLCD_BL: /* Sub LCM BL */
		if( FM_IS_SET(FM_LEDState, gFM_HighlightIdx) ) 
		   FM_LEDStepFunc();
		else
	       FM_SetGpioReq( GPIO_DEV_LED_SUBLCD, LED_LIGHT_LEVEL0 );
	    break;
#endif
   case KEYPAD_LED: /* Keypad LED */
		if( FM_IS_SET(FM_LEDState, gFM_HighlightIdx) ) 
		   FM_LEDStepFunc();
		else
	      FM_SetGpioReq( GPIO_DEV_LED_KEY, LED_LIGHT_LEVEL0 );
	   break;

#ifndef __MMI_HIDE_STATUS_LED_R__   
   case STATUSLED_R: /* Status LED R */
		if( FM_IS_SET(FM_LEDState, gFM_HighlightIdx) ) 
		   FM_LEDStepFunc();
		else
	       FM_SetGpioReq( GPIO_DEV_LED_STATUS_1, LED_LIGHT_LEVEL0 );
	   break;
#endif

#ifndef __MMI_HIDE_STATUS_LED_G__   
   case STATUSLED_G: /* Status LED G */
		if( FM_IS_SET(FM_LEDState, gFM_HighlightIdx) ) 
		   FM_LEDStepFunc();
		else
	       FM_SetGpioReq( GPIO_DEV_LED_STATUS_2, LED_LIGHT_LEVEL0 );
	   break;
#endif

#ifndef __MMI_HIDE_STATUS_LED_B__   
   case STATUSLED_B: /* Status LED B */
		if( FM_IS_SET(FM_LEDState, gFM_HighlightIdx) ) 
		   FM_LEDStepFunc();
		else
	       FM_SetGpioReq( GPIO_DEV_LED_STATUS_3, LED_LIGHT_LEVEL0 );
	   break; 
#endif

   default:
	   break;
   }

#endif

}

/*****************************************************************
 **  LCD
 *****************************************************************/
/**************************************************************
**	FUNCTION NAME		: FM_LCDCertainRskHdlr
**
**  	PURPOSE				: RSK handler function for LCD function
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_LCDCertainRskHdlr(void)
{
   GoBackBothLCDHistory();
}


/**************************************************************
**	FUNCTION NAME		: HightlightFMLCDHdlr
**
**  	PURPOSE				: Handler function for LCD 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMLCDHdlr(void)
{
	SetLeftSoftkeyFunction(EntryFMLCDMenuHdlr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	change_left_softkey(STR_GLOBAL_OK,0);
	redraw_left_softkey();
}


/**************************************************************
**	FUNCTION NAME		: EntryFMLCDMenuHdlr
**
**  	PURPOSE				: Entry function for LCD Menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
extern 	void	(*SUBLCD_ExitCategoryFunction)(void);
void EntryFMLCDMenuHdlr(void)
{	
	U16				ItemList[32];
	U16				ItemIcons[32];
	U16				nItems;
	U8*				guiBuffer;
	S32				i;
	UI_string_type	PopUpList[32];
    #ifdef __MMI_SUBLCD__
	bitmap	*saved_graphics_context=current_LCD_device_bitmap;
        UI_character_type ScrText[16];
    #endif

	//EntryNewScreen(SCR_ID_FM_LCD, ExitFMLCDMenuHdlr, EntryFMLCDMenuHdlr, NULL);
	EntryNewScreen(SCR_ID_FM_LCD, ExitFMLCDMenuHdlr, NULL, NULL);
	entry_full_screen();
	guiBuffer = GetCurrGuiBuffer(SCR_ID_FM_LCD);
	nItems = GetNumOfChild(MENU_ID_FM_LCD);
	GetSequenceStringIds(MENU_ID_FM_LCD,ItemList);
	SetParentHandler(MENU_ID_FM_LCD);
       ClearAllKeyHandler();
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

   /* Main LCD */
	ShowCategory52Screen(	STR_ID_FM_LCD,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

#ifdef __MMI_SUBLCD__
   /* Sub LCD Show Large 88888 Full Screen */

 	// ShowCategory301Screen( (PU8)"", NULL, NULL);

	SUBLCD_ExitCategoryFunction();
	UI_set_sub_LCD_graphics_context();
	clear_screen();
	coolsand_UI_set_font(&MMI_medium_bold_font);
	coolsand_UI_set_text_border_color(coolsand_UI_color(0,0,0));
        // coolsand_UI_set_text_clip(UI_clip_x1,UI_clip_y1,UI_clip_x2,UI_clip_y2);
    AnsiiToUnicodeString((S8*)ScrText, (S8*) "SuLCD");
    coolsand_UI_move_text_cursor(4,20);
	//UI_print_bordered_text(ScrText);
	coolsand_UI_print_text(ScrText);
	
    AnsiiToUnicodeString((S8*)ScrText, (S8*) "TEST");	
    coolsand_UI_move_text_cursor(4,40);
	coolsand_UI_print_text(ScrText);
	//UI_print_bordered_text(ScrText);
	
	//coolsand_UI_BLT_double_buffer(UI_clip_x1,UI_clip_y1,UI_clip_x2,UI_clip_y2);
	coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);

	if(saved_graphics_context==&main_LCD_device_bitmap)
		UI_set_main_LCD_graphics_context();
	else if(saved_graphics_context==&sub_LCD_device_bitmap)
		UI_set_sub_LCD_graphics_context();
#endif

	SetRightSoftkeyFunction(FM_LCDCertainRskHdlr,KEY_EVENT_UP);
}

/**************************************************************
**	FUNCTION NAME		: ExitFMLCDMenuHdlr
**
**  	PURPOSE				: Exit function to change LCD colors
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitFMLCDMenuHdlr(void)
{
	history_t currHistory;

#ifdef __MMI_SUBLCD__
   ShowSubLCDLogoString();
#endif        
	currHistory.scrnID = SCR_ID_FM_LCD;
	currHistory.entryFuncPtr = EntryFMLCDMenuHdlr;
	pfnUnicodeStrcpy((S8*)currHistory.inputBuffer, (S8*)L"");
	GetCategoryHistory (currHistory.guiBuffer);
	AddHistory (currHistory);

}


/*********************
 **  LCD Auto/Certain
 *********************/
/**************************************************************
**	FUNCTION NAME		: HightlightFMLCDCertainHdlr
**
**  	PURPOSE				: Highlight function for LCD certain
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMLCDCertainHdlr(void)
{
	SetLeftSoftkeyFunction(FM_StartCertainLCDTest,KEY_EVENT_UP);
	SetRightSoftkeyFunction(FM_LCDCertainRskHdlr,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: FM_LCDShowColor
**
**  	PURPOSE				: Helper function for LCD showing color
**
**	INPUT PARAMETERS	: type
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_LCDShowColor( S32 type )
{
	switch(type)
	{
	case 0:  /* Auto */
	   sequence_counter = 0;
	   EntryFMColorTest();
	   break;
	case 1:  /* R */

           coolsand_UI_lock_double_buffer();
           FM_HandleInitialColor(255,0,0);
           coolsand_UI_unlock_double_buffer();
           coolsand_UI_BLT_double_buffer(0,0,MAIN_LCD_device_width-1,MAIN_LCD_device_height-1);
           break;

	case 2:  /* G */
           coolsand_UI_lock_double_buffer();
           FM_HandleInitialColor(0,255,0);
           coolsand_UI_unlock_double_buffer();
           coolsand_UI_BLT_double_buffer(0,0,MAIN_LCD_device_width-1,MAIN_LCD_device_height-1);
           break;
           
	case 3:  /* B */
           coolsand_UI_lock_double_buffer();
           FM_HandleInitialColor(0,0,255);
           coolsand_UI_unlock_double_buffer();
           coolsand_UI_BLT_double_buffer(0,0,MAIN_LCD_device_width-1,MAIN_LCD_device_height-1);
           break;
           
	case 4:  /* W */
           coolsand_UI_lock_double_buffer();
           FM_HandleInitialColor(255,255,255);
           coolsand_UI_unlock_double_buffer();
           coolsand_UI_BLT_double_buffer(0,0,MAIN_LCD_device_width-1,MAIN_LCD_device_height-1);
           break;
           
//Lisen 05312005
	case 5:  /* Black */
           coolsand_UI_lock_double_buffer();
           FM_HandleInitialColor(0,0,0);
           coolsand_UI_unlock_double_buffer();
           coolsand_UI_BLT_double_buffer(0,0,MAIN_LCD_device_width-1,MAIN_LCD_device_height-1);
           break;
           
	default:
	    sequence_counter = 0;
           EntryFMColorTest();
           break;
	}

}


/**************************************************************
**	FUNCTION NAME		: FM_StartCertainLCDTest
**
**  	PURPOSE				: Helper function to start certain LCD test
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_StartCertainLCDTest(void)
{
	S32 type;

	type = GetHighlightedItem();

	if( type > 0 )
	{
      ClearAllKeyHandler();
      EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
      ShowCategory7Screen(0, 0,
					     STR_GLOBAL_OK, 0,
					     0, 0,
					     NULL, NULL); 
	  SetLeftSoftkeyFunction (ExitFMColorSetChange, KEY_EVENT_UP);
  	  SetRightSoftkeyFunction (ExitFMColorSetChange, KEY_EVENT_UP);
     SetKeyHandler(ExitFMColorSetChange,KEY_END,KEY_EVENT_UP);
	}

   FM_LCDShowColor( type );


}


/**************************************************************
**	FUNCTION NAME		: FM_HandleInitialColor
**
**  	PURPOSE				: Helper function to hanld initial LCD colors
**
**	INPUT PARAMETERS	: color_r, color_g, color_b
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_HandleInitialColor(U8 color_r, U8 color_g, U8 color_b)
{
   stFontAttribute	debug_print_font={ 0,0,0,SMALL_FONT,0,0 };
   color_t	c;

   c=coolsand_UI_color(color_r,color_g,color_b);
   UI_set_clip(0,0,UI_device_width,UI_device_height);
   gdi_layer_set_source_key(FALSE, 0);
   UI_fill_rectangle(0,0,UI_device_width,UI_device_height,c);
	UI_reset_text_clip();
	UI_set_font(&debug_print_font);
   c=coolsand_UI_color(0,0,0);  
   UI_set_text_color(c);
}


/**************************************************************
**	FUNCTION NAME		: FM_HandleColorSetChange
**
**  	PURPOSE				: Helper function to change LCD colors
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_HandleColorSetChange(void)
{
   /*Wangzl Add Start For 6463 Ver: TBM780  on 2007-8-24 15:36 */
   ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
   /*Wangzl Add End  For 6463 Ver: TBM780  on 2007-8-24 15:36 */
   switch(sequence_counter)
   {
   case 0:
      coolsand_UI_lock_double_buffer();
      sequence_counter++;
      FM_HandleInitialColor(255,0,0);
      StartTimer(FM_LCD_COLOR_CHANGE_TIMER, 1000, FM_HandleColorSetChange);
      coolsand_UI_unlock_double_buffer();
      coolsand_UI_BLT_double_buffer(0,0,MAIN_LCD_device_width-1,MAIN_LCD_device_height-1);
      break;
      
   case 1:
      coolsand_UI_lock_double_buffer();
      sequence_counter++;
      FM_HandleInitialColor(0,255,0);
      StartTimer(FM_LCD_COLOR_CHANGE_TIMER, 1000, FM_HandleColorSetChange);
      coolsand_UI_unlock_double_buffer();
      coolsand_UI_BLT_double_buffer(0,0,MAIN_LCD_device_width-1,MAIN_LCD_device_height-1);
      break;
      
   case 2:
      coolsand_UI_lock_double_buffer();
      sequence_counter++;
      FM_HandleInitialColor(0,0,255);
      StartTimer(FM_LCD_COLOR_CHANGE_TIMER, 1000, FM_HandleColorSetChange);
      coolsand_UI_unlock_double_buffer();
      coolsand_UI_BLT_double_buffer(0,0,MAIN_LCD_device_width-1,MAIN_LCD_device_height-1);
      break;
      
   case 3:
      coolsand_UI_lock_double_buffer();
      sequence_counter++;
      FM_HandleInitialColor(255,255,255);
      StartTimer(FM_LCD_COLOR_CHANGE_TIMER, 1000, FM_HandleColorSetChange);
      coolsand_UI_unlock_double_buffer();
      coolsand_UI_BLT_double_buffer(0,0,MAIN_LCD_device_width-1,MAIN_LCD_device_height-1);
      break;
      
//Lisen 05312005
   case 4:
      coolsand_UI_lock_double_buffer();
      sequence_counter++;
      FM_HandleInitialColor(0,0,0);
      StartTimer(FM_LCD_COLOR_CHANGE_TIMER, 1000, FM_HandleColorSetChange);
      coolsand_UI_unlock_double_buffer();
      coolsand_UI_BLT_double_buffer(0,0,MAIN_LCD_device_width-1,MAIN_LCD_device_height-1);
      break;
      
   default:
      sequence_counter=0;
      StopTimer(FM_LCD_COLOR_CHANGE_TIMER);

      EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
      
      if (autoTestMode == 0) // Display the screen if not in autotest mode
      {
         AnsiiToUnicodeString(EMFMUnicodeDisplayBuf, (S8*)"Pass");
      ShowCategory7Screen( 0, 0,
                           STR_GLOBAL_OK, 0,
                           0, 0,
					     (U8 *)EMFMUnicodeDisplayBuf, NULL); 
      SetKeyHandler(ExitFMColorSetChange,KEY_LSK,KEY_EVENT_UP);
      }
      else
      {
      	   FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_LCD].name);
  	   ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );
		if(!g_test_without_intervenor)
		{
	   		FM_Autotest_set_key_handlers();
		}
		else
		{
			coolsand_UI_start_timer(1000,FM_Handle_Pass_Key_Press);
		}
      }
      return;
      break;
   }
}


/**************************************************************
**	FUNCTION NAME		: ExitFMColorSetChange
**
**  	PURPOSE				: Exit function to change LCD colors
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitFMColorSetChange(void)
{
   sequence_counter=0;
   StopTimer(FM_LCD_COLOR_CHANGE_TIMER);
   GoBackHistory();
}


/**************************************************************
**	FUNCTION NAME		: EntryFMColorTest
**
**  	PURPOSE				: Entry function for color testing
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMColorTest(void)
{
   EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

   ClearAllKeyHandler();
   SetKeyHandler(ExitFMColorSetChange,KEY_END,KEY_EVENT_UP);
   FM_HandleColorSetChange();
	//StartTimer(CM_NOTIFYDURATION_TIMER, 1000, FM_HandleColorSetChange);
}



/*****************************************************************
 **  LCD Contrast
 *****************************************************************/
/**************************************************************
**	FUNCTION NAME		: HightlightFMLCDContrastHdlr
**
**  	PURPOSE				: Highlight function for LCD constrast
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMLCDContrastHdlr(void)
{
	SetLeftSoftkeyFunction(EntryFMLCDContrastMenuHdlr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	change_left_softkey(STR_GLOBAL_OK,0);
	redraw_left_softkey();
}


/**************************************************************
**	FUNCTION NAME		: EntryFMLCDContrastMenuHdlr
**
**  	PURPOSE				: Entry function for LCD contrast menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMLCDContrastMenuHdlr(void)
{	
	U16				ItemList[32];
	U16				ItemIcons[32];
	U16				nItems;
	U8*				guiBuffer;
	S32				i;
	UI_string_type	PopUpList[32];

	EntryNewScreen(SCR_ID_FM_LCD_CONTRAST, NULL, EntryFMLCDContrastMenuHdlr, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_FM_LCD_CONTRAST);
	nItems = GetNumOfChild(MENU_ID_FM_LCD_CONTRAST);
	GetSequenceStringIds(MENU_ID_FM_LCD_CONTRAST,ItemList);
	SetParentHandler(MENU_ID_FM_LCD_CONTRAST);
       ClearAllKeyHandler();
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	ShowCategory52Screen(	STR_ID_FM_LCD_CONTRAST,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}

/*************************/
/* Main/Sub LCD Contrast */
/*************************/
/**************************************************************
**	FUNCTION NAME		: HightlightFMLCDContrastMainHdlr
**
**  	PURPOSE				: Highlight function for Main LCD constrast
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMLCDContrastMainHdlr(void)
{
	PSI_LCDTpye = 0;
	SetLeftSoftkeyFunction(EntryFMLCDContrastSliderHdlr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HightlightFMLCDContrastSubHdlr
**
**  	PURPOSE				: Highlight function for Sub LCD constrast
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMLCDContrastSubHdlr(void)
{

       PSI_LCDTpye = 1;

#ifdef __MMI_SUBLCD__
	SetLeftSoftkeyFunction(EntryFMLCDContrastSliderHdlr,KEY_EVENT_UP);
#else
	SetLeftSoftkeyFunction(NULL,KEY_EVENT_UP);
#endif

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: FM_ShowLCDContrastScreen
**
**  	PURPOSE				: Helpert function to show LCD constrast
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_ShowLCDContrastScreen(void)
{
    UI_character_type text1[32];
    UI_character_type text2[32];
    U8  u8text[32];

	if( PSI_LCDTpye == 0 )
	{
		/* Main LCD */
		sprintf((S8*)u8text, "%d/%d", currentContrast, MAX_CONTRAST_NUM );
	#ifdef __ASCII
		 strcpy((S8*)text1, (S8*)u8text);
	#else
		 AnsiiToUnicodeString((S8*)text1, (S8*)u8text);
	#endif

		ShowCategory205Screen(STR_GLOBAL_OK,0,STR_GLOBAL_BACK,0, text1,IMG_ID_FM_CONTRAST, NULL);
	}
	else
	{  
		/* Sub LCD */
		sprintf((S8*)u8text, "8888\n%d/%d", currentContrast, MAX_CONTRAST_NUM );
	#ifdef __ASCII
		 strcpy((S8*)text1, (S8*)u8text);
	#else
		 AnsiiToUnicodeString((S8*)text1, (S8*)u8text);
         AnsiiToUnicodeString((S8*)text2, (S8*)"Adjusting SubLCD");
	#endif
		//ShowCategory205Screen( FM_OK,0,NULL,0, text2, NULL, NULL);
		ShowCategory301Screen( (PU8)text1, 0, NULL);
#ifdef __MMI_SUBLCD__
      ShowCategory7Screen(0, 0, STR_GLOBAL_OK, 0,STR_GLOBAL_BACK, 0, (U8 *)text2, NULL); 
#endif
	}

	SetLeftSoftkeyFunction (GoBackBothLCDHistory, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackBothLCDHistory, KEY_EVENT_UP);
	SetKeyHandler(FM_LCDContrastLevelUp,KEY_UP_ARROW,KEY_EVENT_UP);
	SetKeyHandler(FM_LCDContrastLevelDown,KEY_DOWN_ARROW,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: EntryFMLCDContrastSliderHdlr
**
**  	PURPOSE				: Entry function for LCD contrast slider
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMLCDContrastSliderHdlr(void)
{
	S16 error;

	ReadValue( NVRAM_SETTING_CONTRAST_LEVEL, &currentContrast, DS_BYTE , &error);
	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
#ifdef __MMI_SUBLCD__
	ExecSubLCDCurrExitHandler();
#endif

	if( PSI_LCDTpye == 0 )
		PhnsetMainLCDContrast((U8)currentContrast);
	else
		PhnsetSubLCDContrast((U8)currentContrast);
      

    ClearAllKeyHandler();
	SetRightSoftkeyFunction (GoBackBothLCDHistory, KEY_EVENT_UP);
	FM_ShowLCDContrastScreen();

}


/**************************************************************
**	FUNCTION NAME		: FM_LCDContrastLevelUp
**
**  	PURPOSE				: Helper function for LCD contrast slider -- UP
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_LCDContrastLevelUp(void)
{

   if (currentContrast < MAX_CONTRAST_NUM - CONTRAST_STEP)
      currentContrast+=CONTRAST_STEP;
   else
      currentContrast=MAX_CONTRAST_NUM;

	if( PSI_LCDTpye == 0 )
		PhnsetMainLCDContrast((U8)currentContrast);
	else
		PhnsetSubLCDContrast((U8)currentContrast);

    FM_ShowLCDContrastScreen();
    SetLeftSoftkeyFunction(FM_LCDSetContrastLevel, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackBothLCDHistory, KEY_EVENT_UP);
	SetKeyHandler(FM_LCDContrastLevelUp,KEY_UP_ARROW,KEY_EVENT_UP);
	SetKeyHandler(FM_LCDContrastLevelDown,KEY_DOWN_ARROW,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: FM_LCDContrastLevelDown
**
**  	PURPOSE				: Helper function for LCD contrast slider -- DOWN
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_LCDContrastLevelDown(void)
{
   if (currentContrast > MIN_CONTRAST_NUM + CONTRAST_STEP)
      currentContrast-=CONTRAST_STEP;
   else
      currentContrast=MIN_CONTRAST_NUM;

	if( PSI_LCDTpye == 0 )
		PhnsetMainLCDContrast((U8)currentContrast);
	else
		PhnsetSubLCDContrast((U8)currentContrast);

	FM_ShowLCDContrastScreen();
	SetLeftSoftkeyFunction (FM_LCDSetContrastLevel, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackBothLCDHistory, KEY_EVENT_UP);
	SetKeyHandler(FM_LCDContrastLevelUp,KEY_UP_ARROW,KEY_EVENT_UP);
	SetKeyHandler(FM_LCDContrastLevelDown,KEY_DOWN_ARROW,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: FM_LCDSetContrastLevel
**
**  	PURPOSE				: Helper function for LCD Set contrast Level
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_LCDSetContrastLevel(void)
{
	S16 error;
	
	WriteValue( NVRAM_SETTING_CONTRAST_LEVEL, &currentContrast, DS_BYTE , &error);
	WriteValue( RESTORE_DEFAULT_CONTRAST_LEVEL, &currentContrast, DS_BYTE , &error);
       GoBackHistory();
}


/**************************************************************
**	FUNCTION NAME		: EntryFMLCDContrastSubHdlr
**
**  	PURPOSE				: Entry function for sub LCD contrast menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMLCDContrastSubHdlr(void)
{

}


/**************************************************************
**	FUNCTION NAME		: ExitFMLCDContrastSubHdlr
**
**  	PURPOSE				: Exit function for sub LCD contrast menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitFMLCDContrastSubHdlr(void)
{
}



/*****************************************************************
 **  Keypad
 *****************************************************************/

/**************************************************************
**	FUNCTION NAME		: HightlightFMKeypadHdlr
**
**  	PURPOSE				: Highlight function for Keypad menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMKeypadHdlr(void)
{
	SetLeftSoftkeyFunction(EntryFMKeypadTest,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	change_left_softkey(STR_GLOBAL_OK,0);
	redraw_left_softkey();
}


/**************************************************************
**	FUNCTION NAME		: FM_IsInKeypadTest
**
**  	PURPOSE				: Helpert function to check if in Keypad test
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: TRUE/FALSE
**
 
**************************************************************/
BOOL FM_IsInKeypadTest(void)
{
    if( InkeyTestingScreen == 1 )
      return MMI_TRUE;
    else
      return MMI_FALSE;
}


//CSD added by JL for 031115 keypad test out of window display at other screen
/**************************************************************
**	FUNCTION NAME		: FM_HandleKeypadEndout
**
**  	PURPOSE				: Helpert function to keypad test out of window display at other screen
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: TRUE/FALSE
**
 
**************************************************************/
void FM_HandleKeypadEndout(void)
{
   EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
   ClearAllKeyHandler();
   GoBackHistory();
}
//CSD end

void FM_Auto_Test_Keypad()
{
	static U8 n_key_test = 0;
	if((n_key_test<N_MAX_KEY_TEST)/*&&(keypad_value[n_key_test]!=KEY_NONE_VALUE)*/)
	{
		if(keypad_value[n_key_test] !=KEY_NONE_VALUE)
		{
			FM_KeypadSet(keypad_value[n_key_test]);
			FM_KeypadCheck();
			if (FM_KeypadPass()==TRUE)
			{
				
				FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_KEYPAD].name);
				ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
									STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
									IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );
				n_key_test = 0;
				coolsand_UI_start_timer(1000,FM_Handle_Pass_Key_Press);
			}
			else
			{
				n_key_test++;
				coolsand_UI_start_timer(500,FM_Auto_Test_Keypad);
			}
		}
		else
		{
			n_key_test++;
			FM_Auto_Test_Keypad();
		}

	}
}
/**************************************************************
**	FUNCTION NAME		: EntryFMKeypadTest
**
**  	PURPOSE				: Entry function for Keypad Test menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMKeypadTest(void)
{
#if defined(__MMI_MAINLCD_220X176__)
	U16 IdleScreenDigits[] = 
	{
		KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,
		KEY_LSK,KEY_RSK,KEY_UP_ARROW,KEY_DOWN_ARROW,KEY_LEFT_ARROW,KEY_RIGHT_ARROW,
		KEY_SEND,KEY_END,KEY_STAR,KEY_POUND,KEY_ENTER,KEY_EXTRA_2,KEY_SMS
	};
#elif defined (__MMI_BLACKBERRY_QWERTY__)
	U16 IdleScreenDigits[] = 
	{




	   KEY_SEND1,	 KEY_SEND2, 		    KEY_LSK,               KEY_RSK,                 KEY_MUSIC,       KEY_END,
	   KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_ENTER,    
	   KEY_1,			 KEY_2,			    KEY_3, 		     KEY_4,			 KEY_5,	        KEY_6,
	   KEY_7,			KEY_8,			    KEY_9,                   KEY_Q,			KEY_A,            KEY_SHIFT_INPUTM,

	   KEY_T,			KEY_Y,			    KEY_U,		     KEY_I, 
			KEY_O, 	        KEY_P,
	   KEY_G,			KEY_H,			    KEY_J,			     KEY_K,			KEY_L,	        KEY_BACKSPACE1,
	   KEY_V,			KEY_B,			    KEY_N,		     KEY_M,			KEY_DOLLAR,  KEY_NEWLINE,
	   KEY_SHIFT1,	KEY_0,			    KEY_SPACE1,	     KEY_SYM,		KEY_CALENDER
};
#else
	U16 IdleScreenDigits[] = 
	{
   KEY_LSK, 
#if defined(__MMI_IP_KEY__) || defined(__MMI_FACTORY_MODE_I_KEY__)   	
   KEY_IP_VALUE, 
#endif   
#if defined(__MMI_CAMERA__)
   KEY_CAMERA, /*wang rui modfify 20081205 begin four fixbug 10640*/
#endif   
   KEY_RSK,
//Lisen 09142005
#if defined(__MMI_WITH_C_KEY__) 
   KEY_CLR_VALUE, 
#endif   
   KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW,
   KEY_SEND, KEY_END,
   KEY_1,KEY_2,KEY_3,
#ifdef __SENDKEY2_SUPPORT__
   KEY_SEND1,
#endif
   KEY_4,KEY_5,KEY_6,
#ifdef __SENDKEY2_SUPPORT__
   KEY_SEND2,
#endif
   KEY_7,KEY_8,KEY_9,
   KEY_STAR,KEY_0,KEY_POUND,KEY_VOL_UP, KEY_VOL_DOWN,
   KEY_MUSIC
	};
#endif
   EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

   entry_full_screen();


   ClearAllKeyHandler();
#if defined(__PROJECT_GALLITE_C01__)
   SetGroupKeyHandler(FM_HandleKeypadTest, (U16*) keypad_value, N_MAX_KEY_TEST/*KEYNUMs*/, KEY_EVENT_UP);
   SetGroupKeyHandler(FM_HandleKeypadTestKeyDown, (U16*) keypad_value, N_MAX_KEY_TEST/*KEYNUMs*/, KEY_EVENT_DOWN);
#else
   SetGroupKeyHandler(FM_HandleKeypadTest, (U16*) IdleScreenDigits, PHYSICAL_KEYs/*KEYNUMs*/, KEY_EVENT_UP);
   SetGroupKeyHandler(FM_HandleKeypadTestKeyDown, (U16*) IdleScreenDigits, PHYSICAL_KEYs/*KEYNUMs*/, KEY_EVENT_DOWN);
#endif

   //CSD added by JL for 031115 keypad test out of window display at other screen
   //SetKeyHandler( GoBackHistory, KEY_END, KEY_EVENT_LONG_PRESS);
   SetKeyHandler(FM_HandleKeypadEndout, KEY_END, KEY_EVENT_LONG_PRESS);
   FM_KeypadInit();
   InkeyTestingScreen = 1;
   if(g_test_without_intervenor)
   {
   	coolsand_UI_start_timer(500,FM_Auto_Test_Keypad);
   }
}


/**************************************************************
**	FUNCTION NAME		: FM_KeypadCheck
**
**  	PURPOSE				: Helpert function to check keypad test 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_KeypadCheck(void)
{
	 
	S16 count = 0, y = 100, index;
	S32 alignment_width=0, alignment_height=0;
	stFontAttribute fontInfo;
   S8 temp1[200], temp2[400];
   bitmap	*saved_graphics_context=current_LCD_device_bitmap;
#ifdef __MMI_MAINLCD_220X176__
	y=40;
#endif
   fontInfo.bold = 0;
	fontInfo.italic = 0;
	fontInfo.underline = 0;

	fontInfo.size = 9;		// (size 8..15 FOR type 2..5 ) & (size 9...15 FOR type 1 )
	fontInfo.type = 1;		// 1...5
	coolsand_UI_lock_double_buffer();

   FM_HandleInitialColor(255,255,255);
//   coolsand_UI_move_text_cursor(0,0); 
	UI_set_main_LCD_graphics_context();

#ifdef __MMI_BLACKBERRY_QWERTY__
	y = 15; //(UI_device_height - Get_StringHeight()*LINE_NUM)>>1;
#else
	y = (UI_device_height - Get_StringHeight()*LINE_NUM)>>1;
#endif	
	for(count=0; count<LINE_NUM; count++)
	{
		memset(temp1, 0, sizeof(temp1));
		for (index=0; index<COLUMN_NUM; index++)
		{
			if (keypad_status[count][index] != KEY_HIDE)  
				strcat(temp1, (S8*)keypad_layout[count][index]);
			else 
				strcat(temp1, (S8*)BLANK);
		}

		AnsiiToUnicodeString(temp2, (S8*)temp1);

		//alignment_width = (S16)Get_StringWidth((U8*)temp2);
		Get_StringWidthHeight((U8*)temp2, (S32 *)&alignment_width, (S32 *)&alignment_height);
		coolsand_UI_set_font(&MMI_small_font);
		coolsand_UI_set_text_clip(UI_clip_x1,UI_clip_y1,UI_clip_x2,UI_clip_y2);
		coolsand_UI_set_text_color(coolsand_UI_color(0,0,0));
		coolsand_UI_move_text_cursor((MAIN_LCD_device_width-alignment_width)/2  , y);
		coolsand_UI_print_text((UI_string_type )temp2);
	#ifdef __MMI_BLACKBERRY_QWERTY__
		y += 12;
	#else
		y += 15;
	#endif
	}

     //RollBack to original LCD
	if(saved_graphics_context==&main_LCD_device_bitmap)
		UI_set_main_LCD_graphics_context();
	else if(saved_graphics_context==&sub_LCD_device_bitmap)
		UI_set_sub_LCD_graphics_context();
     

   coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0,0,MAIN_LCD_device_width-1,MAIN_LCD_device_height-1);
}


/**************************************************************
**	FUNCTION NAME		: FM_KeypadInit
**
**  	PURPOSE				: Helpert function to initialize keypad test 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_KeypadInit(void)
{
   U8 i,j;

   for (i=0; i<LINE_NUM; i++)
   {
      for (j=0; j<COLUMN_NUM; j++)
      {
         if (keypad_value[i*COLUMN_NUM+j]==0xFF)
            keypad_status[i][j] = KEY_NA;// FOR blank dispaly
         else
            keypad_status[i][j] = KEY_SHOW;  
      }
   }

   FM_KeypadCheck();
}


/**************************************************************
**	FUNCTION NAME		: FM_KeypadInit
**
 
**
**	INPUT PARAMETERS	: key_code
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_KeypadSet(U16 key_code)
{
   int i;
   U8 nKeys = sizeof(keypad_value)/sizeof(keypad_value[0]);

	
   for (i = nKeys-1; i >= 0 && i < nKeys; i--)
   {

      if (keypad_value[i]==key_code)
      {
         if (keypad_status[i/COLUMN_NUM][i%COLUMN_NUM] != KEY_HIDE)
         {
           keypad_status[i/COLUMN_NUM][i%COLUMN_NUM] = KEY_HIDE;
           break;
         }
      }
	 

   }
}


/**************************************************************
**	FUNCTION NAME		: FM_KeypadPass
**
**  	PURPOSE				: Helpert function to Check keypad keys 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
U8 FM_KeypadPass(void)
{
   U8 i,j;

   for (i=0; i<LINE_NUM; i++)
   {
      for (j=0; j<COLUMN_NUM; j++)
      {
         if (keypad_status[i][j]==KEY_SHOW)
            return FALSE;
      }
   }
   return TRUE;
}


/**************************************************************
**	FUNCTION NAME		: FM_HandleKeypadTestKeyDown
**
**  	PURPOSE				: Helpert function to Check keypad key down
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_HandleKeypadTestKeyDown(void)
{
    U16 Keycode,Keytype;
    GetkeyInfo(&Keycode,&Keytype);
    FM_SendKeyEventReq((U8)Keytype, (U8)Keycode);
}


/**************************************************************
**	FUNCTION NAME		: FM_HandleKeypadTest
**
**  	PURPOSE				: Helpert function to keypad test
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_HandleKeypadTest(void)
{
	U16 Keycode,Keytype;

   GetkeyInfo(&Keycode,&Keytype);
   FM_SendKeyEventReq((U8)Keytype, (U8)Keycode);

   FM_KeypadSet(Keycode);

   FM_KeypadCheck();

   if (FM_KeypadPass()==TRUE)
   {
      EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
      
      if (autoTestMode == 0)
      {
         AnsiiToUnicodeString(EMFMUnicodeDisplayBuf, (S8*)"Pass");
      ShowCategory7Screen(0, 0,
					     STR_GLOBAL_OK, 0,
					     0, 0,
					     (U8 *)EMFMUnicodeDisplayBuf, NULL); 

      ClearAllKeyHandler();
      InkeyTestingScreen = 0;
	 GoBackHistory();
   }
   else
   {
      	   FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_KEYPAD].name);
  	   ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

	   FM_Autotest_set_key_handlers();
     }
   }

}

/*****************************************************************
 **  LoudSpeak
 *****************************************************************/

/**************************************************************
**	FUNCTION NAME		: HightlightFMLoudSpkHdlr
**
**  	PURPOSE				: Highlight function for Loud Speaker menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMLoudSpkHdlr(void)
{
	SetLeftSoftkeyFunction(EntryFMLoudSpkHdlr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    if(LoudSpkTestOn)
		change_left_softkey(STR_GLOBAL_OFF,0);
	else
		change_left_softkey(STR_GLOBAL_ON,0);

	redraw_left_softkey();

}


/**************************************************************
**	FUNCTION NAME		: EntryFMLoudSpkHdlr
**
**  	PURPOSE				: Entry function for Loud Speaker menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
	#ifdef __FF_AUDIO_SET__
extern void GpioSpeakerModeStart(void);
	#endif
void EntryFMLoudSpkHdlr(void)
{
   if(!LoudSpkTestOn)
   {
   
   FM_RestoreAudio();
      LoudSpkTestOn = TRUE;

   #ifdef MMI_ON_HARDWARE_P
      /* set audio mode to LoudSp */
      //custom_em_set_mode(AUD_MODE_LOUDSPK);
      FM_SendSetAudioModeReq(AUD_MODE_LOUDSPK);
      /* play 1K tone */
      TONE_SetOutputVolume(255, 255);
	 /*Wangzl Modify Start For 6321 Ver: TBM780  on 2007-8-20 10:40 */
	#if 0
	TONE_Play((const L1SP_Tones*)test_tone);
	#else
	#ifdef __FF_AUDIO_SET__
	GpioSpeakerModeStart();
	#endif
	FM_Play_Tone();
	#endif
	/*Wangzl Modify End  For 6321 Ver: TBM780  on 2007-8-20 10:40 */
    #endif

      change_left_softkey(STR_GLOBAL_OFF,0);
   }
   else
   {
      LoudSpkTestOn = FALSE;

   #ifdef MMI_ON_HARDWARE_P
      /* set audio mode to Normal */
      //custom_em_set_mode(AUD_MODE_NORMAL);
       /*Wangzl Modify Start For 6321 Ver: TBM780  on 2007-8-20 10:43 */
       /* stop 1K tone */
      //TONE_Stop();
      FM_Stop_Tone();
      #ifdef __FF_AUDIO_SET__
      GpioSpeakerModeEnd();
      #endif
	FM_SendSetAudioModeReq(AUD_MODE_NORMAL);
       /*Wangzl Modify End  For 6321 Ver: TBM780  on 2007-8-20 10:43 */
    #endif

      change_left_softkey(STR_GLOBAL_ON,0);
   }
   redraw_left_softkey();

}

/*****************************************************************
 **  Echo Loop
 *****************************************************************/
/**************************************************************
**	FUNCTION NAME		: HightlightFMEchoLoopHdlr
**
**  	PURPOSE				: Highlight function for Echo loop menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMEchoLoopHdlr(void)
{
	SetLeftSoftkeyFunction(EntryFMEchoLoopHdlr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    if(EchoLoopTestOn)
      change_left_softkey(STR_GLOBAL_OFF,0);
	else
      change_left_softkey(STR_GLOBAL_ON,0);
	redraw_left_softkey();
}


/**************************************************************
**	FUNCTION NAME		: EntryFMEchoLoopHdlr
**
**  	PURPOSE				: Entry function for Echo Loop menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMEchoLoopHdlr(void)
{
   if(!EchoLoopTestOn)
   {

   	FM_RestoreAudio();
      EchoLoopTestOn = TRUE;

   #ifdef MMI_ON_HARDWARE_P
      /* set audio mode to Normal */
      //custom_em_set_mode(AUD_MODE_NORMAL);
      FM_SendSetAudioModeReq(AUD_MODE_NORMAL);
      /* open loopback */
      kal_sleep_task(kal_milli_secs_to_ticks(200));
	#if 0
	custom_em_set_loopback(TRUE);
	#else
	CFW_EmodAudioTestStart(AUD_MODE_NORMAL);
	#endif
   #endif

      change_left_softkey(STR_GLOBAL_OFF,0);
   }
   else
   {
      EchoLoopTestOn = FALSE;

   #ifdef MMI_ON_HARDWARE_P
      /* set audio mode to Normal */
      //custom_em_set_mode(AUD_MODE_NORMAL);
      FM_SendSetAudioModeReq(AUD_MODE_NORMAL);
      /* open loopback */
      kal_sleep_task(kal_milli_secs_to_ticks(200));

	#if 0
	custom_em_set_loopback(FALSE);
	#else
	CFW_EmodAudioTestEnd();

	#endif
   #endif

      change_left_softkey(STR_GLOBAL_ON,0);

   }
	redraw_left_softkey();
}

/*****************************************************************
 **  Vibrator
 *****************************************************************/
/**************************************************************
**	FUNCTION NAME		: HightlightFMVibratorHdlr
**
**  	PURPOSE				: Highlight function for vibrator menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMVibratorHdlr(void)
{
    if( vib_status )
	{
        SetLeftSoftkeyFunction(FM_StopVibrator,KEY_EVENT_UP);
		change_left_softkey(STR_GLOBAL_OFF,0);
	}
	else
	{
	SetLeftSoftkeyFunction(EntryFMVibratorHdlr,KEY_EVENT_UP);
		change_left_softkey(STR_GLOBAL_ON,0);
	}

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	
	redraw_left_softkey();
}

#ifdef MMI_ON_HARDWARE_P
extern PUBLIC VOID camera_SetFlashL(UINT16);
#ifndef __MMI_DISABLE_DUAL_SENSOR__
extern PUBLIC VOID camera_SwitchSensorReset();
#endif
extern PUBLIC VOID camera_Reserve();
#endif
 void FM_FlashOn(void)
{
#ifdef MMI_ON_HARDWARE_P
    //Load camera drivers before turn on the flash light!
    camera_Reserve();
    camera_SetFlashL(2);
#endif
#ifndef __MMI_DISABLE_DUAL_SENSOR__
	flash_status = TRUE;
	SetLeftSoftkeyFunction(FM_FlashOff,KEY_EVENT_UP);
#endif
	change_left_softkey(STR_GLOBAL_OFF,0);
	redraw_left_softkey();
}
 void FM_FlashOff(void)
{
#ifdef MMI_ON_HARDWARE_P
    camera_SetFlashL(0);
#endif
#ifndef __MMI_DISABLE_DUAL_SENSOR__
	flash_status = FALSE;
	SetLeftSoftkeyFunction(FM_FlashOn,KEY_EVENT_UP);
#endif
	change_left_softkey(STR_GLOBAL_ON,0);
	redraw_left_softkey();
}

void HighLightFMFlashHdlr(void)
{
#ifdef MMI_ON_HARDWARE_P
#ifndef __MMI_DISABLE_DUAL_SENSOR__
    camera_SwitchSensorReset();
#endif
#endif
#ifndef __MMI_DISABLE_DUAL_SENSOR__
	if(flash_status)
	{
		SetLeftSoftkeyFunction(FM_FlashOff,KEY_EVENT_UP);
		change_left_softkey(STR_GLOBAL_OFF,0);
	}
	else
	{
		SetLeftSoftkeyFunction(FM_FlashOn,KEY_EVENT_UP);
		change_left_softkey(STR_GLOBAL_ON,0);
	}
#endif
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	redraw_left_softkey();
}
/**************************************************************
**	FUNCTION NAME		: EntryFMVibratorHdlr
**
**  	PURPOSE				: Entry function for vibrator menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMVibratorHdlr(void)
{
   vib = 1;
#ifdef MMI_ON_HARDWARE_P
   FM_SetGpioReq( GPIO_DEV_VIBRATOR, VIBRATOR_ON );
#endif
   SetLeftSoftkeyFunction(FM_StopVibrator,KEY_EVENT_UP);
   change_left_softkey(STR_GLOBAL_OFF,0);
   redraw_left_softkey();
   StartTimer(FM_VIB_TIMER, 1000, FM_HandleVibratorRsp );
}


/**************************************************************
**	FUNCTION NAME		: FM_HandleVibratorRsp
**
**  	PURPOSE				: Helper function to handle vibrator response
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_HandleVibratorRsp(void)
{
   vib_status = KAL_TRUE;
   if( vib == 1)
   {
	   vib = 0;
#ifdef MMI_ON_HARDWARE_P
	   FM_SetGpioReq( GPIO_DEV_VIBRATOR, VIBRATOR_OFF );
#endif
   }
   else
   {
	   vib = 1;
#ifdef MMI_ON_HARDWARE_P
	   FM_SetGpioReq( GPIO_DEV_VIBRATOR, VIBRATOR_ON );
#endif
   }

   StartTimer(FM_VIB_TIMER, 1000, FM_HandleVibratorRsp );
}


/**************************************************************
**	FUNCTION NAME		: FM_StopVibrator
**
**  	PURPOSE				: Helper function to stope vibrator 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_StopVibrator(void)
{
   vib_status = KAL_FALSE;
   StopTimer(FM_VIB_TIMER);
#ifdef MMI_ON_HARDWARE_P
	   FM_SetGpioReq( GPIO_DEV_VIBRATOR, VIBRATOR_OFF );
#endif
   SetLeftSoftkeyFunction(EntryFMVibratorHdlr,KEY_EVENT_UP);
   change_left_softkey(STR_GLOBAL_ON,0);
   redraw_left_softkey();
}

/*****************************************************************
 **  ADC
 *****************************************************************/
/**************************************************************
**	FUNCTION NAME		: HightlightFMADCHdlr
**
**  	PURPOSE				: Highlight function for ADC menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMADCHdlr(void)
{
	SetLeftSoftkeyFunction(EntryFMADCMenu,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	change_left_softkey(STR_GLOBAL_OK,0);
	redraw_left_softkey();

}


/**************************************************************
**	FUNCTION NAME		: FM_PrepareADCList
**
**  	PURPOSE				: Helper function for ADC list preparation 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
extern	GPIO_MAP_ENTRY adc_map_tbl[];
void FM_PrepareADCList(void)
{
   U16 i=0;
   S8 buffer[32];

   gFM_MenuItemNum=0;
   memset(subMenuData, 0, sizeof(subMenuData) );
   memset(subMenuDataPtrs, 0, sizeof(subMenuDataPtrs) );
   for(i=0; i<=ADC_LABELID_MAX;i++)
   {
      if( adc_map_tbl[i].vaild == GPIO_VAILD )
      {
         sprintf(buffer,"%s %d.%d", adc_map_tbl[i].netname, 0, 0);
         AnsiiToUnicodeString((PS8)subMenuData[gFM_MenuItemNum], buffer);
         subMenuDataPtrs[gFM_MenuItemNum] = subMenuData[gFM_MenuItemNum];
         gFM_MenuItemNum++;
      }
   }
}


/**************************************************************
**	FUNCTION NAME		: EntryFMADCMenu
**
**  	PURPOSE				: Entry function for ADC menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMADCMenu(void)
{
	U8* guiBuffer;

	EntryNewScreen(SCR_ID_FM_ADC, ExitFMADCMenu, NULL, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_FM_ADC);		 

   FM_PrepareADCList();

   if(gFM_MenuItemNum<1) gFM_MenuItemNum = 0;

   ShowCategory6Screen (STR_ID_FM_ADC, 0,
						  0, 0,
						  STR_GLOBAL_BACK, 0,
						  gFM_MenuItemNum, (PU8*)subMenuDataPtrs,NULL, 0,
						  guiBuffer); 

	SetLeftSoftkeyFunction (NULL, KEY_EVENT_UP);
	SetRightSoftkeyFunction (FM_StopADCMenu, KEY_EVENT_UP);

#ifdef MMI_ON_HARDWARE_P
#if 0
   SetProtocolEventHandler( FM_UpdateADCHdlr, MSG_ID_MMI_EQ_ADC_ALL_CHANNEL_IND );
   //   SetProtocolEventHandler( NULL, PRT_BATTERY_STATUS_IND );
   SetProtocolEventHandler( EMFMBatteryStatusMsgHandler, PRT_BATTERY_STATUS_IND );
   FM_SendADCStartReq();   
 #else
	 FM_UpdateADCHdlr();
 	coolsand_UI_start_timer(1000, FM_UpdateADCHdlr);
 #endif
#endif
 	
}


/**************************************************************
**	FUNCTION NAME		: FM_UpdateADCHdlr
**
**  	PURPOSE				: Helper function for ADC update 
**
**	INPUT PARAMETERS	: inMsg
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/

void FM_UpdateADCHdlr()
{
	U8 i=0, j=0;
	S8 buffer[32];
	U16 adc_value;
    const char *netname; 
#ifdef MMI_ON_HARDWARE_P
	CONST UINT32 vmicChannel = pmd_GetEarpieceGpadcChannel();
	CONST UINT32 vbatChannel = pmd_GetBatteryGpadcChannel();
#else
	CONST UINT32 vmicChannel = 0;
	CONST UINT32 vbatChannel =  0;
#endif
#if 1
    for (j=0; j<4; j++)
    {
    	if( adc_map_tbl[j].vaild == GPIO_VAILD )
    	{
    		adc_value = PM_GetGpadcValue(PM_GPADC_CHAN_0+j);
            if (PM_GPADC_CHAN_0+j == vbatChannel)
            {
                netname = adc_map_tbl[4].netname;
            }
            else if (PM_GPADC_CHAN_0+j == vmicChannel)
            {
                netname = adc_map_tbl[5].netname;
            }
            else
            {
                netname = adc_map_tbl[j].netname;
            }
    		sprintf(buffer,"%s %d.%03d (V)", netname, (adc_value)/1000,(adc_value)%1000);

    		AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
    		subMenuDataPtrs[i] = subMenuData[i];
    		i++;
    	}
    }
#else
	//mmi_eq_adc_all_channel_ind_struct *adc_struct = (mmi_eq_adc_all_channel_ind_struct *) inMsg;
	CFW_EMOD_BATTERY_INFO batteryinfo;
	memset(&batteryinfo,0x0,sizeof(batteryinfo));
	#if 0
	CFW_EmodGetBatteryInfo(&batteryinfo);
	#else //chenhe,090525 , to use new api to get the value
	batteryinfo.nBatState.nBatteryRealTimeVoltage = PM_GetGpadcValue(PM_GPADC_CHAN_0)*2;
	batteryinfo.nBatState.nBatteryTemperature = PM_GetGpadcValue(PM_GPADC_CHAN_1);;
	batteryinfo.nBatState.nBatteryCurrent = PM_GetGpadcValue(PM_GPADC_CHAN_2);
	batteryinfo.nChargeInfo.nMpc = PM_GetGpadcValue(PM_GPADC_CHAN_3);
	#endif
	if( adc_map_tbl[j].vaild == GPIO_VAILD )
	{
		adc_value = batteryinfo.nBatState.nBatteryRealTimeVoltage;
		sprintf(buffer,"%s %d.%03d (V)", adc_map_tbl[j].netname,  (adc_value)/1000,(adc_value)%1000);

		AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
		subMenuDataPtrs[i] = subMenuData[i];
		i++;
	}

	j++;
	if( adc_map_tbl[j].vaild == GPIO_VAILD )
	{
		adc_value = batteryinfo.nBatState.nBatteryTemperature;
		if(adc_value)
		{
//Fix Bug#11557 for CP1606 by cong.li on 2009.06.11. BTW, now just support ADC chanel 0 to chanel 3.
#if 0		
			sprintf(buffer,"%s %d.%03d (C)", adc_map_tbl[j].netname,  (adc_value)/1000,(adc_value)%1000);
#else
			//chanel 1
			sprintf(buffer,"%s %d", adc_map_tbl[j].netname,  adc_value);
#endif
		}
		else
		{
			sprintf(buffer,"%s %d", adc_map_tbl[j].netname,  adc_value);

//			sprintf(buffer,"%s n/a(C)", adc_map_tbl[j].netname);
		}

		AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
		subMenuDataPtrs[i] = subMenuData[i];
		i++;
	}

	j++;
	if( adc_map_tbl[j].vaild == GPIO_VAILD )
	{
		adc_value = batteryinfo.nBatState.nBatteryCurrent;//这个是VAUX，值是不对的
//Fix Bug#11557 for CP1606 by cong.li on 2009.06.11. BTW, now just support ADC chanel 0 to chanel 3.
#if 0
		sprintf(buffer,"%s %d.%03d (V)", adc_map_tbl[j].netname,  (adc_value)/1000,(adc_value)%1000 );
#else
			//chanel 2
			sprintf(buffer,"%s %d", adc_map_tbl[j].netname,  adc_value);
#endif
		AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
		subMenuDataPtrs[i] = subMenuData[i];
		i++;
	}

	j++;
	if( adc_map_tbl[j].vaild == GPIO_VAILD )
	{
		adc_value = batteryinfo.nChargeInfo.nMpc;//这个是Charging Current,值是不对的
//Fix Bug#11557 for CP1606 by cong.li on 2009.06.11. BTW, now just support ADC chanel 0 to chanel 3.
#if 0
		if( !batteryinfo.nChargeInfo.nBcs)
		sprintf(buffer,"%s n/a",  adc_map_tbl[j].netname );
		else
		sprintf(buffer,"%s %d.%03d (A)", adc_map_tbl[j].netname, (adc_value)/1000,(adc_value)%1000 );
#else
			//chanel 3
			sprintf(buffer,"%s %d", adc_map_tbl[j].netname,  adc_value);
#endif
		AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
		subMenuDataPtrs[i] = subMenuData[i];
		i++;
	}
	j++;
	if( adc_map_tbl[j].vaild == GPIO_VAILD )
	{
		adc_value = batteryinfo.nChargeInfo.nMpc;//这个是Charging voltage,值是不对的
		if( !batteryinfo.nChargeInfo.nBcs)
		sprintf(buffer,"%s n/a",  adc_map_tbl[j].netname );
		else
		sprintf(buffer,"%s %d.%03d (V)", adc_map_tbl[j].netname, (adc_value)/1000,(adc_value)%1000 );

		AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
		subMenuDataPtrs[i] = subMenuData[i];
		i++;
	}
#endif
	ShowCategory6Screen (STR_ID_FM_ADC, 0, 0, 0,STR_GLOBAL_BACK, 0,
						gFM_MenuItemNum, (PU8*)subMenuDataPtrs,NULL, 0,NULL);

	SetRightSoftkeyFunction ( FM_StopADCMenu, KEY_EVENT_UP);
	ReplaceNewScreenHandler(SCR_ID_FM_ADC, ExitFMADCMenu, NULL);
	coolsand_UI_start_timer(1000, FM_UpdateADCHdlr);
 
#if 0
#ifdef MMI_ON_HARDWARE_P
   U8 i=0, j=0;
   S8 buffer[32];
   double adc_value; 
   mmi_eq_adc_all_channel_ind_struct *adc_struct = (mmi_eq_adc_all_channel_ind_struct *) inMsg;

   if( adc_map_tbl[j].vaild == GPIO_VAILD )
   {
      adc_value = (double)adc_struct->vbat;
      sprintf(buffer,"%s %4.2f V", adc_map_tbl[j].netname,  adc_value/1000000.0 );

      AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
      subMenuDataPtrs[i] = subMenuData[i];
      i++;
    }

   j++;
   if( adc_map_tbl[j].vaild == GPIO_VAILD )
   {
      adc_value = (double)adc_struct->bat_temp;
      sprintf(buffer,"%s %4.2f C", adc_map_tbl[j].netname,  adc_value/1000.0 );

      AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
      subMenuDataPtrs[i] = subMenuData[i];
      i++;
   }

   j++;
   if( adc_map_tbl[j].vaild == GPIO_VAILD )
   {
      adc_value = (double)adc_struct->vaux;
      sprintf(buffer,"%s %4.2f V", adc_map_tbl[j].netname,  adc_value/1000000.0 );

      AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
      subMenuDataPtrs[i] = subMenuData[i];
      i++;
   }
   
   j++;
   if( adc_map_tbl[j].vaild == GPIO_VAILD )
   {
      adc_value = (double) adc_struct->charge_current;
      if( (adc_struct->charge_current)  & 0x80000000 )
        sprintf(buffer,"%s n/a",  adc_map_tbl[j].netname );
      else
      sprintf(buffer,"%s %4.2f A", adc_map_tbl[j].netname, (adc_value)/1000000.0 );

      AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
      subMenuDataPtrs[i] = subMenuData[i];
      i++;
    }
     
   j++;
   if( adc_map_tbl[j].vaild == GPIO_VAILD )
   {
      adc_value = (double)adc_struct->vcharge;
      sprintf(buffer,"%s %4.2f V", adc_map_tbl[j].netname,  adc_value/1000000.0 );

      AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
      subMenuDataPtrs[i] = subMenuData[i];
      i++;
   }
   
   ShowCategory6Screen (STR_ID_FM_ADC, 0,
						  NULL, 0,
						  STR_GLOBAL_BACK, 0,
						  gFM_MenuItemNum, (PU8*)subMenuDataPtrs,NULL, 0,
						  NULL);

   SetRightSoftkeyFunction ( FM_StopADCMenu, KEY_EVENT_UP);
   ReplaceNewScreenHandler(SCR_ID_FM_ADC, ExitFMADCMenu, NULL);

#endif  
#endif  
}


/**************************************************************
**	FUNCTION NAME		: FM_StopADCMenu
**
**  	PURPOSE				: Helper function to stop ADC
**
**	INPUT PARAMETERS	: inMsg
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_StopADCMenu(void)
{
#ifdef MMI_ON_HARDWARE_P
#if 0
   SetProtocolEventHandler( NULL, MSG_ID_MMI_EQ_ADC_ALL_CHANNEL_IND );
   SetProtocolEventHandler(BatteryStatusRsp,PRT_BATTERY_STATUS_IND);
   FM_SendADCStopReq();
 #else
   coolsand_UI_cancel_timer(FM_UpdateADCHdlr);
 #endif
#endif
   GoBackHistory();
}


/**************************************************************
**	FUNCTION NAME		: ExitFMADCMenu
**
**  	PURPOSE				: Exit function for ADC menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitFMADCMenu(void)
{
#ifdef MMI_ON_HARDWARE_P
#if 0
    SetProtocolEventHandler( NULL, MSG_ID_MMI_EQ_ADC_ALL_CHANNEL_IND );
    SetProtocolEventHandler(BatteryStatusRsp,PRT_BATTERY_STATUS_IND);
    FM_SendADCStopReq();
#else
	coolsand_UI_cancel_timer(FM_UpdateADCHdlr);
#endif
#endif
}

/*****************************************************************
 **  Receiver
 *****************************************************************/
/**************************************************************
**	FUNCTION NAME		: HightlightFMReceiverHdlr
**
**  	PURPOSE				: Highlight function for Receiver menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMReceiverHdlr(void)
{
	SetLeftSoftkeyFunction(EntryFMReceiverHdlr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    if(ReceiverTestOn)
      change_left_softkey(STR_GLOBAL_OFF,0);
	else
      change_left_softkey(STR_GLOBAL_ON,0);
	redraw_left_softkey();
}


/**************************************************************
**	FUNCTION NAME		: EntryFMReceiverHdlr
**
**  	PURPOSE				: Entry function for Receiver menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMReceiverHdlr(void)
{

   if(!ReceiverTestOn)
   {
   	FM_RestoreAudio();
      ReceiverTestOn = TRUE;

   #ifdef MMI_ON_HARDWARE_P
      /* set audio mode to Normal */
      //custom_em_set_mode(AUD_MODE_NORMAL);
      FM_SendSetAudioModeReq(AUD_MODE_NORMAL);
      /* play 1K tone */
      TONE_SetOutputVolume(255, 255);
	/*Wangzl Modify Start For 6321 Ver: TBM780  on 2007-8-20 11:26 */
	#if 0
	TONE_Play((const L1SP_Tones*)test_tone);
	#else
	DM_SetAudioVolume(7);
	FM_Play_Tone();
	/*Wangzl Modify End  For 62321 Ver: TBM780  on 2007-8-20 11:26 */
	#endif
   #endif

      change_left_softkey(STR_GLOBAL_OFF,0);

   }
   else
   {
      ReceiverTestOn = FALSE;
   #ifdef MMI_ON_HARDWARE_P
      /* set audio mode to Normal */
      //custom_em_set_mode(AUD_MODE_NORMAL);
      
      /* stop 1K tone */
	/*Wangzl Modify Start For 6321 Ver: TBM780  on 2007-8-20 11:27 */
	#if 0
	TONE_Stop();
	#else
	FM_Stop_Tone();
	#endif
	FM_SendSetAudioModeReq(AUD_MODE_NORMAL);
	/*Wangzl Modify End  For 6321 Ver: TBM780  on 2007-8-20 11:27 */
   
   #endif
      change_left_softkey(STR_GLOBAL_ON,0);
      
   }

   redraw_left_softkey();
}

/*****************************************************************
 **  Headset
 *****************************************************************/
/**************************************************************
**	FUNCTION NAME		: HightlightFMHeadsetHdlr
**
**  	PURPOSE				: Highlight function for Headset menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMHeadsetHdlr(void)
{
	SetLeftSoftkeyFunction(EntryFMHeadsetHdlr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    if(HeadsetTestOn)
      change_left_softkey(STR_GLOBAL_OFF,0);
	else
      change_left_softkey(STR_GLOBAL_ON,0);
	redraw_left_softkey();
}


/**************************************************************
**	FUNCTION NAME		: EntryFMHeadsetHdlr
**
**  	PURPOSE				: Entry function for Headset menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMHeadsetHdlr(void)
{
   if(!HeadsetTestOn)
   {
   	  FM_RestoreAudio();
#ifdef MMI_ON_HARDWARE_P       
      Media_Stop();
#endif
      HeadsetTestOn = TRUE;
   #ifdef MMI_ON_HARDWARE_P
      /* set audio mode to Headset */
      //custom_em_set_mode(AUD_MODE_HEADSET);
      FM_SendSetAudioModeReq(AUD_MODE_HEADSET);
      kal_sleep_task(kal_milli_secs_to_ticks(200));
    #if (HEADSET_MIC_TEST)
      /* open loopback */
      EchoLoopTestOn = TRUE;
	  //custom_em_set_loopback(TRUE);
      CFW_EmodAudioTestStart(AUD_MODE_HEADSET);
	#else
    	FM_Play_Tone();
	#endif
   #endif
      change_left_softkey(STR_GLOBAL_OFF,0);

   }
   else
   {
      HeadsetTestOn = FALSE;
   #ifdef MMI_ON_HARDWARE_P
      /* set audio mode to Normal */
      //custom_em_set_mode(AUD_MODE_NORMAL);
      FM_SendSetAudioModeReq(AUD_MODE_NORMAL);
      kal_sleep_task(kal_milli_secs_to_ticks(200));
	#if (HEADSET_MIC_TEST)
      /* close loopback */
	  //custom_em_set_loopback(FALSE);
      CFW_EmodAudioTestEnd();
      EchoLoopTestOn = FALSE;
	#else
	  FM_Stop_Tone();
	#endif
   #endif

      change_left_softkey(STR_GLOBAL_ON,0);

   }
   redraw_left_softkey();
}


/*****************************************************************
 **  Charger
 *****************************************************************/
/**************************************************************
**	FUNCTION NAME		: HightlightFMChargerHdlr
**
**  	PURPOSE				: Highlight function for Charger menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMChargerHdlr(void)
{
	SetLeftSoftkeyFunction(FM_EntryChargerHdlr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    change_left_softkey(STR_GLOBAL_OK,0);
	redraw_left_softkey();
}


/**************************************************************
**	FUNCTION NAME		: FM_EntryChargerHdlr
**
**  	PURPOSE				: Entry function for Charger menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_EntryChargerHdlr(void)
{
#ifdef MMI_ON_HARDWARE_P
      custom_em_gpio_start(12);
#endif     
	EntryFMADCMenu();
}

/*****************************************************************
 **  RingTone
 *****************************************************************/
/**************************************************************
**	FUNCTION NAME		: HightlightFMRingToneHdlr
**
**  	PURPOSE				: Highlight function for Ringtone menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FMRingToneback(void)
{
if(RingToneTestOn)
{
RingToneTestOn = FALSE;
   #ifdef MMI_ON_HARDWARE_P
      //custom_em_set_mode(AUD_MODE_NORMAL);
      /* stop MIDI */
      FM_SendStopAudioReq(0);
      FM_SendSetAudioModeReq(AUD_MODE_NORMAL);
   #endif

      change_left_softkey(STR_GLOBAL_ON,0);
}
   GoBackHistory();
}
void HightlightFMRingToneHdlr(void)
{
	SetLeftSoftkeyFunction(EntryFMRingToneHdlr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(FMRingToneback,KEY_EVENT_UP);
    if(RingToneTestOn)
      change_left_softkey(STR_GLOBAL_OFF,0);
	else
      change_left_softkey(STR_GLOBAL_ON,0);
	redraw_left_softkey();

}


/**************************************************************
**	FUNCTION NAME		: EntryFMRingToneHdlr
**
**  	PURPOSE				: Helper function for Ringtone menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMRingToneHdlr(void)
{
   
   if(!RingToneTestOn)
   {
   	FM_RestoreAudio();
      RingToneTestOn = TRUE;
   #ifdef MMI_ON_HARDWARE_P
      /* set gain */
      Media_SetOutputVolume(255, 255);
      /* play MIDI_1 */
      FM_SendPlayAudioReq(0);
   #endif

      change_left_softkey(STR_GLOBAL_OFF,0);

   }
   else
   {
      RingToneTestOn = FALSE;
   #ifdef MMI_ON_HARDWARE_P
      //custom_em_set_mode(AUD_MODE_NORMAL);
      /* stop MIDI */
      FM_SendStopAudioReq(0);
      FM_SendSetAudioModeReq(AUD_MODE_NORMAL);
   #endif

      change_left_softkey(STR_GLOBAL_ON,0);

   }
   redraw_left_softkey();
}


/**************************************************************
**	FUNCTION NAME		: FM_RestoreAudio
**
**  	PURPOSE				: Helper function for Restoring audio
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_RestoreAudio(void)
{
#ifdef MMI_ON_HARDWARE_P
	if(LoudSpkTestOn||ReceiverTestOn)
	{
		//TONE_Stop();
		FM_Stop_Tone();
		LoudSpkTestOn = FALSE;
		ReceiverTestOn = FALSE;
	}
	if(EchoLoopTestOn||HeadsetTestOn)
	{
		//custom_em_set_loopback(FALSE);
		CFW_EmodAudioTestEnd();
		EchoLoopTestOn = FALSE;
		HeadsetTestOn = FALSE;
	}
	if(RingToneTestOn)
	{
		FM_SendStopAudioReq(0);
		RingToneTestOn = FALSE;
	}

	//CSD Brian modified for deinit special mode, 2003/12/03   
	FM_SendSetAudioModeReq(AUD_MODE_NORMAL);
#endif
}

/*****************************************************************
 **  RTC
 *****************************************************************/
/**************************************************************
**	FUNCTION NAME		: HightlightFMRTCHdlr
**
**  	PURPOSE				: Highlight function for RTC menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMRTCHdlr(void)
{
	SetLeftSoftkeyFunction(EntryFMRTCHdlr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    change_left_softkey(STR_GLOBAL_OK,0);
	redraw_left_softkey();
}


/**************************************************************
**	FUNCTION NAME		: EntryFMRingToneHdlr
**
**  	PURPOSE				: Entry function for Ringtone menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_HandleRTCRsp(void)
{
    StopTimer(FM_RTC_START_TIMER);
    
#ifdef MMI_ON_HARDWARE_P
 
	gFactoryAlarm = 1;
	FactorySetAlarm(3);
//  custom_em_rtc_set(15);
// CSD end
	//ShutdownSystemOperation();
#endif
 
//  QuitSystemOperation();
// CSD end

}


/**************************************************************
**	FUNCTION NAME		: EntryFMRTCHdlr
**
**  	PURPOSE				: Entry function for RTC menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMRTCHdlr(void)
{
    //StartTimer(FM_RTC_START_TIMER, 300, FM_HandleRTCRsp );
    
    ShowCategory21Screen( 0, 0,
						  0, 0,
						  0, 0,
						 STR_ID_FM_RTC_NOTIFY, NULL);
   FM_HandleRTCRsp();
}


/**************************************************************
**	FUNCTION NAME		: ExitFMRTCInitScr
**
**  	PURPOSE				: Exit function for RTC menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitFMRTCInitScr(void)
{
	QuitSystemOperation();
}

/**************************************************************
**	FUNCTION NAME		: HightlightFMMTBFHdlr
**
**  	PURPOSE				: Highlight function for MTBF
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMMTBFHdlr(void)
{
	SetLeftSoftkeyFunction(EntryFMMTBF,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	change_left_softkey(STR_GLOBAL_OK, 0);
	redraw_left_softkey();
}

/**************************************************************
**	FUNCTION NAME		: FM_StopMTBF
**
**  	PURPOSE				: Helper function to stop MTBF
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
static void FM_StopMTBF(void)
{
       TurnOnBacklight(1);
       FM_BACK_TO_NORMAL_STATE();
       FM_RestoreAudio();
       
       GoBackHistory();
}

/**************************************************************
**	FUNCTION NAME		: EntryFMMTBF
**
**  	PURPOSE				: Entry function for MTBF menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMMTBF(void)
{
	U8* guiBuffer;

	EntryNewScreen(SCR_ID_FM_MTBF, ExitFMMTBF, NULL, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_FM_MTBF);		 

       AUD_TEST_ON = 0;
       MTBF_counter = 0;       
       MTBF_state_LCD = 0; 
       MTBF_state_LED = 0; 
       MTBF_state_VIB = 0; 
       
       FM_MTBF();

	ShowCategory7Screen( STR_ID_FM_MTBF, IMG_ID_FM_MTBF,
	                     0, 0, STR_GLOBAL_ABORT,
	                     IMG_GLOBAL_OK, NULL, guiBuffer );

	SetLeftSoftkeyFunction (NULL, KEY_EVENT_UP);
	SetRightSoftkeyFunction (FM_StopMTBF, KEY_EVENT_UP);
}

/**************************************************************
**	FUNCTION NAME		: ExitFMRTCInitScr
**
**  	PURPOSE				: Exit function for RTC menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitFMMTBF(void)
{
     history_t currHistory;

     TurnOnBacklight(1);
     FM_BACK_TO_NORMAL_STATE();

     FM_RestoreAudio();

     currHistory.scrnID = SCR_ID_FM_MTBF;
     currHistory.entryFuncPtr = EntryFMMTBF;
     pfnUnicodeStrcpy((S8*)currHistory.inputBuffer, (S8*)L"");
     GetCategoryHistory (currHistory.guiBuffer);
     AddHistory (currHistory); 
}

//wangrui delete the micro for fix bug 10484 	
//#if (defined(__MSDC_MS__) || defined(__MSDC_SD_MMC__) || defined(__MSDC_MSPRO__))							   

/**************************************************************
**	FUNCTION NAME		: HightlightFMMemoryCardHdlr
**
**  	PURPOSE				: Highlight function for memory card menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMMemoryCardHdlr(void)
{
	SetLeftSoftkeyFunction(EntryFMMemoryCardMenu,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	change_left_softkey(STR_GLOBAL_OK,0);
	redraw_left_softkey();
}


/**************************************************************
**	FUNCTION NAME		: FM_MemoryCardResponseHandler
**
**  	PURPOSE				: Helper function for handling memory card response
**
**	INPUT PARAMETERS	: info
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_MemoryCardResponseHandler(void* info)
{
     Card_Response_Received = TRUE;
     memcpy((void*) &FM_Card_Status, info, sizeof(mmi_fmt_get_msdc_status_rsp_strcut));
     Card_Response_Sent = FALSE;
}


/**************************************************************
**	FUNCTION NAME		: FM_MemoryCardReq
**
**  	PURPOSE				: Helper function for making memory card request
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_MemoryCardReq(void)
{
	
#ifdef MMI_ON_HARDWARE_P
	MYQUEUE	Message;

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_FMT;
	Message.oslMsgId = MSG_ID_MMI_FMT_GET_MSDC_STATUS_REQ;

	ClearAllKeyHandler();

       Card_Response_Received = FALSE;
	FM_Card_Status.present = FALSE;

	Message.oslDataPtr = (oslParaType *)NULL;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);

        Card_Response_Sent = TRUE;
#endif

//#endif
}


/**************************************************************
**	FUNCTION NAME		: EntryFMMemoryCardMenu
**
**  	PURPOSE				: Entry function for MemoryCard menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMMemoryCardMenu(void)
{
//#if (defined(__MSDC_MS__) || defined(__MSDC_SD_MMC__) || defined(__MSDC_MSPRO__))

	U8*	guiBuffer;

#ifdef MMI_ON_HARDWARE_P

       StopTimer(FM_AUTO_TEST_COMMNON_TIMER);

       if (FM_Card_Status.present == TRUE)
       {
          switch (FM_Card_Status.type)
          { 
             case MS_CARD:
                AnsiiToUnicodeString((S8*)EMFMUnicodeDisplayBuf, "\nMS_CARD.");
                break;
             case SD_CARD:
                AnsiiToUnicodeString((S8*)EMFMUnicodeDisplayBuf, "\nSD_CARD.");
                break;
             case MMC_CARD: 
                AnsiiToUnicodeString((S8*)EMFMUnicodeDisplayBuf, "\nMMC_CARD.");
                break;
             default: 
                AnsiiToUnicodeString((S8*)EMFMUnicodeDisplayBuf, "\nError!!!");
          }
       }
       else
       {
          if (Card_Response_Received == FALSE)
          {  // Keep waiting..... 
             if (Card_Response_Sent == FALSE)
             {
    	       FM_MemoryCardReq();
             }
             StartTimer(FM_AUTO_TEST_COMMNON_TIMER, 300, EntryFMMemoryCardMenu);
             AnsiiToUnicodeString((S8*)EMFMUnicodeDisplayBuf, "\nPlease Wait....");
          }
          else
          {  // No Card inserted.
             AnsiiToUnicodeString((S8*)EMFMUnicodeDisplayBuf, "\nNo Card.");
          }
       }
#else
       AnsiiToUnicodeString((S8*)EMFMUnicodeDisplayBuf, "\nMS_CARD.");
#endif

       if (Card_Response_Sent == FALSE)
       {
	 EntryNewScreen(SCR_ID_FM_MEMORY_CARD, NULL, EntryFMMemoryCardMenu, NULL);

         guiBuffer = GetCurrGuiBuffer(SCR_ID_FM_MEMORY_CARD);
       }
       else
       {
         guiBuffer = NULL;
       }
       
	SetParentHandler(MENU_ID_FM_MEMORY_CARD);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory7Screen( STR_ID_FM_MEMORY_CARD, IMG_ID_FM_MEMORY_CARD,
	                     0, 0, STR_GLOBAL_BACK,
	                     IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, guiBuffer );

	SetLeftSoftkeyFunction(NULL, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	
//#endif
}

//#endif

/**************************************************************
**	FUNCTION NAME		: HightlightFMUARTHdlr
**
**  	PURPOSE				: Highlight function for UART test menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMUARTHdlr(void)
{
	SetLeftSoftkeyFunction(EntryFMUARTMenu,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	change_left_softkey(STR_GLOBAL_OK,0);
	redraw_left_softkey();
}


/**************************************************************
**	FUNCTION NAME		: FM_UARTResponseHandler
**
**  	PURPOSE				: Helper function for handling UART response
**
**	INPUT PARAMETERS	: info
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_UARTResponseHandler(void* info)
{
    UART_Response_Received = TRUE;
}


/**************************************************************
**	FUNCTION NAME		: FM_UARTReq
**
**  	PURPOSE				: Helper function for making UART test request
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_UARTReq(void)
{
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
}

#ifdef __FM_UART_TEST_ON_UART1__
#define port_to_test uart_port1
#else
#define port_to_test uart_port2
#endif

/**************************************************************
**	FUNCTION NAME		: FM_WriteReadUART
**
**  	PURPOSE				: Helper function for making UART test 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_WriteReadUART(void)
{
#if 0
#ifdef MMI_ON_HARDWARE_P

#define TEST_INFO_LENGTH 10
    kal_uint8 put_buffer[]="1234567890";
    kal_uint8 get_buffer[TEST_INFO_LENGTH];
    kal_uint32 read_length = 0;
    kal_uint32 send_length;    

    kal_uint8 status = 0;

    send_length = UART_PutBytes( port_to_test, 
       					 (kal_uint8*)put_buffer,
       					 (kal_uint16)TEST_INFO_LENGTH,
       					 MOD_MMI);

    if (send_length != TEST_INFO_LENGTH)  // Send error or incomplete
    {
       DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 0, 1000, 0);
    }
    else
    {
       memset((void*)get_buffer, 0, TEST_INFO_LENGTH);
       kal_sleep_task(100);
	read_length = UART_GetBytes( port_to_test,
 							       get_buffer,
								TEST_INFO_LENGTH,
								&status,
								MOD_MMI);

	if ((read_length == TEST_INFO_LENGTH) && 
		!strncmp((void*) put_buffer, (void*)get_buffer, TEST_INFO_LENGTH))
	{
          DisplayPopup((PU8)GetString(STR_GLOBAL_OK), IMG_GLOBAL_OK, 0, 1000, 0);
	}
	else // Error case
       {  
          DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 0, 1000, 0);
	}
    }
#endif    
#endif //if 0
}

/**************************************************************
**	FUNCTION NAME		: EntryFMUARTMenu
**
**  	PURPOSE				: Entry function for UART test menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMUARTMenu(void)
{
#ifdef MMI_ON_HARDWARE_P

       module_type UART_Owner = 0;

       UART_Owner = UART_GetOwnerID(port_to_test);
       UART_SetOwner(port_to_test, MOD_MMI);

       FM_WriteReadUART();
       UART_SetOwner(port_to_test, UART_Owner);
#else
       DisplayPopup((PU8)GetString(STR_GLOBAL_OK), IMG_GLOBAL_OK, 0, 1000, 0);
#endif
}




/*****************************************************************************
*
* Camera Parameter Adjusment Test
*
*****************************************************************************/
 
/*****************************************************************************
* FUNCTION
*	 mmi_fm_camera_exit_sublcd_screen()
* DESCRIPTION
*   exit camrea sublcd screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_SUBLCD__	
#if defined(__FM_CAMERA_TUNNING__) || defined(__FM_CAMERA_PREVIEW__)
void mmi_fm_camera_exit_sublcd_screen(void)
{
	SubLCDHistoryNode SubLCDHistory;

	mdi_camera_preview_stop();
	SubLCDHistory.entryFuncPtr=mmi_fm_camera_entry_sublcd_screen;
	AddSubLCDHistory(&SubLCDHistory);
}
#endif /* __FM_CAMERA_TUNNING__ ||  __FM_CAMERA_PREVIEW__ */
#endif /* __MMI_SUBLCD__ */	



/*****************************************************************************
* FUNCTION
*	 mmi_fm_camera_entry_sublcd_screen()
* DESCRIPTION
*   entry camera sublcd screen. avoid Main and Sub update at same time
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_SUBLCD__
#if defined(__FM_CAMERA_TUNNING__) || defined(__FM_CAMERA_PREVIEW__)
void mmi_fm_camera_entry_sublcd_screen(void)
{
	ExecSubLCDCurrExitHandler();
	ShowCategory331Screen();
	SetSubLCDExitHandler(mmi_fm_camera_exit_sublcd_screen);
}
#endif /* __FM_CAMERA_TUNNING__ ||  __FM_CAMERA_PREVIEW__ */
#endif /* __MMI_SUBLCD__ */



/*****************************************************************************
* FUNCTION
*	 mmi_fm_camera_clam_close_event_hdlr()
* DESCRIPTION
*   clam close event handler
* PARAMETERS
*	 ptr		IN		ptr
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__) || defined(__FM_CAMERA_PREVIEW__)
U8 mmi_fm_camera_clam_close_event_hdlr(void* ptr)
{
#ifdef __MMI_CLAMSHELL__
	mmi_eq_gpio_detect_ind_struct *gpioDetectInd;
	
	gpioDetectInd = (mmi_eq_gpio_detect_ind_struct *)ptr;
	
	if(gpioDetectInd->gpio_device == EXT_DEV_CLAM_CLOSE)
	{
		GoBackHistory();
		return FALSE;	
	}
#endif /* __MMI_CLAMSHELL__ */

	/* return false means will pass this interrut to default handler */
	return FALSE;	
}
#endif /* __FM_CAMERA_TUNNING__ ||  __FM_CAMERA_PREVIEW__ */


/*****************************************************************************
* FUNCTION
*	 mmi_fm_highlight_camera()
* DESCRIPTION
*   highlight camera item
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__) || defined(__FM_CAMERA_PREVIEW__)
static void mmi_fm_highlight_camera(void)
{
#if defined(__FM_CAMERA_TUNNING__)

	change_left_softkey(STR_GLOBAL_OK,0);
	SetLeftSoftkeyFunction(mmi_fm_entry_camera_screen,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	redraw_left_softkey();
	
#elif defined(__FM_CAMERA_PREVIEW__)

	change_left_softkey(STR_GLOBAL_OK,0);
	SetLeftSoftkeyFunction(mmi_fm_camera_entry_preview_screen,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	redraw_left_softkey();
	
#endif
}
#endif /* __FM_CAMERA_TUNNING__ ||  __FM_CAMERA_PREVIEW__ */




/*****************************************************************************
* FUNCTION
*	 mmi_fm_camera_exit_preview_screen()
* DESCRIPTION
*   exit factory mode camera preview screen
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__FM_CAMERA_PREVIEW__)
void mmi_fm_camera_exit_preview_screen(void)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#if 0
//	mdi_camera_preview_stop();
	VIM_HAPI_SetPreviewMode(VIM_HAPI_PREVIEW_OFF);
	/* shut down camera */
//	mdi_camera_power_off();
	VIM_HAPI_SetWorkMode(VIM_HAPI_MODE_BYPASS);
  	MainLCD->BlockWrite(0,0,LCD_WIDTH-1,LCD_HEIGHT-1);//2007-7-7 fengzb 
	/* set blt base layer only */
	#endif
	/*wangrui add begin 20081208, fix bug10633*/
	      MCI_CamPowerDown();
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);
	/*wangrui add end*/
	
	gdi_layer_set_blt_layer(GDI_LAYER_MAIN_BASE_LAYER_HANDLE, 0, 0, 0);

	/* resume LCD patten */
	StartLEDPatternBackGround();

	/* let MMI can sleep */
	TurnOffBacklight();

	/* un-hook clam close msg */
	SetInterruptEventHandler(NULL, NULL, MSG_ID_MMI_EQ_GPIO_DETECT_IND);

	/* exit sublcd */
#ifdef __MMI_SUBLCD__
	GoBackSubLCDHistory();
#endif
}
#endif /* __FM_CAMERA_PREVIEW__ */


/*****************************************************************************
* FUNCTION
*	 mmi_fm_camera_entry_preview_screen()
* DESCRIPTION
*   exit factory mode camera preview screen
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__FM_CAMERA_PREVIEW__)

static void gobackHistoryAndcamerapwrOff(void)
{
	MCI_CamPowerDown();
	GoBackHistory();	
}
void mmi_fm_camera_entry_preview_screen(void)
{
#ifndef __MMI_DISABLE_DUAL_SENSOR__
extern VOID camera_SwitchSensor(VOID);
#endif
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8 *gui_buffer;
  	mdi_camera_setting_struct	camera_setting_data;
	static BOOL camera_firstin = TRUE;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* entry new screen */
	/*wangrui add begin 20081208, fix bug10633*/   
	EntryNewScreen(SCR_ID_FM_CAMERA, mmi_fm_camera_exit_preview_screen, NULL, NULL);

	gui_buffer = GetCurrGuiBuffer(SCR_ID_FM_CAMERA);
	
	/* entry a full screen app */
	entry_full_screen();
	
	/* entry sublcd */
#ifdef __MMI_SUBLCD__
	ForceSubLCDScreen(mmi_fm_camera_entry_sublcd_screen);
#endif 

	mmi_camera_load_setting();

	/* enable multi-layer */
	gdi_layer_multi_layer_enable();	

	/* stop MMI sleep */
	TurnOnBacklight(0);

#ifdef __MMI_CLAMSHELL__
	/* register event hdlr */
	SetInterruptEventHandler(mmi_fm_camera_clam_close_event_hdlr,  NULL, MSG_ID_MMI_EQ_GPIO_DETECT_IND);
#endif /* __MMI_CLAMSHELL__ */

	/* stop LCD patten */
	StopLEDPatternBackGround();

	/* power on and init hardware module */
#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
        CSD_UI_Free_bmpbuf();
#endif  
	mdi_camera_power_on(); //chenhe for jasper

	/* clear background */
	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();
	gdi_layer_clear(GDI_COLOR_BLACK);

	/* key handler */
	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
	//SetKeyHandler(GoBackHistory, KEY_LSK, KEY_EVENT_UP);	
	SetKeyHandler(gobackHistoryAndcamerapwrOff, KEY_RSK, KEY_EVENT_UP);	

	/* preview parametes */
	camera_setting_data.zoom				= g_camera_feature_zoom[0]; //qiff modify from 10 to 	g_camera_feature_zoom[0] for GS TRUNK BUG:11190
	camera_setting_data.wb					= camera_wb_command_map[g_camera_cntx.setting.wb];	
	camera_setting_data.ev					= camera_ev_command_map[g_camera_cntx.setting.ev];
	camera_setting_data.banding 			= camera_banding_command_map[g_camera_cntx.setting.banding];
	camera_setting_data.night				= g_camera_cntx.setting.night;

	/* default effect */
	camera_setting_data.effect				= camera_effect_command_map[g_camera_cntx.setting.effect];
	camera_setting_data.brightness 		= 128;
	camera_setting_data.saturation 		= 128;
	camera_setting_data.contrast 			= 128;
	camera_setting_data.hue		 			= 0;
	
	camera_setting_data.preview_width	= CAMERA_FACTORY_PREVIEW_WIDTH;
	camera_setting_data.preview_height	= CAMERA_FACTORY_PREVIEW_HEIGHT;	
	
	/* capture parameters - not used */
	camera_setting_data.image_qty			= camera_image_qty_command_map[g_camera_cntx.setting.image_qty];
	camera_setting_data.image_width		= g_camera_cntx.image_width;
	camera_setting_data.image_height		= g_camera_cntx.image_height;	

	camera_setting_data.lcm					= MDI_CAMERA_PREVIEW_LCM_MAINLCD;
	camera_setting_data.preview_rotate	= CAMERA_PREVIEW_MAINLCD_ROTATE;

	gdi_layer_set_blt_layer(GDI_LAYER_MAIN_BASE_LAYER_HANDLE, 
									0,
									0, 
									0);


	gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);

	if(camera_firstin)
	{
		COS_Sleep(50);
		camera_firstin = FALSE;
#ifndef __MMI_DISABLE_DUAL_SENSOR__
        camera_SwitchSensor();
#endif
	}
    else
    { 
#ifndef __MMI_DISABLE_DUAL_SENSOR__
        camera_SwitchSensor();
#endif
		camera_firstin = TRUE;
	}
	g_camera_cntx.last_error 
		= mdi_camera_preview_start(GDI_LAYER_MAIN_BASE_LAYER_HANDLE,
											0,
											0,
											UI_device_width,
											UI_device_height,
											GDI_LAYER_ENABLE_LAYER_0,
											GDI_LAYER_ENABLE_LAYER_0,
											0,  /* this value is use only CAMERA_MODULE_WITH_LCD */
											FALSE,
											&camera_setting_data);
}
#endif /* __FM_CAMERA_PREVIEW__ */


/*****************************************************************************
* FUNCTION
*   mmi_fm_camtune_init
* DESCRIPTION
*   init camera tunning
* PARAMETERS
*	 void
* RETURNS
*   none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
static void mmi_fm_camtune_init(void)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	g_camtune_cntx.group_idx 		= 0;
	g_camtune_cntx.group_count 	= 0;	
	g_camtune_cntx.is_fullscreen 	= FALSE;
	g_camtune_cntx.is_previewing 	= FALSE;	
	g_camtune_cntx.hilight_idx 	= 0;

	g_camtune_cntx.true_false_str[0] = (UI_string_type)GetString((U16)(STR_GLOBAL_OFF));
	g_camtune_cntx.true_false_str[1] = (UI_string_type)GetString((U16)(STR_GLOBAL_ON));		
}
#endif /* __FM_CAMERA_TUNNING__ */


/*****************************************************************************
* FUNCTION
*   mmi_fm_entry_camera_screen
* DESCRIPTION
*   entry factory mode camera screen
* PARAMETERS
*	 void
* RETURNS
*   none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
static void mmi_fm_entry_camera_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16	item_list[16];
	U16	item_icons[16];
	U16	num_item;
	U8		*gui_buffer;
	U8		*popup_list[16];
	S32	i;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_FM_CAMERA, NULL, mmi_fm_entry_camera_screen, NULL);

	gui_buffer 	= GetCurrGuiBuffer(SCR_ID_FM_CAMERA);
	num_item		= GetNumOfChild(MENU_ID_FM_CAMERA);
	
	GetSequenceStringIds(MENU_ID_FM_CAMERA, item_list);
	GetSequenceImageIds(MENU_ID_FM_CAMERA, item_icons);
	SetParentHandler(MENU_ID_FM_CAMERA);

	RegisterHighlightHandler(mmi_fm_camera_highlight_hdlr);

 	for(i = 0; i < num_item; i++)
		item_icons[i] = (U16) (IMG_GLOBAL_L1+i);

	for(i = 0 ; i < num_item; i++)
		popup_list[i] = NULL;

	ConstructHintsList(MENU_ID_FM_CAMERA, popup_list);

	ShowCategory52Screen(STR_ID_FM_CAMERA,	
								MENU_ID_FM_CAMERA,
								STR_GLOBAL_OK,IMG_GLOBAL_OK,
								STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								num_item, 
								item_list,
								item_icons, 
								(U8**)popup_list, 
								0, 
								0,
								gui_buffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
#endif /* __FM_CAMERA_TUNNING__ */



/*****************************************************************************
* FUNCTION
*	 mmi_fm_camera_entry_restore_confirm_screen()
* DESCRIPTION
*   entry restore default confirm screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
static void mmi_fm_camera_entry_restore_confirm_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	DisplayConfirm( STR_GLOBAL_YES, IMG_GLOBAL_YES,
						 STR_GLOBAL_NO, IMG_GLOBAL_NO,
						 get_string(STR_ID_FM_CAMERA_RESTORE_CONFIRM), IMG_GLOBAL_QUESTION, WARNING_TONE); 

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(mmi_fm_camera_restore_confirm_yes,KEY_EVENT_UP);
}
#endif /* __FM_CAMERA_TUNNING__ */



/*****************************************************************************
* FUNCTION
*	 mmi_fm_camera_entry_restore_confirm_yes()
* DESCRIPTION
*   restore defualt confirm yes
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
static void mmi_fm_camera_restore_confirm_yes(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/	
	S16	error;
   S32 	data_size;

	nvram_camera_para_struct *camera_para_p;
	nvram_camera_para_struct *default_camera_para_p;

	
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
	/*----------------------------------------------------------------*/
   camera_para_p 				= (nvram_camera_para_struct*)get_camera_para();
	default_camera_para_p 	= (nvram_camera_para_struct*)get_default_camera_para();
	
	data_size = sizeof(nvram_camera_para_struct);

	memcpy((void*)camera_para_p, (void*)default_camera_para_p, data_size);
	
	WriteRecord(NVRAM_EF_CAMERA_PARA_LID, 1, (void*)camera_para_p, data_size, &error);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	
}
#endif /* __FM_CAMERA_TUNNING__ */


/*****************************************************************************
* FUNCTION
*	 mmi_fm_camera_entry_save_confirm_screen()
* DESCRIPTION
*   entry save confirm screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
static void mmi_fm_camera_entry_save_confirm_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	DisplayConfirm( STR_GLOBAL_YES, IMG_GLOBAL_YES,
						 STR_GLOBAL_NO, IMG_GLOBAL_NO,
						 get_string(STR_ID_FM_CAMERA_SAVE_CONFRIM), IMG_GLOBAL_QUESTION, WARNING_TONE); 

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(mmi_fm_camera_save_confirm_yes,KEY_EVENT_UP);
}
#endif /* __FM_CAMERA_TUNNING__ */



/*****************************************************************************
* FUNCTION
*	 mmi_fm_camera_save_confirm_yes()
* DESCRIPTION
*   restore defualt confirm yes
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
static void mmi_fm_camera_save_confirm_yes(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16	error;

	nvram_camera_para_struct *camera_para_p;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
	/*----------------------------------------------------------------*/			
   camera_para_p = get_camera_para();
	WriteRecord(NVRAM_EF_CAMERA_PARA_LID, 1, (void*)camera_para_p, sizeof(nvram_camera_para_struct), &error);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_SAVE, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
}
#endif /* __FM_CAMERA_TUNNING__ */


/*****************************************************************************
* FUNCTION
*   mmi_fm_camera_highlight_hdlr
* DESCRIPTION
*   camera screen highlight handler
* PARAMETERS
*	 index		IN			selected index
* RETURNS
*   none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
static void mmi_fm_camera_highlight_hdlr(S32 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
	/*----------------------------------------------------------------*/		
   /* select on Sensor setting or ISP setting */
   switch(index)
   {
		case 0: /*  Sensor Setting */
			g_camtune_cntx.category_idx = index;
			SetLeftSoftkeyFunction(mmi_fm_camtune_entry_screen, KEY_EVENT_UP);		
			break;

		case 1: /* ISP Setting */
			g_camtune_cntx.category_idx = index;
			SetLeftSoftkeyFunction(mmi_fm_camtune_entry_screen, KEY_EVENT_UP);		
			break;

		case 2: /* Save To NVRAM */
			g_camtune_cntx.category_idx = index;
			SetLeftSoftkeyFunction(mmi_fm_camera_entry_save_confirm_screen, KEY_EVENT_UP);		
			break;

		case 3: /* Restore Default */
			g_camtune_cntx.category_idx = index;
			SetLeftSoftkeyFunction(mmi_fm_camera_entry_restore_confirm_screen, KEY_EVENT_UP);		
			break;

		default:
			ASSERT(0);
			
   }
  	  	
}
#endif /* __FM_CAMERA_TUNNING__ */



/*****************************************************************************
* FUNCTION
*   mmi_fm_camtune_set_value
* DESCRIPTION
*   set value to driver
* PARAMETERS
*	 void
* RETURNS
*   none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
static void mmi_fm_camtune_set_value(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/	
	S32 value;
	S32 index;
	S32 max, min;
	S8 buffer[128];
	S8 uni_buffer[256];
	S8	disp_buffer[256];

	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* stop preview. if need to restart preview when apply setting */
	if(g_camtune_cntx.group_data.items[index].is_need_restart)
	{
		mdi_camera_preview_stop();
	}


	/* flush inline edit buffer to app's buffer */
	wgui_update_inline_data();

	/* convert digit string to value */

	index = g_camtune_cntx.hilight_idx/2;		/* 2 line per item */
	min	= g_camtune_cntx.group_data.items[index].min_value;
	max	= g_camtune_cntx.group_data.items[index].max_value;
	
	/* if not ture false item, need covert from string to digit */
	if(!g_camtune_cntx.group_data.items[index].is_true_false)
	{
		value = coolsand_UI_atoi((UI_string_type)g_camtune_cntx.group_data.items[index].digit_buf);

		/* value changed, set to driver */
		if( (value > max) ||  (value < min) )
		{
		   sprintf(buffer,"%d-%d", min, max);
		   AnsiiToUnicodeString(uni_buffer, buffer);
			pfnUnicodeStrcpy(disp_buffer, (PS8)(L"Out of range\n\n"));
			pfnUnicodeStrcat(disp_buffer, g_camtune_cntx.group_data.items[index].name_buf);
			pfnUnicodeStrcat(disp_buffer, (PS8)(L"\n"));
			pfnUnicodeStrcat(disp_buffer, uni_buffer);
				
			DisplayPopup((PU8)disp_buffer, 0, 1, 1000, WARNING_TONE); /* popup time shorter than normal case */
			return;
		}

		g_camtune_cntx.group_data.items[index].value = value;
		
	}

	if(g_camtune_cntx.category_idx == FM_CAMERA_SENSOR)
	{
		set_sensor_item_info(g_camtune_cntx.group_idx, index, g_camtune_cntx.group_data.items[index].value);
	}
	else if(g_camtune_cntx.category_idx == FM_CAMERA_ISP)
	{
		set_isp_engmode_item_info(g_camtune_cntx.group_idx, index, g_camtune_cntx.group_data.items[index].value);
	}
	else
	{
		ASSERT(0);
	}


	/* re-start preview, if need to restart preview when apply setting */
	if(g_camtune_cntx.group_data.items[index].is_need_restart)
	{
		#ifdef __MMI_SUBMENU_USE_MM_IMAGE__

			mdi_camera_preview_start(	g_camtune_cntx.preview_layer_handle,
												0,
												MMI_title_height+MMI_title_y,
												UI_device_width,
												MMI_MENUITEM_HEIGHT*CAMTUNE_PREVIEW_HEIGHT_COUNT,
												GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1|GDI_LAYER_ENABLE_LAYER_2,
												GDI_LAYER_ENABLE_LAYER_0,
												0,  /* this value is use only CAMERA_MODULE_WITH_LCD */
												FALSE,
												&g_camtune_cntx.camera_setting_data);
		#else

			mdi_camera_preview_start(	g_camtune_cntx.preview_layer_handle,
												0,
												MMI_title_height+MMI_title_y,
												UI_device_width,
												MMI_MENUITEM_HEIGHT*CAMTUNE_PREVIEW_HEIGHT_COUNT,
												GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1,
												GDI_LAYER_ENABLE_LAYER_0,
												0,  /* this value is use only CAMERA_MODULE_WITH_LCD */
												FALSE,
												&g_camtune_cntx.camera_setting_data);
		#endif
	}
		

}
#endif /* __FM_CAMERA_TUNNING__ */


/*****************************************************************************
* FUNCTION
*   mmi_fm_camtune_draw_info_callback
* DESCRIPTION
*   draw info callback function. We use info region to display preview.
* PARAMETERS
*	 index 		IN 		hilight handler
*	 x1			IN			pos1 x
*	 y1			IN			pos1 y
*	 x2			IN			pos2 x
*	 y2			IN			pos2 y
* RETURNS
*   none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
static void mmi_fm_camtune_draw_info_callback(S32 index, S32 x1, S32 y1, S32 x2, S32 y2)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	gdi_layer_push_clip();
	gdi_layer_reset_clip();
	gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
	gdi_layer_pop_clip();
}
#endif /* __FM_CAMERA_TUNNING__ */


/*****************************************************************************
* FUNCTION
*   mmi_fm_camtune_highlight_hdlr
* DESCRIPTION
*   get group's info
* PARAMETERS
*	 index 		IN 		hilight handler
* RETURNS
*   none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
static void mmi_fm_camtune_highlight_hdlr(S32 index)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_camtune_cntx.hilight_idx = index;
	
	/* hook key from cat57 */
	set_left_softkey_function(mmi_fm_camtune_set_value, KEY_EVENT_UP);
	set_right_softkey_function(GoBackHistory, KEY_EVENT_UP);	
	SetKeyHandler(mmi_fm_camtune_change_group_fwd, KEY_STAR, KEY_EVENT_UP);
}
#endif /* __FM_CAMERA_TUNNING__ */


/*****************************************************************************
* FUNCTION
*   mmi_fm_camtune_get_category_info
* DESCRIPTION
*   get group's info
* PARAMETERS
*	 category_idx		IN			category index
* RETURNS
*   none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
static void mmi_fm_camtune_get_category_info(U16 category_idx)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* get group count of the category */
	switch(category_idx)
	{
		case FM_CAMERA_SENSOR:
			get_sensor_group_count(&g_camtune_cntx.group_count);
			break;

		case FM_CAMERA_ISP:
			get_isp_engmode_group_count(&g_camtune_cntx.group_count);
			break;

		default:
			ASSERT(0);
			break;
	}
}
#endif /* __FM_CAMERA_TUNNING__ */


/*****************************************************************************
* FUNCTION
*   mmi_fm_camtune_get_group_info
* DESCRIPTION
*   get group's info
* PARAMETERS
*	 category_idx		IN			category index, ISP or Sensor or whatelse
*	 group_idx			IN			group's index
* RETURNS
*   none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
static void mmi_fm_camtune_get_group_info(U16 category_idx, U16 group_idx)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/		
	S32	i;
	S8		buffer[256];

	ENG_sensor_info 					sensor_info;
	isp_engmode_item_info_struct	isp_info;
	
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(category_idx == FM_CAMERA_SENSOR)
	{
		get_sensor_group_info(group_idx, (kal_int8*)buffer, &g_camtune_cntx.group_data.item_count);
		AnsiiToUnicodeString((PS8)g_camtune_cntx.group_data.name_buf, (PS8)buffer);

		g_camtune_cntx.group_data.item_inx = 0;

		/* set name buf's terminate, avoid overflow */
		g_camtune_cntx.group_data.name_buf[CAMTUNE_GROUP_NAME_BUF_LEN-2] = '0';
		g_camtune_cntx.group_data.name_buf[CAMTUNE_GROUP_NAME_BUF_LEN-1] = '0';	

		/* get item info */
		for(i = 0; i < g_camtune_cntx.group_data.item_count; i++)
		{
			/* give it init value */
			sensor_info.is_true_false 		= FALSE;
			sensor_info.is_read_only 		= FALSE;			
			sensor_info.is_need_restart 	= FALSE;
			sensor_info.item_value			= 0;
			sensor_info.min					= 0;
			sensor_info.max 					= 0;

			memset((void*)&g_camtune_cntx.group_data.items[i], 0, sizeof(fm_camtune_item_struct));
				
			get_sensor_item_info(group_idx, i, &sensor_info);
			
			/* set item name */
			AnsiiToUnicodeString((PS8)g_camtune_cntx.group_data.items[i].name_buf, (PS8)sensor_info.item_name_ptr);
			g_camtune_cntx.group_data.items[i].name_buf[CAMTUNE_ITEM_NAME_BUF_LEN-2] = '0';
			g_camtune_cntx.group_data.items[i].name_buf[CAMTUNE_ITEM_NAME_BUF_LEN-1] = '0';		

			g_camtune_cntx.group_data.items[i].is_true_false 	= sensor_info.is_true_false;
			g_camtune_cntx.group_data.items[i].is_read_only	 	= sensor_info.is_read_only;	
			g_camtune_cntx.group_data.items[i].is_need_restart	= sensor_info.is_need_restart;
			g_camtune_cntx.group_data.items[i].value				= sensor_info.item_value;
			g_camtune_cntx.group_data.items[i].min_value			= sensor_info.min;
			g_camtune_cntx.group_data.items[i].max_value			= sensor_info.max;
			
			/* conver from value to digit */
			coolsand_UI_itoa(g_camtune_cntx.group_data.items[i].value, 
								(UI_string_type)g_camtune_cntx.group_data.items[i].digit_buf, 
								10);
		}

	}
	else if(category_idx == FM_CAMERA_ISP) 
	{
		get_isp_engmode_group_info(group_idx, (kal_uint8*)buffer, &g_camtune_cntx.group_data.item_count);
		AnsiiToUnicodeString((PS8)g_camtune_cntx.group_data.name_buf, (PS8)buffer);

		g_camtune_cntx.group_data.item_inx = 0;

		/* set name buf's terminate, avoid overflow */
		g_camtune_cntx.group_data.name_buf[CAMTUNE_GROUP_NAME_BUF_LEN-2] = '0';
		g_camtune_cntx.group_data.name_buf[CAMTUNE_GROUP_NAME_BUF_LEN-1] = '0';	

		/* get item info */
		for(i = 0; i < g_camtune_cntx.group_data.item_count; i++)
		{
			isp_info.is_true_false 		= FALSE;
			isp_info.is_read_only 		= FALSE;
			isp_info.is_need_restart 	= FALSE;
			isp_info.item_value			= 0;
			isp_info.min					= 0;
			isp_info.max					= 0;	

			memset((void*)&g_camtune_cntx.group_data.items[i], 0, sizeof(fm_camtune_item_struct));
			
			get_isp_engmode_item_info(group_idx, i, &isp_info);
			
			/* set item name */
			AnsiiToUnicodeString((PS8)g_camtune_cntx.group_data.items[i].name_buf, (PS8)isp_info.item_name_ptr);
			g_camtune_cntx.group_data.items[i].name_buf[CAMTUNE_ITEM_NAME_BUF_LEN-2] = '0';
			g_camtune_cntx.group_data.items[i].name_buf[CAMTUNE_ITEM_NAME_BUF_LEN-1] = '0';		

			g_camtune_cntx.group_data.items[i].is_true_false 	= isp_info.is_true_false;
			g_camtune_cntx.group_data.items[i].is_read_only	 	= isp_info.is_read_only;	
			g_camtune_cntx.group_data.items[i].is_need_restart	= isp_info.is_need_restart;
			g_camtune_cntx.group_data.items[i].value				= isp_info.item_value;
			g_camtune_cntx.group_data.items[i].min_value			= isp_info.min;
			g_camtune_cntx.group_data.items[i].max_value			= isp_info.max;
			
			/* conver from value to digit */
			coolsand_UI_itoa(g_camtune_cntx.group_data.items[i].value, 
								(UI_string_type)g_camtune_cntx.group_data.items[i].digit_buf, 
								10);
		}


	}
	else
	{
		ASSERT(0);
	}
}
#endif /* __FM_CAMERA_TUNNING__ */


/*****************************************************************************
* FUNCTION
*   mmi_fm_camtune_change_group_fwd
* DESCRIPTION
*   toggle group, change to next group
* PARAMETERS
*	 none
* RETURNS
*   none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
static void mmi_fm_camtune_change_group_fwd(void)
{
	if(g_camtune_cntx.group_count == 1)
		return;
	
	if(g_camtune_cntx.group_idx < g_camtune_cntx.group_count-1)
	{
		g_camtune_cntx.group_idx++;
	}
	else
	{
		/* loop back */
		g_camtune_cntx.group_idx = 0;
	}


	/* stop preview */
	if(g_camtune_cntx.is_previewing)
	{
		mdi_camera_preview_stop();
		g_camtune_cntx.is_previewing = FALSE;
	}
	
	/* close previous category57 */
	/* 
		we are will call category57 many times in one screen, so we need to close it 
		to reset some data before calling next cat57.
	*/
	CloseCategory57Screen();

	mmi_fm_camtune_get_group_info(g_camtune_cntx.category_idx, g_camtune_cntx.group_idx);
	mmi_fm_camtune_show_group();
}
#endif /* __FM_CAMERA_TUNNING__ */



/*****************************************************************************
* FUNCTION
*   mmi_fm_camtune_change_group_bwd
* DESCRIPTION
*   toggle group, change to prev group
* PARAMETERS
*	 none
* RETURNS
*   none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
static void mmi_fm_camtune_change_group_bwd(void)
{
	if(g_camtune_cntx.group_count == 1)
		return;

	if(g_camtune_cntx.group_idx > 0)
	{
		g_camtune_cntx.group_idx--;
	}
	else
	{
		g_camtune_cntx.group_idx = g_camtune_cntx.group_count-1;
	}

	/* stop preview */
	if(g_camtune_cntx.is_previewing)
	{
		mdi_camera_preview_stop();
		g_camtune_cntx.is_previewing = FALSE;
	}
	
	/* close previous category57 */
	/* 
		we are will call category57 many times in one screen, so we need to close it 
		to reset some data before calling next cat57.
	*/
	CloseCategory57Screen();

	mmi_fm_camtune_get_group_info(g_camtune_cntx.category_idx, g_camtune_cntx.group_idx);
	mmi_fm_camtune_show_group();
}
#endif /* __FM_CAMERA_TUNNING__ */




/*****************************************************************************
* FUNCTION
*   mmi_fm_camtune_toggle_fullscreen
* DESCRIPTION
*   toggle to fullscreen, clear OSD
* PARAMETERS
*	 none
* RETURNS
*   none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
static void mmi_fm_camtune_toggle_fullscreen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(!g_camtune_cntx.is_fullscreen)
	{
		/* Change to FULLSCREEN preview */
		
		g_camtune_cntx.is_fullscreen = TRUE;

		/* stop preview */
		mdi_camera_preview_stop();

		gdi_layer_set_blt_layer(g_camtune_cntx.preview_layer_handle,0, 0, 0);	

		/* start preview if it is not previewing */
		
		g_camtune_cntx.camera_setting_data.preview_width	= UI_device_width;
		g_camtune_cntx.camera_setting_data.preview_height	= UI_device_height;
		
		mdi_camera_preview_start(	g_camtune_cntx.preview_layer_handle,
											0,
											0,
											UI_device_width,
											UI_device_height,
											GDI_LAYER_ENABLE_LAYER_0,
											GDI_LAYER_ENABLE_LAYER_0,
											0,  /* this value is use only CAMERA_MODULE_WITH_LCD */
											FALSE,
											&g_camtune_cntx.camera_setting_data);

		ClearAllKeyHandler();
		SetKeyHandler(mmi_fm_camtune_toggle_fullscreen, KEY_POUND, KEY_EVENT_UP);
		
	}
	else
	{
		/* Change to NORMAL preview with UI */
		
		g_camtune_cntx.is_fullscreen = FALSE;

		mdi_camera_preview_stop();

		#ifdef __MMI_SUBMENU_USE_MM_IMAGE__
			/* use wallpaper for submenu */
			gdi_layer_set_blt_layer(g_camtune_cntx.preview_layer_handle, wgui_layer_1, wgui_base_layer, 0);	
		#else
			gdi_layer_set_blt_layer(g_camtune_cntx.preview_layer_handle, wgui_base_layer, 0, 0);		
		#endif 
		
		/* preview parametes */
		g_camtune_cntx.camera_setting_data.preview_width	= UI_device_width;
		g_camtune_cntx.camera_setting_data.preview_height	= MMI_MENUITEM_HEIGHT*CAMTUNE_PREVIEW_HEIGHT_COUNT;

		/* call this function to redraw all UI/keys */
		mmi_fm_camtune_show_group();

		/* blt UI */
		gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);

		#ifdef __MMI_SUBMENU_USE_MM_IMAGE__

			mdi_camera_preview_start(	g_camtune_cntx.preview_layer_handle,
												0,
												MMI_title_height+MMI_title_y,
												UI_device_width,
												MMI_MENUITEM_HEIGHT*CAMTUNE_PREVIEW_HEIGHT_COUNT,
												GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1|GDI_LAYER_ENABLE_LAYER_2,
												GDI_LAYER_ENABLE_LAYER_0,
												0,  /* this value is use only CAMERA_MODULE_WITH_LCD */
												FALSE,
												&g_camtune_cntx.camera_setting_data);
		#else

			mdi_camera_preview_start(	g_camtune_cntx.preview_layer_handle,
												0,
												MMI_title_height+MMI_title_y,
												UI_device_width,
												MMI_MENUITEM_HEIGHT*CAMTUNE_PREVIEW_HEIGHT_COUNT,
												GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1,
												GDI_LAYER_ENABLE_LAYER_0,
												0,  /* this value is use only CAMERA_MODULE_WITH_LCD */
												FALSE,
												&g_camtune_cntx.camera_setting_data);
		#endif

	}

}
#endif /* __FM_CAMERA_TUNNING__ */


/*****************************************************************************
* FUNCTION
*   mmi_fm_camtune_entry_screen
* DESCRIPTION
*   Camera tuning entry fucntion
* PARAMETERS
*	 none
* RETURNS
*   none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
static void mmi_fm_camtune_show_group(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/	
	S32 	i;
	S32	item_id;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	InitializeCategory57Screen();
	
	item_id = 0;


	for(i = 0 ; i < g_camtune_cntx.group_data.item_count; i++)
	{
		SetInlineItemCaption(&wgui_inline_items[item_id++], (PU8)g_camtune_cntx.group_data.items[i].name_buf);
		

		if(g_camtune_cntx.group_data.items[i].is_read_only)
		{
			/* read only, cant modify */
			SetInlineItemActivation(&wgui_inline_items[item_id], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);			
			SetInlineItemDisplayOnly(&wgui_inline_items[item_id++], (PU8)g_camtune_cntx.group_data.items[i].digit_buf);
		}
		else
		{
			if(g_camtune_cntx.group_data.items[i].is_true_false)
			{
				SetInlineItemActivation(&wgui_inline_items[item_id], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);			
				SetInlineItemSelect(	&wgui_inline_items[item_id++],
											2,
											(U8**)g_camtune_cntx.true_false_str,
											&g_camtune_cntx.group_data.items[i].value);
			}
			else
			{
				SetInlineItemActivation(&wgui_inline_items[item_id], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);				
				SetInlineItemTextEdit(	&wgui_inline_items[item_id], 
												(PU8)g_camtune_cntx.group_data.items[i].digit_buf, 
												CAMTUNE_ITEM_DIG_BUF_LEN, 
												INPUT_TYPE_NUMERIC);
				DisableInlineItemHighlight(&wgui_inline_items[item_id++]);
			}

		}
	}


	RegisterHighlightHandler(mmi_fm_camtune_highlight_hdlr);

	/* append a info box on top of cateogory 57 screen */
	Category57AppendTopInfoBox(CAMTUNE_PREVIEW_HEIGHT_COUNT, mmi_fm_camtune_draw_info_callback, NULL, TRUE);

	gdi_layer_lock_frame_buffer();

	ShowCategory57Screen(STR_GLOBAL_OK, 0,
								STR_GLOBAL_OK, IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
			  			  		g_camtune_cntx.group_data.item_count*2, 	/* num of items */ /* 2 line per item */
				  				NULL, 												/* icon list */
				  			  	wgui_inline_items,								/* inline data */ 
				  			  	g_camtune_cntx.hilight_idx, 					/* highlight idx */
				  			  	NULL);	

	/* change titile - since catgory57 do not support disply title from string buffer, we have do it our own */
	ChangeTitleString((PU8)g_camtune_cntx.group_data.name_buf);
	draw_title();

	#ifdef __MMI_SUBMENU_USE_MM_IMAGE__
		/* use wallpaper for submenu */
		gdi_layer_set_blt_layer(GDI_LAYER_MEDIA_HW_TRIGGER_LAYER, wgui_layer_1, wgui_base_layer, 0);	
	#else
		gdi_layer_set_blt_layer(GDI_LAYER_MEDIA_HW_TRIGGER_LAYER, wgui_base_layer, 0, 0);		
	#endif 

	/* set key hdlr */
	SetCategory57RightSoftkeyFunctions(GoBackHistory, GoBackHistory);
	set_left_softkey_function(mmi_fm_camtune_set_value,KEY_EVENT_UP);
	DisableCategory57ScreenDone();

	gdi_layer_unlock_frame_buffer();	
	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);

	SetKeyHandler(mmi_fm_camtune_change_group_fwd, KEY_STAR, KEY_EVENT_UP);
	SetKeyHandler(mmi_fm_camtune_change_group_fwd, KEY_VOL_UP, KEY_EVENT_UP);
	SetKeyHandler(mmi_fm_camtune_change_group_bwd, KEY_VOL_DOWN, KEY_EVENT_UP);	
	
	SetKeyHandler(mmi_fm_camtune_toggle_fullscreen, KEY_POUND, KEY_EVENT_UP);

	/* start preview if it is not previewing */
	if(!g_camtune_cntx.is_previewing)
	{
	
		/* preview parametes */
		g_camtune_cntx.camera_setting_data.zoom			= 10;	/* 1x */
		g_camtune_cntx.camera_setting_data.wb				= camera_wb_command_map[CAMERA_DEFAULT_SETTING_WB];	
		g_camtune_cntx.camera_setting_data.ev				= camera_ev_command_map[CAMERA_DEFAULT_SETTING_EV];
		g_camtune_cntx.camera_setting_data.banding 		= camera_banding_command_map[CAMERA_DEFAULT_SETTING_BANDING];
		g_camtune_cntx.camera_setting_data.night			= CAMERA_DEFAULT_SETTING_NIGHT;
		g_camtune_cntx.camera_setting_data.flash			= CAMERA_DEFAULT_SETTING_FLASH;
		
		/* default effect */
		g_camtune_cntx.camera_setting_data.effect			= camera_effect_command_map[CAMERA_DEFAULT_SETTING_EFFECT];
		g_camtune_cntx.camera_setting_data.brightness 	= 128;
		g_camtune_cntx.camera_setting_data.saturation 	= 128;
		g_camtune_cntx.camera_setting_data.contrast 		= 128;
		g_camtune_cntx.camera_setting_data.hue		 		= 0;
		
		g_camtune_cntx.camera_setting_data.preview_width	= UI_device_width;
		g_camtune_cntx.camera_setting_data.preview_height	= MMI_MENUITEM_HEIGHT*CAMTUNE_PREVIEW_HEIGHT_COUNT;
		
		/* capture parameters */
		g_camtune_cntx.camera_setting_data.image_qty			= camera_image_qty_command_map[CAMERA_DEFAULT_SETTING_IMAGE_QTY];
		g_camtune_cntx.camera_setting_data.image_width		= UI_device_width;
		g_camtune_cntx.camera_setting_data.image_height		= UI_device_height;	

		g_camtune_cntx.camera_setting_data.lcm					= MDI_CAMERA_PREVIEW_LCM_MAINLCD;
		g_camtune_cntx.camera_setting_data.preview_rotate	= CAMERA_PREVIEW_MAINLCD_ROTATE;
		g_camtune_cntx.camera_setting_data.lcm_rotate		= CAMERA_LCM_MAINLCD_ROTATE;


		#ifdef __MMI_SUBMENU_USE_MM_IMAGE__

			mdi_camera_preview_start(	g_camtune_cntx.preview_layer_handle,
												0,
												MMI_title_height+MMI_title_y,
												UI_device_width,
												MMI_MENUITEM_HEIGHT*CAMTUNE_PREVIEW_HEIGHT_COUNT,
												GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1|GDI_LAYER_ENABLE_LAYER_2,
												GDI_LAYER_ENABLE_LAYER_0,
												0,  /* this value is use only CAMERA_MODULE_WITH_LCD */
												FALSE,
												&g_camtune_cntx.camera_setting_data);
		#else

			mdi_camera_preview_start(	g_camtune_cntx.preview_layer_handle,
												0,
												MMI_title_height+MMI_title_y,
												UI_device_width,
												MMI_MENUITEM_HEIGHT*CAMTUNE_PREVIEW_HEIGHT_COUNT,
												GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1,
												GDI_LAYER_ENABLE_LAYER_0,
												0,  /* this value is use only CAMERA_MODULE_WITH_LCD */
												FALSE,
												&g_camtune_cntx.camera_setting_data);
		#endif

		g_camtune_cntx.is_previewing = TRUE;
	}

	
}
#endif /* __FM_CAMERA_TUNNING__ */



/*****************************************************************************
* FUNCTION
*   mmi_fm_camtune_entry_screen
* DESCRIPTION
*   Camera tuning entry fucntion
* PARAMETERS
*	 none
* RETURNS
*   none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
static void mmi_fm_camtune_entry_screen(void)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* this will force title status to redraw */
	entry_full_screen();
	
	EntryNewScreen(SCR_ID_FM_CAMERA, mmi_fm_camtune_exit_screen, NULL, NULL);

	mmi_fm_camtune_get_category_info(g_camtune_cntx.category_idx);

	g_camtune_cntx.is_previewing = FALSE;
	g_camtune_cntx.is_fullscreen = FALSE;
	gdi_layer_multi_layer_enable();

	/* entry sublcd */
#ifdef __MMI_SUBLCD__
	ForceSubLCDScreen(mmi_fm_camera_entry_sublcd_screen);
#endif 

	/* stop MMI sleep */
	TurnOnBacklight(0);

	/* force all playing keypad tone off */
	AudioStopReq(GetCurKeypadTone());

#ifdef __MMI_CLAMSHELL__
	/* register event hdlr */
	SetInterruptEventHandler(mmi_fm_camera_clam_close_event_hdlr,  NULL, MSG_ID_MMI_EQ_GPIO_DETECT_IND);
#endif /* __MMI_CLAMSHELL__ */

	/* stop LCD patten */
	StopLEDPatternBackGround();

	/* power on and init hardware module */
#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
        CSD_UI_Free_bmpbuf();
#endif  
	mdi_camera_power_on();
	
	/* create OSD layer */
	#ifdef APPMEM_SUPPORT
		/* with ASM */
		g_camtune_cntx.preview_layer_buf_ptr = AllocMediaMemory(FM_CAMERA_OSD_BUFFER_SIZE); 
	#else
		/* no ASM */
		g_camtune_cntx.preview_layer_buf_ptr = (PU8)camera_osd_layer_buffer;
	#endif /* APPMEM_SUPPORT */

	
	gdi_layer_create_using_outside_memory(	0,
														0,
														UI_device_width,
														UI_device_height,
														&g_camtune_cntx.preview_layer_handle,
														(PU8)g_camtune_cntx.preview_layer_buf_ptr, 
														(S32)FM_CAMERA_OSD_BUFFER_SIZE);

	gdi_layer_push_and_set_active(g_camtune_cntx.preview_layer_handle);

	#ifdef WIN32 		
		gdi_layer_clear(GDI_COLOR_RED);
	#else
		gdi_layer_clear(GDI_COLOR_BLACK);
	#endif

	gdi_layer_pop_and_restore_active();
	
	

	gdi_layer_push_and_set_active(wgui_base_layer);
	gdi_layer_clear(GDI_COLOR_TRANSPARENT);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);			
	gdi_layer_pop_and_restore_active();

	mmi_fm_camtune_get_group_info(g_camtune_cntx.category_idx, g_camtune_cntx.group_idx);
 	mmi_fm_camtune_show_group();
	
}
#endif /* __FM_CAMERA_TUNNING__ */


/*****************************************************************************
* FUNCTION
*	 mmi_fm_camtune_exit_screen()
* DESCRIPTION
*   exit text selection screen 
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__FM_CAMERA_TUNNING__)
static void mmi_fm_camtune_exit_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 		input_buffer_size;                
	history_t 	h;
	S16		n_history = 0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	h.scrnID = SCR_ID_FM_CAMERA;
	CloseCategory57Screen();
	h.entryFuncPtr = mmi_fm_camtune_entry_screen;
	pfnUnicodeStrcpy((S8*)h.inputBuffer,(S8*)&n_history);
	GetCategory57History (h.guiBuffer);
	input_buffer_size = (U16)GetCategory57DataSize();
	GetCategory57Data ((U8*)h.inputBuffer);
	AddNHistory(h, input_buffer_size);

	
	if(g_camtune_cntx.is_previewing)
	{
		mdi_camera_preview_stop();
		g_camtune_cntx.is_previewing = FALSE;
	}

	gdi_layer_flatten_previous_to_base();

	/****** free osd layer ******/
	#if defined(ISP_SUPPORT)
		gdi_layer_free(g_camtune_cntx.preview_layer_handle);
		g_camtune_cntx.preview_layer_handle = GDI_LAYER_EMPTY_HANDLE;

		#ifdef APPMEM_SUPPORT
			/* with ASM */
			FreeMediaMemory(g_camtune_cntx.preview_layer_buf_ptr); 
			g_camtune_cntx.preview_layer_buf_ptr = NULL;
		#endif /* APPMEM_SUPPORT */	

	#endif

	/* turn off camera */
	mdi_camera_power_off();

	gdi_layer_multi_layer_disable();	

	/* this will force title status to redraw */
	entry_full_screen();

	/* set blt base layer only */
	gdi_layer_set_blt_layer(GDI_LAYER_MAIN_BASE_LAYER_HANDLE, 0, 0, 0);

	/* resume LCD patten */
	StartLEDPatternBackGround();

	/* let MMI can sleep */
	TurnOffBacklight();

	/* re-enable keypad tone */
	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);

	/* un-hook clam close msg */
	SetInterruptEventHandler(NULL, NULL, MSG_ID_MMI_EQ_GPIO_DETECT_IND);

	/* exit sublcd */
#ifdef __MMI_SUBLCD__
	GoBackSubLCDHistory();
#endif
	
}
#endif /* __FM_CAMERA_TUNNING__ */






/**************************************************************
**	FUNCTION NAME		: HightlightFMNandFlashHdlr
**
**  	PURPOSE				: Highlight function for Nand Flash menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightFMNandFlashHdlr(void)
{
#ifdef NAND_SUPPORT
	change_left_softkey(STR_GLOBAL_OK,0);
	SetLeftSoftkeyFunction(EntryFMNandFlashMenu,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	redraw_left_softkey();
#else
	change_left_softkey(0,0);
	SetLeftSoftkeyFunction(NULL,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	redraw_left_softkey();

#endif
}


/**************************************************************
**	FUNCTION NAME		: EntryFMNandFlashMenu
**
**  	PURPOSE				: Entry function for Nand Flash menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMNandFlashMenu(void)
{
#ifdef NAND_SUPPORT

       if (NANDsanitycheck_Factory() == KAL_TRUE)
       {
          DisplayPopup((PU8)GetString(STR_ID_FM_AUTO_TEST_PASS), IMG_GLOBAL_OK, 0, 1000, 0);
       }
       else
       {
          DisplayPopup((PU8)GetString(STR_ID_FM_AUTO_TEST_FAILED), IMG_GLOBAL_ERROR, 0, 1000, 0);
       }
#endif
}


/**************************************************************
**	FUNCTION NAME		: ExitFMNandFlashMenu
**
**  	PURPOSE				: Exit function for Nand Flash menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitFMNandFlashMenu(void)
{

}

/*=============== Auto Testing =============== */

/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_LED
**
**  	PURPOSE				: For Auto Test in LED
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_LED(void)
{
   currentTest = FM_TEST_LED;
   switch(sequence_counter)
   {
   case 0:
      sequence_counter++;
      FM_SetGpioReq( GPIO_DEV_LED_STATUS_1, LED_LIGHT_LEVEL5 );
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_LED_R), NULL );
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, LED_DURATION, FM_AutoTest_LED);
      FM_Autotest_set_key_handlers();
      
      break;
   case 1:
      sequence_counter++;
      FM_SetGpioReq( GPIO_DEV_LED_STATUS_1, LED_LIGHT_LEVEL0 );
      FM_SetGpioReq( GPIO_DEV_LED_STATUS_2, LED_LIGHT_LEVEL5 );
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_LED_G), NULL );
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, LED_DURATION, FM_AutoTest_LED);
      FM_Autotest_set_key_handlers();
      break;
   case 2:
      sequence_counter++;
      FM_SetGpioReq( GPIO_DEV_LED_STATUS_2, LED_LIGHT_LEVEL0 );
      FM_SetGpioReq( GPIO_DEV_LED_STATUS_3, LED_LIGHT_LEVEL5 );
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_LED_B), NULL );
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, LED_DURATION, FM_AutoTest_LED);
      FM_Autotest_set_key_handlers();
      break;
   default:
      sequence_counter=0;
      FM_SetGpioReq( GPIO_DEV_LED_STATUS_3, LED_LIGHT_LEVEL0 );
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);

      {
//      	   FM_Autotest_test_done_string((S8*) EMFMAsciiDisplayBuf, Tests[FM_AUTOTEST_LED].name);
//          AnsiiToUnicodeString(EMFMUnicodeDisplayBuf, (S8*)EMFMAsciiDisplayBuf);
	   FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_LED].name);
  	   ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

	   FM_Autotest_set_key_handlers();
      }
      return;
      break;
   }

}


/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_CLAM
**
**  	PURPOSE				: For Auto Test in CLAM
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_CLAM(void)
{
   currentTest = FM_TEST_CLAM_DETECT;

#ifdef MMI_ON_HARDWARE_P
   switch(sequence_counter)
   {
   case 0:
      sequence_counter++;
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, CLAM_DURATION, FM_AutoTest_CLAM);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_CLAM), NULL );
      FM_Autotest_set_key_handlers();
      
      break;
   default:
      sequence_counter=0;
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);

      {
         FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_CLAM].name);
         ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

         FM_Autotest_set_key_handlers();
      }
   }
#else
   {
      FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_CLAM].name);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

      FM_Autotest_set_key_handlers();
   }
#endif
}


/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_SubLCM
**
**  	PURPOSE				: For Auto Test in SubLCM
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_SubLCM(void)
{
   currentTest = FM_TEST_SUBLCM;

   if (sequence_counter < LCD_CONTRAST_MAX_LEVEL)
   {
       UI_character_type text1[32];
       U8  u8text[32];

      sequence_counter++;
      PhnsetMainLCDContrast((U8)sequence_counter);
      PhnsetSubLCDContrast((U8)sequence_counter);

      sprintf((S8*)u8text, "%d/%d",  sequence_counter, MAX_CONTRAST_NUM );
      AnsiiToUnicodeString((S8*)text1, (S8*)u8text);

      ShowCategory205Screen((U16)STR_ID_FM_AUTO_TEST_PASS,0,(U16)STR_ID_FM_AUTO_TEST_FAIL,0, text1,IMG_ID_FM_CONTRAST, NULL);

      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, LCM_CONSTRAST_DURATION, FM_AutoTest_SubLCM);

      FM_Autotest_set_key_handlers();      
    }
    else
    {
      sequence_counter=0;
      PhnsetMainLCDContrast(7);
      PhnsetSubLCDContrast(7);
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);

       {
           FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_SUBLCM].name);
  	    ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

	   FM_Autotest_set_key_handlers();
       }
    }
}


/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_Version
**
**  	PURPOSE				: For Auto Test in Version
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_Version(void)
{
#if( defined(MMI_ON_HARDWARE_P) && !defined(WIN32) )
   version_struct ver_struct;
#endif

   currentTest = FM_TEST_SW_VERSION;

   switch(sequence_counter)
   {
   case 0:
#if( defined(MMI_ON_HARDWARE_P) && !defined(WIN32) )
      /* Get sturctured version number */
      INT_VersionNumbers(&ver_struct);

      /* MCU (SW Ver.) */
      memset(EMFMAsciiDisplayBuf, 0, sizeof(EMFMAsciiDisplayBuf) );
      memcpy(EMFMAsciiDisplayBuf, ver_struct.mcu_sw, MAX_TEST_STRING_BUF-2);
      AnsiiToUnicodeString((S8*)EMFMUnicodeDisplayBuf, (S8*)EMFMAsciiDisplayBuf);
   
#else
	/*For PC Simulatior*/
      AnsiiToUnicodeString((S8*)EMFMUnicodeDisplayBuf,(S8*)"CSD_SW.V0");
#endif
      sequence_counter++;
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, VERSION_DURATION, FM_AutoTest_Version);

      FM_Autotest_set_key_handlers();
      
      break;
   default:
      sequence_counter=0;
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);

      {
         FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_VERSION].name);
         ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );
	  if(!g_test_without_intervenor)
	  {
         	FM_Autotest_set_key_handlers();
	  }
	  else
	  {
	  	coolsand_UI_start_timer(1000,FM_Handle_Pass_Key_Press);
	  }
      }
   }
}


/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_Backlight
**
**  	PURPOSE				: For Auto Test in Backlight
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_Backlight(void)
{
   currentTest = FM_TEST_BACKLIGHT;

   switch(sequence_counter)
   {
   case 0:
      sequence_counter++;
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, BACKLIGHT_DURATION, FM_AutoTest_Backlight);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_BACKLIGHT), NULL );
      FM_Autotest_set_key_handlers();
      
      break;
   case 1:
      sequence_counter++;
      FM_SetGpioReq( GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL0 );
      FM_SetGpioReq( GPIO_DEV_LED_KEY, LED_LIGHT_LEVEL0 );
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, BACKLIGHT_DURATION, FM_AutoTest_Backlight);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_BACKLIGHT), NULL );
      FM_Autotest_set_key_handlers();
      
      break;
   case 2:
      sequence_counter++;
      FM_SetGpioReq( GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL5 );
	FM_SetGpioReq(GPIO_DEV_LED_KEY, LED_LIGHT_LEVEL5);
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, BACKLIGHT_DURATION, FM_AutoTest_Backlight);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_BACKLIGHT), NULL );
      FM_Autotest_set_key_handlers();
      
      break;
   case 3:
      sequence_counter++;
      FM_SetGpioReq( GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL0 );
      FM_SetGpioReq( GPIO_DEV_LED_KEY, LED_LIGHT_LEVEL0 );
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, BACKLIGHT_DURATION, FM_AutoTest_Backlight);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_BACKLIGHT), NULL );
      FM_Autotest_set_key_handlers();
      
      break;
   default:
      sequence_counter=0;
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);
      FM_SetGpioReq( GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL5 );
      FM_SetGpioReq( GPIO_DEV_LED_KEY, LED_LIGHT_LEVEL5 );

      {
          FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_BACKLIGHT].name);
  	   ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );
	   if(!g_test_without_intervenor)
	   {
	   	FM_Autotest_set_key_handlers();
	   }
	   else
	   {
	   	coolsand_UI_start_timer(1000,FM_Handle_Pass_Key_Press);
	   }
      }
      return;
      break;
   }


}


/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_LCD
**
**  	PURPOSE				: For Auto Test in LCD
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_LCD(void)
{
     currentTest = FM_TEST_LCD;

     sequence_counter = 0;
     FM_HandleColorSetChange();
}


/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_Keypad
**
**  	PURPOSE				: For Auto Test in Keypad
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_Keypad(void)
{
     currentTest = FM_TEST_KEYPAD;

     EntryFMKeypadTest();
}


/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_Receiver
**
**  	PURPOSE				: For Auto Test in Receiver
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_Receiver(void)
{
   currentTest = FM_TEST_RECEIVER;

#ifdef MMI_ON_HARDWARE_P
   FM_SendStopAudioReq( 0 );
   Media_Stop();
   ReceiverTestOn = TRUE;
   FM_SendSetAudioModeReq(AUD_MODE_NORMAL);
   /* play 1K tone */
   TONE_SetOutputVolume(255, 255);
   //TONE_Play((const L1SP_Tones*)test_tone);
   //CFW_EmodAudioTestStart(AUD_MODE_NORMAL);
   DM_SetAudioVolume(7);
   DM_PlayTone(24, 1, RECEIVER_DURATION,4);

   switch(sequence_counter)
   {
   case 0:
      sequence_counter++;
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, RECEIVER_DURATION, FM_AutoTest_Receiver);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_RECEIVER), NULL );
      FM_Autotest_set_key_handlers();

      break;
   default:
      sequence_counter=0;
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);

      ReceiverTestOn = FALSE;

      
      /* stop 1K tone */
      //TONE_Stop();
      //CFW_EmodAudioTestEnd();
      DM_StopTone();
      FM_SendSetAudioModeReq(AUD_MODE_NORMAL);

      {
         FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_RECEIVER].name);
         ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );
	  if(!g_test_without_intervenor)
	  {
         	FM_Autotest_set_key_handlers();
	  }
	  else
	  {
	  	coolsand_UI_start_timer(1000,FM_Handle_Pass_Key_Press);
	  }
      }
   }
#else
   {	   
      FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_RECEIVER].name);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

      FM_Autotest_set_key_handlers();
   }
#endif
}


/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_MIC
**
**  	PURPOSE				: For Auto Test in MIC
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_MIC(void)
{
   currentTest = FM_TEST_MIC;

#ifdef MMI_ON_HARDWARE_P
   FM_SendStopAudioReq( 0 );
   Media_Stop();
   /*Wangzl Add Start For 6319 Ver: TBM780  on 2007-8-27 15:37 */
   DM_StopTone();
   /*Wangzl Add End  For 6319 Ver: TBM780  on 2007-8-27 15:37 */
   EchoLoopTestOn = TRUE;
   FM_SendSetAudioModeReq(AUD_MODE_NORMAL);
   /* open loopback */
   //kal_sleep_task(kal_milli_secs_to_ticks(100));
   //custom_em_set_loopback(TRUE);
   //TMS_StartAudioLoopback();
    CFW_EmodAudioTestStart(AUD_MODE_NORMAL);

   switch(sequence_counter)
   {
   case 0:
      sequence_counter++;
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER,10000/* MIC_DURATION*/, FM_AutoTest_MIC);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_MIC), NULL );

      FM_Autotest_set_key_handlers();

      break;
   default:
      sequence_counter=0;
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);

      EchoLoopTestOn = FALSE;
      /* close loopback */
      //custom_em_set_loopback(FALSE);
     //TMS_StopAudioLoopback();
     CFW_EmodAudioTestEnd();
	FM_SendSetAudioModeReq(AUD_MODE_NORMAL);

      {
         FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_MIC].name);
         ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );
	   if(!g_test_without_intervenor)
	   {
        	 FM_Autotest_set_key_handlers();
	   }
	   else
	   {
	   	coolsand_UI_start_timer(1000,FM_Handle_Pass_Key_Press);
	   }
      }
   }
#else
   {
      FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_MIC].name);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

      FM_Autotest_set_key_handlers();
   }
#endif
}


/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_Speaker
**
**  	PURPOSE				: For Auto Test in Speaker
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_Speaker(void)
{
   currentTest = FM_TEST_SPEAKER;

#ifdef MMI_ON_HARDWARE_P
   FM_SendStopAudioReq( 0 );
   Media_Stop();
   LoudSpkTestOn = TRUE;
   FM_SendSetAudioModeReq(AUD_MODE_LOUDSPK);
   /* play 1K tone */
   TONE_SetOutputVolume(255, 255);
   //TONE_Play((const L1SP_Tones*)test_tone);
   //CFW_EmodAudioTestStart(AUD_MODE_LOUDSPK);
   #ifdef __FF_AUDIO_SET__
   GpioSpeakerModeStart();
   #endif
   DM_PlayTone(24, 1, SPEAKER_DURATION,4);
   switch(sequence_counter)
   {
   case 0:
      sequence_counter++;
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, SPEAKER_DURATION, FM_AutoTest_Speaker);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_SPEAKER), NULL );
      FM_Autotest_set_key_handlers();
      
      break;
   default:
      sequence_counter=0;
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);

      LoudSpkTestOn = FALSE;

      
      /* stop 1K tone */
      //TONE_Stop();
      //CFW_EmodAudioTestEnd();
      DM_StopTone();
      #ifdef __FF_AUDIO_SET__
      GpioSpeakerModeEnd();
      #endif
      FM_SendSetAudioModeReq(AUD_MODE_NORMAL);

      {
         FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_SPEAKER].name);
         ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );
	  if(!g_test_without_intervenor)
	  {
         	FM_Autotest_set_key_handlers();
	  }
	  else
	  {
	  	coolsand_UI_start_timer(1000,FM_Handle_Pass_Key_Press);
	  }
      }
   }
#else
   {
      FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_SPEAKER].name);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

      FM_Autotest_set_key_handlers();
   }
#endif
}

//Lisen 01262005
/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_Double_Speaker
**
**  	PURPOSE				: For Auto Test in Speaker
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_Double_Speaker(void)
{
   currentTest = FM_TEST_DOUBLE_SPEAKER;

#ifdef MMI_ON_HARDWARE_P
   FM_SendStopAudioReq( 0 );
   Media_Stop();
   LoudSpkTestOn = TRUE;
   FM_SendSetAudioModeReq(AUD_MODE_LOUDSPK);
   /* play 1K tone */
   TONE_SetOutputVolume(255, 255);
   
 //sequence_counter=0;
   switch(sequence_counter)
   {
   case 0:
  	sequence_counter++;
	//TONE_Play((const L1SP_Tones*)test_tone);
	//CFW_EmodAudioTestStart(AUD_MODE_LOUDSPK);
	 #ifdef __FF_AUDIO_SET__
   	 GpioSpeakerModeStart();
   	#endif
   	DM_PlayTone(24, 1, SPEAKER_DURATION,4);
	StartTimer(FM_AUTO_TEST_COMMNON_TIMER, SPEAKER_DURATION, FM_AutoTest_Double_Speaker);
	ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_LEFT_SPEAKER), NULL );
	
	FM_Autotest_set_key_handlers();
      
      break;
   case 1:
   	sequence_counter++;
	StartTimer(FM_AUTO_TEST_COMMNON_TIMER, 1000, FM_AutoTest_Double_Speaker);
	ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTO_TEST_STOP), NULL );
	FM_Autotest_set_key_handlers();
	//TONE_Stop();
	//CFW_EmodAudioTestEnd();
	DM_StopTone();
	break;
   case 2:
      sequence_counter++;
     // TONE_Play((const L1SP_Tones*)test_tone);
     //CFW_EmodAudioTestStart(AUD_MODE_LOUDSPK);
      #ifdef __FF_AUDIO_SET__
   	GpioSpeakerModeStart();
  	#endif
  	DM_PlayTone(1, 1, SPEAKER_DURATION,4);
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, SPEAKER_DURATION, FM_AutoTest_Double_Speaker);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_RIGHT_SPEAKER), NULL );
	FM_Autotest_set_key_handlers();
	//TONE_Stop();
	break;
   default:
      sequence_counter=0;
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);

      LoudSpkTestOn = FALSE;


      /* stop 1K tone */
     // TONE_Stop();
	//CFW_EmodAudioTestEnd();
	DM_StopTone();
      FM_SendSetAudioModeReq(AUD_MODE_NORMAL);
      {
         FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_DOUBLE_SPEAKER].name);
         ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );
	  if(!g_test_without_intervenor)
	  {
         	FM_Autotest_set_key_handlers();
	  }
	  else
	  {
	  	coolsand_UI_start_timer(1000,FM_Handle_Pass_Key_Press);
	  }
      }
   }
#else
   {
      FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_DOUBLE_SPEAKER].name);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

      FM_Autotest_set_key_handlers();
   }
#endif
}


/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_Headset
**
**  	PURPOSE				: For Auto Test in Headset
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
extern U8 isEarphonePlugged_1;
void FM_AutoTest_Headset(void)
{
   currentTest = FM_TEST_HEADSET;

#ifdef MMI_ON_HARDWARE_P
   FM_SendStopAudioReq( 0 );
   Media_Stop();
   DM_StopTone();
   
   if(!isEarphonePlugged_1)
   {
	ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
						STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
						IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_NO_EARPHONE), NULL );
	if(!g_test_without_intervenor)
	{
		FM_Autotest_set_key_handlers();
	}
	else
	{
		coolsand_UI_start_timer(1000,FM_Handle_Pass_Key_Press);
	}
   	return;
   }
   
   switch(sequence_counter)
   {
   case 0:
      sequence_counter++;

      HeadsetTestOn = TRUE;
      FM_SendSetAudioModeReq(AUD_MODE_HEADSET);
      kal_sleep_task(kal_milli_secs_to_ticks(200));
#if (HEADSET_MIC_TEST)
      /* open loopback */
      EchoLoopTestOn = TRUE;
      //custom_em_set_loopback(TRUE);
      CFW_EmodAudioTestStart(AUD_MODE_HEADSET);
#else
      DM_PlayTone(24, 1, HEADSET_DURATION,4);
#endif

      StartTimer(FM_AUTO_TEST_COMMNON_TIMER,
#if (HEADSET_MIC_TEST)
          10000,
#else
          HEADSET_DURATION,
#endif
          FM_AutoTest_Headset);

      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_HEADSET), NULL );
      FM_Autotest_set_key_handlers();
      
      break;
   default:
      sequence_counter=0;
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);

      HeadsetTestOn = FALSE;
      kal_sleep_task(kal_milli_secs_to_ticks(200));
#if (HEADSET_MIC_TEST)
      /* close loopback */
      //custom_em_set_loopback(FALSE);
	  //TMS_StopAudioLoopback();
      CFW_EmodAudioTestEnd();
      EchoLoopTestOn = FALSE;
#else
      DM_StopTone();
#endif
      /* set headset mode */
      FM_SendSetAudioModeReq(AUD_MODE_NORMAL);

      {
         FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_HEADSET].name);
         ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );
	  if(!g_test_without_intervenor)
	  {
         	FM_Autotest_set_key_handlers();
	  }
	  else
	  {
	  	coolsand_UI_start_timer(1000,FM_Handle_Pass_Key_Press);
	  }
      }
   }
#else
   {
      FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_HEADSET].name);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

      FM_Autotest_set_key_handlers();
   }
#endif
}


/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_VIB
**
**  	PURPOSE				: For Auto Test in VIB
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_VIB(void)
{
   currentTest = FM_TEST_VIB;

#ifdef MMI_ON_HARDWARE_P

   switch(sequence_counter)
   {
   case 0:
      sequence_counter++;
      FM_SetGpioReq( GPIO_DEV_VIBRATOR, VIBRATOR_ON );
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, VIB_DURATION, FM_AutoTest_VIB);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_VIB), NULL );
      FM_Autotest_set_key_handlers();

      break;
   case 1:
      sequence_counter++;
      FM_SetGpioReq( GPIO_DEV_VIBRATOR, VIBRATOR_OFF );
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, VIB_DURATION, FM_AutoTest_VIB);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)NULL, NULL );
      FM_Autotest_set_key_handlers();

      break;
   case 2:
      sequence_counter++;
      FM_SetGpioReq( GPIO_DEV_VIBRATOR, VIBRATOR_ON );
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, VIB_DURATION, FM_AutoTest_VIB);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_VIB), NULL );
      FM_Autotest_set_key_handlers();

      break;
   default:
      sequence_counter=0;
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);
      FM_SetGpioReq( GPIO_DEV_VIBRATOR, VIBRATOR_OFF );
     {
        FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_VIB].name);
        ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );
	if(!g_test_without_intervenor)
	{
       	 FM_Autotest_set_key_handlers();
	}
	else
	{
		coolsand_UI_start_timer(1000,FM_Handle_Pass_Key_Press);
	}
     }
   }
#else
   {
      FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_VIB].name);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

      FM_Autotest_set_key_handlers();
   }
#endif
}


/**************************************************************
**	FUNCTION NAME		: FM_AutoTestUpdateADCHdlr
**
**  	PURPOSE				: For ADC Auto Test 
**
**	INPUT PARAMETERS	: inMsg
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTestUpdateADCHdlr(void* inMsg)
{
 
#if 0
#ifdef MMI_ON_HARDWARE_P
   double vbat, temp, curr; 
   mmi_eq_adc_all_channel_ind_struct *adc_struct = (mmi_eq_adc_all_channel_ind_struct *) inMsg;

   vbat = ((double)adc_struct->vbat)/1000000.0;
   temp = ((double)adc_struct->bat_temp)/1000.0;
   curr = ((double)adc_struct->charge_current)/1000000.0;

   if (vbat < VBAT_LOWER_BOUNDARY || vbat > VBAT_UPPER_BOUNDARY)
   {
      sprintf((S8*)EMFMAsciiDisplayBuf,"%s%4.2f V\nValid Range:\n%4.2f V - %4.2f V\n\nsuggest: FAIL", 
      	                                     "vbat=", vbat,VBAT_LOWER_BOUNDARY, VBAT_UPPER_BOUNDARY);
   }
   else 
   if ((adc_mode == FM_ADC_BATTERY) && (temp < TEMP_LOWER_BOUNDARY || temp > TEMP_UPPER_BOUNDARY))
   {
      sprintf((S8*)EMFMAsciiDisplayBuf,"%s%4.2f C\nValid Range:\n%d C - %d C\n\nsuggest: FAIL", 
      	                                     "temp=",  temp,TEMP_LOWER_BOUNDARY, TEMP_UPPER_BOUNDARY);
   }
   else 
   if ((adc_mode == FM_ADC_CHARGER) && (curr < CURR__LOWER_BOUNDARY))
   {
      sprintf((S8*)EMFMAsciiDisplayBuf,"%s%4.2f A\nMust be at least %4.2f A\n\nsuggest: FAIL", 
      	                                     "current=",  curr, CURR__LOWER_BOUNDARY);
   }
   else
   {
      if (adc_mode == FM_ADC_BATTERY) 
         sprintf((S8*)EMFMAsciiDisplayBuf,"BATTERY OK!");
      else
         sprintf((S8*)EMFMAsciiDisplayBuf,"CHARGER OK!");      	
   }
        
   AnsiiToUnicodeString((PS8)EMFMUnicodeDisplayBuf, (S8*)EMFMAsciiDisplayBuf); 
   ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );
        
   FM_Autotest_set_key_handlers();
   
   SetProtocolEventHandler( NULL, MSG_ID_MMI_EQ_ADC_ALL_CHANNEL_IND );
   SetProtocolEventHandler(BatteryStatusRsp,PRT_BATTERY_STATUS_IND);
   FM_SendADCStopReq();
#endif  
#endif  
}



/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_Battery
**
**  	PURPOSE				: For Auto test in Battery 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_Battery(void)
{
   currentTest = FM_TEST_BATTERY;
   adc_mode = FM_ADC_BATTERY; 
   
#ifdef MMI_ON_HARDWARE_P
	CFW_EMOD_BATTERY_INFO batteryinfo;
	U16 vbat, temp; 
	CFW_EmodGetBatteryInfo(&batteryinfo);

	vbat = batteryinfo.nBatState.nBatteryVoltage;//((double)adc_struct->vbat)/1000000.0;
	temp = batteryinfo.nBatState.nBatteryTemperature;//((double)adc_struct->bat_temp)/1000.0;
	if (vbat < VBAT_LOWER_BOUNDARY || vbat > VBAT_UPPER_BOUNDARY)
	{
		sprintf((S8*)EMFMAsciiDisplayBuf,"vbat = %d mV\nValid Range:\n%d mV - %d mV\n\nsuggest: FAIL", 
										 vbat,VBAT_LOWER_BOUNDARY, VBAT_UPPER_BOUNDARY);
	}
	#if 0
	else 
	if ( (temp < TEMP_LOWER_BOUNDARY || temp > TEMP_UPPER_BOUNDARY))
	{
		sprintf((S8*)EMFMAsciiDisplayBuf,"temp=%d C\nValid Range:\n%d C - %d C\n\nsuggest: FAIL", 
										 temp,TEMP_LOWER_BOUNDARY, TEMP_UPPER_BOUNDARY);
	}
	#endif
	else
	{
		sprintf((S8*)EMFMAsciiDisplayBuf,"vbat = %d mV\nValid Range:\n%d mV - %d mV\n\nBATTERY OK!", 
									 vbat,VBAT_LOWER_BOUNDARY, VBAT_UPPER_BOUNDARY);
	}

	AnsiiToUnicodeString((PS8)EMFMUnicodeDisplayBuf, (S8*)EMFMAsciiDisplayBuf); 
	ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
							STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
							IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );
	if(!g_test_without_intervenor)
	{
		FM_Autotest_set_key_handlers();
	}
	else
	{
		coolsand_UI_start_timer(1000,FM_Handle_Pass_Key_Press);
	}
   #if 0
   if (sequence_counter == 0)
   {
      SetProtocolEventHandler( FM_AutoTestUpdateADCHdlr, MSG_ID_MMI_EQ_ADC_ALL_CHANNEL_IND );
      SetProtocolEventHandler( EMFMBatteryStatusMsgHandler, PRT_BATTERY_STATUS_IND );
      FM_SendADCStartReq();   
   }

   switch(sequence_counter)
   {
   case 0:
      sequence_counter++;
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, ADC_DURATION, FM_AutoTest_Battery);
      break;
   default:
      sequence_counter=0;
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);
   }
#endif
#else
   {
           FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_BATTERY].name);
  	    ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

	   FM_Autotest_set_key_handlers();
   }
#endif
}


/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_Melody
**
**  	PURPOSE				: For Auto test in Melody 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_Melody(void)
{
   currentTest = FM_TEST_MELODY;

#ifdef MMI_ON_HARDWARE_P

   switch(sequence_counter)
   {
   case 0:
      sequence_counter++;
      RingToneTestOn = TRUE;
      Media_SetOutputVolume(255, 255);
      /* stop MIDI */
      FM_SendStopAudioReq(0);
      /* play MIDI_1 */
      FM_SendPlayAudioReq(0);

      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, MELODY_DURATION, FM_AutoTest_Melody);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_MELODY), NULL );
      
      FM_Autotest_set_key_handlers();
      
      break;
   default:
      sequence_counter=0;
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);

      RingToneTestOn = FALSE;
      /* stop MIDI */
      FM_SendStopAudioReq(0);
      FM_SendSetAudioModeReq(AUD_MODE_NORMAL);

      {
         FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_MELODY].name);
         ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );
	  if(!g_test_without_intervenor)
	  {
         	FM_Autotest_set_key_handlers();
	  }
	  else
	  {
	 	 coolsand_UI_start_timer(1000,FM_Handle_Pass_Key_Press);
	  }
      }
   }
#else
   {
      FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_MELODY].name);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

      FM_Autotest_set_key_handlers();
   }
#endif
}


/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_Charger
**
**  	PURPOSE				: For Auto test in Charger 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_Charger(void)
{
   currentTest = FM_TEST_CHARGER;
   adc_mode = FM_ADC_CHARGER; 
   
#ifdef MMI_ON_HARDWARE_P
#if 0
   if (sequence_counter == 0)
   {
      SetProtocolEventHandler( FM_AutoTestUpdateADCHdlr, MSG_ID_MMI_EQ_ADC_ALL_CHANNEL_IND );
      SetProtocolEventHandler( EMFMBatteryStatusMsgHandler, PRT_BATTERY_STATUS_IND );
      FM_SendADCStartReq();   
   }

   switch(sequence_counter)
   {
   case 0:
      sequence_counter++;
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, ADC_DURATION, FM_AutoTest_Battery);
      break;
   default:
      sequence_counter=0;
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);
   }
  #else
  	CFW_EMOD_BATTERY_INFO batteryinfo;
	CFW_EmodGetBatteryInfo(&batteryinfo);
	U16 curr;
	curr = batteryinfo.nChargeInfo.nMpc;
	curr = 400;//wangrui add for fix charge fail, because we don't support mesure measure by hardware
	if (curr< CURR__LOWER_BOUNDARY&&(batteryinfo.nChargeInfo.nBcs))
	{
		sprintf((S8*)EMFMAsciiDisplayBuf,"current=%d.%d A\nMust be at least %d.%d A\n\nsuggest: FAIL", 
	                               					  curr/1000, (curr%1000),(CURR__LOWER_BOUNDARY%100),(CURR__LOWER_BOUNDARY/100));
	}
	else if(0==batteryinfo.nChargeInfo.nBcs) //No charging adapter is connected
	{
		sprintf((S8*)EMFMAsciiDisplayBuf,"CHARGER NOT CONNECTED!");
	}
	else if(4==batteryinfo.nChargeInfo.nBcs) //Charging error, charging is interrupted
	{
		sprintf((S8*)EMFMAsciiDisplayBuf,"CHARGER ERROR,CHARGING IS INTERRUPTED!");
	}
	else if(5==batteryinfo.nChargeInfo.nBcs) //False charging temperature, charging is interrupted while temperature is beyond allowed range
	{
		sprintf((S8*)EMFMAsciiDisplayBuf,"FALSE CHARGING TEMPERATURE,CHARGING IS INTERRUPTED!");
	}
	else //Charging adapter is connected
	{
		sprintf((S8*)EMFMAsciiDisplayBuf,"CHARGER OK!");  
	}
	AnsiiToUnicodeString((PS8)EMFMUnicodeDisplayBuf, (S8*)EMFMAsciiDisplayBuf); 
	ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
							STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
							IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );
	if(!g_test_without_intervenor)
	{
		FM_Autotest_set_key_handlers();
	}
	else
	{
		coolsand_UI_start_timer(1000,FM_Handle_Pass_Key_Press);
	}
 #endif
#else
   {
           FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_CHARGER].name);
  	    ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

	   FM_Autotest_set_key_handlers();
   }
#endif
}


/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_NAND
**
**  	PURPOSE				: For Auto test for NAND flash
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_NAND(void)
{
   currentTest = FM_TEST_NAND;
   
#ifdef MMI_ON_HARDWARE_P

   switch(sequence_counter)
   {
   case 0:
      sequence_counter++;
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, 3000, FM_AutoTest_NAND);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_NAND), NULL );
      FM_Autotest_set_key_handlers();
      
      break;
   case 1:
      sequence_counter++;
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, 2000, FM_AutoTest_NAND);
      EntryFMNandFlashMenu();
      FM_Autotest_set_key_handlers();
      break;
      
   default:
      sequence_counter=0;
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);
      FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_NAND].name);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

      FM_Autotest_set_key_handlers();
   }
#else
   {
           FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_NAND].name);
  	    ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

	   FM_Autotest_set_key_handlers();
   }
#endif
}

/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_UART
**
**  	PURPOSE				: For Auto test in UART 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_UART(void)
{
   currentTest = FM_TEST_UART;
   
#ifdef MMI_ON_HARDWARE_P

   switch(sequence_counter)
   {
   case 0:
      sequence_counter++;
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, 1000, FM_AutoTest_UART);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, (U8*)GetString(STR_ID_FM_AUTOTEST_UART), NULL );
      FM_Autotest_set_key_handlers();
      break;
   case 1:
      sequence_counter++;
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, 2000, FM_AutoTest_UART);
      EntryFMUARTMenu();
      FM_Autotest_set_key_handlers();
      break;

   default:
      sequence_counter=0;
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);
      FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_UART].name);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

       FM_Autotest_set_key_handlers();
   }
#else
   {
           FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_UART].name);
  	    ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

	   FM_Autotest_set_key_handlers();
   }
#endif
}
 
//Lisen 01262005

#ifdef __MMI_CAMERA__   //luanyy modify for cs621

static void EntryFMCameraTransientScreen(void)
{
	U8*	guiBuffer;

	NeedExitFunc = FALSE; // discard auto test history 
	EntryNewScreen(SCR_ID_FM_AUTO_TEST_START, ExitFMAutoTestStart, NULL, NULL);
	NeedExitFunc = TRUE;
       guiBuffer = GetCurrGuiBuffer(SCR_ID_FM_AUTO_TEST_START);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

       FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_CAMERA].name);
       ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );
	if(!g_test_without_intervenor)
	{
       	FM_Autotest_set_key_handlers();
	}
	else
	{
		coolsand_UI_start_timer(1000,FM_Handle_Pass_Key_Press);
	}
}

#endif
/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_CAMERA
**
**  	PURPOSE				: For Auto test in CAMERA 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_CAMERA(void)
{

#if defined(__FM_CAMERA_PREVIEW__)
  static int mynum=0;
   //currentTest = FM_TEST_CAMERA;//chenhe ,for m309,090312
 mmi_trace(1,"sequence_counter=%d, mynum=%d",sequence_counter,mynum);
   switch(sequence_counter)	/*wangrui add begin 20081208, fix bug10633*/
   {
   case 0:
   	mynum = 1;
      sequence_counter++;
      mmi_fm_camera_entry_preview_screen();
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, CAMERA_DURATION, FM_AutoTest_CAMERA);
      
      break;
   default:
   	mynum=0;
      sequence_counter=0;
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);
      MCI_CamPowerDown();
      EntryFMCameraTransientScreen();
   }
#else
   {

      FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_CAMERA].name);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

      FM_Autotest_set_key_handlers();
   }
#endif
}


/*Wangzl Add Start For auto test camera Ver: TBM780  on 2007-8-17 13:38 */
#define N_MAX_TEST_CAMERA 1000*2
#define CAMERA_PREVIEW_TIME 6000
#define CAMERA_TEST_RESULT_TIME	1000

U16 n_camera_test = 0;
BOOL b_camera_test_interrupt = FALSE;

void mmi_exit_camera_test_result_screen(void);
void mmi_interrupt_camera_test(void);
void EntryAutoTestCamera(void);
void mmi_exit_camera_test(void);

void mmi_entry_camera_test_result_screen(void)
{
	U8*	guiBuffer;
	S8 name_ascii[MAX_TEST_ITEM_NAME_LEN];
	U8 DisplayBuf[MAX_TEST_STRING_BUF];
	S8 UniDispBuff[MAX_TEST_STRING_BUF*2];
	EntryNewScreen(SCR_ID_FM_AUTO_TEST_START, NULL, mmi_entry_camera_test_result_screen, NULL);
	
       guiBuffer = GetCurrGuiBuffer(SCR_ID_FM_AUTO_TEST_START);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);


       UnicodeToAnsii((S8*)name_ascii, (S8*)Tests[FM_AUTOTEST_CAMERA].name);
       sprintf((S8*)DisplayBuf, "%s\n (%d/%d)\n", name_ascii,n_camera_test/2+1,N_MAX_TEST_CAMERA/2);
       AnsiiToUnicodeString((S8*)UniDispBuff, (S8*)DisplayBuf);
	UCS2Strcat((S8*)UniDispBuff, GetString(STR_GLOBAL_DONE));

	 if((n_camera_test < (N_MAX_TEST_CAMERA-1))&&(!b_camera_test_interrupt))
	 {
       	ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        				0, 0, 0, 0, ( PU8 ) UniDispBuff, NULL );
	 }
	 else
	 {
	 	ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
		                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
		                        IMG_GLOBAL_BACK, ( PU8 ) UniDispBuff, NULL );
		if(b_camera_test_interrupt)
		{
			SetLeftSoftkeyFunction(EntryAutoTestCamera,KEY_EVENT_UP);
		}
		else
		{	
			SetLeftSoftkeyFunction(mmi_exit_camera_test,KEY_EVENT_UP);
		}
		SetRightSoftkeyFunction(mmi_exit_camera_test,KEY_EVENT_UP);
		PowerAndEndKeyHandler();
	 }
	
}



void EntryAutoTestCamera(void)
{
	b_camera_test_interrupt = FALSE;
 //wug add used to cs_xxx 20070821 start
#if (defined(__FM_CAMERA_PREVIEW__)&& ((defined __SENSOR__)&&(NO_SENSOR != __SENSOR__)))
	if(n_camera_test < N_MAX_TEST_CAMERA)
	{
		ClearAllKeyHandler();
		ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
		
		if(n_camera_test%2)
		{
			mmi_entry_camera_test_result_screen();
			coolsand_UI_start_timer(CAMERA_TEST_RESULT_TIME,EntryAutoTestCamera);
		}
		else
		{
			mmi_fm_camera_entry_preview_screen();
			coolsand_UI_start_timer(CAMERA_PREVIEW_TIME,EntryAutoTestCamera);
		}
		n_camera_test ++;
		
		SetKeyHandler(mmi_interrupt_camera_test, KEY_END, KEY_EVENT_LONG_PRESS);
	}
 #endif
  //wug add used to cs_xxx 20070821 end
}


void mmi_exit_camera_test(void)
{
	coolsand_UI_cancel_timer(EntryAutoTestCamera);
	n_camera_test = 0;
	ExecuteCurrKeyHandler(KEY_END, KEY_EVENT_DOWN);
}

void mmi_interrupt_camera_test(void)
{
	coolsand_UI_cancel_timer(EntryAutoTestCamera);
	b_camera_test_interrupt = TRUE;
	mmi_entry_camera_test_result_screen();
}
/*Wangzl Add End  For  Ver: TBM780  on 2007-8-17 13:38 */

/**************************************************************
**	FUNCTION NAME		: FM_AutoTest_MemoryCard
**
**  	PURPOSE				: For Auto Test in MemoryCard
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_AutoTest_MemoryCard(void)
{
//wangrui delete the micro for fix bug 10484 	
//#if defined(__MSDC_MS__) || defined(__MSDC_SD_MMC__)|| defined(__MSDC_MSPRO__)
	UI_character_type text1[32];
	U8  u8text[120];
//#endif
   	currentTest = FM_TEST_MEMORYCARD;
	
	EntryNewScreen(SCR_ID_FM_MEMORY_CARD, NULL, FM_AutoTest_MemoryCard, NULL);
//wangrui delete the micro for fix bug 10484 	
//#if defined(__MSDC_MS__) || defined(__MSDC_SD_MMC__)|| defined(__MSDC_MSPRO__)
	if (sequence_counter < 6)
   	{
   		if (FM_Card_Status.present == TRUE)
	       {
	       	sequence_counter = 10;
			switch (FM_Card_Status.type)
	          	{ 
	             	case MS_CARD:
	                	AnsiiToUnicodeString((S8*)CardType, "MS_CARD OK.");
	                	break;
	             	case SD_CARD:
	                	AnsiiToUnicodeString((S8*)CardType, "SD_CARD OK.");
	                	break;
	             	case MMC_CARD: 
	                	AnsiiToUnicodeString((S8*)CardType, "MMC_CARD OK.");
	                	break;
	             	default: 
	                	AnsiiToUnicodeString((S8*)CardType, "Error!!!");
	          	}
			StartTimer(FM_AUTO_TEST_COMMNON_TIMER, 300, FM_AutoTest_MemoryCard);
	       }
		else
		{
			sequence_counter++;

			if (Card_Response_Sent == FALSE)
	   			FM_MemoryCardReq();
			
	      		sprintf((S8*)u8text, "\n Memory Card detect, Please wait....%d",  (MEMORYCARD_DURATION/1000)-sequence_counter);
	      		AnsiiToUnicodeString((S8*)text1, (S8*)u8text);
	      		ShowCategory205Screen((U16)STR_ID_FM_AUTO_TEST_PASS,
							0,
							(U16)STR_ID_FM_AUTO_TEST_FAIL,
							0, 
							text1,
							IMG_ID_FM_MEMORY_CARD, 
							NULL);
			FM_Autotest_set_key_handlers();

			StartTimer(FM_AUTO_TEST_COMMNON_TIMER, 1000, FM_AutoTest_MemoryCard);
			if((sequence_counter >= (MEMORYCARD_DURATION/1000) -1) && FM_Card_Status.present == FALSE)
			{  // No Card inserted.
	             		AnsiiToUnicodeString((S8*)CardType, "No Card.");
				StartTimer(FM_AUTO_TEST_COMMNON_TIMER, 300, FM_AutoTest_MemoryCard);
				sequence_counter++;
			}
		}
	}
	else
//#endif		
	{
	      	sequence_counter = 0;
	      	StopTimer(FM_AUTO_TEST_COMMNON_TIMER);
	       FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_MEMORYCARD].name);//[FM_AUTOTEST_MEMORYCARD-1] by chenhe, for M309 not camera,but can't compile without MMI_CAMERA feature
	  	ShowCategory7Screen( STR_ID_FM_AUTO_TEST_RESULT, IMG_ID_FM_AUTO_TEST_ROOT,
		                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
		                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

		FM_Autotest_set_key_handlers();
	}
}

/**************************************************************
**	FUNCTION NAME		: FM_Handle_RTC_Stop_Key_Press
**
**  	PURPOSE				: Helper function to process RTC stop key in Auto test
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
static void FM_Handle_RTC_Stop_Key_Press(void)
{
      sequence_counter=0;
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);
      WriteCurrentTimeToNVRAM();
      EntryFMMenuAutoTest();
}


/**************************************************************
**	FUNCTION NAME		: FM_ManualTest_RTC
**
**  	PURPOSE				: RTC test function in Auto test. 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_ManualTest_RTC(void)
{
    S8 uniBuf[240];
    U8 AsciiBuf[120];
    U16 msec = 1000;
    U16 sec=2;
    
#ifdef MMI_ON_HARDWARE_P
    MYTIME t;
    rtc_format_struct currtime;

   if (sequence_counter < sec)
   {
      sequence_counter++;
      StartTimer(FM_AUTO_TEST_COMMNON_TIMER, msec, FM_ManualTest_RTC);

      DTGetRTCTime(&t);

      currtime.rtc_sec = t.nSec; 
      currtime.rtc_min = t.nMin; 
      currtime.rtc_hour = t.nHour; 
      currtime.rtc_day = t.nDay; 
      currtime.rtc_mon = t.nMonth;  
      currtime.rtc_wday = 0;  
      currtime.rtc_year = (kal_uint8) (t.nYear-2000); 

      sprintf((S8*) AsciiBuf, 
           	"Last RTC:\n%d.%d.%d %02d:%02d:%02d\nCurrent RTC:\n%d.%d.%d %02d:%02d:%02d\n\npoweroff in %d sec", 
           	2000+myclocktime.rtc_year, myclocktime.rtc_mon, myclocktime.rtc_day, myclocktime.rtc_hour,myclocktime.rtc_min,myclocktime.rtc_sec,
           	2000+currtime.rtc_year, currtime.rtc_mon, currtime.rtc_day, currtime.rtc_hour,currtime.rtc_min, currtime.rtc_sec,
           	sec-sequence_counter);

      AnsiiToUnicodeString(uniBuf, (S8*)AsciiBuf);
      ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        0, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_STOP,
	                        IMG_GLOBAL_BACK, ( PU8 ) uniBuf, NULL );

      ClearAllKeyHandler();
      SetRightSoftkeyFunction(FM_Handle_RTC_Stop_Key_Press, KEY_EVENT_UP);
   }      
   else
   {
      sequence_counter=0;
      StopTimer(FM_AUTO_TEST_COMMNON_TIMER);
      
      WriteCurrentTimeToNVRAM();
      gFactoryAlarm = 1;
      FactorySetAlarm(3);
   }
#else
   {
           sprintf((S8*) AsciiBuf, 
            	"Last RTC:\n%d.%d.%d %02d:%02d:%02d\nCurrent RTC:\n%d.%d.%d %02d:%02d:%02d\n\npoweroff in %d sec", 
           	2004, 1, 1, 0, 0, 0, 
           	2004, 1, 1, 0, 0, 1);

           AnsiiToUnicodeString(uniBuf, (S8*)AsciiBuf);
           ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        0, IMG_GLOBAL_OK, 0,
	                        IMG_GLOBAL_BACK, ( PU8 ) uniBuf, NULL );

           ClearAllKeyHandler();
   }
#endif
}


/**************************************************************
**	FUNCTION NAME		: FM_InitAutoTest
**
**  	PURPOSE				: Initializer function for Auto test. 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_InitAutoTest(void)
{
       U8 i;
       ReadSettingsFromNVRAM();
       ReadTestResultFromNVRAM();
       currTestItem = 0;
       for (i=0; i<MAX_AUTO_TEST_ITEMS; i++)
          TestExecuted[i] = FALSE;
       InitFactoryModeSetting();
}

/**************************************************************
**	FUNCTION NAME		: mmi_fm_auto_test_delete_history_hdlr
**  PURPOSE				: a exit function for go back history or along the path to root history (idle screen)
**	INPUT PARAMETERS	: void* param
**	OUTPUT PARAMETERS: FALSE means no stop from go back history
**	RETURNS				: void
 
**************************************************************/
U8 mmi_fm_auto_test_delete_history_hdlr( void* param )
{
	gCurrentMode = NORMAL_MODE;
	mdi_audio_unblock();
	mdi_audio_resume_background_play();
	StartLEDPatternBackGround();
	mmi_profiles_restore_activated_profile();
	return FALSE;
}

/**************************************************************
**	FUNCTION NAME		: EntryFMMenuAutoTest
**
**  	PURPOSE				: Entry function for Auto test menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMMenuAutoTest(void)
{
	U8*	guiBuffer;
       S8   display_str[30];

	EntryNewScreen(SCR_ID_FM_AUTO_TEST_ROOT, ExitFMMenuAutoTest, NULL, NULL);

       autoTestMode = 1;
       	
       guiBuffer = GetCurrGuiBuffer(SCR_ID_FM_AUTO_TEST_ROOT);
	SetParentHandler(MENU_ID_FM_AUTO_TEST_ROOT);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	
       UCS2Strcpy((S8*)EMFMUnicodeDisplayBuf, GetString(STR_ID_FM_AUTO_TEST_ROOT));

       AnsiiToUnicodeString((S8*)display_str, "\n\n\n\nEndkey: ");

       UCS2Strcat((S8*)EMFMUnicodeDisplayBuf, (S8*)display_str);

       UCS2Strcat((S8*)EMFMUnicodeDisplayBuf, GetString(STR_ID_FM_AUTO_TEST_EXIT));

	ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                     STR_ID_FM_AUTO_TEST_START, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_REPORT,
	                     IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, guiBuffer );

	SetLeftSoftkeyFunction(EntryFMAutoTestStart, KEY_EVENT_UP);
	SetRightSoftkeyFunction(EntryFMAutoTestReport, KEY_EVENT_UP);

	FM_InitAutoTest();
	SetDelScrnIDCallbackHandler( SCR_ID_FM_AUTO_TEST_ROOT, (HistoryDelCBPtr)mmi_fm_auto_test_delete_history_hdlr );
	if(g_test_without_intervenor)
	{
		EntryFMAutoTestStart();
	}
}
/**************************************************************
**	FUNCTION NAME		: EntryFMMenuAutoTest
**
**  	PURPOSE				: Entry function for Auto test menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/

void EntryAutoTest(void)
{
	g_test_without_intervenor = TRUE;
	TurnOnBacklight(0);
	EntryFMMenuAutoTest();
	
}
/**************************************************************
**	FUNCTION NAME		: ExitAutoTest
**
**  	PURPOSE				: Exit function for Auto test menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/

void ExitAutoTest(void)
{
	if(g_test_without_intervenor)
		{
			g_test_without_intervenor = FALSE;
			TurnOffBacklight();
		}
	
}
/**************************************************************
**	FUNCTION NAME		: ExitFMMenuAutoTest
**
**  	PURPOSE				: Exit function for Auto test menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitFMMenuAutoTest(void)
{
	history_t currHistory;

	FM_RestoreAudio();

	currHistory.scrnID = SCR_ID_FM_AUTO_TEST_ROOT;
	currHistory.entryFuncPtr = EntryFMMenuAutoTest;
	pfnUnicodeStrcpy((S8*)currHistory.inputBuffer, (S8*)L"");
	GetCategoryHistory (currHistory.guiBuffer);
	AddHistory (currHistory);
}

/**************************************************************
**	FUNCTION NAME		: AutoTestSceess
**
**  	PURPOSE				: Sucess of auto test
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
BOOL FM_SetValidMarker(UINT8  nStatus);
BOOL FM_SetProductionStatus(UINT8 nFP,BOOL nResult);
void AutoTestSuccess(void)
{
 
//FM_SetProductionStatus(4, TRUE);
	FM_ManualTest_RTC();
}

/**************************************************************
**	FUNCTION NAME		: AutoTestlFail
**
**  	PURPOSE				: fail of auto test
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void AutoTestlFail(void)
{
	//FM_SetProductionStatus(4, TRUE);
	FM_ManualTest_RTC();
}
/**************************************************************
**	FUNCTION NAME		: EntryFMAutoTestReport
**
**  	PURPOSE				: Entry function for Auto test report 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMAutoTestReport(void)
{
	U8*	guiBuffer;
       U16  i, passed, failed, untested;
       U8    fail_items[MAX_AUTO_TEST_ITEMS];
       U8    untested_items[MAX_AUTO_TEST_ITEMS];

	NeedExitFunc = FALSE;
	EntryNewScreen(SCR_ID_FM_AUTO_TEST_REPORT, NULL, EntryFMAutoTestReport, NULL);
	NeedExitFunc = TRUE;
       guiBuffer = GetCurrGuiBuffer(SCR_ID_FM_AUTO_TEST_REPORT);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

       for (i=0, passed=0, failed=0, untested=0; 
              i<gAutoTestListSize; 
              i++)
       {
	   if (nvramTestResultArray.result[i] == FM_TEST_UNTESTED)
	   {
	   	untested_items[untested] = (U8) i;
	   	untested++;
	   }
	   else if (nvramTestResultArray.result[i] == FM_TEST_FAILED)
	   {
	   	fail_items[failed] = (U8) i;
	   	failed++;
	   }
	   else if (nvramTestResultArray.result[i] >= FM_TEST_PASSED)
	   	passed++;
       }

       if (untested > 0)
       {
          S8 uniBuf[MAX_TEST_STRING_BUF*2];
          
          memset(EMFMAsciiDisplayBuf, 0, sizeof(EMFMAsciiDisplayBuf));
          if (untested > 1)
            sprintf((S8*)EMFMAsciiDisplayBuf, "\n\n%d Untested items:\n", untested);
          else
            sprintf((S8*)EMFMAsciiDisplayBuf, "\n\n1 Untested item:\n" );
          
          for (i=0; i<untested; i++)
          {
             S8 str[3];

             if (i == untested-1)
                sprintf(str, "%d", 1+untested_items[i]);
             else
                sprintf(str, "%d-", 1+untested_items[i]);             	

             strcat((S8*) EMFMAsciiDisplayBuf, (const S8*) str);
          }
          
          AnsiiToUnicodeString((S8*)uniBuf, (S8*)EMFMAsciiDisplayBuf);

          UCS2Strcpy((S8*)EMFMUnicodeDisplayBuf, GetString(STR_ID_FM_AUTO_TEST_UNFINISHED));
          
          UCS2Strcat((S8*)EMFMUnicodeDisplayBuf, (S8*)uniBuf);

	   ShowCategory7Screen( STR_ID_FM_AUTO_TEST_REPORT, IMG_ID_FM_AUTO_TEST_ROOT,
	                     0, IMG_GLOBAL_OK, STR_GLOBAL_BACK,
	                     IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, guiBuffer );

	   SetLeftSoftkeyFunction(NULL, KEY_EVENT_UP);
	   SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
       }
       else if (failed > 0)
       {
          S8 uniBuf[MAX_TEST_STRING_BUF*2];

          memset(EMFMAsciiDisplayBuf, 0, sizeof(EMFMAsciiDisplayBuf));
          if (failed > 1)
            sprintf((S8*)EMFMAsciiDisplayBuf, "\n%d Failed items:\n", failed);
          else
            sprintf((S8*)EMFMAsciiDisplayBuf, "\n1 Failed item:\n");          	

          for (i=0; i<failed; i++)
          {
             S8 str[3];

             if (i == failed-1)
                sprintf(str, "%d", 1+fail_items[i]);
             else
                sprintf(str, "%d-", 1+fail_items[i]);             	

             strcat((S8*) EMFMAsciiDisplayBuf, (const S8*) str);
          }
          
          AnsiiToUnicodeString((S8*)uniBuf, (S8*)EMFMAsciiDisplayBuf);

          UCS2Strcpy((S8*)EMFMUnicodeDisplayBuf, GetString(STR_ID_FM_AUTO_TEST_FAILED));
          
          UCS2Strcat((S8*)EMFMUnicodeDisplayBuf, (S8*)uniBuf);

	   ShowCategory7Screen( STR_ID_FM_AUTO_TEST_REPORT, IMG_ID_FM_AUTO_TEST_ROOT,
	                     0, IMG_GLOBAL_OK, STR_GLOBAL_BACK,
	                     IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, guiBuffer );

	   SetLeftSoftkeyFunction(NULL, KEY_EVENT_UP);
	   SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
       }
       else
       {
		if(!g_test_without_intervenor)
		{
			ShowCategory7Screen( STR_ID_FM_AUTO_TEST_REPORT, IMG_ID_FM_AUTO_TEST_ROOT,
								STR_ID_FM_AUTO_TEST_RTC, IMG_GLOBAL_OK, STR_GLOBAL_BACK,
								IMG_GLOBAL_BACK, ( PU8 ) GetString(STR_ID_FM_AUTO_TEST_ALLPASSED), guiBuffer );

			SetLeftSoftkeyFunction(FM_ManualTest_RTC, KEY_EVENT_UP);
			SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
		}
		else
		{
			//修改此处
			#if 1
			ShowCategory7Screen(STR_ID_FM_AUTO_TEST_REPORT, IMG_ID_FM_AUTO_TEST_ROOT,STR_ID_FM_AUTO_TEST_PASS,IMG_GLOBAL_OK,STR_ID_FM_AUTO_TEST_FAIL,IMG_GLOBAL_BACK,
								( PU8 ) GetString(STR_ID_FM_AUTO_TEST_ALLPASSED), guiBuffer);
			ClearAllKeyHandler();
			SetLeftSoftkeyFunction(AutoTestSuccess, KEY_EVENT_UP);
			SetRightSoftkeyFunction(AutoTestlFail, KEY_EVENT_UP);
			#else
			coolsand_UI_start_timer(1000,FM_ManualTest_RTC);
			#endif
		}
       }

}


/**************************************************************
**	FUNCTION NAME		: EntryFMAutoTestSummary
**
**  	PURPOSE				: Entry function for Auto test summary 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMAutoTestSummary(void)
{
	EntryNewScreen(SCR_ID_FM_AUTO_TEST_SUMMARY, NULL, EntryFMAutoTestSummary, NULL);
}


/**************************************************************
**	FUNCTION NAME		: EntryFMAutoTestResult
**
**  	PURPOSE				: Entry function for Auto test result 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMAutoTestResult(void)
{
      EntryNewScreen(SCR_ID_FM_AUTO_TEST_RESULT, NULL, EntryFMAutoTestResult, NULL);
}


/**************************************************************
**	FUNCTION NAME		: EntryFMAutoTestStart
**
**  	PURPOSE				: Entry function for Auto test start screen 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryFMAutoTestStart(void)
{
       U16 testitem; 
	U8*	guiBuffer;

	NeedExitFunc = FALSE; // discard auto test history 
	EntryNewScreen(SCR_ID_FM_AUTO_TEST_START, ExitFMAutoTestStart, NULL, NULL);
	entry_full_screen();
	NeedExitFunc = TRUE;
       guiBuffer = GetCurrGuiBuffer(SCR_ID_FM_AUTO_TEST_START);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

       // currTestItem++;
	//gAutoTestListSize = 14;//tmp chenhe
       if (currTestItem == 0) // Testing just began, clear all tet results
       {
          U16 i;
	   for (i = 0; i < gAutoTestListSize; i++)
             nvramTestResultArray.result[i] = FM_TEST_UNTESTED; 

	   ReadTimeFromNVRAM();
       }
       
       if (currTestItem <= gAutoTestListSize-1) 
       {
          /*Call test item according to the selected order */
          testitem = nvramTestItemArray.priority[currTestItem]<MAX_AUTO_TEST_ITEMS?nvramTestItemArray.priority[currTestItem]:(MAX_AUTO_TEST_ITEMS-1); //chenhe for greenstone as not define __MMI_TOUCH_SCREEN__ 

          if (TestExecuted[currTestItem] == FALSE) // prevent reentry of the same tests on interrupts
          {
             /* Execute the test */
             (Tests[testitem].func)();
             
             TestExecuted[currTestItem] = TRUE; 
          }
          else if (currentTest == FM_TEST_NAND || currentTest == FM_TEST_UART)
          {
             /* Execute the test */
             (Tests[testitem].func)();
          }
          else
          {
             FM_Autotest_Stop_Test();
             sprintf((S8*) EMFMAsciiDisplayBuf, "Interrupted!!\nPlease re-test\n\nSendkey: ReTest\nEndkey: Exit");
             AnsiiToUnicodeString(EMFMUnicodeDisplayBuf, (S8*)EMFMAsciiDisplayBuf);
             ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

             FM_Autotest_set_key_handlers();
          }
       }
       else
       {
  	   ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
	                        STR_ID_FM_AUTO_TEST_REPORT, IMG_GLOBAL_OK, STR_GLOBAL_DONE,
	                        IMG_GLOBAL_BACK, ( PU8 ) GetString(STR_ID_FM_AUTO_TEST_ALLDONE), guiBuffer );

	   DeleteUptoScrID(SCR_ID_FM_AUTO_TEST_ROOT);

	   SetLeftSoftkeyFunction(EntryFMAutoTestReport, KEY_EVENT_UP);
	   SetRightSoftkeyFunction(EntryFMMenuAutoTest, KEY_EVENT_UP);
	   if(g_test_without_intervenor)
	   {
	   	coolsand_UI_start_timer(1000,EntryFMAutoTestReport);
	   }
       }
}


/**************************************************************
**	FUNCTION NAME		: ExitFMAutoTestStart
**
**  	PURPOSE				: Exit function for Auto test start screen 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitFMAutoTestStart(void)
{
	if (NeedExitFunc)
	{
		history_t currHistory;

		currHistory.scrnID = SCR_ID_FM_AUTO_TEST_START;
		currHistory.entryFuncPtr = EntryFMAutoTestStart;
		pfnUnicodeStrcpy((S8*)currHistory.inputBuffer, (S8*)L"");
		GetCategoryHistory (currHistory.guiBuffer);
		AddHistory (currHistory);
		
		StopTimer(FM_AUTO_TEST_COMMNON_TIMER);
	}
	/*jinzh Add Start For 工厂模式验完MIC后直接退出放音有噪音问题 Ver: tbm780  on 2007-11-15 10:16 */
	FM_BACK_TO_NORMAL_STATE();
	/*jinzh Add End  For 工厂模式验完MIC后直接退出放音有噪音问题 Ver: tbm780  on 2007-11-15 10:16 */
}

/*=============== MTBF =============== */

/**************************************************************
**	FUNCTION NAME		: FM_MTBF
**
**  	PURPOSE				: MTBF for LCD, Keypad LED, Speaker, Vibrator, Receiver
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FM_MTBF(void)
{
#ifdef MMI_ON_HARDWARE_P
   U32 Count_ms = FM_MTBF_PERIOD*MTBF_counter;
   
   // LCD
   if ((Count_ms%FM_MTBF_LCD_CYCLE ) == 0)
   {
      TurnOnBacklight(0);
      coolsand_UI_lock_double_buffer();

      switch(MTBF_state_LCD)
      {
       case FM_MTBF_LCD_R:
          FM_HandleInitialColor(255,0,0);
          break;
       case FM_MTBF_LCD_G:
          FM_HandleInitialColor(0,255,0);
          break;
       case FM_MTBF_LCD_B:
          FM_HandleInitialColor(0,0,255);
          break;
       case FM_MTBF_LCD_W:
          FM_HandleInitialColor(255,255,255);
          break;
       default:
          ASSERT(0);
      }

      coolsand_UI_unlock_double_buffer();
      coolsand_UI_BLT_double_buffer(0,0,MAIN_LCD_device_width-1,MAIN_LCD_device_height-1);

      if (MTBF_state_LCD == FM_MTBF_LCD_END)
      	  MTBF_state_LCD = FM_MTBF_LCD_START;
      else
      	  MTBF_state_LCD++;

   }

   // LED
   if ((Count_ms%FM_MTBF_LED_CYCLE ) == 0)
   {
      FM_SetGpioReq( GPIO_DEV_LED_KEY, LED_LIGHT_LEVEL0 );
      FM_SetGpioReq( GPIO_DEV_LED_STATUS_1, LED_LIGHT_LEVEL0 );
      FM_SetGpioReq( GPIO_DEV_LED_STATUS_2, LED_LIGHT_LEVEL0 );
      FM_SetGpioReq( GPIO_DEV_LED_STATUS_3, LED_LIGHT_LEVEL0 );
      
      switch(MTBF_state_LED)
      {
       case FM_MTBF_LED_KEYPAD:
          FM_SetGpioReq( GPIO_DEV_LED_KEY, LED_LIGHT_LEVEL5 );
          break;
       case FM_MTBF_LED_R:
          FM_SetGpioReq( GPIO_DEV_LED_STATUS_1, LED_LIGHT_LEVEL5 );
          break;
       case FM_MTBF_LED_G:
          FM_SetGpioReq( GPIO_DEV_LED_STATUS_2, LED_LIGHT_LEVEL5 );
          break;
       case FM_MTBF_LED_B:
          FM_SetGpioReq( GPIO_DEV_LED_STATUS_3, LED_LIGHT_LEVEL5 );
          break;
       default:
          ASSERT(0);
      }

      if (MTBF_state_LED == FM_MTBF_LED_END)
      	  MTBF_state_LED = FM_MTBF_LED_START;
      else
      	  MTBF_state_LED++;
   }


   // VIB
   if ((Count_ms%FM_MTBF_VIB_CYCLE ) == 0)
   {
      if (MTBF_state_VIB == FM_MTBF_VIB_ON)
         FM_SetGpioReq( GPIO_DEV_VIBRATOR, VIBRATOR_ON );
      else
         FM_SetGpioReq( GPIO_DEV_VIBRATOR, VIBRATOR_OFF );

      if (MTBF_state_VIB == FM_MTBF_VIB_END)
      	  MTBF_state_VIB = FM_MTBF_VIB_START;
      else
      	  MTBF_state_VIB++;
   }

   // Speaker
   if ((Count_ms%FM_MTBF_SPK_CYCLE ) == 0  && AUD_TEST_ON == FM_MTBF_AUD_SPK)
   {
      // Louk Speaker Mode
      TONE_Stop();
      kal_sleep_task(20);
      FM_SendSetAudioModeReq(AUD_MODE_LOUDSPK); 
      //TONE_SetOutputDevice(L1SP_LOUD_SPEAKER);
      TONE_SetOutputVolume(255, 255);
      TONE_Play((const L1SP_Tones*) (SPK_test_tone));

      if (AUD_TEST_ON == FM_MTBF_AUD_END)
      	  AUD_TEST_ON = FM_MTBF_AUD_START;
      else
      	  AUD_TEST_ON++; 
   }
   else // Receiver
   if ((Count_ms%FM_MTBF_REV_CYCLE ) == 0  && AUD_TEST_ON == FM_MTBF_AUD_REV)
   {
      // Normal Mode
      TONE_Stop();
      kal_sleep_task(20);
      FM_SendSetAudioModeReq(AUD_MODE_NORMAL);
      //TONE_SetOutputDevice(L1SP_SPEAKER1);
      TONE_SetOutputVolume(255, 255);
      TONE_Play((const L1SP_Tones*) (REV_test_tone));

      if (AUD_TEST_ON == FM_MTBF_AUD_END)
      	  AUD_TEST_ON = FM_MTBF_AUD_START;
      else
      	  AUD_TEST_ON++; 
   }

   /***** Prepare for the next step *****/
   MTBF_counter++;
   StartTimer(FM_AUTO_TEST_COMMNON_TIMER, FM_MTBF_PERIOD, FM_MTBF);

#endif

}


#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h"

typedef enum
{
	INVALID_TEST=0,
	PARALLEL_TEST_1,		/*	\\	*/
	PARALLEL_TEST_2			/*	//	*/
}parallel_test_type;

/* Previous point in pen test program */
static mmi_pen_point_struct gFMPenPreviousPoint;

/* Capture pen events by test drawing program */
static BOOL gFMPenTestCapture = MMI_FALSE;

/* Line color */
static color_t gFMPenLineColor = {0,0,0,100};


static S16 gFMPenParallel_test_type = PARALLEL_TEST_1;
static S16 gFMPenParallel_first_x1;
static S16 gFMPenParallel_first_y1;
static S16 gFMPenParallel_first_x2;
static S16 gFMPenParallel_first_y2;
static S16 gFMPenParallel_second_x1;
static S16 gFMPenParallel_second_y1;
static S16 gFMPenParallel_second_x2;
static S16 gFMPenParallel_second_y2;
static S16 gFMPenParallel_first_point_x;	/*Starting point's x*/
static S16 gFMPenParallel_first_point_y;	/*Starting point's y*/
static S16 gFMPenParallel_second_point_x;	/*End point's x*/
static S16 gFMPenParallel_second_point_y;	/*End point's y*/

BOOL gFM_pen_test_start_from_top = FALSE;

static const S16 gFMPenPointOffset = 10;	/*Offset around start and end point*/

U8 addtohistory = FALSE;		/*Parallel Test screen need's to be added in History*/


/*****************************************************************************
* FUNCTION
*  HighlightFMParallelTestHdlr
* DESCRIPTION
*	This function is highlight handler for Parallel line Test
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightFMParallelTestHdlr(void)
{
	SetLeftSoftkeyFunction(EntryFMPenParallelTest,KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*  FMPenParallelTestPenDown
* DESCRIPTION
*	This function handles pen down events on Parallel Lines Test screen
*
* PARAMETERS
*	mmi_pen_point_struct
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
static void FMPenParallelTestPenDown(mmi_pen_point_struct pos)
{
	BOOL inside_region=FALSE;
	long x,y;

	x = (pos.x - gFMPenParallel_first_point_x)*(pos.x - gFMPenParallel_first_point_x);
	y = (pos.y - gFMPenParallel_first_point_y)*(pos.y - gFMPenParallel_first_point_y);

	if( (x + y) <= (gFMPenPointOffset*gFMPenPointOffset))
	{
		/*Starting from Top*/
		inside_region = TRUE;
		gFM_pen_test_start_from_top = TRUE;
	}
	else
	{
		x = (pos.x - gFMPenParallel_second_point_x)*(pos.x - gFMPenParallel_second_point_x);
		y = (pos.y - gFMPenParallel_second_point_y)*(pos.y - gFMPenParallel_second_point_y);
	
		if( (x + y) <= (gFMPenPointOffset*gFMPenPointOffset))
		{
			/*Starting from Bottom*/
			inside_region = TRUE;
			gFM_pen_test_start_from_top = FALSE;
		}	
	}
	if(inside_region)
	{
		gFMPenPreviousPoint.x=pos.x;
		gFMPenPreviousPoint.y=pos.y;
		gFMPenTestCapture = MMI_TRUE;
		gFMPenLineColor = coolsand_UI_color(0,150,0);
	}
	else
	{
		gFMPenPreviousPoint.x = -1;
		gFMPenPreviousPoint.y = -1;
		mmi_wgui_general_pen_down_hdlr(pos);
		gFMPenTestCapture = MMI_FALSE;
	}
	
}


/*****************************************************************************
* FUNCTION
*  FMPenParallelTestPenMove
* DESCRIPTION
*	This function handles pen move events on Parallel Lines Test screen
*
* PARAMETERS
*	mmi_pen_point_struct
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
 
* If pen is moved outside parallel lines test fails
*****************************************************************************/
static void FMPenParallelTestPenMove(mmi_pen_point_struct pos)
{
	float first_x1 = gFMPenParallel_first_x1;
	float first_x2 = gFMPenParallel_first_x2;
	float first_y1 = gFMPenParallel_first_y1;
	float first_y2 = gFMPenParallel_first_y2;
	float second_x1 = gFMPenParallel_second_x1;
	float second_x2 = gFMPenParallel_second_x2;
	float second_y1 = gFMPenParallel_second_y1;
	float second_y2 = gFMPenParallel_second_y2;
	BOOL inside_region=FALSE;
	float xdiff1,xdiff2,test1,test2;

	if (!gFMPenTestCapture)
	{
		mmi_wgui_general_pen_move_hdlr(pos);
		return;
	}
	
	xdiff1 = (first_x2-first_x1);
	xdiff2 = (second_x2-second_x1);

	if(0==xdiff1 || 0==xdiff2)
		ASSERT(0);
		
	test1 = pos.y - first_y1 - ((first_y2-first_y1)*(pos.x-first_x1))/(xdiff1);
	test2 = pos.y - second_y1 - ((second_y2-second_y1)*(pos.x-second_x1))/(xdiff2);

	switch(gFMPenParallel_test_type)
	{
	case PARALLEL_TEST_1:
		{
			if((test1 < 0) && (test2 > 0))
				inside_region = TRUE;
		}
		break;
	case PARALLEL_TEST_2:
		{	
			if((test1 > 0) && (test2 < 0))
				inside_region = TRUE;
		}
		break;
	default:
			ASSERT(0);
		break;
	}
	if(inside_region)
	{
		coolsand_UI_lock_double_buffer();
		coolsand_UI_push_clip();
		coolsand_UI_set_clip(0,0,UI_device_width-1,UI_device_height-1);
		coolsand_UI_line(gFMPenPreviousPoint.x,gFMPenPreviousPoint.y,pos.x,pos.y,gFMPenLineColor);
		coolsand_UI_pop_clip();
		coolsand_UI_unlock_double_buffer();
		coolsand_UI_BLT_double_buffer(PEN_MIN(gFMPenPreviousPoint.x, pos.x),
										PEN_MIN(gFMPenPreviousPoint.y, pos.y),
										PEN_MAX(gFMPenPreviousPoint.x, pos.x),
										PEN_MAX(gFMPenPreviousPoint.y, pos.y));
		gFMPenPreviousPoint.x = pos.x;
		gFMPenPreviousPoint.y = pos.y;
	}
	else
	{
		gFMPenPreviousPoint.x = -1;
		gFMPenPreviousPoint.y = -1;
		mmi_wgui_general_pen_move_hdlr(pos);
		gFMPenTestCapture = MMI_FALSE;
		DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 0, 1000, 0);
	}
	
}


/*****************************************************************************
* FUNCTION
*  FMPenParallelTestPenUp
* DESCRIPTION
*	This function handles pen UP event on Parallel Lines Test screen
*
* PARAMETERS
*	mmi_pen_point_struct
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
 
* When pen is UP is should be with in a predefined area (calculated by gFMPenPointOffset)
* of the end point of the test
*****************************************************************************/
void FM_Auto_Test_Pen_Parallel(void);
static void FMPenParallelTestPenUp(mmi_pen_point_struct pos)
{
	if (!gFMPenTestCapture)
	{
		mmi_wgui_general_pen_up_hdlr(pos);
	}
	else
	{
		long x,y;
		S16 result=0;
		if(gFM_pen_test_start_from_top)
		{
			x = (pos.x - gFMPenParallel_second_point_x)*(pos.x - gFMPenParallel_second_point_x);
			y = (pos.y - gFMPenParallel_second_point_y)*(pos.y - gFMPenParallel_second_point_y);
			if( (x + y) < (gFMPenPointOffset*gFMPenPointOffset))
			{
				result = 1;
			}
		}
		else
		{
			x = (pos.x - gFMPenParallel_first_point_x)*(pos.x - gFMPenParallel_first_point_x);
			y = (pos.y - gFMPenParallel_first_point_y)*(pos.y - gFMPenParallel_first_point_y);
			if( (x + y) < (gFMPenPointOffset*gFMPenPointOffset))
			{
				result = 1;
			}
		}
	
		if(result)
		{
			if(PARALLEL_TEST_1==gFMPenParallel_test_type)
			{
				DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 0, 1000, 0);
				gFMPenParallel_test_type = PARALLEL_TEST_2;
				#if 0
				if(g_test_without_intervenor)
				{
					coolsand_UI_start_timer(2000,FM_Auto_Test_Pen_Parallel);
				}
				#endif
			}
			else if(PARALLEL_TEST_2==gFMPenParallel_test_type)
			{
				addtohistory = FALSE;
				EntryNewScreen(SCR_ID_FM_PARALLEL_PEN,NULL,NULL,NULL);
				if (autoTestMode == 0)
				{
					DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 0, 1000, 0);
					gFMPenParallel_test_type = PARALLEL_TEST_1;
				}
				else
				{
					FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_PARALLEL_LINE].name);
					ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
						STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
						IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );
					if(!g_test_without_intervenor)
					{
						FM_Autotest_set_key_handlers();
					}
					else
					{
						coolsand_UI_start_timer(1000,FM_Handle_Pass_Key_Press);
					}
					gFMPenParallel_test_type = PARALLEL_TEST_1;
				}
			}
		}
		else
		{
			gFMPenPreviousPoint.x = -1;
			gFMPenPreviousPoint.y = -1;
			gFMPenTestCapture = MMI_FALSE;
			gFMPenLineColor =coolsand_UI_color(0,0,0);
			mmi_wgui_general_pen_up_hdlr(pos);
			DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 0, 1000, 0);
		}
	}
}


/*****************************************************************************
* FUNCTION
*  EmPenParallelTestSetupCoordinates
* DESCRIPTION
*	This function sets up coordinates according to type of test
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
 
* FIRST TEST :: \\
* SECOND TEST :: //
*****************************************************************************/
static void EmPenParallelTestSetupCoordinates(void)
{

	switch(gFMPenParallel_test_type)
	{
	case PARALLEL_TEST_1:
		{
			gFMPenParallel_first_x1 = 0;
			gFMPenParallel_first_y1 = 20;
			gFMPenParallel_first_x2 = UI_device_width-20;
			gFMPenParallel_first_y2 = UI_device_height;

			gFMPenParallel_second_x1 = 20;
			gFMPenParallel_second_y1 = 0;
			gFMPenParallel_second_x2 = UI_device_width;
			gFMPenParallel_second_y2 = UI_device_height-20;
			
			
			gFMPenParallel_first_point_x = 8;
			gFMPenParallel_first_point_y = 8;
			gFMPenParallel_second_point_x = UI_device_width-8;
			gFMPenParallel_second_point_y = UI_device_height-8;
		}
		break;
	case PARALLEL_TEST_2:
		{
			gFMPenParallel_first_x1 = UI_device_width-20;
			gFMPenParallel_first_y1 = 0;
			gFMPenParallel_first_x2 = 0;
			gFMPenParallel_first_y2 = UI_device_height-20;

			gFMPenParallel_second_x1 = UI_device_width;
			gFMPenParallel_second_y1 = 20;
			gFMPenParallel_second_x2 = 20;
			gFMPenParallel_second_y2 = UI_device_height;

			gFMPenParallel_first_point_x = UI_device_width-8;
			gFMPenParallel_first_point_y = 8;
			gFMPenParallel_second_point_x = 8;
			gFMPenParallel_second_point_y = UI_device_height-8;
		}
		break;
	default:
			ASSERT(0);
		break;
	}

}


/*****************************************************************************
* FUNCTION
*  EmPenParallelTestDisplay
* DESCRIPTION
*	This function displays Parallel lines and start and end points of the test
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
 
* FIRST TEST :: \\
* SECOND TEST :: //
*****************************************************************************/
static void EmPenParallelTestDisplay(void)
{
	clear_screen();
	coolsand_UI_lock_double_buffer();

	coolsand_UI_line(gFMPenParallel_first_x1, gFMPenParallel_first_y1 , gFMPenParallel_first_x2 , gFMPenParallel_first_y2, coolsand_UI_color(255, 0, 0));
	coolsand_UI_line(gFMPenParallel_second_x1, gFMPenParallel_second_y1 , gFMPenParallel_second_x2 , gFMPenParallel_second_y2, coolsand_UI_color(255, 0, 0));
	
	coolsand_UI_line(gFMPenParallel_first_point_x - 5, gFMPenParallel_first_point_y - 5, gFMPenParallel_first_point_x + 5, gFMPenParallel_first_point_y + 5, coolsand_UI_color(0, 0, 0));
	coolsand_UI_line(gFMPenParallel_first_point_x + 5, gFMPenParallel_first_point_y - 5, gFMPenParallel_first_point_x - 5, gFMPenParallel_first_point_y + 5, coolsand_UI_color(0, 0, 0));
	coolsand_UI_line(gFMPenParallel_second_point_x - 5, gFMPenParallel_second_point_y - 5, gFMPenParallel_second_point_x + 5, gFMPenParallel_second_point_y + 5, coolsand_UI_color(0, 0, 0));
	coolsand_UI_line(gFMPenParallel_second_point_x + 5, gFMPenParallel_second_point_y - 5, gFMPenParallel_second_point_x - 5, gFMPenParallel_second_point_y + 5, coolsand_UI_color(0, 0, 0));

	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);

}


/*****************************************************************************
* FUNCTION
*  ExitFMPenParallelTest
* DESCRIPTION
*	This function is Exit handler for Parallel lines Test
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void ExitFMPenParallelTest(void)
{
	if(addtohistory)
	{
		history_t currHistory;
		currHistory.scrnID = SCR_ID_FM_PARALLEL_PEN;
		currHistory.entryFuncPtr = EntryFMPenParallelTest;
		GetCategoryHistory (currHistory.guiBuffer);
		AddHistory (currHistory);
	}
}


/*****************************************************************************
* FUNCTION
*  FM_ParallelTestEndKeyTimeout
* DESCRIPTION
*	This function handles Long Press of End Key on Parallel Line Screen
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void FM_ParallelTestEndKeyTimeout(void)
{
	addtohistory = FALSE;
	gFMPenParallel_test_type = PARALLEL_TEST_1;
	FM_HandleKeypadEndout();
}

void FM_Auto_Test_Pen_Parallel(void)
{
	static U8 n_test_pos = 0;
//chenhe for jasper +
//160/128 = 220/176=1..25
#if (LCD_WIDTH == 128&&LCD_HEIGHT == 160)
#define N_MAX_POS	23

	mmi_pen_point_struct position1[N_MAX_POS] = {{8,8},{17,15},{20,20},{24,26},{29,33},{35,40},{43,51},{46,56},{51,60},{55,65},{60,71},{67,83},{72,90},
									    			{78,96},{84,102},{89,109},{93,114},{98,120},{104,128},{110,135},{114,140},{120,150},{120,150}};
	mmi_pen_point_struct position2[N_MAX_POS] = {{15,150},{22,139},{26,134},{30,129},{34,122},{37,116},{40,110},{46,101},{51,95},{58,87},{62,79},{66,74},{70,69},{75,61},
									    			{79,54},{87,46},{96,37},{101,32},{107,26},{112,19},{115,14},{120,10},{120,10}};
#elif( LCD_WIDTH == 176&&LCD_HEIGHT == 220)    
#define N_MAX_POS	23

	mmi_pen_point_struct position1[N_MAX_POS] = {{8,8},{17,15},{20,20},{24,26},{29,33},{35,40},{43,51},{46,56},{51,60},{55,65},{60,71},{67,83},{72,90},
									    			{78,96},{84,102},{89,109},{93,114},{98,120},{104,128},{110,135},{114,140},{120,150},{120,150}};
	mmi_pen_point_struct position2[N_MAX_POS] = {{15,150},{22,139},{26,134},{30,129},{34,122},{37,116},{40,110},{46,101},{51,95},{58,87},{62,79},{66,74},{70,69},{75,61},
									    			{79,54},{87,46},{96,37},{101,32},{107,26},{112,19},{115,14},{120,10},{120,10}};

#elif( LCD_WIDTH == 220&&LCD_HEIGHT == 176)    
#define N_MAX_POS	23

	mmi_pen_point_struct position1[N_MAX_POS] = {{8,8},{17,15},{20,20},{24,26},{29,33},{35,40},{43,51},{46,56},{51,60},{55,65},{60,71},{67,83},{72,90},
									    			{78,96},{84,102},{89,109},{93,114},{98,120},{104,128},{110,135},{114,140},{120,150},{120,150}};
	mmi_pen_point_struct position2[N_MAX_POS] = {{15,150},{22,139},{26,134},{30,129},{34,122},{37,116},{40,110},{46,101},{51,95},{58,87},{62,79},{66,74},{70,69},{75,61},
									    			{79,54},{87,46},{96,37},{101,32},{107,26},{112,19},{115,14},{120,10},{120,10}};

#elif  (LCD_WIDTH == 240&&LCD_HEIGHT == 320)

#define N_MAX_POS	60
#if 1
	UINT16 xStep = LCD_WIDTH/40,yStep = LCD_HEIGHT/40;
	UINT16 vi=0;
	mmi_pen_point_struct position1[N_MAX_POS],position2[N_MAX_POS] ;
	while(vi++<N_MAX_POS-1)
	{
		position1[vi].x=vi*xStep;//chenhe,the position is begin at:(0,0) to (LCD_WIDTH,LCD_HEIGHT)
		position1[vi].y=vi*yStep;
		mmi_trace(1,"position1[%d].x is %d,position1[vi].y is %d",vi,position1[vi].x,position1[vi].y);
	}
	position1[0].x=8;//for at least is 8
	position1[0].y=8;//for at least is 8
	position1[N_MAX_POS-1].x=LCD_WIDTH-8;
	position1[N_MAX_POS-1].y=LCD_HEIGHT-8;
	vi=0;
	
	while(vi++<N_MAX_POS)
	{
		position2[vi].x=vi*xStep;//chenhe,the position is begin at:(0,LCD_HEIGHT) to (LCD_WIDTH,0)
		position2[vi].y=LCD_HEIGHT-vi*yStep;
		//mmi_trace(1,"position2[vi].x is %d,position1[vi].y is %d",position1[vi].x,position1[vi].y);
	}
	position2[0].x =8;//for at least is 8
	position2[0].y =LCD_HEIGHT-8;//for at least is 8
	position2[N_MAX_POS-1].x=LCD_WIDTH-8;
	position2[N_MAX_POS-1].y=8;
#else
    	mmi_pen_point_struct position1[N_MAX_POS] = {{8,8},{17,15},{20,20},{24,26},{29,33},{35,40},{43,51},{46,56},{51,60},{55,65},{60,71},{67,83},{72,90},
									    			{78,96},{84,102},{89,109},{93,114},{98,120},{104,128},{110,135},{114,140},{120,150},{120,150},//x add 6 (LCD_WIDTH 240/40 /*40 pinits*/),y =x*HEIGHT/WIDTH
									    			{126,166},{132,176},{138,184},{144,192},{150,200},{156,208},{162,216},{168,224},{174,232},{180,240},{186,248},{192,256},{198,264},
									    			{204,272},{210,280},{216,288},{222,296},{228,304},{234,312},{234,312}};
	mmi_pen_point_struct position2[N_MAX_POS] = {{15,150},{22,139},{26,134},{30,129},{34,122},{37,116},{40,110},{46,101},{51,95},{58,87},{62,79},{66,74},{70,69},{75,61},
									    			{79,54},{87,46},{96,37},{101,32},{107,26},{112,19},{115,14},{120,10},{120,10},
									    			{15,150},{22,139},{26,134},{30,129},{34,122},{37,116},{40,110},{46,101},{51,95},{58,87},{62,79},{66,74},{70,69},{75,61},
									    			{79,54},{87,46},{96,37},{101,32},{107,26},{112,19},{115,14},{120,10},{120,10}};
#endif
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#elif  (LCD_WIDTH == 320&&LCD_HEIGHT == 240)

#define N_MAX_POS	60
	UINT16 xStep = LCD_WIDTH/40,yStep = LCD_HEIGHT/40;
	UINT16 vi=0;
	mmi_pen_point_struct position1[N_MAX_POS],position2[N_MAX_POS] ;
	while(vi++<N_MAX_POS-1)
	{
		position1[vi].x=vi*xStep;//chenhe,the position is begin at:(0,0) to (LCD_WIDTH,LCD_HEIGHT)
		position1[vi].y=vi*yStep;
		mmi_trace(1,"position1[%d].x is %d,position1[vi].y is %d",vi,position1[vi].x,position1[vi].y);
	}
	position1[0].x=8;//for at least is 8
	position1[0].y=8;//for at least is 8
	position1[N_MAX_POS-1].x=LCD_WIDTH-8;
	position1[N_MAX_POS-1].y=LCD_HEIGHT-8;
	vi=0;
	
	while(vi++<N_MAX_POS)
	{
		position2[vi].x=vi*xStep;//chenhe,the position is begin at:(0,LCD_HEIGHT) to (LCD_WIDTH,0)
		position2[vi].y=LCD_HEIGHT-vi*yStep;
		//mmi_trace(1,"position2[vi].x is %d,position1[vi].y is %d",position1[vi].x,position1[vi].y);
	}
	position2[0].x =8;//for at least is 8
	position2[0].y =LCD_HEIGHT-8;//for at least is 8
	position2[N_MAX_POS-1].x=LCD_WIDTH-8;
	position2[N_MAX_POS-1].y=8;
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#endif
//chenhe for jasper -    
	//static U8 n_test_pos = 0;
	if(PARALLEL_TEST_1==gFMPenParallel_test_type)
	{
		if(n_test_pos == 0)
		{
			FMPenParallelTestPenDown(position1[n_test_pos]);
			n_test_pos++;
			coolsand_UI_start_timer(100,FM_Auto_Test_Pen_Parallel);
		}
		else if(n_test_pos ==N_MAX_POS -1)
		{
			FMPenParallelTestPenUp(position1[n_test_pos]);
			n_test_pos = 0;
		}
		else
		{
			FMPenParallelTestPenMove(position1[n_test_pos]);
			n_test_pos++;
			coolsand_UI_start_timer(100,FM_Auto_Test_Pen_Parallel);
		}	
	}
	else if(PARALLEL_TEST_2==gFMPenParallel_test_type)
	{
		if(n_test_pos == 0)
		{
			FMPenParallelTestPenDown(position2[n_test_pos]);
			n_test_pos++;
			coolsand_UI_start_timer(100,FM_Auto_Test_Pen_Parallel);
		}
		else if(n_test_pos ==N_MAX_POS -1)
		{
			FMPenParallelTestPenUp(position2[n_test_pos]);
			n_test_pos = 0;
		}
		else
		{
			FMPenParallelTestPenMove(position2[n_test_pos]);
			n_test_pos++;
			coolsand_UI_start_timer(100,FM_Auto_Test_Pen_Parallel);
		}
	}
	
}
/*****************************************************************************
* FUNCTION
*  EntryFMPenParallelTest
* DESCRIPTION
*	This function is Entry function of Parallel Line Screen
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryFMPenParallelTest(void)
{
	addtohistory = FALSE;
	EntryNewScreen(SCR_ID_FM_PARALLEL_PEN, ExitFMPenParallelTest, NULL, NULL);
	addtohistory = TRUE;
	
	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
	SetKeyHandler(FM_ParallelTestEndKeyTimeout, KEY_END, KEY_EVENT_LONG_PRESS);
	gFMPenPreviousPoint.x = -1;
	gFMPenPreviousPoint.y = -1;
	
	EmPenParallelTestSetupCoordinates();
	entry_full_screen();
	EmPenParallelTestDisplay();
	
	mmi_wgui_register_pen_down_handler(FMPenParallelTestPenDown);
	mmi_wgui_register_pen_move_handler(FMPenParallelTestPenMove);
	mmi_wgui_register_pen_up_handler(FMPenParallelTestPenUp);
	mmi_wgui_register_pen_repeat_handler(mmi_pen_dummy_hdlr);
	mmi_wgui_register_pen_long_tap_handler(mmi_pen_dummy_hdlr);
	mmi_wgui_register_pen_abort_handler(mmi_pen_dummy_hdlr);
	#if 1
	if(g_test_without_intervenor)
	{
		coolsand_UI_start_timer(1000,FM_Auto_Test_Pen_Parallel);
	}
	#endif
}


/*****************************************************************************
* FUNCTION
*  FM_AutoTest_Pen_Parallel_Test
* DESCRIPTION
*	This function is for Parallel line Test in Auto Test mode
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void FM_AutoTest_Pen_Parallel_Test(void)
{
	currentTest = FM_TEST_PEN_PARALLEL_LINES;
	EntryFMPenParallelTest();
}


/*N Cross M Points Test*/

#define TOUCH_N_ROWS	4		/*Number of Rows of Points*/
#define TOUCH_N_COLS	5		/*Number of Columns of Points*/
#define IMAGE_HIDE		0		/*Hide image of Point*/
#define IMAGE_SHOW		1		/*Show image of Point*/
// TODO: dynamic compute margin
#define TOUCH_ROW_MARGIN	30	/*Horizontal Margin between points*/
#define TOUCH_COL_MARGIN	15	/*Vertical Margin between points*/

static S16 gFMPenNByMStartx;
static S16 gFMPenNByMStarty;

static S32 gFMPenNByMImageHeight;	/*Point Image's height*/
static S32 gFMPenNByMImageWidth;	/*Point Image's width*/

static S16 gFMPenNByMStatus[TOUCH_N_ROWS][TOUCH_N_COLS];	/*Show hide point status*/


/*****************************************************************************
* FUNCTION
*  HighlightFMNCrossMTestHdlr
* DESCRIPTION
*	This function is Highlight handler for N Cross M points Test
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightFMNCrossMTestHdlr(void)
{
	SetLeftSoftkeyFunction(EntryFMPenNCrossMTest,KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*  FMPenNByMTestInitStatus
* DESCRIPTION
*	This function initializes image status array of the points
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
static void FMPenNByMTestInitStatus(void)
{
	S32 x,y;
	
	for(x=0;x<TOUCH_N_ROWS;x++)
	{
		for(y=0;y<TOUCH_N_COLS;y++)
		{
			gFMPenNByMStatus[x][y]=IMAGE_SHOW;
		}
	}
	
}


/*****************************************************************************
* FUNCTION
*  FMPenNByMPenTestStatus
* DESCRIPTION
*	This function checks image status array of the points to see if all the images
*	have been clicked
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
static BOOL FMPenNByMPenTestStatus(void)
{
	S32 x,y;
	for(x=0;x<TOUCH_N_ROWS;x++)
	{
		for(y=0;y<TOUCH_N_COLS;y++)
		{
			if(IMAGE_SHOW == gFMPenNByMStatus[x][y])
				return FALSE;
		}
	}
	return TRUE;
}


/*****************************************************************************
* FUNCTION
*  FMPenNByMTestDisplay
* DESCRIPTION
*	This function displays point images according to image status array of the points
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
static void FMPenNByMTestDisplay(void)
{
	S32 r,c,x,y;
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif	/*UI_DOUBLE_BUFFER_SUPPORT*/
	coolsand_UI_push_clip();
	coolsand_UI_set_clip(0,0,UI_device_width-1,UI_device_height-1);
	clear_screen();
	for(r=0;r<TOUCH_N_ROWS;r++)
	{
		for(c=0;c<TOUCH_N_COLS;c++)
		{
			if(IMAGE_SHOW == gFMPenNByMStatus[r][c])
			{
				x = gFMPenNByMStartx + c*(gFMPenNByMImageWidth + TOUCH_COL_MARGIN);
				y = gFMPenNByMStarty + r*(gFMPenNByMImageHeight + TOUCH_ROW_MARGIN);
				coolsand_UI_show_image(x,y,get_image((UI_image_ID_type) (IMG_GLOBAL_L1+c)));
			}
		}
	}
	coolsand_UI_pop_clip();
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0,0,UI_device_width,UI_device_height);
#endif	/*UI_DOUBLE_BUFFER_SUPPORT*/
}


/*****************************************************************************
* FUNCTION
*  FMPenNByMTestTranslatePenPosition
* DESCRIPTION
*	This function translates pen position on N Cross M points test screen
*
* PARAMETERS
*  mmi_pen_point_struct 
*  row	
*  col
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
static void FMPenNByMTestTranslatePenPosition(mmi_pen_point_struct pos,S32 *row,S32 *col)
{
	S32 end_x,end_y,x,y,r,c,x1,y1,image_height,image_width,start_x,start_y;
	x = pos.x;
	y = pos.y;
	*row = -1;
	*col = -1;
	start_x = gFMPenNByMStartx;
	start_y = gFMPenNByMStarty;
	image_height = gFMPenNByMImageHeight;
	image_width = gFMPenNByMImageWidth;
	end_x = start_x + TOUCH_N_COLS*image_width + (TOUCH_N_COLS-1)*TOUCH_COL_MARGIN;
	end_y = start_y + TOUCH_N_ROWS*image_height + (TOUCH_N_ROWS-1)*TOUCH_ROW_MARGIN;

	if(PEN_CHECK_BOUND(x,y,start_x,start_y,end_x,end_y))
	{
		for(y1 = start_y,r=0;y1<end_y;y1+=image_height+TOUCH_ROW_MARGIN,r++)
		{
			if( y > y1 && y < y1 + image_height)
				*row = r;
		}
		for(x1 = start_x,c=0;x1<end_x;x1+=image_width+TOUCH_COL_MARGIN,c++)
		{
			if( x > x1 && x < x1 + image_width)
				*col = c;
		}
	}
}


/*****************************************************************************
* FUNCTION
*  FMPenNByMTestPenDown
* DESCRIPTION
*	This function handles Pen Down Events on N Cross M Points screen
*
* PARAMETERS
*  mmi_pen_point_struct
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
static void FMPenNByMTestPenDown(mmi_pen_point_struct pos)
{
	S32 x,y;

	FMPenNByMTestTranslatePenPosition(pos,&x,&y);

	if(-1 != x && -1 != y)
	{
		gFMPenNByMStatus[x][y] = IMAGE_HIDE;
	}
	if(FMPenNByMPenTestStatus())
	{
		EntryNewScreen(SCR_ID_FM_N_CROSS_M_PEN,NULL,NULL,NULL);
		if(0==autoTestMode)
		{
			DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 0, 1000, 0);
		}
		else
		{
			FM_Autotest_test_done_USC2string(Tests[FM_AUTOTEST_N_CROSS_M_POINTS].name);
			ShowCategory7Screen( STR_ID_FM_AUTO_TEST_ROOT, IMG_ID_FM_AUTO_TEST_ROOT,
								STR_ID_FM_AUTO_TEST_PASS, IMG_GLOBAL_OK, STR_ID_FM_AUTO_TEST_FAIL,
								IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );
			FM_Autotest_set_key_handlers();
		}
	}
	else
	{
		FMPenNByMTestDisplay();
	}
}


/*****************************************************************************
* FUNCTION
*  EntryFMPenNCrossMTest
* DESCRIPTION
*	This function is Entry Function of N Cross M Points screen
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryFMPenNCrossMTest(void)
{
	EntryNewScreen(SCR_ID_FM_N_CROSS_M_PEN, NULL, NULL, NULL);
	
	coolsand_UI_measure_image(get_image(IMG_GLOBAL_L1),&gFMPenNByMImageWidth,&gFMPenNByMImageHeight);
	gFMPenNByMStartx = (UI_device_width - ((TOUCH_N_COLS*gFMPenNByMImageWidth) + (TOUCH_N_COLS-1)*TOUCH_COL_MARGIN))/2;
	gFMPenNByMStarty = (UI_device_height - ((TOUCH_N_ROWS*gFMPenNByMImageHeight) + (TOUCH_N_ROWS-1)*TOUCH_ROW_MARGIN))/2;

	ASSERT(gFMPenNByMStartx >= 0 && gFMPenNByMStartx <= UI_device_width);
	ASSERT(gFMPenNByMStarty >= 0 && gFMPenNByMStarty <= UI_device_height);

	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
	SetKeyHandler(FM_HandleKeypadEndout, KEY_END, KEY_EVENT_LONG_PRESS);
	FMPenNByMTestInitStatus();
	entry_full_screen();
	FMPenNByMTestDisplay();

	mmi_wgui_register_pen_up_handler(mmi_pen_dummy_hdlr);
	mmi_wgui_register_pen_down_handler(FMPenNByMTestPenDown);
	mmi_wgui_register_pen_move_handler(mmi_pen_dummy_hdlr);
	mmi_wgui_register_pen_repeat_handler(mmi_pen_dummy_hdlr);
	mmi_wgui_register_pen_long_tap_handler(mmi_pen_dummy_hdlr);
	mmi_wgui_register_pen_abort_handler(mmi_pen_dummy_hdlr);	
}


/*****************************************************************************
* FUNCTION
*  FM_AutoTest_Pen_N_Cross_M_Test
* DESCRIPTION
*	This function sets is for N Cross M point test in Auto Test Mode
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void FM_AutoTest_Pen_N_Cross_M_Test(void)
{
	currentTest = FM_TEST_PEN_N_CROSS_M_POINTS;
	EntryFMPenNCrossMTest();
}


#endif /*__MMI_TOUCH_SCREEN__*/
#if 1
//#include "hal_ct.h"
/*
========================================================   
Flash的特性是擦除过后全部为1，所以这里认为如果标志位1
则是工位不成功，标志位为0，工位执行成功
另外两个结构体factory_settings_t，bind_info_t是连续存储
在FLASH中
========================================================
*/
BOOL MMI_CheckValue(void)
{
#ifndef MMI_ON_WIN32
  factory_settings_t* factory_settings_p = (factory_settings_t*)&_factory_start;
	CSW_TRACE(33, TSTXT(" ############valid_marker=%x\r\n"),&(factory_settings_p->valid_marker));
  
  UINT32 nValue = (factory_settings_p->valid_marker);
  if((nValue>>24) == ((nValue & 0xFFFFFF) %0xFF))
  {
      CSW_TRACE(CFW_SHELL_TS_ID, "===========>Factory_SettingMMI_CheckValue is success.\n");
      return TRUE;
  }
  else
  {
      CSW_TRACE(CFW_SHELL_TS_ID, "===========>Factory_SettingMMI_CheckValue is failed.\n");
      return FALSE;
  }
#else
  return FALSE;
#endif
}
//
//Read ALl factory setting date
//
VOID _ReadAllFacSetingDate(VOID * pDate)
{
#ifndef MMI_ON_WIN32
  factory_settings_t* factory_settings_p = (factory_settings_t*)&_factory_start;
  SUL_MemCopy8(pDate, factory_settings_p, SIZEOF(factory_settings_t) + SIZEOF(bind_info_t));
#endif
}
/*
========================================================   
 
-------------------------------------------------------
   Scope      : Get the status about auto dial, auto
                Response, auto test and Normal
   Parameters : The output status(auto dial, auto response,
 auto test and Normal)
   Return     : TRUE/FALSE.
========================================================
*/
BOOL FM_GetValidMarker(UINT8 *pStatus)
{
#ifndef MMI_ON_WIN32
  //
  //Check the date valid
  //
  if(!MMI_CheckValue())
  {
    return FALSE;
  }
  
  factory_settings_t* factory_settings_p=NULL;
    
	factory_settings_p=(factory_settings_t*)&_factory_start;
	CSW_TRACE(33, TSTXT(" ############valid_marker=%x\r\n"),
		&(factory_settings_p->valid_marker));
  
  *pStatus = ((factory_settings_p->valid_marker)>>16) & 0xFF;
#endif
  return TRUE;

}


/*
========================================================   
 
-------------------------------------------------------
   Scope      : Set auto dial, auto response and auto test
                Normal
   Parameters : The input status(auto dial, auto response
                and auto test Normal)
   Return     : TRUE/FALSE.
========================================================
*/
BOOL FM_SetValidMarker(UINT8  nStatus)
{
#ifndef MMI_ON_WIN32
  UINT32 nWrittenLen = 0x00;
  UINT32 nToWriteLen = SIZEOF(factory_settings_t) + SIZEOF(bind_info_t);

  if(!MMI_CheckValue())
  {
    return FALSE;
  }
  UINT8 _Status = 0x00;
  BOOL nRet = FM_GetValidMarker(&_Status);
  if(TRUE != nRet)
  {
    return FALSE;
  }
  if(_Status == nStatus)
  {
    return TRUE;
  }
  UINT8 Date[SIZEOF(factory_settings_t) + SIZEOF(bind_info_t)];
  SUL_ZeroMemory8(Date, SIZEOF(factory_settings_t) + SIZEOF(bind_info_t));
  _ReadAllFacSetingDate(Date);
  factory_settings_t *pFS = (factory_settings_t *)Date;
  pFS->valid_marker = (pFS->valid_marker | 0xFF0000) | (nStatus << 16);

  pFS->valid_marker &= 0xFFFFFF;
  pFS->valid_marker |= ((pFS->valid_marker &0xFFFFFF)%0xFF)<<24;
  
  if(ERR_SUCCESS != DRV_EraseFlashSector ( ((UINT32)&_factory_start)&0x1fffffff))
  {
      return FALSE;
  }


  if(ERR_SUCCESS != DRV_WriteFlash ( ((UINT32)&_factory_start)&0x1fffffff,Date,nToWriteLen,&nWrittenLen))
  {
      return FALSE;
  }
  if(nToWriteLen != nWrittenLen)
  {
      return FALSE;
  }
#endif
  return TRUE;

}


/*
========================================================   
Function   : CFW_GetProductionStatus 
-------------------------------------------------------
   Scope      : 该函数用来得到各个工位是否完成
   Parameters : nFP表示工位的ID
   Return     : TRUE/FALSE.
========================================================
*/
BOOL FM_GetProductionStatus(UINT8 nFP)
{
#ifndef MMI_ON_WIN32
  //
  //Check the date valid
  //
  if(!MMI_CheckValue())
  {
    return FALSE;
  }
  
  factory_settings_t* factory_settings_p=NULL;
    
	factory_settings_p=(factory_settings_t*)&_factory_start;
	CSW_TRACE(33, TSTXT(" ############valid_marker=%x\r\n"),
		&(factory_settings_p->valid_marker));
  
  return !(((factory_settings_p->valid_marker)>>(nFP - 1)) & 0x01);
#else
  return FALSE;
#endif
}


/*
========================================================   
Function   : CFW_SetProductionStatus
-------------------------------------------------------
   Scope      : 该函数用来设置各个工位是否完成
   Parameters : nFP表示工位的ID,
   Return     : TRUE/FALSE.
========================================================
*/
BOOL FM_SetProductionStatus(UINT8 nFP,BOOL nResult)
{
#ifndef MMI_ON_WIN32
  UINT32 nWrittenLen = 0x00;
  UINT32 nToWriteLen = SIZEOF(factory_settings_t) + SIZEOF(bind_info_t);

  if(!MMI_CheckValue())
  {
       return FALSE;
  }

  if(nResult == FM_GetProductionStatus(nFP))
  {
       return TRUE;
  }

  UINT8 Date[SIZEOF(factory_settings_t) + SIZEOF(bind_info_t)];
  SUL_ZeroMemory8(Date, SIZEOF(factory_settings_t) + SIZEOF(bind_info_t));
  _ReadAllFacSetingDate(Date);
  factory_settings_t *pFS = (factory_settings_t *)Date;

  pFS->valid_marker = (pFS->valid_marker | 1<<(nFP -1) ) | (!(nResult << (nFP -1)));

  pFS->valid_marker &= 0xFFFFFF;
  pFS->valid_marker |= ((pFS->valid_marker & 0xFFFFFF)%0xFF)<<24;
  
  if(ERR_SUCCESS != DRV_EraseFlashSector ( (UINT32)&_factory_start))
  {
      return FALSE;
  }


  if(ERR_SUCCESS != DRV_WriteFlash ( (UINT32)&_factory_start,Date,nToWriteLen,&nWrittenLen))
  {
      return FALSE;
  }
  if(nToWriteLen != nWrittenLen)
  {
      return FALSE;
  }
#endif
  return TRUE;
}


#endif

#ifdef __MMI_FM_RADIO__

typedef struct {
    U16      text_id;
    U16      Currfreq;
    U8       CurrVolume;
    U8       is_FMRDO_on;
} fm_fm_radio_struct;

fm_fm_radio_struct g_fm_fm_radio_cntx={0};

U8 FmcurrentHighlightIndex;

void EntryFMFMRadioTest(void);
void FMRADIOChannelTestInit(void);
void EntryFMRADIOChannelTest(void);
void FmFMRadioLeftArrowHdlr(void);
void FmFMRadioRightArrowHdlr(void);
void FmFMRadioUpArrowHdlr(void);
void FmFMRadioDownArrowHdlr(void);
extern S8 *mmi_ucs2cat(S8 *strDestination, const S8 *strSource);
/*****************************************************************************
 * FUNCTION
 *  FMSetHighlightIndex
 * DESCRIPTION
 *  
 * PARAMETERS
 *  nIndex      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void FMSetHighlightIndex(S32 nIndex)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    FmcurrentHighlightIndex = (U8) nIndex;
}

/*****************************************************************************
 * FUNCTION
 *  HighlightFMFMRadiohdlr
 * DESCRIPTION
 *  This function is Highlight handler for FM Radio Test
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void HighlightFMFMRadiohdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(EntryFMFMRadioTest, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    change_left_softkey(STR_GLOBAL_OK, 0);
    redraw_left_softkey();
}


/*****************************************************************************
 * FUNCTION
 *  EntryFMFMRadioTest
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryFMFMRadioTest(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 ItemList[] = 
    {
        STR_ID_FM_FM_RADIO_CHANNEL_1,
        STR_ID_FM_FM_RADIO_CHANNEL_2,
        STR_ID_FM_FM_RADIO_CHANNEL_3
    };
    U16 ItemIcons[3];
    U16 nItems = 3;
    U8 *guiBuffer;
    U8 *PopUpList[3];
    U8 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* If FM Radio is turned on, then stop/turn-off it */
    if (g_fm_fm_radio_cntx.is_FMRDO_on)
    {
        mdi_audio_stop_fmr();
        g_fm_fm_radio_cntx.is_FMRDO_on = 0;
    }
    
    EntryNewScreen(SCR_ID_FM_FM_RADIO, NULL, EntryFMFMRadioTest, NULL);
    guiBuffer = GetCurrGuiBuffer(SCR_ID_FM_FM_RADIO);
    SetParentHandler(MENU_ID_FM_FMRADIO);

    RegisterHighlightHandler(FMSetHighlightIndex);

    for (i = 0; i < nItems; i++)
    {
        PopUpList[i] = NULL;
        ItemIcons[i] = (U16) (IMG_GLOBAL_L1 + i);   
    }

    ShowCategory52Screen(
        STR_ID_FM_FM_RADIO,
        0,
        STR_GLOBAL_OK,
        0,
        STR_GLOBAL_BACK,
        0,
        nItems,
        ItemList,
        ItemIcons,
        (U8 **) PopUpList,
        0,
        0,
        guiBuffer);
	

    SetLeftSoftkeyFunction(FMRADIOChannelTestInit, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  EntryFMRADIOChannelTestInit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
  extern mdi_result mdi_audio_stop_fmr( void );
 void ExitFMRADIOChannelTest(void)
{
	mdi_audio_stop_fmr();
	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);
}
void FMRADIOChannelTestInit(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
//    S16 error = 0;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* init volume */
    g_fm_fm_radio_cntx.CurrVolume = 3;


   /* Entry New Screen & Show Category */
    EntryNewScreen(SCR_ID_FM_FM_RADIO_CHANNEL_TEST, ExitFMRADIOChannelTest, NULL, NULL);
   
    mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);
/* Turn on FM Radio */
    mdi_fmr_power_on_with_path( MDI_DEVICE_SPEAKER_BOTH, (mdi_callback) NULL);  

    /* Read channel info from NVRAM */
    switch(FmcurrentHighlightIndex)
    {
        case 0:   /* Channel 1 */
        {
			#if 0
            ReadValue(NVRAM_FM_FM_RADIO_CHANNEL_1, &g_fm_fm_radio_cntx.Currfreq, DS_SHORT, &error);
            if (g_fm_fm_radio_cntx.Currfreq == 0xffff)
            {
                g_fm_fm_radio_cntx.Currfreq = 875;    /* init value */
                WriteValue(NVRAM_FM_FM_RADIO_CHANNEL_1, &g_fm_fm_radio_cntx.Currfreq, DS_SHORT, &error);
            }
			#endif
		 g_fm_fm_radio_cntx.Currfreq = 875;    /* init value */
            g_fm_fm_radio_cntx.text_id= STR_ID_FM_FM_RADIO_CHANNEL_1;
            break;
        }
        case 1:   /* Channel 2 */
        {
			#if 0
            ReadValue(NVRAM_FM_FM_RADIO_CHANNEL_2, &g_fm_fm_radio_cntx.Currfreq, DS_SHORT, &error);
            if (g_fm_fm_radio_cntx.Currfreq == 0xffff)
            {
                g_fm_fm_radio_cntx.Currfreq = 875;    /* init value */
                WriteValue(NVRAM_FM_FM_RADIO_CHANNEL_2, &g_fm_fm_radio_cntx.Currfreq, DS_SHORT, &error);
            }
			#endif
			 g_fm_fm_radio_cntx.Currfreq = 875;    /* init value */
            g_fm_fm_radio_cntx.text_id = STR_ID_FM_FM_RADIO_CHANNEL_2;            
            break;
        }
        case 2:   /* Channel 3 */
        {
			#if 0
            ReadValue(NVRAM_FM_FM_RADIO_CHANNEL_3, &g_fm_fm_radio_cntx.Currfreq, DS_SHORT, &error);
            if (g_fm_fm_radio_cntx.Currfreq == 0xffff)
            {
                g_fm_fm_radio_cntx.Currfreq = 875;    /* init value */
                WriteValue(NVRAM_FM_FM_RADIO_CHANNEL_3, &g_fm_fm_radio_cntx.Currfreq, DS_SHORT, &error);
            }            
			#endif
			 g_fm_fm_radio_cntx.Currfreq = 875;    /* init value */
            g_fm_fm_radio_cntx.text_id = STR_ID_FM_FM_RADIO_CHANNEL_3;            
            break;
        }
        default:
        {
            ASSERT(0);
            break;
        }
    }
    
    EntryFMRADIOChannelTest();
}


/*****************************************************************************
 * FUNCTION
 *  EntryFMRADIOChannelTest
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryFMRADIOChannelTest(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8* guiBuffer;
    S8 stringbuf[110];
    S8 tmpStr[12];
    S8 tmpStr_UCS2[24];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_fm_fm_radio_cntx.is_FMRDO_on = 1;

	/* set audio volume */
    mdi_audio_set_volume( 3,  g_fm_fm_radio_cntx.CurrVolume);

    /* set FM Radio Frequency */
    mdi_fmr_set_freq( g_fm_fm_radio_cntx.Currfreq);

	
   /* initialize display string */
    memset(stringbuf, 0, 100);

    /* frequency */
    memset(tmpStr, 0, 10);
    sprintf(tmpStr, "Fre:");
    AnsiiToUnicodeString(tmpStr_UCS2, tmpStr);
    mmi_ucs2cat(stringbuf, tmpStr_UCS2);
    
    memset(tmpStr, 0, 10);
    sprintf(tmpStr, "  %d.%d \n\n",  g_fm_fm_radio_cntx.Currfreq/10, g_fm_fm_radio_cntx.Currfreq%10);
    AnsiiToUnicodeString(tmpStr_UCS2, tmpStr);
    mmi_ucs2cat(stringbuf, tmpStr_UCS2);

    /* volume */
    memset(tmpStr, 0, 10);
    sprintf(tmpStr, "Vol:");
    AnsiiToUnicodeString(tmpStr_UCS2, tmpStr);
    mmi_ucs2cat(stringbuf, tmpStr_UCS2);
    
    memset(tmpStr, 0, 10);
    sprintf(tmpStr, "    %d\n\n", g_fm_fm_radio_cntx.CurrVolume);
    AnsiiToUnicodeString(tmpStr_UCS2, tmpStr);
    mmi_ucs2cat(stringbuf, tmpStr_UCS2);

    
    guiBuffer = GetCurrGuiBuffer (SCR_ID_FM_FM_RADIO_CHANNEL_TEST);
    
    ShowCategory7Screen(g_fm_fm_radio_cntx.text_id, 0, 0, 0,
                        STR_GLOBAL_BACK, IMG_GLOBAL_BACK, (PU8)stringbuf, guiBuffer);

    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

    /* Register End key handler */
    SetKeyHandler(GoBackHistory, KEY_END , KEY_EVENT_DOWN);

    SetKeyHandler(FmFMRadioLeftArrowHdlr, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(FmFMRadioRightArrowHdlr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(FmFMRadioUpArrowHdlr, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(FmFMRadioDownArrowHdlr, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    
}


/**************************************************************
**	FUNCTION NAME		: FmFMRadioLeftArrowHdlr
**
**  	PURPOSE				: Entry function to FM Radio RSSI Info Left Arrow Hdler
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FmFMRadioLeftArrowHdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
//    S16 error = 0;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Channel Freq. Range: 875 - 1080 */
    if (g_fm_fm_radio_cntx.Currfreq > 875)
        g_fm_fm_radio_cntx.Currfreq -= 1;

    /* Restore the changed frequency to NVRAM */
    switch(FmcurrentHighlightIndex)
    {
        case 0:   /* Channel 1 */
        {
 //           WriteValue(NVRAM_FM_FM_RADIO_CHANNEL_1, &g_fm_fm_radio_cntx.Currfreq, DS_SHORT, &error);
            break;
        }
        case 1:   /* Channel 2 */
        {
//            WriteValue(NVRAM_FM_FM_RADIO_CHANNEL_2, &g_fm_fm_radio_cntx.Currfreq, DS_SHORT, &error);
            break;
        }
        case 2:   /* Channel 3 */
        {
//            WriteValue(NVRAM_FM_FM_RADIO_CHANNEL_3, &g_fm_fm_radio_cntx.Currfreq, DS_SHORT, &error);
            break;
        }
        default:
        {
            ASSERT(0);
            break;
        }
    }

    EntryFMRADIOChannelTest();
}


/**************************************************************
**	FUNCTION NAME		: FmFMRadioRightArrowHdlr
**
**  	PURPOSE				: Entry function to FM Radio RSSI Info Right Arrow Hdler
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FmFMRadioRightArrowHdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
//    S16 error = 0;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Channel Freq. Range: 875 - 1080 */    
    if (g_fm_fm_radio_cntx.Currfreq < 1080)
        g_fm_fm_radio_cntx.Currfreq += 1;

    /* Restore the changed frequency to NVRAM */
    switch(FmcurrentHighlightIndex)
    {
        case 0:   /* Channel 1 */
        {
//            WriteValue(NVRAM_FM_FM_RADIO_CHANNEL_1, &g_fm_fm_radio_cntx.Currfreq, DS_SHORT, &error);
            break;
        }
        case 1:   /* Channel 2 */
        {
 //           WriteValue(NVRAM_FM_FM_RADIO_CHANNEL_2, &g_fm_fm_radio_cntx.Currfreq, DS_SHORT, &error);
            break;
        }
        case 2:   /* Channel 3 */
        {
  //          WriteValue(NVRAM_FM_FM_RADIO_CHANNEL_3, &g_fm_fm_radio_cntx.Currfreq, DS_SHORT, &error);
            break;
        }
        default:
        {
            ASSERT(0);
            break;
        }
    }
    
    EntryFMRADIOChannelTest();
}


/**************************************************************
**	FUNCTION NAME		: FmFMRadioUpArrowHdlr
**
**  	PURPOSE				: Entry function to FM Radio RSSI Info Up Arrow Hdler
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FmFMRadioUpArrowHdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_fm_fm_radio_cntx.CurrVolume < MAX_VOL_LEVEL - 1)
        g_fm_fm_radio_cntx.CurrVolume++;

    EntryFMRADIOChannelTest();
}


/**************************************************************
**	FUNCTION NAME		: FmFMRadioDownArrowHdlr
**
**  	PURPOSE				: Entry function to FM Radio RSSI Info Down Arrow Hdler
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void FmFMRadioDownArrowHdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_fm_fm_radio_cntx.CurrVolume > 0)
        g_fm_fm_radio_cntx.CurrVolume--;

    EntryFMRADIOChannelTest();
}

#endif /* #ifdef __MMI_FM_RADIO__ */

#ifdef TGT_AUTOGEN_MODULE_NAME
#ifdef MMI_ON_HARDWARE_P
extern void tgt_GetHardwareModuleSettings(TGT_MODULE_T* hardwareModule);

void EntryFMHWVersionSummaryMenu(void)
{
    TGT_MODULE_T hwModule;
    U8*	guiBuffer;
    
	EntryNewScreen(SCR_ID_FM_HW_VER,NULL,EntryFMHWVersionSummaryMenu,NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_FM_HW_VER);

    tgt_GetHardwareModuleSettings(&hwModule);  
	memset(UnicodeDisplayBuf, 0, sizeof(UnicodeDisplayBuf));

	UCS2Strcat((S8 *) UnicodeDisplayBuf,(S8 *) hwModule.BB);
	UCS2Strcat((S8 *) UnicodeDisplayBuf,(S8 *) hwModule.RFPA);
	UCS2Strcat((S8 *) UnicodeDisplayBuf,(S8 *) hwModule.FLSH);
	UCS2Strcat((S8 *) UnicodeDisplayBuf,(S8 *) hwModule.LCD);
	UCS2Strcat((S8 *) UnicodeDisplayBuf,(S8 *) hwModule.CAM);
	UCS2Strcat((S8 *) UnicodeDisplayBuf,(S8 *) hwModule.FM);
	UCS2Strcat((S8 *) UnicodeDisplayBuf,(S8 *) hwModule.BT);
	UCS2Strcat((S8 *) UnicodeDisplayBuf,(S8 *) hwModule.ATV);
   
	ShowCategory74Screen(SSC_SCR_HW_VERSION_CAPTION_ID,
							0, 
							0, 
							0, 
							STR_GLOBAL_BACK,
							0, 
							(PU8) UnicodeDisplayBuf, 
							sizeof(UnicodeDisplayBuf),
							guiBuffer);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
}
#else
void EntryFMHWVersionSummaryMenu(void)
{
    U8*	guiBuffer;
    
	EntryNewScreen(SCR_ID_FM_HW_VER,NULL,EntryFMHWVersionSummaryMenu,NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_FM_HW_VER);

   	memset(UnicodeDisplayBuf, 0, sizeof(UnicodeDisplayBuf));
   
	ShowCategory74Screen(SSC_SCR_HW_VERSION_CAPTION_ID,
							0, 
							0, 
							0, 
							STR_GLOBAL_BACK,
							0, 
							(PU8) UnicodeDisplayBuf, 
							sizeof(UnicodeDisplayBuf),
							guiBuffer);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
}
#endif

void HightlightFMHwModuleNameHdlr(void)
{
	change_left_softkey(STR_GLOBAL_OK,0);
	redraw_left_softkey();
	SetLeftSoftkeyFunction(EntryFMHWVersionSummaryMenu,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
#endif  /* TGT_AUTOGEN_MODULE_NAME */
#endif /* __MMI_FACTORY_MODE__ */

