/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2002
*
*******************************************************************************/

/*******************************************************************************
* Filename:
* ---------
*	ABRepeaterProt.h
*
* Project:
* --------
 
*
* Description:
* ------------
*   AB Repeater proto-type
*
* Author:
* -------
 
*
*********************************************************************************************/

#ifndef _AB_REPEATER_PROT_H
#define _AB_REPEATER_PROT_H

/*
 * Need to include:
 * (1) mmi_data_types.h
 * (2) mdi_datatype.h
 */


#ifdef __MMI_AB_REPEATER__
U8 mmi_abrepeat_get_preferred_or_available_list( void );
BOOL mmi_abrepeat_is_dir_ready( UI_string_type path );
void mmi_abrepeat_extract_filename( UI_string_type filename, UI_string_type fullfilename );
UI_string_type mmi_abrepeat_match_last_char( UI_string_type ustring1, UI_character_type uchar2 );

void InitABRepeaterApp(void);
void HighlightABRepeater(void);
void mmi_abrepeat_popup_empty_msg(void);
void mmi_abrepeat_entry_list(void);
void mmi_abrepeat_select_file_and_enter_main( void );
void mmi_abrepeat_storage_highlight_hdlr(void);
void mmi_abrepeat_option_highlight_hdlr( S32 index );
void mmi_abrepeat_inline_edit_hightlight_hdlr(S32 index);
void mmi_abrepeat_exit_storage_media(void);
void mmi_abrepeat_save_storage(void);
void mmi_abrepeat_enter_storage_media(void);
void mmi_abrepeat_enter_highlight_hdlr( void );
BOOL mmi_abrepeat_is_card_ready( void );
void mmi_abrepeat_entry_option(void);
void mmi_abrepeat_option_lsk(void);
void mmi_abrepeat_rsk_action(void);
U8 mmi_abrepeat_delete_history_hdlr( void* in_param );
void mmi_abrepeat_redraw_setting_play_screen(void);
void mmi_abrepeat_read_settings( void );
void mmi_abrepeat_filename_callback(void* fullname, int is_short);
void mmi_abrepeat_entry_main(void);
void mmi_abrepeat_exit_main(void);
void mmi_abrepeat_get_fs_info(void);
void mmi_abrepeat_setting_set_key_handlers(void);
void mmi_abrepeat_setting_press_digit_up(void);
void mmi_abrepeat_play_press_digit_down_hdlr(U16 keycode);
void mmi_abrepeat_play_press_digit_down(void);
void mmi_abrepeat_setting_press_digit_down(void);
void mmi_abrepeat_setting_press_digit_down_hdlr(U16 keycode);
void mmi_abrepeat_setting_press_digit_up_hdlr(U16 keycode);
void mmi_abrepeat_setting_long_press_digit_hdlr(U16 keycode);
void mmi_abrepeat_setting_long_press_digit(void);
void mmi_abrepeat_set_mainplay_section_key_handler(void);
void mmi_abrepeat_press_play_button_down(void);
void mmi_abrepeat_press_rewind_button_down(void);
void mmi_abrepeat_press_forward_button_down(void);
void mmi_abrepeat_press_stop_button_down(void); 
void mmi_abrepeat_press_play_button_up(void);
void mmi_abrepeat_press_rewind_button_up(void);
void mmi_abrepeat_press_forward_button_up(void);
void mmi_abrepeat_press_stop_button_up(void) ;

void mmi_abrepeat_press_inc_volume(void);
void mmi_abrepeat_press_dec_volume(void); 

void mmi_abrepeat_play_set_key_handlers(void);
void mmi_abrepeat_play_press_digit_up(void);
void mmi_abrepeat_play_press_digit_up_hdlr(U16 keycode);

void mmi_abrepeat_do_stop_action(void);
void mmi_abrepeat_stopped(void);

void mmi_abrepeat_do_pause_action(void);
void mmi_abrepeat_paused(void);
void mmi_abrepeat_play_completed(void);
void mmi_abrepeat_pause_callback_hdlr( mdi_result result );

void mmi_abrepeat_do_resume_action(void);
void mmi_abrepeat_resumed(void);
void mmi_abrepeat_resume_callback_hdlr( mdi_result result );

void mmi_abrepeat_do_play_action( void );
S32 mmi_abrepeat_do_play_action_without_handle_result(void);
void mmi_abrepeat_play_callback_hdlr( mdi_result result );
void mmi_abrepeat_played(void);
void mmi_abrepeat_timer( void );
U16 mmi_abrepeat_err_str( S32 result );
#ifdef SIMULATOR_TESTING
U32 mmi_abrepeat_dummy_get_curr_offset(void);
void dummy_file_offset(void);
#endif//SIMULATOR_TESTING


 
void mmi_abrepeat_option_back(void);

#endif//__MMI_AB_REPEATER__

#endif /* _AB_REPEATER_PROT_H */


