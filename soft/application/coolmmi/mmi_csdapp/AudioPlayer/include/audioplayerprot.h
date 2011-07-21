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
*	AudioPlayerProt.h
*
* Project:
* --------
 
*
* Description:
* ------------
*   Audio player proto-type
*
* Author:
* -------
 
*
*==============================================================================
* 				HISTORY
 
 *------------------------------------------------------------------------------
 * 
 *
 *------------------------------------------------------------------------------
 
*==============================================================================
*******************************************************************************/

#ifndef _AUDIO_PLAYER_PROT_H
#define _AUDIO_PLAYER_PROT_H
#include "mdi_datatype.h"
/*
 * Need to include: 
 * (1) mmi_data_types.h
 * (2) mdi_datatype.h
 */

#ifdef __MMI_AUDIO_PLAYER__

#ifdef __MMI_A2DP_SUPPORT__
extern void mmi_audply_bt_open_callback(S32 result);
extern void mmi_audply_read_bt_settings(void);
extern void mmi_audply_write_bt_settings(void);
extern BOOL mmi_audply_is_output_to_bt(void);
extern void mmi_audply_switch_bt_output(void);
extern void mmi_audply_sync_bt_output(void);
extern void mmi_audply_bt_stop(BOOL stop_codec, BOOL notify_opener);
#endif /*__MMI_A2DP_SUPPORT__*/
void populateAudioPlayerMenu( void );

S32 mmi_audply_lookup_audio_file_format( UI_string_type file_ext );

// return whether will audio player play or attempt to play song
BOOL mmi_audply_is_play_activated( void );
BOOL mmi_audply_is_playing( void );
void mmi_audply_set_subLCD_shown( BOOL is_shown );

void mmi_audply_clear_play_seconds( void );
void mmi_audply_start_record_play_seconds( void );
S32 mmi_audply_get_play_seconds( void );
void mmi_audply_reset_play_seconds(U32 play_seconds);
void mmi_audply_update_play_seconds( void );

// send mdi request
void mmi_audply_play(void);
void mmi_audply_do_play_action( void );
void mmi_audply_do_play_action_and_back_to_main_screen( void );
S32 mmi_audply_do_play_action_without_handle_result( void );
void mmi_audply_do_pause_action( void );
void mmi_audply_do_resume_action( void );
void mmi_audply_do_stop_action( void );
void mmi_audply_stop_playing( void );
void mmi_audply_do_before_seek_action(INT32 progress);
void mmi_audply_do_seek_action(INT32 progress);
void mmi_audply_do_seeking_action(INT32 progress);
INT32 mmi_audply_get_progress(void);
// handle mdi callback
void mmi_audply_play_callback_hdlr( mdi_result result );
void mmi_audply_pause_callback_hdlr( mdi_result result );
void mmi_audply_resume_callback_hdlr( mdi_result result );
BOOL mmi_audply_background_callback_hdlr( mdi_result result );
// update UI & states
void mmi_audply_play_completed( void );
void mmi_audply_played( void );
void mmi_audply_stopped( void );
void mmi_audply_paused( void );
void mmi_audply_resumed( void );

void mmi_audply_get_fs_info( void );
void mmi_audply_read_settings( void );
void mmi_audply_write_settings( void );

void InitAudioPlayerApp( void );
void HighlightAudioPlayer( void );

void mmi_audply_update_main( void );
void mmi_audply_entry_main( void );
void mmi_audply_exit_main( void );

void mmi_audply_entry_list( void );
void mmi_audply_exit_list( void );
void mmi_audply_list_highlight_hdlr( S32 index );
pBOOL mmi_audply_list_getitem( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask );

void mmi_audply_entry_option( void );
void mmi_audply_exit_option( void );
void mmi_audply_option_highlight_hdlr( S32 index );
void mmi_audply_option_right_arrow_key( void );
void mmi_audply_show_update_list_result( U8 result );
void mmi_audply_option_LSK( void );

void mmi_audply_entry_settings( void );
#ifdef __MMI_A2DP_SUPPORT__
extern void mmi_audply_entry_bluetooth_settings(void);
extern void mmi_audply_exit_bluetooth_settings(void);
extern void mmi_audply_bluetooth_settings_highlight_hdlr(S32 index);
extern void mmi_audply_bluetooth_settings_done(void);
extern void mmi_audply_select_bt_a2dp_headset(void);
#endif /* __MMI_A2DP_SUPPORT__ */
void mmi_audply_exit_settings( void );
void mmi_audply_settings_highlight_hdlr( S32 index );
void mmi_audply_settings_LSK( void );

void mmi_audply_entry_detail( void );
void mmi_audply_detail_highlight_hdlr( S32 index );
void mmi_audply_exit_detail( void );

void mmi_audply_entry_remove_confirm( void );
void mmi_audply_exit_remove_confirm( void );
void mmi_audply_remove_confirm_LSK( void );

void mmi_audply_entry_remove_all_confirm( void );
void mmi_audply_exit_remove_all_confirm( void );
void mmi_audply_remove_all_confirm_LSK( void );

void mmi_audply_process_add_files_in_folder( void );
void mmi_audply_process_add_files_end( void );
void mmi_audply_entry_processing_add( void );
void mmi_audply_exit_processing_add( void );
void mmi_audply_add_file_hdlr( S8* fullPath, int is_short );

void mmi_audply_timer( void );
void mmi_audply_press_stop_button_up( void );
void mmi_audply_press_play_button_up( void );
void mmi_audply_press_prev_button_up( void );
void mmi_audply_press_next_button_up( void );
void mmi_audply_press_stop_button_down( void );
void mmi_audply_press_play_button_down( void );
void mmi_audply_press_prev_button_down( void );
void mmi_audply_press_next_button_down( void );
#if defined(__PROJECT_GALLITE_C01__)
void mmi_audply_press_inc_volume_up();
void mmi_audply_press_dec_volume_up();
#endif
void mmi_audply_press_inc_volume( void );
void mmi_audply_press_dec_volume( void );
BOOL mmi_audply_save_volume( U8 volume );
U8 mmi_audply_get_volume( void );

void mmi_audply_msdc_plug_in_hdlr( BOOL has_popup );
void mmi_audply_msdc_plug_out_hdlr( BOOL has_popup );

U8 mmi_audply_delete_history_hdlr( void* in_param );
#endif /* __MMI_AUDIO_PLAYER__ */

U16 mmi_audply_err_str( S32 result );
// single play
void mmi_audply_single_play_timer( void );
void mmi_audply_single_play( UI_string_type filefullname, void (*exit_callback_func)(void) );
void mmi_audply_entry_play_animation( void );
void mmi_audply_exit_play_animation( void );
U8 mmi_audply_delete_play_animation_history_hdlr( void* param_p );
S32 mmi_audply_do_single_play_action( UI_string_type filefullname );
void mmi_audply_do_single_stop_action( void );
void mmi_audply_single_play_callback_hdlr( mdi_result result );

#if defined(__MMI_AUDIO_PLAYER_LYRIC_DISPLAY__)
extern void mmi_audply_stop_scroll_lyric_and_clean_screen(void);
extern void mmi_audply_scroll_lyric_handler(void);
extern void mmi_audply_redraw_lyric(void);
extern void mmi_audply_lyric_on_time(void);
extern void mmi_audply_lyric_check_backlight(MMI_BOOL on);
extern void mmi_audply_stop_lyric(void);
extern MMI_BOOL mmi_audply_init_lyric(void);
#endif
#endif /* _AUDIO_PLAYER_PROT_H */


