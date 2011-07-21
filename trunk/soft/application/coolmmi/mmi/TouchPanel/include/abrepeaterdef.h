/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2002
*
*******************************************************************************/

/*******************************************************************************
* Filename:
* ---------
*	ABRepeater.h
*
* Project:
* --------
 
*
* Description:
* ------------
*   AB Repeater main screen
*
* Author:
* -------
 
*
********************************************************************************************/

#ifndef AB_REPEATER_DEF_H
#define AB_REPEATER_DEF_H

#ifdef __MMI_AB_REPEATER__
//#define SIMULATOR_TESTING

typedef enum {
	STR_ID_ABREPEAT_BASE = AB_REPEATER_BASE+1,
	STR_ID_ABREPEAT_TITLE,
	STR_ID_ABENTER_TITLE,
	STR_ID_ABREPEAT_STORAGE,
	STR_ID_ABREPEAT_SWITCH,
	STR_ID_ABREPEAT_CANNOT_PAUSE,
	STR_ID_ABREPEAT_COMMAND_ERROR,
	STR_ID_ABREPEAT_DEVICE_BUSY,
	STR_ID_ABREPEAT_DISC_FULL,
	STR_ID_ABREPEAT_OPEN_FILE_ERROR,
	STR_ID_ABREPEAT_BAD_FORMAT,
	STR_ID_ABREPEAT_INVALID_FORMAT,
	STR_ID_ABREPEAT_UNSUPPORTED_CHANNEL,
	STR_ID_ABREPEAT_UNSUPPORTED_FREQ,
	STR_ID_ABREPEAT_UNSUPPORTED_TYPE,
	STR_ID_ABREPEAT_ERROR,
	STR_ID_ABREPEAT_NO_SPACE,
	STR_ID_ABREPEAT_STORAGE_PHONE,
	STR_ID_ABREPEAT_STORAGE_CARD,
	STR_ID_ABREPEAT_NO_CARD,
	NO_OF_STR_ID_ABREPEAT
} abrepeat_str_id_enum;


typedef enum {
	IMG_ID_ABREPEAT_BASE = AB_REPEATER_BASE+1,
	IMG_ID_ABREPEAT_MAIN_SKIN1_BG,
	IMG_ID_ABREPEAT_PROGRESS_BAR,
	IMG_ID_ABREPEAT_INDICATOR,
	IMG_ID_ABREPEAT_BUTTON0,
	IMG_ID_ABREPEAT_BUTTON1,
	IMG_ID_ABREPEAT_BUTTON2,
	IMG_ID_ABREPEAT_BUTTON3,
	IMG_ID_ABREPEAT_BUTTON4,
	IMG_ID_ABREPEAT_BUTTON5,
	IMG_ID_ABREPEAT_BUTTON6,
	IMG_ID_ABREPEAT_BUTTON7,
	IMG_ID_ABREPEAT_BUTTON8,
	IMG_ID_ABREPEAT_BUTTON9,
	IMG_ID_ABREPEAT_ABSECTION_PLAYING,
	IMG_ID_ABREPEAT_ABSECTION_RECORDING,
	IMG_ID_ABREPEAT_ABSECTION_RECORDING_DOWN,
	IMG_ID_ABREPEAT_AB_SETSECTION,
	IMG_ID_ABREPEAT_AB_SETSECTION_DOWN,
	IMG_ID_ABREPEAT_AB_NONSETSECTION,
	IMG_ID_ABREPEAT_AB_NONSETSECTION_DOWN,
	IMG_ID_ABREPEAT_VOLUME,
	IMG_ID_ABREPEAT_STOP_DOWN,
	IMG_ID_ABREPEAT_STOP_UP,
	IMG_ID_ABREPEAT_PROG_BAR_EMPTY,
	IMG_ID_ABREPEAT_PROG_BAR_FILL,
	IMG_ID_ABREPEAT_FWD_DOWN,
	IMG_ID_ABREPEAT_FWD_UP,
	IMG_ID_ABREPEAT_REW_DOWN,
	IMG_ID_ABREPEAT_REW_UP,
	IMG_ID_ABREPEAT_PAUSE_DOWN,
	IMG_ID_ABREPEAT_PAUSE_UP,
	IMG_ID_ABREPEAT_PLAY_DOWN,
	IMG_ID_ABREPEAT_PLAY_UP,
	IMG_ID_ABREPEAT_REPEAT_DOWN,
	IMG_ID_ABREPEAT_REPEAT_UP,
	IMG_ID_ABREPEAT_REPEAT_OFF_DOWN,
	IMG_ID_ABREPEAT_REPEAT_OFF_UP,
	IMG_ID_ABREPEAT_REPEAT_DISABLE,
	IMG_ID_ABREPEAT_SUBMENU_ICON,
	IMG_ID_ABREPEAT_STORAGE,

	NO_OF_AB_REPEATER_ICON
} abrepeat_img_id_enum;

typedef enum {
	SCR_ID_ABREPEAT_BASE = AB_REPEATER_BASE+1,
	SCR_ID_AB_LIST,
	SCR_ID_AB_MAIN,
	SCR_ID_ABREPEAT_OPTION,
	SCR_ID_ABREPEAT_STORAGE_SETTINGS,

	NO_OF_SCR_ABREPEAT
} abrepeat_scr_id_enum;

typedef enum {
	ABREPEAT_MAIN_DISABLED = -1,
	ABREPEAT_MAIN_PLAY = 0,
	ABREPEAT_MAIN_REWIND,
	ABREPEAT_MAIN_FORWARD,
	ABREPEAT_MAIN_STOP,
	ABREPEAT_MAIN_PAUSE,
	NO_OF_ABREPEAT_MAIN_AUDIO_BUTTONS
} abrepeat_main_buttons_enum;

typedef enum {

	AB_SECTION0=0,
	AB_SECTION1,
	AB_SECTION2,
	AB_SECTION3,
	AB_SECTION4,
	AB_SECTION5,
	AB_SECTION6,
	AB_SECTION7,
	AB_SECTION8,
	AB_SECTION9,
	MAX_AB_POINT
}abrepeat_absection_enum;

#ifndef MAX_VOL_LEVEL
#define MAX_VOL_LEVEL 7
#endif
#define MAX_AB_SETTING_SCREEN_DIGITS		13
//#define MAX_AB_STORAGE_PATH_LEN	 20
 
#define MAX_AB_STORAGE_PATH_LEN	 FMGR_MAX_PATH_LEN
#define AB_STORAGE_PHONE 0
#define AB_STORAGE_CARD 1
typedef enum
{
	AB_STATE_IDLE,
	AB_STATE_PLAY,
	AB_STATE_PAUSED,
	AB_NO_OF_STATE
} abrepeat_state_enum;

 
typedef enum
{
	NOTHING=0,
	PLAY_COMPLETED,
	PLAY_STOPPED
}abrepeat_temp_status_enum;


typedef enum
{
	AB_SECTION_DISABLED = -1,
	AB_SECTION_FIRST_SET,
	AB_SECTION_COMPLETE_SET,
	AB_SECTION_NON_PLAYING,
	AB_SECTION_PLAYING
}abrepeat_absection_draw_state;

typedef enum
{
	LIST_FROM_PHONE_AB,
	LIST_FROM_CARD_AB,
	NO_OF_LIST_AB
} abrepeat_playlist_enum;

typedef enum
{
	AB_SET_IDLE=0,
	AB_SET_PARTIAL
}abrepeat_abset_state;

typedef struct
{
	S16 ab_section_x,ab_section_y;
	U16 ab_section_button_img_id;
}abrepeat_absection_button_skin_struct;

typedef struct
{
	U16 main_bg_img_id;
	U8 text_ab_border_color_r, text_ab_border_color_g, text_ab_border_color_b;
	U8 text_main_fg_color_r, text_main_fg_color_g, text_main_fg_color_b;
	U8 text_main_bg_color_r, text_main_bg_color_g, text_main_bg_color_b;
	U8 audio_bg_color_r, audio_bg_color_g, audio_bg_color_b;
	U8 absection_color_r,absection_color_g,absection_color_b;
	S16 title_ab_x, title_ab_y;
	S16 title_ab_w, title_ab_h;
	S16 volumebar_ab_x, volumebar_ab_y, volumebar_ab_ind_left;
	U16 volumebar_ab_ind_image_id;
	S16 rewind_button_x, rewind_button_y;
	U16 rewind_button_up_image_id, rewind_button_down_image_id;

	S16 forward_button_x, forward_button_y;
	U16 forward_button_up_image_id, forward_button_down_image_id;

	S16 play_ab_button_x, play_ab_button_y;
	U16 play_ab_button_up_image_id, play_ab_button_down_image_id;
	U16 pause_ab_button_up_image_id, pause_ab_button_down_image_id;

	S16 stop_ab_button_x, stop_ab_button_y;
	U16 stop_ab_button_up_image_id, stop_ab_button_down_image_id;

	S16 once_always_button_x,once_always_button_y;
	U16 once_always_button_up_img_id,once_always_button_down_img_id;
	U16 once_always_button_off_up_img_id,once_always_button_off_down_img_id;
	U16 once_always_button_disable_img_id;
	abrepeat_absection_button_skin_struct ab_section_button_skin[MAX_AB_POINT];
	S16 progress_bar_x,progress_bar_y, progress_bar_ind_off_y;
	U16 progress_bar_img_id,progress_bar_ind_img_id;
	S16 ab_aud_file_rect_x,ab_aud_file_rect_y;
	U16 progress_bar_rect_img_id;

	U16 absection_playing_img_id;
	U16 absection_partial_set_img_id;	
	U16 absection_partial_set_down_img_id;
	U16 absection_set_sction_img_id;
	U16 absection_set_sction_down_img_id;
	U16 absection_nonset_section_img_id;
	U16 absection_nonset_section_down_img_id;
	U8 up_ab_key_mapping, down_ab_key_mapping, left_ab_key_mapping, right_ab_key_mapping;

} abrepeat_skin_struct;
#define NO_OF_AB_SKIN	1

#endif /* __MMI_AB_REPEATER__ */

#endif /* AB_REPEATER_DEF_H */


