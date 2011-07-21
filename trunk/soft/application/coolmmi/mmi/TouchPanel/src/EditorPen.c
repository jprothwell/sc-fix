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
 * EditorPen.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Pen Function in Editor
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
#ifndef _EDITOR_PEN_C
#define _EDITOR_PEN_C

/*  Include: MMI header file */
#include "mmi_features.h" 
/*wuzc Add Start For   Ver: CS620  on 2007-4-20 16:10 */
#define __MMI_DRAW_MANAGER__
/*wuzc Add End  For   Ver: CS620  on 2007-4-20 16:10 */
#if defined(__MMI_TOUCH_SCREEN__)||defined(__MMI_HANDWRITING_PAD__)
#include "editorpen.h"
#include "wgui_categories_util.h"
#include "gui_fixed_menus.h"
#include "debuginitdef.h"
#include "gdi_include.h"
#include "lcd_sw_rnd.h"
#include "globaldefs.h"
#include "wgui_softkeys.h"
#include "phonesetupgprots.h"
#include "scrmemmgrgprot.h"
#include "wgui_ems_categories.h"

#include "mmi_trace.h"	//wangzl: for test information
#include "hwr_api.h"
#include "gbhmi.h"
#include "gui_virtual_keyboard.h"
#ifndef _CFW_H_
#include "cfw.h"
#endif

#include "mmiappfnptrs.h"

/***************************************************************************** 
* Define
*****************************************************************************/
//#defined __MMI_HAND_WRITING_GET_STROKES__
#define WGUI_MAX_SOFTKEY_FUNCTIONS 3
/***************************************************************************** 
* Typedef 
*****************************************************************************/

/***************************************************************************** 
* Local Variable
*****************************************************************************/
//The candidate is in selection state or not
static BOOL gPenEditorSelectCandidate = MMI_FALSE;
//Enable Hand Writing or not (needed ?)
static BOOL gPenEditorEnableHandWriting = MMI_FALSE;

static BOOL gPenEditorIsHandWriting = MMI_FALSE;
//The state of candidate above virtual keyboard
static BOOL gPenEditorVKCandidate=MMI_FALSE;
#ifdef __MMI_HAND_WRITING__
//The buffer to store points of stroke
static mmi_pen_point_struct gPenEditorStrokeBuffer[PEN_EDITOR_STROKE_BUFFER_SIZE];
//The point used to draw the stroke
static mmi_pen_point_struct gPenEditorPreviousPoint;
//The color of the stroke
static color_t gPenEditorStrokeColor = {255,0,0,100};  

//Stoke Area for execlude Virtual Keyboard Area
static mmi_pen_handwriting_area_struct g_pen_editor_original_handwriting_area;
//The First Stroke or not
static BOOL gPenEditorFirstStroke = MMI_TRUE;
#endif
//The single line input box for the candidates
static single_line_input_box gPenEditorCandidateInputBox;
//The text buffer for the candidates
static UI_character_type gPenEditorCandidateBuffer[PEN_EDITOR_MAX_CANDIDATE_SIZE];
//Which kind of input box

/*shaozhw 20060610 modify begin: for support sm_pinyin and sm_stroke */
#ifdef __MMI_IME_SUPORT_SM_PINYIN_STROKE__
mmi_pen_editor_input_box_type_enum gPenEditorInputBoxType = PEN_EDITOR_NONE_INPUT_BOX;
void RegisterCandidateInputBoxArrowKeys(BOOL IsPenEditor);
extern void SetT9nCursorByEditorPen(void);
#else
static mmi_pen_editor_input_box_type_enum gPenEditorInputBoxType = PEN_EDITOR_NONE_INPUT_BOX;
#endif
/*shaozhw 20060610 modify end */

//To record the type of virtual keyboard for Symbol Keyboard switch (Need to revise)
static mmi_gui_virtual_keyboard_language_enum gPenEditorPreviousVirtualKeyboardType = MMI_GUI_VIRTUAL_KEYBOARD_MAX_LANG;
//The State of Virtual Keyboard
 mmi_pen_editor_virtual_keyboard_state_enum gPenEditorVKState = PEN_EDITOR_VK_NONE;
//Which kind of controls are in the category
static mmi_pen_editor_control_set_struct gPenEditorControlSet;
//The Active Control to process Pen Event (Activated by Pen Pown)
static mmi_pen_editor_control_type_enum gPenEditorActiveControl;
//To strore the content of the Virtual Keyboard Text in Display Area
static mmi_pen_editor_vk_text_struct gPenEditorVKText;
// Pointer to the GDI handler, used by Draw Manager
static gdi_handle *gPenEditorGDIHandler=NULL;
//Layer Buffer for Stroke
//static U32 gPenEditorStrokeGDIBuffer[(GDI_LCD_WIDTH *
//		GDI_LCD_HEIGHT * (GDI_MAINLCD_BIT_PER_PIXEL>>3) + 3) / 4];
static U8* gPenEditorStrokeGDIBuffer = NULL;
//Used for Multitap (need to revise)
static multitap_input* gPenEditorMultitapInput;
//Used for Multitap (need to revise)
static BOOL gPenEditorMultitapInputState = MMI_FALSE;
//Display th evirtual Keyboard or not
static BOOL gPenEditorShowVK = MMI_TRUE;
//LSK function from Application
static void (*gPenEditorLSKfunction)(void);
//The Index NO. of the Selected Candidate
static s32 gPenEditorCandidateIndex = 0;
//The Page NO. of the candidates (In Chinese Input Method)
static s32 gPenEditorCandidatePage = 0;
//To Store the text of LSK
static UI_string_type gPenEditorLSKText;
//To Store Virtual Keyboard Lang. Type in History
static mmi_gui_virtual_keyboard_language_enum gPenEditorHistoryVirtualKeyboardType = MMI_GUI_VIRTUAL_KEYBOARD_MAX_LANG;

static mmi_pen_editor_candidate_state_struct g_pen_editor_candidate_state;
//The valid characters for INPUT_TYPE_PHONE_NUMBER
const UI_character_type gPenEditorPhoneNumberValidCharacters[] = {'0','1','2','3','4','5','6','7','8','9','+','p','w','#','*','\0'};
//The valid characters for INPUT_TYPE_KEYPAD_NUMERIC
const UI_character_type gPenEditorKeyPadNumbericValidCharacters[] = {'0','1','2','3','4','5','6','7','8','9','#','*','\0'};
//The valid characters for INPUT_TYPE_EXT_SIM_NUMERIC
const UI_character_type gPenEditorPhoneNumberSIMValidCharacters[] = {'0','1','2','3','4','5','6','7','8','9','+','#','*','\0'};
//The valid characters for INPUT_TYPE_DECIMAL_NUMERIC
const UI_character_type gPenEditorDecimalNumericValidCharacters[] = {'0','1','2','3','4','5','6','7','8','9','.','\0'};

//The invalid characters array
const UI_character_type gPenEditorPhoneNumberInvalidCharacters[] = {'(',')','-',' ','\0'};
const UI_character_type gPenEditorKeyPadNumbericInvalidCharacters[] = {'(',')','p','w','+','-',' ','\0'};
const UI_character_type gPenEditorPhoneNumberSIMInvalidCharacters[] = {'(',')','p','w','-',' ','\0'};
const UI_character_type gPenEditorUCS2InvalidCharacters[] = {0x20ac,'\0'};
const UI_character_type gPenEditorEmptyInvalidCharacters[] = {'\0'};
const UI_character_type gPenEditorEmailADDREnableSymbles[] = {'.','@','_','+','-'};
const mmi_gui_virtual_keyboard_pen_enum gPenEditorVKDisableSpace[] = {GUI_VKBD_PEN_SPACE,GUI_VKBD_PEN_NONE};
const mmi_gui_virtual_keyboard_pen_enum gPenEditorVKDisableNewLine[] = {GUI_VKBD_PEN_NEWLINE,GUI_VKBD_PEN_NONE};
const mmi_gui_virtual_keyboard_pen_enum gPenEditorVKDisableNewLineCapsLockSymbpicker[] = {GUI_VKBD_PEN_NEWLINE,GUI_VKBD_PEN_CAPSLOCK,GUI_VKBD_PEN_SYMBPICKER,GUI_VKBD_PEN_NONE};
const mmi_gui_virtual_keyboard_pen_enum gPenEditorVKDisableEmpty[] = {GUI_VKBD_PEN_NONE};
const mmi_gui_virtual_keyboard_pen_enum gPenEditorVKDisableHideSpace[] = {GUI_VKBD_PEN_HIDE,GUI_VKBD_PEN_SPACE,GUI_VKBD_PEN_NONE};
const mmi_gui_virtual_keyboard_pen_enum gPenEditorVKDisableShow[] = {GUI_VKBD_PEN_SHOW,GUI_VKBD_PEN_NONE};
const mmi_gui_virtual_keyboard_pen_enum gPenEditorVKDisableNewLineShow[] = {GUI_VKBD_PEN_NEWLINE,GUI_VKBD_PEN_SHOW,GUI_VKBD_PEN_NONE};

//To store the height of list menu for in line editor
static s32 gPenEditorListMenuHeight = 0;
//The pen speed from PhnsetGetHandwritingSpeed
static PHNSET_HANDWRITING_SPEED_ENUM gPenEditorHWSpeed = PHNSET_HANDWRITING_MEDIUM;

static multiline_inputbox_category_history gPenEditorInlineEditHistory;
// Before timer for pen stroking up expires, if we switch input metod
// The editor can't calculate correct height. So, this variable will 
// store if input method is changed while timer running.

/*
const S32 gPenEditorHWSpeedSet[3][2]={
					{750,1750},
					{1000,3000},
					{1250,4000}
					};*/
//modify by chenq for bug 6257  B
const S32 gPenEditorHWSpeedSet[3][2]={
					{500,1500},
					{1000,2500},
					{1500,3500}
					};
//modify by chenq for bug 6257  E
//The Filler for candidate input box
UI_filled_area candidate_inputbox_BG_filler=
{
   UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_ROUNDED_BORDER|UI_FILLED_AREA_SINGLE_BORDER,
   UI_NULL_IMAGE,
   NULL,
   {	192,240,248,100	},
   {	0,0,0,0			},
   {	64,96,176,100		},
   {	255,0,0,100		},
   0
};

//The theme for candidate input box
UI_single_line_input_box_theme	candidate_singleline_inputbox_theme=
{
   &candidate_inputbox_BG_filler,
   &candidate_inputbox_BG_filler,
   &candidate_inputbox_BG_filler,
   {	0,0,0,100		},
   {	128,128,128,100	},
   {	0,0,0,100		},
   {	255,255,255,100	},
   {	51,88,171,100	},
   {	255,0,0,100		},
   &MMI_large_font,
   1,
   UI_SINGLE_LINE_INPUT_BOX_CENTER_Y,
   '*'
};


#define SCRMEM_HANDWRITING_STROKE_LAYER_POOL_SIZE (GDI_LCD_WIDTH *GDI_LCD_HEIGHT * (GDI_MAINLCD_BIT_PER_PIXEL>>3) + 3)
S32 handwriting_stroke_layer_mem=(SCRMEM_HANDWRITING_STROKE_LAYER_POOL_SIZE);

#ifdef __MMI_TR_CHINESE_VK__
typedef struct bopochar_s
{
	U8 sbchar;
	char *pronounciation;
	U16 unicode;
} BOPOCHAR;

 const BOPOCHAR BopomofoTable[] = 
{
	{'b', "b", 0x3105},
	{'p', "p", 0x3106,},
	{'m', "m", 0x3107},
	{'f', "f", 0x3108},
	{'d', "d", 0x3109},
	{'t', "t", 0x310A},
	{'n', "n", 0x310B},
	{'l', "l", 0x310C},
	{'g', "g", 0x310D},
	{'k', "k", 0x310E},
	{'h', "h", 0x310F},
	{'j', "j", 0x3110},
	{'q', "q", 0x3111},
	{'x', "x", 0x3112},
	{0xdf, "zh", 0x3113},
	{0xe7, "ch", 0x3114},
	{0x9a, "sh", 0x3115},
	{'r', "r", 0x3116},
	{'z', "z", 0x3117},
	{'c', "c", 0x3118},
	{'s', "s", 0x3119},
	{'a', "a", 0x311A},
	{'o', "o", 0x311B},
	{'e', "e", 0x311C},
	{0xe9, "ei", 0x311D},
	{0xe4, "ai", 0x311E},
	{0xf6, "ei", 0x311F},
	{0xe5, "ao", 0x3120},
	{0xfd, "ou", 0x3121},
	{0xe3, "an", 0x3122},
	{0xf5, "en", 0x3123},
	{0xe2, "ang", 0x3124},
	{0xf4, "eng", 0x3125},
	{0xf8, "er", 0x3126},
	{'i', "i", 0x3127},
	{'u', "u", 0x3128},
	{0xfc, "iu", 0x3129},
	{0, 0, 0x0000},
};

 U8 UnicodeToBPMFInternal(U16 bpmf)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U16 nIdx;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	for (nIdx = 0; BopomofoTable[nIdx].unicode; nIdx++)
	{
		if (BopomofoTable[nIdx].unicode == bpmf)
		{
			return BopomofoTable[nIdx].sbchar;
		}
	}

	return BopomofoTable[nIdx].sbchar;

}
#endif
 
multitap_input g_pen_editor_bpmf_pinyin_active_multitap[MAX_MULTITAPS];
S32 g_pen_editor_active_multitap_index = -1;
UI_character_type g_pen_editor_bpmf_pinyin_previous_valid_character = 0;
BOOL g_pen_editor_hold_confirm = MMI_FALSE;
BOOL g_pen_editor_vk_show_at_start = MMI_FALSE;
/***************************************************************************** 
* Local Function
*****************************************************************************/
BOOL change_input_mode_when_writing(void) ;
static S32 get_changed_height(
	mmi_pen_editor_virtual_keyboard_state_enum, BOOL);
static S32 get_max_blank_area_height(void);
static S32 get_VK_height(void);

void mmi_pen_editor_hold_confirm_in_callback_space(void);
void mmi_pen_editor_vk_euro_symbol_picker(void);
/***************************************************************************** 
* Global Variable
*****************************************************************************/
#ifdef __MMI_HAND_WRITING__
unsigned char bufRam[8192];//wangzl:070613 add
HWRData theHWRData; //wangzl:070613 add
#endif

extern UI_EMS_input_box MMI_EMS_inputbox;
extern multi_line_input_box MMI_multiline_inputbox;
extern UI_theme *current_UI_theme;
extern UI_multitap_input_theme* current_multitap_input_theme;
extern IP4_input*				current_IP4_input;
extern date_input*				current_date_input;
extern single_line_input_box MMI_inline_singleline_inputbox;
extern time_period_input*		current_time_period_input;
extern time_input* current_time_input;
extern S32 UI_device_width;
extern s32 wgui_EMS_inputbox_information_bar_x;
extern s32 wgui_EMS_inputbox_information_bar_y;
extern s32 wgui_EMS_inputbox_information_bar_width;
extern s32 wgui_EMS_inputbox_information_bar_height;
extern s16 MMI_current_input_type;
extern fixed_list_menu MMI_fixed_list_menu;
extern S32 (*cat200_search_function)(U8 *);
extern s32 wgui_inline_item_highlighted_index;
extern s32 UI_cursor_x1;
extern s32 UI_cursor_y1;
extern void handle_category28_change_input_mode(void);
extern void (*EMS_inputbox_input_callback)(void);
extern void (*multiline_inputbox_input_callback)(void);
extern void (*singleline_inputbox_input_callback)(void);
extern void (*inline_singleline_inputbox_input_callback)(void);
extern byte	 wgui_inline_singleline_inputbox_decimal_input_mode_flag;
extern byte wgui_multiline_inputbox_decimal_input_mode_flag;
extern byte wgui_singleline_inputbox_decimal_input_mode_flag;
extern byte wgui_inline_item_handled;
extern void (*complete_inline_item_edit)(void);
extern byte wgui_inputbox_RSK_label_clear;
extern byte category28_RSK_label_clear;
extern byte wgui_singleline_inputbox_RSK_label_clear;
extern icontext_button	MMI_softkeys[];
extern void (*wgui_inline_edit_DONE_function)(void);
extern void (*softkey_functions[WGUI_MAX_SOFTKEYS][WGUI_MAX_SOFTKEY_FUNCTIONS])(void);
extern s16 MMI_current_input_ext_type;
extern U8 numeric_leading_zero;
extern void register_inline_text_edit_handle_right_softkey_up(void);
extern UI_string_type	inline_text_edit_RSK_label_string;
extern UI_image_type	inline_text_edit_RSK_label_icon;
extern byte wgui_inline_list_menu_changed;
extern byte wgui_inline_list_menu_disable_done;
extern void handle_inline_text_edit_right_softkey_long_press(void);
extern void wgui_inline_text_edit_set_RSK_BACK_function(void);
extern UI_string_type wgui_inline_edit_get_RSK_string(void);
extern void handle_inline_text_edit_right_softkey_down(void);
extern void (*redraw_current_inline_item)(void);
extern void register_EMS_inputbox_keys(void);
extern void wgui_set_EMS_inputbox_RSK(void);
extern void inline_edit_singleline_inputbox_multitap_input_complete(void);
extern void EMS_inputbox_multitap_input_complete(void);
extern void set_leading_zero(U8 u8type);
extern void coolsand_UI_multi_line_input_box_set_pen_scroll_delay(multi_line_input_box *b, S32 delay_time);
extern void coolsand_UI_EMS_input_box_set_pen_scroll_delay(UI_EMS_input_box *b, S32 delay_time);
extern void EMS_inputbox_multitap_input(UI_character_type c);
extern void show_inline_singleline_inputbox(void);
extern void date_input_move_to_x_y(date_input *d,s32 x,s32 y);
extern void IP4_input_move_to_x_y(IP4_input *ip4,s32 x,s32 y);
extern void time_input_move_to_x_y(time_input *t,s32 x,s32 y);
extern void time_period_input_move_to_x_y(time_period_input *tp,s32 x,s32 y);
extern void wgui_multiline_inputbox_decimal_numeric_handle_pound_key_down(void);
extern void EMS_inputbox_direct_input(UI_character_type c);
extern void refresh_search_list(U8 * name);
extern void wgui_singleline_inputbox_decimal_numeric_handle_pound_key_down(void);
extern void inline_edit_singleline_inputbox_multitap_input(UI_character_type c);
extern void wgui_inline_singleline_inputbox_decimal_numeric_handle_pound_key_down(void);
extern void inline_edit_singleline_inputbox_direct_input(UI_character_type c);
extern void date_input_direct_input_nodraw(UI_character_type c);
extern s32 date_input_test_last_position(date_input *d);
extern void IP4_input_direct_input_nodraw(UI_character_type c);
extern s32 IP4_input_test_last_position(IP4_input *ip4);
extern void time_input_direct_input_nodraw(UI_character_type c);
extern s32 time_input_test_last_position(time_input *t);
extern void time_period_input_direct_input_nodraw(UI_character_type c);
extern s32 time_period_input_test_last_position(time_period_input *tp);
extern void wgui_handle_inputbox_right_softkey_down(void);
extern void EMS_inputbox_backspace(void);
extern void wgui_handle_singleline_inputbox_right_softkey_down(void);
extern void inline_edit_singleline_inputbox_delete_character(void);
extern int coolsand_UI_multi_line_input_box_get_remaining_length(multi_line_input_box *b);
extern void EMS_inputbox_previous(void);
extern int coolsand_UI_single_line_input_box_get_remaining_length(single_line_input_box *b);
extern void inline_edit_singleline_inputbox_previous_character(void);
extern void redraw_inline_singleline_inputbox(void);
extern void EMS_inputbox_insert_new_line(void);
extern s32 mmi_ime_get_candidates_by_composition(UI_character_type *composition,UI_character_type *candidatebuffer);
extern s32 mmi_ime_get_candidates_by_phrase(UI_character_type phrase,UI_character_type *candidatebuffer);
extern s32 mmi_ime_get_candidates_by_page(S32 page,UI_character_type *candidatebuffer);
extern void wgui_set_singleline_RSK(void);
extern void handle_category28_input(void);
extern void wgui_handle_inputbox_input(void);
extern void wgui_handle_qsearch_singleline_inputbox_change_input_mode(void);
extern void cat200_virtual_keypad_callback(void);
extern void RedrawCategoryControlledArea200Screen(dm_coordinates* coordinate);
extern void wgui_redraw_input_information_bar(void);
extern void change_EMS_inputbox_mode(byte mode);
extern void wgui_EMS_redraw_input_information_bar(void);
extern void wgui_inline_edit_default_list_highlight_handler(s32 item_index);
extern void wgui_handle_singleline_inputbox_change_input_mode(void);
extern void wgui_handle_inline_singleline_inputbox_change_input_mode(void);
extern MMI_BOOL get_multitap_state(void);
extern void inline_edit_change_singleline_inputbox_mode(byte mode);
extern U8* wgui_inline_edit_get_current_item_history(U8* history_buffer);
 
extern FuncPtr currKeyFuncPtrs[MAX_KEYS][MAX_KEY_TYPE];
extern UI_character_type *get_bpmf_multitap_string(s32 i);
extern const UI_character_type numberless_lowercase_multitap_strings[MAX_MULTITAPS][32];
extern void handle_inline_text_edit_right_softkey_up(void);
extern void cat201_register_key_func(void);
extern void RedrawCategoryControlledArea201Screen(void);  
extern void coolsand_UI_single_line_input_box_locate_cursor(single_line_input_box *b);
extern BOOL UI_is_timer_start(void (*callback)(void));
extern void wgui_set_RSK(void);
extern void gb_register_arrow_handler(void);
extern void GB_Reset(void);
extern BOOL gb_is_pen_in_composition_area(mmi_pen_point_struct pos);
extern void gb_display_syllable_by_index(U8 index);
extern BOOL gb_is_pen_in_candidate_area(mmi_pen_point_struct pos);
extern void gb_set_candidate_by_index(U8 index);
extern void gb_composition_area_left_arrow_pen_up_handler(void);
extern void gb_composition_area_right_arrow_pen_up_handler(void);
extern void gb_set_syllable_by_index(U8 index);
extern void gb_composition_area_left_arrow_pen_up_handler(void);
extern void gb_candidate_area_right_arrow_pen_up_handler(void);
extern void gb_input_character_by_candidate_index(U8 index);
extern void gb_candidate_area_left_arrow_pen_up_handler(void);
/*Fix Bug#12433,12607 for cs2324c by cong.li on 2009.06.05. 
   The arrow key handle is cleared in stroke_up_timer_handler, so we should reset them again.*/
extern void inline_date_edit_handle_down_arrow(void);
extern void inline_date_edit_handle_up_arrow(void);
extern void inline_time_edit_handle_down_arrow(void);
extern void inline_time_edit_handle_up_arrow(void);
extern void inline_time_period_edit_handle_down_arrow(void);
extern void inline_time_period_edit_handle_up_arrow(void);
extern void inline_text_edit_handle_down_arrow(void);
extern void inline_text_edit_handle_up_arrow(void);
extern void inline_edit_singleline_inputbox_previous_character(void);
extern void inline_edit_singleline_inputbox_next_character(void);
extern void mmi_pen_editor_draw_arrow(mmi_pen_editor_arrow_direction_enum arrow_direction,mmi_pen_editor_arrow_state_enum arrow_state,mmi_pen_editor_arrow_type_enum arrow_type);
void mmi_pen_editor_pen_set_stroke_color(color_t stroke_color);
BOOL gbChar2Unicode( UINT16* gb_char, UINT16* unicode_char);
extern void draw_resizeable_arrow(S32 x,S32 y,U8 arrow_type,color_t c, S32 width, S32 height);
extern void mmi_pen_editor_show_candidate_area(MMI_BOOL arrow_enable, mmi_pen_editor_candidate_state_enum candiate_state);
extern EBLong GbInputMethodCategoryFlag;

#define TP_USE_NAVIGATE_KEYS
#ifdef TP_USE_NAVIGATE_KEYS
void mmi_pen_editor_register_arrow_handler(void);
void mmi_pen_editor_reset_candidate_position(void);
#endif
void mmi_pen_editor_stroke_up_pound_key_handler(void);
#ifdef __MMI_HAND_WRITING__  
static fnPtr mmi_pen_editor_save_arrow_handler[4][2];
#endif
fnPtr 	mmi_pen_editor_vk_pre_handler = NULL;
fnPtr		mmi_pen_editor_hw_pre_handler = NULL;
FuncPtr    mmi_pen_editor_poudkey_down_handler = NULL;
BOOL bPoundkeyHandlerSaved = FALSE;

/***************************************************************************** 
* Global Function
*****************************************************************************/
//extern void mmi_pen_editor_hand_writing_initailize(void);
//extern s32 mmi_pen_editor_hand_writing_get_candidates(U16* stroke_buffer,UI_character_type* candidate_array,mmi_pen_editor_input_type_enum h_w_type,s32 number);
BOOL mmi_pen_in_handwriting_process(void);

#if defined(__MMI_GB__)
void mmi_resize_and_draw_input_box_according_Pen_Editor_Candidate(void)
{
	if(GbInputMethodCategoryFlag & GB_IN_CATEGORY_28_FLAG)
	{
		S32 h = gPenEditorCandidateInputBox.y - MMI_EMS_inputbox.y;
		coolsand_UI_resize_EMS_input_box(&MMI_EMS_inputbox, MMI_EMS_inputbox.width,h);
		coolsand_UI_show_EMS_input_box(&MMI_EMS_inputbox);
	}
	if(GbInputMethodCategoryFlag & GB_IN_CATEGORY_5_FLAG)
	{
		S32 h = gPenEditorCandidateInputBox.y - MMI_multiline_inputbox.y;
		resize_multiline_inputbox(MMI_multiline_inputbox.width,h);
		show_multiline_inputbox();
	}
}
#endif
#if defined(__MMI_PENPOWER__)&&defined(__MMI_HAND_WRITING__)
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_hand_writing_initailize
* DESCRIPTION
*   \brief This function is used to initialize hand writing.(Dummy)
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_pen_editor_hand_writing_initailize(void)
{
    //wangzl:070613 add B
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int RamSize = 0;   
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: mmi_pen_editor_hand_writing_initailize");
    RamSize = PPHWRGetRamSize();
    theHWRData.pPrivate=(signed char*)bufRam;
    theHWRData.pInternalDict=0;
    if( PPHWRInit(&theHWRData) != STATUS_OK )
    {
        mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: initailize failed");
        MMI_DBG_ASSERT(0);//MMI_ASSERT(0);
    }
    //wangzl:070613 add E
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_hand_writing_get_candidates
* DESCRIPTION
*   \brief This function is used to get candidates from hand writing.(Dummy)
*
* PARAMETERS
*  \param U16* stroke_buffer : the buffer of the stroke
*  \param UI_character_type* candidate_array : the buffer to store candidates
*  \param mmi_pen_editor_input_type_enum h_w_type : which kind of candidate should return, Chinese, English, Symbol (need to revise)
*  \param s32 number : the number of the candidates wished
*  
* RETURNS
*  \return s32 : the number of the candidates returned
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/


s32 mmi_pen_editor_hand_writing_get_candidates(U16* stroke_buffer,UI_character_type* candidate_array,mmi_pen_editor_input_type_enum h_w_type,s32 number)
{
	//wangzl:070613 add B
	/*----------------------------------------------------------------*/
  	/* Local Variables                                                */
 	/*----------------------------------------------------------------*/
   	int num = 0,i=0;
   	unsigned char szResult[PEN_EDITOR_MAX_CANDIDATE_SIZE*2];
   	UI_character_type curCh;

  	/*----------------------------------------------------------------*/
 	/* Code Body                                                      */
 	/*----------------------------------------------------------------*/
	TBM_ENTRY(0x2822);
	mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: mmi_pen_editor_hand_writing_get_candidates");
   	PPHWRSetCandidateNum(&theHWRData,number);
   	if (PEN_EDITOR_SIMPLIFIED_CHINESE==h_w_type)
   	{
      //chenqiang add for class tow chinese Chinese characters
   		PPHWRSetType(&theHWRData,(DWORD)CHINESE_1_TYPE|CHINESE_2_TYPE);
   	}
   	else if (PEN_EDITOR_TRADITIONAL_CHINESE==h_w_type)
	{
     //chenqiang add for class tow chinese Chinese characters
		PPHWRSetType(&theHWRData,(DWORD)TRADITIONAL_ONLY);
	}
	else if (PEN_EDITOR_LOWER_CASE_ENGLISH==h_w_type)
	{
		PPHWRSetType(&theHWRData,(DWORD)ALPHA_TYPE|NUMERIC_TYPE);
   	}
	else if (PEN_EDITOR_UPPER_CASE_ENGLISH==h_w_type)
	{
		PPHWRSetType(&theHWRData,(DWORD)ALPHA_TYPE|NUMERIC_TYPE);
	}
	else if (PEN_EDITOR_NUMBER==h_w_type)
	{
		PPHWRSetType(&theHWRData,(DWORD)NUMERIC_TYPE);
	}
	else if (PEN_EDITOR_PUNCTUATION==h_w_type)
	{
		PPHWRSetType(&theHWRData,(DWORD)SYMBOL_TYPE);
	}
	else
	{
		/*MMI_ASSERT(0)??*/
		PPHWRSetType(&theHWRData,(DWORD)SYMBOL_TYPE);
	}
	if( PPHWRRecognize(&theHWRData, (WORD *)stroke_buffer,(WORD *)szResult) != STATUS_OK )
	{
		TBM_EXIT(0x2822);
		return 0;
	}

	for (;;i+=2)
	{
		U16 c1 = szResult[i];
		U16 c2 = szResult[i+1];
		curCh = c1 | (c2 << 8);
		if (0 == curCh)
		{
			break;
		}
		num++;
		candidate_array[i>>1] = curCh;
		}
	mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: mmi_pen_editor_hand_writing_get_candidates num=%d",num);
	TBM_EXIT(0x2822);
	return num;
	//wangzl:070613 add E
   }
#endif
extern BOOL bUseLSKAsSendKey2;
void mmi_pen_editor_set_candidate_state(BOOL state)
{
	#ifdef __MMI_USE_LSK_AS_SEND2__
	static void (*mmi_pen_editor_ENTER_function)(void) = NULL;
	if(gPenEditorSelectCandidate && (!state))
	{
		SetKeyHandler(mmi_pen_editor_ENTER_function, KEY_ENTER, KEY_EVENT_UP);
		bUseLSKAsSendKey2 = TRUE;
	}
	else if((!gPenEditorSelectCandidate) && state)
	{
		mmi_pen_editor_ENTER_function = GetKeyHandler(KEY_ENTER, KEY_EVENT_UP);
		SetKeyHandler(mmi_pen_editor_left_soft_key_handler, KEY_ENTER, KEY_EVENT_UP);
		bUseLSKAsSendKey2 = FALSE;
	}
	#endif
	
	gPenEditorSelectCandidate = state;
}
#if defined (__MMI_HAND_WRITING__)
static void mmi_pen_editor_change_handwriting_area(void)
{
    static HWRBOX box;
    if (gPenEditorEnableHandWriting)
    {
        mmi_pen_handwriting_area_struct ext_stroke;

        ext_stroke.x1 = 0;
        ext_stroke.y1 = 0;
        ext_stroke.x2 = UI_device_width - 1;
        ext_stroke.y2 = UI_device_height - 1;

        g_pen_editor_original_handwriting_area.y2 = MMI_virtual_keyboard.y - 3;  // modify by chenq for bug 5954 
        if (gPenEditorSelectCandidate || gPenEditorVKCandidate)
        {
            g_pen_editor_original_handwriting_area.y2 = gPenEditorCandidateInputBox.y;
        }

        box.left = ext_stroke.x1;
        box.top = ext_stroke.y1;
        box.right = ext_stroke.x2;
        box.bottom = ext_stroke.y2;
        PPHWRSetBox( &theHWRData, &box);

        mmi_pen_change_handwriting_area(1,&g_pen_editor_original_handwriting_area,&ext_stroke);
    }
}
#endif

static void mmi_pen_editor_set_left_soft_key(void)
{
   set_left_softkey_label(gPenEditorLSKText);
   set_left_softkey_icon(NULL);
   redraw_left_softkey();
   register_left_softkey_handler();
}
static void mmi_pen_editor_set_left_soft_key_select(void)
{
   set_left_softkey_label((UI_string_type)get_string(STR_GLOBAL_SELECT));
   set_left_softkey_icon(NULL);
   redraw_left_softkey();
   register_left_softkey_handler();
}
static void mmi_pen_editor_set_right_soft_key_inline_text(void)
{
   if(coolsand_UI_single_line_input_box_get_text_length(&MMI_inline_singleline_inputbox)>0)
   {
      set_right_softkey_label((UI_string_type)get_string(WGUI_CATEGORY_CLEAR_STRING_ID));
      set_right_softkey_icon(NULL);
      register_right_softkey_handler();
      redraw_right_softkey();
      set_right_softkey_function(UI_dummy_function,KEY_EVENT_UP);
      set_right_softkey_function(handle_inline_text_edit_right_softkey_down,KEY_EVENT_DOWN);
      set_right_softkey_function(handle_inline_text_edit_right_softkey_long_press,KEY_EVENT_LONG_PRESS);
   }
   else
   {
      if(wgui_inline_list_menu_changed)
      {
         wgui_inline_text_edit_set_RSK_BACK_function();
          
         set_right_softkey_function(handle_inline_text_edit_right_softkey_up, KEY_EVENT_UP);
      }
      else
      {
         set_right_softkey_label(inline_text_edit_RSK_label_string);
         set_right_softkey_icon(inline_text_edit_RSK_label_icon);
         register_right_softkey_handler();
         redraw_right_softkey();
         set_right_softkey_function(register_inline_text_edit_handle_right_softkey_up,KEY_EVENT_UP);
         set_right_softkey_function(UI_dummy_function,KEY_EVENT_DOWN);
      }
   }
}
static void mmi_pen_editor_set_right_softkey(mmi_pen_editor_rsk_type_enum rsk_type)
{
   switch (rsk_type)
   {
      case PEN_EDITOR_RSK_BY_STATE:
         if (get_multitap_state())
         {
         	set_right_softkey_label((UI_string_type)get_string(STR_GLOBAL_CLEAR));
         }
         else
         {
         	set_right_softkey_label((UI_string_type)get_string(STR_GLOBAL_BACK));
         }
         break;

      case PEN_EDITOR_RSK_BACK:
      	set_right_softkey_label((UI_string_type)get_string(STR_GLOBAL_BACK));
      	break;
      
      case PEN_EDITOR_RSK_CLEAR:
      	set_right_softkey_label((UI_string_type)get_string(STR_GLOBAL_CLEAR));
      	break;
      
      default :
         MMI_DBG_ASSERT(0);
   }
   set_right_softkey_icon(NULL);
   redraw_right_softkey();
   register_right_softkey_handler();
   set_right_softkey_function( mmi_pen_editor_right_soft_key_handler, KEY_EVENT_UP);
   set_right_softkey_function( mmi_pen_editor_hold_confirm_in_callback_space, KEY_EVENT_DOWN);
   set_right_softkey_function( UI_dummy_function, KEY_LONG_PRESS);
}

#if defined(__MMI_HAND_WRITING_GET_STROKES__)
static void mmi_pen_editor_save_stroke_to_file(void)
{
#if defined(MMI_ON_HARDWARE_P)
      {
         FS_HANDLE stroke_out;
         U32 write_len;
         int i = 0;

         stroke_out = FS_Open((UINT8 *)L"Stroke_Out.str", FS_CREATE | FS_READ_WRITE);         
         if (stroke_out < 0)
         {
            //MMI_ASSERT(0);
         }
         
         FS_Seek(stroke_out, 0, FS_FILE_END);
         for (i = 0;i < PEN_EDITOR_STROKE_BUFFER_SIZE ; i++)
         {
            FS_Write(stroke_out, &(gPenEditorStrokeBuffer[i].x), sizeof(S16), (UINT*)&write_len);
            FS_Write(stroke_out, &(gPenEditorStrokeBuffer[i].y), sizeof(S16), (UINT*)&write_len);
#if defined(__MMI_HANWANG__)
            if ((-1) == gPenEditorStrokeBuffer[i].x && (-1) == gPenEditorStrokeBuffer[i].y )
            {
               break;
            }
#elif defined(__MMI_PENPOWER__)
            if ((-1) == gPenEditorStrokeBuffer[i].x && (0) == gPenEditorStrokeBuffer[i].y )
            {
               break;
            }
#else
            if ((-1) == gPenEditorStrokeBuffer[i].x && (-1) == gPenEditorStrokeBuffer[i].y )
            {
               break;
            }
#endif
         }
         FS_Close(stroke_out);
      }
#else
      {
         FILE *stroke_out;
         int i = 0;
         
         if ((stroke_out = fopen("Stroke_Out.str", "a+"))
               == NULL)
         {
            fprintf(stderr, "Cannot open output file.\n");
         }
         
         fseek(stroke_out, 0, SEEK_END);
         for (i = 0;i < PEN_EDITOR_STROKE_BUFFER_SIZE ; i++)
         {
            fwrite(&(gPenEditorStrokeBuffer[i].x),sizeof(S16),1,stroke_out);
            fwrite(&(gPenEditorStrokeBuffer[i].y),sizeof(S16),1,stroke_out);
            if ((-1) == gPenEditorStrokeBuffer[i].x && (-1) == gPenEditorStrokeBuffer[i].y )
            {
               break;
            }
         }
         fclose(stroke_out);
      }
#endif
}
#endif
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_hide_candidate_area
* DESCRIPTION
*   \brief This function is to hide the candidate input box.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_hide_candidate_area(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   color_t c;   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   c.r=255;
   c.g=255;
   c.b=255;
   c.alpha=100;

   coolsand_UI_reset_clip();
   coolsand_UI_lock_double_buffer();
   if ( wgui_is_wallpaper_on_bottom()==MMI_TRUE)
   {
#if defined(__MMI_MAINLCD_220X176__)
      gdi_draw_solid_rect(0,gPenEditorCandidateInputBox.y,UI_device_width,gPenEditorCandidateInputBox.y+gPenEditorCandidateInputBox.height, GDI_COLOR_TRANSPARENT);
#else
      gdi_draw_solid_rect(0,gPenEditorCandidateInputBox.y,UI_device_width,gPenEditorCandidateInputBox.y+gPenEditorCandidateInputBox.height-1, GDI_COLOR_TRANSPARENT);
#endif
   }
   else
   {
      coolsand_UI_fill_rectangle(0,gPenEditorCandidateInputBox.y,UI_device_width,gPenEditorCandidateInputBox.y+gPenEditorCandidateInputBox.height,c);
   }
   coolsand_UI_unlock_double_buffer();
   coolsand_UI_BLT_double_buffer(0,gPenEditorCandidateInputBox.y,UI_device_width,gPenEditorCandidateInputBox.y+gPenEditorCandidateInputBox.height-1);
} /* end of mmi_pen_editor_hide_candidate_area*/

#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
extern U8	g_phb_edit_email_addr ;
#endif

void mmi_pen_editor_vk_disable_keys(void)
{
    
   const UI_character_type *DisableCharacters=gPenEditorEmptyInvalidCharacters;
   const mmi_gui_virtual_keyboard_pen_enum *DisableSymbols=gPenEditorVKDisableEmpty;
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
  if((g_phb_edit_email_addr != 0)&&(MMI_GUI_VIRTUAL_KEYBOARD_SYMBOL == MMI_virtual_keyboard.lang_type||
  	MMI_GUI_VIRTUAL_KEYBOARD_TRAY == MMI_virtual_keyboard.lang_type||
  	MMI_GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY == MMI_virtual_keyboard.lang_type))
  {
	    wgui_set_virtual_keyboard_allowed_characters(gPenEditorEmailADDREnableSymbles);
  }
  else
  {
#endif  

	if ((gPenEditorInputBoxType == PEN_EDITOR_INLINE_TEXT)||(gPenEditorInputBoxType == PEN_EDITOR_SINGLE_LINE_INPU_BOX)||(MMI_current_input_ext_type & INPUT_TYPE_EXT_NO_SHOW_NEW_LINE_SYMBOL))
	{
		DisableSymbols = gPenEditorVKDisableNewLine;
	}
	 
	//add by chenq for bug 6248 20080823 B
	if((INPUT_MODE_TR_BOPOMO==MMI_current_input_mode)||(INPUT_MODE_SM_PINYIN==MMI_current_input_mode)||(INPUT_MODE_SM_STROKE==MMI_current_input_mode))
	{
		DisableSymbols = gPenEditorVKDisableEmpty;
	}
	//add by chenq for bug 6248 20080823 E
	if ((gPenEditorInputBoxType == PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX))
	{
		DisableSymbols = gPenEditorVKDisableNewLineCapsLockSymbpicker;
	}
	//add by chenq for bug 6670
	if(((INPUT_MODE_TR_BOPOMO==MMI_current_input_mode)||(INPUT_MODE_SM_PINYIN==MMI_current_input_mode)||(INPUT_MODE_SM_STROKE==MMI_current_input_mode))&&
	((gPenEditorInputBoxType == PEN_EDITOR_INLINE_TEXT)||(gPenEditorInputBoxType == PEN_EDITOR_SINGLE_LINE_INPU_BOX)||(MMI_current_input_ext_type & INPUT_TYPE_EXT_NO_SHOW_NEW_LINE_SYMBOL)))
	{
		DisableSymbols = gPenEditorVKDisableNewLine ; //gPenEditorVKDisableNewLineShow;
	}
	//add by chenq for bug 6670 20080918 E

	if ( INPUT_TYPE_PHONE_NUMBER == ( MMI_current_input_type & INPUT_TYPE_MASK ) )
	{
		if (MMI_current_input_ext_type & INPUT_TYPE_EXT_SIM_NUMERIC)
		{
			DisableCharacters = gPenEditorPhoneNumberSIMInvalidCharacters;
			DisableSymbols = gPenEditorVKDisableHideSpace;
		}
		else
		{
			DisableCharacters = gPenEditorPhoneNumberInvalidCharacters;
			DisableSymbols = gPenEditorVKDisableHideSpace;
		}
	}
	else if ( INPUT_TYPE_KEYPAD_NUMERIC == (MMI_current_input_type&INPUT_TYPE_MASK) 
	|| INPUT_TYPE_SAT_NUMERIC_PASSWORD == (MMI_current_input_type&INPUT_TYPE_MASK))
	{
		DisableCharacters = gPenEditorKeyPadNumbericInvalidCharacters;
		DisableSymbols = gPenEditorVKDisableHideSpace;
	}
	else if ( MMI_allow_only_english_input_modes )
	{
		DisableCharacters = gPenEditorUCS2InvalidCharacters;
	}
	wgui_set_virtual_keyboard_disabled_list(DisableCharacters,DisableSymbols);
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
  }
#endif
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_clear_and_show_virtual_keyboard_area
* DESCRIPTION
*   \brief This function is to clear the virtual keyboard area and show virtual keyboard.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_clear_and_show_virtual_keyboard_area(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   color_t c;   
   S32  y;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   c.r=255;
   c.g=255;
   c.b=255;
   c.alpha=100;
    
   mmi_pen_editor_vk_disable_keys();
   coolsand_UI_reset_clip();
   coolsand_UI_lock_double_buffer();
   /*shaozhw 20060610 add begin: for support sm_pinyin and sm_stroke */
   #ifdef __MMI_IME_SUPORT_SM_PINYIN_STROKE__
   if((INPUT_MODE_TR_BOPOMO==MMI_current_input_mode)||(INPUT_MODE_SM_PINYIN == MMI_current_input_mode) || (INPUT_MODE_SM_STROKE == MMI_current_input_mode))
   {
        mmi_pen_editor_vk_hide();
   }
   #endif
   /*shaozhw 20060610 add end */
   
    y = MMI_virtual_keyboard.y-1 ;// + MMI_virtual_keyboard.height - get_VK_height()-1 ;
   if ( wgui_is_wallpaper_on_bottom()==MMI_TRUE)
   {
	  gdi_draw_solid_rect(0, y, UI_device_width,MMI_virtual_keyboard.y+MMI_virtual_keyboard.height+1, GDI_COLOR_TRANSPARENT);
   }
   else
   {

	  coolsand_UI_fill_rectangle(0, y, UI_device_width,MMI_virtual_keyboard.y+MMI_virtual_keyboard.height,c);
   }
/*shaozhw 20060610 modify begin: for support sm_pinyin and sm_stroke */
#ifdef __MMI_IME_SUPORT_SM_PINYIN_STROKE__
   if((INPUT_MODE_TR_BOPOMO!=MMI_current_input_mode)&&(INPUT_MODE_SM_PINYIN != MMI_current_input_mode) && (INPUT_MODE_SM_STROKE != MMI_current_input_mode))
   {
        MMI_virtual_keyboard.height = MMI_virtual_keyboard_language_map[MMI_virtual_keyboard.lang_type].virtual_keyboard_layout->height;
        MMI_virtual_keyboard.width = MMI_virtual_keyboard_language_map[MMI_virtual_keyboard.lang_type].virtual_keyboard_layout->width;
        coolsand_UI_show_virtual_keyboard(&MMI_virtual_keyboard);
   }
   else
   {
        MMI_virtual_keyboard.width = MMI_virtual_keyboard.height = 0;
   }
#else
    coolsand_UI_show_virtual_keyboard(&MMI_virtual_keyboard);
#endif
/*shaozhw 20060610 modify end */
   coolsand_UI_unlock_double_buffer();
   coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
} /* end of mmi_pen_editor_hide_candidate_area*/


/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_hide_virtual_keyboard_area
* DESCRIPTION
*   \brief This function is to hide the virtual keyboard.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_hide_virtual_keyboard_area(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   color_t c;   
   S32   y;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   c.r=255;
   c.g=255;
   c.b=255;
   c.alpha=100;

   coolsand_UI_reset_clip();
   coolsand_UI_lock_double_buffer();
	 y = MMI_virtual_keyboard.y -1 ; //+ MMI_virtual_keyboard.height - get_VK_height();
   if ( wgui_is_wallpaper_on_bottom()==MMI_TRUE)
   {
      gdi_draw_solid_rect(0, y,UI_device_width,MMI_virtual_keyboard.y+MMI_virtual_keyboard.height-1, GDI_COLOR_TRANSPARENT);
   }
   else
   {
      coolsand_UI_fill_rectangle(0, y,UI_device_width,MMI_virtual_keyboard.y+MMI_virtual_keyboard.height-1,c);
   }
   coolsand_UI_unlock_double_buffer();
   coolsand_UI_BLT_double_buffer(0, y,UI_device_width-1,MMI_virtual_keyboard.y+MMI_virtual_keyboard.height-1);
} /* end of mmi_pen_editor_hide_candidate_area*/
#ifdef __MMI_HAND_WRITING__
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_stroke_down_handler
* DESCRIPTION
*   \brief This function is to handle the stroke down event.
*
* PARAMETERS
*  \param pos : the point of stroke down  event
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
#if defined(__MMI_GB__)
extern BOOL gbIsInput(void);
extern void GbInputMethodExit(void);
#endif
#if defined(__NATIVE_JAVA__)
extern void* gcj_GetOverlayBuffer(unsigned int size);
extern void gcj_ReleaseOverlayBuffer();
#endif
void mmi_pen_editor_confirm_selection_continue(void);

void mmi_pen_editor_stroke_down_handler(mmi_pen_point_struct pos)
{
   //----------------------------------------------------------------//
   // Code Body                                                      //
   //----------------------------------------------------------------//
	TBM_ENTRY(0x282B);
	//if hand writing is not enabled, it should not go here
	if (!gPenEditorEnableHandWriting)
	{
		//disable inputmethod switch fix 15811
		  if(mmi_pen_editor_stroke_up_pound_key_handler != GetKeyHandler(KEY_POUND, KEY_EVENT_DOWN))
		  {
		  	mmi_pen_editor_poudkey_down_handler = GetKeyHandler(KEY_POUND, KEY_EVENT_DOWN);
			bPoundkeyHandlerSaved = TRUE;
		  }
		  ClearKeyHandler(KEY_POUND, KEY_EVENT_DOWN);
		  ClearKeyHandler(KEY_POUND, KEY_EVENT_UP);
		TBM_EXIT(0x282B);
		return;
	}


   if ( gPenEditorGDIHandler == NULL || *gPenEditorGDIHandler == GDI_ERROR_HANDLE )
   {
#if defined(__NATIVE_JAVA__)
      gPenEditorStrokeGDIBuffer = (U8*)gcj_GetOverlayBuffer(handwriting_stroke_layer_mem);
#else
      gPenEditorStrokeGDIBuffer = (U8*)OslMalloc(handwriting_stroke_layer_mem);
#endif
      MMI_ASSERT(gPenEditorStrokeGDIBuffer);	
      if (GDI_LAYER_SUCCEED != dm_create_layer_using_outside_memory(0, 0, UI_device_width, UI_device_height, 
         &gPenEditorGDIHandler,(U8*) gPenEditorStrokeGDIBuffer, handwriting_stroke_layer_mem,DM_LAYER_TOP) )
      {
         MMI_ASSERT(0);
      }
	mmi_trace(g_sw_TPL, "MMI_PEN_EDIOTR: mmi_pen_editor_stroke_down_handler: dm_create_layer_using_outside_memory finished " ); 
	gdi_layer_push_and_set_active(*gPenEditorGDIHandler);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
	gdi_layer_clear(GDI_COLOR_TRANSPARENT);
	mmi_trace(g_sw_TPL, "MMI_PEN_EDIOTR: mmi_pen_editor_stroke_down_handler: gdi_layer_clear finished " ); 
	gdi_layer_pop_and_restore_active();
   }

	gdi_layer_lock_frame_buffer();
	
	if(mmi_pen_editor_hw_pre_handler != NULL)
	{
		mmi_pen_editor_hw_pre_handler();
		mmi_pen_editor_hw_pre_handler = NULL;
		mmi_pen_editor_vk_pre_handler = NULL;
	}
      coolsand_UI_cancel_timer(mmi_pen_editor_stroke_up_timer_handler);


   //If the state requires confirm, confirm it directly in this situation//
   if (gPenEditorSelectCandidate&&(gPenEditorCandidateInputBox.y>=(MMI_multitap_y -1)))
   {
        coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
#if defined(__MMI_GB__)
        if(gbIsInput())
            gPenEditorShowVK = MMI_TRUE;
        else
#endif            
            gPenEditorShowVK = MMI_FALSE;

        mmi_pen_editor_confirm_selection_continue();
   }

   //Clear Virtual Keyboard Area only at the first stroke
   if (gPenEditorFirstStroke)
   {
      gPenEditorFirstStroke = MMI_FALSE;
      //Clear Virtual Keyboard Text
      if (gPenEditorVKText.TextLength>0 || gPenEditorSelectCandidate)
      {
         mmi_pen_editor_set_left_soft_key();
         switch (gPenEditorInputBoxType)
         {
            case PEN_EDITOR_EMS_INPUT_BOX:
               #ifdef __MMI_GB__
		gb_register_arrow_handler();
		#else
           	 register_EMS_inputbox_keys();
		#endif
               break;
            case PEN_EDITOR_MULTILINE_INPUT_BOX:
            case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
            case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
		 #ifdef __MMI_GB__
		gb_register_arrow_handler();
		#else
               register_multiline_inputbox_keys();
		#endif
               if (PEN_EDITOR_DICT_MULTILINE_INPUT_BOX == gPenEditorInputBoxType)
               {
                  cat201_register_key_func();
               }
               break;
            default :
               break;
         }
         mmi_pen_editor_reset_vk_text(&gPenEditorVKText);
      //   wgui_set_virtual_keyboard_display_area((U8*)gPenEditorVKText.VKText, MMI_FALSE);
      }

      if (gPenEditorShowVK)
      {
         mmi_pen_editor_hide_virtual_keyboard_area();
      }
//wangzl:add B reset GB and clear GB input area
#if defined(__MMI_GB__)
	if(gbIsInput())
	{
	  	GB_Reset();
		//GbInputMethodExit();	
	}
#endif		
	if (gPenEditorShowVK)
	{
		if (PEN_EDITOR_DICT_MULTILINE_INPUT_BOX != gPenEditorInputBoxType)
		{
			S32 h = get_changed_height(PEN_EDITOR_VK_OFF, MMI_TRUE)+1;
			mmi_pen_editor_resize_content_area(h);
		}
		coolsand_UI_move_single_line_input_box(&gPenEditorCandidateInputBox, MMI_multitap_x, (MMI_multitap_y));
	}
//wangzl: add E
   }

	gdi_layer_unlock_frame_buffer();
	//gdi_layer_blt_previous(0,0,UI_device_width-1,UI_device_height-1);
#ifdef TP_USE_NAVIGATE_KEYS
//save keyshandler
//left
	mmi_pen_editor_save_arrow_handler[0][0]= GetKeyHandler(KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	mmi_pen_editor_save_arrow_handler[0][1]=GetKeyHandler(KEY_LEFT_ARROW,KEY_EVENT_UP);
//right
	mmi_pen_editor_save_arrow_handler[1][0]=GetKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	mmi_pen_editor_save_arrow_handler[1][1]=GetKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_UP);
//up
	mmi_pen_editor_save_arrow_handler[2][0]=GetKeyHandler(KEY_UP_ARROW,KEY_EVENT_DOWN);
	mmi_pen_editor_save_arrow_handler[2][1]=GetKeyHandler(KEY_UP_ARROW,KEY_EVENT_UP);
//down
	mmi_pen_editor_save_arrow_handler[3][0]=GetKeyHandler(KEY_DOWN_ARROW,KEY_EVENT_DOWN);
	mmi_pen_editor_save_arrow_handler[3][1]=GetKeyHandler(KEY_DOWN_ARROW,KEY_EVENT_UP);
	
	SetKeyHandler(UI_dummy_function,KEY_LEFT_ARROW,KEY_EVENT_UP);
	SetKeyHandler(UI_dummy_function,KEY_RIGHT_ARROW,KEY_EVENT_UP);
	SetKeyHandler(UI_dummy_function,KEY_UP_ARROW,KEY_EVENT_UP);
	SetKeyHandler(UI_dummy_function,KEY_DOWN_ARROW,KEY_EVENT_UP);
	SetKeyHandler(UI_dummy_function,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(UI_dummy_function,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(UI_dummy_function,KEY_UP_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(UI_dummy_function,KEY_DOWN_ARROW,KEY_EVENT_DOWN);
#endif	
	//disable inputmethod switch fix 15811
	  if(mmi_pen_editor_stroke_up_pound_key_handler != GetKeyHandler(KEY_POUND, KEY_EVENT_DOWN))
	  {
	  	mmi_pen_editor_poudkey_down_handler = GetKeyHandler(KEY_POUND, KEY_EVENT_DOWN);
		bPoundkeyHandlerSaved = TRUE;
	  }
	  ClearKeyHandler(KEY_POUND, KEY_EVENT_DOWN);
	  ClearKeyHandler(KEY_POUND, KEY_EVENT_UP);

	  
   gPenEditorShowVK = MMI_TRUE;
   gPenEditorIsHandWriting = MMI_TRUE;
   gPenEditorVKState = PEN_EDITOR_VK_OFF;
   gPenEditorPreviousPoint.x = pos.x;
   gPenEditorPreviousPoint.y = pos.y;
   TBM_EXIT(0x282B);
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_stroke_move_handler
* DESCRIPTION
*   \brief This function is to handle the stroke move event.
*
* PARAMETERS
*  \param pos : the point of stroke move
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_stroke_move_handler(mmi_pen_point_struct pos)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   TBM_ENTRY(0x282C);
   //if hand writing is not enabled, it should not go here
   if (!gPenEditorEnableHandWriting)
   {
     // MMI_DBG_ASSERT(0);
	  TBM_EXIT(0x282C);
      return;
   }
   gdi_layer_lock_frame_buffer();
   gdi_layer_push_and_set_active(*gPenEditorGDIHandler);
   gdi_layer_push_clip();
   //Draw the point of the stroke
   if((gPenEditorPreviousPoint.x >= 0)&&(gPenEditorPreviousPoint.y >= 0)&&(pos.x >= 0)&&(pos.y >= 0))
   	{
	   coolsand_UI_line(gPenEditorPreviousPoint.x,gPenEditorPreviousPoint.y,pos.x,pos.y,gPenEditorStrokeColor);
	   coolsand_UI_line(gPenEditorPreviousPoint.x+1,gPenEditorPreviousPoint.y,pos.x+1,pos.y,gPenEditorStrokeColor);
	   coolsand_UI_line(gPenEditorPreviousPoint.x,gPenEditorPreviousPoint.y+1,pos.x,pos.y+1,gPenEditorStrokeColor);
	   coolsand_UI_line(gPenEditorPreviousPoint.x-1,gPenEditorPreviousPoint.y,pos.x-1,pos.y,gPenEditorStrokeColor);
	   coolsand_UI_line(gPenEditorPreviousPoint.x,gPenEditorPreviousPoint.y-1,pos.x,pos.y-1,gPenEditorStrokeColor);
   	}
   
   gdi_layer_pop_clip();
   gdi_layer_pop_and_restore_active();
   gdi_layer_unlock_frame_buffer();

   gdi_layer_blt_previous(PEN_MIN(gPenEditorPreviousPoint.x, pos.x)-1,
                         PEN_MIN(gPenEditorPreviousPoint.y, pos.y)-1,
                         PEN_MAX(gPenEditorPreviousPoint.x, pos.x)+1,
                         PEN_MAX(gPenEditorPreviousPoint.y, pos.y)+1);
   
   gPenEditorPreviousPoint.x = pos.x;
   gPenEditorPreviousPoint.y = pos.y;
   TBM_EXIT(0x282C);
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_stroke_move_handler
* DESCRIPTION
*   \brief This function is to handle the stroke up event.
*
* PARAMETERS
*  \param pos : the point of stroke up
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_stroke_up_handler(mmi_pen_point_struct pos)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   //if hand writing is not enabled, it should not go here
    mmi_pen_handwriting_area_struct stroke_are;
      stroke_are.x1 = 0;
      stroke_are.y1 = 0;
      stroke_are.x2 = UI_device_width - 1;
      stroke_are.y2 = UI_device_height - 1;
  mmi_pen_change_handwriting_area(1,&stroke_are,&stroke_are);
   mmi_trace(g_sw_HandW, "MMI_PEN_EDITOR: mmi_pen_editor_stroke_up_handler ");
   if (!gPenEditorEnableHandWriting)
   {
	   if(bPoundkeyHandlerSaved)
	  {
		SetKeyHandler( mmi_pen_editor_poudkey_down_handler,  KEY_POUND,  KEY_EVENT_DOWN);
		bPoundkeyHandlerSaved = FALSE;
	  }
     // MMI_DBG_ASSERT(0);
      return;
   }
   mmi_pen_editor_stroke_move_handler(pos);
   gPenEditorPreviousPoint.x = -1;
   gPenEditorPreviousPoint.y = -1;
   gPenEditorIsHandWriting = MMI_FALSE;
 coolsand_UI_cancel_timer(mmi_pen_editor_stroke_up_timer_handler);
   //The end of the stroke, if no more stroke in the period, the end of the character
 coolsand_UI_start_timer(PEN_EDITOR_CHARACTER_END_PERIOD,mmi_pen_editor_stroke_up_timer_handler);
  SetKeyHandler( mmi_pen_editor_stroke_up_pound_key_handler,  KEY_POUND,  KEY_EVENT_DOWN);
 //COS_KillTimer(GetMmiTaskHandle(MOD_TP_TASK), TPTimerIdHandwritingUp );
 //COS_SetTimer(GetMmiTaskHandle(MOD_TP_TASK), TPTimerIdHandwritingUp, 0, (TP_TIMER_HANDWRITING_UP)MILLI_SECOND);
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_stroke_up_timer_handler
* DESCRIPTION
*   \brief This function is to handle the stroke end event.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
extern U16 get_ems_input_character_number(void); //wangzl: add
void mmi_pen_editor_stroke_up_timer_handler(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   BOOL has_unfinished_stroke;
   //wangzl:add B
   BOOL input_flag = MMI_TRUE;
   int nRegRet = 0;
   //wangzl:add E
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   TBM_ENTRY(0x2821);
   mmi_trace(g_sw_HandW, "MMI_PEN_EDITOR: entry mmi_pen_editor_stroke_up_timer_handler gPenEditorEnableHandWriting=%d",gPenEditorEnableHandWriting);
   //if hand writing is not enabled, it should not go here
   if (!gPenEditorEnableHandWriting)
   {
      MMI_DBG_ASSERT(0);
	TBM_EXIT(0x2821);
      return;
   }
   
   mmi_pen_peek_stroke_state(&has_unfinished_stroke);
   mmi_trace(g_sw_HandW, "MMI_PEN_EDITOR: mmi_pen_editor_stroke_up_timer_handler has_unfinished_stroke=%d",has_unfinished_stroke);
   if (!has_unfinished_stroke)
   {
      // The end of the character, pass the stroke data for hand writing recognization
      mmi_pen_end_strokes_of_character();
#if defined(__MMI_HAND_WRITING_GET_STROKES__)
      mmi_pen_editor_save_stroke_to_file();
#endif
      gdi_layer_push_and_set_active(*gPenEditorGDIHandler);
      gdi_layer_clear(GDI_COLOR_TRANSPARENT);
      gdi_layer_pop_and_restore_active();
      gPenEditorFirstStroke = MMI_TRUE;
//wangzl:add B	解决短信界面纯英文字符数量达到268后 可以继续需要中文的bug
#if 1
	if((PEN_EDITOR_EMS_INPUT_BOX == gPenEditorInputBoxType)&&(((INPUT_MODE_TR_BOPOMO==MMI_current_input_mode)||INPUT_MODE_SM_PINYIN==MMI_current_input_mode||INPUT_MODE_SM_STROKE==MMI_current_input_mode )))
	{
		if(get_ems_input_character_number() >=268)
		{	
			input_flag = MMI_FALSE;
		}
	}
      
   	mmi_trace(g_sw_HandW, "MMI_PEN_EDITOR: mmi_pen_editor_stroke_up_timer_handler input_flag=%d",input_flag);

	if(input_flag)
	{
		nRegRet = mmi_pen_editor_get_candidates_from_hand_writing();
	}	
	else
	{
		//DisplayPopup ((PU8)GetString (<str id of no space for input chinese>),IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		UI_editor_play_tone_cannot_insert();
	}
	
   	mmi_trace(g_sw_HandW, "MMI_PEN_EDITOR: mmi_pen_editor_stroke_up_timer_handler nRegRet=%d",nRegRet);
	if (0 == nRegRet)
#else
       
      if (!mmi_pen_editor_get_candidates_from_hand_writing())
#endif
//wangzl:modify E
      {
         coolsand_UI_lock_double_buffer();
         gPenEditorVKState = PEN_EDITOR_VK_ON;
         if (gPenEditorShowVK&&(gPenEditorCandidateInputBox.y>=(MMI_multitap_y -1)))
         {
             
            if (!(MMI_GUI_VIRTUAL_KEYBOARD_TRAY==MMI_virtual_keyboard.lang_type
               ||MMI_GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY==MMI_virtual_keyboard.lang_type
               ||MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY==MMI_virtual_keyboard.lang_type
               ||MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_DOT_TRAY==MMI_virtual_keyboard.lang_type))
            {
               if (PEN_EDITOR_DICT_MULTILINE_INPUT_BOX != gPenEditorInputBoxType)
               {
                  	S32 h = get_changed_height(PEN_EDITOR_VK_ON, MMI_FALSE);
				  	mmi_pen_editor_resize_content_area(h-1);
               }
            }
         }
		
         if (gPenEditorShowVK)
         {
            mmi_pen_editor_clear_and_show_virtual_keyboard_area();
         }
   
         if (gPenEditorVKCandidate)
         {
            if (gPenEditorShowVK)
            {
               coolsand_UI_move_single_line_input_box(&gPenEditorCandidateInputBox, gPenEditorCandidateInputBox.x, MMI_virtual_keyboard.y-gPenEditorCandidateInputBox.height-2);
            }
            mmi_pen_editor_hide_candidate_area();
         }
//fix bug 15852
         mmi_pen_editor_change_handwriting_area();

         mmi_pen_begin_strokes_of_character();

		 
         coolsand_UI_unlock_double_buffer();
         coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
	  if( GetKeyHandler(KEY_POUND,KEY_EVENT_DOWN) == mmi_pen_editor_stroke_up_pound_key_handler )
	  {
		SetKeyHandler(mmi_pen_editor_switch_input_method, KEY_POUND, KEY_EVENT_DOWN);
	  }
         switch (gPenEditorInputBoxType)
         {
            case PEN_EDITOR_EMS_INPUT_BOX:
               #ifdef __MMI_GB__
		gb_register_arrow_handler();
		#else
           	 register_EMS_inputbox_keys();
		#endif
               break;
            case PEN_EDITOR_MULTILINE_INPUT_BOX:
            case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
            case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
		 #ifdef __MMI_GB__
		gb_register_arrow_handler();
		#else
               register_multiline_inputbox_keys();
		#endif
               if (PEN_EDITOR_DICT_MULTILINE_INPUT_BOX == gPenEditorInputBoxType)
               {
                  cat201_register_key_func();
               }
               break;
            default :
               break;
         }
	  TBM_EXIT(0x2821);
         return;
      }

      /*According different input box, do different actions*/
      mmi_pen_editor_input_character(gPenEditorCandidateBuffer[0], MMI_TRUE);
      mmi_pen_editor_change_handwriting_area();
      /*Temp Solution for LSK Text Update*/
      mmi_pen_editor_store_LSK_Text();
      mmi_pen_editor_set_left_soft_key_select();
       
      if (softkey_functions[MMI_LEFT_SOFTKEY][KEY_EVENT_UP] != mmi_pen_editor_left_soft_key_handler)
      {
         mmi_pen_editor_set_inputbox_LSK_Function(softkey_functions[MMI_LEFT_SOFTKEY][KEY_EVENT_UP]);
      }
      set_left_softkey_function(mmi_pen_editor_left_soft_key_handler,KEY_EVENT_UP);
      set_left_softkey_function(mmi_pen_editor_hold_confirm,KEY_EVENT_DOWN);
#if !defined(__MMI_HANDWRITING_PAD__)
      coolsand_UI_start_timer(PEN_EDITOR_CONFIRM_PERIOD,mmi_pen_editor_confirm_selection);
   //  COS_SetTimer(GetMmiTaskHandle(MOD_TP_TASK), TPTimerIdHandwritingConfirm, 0,
    // (TP_TIMER_HANDWRITING_CONFIRM)MILLI_SECOND);
#endif
      mmi_pen_begin_strokes_of_character();
#if !defined(TP_USE_NAVIGATE_KEYS)
      SetKeyHandler(UI_dummy_function,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
      SetKeyHandler(UI_dummy_function,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
      SetKeyHandler(UI_dummy_function,KEY_UP_ARROW,KEY_EVENT_DOWN);
      SetKeyHandler(UI_dummy_function,KEY_DOWN_ARROW,KEY_EVENT_DOWN);
#endif	  
   }
   else
   {
	  if( GetKeyHandler(KEY_POUND,KEY_EVENT_DOWN) == mmi_pen_editor_stroke_up_pound_key_handler )
	  {
		return ;
	  }
   }
 #ifdef TP_USE_NAVIGATE_KEYS
	mmi_pen_editor_register_arrow_handler();
#endif
  ClearKeyHandler(KEY_POUND, KEY_EVENT_DOWN);
  ClearKeyHandler(KEY_POUND, KEY_EVENT_UP);
  if(bPoundkeyHandlerSaved)
  {
	SetKeyHandler( mmi_pen_editor_poudkey_down_handler,  KEY_POUND,  KEY_EVENT_DOWN);
	bPoundkeyHandlerSaved = FALSE;
  }
  
   TBM_EXIT(0x2821);
}

void mmi_pen_editor_stroke_up_pound_key_handler(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   BOOL has_unfinished_stroke;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   TBM_ENTRY(0x2821);
   mmi_trace(g_sw_HandW, "MMI_PEN_EDITOR: mmi_pen_editor_stroke_up_pound_key_handler gPenEditorEnableHandWriting=%d",gPenEditorEnableHandWriting);
   //if hand writing is not enabled, it should not go here
    if(UI_is_timer_start(mmi_pen_editor_stroke_up_timer_handler))    
    {
        coolsand_UI_cancel_timer(mmi_pen_editor_stroke_up_timer_handler);
    }

   if (!gPenEditorEnableHandWriting)
   {
      MMI_DBG_ASSERT(0);
	TBM_EXIT(0x2821);
      return;
   }
   
   mmi_pen_peek_stroke_state(&has_unfinished_stroke);
   mmi_trace(g_sw_HandW, "MMI_PEN_EDITOR: mmi_pen_editor_stroke_up_pound_key_handler has_unfinished_stroke=%d",has_unfinished_stroke);
   if (!has_unfinished_stroke)
   {
      // The end of the character, pass the stroke data for hand writing recognization
      mmi_pen_end_strokes_of_character();
#if defined(__MMI_HAND_WRITING_GET_STROKES__)
      mmi_pen_editor_save_stroke_to_file();
#endif
      gdi_layer_push_and_set_active(*gPenEditorGDIHandler);
      gdi_layer_clear(GDI_COLOR_TRANSPARENT);
      gdi_layer_pop_and_restore_active();
      gPenEditorFirstStroke = MMI_TRUE;
//wangzl:add B	解决短信界面纯英文字符数量达到268后 可以继续需要中文的bug
         mmi_pen_begin_strokes_of_character();
         coolsand_UI_lock_double_buffer();
         gPenEditorVKState = PEN_EDITOR_VK_ON;
         if (gPenEditorShowVK&&(gPenEditorCandidateInputBox.y>=(MMI_multitap_y -1)))
         {
             
            if (!(MMI_GUI_VIRTUAL_KEYBOARD_TRAY==MMI_virtual_keyboard.lang_type
               ||MMI_GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY==MMI_virtual_keyboard.lang_type
               ||MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY==MMI_virtual_keyboard.lang_type
               ||MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_DOT_TRAY==MMI_virtual_keyboard.lang_type))
            {
               if (PEN_EDITOR_DICT_MULTILINE_INPUT_BOX != gPenEditorInputBoxType)
               {
                  	S32 h = get_changed_height(PEN_EDITOR_VK_ON, MMI_FALSE);
				  	mmi_pen_editor_resize_content_area(h-1);
               }
            }
         }
		
         if (gPenEditorShowVK)
         {
            mmi_pen_editor_clear_and_show_virtual_keyboard_area();
         }
   
         if (gPenEditorVKCandidate)
         {
            if (gPenEditorShowVK)
            {
               coolsand_UI_move_single_line_input_box(&gPenEditorCandidateInputBox, gPenEditorCandidateInputBox.x, MMI_virtual_keyboard.y-gPenEditorCandidateInputBox.height-2);
            }
            mmi_pen_editor_hide_candidate_area();
         }
         coolsand_UI_unlock_double_buffer();
         coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
   	}
	if(bPoundkeyHandlerSaved)
	{
		SetKeyHandler(mmi_pen_editor_poudkey_down_handler,KEY_POUND,KEY_EVENT_DOWN);
		bPoundkeyHandlerSaved = FALSE;
	}

#ifdef TP_USE_NAVIGATE_KEYS
	SetKeyHandler(mmi_pen_editor_save_arrow_handler[0][0],KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(mmi_pen_editor_save_arrow_handler[1][0],KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(mmi_pen_editor_save_arrow_handler[2][0],KEY_UP_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(mmi_pen_editor_save_arrow_handler[3][0],KEY_DOWN_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(mmi_pen_editor_save_arrow_handler[0][1],KEY_LEFT_ARROW,KEY_EVENT_UP);
	SetKeyHandler(mmi_pen_editor_save_arrow_handler[1][1],KEY_RIGHT_ARROW,KEY_EVENT_UP);
	SetKeyHandler(mmi_pen_editor_save_arrow_handler[2][1],KEY_UP_ARROW,KEY_EVENT_UP);
	SetKeyHandler(mmi_pen_editor_save_arrow_handler[3][1],KEY_DOWN_ARROW,KEY_EVENT_UP);
	
#endif	
	TBM_EXIT(0x2821);
         return;
}
#endif
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_confirm_selection
* DESCRIPTION
*   \brief This function is to confirm the selection.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_confirm_selection(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   //if hand writing is not enabled, it should not go here
   TBM_ENTRY(0x2826);
   if (!gPenEditorEnableHandWriting)
   {
      MMI_DBG_ASSERT(0);
	  TBM_EXIT(0x2826);
      return;
   }
   mmi_trace(g_sw_HandW, "MMI_PEN_EDITOR: mmi_pen_editor_confirm_selection");
   //gPenEditorShowVK to avoid clear the virtual keyboard area in some situation
   if (gPenEditorSelectCandidate)
   {
      coolsand_UI_lock_double_buffer();
      //gPenEditorSelectCandidate = MMI_FALSE;
      mmi_pen_editor_set_candidate_state(MMI_FALSE);
      /*Temp Solution for LSK Text Update*/
      mmi_pen_editor_set_left_soft_key();
      switch (gPenEditorInputBoxType)
      {
         case PEN_EDITOR_EMS_INPUT_BOX:
		#ifdef __MMI_GB__
		gb_register_arrow_handler();
		#else
           	 register_EMS_inputbox_keys();
		#endif
		wgui_set_EMS_inputbox_RSK();
		break;
         case PEN_EDITOR_MULTILINE_INPUT_BOX:
         case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
         case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
             #ifdef __MMI_GB__
		gb_register_arrow_handler();
		#else
               register_multiline_inputbox_keys();
		#endif
            wgui_set_RSK();
            if (PEN_EDITOR_DICT_MULTILINE_INPUT_BOX == gPenEditorInputBoxType)
            {
               cat201_register_key_func();
            }
            break;
         case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
            wgui_singleline_inputbox_RSK_label_clear=0;
            singleline_inputbox_multitap_input_complete();
            wgui_set_singleline_RSK();
            break;
	  /*Fix Bug#12607 for cs2324c by cong.li on 2009.06.16. 
  		 The arrow key handle is cleared in stroke_up_timer_handler, so we should reset them again.*/			
         case PEN_EDITOR_INLINE_TEXT:
		{
                      SetKeyHandler(inline_text_edit_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
                      SetKeyHandler(inline_text_edit_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
                      SetKeyHandler(inline_edit_singleline_inputbox_previous_character, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
                      SetKeyHandler(inline_edit_singleline_inputbox_next_character, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);				
                      inline_edit_singleline_inputbox_multitap_input_complete();
                      mmi_pen_editor_set_right_soft_key_inline_text();
                      break;				
         	}
	  /*Fix Bug#12433 for cs2324c by cong.li on 2009.06.05. 
	     The arrow key handle is cleared in stroke_up_timer_handler, so we should reset them again.*/
         case PEN_EDITOR_DATE:
		{
			SetKeyHandler(inline_date_edit_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
			SetKeyHandler(inline_date_edit_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
    			SetKeyHandler(date_input_previous_character, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    			SetKeyHandler(date_input_next_character, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);			
			set_right_softkey_label(wgui_inline_edit_get_RSK_string());
			set_right_softkey_icon(NULL);
			register_right_softkey_handler();
			set_right_softkey_function(execute_wgui_inline_edit_RSK_function,KEY_EVENT_UP);
			redraw_right_softkey();
			break;
         	}		 	
         case PEN_EDITOR_TIME:
		{
			SetKeyHandler(inline_time_edit_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
			SetKeyHandler(inline_time_edit_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
    			SetKeyHandler(time_input_previous_character, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    			SetKeyHandler(time_input_next_character, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);			
			set_right_softkey_label(wgui_inline_edit_get_RSK_string());
			set_right_softkey_icon(NULL);
			register_right_softkey_handler();
			set_right_softkey_function(execute_wgui_inline_edit_RSK_function,KEY_EVENT_UP);
			redraw_right_softkey();
			break;
         	}
         case PEN_EDITOR_TIME_PERIOD:
		{
			SetKeyHandler(inline_time_period_edit_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
			SetKeyHandler(inline_time_period_edit_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
    			SetKeyHandler(time_period_input_previous_character, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    			SetKeyHandler(time_period_input_next_character, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);			
			set_right_softkey_label(wgui_inline_edit_get_RSK_string());
			set_right_softkey_icon(NULL);
			register_right_softkey_handler();
			set_right_softkey_function(execute_wgui_inline_edit_RSK_function,KEY_EVENT_UP);
			redraw_right_softkey();
			break;
         	}		 	
         case PEN_EDITOR_IP4:
            set_right_softkey_label(wgui_inline_edit_get_RSK_string());
            set_right_softkey_icon(NULL);
            register_right_softkey_handler();
            set_right_softkey_function(execute_wgui_inline_edit_RSK_function,KEY_EVENT_UP);
            redraw_right_softkey();
            break;
         default :
            break;
      }
      mmi_pen_editor_hide_candidate_area();
      gPenEditorVKState = PEN_EDITOR_VK_ON;
	  if (gPenEditorShowVK&&(gPenEditorCandidateInputBox.y>=(MMI_multitap_y -1)))
      {
      	 
         if (!(MMI_GUI_VIRTUAL_KEYBOARD_TRAY==MMI_virtual_keyboard.lang_type
				||MMI_GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY==MMI_virtual_keyboard.lang_type
				||MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY==MMI_virtual_keyboard.lang_type
				||MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_DOT_TRAY==MMI_virtual_keyboard.lang_type))
         {
			if (PEN_EDITOR_DICT_MULTILINE_INPUT_BOX != gPenEditorInputBoxType)
            {
				S32 h = get_changed_height(gPenEditorVKState, MMI_FALSE)-1;
			   	mmi_pen_editor_resize_content_area(h);
			}
         }
      }
      if (gPenEditorShowVK)
      {
         mmi_pen_editor_clear_and_show_virtual_keyboard_area();
      }

      if (gPenEditorVKCandidate)
      {
         if (gPenEditorShowVK)
         {
         	 
	         if (gPenEditorVKText.TextLength>0)
	         {
	            mmi_pen_editor_reset_vk_text(&gPenEditorVKText);
	            wgui_set_virtual_keyboard_display_area((U8*)gPenEditorVKText.VKText, MMI_TRUE);
	         }
            coolsand_UI_move_single_line_input_box(&gPenEditorCandidateInputBox, gPenEditorCandidateInputBox.x, MMI_virtual_keyboard.y-gPenEditorCandidateInputBox.height-2);
         }
         mmi_pen_editor_hide_candidate_area();
      }
      #ifdef __MMI_HAND_WRITING__
      if (gPenEditorShowVK)
      {
         mmi_pen_editor_change_handwriting_area();
      }
	#endif
      coolsand_UI_unlock_double_buffer();
      coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
   }
   else
   {
   	TBM_EXIT(0x2826);
      return;
   }

   //According different input box, do different action
   switch (gPenEditorInputBoxType)
   {
      case PEN_EDITOR_MULTILINE_INPUT_BOX:
      case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
      case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
         multiline_inputbox_multitap_input_complete();
         break;
      case PEN_EDITOR_EMS_INPUT_BOX:
         EMS_inputbox_multitap_input_complete();
         break;
      case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
         singleline_inputbox_multitap_input_complete();
         break;
      case PEN_EDITOR_INLINE_TEXT:
         inline_edit_singleline_inputbox_multitap_input_complete();
         break;
      case PEN_EDITOR_DATE:
         date_input_next_character();
         break;
      case PEN_EDITOR_IP4:
         IP4_input_next_character();
         break;
      case PEN_EDITOR_TIME:
         time_input_next_character();
         break;
      case PEN_EDITOR_TIME_PERIOD:
         time_period_input_next_character();
         break;
      default:
         MMI_ASSERT(0);
   }
   TBM_EXIT(0x2826);
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_confirm_selection
* DESCRIPTION
*   \brief This function is to confirm the selection.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_confirm_selection_continue(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   //if hand writing is not enabled, it should not go here
   TBM_ENTRY(0x2826);
   if (!gPenEditorEnableHandWriting)
   {
      MMI_DBG_ASSERT(0);
	  TBM_EXIT(0x2826);
      return;
   }
   mmi_trace(g_sw_HandW, "MMI_PEN_EDITOR: mmi_pen_editor_confirm_selection");
   //gPenEditorShowVK to avoid clear the virtual keyboard area in some situation
   if (gPenEditorSelectCandidate)
   {
      //gPenEditorSelectCandidate = MMI_FALSE;
      mmi_pen_editor_set_candidate_state(MMI_FALSE);
      /*Temp Solution for LSK Text Update*/
      mmi_pen_editor_set_left_soft_key();
      switch (gPenEditorInputBoxType)
      {
         case PEN_EDITOR_EMS_INPUT_BOX:
		#ifdef __MMI_GB__
		gb_register_arrow_handler();
		#else
           	 register_EMS_inputbox_keys();
		#endif
		wgui_set_EMS_inputbox_RSK();
		break;
         case PEN_EDITOR_MULTILINE_INPUT_BOX:
         case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
         case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
             #ifdef __MMI_GB__
		gb_register_arrow_handler();
		#else
               register_multiline_inputbox_keys();
		#endif
            wgui_set_RSK();
            if (PEN_EDITOR_DICT_MULTILINE_INPUT_BOX == gPenEditorInputBoxType)
            {
               cat201_register_key_func();
            }
            break;
         case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
            wgui_singleline_inputbox_RSK_label_clear=0;
            singleline_inputbox_multitap_input_complete();
            wgui_set_singleline_RSK();
            break;
	  /*Fix Bug#12607 for cs2324c by cong.li on 2009.06.16. 
  		 The arrow key handle is cleared in stroke_up_timer_handler, so we should reset them again.*/			
         case PEN_EDITOR_INLINE_TEXT:
		{
                      SetKeyHandler(inline_text_edit_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
                      SetKeyHandler(inline_text_edit_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
                      SetKeyHandler(inline_edit_singleline_inputbox_previous_character, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
                      SetKeyHandler(inline_edit_singleline_inputbox_next_character, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);				
                      inline_edit_singleline_inputbox_multitap_input_complete();
                      mmi_pen_editor_set_right_soft_key_inline_text();
                      break;				
         	}
	  /*Fix Bug#12433 for cs2324c by cong.li on 2009.06.05. 
	     The arrow key handle is cleared in stroke_up_timer_handler, so we should reset them again.*/
         case PEN_EDITOR_DATE:
		{
			SetKeyHandler(inline_date_edit_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
			SetKeyHandler(inline_date_edit_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
    			SetKeyHandler(date_input_previous_character, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    			SetKeyHandler(date_input_next_character, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);			
			set_right_softkey_label(wgui_inline_edit_get_RSK_string());
			set_right_softkey_icon(NULL);
			register_right_softkey_handler();
			set_right_softkey_function(execute_wgui_inline_edit_RSK_function,KEY_EVENT_UP);
			redraw_right_softkey();
			break;
         	}		 	
         case PEN_EDITOR_TIME:
		{
			SetKeyHandler(inline_time_edit_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
			SetKeyHandler(inline_time_edit_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
    			SetKeyHandler(time_input_previous_character, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    			SetKeyHandler(time_input_next_character, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);			
			set_right_softkey_label(wgui_inline_edit_get_RSK_string());
			set_right_softkey_icon(NULL);
			register_right_softkey_handler();
			set_right_softkey_function(execute_wgui_inline_edit_RSK_function,KEY_EVENT_UP);
			redraw_right_softkey();
			break;
         	}
         case PEN_EDITOR_TIME_PERIOD:
		{
			SetKeyHandler(inline_time_period_edit_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
			SetKeyHandler(inline_time_period_edit_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
    			SetKeyHandler(time_period_input_previous_character, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    			SetKeyHandler(time_period_input_next_character, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);			
			set_right_softkey_label(wgui_inline_edit_get_RSK_string());
			set_right_softkey_icon(NULL);
			register_right_softkey_handler();
			set_right_softkey_function(execute_wgui_inline_edit_RSK_function,KEY_EVENT_UP);
			redraw_right_softkey();
			break;
         	}		 	
         case PEN_EDITOR_IP4:
            set_right_softkey_label(wgui_inline_edit_get_RSK_string());
            set_right_softkey_icon(NULL);
            register_right_softkey_handler();
            set_right_softkey_function(execute_wgui_inline_edit_RSK_function,KEY_EVENT_UP);
            redraw_right_softkey();
            break;
         default :
            break;
      }
      mmi_pen_editor_hide_candidate_area();
      
   }
   else
   {
   	TBM_EXIT(0x2826);
      return;
   }

   //According different input box, do different action
   switch (gPenEditorInputBoxType)
   {
      case PEN_EDITOR_MULTILINE_INPUT_BOX:
      case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
      case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
         multiline_inputbox_multitap_input_complete();
         break;
      case PEN_EDITOR_EMS_INPUT_BOX:
         EMS_inputbox_multitap_input_complete();
         break;
      case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
         singleline_inputbox_multitap_input_complete();
         break;
      case PEN_EDITOR_INLINE_TEXT:
         inline_edit_singleline_inputbox_multitap_input_complete();
         break;
      case PEN_EDITOR_DATE:
         date_input_next_character();
         break;
      case PEN_EDITOR_IP4:
         IP4_input_next_character();
         break;
      case PEN_EDITOR_TIME:
         time_input_next_character();
         break;
      case PEN_EDITOR_TIME_PERIOD:
         time_period_input_next_character();
         break;
      default:
         MMI_ASSERT(0);
   }
   TBM_EXIT(0x2826);
}
#ifdef __MMI_HAND_WRITING__
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_get_candidates_from_hand_writing
* DESCRIPTION
*   \brief This function is to get candidates from hand writing and put into candidate input box.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
extern INT16 fat_ConvertGBToUni( UINT16 c );//wangzl:070616 add,use for convert return value of penpower from GBK to Unicode when it is chinese input method
int mmi_pen_editor_get_candidates_from_hand_writing(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   UI_character_type candidate_buffer[PEN_EDITOR_MAX_CANDIDATE_SIZE];
   int num=0,i=0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   switch (MMI_current_input_mode)
   {
      case INPUT_MODE_TR_MULTITAP_BOPOMO:
     case INPUT_MODE_TR_BOPOMO:
      case INPUT_MODE_TR_STROKE:
         num=mmi_pen_editor_hand_writing_get_candidates((U16*)gPenEditorStrokeBuffer,candidate_buffer,PEN_EDITOR_TRADITIONAL_CHINESE,PEN_EDITOR_CANDIDATE_SIZE);
         break;
      case INPUT_MODE_SM_MULTITAP_PINYIN:
      case INPUT_MODE_SM_PINYIN:
      case INPUT_MODE_SM_STROKE:
         num=mmi_pen_editor_hand_writing_get_candidates((U16*)gPenEditorStrokeBuffer,candidate_buffer,PEN_EDITOR_SIMPLIFIED_CHINESE,PEN_EDITOR_CANDIDATE_SIZE);
         break;
      case INPUT_MODE_MULTITAP_UPPERCASE_ABC:
      case INPUT_MODE_SMART_UPPERCASE_ABC:
         num=mmi_pen_editor_hand_writing_get_candidates((U16*)gPenEditorStrokeBuffer,candidate_buffer,PEN_EDITOR_UPPER_CASE_ENGLISH,PEN_EDITOR_CANDIDATE_SIZE);
          
         mmi_pen_editor_resort_candidates(candidate_buffer,num,PEN_EDITOR_SORT_CAPITAL_FIRST);
         break;
      case INPUT_MODE_MULTITAP_LOWERCASE_ABC:
      case INPUT_MODE_SMART_LOWERCASE_ABC:
         num=mmi_pen_editor_hand_writing_get_candidates((U16*)gPenEditorStrokeBuffer,candidate_buffer,PEN_EDITOR_LOWER_CASE_ENGLISH,PEN_EDITOR_CANDIDATE_SIZE);
          
         mmi_pen_editor_resort_candidates(candidate_buffer,num,PEN_EDITOR_SORT_NON_CAPITAL_FIRST);
         break;
      case INPUT_MODE_123:
         num=mmi_pen_editor_hand_writing_get_candidates((U16*)gPenEditorStrokeBuffer,candidate_buffer,PEN_EDITOR_NUMBER,PEN_EDITOR_CANDIDATE_SIZE);
         break;
      default:
         break;
   }
   coolsand_UI_single_line_input_box_delete_all(&gPenEditorCandidateInputBox);
    
   if (0 == num)
   {
      return 0;
   }
   {
       
      U8 temp = numeric_leading_zero;
      set_leading_zero(TRUE);
	  //wangzl: 070616 modify B  for return value of penpower is GBK when it is chinese
  #if defined( __MMI_PENPOWER__)&&defined( MMI_ON_HARDWARE_P)
	if((INPUT_MODE_TR_BOPOMO==MMI_current_input_mode)||(INPUT_MODE_SM_MULTITAP_PINYIN == MMI_current_input_mode) ||(INPUT_MODE_SM_PINYIN == MMI_current_input_mode)||(INPUT_MODE_SM_STROKE == MMI_current_input_mode))
	{
		for(i=0;i<num;i++)
		{
			if(gbChar2Unicode( &candidate_buffer[i], &candidate_buffer[i]))
            {
    			coolsand_UI_single_line_input_box_insert_character(&gPenEditorCandidateInputBox,
    			candidate_buffer[i]);
            }         
		}
	}
	else
#endif
	for(i=0;i<num;i++)
	{
		coolsand_UI_single_line_input_box_insert_character(&gPenEditorCandidateInputBox, candidate_buffer[i]);
	}
	  //wangzl:070616 modify E
      /*shaozhw 20060610 add begin: for support sm_pinyin and sm_stroke */
      #ifdef __MMI_IME_SUPORT_SM_PINYIN_STROKE__
          RegisterCandidateInputBoxArrowKeys(TRUE);
      #endif
      /*shaozhw 20060610 add end */
      set_leading_zero(temp);
   }
   
#ifdef TP_USE_NAVIGATE_KEYS
	mmi_pen_editor_reset_candidate_position();
#endif
   
   coolsand_UI_single_line_input_box_goto_first_character(&gPenEditorCandidateInputBox);
   coolsand_UI_single_line_input_box_next(&gPenEditorCandidateInputBox);
   gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_STATE_MULTITAP;
   gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_FULL_HEIGHT_BLOCK_CURSOR;
   gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_CHARACTER_HIGHLIGHT;
   coolsand_UI_lock_double_buffer();
   mmi_pen_editor_hide_candidate_area();

   /*shaozhw 20060610 add begin: for support sm_pinyin and sm_stroke */
   #ifdef __MMI_IME_SUPORT_SM_PINYIN_STROKE__
        gPenEditorCandidateInputBox.y = UI_device_height - gPenEditorCandidateInputBox.height - MMI_softkey_height;
   #endif
   
   /*shaozhw 20060610 add end */
   //coolsand_UI_show_single_line_input_box(&gPenEditorCandidateInputBox);
   mmi_pen_editor_show_candidate_area(MMI_FALSE, PEN_EDITOR_CANDIDATE_ON_AND_FOCUS);
   coolsand_UI_unlock_double_buffer();
   coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
   return num;
}

/* --------------------------------------------------------------------*/
/** gbChar2Unicode
 * @brief Translate a encoding GB char to unicode.
 * @TODO:This function should not be here. Move it to some utility directories.
 * 
 * @param gb_char INPUT.
 * @param unicode_char OUTPUT.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
BOOL gbChar2Unicode( UINT16* gb_char, UINT16* unicode_char)
{ 
    UINT8* out;
    UINT32 outLen;
    UINT8 in[2];

    in[0] = ( *gb_char & 0xff00) >> 8;
    in[1] = ( *gb_char & 0x00ff);
    ML_LocalLanguage2Unicode( in, 2,
            &out, &outLen, ML_CP936);
    if(out)
    {
        *unicode_char = out[1] + (out[0]<<8);
        OslMfree( out);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_input_selection_character
* DESCRIPTION
*   \brief This function is to input selected characters.
*
* PARAMETERS
*  \param c : the character to input
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_input_selection_character(UI_character_type c)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
}
MMI_BOOL mmi_pen_editor_check_input(UI_character_type c)
{
   if (test_change_input_mode)
   {
	   if (0 == test_change_input_mode() && UI_TEST_UCS2_CHARACTER(c))
	   {
	      return MMI_FALSE;
	   }
   }
   return MMI_TRUE;
}

#if defined(__MMI_GB__)
extern single_line_input_box gbCompositionArea;
extern single_line_input_box gbCandidateArea;
#endif
/*****************************************************************************
 * FUNCTION
 *  mmi_pen_editor_pinyin_set_syllable_by_position
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 mmi_pen_editor_pinyin_set_syllable_by_position(void)
{
#if defined(__MMI_GB__)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int i = 0;
    UI_buffer_type position;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (position = gbCompositionArea.text;;)
    {
        UI_character_type ch, ch1, ch2;

        ch1 = *(position);
        ch2 = *(position + 1);
        ch = ch1 | (ch2 << 8);
        if (ch == 0x20)
        {
            i++;
        }
        if (position == gbCompositionArea.current_text_p)
        {
            if (0 == ch || 0x20 == ch)
            {
                i--;
            }
            return i;
        }
        position += 2;
    }
#else
    return 0;
#endif
}

/*****************************************************************************
 * FUNCTION
 *  mmi_pen_editor_pinyin_set_syllable_by_position
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 mmi_pen_editor_pinyin_set_candidate_by_position(void)
{
#if defined(__MMI_GB__)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int i = 0;
    UI_buffer_type position;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    for (position = gbCandidateArea.text;;)
    {
      
        if (position == gbCandidateArea.current_text_p)
        {
            return i;
        }
		
        i++;
        position += 2;
    }
#else
    return 0;
#endif
}
/*****************************************************************************
 * FUNCTION
 *  mmi_pen_editor_pinyin_set_syllable_by_position
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 mmi_pen_editor_SABC_Sabc_set_candidate_by_position(void)
{
#if defined(__MMI_GB__)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int i = 0;
    UI_buffer_type position;
    U16 EnisCandidateCount = 0;
    extern U16 GetEnisCandidateCount(void);
    EnisCandidateCount = GetEnisCandidateCount();
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (position = gbCandidateArea.text;;)
    {
      
        if (position == gbCandidateArea.current_text_p)
        {
          if((i/2 + 1) >= EnisCandidateCount)
          {
            return EnisCandidateCount;
          }
          else
          {
            return (i/2 + 1);
          }
        }
		
        i++;
        position += 2;
    }
#else
    return 0;
#endif
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_pen_down_handler
* DESCRIPTION
*   \brief This function is to handle pen down event.
*
* PARAMETERS
*  \param pos : the point of pen down
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
#if !defined(__MMI_HANDWRITING_PAD__)
void mmi_pen_editor_pen_down_handler(mmi_pen_point_struct pos)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	s32 no_of_control_set = 0;
	mmi_gui_single_line_input_box_pen_enum single_line_input_box_event;
	mmi_gui_multi_line_input_box_pen_enum multi_line_input_box_event;
	mmi_gui_EMS_input_box_pen_enum EMS_input_box_event;
	mmi_gui_pen_event_param_struct vk_para;
	mmi_gui_virtual_keyboard_pen_enum vk_event;
	S32 multitap_index=0;
	BOOL ret = MMI_FALSE;
#if defined(__MMI_GB__)
	U8 syllableIndex = 0; //wangzl:add
	U8 candidateIndex = 0;
#endif    
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	TBM_ENTRY(0x2827);
	//wangzl:add B
	mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: mmi_pen_editor_pen_down_handler");


//if in handwriting process no handle pen event
        if(mmi_pen_in_handwriting_process())    
        {
            return ;
        }
      
 #if defined(__MMI_GB__)   
	if(gb_is_pen_in_composition_area(pos))
	{
		mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: pen down in composition area");
		ret = coolsand_UI_single_line_input_box_translate_pen_event(&gbCompositionArea,MMI_PEN_EVENT_DOWN,pos.x,pos.y,&single_line_input_box_event);
		if (ret)
		{
			coolsand_UI_show_single_line_input_box_ext(&gbCompositionArea, pos.x, pos.y);
			coolsand_UI_lock_double_buffer();
			/* /TODO : Jump to the index */
			if ((INPUT_MODE_TR_BOPOMO == MMI_current_input_mode ||INPUT_MODE_SM_PINYIN == MMI_current_input_mode) )
			{
				syllableIndex= mmi_pen_editor_pinyin_set_syllable_by_position();
				mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: syllableindex is:%d",syllableIndex);
				//gb_set_syllable_by_index(syllableIndex);
				gb_display_syllable_by_index(syllableIndex);
			}
			coolsand_UI_unlock_double_buffer();
			coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
			TBM_EXIT(0x2827);
			return;
		}
		gPenEditorActiveControl = PEN_EDITOR_CONTROL_NONE;
		TBM_EXIT(0x2827);
		return;
	}
	else if(gb_is_pen_in_candidate_area(pos))
	{
		mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: pen down in candidate area");

		ret = coolsand_UI_single_line_input_box_translate_pen_event(&gbCandidateArea,MMI_PEN_EVENT_DOWN,pos.x,pos.y,&single_line_input_box_event);
		if (ret)
		{
			coolsand_UI_show_single_line_input_box_ext(&gbCandidateArea, pos.x, pos.y);
			coolsand_UI_single_line_input_box_next(&gbCandidateArea);
			coolsand_UI_lock_double_buffer();
			/* /TODO : Jump to the index */
			if ((INPUT_MODE_TR_BOPOMO == MMI_current_input_mode ||INPUT_MODE_SM_PINYIN == MMI_current_input_mode||
			INPUT_MODE_SM_STROKE == MMI_current_input_mode))  //add by chenq for bug 6094 
			{
				candidateIndex= mmi_pen_editor_pinyin_set_candidate_by_position();
				mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: chinese candidateindex is:%d",candidateIndex);
				if(candidateIndex > 0)
				{
					gb_set_candidate_by_index(candidateIndex - 1);
					//gb_input_character_by_candidate_index(candidateIndex - 1);
				}
			}
			else if(INPUT_MODE_SMART_UPPERCASE_ABC == MMI_current_input_mode ||INPUT_MODE_SMART_LOWERCASE_ABC == MMI_current_input_mode) 
			{
				candidateIndex= mmi_pen_editor_SABC_Sabc_set_candidate_by_position();
				mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: english candidateindex is:%d",candidateIndex);
				if(candidateIndex > 0)
				{
					gb_set_candidate_by_index(candidateIndex - 1);
				}

			}

			coolsand_UI_unlock_double_buffer();
			coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
			TBM_EXIT(0x2827);
			return;
		}
		gPenEditorActiveControl = PEN_EDITOR_CONTROL_NONE;
		TBM_EXIT(0x2827);
		return;
	}
#endif
    
	//wangzl:add E
   for (;no_of_control_set < gPenEditorControlSet.NumberofControlSets;no_of_control_set++)
   {
      switch (gPenEditorControlSet.PenEditorControlSet[no_of_control_set])
      {
         case PEN_EDITOR_CONTROL_INFORMATION_BAR:
             
            switch (gPenEditorInputBoxType)
            {
               case PEN_EDITOR_MULTILINE_INPUT_BOX:
               case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
               case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
               case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
               case PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX:
               case PEN_EDITOR_INLINE_TEXT:
                  ret = PEN_CHECK_BOUND(pos.x, pos.y, 
                           wgui_inputbox_information_bar_x, 
                           wgui_inputbox_information_bar_y, 
                           wgui_inputbox_information_bar_width, 
                           wgui_inputbox_information_bar_height);
                  break;
               case PEN_EDITOR_EMS_INPUT_BOX:
                  ret = PEN_CHECK_BOUND(pos.x, pos.y, 
                           wgui_EMS_inputbox_information_bar_x, 
                           wgui_EMS_inputbox_information_bar_y, 
                           wgui_EMS_inputbox_information_bar_width, 
                           wgui_EMS_inputbox_information_bar_height);
                  break;
               case PEN_EDITOR_DATE:
               case PEN_EDITOR_IP4:
               case PEN_EDITOR_TIME:
               case PEN_EDITOR_TIME_PERIOD:
	//	case PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX:
        //          break;
               default :
                  break;
            }

            if (ret)
            {
               gPenEditorActiveControl = PEN_EDITOR_CONTROL_INFORMATION_BAR;
//fix bug 13382 20/10/2009 by qiff
#if 0              
              if (gPenEditorSelectCandidate)
              {
                  coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
           //    COS_KillTimer(GetMmiTaskHandle(MOD_TP_TASK),TPTimerIdHandwritingConfirm );
               }
#endif
		TBM_EXIT(0x2827);
               return;
            }
            break;
         case PEN_EDITOR_CONTROL_MULTILINE_INPUT_BOX:
            if (MMI_multiline_inputbox.n_lines > PEN_EDITOR_GUI_MULTI_LINE_INPUT_BOX_LINES)
            {
               coolsand_UI_multi_line_input_box_set_pen_scroll_delay(&MMI_multiline_inputbox, PEN_EDITOR_GUI_MULTI_LINE_INPUT_BOX_SCROLL_DELAY);
            }
            ret = coolsand_UI_multi_line_input_box_translate_pen_event(&MMI_multiline_inputbox,MMI_PEN_EVENT_DOWN,pos.x,pos.y,&multi_line_input_box_event);
            if (ret)
            {
                
               if (gPenEditorSelectCandidate)
               {
                  coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
              //   COS_KillTimer(GetMmiTaskHandle(MOD_TP_TASK),TPTimerIdHandwritingConfirm );
               }
               gPenEditorActiveControl = PEN_EDITOR_CONTROL_MULTILINE_INPUT_BOX;
		TBM_EXIT(0x2827);
               return;
            }
            break;
         case PEN_EDITOR_CONTROL_EMS_INPUT_BOX:
            if (MMI_EMS_inputbox.n_lines > PEN_EDITOR_GUI_MULTI_LINE_INPUT_BOX_LINES)
            {
               coolsand_UI_EMS_input_box_set_pen_scroll_delay(&MMI_EMS_inputbox, PEN_EDITOR_GUI_MULTI_LINE_INPUT_BOX_SCROLL_DELAY);
            }
            ret = coolsand_UI_EMS_input_box_translate_pen_event(&MMI_EMS_inputbox,MMI_PEN_EVENT_DOWN,pos.x,pos.y,&EMS_input_box_event);
            if (ret)
            {
                
               if (gPenEditorSelectCandidate)
               {
                  coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
            //   COS_KillTimer(GetMmiTaskHandle(MOD_TP_TASK),TPTimerIdHandwritingConfirm );

               }
               gPenEditorActiveControl = PEN_EDITOR_CONTROL_EMS_INPUT_BOX;
		TBM_EXIT(0x2827);
               return;
            }
            break;
         case PEN_EDITOR_CONTROL_INLINE_MULTILINE_INPUT_BOX:
            if (MMI_multiline_inputbox.n_lines > PEN_EDITOR_GUI_MULTI_LINE_INPUT_BOX_LINES)
            {
               coolsand_UI_multi_line_input_box_set_pen_scroll_delay(&MMI_multiline_inputbox, PEN_EDITOR_GUI_MULTI_LINE_INPUT_BOX_SCROLL_DELAY);
            }
            ret = coolsand_UI_multi_line_input_box_translate_pen_event(&MMI_multiline_inputbox,MMI_PEN_EVENT_DOWN,pos.x,pos.y,&multi_line_input_box_event);
            if (ret)
            {
                
               if (gPenEditorSelectCandidate)
               {
                  coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
                // COS_KillTimer(GetMmiTaskHandle(MOD_TP_TASK),TPTimerIdHandwritingConfirm );
               }
               gPenEditorActiveControl = PEN_EDITOR_CONTROL_INLINE_MULTILINE_INPUT_BOX;
		TBM_EXIT(0x2827);
               return;
            }
            break;
         case PEN_EDITOR_CONTROL_SINGLE_LINE_INPU_BOX:
            ret = coolsand_UI_single_line_input_box_translate_pen_event(&MMI_singleline_inputbox,MMI_PEN_EVENT_DOWN,pos.x,pos.y,&single_line_input_box_event);
            if (ret)
            {
                
               if (gPenEditorSelectCandidate)
               {
                  coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
                //  COS_KillTimer(GetMmiTaskHandle(MOD_TP_TASK),TPTimerIdHandwritingConfirm );
               }
               gPenEditorActiveControl = PEN_EDITOR_CONTROL_SINGLE_LINE_INPU_BOX;
		TBM_EXIT(0x2827);
               return;
            }
            break;
         case PEN_EDITOR_CONTROL_INLINE_TEXT:
            ret = coolsand_UI_single_line_input_box_translate_pen_event(&MMI_inline_singleline_inputbox,MMI_PEN_EVENT_DOWN,pos.x,pos.y,&single_line_input_box_event);
            if (ret)
            {
                
               if (gPenEditorSelectCandidate)
               {
                  coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
               //   COS_KillTimer(GetMmiTaskHandle(MOD_TP_TASK),TPTimerIdHandwritingConfirm );
               }
               gPenEditorActiveControl = PEN_EDITOR_CONTROL_INLINE_TEXT;
		TBM_EXIT(0x2827);
               return;
            }
            break;
         case PEN_EDITOR_CONTROL_DATE:
            if (PEN_CHECK_BOUND(pos.x, pos.y,current_date_input->x,current_date_input->y, current_date_input->width, current_date_input->height))
            {
                
               if (gPenEditorSelectCandidate)
               {
                  coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
                // COS_KillTimer(GetMmiTaskHandle(MOD_TP_TASK),TPTimerIdHandwritingConfirm );
               }
               gPenEditorActiveControl = PEN_EDITOR_CONTROL_DATE;
		TBM_EXIT(0x2827);
               return;
            }
            break;
         case PEN_EDITOR_CONTROL_IP4:
            if (PEN_CHECK_BOUND(pos.x, pos.y,current_IP4_input->x,current_IP4_input->y, current_IP4_input->width, current_IP4_input->height))
            {
                
               if (gPenEditorSelectCandidate)
               {
                  coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
                //  COS_KillTimer(GetMmiTaskHandle(MOD_TP_TASK),TPTimerIdHandwritingConfirm );
               }
               gPenEditorActiveControl = PEN_EDITOR_CONTROL_IP4;
		TBM_EXIT(0x2827);
               return;
            }
            break;
         case PEN_EDITOR_CONTROL_TIME:
            if (PEN_CHECK_BOUND(pos.x, pos.y,current_time_input->x,current_time_input->y, current_time_input->width, current_time_input->height))
            {
                
               if (gPenEditorSelectCandidate)
               {
                  coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
                  //COS_KillTimer(GetMmiTaskHandle(MOD_TP_TASK),TPTimerIdHandwritingConfirm );
               }
               gPenEditorActiveControl = PEN_EDITOR_CONTROL_TIME;
		TBM_EXIT(0x2827);
               return;
            }
            break;
         case PEN_EDITOR_CONTROL_TIME_PERIOD:
            if (PEN_CHECK_BOUND(pos.x, pos.y,current_time_period_input->x,current_time_period_input->y, current_time_period_input->width, current_time_period_input->height))
            {
                
               if (gPenEditorSelectCandidate)
               {
                  coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
             //    COS_KillTimer(GetMmiTaskHandle(MOD_TP_TASK),TPTimerIdHandwritingConfirm );
               }
               gPenEditorActiveControl = PEN_EDITOR_CONTROL_TIME_PERIOD;
		TBM_EXIT(0x2827);
               return;
            }
            break;
         case PEN_EDITOR_CONTROL_SCROLL_BAR:
            break;
         case PEN_EDITOR_CONTROL_VIRTUAL_KEYBOARD:
            if (PEN_EDITOR_VK_ON==gPenEditorVKState)
            {
               ret = coolsand_UI_virtual_keyboard_translate_pen_event(&MMI_virtual_keyboard, pos.x, pos.y, MMI_PEN_EVENT_DOWN, &vk_event, &vk_para);
            }
            else
            {
               break;
            }
            if (ret)
            {
               switch (vk_event)
               {
                  case GUI_VKBD_PEN_BAKSPACE:
                     break;
                  case GUI_VKBD_PEN_BRACKET:
                     break;
                  case GUI_VKBD_PEN_CAPSLOCK:
                     break;
                  case GUI_VKBD_PEN_CHAR_I:
                     break;
                  case GUI_VKBD_PEN_DISPLAY_AREA:
                     break;
                  case GUI_VKBD_PEN_HIDE:
                     break;
                  case GUI_VKBD_PEN_NEWLINE:
                     break;
                  case GUI_VKBD_PEN_NONE:
                     break;
                  case GUI_VKBD_PEN_SHOW:
                     break;
                  case GUI_VKBD_PEN_SPACE:
                     break;
                  case GUI_VKBD_PEN_SYMBPICKER:
                     break;
                  case GUI_VKBD_PEN_EUROSYMB:
                     break;
                  default :
                     MMI_ASSERT(0);
                     break;
               }
               gPenEditorActiveControl = PEN_EDITOR_CONTROL_VIRTUAL_KEYBOARD;
		TBM_EXIT(0x2827);
               return;
            }
            break;
         case PEN_EDITOR_CONTROL_CANDIDATE_AREA:
		if (gPenEditorSelectCandidate)
		{
			ret = coolsand_UI_single_line_input_box_translate_pen_event(&gPenEditorCandidateInputBox,MMI_PEN_EVENT_DOWN,pos.x,pos.y,&single_line_input_box_event);
			if (ret)
			{
				UI_character_type ch,ch1,ch2;
				coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
				//  COS_KillTimer(GetMmiTaskHandle(MOD_TP_TASK),TPTimerIdHandwritingConfirm );
				gPenEditorActiveControl = PEN_EDITOR_CONTROL_CANDIDATE_AREA;
				coolsand_UI_show_single_line_input_box_ext(&gPenEditorCandidateInputBox, pos.x, pos.y);
				coolsand_UI_single_line_input_box_next(&gPenEditorCandidateInputBox);
				ch1 = *(gPenEditorCandidateInputBox.current_text_p-2);
				ch2 = *(gPenEditorCandidateInputBox.current_text_p-1);
				ch = ch1 | (ch2 << 8);
				coolsand_UI_lock_double_buffer();
				if (mmi_pen_editor_check_input(ch))
				{
					switch (gPenEditorInputBoxType)
					{
						 
						case PEN_EDITOR_MULTILINE_INPUT_BOX:
						case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
						case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
							multiline_inputbox_multitap_input(ch);
							break;
						case PEN_EDITOR_EMS_INPUT_BOX:
							EMS_inputbox_multitap_input(ch);
							break;
						case PEN_EDITOR_INLINE_TEXT:
							inline_edit_singleline_inputbox_multitap_input(ch);
							break;
						default :
							break;
					}
				}
				else
				{
					 
					UI_editor_play_tone_cannot_insert();
				}
				//coolsand_UI_show_single_line_input_box(&gPenEditorCandidateInputBox);
				mmi_pen_editor_show_candidate_area( g_pen_editor_candidate_state.arrow_enable, g_pen_editor_candidate_state.candidate_state);
				coolsand_UI_unlock_double_buffer();
				coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
				TBM_EXIT(0x2827);
				return;
			}
			if (g_pen_editor_candidate_state.arrow_enable)
			{
				ret = PEN_CHECK_BOUND(	pos.x,pos.y,
										gPenEditorCandidateInputBox.x - (((gPenEditorCandidateInputBox.grid_width << 1) + gPenEditorCandidateInputBox.grid_width) >> 2) - 1,
										gPenEditorCandidateInputBox.y + (gPenEditorCandidateInputBox.height >> 3),
										gPenEditorCandidateInputBox.grid_width >> 1,
										((gPenEditorCandidateInputBox.height << 1) + gPenEditorCandidateInputBox.height) >> 2);
				if (ret)
				{
					g_pen_editor_candidate_state.pen_down_state = PEN_EDITOR_IN_LEFT_ARROW;
					gPenEditorActiveControl = PEN_EDITOR_CONTROL_CANDIDATE_AREA;
					coolsand_UI_lock_double_buffer();
					mmi_pen_editor_hide_candidate_area();
					coolsand_UI_show_single_line_input_box(&gPenEditorCandidateInputBox);
					if (PEN_EDITOR_ARROW_PRESS_UP == g_pen_editor_candidate_state.left_arrow_state)
					{
						g_pen_editor_candidate_state.left_arrow_state = PEN_EDITOR_ARROW_PRESS_DOWN;
					}
					mmi_pen_editor_draw_arrow(PEN_EDITOR_ARROW_DIRECTION_LEFT,
												g_pen_editor_candidate_state.left_arrow_state,
												PEN_EDITOR_ARROW_FOR_CANDIDATE);
					mmi_pen_editor_draw_arrow(PEN_EDITOR_ARROW_DIRECTION_RIGHT,
												g_pen_editor_candidate_state.right_arrow_state,
												PEN_EDITOR_ARROW_FOR_CANDIDATE);
					coolsand_UI_unlock_double_buffer();
					coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
					TBM_EXIT(0x2827);
					return;
				}
				ret = PEN_CHECK_BOUND(pos.x,pos.y,
										gPenEditorCandidateInputBox.x + gPenEditorCandidateInputBox.width + (gPenEditorCandidateInputBox.grid_width >> 2) - 1,
										gPenEditorCandidateInputBox.y + (gPenEditorCandidateInputBox.height >> 3),
										gPenEditorCandidateInputBox.grid_width >> 1,
										((gPenEditorCandidateInputBox.height << 1) + gPenEditorCandidateInputBox.height) >> 2);
				if (ret)
				{
					g_pen_editor_candidate_state.pen_down_state = PEN_EDITOR_IN_RIGHT_ARROW;
					gPenEditorActiveControl = PEN_EDITOR_CONTROL_CANDIDATE_AREA;
					coolsand_UI_lock_double_buffer();
					mmi_pen_editor_hide_candidate_area();
					coolsand_UI_show_single_line_input_box(&gPenEditorCandidateInputBox);
					if (PEN_EDITOR_ARROW_PRESS_UP == g_pen_editor_candidate_state.right_arrow_state)
					{
						g_pen_editor_candidate_state.right_arrow_state = PEN_EDITOR_ARROW_PRESS_DOWN;
					}
					mmi_pen_editor_draw_arrow(PEN_EDITOR_ARROW_DIRECTION_LEFT,
												g_pen_editor_candidate_state.left_arrow_state,
												PEN_EDITOR_ARROW_FOR_CANDIDATE);
					mmi_pen_editor_draw_arrow(
												PEN_EDITOR_ARROW_DIRECTION_RIGHT,
												g_pen_editor_candidate_state.right_arrow_state,
												PEN_EDITOR_ARROW_FOR_CANDIDATE);
					coolsand_UI_unlock_double_buffer();
					coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
					TBM_EXIT(0x2827);
					return;
				}
				ret = PEN_CHECK_BOUND(pos.x, pos.y,
										0,
										gPenEditorCandidateInputBox.y,
										UI_device_width,
										gPenEditorCandidateInputBox.height);
				if (ret)
				{
					g_pen_editor_candidate_state.pen_down_state = PEN_EDITOR_OUT_OF_RANGE;
					gPenEditorActiveControl = PEN_EDITOR_CONTROL_CANDIDATE_AREA;
					return;
				}
			}
		}
		break;
         case PEN_EDITOR_CONTROL_IME_BUTTON:
            if (ret)
            {
               gPenEditorActiveControl = PEN_EDITOR_CONTROL_IME_BUTTON;
		TBM_EXIT(0x2827);
               return;
            }
            break;
         case PEN_EDITOR_CONTROL_MULTITAP:
            if (MMI_TRUE == gPenEditorMultitapInputState)
            {
               ret = coolsand_UI_multitap_input_translate_pen_position(gPenEditorMultitapInput,pos.x,pos.y,&multitap_index);
            }
            if (ret)
            {
               gPenEditorActiveControl = PEN_EDITOR_CONTROL_MULTITAP;
		TBM_EXIT(0x2827);
               return;
            }
            break;
         case PEN_EDITOR_CONTROL_NONE:
            break;
         default:
            MMI_ASSERT(0);
      }
   }
   if (MMI_FALSE == ret)
   {
       
       
      gPenEditorActiveControl = PEN_EDITOR_CONTROL_NONE;
      mmi_wgui_general_pen_down_hdlr(pos);
      if (gPenEditorSelectCandidate && !g_pen_editor_hold_confirm)
      {
         coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
     //  COS_KillTimer(GetMmiTaskHandle(MOD_TP_TASK),TPTimerIdHandwritingConfirm );
         mmi_pen_editor_confirm_selection();
      }
   }
   TBM_EXIT(0x2827);
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_pen_up_handler
* DESCRIPTION
*   \brief This function is to handle pen up event.
*
* PARAMETERS
*  \param pos : the point of pen up
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_pen_up_handler(mmi_pen_point_struct pos)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   //s32 no_of_control_set = 0;
   mmi_gui_single_line_input_box_pen_enum single_line_input_box_event;
   mmi_gui_multi_line_input_box_pen_enum multi_line_input_box_event;
   mmi_gui_EMS_input_box_pen_enum EMS_input_box_event;
   mmi_gui_pen_event_param_struct vk_para;
   mmi_gui_virtual_keyboard_pen_enum vk_event;
   BOOL ret = MMI_FALSE;
   S32 multitap_index=0;
#if defined(__MMI_GB__)
   U8 syllableIndex = 0; //wangzl:add
   U8 candidateIndex = 0;//wangzl:add
#endif   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   	TBM_ENTRY(0x2828);
   	//wangzl:add B
   	mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR:  mmi_pen_editor_pen_up_handler gPenEditorActiveControl is:%d", gPenEditorActiveControl);
//if in handwriting process no handle pen event
        if(mmi_pen_in_handwriting_process())    
        {
            return ;
        }
#if defined(__MMI_GB__)
        
    if(gb_is_pen_in_composition_area(pos))
	{
		mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: pen up in composition area");
        
		#ifdef __GB_COMPOSITION_AREA_WITH_ARROW__
		if(pos.x < gbCompositionArea.x)
		{
			gb_composition_area_left_arrow_pen_up_handler();
		}
		else if(pos.x > (gbCompositionArea.x+gbCompositionArea.width))
		{
			gb_composition_area_right_arrow_pen_up_handler();
		}
		else
		#endif
		{
			ret = coolsand_UI_single_line_input_box_translate_pen_event(&gbCompositionArea,MMI_PEN_EVENT_UP,pos.x,pos.y,&single_line_input_box_event);
			if (ret)
			{
				coolsand_UI_show_single_line_input_box_ext(&gbCompositionArea, pos.x, pos.y);
				coolsand_UI_lock_double_buffer();
				/* /TODO : Jump to the index */
				if ((INPUT_MODE_TR_BOPOMO == MMI_current_input_mode ||INPUT_MODE_SM_PINYIN == MMI_current_input_mode) )
				{
					syllableIndex= mmi_pen_editor_pinyin_set_syllable_by_position();
					mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: syllableindex is:%d",syllableIndex);
					gb_set_syllable_by_index(syllableIndex);
					//gb_display_syllable_by_index(syllableIndex);
				}
				coolsand_UI_unlock_double_buffer();
				coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
				TBM_EXIT(0x2828);
				return;
			}
		}
		gPenEditorActiveControl = PEN_EDITOR_CONTROL_NONE;
		TBM_EXIT(0x2828);
		return;
	}
	else if(gb_is_pen_in_candidate_area(pos))
	{
		mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: pen up in candidate area");

#ifdef __GB_CANDIDATE_AREA_WITH_ARROW__
		if(pos.x < gbCandidateArea.x)
		{
			gb_candidate_area_left_arrow_pen_up_handler();
		}
		else if(pos.x > (gbCandidateArea.x + gbCandidateArea.width))
		{
			gb_candidate_area_right_arrow_pen_up_handler();
		}
		else
#endif
		{
			ret = coolsand_UI_single_line_input_box_translate_pen_event(&gbCandidateArea,MMI_PEN_EVENT_UP,pos.x,pos.y,&single_line_input_box_event);
			if (ret)
			{
				coolsand_UI_show_single_line_input_box_ext(&gbCandidateArea, pos.x, pos.y);
				coolsand_UI_single_line_input_box_next(&gbCandidateArea);
				coolsand_UI_lock_double_buffer();
			/* /TODO : Jump to the index */
				if ((INPUT_MODE_TR_BOPOMO == MMI_current_input_mode ||INPUT_MODE_SM_PINYIN == MMI_current_input_mode||
				INPUT_MODE_SM_STROKE == MMI_current_input_mode))  //add by chenq for bug 6094 
				{
					candidateIndex= mmi_pen_editor_pinyin_set_candidate_by_position();
					mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: chinese candidateindex is:%d",candidateIndex);
					if(candidateIndex > 0)
					{
						//gb_set_candidate_by_index(candidateIndex - 1);
						gb_input_character_by_candidate_index(candidateIndex - 1);
					}
				}
				else if(INPUT_MODE_SMART_UPPERCASE_ABC == MMI_current_input_mode ||INPUT_MODE_SMART_LOWERCASE_ABC == MMI_current_input_mode) 
				{
					candidateIndex= mmi_pen_editor_SABC_Sabc_set_candidate_by_position();
					mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR:  english candidateindex is:%d",candidateIndex);
					if(candidateIndex > 0)
					{
						gb_input_character_by_candidate_index(candidateIndex - 1);
					}

				}
				coolsand_UI_unlock_double_buffer();
				coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
				TBM_EXIT(0x2828);
				return;
			}
		}
		gPenEditorActiveControl = PEN_EDITOR_CONTROL_NONE;
		TBM_EXIT(0x2828);
		return;
	}
#endif
	//wangzl:add E
    
   switch (gPenEditorActiveControl)
   {
      case PEN_EDITOR_CONTROL_INFORMATION_BAR:
             
            switch (gPenEditorInputBoxType)
            {
               case PEN_EDITOR_MULTILINE_INPUT_BOX:
               case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
               case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
               case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
               case PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX:
               case PEN_EDITOR_INLINE_TEXT:
                  ret = PEN_CHECK_BOUND(pos.x, pos.y, 
                           wgui_inputbox_information_bar_x, 
                           wgui_inputbox_information_bar_y, 
                           wgui_inputbox_information_bar_width, 
                           wgui_inputbox_information_bar_height);
                  break;
               case PEN_EDITOR_EMS_INPUT_BOX:
                  ret = PEN_CHECK_BOUND(pos.x, pos.y, 
                           wgui_EMS_inputbox_information_bar_x, 
                           wgui_EMS_inputbox_information_bar_y, 
                           wgui_EMS_inputbox_information_bar_width, 
                           wgui_EMS_inputbox_information_bar_height);
                  break;
               case PEN_EDITOR_DATE:
               case PEN_EDITOR_IP4:
               case PEN_EDITOR_TIME:
               case PEN_EDITOR_TIME_PERIOD:
	//	case PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX:
       //           break;
               default :
                  break;
            }
         if (!ret)
         {
            break;
         }
         if (gPenEditorSelectCandidate)
         {
            mmi_pen_editor_confirm_selection();
         }
	 if( GetKeyHandler(KEY_POUND,KEY_EVENT_DOWN) == mmi_pen_editor_switch_input_method )
	 {
		mmi_pen_editor_switch_input_method();
	 }
         if (ret)
         {
         	TBM_EXIT(0x2828);
            return;
         }
         break;
      case PEN_EDITOR_CONTROL_MULTILINE_INPUT_BOX:
         ret = coolsand_UI_multi_line_input_box_translate_pen_event(&MMI_multiline_inputbox,MMI_PEN_EVENT_UP,pos.x,pos.y,&multi_line_input_box_event);
         if (ret)
         {
            if (gPenEditorSelectCandidate&&gPenEditorVKText.TextLength==0)
            {
               mmi_pen_editor_confirm_selection();
            }
            if (GUI_MULTI_LINE_INPUT_BOX_PEN_SCROLL_BAR != multi_line_input_box_event)
            {
               coolsand_UI_show_multi_line_input_box_ext(&MMI_multiline_inputbox, pos.x, pos.y);
               /*shaozhw 20060610 modify begin: for support sm_pinyin and sm_stroke */
/*wuzc Del Start For SIMM Ver: MT1.9.2  on 2007-1-18 16:25 */
               #if 0
               #ifdef __MMI_IME_SUPORT_SM_PINYIN_STROKE__
                if((INPUT_MODE_SM_STROKE == MMI_current_input_mode) || (INPUT_MODE_SM_PINYIN== MMI_current_input_mode))
                {
                    SetT9nCursorByEditorPen();
                }
               #endif
               #endif
/*wuzc Del End  For SIMM Ver: MT1.9.2  on 2007-1-18 16:25 */
               /*shaozhw 20060610 modify end */
               redraw_multiline_inputbox();
            }
		TBM_EXIT(0x2828);
            return;
         }
         break;
      case PEN_EDITOR_CONTROL_EMS_INPUT_BOX:
         ret = coolsand_UI_EMS_input_box_translate_pen_event(&MMI_EMS_inputbox,MMI_PEN_EVENT_UP,pos.x,pos.y,&EMS_input_box_event);
         if (ret)
         {
            if (gPenEditorSelectCandidate&&gPenEditorVKText.TextLength==0)
            {
               mmi_pen_editor_confirm_selection();
            }
            if (GUI_EMS_INPUT_BOX_PEN_SCROLL_BAR != EMS_input_box_event)
            {
               coolsand_UI_show_EMS_input_box_ext(&MMI_EMS_inputbox, pos.x, pos.y);
               redraw_EMS_inputbox();
            }
		TBM_EXIT(0x2828);
            return;
         }
         break;
      case PEN_EDITOR_CONTROL_INLINE_MULTILINE_INPUT_BOX:
         ret = coolsand_UI_multi_line_input_box_translate_pen_event(&MMI_multiline_inputbox,MMI_PEN_EVENT_UP,pos.x,pos.y,&multi_line_input_box_event);
         if (ret)
         {
            if (gPenEditorSelectCandidate&&gPenEditorVKText.TextLength==0)
            {
               mmi_pen_editor_confirm_selection();
            }
            if (GUI_MULTI_LINE_INPUT_BOX_PEN_SCROLL_BAR != multi_line_input_box_event)
            {
               coolsand_UI_show_multi_line_input_box_ext(&MMI_multiline_inputbox, pos.x, pos.y);
            }
            redraw_multiline_inputbox();
		TBM_EXIT(0x2828);
            return;
         }
         break;
      case PEN_EDITOR_CONTROL_SINGLE_LINE_INPU_BOX:
         ret = coolsand_UI_single_line_input_box_translate_pen_event(&MMI_singleline_inputbox,MMI_PEN_EVENT_UP,pos.x,pos.y,&single_line_input_box_event);
         if (ret)
         {
            if (gPenEditorSelectCandidate)
            {
               mmi_pen_editor_confirm_selection();
            }
		coolsand_UI_show_single_line_input_box_ext(&MMI_singleline_inputbox, pos.x, pos.y);
		 
		coolsand_UI_single_line_input_box_locate_cursor(&MMI_singleline_inputbox);
		redraw_singleline_inputbox();
		TBM_EXIT(0x2828);
            return;
         }
         break;
      case PEN_EDITOR_CONTROL_INLINE_TEXT:
         ret = coolsand_UI_single_line_input_box_translate_pen_event(&MMI_inline_singleline_inputbox,MMI_PEN_EVENT_UP,pos.x,pos.y,&single_line_input_box_event);
         if (ret)
         {
            if (gPenEditorSelectCandidate)
            {
               mmi_pen_editor_confirm_selection();
            }
            coolsand_UI_show_single_line_input_box_ext(&MMI_inline_singleline_inputbox, pos.x, pos.y);
			 
			coolsand_UI_single_line_input_box_locate_cursor(&MMI_inline_singleline_inputbox);
            show_inline_singleline_inputbox();
		TBM_EXIT(0x2828);
            return;
         }
         break;
      case PEN_EDITOR_CONTROL_DATE:
         if (gPenEditorSelectCandidate)
         {
            mmi_pen_editor_confirm_selection();
         }
         date_input_move_to_x_y(current_date_input, pos.x, pos.y);
         if (ret)
         {
         	TBM_EXIT(0x2828);
            return;
         }
         break;
      case PEN_EDITOR_CONTROL_IP4:
         if (gPenEditorSelectCandidate)
         {
            mmi_pen_editor_confirm_selection();
         }
         IP4_input_move_to_x_y(current_IP4_input, pos.x, pos.y);
         if (ret)
         {
         	TBM_EXIT(0x2828);
            return;
         }
         break;
      case PEN_EDITOR_CONTROL_TIME:
         if (gPenEditorSelectCandidate)
         {
            mmi_pen_editor_confirm_selection();
         }
         time_input_move_to_x_y(current_time_input, pos.x, pos.y);
         if (ret)
         {
          TBM_EXIT(0x2828);
            return;
         }
         break;
      case PEN_EDITOR_CONTROL_TIME_PERIOD:
         if (gPenEditorSelectCandidate)
         {
            mmi_pen_editor_confirm_selection();
         }
         time_period_input_move_to_x_y(current_time_period_input, pos.x, pos.y);
         if (ret)
         {
           TBM_EXIT(0x2828);
            return;
         }
         break;
      case PEN_EDITOR_CONTROL_SCROLL_BAR:
         break;
      case PEN_EDITOR_CONTROL_VIRTUAL_KEYBOARD:
         ret = coolsand_UI_virtual_keyboard_translate_pen_event(&MMI_virtual_keyboard, pos.x, pos.y, MMI_PEN_EVENT_UP, &vk_event, &vk_para);
	if(mmi_pen_editor_vk_pre_handler != NULL)
	{
		mmi_pen_editor_vk_pre_handler();
		mmi_pen_editor_vk_pre_handler = NULL;
		mmi_pen_editor_hw_pre_handler = NULL;
	}
	  if (ret)
         {
            switch (vk_event)
            {
               case GUI_VKBD_PEN_BAKSPACE:
                  mmi_pen_editor_right_soft_key_handler();
                  break;
               case GUI_VKBD_PEN_BRACKET:
                  mmi_pen_editor_bracket();
                  break;
               case GUI_VKBD_PEN_CAPSLOCK:
                  mmi_pen_editor_vk_caps_lock();
                  break;
               case GUI_VKBD_PEN_CHAR_I:
                  mmi_pen_editor_vk_insertcharacter(vk_para);
                  break;
               case GUI_VKBD_PEN_DISPLAY_AREA:
                  break;
               case GUI_VKBD_PEN_HIDE:
                  mmi_pen_editor_vk_hide();
                  break;
               case GUI_VKBD_PEN_NEWLINE:
                  mmi_pen_editor_new_line();
                  break;
               case GUI_VKBD_PEN_NONE:
                  ret = MMI_FALSE;
                  break;
               case GUI_VKBD_PEN_SHOW:
                  mmi_pen_editor_vk_show();
                  break;
               case GUI_VKBD_PEN_SPACE:
                  /*Code for ??*/
                  mmi_pen_editor_input_character(' ', MMI_FALSE);
                  break;
               case GUI_VKBD_PEN_SYMBPICKER:
                  mmi_pen_editor_vk_symbol_picker();
                  break;
               #ifdef __MMI_VKBD_EUROPEAN_SUPPORT__
                  case GUI_VKBD_PEN_EUROSYMB:
                  mmi_pen_editor_vk_euro_symbol_picker();
                  break;
               #endif
               default :
                  MMI_ASSERT(0);
            }
            if (ret)
            {
            	TBM_EXIT(0x2828);
               return;
            }
         }
         break;
      case PEN_EDITOR_CONTROL_CANDIDATE_AREA:
         if (gPenEditorSelectCandidate)
         {
		if (g_pen_editor_candidate_state.arrow_enable && PEN_EDITOR_MOVE_OUT != g_pen_editor_candidate_state.pen_down_state)
		{
			if (PEN_EDITOR_ARROW_PRESS_DOWN == g_pen_editor_candidate_state.right_arrow_state)
			{
				ret = PEN_CHECK_BOUND(pos.x,pos.y,
										gPenEditorCandidateInputBox.x + gPenEditorCandidateInputBox.width + (gPenEditorCandidateInputBox.grid_width >> 2) - 1,
										gPenEditorCandidateInputBox.y + (gPenEditorCandidateInputBox.height >> 3),
										gPenEditorCandidateInputBox.grid_width >> 1,
										((gPenEditorCandidateInputBox.height << 1) + gPenEditorCandidateInputBox.height) >> 2);
				if (ret)
				{
					mmi_pen_editor_get_candidates_next_page();
				}
			}
			else if (PEN_EDITOR_ARROW_PRESS_DOWN == g_pen_editor_candidate_state.left_arrow_state)
			{
				ret = PEN_CHECK_BOUND(pos.x, pos.y,
										gPenEditorCandidateInputBox.x - (((gPenEditorCandidateInputBox.grid_width << 1) + gPenEditorCandidateInputBox.grid_width) >> 2) - 1,
										gPenEditorCandidateInputBox.y + (gPenEditorCandidateInputBox.height >> 3),
										gPenEditorCandidateInputBox.grid_width >> 1,
										((gPenEditorCandidateInputBox.height << 1) + gPenEditorCandidateInputBox.height) >> 2);
				if (ret)
				{
					mmi_pen_editor_get_candidates_prev_page();
				}
			}
		}
			 
            ret = coolsand_UI_single_line_input_box_translate_pen_event(&gPenEditorCandidateInputBox,MMI_PEN_EVENT_UP,pos.x,pos.y,&single_line_input_box_event);
        	if (ret)
            {
               UI_character_type ch,ch1,ch2;
               coolsand_UI_lock_double_buffer();
                
               gPenEditorCandidateInputBox.text_offset_x = 0;
               coolsand_UI_show_single_line_input_box_ext(&gPenEditorCandidateInputBox, pos.x, pos.y);
               coolsand_UI_single_line_input_box_next(&gPenEditorCandidateInputBox);
               ch1 = *(gPenEditorCandidateInputBox.current_text_p-2);
               ch2 = *(gPenEditorCandidateInputBox.current_text_p-1);
               ch = ch1 | (ch2 << 8);
               mmi_pen_editor_input_character(ch, MMI_FALSE);
               switch (gPenEditorInputBoxType)
               {
                  case PEN_EDITOR_EMS_INPUT_BOX:
                     wgui_set_EMS_inputbox_RSK();
                     break;
                  case PEN_EDITOR_MULTILINE_INPUT_BOX:
                  case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
                     wgui_set_RSK();
                     break;
                  case PEN_EDITOR_INLINE_TEXT:
                     mmi_pen_editor_set_right_soft_key_inline_text();
                     break;
                  case PEN_EDITOR_DATE:
                  case PEN_EDITOR_IP4:
                  case PEN_EDITOR_TIME:
                  case PEN_EDITOR_TIME_PERIOD:
                        set_right_softkey_label(wgui_inline_edit_get_RSK_string());
                        set_right_softkey_icon(NULL);
                        register_right_softkey_handler();
                        set_right_softkey_function(execute_wgui_inline_edit_RSK_function,KEY_EVENT_UP);
                        redraw_right_softkey();
                     break;
                  default :
                     break;
               }
               /*Temp Solution for checking Chinese Characters*/
               if (ch>=0x4e00 && ch <=0x9fff)
               {
                   
                  mmi_pen_editor_reset_candidate_index();
                  if (mmi_pen_editor_get_candidates_by_phrase(ch))
                  {
                     //gPenEditorSelectCandidate = MMI_TRUE;
                     mmi_pen_editor_set_candidate_state(MMI_TRUE);
                     /*Temp Solution for LSK Text Update*/
                      
                     //mmi_pen_editor_store_LSK_Text();
                     mmi_pen_editor_set_left_soft_key_select();
                     set_left_softkey_function(mmi_pen_editor_left_soft_key_handler,KEY_EVENT_UP);
                     set_left_softkey_function(mmi_pen_editor_hold_confirm,KEY_EVENT_DOWN);
                     switch (gPenEditorInputBoxType)
                     {
                        case PEN_EDITOR_EMS_INPUT_BOX:
                        case PEN_EDITOR_MULTILINE_INPUT_BOX:
                        case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
                        case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
                           mmi_pen_editor_register_arrow_keys();
                           break;
                        default :
                           break;
                     }
                     if (gPenEditorCandidateInputBox.y >= (MMI_multitap_y - 1))
                     {
                        gPenEditorVKState = PEN_EDITOR_VK_OFF;
                     }
                     mmi_pen_editor_set_right_softkey(PEN_EDITOR_RSK_BY_STATE);
                  }
               }
		#ifdef __MMI_HAND_WRITING__
               mmi_pen_editor_change_handwriting_area();
		#endif
               coolsand_UI_unlock_double_buffer();
               coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
            }
            //add by chenq for bug 6800 200809201400 B
          //  else
         //   {
         //     mmi_pen_editor_confirm_selection();
         //   }
            //add by chenq for bug 6800 200809201400 E
         }
         break;
      case PEN_EDITOR_CONTROL_IME_BUTTON:
         if (ret)
         {
            
         }
         break;
      case PEN_EDITOR_CONTROL_MULTITAP:
         ret = coolsand_UI_multitap_input_translate_pen_position(gPenEditorMultitapInput,pos.x,pos.y,&multitap_index);
         if (ret)
         {
            
         }
         break;
      case PEN_EDITOR_CONTROL_NONE:
         break;
      default:
         MMI_ASSERT(0);
         break;
   }
   if (MMI_FALSE == ret)
   {
       
      //coolsand_UI_lock_double_buffer();
      mmi_wgui_general_pen_up_hdlr(pos);
      //if (gPenEditorSelectCandidate
      //	&&((PEN_EDITOR_EMS_INPUT_BOX!=gPenEditorInputBoxType
      //			&&PEN_EDITOR_MULTILINE_INPUT_BOX!=gPenEditorInputBoxType
      //			&&PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX!=gPenEditorInputBoxType)
      //		||get_multitap_state())
      //	)
      //{
      //   mmi_pen_editor_confirm_selection();
      //}
      //coolsand_UI_unlock_double_buffer();
      //coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
   }

//qiff add re-register for VK # handler changed by another
	if (wgui_handle_inputbox_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN]||
	   handle_category28_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN]||
	   wgui_handle_qsearch_singleline_inputbox_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN]||
	   wgui_handle_singleline_inputbox_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN]||
	   wgui_handle_inline_singleline_inputbox_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN])
	{
		SetKeyHandler(mmi_pen_editor_switch_input_method,KEY_POUND,KEY_EVENT_DOWN);
	}
   TBM_EXIT(0x2828);
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_pen_long_tap_handler
* DESCRIPTION
*   \brief This function is to handle pen long tap event.
*
* PARAMETERS
*  \param pos : the point of pen long tap
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_pen_long_tap_handler(mmi_pen_point_struct pos)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   mmi_gui_multi_line_input_box_pen_enum multi_line_input_box_event;
   mmi_gui_EMS_input_box_pen_enum EMS_input_box_event;
   S32 multitap_index=0;
   BOOL ret = MMI_FALSE;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

//if in handwriting process no handle pen event
    if(mmi_pen_in_handwriting_process())    
    {
        return ;
    }

   
   switch (gPenEditorActiveControl)
   {
      case PEN_EDITOR_CONTROL_INFORMATION_BAR:
         if (ret)
	 {
            
	 }
         break;
      case PEN_EDITOR_CONTROL_MULTILINE_INPUT_BOX:
         ret = coolsand_UI_multi_line_input_box_translate_pen_event(&MMI_multiline_inputbox,MMI_PEN_EVENT_LONG_TAP,pos.x,pos.y,&multi_line_input_box_event);
         if (ret)
         {
            
         }
         break;
      case PEN_EDITOR_CONTROL_EMS_INPUT_BOX:
         ret = coolsand_UI_EMS_input_box_translate_pen_event(&MMI_EMS_inputbox,MMI_PEN_EVENT_LONG_TAP,pos.x,pos.y,&EMS_input_box_event);
         if (ret)
         {
            
         }
         break;
      case PEN_EDITOR_CONTROL_INLINE_MULTILINE_INPUT_BOX:
         if (ret)
         {
            
         }
         break;
      case PEN_EDITOR_CONTROL_SINGLE_LINE_INPU_BOX:
         if (ret)
         {
            
         }
         break;
      case PEN_EDITOR_CONTROL_INLINE_TEXT:
         if (ret)
         {
            
         }
         break;
      case PEN_EDITOR_CONTROL_DATE:
         if (ret)
         {
            
         }
         break;
      case PEN_EDITOR_CONTROL_IP4:
         if (ret)
         {
            
         }
         break;
      case PEN_EDITOR_CONTROL_TIME:
         if (ret)
         {
            
         }
         break;
      case PEN_EDITOR_CONTROL_TIME_PERIOD:
         if (ret)
         {
            
         }
         break;
      case PEN_EDITOR_CONTROL_SCROLL_BAR:
         break;
      case PEN_EDITOR_CONTROL_VIRTUAL_KEYBOARD:
         if (ret)
         {
            
         }
         break;
      case PEN_EDITOR_CONTROL_CANDIDATE_AREA:
         if (ret)
         {
            
         }
         break;
      case PEN_EDITOR_CONTROL_IME_BUTTON:
         if (ret)
         {
            
         }
         break;
      case PEN_EDITOR_CONTROL_MULTITAP:
         ret = coolsand_UI_multitap_input_translate_pen_position(gPenEditorMultitapInput,pos.x,pos.y,&multitap_index);
         if (ret)
         {
            
         }
         break;
      case PEN_EDITOR_CONTROL_NONE:
         break;
      default:
         MMI_ASSERT(0);
         break;
   }
   if (MMI_FALSE == ret)
   {
      mmi_wgui_general_pen_long_tap_hdlr(pos);
   }
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_pen_repeat_handler
* DESCRIPTION
*   \brief This function is to handle pen repeat event.
*
* PARAMETERS
*  \param pos : the point of pen repeat
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_pen_repeat_handler(mmi_pen_point_struct pos)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   mmi_gui_multi_line_input_box_pen_enum multi_line_input_box_event;
   mmi_gui_EMS_input_box_pen_enum EMS_input_box_event;
   BOOL ret = MMI_FALSE;
   S32 multitap_index=0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
//if in handwriting process no handle pen event
    if(mmi_pen_in_handwriting_process())    
    {
        return ;
    }

   switch (gPenEditorActiveControl)
   {
      case PEN_EDITOR_CONTROL_INFORMATION_BAR:
         break;
      case PEN_EDITOR_CONTROL_MULTILINE_INPUT_BOX:
         ret = coolsand_UI_multi_line_input_box_translate_pen_event(&MMI_multiline_inputbox,MMI_PEN_EVENT_REPEAT,pos.x,pos.y,&multi_line_input_box_event);
         break;
      case PEN_EDITOR_CONTROL_EMS_INPUT_BOX:
         ret = coolsand_UI_EMS_input_box_translate_pen_event(&MMI_EMS_inputbox,MMI_PEN_EVENT_REPEAT,pos.x,pos.y,&EMS_input_box_event);
         break;
      case PEN_EDITOR_CONTROL_INLINE_MULTILINE_INPUT_BOX:
         break;
      case PEN_EDITOR_CONTROL_SINGLE_LINE_INPU_BOX:
         break;
      case PEN_EDITOR_CONTROL_INLINE_TEXT:
         break;
      case PEN_EDITOR_CONTROL_DATE:
         break;
      case PEN_EDITOR_CONTROL_IP4:
         break;
      case PEN_EDITOR_CONTROL_TIME:
         break;
      case PEN_EDITOR_CONTROL_TIME_PERIOD:
         break;
      case PEN_EDITOR_CONTROL_SCROLL_BAR:
         break;
      case PEN_EDITOR_CONTROL_VIRTUAL_KEYBOARD:
         break;
      case PEN_EDITOR_CONTROL_CANDIDATE_AREA:
         break;
      case PEN_EDITOR_CONTROL_IME_BUTTON:
         break;
      case PEN_EDITOR_CONTROL_MULTITAP:
         ret = coolsand_UI_multitap_input_translate_pen_position(gPenEditorMultitapInput,pos.x,pos.y,&multitap_index);
         if (ret)
         {
            
         }
         break;
      case PEN_EDITOR_CONTROL_NONE:
         break;
      default:
         MMI_ASSERT(0);
         break;
   }
   if (MMI_FALSE == ret)
   {
      mmi_wgui_general_pen_repeat_hdlr(pos);
   }
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_pen_abort_handler
* DESCRIPTION
*   \brief This function is to handle pen abort event.
*
* PARAMETERS
*  \param pos : the point of pen abort
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_pen_abort_handler(mmi_pen_point_struct pos)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   mmi_gui_multi_line_input_box_pen_enum multi_line_input_box_event;
   mmi_gui_EMS_input_box_pen_enum EMS_input_box_event;
   BOOL ret = MMI_FALSE;
   S32 multitap_index=0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
//if in handwriting process no handle pen event
    if(mmi_pen_in_handwriting_process())    
    {
        return ;
    }
   switch (gPenEditorActiveControl)
   {
      case PEN_EDITOR_CONTROL_INFORMATION_BAR:
         break;
      case PEN_EDITOR_CONTROL_MULTILINE_INPUT_BOX:
         ret = coolsand_UI_multi_line_input_box_translate_pen_event(&MMI_multiline_inputbox,MMI_PEN_EVENT_ABORT,pos.x,pos.y,&multi_line_input_box_event);
         break;
      case PEN_EDITOR_CONTROL_EMS_INPUT_BOX:
         ret = coolsand_UI_EMS_input_box_translate_pen_event(&MMI_EMS_inputbox,MMI_PEN_EVENT_ABORT,pos.x,pos.y,&EMS_input_box_event);
         break;
      case PEN_EDITOR_CONTROL_INLINE_MULTILINE_INPUT_BOX:
         break;
      case PEN_EDITOR_CONTROL_SINGLE_LINE_INPU_BOX:
         break;
      case PEN_EDITOR_CONTROL_INLINE_TEXT:
         break;
      case PEN_EDITOR_CONTROL_DATE:
         break;
      case PEN_EDITOR_CONTROL_IP4:
         break;
      case PEN_EDITOR_CONTROL_TIME:
         break;
      case PEN_EDITOR_CONTROL_TIME_PERIOD:
         break;
      case PEN_EDITOR_CONTROL_SCROLL_BAR:
         break;
      case PEN_EDITOR_CONTROL_VIRTUAL_KEYBOARD:
         {
            mmi_gui_virtual_keyboard_pen_enum vk_event;
            mmi_gui_pen_event_param_struct vk_para;
            ret = coolsand_UI_virtual_keyboard_translate_pen_event(&MMI_virtual_keyboard, pos.x, pos.y, MMI_PEN_EVENT_ABORT, &vk_event, &vk_para);
         }
         break;
      case PEN_EDITOR_CONTROL_CANDIDATE_AREA:
         break;
      case PEN_EDITOR_CONTROL_IME_BUTTON:
         break;
      case PEN_EDITOR_CONTROL_MULTITAP:
         ret = coolsand_UI_multitap_input_translate_pen_position(gPenEditorMultitapInput,pos.x,pos.y,&multitap_index);
         if (ret)
         {
            
         }
         break;
      case PEN_EDITOR_CONTROL_NONE:
         break;
      default:
         MMI_ASSERT(0);
         break;
   }
   gPenEditorActiveControl = PEN_EDITOR_CONTROL_NONE;
   if (MMI_FALSE == ret)
   {
      mmi_wgui_general_pen_abort_hdlr(pos);
   }
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_pen_move_handler
* DESCRIPTION
*   \brief This function is to handle pen move event.
*
* PARAMETERS
*  \param pos : the point of pen move
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_pen_move_handler(mmi_pen_point_struct pos)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   mmi_gui_multi_line_input_box_pen_enum multi_line_input_box_event;
   mmi_gui_EMS_input_box_pen_enum EMS_input_box_event;
   S32 multitap_index=0;
   BOOL ret = MMI_FALSE;
   mmi_gui_pen_event_param_struct vk_para;
   mmi_gui_virtual_keyboard_pen_enum vk_event;
#if defined(__MMI_GB__)        
     mmi_gui_single_line_input_box_pen_enum single_line_input_box_event;//wangzl:add
   U8 syllableIndex = 0; //wangzl:add
   U8 candidateIndex = 0;//wangzl:add
#endif   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   TBM_ENTRY(0x2829);
//if in handwriting process no handle pen event
    if(mmi_pen_in_handwriting_process())    
    {
        return ;
    }
#if defined(__MMI_GB__)    
   //wangzl:add B
	if(gb_is_pen_in_composition_area(pos))
	{
		mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: pen move in composition area");
		ret = coolsand_UI_single_line_input_box_translate_pen_event(&gbCompositionArea,MMI_PEN_EVENT_DOWN,pos.x,pos.y,&single_line_input_box_event);
		if (ret)
		{
			coolsand_UI_show_single_line_input_box_ext(&gbCompositionArea, pos.x, pos.y);
			coolsand_UI_lock_double_buffer();
			/* /TODO : Jump to the index */
			if ((INPUT_MODE_TR_BOPOMO == MMI_current_input_mode ||INPUT_MODE_SM_PINYIN == MMI_current_input_mode) )
			{
				syllableIndex= mmi_pen_editor_pinyin_set_syllable_by_position();
				mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: syllableindex is:%d",syllableIndex);
				//gb_set_syllable_by_index(syllableIndex);
				gb_display_syllable_by_index(syllableIndex);
			}
			coolsand_UI_unlock_double_buffer();
			coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
			TBM_EXIT(0x2829);
			return;
		}
		gPenEditorActiveControl = PEN_EDITOR_CONTROL_NONE;
		TBM_EXIT(0x2829);
		return;
	}
	else if(gb_is_pen_in_candidate_area(pos))
	{

		mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: pen move in candidate area");
		ret = coolsand_UI_single_line_input_box_translate_pen_event(&gbCandidateArea,MMI_PEN_EVENT_MOVE,pos.x,pos.y,&single_line_input_box_event);
		if (ret)
		{
			coolsand_UI_show_single_line_input_box_ext(&gbCandidateArea, pos.x, pos.y);
			coolsand_UI_single_line_input_box_next(&gbCandidateArea);
			coolsand_UI_lock_double_buffer();
			/* /TODO : Jump to the index */
			if ((INPUT_MODE_TR_BOPOMO == MMI_current_input_mode ||INPUT_MODE_SM_PINYIN == MMI_current_input_mode||
			INPUT_MODE_SM_STROKE == MMI_current_input_mode))  //add by chenq for bug 6094 
			{
				candidateIndex= mmi_pen_editor_pinyin_set_candidate_by_position();
				mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: chiense candidateindex is:%d",candidateIndex);
				if(candidateIndex > 0)
				{
					gb_set_candidate_by_index(candidateIndex - 1);
					//gb_input_character_by_candidate_index(candidateIndex - 1);
				}
			}
			else if(INPUT_MODE_SMART_UPPERCASE_ABC == MMI_current_input_mode ||INPUT_MODE_SMART_LOWERCASE_ABC == MMI_current_input_mode) 
			{
				candidateIndex= mmi_pen_editor_SABC_Sabc_set_candidate_by_position();
				mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: english candidateindex is:%d",candidateIndex);
				if(candidateIndex > 0)
				{
					gb_set_candidate_by_index(candidateIndex - 1);
				}

			}
			coolsand_UI_unlock_double_buffer();
			coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
			TBM_EXIT(0x2829);
			return;
		}
		gPenEditorActiveControl = PEN_EDITOR_CONTROL_NONE;
		TBM_EXIT(0x2829);
		return;
	}
#endif
    
	//wangzl:add E
   switch (gPenEditorActiveControl)
   {
      case PEN_EDITOR_CONTROL_INFORMATION_BAR:
         break;
      case PEN_EDITOR_CONTROL_MULTILINE_INPUT_BOX:
         ret = coolsand_UI_multi_line_input_box_translate_pen_event(&MMI_multiline_inputbox,MMI_PEN_EVENT_MOVE,pos.x,pos.y,&multi_line_input_box_event);
         break;
      case PEN_EDITOR_CONTROL_EMS_INPUT_BOX:
         ret = coolsand_UI_EMS_input_box_translate_pen_event(&MMI_EMS_inputbox,MMI_PEN_EVENT_MOVE,pos.x,pos.y,&EMS_input_box_event);
         break;
      case PEN_EDITOR_CONTROL_INLINE_MULTILINE_INPUT_BOX:
         break;
      case PEN_EDITOR_CONTROL_SINGLE_LINE_INPU_BOX:
         break;
      case PEN_EDITOR_CONTROL_INLINE_TEXT:
         break;
      case PEN_EDITOR_CONTROL_DATE:
         break;
      case PEN_EDITOR_CONTROL_IP4:
         break;
      case PEN_EDITOR_CONTROL_TIME:
         break;
      case PEN_EDITOR_CONTROL_TIME_PERIOD:
         break;
      case PEN_EDITOR_CONTROL_SCROLL_BAR:
         break;
      case PEN_EDITOR_CONTROL_VIRTUAL_KEYBOARD:
         ret = coolsand_UI_virtual_keyboard_translate_pen_event(&MMI_virtual_keyboard, pos.x, pos.y, MMI_PEN_EVENT_MOVE, &vk_event, &vk_para);
         break;
      case PEN_EDITOR_CONTROL_CANDIDATE_AREA:
         break;
      case PEN_EDITOR_CONTROL_IME_BUTTON:
         break;
      case PEN_EDITOR_CONTROL_MULTITAP:
         ret = coolsand_UI_multitap_input_translate_pen_position(gPenEditorMultitapInput,pos.x,pos.y,&multitap_index);
         if (ret)
         {
            
         }
         break;
      case PEN_EDITOR_CONTROL_NONE:
         break;
      default:
         MMI_ASSERT(0);
         break;
   }
   if (MMI_FALSE == ret)
   {
      mmi_wgui_general_pen_move_hdlr(pos);
   }
   TBM_EXIT(0x2829);
}
#endif
#ifdef __MMI_HAND_WRITING__
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_pen_set_stroke_color
* DESCRIPTION
*   \brief This function is to set the stroke color.
*
* PARAMETERS
*  \param stroke_color : the color of stroke
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_pen_set_stroke_color(color_t stroke_color)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   gPenEditorStrokeColor = stroke_color;
}
#endif
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_setup_input_box
* DESCRIPTION
*   \brief This function is to set up the input box for hand writing.
*
* PARAMETERS
*  \param stroke_area : the initial stroke area
*  \param ext_stroke : the stroke area after stroke down
*  \param input_type : the input method type
*  \param information_bar_flag : the information bar is on or not
*  \param input_box_type : the type of the input box
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_setup_input_box (mmi_pen_handwriting_area_struct* stroke_area,
                                                                  mmi_pen_handwriting_area_struct* ext_stroke,
                                                                     U16 input_type,byte information_bar_flag,
                                                                        mmi_pen_editor_input_box_type_enum input_box_type)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   U16 input_type_only = input_type&INPUT_TYPE_MASK;
   static BOOL	is_init = MMI_FALSE;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   TBM_ENTRY(0x2820);
   gPenEditorHistoryVirtualKeyboardType = MMI_GUI_VIRTUAL_KEYBOARD_MAX_LANG;
   
#ifdef __MMI_HAND_WRITING__
   if(NULL!=stroke_area)
   {
   	stroke_area->y1+= 40;//wangzl:070620:temp solution,must be modified
   }
   if ((NULL!=stroke_area)&&(NULL!=ext_stroke)
   	&&!(INPUT_TYPE_NUMERIC_PASSWORD==input_type_only
           ||INPUT_TYPE_SAT_NUMERIC_PASSWORD==input_type_only))
   {
      gPenEditorEnableHandWriting = MMI_TRUE;
      mmi_pen_start_capture_strokes(PEN_EDITOR_STROKE_BUFFER_SIZE, gPenEditorStrokeBuffer, 1, stroke_area, ext_stroke);
      memcpy(&g_pen_editor_original_handwriting_area,stroke_area,sizeof(mmi_pen_handwriting_area_struct));
      mmi_pen_register_stroke_down_handler( mmi_pen_editor_stroke_down_handler);
      mmi_pen_register_stroke_move_handler( mmi_pen_editor_stroke_move_handler);
      mmi_pen_register_stroke_up_handler(mmi_pen_editor_stroke_up_handler);

       
	mmi_pen_editor_hand_writing_initailize();
	mmi_pen_begin_strokes_of_character();
	switch(MMI_current_input_mode)
	{//Disable handwriting when the handwriting language is not supported.	
		case INPUT_MODE_MULTITAP_UPPERCASE_ABC:	
		case INPUT_MODE_MULTITAP_UPPERCASE_ABC_NO_NUMERIC:
		case INPUT_MODE_SMART_UPPERCASE_ABC:
		case INPUT_MODE_MULTITAP_LOWERCASE_ABC:	
		case INPUT_MODE_MMI_LOWERCASE_ABC:	
		case INPUT_MODE_MULTITAP_LOWERCASE_ABC_NO_NUMERIC:
		case INPUT_MODE_SMART_LOWERCASE_ABC:
		case INPUT_MODE_123: 
		case INPUT_MODE_MMI_NUMERIC:	
		case INPUT_MODE_123_SYMBOLS: 
		case INPUT_MODE_MMI_MULTITAP_BPMF:
		case INPUT_MODE_MMI_MULTITAP_PINYIN:       
		case INPUT_MODE_MMI_SM_STROKE:
		case INPUT_MODE_MMI_TR_STROKE:
		case INPUT_MODE_TR_MULTITAP_BOPOMO:
		case INPUT_MODE_SM_MULTITAP_PINYIN:
		case INPUT_MODE_TR_STROKE:
		case INPUT_MODE_SM_STROKE:
		case INPUT_MODE_TR_BOPOMO:
		case INPUT_MODE_SM_PINYIN:
			mmi_pen_resume_capture_strokes( );
		break;
		default:
			mmi_pen_pause_capture_strokes( );
		break;
	}
	  
   }
#endif

#if !defined(__MMI_HANDWRITING_PAD__)
   mmi_pen_register_abort_handler(mmi_pen_editor_pen_abort_handler);
   mmi_pen_register_down_handler(mmi_pen_editor_pen_down_handler);
   mmi_pen_register_long_tap_handler(mmi_pen_editor_pen_long_tap_handler);
   mmi_pen_register_move_handler(mmi_pen_editor_pen_move_handler);
   mmi_pen_register_up_handler(mmi_pen_editor_pen_up_handler);
   mmi_pen_register_repeat_handler(mmi_pen_editor_pen_repeat_handler);
#endif
   
   coolsand_UI_create_single_line_input_box_use_buffer(&gPenEditorCandidateInputBox,
                                                                                       MMI_multitap_x,
                                                                                       MMI_multitap_y - 1,
                                                                                       MMI_multitap_width,
                                                                                       MMI_multitap_height ,
                                                                                       gPenEditorCandidateBuffer,
                                                                                       ((PEN_EDITOR_MAX_CANDIDATE_SIZE+1) * sizeof(U16)));
   coolsand_UI_set_single_line_input_box_theme(&gPenEditorCandidateInputBox, &candidate_singleline_inputbox_theme);
   gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_DISABLE_SCROLLING;
   gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_FULL_HEIGHT_BLOCK_CURSOR;
   gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_VIEW_FULL_CHARACTERS;
   gPenEditorCandidateInputBox.ext_flags |= UI_SINGLE_LINE_INPUT_BOX_EXT_FIXED_GRID;
    
    
   gPenEditorCandidateInputBox.text_x = 0;
   gPenEditorCandidateInputBox.grid_width = ((UI_device_width)/PEN_EDITOR_CANDIDATE_SIZE);//coolsand_UI_get_character_width(0x4E00) + 2;

   gPenEditorInputBoxType = input_box_type;

    
   if (MMI_FALSE==is_init)
   {
      wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY);
      is_init = MMI_TRUE;
   }

   if ((MMI_GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY == MMI_virtual_keyboard.lang_type) && !(PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX==gPenEditorInputBoxType))
   {
      wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_TRAY);
   }
   if ((PEN_EDITOR_DATE==gPenEditorInputBoxType)
      ||(PEN_EDITOR_IP4==gPenEditorInputBoxType)
      ||(PEN_EDITOR_TIME==gPenEditorInputBoxType)
      ||(PEN_EDITOR_TIME_PERIOD==gPenEditorInputBoxType))
   {
      MMI_current_input_mode = INPUT_MODE_123;
   }

    
    
   if (!(INPUT_TYPE_NUMERIC_PASSWORD==input_type_only
         ||INPUT_TYPE_PHONE_NUMBER==input_type_only
         ||INPUT_TYPE_SAT_NUMERIC_PASSWORD==input_type_only
         ||INPUT_TYPE_KEYPAD_NUMERIC==input_type_only
         ||INPUT_TYPE_DECIMAL_NUMERIC==input_type_only
         ||INPUT_TYPE_ALPHABATIC_LOWERCASE==input_type_only
         ||INPUT_TYPE_ALPHABATIC_UPPERCASE==input_type_only
         ||INPUT_TYPE_NUMERIC_SYMBOL==input_type_only
         ||INPUT_TYPE_NUMERIC == input_type_only
   ))
   {
      if (information_bar_flag)
      {
         mmi_pen_editor_add_cocontrol(&gPenEditorControlSet, PEN_EDITOR_CONTROL_INFORMATION_BAR);
      }
#ifdef __MMI_WGUI_CSK_ENABLE__
      if (!((PEN_EDITOR_DATE==gPenEditorInputBoxType)
         ||(PEN_EDITOR_IP4==gPenEditorInputBoxType)
         ||(PEN_EDITOR_TIME==gPenEditorInputBoxType)
         ||(PEN_EDITOR_TIME_PERIOD==gPenEditorInputBoxType)))
      {
         mmi_pen_editor_set_CSK_content();
         set_softkey_function(mmi_pen_editor_switch_input_method, KEY_EVENT_UP, MMI_CENTER_SOFTKEY);
         register_softkey_handler(MMI_CENTER_SOFTKEY);
      }
#endif
   }
   switch (gPenEditorInputBoxType)
   {
      case PEN_EDITOR_MULTILINE_INPUT_BOX:
      case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
           
         switch (input_type_only)
         {
            case INPUT_TYPE_PHONE_NUMBER:
               wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER);
               break;
            case INPUT_TYPE_SAT_NUMERIC_PASSWORD:
            case INPUT_TYPE_KEYPAD_NUMERIC:
               wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER);
               break;
            case INPUT_TYPE_DECIMAL_NUMERIC:
               wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_DOT_TRAY);
               break;
            case INPUT_TYPE_NUMERIC:
            case INPUT_TYPE_NUMERIC_PASSWORD:
               wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY);
               break;
            default :							
                    wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_TRAY);//modify default virtual keyboard type by qiff
               break;
         }
         mmi_pen_editor_add_cocontrol(&gPenEditorControlSet, PEN_EDITOR_CONTROL_MULTILINE_INPUT_BOX);
         mmi_pen_editor_add_cocontrol(&gPenEditorControlSet, PEN_EDITOR_CONTROL_MULTITAP);
         break;
      case PEN_EDITOR_EMS_INPUT_BOX:
         mmi_pen_editor_add_cocontrol(&gPenEditorControlSet, PEN_EDITOR_CONTROL_EMS_INPUT_BOX);
         mmi_pen_editor_add_cocontrol(&gPenEditorControlSet, PEN_EDITOR_CONTROL_MULTITAP);
         break;
      case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
          
         switch (input_type_only)
         {
            case INPUT_TYPE_PHONE_NUMBER:
               wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER);
               break;
            case INPUT_TYPE_SAT_NUMERIC_PASSWORD:
            case INPUT_TYPE_KEYPAD_NUMERIC:
               wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER);
               break;
            case INPUT_TYPE_DECIMAL_NUMERIC:
               wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_DOT_TRAY);
               break;
            case INPUT_TYPE_NUMERIC:
            case INPUT_TYPE_NUMERIC_PASSWORD:
               wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY);
               break;
            default :
               break;
         }
         mmi_pen_editor_set_inputbox_LSK_Function(wgui_inline_edit_DONE_function);
         mmi_pen_editor_add_cocontrol(&gPenEditorControlSet, PEN_EDITOR_CONTROL_INLINE_MULTILINE_INPUT_BOX);
         mmi_pen_editor_add_cocontrol(&gPenEditorControlSet, PEN_EDITOR_CONTROL_MULTITAP);
         break;
      case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
         switch (input_type_only)
         {
            case INPUT_TYPE_ALPHANUMERIC_LOWERCASE:
               wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH);
               break;
            case INPUT_TYPE_ALPHANUMERIC_UPPERCASE:
               wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH_UPPERCASE);
               break;
            case INPUT_TYPE_PHONE_NUMBER:
               wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER);
               break;
            case INPUT_TYPE_SAT_NUMERIC_PASSWORD:
            case INPUT_TYPE_KEYPAD_NUMERIC:
               wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER);
               break;
            case INPUT_TYPE_DECIMAL_NUMERIC:
               wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_DOT_TRAY);
               break;
            case INPUT_TYPE_NUMERIC:
            case INPUT_TYPE_NUMERIC_PASSWORD:
               wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY);
               break;
            default :
               //MMI_ASSERT(0);
               break;
         }
         mmi_pen_editor_add_cocontrol(&gPenEditorControlSet, PEN_EDITOR_CONTROL_SINGLE_LINE_INPU_BOX);
         break;
      case PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX:
          
          
         if ((MMI_GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY == MMI_virtual_keyboard.lang_type))
         {
//fix bug 15133
         #if 0 
            switch (input_type_only)
            {
               case INPUT_TYPE_MMI_LOWERCASE_ABC:
                  wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH);
                  break;
               case INPUT_TYPE_MMI_MULTITAP_BPMF:
                  wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_BOPOMO);
                  break;
               case INPUT_TYPE_MMI_MULTITAP_PINYIN:
                  wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PINYIN);
                  break;
               case INPUT_TYPE_MMI_NUMERIC:
                  wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER);
                  break;
               /*shaozhw 20060620 modify begin: for support sm_pinyin and sm_stroke */
               case INPUT_TYPE_MMI_SM_STROKE:
               case INPUT_TYPE_SM_STROKE:
                  wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER);
                  break;
               case INPUT_TYPE_SM_PINYIN:
                  break;
               /*shaozhw 20060620 modify end */
               default :
			wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER);
						//MMI_ASSERT(0);
                  break;
            }
#endif
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_TRAY);
         }
         mmi_pen_editor_add_cocontrol(&gPenEditorControlSet, PEN_EDITOR_CONTROL_SINGLE_LINE_INPU_BOX);
         break;
      case PEN_EDITOR_INLINE_TEXT:
         switch (input_type_only)
         {
            case INPUT_TYPE_ALPHANUMERIC_UPPERCASE:
			case INPUT_TYPE_ALPHANUMERIC_LOWERCASE:
				wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_TRAY);
				//wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH);
               	//wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH_UPPERCASE);
               break;
            case INPUT_TYPE_PHONE_NUMBER:
               wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER);
               break;
            case INPUT_TYPE_SAT_NUMERIC_PASSWORD:
            case INPUT_TYPE_KEYPAD_NUMERIC:
               wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER);
               break;
            case INPUT_TYPE_DECIMAL_NUMERIC:
               wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_DOT_TRAY);
               break;
            case INPUT_TYPE_NUMERIC:
            case INPUT_TYPE_NUMERIC_PASSWORD:
               wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY);
               break;
            default :
		 wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_TRAY); //set default virtual keyboard type by qiff

               //MMI_ASSERT(0);
               break;
         }
         mmi_pen_editor_add_cocontrol(&gPenEditorControlSet, PEN_EDITOR_CONTROL_INLINE_TEXT);
         mmi_pen_editor_add_cocontrol(&gPenEditorControlSet, PEN_EDITOR_CONTROL_MULTITAP);
         break;
      case PEN_EDITOR_DATE:
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY);
         mmi_pen_editor_add_cocontrol(&gPenEditorControlSet, PEN_EDITOR_CONTROL_DATE);
         break;
      case PEN_EDITOR_IP4:
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY);
         mmi_pen_editor_add_cocontrol(&gPenEditorControlSet, PEN_EDITOR_CONTROL_IP4);
         break;
      case PEN_EDITOR_TIME:
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY);
         mmi_pen_editor_add_cocontrol(&gPenEditorControlSet, PEN_EDITOR_CONTROL_TIME);
         break;
      case PEN_EDITOR_TIME_PERIOD:
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY);
         mmi_pen_editor_add_cocontrol(&gPenEditorControlSet, PEN_EDITOR_CONTROL_TIME_PERIOD);
         break;
      default :
         MMI_ASSERT(0);
         break;
   }
   if (!(INPUT_TYPE_NUMERIC_PASSWORD==input_type_only
           ||INPUT_TYPE_SAT_NUMERIC_PASSWORD==input_type_only)
           ||!(PEN_EDITOR_SINGLE_LINE_INPU_BOX==gPenEditorInputBoxType))
   {
	
      mmi_pen_editor_add_cocontrol(&gPenEditorControlSet, PEN_EDITOR_CONTROL_VIRTUAL_KEYBOARD);
   }
   mmi_pen_editor_add_cocontrol(&gPenEditorControlSet, PEN_EDITOR_CONTROL_CANDIDATE_AREA);
   mmi_pen_editor_add_cocontrol(&gPenEditorControlSet, PEN_EDITOR_CONTROL_IME_BUTTON);
   gPenEditorVKState = PEN_EDITOR_VK_ON;
   gPenEditorVKCandidate=MMI_FALSE;
    
   //gPenEditorGDIHandler = NULL;
   gPenEditorShowVK = MMI_TRUE;
   mmi_pen_editor_reset_vk_text(&gPenEditorVKText);
   wgui_set_virtual_keyboard_display_area((U8*)gPenEditorVKText.VKText, MMI_FALSE);
   #ifdef __MMI_HAND_WRITING__
   gPenEditorFirstStroke = MMI_TRUE;
   mmi_pen_editor_pen_set_stroke_color(PhnsetGetHandwritingColor());
   gPenEditorHWSpeed = PhnsetGetHandwritingSpeed();
   #endif
    
   //mmi_pen_editor_clear_key_handler();
   g_pen_editor_active_multitap_index = -1;
   g_pen_editor_hold_confirm = MMI_FALSE;
   if (INPUT_MODE_TR_MULTITAP_BOPOMO == MMI_current_input_mode || INPUT_MODE_SM_MULTITAP_PINYIN == MMI_current_input_mode)
   {
	   mmi_pen_editor_bpmf_pinyin_create_multitap(MMI_current_input_mode);
	   mmi_pen_editor_bpmf_pinyin_register_multitap_function();
   }

   #ifdef __MMI_HAND_WRITING__
   mmi_pen_editor_change_handwriting_area();
   #endif
    
   if (wgui_handle_inputbox_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN]||
       handle_category28_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN]||
       wgui_handle_qsearch_singleline_inputbox_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN]||
       wgui_handle_singleline_inputbox_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN]||
       wgui_handle_inline_singleline_inputbox_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN])
   {
      SetKeyHandler(mmi_pen_editor_switch_input_method,KEY_POUND,KEY_EVENT_DOWN);
   }
   TBM_EXIT(0x2820);
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_close_input_box
* DESCRIPTION
*   \brief This function is to close the pen editor.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_close_input_box(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   TBM_ENTRY(0x282A);
    
   gPenEditorHistoryVirtualKeyboardType = MMI_virtual_keyboard.lang_type;
    
   #if 0 //wangzl: disabel this for when press star key there is a bug if the select is not do 
   if (gPenEditorSelectCandidate)
   {
	mmi_pen_editor_confirm_selection();
   }
   #endif
   //Disable Hand Writing
    
   mmi_pen_stop_capture_strokes();
   gPenEditorEnableHandWriting = MMI_FALSE;
   //Reset Pen Event Queue
   //Flush pen event queue in mmi_pen_stop_capture_strokes instead of mmi_pen_reset here
   //Clear stroke event handler as nothing
   //mmi_pen_reset();
    
   if ( gPenEditorGDIHandler != NULL && *gPenEditorGDIHandler != GDI_ERROR_HANDLE )
   {
      gdi_layer_push_and_set_active(*gPenEditorGDIHandler);
      gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
      gdi_layer_clear(GDI_COLOR_TRANSPARENT);
      gdi_layer_pop_and_restore_active();
   }
   #ifdef __MMI_HAND_WRITING__
   mmi_pen_register_stroke_down_handler( mmi_pen_dummy_hdlr);
   mmi_pen_register_stroke_move_handler( mmi_pen_dummy_hdlr);
   mmi_pen_register_stroke_up_handler(mmi_pen_dummy_hdlr);
   coolsand_UI_cancel_timer(mmi_pen_editor_stroke_up_timer_handler);
   #endif
   wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY);

 //  COS_KillTimer(GetMmiTaskHandle(MOD_TP_TASK), TPTimerIdHandwritingUp );
   coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
//   COS_KillTimer(GetMmiTaskHandle(MOD_TP_TASK),TPTimerIdHandwritingConfirm );
   mmi_pen_editor_reset_control_set(&gPenEditorControlSet);
   mmi_pen_editor_reset_vk_text(&gPenEditorVKText);
   wgui_set_virtual_keyboard_display_area((U8*)gPenEditorVKText.VKText, MMI_FALSE);
   gPenEditorActiveControl = PEN_EDITOR_CONTROL_NONE;
   gPenEditorCandidateIndex = 0;
   gPenEditorCandidatePage = 0;
   gPenEditorVKCandidate=MMI_FALSE;
   g_pen_editor_hold_confirm = MMI_FALSE;
   g_pen_editor_vk_show_at_start = MMI_FALSE;
#ifdef __MMI_WGUI_CSK_ENABLE__
    
   set_softkey_label(NULL, MMI_CENTER_SOFTKEY);
   set_softkey_icon(NULL, MMI_CENTER_SOFTKEY);
    
   //But in in-line editor, redrawing is needed when completing in-line item
   //redraw_softkey(MMI_CENTER_SOFTKEY);
   clear_softkey_handler(MMI_CENTER_SOFTKEY);
#endif
TBM_EXIT(0x282A);
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_show_component
* DESCRIPTION
*   \brief This function is to show the components of the pen editor.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_show_component(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_add_cocontrol
* DESCRIPTION
*   \brief This function is to add the pen editor control to the control stes.
*
* PARAMETERS
*  \param peneditorcontrolset : The Control Set to be added
*  \param peneditorcontrol : The Control to add 
*
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_add_cocontrol(mmi_pen_editor_control_set_struct *peneditorcontrolset,
                                                            mmi_pen_editor_control_type_enum peneditorcontrol)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   peneditorcontrolset->PenEditorControlSet[peneditorcontrolset->NumberofControlSets++] = peneditorcontrol;
   MMI_ASSERT(peneditorcontrolset->NumberofControlSets < (PEN_EDITOR_MAX_CONTROL_SET_SIZE+1));
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_reset_cocontrol_set
* DESCRIPTION
*   \brief This function is to reset the control stes.
*
* PARAMETERS
*  \param peneditorcontrolset : the control to be reset
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_reset_control_set (mmi_pen_editor_control_set_struct *peneditorcontrolset)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   memset(peneditorcontrolset,0,sizeof(mmi_pen_editor_control_set_struct));
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_reset_vk_text
* DESCRIPTION
*   \brief This function is to reset the virtual keyboard text.
*
* PARAMETERS
*  \param peneditorvktext : the text of virtual keyboard
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_reset_vk_text (mmi_pen_editor_vk_text_struct *peneditorvktext)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   memset(peneditorvktext,0,sizeof(mmi_pen_editor_vk_text_struct));
 /*  #ifdef __MMI_GB__
   //GB_Reset();        //wangzl add for bug 18126
   #endif*/       //chenqiang delete it
}
void mmi_pen_editor_set_vk_to_show(MMI_BOOL enable)
{
	g_pen_editor_vk_show_at_start = enable;
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_input_character
* DESCRIPTION
*   \brief This function is to input character.
*
* PARAMETERS
*  \param c : The character to input
*  \param needselect : The candidate can be replaced or not
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_input_character(UI_character_type c,BOOL needselect)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   BOOL is_last = MMI_FALSE;
   BOOL is_able_to_insert = MMI_TRUE;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   TBM_ENTRY(0x2825);
    
   if ( INPUT_TYPE_PHONE_NUMBER == ( MMI_current_input_type & INPUT_TYPE_MASK ) )
   {
      if (MMI_current_input_ext_type & INPUT_TYPE_EXT_SIM_NUMERIC)
      {
         if ( !mmi_pen_editor_check_valid_input ( c, gPenEditorPhoneNumberSIMValidCharacters ) )
         {
            TBM_EXIT(0x2825);
            return;
         }
      }
      else
      {
         if ( !mmi_pen_editor_check_valid_input(c, gPenEditorPhoneNumberValidCharacters))
         {
            TBM_EXIT(0x2825);
            return;
         }
      }
   }
   else if ( INPUT_TYPE_DECIMAL_NUMERIC == (MMI_current_input_type & INPUT_TYPE_MASK) )
   {
      if (!mmi_pen_editor_check_valid_input(c, gPenEditorDecimalNumericValidCharacters))
      {
      	  TBM_EXIT(0x2825);
         return;
      }
   }
   else if ( INPUT_TYPE_KEYPAD_NUMERIC == (MMI_current_input_type&INPUT_TYPE_MASK) 
               || INPUT_TYPE_SAT_NUMERIC_PASSWORD == (MMI_current_input_type&INPUT_TYPE_MASK))
   {
      if ( !mmi_pen_editor_check_valid_input(c, gPenEditorKeyPadNumbericValidCharacters) )
      {
         TBM_EXIT(0x2825);
         return;
      }
   }
   else if ( MMI_allow_only_english_input_modes )
   {
       
       
      if ( (c > 0xFF) )
      {
         TBM_EXIT(0x2825);
         return;
      }
   }
   is_able_to_insert = mmi_pen_editor_check_input(c);

mmi_trace(g_sw_HandW,"vkmixed: needselect: %d",needselect);
mmi_trace(g_sw_HandW,"vkmixed: gPenEditorVKText.TextLength: %d",gPenEditorVKText.TextLength);
mmi_trace(g_sw_HandW,"vkmixed: MMI_virtual_keyboard.lang_type: %d",MMI_virtual_keyboard.lang_type);
mmi_trace(g_sw_HandW,"vkmixed: gPenEditorSelectCandidate: %d",gPenEditorSelectCandidate);
mmi_trace(g_sw_HandW,"vkmixed: is_able_to_insert: %d",is_able_to_insert);
mmi_trace(g_sw_HandW,"vkmixed: gPenEditorVKState: %d",gPenEditorVKState);


   coolsand_UI_lock_double_buffer();
   
   if (needselect)
   {
      //gPenEditorSelectCandidate = MMI_TRUE;
      mmi_pen_editor_set_candidate_state(MMI_TRUE);
   }
   else
   {
      {
         if (gPenEditorVKText.TextLength>0)
         {
            mmi_pen_editor_reset_vk_text(&gPenEditorVKText);
            wgui_set_virtual_keyboard_display_area((U8*)gPenEditorVKText.VKText, MMI_TRUE);
         }
         if (MMI_GUI_VIRTUAL_KEYBOARD_PINYIN == MMI_virtual_keyboard.lang_type
		 #ifdef __MMI_TR_CHINESE_VK__
		||MMI_GUI_VIRTUAL_KEYBOARD_BOPOMO == MMI_virtual_keyboard.lang_type
		#endif
              )
         {
            if (PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX != gPenEditorInputBoxType)
            {
               mmi_pen_editor_hide_candidate_area();
            }
         }
      }
   }
   switch (gPenEditorInputBoxType)
   {
      case PEN_EDITOR_MULTILINE_INPUT_BOX:
      case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
      case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
         if (needselect)
         {
            if (is_able_to_insert) 
            {
            multiline_inputbox_multitap_input(c);
            }
            mmi_pen_editor_set_right_softkey(PEN_EDITOR_RSK_BY_STATE);
         }
         else
         {
            if (gPenEditorSelectCandidate)
            {
               if (is_able_to_insert) 
               {
	               multiline_inputbox_multitap_input(c);
               }
               /*Temp Solution for LSK Text Update*/
               //gPenEditorSelectCandidate = MMI_FALSE;
               mmi_pen_editor_set_candidate_state(MMI_FALSE);
               mmi_pen_editor_set_left_soft_key();
		#ifdef __MMI_GB__
		gb_register_arrow_handler();
		#else
		register_multiline_inputbox_keys();
		#endif
               if (PEN_EDITOR_DICT_MULTILINE_INPUT_BOX == gPenEditorInputBoxType)
               {
                  cat201_register_key_func();
               }
               multiline_inputbox_multitap_input_complete();
			if (c < 0x4e00 || c > 0x9fff)
			{
				if (PEN_EDITOR_VK_OFF==gPenEditorVKState)
				{
					gPenEditorVKState = PEN_EDITOR_VK_ON;
					 
					if (!(MMI_GUI_VIRTUAL_KEYBOARD_TRAY==MMI_virtual_keyboard.lang_type
					||MMI_GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY==MMI_virtual_keyboard.lang_type
					||MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY==MMI_virtual_keyboard.lang_type
					||MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_DOT_TRAY==MMI_virtual_keyboard.lang_type
                     ||MMI_GUI_VIRTUAL_KEYBOARD_CHINESE_SYMBOL==MMI_virtual_keyboard.lang_type
                     ))
                    // fixed bug : 15108 ||MMI_GUI_VIRTUAL_KEYBOARD_NUMBER == MMI_virtual_keyboard.lang_type))
					{
						if (PEN_EDITOR_DICT_MULTILINE_INPUT_BOX != gPenEditorInputBoxType)
						{
							S32 h = get_changed_height(gPenEditorVKState, gPenEditorVKCandidate)-1;
							mmi_pen_editor_resize_content_area(h);
						}
					coolsand_UI_move_single_line_input_box(&gPenEditorCandidateInputBox, gPenEditorCandidateInputBox.x, MMI_virtual_keyboard.y-gPenEditorCandidateInputBox.height-2);
					}
				 
                  if (!(MMI_GUI_VIRTUAL_KEYBOARD_CHINESE_SYMBOL == MMI_virtual_keyboard.lang_type 
			 ))
			 //fixed bug 15108||MMI_GUI_VIRTUAL_KEYBOARD_NUMBER == MMI_virtual_keyboard.lang_type))
					{
						mmi_pen_editor_clear_and_show_virtual_keyboard_area();
					}
				}
			}
            }
            else
            {
               if (wgui_multiline_inputbox_decimal_input_mode_flag&&(c==(UI_character_type)'.'))
               {
                  wgui_multiline_inputbox_decimal_numeric_handle_pound_key_down();
               }
               else
               {
                  multiline_inputbox_direct_input(c);
               }
            }
         }
         break;
      case PEN_EDITOR_EMS_INPUT_BOX:
         if (needselect)
         {
            if (is_able_to_insert) 
            {
            EMS_inputbox_multitap_input(c);
            }
            mmi_pen_editor_set_right_softkey(PEN_EDITOR_RSK_BY_STATE);
         }
         else
         {
            if (gPenEditorSelectCandidate)
            {
               if (is_able_to_insert) 
               {
	               EMS_inputbox_multitap_input(c);
               }
               //gPenEditorSelectCandidate = MMI_FALSE;
               mmi_pen_editor_set_candidate_state(MMI_FALSE);
               mmi_pen_editor_set_left_soft_key();
              #ifdef __MMI_GB__
		gb_register_arrow_handler();
		#else
           	 register_EMS_inputbox_keys();
		#endif
               EMS_inputbox_multitap_input_complete();
		if (c < 0x4e00 || c > 0x9fff)
		{
	               if (PEN_EDITOR_VK_OFF==gPenEditorVKState)
	               {
	                  gPenEditorVKState = PEN_EDITOR_VK_ON;
	                   
	                  if (!(MMI_GUI_VIRTUAL_KEYBOARD_TRAY==MMI_virtual_keyboard.lang_type
	                        ||MMI_GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY==MMI_virtual_keyboard.lang_type
	                        ||MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY==MMI_virtual_keyboard.lang_type
	                        ||MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_DOT_TRAY==MMI_virtual_keyboard.lang_type
				   ||MMI_GUI_VIRTUAL_KEYBOARD_CHINESE_SYMBOL==MMI_virtual_keyboard.lang_type))
		//fix bug 15108	||MMI_GUI_VIRTUAL_KEYBOARD_NUMBER == MMI_virtual_keyboard.lang_type))
	                  {
				S32 h = get_changed_height(gPenEditorVKState, gPenEditorVKCandidate)-1;
				mmi_pen_editor_resize_content_area(h);
				coolsand_UI_move_single_line_input_box(&gPenEditorCandidateInputBox, gPenEditorCandidateInputBox.x, MMI_virtual_keyboard.y-gPenEditorCandidateInputBox.height-2);
	                  }
	                   
			   if (!(MMI_GUI_VIRTUAL_KEYBOARD_CHINESE_SYMBOL == MMI_virtual_keyboard.lang_type ))
		//fix bug 15108     ||MMI_GUI_VIRTUAL_KEYBOARD_NUMBER == MMI_virtual_keyboard.lang_type))
	                  {
	                     mmi_pen_editor_clear_and_show_virtual_keyboard_area();
	                  }
	               }
		}
            }
            else
            {
               EMS_inputbox_direct_input(c);
            }
         }
         break;
      case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
      case PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX:
         if (needselect)
         {
            singleline_inputbox_multitap_input(c);
            mmi_pen_editor_set_right_softkey(PEN_EDITOR_RSK_BY_STATE);
         }
         else
         {
            if (gPenEditorSelectCandidate)
            {
               singleline_inputbox_multitap_input(c);
               //gPenEditorSelectCandidate = MMI_FALSE;
               mmi_pen_editor_set_candidate_state(MMI_FALSE);
               /*Temp Solution for LSK Text Update*/
               mmi_pen_editor_set_left_soft_key();
               singleline_inputbox_multitap_input_complete();
               if (PEN_EDITOR_VK_OFF==gPenEditorVKState)
               {
                  gPenEditorVKState = PEN_EDITOR_VK_ON;
                   
                  mmi_pen_editor_clear_and_show_virtual_keyboard_area();
               }
            }
            else
            {
               if (PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX==gPenEditorInputBoxType)
               {
                  if(cat200_search_function)
                  {
                     singleline_inputbox_direct_input(c);
                     refresh_search_list(MMI_singleline_inputbox.text);
                     RedrawCategoryFunction();
                  }
               }
               else
               {
                  if (wgui_singleline_inputbox_decimal_input_mode_flag&&(c==(UI_character_type)'.'))
                  {
                     wgui_singleline_inputbox_decimal_numeric_handle_pound_key_down();
                  }
                  else
                  {
                     singleline_inputbox_direct_input(c);
                  }
               }
            }
         }
         break;
      case PEN_EDITOR_INLINE_TEXT:
         if (needselect)
         {
            inline_edit_singleline_inputbox_multitap_input(c);
            mmi_pen_editor_set_right_softkey(PEN_EDITOR_RSK_BY_STATE);
         }
         else
         {
            if (gPenEditorSelectCandidate)
            {
               inline_edit_singleline_inputbox_multitap_input(c);
               //gPenEditorSelectCandidate = MMI_FALSE;
               mmi_pen_editor_set_candidate_state(MMI_FALSE);
               /*Temp Solution for LSK Text Update*/
               mmi_pen_editor_set_left_soft_key();
               inline_edit_singleline_inputbox_multitap_input_complete();
		 /*Fix Bug#12607 for cs2324c by cong.li on 2009.06.16. 
  		  The arrow key handle is cleared in stroke_up_timer_handler, so we should reset them again.*/
		 SetKeyHandler(inline_text_edit_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
		 SetKeyHandler(inline_text_edit_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
		 SetKeyHandler(inline_edit_singleline_inputbox_previous_character, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		 SetKeyHandler(inline_edit_singleline_inputbox_next_character, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
			   
               if (PEN_EDITOR_VK_OFF==gPenEditorVKState)
               {
                  gPenEditorVKState = PEN_EDITOR_VK_ON;
                  mmi_pen_editor_clear_and_show_virtual_keyboard_area();
               }
            }
            else
            {
               if (wgui_inline_singleline_inputbox_decimal_input_mode_flag&&(c==(UI_character_type)'.'))
               {
                  wgui_inline_singleline_inputbox_decimal_numeric_handle_pound_key_down();
               }
               else
               {
                  inline_edit_singleline_inputbox_direct_input(c);
               }
            }
         }
         break;
      case PEN_EDITOR_DATE:
         if (needselect)
         {
            if (date_input_test_last_position(current_date_input))
            {
				is_last = MMI_TRUE;
            }
            date_input_direct_input_nodraw(c);
            if (!is_last)
            {
               date_input_previous_character();
            }
            show_date_input(current_date_input);
            mmi_pen_editor_set_right_softkey(PEN_EDITOR_RSK_BACK);
         }
         else
         {
            if (gPenEditorSelectCandidate)
            {
               date_input_direct_input(c);
               //gPenEditorSelectCandidate = MMI_FALSE;
               mmi_pen_editor_set_candidate_state(MMI_FALSE);
               /*Temp Solution for LSK Text Update*/
               mmi_pen_editor_set_left_soft_key();
	        /*Fix Bug#12433 for cs2324c by cong.li on 2009.06.05. 
	           The arrow key handle is cleared in stroke_up_timer_handler, so we should reset them again.*/
               SetKeyHandler(inline_date_edit_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
               SetKeyHandler(inline_date_edit_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
               SetKeyHandler(date_input_previous_character, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
               SetKeyHandler(date_input_next_character, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		 
               if (PEN_EDITOR_VK_OFF==gPenEditorVKState)
               {
                  gPenEditorVKState = PEN_EDITOR_VK_ON;
                  mmi_pen_editor_clear_and_show_virtual_keyboard_area();
               }
            }
            else
            {
               date_input_direct_input(c);
            }
         }
         break;
      case PEN_EDITOR_IP4:
         if (needselect)
         {
            if (IP4_input_test_last_position(current_IP4_input))
            {
				is_last = MMI_TRUE;
            }
            IP4_input_direct_input_nodraw(c);
            if (!is_last)
            {
               IP4_input_previous_character();
            }
            show_IP4_input(current_IP4_input);
            mmi_pen_editor_set_right_softkey(PEN_EDITOR_RSK_BACK);
         }
         else
         {
            if (gPenEditorSelectCandidate)
            {
               IP4_input_direct_input(c);
               //gPenEditorSelectCandidate = MMI_FALSE;
               mmi_pen_editor_set_candidate_state(MMI_FALSE);
               /*Temp Solution for LSK Text Update*/
               mmi_pen_editor_set_left_soft_key();
               if (PEN_EDITOR_VK_OFF==gPenEditorVKState)
               {
                  gPenEditorVKState = PEN_EDITOR_VK_ON;
                  mmi_pen_editor_clear_and_show_virtual_keyboard_area();
               }
            }
            else
            {
               IP4_input_direct_input(c);
            }
         }
         break;
      case PEN_EDITOR_TIME:
         if (needselect)
         {
            if (time_input_test_last_position(current_time_input))
            {
				is_last = MMI_TRUE;
            }
            time_input_direct_input_nodraw(c);
            if (!is_last)
            {
               time_input_previous_character();
            }
            show_time_input(current_time_input);
            mmi_pen_editor_set_right_softkey(PEN_EDITOR_RSK_BACK);
         }
         else
         {
            if (gPenEditorSelectCandidate)
            {
               time_input_direct_input(c);
               //gPenEditorSelectCandidate = MMI_FALSE;
               mmi_pen_editor_set_candidate_state(MMI_FALSE);
               /*Temp Solution for LSK Text Update*/
               mmi_pen_editor_set_left_soft_key();
	        /*Fix Bug#12433 for cs2324c by cong.li on 2009.06.05. 
	           The arrow key handle is cleared in stroke_up_timer_handler, so we should reset them again.*/
		 SetKeyHandler(inline_time_edit_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
		 SetKeyHandler(inline_time_edit_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
		 SetKeyHandler(time_input_previous_character, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		 SetKeyHandler(time_input_next_character, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);			   
		
               if (PEN_EDITOR_VK_OFF==gPenEditorVKState)
               {
                  gPenEditorVKState = PEN_EDITOR_VK_ON;
                  mmi_pen_editor_clear_and_show_virtual_keyboard_area();
               }
            }
            else
            {
               time_input_direct_input(c);
            }
         }
         break;
      case PEN_EDITOR_TIME_PERIOD:
         if (needselect)
         {
            if (time_period_input_test_last_position(current_time_period_input))
            {
				is_last = MMI_TRUE;
            }
            time_period_input_direct_input_nodraw(c);
            if (!is_last)
            {
               time_period_input_previous_character();
            }
            show_time_period_input(current_time_period_input);
            mmi_pen_editor_set_right_softkey(PEN_EDITOR_RSK_BACK);
         }
         else
         {
            if (gPenEditorSelectCandidate)
            {
               time_period_input_direct_input(c);
               //gPenEditorSelectCandidate = MMI_FALSE;
               mmi_pen_editor_set_candidate_state(MMI_FALSE);
               /*Temp Solution for LSK Text Update*/
               mmi_pen_editor_set_left_soft_key();
	        /*Fix Bug#12433 for cs2324c by cong.li on 2009.06.05. 
	           The arrow key handle is cleared in stroke_up_timer_handler, so we should reset them again.*/
		 SetKeyHandler(inline_time_period_edit_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
		 SetKeyHandler(inline_time_period_edit_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
		 SetKeyHandler(time_period_input_previous_character, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		 SetKeyHandler(time_period_input_next_character, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);		
		
               if (PEN_EDITOR_VK_OFF==gPenEditorVKState)
               {
                  gPenEditorVKState = PEN_EDITOR_VK_ON;
                  mmi_pen_editor_clear_and_show_virtual_keyboard_area();
               }
            }
            else
            {
               time_period_input_direct_input(c);
            }
         }
         break;
      default:
         MMI_ASSERT(0);
   }
   coolsand_UI_unlock_double_buffer();
   coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
   TBM_EXIT(0x2825);
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_backspace
* DESCRIPTION
*   \brief This function is to backspace.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_backspace(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
    
   switch (gPenEditorInputBoxType)
   {
      case PEN_EDITOR_MULTILINE_INPUT_BOX:
      case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
      case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
         if (wgui_multiline_inputbox_decimal_input_mode_flag)
         {
            wgui_handle_inputbox_right_softkey_down();
         }
         else
         {
            multiline_inputbox_delete_character();
         }
         wgui_set_RSK();
         break;
      case PEN_EDITOR_EMS_INPUT_BOX:
         EMS_inputbox_backspace();
         wgui_set_EMS_inputbox_RSK();
         break;
      case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
      case PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX:
         if (wgui_singleline_inputbox_decimal_input_mode_flag)
         {
            wgui_handle_singleline_inputbox_right_softkey_down();
         }
         else
         {
            singleline_inputbox_delete_character();
         }
         if(cat200_search_function)
         {
            refresh_search_list(MMI_singleline_inputbox.text);
            RedrawCategoryFunction();
         }
         wgui_set_singleline_RSK();
         break;
      case PEN_EDITOR_INLINE_TEXT:
          
         if (coolsand_UI_single_line_input_box_get_text_length(&MMI_inline_singleline_inputbox) > 0)
         {
         if (wgui_inline_singleline_inputbox_decimal_input_mode_flag)
         {
            handle_inline_text_edit_right_softkey_down();
         }
         else
         {
            inline_edit_singleline_inputbox_delete_character();
         }
         mmi_pen_editor_set_right_soft_key_inline_text();
         }
         else
         {
            UI_editor_play_tone_cannot_change();
         }
         break;
      case PEN_EDITOR_DATE:
         date_input_previous_character();
         break;
      case PEN_EDITOR_IP4:
         IP4_input_previous_character();
         break;
      case PEN_EDITOR_TIME:
         time_input_previous_character();
         break;
      case PEN_EDITOR_TIME_PERIOD:
         time_period_input_previous_character();
         break;
      default :
         MMI_ASSERT(0);
   }
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_bracket
* DESCRIPTION
*   \brief This function is to bracket.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_bracket(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
    
   if (gPenEditorSelectCandidate)
   {
      return;
   }
   switch (gPenEditorInputBoxType)
   {
      case PEN_EDITOR_MULTILINE_INPUT_BOX:
      case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
      case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
         coolsand_UI_multi_line_input_box_insert_character(&MMI_multiline_inputbox, '(');
         if (0 < coolsand_UI_multi_line_input_box_get_remaining_length(&MMI_multiline_inputbox))
         {
            coolsand_UI_multi_line_input_box_insert_character(&MMI_multiline_inputbox, ')');
            multiline_inputbox_previous_character();
         }
         else
         {
            redraw_multiline_inputbox();
         }
         multiline_inputbox_input_callback();
         break;
      case PEN_EDITOR_EMS_INPUT_BOX:
         {
             
	    //We check this by coolsand_UI_EMS_input_box_insert_character
            U8 ret = coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox, '(');
            if (0 < EMS_get_remaining_length(MMI_EMS_inputbox.data) && (ret == EMS_OK))
            {
               if (coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox, ')') == EMS_OK)
                  EMS_inputbox_previous();
            }
            redraw_EMS_inputbox();
            EMS_inputbox_input_callback();
            break;
	 }
      case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
      case PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX:
         coolsand_UI_single_line_input_box_insert_character(&MMI_singleline_inputbox, '(');
         if (0 < coolsand_UI_single_line_input_box_get_remaining_length(&MMI_singleline_inputbox))
         {
            coolsand_UI_single_line_input_box_insert_character(&MMI_singleline_inputbox, ')');
            singleline_inputbox_previous_character();
         }
         else
         {
            redraw_singleline_inputbox();
         }
         singleline_inputbox_input_callback();
          
         if(cat200_search_function)
         {
            refresh_search_list(MMI_singleline_inputbox.text);
            RedrawCategoryFunction();
         }
         break;
      case PEN_EDITOR_INLINE_TEXT:
         coolsand_UI_single_line_input_box_insert_character(&MMI_inline_singleline_inputbox, '(');
         if (0 < coolsand_UI_single_line_input_box_get_remaining_length(&MMI_inline_singleline_inputbox))
         {
            coolsand_UI_single_line_input_box_insert_character(&MMI_inline_singleline_inputbox, ')');
            inline_edit_singleline_inputbox_previous_character();
         }
         else
         {
            redraw_inline_singleline_inputbox();
         }
         inline_singleline_inputbox_input_callback();
         break;
      case PEN_EDITOR_DATE:
      case PEN_EDITOR_IP4:
      case PEN_EDITOR_TIME:
      case PEN_EDITOR_TIME_PERIOD:
         break;
      default :
         MMI_ASSERT(0);
   }
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_new_line
* DESCRIPTION
*   \brief This function is to new line.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_new_line(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
    
   if (INPUT_TYPE_EXT_NO_SHOW_NEW_LINE_SYMBOL & MMI_current_input_ext_type)
   {
   	return;
   }
   switch (gPenEditorInputBoxType)
   {
      case PEN_EDITOR_MULTILINE_INPUT_BOX:
      case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
      case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
         coolsand_UI_multi_line_input_box_insert_newline(&MMI_multiline_inputbox);
         redraw_multiline_inputbox();
         multiline_inputbox_input_callback();
         break;
      case PEN_EDITOR_EMS_INPUT_BOX:
			EMS_inputbox_insert_new_line();
         redraw_EMS_inputbox();
         break;
      case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
      case PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX:
      case PEN_EDITOR_INLINE_TEXT:
      case PEN_EDITOR_DATE:
      case PEN_EDITOR_IP4:
      case PEN_EDITOR_TIME:
      case PEN_EDITOR_TIME_PERIOD:
         break;
      default :
         MMI_ASSERT(0);
         break;
   }
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_resize_content_area
* DESCRIPTION
*   \brief This function is to resize the content area.
*
* PARAMETERS
*  \param changed_height : the changed height of the control
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_resize_content_area(int changed_height)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   /*shaozhw 20060610 modify begin: for support sm_pinyin and sm_stroke */
/*wuzc Del Start For SIMM Ver: MT1.9.2  on 2007-1-18 16:26 */
   #if 0
   #if !defined(__MMI_IME_SUPORT_SM_PINYIN_STROKE__)
       if (0 == changed_height)
       {
          return;
       }
   #endif
   #endif
/*wuzc Del End  For SIMM Ver: MT1.9.2  on 2007-1-18 16:26 */

   /*shaozhw 20060610 modify end */
   switch (gPenEditorInputBoxType)
   {
      case PEN_EDITOR_MULTILINE_INPUT_BOX:
            #if !defined(__MMI_IME_SUPORT_SM_PINYIN_STROKE__)
                   if (0 == changed_height)
                   {
                      return;
                   }
            #endif
         coolsand_UI_resize_multi_line_input_box(&MMI_multiline_inputbox,MMI_multiline_inputbox.width,MMI_multiline_inputbox.height+changed_height);
         redraw_multiline_inputbox();
         break;
      case PEN_EDITOR_EMS_INPUT_BOX:
        /*shaozhw 20060610 add begin: for support sm_pinyin and sm_stroke */
        /*wuzc Modify Start For SIMM Ver: MT1.9.2  on 2007-1-18 16:26 */
        #if 0 //def __MMI_IME_SUPORT_SM_PINYIN_STROKE__
         if(MMI_GUI_VIRTUAL_KEYBOARD_TRAY == MMI_virtual_keyboard.lang_type)
         {
              coolsand_UI_resize_EMS_input_box(&MMI_EMS_inputbox,MMI_EMS_inputbox.width,(UI_device_height - MMI_EMS_inputbox.y - MMI_virtual_keyboard_language_map[MMI_GUI_VIRTUAL_KEYBOARD_TRAY].virtual_keyboard_layout->height - MMI_softkey_height - 2));
         }
         else
         {
             coolsand_UI_resize_EMS_input_box(&MMI_EMS_inputbox,MMI_EMS_inputbox.width,MMI_EMS_inputbox.height+changed_height);
         }
        #else
             coolsand_UI_resize_EMS_input_box(&MMI_EMS_inputbox,MMI_EMS_inputbox.width,MMI_EMS_inputbox.height+changed_height);
        #endif
        /*wuzc Modify End  For SIMM Ver: MT1.9.2  on 2007-1-18 16:26 */
        /*shaozhw 20060610 add end */
         redraw_EMS_inputbox();
         break;
      case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
         coolsand_UI_resize_multi_line_input_box(&MMI_multiline_inputbox,MMI_multiline_inputbox.width,MMI_multiline_inputbox.height+changed_height);
         redraw_multiline_inputbox();
         break;
      case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
         break;
      case PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX:
         resize_fixed_list(MMI_fixed_list_menu.width, MMI_fixed_list_menu.height+changed_height);
          
         coolsand_UI_dynamic_list_menu_locate_highlighted_item(&MMI_fixed_list_menu);
         move_singleline_inputbox(MMI_singleline_inputbox.x, MMI_fixed_list_menu.y+MMI_fixed_list_menu.height+1);
         break;
      case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
         resize_fixed_list(MMI_fixed_list_menu.width, MMI_fixed_list_menu.height+changed_height);
         coolsand_UI_dynamic_list_menu_locate_highlighted_item(&MMI_fixed_list_menu);
         move_multiline_inputbox(MMI_multiline_inputbox.x, MMI_fixed_list_menu.y+MMI_fixed_list_menu.height+wgui_inputbox_information_bar_height);
         wgui_setup_input_information(MMI_multiline_inputbox.x,MMI_multiline_inputbox.y-wgui_inputbox_information_bar_height,MMI_multiline_inputbox.width,wgui_inputbox_information_bar_height);
         RedrawCategoryControlledArea201Screen();
         break;
      case PEN_EDITOR_INLINE_TEXT:
         break;
      case PEN_EDITOR_DATE:
         break;
      case PEN_EDITOR_IP4:
         break;
      case PEN_EDITOR_TIME:
         break;
      case PEN_EDITOR_TIME_PERIOD:
         break;
      default :
         MMI_ASSERT(0);
         break;
   }
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_set_virtual_keyboard_by_input_mode
* DESCRIPTION
*   \brief This function is to set the type of virtual keyboard by input mode.
*
* PARAMETERS
*  \param : the input mode
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_set_virtual_keyboard_by_input_mode(s32 input_mode)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   switch (input_mode)
   {
      case INPUT_MODE_MMI_LOWERCASE_ABC:
      case INPUT_MODE_MULTITAP_LOWERCASE_ABC:
      case INPUT_MODE_SMART_LOWERCASE_ABC:
         gPenEditorVKCandidate=MMI_FALSE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH);
         break;
      case INPUT_MODE_MMI_NUMERIC:
      case INPUT_MODE_123:
         gPenEditorVKCandidate=MMI_FALSE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER);
         break;
      case INPUT_MODE_MMI_MULTITAP_PINYIN:
      case INPUT_MODE_MMI_SM_STROKE:
         gPenEditorVKCandidate=MMI_FALSE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PINYIN);
         break;
      case INPUT_MODE_MULTITAP_UPPERCASE_ABC:
      case INPUT_MODE_SMART_UPPERCASE_ABC:
         gPenEditorVKCandidate=MMI_FALSE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH_UPPERCASE);
         break;
  #if 0
      case INPUT_MODE_MMI_MULTITAP_BPMF:
      case INPUT_MODE_MMI_TR_STROKE:
         gPenEditorVKCandidate=MMI_FALSE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_BOPOMO);
         break;
      case INPUT_MODE_TR_MULTITAP_BOPOMO:
      case INPUT_MODE_TR_STROKE:
   //   case INPUT_MODE_TR_BOPOMO:
         gPenEditorVKCandidate=MMI_TRUE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_BOPOMO);
         break;
      #endif
      case INPUT_MODE_SM_MULTITAP_PINYIN:
      case INPUT_MODE_SM_STROKE:
      case INPUT_MODE_SM_PINYIN:
         gPenEditorVKCandidate=MMI_TRUE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PINYIN);
         break;
	#if defined(__MMI_MULTITAP_SPANISH__)&&defined(__MMI_SPANISH_VK__)
	case INPUT_MODE_MMI_LOWERCASE_SPANISH:
         gPenEditorVKCandidate=MMI_FALSE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_SPANISH);
         break;
	#endif


//qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)&&defined(__MMI_TAMIL_VK__)
			case INPUT_MODE_MMI_MULTITAP_TAMIL:
                     gPenEditorVKCandidate=MMI_FALSE;
                     wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_TAMIL);
				break;
#endif
#if defined(__MMI_MULTITAP_BENGALI__)&&defined(__MMI_BENGALI_VK__)
			case INPUT_MODE_MMI_MULTITAP_BENGALI:
                     gPenEditorVKCandidate=MMI_FALSE;
                     wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_BENGALI);
				break;
#endif
#if defined(__MMI_MULTITAP_PUNJABI__)&&defined(__MMI_PUNJABI_VK__)
			case INPUT_MODE_MMI_MULTITAP_PUNJABI:
                     gPenEditorVKCandidate=MMI_FALSE;
                     wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PUNJABI);
				break;
#endif
#if defined(__MMI_MULTITAP_TELUGU__)&&defined(__MMI_TELUGU_VK__)
			case INPUT_MODE_MMI_MULTITAP_TELUGU:
                     gPenEditorVKCandidate=MMI_FALSE;
                     wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_TELUGU);
				break;
#endif
#if defined(__MMI_MULTITAP_KANNADA__)&&defined(__MMI_KANNADA_VK__)
			case INPUT_MODE_MMI_MULTITAP_KANNADA:
                     gPenEditorVKCandidate=MMI_FALSE;
                     wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_KANNADA);
				break;
#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)&&defined(__MMI_MALAYALAM_VK__)
			case INPUT_MODE_MMI_MULTITAP_MALAYALAM:
                     gPenEditorVKCandidate=MMI_FALSE;
                     wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_MALAYALAM);
				break;
#endif
#if defined(__MMI_MULTITAP_ORIYA__)&&defined(__MMI_ORIYA_VK__)
			case INPUT_MODE_MMI_MULTITAP_ORIYA:
                     gPenEditorVKCandidate=MMI_FALSE;
                     wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ORIYA);
				break;
#endif
//qiff add end
#if defined(__MMI_MULTITAP_MARATHI__)&&defined(__MMI_MARATHI_VK__)
			case INPUT_MODE_MMI_MULTITAP_MARATHI:
                     gPenEditorVKCandidate=MMI_FALSE;
                     wgui_setup_virtual_keyboard(MMI_GUI_VIRTUTAL_KEYBOARD_MARATHI);
				break;
#endif

#if defined(__MMI_MULTITAP_URDU__)&&defined(__MMI_URDU_VK__)
			case INPUT_MODE_MMI_MULTITAP_URDU:
                     gPenEditorVKCandidate=MMI_FALSE;
                     wgui_setup_virtual_keyboard(MMI_GUI_VIRTUTAL_KEYBOARD_URDU);
				break;
#endif

	#if defined(__MMI_MULTITAP_FRENCH__)&&defined(__MMI_FRENCH_VK__)
	case INPUT_MODE_MMI_LOWERCASE_FRENCH:
         gPenEditorVKCandidate=MMI_FALSE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_FRENCH);
         break;
	#endif
	#if defined(__MMI_MULTITAP_GERMAN__)&&defined(__MMI_GERMAN_VK__)
	case INPUT_MODE_MMI_LOWERCASE_GERMAN:
         gPenEditorVKCandidate=MMI_FALSE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_GERMAN);
         break;
	#endif
	#if defined(__MMI_MULTITAP_ITALIAN__)&&defined(__MMI_ITALIAN_VK__)
	case INPUT_MODE_MMI_LOWERCASE_ITALIAN:
         gPenEditorVKCandidate=MMI_FALSE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ITALIAN);
         break;
	#endif
	#if defined(__MMI_MULTITAP_RUSSIAN__)&&defined(__MMI_RUSSIAN_VK__)
	case INPUT_MODE_MMI_LOWERCASE_RUSSIAN:
         gPenEditorVKCandidate=MMI_FALSE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_RUSSIAN);
         break;
	#endif
	#if defined(__MMI_MULTITAP_TURKISH__)&&defined(__MMI_TURKISH_VK__)
	case INPUT_MODE_MMI_LOWERCASE_TURKISH:
         gPenEditorVKCandidate=MMI_FALSE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_TURKISH);
         break;
	#endif
	#if defined(__MMI_MULTITAP_PORTUGUESE__)&&defined(__MMI_PORTUGUESE_VK__)
	case INPUT_MODE_MMI_LOWERCASE_PORTUGUESE:
         gPenEditorVKCandidate=MMI_FALSE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PORTUGUESE);
         break;
	#endif
	#if defined(__MMI_MULTITAP_INDONESIAN__)&&defined(__MMI_INDONESIAN_VK__)
	case INPUT_MODE_MMI_LOWERCASE_INDONESIAN:
         gPenEditorVKCandidate=MMI_FALSE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_INDONESIAN);
         break;
	#endif
	#if defined(__MMI_MULTITAP_MALAY__)&&defined(__MMI_MALAY_VK__)
	case INPUT_MODE_MMI_LOWERCASE_MALAY:
         gPenEditorVKCandidate=MMI_FALSE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_MALAY);
         break;
	#endif
	#if defined(__MMI_MULTITAP_VIETNAMESE__)&&defined(__MMI_VIETNAMESE_VK__)
	case INPUT_MODE_MMI_LOWERCASE_VIETNAMESE:
         gPenEditorVKCandidate=MMI_FALSE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_VIETNAMESE);
         break;
	#endif
	#if defined(__MMI_MULTITAP_HINDI__)&&defined(__MMI_HINDI_VK__)
	case INPUT_MODE_MULTITAP_HINDI:
         gPenEditorVKCandidate=MMI_FALSE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_HINDI);
         break;
	#endif
	#if defined(__MMI_MULTITAP_THAI__)&&defined(__MMI_THAI_VK__)
	case INPUT_MODE_MMI_MULTITAP_THAI:
         gPenEditorVKCandidate=MMI_FALSE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_THAI);
         break;
	#endif
      default :
         gPenEditorVKCandidate=MMI_FALSE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER);
         break;
   }
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_vk_insert_character
* DESCRIPTION
*   \brief This function is to insert character in Virtual Keyboard Text.
*
* PARAMETERS
*  \param vk_text : the virtual keyboard text control
*  \param ch : the character to insert to the vk_text control
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
BOOL mmi_pen_editor_vk_insert_character(mmi_pen_editor_vk_text_struct* vk_text,UI_character_type ch)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   switch (MMI_virtual_keyboard.lang_type)
   {
      #ifdef __MMI_TR_CHINESE_VK__
      case MMI_GUI_VIRTUAL_KEYBOARD_BOPOMO:
         if (vk_text->TextLength < PEN_EDITOR_MAX_BOPOMOFO_SIZE-1)
         {
            vk_text->VKText[(S32)(vk_text->TextLength++)]=UnicodeToBPMFInternal(ch);
            return MMI_TRUE;
         }
         return MMI_FALSE;
      #endif
      case MMI_GUI_VIRTUAL_KEYBOARD_PINYIN:
         /*shaozhw 20060614 modify begin: for bug5229 */
         /* if ((vk_text->TextLength < PEN_EDITOR_MAX_PINYIN_SIZE-1)&&(ch>0x60)&&(ch<0x7b)) */
         if (((vk_text->TextLength < PEN_EDITOR_MAX_PINYIN_SIZE-1)&&(ch>0x60)&&(ch<0x7b))|| (ch == 0x20))
         /*shaozhw 20060614 modify emd */
         {
            vk_text->VKText[(S32)(vk_text->TextLength++)]=ch;
            return MMI_TRUE;
         }
         return MMI_FALSE;
      default:
         return MMI_FALSE;
   }
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_vk_delete_character
* DESCRIPTION
*   \brief This function is to delete character in Virtual Keyboard Text.
*
* PARAMETERS
*  \param vk_text : the virtual keyboard text control
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/

BOOL mmi_pen_editor_vk_delete_character(mmi_pen_editor_vk_text_struct* vk_text)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   if (vk_text->TextLength > 0)
   {
      vk_text->VKText[(S32)(--vk_text->TextLength)]='\0';
      return MMI_TRUE;
   }
   return MMI_FALSE;
}

BOOL mmi_pen_editor_vk_delete_last_character(void)
{
	return mmi_pen_editor_vk_delete_character(&gPenEditorVKText);
}
/*****************************************************************************
 * FUNCTION
 *  mmi_pen_editor_candidate_is_able_to_next_page
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL mmi_pen_editor_candidate_is_able_to_next_page(void)
{

#if defined(__MMI_GB__)

    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_character_type candidate_buffer[PEN_EDITOR_MAX_CANDIDATE_SIZE];
    //int num = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
   return (mmi_ime_get_candidates_by_page((gPenEditorCandidatePage + 1), candidate_buffer) ? MMI_TRUE : MMI_FALSE);
#else
   return TRUE;
#endif
}   /* end of mmi_pen_editor_candidate_is_able_to_next_page */

/*****************************************************************************
 * FUNCTION
 *  mmi_pen_editor_candidate_is_able_to_prev_page
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL mmi_pen_editor_candidate_is_able_to_prev_page(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gPenEditorCandidatePage)
    {
        return MMI_TRUE;
    }
    return MMI_FALSE;
}   /* end of mmi_pen_editor_candidate_is_able_to_prev_page */
/*****************************************************************************
 * FUNCTION
 *  mmi_pen_editor_draw_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  arrow_direction     [IN]        
 *  arrow_state         [IN]        
 *  arrow_type          [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_pen_editor_draw_arrow(
        mmi_pen_editor_arrow_direction_enum arrow_direction,
        mmi_pen_editor_arrow_state_enum arrow_state,
        mmi_pen_editor_arrow_type_enum arrow_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t arrow_color;
    S32 x = 0, y = 0, width = 0, height = 0;
    S32 input_box_x = 0, input_box_width = 0, input_box_grid_width = 0;
    U8 direction = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (PEN_EDITOR_ARROW_FOR_CANDIDATE == arrow_type)
    {
        input_box_x = gPenEditorCandidateInputBox.x;
        input_box_width = gPenEditorCandidateInputBox.width;
        input_box_grid_width = gPenEditorCandidateInputBox.grid_width;
        y = gPenEditorCandidateInputBox.y + (gPenEditorCandidateInputBox.height >> 3) + 1;
        width = GB_ARROW_WIDTH; //gPenEditorCandidateInputBox.grid_width >> 1;
        height = ((gPenEditorCandidateInputBox.height << 1) + gPenEditorCandidateInputBox.height) >> 2;
    }
	#if 0
    else if (PEN_EDITOR_ARROW_FOR_COMPOSITION == arrow_type)
    {
        input_box_x = g_pen_editor_composition_input_box.x;
        input_box_width = g_pen_editor_composition_input_box.width;
        input_box_grid_width = g_pen_editor_composition_input_box.grid_width;
        y = g_pen_editor_composition_input_box.y + (g_pen_editor_composition_input_box.height >> 3) + 1;
        width = g_pen_editor_composition_input_box.grid_width >> 1;
        height = ((g_pen_editor_composition_input_box.height << 1) + g_pen_editor_composition_input_box.height) >> 2;
    }
	#endif
    if (PEN_EDITOR_ARROW_DISABLE == arrow_state)
    {
        arrow_color = coolsand_UI_color(200, 200, 200);
    }
    else
    {
        arrow_color = coolsand_UI_color(0, 102, 204);
    }

    if (PEN_EDITOR_ARROW_DIRECTION_RIGHT == arrow_direction)
    {
        x = input_box_x + input_box_width + (input_box_grid_width >> 2);
        direction = 1;
        if (PEN_EDITOR_ARROW_DISABLE == arrow_state || PEN_EDITOR_ARROW_PRESS_UP == arrow_state)
        {
        }
        else if (PEN_EDITOR_ARROW_PRESS_DOWN == arrow_state)
        {
            x++;
            y++;
        }
    }
    else if (PEN_EDITOR_ARROW_DIRECTION_LEFT == arrow_direction)
    {
        x = input_box_x - (((input_box_grid_width << 1) + input_box_grid_width) >> 2) - 2;
        direction = 0;
        if (PEN_EDITOR_ARROW_DISABLE == arrow_state || PEN_EDITOR_ARROW_PRESS_UP == arrow_state)
        {
        }
        else if (PEN_EDITOR_ARROW_PRESS_DOWN == arrow_state)
        {
            x--;
            y++;
        }
    }
    draw_resizeable_arrow(x, y, direction, arrow_color, width, height);
}
/*****************************************************************************
 * FUNCTION
 *  mmi_pen_editor_show_candidate_area
 * DESCRIPTION
 *  
 * PARAMETERS
 *  arrow_enable        [IN]        
 *  candiate_state      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_pen_editor_show_candidate_area(MMI_BOOL arrow_enable, mmi_pen_editor_candidate_state_enum candiate_state)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_pen_editor_hide_candidate_area();
    if (PEN_EDITOR_CANDIDATE_ON_AND_FOCUS == candiate_state)
    {
        gPenEditorCandidateInputBox.flags &= ~UI_SINGLE_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW;
    }
    else if (PEN_EDITOR_CANDIDATE_ON_NOT_FOCUS == candiate_state)
    {
        gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW;
    }

    gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_STATE_MULTITAP;
    gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_FULL_HEIGHT_BLOCK_CURSOR;
    gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_CHARACTER_HIGHLIGHT;

    if (!arrow_enable)
    {
        coolsand_UI_resize_single_line_input_box(
            &gPenEditorCandidateInputBox,
            gPenEditorCandidateInputBox.grid_width * PEN_EDITOR_CANDIDATE_SIZE,
            gPenEditorCandidateInputBox.height);
        coolsand_UI_move_single_line_input_box(
            &gPenEditorCandidateInputBox,
            MMI_multitap_x,
            gPenEditorCandidateInputBox.y);
        g_pen_editor_candidate_state.arrow_enable = MMI_FALSE;
        g_pen_editor_candidate_state.left_arrow_state = PEN_EDITOR_ARROW_DISABLE;
        g_pen_editor_candidate_state.right_arrow_state = PEN_EDITOR_ARROW_DISABLE;
    }
    else
    {
        g_pen_editor_candidate_state.arrow_enable = MMI_TRUE;
        g_pen_editor_candidate_state.left_arrow_state = PEN_EDITOR_ARROW_DISABLE;
        g_pen_editor_candidate_state.right_arrow_state = PEN_EDITOR_ARROW_DISABLE;
        if (mmi_pen_editor_candidate_is_able_to_next_page() &&
            !(UI_SINGLE_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW & gPenEditorCandidateInputBox.flags))
        {
            g_pen_editor_candidate_state.right_arrow_state = PEN_EDITOR_ARROW_PRESS_UP;
        }
        if (mmi_pen_editor_candidate_is_able_to_prev_page() &&
            !(UI_SINGLE_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW & gPenEditorCandidateInputBox.flags))
        {
            g_pen_editor_candidate_state.left_arrow_state = PEN_EDITOR_ARROW_PRESS_UP;
        }
        coolsand_UI_resize_single_line_input_box(
            &gPenEditorCandidateInputBox,
            gPenEditorCandidateInputBox.grid_width * PEN_EDITOR_CANDIDATE_SIZE_WITH_ARROWS,
            gPenEditorCandidateInputBox.height);
        coolsand_UI_move_single_line_input_box(
            &gPenEditorCandidateInputBox,
            MMI_multitap_x + gPenEditorCandidateInputBox.grid_width,
            gPenEditorCandidateInputBox.y);
        mmi_pen_editor_draw_arrow(
            PEN_EDITOR_ARROW_DIRECTION_RIGHT,
            g_pen_editor_candidate_state.right_arrow_state,
            PEN_EDITOR_ARROW_FOR_CANDIDATE);
        mmi_pen_editor_draw_arrow(
            PEN_EDITOR_ARROW_DIRECTION_LEFT,
            g_pen_editor_candidate_state.left_arrow_state,
            PEN_EDITOR_ARROW_FOR_CANDIDATE);
    }
    coolsand_UI_show_single_line_input_box(&gPenEditorCandidateInputBox);
} 

BOOL mmi_pen_editor_is_arraw_able(void)
{
	return g_pen_editor_candidate_state.arrow_enable;
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_get_candidates_by_vk
* DESCRIPTION
*   \brief This function is to get candidates from virtual keyboard input
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/

BOOL mmi_pen_editor_get_candidates_by_vk(void)
{
#if defined(__MMI_GB__)

   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   UI_character_type candidate_buffer[PEN_EDITOR_MAX_CANDIDATE_SIZE];
   int num=0,i=0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   if (gPenEditorVKText.TextLength > 0)
   {
       
      coolsand_UI_set_font(gPenEditorCandidateInputBox.text_font);
      num = mmi_ime_get_candidates_by_composition(gPenEditorVKText.VKText,candidate_buffer);
      if (num)
      {
          
#if defined(__MMI_GB__)
		 mmi_resize_and_draw_input_box_according_Pen_Editor_Candidate();
#endif
		 mmi_pen_editor_reset_candidate_index();
         coolsand_UI_single_line_input_box_delete_all(&gPenEditorCandidateInputBox);
         for(;i<num;i++)
         {
            coolsand_UI_single_line_input_box_insert_character(&gPenEditorCandidateInputBox, candidate_buffer[i]);
         }
         coolsand_UI_single_line_input_box_goto_first_character(&gPenEditorCandidateInputBox);
         coolsand_UI_single_line_input_box_next(&gPenEditorCandidateInputBox);
         gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_STATE_MULTITAP;
         gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_FULL_HEIGHT_BLOCK_CURSOR;
         gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_CHARACTER_HIGHLIGHT;
         coolsand_UI_lock_double_buffer();
         mmi_pen_editor_hide_candidate_area();
         //coolsand_UI_show_single_line_input_box(&gPenEditorCandidateInputBox);
         mmi_pen_editor_show_candidate_area(MMI_TRUE, PEN_EDITOR_CANDIDATE_ON_AND_FOCUS);
         /*shaozhw 20060610 add begin: for support sm_pinyin and sm_stroke */
         /*shaozhw 20060610 add end */
         coolsand_UI_unlock_double_buffer();
         coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
         return MMI_TRUE;
      }
   }
#endif      
   
   return MMI_FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_get_candidates_by_phrase
* DESCRIPTION
*   \brief This function is to get candidates from input character
*
* PARAMETERS
*  \param ch : the previous character of the phrase
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/

BOOL mmi_pen_editor_get_candidates_by_phrase(UI_character_type ch)
{
#if defined(__MMI_GB__)

   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   UI_character_type candidate_buffer[PEN_EDITOR_MAX_CANDIDATE_SIZE];
   int num=0,i=0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
    
   coolsand_UI_set_font(gPenEditorCandidateInputBox.text_font);

   num=mmi_ime_get_candidates_by_phrase(ch, candidate_buffer);

   if (num)
   {
      coolsand_UI_single_line_input_box_delete_all(&gPenEditorCandidateInputBox);
      for(;i<num;i++)
      {
         coolsand_UI_single_line_input_box_insert_character(&gPenEditorCandidateInputBox, candidate_buffer[i]);
      }
       /*shaozhw 20060610 add begin: for support sm_pinyin and sm_stroke */
         #ifdef __MMI_IME_SUPORT_SM_PINYIN_STROKE__
             RegisterCandidateInputBoxArrowKeys(TRUE);
         #endif
       /*shaozhw 20060610 add end */
      coolsand_UI_single_line_input_box_goto_first_character(&gPenEditorCandidateInputBox);
      coolsand_UI_single_line_input_box_next(&gPenEditorCandidateInputBox);
      gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_STATE_MULTITAP;
      gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_FULL_HEIGHT_BLOCK_CURSOR;
      gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_CHARACTER_HIGHLIGHT;
      coolsand_UI_lock_double_buffer();
      mmi_pen_editor_hide_candidate_area();
      //coolsand_UI_show_single_line_input_box(&gPenEditorCandidateInputBox);
      mmi_pen_editor_show_candidate_area(g_pen_editor_candidate_state.arrow_enable, PEN_EDITOR_CANDIDATE_ON_AND_FOCUS);
      coolsand_UI_unlock_double_buffer();
      coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
      return MMI_TRUE;
   }
#endif   
   return MMI_FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_get_candidates_by_page
* DESCRIPTION
*   \brief This function is to get candidates from input character
*
* PARAMETERS
*  \param : the page number of the candidates
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/

BOOL mmi_pen_editor_get_candidates_by_page(s32 page)
{
#if defined(__MMI_GB__)

   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   UI_character_type candidate_buffer[PEN_EDITOR_MAX_CANDIDATE_SIZE];
   int num=0,i=0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
    
   coolsand_UI_set_font(gPenEditorCandidateInputBox.text_font);
   num=mmi_ime_get_candidates_by_page(page, candidate_buffer);
   if (num)
   {
      coolsand_UI_single_line_input_box_delete_all(&gPenEditorCandidateInputBox);
      for(;i<num;i++)
      {
         coolsand_UI_single_line_input_box_insert_character(&gPenEditorCandidateInputBox, candidate_buffer[i]);
      }
      /*shaozhw 20060610 add begin: for support sm_pinyin and sm_stroke */
         #ifdef __MMI_IME_SUPORT_SM_PINYIN_STROKE__
             RegisterCandidateInputBoxArrowKeys(TRUE);
         #endif
       /*shaozhw 20060610 add end */
      coolsand_UI_single_line_input_box_goto_first_character(&gPenEditorCandidateInputBox);
      coolsand_UI_single_line_input_box_next(&gPenEditorCandidateInputBox);
      gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_STATE_MULTITAP;
      gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_FULL_HEIGHT_BLOCK_CURSOR;
      gPenEditorCandidateInputBox.flags |= UI_SINGLE_LINE_INPUT_BOX_CHARACTER_HIGHLIGHT;
      coolsand_UI_lock_double_buffer();
      mmi_pen_editor_hide_candidate_area();
      //coolsand_UI_show_single_line_input_box(&gPenEditorCandidateInputBox);
      mmi_pen_editor_show_candidate_area(g_pen_editor_candidate_state.arrow_enable, PEN_EDITOR_CANDIDATE_ON_AND_FOCUS);
      coolsand_UI_unlock_double_buffer();
      coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
      return MMI_TRUE;
   }
#endif   
   return MMI_FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_get_candidates_prev_page
* DESCRIPTION
*   \brief This function is to get candidates of previous page
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/

void mmi_pen_editor_get_candidates_prev_page(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   if (gPenEditorCandidatePage > 0)
   {
      gPenEditorCandidatePage--;
      mmi_pen_editor_get_candidates_by_page(gPenEditorCandidatePage);
      coolsand_UI_single_line_input_box_next_steps(&gPenEditorCandidateInputBox,gPenEditorCandidateIndex);
      coolsand_UI_lock_double_buffer();
      mmi_pen_editor_hide_candidate_area();
      //coolsand_UI_show_single_line_input_box(&gPenEditorCandidateInputBox);
      mmi_pen_editor_show_candidate_area(g_pen_editor_candidate_state.arrow_enable, PEN_EDITOR_CANDIDATE_ON_AND_FOCUS);
      coolsand_UI_unlock_double_buffer();
      coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
  }
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_get_candidates_next_page
* DESCRIPTION
*   \brief This function is to get candidates of next page
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/

void mmi_pen_editor_get_candidates_next_page(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   gPenEditorCandidatePage++;
   if (!mmi_pen_editor_get_candidates_by_page(gPenEditorCandidatePage))
   {
      gPenEditorCandidatePage--;
      return;
   }
   coolsand_UI_single_line_input_box_next_steps(&gPenEditorCandidateInputBox,gPenEditorCandidateIndex);
   coolsand_UI_lock_double_buffer();
   mmi_pen_editor_hide_candidate_area();
   //coolsand_UI_show_single_line_input_box(&gPenEditorCandidateInputBox);
   mmi_pen_editor_show_candidate_area(g_pen_editor_candidate_state.arrow_enable, PEN_EDITOR_CANDIDATE_ON_AND_FOCUS);
   coolsand_UI_unlock_double_buffer();
   coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_get_candidates_prev
* DESCRIPTION
*   \brief This function is to get candidates of previous page
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/

void mmi_pen_editor_get_candidates_prev(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   if (gPenEditorCandidateIndex > 0)
   {
      gPenEditorCandidateIndex--;
      coolsand_UI_single_line_input_box_previous(&gPenEditorCandidateInputBox);
   }
   else
   {
      if ( INPUT_MODE_TR_MULTITAP_BOPOMO==MMI_current_input_mode
            ||INPUT_MODE_TR_BOPOMO==MMI_current_input_mode
            ||INPUT_MODE_TR_STROKE==MMI_current_input_mode
            ||INPUT_MODE_SM_MULTITAP_PINYIN==MMI_current_input_mode
            ||INPUT_MODE_SM_PINYIN==MMI_current_input_mode
            ||INPUT_MODE_SM_STROKE==MMI_current_input_mode )
      {
         if (gPenEditorCandidatePage > 0)
         {
            gPenEditorCandidatePage--;
            gPenEditorCandidateIndex = PEN_EDITOR_CANDIDATE_SIZE - 1;
            mmi_pen_editor_get_candidates_by_page(gPenEditorCandidatePage);
            coolsand_UI_single_line_input_box_goto_last_character(&gPenEditorCandidateInputBox);
         }
      }
   }
   coolsand_UI_lock_double_buffer();
   mmi_pen_editor_hide_candidate_area();
   //coolsand_UI_show_single_line_input_box(&gPenEditorCandidateInputBox);
   mmi_pen_editor_show_candidate_area(g_pen_editor_candidate_state.arrow_enable, PEN_EDITOR_CANDIDATE_ON_AND_FOCUS);
   coolsand_UI_unlock_double_buffer();
   coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_get_candidates_next
* DESCRIPTION
*   \brief This function is to get candidates of next page
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/

void mmi_pen_editor_get_candidates_next(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
    
   if ((gPenEditorCandidateIndex < PEN_EDITOR_CANDIDATE_SIZE - 1)
      && (gPenEditorCandidateIndex < (gPenEditorCandidateInputBox.text_length/2)-2))
   {
      gPenEditorCandidateIndex++;
      coolsand_UI_single_line_input_box_next(&gPenEditorCandidateInputBox);
   }
   else
   {
      if ( INPUT_MODE_TR_MULTITAP_BOPOMO==MMI_current_input_mode
            ||INPUT_MODE_TR_BOPOMO==MMI_current_input_mode
            ||INPUT_MODE_TR_STROKE==MMI_current_input_mode
            ||INPUT_MODE_SM_MULTITAP_PINYIN==MMI_current_input_mode
            ||INPUT_MODE_SM_PINYIN==MMI_current_input_mode
            ||INPUT_MODE_SM_STROKE==MMI_current_input_mode )
      {
         gPenEditorCandidatePage++;
         if (!mmi_pen_editor_get_candidates_by_page(gPenEditorCandidatePage))
         {
            gPenEditorCandidatePage--;
            return;
         }
         gPenEditorCandidateIndex = 0;
      }
   }
   coolsand_UI_lock_double_buffer();
   mmi_pen_editor_hide_candidate_area();
   //coolsand_UI_show_single_line_input_box(&gPenEditorCandidateInputBox);
   mmi_pen_editor_show_candidate_area(g_pen_editor_candidate_state.arrow_enable, PEN_EDITOR_CANDIDATE_ON_AND_FOCUS);
   coolsand_UI_unlock_double_buffer();
   coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_register_arrow_keys
* DESCRIPTION
*   \brief This function is to register the arrow keys when candidates is on.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_register_arrow_keys(void)
{
   SetKeyHandler(mmi_pen_editor_get_candidates_prev,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
   SetKeyHandler(mmi_pen_editor_get_candidates_next,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
   SetKeyHandler(mmi_pen_editor_get_candidates_prev_page,KEY_UP_ARROW,KEY_EVENT_DOWN);
   SetKeyHandler(mmi_pen_editor_get_candidates_next_page,KEY_DOWN_ARROW,KEY_EVENT_DOWN);
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_set_inline_text_item
* DESCRIPTION
*   \brief This function is to set inline text item.
*
* PARAMETERS
*  \param i : the inline text item
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/

void mmi_pen_editor_set_inline_text_item(const wgui_inline_item *i)
{
	S32 h;
   wgui_inline_item_text_edit *item_text = (wgui_inline_item_text_edit*)wgui_inline_items[wgui_inline_item_highlighted_index].item;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   switch ((item_text->input_type)& INPUT_TYPE_MASK)
   {
      case INPUT_TYPE_ALPHANUMERIC_LOWERCASE:
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH);
         break;
      case INPUT_TYPE_PHONE_NUMBER:
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER);
         break;
      case INPUT_TYPE_SAT_NUMERIC_PASSWORD:
      case INPUT_TYPE_KEYPAD_NUMERIC:
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER);
         break;
      case INPUT_TYPE_DECIMAL_NUMERIC:
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_DOT_TRAY);
         break;
      case INPUT_TYPE_NUMERIC:
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY);
         break;
     /*shaozhw 20060620 modify begin: for support sm_pinyin and sm_stroke */
     case INPUT_TYPE_MMI_SM_STROKE:
     case INPUT_TYPE_SM_STROKE:
     case INPUT_TYPE_SM_PINYIN:
         //wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_TRAY);
         break;
     /*shaozhw 20060620 modify end */
      default :
         MMI_ASSERT(0);
         break;
   }
   gPenEditorInputBoxType = PEN_EDITOR_INLINE_TEXT;
	 h = get_changed_height(gPenEditorVKState, gPenEditorVKCandidate);
   	mmi_pen_editor_resize_content_area(h);
   mmi_pen_editor_clear_and_show_virtual_keyboard_area();
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_right_soft_key_handler
* DESCRIPTION
*   \brief This function is to handle right soft key event.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_right_soft_key_handler(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   BOOL flag = FALSE;
   TBM_ENTRY(0x2824);
   if(UI_is_timer_start(mmi_pen_editor_confirm_selection))
   {
	    coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
	    mmi_pen_editor_confirm_selection();
	    flag = TRUE;
   }
   if (gPenEditorSelectCandidate&&(0==gPenEditorVKText.TextLength)&&(!flag))
   {
      //gPenEditorSelectCandidate = MMI_FALSE;
      mmi_pen_editor_set_candidate_state(MMI_FALSE);
       
      mmi_pen_editor_reset_candidate_index();
      /*Temp Solution for LSK Text Update*/
      mmi_pen_editor_set_left_soft_key();
      switch (gPenEditorInputBoxType)
      {
         case PEN_EDITOR_EMS_INPUT_BOX:
            if (UI_EMS_INPUT_BOX_STATE_MULTITAP & MMI_EMS_inputbox.flags)
            {
               EMS_inputbox_multitap_input_complete();
               mmi_pen_editor_backspace();
            }
		#ifdef __MMI_GB__
		gb_register_arrow_handler();
		#else
		register_EMS_inputbox_keys();
		#endif
            wgui_set_EMS_inputbox_RSK();
            break;
         case PEN_EDITOR_MULTILINE_INPUT_BOX:
         case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
         case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
            if (UI_MULTI_LINE_INPUT_BOX_STATE_MULTITAP & MMI_multiline_inputbox.flags)
            {
               multiline_inputbox_multitap_input_complete();
               mmi_pen_editor_backspace();
            }
             #ifdef __MMI_GB__
		gb_register_arrow_handler();
		#else
               register_multiline_inputbox_keys();
		#endif
            if ( PEN_EDITOR_DICT_MULTILINE_INPUT_BOX == gPenEditorInputBoxType)
            {
               cat201_register_key_func();
            }
            wgui_set_RSK();
            break;
         case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
            wgui_singleline_inputbox_RSK_label_clear=0;
            if (UI_SINGLE_LINE_INPUT_BOX_STATE_MULTITAP & MMI_singleline_inputbox.flags)
            {
               singleline_inputbox_multitap_input_complete();
               mmi_pen_editor_backspace();
            }
            register_singleline_inputbox_keys();
            wgui_set_singleline_RSK();
            break;
         case PEN_EDITOR_INLINE_TEXT:
            if (UI_SINGLE_LINE_INPUT_BOX_STATE_MULTITAP & MMI_inline_singleline_inputbox.flags)
            {
               inline_edit_singleline_inputbox_multitap_input_complete();
               mmi_pen_editor_backspace();
            }
            mmi_pen_editor_set_right_soft_key_inline_text();
            break;
         case PEN_EDITOR_DATE:
         case PEN_EDITOR_IP4:
         case PEN_EDITOR_TIME:
         case PEN_EDITOR_TIME_PERIOD:
            set_right_softkey_label(wgui_inline_edit_get_RSK_string());
            set_right_softkey_icon(NULL);
            register_right_softkey_handler();
            set_right_softkey_function(execute_wgui_inline_edit_RSK_function,KEY_EVENT_UP);
            redraw_right_softkey();
            break;
         default :
            break;
      }

	gdi_layer_lock_frame_buffer();
       
      mmi_pen_editor_hide_candidate_area();
      if (PEN_EDITOR_VK_OFF==gPenEditorVKState)
      {
	    gPenEditorVKState = PEN_EDITOR_VK_ON;
          
         if (!(MMI_GUI_VIRTUAL_KEYBOARD_TRAY==MMI_virtual_keyboard.lang_type
            ||MMI_GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY==MMI_virtual_keyboard.lang_type
            ||MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY==MMI_virtual_keyboard.lang_type
            ||MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_DOT_TRAY==MMI_virtual_keyboard.lang_type))
         {
	     if (PEN_EDITOR_DICT_MULTILINE_INPUT_BOX != gPenEditorInputBoxType)
            {
			S32 h = get_changed_height(gPenEditorVKState, gPenEditorVKCandidate) -1;
			mmi_pen_editor_resize_content_area(h);
            }
			 
            if (gPenEditorVKCandidate)
            {
               coolsand_UI_move_single_line_input_box(&gPenEditorCandidateInputBox, gPenEditorCandidateInputBox.x, MMI_virtual_keyboard.y-gPenEditorCandidateInputBox.height-2);
                
               mmi_pen_editor_hide_candidate_area();
            }
         }
         #ifdef __MMI_HAND_WRITING__
         mmi_pen_editor_change_handwriting_area();
         #endif
         mmi_pen_editor_clear_and_show_virtual_keyboard_area();
      }

      gdi_layer_unlock_frame_buffer();
      gdi_layer_blt_previous(0, 0, UI_device_width -1,UI_device_height -1 );
      TBM_EXIT(0x2824);
      return;
   }
   if (MMI_GUI_VIRTUAL_KEYBOARD_PINYIN == MMI_virtual_keyboard.lang_type
		 #ifdef __MMI_TR_CHINESE_VK__
		||MMI_GUI_VIRTUAL_KEYBOARD_BOPOMO == MMI_virtual_keyboard.lang_type
		#endif
        )
   {
      if (INPUT_MODE_MMI_MULTITAP_BPMF==MMI_current_input_mode
          ||INPUT_MODE_MMI_MULTITAP_PINYIN==MMI_current_input_mode)
      {
         mmi_pen_editor_backspace();
      }
      else
      {
         if (mmi_pen_editor_vk_delete_character(&gPenEditorVKText))
         {
             
            mmi_pen_editor_reset_candidate_index();
            mmi_pen_editor_hide_candidate_area();
	      if (mmi_pen_editor_get_candidates_by_vk())
            {
               //gPenEditorSelectCandidate = MMI_TRUE;
               mmi_pen_editor_set_candidate_state(MMI_TRUE);
               /*Temp Solution for LSK Text Update*/
                
               //mmi_pen_editor_store_LSK_Text();
               mmi_pen_editor_set_left_soft_key_select();
               set_left_softkey_function(mmi_pen_editor_left_soft_key_handler,KEY_EVENT_UP);
               set_left_softkey_function(mmi_pen_editor_hold_confirm,KEY_EVENT_DOWN);
               switch (gPenEditorInputBoxType)
               {
                  case PEN_EDITOR_EMS_INPUT_BOX:
                  case PEN_EDITOR_MULTILINE_INPUT_BOX:
                  case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
                  case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
                     mmi_pen_editor_register_arrow_keys();
                     break;
                  default :
                     break;
               }
            }
            else
            {
               //gPenEditorSelectCandidate = MMI_FALSE;
               mmi_pen_editor_set_candidate_state(MMI_FALSE);
               /*Temp Solution for LSK Text Update*/
               mmi_pen_editor_set_left_soft_key();
               switch (gPenEditorInputBoxType)
               {
                  case PEN_EDITOR_EMS_INPUT_BOX:
			#ifdef __MMI_GB__
			gb_register_arrow_handler();
			#else
			register_EMS_inputbox_keys();
			#endif
                     break;
                  case PEN_EDITOR_MULTILINE_INPUT_BOX:
                  case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
                  case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
                      #ifdef __MMI_GB__
			gb_register_arrow_handler();
			#else
	               register_multiline_inputbox_keys();
			#endif
                     if (PEN_EDITOR_DICT_MULTILINE_INPUT_BOX == gPenEditorInputBoxType)
                     {
                        cat201_register_key_func();
                     }
                     break;
                  default :
                     break;
               }
            }
            wgui_set_virtual_keyboard_display_area((U8*)gPenEditorVKText.VKText, MMI_TRUE);
            if (0 == gPenEditorVKText.TextLength)
            {
               switch (gPenEditorInputBoxType)
               {
                  case PEN_EDITOR_EMS_INPUT_BOX:
                     handle_category28_input();
                     wgui_set_EMS_inputbox_RSK();
                     break;
                  case PEN_EDITOR_MULTILINE_INPUT_BOX:
                  case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
                  case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
                     wgui_handle_inputbox_input();
                     wgui_set_RSK();
                     break;
                  default :
                     break;
               }
            }
         }
         else
         {
            mmi_pen_editor_backspace();
         }
      }
   }
   else
   {
      mmi_pen_editor_backspace();
   }
   #ifdef __MMI_HAND_WRITING__
   mmi_pen_editor_change_handwriting_area();
   #endif
   TBM_EXIT(0x2824);
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_left_soft_key_handler
* DESCRIPTION
*   \brief This function is to handle left soft key event.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_left_soft_key_handler(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   MMI_ASSERT(!(mmi_pen_editor_left_soft_key_handler==gPenEditorLSKfunction));
   TBM_ENTRY(0x2823);
   if (gPenEditorSelectCandidate)
   {
      UI_character_type ch,ch1,ch2;
      g_pen_editor_hold_confirm = MMI_FALSE;
      coolsand_UI_lock_double_buffer();
     coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
    //  COS_KillTimer(GetMmiTaskHandle(MOD_TP_TASK),TPTimerIdHandwritingConfirm );
      ch1 = *(gPenEditorCandidateInputBox.current_text_p-2);
      ch2 = *(gPenEditorCandidateInputBox.current_text_p-1);
      ch = ch1 | (ch2 << 8);
      mmi_pen_editor_input_character(ch, MMI_FALSE);
      switch (gPenEditorInputBoxType)
      {
         case PEN_EDITOR_EMS_INPUT_BOX:
            wgui_set_EMS_inputbox_RSK();
            break;
         case PEN_EDITOR_MULTILINE_INPUT_BOX:
         case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
            wgui_set_RSK();
            break;
         case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
            wgui_singleline_inputbox_RSK_label_clear=0;
            wgui_set_singleline_RSK();
            break;
         case PEN_EDITOR_INLINE_TEXT:
            mmi_pen_editor_set_right_soft_key_inline_text();
            break;
         case PEN_EDITOR_DATE:
         case PEN_EDITOR_IP4:
         case PEN_EDITOR_TIME:
         case PEN_EDITOR_TIME_PERIOD:
               set_right_softkey_label(wgui_inline_edit_get_RSK_string());
               set_right_softkey_icon(NULL);
               register_right_softkey_handler();
               set_right_softkey_function(execute_wgui_inline_edit_RSK_function,KEY_EVENT_UP);
               redraw_right_softkey();
            break;
         default :
            break;
      }
      /*Temp Solution for checking Chinese Characters*/
	  mmi_trace(1," mmi_pen_editor_left_soft_key_handler 0x%x  ",ch);
      if (ch>=0x4e00 && ch <=0x9fff)
      {
          
         mmi_pen_editor_reset_candidate_index();
         if (mmi_pen_editor_get_candidates_by_phrase(ch))
         {
            //gPenEditorSelectCandidate = MMI_TRUE;
            mmi_pen_editor_set_candidate_state(MMI_TRUE);
            /*Temp Solution for LSK Text Update*/
             
            //mmi_pen_editor_store_LSK_Text();
            mmi_pen_editor_set_left_soft_key_select();
            set_left_softkey_function(mmi_pen_editor_left_soft_key_handler,KEY_EVENT_UP);
            set_left_softkey_function(mmi_pen_editor_hold_confirm,KEY_EVENT_DOWN);
            switch (gPenEditorInputBoxType)
            {
               case PEN_EDITOR_EMS_INPUT_BOX:
               case PEN_EDITOR_MULTILINE_INPUT_BOX:
               case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
               case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
                  mmi_pen_editor_register_arrow_keys();
                  break;
               default :
                  break;
            }
            if (gPenEditorCandidateInputBox.y >= (MMI_multitap_y - 1))
            {
               gPenEditorVKState = PEN_EDITOR_VK_OFF;
            }
            mmi_pen_editor_set_right_softkey(PEN_EDITOR_RSK_BY_STATE);
         }
      }
      coolsand_UI_unlock_double_buffer();
      coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
   }
   else
   {
	if(gPenEditorLSKfunction == NULL)
	{
		mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR:  gPenEditorLSKfunction is NULL");
	}
	else
	{
		gPenEditorLSKfunction();
	}
   }
   #ifdef __MMI_HAND_WRITING__
   mmi_pen_editor_change_handwriting_area();
   #endif
   TBM_EXIT(0x2823);
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_switch_input_method
* DESCRIPTION
*   \brief This function is to switch input method.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_switch_input_method(void)
{

  S32 h;
     coolsand_UI_lock_double_buffer();

   if (gPenEditorSelectCandidate||gPenEditorVKText.TextLength>0)
   {
       
       
      if (gPenEditorSelectCandidate)
      {
         coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
       //  COS_KillTimer(GetMmiTaskHandle(MOD_TP_TASK),TPTimerIdHandwritingConfirm );
         mmi_pen_editor_confirm_selection();
      }
      //gPenEditorSelectCandidate = MMI_FALSE;
      mmi_pen_editor_set_candidate_state(MMI_FALSE);
      mmi_pen_editor_reset_vk_text(&gPenEditorVKText);
      wgui_set_virtual_keyboard_display_area((U8*)gPenEditorVKText.VKText, MMI_FALSE);
      mmi_pen_editor_hide_candidate_area();
      /*Temp Solution for LSK Text Update*/
      mmi_pen_editor_set_left_soft_key();
   }
    
   switch (gPenEditorInputBoxType)
   {
	case PEN_EDITOR_EMS_INPUT_BOX:
		handle_category28_change_input_mode();
	break;
	case PEN_EDITOR_MULTILINE_INPUT_BOX:
	case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
	case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:

		wgui_handle_inputbox_change_input_mode();

	break;
	case PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX:
		wgui_handle_qsearch_singleline_inputbox_change_input_mode();
	break;
	case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
		wgui_handle_singleline_inputbox_change_input_mode();
	break;
	case PEN_EDITOR_INLINE_TEXT:
		wgui_handle_inline_singleline_inputbox_change_input_mode();
	break;
	default:
		MMI_ASSERT(0);
	break;
   }
      
	//wangzl:070616 add B the handler of # key may be modified
	if (wgui_handle_inputbox_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN]||
	   handle_category28_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN]||
	   wgui_handle_qsearch_singleline_inputbox_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN]||
	   wgui_handle_singleline_inputbox_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN]||
	   wgui_handle_inline_singleline_inputbox_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN]||
 	   wgui_handle_inputbox_change_input_mode_bymultitapthai == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN])
	{
		SetKeyHandler(mmi_pen_editor_switch_input_method,KEY_POUND,KEY_EVENT_DOWN);
	}
	//wangzl:070616 add E
	
	
   if (MMI_GUI_VIRTUAL_KEYBOARD_TRAY != MMI_virtual_keyboard.lang_type)
   {	
	      mmi_pen_editor_hide_virtual_keyboard_area();
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_TRAY);//fix bug 14773 by qiff 20/10/2009

		//mmi_pen_editor_set_virtual_keyboard_by_input_mode(MMI_current_input_mode);
		if (gPenEditorVKCandidate) 
		{
	            coolsand_UI_move_single_line_input_box(&gPenEditorCandidateInputBox, gPenEditorCandidateInputBox.x, MMI_virtual_keyboard.y-gPenEditorCandidateInputBox.height-2);
	            mmi_pen_editor_hide_candidate_area();
	        }
	  	 h = get_changed_height(gPenEditorVKState, gPenEditorVKCandidate)+1;//fix bug 14773 by qiff 20/10/2009
   		mmi_pen_editor_resize_content_area(h);
   }
    
   if (PEN_EDITOR_INLINE_TEXT==gPenEditorInputBoxType
      || PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX == gPenEditorInputBoxType)
   {
      s32 temp_multiline_height = MMI_multiline_inputbox.height;
      MMI_BOOL temp_VKCandidate = gPenEditorVKCandidate;
      U8 temp_current_input_mode = MMI_current_input_mode;
      mmi_gui_virtual_keyboard_language_enum keyboard_type = MMI_virtual_keyboard.lang_type;
      mmi_pen_editor_vk_list_menu_resize();
      MMI_current_input_mode = temp_current_input_mode;
      inputbox_change_mode();
      set_MMI_current_input_type();
      if ( PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX == gPenEditorInputBoxType )
      {
         coolsand_UI_resize_multi_line_input_box(&MMI_multiline_inputbox,MMI_multiline_inputbox.width,temp_multiline_height);
         gPenEditorVKCandidate = temp_VKCandidate;
         redraw_fixed_list();
         redraw_current_inline_item();
      }
      wgui_setup_virtual_keyboard(keyboard_type);
      if (gPenEditorVKCandidate)
      {
         coolsand_UI_move_single_line_input_box(&gPenEditorCandidateInputBox, gPenEditorCandidateInputBox.x, MMI_virtual_keyboard.y-gPenEditorCandidateInputBox.height-2);
         mmi_pen_editor_hide_candidate_area();
      }
   }
   if (PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX==gPenEditorInputBoxType)
   {
      cat200_virtual_keypad_callback();
     // RedrawCategoryControlledArea200Screen(NULL);
   }
   else
   {

      mmi_pen_editor_clear_and_show_virtual_keyboard_area();
   }
    
    
   //mmi_pen_editor_clear_key_handler();
   if (INPUT_MODE_TR_MULTITAP_BOPOMO == MMI_current_input_mode || INPUT_MODE_SM_MULTITAP_PINYIN == MMI_current_input_mode)
   {
	   mmi_pen_editor_bpmf_pinyin_create_multitap(MMI_current_input_mode);
	   mmi_pen_editor_bpmf_pinyin_register_multitap_function();
   }
   mmi_pen_editor_set_CSK_content();

   switch(MMI_current_input_mode)
   {//Disable handwriting when the handwriting language is not supported.	
      case INPUT_MODE_MULTITAP_UPPERCASE_ABC:	
      case INPUT_MODE_MULTITAP_UPPERCASE_ABC_NO_NUMERIC:
      case INPUT_MODE_SMART_UPPERCASE_ABC:
      case INPUT_MODE_MULTITAP_LOWERCASE_ABC:	
      case INPUT_MODE_MMI_LOWERCASE_ABC:	
      case INPUT_MODE_MULTITAP_LOWERCASE_ABC_NO_NUMERIC:
      case INPUT_MODE_SMART_LOWERCASE_ABC:
      case INPUT_MODE_123: 
      case INPUT_MODE_MMI_NUMERIC:	
      case INPUT_MODE_123_SYMBOLS: 
      case INPUT_MODE_MMI_MULTITAP_PINYIN:       
      case INPUT_MODE_MMI_SM_STROKE:
      case INPUT_MODE_SM_MULTITAP_PINYIN:
      case INPUT_MODE_SM_STROKE:
      case INPUT_MODE_SM_PINYIN:
   //   case INPUT_MODE_MMI_MULTITAP_BPMF:
     // case INPUT_MODE_TR_MULTITAP_BOPOMO:
     // case INPUT_MODE_TR_STROKE:
     // case INPUT_MODE_MMI_TR_STROKE:
    //  case INPUT_MODE_TR_BOPOMO:
	  	gPenEditorEnableHandWriting = MMI_TRUE;
        //  mmi_pen_resume_capture_strokes( );
          break;
      default:
	  	gPenEditorEnableHandWriting = MMI_FALSE;
       //   mmi_pen_pause_capture_strokes( );
          break;
   }
   #ifdef __MMI_HAND_WRITING__
   mmi_pen_editor_change_handwriting_area();
   #endif
   coolsand_UI_unlock_double_buffer();
   coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);

}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_vk_caps_lock
* DESCRIPTION
*   \brief This function is to handle caps lock in virtual keyboard.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/

void mmi_pen_editor_vk_caps_lock(void)
{
	gui_virtual_keyboard_language_enum templangType = 0;
   if (PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX==gPenEditorInputBoxType)
   {
      return;
   }
   /*To Do : Check Other Input Method Mode*/
	switch(MMI_virtual_keyboard.lang_type)
	{
		case MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH_UPPERCASE;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_LOWERCASE_ABC:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_ABC;
					break;
				case INPUT_MODE_SMART_LOWERCASE_ABC:
					MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_ABC;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_ABC;
					break;
			}
			break;
		case MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH_UPPERCASE:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_UPPERCASE_ABC:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_ABC;
					break;
				case INPUT_MODE_SMART_UPPERCASE_ABC:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_ABC;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_ABC;
					break;
			}
			break;
		#ifdef __MMI_FRENCH_VK__
		case MMI_GUI_VIRTUAL_KEYBOARD_FRENCH:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_FRENCH_UPPERCASE;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_LOWERCASE_FRENCH:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_FRENCH;
					break;
				case INPUT_MODE_SMART_LOWERCASE_FRENCH:
					MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_FRENCH;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_FRENCH;
					break;
			}
			break;
		case MMI_GUI_VIRTUAL_KEYBOARD_FRENCH_UPPERCASE:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_FRENCH;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_UPPERCASE_FRENCH:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_FRENCH;
					break;
				case INPUT_MODE_SMART_UPPERCASE_FRENCH:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_FRENCH;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_FRENCH;
					break;
			}
			break;
		#endif
		#ifdef __MMI_GERMAN_VK__
		case MMI_GUI_VIRTUAL_KEYBOARD_GERMAN:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_GERMAN_UPPERCASE;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_LOWERCASE_GERMAN:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_GERMAN;
					break;
				case INPUT_MODE_SMART_LOWERCASE_GERMAN:
					MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_GERMAN;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_GERMAN;
					break;
			}
			break;
		case MMI_GUI_VIRTUAL_KEYBOARD_GERMAN_UPPERCASE:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_GERMAN;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_UPPERCASE_GERMAN:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_GERMAN;
					break;
				case INPUT_MODE_SMART_UPPERCASE_GERMAN:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_GERMAN;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_GERMAN;
					break;
			}
			break;
		#endif
		#ifdef __MMI_RUSSIAN_VK__
		case MMI_GUI_VIRTUAL_KEYBOARD_RUSSIAN:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_RUSSIAN_UPPERCASE;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_LOWERCASE_RUSSIAN:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_RUSSIAN;
					break;
				case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:
					MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_RUSSIAN;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_RUSSIAN;
					break;
			}
			break;
		case MMI_GUI_VIRTUAL_KEYBOARD_RUSSIAN_UPPERCASE:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_RUSSIAN;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_UPPERCASE_RUSSIAN:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_RUSSIAN;
					break;
				case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_RUSSIAN;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_RUSSIAN;
					break;
			}
			break;
		#endif
		#ifdef __MMI_SPANISH_VK__
		case MMI_GUI_VIRTUAL_KEYBOARD_SPANISH:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_SPANISH_UPPERCASE;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_LOWERCASE_SPANISH:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_SPANISH;
					break;
				case INPUT_MODE_SMART_LOWERCASE_SPANISH:
					MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_SPANISH;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_SPANISH;
					break;
			}
			break;
		case MMI_GUI_VIRTUAL_KEYBOARD_SPANISH_UPPERCASE:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_SPANISH;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_UPPERCASE_SPANISH:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_SPANISH;
					break;
				case INPUT_MODE_SMART_UPPERCASE_SPANISH:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_SPANISH;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_SPANISH;
					break;
			}
			break;
		#endif
		#ifdef __MMI_TURKISH_VK__
		case MMI_GUI_VIRTUAL_KEYBOARD_TURKISH:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_TURKISH_UPPERCASE;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_LOWERCASE_TURKISH:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_TURKISH;
					break;
				case INPUT_MODE_SMART_LOWERCASE_TURKISH:
					MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_TURKISH;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_TURKISH;
					break;
			}
			break;
		case MMI_GUI_VIRTUAL_KEYBOARD_TURKISH_UPPERCASE:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_TURKISH;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_UPPERCASE_TURKISH:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_TURKISH;
					break;
				case INPUT_MODE_SMART_UPPERCASE_TURKISH:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_TURKISH;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_TURKISH;
					break;
			}
			break;
		#endif
		#ifdef __MMI_PORTUGUESE_VK__
		case MMI_GUI_VIRTUAL_KEYBOARD_PORTUGUESE:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_PORTUGUESE_UPPERCASE;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_LOWERCASE_PORTUGUESE:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_PORTUGUESE;
					break;
				case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
					MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_PORTUGUESE;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_PORTUGUESE;
					break;
			}
			break;
		case MMI_GUI_VIRTUAL_KEYBOARD_PORTUGUESE_UPPERCASE:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_PORTUGUESE;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_UPPERCASE_PORTUGUESE:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_PORTUGUESE;
					break;
				case INPUT_MODE_SMART_UPPERCASE_MALAY:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_PORTUGUESE;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_PORTUGUESE;
					break;
			}
			break;
		#endif
		#ifdef __MMI_MALAY_VK__
		case MMI_GUI_VIRTUAL_KEYBOARD_MALAY:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_MALAY_UPPERCASE;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_LOWERCASE_MALAY:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_MALAY;
					break;
				case INPUT_MODE_SMART_LOWERCASE_MALAY:
					MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_MALAY;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_MALAY;
					break;
			}
			break;
		case MMI_GUI_VIRTUAL_KEYBOARD_MALAY_UPPERCASE:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_MALAY;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_UPPERCASE_MALAY:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_MALAY;
					break;
				case INPUT_MODE_SMART_UPPERCASE_MALAY:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_MALAY;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_MALAY;
					break;
			}
			break;
		#endif
		#ifdef __MMI_INDONESIAN_VK__
		case MMI_GUI_VIRTUAL_KEYBOARD_INDONESIAN:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_INDONESIAN_UPPERCASE;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_LOWERCASE_INDONESIAN:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_INDONESIAN;
					break;
				case INPUT_MODE_SMART_LOWERCASE_INDONESIAN:
					MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_INDONESIAN;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_INDONESIAN;
					break;
			}
			break;
		case MMI_GUI_VIRTUAL_KEYBOARD_INDONESIAN_UPPERCASE:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_INDONESIAN;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_UPPERCASE_INDONESIAN:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_INDONESIAN;
					break;
				case INPUT_MODE_SMART_UPPERCASE_INDONESIAN:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_INDONESIAN;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_INDONESIAN;
					break;
			}
			break;
		#endif
		#ifdef __MMI_ITALIAN_VK__
		case MMI_GUI_VIRTUAL_KEYBOARD_ITALIAN:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_ITALIAN_UPPERCASE;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_LOWERCASE_ITALIAN:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_ITALIAN;
					break;
				case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
					MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_ITALIAN;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_ITALIAN;
					break;
			}
			break;
		case MMI_GUI_VIRTUAL_KEYBOARD_ITALIAN_UPPERCASE:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_ITALIAN;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_UPPERCASE_ITALIAN:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_ITALIAN;
					break;
				case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_ITALIAN;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_ITALIAN;
					break;
			}
			break;
		#endif
		#ifdef __MMI_VIETNAMESE_VK__
			case MMI_GUI_VIRTUAL_KEYBOARD_VIETNAMESE:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_VIETNAMESE_UPPERCASE;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_LOWERCASE_VIETNAMESE:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_VIETNAMESE;
					break;
				case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
					MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_VIETNAMESE;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_VIETNAMESE;
					break;
			}
			break;
		case MMI_GUI_VIRTUAL_KEYBOARD_VIETNAMESE_UPPERCASE:
			templangType = MMI_GUI_VIRTUAL_KEYBOARD_VIETNAMESE;
			switch(MMI_current_input_mode)
			{
				case INPUT_MODE_MULTITAP_UPPERCASE_VIETNAMESE:
					MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_VIETNAMESE;
					break;
				case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_VIETNAMESE;
					break;
				default:
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_VIETNAMESE;
					break;
			}
			break;
		#endif
		default:
			//MMI_ASSERT(0);
			break;
	}
	wgui_setup_virtual_keyboard(templangType);
	wgui_show_virtual_keyboard();
	switch (gPenEditorInputBoxType)
	{
		case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
		case PEN_EDITOR_MULTILINE_INPUT_BOX:
		case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
			change_multiline_inputbox_mode(MMI_current_input_mode);
			wgui_redraw_input_information_bar();
			break;
		case PEN_EDITOR_EMS_INPUT_BOX:
			change_EMS_inputbox_mode(MMI_current_input_mode);
			wgui_EMS_redraw_input_information_bar();
			break;
		case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
			change_singleline_inputbox_mode(MMI_current_input_mode);
			break;
		case PEN_EDITOR_INLINE_TEXT:
			 
			inline_edit_change_singleline_inputbox_mode(MMI_current_input_mode);
			break;
		default :
			break;
	}
	#if  0
   if (MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH==MMI_virtual_keyboard.lang_type)
   {
   			if(MMI_current_input_mode == INPUT_MODE_MULTITAP_LOWERCASE_ABC)
   			{
      wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH_UPPERCASE);
      wgui_show_virtual_keyboard();
      /*Change Input Method Mode to Upper Case*/
      MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_ABC;
       
      switch (gPenEditorInputBoxType)
      {
         case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
         case PEN_EDITOR_MULTILINE_INPUT_BOX:
         case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
			            change_multiline_inputbox_mode(MMI_current_input_mode);
            wgui_redraw_input_information_bar();
            break;
         case PEN_EDITOR_EMS_INPUT_BOX:
			            change_EMS_inputbox_mode(MMI_current_input_mode);
            wgui_EMS_redraw_input_information_bar();
            break;
         case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
			            change_singleline_inputbox_mode(MMI_current_input_mode);
            break;
         case PEN_EDITOR_INLINE_TEXT:
	     
				    inline_edit_change_singleline_inputbox_mode(MMI_current_input_mode);
            break;
         default :
            break;
      }
   }
			else 
			{
			      wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH_UPPERCASE);
			      wgui_show_virtual_keyboard();
			      /*Change Input Method Mode to Upper Case*/
			      MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_ABC;
			       
			      switch (gPenEditorInputBoxType)
			      {
			         case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
			         case PEN_EDITOR_MULTILINE_INPUT_BOX:
			         case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
			            change_multiline_inputbox_mode(MMI_current_input_mode);
			            wgui_redraw_input_information_bar();
			            break;
			         case PEN_EDITOR_EMS_INPUT_BOX:
			            change_EMS_inputbox_mode(MMI_current_input_mode);
			            wgui_EMS_redraw_input_information_bar();
			            break;
			         case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
			            change_singleline_inputbox_mode(MMI_current_input_mode);
			            break;
			         case PEN_EDITOR_INLINE_TEXT:
				     
				    inline_edit_change_singleline_inputbox_mode(MMI_current_input_mode);
			            break;
			         default :
			            break;
			      }			
			}

   }
   else if (MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH_UPPERCASE==MMI_virtual_keyboard.lang_type)
   {
	    	if(MMI_current_input_mode == INPUT_MODE_MULTITAP_UPPERCASE_ABC)
    		{
      wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH);
      wgui_show_virtual_keyboard();
      /*Change Input Method Mode to Lower Case*/
      MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_ABC;
       
      switch (gPenEditorInputBoxType)
      {
         case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
         case PEN_EDITOR_MULTILINE_INPUT_BOX:
         case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
							change_multiline_inputbox_mode(MMI_current_input_mode);
            wgui_redraw_input_information_bar();
            break;
         case PEN_EDITOR_EMS_INPUT_BOX:
							change_EMS_inputbox_mode(MMI_current_input_mode);
							wgui_EMS_redraw_input_information_bar();
						break;
						case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
							change_singleline_inputbox_mode(MMI_current_input_mode);
						break;
						case PEN_EDITOR_INLINE_TEXT:
						 
							inline_edit_change_singleline_inputbox_mode(MMI_current_input_mode);
						break;
						default :
						break;
					}			
			}
			else
			{
					wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH);
					wgui_show_virtual_keyboard();
					/*Change Input Method Mode to Lower Case*/
					MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_ABC;
					 
					switch (gPenEditorInputBoxType)
					{
						case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
						case PEN_EDITOR_MULTILINE_INPUT_BOX:
						case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
							change_multiline_inputbox_mode(MMI_current_input_mode);
							wgui_redraw_input_information_bar();
						break;
						case PEN_EDITOR_EMS_INPUT_BOX:
							change_EMS_inputbox_mode(MMI_current_input_mode);
            wgui_EMS_redraw_input_information_bar();
            break;
         case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
							change_singleline_inputbox_mode(MMI_current_input_mode);
            break;
         case PEN_EDITOR_INLINE_TEXT:
	     
							inline_edit_change_singleline_inputbox_mode(MMI_current_input_mode);
            break;
         default :
            break;
      }
   }
      
   }
	#endif
    
   set_MMI_current_input_type();
   mmi_pen_editor_set_CSK_content();
//qiff add here for TP caps lock re_register # handler
	if (wgui_handle_inputbox_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN]||
	   handle_category28_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN]||
	   wgui_handle_qsearch_singleline_inputbox_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN]||
	   wgui_handle_singleline_inputbox_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN]||
	   wgui_handle_inline_singleline_inputbox_change_input_mode == currKeyFuncPtrs[KEY_POUND][KEY_EVENT_DOWN])
	{
		SetKeyHandler(mmi_pen_editor_switch_input_method,KEY_POUND,KEY_EVENT_DOWN);
	}   
   //mmi_pen_editor_clear_key_handler();
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_vk_insertcharacter
* DESCRIPTION
*   \brief This function is to handle caps lock in virtual keyboard.
*
* PARAMETERS
*  \param vk_para : the vk_para returned by pen event
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_vk_insertcharacter(mmi_gui_pen_event_param_struct vk_para)
{
  if (MMI_GUI_VIRTUAL_KEYBOARD_PINYIN == MMI_virtual_keyboard.lang_type
	 #ifdef __MMI_TR_CHINESE_VK__
	||MMI_GUI_VIRTUAL_KEYBOARD_BOPOMO == MMI_virtual_keyboard.lang_type
	#endif
   )
   {
      if (INPUT_MODE_MMI_MULTITAP_BPMF==MMI_current_input_mode
          ||INPUT_MODE_MMI_MULTITAP_PINYIN==MMI_current_input_mode)
      {
         mmi_pen_editor_input_character((UI_character_type)vk_para._u.i, MMI_FALSE);
      }
      else
      {
         if (!((vk_para._u.i>0x60&&vk_para._u.i<0x7b)||(vk_para._u.i>0x3104&&vk_para._u.i<0x3130)))
         {
             
            if (0 == gPenEditorVKText.TextLength && !gPenEditorSelectCandidate)
            {
               mmi_pen_editor_input_character((UI_character_type)vk_para._u.i, MMI_FALSE);
            }
         }
         else if (mmi_pen_editor_vk_insert_character(&gPenEditorVKText,(UI_character_type)vk_para._u.i))
         {
            /*Check the input is valid or not*/
            if (mmi_pen_editor_get_candidates_by_vk())
            {
               switch (gPenEditorInputBoxType)
               {
                  case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
                  case PEN_EDITOR_MULTILINE_INPUT_BOX:
                  case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
                     wgui_inputbox_RSK_label_clear = 1;
                     break;
                  case PEN_EDITOR_EMS_INPUT_BOX:
                     category28_RSK_label_clear= 1;
                     break;
                  default :
                     break;
               }
               /*Temp Solution for LSK Text Update*/
                
               if (gPenEditorVKText.TextLength == 1 && MMI_FALSE == gPenEditorSelectCandidate)
               {
                  mmi_pen_editor_store_LSK_Text();
               }
               mmi_pen_editor_set_left_soft_key_select();
               set_left_softkey_function(mmi_pen_editor_left_soft_key_handler,KEY_EVENT_UP);
               set_left_softkey_function(mmi_pen_editor_hold_confirm,KEY_EVENT_DOWN);
               //gPenEditorSelectCandidate = MMI_TRUE;
               mmi_pen_editor_set_candidate_state(MMI_TRUE);
               switch (gPenEditorInputBoxType)
               {
                  case PEN_EDITOR_EMS_INPUT_BOX:
                  case PEN_EDITOR_MULTILINE_INPUT_BOX:
                  case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
                  case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
                     mmi_pen_editor_register_arrow_keys();
                     break;
                  default :
                     break;
               }
               mmi_pen_editor_set_right_softkey(PEN_EDITOR_RSK_CLEAR);
            }
            else
            {
               mmi_pen_editor_vk_delete_character(&gPenEditorVKText);
            }
            wgui_set_virtual_keyboard_display_area((U8*)gPenEditorVKText.VKText, MMI_TRUE);
         }
         else if (!(vk_para._u.i>0x60&&vk_para._u.i<0x7b)&&(MMI_GUI_VIRTUAL_KEYBOARD_PINYIN==MMI_virtual_keyboard.lang_type))
         {
            mmi_pen_editor_input_character((UI_character_type)vk_para._u.i, MMI_FALSE);
         }
      }
   }
   else
   {
      if (0 != vk_para._u.i)
      {
         mmi_pen_editor_input_character((UI_character_type)vk_para._u.i, MMI_FALSE);
      }
      else
      {
         /*Test for Multitap*/
      }
   }
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_vk_hide
* DESCRIPTION
*   \brief This function is to handle hide in virtual keyboard.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_vk_hide(void)
{
	S32 h;
   if (PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX==gPenEditorInputBoxType)
   {
       
      if (MMI_GUI_VIRTUAL_KEYBOARD_PINYIN==MMI_virtual_keyboard.lang_type
          ||MMI_GUI_VIRTUAL_KEYBOARD_NUMBER==MMI_virtual_keyboard.lang_type
          ||MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH ==MMI_virtual_keyboard.lang_type
          ||MMI_GUI_VIRTUAL_KEYBOARD_SYMBOL == MMI_virtual_keyboard.lang_type
	  ||MMI_GUI_VIRTUAL_KEYBOARD_CHINESE_SYMBOL == MMI_virtual_keyboard.lang_type
	  #ifdef __MMI_TR_CHINESE_VK__
	  ||MMI_GUI_VIRTUAL_KEYBOARD_BOPOMO==MMI_virtual_keyboard.lang_type
	  #endif
	  )
      {
         mmi_pen_editor_hide_virtual_keyboard_area();
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_TRAY);
		 h = get_changed_height(gPenEditorVKState, gPenEditorVKCandidate);
		mmi_pen_editor_resize_content_area(h);
         RedrawCategoryControlledArea200Screen(NULL);
      }
      return;
   }
   if (MMI_GUI_VIRTUAL_KEYBOARD_TRAY != MMI_virtual_keyboard.lang_type)
   {
       
      if ( MMI_GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER == MMI_virtual_keyboard.lang_type)
      {
         return;  
      }
      coolsand_UI_lock_double_buffer();
	  if (MMI_GUI_VIRTUAL_KEYBOARD_PINYIN==MMI_virtual_keyboard.lang_type
	  #ifdef __MMI_TR_CHINESE_VK__
	  ||MMI_GUI_VIRTUAL_KEYBOARD_BOPOMO==MMI_virtual_keyboard.lang_type
	  #endif
	  )
      {
		 if (gPenEditorVKCandidate)
         {
            gPenEditorVKCandidate=MMI_FALSE;
         }
         if (gPenEditorSelectCandidate)
         {
            //gPenEditorSelectCandidate = MMI_FALSE;
            mmi_pen_editor_set_candidate_state(MMI_FALSE);
            /*Temp Solution for LSK Text Update*/
            mmi_pen_editor_set_left_soft_key();
            switch (gPenEditorInputBoxType)
            {
               case PEN_EDITOR_EMS_INPUT_BOX:
			#ifdef __MMI_GB__
			gb_register_arrow_handler();
			#else
			register_EMS_inputbox_keys();
			#endif
			break;
               case PEN_EDITOR_MULTILINE_INPUT_BOX:
               case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
               case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
                   	#ifdef __MMI_GB__
			gb_register_arrow_handler();
			#else
	               register_multiline_inputbox_keys();
			#endif
                  if (PEN_EDITOR_DICT_MULTILINE_INPUT_BOX == gPenEditorInputBoxType)
                  {
                    cat201_register_key_func();
                  }
                  break;
               default :
                  break;
            }
            mmi_pen_editor_reset_vk_text(&gPenEditorVKText);
            wgui_set_virtual_keyboard_display_area((U8*)gPenEditorVKText.VKText, MMI_FALSE);
            switch (gPenEditorInputBoxType)
            {
               case PEN_EDITOR_MULTILINE_INPUT_BOX:
               case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
               case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
                   
                  wgui_handle_inputbox_input();
                  wgui_set_RSK();
                  break;
               case PEN_EDITOR_EMS_INPUT_BOX:
                   
                  handle_category28_input();
                  wgui_set_EMS_inputbox_RSK();
                  break;
               default:
                  break;;
            }
         }
      }
      //mmi_pen_editor_reset_vk_text(&gPenEditorVKText);
      mmi_pen_editor_hide_virtual_keyboard_area();
      wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_TRAY);
	 h = get_changed_height(gPenEditorVKState, gPenEditorVKCandidate);
	 if(h>0)
	 {
		h +=1;
	 }
	mmi_pen_editor_resize_content_area(h);
       
      if (PEN_EDITOR_INLINE_TEXT==gPenEditorInputBoxType
         || PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX == gPenEditorInputBoxType)
      {
         s32 temp_multiline_height = MMI_multiline_inputbox.height;
         mmi_pen_editor_vk_list_menu_resize();
         if ( PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX == gPenEditorInputBoxType )
         {
            coolsand_UI_resize_multi_line_input_box(&MMI_multiline_inputbox,MMI_multiline_inputbox.width,temp_multiline_height);
            redraw_fixed_list();
            redraw_current_inline_item();
         }
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_TRAY);
      }
      coolsand_UI_move_single_line_input_box(&gPenEditorCandidateInputBox, MMI_multitap_x, MMI_multitap_y - 1);
      coolsand_UI_unlock_double_buffer();
      mmi_pen_editor_clear_and_show_virtual_keyboard_area();
   }
   #ifdef __MMI_HAND_WRITING__
   mmi_pen_editor_change_handwriting_area();
   #endif
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_vk_show
* DESCRIPTION
*   \brief This function is to handle show in virtual keyboard.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_vk_show(void)
{
	S32 h = 0;
   if (PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX==gPenEditorInputBoxType)
   {
      mmi_pen_editor_set_virtual_keyboard_by_input_mode(MMI_current_input_mode);
		 h = get_changed_height(gPenEditorVKState, gPenEditorVKCandidate);
	  	mmi_pen_editor_resize_content_area(h);
      RedrawCategoryControlledArea200Screen(NULL);
      return;
   }
   if (MMI_GUI_VIRTUAL_KEYBOARD_TRAY == MMI_virtual_keyboard.lang_type)
   {
	#ifdef __MMI_TR_CHINESE_VK__
	if (INPUT_MODE_TR_MULTITAP_BOPOMO==MMI_current_input_mode||INPUT_MODE_TR_BOPOMO==MMI_current_input_mode||INPUT_MODE_TR_STROKE==MMI_current_input_mode)
      {
         gPenEditorVKCandidate=MMI_TRUE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_BOPOMO);
      }else 
	#endif
	if (INPUT_MODE_SM_MULTITAP_PINYIN==MMI_current_input_mode||INPUT_MODE_SM_PINYIN==MMI_current_input_mode||INPUT_MODE_SM_STROKE==MMI_current_input_mode)
      {
         gPenEditorVKCandidate=MMI_TRUE;
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PINYIN);
      }else 	  
    
      if (INPUT_MODE_MULTITAP_LOWERCASE_ABC==MMI_current_input_mode||INPUT_MODE_SMART_LOWERCASE_ABC==MMI_current_input_mode)
      {
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH);
      }
      else if (INPUT_MODE_MULTITAP_UPPERCASE_ABC==MMI_current_input_mode||INPUT_MODE_SMART_UPPERCASE_ABC==MMI_current_input_mode)
      {
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ENGLISH_UPPERCASE);
      }
      else if (INPUT_MODE_123==MMI_current_input_mode)
      {
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER);
      }
#ifdef __MMI_VIETNAMESE_VK__
	else if((INPUT_MODE_MULTITAP_UPPERCASE_VIETNAMESE==MMI_current_input_mode) || (INPUT_MODE_SMART_UPPERCASE_VIETNAMESE==MMI_current_input_mode))
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_VIETNAMESE_UPPERCASE);
	}
	else if((INPUT_MODE_MULTITAP_LOWERCASE_VIETNAMESE==MMI_current_input_mode) || (INPUT_MODE_SMART_LOWERCASE_VIETNAMESE==MMI_current_input_mode) || (INPUT_MODE_MMI_LOWERCASE_VIETNAMESE==MMI_current_input_mode))
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_VIETNAMESE);		
	}
#endif
#ifdef __MMI_THAI_VK__
	else if(INPUT_MODE_SMART_THAI == MMI_current_input_mode || INPUT_MODE_MULTITAP_THAI == MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_THAI);
	}
#endif
#ifdef __MMI_INDONESIAN_VK__
	else if((INPUT_MODE_MULTITAP_UPPERCASE_INDONESIAN ==MMI_current_input_mode) || (INPUT_MODE_SMART_UPPERCASE_INDONESIAN==MMI_current_input_mode))
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_INDONESIAN_UPPERCASE);
	}
	else if((INPUT_MODE_MULTITAP_LOWERCASE_INDONESIAN==MMI_current_input_mode) || (INPUT_MODE_SMART_LOWERCASE_INDONESIAN==MMI_current_input_mode))
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_INDONESIAN);		
	}
#endif
#ifdef __MMI_MALAY_VK__
	else if(INPUT_MODE_MULTITAP_LOWERCASE_MALAY== MMI_current_input_mode ||INPUT_MODE_SMART_LOWERCASE_MALAY == MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_MALAY);
	}
	else if(INPUT_MODE_MULTITAP_UPPERCASE_MALAY== MMI_current_input_mode ||INPUT_MODE_SMART_UPPERCASE_MALAY == MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_MALAY_UPPERCASE);
	}
#endif
#ifdef __MMI_PERSIAN_VK__
	else if(INPUT_MODE_MULTITAP_PERSIAN== MMI_current_input_mode || INPUT_MODE_SMART_PERSIAN== MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PERSIAN);
	}
#endif
      #ifdef __MMI_ARABIC_VK__
      else if(INPUT_MODE_MULTITAP_ARABIC == MMI_current_input_mode || INPUT_MODE_SMART_ARABIC == MMI_current_input_mode)
      {
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ARABIC);
      }
      #endif
	 #ifdef __MMI_FRENCH_VK__
	 else if(INPUT_MODE_MULTITAP_LOWERCASE_FRENCH == MMI_current_input_mode|| INPUT_MODE_SMART_LOWERCASE_FRENCH == MMI_current_input_mode)
	 {
	 wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_FRENCH);
	 }
	 else if(INPUT_MODE_MULTITAP_UPPERCASE_FRENCH == MMI_current_input_mode || INPUT_MODE_SMART_UPPERCASE_FRENCH == MMI_current_input_mode)
	 {
	 wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_FRENCH_UPPERCASE);
	 }
	 #endif
#ifdef __MMI_GERMAN_VK__
	else if(INPUT_MODE_MULTITAP_LOWERCASE_GERMAN == MMI_current_input_mode || INPUT_MODE_SMART_LOWERCASE_GERMAN == MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_GERMAN);
	}
	else if(INPUT_MODE_MULTITAP_UPPERCASE_GERMAN == MMI_current_input_mode || INPUT_MODE_SMART_UPPERCASE_GERMAN == MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_GERMAN_UPPERCASE);
	}
#endif
#ifdef __MMI_TURKISH_VK__
	else if(INPUT_MODE_MULTITAP_LOWERCASE_TURKISH== MMI_current_input_mode || INPUT_MODE_SMART_LOWERCASE_TURKISH== MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_TURKISH);
	}
	else if(INPUT_MODE_MULTITAP_UPPERCASE_TURKISH== MMI_current_input_mode || INPUT_MODE_SMART_UPPERCASE_TURKISH== MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_TURKISH_UPPERCASE);
	}
#endif
#ifdef __MMI_RUSSIAN_VK__
	else if(INPUT_MODE_MULTITAP_LOWERCASE_RUSSIAN== MMI_current_input_mode || INPUT_MODE_SMART_LOWERCASE_RUSSIAN == MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_RUSSIAN);
	}
	else if(INPUT_MODE_MULTITAP_UPPERCASE_RUSSIAN == MMI_current_input_mode || INPUT_MODE_SMART_UPPERCASE_RUSSIAN == MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_RUSSIAN_UPPERCASE);
	}
#endif
#ifdef __MMI_SPANISH_VK__
	else if(INPUT_MODE_MULTITAP_LOWERCASE_SPANISH== MMI_current_input_mode || INPUT_MODE_SMART_LOWERCASE_SPANISH== MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_SPANISH);
	}
	else if(INPUT_MODE_MULTITAP_UPPERCASE_SPANISH== MMI_current_input_mode || INPUT_MODE_SMART_UPPERCASE_SPANISH== MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_SPANISH_UPPERCASE);
	}
#endif
#ifdef __MMI_HINDI_VK__
	else if(INPUT_MODE_MULTITAP_HINDI == MMI_current_input_mode || INPUT_MODE_SMART_HINDI == MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_HINDI);
	}
#endif
#ifdef __MMI_PORTUGUESE_VK__
	else if(INPUT_MODE_MULTITAP_LOWERCASE_PORTUGUESE == MMI_current_input_mode || INPUT_MODE_SMART_LOWERCASE_PORTUGUESE == MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PORTUGUESE);
	}
	else if(INPUT_MODE_MULTITAP_UPPERCASE_PORTUGUESE == MMI_current_input_mode || INPUT_MODE_SMART_UPPERCASE_PORTUGUESE== MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PORTUGUESE_UPPERCASE);
	}
#endif
#ifdef __MMI_ITALIAN_VK__
	else if(INPUT_MODE_MULTITAP_LOWERCASE_ITALIAN== MMI_current_input_mode || INPUT_MODE_SMART_LOWERCASE_ITALIAN== MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ITALIAN);
	}
	else if(INPUT_MODE_MULTITAP_UPPERCASE_ITALIAN== MMI_current_input_mode || INPUT_MODE_SMART_UPPERCASE_ITALIAN== MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ITALIAN_UPPERCASE);
	}
#endif
	else
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_NUMBER);
	}
	  coolsand_UI_lock_double_buffer();
	 h = get_changed_height(gPenEditorVKState, gPenEditorVKCandidate);
	 if(h<0)
	 {
		h-=1;
	 }
	mmi_pen_editor_resize_content_area(h);
       
      if (PEN_EDITOR_INLINE_TEXT==gPenEditorInputBoxType
         || PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX == gPenEditorInputBoxType)
      {
         s32 temp_multiline_height = MMI_multiline_inputbox.height;
         MMI_BOOL temp_VKCandidate = gPenEditorVKCandidate;
         mmi_gui_virtual_keyboard_language_enum keyboard_type = MMI_virtual_keyboard.lang_type;
         mmi_pen_editor_vk_list_menu_resize();
         if ( PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX == gPenEditorInputBoxType )
         {
            coolsand_UI_resize_multi_line_input_box(&MMI_multiline_inputbox,MMI_multiline_inputbox.width,temp_multiline_height);
            redraw_fixed_list();
            redraw_current_inline_item();
            gPenEditorVKCandidate = temp_VKCandidate;
         }
         wgui_setup_virtual_keyboard(keyboard_type);
      }
      coolsand_UI_move_single_line_input_box(&gPenEditorCandidateInputBox, gPenEditorCandidateInputBox.x, MMI_virtual_keyboard.y-gPenEditorCandidateInputBox.height-2);
      if (gPenEditorVKCandidate)
      {
         mmi_pen_editor_hide_candidate_area();
      }
      coolsand_UI_unlock_double_buffer();
      mmi_pen_editor_clear_and_show_virtual_keyboard_area();
   }
   #ifdef __MMI_HAND_WRITING__
   mmi_pen_editor_change_handwriting_area();
   #endif
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_vk_symbol_picker
* DESCRIPTION
*   \brief This function is to handle symbol picker in virtual keyboard.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_vk_symbol_picker(void)
{
	S32 h;
   if (PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX==gPenEditorInputBoxType)
   {
      coolsand_UI_lock_double_buffer();
      mmi_pen_editor_hide_virtual_keyboard_area();
      if (MMI_GUI_VIRTUAL_KEYBOARD_SYMBOL==MMI_virtual_keyboard.lang_type
         ||MMI_GUI_VIRTUAL_KEYBOARD_CHINESE_SYMBOL==MMI_virtual_keyboard.lang_type
      #ifdef __MMI_ARABIC_VK__
	  ||MMI_GUI_VIRTUAL_KEYBOARD_ARABIC_SYMBOLS == MMI_virtual_keyboard.lang_type
      #endif
      #ifdef __MMI_THAI_VK__
	  || MMI_GUI_VIRTUAL_KEYBOARD_THAI_SYMBOL == MMI_virtual_keyboard.lang_type
      #endif
	  #ifdef __MMI_PERSIAN_VK__
	  ||MMI_GUI_VIRTUAL_KEYBOARD_PERSIAN_SYMBOL== MMI_virtual_keyboard.lang_type
      #endif
	  #ifdef __MMI_HINDI_VK__
	  ||MMI_GUI_VIRTUAL_KEYBOARD_HINDI_SYMBOLS== MMI_virtual_keyboard.lang_type
      #endif
		 )
      {
         wgui_setup_virtual_keyboard(gPenEditorPreviousVirtualKeyboardType);
      }
      else
      {
          
          
         gPenEditorPreviousVirtualKeyboardType = MMI_virtual_keyboard.lang_type;
         if (INPUT_MODE_MMI_MULTITAP_BPMF==MMI_current_input_mode
          ||INPUT_MODE_MMI_MULTITAP_PINYIN==MMI_current_input_mode)
         {
            wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_CHINESE_SYMBOL);
         }
         #ifdef __MMI_ARABIC_VK__
         else if(INPUT_MODE_MMI_MULTITAP_ARABIC == MMI_current_input_mode)
         {
         	wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ARABIC_SYMBOLS);
         }
         #endif
         #ifdef __MMI_THAI_VK__
         else if(INPUT_MODE_MMI_MULTITAP_THAI == MMI_current_input_mode)
         {
         	wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_THAI_SYMBOL);
         }
         #endif
         #ifdef __MMI_PERSIAN_VK__
         else if(INPUT_MODE_MMI_MULTITAP_PERSIAN== MMI_current_input_mode)
         {
         	wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PERSIAN_SYMBOL);
         }
         #endif
	  #ifdef __MMI_HINDI_VK__
         else if(INPUT_MODE_MULTITAP_HINDI== MMI_current_input_mode)
         {
         	wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_HINDI_SYMBOLS);
         }
         #endif
         else
         {
            wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_SYMBOL);
         }
      }
	 h = get_changed_height(gPenEditorVKState, gPenEditorVKCandidate);
	mmi_pen_editor_resize_content_area(h);
      coolsand_UI_unlock_double_buffer();
      RedrawCategoryControlledArea200Screen(NULL);
      return;
   }
	 coolsand_UI_lock_double_buffer();
   if (MMI_GUI_VIRTUAL_KEYBOARD_SYMBOL==MMI_virtual_keyboard.lang_type||MMI_GUI_VIRTUAL_KEYBOARD_CHINESE_SYMBOL==MMI_virtual_keyboard.lang_type
   	#ifdef __MMI_ARABIC_VK__
   	||MMI_GUI_VIRTUAL_KEYBOARD_ARABIC_SYMBOLS == MMI_virtual_keyboard.lang_type
   	#endif
	#ifdef __MMI_THAI_VK__
	||MMI_GUI_VIRTUAL_KEYBOARD_THAI_SYMBOL == MMI_virtual_keyboard.lang_type
	#endif
	#ifdef __MMI_PERSIAN_VK__
	||MMI_GUI_VIRTUAL_KEYBOARD_PERSIAN_SYMBOL == MMI_virtual_keyboard.lang_type
	#endif
	#ifdef __MMI_HINDI_VK__
	||MMI_GUI_VIRTUAL_KEYBOARD_HINDI_SYMBOLS== MMI_virtual_keyboard.lang_type
	#endif
   	)
   {
      mmi_pen_editor_hide_virtual_keyboard_area();
      wgui_setup_virtual_keyboard(gPenEditorPreviousVirtualKeyboardType);
      if (MMI_GUI_VIRTUAL_KEYBOARD_PINYIN == MMI_virtual_keyboard.lang_type
	 #ifdef __MMI_TR_CHINESE_VK__
	||MMI_GUI_VIRTUAL_KEYBOARD_BOPOMO == MMI_virtual_keyboard.lang_type
	#endif
        )
      {
         gPenEditorVKCandidate = MMI_TRUE;
         mmi_pen_editor_hide_candidate_area();
      }
   }
   else
   {
      gPenEditorPreviousVirtualKeyboardType = MMI_virtual_keyboard.lang_type;
      if (MMI_GUI_VIRTUAL_KEYBOARD_PINYIN == MMI_virtual_keyboard.lang_type
		 #ifdef __MMI_TR_CHINESE_VK__
		||MMI_GUI_VIRTUAL_KEYBOARD_BOPOMO == MMI_virtual_keyboard.lang_type
		#endif
        )
      {
          
          
          
         if (gPenEditorVKText.TextLength||gPenEditorSelectCandidate)
         {
            //gPenEditorSelectCandidate = MMI_FALSE;
            mmi_pen_editor_set_candidate_state(MMI_FALSE);
            /*Temp Solution for LSK Text Update*/
            mmi_pen_editor_set_left_soft_key();
            switch (gPenEditorInputBoxType)
            {
               case PEN_EDITOR_EMS_INPUT_BOX:
              	#ifdef __MMI_GB__
			gb_register_arrow_handler();
			#else
			register_EMS_inputbox_keys();
			#endif
			break;
               case PEN_EDITOR_MULTILINE_INPUT_BOX:
               case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
               case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
                   	#ifdef __MMI_GB__
			gb_register_arrow_handler();
			#else
              	 register_multiline_inputbox_keys();
			#endif
                  if (PEN_EDITOR_DICT_MULTILINE_INPUT_BOX == gPenEditorInputBoxType)
                  {
                    cat201_register_key_func();
                  }
                  break;
               default :
                  break;
            }
            mmi_pen_editor_reset_vk_text(&gPenEditorVKText);
            wgui_set_virtual_keyboard_display_area((U8*)gPenEditorVKText.VKText, MMI_FALSE);
            switch (gPenEditorInputBoxType)
            {
               case PEN_EDITOR_MULTILINE_INPUT_BOX:
               case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
               case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
                   
                  wgui_handle_inputbox_input();
                  wgui_set_RSK();
                  break;
               case PEN_EDITOR_EMS_INPUT_BOX:
                   
                  handle_category28_input();
                  wgui_set_EMS_inputbox_RSK();
                  break;
               default:
                  break;;
            }
         }
         mmi_pen_editor_hide_virtual_keyboard_area();
         gPenEditorVKCandidate = MMI_FALSE;
         //gPenEditorSelectCandidate = MMI_FALSE;
         mmi_pen_editor_set_candidate_state(MMI_FALSE);
      }
       
      if (INPUT_MODE_TR_MULTITAP_BOPOMO==MMI_current_input_mode
            ||INPUT_MODE_TR_BOPOMO==MMI_current_input_mode
            ||INPUT_MODE_TR_STROKE==MMI_current_input_mode
            ||INPUT_MODE_SM_MULTITAP_PINYIN==MMI_current_input_mode
            ||INPUT_MODE_SM_PINYIN==MMI_current_input_mode
            ||INPUT_MODE_SM_STROKE==MMI_current_input_mode)
      {
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_CHINESE_SYMBOL);
      }
	#ifdef __MMI_ARABIC_VK__
	if(INPUT_MODE_MULTITAP_ARABIC == MMI_current_input_mode
		||INPUT_MODE_SMART_ARABIC == MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ARABIC_SYMBOLS);
	}
	#endif
	#ifdef __MMI_THAI_VK__
	if(INPUT_MODE_MULTITAP_THAI== MMI_current_input_mode
		||INPUT_MODE_SMART_THAI== MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_THAI_SYMBOL);
	}
	#endif
	#ifdef __MMI_PERSIAN_VK__
	if(INPUT_MODE_MULTITAP_PERSIAN == MMI_current_input_mode
		||INPUT_MODE_SMART_PERSIAN == MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PERSIAN_SYMBOL);
	}
	#endif
	#ifdef __MMI_HINDI_VK__
	if(INPUT_MODE_MULTITAP_HINDI== MMI_current_input_mode
		||INPUT_MODE_SMART_HINDI== MMI_current_input_mode)
	{
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_HINDI_SYMBOLS);
	}
	#endif
      else
      {
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_SYMBOL);
      }
   }
	
	 h = get_changed_height(gPenEditorVKState, gPenEditorVKCandidate);
	if((h>0)&&(MMI_virtual_keyboard.lang_type==GUI_VIRTUAL_KEYBOARD_TRAY||MMI_virtual_keyboard.lang_type==GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY))
	{
		h += 1;
	}
	else
	{
		h -= 1;
	}
    mmi_pen_editor_resize_content_area(h);
    
   if (PEN_EDITOR_INLINE_TEXT == gPenEditorInputBoxType
         || PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX == gPenEditorInputBoxType)
   {
      BOOL symbol_keyboard;
      s32 temp_multiline_height = MMI_multiline_inputbox.height;
      mmi_gui_virtual_keyboard_language_enum tempVirtualKeyboardType = MMI_virtual_keyboard.lang_type;
      MMI_BOOL temp_VKCandidate = gPenEditorVKCandidate;
      if (MMI_GUI_VIRTUAL_KEYBOARD_SYMBOL == MMI_virtual_keyboard.lang_type
         || MMI_GUI_VIRTUAL_KEYBOARD_CHINESE_SYMBOL == MMI_virtual_keyboard.lang_type
      #ifdef __MMI_ARABIC_VK__
         || MMI_GUI_VIRTUAL_KEYBOARD_ARABIC_SYMBOLS == MMI_virtual_keyboard.lang_type
      #endif
	   #ifdef __MMI_THAI_VK__
         || MMI_GUI_VIRTUAL_KEYBOARD_THAI_SYMBOL == MMI_virtual_keyboard.lang_type
      #endif
	  #ifdef __MMI_PERSIAN_VK__
         || MMI_GUI_VIRTUAL_KEYBOARD_PERSIAN_SYMBOL == MMI_virtual_keyboard.lang_type
      #endif
	  #ifdef __MMI_HINDI_VK__
         || MMI_GUI_VIRTUAL_KEYBOARD_HINDI_SYMBOLS== MMI_virtual_keyboard.lang_type
      #endif
		 ) 
      {
         symbol_keyboard = MMI_TRUE;
      }
      else
      {
         symbol_keyboard = MMI_FALSE;
      }
      mmi_pen_editor_vk_list_menu_resize();
      if ( PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX == gPenEditorInputBoxType )
      {
         coolsand_UI_resize_multi_line_input_box(&MMI_multiline_inputbox,MMI_multiline_inputbox.width,temp_multiline_height);
         redraw_fixed_list();
         redraw_current_inline_item();
         gPenEditorVKCandidate = temp_VKCandidate;
      }
      if (symbol_keyboard)
      {
         if (INPUT_MODE_TR_MULTITAP_BOPOMO==MMI_current_input_mode
               ||INPUT_MODE_TR_BOPOMO==MMI_current_input_mode
               ||INPUT_MODE_TR_STROKE==MMI_current_input_mode
               ||INPUT_MODE_SM_MULTITAP_PINYIN==MMI_current_input_mode
               ||INPUT_MODE_SM_PINYIN==MMI_current_input_mode
               ||INPUT_MODE_SM_STROKE==MMI_current_input_mode)
         {
            wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_CHINESE_SYMBOL);
         }
         #ifdef __MMI_ARABIC_VK__
         else if(INPUT_MODE_MULTITAP_ARABIC == MMI_current_input_mode || INPUT_MODE_SMART_ARABIC == MMI_current_input_mode)
         {
         	wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_ARABIC_SYMBOLS);
         }
         #endif
         #ifdef __MMI_THAI_VK__
         else if(INPUT_MODE_MULTITAP_THAI== MMI_current_input_mode || INPUT_MODE_SMART_THAI== MMI_current_input_mode)
         {
         	wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_THAI_SYMBOL);
         }
         #endif
	#ifdef __MMI_PERSIAN_VK__
         else if(INPUT_MODE_MULTITAP_PERSIAN== MMI_current_input_mode || INPUT_MODE_SMART_PERSIAN== MMI_current_input_mode)
         {
         	wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_PERSIAN_SYMBOL);
         }
         #endif
	#ifdef __MMI_HINDI_VK__
         else if(INPUT_MODE_MULTITAP_HINDI== MMI_current_input_mode || INPUT_MODE_SMART_HINDI== MMI_current_input_mode)
         {
         	wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_HINDI_SYMBOLS);
         }
         #endif
         else
         {
            wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_SYMBOL);
         }
      }
      else
      {
         wgui_setup_virtual_keyboard(tempVirtualKeyboardType);
      }
   }
   if (gPenEditorVKCandidate)
   {
      coolsand_UI_move_single_line_input_box(&gPenEditorCandidateInputBox, gPenEditorCandidateInputBox.x, MMI_virtual_keyboard.y-gPenEditorCandidateInputBox.height-2);
      mmi_pen_editor_hide_candidate_area();
   }
   /*Wangzl Add Start For 5972 Ver: TBM780  on 2007-8-14 16:23 */
   if(MMI_GUI_VIRTUAL_KEYBOARD_TRAY==MMI_virtual_keyboard.lang_type)
   {
   	coolsand_UI_move_single_line_input_box(&gPenEditorCandidateInputBox, MMI_multitap_x, MMI_multitap_y - 1);
       mmi_pen_editor_hide_candidate_area();
   }
   /*Wangzl Add End For 5972 Ver: TBM780  on 2007-8-14 16:23 */
   coolsand_UI_unlock_double_buffer();
   mmi_pen_editor_clear_and_show_virtual_keyboard_area();
   #ifdef __MMI_HAND_WRITING__
   mmi_pen_editor_change_handwriting_area();
   #endif
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_vk_euro_symbol_picker
* DESCRIPTION
*   \brief This function is to handle symbol picker in virtual keyboard.
*
* PARAMETERS
*  None
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
#ifdef __MMI_VKBD_EUROPEAN_SUPPORT__
void mmi_pen_editor_vk_euro_symbol_picker(void)
{
	S32 h;
	if (PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX==gPenEditorInputBoxType)
	{
		coolsand_UI_lock_double_buffer();
		mmi_pen_editor_hide_virtual_keyboard_area();
		if (MMI_GUI_VIRTUAL_KEYBOARD_EUROPEAN_SYMBOLS==MMI_virtual_keyboard.lang_type)
		{
			wgui_setup_virtual_keyboard(gPenEditorPreviousVirtualKeyboardType);
		}
		else
		{
			gPenEditorPreviousVirtualKeyboardType = MMI_virtual_keyboard.lang_type;
			wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_EUROPEAN_SYMBOLS);
		}
		h = get_changed_height(gPenEditorVKState, gPenEditorVKCandidate);
		mmi_pen_editor_resize_content_area(h);
		coolsand_UI_unlock_double_buffer();
		RedrawCategoryControlledArea200Screen(NULL);
		return;
	}
	coolsand_UI_lock_double_buffer();
	if (MMI_GUI_VIRTUAL_KEYBOARD_EUROPEAN_SYMBOLS == MMI_virtual_keyboard.lang_type)
	{
		mmi_pen_editor_hide_virtual_keyboard_area();
		wgui_setup_virtual_keyboard(gPenEditorPreviousVirtualKeyboardType);
	}
	else
	{
		gPenEditorPreviousVirtualKeyboardType = MMI_virtual_keyboard.lang_type;
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_EUROPEAN_SYMBOLS);
	}

	h = get_changed_height(gPenEditorVKState, gPenEditorVKCandidate);
	if((h>0)&&(MMI_virtual_keyboard.lang_type==GUI_VIRTUAL_KEYBOARD_TRAY||MMI_virtual_keyboard.lang_type==GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY))
	{
		h += 1;
	}
	else
	{
		h -= 1;
	}
	mmi_pen_editor_resize_content_area(h);
	if (gPenEditorVKCandidate)
	{
		coolsand_UI_move_single_line_input_box(&gPenEditorCandidateInputBox, gPenEditorCandidateInputBox.x, MMI_virtual_keyboard.y-gPenEditorCandidateInputBox.height-2);
		mmi_pen_editor_hide_candidate_area();
	}
	if(MMI_GUI_VIRTUAL_KEYBOARD_TRAY==MMI_virtual_keyboard.lang_type)
	{
		coolsand_UI_move_single_line_input_box(&gPenEditorCandidateInputBox, MMI_multitap_x, MMI_multitap_y - 1);
		mmi_pen_editor_hide_candidate_area();
	}
	coolsand_UI_unlock_double_buffer();
	mmi_pen_editor_clear_and_show_virtual_keyboard_area();
	#ifdef __MMI_HAND_WRITING__
	mmi_pen_editor_change_handwriting_area();
	#endif
}
#endif
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_set_inputbox_LSK_Function
* DESCRIPTION
*   \brief This function is to store LSK funmction from Application.
*
* PARAMETERS
*  \param FuncPtr f : the function pointer from Application
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_set_inputbox_LSK_Function(FuncPtr f)
{
   gPenEditorLSKfunction = f;
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_store_LSK_Text
* DESCRIPTION
*   \brief This function is to store LSK text.
*
* PARAMETERS
*  \param 
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_store_LSK_Text(void)
{
   gPenEditorLSKText= MMI_softkeys[MMI_LEFT_SOFTKEY].text;
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_history_vk_lang_type
* DESCRIPTION
*   \brief This function is to store LSK text.
*
* PARAMETERS
*  \param 
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
mmi_gui_virtual_keyboard_language_enum mmi_pen_editor_history_vk_lang_type(void)
{
   return gPenEditorHistoryVirtualKeyboardType;
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_check_valid_input
* DESCRIPTION
*   \brief This function is to store LSK text.
*
* PARAMETERS
*  \param 
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
MMI_BOOL mmi_pen_editor_check_valid_input(UI_character_type input,const UI_character_type* valid_input)
{
   int i;
   
   for (i = 0;;i++)
   {
      if (0 == valid_input[i])
      {
         return MMI_FALSE;
      }
      else if (input == valid_input[i])
      {
         return MMI_TRUE;
      }
   }
   return MMI_FALSE;
}

 
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_resort_candidates
* DESCRIPTION
*   \brief This function is to resort the candidates.
*
* PARAMETERS
*  \param candidates : the candidates list for resorting
*  \param num : the number of candidates in the list
*  \param SortRequirement : how to resort the candidates
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_resort_candidates(UI_character_type* candidates,int num, mmi_pen_editor_resort_type_enum SortRequirement)
{
   int i;
   
   for (i = 0;i < num - 1;i++)
   {
      if (PEN_EDITOR_SORT_NON_CAPITAL_FIRST == SortRequirement)
      {
         if ((candidates[i+1] - 0x20) == candidates[i])
         {
            candidates[i] = candidates[i+1];
            candidates[i+1] = candidates[i] - 0x20;
         }
      }
      else if (PEN_EDITOR_SORT_CAPITAL_FIRST == SortRequirement)
      {
         if ((candidates[i+1] + 0x20) == candidates[i])
         {
            candidates[i] = candidates[i+1];
            candidates[i+1] = candidates[i] + 0x20;
         }
      }
   }
   
   //For '0' special case
   if ('0' == candidates[0])
   {
      if (PEN_EDITOR_SORT_NON_CAPITAL_FIRST == SortRequirement)
      {
         if ('o'==candidates[1] && 'O'==candidates[2] )
         {
           candidates[0] = 'o'; 
           candidates[1] = 'O'; 
           candidates[2] = '0'; 
         }
      }
      else if (PEN_EDITOR_SORT_CAPITAL_FIRST == SortRequirement)
      {
         if ('O'==candidates[1] && 'o'==candidates[2] )
         {
           candidates[0] = 'O'; 
           candidates[1] = 'o'; 
           candidates[2] = '0'; 
         }
      }
   }
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_clear_key_handler
* DESCRIPTION
*   \brief This function is to store LSK text.
*
* PARAMETERS
*  \param 
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_clear_key_handler(void)
{
#if !defined(__MMI_HANDWRITING_PAD__)
   clear_MMI_key_input_handler();

    
	ClearKeyHandler(KEY_0,KEY_LONG_PRESS);
	ClearKeyHandler(KEY_1,KEY_LONG_PRESS);
	ClearKeyHandler(KEY_2,KEY_LONG_PRESS);
	ClearKeyHandler(KEY_3,KEY_LONG_PRESS);
	ClearKeyHandler(KEY_4,KEY_LONG_PRESS);
	ClearKeyHandler(KEY_5,KEY_LONG_PRESS);
	ClearKeyHandler(KEY_6,KEY_LONG_PRESS);
	ClearKeyHandler(KEY_7,KEY_LONG_PRESS);
	ClearKeyHandler(KEY_8,KEY_LONG_PRESS);
	ClearKeyHandler(KEY_9,KEY_LONG_PRESS);

	ClearKeyHandler(KEY_0,KEY_REPEAT);
	ClearKeyHandler(KEY_1,KEY_REPEAT);
	ClearKeyHandler(KEY_2,KEY_REPEAT);
	ClearKeyHandler(KEY_3,KEY_REPEAT);
	ClearKeyHandler(KEY_4,KEY_REPEAT);
	ClearKeyHandler(KEY_5,KEY_REPEAT);
	ClearKeyHandler(KEY_6,KEY_REPEAT);
	ClearKeyHandler(KEY_7,KEY_REPEAT);
	ClearKeyHandler(KEY_8,KEY_REPEAT);
	ClearKeyHandler(KEY_9,KEY_REPEAT);

   ClearKeyHandler(KEY_POUND,KEY_EVENT_DOWN);
   ClearKeyHandler(KEY_STAR,KEY_EVENT_DOWN);
   ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
   ClearKeyHandler(KEY_STAR,KEY_EVENT_UP);
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_vk_list_menu_resize
* DESCRIPTION
*   \brief This function is to resize the list menu for virtual keyboard.
*
* PARAMETERS
*  \param 
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_vk_list_menu_resize(void)
{
    
   if ((current_wgui_inline_item->flags&INLINE_ITEM_TYPE_MASK)!=INLINE_ITEM_TYPE_MULTI_LINE_EDIT)
   {
      resize_fixed_list(MMI_content_width,gPenEditorListMenuHeight-MMI_virtual_keyboard.height-1);
   }
   else
   {
      if (gPenEditorVKCandidate)
      {
         resize_fixed_list(MMI_content_width,gPenEditorListMenuHeight-MMI_virtual_keyboard.height-gPenEditorCandidateInputBox.height-1);
         mmi_pen_editor_hide_candidate_area();
      }
      else
      {
         resize_fixed_list(MMI_content_width,gPenEditorListMenuHeight-MMI_virtual_keyboard.height-1);
      }
   }
    
   if (((current_wgui_inline_item->flags&INLINE_ITEM_TYPE_MASK)==INLINE_ITEM_TYPE_TEXT_EDIT)||((current_wgui_inline_item->flags&INLINE_ITEM_TYPE_MASK)==INLINE_ITEM_TYPE_MULTI_LINE_EDIT))
   {
      memset(&gPenEditorInlineEditHistory,0,sizeof(gPenEditorInlineEditHistory));
      wgui_inline_edit_get_current_item_history((U8*)&gPenEditorInlineEditHistory);
   }
   complete_inline_item_edit();
   wgui_inline_edit_default_list_highlight_handler(MMI_fixed_list_menu.highlighted_item);
   wgui_inline_item_handled=0;
   coolsand_UI_fixed_list_menu_locate_highlighted_item(&MMI_fixed_list_menu);
   if ((current_wgui_inline_item->flags&INLINE_ITEM_TYPE_MASK)==INLINE_ITEM_TYPE_MULTI_LINE_EDIT)
   {
      MMI_fixed_list_menu.first_displayed_item = MMI_fixed_list_menu.highlighted_item;
   }
   if (((current_wgui_inline_item->flags&INLINE_ITEM_TYPE_MASK)==INLINE_ITEM_TYPE_TEXT_EDIT)||((current_wgui_inline_item->flags&INLINE_ITEM_TYPE_MASK)==INLINE_ITEM_TYPE_MULTI_LINE_EDIT))
   {
      memset(&gPenEditorInlineEditHistory,0,sizeof(gPenEditorInlineEditHistory));
      wgui_inline_edit_get_current_item_history((U8*)&gPenEditorInlineEditHistory);
   }
   redraw_fixed_list();
}
void mmi_pen_editor_list_menu_highlight_resize(void)
{
   if (gPenEditorListMenuHeight)
   {
      if ((current_wgui_inline_item->flags&INLINE_ITEM_TYPE_MASK)!=INLINE_ITEM_TYPE_MULTI_LINE_EDIT)
      {
         resize_fixed_list(MMI_content_width,gPenEditorListMenuHeight-MMI_virtual_keyboard.height-1);
      }
      else
      {
         if (gPenEditorVKCandidate)
         {
            resize_fixed_list(MMI_content_width,gPenEditorListMenuHeight-MMI_virtual_keyboard.height-gPenEditorCandidateInputBox.height-1);
            mmi_pen_editor_hide_candidate_area();
         }
         else
         {
            resize_fixed_list(MMI_content_width,gPenEditorListMenuHeight-MMI_virtual_keyboard.height-1);
         }
      }
   }
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_store_list_menu_height
* DESCRIPTION
*   \brief This function is to store list menu height.
*
* PARAMETERS
*  \param 
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_store_list_menu_height(s32 list_menu_height)
{
   gPenEditorListMenuHeight = list_menu_height;
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_get_list_menu_height
* DESCRIPTION
*   \brief This function is to return list menu height at the beginning.
*
* PARAMETERS
*  \param 
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
s32 mmi_pen_editor_get_list_menu_height(void)
{
   return gPenEditorListMenuHeight;
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_resize_multiline_input_box_for_vk
* DESCRIPTION
*   \brief This function is to return list menu height at the beginning.
*
* PARAMETERS
*  \param 
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_resize_multiline_input_box_for_vk(void)
{
	S32 h;
   if (MMI_GUI_VIRTUAL_KEYBOARD_TRAY != MMI_virtual_keyboard.lang_type)
   {
       
      if (((MMI_GUI_VIRTUAL_KEYBOARD_SYMBOL == MMI_virtual_keyboard.lang_type)||
          (MMI_GUI_VIRTUAL_KEYBOARD_CHINESE_SYMBOL == MMI_virtual_keyboard.lang_type)) &&
          (!g_pen_editor_vk_show_at_start))
      {
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_TRAY);
         mmi_pen_editor_vk_symbol_picker();
      }
      else if (MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_DOT_TRAY == MMI_virtual_keyboard.lang_type
      	        ||MMI_GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY == MMI_virtual_keyboard.lang_type
      	        ||MMI_GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER == MMI_virtual_keyboard.lang_type)
      {
		mmi_gui_virtual_keyboard_language_enum temp_vk_lang_type = MMI_virtual_keyboard.lang_type;
		wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_TRAY);
		wgui_setup_virtual_keyboard(temp_vk_lang_type);
		h = get_changed_height(gPenEditorVKState, gPenEditorVKCandidate)-1;
		mmi_pen_editor_resize_content_area(h);
      }
      else
      {
         wgui_setup_virtual_keyboard(MMI_GUI_VIRTUAL_KEYBOARD_TRAY);
         mmi_pen_editor_vk_show();
      }
   }
   mmi_pen_editor_clear_and_show_virtual_keyboard_area();
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_reset_candidate_index
* DESCRIPTION
*   \brief This function is to reset candidate index value.
*
* PARAMETERS
*  \param 
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_reset_candidate_index(void)
{
    
   gPenEditorCandidateIndex = 0;
   gPenEditorCandidatePage = 0;
}
/*****************************************************************************
* FUNCTION
*  mmi_pen_editor_set_CSK_content
* DESCRIPTION
*   \brief This function is to set CSK display.
*
* PARAMETERS
*  \param 
*  
* RETURNS
*  None
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_pen_editor_set_CSK_content(void)
{
#if 0 //def __MMI_WGUI_CSK_ENABLE__
    
   UI_string_type inputmode_string = NULL;
   UI_image_type inputbox_information_icon = NULL;
   U16 name[]={0x0}; 

   set_softkey_label(NULL, MMI_CENTER_SOFTKEY);
   set_softkey_icon(NULL, MMI_CENTER_SOFTKEY);

   switch(MMI_current_input_mode)
   {	
      case INPUT_MODE_MULTITAP_UPPERCASE_ABC:	
      case INPUT_MODE_MULTITAP_UPPERCASE_ABC_NO_NUMERIC:
      case INPUT_MODE_SMART_UPPERCASE_ABC:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_ABC_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_LOWERCASE_ABC:	
      case INPUT_MODE_MMI_LOWERCASE_ABC:	
      case INPUT_MODE_MULTITAP_LOWERCASE_ABC_NO_NUMERIC:
      case INPUT_MODE_SMART_LOWERCASE_ABC:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_abc_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_THAI:
      case INPUT_MODE_MMI_MULTITAP_THAI:
      case INPUT_MODE_SMART_THAI:
         //inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_thai_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_UPPERCASE_SPANISH:
      case INPUT_MODE_SMART_UPPERCASE_SPANISH:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_SPANISH_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_LOWERCASE_SPANISH:	
      case INPUT_MODE_MMI_LOWERCASE_SPANISH:
      case INPUT_MODE_SMART_LOWERCASE_SPANISH:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_spanish_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_UPPERCASE_DANISH:
      case INPUT_MODE_SMART_UPPERCASE_DANISH:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_DANISH_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_LOWERCASE_DANISH:	
      case INPUT_MODE_MMI_LOWERCASE_DANISH:
      case INPUT_MODE_SMART_LOWERCASE_DANISH:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_danish_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_UPPERCASE_FRENCH:
      case INPUT_MODE_SMART_UPPERCASE_FRENCH:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_FRENCH_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_LOWERCASE_FRENCH:	
      case INPUT_MODE_MMI_LOWERCASE_FRENCH:
      case INPUT_MODE_SMART_LOWERCASE_FRENCH:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_french_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_UPPERCASE_GERMAN:
      case INPUT_MODE_SMART_UPPERCASE_GERMAN:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_GERMAN_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_LOWERCASE_GERMAN:	
      case INPUT_MODE_MMI_LOWERCASE_GERMAN:
      case INPUT_MODE_SMART_LOWERCASE_GERMAN:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_german_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_UPPERCASE_ITALIAN:
      case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_ITALIAN_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_LOWERCASE_ITALIAN:	
      case INPUT_MODE_MMI_LOWERCASE_ITALIAN:
      case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_italian_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_UPPERCASE_RUSSIAN:
      case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_RUSSIAN_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_LOWERCASE_RUSSIAN:	
      case INPUT_MODE_MMI_LOWERCASE_RUSSIAN:
      case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_russian_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_UPPERCASE_TURKISH:
      case INPUT_MODE_SMART_UPPERCASE_TURKISH:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_TURKISH_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_LOWERCASE_TURKISH:	
      case INPUT_MODE_MMI_LOWERCASE_TURKISH:
      case INPUT_MODE_SMART_LOWERCASE_TURKISH:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_turkish_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_UPPERCASE_PORTUGUESE:
      case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_PORTUGUESE_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_LOWERCASE_PORTUGUESE:	
      case INPUT_MODE_MMI_LOWERCASE_PORTUGUESE:
      case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_portuguese_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_HINDI:
         inputmode_string=(UI_string_type)name;
         break;
      case INPUT_MODE_MULTITAP_UPPERCASE_INDONESIAN:
      case INPUT_MODE_SMART_UPPERCASE_INDONESIAN:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_INDONESIAN_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_LOWERCASE_INDONESIAN:	
      case INPUT_MODE_MMI_LOWERCASE_INDONESIAN:
      case INPUT_MODE_SMART_LOWERCASE_INDONESIAN:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_indonesian_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_UPPERCASE_MALAY:
      case INPUT_MODE_SMART_UPPERCASE_MALAY:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_MALAY_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_LOWERCASE_MALAY:	
      case INPUT_MODE_MMI_LOWERCASE_MALAY:
      case INPUT_MODE_SMART_LOWERCASE_MALAY:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_malay_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_UPPERCASE_VIETNAMESE:
      case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_VIETNAMESE_STRING_ID);
         break;
      case INPUT_MODE_MULTITAP_LOWERCASE_VIETNAMESE:	
      case INPUT_MODE_MMI_LOWERCASE_VIETNAMESE:
      case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_vietnamese_STRING_ID);
         break;
      case INPUT_MODE_123: 
      case INPUT_MODE_MMI_NUMERIC:	
      case INPUT_MODE_123_SYMBOLS: 
         inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_123_STRING_ID);
         break;
      case INPUT_MODE_SMART_ARABIC:
         //inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_ARABIC);
         break;
      case INPUT_MODE_MMI_MULTITAP_BPMF:
#if defined(__MMI_T9__)
         inputbox_information_icon = get_image(WGUI_IME_MULTITAP_BPMF_IMG);
#elif defined(__MMI_ZI__)
         inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_BPMF_LOGO);
#else
         inputbox_information_icon = get_image(WGUI_IME_MULTITAP_BPMF_IMG);
#endif
         break;
      case INPUT_MODE_MMI_MULTITAP_PINYIN:       
#if defined(__MMI_T9__)
         inputbox_information_icon = get_image(WGUI_IME_MULTITAP_PINYIN_IMG);
#elif defined(__MMI_ZI__)
         inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_PINYIN_LOGO);
#else
         inputbox_information_icon = get_image(WGUI_IME_MULTITAP_BPMF_IMG);
#endif
         break;
      case INPUT_MODE_MMI_SM_STROKE:
#if defined(__MMI_T9__)
         inputbox_information_icon = get_image(WGUI_IME_SM_STROKE_IMG);
#elif defined(__MMI_ZI__)
         inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_SM_STROKE_LOGO);
#else
         inputbox_information_icon = get_image(WGUI_IME_MULTITAP_BPMF_IMG);
#endif
         break;
      case INPUT_MODE_MMI_TR_STROKE:
#if defined(__MMI_T9__)
         inputbox_information_icon = get_image(WGUI_IME_TR_STROKE_IMG);
#elif defined(__MMI_ZI__)
         inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_TR_STROKE_LOGO);
#else
         inputbox_information_icon = get_image(WGUI_IME_MULTITAP_BPMF_IMG);
#endif
         break;
      case INPUT_MODE_TR_MULTITAP_BOPOMO:
#if defined(__MMI_T9__)
         inputbox_information_icon = get_image(WGUI_IME_MULTITAP_BPMF_IMG);
#elif defined(__MMI_ZI__)
         inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_BPMF_LOGO);
#else
         inputbox_information_icon = get_image(WGUI_IME_MULTITAP_BPMF_IMG);
#endif
         break;
      case INPUT_MODE_SM_MULTITAP_PINYIN:
#if defined(__MMI_T9__)
         inputbox_information_icon = get_image(WGUI_IME_MULTITAP_PINYIN_IMG);
#elif defined(__MMI_ZI__)
         inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_PINYIN_LOGO);
#else
         inputbox_information_icon = get_image(WGUI_IME_MULTITAP_BPMF_IMG);
#endif
         break;
      case INPUT_MODE_TR_STROKE:
#if defined(__MMI_T9__)
         inputbox_information_icon = get_image(WGUI_IME_TR_STROKE_IMG);
#elif defined(__MMI_ZI__)
         inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_TR_STROKE_LOGO);
#else
         inputbox_information_icon = get_image(WGUI_IME_MULTITAP_BPMF_IMG);
#endif
         break;
      case INPUT_MODE_SM_STROKE:
#if defined(__MMI_T9__)
         inputbox_information_icon = get_image(WGUI_IME_SM_STROKE_IMG);
#elif defined(__MMI_ZI__)
         inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_SM_STROKE_LOGO);
#else
         inputbox_information_icon = get_image(WGUI_IME_MULTITAP_BPMF_IMG);
#endif
         break;
      case INPUT_MODE_TR_BOPOMO:
#if defined(__MMI_T9__)
         inputbox_information_icon = get_image(WGUI_IME_MULTITAP_BPMF_IMG);
#elif defined(__MMI_ZI__)
         inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_BPMF_LOGO);
#else
         inputbox_information_icon = get_image(WGUI_IME_MULTITAP_BPMF_IMG);
#endif
         break;
      case INPUT_MODE_SM_PINYIN:
#if defined(__MMI_T9__)
        /*shaozhw 20060610 add begin: for support sm_pinyin and sm_stroke */
         #ifdef __MMI_IME_SUPORT_SM_PINYIN_STROKE__
             inputbox_information_icon = get_image(WGUI_IME_SMART_PINYIN_IMG);
         #else
             inputbox_information_icon = get_image(WGUI_IME_MULTITAP_PINYIN_IMG);
         #endif
       /*shaozhw 20060610 add end */
#elif defined(__MMI_ZI__)
         inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_PINYIN_LOGO);
#else
         inputbox_information_icon = get_image(WGUI_IME_MULTITAP_BPMF_IMG);
#endif
         break;
   }
   if (NULL != inputmode_string)
   {
      set_softkey_label(inputmode_string, MMI_CENTER_SOFTKEY);
   }
   else if (NULL != inputbox_information_icon)
   {
      set_softkey_icon(inputbox_information_icon, MMI_CENTER_SOFTKEY);
   }
   redraw_softkey(MMI_CENTER_SOFTKEY);
#endif
}
void mmi_pen_editor_exit_screen(void)
{
	 
	gPenEditorGDIHandler = NULL;
	gPenEditorListMenuHeight = 0;
	if (gPenEditorStrokeGDIBuffer)
	{
#if defined(__NATIVE_JAVA__)
		gcj_ReleaseOverlayBuffer();
#else
		//mmi_frm_scrmem_free((void*)gPenEditorStrokeGDIBuffer);
		OslMfree((void*)gPenEditorStrokeGDIBuffer);
#endif
	}
	gPenEditorStrokeGDIBuffer = NULL;
	mmi_pen_editor_poudkey_down_handler = NULL;
}
 
void mmi_pen_editor_bpmf_pinyin_key_down_handler(S32 key)
{
    /*shaozhw 20060614 add begin: for bug5405 */
    #ifdef __MMI_HAND_WRITING__
    if(UI_is_timer_start(mmi_pen_editor_stroke_up_timer_handler))    
    {
        coolsand_UI_cancel_timer(mmi_pen_editor_stroke_up_timer_handler);
        mmi_pen_editor_stroke_up_timer_handler();
    }
    #endif
    if(UI_is_timer_start(mmi_pen_editor_confirm_selection))                
    {
        coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
        mmi_pen_editor_confirm_selection();
    }
    /*shaozhw 20060614 add end */
    /*shaozhw 20060614 modify begin: for bug5229 */
    //if (key < 0 || key > MAX_MULTITAPS - 1)
    if (key <= 0 || key > MAX_MULTITAPS - 1)
    /*shaozhw 20060614 end */
    {
        return;
    }

    if (MMI_virtual_keyboard.lang_type == MMI_GUI_VIRTUAL_KEYBOARD_CHINESE_SYMBOL)
    {
        mmi_pen_editor_vk_symbol_picker();
    }
    if (MMI_virtual_keyboard.lang_type == MMI_GUI_VIRTUAL_KEYBOARD_TRAY)
    {
        mmi_pen_editor_vk_show();
    }
    if (key != g_pen_editor_active_multitap_index)
    {
        if (g_pen_editor_active_multitap_index >= 0)
        {
            coolsand_UI_multitap_input_complete(&(g_pen_editor_bpmf_pinyin_active_multitap[g_pen_editor_active_multitap_index]));
        }
        g_pen_editor_active_multitap_index = key;
    }

    coolsand_UI_change_multitap_input_state(&(g_pen_editor_bpmf_pinyin_active_multitap[g_pen_editor_active_multitap_index]));
}

void mmi_pen_editor_bpmf_pinyin_key_up_handler(S32 key)
{
    /*shaozhw 20060614 modify begin: for bug5229 */
    /*
    UI_UNUSED_PARAMETER(key);
    coolsand_UI_start_timer(MMI_MULTITAP_TIMEOUT,mmi_pen_editor_bpmf_pinyin_disable_active_multitap);
    */
    if(key)
    {
        UI_UNUSED_PARAMETER(key);
        coolsand_UI_start_timer(MMI_MULTITAP_TIMEOUT,mmi_pen_editor_bpmf_pinyin_disable_active_multitap);
    }
    else
    {
        mmi_pen_editor_input_character(' ',MMI_FALSE);
    }
    /*shaozhw 20060614 modify end */
}

void mmi_pen_editor_bpmf_pinyin_disable_active_multitap(void)
{
	if (g_pen_editor_active_multitap_index < 0)
	{
		return;
	}
	coolsand_UI_multitap_input_complete(&g_pen_editor_bpmf_pinyin_active_multitap[g_pen_editor_active_multitap_index]);
	g_pen_editor_active_multitap_index = -1;
}

void mmi_pen_editor_bpmf_pinyin_multitap_input(UI_character_type c)
{
	if (g_pen_editor_bpmf_pinyin_previous_valid_character)
	{
		mmi_pen_editor_vk_delete_character(&gPenEditorVKText);
	}
	if (mmi_pen_editor_vk_insert_character(&gPenEditorVKText,c))
	{
		/*Check the input is valid or not*/
		if (mmi_pen_editor_get_candidates_by_vk())
		{
			switch (gPenEditorInputBoxType)
			{
				case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
				case PEN_EDITOR_MULTILINE_INPUT_BOX:
				case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
					wgui_inputbox_RSK_label_clear = 1;
					break;

				case PEN_EDITOR_EMS_INPUT_BOX:
					category28_RSK_label_clear= 1;
					break;

				default :
					break;
			}
			/*Temp Solution for LSK Text Update*/
			 
			if (gPenEditorVKText.TextLength == 1 && MMI_FALSE == gPenEditorSelectCandidate)
			{
				mmi_pen_editor_store_LSK_Text();
			}
			mmi_pen_editor_set_left_soft_key_select();
			set_left_softkey_function(mmi_pen_editor_left_soft_key_handler,KEY_EVENT_UP);
			set_left_softkey_function(mmi_pen_editor_hold_confirm,KEY_EVENT_DOWN);
			//gPenEditorSelectCandidate = MMI_TRUE;
			mmi_pen_editor_set_candidate_state(MMI_TRUE);
			switch (gPenEditorInputBoxType)
			{
				case PEN_EDITOR_EMS_INPUT_BOX:
				case PEN_EDITOR_MULTILINE_INPUT_BOX:
				case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
				case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
					mmi_pen_editor_register_arrow_keys();
					break;

				default :
					break;
			}
			mmi_pen_editor_set_right_softkey(PEN_EDITOR_RSK_CLEAR);
			g_pen_editor_bpmf_pinyin_previous_valid_character = c;
		}
		else
		{
			mmi_pen_editor_vk_delete_character(&gPenEditorVKText);
			if (g_pen_editor_bpmf_pinyin_previous_valid_character)
			{
				mmi_pen_editor_vk_insert_character(&gPenEditorVKText,g_pen_editor_bpmf_pinyin_previous_valid_character);
			}
		}
	}
	wgui_set_virtual_keyboard_display_area((U8*)gPenEditorVKText.VKText, MMI_TRUE);
}

void mmi_pen_editor_bpmf_pinyin_multitap_input_complete(void)
{
	g_pen_editor_bpmf_pinyin_previous_valid_character = 0;
}

void mmi_pen_editor_bpmf_pinyin_register_multitap_function(void)
{
	int i = 0;
	register_MMI_key_input_handler();
	register_key_up_handler(mmi_pen_editor_bpmf_pinyin_key_up_handler);
	register_key_down_handler(mmi_pen_editor_bpmf_pinyin_key_down_handler);
	for(i = 0; i < MAX_MULTITAPS; i++)
	{
		coolsand_UI_set_multitap_input_callbacks(&g_pen_editor_bpmf_pinyin_active_multitap[i],mmi_pen_editor_bpmf_pinyin_multitap_input,mmi_pen_editor_bpmf_pinyin_multitap_input_complete);
	}
}

void mmi_pen_editor_bpmf_pinyin_create_multitap(U8 input_mode)
{
	s32 i,width;
	UI_string_type s;

	width=MMI_multitap_width;	/*	Width needs to be auto-calculated	*/
	g_pen_editor_active_multitap_index = -1;
	for(i = 0;i < MAX_MULTITAPS;i++)
	{
		if (INPUT_MODE_TR_MULTITAP_BOPOMO == MMI_current_input_mode)
		{
			s = get_bpmf_multitap_string(i);
			coolsand_UI_create_multitap_input(&g_pen_editor_bpmf_pinyin_active_multitap[i],MMI_multitap_x,MMI_multitap_y,width,MMI_multitap_height,s);
		}
		else if (INPUT_MODE_SM_MULTITAP_PINYIN == MMI_current_input_mode)
		{
			s = (UI_string_type) numberless_lowercase_multitap_strings[i];
			coolsand_UI_create_multitap_input(&g_pen_editor_bpmf_pinyin_active_multitap[i],MMI_multitap_x,MMI_multitap_y,width,MMI_multitap_height,s);
		}
		else
		{
			MMI_ASSERT(0);
		}
	}
}
void mmi_pen_editor_hold_confirm(void)
{
	if (gPenEditorSelectCandidate)
	{
		//coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);//no cancel timer for auto select word for bug 13382 by qiff
//	COS_KillTimer(GetMmiTaskHandle(MOD_TP_TASK),TPTimerIdHandwritingConfirm );
		g_pen_editor_hold_confirm = MMI_TRUE;
	}
}
void mmi_pen_editor_hold_confirm_in_callback_space(void)
{
	if (gPenEditorSelectCandidate)
	{
		g_pen_editor_hold_confirm = MMI_TRUE;
	}
}
void mmi_pen_editor_do_confirm(void)
{
	mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: mmi_pen_editor_do_confirm %d",gPenEditorSelectCandidate);
	// modify by chenq for 6093 20080815 E
	#ifdef __MMI_HAND_WRITING__
	if(UI_is_timer_start(mmi_pen_editor_stroke_up_timer_handler)) 
	{
		coolsand_UI_cancel_timer(mmi_pen_editor_stroke_up_timer_handler);
		mmi_pen_editor_stroke_up_timer_handler();
	}
	#endif
	if (gPenEditorSelectCandidate)
	{	
		mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: timer is on");

		if(UI_is_timer_start(mmi_pen_editor_confirm_selection)) 
		{
			coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
			mmi_pen_editor_confirm_selection();
		}
	}
  // modify by chenq for 6093 20080815 E
}
#ifdef __MMI_HAND_WRITING__
extern GBIMESTATE gbstate;
void mmi_pen_editor_change_handwriting_area_gb(void)
{
	
      mmi_pen_handwriting_area_struct ext_stroke;

      ext_stroke.x1 = 0;
      ext_stroke.y1 = 0;
      ext_stroke.x2 = UI_device_width - 1;
      ext_stroke.y2 = UI_device_height - 1;
#if defined(__MMI_GB__)
	switch(gbstate)
	{
		case GBIMESTATE_INPUT:
			g_pen_editor_original_handwriting_area.y2 = gbCompositionArea.y;
			break;
		case GBIMESTATE_ASSATION:
			g_pen_editor_original_handwriting_area.y2 = gbCandidateArea.y;
			break;
		case GBIMESTATE_IDLE:
			g_pen_editor_original_handwriting_area.y2 = gbCandidateArea.y;
			break;
	}
#endif    
	mmi_trace(g_sw_HandW,"MMI_PEN_EDIOTR: mmi_pen_editor_change_handwriting_area_gb---y2 is:%d",g_pen_editor_original_handwriting_area.y2);
      mmi_pen_change_handwriting_area(1,&g_pen_editor_original_handwriting_area,&ext_stroke);
   }
#endif
/*shaozhw 20060610 add begin: for support sm_pinyin and sm_stroke */
#ifdef __MMI_IME_SUPORT_SM_PINYIN_STROKE__

extern void T9KeyArrowUPHandlerForEMSInputBox(void);
extern void T9KeyArrowRightHandlerForEMSInputBox(void);
extern void T9KeyArrowDownHandlerForEMSInputBox(void);
extern void T9KeyArrowLeftHandlerForEMSInputBox(void);
extern void T9KeyArrowUPHandlerMultilineInputBox(void);
extern void T9KeyArrowRightHandlerMultilineInputBox(void);
extern void T9KeyArrowDownHandlerMultilineInputBox(void);
extern void T9KeyArrowLeftHandlerMultilineInputBox(void);

void RegisterCandidateInputBoxArrowKeys(BOOL IsPenEditor)
{
    if(IsPenEditor)
    {
        SetKeyHandler(mmi_pen_editor_get_candidates_prev,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
        SetKeyHandler(mmi_pen_editor_get_candidates_next,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
        SetKeyHandler(mmi_pen_editor_get_candidates_prev_page,KEY_UP_ARROW,KEY_EVENT_DOWN);
        SetKeyHandler(mmi_pen_editor_get_candidates_next_page,KEY_DOWN_ARROW,KEY_EVENT_DOWN);
        //gPenEditorSelectCandidate = TRUE;
        mmi_pen_editor_set_candidate_state(MMI_TRUE);
    }
    else
    {
        switch (gPenEditorInputBoxType)
        {
        /*wuzc Del Start For SIMM Ver: MT1.9.2  on 2007-1-18 16:29 */
            #if 0
            case PEN_EDITOR_EMS_INPUT_BOX:
                SetKeyHandler(T9KeyArrowUPHandlerForEMSInputBox,   KEY_UP_ARROW,   KEY_EVENT_DOWN);
                SetKeyHandler(T9KeyArrowRightHandlerForEMSInputBox,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
                SetKeyHandler(T9KeyArrowDownHandlerForEMSInputBox, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
                SetKeyHandler(T9KeyArrowLeftHandlerForEMSInputBox, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
                break;
            case PEN_EDITOR_MULTILINE_INPUT_BOX:
            case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
            case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
                SetKeyHandler(T9KeyArrowUPHandlerMultilineInputBox,   KEY_UP_ARROW,   KEY_EVENT_DOWN);
                SetKeyHandler(T9KeyArrowRightHandlerMultilineInputBox,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
                SetKeyHandler(T9KeyArrowDownHandlerMultilineInputBox, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
                SetKeyHandler(T9KeyArrowLeftHandlerMultilineInputBox, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
                break;
               #endif
		/*wuzc Del End  For SIMM Ver: MT1.9.2  on 2007-1-18 16:29 */
            case PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX:
                break;
            case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
                break;
            case PEN_EDITOR_INLINE_TEXT:
                break;
            default:
                break;
        }

        //gPenEditorSelectCandidate = FALSE;
        mmi_pen_editor_set_candidate_state(MMI_FALSE);
    }
}
#endif
/*shaozhw 20060610 add end */

BOOL change_input_mode_when_writing(void) 
{
  BOOL v_ret = FALSE;
    #ifdef __MMI_HAND_WRITING__
    if(UI_is_timer_start(mmi_pen_editor_stroke_up_timer_handler)) 
     {
      coolsand_UI_cancel_timer(mmi_pen_editor_stroke_up_timer_handler);
      mmi_pen_editor_stroke_up_timer_handler();
     }
    #endif
	if (gPenEditorSelectCandidate)
	{	
     if(UI_is_timer_start(mmi_pen_editor_confirm_selection)) 
     {
    	v_ret = TRUE;
     }
	}

  return v_ret;
}

static S32 get_VK_height(void) {
	// Please refer to editor size initialization in draw manager
	// editor height = content height - info bar height - 
	// 		multitap height - 1;
	// So if vk height or candidate height < multitap height + 1
	// we should minus (multitap height + 1) from editor size.  
	// When VK presents as tray, its height should be maximum value between
	// it and maximum blank area
	S32 h = get_max_blank_area_height();

	h = (MMI_virtual_keyboard.height > h) ? MMI_virtual_keyboard.height : h;
	return h;
}
static S32 get_max_blank_area_height(void) {
	// Please refer to editor size initialization in draw manager
	// editor height = content height - info bar height - 
	// 		multitap height - 1;
	// Height of blank area is maximum value from (MMI_multitap_height + 1)
	// and candidates height.
	S32 h = MMI_multitap_height + 1;

	h = (gPenEditorCandidateInputBox.height > h) ? 
		gPenEditorCandidateInputBox.height : h;
	return h;
}
static S32 get_changed_height(
	mmi_pen_editor_virtual_keyboard_state_enum isVKVisible, BOOL isCandidateVisible) {
	/*----------------------------------------------------------------*/
   	/* Code Body                                                      */
   	/*----------------------------------------------------------------*/
	S32 top = 0;
    S32 tmp = 0;
    S32  h; 

	// get maximum editor height
   	switch (gPenEditorInputBoxType) {
		case PEN_EDITOR_MULTILINE_INPUT_BOX:
		case PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX:
			top = MMI_multiline_inputbox.y;
			tmp = MMI_multiline_inputbox.height;
			break;
		case PEN_EDITOR_EMS_INPUT_BOX:
			top = MMI_EMS_inputbox.y;
			tmp = MMI_EMS_inputbox.height;
			break;
		case PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX:
		case PEN_EDITOR_DICT_MULTILINE_INPUT_BOX:
			top = MMI_fixed_list_menu.y;
			tmp = MMI_fixed_list_menu.height;
			break;
		case PEN_EDITOR_SINGLE_LINE_INPU_BOX:
    case PEN_EDITOR_INLINE_TEXT:
		case PEN_EDITOR_DATE:
		case PEN_EDITOR_IP4:
		case PEN_EDITOR_TIME:
		case PEN_EDITOR_TIME_PERIOD:
			break;
		default :
			MMI_ASSERT(0);
			break;
	}

	// get maximum editor height
	h = get_max_editor_height(top);
	
	// get changed editor height
	h -= tmp;
	if (isVKVisible == PEN_EDITOR_VK_ON) {
		h -= get_VK_height();
	}

	if (isCandidateVisible) {
		h -= get_max_blank_area_height();
    }
	  
	return h;
}

void mmi_pen_editor_set_vk_hw_pre_handler(fnPtr	vk_pre,fnPtr hw_pre)
{
	mmi_pen_editor_vk_pre_handler = vk_pre;
	mmi_pen_editor_hw_pre_handler = hw_pre;
}
void mmi_pen_editor_reset_vk_inputs(void)
{
	if(gPenEditorVKText.TextLength != 0)
	{
		mmi_pen_editor_reset_vk_text(&gPenEditorVKText);
		wgui_set_virtual_keyboard_display_area((U8*)gPenEditorVKText.VKText, MMI_FALSE);
	}	 
}
#endif /*__MMI_TOUCH_SCREEN__*/

#ifdef TP_USE_NAVIGATE_KEYS
static U8 candidate_curr_position = 0;
//keys down handle
//keys up handle
void mmi_pen_editor_left_arrow_key_handler(void)
{
//promt if candidates for choose
	coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);

	//goto pre candidates for choose, circle 
	//candidate size: PEN_EDITOR_CANDIDATE_SIZE
	//hode current position
	if(candidate_curr_position)
	{
		coolsand_UI_single_line_input_box_previous(&gPenEditorCandidateInputBox);
		candidate_curr_position-- ;
	}
	else
	{
		coolsand_UI_single_line_input_box_goto_last_character(&gPenEditorCandidateInputBox);
		candidate_curr_position = PEN_EDITOR_CANDIDATE_SIZE -1;
	}
	/*According different input box, do different actions*/
	mmi_pen_editor_input_character(gPenEditorCandidateBuffer[candidate_curr_position], MMI_TRUE);

	//redraw candidates area
	coolsand_UI_lock_double_buffer();
	//coolsand_UI_show_single_line_input_box(&gPenEditorCandidateInputBox);
	mmi_pen_editor_show_candidate_area(g_pen_editor_candidate_state.arrow_enable, PEN_EDITOR_CANDIDATE_ON_AND_FOCUS);
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);

}
void mmi_pen_editor_right_arrow_key_handler(void)
{

	coolsand_UI_cancel_timer(mmi_pen_editor_confirm_selection);
	if(candidate_curr_position < PEN_EDITOR_CANDIDATE_SIZE - 1 )
	{
		coolsand_UI_single_line_input_box_next(&gPenEditorCandidateInputBox);
		candidate_curr_position++ ;
	}
	else
	{
		coolsand_UI_single_line_input_box_goto_first_character(&gPenEditorCandidateInputBox);
		coolsand_UI_single_line_input_box_next(&gPenEditorCandidateInputBox);
		candidate_curr_position = 0;
	}
	mmi_pen_editor_input_character(gPenEditorCandidateBuffer[candidate_curr_position], MMI_TRUE);

	//redraw candidates area
	coolsand_UI_lock_double_buffer();
	//coolsand_UI_show_single_line_input_box(&gPenEditorCandidateInputBox);
	mmi_pen_editor_show_candidate_area(g_pen_editor_candidate_state.arrow_enable, PEN_EDITOR_CANDIDATE_ON_AND_FOCUS);
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);

}

void mmi_pen_editor_reset_candidate_position(void)
{
	/*save arrow keys handle*/
	candidate_curr_position = 0;
}
void mmi_pen_editor_hode_candidate_state(void)
{
	//restart auto select timer
#if !defined(__MMI_HANDWRITING_PAD__)
	coolsand_UI_start_timer(PEN_EDITOR_CONFIRM_PERIOD,mmi_pen_editor_confirm_selection);
#endif

}
void mmi_pen_editor_register_arrow_handler(void)
{

//take care in mmi_pen_editor_hold_confirm for the local variable
	SetKeyHandler(mmi_pen_editor_hode_candidate_state,KEY_LEFT_ARROW,KEY_EVENT_UP);
	SetKeyHandler(mmi_pen_editor_hode_candidate_state,KEY_RIGHT_ARROW,KEY_EVENT_UP);
	SetKeyHandler(mmi_pen_editor_hode_candidate_state,KEY_UP_ARROW,KEY_EVENT_UP);
	SetKeyHandler(mmi_pen_editor_hode_candidate_state,KEY_DOWN_ARROW,KEY_EVENT_UP);

	SetKeyHandler(mmi_pen_editor_left_arrow_key_handler,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(mmi_pen_editor_right_arrow_key_handler,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(mmi_pen_editor_left_arrow_key_handler,KEY_UP_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(mmi_pen_editor_right_arrow_key_handler,KEY_DOWN_ARROW,KEY_EVENT_DOWN);
	
}


#endif

#ifdef __MMI_TOUCH_SCREEN__
BOOL mmi_pen_in_handwriting_process(void)
{
    BOOL is_stroke_down;

    if(gPenEditorEnableHandWriting)
    {
        mmi_pen_peek_stroke_state(&is_stroke_down);
        if(is_stroke_down&&gPenEditorIsHandWriting)
        {
            return MMI_TRUE;
        }
	#ifdef __MMI_HAND_WRITING__
        if(UI_is_timer_start(mmi_pen_editor_stroke_up_timer_handler))    
        {
            return MMI_TRUE;
        }
	#endif
    }
    return MMI_FALSE;
}
#endif

#endif
