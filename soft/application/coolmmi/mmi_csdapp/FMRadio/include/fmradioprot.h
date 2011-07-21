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
*	FMRadioProt.h
*
* Project:
* --------
 
*
* Description:
* ------------
*   FM Radio main prototype
*
* Author:
* -------
 
*
*==============================================================================
* 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Log: FMRadioProt.h,v $
 * Revision 1.1.1.1  2006/10/24 08:16:41  huangjiwei
 * BASELINE VERSION
 *
 *
 *------------------------------------------------------------------------------
 
*==============================================================================
*******************************************************************************/
#ifndef _FM_RADIO_PROT_H
#define _FM_RADIO_PROT_H

#include "mmi_features.h"
#include "mmi_trace.h"
#ifdef __MMI_FM_RADIO__
/*
 * Need to include:
 * (1) xxx.h
 * (2) yyy.h
 */


void mmi_fmrdo_set_subLCD_shown( BOOL is_shown );
void mmi_fmrdo_power_on_main_screen( BOOL turn_on );
BOOL mmi_fmrdo_is_power_on( void );
UI_string_type mmi_fmrdo_get_channel_name_or_freq( void );

void mmi_fmrdo_preset_list_done( U8* signal_band );

void mmi_fmrdo_start_twinkle_frequency( void );
void mmi_fmrdo_stop_twinkle_frequency( BOOL is_shown );

void mmi_fmrdo_long_press_number_key( void );
void mmi_fmrdo_release_number_key( void );

void mmi_fmrdo_twinkle_frequency_timer( void );
void mmi_fmrdo_step_down_timer( void );
void mmi_fmrdo_step_up_timer( void );
BOOL mmi_fmrdo_is_stepping_up( void );
BOOL mmi_fmrdo_is_stepping_down( void );

U16 mmi_fmrdo_get_freq_from_str( UI_string_type input_str );

void mmi_fmrdo_set_channel( S16 channel_index, U16 freq, UI_string_type name );

U8 mmi_fmrdo_get_volume( void );
void mmi_fmrdo_set_volume( U8 volume );

void mmi_fmrdo_write_settings_to_nvram( void );

void mmi_fmrdo_inc_volume( void );
void mmi_fmrdo_dec_volume( void );

void mmi_fmrdo_init_app( void );
void mmi_fmrdo_run_app( void );
void mmi_fmrdo_highlight_menu( void );

S16 mmi_fmrdo_match_channel_list( U16 frequency );

void mmi_fmrdo_press_step_down_button( void );
void mmi_fmrdo_release_step_down_button( void );

void mmi_fmrdo_press_step_up_button( void );
void mmi_fmrdo_release_step_up_button( void );

void mmi_fmrdo_press_search_button( void );
void mmi_fmrdo_release_search_button( void );

void mmi_fmrdo_press_power_button( void );
void mmi_fmrdo_release_power_button( void );
#if defined(__MMI_FM_RADIO_RECORD__)
void mmi_fmrdo_release_pause_button(void);
#endif
void mmi_fmrdo_press_setting_button( void );
void mmi_fmrdo_release_setting_button( void );

U8 mmi_fmrdo_exit_root( void *param_p );
void mmi_fmrdo_entry_main( void );
void mmi_fmrdo_exit_main( void );

void mmi_fmrdo_entry_main_option( void );
void mmi_fmrdo_exit_main_option( void );
void mmi_fmrdo_main_option_highlight_hdlr( S32 index );
void mmi_fmrdo_main_option_LSK( void );

pBOOL mmi_fmrdo_channel_list_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask );
S32 mmi_fmrdo_channel_list_item_hint( S32 item_index, UI_string_type *hint_array );
void mmi_fmrdo_entry_channel_list( void );
void mmi_fmrdo_exit_channel_list( void );
void mmi_fmrdo_channel_list_highlight_hdlr( S32 index );

void mmi_fmrdo_entry_manual_input( void );
void mmi_fmrdo_exit_manual_input( void );
void mmi_fmrdo_manual_input_highlight_hdlr( S32 index );
void mmi_fmrdo_manual_input_LSK( void );

void mmi_fmrdo_entry_preset_list_confirm( void );
void mmi_fmrdo_exit_preset_list_confirm( void );
void mmi_fmrdo_preset_list_confirm_LSK( void );

void mmi_fmrdo_entry_settings( void );
void mmi_fmrdo_exit_settings( void );
void mmi_fmrdo_settings_highlight_hdlr( S32 index );
void mmi_fmrdo_settings_LSK( void );

void mmi_fmrdo_entry_channel_option( void );
void mmi_fmrdo_exit_channel_option( void );
void mmi_fmrdo_channel_option_highlight_hdlr( S32 index );
void mmi_fmrdo_channel_option_LSK( void );

void mmi_fmrdo_entry_searching( void );
void mmi_fmrdo_exit_searching( void );
void mmi_fmrdo_searching_abort( void );

void mmi_fmrdo_entry_channel_edit( void );
void mmi_fmrdo_exit_channel_edit( void );
void mmi_fmrdo_channel_edit_highlight_hdlr( S32 index );
void mmi_fmrdo_channel_edit_LSK( void );

void mmi_fmrdo_entry_channel_name_edit( void );

void mmi_fmrdo_entry_channel_name_edit_option( void );
void mmi_fmrdo_channel_name_edit_option_highlight_hdlr( S32 index );
void mmi_fmrdo_channel_name_edit_option_LSK( void );

void mmi_fmrdo_entry_clear_channel_confirm( void );
void mmi_fmrdo_exit_clear_channel_confirm( void );
void mmi_fmrdo_clear_channel_LSK( void );

void mmi_fmrdo_entry_save_channel_confirm( void );
void mmi_fmrdo_exit_save_channel_confirm( void );
void mmi_fmrdo_save_channel_LSK( void );
void mmi_fmrdo_save_channel_RSK( void );
BOOL mmi_fmrdo_state_is_idle(void);
#if defined(__MMI_FM_RADIO_RECORD__)
extern void mmi_fmrdo_release_record_button( void );
extern void mmi_fmrdo_release_append_button( void );
extern void mmi_fmrdo_release_stop_button( void );
extern void mmi_fmrdo_release_resume_button(void);
extern void mmi_fmrdo_select_file_callback(void* fullname, int is_short);
extern void mmi_fmrdo_send_record_req(BOOL is_append);
extern void mmi_fmrdo_entry_main_record_screen(void);
extern void mmi_fmrdo_entry_main_pause_screen(void);
extern void mmi_fmrdo_entry_restore_main_screen(void);
extern void mmi_fmrdo_record_stopped_hdlr(void);
extern void mmi_fmrdo_entry_edit_filename(void);
extern void mmi_fmrdo_entry_edit_abort_query(void);
extern void mmi_fmrdo_abort_edit_file(void);
extern void mmi_fmrdo_entry_edit_filename_option(void);
extern void mmi_fmrdo_highlight_record_save_file(void);
extern void mmi_fmrdo_highlight_record_input_method(void);
extern void mmi_fmrdo_record_save_file(void);
extern void mmi_fmrdo_entry_record_list(void);
extern void mmi_fmrdo_browse_file_callback(void* fullname, int is_short);
extern void mmi_fmrdo_record_error_hdlr(S32 cause);

extern void mmi_fmrdo_util_get_storage_path(UI_string_type path, U8 storage);
extern void mmi_fmrdo_util_check_storage_folder(UI_string_type path);
extern void mmi_fmrdo_util_append_file_ext(UI_string_type filename, U8 format);
extern void mmi_fmrdo_util_remove_file_ext(UI_string_type filename);
extern BOOL mmi_fmrdo_util_check_file_exist(UI_string_type filename);
extern void mmi_fmrdo_util_increase_file_name(UI_string_type filename);
extern void mmi_fmrdo_util_get_new_file(UI_string_type fullname);
extern UI_string_type mmi_fmrdo_util_get_file_name(UI_string_type fullname);
extern void mmi_fmrdo_util_init_duration(void);
extern void mmi_fmrdo_util_start_duration(void);
extern void mmi_fmrdo_util_update_duration(void);
extern void mmi_fmrdo_util_stop_duration(void);
extern void mmi_fmrdo_go_back_2_history(void) ;
#endif


// FMRadioMsgHdlr.c
BOOL mmi_fmrdo_continued( S32 result );
void mmi_fmrdo_preempted( S32 result );
void mmi_fmrdo_goto_channel( U16 channel_index );
void mmi_fmrdo_set_frequency( U16 freq );
void mmi_fmrdo_power_on_before_preset_auto_search( void );
void mmi_fmrdo_power_on( BOOL turn_on );
void mmi_fmrdo_search_frequency_start( U16 start_freq, BOOL is_step_up );
void mmi_fmrdo_search_frequency( BOOL is_valid );
void mmi_fmrdo_search_frequency_stop( void );
void mmi_fmrdo_search_all_channels_start( void (*callback_func)(U8* signal_band) );
void mmi_fmrdo_search_all_channels( U8 signal_level );
void mmi_fmrdo_search_all_channels_stop( void );

#if defined(__MMI_FM_RADIO_RECORD__)
extern void mmi_fmrdo_start_record(UI_string_type filename, U8 format);
extern void mmi_fmrdo_stop_record(void);
extern void mmi_fmrdo_stop_record_on_exit(void);
extern void mmi_fmrdo_pause_record(void);
extern void mmi_fmrdo_resume_record(void);
#endif

#endif // __MMI_FM_RADIO__
#endif // _FM_RADIO_PROT_H


