/*****************************************************************************
*  Copyright Statement:
*  --------------------
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *  analogtvapp.h
 *
 * Project:
 * --------
 *  
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 * -------
 *                      
 *============================================================================
 *             HISTORY
 * 
 *============================================================================
 ****************************************************************************/
#ifndef _ANALOGTV_DATATYPE_H_
#define _ANALOGTV_DATATYPE_H_
#include "mmi_features.h"

#if defined(__MMI_ANALOGTV__)
#include "kal_non_specific_general_types.h"
#include "mmi_data_types.h"
#include "gdi_datatype.h"
#include "touchscreengprot.h"

#define __ANALOGTV_DEBUG__
//#define __MMI_ANALOGTV_SNAPSHOT__

/*****define *******/
#define ANALOGTV_OSD_BUFFER_SIZE			(((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3)*2)

#define ANALOGTV_ENTER_STATE(_STATE_)          {                                            \
                                                   g_analogtv_cntx.app_next_state = _STATE_;\
                                                   mmi_analogtv_exit_current_state();       \
                                                   mmi_analogtv_enter_state(_STATE_);       \
                                             }
#define MAX_ANALOGTV_CHANNEL_NUMBER				80
#define MAX_USER_CHANNEL_NUMBER 10
#define STANDARD_YUV_VALUE_Y				4
#define STANDARD_YUV_VALUE_U				0
#define STANDARD_YUV_VALUE_V				195

#define ANALOGTV_AUTO_SEARCH_TIME_DELAY		500

#define ANALOGTV_CHANNEL_DEFAULT					38
#define MAX_VOL_LEVEL 7  
#define MAX_CH_NAME_LEN 10

typedef enum
{
	ANALOGTV_ERROR_SUCCESS,
	ANALOGTV_ERROR_PLAY,
	ANALOGTV_ERROR_RECORD,
	ANALOGTV_ERROR_SANP,
	ANALOGTV_ERROR_DISK_FULL,
	ANALOGTV_ERROR_DISK,
	ANALOGTV_ERR_CORRUPT_FAT,
	ANALOGTV_ERR_NO_DISK,
	ANALOGTV_ERR_WRITE_PROTECT,
	ANALOGTV_ERR_ROOT_DIR_FULL,
	ANALOGTV_MAX_ERROR
} analogtv_error_enum;

typedef enum
{
	ANALOGTV_STORAGE_PHONE = 67,
	ANALOGTV_STORAGE_CARD = 69,
	ANALOGTV_STORAGE_MAX
} analogtv_storage_enum;

typedef enum
{
	ANALOGTV_SEARCH_AREA_CHINA,
	ANALOGTV_SEARCH_AREA_SHENZHEN,
	ANALOGTV_SEARCH_AREA_HONGKONG,
	ANALOGTV_SEARCH_AREA_ARGENTINA,
	ANALOGTV_SEARCH_AREA_AMERICA,
	ANALOGTV_SEARCH_AREA_BRAZIL,
	ANALOGTV_SEARCH_AREA_EUROPE,
	ANALOGTV_SEARCH_AREA_JAPAN,
	ANALOGTV_SEARCH_AREA_VIETNAM,
	ANALOGTV_SEARCH_AREA_MAX
} analogtv_search_area;

typedef enum
{
	ANALOGTV_FILE_TYPE_JPG,
	ANALOGTV_FILE_TYPE_3GP,
	ANALOGTV_FILE_TYPE_MAX
} analogtv_file_type_enum;

/********************************************************************************/
/* Setting are value will be store in nvram */
/* NVRAM ITEM SIZE: 75 byte */
typedef struct
{
	BOOL	Present;	
	U8	name[(MAX_CH_NAME_LEN+1)*ENCODING_LENGTH];							// User of a Data Account
	U16	frequency;	
}analogtv_channel_struct;

typedef struct
{
	S16 cur_index;
	analogtv_channel_struct channel;
}channel_context_struct;

typedef enum
{
	WEAK_LEVEL,
	NORMAL_LEVEL,		
	HIGH_LEVEL,
	MAX_YUV	
}yuv_enum;

typedef struct
{
	U8	volume; 		// 1 byte
	U8	mute; 			// 1 byte
	U8	storage;	
	U8	total_channel; // 1 byte
	
	U8   total_mychannel;
	BOOL is_mychannel;
	S8 yuv_brightness;
	yuv_enum yuv_color;
	U8 	filename_seq_no;
	channel_context_struct cur_mychannel;
	channel_context_struct cur_channel;	// 1 byte    
	analogtv_channel_struct mychannel[MAX_USER_CHANNEL_NUMBER];
} analogtv_setting_struct;
/********************************************************************************/
typedef struct
{
    S32	offset_x;
    S32	offset_y;
    S32	width;
    S32	height;
    BOOL	is_press;
} analogtv_ui_touch_struct;

/* AnalogTV application state */
typedef enum
{
    ANALOGTV_STATE_EXIT = 0,
    ANALOGTV_STATE_PREVIEW,                 
	ANALOGTV_STATE_RECORD, 
	ANALOGTV_STATE_PAUSE,
	ANALOGTV_STATE_PREVIEW_CHANNEL_AUTO_SEARCH,
	ANALOGTV_STATE_FULL_SCR_PLAY,
	ANALOGTV_STATE_SEARCH_CHANNEL,  
	ANALOGTV_STATE_SNAPSHOT, 
	ANALOGTV_STATE_IDLE   
} analogtv_state_enum;

typedef enum {
	ATV_PLAYER_SETTING_FIRST_USE_ID,// 0
	ATV_PLAYER_SETTING_VOLUME_ID,
	ATV_PLAYER_SETTING_TOTAL_CHANNEL_ID,
	ATV_PLAYER_SETTING_LAST_CHANNEL_ID,
	ATV_PLAYER_SETTING_STORAGE_ID,
	ATV_PLAYER_SETTING_MUTE_ID,//
	ATV_PLAYER_SETTING_FILENAME_SEQ_ID,
	ATV_PLAYER_SETTING_SEARCH_AREA_ID,
	/* don not remove last element */	
	ATV_PLAYER_SETTING_LAST_BYTE_ELEMENT
} analogtv_setting_enum;
typedef struct
{
    U16 video_qty;
    U16 size_limit;
    U16 time_limit;
    U16 record_aud;
    U16 video_format;
    U16 video_size;
} vdorec_setting_struct_TV;

typedef struct
{
    S32 offset_y;
    S32 hr_0_offset_x;
    S32 hr_1_offset_x;
    S32 col_0_offset_x;
    S32 min_0_offset_x;
    S32 min_1_offset_x;
    S32 col_1_offset_x;
    S32 sec_0_offset_x;
    S32 sec_1_offset_x;
    U16 prev_hr;
    U16 prev_min;
    U16 prev_sec;

} vdorec_timer_struct_TV;

typedef struct
{
	PS8 lsk_str_ptr;
	PS8 rsk_str_ptr;

	U16 lsk_img_id;
	U16 rsk_img_id;
	U16 ck_img_id;	

	S32 lcd_width;
	S32 lcd_height;
	S32 last_error;
	S32 save_result;
	S16	error_code;
	U8 osd_rotate;
	U8 previous_osd_rotate;
	U8 lcm_rotate;

	BOOL is_ck_pressed;
	BOOL is_change_storage;
	BOOL is_option_storage;
	BOOL is_saving;
	BOOL is_foward_to;	
	BOOL is_save_done_when_mt;
	BOOL is_direct_exit;
	BOOL prev_rotate_by_layer;
	BOOL is_lsk_pressed;
	BOOL is_rsk_pressed;
	BOOL is_up_arrow_pressed;
	BOOL is_down_arrow_pressed;   
	BOOL is_clam_close;   
	BOOL is_mainlcd_resource;    
	BOOL is_fullscreen;    
	BOOL is_hide_osd; 
	BOOL is_3d_sound_on;

	U64 record_time;
	U16 state;
	U16 filename_seq_no;
	U16 index;
	U16 index2;
	analogtv_channel_struct channel;
	analogtv_channel_struct channel_list[32];  //76 bytes  

	analogtv_state_enum app_state;
	analogtv_state_enum app_prev_state;
	analogtv_state_enum app_next_state;

	analogtv_state_enum set_state;
	
	analogtv_setting_struct analogtv_setting;
	
	vdorec_setting_struct_TV setting;
	vdorec_timer_struct_TV timer;
	PS8 storage_filepath;
	S8 hint1_buf[52];
	S8 hint2_buf[52];
	U16 fade_time;
	void (*ext_req_callback)(MMI_BOOL result, PS8 filepath);
	MMI_BOOL is_ext_req;
	MMI_BOOL is_use_ext_req_path;
	
	U8 volume;
	U8 searched_area_id;
	U8 searching_area_id;
	U16 current_area_chn_count;
	U16 touch_object;
	analogtv_ui_touch_struct touch_lsk;
	analogtv_ui_touch_struct touch_rsk;
	analogtv_ui_touch_struct touch_ck;
} analogtv_context_struct;

/********************************************************************************/
/********************************************************************************/
/* analogtv contex */
extern analogtv_context_struct g_analogtv_cntx;

/***************************************************************************** 
* Typedef 
*****************************************************************************/

/****************************************************************************
* Global Variable - Extern                                                                 
*****************************************************************************/
extern BOOL r2lMMIFlag;
/****************************************************************************
* Global Function                                                           
*****************************************************************************/
extern void AudioStopReq(U16 soundId);
extern U16 GetCurKeypadTone(void);

#ifdef __MMI_TOUCH_SCREEN__
MMI_BOOL wgui_test_lsk_position(mmi_pen_point_struct pos);
#endif 

/***************************************************************************** 
* Local Variable
*****************************************************************************/
/* analogtv contex */
/*****************************************************************************
* Local Function 
*****************************************************************************/

/********************************************************************************/
/********************************************************************************/
/****************************************************************************
* Extern Global Function                                                            
*****************************************************************************/
void mmi_analogtv_init_app(void);
void mmi_analogtv_load_setting(void);
void mmi_analogtv_save_setting(void);
extern void mmi_analogtv_entry_app(void);
void mmi_analogtv_entry_preview_screen(void);
void highlight_mainmenu_analogtv(void);
void mmi_analogtv_hilight_app(void);
extern void InitAnalogTV(void);
void mmi_analogtv_entry_Search(void);
void mmi_analogtv_entry_Search_Auto(void);
void mmi_analogtv_entry_Search_Manual(void);
void mmi_analogtv_entry_Select(void);
void mmi_analogtv_entry_Select_List(void);
void mmi_analogtv_entry_Select_User_Channel(void);
void mmi_analogtv_entry_Select_List_Play(void);
void mmi_analogtv_entry_Select_List_Rename(void);
void mmi_analogtv_entry_Select_List_Add(void);
void mmi_analogtv_entry_Select_User_Play(void);
void mmi_analogtv_entry_Select_User_Rename(void);
void mmi_analogtv_entry_Add(void);
void mmi_analogtv_entry_Search_Area(void);
void mmi_analogtv_entry_Search_Area_China(void);
void mmi_analogtv_entry_Search_Area_Europe(void);
void mmi_analogtv_entry_Effect(void);
void mmi_analogtv_entry_Effect_Brightness(void);
void mmi_analogtv_entry_Effect_Color_Depth(void);
void mmi_analogtv_entry_Recorder(void);
void mmi_analogtv_preview_show_controlpannel(void);
void mmi_analogtv_entry_option_screen(void);
void mmi_analogtv_preview_show_pannel_background(void);
void mmi_analogtv_preview_show_button(void);
void mmi_analogtv_blt_screen(void);
void channel_next_button_up_handle(void);
void channel_pre_button_up_handle(void);
void channel_next_button_down_handle(void);
void channel_pre_button_down_handle(void);
void volume_dec_button_down_handle(void);
void volume_inc_button_down_handle(void);
void volume_dec_button_up_handle(void);
void volume_inc_button_up_handle(void);
BOOL mmi_analogtv_reset_all_nvram_list(void); 
void mmi_analogtv_display_pop_error(S16 error_code);
void mmi_analogtv_entry_channel_auto_search_state(void);
void mmi_analogtv_preview_show_autosearch_pannel(void);
void mmi_analogtv_preview_auto_search_set_key_hdlr(void);
void mmi_analogtv_exit_preview_auto_search_screen(void);
void mmi_analogtv_entry_channel_list_option(void);
void mmi_mainmenu_analogtv(void);

void channel_pre_button_down_handle(void);

void mmi_analogtv_set_search_area(void);
void mmi_analogtv_entry_delete_channel_confirm( void );
void mmi_analogtv_entry_Select_List_Delete(void);


extern void EntryATVEngineerModePlayScn(void);

#endif /* ANALOG_TV_SUPPORT */ 
#endif /* _ANALOGTV_DATATYPE_H_ */ 

