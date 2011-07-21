#include "mmi_features.h"
#include "gdi_lcd_config.h"

// dependent headers for AudioPlayerDef.h"
#include "frameworkstruct.h"
#include "globalconstants.h"
//#include "filemgr.h"   //remove useless head file

#include "audioplayerdef.h"
//#include "fmradiodef.h"  //remove useless head file

#ifdef __MMI_AUDIO_PLAYER__ 
#ifdef __MMI_MAINLCD_240X320__
#ifdef __MMI_TOUCH_SCREEN__
static const audply_point_struct skin1_seek_points[] = {{29,277},{209,277},{29,283},{209,283}};
static const audply_point_struct skin1_volume_inc_points[] = {{71,154},{86,154},{71,167},{86,167}};
static const audply_point_struct skin1_volume_dec_points[] = {{27,154},{42,154},{27,167},{42,167}};
static const audply_point_struct skin1_prev_button_points[] =  {{42,191},{74,191},{42,239},{74,239}};
static const audply_point_struct skin1_next_button_points[] = {{158,188},{192,188},{158,238},{192,238}};
static const audply_point_struct skin1_play_button_points[] = {{78,170},{155,170},{78,247},{155,247}};
static const audply_point_struct skin1_stop_button_points[] = {{168,153},{213,153},{168,170},{213,170}};
static const audply_point_struct skin1_settings_button_points[] = {{179,250},{210,250},{179,272},{210,272}};
static const audply_point_struct skin1_repeat_points[] = {{28,250},{58,250},{28,272},{58,272}};
static const audply_point_struct skin1_shuffle_points[] = {{58,250},{88,250},{58,272},{88,272}};

static const audply_point_struct skin2_seek_points[] = {{0,0},{194,0},{194,10},{0,10}};
static const audply_point_struct skin2_volume_inc_points[] = {{0,0},{19,0},{19,19},{0,19}};
static const audply_point_struct skin2_volume_dec_points[] = {{0,0},{20,0},{20,19},{0,19}};
static const audply_point_struct skin2_prev_button_points[] = {{0,0},{20,0},{20,21},{0,21}};
static const audply_point_struct skin2_next_button_points[] = {{0,0},{20,0},{20,20},{0,20}};
static const audply_point_struct skin2_play_button_points[] = {{0,0},{36,0},{36,34},{0,34}};
static const audply_point_struct skin2_stop_button_points[] = {{0,0},{23,0},{23,23},{0,23}};
static const audply_point_struct skin2_setting_points[] = {{0,0},{22,0},{22,23},{0,23}};
static const audply_point_struct skin2_repeat_points[] = {{0,0},{22,0},{22,20},{0,20}};
static const audply_point_struct skin2_shuffle_points[] = {{0,0},{22,0},{22,28},{0,28}};

static const audply_point_struct skin3_seek_points[] = {{0,0},{194,0},{194,10},{0,10}};
static const audply_point_struct skin3_volume_inc_points[] = {{0,0},{16,0},{16,16},{0,16}};
static const audply_point_struct skin3_volume_dec_points[] = {{0,0},{16,0},{16,16},{0,16}};
static const audply_point_struct skin3_prev_button_points[] = {{0,0},{36,0},{36,37},{0,37}};
static const audply_point_struct skin3_next_button_points[] = {{0,0},{31,0},{31,30},{0,30}};
static const audply_point_struct skin3_play_button_points[] = {{0,0},{49,0},{49,48},{0,48}};
static const audply_point_struct skin3_stop_button_points[] = {{0,0},{20,0},{20,20},{0,20}};
static const audply_point_struct skin3_setting_points[] = {{0,0},{28,0},{28,28},{0,28}};
static const audply_point_struct skin3_repeat_points[] = {{0,0},{36,0},{36,16},{0,16}};
static const audply_point_struct skin3_shuffle_points[] = {{0,0},{20,0},{20,22},{0,22}};
#endif /*__MMI_TOUCH_SCREEN__*/


const audply_skin_struct g_audply_skins[] =
{
	{	//skin1
		IMG_ID_AUDPLY_MAIN_SKIN1_BG,  // background image
		255,255,0,//COMMON_COLOR Border color
		255,255,255,//COMMON_COLOR Text color
		0,0,0,//COMMON_COLOR BackGround color
		32,38,//SONG_TITLE x,y
		172,20,//SONG_TITLE w,h
		164,59,//TIME x,y
		40,15,//TIME w,h
		32,59,//SONG_INDEX x,y
		60,15,//SONG_INDEX w,h
		27,172,//PLAYLIST_ICON x,y
		{IMG_ID_AUDPLY_MAIN_SKIN1_PLAYLIST_PHONE_ICON,
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAYLIST_CARD_ICON},
		43,152, //VOLUME_BAR x,y
		7,//VOLUME_BAR indicator left
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_INDICATOR, // volumebar_ind_image_id
		29,277,// progressbar_x, progressbar_y
		IMG_ID_AUDPLY_MAIN_SKIN1_PROGRESSBAR, // progressbar_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PROGRESSBAR_FAIL, // progressbar_fail_image_id
		42,191, //PREVIOUS_BUTTON x,y 
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_ON, // prev_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_OFF, // prev_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_DISABLED, // prev_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_DOWN, // prev_button_down_image_id
		158,188, //NEXT_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_ON, // next_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_OFF, // next_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_DISABLED, // next_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_DOWN, // next_button_down_image_id
		80,170,//PLAY_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_ON, // play_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_OFF, // play_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_DISABLED, // play_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_DOWN, // play_button_down_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PAUSE_ON, // pause_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PAUSE_DOWN, // pause_button_down_image_id
		168,153, //STOP_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_ON, // stop_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_OFF, // stop_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_DISABLED, // stop_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_DOWN, // stop_button_down_image_id
#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
		30,83,//ANIMATION x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_ANIMATION, // animation_image_id
#endif
#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
		0,0,0, // lyric_fg_color_r,lyric_fg_color_g, lyric_fg_color_b
		255,255,255, // lyric_border_color_r, lyric_border_color_g, lyric_border_color_b
		13,73,//13,93, // lyric_display_x, lyric_display_y
		212,44,//212,94, // lyric_display_w, lyric_display_h
#endif
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
		28,249, // repeat_x, repeat_y
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_OFF_UP, // repeat off
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ALL_UP, // repeat all
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ONE_UP, // repeat one
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ALL_DOWN, // repeat all
        IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ONE_DOWN, // repeat one        
#endif /*__MMI_TOUCH_SCREEN__*/
		58,251, // shuffle_x, shuffle_y
		IMG_ID_AUDPLY_MAIN_SKIN1_SHUFFLE_OFF_UP, // shuffle off
		IMG_ID_AUDPLY_MAIN_SKIN1_SHUFFLE_ON_UP, // shuffle on		
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN1_SHUFFLE_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN1_SHUFFLE_ON_DOWN, // repeat all   
#endif /*__MMI_TOUCH_SCREEN__*/
#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/
#ifdef __MMI_TOUCH_SCREEN__
		27,154,// volume_inc_x, volume_inc_y,
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_INC_DOWN,// volume_inc_down_image_id
		71,154,// volume_dec_x, volume_dec_y,
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		179,250,//SETTINGS_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_SETTING_UP, // setting_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_SETTING_DOWN, // setting_down_image_id
		{(const audply_point_struct *)skin1_seek_points, 4},
		{(const audply_point_struct *)skin1_volume_inc_points, 4}, // volume_inc_area
		{(const audply_point_struct *)skin1_volume_dec_points, 4}, // volume_dec_area
		{(const audply_point_struct *)skin1_prev_button_points, 4}, // prev_button_area
		{(const audply_point_struct *)skin1_next_button_points, 4}, // next_button_area
		{(const audply_point_struct *)skin1_play_button_points, 4}, // play_button_area
		{(const audply_point_struct *)skin1_stop_button_points, 4}, // stop_button_area
		{(const audply_point_struct *) skin1_settings_button_points, 4},//SETTINGS_BUTTON area
		#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__		
		{(const audply_point_struct *)skin1_repeat_points, 4}, // repeat_area
		{(const audply_point_struct *)skin1_shuffle_points, 4}, // shuffle_area				
		#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/		

#endif
		AUDPLY_MAIN_PLAY, AUDPLY_MAIN_STOP, AUDPLY_MAIN_PREV, AUDPLY_MAIN_NEXT // up, down, left, right key mapping
	},
#ifndef __MMI_SLIM_AUDIO_PLAYER__
{	//skin2
		IMG_ID_AUDPLY_MAIN_SKIN2_BG, // bg_image_id
		0,0,0, // text_border_color_r, text_border_color_g, text_border_color_b
		0,217,0, // text_fg_color_r, text_fg_color_g, text_fg_color_b
		0,0,0, // text_bg_color_r, text_bg_color_g, text_bg_color_b
		26,31,//25,44,// title_x, title_y
		80,17,// title_w, title_h
		5,60,//40,64,// time_x, time_y
		48,10,// time_w, time_h
		45,60,//35,78, // file_index_x, file_index_y
		60,10, // file_index_w, file_index_h
		22,64, // playlisticon_x, playlisticon_y
		{IMG_ID_AUDPLY_MAIN_SKIN2_PLAYLIST_PHONE_ICON, IMG_ID_AUDPLY_MAIN_SKIN2_PLAYLIST_CARD_ICON}, // playlisticon_images_id[NO_OF_LIST]
		48,141,5,//99,107,5,// volumebar_x, volumebar_y, volumebar_ind_left
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_INDICATOR, // volumebar_ind_image_id
		16,194,// progressbar_x, progressbar_y
		IMG_ID_AUDPLY_MAIN_SKIN2_PROGRESSBAR, // progressbar_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PROGRESSBAR_FAIL, // progressbar_fail_image_id
		35,98,//7,105, // prev_button_x, prev_button_y
		IMG_ID_AUDPLY_MAIN_SKIN2_PREV_ON, // prev_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PREV_OFF, // prev_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PREV_DISABLED, // prev_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PREV_DOWN, // prev_button_down_image_id
		83,98,//59,105, // next_button_x, next_button_y
		IMG_ID_AUDPLY_MAIN_SKIN2_NEXT_ON, // next_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_NEXT_OFF, // next_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_NEXT_DISABLED, // next_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_NEXT_DOWN, // next_button_down_image_id
		60,73,//25,105, // play_button_x, play_button_y
		IMG_ID_AUDPLY_MAIN_SKIN2_PLAY_ON, // play_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PLAY_OFF, // play_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PLAY_DISABLED, // play_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PLAY_DOWN, // play_button_down_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PAUSE_ON, // pause_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PAUSE_DOWN, // pause_button_down_image_id
		60,122,//43,105, // stop_button_x, stop_button_y
		IMG_ID_AUDPLY_MAIN_SKIN2_STOP_ON, // stop_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_STOP_OFF, // stop_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_STOP_DISABLED, // stop_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_STOP_DOWN, // stop_button_down_image_id
#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
		-1,-1, // animation_x, animation_y
		IMG_ID_AUDPLY_MAIN_SKIN2_ANIMATION, // animation_image_id
#endif
#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
		2,88,137, // lyric_fg_color_r,lyric_fg_color_g, lyric_fg_color_b
		255,255,255, // lyric_border_color_r, lyric_border_color_g, lyric_border_color_b
		18,96, // lyric_display_x, lyric_display_y
		202,88, // lyric_display_w, lyric_display_h
#endif
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
		12,216, // repeat_x, repeat_y
		IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_OFF_UP, // repeat off
		IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_ALL_UP, // repeat all
		IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_ONE_UP, // repeat one
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_ALL_DOWN, // repeat all
        IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_ONE_DOWN, // repeat one        
#endif /*__MMI_TOUCH_SCREEN__*/
		39,216, // shuffle_x, shuffle_y
		IMG_ID_AUDPLY_MAIN_SKIN2_SHUFFLE_OFF_UP, // shuffle off
		IMG_ID_AUDPLY_MAIN_SKIN2_SHUFFLE_ON_UP, // shuffle on		
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN2_SHUFFLE_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN2_SHUFFLE_ON_DOWN, // repeat all   
#endif /*__MMI_TOUCH_SCREEN__*/
#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/
#ifdef __MMI_TOUCH_SCREEN__
		147,200,// volume_inc_x, volume_inc_y,
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_INC_DOWN,// volume_inc_down_image_id
		84,200,// volume_dec_x, volume_dec_y,
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		8,158,// setting_x, setting_y
		IMG_ID_AUDPLY_MAIN_SKIN2_SETTING_UP, // setting_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_SETTING_DOWN, // setting_down_image_id
		{(const audply_point_struct *)skin2_seek_points, 4},
		{(const audply_point_struct *)skin2_volume_inc_points, 4}, // volume_inc_area
		{(const audply_point_struct *)skin2_volume_dec_points, 4}, // volume_dec_area
		{(const audply_point_struct *)skin2_prev_button_points, 4}, // prev_button_area
		{(const audply_point_struct *)skin2_next_button_points, 4}, // next_button_area
		{(const audply_point_struct *)skin2_play_button_points, 4}, // play_button_area
		{(const audply_point_struct *)skin2_stop_button_points, 4}, // stop_button_area
		{(const audply_point_struct *)0, 0}, // setting_area 4
		#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__		
		{(const audply_point_struct *)skin2_repeat_points, 4}, // repeat_area
		{(const audply_point_struct *)skin2_shuffle_points, 4}, // shuffle_area				
		#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/		

#endif
		AUDPLY_MAIN_PLAY, AUDPLY_MAIN_STOP, AUDPLY_MAIN_PREV, AUDPLY_MAIN_NEXT // up, down, left, right key mapping
	},
{	//skin3
		IMG_ID_AUDPLY_MAIN_SKIN3_BG, // bg_image_id
		0,0,0, // text_border_color_r, text_border_color_g, text_border_color_b
		0,217,0, // text_fg_color_r, text_fg_color_g, text_fg_color_b
		0,0,0, // text_bg_color_r, text_bg_color_g, text_bg_color_b
		26,31,//25,44,// title_x, title_y
		80,17,// title_w, title_h
		5,60,//40,64,// time_x, time_y
		48,10,// time_w, time_h
		45,60,//35,78, // file_index_x, file_index_y
		60,10, // file_index_w, file_index_h
		22,64, // playlisticon_x, playlisticon_y
		{IMG_ID_AUDPLY_MAIN_SKIN3_PLAYLIST_PHONE_ICON, IMG_ID_AUDPLY_MAIN_SKIN3_PLAYLIST_CARD_ICON}, // playlisticon_images_id[NO_OF_LIST]
		48,141,5,//99,107,5,// volumebar_x, volumebar_y, volumebar_ind_left
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_INDICATOR, // volumebar_ind_image_id
		16,194,// progressbar_x, progressbar_y
		IMG_ID_AUDPLY_MAIN_SKIN3_PROGRESSBAR, // progressbar_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PROGRESSBAR_FAIL, // progressbar_fail_image_id
		35,98,//7,105, // prev_button_x, prev_button_y
		IMG_ID_AUDPLY_MAIN_SKIN3_PREV_ON, // prev_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PREV_OFF, // prev_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PREV_DISABLED, // prev_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PREV_DOWN, // prev_button_down_image_id
		83,98,//59,105, // next_button_x, next_button_y
		IMG_ID_AUDPLY_MAIN_SKIN3_NEXT_ON, // next_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_NEXT_OFF, // next_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_NEXT_DISABLED, // next_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_NEXT_DOWN, // next_button_down_image_id
		60,73,//25,105, // play_button_x, play_button_y
		IMG_ID_AUDPLY_MAIN_SKIN3_PLAY_ON, // play_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PLAY_OFF, // play_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PLAY_DISABLED, // play_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PLAY_DOWN, // play_button_down_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PAUSE_ON, // pause_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PAUSE_DOWN, // pause_button_down_image_id
		60,122,//43,105, // stop_button_x, stop_button_y
		IMG_ID_AUDPLY_MAIN_SKIN3_STOP_ON, // stop_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_STOP_OFF, // stop_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_STOP_DISABLED, // stop_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_STOP_DOWN, // stop_button_down_image_id
#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
		-1,-1, // animation_x, animation_y
		IMG_ID_AUDPLY_MAIN_SKIN3_ANIMATION, // animation_image_id
#endif
#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
		0,0,0, // lyric_fg_color_r,lyric_fg_color_g, lyric_fg_color_b
		255,255,255, // lyric_border_color_r, lyric_border_color_g, lyric_border_color_b
		13,143,//13,93, // lyric_display_x, lyric_display_y
		212,44,//212,94, // lyric_display_w, lyric_display_h
#endif
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
		12,216, // repeat_x, repeat_y
		IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_OFF_UP, // repeat off
		IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_ALL_UP, // repeat all
		IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_ONE_UP, // repeat one
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_ALL_DOWN, // repeat all
        IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_ONE_DOWN, // repeat one        
#endif /*__MMI_TOUCH_SCREEN__*/
		39,216, // shuffle_x, shuffle_y
		IMG_ID_AUDPLY_MAIN_SKIN3_SHUFFLE_OFF_UP, // shuffle off
		IMG_ID_AUDPLY_MAIN_SKIN3_SHUFFLE_ON_UP, // shuffle on		
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN3_SHUFFLE_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN3_SHUFFLE_ON_DOWN, // repeat all   
#endif /*__MMI_TOUCH_SCREEN__*/
#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/
#ifdef __MMI_TOUCH_SCREEN__
		147,200,// volume_inc_x, volume_inc_y,
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_INC_DOWN,// volume_inc_down_image_id
		84,200,// volume_dec_x, volume_dec_y,
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		8,158,// setting_x, setting_y
		IMG_ID_AUDPLY_MAIN_SKIN3_SETTING_UP, // setting_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_SETTING_DOWN, // setting_down_image_id
		{(const audply_point_struct *)skin3_seek_points, 4},
		{(const audply_point_struct *)skin3_volume_inc_points, 4}, // volume_inc_area
		{(const audply_point_struct *)skin3_volume_dec_points, 4}, // volume_dec_area
		{(const audply_point_struct *)skin3_prev_button_points, 4}, // prev_button_area
		{(const audply_point_struct *)skin3_next_button_points, 4}, // next_button_area
		{(const audply_point_struct *)skin3_play_button_points, 4}, // play_button_area
		{(const audply_point_struct *)skin3_stop_button_points, 4}, // stop_button_area
		{(const audply_point_struct *)0, 0}, // setting_area 4
		#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__		
		{(const audply_point_struct *)skin1_repeat_points, 4}, // repeat_area
		{(const audply_point_struct *)skin1_shuffle_points, 4}, // shuffle_area				
		#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/		
#endif
		AUDPLY_MAIN_PLAY, AUDPLY_MAIN_STOP, AUDPLY_MAIN_PREV, AUDPLY_MAIN_NEXT // up, down, left, right key mapping
	}
#endif
};

#elif defined(__MMI_MAINLCD_176X220__)
#ifdef __MMI_TOUCH_SCREEN__
static const audply_point_struct skin1_previous_button_points[] = {{54,160},{78,160},{54,185},{78,185}};
static const audply_point_struct skin1_next_button_points[] = {{125,161},{149,161},{125,192},{149,192}};
static const audply_point_struct skin1_play_button_points[] = {{102,161},{125,161},{102,186},{125,186}};
static const audply_point_struct skin1_stop_button_points[] = {{77,160},{103,160},{77,185},{103,185}};
static const audply_point_struct skin1_volume_increase_points[] = {{70,130},{83,130},{70,149},{83,149}};
static const audply_point_struct skin1_volume_decrease_points[] = {{28,128},{41,128},{28,149},{41,149}};
static const audply_point_struct skin1_settings_button_points[] = {{26,160},{56,160},{26,186},{56,186}};

static const audply_point_struct skin2_previous_button_points[] = {{28,184},{44,184},{28,220},{44,220}};
static const audply_point_struct skin2_next_button_points[] = {{89,184},{105,184},{89,210},{105,210}};
static const audply_point_struct skin2_play_button_points[] = {{65,184},{81,184},{65,207},{81,207}};
static const audply_point_struct skin2_stop_button_points[] = {{116,184},{149,184},{116,200},{149,200}};
static const audply_point_struct skin2_volume_increase_points[] = {{2,162},{19,162},{2,179},{19,179}};
static const audply_point_struct skin2_volume_decrease_points[] = {{48,164},{65,164},{48,181},{65,181}};
static const audply_point_struct skin2_settings_button_points[] = {{83,162},{102,162},{83,177},{102,177}};

static const audply_point_struct skin3_previous_button_points[] = {{29,158},{45,158},{29,171},{45,171}};
static const audply_point_struct skin3_next_button_points[] = {{76,177},{89,177},{76,189},{89,189}};
static const audply_point_struct skin3_play_button_points[] = {{53,141},{69,141},{53,156},{69,156}};
static const audply_point_struct skin3_stop_button_points[] = {{51,174},{62,174},{51,184},{62,184}};
static const audply_point_struct skin3_volume_increase_points[] = {{138,143},{162,143},{138,168},{162,168}};
static const audply_point_struct skin3_volume_decrease_points[] = {{83,144},{106,144},{83,168},{106,168}};
static const audply_point_struct skin3_settings_button_points[] = {{121,37},{149,37},{121,66},{149,66}};


static const audply_point_struct skin1_seek_points[] = {{29,277},{209,277},{29,283},{209,283}};
static const audply_point_struct skin1_volume_inc_points[] = {{71,154},{84,154},{71,164},{84,164}};
static const audply_point_struct skin1_volume_dec_points[] = {{27,154},{39,154},{27,164},{39,164}};
static const audply_point_struct skin1_prev_button_points[] =  {{42,191},{74,191},{42,239},{74,239}};
static const audply_point_struct skin1_repeat_points[] = {{28,250},{58,250},{28,272},{58,272}};
static const audply_point_struct skin1_shuffle_points[] = {{58,250},{88,250},{58,272},{88,272}};

static const audply_point_struct skin2_seek_points[] = {{0,0},{194,0},{194,10},{0,10}};
static const audply_point_struct skin2_volume_inc_points[] = {{0,0},{19,0},{19,19},{0,19}};
static const audply_point_struct skin2_volume_dec_points[] = {{0,0},{20,0},{20,19},{0,19}};
static const audply_point_struct skin2_prev_button_points[] = {{0,0},{20,0},{20,21},{0,21}};
static const audply_point_struct skin2_setting_points[] = {{0,0},{22,0},{22,23},{0,23}};
static const audply_point_struct skin2_repeat_points[] = {{0,0},{22,0},{22,20},{0,20}};
static const audply_point_struct skin2_shuffle_points[] = {{0,0},{22,0},{22,28},{0,28}};

static const audply_point_struct skin3_seek_points[] = {{0,0},{194,0},{194,10},{0,10}};
static const audply_point_struct skin3_volume_inc_points[] = {{0,0},{16,0},{16,16},{0,16}};
static const audply_point_struct skin3_volume_dec_points[] = {{0,0},{16,0},{16,16},{0,16}};
static const audply_point_struct skin3_prev_button_points[] = {{0,0},{36,0},{36,37},{0,37}};
static const audply_point_struct skin3_setting_points[] = {{0,0},{28,0},{28,28},{0,28}};
static const audply_point_struct skin3_repeat_points[] = {{0,0},{36,0},{36,16},{0,16}};
static const audply_point_struct skin3_shuffle_points[] = {{0,0},{20,0},{20,22},{0,22}};

#endif
const audply_skin_struct g_audply_skins[] =
{
	{//SKIN1
		IMG_ID_AUDPLY_MAIN_SKIN1_BG,// background image
		255,255,0,//COMMON_COLOR Border color
		255,255,255,//COMMON_COLOR Text color
		0,0,0,//COMMON_COLOR BackGround color
		28,53, //SONG_TITLE x, y
		115,18, //SONG_TITLE w, h
		0,23, //TIME x, y
		48,18, //TIME w, h
		125,23, //SONG_INDEX x, y
		50,18, //SONG_INDEX w, h
		0,1, //PLAYLIST_ICON x,y
		{IMG_ID_AUDPLY_MAIN_SKIN1_PLAYLIST_PHONE_ICON,
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAYLIST_CARD_ICON},
		43,134, //VOLUME_BAR x,y
		6,
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_INDICATOR,
		16,194,// progressbar_x, progressbar_y
		IMG_ID_AUDPLY_MAIN_SKIN1_PROGRESSBAR, // progressbar_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PROGRESSBAR_FAIL, // progressbar_fail_image_id
		53,160, //PREVIOUS_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_ON,
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_DOWN,
		128,161, //NEXT_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_ON,
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_DOWN,
		103,161, //PLAY_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_ON,
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_DOWN,
		IMG_ID_AUDPLY_MAIN_SKIN1_PAUSE_ON,
		IMG_ID_AUDPLY_MAIN_SKIN1_PAUSE_DOWN,
		77,160,//STOP_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_ON,
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_DOWN,
#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
		30,77, //ANIMATION x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_ANIMATION, // animation_image_id
#endif
#ifdef __MMI_AUDIO_SPECTRUM_DISPLAY__
		2,160, //SPECTRUM x, y
		18,18, //SPECTRUM w, h
#endif
#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
		0,0,100,//LYRICS Text color
		255,255,0,//LYRICS Border color
        28,2,          //LYRICS x,y
        120,18,      //LYRICS w,h
/*        49,23, //LYRICS x, y
		90,18, //LYRICS w, h*/
#if defined(__MMI_AUDIO_SPECTRUM_DISPLAY__) && defined(__MMI_AUDIO_PLAYER_LYRIC_DISPLAY__)
0,//exclusive (when exclusive box is checkd)
#endif
#endif
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
		12,216, // repeat_x, repeat_y
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_OFF_UP, // repeat off
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ALL_UP, // repeat all
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ONE_UP, // repeat one
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ALL_DOWN, // repeat all
        IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ONE_DOWN, // repeat one        
#endif /*__MMI_TOUCH_SCREEN__*/
		39,216, // shuffle_x, shuffle_y
		IMG_ID_AUDPLY_MAIN_SKIN1_SHUFFLE_OFF_UP, // shuffle off
		IMG_ID_AUDPLY_MAIN_SKIN1_SHUFFLE_ON_UP, // shuffle on		
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN1_SHUFFLE_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN1_SHUFFLE_ON_DOWN, // repeat all   
#endif /*__MMI_TOUCH_SCREEN__*/
#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/
#ifdef __MMI_TOUCH_SCREEN__
		71,129, //VOLUME_INCREASE x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_INC_DOWN,// volume_inc_down_image_id
		28,128,//VOLUME_DECREASE x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		23,160, //SETTINGS_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_SETTING_UP, // setting_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_SETTING_DOWN, // setting_down_image_id
		{(const audply_point_struct *)skin1_seek_points, 4},
		{(const audply_point_struct *) skin1_volume_increase_points, 4},//VOLUME_INCREASE area
		{(const audply_point_struct *) skin1_volume_decrease_points, 4},//VOLUME_DECREASE area
		{(const audply_point_struct *) skin1_previous_button_points, 4},//PREVIOUS_BUTTON area
		{(const audply_point_struct *)skin1_next_button_points, 4}, // next_button_area
		{(const audply_point_struct *)skin1_play_button_points, 4}, // play_button_area
		{(const audply_point_struct *)skin1_stop_button_points, 4}, // stop_button_area
{(const audply_point_struct *) skin1_settings_button_points, 4},//SETTINGS_BUTTON area
		#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__		
		{(const audply_point_struct *)skin1_repeat_points, 4}, // repeat_area
		{(const audply_point_struct *)skin1_shuffle_points, 4}, // shuffle_area				
		#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/		

#endif
		AUDPLY_MAIN_PLAY, AUDPLY_MAIN_STOP, AUDPLY_MAIN_PREV, AUDPLY_MAIN_NEXT // up, down, left, right key mapping
	},
#ifndef __MMI_SLIM_AUDIO_PLAYER__
{	//skin2
		IMG_ID_AUDPLY_MAIN_SKIN2_BG, // bg_image_id
		255,255,0,//COMMON_COLOR Border color
		255,255,255,//COMMON_COLOR Text color
		0,0,0,//COMMON_COLOR BackGround color
		82,3,//SONG_TITLE x,y
		90,16,//SONG_TITLE w,h
		112,26,//TIME x,y
		60,15,//TIME w,h
		63,3,//SONG_INDEX x,y
		15,16,//SONG_INDEX w,h
		50,183,//PLAYLIST_ICON x,y
		{IMG_ID_AUDPLY_MAIN_SKIN2_PLAYLIST_PHONE_ICON,
		IMG_ID_AUDPLY_MAIN_SKIN2_PLAYLIST_CARD_ICON},
		20,167,//VOLUME_BAR x,y
		7,
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_INDICATOR,
		16,194,// progressbar_x, progressbar_y
		IMG_ID_AUDPLY_MAIN_SKIN2_PROGRESSBAR, // progressbar_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PROGRESSBAR_FAIL, // progressbar_fail_image_id
		28,184,//PREVIOUS_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN2_PREV_ON,
		IMG_ID_AUDPLY_MAIN_SKIN2_PREV_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN2_PREV_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN2_PREV_DOWN,
		89,184,//NEXT_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN2_NEXT_ON,
		IMG_ID_AUDPLY_MAIN_SKIN2_NEXT_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN2_NEXT_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN2_NEXT_DOWN,
		65,184,//PLAY_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN2_PLAY_ON,
		IMG_ID_AUDPLY_MAIN_SKIN2_PLAY_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN2_PLAY_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN2_PLAY_DOWN,
		IMG_ID_AUDPLY_MAIN_SKIN2_PAUSE_ON,
		IMG_ID_AUDPLY_MAIN_SKIN2_PAUSE_DOWN,
		116,184,//STOP_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN2_STOP_ON,
		IMG_ID_AUDPLY_MAIN_SKIN2_STOP_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN2_STOP_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN2_STOP_DOWN,
#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
		10,66,//ANIMATION x,y
		IMG_ID_AUDPLY_MAIN_SKIN2_ANIMATION, // animation_image_id
#endif
#ifdef __MMI_AUDIO_SPECTRUM_DISPLAY__
		107,162,//SPECTRUM x,y
		57,15,//SPECTRUM w,h
#endif
#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
		0,0,100,//LYRICS Text color
		255,255,0,//LYRICS Border color
		71,51,//LYRICS x,y
		100,16,//LYRICS w,h
#if defined(__MMI_AUDIO_SPECTRUM_DISPLAY__) && defined(__MMI_AUDIO_PLAYER_LYRIC_DISPLAY__)
0,//exclusive (when exclusive box is checkd)
#endif
#endif
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
		12,216, // repeat_x, repeat_y
		IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_OFF_UP, // repeat off
		IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_ALL_UP, // repeat all
		IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_ONE_UP, // repeat one
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_ALL_DOWN, // repeat all
        IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_ONE_DOWN, // repeat one        
#endif /*__MMI_TOUCH_SCREEN__*/
		39,216, // shuffle_x, shuffle_y
		IMG_ID_AUDPLY_MAIN_SKIN2_SHUFFLE_OFF_UP, // shuffle off
		IMG_ID_AUDPLY_MAIN_SKIN2_SHUFFLE_ON_UP, // shuffle on		
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN2_SHUFFLE_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN2_SHUFFLE_ON_DOWN, // repeat all   
#endif /*__MMI_TOUCH_SCREEN__*/
#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/
#ifdef __MMI_TOUCH_SCREEN__
		2,162,//VOLUME_INCREASE x,y
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_INC_DOWN,// volume_inc_down_image_id
		48,164,//VOLUME_DECREASE x,y
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		83,162,//SETTINGS_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN2_SETTING_UP, // setting_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_SETTING_DOWN, // setting_down_image_id
		{(const audply_point_struct *)skin2_seek_points, 4},
		{(const audply_point_struct *)skin2_volume_increase_points,4 }, // volume_inc_area
		{(const audply_point_struct *)skin2_volume_decrease_points, 4}, // volume_dec_area
		{(const audply_point_struct *) skin2_previous_button_points, 4},//PREVIOUS_BUTTON area
		{(const audply_point_struct *) skin2_next_button_points, 4},//NEXT_BUTTON area
		{(const audply_point_struct *) skin2_play_button_points, 4},//PLAY_BUTTON area
		{(const audply_point_struct *) skin2_stop_button_points, 4},//STOP_BUTTON area
		{(const audply_point_struct *) skin2_settings_button_points, 4},//SETTINGS_BUTTON area
		#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__		
		{(const audply_point_struct *)skin2_repeat_points, 4}, // repeat_area
		{(const audply_point_struct *)skin2_shuffle_points, 4}, // shuffle_area				
		#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/		

#endif
		AUDPLY_MAIN_PLAY, AUDPLY_MAIN_STOP, AUDPLY_MAIN_PREV, AUDPLY_MAIN_NEXT // up, down, left, right key mapping
	},
{	//skin3
		IMG_ID_AUDPLY_MAIN_SKIN3_BG, // bg_image_id
		255,255,0,//COMMON_COLOR Border color
		255,255,255,//COMMON_COLOR Text color
		0,0,0,//COMMON_COLOR BackGround color
		39,6,//SONG_TITLE x,y
		110,16,//SONG_TITLE w,h
		46,44,//TIME x,y
		60,15,//TIME w,h
		20,6,//SONG_INDEX x,y
		15,16,//SONG_INDEX w,h
		25,43,//PLAYLIST_ICON x,y
		{IMG_ID_AUDPLY_MAIN_SKIN3_PLAYLIST_PHONE_ICON,
		IMG_ID_AUDPLY_MAIN_SKIN3_PLAYLIST_CARD_ICON},
		109,150,//VOLUME_BAR x,y
		7,
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_INDICATOR,
		16,194,// progressbar_x, progressbar_y
		IMG_ID_AUDPLY_MAIN_SKIN3_PROGRESSBAR, // progressbar_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PROGRESSBAR_FAIL, // progressbar_fail_image_id
		29,158,//PREVIOUS_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN3_PREV_ON,
		IMG_ID_AUDPLY_MAIN_SKIN3_PREV_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN3_PREV_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN3_PREV_DOWN,
		76,177,//NEXT_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN3_NEXT_ON,
		IMG_ID_AUDPLY_MAIN_SKIN3_NEXT_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN3_NEXT_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN3_NEXT_DOWN,
		53,141,//PLAY_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN3_PLAY_ON,
		IMG_ID_AUDPLY_MAIN_SKIN3_PLAY_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN3_PLAY_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN3_PLAY_DOWN,
		IMG_ID_AUDPLY_MAIN_SKIN3_PAUSE_ON,
		IMG_ID_AUDPLY_MAIN_SKIN3_PAUSE_DOWN,
		51,174,//STOP_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN3_STOP_ON,
		IMG_ID_AUDPLY_MAIN_SKIN3_STOP_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN3_STOP_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN3_STOP_DOWN,
#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
		42,96,//ANIMATION x,y
		IMG_ID_AUDPLY_MAIN_SKIN3_ANIMATION,
#endif
#ifdef __MMI_AUDIO_SPECTRUM_DISPLAY__
		67,60,//SPECTRUM x,y
		50,15,//SPECTRUM w,h
#endif
#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
		0,0,100,//LYRICS Text color
		255,255,0,//LYRICS Border color
		48,80,//LYRICS x,y
		80,16,//LYRICS w,h
#if defined(__MMI_AUDIO_SPECTRUM_DISPLAY__) && defined(__MMI_AUDIO_PLAYER_LYRIC_DISPLAY__)
0,//exclusive (when exclusive box is checkd)
#endif
#endif
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
		12,216, // repeat_x, repeat_y
		IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_OFF_UP, // repeat off
		IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_ALL_UP, // repeat all
		IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_ONE_UP, // repeat one
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_ALL_DOWN, // repeat all
        IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_ONE_DOWN, // repeat one        
#endif /*__MMI_TOUCH_SCREEN__*/
		39,216, // shuffle_x, shuffle_y
		IMG_ID_AUDPLY_MAIN_SKIN3_SHUFFLE_OFF_UP, // shuffle off
		IMG_ID_AUDPLY_MAIN_SKIN3_SHUFFLE_ON_UP, // shuffle on		
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN3_SHUFFLE_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN3_SHUFFLE_ON_DOWN, // repeat all   
#endif /*__MMI_TOUCH_SCREEN__*/
#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/
#ifdef __MMI_TOUCH_SCREEN__
		138,143,//VOLUME_INCREASE x,y
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_INC_DOWN,// volume_inc_down_image_id
		83,144,//VOLUME_DECREASE x,y
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		121,37,//SETTINGS_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN3_SETTING_UP, // setting_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_SETTING_DOWN, // setting_down_image_id
		{(const audply_point_struct *)skin3_seek_points, 4},
		{(const audply_point_struct *)skin3_volume_increase_points,4 }, // volume_inc_area
		{(const audply_point_struct *)skin3_volume_decrease_points, 4}, // volume_dec_area
		{(const audply_point_struct *) skin3_previous_button_points, 4},//PREVIOUS_BUTTON area
		{(const audply_point_struct *) skin3_next_button_points, 4},//NEXT_BUTTON area
		{(const audply_point_struct *) skin3_play_button_points, 4},//PLAY_BUTTON area
		{(const audply_point_struct *) skin3_stop_button_points, 4},//STOP_BUTTON area
		{(const audply_point_struct *) skin3_settings_button_points, 4},//SETTINGS_BUTTON area
		#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__		
		{(const audply_point_struct *)skin3_repeat_points, 4}, // repeat_area
		{(const audply_point_struct *)skin3_shuffle_points, 4}, // shuffle_area				
		#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/		

#endif
		AUDPLY_MAIN_PLAY, AUDPLY_MAIN_STOP, AUDPLY_MAIN_PREV, AUDPLY_MAIN_NEXT // up, down, left, right key mapping
	}
#endif
};
#elif defined(__MMI_MAINLCD_220X176__)
#ifdef __MMI_TOUCH_SCREEN__
static const audply_point_struct skin1_seek_points[] = {{29,277},{209,277},{29,283},{209,283}};
static const audply_point_struct skin1_previous_button_points[] = {{54,160},{78,160},{54,185},{78,185}};
static const audply_point_struct skin1_next_button_points[] = {{125,161},{149,161},{125,192},{149,192}};
static const audply_point_struct skin1_play_button_points[] = {{102,161},{125,161},{102,186},{125,186}};
static const audply_point_struct skin1_stop_button_points[] = {{77,160},{103,160},{77,185},{103,185}};
static const audply_point_struct skin1_volume_increase_points[] = {{70,130},{83,130},{70,149},{83,149}};
static const audply_point_struct skin1_volume_decrease_points[] = {{28,128},{41,128},{28,149},{41,149}};
static const audply_point_struct skin1_settings_button_points[] = {{26,160},{56,160},{26,186},{56,186}};
static const audply_point_struct skin1_repeat_points[] = {{28,250},{58,250},{28,272},{58,272}};
static const audply_point_struct skin1_shuffle_points[] = {{58,250},{88,250},{58,272},{88,272}};

static const audply_point_struct skin2_previous_button_points[] = {{28,184},{44,184},{28,220},{44,220}};
static const audply_point_struct skin2_next_button_points[] = {{89,184},{105,184},{89,210},{105,210}};
static const audply_point_struct skin2_play_button_points[] = {{65,184},{81,184},{65,207},{81,207}};
static const audply_point_struct skin2_stop_button_points[] = {{116,184},{149,184},{116,200},{149,200}};
static const audply_point_struct skin2_volume_increase_points[] = {{2,162},{19,162},{2,179},{19,179}};
static const audply_point_struct skin2_volume_decrease_points[] = {{48,164},{65,164},{48,181},{65,181}};
static const audply_point_struct skin2_settings_button_points[] = {{83,162},{102,162},{83,177},{102,177}};

static const audply_point_struct skin3_previous_button_points[] = {{29,158},{45,158},{29,171},{45,171}};
static const audply_point_struct skin3_next_button_points[] = {{76,177},{89,177},{76,189},{89,189}};
static const audply_point_struct skin3_play_button_points[] = {{53,141},{69,141},{53,156},{69,156}};
static const audply_point_struct skin3_stop_button_points[] = {{51,174},{62,174},{51,184},{62,184}};
static const audply_point_struct skin3_volume_increase_points[] = {{138,143},{162,143},{138,168},{162,168}};
static const audply_point_struct skin3_volume_decrease_points[] = {{83,144},{106,144},{83,168},{106,168}};
static const audply_point_struct skin3_settings_button_points[] = {{121,37},{149,37},{121,66},{149,66}};

#endif
const audply_skin_struct g_audply_skins[] =
{
	{//SKIN1
		IMG_ID_AUDPLY_MAIN_SKIN1_BG,// background image
		255,255,0,//COMMON_COLOR Border color
		255,255,255,//COMMON_COLOR Text color
		0,0,0,//COMMON_COLOR BackGround color
		51,26, //SONG_TITLE x, y
		115,18, //SONG_TITLE w, h
		120,104, //TIME x, y
		48,18, //TIME w, h
		139,44, //SONG_INDEX x, y
		36,18, //SONG_INDEX w, h
		0,1, //PLAYLIST_ICON x,y
		{IMG_ID_AUDPLY_MAIN_SKIN1_PLAYLIST_PHONE_ICON,
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAYLIST_CARD_ICON},
		65,105, //VOLUME_BAR x,y
		7,
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_INDICATOR,
		0,0,// progressbar_x, progressbar_y
		0, // progressbar_image_id
		0, // progressbar_fail_image_id
		75,132, //PREVIOUS_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_ON,
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_DOWN,
		146,133, //NEXT_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_ON,
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_DOWN,
		122,133, //PLAY_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_ON,
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_DOWN,
		IMG_ID_AUDPLY_MAIN_SKIN1_PAUSE_ON,
		IMG_ID_AUDPLY_MAIN_SKIN1_PAUSE_DOWN,
		98,132,//STOP_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_ON,
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_DOWN,
#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
		52,48, //ANIMATION x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_ANIMATION, // animation_image_id
#endif
#ifdef __MMI_AUDIO_SPECTRUM_DISPLAY__
		2,130, //SPECTRUM x, y
		18,18, //SPECTRUM w, h
#endif
#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
		0,0,100,//LYRICS Text color
		255,255,0,//LYRICS Border color
		49,23, //LYRICS x, y
		90,18, //LYRICS w, h
#if defined(__MMI_AUDIO_SPECTRUM_DISPLAY__) && defined(__MMI_AUDIO_PLAYER_LYRIC_DISPLAY__)
		0,//exclusive (when exclusive box is checkd)
#endif
#endif
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
		89,130, //REPEAT_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_OFF_UP, // repeat off
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ALL_UP, // repeat all
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ONE_UP, // repeat one
		#ifdef __MMI_TOUCH_SCREEN__
    		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_OFF_DOWN, // repeat off
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ALL_DOWN, // repeat all
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ONE_DOWN, // repeat one
    		#endif
		118,129, //SHUFFLE_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_SHUFFLE_OFF_UP, // shuffle off
		IMG_ID_AUDPLY_MAIN_SKIN1_SHUFFLE_ON_UP, // shuffle on
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN1_SHUFFLE_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN1_SHUFFLE_ON_DOWN, // repeat all   
#endif /*__MMI_TOUCH_SCREEN__*/
#endif
#ifdef __MMI_TOUCH_SCREEN__
		71,129, //VOLUME_INCREASE x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_INC_DOWN,// volume_inc_down_image_id
		28,128,//VOLUME_DECREASE x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		23,160, //SETTINGS_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_SETTING_UP, // setting_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_SETTING_DOWN, // setting_down_image_id
		{(const audply_point_struct *)skin1_seek_points, 4},
		{(const audply_point_struct *) skin1_volume_increase_points, 4},//VOLUME_INCREASE area
		{(const audply_point_struct *) skin1_volume_decrease_points, 4},//VOLUME_DECREASE area
		{(const audply_point_struct *) skin1_previous_button_points, 4},//PREVIOUS_BUTTON area
		{(const audply_point_struct *)skin1_next_button_points, 4}, // next_button_area
		{(const audply_point_struct *)skin1_play_button_points, 4}, // play_button_area
		{(const audply_point_struct *)skin1_stop_button_points, 4}, // stop_button_area
		{(const audply_point_struct *) skin1_settings_button_points, 4},//SETTINGS_BUTTON area
		#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__		
		{(const audply_point_struct *)skin1_repeat_points, 4}, // repeat_area
		{(const audply_point_struct *)skin1_shuffle_points, 4}, // shuffle_area				
		#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/		

#endif
		AUDPLY_MAIN_PLAY, AUDPLY_MAIN_STOP, AUDPLY_MAIN_PREV, AUDPLY_MAIN_NEXT // up, down, left, right key mapping
	},
#ifndef __MMI_SLIM_AUDIO_PLAYER__
{	//skin2
		IMG_ID_AUDPLY_MAIN_SKIN2_BG, // bg_image_id
		255,255,0,//COMMON_COLOR Border color
		255,255,255,//COMMON_COLOR Text color
		0,0,0,//COMMON_COLOR BackGround color
		82,3,//SONG_TITLE x,y
		90,16,//SONG_TITLE w,h
		112,26,//TIME x,y
		60,15,//TIME w,h
		63,3,//SONG_INDEX x,y
		15,16,//SONG_INDEX w,h
		50,183,//PLAYLIST_ICON x,y
		{IMG_ID_AUDPLY_MAIN_SKIN2_PLAYLIST_PHONE_ICON,
		IMG_ID_AUDPLY_MAIN_SKIN2_PLAYLIST_CARD_ICON},
		20,167,//VOLUME_BAR x,y
		7,
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_INDICATOR,
		28,184,//PREVIOUS_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN2_PREV_ON,
		IMG_ID_AUDPLY_MAIN_SKIN2_PREV_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN2_PREV_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN2_PREV_DOWN,
		89,184,//NEXT_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN2_NEXT_ON,
		IMG_ID_AUDPLY_MAIN_SKIN2_NEXT_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN2_NEXT_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN2_NEXT_DOWN,
		65,184,//PLAY_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN2_PLAY_ON,
		IMG_ID_AUDPLY_MAIN_SKIN2_PLAY_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN2_PLAY_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN2_PLAY_DOWN,
		IMG_ID_AUDPLY_MAIN_SKIN2_PAUSE_ON,
		IMG_ID_AUDPLY_MAIN_SKIN2_PAUSE_DOWN,
		116,184,//STOP_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN2_STOP_ON,
		IMG_ID_AUDPLY_MAIN_SKIN2_STOP_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN2_STOP_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN2_STOP_DOWN,
#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
		10,66,//ANIMATION x,y
		IMG_ID_AUDPLY_MAIN_SKIN2_ANIMATION, // animation_image_id
#endif
#ifdef __MMI_AUDIO_SPECTRUM_DISPLAY__
		107,162,//SPECTRUM x,y
		57,15,//SPECTRUM w,h
#endif
#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
		0,0,100,//LYRICS Text color
		255,255,0,//LYRICS Border color
		71,51,//LYRICS x,y
		100,16,//LYRICS w,h
#if defined(__MMI_AUDIO_SPECTRUM_DISPLAY__) && defined(__MMI_AUDIO_PLAYER_LYRIC_DISPLAY__)
		0,//exclusive (when exclusive box is checkd)
#endif
#endif
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
		130,203,//REPEAT_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_OFF_UP, // repeat off
		IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_ALL_UP, // repeat all
		IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_ONE_UP, // repeat one
		#ifdef __MMI_TOUCH_SCREEN__
    		IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_OFF_DOWN, // repeat off
		IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_ALL_DOWN, // repeat all
		IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_ONE_DOWN, // repeat one
    		#endif
		110,202,//SHUFFLE_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN2_SHUFFLE_OFF_UP, // shuffle off
		IMG_ID_AUDPLY_MAIN_SKIN2_SHUFFLE_ON_UP, // shuffle on
#endif
#ifdef __MMI_TOUCH_SCREEN__
		2,162,//VOLUME_INCREASE x,y
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_INC_DOWN,// volume_inc_down_image_id
		48,164,//VOLUME_DECREASE x,y
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		83,162,//SETTINGS_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN2_SETTING_UP, // setting_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_SETTING_DOWN, // setting_down_image_id
		{(const audply_point_struct *)skin2_volume_increase_points,4 }, // volume_inc_area
		{(const audply_point_struct *)skin2_volume_decrease_points, 4}, // volume_dec_area
		{(const audply_point_struct *) skin2_previous_button_points, 4},//PREVIOUS_BUTTON area
		{(const audply_point_struct *) skin2_next_button_points, 4},//NEXT_BUTTON area
		{(const audply_point_struct *) skin2_play_button_points, 4},//PLAY_BUTTON area
		{(const audply_point_struct *) skin2_stop_button_points, 4},//STOP_BUTTON area
		{(const audply_point_struct *) skin2_settings_button_points, 4},//SETTINGS_BUTTON area
#endif
		AUDPLY_MAIN_PLAY, AUDPLY_MAIN_STOP, AUDPLY_MAIN_PREV, AUDPLY_MAIN_NEXT // up, down, left, right key mapping
	},
{	//skin3
		IMG_ID_AUDPLY_MAIN_SKIN3_BG, // bg_image_id
		255,255,0,//COMMON_COLOR Border color
		255,255,255,//COMMON_COLOR Text color
		0,0,0,//COMMON_COLOR BackGround color
		39,6,//SONG_TITLE x,y
		110,16,//SONG_TITLE w,h
		46,44,//TIME x,y
		60,15,//TIME w,h
		20,6,//SONG_INDEX x,y
		15,16,//SONG_INDEX w,h
		25,43,//PLAYLIST_ICON x,y
		{IMG_ID_AUDPLY_MAIN_SKIN3_PLAYLIST_PHONE_ICON,
		IMG_ID_AUDPLY_MAIN_SKIN3_PLAYLIST_CARD_ICON},
		109,150,//VOLUME_BAR x,y
		7,
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_INDICATOR,
		29,158,//PREVIOUS_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN3_PREV_ON,
		IMG_ID_AUDPLY_MAIN_SKIN3_PREV_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN3_PREV_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN3_PREV_DOWN,
		76,177,//NEXT_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN3_NEXT_ON,
		IMG_ID_AUDPLY_MAIN_SKIN3_NEXT_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN3_NEXT_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN3_NEXT_DOWN,
		53,141,//PLAY_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN3_PLAY_ON,
		IMG_ID_AUDPLY_MAIN_SKIN3_PLAY_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN3_PLAY_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN3_PLAY_DOWN,
		IMG_ID_AUDPLY_MAIN_SKIN3_PAUSE_ON,
		IMG_ID_AUDPLY_MAIN_SKIN3_PAUSE_DOWN,
		51,174,//STOP_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN3_STOP_ON,
		IMG_ID_AUDPLY_MAIN_SKIN3_STOP_OFF,
		IMG_ID_AUDPLY_MAIN_SKIN3_STOP_DISABLED,
		IMG_ID_AUDPLY_MAIN_SKIN3_STOP_DOWN,
#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
		42,96,//ANIMATION x,y
		IMG_ID_AUDPLY_MAIN_SKIN3_ANIMATION,
#endif
#ifdef __MMI_AUDIO_SPECTRUM_DISPLAY__
		67,60,//SPECTRUM x,y
		50,15,//SPECTRUM w,h
#endif
#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
		0,0,100,//LYRICS Text color
		255,255,0,//LYRICS Border color
		48,80,//LYRICS x,y
		80,16,//LYRICS w,h
#if defined(__MMI_AUDIO_SPECTRUM_DISPLAY__) && defined(__MMI_AUDIO_PLAYER_LYRIC_DISPLAY__)
		0,//exclusive (when exclusive box is checkd)
#endif
#endif
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
		132,63,//REPEAT_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_OFF_UP, // repeat off
		IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_ALL_UP, // repeat all
		IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_ONE_UP, // repeat one
		#ifdef __MMI_TOUCH_SCREEN__
    		IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_OFF_DOWN, // repeat off
		IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_ALL_DOWN, // repeat all
		IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_ONE_DOWN, // repeat one
    		#endif
		133,82,//SHUFFLE_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN3_SHUFFLE_OFF_UP, // shuffle off
		IMG_ID_AUDPLY_MAIN_SKIN3_SHUFFLE_ON_UP, // shuffle on
#endif
#ifdef __MMI_TOUCH_SCREEN__
		138,143,//VOLUME_INCREASE x,y
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_INC_DOWN,// volume_inc_down_image_id
		83,144,//VOLUME_DECREASE x,y
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		121,37,//SETTINGS_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN3_SETTING_UP, // setting_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_SETTING_DOWN, // setting_down_image_id
		{(const audply_point_struct *)skin3_volume_increase_points,4 }, // volume_inc_area
		{(const audply_point_struct *)skin3_volume_decrease_points, 4}, // volume_dec_area
		{(const audply_point_struct *) skin3_previous_button_points, 4},//PREVIOUS_BUTTON area
		{(const audply_point_struct *) skin3_next_button_points, 4},//NEXT_BUTTON area
		{(const audply_point_struct *) skin3_play_button_points, 4},//PLAY_BUTTON area
		{(const audply_point_struct *) skin3_stop_button_points, 4},//STOP_BUTTON area
		{(const audply_point_struct *) skin3_settings_button_points, 4},//SETTINGS_BUTTON area
#endif
		AUDPLY_MAIN_PLAY, AUDPLY_MAIN_STOP, AUDPLY_MAIN_PREV, AUDPLY_MAIN_NEXT // up, down, left, right key mapping
	}
#endif
};
#elif defined __MMI_MAINLCD_128X160__

#ifdef __MMI_TOUCH_SCREEN__
static const audply_point_struct skin1_seek_points[] = {{0,0},{114,0},{114,6},{0,6}};
static const audply_point_struct skin1_volume_inc_points[] = {{100,108},{103,129},{90,150},{79,135},{79,135}};//{{1,0},{12,0},{12,18},{1,18}};
static const audply_point_struct skin1_volume_dec_points[] = {{27,111},{35,127},{51,137},{39,148},{27,134}};//{{0,0},{11,0},{11,18},{0,18}};
static const audply_point_struct skin1_prev_button_points[] = {{35,95},{45,95},{48,100},{48,106},{45,111},{35,111},{32,106},{32,100}};
static const audply_point_struct skin1_next_button_points[] = {{83,95},{91,95},{95,99},{95,106},{91,111},{83,111},{79,106},{79,99}};
static const audply_point_struct skin1_play_button_points[] = {{58,70},{68,70},{71,73},{71,83},{68,86},{58,86},{55,83},{55,73}};
static const audply_point_struct skin1_stop_button_points[] = {{58,120},{68,120},{71,124},{71,130},{68,135},{58,135},{56,130},{56,124}};
static const audply_point_struct skin1_setting_points[] = {{1,1},{25,1},{25,27},{1,27}};
#endif


#if 1//m301wapmms

//greenstone 128*160 screen use this skins define
const audply_skin_struct g_audply_skins[] =
{
	{	//skin1
		IMG_ID_AUDPLY_MAIN_SKIN1_BG,	/* back ground image */
		0, 0, 0,						/* text border color (r, g, b) */
		255, 255, 255,						/* text foreground color (r, g, b) */
		0, 0, 0,						/* text background color (r, g, b) */
		15, 35, 100, 20,					/* title (x, y, w, h) */
		65, 60,	50, 18,					/* time (x, y, w, h) */
		8, 60,	50, 18,					/* file index (x, y, w, h) */
		15, 100,							/* play list icon (x, y) */
		{		
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAYLIST_PHONE_ICON, //PLAYLIST_PHONE_ICON Image
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAYLIST_CARD_ICON //PLAYLIST_CARD_ICON Image
		},
		92, 102, //VOLUME_BAR x,y
		7,//VOLUME_BAR indicator left
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_INDICATOR, // volumebar_ind_image_id
		5, 92,// progressbar_x, progressbar_y
		IMG_ID_AUDPLY_MAIN_SKIN1_PROGRESSBAR, // progressbar_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PROGRESSBAR_FAIL, // progressbar_fail_image_id
		72, 122,	 //PREVIOUS_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_ON, // prev_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_OFF, // prev_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_DISABLED, // prev_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_DOWN, // prev_button_down_image_id
		89, 122, //NEXT_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_ON, // next_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_OFF, // next_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_DISABLED, // next_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_DOWN, // next_button_down_image_id
		46, 118, //PLAY_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_ON, // play_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_OFF, // play_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_DISABLED, // play_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_DOWN, // play_button_down_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PAUSE_ON, // pause_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PAUSE_DOWN, // pause_button_down_image_id
		28, 122, //STOP_BUTTON x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_ON, // stop_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_OFF, // stop_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_DISABLED, // stop_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_DOWN, // stop_button_down_image_id
#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
		0,0, //ANIMATION x,y
		IMG_ID_AUDPLY_MAIN_SKIN1_ANIMATION, // animation_image_id
#endif
#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
		0,0,0, // lyric_fg_color_r,lyric_fg_color_g, lyric_fg_color_b
		255,255,255, // lyric_border_color_r, lyric_border_color_g, lyric_border_color_b
		0,45,//13,93, // lyric_display_x, lyric_display_y
		128,22,//212,94, // lyric_display_w, lyric_display_h
#endif
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
		12,216, // repeat_x, repeat_y
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_OFF_UP, // repeat off
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ALL_UP, // repeat all
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ONE_UP, // repeat one
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ALL_DOWN, // repeat all
        IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ONE_DOWN, // repeat one        
#endif /*__MMI_TOUCH_SCREEN__*/
		46, 84,		 // shuffle_x, shuffle_y
		IMG_ID_AUDPLY_MAIN_SKIN1_SHUFFLE_OFF_UP, // shuffle off
		IMG_ID_AUDPLY_MAIN_SKIN1_SHUFFLE_ON_UP, // shuffle on		
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN1_SHUFFLE_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN1_SHUFFLE_ON_DOWN, // repeat all   
#endif /*__MMI_TOUCH_SCREEN__*/
#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/
#ifdef __MMI_TOUCH_SCREEN__
		112, 98,// volume_inc_x, volume_inc_y,
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_INC_DOWN,// volume_inc_down_image_id
		78, 98,	// volume_dec_x, volume_dec_y,
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		9, 120,// setting_x, setting_y
		IMG_ID_AUDPLY_MAIN_SKIN1_SETTING_UP, // setting_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_SETTING_DOWN, // setting_down_image_id
		{(const audply_point_struct *)skin1_seek_points, 4},
		{(const audply_point_struct *)skin1_volume_inc_points, 4}, // volume_inc_area
		{(const audply_point_struct *)skin1_volume_dec_points, 4}, // volume_dec_area
		{(const audply_point_struct *)skin1_prev_button_points, 4}, // prev_button_area
		{(const audply_point_struct *)skin1_next_button_points, 4}, // next_button_area
		{(const audply_point_struct *)skin1_play_button_points, 4}, // play_button_area
		{(const audply_point_struct *)skin1_stop_button_points, 4}, // stop_button_area
		{(const audply_point_struct *)0, 0}, // setting_area 4
		#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__		
		{(const audply_point_struct *)skin1_repeat_points, 4}, // repeat_area
		{(const audply_point_struct *)skin1_shuffle_points, 4}, // shuffle_area				
		#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/		

#endif
		AUDPLY_MAIN_PLAY, AUDPLY_MAIN_STOP, AUDPLY_MAIN_PREV, AUDPLY_MAIN_NEXT // up, down, left, right key mapping
	}

#else
//greenstone 128*160 screen use this skins define
const audply_skin_struct g_audply_skins[] =
{
	{	//skin1
		IMG_ID_AUDPLY_MAIN_SKIN1_BG, // bg_image_id
		0,0,0, // text_border_color_r, text_border_color_g, text_border_color_b
		0,217,0, // text_fg_color_r, text_fg_color_g, text_fg_color_b
		0,0,0, // text_bg_color_r, text_bg_color_g, text_bg_color_b
		26,31,//25,44,// title_x, title_y
		80,17,// title_w, title_h
		5,60,//40,64,// time_x, time_y
		48,10,// time_w, time_h
		50,60,//35,78, // file_index_x, file_index_y//chenhe modify file_index_x from 45 to 50 for greenstone
		60,10, // file_index_w, file_index_h
		22,64, // playlisticon_x, playlisticon_y
		{IMG_ID_AUDPLY_MAIN_SKIN1_PLAYLIST_PHONE_ICON, IMG_ID_AUDPLY_MAIN_SKIN1_PLAYLIST_CARD_ICON}, // playlisticon_images_id[NO_OF_LIST]
		24,94,5,// volumebar_x, volumebar_y, volumebar_ind_left
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_INDICATOR, // volumebar_ind_image_id
		22,109,//7,105, // prev_button_x, prev_button_y
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_ON, // prev_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_OFF, // prev_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_DISABLED, // prev_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PREV_DOWN, // prev_button_down_image_id
		83,109,//59,105, // next_button_x, next_button_y
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_ON, // next_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_OFF, // next_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_DISABLED, // next_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_NEXT_DOWN, // next_button_down_image_id
		49,106,//25,105, // play_button_x, play_button_y
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_ON, // play_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_OFF, // play_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_DISABLED, // play_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_DOWN, // play_button_down_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PAUSE_ON, // pause_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_PAUSE_DOWN, // pause_button_down_image_id
		55,103,//43,105, // stop_button_x, stop_button_y
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_ON, // stop_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_OFF, // stop_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_DISABLED, // stop_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_STOP_DOWN, // stop_button_down_image_id
#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
		-1,-1, // animation_x, animation_y
		IMG_ID_AUDPLY_MAIN_SKIN1_ANIMATION, // animation_image_id
#endif
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
		0,27, // repeat_x, repeat_y
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_OFF_UP, // repeat off
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ALL_UP, // repeat all
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ONE_UP, // repeat one
		#ifdef __MMI_TOUCH_SCREEN__
    IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_OFF_DOWN, // repeat off
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ALL_DOWN, // repeat all
		IMG_ID_AUDPLY_MAIN_SKIN1_REPEAT_ONE_DOWN, // repeat one
    #endif
		110,27, // shuffle_x, shuffle_y
		IMG_ID_AUDPLY_MAIN_SKIN1_SHUFFLE_OFF_UP, // shuffle off
		IMG_ID_AUDPLY_MAIN_SKIN1_SHUFFLE_ON_UP, // shuffle on
#endif
#ifdef __MMI_TOUCH_SCREEN__
		145,294,// volume_inc_x, volume_inc_y,
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_INC_DOWN,// volume_inc_down_image_id
		86,294,// volume_dec_x, volume_dec_y,
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		8,158,// setting_x, setting_y
		IMG_ID_AUDPLY_MAIN_SKIN1_SETTING_UP, // setting_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN1_SETTING_DOWN, // setting_down_image_id
		{(const audply_point_struct *)skin1_volume_inc_points, 4}, // volume_inc_area
		{(const audply_point_struct *)skin1_volume_dec_points, 4}, // volume_dec_area
		{(const audply_point_struct *)skin1_prev_button_points, 4}, // prev_button_area
		{(const audply_point_struct *)skin1_next_button_points, 4}, // next_button_area
		{(const audply_point_struct *)skin1_play_button_points, 4}, // play_button_area
		{(const audply_point_struct *)skin1_stop_button_points, 4}, // stop_button_area
		{(const audply_point_struct *)0, 0}, // setting_area 4
#endif
		AUDPLY_MAIN_PLAY, AUDPLY_MAIN_STOP, AUDPLY_MAIN_PREV, AUDPLY_MAIN_NEXT // up, down, left, right key mapping
	},
	#endif
#ifndef __MMI_SLIM_AUDIO_PLAYER__	
{	//skin2
		IMG_ID_AUDPLY_MAIN_SKIN2_BG, // bg_image_id
		0,0,0, // text_border_color_r, text_border_color_g, text_border_color_b
		0,217,0, // text_fg_color_r, text_fg_color_g, text_fg_color_b
		0,0,0, // text_bg_color_r, text_bg_color_g, text_bg_color_b
		26,37,//25,44,// title_x, title_y
		80,17,// title_w, title_h
		5,60,//40,64,// time_x, time_y
		48,10,// time_w, time_h
		50,60,//35,78, // file_index_x, file_index_y//chenhe modify file_index_x from 45 to 50 for greenstone
		60,10, // file_index_w, file_index_h
		22,64, // playlisticon_x, playlisticon_y
		{IMG_ID_AUDPLY_MAIN_SKIN2_PLAYLIST_PHONE_ICON, IMG_ID_AUDPLY_MAIN_SKIN2_PLAYLIST_CARD_ICON}, // playlisticon_images_id[NO_OF_LIST]
		48,92,5,//99,107,5,// volumebar_x, volumebar_y, volumebar_ind_left
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_INDICATOR, // volumebar_ind_image_id
		16,194,// progressbar_x, progressbar_y
		IMG_ID_AUDPLY_MAIN_SKIN2_PROGRESSBAR, // progressbar_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PROGRESSBAR_FAIL, // progressbar_fail_image_id
		34,116,//7,105, // prev_button_x, prev_button_y
		IMG_ID_AUDPLY_MAIN_SKIN2_PREV_ON, // prev_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PREV_OFF, // prev_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PREV_DISABLED, // prev_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PREV_DOWN, // prev_button_down_image_id
		74,116,//59,105, // next_button_x, next_button_y
		IMG_ID_AUDPLY_MAIN_SKIN2_NEXT_ON, // next_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_NEXT_OFF, // next_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_NEXT_DISABLED, // next_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_NEXT_DOWN, // next_button_down_image_id
		54,104,//25,105, // play_button_x, play_button_y
		IMG_ID_AUDPLY_MAIN_SKIN2_PLAY_ON, // play_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PLAY_OFF, // play_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PLAY_DISABLED, // play_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PLAY_DOWN, // play_button_down_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PAUSE_ON, // pause_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_PAUSE_DOWN, // pause_button_down_image_id
		54,124,//43,105, // stop_button_x, stop_button_y
		IMG_ID_AUDPLY_MAIN_SKIN2_STOP_ON, // stop_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_STOP_OFF, // stop_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_STOP_DISABLED, // stop_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_STOP_DOWN, // stop_button_down_image_id
#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
		-1,-1, // animation_x, animation_y
		IMG_ID_AUDPLY_MAIN_SKIN2_ANIMATION, // animation_image_id
#endif
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
		12,216, // repeat_x, repeat_y
		IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_OFF_UP, // repeat off
		IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_ALL_UP, // repeat all
		IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_ONE_UP, // repeat one
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_ALL_DOWN, // repeat all
        IMG_ID_AUDPLY_MAIN_SKIN2_REPEAT_ONE_DOWN, // repeat one        
#endif /*__MMI_TOUCH_SCREEN__*/
		39,216, // shuffle_x, shuffle_y
		IMG_ID_AUDPLY_MAIN_SKIN2_SHUFFLE_OFF_UP, // shuffle off
		IMG_ID_AUDPLY_MAIN_SKIN2_SHUFFLE_ON_UP, // shuffle on		
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN2_SHUFFLE_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN2_SHUFFLE_ON_DOWN, // repeat all   
#endif /*__MMI_TOUCH_SCREEN__*/
#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/
#ifdef __MMI_TOUCH_SCREEN__
		147,200,// volume_inc_x, volume_inc_y,
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_INC_DOWN,// volume_inc_down_image_id
		84,200,// volume_dec_x, volume_dec_y,
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		8,158,// setting_x, setting_y
		IMG_ID_AUDPLY_MAIN_SKIN2_SETTING_UP, // setting_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN2_SETTING_DOWN, // setting_down_image_id
		{(const audply_point_struct *)skin2_seek_points, 4},
		{(const audply_point_struct *)skin2_volume_inc_points, 4}, // volume_inc_area
		{(const audply_point_struct *)skin2_volume_dec_points, 4}, // volume_dec_area
		{(const audply_point_struct *)skin2_prev_button_points, 4}, // prev_button_area
		{(const audply_point_struct *)skin2_next_button_points, 4}, // next_button_area
		{(const audply_point_struct *)skin2_play_button_points, 4}, // play_button_area
		{(const audply_point_struct *)skin2_stop_button_points, 4}, // stop_button_area
		{(const audply_point_struct *)0, 0}, // setting_area 4
		#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__		
		{(const audply_point_struct *)skin1_repeat_points, 4}, // repeat_area
		{(const audply_point_struct *)skin1_shuffle_points, 4}, // shuffle_area				
		#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/		

#endif
		AUDPLY_MAIN_PLAY, AUDPLY_MAIN_STOP, AUDPLY_MAIN_PREV, AUDPLY_MAIN_NEXT // up, down, left, right key mapping
	},
{	//skin3
		IMG_ID_AUDPLY_MAIN_SKIN3_BG, // bg_image_id
		0,0,0, // text_border_color_r, text_border_color_g, text_border_color_b
		0,217,0, // text_fg_color_r, text_fg_color_g, text_fg_color_b
		0,0,0, // text_bg_color_r, text_bg_color_g, text_bg_color_b
		26,31,//25,44,// title_x, title_y
		80,17,// title_w, title_h
		5,60,//40,64,// time_x, time_y
		48,10,// time_w, time_h
		50,60,//35,78, // file_index_x, file_index_y//chenhe modify file_index_x from 45 to 50 for greenstone
		60,10, // file_index_w, file_index_h
		22,64, // playlisticon_x, playlisticon_y
		{IMG_ID_AUDPLY_MAIN_SKIN3_PLAYLIST_PHONE_ICON, IMG_ID_AUDPLY_MAIN_SKIN3_PLAYLIST_CARD_ICON}, // playlisticon_images_id[NO_OF_LIST]
		48,80,5,//99,107,5,// volumebar_x, volumebar_y, volumebar_ind_left
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_INDICATOR, // volumebar_ind_image_id
		16,194,// progressbar_x, progressbar_y
		IMG_ID_AUDPLY_MAIN_SKIN3_PROGRESSBAR, // progressbar_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PROGRESSBAR_FAIL, // progressbar_fail_image_id
		33,114,//7,105, // prev_button_x, prev_button_y
		IMG_ID_AUDPLY_MAIN_SKIN3_PREV_ON, // prev_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PREV_OFF, // prev_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PREV_DISABLED, // prev_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PREV_DOWN, // prev_button_down_image_id
		74,114,//59,105, // next_button_x, next_button_y
		IMG_ID_AUDPLY_MAIN_SKIN3_NEXT_ON, // next_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_NEXT_OFF, // next_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_NEXT_DISABLED, // next_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_NEXT_DOWN, // next_button_down_image_id
		54,101,//25,105, // play_button_x, play_button_y
		IMG_ID_AUDPLY_MAIN_SKIN3_PLAY_ON, // play_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PLAY_OFF, // play_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PLAY_DISABLED, // play_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PLAY_DOWN, // play_button_down_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PAUSE_ON, // pause_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_PAUSE_DOWN, // pause_button_down_image_id
		54,120,//43,105, // stop_button_x, stop_button_y
		IMG_ID_AUDPLY_MAIN_SKIN3_STOP_ON, // stop_button_on_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_STOP_OFF, // stop_button_off_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_STOP_DISABLED, // stop_button_disabled_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_STOP_DOWN, // stop_button_down_image_id
#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
		-1,-1, // animation_x, animation_y
		IMG_ID_AUDPLY_MAIN_SKIN3_ANIMATION, // animation_image_id
#endif
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
		25,216, // repeat_x, repeat_y
		IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_OFF_UP, // repeat off
		IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_ALL_UP, // repeat all
		IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_ONE_UP, // repeat one
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_ALL_DOWN, // repeat all
        IMG_ID_AUDPLY_MAIN_SKIN3_REPEAT_ONE_DOWN, // repeat one        
#endif /*__MMI_TOUCH_SCREEN__*/
		39,216, // shuffle_x, shuffle_y
		IMG_ID_AUDPLY_MAIN_SKIN3_SHUFFLE_OFF_UP, // shuffle off
		IMG_ID_AUDPLY_MAIN_SKIN3_SHUFFLE_ON_UP, // shuffle on		
#ifdef __MMI_TOUCH_SCREEN__
        IMG_ID_AUDPLY_MAIN_SKIN3_SHUFFLE_OFF_DOWN, // repeat off
        IMG_ID_AUDPLY_MAIN_SKIN3_SHUFFLE_ON_DOWN, // repeat all   
#endif /*__MMI_TOUCH_SCREEN__*/
#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/
#ifdef __MMI_TOUCH_SCREEN__
		147,200,// volume_inc_x, volume_inc_y,
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_INC_DOWN,// volume_inc_down_image_id
		84,200,// volume_dec_x, volume_dec_y,
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		8,158,// setting_x, setting_y
		IMG_ID_AUDPLY_MAIN_SKIN3_SETTING_UP, // setting_up_image_id
		IMG_ID_AUDPLY_MAIN_SKIN3_SETTING_DOWN, // setting_down_image_id
		{(const audply_point_struct *)skin3_seek_points, 4},
		{(const audply_point_struct *)skin3_volume_inc_points, 4}, // volume_inc_area
		{(const audply_point_struct *)skin3_volume_dec_points, 4}, // volume_dec_area
		{(const audply_point_struct *)skin3_prev_button_points, 4}, // prev_button_area
		{(const audply_point_struct *)skin3_next_button_points, 4}, // next_button_area
		{(const audply_point_struct *)skin3_play_button_points, 4}, // play_button_area
		{(const audply_point_struct *)skin3_stop_button_points, 4}, // stop_button_area
		{(const audply_point_struct *)0, 0}, // setting_area 4
		#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__		
		{(const audply_point_struct *)skin1_repeat_points, 4}, // repeat_area
		{(const audply_point_struct *)skin1_shuffle_points, 4}, // shuffle_area				
		#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/		

#endif
		AUDPLY_MAIN_PLAY, AUDPLY_MAIN_STOP, AUDPLY_MAIN_PREV, AUDPLY_MAIN_NEXT // up, down, left, right key mapping
	}
#endif
};
#else
#error "MAINLCD definition is not defined"
#endif
const audply_skin_struct g_mp4ply_skins[] =
{
{0}
};

#endif
