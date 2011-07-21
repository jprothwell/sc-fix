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
 *  CustThemesRes.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************
	FILENAME	: CustThemesRes.h

   	PURPOSE		: To define Common Structures used by the UI Elements and Cutomization Tool 
				  for Customizing Themes

 

 

	DATE		: 01thSept 2003.
*************************************************************/

#ifndef _PIXCOM_CUSTTHEMESRES_H
#define _PIXCOM_CUSTTHEMESRES_H

//jl #include "mmi_data_types.h"
#include "gui_data_types.h"

#define MAX_COMPONENTS    1000
#define MAX_THEME_COMP_NAME_SIZE 55

typedef enum
{
	THEME_COMP_FA,
	THEME_COMP_COLOR,
	THEME_COMP_GRADIENT_COLOR
}THEME_COMP_TYPE;

typedef struct _gradient_color
{	color_t *c;									/*	List of colors in the gradient	*/
	byte  *p;									/*	Percentage of each color_t		*/
	byte  n;									/*	Number of colors in the list	*/
} gradient_color;


typedef struct _UI_filled_area
{	dword						flags;
	UI_image_type				b;
	gradient_color*				gc;
	color_t						c;	
	color_t						ac;						/*	alternate color_t used for hatch fills	*/
	color_t						border_color;
	color_t						shadow_color;
	UI_transparent_color_type	transparent_color;
} UI_filled_area;


typedef struct _MMI_theme
{	
	UI_filled_area*		LSK_up_filler;
	UI_filled_area*		LSK_down_filler;
	UI_filled_area*		LSK_disabled_filler;
	UI_filled_area*		RSK_up_filler;
	UI_filled_area*		RSK_down_filler;//5
	UI_filled_area*		RSK_disabled_filler;
	UI_filled_area*		multitap_filler;
	UI_filled_area*		inputbox_filler;
	UI_filled_area*		scrollbar_background_filler;
	UI_filled_area*		scrollbar_indicator_filler;//10
	UI_filled_area*		title_filler;
	UI_filled_area*		list_background_filler;
	UI_filled_area*		list_normal_item_filler;
	UI_filled_area*		list_selected_item_filler;
	UI_filled_area*		list_disabled_item_filler;//15
	UI_filled_area*		matrix_background_filler;
	UI_filled_area*		matrix_normal_item_filler;
	UI_filled_area*		matrix_selected_item_filler;
	UI_filled_area*		matrix_disabled_item_filler;
	UI_filled_area*		shortcut_indicator_background_filler;//20
	UI_filled_area*		formatted_inputbox_normal_filler;
	UI_filled_area*		formatted_inputbox_selected_filler;
	UI_filled_area*		main_menu_background_filler;
	UI_filled_area*		general_background_filler;
	UI_filled_area*		popup_description_background_filler;//25
	UI_filled_area*		inline_list_background_filler;
	UI_filled_area*		inline_list_normal_item_filler;
	UI_filled_area*		inline_list_selected_item_filler;
	UI_filled_area*		inline_list_disabled_item_filler;
	UI_filled_area*		inline_edit_focussed_filler;//30
	UI_filled_area*		inline_edit_unfocussed_filler;
	UI_filled_area*		inline_edit_noboundary_filler;
	UI_filled_area*		PIN_inputbox_background_filler;
	UI_filled_area*		PIN_screen_background_filler;
	UI_filled_area*		network_screen_background_filler;//35
	UI_filled_area*		aphorism_screen_background_filler;
	UI_filled_area*		CM_screen_background_filler;
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
	UI_filled_area*		progress_screen_background_filler;
#endif
	UI_filled_area*		dialer_inputbox_background_filler;
	UI_filled_area*		virtual_keyboard_key_down_filler;//40

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
	UI_filled_area*		alarm_background_filler;
#endif

#ifdef UI_SMALL_LIST_SCREEN
/* Menu background filler of small list screen */
    UI_filled_area  *small_list_menu_normal_filler;
    /* Border filler of small list screen */
    UI_filled_area  *small_list_screen_border_filler;
#endif

	color_t*				UI_black_color;
	color_t*				UI_white_color;//45
    color_t           *lite_disp_scr_bg_color;
    
	color_t*				LSK_up_text_color;
	color_t*				LSK_down_text_color;
	color_t*				LSK_disabled_text_color;
	color_t*				RSK_up_text_color;//50
	color_t*				RSK_down_text_color;
	color_t*				RSK_disabled_text_color;
	color_t*				multitap_selector_color;
	color_t*				multitap_normal_text_color;
	color_t*				multitap_selected_text_color;//55
	color_t*				inputbox_normal_text_color;
	color_t*				inputbox_selected_text_color;
	color_t*				inputbox_selector_color;
	color_t*				inputbox_cursor_color;
	color_t*				title_text_color;//60
	color_t*				title_text_border_color;
	color_t*				title_shortcut_text_color;
	color_t*				list_normal_text_color;
	color_t*				list_selected_text_color;
	color_t*				list_disabled_text_color;//65
	color_t*				matrix_normal_text_color;
	color_t*				matrix_selected_text_color;
	color_t*				matrix_disabled_text_color;
	color_t*				menubar_index_background_color;
	color_t*				menubar_item_background_color;//70
	color_t*				menubar_index_text_color;
	color_t*				menubar_item_text_color;
	color_t*				shortcut_indicator_text_color;
	color_t*				shortcut_indicator_cursor_color;
	color_t*				drop_down_normal_text_color;//75
	color_t*				drop_down_disabled_text_color;
	color_t*				drop_down_focussed_text_color;
	color_t*				formatted_inputbox_selector_color;
	color_t*				formatted_inputbox_normal_text_color;
	color_t*				formatted_inputbox_disabled_text_color;//80
	color_t*				formatted_inputbox_selected_text_color;
	color_t*				formatted_inputbox_cursor_color;
	color_t*				list_main_menu_normal_text_color;
	color_t*				list_main_menu_selected_text_color;
	color_t*				list_main_menu_disabled_text_color;//85
	color_t*				matrix_main_menu_normal_text_color;
	color_t*				matrix_main_menu_selected_text_color;
	color_t*				matrix_main_menu_disabled_text_color;
	color_t*				circular_main_menu_normal_text_color;
	color_t*				circular_main_menu_selected_text_color;//90
	color_t*				circular_main_menu_disabled_text_color;
	color_t*				popup_screen_background_color;
	color_t*				popup_screen_border_color;
	color_t*				popup_description_text_color;

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
	color_t*				CM_screen_text_color;//95
#endif

	color_t*				datetime_bar_background_color;
	color_t*				datetime_bar_duration_background_color;
	color_t*				datetime_bar_date_text_color;
	color_t*				datetime_bar_time_text_color;
	color_t*				datetime_bar_duration_text_color;//100
	color_t*				datetime_bar_AOC_text_color;
	color_t*				button_bar_color;
	color_t*				information_bar_color;
	color_t*				input_method_text_color;
	color_t*				remaining_length_text_color;//105
	color_t*				statusbar0_background_color;

	color_t*				inline_edit_focussed_text_color;
	color_t*				inline_edit_unfocussed_text_color;
	color_t*				inline_edit_selected_text_color;
	color_t*				inline_edit_selector_color;//110
	color_t*				inline_edit_cursor_color;

	color_t*				inline_list_normal_text_color;
	color_t*				inline_list_selected_text_color;
	color_t*				inline_list_disabled_text_color;

	color_t*				PIN_inputbox_normal_text_color;//115
	color_t*				PIN_inputbox_selected_text_color;
	color_t*				PIN_inputbox_selector_color;
	color_t*				PIN_inputbox_cursor_color;

	color_t*				dialer_inputbox_normal_text_color;
	color_t*				dialer_inputbox_selected_text_color;//120
	color_t*				dialer_inputbox_selector_color;
	color_t*				dialer_inputbox_cursor_color;
	
	color_t*				virtual_keyboard_key_down_text_color;
	color_t*				virtual_keyboard_key_up_text_color;
	color_t*				virtual_keyboard_dead_key_down_text_color;//125
	color_t*				virtual_keyboard_dead_key_up_text_color;	
	color_t*				virtual_keyboard_disp_area_text_color;

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
	color_t*				calculator_color;
	color_t*				idle_scr_date_color;
	color_t*				idle_scr_time_color;//130
	color_t*				alarm_text_color;
#endif

	UI_font_type		button_font;
	UI_font_type		multitap_font;
	UI_font_type		inputbox_font;
	UI_font_type		window_title_font;//135
	UI_font_type		list_menu_font;
	UI_font_type		matrix_menu_font;
	UI_font_type		menu_bar_index_font;
	UI_font_type		menu_bar_item_font;
	UI_font_type		shortcut_indicator_font;//140
	UI_font_type		title_text_font;
	UI_font_type		drop_down_control_font;
	UI_font_type		list_main_menu_font;
	UI_font_type		matrix_main_menu_font;
	UI_font_type		virtual_keyboard_font;//145

	S32					scrollbar_size;
} MMI_theme;


typedef struct __ThemesCompInfo
{
	void* themeComponent; 
	U8	type;
	U8	themeCompName[MAX_THEME_COMP_NAME_SIZE];

}THEMES_COMPONENT_INFO;
typedef struct 
{
	U16	image_id;
	U32 image_offset;
} theme_image_detail_struct;

typedef struct 
{
	MMI_theme*					theme;
	theme_image_detail_struct* theme_image_details_list;
	U8**						system_image_list;
} theme_details_struct;

#endif //_PIXCOM_CUSTTHEMESRES_H

