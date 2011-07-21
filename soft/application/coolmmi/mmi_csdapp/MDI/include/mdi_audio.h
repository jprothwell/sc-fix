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
 *	mdi_audio.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Audio interfce header file
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
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#ifndef _MDI_AUDIO_H_
#define _MDI_AUDIO_H_

//added by panxu 20061222
#include "mdi_datatype.h"
#include "stack_config.h"
#ifndef _MDI_DATATYPE_H_
#error "Must include mdi_datatype.h before include this file"
#endif
/*shizh for FMRadio*/
typedef enum
{
	MDI_BACKGROUND_APP_POC,
	MDI_BACKGROUND_APP_FMR,
	MDI_BACKGROUND_APP_AUDPLY,
	MDI_BACKGROUND_APP_TOTAL
}MDI_BACKGROUND_APP;


enum {
	MDI_AUDIO_SUCCESS = 0,              /* 0 */
   MDI_AUDIO_FAIL,                     /* 1 */
   MDI_AUDIO_BUSY,                     /* 2 */
   MDI_AUDIO_DISC_FULL,                /* 3 */
   MDI_AUDIO_OPEN_FILE_FAIL,           /* 4 */
   MDI_AUDIO_END_OF_FILE,	               /* 5 */
   MDI_AUDIO_TERMINATED,               /* 6 */    
   MDI_AUDIO_BAD_FORMAT,               /* 7 */
   MDI_AUDIO_INVALID_FORMAT,           /* 8 */
   MDI_AUDIO_ERROR,                    /* 9 */
   MDI_AUDIO_NO_DISC,                  /* 10 */
   MDI_AUDIO_NO_SPACE,   	            /* 11 */
   MDI_AUDIO_INVALID_HANDLE,         /* 12 */
   MDI_AUDIO_NO_HANDLE,             /* 13 */
   MDI_AUDIO_RESUME,                /* 14 */
   MDI_AUDIO_BLOCKED,               /* 15 */
   MDI_AUDIO_MEM_INSUFFICIENT,      /* 16 */ 
   MDI_AUDIO_BUFFER_INSUFFICIENT,   /* 17 */
   MDI_AUDIO_FILE_EXIST,               /* 18 */
   MDI_AUDIO_WRITE_PROTECTION,         /* 19 */
   MDI_AUDIO_PARAM_ERROR,	                /* 20 */
   /* MP4,AAC */
   MDI_AUDIO_UNSUPPORTED_CHANNEL,	         /* 21 */
   MDI_AUDIO_UNSUPPORTED_FREQ,
   MDI_AUDIO_UNSUPPORTED_TYPE,
   MDI_AUDIO_UNSUPPORTED_OPERATION,
   MDI_AUDIO_PARSER_ERROR,
   MDI_AUDIO_VIDEO_ERROR,
   MDI_AUDIO_AUDIO_ERROR,
   MDI_AUDIO_FSAL_ERROR,
   MDI_AUDIO_MP4_PRODUCER_ERROR,
   MDI_AUDIO_MP4_SAMPLEDATA_ERROR,
   MDI_AUDIO_MP4_NO_VIDEO_TRACK,
   MDI_AUDIO_MP4_NO_AUDIO_TRACK,
   /* VR */
   MDI_AUDIO_ID_MISMATCH,	           /* 33 */
   MDI_AUDIO_ID_EXIST,
   MDI_AUDIO_TRAINING_CONTINUE,
   MDI_AUDIO_NO_SOUND,
   MDI_AUDIO_TOO_SIMILAR,
   MDI_AUDIO_TOO_DIFFERENT,
   MDI_AUDIO_NO_MATCH,
   MDI_AUDIO_SPEAK_TOO_SHORT,
   MDI_AUDIO_SPEAK_TOO_LONG,
   /* FM Record */
   MDI_AUDIO_STOP_FM_RECORD,

	MDI_NO_OF_EVENT
};

enum{
   JDI_AUDIO_SUCCESS = 0,              /* 0 */
   JDI_AUDIO_FAIL,                     /* 1 */
   JDI_AUDIO_BUSY,                     /* 2 */
   JDI_AUDIO_DISC_FULL,                /* 3 */
   JDI_AUDIO_OPEN_FILE_FAIL,           /* 4 */
   JDI_AUDIO_END_OF_FILE,	               /* 5 */
   JDI_AUDIO_TERMINATED,               /* 6 */    
   JDI_AUDIO_BAD_FORMAT,               /* 7 */
   JDI_AUDIO_INVALID_FORMAT,           /* 8 */
   JDI_AUDIO_ERROR,                    /* 9 */
   JDI_AUDIO_NO_DISC,                  /* 10 */
   JDI_AUDIO_NO_SPACE,   	            /* 11 */
   JDI_AUDIO_INVALID_HANDLE,         /* 12 */
   JDI_AUDIO_NO_HANDLE,             /* 13 */
   JDI_AUDIO_RESUME,                /* 14 */
   JDI_AUDIO_BLOCKED,               /* 15 */
   JDI_AUDIO_MEM_INSUFFICIENT,      /* 16 */ 
   JDI_AUDIO_BUFFER_INSUFFICIENT,   /* 17 */
   JDI_AUDIO_FILE_EXIST,               /* 18 */
   JDI_AUDIO_WRITE_PROTECTION,         /* 19 */
   JDI_AUDIO_PARAM_ERROR,	                /* 20 */
   /* MP4,AAC */
   JDI_AUDIO_UNSUPPORTED_CHANNEL,	         /* 21 */
   JDI_AUDIO_UNSUPPORTED_FREQ,
   JDI_AUDIO_UNSUPPORTED_TYPE,
   JDI_AUDIO_UNSUPPORTED_OPERATION,
   JDI_AUDIO_PARSER_ERROR,
   JDI_AUDIO_VIDEO_ERROR,
   JDI_AUDIO_AUDIO_ERROR,
   JDI_AUDIO_FSAL_ERROR,
   JDI_AUDIO_MP4_PRODUCER_ERROR,
   JDI_AUDIO_MP4_SAMPLEDATA_ERROR,
   JDI_AUDIO_MP4_NO_VIDEO_TRACK,
   JDI_AUDIO_MP4_NO_AUDIO_TRACK,
   /* VR */
   JDI_AUDIO_ID_MISMATCH,	           /* 33 */
   JDI_AUDIO_ID_EXIST,
   JDI_AUDIO_TRAINING_CONTINUE,
   JDI_AUDIO_NO_SOUND,
   JDI_AUDIO_TOO_SIMILAR,
   JDI_AUDIO_TOO_DIFFERENT,
   JDI_AUDIO_NO_MATCH,
   JDI_AUDIO_SPEAK_TOO_SHORT,
   JDI_AUDIO_SPEAK_TOO_LONG,
   
	JDI_NO_OF_EVENT
};

enum{
   MDI_AUDIO_IDLE,
   MDI_AUDIO_PLAY,
   MDI_AUDIO_PLAY_PAUSED,
   MDI_AUDIO_RECORD,
   MDI_AUDIO_RECORD_PAUSED,
   MDI_AUDIO_SPEECH_ON,
   MDI_AUDIO_VRSI_PROCESS,
   MDI_NO_AUDIO_STATE
};

enum{
   JDI_AUDIO_IDLE,
   JDI_AUDIO_PLAY,
   JDI_AUDIO_PLAY_PAUSED,
   JDI_AUDIO_RECORD,
   JDI_AUDIO_RECORD_PAUSED,
   JDI_AUDIO_SPEECH_ON,
   JDI_AUDIO_VRSI_PROCESS,
   JDI_NO_AUDIO_STATE
};

enum{
   MDI_FORMAT_NONE=-1,
   MDI_FORMAT_GSM_FR,                /* 0 */
   MDI_FORMAT_GSM_HR,               /* 1 */
   MDI_FORMAT_GSM_EFR,               /* 2 */
   MDI_FORMAT_AMR,                      /* 3 */
   MDI_FORMAT_AMR_WB,                  /* 4 */
   MDI_FORMAT_DAF,                     /* 5 */
   MDI_FORMAT_AAC,                     /* 6 */
   MDI_FORMAT_PCM_8K,                  /* 7 */
   MDI_FORMAT_PCM_16K,                 /* 8 */
   MDI_FORMAT_G711_ALAW,            /* 9 */
   MDI_FORMAT_G711_ULAW,            /* 10 */
   MDI_FORMAT_DVI_ADPCM,            /* 11 */
   MDI_FORMAT_VR,                   /* 12 */
   MDI_FORMAT_WAV,                  /* 13 */
   MDI_FORMAT_WAV_ALAW,             /* 14 */
   MDI_FORMAT_WAV_ULAW,              /* 15 */
   MDI_FORMAT_WAV_DVI_ADPCM,       /* 16 */
   MDI_FORMAT_SMF,                   /* 17 */
   MDI_FORMAT_IMELODY,            /* 18 */
   MDI_FORMAT_SMF_SND,              /* 19 */
   MDI_FORMAT_MMF,                 /* 20 */
   MDI_FORMAT_AU,                 /*  21 */
   MDI_FORMAT_AIFF,                 /*  22 */
   MDI_FORMAT_M4A,                   /*  23 */
   MDI_FORMAT_3GP,                     /* 24 */ 
   MDI_FORMAT_MP4,	                  /* 25 */
   MDI_FORMAT_JPG,	                     /* 26 */
   MDI_FORMAT_GIF,	                     /* 27 */
   MDI_FORMAT_MJPG,	                  /* 28 */
   MDI_FORMAT_WMA,                  /* 29 */
   MDI_FORMAT_MID,                  /* 30 */
   MDI_NO_OF_FORMAT
};
#ifdef __GENERAL_TTS__
typedef enum
{
    MDI_TTS_STR_TEXT,		/*regard input as normal text */
	MDI_TTS_STR_PHN_SBL,	/*regard input as phonetic symbol */
    MDI_TTS_STR_CHAR		/*regard input as character set*/
} MDI_AUD_TTS_STR_TYPE;

typedef struct {
    U16 *text_string;
    U16 str_type;	/* String Type */
    U16 lang;	    /* Language */
    U16 app_type;	/* Application Type */
    U16 gander;	    /* Gender */
    U8 volume;	    /* Volume, from 0~6 */
    U8 path;		/* Audio Path */
    U8 pitch;	    /* Audio Pitch, from 0~100 */
    U8 speed;	    /* Audio Speed, from 0~100 */
    mdi_callback callback;
} mdi_audio_tts_struct;
#endif /*__GENERAL_TTS__*/

#define MDI_AUD_MODE_NORMAL        0   /* Normal Mode */
#define MDI_AUD_MODE_HEADSET        1   /* HeadSet (Earphone) Mode */
#define MDI_AUD_MODE_LOUDSPK        2   /* Loudspeaker Mode */
#define MDI_MAX_AUD_MODE_NUM      3   

#define MDI_VOLUME_CTN                0  /* MMI can apply to associate volume; call tone attribute */
#define MDI_VOLUME_KEY                1  /* MMI can apply to associate volume; keypad tone attribute */
#define MDI_VOLUME_MIC                2  /* microphone attribute */
#define MDI_VOLUME_FMR                3  /* MMI can apply to FM radio attribute */
#define MDI_VOLUME_SPH                4  /* MMI can apply to associate volume; speech sound attribute */
#define MDI_VOLUME_SID				      5  /* side tone attribute */
#define MDI_VOLUME_MEDIA            6  /* MMI can apply to associate volume; As MP3, Wave,... attribute */
#define MDI_VOLUME_VIDEO            7  /* MMI can apply to associate volume; As MP3, Wave,... attribute */
#define MDI_MAX_VOLUME_TYPE         8

#define MDI_MAX_VOLUME_LEVEL            7

#define MDI_DEVICE_SPEAKER            0 /* Tone, Keypad sound to play on audio device. */
#define MDI_DEVICE_MICROPHONE          1 /* Microphone sound to send on audio device. */
#define MDI_DEVICE_SPEAKER2            4 /* earphone, carkit */
#define MDI_DEVICE_LOUDSPEAKER         5 /* loudspeaker for free sound */
#define MDI_DEVICE_SPEAKER_BOTH        6
#define MDI_DEVICE_BT_HEADSET 8 /* used to be OR bitwise with above audio output devices */

#define MDI_AUDIO_PLAY_CRESCENDO    0 /* Play sound for crescendo. */
#define MDI_AUDIO_PLAY_INFINITE     1 /* Play sound for infinite. */
#define MDI_AUDIO_PLAY_ONCE         2 /* Play sound for once. */
#define MDI_AUDIO_PLAY_DESCENDO     3 /* Play sound for descendo. */

#define MDI_INPUT_MIC1	0
#define MDI_INPUT_MIC2  1
#define MDI_INPUT_FMRR  2 

/* Exclude 1st(16-level) and 2nd(Mute) bit to get actual volume level*/
#define MDI_AUD_VOL_GET_EX_LEVEL(x)     ((x) & 0x3F)
/* Is 16 Level*/
#define MDI_AUD_VOL_IS_EX_LEVEL(x)      ((x) & 0x80)
/* Is Level 0 is mute */
#define MDI_AUD_VOL_IS_MUTE_LEVEL(x)    ((x) & 0x40)

/* 16 Level*/
#define MDI_AUD_VOL_EX(x)       ((x) | 0x80)
/* 7 Level, Level 0 is mute (2nd bit on), the volume level is 0 ~ 7 */
#define MDI_AUD_VOL_MUTE(x)     ((x) | 0x40)
/* 16 Level, Level 0 is mute(1st , 2nd bit on), the volume level is total 0 ~ 16 */
#define MDI_AUD_VOL_EX_MUTE(x)  ((x) | 0xC0)
/* Audio path macro */
#define MDI_AUD_PTH_EX(x) (x)

/* 7 Level */
#define MDI_AUD_VOL_LEVEL_NORMAL        1
/* 7 Level + Level 0 mute */
#define MDI_AUD_VOL_LEVEL_NORMAL_MUTE   2
/* 16 Level */
#define MDI_AUD_VOL_LEVEL_EXTEND        3
/* 16 Level + Level 0 mute */
#define MDI_AUD_VOL_LEVEL_EXTEND_MUTE   4
/*
*  TTS defines
*/
#ifdef __GENERAL_TTS__
#define	MDI_TTS_LANG_ENG	0x1;	/*English*/
#define	MDI_TTS_LANG_TWN	0x2;	/*Taiwan-accent Chinese*/
#define	MDI_TTS_LANG_MDN	0x4;	/*Mandarin*/

#define	MDI_TTS_TYPE_GENERAL	0x1;	/*General Purpose TTS*/
#define	MDI_TTS_TYPE_DIC		0x2;	/*Dictionary*/
#define	MDI_TTS_TYPE_NAME		0x4;	/*Name*/
#define	MDI_TTS_TYPE_GPS		0x8;	/*GPS, traffic guiding*/
#define	MDI_TTS_TYPE_WEATHER	0x10;	/*weather report*/
#define	MDI_TTS_TYPE_STOCK		0x20;	/*stock report*/

#define	MDI_TTS_GENDER_MALE	0x1;
#define	MDI_TTS_GENDER_FEMALE	0x2;
#define	MDI_TTS_GENDER_BOY		0x4
#define	MDI_TTS_GENDER_GIRL	0x8;
#endif /*__GENERAL_TTS__*/
/************ function prototype ***************/

void mdi_audio_init(void);
void jdi_audio_init(void);
#ifdef VR_ENABLE
mdi_result mdi_audio_play_voice_tag( U8 mode, U8 lang, U8 group_id, U16 word_id, mdi_handle* handle_p, mdi_callback handler );
mdi_result mdi_audio_start_vr_voice_record( U32 session_id, U8 seq_no, mdi_handle* handle_p, mdi_callback handler );
mdi_result mdi_audio_stop_vr_voice_record( void );
U32 mdi_audio_vr_rcg_session_id( void );
U32 mdi_audio_vr_trn_session_id( void );
U16 mdi_audio_vr_rcg_result_id_length( void );
U16* mdi_audio_vr_rcg_result_id_array( void );
void mdi_audio_vr_rcg_result_ind( void* inMsg );
void mdi_audio_vr_trn_result_ind( void* inMsg );
void mdi_audio_vr_abort( void );
#endif
mdi_result mdi_audio_play_id(U16 audio_id, U8 play_style, mdi_handle* handle_p, mdi_callback handler);
mdi_result mdi_audio_play_id_with_vol_path(U16 audio_id, 
                                                U8 play_style, 
                                                mdi_handle* handle_p, 
                                                mdi_callback handler,
                                                U8 volume,
                                                U8 path);
mdi_result mdi_audio_play_string(void* audio_data, U32 len, U8 format, U8 play_style, mdi_handle* handle_p, mdi_callback handler);
mdi_result mdi_audio_play_string_with_vol_path(void* audio_data, 
                                                U32 len, 
                                                U8 format, 
                                                U8 play_style, 
                                                mdi_handle* handle_p, 
                                                mdi_callback handler,
                                                U8 volume,
                                                U8 path);
mdi_result mdi_audio_play_string_with_vol_path_non_block(void* audio_data, 
                                                U32 len, 
                                                U8 format, 
                                                U8 play_style, 
                                                mdi_handle* handle_p, 
                                                mdi_callback handler,
                                                U8 volume,
                                                U8 path);
mdi_result mdi_audio_play_file(void* file_name, U8 play_style, mdi_handle* handle_p, mdi_callback handler);
jdi_result jdi_audio_play_file(void* file_name, int java_player, jdi_callback handler);
mdi_result mdi_audio_play_file_with_vol_path(void* file_name, 
                                                U8 play_style, 
                                                mdi_handle* handle_p, 
                                                mdi_callback handler,
                                                U8 volume,
                                                U8 path,
                                                U8 StartOffset);
#if defined( DIGIT_TONE_SUPPORT ) || defined(SIMPLE_TTS)
void mdi_audio_play_text_event_handler( mdi_result result );
mdi_result mdi_audio_play_text_with_vol_path( U8* words,
							S32 len,
							S32 msecs,
							mdi_handle* handle_p,
							mdi_callback handler,
							U8 volume,
							U8 path );
#endif
mdi_result mdi_audio_store_file( mdi_handle audio_handle );
mdi_result mdi_audio_restore_file_with_vol_path(void* file_name, 
                                                U8 play_style, 
                                                mdi_handle* handle_p, 
                                                mdi_callback handler,
                                                U8 volume,
                                                U8 path);
mdi_result mdi_audio_stop_id(U16 audio_id);
mdi_result mdi_audio_stop_string(void);
mdi_result mdi_audio_stop_file(void);
jdi_result jdi_audio_stop_file(void);
mdi_result mdi_audio_stop_all(void);
mdi_result mdi_audio_stop_fmr(void);
mdi_result mdi_audio_pause(mdi_handle audio_handle, mdi_callback handler);
jdi_result jdi_audio_pause(jdi_callback handler);
mdi_result mdi_audio_resume(mdi_handle audio_handle, mdi_callback handler);
jdi_result jdi_audio_resume(jdi_callback handler);
mdi_result mdi_audio_start_record(void* file_name, U8 format, mdi_handle* handle_p, mdi_callback handler);
mdi_result mdi_audio_start_record_with_quality(void* file_name, U8 format,  U8 quality, mdi_handle* handle_p, mdi_callback handler);
mdi_result mdi_audio_stop_record(void);
jdi_result jdi_audio_start_record(void* file_name, U8 format, mdi_handle* handle_p, mdi_callback handler);
jdi_result jdi_audio_stop_record(void);
void mdi_audio_resume_background_play(void);
void mdi_audio_suspend_background_play(void);
mdi_result mdi_audio_set_background_handler(U8 level, mdi_bg_callback handler);
mdi_result mdi_audio_reset_background_handler(U8 level);
void mdi_audio_set_volume(U8 type, U8 volume);
void mdi_audio_set_volume_no_apply(U8 type, U8 volume);
void mdi_audio_set_mic_volume( void );
void mdi_audio_set_speech_mode(BOOL on);
void mdi_audio_set_mute(U8 vol_type, BOOL mute);
#if defined (__BT_ACCEL_SUPPORT__) || defined(__MMI_BT_SUPPORT__)
void mdi_audio_set_bluetooth_path(BOOL on);
#endif
#ifdef MMI_ON_HARDWARE_P
BOOL mdi_audio_block(module_type mod_id);
#else
BOOL mdi_audio_block(int mod_id);
#endif
void mdi_audio_unblock(void);
void mdi_audio_set_headset_mode_output_path( unsigned char device );
mdi_result mdi_audio_set_vibrator_enabled( BOOL enable );
mdi_result mdi_audio_set_equalizer( signed char* magnitude );
mdi_state mdi_audio_get_state(void);
BOOL mdi_audio_is_background_play_suspended(void);
BOOL mdi_audio_is_idle(void);
BOOL mdi_audio_is_speech_mode( void );
BOOL mdi_audio_is_blocked(void);
mdi_result mdi_audio_get_file_time_duration( void* file_name,U32 PlayProgress,U32* duration );
mdi_result mdi_audio_get_file_data_progress( U32* progress );
U32 mdi_audio_set_file_data_progress( U32 progress );
mdi_result mdi_audio_play_file_portion_with_vol_path( void* file_name, 
	U32 start_offset, U32 end_offset,
	U8 play_style, 
	mdi_handle* handle_p, 
	mdi_callback handler,
	U8 volume,
	U8 path );
mdi_result mdi_audio_restore_file_portion_with_vol_path( void* file_name, 
	U32 start_offset, U32 end_offset,
	U8 play_style, 
	mdi_handle* handle_p, 
	mdi_callback handler,
	U8 volume,
	U8 path );
mdi_handle mdi_audio_mma_open_midi(void* audio_data, 
                                   U32 len, 
                                   U8 repeats, 
                                   mdi_mma_callback handler);

mdi_result mdi_audio_mma_close_midi(mdi_handle handle);
mdi_result mdi_audio_mma_play_midi(mdi_handle handle);
mdi_result mdi_audio_mma_stop_midi(mdi_handle handle);
mdi_result mdi_audio_mma_set_midi_volume(mdi_handle handle, kal_uint8 volume);

mdi_handle mdi_audio_mma_open_wav(void* audio_data, 
                                   U32 len, 
                                   U8 repeats, 
                                   mdi_mma_callback handler);

mdi_result mdi_audio_mma_close_wav(mdi_handle handle);
mdi_result mdi_audio_mma_play_wav(mdi_handle handle);
mdi_result mdi_audio_mma_stop_wav(mdi_handle handle);
mdi_result mdi_audio_mma_set_wav_volume(mdi_handle handle, kal_uint8 volume);


 
// functions in blocking mode
void mdi_fmr_power_on_with_path( U8 path, mdi_callback handler );
void mdi_fmr_power_off( void );
void mdi_fmr_set_freq( U16 freq );
void mdi_fmr_mute( U8 mute );

// functions in non-blocking mode
void mdi_fmr_check_is_valid_stop( U16 freq, BOOL is_step_up, void (*callback_func)(BOOL is_valid) );
void mdi_fmr_cancel_checking_is_valid_stop( void );
void mdi_fmr_get_signal_level( U16 freq, BOOL is_step_up, void (*callback_func)(U8 sig_lvl) );
void mdi_fmr_cancel_getting_signal_level( void );
 

// FM Radio Record Begin
#if 1 /*FM_RADIO_RECORD*/
mdi_result mdi_fmr_start_record(void* file_name, U8 format, mdi_handle* handle_p, mdi_callback handler);
void mdi_fmr_record_finish_ind(void* inMsg);
mdi_result mdi_audio_stop_fmr_record_only( void );
void mdi_fmr_stop_record( void );
mdi_result mdi_fmr_pause_record(mdi_callback handler);
mdi_result mdi_frm_resume_record(mdi_callback handler);
#endif
// FM Radio Record END



#ifdef VRSI_ENABLE

void mdi_audio_vrsi_trn_prompt_req_hdlr(void* inMsg);
void mdi_audio_vrsi_trn_result_ind(void* inMsg);
void mdi_audio_vrsi_rcg_prompt_req_hdlr(void* inMsg);
void mdi_audio_vrsi_rcg_result_ind(void* inMsg);
void mdi_audio_vrsi_err_ind(void* inMsg);
mdi_result mdi_audio_vrsi_init(mdi_callback handler);
U32 mdi_audio_vrsi_new_session_id(void);
void mdi_audio_vrsi_stop(U32 session_id);
void mdi_audio_vrsi_close(void);
void mdi_audio_vrsi_play_tag_finish_ind(void* inMsg);
mdi_result mdi_audio_vrsi_play_tag(U32 session_id, U8 *grammar, U16 tag_id, U8 volume, U8 output_path); 
void mdi_audio_vrsi_play_tts_finish_ind(void* inMsg);
mdi_result mdi_audio_vrsi_play_TTS(U32 session_id, U16 *text, U8 volume, U8 output_path, U8 lang);
void mdi_audio_vrsi_register_record(void); 
void mdi_audio_vrsi_unregister_record(void);

#endif // VRSI_ENABLE
#if defined (__MMI_A2DP_SUPPORT__)   || defined( __MMI_HFP_SUPPORT__)

extern void mdi_audio_bt_init(U8 profile, mdi_bt_callback open_handler, mdi_bt_callback close_handler);
extern U8 mdi_audio_bt_get_stream_status(U8 profile);
extern void mdi_audio_bt_close_stream_ind(void *msg);
extern void mdi_audio_bt_close_stream_cnf(void *msg);
extern void mdi_audio_bt_open_stream_ind(void *msg);
extern void mdi_audio_bt_open_stream_cnf(void *msg);
extern BOOL mdi_audio_bt_open_stream(U8 profile, U16 connect_id);
extern BOOL mdi_audio_bt_close_stream(U8 profile);
extern BOOL mdi_audio_bt_is_stream_open(U8 profile);
extern void mdi_audio_bt_hfp_turn_on(void);
extern void mdi_audio_bt_hfp_turn_off(void);
extern BOOL mdi_audio_bt_is_a2dp_codec_open(void);

typedef enum
{
    MDI_AUDIO_BT_STATE_IDLE,
    MDI_AUDIO_BT_STATE_WAIT_OPEN,
    MDI_AUDIO_BT_STATE_OPEN,
    MDI_AUDIO_BT_STATE_WAIT_CLOSE
} MDI_AUDIO_BT_STATE_ENUM;


#endif

void TurnOffBacklight(void);

/* General purpose TTS */
#ifdef __GENERAL_TTS__
extern mdi_result mdi_audio_tts_set_attr(U32 attr_id, U32 attr_value);
extern mdi_result mdi_audio_tts_play(mdi_audio_tts_struct tts_param);
extern mdi_result mdi_audio_tts_stop(void);
extern mdi_result mdi_audio_tts_pause(void);
extern mdi_result mdi_audio_tts_resume(void);
#endif /*__GENERAL_TTS__*/
#ifdef MAGIC_AE_SUPPORT
 void mdi_audio_ae_process(UINT32  *Handle,  INT16 *pData, INT32 dataLen, void (*ae_callback)(void));
#endif

#endif /* _MDI_AUDIO_H_ */

