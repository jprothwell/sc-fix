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
 *   FMScheduleRecScreen.c
 *
 * Project:
 * --------
 *   Maui_sw
 *
 * Description:
 * ------------
 *  .............
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
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _MMI_FMSRSCREEN_C
#define _MMI_FMSRSCREEN_C

#include "mmi_features.h" 

#ifdef __MMI_FM_RADIO_SCHEDULE_REC__

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "commonscreens.h"
#include "settingprofile.h"
#include "wgui_categories_inputs.h"
#include "wgui_status_icons.h"
#ifdef __MMI_TOUCH_SCREEN__
#include "touchscreengprot.h"
#endif
//... Add More MMI header

#include "fmschedulerecdef.h"
#include "fmschedulerectype.h"
#include "fmschedulerecprot.h"
#include "fmschedulerecscreen.h"

#undef __NEWSIMULATOR 

/*  Include: PS header file */
//... Add More PS header


/* 
** Typedef 
*/

#ifdef __MMI_TOUCH_SCREEN__
typedef enum
{
	FMSR_PEN_NONE,
	FMSR_PEN_LSK,
	FMSR_PEN_RSK,
	FMSR_PEN_COUNT
} FMSR_PEN_STATE;

typedef enum
{
	FMSR_PEN_SK_UP,
	FMSR_PEN_SK_DOWN
} FMSR_SOFTKEY_STATE;
#endif 

/* 
** Define
*/
#define FMSR_LSK_X_OFFSET	7
#define FMSR_LSK_Y_OFFSET	2
#define FMSR_RSK_X_OFFSET	6
#define FMSR_RSK_Y_OFFSET	3

/* 
** Local Variable
*/

#ifdef __GDI_MEMORY_PROFILE_2__
GDI_HANDLE	fmsr_osd_layer;	/*Draw on this layer*/
GDI_HANDLE	fmsr_skin_layer;	/*Draw background layer*/
BOOL is_fmsr_skin_layer_drawn = FALSE;
#endif

fmsr_rec_screen_struct g_fmsr_screen;
extern const fmsr_skin_struct g_fmsr_skins[]; // defined in "resource_fmschedulerec_skins.c"

#ifdef __MMI_TOUCH_SCREEN__
static S32 fmsr_pen_event_on_object = FMSR_PEN_NONE;
static mmi_pen_point_struct LSK_points[4];
static mmi_pen_point_struct RSK_points[4];
static mmi_pen_polygon_area_struct LSK_area;
static mmi_pen_polygon_area_struct RSK_area;
#endif /*__MMI_TOUCH_SCREEN__*/

/* 
** Local Function
*/

/* 
** Global Variable
*/
extern BOOL r2lMMIFlag; // definedi n gui_inputs.c

/* 
** Global Function
*/
extern void set_on_idlescreen(S32 temo_on_idlescreen);

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_init_screen
* DESCRIPTION
*   Init the screen coordinate, strings, ... for FM record screen
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_init_screen(void)
{
	const fmsr_skin_struct *skin_p = &g_fmsr_skins[0]; /*Currently only one skin*/

	g_fmsr_screen.bg_img = (UI_image_type)GetImage(skin_p->bg_image_id);

	g_fmsr_screen.SK_text_color = coolsand_UI_color( skin_p->SK_text_color_r, skin_p->SK_text_color_g, skin_p->SK_text_color_b );
	g_fmsr_screen.SK_text_border_color =coolsand_UI_color( skin_p->SK_text_border_color_r, skin_p->SK_text_border_color_g, skin_p->SK_text_border_color_b );

	g_fmsr_screen.status.y = skin_p->status_text_y;
	g_fmsr_screen.status_text_color = coolsand_UI_color( skin_p->status_text_color_r, skin_p->status_text_color_g, skin_p->status_text_color_b );

	g_fmsr_screen.freq.y = skin_p->freq_text_y;
	g_fmsr_screen.freq_text_color = coolsand_UI_color( skin_p->freq_text_color_r, skin_p->freq_text_color_g, skin_p->freq_text_color_b );

	g_fmsr_screen.duration.y = skin_p->duration_text_y;		
	g_fmsr_screen.duration_text_color = coolsand_UI_color( skin_p->duration_text_color_r, skin_p->duration_text_color_g, skin_p->duration_text_color_b);
	g_fmsr_screen.date.y = skin_p->date_text_y;	
	g_fmsr_screen.date_text_color = coolsand_UI_color( skin_p->date_text_color_r, skin_p->date_text_color_g, skin_p->date_text_color_b );

	g_fmsr_screen.time.y = skin_p->time_text_y;;	
	g_fmsr_screen.time_text_color = coolsand_UI_color( skin_p->time_text_color_r, skin_p->time_text_color_g, skin_p->time_text_color_b );
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_show_main
* DESCRIPTION
*   Display the main FM record screen
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_show_main(U16 title,U16 title_icon,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,U8* history_buffer)
{
	//UI_UNUSED_PARAMETER(history_buffer);

#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_layer_multi_layer_enable();

	/* get base layer as skin layer */
	gdi_layer_get_base_handle( &fmsr_osd_layer );
	gdi_layer_clear_background( GDI_COLOR_TRANSPARENT );
	gdi_layer_set_source_key( TRUE, GDI_COLOR_TRANSPARENT );
	gdi_layer_create( 0, 0, UI_device_width, UI_device_height, &fmsr_skin_layer );
	gdi_layer_set_blt_layer( fmsr_skin_layer, fmsr_osd_layer, 0, 0 );

	is_fmsr_skin_layer_drawn = FALSE;

#endif


	MMI_menu_shortcut_number=-1;
	SetupCategoryKeyHandlers();

	if(history_buffer == NULL)
	{
		g_fmsr_screen.LSK_text = get_string( left_softkey );
		g_fmsr_screen.RSK_text = get_string( right_softkey );
		g_fmsr_screen.status_text = get_string(STR_ID_FMSR_RECORDING);
		g_fmsr_cntx.rec.is_start_duration = TRUE;
	}


	// associate functions
	ExitCategoryFunction = mmi_fmsr_hide_main;
	RedrawCategoryFunction = mmi_fmsr_redraw_main_all;
	GetCategoryHistory = dummy_get_history;
	GetCategoryHistorySize = dummy_get_history_size;
	
	// draw screen
	set_on_idlescreen(1);
	mmi_fmsr_redraw_main_all();
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_hide_main
* DESCRIPTION
*   Hide main FM record screen
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_hide_main( void )
{
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_layer_flatten_to_base( fmsr_skin_layer, fmsr_osd_layer, 0, 0 );
	gdi_layer_free( fmsr_skin_layer );
	gdi_layer_multi_layer_disable();

	/* restore blt layer as base layer only */
	gdi_layer_set_blt_layer( fmsr_osd_layer, 0, 0, 0 );
#endif

#ifdef __MMI_TOUCH_SCREEN__
	fmsr_pen_event_on_object = FMSR_PEN_NONE;
#endif

	// associate functions
	ExitCategoryFunction = MMI_dummy_function;
	RedrawCategoryFunction = MMI_dummy_function;
	GetCategoryHistory = dummy_get_history;
	GetCategoryHistorySize = dummy_get_history_size;

	fmsr_close_main_LCD_time_display();
	set_on_idlescreen(0);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_hide_status_icons
* DESCRIPTION
*  clear status bar background as the FMSR's background image
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_hide_status_icons(void)
{
	coolsand_UI_push_clip();

	coolsand_UI_set_clip( 0, 0, UI_device_width - 1, MMI_status_bar_height - 1 );
	coolsand_UI_show_image( 0, 0, (UI_image_type) GetImage(IMG_ID_FMSR_BACKGROUND) );

	coolsand_UI_pop_clip();
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_redraw_main_all
* DESCRIPTION
*   Redraw function for FM radio main screen
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_redraw_main_all( void )
{

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif	

	coolsand_UI_push_clip();
	coolsand_UI_push_text_clip();
	
	coolsand_UI_set_clip( 0, 0, UI_device_width - 1, UI_device_height - 1 );

#ifdef __GDI_MEMORY_PROFILE_2__
	if( !is_fmsr_skin_layer_drawn )
	{
		gdi_layer_push_and_set_active( fmsr_skin_layer );
#endif
	// main background
	coolsand_UI_show_image( 0, 0, (UI_image_type) GetImage(IMG_ID_FMSR_BACKGROUND) );
#ifdef __GDI_MEMORY_PROFILE_2__
		is_fmsr_skin_layer_drawn = TRUE;
		gdi_layer_pop_and_restore_active();
	}
#endif

	coolsand_UI_pop_text_clip();
	coolsand_UI_pop_clip();

	register_hide_status_icon_bar(0,mmi_fmsr_hide_status_icons);
	show_status_icon_bar(0);
	show_status_icons();

	mmi_fmsr_redraw_state();
	mmi_fmsr_redraw_frequency();
	mmi_fmsr_redraw_duration();
	mmi_fmsr_redraw_time();/*Draw from bottom*/	
	mmi_fmsr_redraw_date();

	mmi_fmsr_redraw_LSK_up();
	mmi_fmsr_redraw_RSK_up();


#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( 0, 0, UI_device_width - 1, UI_device_height - 1 );
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_redraw_state
* DESCRIPTION
*   Redraw state text
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_redraw_state( void)
{
	S32 x,y;
	S32 y1,y2;
	S32 str_w, str_h;

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	coolsand_UI_push_clip();

	coolsand_UI_set_font( &MMI_medium_font );
	coolsand_UI_measure_string( g_fmsr_screen.status_text, &str_w, &str_h );

	y1 = g_fmsr_screen.status.y;
	y2 = y1 + str_h;

	coolsand_UI_set_clip( 0, y1, UI_device_width - 1, y2 );

#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( 0, y1, UI_device_width - 1, y2, GDI_COLOR_TRANSPARENT );
#else
	coolsand_UI_show_image( 0, 0, g_fmsr_screen.bg_img );
#endif

	coolsand_UI_push_text_clip();
	coolsand_UI_set_text_clip( 0, y1, UI_device_width - 1, y2 );
	
//wanjian 2006-07-19
#if 0
	coolsand_UI_set_text_color(coolsand_UI_color(255,254,255));
#else
	coolsand_UI_set_text_color( g_fmsr_screen.status_text_color );
#endif
//wanjian 

	x = UI_device_width/2 - str_w/2;
	y = y1;
	
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x + str_w, y);
	else
		coolsand_UI_move_text_cursor( x,y );
			
	coolsand_UI_print_text( g_fmsr_screen.status_text );
	
	coolsand_UI_pop_text_clip();
	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( 0, y1, UI_device_width - 1, y2 );
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_redraw_frequency
* DESCRIPTION
*   Redraw frequency text
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_redraw_frequency( void )
{
	S32 x,y;
	S32 y1,y2;
	S32 str_w, str_h;
	UI_character_type str[20];
	UINT8 tmp[32];

	//kal_wsprintf( (UI_string_type)str, "FM %d.%d", g_fmsr_cntx.rec.freqency/ 10, g_fmsr_cntx.rec.freqency  % 10 );
	sprintf( tmp, "FM %d.%d", g_fmsr_cntx.rec.freqency/ 10, g_fmsr_cntx.rec.freqency  % 10 );
	AnsiiToUnicodeString((S8*)str,(S8*)tmp);

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	coolsand_UI_push_clip();

	coolsand_UI_set_font( &MMI_medium_font );
	coolsand_UI_measure_string( str, &str_w, &str_h );

	y1 = g_fmsr_screen.freq.y;
	y2 = y1 + str_h;

	coolsand_UI_set_clip( 0, y1, UI_device_width - 1, y2 );

#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( 0, y1, UI_device_width - 1, y2, GDI_COLOR_TRANSPARENT );
#else
	coolsand_UI_show_image( 0, 0, g_fmsr_screen.bg_img);
#endif

	coolsand_UI_push_text_clip();
	coolsand_UI_set_text_clip( 0, y1, UI_device_width - 1, y2 );

//wanjian 2006-07-19
#if 0
	coolsand_UI_set_text_color(coolsand_UI_color(255,254,255));
#else
	coolsand_UI_set_text_color( g_fmsr_screen.freq_text_color);
#endif
//wanjain 


	x = UI_device_width/2 - str_w/2;
	y = y1;
	
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x + str_w, y);
	else
		coolsand_UI_move_text_cursor( x,y );
			
	coolsand_UI_print_text( str );
	
	coolsand_UI_pop_text_clip();
	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( 0, y1, UI_device_width - 1, y2 );
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_redraw_init_duration
* DESCRIPTION
*   Init duration counter
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_redraw_init_duration(UI_time* t, U16 duration)
{
	U16 buf1,buf2;

	t->DayIndex=0;
	buf1 = duration/60;
	t->nSec = duration -buf1*60;
	buf2 = buf1/60;
	t->nMin = buf1 - buf2*60;
	t->nHour = (U8)buf2;
}

extern void set_main_LCD_time_duration_hide_function(void (*f) (S32 x1, S32 y1, S32 x2, S32 y2));


static void FmRecHideDurationDisplay(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    //color_t c = *current_MMI_theme->datetime_bar_duration_background_color;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_push_clip();
    gui_set_clip(x1, y1, x2, y2);
    coolsand_UI_show_image( 0, 0, (UI_image_type) GetImage(IMG_ID_FMSR_BACKGROUND) );
    gui_pop_clip();
}


/*****************************************************************************
* FUNCTION
*  mmi_fmsr_redraw_duration
* DESCRIPTION
*   Redraw durtion counter
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_redraw_duration( void )
{
	UINT16 str[20] ; /*Use widest character to estimate font size*/
	S32 str_w, str_h;
	S32 x,y;
	UI_time t;
	U32 duration = mmi_fmsr_get_duration_ticks();
	AnsiiToUnicodeString((S8*)str,(S8*)"44:44:44");
	coolsand_UI_set_font( &MMI_large_font);
	coolsand_UI_measure_string( str, &str_w, &str_h );

	x = UI_device_width/2 - str_w/2;
	y = g_fmsr_screen.duration.y;

	mmi_fmsr_redraw_init_duration(&t, (U16)(duration*0.00390625 + 0.5));
	fmsr_set_time_duration(&t, duration);
      set_main_LCD_time_duration_hide_function(FmRecHideDurationDisplay);

	if(g_fmsr_cntx.rec.is_start_duration)
	{
		fmsr_set_time_display(1, x, y, str_w+1, str_h);
	}
	else
	{
		fmsr_set_time_display(0, x, y, str_w+1, str_h);
	}
	
	time_display_duration();
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_redraw_date
* DESCRIPTION
*   Redraw date text
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_redraw_date( void )
{
	S32 x,y;
	S32 y1,y2;
	S32 str_w, str_h;
	UI_character_type str[20];
	UINT8 tmp[32];
	
	//kal_wsprintf( (UI_string_type)str, "%4d/%2d/%2d", g_fmsr_cntx.rec.year,g_fmsr_cntx.rec.mon,g_fmsr_cntx.rec.day);
	sprintf( tmp, "%4d/%02d/%02d", g_fmsr_cntx.rec.year,g_fmsr_cntx.rec.mon,g_fmsr_cntx.rec.day);
	AnsiiToUnicodeString((S8*)str,(S8*)tmp);
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	coolsand_UI_push_clip();

	coolsand_UI_set_font( &MMI_medium_font );
	coolsand_UI_measure_string( str, &str_w, &str_h );

	y1 = g_fmsr_screen.date.y;
	y2 = y1 + str_h;

	coolsand_UI_set_clip( 0, y1, UI_device_width - 1, y2 );

#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( 0, y1, UI_device_width - 1, y2, GDI_COLOR_TRANSPARENT );
#else
	coolsand_UI_show_image( 0, 0, g_fmsr_screen.bg_img );
#endif

	coolsand_UI_push_text_clip();
	coolsand_UI_set_text_clip( 0, y1, UI_device_width - 1, y2 );

//wanjian 2006-07-19
#if 0
	coolsand_UI_set_text_color(coolsand_UI_color(255,254,255));
#else
	coolsand_UI_set_text_color( g_fmsr_screen.date_text_color);
#endif
//wanjian 

	x = UI_device_width/2 - str_w/2;
	y = y1;
	
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x + str_w, y);
	else
		coolsand_UI_move_text_cursor( x,y );
			
	coolsand_UI_print_text( str );
	
	coolsand_UI_pop_text_clip();
	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( 0, y1, UI_device_width - 1, y2 );
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_redraw_time
* DESCRIPTION
*   Redraw time text
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_redraw_time( void )
{
	S32 x,y;
	S32 y1,y2;
	S32 str_w, str_h;
	UI_character_type str[20];
	UINT8 tmp[32];
	
	//kal_wsprintf( (UI_string_type)str, "%2d:%2d - %2d:%2d", g_fmsr_cntx.rec.hour1,g_fmsr_cntx.rec.min1,g_fmsr_cntx.rec.hour2,g_fmsr_cntx.rec.min2);
	sprintf( tmp, "%02d:%02d - %02d:%02d", g_fmsr_cntx.rec.hour1,g_fmsr_cntx.rec.min1,g_fmsr_cntx.rec.hour2,g_fmsr_cntx.rec.min2);
	AnsiiToUnicodeString((S8*)str,(S8*)tmp);
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

	coolsand_UI_push_clip();

	coolsand_UI_set_font( &MMI_small_font );
	coolsand_UI_measure_string( str, &str_w, &str_h );

	y1 = g_fmsr_screen.time.y;
	y2 = y1 + str_h;

	coolsand_UI_set_clip( 0, y1, UI_device_width - 1, y2 );

#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( 0, y1, UI_device_width - 1, y2, GDI_COLOR_TRANSPARENT );
#else
	coolsand_UI_show_image( 0, 0, g_fmsr_screen.bg_img );
#endif

	coolsand_UI_push_text_clip();
	coolsand_UI_set_text_clip( 0, y1, UI_device_width - 1, y2 );

//wanjian 2006-07-19
#if 0
	coolsand_UI_set_text_color(coolsand_UI_color(255,254,255));
#else
	coolsand_UI_set_text_color( g_fmsr_screen.time_text_color);
#endif
//wanjian 
	
	x = UI_device_width/2 - str_w/2;
	y = y1;
	
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x + str_w, y);
	else
		coolsand_UI_move_text_cursor( x,y );
			
	coolsand_UI_print_text( str );
	
	coolsand_UI_pop_text_clip();
	coolsand_UI_pop_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( 0, y1, UI_device_width - 1, y2 );
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_redraw_LSK_up
* DESCRIPTION
*   Redraw LSK up icon
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_redraw_LSK_up( void )
{
	S32 str_w, str_h;
	S32 x1, y1, x2, y2;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif	
	coolsand_UI_set_font( &MMI_medium_font );
	coolsand_UI_set_text_color( g_fmsr_screen.SK_text_color );
	coolsand_UI_set_text_border_color( g_fmsr_screen.SK_text_border_color );
	
	// show LSK
	coolsand_UI_measure_string( g_fmsr_screen.LSK_text, &str_w, &str_h );
	x1 = FMSR_LSK_X_OFFSET - 1;
	y1 = UI_device_height - str_h - FMSR_LSK_Y_OFFSET - 1;
	x2 = str_w + FMSR_LSK_X_OFFSET - 1;
	y2 = UI_device_height - FMSR_LSK_Y_OFFSET - 1;
	coolsand_UI_push_clip();
	coolsand_UI_set_clip( x1, y1, UI_device_width/2, y2+1 );
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( x1, y1, UI_device_width/2, y2+1, GDI_COLOR_TRANSPARENT );
#else
	coolsand_UI_show_image( 0, 0, g_fmsr_screen.bg_img );
#endif
	coolsand_UI_pop_clip();
	coolsand_UI_push_text_clip();
	coolsand_UI_set_text_clip( x1, y1-1, x2, y2 );
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x2, y1 );
	else
		coolsand_UI_move_text_cursor( x1, y1 );
	coolsand_UI_print_bordered_text( g_fmsr_screen.LSK_text );
	coolsand_UI_pop_text_clip();

#ifdef __MMI_TOUCH_SCREEN__
	LSK_area.num = 4;
	LSK_area.points = (mmi_pen_point_struct*) LSK_points;
	LSK_points[0].x = LSK_points[3].x = x1;
	LSK_points[0].y = LSK_points[1].y = y1;
	LSK_points[1].x = LSK_points[2].x = x2+1;
	LSK_points[2].y = LSK_points[3].y = y2+1;
#endif

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1, y1-1, UI_device_width/2, y2+1 );
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_redraw_RSK_up
* DESCRIPTION
*   Redraw RSK up icon
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_redraw_RSK_up( void )
{
	S32 str_w, str_h;
	S32 x1, y1, x2, y2;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif	
	coolsand_UI_set_font( &MMI_medium_font );
	coolsand_UI_set_text_color( g_fmsr_screen.SK_text_color );
	coolsand_UI_set_text_border_color( g_fmsr_screen.SK_text_border_color );
	
	// show RSK
	coolsand_UI_measure_string( g_fmsr_screen.RSK_text, &str_w, &str_h );
	x1 = UI_device_width - str_w - FMSR_RSK_X_OFFSET - 1;
	y1 = UI_device_height - str_h - FMSR_RSK_Y_OFFSET - 1;
	x2 = UI_device_width - FMSR_RSK_X_OFFSET-1;
	y2 = UI_device_height - FMSR_RSK_Y_OFFSET-1;
	coolsand_UI_push_clip();
	coolsand_UI_set_clip( UI_device_width/2, y1-1, x2+1, y2+1 );
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect(UI_device_width/2, y1-1, x2+1, y2+1, GDI_COLOR_TRANSPARENT);
#else
	coolsand_UI_show_image( 0, 0, g_fmsr_screen.bg_img );
#endif
	coolsand_UI_pop_clip();
	coolsand_UI_push_text_clip();
	coolsand_UI_set_text_clip( x1, y1-1, x2, y2 );
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x2, y1 );
	else
		coolsand_UI_move_text_cursor( x1, y1 );
	coolsand_UI_print_bordered_text( g_fmsr_screen.RSK_text );
	coolsand_UI_pop_text_clip();

#ifdef __MMI_TOUCH_SCREEN__
	RSK_area.num = 4;
	RSK_area.points = (mmi_pen_point_struct*) RSK_points;
	RSK_points[0].x = RSK_points[3].x = x1;
	RSK_points[0].y = RSK_points[1].y = y1;
	RSK_points[1].x = RSK_points[2].x = x2+1;
	RSK_points[2].y = RSK_points[3].y = y2+1;
#endif

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( UI_device_width/2, y1-1, x2+1, y2+1 );
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_redraw_LSK_down
* DESCRIPTION
*   Redraw LSK down icon
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_redraw_LSK_down( void )
{
	S32 str_w, str_h;
	S32 x1, y1, x2, y2;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif
	coolsand_UI_set_font( &MMI_medium_font );
	coolsand_UI_set_text_color( g_fmsr_screen.SK_text_color );
	coolsand_UI_set_text_border_color( g_fmsr_screen.SK_text_border_color );
	
	// show LSK
	coolsand_UI_measure_string( g_fmsr_screen.LSK_text, &str_w, &str_h );
	x1 = FMSR_LSK_X_OFFSET;
	y1 = UI_device_height - str_h - FMSR_LSK_Y_OFFSET;
	x2 = str_w + FMSR_LSK_X_OFFSET;
	y2 = UI_device_height - FMSR_LSK_Y_OFFSET;
	coolsand_UI_push_clip();
	coolsand_UI_set_clip( x1-2, y1-2, UI_device_width/2, y2 );
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( x1-2, y1-2, UI_device_width/2, y2, GDI_COLOR_TRANSPARENT );
#else
	coolsand_UI_show_image( 0, 0, g_fmsr_screen.bg_img );
#endif
	coolsand_UI_pop_clip();
	coolsand_UI_push_text_clip();
	coolsand_UI_set_text_clip( x1, y1-1, x2, y2 );
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x2, y1 );
	else
		coolsand_UI_move_text_cursor( x1, y1 );
	coolsand_UI_print_bordered_text( g_fmsr_screen.LSK_text );
	coolsand_UI_pop_text_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( x1-2, y1-2, UI_device_width/2, y2 );
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_redraw_RSK_down
* DESCRIPTION
*   Redraw RSK down icon
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_redraw_RSK_down( void )
{
	S32 str_w, str_h;
	S32 x1, y1, x2, y2;
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif
	coolsand_UI_set_font( &MMI_medium_font );
	coolsand_UI_set_text_color( g_fmsr_screen.SK_text_color );
	coolsand_UI_set_text_border_color( g_fmsr_screen.SK_text_border_color );
	
	// show RSK
	coolsand_UI_measure_string( g_fmsr_screen.RSK_text, &str_w, &str_h );
	x1 = UI_device_width - str_w - FMSR_RSK_X_OFFSET;
	y1 = UI_device_height - str_h - FMSR_RSK_Y_OFFSET;
	x2 = UI_device_width - FMSR_RSK_X_OFFSET;
	y2 = UI_device_height - FMSR_RSK_Y_OFFSET;
	coolsand_UI_push_clip();
	coolsand_UI_set_clip( UI_device_width/2, y1-2, x2, y2 );
#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect( UI_device_width/2, y1-2, x2, y2, GDI_COLOR_TRANSPARENT );
#else
	coolsand_UI_show_image( 0, 0, g_fmsr_screen.bg_img );
#endif
	coolsand_UI_pop_clip();
	coolsand_UI_push_text_clip();
	coolsand_UI_set_text_clip( x1, y1-1, x2, y2 );
	if( r2lMMIFlag )
		coolsand_UI_move_text_cursor( x2, y1 );
	else
		coolsand_UI_move_text_cursor( x1, y1 );
	coolsand_UI_print_bordered_text( g_fmsr_screen.RSK_text );
	coolsand_UI_pop_text_clip();

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer( UI_device_width/2, y1-2, x2, y2 );
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_redraw_LSK
* DESCRIPTION
*   Change LSK string and redraw
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_redraw_LSK( U16 left_softkey,U16 left_softkey_icon )
{
	g_fmsr_screen.LSK_text = (UI_string_type) GetString( left_softkey );
	//mmi_fmsr_redraw_LSK_down( );
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_change_pause_screen
* DESCRIPTION
*   Change record screen to pause screen
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_change_pause_screen(void)
{
	g_fmsr_screen.status_text = get_string(STR_ID_FMSR_PAUSE);
	g_fmsr_cntx.rec.is_start_duration = FALSE;

	mmi_fmsr_redraw_state();
	mmi_fmsr_redraw_LSK( STR_ID_FMSR_RESUME, IMG_GLOBAL_OK );
	mmi_fmsr_redraw_duration();
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_restore_recording_screen
* DESCRIPTION
*   Restore from pause screen to record screen
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_restore_recording_screen(void)
{
	g_fmsr_screen.status_text = get_string(STR_ID_FMSR_RECORDING);
	g_fmsr_cntx.rec.is_start_duration = TRUE;

	mmi_fmsr_redraw_state();
	mmi_fmsr_redraw_LSK( STR_ID_FMSR_PAUSE, IMG_GLOBAL_OK );
	mmi_fmsr_redraw_duration();
}

#ifdef __MMI_TOUCH_SCREEN__
/*****************************************************************************
* FUNCTION
*  mmi_fmsr_pen_down_hdlr
* DESCRIPTION
*   
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_pen_down_hdlr(mmi_pen_point_struct pos)
{
	S16 i;
	mmi_pen_point_struct point = {0,0};
	mmi_pen_polygon_area_struct* area = NULL;
	
	fmsr_pen_event_on_object = FMSR_PEN_NONE;

	for( i = FMSR_PEN_LSK; i < FMSR_PEN_COUNT; i++ )
	{
		switch( i )
		{
			case FMSR_PEN_LSK:
				area = (mmi_pen_polygon_area_struct*)&LSK_area;
				point = pos;
				break;
			case FMSR_PEN_RSK:	
				area = (mmi_pen_polygon_area_struct*)&RSK_area;
				point = pos;
				break;
		}
		
		if( mmi_pen_check_inside_polygon( area, point ) )
		{
			fmsr_pen_event_on_object = i;
			break;
		}
	}

	switch( fmsr_pen_event_on_object )
	{
		case FMSR_PEN_LSK:
			ExecuteCurrKeyHandler(KEY_LSK, KEY_EVENT_DOWN);
			break;
		case FMSR_PEN_RSK:
			ExecuteCurrKeyHandler(KEY_RSK, KEY_EVENT_DOWN);
			break;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_pen_up_hdlr
* DESCRIPTION
*   
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_pen_up_hdlr(mmi_pen_point_struct pos)
{
	switch (fmsr_pen_event_on_object)
	{
		case FMSR_PEN_LSK:
			if ( mmi_pen_check_inside_polygon( &LSK_area, pos ) )
			{
				ExecuteCurrKeyHandler(KEY_LSK, KEY_EVENT_UP);
			}
			mmi_fmsr_redraw_LSK_up(); /* Redraw after execute handler */
			break;
			
		case FMSR_PEN_RSK:

			mmi_fmsr_redraw_RSK_up(); /* Redraw before execute handler */
			if ( mmi_pen_check_inside_polygon( &RSK_area, pos ) )
			{
				ExecuteCurrKeyHandler(KEY_RSK, KEY_EVENT_UP);
			}
			break;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_pen_move_hdlr
* DESCRIPTION
*   
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_pen_move_hdlr(mmi_pen_point_struct pos)
{
	switch (fmsr_pen_event_on_object)
	{
		case FMSR_PEN_LSK:
			if ( mmi_pen_check_inside_polygon( &LSK_area, pos ) )
			{
				mmi_fmsr_redraw_LSK_down();
			}
			else
			{
				mmi_fmsr_redraw_LSK_up();
			}
			break;
		case FMSR_PEN_RSK:
			if ( mmi_pen_check_inside_polygon( &LSK_area, pos ) )
			{
				mmi_fmsr_redraw_RSK_down();
			}
			else
			{
				mmi_fmsr_redraw_RSK_up();
			}
			break;
	}
}
#endif
 

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
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#ifdef __GDI_MEMORY_PROFILE_2__
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
/* under construction !*/
/* under construction !*/
#if(UI_DOUBLE_BUFFER_SUPPORT)
/* under construction !*/
#endif
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
#if(UI_DOUBLE_BUFFER_SUPPORT)
/* under construction !*/
#endif
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
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#ifdef __GDI_MEMORY_PROFILE_2__
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
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
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if(UI_DOUBLE_BUFFER_SUPPORT)
/* under construction !*/
#endif	
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#ifdef __GDI_MEMORY_PROFILE_2__
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
/* under construction !*/
/* under construction !*/
#ifdef __GDI_MEMORY_PROFILE_2__
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
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
/* under construction !*/
#if(UI_DOUBLE_BUFFER_SUPPORT)
/* under construction !*/
/* under construction !*/
#endif
/* under construction !*/
#endif

#endif /* #ifdef __MMI_FM_RADIO_SCHEDULE_REC__ */
#endif /* #ifndef _MMI_FMSRSCREEN_C */
