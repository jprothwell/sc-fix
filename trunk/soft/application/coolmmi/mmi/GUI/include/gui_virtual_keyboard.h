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
 * Filename:
 * ---------
 *   gui_virtual_keyboard.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *   Virtual keyboard - UI component
 * Author:
 * -------
 * -------
 * -------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#ifndef __GUI_VIRTUAL_KEYBOARD_H__
#define __GUI_VIRTUAL_KEYBOARD_H__

#include "mmi_data_types.h"
#include "gui_data_types.h"
#include "mmi_features.h"
#include "gdi_include.h"
#include "gui_theme_struct.h"
#include "wgui.h"

#ifdef __MMI_TOUCH_SCREEN__
#include "kal_non_specific_general_types.h"
#include "touchscreengprot.h"
#endif /* __MMI_TOUCH_SCREEN__ */ 

/***************************************************************************** 
* Define
*****************************************************************************/

#define  MMI_VKBD_MAX_ROW                          5
#define  MMI_VKBD_MAX_COLUMN                       15
#define  MMI_VKBD_MAX_CUSTOM_KEYS                  30
#define  MMI_VKBD_MAX_DISPLAY_AREA_TEXT_LEN        11
#define  MMI_VKBD_MAX_DEADKEY_INPUT                2
#define  MMI_VKBD_MAX_DISABLED_KEYS                7

#define  MMI_VKBD_GRAYSCALE_VALUE                  5

// TODO: Russian should be handled differently
#if defined(__MMI_LANG_RUSSIAN__) || defined(__MMI_LANG_SPANISH__) || defined(__MMI_LANG_FRENCH__) || defined(__MMI_LANG_PORTUGUESE__) || defined(__MMI_LANG_GERMAN__) || defined(__MMI_LANG_ITALIAN__) || defined(__MMI_LANG_TURKISH__)
#define  __MMI_VKBD_EUROPEAN_SUPPORT__
#endif /* European languages */

/***************************************************************************** 
* Typedef 
*****************************************************************************/

/* 
 * Supported languages 
 * MMI_virtual_keyboard_language_map[] and gui_dead_key_symbol_table[] should be changed
 * accrording to gui_virtual_keyboard_language_enum.
 */
typedef enum
{
    GUI_VIRTUAL_KEYBOARD_ENGLISH = 0,
    GUI_VIRTUAL_KEYBOARD_ENGLISH_UPPERCASE,
    GUI_VIRTUAL_KEYBOARD_SYMBOL,
    GUI_VIRTUAL_KEYBOARD_TRAY,
    GUI_VIRTUAL_KEYBOARD_NUMBER,
    GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER,
    GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY,
    GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY,
    GUI_VIRTUAL_KEYBOARD_NUMBER_DOT_TRAY,
    /* Chinese */
    // #ifdef __MMI_LANG_SM_CHINESE__
    GUI_VIRTUAL_KEYBOARD_PINYIN,
    //#endif
    #ifdef __MMI_TR_CHINESE_VK__
    GUI_VIRTUAL_KEYBOARD_BOPOMO,
    #endif
    //#if defined(__MMI_LANG_TR_CHINESE__) || defined(__MMI_LANG_SM_CHINESE__)
    GUI_VIRTUAL_KEYBOARD_CHINESE_SYMBOL,
    // #endif
#ifdef __MMI_THAI_VK__
	GUI_VIRTUAL_KEYBOARD_THAI,
	GUI_VIRTUAL_KEYBOARD_THAI_SYMBOL,
#endif 
#ifdef __MMI_INDONESIAN_VK__
	GUI_VIRTUAL_KEYBOARD_INDONESIAN,
	GUI_VIRTUAL_KEYBOARD_INDONESIAN_UPPERCASE,
#endif 
#ifdef __MMI_MALAY_VK__
	GUI_VIRTUAL_KEYBOARD_MALAY,
	GUI_VIRTUAL_KEYBOARD_MALAY_UPPERCASE,
#endif 
#ifdef __MMI_VIETNAMESE_VK__
	GUI_VIRTUAL_KEYBOARD_VIETNAMESE,
	GUI_VIRTUAL_KEYBOARD_VIETNAMESE_UPPERCASE,
#endif 
    /* Hindi */
#ifdef __MMI_HINDI_VK__
	GUI_VIRTUAL_KEYBOARD_HINDI,
	GUI_VIRTUAL_KEYBOARD_HINDI_SYMBOLS,
#endif 
    /* Arabic */
#ifdef __MMI_ARABIC_VK__
    GUI_VIRTUAL_KEYBOARD_ARABIC,
    GUI_VIRTUAL_KEYBOARD_ARABIC_SYMBOLS, 
    GUI_VIRTUAL_KEYBOARD_ARABIC_NUMERIC, 
#endif 
    /* Europe */
#ifdef __MMI_SPANISH_VK__
    GUI_VIRTUAL_KEYBOARD_SPANISH,
    GUI_VIRTUAL_KEYBOARD_SPANISH_UPPERCASE,
#endif /* __MMI_SPANISH_VK__ */ 
#ifdef __MMI_FRENCH_VK__
    GUI_VIRTUAL_KEYBOARD_FRENCH,
    GUI_VIRTUAL_KEYBOARD_FRENCH_UPPERCASE,
#endif /* __MMI_FRENCH_VK__ */ 
#ifdef __MMI_GERMAN_VK__
    GUI_VIRTUAL_KEYBOARD_GERMAN,
    GUI_VIRTUAL_KEYBOARD_GERMAN_UPPERCASE,
#endif /* __MMI_GERMAN_VK__ */ 
#ifdef __MMI_ITALIAN_VK__
    GUI_VIRTUAL_KEYBOARD_ITALIAN,
    GUI_VIRTUAL_KEYBOARD_ITALIAN_UPPERCASE,
#endif /* __MMI_ITALIAN_VK__ */ 
#ifdef __MMI_RUSSIAN_VK__
    GUI_VIRTUAL_KEYBOARD_RUSSIAN,
    GUI_VIRTUAL_KEYBOARD_RUSSIAN_UPPERCASE,
#endif /* __MMI_RUSSIAN_VK__ */ 
#ifdef __MMI_TURKISH_VK__
    GUI_VIRTUAL_KEYBOARD_TURKISH,
    GUI_VIRTUAL_KEYBOARD_TURKISH_UPPERCASE,
#endif /* __MMI_TURKISH_VK__ */ 
#ifdef __MMI_PORTUGUESE_VK__
    GUI_VIRTUAL_KEYBOARD_PORTUGUESE,
    GUI_VIRTUAL_KEYBOARD_PORTUGUESE_UPPERCASE,
#endif /* __MMI_PORTUGUESE_VK__ */
#ifdef __MMI_PERSIAN_VK__
	GUI_VIRTUAL_KEYBOARD_FARSI,
	GUI_VIRTUAL_KEYBOARD_FARSI_SYMBOL,
#endif
#ifdef __MMI_VKBD_EUROPEAN_SUPPORT__
    GUI_VIRTUAL_KEYBOARD_EUROPEAN_SYMBOLS,
#endif 

//qiff add multi-input method support
#if defined(__MMI_TAMIL_VK__)
    GUI_VIRTUAL_KEYBOARD_TAMIL_SYMBOLS,
#endif
#if defined(__MMI_BENGALI_VK__)
    GUI_VIRTUAL_KEYBOARD_BENGALI_SYMBOLS,
#endif
#if defined(__MMI_PUNJABI_VK__)
    GUI_VIRTUAL_KEYBOARD_PUNJABI_SYMBOLS,
#endif
#if defined(__MMI_TELUGU_VK__)
    GUI_VIRTUAL_KEYBOARD_TELUGU_SYMBOLS,
#endif
#if defined(__MMI_KANNADA_VK__)
    GUI_VIRTUAL_KEYBOARD_KANNADA_SYMBOLS,
#endif
#if defined(__MMI_MALAYALAM_VK__)
    GUI_VIRTUAL_KEYBOARD_MALAYALAM_SYMBOLS,
#endif
#if defined(__MMI_ORIYA_VK__)
    GUI_VIRTUAL_KEYBOARD_ORIYA_SYMBOLS,
#endif
//qiff add end
#if defined(__MMI_MARATHI_VK__)
	GUI_VIRTUAL_KEYBOARD_MARATHI_SYMBOLS,
#endif

    GUI_VIRTUAL_KEYBOARD_MAX_LANG
} gui_virtual_keyboard_language_enum;

/* Pen events */
typedef enum
{
    GUI_VKBD_PEN_NONE,
    GUI_VKBD_PEN_DISPLAY_AREA,
    GUI_VKBD_PEN_CHAR_I,
    GUI_VKBD_PEN_HIDE,
    GUI_VKBD_PEN_SHOW,
    GUI_VKBD_PEN_CAPSLOCK,
    GUI_VKBD_PEN_SPACE,
    GUI_VKBD_PEN_BAKSPACE,
    GUI_VKBD_PEN_SYMBPICKER,
    GUI_VKBD_PEN_BRACKET,
    GUI_VKBD_PEN_NEWLINE,
    GUI_VKBD_PEN_EUROSYMB,
    GUI_VKBD_PEN_DEAD_KEY,
    GUI_VKBD_PEN_MAX
} gui_virtual_keyboard_pen_enum;


/* Flags */

/* Flag - multi-block handwriting mode enabled */
#define GUI_VKBD_FLAG_MULTI_BLOCK_MODE          0x00000001

typedef struct
{
    S16 nInputLen;
    UI_character_type input_char[MMI_VKBD_MAX_DEADKEY_INPUT];
    UI_character_type output_char;
} gui_dead_key_symbol_struct;

typedef struct
{
    const gui_dead_key_symbol_struct *dead_key_symbol;
    S16 nentry;
} gui_dead_key_map_struct;

typedef struct
{
    /* Index of matrix layout tapped by pen (<0 if none) */
    S16 matrix_index;
    S16 matrix_column;
    S16 matrix_row;

    /* Index of custom key tapped by pen (<0 if none) */
    S16 custom_key_index;

    /* size of selected key */
    S16 key_width;
    S16 key_height;
    S16 key_x;
    S16 key_y;
} virtual_keyboard_selection_struct;

typedef struct _virtual_keyboard_struct
{
    S16 x, y;
    S16 width, height;
    U32 flags;
    gui_virtual_keyboard_language_enum lang_type;
    UI_virtual_keyboard_theme *keyboard_theme;
    UI_character_type disp_area_text[MMI_VKBD_MAX_DISPLAY_AREA_TEXT_LEN + 1];

    /* 
     * If `allowed_characters' is not NULL, the parameters `disabled_chars' and `disabled_symbols'
     * will be ignored. 
     * 
     * It only enables characters listed in `allowed_characters'
     * and special custom keys: GUI_VKBD_PEN_HIDE, GUI_VKBD_PEN_SHOW, 
     * GUI_VKBD_PEN_CAPSLOCK, GUI_VKBD_PEN_BAKSPACE.
     *
     * Custom key GUI_VKBD_PEN_NEWLINE and GUI_VKBD_PEN_SPACE are enabled 
     * if '\n' and ' ' are listed. 
     *
     * Custom key GUI_VKBD_PEN_SYMBPICKER is enabled if any non-alphanumeric characters is listed.
     *
     * `allowed_characters' may contain "-" for consecutive characters
     * (Ex. "a-z" for characters from a to z.)
     * If we want to accept "-", please write it as "\-" 
     * For example, "\-0-9." represents "-0123456789."
     */
    const UI_character_type *allowed_characters;

    /* Null-terminated list for disabled characters */
    UI_character_type disabled_chars[MMI_VKBD_MAX_DISABLED_KEYS + 1];
    /* Terminated with GUI_VKBD_PEN_NONE */
    gui_virtual_keyboard_pen_enum disabled_symbols[MMI_VKBD_MAX_DISABLED_KEYS + 1];

#ifdef __MMI_TOUCH_SCREEN__
    MMI_BOOL key_down_accepted;
#endif 

    /* Dead key character */
    UI_character_type dead_key_input[MMI_VKBD_MAX_DEADKEY_INPUT];

    BOOL selected_key_cached;
    virtual_keyboard_selection_struct selected_key;
    gdi_image_cache_bmp_struct selected_key_bitmap;
} virtual_keyboard;

/* Layout definition */
typedef struct
{
    S16 x, y;
    S16 n_columns, n_rows;
    S16 width, height;
    S16 key_width, key_height;
    S16 horizontal_key_gap, vertical_key_gap;
} gui_matrix_key_layout_struct;

typedef struct
{
    S16 x, y;
    S16 key_width, key_height;
} gui_custom_key_layout_struct;

typedef struct
{
    S16 width;
    S16 height;
    S16 n_matrix_key_layout;
    S16 n_custom_keys;
    const gui_matrix_key_layout_struct *matrix_layout;
    const gui_custom_key_layout_struct *custom_keys;
    U16 ImageId;
} gui_keyboard_layout_struct;

typedef struct
{
    BOOL enable_dead_key;   /* Dead key for european languages */
    S16 n_matrix_layout;
    S16 n_custom_keys;
    UI_character_type matrix_string[MMI_VKBD_MAX_ROW][MMI_VKBD_MAX_COLUMN + 1];
    UI_character_type custom_string[MMI_VKBD_MAX_CUSTOM_KEYS];
    UI_image_ID_type custom_key_image[MMI_VKBD_MAX_CUSTOM_KEYS];
    gui_virtual_keyboard_pen_enum custom_key_type[MMI_VKBD_MAX_CUSTOM_KEYS];
} gui_keyboard_language_struct;

typedef struct
{
    const gui_keyboard_layout_struct *virtual_keyboard_layout;
    const gui_keyboard_language_struct *virtual_keyboard_language;
} gui_keyboard_lang_map_struct;

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/

extern UI_virtual_keyboard_theme *current_virtual_keyboard_theme;

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/

extern void gui_set_virtual_keyboard_current_theme(virtual_keyboard *v);
extern void gui_set_virtual_keyboard_theme(virtual_keyboard *v, UI_virtual_keyboard_theme *t);

extern void gui_create_virtual_keyboard(
                virtual_keyboard *v,
                S16 x,
                S16 y,
                gui_virtual_keyboard_language_enum lang_type);
extern void gui_move_virtual_keyboard(virtual_keyboard *v, S16 x, S16 y);
extern void gui_show_virtual_keyboard(virtual_keyboard *v);

extern void gui_set_virtual_keyboard_allowed_characters(virtual_keyboard *v, const UI_character_type *allowed_chars);
extern void gui_set_virtual_board_disable_list(
                virtual_keyboard *v,
                const UI_character_type *disabled_chars,
                const gui_virtual_keyboard_pen_enum *disabled_symbols);

extern void gui_set_virtual_keyboard_display_area(virtual_keyboard *v, const U8 *string, BOOL redraw);

extern void gui_set_virtual_keyboard_multi_block_mode(virtual_keyboard *v, MMI_BOOL enabled);

#ifdef __MMI_TOUCH_SCREEN__
extern BOOL gui_virtual_keyboard_translate_pen_event(
                virtual_keyboard *v,
                S16 x,
                S16 y,
                mmi_pen_event_type_enum pen_event,
                gui_virtual_keyboard_pen_enum *vkbd_event,
                gui_pen_event_param_struct *vkbd_param);
#endif /* __MMI_TOUCH_SCREEN__ */ 

#endif /* __GUI_VIRTUAL_KEYBOARD_H__ */ 

