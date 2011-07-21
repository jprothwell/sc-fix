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
 *	afe.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Audio Manager
 *
 * Author:
 * -------
 * -------
 *
 *------------------------------------------------------------------------------
 *
 *
 *******************************************************************************/
#ifndef AM_H
#define AM_H


#define  ASP_TYPE_SPEECH      0
#define  ASP_TYPE_MELODY_8K   0
#define  ASP_TYPE_PCM_8K      0
#define  ASP_TYPE_PCM_HI      1
#define  ASP_TYPE_MELODY_16K  2
#define  ASP_TYPE_DAF         3
#define  ASP_TYPE_AAC         32
#define  ASP_TYPE_WMA         128
#define  ASP_TYPE_AACPLUS     256

#define  ASP_FS_8K            0x00
#define  ASP_FS_11K           0x11
#define  ASP_FS_12K           0x12
#define  ASP_FS_16K           0x20
#define  ASP_FS_22K           0x21
#define  ASP_FS_24K           0x22
#define  ASP_FS_32K           0x40
#define  ASP_FS_44K           0x41
#define  ASP_FS_48K           0x42

void     AM_Init( void );
bool     AM_AudioManager( bool handover,uint16 speech_mode );
void     AM_FlushQFunction( void );
void     AM_FillSilencePattern( volatile uint16 *addr, uint16 info, uint16 speech_mode );
uint16   AM_GetSpeechPatternLength( uint8 speech_mode );
void     AM_SpeechOn( void );
void     AM_SpeechOff( void );
bool     AM_IsSpeechOn( void );
void     AM_SetDAIMode ( uint8 mode );
bool     AM_IsDAIMode( void );
void     AM_WriteFirCoeffs( const int16 *in_coeff, const int16 *out_coeff );
void     AM_VMRecordOn( uint16 speech_mode );
void     AM_VMRecordOff( void );
void     AM_VMPlaybackOn( uint16 control, uint32 style );
void     AM_VMPlaybackOff( uint32 style );
void     AM_MelodyOn( void );
void     AM_MelodyOff( void );
void     AM_KeyToneOn( void );
void     AM_KeyToneOff( void );
void     AM_ToneOn( void );
void     AM_ToneOff( void );
void     AM_PCM8K_PlaybackOn( void );
void     AM_PCM8K_PlaybackOff( bool wait );
void     AM_PCM16K_PlaybackOn( void );        
void     AM_PCM16K_PlaybackOff( bool wait );
void     AM_PCM8K_RecordOn( void );
void     AM_PCM8K_RecordOff( bool wait );
void     AM_AudioPlaybackOn( uint16 asp_type, uint16 asp_fs );
void     AM_AudioPlaybackOff( bool wait );
int16    AM_IsAudioPlaybackOn( void );
bool     AM_IsVMPlaybackOn( void );
void     AM_VRRecordOn( uint32 trainPhase );
void     AM_VRRecordOff( uint32 trainPhase );
void     AM_MuteULSpeech( bool mute );
void     AM_PCM_EX_On( uint32 type );
void     AM_PCM_EX_Off( uint32 type );

// EC/AEC/EES control functions
void     AM_AEC_EES_Open( void );
void     AM_AEC_EES_Close( void );
void     AM_NormalMode_SetAEC( bool ec );
void     AM_NormalMode_SetAECParameter( uint16 aec_np_on, uint16 tcl );
void     AM_LoudSpkMode_SetEES( bool ec );
void     AM_LoudSpkMode_SetEESParameter( uint16 fe_atten, uint16 dt_atten );
void     AM_SetEchoSuppression( bool ec );
void     AM_SetEchoSuppressionParameter( uint16 timeConst, uint16 volConst );
void     AM_SetSBSD( bool state );

// Bluetooth earphone control functions
void     AM_BluetoothOn( uint8 bt_mode, uint8 bt_sync_type, uint8 bt_sync_length );
void     AM_BluetoothOff( void );
#endif


