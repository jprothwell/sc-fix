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
 *	EditorPen.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for ¡K.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
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
 
 *==============================================================================
 *******************************************************************************/
#ifndef _EDITOR_PEN_H
#define _EDITOR_PEN_H


/* if some header should be include before this one */
#if (0)
/* under construction !*/
#endif
#if defined(__MMI_TOUCH_SCREEN__)|| defined(__MMI_HANDWRITING_PAD__)
#include "gui_data_types.h"
#include "gui_inputs.h"
#include "wgui_touch_screen.h"
#include "wgui_categories_inputs.h"
#include "wgui_inputs.h"
#include "gui_ems.h"
#include "gui_inputs.h"
#include "gui_themes.h"
#include "wgui_datetime.h"
#include "gui_virtual_keyboard.h"
#include "wgui_virtual_keyboard.h"
#include "wgui_inline_edit.h"
#include "touchscreengprot.h"

/***************************************************************************** 
* Define
*****************************************************************************/
#define PEN_EDITOR_CHARACTER_END_PERIOD gPenEditorHWSpeedSet[gPenEditorHWSpeed][0]//750
#define PEN_EDITOR_CONFIRM_PERIOD gPenEditorHWSpeedSet[gPenEditorHWSpeed][1]//1750
#define PEN_EDITOR_MAX_CANDIDATE_SIZE 20
 
#define PEN_EDITOR_CANDIDATE_SIZE (9)//((UI_device_width)/coolsand_UI_get_character_width(0x4E00)-2)
#define PEN_EDITOR_CANDIDATE_SIZE_WITH_ARROWS (7)
#define PEN_EDITOR_STROKE_BUFFER_SIZE 500
#define PEN_EDITOR_MAX_CONTROL_SET_SIZE 10
#define PEN_EDITOR_MAX_VK_TEXT_SIZE 6
#define PEN_EDITOR_MAX_BOPOMOFO_SIZE 4
#define PEN_EDITOR_MAX_PINYIN_SIZE 6
#define PEN_EDITOR_GUI_MULTI_LINE_INPUT_BOX_SCROLL_DELAY 250
#define PEN_EDITOR_GUI_MULTI_LINE_INPUT_BOX_LINES 25

/***************************************************************************** 
* Typedef 
*****************************************************************************/
typedef enum {
	PEN_EDITOR_TRADITIONAL_CHINESE,
	PEN_EDITOR_SIMPLIFIED_CHINESE,
	PEN_EDITOR_UPPER_CASE_ENGLISH,
	PEN_EDITOR_LOWER_CASE_ENGLISH,
	PEN_EDITOR_NUMBER,
	PEN_EDITOR_PUNCTUATION
} mmi_pen_editor_input_type_enum;

typedef enum {
	PEN_EDITOR_MULTILINE_INPUT_BOX,
	PEN_EDITOR_EMS_INPUT_BOX,
	PEN_EDITOR_INLINE_MULTILINE_INPUT_BOX,
	PEN_EDITOR_SINGLE_LINE_INPU_BOX,
	PEN_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX,
	PEN_EDITOR_INLINE_TEXT,
	PEN_EDITOR_DATE,
	PEN_EDITOR_IP4,
	PEN_EDITOR_TIME,
	PEN_EDITOR_TIME_PERIOD,
	PEN_EDITOR_DICT_MULTILINE_INPUT_BOX,
	PEN_EDITOR_PAGING_IN_MULTLINE_EDITOR,
	PEN_EDITOR_NONE_INPUT_BOX
}mmi_pen_editor_input_box_type_enum;

typedef enum {
	PEN_EDITOR_VK_ON,
	PEN_EDITOR_VK_OFF,
	PEN_EDITOR_VK_GRAY,
	PEN_EDITOR_VK_NONE
}mmi_pen_editor_virtual_keyboard_state_enum;

typedef enum {
	PEN_EDITOR_CONTROL_INFORMATION_BAR,
	PEN_EDITOR_CONTROL_MULTILINE_INPUT_BOX,
	PEN_EDITOR_CONTROL_EMS_INPUT_BOX,
	PEN_EDITOR_CONTROL_INLINE_MULTILINE_INPUT_BOX,
	PEN_EDITOR_CONTROL_SINGLE_LINE_INPU_BOX,
	PEN_EDITOR_CONTROL_INLINE_TEXT,
	PEN_EDITOR_CONTROL_DATE,
	PEN_EDITOR_CONTROL_IP4,
	PEN_EDITOR_CONTROL_TIME,
	PEN_EDITOR_CONTROL_TIME_PERIOD,
	PEN_EDITOR_CONTROL_SCROLL_BAR,
	PEN_EDITOR_CONTROL_VIRTUAL_KEYBOARD,
	PEN_EDITOR_CONTROL_CANDIDATE_AREA,
	PEN_EDITOR_CONTROL_IME_BUTTON,
	PEN_EDITOR_CONTROL_MULTITAP,
	PEN_EDITOR_CONTROL_NONE
}mmi_pen_editor_control_type_enum;
 
typedef enum {
	PEN_EDITOR_SORT_CAPITAL_FIRST,
	PEN_EDITOR_SORT_NON_CAPITAL_FIRST
}mmi_pen_editor_resort_type_enum;

typedef enum {
	PEN_EDITOR_RSK_BY_STATE,
	PEN_EDITOR_RSK_BACK,
	PEN_EDITOR_RSK_CLEAR
}mmi_pen_editor_rsk_type_enum;

typedef struct {
   U8 NumberofControlSets;
   U8 reserverd1;
   U16 reserverd2;
   mmi_pen_editor_control_type_enum PenEditorControlSet[PEN_EDITOR_MAX_CONTROL_SET_SIZE];
}mmi_pen_editor_control_set_struct;

typedef struct {
   S8 TextLength;
   U8 reserverd1;
   U16 reserverd2;
   UI_character_type VKText[PEN_EDITOR_MAX_VK_TEXT_SIZE+1];
}mmi_pen_editor_vk_text_struct;

typedef enum {
	PEN_EDITOR_CANDIDATE_OFF,
	PEN_EDITOR_CANDIDATE_ON_NOT_FOCUS,
	PEN_EDITOR_CANDIDATE_ON_AND_FOCUS
}mmi_pen_editor_candidate_state_enum;

typedef enum {
	PEN_EDITOR_ARROW_PRESS_UP,
	PEN_EDITOR_ARROW_PRESS_DOWN,
	PEN_EDITOR_ARROW_DISABLE
}mmi_pen_editor_arrow_state_enum;

typedef enum {
	PEN_EDITOR_IN_CONTROL,
	PEN_EDITOR_IN_RIGHT_ARROW,
	PEN_EDITOR_IN_LEFT_ARROW,
	PEN_EDITOR_MOVE_OUT,
	PEN_EDITOR_OUT_OF_RANGE
}mmi_pen_editor_control_with_arrow_enum;

typedef struct {
   mmi_pen_editor_candidate_state_enum candidate_state;
   MMI_BOOL arrow_enable;
   mmi_pen_editor_arrow_state_enum left_arrow_state;
   mmi_pen_editor_arrow_state_enum right_arrow_state;
   mmi_pen_editor_control_with_arrow_enum pen_down_state;
   UI_character_type selected_candidate;
}mmi_pen_editor_candidate_state_struct;

typedef enum {
	PEN_EDITOR_ARROW_DIRECTION_UP,
	PEN_EDITOR_ARROW_DIRECTION_DOWN,
	PEN_EDITOR_ARROW_DIRECTION_LEFT,
	PEN_EDITOR_ARROW_DIRECTION_RIGHT
}mmi_pen_editor_arrow_direction_enum;

typedef enum {
	PEN_EDITOR_ARROW_FOR_CANDIDATE,
	PEN_EDITOR_ARROW_FOR_COMPOSITION
}mmi_pen_editor_arrow_type_enum;
/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
void mmi_pen_editor_hide_candidate_area(void);
void mmi_pen_editor_clear_and_show_virtual_keyboard_area(void);
void mmi_pen_editor_hide_virtual_keyboard_area(void);
void mmi_pen_editor_stroke_down_handler(mmi_pen_point_struct pos);
void mmi_pen_editor_stroke_move_handler(mmi_pen_point_struct pos);
void mmi_pen_editor_stroke_up_handler(mmi_pen_point_struct pos);
void mmi_pen_editor_stroke_up_timer_handler(void);
void mmi_pen_editor_confirm_selection(void);
int mmi_pen_editor_get_candidates_from_hand_writing(void);
void mmi_pen_editor_input_selection_character(UI_character_type c);
void mmi_pen_editor_pen_down_handler(mmi_pen_point_struct pos);
void mmi_pen_editor_pen_up_handler(mmi_pen_point_struct pos);
void mmi_pen_editor_pen_long_tap_handler(mmi_pen_point_struct pos);
void mmi_pen_editor_pen_repeat_handler(mmi_pen_point_struct pos);
void mmi_pen_editor_pen_abort_handler(mmi_pen_point_struct pos);
void mmi_pen_editor_pen_move_handler(mmi_pen_point_struct pos);
void mmi_pen_editor_pen_set_stroke_color(color_t stroke_color);
void mmi_pen_editor_setup_input_box
         (mmi_pen_handwriting_area_struct* stroke_area,
            mmi_pen_handwriting_area_struct* ext_stroke,
               U16 input_type,byte information_bar_flag,
                  mmi_pen_editor_input_box_type_enum input_box_type);
void mmi_pen_editor_close_input_box(void);
void mmi_pen_editor_show_component(void);
void mmi_pen_editor_add_cocontrol
	(mmi_pen_editor_control_set_struct *peneditorcontrolset,
	   mmi_pen_editor_control_type_enum peneditorcontrol);
void mmi_pen_editor_reset_control_set
	(mmi_pen_editor_control_set_struct *peneditorcontrolset);
void mmi_pen_editor_input_character(UI_character_type c,BOOL needselect);
void mmi_pen_editor_reset_vk_text
	(mmi_pen_editor_vk_text_struct *peneditorvktext);
void mmi_pen_editor_backspace(void);
void mmi_pen_editor_bracket(void);
void mmi_pen_editor_new_line(void);
void mmi_pen_editor_resize_content_area(int changed_height);
void mmi_pen_editor_set_virtual_keyboard_by_input_mode(s32 input_mode);
BOOL mmi_pen_editor_vk_insert_character(mmi_pen_editor_vk_text_struct* vk_text,UI_character_type ch);
BOOL mmi_pen_editor_vk_delete_character(mmi_pen_editor_vk_text_struct* vk_text);
BOOL mmi_pen_editor_get_candidates_by_vk(void);
BOOL mmi_pen_editor_get_candidates_by_phrase(UI_character_type ch);
BOOL mmi_pen_editor_get_candidates_by_page(s32 page);
void mmi_pen_editor_get_candidates_prev_page(void);
void mmi_pen_editor_get_candidates_next_page(void);
void mmi_pen_editor_get_candidates_prev(void);
void mmi_pen_editor_get_candidates_next(void);
void mmi_pen_editor_register_arrow_keys(void);
void mmi_pen_editor_set_inline_text_item(const wgui_inline_item *i);
void mmi_pen_editor_right_soft_key_handler(void);
void mmi_pen_editor_left_soft_key_handler(void);
void mmi_pen_editor_switch_input_method(void);
void mmi_pen_editor_vk_caps_lock(void);
void mmi_pen_editor_vk_insertcharacter(mmi_gui_pen_event_param_struct vk_para);
void mmi_pen_editor_vk_hide(void);
void mmi_pen_editor_vk_show(void);
void mmi_pen_editor_vk_symbol_picker(void);
void mmi_pen_editor_set_inputbox_LSK_Function(FuncPtr f);
void mmi_pen_editor_store_LSK_Text(void);
mmi_gui_virtual_keyboard_language_enum mmi_pen_editor_history_vk_lang_type(void);
MMI_BOOL mmi_pen_editor_check_valid_input(UI_character_type input,const UI_character_type* valid_input);
void mmi_pen_editor_resort_candidates(UI_character_type* candidates,int num, mmi_pen_editor_resort_type_enum SortRequirement);
void mmi_pen_editor_clear_key_handler(void);
void mmi_pen_editor_vk_list_menu_resize(void);
void mmi_pen_editor_store_list_menu_height(s32 list_menu_height);
s32 mmi_pen_editor_get_list_menu_height(void);
void mmi_pen_editor_list_menu_highlight_resize(void);
void mmi_pen_editor_resize_multiline_input_box_for_vk(void);
void mmi_pen_editor_reset_candidate_index(void);
void mmi_pen_editor_set_CSK_content(void);
void mmi_pen_editor_exit_screen(void);
 
void mmi_pen_editor_bpmf_pinyin_key_down_handler(S32 key);
void mmi_pen_editor_bpmf_pinyin_key_up_handler(S32 key);
void mmi_pen_editor_bpmf_pinyin_disable_active_multitap(void);
void mmi_pen_editor_bpmf_pinyin_multitap_input(UI_character_type c);
void mmi_pen_editor_bpmf_pinyin_multitap_input_complete(void);
void mmi_pen_editor_bpmf_pinyin_register_multitap_function(void);
void mmi_pen_editor_bpmf_pinyin_create_multitap(U8 input_mode);
void mmi_pen_editor_hold_confirm(void);
MMI_BOOL mmi_pen_editor_check_input(UI_character_type c);
void mmi_pen_editor_set_vk_to_show(MMI_BOOL enable);
extern void mmi_pen_editor_set_vk_keys(
                MMI_BOOL is_allowed,
                const UI_character_type *characters_lsit,
                const gui_virtual_keyboard_pen_enum *disable_enevt_keys_list);
extern void mmi_pen_editor_do_confirm(void);
extern BOOL mmi_pen_editor_is_arraw_able(void);

#endif /*__MMI_TOUCH_SCREEN__*/
#else
#error "include editorpen.h twice"
#endif
