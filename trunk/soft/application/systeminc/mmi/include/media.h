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
 *
 * Filename:
 * ---------
 * media.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Media file recording/playback
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
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef __MEDIA_H
#define __MEDIA_H

#include "l1audio.h"
#include "am.h"

#if !defined( MT6205B ) && !defined( MT6208 )
#include "ddload.h"      
#endif

#define MASTER_GAIN_FACTOR          12
#define MEDIA_RB_DEFAULT_THRESHOLD  200

/*
   Warning:  any new declared variable should be placed at last of this structure 
             because "mediaControl" is used in ROMSA
*/

typedef struct {
   union {
      Media_SMF_Param      smf;
      Media_iMelody_Param  imy;
      uint32               vm;
   } param;
   int32          max_swing;        /* 0 ~ 32767                     */
   int8           key_shift;
   uint8          level;            /* Digital volume control 0~100 */
   uint8          gpt;
   kal_bool           in_speech;
   Media_Format   format;
   Media_Event    end_status;
   uint16         rb_threshold;
   /* DSP ring buffer control variables */
   uint16         dsp_rb_base;
   uint16         dsp_rb_size;
   uint16         dsp_rb_end;
   uint16         dsp_last_sample;  /* The last sample written to DSP buffer by MCU, for last-sample padding */
   /* Ring buffer control variables */
   kal_bool           eof;              /* Flag indicating End-Of-File   */
   kal_bool           ob_flag_r;        /* if ob_flag_r is set, then the odd_byte for read is stored in odd_byte_r */
   kal_bool           ob_flag_w;        /* if ob_flag_w is set, then the odd_byte for write is stored in odd_byte_w */
   uint8          odd_byte_r;
   uint8          odd_byte_w;
   uint16         *rb_base;         /* Pointer to the ring buffer    */
   uint32         rb_size;          /* Size of the ring buffer(word) */
   uint32         write;            /* R/W by MMI, R by WAV driver   */
   uint32         read;             /* R by MMI, R/W by WAV driver   */
   uint32         file_len;   /* for wav recordig */
#ifdef   DSP_WT_SYN
   int16          Media_FIR_Melody32K_1[45];
   int16          Media_FIR_Melody32K_2[45];
#endif
   kal_bool           isDataConsumed;
   kal_bool           is_stereo;
   kal_bool           mono2stereo;
} mediaControl;


struct media_handle {
   kal_uint8      state;
   kal_bool       stop_flag;
   kal_int8       repeat_count;
   kal_uint8      volume_level;
   kal_uint8      index;
   kal_int32      start_time;
   kal_int32      current_time;
   kal_int32      stop_time;
   kal_uint32     data;
   void           (*handler)( Media_Handle *handle, Media_Event event );
   void           *param;
};

/*****************************************************************************
* Function Declaration
*****************************************************************************/
void  mediaInit( uint16 aud_id );
void  mediaHisr( uint16 flag );
int32 mediaGetFreeSpace( void );
int32 mediaGetDataCount( void );
void  mediaSetEvent( Media_Event event );
void  mediaHandler( void *data );
kal_bool  mediaKeytoneForbidden( void );
kal_bool  mediaIsDataConsumed( void );
mediaControl *mediaGetControl( void );

/* For Ring Buffer Access */
void  rb_ClearRB( void );
int32 rb_GetByteCount( void );
int32 rb_GetFreeSpace( void );
kal_bool  rb_GetByte( uint8* data_8 );
kal_bool  rb_GetWord( uint16* data_16 );
kal_bool  rb_GetDWord( uint32* data_32 );
kal_bool  rb_PeekByte( uint8* data_8 );
kal_bool  rb_PeekWord( uint16* data_16 );
kal_bool  rb_PeekDWord( uint32* data_32 );
kal_bool  rb_PutByte( uint8 data_8 );
kal_bool  rb_PutWord( uint16 data_16 );
kal_bool  rb_PutDWord( uint32 data_32 );
uint32 rb_SkipNByte( uint32 skip_count );
uint32 rb_SkipNWord( uint32 skip_count );
uint32 rb_GetWriteIndex( void );
uint32 rb_GetReadIndex( void );
uint32 rb_GetSize( void );
uint16 rb_GetOBFlag( void );
uint16 rb_GetOddByte( void );
int rb_PutNByte( const uint8 *buffer, int len );
int rb_GetNByte( uint8 *buffer, int len );
/* For Ring Buffer Access */

void  pcmPlay( mediaControl *ctrl );
void  pcmRecord( mediaControl *ctrl );
void  pcmStop( void );
void  pcmPlaybackHisr( void );
void  pcmRecordHisr( void );

Media_Status  wavPlay( mediaControl *mctrl );
void  wavStop( void );
void  wavPause( mediaControl *mctrl );
void  wavResume( mediaControl *mctrl );
void  wavPlaybackHisr( void );
void  wavRecord( mediaControl *ctrl );
void  wavRecordHisr( void );
void  wavGetFileHeader( uint32 *len );
void  wavUpdateLevel( void );
void  wavInit( uint16 aud_id );
void  wavSetEvent( Media_Event event );

Media_Status  auPlay( mediaControl *mctrl );
void  auStop( void );
void  auPause( mediaControl *mctrl );
void  auResume( mediaControl *mctrl );
void  auPlaybackHisr( void );
void  auUpdateLevel( void );

Media_Status  aiffPlay( mediaControl *mctrl );
void  aiffStop( void );
void  aiffPause( mediaControl *mctrl );
void  aiffResume( mediaControl *mctrl );
void  aiffPlaybackHisr( void );
void  aiffUpdateLevel( void );

Media_Status dafPlay( mediaControl *ctrl );
void  dafStop( void );
void  dafPause( mediaControl *mctrl );
Media_Status  dafResume( mediaControl *mctrl );
void  dafPlaybackHisr( void );
void  dafUpdateLevel( void );

Media_Status aacPlay( mediaControl *ctrl );
void  aacStop( void );
Media_Status  aacPause( mediaControl *mctrl );
Media_Status  aacResume( mediaControl *mctrl );
void  aacPlaybackHisr( void );
void  aacUpdateLevel( void );

void  vmRecord( mediaControl *ctrl, uint16 type );
Media_Status  vmPlay( mediaControl *ctrl );
void  vmStop( void );
void  vmRecordHisr( void );
void  vmPlaybackHisr( void );
void  vmPause( mediaControl *ctrl );
void  vmResume( mediaControl *ctrl );

void  amrRecord( mediaControl *ctrl, uint16 type );
Media_Status  amrPlay( mediaControl *ctrl );
void  amrStop( void );
void  amrRecordHisr( void );
void  amrPlaybackHisr( void );
void  amrPause( mediaControl *ctrl );
void  amrResume( mediaControl *ctrl );
void  amrUpdateLevel( void );

void  amr515Init( uint16 aud_id );
void  amr515Record( mediaControl *ctrl );
void  amr515Stop( void );
void  amr515Pause(mediaControl *mctrl);
void  amr515Resume(mediaControl *mctrl);
void  amr515RecordHisr( void );

void  smfInit( uint16 aud_id );
void  smfSetEvent( Media_Event event );
Media_Status smfStart( mediaControl *ctrl );
void  smfStop( void );

/*
void  smfSndInit( void );
Media_Status smfSndPlay( Media_SMF_SND_Param *param );
void  smfSndMix( void );
*/

void  imyInit( uint16 aud_id );
void  imySetEvent( Media_Event event );
Media_Status imyStart( mediaControl *ctrl );
void  imyStop( void );

Media_Status smfDspStart( mediaControl *ctrl );
void  smfDspStop( void );

Media_Status imyDspStart( mediaControl *ctrl );
void  imyDspStop( void );

#if defined(EXT_AUDIO_CHIP)
/* abstract audio adaptation layer */
void AbsExtAudChip_Init(uint16 aud_int);
Media_Status AbsExtAudChip_Play( mediaControl *ctrl );
Media_Status AbsExtAudChip_Stop( mediaControl *ctrl );
Media_Status AbsExtAudChip_Pause( mediaControl *ctrl );
Media_Status AbsExtAudChip_Resume( mediaControl *ctrl );
Media_Status AbsExtAudChip_SetVolume( mediaControl *ctrl, uint8 volume );
Media_Status AbsExtAudChip_Control( mediaControl *ctrl );

/* audio adaptation layer */
typedef struct {
   const uint8 *melody_data;
   int32 data_length;
   int16 repeat_times;
   Media_Format  format;
   struct {
      int16  cmd_type;
      uint16 cmd_len;
      const uint8  *cmd_data;
      uint8 *return_data;
   }command;
} AudChipControl;

void ExtAudChip_Init( void  );
#define AudChip_Status Media_Status
#define ExtAudChip_ReturnSuccess {return MEDIA_SUCCESS;}
#define ExtAudChip_ReturnFail {return MEDIA_FAIL;}

AudChip_Status ExtAudChip_Play( AudChipControl *audControl  );
AudChip_Status ExtAudChip_Stop( AudChipControl *audControl  );
AudChip_Status ExtAudChip_Pause( AudChipControl *audControl  );
AudChip_Status ExtAudChip_Resume( AudChipControl *audControl  );
AudChip_Status ExtAudChip_SetVolume( AudChipControl *audControl , uint8 volume );
AudChip_Status ExtAudChip_Control( AudChipControl *audControl  );
void ExtAudChip_Reset( void );

void ExtAudChip_SetEventHandler(void(*eventHandler)(void *param));
void ExtAudChip_SetEvent(void *param);

void ExtAudChip_NotifyError(void);
void ExtAudChip_NotifyStop(void);
void ExtAudChip_NotifyEOF(void);
void ExtAudChip_NotifyRepeated(void);


#if defined(YAMAHA_MA3)
void YamahaMa3_SpkAmpOn(void);
void YamahaMa3_SpkAmpOff(void);
#endif

#if defined(ROHM_8788)
void RegWrite( int nAdrs, int nData );
uint8 RegRead( int nAdrs );
#endif

#endif

void  ktInit( uint16 aud_id );
void  ktLock( void );
void  ktUnlock( void );
void  toneInit( uint16 aud_id );

void JSmf_Init( void );
void JImy_Init( void );
void JTone_Init( mediaControl *ctrl );
void JWav_Init( mediaControl *ctrl );
void JXWav_Init( mediaControl *ctrl );

/* Obsolete functions: MIDI playback interface using GMI interface */
Media_Status midiPlay( mediaControl *ctrl );
void  midiStop( void );

void iMelodyInit( uint16 aud_id );
Media_Status iMelodyPlay( mediaControl *ctrl );
void iMelodyStop( void );

/* A/V Sync interface */
#define VERIFY_AVSYNC 0
void Media_A2V_LISR( void );

void   FMR_Init(uint16 aud_id);

void   vrsdInit( void );

#define VERIFY_DATA_TO_DSP 0

/* VRSI interface */
void vrsiInit( void ) ;

#endif /* __MEDIA_H */


