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
 *  wgui_inputs.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  Functions for editor.
 *  Author:
 * -------
 
 
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**********************************************************************************
   Filename:      wgui_inputs.h
   Date Created:  August-07-2002
   Contains:      UI wrapper routines header for Input objects
**********************************************************************************/

#ifndef __WGUI_INPUTS_H__
#define __WGUI_INPUTS_H__

#include "wgui.h"
#include "gui_inputs.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */ 

#ifdef __MMI_TOUCH_SCREEN__
#include "kal_non_specific_general_types.h"
#include "touchscreengprot.h"
#endif /* __MMI_TOUCH_SCREEN__ */ 

    /* multitap object wrappers            */

    extern S32 MMI_multitap_x;
    extern S32 MMI_multitap_y;
    extern S32 MMI_multitap_width;
    extern S32 MMI_multitap_height;
    extern S32 MMI_singleline_inputbox_x;
    extern S32 MMI_singleline_inputbox_y;
    extern S32 MMI_singleline_inputbox_width;
    extern S32 MMI_singleline_inputbox_height;
    extern S32 MMI_multiline_inputbox_x;
    extern S32 MMI_multiline_inputbox_y;
    extern S32 MMI_multiline_inputbox_width;
    extern S32 MMI_multiline_inputbox_height;
    extern S32 MMI_general_inputbox_x;
    extern S32 MMI_general_inputbox_y;
    extern S32 MMI_general_inputbox_width;
    extern S32 MMI_general_inputbox_height;

     

     
#ifdef __MMI_MULTITAP_THAI__
#define MAX_MULTITAPS_THAI       12
#endif 
#if defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
#define MAX_MULTITAPS         12
#else 
#define MAX_MULTITAPS         10
#endif 

#define  UI_MULTI_LINE_INIT_SCROLL           2000
#define  UI_MULTI_LINE_SCROLL_TIMEOUT        500

#if defined(__UI_PAGING_IN_MULTILINE_EDITOR__)

#define WGUI_PAGING_SCROLL_LINE_BY_LINE         0
#define WGUI_PAGING_SCROLL_PAGE_BY_PAGE         1

#define WGUI_PAGING_AUTO_SCROLL_ON           1
#define WGUI_PAGING_AUTO_SCROLL_OFF          0

#define COMPLETE_PERCENTAGE_SCALE            100

#define WGUI_MAX_SCROLL_TIME              8
    typedef struct
    {
        U32 end_id;
        U32 start_id;
        U32 doc_size;
 
        S32 last_text_offset_y;  
  
        S16 jump_offset_from_app;
        S16 text_offset;
        U8 scroll_speed;
        U8 scroll_type;
        U8 scroll_flag;
        U8 font_size;
        S8 search_str_len;
        S8 font_style;
        pBOOL jump_to_end;
        pBOOL highlight_string;
        pBOOL search_flag_is_on;
        U8 last_showed_percentage;
        U8 fixed_view_flag;
        U8 override_direction;
        get_page_callback get_page_cb;
#ifdef __MMI_TOUCH_SCREEN__
        void (*key_callback)(UI_character_type c);
   
        void (*get_data_during_scrolling)(S32 page_id,PU8 buffer);
    
#endif
    } gui_page_info_app;

    typedef enum
    {
        FONT_SIZE_SMALL,
        FONT_SIZE_MEDIUM,
        FONT_SIZE_LARGE
    } WGUI_FONT_SIZE;
extern S32 wgui_paging_multiline_get_visible_count(void);
#endif /* defined(__UI_PAGING_IN_MULTILINE_EDITOR__) */ 
     
    // #define ENABLE_MULTILINE_INPUTBOX_FRAME_SKIPPING             1
     

    extern multitap_input MMI_uppercase_multitaps[];
    extern multitap_input MMI_lowercase_multitaps[];
     
    extern multitap_input MMI_bpmf_multitaps[];
    /* CSD end */

    extern U8 MMI_current_multitap_mode;
    extern S32 MMI_active_multitap;

#ifdef __MMI_TOUCH_DIAL_SCREEN__
    extern dialing_keypad_struct g_dialing_keypad;
#endif 

    extern void (*hide_multitap) (void);

    extern void show_multitap(void);
    extern void disable_active_multitap(void);
    extern void disable_active_multitap_no_draw(void);
    extern void change_multitap_state(S32 i);
    extern void multitap_input_key_down(S32 key);
    extern void multitap_input_key_up(S32 key);
    extern void create_multitaps(void);
    void set_multitap_functions(void (*input_callback) (UI_character_type), void (*input_complete_callback) (void));
    extern void change_multitap_mode(U8 mode);

     
#ifdef __MMI_MULTITAP_THAI__
    /* void change_inputbox_type_multitapthai(byte type); */
    extern U8 Get_inputbox_update_states(void);
#endif /* __MMI_MULTITAP_THAI__ */ 
    extern void register_multitap_key_handlers(void);
    extern void register_multitap_no_draw_key_handlers(void);
    extern void clear_multitap_key_handlers(void);
    void register_hide_multitap(void (*f) (void));
    extern void reset_multitaps(void);
    extern void set_MMI_multitap_theme(void);
    extern void set_numberless_multitap(void);
    extern void set_normal_multitap(void);
     
    extern void set_english_only_multitap(void);
#ifdef __MMI_TOUCH_SCREEN__
    extern BOOL wgui_multitap_translate_and_handle_pen_event(S32 x, S32 y, mmi_pen_event_type_enum pen_event);
    extern void register_multiline_inputbox_hilite_pen_event_handler(void);
#endif 

    /* single line input box wrappers         */

    extern single_line_input_box MMI_singleline_inputbox;
    extern void (*singleline_inputbox_input_callback) (void);
    extern void (*singleline_inputbox_navigate_callback) (void);

    extern void create_singleline_inputbox(UI_string_type buffer, S32 buffer_length);
    extern void create_singleline_inputbox_set_buffer(
                    UI_string_type buffer,
                    S32 buffer_length,
                    S32 text_length,
                    S32 edit_position);
    extern void show_singleline_inputbox(void);
    extern void redraw_singleline_inputbox(void);
    extern void singleline_inputbox_multitap_input(UI_character_type c);
    extern void singleline_inputbox_multitap_input_complete(void);
    extern void singleline_inputbox_direct_input(UI_character_type c);
    extern void singleline_inputbox_delete_character(void);
    extern void singleline_inputbox_previous_character(void);
    extern void singleline_inputbox_next_character(void);
    extern void singleline_inputbox_toggle_insert_mode(void);
    extern void singleline_inputbox_delete_current_character(void);
    extern void singleline_inputbox_delete_all_characters(void);
    extern void register_singleline_inputbox_keys(void);
    extern void change_singleline_inputbox_mode(U8 mode);
     
    extern void change_qsearch_singleline_inputbox_mode(U8 mode);
    /* CSD end */
    extern void set_singleline_inputbox_mask(U8 m);
    extern void reset_singleline_inputbox(void);
    extern void move_singleline_inputbox(S32 x, S32 y);
    extern void resize_singleline_inputbox(S32 width, S32 height);
    extern void singleline_inputbox_numeric_keyboard_input_handler(S32 keyc);
    extern void singleline_inputbox_alphabetic_keyboard_input_handler(S32 keyc);
    extern void singleline_inputbox_handle_key_down(MMI_key_code_type k);
     
    extern void singleline_inputbox_handle_stroke_key_down(MMI_key_code_type k);
    extern void singleline_inputbox_handle_numeric_key_down(MMI_key_code_type k);
    /* CSD end */
    void register_singleline_inputbox_input_callback(void (*f) (void));
    void register_singleline_inputbox_navigate_callback(void (*f) (void));

    /* multi line input box wrappers       */

    extern multi_line_input_box MMI_multiline_inputbox;

    extern void create_multiline_inputbox(UI_string_type buffer, S32 buffer_length);
    extern void create_multiline_inputbox_set_buffer(
                    UI_string_type buffer,
                    S32 buffer_length,
                    S32 text_length,
                    S32 edit_position);
#ifdef __UI_PAGING_IN_MULTILINE_EDITOR__
    extern void create_multiline_inputbox_set_buffer_full_screen(
                    UI_string_type buffer,
                    S32 buffer_length,
                    S32 text_length,
                    S32 edit_position);
#endif /* __UI_PAGING_IN_MULTILINE_EDITOR__ */ 
    extern void resize_multiline_inputbox_fit(S32 width, S32 height, S32 *fit_width, S32 *fit_height);
    extern void resize_multiline_inputbox(S32 width, S32 height);
    extern void move_multiline_inputbox(S32 x, S32 y);
    extern void show_multiline_inputbox(void);
    extern void show_multiline_inputbox_no_draw(void);
    extern void show_multiline_inputbox_no_draw_new(void);
    extern void redraw_multiline_inputbox(void);
    extern void multiline_inputbox_multitap_input(UI_character_type c);
    extern void multiline_inputbox_multitap_input_complete(void);
    extern void multiline_inputbox_direct_input(UI_character_type c);
    extern void multiline_inputbox_delete_character(void);
    extern void multiline_inputbox_previous_character(void);
    extern void multiline_inputbox_next_character(void);
    extern void multiline_inputbox_delete_all_characters(void);
    extern void multiline_inputbox_previous_line(void);
    extern void multiline_inputbox_next_line(void);
    extern void multiline_inputbox_home(void);
    extern void multiline_inputbox_end(void);
    extern void multiline_inputbox_previous_page(void);
    extern void multiline_inputbox_next_page(void);

#if defined(__UI_PAGING_IN_MULTILINE_EDITOR__)
    extern void wgui_paging_multiline_inputbox_calculate_percentage(PU8 percentage_scale);
    extern void wgui_paging_multiline_inputbox_previous_page(void);
    extern void wgui_paging_multiline_inputbox_next_page(void);
    extern void wgui_paging_multiline_set_percentage(U32 page_id, S16 text_offset);
    extern void wgui_paging_multiline_inputbox_set_percentage_scale(U8 percentage_scale);

#ifdef __MMI_TOUCH_SCREEN__
	extern void wgui_paging_multiline_key_maaping_TS(UI_character_type c);
	extern void wgui_paging_multiline_inputbox_set_percentage_scale_TS(U8 percentage_scale);
	extern void wgui_paging_get_data_during_scrolling(S32 percentage_val);
	extern void wgui_paging_set_page_info(gui_page_info_app *page_info_d);
    extern void wgui_paging_set_full_screen_flag(pBOOL flag_value);
    extern MMI_BOOL wgui_paging_pen_move_hdlr(mmi_pen_point_struct pos);
    extern MMI_BOOL wgui_paging_pen_up_hdlr(mmi_pen_point_struct pos);
    extern MMI_BOOL wgui_paging_pen_down_hdlr(mmi_pen_point_struct pos);
    extern void wgui_paging_virtual_keypad_callback(void);
#endif 

    extern void wgui_paging_set_show_percentage_flag(void);
    extern void wgui_paging_reset_show_percentage_flag(void);
    extern void wgui_paging_jump_to_end(void);
    extern void wgui_paging_multiline_set_page(U32 start_id, U32 end_id);
    extern void wgui_paging_multiline_inputbox_previous_line_in_paging(void);
    extern void wgui_paging_multiline_inputbox_next_line_in_paging(void);
    extern void wgui_paging_multiline_inputbox_next_line_in_paging_scroll(void);
    extern void wgui_paging_stop_text_scroll(void);
    extern void wgui_paging_start_text_scroll(void);
    extern void wgui_paging_multiline_start_text_scroll(void);
    extern void wgui_paging_multiline_set_font(U8 gui_paging_font_type, U8 gui_paging_font_style);
    extern void wgui_paging_set_scrollbar_range(U32 document_size);
    extern void wgui_paging_multiline_set_scroll_info(gui_page_info_app **gui_scroll_info);
    extern void wgui_dm_set_multiline_data(dm_coordinates *gui_multiline_get_coordinates);
    extern void wgui_paging_set_data_for_find_next(gui_page_info_app *gui_multiline_data, U8 *buffer_data);
    extern void wgui_paging_highlight_search_string(S32 text_offset, S8 search_str_len);
    extern void wgui_paging_multiline_get_current_page(
                    PU32 page_id,
                    PS32 text_offset,
                    PU8 shared_bytes,
                    PU8 last_showed_percentage,
					     PS32 last_text_offset_y);
    extern void gui_paging_reset_auto_scroll(void);
    extern void wgui_paging_multiline_set_get_page_callback(get_page_callback get_page);
#endif /* defined(__UI_PAGING_IN_MULTILINE_EDITOR__) */ 



    extern void multiline_inputbox_toggle_insert_mode(void);
    extern void multiline_inputbox_delete_current_character(void);
    extern void change_multiline_inputbox_mode(U8 mode);
    extern void register_multiline_inputbox_keys(void);
    extern void set_multiline_inputbox_mask(U8 m);
    extern void reset_multiline_inputbox(void);
    extern void reset_multiline_inputbox_vertical_scroll(void);
    void register_multiline_inputbox_input_callback(void (*f) (void));
    void register_multiline_inputbox_navigate_callback(void (*f) (void));
    extern void register_multiline_inputbox_viewer_keys(void);
    extern void register_multiline_sms_chat_inputbox_keys(void);
    extern void (*wgui_register_multiline_inputbox_up_down_key_handler) (void);
    void wgui_set_multiline_inputbox_register_up_down_key_funcptr(void (*pfunc) (void));
    extern void wgui_reset_multiline_inputbox_register_up_down_key_funcptr(void);

    extern U8 multiline_inputbox_empty(void);
    extern S32 multiline_inputbox_length(void);
    extern S32 get_multiline_inputbox_height(void);
    extern MMI_BOOL get_multiline_inputbox_display_status(void);
    extern S32 get_multiline_inputbox_line_height(void);
    extern S32 get_multiline_inputbox_line_height_with_obj(multi_line_input_box *b);

    extern void (*multiline_inputbox_input_callback) (void);

    typedef struct
    {
        /* Try to reduce overheads of GetImage() and gui_measure_image() */
        PU8 icon;
        U8 icon_width;
        U8 icon_height;
        /* Character sequence to represent an icon. English only. */
        char string[MMI_MULTILINE_ICON_MAX_STRING_LEN + 1];
    } wgui_multiline_icon_struct;

    extern void wgui_config_general_multiline_icon_handler(
                    BOOL is_sorted,
                    U8 num_items,
                    const wgui_multiline_icon_struct *items);
    extern void wgui_reset_general_multiline_icon_handler(void);
    extern S32 wgui_general_multiline_icon_handler(
                const U8 *s,
                BOOL do_draw,
                S32 *icon_width,
                S32 *icon_height,
                S32 x,
                S32 y);

    /* API to read/write color editor buffer (UI_MULTI_LINE_INPUT_BOX_RICHTEXT)  */

    typedef struct
    {
        U8 *buffer;
        S32 head;       /* current read position (in byte) */
        S32 byte_count; /* byte count excluding '\0' */
    } wgui_color_buffer_reader_struct;

    typedef struct
    {
        U8 *buffer;
        S32 tail;   /* current write position (in byte) */
        S32 size;   /* buffer size */
    } wgui_color_buffer_writer_struct;

    extern void wgui_color_buffer_create_reader(wgui_color_buffer_reader_struct *reader, U8 *buffer);

    extern BOOL wgui_color_buffer_read_stripped_text(
                    wgui_color_buffer_reader_struct *reader,
                    U8 *dest_buffer,
                    S32 size_of_dest,
                    S32 *bytes_written);

    extern void wgui_color_buffer_create_writer(wgui_color_buffer_writer_struct *writer, U8 *buffer, S32 size);

    extern S32 wgui_color_buffer_get_space_for_write(wgui_color_buffer_writer_struct *writer);

    extern BOOL wgui_color_buffer_write_char(wgui_color_buffer_writer_struct *writer, U16 ch);

    extern BOOL wgui_color_buffer_write_2char(wgui_color_buffer_writer_struct *writer, U16 ch1, U16 ch2);

    extern BOOL wgui_color_buffer_write_3char(wgui_color_buffer_writer_struct *writer, U16 ch1, U16 ch2, U16 ch3);

    extern BOOL wgui_color_buffer_write_4char(
                    wgui_color_buffer_writer_struct *writer,
                    U16 ch1,
                    U16 ch2,
                    U16 ch3,
                    U16 ch4);

    extern S32 wgui_color_buffer_write_string(
                wgui_color_buffer_writer_struct *writer,
                const U8 *string,
                BOOL allow_partial_write,
                BOOL *whole_string_written);

    /* Dialer input box wrappers        */

    extern dialer_input_box MMI_dialer_inputbox;
    extern void (*dialer_inputbox_input_callback) (void);
    extern void (*dialer_inputbox_navigate_callback) (void);

    extern void create_dialer_inputbox(UI_string_type buffer, S32 buffer_length);
    extern void create_dialer_inputbox_set_buffer(
                    UI_string_type buffer,
                    S32 buffer_length,
                    S32 text_length,
                    S32 edit_position);
    extern void show_dialer_inputbox(void);
    extern void redraw_dialer_inputbox(void);
    extern void dialer_inputbox_multitap_input(UI_character_type c);
    extern void dialer_inputbox_multitap_input_complete(void);
    extern void dialer_inputbox_direct_input(UI_character_type c);
    extern void dialer_inputbox_delete_character(void);
    extern void dialer_inputbox_previous_character(void);
    extern void dialer_inputbox_next_character(void);
    extern void dialer_inputbox_toggle_insert_mode(void);
    extern void dialer_inputbox_delete_current_character(void);
    extern void dialer_inputbox_delete_all_characters(void);
    extern void register_dialer_inputbox_keys(void);
    extern void change_dialer_inputbox_mode(U8 mode);
    extern void set_dialer_inputbox_mask(U8 m);
    extern void reset_dialer_inputbox(void);
    extern void move_dialer_inputbox(S32 x, S32 y);
    extern void dialer_inputbox_numeric_keyboard_input_handler(S32 keyc);
    extern void dialer_inputbox_alphabetic_keyboard_input_handler(S32 keyc);
    extern void dialer_inputbox_handle_key_down(MMI_key_code_type k);
    void register_dialer_inputbox_input_callback(void (*f) (void));
    void register_dialer_inputbox_navigate_callback(void (*f) (void));
    extern void resize_dialer_inputbox(S32 width, S32 height);
    extern void register_multiline_sms_chat_inputbox_keys(void);
    extern void register_inscreen_multiline_inputbox_keys(void);
    extern void multiline_inscreen_inputbox_previous_line(void);
    extern void multiline_inscreen_inputbox_next_line(void);
 
    extern void create_multiline_inputbox_set_buffer_no_text_reset(UI_string_type, S32, S32, S32);
#ifdef  __FT_IDLE_SMART_DIAL__ 
	extern void dialer_inputbox_show_fixed_list(U8* pString,U32 len);
	extern U16 IsSmartDialStyle(void);
#endif

#ifdef __cplusplus
}
#endif 
#if defined(__MMI_FULL_KEYPAD__)

#if defined(__MMI_MULTITAP_PORTUGUESE__)
#define MAX_DISTRIBUTION_KEY_NUM_PORTUGUESE	27
#endif

#if defined(__MMI_MULTITAP_ARABIC__)
#define MAX_DISTRIBUTION_KEY_NUM_ARABIC	27
#endif

#if defined(__MMI_MULTITAP_FRENCH__)
#define MAX_DISTRIBUTION_KEY_NUM_FRENCH		27
#endif

#if defined(__MMI_MULTITAP_INDONESIAN__)
#define MAX_DISTRIBUTION_KEY_NUM_INDONESIAN	27
#endif

#if defined(__MMI_MULTITAP_RUSSIAN__)
#define MAX_DISTRIBUTION_KEY_NUM_RUSSIAN	27
#endif

#if defined(__MMI_MULTITAP_BENGALI__)
#define MAX_DISTRIBUTION_KEY_NUM_BENGALI	27
#endif

#endif
#endif /* __WGUI_INPUTS_H__ */ 

