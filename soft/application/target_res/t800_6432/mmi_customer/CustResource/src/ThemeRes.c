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
#include "mmi_data_types.h"
#include "mmi_features.h"
#include "gui_themes.h"

#include "themecomponents.h"
#if defined(__MMI_RESOURCE_SLIM_ON_128X160__)
#define MAX_MMI_THEMES 1
#else
#define MAX_MMI_THEMES 3
#endif
CONST_HARDWARE U8 IMG_TITLE_1[] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
CONST_HARDWARE U8 IMG_TITLE_2[]=
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
CONST_HARDWARE U8 IMG_TITLE_3[] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

S32 CSD_n_MMI_themes=MAX_MMI_THEMES;

const MMI_theme theme_1={
(UI_filled_area*)&LSK_up_1,
(UI_filled_area*)&LSK_down_1,
(UI_filled_area*)&Disabled_items,
(UI_filled_area*)&RSK_up_1,
(UI_filled_area*)&RSK_down_1,//5
(UI_filled_area*)&Disabled_items,
(UI_filled_area*)&mtab_1,
(UI_filled_area*)&ibox_1,
(UI_filled_area*)&sbar_bkgrnd_1,
(UI_filled_area*)&sbar_indicator_1,//10
(UI_filled_area*)&Title_1,
(UI_filled_area*)&mm_bkgrnd_1,
(UI_filled_area*)&list_normal_item_1,
(UI_filled_area*)&list_selected_item_1,
(UI_filled_area*)&Disabled_items,//15
(UI_filled_area*)&matrix_bkgrnd_1,
(UI_filled_area*)&matrix_normal_item_1,
(UI_filled_area*)&matrix_selected_item_1,
(UI_filled_area*)&Disabled_items,
(UI_filled_area*)&General_BG,//20
(UI_filled_area*)&fibox_normal_1,
(UI_filled_area*)&fibox_selected_1,
#ifdef __T800_MAIN_MENU__
(UI_filled_area*)&mm_bkgrnd_t800,
#else
(UI_filled_area*)&mm_bkgrnd_1,
#endif
(UI_filled_area*)&General_BG,
(UI_filled_area*)&popup_description_bkgrnd_1,//25

(UI_filled_area*)&list_bkgrnd_1,
(UI_filled_area*)&list_normal_item_1,
(UI_filled_area*)&list_selected_item_1,
(UI_filled_area*)&Disabled_items,
(UI_filled_area*)&list_selected_item_1,//30
#ifdef __MMI_STYLE_E71_QCIF_S__
(UI_filled_area*)&list_bkgrnd_1,
(UI_filled_area*)&list_bkgrnd_1,
#else
(UI_filled_area*)&General_BG,
(UI_filled_area*)&Unused,
#endif

(UI_filled_area*)&list_selected_item_1,
(UI_filled_area*)&common_screen_BG,
(UI_filled_area*)&common_screen_BG,//35
(UI_filled_area*)&common_screen_BG,
(UI_filled_area*)&common_screen_BG,
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(UI_filled_area*)&common_screen_BG,
#endif
(UI_filled_area*)&dial_inputbox_bg_filler,
(UI_filled_area*)&virtual_keyboard_down_bkgrnd_1,//40
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(UI_filled_area*)&alarm_background_filler1,
#endif
#ifdef UI_SMALL_LIST_SCREEN
(UI_filled_area*)&small_list_menu_normal_filler1,
(UI_filled_area*)&small_list_screen_border_filler1,
#endif
(color_t*)&black_color,
(color_t*)&white_color,//45
(color_t*)&black_color,
(color_t*)&LSK_up_txt_col_1,
(color_t*)&LSK_down_txt_col_1,
(color_t*)&Disabled_text_col,
(color_t*)&RSK_up_txt_col_1,//50
(color_t*)&RSK_down_txt_col_1,
(color_t*)&Disabled_text_col,
(color_t*)&mtab_selector_txt_col_1,
(color_t*)&mtab_normal_txt_col_1,
(color_t*)&mtab_selected_txt_col_1,//55
(color_t*)&ibox_normal_txt_col_1,
(color_t*)&ibox_selected_txt_col_1,
(color_t*)&ibox_selector_col_1,
(color_t*)&ibox_cursor_col_1,
(color_t*)&Title_text_color_1,//60
(color_t*)&Title_text_border_col,
(color_t*)&title_scut_txt_col_1,
(color_t*)&list_normal_txt_col_1,
(color_t*)&list_selected_txt_col_1,
(color_t*)&Disabled_text_col,//65
(color_t*)&list_normal_txt_col_1,
(color_t*)&list_selected_txt_col_1,
(color_t*)&Disabled_text_col,
(color_t*)&Unused_color,
(color_t*)&Unused_color,//70
(color_t*)&Unused_color,
(color_t*)&Unused_color,
(color_t*)&scut_indicator_txt_col_1,
(color_t*)&white_color,
(color_t*)&Unused_color,//75
(color_t*)&Unused_color,
(color_t*)&Unused_color,
(color_t*)&ibox_selector_col_1,
(color_t*)&ibox_normal_txt_col_1,
(color_t*)&Disabled_text_col,//80
(color_t*)&ibox_selected_txt_col_1,
(color_t*)&ibox_cursor_col_1,
(color_t*)&list_normal_txt_col_1,
(color_t*)&list_selected_txt_col_1,
(color_t*)&Disabled_text_col,//85
(color_t*)&list_normal_txt_col_1,
(color_t*)&list_selected_txt_col_1,
(color_t*)&Disabled_text_col,
(color_t*)&list_normal_txt_col_1,
(color_t*)&list_selected_txt_col_1,//90
(color_t*)&Disabled_text_col,
#ifdef __MMI_STYLE_E71_QCIF_S__
(color_t*)&black_color,
(color_t*)&white_color,
#else
#ifdef __T800_MAIN_MENU__
(color_t*)&middle_color,
(color_t*)&black_color,
#else
(color_t*)&white_color,
(color_t*)&black_color,	
#endif
#endif
(color_t*)&popup_description_txt_col_1,
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(color_t*)&CM_screen_text_color_1,//95
#endif
#ifdef __MMI_STYLE_E71_QCIF_S__
(color_t*)&black_color,
(color_t*)&black_color,
(color_t*)&white_color,	
(color_t*)&white_color,
(color_t*)&white_color,
(color_t*)&white_color,
#else
(color_t*)&white_color,
(color_t*)&white_color,	
(color_t*)&black_color,
(color_t*)&black_color,
(color_t*)&black_color,
(color_t*)&black_color,
#endif
(color_t*)&button_bar_col_1,	//102
(color_t*)&information_bar_col_1,
(color_t*)&input_method_txt_col_1,
(color_t*)&input_method_txt_col_1,//105
(color_t*)&Status_bar_col,
#if 0
	color_t*				inline_edit_focussed_text_color;
	color_t*				inline_edit_unfocussed_text_color;
	color_t*				inline_edit_selected_text_color;
	color_t*				inline_edit_selector_color;//110
	color_t*				inline_edit_cursor_color;

	color_t*				inline_list_normal_text_color;
	color_t*				inline_list_selected_text_color;
	color_t*				inline_list_disabled_text_color;
#endif

#ifdef __MMI_STYLE_E71_QCIF_S__
(color_t*)&black_color,
(color_t*)&white_color,
(color_t*)&ibox_selected_txt_col_1,
(color_t*)&ibox_selector_col_1,//110
(color_t*)&ibox_cursor_col_1,

(color_t*)&list_normal_txt_col_1,
(color_t*)&list_selected_txt_col_1,
(color_t*)&Disabled_text_col,
#else
(color_t*)&black_color,		
(color_t*)&ibox_normal_txt_col_1,
(color_t*)&ibox_selected_txt_col_1,
(color_t*)&ibox_selector_col_1,//110
(color_t*)&ibox_cursor_col_1,

(color_t*)&list_normal_txt_col_1,
(color_t*)&list_selected_txt_col_1,
(color_t*)&Disabled_text_col,
#endif
(color_t*)&ibox_normal_txt_col_1,//115
(color_t*)&ibox_selected_txt_col_1,
(color_t*)&ibox_selector_col_1,
(color_t*)&ibox_cursor_col_1,
(color_t*)&ibox_normal_txt_col_1,
(color_t*)&ibox_selected_txt_col_1,//120
(color_t*)&ibox_selector_col_1,
(color_t*)&ibox_cursor_col_1,
(color_t*)&virtual_keyboard_key_down_text_col_1,
(color_t*)&virtual_keyboard_key_up_text_col_1,
(color_t*)&virtual_keyboard_dead_key_down_text_col_1,
(color_t*)&virtual_keyboard_dead_key_up_text_col_1,
(color_t*)&virtual_keyboard_disp_area_text_col_1,
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(color_t*)&calculator_color1,
#endif
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(color_t*)&idle_scr_date_color1,
#endif
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(color_t*)&idle_scr_time_color1,
#endif
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(color_t*)&alarm_text_color1,
#endif
&MMI_medium_font,
&MMI_medium_font,
&MMI_medium_font,
&MMI_medium_font,
&MMI_default_font,
&MMI_default_font,
#ifdef __MMI_STYLE_E71_QCIF_S__
&MMI_small_font,					//145
#else
&MMI_default_font,
#endif
&MMI_default_font,
&MMI_default_font,
&MMI_default_font,
&MMI_default_font,
&MMI_default_font,
#ifdef __MMI_STYLE_E71_QCIF_S__
&MMI_small_font,
#else
&MMI_default_font,
#endif
&MMI_virtual_keyboard_font,		//155
UI_SCROLLBAR_WIDTH
};

const MMI_theme theme_2={
(UI_filled_area*)&LSK_up_2,
(UI_filled_area*)&LSK_down_2,
(UI_filled_area*)&Disabled_items,
(UI_filled_area*)&RSK_up_2,
(UI_filled_area*)&RSK_down_2,
(UI_filled_area*)&Disabled_items,
(UI_filled_area*)&mtab_2,
(UI_filled_area*)&ibox_2,
(UI_filled_area*)&sbar_bkgrnd_2,
(UI_filled_area*)&sbar_indicator_2,
(UI_filled_area*)&Title_2,
(UI_filled_area*)&list_bkgrnd_2,
(UI_filled_area*)&list_normal_item_2,
(UI_filled_area*)&list_selected_item_2,
(UI_filled_area*)&Disabled_items,
(UI_filled_area*)&matrix_bkgrnd_2,
(UI_filled_area*)&matrix_normal_item_2,
(UI_filled_area*)&matrix_selected_item_2,
(UI_filled_area*)&Disabled_items,
(UI_filled_area*)&General_BG,
(UI_filled_area*)&fibox_normal_2,
(UI_filled_area*)&fibox_selected_2,
#ifdef __T800_MAIN_MENU__
(UI_filled_area*)&mm_bkgrnd_t800,
#else
(UI_filled_area*)&mm_bkgrnd_2,
#endif
(UI_filled_area*)&General_BG,
(UI_filled_area*)&popup_description_bkgrnd_2,
(UI_filled_area*)&list_bkgrnd_2,
(UI_filled_area*)&list_normal_item_2,
(UI_filled_area*)&list_selected_item_2,
(UI_filled_area*)&Disabled_items,
(UI_filled_area*)&list_selected_item_2,
(UI_filled_area*)&General_BG,
(UI_filled_area*)&Unused,
(UI_filled_area*)&list_selected_item_2,
(UI_filled_area*)&common_screen_BG,
(UI_filled_area*)&common_screen_BG,
(UI_filled_area*)&common_screen_BG,
(UI_filled_area*)&common_screen_BG,
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(UI_filled_area*)&common_screen_BG,
#endif
(UI_filled_area*)&dial_inputbox_bg_filler,
(UI_filled_area*)&virtual_keyboard_down_bkgrnd_1,
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(UI_filled_area*)&alarm_background_filler2,
#endif
#ifdef UI_SMALL_LIST_SCREEN
(UI_filled_area*)&small_list_menu_normal_filler2,
(UI_filled_area*)&small_list_screen_border_filler2,
#endif
(color_t*)&black_color,
(color_t*)&white_color,
(color_t*)&white_color,
(color_t*)&LSK_up_txt_col_2,
(color_t*)&LSK_down_txt_col_2,
(color_t*)&Disabled_text_col,
(color_t*)&RSK_up_txt_col_2,
(color_t*)&RSK_down_txt_col_2,
(color_t*)&Disabled_text_col,
(color_t*)&mtab_selector_txt_col_2,
(color_t*)&mtab_normal_txt_col_2,
(color_t*)&mtab_selected_txt_col_2,
(color_t*)&ibox_normal_txt_col_2,
(color_t*)&ibox_selected_txt_col_2,
(color_t*)&ibox_selector_col_2,
(color_t*)&ibox_cursor_col_2,
(color_t*)&title_scut_txt_col_2,
(color_t*)&white_color,
(color_t*)&title_scut_txt_col_2,
(color_t*)&list_normal_txt_col_2,
(color_t*)&list_selected_txt_col_2,
(color_t*)&Disabled_text_col,
(color_t*)&list_normal_txt_col_2,
(color_t*)&list_selected_txt_col_2,
(color_t*)&Disabled_text_col,
(color_t*)&Unused_color,
(color_t*)&Unused_color,
(color_t*)&Unused_color,
(color_t*)&Unused_color,
(color_t*)&scut_indicator_txt_col_2,
(color_t*)&white_color,
(color_t*)&Unused_color,
(color_t*)&Unused_color,
(color_t*)&Unused_color,
(color_t*)&ibox_selector_col_2,
(color_t*)&ibox_normal_txt_col_2,
(color_t*)&Disabled_text_col,
(color_t*)&ibox_selected_txt_col_2,
(color_t*)&ibox_cursor_col_2,
(color_t*)&list_normal_txt_col_2,
(color_t*)&list_selected_txt_col_2,
(color_t*)&Disabled_text_col,
(color_t*)&list_normal_txt_col_2,
(color_t*)&list_selected_txt_col_2,
(color_t*)&Disabled_text_col,
(color_t*)&list_normal_txt_col_2,
(color_t*)&list_selected_txt_col_2,
(color_t*)&Disabled_text_col,
(color_t*)&white_color,
(color_t*)&black_color,
(color_t*)&popup_description_txt_col_2,
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(color_t*)&CM_screen_text_color_2,
#endif
(color_t*)&white_color,
(color_t*)&white_color,
(color_t*)&black_color,
(color_t*)&black_color,
(color_t*)&black_color,
(color_t*)&black_color,
(color_t*)&button_bar_col_2,
(color_t*)&information_bar_col_2,
(color_t*)&input_method_txt_col_2,
(color_t*)&input_method_txt_col_2,
(color_t*)&Status_bar_col,
(color_t*)&black_color,
(color_t*)&ibox_normal_txt_col_2,
(color_t*)&ibox_selected_txt_col_2,
(color_t*)&ibox_selector_col_2,
(color_t*)&ibox_cursor_col_2,
(color_t*)&list_normal_txt_col_2,
(color_t*)&list_selected_txt_col_2,
(color_t*)&Disabled_text_col,
(color_t*)&ibox_normal_txt_col_2,
(color_t*)&ibox_selected_txt_col_2,
(color_t*)&ibox_selector_col_2,
(color_t*)&ibox_cursor_col_2,
(color_t*)&ibox_normal_txt_col_2,
(color_t*)&ibox_selected_txt_col_2,
(color_t*)&ibox_selector_col_2,
(color_t*)&ibox_cursor_col_2,
(color_t*)&virtual_keyboard_key_down_text_col_1,
(color_t*)&virtual_keyboard_key_up_text_col_1,
(color_t*)&virtual_keyboard_dead_key_down_text_col_1,
(color_t*)&virtual_keyboard_dead_key_up_text_col_1,
(color_t*)&virtual_keyboard_disp_area_text_col_1,
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(color_t*)&calculator_color2,
#endif
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(color_t*)&idle_scr_date_color2,
#endif
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(color_t*)&idle_scr_time_color2,
#endif
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(color_t*)&alarm_text_color2,
#endif
&MMI_medium_font,
&MMI_medium_font,
&MMI_medium_font,
&MMI_medium_font,
&MMI_default_font,
&MMI_default_font,
&MMI_default_font,
&MMI_default_font,
&MMI_default_font,
&MMI_default_font,
&MMI_default_font,
&MMI_default_font,
&MMI_default_font,
&MMI_virtual_keyboard_font,
UI_SCROLLBAR_WIDTH
};

const MMI_theme theme_3={
(UI_filled_area*)&LSK_up_3,
(UI_filled_area*)&LSK_down_3,
(UI_filled_area*)&Disabled_items,
(UI_filled_area*)&RSK_up_3,
(UI_filled_area*)&RSK_down_3,
(UI_filled_area*)&Disabled_items,
(UI_filled_area*)&mtab_3,
(UI_filled_area*)&ibox_3,
(UI_filled_area*)&sbar_bkgrnd_3,
(UI_filled_area*)&sbar_indicator_3,
(UI_filled_area*)&Title_3,
(UI_filled_area*)&list_bkgrnd_3,
(UI_filled_area*)&list_normal_item_3,
(UI_filled_area*)&list_selected_item_3,
(UI_filled_area*)&Disabled_items,
(UI_filled_area*)&matrix_bkgrnd_3,
(UI_filled_area*)&matrix_normal_item_3,
(UI_filled_area*)&matrix_selected_item_3,
(UI_filled_area*)&Disabled_items,
(UI_filled_area*)&General_BG,
(UI_filled_area*)&fibox_normal_3,
(UI_filled_area*)&fibox_selected_3,
#ifdef __T800_MAIN_MENU__
(UI_filled_area*)&mm_bkgrnd_t800,
#else
(UI_filled_area*)&mm_bkgrnd_3,
#endif
(UI_filled_area*)&General_BG,
(UI_filled_area*)&popup_description_bkgrnd_3,
(UI_filled_area*)&list_bkgrnd_3,
(UI_filled_area*)&list_normal_item_3,
(UI_filled_area*)&list_selected_item_3,
(UI_filled_area*)&Disabled_items,
(UI_filled_area*)&list_selected_item_3,
(UI_filled_area*)&General_BG,
(UI_filled_area*)&Unused,
(UI_filled_area*)&list_selected_item_3,
(UI_filled_area*)&common_screen_BG,
(UI_filled_area*)&common_screen_BG,
(UI_filled_area*)&common_screen_BG,
(UI_filled_area*)&common_screen_BG,
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(UI_filled_area*)&common_screen_BG,
#endif
(UI_filled_area*)&dial_inputbox_bg_filler,
(UI_filled_area*)&virtual_keyboard_down_bkgrnd_1,
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(UI_filled_area*)&alarm_background_filler3,
#endif
#ifdef UI_SMALL_LIST_SCREEN
(UI_filled_area*)&small_list_menu_normal_filler3,
(UI_filled_area*)&small_list_screen_border_filler3,
#endif
(color_t*)&black_color,
(color_t*)&white_color,
(color_t*)&white_color,
(color_t*)&LSK_up_txt_col_3,
(color_t*)&LSK_down_txt_col_3,
(color_t*)&Disabled_text_col,
(color_t*)&RSK_up_txt_col_3,
(color_t*)&RSK_down_txt_col_3,
(color_t*)&Disabled_text_col,
(color_t*)&mtab_selector_txt_col_3,
(color_t*)&mtab_normal_txt_col_3,
(color_t*)&mtab_selected_txt_col_3,
(color_t*)&ibox_normal_txt_col_3,
(color_t*)&ibox_selected_txt_col_3,
(color_t*)&ibox_selector_col_3,
(color_t*)&ibox_cursor_col_3,
(color_t*)&title_scut_txt_col_3,
(color_t*)&white_color,
(color_t*)&title_scut_txt_col_3,
(color_t*)&list_normal_txt_col_3,
(color_t*)&list_selected_txt_col_3,
(color_t*)&Disabled_text_col,
(color_t*)&list_normal_txt_col_3,
(color_t*)&list_selected_txt_col_3,
(color_t*)&Disabled_text_col,
(color_t*)&Unused_color,
(color_t*)&Unused_color,
(color_t*)&Unused_color,
(color_t*)&Unused_color,
(color_t*)&scut_indicator_txt_col_3,
(color_t*)&white_color,
(color_t*)&Unused_color,
(color_t*)&Unused_color,
(color_t*)&Unused_color,
(color_t*)&ibox_selector_col_3,
(color_t*)&ibox_normal_txt_col_3,
(color_t*)&Disabled_text_col,
(color_t*)&ibox_selected_txt_col_3,
(color_t*)&ibox_cursor_col_3,
(color_t*)&list_normal_txt_col_3,
(color_t*)&list_selected_txt_col_3,
(color_t*)&Disabled_text_col,
(color_t*)&list_normal_txt_col_3,
(color_t*)&list_selected_txt_col_3,
(color_t*)&Disabled_text_col,
(color_t*)&list_normal_txt_col_3,
(color_t*)&list_selected_txt_col_3,
(color_t*)&Disabled_text_col,
(color_t*)&white_color,
(color_t*)&black_color,
(color_t*)&popup_description_txt_col_3,
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(color_t*)&CM_screen_text_color_3,
#endif
(color_t*)&white_color,
(color_t*)&white_color,
(color_t*)&black_color,
(color_t*)&black_color,
(color_t*)&black_color,
(color_t*)&black_color,
(color_t*)&button_bar_col_3,
(color_t*)&information_bar_col_3,
(color_t*)&input_method_txt_col_3,
(color_t*)&input_method_txt_col_3,
(color_t*)&Status_bar_col,
(color_t*)&black_color,
(color_t*)&ibox_normal_txt_col_3,
(color_t*)&ibox_selected_txt_col_3,
(color_t*)&ibox_selector_col_3,
(color_t*)&ibox_cursor_col_3,
(color_t*)&list_normal_txt_col_3,
(color_t*)&list_selected_txt_col_3,
(color_t*)&Disabled_text_col,
(color_t*)&ibox_normal_txt_col_3,
(color_t*)&ibox_selected_txt_col_3,
(color_t*)&ibox_selector_col_3,
(color_t*)&ibox_cursor_col_3,
(color_t*)&ibox_normal_txt_col_3,
(color_t*)&ibox_selected_txt_col_3,
(color_t*)&ibox_selector_col_3,
(color_t*)&ibox_cursor_col_3,
(color_t*)&virtual_keyboard_key_down_text_col_1,
(color_t*)&virtual_keyboard_key_up_text_col_1,
(color_t*)&virtual_keyboard_dead_key_down_text_col_1,
(color_t*)&virtual_keyboard_dead_key_up_text_col_1,
(color_t*)&virtual_keyboard_disp_area_text_col_1,
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(color_t*)&calculator_color3,
#endif
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(color_t*)&idle_scr_date_color3,
#endif
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(color_t*)&idle_scr_time_color3,
#endif
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
(color_t*)&alarm_text_color3,
#endif
&MMI_medium_font,
&MMI_medium_font,
&MMI_medium_font,
&MMI_medium_font,
&MMI_default_font,
&MMI_default_font,
&MMI_default_font,
&MMI_default_font,
&MMI_default_font,
&MMI_default_font,
&MMI_default_font,
&MMI_default_font,
&MMI_default_font,
&MMI_virtual_keyboard_font,
UI_SCROLLBAR_WIDTH
};

#if defined(__MMI_RESOURCE_SLIM_ON_128X160__)
const MMI_theme *CSD_MMI_themes[MAX_MMI_THEMES]={
(MMI_theme *)&theme_1,
};

#else
const MMI_theme *CSD_MMI_themes[MAX_MMI_THEMES]={
(MMI_theme *)&theme_1,
(MMI_theme *)&theme_2,
(MMI_theme *)&theme_3,
};

#endif

