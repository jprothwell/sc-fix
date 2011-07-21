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
*	FMRadioMainScreen.c
*
* Project:
* --------
 
*
* Description:
* ------------
*   FM Radio main screen drawing program
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

#include "mmi_features.h"

#ifdef __MMI_FM_RADIO__

#include "globaldefs.h"
#include "globalscrenum.h"
#include "commonscreens.h"

#include "fmradiodef.h"
#include "fmradiotype.h"
#include "fmradioprot.h"
#include "fmradiomainscreen.h"
 
#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h"
#endif
 
#include "gdi_include.h"
#include "csw.h"

#ifdef __MMI_MAINLCD_220X176__
#include "wgui_status_icons.h"
#endif

#ifdef __GDI_MEMORY_PROFILE_2__
GDI_HANDLE	fmrdo_osd_layer;
GDI_HANDLE	fmrdo_skin_layer;
BOOL		is_fmrdo_skin_layer_drawn = FALSE;
#endif

static fmrdo_main_draw_struct fmrdo_main_draw; // all memeber variables are zero initial

extern mmi_fmrdo_struct g_fmrdo; // defined in FMRadioSrc.c
extern S32 UI_device_width;  // from gui_wrapper.c
extern S32 UI_device_height; // from gui_wrapper.c
extern const mmi_fmrdo_skin_struct g_fmrdo_skins[]; // defined in resource_fmradio_skins.c

extern void kal_wsprintf(unsigned short *outstr, char *fmt,...);
extern BOOL r2lMMIFlag; // definedi n gui_inputs.c

#define FMRDO_LSK_X_OFFSET	7
#define FMRDO_LSK_Y_OFFSET	2
#define FMRDO_RSK_X_OFFSET	6
#define FMRDO_RSK_Y_OFFSET	2
#ifdef __MMI_TOUCH_SCREEN__
 
typedef enum
{
	FMRDO_PEN_NONE,
	FMRDO_PEN_LSK,
	FMRDO_PEN_RSK,
	FMRDO_PEN_MAIN_STEP_UP,
	FMRDO_PEN_MAIN_STEP_DOWN,
	FMRDO_PEN_MAIN_POWER,
	FMRDO_PEN_MAIN_SEARCH,
	//add gdm 080515  
#if defined(__MMI_FM_RADIO_RECORD__)
	FMRDO_PEN_MAIN_RECORD,
#endif
	FMRDO_PEN_MAIN_SETTING,
	FMRDO_PEN_MAIN_VOLUME_INC,
	FMRDO_PEN_MAIN_VOLUME_DEC,
	FMRDO_PEN_MAIN_FREQ,
	FMRDO_PEN_COUNT
} FMRDO_PEN_STATE;
typedef enum
{
	FMRDO_PEN_SK_UP,
	FMRDO_PEN_SK_DOWN
} FMRDO_SOFTKEY_STATE;

S32 fmrdo_pen_event_on_object = FMRDO_PEN_NONE;
static mmi_pen_point_struct LSK_points[4];
static mmi_pen_point_struct RSK_points[4];
static mmi_pen_polygon_area_struct LSK_area;
static mmi_pen_polygon_area_struct RSK_area;
 
#endif

// sub LCD
#ifdef __MMI_SUBLCD__
scrolling_text fmrdo_sub_LCD_scrolling_title;
scrolling_text* fmrdo_sub_LCD_scrolling_title_p = NULL;
extern bitmap *current_LCD_device_bitmap; // defined in wgui.c
extern bitmap main_LCD_device_bitmap; // defined in wingui.c
extern bitmap sub_LCD_device_bitmap; // defined in wingui.c
#endif /* __MMI_SUBLCD__ */

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_init_main_images
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     S32 skin_index
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_fmrdo_init_main_images( S32 skin_index )
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S32 w, h;
#if defined(__MMI_FM_RADIO_RECORD__)	
	S32 w2,h2;	
#endif
	const mmi_fmrdo_skin_struct *skin_p = &g_fmrdo_skins[ skin_index ];

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	
	// LSK
	fmrdo_main_draw.LSK_text_border_color = coolsand_UI_color( skin_p->LSK_text_border_color_r, skin_p->LSK_text_border_color_g, skin_p->LSK_text_border_color_b );
	fmrdo_main_draw.LSK_text_color = coolsand_UI_color( skin_p->LSK_text_color_r, skin_p->LSK_text_color_g, skin_p->LSK_text_color_b );
	
	// RSK
	fmrdo_main_draw.RSK_text_border_color = coolsand_UI_color( skin_p->RSK_text_border_color_r, skin_p->RSK_text_border_color_g, skin_p->RSK_text_border_color_b );
	fmrdo_main_draw.RSK_text_color = coolsand_UI_color( skin_p->RSK_text_color_r, skin_p->RSK_text_color_g, skin_p->RSK_text_color_b );

	// background
	fmrdo_main_draw.bg_img = (UI_image_type) GetImage( skin_p->bg_image_id );
	
	// channel name
	fmrdo_main_draw.channel_name_text_border_color = coolsand_UI_color( skin_p->channel_name_text_border_color_r, skin_p->channel_name_text_border_color_g, skin_p->channel_name_text_border_color_b );
	fmrdo_main_draw.channel_name_text_color = coolsand_UI_color( skin_p->channel_name_text_color_r, skin_p->channel_name_text_color_g, skin_p->channel_name_text_color_b );
	fmrdo_main_draw.channel_name.x = skin_p->channel_name_x;
	fmrdo_main_draw.channel_name.y = skin_p->channel_name_y;
	fmrdo_main_draw.channel_name.w = skin_p->channel_name_w;
	fmrdo_main_draw.channel_name.h = skin_p->channel_name_h;
	
	// frequency
	fmrdo_main_draw.frequency_text_on_color = coolsand_UI_color( skin_p->frequency_text_on_color_r, skin_p->frequency_text_on_color_g, skin_p->frequency_text_on_color_b );
	fmrdo_main_draw.frequency_text_off_color = coolsand_UI_color( skin_p->frequency_text_off_color_r, skin_p->frequency_text_off_color_g, skin_p->frequency_text_off_color_b );
	fmrdo_main_draw.frequency.x = skin_p->frequency_x;
	fmrdo_main_draw.frequency.y = skin_p->frequency_y;
	fmrdo_main_draw.frequency.w = skin_p->frequency_w;
	fmrdo_main_draw.frequency.h = skin_p->frequency_h;
	
	// volume
	fmrdo_main_draw.volume.bg_img = (UI_image_type) GetImage( skin_p->volume_image_id );
	fmrdo_main_draw.volume.type = skin_p->volume_bar_type; // 0: horizontal, 1: vertical
	fmrdo_main_draw.volume.x = skin_p->volume_bar_x;
	fmrdo_main_draw.volume.y = skin_p->volume_bar_y;
	coolsand_UI_measure_image( fmrdo_main_draw.volume.bg_img, &w, &h );
	fmrdo_main_draw.volume.w = (S16)w;
	fmrdo_main_draw.volume.h = (S16)h;
	fmrdo_main_draw.volume.ind_start_gap = skin_p->volume_ind_start_gap;
	fmrdo_main_draw.volume.ind_end_gap = skin_p->volume_ind_end_gap;

#ifdef __MMI_FM_RADIO_ANIMATION__
	// animation
	fmrdo_main_draw.animation.x = skin_p->animation_x;
	fmrdo_main_draw.animation.y = skin_p->animation_y;	
	fmrdo_main_draw.animation.animation_img = (UI_image_type) GetImage( skin_p->animation_image_id );
#endif	

#ifdef __MMI_TOUCH_SCREEN__
	// volume inc & dec control
	fmrdo_main_draw.volume_inc_button.x = skin_p->volume_inc_x;
	fmrdo_main_draw.volume_inc_button.y = skin_p->volume_inc_y;
	fmrdo_main_draw.volume_inc_button.up_img = (UI_image_type) GetImage( skin_p->volume_inc_up_image_id );
	fmrdo_main_draw.volume_inc_button.down_img = (UI_image_type) GetImage( skin_p->volume_inc_down_image_id );
	coolsand_UI_measure_image( fmrdo_main_draw.volume_inc_button.up_img, &w, &h );
	fmrdo_main_draw.volume_inc_button.w = (S16) w;
	fmrdo_main_draw.volume_inc_button.h = (S16) h;
	fmrdo_main_draw.volume_inc_button.down = FALSE;

	fmrdo_main_draw.volume_dec_button.x = skin_p->volume_dec_x;
	fmrdo_main_draw.volume_dec_button.y = skin_p->volume_dec_y;
	fmrdo_main_draw.volume_dec_button.up_img = (UI_image_type) GetImage( skin_p->volume_dec_up_image_id );
	fmrdo_main_draw.volume_dec_button.down_img = (UI_image_type) GetImage( skin_p->volume_dec_down_image_id );
	coolsand_UI_measure_image( fmrdo_main_draw.volume_dec_button.up_img, &w, &h );
	fmrdo_main_draw.volume_dec_button.w = (S16) w;
	fmrdo_main_draw.volume_dec_button.h = (S16) h;
	fmrdo_main_draw.volume_dec_button.down = FALSE;

	// setting icon
	fmrdo_main_draw.setting_button.x = skin_p->setting_x;
	fmrdo_main_draw.setting_button.y = skin_p->setting_y;
	fmrdo_main_draw.setting_button.up_img = (UI_image_type) GetImage( skin_p->setting_up_image_id );
	fmrdo_main_draw.setting_button.down_img = (UI_image_type) GetImage( skin_p->setting_down_image_id );
	coolsand_UI_measure_image( fmrdo_main_draw.setting_button.up_img, &w, &h );
	fmrdo_main_draw.setting_button.w = (S16) w;
	fmrdo_main_draw.setting_button.h = (S16) h;
	fmrdo_main_draw.setting_button.down = FALSE;
#endif

	// tuner scale
	fmrdo_main_draw.tuner_scale.bg_img = (UI_image_type) GetImage( skin_p->tuner_scale_bg_image_id );
	fmrdo_main_draw.tuner_scale.ind_img = (UI_image_type) GetImage( skin_p->tuner_scale_ind_image_id );
	fmrdo_main_draw.tuner_scale.type = skin_p->tuner_scale_type; // 0: horizontal, 1: vertical
	fmrdo_main_draw.tuner_scale.x = skin_p->tuner_scale_x;
	fmrdo_main_draw.tuner_scale.y = skin_p->tuner_scale_y;
	coolsand_UI_measure_image( fmrdo_main_draw.tuner_scale.bg_img, &w, &h );
	fmrdo_main_draw.tuner_scale.w = (S16)w;
	fmrdo_main_draw.tuner_scale.h = (S16)h;
	//fmrdo_main_draw.tuner_scale.w = 89;
	//fmrdo_main_draw.tuner_scale.h = 26;

	coolsand_UI_measure_image( fmrdo_main_draw.tuner_scale.ind_img, &w, &h );
	fmrdo_main_draw.tuner_scale.ind_w = (S16)w;
	fmrdo_main_draw.tuner_scale.ind_h = (S16)h;
	fmrdo_main_draw.tuner_scale.ind_offset = skin_p->tunner_scale_ind_offset;
	fmrdo_main_draw.tuner_scale.ind_start_gap = skin_p->tuner_scale_start_gap;
	fmrdo_main_draw.tuner_scale.ind_end_gap = skin_p->tuner_scale_end_gap;
	
	// step down button
	fmrdo_main_draw.step_down_button.up_img = (UI_image_type) GetImage( skin_p->step_down_button_up_image_id );
	fmrdo_main_draw.step_down_button.down_img = (UI_image_type) GetImage( skin_p->step_down_button_down_image_id );
	fmrdo_main_draw.step_down_button.x = skin_p->step_down_button_x;
	fmrdo_main_draw.step_down_button.y = skin_p->step_down_button_y;
	coolsand_UI_measure_image( fmrdo_main_draw.step_down_button.up_img, &w, &h );
	fmrdo_main_draw.step_down_button.w = (S16)w;
	fmrdo_main_draw.step_down_button.h = (S16)h;
	
	// step up button
	fmrdo_main_draw.step_up_button.up_img = (UI_image_type) GetImage( skin_p->step_up_button_up_image_id );
	fmrdo_main_draw.step_up_button.down_img = (UI_image_type) GetImage( skin_p->step_up_button_down_image_id );
	fmrdo_main_draw.step_up_button.x = skin_p->step_up_button_x;
	fmrdo_main_draw.step_up_button.y = skin_p->step_up_button_y;
	coolsand_UI_measure_image( fmrdo_main_draw.step_up_button.up_img, &w, &h );
	fmrdo_main_draw.step_up_button.w = (S16)w;
	fmrdo_main_draw.step_up_button.h = (S16)h;
	
	// search button
	fmrdo_main_draw.search_button.on_up_img = (UI_image_type) GetImage( skin_p->search_button_on_up_image_id );
	fmrdo_main_draw.search_button.on_down_img = (UI_image_type) GetImage( skin_p->search_button_on_down_image_id );
	fmrdo_main_draw.search_button.off_up_img = (UI_image_type) GetImage( skin_p->search_button_off_up_image_id );
	fmrdo_main_draw.search_button.off_down_img = (UI_image_type) GetImage( skin_p->search_button_off_down_image_id );
	fmrdo_main_draw.search_button.x = skin_p->search_button_x;
	fmrdo_main_draw.search_button.y = skin_p->search_button_y;
	coolsand_UI_measure_image( fmrdo_main_draw.search_button.on_up_img, &w, &h );
	fmrdo_main_draw.search_button.w = (S16)w;
	fmrdo_main_draw.search_button.h = (S16)h;
	
	// power button
	fmrdo_main_draw.power_button.on_up_img = (UI_image_type) GetImage( skin_p->power_button_on_up_image_id );
	fmrdo_main_draw.power_button.on_down_img = (UI_image_type) GetImage( skin_p->power_button_on_down_image_id );
	fmrdo_main_draw.power_button.off_up_img = (UI_image_type) GetImage( skin_p->power_button_off_up_image_id );
	fmrdo_main_draw.power_button.off_down_img = (UI_image_type) GetImage( skin_p->power_button_off_down_image_id );
	fmrdo_main_draw.power_button.x = skin_p->power_button_x;
	fmrdo_main_draw.power_button.y = skin_p->power_button_y;
	coolsand_UI_measure_image( fmrdo_main_draw.power_button.on_up_img, &w, &h );
	fmrdo_main_draw.power_button.w = (S16)w;
	fmrdo_main_draw.power_button.h = (S16)h;

#if defined(__MMI_FM_RADIO_RECORD__)	
	// Record Indication Icon
	fmrdo_main_draw.rec_button.on_up_img = (UI_image_type) GetImage( skin_p->rec_button_image_id); /*Use same image for all*/
	fmrdo_main_draw.rec_button.on_down_img = (UI_image_type) GetImage( skin_p->rec_button_image_id );
	fmrdo_main_draw.rec_button.off_up_img = (UI_image_type) GetImage( skin_p->rec_button_image_id );
	fmrdo_main_draw.rec_button.off_down_img = (UI_image_type) GetImage( skin_p->rec_button_image_id );
	fmrdo_main_draw.rec_button.x = skin_p->rec_button_x;
	fmrdo_main_draw.rec_button.y = skin_p->rec_button_y;
	coolsand_UI_measure_image( fmrdo_main_draw.rec_button.on_up_img, &w, &h );
	fmrdo_main_draw.rec_button.w = (S16)w;
	fmrdo_main_draw.rec_button.h = (S16)h;

	// Pause Indication Icon
	fmrdo_main_draw.rec_pause_button.on_up_img = (UI_image_type) GetImage( skin_p->rec_pause_button_image_id); /*Use same image for all*/
	fmrdo_main_draw.rec_pause_button.on_down_img = (UI_image_type) GetImage( skin_p->rec_pause_button_image_id );
	fmrdo_main_draw.rec_pause_button.off_up_img = (UI_image_type) GetImage( skin_p->rec_pause_button_image_id );
	fmrdo_main_draw.rec_pause_button.off_down_img = (UI_image_type) GetImage( skin_p->rec_pause_button_image_id );
	fmrdo_main_draw.rec_pause_button.x = skin_p->rec_button_x;
	fmrdo_main_draw.rec_pause_button.y = skin_p->rec_button_y;
	coolsand_UI_measure_image( fmrdo_main_draw.rec_pause_button.on_up_img, &w, &h );
	fmrdo_main_draw.rec_pause_button.w = (S16)w;
	fmrdo_main_draw.rec_pause_button.h = (S16)h;

	//Record Duration
	fmrdo_main_draw.rec_base_num_id = skin_p->rec_duration_number_id_base;
	fmrdo_main_draw.rec_num_col_id = 	skin_p->rec_duration_number_col_image_id;

	coolsand_UI_measure_image( (UI_image_type) GetImage(fmrdo_main_draw.rec_base_num_id), &w, &h );
	coolsand_UI_measure_image( (UI_image_type) GetImage(fmrdo_main_draw.rec_num_col_id), &w2, &h2 );	
	
	fmrdo_main_draw.timer_hr_0_offset_x 	= skin_p->rec_duration_x;
	fmrdo_main_draw.timer_hr_1_offset_x	= fmrdo_main_draw.timer_hr_0_offset_x + w;
	fmrdo_main_draw.timer_col_0_offset_x	= fmrdo_main_draw.timer_hr_1_offset_x + w;
	fmrdo_main_draw.timer_min_0_offset_x	= fmrdo_main_draw.timer_col_0_offset_x + w2;
	fmrdo_main_draw.timer_min_1_offset_x	= fmrdo_main_draw.timer_min_0_offset_x + w;
	fmrdo_main_draw.timer_col_1_offset_x	= fmrdo_main_draw.timer_min_1_offset_x + w;
	fmrdo_main_draw.timer_sec_0_offset_x	= fmrdo_main_draw.timer_col_1_offset_x + w2;
	fmrdo_main_draw.timer_sec_1_offset_x	= fmrdo_main_draw.timer_sec_0_offset_x + w;

	fmrdo_main_draw.timer_offset_y 	= skin_p->rec_duration_y;
	fmrdo_main_draw.timer_offset_x2 	= fmrdo_main_draw.timer_sec_1_offset_x + w;
	fmrdo_main_draw.timer_offset_y2 	= fmrdo_main_draw.timer_offset_y + h;	
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_redraw_subLCD_background
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
void mmi_fmrdo_redraw_subLCD_background( S32 x1, S32 y1, S32 x2, S32 y2 )
{
#ifdef __MMI_SUBLCD__
	color_t c = coolsand_UI_color( 255, 255, 255 );
	coolsand_UI_fill_rectangle( x1, y1, x2, y2, c);
#endif // __MMI_SUBLCD__
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_subLCD_text_scroll_handler
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
void mmi_fmrdo_subLCD_text_scroll_handler( void )
{
#ifdef __MMI_SUBLCD__
	coolsand_UI_handle_scrolling_text( &fmrdo_sub_LCD_scrolling_title );
#endif // __MMI_SUBLCD__
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_show_subLCD
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
void mmi_fmrdo_redraw_subLCD( BOOL is_show )
{
#ifdef __MMI_SUBLCD__
	color_t text_color = coolsand_UI_color( 0, 0, 0 );
	S32 x1, y1, x2, y2, x, y, str_w, str_h;
	bitmap *saved_graphics_context = current_LCD_device_bitmap;
	UI_string_type title;

	if( !g_fmrdo.is_ready )
		return;

	mmi_fmrdo_set_subLCD_shown( TRUE );
	
	UI_set_sub_LCD_graphics_context();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	if( fmrdo_sub_LCD_scrolling_title_p != NULL )
	{
		coolsand_UI_scrolling_text_stop( fmrdo_sub_LCD_scrolling_title_p );
		fmrdo_sub_LCD_scrolling_title_p = NULL;
	}

	// clear background
	x1 = 0;
	y1 = 13;
	x2 = UI_device_width - 1;
	y2 = UI_device_height - 1;
	coolsand_UI_push_clip();
	coolsand_UI_set_clip( x1, y1, x2, y2 );
	mmi_fmrdo_redraw_subLCD_background( x1, y1, x2, y2 );

	if( is_show )
	{
		title = (UI_string_type) mmi_fmrdo_get_channel_name_or_freq();
		
		// draw song name
		coolsand_UI_set_font( &MMI_medium_font );
		coolsand_UI_measure_string( title, &str_w, &str_h );
		x = (UI_device_width - str_w)/2;
		y = 12 + (UI_device_height - 12 - str_h)/2;
		if( x >= 0 )
		{
			coolsand_UI_push_text_clip();
			coolsand_UI_set_text_clip( x, y, x + str_w - 1, y + str_h - 1 );
			coolsand_UI_set_text_color( text_color );
			coolsand_UI_move_text_cursor( x, y );
			coolsand_UI_print_text( title );
			coolsand_UI_pop_text_clip();
		}
		else
		{
			// scroll
			x = 0;
			fmrdo_sub_LCD_scrolling_title_p = &fmrdo_sub_LCD_scrolling_title;
			coolsand_UI_create_scrolling_text( fmrdo_sub_LCD_scrolling_title_p,
				x + 2, y, UI_device_width - 4, str_h, title,
				mmi_fmrdo_subLCD_text_scroll_handler, mmi_fmrdo_redraw_subLCD_background,
				text_color, text_color );
			fmrdo_sub_LCD_scrolling_title_p->text_font = &MMI_medium_font;
			coolsand_UI_show_scrolling_text( fmrdo_sub_LCD_scrolling_title_p );
		}
	}
	
	coolsand_UI_pop_clip();
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1, x2, y2 );
#endif

	if( saved_graphics_context == &main_LCD_device_bitmap )
		UI_set_main_LCD_graphics_context();
	else if( saved_graphics_context == &sub_LCD_device_bitmap )
		UI_set_sub_LCD_graphics_context();
#endif // __MMI_SUBLCD__
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_exit_subLCD
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
void mmi_fmrdo_exit_subLCD()
{
#ifdef __MMI_SUBLCD__
	if( g_fmrdo.is_subLCD_shown )
	{
		bitmap *saved_graphics_context = current_LCD_device_bitmap;
	
		UI_set_sub_LCD_graphics_context();

		if( fmrdo_sub_LCD_scrolling_title_p != NULL )
		{
			coolsand_UI_scrolling_text_stop( fmrdo_sub_LCD_scrolling_title_p );
			fmrdo_sub_LCD_scrolling_title_p = NULL;
		}
		if( saved_graphics_context == &main_LCD_device_bitmap )
			UI_set_main_LCD_graphics_context();
		else if( saved_graphics_context == &sub_LCD_device_bitmap )
			UI_set_sub_LCD_graphics_context();

		mmi_fmrdo_set_subLCD_shown( FALSE );
	}
#endif // __MMI_SUBLCD__
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_show_main
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     U16 left_softkey
*     U16 right_softkey
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_fmrdo_show_main( U16 left_softkey, U16 right_softkey, U8 skin_index )
{
        FMR_TRACE();

#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_layer_multi_layer_enable();

	/* get base layer as skin layer */
	gdi_layer_get_base_handle( &fmrdo_osd_layer );
	gdi_layer_clear_background( GDI_COLOR_TRANSPARENT );
	gdi_layer_set_source_key( TRUE, GDI_COLOR_TRANSPARENT );
	gdi_layer_create( 0, 0, UI_device_width, UI_device_height, &fmrdo_skin_layer );
	gdi_layer_set_blt_layer( fmrdo_skin_layer, fmrdo_osd_layer, 0, 0 );
#endif

	// init images
	if( !fmrdo_main_draw.is_ready || fmrdo_main_draw.skin_index != skin_index )
	{
		mmi_fmrdo_init_main_images( skin_index );
		fmrdo_main_draw.skin_index = skin_index;
		fmrdo_main_draw.is_ready = TRUE;
		fmrdo_main_draw.is_channel_name_scroll_ready = FALSE;
	}
#ifdef __GDI_MEMORY_PROFILE_2__
	is_fmrdo_skin_layer_drawn = FALSE;
#endif

	clear_key_handlers();
	clear_left_softkey();
	clear_right_softkey();
	fmrdo_main_draw.LSK_text = (UI_string_type) GetString( left_softkey );
	fmrdo_main_draw.RSK_text = (UI_string_type) GetString( right_softkey );
	SetKeyHandler( mmi_fmrdo_redraw_main_LSK_down, KEY_LSK, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_fmrdo_redraw_main_RSK_down, KEY_RSK, KEY_EVENT_DOWN );

	// associate functions
	ExitCategoryFunction = mmi_fmrdo_hide_main;
	RedrawCategoryFunction = mmi_fmrdo_redraw_main_all;
	GetCategoryHistory = dummy_get_history;
	GetCategoryHistorySize = dummy_get_history_size;
	
	// draw screen
	mmi_fmrdo_redraw_main_all();
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_hide_main
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
void mmi_fmrdo_hide_main( void )
{
	coolsand_UI_scrolling_text_stop( &fmrdo_main_draw.channel_name_scroll );

#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_layer_flatten_to_base( fmrdo_skin_layer, fmrdo_osd_layer, 0, 0 );
	gdi_layer_free( fmrdo_skin_layer );
	gdi_layer_multi_layer_disable();

	/* restore blt layer as base layer only */
	gdi_layer_set_blt_layer( fmrdo_osd_layer, 0, 0, 0 );
#endif
#ifdef __MMI_TOUCH_SCREEN__
	fmrdo_main_draw.volume_dec_button.down = FALSE;
	fmrdo_main_draw.volume_inc_button.down = FALSE;
	fmrdo_main_draw.setting_button.down = FALSE;

	fmrdo_pen_event_on_object = FMRDO_PEN_NONE;
#endif

#ifdef __MMI_FM_RADIO_ANIMATION__
	// stop animation
	mmi_fmrdo_animation_stop();
#endif
	// associate functions
	ExitCategoryFunction = MMI_dummy_function;
	RedrawCategoryFunction = MMI_dummy_function;
	GetCategoryHistory = dummy_get_history;
	GetCategoryHistorySize = dummy_get_history_size;
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_redraw_main_LSK_down
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
void mmi_fmrdo_redraw_main_LSK_down( void )
{
	S32 str_w, str_h;
	S32 x1, y1, x2, y2;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif
	coolsand_UI_set_font( &MMI_medium_font );
	coolsand_UI_set_text_color( fmrdo_main_draw.LSK_text_color );
	coolsand_UI_set_text_border_color( fmrdo_main_draw.LSK_text_border_color );
	
	// show LSK
	coolsand_UI_measure_string( fmrdo_main_draw.LSK_text, &str_w, &str_h );
	x1 = FMRDO_LSK_X_OFFSET;
	y1 = UI_device_height - str_h - FMRDO_LSK_Y_OFFSET;
	x2 = str_w + FMRDO_LSK_X_OFFSET;
	y2 = UI_device_height - FMRDO_LSK_Y_OFFSET;
	coolsand_UI_push_clip();
	coolsand_UI_set_clip( x1-2, y1-2, x2, y2 );
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( x1-2, y1-2, x2, y2, GDI_COLOR_TRANSPARENT );
#else
	coolsand_UI_show_image( 0, 0, fmrdo_main_draw.bg_img );
#endif
	coolsand_UI_pop_clip();
	coolsand_UI_push_text_clip();
	coolsand_UI_set_text_clip( x1, y1-1, x2, y2 );
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x2, y1 );
	else
		coolsand_UI_move_text_cursor( x1, y1 );

#ifdef __MMI_MAINLCD_220X176__
	coolsand_UI_print_text( fmrdo_main_draw.LSK_text );
#else
	coolsand_UI_print_bordered_text( fmrdo_main_draw.LSK_text );
#endif
	coolsand_UI_pop_text_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1-2, y1-2, x2, y2 );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_redraw_main_RSK_down
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
void mmi_fmrdo_redraw_main_RSK_down( void )
{
	S32 str_w, str_h;
	S32 x1, y1, x2, y2;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif
	coolsand_UI_set_font( &MMI_medium_font );
	coolsand_UI_set_text_color( fmrdo_main_draw.RSK_text_color );
	coolsand_UI_set_text_border_color( fmrdo_main_draw.RSK_text_border_color );
	
	// show RSK
	coolsand_UI_measure_string( fmrdo_main_draw.RSK_text, &str_w, &str_h );
	x1 = UI_device_width - str_w - FMRDO_RSK_X_OFFSET;
	y1 = UI_device_height - str_h - FMRDO_RSK_Y_OFFSET;
	x2 = UI_device_width - FMRDO_RSK_X_OFFSET;
	y2 = UI_device_height - FMRDO_RSK_Y_OFFSET;
	coolsand_UI_push_clip();
	coolsand_UI_set_clip( x1-2, y1-2, x2, y2 );
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( x1-2, y1-2, x2, y2, GDI_COLOR_TRANSPARENT );
#else
	coolsand_UI_show_image( 0, 0, fmrdo_main_draw.bg_img );
#endif
	coolsand_UI_pop_clip();
	coolsand_UI_push_text_clip();
	coolsand_UI_set_text_clip( x1, y1-1, x2, y2 );
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x2, y1 );
	else
		coolsand_UI_move_text_cursor( x1, y1 );

#ifdef __MMI_MAINLCD_220X176__
	coolsand_UI_print_text( fmrdo_main_draw.RSK_text );
#else
	coolsand_UI_print_bordered_text( fmrdo_main_draw.RSK_text );
#endif

	coolsand_UI_pop_text_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1-2, y1-2, x2, y2 );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_redraw_main_LSK_up
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
void mmi_fmrdo_redraw_main_LSK_up( void )
{
	S32 str_w, str_h;
	S32 x1, y1, x2, y2;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif	
	coolsand_UI_set_font( &MMI_medium_font );
	coolsand_UI_set_text_color( fmrdo_main_draw.LSK_text_color );
	coolsand_UI_set_text_border_color( fmrdo_main_draw.LSK_text_border_color );
	
	// show LSK
	coolsand_UI_measure_string( fmrdo_main_draw.LSK_text, &str_w, &str_h );
	x1 = FMRDO_LSK_X_OFFSET - 1;
	y1 = UI_device_height - str_h - FMRDO_LSK_Y_OFFSET - 1;
	x2 = str_w + FMRDO_LSK_X_OFFSET - 1;
	y2 = UI_device_height - FMRDO_LSK_Y_OFFSET - 1;
	coolsand_UI_push_clip();
	coolsand_UI_set_clip( x1, y1-1, UI_device_width/2 - 10, y2+1 );
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( x1, y1-1, UI_device_width/2 - 10, y2+1, GDI_COLOR_TRANSPARENT );
#else
	coolsand_UI_show_image( 0, 0, fmrdo_main_draw.bg_img );
#endif
	coolsand_UI_pop_clip();
	coolsand_UI_push_text_clip();
	coolsand_UI_set_text_clip( x1, y1-1, x2, y2 );
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x2, y1 );
	else
		coolsand_UI_move_text_cursor( x1, y1 );
#ifdef __MMI_MAINLCD_220X176__
	coolsand_UI_print_text( fmrdo_main_draw.LSK_text );
#else
	coolsand_UI_print_bordered_text( fmrdo_main_draw.LSK_text );
#endif


	coolsand_UI_pop_text_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1-1, UI_device_width/2 - 10, y2+1 );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_redraw_main_RSK_up
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
void mmi_fmrdo_redraw_main_RSK_up( void )
{
	S32 str_w, str_h;
	S32 x1, y1, x2, y2;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif	
	coolsand_UI_set_font( &MMI_medium_font );
	coolsand_UI_set_text_color( fmrdo_main_draw.RSK_text_color );
	coolsand_UI_set_text_border_color( fmrdo_main_draw.RSK_text_border_color );
	
	// show RSK
	coolsand_UI_measure_string( fmrdo_main_draw.RSK_text, &str_w, &str_h );
	x1 = UI_device_width - str_w - FMRDO_RSK_X_OFFSET - 1;
	y1 = UI_device_height - str_h - FMRDO_RSK_Y_OFFSET - 1;
	x2 = UI_device_width - FMRDO_RSK_X_OFFSET-1;
	y2 = UI_device_height - FMRDO_RSK_Y_OFFSET-1;
	coolsand_UI_push_clip();
	coolsand_UI_set_clip( UI_device_width/2 + 10, y1-1, x2+1, y2+1 );
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect(UI_device_width/2 + 10, y1-1, x2+1, y2+1, GDI_COLOR_TRANSPARENT);
#else
	coolsand_UI_show_image( 0, 0, fmrdo_main_draw.bg_img );
#endif
	coolsand_UI_pop_clip();
	coolsand_UI_push_text_clip();
	coolsand_UI_set_text_clip( x1, y1-1, x2, y2 );
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x2, y1 );
	else
		coolsand_UI_move_text_cursor( x1, y1 );
#ifdef __MMI_MAINLCD_220X176__
	coolsand_UI_print_text( fmrdo_main_draw.RSK_text );
#else
	coolsand_UI_print_bordered_text( fmrdo_main_draw.RSK_text );
#endif


	coolsand_UI_pop_text_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( UI_device_width/2 + 10, y1-1, x2+1, y2+1 );
#endif
}


/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_scroll_channel_name_hdlr
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
void mmi_fmrdo_scroll_channel_name_hdlr( void )
{
	coolsand_UI_handle_scrolling_text( &fmrdo_main_draw.channel_name_scroll );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_redraw_main_background
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
void mmi_fmrdo_redraw_main_background( S32 x1, S32 y1, S32 x2, S32 y2 )
{
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( x1, y1, x2, y2, GDI_COLOR_TRANSPARENT );
#else
	// we don't set clip here because its caller will do that
	coolsand_UI_show_image( 0, 0, fmrdo_main_draw.bg_img );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_redraw_main_all
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
void mmi_fmrdo_redraw_main_all( void )
{
	S32 str_w, str_h;
	S32 x1, y1, x2, y2;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif
	
	coolsand_UI_push_clip();
	coolsand_UI_push_text_clip();
	
	coolsand_UI_set_clip( 0, 0, UI_device_width - 1, UI_device_height - 1 );
	
#ifdef __GDI_MEMORY_PROFILE_2__
	if( !is_fmrdo_skin_layer_drawn )
	{
		gdi_layer_push_and_set_active( fmrdo_skin_layer );
#endif
#ifdef __MMI_MAINLCD_220X176__
	gdi_layer_clear(GDI_COLOR_BLACK);
#endif





		// main background
#ifdef __MMI_MAINLCD_220X176__
		coolsand_UI_show_image( 0, 18, fmrdo_main_draw.bg_img );
#else
		coolsand_UI_show_image( 0, 0, fmrdo_main_draw.bg_img );
#endif
		// animation
#ifdef __MMI_FM_RADIO_ANIMATION__
		fmrdo_main_draw.animation.img_handle = GDI_ERROR_HANDLE;
		if(g_fmrdo.is_power_on)
		{
			gdi_image_draw_animation( fmrdo_main_draw.animation.x,fmrdo_main_draw.animation.y, fmrdo_main_draw.animation.animation_img,&fmrdo_main_draw.animation.img_handle);
		}
		else
		{
			gdi_image_draw(fmrdo_main_draw.animation.x, fmrdo_main_draw.animation.y, fmrdo_main_draw.animation.animation_img);
		}
#endif

#ifdef __GDI_MEMORY_PROFILE_2__
		// tuner scale background
		coolsand_UI_show_image( fmrdo_main_draw.tuner_scale.x, fmrdo_main_draw.tuner_scale.y, fmrdo_main_draw.tuner_scale.bg_img );
		is_fmrdo_skin_layer_drawn = TRUE;
		gdi_layer_pop_and_restore_active();
	}
#endif

	// show LSK
	coolsand_UI_set_font( &MMI_medium_font );
	coolsand_UI_set_text_color( fmrdo_main_draw.LSK_text_color );
	coolsand_UI_set_text_border_color( fmrdo_main_draw.LSK_text_border_color );
	coolsand_UI_measure_string( fmrdo_main_draw.LSK_text, &str_w, &str_h );

	x1 = FMRDO_LSK_X_OFFSET-1;
	y1 = UI_device_height - str_h - FMRDO_LSK_Y_OFFSET-1;
	x2 = str_w + FMRDO_LSK_X_OFFSET-1;
	y2 = UI_device_height - FMRDO_LSK_Y_OFFSET-1;
	coolsand_UI_set_text_clip( x1, y1-1, x2, y2 );
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x2, y1 );
	else
		coolsand_UI_move_text_cursor( x1, y1 );

#if (defined(__MMI_MAINLCD_220X176__) || defined(__MMI_NOKIA_STYLE_N800__))
	coolsand_UI_print_text( fmrdo_main_draw.LSK_text );
#else
	coolsand_UI_print_bordered_text( fmrdo_main_draw.LSK_text );
#endif

#ifdef __MMI_TOUCH_SCREEN__
	LSK_area.num = 4;
	LSK_area.points = (mmi_pen_point_struct*) LSK_points;
	LSK_points[0].x = LSK_points[2].x = x1;
	LSK_points[0].y = LSK_points[1].y = y1;
	LSK_points[3].x = LSK_points[1].x = x2;
	LSK_points[3].y = LSK_points[2].y = y2;
#endif
	// show RSK
	coolsand_UI_set_text_color( fmrdo_main_draw.RSK_text_color );
	coolsand_UI_set_text_border_color( fmrdo_main_draw.RSK_text_border_color );
	coolsand_UI_measure_string( fmrdo_main_draw.RSK_text, &str_w, &str_h );
	x1 = UI_device_width - str_w - FMRDO_RSK_X_OFFSET-1;
	y1 = UI_device_height - str_h - FMRDO_RSK_Y_OFFSET-1;
	x2 = UI_device_width - FMRDO_RSK_X_OFFSET-1;
	y2 = UI_device_height - FMRDO_RSK_Y_OFFSET-1;
	coolsand_UI_set_text_clip( x1, y1-1, x2, y2 );
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x2, y1 );
	else
		coolsand_UI_move_text_cursor( x1, y1 );
#if (defined(__MMI_MAINLCD_220X176__) || defined(__MMI_NOKIA_STYLE_N800__))
	coolsand_UI_print_text( fmrdo_main_draw.RSK_text );
#else
	coolsand_UI_print_bordered_text( fmrdo_main_draw.RSK_text );
#endif

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

	// draw components	
	mmi_fmrdo_redraw_main_frequency( g_fmrdo.frequency, TRUE );
	mmi_fmrdo_redraw_main_channel_name( 0 );
	mmi_fmrdo_redraw_main_channel_tuner( g_fmrdo.frequency );
	mmi_fmrdo_redraw_main_volume();
	mmi_fmrdo_redraw_main_push_button( FMRDO_BUTTON_STEP_DOWN, 
		(BOOL)(g_fmrdo.is_button_down && (g_fmrdo.selected_button == FMRDO_BUTTON_STEP_DOWN)) );
	mmi_fmrdo_redraw_main_push_button( FMRDO_BUTTON_STEP_UP, 
		(BOOL)(g_fmrdo.is_button_down && (g_fmrdo.selected_button == FMRDO_BUTTON_STEP_UP)) );
	mmi_fmrdo_redraw_main_toggle_button( FMRDO_BUTTON_SEARCH,
		(BOOL)(g_fmrdo.is_button_down && (g_fmrdo.selected_button == FMRDO_BUTTON_SEARCH)),
		g_fmrdo.is_search_on ,TRUE);
	mmi_fmrdo_redraw_main_toggle_button( FMRDO_BUTTON_POWER,
		(BOOL)(g_fmrdo.is_button_down && (g_fmrdo.selected_button == FMRDO_BUTTON_POWER)),
		g_fmrdo.is_power_on ,TRUE);
#ifdef __MMI_TOUCH_SCREEN__
	mmi_fmrdo_redraw_main_push_down_button( FMRDO_BUTTON_VOL_DEC, fmrdo_main_draw.volume_dec_button.down );
	mmi_fmrdo_redraw_main_push_down_button( FMRDO_BUTTON_VOL_INC, fmrdo_main_draw.volume_inc_button.down );
	mmi_fmrdo_redraw_main_push_down_button( FMRDO_BUTTON_SETTINGS, fmrdo_main_draw.setting_button.down );
#endif

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( 0, 0, UI_device_width - 1, UI_device_height - 1 );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_redraw_main_frequency
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
void mmi_fmrdo_redraw_main_frequency( U16 freq, BOOL is_shown )
{
	S32 x, y;
	S32 x1 = fmrdo_main_draw.frequency.x;
	S32 y1 = fmrdo_main_draw.frequency.y;
	S32 x2 = fmrdo_main_draw.frequency.x + fmrdo_main_draw.frequency.w - 1;
	S32 y2 = fmrdo_main_draw.frequency.y + fmrdo_main_draw.frequency.h - 1;
	S32 prefix_str_w, prefix_str_h, str_w, str_h;

	//UI_character_type prefix_str[] = {'F',0,'M',0,0,0}; //wrong
	//UI_character_type prefix_str[] = {'F','M',0};          //right
	UI_character_type prefix_str[4];
	S8 prefix_str_temp[] = {'F','M',0};
	U8 string[20];
	UI_character_type str[32];
	
	//kal_wsprintf( str, "%d.%d", freq / 10, freq % 10 );
	AnsiiToUnicodeString((S8 *)prefix_str, prefix_str_temp);
	sprintf(string,"%d.%d", freq / 10, freq % 10);
	AnsiiToUnicodeString((S8 *)str, string);

	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	coolsand_UI_push_clip();
	coolsand_UI_set_clip( x1, y1, x2, y2 );

	// background & text	
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( x1, y1, x2, y2, GDI_COLOR_TRANSPARENT );
#else
	coolsand_UI_show_image( 0, 0, fmrdo_main_draw.bg_img );
#endif
	
	if( is_shown )
	{
		coolsand_UI_push_text_clip();
		coolsand_UI_set_text_clip( x1, y1, x2, y2 );
//wanjian 2006-07-19
#if 0
		coolsand_UI_set_text_color(coolsand_UI_color(255,254,255));
#else
		if( g_fmrdo.is_power_on )
			coolsand_UI_set_text_color( fmrdo_main_draw.frequency_text_on_color );
		else
			coolsand_UI_set_text_color( fmrdo_main_draw.frequency_text_off_color );
#endif
//wanjian 2006-07-19
		coolsand_UI_set_font( &MMI_medium_font );
		coolsand_UI_measure_string( str, &str_w, &str_h );
		coolsand_UI_set_font( &MMI_small_font );
		coolsand_UI_measure_string( prefix_str, &prefix_str_w, &prefix_str_h );
		x = x1 + (fmrdo_main_draw.frequency.w - str_w - prefix_str_w)/2;
		y = y1 + (fmrdo_main_draw.frequency.h - str_h)/2;
	
		// prefix "FM "
		if( r2lMMIFlag )
		{	
			FMR_TRACE();
			coolsand_UI_move_text_cursor( x + prefix_str_w - 1, y + str_h - prefix_str_h - 1 );
		}
		else
		{
			FMR_TRACE();
			coolsand_UI_move_text_cursor( x, y + str_h - prefix_str_h - 1 );
		}
		coolsand_UI_print_text( prefix_str );
	
		// frequency number
		coolsand_UI_set_font( &MMI_medium_font );
				
		if( r2lMMIFlag )
		{
			coolsand_UI_move_text_cursor( x + prefix_str_w + str_w - 1, y );
		}
		else
		{
			coolsand_UI_move_text_cursor( x + prefix_str_w, y );
		}
		coolsand_UI_print_text( str );
	
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
*     mmi_fmrdo_redraw_main_channel_name
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
void mmi_fmrdo_redraw_main_channel_name( UI_string_type other_str )
{
#ifndef __MMI_MAINLCD_220X176__
	S32 x1 = fmrdo_main_draw.channel_name.x;
	S32 y1 = fmrdo_main_draw.channel_name.y;
	S32 x2 = fmrdo_main_draw.channel_name.x + fmrdo_main_draw.channel_name.w - 1;
	S32 y2 = fmrdo_main_draw.channel_name.y + fmrdo_main_draw.channel_name.h - 1;
	S32 str_w, str_h;
	UI_string_type str;
	
	U8 str1[MAX_NAME_INPUT_LEN];
	U8 str2[MAX_NAME_INPUT_LEN];	

	
	if( other_str == 0 )
	{
		if( g_fmrdo.channel_index >= 0 )
		{
			str = fmrdo_main_draw.channel_name_str;
			
			//kal_wsprintf( str, "%d. %w", g_fmrdo.channel_index + 1, g_fmrdo.channel_list.name[g_fmrdo.channel_index] );

			sprintf(str1, "%d.", g_fmrdo.channel_index + 1);
			AnsiiToUnicodeString(str2, str1);
			pfnUnicodeStrcpy((S8 *)str,str2);
			pfnUnicodeStrcat((S8 *)str,(const S8 *)g_fmrdo.channel_list.name[g_fmrdo.channel_index]);
		}
		else
		{
			str = (UI_string_type) GetString( STR_ID_FMRDO_TITLE );
		}
	}
	else
		str = other_str;

	// stop previous scroll text
	if( !fmrdo_main_draw.is_channel_name_scroll_ready )
		coolsand_UI_scrolling_text_stop( &fmrdo_main_draw.channel_name_scroll );
	else
		fmrdo_main_draw.is_channel_name_scroll_ready = TRUE;

	coolsand_UI_create_scrolling_text( &fmrdo_main_draw.channel_name_scroll,
		x1 + 2, y1 + 1, fmrdo_main_draw.channel_name.w - 4, fmrdo_main_draw.channel_name.h - 2, str,
		mmi_fmrdo_scroll_channel_name_hdlr, mmi_fmrdo_redraw_main_background,
		fmrdo_main_draw.channel_name_text_color, fmrdo_main_draw.channel_name_text_border_color );
	fmrdo_main_draw.channel_name_scroll.text_font = &MMI_medium_font;
	fmrdo_main_draw.channel_name_scroll.flags |= UI_SCROLLING_TEXT_BORDERED_TEXT;

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	coolsand_UI_push_clip();
	coolsand_UI_set_clip( x1, y1, x2, y2 );

	// stop scrolling if necessary
	coolsand_UI_scrolling_text_stop( &fmrdo_main_draw.channel_name_scroll );
	
	coolsand_UI_set_font( &MMI_medium_font );
	coolsand_UI_measure_string( str, &str_w, &str_h );
	
	// background
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( x1, y1, x2, y2, GDI_COLOR_TRANSPARENT );
#else
	coolsand_UI_show_image( 0, 0, fmrdo_main_draw.bg_img );
#endif

	// text
	if( str_w > fmrdo_main_draw.channel_name.w - 4 )
	{
		// scrolling
		coolsand_UI_change_scrolling_text( &fmrdo_main_draw.channel_name_scroll, str );
		coolsand_UI_show_scrolling_text( &fmrdo_main_draw.channel_name_scroll );
	}
	else
	{
		// no scrolling
		coolsand_UI_push_text_clip();
		coolsand_UI_set_text_clip( x1, y1, x2, y2 );
		
		coolsand_UI_set_text_color( fmrdo_main_draw.channel_name_text_color );
		coolsand_UI_set_text_border_color( fmrdo_main_draw.channel_name_text_border_color );
		if( r2lMMIFlag )
			coolsand_UI_move_text_cursor( x1 + (fmrdo_main_draw.channel_name.w + str_w)/2, y1 + (fmrdo_main_draw.channel_name.h - str_h)/2 );
		else
			coolsand_UI_move_text_cursor( x1 + (fmrdo_main_draw.channel_name.w - str_w)/2, y1 + (fmrdo_main_draw.channel_name.h - str_h)/2 );
		coolsand_UI_print_bordered_text( str );
		coolsand_UI_pop_text_clip();
	}
	
	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1, x2, y2 );
#endif
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_redraw_main_channel_tuner
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     U16 freq
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_fmrdo_redraw_main_channel_tuner( U16 freq )
{
	S32 x1 = fmrdo_main_draw.tuner_scale.x;
	S32 y1 = fmrdo_main_draw.tuner_scale.y;
	S32 x2 = fmrdo_main_draw.tuner_scale.x + fmrdo_main_draw.tuner_scale.w - 1;
	S32 y2 = fmrdo_main_draw.tuner_scale.y + fmrdo_main_draw.tuner_scale.h - 1;
	S32 delta;

	FMR_TRACE();
	FMR_TRACE_ARG("[########FMRadio] mmi_fmrdo_redraw_main_channel_tuner() freq= %d.",freq);
	//FMR_TRACE_ARG("fmrdo_main_draw.tuner_scale.w = %d.",fmrdo_main_draw.tuner_scale.w);
	//FMR_TRACE_ARG("fmrdo_main_draw.tuner_scale.h = %d.",fmrdo_main_draw.tuner_scale.h);	
	
#ifndef __GDI_MEMORY_PROFILE_2__
	BOOL outside = FALSE;
#endif

	if( fmrdo_main_draw.tuner_scale.type == 0 ) // 0: horizontal, 1: vertical
	{
		if( fmrdo_main_draw.tuner_scale.ind_offset < 0 )
			y1 += fmrdo_main_draw.tuner_scale.ind_offset;
		delta = fmrdo_main_draw.tuner_scale.y + fmrdo_main_draw.tuner_scale.ind_offset + fmrdo_main_draw.tuner_scale.ind_h - 1;
		if( delta > y2 )
		{
			y2 = delta;
#ifndef __GDI_MEMORY_PROFILE_2__
			outside = TRUE;
#endif
		}
	}
	else
	{
		if( fmrdo_main_draw.tuner_scale.ind_offset < 0 )
			x1 += fmrdo_main_draw.tuner_scale.ind_offset;
		
		delta = fmrdo_main_draw.tuner_scale.x + fmrdo_main_draw.tuner_scale.ind_offset + fmrdo_main_draw.tuner_scale.ind_w - 1;
		if( delta > x2 )
		{
			x2 = delta;
#ifndef __GDI_MEMORY_PROFILE_2__
			outside = TRUE;
#endif
		}
	}

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	coolsand_UI_push_clip();
	
	coolsand_UI_set_clip( x1, y1, x2, y2 );

#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( x1, y1, x2, y2, GDI_COLOR_TRANSPARENT );
#else
	if( outside )
#ifdef __MMI_MAINLCD_220X176__
		coolsand_UI_show_image( 0, 18, fmrdo_main_draw.bg_img ); // background
#else
		coolsand_UI_show_image( 0, 0, fmrdo_main_draw.bg_img ); // background
#endif

	// tuner scale background
	coolsand_UI_show_image( fmrdo_main_draw.tuner_scale.x, fmrdo_main_draw.tuner_scale.y, fmrdo_main_draw.tuner_scale.bg_img );
#endif

	// tuner scale indicator
	if( fmrdo_main_draw.tuner_scale.type == 0 ) // 0: horizontal, 1: vertical
	{
		S32 w = fmrdo_main_draw.tuner_scale.w - fmrdo_main_draw.tuner_scale.ind_start_gap - 
			fmrdo_main_draw.tuner_scale.ind_end_gap - fmrdo_main_draw.tuner_scale.ind_w;
		coolsand_UI_show_image( fmrdo_main_draw.tuner_scale.x + fmrdo_main_draw.tuner_scale.ind_start_gap + (freq - MIN_FM_FREQUENCY) * w / FM_BANDWIDTH, 
			fmrdo_main_draw.tuner_scale.y + fmrdo_main_draw.tuner_scale.ind_offset,
			fmrdo_main_draw.tuner_scale.ind_img );
	}
	else
	{
		S32 h = fmrdo_main_draw.tuner_scale.h - fmrdo_main_draw.tuner_scale.ind_start_gap - 
			fmrdo_main_draw.tuner_scale.ind_end_gap - fmrdo_main_draw.tuner_scale.ind_h;
		coolsand_UI_show_image( fmrdo_main_draw.tuner_scale.x + fmrdo_main_draw.tuner_scale.ind_offset,
			fmrdo_main_draw.tuner_scale.y +	fmrdo_main_draw.tuner_scale.ind_start_gap + (freq - MIN_FM_FREQUENCY) * h / FM_BANDWIDTH,
			fmrdo_main_draw.tuner_scale.ind_img );
	}
	
	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1, x2, y2 );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_redraw_main_volume
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
void mmi_fmrdo_redraw_main_volume( void )
{
	S32 x1 = fmrdo_main_draw.volume.x;
	S32 y1 = fmrdo_main_draw.volume.y;
	S32 x2 = fmrdo_main_draw.volume.x + fmrdo_main_draw.volume.w - 1;
	S32 y2 = fmrdo_main_draw.volume.y + fmrdo_main_draw.volume.h - 1;
#if defined(TGT_GALLITE_G800)
#define GAP_LSPKER 14 
#endif   
#ifdef __MMI_MAINLCD_220X176__
	byte * volume_img;
#endif
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	coolsand_UI_push_clip();

#if defined(TGT_GALLITE_G800)
	coolsand_UI_set_clip( x1-GAP_LSPKER, y1, x2, y2 );
#else	
	coolsand_UI_set_clip( x1, y1, x2, y2 );
#endif
	// background
#ifdef __GDI_MEMORY_PROFILE_2__
#if defined(TGT_GALLITE_G800)
	gdi_draw_solid_rect( x1-GAP_LSPKER, y1, x2, y2, GDI_COLOR_TRANSPARENT );
#else
	gdi_draw_solid_rect( x1, y1, x2, y2, GDI_COLOR_TRANSPARENT );
#endif
#else
	coolsand_UI_show_image( 0, 0, fmrdo_main_draw.bg_img );
#endif
#ifndef __MMI_MAINLCD_220X176__
	// volume
	if( fmrdo_main_draw.volume.type == 0 ) // 0: horizontal, 1: vertical
	{
		S32 delta = ((U32)g_fmrdo.volume ) * (fmrdo_main_draw.volume.w - fmrdo_main_draw.volume.ind_start_gap - 
			fmrdo_main_draw.volume.ind_end_gap) / MAX_VOL_LEVEL;
#if defined(TGT_GALLITE_G800)
		coolsand_UI_set_clip( x1-GAP_LSPKER, y1, x1 + fmrdo_main_draw.volume.ind_start_gap + delta - 1, y2 );
#else			
		coolsand_UI_set_clip( x1, y1, x1 + fmrdo_main_draw.volume.ind_start_gap + delta - 1, y2 );
#endif
	}
	else
	{
		S32 delta = ((U32)g_fmrdo.volume ) * (fmrdo_main_draw.volume.h - fmrdo_main_draw.volume.ind_start_gap - 
			fmrdo_main_draw.volume.ind_end_gap) / MAX_VOL_LEVEL;
#if defined(TGT_GALLITE_G800)
		coolsand_UI_set_clip( x1-GAP_LSPKER, y2 - fmrdo_main_draw.volume.ind_start_gap - delta - 1, x1, y2 );
#else			
		coolsand_UI_set_clip( x1, y2 - fmrdo_main_draw.volume.ind_start_gap - delta - 1, x1, y2 );
#endif
	}
#endif	
#if defined(TGT_GALLITE_G800)
	draw_resizeable_arrow(x1 - GAP_LSPKER, y1+1, 0, coolsand_UI_color(255,255,255),12,12);
#endif	
	coolsand_UI_show_image( x1, y1, fmrdo_main_draw.volume.bg_img );

#ifdef __MMI_MAINLCD_220X176__
	volume_img = (UI_image_type) GetImage( g_fmrdo_skins[0].volume_image_id +g_fmrdo.volume+1 );
	coolsand_UI_show_image( x1, y1+20, volume_img );
	
	coolsand_UI_set_clip( x1+180, y1, x2+180, y2 );

	
	volume_img = (UI_image_type) GetImage( g_fmrdo_skins[0].volume_image_id +g_fmrdo.volume+1+8 );
	coolsand_UI_show_image( x1+180, y1+20, volume_img );
#endif
	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
#if defined(TGT_GALLITE_G800)
	coolsand_UI_BLT_double_buffer( x1-GAP_LSPKER, y1, x2, y2 );
#else	
	coolsand_UI_BLT_double_buffer( x1, y1, x2, y2 );
#endif	
#ifdef __MMI_MAINLCD_220X176__
	coolsand_UI_BLT_double_buffer( x1+180, y1, x2+180, y2 );
#endif
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_redraw_main_push_button
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     U8 button
*     BOOL is_down
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_fmrdo_redraw_main_push_button( U8 fmrdobutton, BOOL is_down )
{
	fmrdo_main_push_button_struct *push_button_p;
	S32 x1, y1, x2, y2;
	
	switch( fmrdobutton )
	{
		case FMRDO_BUTTON_STEP_UP:
			push_button_p = &fmrdo_main_draw.step_up_button;
			break;
		case FMRDO_BUTTON_STEP_DOWN:
			push_button_p = &fmrdo_main_draw.step_down_button;
			break;
		default:
			return;
	}
	x1 = push_button_p->x;
	y1 = push_button_p->y;
	x2 = push_button_p->x + push_button_p->w - 1;
	y2 = push_button_p->y + push_button_p->h - 1;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	coolsand_UI_push_clip();
	
	coolsand_UI_set_clip( x1, y1, x2, y2 );

	// button
	if( is_down )
		coolsand_UI_show_image( x1, y1, push_button_p->down_img );
	else
		coolsand_UI_show_image( x1, y1, push_button_p->up_img );

	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1, x2, y2 );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_redraw_main_toggle_button
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     U8 button
*     BOOL is_down
*     BOOL is_on
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_fmrdo_redraw_main_toggle_button( U8 fmrdobutton, BOOL is_down, BOOL is_on , BOOL is_shown)
{
	fmrdo_main_toggle_button_struct *toggle_button_p;
	S32 x1, y1, x2, y2;
	
	switch( fmrdobutton )
	{
		case FMRDO_BUTTON_POWER:
			toggle_button_p = &fmrdo_main_draw.power_button;
			break;
		case FMRDO_BUTTON_SEARCH:
			toggle_button_p = &fmrdo_main_draw.search_button;
			break;
#if defined(__MMI_FM_RADIO_RECORD__)			
		case FMRDO_BUTTON_REC:
			toggle_button_p = &fmrdo_main_draw.rec_button;
			break;
		case FMRDO_BUTTON_REC_PAUSE:
			toggle_button_p = &fmrdo_main_draw.rec_pause_button;
			break;
#endif			
		default:
			return;
	}
	x1 = toggle_button_p->x;
	y1 = toggle_button_p->y;
	x2 = toggle_button_p->x + toggle_button_p->w - 1;
	y2 = toggle_button_p->y + toggle_button_p->h - 1;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	coolsand_UI_push_clip();
	
	coolsand_UI_set_clip( x1, y1, x2, y2 );

	if(is_shown)
	{
		// button
		if( is_on )
		{
			if( is_down )
				coolsand_UI_show_image( x1, y1, toggle_button_p->on_down_img );
			else
				coolsand_UI_show_image( x1, y1, toggle_button_p->on_up_img );
		}
		else
		{
			if( is_down )
#ifdef __MMI_MAINLCD_220X176__
				coolsand_UI_show_image( x1, y1, toggle_button_p->off_up_img );
#else
				coolsand_UI_show_image( x1, y1, toggle_button_p->off_down_img );
#endif
			else
				coolsand_UI_show_image( x1, y1, toggle_button_p->off_up_img );
		}
	}
	else /*Clear to background*/
	{
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( x1, y1, x2, y2, GDI_COLOR_TRANSPARENT );
#else
	coolsand_UI_show_image( 0, 0, fmrdo_main_draw.bg_img );
#endif	
	}


	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1, x2, y2 );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_redraw_main_push_down_button
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     U8 button: 0 = volume_dec_button, 1 = volume_inc_button, 2 = setting_button
*     BOOL is_down
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_fmrdo_redraw_main_push_down_button( U8 fmrdobutton, BOOL is_down )
{
	fmrdo_main_push_down_button_struct *push_down_button_p;
	S32 x1, y1, x2, y2;
	
	switch( fmrdobutton )
	{
		case FMRDO_BUTTON_VOL_DEC:
			push_down_button_p = &fmrdo_main_draw.volume_dec_button;
			break;
		case FMRDO_BUTTON_VOL_INC:
			push_down_button_p = &fmrdo_main_draw.volume_inc_button;
			break;
		case FMRDO_BUTTON_SETTINGS:
			push_down_button_p = &fmrdo_main_draw.setting_button;
			break;
		default:
			return;
	}
	x1 = push_down_button_p->x;
	y1 = push_down_button_p->y;
	x2 = push_down_button_p->x + push_down_button_p->w - 1;
	y2 = push_down_button_p->y + push_down_button_p->h - 1;
	push_down_button_p->down = is_down;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	coolsand_UI_push_clip();
	coolsand_UI_set_clip( x1, y1, x2, y2 );
	coolsand_UI_show_image( x1, y1, (is_down ? push_down_button_p->down_img: push_down_button_p->up_img) );
	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1, x2, y2 );
#endif
}
 
#ifdef __MMI_TOUCH_SCREEN__
void mmi_fmrdo_pen_down_hdlr(mmi_pen_point_struct pos)
{
	S16 i;
	mmi_pen_point_struct point;
	mmi_pen_polygon_area_struct* area = NULL;
	const mmi_fmrdo_skin_struct *skin_p = &g_fmrdo_skins[g_fmrdo.skin_index];
	
	fmrdo_pen_event_on_object = FMRDO_PEN_NONE;

	for( i = FMRDO_PEN_LSK; i < FMRDO_PEN_COUNT; i++ )
	{
		switch( i )
		{
		case FMRDO_PEN_LSK:
			area = (mmi_pen_polygon_area_struct*)&LSK_area;
			point = pos;
			break;
		case FMRDO_PEN_RSK:	
			area = (mmi_pen_polygon_area_struct*)&RSK_area;
			point = pos;
			break;
		case FMRDO_PEN_MAIN_STEP_UP:
			area = (mmi_pen_polygon_area_struct*)&skin_p->step_up_button_area;
			point.x = pos.x;// - skin_p->step_up_button_x;
			point.y = pos.y ;//- skin_p->step_up_button_y;
			break;
		case FMRDO_PEN_MAIN_STEP_DOWN:
			area = (mmi_pen_polygon_area_struct*)&skin_p->step_down_button_area;
			point.x = pos.x;// - skin_p->step_down_button_x;
			point.y = pos.y;// - skin_p->step_down_button_y;
			break;
		case FMRDO_PEN_MAIN_POWER:
			area = (mmi_pen_polygon_area_struct*)&skin_p->power_button_area;
			point.x = pos.x ;
			point.y= pos.y;
			break;
		case FMRDO_PEN_MAIN_SEARCH:
			area = (mmi_pen_polygon_area_struct*)&skin_p->search_button_area;
			point.x = pos.x;// - skin_p->search_button_x;
			point.y = pos.y;// - skin_p->search_button_y;
			break;
		//add gdm 080515
#if defined(__MMI_FM_RADIO_RECORD__)
		case FMRDO_PEN_MAIN_RECORD:
			area = (mmi_pen_polygon_area_struct*)&skin_p->record_button_area;
			point.x = pos.x;// - skin_p->rec_button_x;
			point.y = pos.y;// - skin_p->rec_button_y;
			break;
#endif
		case FMRDO_PEN_MAIN_SETTING:
			area = (mmi_pen_polygon_area_struct*)&skin_p->setting_area;
			point.x = pos.x;// - skin_p->setting_x;
			point.y = pos.y;// - skin_p->setting_y;
			break;
		case FMRDO_PEN_MAIN_VOLUME_INC:
			area = (mmi_pen_polygon_area_struct*)&skin_p->volume_inc_area;
			point.x = pos.x;// - skin_p->volume_inc_x;
			point.y = pos.y ;//- skin_p->volume_inc_y;
			break;
		case FMRDO_PEN_MAIN_VOLUME_DEC:
			area = (mmi_pen_polygon_area_struct*)&skin_p->volume_dec_area;
			point.x = pos.x;// - skin_p->volume_dec_x;
			point.y = pos.y;// - skin_p->volume_dec_y;
			break;
		case FMRDO_PEN_MAIN_FREQ:
			area = (mmi_pen_polygon_area_struct*)&skin_p->frequency_area;
			point.x = pos.x;// - skin_p->frequency_x;
			point.y = pos.y;// - skin_p->frequency_y;
			break;
        default:
            break;
		}
        ASSERT( area != NULL);
		if( mmi_pen_check_inside_rect( area, point ) )
		{
			fmrdo_pen_event_on_object = i;
			break;
		}
	}

	switch( fmrdo_pen_event_on_object )
	{
	case FMRDO_PEN_LSK:
		mmi_fmrdo_redraw_main_LSK_down();
		ExecuteCurrKeyHandler(KEY_LSK, KEY_EVENT_DOWN);
		break;
	case FMRDO_PEN_RSK:
		mmi_fmrdo_redraw_main_RSK_down();
		ExecuteCurrKeyHandler(KEY_RSK, KEY_EVENT_DOWN);
		break;
	case FMRDO_PEN_MAIN_STEP_UP:
		if(g_fmrdo.state != FMRDO_STATE_RECORD && g_fmrdo.state != FMRDO_STATE_RECORD_PAUSE)
		{
			mmi_fmrdo_press_step_up_button();
		}
		break;
	case FMRDO_PEN_MAIN_STEP_DOWN:
		if(g_fmrdo.state != FMRDO_STATE_RECORD && g_fmrdo.state != FMRDO_STATE_RECORD_PAUSE)
		{
			mmi_fmrdo_press_step_down_button();
		}
		break;
	case FMRDO_PEN_MAIN_POWER:
		if(g_fmrdo.state != FMRDO_STATE_RECORD && g_fmrdo.state != FMRDO_STATE_RECORD_PAUSE)
		{
			mmi_fmrdo_press_power_button();
		}
		break;
	case FMRDO_PEN_MAIN_SEARCH:
		if(g_fmrdo.state != FMRDO_STATE_RECORD && g_fmrdo.state != FMRDO_STATE_RECORD_PAUSE)
		{
			mmi_fmrdo_press_search_button();
		}
		break;
#if defined(__MMI_FM_RADIO_RECORD__)
	case FMRDO_PEN_MAIN_RECORD:
		if(g_fmrdo.state != FMRDO_STATE_RECORD && g_fmrdo.state != FMRDO_STATE_RECORD_PAUSE)
		{
			mmi_fmrdo_press_power_button();
		}      
		break;
#endif        
	case FMRDO_PEN_MAIN_SETTING:
		if(g_fmrdo.state != FMRDO_STATE_RECORD && g_fmrdo.state != FMRDO_STATE_RECORD_PAUSE)
		{
			mmi_fmrdo_redraw_main_push_down_button( FMRDO_BUTTON_SETTINGS, TRUE );
		}
		break;
	case FMRDO_PEN_MAIN_VOLUME_INC:
		mmi_fmrdo_redraw_main_push_down_button( FMRDO_BUTTON_VOL_INC, TRUE );
		break;
	case FMRDO_PEN_MAIN_VOLUME_DEC:
		mmi_fmrdo_redraw_main_push_down_button( FMRDO_BUTTON_VOL_DEC, TRUE );
		break;
	case FMRDO_PEN_MAIN_FREQ:
		if(g_fmrdo.state != FMRDO_STATE_RECORD && g_fmrdo.state != FMRDO_STATE_RECORD_PAUSE)
		{
			mmi_fmrdo_entry_channel_list();
		}
		break;
	}
}

void mmi_fmrdo_pen_up_hdlr(mmi_pen_point_struct pos)
{
	switch (fmrdo_pen_event_on_object)
	{
		case FMRDO_PEN_MAIN_STEP_UP:
			if(g_fmrdo.state != FMRDO_STATE_RECORD && g_fmrdo.state != FMRDO_STATE_RECORD_PAUSE)
			{
				mmi_fmrdo_release_step_up_button();
			}
			break;
		case FMRDO_PEN_MAIN_STEP_DOWN:
			if(g_fmrdo.state != FMRDO_STATE_RECORD && g_fmrdo.state != FMRDO_STATE_RECORD_PAUSE)
			{
				mmi_fmrdo_release_step_down_button();
			}
			break;
		case FMRDO_PEN_MAIN_SEARCH:
			if(g_fmrdo.state != FMRDO_STATE_RECORD && g_fmrdo.state != FMRDO_STATE_RECORD_PAUSE)
			{
				mmi_fmrdo_release_search_button();
			}
			break;
		case FMRDO_PEN_MAIN_POWER:
			if(g_fmrdo.state != FMRDO_STATE_RECORD && g_fmrdo.state != FMRDO_STATE_RECORD_PAUSE)
			{
				mmi_fmrdo_release_power_button();
			}
			break;
		//add gdm 080515
#if defined(__MMI_FM_RADIO_RECORD__)
		case FMRDO_PEN_MAIN_RECORD:
			if(g_fmrdo.state == FMRDO_STATE_RECORD)
			{
				mmi_fmrdo_release_pause_button();
			}
			else if(g_fmrdo.state == FMRDO_STATE_RECORD_PAUSE)
			{
				mmi_fmrdo_release_resume_button();
			}
			else
			{
				mmi_fmrdo_release_record_button();
			}
			break;
#endif
		case FMRDO_PEN_MAIN_SETTING:
			if(g_fmrdo.state != FMRDO_STATE_RECORD && g_fmrdo.state != FMRDO_STATE_RECORD_PAUSE)
			{
				mmi_fmrdo_redraw_main_push_down_button( FMRDO_BUTTON_SETTINGS, FALSE );
				mmi_fmrdo_entry_settings();
			}
			break;
		case FMRDO_PEN_MAIN_VOLUME_DEC:
			mmi_fmrdo_redraw_main_push_down_button( FMRDO_BUTTON_VOL_DEC, FALSE );
			mmi_fmrdo_dec_volume();
			break;
		case FMRDO_PEN_MAIN_VOLUME_INC:
			mmi_fmrdo_redraw_main_push_down_button( FMRDO_BUTTON_VOL_INC, FALSE );
			mmi_fmrdo_inc_volume();
			break;
		case FMRDO_PEN_LSK:
			mmi_fmrdo_redraw_main_LSK_up();
			if ( mmi_pen_check_inside_rect( &LSK_area, pos ) )
				ExecuteCurrKeyHandler(KEY_LSK, KEY_EVENT_UP);
			break;
		case FMRDO_PEN_RSK:
			mmi_fmrdo_redraw_main_RSK_up();
			if ( mmi_pen_check_inside_rect( &RSK_area, pos ) )
				ExecuteCurrKeyHandler(KEY_RSK, KEY_EVENT_UP);
			break;
	}
}

void mmi_fmrdo_pen_move_hdlr(mmi_pen_point_struct pos)
{
	switch (fmrdo_pen_event_on_object)
	{
#if (0) // Not handle these four buttons
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
		case FMRDO_PEN_LSK:
			if ( mmi_pen_check_inside_rect( &LSK_area, pos ) )
			{
				mmi_fmrdo_redraw_main_LSK_down();
			}
			else
			{
				mmi_fmrdo_redraw_main_LSK_up();
			}
			break;
		case FMRDO_PEN_RSK:
			if ( mmi_pen_check_inside_rect( &LSK_area, pos ) )
			{
				mmi_fmrdo_redraw_main_RSK_down();
			}
			else
			{
				mmi_fmrdo_redraw_main_RSK_up();
			}
			break;
	}
}
#endif
 


#ifdef __MMI_FM_RADIO_ANIMATION__
/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_animation_start
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_fmrdo_animation_start(void)
{
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_layer_push_and_set_active( fmrdo_skin_layer );
#endif

	// animation
#ifdef __MMI_FM_RADIO_ANIMATION__
	fmrdo_main_draw.animation.img_handle = GDI_ERROR_HANDLE;
	gdi_image_draw_animation( fmrdo_main_draw.animation.x,fmrdo_main_draw.animation.y, fmrdo_main_draw.animation.animation_img,&fmrdo_main_draw.animation.img_handle);
#endif

#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_layer_pop_and_restore_active();
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_animation_stop
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_fmrdo_animation_stop(void)
{
	// stop animation
	if ( fmrdo_main_draw.animation.img_handle != GDI_ERROR_HANDLE)
	{
		gdi_image_stop_animation( fmrdo_main_draw.animation.img_handle);
	}
}
#endif

#if defined(__MMI_FM_RADIO_RECORD__)
/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_change_main_SK_record
* DESCRIPTION
*   Redraw main screen LSK and RSK for record
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_change_main_SK_record(void)
{
	fmrdo_main_draw.LSK_text = (UI_string_type) GetString( STR_ID_FMRDO_PAUSE );
	fmrdo_main_draw.RSK_text = (UI_string_type) GetString( STR_ID_FMRDO_STOP);

	mmi_fmrdo_redraw_main_LSK_up();
	mmi_fmrdo_redraw_main_RSK_up();
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_change_main_SK_pause
* DESCRIPTION
*   Redraw main screen LSK and RSK for pause
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_change_main_SK_pause(void)
{
	fmrdo_main_draw.LSK_text = (UI_string_type) GetString( STR_ID_FMRDO_RESUME);
	fmrdo_main_draw.RSK_text = (UI_string_type) GetString( STR_ID_FMRDO_STOP);

	mmi_fmrdo_redraw_main_LSK_up();
	mmi_fmrdo_redraw_main_RSK_up();
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_restore_main_SK
* DESCRIPTION
*   Redraw main screen LSK and RSK for restore back to play screen
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_restore_main_SK(void)
{
	fmrdo_main_draw.LSK_text = (UI_string_type) GetString( STR_GLOBAL_OPTIONS );
	fmrdo_main_draw.RSK_text = (UI_string_type) GetString( STR_GLOBAL_BACK);

	mmi_fmrdo_redraw_main_LSK_up();
	mmi_fmrdo_redraw_main_RSK_up();
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_redraw_main_time_duration
* DESCRIPTION
*   Draw time duration for FM radio record
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_redraw_main_time_duration(BOOL is_shown)
{
	S32 x1, y1, x2, y2;
	U32 rec_time = 4232;
	U16 hour, min,sec;

	rec_time = (U32)(g_fmrdo.rec.duration*0.00390625 + 0.5);

	hour = rec_time / 3600;
	min = (rec_time - hour*3600) / 60;
	sec = (rec_time - hour*3600 - min*60);
	
	x1 = fmrdo_main_draw.timer_hr_0_offset_x;
	y1 = fmrdo_main_draw.timer_offset_y;
	x2 = fmrdo_main_draw.timer_offset_x2;
	y2 = fmrdo_main_draw.timer_offset_y2;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	coolsand_UI_push_clip();
	coolsand_UI_set_clip( x1, y1, x2, y2 );
#ifdef __MMI_MAINLCD_220X176__
	coolsand_UI_show_image( 0, 18, fmrdo_main_draw.bg_img );
#else
	coolsand_UI_show_image( 0, 0, fmrdo_main_draw.bg_img );
#endif

#if defined(__MMI_MAINLCD_220X176__)

	gdi_draw_solid_rect(x1, y1, x2, y2, 0xffffffff);
#endif

	if(is_shown)
	{
		/*hh:mm:ss*/
		coolsand_UI_show_image( fmrdo_main_draw.timer_hr_0_offset_x, y1,
								(UI_image_type) GetImage((U16)(fmrdo_main_draw.rec_base_num_id + (hour / 10))) );
		coolsand_UI_show_image( fmrdo_main_draw.timer_hr_1_offset_x, y1,
								(UI_image_type) GetImage((U16)(fmrdo_main_draw.rec_base_num_id + (hour % 10))) );
		coolsand_UI_show_image( fmrdo_main_draw.timer_col_0_offset_x, y1,
								(UI_image_type) GetImage(fmrdo_main_draw.rec_num_col_id) );
		coolsand_UI_show_image( fmrdo_main_draw.timer_min_0_offset_x, y1,
								(UI_image_type) GetImage((U16)(fmrdo_main_draw.rec_base_num_id + (min / 10))) );
		coolsand_UI_show_image( fmrdo_main_draw.timer_min_1_offset_x, y1,
								(UI_image_type) GetImage((U16)(fmrdo_main_draw.rec_base_num_id + (min % 10))) );
		coolsand_UI_show_image( fmrdo_main_draw.timer_col_1_offset_x, y1,
								(UI_image_type) GetImage(fmrdo_main_draw.rec_num_col_id) );
		coolsand_UI_show_image( fmrdo_main_draw.timer_sec_0_offset_x, y1,
								(UI_image_type) GetImage((U16)(fmrdo_main_draw.rec_base_num_id + (sec / 10))) );
		coolsand_UI_show_image( fmrdo_main_draw.timer_sec_1_offset_x, y1,
								(UI_image_type) GetImage((U16)(fmrdo_main_draw.rec_base_num_id + (sec % 10))) );
	}
	else /*Clear to background*/
	{
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( x1, y1, x2, y2, GDI_COLOR_TRANSPARENT );
#else
	coolsand_UI_show_image( 0, 0, fmrdo_main_draw.bg_img );
#endif	
	}


	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1, x2, y2 );
#endif

}
#endif

#endif // __MMI_FM_RADIO__


