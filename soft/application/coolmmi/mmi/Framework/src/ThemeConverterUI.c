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
 *   ThemeConverterUI.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 * Contains function fort he filling of the MMI_theme structure
 *
 * Author:
 * -------
 * -------
 
 *   
 **************************************************************/
#define __NEWSIMULATOR

#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "fat_fs.h"
#include "settingprofile.h"
#include "gui_data_types.h"
#include "mmi_data_types.h"
#include "pixcomfontengine.h"
#include "custthemesres.h"
#include "themeconverter.h"
#include "thememanager.h"
 
#include "stdlib.h"
#include "mainmenudef.h"
#include "globaldefs.h"

#undef __NEWSIMULATOR 
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__


extern MMI_theme **MMI_themes;
extern XML_PARSER_STRUCT parser;

//... Add More MMI header


extern U16 gcheckThemeVersion;
U8 gaudiothmfileformat; //ritesh
/*************************************************************/


//Theme Versioning Structures


deviceinfo gdeviceInfo;
theme_download_info_struct gthemeInfo;


/**************************************************************/
/*************************************color objetcs********************************/
	
	color_t 				UI_black_color={0,0,0,100};
	color_t 				UI_white_color={255,255,255,100};
	color_t 				LSK_up_txt_col_DLT={0,0,0,100};
	color_t 				LSK_down_txt_col_DLT={255,255,255,100};
	color_t 				LSK_disabled_text_color={128,128,128,100};
	color_t 				RSK_up_txt_col_DLT={0,0,0,100};
	color_t 				RSK_down_txt_col_DLT={255,255,255,100};
	color_t 				RSK_disabled_text_color={128,128,128,100};
	color_t 				multitap_selector_color_DLT={51,88,171,100};
	color_t 				multitap_normal_text_color_DLT={0,0,0,100};
	color_t 				multitap_selected_text_color_DLT={255,255,255,100};
	color_t 				inputbox_normal_text_color_DLT={0,0,0,100};
	color_t 				inputbox_selected_text_color_DLT={255,255,255,100};
	color_t 				inputbox_selector_color_DLT={51,88,171,100};
	color_t 				inputbox_cursor_color_DLT={255,0,0,100};
	color_t 				title_text_color={255,255,255,100};
	color_t 				title_text_border_color={0,0,0,100};
	color_t 				title_shortcut_text_color={0,0,0,100};
	color_t 				list_normal_text_color={0,46,114,100};
	color_t 				list_selected_text_color={255,255,255,100};
	color_t 				list_disabled_text_color={0,0,0,100};
	color_t 				matrix_normal_text_color={0,0,0,100};
	color_t 				matrix_selected_text_color={255,255,255,100};
	color_t 				matrix_disabled_text_color={0,0,0,100};
	color_t 				menubar_index_background_color={50,90,140,100};
	color_t 				menubar_item_background_color={149,207,247,100};
	color_t 				menubar_index_text_color={255,255,255,100};
	color_t 				menubar_item_text_color={0,0,0,100};
	color_t 				shortcut_indicator_text_color={0,0,0,100};
	color_t 				shortcut_indicator_cursor_color={255,0,0,100};
	color_t 				drop_down_normal_text_color={0,0,0,100};
	color_t 				drop_down_disabled_text_color={64,64,64,100};
	color_t 				drop_down_focussed_text_color={0,0,0,100};
	color_t 				formatted_inputbox_selector_color={51,88,171,100};
	color_t 				formatted_inputbox_normal_text_color={0,0,0,100};
	color_t 				formatted_inputbox_disabled_text_color={128,128,128,100};
	color_t 				formatted_inputbox_selected_text_color={255,255,255,100};
	color_t 				formatted_inputbox_cursor_color={255,0,0,100};
	color_t 				list_main_menu_normal_text_color={0,46,114,100};
	color_t 				list_main_menu_selected_text_color={255,255,255,100};
	color_t 				list_main_menu_disabled_text_color={0,0,0,100};
	color_t 				matrix_main_menu_normal_text_color={0,0,0,100};
	color_t 				matrix_main_menu_selected_text_color={255,255,255,100};
	color_t 				matrix_main_menu_disabled_text_color={0,0,0,100};
	color_t 				circular_main_menu_normal_text_color={0,0,0,100};
	color_t 				circular_main_menu_selected_text_color={255,255,255,100};
	color_t 				circular_main_menu_disabled_text_color={0,0,0,100};
	color_t 				popup_screen_background_color={	246,255,171,100};
	color_t 				popup_screen_border_color={142,204,145,100};
	color_t 				popup_description_text_color={255,255,255,100};
	color_t 				datetime_bar_background_color={176,223,141,100};
	color_t 				datetime_bar_duration_background_color={255,255,255,100};
	color_t 				datetime_bar_date_text_color={0,0,0,100};
	color_t 				datetime_bar_time_text_color={0,0,0,100};
	color_t 				datetime_bar_duration_text_color={224,0,0,100};
	color_t 				datetime_bar_AOC_text_color={0,0,0,100};
	color_t 				button_bar_color={216,212,212,100};
	color_t 				information_bar_color={255,231,93,100};
	color_t 				input_method_text_color={255,0,0,100};
	color_t 				remaining_length_text_color={0,0,0,100};
	color_t 				statusbar0_background_color={255,255,255,100};
	color_t 				inline_edit_focussed_text_color={0,0,0,100};
	color_t 				inline_edit_unfocussed_text_color={0,0,0,100};
	color_t 				inline_edit_selected_text_color={	255,255,255,100	};
	color_t 				inline_edit_selector_color={	51,88,171,100	};
	color_t 				inline_edit_cursor_color={255,0,0,100};
	color_t 				inline_list_normal_text_color={0,0,0,100};
	color_t 				inline_list_selected_text_color={0,0,0,100};
	color_t 				inline_list_disabled_text_color={0,0,0,100};
	color_t 				PIN_inputbox_normal_text_color_DLT={0,0,0,100};
	color_t 				PIN_inputbox_selected_text_color_DLT={255,255,255,100};
	color_t 				PIN_inputbox_selector_color_DLT={51,88,171,100};
	color_t 				PIN_inputbox_cursor_color_DLT={255,0,0,100};
	color_t 				dialer_inputbox_normal_text_color_DLT={0,0,0,100};
	color_t 				dialer_inputbox_selected_text_color_DLT={255,255,255,100};
	color_t 				dialer_inputbox_selector_color_DLT={51,88,171,100};
	color_t 				dialer_inputbox_cursor_color_DLT={255,0,0,100};
	color_t 				virtual_keyboard_key_down_text_color={255,255,255,100};
	color_t 				virtual_keyboard_key_up_text_color={0,0,0,100};
	color_t				virtual_keyboard_disp_area_text_color={0,0,0,100};

	//Added for calculator color changes
	color_t			calculator_theme_color={234,56,23,100};
	color_t			idle_scr_date_theme_color={234,123,45,100};
	color_t			idle_scr_time_theme_color={234,123,45,100};
	
/******************************************************************************/


/**************************UI_filled_area objects******************************/
    UI_filled_area		LSK_up_DLT=
{
	UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
	UI_NULL_IMAGE,
	NULL,
	{	118,224,15,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};
	UI_filled_area		LSK_down_DLT=
{
	UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER|UI_FILLED_AREA_SHADOW,
	UI_NULL_IMAGE,
	NULL,
	{	118,224,15,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};
	UI_filled_area		LSK_disabled_filler=
{
	UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER|UI_FILLED_AREA_SHADOW,
	UI_NULL_IMAGE,
	NULL,
	{	118,224,15,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};
	UI_filled_area		RSK_up_DLT=
{
	UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
	UI_NULL_IMAGE,
	NULL,
	{	118,224,15,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};
	UI_filled_area		RSK_down_DLT=
{
	UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER|UI_FILLED_AREA_SHADOW,
	UI_NULL_IMAGE,
	NULL,
	{	118,224,15,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};
	UI_filled_area		RSK_disabled_filler=
{
	UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER|UI_FILLED_AREA_SHADOW,
	UI_NULL_IMAGE,
	NULL,
	{	118,224,15,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};

	UI_filled_area		multitap_filler=
{

	UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER|UI_FILLED_AREA_SHADOW|UI_FILLED_AREA_SHADOW_DOUBLE_LINE,
	UI_NULL_IMAGE,
	NULL,
	{	196,228,251,100	},
	{	0,0,0,0			},
	{	0,0,0,100		},
	{	128,128,128,100	},
	0
};
	UI_filled_area		inputbox_filler_DLT;

	UI_filled_area		scrollbar_background_filler_DLT;
	UI_filled_area		scrollbar_indicator_filler;

	UI_filled_area		Title_DLT = 
	{
		UI_FILLED_AREA_TYPE_COLOR,
		UI_IMAGE_ID_NULL,
			NULL,
		{66,96,215,100},
		{0,0,0,100},
		{0,0,0,100},
		{0,0,0,100},
	0};

	UI_filled_area		list_background_filler=
{
	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{	255,255,255,100	},
	{	0,0,0,0			},
	{	0,0,0,100		},
	{	0,0,0,0			},
	0
};
	UI_filled_area 		list_normal_item_filler=
{	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{	190,220,255,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};
	UI_filled_area 		list_selected_item_filler=
{	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{	0,240,255,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};
	UI_filled_area 		list_disabled_item_filler=
{	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{	141,167,191,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};

	UI_filled_area 		matrix_background_filler=
{	UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
	UI_NULL_IMAGE,
	NULL,
	{	255,255,255,100	},
	{	0,0,0,0			},
	{	0,0,0,100		},
	{	0,0,0,0			},
	255	
};
	UI_filled_area 		matrix_normal_item_filler=
{	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{	255,255,255,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};
	UI_filled_area 		matrix_selected_item_filler=
{	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{	51,88,171,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};
	UI_filled_area 		matrix_disabled_item_filler=
{	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{	200,200,200,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};

	UI_filled_area 		shortcut_indicator_background_filler=
{	UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
	UI_NULL_IMAGE,
	NULL,
	{	224,224,224,100	},
	{	0,0,0,0			},
	{	0,0,0,100		},
	{	0,0,0,0			},
	0
};

	UI_filled_area 		formatted_inputbox_normal_filler=
{	UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
	UI_NULL_IMAGE,
	NULL,
	{	200,200,200,100	},
	{	0,0,0,0			},
	{	0,0,0,100		},
	{	0,0,0,0			},
	0
};
	UI_filled_area 		formatted_inputbox_selected_filler=
{	UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
	UI_NULL_IMAGE,
	NULL,
	{	255,255,255,100	},
	{	0,0,0,0			},
	{	0,0,0,100		},
	{	0,0,0,0			},
	0
};
	
	UI_filled_area 		main_menu_background_filler_DLT=
{	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{	255,255,255,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};
	UI_filled_area 	    general_background_filler=
{	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{	255,255,255,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};

	UI_filled_area 		popup_description_background_filler=
{	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{	245,247,115,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};

	UI_filled_area 		inline_list_background_filler=
{
	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{	240,240,240,100	},
	{	0,0,0,0			},
	{	0,0,0,100		},
	{	0,0,0,0			},
	0
};
	UI_filled_area 		inline_list_normal_item_filler={	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{	240,240,240,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};
	UI_filled_area 		inline_list_selected_item_filler={	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{	167,232,93,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};
	UI_filled_area 		inline_list_disabled_item_filler={	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{	192,192,192,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};

	UI_filled_area 		inline_edit_focussed_filler={	UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
	NULL,
	NULL,
	{	167,232,93,100	},
	{	0,0,0,0			},
	{	0,0,0,100		},
	{	0,0,0,0			},
	0
};
	UI_filled_area 		inline_edit_unfocussed_filler=
{	UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
	NULL,
	NULL,
	{	255,255,255,100	},
	{	0,0,0,0			},
	{	0,0,0,100		},
	{	0,0,0,0			},
	0
};
	UI_filled_area 		inline_edit_noboundary_filler=
{	UI_FILLED_AREA_TYPE_COLOR,
	NULL,
	NULL,
	{	167,232,93,100	},
	{	0,0,0,0			},
	{	0,0,0,100		},
	{	0,0,0,0			},
	0
};

	UI_filled_area 		PIN_inputbox_background_filler_DLT=
{
	UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER|UI_FILLED_AREA_SHADOW,
	UI_NULL_IMAGE,
	NULL,
	{	255,222,255,100	},
	{	0,0,0,0			},
	{	255,0,0,100		},
	{	255,0,0,100		},
	0
};
	UI_filled_area 		PIN_screen_background_filler_DLT=
{	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{255,255,255,100},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	0
};
	UI_filled_area 		network_screen_background_filler=
{	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{255,255,255,100},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	0
};
	UI_filled_area 		aphorism_screen_background_filler=
{	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{255,255,255,100},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	0
};
	UI_filled_area 		CM_screen_background_filler=
{	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{255,255,255,100},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	0
};
	UI_filled_area 		dialer_inputbox_background_filler=
{
   UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{	255,255,255,100	},
	{	0,0,0,0			},
	{	0,0,0,100		},
	{	0,0,0,0			},
	0
};
	
	UI_filled_area 		virtual_keyboard_key_down_filler=
{
	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{	0,0,0,100 },
	{	0,0,0,100 },
	{	0,0,0,100 },
	{	128,128,128,100 },
	0
};
	UI_filled_area		alarm_background_filler_DLT=
{
	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
		{	255,00,255,100	},
	{	0,0,0,100 },
	{	0,0,0,100 },
	{	128,128,128,100 },
	0
};

/******************************************************************************/


/****************************button fonts objets*********************************/
	stFontAttribute		butt_font ;
   // UI_font_type		button_font = &butt_font;//=(stFontAttribute*)malloc(sizeof(stFontAttribute));
	stFontAttribute		matrix_menu_font;//=(stFontAttribute*)malloc(sizeof(stFontAttribute));;
	stFontAttribute		multitap_font;
	stFontAttribute		inputbox_font;
	stFontAttribute		window_title_font;
	stFontAttribute		list_menu_font;
	stFontAttribute		menu_bar_index_font;
	stFontAttribute		menu_bar_item_font;
	stFontAttribute		shortcut_indicator_font;
	stFontAttribute		title_text_font;
	stFontAttribute		drop_down_control_font;
	stFontAttribute		list_main_menu_font;
	stFontAttribute		matrix_main_menu_font;
	stFontAttribute		virtual_keyboard_font;
	S32					scrollbar_size;

/********************************************************************************/
/****************************************************************************
MMI theme structure filled using the Downloaded theme file 
*****************************************************************************/

 MMI_theme theme_DLT={
	(UI_filled_area*)&LSK_up_DLT,
	(UI_filled_area*)&LSK_down_DLT,
	(UI_filled_area*)&LSK_disabled_filler,
	(UI_filled_area*)&RSK_up_DLT,
	(UI_filled_area*)&RSK_down_DLT,
	(UI_filled_area*)&RSK_disabled_filler,
	(UI_filled_area*)&multitap_filler,
	(UI_filled_area*)&inputbox_filler_DLT,
	(UI_filled_area*)&scrollbar_background_filler_DLT,
	(UI_filled_area*)&scrollbar_indicator_filler,
	(UI_filled_area*)&Title_DLT,
	(UI_filled_area*)&list_background_filler,
	(UI_filled_area*)&list_normal_item_filler,
	(UI_filled_area*)&list_selected_item_filler,
	(UI_filled_area*)&list_disabled_item_filler,
	(UI_filled_area*)&matrix_background_filler,
	(UI_filled_area*)&matrix_normal_item_filler,
	(UI_filled_area*)&matrix_selected_item_filler,
	(UI_filled_area*)&matrix_disabled_item_filler,
	(UI_filled_area*)&shortcut_indicator_background_filler,
	(UI_filled_area*)&formatted_inputbox_normal_filler,
	(UI_filled_area*)&formatted_inputbox_selected_filler,
	(UI_filled_area*)&main_menu_background_filler_DLT,
	(UI_filled_area*)&general_background_filler,
	(UI_filled_area*)&popup_description_background_filler,
	(UI_filled_area*)&inline_list_background_filler,
	(UI_filled_area*)&inline_list_normal_item_filler,
	(UI_filled_area*)&inline_list_selected_item_filler,
	(UI_filled_area*)&inline_list_disabled_item_filler,
	(UI_filled_area*)&inline_edit_focussed_filler,
	(UI_filled_area*)&inline_edit_unfocussed_filler,
	(UI_filled_area*)&inline_edit_noboundary_filler,
	(UI_filled_area*)&PIN_inputbox_background_filler_DLT,
	(UI_filled_area*)&PIN_screen_background_filler_DLT,
	(UI_filled_area*)&network_screen_background_filler,
	(UI_filled_area*)&aphorism_screen_background_filler,
	(UI_filled_area*)&CM_screen_background_filler,
	(UI_filled_area*)&dialer_inputbox_background_filler,
	(UI_filled_area*)&virtual_keyboard_key_down_filler,
	(UI_filled_area*)&alarm_background_filler_DLT,
	(color_t*)&UI_black_color,
	(color_t*)&UI_white_color,
	(color_t*)&LSK_up_txt_col_DLT,
	(color_t*)&LSK_down_txt_col_DLT,
	(color_t*)&LSK_disabled_text_color,
	(color_t*)&RSK_up_txt_col_DLT,
	(color_t*)&RSK_down_txt_col_DLT,
	(color_t*)&RSK_disabled_text_color,
	(color_t*)&multitap_selector_color_DLT,
	(color_t*)&multitap_normal_text_color_DLT,
	(color_t*)&multitap_selected_text_color_DLT,
	(color_t*)&inputbox_normal_text_color_DLT,
	(color_t*)&inputbox_selected_text_color_DLT,
	(color_t*)&inputbox_selector_color_DLT,
	(color_t*)&inputbox_cursor_color_DLT,
	(color_t*)&title_text_color,
	(color_t*)&title_text_border_color,
	(color_t*)&title_shortcut_text_color,
	(color_t*)&list_normal_text_color,
	(color_t*)&list_selected_text_color,
	(color_t*)&list_disabled_text_color,
	(color_t*)&matrix_normal_text_color,
	(color_t*)&matrix_selected_text_color,
	(color_t*)&matrix_disabled_text_color,
	(color_t*)&menubar_index_background_color,
	(color_t*)&menubar_item_background_color,
	(color_t*)&menubar_index_text_color,
	(color_t*)&menubar_item_text_color,
	(color_t*)&shortcut_indicator_text_color,
	(color_t*)&shortcut_indicator_cursor_color,
	(color_t*)&drop_down_normal_text_color,
	(color_t*)&drop_down_disabled_text_color,
	(color_t*)&drop_down_focussed_text_color,
	(color_t*)&formatted_inputbox_selector_color,
	(color_t*)&formatted_inputbox_normal_text_color,
	(color_t*)&formatted_inputbox_disabled_text_color,
	(color_t*)&formatted_inputbox_selected_text_color,
	(color_t*)&formatted_inputbox_cursor_color,
	(color_t*)&list_main_menu_normal_text_color,
	(color_t*)&list_main_menu_selected_text_color,
	(color_t*)&list_main_menu_disabled_text_color,
	(color_t*)&matrix_main_menu_normal_text_color,
	(color_t*)&matrix_main_menu_selected_text_color,
	(color_t*)&matrix_main_menu_disabled_text_color,
	(color_t*)&circular_main_menu_normal_text_color,
	(color_t*)&circular_main_menu_selected_text_color,
	(color_t*)&circular_main_menu_disabled_text_color,
	(color_t*)&popup_screen_background_color,
	(color_t*)&popup_screen_border_color,
	(color_t*)&popup_description_text_color,
	(color_t*)&datetime_bar_background_color,
	(color_t*)&datetime_bar_duration_background_color,
	(color_t*)&datetime_bar_date_text_color,
	(color_t*)&datetime_bar_time_text_color,
	(color_t*)&datetime_bar_duration_text_color,
	(color_t*)&datetime_bar_AOC_text_color,
	(color_t*)&button_bar_color,
	(color_t*)&information_bar_color,
	(color_t*)&input_method_text_color,
	(color_t*)&remaining_length_text_color,
	(color_t*)&statusbar0_background_color,
	(color_t*)&inline_edit_focussed_text_color,
	(color_t*)&inline_edit_unfocussed_text_color,
	(color_t*)&inline_edit_selected_text_color,
	(color_t*)&inline_edit_selector_color,
	(color_t*)&inline_edit_cursor_color,
	(color_t*)&inline_list_normal_text_color,
	(color_t*)&inline_list_selected_text_color,
	(color_t*)&inline_list_disabled_text_color,
	(color_t*)&PIN_inputbox_normal_text_color_DLT,
	(color_t*)&PIN_inputbox_selected_text_color_DLT,
	(color_t*)&PIN_inputbox_selector_color_DLT,
	(color_t*)&PIN_inputbox_cursor_color_DLT,
	(color_t*)&dialer_inputbox_normal_text_color_DLT,
	(color_t*)&dialer_inputbox_selected_text_color_DLT,
	(color_t*)&dialer_inputbox_selector_color_DLT,
	(color_t*)&dialer_inputbox_cursor_color_DLT,
	(color_t*)&virtual_keyboard_key_down_text_color,
	(color_t*)&virtual_keyboard_key_up_text_color,
	(color_t*)&virtual_keyboard_key_down_text_color,
	(color_t*)&virtual_keyboard_key_up_text_color,
	(color_t*)&virtual_keyboard_disp_area_text_color,
	(color_t*)&calculator_theme_color,
	(color_t*)&idle_scr_date_theme_color,
	(color_t*)&idle_scr_time_theme_color,
	&butt_font,
	&multitap_font,
	&inputbox_font,
	&window_title_font,
	&list_menu_font,
	&matrix_menu_font,
	&menu_bar_index_font,
	&menu_bar_item_font,
	&shortcut_indicator_font,
	&title_text_font,
	&drop_down_control_font,
	&list_main_menu_font,
	&matrix_main_menu_font,
	&virtual_keyboard_font,
	10
};

MMI_theme*	g_pConverterTheme = &theme_DLT;
/*****************************************************************************
* FUNCTION
*	 ascii_to_hex_convert(S8  character)
* DESCRIPTION
*  converts a given character in ansii to hex value
* PARAMETERS
*	character  
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/
U8 ascii_to_hex_convert(S8  character)
{
	
	if(character>47 && character<58)		/* Digits 0..9 */
	{
		return(character-48);
	}
	else if(character>64 && character<71)	/* Characters A..F */
	{
		return(character-55);
	}
	else if(character>96 && character<103)	/* Characters a..f */
	{
		return(character-87);
	}
	else 
	{
		return(0xFF);
	}
}

/*****************************************************************************
* FUNCTION
*	convert_ascii_hex_to_dec
* DESCRIPTION
*   Converts Ascii value to Decimal value
* PARAMETERS
*	S8 ascii values
* RETURNS
*	U8 Decimal value.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
U8 convert_ascii_hex_to_dec(S8  character)
{
	if(character>47 && character<58)		/* Digits 0..9 */
	{
		return(character-48);
	}
	else if(character>64 && character<71)	/* Characters A..F */
	{
		return(character-55);
	}
	else if(character>96 && character<103)	/* Characters a..f */
	{
		return(character-87);
	}
	else 
	{
		return(0xFF);
	}
}


/*****************************************************************************
* FUNCTION
*	 extract_no_of_bits(U8  *Data,U16  Offset, U16  Bits)
* DESCRIPTION
*  This function extracts given no of bits form the string provided
* PARAMETERS
*	character  
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

U8 extract_no_of_bits(U8  *Data,U16  Offset, U16  Bits)
{

	const U16  BITSPERVAL = 4;
	U8 ClearBits[] = {0x0,0x1,0x3,0x7,0xf};
	U8 val1 = 0;
	U8 val2 = 0;
	U8 returnval = 0;
	U16 index = Offset/BITSPERVAL ;
	U8 BitsfromFirstIndex = BITSPERVAL - (Offset % BITSPERVAL);

	if (Bits > 2*BITSPERVAL) return 0;

	val1 = ascii_to_hex_convert(Data[index]);
	val1 &= ClearBits[BitsfromFirstIndex];

	if (BitsfromFirstIndex < Bits)
	{
		/* Do nothing	*/
	}
	else if (BitsfromFirstIndex == Bits)
	{
		return val1;
	}
	else	/*(BitsfromFirstIndex > Bits) */
	{
		return (val1 >> (BitsfromFirstIndex - Bits));
	}

	val2 = convert_ascii_hex_to_dec(Data[index+1]);
	returnval = val1<< BITSPERVAL | val2 ;

	if ((BitsfromFirstIndex+ BITSPERVAL) >= Bits)
	{
		returnval >>= (BITSPERVAL - Bits + BitsfromFirstIndex);
	}
	else
	{
		returnval <<=(Bits - BitsfromFirstIndex - BITSPERVAL);
		val1 = convert_ascii_hex_to_dec(Data[index+2]);
		val1 >>= ((2* BITSPERVAL)+ BitsfromFirstIndex - Bits );
		returnval |= val1 ;
	}

	return returnval;

}

/*****************************************************************************
* FUNCTION
*	 lsk_up_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for lsk up position
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/
void lsk_up_tag_handler(U8** attr)
{	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			LSK_up_DLT.c.r=extract_no_of_bits((PU8)attr[attrCount+1],0,8);
			LSK_up_DLT.c.g=extract_no_of_bits((PU8)attr[attrCount+1],8,8);
			LSK_up_DLT.c.b=extract_no_of_bits((PU8)attr[attrCount+1],16,8);
			LSK_up_DLT.c.alpha=extract_no_of_bits((PU8)attr[attrCount+1],24,8);
		}
		else if(!strncmp((PS8)attr[attrCount],"border_col",strlen("border_col")))
		{

			LSK_up_DLT.border_color.r=extract_no_of_bits((PU8)attr[attrCount+1],0,8);
			LSK_up_DLT.border_color.g=extract_no_of_bits((PU8)attr[attrCount+1],0,8);
			LSK_up_DLT.border_color.b=extract_no_of_bits((PU8)attr[attrCount+1],16,8);
			LSK_up_DLT.border_color.alpha=extract_no_of_bits((PU8)attr[attrCount+1],24,8);
		}
		else if(!strncmp((PS8)attr[attrCount],"flag",strlen("flag")))
		{
			LSK_up_DLT.flags=(U32)atoi((PS8)attr[attrCount+1]);
		}
		else if(!strncmp((PS8)attr[attrCount],"txt_col",strlen("txt_col")))
		{
			LSK_up_txt_col_DLT.r=extract_no_of_bits((PU8)attr[attrCount+1],0,8);
			LSK_up_txt_col_DLT.g=extract_no_of_bits((PU8)attr[attrCount+1],8,8);
			LSK_up_txt_col_DLT.b=extract_no_of_bits((PU8)attr[attrCount+1],16,8);
			LSK_up_txt_col_DLT.alpha=extract_no_of_bits((PU8)attr[attrCount+1],24,8);
		}
		else if(!strncmp((PS8)attr[attrCount],"img_offset",strlen("img_offset")))
		{
		LSK_up_DLT.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		attrCount+=2;
	}

	theme_DLT.LSK_up_filler = (UI_filled_area*)&LSK_up_DLT;
	theme_DLT.LSK_up_text_color = (color_t*)&LSK_up_txt_col_DLT;


}
/*****************************************************************************
* FUNCTION
*	 lsk_down_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for lsk down position
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

void lsk_down_tag_handler(U8** attr)
{
		U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			LSK_down_DLT.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            LSK_down_DLT.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			LSK_down_DLT.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			LSK_down_DLT.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		else if(!strncmp((PS8)attr[attrCount],"border_col",strlen("border_col")))
		{
			LSK_down_DLT.border_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			LSK_down_DLT.border_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			LSK_down_DLT.border_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			LSK_down_DLT.border_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		else if(!strncmp((PS8)attr[attrCount],"flag",strlen("flag")))
		{
			LSK_down_DLT.flags=(U32)atoi((PS8)attr[attrCount+1]);
		}
		else if(!strncmp((PS8)attr[attrCount],"txt_col",strlen("txt_col")))
		{
			LSK_down_txt_col_DLT.r=extract_no_of_bits(attr[attrCount+1],0,8);
			LSK_down_txt_col_DLT.g=extract_no_of_bits(attr[attrCount+1],8,8);
			LSK_down_txt_col_DLT.b=extract_no_of_bits(attr[attrCount+1],16,8);
			LSK_down_txt_col_DLT.alpha=extract_no_of_bits(attr[attrCount+1],24,8);

		}
		else if(!strncmp((PS8)attr[attrCount],"img_offset",strlen("img_offset")))
		{
			LSK_down_DLT.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		attrCount+=2;
	}
	theme_DLT.LSK_down_filler = (UI_filled_area*)&LSK_down_DLT;
	theme_DLT.LSK_down_text_color = (color_t*)&LSK_down_txt_col_DLT;

}
/*****************************************************************************
* FUNCTION
*	 rsk_up_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for rsk up position
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

void rsk_up_tag_handler(U8 **attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			RSK_up_DLT.c.r=extract_no_of_bits((PU8)attr[attrCount+1],0,8);
            RSK_up_DLT.c.g=extract_no_of_bits((PU8)attr[attrCount+1],8,8);
			RSK_up_DLT.c.b=extract_no_of_bits((PU8)attr[attrCount+1],16,8);
			RSK_up_DLT.c.alpha=extract_no_of_bits((PU8)attr[attrCount+1],24,8);
		}
		else if(!strncmp((PS8)attr[attrCount],"border_col",strlen("border_col")))
		{
			
			RSK_up_DLT.border_color.r=extract_no_of_bits((PU8)attr[attrCount+1],0,8);
			RSK_up_DLT.border_color.g=extract_no_of_bits((PU8)attr[attrCount+1],8,8);
			RSK_up_DLT.border_color.b=extract_no_of_bits((PU8)attr[attrCount+1],16,8);
			RSK_up_DLT.border_color.alpha=extract_no_of_bits((PU8)attr[attrCount+1],24,8);
		}
		else if(!strncmp((PS8)attr[attrCount],"flag",strlen("flag")))
		{
			RSK_up_DLT.flags=(U32)atoi((PS8)attr[attrCount+1]);
		}
		else if(!strncmp((PS8)attr[attrCount],"txt_col",strlen("txt_col")))
		{
			RSK_up_txt_col_DLT.r=extract_no_of_bits((PU8)attr[attrCount+1],0,8);
			RSK_up_txt_col_DLT.g=extract_no_of_bits((PU8)attr[attrCount+1],8,8);
			RSK_up_txt_col_DLT.b=extract_no_of_bits((PU8)attr[attrCount+1],16,8);
			RSK_up_txt_col_DLT.alpha=extract_no_of_bits((PU8)attr[attrCount+1],24,8);

		}
		else if(!strncmp((PS8)attr[attrCount],"img_offset",strlen("img_offset")))
		{
			RSK_up_DLT.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		attrCount+=2;
	}
	theme_DLT.RSK_up_filler = (UI_filled_area*)&RSK_up_DLT;
	theme_DLT.RSK_up_text_color = (color_t*)&RSK_up_txt_col_DLT;

	
}
/*****************************************************************************
* FUNCTION
*	 rsk_down_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for rsk down position
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/
void rsk_down_tag_handler(U8** attr)
{
	U8 attrCount = 0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			RSK_down_DLT.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
			RSK_down_DLT.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			RSK_down_DLT.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			RSK_down_DLT.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		else if(!strncmp((PS8)attr[attrCount],"border_col",strlen("border_col")))
		{
			RSK_down_DLT.border_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			RSK_down_DLT.border_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			RSK_down_DLT.border_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			RSK_down_DLT.border_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		else if(!strncmp((PS8)attr[attrCount],"flag",strlen("flag")))
		{
			RSK_down_DLT.flags=(U32)atoi((PS8)attr[attrCount+1]);
		}
		else if(!strncmp((PS8)attr[attrCount],"txt_col",strlen("txt_col")))
		{
			RSK_down_txt_col_DLT.r=extract_no_of_bits(attr[attrCount+1],0,8);
			RSK_down_txt_col_DLT.g=extract_no_of_bits(attr[attrCount+1],8,8);
			RSK_down_txt_col_DLT.b=extract_no_of_bits(attr[attrCount+1],16,8);
			RSK_down_txt_col_DLT.alpha=extract_no_of_bits(attr[attrCount+1],24,8);

		}
		else if(!strncmp((PS8)attr[attrCount],"img_offset",strlen("img_offset")))
		{
			RSK_down_DLT.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		attrCount+=2;
	}
	theme_DLT.RSK_down_filler = (UI_filled_area*)&RSK_down_DLT;
	theme_DLT.RSK_down_text_color = (color_t*)&RSK_down_txt_col_DLT;

}

//Title_TAG handler
/*****************************************************************************
* FUNCTION
*	 Title_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for Title Tag
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/
void title_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			Title_DLT.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            Title_DLT.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			Title_DLT.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			Title_DLT.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		else if(!strncmp((PS8)attr[attrCount],"border_col",strlen("border_col")))
		{
			Title_DLT.border_color.r=extract_no_of_bits(attr[attrCount+1],0,8);

			Title_DLT.border_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			Title_DLT.border_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			Title_DLT.border_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		else if(!strncmp((PS8)attr[attrCount],"flag",strlen("flag")))
		{
			Title_DLT.flags=atoi((PS8)attr[attrCount+1]);
		}
		else if(!strncmp((PS8)attr[attrCount],"txt_col",strlen("txt_col")))
		{
			title_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			title_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			title_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			title_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);

		}
		else if(!strncmp((PS8)attr[attrCount],"txt_border_col",strlen("txt_border_col")))
		{
			title_text_border_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			title_text_border_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			title_text_border_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			title_text_border_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);

		}
		else if(!strncmp((PS8)attr[attrCount],"shortcut_txt_col",strlen("shortcut_txt_col")))
		{
			title_shortcut_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			title_shortcut_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			title_shortcut_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			title_shortcut_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);

		}
		else if(!strncmp((PS8)attr[attrCount],"bk_img_offset",strlen("bk_img_offset")))
		{
			Title_DLT.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		else if(!strncmp((PS8)attr[attrCount],"txtfont",strlen("txtfont")))
		{
				title_text_font.bold=extract_no_of_bits(attr[attrCount+1],0,4);
				title_text_font.italic=extract_no_of_bits(attr[attrCount+1],4,4);
				title_text_font.underline=extract_no_of_bits(attr[attrCount+1],8,4);
				title_text_font.size=extract_no_of_bits(attr[attrCount+1],12,4);
				title_text_font.color_t=extract_no_of_bits(attr[attrCount+1],16,4);
				title_text_font.type=extract_no_of_bits(attr[attrCount+1],20,4);
				title_text_font.oblique=extract_no_of_bits(attr[attrCount+1],24,4);
				title_text_font.smallCaps=extract_no_of_bits(attr[attrCount+1],28,4);
	
		}
		else if(!strncmp((PS8)attr[attrCount],"windowtitlefont",strlen("windowtitlefont")))
		{
				window_title_font.bold=extract_no_of_bits(attr[attrCount+1],0,4);
				window_title_font.italic=extract_no_of_bits(attr[attrCount+1],4,4);
				window_title_font.underline=extract_no_of_bits(attr[attrCount+1],8,4);
				window_title_font.size=extract_no_of_bits(attr[attrCount+1],12,4);
				window_title_font.color_t=extract_no_of_bits(attr[attrCount+1],16,4);
				window_title_font.type=extract_no_of_bits(attr[attrCount+1],20,4);
				window_title_font.oblique=extract_no_of_bits(attr[attrCount+1],24,4);
				window_title_font.smallCaps=extract_no_of_bits(attr[attrCount+1],28,4);
		}
		attrCount+=2;
	}
	theme_DLT.title_filler=(UI_filled_area*)&Title_DLT;
	theme_DLT.window_title_font=(stFontAttribute*)&window_title_font;
	theme_DLT.title_text_font=(stFontAttribute*)&title_text_font;
	theme_DLT.title_shortcut_text_color=(color_t*)&title_shortcut_text_color;
	theme_DLT.title_text_border_color=(color_t*)&title_text_border_color;
	theme_DLT.title_text_color=(color_t*)&title_text_color;

}

/*****************************************************************************
* FUNCTION
*	 scroll_bar_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for scroll bar 
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

void scroll_bar_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
			if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			scrollbar_background_filler_DLT.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            scrollbar_background_filler_DLT.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			scrollbar_background_filler_DLT.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			scrollbar_background_filler_DLT.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"indicator_col",strlen("indicator_col")))
		{
			scrollbar_indicator_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            scrollbar_indicator_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			scrollbar_indicator_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			scrollbar_indicator_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"size",strlen("size")))
		{
			theme_DLT.scrollbar_size =(S32)atoi((const char*)attr[attrCount+1]);
		}
			attrCount+=2;
	}	
	theme_DLT.scrollbar_background_filler=(UI_filled_area*)&scrollbar_background_filler_DLT;
	theme_DLT.scrollbar_indicator_filler=(UI_filled_area*)&scrollbar_indicator_filler;
//	theme_DLT.scrollbar_size=(S32)&scrollbar_size;
}
	
/*****************************************************************************
* FUNCTION
*	 multi_tap_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for multi tap structure
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

void multi_tap_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
			{
					multitap_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
					multitap_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
					multitap_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
					multitap_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
			}
		if(!strncmp((PS8)attr[attrCount],"border_col",strlen("border_col")))
			{
					multitap_filler.border_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
					multitap_filler.border_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
					multitap_filler.border_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
					multitap_filler.border_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
			}
			if(!strncmp((PS8)attr[attrCount],"img_offset",strlen("img_offset")))
			{
				multitap_filler.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
			
			}
		if(!strncmp((PS8)attr[attrCount],"flag",strlen("flag")))
			{
					multitap_filler.flags=(U32)atoi((PS8)attr[attrCount+1]);
			}
			if(!strncmp((PS8)attr[attrCount],"txt_col",strlen("txt_col")))
			{
					multitap_normal_text_color_DLT.r=extract_no_of_bits(attr[attrCount+1],0,8);
					multitap_normal_text_color_DLT.g=extract_no_of_bits(attr[attrCount+1],8,8);
					multitap_normal_text_color_DLT.b=extract_no_of_bits(attr[attrCount+1],16,8);
					multitap_normal_text_color_DLT.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
			}
			if(!strncmp((PS8)attr[attrCount],"selection_col",strlen("selection_col")))
			{
					multitap_selector_color_DLT.r=extract_no_of_bits(attr[attrCount+1],0,8);
					multitap_selector_color_DLT.g=extract_no_of_bits(attr[attrCount+1],8,8);
					multitap_selector_color_DLT.b=extract_no_of_bits(attr[attrCount+1],16,8);
					multitap_selector_color_DLT.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
			}
			if(!strncmp((PS8)attr[attrCount],"sel_txt_col",strlen("sel_txt_col")))
			
			{
					multitap_selected_text_color_DLT.r=extract_no_of_bits(attr[attrCount+1],0,8);
					multitap_selected_text_color_DLT.g=extract_no_of_bits(attr[attrCount+1],8,8);
					multitap_selected_text_color_DLT.b=extract_no_of_bits(attr[attrCount+1],16,8);
					multitap_selected_text_color_DLT.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
			}
			if(!strncmp((PS8)attr[attrCount],"font",strlen("font")))
			{
					
				multitap_font.bold=extract_no_of_bits(attr[attrCount+1],0,4);
				multitap_font.italic=extract_no_of_bits(attr[attrCount+1],4,4);
				multitap_font.underline=extract_no_of_bits(attr[attrCount+1],8,4);
				multitap_font.size=extract_no_of_bits(attr[attrCount+1],12,4);
				multitap_font.color_t=extract_no_of_bits(attr[attrCount+1],16,4);
				multitap_font.type=extract_no_of_bits(attr[attrCount+1],20,4);
				multitap_font.oblique=extract_no_of_bits(attr[attrCount+1],24,4);
				multitap_font.smallCaps=extract_no_of_bits(attr[attrCount+1],28,4);
		
			}
			attrCount+=2;
	}
	theme_DLT.multitap_font=(stFontAttribute*)&multitap_font;
	theme_DLT.multitap_selected_text_color=(color_t*)&multitap_selected_text_color_DLT;
	theme_DLT.multitap_filler=(UI_filled_area*)&multitap_filler;
	theme_DLT.multitap_normal_text_color=(color_t*)&multitap_normal_text_color_DLT;
	theme_DLT.multitap_selector_color=(color_t*)&multitap_selector_color_DLT;

					
}
/*****************************************************************************
* FUNCTION
*	 call scr handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for caller screen
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

void call_scr_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			CM_screen_background_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            CM_screen_background_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			CM_screen_background_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			CM_screen_background_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"border_col",strlen("border_col")))
		{
			CM_screen_background_filler.border_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			CM_screen_background_filler.border_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			CM_screen_background_filler.border_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			CM_screen_background_filler.border_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"txt_col",strlen("txt_col")))
		{
			CM_screen_background_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            CM_screen_background_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			CM_screen_background_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			CM_screen_background_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"img_offset",strlen("img_offset")))
		{
			CM_screen_background_filler.b = tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		if(!strncmp((PS8)attr[attrCount],"flag",strlen("flag")))
		{
			CM_screen_background_filler.flags=atoi((PS8)attr[attrCount+1]);
		}
		
		attrCount+=2;
	}
	theme_DLT.CM_screen_background_filler=(UI_filled_area*)&CM_screen_background_filler;

}
	
/*****************************************************************************
* FUNCTION
*	 datetimebar_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for datetimebar
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

	
void datetimebar_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{

		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			datetime_bar_background_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            datetime_bar_background_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			datetime_bar_background_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			datetime_bar_background_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"duration_bk_col",strlen("duration_bk_col")))
		{
			datetime_bar_duration_background_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            datetime_bar_duration_background_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			datetime_bar_duration_background_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			datetime_bar_duration_background_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"duration_txt_col",strlen("duration_txt_col")))
		{
			datetime_bar_duration_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            datetime_bar_duration_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			datetime_bar_duration_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			datetime_bar_duration_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"aoc_txt_col",strlen("aoc_txt_col")))
		{
			datetime_bar_AOC_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            datetime_bar_AOC_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			datetime_bar_AOC_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			datetime_bar_AOC_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}

		if(!strncmp((PS8)attr[attrCount],"dt_txt_col",strlen("dt_txt_col")))
		{
			datetime_bar_date_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            datetime_bar_date_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			datetime_bar_date_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			datetime_bar_date_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
			if(!strncmp((PS8)attr[attrCount],"tm_txt_col",strlen("tm_txt_col")))
		{
			datetime_bar_time_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            datetime_bar_time_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			datetime_bar_time_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			datetime_bar_time_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		attrCount+=2;
	}
	theme_DLT.datetime_bar_AOC_text_color=(color_t*)&datetime_bar_AOC_text_color;
	theme_DLT.datetime_bar_background_color=(color_t*)&datetime_bar_background_color;
	theme_DLT.datetime_bar_date_text_color=(color_t*)&datetime_bar_date_text_color;
	theme_DLT.datetime_bar_duration_background_color=(color_t*)&datetime_bar_duration_background_color;
	theme_DLT.datetime_bar_duration_text_color=(color_t*)&datetime_bar_duration_text_color;
	theme_DLT.datetime_bar_time_text_color=(color_t*)&datetime_bar_time_text_color;
	
}
	
/*****************************************************************************
* FUNCTION
*	 drop_dn_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for drop_dn
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

	
void drop_dn_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"normal_txt_col",strlen("normal_txt_col")))
		{
			drop_down_normal_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            drop_down_normal_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			drop_down_normal_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			drop_down_normal_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"disabled_txt_col",strlen("disabled_txt_col")))
		{
			drop_down_disabled_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            drop_down_disabled_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			drop_down_disabled_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			drop_down_disabled_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"focussed_txt_col",strlen("focussed_txt_col")))
		{
			drop_down_focussed_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            drop_down_focussed_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			drop_down_focussed_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			drop_down_focussed_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"font",strlen("font")))
		{
				drop_down_control_font.bold=extract_no_of_bits(attr[attrCount+1],0,4);
				drop_down_control_font.italic=extract_no_of_bits(attr[attrCount+1],4,4);
				drop_down_control_font.underline=extract_no_of_bits(attr[attrCount+1],8,4);
				drop_down_control_font.size=extract_no_of_bits(attr[attrCount+1],12,4);
				drop_down_control_font.color_t=extract_no_of_bits(attr[attrCount+1],16,4);
				drop_down_control_font.type=extract_no_of_bits(attr[attrCount+1],20,4);
				drop_down_control_font.oblique=extract_no_of_bits(attr[attrCount+1],24,4);
				drop_down_control_font.smallCaps=extract_no_of_bits(attr[attrCount+1],28,4);

}
		attrCount+=2;
	}
	theme_DLT.drop_down_control_font=(stFontAttribute*)&drop_down_control_font;
	theme_DLT.drop_down_disabled_text_color=(color_t*)&drop_down_disabled_text_color;
	theme_DLT.drop_down_focussed_text_color=(color_t*)&drop_down_focussed_text_color;
	theme_DLT.drop_down_normal_text_color=(color_t*)&drop_down_normal_text_color;
	theme_DLT.drop_down_control_font=&drop_down_control_font;
}
/*****************************************************************************
* FUNCTION
*	 dialer_ip_box_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for dialer input box screen
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

void dialer_ip_box_tag_handler(U8** attr)
{
	
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			dialer_inputbox_background_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            dialer_inputbox_background_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			dialer_inputbox_background_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			dialer_inputbox_background_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"flag",strlen("flag")))
		{
			dialer_inputbox_background_filler.flags=atoi((PS8)attr[attrCount]);
		}
		if(!strncmp((PS8)attr[attrCount],"txt_col",strlen("txt_col")))
		{
			
					dialer_inputbox_normal_text_color_DLT.r=extract_no_of_bits(attr[attrCount+1],0,8);
					dialer_inputbox_normal_text_color_DLT.g=extract_no_of_bits(attr[attrCount+1],8,8);
					dialer_inputbox_normal_text_color_DLT.b=extract_no_of_bits(attr[attrCount+1],16,8);
					dialer_inputbox_normal_text_color_DLT.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
			if(!strncmp((PS8)attr[attrCount],"selection_col",strlen("selection_col")))
		{
					dialer_inputbox_selector_color_DLT.r=extract_no_of_bits(attr[attrCount+1],0,8);
					dialer_inputbox_selector_color_DLT.g=extract_no_of_bits(attr[attrCount+1],8,8);
					dialer_inputbox_selector_color_DLT.b=extract_no_of_bits(attr[attrCount+1],16,8);
					dialer_inputbox_selector_color_DLT.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"sel_txt_col",strlen("sel_txt_col")))
	
		{
					dialer_inputbox_selected_text_color_DLT.r=extract_no_of_bits(attr[attrCount+1],0,8);
					dialer_inputbox_selected_text_color_DLT.g=extract_no_of_bits(attr[attrCount+1],8,8);
					dialer_inputbox_selected_text_color_DLT.b=extract_no_of_bits(attr[attrCount+1],16,8);
					dialer_inputbox_selected_text_color_DLT.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"cursor_col",strlen("cursor_col")))
		{
					dialer_inputbox_cursor_color_DLT.r=extract_no_of_bits(attr[attrCount+1],0,8);
					dialer_inputbox_cursor_color_DLT.g=extract_no_of_bits(attr[attrCount+1],8,8);
					dialer_inputbox_cursor_color_DLT.b=extract_no_of_bits(attr[attrCount+1],16,8);
					dialer_inputbox_cursor_color_DLT.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		attrCount+=2;
	}
	theme_DLT.dialer_inputbox_background_filler=(UI_filled_area*)&dialer_inputbox_background_filler;
	theme_DLT.dialer_inputbox_cursor_color=(color_t*)&dialer_inputbox_cursor_color_DLT;
	theme_DLT.dialer_inputbox_normal_text_color =(color_t*)&dialer_inputbox_normal_text_color_DLT;
	theme_DLT.dialer_inputbox_selected_text_color=(color_t*)&dialer_inputbox_selected_text_color_DLT;
	theme_DLT.dialer_inputbox_selector_color=(color_t*)&dialer_inputbox_selector_color_DLT;
}


/*****************************************************************************
* FUNCTION
*	 inline_edit_scr_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for inline edit screen
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

void inline_edit_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"focussed_bk_col",strlen("focussed_bk_col")))
		{
			inline_edit_focussed_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            inline_edit_focussed_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inline_edit_focussed_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inline_edit_focussed_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"focussed_bk_imgoffset",strlen("focussed_bk_imgoffset")))
		{
			inline_edit_focussed_filler.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		if(!strncmp((PS8)attr[attrCount],"border_col",strlen("border_col")))
		{
			inline_edit_focussed_filler.border_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            inline_edit_focussed_filler.border_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inline_edit_focussed_filler.border_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inline_edit_focussed_filler.border_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"sel_bk_col",strlen("sel_bk_col")))
		{
			inline_edit_focussed_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            inline_edit_focussed_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inline_edit_focussed_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inline_edit_focussed_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
			if(!strncmp((PS8)attr[attrCount],"focussed_flag",strlen("focussed_flag")))
		{
			inline_edit_focussed_filler.flags=(U32)atoi((PS8)attr[attrCount+1]);
		}
		if(!strncmp((PS8)attr[attrCount],"focussed_txt_col",strlen("focussed_txt_col")))
		{
			inline_edit_focussed_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			inline_edit_focussed_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inline_edit_focussed_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inline_edit_focussed_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"selection_col",strlen("selection_col")))
		{
			inline_edit_selector_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			inline_edit_selector_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inline_edit_selector_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inline_edit_selector_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"sel_txt_col",strlen("sel_txt_col")))
		
		{
			inline_edit_selected_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			inline_edit_selected_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inline_edit_selected_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inline_edit_selected_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"cursor_col",strlen("cursor_col")))
		{
			inline_edit_cursor_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			inline_edit_cursor_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inline_edit_cursor_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inline_edit_cursor_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"unfocussed_bk_col",strlen("unfocussed_bk_col")))
		{
			inline_edit_unfocussed_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
			inline_edit_unfocussed_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inline_edit_unfocussed_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inline_edit_unfocussed_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"unfocussed_bk_imgoffset",strlen("unfocussed_bk_imgoffset")))
		{
			inline_edit_unfocussed_filler.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		if(!strncmp((PS8)attr[attrCount],"unfocussed_flag",strlen("unfocussed_flag")))
		{
			inline_edit_unfocussed_filler.flags=(U32)atoi((PS8)attr[attrCount+1]);
		}
		if(!strncmp((PS8)attr[attrCount],"unfocussed_txt_col",strlen("unfocussed_txt_col")))
		{
			inline_edit_unfocussed_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			inline_edit_unfocussed_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inline_edit_unfocussed_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inline_edit_unfocussed_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"noboundary_bk_col",strlen("noboundary_bk_col")))
		{
			inline_edit_noboundary_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
			inline_edit_noboundary_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inline_edit_noboundary_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inline_edit_noboundary_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"noboundary_bk_imgoffset",strlen("noboundary_bk_imgoffset")))
		{
			inline_edit_noboundary_filler.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		if(!strncmp((PS8)attr[attrCount],"noboundary_flag",strlen("noboundary_flag")))
		{
			inline_edit_noboundary_filler.flags=(U32)atoi((PS8)attr[attrCount+1]);
			
		}
		attrCount+=2;
	}
	theme_DLT.inline_edit_cursor_color=(color_t*)&inline_edit_cursor_color;
	theme_DLT.inline_edit_focussed_filler=(UI_filled_area*)&inline_edit_focussed_filler;
	theme_DLT.inline_edit_focussed_text_color=(color_t*)&inline_edit_focussed_text_color;
	theme_DLT.inline_edit_noboundary_filler=(UI_filled_area*)&inline_edit_noboundary_filler;
	theme_DLT.inline_edit_selected_text_color=(color_t*)&inline_edit_selected_text_color;
	theme_DLT.inline_edit_selector_color=(color_t*)&inline_edit_selector_color;
	theme_DLT.inline_edit_unfocussed_filler=(UI_filled_area*)&inline_edit_unfocussed_filler;
	theme_DLT.inline_edit_unfocussed_text_color=(color_t*)&inline_edit_unfocussed_text_color;
}

/*****************************************************************************
* FUNCTION
*	progress_scr_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for progress screen position
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/
void progress_scr_tag_handler(U8** attr)
{
}
/*****************************************************************************
* FUNCTION
*	 deviceinfo_tag_handler(U8** attr)
* DESCRIPTION
*  Gives the information about the device structurewhich is downloading the theme
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/
extern U16 mmi_check_theme_file_lcd_dimension_compatibiltiy(U32 lcd_width, U32 lcd_height);


void deviceinfo_tag_handler(U8** attr)
{

	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"model_ID",strlen("model_ID")))
		{
			gdeviceInfo.model_ID=(U8)atoi((PS8)attr[attrCount+1]);
			
		}	
		if(!strncmp((PS8)attr[attrCount],"lcdwidth",strlen("lcdwidth")))
		{
			gdeviceInfo.lcdwidth=(U32)atoi((PS8)attr[attrCount+1]);
			
		}
		if(!strncmp((PS8)attr[attrCount],"lcdht",strlen("lcdht")))
		{
			gdeviceInfo.lcdht=(U32)atoi((PS8)attr[attrCount+1]);
		}
		attrCount+=2;


	}


}
/*****************************************************************************
* FUNCTION
*	 main_menu_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for main menu screen
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/
void main_menu_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			main_menu_background_filler_DLT.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            main_menu_background_filler_DLT.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			main_menu_background_filler_DLT.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			main_menu_background_filler_DLT.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"flag",strlen("flag")))
		{
			main_menu_background_filler_DLT.flags=(U32)atoi((PS8)attr[attrCount+1]);//|000|4096;
		}
		if(!strncmp((PS8)attr[attrCount],"bk_image_offset",strlen("bk_image_offset")))
		{
			main_menu_background_filler_DLT.b = tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		if(!strncmp((PS8)attr[attrCount],"list_txt_col",strlen("list_txt_col")))
		{
			list_main_menu_normal_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            list_main_menu_normal_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			list_main_menu_normal_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			list_main_menu_normal_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"list_sel_txt_col",strlen("list_sel_txt_col")))
		{
			list_main_menu_selected_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            list_main_menu_selected_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			list_main_menu_selected_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			list_main_menu_selected_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"matrix_sel_txt_col",strlen("matrix_sel_txt_col")))
		{
			matrix_main_menu_selected_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            matrix_main_menu_selected_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			matrix_main_menu_selected_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			matrix_main_menu_selected_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}

		if(!strncmp((PS8)attr[attrCount],"matrix_txt_col",strlen("matrix_txt_col")))
		{
			matrix_main_menu_normal_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            matrix_main_menu_normal_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			matrix_main_menu_normal_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			matrix_main_menu_normal_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"circ_sel_txt_col",strlen("circ_sel_txt_col")))
		{
			circular_main_menu_selected_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            circular_main_menu_selected_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			circular_main_menu_selected_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			circular_main_menu_selected_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"circ_txt_col",strlen("circ_txt_col")))
		{
			circular_main_menu_normal_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            circular_main_menu_normal_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			circular_main_menu_normal_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			circular_main_menu_normal_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"list_mm_font",strlen("list_mm_font")))
		{
				list_main_menu_font.bold=extract_no_of_bits(attr[attrCount+1],0,4);
				list_main_menu_font.italic=extract_no_of_bits(attr[attrCount+1],4,4);
				list_main_menu_font.underline=extract_no_of_bits(attr[attrCount+1],8,4);
				list_main_menu_font.size=extract_no_of_bits(attr[attrCount+1],12,4);
				list_main_menu_font.color_t=extract_no_of_bits(attr[attrCount+1],16,4);
				list_main_menu_font.type=extract_no_of_bits(attr[attrCount+1],20,4);
				list_main_menu_font.oblique=extract_no_of_bits(attr[attrCount+1],24,4);
				list_main_menu_font.smallCaps=extract_no_of_bits(attr[attrCount+1],28,4);
		}
		if(!strncmp((PS8)attr[attrCount],"matrix_mm_font",strlen("matrix_mm_font")))
		{
				matrix_main_menu_font.bold=extract_no_of_bits(attr[attrCount+1],0,4);
				matrix_main_menu_font.italic=extract_no_of_bits(attr[attrCount+1],4,4);
				matrix_main_menu_font.underline=extract_no_of_bits(attr[attrCount+1],8,4);
				matrix_main_menu_font.size=extract_no_of_bits(attr[attrCount+1],12,4);
				matrix_main_menu_font.color_t=extract_no_of_bits(attr[attrCount+1],16,4);
				matrix_main_menu_font.type=extract_no_of_bits(attr[attrCount+1],20,4);
				matrix_main_menu_font.oblique=extract_no_of_bits(attr[attrCount+1],24,4);
				matrix_main_menu_font.smallCaps=extract_no_of_bits(attr[attrCount+1],28,4);
		}
			attrCount+=2;
	}
	theme_DLT.main_menu_background_filler=(UI_filled_area*)&main_menu_background_filler_DLT;
	theme_DLT.matrix_main_menu_font=(stFontAttribute*)&matrix_main_menu_font;
	theme_DLT.list_main_menu_font=(stFontAttribute*)&list_main_menu_font;
	theme_DLT.circular_main_menu_normal_text_color=(color_t*)&circular_main_menu_normal_text_color;
	theme_DLT.circular_main_menu_selected_text_color=(color_t*)&circular_main_menu_selected_text_color;
	theme_DLT.matrix_main_menu_normal_text_color=(color_t*)&matrix_main_menu_normal_text_color;
	theme_DLT.matrix_main_menu_selected_text_color=(color_t*)&matrix_main_menu_selected_text_color;
	theme_DLT.list_main_menu_selected_text_color=(color_t*)&list_main_menu_selected_text_color;
	theme_DLT.list_main_menu_normal_text_color=(color_t*)&list_main_menu_normal_text_color;
	theme_DLT.matrix_main_menu_font=&matrix_main_menu_font;
	theme_DLT.list_main_menu_font=&list_main_menu_font;
}
	
/*****************************************************************************
* FUNCTION
*	 tones_list_tag_handler(U8** attr)
* DESCRIPTION
*  Function for handling the downloaded audio file
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/
void tones_list_tag_handler(U8** attr)
{
	U8 attrCount=0;
	char audio_file_name[25];

		if(!strncmp((PS8)attr[attrCount],"name",strlen("name")))
		{
		/*	if(!strncmp((PS8)attr[attrCount+1],"inc_call",strlen("inc_call")))
			{*/
				U32 offset=0,size=0;
				
				strcpy(audio_file_name,(PS8)attr[attrCount+1]);
				offset=(U32)atoi((PS8)attr[attrCount+3]);
				size=(U32)atoi((PS8)attr[attrCount+5]);
				
				if((strstr(audio_file_name,".wav")!=NULL) || (strstr(audio_file_name,".WAV")!=NULL) )
				{
					gaudiothmfileformat=THEME_AUDIO;
				}
				else if((strstr(audio_file_name,".mid")!=NULL) || (strstr(audio_file_name,".MID")!=NULL))
				{
					gaudiothmfileformat=THEME_MIDI;
				}

				tm_handle_audio(offset,size);
			/*}*/
		}

	
	
}
/*****************************************************************************
* FUNCTION
*	 setting_PIN_scr_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for setting PIN screen handler
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/
void setting_pin_scr_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			PIN_screen_background_filler_DLT.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            PIN_screen_background_filler_DLT.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			PIN_screen_background_filler_DLT.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			PIN_screen_background_filler_DLT.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"border_col",strlen("border_col")))
		{
			PIN_screen_background_filler_DLT.border_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			PIN_screen_background_filler_DLT.border_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			PIN_screen_background_filler_DLT.border_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			PIN_screen_background_filler_DLT.border_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
			if(!strncmp((PS8)attr[attrCount],"img_offset",strlen("img_offset")))
		{
			PIN_screen_background_filler_DLT.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
			if(!strncmp((PS8)attr[attrCount],"flag",strlen("flag")))
		{
			PIN_screen_background_filler_DLT.flags=(U32)atoi((PS8)attr[attrCount+1]);
		}

		attrCount+=2;
	}
	theme_DLT.PIN_screen_background_filler=(UI_filled_area*)&PIN_screen_background_filler_DLT;

}


/*****************************************************************************
* FUNCTION
* setimagehandler(U16 ImageID)	 
* DESCRIPTION
*  sets the image using the image ID provided
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

void setimagehandler(U16 ImageID)
{
}


//My new coding starts here
void nw_scr_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			network_screen_background_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            network_screen_background_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			network_screen_background_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			network_screen_background_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"border_col",strlen("border_col")))
		{
			network_screen_background_filler.border_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			network_screen_background_filler.border_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			network_screen_background_filler.border_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			network_screen_background_filler.border_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"img_offset",strlen("img_offset")))
		{
				network_screen_background_filler.b = tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		if(!strncmp((PS8)attr[attrCount],"flag",strlen("flag")))
		{
			network_screen_background_filler.flags=(U32)atoi((PS8)attr[attrCount+1]);
		}
			attrCount+=2;
	}
	theme_DLT.network_screen_background_filler=(UI_filled_area*)&network_screen_background_filler;
	
}

/*****************************************************************************
* FUNCTION
*	 buttonbar_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for buttonbar
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/


void buttonbar_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			button_bar_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            button_bar_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			button_bar_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			button_bar_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"font",strlen("font")))
		{
				butt_font.bold=extract_no_of_bits(attr[attrCount+1],0,4);
				butt_font.italic=extract_no_of_bits(attr[attrCount+1],4,4);
				butt_font.underline=extract_no_of_bits(attr[attrCount+1],8,4);
				butt_font.size=extract_no_of_bits(attr[attrCount+1],12,4);
				butt_font.color_t=extract_no_of_bits(attr[attrCount+1],16,4);
				butt_font.type=extract_no_of_bits(attr[attrCount+1],20,4);
				butt_font.oblique=extract_no_of_bits(attr[attrCount+1],24,4);
				butt_font.smallCaps=extract_no_of_bits(attr[attrCount+1],28,4);
		}
		attrCount+=2;
	}
	theme_DLT.button_font=(stFontAttribute*)&butt_font;
	theme_DLT.button_bar_color=(color_t*)&button_bar_color;
}

/*****************************************************************************
* FUNCTION
*	 popup_scr_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for popup_scr
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/



void popup_scr_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			popup_screen_background_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            popup_screen_background_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			popup_screen_background_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			popup_screen_background_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"border_col",strlen("border_col")))
		{
			popup_screen_border_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			popup_screen_border_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			popup_screen_border_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			popup_screen_border_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		attrCount+=2;
	}
	theme_DLT.popup_screen_background_color=(color_t*)&popup_screen_background_color;
	theme_DLT.popup_screen_border_color=(color_t*)&popup_screen_border_color;
}

/*****************************************************************************
* FUNCTION
*	 aphorism_scr_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for aphorism_scr
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/


void aphorism_scr_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			aphorism_screen_background_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            aphorism_screen_background_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			aphorism_screen_background_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			aphorism_screen_background_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"border_col",strlen("border_col")))
		{
			aphorism_screen_background_filler.border_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			aphorism_screen_background_filler.border_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			aphorism_screen_background_filler.border_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			aphorism_screen_background_filler.border_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"img_offset",strlen("img_offset")))
		{
			aphorism_screen_background_filler.b = tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		attrCount+=2;
	}
	theme_DLT.aphorism_screen_background_filler=(UI_filled_area*)&aphorism_screen_background_filler;

}

/*****************************************************************************
* FUNCTION
*	 popup_description_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for popup_description
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/




void popup_description_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			popup_description_background_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            popup_description_background_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			popup_description_background_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			popup_description_background_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"border_col",strlen("border_col")))
		{
			popup_description_background_filler.border_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			popup_description_background_filler.border_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			popup_description_background_filler.border_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			popup_description_background_filler.border_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"img_offset",strlen("img_offset")))
		{
			popup_description_background_filler.b = tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		if(!strncmp((PS8)attr[attrCount],"txt_col",strlen("txt_col")))
		{
			popup_description_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            popup_description_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			popup_description_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			popup_description_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"flag",strlen("flag")))
		{
			popup_description_background_filler.flags=(U32)atoi((PS8)attr[attrCount+1]);
		}
		attrCount+=2;
	}
	theme_DLT.popup_description_background_filler=(UI_filled_area*)&popup_description_background_filler;
	theme_DLT.popup_description_text_color=(color_t*)&popup_description_text_color;

}

/*****************************************************************************
* FUNCTION
*	 virtual_kb_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for virtual_kb
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

void virtual_kb_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			virtual_keyboard_key_down_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            virtual_keyboard_key_down_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			virtual_keyboard_key_down_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			virtual_keyboard_key_down_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"border_col",strlen("border_col")))
		{
			virtual_keyboard_key_down_filler.border_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			virtual_keyboard_key_down_filler.border_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			virtual_keyboard_key_down_filler.border_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			virtual_keyboard_key_down_filler.border_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"flag",strlen("flag")))
		{
			virtual_keyboard_key_down_filler.flags=atoi((PS8)attr[attrCount]);
		}
		if(!strncmp((PS8)attr[attrCount],"img_offset",strlen("img_offset")))
		{
				virtual_keyboard_key_down_filler.b = tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		if(!strncmp((PS8)attr[attrCount],"key_up_txt_col",strlen("key_up_txt_col")))
		{
			virtual_keyboard_key_up_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			virtual_keyboard_key_up_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			virtual_keyboard_key_up_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			virtual_keyboard_key_up_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
			if(!strncmp((PS8)attr[attrCount],"key_down_txt_col",strlen("key_down_txt_col")))
		{
			virtual_keyboard_key_down_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			virtual_keyboard_key_down_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			virtual_keyboard_key_down_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			virtual_keyboard_key_down_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
	if(!strncmp((PS8)attr[attrCount],"font",strlen("font")))
		{
				virtual_keyboard_font.bold=extract_no_of_bits(attr[attrCount+1],0,4);
				virtual_keyboard_font.italic=extract_no_of_bits(attr[attrCount+1],4,4);
				virtual_keyboard_font.underline=extract_no_of_bits(attr[attrCount+1],8,4);
				virtual_keyboard_font.size=extract_no_of_bits(attr[attrCount+1],12,4);
				virtual_keyboard_font.color_t=extract_no_of_bits(attr[attrCount+1],16,4);
				virtual_keyboard_font.type=extract_no_of_bits(attr[attrCount+1],20,4);
				virtual_keyboard_font.oblique=extract_no_of_bits(attr[attrCount+1],24,4);
				virtual_keyboard_font.smallCaps=extract_no_of_bits(attr[attrCount+1],28,4);
	
		}
			attrCount+=2;
	}
	theme_DLT.virtual_keyboard_font=(stFontAttribute*)&virtual_keyboard_font;
	theme_DLT.virtual_keyboard_key_up_text_color=(color_t*)&virtual_keyboard_key_up_text_color;
	theme_DLT.virtual_keyboard_key_down_text_color=(color_t*)&virtual_keyboard_key_down_text_color;
	theme_DLT.virtual_keyboard_key_down_filler=(UI_filled_area*)&virtual_keyboard_key_down_filler;
}

/*****************************************************************************
* FUNCTION
*	 format_ipbox_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for format_ipbox
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/


void format_ipbox_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"normal_bk_col",strlen("normal_bk_col")))
		{
			formatted_inputbox_normal_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            formatted_inputbox_normal_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			formatted_inputbox_normal_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			formatted_inputbox_normal_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"normal_flag",strlen("normal_flag")))
		{
			formatted_inputbox_normal_filler.flags=atoi((PS8)attr[attrCount+1]);

		}
		if(!strncmp((PS8)attr[attrCount],"normal_txt_col",strlen("normal_txt_col")))
		{
			
			formatted_inputbox_normal_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			formatted_inputbox_normal_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			formatted_inputbox_normal_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			formatted_inputbox_normal_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"selected_bk_col",strlen("selected_bk_col")))
		{
			formatted_inputbox_selected_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
			formatted_inputbox_selected_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			formatted_inputbox_selected_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			formatted_inputbox_selected_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"selected_flag",strlen("selected_flag")))
		{
			formatted_inputbox_selected_filler.flags=atoi((PS8)attr[attrCount+1]);
		}
		if(!strncmp((PS8)attr[attrCount],"sel_txt_col",strlen("sel_txt_col")))
		{
			formatted_inputbox_selected_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			formatted_inputbox_selected_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			formatted_inputbox_selected_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			formatted_inputbox_selected_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
			if(!strncmp((PS8)attr[attrCount],"cursor_col",strlen("cursor_col")))
		{
			formatted_inputbox_cursor_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			formatted_inputbox_cursor_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			formatted_inputbox_cursor_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			formatted_inputbox_cursor_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"selection_col",strlen("selection_col")))
		{
			formatted_inputbox_selector_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			formatted_inputbox_selector_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			formatted_inputbox_selector_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			formatted_inputbox_selector_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		attrCount+=2;
	}
	theme_DLT.formatted_inputbox_selector_color=(color_t*)&formatted_inputbox_selector_color;
	theme_DLT.formatted_inputbox_cursor_color=(color_t*)&formatted_inputbox_cursor_color;
	theme_DLT.formatted_inputbox_selected_text_color=(color_t*)&formatted_inputbox_selected_text_color;
	theme_DLT.formatted_inputbox_selected_filler=(UI_filled_area*)&formatted_inputbox_selected_filler;
	theme_DLT.formatted_inputbox_normal_filler=(UI_filled_area*)&formatted_inputbox_normal_filler;

}

/*****************************************************************************
* FUNCTION
*	 general_bkgrnd_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for general_bkgrnd
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

void general_bkgrnd_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
			if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			general_background_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            general_background_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			general_background_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			general_background_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"flag",strlen("flag")))
		{
			general_background_filler.flags=atoi((PS8)attr[attrCount+1]);

		}
		if(!strncmp((PS8)attr[attrCount],"border_col",strlen("border_col")))
		{
			
			general_background_filler.border_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			general_background_filler.border_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			general_background_filler.border_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			general_background_filler.border_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"img_offset",strlen("img_offset")))
		{
				general_background_filler.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		attrCount+=2;
	}
	theme_DLT.general_background_filler=(UI_filled_area*)&general_background_filler;
	
}

/*****************************************************************************
* FUNCTION
*	 infobar_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for infobar
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/


void infobar_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
			if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			information_bar_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
                        information_bar_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			information_bar_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			information_bar_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
			
			if(!strncmp((PS8)attr[attrCount],"ipmethod_txt_col",strlen("ipmethod_txt_col")))
			{input_method_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
				input_method_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
				input_method_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
				input_method_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);

			}
			
			if(!strncmp((PS8)attr[attrCount],"remaining_len_txt_col",strlen("remaining_len_txt_col")))
			{        remaining_length_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
				remaining_length_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
				remaining_length_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
				remaining_length_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
			}
					
					
		attrCount+=2;
	}
	theme_DLT.information_bar_color=(color_t*)&information_bar_color;
	theme_DLT.input_method_text_color=(color_t*)&input_method_text_color;
	theme_DLT.remaining_length_text_color=(color_t*)&remaining_length_text_color;
}

/*****************************************************************************
* FUNCTION
*	 list_menu_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for list_menu
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

void list_menu_tag_handler(U8** attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			list_background_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            list_background_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			list_background_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			list_background_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"bk_img_offset",strlen("bk_img_offset")))
		{
			list_background_filler.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));

		}
		if(!strncmp((PS8)attr[attrCount],"normal_item_col",strlen("normal_item_col")))
		{
			
			list_normal_item_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
			list_normal_item_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			list_normal_item_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			list_normal_item_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"normal_item_img_offset",strlen("normal_item_img_offset")))
		{
		list_normal_item_filler.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		if(!strncmp((PS8)attr[attrCount],"normal_txt_col",strlen("normal_txt_col")))
		{
			list_normal_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			list_normal_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			list_normal_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			list_normal_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
			
		}
		if(!strncmp((PS8)attr[attrCount],"selected_item_col",strlen("selected_item_col")))
		{
			list_selected_item_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
			list_selected_item_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			list_selected_item_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			list_selected_item_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"selected_item_img_offset",strlen("selected_item_img_offset")))
		{
			list_selected_item_filler.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		if(!strncmp((PS8)attr[attrCount],"selected_txt_col",strlen("selected_txt_col")))
		{
			list_selected_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			list_selected_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			list_selected_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			list_selected_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"list_font",strlen("list_font")))
		{
				list_menu_font.bold=extract_no_of_bits(attr[attrCount+1],0,4);
				list_menu_font.italic=extract_no_of_bits(attr[attrCount+1],4,4);
				list_menu_font.underline=extract_no_of_bits(attr[attrCount+1],8,4);
				list_menu_font.size=extract_no_of_bits(attr[attrCount+1],12,4);
				list_menu_font.color_t=extract_no_of_bits(attr[attrCount+1],16,4);
				list_menu_font.type=extract_no_of_bits(attr[attrCount+1],20,4);
				list_menu_font.oblique=extract_no_of_bits(attr[attrCount+1],24,4);
				list_menu_font.smallCaps=extract_no_of_bits(attr[attrCount+1],28,4);
	
		}
		attrCount+=2;
	}
	theme_DLT.list_menu_font=(stFontAttribute*)&list_menu_font;
	theme_DLT.list_selected_text_color=(color_t*)&list_selected_text_color;
	theme_DLT.list_selected_item_filler=(UI_filled_area*)&list_selected_item_filler;
	theme_DLT.list_normal_text_color=(color_t*)&list_normal_text_color;
	theme_DLT.list_normal_item_filler=(UI_filled_area*)&list_normal_item_filler;
	theme_DLT.list_background_filler=(UI_filled_area*)&list_background_filler;


}

/*****************************************************************************
* FUNCTION
*	 matrix_menu_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for matrix_menu
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/


void matrix_menu_tag_handler(U8** attr)
{
	
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			matrix_background_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            matrix_background_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			matrix_background_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			matrix_background_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"bk_img_offset",strlen("bk_img_offset")))
		{
		matrix_background_filler.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));

		}
		if(!strncmp((PS8)attr[attrCount],"flag",strlen("flag")))
		{
			matrix_background_filler.flags=atoi((PS8)attr[attrCount+1]);
		}
		if(!strncmp((PS8)attr[attrCount],"normal_item_col",strlen("normal_item_col")))
		{
			
			matrix_normal_item_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
			matrix_normal_item_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			matrix_normal_item_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			matrix_normal_item_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"normal_item_img_offset",strlen("normal_item_img_offset")))
		{
			matrix_normal_item_filler.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		if(!strncmp((PS8)attr[attrCount],"normal_txt_col",strlen("normal_txt_col")))
		{
			matrix_normal_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			matrix_normal_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			matrix_normal_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			matrix_normal_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
			
		}
		if(!strncmp((PS8)attr[attrCount],"selected_item_col",strlen("selected_item_col")))
		{
			matrix_selected_item_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
			matrix_selected_item_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			matrix_selected_item_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			matrix_selected_item_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"selected_item_img_offset",strlen("selected_item_img_offset")))
		{
			matrix_selected_item_filler.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		if(!strncmp((PS8)attr[attrCount],"selected_txt_col",strlen("selected_txt_col")))
		{
			matrix_selected_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			matrix_selected_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			matrix_selected_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			matrix_selected_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"matrix_font",strlen("matrix_font")))
		{
				matrix_menu_font.bold=extract_no_of_bits(attr[attrCount+1],0,4);
				matrix_menu_font.italic=extract_no_of_bits(attr[attrCount+1],4,4);
				matrix_menu_font.underline=extract_no_of_bits(attr[attrCount+1],8,4);
				matrix_menu_font.size=extract_no_of_bits(attr[attrCount+1],12,4);
				matrix_menu_font.color_t=extract_no_of_bits(attr[attrCount+1],16,4);
				matrix_menu_font.type=extract_no_of_bits(attr[attrCount+1],20,4);
				matrix_menu_font.oblique=extract_no_of_bits(attr[attrCount+1],24,4);
				matrix_menu_font.smallCaps=extract_no_of_bits(attr[attrCount+1],28,4);
		}
		attrCount+=2;
	}
	theme_DLT.matrix_menu_font=(stFontAttribute*)&matrix_menu_font;
	theme_DLT.matrix_selected_text_color=(color_t*)&matrix_selected_text_color;
	theme_DLT.matrix_selected_item_filler=(UI_filled_area*)&matrix_selected_item_filler;
	theme_DLT.matrix_normal_text_color=(color_t*)&matrix_normal_text_color;
	theme_DLT.matrix_normal_item_filler=(UI_filled_area*)&matrix_normal_item_filler;
	theme_DLT.matrix_background_filler=(UI_filled_area*)&matrix_background_filler;

}

/*****************************************************************************
* FUNCTION
*	 menubar_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for menubar
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

void menubar_tag_handler(U8** attr)
{
		U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"index_bk_col",strlen("index_bk_col")))
		{
			menubar_index_background_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            menubar_index_background_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			menubar_index_background_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			menubar_index_background_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"indexfont",strlen("indexfont")))
		{
				menu_bar_index_font.bold=extract_no_of_bits(attr[attrCount+1],0,4);
				menu_bar_index_font.italic=extract_no_of_bits(attr[attrCount+1],4,4);
				menu_bar_index_font.underline=extract_no_of_bits(attr[attrCount+1],8,4);
				menu_bar_index_font.size=extract_no_of_bits(attr[attrCount+1],12,4);
				menu_bar_index_font.color_t=extract_no_of_bits(attr[attrCount+1],16,4);
				menu_bar_index_font.type=extract_no_of_bits(attr[attrCount+1],20,4);
				menu_bar_index_font.oblique=extract_no_of_bits(attr[attrCount+1],24,4);
				menu_bar_index_font.smallCaps=extract_no_of_bits(attr[attrCount+1],28,4);
			
		}
		if(!strncmp((PS8)attr[attrCount],"item_bk_col",strlen("item_bk_col")))
		{
			
			menubar_item_background_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			menubar_item_background_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			menubar_item_background_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			menubar_item_background_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"itemfont",strlen("itemfont")))
		{
			
				menu_bar_item_font.bold=extract_no_of_bits(attr[attrCount+1],0,4);
				menu_bar_item_font.italic=extract_no_of_bits(attr[attrCount+1],4,4);
				menu_bar_item_font.underline=extract_no_of_bits(attr[attrCount+1],8,4);
				menu_bar_item_font.size=extract_no_of_bits(attr[attrCount+1],12,4);
				menu_bar_item_font.color_t=extract_no_of_bits(attr[attrCount+1],16,4);
				menu_bar_item_font.type=extract_no_of_bits(attr[attrCount+1],20,4);
				menu_bar_item_font.oblique=extract_no_of_bits(attr[attrCount+1],24,4);
				menu_bar_item_font.smallCaps=extract_no_of_bits(attr[attrCount+1],28,4);
			
		}
		if(!strncmp((PS8)attr[attrCount],"index_txt_col",strlen("index_txt_col")))
		{
			menubar_index_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			menubar_index_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			menubar_index_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			menubar_index_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
			
		}
		if(!strncmp((PS8)attr[attrCount],"item_txt_col",strlen("item_txt_col")))
		{
			menubar_item_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
			menubar_item_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			menubar_item_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			menubar_item_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
	
		attrCount+=2;
	}
	theme_DLT.menubar_item_text_color=(color_t*)&menubar_item_text_color;
	theme_DLT.menubar_index_text_color=(color_t*)&menubar_index_text_color;
	theme_DLT.menu_bar_item_font=(stFontAttribute*)&menu_bar_item_font;
	theme_DLT.menubar_item_background_color=(color_t*)&menubar_item_background_color;
	theme_DLT.menu_bar_index_font=(stFontAttribute*)&menu_bar_index_font;
	theme_DLT.menubar_index_background_color=(color_t*)&menubar_index_background_color;
}

/*****************************************************************************
* FUNCTION
*	 statusbar_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for statusbar
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

void statusbar_tag_handler(U8** attr)
{
	
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			statusbar0_background_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            statusbar0_background_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			statusbar0_background_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			statusbar0_background_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		attrCount+=2;
	}
	theme_DLT.statusbar0_background_color=(color_t*)&statusbar0_background_color;
}

/*****************************************************************************
* FUNCTION
*	 shortcut_ind_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for shortcut_ind
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

void shortcut_ind_tag_handler(U8** attr)
{
	
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			shortcut_indicator_background_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            shortcut_indicator_background_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			shortcut_indicator_background_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			shortcut_indicator_background_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"txt_col",strlen("txt_col")))
		{
			shortcut_indicator_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            shortcut_indicator_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			shortcut_indicator_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			shortcut_indicator_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"cursor_col",strlen("cursor_col")))
		{
			shortcut_indicator_cursor_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            shortcut_indicator_cursor_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			shortcut_indicator_cursor_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			shortcut_indicator_cursor_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"font",strlen("font")))
		{
				shortcut_indicator_font.bold		=extract_no_of_bits(attr[attrCount+1],0,4);
				shortcut_indicator_font.italic		=extract_no_of_bits(attr[attrCount+1],4,4);
				shortcut_indicator_font.underline	=extract_no_of_bits(attr[attrCount+1],8,4);
				shortcut_indicator_font.size		=extract_no_of_bits(attr[attrCount+1],12,4);
				shortcut_indicator_font.color_t		=extract_no_of_bits(attr[attrCount+1],16,4);
				shortcut_indicator_font.type		=extract_no_of_bits(attr[attrCount+1],20,4);
				shortcut_indicator_font.oblique	=extract_no_of_bits(attr[attrCount+1],24,4);
				shortcut_indicator_font.smallCaps	=extract_no_of_bits(attr[attrCount+1],28,4);
		}
		attrCount+=2;
	}
	theme_DLT.shortcut_indicator_font=(stFontAttribute*)&shortcut_indicator_font;
	theme_DLT.shortcut_indicator_cursor_color=(color_t*)&shortcut_indicator_cursor_color;
	theme_DLT.shortcut_indicator_text_color=(color_t*)&shortcut_indicator_text_color;
	theme_DLT.shortcut_indicator_background_filler=(UI_filled_area*)&shortcut_indicator_background_filler;

}

/*****************************************************************************
* FUNCTION
*	 ip_box_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for ip_box
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

void ip_box_tag_handler(U8** attr)
{
		U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			inputbox_filler_DLT.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            inputbox_filler_DLT.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inputbox_filler_DLT.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inputbox_filler_DLT.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"flag",strlen("flag")))
		{
			inputbox_filler_DLT.flags=atoi((PS8)attr[attrCount+1]);
		}
		if(!strncmp((PS8)attr[attrCount],"txt_col",strlen("txt_col")))
		{
			inputbox_normal_text_color_DLT.r=extract_no_of_bits(attr[attrCount+1],0,8);
			inputbox_normal_text_color_DLT.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inputbox_normal_text_color_DLT.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inputbox_normal_text_color_DLT.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
			if(!strncmp((PS8)attr[attrCount],"selection_col",strlen("selection_col")))
		{
			inputbox_selector_color_DLT.r=extract_no_of_bits(attr[attrCount+1],0,8);
			inputbox_selector_color_DLT.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inputbox_selector_color_DLT.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inputbox_selector_color_DLT.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
}
		if(!strncmp((PS8)attr[attrCount],"sel_txt_col",strlen("sel_txt_col")))
	
		{
			inputbox_selected_text_color_DLT.r=extract_no_of_bits(attr[attrCount+1],0,8);
			inputbox_selected_text_color_DLT.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inputbox_selected_text_color_DLT.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inputbox_selected_text_color_DLT.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"cursor_col",strlen("cursor_col")))
		{
			inputbox_cursor_color_DLT.r=extract_no_of_bits(attr[attrCount+1],0,8);
			inputbox_cursor_color_DLT.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inputbox_cursor_color_DLT.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inputbox_cursor_color_DLT.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"img_offset",strlen("img_offset")))
		{
			inputbox_filler_DLT.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		if(!strncmp((PS8)attr[attrCount],"font",strlen("font")))
		{
				inputbox_font.bold=extract_no_of_bits(attr[attrCount+1],0,4);
				inputbox_font.italic=extract_no_of_bits(attr[attrCount+1],4,4);
				inputbox_font.underline=extract_no_of_bits(attr[attrCount+1],8,4);
				inputbox_font.size=extract_no_of_bits(attr[attrCount+1],12,4);
				inputbox_font.color_t=extract_no_of_bits(attr[attrCount+1],16,4);
				inputbox_font.type=extract_no_of_bits(attr[attrCount+1],20,4);
				inputbox_font.oblique=extract_no_of_bits(attr[attrCount+1],24,4);
				inputbox_font.smallCaps=extract_no_of_bits(attr[attrCount+1],28,4);
			}
		attrCount+=2;

	}
	theme_DLT.inputbox_font=(stFontAttribute*)&inputbox_font;
	theme_DLT.inputbox_filler=(UI_filled_area*)&inputbox_filler_DLT;
	theme_DLT.inputbox_cursor_color=(color_t*)&inputbox_cursor_color_DLT;
	theme_DLT.inputbox_selected_text_color=(color_t*)&inputbox_selected_text_color_DLT;
	theme_DLT.inputbox_selector_color=(color_t*)&inputbox_selector_color_DLT;
	theme_DLT.inputbox_normal_text_color=(color_t*)&inputbox_normal_text_color_DLT;


}

/*****************************************************************************
* FUNCTION
*	 pin_ip_box_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for pin_ip_box
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

void pin_ip_box_tag_handler(U8** attr)
{ 
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			
			PIN_inputbox_background_filler_DLT.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            PIN_inputbox_background_filler_DLT.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			PIN_inputbox_background_filler_DLT.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			PIN_inputbox_background_filler_DLT.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		
		if(!strncmp((PS8)attr[attrCount],"border_col",strlen("border_col")))
		{
			PIN_inputbox_background_filler_DLT.border_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            PIN_inputbox_background_filler_DLT.border_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			PIN_inputbox_background_filler_DLT.border_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			PIN_inputbox_background_filler_DLT.border_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
	
		}
		if(!strncmp((PS8)attr[attrCount],"cursor_col",strlen("cursor_col")))
		{
			
			PIN_inputbox_cursor_color_DLT.r=extract_no_of_bits(attr[attrCount+1],0,8);
			PIN_inputbox_cursor_color_DLT.g=extract_no_of_bits(attr[attrCount+1],8,8);
			PIN_inputbox_cursor_color_DLT.b=extract_no_of_bits(attr[attrCount+1],16,8);
			PIN_inputbox_cursor_color_DLT.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"txt_col",strlen("txt_col")))
		{
			PIN_inputbox_normal_text_color_DLT.r=extract_no_of_bits(attr[attrCount+1],0,8);
			PIN_inputbox_normal_text_color_DLT.g=extract_no_of_bits(attr[attrCount+1],8,8);
			PIN_inputbox_normal_text_color_DLT.b=extract_no_of_bits(attr[attrCount+1],16,8);
			PIN_inputbox_normal_text_color_DLT.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"selection_col",strlen("selection_col")))
		{
			
			PIN_inputbox_selector_color_DLT.r=extract_no_of_bits(attr[attrCount+1],0,8);
			PIN_inputbox_selector_color_DLT.g=extract_no_of_bits(attr[attrCount+1],8,8);
			PIN_inputbox_selector_color_DLT.b=extract_no_of_bits(attr[attrCount+1],16,8);
			PIN_inputbox_selector_color_DLT.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"sel_txt_col",strlen("sel_txt_col")))
		
		{
			PIN_inputbox_selected_text_color_DLT.r=extract_no_of_bits(attr[attrCount+1],0,8);
			PIN_inputbox_selected_text_color_DLT.g=extract_no_of_bits(attr[attrCount+1],8,8);
			PIN_inputbox_selected_text_color_DLT.b=extract_no_of_bits(attr[attrCount+1],16,8);
			PIN_inputbox_selected_text_color_DLT.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"img_offset",strlen("img_offset")))
		{
			PIN_inputbox_background_filler_DLT.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		if(!strncmp((PS8)attr[attrCount],"flag",strlen("flag")))
		{
			PIN_inputbox_background_filler_DLT.flags=(U32)atoi((PS8)attr[attrCount+1]);
		}

			attrCount+=2;
			
	}
	theme_DLT.PIN_inputbox_background_filler=(UI_filled_area*)&PIN_inputbox_background_filler_DLT;
	theme_DLT.PIN_inputbox_selected_text_color=(color_t*)&PIN_inputbox_selected_text_color_DLT;
	theme_DLT.PIN_inputbox_selector_color=(color_t*)&PIN_inputbox_selector_color_DLT;
	theme_DLT.PIN_inputbox_normal_text_color=(color_t*)&PIN_inputbox_normal_text_color_DLT;
	theme_DLT.PIN_inputbox_cursor_color=(color_t*)&PIN_inputbox_cursor_color_DLT;
	
}
extern U16 mmi_check_theme_file_version_compatibility(float32 theme_version);

/*****************************************************************************
* FUNCTION
*	 device_theme_ver_info(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for device_theme
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

void device_theme_ver_info(U8** attr)
{ 
	U8 attrCount=0;
		while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"name",strlen("name")))
		{
			gthemeInfo.name=attr[attrCount+1];
		}
		if(!strncmp((PS8)attr[attrCount],"version",strlen("version")))
		{
			gthemeInfo.version=(float32)atoi((PS8)attr[attrCount+1]);
		}
		attrCount+=2;
	}

	if(gcheckThemeVersion)
		xml_stop_parse(&parser );
	
	}

/*****************************************************************************
* FUNCTION
*	 inline_list_menu_tag_handler(U8** attr)
* DESCRIPTION
*  fill the MMI theme structure for inline_list_menu
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/


void inline_list_menu_tag_handler(U8 **attr)
{
	U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			inline_list_background_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            inline_list_background_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inline_list_background_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inline_list_background_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"bk_imgoffset",strlen("bk_imgoffset")))
		{
			inline_list_background_filler.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));

		}
		if(!strncmp((PS8)attr[attrCount],"normal_item_col",strlen("normal_item_col")))
		{
			inline_list_normal_item_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            inline_list_normal_item_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inline_list_normal_item_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inline_list_normal_item_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"normal_item_imgoffset",strlen("normal_item_imgoffset")))
		{
			inline_list_normal_item_filler.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		if(!strncmp((PS8)attr[attrCount],"normal_txt_col",strlen("normal_txt_col")))
		{
			inline_list_normal_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            inline_list_normal_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inline_list_normal_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inline_list_normal_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"selected_item_col",strlen("selected_item_col")))
		{
			inline_list_selected_item_filler.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            inline_list_selected_item_filler.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inline_list_selected_item_filler.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inline_list_selected_item_filler.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"selected_item_imgoffset",strlen("selected_item_imgoffset")))
		{
			inline_list_selected_item_filler.b=tm_create_non_imageid_image((U32)atoi((PS8)attr[attrCount+1]));
		}
		if(!strncmp((PS8)attr[attrCount],"selected_txt_col",strlen("selected_txt_col")))
		{
			inline_list_selected_text_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            inline_list_selected_text_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			inline_list_selected_text_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			inline_list_selected_text_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
	
		
		attrCount+=2;
	}
	theme_DLT.inline_list_selected_text_color=(color_t*)&inline_list_selected_text_color;
	theme_DLT.inline_list_selected_item_filler=(UI_filled_area*)&inline_list_selected_item_filler;
	theme_DLT.inline_list_normal_text_color=(color_t*)&inline_list_normal_text_color;
	theme_DLT.inline_list_normal_item_filler=(UI_filled_area*)&inline_list_normal_item_filler;
	theme_DLT.inline_list_background_filler=(UI_filled_area*)&inline_list_background_filler;
}

/*****************************************************************************
* FUNCTION
*	 font_col_tag_handler(U8** attr)
* DESCRIPTION
*  Handles the font color list
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

//Added for font color changes
void font_col_tag_handler(U8** attr)
{
		U8 attrCount=0;
	while(attr[attrCount])
	{
		if(!strncmp((PS8)attr[attrCount],"calc",strlen("calc")))
		{
			calculator_theme_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            calculator_theme_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			calculator_theme_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			calculator_theme_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"idle_scr_date_txt",strlen("idle_scr_date_txt")))
		{
			idle_scr_date_theme_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            idle_scr_date_theme_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			idle_scr_date_theme_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			idle_scr_date_theme_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"idle_scr_time_txt",strlen("idle_scr_time_txt")))
		{
			idle_scr_time_theme_color.r=extract_no_of_bits(attr[attrCount+1],0,8);
            idle_scr_time_theme_color.g=extract_no_of_bits(attr[attrCount+1],8,8);
			idle_scr_time_theme_color.b=extract_no_of_bits(attr[attrCount+1],16,8);
			idle_scr_time_theme_color.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		if(!strncmp((PS8)attr[attrCount],"bk_col",strlen("bk_col")))
		{
			alarm_background_filler_DLT.c.r=extract_no_of_bits(attr[attrCount+1],0,8);
            alarm_background_filler_DLT.c.g=extract_no_of_bits(attr[attrCount+1],8,8);
			alarm_background_filler_DLT.c.b=extract_no_of_bits(attr[attrCount+1],16,8);
			alarm_background_filler_DLT.c.alpha=extract_no_of_bits(attr[attrCount+1],24,8);
		}
		attrCount+=2;
	}
	theme_DLT.calculator_color=(color_t*)&calculator_theme_color;
	theme_DLT.alarm_background_filler=(UI_filled_area*)&alarm_background_filler_DLT;
	theme_DLT.idle_scr_time_color=(color_t*)&idle_scr_time_theme_color;
	theme_DLT.idle_scr_date_color=(color_t*)&idle_scr_date_theme_color;
}






/*****************************************************************************
* FUNCTION
*	 convert_theme_file_to_mmi_theme(MMI_theme** theme, U32 file_handle)
* DESCRIPTION
*  fills the MMI theme structure by parsing the downloaded theme file 
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/

extern MMI_theme theme_Default;

BOOL convert_theme_file_to_mmi_theme(MMI_theme** theme, U32 file_handle)
{
	memcpy(g_pConverterTheme,&theme_Default,sizeof(MMI_theme));
	



	
	if(xml_new_parser(&parser) == -1)
		return FALSE;
	xml_register_element_handler(&parser,my_xml_elem_start_hdlr,my_xml_elem_end_hdlr);
	
	xml_parse_file_from_offset(&parser, file_handle);
	xml_close_parser_offset(&parser);
	
	*theme = &theme_DLT;
	return TRUE;
}


#endif//MMI theme downloadable support
