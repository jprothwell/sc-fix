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
 *	AudioSrc.c
 *
 * Project:
 * --------
 *   MediaTeck GSM
 *
 * Description:
 * ------------
 
 *
 * Author:
 * -------
 * -------
 * 				
 *
 * HISTORY
 * -------
 * Created On 12 March
*******************************************************************************/
#define __NEWSIMULATOR
#include "stdc.h"
#include "l4dr1.h"

#ifdef MMI_ON_HARDWARE_P
//    #include "aud_defs.h"
	#include "l4dr.h"
	#include "queuegprot.h"
#endif

#include "audioinc.h"
#include "taskinit.h"
#include "protocolevents.h"
#include "unicodexdcl.h"
#include "downloadprotos.h"
#include "gpioinc.h"
#include "profilesgexdcl.h"             
#include "filesystemdef.h"
#include "filemgr.h"
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
#include "profilemgrgprot.h"
#include "profilesgexdcl.h"
#endif

#if defined( __MMI_HUMAN_VOICE_KEYPAD_TONE__ )
#include "resource_audio.h"
#include "settingprofile.h"
#endif

#include "profilegprots.h"

#undef __NEWSIMULATOR
#include "mdi_datatype.h"
#include "mdi_audio.h"

#include "idleappdef.h"

/*---------------global variable ---------------*/

#if defined( __MMI_HUMAN_VOICE_KEYPAD_TONE__ )
static U8 IsHumanVoiceKeyTonePlaying = 0;
extern U16   gKeyPadLockFlag ;
U8 CanPlayHumanVoiceKeyTone( void )
{
	return (g_idle_context.IsOnIdleScreen || g_idle_context.IsOnSimErrorScreen || 
		g_idle_context.IsOnSimErrorDialerScreen || g_idle_context.IsOnDialerScreen) &&
		(mdi_audio_is_idle() || IsHumanVoiceKeyTonePlaying || mdi_audio_is_paused())&&(!gKeyPadLockFlag);
}

void HumanVoiceKeyTonePlayHandler( mdi_result result )
{
	IsHumanVoiceKeyTonePlaying = 0;
}
#endif

#if defined(__MMI_CUST_KEYPAD_TONE__)
static U8 IsCustKeyTonePlaying = 0;
U8 CanPlayCustKeyTone( void )
{
	return (g_idle_context.IsOnIdleScreen || g_idle_context.IsOnSimErrorScreen || 
		g_idle_context.IsOnSimErrorDialerScreen || g_idle_context.IsOnDialerScreen) &&
		(mdi_audio_is_idle() || IsCustKeyTonePlaying);
}

void CustKeyTonePlayHandler( mdi_result result )
{
	IsCustKeyTonePlaying = 0;
}
#endif

/*********************************************************************
  * Function			: AudioPlayToneMDICallBack
  *
  * Purpose			: This function is call back function for AudioPlayToneWithCallBack
  *                              if play file fail.
  * 
  * Input Parameters	:  mdi_result result
  *
  * Output Parameters  : none
  *
  * Returns			: void
  *
 
  *
  *                                1. Incomming Call
  *                                2. Aalrm Tone
  *                                3. Message Tone
  *
  *                               File might play fail, cause of file broken or file not exist, 
  *                               This function will be call back if mdi play fail
  *
  *
**********************************************************************/
static U16 tone_soundId_callback = 0;
static U8 tone_style_callback = 0, tone_type_callback = 0, tone_volume_callback = 0;
void AudioPlayToneMDICallBack(mdi_result result)
{
   if( result != MDI_AUDIO_SUCCESS && result != MDI_AUDIO_TERMINATED &&
      result != MDI_AUDIO_RESUME && result != MDI_AUDIO_BLOCKED && result != MDI_AUDIO_END_OF_FILE  )
   {
       switch(tone_type_callback)
       {
        case INCOMING_CALL_TONE:
         mdi_audio_play_id_with_vol_path( gstartMidiId, 
                                                                    tone_style_callback,
                                                                    NULL, NULL,
                                                                    tone_volume_callback,
                                                                    MDI_DEVICE_SPEAKER_BOTH );

           break;
        case ALARM_TONE:
           mdi_audio_play_id_with_vol_path( (U16)(gstartMidiId+1), 
                                                                    tone_style_callback,
                                                                    NULL, NULL,
                                                                    tone_volume_callback,
                                                                    MDI_DEVICE_SPEAKER_BOTH );

           break;
         
        case MESSAGE_TONE:
           mdi_audio_play_id_with_vol_path( (U16)(gstartMiscToneId+4), 
                                                                    tone_style_callback,
                                                                    NULL, NULL,
                                                                    tone_volume_callback,
                                                                    MDI_DEVICE_SPEAKER_BOTH );
           break;

        default:
           ASSERT(0);
           break;
       }
   }
   else if((result == MDI_AUDIO_END_OF_FILE)&&(GetRingTypeEnum() == 1))
   {
	AudioPlayToneWithCallBackVolPath( tone_soundId_callback, tone_style_callback, tone_type_callback, GetRingVolumeLevel(), MDI_DEVICE_SPEAKER_BOTH );

   }
}



/*********************************************************************
  * Function			: AudioPlayToneWithCallBack
  *
  * Purpose			: This function is play tone function with callbakc for mdi interface
  * 
  * Input Parameters	:  U16 soundId, U8 style, U8 tone_type
  *
  * Output Parameters  : none
  *
  * Returns			: void
  *
 
  *
  *                                1. Incomming Call  ( StartRingTone()  )
  *                                2. Aalrm Tone ( playRequestedTone(ALARM_TONE) )
  *                                3. Message Tone ( playRequestedTone(MESSAGE_TONE) )
  *
  *                               File might play fail, cause of file broken or file not exist, 
  *                               This function will be call back register function if mdi play fail
  *                               Shall play default ringtone in callback function
  *
**********************************************************************/

void AudioPlayToneWithCallBack(U16 soundId, U8 style, U8 tone_type )
{
	AudioPlayToneWithCallBackVolPath( soundId, style, tone_type, GetRingVolumeLevel(), MDI_DEVICE_SPEAKER_BOTH );
}

/*********************************************************************
  * Function			: AudioPlayToneWithCallBackVolPath
  *
  * Purpose			: This function is play tone function with callbakc for mdi interface
  * 
  * Input Parameters	:  U16 soundId, U8 style, U8 tone_type, U8 volume, U8 path
  *
  * Output Parameters  : none
  *
  * Returns			: void
  *
 
  *
  *                                1. Incomming Call  ( StartRingTone()  )
  *                                2. Aalrm Tone ( playRequestedTone(ALARM_TONE) )
  *                                3. Message Tone ( playRequestedTone(MESSAGE_TONE) )
  *
  *                               File might play fail, cause of file broken or file not exist, 
  *                               This function will be call back register function if mdi play fail
  *                               Shall play default ringtone in callback function
  *
**********************************************************************/
void AudioPlayToneWithCallBackVolPath(U16 soundId, U8 style, U8 tone_type, U8 volume, U8 path)
{
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
	S8 buf[MAX_EXT_MELODY_FULLNAME_WIDTH];
#else
	S8 buf[(FMGR_MAX_PATH_LEN+1)*ENCODING_LENGTH];
#endif
	BOOL play_file = FALSE;

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
	if( soundId >= PMG_EXT_MELODY_BEGIN && soundId <= PMG_EXT_MELODY_END )
	{
		if( PmgGetFilePathNameByAudioId( buf, sizeof(buf), soundId ) == MMI_TRUE )
			play_file = TRUE;
	}
	else
#endif
	{
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
		if( GetAudioNameWithPathInFileSystem(soundId,(PS8)buf ) != NULL )
			play_file = TRUE;
#endif
	}

	if( play_file )
	{
		mdi_result result;
		tone_soundId_callback = soundId;
		tone_style_callback = style;
		tone_type_callback = tone_type;
		tone_volume_callback = volume;
		result=mdi_audio_play_file_with_vol_path( (void *)buf, 
			style, 
			NULL, 
			AudioPlayToneMDICallBack,
			tone_volume_callback,
			path,0);
		//mmi_trace(1,"chenhe,AudioPlayToneWithCallBackVolPath,result is %d ",result);


		/*chenhe comment it for bug 10757. for if as far as now,we only return MDI_AUDIO_END_OF_FILE when play a wave file which file size is only 44bytes.
			in this case, it leads to mmi task call the AudioPlayToneWithCallBackVolPath recursively many times, so mmi task stack overflow.
			i comment call the callback for we only return MDI_AUDIO_END_OF_FILE when play 44bytes wave,others will send a indication msg.
			in a all ,we should return success when play a file if it's over at now,send the finish ind by message!!*/
		if( result != MDI_AUDIO_SUCCESS &&result!=MDI_AUDIO_END_OF_FILE)
		{
			AudioPlayToneMDICallBack( result );
		}
	}
	else
	{
		mdi_audio_play_id_with_vol_path( soundId, 
			style,
			NULL, NULL,
			volume,
			path );
	}
}

void AudioPlayReqWithVolPath( U16 soundId, U8 style, U8 volume, U8 path )
{
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
	S8 buf[MAX_EXT_MELODY_FULLNAME_WIDTH];
#else
	S8 buf[(FMGR_MAX_PATH_LEN+1)*ENCODING_LENGTH];
#endif

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
	if(soundId>=PMG_EXT_MELODY_BEGIN && soundId<=PMG_EXT_MELODY_END)
	{
		if( PmgGetFilePathNameByAudioId(buf, sizeof(buf), soundId) == MMI_TRUE )
			mdi_audio_play_file_with_vol_path(buf, style, NULL, NULL, volume, path,0);
	}
	else
#endif
#if defined( __MMI_HUMAN_VOICE_KEYPAD_TONE__ )
	if( soundId >= MIN_HUMAN_VOICE_ID && soundId <= MAX_HUMAN_VOICE_ID )
	{
		audio_resource_struct* audio_resource_p = &resource_human_voice_tones[soundId - MIN_HUMAN_VOICE_ID];
		if( mdi_audio_play_string_with_vol_path((void*)audio_resource_p->data,
			audio_resource_p->len, 
			(U8)audio_resource_p->format,
			style,
			NULL,
			HumanVoiceKeyTonePlayHandler,
			volume,
			path ) == MDI_AUDIO_SUCCESS )
			IsHumanVoiceKeyTonePlaying = 1;
		else
			IsHumanVoiceKeyTonePlaying = 0;
	}
	else
#endif
#if defined( __MMI_CUST_KEYPAD_TONE__ )
	if( soundId >= MIN_KEYPAD_TONE_ID && soundId <= MAX_KEYPAD_TONE_ID )
	{
		audio_resource_struct* audio_resource_p = &resource_keypad_tones[soundId - MIN_KEYPAD_TONE_ID];
		if( mdi_audio_play_string_with_vol_path((void*)audio_resource_p->data,
			audio_resource_p->len, 
			(U8)audio_resource_p->format,
			style,
			NULL,
			CustKeyTonePlayHandler,
			volume,
			path ) == MDI_AUDIO_SUCCESS )
			IsCustKeyTonePlaying = 1;
		else
			IsCustKeyTonePlaying = 0;
	}
	else
#endif
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
	if( GetAudioNameWithPathInFileSystem( soundId, buf ) != NULL )
		mdi_audio_play_file_with_vol_path( buf, style, NULL, NULL, volume, path,0 );
	else
#endif
	{
#ifdef MMI_ON_HARDWARE_P
		mdi_audio_play_id_with_vol_path( soundId, style, NULL, NULL, volume, path );
#else
		MYQUEUE Message;
		mmi_eq_play_audio_req_struct *audioPlayReq;
 
		audioPlayReq = OslConstructDataPtr(sizeof (mmi_eq_play_audio_req_struct));
		audioPlayReq->sound_id = (U8)soundId;
		audioPlayReq->style = 2;//style;
		audioPlayReq->identifier= (U16)soundId;  
		
		Message.oslMsgId = PRT_EQ_PLAY_AUDIO_REQ;	
		Message.oslDataPtr = (oslParaType *)audioPlayReq;
		Message.oslPeerBuffPtr= NULL;
		Message.oslSrcId=MOD_MMI;
		Message.oslDestId=MOD_L4C;
		OslMsgSendExtQueue(&Message);
#endif
	}
}

/*********************************************************************
  * Function			: AudioPlayReq
  *
  * Purpose			: This function Play request tone
  * 
  * Input Parameters	:  U16 soundId, U8 style
  *
  * Output Parameters  : none
  *
  * Returns			: void
  *
 
  *
  *
**********************************************************************/

void AudioPlayReq(U16 soundId, U8 style)
{
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
        S8 buf[MAX_EXT_MELODY_FULLNAME_WIDTH];
#else
        S8 buf[(FMGR_MAX_PATH_LEN+1)*ENCODING_LENGTH];
#endif

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
	if(soundId>=PMG_EXT_MELODY_BEGIN && soundId<=PMG_EXT_MELODY_END)
	{
		if( PmgGetFilePathNameByAudioId(buf, sizeof(buf), soundId) == MMI_TRUE )
			mdi_audio_play_file(buf, style, NULL, NULL);
	}
	else
#endif
#if defined( __MMI_HUMAN_VOICE_KEYPAD_TONE__ )
	if( soundId >= MIN_HUMAN_VOICE_ID && soundId <= MAX_HUMAN_VOICE_ID )
	{
		audio_resource_struct* audio_resource_p = &resource_human_voice_tones[soundId - MIN_HUMAN_VOICE_ID];
          #if defined(__PROJECT_GALLITE_C01__)		
                audio_resource_p->len = audio_resource_p->len/4;
          #endif
		if( mdi_audio_play_string_with_vol_path((void*)audio_resource_p->data,
			audio_resource_p->len, 
#if defined(__PROJECT_GALLITE_C01__)
			MDI_FORMAT_WAV,
#else
            (U8)audio_resource_p->format,
#endif
			style,
			NULL,
			HumanVoiceKeyTonePlayHandler,
			GetKeypadVolumeLevel(),
			MDI_DEVICE_SPEAKER2 ) == MDI_AUDIO_SUCCESS )
			IsHumanVoiceKeyTonePlaying = 1;
		else
			IsHumanVoiceKeyTonePlaying = 0;
	}
	else
#endif
#if defined( __MMI_CUST_KEYPAD_TONE__ )
	if( soundId >= MIN_KEYPAD_TONE_ID && soundId <= MAX_KEYPAD_TONE_ID )
	{
		audio_resource_struct* audio_resource_p = &resource_keypad_tones[soundId - MIN_KEYPAD_TONE_ID];
		if( mdi_audio_play_string_with_vol_path((void*)audio_resource_p->data,
			audio_resource_p->len, 
			(U8)audio_resource_p->format,
			style,
			NULL,
			CustKeyTonePlayHandler,
			GetKeypadVolumeLevel(),
			MDI_DEVICE_SPEAKER2 ) == MDI_AUDIO_SUCCESS )
			IsCustKeyTonePlaying = 1;
		else
			IsCustKeyTonePlaying = 0;
	}
	else
#endif
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
	if( GetAudioNameWithPathInFileSystem( soundId, buf ) != NULL )
		mdi_audio_play_file( buf,  style, NULL, NULL );
	else
#endif
	{
#ifdef MMI_ON_HARDWARE_P
		mdi_audio_play_id(soundId, style, NULL, NULL);
#else
	MYQUEUE Message;
	mmi_eq_play_audio_req_struct *audioPlayReq;
 
	audioPlayReq = OslConstructDataPtr(sizeof (mmi_eq_play_audio_req_struct));
	audioPlayReq->sound_id = (U8)soundId;
		audioPlayReq->style = 2;//style;
	audioPlayReq->identifier= (U16)soundId;  
		
	Message.oslMsgId = PRT_EQ_PLAY_AUDIO_REQ;	
    Message.oslDataPtr = (oslParaType *)audioPlayReq;
    Message.oslPeerBuffPtr= NULL;
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C;
	OslMsgSendExtQueue(&Message);
#endif
	}
}	
		


/*********************************************************************
 Function			: AudioStopReq
 Purpose			: This function stops sound playing.
 Input Parameters	: U8 soundId
 Output Parameters  : none
 Returns			: void
 
**********************************************************************/
void AudioStopReq(U16 soundId)
{
	if(soundId>=FILESYSTEM_AUDIO_BASE && soundId<=FILESYSTEM_AUDIO_BASE_END)
	{
		StopAudioFileSystem(soundId);
	}
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
	else if(soundId>=PMG_EXT_MELODY_BEGIN && soundId<=PMG_EXT_MELODY_END)
	{
		mdi_audio_stop_file();
	}
#endif
#if defined( __MMI_HUMAN_VOICE_KEYPAD_TONE__ )
	else if( soundId >= MIN_HUMAN_VOICE_ID && soundId <= MAX_HUMAN_VOICE_ID )
		mdi_audio_stop_string();
#endif
	else
	{
#ifdef MMI_ON_HARDWARE_P
		mdi_audio_stop_id(soundId);
#else
		MYQUEUE Message;
		mmi_eq_stop_audio_req_struct* msg_p;
   
		msg_p = OslConstructDataPtr(sizeof(mmi_eq_stop_audio_req_struct));
		msg_p->sound_id = (U8)soundId;
		
		Message.oslMsgId = PRT_EQ_STOP_AUDIO_REQ;	
		Message.oslDataPtr = (oslParaType *)msg_p;
		Message.oslPeerBuffPtr= NULL;
		Message.oslSrcId=MOD_MMI;
		Message.oslDestId=MOD_L4C;
		OslMsgSendExtQueue(&Message);
#endif
	}
}	


void AudioPlayIMelodyString(U8 *string, U16 len, U8 style)
{
	MYQUEUE Message;
	mmi_eq_play_ext_imelody_req_struct *iMelodyPlayReq;
	if(len>MAX_RING_COMPOSE_LEN) return;
	iMelodyPlayReq = OslConstructDataPtr(sizeof (mmi_eq_play_ext_imelody_req_struct));
	memcpy(iMelodyPlayReq->imelody, string, len);
	iMelodyPlayReq->len=len;
	iMelodyPlayReq->play_style=style;
	Message.oslMsgId = PRT_EQ_PLAY_EXT_IMELODY_REQ;	
    Message.oslDataPtr = (oslParaType *)iMelodyPlayReq;
    Message.oslPeerBuffPtr= NULL;
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C;
	OslMsgSendExtQueue(&Message);
}

void AudioPlayIMelodyFile(U8 *file, U8 style)
{
	if(file)//!=NULL
	{
#if 1
      mdi_audio_play_file(file, style, NULL, NULL);
#else
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
}
}

void AudioPlayIMelodyFileWithId(U8 *file, U8 style, U16 nId)
{
	if(file)//!=NULL
	{
	MYQUEUE Message;
	mmi_eq_play_audio_by_name_req_struct *audioByNamePlayReq;
	if(pfnUnicodeStrlen((PS8)file)>MAX_MELODY_FILE_NAME) return;
	audioByNamePlayReq = OslConstructDataPtr(sizeof (mmi_eq_play_audio_by_name_req_struct));
	audioByNamePlayReq->style=style,
	audioByNamePlayReq->identifier=nId,
	
	pfnUnicodeStrcpy((PS8)audioByNamePlayReq->file_name,(PS8)file),
	Message.oslMsgId = PRT_EQ_PLAY_AUDIO_BY_NAME_REQ;	
    Message.oslDataPtr = (oslParaType *)audioByNamePlayReq;
    Message.oslPeerBuffPtr= NULL;
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C;
	OslMsgSendExtQueue(&Message);
}

}
void AudioStopIMelodyFile(U8 *file)
{
	if (file)
	{
#if 1
      mdi_audio_stop_file();
#else
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif	
}
}

//#if 0
#if 0
void PlayAudioMidiStream( U8* audioBuffer, U16 length, U8 style)
{
#ifdef MMI_ON_HARDWARE_P
	U8 format = MEDIA_SMF; //AUD_MEDIA_SMF;
	PlayAudioStream(audioBuffer, length, format, style );
#endif
}

void PlayAudioWavStream(  U8* audioBuffer, U16 length, U8 style)
{
#ifdef MMI_ON_HARDWARE_P
	U8 format = MEDIA_WAV; //AUD_MEDIA_WAV;
	PlayAudioStream(audioBuffer, length, format, style );
#endif
}

void PlayAudioDviStream(  U8* audioBuffer, U16 length, U8 style)
{
#ifdef MMI_ON_HARDWARE_P
	U8 format = MEDIA_DVI_ADPCM; //AUD_MEDIA_WAV;
	PlayAudioStream(audioBuffer, length, format, style );
#endif
}

void PlayAudioStream(U8* audioBuffer, U16 length, U8 format, U8 style )
{
#if 1
   mdi_audio_play_string((void *)audioBuffer, length, format, style, NULL, NULL);
#else
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
}

void StopAudioMidiStream(void)
{
	StopAudioStream();
}

void StopAudioWavStream(void)
{
	StopAudioStream();
}

void StopAudioDviStream(void)
{
	StopAudioStream();
}

void StopAudioStream(void)
{
#if 1
   mdi_audio_stop_string();
#else
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif	
}

//#endif
#endif
