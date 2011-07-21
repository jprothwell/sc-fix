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
 *  wgui_categories_inputs.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  Input/Editor related categories.
 *  Author:
 * -------
 
 
 
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef __WGUI_CATEGORIES_INPUTS_H__
#define __WGUI_CATEGORIES_INPUTS_H__

#include "gui_inputs.h"
#include "wgui.h"
 
#include "mmi_features.h"
/* CSD end */
#include "wgui_categories_inputsenum.h"
extern void (*wgui_singleline_inputbox_RSK_function) (void);

 
extern void (*wgui_singleline_inputbox_LSK_function) (void);

/* store rsk key string of single input box */
extern UI_string_type wgui_singleline_inputbox_LSK_label_string;

/* store rsk key icon of single input box */
extern PU8 wgui_singleline_inputbox_LSK_label_icon;

#define MULTILINE_INPUTBOX_HEIGHT_PAD  4

extern BOOL r2lMMIFlag;
extern MMI_BOOL wgui_is_r2l_display(void);

/* Set this to 1 for (0 -> + -> 0) sequence on 0 key and (* -> p -> w -> *)
   sequence on * key.
   Set this to 0 for (0 only) on 0 key and (* -> + -> p -> w -> *)
   sequence on * key                                        */
#ifdef __MMI_MULTITAP_KEY_0__
#define ENABLE_DIALER_ZERO_KEY_MULTITAP            1
#else 
#define ENABLE_DIALER_ZERO_KEY_MULTITAP            0
#endif 

#define DISABLE_DIALER_BACK_KEY                 1
#define ENABLE_DIALER_PLUS_CHARACTER_HANDLING      1
#define MULTILINE_INPUTBOX_EDIT_HEIGHT_PAD         8
#define MULTILINE_INPUTBOX_HEIGHT_PAD           4
#define MULTILINE_INPUTBOX_LINE_PAD             1

#define WGUI_DIALER_BOX_ACTIVE_MULTITAP_NONE    0
#define WGUI_DIALER_BOX_ACTIVE_MULTITAP_STAR_KEY   1
#define WGUI_DIALER_BOX_ACTIVE_MULTITAP_ZERO_KEY   2
#define WGUI_DIALER_BOX_ACTIVE_MULTITAP_ANY        4

#define INPUT_TYPE_MASK                0x00ff

#define INPUT_TYPE_USE_ENCODING_BASED_LENGTH    0x0100
#define INPUT_TYPE_USE_ONLY_ENGLISH_MODES       0x0200
#define INPUT_TYPE_ONE_LESS_CHARACTER           0x0400
#define INPUT_TYPE_FORTY_FOUR_LESS_CHARACTER    0x0800
#define INPUT_TYPE_OVERRIDE_DEFAULT             0x1000
#define INPUT_TYPE_PLUS_CHARACTER_HANDLING      0x2000
#define INPUT_TYPE_GSM_EXTENDED_HANDLING        0x4000
#define INPUT_TYPE_SMS_CHAT_MAX_LENGTH_HANDLING 0x8000

 
#define INPUT_TYPE_EXT_USE_0x81_ENCODING        0x0001

#define INPUT_TYPE_EXT_SIM_NUMERIC              0x0002

#define INPUT_TYPE_EXT_NO_SHOW_NEW_LINE_SYMBOL  0x0004

extern multitap_input wgui_dialer_box_star_key_handler;
extern multitap_input wgui_dialer_box_zero_key_handler;
extern U8 wgui_dialer_box_active_multitap;

 
#if defined(__MMI_MULTITAP_THAI__)
#define SINGLELINE_INPUTBOX                        0
#define MULTILINE_INPUTBOX                      1
#define EMS_INPUTBOX                         2
#define  QSEARCH_INPUTBOX                       3
#endif /* defined(__MMI_MULTITAP_THAI__) */ 
 
typedef enum
{
    EDITOR_MULTILINE_INPUT_BOX,
    EDITOR_EMS_INPUT_BOX,
    EDITOR_INLINE_MULTILINE_INPUT_BOX,
    EDITOR_SINGLE_LINE_INPU_BOX,
    EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX,
    EDITOR_INLINE_TEXT,
    EDITOR_NONE_INPUT_BOX
} mmi_editor_inoutbox_enum;

/*
 * extern UI_character_type   wgui_dialer_box_star_key_string[];
 * extern UI_character_type   wgui_dialer_box_zero_key_string[];
 * extern byte wgui_multiline_inputbox_input_mode_changed;
 * extern byte wgui_multiline_inputbox_input_type;
 * extern byte wgui_multiline_inputbox_decimal_input_mode_flag;
 * extern S32 (*wgui_inputbox_UCS2_count_function)(void);
 * extern S32 (*wgui_inputbox_GSM_ext_count_function)(void);
 * extern byte       wgui_multiline_inputbox_decimal_flag;
 * extern S32  wgui_inputbox_buffer_size;
 * extern multitap_input      wgui_inputbox_multitap_zero_key;
 * extern UI_character_type   wgui_inputbox_phone_number_zero_key_string;
 */
 
typedef struct _singleline_inputbox_category_history
{
    U16 history_ID;
    S16 text_length;
    S16 allocated_length;
    S16 available_length;
    S16 current_position;
    S16 text_offset_x;
    S16 input_type;
    S16 UCS2_count;
    U32 flags;
#if defined(__MMI_TOUCH_SCREEN__)
    S16 vk_lang_type;
#endif 
} singleline_inputbox_category_history;

typedef struct _mutliline_inputbox_category_history
{
    U16 history_ID;
    S16 text_length;
    S16 allocated_length;
    S16 available_length;
    S16 n_lines;
    S16 text_height;
    S16 text_offset_x;
    S16 text_offset_y;
    S16 cursor_x;
    S16 cursor_y;
    S16 cursor_line_position_counter;
    S16 line_before_cursor_line_p;
    S16 cursor_line_p;
    S16 cursor_p;
    S16 line_after_cursor_line_p;
    S16 line_after_cursor_line_last_p;
    S16 last_position_p;
    S16 input_type;
    U32 flags;   
    S16 UCS2_count;
    S32 hilite_idx;
#if defined(__MMI_TOUCH_SCREEN__)
    S16 vk_lang_type;
#endif 
    S8 state;   /* CSD 2004-02-13 Wilson, To keep current screen state in history buffer. */
    //barcode
	S32	current_hilite_idx;
   S32	curr_hilite_text_offset_y;
	HILITE_STR 		*hilite_list;
	U16	hilite_str_number;
	void 	(*hilite_cb)(S32 idx);
	U32     ext_flags;   
 
   /*** ritesh started****/
#ifdef __UI_PAGING_IN_MULTILINE_EDITOR__
    U32 start_page_id;
    U32 end_page_id;
    S32 text_offset;
    //U32 ext_flags;
    U8 previous_shared_bytes;
#endif /* __UI_PAGING_IN_MULTILINE_EDITOR__ */ 
   /*** ritesh ended****/
} multiline_inputbox_category_history;

extern void wgui_hide_input_information_bar(void);
extern void wgui_dialer_inputbox_set_border(MMI_BOOL is_border_existing, color_t border_color);
extern void wgui_setup_inputbox_ext(
                S32 x,
                S32 y,
                S32 width,
                S32 height,
                U8 *buffer,
                S32 buffer_size,
                U16 category_screen_ID,
                UI_string_type RSK_label,
                PU8 RSK_icon,
                U16 input_type,
                U8 *history_buffer,
                U8 information_bar_flag,
                U32 additional_flags,
                S16 min_text_height,
                S16 min_line_height,
                S16 header_height,
                S16 footer_height,
                multi_line_input_box_icon_hdlr icon_callback);
extern void wgui_setup_inputbox(
                S32 x,
                S32 y,
                S32 width,
                S32 height,
                U8 *buffer,
                S32 buffer_size,
                U16 category_screen_ID,
                UI_string_type RSK_label,
                PU8 RSK_icon,
                U16 input_type,
                U8 *history_buffer,
                U8 information_bar_flag);
extern S32 wgui_multiline_inputbox_get_UCS2_count(void);
extern void wgui_set_default_input_type(U16 *input_type);
extern void wgui_set_qsearch_default_input_type(U16 *input_type);
extern S32 wgui_inputbox_information_bar_x;
extern S32 wgui_inputbox_information_bar_y;
extern S32 wgui_inputbox_information_bar_width;
extern S32 wgui_inputbox_information_bar_height;
extern U8 wgui_inputbox_information_flag;

#ifdef UI_SMALL_CATEGORY_EDITOR
extern U8 wgui_small_editor_information_flag;
#endif 
extern PU8 wgui_inputbox_information_icon;
extern void wgui_show_inputbox(void);
extern void category5_hide_multitap(void);
extern void set_MMI_current_input_type(void);
extern void wgui_close_inputbox(void);

/* Common inputbox input method functions */
extern void (*wgui_change_inputbox_mode_multitap_ABC) (void);
extern void (*wgui_change_inputbox_mode_multitap_abc) (void);
extern void (*wgui_change_inputbox_mode_mode_numeric) (void);
extern void (*wgui_inputbox_insert_symbol) (UI_character_type c);

/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-10 14:45 */
#if defined(__MMI_GB__)                                                                     //add by yaosq 20061011
extern void (*wgui_change_inputbox_mode_multitap_GBStroke)(void);
extern void (*wgui_change_inputbox_mode_multitap_GBPinYin)(void);
extern void (*wgui_change_inputbox_mode_multitap_GB_SABC)(void);
extern void (*wgui_change_inputbox_mode_multitap_GBSabc)(void);
extern void (*wgui_change_inputbox_mode_multitap_GB_TR_BPMF)(void);
extern void (*wgui_change_inputbox_mode_multitap_GB_TR_STROKE)(void);
#endif
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-10 14:45 */
#if defined(__MMI_SMART_FRENCH__)
extern void (*wgui_change_inputbox_mode_smart_FRENCH)(void);
extern void (*wgui_change_inputbox_mode_smart_french)(void);
#endif
#if defined(__MMI_SMART_URDU__)
extern void (*wgui_change_inputbox_mode_smart_urdu)(void);
#endif
#if defined(__MMI_SMART_PUNJABI__)
extern void (*wgui_change_inputbox_mode_smart_punjabi)(void);
#endif
#if defined(__MMI_SMART_BENGALI__)
extern void (*wgui_change_inputbox_mode_smart_bengali)(void);
#endif
#if defined(__MMI_SMART_TAMIL__)
extern void (*wgui_change_inputbox_mode_smart_tamil)(void);
#endif
#if defined(__MMI_SMART_TURKISH__)
extern void (*wgui_change_inputbox_mode_smart_TURKISH)(void);
extern void (*wgui_change_inputbox_mode_smart_turkish)(void);
#endif
#if defined(__MMI_SMART_INDONESIAN__)
extern void (*wgui_change_inputbox_mode_smart_INDONESIAN)(void);
extern void (*wgui_change_inputbox_mode_smart_indonesian)(void);
#endif
#if defined(__MMI_SMART_MALAY__)
extern void (*wgui_change_inputbox_mode_smart_MALAY)(void);
extern void (*wgui_change_inputbox_mode_smart_malay)(void);
#endif
// beginning--added by kecx for adding italian on 20081223
#if defined(__MMI_SMART_ITALIAN__)
extern void (*wgui_change_inputbox_mode_smart_ITALIAN)(void);
extern void (*wgui_change_inputbox_mode_smart_italian)(void);
#endif
// end--added by kecx for adding italian on 20081223
#if defined(__MMI_MULTITAP_RUSSIAN__)
extern void (*wgui_change_inputbox_mode_multitap_RUSSIAN)(void);
extern void (*wgui_change_inputbox_mode_multitap_russian)(void);
#endif
#if defined(__MMI_SMART_RUSSIAN__)
extern void (*wgui_change_inputbox_mode_smart_RUSSIAN)(void);
extern void (*wgui_change_inputbox_mode_smart_russian)(void);
#endif
#if defined(__MMI_GB_SMART_PORTUGUESE__)
extern void (*wgui_change_inputbox_mode_smart_PORTUGUESE)(void) ;
extern void(*wgui_change_inputbox_mode_smart_portuguese)(void);
#endif

#if defined(__MMI_SMART_ARABIC__)
extern void(*wgui_change_inputbox_mode_smart_arabic)(void);
#endif
#if defined(__MMI_SMART_PERSIAN__)
extern void(*wgui_change_inputbox_mode_smart_persian)(void);
#endif
#if defined(__MMI_SMART_THAI__)
extern void (*wgui_change_inputbox_mode_smart_thai)(void);
#endif
#if defined(__MMI_SMART_VIETNAMESE__)
extern void(*wgui_change_inputbox_mode_smart_VIETNAMESE)(void);
extern void(*wgui_change_inputbox_mode_smart_vietnamese)(void);
#endif
 
#if defined(__MMI_MULTITAP_THAI__)
extern void (*wgui_change_inputbox_mode_multitap_thai) (void);
#endif 
 
 
#if defined(__MMI_MULTITAP_HEBREW__)
extern void (*wgui_change_inputbox_mode_multitap_hebrew) (void);
#endif 
 
 
//for Spanish French German Italian Russian Turkish Portuguese Indonesian Malay Vietnamese
#if defined(__MMI_MULTITAP_SPANISH__)
extern void (*wgui_change_inputbox_mode_multitap_SPANISH) (void);
extern void (*wgui_change_inputbox_mode_multitap_spanish) (void);
#endif /* defined(__MMI_MULTITAP_SPANISH__) */ 

    //qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)
extern void (*wgui_change_inputbox_mode_multitap_tamil) (void);
#endif
#if defined(__MMI_MULTITAP_BENGALI__)
extern void (*wgui_change_inputbox_mode_multitap_bengali) (void);

#endif
#if defined(__MMI_MULTITAP_PUNJABI__)
extern void (*wgui_change_inputbox_mode_multitap_punjabi) (void);
#endif
#if defined(__MMI_MULTITAP_TELUGU__)
extern void (*wgui_change_inputbox_mode_multitap_telugu) (void);
#endif
#if defined(__MMI_MULTITAP_KANNADA__)
extern void (*wgui_change_inputbox_mode_multitap_kannada) (void);

#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)
extern void (*wgui_change_inputbox_mode_multitap_malayalam) (void);
#endif
#if defined(__MMI_MULTITAP_ORIYA__)
extern void (*wgui_change_inputbox_mode_multitap_oriya) (void);
#endif
//qiff add end
#if defined(__MMI_MULTITAP_MARATHI__)
extern void (*wgui_change_inputbox_mode_multitap_marathi) (void);
#endif
#if defined(__MMI_MULTITAP_URDU__)
extern void (*wgui_change_inputbox_mode_multitap_urdu) (void);
#endif
 
#if defined(__MMI_MULTITAP_DANISH__)
extern void (*wgui_change_inputbox_mode_multitap_DANISH) (void);
extern void (*wgui_change_inputbox_mode_multitap_danish) (void);
#endif /* defined(__MMI_MULTITAP_DANISH__) */ 
 
 
#if defined(__MMI_MULTITAP_POLISH__)
extern void (*wgui_change_inputbox_mode_multitap_POLISH) (void);
extern void (*wgui_change_inputbox_mode_multitap_polish) (void);
#endif /* defined(__MMI_MULTITAP_POLISH__) */ 
 
#if defined(__MMI_MULTITAP_FRENCH__)
extern void (*wgui_change_inputbox_mode_multitap_FRENCH) (void);
extern void (*wgui_change_inputbox_mode_multitap_french) (void);
#endif /* defined(__MMI_MULTITAP_FRENCH__) */ 
#if defined(__MMI_MULTITAP_MYANMAR__)
extern void (*wgui_change_inputbox_mode_multitap_myanmar) (void);
#endif
#if defined(__MMI_MULTITAP_GERMAN__)
extern void (*wgui_change_inputbox_mode_multitap_GERMAN) (void);
extern void (*wgui_change_inputbox_mode_multitap_german) (void);
#endif /* defined(__MMI_MULTITAP_GERMAN__) */ 
#if defined(__MMI_MULTITAP_ITALIAN__)
extern void (*wgui_change_inputbox_mode_multitap_ITALIAN) (void);
extern void (*wgui_change_inputbox_mode_multitap_italian) (void);
#endif /* defined(__MMI_MULTITAP_ITALIAN__) */ 
#if defined(__MMI_MULTITAP_RUSSIAN__)
extern void (*wgui_change_inputbox_mode_multitap_RUSSIAN) (void);
extern void (*wgui_change_inputbox_mode_multitap_russian) (void);
#endif /* defined(__MMI_MULTITAP_RUSSIAN__) */ 
#if defined(__MMI_MULTITAP_BULGARIAN__)
extern void (*wgui_change_inputbox_mode_multitap_BULGARIAN) (void);
extern void (*wgui_change_inputbox_mode_multitap_bulgarian) (void);
#endif /* defined(__MMI_MULTITAP_BULGARIAN__) */ 
#if defined(__MMI_MULTITAP_TURKISH__)
extern void (*wgui_change_inputbox_mode_multitap_TURKISH) (void);
extern void (*wgui_change_inputbox_mode_multitap_turkish) (void);
#endif /* defined(__MMI_MULTITAP_TURKISH__) */ 
#if defined(__MMI_MULTITAP_PORTUGUESE__)
extern void (*wgui_change_inputbox_mode_multitap_PORTUGUESE) (void);
extern void (*wgui_change_inputbox_mode_multitap_portuguese) (void);
#endif /* defined(__MMI_MULTITAP_PORTUGUESE__) */ 
#if defined(__MMI_MULTITAP_HINDI__)
extern void (*wgui_change_inputbox_mode_multitap_HINDI) (void);
#endif 

#if defined(__MMI_MULTITAP_INDONESIAN__)
extern void (*wgui_change_inputbox_mode_multitap_INDONESIAN) (void);
extern void (*wgui_change_inputbox_mode_multitap_indonesian) (void);
#endif /* defined(__MMI_MULTITAP_INDONESIAN__) */ 
 
#if defined(__MMI_MULTITAP_CZECH__)
extern void (*wgui_change_inputbox_mode_multitap_CZECH) (void);
extern void (*wgui_change_inputbox_mode_multitap_czech) (void);
#endif /* defined(__MMI_MULTITAP_CZECH__) */ 
 
#if defined(__MMI_MULTITAP_MALAY__)
extern void (*wgui_change_inputbox_mode_multitap_MALAY) (void);
extern void (*wgui_change_inputbox_mode_multitap_malay) (void);
#endif /* defined(__MMI_MULTITAP_MALAY__) */ 
#if defined(__MMI_MULTITAP_VIETNAMESE__)
extern void (*wgui_change_inputbox_mode_multitap_VIETNAMESE) (void);
extern void (*wgui_change_inputbox_mode_multitap_vietnamese) (void);
#endif /* defined(__MMI_MULTITAP_VIETNAMESE__) */ 
 
#if defined(__MMI_MULTITAP_FINNISH__)
extern void (*wgui_change_inputbox_mode_multitap_FINNISH) (void);
extern void (*wgui_change_inputbox_mode_multitap_finnish) (void);
#endif /* defined(__MMI_MULTITAP_FINNISH__) */ 
#if defined(__MMI_MULTITAP_HUNGARIAN__)
extern void (*wgui_change_inputbox_mode_multitap_HUNGARIAN) (void);
extern void (*wgui_change_inputbox_mode_multitap_hungarian) (void);
#endif /* defined(__MMI_MULTITAP_HUNGARIAN__) */ 
 
 
#if defined(__MMI_MULTITAP_SLOVAK__)
extern void (*wgui_change_inputbox_mode_multitap_SLOVAK) (void);
extern void (*wgui_change_inputbox_mode_multitap_slovak) (void);
#endif /* defined(__MMI_MULTITAP_SLOVAK__) */ 
#if defined(__MMI_MULTITAP_DUTCH__)
extern void (*wgui_change_inputbox_mode_multitap_DUTCH) (void);
extern void (*wgui_change_inputbox_mode_multitap_dutch) (void);
#endif /* defined(__MMI_MULTITAP_DUTCH__) */ 
#if defined(__MMI_MULTITAP_ARABIC__)
extern void (*wgui_change_inputbox_mode_multitap_arabic) (void);
#endif 
/* Norwegian start */
#if defined(__MMI_MULTITAP_NORWEGIAN__)
extern void (*wgui_change_inputbox_mode_multitap_NORWEGIAN) (void);
extern void (*wgui_change_inputbox_mode_multitap_norwegian) (void);
#endif /* defined(__MMI_MULTITAP_NORWEGIAN__) */ 
//Norwegian end
 
 
#if defined(__MMI_MULTITAP_SWEDISH__)
extern void (*wgui_change_inputbox_mode_multitap_SWEDISH) (void);
extern void (*wgui_change_inputbox_mode_multitap_swedish) (void);
#endif /* defined(__MMI_MULTITAP_SWEDISH__) */ 
 
 
#if defined(__MMI_MULTITAP_CROATIAN__)
extern void (*wgui_change_inputbox_mode_multitap_CROATIAN) (void);
extern void (*wgui_change_inputbox_mode_multitap_croatian) (void);
#endif /* defined(__MMI_MULTITAP_CROATIAN__) */ 
#if defined(__MMI_MULTITAP_ROMANIAN__)
extern void (*wgui_change_inputbox_mode_multitap_ROMANIAN) (void);
extern void (*wgui_change_inputbox_mode_multitap_romanian) (void);
#endif /* defined(__MMI_MULTITAP_ROMANIAN__) */ 
#if defined(__MMI_MULTITAP_SLOVENIAN__)
extern void (*wgui_change_inputbox_mode_multitap_SLOVENIAN) (void);
extern void (*wgui_change_inputbox_mode_multitap_slovenian) (void);
#endif /* defined(__MMI_MULTITAP_SLOVENIAN__) */ 
 

 
#if defined(__MMI_MULTITAP_GREEK__)
extern void (*wgui_change_inputbox_mode_multitap_GREEK) (void);
extern void (*wgui_change_inputbox_mode_multitap_greek) (void);
#endif /* defined(__MMI_MULTITAP_GREEK__) */ 
 
#if defined(__MMI_T9__) || defined(__MMI_ZI__) || defined(__MMI_KA__) || defined(__MMI_ITAP__)
extern void (*wgui_change_inputbox_mode_smart_ABC) (void);
extern void (*wgui_change_inputbox_mode_smart_abc) (void);
extern void (*wgui_change_inputbox_mode_tr_bpmf) (void);
extern void (*wgui_change_inputbox_mode_tr_stroke) (void);
extern void (*wgui_change_inputbox_mode_sm_pinyin) (void);
extern void (*wgui_change_inputbox_mode_sm_stroke) (void);
extern void (*wgui_change_inputbox_mode_tr_multitap_bpmf) (void);
extern void (*wgui_change_inputbox_mode_sm_multitap_pinyin) (void);
extern void (*wgui_change_inputbox_mode_smart_thai) (void);
extern void (*wgui_change_inputbox_mode_smart_arabic) (void);

 
extern void (*wgui_change_inputbox_mode_smart_hindi) (void);

 
 
extern void (*wgui_change_inputbox_mode_smart_hebrew) (void);

 
 
#if defined(__MMI_T9_SPANISH__) || defined(__MMI_ZI_EU_SPANISH__) || defined(__MMI_ZI_SA_SPANISH__)
extern void (*wgui_change_inputbox_mode_smart_SPANISH) (void);
extern void (*wgui_change_inputbox_mode_smart_spanish) (void);
#endif /* defined(__MMI_T9_SPANISH__) || defined(__MMI_ZI_EU_SPANISH__) || defined(__MMI_ZI_SA_SPANISH__) */ 
 
#if defined(__MMI_T9_DANISH__) || defined(__MMI_ZI_DANISH__)
extern void (*wgui_change_inputbox_mode_smart_DANISH) (void);
extern void (*wgui_change_inputbox_mode_smart_danish) (void);
#endif /* defined(__MMI_T9_DANISH__) || defined(__MMI_ZI_DANISH__) */ 
 
 
#if defined(__MMI_T9_POLISH__) || defined(__MMI_ZI_POLISH__)
extern void (*wgui_change_inputbox_mode_smart_POLISH) (void);
extern void (*wgui_change_inputbox_mode_smart_polish) (void);
#endif /* defined(__MMI_T9_POLISH__) || defined(__MMI_ZI_POLISH__) */ 
 
#if defined(__MMI_T9_FRENCH__) || defined(__MMI_ZI_CA_FRENCH__) || defined(__MMI_ZI_EU_FRENCH__)
extern void (*wgui_change_inputbox_mode_smart_FRENCH) (void);
extern void (*wgui_change_inputbox_mode_smart_french) (void);
#endif /* defined(__MMI_T9_FRENCH__) || defined(__MMI_ZI_CA_FRENCH__) || defined(__MMI_ZI_EU_FRENCH__) */ 
#if defined(__MMI_T9_GERMAN__) || defined(__MMI_ZI_GERMAN__)
extern void (*wgui_change_inputbox_mode_smart_GERMAN) (void);
extern void (*wgui_change_inputbox_mode_smart_german) (void);
#endif /* defined(__MMI_T9_GERMAN__) || defined(__MMI_ZI_GERMAN__) */ 
#if defined(__MMI_T9_ITALIAN__) || defined(__MMI_ZI_ITALIAN__)
extern void (*wgui_change_inputbox_mode_smart_ITALIAN) (void);
extern void (*wgui_change_inputbox_mode_smart_italian) (void);
#endif /* defined(__MMI_T9_ITALIAN__) || defined(__MMI_ZI_ITALIAN__) */ 
 
#if defined(__MMI_T9_RUSSIAN__) || defined (__MMI_ZI_RUSSIAN__) || defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__)
extern void (*wgui_change_inputbox_mode_smart_RUSSIAN) (void);
extern void (*wgui_change_inputbox_mode_smart_russian) (void);
#endif /* defined(__MMI_T9_RUSSIAN__) || defined (__MMI_ZI_RUSSIAN__) || defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__) */ 
#if defined(__MMI_T9_BULGARIAN__) || defined (__MMI_ZI_BULGARIAN__)
extern void (*wgui_change_inputbox_mode_smart_BULGARIAN) (void);
extern void (*wgui_change_inputbox_mode_smart_bulgarian) (void);
#endif /* defined(__MMI_T9_BULGARIAN__) || defined (__MMI_ZI_BULGARIAN__) */ 
 
 
#if defined(__MMI_ZI_TURKISH__) || defined(__MMI_T9_TURKISH__)
 
extern void (*wgui_change_inputbox_mode_smart_TURKISH) (void);
extern void (*wgui_change_inputbox_mode_smart_turkish) (void);
#endif /* defined(__MMI_ZI_TURKISH__) || defined(__MMI_T9_TURKISH__) */ 
 
#if defined(__MMI_T9_PORTUGUESE__) || defined(__MMI_ZI_EU_PORTUGUESE__) || defined(__MMI_ZI_SA_PORTUGUESE__) || defined(__MMI_ZI_BZ_PORTUGUESE__)
 
extern void (*wgui_change_inputbox_mode_smart_PORTUGUESE) (void);
extern void (*wgui_change_inputbox_mode_smart_portuguese) (void);
#endif /* defined(__MMI_T9_PORTUGUESE__) || defined(__MMI_ZI_EU_PORTUGUESE__) || defined(__MMI_ZI_SA_PORTUGUESE__) || defined(__MMI_ZI_BZ_PORTUGUESE__) */ 
 
#if defined(__MMI_ZI_INDONESIAN__) || defined(__MMI_T9_INDONESIAN__)
extern void (*wgui_change_inputbox_mode_smart_INDONESIAN) (void);
extern void (*wgui_change_inputbox_mode_smart_indonesian) (void);
#endif /* defined(__MMI_ZI_INDONESIAN__) || defined(__MMI_T9_INDONESIAN__) */ 
 
 
 
#if defined(__MMI_T9_CZECH__) || defined(__MMI_ZI_CZECH__)
 
extern void (*wgui_change_inputbox_mode_smart_CZECH) (void);
extern void (*wgui_change_inputbox_mode_smart_czech) (void);
#endif /* defined(__MMI_T9_CZECH__) || defined(__MMI_ZI_CZECH__) */ 
 
 
#if defined(__MMI_ZI_MALAY__) || defined(__MMI_T9_MALAY__)
extern void (*wgui_change_inputbox_mode_smart_MALAY) (void);
extern void (*wgui_change_inputbox_mode_smart_malay) (void);
#endif /* defined(__MMI_ZI_MALAY__) || defined(__MMI_T9_MALAY__) */ 
 
#if defined(__MMI_ZI_VIETNAMESE__) || defined(__MMI_T9_VIETNAMESE__)
 
extern void (*wgui_change_inputbox_mode_smart_VIETNAMESE) (void);
extern void (*wgui_change_inputbox_mode_smart_vietnamese) (void);
#endif /* defined(__MMI_ZI_VIETNAMESE__) || defined(__MMI_T9_VIETNAMESE__) */ 
 
#if defined(__MMI_T9_FINNISH__) || defined(__MMI_ZI_FINNISH__)
extern void (*wgui_change_inputbox_mode_smart_FINNISH) (void);
extern void (*wgui_change_inputbox_mode_smart_finnish) (void);
#endif /* defined(__MMI_T9_FINNISH__) || defined(__MMI_ZI_FINNISH__) */ 
 
#if defined(__MMI_T9_HUNGARIAN__) || defined(__MMI_ZI_HUNGARIAN__)
 
extern void (*wgui_change_inputbox_mode_smart_HUNGARIAN) (void);
extern void (*wgui_change_inputbox_mode_smart_hungarian) (void);
#endif /* defined(__MMI_T9_HUNGARIAN__) || defined(__MMI_ZI_HUNGARIAN__) */ 
 
 
#if defined(__MMI_T9_SLOVAK__) || defined(__MMI_ZI_SLOVAK__)
extern void (*wgui_change_inputbox_mode_smart_SLOVAK) (void);
extern void (*wgui_change_inputbox_mode_smart_slovak) (void);
#endif /* defined(__MMI_T9_SLOVAK__) || defined(__MMI_ZI_SLOVAK__) */ 
#if defined(__MMI_ZI_DUTCH__) || defined(__MMI_T9_DUTCH__)
extern void (*wgui_change_inputbox_mode_smart_DUTCH) (void);
extern void (*wgui_change_inputbox_mode_smart_dutch) (void);
#endif /* defined(__MMI_ZI_DUTCH__) || defined(__MMI_T9_DUTCH__) */ 
//Norwegian start
 
#if defined(__MMI_T9_NORWEGIAN__) || defined(__MMI_ZI_NORWEGIAN__)
 
extern void (*wgui_change_inputbox_mode_smart_NORWEGIAN) (void);
extern void (*wgui_change_inputbox_mode_smart_norwegian) (void);
#endif /* defined(__MMI_T9_NORWEGIAN__) || defined(__MMI_ZI_NORWEGIAN__) */ 
//Norwegian end
 
 
#if defined(__MMI_T9_SWEDISH__) || defined(__MMI_ZI_SWEDISH__)
extern void (*wgui_change_inputbox_mode_smart_SWEDISH) (void);
extern void (*wgui_change_inputbox_mode_smart_swedish) (void);
#endif /* defined(__MMI_T9_SWEDISH__) || defined(__MMI_ZI_SWEDISH__) */ 
 
 
#if defined(__MMI_T9_CROATIAN__) || defined(__MMI_ZI_CROATIAN__)
extern void (*wgui_change_inputbox_mode_smart_CROATIAN) (void);
extern void (*wgui_change_inputbox_mode_smart_croatian) (void);
#endif /* defined(__MMI_T9_CROATIAN__) || defined(__MMI_ZI_CROATIAN__) */ 
#if defined(__MMI_ZI_ROMANIAN__) || defined(__MMI_T9_ROMANIAN__)
extern void (*wgui_change_inputbox_mode_smart_ROMANIAN) (void);
extern void (*wgui_change_inputbox_mode_smart_romanian) (void);
#endif /* defined(__MMI_ZI_ROMANIAN__) || defined(__MMI_T9_ROMANIAN__) */ 
#if defined(__MMI_T9_SLOVENIAN__) || defined(__MMI_ZI_SLOVENIAN__)
extern void (*wgui_change_inputbox_mode_smart_SLOVENIAN) (void);
extern void (*wgui_change_inputbox_mode_smart_slovenian) (void);
#endif /* defined(__MMI_T9_SLOVENIAN__) || defined(__MMI_ZI_SLOVENIAN__) */ 
 
 
#if defined(__MMI_T9_GREEK__) || defined(__MMI_ZI_GREEK__)
extern void (*wgui_change_inputbox_mode_smart_GREEK) (void);
extern void (*wgui_change_inputbox_mode_smart_greek) (void);
#endif /* defined(__MMI_T9_GREEK__) || defined(__MMI_ZI_GREEK__) */ 
 
//CSD end
#endif /* defined(__MMI_T9__) || defined(__MMI_ZI__) || defined(__MMI_KA__) || defined(__MMI_ITAP__) */ 
/* CSD end */
extern void (*wgui_inputbox_not_empty_callback) (void);
extern void (*wgui_inputbox_empty_callback) (void);
extern void (*wgui_inputbox_validation_callback) (U8 *text, U8 *cursor, S32 text_length);

void RegisterInputBoxValidationFunction(void (*f) (U8 *, U8 *, S32));
void RegisterInputBoxNotEmptyFunction(void (*f) (void));
void RegisterInputBoxEmptyFunction(void (*f) (void));
extern S32 GetInputBoxUCS2Count(void);
extern S32 GetInputBoxExtendedGSMCount(void);

/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-17 14:58 */
#if defined(__MMI_GB__)
 extern void (*wgui_change_inputbox_mode_tr_bpmf)(void);
 extern void (*wgui_change_inputbox_mode_tr_stroke)(void);
 extern void (*wgui_change_inputbox_mode_sm_pinyin)(void);
 extern void (*wgui_change_inputbox_mode_sm_stroke)(void);
 extern void (*wgui_change_inputbox_mode_smart_ABC)(void);
 extern void (*wgui_change_inputbox_mode_smart_abc)(void);
#endif
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-17 14:58 */

extern void wgui_setup_input_information(S32 x, S32 y, S32 width, S32 height);

extern void category5_hide_multitap(void);
extern void RedrawCategory5Screen(void);
extern void ShowCategory5Screen(
                U16 title,
                U16 title_icon,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                S16 input_type,
                U8 *buffer,
                S32 buffer_size,
                U8 *history_buffer);
/* Begin AP Symbol table */
extern void ShowCategory5Screen_ext(
                U16 title,
                U16 title_icon,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                S16 input_type,
                U8 *buffer,
                S32 buffer_size,
                U8 *history_buffer,
                S16 input_ext_type,
                S16 *required_input_mode_set,
                U8 *no_show_symbol_list[]);
/* End AP Symbol table */
extern void ExitCategory5Screen(void);
extern S32 GetCategory5HistorySize(void);
extern U8 *GetCategory5History(U8 *history_buffer);
void SetCategory5RightSoftkeyFunction(void (*f) (void), MMI_key_event_type k);
extern void AppendCategory5String(S16 input_type, U8 *buffer, S32 buffer_size, U8 *s, U8 *history_buffer);

#ifdef UI_SMALL_CATEGORY_EDITOR
extern void category100_hide_multitap(void);
extern void ShowCategory100Screen(
                U16 title,
                U16 title_icon,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                S16 input_type,
                U8 *buffer,
                S32 buffer_size,
                U8 *history_buffer);
extern void ExitCategory100Screen(void);
#endif /* UI_SMALL_CATEGORY_EDITOR */ 
extern void RedrawCategory35Screen(void);
extern void ShowCategory35Screen(
                U8 *left_softkey,
                PU8 left_softkey_icon,
                U8 *right_softkey,
                PU8 right_softkey_icon,
                U8 *message,
                PU8 message_icon,
                S16 input_type,
                U8 *buffer,
                S32 buffer_size,
                U8 *history_buffer);
extern void ExitCategory35Screen(void);
extern S32 GetCategory35HistorySize(void);
extern U8 *GetCategory35History(U8 *history_buffer);
void SetCategory35RightSoftkeyFunction(void (*f) (void), MMI_key_event_type k);

extern void ShowCategory171Screen(
                U8 *left_softkey,
                PU8 left_softkey_icon,
                U8 *right_softkey,
                PU8 right_softkey_icon,
                U8 *message,
                PU8 message_icon,
                S16 input_type,
                U8 *buffer,
                S32 buffer_size,
                U8 *history_buffer);
extern void ExitCategory171Screen(void);
extern U8 *GetCategory171History(U8 *history_buffer);
extern S32 GetCategory171HistorySize(void);
extern void RedrawCategory171Screen(void);
void SetCategory171RightSoftkeyFunction(void (*f) (void), MMI_key_event_type k);

extern void SetInputboxLSKFunction(FuncPtr f);

extern void wgui_handle_dialer_inputbox_right_softkey_down(void);
extern void wgui_handle_dialer_inputbox_right_softkey_up(void);
extern void wgui_handle_dialer_inputbox_right_softkey_long_press(void);

#if defined(__MMI_WCSS_INPUT_FORMAT_SUPPORT__)
#define CAT115_MAXSIZE  128

extern void ShowCategory115Screen(
                U16 title,
                U16 title_icon,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U8 *buffer,
                S32 buffer_size,
                U8 *input_format,
                U8 input_required,
                U8 *history_buffer);
extern U8 *GetCategory115History(U8 *history_buffer);
void SetCategory115RightSoftkeyFunction(void (*f) (void), MMI_key_event_type k);
#endif /* defined(__MMI_WCSS_INPUT_FORMAT_SUPPORT__) */ 

#if defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
#endif 

extern void wgui_input_information_background(S32 x1, S32 y1, S32 x2, S32 y2);
extern void wgui_show_input_mode(U8 input_mode, PU8 inputbox_information_icon);
extern void wgui_setup_input_information(S32 x, S32 y, S32 width, S32 height);

extern void wgui_handle_inputbox_change_input_mode(void);
extern U8 MMI_allow_only_english_input_modes;
extern void change_input_mode(void);
extern void change_multiline_information_bar_input_method(void);
extern void change_multiline_inputbox_input_mode(void);

/* Begin AP input mode set */
extern void wgui_fill_AP_required_input_mode_set(S16 *required_input_mode_set);
extern void wgui_set_AP_required_input_mode_set_flag(void);
extern void wgui_unset_AP_required_input_mode_set_flag(void);

/* End AP input mode set */

extern void (*inputbox_change_mode) (void);
extern U8 MMI_current_input_mode;

extern void wgui_multiline_setup_inputbox(
                S32 x,
                S32 y,
                S32 width,
                S32 height,
                U8 *buffer,
                S32 buffer_size,
                U16 category_screen_ID,
                UI_string_type RSK_label,
                PU8 RSK_icon,
                U16 input_type,
                U8 *history_buffer,
                U8 information_bar_flag);

extern U8 set_multiline_inputbox_category_history(U16 history_ID, U8 *history_buffer, S16 *input_type);
extern void get_multiline_inputbox_category_history(U16 history_ID, U8 *history_buffer, S16 input_type);
extern void set_multiline_inputbox_category_history_state(U16 history_ID, U8 *history_buffer, S8 state);       /* CSD 2004-02-12 Wilson */
extern S8 get_multiline_inputbox_category_history_state(U16 history_ID, U8 *history_buffer);   /* CSD 2004-02-12 Wilson */

/* for E-dictionary */
void wgui_set_e_dictionary_test_clear_inputbox_buffer_funcptr(pBOOL(*pfunc) (void));
extern void wgui_reset_e_dictionary_test_clear_inputbox_buffer_funcptr(void);
extern pBOOL(*wgui_e_dictionary_test_clear_inputbox_buffer) (void);
void Register_cat5_editor_buffer_need_flush(BOOL(*pfunc) (void));
void Register_pound_key_buffer_flush_test_function(BOOL(*pfunc) (void));

/* For Draw Manager  */
extern void get_singleline_inputbox_category_history(U16 history_ID, U8 *history_buffer, S16 input_type);

/* for Chat */
#define CHAT_MESSAGE_BUFFER      (3024+(100<<2))        /* Added  100<<2 for Icon Text(Arrow+Buddy) */
typedef struct _chat_message
{
    U8 *chat_message_string;
    U8 flag;
} chat_message_struct;


extern void ShowCategory305Screen(
                U16 title_id,
                U16 title_icon,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                S32 number_of_items,
                chat_message_struct *chat,
                U8 *history_buffer,
                U8 *top_msg,
                U16 top_msg_icon);

extern void ExitCategory305Screen(void);
extern U8 *GetCategory305History(U8 *history_buffer);
extern S32 GetCategory305HistorySize(void);
extern void RedrawCategory305Screen(void);
extern void ShowCatMessage(U8 *message, U8 type_of_msg);

extern void wgui_set_RSK(void);
#endif /* __WGUI_CATEGORIES_INPUTS_H__ */ 

