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
 *   med_struct.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes primary global variables of media task.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef MED_NOT_PRESENT

#ifndef _MED_STRUCT_H
#define _MED_STRUCT_H
#include "app_ltlcom.h"

/*==== DEFINES ========*/


#define MAX_MELODY_FILE_NAME 50

/* define for kal_wchar */
#define ENCODE_BYTE   (2)

#define MAX_VM_FILE_NAME 20
#define MAX_NUM_OF_VM_LIST 20

#define MAX_MEDIA_FILE_NAME 512
/*WeiD Add Start For 6208 Ver: TBM780  on 2007-8-17 15:1 */
#define MAX_MEDIA_ALL_FILE_NAME 520
/*WeiD Add End  For 6208 Ver: TBM780  on 2007-8-17 15:1 */

#define AUD_TTS_MAX_PROCESS_DATA_SIZE 30      /* This size shall be larger than TTS_PROCESS_DATA.*/

#ifndef __VR_MED_CONST__
#define __VR_MED_CONST__
#define VR_SD_MAX_GROUP_TAG_NUM 20
#define VR_SD_MAX_GROUP_NUM 4
#define VR_MAX_RCG_RESULT_NUM 10
#define VR_MAX_DB_PATH_LEN 80
#define VR_MAX_ADD_ONCE_NUM 20
#endif

#ifndef __BT_MED_CONST__
#define __BT_MED_CONST__
#define BT_HFP 0
#define BT_A2DP 1

#define BT_A2DP_AGGRESSIVE_MODE 0
#define BT_A2DP_PASSIVE_MODE 1
#define BT_A2DP_PTS_PASSIVE_MODE 2

#define BT_HFP_EARPHONE_MODE 0
#define BT_HFP_CARKIT_MODE 1
#endif /* __BT_MED_CONST__ */ 
/*==== MESSAGE STRUCTS ==========*/

/* common */
typedef struct {
   LOCAL_PARA_HDR
} 
med_startup_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   result;
} 
med_startup_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   location;
   void**	buffer_p;
   kal_uint32 buffer_size;  
} 
media_get_buffer_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   location;
   void**	buffer_p;
} 
media_free_buffer_req_struct;

/* audio */

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8    src_id;
} 
l4aud_get_audio_mode_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8    src_id;
   kal_uint8    mode;
} 
l4aud_get_audio_mode_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8    src_id;
   kal_uint8    mode;
} 
l4aud_set_audio_mode_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8    src_id;
   kal_bool    result;
   kal_uint16  cause;
} 
l4aud_set_audio_mode_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 gpio_device;
} 
l4aud_gpio_detect_ind_struct;

typedef struct
{
   kal_uint8   mode;
   kal_uint8   melody[7];
   kal_uint8   sound[7];
   kal_uint8   keytone[7];
   kal_uint8   speech[7];
   kal_uint8   mic[7];
   kal_uint8   sidetone;
} 
audio_profile_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   audio_profile_struct audio_profile;
} 
l4aud_set_audio_profile_req_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_bool    result;
   kal_uint16  cause;
} 
l4aud_set_audio_profile_cnf_struct;

typedef struct
{
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   mode;
} 
l4aud_get_audio_profile_req_struct;

typedef struct
{
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_bool    result;
   audio_profile_struct audio_profile;
} 
l4aud_get_audio_profile_cnf_struct;

typedef struct
{
   kal_int16   FIR_input_coeff[2][30];
   kal_int16   FIR_output_coeff[7][30];
   kal_uint16  selected_FIR_output_index;
   kal_uint16  es_time_const;
   kal_uint16  es_vol_const;
   kal_uint16  es_time_const2;
   kal_uint16  es_vol_const2;
   kal_uint16  max_swing;
   kal_int16  Speech_FIR_Input_Coeff_16k_Tbl1[62];
   kal_int16  Speech_FIR_Output_Coeff_16k_Tbl1[62];
   kal_int16  Speech_FIR_Input_Coeff_16k_Tbl2[62];
   kal_int16  Speech_FIR_Output_Coeff_16k_Tbl2[62];
   kal_int16  Melody_FIR_Output_Coeff_32k_Tbl1[45];
   kal_int16  Melody_FIR_Output_Coeff_32k_Tbl2[45];
}
audio_param_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   audio_param_struct audio_param;
} 
l4aud_set_audio_param_req_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_bool    result;
   kal_uint16  cause;
} 
l4aud_set_audio_param_cnf_struct;

typedef struct
{
   LOCAL_PARA_HDR
   kal_uint8   src_id;
} 
l4aud_get_audio_param_req_struct;

typedef struct
{
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_bool    result;
   audio_param_struct audio_param;
} 
l4aud_get_audio_param_cnf_struct;



typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   audio_type;
   kal_uint8   device;
} 
l4aud_audio_set_device_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   result;
} 
l4aud_audio_set_device_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   audio_type;
   kal_uint8   volume;
   kal_uint8   aux_volume;/* for side tone used */
   kal_uint8   mode;
   kal_bool   apply;
} 
l4aud_audio_set_volume_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   result;
} 
l4aud_audio_set_volume_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   audio_type;
   kal_uint8   volume_level;
   kal_uint8   gain;
} 
l4aud_audio_set_gain_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   result;
} 
l4aud_audio_set_gain_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   audio_id;
   kal_uint8   play_style;
   kal_uint8   volume;
   kal_uint8   output_path;
   kal_uint16  identifier;
} 
l4aud_audio_play_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   result;
} 
l4aud_audio_play_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   audio_id;
} 
l4aud_audio_stop_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   result;
} 
l4aud_audio_stop_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_wchar    file_name[MAX_MELODY_FILE_NAME];
   kal_uint8   play_style;
   kal_uint8   volume;
   kal_uint8   output_path;
   kal_uint16  identifier;
} 
l4aud_audio_play_by_name_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_audio_play_by_name_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_wchar    file_name[MAX_MELODY_FILE_NAME];
} 
l4aud_audio_stop_by_name_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_audio_stop_by_name_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   const kal_uint8 *melody;
   kal_uint32  len;
   kal_uint8   format;
   kal_uint8   play_style;
   kal_uint16  identifier;
   kal_uint8   volume;
   kal_uint8   output_path;
   kal_uint8   blocking;
} 
l4aud_audio_play_by_string_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_audio_play_by_string_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
} 
l4aud_audio_stop_by_string_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_audio_stop_by_string_cnf_struct;

/* this is the same to device_tones_struct and L1SP_Tones */
typedef struct {
   kal_uint16   freq1;            /* First frequency                                    */
   kal_uint16   freq2;            /* Second frequency                                   */
   kal_uint16   on_duration;      /* Tone on duation, in ms unit, 0 for continuous tone */
   kal_uint16   off_duration;     /* Tone off duation, in ms unit, 0 for end of playing */
   kal_int8     next_operation;   /* Index of the next tone                             */
} aud_tones_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   aud_tones_struct *tone_list;
   kal_uint8   volume;
   kal_uint16  identifier;
} 
l4aud_audio_play_tone_seq_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_audio_play_tone_seq_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint16  identifier;
} 
l4aud_audio_stop_tone_seq_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_audio_stop_tone_seq_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint16  imy_len;
   kal_uint8   play_style;
   kal_uint16  identifier;
} 
l4aud_audio_demo_imy_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   result;
} 
l4aud_audio_demo_imy_cnf_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   result;
   kal_uint16  identifier;
} 
l4aud_audio_play_finish_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   device;
   kal_bool    mute;
} 
l4aud_audio_mute_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   result;
} 
l4aud_audio_mute_cnf_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   audio_type;
   kal_bool    fir_on;
} 
l4aud_audio_set_fir_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   result;
} 
l4aud_audio_set_fir_cnf_struct;



/* speech */

typedef struct {
   LOCAL_PARA_HDR
   kal_bool    speech_on;
} 
l4aud_sp_set_mode_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   result;
} 
l4aud_sp_set_mode_cnf_struct;



/*
*  General TTS
*/

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint32 attr_id;
    kal_uint32 attr_value;
} media_aud_tts_set_attr_req_struct;

typedef struct
{
    LOCAL_PARA_HDR 
    kal_uint32 handle;
    kal_uint32 seq_no;
    kal_uint8 data[AUD_TTS_MAX_PROCESS_DATA_SIZE];
    kal_uint8 event;
}media_aud_tts_play_event_ind_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint16 *tts_string;
    kal_uint16 str_type;
    kal_uint16 lang;
    kal_uint16 app_type;
    kal_uint16 gander;
    kal_uint16 identifier;
    kal_uint8  volume;
    kal_uint8  output_path;
    kal_uint8  pitch;
    kal_uint8  speed;
} media_aud_tts_play_req_struct;

typedef struct
{
    LOCAL_PARA_HDR 
} media_aud_tts_stop_req_struct;

typedef struct
{
    LOCAL_PARA_HDR 
} media_aud_tts_pause_req_struct;

typedef struct
{
    LOCAL_PARA_HDR 
} media_aud_tts_resume_req_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 data[AUD_TTS_MAX_PROCESS_DATA_SIZE];
} media_aud_tts_update_info_ind_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_bool    bluetooth_on;
} 
l4aud_sp_set_bt_path_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   result;
} 
l4aud_sp_set_bt_path_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   mode; 
   kal_uint8   mic_volume; 
   kal_uint8   sidetone_volume;
}
l4aud_sp_set_input_mode_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   result;
} 
l4aud_sp_set_input_mode_cnf_struct;

/* voice memo */


/* informatiom struct for voice memo files */
typedef struct
{
   kal_uint16   no_of_vm;
   kal_wchar    file_name_list[MAX_NUM_OF_VM_LIST][MAX_VM_FILE_NAME];
   kal_bool    more_entry;
   kal_uint32  free_space;
   kal_uint32  reserved;
}
aud_vm_info_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   direction;
} 
l4aud_vm_record_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_vm_record_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_wchar    file_name[MAX_VM_FILE_NAME];
} 
l4aud_vm_play_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_vm_play_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
} 
l4aud_vm_stop_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
   kal_wchar    file_name[MAX_VM_FILE_NAME];
} 
l4aud_vm_stop_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_wchar    file_name[MAX_VM_FILE_NAME];
   kal_bool   delete_all;
} 
l4aud_vm_delete_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_vm_delete_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
} 
l4aud_vm_abort_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_vm_abort_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
} 
l4aud_vm_pause_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_wchar    file_name[MAX_VM_FILE_NAME];
   kal_uint8   result;
} 
l4aud_vm_pause_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
} 
l4aud_vm_resume_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_vm_resume_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_wchar    file_name[MAX_VM_FILE_NAME];
} 
l4aud_vm_append_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_vm_append_cnf_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
} 
l4aud_vm_get_info_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   aud_vm_info_struct   vm_info;
} 
l4aud_vm_get_info_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_wchar    old_file_name[MAX_VM_FILE_NAME];
   kal_wchar    new_file_name[MAX_VM_FILE_NAME];   
} 
l4aud_vm_rename_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_vm_rename_cnf_struct;

/* indication */

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_wchar    file_name[MAX_VM_FILE_NAME];
   kal_uint8   result;
} 
l4aud_vm_record_finish_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_vm_play_finish_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   event;
} 
aud_vm_read_data_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   event;
} 
aud_vm_write_data_ind_struct;



typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_wchar   file_name[MAX_MEDIA_ALL_FILE_NAME+ENCODE_BYTE];
   kal_uint8   play_style;
   kal_uint8   volume;
   kal_uint8   output_path;
   kal_uint16  identifier;
// set both start_offset and end_offset to 0 for backward compatible
   kal_uint32   start_offset; /* 0 : file begining */
   kal_uint32   end_offset;  /* 0 or file data len-1: file end */
} 
l4aud_media_play_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_media_play_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_wchar   file_name[MAX_MEDIA_FILE_NAME];
   kal_uint8   format;
    kal_uint8 quality;
   kal_bool   default_input;
   kal_uint8   input_source;
    kal_uint32 size_limit;
    kal_uint32 time_limit;    
} 
l4aud_media_record_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_media_record_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
} 
l4aud_media_stop_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_media_stop_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
} 
l4aud_media_pause_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_media_pause_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
} 
l4aud_media_resume_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_media_resume_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
} 
l4aud_media_store_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_wchar   file_name[MAX_MEDIA_FILE_NAME];
   kal_uint8   play_style;
   kal_uint8   volume;
   kal_uint8   output_path;
   kal_uint16  identifier;
// set both start_offset and end_offset to 0 for backward compatible
   kal_uint32   start_offset; /* 0 : file begining */
   kal_uint32   end_offset;  /* 0 or file data len-1: file end */
} 
l4aud_media_restore_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   vol_type;
   kal_bool    mute;
} 
media_aud_set_mute_req_struct;


/* indication */

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   event;
   kal_uint32 seq_no;
} 
aud_media_write_data_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   event;
} 
aud_media_read_data_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
} 
l4aud_media_record_finish_ind_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   result;
   kal_uint16  identifier;
} 
l4aud_media_play_finish_ind_struct;

 
typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 output_path;
} 
media_fmr_power_on_req_struct;

typedef struct {
   LOCAL_PARA_HDR
} 
media_fmr_power_off_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint16 freq;
} 
media_fmr_set_freq_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 mute;
} 
media_fmr_mute_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint16 freq;
   kal_uint8 is_step_up;
} 
media_fmr_check_is_valid_stop_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 is_valid;
} 
media_fmr_check_is_valid_stop_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint16 freq;
   kal_uint8 is_step_up;
} 
media_fmr_get_signal_level_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 signal_level;
} 
media_fmr_get_signal_level_cnf_struct;
 

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   src_id;
   kal_uint8   event;
   kal_uint32 seq_no;
} 
aud_media_play_event_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint16   mod_id;
   kal_uint8   level;
} 
media_aud_block_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 enabled;
}
media_aud_set_vibrator_enabled_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int8 magnitude[8];
}
media_aud_set_equalizer_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_wchar file_name[MAX_MEDIA_FILE_NAME];
   kal_uint32 progress;
}
media_aud_get_file_data_length_req_struct;

typedef struct {
   LOCAL_PARA_HDR
}
media_aud_get_file_data_progress_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 progress;
}
media_aud_set_file_data_progress_req_struct;

/* CAMERA */

typedef struct {
   LOCAL_PARA_HDR
   kal_uint16  seq_num;
} 
media_cam_power_up_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   delay_time;
} 
media_cam_power_down_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   result;
   kal_uint16  seq_num;
} 
media_cam_ready_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint16   image_width;
   kal_uint16   image_height;
   kal_uint16   buffer_width;
   kal_uint16   buffer_height;
   kal_uint16   preview_width;
   kal_uint16   preview_height;
   kal_uint16   preview_offset_x;
   kal_uint16   preview_offset_y;
   void*      image_buffer_p;
   kal_uint32  image_buffer_size;
   kal_int16   frame_rate;
   kal_int16   contrast;
   kal_int16   brightness;
   kal_int16   saturation;
   kal_int16   hue;
   kal_int16   gamma;
   kal_int16   WB;
   kal_int16   exposure;
   kal_int16   effect;
   kal_int16   zoom_factor;
   kal_int16   rotate;
   kal_int16   lcd_rotate;
   kal_int16   flash_mode;
   kal_uint8   snapshot_number;
   kal_bool   night_mode;
   kal_uint8   banding_freq;

   kal_uint8   lcd_id;
   kal_uint16  lcd_start_x;
   kal_uint16  lcd_start_y;
   kal_uint16  lcd_end_x;
   kal_uint16  lcd_end_y;
   kal_uint16  roi_offset_x;
   kal_uint16  roi_offset_y;
   kal_uint32  update_layer;
   kal_uint32  hw_update_layer;
   kal_uint32  src_key_color;   
   /* LCD Setting */
   kal_uint32   roi_background_color;         /* set the background color */

#if (defined(MT6228)||defined(MT6229))
   /* ISP Setting */
   kal_bool    manual_wb;                     /* KAL_TRUE or KAL_FALSE to enable or disable manual white balance */
   kal_uint8   ae_metering_mode;               /* AUTO, SPOT, CENTRAL WEIGHT, AVERAGE */
   kal_uint8   iso_setting;                  /* AUTO, 100, 200, 400 */
   kal_uint8	scene_mode;							/* NORMALSCENE, PORTRAIT, LANDSCAPE, SPORT, FLOWER, NIGHTSCENE */
   /* IPP Setting */
   kal_bool   image_pitch_mode;               /* KAL_TRUE or KAL_FALSE for block mode of decoded image */
   kal_uint8   image_data_format;            /* IMGDMA_IBW_OUTPUT_RGB565 or IMGDMA_IBW_OUTPUT_RGB888 */
   kal_uint16   image_pitch_bytes;            /* pitch width */

   kal_bool    overlay_frame_mode;            /* KAL_TRUE or KAL_FALSE to enable or disable overlay function */
   kal_uint8   overlay_color_depth;            /* The color depth of overlay frame buffer */
   kal_uint8   overlay_frame_source_key;
   kal_uint16  overlay_frame_width;            /* Image width of overlay frame buffer */
   kal_uint16  overlay_frame_height;         /* Image height of overlay frame buffer */
   kal_uint32  overlay_frame_buffer_address;   /* The starting address of overlay frame buffer */
   /* TV Setting */
   kal_uint8     tv_output_mode;
   kal_bool     tv_output;
   kal_uint16   tv_output_width;               /* image width for TV output */
   kal_uint16   tv_output_height;               /* image height for TV output */
   kal_uint32    tv_output_buffer1_address;      /* image buffer1 for TV source buffer */
   kal_uint32    tv_output_buffer2_address;      /* image buffer2 for TV source buffer */
   kal_uint32    tv_output_buffer_size;         /* image buffer size for TV source buffer */
#endif

} 
media_cam_preview_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   result;
} 
media_cam_preview_fail_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
} 
media_cam_stop_req_struct;

typedef struct
{
   kal_uint32   file_size[20];
   kal_uint32   file_address[20];
}   
media_jpegs_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_mode;
   kal_uint8    source_device;
   kal_uint16    image_width;
   kal_uint16    image_height;
   void*       image_buffer_p;
   void*       data;
   void**       capture_buffer_p;
   kal_uint32   file_buffer_len;
   kal_uint32   *file_size_p;
   kal_int16  flash_mode;
   kal_uint8   image_quality;
   kal_uint8   gray_mode;   
   kal_uint8   snapshot_number;
   kal_uint32  capture_layer;                /* capture layer for capture from mem(lcd) case */
   kal_uint16  roi_offset_x;                 /* offset x for capture from mem(lcd) case */
   kal_uint16  roi_offset_y;                 /* offset y for capture from mem(lcd) case */
   media_jpegs_struct *jpegs_p;
   kal_uint8   continue_capture;
#if (defined(MT6228)||defined(MT6229))
	/* JPEG Setting */
	kal_uint8	jpeg_yuv_mode;
	
	kal_bool		thumbnail_mode;					/* add thumbnail in JPEG file or not */
	kal_uint16	thumbnail_width;
	kal_uint16 	thumbnail_height;
	/* IPP Setting */
	kal_bool 	overlay_frame_mode;				/* KAL_TRUE or KAL_FALSE */
	kal_uint8	overlay_color_depth;				/* 1/2/4/8 bpp */
	kal_uint8	overlay_frame_source_key;		/* source key of the overlay frame buffer */
	kal_uint16  overlay_frame_width;				/* overlay frame width */
	kal_uint16	overlay_frame_height;			/* overlay frame height */
	kal_uint32  overlay_frame_buffer_address;	/* bsae address of the overlay frame buffer */

	kal_bool		image_pitch_mode;					/* KAL_TRUE or KAL_FALSE for block mode of decoded image */
	kal_uint8	image_data_format;				/* IMGDMA_IBW_OUTPUT_RGB565 or IMGDMA_IBW_OUTPUT_RGB888 */
	kal_uint16	image_pitch_bytes;				/* pitch width */

	/* Display Setting */
	kal_uint8 	lcd_id;
	kal_uint16 	lcm_start_x;				/* the start x axis of LCM to be updated*/
	kal_uint16 	lcm_start_y;				/* the start y axis of LCM to be updated*/
	kal_uint16 	lcm_end_x;					/* the end x axis of LCM to be updated*/
	kal_uint16 	lcm_end_y;					/* the end y axis of LCM to be updated*/
//	kal_uint16 	roi_offset_x;				/* the ROI window offset x from main window */
//	kal_uint16 	roi_offset_y;				/* the ROI window offset y from main window */
	kal_uint32 	update_layer;				/* the updated layer */
	kal_uint32 	hw_update_layer;			 

	kal_bool		display;								/* output to LCM or TV */
	kal_uint32	frame_buffer_address;
	kal_uint32 	frame_buffer_size;

	kal_bool		tv_output;							/* works when display item is KAL_TRUE */
   kal_uint32 	tv_output_buffer1_address;		/* image buffer1 for TV source buffer */
	kal_uint32 	tv_output_buffer2_address;		/* image buffer2 for TV source buffer, if snapshot_number==1, this buffer is not necessary */
   kal_uint32 	tv_output_buffer_size;			/* image buffer size for TV source buffer */
#endif
} 
media_cam_capture_req_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_uint32  param_id;
   kal_int16   value;
} 
media_cam_set_param_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16  flash_mode;
} 
media_cam_set_flash_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   event;
   kal_uint8   cause;
} 
media_cam_event_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   result;
   kal_uint16   count;
} 
media_cam_capture_event_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int32 sensLevel; //sensitive level. 0=low,1=mid,2=high
   void(*md_callback)(int32);
} 
media_cam_md_open_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int32   result;
} 
media_cam_md_work_ind_struct;

/* IMAGE */

typedef struct {
   LOCAL_PARA_HDR
   kal_uint16   display_width;
   kal_uint16   display_height;
   kal_uint16  image_clip_x1;                  /* clip window start x for clipping decoded image */
   kal_uint16  image_clip_x2;                  /* clip window end x for clipping decoded image */
   kal_uint16   image_clip_y1;                  /* clip window start y for clipping decoded image */
   kal_uint16   image_clip_y2;                  /* clip window end y for clipping decoded image */
   void*      image_buffer_p;
   kal_uint32   image_buffer_size;
   kal_uint8   media_mode;
   kal_uint8   media_type;
   void*      data;
   kal_uint32  file_size;
   kal_uint16  seq_num;
   kal_uint8   blocking;
	kal_bool  	jpeg_thumbnail_mode;				/* decode thumbnail or not */
#if (defined(MT6228)||defined(MT6229))
	kal_bool		image_pitch_mode;					/* KAL_TRUE or KAL_FALSE for block mode of decoded image */
	kal_uint8	image_data_format;				/* IMGDMA_IBW_OUTPUT_RGB565 or IMGDMA_IBW_OUTPUT_RGB888 */
	kal_uint16	image_pitch_bytes;				/* pitch width */

	kal_bool		memory_output;						/* output another image for TV output */
	kal_uint16	memory_output_width;				/* image width for TV output */
	kal_uint16	memory_output_height;			/* image height for TV output */
	kal_uint32 	memory_output_buffer_address;
	kal_uint32 	memory_output_buffer_size;
#endif
   kal_uint16  decoded_image_width;				// decoded image width
   void      (*callback)(kal_int32 result);
} 
media_img_decode_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   result;
   kal_uint16  seq_num;
   kal_uint16  image_width;
   kal_uint16  image_height;
} 
media_img_decode_finish_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   cause;
} 
media_img_decode_event_ind_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_uint16   image_width;
   kal_uint16   image_height;
   kal_uint8   image_quality;
   void*      image_buffer_p;
   kal_uint32   image_buffer_size;
   kal_uint16   target_width;
   kal_uint16   target_height;
   kal_uint8   media_mode;
   kal_uint8   media_type;
   void*      data;
   kal_uint32   file_buffer_len;
   kal_uint32   *file_size_p;
   kal_uint16  seq_num;
   kal_uint8   blocking;
   kal_uint8   gray_mode;   
#if (defined(MT6228)||defined(MT6229))
	kal_uint8	jpeg_yuv_mode;						/* YUV420, YUV422 or Gray mode*/

	kal_bool		thumbnail_mode;					/* add thumbnail in JPEG file or not */
	kal_uint16	thumbnail_width;
	kal_uint16 	thumbnail_height;

	kal_bool 	overlay_frame_mode;				/* KAL_TRUE or KAL_FALSE */
	kal_uint8	overlay_color_depth;				/* 1/2/4/8 bpp */
	kal_uint8	overlay_frame_source_key;		/* source key of the overlay frame buffer */
	kal_uint16  overlay_frame_width;				/* overlay frame width */
	kal_uint16	overlay_frame_height;			/* overlay frame height */
	kal_uint32  overlay_frame_buffer_address;	/* bsae address of the overlay frame buffer */
#endif
} 
media_img_encode_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   result;
   kal_uint16  seq_num;
} 
media_img_encode_finish_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   cause;
} 
media_img_encode_event_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
} 
media_img_stop_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 src_image_address;            /* the source image buffer start address */
   kal_uint32 dest_image_address;         /* the destination image buffer start address */
   kal_uint32 dest_buffer_size;            /* the buffer size of destination image buffer size */
   kal_uint16 src_width;                  /* the width of image source */
   kal_uint16 src_height;                  /* the height of image source */
   kal_uint16 src_pitch;                  /* the pitch width of image source */
   kal_uint16 target_width;               /* the width of target image */
   kal_uint16 target_height;               /* the height of target image */
   kal_uint16 image_clip_x1;               /* image clip window start x */
   kal_uint16 image_clip_x2;               /* image clip window end x */
   kal_uint16 image_clip_y1;               /* image clip window start y */
   kal_uint16 image_clip_y2;               /* image clip window end x */
   kal_uint16  seq_num;                   /* use for non-blocking mode */
   kal_uint8   blocking;
#if (defined(MT6228)||defined(MT6229))
	kal_bool		image_pitch_mode;					/* KAL_TRUE or KAL_FALSE for block mode of decoded image */
	kal_uint8	image_data_format;				/* IMGDMA_IBW_OUTPUT_RGB565 or IMGDMA_IBW_OUTPUT_RGB888 */
	kal_uint16	image_pitch_bytes;				/* pitch width */
#endif
} 
media_img_resize_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   result;
   kal_uint16  seq_num;
} 
media_img_resize_done_ind_struct;


/* VIDEO */
typedef struct {
   LOCAL_PARA_HDR
   kal_uint16   image_width;
   kal_uint16   image_height;
   kal_uint16   buffer_width;
   kal_uint16   buffer_height;
   kal_uint16   preview_width;
   kal_uint16   preview_height;
   kal_uint16   preview_offset_x;
   kal_uint16   preview_offset_y;
   void*      image_buffer_p;
   kal_uint32  image_buffer_size;
   kal_int16   frame_rate;
   kal_int16   contrast;
   kal_int16   brightness;
   kal_int16   saturation;
   kal_int16   hue;
   kal_int16   gamma;
   kal_int16   WB;
   kal_int16   exposure;
   kal_int16   effect;
   kal_int16   zoom_factor;
   kal_int16   rotate;
   kal_int16   lcd_rotate;
   kal_int16   flash_mode;
   kal_uint8   snapshot_number;
   kal_bool   night_mode;
   kal_uint8   banding_freq;
   kal_uint8   encode_quality;
   kal_uint8   bit_stream_type;
   kal_uint8     gray_mode;   
   kal_uint16  seq_num;
   kal_uint16  size_limit;
   kal_uint16  time_limit;  
   kal_uint16   record_aud;

   kal_uint8   lcd_id;
   kal_uint16  lcd_start_x;
   kal_uint16  lcd_start_y;
   kal_uint16  lcd_end_x;
   kal_uint16  lcd_end_y;
   kal_uint16  roi_offset_x;
   kal_uint16  roi_offset_y;
   kal_uint32  update_layer;
   kal_uint32  hw_update_layer;
   kal_uint32  src_key_color;   

#if (defined(MT6228)||defined(MT6229))
   /* ISP Setting */
   kal_bool    manual_wb;                     /* KAL_TRUE or KAL_FALSE to enable or disable manual white balance */
   kal_uint8   ae_metering_mode;               /* AUTO, SPOT, CENTRAL WEIGHT, AVERAGE */
   kal_uint8   iso_setting;                  /* AUTO, 100, 200, 400 */
   kal_uint8	scene_mode;							/* NORMALSCENE, PORTRAIT, LANDSCAPE, SPORT, FLOWER, NIGHTSCENE */
   /* IPP Setting */
   kal_bool   image_pitch_mode;               /* KAL_TRUE or KAL_FALSE for block mode of decoded image */
   kal_uint8   image_data_format;            /* IMGDMA_IBW_OUTPUT_RGB565 or IMGDMA_IBW_OUTPUT_RGB888 */
   kal_uint16   image_pitch_bytes;            /* pitch width */

   kal_bool    overlay_frame_mode;            /* KAL_TRUE or KAL_FALSE to enable or disable overlay function */
   kal_uint8   overlay_color_depth;            /* The color depth of overlay frame buffer */
   kal_uint8   overlay_frame_source_key;
   kal_uint16  overlay_frame_width;            /* Image width of overlay frame buffer */
   kal_uint16  overlay_frame_height;         /* Image height of overlay frame buffer */
   kal_uint32  overlay_frame_buffer_address;   /* The starting address of overlay frame buffer */
   /* TV Setting */
   kal_uint8     tv_output_mode;
   kal_bool     tv_output;
   kal_uint16   tv_output_width;               /* image width for TV output */
   kal_uint16   tv_output_height;               /* image height for TV output */
   kal_uint32    tv_output_buffer1_address;      /* image buffer1 for TV source buffer */
   kal_uint32    tv_output_buffer2_address;      /* image buffer2 for TV source buffer */
   kal_uint32    tv_output_buffer_size;         /* image buffer size for TV source buffer */
   /* LCD Setting */
   kal_uint32   roi_background_color;         /* set the background color */
#endif

} 
media_vid_preview_req_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_mode;
   kal_uint8   media_type;
   kal_uint8   record_audio;
   kal_uint8   encode_quality;
   kal_int32       data;
   kal_uint32   file_size_limit;
   kal_uint16  seq_num;


} 
media_vid_record_req_struct;

typedef struct {
   LOCAL_PARA_HDR
} 
media_vid_stop_req_struct;
typedef struct {
   LOCAL_PARA_HDR
   kal_int16 ZoomWidth;
   kal_int16 ZoomHeight;
   kal_int16 startX;
   kal_int16 startY;
   kal_int16 cutX; 
   kal_int16 cutY; 
   kal_int16 cutW; 
   kal_int16 cutH;
   kal_int16 rotate;
} 
media_vid_display_set_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   result;
   kal_uint16  seq_num;
} 
media_vid_record_finish_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   event;
} 
media_vid_encode_event_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   event;
} 
media_vid_read_audio_data_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   event;
} 
media_vid_read_video_data_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   event;
} 
media_vid_read_meta_data_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_wchar* file_path;
} 
media_vid_file_merge_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   result;
} 
media_vid_file_merge_done_ind_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_wchar* file_name_p;
   kal_uint16  seq_num;
   kal_uint16  cache_in_mem;
   void*         videoplayer_drawtime_hdlr;
} 
media_vid_open_file_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   result;
   kal_uint16  image_width;
   kal_uint16  image_height;
   kal_uint32  total_frame_num;
   kal_uint64  total_time;
   kal_uint16  seq_num;
   kal_uint16  media_type;
   kal_uint16  aud_channel;
   kal_uint16  aud_sample_rate;
   kal_uint16  track;
   
} 
media_vid_file_ready_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
} 
media_vid_close_file_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_mode;
   kal_uint8   media_type;
   void*      data;
   kal_uint32  data_size;
   kal_uint16  seq_num;
   kal_uint16  max_width;
   kal_uint16  max_height;
   kal_uint8   blocking;
   kal_uint8   play_audio;
}
media_vid_open_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint16  seq_num;
} 
media_vid_close_req_struct;



typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_mode;  /* reseved */
   kal_uint8   media_type;   /* reseved */
   void*       data;  /* reseved */
   kal_uint16   display_width;
   kal_uint16   display_height;
   kal_uint16   display_offset_x;
   kal_uint16   display_offset_y;
   void*         image_buffer_p;
   kal_uint32    image_buffer_size;
   kal_int16   zoom_factor;  /* reseved */
   kal_int16   frame_rate; /* reseved */
   kal_int16   contrast; /* reseved */
   kal_int16   brightness; /* reseved */
   kal_int16  play_speed;
   kal_uint8   play_audio;
   kal_uint8   display_device;
   kal_uint8   audio_path;
   kal_uint16  seq_num;
   kal_int16   lcd_rotate;
   kal_uint16  repeats;    /* repeat count, 0xffff: infinite */

   kal_uint8   lcd_id;
   kal_uint16  lcd_start_x;
   kal_uint16  lcd_start_y;
   kal_uint16  lcd_end_x;
   kal_uint16  lcd_end_y;
   kal_uint16  roi_offset_x;
   kal_uint16  roi_offset_y;
   kal_uint32   update_layer;
   kal_uint32   hw_update_layer;
   kal_uint16   video_display_offset_x;
   kal_uint16   video_display_offset_y;
#if (defined(MT6228)||defined(MT6229))
	kal_bool		image_pitch_mode;							/* KAL_TRUE or KAL_FALSE for block mode of decoded image */
	kal_uint8	image_data_format;						/* IMGDMA_IBW_OUTPUT_RGB565 or IMGDMA_IBW_OUTPUT_RGB888 */
	kal_uint16	image_pitch_bytes;						/* pitch width */
	/* TV Setting */
	kal_bool		tv_output;
   kal_uint32 	tv_output_buffer1_address;		/* image buffer1 for TV source buffer */
   kal_uint32 	tv_output_buffer2_address;		/* image buffer2 for TV source buffer */
   kal_uint32 	tv_output_buffer_size;			/* image buffer size for TV source buffer */
#endif
} 
media_vid_play_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   result;
   kal_uint16  seq_num;
} 
media_vid_play_finish_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int32   playtime;
} 
media_vid_play_time_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   event;
} 
media_vid_decode_event_ind_struct;


typedef struct {
   LOCAL_PARA_HDR
} 
media_vid_pause_req_struct;

typedef struct {
   LOCAL_PARA_HDR
} 
media_vid_display_interface_req_struct;

typedef struct {
   LOCAL_PARA_HDR
} 
media_vid_resume_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8     get_frame;
   kal_uint32    frame_num;
   kal_uint64    time;
   kal_uint16   display_width;
   kal_uint16   display_height;
   kal_int16   display_offset_x;
   kal_int16   display_offset_y;
   void*         image_buffer_p;
   kal_uint32    image_buffer_size;
   kal_uint8   blocking;
   kal_uint16  seq_num;
#if (defined(MT6228)||defined(MT6229))
   kal_bool		image_pitch_mode;							/* KAL_TRUE or KAL_FALSE for block mode of decoded image */
   kal_uint8	image_data_format;						/* IMGDMA_IBW_OUTPUT_RGB565 or IMGDMA_IBW_OUTPUT_RGB888 */
   kal_uint16	image_pitch_bytes;						/* pitch width */
   /* TV Setting */
   kal_bool		tv_output;
   kal_uint32 	tv_output_buffer1_address;		/* image buffer1 for TV source buffer */
   kal_uint32 	tv_output_buffer2_address;		/* image buffer2 for TV source buffer */
   kal_uint32 	tv_output_buffer_size;			/* image buffer size for TV source buffer */
#endif
} 
media_vid_seek_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   result;
   kal_uint16  seq_num;
} 
media_vid_seek_done_ind_struct;


typedef struct {
   LOCAL_PARA_HDR
   void*         image_buffer_p;
   kal_uint32    image_buffer_size;
} 
media_vid_snapshot_req_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_mode;
   kal_uint8   media_type;
   void*       data;
   kal_uint16   display_width;
   kal_uint16   display_height;
   void*         image_buffer_p;
   kal_uint32    image_buffer_size;
   kal_uint16   frame_num;
   kal_uint16  seq_num;
   kal_uint8   blocking;
#if (defined(MT6228)||defined(MT6229))
	kal_bool		image_pitch_mode;							/* KAL_TRUE or KAL_FALSE for block mode of decoded image */
	kal_uint8	image_data_format;						/* IMGDMA_IBW_OUTPUT_RGB565 or IMGDMA_IBW_OUTPUT_RGB888 */
	kal_uint16	image_pitch_bytes;						/* pitch width */
	/* TV Setting */
	kal_bool		tv_output;
   kal_uint32 	tv_output_buffer1_address;		/* image buffer1 for TV source buffer */
   kal_uint32 	tv_output_buffer2_address;		/* image buffer2 for TV source buffer */
   kal_uint32 	tv_output_buffer_size;			/* image buffer size for TV source buffer */
#endif
} 
media_vid_get_iframe_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   result;
   kal_uint16  seq_num;
} 
media_vid_iframe_ready_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 audio_start_frame_num;
} 
media_vid_play_restart_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   result;
} 
media_vid_audio_play_finish_ind_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 device;
} 
media_vid_set_display_req_struct;

/* Web camera */

typedef struct
{
	kal_uint16 width;
	kal_uint16 height;
	kal_uint8* start_addr;
	kal_uint32 size;
} 
webcam_default_jpeg_file_info;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   rotate;
    kal_uint16 banding;
   kal_uint16  seq_num;                         /* sequence num */
} 
media_webcam_start_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   result;
   kal_uint16  seq_num;                         /* sequence num */
    kal_bool camera_workable;
} 
media_webcam_start_cnf_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint16 seq_num;  /* sequence num */
}
media_webcam_abort_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
} 
media_webcam_stop_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   webcam_default_jpeg_file_info *file_info;
   kal_uint8   no_of_jpeg_files;
} 
media_webcam_pause_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   rotate;
} 
media_webcam_resume_req_struct;


typedef struct {
   LOCAL_PARA_HDR
} 
media_webcam_capture_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   rotate;
} 
media_webcam_rotate_req_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_int16 event;
}
media_webcam_encode_req_struct;


/* TV out */

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   tv_output_mode;
   kal_uint8   tv_output_format;
   kal_uint16   tv_output_width;               /* image width for TV output */
   kal_uint16   tv_output_height;               /* image height for TV output */
} 
media_tvo_start_req_struct;

typedef struct {
   LOCAL_PARA_HDR
} 
media_tvo_stop_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   tv_output_mode;
   kal_uint8   tv_output_format;
   kal_uint16   tv_output_width;               /* image width for TV output */
   kal_uint16   tv_output_height;               /* image height for TV output */
} 
media_tvo_set_mode_req_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_int16   result;
} 
media_tvo_tv_connect_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int16   result;
} 
media_tvo_av_cable_detect_ind_struct;


/* MMA */

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_type;
   kal_int32   handler;
   kal_uint8   *data;
   kal_int32   size;            
   kal_int16   repeats;           
} 
media_mma_open_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_type;
   kal_int32   handle;
} 
media_mma_close_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_type;
   kal_int32   handle;
} 
media_mma_play_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_type;
   kal_int32   handle;
} 
media_mma_stop_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_type;
   kal_int32   handle;
} 
media_mma_pause_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_type;
   kal_int32   handle;
} 
media_mma_resume_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_type;
   kal_int32   handle;
   kal_int32   start_time;
} 
media_mma_set_start_time_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_type;
   kal_int32   handle;
   kal_int32   stop_time;
} 
media_mma_set_stop_time_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_type;
   kal_int32   handle;
   kal_int32   rate;
} 
media_mma_set_rate_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_type;
   kal_int32   handle;
   kal_int32   tempo;
} 
media_mma_set_tempo_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_type;
   kal_int32   handle;
   kal_int32   pitch;
} 
media_mma_set_pitch_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_type;
   kal_int32   handle;
   kal_uint8   volume;
} 
media_mma_set_volume_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int32   handle;
   kal_uint8*   buf;
   kal_uint16   len;
} 
media_mma_send_long_msg_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int32   handle;
   kal_uint8   type;
   kal_uint8   data1;
   kal_uint8   data2;
} 
media_mma_send_short_msg_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_type;
   kal_int32   handler;
   kal_uint8   *data;
   kal_int32   size;            
   kal_int16   repeats;           
} 
media_mma_open_device_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int32   handle;
} 
media_mma_close_device_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int32   handle;
   kal_uint8   channel;
   kal_uint8   level;
} 
media_mma_set_channel_volume_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_int32   handle;
   kal_uint8   channel;
   kal_uint8   bank;
   kal_uint8   program;
} 
media_mma_set_program_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8   media_type;
   kal_int32   handle;
   kal_uint8   *data;
   kal_int32   size;            
   kal_int16   repeats;           
} 
media_mma_set_data_req_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_int32   handle;
   kal_uint8   event;
} 
media_mma_play_event_ind_struct;

/* VR related messages */
typedef struct {
   kal_uint8 mode;
   kal_uint8 language;
   LOCAL_PARA_HDR
}
media_vr_get_version_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 mode;
   kal_uint8 language;
   kal_int16* sim;
   kal_int16* diff;
   kal_int16* rej;
}
media_vr_get_param_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 mode;
   kal_uint8 language;
   kal_int16 sim;
   kal_int16 diff;
   kal_int16 rej;
}
media_vr_set_param_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id;
   kal_uint8 mode;
   kal_uint8 language;
   kal_uint8 group_id;
}
media_vr_init_rcg_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id;
   kal_int32 result;
   kal_uint16 id_length;
   kal_uint16 id_array[VR_MAX_RCG_RESULT_NUM];
}
media_vr_rcg_result_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id;
   kal_uint8 mode;
   kal_uint8 language;
   kal_uint8 group_id;
   kal_uint16 word_id;
}
media_vr_init_trn_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id;
   kal_int32 result;
}
media_vr_trn_result_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id;
   kal_uint8 seq_no;
}
media_vr_voice_in_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id;
   kal_int32 result;
}
media_vr_voice_in_cnf_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 mode;
   kal_uint8 language;
   kal_uint8 group_id;
   kal_uint16 word_id;
}
media_vr_del_tag_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 mode;
   kal_uint8 language;
   kal_uint8 group_id;
   kal_uint16 word_id;
}
media_vr_check_tag_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 mode;
   kal_uint8 language;
   kal_uint8 group_id;
   kal_uint16* id_length_p;
   kal_uint16* id_array;
}
media_vr_sync_db_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 mode;
   kal_uint8 language;
   kal_uint8 group_id;
   kal_uint16 word_id;
   kal_uint16 identifier;
}
media_vr_play_tag_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 mode;
   kal_uint8 language;
   kal_uint8 group_id;
   kal_uint16* folder_name;
}
media_vr_get_dir_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 mode;
   kal_uint8 language;
   kal_uint8 group_id;
   kal_uint16 id_length;
   kal_uint16 id_array[VR_MAX_ADD_ONCE_NUM];
}
media_vr_add_tag_req_struct;

typedef struct {
   LOCAL_PARA_HDR
}
media_vr_abort_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id;
}
media_vr_abort_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 device;
} 
media_aud_set_headset_mode_output_path_struct;

typedef struct {
   LOCAL_PARA_HDR   
}media_vrsi_init_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id; 
   kal_uint8 *grammar; 
   kal_uint16 tag_num; 
   kal_uint16** name_list; 
   kal_int32 *long_list; 
   kal_uint8 *is_name_list; 
   kal_uint16 *id_list;
}media_vrsi_add_tags_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id; 
   kal_uint8 *grammar; 
   kal_uint16 *tag_name; 
   kal_int32 tag_long;
   kal_uint8 lang;
}media_vrsi_start_trn_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id; 
   kal_uint8 pmp_mode;    
}media_vrsi_trn_prompt_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id; 
   kal_uint16 tag_id;
}media_vrsi_trn_result_ind_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id; 
   kal_uint8 *grammar;
   kal_uint8 lang; 
   kal_uint16 limit;
}media_vrsi_start_rcg_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id; 
   kal_uint8 pmp_mode;
}media_vrsi_rcg_prompt_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id; 
   kal_uint16 res_type;    
}media_vrsi_rcg_result_ind_struct;


typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id;
   kal_uint8 proc_type;
}media_vrsi_process_ind_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id; 
   kal_uint8 *grammar; 
   kal_uint16 tag_id;
   kal_uint8 volume;
   kal_uint8 output_path;    
}media_vrsi_play_tag_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id; 
   kal_uint16 *text;
   kal_uint8 volume;
   kal_uint8 output_path;   
   kal_uint8 lang;
}media_vrsi_play_tts_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 *grammar; 
   kal_int32 *tag_num;
}media_vrsi_get_tagnum_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 *grammar; 
   kal_int32 tag_num; 
   kal_uint16 *id_list; 
   kal_uint16 **name_list; 
   kal_int32 *long_list;
}media_vrsi_get_taginfo_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 *grammar; 
   kal_int32 tag_num; 
   kal_uint16 *id_list;
}media_vrsi_del_tags_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 *grammar;
}media_vrsi_reset_tags_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint8 *grammar; 
   kal_int32 tag_num; 
   kal_uint16 *id_list; 
   kal_uint16 **name_list; 
   kal_uint8 *sync_flag;
}media_vrsi_sync_db_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id;
}media_vrsi_abort_req_struct;

typedef struct {
   LOCAL_PARA_HDR 
}media_vrsi_close_req_struct;

typedef struct {
   LOCAL_PARA_HDR
   kal_uint32 session_id;    
}media_vrsi_err_ind_struct;

typedef struct {
   LOCAL_PARA_HDR 
   kal_uint32 session_id; 
}media_vrsi_play_tag_finish_ind_struct;

typedef struct {
   LOCAL_PARA_HDR 
   kal_uint32 session_id; 
}media_vrsi_play_tts_finish_ind_struct;

 
typedef struct
{
   LOCAL_PARA_HDR
   kal_uint16 res_type;
   kal_uint16 tag_num;
   kal_uint16 *id_list;
   kal_uint16 **name_list;
   kal_int32 *long_list; 
   kal_int16 pre_cmd;
   kal_int16 post_cmd;
}media_aud_vrsi_cmd_rcg_result_struct; 

/* ---- bt audio begin ---- */
typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 profile;   /* 0: HFP, 1: A2DP */
    kal_uint8 mode;      /* HFP => 0: Earphone, 1: Car Kit /// A2DP => 0: INT mode, 1: ACP mode, 2: ACP mode for PTS only*/
    kal_uint16 connect_id;
} media_bt_audio_open_req_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 profile;
    kal_uint16 result;
} media_bt_audio_open_cnf_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 profile;
} media_bt_audio_open_ind_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 profile;
} media_bt_audio_close_req_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 profile;
    kal_uint16 result;
} media_bt_audio_close_cnf_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 profile;
    kal_uint16 cause;
} media_bt_audio_close_ind_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 profile;
} media_bt_audio_turn_on_req_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint8 profile;
} media_bt_audio_turn_off_req_struct;

/* ---- bt audio end ---- */
#ifndef __BT_A2DP_CODEC_TYPES__
#define __BT_A2DP_CODEC_TYPES__
typedef struct
{
	kal_uint8 min_bit_pool;
	kal_uint8 max_bit_pool;
	kal_uint8 block_len; // b0: 16, b1: 12, b2: 8, b3: 4
	kal_uint8 subband_num; // b0: 8, b1: 4
	kal_uint8 alloc_method; // b0: loudness, b1: SNR
	kal_uint8 sample_rate; // b0: 48000, b1: 44100, b2: 32000, b3: 16000
	kal_uint8 channel_mode; // b0: joint stereo, b1: stereo, b2: dual channel, b3: mono
} bt_a2dp_sbc_codec_cap_struct;

typedef struct
{
	kal_uint8 layer; // b0: layerIII, b1: layerII, b0: layerI
	kal_uint8 CRC;
	kal_uint8 channel_mode; // b0: joint stereo, b1: stereo, b2: dual channel, b3: mono
	kal_uint8 MPF; // is support MPF-2
	kal_uint8 sample_rate; // b0: 48000, b1: 44100, b2: 32000, b3: 24000, b4: 22050, b5: 16000
	kal_uint8 VBR; // is support VBR
	kal_uint16 bit_rate; // bit-rate index in ISO 11172-3 , b0:0000 ~ b14: 1110
} bt_a2dp_mp3_codec_cap_struct; /* all MPEG-1,2 Audio */


typedef struct
{
	kal_uint8 object_type; // b4: M4-scalable, b5: M4-LTP, b6: M4-LC, b7: M2-LC
	kal_uint16 sample_rate; // b0~b11: 96000,88200,64000,48000,44100,32000,24000,22050,16000,12000,11025,8000
	kal_uint8 channels; // b0: 2, b1: 1
	kal_uint8 VBR; // is supported VBR
	kal_uint32 bit_rate; // constant/peak bits per second in 23 bit UiMsbf, 0:unknown
} bt_a2dp_aac_codec_cap_struct; /* all MPEG-2,4 AAC */

typedef struct
{
	kal_uint8 version; // 1:ATRAC, 2:ATRAC2, 3:ATRAC3
	kal_uint8 channel_mode; // b0: joint stereo, b1: dual, b2: single
	kal_uint8 sample_rate; // b0: 48000, b1: 44100
	kal_bool VBR; // is supported VBR
	kal_uint32 bit_rate; // bit-rate index in ATRAC, b0: 0x0012 ~ b18: 0x0000
	kal_uint16 max_sul; // sound unit length in 16 bits UiMsbf
} bt_a2dp_atrac_codec_cap_struct; /* all ATRAC family */

typedef union
{
	bt_a2dp_sbc_codec_cap_struct sbc;
	bt_a2dp_mp3_codec_cap_struct mp3;
//	bt_a2dp_aac_codec_cap_struct aac;
//	bt_a2dp_atrac_codec_cap_struct atrac;
} bt_a2dp_audio_codec_cap_struct;

typedef struct
{
	kal_uint8 codec_type; // SBC, MP3
	bt_a2dp_audio_codec_cap_struct codec_cap;
} bt_a2dp_audio_cap_struct;
#endif /* __BT_A2DP_CODEC_TYPES__ */

typedef struct
{
    LOCAL_PARA_HDR
    bt_a2dp_audio_cap_struct cfg;
    kal_uint32 seq_no;
    kal_uint8 stream_handle;
    kal_bool immediate;
} media_bt_open_codec_req_struct;

typedef struct
{
    LOCAL_PARA_HDR
    kal_uint32 seq_no;
    kal_uint8 stream_handle;
} media_bt_close_codec_ind_struct;
typedef struct
{
    LOCAL_PARA_HDR
   kal_uint32   *aeHandle;
   kal_int16  *pData;
   kal_int32   dataLen;
   void (*ae_callback)(void);
} media_aud_ae_req_struct;
#endif /* _MED_STRUCT_H */

#endif /* MED_NOT_PRESENT */

