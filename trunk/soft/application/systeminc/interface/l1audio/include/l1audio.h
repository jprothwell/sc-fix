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
 *  l1sp.h
 *
 * Project:
 * --------
 *   CSD6208
 *
 * Description:
 * ------------
 *   Audio Task interface definition
 *
 * Author:
 * -------
 * -------
 *
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef L1AUDIO_H
#define L1AUDIO_H

#ifndef WIN32
  #include "kal_release.h"
	 
  //#include "l1audio_trace.h"
  #include "med_status.h"
  #include "fsal.h"
#else
  #include "kal_non_specific_general_types.h"
  #define MED_STAT_AUDIO_START 0
  #include "fsal.h"
#endif












/* ------------------------------------------------------------------------------ */
/*  Audio Function Definition                                                     */
/* ------------------------------------------------------------------------------ */
#define  L1SP_KEYTONE         0
#define  L1SP_TONE            1
#define  L1SP_SPEECH          2
#define  L1SP_VMP             3
#define  L1SP_VMR             4
#define  L1SP_VR              5
#define  L1SP_MEDIA           6
#define  L1SP_DAI             7
#define  L1SP_FM_RADIO        8
#define  L1SP_A_MEDIA         9
#define  L1SP_PCM_EX          10 

#define  L1SP_MAX_AUDIO       11

#define  SRC_VOICE_DAC        0
#define  SRC_AUDIO_DAC        1
#define  SRC_EXT_INPUT        2

#define  SND_SRC_MAP   (kal_uint32)((SRC_VOICE_DAC<<L1SP_KEYTONE*2) |      \
                                    (SRC_VOICE_DAC<<L1SP_TONE*2) |         \
                                    (SRC_VOICE_DAC<<L1SP_SPEECH*2) |       \
                                    (SRC_VOICE_DAC<<L1SP_VMP*2) |          \
                                    (SRC_VOICE_DAC<<L1SP_VMR*2) |          \
                                    (SRC_VOICE_DAC<<L1SP_VR*2) |           \
                                    (SRC_VOICE_DAC<<L1SP_MEDIA*2) |        \
                                    (SRC_VOICE_DAC<<L1SP_DAI*2) |          \
                                    (SRC_EXT_INPUT<<L1SP_FM_RADIO*2) |     \
                                    (SRC_AUDIO_DAC<<L1SP_A_MEDIA*2))


typedef  void  (*L1Audio_EventHandler)(void*);

kal_uint16  L1Audio_GetAudioID( void );
void        L1Audio_FreeAudioID( kal_uint16 aud_id );
void        L1Audio_SetEventHandler( kal_uint16 audio_id, L1Audio_EventHandler handler );
void        L1Audio_SetEvent( kal_uint16 audio_id, void *data );
void        L1Audio_LSetEvent( kal_uint16 audio_id, void *data );
kal_bool    L1Audio_CheckFlag( kal_uint16 audio_id );
void        L1Audio_SetFlag( kal_uint16 audio_id );
void        L1Audio_ClearFlag( kal_uint16 audio_id );
void        L1Audio_HookHisrHandler( kal_uint16 magic_no, L1Audio_EventHandler handler, void *userData );
void        L1Audio_UnhookHisrHandler( kal_uint16 magic_no );

void        L1Audio_EnterDedicatedMode( void );
void        L1Audio_LeaveDedicatedMode( void );
void        L1Audio_ResetDevice( void );

/* ------------------------------------------------------------------------------ */
/*  Audio Front End Interface                                                     */
/* ------------------------------------------------------------------------------ */
/* -------------------------------- */
/*  Audio Output Device Definition  */
/* -------------------------------- */
#define  L1SP_BUFFER_0        0x01    /* NOTE: Don't use buffer definition directly   */
#define  L1SP_BUFFER_1        0x02    /*       Use speaker definition below           */
#define  L1SP_BUFFER_ST       0x04
#define  L1SP_BUFFER_EXT      0x08
#define  L1SP_BUFFER_EXT_G    0x10
#define  L1SP_STEREO2MONO     0x20     /* Do not use this term for speaker definition */
#define  L1SP_BUFFER_ST_M     (L1SP_BUFFER_ST|L1SP_STEREO2MONO)

#define  L1SP_LNA_0           0
#define  L1SP_LNA_1           1
#define  L1SP_LNA_FMRR        3

/* ------------------------------------------------------------------------------ */
/*  Key Tone Interface                                                            */
/* ------------------------------------------------------------------------------ */
void  KT_SetOutputDevice( kal_uint8 device );
void  KT_SetOutputVolume( kal_uint8 volume1, kal_uint8 volume2 );
void  KT_Play( kal_uint16 freq1, kal_uint16 freq2, kal_uint16 duration );
void  KT_Stop( void );
void  KT_SetAmplitude( kal_int16 amp );
void  KT_SetFIR( kal_bool enable );


/* ------------------------------------------------------------------------------ */
/*  Tone Interface                                                                */
/* ------------------------------------------------------------------------------ */
typedef struct {
   kal_uint16   freq1;         /* First frequency                              */
   kal_uint16   freq2;         /* Second frequency                             */
   kal_uint16   on_duration;   /* Tone on duation(ms), 0 for continuous tone   */
   kal_uint16   off_duration;  /* Tone off duation(ms), 0 for end of playing   */
   kal_int8     next_tone;     /* Index of the next tone                       */
} L1SP_Tones;

/* -------------------------------- */
/*  Example of Tone Definition      */
/* -------------------------------- */
/*
static const L1SP_Tones tone1_gsm[] = { { 425,   0,   0,   0,   0 } };
static const L1SP_Tones tone1_pcs[] = { { 350, 440,   0,   0,   0 } };
static const L1SP_Tones tone2_gsm[] = { { 425,   0, 500, 500,   0 } };
static const L1SP_Tones tone2_pcs[] = { { 480, 620, 500, 500,   0 } };
static const L1SP_Tones tone3_gsm[] = { { 425,   0, 200, 200,   0 } };
static const L1SP_Tones tone3_pcs[] = { { 480, 620, 200, 200,   0 } };
static const L1SP_Tones tone4[]     = { { 425,   0, 200,   0,   0 } };
static const L1SP_Tones tone5[]     = { { 425,   0, 200, 200,   1 },
                                        { 425,   0, 200, 200,   2 },
                                        { 425,   0, 200,   0,   0 } };
static const L1SP_Tones tone61[]    = { { 950,   0, 330,1000,   0 } };
static const L1SP_Tones tone62[]    = { {1400,   0, 330,1000,   0 } };
static const L1SP_Tones tone63[]    = { {1800,   0, 330,1000,   0 } };
static const L1SP_Tones tone7[]     = { { 425,   0, 200, 600,   1 },
                                        { 425,   0, 200,3000,   2 },
                                        { 425,   0, 200, 600,   3 },
                                        { 425,   0, 200,   0,   0 } };
*/

void  TONE_SetOutputDevice( kal_uint8 device );
void  TONE_SetOutputVolume( kal_uint8 volume1, kal_uint8 volume2 );
void  TONE_Play( const L1SP_Tones *tonelist );
void  TONE_Stop( void );
void  TONE_SetAmplitude( kal_int16 amp );
void  TONE_SetFIR( kal_bool enable );
void  TONE_SetHandler( void (*handler)(void) );


/* ------------------------------------------------------------------------------ */
/*  Speech Interface                                                              */
/* ------------------------------------------------------------------------------ */
void        L1SP_SetOutputDevice( kal_uint8 device );
void        L1SP_SetOutputVolume( kal_uint8 volume1, kal_uint8 volume2 );
void        L1SP_Speech_On( void );
void        L1SP_Speech_Off( void );
void        L1SP_SetInputSource( kal_uint8 src );
kal_uint8   L1SP_GetInputSource( void );
void        L1SP_SetMicrophoneVolume( kal_uint8 mic_volume );
kal_uint8   L1SP_GetMicrophoneVolume( void );
void        L1SP_SetSidetoneVolume( kal_uint8 sidetone );
kal_uint8   L1SP_GetSidetoneVolume( void );
void        L1SP_MuteMicrophone( kal_bool mute );
kal_bool    L1SP_IsMicrophoneMuted( void );
void        L1SP_SetFIR( kal_bool enable );
void        L1SP_NormalMode_SetAEC( kal_bool ec );
void        L1SP_NormalMode_SetAECParameter( kal_uint16 aec_np_on, kal_uint16 tcl );
void        L1SP_LoudSpkMode_SetEES( kal_bool ec );
void        L1SP_LoudSpkMode_SetEESParameter( kal_uint16 fe_atten, kal_uint16 dt_atten );
void        L1SP_SetEchoSuppression( kal_bool ec );
void        L1SP_SetEchoSuppressionParameter( kal_uint16 timeConst, kal_uint16 volConst );
void        L1SP_Set_DAI_Mode( kal_uint8 mode );
void        L1SP_Write_Audio_Coefficients( const kal_int16 in_coeff[30], const kal_int16 out_coeff[30] );
void        L1SP_SetAfeLoopback( kal_bool enable );
void        L1SP_Bluetooth_On( kal_uint8 bt_mode, kal_uint8 bt_sync_type, kal_uint8 bt_sync_length );
void        L1SP_Bluetooth_Off( void );
kal_bool    L1SP_IsBluetoothOn( void );
kal_uint16  L1SP_Bluetooth_GetEarphoneVolume( void );
void        L1SP_Bluetooth_SetEarphoneVolume( kal_uint16 vol );

/* ------------------------------------------------------------------------------ */
/*  AMR encoder buffer management                                                 */
/* ------------------------------------------------------------------------------ */
kal_uint32 AMR515_BufferSize( void );
kal_uint16 AMR515_SetBuffer( void *buf, kal_uint32 buf_size );

/* ------------------------------------------------------------------------------ */
/*  Media File Playback/Recording Interface                                       */
/* ------------------------------------------------------------------------------ */
typedef enum {
   MEDIA_SUCCESS = MED_STAT_AUDIO_START,
   MEDIA_FAIL,
   MEDIA_REENTRY,                /* media reentry error                       */
   MEDIA_NOT_INITIALIZED,        /* media control is not initialized          */
   MEDIA_BAD_FORMAT,             /* media format error                        */
   MEDIA_BAD_PARAMETER,          /* media bad parameter error                 */
   MEDIA_BAD_COMMAND,            /* media bad command error                   */
   MEDIA_NO_HANDLER,             /* media no handler error                    */
   MEDIA_UNSUPPORTED_CHANNEL,    /* unsupported audio channel count           */
   MEDIA_UNSUPPORTED_FREQ,       /* unsupported audio freqency                */
   MEDIA_UNSUPPORTED_TYPE,       /* unsupported audio content                 */
   MEDIA_UNSUPPORTED_OPERATION   /* unsupported operation on such audio type  */
} Media_Status;

typedef enum {
   MEDIA_NONE,
   MEDIA_DATA_REQUEST,
   MEDIA_DATA_NOTIFICATION,
   MEDIA_END,
   MEDIA_ERROR,
   /*liwh Add Start For 5993 Ver: TBM780  on 2007-8-8 17:23 */
   MEDIA_UNEXPECTED_FORMAT,
   /*liwh Add End  For 5993 Ver: TBM780  on 2007-8-8 17:23 */
   MEDIA_REPEATED,
   MEDIA_TERMINATED,
   MEDIA_LED_ON,
   MEDIA_LED_OFF,
   MEDIA_VIBRATOR_ON,
   MEDIA_VIBRATOR_OFF,
   MEDIA_BACKLIGHT_ON,
   MEDIA_BACKLIGHT_OFF,
   MEDIA_EXTENDED_EVENT
} Media_Event;

typedef enum {
   MEDIA_FORMAT_GSM_FR,
   MEDIA_FORMAT_GSM_HR,
   MEDIA_FORMAT_GSM_EFR,
   MEDIA_FORMAT_AMR,
   MEDIA_FORMAT_AMR_WB,
   MEDIA_FORMAT_DAF,
   MEDIA_FORMAT_AAC,
   MEDIA_FORMAT_PCM_8K,
   MEDIA_FORMAT_PCM_16K,
   MEDIA_FORMAT_G711_ALAW,
   MEDIA_FORMAT_G711_ULAW,
   MEDIA_FORMAT_DVI_ADPCM,
   MEDIA_FORMAT_VRD,
   MEDIA_FORMAT_WAV,
   MEDIA_FORMAT_WAV_ALAW,
   MEDIA_FORMAT_WAV_ULAW,
   MEDIA_FORMAT_WAV_DVI_ADPCM,
   MEDIA_FORMAT_SMF,
   MEDIA_FORMAT_IMELODY,
   MEDIA_FORMAT_SMF_SND,
   MEDIA_FORMAT_MMF,
   MEDIA_FORMAT_AU,
   MEDIA_FORMAT_AIFF,
   MEDIA_FORMAT_VRSI,
   MEDIA_FORMAT_WMA
} Media_Format;

typedef enum {
   MEDIA_FIR_SPEECH_OUTPUT1_8K,
   MEDIA_FIR_SPEECH_INPUT1_8K,
   MEDIA_FIR_SPEECH_OUTPUT2_8K,
   MEDIA_FIR_SPEECH_INPUT2_8K,
   MEDIA_FIR_SPEECH_OUTPUT1_16K,
   MEDIA_FIR_SPEECH_INPUT1_16K,
   MEDIA_FIR_SPEECH_OUTPUT2_16K,
   MEDIA_FIR_SPEECH_INPUT2_16K,
   MEDIA_FIR_MELODY_OUTPUT1_32K,
   MEDIA_FIR_MELODY_OUTPUT2_32K
} Media_Filter_Type;

typedef struct {
   const kal_uint8   *smffile;
   kal_int32         filesize;            /* MIDI file size             */
   kal_int16         repeats;             /* 0 -> endless               */
} Media_SMF_Param;

typedef struct {
   const kal_uint8   *imyfile;
   kal_int32         filesize;            /* MIDI file size             */
   kal_int16         repeats;             /* 0 -> endless               */
   kal_int16         default_instrument;  /* default instrument(1~128)  */
} Media_iMelody_Param;

typedef struct {
   const kal_uint8 *wavefile;
   kal_int32       filesize;            /* Wave file size             */
   kal_int16       repeats;             /* 0 -> endless               */
   kal_int16       format;              /* add for direct wave        */
   kal_uint16      sampling_rate;       /* add for direct wave        */
} Media_Wav_Param;

typedef struct {
   const kal_uint8 *toneSeq;
   kal_int32     toneSeqLen;          /* tone sequence data length  */
   kal_int16     repeats;             /* 0 -> endless               */
} Media_ToneSeq_Param;

typedef struct{
   const kal_uint8 *mmffile;
   kal_int32 filesize;
   kal_int16 repeats;
} Media_MMF_Param;

typedef struct{
   const kal_uint8 *amrfile;
   kal_int32 filesize;
   kal_int16 repeats;
} Media_AMR_Param;

typedef enum {
   MEDIA_CTRL_MAX_SWING,
   MEDIA_CTRL_KEY_SHIFT
} Media_Ctrl;

typedef  struct media_handle Media_Handle;

#define  MEDIA_VMP_AS_SPEECH     ((void*)0)
#define  MEDIA_VMP_AS_RINGTONE   ((void*)1)
#define  MEDIA_VMP_AMR           ((void*)0)
#define  MEDIA_VMP_AMR_WB        ((void*)2)

#define MAX_DESCRIPTION     80
typedef struct tWMADescription
{
   kal_uint32 time; // duration in sec
   kal_uint32 bitRate;
   kal_uint32 sampleRate;
   kal_bool stereo;
   kal_wchar title[MAX_DESCRIPTION+1];
   kal_wchar artist[MAX_DESCRIPTION+1];
   kal_wchar album[MAX_DESCRIPTION+1];
      union
   {
      struct
      {
         kal_uint8 CRC;
         kal_uint8 channel_mode;
         kal_uint8 bit_rate_index;
      } DAF;
   } details;

} audInfoStruct;

void  Media_SetOutputDevice( kal_uint8 device );
void  Media_SetOutputVolume( kal_uint8 volume1, kal_uint8 volume2 );
void  Media_SetOutputVolume_ST( kal_uint8 volume );
void  Media_SetBuffer( kal_uint16 *buffer, kal_uint32 buf_len );
void  Media_GetWriteBuffer( kal_uint16 **buffer, kal_uint32 *buf_len );
void  Media_GetWriteBuffer_ext( kal_uint8 **buffer, kal_uint32 *buf_len );

void  Media_WriteDataDone( kal_uint32 len );
void  Media_DataFinished( void );
void  Media_GetReadBuffer( kal_uint16 **buffer, kal_uint32 *buf_len );
void  Media_ReadDataDone( kal_uint32 len );
void  Media_GetFileHeader( kal_uint8 **header, kal_uint32 *len );
void  Media_SetRBThreshold( kal_uint16 threshold );   /* Ring buffer threshold in words */

Media_Status  Media_Play( Media_Format format, void (*media_handler)( Media_Event event ), void *param );
Media_Status  Media_Record( Media_Format format, void (*media_handler)( Media_Event event ), void *param );
void          Media_Stop( void );
Media_Status  Media_Pause( void );
Media_Status  Media_Resume( void );
Media_Status  Media_Control( Media_Ctrl ctrl_no, kal_int32 ctrl_val );
Media_Status  Media_SetLevel( kal_uint8 level );
kal_uint8     Media_GetLevel( void );

void  Media_SetFilter( Media_Filter_Type type, kal_uint16 len, const kal_int16 *filter );
/* ------------------------------------------------------------------------------ */

Media_Handle   *JSmf_Open( void (*handler)( Media_Handle *handle, Media_Event event ), Media_SMF_Param **param );
Media_Status   JSmf_Close( Media_Handle *handle );
Media_Status   JSmf_Play( Media_Handle *handle );
Media_Status   JSmf_Stop( Media_Handle *handle );
Media_Status   JSmf_Pause( Media_Handle *handle );
Media_Status   JSmf_Resume( Media_Handle *handle );
Media_Status   JSmf_SetStartTime( Media_Handle *handle, kal_int32 msStartTime );
Media_Status   JSmf_SetStopTime( Media_Handle *handle, kal_int32 msStopTime );
kal_int32      JSmf_GetCurrentTime( Media_Handle *handle );
kal_int32      JSmf_GetDuration( Media_Handle *handle );
kal_int32      JSmf_GetMidiDuration( const kal_uint8 *mid );
Media_Event    JSmf_QueryExtEvent( Media_Handle *handle, kal_int32 *note );

kal_bool       JMidi_isBankQuerySupported( void );
void           JMidi_GetBankList( kal_bool custom, const kal_int16 **buf, kal_int16 *len);
void           JMidi_GetProgramList( kal_int16 bank, const kal_int8 **buf, kal_int16 *len);
const kal_uint8*JMidi_GetProgramName( kal_int16 bank, kal_int8 prog );
const kal_uint8*JMidi_GetKeyName( kal_int16 bank, kal_int8 prog, kal_int8 key );

Media_Handle   *JMidi_OpenDevice( void );
Media_Status   JMidi_CloseDevice( Media_Handle *handle );
Media_Status   JMidi_SendLongMsg( Media_Handle *handle, const kal_uint8 *buf, kal_uint16 len );
Media_Status   JMidi_SendShortMsg( Media_Handle *handle, kal_uint8 type, kal_uint8 data1, kal_uint8 data2 );
Media_Status   JMidi_SetLevel( Media_Handle *handle, kal_uint8 level );
kal_uint8      JMidi_GetLevel( Media_Handle *handle );
Media_Status   JMidi_SetTempo( Media_Handle *handle, kal_int32 milliTempo );
kal_int32      JMidi_GetTempo( Media_Handle *handle );
Media_Status   JMidi_SetRate( Media_Handle *handle, kal_int32 milliRate );
kal_int32      JMidi_GetRate( Media_Handle *handle );
Media_Status   JMidi_SetPitchTranspose( Media_Handle *handle, kal_int32 milliSt );
kal_int32      JMidi_GetPitchTranspose( Media_Handle *handle );
Media_Status   JMidi_SetChannelVolume( Media_Handle *handle, kal_int8 channel, kal_uint8 level );
kal_int8       JMidi_GetChannelVolume( Media_Handle *handle, kal_int8 channel );
Media_Status   JMidi_SetProgram( Media_Handle *handle, kal_int8 channel, kal_int16 bank, kal_int8 program );
void           JMidi_GetProgram( Media_Handle *handle, kal_int8 channel, kal_int16 *bank, kal_int8 *program );


Media_Handle   *JTone_Open( void (*handler)( Media_Handle *handle, Media_Event event ), Media_ToneSeq_Param **param );
Media_Status   JTone_Close( Media_Handle *handle );
Media_Status   JTone_Play( Media_Handle *handle );
Media_Status   JTone_Stop( Media_Handle *handle );
Media_Status   JTone_Pause( Media_Handle *handle );
Media_Status   JTone_Resume( Media_Handle *handle );
Media_Status   JTone_SetStartTime( Media_Handle *handle, kal_int32 msStartTime );
Media_Status   JTone_SetStopTime( Media_Handle *handle, kal_int32 msStopTime );
kal_int32      JTone_GetCurrentTime( Media_Handle *handle );
kal_int32      JTone_GetDuration( Media_Handle *handle );
Media_Status   JTone_SetLevel( Media_Handle *handle, kal_uint8 level );
kal_uint8      JTone_GetLevel( Media_Handle *handle );
Media_Status   JTone_SetPitchShift(Media_Handle *handle, kal_int8 pitch_shift);
kal_int8       JTone_GetPitchShift(Media_Handle *handle);
Media_Status   JTone_SetSpeed(Media_Handle *handle, kal_uint32 speed_factor);
Media_Status   JTone_SetProgram(Media_Handle *handle, kal_uint8 program);
Media_Status   JTone_SetPlayStyle(Media_Handle *handle, kal_uint8 play_style);

Media_Handle   *JImy_Open( void (*handler)( Media_Handle *handle, Media_Event event ), Media_iMelody_Param **param );
Media_Status   JImy_Close( Media_Handle *handle );
Media_Status   JImy_Play( Media_Handle *handle );
Media_Status   JImy_Stop( Media_Handle *handle );
Media_Status   JImy_Pause( Media_Handle *handle );
Media_Status   JImy_Resume( Media_Handle *handle );
Media_Status   JImy_SetStartTime( Media_Handle *handle, kal_int32 msStartTime );
Media_Status   JImy_SetStopTime( Media_Handle *handle, kal_int32 msStopTime );
kal_int32      JImy_GetCurrentTime( Media_Handle *handle );
kal_int32      JImy_GetDuration( Media_Handle *handle );
Media_Status   JImy_SetLevel( Media_Handle *handle, kal_uint8 level );
kal_uint8      JImy_GetLevel( Media_Handle *handle );

Media_Handle   *JWav_Open( void (*handler)( Media_Handle *handle, Media_Event event ), Media_Wav_Param **param );
Media_Status   JWav_Close( Media_Handle *handle );
Media_Status   JWav_Load(Media_Handle *handle, kal_uint32 *requiredBufSize);
void           JWav_SetBuffer(Media_Handle *handle, kal_int16 *buffer, kal_uint32 buf_len);
Media_Status   JWav_Play( Media_Handle *handle );
Media_Status   JWav_Stop( Media_Handle *handle );
Media_Status   JWav_Pause( Media_Handle *handle );
Media_Status   JWav_Resume( Media_Handle *handle );
Media_Status   JWav_SetStartTime( Media_Handle *handle, kal_int32 msStartTime );
Media_Status   JWav_SetStopTime( Media_Handle *handle, kal_int32 msStopTime );
kal_int32      JWav_GetCurrentTime( Media_Handle *handle );
kal_int32      JWav_GetDuration( Media_Handle *handle );
Media_Status   JWav_SetLevel( Media_Handle *handle, kal_uint8 level );
kal_uint8      JWav_GetLevel( Media_Handle *handle );

Media_Handle   *JXWav_Open( void (*handler)( Media_Handle *handle, Media_Event event ));
Media_Status   JXWav_Close( Media_Handle *handle );
Media_Status   JXWav_Load(Media_Handle *handle, kal_uint32 *requiredBufSize);
void           JXWav_SetBuffer(Media_Handle *handle, kal_int16 *buffer, kal_uint32 buf_len);
Media_Status   JXWav_Play( Media_Handle *handle );
Media_Status   JXWav_Stop( Media_Handle *handle );
Media_Status   JXWav_Pause( Media_Handle *handle );
Media_Status   JXWav_Resume( Media_Handle *handle );
kal_int32      JXWav_GetCurrentTime( Media_Handle *handle );
Media_Status   JXWav_SetLevel( Media_Handle *handle, kal_uint8 level );
kal_uint8      JXWav_GetLevel( Media_Handle *handle );

Media_Handle   *JMmf_Open( void (*handler)( Media_Handle *handle, Media_Event event ), Media_MMF_Param **param );
Media_Status   JMmf_Close( Media_Handle *handle );
Media_Status   JMmf_Play( Media_Handle *handle );
Media_Status   JMmf_Stop( Media_Handle *handle );
Media_Status   JMmf_Pause( Media_Handle *handle );
Media_Status   JMmf_Resume(Media_Handle *handle);
Media_Status   JMmf_SetStartTime( Media_Handle *handle, kal_int32 msStartTime );
Media_Status   JMmf_SetStopTime( Media_Handle *handle, kal_int32 msStopTime );
kal_int32      JMmf_GetCurrentTime( Media_Handle *handle );
kal_int32      JMmf_GetDuration( Media_Handle *handle );
Media_Status   JMmf_SetLevel( Media_Handle *handle, kal_uint8 level );
kal_uint8      JMmf_GetLevel( Media_Handle *handle );

Media_Handle   *JAmr_Open( void (*handler)( Media_Handle *handle, Media_Event event ), Media_AMR_Param **param );
Media_Status   JAmr_Close( Media_Handle *handle );
Media_Status   JAmr_Play( Media_Handle *handle );
Media_Status   JAmr_Stop( Media_Handle *handle );
Media_Status   JAmr_Pause( Media_Handle *handle );
Media_Status   JAmr_Resume( Media_Handle *handle );
Media_Status   JAmr_SetStartTime( Media_Handle *handle, kal_int32 msStartTime );
Media_Status   JAmr_SetStopTime( Media_Handle *handle, kal_int32 msStopTime );
kal_int32      JAmr_GetCurrentTime( Media_Handle *handle );
Media_Status   JAmr_SetLevel( Media_Handle *handle, kal_uint8 level );
kal_uint8      JAmr_GetLevel( Media_Handle *handle );
Media_Status   JAmr_GetDuration( const kal_uint8 *amrfile, kal_int32 amrsize, kal_int32 *duration );

/* ------------------------------------------------------------------------------ */
void AEQ_SetAudioEqualizer( kal_int8 magnitude[8] );
void AEQ_Activate( kal_int16 asp_fs, kal_bool rampup );  /* l1audio internal use */

/* ------------------------------------------------------------------------------ */
Media_Status AudioPP_SetTS(kal_uint16 speed_mode);
void AudioPP_CloseTS(void);
void AudioPP_Set3D(kal_int16 audPP_3DTable[64]);
Media_Status AudioPP_TurnOn3D(void);
void AudioPP_TurnOff3D(void);
kal_uint16 AudioPP_GetTSSpeed(void);
kal_bool AudioPP_IsTSOn(void);
kal_bool AudioPP_Is3DOn(void);
kal_bool AudioPP_IsTSSupport(Media_Format eFormat, STFSAL *pstFSAL);
kal_bool AudioPP_Is3DSupport(Media_Format eFormat, STFSAL *pstFSAL);
void AudioPP_TSActivate(kal_bool voice_init);      /* l1audio internal use */
void AudioPP_3DActivate(kal_int16 asp_fs);         /* l1audio internal use */
void AudioPP_Close(void);                          /* l1audio internal use */
void AudioPP_Disable(void);                        /* l1audio internal use */
void AudioPP_Enable(void);                         /* l1audio internal use */

/* ------------------------------------------------------------------------------ */

/* FM Radio Interface */
void  FMR_PowerOn(void);
void  FMR_PowerOff(void);
kal_uint8 FMR_ValidStop(kal_int16 freq, kal_int8 signalvl, kal_bool is_step_up);
void  FMR_SetFreq(kal_int16 curf);
void FMR_AutoSearch(kal_uint8 dir, void(*fm_handler)(kal_int16 *data));
kal_int16 FMR_ManualSearch(kal_uint8 dir);
kal_int16 FMR_GetFreq(void);
void  FMR_Mute(kal_uint8 mute);
void  FMR_Mono(kal_uint8 MS);
void  FMR_Standby(kal_uint8 standby);
void FMR_SetOutputDevice( kal_uint8 device );
void  FMR_SetOutputVolume( kal_uint8 volume1, kal_uint8 volume2 );
kal_uint8 FMR_GetSignalLevel(kal_int16 curf);

/* ------------------------------------------------------------------------------ */

/* A/V Sync Interface */

kal_uint32 Media_A2V_GetInterruptCount(void);
void Media_A2V_EnableInterrupt(kal_bool bFlag, kal_uint32 start_count);
void Media_A2V_HookInterrupt(void (*a2v_lisr)(void));
void Media_A2V_SetTimeStamp(kal_uint16 wTimeStamp);
void Media_A2V_SetInterruptCount(kal_uint32 uInterruptCount);
kal_uint16 Media_A2V_GetAudioFrameCount(void);
kal_uint16 Media_A2V_GetVideoFrameCount(void);
kal_uint32 Media_A2V_GetAudioInterruptTimeScale(Media_Format eFormat, kal_uint8 bSamplingFreqIndex);
kal_uint32 Media_A2V_GetAudioInterruptTimeScaleFxdPnt(Media_Format eFormat, kal_uint8 bSamplingFreqIndex);
kal_uint32 Media_A2V_GetAudioPlaybackDelay(Media_Format eFormat);
kal_uint32 Media_A2V_GetAudioRecordDelay(Media_Format eFormat);


/* ------------------------------------------------------------------------------ */
typedef enum {
   POC_AMR_NONE   = 0x00,
   POC_AMR_0475   = 0x01,
   POC_AMR_0515   = 0x02,
   POC_AMR_0590   = 0x04,
   POC_AMR_0670   = 0x08,
   POC_AMR_0740   = 0x10,
   POC_AMR_0795   = 0x20,
   POC_AMR_1020   = 0x40,
   POC_AMR_1220   = 0x80,
   POC_AMR_ALL    = 0xFF
} PoC_AMR_Type;

void PoC_AMR_SetBuffer(kal_uint16 *buf, kal_uint32 len);
PoC_AMR_Type PoC_AMR_GetVocoderCapability(void);
Media_Status PoC_AMR_EncodeStart( PoC_AMR_Type vocoder, void (*media_handler)(void), kal_int32 frame_threshold );
Media_Status PoC_AMR_EncodeStop( void );
Media_Status PoC_AMR_DecodeStart( void );
Media_Status PoC_AMR_DecodeStop( void );
kal_int32 PoC_AMR_GetFrameCount( void );
kal_int32 PoC_AMR_GetPayload( kal_uint8 *buf, kal_int32 len, kal_int32 frames );
Media_Status PoC_AMR_PutPayload( const kal_uint8 *buf, kal_int32 len );

/* ------------------------------------------------------------------------------ */
/* VR SD Interface */
typedef enum
{
   VR_FINISHED,
   VR_OK,
   VR_FAILED,
   VR_CONTINUE,
   VR_NO_SOUND,
   VR_TOO_SIM,
   VR_TOO_DIFF,
   VR_SDMODEL_NOT_READY,
   VR_MISMATCH_ID,
   VR_EXISTENT_ID,
   VR_FLASH_DISK_FULL,
   VR_FLASH_ERROR,
   VR_STOP,
   VR_TOO_LONG,
   VR_TOO_SHORT
} VR_Result;

VR_Result VR_GetParameters(kal_int16 *SimThrld, kal_int16 *DiffThrld, kal_int16 *RejThrld);
VR_Result VR_SetParemeters (kal_int16 SimThrld, kal_int16 DiffThrld, kal_int16 RejThrld);
kal_uint32 VR_GetBufferSize(void);
void VR_SetBuffer(kal_uint8 *RTmemory);
VR_Result  VR_SetDatabaseDir(kal_uint8 *Dir);
VR_Result VR_TRA_Start(kal_uint16 wID, kal_uint16 *mdIDList, kal_uint16 *mdIDListLen, void (*vr_handler)(void *parameter ));
VR_Result VR_TRA2Start(void);
VR_Result VR_RCG_Start(kal_uint16 *mdIDList,kal_uint16 *mdIDListLen,void(*vr_handler)(void *parameter));
VR_Result VR_Process(kal_uint16 *RcgID);
void VR_Stop(void);
kal_int32 VR_GetEngineVer(void);
/* ------------------------------------------------------------------------------ */

typedef void* VRSI_Handle;

typedef enum {
   VRSI_OK,
   VRSI_FAIL
}VRSI_Status;

typedef enum {
   VRSI_PROCESS,
   VRSI_PLAY_REQUEST,
   VRSI_RECOG_MPR,
   VRSI_RECOG_OK,
   VRSI_DIGIT_RECOG_MPR,
   VRSI_DIGIT_RECOG_OK,
   VRSI_DIGIT_ADAPT_MPR,
   VRSI_DIGIT_ADAPT_OK,
   VRSI_TRAIN_TAG_1ST_MPR,
   VRSI_TRAIN_TAG_2ND_MPR,
   VRSI_TRAIN_TAG_OK,
   VRSI_ADD_TAGS_OK,
   VRSI_TTS_OK,
   VRSI_PLAY_TAG_OK,
   VRSI_START_CAPTURE_TIMER,
   VRSI_START_PLAYBACK_TIMER,
   VRSI_START_PROCESS_TIMER,
   VRSI_START_QUICK_TIMER,
   VRSI_STOP_TIMER,
   VRSI_ERROR
}VRSI_Event;

typedef enum {
   VRSI_ERR_NO,
   VRSI_ERR_UNINITIALIZED,
   VRSI_ERR_STATE_UNMATCH,
   VRSI_ERR_BUSY,
   VRSI_ERR_INSUFFICIENT_MEM,
   VRSI_ERR_TOO_MANY_TAGS,
   VRSI_ERR_WRONG_DATA_DIR,
   VRSI_ERR_WRONG_DATA_FILE,
   VRSI_ERR_LIBRARY_CORRUPT,
   VRSI_ERR_DB_FULL,
   VRSI_ERR_DB_ERROR,
   VRSI_ERR_NO_MATCH_TAG,
   VRSI_ERR_NO_SOUND,
   VRSI_ERR_LANG_NOT_SUPPORT,
   VRSI_ERR_BAD_GRAMMAR,
   VRSI_ERR_ADD_TAGS_FAIL,
   VRSI_ERR_TRAIN_TAG_FAIL,
   VRSI_ERR_RECOG_FAIL,
   VRSI_ERR_DIGIT_RECOG_FAIL,
   VRSI_ERR_DIGIT_ADAPT_FAIL,
   VRSI_ERR_TTS_TOO_LONG,
   VRSI_ERR_UNKNOWN
}VRSI_ErrMsg;


typedef enum{
   VRSI_LANG_TAIWAN,
   VRSI_LANG_CHINA,
   VRSI_LANG_AMERICAN,
   VRSI_LANG_BRITISH
}VRSI_Language;

// data structure
typedef struct{
   kal_uint16 *pTagName;
   kal_int32 tagLong;
   kal_uint16 tagID;
   kal_uint8 isPeopleName;
}VRSI_Tag_Param;

typedef struct {
   kal_uint16 **ppTagName;
   kal_int32 *pTagLong;
   kal_uint8  *pIsPeopleName;
   kal_uint16 numTag;
}VRSI_AddTags_Param;


typedef struct {
   kal_uint16 *pTagID;
   //kal_int32 *pTagLong;
   kal_uint16 **ppTagName;
   kal_uint16 numTag;
}VRSI_AddTags_Result;

typedef struct {
   kal_uint16 *pTagID;
   kal_int32 *pTagLong;
   kal_uint16 **ppTagName;
   kal_uint16 numTag;
   kal_uint16 resType;
   kal_int16 preCmd;
   kal_int16 postCmd;
}VRSI_Recog_Result;

typedef struct {
   kal_uint16 digitLen;
   kal_uint16 *pDigits;
}VRSI_Digit_Recog_Result;

// API functions
VRSI_Status VRSI_Init( void (*vrsi_handler)( VRSI_Event event ));
kal_uint32 VRSI_GetMemRequest( kal_uint16 maxRecogTag, kal_uint8 maxSDTag );
VRSI_Status VRSI_SetMem(kal_uint8 *memory, kal_uint32 size, kal_uint16 maxRecogTag, kal_uint8 maxSDTag ,kal_uint16 *vrsiDir);
VRSI_Status VRSI_Add_Tags(const kal_uint8 *grammar, VRSI_AddTags_Param *param );
VRSI_Status VRSI_Train_Tag (const kal_uint8 *grammar, VRSI_Tag_Param *param );
VRSI_Status VRSI_Recog(const kal_uint8 *grammar );
VRSI_Status VRSI_Digit_Recog( VRSI_Language  lang, kal_uint16 limit );
VRSI_Status VRSI_Digit_Adapt( VRSI_Language  lang );
VRSI_Status VRSI_Play_Tag( const kal_uint8 *grammar , kal_uint16 tagID ,void* param  );
VRSI_Status VRSI_TTS_Play( VRSI_Language lang, kal_uint16 *pText );
VRSI_Status VRSI_Process( void );
void VRSI_ReadPrompt (  kal_uint8 *promptLen, const kal_uint16 **prompt );
void VRSI_MMI_Confirmed( void );
void VRSI_ReadResult( void **result );
VRSI_ErrMsg VRSI_GetErrMsg( void );
VRSI_Status VRSI_Gram_GetTagNum(const kal_uint8 *grammar , kal_int32 *tagNum );
VRSI_Status VRSI_Gram_ReadTagIDs(const kal_uint8 *grammar, kal_uint16 *pTagID , kal_int32 bufferSize, kal_int32 *readTagNum);
VRSI_Status VRSI_Gram_DeleteAllTags( const kal_uint8 *grammar );
VRSI_Status VRSI_DeleteTags(const kal_uint8 *grammar, kal_int32 tagNum, const kal_uint16 *pTagID, kal_int32 *delTagNum, kal_uint16 *pDelTagID );
VRSI_Status VRSI_TagExist( const kal_uint8 *grammar, kal_uint16 tagID, kal_bool *bExist  );
VRSI_Status VRSI_ReadTags(const kal_uint8 *grammar, kal_int32 tagNum, const kal_uint16 *pTagID, kal_uint16 **ppTagName, kal_int32 *pTagLong );
VRSI_Status VRSI_Stop( void );
VRSI_Status VRSI_Close( void );
VRSI_Status VRSI_QueryTagId(const kal_uint8 *grammar, kal_int32 tagLong, kal_uint16 *tagID );
VRSI_Status VRSI_QueryNextTagId(const kal_uint8 *grammar, kal_uint16 *tagID );
VRSI_Status VRSI_QueryAllTagIds(const kal_uint8 *grammar, kal_int32 tagLong, kal_uint16 buf_size, kal_uint16 *id_buf, kal_uint16 *tagNum, kal_bool *getAll );

// WMA API
Media_Event WMA_Process( Media_Handle *hdl, Media_Event event );
Media_Status WMA_SetStartTime( Media_Handle *hdl, kal_int32 msStartTime );
Media_Status WMA_SetStopTime( Media_Handle *hdl, kal_int32 msStopTime );
kal_int32 WMA_GetCurrentTime( Media_Handle *hdl );
kal_int32 WMA_GetDuration( STFSAL *pstFSAL );
Media_Status WMA_SetLevel( Media_Handle *hdl, kal_uint8 level );
kal_uint8 WMA_GetLevel( Media_Handle *hdl );
Media_Handle *WMA_Open(void(*handler)( Media_Handle *handle, Media_Event event ), STFSAL *pstFSAL);
Media_Status WMA_Close( Media_Handle *hdl );
void WMA_SetBuffer( Media_Handle *hdl, kal_uint8 *buffer, kal_uint32 buf_len );
Media_Status WMA_Play( Media_Handle *hdl );
Media_Status WMA_Stop( Media_Handle *hdl );
Media_Status WMA_Pause( Media_Handle *hdl );
Media_Status WMA_Resume( Media_Handle *hdl );
Media_Status WMA_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo );

// AAC interface
Media_Event AAC_Process( Media_Handle *hdl, Media_Event event );
Media_Status AAC_SetStartTime( Media_Handle *hdl, kal_int32 msStartTime );
Media_Status AAC_SetStopTime( Media_Handle *hdl, kal_int32 msStopTime );
kal_int32 AAC_GetCurrentTime( Media_Handle *hdl );
kal_int32 AAC_GetDuration( STFSAL *pstFSAL );
Media_Status AAC_SetLevel( Media_Handle *hdl, kal_uint8 level );
kal_uint8 AAC_GetLevel( Media_Handle *hdl );
Media_Handle *AAC_Open(void(*handler)( Media_Handle *handle, Media_Event event ), STFSAL *pstFSAL);
Media_Status AAC_Close( Media_Handle *hdl );
void AAC_SetBuffer( Media_Handle *hdl, kal_uint8 *buffer, kal_uint32 buf_len );
Media_Status AAC_Play( Media_Handle *hdl );
Media_Status AAC_Stop( Media_Handle *hdl );
Media_Status AAC_Pause( Media_Handle *hdl );
Media_Status AAC_Resume( Media_Handle *hdl );
Media_Status AAC_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo );



/*====================start of  l1TTS interface ===================*/
typedef enum
{
	TTS_STR_TEXT,     /*regard input as normal text */
	TTS_STR_PHN_SBL,  /*regard input as phonetic symbol*/
	TTS_STR_CHAR      /*regard input as character set. When this type is set, the engine usually synthesize character by character*/
}TTS_STR_TYPE;

typedef kal_uint8    TTS_LANG;
typedef kal_uint8    TTS_TYPE;
typedef kal_uint8    TTS_GENDER;

//The following definition may be extended in the future.

//TTS_LANGUAGE definition
#define	TTS_LANG_ENG   0x1;	/*English*/
#define	TTS_LANG_TWN   0x2;	/*Taiwan-accent Chinese*/
#define	TTS_LANG_MDN   0x4;	/*Mandarin*/

//TTS_TYPE definition
#define	TTS_TYPE_GENERAL  0x1	/*General Purpose TTS*/
#define	TTS_TYPE_DIC      0x2	/*Dictionary*/
#define	TTS_TYPE_NAME     0x4	/*Name*/
#define	TTS_TYPE_GPS      0x8	/*GPS, traffic guiding*/
#define	TTS_TYPE_WEATHER  0x10	/*weather report*/
#define	TTS_TYPE_STOCK    0x20	/*stock report*/

//TTS_GENDER definition
#define	TTS_GENDER_MALE   0x1
#define	TTS_GENDER_FEMALE 0x2
#define	TTS_GENDER_BOY    0x4
#define	TTS_GENDER_GIRL   0x8

/* constants for values of field dwParamId */
#if defined(IFLY_TTS)
#define TTS_ATTR_INPUT_CODEPAGE    0x00000101	/* input code page, e.g. GBK */
#define TTS_ATTR_TEXT_SCOPE        0x00000103	/* text scope, e.g. number */
#define TTS_ATTR_INPUT_MODE        0x00000200	/* input mode, e.g. from fixed buffer, from callback */
#define TTS_ATTR_PROGRESS_CALLBACK 0x00000206	/* progress callback entry */
#define TTS_ATTR_READ_DIGIT        0x00000302	/* how to read digit, e.g. read as number, read as value  */
#define TTS_ATTR_CHINESE_NUMBER_1  0x00000303	/* how to read number "1" in Chinese */
#define TTS_ATTR_PHONEME_WATCH     0x00000402	/* how to watch phoneme, e.g. Chinese Pin-Yin */
#define TTS_ATTR_WATCH_CALLBACK    0x00000403	/* watch callback entry */
#define TTS_ATTR_SPEAK_STYLE       0x00000501	/* speak style */
#else
#define TTS_ATTR_INPUT_CODEPAGE    0x00000000	/* input code page, e.g. GBK */
#define TTS_ATTR_TEXT_SCOPE        0x00000103	/* text scope, e.g. number */
#define TTS_ATTR_INPUT_MODE        0x00000200	/* input mode, e.g. from fixed buffer, from callback */
#define TTS_ATTR_PROGRESS_CALLBACK 0x00000206	/* progress callback entry */
#define TTS_ATTR_READ_DIGIT        0x00000302	/* how to read digit, e.g. read as number, read as value  */
#define TTS_ATTR_CHINESE_NUMBER_1  0x00000303	/* how to read number "1" in Chinese */
#define TTS_ATTR_PHONEME_WATCH     0x00000402	/* how to watch phoneme, e.g. Chinese Pin-Yin */
#define TTS_ATTR_WATCH_CALLBACK    0x00000403	/* watch callback entry */
#define TTS_ATTR_SPEAK_STYLE       0x00000501	/* speak style */
#endif

/* constants for values of attribute TTSATTR_INPUT_CODEPAGE */
#define TTS_CODEPAGE_GBK             936   /* GBK (default) */
#define TTS_CODEPAGE_BIG5            950   /* Big5 */
#define TTS_CODEPAGE_UTF16LE         1200  /* UTF-16 little-endian */
#define TTS_CODEPAGE_UTF16BE         1201  /* UTF-16 big-endian */

/* constants for values of attribute TTSATTR_TEXT_SCOPE */
#define TTS_SCOPE_UNLIMITED          0     /* unlimited scope (default) */
#define TTS_SCOPE_CHINESE_NUMBER     1     /* Chinese number */
#define TTS_SCOPE_CHINESE_VALUE      2     /* Chinese value */
#define TTS_SCOPE_CHINESE_DATETIME   3     /* Chinese date or time */

/* constants for values of attribute TTSATTR_INPUT_MODE */
#define TTS_INPUT_FIXED_BUFFER       0     /* from fixed buffer */
#define TTS_INPUT_CALLBACK           1     /* from callback */

/* constants for values of attribute TTSATTR_READ_DIGIT */
#define TTS_READDIGIT_AUTO           0     /* decide automatically (default) */
#define TTS_READDIGIT_AS_NUMBER      1     /* say digit as number */
#define TTS_READDIGIT_AS_VALUE       2     /* say digit as value */

/* constants for values of attribute TTSATTR_CHINESE_NUMBER_1 */
#define TTS_CHNUM1_READ_YAO          0     /* read number "1" [yao1] in chinese (default) */
#define TTS_CHNUM1_READ_YI           1     /* read number "1" [yi1] in chinese */

/* constants for values of attribute TTSATTR_PHONEME_WATCH */
#define TTS_PHONEME_NONE             0     /* none (default) */
#define TTS_PHONEME_PINYIN           1     /* Chinese Pin-Yin */

/* constants for values of attribute TTSATTR_SPEAK_STYLE */
#define TTSSTYLE_PLAIN			0		/* plain speak style */
#define TTSSTYLE_NORMAL			1		/* normal speak style (default) */
#define TTSSTYLE_VIVID			2		/* vivid speak style */

typedef enum {
   TTS_END,
   TTS_ERROR,
   TTS_DATA_REQUEST,
   TTS_WATCH,                  /* TTS tell current synth information      */
   TTS_PROGRESS,               /* TTS tell the synth progress             */
   TTS_PROCESS
} TTS_Event;

typedef struct _tagTTSProcessData TTS_PROCESS_DATA;
struct _tagTTSProcessData
{
   const void *lpcSylText;   /* the pointer of text      */
   kal_uint32 dwTextLen;     /* length of text (in byte) */
   const void *lpcSylPhone;  /* pointer of pinyin        */
   kal_uint32 dwPhoneLen;    /* length of pinyin(in byte)*/
   kal_uint16 wSylType;      /* text type                */
};

typedef struct _tagTTSProgress TTSPROGRESS;
struct _tagTTSProgress
{
   kal_uint32    dwProcPos;  /* the offset(in byte) of currently synthesizing context of original input text*/
   kal_uint32    dwProcLen;  /* the length(in byte) of currently synthesizing context of original input text*/
};

typedef struct _tagTTSAttr TTS_ATTR;
struct _tagTTSAttr
{
   kal_uint32    dwAttrId;      /* attribute id */
   kal_uint32    varAttrValue;  /* attribute value */
};
#if 0
typedef struct ttsHandle TTS_Handle;

struct ttsHandle {
	void (*handler)(TTS_Handle *hdl, TTS_Event event, void *data);
	kal_uint32 speed;
	kal_uint32 pitch;
	kal_uint32 volume;
	Media_Status (*SetBuffer)(TTS_Handle *hdl, kal_uint8* buffer, kal_uint32 buf_len);
	kal_uint32   (*GetBufSize)(TTS_Handle *hdl);
	Media_Status (*Play)(TTS_Handle *hdl, kal_wchar* string, TTS_STR_TYPE strType);
	Media_Status (*Pause)(TTS_Handle *hdl);
	Media_Status (*Resume)(TTS_Handle *hdl);
	Media_Status (*Stop)(TTS_Handle *hdl);
	Media_Status (*Close)(TTS_Handle *hdl);
	Media_Status (*SetPitch)(TTS_Handle *hdl, kal_uint32 pitch);
	Media_Status (*SetVolume)(TTS_Handle *hdl, kal_uint32 volume);
	Media_Status (*SetSpeed)(TTS_Handle *hdl, kal_uint32 speed);
	Media_Status (*SetAttr)(TTS_Handle *hdl, kal_uint32 dwAttrId, kal_uint32 varAttrValue);
	Media_Status (*Process)(TTS_Handle *hdl, TTS_Event event);
	kal_uint32  errMsg;
	TTS_STR_TYPE   strType;
	TTS_LANG    lang;
	TTS_TYPE    type;
	TTS_GENDER  gender;
/*other internal variable*/
#if defined(SINOVOICE_TTS)
   kal_uint16 *voiceBuf;
   kal_uint32 buf_r;
   kal_uint32 buf_end;
#endif
#if defined(IFLY_TTS) || defined(SINOVOICE_TTS)
   kal_bool  dataReq;
   kal_uint16 aud_id;
#if defined(IFLY_TTS)
   void *customHdl;
#else//SINOVOICE
   kal_uint16 flag;
   unsigned long customHdl;
#endif
   void *text;
#endif
	kal_uint16 state;
   MHdl *spcmHdl;
   Media_Event event;
   kal_wchar dataDir[100];
   kal_bool  bProcessDone;
};

TTS_Handle *TTS_Open(void(*callback)(TTS_Handle *hdl, TTS_Event event, void *data), TTS_LANG lang, TTS_TYPE type, TTS_GENDER gender);
#endif
/*====================end of l1TTS interface ===================*/


#endif


