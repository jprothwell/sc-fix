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


#include "mmi_features.h"

#ifdef __MMI_AUDIO_PLAYER__

#define __NEWSIMULATOR 
#include "wgui_categories.h"

// dependent headers for AudioPlayerXXX.h"
#include "gui_data_types.h"
#include "frameworkstruct.h"
#include "globalconstants.h"
#include "mdi_datatype.h"
#include "kal_release.h"
#include "conversions.h"
#include "filemgr.h"
#include "gdi_include.h"


#include "audioplayerdef.h"
#include "audioplayertype.h"
#include "audioplayerprot.h"
#include "audioplayermainscreen.h"

 
#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h"
#endif

#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "custom_nvram_editor_data_item.h"
 
#undef __NEWSIMULATOR 
#include "med_struct.h"
#include "l1audio.h"
#include "aud_main.h"

static BOOL 					init_images_done = FALSE;
static BOOL						init_scrolling_done = FALSE;
signed char						g_audply_main_selected_button;
BOOL							g_audply_main_selected_button_down;
static color_t					text_border_color;
static color_t					text_bg_color;
static color_t 					text_fg_color;
UI_image_type					audply_main_bg_img;
UI_image_type					audply_main_pause_on_img;
UI_image_type					audply_main_pause_down_img;
audply_main_title_struct		audply_main_title;
audply_main_time_struct 		audply_main_time;
audply_main_file_index_struct 	audply_main_file_index;
audply_main_playlisticon_struct	audply_main_playlisticon;
audply_main_volumebar_struct	audply_main_volumebar;
audply_main_progressbar_struct audply_main_progressbar;
#if defined(__PROJECT_GALLITE_C01__)
audply_main_volumectrl_struct	audply_main_volume_inc, audply_main_volume_dec;
#endif
#ifdef __MMI_TOUCH_SCREEN__
audply_main_volumectrl_struct	audply_main_volume_inc, audply_main_volume_dec;
audply_main_setting_struct		audply_main_setting;
#endif
#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
audply_main_animation_struct	audply_main_animation;
#endif
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
audply_main_repeat_struct		audply_main_repeat;
audply_main_shuffle_struct		audply_main_shuffle;
#endif
audply_main_button_struct		audply_main_buttons[NO_OF_AUDPLY_MAIN_BUTTONS];
S16								audply_main_buttons_x1;
S16								audply_main_buttons_y1;
S16								audply_main_buttons_x2; 
S16								audply_main_buttons_y2;
U8								audply_main_skin_index;
scrolling_text					audply_main_scrolling_title;
UI_string_type					audply_main_LSK_text;
UI_string_type					audply_main_RSK_text;

#ifdef __GDI_MEMORY_PROFILE_2__
GDI_HANDLE					audply_osd_layer;
GDI_HANDLE					audply_skin_layer;
BOOL						is_audply_skin_layer_drawn = FALSE;
#endif

extern audply_struct		g_audply; // from AudioPlayerSrc.c
extern const audply_skin_struct	g_audply_skins[NO_OF_SKIN]; // from AudioPlayerSrc.c
extern UI_string_type 		MMI_title_string; // from wgui_categories.c
extern S32					MMI_title_height; // from wgui_categories.c
extern S32					UI_device_width;  // from gui_wrapper.c
extern S32					UI_device_height; // from gui_wrapper.c

extern BOOL g_mmi_audply_init_done; // from AudioPlayerSrc.c
extern BOOL r2lMMIFlag; // definedi n gui_inputs.c

// sub LCD
#ifdef __MMI_SUBLCD__
scrolling_text audply_sub_LCD_scrolling_title;
scrolling_text* audply_sub_LCD_scrolling_title_p = NULL;
extern bitmap *current_LCD_device_bitmap; // defined in wgui.c
extern bitmap main_LCD_device_bitmap; // defined in wingui.c
extern bitmap sub_LCD_device_bitmap; // defined in wingui.c
#endif /* __MMI_SUBLCD__ */

#define AUDPLY_LSK_X_OFFSET	7
#define AUDPLY_LSK_Y_OFFSET	2
#define AUDPLY_RSK_X_OFFSET	7
#define AUDPLY_RSK_Y_OFFSET	2
#ifdef __MMI_TOUCH_SCREEN__
 
typedef enum
{
	AUDPLY_PEN_NONE,
	AUDPLY_PEN_LSK,
	AUDPLY_PEN_RSK,
	AUDPLY_PEN_SEEK,
	AUDPLY_PEN_MAIN_PREV,
	AUDPLY_PEN_MAIN_NEXT,
	AUDPLY_PEN_MAIN_STOP,
	AUDPLY_PEN_MAIN_PLAY,
	AUDPLY_PEN_MAIN_SETTING,
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__    
      AUDPLY_PEN_MAIN_REPEAT,
      AUDPLY_PEN_MAIN_SHUFFLE,
#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/
	AUDPLY_PEN_MAIN_VOLUME_INC,
	AUDPLY_PEN_MAIN_VOLUME_DEC,
	AUDPLY_PEN_COUNT
} AUDPLY_PEN_STATE;
typedef enum
{
	AUDPLY_PEN_SK_UP,
	AUDPLY_PEN_SK_DOWN
} AUDPLY_SOFTKEY_STATE;

S32 audply_pen_event_on_object = AUDPLY_PEN_NONE;
static mmi_pen_point_struct LSK_points[4];
static mmi_pen_point_struct RSK_points[4];
static mmi_pen_polygon_area_struct LSK_area;
static mmi_pen_polygon_area_struct RSK_area;
 
#endif

#if defined(__MMI_AUDIO_PLAYER_LYRIC_DISPLAY__)
extern audply_lyric_struct audply_lyric;
extern audply_lyric_printing_struct audply_lyric_print;
extern MMI_BOOL lyric_reEntry;
extern MMI_BOOL lyric_parse_ok;
#endif
#if (defined(__MMI_MAINLCD_176X220__) && defined(__MMI_AUDIO_PLAYER_LYRIC_DISPLAY__))
#define AUDPLY_TITLE_WIDTH_FOR_176X220 86
#define AUDPLY_TITLE_AREA_BEGIN_X 48
#define AUDPLY_TITLE_AREA_BEGIN_Y 23
#endif
/*****************************************************************************
* FUNCTION
*     mmi_audply_init_main_screen_images
* DESCRIPTION
*     init audio player main screen images
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_init_main_screen_images( S8 skin )
{
	UINT8 skinIndex = skin;
	const audply_skin_struct *skin_p = &g_audply_skins[skinIndex];
	S32 i, w, h;
	
	// bg
	audply_main_bg_img = (UI_image_type) GetImage( skin_p->bg_image_id );
	
	// text_border_color, text fg, bg color
	text_border_color = coolsand_UI_color( skin_p->text_border_color_r, skin_p->text_border_color_g, skin_p->text_border_color_b );
	text_fg_color = coolsand_UI_color( skin_p->text_fg_color_r, skin_p->text_fg_color_g, skin_p->text_fg_color_b );
#ifdef __GDI_MEMORY_PROFILE_2__
	text_bg_color = coolsand_UI_color( 0, 0, 255 ); // transparent
#else
	text_bg_color = coolsand_UI_color( skin_p->text_bg_color_r, skin_p->text_bg_color_g, skin_p->text_bg_color_b );
#endif
	
	// title
	audply_main_title.x = skin_p->title_x;
	audply_main_title.y = skin_p->title_y;
	audply_main_title.w = skin_p->title_w;
	audply_main_title.h = skin_p->title_h;
	
	// time
	audply_main_time.x = skin_p->time_x;
	audply_main_time.y = skin_p->time_y;
	audply_main_time.w = skin_p->time_w;
	audply_main_time.h = skin_p->time_h;
	
	// file index
	audply_main_file_index.x = skin_p->file_index_x;
	audply_main_file_index.y = skin_p->file_index_y;
	audply_main_file_index.w = skin_p->file_index_w;
	audply_main_file_index.h = skin_p->file_index_h;
	
	// play list icon
	audply_main_playlisticon.x = skin_p->playlisticon_x;
	audply_main_playlisticon.y = skin_p->playlisticon_y;
	audply_main_playlisticon.list_icon[LIST_FROM_PHONE] = (UI_image_type) GetImage( skin_p->playlisticon_images_id[LIST_FROM_PHONE] );
	audply_main_playlisticon.list_icon[LIST_FROM_CARD] = (UI_image_type) GetImage( skin_p->playlisticon_images_id[LIST_FROM_CARD] );
	coolsand_UI_measure_image( audply_main_playlisticon.list_icon[LIST_FROM_PHONE], &w, &h );
	audply_main_playlisticon.w = (S16) w;
	audply_main_playlisticon.h = (S16) h;
	
	// volume
	audply_main_volumebar.x = skin_p->volumebar_x;
	audply_main_volumebar.y = skin_p->volumebar_y;
	audply_main_volumebar.ind_left = skin_p->volumebar_ind_left;
	audply_main_volumebar.ind_img = (UI_image_type) GetImage( skin_p->volumebar_ind_image_id );
	coolsand_UI_measure_image( audply_main_volumebar.ind_img, &w, &h );
	audply_main_volumebar.w = (S16) w;
	audply_main_volumebar.h = (S16) h;

	 /* progress bar */
	    audply_main_progressbar.x = skin_p->progressbar_x;
	    audply_main_progressbar.y = skin_p->progressbar_y;
	    audply_main_progressbar.img = (PU8) GetImage(skin_p->progressbar_image_id);
	    audply_main_progressbar.img_fail = (PU8) GetImage(skin_p->progressbar_fail_image_id);
	    coolsand_UI_measure_image(audply_main_progressbar.img, &w, &h);
#if defined(__MMI_NOKIA_STYLE_N800__)
		w += 16;					//for image audply_main_progressbar.img's width is too small.
#endif
	    audply_main_progressbar.w = (S16) w;
	    audply_main_progressbar.h = (S16) h;
           audply_main_progressbar.PlayProgress = 0;
	
#ifdef __MMI_TOUCH_SCREEN__
	// volume inc & dec control
	audply_main_volume_inc.x = skin_p->volume_inc_x;
	audply_main_volume_inc.y = skin_p->volume_inc_y;
	audply_main_volume_inc.up_img = (UI_image_type) GetImage( skin_p->volume_inc_up_image_id );
	audply_main_volume_inc.down_img = (UI_image_type) GetImage( skin_p->volume_inc_down_image_id );
	coolsand_UI_measure_image( audply_main_volume_inc.up_img, &w, &h );
	audply_main_volume_inc.w = (S16) w;
	audply_main_volume_inc.h = (S16) h;
	audply_main_volume_inc.down = FALSE;

	audply_main_volume_dec.x = skin_p->volume_dec_x;
	audply_main_volume_dec.y = skin_p->volume_dec_y;
	audply_main_volume_dec.up_img = (UI_image_type) GetImage( skin_p->volume_dec_up_image_id );
	audply_main_volume_dec.down_img = (UI_image_type) GetImage( skin_p->volume_dec_down_image_id );
	coolsand_UI_measure_image( audply_main_volume_dec.up_img, &w, &h );
	audply_main_volume_dec.w = (S16) w;
	audply_main_volume_dec.h = (S16) h;
	audply_main_volume_dec.down = FALSE;

	// setting icon
	audply_main_setting.x = skin_p->setting_x;
	audply_main_setting.y = skin_p->setting_y;
	audply_main_setting.up_img = (UI_image_type) GetImage( skin_p->setting_up_image_id );
	audply_main_setting.down_img = (UI_image_type) GetImage( skin_p->setting_down_image_id );
	coolsand_UI_measure_image( audply_main_setting.up_img, &w, &h );
	audply_main_setting.w = (S16) w;
	audply_main_setting.h = (S16) h;
	audply_main_setting.down = FALSE;
#endif
	// play, pause, stop, prev, next buttons
	audply_main_buttons[AUDPLY_MAIN_PREV].x = skin_p->prev_button_x;
	audply_main_buttons[AUDPLY_MAIN_PREV].y = skin_p->prev_button_y;
	audply_main_buttons[AUDPLY_MAIN_PREV].on_img = (UI_image_type) GetImage( skin_p->prev_button_on_image_id );
	audply_main_buttons[AUDPLY_MAIN_PREV].off_img = (UI_image_type) GetImage( skin_p->prev_button_off_image_id );
	audply_main_buttons[AUDPLY_MAIN_PREV].disabled_img = (UI_image_type) GetImage( skin_p->prev_button_disabled_image_id );
	audply_main_buttons[AUDPLY_MAIN_PREV].down_img = (UI_image_type) GetImage( skin_p->prev_button_down_image_id );

	audply_main_buttons[AUDPLY_MAIN_NEXT].x = skin_p->next_button_x;
	audply_main_buttons[AUDPLY_MAIN_NEXT].y = skin_p->next_button_y;
	audply_main_buttons[AUDPLY_MAIN_NEXT].on_img = (UI_image_type) GetImage( skin_p->next_button_on_image_id );
	audply_main_buttons[AUDPLY_MAIN_NEXT].off_img = (UI_image_type) GetImage( skin_p->next_button_off_image_id );
	audply_main_buttons[AUDPLY_MAIN_NEXT].disabled_img = (UI_image_type) GetImage( skin_p->next_button_disabled_image_id );
	audply_main_buttons[AUDPLY_MAIN_NEXT].down_img = (UI_image_type) GetImage( skin_p->next_button_down_image_id );

	audply_main_buttons[AUDPLY_MAIN_PLAY].x = skin_p->play_button_x;
	audply_main_buttons[AUDPLY_MAIN_PLAY].y = skin_p->play_button_y;
	audply_main_buttons[AUDPLY_MAIN_PLAY].on_img = (UI_image_type) GetImage( skin_p->play_button_on_image_id );
	audply_main_buttons[AUDPLY_MAIN_PLAY].off_img = (UI_image_type) GetImage( skin_p->play_button_off_image_id );
	audply_main_buttons[AUDPLY_MAIN_PLAY].disabled_img = (UI_image_type) GetImage( skin_p->play_button_disabled_image_id );
	audply_main_buttons[AUDPLY_MAIN_PLAY].down_img = (UI_image_type) GetImage( skin_p->play_button_down_image_id );
	audply_main_pause_on_img = (UI_image_type) GetImage( skin_p->pause_button_on_image_id );
	audply_main_pause_down_img = (UI_image_type) GetImage( skin_p->pause_button_down_image_id );

	audply_main_buttons[AUDPLY_MAIN_STOP].x = skin_p->stop_button_x;
	audply_main_buttons[AUDPLY_MAIN_STOP].y = skin_p->stop_button_y;
	audply_main_buttons[AUDPLY_MAIN_STOP].on_img = (UI_image_type) GetImage( skin_p->stop_button_on_image_id );
	audply_main_buttons[AUDPLY_MAIN_STOP].off_img = (UI_image_type) GetImage( skin_p->stop_button_off_image_id );
	audply_main_buttons[AUDPLY_MAIN_STOP].disabled_img = (UI_image_type) GetImage( skin_p->stop_button_disabled_image_id );
	audply_main_buttons[AUDPLY_MAIN_STOP].down_img = (UI_image_type) GetImage( skin_p->stop_button_down_image_id );

//volume inc,dec buttons
#if defined(__PROJECT_GALLITE_C01__)
	// volume inc & dec control
	audply_main_volume_inc.x = skin_p->volume_inc_x;
	audply_main_volume_inc.y = skin_p->volume_inc_y;
	audply_main_volume_inc.up_img = (UI_image_type) GetImage( skin_p->volume_inc_up_image_id );
	audply_main_volume_inc.down_img = (UI_image_type) GetImage( skin_p->volume_inc_down_image_id );
	coolsand_UI_measure_image( audply_main_volume_inc.up_img, &w, &h );
	audply_main_volume_inc.w = (S16) w;
	audply_main_volume_inc.h = (S16) h;
	audply_main_volume_inc.down = FALSE;

	audply_main_volume_dec.x = skin_p->volume_dec_x;
	audply_main_volume_dec.y = skin_p->volume_dec_y;
	audply_main_volume_dec.up_img = (UI_image_type) GetImage( skin_p->volume_dec_up_image_id );
	audply_main_volume_dec.down_img = (UI_image_type) GetImage( skin_p->volume_dec_down_image_id );
	coolsand_UI_measure_image( audply_main_volume_dec.up_img, &w, &h );
	audply_main_volume_dec.w = (S16) w;
	audply_main_volume_dec.h = (S16) h;
	audply_main_volume_dec.down = FALSE;
#endif

	// calculate buttons boundary
	coolsand_UI_measure_image( audply_main_buttons[0].on_img, &w, &h );
	audply_main_buttons[0].w = (S16) w;
	audply_main_buttons[0].h = (S16) h;
	audply_main_buttons_x1 = audply_main_buttons[0].x;
	audply_main_buttons_y1 = audply_main_buttons[0].y;
	audply_main_buttons_x2 = audply_main_buttons[0].x + w - 1;
	audply_main_buttons_y2 = audply_main_buttons[0].y + h - 1;
	for( i = 1; i < NO_OF_AUDPLY_MAIN_BUTTONS; i++ )
	{
		coolsand_UI_measure_image( audply_main_buttons[i].on_img, &w, &h );
		audply_main_buttons[i].w = (S16) w;
		audply_main_buttons[i].h = (S16) h;
		if( audply_main_buttons[i].x < audply_main_buttons_x1 )
			audply_main_buttons_x1 = audply_main_buttons[i].x;
		if( audply_main_buttons[i].y < audply_main_buttons_y1 )
			audply_main_buttons_y1 = audply_main_buttons[i].y;
		if( audply_main_buttons[i].x + w - 1 > audply_main_buttons_x2 )
			audply_main_buttons_x2 = audply_main_buttons[i].x + w - 1;
		if( audply_main_buttons[i].y + h - 1 > audply_main_buttons_y2 )
			audply_main_buttons_y2 = audply_main_buttons[i].y + h - 1;
	}

#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
	// animation
	audply_main_animation.x = skin_p->animation_x;
	audply_main_animation.y = skin_p->animation_y;
	audply_main_animation.animation_img = (UI_image_type) GetImage( skin_p->animation_image_id );
	coolsand_UI_measure_image( audply_main_animation.animation_img, &w, &h );
	audply_main_animation.w = (S16) w;
	audply_main_animation.h = (S16) h;
#endif

#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
    /* repeat */
    audply_main_repeat.x = skin_p->repeat_x;
    audply_main_repeat.y = skin_p->repeat_y;
    audply_main_repeat.off_up_img = (PU8) GetImage(skin_p->repeat_off_up_image_id);
    audply_main_repeat.all_up_img = (PU8) GetImage(skin_p->repeat_all_up_image_id);
    audply_main_repeat.one_up_img = (PU8) GetImage(skin_p->repeat_one_up_image_id);
#ifdef __MMI_TOUCH_SCREEN__
    audply_main_repeat.off_down_img = (PU8) GetImage(skin_p->repeat_off_down_image_id);
    audply_main_repeat.all_down_img = (PU8) GetImage(skin_p->repeat_all_down_image_id);
    audply_main_repeat.one_down_img = (PU8) GetImage(skin_p->repeat_one_down_image_id);
    audply_main_repeat.down = MMI_FALSE;
#endif /*__MMI_TOUCH_SCREEN__*/
    coolsand_UI_measure_image(audply_main_repeat.off_up_img, &w, &h);
    audply_main_repeat.w = (S16) w;
    audply_main_repeat.h = (S16) h;
    /* shuffle */
    audply_main_shuffle.x = skin_p->shuffle_x;
    audply_main_shuffle.y = skin_p->shuffle_y;
    audply_main_shuffle.off_up_img = (PU8) GetImage(skin_p->shuffle_off_up_image_id);
    audply_main_shuffle.on_up_img = (PU8) GetImage(skin_p->shuffle_on_up_image_id);    
#ifdef __MMI_TOUCH_SCREEN__
    audply_main_shuffle.off_down_img = (PU8) GetImage(skin_p->shuffle_off_down_image_id);
    audply_main_shuffle.on_down_img = (PU8) GetImage(skin_p->shuffle_on_down_image_id);
    audply_main_shuffle.down = MMI_FALSE;
#endif /*__MMI_TOUCH_SCREEN__*/
    coolsand_UI_measure_image(audply_main_shuffle.off_up_img, &w, &h);
    audply_main_shuffle.w = (S16) w;
    audply_main_shuffle.h = (S16) h;
#endif /* __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__ */ 

#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
    audply_lyric_print.start_x = skin_p->lyric_display_x;
    audply_lyric_print.start_y = skin_p->lyric_display_y;
    audply_lyric_print.width = skin_p->lyric_display_w;
    audply_lyric_print.height = skin_p->lyric_display_h;
    if(audply_lyric_print.start_x >= 0)
    {
        audply_lyric_print.lyric_fg_color = gui_color((U8)skin_p->lyric_fg_color_r, (U8)skin_p->lyric_fg_color_g, (U8)skin_p->lyric_fg_color_b);
        audply_lyric_print.lyric_border_color = gui_color((U8)skin_p->lyric_border_color_r, (U8)skin_p->lyric_border_color_g, (U8)skin_p->lyric_border_color_b);
    }
#endif /* __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__ */ 
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_redraw_subLCD_background
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     S32 x1
*     S32 y1
*     S32 x2
*     S32 y2
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_redraw_subLCD_background( S32 x1, S32 y1, S32 x2, S32 y2 )
{
#ifdef __MMI_SUBLCD__
	color_t c = coolsand_UI_color( 255, 255, 255 );
	coolsand_UI_fill_rectangle( x1, y1, x2, y2, c);
#endif // __MMI_SUBLCD__
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_subLCD_text_scroll_handler
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_subLCD_text_scroll_handler( void )
{
#ifdef __MMI_SUBLCD__
	coolsand_UI_handle_scrolling_text( &audply_sub_LCD_scrolling_title );
#endif // __MMI_SUBLCD__
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_show_subLCD
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     BOOL is_show
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_redraw_subLCD( BOOL is_show )
{
#ifdef __MMI_SUBLCD__
	#ifdef __MMI_SUB_WALLPAPER__
	color_t border_color = coolsand_UI_color( 0, 0, 0 );
	color_t text_color = coolsand_UI_color( 255, 255, 255 );
	#else
	color_t text_color = coolsand_UI_color( 0, 0, 0 );
	#endif
	S32 x1, y1, x2, y2, x, y, str_w, str_h;
	bitmap *saved_graphics_context = current_LCD_device_bitmap;

	if( !g_mmi_audply_init_done )
		return;

	mmi_audply_set_subLCD_shown( TRUE );
	
	UI_set_sub_LCD_graphics_context();

	coolsand_UI_lock_double_buffer();

	if( audply_sub_LCD_scrolling_title_p != NULL )
	{
		coolsand_UI_scrolling_text_stop( audply_sub_LCD_scrolling_title_p );
		audply_sub_LCD_scrolling_title_p = NULL;
	}

	// clear background
	x1 = 0;
	y1 = 13;
	x2 = UI_device_width - 1;
	y2 = UI_device_height - 1;
	coolsand_UI_push_clip();
	coolsand_UI_set_clip( x1, y1, x2, y2 );
	mmi_audply_redraw_subLCD_background( x1, y1, x2, y2 );

	if( is_show )
	{
		// draw song name
		coolsand_UI_set_font( &MMI_medium_font );
		coolsand_UI_measure_string( g_audply.title, &str_w, &str_h );
#ifdef __MMI_SUB_WALLPAPER__
		str_w += 1;
		str_h += 2;
#endif // __MMI_SUB_WALLPAPER__
		x = (UI_device_width - str_w)/2;
		y = 12 + (UI_device_height - 12 - str_h)/2;
		if( x >= 0 )
		{
			coolsand_UI_push_text_clip();
			coolsand_UI_set_text_color( text_color );
		#ifdef __MMI_SUB_WALLPAPER__
			coolsand_UI_set_text_border_color( border_color );
		#endif
			if( r2lMMIFlag )
				coolsand_UI_move_text_cursor( UI_device_width - x - 2, y );
			else
				coolsand_UI_move_text_cursor( x, y );
		#ifdef __MMI_SUB_WALLPAPER__
			coolsand_UI_set_text_clip( x, y-1, x + str_w - 1, y + str_h - 1 );
			coolsand_UI_print_bordered_text( g_audply.title );
		#else
			coolsand_UI_set_text_clip( x, y, x + str_w - 1, y + str_h - 1 );
			coolsand_UI_print_text( g_audply.title );
		#endif
			coolsand_UI_pop_text_clip();
		}
		else
		{
			// scroll
			x = 0;
			audply_sub_LCD_scrolling_title_p = &audply_sub_LCD_scrolling_title;
			coolsand_UI_create_scrolling_text( audply_sub_LCD_scrolling_title_p,
				x + 2, y, UI_device_width - 4, str_h, g_audply.title,
				mmi_audply_subLCD_text_scroll_handler, mmi_audply_redraw_subLCD_background,
				text_color, 
			#ifdef __MMI_SUB_WALLPAPER__
				border_color
			#else
				text_color 
			#endif
				);
 
			#ifdef __MMI_SUB_WALLPAPER__
			audply_sub_LCD_scrolling_title_p->flags |= UI_SCROLLING_TEXT_BORDERED_TEXT;
			#endif
 
			audply_sub_LCD_scrolling_title_p->text_font = &MMI_medium_font;
			coolsand_UI_show_scrolling_text( audply_sub_LCD_scrolling_title_p );
		}
	}
	

	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1, x2, y2 );

	coolsand_UI_pop_clip();

	if( saved_graphics_context == &main_LCD_device_bitmap )
		UI_set_main_LCD_graphics_context();
	else if( saved_graphics_context == &sub_LCD_device_bitmap )
		UI_set_sub_LCD_graphics_context();
#endif // __MMI_SUBLCD__
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_exit_subLCD
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_exit_subLCD()
{
#ifdef __MMI_SUBLCD__
	if( g_audply.is_subLCD_shown )
	{
		bitmap *saved_graphics_context = current_LCD_device_bitmap;
	
		UI_set_sub_LCD_graphics_context();

		if( audply_sub_LCD_scrolling_title_p != NULL )
		{
			coolsand_UI_scrolling_text_stop( audply_sub_LCD_scrolling_title_p );
			audply_sub_LCD_scrolling_title_p = NULL;
		}
		if( saved_graphics_context == &main_LCD_device_bitmap )
			UI_set_main_LCD_graphics_context();
		else if( saved_graphics_context == &sub_LCD_device_bitmap )
			UI_set_sub_LCD_graphics_context();

		mmi_audply_set_subLCD_shown( FALSE );
	}
#endif // __MMI_SUBLCD__
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_show_mainscreen
* DESCRIPTION
*     show audio player main screen
* IMPACT 
*     
* PARAMETERS
*     U8* history_buffer
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_show_mainscreen( U16 title, U16 left_softkey, U16 right_softkey, S8 skin )
{
	MMI_title_string = (UI_string_type) GetString( title );
	
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_layer_multi_layer_enable();

	/* get base layer as skin layer */
	gdi_layer_get_base_handle( &audply_osd_layer );
	gdi_layer_clear_background( GDI_COLOR_TRANSPARENT );
	gdi_layer_set_source_key( TRUE, GDI_COLOR_TRANSPARENT );
	gdi_layer_create( 0, 0, UI_device_width, UI_device_height, &audply_skin_layer );
	gdi_layer_set_blt_layer( audply_skin_layer, audply_osd_layer, 0, 0 );
#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
    audply_lyric_print.target_layer = audply_osd_layer;
#endif 
#endif

	// init images
	if( !init_images_done || audply_main_skin_index != skin )
	{
		mmi_audply_init_main_screen_images( skin );
		audply_main_skin_index = skin;
		init_images_done = TRUE;
		init_scrolling_done = FALSE;
	}
#ifdef __GDI_MEMORY_PROFILE_2__
	is_audply_skin_layer_drawn = FALSE;
#endif

	clear_key_handlers();
	clear_left_softkey();
	clear_right_softkey();
	audply_main_LSK_text = (UI_string_type) GetString( left_softkey );
	audply_main_RSK_text = (UI_string_type) GetString( right_softkey );
	SetKeyHandler( mmi_audply_redraw_LSK_down, KEY_LSK, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_redraw_RSK_down, KEY_RSK, KEY_EVENT_DOWN );
	
	// associate functions
	ExitCategoryFunction = MMI_dummy_function;
	RedrawCategoryFunction = MMI_dummy_function;
	GetCategoryHistory = dummy_get_history;
	GetCategoryHistorySize = dummy_get_history_size;
	
	// draw screen
	mmi_audply_redraw_main_screen();
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_exit_mainscreen
* DESCRIPTION
*     exit audio player main screen
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_exit_mainscreen()
{
	coolsand_UI_scrolling_text_stop( &audply_main_scrolling_title );
#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
    mmi_audply_stop_scroll_lyric_and_clean_screen();
    mmi_audply_lyric_check_backlight(0);
    lyric_reEntry = MMI_TRUE;
#endif /* __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__ */ 
#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
	// stop animation
	if ( audply_main_animation.img_handle!=GDI_ERROR_HANDLE)
		gdi_image_stop_animation( audply_main_animation.img_handle );
#endif
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_layer_flatten_to_base( audply_skin_layer, audply_osd_layer, 0, 0 );
	gdi_layer_free( audply_skin_layer );
	gdi_layer_multi_layer_disable();

	/* restore blt layer as base layer only */
	gdi_layer_set_blt_layer( audply_osd_layer, 0, 0, 0 );
#endif
#if defined(__PROJECT_GALLITE_C01__)
	audply_main_volume_inc.down = FALSE;
	audply_main_volume_dec.down = FALSE;
#endif
#ifdef __MMI_TOUCH_SCREEN__
	audply_main_volume_inc.down = FALSE;
	audply_main_volume_dec.down = FALSE;
	audply_main_setting.down = FALSE;

	audply_pen_event_on_object = AUDPLY_PEN_NONE;
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_redraw_LSK_down
* DESCRIPTION
*     redraw shifted LSK for key press down event
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_redraw_LSK_down( void )
{
	S32 str_w, str_h;
	S32 x1, y1, x2, y2;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif
       /*Fix Bug#12771 for cs2324c by cong.li on 2009.06.26. 
           The text display style should follow as the current theme.*/
	coolsand_UI_set_font( current_icontext_button_theme->text_font);
	coolsand_UI_set_text_color(current_icontext_button_theme->clicked_down_text_color);
	coolsand_UI_set_text_border_color(*current_MMI_theme->title_text_border_color);
	
	// show LSK
	coolsand_UI_measure_string( audply_main_LSK_text, &str_w, &str_h );
	x1 = AUDPLY_LSK_X_OFFSET;
	y1 = UI_device_height - str_h - AUDPLY_LSK_Y_OFFSET;
	x2 = str_w + AUDPLY_LSK_X_OFFSET;
	y2 = UI_device_height - AUDPLY_LSK_Y_OFFSET;
	gdi_layer_push_clip();
	gdi_layer_set_clip( x1-1, y1 - 2, x2, y2 );
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( x1-1, y1 - 2, x2, y2, GDI_COLOR_TRANSPARENT );
#else
	gdi_image_draw( 0, 0, audply_main_bg_img );
#endif
	gdi_layer_pop_clip();
	coolsand_UI_push_text_clip();
	coolsand_UI_set_text_clip( x1, y1-1, x2, y2 );
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x2, y1 );
	else
		coolsand_UI_move_text_cursor( x1, y1 );
	coolsand_UI_print_bordered_text( audply_main_LSK_text );
	coolsand_UI_pop_text_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1-1, y1-2, x2, y2 );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_redraw_RSK_down
* DESCRIPTION
*     redraw shifted RSK for key press down event
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_redraw_RSK_down( void )
{
	S32 str_w, str_h;
	S32 x1, y1, x2, y2;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif
       /*Fix Bug#12771 for cs2324c by cong.li on 2009.06.26. 
           The text display style should follow as the current theme.*/
	coolsand_UI_set_font( current_icontext_button_theme->text_font);
	coolsand_UI_set_text_color(current_icontext_button_theme->clicked_down_text_color);
	coolsand_UI_set_text_border_color(*current_MMI_theme->title_text_border_color);

	// show RSK
	coolsand_UI_measure_string( audply_main_RSK_text, &str_w, &str_h );
	x1 = UI_device_width - str_w - AUDPLY_RSK_X_OFFSET;
	y1 = UI_device_height - str_h - AUDPLY_RSK_Y_OFFSET;
	x2 = UI_device_width - AUDPLY_RSK_X_OFFSET;
	y2 = UI_device_height - AUDPLY_RSK_Y_OFFSET;
	gdi_layer_push_clip();
	gdi_layer_set_clip( x1-1, y1-2, x2, y2 );
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( x1-1, y1-2, x2, y2, GDI_COLOR_TRANSPARENT );
#else
	gdi_image_draw(0, 0, audply_main_bg_img );
#endif
	gdi_layer_pop_clip();
	coolsand_UI_push_text_clip();
	coolsand_UI_set_text_clip( x1, y1-1, x2, y2 );
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x2, y1 );	
	else
		coolsand_UI_move_text_cursor( x1, y1 );
	coolsand_UI_print_bordered_text( audply_main_RSK_text );
	coolsand_UI_pop_text_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1-1, y1-2, x2, y2 );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_redraw_LSK_down
* DESCRIPTION
*     redraw shifted LSK for key press down event
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_redraw_LSK_up( void )
{
	S32 str_w, str_h;
	S32 x1, y1, x2, y2;

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif
       /*Fix Bug#12771 for cs2324c by cong.li on 2009.06.26. 
           The text display style should follow as the current theme.*/
	coolsand_UI_set_font( current_icontext_button_theme->text_font);
	coolsand_UI_set_text_color(current_icontext_button_theme->clicked_up_text_color);
	coolsand_UI_set_text_border_color(*current_MMI_theme->title_text_border_color);

	// show LSK
	coolsand_UI_measure_string( audply_main_LSK_text, &str_w, &str_h );
	x1 = AUDPLY_LSK_X_OFFSET - 1;
	y1 = UI_device_height - str_h - AUDPLY_LSK_Y_OFFSET - 1;
	x2 = str_w + AUDPLY_LSK_X_OFFSET - 1;
	y2 = UI_device_height - AUDPLY_LSK_Y_OFFSET - 1;
	gdi_layer_push_clip();
	gdi_layer_set_clip( x1, y1, x2+1, y2+1 );
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( x1, y1, x2+1, y2+1, GDI_COLOR_TRANSPARENT );
#else
	gdi_image_draw( 0, 0, audply_main_bg_img );
#endif
	gdi_layer_pop_clip();
	coolsand_UI_push_text_clip();
	coolsand_UI_set_text_clip( x1, y1-1, x2, y2 );
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x2, y1 );
	else
		coolsand_UI_move_text_cursor( x1, y1 );
	coolsand_UI_print_bordered_text( audply_main_LSK_text );
	coolsand_UI_pop_text_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1, x2+1, y2+1 );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_redraw_RSK_down
* DESCRIPTION
*     redraw shifted RSK for key press down event
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_redraw_RSK_up( void )
{
	S32 str_w, str_h;
	S32 x1, y1, x2, y2;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif	
       /*Fix Bug#12771 for cs2324c by cong.li on 2009.06.26. 
           The text display style should follow as the current theme.*/
	coolsand_UI_set_font( current_icontext_button_theme->text_font);
	coolsand_UI_set_text_color(current_icontext_button_theme->clicked_up_text_color);
	coolsand_UI_set_text_border_color(*current_MMI_theme->title_text_border_color);

	// show RSK
	coolsand_UI_measure_string( audply_main_RSK_text, &str_w, &str_h );
	x1 = UI_device_width - str_w - AUDPLY_RSK_X_OFFSET - 1;
	y1 = UI_device_height - str_h - AUDPLY_RSK_Y_OFFSET - 1;
	x2 = UI_device_width - AUDPLY_RSK_X_OFFSET - 1;
	y2 = UI_device_height - AUDPLY_RSK_Y_OFFSET - 1;
	gdi_layer_push_clip();
	gdi_layer_set_clip( x1, y1, x2+1, y2+1 );
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( x1, y1, x2+1, y2+1, GDI_COLOR_TRANSPARENT );
#else
	gdi_image_draw(0, 0, audply_main_bg_img );
#endif
	gdi_layer_pop_clip();
	coolsand_UI_push_text_clip();
	coolsand_UI_set_text_clip( x1, y1-1, x2, y2 );
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x2, y1 );	
	else
		coolsand_UI_move_text_cursor( x1, y1 );
	coolsand_UI_print_bordered_text( audply_main_RSK_text );
	coolsand_UI_pop_text_clip();

	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1, x2+1, y2+1 );

}
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
/*****************************************************************************
 * FUNCTION
 *  mmi_audply_redraw_main_repeat_shuffle
 * DESCRIPTION
 *  redraw audio player main repeat and shuffle icon
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_audply_redraw_main_repeat_shuffle(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    S32 x1, y1, x2, y2;
    PU8 image_id = NULL;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

#ifdef __GDI_MEMORY_PROFILE_2__
    gdi_layer_push_and_set_active(audply_skin_layer);
#endif /* __GDI_MEMORY_PROFILE_2__ */
    
    gdi_layer_push_clip();

    /* show repeat icon */
    x1 = (S32) audply_main_repeat.x;
    y1 = (S32) audply_main_repeat.y;
    x2 = x1 + audply_main_repeat.w - 1;
    y2 = y1 + audply_main_repeat.h - 1;
    gdi_layer_set_clip(x1, y1, x2, y2);
    switch (g_audply.repeat_mode)
    {
        case REPEAT_OFF:
            image_id = audply_main_repeat.off_up_img;
            break;
        case REPEAT_ALL:
            image_id = audply_main_repeat.all_up_img;
            break;
        case REPEAT_ONE:
            image_id = audply_main_repeat.one_up_img;
            break;
        default:
            break;
    }
    gdi_image_draw(x1, y1, image_id);

    /* show shuffle icon */
    x1 = (S32) audply_main_shuffle.x;
    y1 = (S32) audply_main_shuffle.y;
    x2 = x1 + audply_main_shuffle.w - 1;
    y2 = y1 + audply_main_shuffle.h - 1;
    gdi_layer_set_clip(x1, y1, x2, y2);
    switch (g_audply.random_mode)
    {
        case SHUFFLE_OFF:
            image_id = audply_main_shuffle.off_up_img;
            break;
        case SHUFFLE_ON:
            image_id = audply_main_shuffle.on_up_img;
            break;
        default:
            break;
    }
    gdi_image_draw(x1, y1, image_id);

    gdi_layer_pop_clip();

#ifdef __GDI_MEMORY_PROFILE_2__
    gdi_layer_pop_and_restore_active();
#endif /* __GDI_MEMORY_PROFILE_2__ */
}
#endif /* __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__ */

/*****************************************************************************
* FUNCTION
*     mmi_audply_redraw_main_screen
* DESCRIPTION
*     redraw audio player main screen
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_redraw_main_screen( void )
{	
	S32 str_w, str_h;
	S32 x1, y1, x2, y2;


#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif
	
	coolsand_UI_push_clip();
	coolsand_UI_push_text_clip();
	
	// background ,head, LSK, and RSK
#ifdef __GDI_MEMORY_PROFILE_2__
	if( !is_audply_skin_layer_drawn )
	{
		gdi_layer_push_and_set_active( audply_skin_layer );
#endif
	coolsand_UI_set_clip( 0, 0, UI_device_width - 1, UI_device_height - 1 );
	coolsand_UI_show_image( 0, 0, audply_main_bg_img );
#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
		// show animation
		audply_main_animation.img_handle=GDI_ERROR_HANDLE;
		gdi_image_draw_animation( audply_main_animation.x, audply_main_animation.y, audply_main_animation.animation_img, &audply_main_animation.img_handle );
	if(!mmi_audply_is_playing())	
		{
		if ( audply_main_animation.img_handle!=GDI_ERROR_HANDLE)
			gdi_image_stop_animation( audply_main_animation.img_handle );
		}
#endif
#ifdef __GDI_MEMORY_PROFILE_2__
		is_audply_skin_layer_drawn = TRUE;
		gdi_layer_pop_and_restore_active();
	}
#endif
       /*Fix Bug#12771 for cs2324c by cong.li on 2009.06.26. 
           The text display style should follow as the current theme.*/
	coolsand_UI_set_font( current_icontext_button_theme->text_font);
	coolsand_UI_set_text_color(current_icontext_button_theme->normal_up_text_color);
	coolsand_UI_set_text_border_color(*current_MMI_theme->title_text_border_color);
	coolsand_UI_measure_string( MMI_title_string, &str_w, &str_h );
#if (defined(__MMI_MAINLCD_176X220__) && defined(__MMI_AUDIO_PLAYER_LYRIC_DISPLAY__))//for proj t33a(176X220)
    if(str_w > AUDPLY_TITLE_WIDTH_FOR_176X220)
         str_w = AUDPLY_TITLE_WIDTH_FOR_176X220;
	coolsand_UI_set_text_clip(AUDPLY_TITLE_AREA_BEGIN_X,AUDPLY_TITLE_AREA_BEGIN_Y,AUDPLY_TITLE_WIDTH_FOR_176X220 + AUDPLY_TITLE_AREA_BEGIN_X,AUDPLY_TITLE_AREA_BEGIN_Y+MMI_title_height - 1 );
#else
	if( str_w > UI_device_width )
		str_w = UI_device_width;
	coolsand_UI_set_text_clip( 0, 0, UI_device_width - 1, MMI_title_height - 1 );
#endif
    if( r2lMMIFlag )
		x1 = (UI_device_width + str_w)/2;
	else
		x1 = (UI_device_width - str_w)/2;
#if (defined(__MMI_MAINLCD_176X220__) && defined(__MMI_AUDIO_PLAYER_LYRIC_DISPLAY__))//for proj t33a(176X220)
	coolsand_UI_print_truncated_borderd_text(x1,AUDPLY_TITLE_AREA_BEGIN_Y,AUDPLY_TITLE_WIDTH_FOR_176X220,MMI_title_string);
#else
	coolsand_UI_print_truncated_borderd_text( x1, (MMI_title_height - str_h)/2, UI_device_width - 2, MMI_title_string );
#endif
	// show LSK
	coolsand_UI_measure_string( audply_main_LSK_text, &str_w, &str_h );
	x1 = AUDPLY_LSK_X_OFFSET - 1;
	y1 = UI_device_height - str_h - AUDPLY_LSK_Y_OFFSET - 1;
	x2 = str_w + AUDPLY_LSK_X_OFFSET - 1;
	y2 = UI_device_height - AUDPLY_LSK_Y_OFFSET - 1;
	coolsand_UI_set_text_clip( x1, y1-1, x2, y2 );
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x2, y1 );
	else
		coolsand_UI_move_text_cursor( x1, y1 );

	coolsand_UI_print_bordered_text( audply_main_LSK_text );

#ifdef __MMI_TOUCH_SCREEN__
	LSK_area.num = 4;
	LSK_area.points = (mmi_pen_point_struct*) LSK_points;
	LSK_points[0].x = LSK_points[2].x = x1;
	LSK_points[0].y = LSK_points[1].y = y1;
	LSK_points[3].x = LSK_points[1].x = x2;
	LSK_points[3].y = LSK_points[2].y = y2;
#endif

	// show RSK
	coolsand_UI_measure_string( audply_main_RSK_text, &str_w, &str_h );
	x1 = UI_device_width - str_w - AUDPLY_RSK_X_OFFSET - 1;
	y1 = UI_device_height - str_h - AUDPLY_RSK_Y_OFFSET - 1;
	x2 = UI_device_width - AUDPLY_RSK_X_OFFSET - 1;
	y2 = UI_device_height - AUDPLY_RSK_Y_OFFSET - 1;
	coolsand_UI_set_text_clip( x1, y1-1, x2, y2 );
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x2, y1 );
	else
		coolsand_UI_move_text_cursor( x1, y1 );

	coolsand_UI_print_bordered_text( audply_main_RSK_text );

#ifdef __MMI_TOUCH_SCREEN__
	RSK_area.num = 4;
	RSK_area.points = (mmi_pen_point_struct*) RSK_points;
	RSK_points[0].x = RSK_points[2].x = x1;
	RSK_points[0].y = RSK_points[1].y = y1;
	RSK_points[3].x = RSK_points[1].x = x2;
	RSK_points[3].y = RSK_points[2].y = y2;
#endif

	coolsand_UI_pop_text_clip();
	coolsand_UI_pop_clip();

	mmi_audply_redraw_main_title();
	mmi_audply_redraw_main_playlisticon();
	mmi_audply_redraw_main_file_index();
	mmi_audply_redraw_main_time();
	mmi_audply_redraw_main_volumebar();
	/*show progress bar*/
        mmi_audply_redraw_main_progressbar();
#if defined(__PROJECT_GALLITE_C01__)
	mmi_audply_redraw_main_volumectrl( 0, audply_main_volume_dec.down );
	mmi_audply_redraw_main_volumectrl( 1, audply_main_volume_inc.down );
#endif
#ifdef __MMI_TOUCH_SCREEN__

	mmi_audply_redraw_main_volumectrl( 0, audply_main_volume_dec.down );
	mmi_audply_redraw_main_volumectrl( 1, audply_main_volume_inc.down );
	mmi_audply_redraw_main_setting( audply_main_setting.down );

#endif
	mmi_audply_redraw_main_buttons();

#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
	mmi_audply_redraw_main_repeat_shuffle();
#endif
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( 0, 0, UI_device_width - 1, UI_device_height - 1 );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_redraw_main_title_background
* DESCRIPTION
*     redraw audio player main title background
* IMPACT 
*     
* PARAMETERS
*      S32 x1
*      S32 y1
*      S32 x2
*      S32 y2 
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_redraw_main_title_background( S32 x1, S32 y1, S32 x2, S32 y2 )
{
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
#else
	coolsand_UI_fill_rectangle( x1, y1, x2, y2, text_bg_color );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_scroll_main_title_handler
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_scroll_main_title_handler( void )
{
	coolsand_UI_handle_scrolling_text( &audply_main_scrolling_title );
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_redraw_main_title
* DESCRIPTION
*     redraw audio player main title
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_redraw_main_title( void )
{
	S32 character_height, w;
	S32 x1 = audply_main_title.x;
	S32 y1 = audply_main_title.y;
	S32 x2 = audply_main_title.x + audply_main_title.w - 1;
	S32 y2 = audply_main_title.y + audply_main_title.h - 1;

	if( !init_scrolling_done )
	{
		coolsand_UI_create_scrolling_text( &audply_main_scrolling_title,
			x1, y1, audply_main_title.w, audply_main_title.h, g_audply.title,
			mmi_audply_scroll_main_title_handler, mmi_audply_redraw_main_title_background,
			text_fg_color, text_bg_color );
		audply_main_scrolling_title.text_font = &MMI_medium_font;
		init_scrolling_done = TRUE;
	}

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	coolsand_UI_push_clip();
	coolsand_UI_set_clip( x1, y1, x2, y2 );
	
	// title
	coolsand_UI_scrolling_text_stop( &audply_main_scrolling_title );
	coolsand_UI_set_font( &MMI_medium_font );
	coolsand_UI_measure_string( g_audply.title, &w, &character_height );
	if( w > audply_main_title.w )
	{
		// scrolling
		coolsand_UI_change_scrolling_text( &audply_main_scrolling_title, g_audply.title );
		coolsand_UI_show_scrolling_text( &audply_main_scrolling_title );
	}
	else
	{
		// background
		mmi_audply_redraw_main_title_background( x1, y1, x2, y2 );

		coolsand_UI_push_text_clip();
		coolsand_UI_set_text_clip( x1, y1, x2, y2 );
		coolsand_UI_set_text_color( text_fg_color );
		if( r2lMMIFlag )
			coolsand_UI_move_text_cursor( x1 + (audply_main_title.w + w )/2, y1 );
		else
			coolsand_UI_move_text_cursor( x1 + (audply_main_title.w - w )/2, y1 );
		coolsand_UI_set_line_height( character_height );
		coolsand_UI_print_text( g_audply.title );
		coolsand_UI_pop_text_clip();
	}

	coolsand_UI_pop_clip();
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1, x2, y2 );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_redraw_main_playlisticon
* DESCRIPTION
*     redraw audio player present play list icon
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_redraw_main_playlisticon( void )
{
	S32 x1 = audply_main_playlisticon.x;
	S32 y1 = audply_main_playlisticon.y;
	S32 x2 = audply_main_playlisticon.x + audply_main_playlisticon.w - 1;
	S32 y2 = audply_main_playlisticon.y + audply_main_playlisticon.h - 1;
	U8 playlist = g_audply.present_list;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	// play list icon
	coolsand_UI_push_clip();
	coolsand_UI_set_clip( x1, y1, x2, y2 );
	coolsand_UI_show_image( x1, y1, audply_main_playlisticon.list_icon[playlist] );
	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1, x2, y2 );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_redraw_main_time
* DESCRIPTION
*     redraw audio player main time
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_redraw_main_time( void )
{
	S32 x1 = audply_main_time.x;
	S32 y1 = audply_main_time.y;
	S32 x2 = audply_main_time.x + audply_main_time.w - 1;
	S32 y2 = audply_main_time.y + audply_main_time.h - 1;
	S32 str_w, str_h;
	S8 temp[8];
	UI_character_type time_str[8];
	S32 elapsed_seconds = mmi_audply_get_play_seconds();
	sprintf( temp, "%02d:%02d", elapsed_seconds / 60, elapsed_seconds % 60 );
	AnsiiToUnicodeString( (S8*)time_str, (S8*)temp );

	if(0 == elapsed_seconds)
		{
			audply_main_progressbar.PlayProgress = 0;
		}
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	// background & time
	coolsand_UI_push_clip();
	coolsand_UI_push_text_clip();
	coolsand_UI_set_clip( x1, y1, x2, y2 );
	coolsand_UI_set_text_clip( x1, y1, x2, y2 );
	coolsand_UI_fill_rectangle( x1, y1, x2, y2, text_bg_color );
	coolsand_UI_set_font( &MMI_small_font );
	coolsand_UI_set_text_color( text_fg_color );
	coolsand_UI_measure_string( time_str, &str_w, &str_h );
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x1 + (audply_main_time.w + str_w)/2, y1 + 1 );
	else
		coolsand_UI_move_text_cursor( x1 + (audply_main_time.w - str_w)/2, y1 + 1 );
	coolsand_UI_print_text( time_str );
	coolsand_UI_pop_text_clip();
	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1, x2, y2 );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_redraw_main_file_index
* DESCRIPTION
*     mmi_audply_redraw_main_file_index
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_redraw_main_file_index()
{
	S32 x1 = audply_main_file_index.x;
	S32 y1 = audply_main_file_index.y;
	S32 x2 = audply_main_file_index.x + audply_main_file_index.w - 1;
	S32 y2 = audply_main_file_index.y + audply_main_file_index.h - 1;
	S32 str_w, str_h;
	S8 temp[24];
	UI_character_type file_index[10];
	sprintf( temp, "%d/%d", g_audply.pick_index + 1, g_audply.n_total_items );
	AnsiiToUnicodeString( (S8*)file_index, (S8*)temp );
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	// background & file index
	coolsand_UI_push_clip();
	coolsand_UI_push_text_clip();
	coolsand_UI_set_clip( x1, y1, x2, y2 );
	coolsand_UI_set_text_clip( x1, y1, x2, y2 );
	coolsand_UI_fill_rectangle( x1, y1, x2, y2, text_bg_color );
	coolsand_UI_set_font( &MMI_small_font );
	coolsand_UI_set_text_color( text_fg_color );
	coolsand_UI_measure_string( file_index, &str_w, &str_h );
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x2, y1 + 1 );
	else
		coolsand_UI_move_text_cursor( x1 + audply_main_file_index.w - str_w, y1 + 1 );
	coolsand_UI_print_text( file_index );
	coolsand_UI_pop_text_clip();
	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1, x2, y2 );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_redraw_main_volumebar
* DESCRIPTION
*     redraw audio player main volume bar
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_redraw_main_volumebar( void )
{
	S32 x1 = audply_main_volumebar.x;
	S32 y1 = audply_main_volumebar.y;
	S32 x2 = audply_main_volumebar.x + audply_main_volumebar.w - 1;
	S32 y2 = audply_main_volumebar.y + audply_main_volumebar.h - 1;
	U32 dx = audply_main_volumebar.ind_left + ((U32)g_audply.volume ) * (audply_main_volumebar.w - audply_main_volumebar.ind_left) / MAX_VOL_LEVEL;
	if( dx > (U32)audply_main_volumebar.w ) dx = (U32)audply_main_volumebar.w;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	// background & indicator
	coolsand_UI_push_clip();
	coolsand_UI_set_clip( x1, y1, x2, y2 );
	coolsand_UI_fill_rectangle( x1, y1, x2, y2, text_bg_color );
	coolsand_UI_set_clip( x1, y1, x1 + dx, y2 );
	if (dx != 0) {
	    coolsand_UI_show_image( x1, y1, audply_main_volumebar.ind_img );
    }
	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1, x2, y2 );
#endif
}




/*****************************************************************************
 * FUNCTION
 *  mmi_audply_redraw_main_progressbar
 * DESCRIPTION
 *  redraw audio player main progress bar
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 extern audply_state_enum mmi_audply_get_state();

void mmi_audply_redraw_main_progressbar(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1 = audply_main_progressbar.x;
    S32 y1 = audply_main_progressbar.y;
    S32 x2 = audply_main_progressbar.x + audply_main_progressbar.w - 1;
    S32 y2 = audply_main_progressbar.y + audply_main_progressbar.h - 1;
    U32 dx ; 
    INT32 PlayProgress = 0;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	
	if(mmi_audply_get_state() == STATE_PLAY)
	{
	#ifdef MMI_ON_HARDWARE_P
		PlayProgress =(INT32)mmi_audply_get_progress();
	#endif
		audply_main_progressbar.PlayProgress = PlayProgress;
	}
	else
	{
		PlayProgress = audply_main_progressbar.PlayProgress;
	}
    dx = (U32)(audply_main_progressbar.w * PlayProgress/10000) ;

#ifdef __GDI_MEMORY_PROFILE_2__
    gdi_layer_push_and_set_active(audply_skin_layer);
#endif /* __GDI_MEMORY_PROFILE_2__ */
#if(UI_DOUBLE_BUFFER_SUPPORT)
    gui_lock_double_buffer();
#endif 

    gdi_layer_push_clip();
    gdi_layer_set_clip(x1, y1, x2, y2);
    gdi_image_draw(0, 0, audply_main_bg_img );

        
        if (PlayProgress != 0)
#if defined(__MMI_NOKIA_STYLE_N800__)
		gdi_draw_solid_rect(x1,y1+1,x1+dx,y2-1,GDI_COLOR_BLUE);	//for image audply_main_progressbar.img's width is too small.
#else
		{
            gdi_layer_set_clip(x1, y1, x1 + dx, y2);
            gdi_image_draw(x1, y1, audply_main_progressbar.img);
        }
#endif    
    gdi_layer_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(x1, y1, x2, y2);
#endif /* (UI_DOUBLE_BUFFER_SUPPORT) */ 
#ifdef __GDI_MEMORY_PROFILE_2__
    gdi_layer_pop_and_restore_active();
#endif /* __GDI_MEMORY_PROFILE_2__ */

}

#if defined(__PROJECT_GALLITE_C01__) && !defined(__MMI_TOUCH_SCREEN__)
/*****************************************************************************
* FUNCTION
*     mmi_audply_redraw_main_volumectrl
* DESCRIPTION
*     redraw audio player main volume inc / dec control
* IMPACT 
*     
* PARAMETERS
*     U8 sel: 0 -> dec, 1 -> inc
*     BOOL down
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_redraw_main_volumectrl( U8 sel, BOOL down )
{
	S32 x1, y1, x2, y2;
	audply_main_volumectrl_struct *volumectrl_p = 
		(sel == 0) ? &audply_main_volume_dec: &audply_main_volume_inc;

	x1 = volumectrl_p->x;
	y1 = volumectrl_p->y;
	x2 = volumectrl_p->x + volumectrl_p->w - 1;
	y2 = volumectrl_p->y + volumectrl_p->h - 1;
	// update the down state
	volumectrl_p->down = down;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	// background & indicator
	coolsand_UI_push_clip();
	coolsand_UI_set_clip( x1, y1, x2, y2 );

	coolsand_UI_show_image( x1, y1, (down ? volumectrl_p->down_img: volumectrl_p->up_img) );
	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1, x2, y2 );
#endif
}
#endif

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  mmi_audply_touch_progress_hdlr
 * DESCRIPTION
 *  take care the touch progress build cache and progress bar drawing
 *  we think of touch progress the same as longpress action
 * PARAMETERS
 *  cor_x       [IN]    x coordinate of touch down position
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_touch_progress_hdlr(S32 cor_x)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

	if(cor_x < audply_main_progressbar.x)
		{
			audply_main_progressbar.PlayProgress = 0;
		}
	else if(cor_x >=( audply_main_progressbar.x + audply_main_progressbar.w))
		{
			audply_main_progressbar.PlayProgress = AUD_FULL_PROGRESS_CNT;
		}
	else
		{
			audply_main_progressbar.PlayProgress = (cor_x - audply_main_progressbar.x)*(AUD_FULL_PROGRESS_CNT+1)/audply_main_progressbar.w;
		}
	mmi_audply_redraw_main_progressbar();
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_redraw_main_volumectrl
* DESCRIPTION
*     redraw audio player main volume inc / dec control
* IMPACT 
*     
* PARAMETERS
*     U8 sel: 0 -> dec, 1 -> inc
*     BOOL down
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_redraw_main_volumectrl( U8 sel, BOOL down )
{
	S32 x1, y1, x2, y2;
	audply_main_volumectrl_struct *volumectrl_p = 
		(sel == 0) ? &audply_main_volume_dec: &audply_main_volume_inc;

	x1 = volumectrl_p->x;
	y1 = volumectrl_p->y;
	x2 = volumectrl_p->x + volumectrl_p->w - 1;
	y2 = volumectrl_p->y + volumectrl_p->h - 1;
	// update the down state
	volumectrl_p->down = down;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	// background & indicator
	coolsand_UI_push_clip();
	coolsand_UI_set_clip( x1, y1, x2, y2 );

	coolsand_UI_show_image( x1, y1, (down ? volumectrl_p->down_img: volumectrl_p->up_img) );
	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1, x2, y2 );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_redraw_main_setting
* DESCRIPTION
*     redraw audio player main setting icon
* IMPACT 
*     
* PARAMETERS
*     BOOL down
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_redraw_main_setting( BOOL down )
{
	S32 x1 = audply_main_setting.x;
	S32 y1 = audply_main_setting.y;
	S32 x2 = audply_main_setting.x + audply_main_setting.w - 1;
	S32 y2 = audply_main_setting.y + audply_main_setting.h - 1;
	// update the down state
	audply_main_setting.down = down;
#ifdef __GDI_MEMORY_PROFILE_2__
    gdi_layer_push_and_set_active(audply_skin_layer);
#endif /* __GDI_MEMORY_PROFILE_2__ */

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	// background & indicator
	coolsand_UI_push_clip();
	coolsand_UI_set_clip( x1, y1, x2, y2 );
	coolsand_UI_show_image( x1, y1, (down ? audply_main_setting.down_img: audply_main_setting.up_img) );
	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1, x2, y2 );
#endif
#ifdef __GDI_MEMORY_PROFILE_2__
    gdi_layer_pop_and_restore_active();
#endif /* __GDI_MEMORY_PROFILE_2__ */

}
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
/*****************************************************************************
 * FUNCTION
 *  mmi_audply_redraw_main_repeat
 * DESCRIPTION
 *  redraw audio player main repeat icon
 * PARAMETERS
 *  down        [IN]        to indicate weather the button is up or down.
 *                          down = TURE
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_redraw_main_repeat(MMI_BOOL down)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1 = (S32) audply_main_repeat.x;
    S32 y1 = (S32) audply_main_repeat.y;
    S32 x2 = x1 + audply_main_repeat.w - 1;
    S32 y2 = y1 + audply_main_repeat.h - 1;

    PU8 image_id = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* update the down state */
    audply_main_repeat.down = down;

    switch (g_audply.repeat_mode)
    {
        case REPEAT_OFF:
            image_id = down ? audply_main_repeat.off_down_img : audply_main_repeat.off_up_img ; 
            break;
        case REPEAT_ALL:
            image_id = down ? audply_main_repeat.all_down_img: audply_main_repeat.all_up_img; 
            break;
        case REPEAT_ONE:
            image_id = down ? audply_main_repeat.one_down_img: audply_main_repeat.one_up_img; 
            break;
        default:
            break;
    }

#if(UI_DOUBLE_BUFFER_SUPPORT)
    gui_lock_double_buffer();
#endif 

    /* background & indicator */
    gdi_layer_push_clip();
    gdi_layer_set_clip(x1, y1, x2, y2);
    gdi_image_draw(x1, y1, image_id);
    gdi_layer_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(x1, y1, x2, y2);
#endif /* (UI_DOUBLE_BUFFER_SUPPORT) */ 
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_touch_repeat_toggle
 * DESCRIPTION
 *  handle the switching action among repear sates.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_touch_repeat_toggle()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/    
    S16 error; 
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_audply.repeat_mode = (g_audply.repeat_mode + 1)%3 ;
    WriteValue(NVRAM_AUDPLY_REPEAT, &g_audply.repeat_mode, DS_BYTE, &error);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_redraw_main_shuffle
 * DESCRIPTION
 *  redraw audio player main shuffle icon
 * PARAMETERS
 *  down        [IN]        to indicate weather the button is up or down.
 *                          down = TURE
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_redraw_main_shuffle(MMI_BOOL down)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1 = (S32) audply_main_shuffle.x;
    S32 y1 = (S32) audply_main_shuffle.y;
    S32 x2 = x1 + audply_main_shuffle.w - 1;
    S32 y2 = y1 + audply_main_shuffle.h - 1;

    PU8 image_id = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* update the down state */
    audply_main_shuffle.down = down;

    switch (g_audply.random_mode)
    {
        case SHUFFLE_OFF:
            image_id = down ? audply_main_shuffle.off_down_img : audply_main_shuffle.off_up_img ; 
            break;
        case SHUFFLE_ON:
            image_id = down ? audply_main_shuffle.on_down_img: audply_main_shuffle.on_up_img; 
            break;
        default:
            break;
    }

#if(UI_DOUBLE_BUFFER_SUPPORT)
    gui_lock_double_buffer();
#endif 

    /* background & indicator */
    gdi_layer_push_clip();
    gdi_layer_set_clip(x1, y1, x2, y2);
    gdi_image_draw(x1, y1, image_id);
    gdi_layer_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(x1, y1, x2, y2);
#endif /* (UI_DOUBLE_BUFFER_SUPPORT) */ 
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_touch_repeat_toggle
 * DESCRIPTION
 *  handle the switching action among repear sates.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_touch_shuffle_toggle()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/    
    S16 error ; 
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_audply.random_mode = (g_audply.random_mode + 1)%2 ;
    WriteValue(NVRAM_AUDPLY_SHUFFLE, &g_audply.random_mode, DS_BYTE, &error);
}
#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/

#endif

/*****************************************************************************
* FUNCTION
*     mmi_audply_redraw_main_buttons
* DESCRIPTION
*     redraw audio player main buttons
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_redraw_main_buttons( void )
{
	S32 i;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	// background & buttons
	coolsand_UI_push_clip();
	coolsand_UI_set_clip( audply_main_buttons_x1, audply_main_buttons_y1, audply_main_buttons_x2, audply_main_buttons_y2 );
	//Fix Bug#11441 for CP1606 by cong.li on 2009.05.25. 
	//Here we shouldn't show the main_bg_img. Or main_bg_img will cover the volumebar, time, file index, etc.  
	//coolsand_UI_show_image( 0, 0, audply_main_bg_img);
	for( i = 0; i < NO_OF_AUDPLY_MAIN_BUTTONS; i++ )
	{
#if defined(__PROJECT_GALLITE_C01__)		
		if (i == AUDPLY_MAIN_STOP)
		{
			continue;
		}
		else
#endif		
		if( g_audply_main_selected_button == AUDPLY_MAIN_DISABLED )
			coolsand_UI_show_image( audply_main_buttons[i].x, audply_main_buttons[i].y, audply_main_buttons[i].disabled_img );
		else if( g_audply_main_selected_button == i )
		{
			if( g_audply_main_selected_button_down )
				coolsand_UI_show_image( audply_main_buttons[i].x, audply_main_buttons[i].y, audply_main_buttons[i].down_img );
			else
				coolsand_UI_show_image( audply_main_buttons[i].x, audply_main_buttons[i].y, audply_main_buttons[i].on_img );
		}
		else
			coolsand_UI_show_image( audply_main_buttons[i].x, audply_main_buttons[i].y, audply_main_buttons[i].off_img );
	}
	if( g_audply_main_selected_button == AUDPLY_MAIN_PAUSE )
	{
		if( g_audply_main_selected_button_down )
			coolsand_UI_show_image( audply_main_buttons[AUDPLY_MAIN_PLAY].x, audply_main_buttons[AUDPLY_MAIN_PLAY].y, audply_main_pause_down_img );
		else
			coolsand_UI_show_image( audply_main_buttons[AUDPLY_MAIN_PLAY].x, audply_main_buttons[AUDPLY_MAIN_PLAY].y, audply_main_pause_on_img );
	}
	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( audply_main_buttons_x1, audply_main_buttons_y1, audply_main_buttons_x2, audply_main_buttons_y2 );
#endif
}

 
#ifdef __MMI_TOUCH_SCREEN__
extern PUBLIC BOOL mmi_pen_check_inside_rect( mmi_pen_polygon_area_struct* polygon, mmi_pen_point_struct pos );

void mmi_audply_pen_down_hdlr( mmi_pen_point_struct pos )
{
	//return; //add by panxu 2007-7-8
	S16 i,j;
	mmi_pen_point_struct point;
	mmi_pen_polygon_area_struct* area = NULL;
	const audply_skin_struct *skin_p = &g_audply_skins[g_audply.skin];
	
	audply_pen_event_on_object = AUDPLY_PEN_NONE;

	for( i = AUDPLY_PEN_LSK; i < AUDPLY_PEN_COUNT; i++ )
	{
		switch( i )
		{
		case AUDPLY_PEN_LSK:
//			if (g_audply.in_main_screen = TRUE)
//				return;
			area = (mmi_pen_polygon_area_struct*)&LSK_area;
			point = pos;
			break;
		case AUDPLY_PEN_RSK:
// dingjian 20080124 close for TP	
//			if (g_audply.in_main_screen = TRUE)
//				return;
			area = (mmi_pen_polygon_area_struct*)&RSK_area;
			point = pos;
			break;
		case AUDPLY_PEN_SEEK:	
			{
				S32 x1 = audply_main_progressbar.x;
				S32 y1 = audply_main_progressbar.y;
				S32 x2 = audply_main_progressbar.x + audply_main_progressbar.w - 1;
				S32 y2 = audply_main_progressbar.y + audply_main_progressbar.h*2 - 1;	
				if((x1 <= pos.x)&&(pos.x <= x2)&&(y1 <= pos.y)&&(pos.y <= y2))
				{
					audply_pen_event_on_object = i;
				}
			}
			break;
			
		#if 1	
		case AUDPLY_PEN_MAIN_PREV:
			area = (mmi_pen_polygon_area_struct*)&skin_p->prev_button_area;
			// dingjian 20080124 modify for TP	
			point.x = pos.x; //- skin_p->prev_button_x;
			point.y = pos.y;// - skin_p->prev_button_y;
			break;
		case AUDPLY_PEN_MAIN_NEXT:
			area = (mmi_pen_polygon_area_struct*)&skin_p->next_button_area;
			// dingjian 20080124 modify for TP	
			point.x = pos.x;// - skin_p->next_button_x;
			point.y = pos.y;// - skin_p->next_button_y;
			break;
		case AUDPLY_PEN_MAIN_STOP:
			area = (mmi_pen_polygon_area_struct*)&skin_p->stop_button_area;
			// dingjian 20080124 modify for TP	
			point.x = pos.x;// - skin_p->stop_button_x;
			point.y= pos.y;// - skin_p->stop_button_y;
			break;
		case AUDPLY_PEN_MAIN_PLAY:
			area = (mmi_pen_polygon_area_struct*)&skin_p->play_button_area;
			// dingjian 20080124 modify for TP	
			point.x = pos.x;// - skin_p->play_button_x;
			point.y = pos.y ;//- skin_p->play_button_y;
			break;
		#endif	
		case AUDPLY_PEN_MAIN_SETTING:
			area = (mmi_pen_polygon_area_struct*)&skin_p->setting_area;
			point.x = pos.x;//- skin_p->setting_x;
			point.y = pos.y;//- skin_p->setting_y;
			break;
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__                
            case AUDPLY_PEN_MAIN_REPEAT:
                area = (mmi_pen_polygon_area_struct*) & skin_p->repeat_area;
                point.x = pos.x;// - skin_p->repeat_x;
                point.y = pos.y;// - skin_p->repeat_y;
                break;
            case AUDPLY_PEN_MAIN_SHUFFLE:
                area = (mmi_pen_polygon_area_struct*) & skin_p->shuffle_area;
                point.x = pos.x;// - skin_p->shuffle_x;
                point.y = pos.y ;//- skin_p->shuffle_y;
                break;
#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/

		#if 1	
		case AUDPLY_PEN_MAIN_VOLUME_INC:
			area = (mmi_pen_polygon_area_struct*)&skin_p->volume_inc_area;
			// dingjian 20080124 modify for TP	
			point.x = pos.x;// - skin_p->volume_inc_x;
			point.y = pos.y;// - skin_p->volume_inc_y;
			break;
		case AUDPLY_PEN_MAIN_VOLUME_DEC:
			area = (mmi_pen_polygon_area_struct*)&skin_p->volume_dec_area;
			// dingjian 20080124 modify for TP	
			point.x = pos.x;// - skin_p->volume_dec_x;
			point.y = pos.y;// - skin_p->volume_dec_y;
			break;
		#endif	
		}
              for(j=0;j<area->num;j++)
              {
                mmi_trace(1,"current area point%d = (  %d  ,  %d)",j,area->points[j].x,area->points[j].y);

              }
		if( mmi_pen_check_inside_rect( area, point ) )
		{
			audply_pen_event_on_object = i;
                    mmi_trace(1,"--------audply_pen_event_on_object : %d",audply_pen_event_on_object);
			break;
		}
	}
    
       mmi_trace(1,"has matched???? -------audply_pen_event_on_object match : %d",audply_pen_event_on_object);

	switch( audply_pen_event_on_object )
	{
	case AUDPLY_PEN_LSK:
		mmi_audply_redraw_LSK_down();
		ExecuteCurrKeyHandler(KEY_LSK, KEY_EVENT_DOWN);
		break;
	case AUDPLY_PEN_RSK:
		mmi_audply_redraw_RSK_down();
		ExecuteCurrKeyHandler(KEY_RSK, KEY_EVENT_DOWN);
		break;
	case AUDPLY_PEN_SEEK:
		if(pos.x < audply_main_progressbar.x)
		{
			audply_main_progressbar.PlayProgress = 0;
		}
		else if(pos.x >=( audply_main_progressbar.x + audply_main_progressbar.w))
			{
				audply_main_progressbar.PlayProgress = AUD_FULL_PROGRESS_CNT;
			}
		else
			{
				audply_main_progressbar.PlayProgress = (pos.x - audply_main_progressbar.x)*(AUD_FULL_PROGRESS_CNT+1)/audply_main_progressbar.w;
			}
		mmi_audply_do_before_seek_action(audply_main_progressbar.PlayProgress);
		mmi_audply_redraw_main_progressbar();
		mmi_audply_redraw_main_time();
		break;
	#if 1	
	case AUDPLY_PEN_MAIN_PREV:
		mmi_audply_press_prev_button_down();
		break;
	case AUDPLY_PEN_MAIN_NEXT:
		mmi_audply_press_next_button_down();
		break;
	case AUDPLY_PEN_MAIN_STOP:
		mmi_audply_press_stop_button_down();
		break;
	case AUDPLY_PEN_MAIN_PLAY:
		mmi_audply_press_play_button_down();
		break;
	#endif	
	case AUDPLY_PEN_MAIN_SETTING:
		mmi_audply_redraw_main_setting( TRUE );
		break;
	#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
        case AUDPLY_PEN_MAIN_REPEAT:
            mmi_audply_redraw_main_repeat(MMI_TRUE);
            break;
        case AUDPLY_PEN_MAIN_SHUFFLE:
            mmi_audply_redraw_main_shuffle(MMI_TRUE);
            break;            
    #endif /* __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__ */

	#if 1	
	case AUDPLY_PEN_MAIN_VOLUME_INC:
		mmi_audply_redraw_main_volumectrl( 1, TRUE );
		break;
	case AUDPLY_PEN_MAIN_VOLUME_DEC:
		mmi_audply_redraw_main_volumectrl( 0, TRUE );
		break;
	#endif
	}
}

void mmi_audply_pen_up_hdlr(mmi_pen_point_struct pos)
{
  /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
    mmi_pen_point_struct point;
    mmi_pen_polygon_area_struct *area = NULL;
    const audply_skin_struct *skin_p = &g_audply_skins[g_audply.skin];

#endif

	
	switch (audply_pen_event_on_object)
	{
		case AUDPLY_PEN_SEEK:	
			mmi_audply_touch_progress_hdlr(pos.x);
			mmi_audply_do_seek_action(audply_main_progressbar.PlayProgress);
			mmi_audply_redraw_main_time();
			break;
		#if 1
		case AUDPLY_PEN_MAIN_NEXT:
			mmi_audply_press_next_button_up();
			break;
		case AUDPLY_PEN_MAIN_PREV:
			mmi_audply_press_prev_button_up();
			break;
		case AUDPLY_PEN_MAIN_STOP:
			mmi_audply_press_stop_button_up();
			break;
		case AUDPLY_PEN_MAIN_PLAY:
			mmi_audply_press_play_button_up();
			break;
		#endif	
		case AUDPLY_PEN_MAIN_SETTING:
			mmi_audply_redraw_main_setting( FALSE );
			mmi_audply_entry_settings();
			break;
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
        case AUDPLY_PEN_MAIN_REPEAT:
            area = (mmi_pen_polygon_area_struct*) & skin_p->repeat_area;
            point.x = pos.x;
            point.y = pos.y;
            if (mmi_pen_check_inside_rect(area, point))
            {
                mmi_audply_touch_repeat_toggle();
                mmi_audply_redraw_main_repeat(MMI_FALSE);
            }
            break;
        case AUDPLY_PEN_MAIN_SHUFFLE:
            area = (mmi_pen_polygon_area_struct*) & skin_p->shuffle_area;
            point.x = pos.x ;
            point.y = pos.y ;
            if (mmi_pen_check_inside_rect(area, point))
            {
                mmi_audply_touch_shuffle_toggle();
                mmi_audply_redraw_main_shuffle(MMI_FALSE);
            }
            break;            
    #endif /* __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__ */            

		#if 1	
		case AUDPLY_PEN_MAIN_VOLUME_DEC:
			mmi_audply_redraw_main_volumectrl( 0, FALSE );
			mmi_audply_press_dec_volume();
			break;
		case AUDPLY_PEN_MAIN_VOLUME_INC:
			mmi_audply_redraw_main_volumectrl( 1, FALSE );
			mmi_audply_press_inc_volume();
			break;
		#endif	
		case AUDPLY_PEN_LSK:
// dingjian 20080124 close for TP				
//			if (g_audply.in_main_screen = TRUE)
//				return;
//			mmi_audply_redraw_LSK_up();    chenqiang modify 20101101
			if ( mmi_pen_check_inside_rect( &LSK_area, pos ) )
				ExecuteCurrKeyHandler( KEY_LSK, KEY_EVENT_UP );
			break;
		case AUDPLY_PEN_RSK:
// dingjian 20080124 close for TP				
//			if (g_audply.in_main_screen = TRUE)
//				return;
//			mmi_audply_redraw_RSK_up();   chenqiang modify 20101101
			if ( mmi_pen_check_inside_rect( &RSK_area, pos ) )
				ExecuteCurrKeyHandler( KEY_RSK, KEY_EVENT_UP );
			break;
	}

	audply_pen_event_on_object = AUDPLY_PEN_NONE;//chenhe add,20100611,if pen up done,clear this globle flag
}

void mmi_audply_pen_move_hdlr(mmi_pen_point_struct pos)
{
/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__                

    mmi_pen_point_struct point;
    mmi_pen_polygon_area_struct *area = NULL;
    const audply_skin_struct *skin_p = &g_audply_skins[g_audply.skin];
#endif	
	switch (audply_pen_event_on_object)
	{

		case AUDPLY_PEN_LSK:
			if ( mmi_pen_check_inside_rect( &LSK_area, pos ) )
			{
				mmi_audply_redraw_LSK_down();
			}
			else
			{
				mmi_audply_redraw_LSK_up();
			}
			break;
		case AUDPLY_PEN_RSK:
			if ( mmi_pen_check_inside_rect( &RSK_area, pos ) )
			{
				mmi_audply_redraw_RSK_down();
			}
			else
			{
				mmi_audply_redraw_RSK_up();
			}
			break;
		case AUDPLY_PEN_SEEK:	
		mmi_audply_touch_progress_hdlr(pos.x);
		mmi_audply_do_seeking_action(audply_main_progressbar.PlayProgress);
		mmi_audply_redraw_main_time();
		break;
#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__                
        case AUDPLY_PEN_MAIN_REPEAT:
            area = (mmi_pen_polygon_area_struct*) & skin_p->repeat_area;
            point.x = pos.x ;
            point.y = pos.y ;
            if (mmi_pen_check_inside_rect(area, point))
            {
                mmi_audply_redraw_main_repeat(MMI_TRUE);
            }else
            {
                mmi_audply_redraw_main_repeat(MMI_FALSE);
            }
            break;
        case AUDPLY_PEN_MAIN_SHUFFLE:
            area = (mmi_pen_polygon_area_struct*) & skin_p->shuffle_area;
            point.x = pos.x ;
            point.y = pos.y ;
            if (mmi_pen_check_inside_rect(area, point))
            {
                mmi_audply_redraw_main_shuffle(MMI_TRUE);
            }else
            {
                mmi_audply_redraw_main_shuffle(MMI_FALSE);
            }
            break;
#endif /*__MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__*/

	}
}
#endif
 

#if defined(__MMI_AUDIO_PLAYER_LYRIC_DISPLAY__)
MMI_BOOL mmi_audply_is_lyrics_valid(void)
{
    if (g_audply_skins[g_audply.skin].lyric_display_x >= 0)
    {
        return MMI_TRUE;
    }
    return MMI_FALSE;
}
#endif  /* __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__ */

/*************************************************************
* function:   mmi_audply_fast_forward
* purpose:    play audio fast forward
* parameters: [in] nil
*             [out] nil
* return:     void
* remarks:
*************************************************************/
extern void mmi_audply_do_seeking_action(INT32 progress);
void mmi_audply_fast_forward_time_handel(void)
{
	audply_main_progressbar.PlayProgress = audply_main_progressbar.PlayProgress + 1 * AUD_FULL_PROGRESS_CNT / 20;
	if(audply_main_progressbar.PlayProgress >= AUD_FULL_PROGRESS_CNT)
		{
			audply_main_progressbar.PlayProgress = AUD_FULL_PROGRESS_CNT;
		}
	mmi_audply_redraw_main_progressbar();
	mmi_audply_do_seeking_action(audply_main_progressbar.PlayProgress);
	mmi_audply_redraw_main_time();
	coolsand_UI_start_timer(100, mmi_audply_fast_forward_time_handel);
}

/*************************************************************
* function:   mmi_audply_fast_backward
* purpose:    play audio fast backward
* parameters: [in] nil
*             [out] nil
* return:     void
* remarks:
*************************************************************/
void mmi_audply_fast_backward_time_handel(void)
{
	audply_main_progressbar.PlayProgress = audply_main_progressbar.PlayProgress - 1 * AUD_FULL_PROGRESS_CNT / 20;
	if(audply_main_progressbar.PlayProgress <= 0)
		{
			audply_main_progressbar.PlayProgress = 0;
		}
	mmi_audply_redraw_main_progressbar();
	mmi_audply_do_seeking_action(audply_main_progressbar.PlayProgress);
	mmi_audply_redraw_main_time();
	coolsand_UI_start_timer(100, mmi_audply_fast_backward_time_handel);
}

void mmi_audply_cancel_fast_timer_no_condition(void)
{
	coolsand_UI_cancel_timer(mmi_audply_fast_backward_time_handel);
	coolsand_UI_cancel_timer(mmi_audply_fast_forward_time_handel);
}
void mmi_audply_fast_forward(void)
{
	g_audply_main_selected_button = (signed char) AUDPLY_MAIN_FORWARD_LONG_PRESS;
	mmi_audply_cancel_fast_timer_no_condition();
if(mmi_audply_get_state() == STATE_PLAY)
	{
	audply_main_progressbar.PlayProgress = mmi_audply_get_progress();
}
       mmi_audply_do_before_seek_action(audply_main_progressbar.PlayProgress);
	mmi_audply_fast_forward_time_handel();
//	mmi_audply_do_seek_action(audply_main_progressbar.PlayProgress);
}
void mmi_audply_fast_backward(void)
{   
    g_audply_main_selected_button = (signed char) AUDPLY_MAIN_BACKWARD_LONG_PRESS;
	mmi_audply_cancel_fast_timer_no_condition();
if(mmi_audply_get_state() == STATE_PLAY)
	{
    audply_main_progressbar.PlayProgress = mmi_audply_get_progress();
}
    mmi_audply_do_before_seek_action(audply_main_progressbar.PlayProgress);
    mmi_audply_fast_backward_time_handel();
}
void mmi_audply_cancel_fast_timer_handel(void)
{
	if(g_audply_main_selected_button == AUDPLY_MAIN_BACKWARD_LONG_PRESS)
		{
			coolsand_UI_cancel_timer(mmi_audply_fast_backward_time_handel);
		}
	else if(g_audply_main_selected_button == AUDPLY_MAIN_FORWARD_LONG_PRESS)
		{
			coolsand_UI_cancel_timer(mmi_audply_fast_forward_time_handel);
		}
	mmi_audply_do_seek_action(audply_main_progressbar.PlayProgress);
}

void mmi_audply_fast_timer_redraw_icon(void)
{
	
	gdi_layer_lock_frame_buffer();
	mmi_audply_redraw_main_buttons();

	if( g_audply.state == STATE_PLAY)
		coolsand_UI_show_image( audply_main_buttons[AUDPLY_MAIN_PLAY].x, audply_main_buttons[AUDPLY_MAIN_PLAY].y, GetImage(IMG_ID_AUDPLY_MAIN_SKIN1_PLAY_ON));

	gdi_layer_unlock_frame_buffer();

	coolsand_UI_BLT_double_buffer( audply_main_buttons_x1, audply_main_buttons_y1, audply_main_buttons_x2, audply_main_buttons_y2 );
}

#endif // __MMI_AUDIO_PLAYER__


