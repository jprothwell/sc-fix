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
 * RESOURCE_AUDIO.H
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for audio resource database.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "cswtype.h"

#ifndef _MMI_AUDIO_H
#define _MMI_AUDIO_H

typedef enum
{
   MIN_TONE_ID = 0,                         
   TONE_DTMF_0 = MIN_TONE_ID,            	/* 0 */
   TONE_DTMF_1,            					/* 1 */
   TONE_DTMF_2,            					/* 2 */
   TONE_DTMF_3,            					/* 3 */
   TONE_DTMF_4,            					/* 4 */
   TONE_DTMF_5,            					/* 5 */
   TONE_DTMF_6,            					/* 6 */
   TONE_DTMF_7,            					/* 7 */
   TONE_DTMF_8,            					/* 8 */
   TONE_DTMF_9,								/* 9 */
   TONE_KEY_STAR,          					/* 10 */
   TONE_KEY_HASH,          					/* 11 */
   TONE_KEY_NORMAL,							/* 12 */
   TONE_KEY_VOL_UP = TONE_KEY_NORMAL,     /* 12 */   
   TONE_KEY_VOL_DOWN = TONE_KEY_NORMAL,  /* 12 */   
   TONE_KEY_UP = TONE_KEY_NORMAL,         /* 12 */   
   TONE_KEY_DOWN = TONE_KEY_NORMAL,      /* 12 */   
   TONE_KEY_LEFT = TONE_KEY_NORMAL,       /* 12 */   
   TONE_KEY_RIGHT = TONE_KEY_NORMAL,     /* 12 */    
   TONE_KEY_MENU = TONE_KEY_NORMAL,      /* 12 */    
   TONE_KEY_FUNCTION = TONE_KEY_NORMAL,  /* 12 */    
   TONE_KEY_SK_LEFT = TONE_KEY_NORMAL,   /* 12 */    
   TONE_KEY_SK_RIGHT = TONE_KEY_NORMAL,  /* 12 */    
   TONE_KEY_SEND = TONE_KEY_NORMAL,      /* 12 */    
   TONE_KEY_END = TONE_KEY_NORMAL,       /* 12 */    
   TONE_KEY_POWER = TONE_KEY_NORMAL,     /* 12 */   

   TONE_KEY_CLICK,						/* 13 */

   TONE_CALL_CONNECT,					/* 14 */
   TONE_MT_SMS_IN_CALL,					/* 15 */
   TONE_CALL_TIME_REMINDER,				/* 16 */

   TONE_DIAL_CALL_GSM,    				/* 17 */
   TONE_DIAL_CALL_PCS,     				/* 18 */
   TONE_BUSY_CALL_GSM,    				/* 19 */
   TONE_BUSY_CALL_PCS,    				/* 20 */
   TONE_CONGESTION_GSM,  				/* 21 */ 
   TONE_CONGESTION_PCS,   				/* 22 */
   TONE_RADIO_PATH,       				/* 23 */
   TONE_RADIO_NA,         				/* 24 */
   TONE_ERROR_INFO,        				/* 25 */
   TONE_NUM_UNOBTAIN,     				/* 26 */
   TONE_AUTH_ERROR,       				/* 27 */
   TONE_CALL_WAITING,      				/* 28 */
   TONE_RINGING_CALL_GSM, 				/* 29 */
   TONE_RINGING_CALL_PCS, 				/* 30 */
   TONE_POSITIVE_ACK,      				/* 31 */
   TONE_NEGATIVE_ACK,     				/* 32 */   
													
   TONE_DIAL_CALL_GSM_STK,				/* 33 */
   TONE_DIAL_CALL_PCS_STK,				/* 34 */
   TONE_BUSY_CALL_GSM_STK,				/* 35 */
   TONE_BUSY_CALL_PCS_STK,				/* 36 */
   TONE_CONGESTION_GSM_STK,				/* 37 */
   TONE_CONGESTION_PCS_STK,				/* 38 */
   TONE_RADIO_PATH_STK,   				/* 39 */
   TONE_RADIO_NA_STK,     				/* 40 */
   TONE_ERROR_INFO_STK,   			    /* 41 */
   TONE_NUM_UNOBTAIN_STK, 				/* 42 */
   TONE_AUTH_ERROR_STK,   				/* 43 */
   TONE_CALL_WAITING_STK, 				/* 44 */
   TONE_RINGING_CALL_GSM_STK,			/* 45 */
   TONE_RINGING_CALL_PCS_STK,			/* 46 */
   TONE_POSITIVE_ACK_STK, 				/* 47 */
   TONE_NEGATIVE_ACK_STK, 				/* 48 */
			
			
   TONE_WARNING1,						/* 49 */
   TONE_WARNING2,						/* 50 */
   TONE_ERROR1,							/* 51 */
   TONE_ERROR2,							/* 52 */
   TONE_GENERAL_BEEP,					/* 53 */
   TONE_BATTERY_LOW,					/* 54 */
   TONE_BATTERY_WARNING,				/* 55 */
   TONE_AUX_IND,						/* 56 */
   TONE_DATA_CALL,						/* 57 */

   MAX_TONE_ID = 80,

   MIN_EMS_SND_ID = 81,							/* 81 */
   EMS_CHIMES_HIGH = MIN_EMS_SND_ID,			/* 81 */
   EMS_CHIMES_LOW,								/* 82 */
   EMS_DING,									/* 83 */
   EMS_TADA,									/* 84 */
   EMS_NOTIFY,									/* 85 */
   EMS_DRUM,									/* 86 */
   EMS_CLAPS,									/* 87 */
   EMS_FANFAR,									/* 88 */
   EMS_CHORE_HGH,								/* 89 */
   EMS_CHORE_LOW,								/* 90 */
   MAX_EMS_SND_ID = EMS_CHORE_LOW				/* 90 */
   
}audio_id_enum;





#define MIDI_FILE_EXT   ".mid"
#define IMY_FILE_EXT    ".imy"
#define VM_FILE_EXT     ".vm"

/*
** Define the tone format: Keypad, DTMF, Call Tone.
*/
typedef struct {
   kal_uint16   freq1;            /* First frequency                                    */
   kal_uint16   freq2;            /* Second frequency                                   */
   kal_uint16   on_duration;      /* Tone on duation, in ms unit, 0 for continuous tone */
   kal_uint16   off_duration;     /* Tone off duation, in ms unit, 0 for end of playing */
   kal_int8     next_operation;   /* Index of the next tone                             */
} device_tones_struct;

typedef struct {
   const unsigned int *data; //add by panxu 20061222
   kal_uint32  len;
   kal_int32 format;
}
audio_resource_struct;

typedef struct {
	short start;
	short end;
}
start_end_t;
       
extern device_tones_struct** resource_tones;
extern audio_resource_struct* resource_midis;
extern audio_resource_struct* resource_sounds;
extern audio_resource_struct* resource_message_sounds;
extern audio_resource_struct* resource_poweronoff_sounds;
extern audio_resource_struct* resource_imelodys;
extern audio_resource_struct* resource_ems_imelodys;
extern audio_resource_struct* resource_ems_sounds;
extern audio_resource_struct* resource_mms_sounds;
extern audio_resource_struct* resource_fng_sounds;   //Added by jinzh:20070724
extern audio_resource_struct* resource_human_voice_tones;
extern audio_resource_struct* resource_tts_sounds;
/*Jinzh Add Start For 5754 Ver: TBM780  on 2007-8-8 17:2 */
extern audio_resource_struct* resource_camera_sounds;   
/*Jinzh Add End  For 5754 Ver: TBM780  on 2007-8-8 17:2 */
extern start_end_t* TTS_UCS2_MSB_index;
extern unsigned char* TTS_UCS2_LSB;
extern unsigned short* TTS_index_table;
extern audio_resource_struct* resource_keypad_tones;

void custom_get_fixed_imelody_range(kal_uint8 *min_id, kal_uint8 *max_id);
void custom_get_fixed_sound_range(kal_uint8 *min_id, kal_uint8 *max_id);
void custom_get_fixed_midi_range(kal_uint8 *min_id, kal_uint8 *max_id);
void custom_get_fixed_message_sound_range(kal_uint8 *min_id, kal_uint8 *max_id);
void custom_get_fixed_ems_imelody_range(kal_uint8 *min_id, kal_uint8 *max_id);
void custom_get_fixed_mms_sound_range(kal_uint8 *min_id, kal_uint8 *max_id);

/*********************************************************
 *                                                       *
 *  Define which audio groups in custpack bin            *
 *                                                       *
 *  Notice: if a audio group is defined in custpack bin, *
 *          it must have at least one sound              *
 *                                                       *
 ********************************************************/
//#define IMY_IN_CUSTPACK_BIN
//#define EMS_IMY_IN_CUSTPACK_BIN
#define MIDI_IN_CUSTPACK_BIN
//#define SND_IN_CUSTPACK_BIN
//#define MSG_SND_IN_CUSTPACK_BIN
//#define EMS_SND_IN_CUSTPACK_BIN
//#define MMS_SND_IN_CUSTPACK_BIN
//#define HUMAN_VOICE_IN_CUSTPACK_BIN
//#define CUST_KEYPAD_TONE_IN_CUSTPACK_BIN

#define MIN_RING_TONE_ID	101
#define RING_TONE_1  101
#if !defined (__MMI_RESOURCE_SLIM_ON_128X160__)
#define RING_TONE_2  102
#define RING_TONE_3  103
#define RING_TONE_4  104
#ifndef __MMI_RESOURCE_SLIM__
#define RING_TONE_5  105
#define MAX_RING_TONE_ID	105
#else
#define MAX_RING_TONE_ID	104
#endif
#else
#ifndef MEM_ULC_3216		
#define MAX_RING_TONE_ID	102
#else
#define MAX_RING_TONE_ID	101
#endif
#endif
#define MIN_EMS_IMY_ID	141
#define MAX_EMS_IMY_ID	145
#define MIN_MIDI_ID	151
#define MIDI_1  151
#define MIDI_2  152

#if !defined (__MMI_RESOURCE_SLIM_ON_128X160__)
#define MIDI_3  153
#define MIDI_4  154
#define MIDI_5  155
#define MIDI_6  156
#define MIDI_7  157
#define MIDI_8  158
#define MIDI_9  159
#define MIDI_10  160
#define MIDI_11  161
#define MIDI_12  162
//#define MAX_MIDI_ID	162
#ifndef __MMI_RESOURCE_SLIM__
#define MAX_MIDI_ID	156  //modified by lin for reduce audio resource@20070623
#else
#ifndef MEM_ULC_3216		
#define MAX_MIDI_ID	154  //modified by lin for reduce audio resource@20070623
#else
#define MAX_MIDI_ID	153  //modified by lin for reduce audio resource@20070623
#endif
#endif
#else
#define MAX_MIDI_ID	152  //modified by lin for reduce audio resource@20070623
#endif
#define MIN_FNG_SND_ID  171   //Added by jinzh start:20070724
#define FNG_PUZZLE_SOUND	171
#define FNG_PANDA_SOUND		172
#define FNG_UFO_SOUND       173
#define MAX_FNG_SND_ID  180	  //Added by jinzh end:20070724

enum CAMERA_SOUND_ID{
	MIN_CAMERA_SND_ID = 181,
#if defined(MEM_ULC_3216)
	CAMERA_CNT_SOUND1=MIN_CAMERA_SND_ID,
#else
	CAMERA_SOUND1=MIN_CAMERA_SND_ID,
	CAMERA_SOUND2,
	CAMERA_SOUND3,
	/*chenhe add pcm audio ID*/
	CAMERA_DAOJISHI,
	CAMERA_CNT_SOUND1,
	CAMERA_CNT_SOUND2,
	CAMERA_CNT_SOUND3,//187
#endif

	MAX_CAMERA_SND_ID = 190	
};


#define MIN_SND_ID	201
#define SOUND_1  201
#define SOUND_2  202
#if !defined (__MMI_RESOURCE_SLIM_ON_128X160__)
#define SOUND_3  203
#define SOUND_4  204
#define SOUND_5  205
#define SOUND_6  206
#ifndef __MMI_RESOURCE_SLIM__
#define MAX_SND_ID	206
#else
#define MAX_SND_ID	203
#endif
#else
#define MAX_SND_ID	202
#endif

#if !defined (__MMI_RESOURCE_SLIM_ON_128X160__)
#define MIN_MSG_SND_ID	221
#ifndef __MMI_RESOURCE_SLIM__
#define MAX_MSG_SND_ID	226
#else
#define MAX_MSG_SND_ID	224
#endif
#define MIN_MMS_SND_ID	241
#define MAX_MMS_SND_ID	250
#else
#define MIN_MSG_SND_ID	221
#ifndef MEM_ULC_3216		
#if defined(__PROJECT_GALLITE_C01__)
#define MAX_MSG_SND_ID	224
#else
#define MAX_MSG_SND_ID	222
#endif
#else
#define MAX_MSG_SND_ID	221
#endif
#define MIN_MMS_SND_ID	241
#define MAX_MMS_SND_ID	242
#endif

#if defined(__PROJECT_GALLITE_C01__)
#define MIN_HUMAN_VOICE_ID 243
#define MAX_HUMAN_VOICE_ID 255
#endif

#if defined( IMY_IN_CUSTPACK_BIN ) || defined( EMS_IMY_IN_CUSTPACK_BIN ) || defined( MIDI_IN_CUSTPACK_BIN ) || defined( SND_IN_CUSTPACK_BIN ) || defined( MSG_SND_IN_CUSTPACK_BIN ) || defined( EMS_SND_IN_CUSTPACK_BIN ) || defined( MMS_SND_IN_CUSTPACK_BIN ) || defined( HUMAN_VOICE_IN_CUSTPACK_BIN ) || defined( CUST_KEYPAD_TONE_IN_CUSTPACK_BIN )

typedef struct {
#ifdef MIDI_IN_CUSTPACK_BIN
	kal_uint32 midis_count;
	const audio_resource_struct CSD_resource_midis[MAX_MIDI_ID - MIN_MIDI_ID + 1];
#endif

#ifdef SND_IN_CUSTPACK_BIN
	kal_uint32 sounds_count;
	const audio_resource_struct CSD_resource_sounds[MAX_SND_ID - MIN_SND_ID + 1];
#endif

#ifdef EMS_SND_IN_CUSTPACK_BIN
	kal_uint32 ems_sounds_count;
	const audio_resource_struct CSD_resource_ems_sounds[MAX_EMS_SND_ID - MIN_EMS_SND_ID + 1];
#endif

#ifdef IMY_IN_CUSTPACK_BIN
	kal_uint32 imelodys_count;
	const audio_resource_struct CSD_resource_imelodys[MAX_RING_TONE_ID - MIN_RING_TONE_ID + 1];
#endif

#ifdef EMS_IMY_IN_CUSTPACK_BIN
	kal_uint32 ems_imelodys_count;
	const audio_resource_struct CSD_resource_ems_imelodys[MAX_EMS_IMY_ID - MIN_EMS_IMY_ID + 1];
#endif

#ifdef MSG_SND_IN_CUSTPACK_BIN
	kal_uint32 message_sounds_count;
	const audio_resource_struct CSD_resource_message_sounds[MAX_MSG_SND_ID - MIN_MSG_SND_ID + 1];
#endif

#ifdef MMS_SND_IN_CUSTPACK_BIN
	kal_uint32 mms_sounds_count;
	const audio_resource_struct CSD_resource_mms_sounds[MAX_MMS_SND_ID - MIN_MMS_SND_ID + 1];
#endif

#ifdef FNG_SND_IN_CUSTPACK_BIN   //Added by Jinzh start:20070724
#ifdef __MMI_GAME__
	kal_uint32 fng_sounds_count;
	const audio_resource_struct CSD_resource_fng_sounds[MAX_FNG_SND_ID - MIN_FNG_SND_ID + 1];
#endif							//Added by Jinzh end:20070724
#endif
/*Jinzh Add Start For 5754 Ver: TBM780  on 2007-8-8 16:18 */
#ifdef CAMERA_SND_IN_CUSTPACK_BIN   
	kal_uint32 camera_sounds_count;
	const audio_resource_struct CSD_resource_camera_sounds[MAX_CAMERA_SND_ID - MIN_CAMERA_SND_ID + 1];
#endif	
/*Jinzh Add End  For 5754 Ver: TBM780  on 2007-8-8 16:18 */

#if defined( DIGIT_TONE_SUPPORT ) && defined( HUMAN_VOICE_IN_CUSTPACK_BIN )
	kal_uint32 human_voice_tones_count;
	const audio_resource_struct CSD_resource_human_voice_tones[MAX_HUMAN_VOICE_ID - MIN_HUMAN_VOICE_ID + 1];
#endif

#if defined( CUST_KEYPAD_TONE_SUPPORT ) && defined( CUST_KEYPAD_TONE_IN_CUSTPACK_BIN )
	kal_uint32 keypad_tones_count;
	const audio_resource_struct CSD_resource_keypad_tones[MAX_KEYPAD_TONE_ID - MIN_KEYPAD_TONE_ID + 1];
#endif
} custpack_audio_header;

#endif

#endif // _MMI_AUDIO_H

  
