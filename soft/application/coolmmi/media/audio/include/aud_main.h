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
 *	aud_main.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes primary global variables of audio manager task.
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef MED_NOT_PRESENT

#ifndef _AUD_MAIN_H
#define _AUD_MAIN_H

extern HANDLE g_audioSem;//chenhe,20100417

typedef enum
{
   AUD_EVT_NOWAIT    = 0,
   AUD_EVT_PLAY	     = 0x0001,
   AUD_EVT_STOP     = 0x0002,
   AUD_EVT_RECORD      = 0x0004,
   AUD_EVT_PAUSE      = 0x0008,
   AUD_EVT_RESUME      = 0x0010,
   AUD_EVT_BLOCK      = 0x0020,
   AUD_EVT_FMR_ON   = 0x0040,
   AUD_EVT_FMR_OFF = 0x0080,
   AUD_EVT_FMR_SET = 0x0100,
   AUD_EVT_FMR_MUTE = 0x0200,
   AUD_EVT_VR_GET_VER = 0x0400,
   AUD_EVT_VR_GET_PARAM = 0x0800,
   AUD_EVT_VR_SET_PARAM = 0x1000,
   AUD_EVT_VR_INIT_RCG = 0x2000,
   AUD_EVT_VR_INIT_TRN = 0x4000,
   AUD_EVT_VR_VOICE_IN = 0x8000,
   AUD_EVT_VR_DEL_TAG = 0x10000,
   AUD_EVT_VR_CHECK_TAG = 0x20000,
   AUD_EVT_VR_SYNC_DB = 0x40000,
   AUD_EVT_VR_PLAY_TAG = 0x80000,
   AUD_EVT_VR_GET_DIR = 0x100000,
   AUD_EVT_VR_ADD_TAG = 0x200000,
   AUD_EVT_VR_ABORT = 0x400000,
   AUD_EVT_GET_FILE_DATA_LEN = 0x800000,
   AUD_EVT_GET_FILE_DATA_PROGRESS = 0x10000000,
   AUD_EVT_SET_FILE_DATA_PROGRESS = 0x20000000,
    AUD_EVT_BT_CLOSE_AUDIO =    0x40000000,
    AUD_EVT_BT_TURN_OFF_AUDIO = 0x80000000,
} 
aud_wait_event_enum;

typedef enum
{
   VRSI_EVT_INIT = 0x0001,
   VRSI_EVT_ADD_TAGS = 0x0002, 
   VRSI_EVT_START_CMD_TRN = 0x0004, 
   VRSI_EVT_START_CMD_RCG = 0x0008,  
   VRSI_EVT_START_DIGIT_RCG = 0x0010, 
   VRSI_EVT_START_DIGIT_ADAPT = 0x0020, 
   VRSI_EVT_PLAY_TAG = 0x0040, 
   VRSI_EVT_PLAY_TTS = 0x0080, 
   VRSI_EVT_GET_TAG_NUM = 0x0100, 
   VRSI_EVT_GET_TAG_INFO = 0x0200, 
   VRSI_EVT_DEL_TAGS = 0x0400, 
   VRSI_EVT_RESET_TAGS = 0x0800, 
   VRSI_EVT_SYNC_DB = 0x1000, 
   VRSI_EVT_ABORT = 0x2000, 
   VRSI_EVT_CLOSE = 0x4000  
}
vrsi_wait_event_enum;

typedef enum {
	AUD_VR_STATE_IDLE,
	AUD_VR_STATE_TRN_SESSION,
	AUD_VR_STATE_RCG_SESSION,
	AUD_VR_NO_OF_STATE
} aud_vr_state_enum;

typedef enum {
	AUD_VR_SD_SESSION_STAGE_NOT_READY,
	AUD_VR_SD_SESSION_STAGE_READY,
	AUD_VR_SD_SESSION_STAGE_PROCESS,
	AUD_VR_SD_SESSION_STAGE_WAIT,
	AUD_VR_SD_SESSION_STAGE_PROCESS_2ND,
	AUD_VR_SD_SESSION_NO_OF_STAGE
} aud_vr_sd_session_stage_enum;

typedef struct
{
	module_type src_mod;
	kal_uint32 session_id;
	kal_uint16 db_path[VR_MAX_DB_PATH_LEN + 1];
	kal_uint16 group_id;
	kal_uint16 word_id;
	kal_int16 db_drive;
	kal_uint8 state;
	kal_uint8 mode;
	kal_uint8 lang;
}
aud_vr_context_struct;

typedef struct
{
   VRSI_AddTags_Param addtags_param;
   VRSI_Tag_Param tag_param; 
   VRSI_Recog_Result *recog_result;   
   VRSI_Digit_Recog_Result *dgt_rcg_result;
   kal_uint8 *buffer;
   kal_uint16 *id_list;  
   kal_uint8 *grammar; 
   kal_uint32 session_id;     
   kal_bool session_on;    
   kal_uint8 pmp_len;
   kal_uint8 state;
   const kal_uint16 *pmp_text; 
   kal_uint16 db_path[VR_MAX_DB_PATH_LEN + 1];
}aud_vrsi_context_struct;   

typedef struct
{
	kal_uint8 stage;
	kal_uint16 id_array[VR_SD_MAX_GROUP_NUM][VR_SD_MAX_GROUP_TAG_NUM];
	kal_uint16 id_length[VR_SD_MAX_GROUP_NUM];
	kal_uint8* buffer;
}
aud_vr_sd_context_struct;

#if 0
#define AUD_WAIT_EVENT(evt_) do{ \
         kal_uint32 retrieved_events;\
         kal_retrieve_eg_events(aud_context_p->aud_event, (evt_), \
         KAL_OR_CONSUME, &retrieved_events, KAL_SUSPEND); }while(0) 
#endif
typedef struct
{
    BOOL issetstate;
    UINT32 audiostatevar;
}AudioStateData;

extern AudioStateData Audiostate;

#define AUD_INIT_EVENT() 
#if 0 
do{ \
    Audiostate.audiostatevar = AUD_EVT_NOWAIT; \
    Audiostate.issetstate = FALSE; \
    }while(0)
#endif    
#define AUD_WAIT_EVENT(evt_)  COS_WaitForSemaphore(g_audioSem, COS_WAIT_FOREVER)
#if 0
while(1 && (!Audiostate.issetstate)){ \
         if ((Audiostate.audiostatevar == evt_) && (Audiostate.issetstate == TRUE)) \
         break;\
          } 
#endif         
#define AUD_SET_EVENT(evt_)   COS_ReleaseSemaphore(g_audioSem)
#if 0
do{ \
        Audiostate.audiostatevar = evt_; \
        Audiostate.issetstate = TRUE; \
        } while(0)
#endif
#define VRSI_WAIT_EVENT(evt_) do{ \
         kal_uint32 retrieved_events;\
         kal_retrieve_eg_events(aud_context_p->vrsi_event, (evt_), \
         KAL_OR_CONSUME, &retrieved_events, KAL_SUSPEND); }while(0) 

#define VRSI_SET_EVENT(evt_) do{ \
	kal_set_eg_events(aud_context_p->vrsi_event, (evt_),KAL_OR); \
	} while(0)
//in greenstone platform, when set current volume, save the volume value in the three different mode by the same volume value. NOTE:this manner diff from original   
#define SET_CURRENT_VOLUME_LEVEL(_m_,_v_) if(aud_context_p->audio_mode<MAX_AUD_MODE_NUM&&_m_<AUD_MAX_VOLUME_TYPE&&_v_<AUD_MAX_VOLUME_LEVEL)\
                                                {aud_context_p->acoustic_data.volume[AUD_MODE_NORMAL][_m_]=_v_;\
                                                 aud_context_p->acoustic_data.volume[AUD_MODE_HEADSET][_m_]=_v_;\
                                                 aud_context_p->acoustic_data.volume[AUD_MODE_LOUDSPK][_m_]=_v_;}
/* for MMA event group */
typedef enum
{
   MMA_EVT_NOWAIT    = 0,
   MMA_EVT_OPEN	     = 0x0001,
   MMA_EVT_CLOSE     = 0x0002,
   MMA_EVT_PLAY	     = 0x0004,
   MMA_EVT_STOP     = 0x0008,
   MMA_EVT_PAUSE      = 0x0010,
   MMA_EVT_RESUME          = 0x0020,
   MMA_EVT_SET_START_TIME      = 0x0040,
   MMA_EVT_SET_STOP_TIME      = 0x0080,
   MMA_EVT_SET_RATE           = 0x0100,
   MMA_EVT_SET_TEMPO          = 0x0200,
   MMA_EVT_SET_PITCH          = 0x0400,
   MMA_EVT_SET_VOLUME         = 0x0800,
   MMA_EVT_SEND_LONG_MSG     = 0x1000,
   MMA_EVT_SEND_SHORT_MSG     = 0x2000,
   MMA_EVT_OPEN_DEVICE        = 0x4000,
   MMA_EVT_CLOSE_DEVICE       = 0x8000,
   MMA_EVT_SET_CHANNEL_VOLUME      = 0x10000,
   MMA_EVT_SET_PROGRAM              = 0x20000,
   MMA_EVT_SET_DATA              = 0x40000
} 
mma_wait_event_enum;


#define MMA_WAIT_EVENT(evt_) do{ \
         kal_uint32 retrieved_events;\
         kal_retrieve_eg_events(aud_context_p->mma_event, (evt_), \
         KAL_OR_CONSUME, &retrieved_events, KAL_SUSPEND); }while(0) 


#define MMA_SET_EVENT(evt_) kal_set_eg_events(aud_context_p->mma_event, (evt_),KAL_OR)

/*
typedef enum
{
   MMA_PLAYER_IMY=0,
   MMA_PLAYER_SMF_1,
   MMA_PLAYER_SMF_2,
   MMA_PLAYER_SMF_3,
   MMA_PLAYER_SMF_4,
   MMA_PLAYER_TONE_S1,
   MMA_PLAYER_TONE_S2,
   MMA_PLAYER_TONE_S3,
   MMA_PLAYER_TONE_S4,
   MMA_PLAYER_WAV_1,
   MMA_PLAYER_WAV_2,
   MMA_PLAYER_WAV_3,
   MMA_PLAYER_XWAV,
   MMA_NO_OF_PLAYER
}
mma_player_enum;
*/
#if defined(__MED_GENERAL_TTS__)
typedef enum
{
    TTS_EVT_NO_WAIT = 0,
    TTS_EVT_PLAY = 0x0001,
    TTS_EVT_STOP = 0x0002,
    TTS_EVT_PAUSE = 0x0004,
    TTS_EVT_RESUME = 0x0008,
    TTS_EVT_SET_ATTR = 0x0010
}
aud_tts_wait_event_enum;


#define TTS_WAIT_EVENT(evt_)  COS_WaitForSemaphore(g_TTSSem, COS_WAIT_FOREVER)
#if 0
#define TTS_WAIT_EVENT(evt_) do{                                  \
         kal_uint32 retrieved_events;                                \
         kal_retrieve_eg_events(aud_context_p->tts_event, (evt_), \
         KAL_OR_CONSUME, &retrieved_events, KAL_SUSPEND); }while(0)
#endif         

#define TTS_SET_EVENT(evt_)   COS_ReleaseSemaphore(g_TTSSem)

#if 0
#define TTS_SET_EVENT(evt_) kal_set_eg_events(aud_context_p->tts_event, (evt_),KAL_OR)
#endif



#endif /* #if defined(__MED_GENERAL_TTS__) */ 

#define aud_send_msg_to_med( src_id, msg_id, local_param_ptr ) aud_send_msg(src_id, MOD_MED, msg_id, local_param_ptr)
#define aud_send_msg_from_med( dst_id, msg_id, local_param_ptr ) aud_send_msg(MOD_MED, dst_id, msg_id, local_param_ptr)

#ifdef __MED_BT_A2DP_MOD__
typedef enum
{
    AUD_BT_A2DP_STATE_IDLE,
    AUD_BT_A2DP_STATE_CONFIGURED,
    AUD_BT_A2DP_STATE_OPEN,
    AUD_BT_A2DP_STATE_STREAMING
} aud_bt_a2dp_state_enum;

typedef enum
{
    AUD_BT_A2DP_WAIT_NONE,
    AUD_BT_A2DP_WAIT_ABORT,
    AUD_BT_A2DP_WAIT_DISCOVER,
    AUD_BT_A2DP_WAIT_GET_CAP,
    AUD_BT_A2DP_WAIT_CONFIG_STREAM,
    AUD_BT_A2DP_WAIT_RECONFIG_STREAM,
    AUD_BT_A2DP_WAIT_OPEN_STREAM,
    AUD_BT_A2DP_WAIT_START_STREAM,
    AUD_BT_A2DP_WAIT_PAUSE_STREAM,
    AUD_BT_A2DP_WAIT_CLOSE_STREAM
} aud_bt_a2dp_wait_enum;

typedef enum
{
    AUD_BT_A2DP_CMD_NONE,
    AUD_BT_A2DP_CMD_ABORT,
    AUD_BT_A2DP_CMD_RECONFIG_STREAM,
    AUD_BT_A2DP_CMD_START_STREAM,
    AUD_BT_A2DP_CMD_PAUSE_STREAM,
    AUD_BT_A2DP_CMD_CLOSE_STREAM
} aud_bt_a2dp_cmd_enum;

typedef enum
{
    AUD_BT_A2DP_SERVICE_STATUS_IDLE,
    AUD_BT_A2DP_SERVICE_STATUS_OPENING,
    AUD_BT_A2DP_SERVICE_STATUS_RUNNING,
    AUD_BT_A2DP_SERVICE_STATUS_CLOSING,
    AUD_BT_A2DP_SERVICE_STATUS_OPENING_CLOSING
} aud_bt_a2dp_sevice_status_enum;
#endif /* __MED_BT_A2DP_MOD__ */ 

#ifdef __MED_BT_HFP_MOD__
typedef enum
{
    AUD_BT_HFP_STATE_IDLE,
    AUD_BT_HFP_STATE_OPEN
} aud_bt_hfp_state_enum;

typedef enum
{
    AUD_BT_HFP_CMD_NONE,
    AUD_BT_HFP_CMD_CONNECT_SCO,
    AUD_BT_HFP_CMD_DISCONNECT_SCO
} aud_bt_hfp_cmd_enum;
#endif /* __MED_BT_HFP_MOD__ */ 
/*==== PROTOTYPE ===========*/

/* main */
kal_bool aud_init(void);
void aud_main( ilm_struct *ilm_ptr);
void aud_startup_hdlr( ilm_struct *ilm_ptr);
void aud_set_startup_param(void);
void aud_startup_read_nvram_data(void);


/* audio */
kal_bool aud_audio_init(void);
void aud_get_audio_mode_req_hdlr(ilm_struct *ilm_ptr);
void aud_set_audio_mode_req_hdlr(ilm_struct *ilm_ptr);
void aud_gpio_detect_ind_hdlr(ilm_struct *ilm_ptr);
void aud_get_audio_profile_req_hdlr(ilm_struct *ilm_ptr);
void aud_set_audio_profile_req_hdlr(ilm_struct *ilm_ptr);
void aud_get_audio_param_req_hdlr(ilm_struct *ilm_ptr);
void aud_set_audio_param_req_hdlr(ilm_struct *ilm_ptr);
void aud_set_aud_path_volume(kal_uint8 audio_mode);
void aud_audio_set_volume_req_hdlr(ilm_struct *ilm_ptr);
void aud_audio_mute_req_hdlr(ilm_struct *ilm_ptr);
void aud_set_mute_req_hdlr(ilm_struct *ilm_ptr);
kal_bool aud_cfg_audio_out_device(kal_uint8 audio_sound_id, kal_uint8 *out_device_path);
void aud_play_by_id_req_hdlr(ilm_struct *ilm_ptr);
void aud_stop_by_id_req_hdlr(ilm_struct *ilm_ptr);
void aud_play_by_name_req_hdlr(ilm_struct *ilm_ptr);
void aud_stop_by_name_req_hdlr(ilm_struct *ilm_ptr);
void aud_play_by_string_req_hdlr(ilm_struct *ilm_ptr);
#ifdef __MMI_TTS_FEATURES__
extern void aud_play_by_endless_string_req_hdlr(ilm_struct *ilm_ptr);
#endif
void aud_stop_by_string_req_hdlr(ilm_struct *ilm_ptr);
kal_uint8 aud_get_volume_gain(kal_uint8 mode, kal_uint8 type, kal_uint8 level);
kal_uint8 aud_get_volume_level(kal_uint8 mode, kal_uint8 type);
void aud_audio_set_device_req_hdlr(ilm_struct *ilm_ptr);
void aud_audio_set_gain_req_hdlr(ilm_struct *ilm_ptr);
void aud_block_req_hdlr(ilm_struct *ilm_ptr);
void aud_set_equalizer_req_hdlr(ilm_struct *ilm_ptr);
void aud_set_headset_mode_output_path_req_hdlr( ilm_struct *ilm_ptr );
void aud_get_file_data_length_req_hdlr( ilm_struct* ilm_ptr );
void aud_get_file_data_progress_req_hdlr( ilm_struct* ilm_ptr );
void aud_set_file_data_progress_req_hdlr( ilm_struct* ilm_ptr );

/* speech */
kal_bool aud_speech_init(void);
void aud_speech_main(ilm_struct *ilm_ptr);
void aud_speech_set_output_device(kal_uint8 device);
void aud_speech_set_output_volume(kal_uint8 volume1, kal_uint8 volume2);
void aud_speech_set_input_mode_req_hdlr(ilm_struct *ilm_ptr);
void aud_speech_set_mode_req_hdlr(ilm_struct *ilm_ptr);
void aud_speech_set_fir(kal_bool fir_on);
void aud_mic_set_device(kal_uint8 device);
void aud_mic_set_device_no_apply(kal_uint8 device);
void aud_mic_set_volume(kal_uint8 volume1, kal_uint8 volume2);
void aud_mic_mute(kal_bool mute);
void aud_side_tone_set_volume(kal_uint8 volume1, kal_uint8 volume2);


/* keytone */
kal_bool aud_keytone_init(void);
void aud_keytone_main(ilm_struct *ilm_ptr);
void aud_keytone_set_output_device(kal_uint8 device);
void aud_keytone_set_output_volume(kal_uint8 volume1, kal_uint8 volume2);
void aud_keytone_play(kal_uint8 audio_id, kal_uint8 audio_out_device);
void aud_keytone_stop(void);

/* tone */
kal_bool aud_tone_init(void);
void aud_tone_main(ilm_struct *ilm_ptr);
void aud_tone_set_output_device(kal_uint8 device);
void aud_tone_set_output_volume(kal_uint8 volume1, kal_uint8 volume2);
void aud_tone_play(kal_uint8 audio_id, kal_uint8 audio_out_device);
void aud_tone_stop(void);
void aud_set_active_tone_volume(void);

/* melody */
kal_bool aud_melody_init(void);
void aud_free_melody_file_buffer(void);
kal_bool aud_melody_startup(void);
void aud_melody_main(ilm_struct *ilm_ptr);
void aud_melody_set_output_device(kal_uint8 device);
void aud_melody_set_output_volume(kal_uint8 volume1, kal_uint8 volume2);
void aud_set_active_ring_tone_volume(void);
void aud_set_active_sound_volume(void);
kal_uint8 aud_melody_play_style_convert(kal_uint8 play_style);
kal_int32 aud_replay_stream(void);
void aud_melody_play_by_id(kal_uint8 audio_id, kal_uint8 play_style, kal_uint8 audio_out_device);
void aud_melody_stop_crescendo(void);
void aud_melody_crescendo_hdlr(void* arg);
void aud_melody_set_volume(kal_uint8 volume1, kal_uint8 volume2);
kal_uint8 aud_melody_resume_by_name(kal_wchar* file_name, kal_uint8 play_style, kal_uint8 audio_out_device);
kal_uint8 aud_melody_play_by_name(kal_wchar* file_name, kal_uint8 play_style, kal_uint8 audio_out_device);
kal_int32 aud_melody_play_by_string(kal_uint8* string, kal_uint8 play_style, kal_uint32 string_len, kal_uint8 format);
void aud_melody_set_max_swing(kal_uint16 max_swing);
void aud_media_play_event_hdlr(ilm_struct *ilm_ptr);
void aud_media_event_callback(Media_Event event);
void aud_set_vibrator_enabled_req_hdlr( ilm_struct *ilm_ptr );
void aud_melody_stop_driver(void);
void aud_melody_imy_close_backlight_ctrl( void );
void aud_melody_imy_vibrator_hdlr(kal_bool on);
void aud_melody_imy_led_hdlr(kal_bool on);
void aud_melody_imy_backlight_hdlr(kal_bool on);
void aud_store_driver_status(void);
void aud_restore_driver_status(void);

#ifdef __MED_MMA_MOD__
/* mma */
void mma_set_handle(kal_int32 handle);
void mma_set_status(kal_int32 status);
kal_bool aud_mma_startup(void);
void aud_mma_close_all(void);
void aud_mma_play_event_ind_hdlr(ilm_struct *ilm_ptr);
void aud_mma_open_req_hdlr(ilm_struct *ilm_ptr);
void aud_mma_close_req_hdlr(ilm_struct *ilm_ptr);
void aud_mma_play_req_hdlr(ilm_struct *ilm_ptr);
void aud_mma_stop_req_hdlr(ilm_struct *ilm_ptr);
void aud_mma_pause_req_hdlr(ilm_struct *ilm_ptr);
void aud_mma_resume_req_hdlr(ilm_struct *ilm_ptr);
void aud_mma_set_start_time_req_hdlr(ilm_struct *ilm_ptr);
void aud_mma_set_stop_time_req_hdlr(ilm_struct *ilm_ptr);
void aud_mma_set_rate_req_hdlr(ilm_struct *ilm_ptr);
void aud_mma_set_tempo_req_hdlr(ilm_struct *ilm_ptr);
void aud_mma_set_pitch_req_hdlr(ilm_struct *ilm_ptr);
void aud_mma_set_volume_req_hdlr(ilm_struct *ilm_ptr);
void aud_mma_send_long_msg_req_hdlr(ilm_struct *ilm_ptr);
void aud_mma_send_short_msg_req_hdlr(ilm_struct *ilm_ptr);
void aud_mma_open_device_req_hdlr(ilm_struct *ilm_ptr);
void aud_mma_close_device_req_hdlr(ilm_struct *ilm_ptr);
void aud_mma_set_channel_volume_req_hdlr(ilm_struct *ilm_ptr);
void aud_mma_set_program_req_hdlr(ilm_struct *ilm_ptr);
void aud_mma_set_data_req_hdlr(ilm_struct *ilm_ptr);
#endif /* __MED_MMA_MOD__ */


#ifdef VM_SUPPORT
/* vm */
kal_bool aud_vm_init(void);
kal_bool aud_vm_startup(void);
void aud_vm_main(ilm_struct *ilm_ptr);
void aud_vm_set_output_volume(kal_uint8 volume1, kal_uint8 volume2);
void aud_vm_set_output_device(kal_uint8 device);
kal_bool aud_vm_check_valid_id(kal_uint8 vm_id);
void aud_vm_play(kal_uint8 file_id, kal_uint8 play_style, kal_uint8 out_device);
void aud_vm_stop(void);
void aud_vm_get_info_req_hdlr(ilm_struct *ilm_ptr);
void aud_vm_record_req_hdlr(ilm_struct *ilm_ptr);
void aud_vm_play_req_hdlr(ilm_struct *ilm_ptr);
void aud_vm_stop_req_hdlr(ilm_struct *ilm_ptr);
void aud_vm_delete_req_hdlr(ilm_struct *ilm_ptr);
void aud_vm_abort_req_hdlr(ilm_struct *ilm_ptr);
void aud_vm_pause_req_hdlr(ilm_struct *ilm_ptr);
void aud_vm_resume_req_hdlr(ilm_struct *ilm_ptr);
void aud_vm_append_req_hdlr(ilm_struct *ilm_ptr);
void aud_vm_rename_req_hdlr(ilm_struct *ilm_ptr);
void aud_vm_get_info_req_hdlr(ilm_struct *ilm_ptr);
void aud_vm_read_data_ind_hdlr(ilm_struct *ilm_ptr);
void aud_vm_write_data_ind_hdlr(ilm_struct *ilm_ptr);


#endif


kal_bool aud_media_init(void);
void aud_media_main(ilm_struct *ilm_ptr);
void aud_media_record_req_hdlr(ilm_struct *ilm_ptr);
void aud_media_play_req_hdlr(ilm_struct *ilm_ptr);
void aud_media_stop_req_hdlr(ilm_struct *ilm_ptr);
void aud_media_pause_req_hdlr(ilm_struct *ilm_ptr);
void aud_media_resume_req_hdlr(ilm_struct *ilm_ptr);
void aud_media_store_req_hdlr(ilm_struct *ilm_ptr);
void aud_media_restore_req_hdlr(ilm_struct *ilm_ptr);
void aud_media_read_data_ind_hdlr(ilm_struct *ilm_ptr);
void aud_media_write_data_ind_hdlr(ilm_struct *ilm_ptr);
kal_uint8 aud_media_play(kal_wchar* file_name, kal_uint8 play_style, kal_uint8 first_time);
void aud_stop_unfinished_process(void);

void aud_send_fmr_power_on_req(module_type src_mod_id, kal_uint8 output_path);
void aud_send_fmr_power_off_req(module_type src_mod_id);
void aud_send_fmr_set_freq_req(module_type src_mod_id, kal_uint16 freq);
void aud_send_fmr_mute_req(module_type src_mod_id, kal_uint8 mute);
void aud_send_fmr_check_is_valid_stop_cnf(kal_uint8 is_valid);
void aud_send_fmr_get_signal_level_cnf(kal_uint8 sig_lvl);

#ifdef __MMI_SUPPORT_BACKLIGHT_SYNC__
/* -- added for turn on/off backlight by MMI task -- */
void aud_backlight_ctrl_rsp_hdlr(ilm_struct *ilm_ptr);
void aud_send_backlight_ctrl_req(module_type dst_mod_id, kal_uint8 on_off, kal_uint8 disable_timer);
#endif

#ifdef __MMI_SUPPORT_LED_SYNC__
/* -- added for turn on/off led by MMI task -- */
void aud_led_ctrl_rsp_hdlr(ilm_struct *ilm_ptr);
void aud_send_led_ctrl_req(module_type dst_mod_id, kal_uint8 on_off);
#endif

/* ilm */
void aud_send_ilm(module_type dest_id, 
                     kal_uint32 msg_id, 
                     void *local_param_ptr, 
                     void *peer_buf_ptr);
                     
void aud_send_audio_play_finish_ind(kal_uint8 result);

void aud_send_msg_to_nvram(msg_type_t msg_name, 
                                  kal_uint16 ef_id,
                                  void *data_ptr,
                                  kal_uint16 length);
void aud_send_startup_cnf(kal_uint8 result);
void aud_send_set_audio_profile_cnf(kal_uint8 result);
void aud_send_set_audio_param_cnf(kal_uint8 result);
/* for aud_api.c, to send message to media task */
void aud_send_play_id_req(module_type src_mod_id, void* id_param);
void aud_send_stop_id_req(module_type src_mod_id, kal_uint8 audio_id);
void aud_send_play_string_req(module_type src_mod_id, void* string_param);
void aud_send_stop_string_req(module_type src_mod_id, kal_uint8 src_id);
void aud_send_play_file_req(module_type src_mod_id, void* file_param);
void aud_send_stop_file_req(module_type src_mod_id, kal_uint8 src_id);
//void aud_send_record_req(module_type src_mod_id, kal_wchar* file_name, kal_uint8 format, kal_bool default_input, kal_uint8 input_source);
void aud_send_record_req(  module_type src_mod_id,   kal_wchar *file_name, kal_uint8 format,  kal_uint8 quality,    kal_bool default_input,   kal_uint8 input_source);
void aud_send_pause_req(module_type src_mod_id, kal_uint8 src_id);
void aud_send_resume_req(module_type src_mod_id, kal_uint8 src_id);
void aud_send_stop_record_req(module_type src_mod_id, kal_uint8 src_id);
void aud_send_block_req(module_type src_mod_id, kal_uint16 mod_id, kal_uint8 level);
void aud_send_set_vibrator_enabled_req( module_type src_mod_id, kal_uint8 enable );
void aud_send_set_headset_mode_output_path_req(module_type src_mod_id, kal_uint8 device);
void aud_send_store_req(module_type src_mod_id, kal_uint8 src_id);
void aud_send_restore_req(module_type src_mod_id, void* file_param);
void aud_send_set_equalizer_req( module_type src_mod_id, kal_int8* magnitude );
void aud_send_get_file_data_length_req( module_type src_mod_id, void* file_name,kal_int32 PlayProgress );
void aud_send_get_file_data_progress_req( module_type src_mod_id );
void aud_send_set_file_data_progress_req( module_type src_mod_id, kal_uint32 progress );

#ifdef __MED_MMA_MOD__
/* mma */
void aud_send_mma_open_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handler, kal_int32 param);
void aud_send_mma_close_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle);
void aud_send_mma_play_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle);
void aud_send_mma_stop_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle);
void aud_send_mma_pause_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle);
void aud_send_mma_resume_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle);
void aud_send_mma_set_start_time_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle, kal_int32 start_time);
void aud_send_mma_set_stop_time_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle, kal_int32 stop_time);
void aud_send_mma_set_rate_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle, kal_int32 rate);
void aud_send_mma_set_tempo_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle, kal_int32 tempo);
void aud_send_mma_set_pitch_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle, kal_int32 pitch);
void aud_send_mma_set_volume_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle, kal_uint8 volume);
void aud_send_mma_send_long_msg_req(module_type src_mod_id, kal_int32 handle, kal_uint8* buf, kal_uint16 len);
void aud_send_mma_send_short_msg_req(module_type src_mod_id, kal_int32 handle, kal_uint8 type, kal_uint8 data1, kal_uint8 data2);
void aud_send_mma_open_device_req(module_type src_mod_id);
void aud_send_mma_close_device_req(module_type src_mod_id, kal_int32 handle);
void aud_send_mma_set_channel_volume_req(module_type src_mod_id, kal_int32 handle, kal_uint8 channel, kal_uint8 level);
void aud_send_mma_set_program_req(module_type src_mod_id, kal_int32 handle, kal_uint8 channel, kal_uint8 bank, kal_uint8 program);
void aud_send_mma_set_data_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle, kal_int32 param);
#endif /* __MED_MMA_MOD__ */

#ifdef VM_SUPPORT

void aud_send_vm_get_info_cnf(void);
void aud_send_vm_record_cnf(kal_uint8 result);
void aud_send_vm_play_cnf(kal_uint8 result);
void aud_send_vm_stop_cnf(kal_uint8 result, kal_wchar *file_name);
void aud_send_vm_abort_cnf(kal_uint8 result);
void aud_send_vm_pause_cnf(kal_uint8 result, kal_wchar *file_name);
void aud_send_vm_resume_cnf(kal_uint8 result);
void aud_send_vm_append_cnf(kal_uint8 result);
void aud_send_vm_delete_cnf(kal_uint8 result);
void aud_send_vm_rename_cnf(kal_uint8 result);
void aud_send_vm_record_finish_ind(kal_uint8 result, kal_wchar *file_name);
void aud_send_vm_play_finish_ind(kal_uint8 result);



#endif
                     

void aud_send_media_record_cnf(kal_uint8   result);
void aud_send_media_play_cnf(kal_uint8   result);
void aud_send_media_stop_cnf(kal_uint8   result, kal_wchar    *file_name);
void aud_send_media_pause_cnf(kal_uint8   result, kal_wchar    *file_name);
void aud_send_media_resume_cnf(kal_uint8   result);
void aud_send_media_record_finish_ind(kal_uint8   result);
void aud_send_media_play_finish_ind(kal_uint8   result);

/* utility */
kal_uint8 aud_audio_out_dev_convert(kal_uint8 speaker_id);
kal_uint8 aud_get_res(kal_uint8 result);
kal_uint8 aud_check_disc_space(kal_wchar* dir_name, kal_int32 free_byte);
int aud_create_full_path_folder( kal_uint16 *filepath );
kal_bool aud_is_streaming_type(kal_uint8 format);

void aud_fmr_power_on_req_hdlr( ilm_struct *ilm_ptr );
void aud_fmr_power_off_req_hdlr( ilm_struct *ilm_ptr );
void aud_fmr_set_freq_req_hdlr( ilm_struct *ilm_ptr );
void aud_fmr_mute_req_hdlr( ilm_struct *ilm_ptr );
void aud_fmr_check_is_valid_stop_req_hdlr( ilm_struct *ilm_ptr );
void aud_fmr_get_signal_level_req_hdlr( ilm_struct *ilm_ptr );
void aud_fmr_set_volume( kal_uint8 volume1 );
void aud_fmr_set_output_device( kal_uint8 device );

/* api */
void aud_set_result(kal_int32 result);
void aud_set_pair_results(kal_int32 result, kal_uint32 value);

/* VR */
#ifdef __MED_VR_MOD__
// SD
kal_int32 aud_vr_sd_find_word_id( kal_uint16 group_id, kal_uint16 word_id );
kal_int32 aud_vr_sd_extract_group_id( kal_uint16* filename );
kal_int32 aud_vr_sd_extract_word_id( kal_uint16* filename );
void aud_vr_sd_scan_tags_in_group_folder( kal_uint16 group_id, kal_uint16* path );
void aud_vr_sd_filter_out_not_exist_tags( kal_uint16 group_id );
kal_bool aud_vr_sd_startup( void );
kal_int32 aud_vr_sd_init_rcg( void );
kal_int32 aud_vr_sd_init_trn( void );
void aud_vr_sd_rcg_process( void );
void aud_vr_sd_trn_process( void );
kal_int32 aud_vr_sd_rcg_voice_in( void );
kal_int32 aud_vr_sd_trn_voice_in( kal_uint8 seq_no );
kal_int32 aud_vr_sd_sync_db( kal_uint16 group_id, kal_uint16* id_length_p, kal_uint16* id_array );
kal_bool aud_vr_sd_delete_one_tag( kal_uint16 group_id, kal_uint16 word_id );
kal_int32 aud_vr_sd_delete_tag( kal_uint16 group_id, kal_uint16 word_id );
kal_int32 aud_vr_sd_add_tag( kal_uint16 group_id, kal_uint16* cid_array, kal_uint16 cid_length );
kal_int32 aud_vr_sd_play_tag( void );

void aud_vr_sd_abort( void );

kal_bool aud_vr_check_db_folder( kal_uint16* folder_name, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id );
kal_bool aud_vr_startup( void );
void aud_vr_main( ilm_struct *ilm_ptr );
void aud_vr_get_version_req_hdlr( ilm_struct *ilm_ptr );
void aud_vr_get_param_req_hdlr( ilm_struct *ilm_ptr );
void aud_vr_set_param_req_hdlr( ilm_struct *ilm_ptr );
void aud_vr_init_rcg_req_hdlr( ilm_struct *ilm_ptr );
void aud_vr_init_trn_req_hdlr( ilm_struct *ilm_ptr );
void aud_vr_voice_in_req_hdlr( ilm_struct *ilm_ptr );
void aud_vr_del_tag_req_hdlr( ilm_struct *ilm_ptr );
void aud_vr_check_tag_req_hdlr( ilm_struct *ilm_ptr );
void aud_vr_sync_db_req_hdlr( ilm_struct *ilm_ptr );
void aud_vr_play_tag_req_hdlr( ilm_struct *ilm_ptr );
void aud_vr_get_dir_req_hdlr( ilm_struct *ilm_ptr );
void aud_vr_add_tag_req_hdlr( ilm_struct *ilm_ptr );
void aud_vr_abort_req_hdlr( ilm_struct *ilm_ptr );
void aud_send_vr_get_version_req( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang );
void aud_send_vr_get_param_req( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_int16* sim, kal_int16* diff, kal_int16* rej );
void aud_send_vr_set_param_req( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_int16 sim, kal_int16 diff, kal_int16 rej );
void aud_send_vr_init_rcg_req( module_type src_mod_id, kal_uint32 session_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id );
void aud_send_vr_init_trn_req( module_type src_mod_id, kal_uint32 session_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 word_id );
void aud_send_vr_voice_in_req( module_type src_mod_id, kal_uint32 session_id, kal_uint8 seq_no );
void aud_send_vr_rcg_result_ind( module_type dst_mod_id, kal_uint32 session_id, kal_int32 result, kal_uint16 id_length, kal_uint16* id_array );
void aud_send_vr_trn_result_ind( module_type dst_mod_id, kal_uint32 session_id, kal_int32 result );
void aud_send_vr_del_tag_req( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 word_id );
void aud_send_vr_check_tag_req( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 word_id );
void aud_send_vr_sync_db_req( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16* id_length_p, kal_uint16* id_array );
void aud_send_vr_play_tag_req( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 word_id, kal_uint16 identifier );
void aud_send_vr_get_dir_req( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16* folder_name );
void aud_send_vr_add_tag_req( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 id_length, kal_uint16* id_array );
void aud_send_vr_abort_req( module_type src_mod_id );
void aud_send_vr_abort_ind( module_type dst_mod_id, kal_uint32 session_id );
#endif // __MED_VR_MOD__

#ifdef __MED_VRSI_MOD__

void aud_vrsi_startup(void); 
void aud_vrsi_main( ilm_struct *ilm_ptr );
void aud_vrsi_init_req_hdlr( ilm_struct *ilm_ptr ); 
void aud_vrsi_add_tags_req_hdlr( ilm_struct *ilm_ptr ); 
void aud_vrsi_start_trn_req_hdlr( ilm_struct *ilm_ptr );
void aud_vrsi_start_rcg_req_hdlr( ilm_struct *ilm_ptr ); 
void aud_vrsi_process_ind_hdlr( ilm_struct *ilm_ptr );
void aud_vrsi_play_tag_req_hdlr( ilm_struct *ilm_ptr );
void aud_vrsi_play_tts_req_hdlr( ilm_struct *ilm_ptr ); 
void aud_vrsi_get_tagnum_req_hdlr( ilm_struct *ilm_ptr ); 
void aud_vrsi_get_taginfo_req_hdlr( ilm_struct *ilm_ptr ); 
void aud_vrsi_del_tags_req_hdlr( ilm_struct *ilm_ptr ); 
void aud_vrsi_reset_tags_req_hdlr( ilm_struct *ilm_ptr );
void aud_vrsi_sync_db_req_hdlr( ilm_struct *ilm_ptr ); 
void aud_vrsi_abort_req_hdlr( ilm_struct *ilm_ptr );
void aud_vrsi_close_req_hdlr( ilm_struct *ilm_ptr ); 

void aud_vrsi_callback(VRSI_Event event); 
void aud_vrsi_prompt_cnf(void);
kal_int32 aud_vrsi_error(void); 
kal_int32 aud_vrsi_init(void); 
kal_int32 aud_vrsi_add_tags(void); 
void aud_vrsi_save_addtags_result(void); 
kal_int32 aud_vrsi_start_cmd_rcg(void); 
kal_int32 aud_vrsi_start_cmd_trn(void); 
kal_int32 aud_vrsi_start_digit_rcg(kal_uint8 lang, kal_uint16 limit); 
kal_int32 aud_vrsi_start_digit_adapt(kal_uint8 lang); 
kal_int32 aud_vrsi_play_tag(kal_uint16 tag_id); 
kal_int32 aud_vrsi_play_tts(kal_uint8 lang); 
kal_int32 aud_vrsi_get_tagnum(kal_int32 *tagNum); 
kal_int32 aud_vrsi_get_tag_info(kal_int32 tagNum, const kal_uint16 *pTagID, kal_uint16 **ppTagName, kal_int32 *pTagLong); 
kal_int32 aud_vrsi_del_tags(kal_int32 tagNum, const kal_uint16 *pTagID); 
kal_int32 aud_vrsi_reset_tags(void); 
kal_int32 aud_vrsi_sync_db(kal_uint32 tag_num, kal_uint16 *id_list, kal_uint16 **name_list, kal_uint8 *sync_flag); 
kal_int32 aud_vrsi_sync_db_by_id(kal_uint32 tag_num, kal_uint16 *id_list, kal_uint8 *sync_flag);
void aud_vrsi_abort(void); 
void aud_vrsi_close(void);      

void aud_send_vrsi_init_req(module_type src_mod_id); 
void aud_send_vrsi_add_tags_req(module_type src_mod_id, kal_uint32 session_id, kal_uint8 *grammar, kal_uint16 tag_num, kal_uint16** name_list, kal_int32 *long_list, kal_uint8 *is_name_list, kal_uint16 *id_list); 
void aud_send_vrsi_start_cmd_trn_req(module_type src_mod_id, kal_uint32 session_id, kal_uint8 *grammar, kal_uint16 *tag_name, kal_int32 tag_long); 
void aud_send_vrsi_start_cmd_rcg_req(module_type src_mod_id, kal_uint32 session_id, kal_uint8 *grammar); 
void aud_send_vrsi_start_digit_rcg_req(module_type src_mod_id, kal_uint32 session_id, kal_uint8 lang, kal_uint16 limit);
void aud_send_vrsi_start_digit_adapt_req(module_type src_mod_id, kal_uint32 session_id, kal_uint8 lang);
void aud_send_vrsi_play_tag_req(module_type src_mod_id, kal_uint32 session_id, kal_uint8 *grammar, kal_uint16 tag_id ,kal_uint8 volume, kal_uint8 output_path);
void aud_send_vrsi_play_TTS_req(module_type src_mod_id, kal_uint32 session_id, kal_uint16 *text, kal_uint8 volume, kal_uint8 output_path,  kal_uint8 lang);
void aud_send_vrsi_get_tag_num_req(module_type src_mod_id, kal_uint8 *grammar, kal_int32 *tag_num);
void aud_send_vrsi_get_tag_info_req(module_type src_mod_id, kal_uint8 *grammar, kal_int32 tag_num, kal_uint16 *id_list, kal_uint16 **name_list, kal_int32 *long_list);
void aud_send_vrsi_del_tags_req(module_type src_mod_id, kal_uint8 *grammar, kal_int32 tag_num, kal_uint16 *id_list);
void aud_send_vrsi_reset_tags_req(module_type src_mod_id, kal_uint8 *grammar);
void aud_send_vrsi_sync_db_req(module_type src_mod_id, kal_uint8 *grammar, kal_int32 tag_num, kal_uint16 *id_list, kal_uint16 **name_list, kal_uint8 *sync_flag);
void aud_send_vrsi_abort_req(module_type src_mod_id, kal_uint32 session_id);
void aud_send_vrsi_close_req(module_type src_mod_id);
void aud_send_vrsi_process_ind(module_type dst_mod_id, kal_uint32 session_id, kal_uint8 proc_type);
void aud_send_vrsi_trn_prompt_req(module_type dst_mod_id, kal_uint32 session_id, kal_uint8 pmp_mode);
void aud_send_vrsi_trn_result_ind(module_type dst_mod_id, kal_uint32 session_id, kal_uint16 tag_id);
void aud_send_vrsi_rcg_prompt_req(module_type dst_mod_id, kal_uint32 session_id, kal_uint8 pmp_mode);
void aud_send_vrsi_rcg_result_ind(module_type dst_mod_id, kal_uint32 session_id, kal_uint16 res_type);
void aud_send_vrsi_err_ind(module_type dst_mod_id, kal_uint32 session_id);
void aud_send_vrsi_play_tag_finish_ind(module_type dst_mod_id, kal_uint32 session_id);
void aud_send_vrsi_play_tts_finish_ind(module_type dst_mod_id, kal_uint32 session_id);

#endif // __MED_VRSI_MOD__

#ifdef __MED_GENERAL_TTS__
extern void aud_send_tts_set_attr_req(module_type src_mod_id, kal_uint32 attr_id, kal_uint32 attr_value);
extern void aud_send_tts_play_req(module_type src_mod_id, void *tts_param);
extern void aud_send_tts_update_info_ind(void *data);
#endif
#ifdef __MED_GENERAL_TTS__
extern void aud_tts_startup(void);
extern void aud_tts_set_attr_req_hdlr(ilm_struct *ilm_ptr);
extern void aud_tts_process_event_ind_hdlr(ilm_struct *ilm_ptr);
extern void aud_tts_play_req_hdlr(ilm_struct *ilm_ptr);
extern void aud_tts_stop_req_hdlr(ilm_struct *ilm_ptr);
extern void aud_tts_pause_req_hdlr(ilm_struct *ilm_ptr);
extern void aud_tts_resume_req_hdlr(ilm_struct *ilm_ptr);
#endif /*#ifdef __MED_GENERAL_TTS__*/
extern void aud_send_bt_open_codec_req(module_type src_mod_id, bt_a2dp_audio_cap_struct *cfg, 
    kal_uint8 stream_handle, kal_bool immediate, kal_uint32 seq_no);
extern void aud_send_bt_close_codec_req(module_type src_mod_id);
extern void aud_send_bt_close_codec_ind(module_type src_mod_id,
    kal_uint8 stream_handle, kal_uint32 seq_no);

#ifdef __MED_BT_MOD__
extern void aud_send_bt_audio_open_req(
                module_type src_mod_id,
                kal_uint8 profile,
                kal_uint8 mode,
                kal_uint16 connect_id);
extern void aud_send_bt_audio_open_cnf(module_type dst_mod_id, kal_uint8 profile, kal_uint16 result);
extern void aud_send_bt_audio_open_ind(module_type dst_mod_id, kal_uint8 profile);
extern void aud_send_bt_audio_close_req(module_type src_mod_id, kal_uint8 profile);
extern void aud_send_bt_audio_close_cnf(module_type dst_mod_id, kal_uint8 profile, kal_uint16 result);
extern void aud_send_bt_audio_close_ind(module_type dst_mod_id, kal_uint8 profile, kal_uint16 cause);
extern void aud_send_bt_audio_turn_on_req(module_type src_mod_id, kal_uint8 profile);
extern void aud_send_bt_audio_turn_off_req(module_type src_mod_id, kal_uint8 profile);
extern void aud_send_bt_avrcp_status_ind(module_type src_mod_id, kal_bool on);

#ifdef __MED_BT_HFP_MOD__
extern kal_bool aud_bt_hfp_is_audio_path_on(void);
extern void aud_bt_hfp_set_audio_path_on(void);
extern void aud_bt_hfp_set_audio_path_off(void);
extern void aud_bt_hfp_set_audio_path(kal_bool on);
#endif /* __MED_BT_HFP_MOD__ */ 

#ifdef __MED_BT_A2DP_MOD__
#if defined(__BTCOOLSAND__)
extern void aud_bt_a2dp_open_codec(kal_int32 audio_format);
extern void aud_bt_a2dp_close_codec_ind(void);
extern void aud_bt_a2dp_close_codec_for_open(void);
extern kal_bool aud_bt_a2dp_is_mute_phone(void);
extern kal_bool aud_bt_a2dp_is_codec_opened(void);
#endif
#endif /* __MED_BT_A2DP_MOD__ */

#endif /* __MED_BT_MOD__ */ 

#ifdef MAGIC_AE_SUPPORT
void aud_audio_ae_req_hdlr(ilm_struct *ilm_ptr);
#endif

#define AUD_FULL_PROGRESS_CNT (9999)

#endif /* _AUD_H */

#endif /* MED_NOT_PRESENT */

