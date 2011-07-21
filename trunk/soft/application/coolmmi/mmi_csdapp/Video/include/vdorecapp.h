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
 *	  VdoRecApp.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Video Recorder Applcation header file.
 *
 * Author:
 * -------
 *	
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 * 
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef _MMI_VDORECAPP_H
#define _MMI_VDORECAPP_H


/***************************************************************************** 
* Define
*****************************************************************************/
/* Storage file path */
#define VDOREC_STORAGE_FILEPATH_PHONE					FMGR_DEFAULT_FOLDER_VIDEO
#define VDOREC_STORAGE_FILEPATH_MEMORY_CARD			FMGR_DEFAULT_FOLDER_VIDEO
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
#define VDOREC_STORAGE_FILEPATH_MEMORY_SEC_CARD			FMGR_DEFAULT_FOLDER_VIDEO
#endif
/*- zhouqin modify for dual t-flash card 20110503*/

/* rotate */
#define VDOREC_PREVIEW_ROTATE_0 					MDI_VIDEO_PREVIEW_ROTATE_0
#define VDOREC_PREVIEW_ROTATE_90					MDI_VIDEO_PREVIEW_ROTATE_90
#define VDOREC_PREVIEW_ROTATE_180 				MDI_VIDEO_PREVIEW_ROTATE_180
#define VDOREC_PREVIEW_ROTATE_270				MDI_VIDEO_PREVIEW_ROTATE_270
#define VDOREC_PREVIEW_ROTATE_0_MIRROR 		MDI_VIDEO_PREVIEW_ROTATE_0_MIRROR
#define VDOREC_PREVIEW_ROTATE_90_MIRROR 		MDI_VIDEO_PREVIEW_ROTATE_90_MIRROR
#define VDOREC_PREVIEW_ROTATE_180_MIRROR		MDI_VIDEO_PREVIEW_ROTATE_180_MIRROR
#define VDOREC_PREVIEW_ROTATE_270_MIRROR		MDI_VIDEO_PREVIEW_ROTATE_270_MIRROR

/* buffer size */
#define VDOREC_HINT_CHAR_COUNT					(25)
#define VDOREC_HINT_BUF_SIZE						((VDOREC_HINT_CHAR_COUNT+1)*ENCODING_LENGTH)


/***************************************************************************** 
* Typedef 
*****************************************************************************/
/* Camera function settings */
/*********** WB for greenstone ***********/
typedef enum {

	VDOREC_SETTING_WB_SUNNY=0,

	VDOREC_SETTING_WB_AUTO,

	VDOREC_SETTING_WB_OFFICE,

	VDOREC_SETTING_WB_CLOUDY,

	VDOREC_SETTING_WB_ALIGHT,	

	VDOREC_SETTING_WB_COUNT
	
} vdorec_setting_wb_enum;

#if 0
/*********** WB ***********/
typedef enum {

#ifdef __VDOREC_FEATURE_WB_AUTO__
	VDOREC_SETTING_WB_AUTO,
#endif

#ifdef __VDOREC_FEATURE_WB_DAYLIGHT__
	VDOREC_SETTING_WB_DAYLIGHT,
#endif

#ifdef __VDOREC_FEATURE_WB_TUNGSTEN__
	VDOREC_SETTING_WB_TUNGSTEN,
#endif 

#ifdef __VDOREC_FEATURE_WB_FLUORESCENT__
	VDOREC_SETTING_WB_FLUORESCENT,
#endif

#ifdef __VDOREC_FEATURE_WB_CLOUD__
	VDOREC_SETTING_WB_CLOUD,
#endif

#ifdef __VDOREC_FEATURE_WB_INCANDESCENCE__
	VDOREC_SETTING_WB_INCANDESCENCE,
#endif

	VDOREC_SETTING_WB_COUNT
} vdorec_setting_wb_enum;
#endif


/*********** LED Highlight ***********/
typedef enum {

	VDOREC_SETTING_LED_HIGHLIGHT_OFF,

#ifdef __VDOREC_FEATURE_LED_HIGHLIGHT_ON_OFF__
	VDOREC_SETTING_LED_HIGHLIGHT_ON,
#endif 

#ifdef __VDOREC_FEATURE_LED_HIGHLIGHT_7_COLOR__
	VDOREC_SETTING_LED_HIGHLIGHT_WHITE,			
	VDOREC_SETTING_LED_HIGHLIGHT_RED,			
	VDOREC_SETTING_LED_HIGHLIGHT_GREEN,				
	VDOREC_SETTING_LED_HIGHLIGHT_BLUE,					
	VDOREC_SETTING_LED_HIGHLIGHT_YELLOW,						
	VDOREC_SETTING_LED_HIGHLIGHT_PURPLE,							
	VDOREC_SETTING_LED_HIGHLIGHT_CYAN,
#endif

	VDOREC_SETTING_LED_HIGHLIGHT_COUNT
} vdorec_setting_led_highlight_enum;


/*********** EV for greenstone ***********/
typedef enum {

	VDOREC_SETTING_EV_ZERO,
	VDOREC_SETTING_EV_POS_1,
	VDOREC_SETTING_EV_POS_2,	
	VDOREC_SETTING_EV_POS_3,
	VDOREC_SETTING_EV_POS_4,

	VDOREC_SETTING_EV_COUNT

} vdorec_setting_ev_enum;

#if 0
/*********** EV ***********/
typedef enum {

#ifdef __VDOREC_FEATURE_EV_4__
	VDOREC_SETTING_EV_POS_4,
#endif

#ifdef __VDOREC_FEATURE_EV_3__	
	VDOREC_SETTING_EV_POS_3,
#endif
	
#ifdef __VDOREC_FEATURE_EV_2__
	VDOREC_SETTING_EV_POS_2,
#endif

	VDOREC_SETTING_EV_POS_1,
	VDOREC_SETTING_EV_ZERO,
	VDOREC_SETTING_EV_NEG_1,

#ifdef __VDOREC_FEATURE_EV_2__
	VDOREC_SETTING_EV_NEG_2,
#endif

#ifdef __VDOREC_FEATURE_EV_3__
	VDOREC_SETTING_EV_NEG_3,
#endif

#ifdef __VDOREC_FEATURE_EV_4__
	VDOREC_SETTING_EV_NEG_4,
#endif

	VDOREC_SETTING_EV_COUNT

} vdorec_setting_ev_enum;
#endif

//add by zhangl for greenstone
typedef enum {
	VDOREC_SETTING_BRIGHT_1 = 0,
	VDOREC_SETTING_BRIGHT_2,
	VDOREC_SETTING_BRIGHT_3,
	VDOREC_SETTING_BRIGHT_4,
	VDOREC_SETTING_BRIGHT_5,
	VDOREC_SETTING_BT_COUNT
}vdorec_setting_brightness_enum;


typedef enum {
	VDOREC_SETTING_CONTRAST_1 = 0,
	VDOREC_SETTING_CONTRAST_2,
	VDOREC_SETTING_CONTRAST_3,
	VDOREC_SETTING_CONTRAST_4,
	VDOREC_SETTING_CONTRAST_5,
	VDOREC_SETTING_CONTRAST_COUNT
}vdorec_setting_contrast_enum;

/*********** Night ***********/
typedef enum {
	VDOREC_SETTING_NIGHT_OFF = 0,
	VDOREC_SETTING_NIGHT_ON,
	VDOREC_SETTING_NIGHT_COUNT
} vdorec_setting_night_enum;



/*********** Banding ***********/
typedef enum {
	VDOREC_SETTING_BANDING_50HZ = 0, 
	VDOREC_SETTING_BANDING_60HZ,
	VDOREC_SETTING_BANDING_COUNT
} vdorec_setting_banding_enum;



#if defined(MT6228) || defined(MT6229)
	 

	/*********** Video Size ***********/
	typedef enum {

		#ifdef __VDOREC_FEATURE_VIDEO_SIZE_SQCIF__
			VDOREC_SETTING_VIDEO_SIZE_SQCIF, 
		#endif /* __VDOREC_FEATURE_VIDEO_SIZE_SQCIF__ */

		#ifdef __VDOREC_FEATURE_VIDEO_SIZE_QQVGA__
			VDOREC_SETTING_VIDEO_SIZE_QQVGA,
		#endif /* __VDOREC_FEATURE_VIDEO_SIZE_QQVGA__ */

		#ifdef __VDOREC_FEATURE_VIDEO_SIZE_QCIF__
			VDOREC_SETTING_VIDEO_SIZE_QCIF,
		#endif /* __VDOREC_FEATURE_VIDEO_SIZE_QCIF__ */

		#ifdef __VDOREC_FEATURE_VIDEO_SIZE_CIF__
			VDOREC_SETTING_VIDEO_SIZE_CIF,
		#endif /* __VDOREC_FEATURE_VIDEO_SIZE_CIF__ */

		VDOREC_SETTING_VIDEO_SIZE_COUNT
	} vdorec_setting_video_size_enum;
#else
	 
	/*********** Video Size ***********/
	typedef enum {
		VDOREC_SETTING_VIDEO_SIZE_SQCIF,//added by zhangl for vidrec 11-18
		VDOREC_SETTING_VIDEO_SIZE_QCIF,
		VDOREC_SETTING_VIDEO_SIZE_COUNT
	} vdorec_setting_video_size_enum;
#endif


/*********** Video Qty ***********/
typedef enum {

	#ifdef __VDOREC_FEATURE_VIDEO_QTY_FINE__
		VDOREC_SETTING_VIDEO_QTY_FINE, 
	#endif /* __VDOREC_FEATURE_VIDEO_QTY_FINE__ */

	#ifdef __VDOREC_FEATURE_VIDEO_QTY_HIGH__
		VDOREC_SETTING_VIDEO_QTY_HIGH,
	#endif /* __VDOREC_FEATURE_VIDEO_QTY_HIGH__ */

	#ifdef __VDOREC_FEATURE_VIDEO_QTY_NORMAL__
		VDOREC_SETTING_VIDEO_QTY_NORMAL,
	#endif /* __VDOREC_FEATURE_VIDEO_QTY_NORMAL__ */

	#ifdef __VDOREC_FEATURE_VIDEO_QTY_LOW__
		VDOREC_SETTING_VIDEO_QTY_LOW,
	#endif /* __VDOREC_FEATURE_VIDEO_QTY_LOW__ */

	VDOREC_SETTING_VIDEO_QTY_COUNT
} vdorec_setting_video_qty_enum;



/*********** Size Limit ***********/
typedef enum {
	VDOREC_SETTING_SIZE_LIMIT_NO_LIMIT,
	VDOREC_SETTING_SIZE_LIMIT_1,
	VDOREC_SETTING_SIZE_LIMIT_2,
	VDOREC_SETTING_SIZE_LIMIT_3,

	VDOREC_SETTING_SIZE_LIMIT_COUNT
} vdorec_setting_size_limit_enum;



/*********** TIME Limit ***********/
typedef enum {
//	VDOREC_SETTING_TIME_LIMIT_NO_LIMIT,
	VDOREC_SETTING_TIME_LIMIT_1,
	VDOREC_SETTING_TIME_LIMIT_2,
	VDOREC_SETTING_TIME_LIMIT_3,

	VDOREC_SETTING_TIME_LIMIT_COUNT
} vdorec_setting_time_limit_enum;


/*********** Record Aud ***********/
typedef enum {
	VDOREC_SETTING_RECORD_AUD_OFF,
	VDOREC_SETTING_RECORD_AUD_ON,

	VDOREC_SETTING_RECORD_AUD_COUNT
} vdorec_setting_record_aud_enum;


/*********** Video Format ***********/
typedef enum {
	VDOREC_SETTING_VIDEO_FORMAT_3GP,
	VDOREC_SETTING_VIDEO_FORMAT_MP4,

	VDOREC_SETTING_VIDEO_FORMAT_COUNT
} vdorec_setting_video_format_enum;


/*********** Effect ***********/
typedef enum {
#ifdef __VDOREC_FEATURE_EFFECT_NORMAL__
	VDOREC_SETTING_EFFECT_NORMAL=0,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_GRAYSCALE__
	VDOREC_SETTING_EFFECT_GRAYSCALE,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_SEPIA__
	VDOREC_SETTING_EFFECT_SEPIA,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_SEPIA_GREEN__
	VDOREC_SETTING_EFFECT_SEPIA_GREEN,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_SEPIA_BLUE__
	VDOREC_SETTING_EFFECT_SEPIA_BLUE,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_COLOR_INVERT__
	VDOREC_SETTING_EFFECT_COLOR_INVERT,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_GRAY_INVERT__
	VDOREC_SETTING_EFFECT_GRAY_INVERT,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_BLACKBOARD__
	VDOREC_SETTING_EFFECT_BLACKBOARD,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_WHITEBOARD__
	VDOREC_SETTING_EFFECT_WHITEBOARD,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_COPPER_CARVING__
	VDOREC_SETTING_EFFECT_COPPER_CARVING,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_BLUE_CARVING__
	VDOREC_SETTING_EFFECT_BLUE_CARVING,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_EMBOSSMENT__
	VDOREC_SETTING_EFFECT_EMBOSSMENT,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_CONTRAST__
	VDOREC_SETTING_EFFECT_CONTRAST,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_JEAN__
	VDOREC_SETTING_EFFECT_JEAN,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_SKETCH__
	VDOREC_SETTING_EFFECT_SKETCH,
#endif

#ifdef __VDOREC_FEATURE_EFFECT_OIL__
	VDOREC_SETTING_EFFECT_OIL,
#endif

/////added  by zhangl
#ifdef  __VDOREC_FEATURE_EFFECT_ANTIQUE__
     VDOREC_SETTING_EFFECT_ANTIQUE,
#endif

#ifdef  __VDOREC_FEATURE_EFFECT_REDRISH__
    VDOREC_SETTING_EFFECT_REDRISH,
#endif

#ifdef  __VDOREC_FEATURE_EFFECT_GREENRISH__
    VDOREC_SETTING_EFFECT_GREENRISH,
#endif

#ifdef  __VDOREC_FEATURE_EFFECT_BLUERISH__
    VDOREC_SETTING_EFFECT_BLUERISH,
#endif


#ifdef __VDOREC_FEATURE_EFFECT_BLACKWHITE__
     VDOREC_SETTING_EFFECT_BLACKWHITE,
#endif


#ifdef  __VDOREC_FEATURE_EFFECT_NEGATIVE__
    VDOREC_SETTING_EFFECT_NEGATIVENESS,
#endif


	VDOREC_SETTING_EFFECT_COUNT
} vdorec_setting_effect_enum;

/*********** Storage ***********/
typedef enum {
	VDOREC_SETTING_STORAGE_PHONE = 0,
	VDOREC_SETTING_STORAGE_MEMORY_CARD,
	/*+ zhouqin modify for dual t-flash card 20110503*/
    #if defined(DUAL_TFLASH_SUPPORT)
    VDOREC_SETTING_STORAGE_MEMORY_SEC_CARD,
    #endif
    /*- zhouqin modify for dual t-flash card 20110503*/
	VDOREC_SETTING_STORAGE_COUNT
} vdorec_setting_storage_enum;



typedef enum {
	VDOREC_STATE_EXIT,
	VDOREC_STATE_PREVIEW,
	VDOREC_STATE_RECORD,
	VDOREC_STATE_PAUSE
} vdorec_state_neum;



/***************************************************************************** 
* Structure
*****************************************************************************/

/* NVRAM ITEM SIZE: 51 byte */
typedef struct {

	/* preview - 18 byte */
	U16	wb;
	U16	ev;	
	U16	led_highlight;
	U16	night;
	U16	banding;
	U16	effect;	
	U16 	brightness;
	U16	contrast;
	U16	zoom;

 	/* video setting - 10 byte */ 
	U16	video_qty;
	U16	size_limit;
	U16	time_limit;
	U16	record_aud;
	U16	video_format;
	
	/* others - 2 byte */
	U16	storage;

	/* exstention */	
	U16	video_size;
	
	/* reserve - 18 byte */	
	U16	reserve1;
	U16	reserve2;
	U16	reserve3;
	U16	reserve4;
	U16	reserve5;
	U16	reserve6;
	U16	reserve7;
	U16	reserve8;
	U16	reserve9;

	/* LED - 1 byte */
	U8		led_value;

} vdorec_setting_struct;


typedef struct {
	S32 	offset_y;
	S32 	hr_0_offset_x;
	S32 	hr_1_offset_x;
	S32 	col_0_offset_x;
	S32 	min_0_offset_x;
	S32 	min_1_offset_x;
	S32 	col_1_offset_x;
	S32 	sec_0_offset_x;
	S32 	sec_1_offset_x;
	U16	prev_hr;
	U16	prev_min;	
	U16	prev_sec;		
	
} vdorec_timer_struct;

typedef struct{

	/* strings for inline selection display */
	UI_string_type wb_str[VDOREC_WB_ITEM_COUNT];
	UI_string_type led_highlight_str[VDOREC_LED_HIGHLIGHT_ITEM_COUNT];	
	UI_string_type ev_str[VDOREC_EV_ITEM_COUNT];
	UI_string_type bright_str[6];  //add by ZhangL
	UI_string_type contrast_str[6];   //add by ZhangL
	UI_string_type night_str[VDOREC_NIGHT_ITEM_COUNT];
	UI_string_type banding_str[VDOREC_BANDING_ITEM_COUNT];

	#ifdef __VDOREC_FEATURE_MERGE_MENU__
		UI_string_type video_size_str[VDOREC_VIDEO_QTY_ITEM_COUNT];	
		UI_string_type video_qty_str[VDOREC_VIDEO_QTY_ITEM_COUNT];
		UI_string_type size_limit_str[VDOREC_SIZE_LIMIT_ITEM_COUNT];	
		UI_string_type time_limit_str[VDOREC_TIME_LIMIT_ITEM_COUNT];	
		UI_string_type record_aud_str[VDOREC_RECORD_AUD_ITEM_COUNT];	
		UI_string_type video_format_str[VDOREC_VIDEO_FORMAT_ITEM_COUNT];	
	#endif /* __VDOREC_FEATURE_MERGE_MENU__ */
	
	/* selected item */
	S32 bright_selected; //add by ZhangL
	S32 contrast_selected; //add by ZhangL
	S32 wb_selected;
	S32 led_highlight_selected;
	S32 ev_selected;
	S32 night_selected;
	S32 banding_selected;

	#ifdef __VDOREC_FEATURE_MERGE_MENU__
		S32 video_size_selected;	
		S32 video_qty_selected;
		S32 size_limit_selected;
		S32 time_limit_selected;
		S32 record_aud_selected;
		S32 video_format_selected;
	#endif /* __VDOREC_FEATURE_MERGE_MENU__ */
	
} vdorec_camcoder_setting_inline_data_struct;



#ifndef __VDOREC_FEATURE_MERGE_MENU__
typedef struct{

		/* strings for inline selection display */
		UI_string_type video_size_str[VDOREC_VIDEO_QTY_ITEM_COUNT];		
		UI_string_type video_qty_str[VDOREC_VIDEO_QTY_ITEM_COUNT];
		UI_string_type size_limit_str[VDOREC_SIZE_LIMIT_ITEM_COUNT];	
		UI_string_type time_limit_str[VDOREC_TIME_LIMIT_ITEM_COUNT];	
		UI_string_type record_aud_str[VDOREC_RECORD_AUD_ITEM_COUNT];	
		UI_string_type video_format_str[VDOREC_VIDEO_FORMAT_ITEM_COUNT];	
		
		/* selected item */
		S32 video_size_selected;		
		S32 video_qty_selected;
		S32 size_limit_selected;
		S32 time_limit_selected;
		S32 record_aud_selected;
		S32 video_format_selected;
		
} vdorec_video_setting_inline_data_struct;
#endif /* !__VDOREC_FEATURE_MERGE_MENU__ */

typedef struct {
	S32	offset_x;
	S32	offset_y;
	S32	width;
	S32	height;
	BOOL	is_press;
} vdorec_ui_touch_struct;


typedef enum{
	VDOREC_TOUCH_EV_INC,
	VDOREC_TOUCH_EV_DEC,
	VDOREC_TOUCH_ZOOM_INC,
	VDOREC_TOUCH_ZOOM_DEC,
	VDOREC_TOUCH_REC,
	VDOREC_TOUCH_STOP,
	VDOREC_TOUCH_RSK,
	VDOREC_TOUCH_LSK,	
	VDOREC_TOUCH_NONE
} vdorec_touch_object_enum;


/* context */
typedef struct {	

	gdi_handle		base_layer_handle;
	gdi_handle		preview_layer_handle;
	gdi_handle		osd_layer_handle;

	U64				record_time;					/* in ms */
	U16				state;
	U16				filename_seq_no;
	PU8				osd_layer_buf_ptr;
	PS8				lsk_str_ptr;
	PS8				rsk_str_ptr;
	U16				lsk_img_id;
	U16				rsk_img_id;	
	S32				lcd_width;
	S32				lcd_height;
	BOOL				is_lsk_pressed;
	BOOL				is_rsk_pressed;
	BOOL				is_storage_path_changed;
	BOOL				is_saving;
	
	BOOL				is_zoom_in_key_pressed;
	BOOL				is_zoom_out_key_pressed;	
	BOOL				is_ev_inc_key_pressed;
	BOOL				is_ev_dec_key_pressed;	
	BOOL				is_bt_inc_key_pressed;
	BOOL				is_bt_dec_key_pressed;	
	BOOL				is_ct_inc_key_pressed;
	BOOL				is_ct_dec_key_pressed;	
	
	BOOL				is_app_popup;
	BOOL				is_sub_display;
	BOOL				is_self_record;
	BOOL				is_force_self_record;
	BOOL				is_setting_loaded;
	BOOL				is_force_saving;
	BOOL				is_first_time_zoom;
	PS8				storage_filepath;

	S8					hint1_buf[VDOREC_HINT_BUF_SIZE];
	S8					hint2_buf[VDOREC_HINT_BUF_SIZE];
	U16				fade_time;

	MDI_RESULT		last_error;

	vdorec_setting_struct		setting;
	vdorec_timer_struct			timer;

	#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__
		U16								touch_object;
		
		vdorec_ui_touch_struct		touch_ev_inc;
		vdorec_ui_touch_struct		touch_ev_dec;
		vdorec_ui_touch_struct		touch_zoom_inc;
		vdorec_ui_touch_struct		touch_zoom_dec;	
		vdorec_ui_touch_struct		touch_rec;
		vdorec_ui_touch_struct		touch_stop;
		vdorec_ui_touch_struct		touch_lsk;		
		vdorec_ui_touch_struct		touch_rsk;
	#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */

	
} vdorec_context_struct;

typedef struct {
	vdorec_camcoder_setting_inline_data_struct	camcoder_setting_inline;
#ifndef __VDOREC_FEATURE_MERGE_MENU__
	vdorec_video_setting_inline_data_struct		video_setting_inline;
#endif /* !__VDOREC_FEATURE_MERGE_MENU__ */
} vdorec_menu_context_struct;

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/
extern vdorec_context_struct 			g_vdorec_cntx;
extern vdorec_menu_context_struct	g_vdorec_menu_cntx;


#endif /* _MMI_VDORECAPP_H */


