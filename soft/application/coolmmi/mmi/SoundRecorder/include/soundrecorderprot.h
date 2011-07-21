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
 * SoundRecorderProt.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is for sound recorder.
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _SOUND_RECORDER_PROT_H
#define _SOUND_RECORDER_PROT_H
#include "mmi_data_types.h"

#ifdef __MMI_SOUND_RECORDER__
void populateSoundRecorderMenu(void);
void InitSoundRecorderApp(void);
void mmi_sndrec_entry_in_idle(void);
void mmi_sndrec_entry_in_call(void);
void mmi_sndrec_entry_main(void);
void mmi_sndrec_hightlight_main(void);
#ifdef __MMI_SOUND_RECORDER_STYLE_X__
void mmi_sndrec_highlight_list(void);
void mmi_sndrec_entry_style_x_main(void);
#endif
void mmi_sndrec_create_sound_folder(void);
BOOL mmi_sndrec_check_dest_folder(U8 storage);
void mmi_sndrec_get_new_file_name(UI_string_type filename);
void mmi_sndrec_get_new_file(UI_string_type filename, UI_string_type fullname);
void mmi_sndrec_init_file_list(void);
BOOL mmi_sndrec_is_file_exist(void);

void mmi_sndrec_entry_option(void);

void mmi_sndrec_exit_hdlr(void);
void mmi_sndrec_set_interrupt_hdlr(void);

void mmi_sndrec_entry_record(void);
void mmi_sndrec_hightlight_record(void);
void mmi_sndrec_entry_record_paused(void);
void mmi_sndrec_record_stopped_hdlr(void);
void mmi_sndrec_entry_edit_name(void);
void mmi_sndrec_entry_abort_query(void);
void mmi_sndrec_abort_file(void);
void mmi_sndrec_send_record_req(void);
 
void mmi_sndrec_entry_play(void);
void mmi_sndrec_hightlight_play(void);
void mmi_sndrec_entry_play_paused(void);
void mmi_sndrec_send_play_req(void);

void mmi_sndrec_entry_append(void);
void mmi_sndrec_hightlight_append(void);
void mmi_sndrec_send_append_req(void);

void mmi_sndrec_entry_rename_option(void);
void mmi_sndrec_entry_save(void);
void mmi_sndrec_hightlight_save(void);
void mmi_sndrec_hightlight_input_method(void);

void mmi_sndrec_hightlight_delete(void);
void mmi_sndrec_hightlight_delete_all(void);
void mmi_sndrec_hightlight_settings(void);

void mmi_sndrec_entry_inline_edit(void); 
void mmi_sndrec_exit_inline_edit(void); 
void mmi_sndrec_inline_edit_hightlight_hdlr(S32 index); 

void mmi_sndrec_init_settings(void);
void mmi_sndrec_settings_pre_save(void);
void mmi_sndrec_entry_settings_save_query(void);
void mmi_sndrec_exit_settings_save_query(void);
void mmi_sndrec_get_settings(void);

void mmi_sndrec_set_sidekey_handler(void);
void mmi_sndrec_error_hdlr(S32 cause);
void mmi_sndrec_delete_name_editor_history_nodes(void);
void mmi_sndrec_delete_history_nodes(U16 screenId);
void mmi_sndrec_go_back_2_history(void); 
 
/* Msg Handler */
UI_string_ID_type mmi_sndrec_get_res_string(U8 result);
U32 mmi_sndrec_get_duration_ticks( void );
void mmi_sndrec_init_duration( void );
void mmi_sndrec_start_duration( void );
void mmi_sndrec_update_duration( void );
void mmi_sndrec_start_record(UI_string_type filename, U8 format);
void mmi_sndrec_start_play(UI_string_type filename);
void mmi_sndrec_stop_play(void);
void mmi_sndrec_stop_record(void);
void mmi_sndrec_pause(void);
void mmi_sndrec_resume(void);
void mmi_sndrec_set_media_volume(U8 volume);
void mmi_sndrec_set_speech_volume(U8 volume);

typedef enum
{
	Rec_stub_0=0x2A40,
	Rec_stub_1,	
	Rec_stub_2,	
	Rec_stub_3,
	Rec_stub_4,
	Rec_stub_5,
	Rec_stub_6,
	Rec_stub_7,
	Rec_stub_8,
	Rec_stub_9,
	Rec_stub_10,
	Rec_stub_11,
	Rec_stub_12,
	Rec_stub_13,
	Rec_stub_14,
	Rec_stub_15,
	Rec_stub_16,
	Rec_stub_17,
	Rec_stub_18,
	Rec_stub_19,
	Rec_stub_20,
	Rec_stub_21,
	Rec_stub_22,
	Rec_stub_23,
	Rec_stub_24,
	Rec_stub_25,
	Rec_stub_26,
	Rec_stub_27,
	Rec_stub_28,
	Rec_stub_29,
	Rec_stub_30,
	Rec_stub_end =0x2A5F
}stub_recorder;

#endif /* __MMI_SOUND_RECORDER__ */

#endif /* _SOUND_RECORDER_PROT_H */


