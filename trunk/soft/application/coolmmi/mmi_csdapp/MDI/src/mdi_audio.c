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
 *   mdi_audio.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Audio interfce source file
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
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "mmi_features.h"

#define __NEWSIMULATOR
#include "mmi_data_types.h"
#include "protocolevents.h"
#include "eventsgprot.h"
#include "queuegprot.h"
#include "timerevents.h"
#include "profilegprots.h"
#include "debuginitdef.h"
#include "stack_config.h"

#include "mdi_datatype.h"
#include "mdi_audio.h"
#ifdef MMI_ON_HARDWARE_P
#include "resource_audio.h"
#include "aud_defs.h"
#include "med_global.h"
#include "med_api.h"
#include "med_main.h"
#include "aud_main.h"
#endif
#include "med_struct.h"
#include "profilegprots.h"
#include "soundeffect.h"
#undef __NEWSIMULATOR
#include "mmi_trace.h"
#include "mci.h"


    #if defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_A2DP_SUPPORT__)
	#include "av_bt.h"
#endif
    #if defined(__MMI_BT_COOLSAND_SUPPORT__)
	#include "btmmiscrgprots.h"
	#endif
enum{
   MDI_AUDIO_PLAY_KT,
   MDI_AUDIO_PLAY_TONE,
   MDI_AUDIO_PLAY_ID,
   MDI_AUDIO_PLAY_STRING,
   MDI_AUDIO_PLAY_FILE,
   MDI_AUDIO_RECORD_FILE,
   MDI_AUDIO_RECORD_FM_RADIO,   
   MDI_AUDIO_PLAY_FM_RADIO,
   MDI_AUDIO_RECORD_VR_VOICE,   
   MDI_AUDIO_VRSI_PLAY_TTS,
   MDI_AUDIO_VRSI_PLAY_TAG, 
   MDI_AUDIO_VRSI_RECORD,  
   MDI_AUDIO_PLAY_TTS,             /* Play a general TTS */

   MDI_NO_AUDIO_MODE  
};

#define MDI_BLOCKING_STOP

#define SRC_LMMI 0
#define MAX_BACKGROUND_HANDLER 3
static mdi_bg_callback background_handler[MAX_BACKGROUND_HANDLER];
static mdi_callback currentHandler;
static mdi_callback previousHandler;

static U16 mdi_audio_state;
INT32 mdi_audio_file_playStyle;
static U16 mdi_audio_mode;
static U16 mdi_audio_id;
static BOOL mdi_speech_on;
static BOOL backgroud_enabled;
#if defined(__MMI_MAINLCD_220X176__)
static BOOL fm_backgroud_enabled;
#endif
static BOOL blocked;
static U16  identifier;

static void (*mdi_fmr_check_is_valid_stop_hdlr)(BOOL is_valid);
static void (*mdi_fmr_get_signal_level_hdlr)(U8 sig_lvl);

#if defined( DIGIT_TONE_SUPPORT ) || defined(SIMPLE_TTS)
#define MDI_MAX_PLAY_TEXT_LEN	60

typedef enum {
	MDI_AUDIO_PLAY_TEXT_IDLE_STATE,
	MDI_AUDIO_PLAY_TEXT_PLAY_STATE,
	MDI_AUDIO_PLAY_TEXT_WAIT_STATE
} mdi_audio_play_text_state_enum;

typedef struct
{
	U16 identifier;
	U16	words[MDI_MAX_PLAY_TEXT_LEN];
	S32 len;
	S32 cursor;
	S32 msecs;
	mdi_callback handler;
	mdi_audio_play_text_state_enum state; // 0: idle, 1: playing, 2: waiting for time out to play
	U8 volume;
	U8 path;
} mdi_audio_play_text_struct;
mdi_audio_play_text_struct mdi_audio_play_text_context;
#endif

#ifdef VR_ENABLE
media_vr_rcg_result_ind_struct* mdi_audio_vr_rcg_result_msg = NULL;
media_vr_trn_result_ind_struct* mdi_audio_vr_trn_result_msg = NULL;
#endif

#ifdef VRSI_ENABLE
static mdi_callback vrsiHandler;
#endif 

 
BOOL fmr_play_from_bg_to_fg = FALSE;
extern audio_mic_volume_struct g_settings_audio_mic_volume;
extern U16 gcurrentaudioId;
extern UINT32 aud_getFilePlayPercent(void);
extern UINT8 audioplayer_restore_flag;
extern void mmi_audply_reset_play_seconds(U32 play_seconds);
extern U32 mdi_audio_set_file_data_progress(U32 progress);
/*========= static function prototype ===========*/
void mdi_audio_play_string_cnf(void* inMsg);
void mdi_audio_play_finish_ind(void* inMsg);
void mdi_audio_play_finish_ind1(void* inMsg);
void mdi_audio_record_finish_ind(void* inMsg);
void mdi_audio_background_timeout_handler(void);
void mdi_send_msg(U32 msg_id,void *local_param_ptr, void *peer_buf_ptr);
void mdi_audio_play_timeout_handler( void );
mdi_result mdi_audio_play_current_word( void );
extern void aud_tone_stop(void);
extern void AudioPlayReq(U16 soundId, U8 style);

#ifdef __FF_AUDIO_SET__
extern void MusicEndAudioMode() ;
#endif

#ifdef __MMI_BT_PROFILE__
typedef struct
{
    mdi_bt_callback open_hdlr[2];
    mdi_bt_callback close_hdlr[2];
    U8 state[2];
    BOOL pending_connect[2];
    U16 connect_id[2];
} mdi_audio_bt_cntx_struct;
mdi_audio_bt_cntx_struct mdi_audio_bt_cntx;
extern U8 g_ssc_bt_a2dp_mode;   /* defined in SSCStringHandle.c */
extern void aud_send_bt_audio_open_req(
                module_type src_mod_id,
                kal_uint8 profile,
                kal_uint8 mode,
                kal_uint16 connect_id);
extern void aud_send_bt_audio_close_req(module_type src_mod_id, kal_uint8 profile);
extern void aud_send_bt_audio_turn_on_req(module_type src_mod_id, kal_uint8 profile);
extern void aud_send_bt_audio_turn_off_req(module_type src_mod_id, kal_uint8 profile);
#endif /* __MMI_BT_PROFILE__ */ 
/*========= functions =====================*/

/******************************************************************
* FUNCTION
*    mdi_audio_init
* DESCRIPTION
*    This function is to initialize the mdi audio interface.
* PARAMETERS
*    None
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_init(void)
{
   S16 i;

   mdi_audio_state=MDI_AUDIO_IDLE;
   backgroud_enabled=TRUE;
   blocked=FALSE;
   identifier=0;
   
   for(i=0;i<MAX_BACKGROUND_HANDLER;i++)
      background_handler[i]=NULL;

#ifdef __MMI_BT_PROFILE__  //zhu jianguo added it. 2009.6.23
    for (i = 0; i < 2; i++)
    {
        mdi_audio_bt_cntx.open_hdlr[i] = mdi_audio_bt_cntx.close_hdlr[i] = NULL;
        mdi_audio_bt_cntx.state[i] = MDI_AUDIO_BT_STATE_IDLE;
        mdi_audio_bt_cntx.pending_connect[i] = FALSE;
    }
#endif /* __MMI_BT_PROFILE__ */ 
#if defined( DIGIT_TONE_SUPPORT ) || defined(SIMPLE_TTS)
   mdi_audio_play_text_context.identifier = 0;
   mdi_audio_play_text_context.state = MDI_AUDIO_PLAY_TEXT_IDLE_STATE;
   mdi_audio_play_text_context.len = 0;
   mdi_audio_play_text_context.cursor = -1;
   mdi_audio_play_text_context.msecs = 0;
   mdi_audio_play_text_context.handler = NULL;
   mdi_audio_play_text_context.volume = 0;
   mdi_audio_play_text_context.path = MDI_DEVICE_SPEAKER;
   memset( mdi_audio_play_text_context.words, 0, sizeof(mdi_audio_play_text_context.words) );
#endif
}
/*****************************************************************************
 * FUNCTION
 *  mdi_audio_init_event_hdlrs
 * DESCRIPTION
 *  This function is to set event hdlrs.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mdi_audio_init_event_hdlrs(void)
{
#ifdef __MMI_BT_PROFILE__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef MMI_ON_HARDWARE_P /* zhuoxz,2009-7-29 */
    SetProtocolEventHandler(mdi_audio_bt_close_stream_cnf, MSG_ID_MEDIA_BT_AUDIO_CLOSE_CNF);
    SetProtocolEventHandler(mdi_audio_bt_close_stream_ind, MSG_ID_MEDIA_BT_AUDIO_CLOSE_IND);
    SetProtocolEventHandler(mdi_audio_bt_open_stream_cnf, MSG_ID_MEDIA_BT_AUDIO_OPEN_CNF);
    SetProtocolEventHandler(mdi_audio_bt_open_stream_ind, MSG_ID_MEDIA_BT_AUDIO_OPEN_IND);
#endif /*#ifdef MMI_ON_HARDWARE_P*/
    #if defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_A2DP_SUPPORT__)
    av_bt_init();
    #endif
#endif 
    /* __MMI_BT_PROFILE__ */ 
}

/******************************************************************
* FUNCTION
*    mdi_get_event
* DESCRIPTION
*    This function is to convert event to mdi result.
* PARAMETERS
*    U8 result
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_get_event(U8 result)
{
   return (mdi_result)result;
}

/******************************************************************
* FUNCTION
*    mdi_start_background_timer
* DESCRIPTION
*    This function is to start timer for background play.
* PARAMETERS
*    None
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_start_background_timer(void)
{
   if (!blocked && backgroud_enabled && !mdi_speech_on)
	   StartTimer(TIMER_PROFILES_ALERT_PLAY,3000,mdi_audio_background_timeout_handler);
}

/******************************************************************
* FUNCTION
*    mdi_stop_background_timer
* DESCRIPTION
*    This function is to stop timer for background play.
* PARAMETERS
*    None
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_stop_background_timer(void)
{
	StopTimer(TIMER_PROFILES_ALERT_PLAY);
}

/******************************************************************
* FUNCTION
*    mdi_audio_check_states_and_handlers
* DESCRIPTION
*    This function is to check states and handlers before play or record request is executed
* PARAMETERS
*    mdi_result* result_p
* RETURNS
*    BOOL
* GLOBALS AFFECTED
*  
******************************************************************/
BOOL mdi_audio_check_states_and_handlers( mdi_result* result_p )
{
	mmi_trace(g_sw_MP3,"mdi_audio_check_states_and_handlers,blocked:%d,mdi_audio_state:%d,mdi_speech_on:%d",blocked,mdi_audio_state, mdi_speech_on);

	if( blocked )
	{
		*result_p = MDI_AUDIO_BLOCKED;
		return FALSE;
	}
	else if( mdi_audio_state == MDI_AUDIO_RECORD || mdi_audio_state == MDI_AUDIO_RECORD_PAUSED ||
		mdi_audio_state == MDI_AUDIO_VRSI_PROCESS || mdi_speech_on )
	{
		*result_p = MDI_AUDIO_BUSY;
		return FALSE;
	}
	else if( currentHandler && (mdi_audio_state == MDI_AUDIO_PLAY || mdi_audio_state == MDI_AUDIO_PLAY_PAUSED
#if defined( DIGIT_TONE_SUPPORT ) || defined(SIMPLE_TTS)
		|| mdi_audio_play_text_context.state == MDI_AUDIO_PLAY_TEXT_WAIT_STATE
#endif
		) )
	{
		currentHandler( MDI_AUDIO_TERMINATED );
		if( mdi_audio_state == MDI_AUDIO_PLAY && mdi_audio_mode == MDI_AUDIO_PLAY_FM_RADIO )
			mdi_fmr_power_off();
	}

//	*result_p = MDI_AUDIO_SUCCESS;
	return TRUE;
}

/******************************************************************
* FUNCTION
*    mdi_audio_play_id
* DESCRIPTION
*    This function is to play audio id.
* PARAMETERS
*    U16 audio_id, U8 play_style, mdi_handle* handle_p, mdi_callback handler
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
extern pBOOL isInCall(void);
extern U8 GetModeSpeechVolume(void);

mdi_result mdi_audio_play_id(U16 audio_id, U8 play_style, mdi_handle* handle_p, mdi_callback handler)
{
#ifdef MMI_ON_HARDWARE_P
   aud_play_id_struct id_param;
#endif
   mdi_result  result;
   
   //PRINT_INFORMATION(("mdi_audio_play_id, id=%d, state=%d\n",audio_id,mdi_audio_state));

   if (blocked)
      return MDI_AUDIO_BLOCKED;

   if (audio_id>MAX_TONE_ID)
   {
      if( !mdi_audio_check_states_and_handlers( &result ) )
         return result;

      currentHandler=handler;
      mdi_audio_state=MDI_AUDIO_PLAY;
      mdi_audio_mode=MDI_AUDIO_PLAY_ID;
      mdi_audio_id = audio_id;
      ++identifier;
      mdi_stop_background_timer();
   }

#ifdef MMI_ON_HARDWARE_P
   id_param.audio_id = (U8)audio_id;
   id_param.play_style = play_style;
   
	if(id_param.audio_id <= TONE_KEY_CLICK)//key tone
	{
		id_param.volume=GetKeypadVolumeLevel();
	}
	else if(id_param.audio_id>TONE_KEY_CLICK && id_param.audio_id<=MAX_TONE_ID) //warning tone
	{
		if(isInCall() == TRUE)
		{
			id_param.volume=GetModeSpeechVolume();
		}
		else
		{
			id_param.volume = LEVEL7;
		}
	}
	//jiashuo add for jessamine,20080604,puzzle game sound problem
	else if((id_param.audio_id == FNG_PUZZLE_SOUND)||(id_param.audio_id == FNG_UFO_SOUND))
	{
		id_param.volume=2;
	}
    else if((MIN_CAMERA_SND_ID<= id_param.audio_id)&&(id_param.audio_id <= MAX_CAMERA_SND_ID))
    {
		id_param.volume=4;
	}
	else
	{
		id_param.volume=GetRingVolumeLevel();
	}
	id_param.output_path=MDI_DEVICE_SPEAKER2;
	id_param.identifier= (U16)(identifier); 
	if((result=media_aud_play_id(MOD_MMI, &id_param))==MED_RES_OK)
	{
		SetProtocolEventHandler(mdi_audio_play_finish_ind, MSG_ID_L4AUD_AUDIO_PLAY_FINISH_IND);
		return MDI_AUDIO_SUCCESS;
	}
	else
	{
        if (audio_id>MAX_TONE_ID)
        {
    		currentHandler=NULL;
    		mdi_audio_state=MDI_AUDIO_IDLE;
    		mdi_start_background_timer();
        }
		return mdi_get_event((U8)result);
	}
#else
   return MDI_AUDIO_SUCCESS;
#endif
}





/*****************************************************************************
 * FUNCTION
 *  mdi_audio_sync_volume_from_bt
 * DESCRIPTION
 *  7-Level <- 16 Level Mapping 
 *    0 <-    0
 *    1 <-    1   2   3
 *    2 <-    4   5   6
 *    3 <-    7   8   9
 *    4 <-   10  11  12
 *    5 <-   13  14
 *    6 <-   15
 * PARAMETERS
 *   bt_volume: bluetooth volume from 0 ~ 15
 * RETURNS
 *  void
 *****************************************************************************/
U8 mdi_audio_bt_get_volume_level_from_type(U8 bt_volume, U8 volume_lvl_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 app_volume = 0;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    /* 16 level */
    if(volume_lvl_type == MDI_AUD_VOL_LEVEL_EXTEND)
    {
        app_volume = bt_volume;
    }
    /* 16 level + mute */
    else if(volume_lvl_type == MDI_AUD_VOL_LEVEL_EXTEND_MUTE)
    {
        app_volume = bt_volume + 1;
    }
    else /* MDI_AUD_VOL_LEVEL_NORMAL || MDI_AUD_VOL_LEVEL_NORMAL_MUTE */
    {
        switch(bt_volume)
        {
            case 0:
                app_volume = 0;
                break;
            case 1:
            case 2:
            case 3:
                app_volume = 1;
                break;
            case 4:
            case 5:            
            case 6:
                app_volume = 2;
                break;
            case 7:
            case 8:
            case 9:
                app_volume = 3;
                break;
            case 10:
            case 11:
            case 12:
                app_volume = 4;
                break;
            case 13:
            case 14:            
                app_volume = 5;
                break;
            case 15:
                app_volume = 6;
                break;
            default:
                ASSERT(0);
        }    

        if(volume_lvl_type == MDI_AUD_VOL_LEVEL_NORMAL_MUTE)
        {
            app_volume = app_volume + 1;
        }
    }

    return app_volume;
}


/*****************************************************************************
 * FUNCTION
 *  mdi_audio_set_volume_to_bt
 * DESCRIPTION
 *  
 *  7-Level -> 16 Level Mapping 
 *    0 ->    0
 *    1 ->    2
 *    2 ->    5
 *    3 ->    7
 *    4 ->   10
 *    5 ->   13
 *    6 ->   15
 *
 * PARAMETERS
 *   volume: 7-level or 16-level volume from application
 * RETURNS
 *  void
 *****************************************************************************/
void mdi_audio_set_volume_to_bt(U8 volume)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 volume_level, bt_volume = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//    MMI_TRACE(MMI_MEDIA_TRC_G4_MDI, MDI_AUDIO_TRC_SET_VOLUME_TO_BT, volume);

    mmi_trace(1,"mdi_audio_set_volume_to_bt,  volume = %d ",volume); 

    /* Get actual volume level value */
    volume_level = MDI_AUD_VOL_GET_EX_LEVEL(volume);

    /* 7-Level volume + Level 0 is mute, do not set to bt for mute level */
    if(MDI_AUD_VOL_IS_MUTE_LEVEL(volume))
    {
        if(volume_level == 0)
        {
            return;
        }
        else
        {
            volume_level = volume_level - 1;
        }
    }

    /* 16-Level volume */
    if(MDI_AUD_VOL_IS_EX_LEVEL(volume))
    {
        bt_volume = volume_level;
    }
    /* 7-Level volume */
    else
    {

        mmi_trace(1,"mdi_audio_set_volume_to_bt,  volume_level = %d ",volume_level); 

        /* Use previous bt 16-level volume to sync back to BT to resolve mapping problem */
       /* if(volume_level == mdi_audio_bt_get_volume_level_from_type(g_mdi_aud_last_bt_vol, MDI_AUD_VOL_LEVEL_NORMAL))
        {
            bt_volume = g_mdi_aud_last_bt_vol;
        }
        else*/
    //    {
            switch(volume_level)
            {
                case 0: bt_volume = 0; break;
                case 1: bt_volume = 2; break;
                case 2: bt_volume = 5; break;
                case 3: bt_volume = 7; break;
                case 4: bt_volume = 10; break;
                case 5: bt_volume = 13; break;
                case 6: bt_volume = 15; break;
                default:
                //    ASSERT(0);
                       bt_volume = 15; break;
            }
        //}
    }

    mmi_bt_hfp_set_speaker_vol((U32)bt_volume);
}
/******************************************************************
* FUNCTION
*    mdi_audio_play_id_with_vol_path
* DESCRIPTION
*    This function is to play audio id along with volume and output path.
* PARAMETERS
*    U16 audio_id, U8 play_style, mdi_handle* handle_p, mdi_callback handler
*    U8 volume, U8 path
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_play_id_with_vol_path(U16 audio_id, 
                                                U8 play_style, 
                                                mdi_handle* handle_p, 
                                                mdi_callback handler,
                                                U8 volume,
                                                U8 path)
{
#ifdef MMI_ON_HARDWARE_P
   aud_play_id_struct id_param;
#endif
   mdi_result  result;
   
   PRINT_INFORMATION(("mdi_audio_play_id, id=%d, state=%d blocked=%d\n",audio_id,mdi_audio_state, blocked));

   if (blocked)
      return MDI_AUDIO_BLOCKED;

   if (audio_id>MAX_TONE_ID)
   {
      if( !mdi_audio_check_states_and_handlers( &result ) )
         return result;
      
      currentHandler=handler;
      mdi_audio_state=MDI_AUDIO_PLAY;
      mdi_audio_mode=MDI_AUDIO_PLAY_ID;
      mdi_audio_id = audio_id;
      ++identifier;
      mdi_stop_background_timer();
   }

#if 0
   /* All sound shall go through bluetooth earphone. (Except keypad tone) */
#ifdef __MMI_BT_AUDIO_VIA_SCO__
#ifndef __MMI_BT_SCO_KEY_TONE__
    if(audio_id > TONE_KEY_CLICK) /* Key tone does not go to BT headset */
#endif
    {
        mmi_profiles_bt_connect_audio_via_sco();
    }
#endif /*__MMI_BT_AUDIO_VIA_SCO__*/
#endif


#ifdef MMI_ON_HARDWARE_P
   id_param.audio_id = (U8)audio_id;
   id_param.play_style = play_style;
   id_param.volume=volume;
   id_param.output_path=path;
   id_param.identifier= (U16)(identifier); 
   if((result=media_aud_play_id(MOD_MMI, &id_param))==MED_RES_OK)
   {
      SetProtocolEventHandler(mdi_audio_play_finish_ind, MSG_ID_L4AUD_AUDIO_PLAY_FINISH_IND);
      return MDI_AUDIO_SUCCESS;
   }
   else
   {
      if (audio_id>MAX_TONE_ID)
      {
          currentHandler=NULL;
          mdi_audio_state=MDI_AUDIO_IDLE;
          mdi_start_background_timer();
      }
      return mdi_get_event((U8)result);
   }
#else
   return MDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_play_string
* DESCRIPTION
*    This function is to play audio string.
* PARAMETERS
*    void* audio_data, U32 len, U8 format, U8 play_style, mdi_handle* handle_p, mdi_callback handler
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_play_string(void* audio_data, U32 len, U8 format, U8 play_style, mdi_handle* handle_p, mdi_callback handler)
{
#ifdef MMI_ON_HARDWARE_P
   aud_play_string_struct string_param;
#endif
   mdi_result  result;

   //PRINT_INFORMATION(("mdi_audio_play_string, format=%d, state=%d\n",format,mdi_audio_state));

   if( !mdi_audio_check_states_and_handlers( &result ) )
      return result;
         
   mdi_stop_background_timer();
   ++identifier;
   
#ifdef MMI_ON_HARDWARE_P
   string_param.data_p = (U8*)audio_data;
   string_param.len = len;
   string_param.format = format;
   string_param.play_style = play_style;
   string_param.volume=GetRingVolumeLevel();
   string_param.output_path=MDI_DEVICE_SPEAKER2;
   string_param.identifier= (U16)identifier; 
   string_param.blocking=TRUE;
   if((result=media_aud_play_string(MOD_MMI, &string_param))==MED_RES_OK)
   {
      currentHandler=handler;
      mdi_audio_state=MDI_AUDIO_PLAY;
      mdi_audio_mode=MDI_AUDIO_PLAY_STRING;
      SetProtocolEventHandler(mdi_audio_play_finish_ind, MSG_ID_L4AUD_AUDIO_PLAY_FINISH_IND);
      return MDI_AUDIO_SUCCESS;
   }
   else
   {
      currentHandler=NULL;
      mdi_audio_state=MDI_AUDIO_IDLE;
      mdi_start_background_timer();
      return mdi_get_event((U8)result);
   }
#else
   currentHandler=handler;
   mdi_audio_state=MDI_AUDIO_PLAY;
   mdi_audio_mode=MDI_AUDIO_PLAY_STRING;
   return MDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_play_string_with_vol_path
* DESCRIPTION
*    This function is to play audio string along with volume and output path.
* PARAMETERS
*    void* audio_data, U32 len, U8 format, U8 play_style, mdi_handle* handle_p, mdi_callback handler
*    U8 volume, U8 path
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_play_string_with_vol_path(void* audio_data, 
                                                U32 len, 
                                                U8 format, 
                                                U8 play_style, 
                                                mdi_handle* handle_p, 
                                                mdi_callback handler,
                                                U8 volume,
                                                U8 path)
{
#ifdef MMI_ON_HARDWARE_P
   aud_play_string_struct string_param;
#endif
   mdi_result  result;

   //PRINT_INFORMATION(("mdi_audio_play_string, format=%d, state=%d\n",format,mdi_audio_state));

   if( !mdi_audio_check_states_and_handlers( &result ) )
      return result;

   mdi_stop_background_timer();
   ++identifier;
      
#ifdef MMI_ON_HARDWARE_P
   string_param.data_p = (U8*)audio_data;
   string_param.len = len;
   string_param.format = format;
   string_param.play_style = play_style;
   string_param.volume=volume;
   string_param.output_path=path;
   string_param.identifier= (U16)identifier; 
   string_param.blocking=TRUE;
   if((result=media_aud_play_string(MOD_MMI, &string_param))==MED_RES_OK)
   {
      currentHandler=handler;
      mdi_audio_state=MDI_AUDIO_PLAY;
      mdi_audio_mode=MDI_AUDIO_PLAY_STRING;
      SetProtocolEventHandler(mdi_audio_play_finish_ind, MSG_ID_L4AUD_AUDIO_PLAY_FINISH_IND);
      return MDI_AUDIO_SUCCESS;
   }
   else
   {
      currentHandler=NULL;
      mdi_audio_state=MDI_AUDIO_IDLE;
      mdi_start_background_timer();
      return mdi_get_event((U8)result);
   }
#else
   currentHandler=handler;
   mdi_audio_state=MDI_AUDIO_PLAY;
   mdi_audio_mode=MDI_AUDIO_PLAY_STRING;
   return MDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_play_string_with_vol_path_non_block
* DESCRIPTION
*    This function is to play audio string along with volume and output path.
* PARAMETERS
*    void* audio_data, U32 len, U8 format, U8 play_style, mdi_handle* handle_p, mdi_callback handler
*    U8 volume, U8 path
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_play_string_with_vol_path_non_block(void* audio_data, 
                                                U32 len, 
                                                U8 format, 
                                                U8 play_style, 
                                                mdi_handle* handle_p, 
                                                mdi_callback handler,
                                                U8 volume,
                                                U8 path)
{
#ifdef MMI_ON_HARDWARE_P
   aud_play_string_struct string_param;
#endif
   mdi_result  result;

   //PRINT_INFORMATION(("mdi_audio_play_string, format=%d, state=%d\n",format,mdi_audio_state));

   if( !mdi_audio_check_states_and_handlers( &result ) )
      return result;

   mdi_stop_background_timer();
   ++identifier;

    /* All sound shall go through bluetooth earphone */
#ifdef __MMI_BT_AUDIO_VIA_SCO__
    mmi_profiles_bt_connect_audio_via_sco();
#endif


      
#ifdef MMI_ON_HARDWARE_P
   string_param.data_p = (U8*)audio_data;
   string_param.len = len;
   string_param.format = format;
   string_param.play_style = play_style;
   string_param.volume=volume;
   string_param.output_path=path;
   string_param.identifier= (U16)identifier; 
   string_param.blocking=FALSE;
   if((result=media_aud_play_string(MOD_MMI, &string_param))==MED_RES_OK)
   {
      currentHandler=handler;
      mdi_audio_state=MDI_AUDIO_PLAY;
      mdi_audio_mode=MDI_AUDIO_PLAY_STRING;
      SetProtocolEventHandler(mdi_audio_play_finish_ind, MSG_ID_L4AUD_AUDIO_PLAY_FINISH_IND);
      return MDI_AUDIO_SUCCESS;
   }
   else
   {
      currentHandler=NULL;
      mdi_audio_state=MDI_AUDIO_IDLE;
      mdi_start_background_timer();
      return mdi_get_event((U8)result);
   }
#else
   currentHandler=handler;
   mdi_audio_state=MDI_AUDIO_PLAY;
   mdi_audio_mode=MDI_AUDIO_PLAY_STRING;
   return MDI_AUDIO_SUCCESS;
#endif
}



/******************************************************************
* FUNCTION
*    mdi_audio_play_file
* DESCRIPTION
*    This function is to play audio file.
* PARAMETERS
*    void* file_name, U8 play_style, mdi_handle* handle_p, mdi_callback handler
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_play_file(void* file_name, U8 play_style, mdi_handle* handle_p, mdi_callback handler)
{
#ifdef MMI_ON_HARDWARE_P
   aud_play_file_struct file_param;
#endif
   mdi_result  result;

   //PRINT_INFORMATION(("mdi_audio_play_file, state=%d\n",mdi_audio_state));

   if( !mdi_audio_check_states_and_handlers( &result ) )
      return result;

   mdi_stop_background_timer();
   ++identifier;
#ifdef MMI_ON_HARDWARE_P
   file_param.file_name_p = (UI_string_type)file_name;
   file_param.play_style = play_style;
   file_param.volume=GetRingVolumeLevel();
   file_param.output_path=MDI_DEVICE_SPEAKER2;
   file_param.identifier= (U16)identifier;
// set both start_offset and end_offset to 0 for backward compatible
   file_param.start_offset = file_param.end_offset = 0;
   if((result=media_aud_play_file(MOD_MMI, &file_param))==MED_RES_OK)
   {
      currentHandler=handler;
      mdi_audio_state=MDI_AUDIO_PLAY;
      mdi_audio_mode=MDI_AUDIO_PLAY_FILE;
      SetProtocolEventHandler(mdi_audio_play_finish_ind1, MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND);
      return MDI_AUDIO_SUCCESS;
   }
   else
   {
      currentHandler=NULL;
      mdi_audio_state=MDI_AUDIO_IDLE;
      mdi_start_background_timer();
      return mdi_get_event((U8)result);
   }
#else
   currentHandler=handler;
   mdi_audio_state=MDI_AUDIO_PLAY;
   mdi_audio_mode=MDI_AUDIO_PLAY_FILE;
   return MDI_AUDIO_SUCCESS;
#endif
}


/******************************************************************
* FUNCTION
*    mdi_audio_play_file_with_vol_path
* DESCRIPTION
*    This function is to play audio file along with volume and output path.
* PARAMETERS
*    void* file_name, U8 play_style, mdi_handle* handle_p, mdi_callback handler
*    U8 volume, U8 path
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_play_file_with_vol_path(void* file_name, 
                                                U8 play_style, 
                                                mdi_handle* handle_p, 
                                                mdi_callback handler,
                                                U8 volume,
                                                U8 path,
                                                U8 StartOffset)
{
#ifdef MMI_ON_HARDWARE_P
   aud_play_file_struct file_param;
#endif
   mdi_result  result;

   //PRINT_INFORMATION(("mdi_audio_play_file, state=%d\n",mdi_audio_state));

   if( !mdi_audio_check_states_and_handlers( &result ) )
      return result;
         
   mdi_stop_background_timer();
   ++identifier;



    /* All sound shall go through bluetooth earphone */
#ifdef __MMI_BT_AUDIO_VIA_SCO__
 //   if( !mmi_audply_is_output_to_bt() )
    {
    mdi_audio_set_volume_to_bt(volume);
    mmi_profiles_bt_connect_audio_via_sco();
      }
#endif



#ifdef MMI_ON_HARDWARE_P   
   file_param.file_name_p = (UI_string_type)file_name;
   file_param.play_style = play_style;
   file_param.volume=volume;
   file_param.output_path=path;
   file_param.identifier= (U16)identifier;
// set both start_offset and end_offset to 0 for backward compatible
  // file_param.start_offset = file_param.end_offset = 0;

	if(StartOffset)
	{
		file_param.start_offset = aud_getFilePlayPercent();
	}
	else
	{
		file_param.start_offset = 0;
	}

   if((result=media_aud_play_file(MOD_MMI, &file_param))==MED_RES_OK)
   {
      currentHandler=handler;
      mdi_audio_state=MDI_AUDIO_PLAY;
      mdi_audio_mode=MDI_AUDIO_PLAY_FILE;
      SetProtocolEventHandler(mdi_audio_play_finish_ind1, MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND);
      return MDI_AUDIO_SUCCESS;
   }
   else
   {
      currentHandler=NULL;
      mdi_audio_state=MDI_AUDIO_IDLE;
      mdi_start_background_timer();
      return mdi_get_event((U8)result);
   }
#else
   currentHandler=handler;
   mdi_audio_state=MDI_AUDIO_PLAY;
   mdi_audio_mode=MDI_AUDIO_PLAY_FILE;
   return MDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_play_file_portion_with_vol_path
* DESCRIPTION
*    This function is to play audio file portion with volume and output path.
* PARAMETERS
*    void* file_name, U32 start_offset, U32 end_offset, U8 play_style, mdi_handle* handle_p, 
*    mdi_callback handler, U8 volume, U8 path
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_play_file_portion_with_vol_path( void* file_name, 
	U32 start_offset, U32 end_offset,
	U8 play_style, 
	mdi_handle* handle_p, 
	mdi_callback handler,
	U8 volume,
	U8 path )
{
#ifdef MMI_ON_HARDWARE_P
   aud_play_file_struct file_param;
#endif
   mdi_result  result;

   //PRINT_INFORMATION(("mdi_audio_play_file_portion, state=%d,start=%d,end=%d\n",mdi_audio_state,start_offset,end_offset));

   if( !mdi_audio_check_states_and_handlers( &result ) )
      return result;
         
   mdi_stop_background_timer();
   ++identifier;
#ifdef __MMI_BT_AUDIO_VIA_SCO__
    mmi_profiles_bt_connect_audio_via_sco();
#endif


#ifdef MMI_ON_HARDWARE_P   
   file_param.file_name_p = (UI_string_type)file_name;
   file_param.play_style = play_style;
   file_param.volume=volume;
   file_param.output_path=path;
   file_param.identifier= (U16)identifier;
   file_param.start_offset = start_offset;
   file_param.end_offset = end_offset;
   if((result=media_aud_play_file(MOD_MMI, &file_param))==MED_RES_OK)
   {
      currentHandler=handler;
      mdi_audio_state=MDI_AUDIO_PLAY;
      mdi_audio_mode=MDI_AUDIO_PLAY_FILE;
      SetProtocolEventHandler(mdi_audio_play_finish_ind1, MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND);
      return MDI_AUDIO_SUCCESS;
   }
   else
   {
      currentHandler=NULL;
      mdi_audio_state=MDI_AUDIO_IDLE;
      mdi_start_background_timer();
      return mdi_get_event((U8)result);
   }
#else
   currentHandler=handler;
   mdi_audio_state=MDI_AUDIO_PLAY;
   mdi_audio_mode=MDI_AUDIO_PLAY_FILE;
   return MDI_AUDIO_SUCCESS;
#endif
}


/******************************************************************
* FUNCTION
*    mdi_audio_get_file_time_length
* DESCRIPTION
*    This function is to get the data length of a audio file.
* PARAMETERS
*    void* file_name, U32* length
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_get_file_time_duration( void* file_name,U32 PlayProgress,U32* duration )
{
#ifdef MMI_ON_HARDWARE_P
	mdi_result result = (mdi_result)media_aud_get_file_data_length( MOD_MMI, file_name,PlayProgress, duration );

	return result;
#else
	*duration = 10000<<14;
	return MDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_get_file_data_progress
* DESCRIPTION
*    This function is to get the progress of file playing
* PARAMETERS
*    U32 progress 
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_get_file_data_progress( U32* progress )
{
#ifdef MMI_ON_HARDWARE_P
	mdi_result result;

	if( (mdi_audio_state == MDI_AUDIO_PLAY ||mdi_audio_state == MDI_AUDIO_PLAY_PAUSED) &&
		mdi_audio_mode == MDI_AUDIO_PLAY_FILE )
		result = (mdi_result)media_aud_get_file_data_progress( MOD_MMI, progress );
	else
	{
		result = MDI_AUDIO_FAIL;
		*progress = 0;
	}
	//PRINT_INFORMATION(("mdi_audio_get_file_data_progress, result=%d, progress=%d\n", result, *progress));
	
	return result;
#else
	*progress = 0;
	return MDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_set_file_data_progress
* DESCRIPTION
*    This function is to seek file playing offset when it's playing a file.
* PARAMETERS
*    U32 progress 
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
U32 mdi_audio_set_file_data_progress( U32 progress )
{
#ifdef MMI_ON_HARDWARE_P
	U32  duration = 0;
		media_aud_set_file_data_progress( MOD_MMI, progress,&duration );
	
	return duration;
#else
	return MDI_AUDIO_SUCCESS;
#endif
}

#if defined( DIGIT_TONE_SUPPORT ) || defined(SIMPLE_TTS)
#if defined(SIMPLE_TTS)
/******************************************************************
* FUNCTION
*    BinarySearch
* DESCRIPTION
*    This function is to perform a binary search in a index sorted table
* PARAMETERS
*    const unsigned char key
*    const unsigned char* lookup_table
*    short start
*    short end
* RETURNS
*    short
* GLOBALS AFFECTED
*  
******************************************************************/
static short BinarySearch( const unsigned char key, const unsigned char* lookup_table, short start, short end )
{
	while( start <= end )
	{
		short mid = (start + end)/2;
		if( key > lookup_table[mid] )
			start = mid + 1;
		else if( key < lookup_table[mid] )
			end = mid - 1;
		else // key == target
			return mid;
	}

	return -1;
}

/******************************************************************
* FUNCTION
*    LookupTTS
* DESCRIPTION
*    This function is to lookup the sound index from TTS database
* PARAMETERS
*    unsigned short ucs2
* RETURNS
*    short
* GLOBALS AFFECTED
*  
******************************************************************/
static short LookupTTS( unsigned short ucs2 )
{
	unsigned char key_MSB = (unsigned char)(ucs2 >> 8);
	unsigned char key_LSB;
	short start = TTS_UCS2_MSB_index[key_MSB].start;
	short end;
	short index;

	if( start < 0 ) // key MSB not found in index table
		return -1;

	key_LSB = (unsigned char)(ucs2 & 0xff);
	end = TTS_UCS2_MSB_index[key_MSB].end;
	if( (index = BinarySearch( key_LSB, TTS_UCS2_LSB, start, end )) < 0 ) // key LSB not found
		return -1;

	return TTS_index_table[index];
}
#endif

/******************************************************************
* FUNCTION
*    mdi_audio_get_word_resource
* DESCRIPTION
*    This function is to get the associated audio resource with the given word, which is a UCS2 char
* PARAMETERS
*    U16 wordVal
* RETURNS
*    audio_resource_struct*
* GLOBALS AFFECTED
*  
******************************************************************/
audio_resource_struct* mdi_audio_get_word_resource( U16 wordVal )
{
#ifdef MMI_ON_HARDWARE_P
#if defined( DIGIT_TONE_SUPPORT )
	S32 i;
	if( wordVal >= (U16)_T_MMI'0' && wordVal <= (U16)_T_MMI'9' )
		i = (S32)(wordVal - _T_MMI'0');
	else if( wordVal == _T_MMI'*' )
		i = 10;
	else if( wordVal == (U16)_T_MMI'#' )
		i = 11;
	else if( wordVal == (U16)_T_MMI'+' )
		i = -1;
	else
		i = -1;
	if( i >= 0 )
	{
		U8 keypad_tone_type = GetKeypadToneType();
		if( keypad_tone_type < KEYPAD_TONE_HUMAN_VOICE_1 || keypad_tone_type > KEYPAD_TONE_HUMAN_VOICE_6 )
			keypad_tone_type = KEYPAD_TONE_HUMAN_VOICE_1;
		return &resource_human_voice_tones[((keypad_tone_type - KEYPAD_TONE_HUMAN_VOICE_1)*13 + i)];
	}
	else
#endif
	{
#if defined( SIMPLE_TTS )
		S16 tts_index = LookupTTS(wordVal);
		if( tts_index < 0 )
			return NULL;
		return &resource_tts_sounds[tts_index];
#else
		return NULL;
#endif
	}
#else
	return NULL;
#endif
}

#if defined(__PROJECT_GALLITE_C01__)
/******************************************************************
* FUNCTION
*    mdi_audio_get_word_resource_id
* DESCRIPTION
*    This function is to get the associated audio resource with the given word, which is a UCS2 char
* PARAMETERS
*    U16 wordVal
* RETURNS
*    S16
* GLOBALS AFFECTED
*  
******************************************************************/
S16 mdi_audio_get_word_resource_id( U16 wordVal )
{
	S16 i;
    
	if( wordVal >= (U16)_T_MMI'0' && wordVal <= (U16)_T_MMI'9' )
		i = (S32)(wordVal - _T_MMI'0');
	else if( wordVal == _T_MMI'*' )
		i = 10;
	else if( wordVal == (U16)_T_MMI'#' )
		i = 11;
	else if( wordVal == (U16)_T_MMI'+' )
		i = -1;
	else
		i = -1;

	if(i >= 0)
	{
		i += MIN_HUMAN_VOICE_ID;
	}
    
	return i;
}
#endif
/******************************************************************
* FUNCTION
*    mdi_audio_handle_play_text_internal_callback
* DESCRIPTION
*    This function handle the play text internal callback
* PARAMETERS
*    mdi_result result
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
#if defined(__PROJECT_GALLITE_C01__)
void mdi_audio_handle_play_text_internal_callback(void)
{
	mdi_callback prev_handler;
	
	mdi_audio_play_text_context.state = MDI_AUDIO_PLAY_TEXT_IDLE_STATE;
	StopTimer( MDI_AUDIO_PLAYER_TEXT_TIMER );
	prev_handler = mdi_audio_play_text_context.handler;
	if( prev_handler != NULL )
	{
		mdi_audio_play_text_context.handler = NULL;
		prev_handler(MDI_AUDIO_END_OF_FILE );
	}
}
#else
void mdi_audio_handle_play_text_internal_callback( mdi_result result )
{
	mdi_callback prev_handler;
	
	mdi_audio_play_text_context.state = MDI_AUDIO_PLAY_TEXT_IDLE_STATE;
	StopTimer( MDI_AUDIO_PLAYER_TEXT_TIMER );
	prev_handler = mdi_audio_play_text_context.handler;
	if( prev_handler != NULL )
	{
		mdi_audio_play_text_context.handler = NULL;
		prev_handler( result );
	}
}
#endif
/******************************************************************
* FUNCTION
*    mdi_audio_play_current_word
* DESCRIPTION
*    This function play the word indicated by cursor
* PARAMETERS
*    void
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
extern S32 volume_level_UI;
mdi_result mdi_audio_play_current_word( void )
{
	mdi_result result;
#if defined(__PROJECT_GALLITE_C01__)
	S16 soundId;
#else
	audio_resource_struct* audio_resource_p;
#endif
	if( mdi_audio_play_text_context.len <= 0 )
		result = MDI_AUDIO_FAIL;
	else
	{
#if defined(__PROJECT_GALLITE_C01__)
		soundId = mdi_audio_get_word_resource_id( 
			mdi_audio_play_text_context.words[++mdi_audio_play_text_context.cursor] );
        
        
		if( soundId == -1 )
#else
		do
		{
			audio_resource_p = mdi_audio_get_word_resource( 
				mdi_audio_play_text_context.words[++mdi_audio_play_text_context.cursor] );
		} while( audio_resource_p == NULL && mdi_audio_play_text_context.cursor < mdi_audio_play_text_context.len - 1 );
		if( audio_resource_p == NULL )
#endif
		{
			// pretend it's playing
			currentHandler = mdi_audio_play_text_event_handler; // we hook up current handler to keep ourself being notified when other requests come
			mdi_audio_state = MDI_AUDIO_PLAY;
			mdi_audio_mode = MDI_AUDIO_PLAY_STRING;
			StartTimer( MDI_AUDIO_PLAYER_TEXT_TIMER, 40, mdi_audio_play_timeout_handler );
			result = MDI_AUDIO_SUCCESS;
		}
		else
		{
#if defined(__PROJECT_GALLITE_C01__)
			result = mdi_audio_play_id_with_vol_path(soundId,
				DEVICE_AUDIO_PLAY_ONCE,
				NULL,
				mdi_audio_play_text_event_handler,
				volume_level_UI,
				mdi_audio_play_text_context.path );
#else
			result = mdi_audio_play_string_with_vol_path((void*)audio_resource_p->data,
				audio_resource_p->len, 
				(U8)audio_resource_p->format,
				DEVICE_AUDIO_PLAY_ONCE,
				NULL,
				mdi_audio_play_text_event_handler,
				mmi_audply_get_volume(),
				mdi_audio_play_text_context.path );
#endif
			mdi_audio_play_text_context.identifier = identifier;
		}
	}
	if( result == MDI_AUDIO_SUCCESS )
		mdi_audio_play_text_context.state = MDI_AUDIO_PLAY_TEXT_PLAY_STATE;
	else
#if !defined(__PROJECT_GALLITE_C01__)
		mdi_audio_handle_play_text_internal_callback( result );
#else
	{      
		if(result == MDI_AUDIO_END_OF_FILE)
		{         
			StartTimer( MDI_AUDIO_PLAYER_TEXT_TIMER, mdi_audio_play_text_context.msecs, mdi_audio_handle_play_text_internal_callback );
		}
		else
		{
			mdi_audio_play_text_context.state = MDI_AUDIO_PLAY_TEXT_IDLE_STATE;
			StopTimer( MDI_AUDIO_PLAYER_TEXT_TIMER );
		}
       }
#endif
	return result;
}

/******************************************************************
* FUNCTION
*    mdi_audio_play_timeout_handler
* DESCRIPTION
*    time expired event handler for a short period of time after finish playing a number or word
* PARAMETERS
*    void
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_play_timeout_handler( void )
{
	currentHandler = NULL; // clear the hook up
	if( mdi_audio_play_text_context.cursor < mdi_audio_play_text_context.len - 1 )
		mdi_audio_play_current_word();
	else
	{
		mdi_audio_state = MDI_AUDIO_IDLE;
		mdi_audio_play_text_event_handler( MDI_AUDIO_END_OF_FILE );
		mdi_start_background_timer();
	}
}

/******************************************************************
* FUNCTION
*    mdi_audio_play_text_event_handler
* DESCRIPTION
*    This function is to handle events during playing text
* PARAMETERS
*    mdi_result result
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_play_text_event_handler( mdi_result result )
{
 	if( mdi_audio_play_text_context.state == MDI_AUDIO_PLAY_TEXT_PLAY_STATE )
 	{
		if( result == MDI_AUDIO_END_OF_FILE && mdi_audio_play_text_context.cursor < mdi_audio_play_text_context.len - 1 )
		{
			// need to play more words
			mdi_audio_play_text_context.state = MDI_AUDIO_PLAY_TEXT_WAIT_STATE;
			currentHandler = mdi_audio_play_text_event_handler; // we hook up current handler to keep ourself being notified when other requests come
			StartTimer( MDI_AUDIO_PLAYER_TEXT_TIMER, mdi_audio_play_text_context.msecs, mdi_audio_play_timeout_handler );
		}
		else
#if !defined(__PROJECT_GALLITE_C01__)
			mdi_audio_handle_play_text_internal_callback( result );
#else
		{      
			if(result == MDI_AUDIO_END_OF_FILE)
			{         
			    StartTimer( MDI_AUDIO_PLAYER_TEXT_TIMER, mdi_audio_play_text_context.msecs, mdi_audio_handle_play_text_internal_callback );
			}
			else
			{
			    mdi_audio_play_text_context.state = MDI_AUDIO_PLAY_TEXT_IDLE_STATE;
			    StopTimer( MDI_AUDIO_PLAYER_TEXT_TIMER );
			}
		}
#endif
 	}
	else if( mdi_audio_play_text_context.state ==  MDI_AUDIO_PLAY_TEXT_WAIT_STATE )
#if !defined(__PROJECT_GALLITE_C01__)
		mdi_audio_handle_play_text_internal_callback( result );
#else
	{      
		if(result == MDI_AUDIO_END_OF_FILE)
		{         
			StartTimer( MDI_AUDIO_PLAYER_TEXT_TIMER, mdi_audio_play_text_context.msecs, mdi_audio_handle_play_text_internal_callback );
		}
		else
		{
			mdi_audio_play_text_context.state = MDI_AUDIO_PLAY_TEXT_IDLE_STATE;
			StopTimer( MDI_AUDIO_PLAYER_TEXT_TIMER );
		}
	}
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_play_text_with_vol_path
* DESCRIPTION
*    This function is to play text (number or words)
* PARAMETERS
*    U8* words
*    S32 len
*    S32 msecs
*    mdi_handle* handle_p
*    mdi_callback handler
*    U8 volume
*    U8 path
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_play_text_with_vol_path( U8* words, S32 len, S32 msecs, mdi_handle* handle_p, mdi_callback handler, U8 volume, U8 path )
{
	mdi_result result;

	if( !mdi_audio_check_states_and_handlers( &result ) )
		return result;
	mdi_audio_play_text_context.cursor = -1;
	mdi_audio_play_text_context.len = len;
	mdi_audio_play_text_context.msecs = (msecs > 0) ? msecs: 10 ;
	// to avoid 2 bytes alignment issue, copy text to the internal buffer, which is 2 bytes alignment
	if( len > MDI_MAX_PLAY_TEXT_LEN )
		len = MDI_MAX_PLAY_TEXT_LEN;
	memcpy( mdi_audio_play_text_context.words, words, len << 1 );
	mdi_audio_play_text_context.volume = volume;
	mdi_audio_play_text_context.path = path;
	result = mdi_audio_play_current_word();
	if( result == MDI_AUDIO_SUCCESS )
		mdi_audio_play_text_context.handler = handler;
	return result;
}
#endif

/******************************************************************
* FUNCTION
*    mdi_audio_play_finish_ind
* DESCRIPTION
*    This function is to handle the play finish indication for id and string.
* PARAMETERS
*    void* inMsg
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
extern void mmi_replay_last_file(void);
extern INT32 mdi_audio_file_playStyle;
extern kal_uint8 aud_close_play_file(void);
extern void mmi_audply_clear_play_seconds();

void mdi_audio_play_finish_ind(void* inMsg)
{
#if defined(__PROJECT_GALLITE_C01__)
	UINT32 digttime=0;
    
	digttime = csw_TMGetTick();

        mmi_trace(1, "DIGITTIME func:%s, time = %d", __FUNCTION__, digttime);			
#endif
#ifdef MMI_ON_HARDWARE_P
   l4aud_audio_play_finish_ind_struct* msg_p=(l4aud_audio_play_finish_ind_struct*)inMsg;

   PRINT_INFORMATION(("mdi_audio_play_finish_ind, state=%d\n",mdi_audio_state));
   if((mdi_audio_state==MDI_AUDIO_PLAY||mdi_audio_state==MDI_AUDIO_PLAY_PAUSED)
      && msg_p->identifier==identifier)
   {
      mdi_audio_state=MDI_AUDIO_IDLE;
      if (currentHandler)
      {
         previousHandler=currentHandler;
         currentHandler=NULL;
         previousHandler(mdi_get_event(msg_p->result)); // mdi_audio_play_text handler could set currentHandler to itself here
      }
      mdi_start_background_timer();
#ifdef __FF_AUDIO_SET__
      MusicEndAudioMode();
#endif
   }
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_play_finish_ind1
* DESCRIPTION
*    This function is to handle the play finish indication for file.
* PARAMETERS
*    void* inMsg
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/

void mdi_audio_play_finish_ind1(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
	l4aud_media_play_finish_ind_struct* msg_p=(l4aud_media_play_finish_ind_struct*)inMsg;
	/*chenhe 081017 to fix bug 10101 */
	mdi_audio_set_file_data_progress(0);
	mmi_audply_clear_play_seconds();
	if(DEVICE_AUDIO_PLAY_INFINITE ==mdi_audio_file_playStyle && msg_p->result != MDI_AUDIO_INVALID_FORMAT)//jiashuo complement condition for bug#13142  
	{
		mmi_replay_last_file();
		return;
	}
	aud_tone_stop();
	//PRINT_INFORMATION(("mdi_audio_play_finish_ind1, state=%d\n",mdi_audio_state));
	if((mdi_audio_state==MDI_AUDIO_PLAY||mdi_audio_state==MDI_AUDIO_PLAY_PAUSED)
	&& 1)//msg_p->identifier==identifier)
	{
		mdi_audio_state=MDI_AUDIO_IDLE;
		if (currentHandler)
		{
			previousHandler=currentHandler;
			currentHandler=NULL;
			previousHandler(mdi_get_event(msg_p->result));
		}
		mdi_start_background_timer();
		aud_close_play_file();
		//MusicEndAudioMode(); //delete by chenhe,20100416,for bug 
	}
	
	TurnOffBacklight();
	mmi_trace(1,"chenhe,mdi_audio_play_finish_ind1,mdi_audio_file_playStyle is %d",mdi_audio_file_playStyle);


#endif
}


/******************************************************************
* FUNCTION
*    mdi_audio_stop_id
* DESCRIPTION
*    This function is to stop audio id.
* PARAMETERS
*    U16 audio_id
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_stop_id(U16 audio_id)
{
   //PRINT_INFORMATION(("mdi_audio_stop_id, id=%d, state=%d, mode=%d\n", audio_id, mdi_audio_state, mdi_audio_mode));
   TRACE_EF(1,"chenhe,mdi_audio_stop_id");
   if (blocked)
      return MDI_AUDIO_BLOCKED;

   if( audio_id>MAX_TONE_ID && !((mdi_audio_state == MDI_AUDIO_PLAY || mdi_audio_state == MDI_AUDIO_PLAY_PAUSED)
   	 && mdi_audio_mode == MDI_AUDIO_PLAY_ID) )
      return MDI_AUDIO_SUCCESS;
   
#ifdef MMI_ON_HARDWARE_P
   media_aud_stop_id(MOD_MMI, (U8)audio_id);
#endif
#ifdef __FF_AUDIO_SET__
    // Should NOT change the audio mode when stopping tones
    if (audio_id > MAX_TONE_ID)
    {
        // Reset the audio mode, just as what mdi_audio_play_finish_ind() does.
        // There is a race condition that FINISH_IND event handler is set to
        // NULL while the event has been in MBX queue (MED task has cleaned up
        // the audio context state). This case is likely to occur when pressing
        // dial keys and the send key in a rush with human voice tone enabled.
        MusicEndAudioMode();
    }
#endif

   if (audio_id>MAX_TONE_ID)
   {
      if (currentHandler)
      {
         previousHandler=currentHandler;
         currentHandler=NULL;
         previousHandler(MDI_AUDIO_TERMINATED);
         if( mdi_audio_state == MDI_AUDIO_PLAY && mdi_audio_mode == MDI_AUDIO_PLAY_FM_RADIO )
            mdi_fmr_power_off();
      }
      mdi_audio_state=MDI_AUDIO_IDLE;
#ifdef MMI_ON_HARDWARE_P
      SetProtocolEventHandler(NULL, MSG_ID_L4AUD_AUDIO_PLAY_FINISH_IND);
#endif
      mdi_start_background_timer();
   }

   return MDI_AUDIO_SUCCESS;
}

/******************************************************************
* FUNCTION
*    mdi_audio_stop_string
* DESCRIPTION
*    This function is to stop audio string.
* PARAMETERS
*    None
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_stop_string(void)
{
  PRINT_INFORMATION(("mdi_audio_stop_string, state=%d, mode=%d\n",mdi_audio_state, mdi_audio_mode));
   if (blocked)
      return MDI_AUDIO_BLOCKED;

   if( !((mdi_audio_state == MDI_AUDIO_PLAY || mdi_audio_state == MDI_AUDIO_PLAY_PAUSED) 
      && mdi_audio_mode == MDI_AUDIO_PLAY_STRING) )
   {
#if defined( DIGIT_TONE_SUPPORT ) || defined(SIMPLE_TTS)
      if( mdi_audio_play_text_context.state == MDI_AUDIO_PLAY_TEXT_IDLE_STATE )
#endif
         return MDI_AUDIO_SUCCESS;
   }

#ifdef MMI_ON_HARDWARE_P
   SetProtocolEventHandler(NULL, MSG_ID_L4AUD_AUDIO_PLAY_FINISH_IND);
   media_aud_stop_string(MOD_MMI, SRC_LMMI);
#endif
#ifdef __FF_AUDIO_SET__
    // Reset the audio mode, just as what mdi_audio_play_finish_ind() does.
    // There is a race condition that FINISH_IND event handler is set to
    // NULL while the event has been in MBX queue (MED task has cleaned up
    // the audio context state). This case is likely to occur when pressing
    // dial keys and the send key in a rush with human voice tone enabled.
    MusicEndAudioMode();
#endif

   if (currentHandler)
   {
      previousHandler=currentHandler;
      currentHandler=NULL;
      previousHandler(MDI_AUDIO_TERMINATED);
      if( mdi_audio_state == MDI_AUDIO_PLAY && mdi_audio_mode == MDI_AUDIO_PLAY_FM_RADIO )
            mdi_fmr_power_off();
   }
   mdi_audio_state=MDI_AUDIO_IDLE;
   mdi_start_background_timer();

   return MDI_AUDIO_SUCCESS;
}

 
/******************************************************************
* FUNCTION
*    mdi_audio_stop_file
* DESCRIPTION
*    This function is to stop audio file.
* PARAMETERS
*    None
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/

mdi_result mdi_audio_stop_file(void)
{
   //PRINT_INFORMATION(("mdi_audio_stop_file, state=%d, mode=%d\n",mdi_audio_state, mdi_audio_mode));
   if (blocked)
      return MDI_AUDIO_BLOCKED;
mmi_trace(1,"mdi_audio_stop_file");

   if( !((mdi_audio_state == MDI_AUDIO_PLAY || mdi_audio_state == MDI_AUDIO_PLAY_PAUSED) 
      && mdi_audio_mode == MDI_AUDIO_PLAY_FILE) )
      return MDI_AUDIO_SUCCESS;

    /* stop  bluetooth earphone */
/*#ifdef __MMI_BT_AUDIO_VIA_SCO__
      mdi_audio_bt_close_stream(BT_HFP);

#endif
*/

#ifdef MMI_ON_HARDWARE_P
#if 0
   mmi_audply_clear_play_seconds();
   mdi_audio_set_file_data_progress(0);
   #endif
   SetProtocolEventHandler(NULL, MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND);
   media_aud_stop_file(MOD_MMI, SRC_LMMI);
#endif

   if (currentHandler)
   {
      previousHandler=currentHandler;
      currentHandler=NULL;
      previousHandler(MDI_AUDIO_TERMINATED);
      if( mdi_audio_state == MDI_AUDIO_PLAY && mdi_audio_mode == MDI_AUDIO_PLAY_FM_RADIO )
            mdi_fmr_power_off();
   }
   mdi_audio_state=MDI_AUDIO_IDLE;
   mdi_start_background_timer();
	mdi_audio_file_playStyle = -1;

	mmi_trace(1,"mdi_audio_stop_file,end");

   return MDI_AUDIO_SUCCESS;
}


/******************************************************************
* FUNCTION
*    mdi_audio_stop_all
* DESCRIPTION
*    This function is to stop all audio id/string/file.
* PARAMETERS
*    None
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
extern pBOOL GetRingingFlag(void);

mdi_result mdi_audio_stop_all(void)
{
   //PRINT_INFORMATION(("mdi_audio_stop_all, state=%d, mode=%d\n",mdi_audio_state,mdi_audio_mode));
mmi_trace(1,"mdi_audio_stop_all is called,blocked=%d,mdi_audio_state=%d,mdi_audio_mode is %d,mdi_audio_id is %d",blocked,mdi_audio_state,mdi_audio_mode,mdi_audio_id);
	if (blocked)
	{
		return MDI_AUDIO_BLOCKED;
	}
#ifdef MMI_ON_HARDWARE_P    
   if(GetRingingFlag())
    {
        Trace("incoming ring is playing");
       /// hal_DbgAssert("See who call mdi audio stop");
        return MDI_AUDIO_BLOCKED;
    }
#endif   
	mdi_audio_file_playStyle = -1;
   // to avoid the case that play finish indication of previous request is treat as the one of current request
   if (mdi_audio_state!=MDI_AUDIO_IDLE)
   {
      switch(mdi_audio_mode)
      {
         case MDI_AUDIO_PLAY_ID:
            mdi_audio_stop_id(mdi_audio_id);
            break;
         case MDI_AUDIO_PLAY_STRING:
            mdi_audio_stop_string();
            break;
         case MDI_AUDIO_PLAY_FILE:
            mdi_audio_stop_file();
            break;
         case MDI_AUDIO_RECORD_FILE:
            mdi_audio_stop_record();
            break;
         case MDI_AUDIO_PLAY_FM_RADIO:
         case MDI_AUDIO_RECORD_FM_RADIO:
            mdi_audio_stop_fmr();
            break;
#ifdef VR_ENABLE
         case MDI_AUDIO_RECORD_VR_VOICE:
            if( mdi_audio_state == MDI_AUDIO_RECORD )
               mdi_audio_stop_vr_voice_record();
            break;
#endif
#ifdef VRSI_ENABLE
         case MDI_AUDIO_VRSI_PLAY_TTS: 
         case MDI_AUDIO_VRSI_PLAY_TAG: 
         case MDI_AUDIO_VRSI_RECORD:   
            if( mdi_audio_state == MDI_AUDIO_VRSI_PROCESS ) 
               mdi_audio_vrsi_stop(identifier);
            break;   
#endif 
        #ifdef __GENERAL_TTS__
            case MDI_AUDIO_PLAY_TTS:
                mdi_audio_tts_stop();
                break;
        #endif /* __GENERAL_TTS__ */
         default:
            break;

      }

   }
   return MDI_AUDIO_SUCCESS;

}

/******************************************************************
* FUNCTION
*    mdi_audio_pause
* DESCRIPTION
*    This function is to pause audio play/record.
* PARAMETERS
*    mdi_handle audio_handle, mdi_callback handler
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_pause(mdi_handle audio_handle, mdi_callback handler)
{
#ifdef MMI_ON_HARDWARE_P
   mdi_result  result;
#endif

   //PRINT_INFORMATION(("mdi_audio_pause, state=%d\n",mdi_audio_state));
   if (blocked)
      return MDI_AUDIO_BLOCKED;

#ifdef MMI_ON_HARDWARE_P
   if((result=media_aud_pause(MOD_MMI, SRC_LMMI))==MED_RES_OK)
   {
      if(mdi_audio_state==MDI_AUDIO_PLAY)
         mdi_audio_state=MDI_AUDIO_PLAY_PAUSED;
      else
         mdi_audio_state=MDI_AUDIO_RECORD_PAUSED;
      currentHandler=handler;
      return MDI_AUDIO_SUCCESS;
   }
   else if(result == MED_RES_UNSUPPORTED_OPERATION)
   	  return MDI_AUDIO_UNSUPPORTED_OPERATION;
   else
   {
      currentHandler=NULL;
      mdi_audio_state=MDI_AUDIO_IDLE;
      mdi_start_background_timer();
      return mdi_get_event((U8)result);
   }
#else
   if(mdi_audio_state==MDI_AUDIO_PLAY)
      mdi_audio_state=MDI_AUDIO_PLAY_PAUSED;
   else
      mdi_audio_state=MDI_AUDIO_RECORD_PAUSED;
   currentHandler=handler;
   return MDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_store_file
* DESCRIPTION
*    This function is to store audio play state for restore it later
* PARAMETERS
*    mdi_handle audio_handle
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_store_file( mdi_handle audio_handle )
{
#ifdef MMI_ON_HARDWARE_P
   mdi_result  result;

   //PRINT_INFORMATION(("mdi_audio_store_file, state=%d\n",mdi_audio_state));
   if (blocked)
      return MDI_AUDIO_BLOCKED;

   if((result=media_aud_store(MOD_MMI, SRC_LMMI))==MED_RES_OK)
   {
      return MDI_AUDIO_SUCCESS;
   }
   else
   {
      return mdi_get_event((U8)result);
   }
#else
   return MDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_restore_file_with_vol_path
* DESCRIPTION
*    This function is to restore audio file along with volume and output path.
* PARAMETERS
*    void* file_name, U8 play_style, mdi_handle* handle_p, mdi_callback handler
*    U8 volume, U8 path
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/

mdi_result mdi_audio_restore_file_with_vol_path(void* file_name, 
                                                U8 play_style, 
                                                mdi_handle* handle_p, 
                                                mdi_callback handler,
                                                U8 volume,
                                                U8 path)
{
#ifdef MMI_ON_HARDWARE_P
   aud_play_file_struct file_param;
#endif
   mdi_result  result;
	U32 duration ;
   //PRINT_INFORMATION(("mdi_audio_restore_file, state=%d\n",mdi_audio_state));

   if( !mdi_audio_check_states_and_handlers( &result ) )
      return result;
         
   mdi_stop_background_timer();
   ++identifier;

    /* All sound shall go through bluetooth earphone */
#ifdef __MMI_BT_AUDIO_VIA_SCO__
    mmi_profiles_bt_connect_audio_via_sco();
#endif

#ifdef MMI_ON_HARDWARE_P   
   file_param.file_name_p = (UI_string_type)file_name;
   file_param.play_style = play_style;
   file_param.volume=volume;
   file_param.output_path=path;
   file_param.identifier= (U16)identifier;
// set both start_offset and end_offset to 0 for backward compatible
#if 1
	file_param.start_offset=aud_getFilePlayPercent();
	mdi_audio_get_file_time_duration(file_param.file_name_p,(U32)file_param.start_offset,&duration);
	duration = (U64)duration*16384/1000;
	mmi_audply_reset_play_seconds((U32)duration);
	mdi_audio_set_file_data_progress(AUD_FULL_PROGRESS_CNT+100);
	audioplayer_restore_flag = 0;
#else
   file_param.start_offset = file_param.end_offset = 0;
#endif
   if((result=media_aud_restore(MOD_MMI, &file_param))==MED_RES_OK)
   {
      currentHandler=handler;
      mdi_audio_state=MDI_AUDIO_PLAY;
      mdi_audio_mode=MDI_AUDIO_PLAY_FILE;
      SetProtocolEventHandler(mdi_audio_play_finish_ind1, MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND);
      return MDI_AUDIO_SUCCESS;
   }
   else
   {
      currentHandler=NULL;
      mdi_audio_state=MDI_AUDIO_IDLE;
      mdi_start_background_timer();
      return mdi_get_event((U8)result);
   }
#else
   currentHandler=handler;
   mdi_audio_state=MDI_AUDIO_PLAY;
   mdi_audio_mode=MDI_AUDIO_PLAY_FILE;
   return MDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_restore_file_portion_with_vol_path
* DESCRIPTION
*    This function is to restore audio file portion with volume and output path.
* PARAMETERS
*    void* file_name, U32 start_offset, U32 end_offset, U8 play_style, mdi_handle* handle_p, 
*    mdi_callback handler, U8 volume, U8 path
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_restore_file_portion_with_vol_path( void* file_name, 
	U32 start_offset, U32 end_offset,
	U8 play_style, 
	mdi_handle* handle_p, 
	mdi_callback handler,
	U8 volume,
	U8 path )
{
#ifdef MMI_ON_HARDWARE_P
   aud_play_file_struct file_param;
#endif
   mdi_result  result;

   //PRINT_INFORMATION(("mdi_audio_play_file_portion, state=%d,start=%d,end=%d\n",mdi_audio_state,start_offset,end_offset));

   if( !mdi_audio_check_states_and_handlers( &result ) )
      return result;
         
   mdi_stop_background_timer();
   ++identifier;

#ifdef MMI_ON_HARDWARE_P   
   file_param.file_name_p = (UI_string_type)file_name;
   file_param.play_style = play_style;
   file_param.volume=volume;
   file_param.output_path=path;
   file_param.identifier= (U16)identifier;
// set both start_offset and end_offset to 0 for backward compatible
   file_param.start_offset = start_offset;
   file_param.end_offset = end_offset;
   if((result=media_aud_restore(MOD_MMI, &file_param))==MED_RES_OK)
   {
      currentHandler=handler;
      mdi_audio_state=MDI_AUDIO_PLAY;
      mdi_audio_mode=MDI_AUDIO_PLAY_FILE;
      SetProtocolEventHandler(mdi_audio_play_finish_ind1, MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND);
      return MDI_AUDIO_SUCCESS;
   }
   else
   {
      currentHandler=NULL;
      mdi_audio_state=MDI_AUDIO_IDLE;
      mdi_start_background_timer();
      return mdi_get_event((U8)result);
   }
#else
   currentHandler=handler;
   mdi_audio_state=MDI_AUDIO_PLAY;
   mdi_audio_mode=MDI_AUDIO_PLAY_FILE;
   return MDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_pause
* DESCRIPTION
*    This function is to resume audio play/record.
* PARAMETERS
*    mdi_handle audio_handle, mdi_callback handler
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_resume(mdi_handle audio_handle, mdi_callback handler)
{
#ifdef MMI_ON_HARDWARE_P
   mdi_result  result;
#endif

   //PRINT_INFORMATION(("mdi_audio_resume, state=%d\n",mdi_audio_state));
   if (blocked)
      return MDI_AUDIO_BLOCKED;

#ifdef MMI_ON_HARDWARE_P
   if((result=media_aud_resume(MOD_MMI, SRC_LMMI))==MED_RES_OK)
   {
      if(mdi_audio_state==MDI_AUDIO_PLAY_PAUSED)
         mdi_audio_state=MDI_AUDIO_PLAY;
      else
         mdi_audio_state=MDI_AUDIO_RECORD;
      currentHandler=handler;
      return MDI_AUDIO_SUCCESS;
   }
   else
   {
      currentHandler=NULL;
      mdi_audio_state=MDI_AUDIO_IDLE;
      mdi_start_background_timer();
      return mdi_get_event((U8)result);
   }
#else
   if(mdi_audio_state==MDI_AUDIO_PLAY_PAUSED)
      mdi_audio_state=MDI_AUDIO_PLAY;
   else
      mdi_audio_state=MDI_AUDIO_RECORD;
   currentHandler=handler;
   return MDI_AUDIO_SUCCESS;
#endif
}


/******************************************************************
* FUNCTION
*    mdi_audio_start_record
* DESCRIPTION
*    This function is to start audio record.
* PARAMETERS
*    void* file_name, U8 format, mdi_handle* handle_p, mdi_callback handler
* RETURNS
*    mdi_handle
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_start_record(void* file_name, U8 format, mdi_handle* handle_p, mdi_callback handler)
{
		
	return mdi_audio_start_record_with_quality( file_name, format, 7, handle_p,handler);
}


///*quality:: 0x03 - 0x0A */

mdi_result mdi_audio_start_record_with_quality(void* file_name, U8 format,  U8 quality, mdi_handle* handle_p, mdi_callback handler)
{
#ifdef MMI_ON_HARDWARE_P
   mdi_result result;
#endif
  PRINT_INFORMATION(("mdi_audio_start_record, blocked  = %d, state=%d\n",blocked, mdi_audio_state));

   if( blocked )
   	return MDI_AUDIO_BLOCKED;
   else if( mdi_audio_state == MDI_AUDIO_RECORD || mdi_audio_state == MDI_AUDIO_RECORD_PAUSED )
	return MDI_AUDIO_BUSY;
   else if( currentHandler && (mdi_audio_state == MDI_AUDIO_PLAY || mdi_audio_state == MDI_AUDIO_PLAY_PAUSED
#if defined( DIGIT_TONE_SUPPORT ) || defined(SIMPLE_TTS)
		|| mdi_audio_play_text_context.state == MDI_AUDIO_PLAY_TEXT_WAIT_STATE
#endif
   ) )
   {
      currentHandler( MDI_AUDIO_TERMINATED );
      if( mdi_audio_state == MDI_AUDIO_PLAY && mdi_audio_mode == MDI_AUDIO_PLAY_FM_RADIO )
         mdi_fmr_power_off();
   }

   mdi_stop_background_timer();

#ifdef MMI_ON_HARDWARE_P
   if( (result = media_aud_start_record(MOD_MMI, (kal_wchar *) file_name, format, quality,TRUE, 0)) == MED_RES_OK)
   {
   TRACE_EF(1,"chenhe,mdi_audio_start_record,success");
      currentHandler=handler;
      mdi_audio_state=MDI_AUDIO_RECORD;
      mdi_audio_mode=MDI_AUDIO_RECORD_FILE;
      SetProtocolEventHandler(mdi_audio_record_finish_ind, MSG_ID_L4AUD_MEDIA_RECORD_FINISH_IND);
      return MDI_AUDIO_SUCCESS;
   }
   else
   {
      currentHandler=NULL;
      mdi_audio_state=MDI_AUDIO_IDLE;
      mdi_start_background_timer();
      return mdi_get_event((U8)result);
   }
#else
   currentHandler=handler;
   mdi_audio_state=MDI_AUDIO_RECORD;
   mdi_audio_mode=MDI_AUDIO_RECORD_FILE;
   return MDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_record_finish_ind
* DESCRIPTION
*    This function is to handle audio record finish indication.
* PARAMETERS
*    void* inMsg
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_record_finish_ind(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   l4aud_media_record_finish_ind_struct* msg_p=(l4aud_media_record_finish_ind_struct*)inMsg;

   //PRINT_INFORMATION(("mdi_audio_record_finish_ind, state=%d\n",mdi_audio_state));
   TRACE_EF(1,"chenhe,mdi_audio_record_finish_ind,mdi_get_event(msg_p->result is %d",mdi_get_event(msg_p->result));
   if(mdi_audio_state==MDI_AUDIO_RECORD)
   {
      mdi_audio_state=MDI_AUDIO_IDLE;
      if (currentHandler)
      {
         previousHandler=currentHandler;
         currentHandler=NULL;
         previousHandler(mdi_get_event(msg_p->result));
      }
      mdi_start_background_timer();
   }   
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_stop_record
* DESCRIPTION
*    This function is to stop audio record.
* PARAMETERS
*    void* inMsg
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_stop_record(void)
{
   //PRINT_INFORMATION(("mdi_audio_stop_record, state=%d\n",mdi_audio_state));
   TRACE_EF(1,"chenhe,mdi_audio_stop_record");
   if (blocked)
      return MDI_AUDIO_BLOCKED;

#ifdef MMI_ON_HARDWARE_P
   SetProtocolEventHandler(NULL, MSG_ID_L4AUD_MEDIA_RECORD_FINISH_IND);
   media_aud_stop_record(MOD_MMI, SRC_LMMI);
#endif

   mdi_audio_state=MDI_AUDIO_IDLE;
   if (currentHandler)
   {
      previousHandler=currentHandler;
      currentHandler=NULL;
      previousHandler(MDI_AUDIO_TERMINATED);
   }

   mdi_start_background_timer();

   return MDI_AUDIO_SUCCESS;
}



/******************************************************************
* FUNCTION
*    mdi_audio_background_timeout_handler
* DESCRIPTION
*    This function is to handle background timeout.
* PARAMETERS
*    None
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_background_timeout_handler(void)
{
   S16 i=0;

   //PRINT_INFORMATION(("mdi_audio_bg_timeout, blocked=%d,bg_enabled=%d,state=%d,speech_on=%d\n",blocked,backgroud_enabled,mdi_audio_state,mdi_speech_on));
   
   if( blocked || !backgroud_enabled || (mdi_audio_state!=MDI_AUDIO_IDLE) || mdi_speech_on)
      return;

   for( i=0; i<MAX_BACKGROUND_HANDLER; i++ )
   {
#if defined(__MMI_MAINLCD_220X176__)
   	if (!fm_backgroud_enabled && i==MDI_BACKGROUND_APP_FMR)
		continue;
#endif	
      if (background_handler[i])
      {
         if( background_handler[i](MDI_AUDIO_RESUME) )
         break;
      }
   }
}


/******************************************************************
* FUNCTION
*    mdi_audio_resume_background_play
* DESCRIPTION
*    This function is to resume background play.
* PARAMETERS
*    None
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
extern BOOL checkVMOn(void);
void mdi_audio_resume_background_play(void)
{
   //PRINT_INFORMATION(("mdi_audio_resume_background_play, state=%d\n",mdi_audio_state));
	if(!isInCall()&&!checkVMOn())
	{
		backgroud_enabled=TRUE;
#if defined(__MMI_MAINLCD_220X176__)
		fm_backgroud_enabled=TRUE;
#endif		
		mdi_start_background_timer();
	}
}

#if defined(__MMI_MAINLCD_220X176__)
void mdi_audio_suspend_background_playFM(void)
{
   fm_backgroud_enabled=FALSE; //delete by panxu //chenhe enable for replay after interrupt
   mdi_audio_stop_fmr();
}
#endif
/******************************************************************
* FUNCTION
*    mdi_audio_suspend_background_play
* DESCRIPTION
*    This function is to suspend background play.
* PARAMETERS
*    None
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_suspend_background_play(void)
{
   //PRINT_INFORMATION(("mdi_audio_suspend_background_play, state=%d\n",mdi_audio_state));
   backgroud_enabled=FALSE; //delete by panxu //chenhe enable for replay after interrupt
   mdi_stop_background_timer();
   mdi_audio_stop_all();
}

/******************************************************************
* FUNCTION
*    mdi_audio_is_background_play_suspended
* DESCRIPTION
*    This function is to check if background play suspended.
* PARAMETERS
*    None
* RETURNS
*    BOOL
* GLOBALS AFFECTED
*  
******************************************************************/
BOOL mdi_audio_is_background_play_suspended(void)
{
	return !backgroud_enabled;
}

/******************************************************************
* FUNCTION
*    mdi_audio_is_idle
* DESCRIPTION
*    This function is to check if audio is in idle state.
* PARAMETERS
*    None
* RETURNS
*    BOOL
* GLOBALS AFFECTED
*  
******************************************************************/
BOOL mdi_audio_is_idle(void)
{
	return mdi_audio_state == MDI_AUDIO_IDLE;
}



/*****************************************************************************
 * FUNCTION
 *  mdi_audio_is_idle
 * DESCRIPTION
 *  This function is to check if audio is in idle state.
 * PARAMETERS
 *  void
 * RETURNS
 * BOOL
 *****************************************************************************/
MMI_BOOL mdi_audio_is_playing(U16 audio_mode)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_BOOL is_playing;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//    MDI_AUDIO_LOCK;
 //   MMI_TRACE(MMI_MEDIA_TRC_G4_MDI, MDI_AUDIO_TRC_IS_PLAYING, mdi_audio_state, mdi_audio_mode);
    mmi_trace(1,"  mdi_audio_is_playing !! mdi_audio_state = %d, mdi_audio_mode =%d ",mdi_audio_state,  mdi_audio_mode  );	
    is_playing = (mdi_audio_state == MDI_AUDIO_PLAY) && (mdi_audio_mode == audio_mode);
//    MDI_AUDIO_UNLOCK;
    return is_playing;
}

/******************************************************************
* FUNCTION
*    mdi_audio_is_speech_mode
* DESCRIPTION
*    This function is to check is in speech mode
* PARAMETERS
*    void
* RETURNS
*    BOOL
* GLOBALS AFFECTED
*  
******************************************************************/
BOOL mdi_audio_is_speech_mode( void )
{
	return mdi_speech_on;
}

/******************************************************************
* FUNCTION
*    mdi_audio_is_blocked
* DESCRIPTION
*    This function is to check if it is blocked.
* PARAMETERS
*    None
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
BOOL mdi_audio_is_blocked(void)
{
	return blocked;
}

/******************************************************************
* FUNCTION
*    mdi_audio_block
* DESCRIPTION
*    This function is to block all audio functions through mdi audio interface 
*     except tone and ketytone.
* PARAMETERS
*    module_type mod_id
* RETURNS
*    BOOL
* GLOBALS AFFECTED
*  
******************************************************************/
BOOL mdi_audio_block(module_type mod_id)
{
   //PRINT_INFORMATION(("mdi_audio_block, state=%d, module=%d\n",mdi_audio_state, mod_id));
   mdi_audio_stop_all();
   if( blocked )
      return FALSE;
#ifdef MMI_ON_HARDWARE_P
   media_aud_block( MOD_MMI, mod_id, AUD_BLOCK_ALL );
#endif
   blocked = TRUE;
   return TRUE;
}


/******************************************************************
* FUNCTION
*    mdi_audio_unblock
* DESCRIPTION
*    This function is to unblock all audio functions.
* PARAMETERS
*    None
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_unblock(void)
{
   //PRINT_INFORMATION(("mdi_audio_unblock, state=%d\n",mdi_audio_state));
   if( blocked )
   {
      blocked = FALSE;
#ifdef MMI_ON_HARDWARE_P
      media_aud_block(MOD_MMI, MOD_MMI, AUD_BLOCK_ALL);
#endif
      mdi_start_background_timer();
   }
}

/******************************************************************
* FUNCTION
*    mdi_audio_set_headset_mode_output_path
* DESCRIPTION
*    This function is to set headset mode output path.
* PARAMETERS
*    unsigned char device
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_set_headset_mode_output_path( unsigned char device )
{
#ifdef MMI_ON_HARDWARE_P
	media_aud_set_headset_mode_output_path( MOD_MMI, device );
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_set_vibrator_enabled
* DESCRIPTION
*    This function is to set vibrator enabled
* PARAMETERS
*    BOOL enable
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_set_vibrator_enabled( BOOL enable )
{
#ifdef MMI_ON_HARDWARE_P
	return media_aud_set_vibrator_enabled( MOD_MMI, (kal_uint8) enable );
#else
	return MDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_set_equalizer
* DESCRIPTION
*    This function is to set equalizer
* PARAMETERS
*    U8* magnitude
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_set_equalizer( signed char* magnitude )
{
#ifdef MMI_ON_HARDWARE_P
	return media_aud_set_equalizer( MOD_MMI, (kal_int8*) magnitude );
#else
	return MDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_set_background_handler
* DESCRIPTION
*    This function is to set background play timeout handler.
* PARAMETERS
*    U8 level, mdi_bg_callback handler
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_set_background_handler(U8 level, mdi_bg_callback handler)
{
   if(level<MAX_BACKGROUND_HANDLER)
   {
      background_handler[level]=handler;
      return MDI_AUDIO_SUCCESS;
   }
   else
      return MDI_AUDIO_ERROR;
}

/******************************************************************
* FUNCTION
*    mdi_audio_reset_background_handler
* DESCRIPTION
*    This function is to reset background play timeout handler.
* PARAMETERS
*    U8 level
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_reset_background_handler(U8 level)
{
   if(level<MAX_BACKGROUND_HANDLER)
   {
      background_handler[level]=NULL;
      return MDI_AUDIO_SUCCESS;
   }
   else
      return MDI_AUDIO_ERROR;
}

void mdi_audio_clear_all_background_handler(void)
{
    int i;
    for(i=0;i<MAX_BACKGROUND_HANDLER;i++)
        background_handler[i]=NULL;
}

/******************************************************************
* FUNCTION
*    mdi_audio_set_volume
* DESCRIPTION
*    This function is to set the volume with the given type and volume.
* PARAMETERS
*    None
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_set_volume(U8 type, U8 volume)
{
#if 0
  if(type == AUD_VOLUME_FMR)
  {
    mmi_trace(TRUE,"mdi_audio_set_volume : volume =%d",volume);
	// FM_RDA5800_setVolume(volume);
	//FM_RDA5800_setVolume(volume * 2 + 1);
	MCI_RingSetVolume(volume+1);
   return;
  }
  else if (type == AUD_VOLUME_MEDIA)
  {
	  mmi_trace(1,"mdi_audio_set_volume #AUD_VOLUME_MEDIA#: volume =%d",volume);
	  MCI_AudioSetVolume(volume+1);
	  return;
  }
 #endif
#ifdef MMI_ON_HARDWARE_P
   l4aud_audio_set_volume_req_struct* msg_p;
      
   msg_p = OslConstructDataPtr(sizeof(l4aud_audio_set_volume_req_struct));
   msg_p->audio_type=type;
   msg_p->volume=volume;
   msg_p->aux_volume=volume;
   msg_p->apply=TRUE;
   mdi_send_msg(MSG_ID_L4AUD_AUDIO_SET_VOLUME_REQ, msg_p, NULL);
  
#endif   
}

/******************************************************************
* FUNCTION
*    mdi_audio_set_volume_no_apply
* DESCRIPTION
*    This function is to set the volume with the given type and volume 
*     but this value will not apply to HW.
* PARAMETERS
*    None
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_set_volume_no_apply(U8 type, U8 volume)
{
#ifdef MMI_ON_HARDWARE_P
   l4aud_audio_set_volume_req_struct* msg_p;
      
   msg_p = OslConstructDataPtr(sizeof(l4aud_audio_set_volume_req_struct));
   msg_p->audio_type=type;
   msg_p->volume=volume;
   msg_p->aux_volume=volume;
   msg_p->apply=FALSE;
   mdi_send_msg(MSG_ID_L4AUD_AUDIO_SET_VOLUME_REQ, msg_p, NULL);
  
#endif   
}

/******************************************************************
* FUNCTION
*    mdi_audio_set_mic_volume
* DESCRIPTION
*    This function is to set microphone volume level
* PARAMETERS
*    None
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_set_mic_volume( void )
{
	U8 set_index;
	
	if( mdi_speech_on )
		set_index = AUDIO_MIC_IN_CALL;
	else
		set_index = AUDIO_MIC_APP;

	mdi_audio_set_volume( MDI_VOLUME_MIC, g_settings_audio_mic_volume.levels[set_index] );
}

/******************************************************************
* FUNCTION
*    mdi_audio_get_state
* DESCRIPTION
*    This function is to get the audio state.
* PARAMETERS
*    None
* RETURNS
*    mdi_state
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_state mdi_audio_get_state(void)
{
   return mdi_audio_state;
}


/******************************************************************
* FUNCTION
*    mdi_audio_set_speech_mode
* DESCRIPTION
*    This function is to set the speech mode (on/off).
* PARAMETERS
*    BOOL on
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_set_speech_mode(BOOL on)
{
#ifdef MMI_ON_HARDWARE_P
   l4aud_sp_set_mode_req_struct* msg_p;
      
   msg_p = OslConstructDataPtr(sizeof(l4aud_sp_set_mode_req_struct));

   //mdi_audio_stop_all();
   if(on)
   {
      mdi_speech_on=TRUE;
      mdi_audio_state=MDI_AUDIO_IDLE;
      mdi_stop_background_timer();
   }
   else
   {
      mdi_speech_on=FALSE;
      mdi_audio_state=MDI_AUDIO_IDLE;
      mdi_start_background_timer();
   }

//   mdi_audio_set_mic_volume();
   
   msg_p->speech_on=on;
   mdi_send_msg(MSG_ID_L4AUD_SP_SET_MODE_REQ, msg_p, NULL);
   
#endif   
}

#ifdef __MMI_BT_SUPPORT__
/******************************************************************
* FUNCTION
*    mdi_audio_set_bluetooth_path
* DESCRIPTION
*    This function is to set the bluetooth path (on/off).
* PARAMETERS
*    BOOL on
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_set_bluetooth_path(BOOL on)
{
#ifdef MMI_ON_HARDWARE_P
   l4aud_sp_set_bt_path_req_struct* msg_p;
      
   msg_p = OslConstructDataPtr(sizeof(l4aud_sp_set_bt_path_req_struct));
   msg_p->bluetooth_on=on;
   mdi_send_msg(MSG_ID_L4AUD_SP_SET_BT_PATH_REQ, msg_p, NULL);
   
#endif   
}
#endif

#ifdef __MMI_BT_PROFILE__


/*****************************************************************************
 * FUNCTION
 *  mdi_audio_bt_init
 * DESCRIPTION
 *  This function is to init bluetooth open & close cnf/ind handlers.
 * PARAMETERS
 *  profile             [IN]        
 *  open_handler        [IN]        
 *  close_handler       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mdi_audio_bt_init(U8 profile, mdi_bt_callback open_handler, mdi_bt_callback close_handler)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ASSERT(profile < 2);
    mdi_audio_bt_cntx.open_hdlr[profile] = open_handler;
    mdi_audio_bt_cntx.close_hdlr[profile] = close_handler;
}


/*****************************************************************************
 * FUNCTION
 *  mdi_audio_bt_get_stream_status
 * DESCRIPTION
 *  This function is to get the bt stream state.
 * PARAMETERS
 *  profile     [IN]        
 * RETURNS
 * BOOL
 *****************************************************************************/
U8 mdi_audio_bt_get_stream_status(U8 profile)
{
    return mdi_audio_bt_cntx.state[profile];
}


/*****************************************************************************
 * FUNCTION
 *  mdi_audio_bt_close_stream_ind
 * DESCRIPTION
 *  This function is to indicate bluetooth audio stream close.
 * PARAMETERS
 *  msg     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void mdi_audio_bt_close_stream_ind(void *msg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_audio_close_ind_struct *msg_p = (media_bt_audio_close_ind_struct*) msg;
    U8 profile;
    U16 cause;
    mdi_bt_callback handler;
    U8 state;
    U8 mode;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    profile = msg_p->profile;
    ASSERT(profile < 2);
    cause = msg_p->cause;
    state = mdi_audio_bt_cntx.state[profile];
    handler = mdi_audio_bt_cntx.close_hdlr[profile];


  kal_prompt_trace(1, "mdi_audio_bt_close_stream_ind: profile =%d, state = %d, cause = %d, handler= %x\n", profile,
                       state, cause, handler);

    if (state == MDI_AUDIO_BT_STATE_OPEN || state == MDI_AUDIO_BT_STATE_WAIT_CLOSE)
    {
        mdi_audio_bt_cntx.state[profile] = MDI_AUDIO_BT_STATE_IDLE;
        if (profile == BT_A2DP)
        {
            ASSERT(0);
        }
        /* execute pending connect req */
        if( mdi_audio_bt_cntx.pending_connect[profile] )
        {
            mdi_audio_bt_cntx.pending_connect[profile] = FALSE;
            mdi_audio_bt_cntx.state[profile] = MDI_AUDIO_BT_STATE_WAIT_OPEN;

            if(profile == BT_HFP)
            {
                if(mmi_bt_is_hf_car_kit_connected())
                {
                    mode = BT_HFP_CARKIT_MODE;
                }
                else
                {
                    mode = BT_HFP_EARPHONE_MODE;
                }
            }
            else
            {
                mode = g_ssc_bt_a2dp_mode;
            }
            aud_send_bt_audio_open_req( MOD_MMI, profile, mode, 
                mdi_audio_bt_cntx.connect_id[profile] );
        }
        if (handler != NULL)
        {
            handler(profile, cause);
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  mdi_audio_bt_close_stream_cnf
 * DESCRIPTION
 *  This function is to response bluetooth audio stream close.
 * PARAMETERS
 *  msg     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void mdi_audio_bt_close_stream_cnf(void *msg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_audio_close_cnf_struct *msg_p = (media_bt_audio_close_cnf_struct*) msg;
    U8 profile;
    U16 result;
    mdi_bt_callback handler;
    U8 state;
    U8 mode;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    profile = msg_p->profile;
    ASSERT(profile < 2);
    result = msg_p->result;
    state = mdi_audio_bt_cntx.state[profile];
    handler = mdi_audio_bt_cntx.close_hdlr[profile];

//    MMI_TRACE(MMI_MEDIA_TRC_G4_MDI, MDI_AUDIO_TRC_BT_CLOSE_STREAM_CNF, profile, state, result, handler);

    if (state == MDI_AUDIO_BT_STATE_WAIT_CLOSE)
    {
        mdi_audio_bt_cntx.state[profile] = MDI_AUDIO_BT_STATE_IDLE;
        if (profile == BT_A2DP)
        {
            ASSERT(0); 
        }
        /* execute pending connect req */
        if( mdi_audio_bt_cntx.pending_connect[profile] )
        {
            mdi_audio_bt_cntx.pending_connect[profile] = FALSE;
            mdi_audio_bt_cntx.state[profile] = MDI_AUDIO_BT_STATE_WAIT_OPEN;
            if(profile == BT_HFP)
            {
                if(mmi_bt_is_hf_car_kit_connected())
                {
                    mode = BT_HFP_CARKIT_MODE;
                }
                else
                {
                    mode = BT_HFP_EARPHONE_MODE;
                }
            }
            else
            {
                mode = g_ssc_bt_a2dp_mode;
            }
            aud_send_bt_audio_open_req( MOD_MMI, profile, mode, 
                mdi_audio_bt_cntx.connect_id[profile] );
        }
        if (handler != NULL)
        {
            handler(profile, result);
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  mdi_audio_bt_open_stream_ind
 * DESCRIPTION
 *  This function is to indicate bluetooth audio open.
 * PARAMETERS
 *  msg     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void mdi_audio_bt_open_stream_ind(void *msg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_audio_open_ind_struct *msg_p = (media_bt_audio_open_ind_struct*) msg;
    U8 profile;
    mdi_bt_callback handler;
    U8 state;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    profile = msg_p->profile;
    ASSERT(profile < 2);
    state = mdi_audio_bt_cntx.state[profile];
    handler = mdi_audio_bt_cntx.open_hdlr[profile];

//    MMI_TRACE(MMI_MEDIA_TRC_G4_MDI, MDI_AUDIO_TRC_BT_OPEN_STREAM_IND, profile, state, handler);

 mmi_trace(1, "mdi_audio_bt_open_stream_ind: profile =%d, state = %d handler=%x\n", profile, state,handler);


    if (state == MDI_AUDIO_BT_STATE_IDLE || state == MDI_AUDIO_BT_STATE_WAIT_OPEN)
    {
        mdi_audio_bt_cntx.state[profile] = MDI_AUDIO_BT_STATE_OPEN;
        mdi_audio_bt_cntx.pending_connect[profile] = FALSE;
        if (handler != NULL)
        {
            handler(profile, MDI_AUDIO_SUCCESS);
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  mdi_audio_bt_open_stream_cnf
 * DESCRIPTION
 *  This function is to indicate bluetooth audio stream close.
 * PARAMETERS
 *  msg     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void mdi_audio_bt_open_stream_cnf(void *msg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_audio_open_cnf_struct *msg_p = (media_bt_audio_open_cnf_struct*) msg;
    U8 profile;
    U16 result;
    mdi_bt_callback handler;
    U8 state;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    profile = msg_p->profile;
    ASSERT(profile < 2);
    result = msg_p->result;
    state = mdi_audio_bt_cntx.state[profile];
    handler = mdi_audio_bt_cntx.open_hdlr[profile];

//    MMI_TRACE(MMI_MEDIA_TRC_G4_MDI, MDI_AUDIO_TRC_BT_OPEN_STREAM_CNF, profile, state, result, handler);

    if (state == MDI_AUDIO_BT_STATE_WAIT_OPEN)
    {
        if (result == MDI_AUDIO_SUCCESS)
        {
            mdi_audio_bt_cntx.state[profile] = MDI_AUDIO_BT_STATE_OPEN;
        }
        else
        {
            mdi_audio_bt_cntx.state[profile] = MDI_AUDIO_BT_STATE_IDLE;
        }
        if (handler != NULL)
        {
            handler(profile, result);
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  mdi_audio_bt_open_stream
 * DESCRIPTION
 *  This function is to turn on bluetooth audio stream.
 * PARAMETERS
 *  profile         [IN]        
 *  connect_id      [IN]        
 * RETURNS
 * BOOL
 *****************************************************************************/
BOOL mdi_audio_bt_open_stream(U8 profile, U16 connect_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 state = mdi_audio_bt_cntx.state[profile];
    U8 mode;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
 
 kal_prompt_trace(1, "mdi_audio_bt_open_stream: profile =%d, state = %d connect_id=%x\n", profile, state,connect_id);

    ASSERT(profile < 2);
    if (state == MDI_AUDIO_BT_STATE_IDLE)
    {
        if(profile == BT_HFP)
        {
            if(mmi_bt_is_hf_car_kit_connected())
            {
                mode = BT_HFP_CARKIT_MODE;
            }
            else
            {
                mode = BT_HFP_EARPHONE_MODE;
            }
        }
        else
        {
            mode = g_ssc_bt_a2dp_mode;
        }
        aud_send_bt_audio_open_req(MOD_MMI, profile, mode, connect_id);
        mdi_audio_bt_cntx.state[profile] = MDI_AUDIO_BT_STATE_WAIT_OPEN;

        return TRUE;
    }
    else if( state == MDI_AUDIO_BT_STATE_WAIT_CLOSE )
    {
        /* postpond thie connect req to disconnect ind or cnf */
        mdi_audio_bt_cntx.pending_connect[profile] = TRUE;
        mdi_audio_bt_cntx.connect_id[profile] = connect_id;

        return TRUE;
    }
    return FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  mdi_audio_bt_close_stream
 * DESCRIPTION
 *  This function is to turn off bluetooth audio stream.
 * PARAMETERS
 *  profile     [IN]        
 * RETURNS
 * BOOL
 *****************************************************************************/
BOOL mdi_audio_bt_close_stream(U8 profile)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 state = mdi_audio_bt_cntx.state[profile];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//    MMI_TRACE(MMI_MEDIA_TRC_G4_MDI, MDI_AUDIO_TRC_BT_CLOSE_STREAM, profile, state);
    mmi_trace(g_sw_BT, "mdi_audio_bt_close_stream::profile=%d, state=%d", profile, state);
    ASSERT(profile < 2);

    /* clean up pending connect req */
    if( mdi_audio_bt_cntx.pending_connect[profile] )
    {
        mdi_audio_bt_cntx.pending_connect[profile] = FALSE;
        mdi_audio_bt_cntx.open_hdlr[profile]( profile, MDI_AUDIO_FAIL );
    }

    if (state == MDI_AUDIO_BT_STATE_OPEN || state == MDI_AUDIO_BT_STATE_WAIT_OPEN)
    {
        if(profile == BT_A2DP)
        {
            ASSERT(0); 
        }
        else /* BT_HFP */
        {
            /* Use blocking mode request function for HFP profile */
            /* To make sure MED will send message to BT before MMI continues */
            media_aud_bt_audio_close(profile);
        }
        mdi_audio_bt_cntx.state[profile] = MDI_AUDIO_BT_STATE_WAIT_CLOSE;

        return TRUE;
    }
    else if (profile == BT_A2DP && state == MDI_AUDIO_BT_STATE_IDLE)
    {
        ASSERT(0);
    }

    return FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  mdi_audio_bt_is_stream_open
 * DESCRIPTION
 *  This function is to check the state to know if bt audio is open.
 * PARAMETERS
 *  profile     [IN]        
 * RETURNS
 * BOOL
 *****************************************************************************/
BOOL mdi_audio_bt_is_stream_open(U8 profile)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 state = mdi_audio_bt_cntx.state[profile];
    BOOL result = (state == MDI_AUDIO_BT_STATE_OPEN) || (state == MDI_AUDIO_BT_STATE_WAIT_CLOSE);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//    MMI_TRACE(MMI_MEDIA_TRC_G4_MDI, MDI_AUDIO_TRC_BT_IS_STREAM_OPEN, profile, state, result);

    ASSERT(profile < 2);
    return result;
}


/******************************************************************
* FUNCTION
*    mdi_audio_bt_hfp_turn_on
* DESCRIPTION
*    This function is to change audio path to SCO
* PARAMETERS
*    void
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_bt_hfp_turn_on(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	U8 state = mdi_audio_bt_cntx.state[BT_HFP];
	state = state;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//    MMI_TRACE(MMI_MEDIA_TRC_G4_MDI, MDI_AUDIO_TRC_BT_HFP_TURN_ON, state);

    if(mdi_audio_bt_is_stream_open(BT_HFP))
    {
        aud_send_bt_audio_turn_on_req( MOD_MMI, BT_HFP );
    }
}


/******************************************************************
* FUNCTION
*    mdi_audio_bt_hfp_turn_off
* DESCRIPTION
*    This function is to change audio path to mobile device
* PARAMETERS
*    void
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_bt_hfp_turn_off(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	U8 state = mdi_audio_bt_cntx.state[BT_HFP];
	state = state;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//    MMI_TRACE(MMI_MEDIA_TRC_G4_MDI, MDI_AUDIO_TRC_BT_HFP_TURN_OFF, state);

    if(mdi_audio_bt_is_stream_open(BT_HFP))
    {
        /* Use blocking mode request function for HFP profile */
        /* To make sure MED will send message to BT before MMI continues */    
        media_aud_bt_turn_off_audio(BT_HFP );
    }        
}
#endif /* __MMI_BT_PROFILE__ */ 

#ifdef __MMI_A2DP_SUPPORT__
/******************************************************************
* FUNCTION
*    mdi_audio_bt_is_a2dp_codec_open
* DESCRIPTION
*    This function is to check if a2dp is under streaming state or codec
*    is open. For performance tuning querying.
* PARAMETERS
*    void
* RETURNS
*    BOOL
* GLOBALS AFFECTED
*  
******************************************************************/
BOOL mdi_audio_bt_is_a2dp_codec_open(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_bool result = KAL_FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

#if defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_A2DP_SUPPORT__)
    result = av_bt_is_codec_open();
//    MMI_TRACE(MMI_MEDIA_TRC_G4_MDI, MDI_AUDIO_TRC_BT_IS_A2DP_CODEC_ON, result);
#endif

    return result ;
}
#endif /*__MMI_A2DP_SUPPORT__*/

/******************************************************************
* FUNCTION
*    mdi_audio_set_mute
* DESCRIPTION
*    This function is to set mute on the given volume type.
* PARAMETERS
*    U8 vol_type
*    BOOL on
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_set_mute(U8 vol_type, BOOL mute)
{
#ifdef MMI_ON_HARDWARE_P
   media_aud_set_mute_req_struct* msg_p;
      
   msg_p = OslConstructDataPtr(sizeof(media_aud_set_mute_req_struct));

   msg_p->vol_type=vol_type;   
   msg_p->mute=mute;

   mdi_send_msg(MSG_ID_MEDIA_AUD_SET_MUTE_REQ, msg_p, NULL);
   
#endif   
}


/******************************************************************
* FUNCTION
*    mdi_send_msg
* DESCRIPTION
*    This function is to send message to MED task.
* PARAMETERS
*    U16 msg_id,void *local_param_ptr, void *peer_buf_ptr
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_send_msg(U32 msg_id,void *local_param_ptr, void *peer_buf_ptr)
{
#ifdef MMI_ON_HARDWARE_P
   ilm_struct       *ilm_ptr = NULL;

   ilm_ptr = allocate_ilm(MOD_MMI);
   ilm_ptr->src_mod_id = MOD_MMI;
   ilm_ptr->sap_id = MED_SAP;//MMI_L4C_SAP;
   ilm_ptr->dest_mod_id = MOD_MED;//MOD_L4C;
   
   ilm_ptr->msg_id = msg_id;
   ilm_ptr->local_para_ptr = (local_para_struct*)local_param_ptr;
   ilm_ptr->peer_buff_ptr = (peer_buff_struct*) peer_buf_ptr;

   msg_send_ext_queue(ilm_ptr);
#endif
} 


/******** For multiple players ****************/

/******************************************************************
* FUNCTION
*    mdi_audio_mma_open_midi
* DESCRIPTION
*    This function is to open one midi player. Currently one player is supported.
* PARAMETERS
*    void* audio_data 
*    U32 len 
*    U8 repeats 
*    mdi_mma_callback handler
* RETURNS
*    mdi_handle
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_handle mdi_audio_mma_open_midi(void* audio_data, 
                                   U32 len, 
                                   U8 repeats, 
                                   mdi_mma_callback handler)
{
#if defined(MMI_ON_HARDWARE_P) && defined(DSP_WT_SYN)
   mma_player_struct param;
   mdi_handle handle=0;

   param.data= (kal_uint8*)audio_data;
   param.size=len;
   param.repeats=repeats;
   handle=media_mma_midi_open(handler, &param);
   return handle;
#else
   return 0;
#endif   
}

/******************************************************************
* FUNCTION
*    mdi_audio_mma_close_midi
* DESCRIPTION
*    This function is to close midi player. 
* PARAMETERS
*    mdi_handle handle
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_mma_close_midi(mdi_handle handle)
{
#if defined(MMI_ON_HARDWARE_P) && defined(DSP_WT_SYN)
   return media_mma_midi_close(handle);
#else
   return 0;
#endif
}


/******************************************************************
* FUNCTION
*    mdi_audio_mma_play_midi
* DESCRIPTION
*    This function is to play midi. 
* PARAMETERS
*    mdi_handle handle
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_mma_play_midi(mdi_handle handle)
{
#if defined(MMI_ON_HARDWARE_P) && defined(DSP_WT_SYN)
   return media_mma_midi_play(handle);
#else
   return 0;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_mma_stop_midi
* DESCRIPTION
*    This function is to stop midi. 
* PARAMETERS
*    mdi_handle handle
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_mma_stop_midi(mdi_handle handle)
{
#if defined(MMI_ON_HARDWARE_P) && defined(DSP_WT_SYN)
   return media_mma_midi_stop(handle);
#else
   return 0;
#endif
}


/******************************************************************
* FUNCTION
*    mdi_audio_mma_open_wav
* DESCRIPTION
*    This function is to open one wav player. Currently 3 players is supported.
* PARAMETERS
*    void* audio_data, 
*    U32 len, 
*    U8 repeats, 
*    mdi_mma_callback handler
* RETURNS
*    mdi_handle
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_handle mdi_audio_mma_open_wav(void* audio_data, 
                                   U32 len, 
                                   U8 repeats, 
                                   mdi_mma_callback handler)
{
#if defined(MMI_ON_HARDWARE_P) && defined(DSP_WT_SYN)
   mma_player_struct param;
   mdi_handle handle=0;

   param.data= (kal_uint8*)audio_data;
   param.size=len;
   param.repeats=repeats;
   
   handle=media_mma_wav_open(handler, &param);

   return handle;
#else
   return 0;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_mma_close_wav
* DESCRIPTION
*    This function is to close wav player. 
* PARAMETERS
*    mdi_handle handle
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_mma_close_wav(mdi_handle handle)
{
#if defined(MMI_ON_HARDWARE_P) && defined(DSP_WT_SYN)
   return media_mma_wav_close(handle);
#else
   return 0;
#endif
}


/******************************************************************
* FUNCTION
*    mdi_audio_mma_play_wav
* DESCRIPTION
*    This function is to play wav. 
* PARAMETERS
*    mdi_handle handle
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_mma_play_wav(mdi_handle handle)
{
#if defined(MMI_ON_HARDWARE_P) && defined(DSP_WT_SYN)
   return media_mma_wav_play(handle);
#else
   return 0;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_mma_stop_wav
* DESCRIPTION
*    This function is to stop wav. 
* PARAMETERS
*    mdi_handle handle
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_mma_stop_wav(mdi_handle handle)
{
#if defined(MMI_ON_HARDWARE_P) && defined(DSP_WT_SYN)
   return media_mma_wav_stop(handle);
#else
   return 0;
#endif
}
 
/******************************************************************
* FUNCTION
*    mdi_fmr_power_on_with_path
* DESCRIPTION
*    This function is to turn on FM radio with specific output path
* PARAMETERS
*    U8 path,
*	mdi_callback handler
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_fmr_power_on_with_path( U8 path, mdi_callback handler )
{
	if( blocked || (mdi_audio_state == MDI_AUDIO_PLAY && mdi_audio_mode == MDI_AUDIO_PLAY_FM_RADIO) )
	{
		/*when fm radio is played from background to foreground,set fmr_play_from_bg_to_fg as TRUE*/	
		fmr_play_from_bg_to_fg = TRUE;
		return;
	}

	if( mdi_audio_state != MDI_AUDIO_IDLE )
		mdi_audio_stop_all();

	mdi_stop_background_timer();
#ifdef MMI_ON_HARDWARE_P
	media_aud_fmr_power_on( MOD_MMI, path);
#endif

   /* Combinations to connect SCO */
#if defined(__MMI_BT_FM_VIA_SCO__)
    mmi_profiles_bt_connect_audio_via_sco();        
#elif defined(__MMI_BT_AUDIO_VIA_SCO__)
    mmi_profiles_bt_disable_sco_connection(MMI_PROFILE_BT_SCO_INT_FMRADIO);
#endif



	currentHandler = handler;
	mdi_audio_state = MDI_AUDIO_PLAY;
	mdi_audio_mode = MDI_AUDIO_PLAY_FM_RADIO;
	
}

/******************************************************************
* FUNCTION
*    mdi_audio_stop_fmr
* DESCRIPTION
*    This function is to stop FM radio playing/recording
* PARAMETERS
*    None
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_stop_fmr( void )
{
	//PRINT_INFORMATION( ("mdi_audio_stop_fmr, state=%d, mode=%d\n",mdi_audio_state,mdi_audio_mode));

	if( mdi_audio_state == MDI_AUDIO_PLAY && mdi_audio_mode == MDI_AUDIO_PLAY_FM_RADIO )
	{
		if( currentHandler )
			currentHandler( MDI_AUDIO_TERMINATED );
		mdi_fmr_power_off();
	}
	/*Stop FM Record and Turn off FM Radio*/
	else if( (mdi_audio_state == MDI_AUDIO_RECORD || mdi_audio_state == MDI_AUDIO_RECORD_PAUSED) && 
	             mdi_audio_mode == MDI_AUDIO_RECORD_FM_RADIO )
	{ 
	#ifdef FM_RADIO_RECORD

		mdi_fmr_stop_record(); /*Stop before callback because need to handle record file in MMI*/
	#endif
		if( currentHandler )
		{
			currentHandler( MDI_AUDIO_TERMINATED );
		}
		mdi_fmr_power_off();
	}

	return MDI_AUDIO_SUCCESS;
}

/******************************************************************
* FUNCTION
*    mdi_fmr_power_off
* DESCRIPTION
*    This function is to turn off FM radio
* PARAMETERS
*    None
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_fmr_power_off( void )
{
	if( mdi_audio_state == MDI_AUDIO_IDLE )
		return;

#ifdef MMI_ON_HARDWARE_P
	media_aud_fmr_power_off( MOD_MMI );
#endif

    /* Combinations to connect SCO */
#if defined(__MMI_BT_FM_VIA_SCO__)
    #ifndef __MMI_BT_AUDIO_VIA_SCO__
        mmi_profiles_bt_disconnect_sco();
    #endif        
#elif defined(__MMI_BT_AUDIO_VIA_SCO__)
    mmi_profiles_bt_enable_sco_connection(MMI_PROFILE_BT_SCO_INT_FMRADIO);
#endif  

	mdi_audio_state = MDI_AUDIO_IDLE;
	mdi_start_background_timer();
}

/******************************************************************
* FUNCTION
*    mdi_fmr_set_freq
* DESCRIPTION
*    This function is to set the frequency of FM radio.
* PARAMETERS
*    U16 freq
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_fmr_set_freq( U16 freq )
{
	if( mdi_audio_state == MDI_AUDIO_IDLE )
		return;

#ifdef MMI_ON_HARDWARE_P
	media_aud_fmr_set_freq( MOD_MMI, freq );
#endif
}

/******************************************************************
* FUNCTION
*    mdi_fmr_mute
* DESCRIPTION
*    This function is to mute the FM radio
* PARAMETERS
*    U8 mute
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_fmr_mute( U8 mute )
{
	if( mdi_audio_state == MDI_AUDIO_IDLE )
		return;

#ifdef MMI_ON_HARDWARE_P
	media_aud_fmr_mute( MOD_MMI, mute );
#endif
}

/******************************************************************
* FUNCTION
*    mdi_fmr_check_is_valid_stop_cnf
* DESCRIPTION
*    This function is the callback function of check valid stop
* PARAMETERS
*    void* inMsg
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_fmr_check_is_valid_stop_cnf( void* inMsg )
{
#ifdef MMI_ON_HARDWARE_P
	media_fmr_check_is_valid_stop_cnf_struct* msg_p = (media_fmr_check_is_valid_stop_cnf_struct*)inMsg;
	if( mdi_fmr_check_is_valid_stop_hdlr != NULL )
		mdi_fmr_check_is_valid_stop_hdlr( msg_p->is_valid );
#endif
}

/******************************************************************
* FUNCTION
*    mdi_fmr_check_is_valid_stop
* DESCRIPTION
*    This function is to check if a frequency is valid stop
* PARAMETERS
*    U16 freq,
*	BOOL is_set_up,
*	void (*callback_func)(BOOL is_valid)
*
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_fmr_check_is_valid_stop( U16 freq, BOOL is_step_up, void (*callback_func)(BOOL is_valid) )
{
#ifdef MMI_ON_HARDWARE_P
	media_fmr_check_is_valid_stop_req_struct* msg_p = OslConstructDataPtr( sizeof(media_fmr_check_is_valid_stop_req_struct) );
	msg_p->freq = freq;
	msg_p->is_step_up = is_step_up;
	mdi_fmr_check_is_valid_stop_hdlr = callback_func;
	SetProtocolEventHandler( mdi_fmr_check_is_valid_stop_cnf, MSG_ID_MEDIA_FMR_CHECK_IS_VALID_STOP_CNF );
	mdi_send_msg( MSG_ID_MEDIA_FMR_CHECK_IS_VALID_STOP_REQ, msg_p, NULL );
#endif
}

/******************************************************************
* FUNCTION
*    mdi_fmr_cancel_checking_is_valid_stop
* DESCRIPTION
*    This function is to cancel check valid stop
* PARAMETERS
*    None
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_fmr_cancel_checking_is_valid_stop( void )
{
#ifdef MMI_ON_HARDWARE_P
	SetProtocolEventHandler( NULL, MSG_ID_MEDIA_FMR_CHECK_IS_VALID_STOP_CNF );
	mdi_fmr_check_is_valid_stop_hdlr = NULL;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_fmr_get_signal_level_cnf
* DESCRIPTION
*    Thsi function is the callback function of get signal level
* PARAMETERS
*    void* inMsg
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_fmr_get_signal_level_cnf( void* inMsg )
{
#ifdef MMI_ON_HARDWARE_P
	media_fmr_get_signal_level_cnf_struct* msg_p = (media_fmr_get_signal_level_cnf_struct*)inMsg;
	if( mdi_fmr_get_signal_level_hdlr != NULL )
		mdi_fmr_get_signal_level_hdlr( msg_p->signal_level );
#endif
}

/******************************************************************
* FUNCTION
*    mdi_fmr_get_signal_level
* DESCRIPTION
*	This function is to get the signal level of a frequency
* PARAMETERS
*    U16 freq, 
*	BOOL is_step_up, 
*	void (*callback_func)(U8 sig_lvl)
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_fmr_get_signal_level( U16 freq, BOOL is_step_up, void (*callback_func)(U8 sig_lvl) )
{
#ifdef MMI_ON_HARDWARE_P
	media_fmr_get_signal_level_req_struct* msg_p = OslConstructDataPtr( sizeof(media_fmr_get_signal_level_req_struct) );
	msg_p->freq = freq;
	msg_p->is_step_up = is_step_up;
	mdi_fmr_get_signal_level_hdlr = callback_func;
	SetProtocolEventHandler( mdi_fmr_get_signal_level_cnf, MSG_ID_MEDIA_FMR_GET_SIGNAL_LEVEL_CNF );
	mdi_send_msg( MSG_ID_MEDIA_FMR_GET_SIGNAL_LEVEL_REQ, msg_p, NULL );
#endif
}

/******************************************************************
* FUNCTION
*    mdi_fmr_cancel_getting_signal_level
* DESCRIPTION
*    This fuction is to cancel get signal level request
* PARAMETERS
*    None
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_fmr_cancel_getting_signal_level( void )
{
#ifdef MMI_ON_HARDWARE_P
	SetProtocolEventHandler( NULL, MSG_ID_MEDIA_FMR_GET_SIGNAL_LEVEL_CNF );
	mdi_fmr_get_signal_level_hdlr = NULL;
#endif
}
 


// FM Radio Record Begin
#if 1 /*FM_RADIO_RECORD*/
/******************************************************************
* FUNCTION
*    mdi_fmr_start_record
* DESCRIPTION
*   This function is to start record for FM radio
* PARAMETERS
*   void* file_name, 
*	U8 format, 
*	mdi_handle* handle_p, 
*	mdi_callback handler
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_fmr_start_record(void* file_name, U8 format, mdi_handle* handle_p, mdi_callback handler)
{
#if defined(MMI_ON_HARDWARE_P) && !defined(__AMNT__)
   mdi_result result;
#endif
   
  TRACE_EF(1,"mdi_fmr_start_record, state=[%d], mode=[%d]\n",mdi_audio_state, mdi_audio_mode);

   if( blocked )
   {
   	return MDI_AUDIO_BLOCKED;
   }
   else if( mdi_audio_state == MDI_AUDIO_RECORD || mdi_audio_state == MDI_AUDIO_RECORD_PAUSED )
   {
	return MDI_AUDIO_BUSY;
   }
   else if( currentHandler && (mdi_audio_state == MDI_AUDIO_PLAY || mdi_audio_state == MDI_AUDIO_PLAY_PAUSED
#if defined( DIGIT_TONE_SUPPORT ) || defined(SIMPLE_TTS)
		|| mdi_audio_play_text_context.state == MDI_AUDIO_PLAY_TEXT_WAIT_STATE
#endif
   ) )
   {  
      /*Don't turn off FM radio*/
      if( !(mdi_audio_state == MDI_AUDIO_PLAY && mdi_audio_mode == MDI_AUDIO_PLAY_FM_RADIO) )
      {
         currentHandler( MDI_AUDIO_TERMINATED );
      }
   }

   /*Must turn on FM radio first*/
   if( !(mdi_audio_state == MDI_AUDIO_PLAY && mdi_audio_mode == MDI_AUDIO_PLAY_FM_RADIO) )
   {
   #if defined(MMI_ON_HARDWARE_P)
      return MDI_AUDIO_FAIL;
   #else
	   currentHandler=handler;
	   mdi_audio_state=MDI_AUDIO_RECORD;
	   mdi_audio_mode=MDI_AUDIO_RECORD_FM_RADIO;
	   return MDI_AUDIO_SUCCESS;
   #endif
   }

   mdi_stop_background_timer();

#if defined(MMI_ON_HARDWARE_P) && !defined(__AMNT__)

   if((result=media_aud_start_record(MOD_MMI, (kal_wchar *) file_name, format, 7,FALSE, MDI_INPUT_FMRR))==MED_RES_OK)
   {
      currentHandler=handler;
      mdi_audio_state=MDI_AUDIO_RECORD;
      mdi_audio_mode=MDI_AUDIO_RECORD_FM_RADIO;
      SetProtocolEventHandler(mdi_fmr_record_finish_ind, MSG_ID_L4AUD_MEDIA_RECORD_FINISH_IND);
      return MDI_AUDIO_SUCCESS;
   }
   else
   {
      //currentHandler=NULL;
      //mdi_audio_state=MDI_AUDIO_IDLE;
      //mdi_start_background_timer();
      return mdi_get_event((U8)result);
   }
#else
   currentHandler=handler;
   mdi_audio_state=MDI_AUDIO_RECORD;
   mdi_audio_mode=MDI_AUDIO_RECORD_FM_RADIO;
   return MDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_fmr_record_finish_ind
* DESCRIPTION
*    This function is the callback function of finish FM radio record
* PARAMETERS
*    void* inMsg
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_fmr_record_finish_ind(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   l4aud_media_record_finish_ind_struct* msg_p=(l4aud_media_record_finish_ind_struct*)inMsg;
   TRACE_EF(1,"chenhe,mdi_fmr_record_finish_ind");

   //PRINT_INFORMATION(("mdi_audio_record_finish_ind, state=%d\n",mdi_audio_state));
   if(mdi_audio_state==MDI_AUDIO_RECORD)
   {
      /*Go back to FM radio playing state*/
      mdi_audio_state=MDI_AUDIO_PLAY;
      mdi_audio_mode = MDI_AUDIO_PLAY_FM_RADIO;
      
      if (currentHandler)
      {
         previousHandler=currentHandler;
         //currentHandler=NULL; /* Clear at stop FM radio time.*/
         previousHandler(mdi_get_event(msg_p->result));
      }
      //mdi_start_background_timer();
   }   
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_stop_fmr_record_only
* DESCRIPTION
*    This function is to stop FM recording and call back to MMI.
*    After calling this function, FM radio will still playing
*
* PARAMETERS
*    None
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_stop_fmr_record_only( void )
{
   //PRINT_INFORMATION(("mdi_audio_stop_fmr_record_only, state=%d\n",mdi_audio_state));
   
   if (blocked)
      return MDI_AUDIO_BLOCKED;

   /*Need to chceck is recording before stop it ???*/   
   mdi_fmr_stop_record();

   if (currentHandler)
   {
   	/*Still keep callback for playing*/
     currentHandler(MDI_AUDIO_STOP_FM_RECORD);
   }

   //mdi_start_background_timer();

   return MDI_AUDIO_SUCCESS;
}

/******************************************************************
* FUNCTION
*    mdi_fmr_stop_record
* DESCRIPTION
*    This function is to stop FM recording in media task
* PARAMETERS
*    None
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_fmr_stop_record( void )
{
   if( mdi_audio_state == MDI_AUDIO_IDLE )
      return;

   /*Stop FM record*/
#ifdef MMI_ON_HARDWARE_P
   SetProtocolEventHandler(NULL, MSG_ID_L4AUD_MEDIA_RECORD_FINISH_IND);
   media_aud_stop_record(MOD_MMI, SRC_LMMI);
#endif

   /*Remain in playing state*/
   mdi_audio_state = MDI_AUDIO_PLAY;
   mdi_audio_mode = MDI_AUDIO_PLAY_FM_RADIO;
}

/******************************************************************
* FUNCTION
*    mdi_fmr_pause_record
* DESCRIPTION
*    This function is to pause FM radio record
* PARAMETERS
*    mdi_callback handler
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_fmr_pause_record(mdi_callback handler)
{
   mdi_result  result;

   //PRINT_INFORMATION(("mdi_fmr_pause_record, state=%d\n",mdi_audio_state));
   if (blocked)
      return MDI_AUDIO_BLOCKED;

#if defined(MMI_ON_HARDWARE_P) && !defined(__AMNT__)
   if((result = media_aud_pause(MOD_MMI, SRC_LMMI))==MED_RES_OK)
   {
      mdi_audio_state=MDI_AUDIO_RECORD_PAUSED;
      currentHandler=handler;
      return MDI_AUDIO_SUCCESS;
   }
   //else if(result == MED_RES_UNSUPPORTED_OPERATION)
   //{
   //	  return MDI_AUDIO_UNSUPPORTED_OPERATION;
   //}
   else
   {
      //currentHandler=NULL;
      mdi_audio_state=MDI_AUDIO_PLAY;
      mdi_audio_mode = MDI_AUDIO_PLAY_FM_RADIO;
      //mdi_start_background_timer();
      return mdi_get_event((U8)result);
   }
#else
   result = MDI_AUDIO_SUCCESS;
   mdi_audio_state=MDI_AUDIO_RECORD_PAUSED;
   currentHandler=handler;
   return result;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_frm_resume_record
* DESCRIPTION
*    This function is to resume FM radio record
* PARAMETERS
*    mdi_callback handler
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_frm_resume_record(mdi_callback handler)
{
   mdi_result  result;

   //PRINT_INFORMATION(("mdi_audio_resume, state=%d\n",mdi_audio_state));
   if (blocked)
      return MDI_AUDIO_BLOCKED;

#if defined(MMI_ON_HARDWARE_P) && !defined(__AMNT__)
   if((result = media_aud_resume(MOD_MMI, SRC_LMMI))==MED_RES_OK)
   {
      mdi_audio_state = MDI_AUDIO_RECORD;
      mdi_audio_mode = MDI_AUDIO_RECORD_FM_RADIO;
      currentHandler=handler;
      return MDI_AUDIO_SUCCESS;
   }
   else
   {
      currentHandler=NULL;
      mdi_audio_state = MDI_AUDIO_PLAY;
      mdi_audio_mode = MDI_AUDIO_PLAY_FM_RADIO;
      //mdi_start_background_timer();
      return mdi_get_event((U8)result);
   }
#else
   result = MDI_AUDIO_SUCCESS;
   mdi_audio_state=MDI_AUDIO_RECORD;
   mdi_audio_mode = MDI_AUDIO_RECORD_FM_RADIO;
   currentHandler=handler;
   return result;
#endif
}

#endif /*FM Record Enable*/
// FM Radio Record END

#ifdef VR_ENABLE
/******************************************************************
* FUNCTION
*    mdi_audio_play_voice_tag
* DESCRIPTION
*    This function is to play vr voice tag
* PARAMETERS
*    U8 mode
*    U8 lang
*    U8 group_id
*    U16 word_id
*    mdi_handle* handle_p
*    mdi_callback handler
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_play_voice_tag( U8 mode, U8 lang, U8 group_id, U16 word_id, mdi_handle* handle_p, mdi_callback handler )
{
   mdi_result  result;
   
   //PRINT_INFORMATION(("mdi_audio_play_voice_tag, mode=%d, lang=%d, group_id=%d, word_id=%d, state=%d\n",mode,lang,group_id,word_id,mdi_audio_state));

   if( !mdi_audio_check_states_and_handlers( &result ) )
      return result;

   ++identifier;
   mdi_stop_background_timer();

       /* All sound shall go through bluetooth earphone */
#ifdef __MMI_BT_AUDIO_VIA_SCO__
    mmi_profiles_bt_connect_audio_via_sco();
#endif

   
#ifdef MMI_ON_HARDWARE_P
   if((result = media_aud_vr_play_tag( MOD_MMI, mode, lang, group_id, word_id, identifier ))==MED_RES_OK)
   {
      currentHandler=handler;
      mdi_audio_state=MDI_AUDIO_PLAY;
      mdi_audio_mode=MDI_AUDIO_PLAY_FILE;
      SetProtocolEventHandler(mdi_audio_play_finish_ind1, MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND);
      return MDI_AUDIO_SUCCESS;
   }
   else
   {
      currentHandler=NULL;
      mdi_audio_state=MDI_AUDIO_IDLE;
      mdi_start_background_timer();
      return mdi_get_event((U8)result);
   }
#else
   currentHandler=handler;
   mdi_audio_state=MDI_AUDIO_PLAY;
   mdi_audio_mode=MDI_AUDIO_PLAY_FILE;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_vr_rcg_result_ind
* DESCRIPTION
*    This function is to handle vr recognition result indication
* PARAMETERS
*    void* inMsg
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_vr_rcg_result_ind( void* inMsg )
{
#ifdef MMI_ON_HARDWARE_P
	media_vr_rcg_result_ind_struct* msg_p = (media_vr_rcg_result_ind_struct*)inMsg;

	//PRINT_INFORMATION(("mdi_audio_vr_rcg_result_ind, result=%d,state=%d\n",msg_p->result, mdi_audio_state));
	if( mdi_audio_state == MDI_AUDIO_RECORD )
	{
		mdi_audio_state = MDI_AUDIO_IDLE;
		if( currentHandler )
		{
			previousHandler = currentHandler;
			currentHandler = NULL;
			mdi_audio_vr_rcg_result_msg = msg_p;
			previousHandler( mdi_get_event(msg_p->result) );
			mdi_audio_vr_rcg_result_msg = NULL;
		}
		mdi_start_background_timer();
	}
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_vr_trn_result_ind
* DESCRIPTION
*    This function is to handle vr training result indication
* PARAMETERS
*    void* inMsg
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_vr_trn_result_ind( void* inMsg )
{
#ifdef MMI_ON_HARDWARE_P
	media_vr_trn_result_ind_struct* msg_p = (media_vr_trn_result_ind_struct*)inMsg;

	//PRINT_INFORMATION(("mdi_audio_vr_trn_result_ind, result=%d,state=%d\n",msg_p->result, mdi_audio_state));
	if( mdi_audio_state == MDI_AUDIO_RECORD )
	{
		mdi_audio_state = MDI_AUDIO_IDLE;
		if( currentHandler )
		{
			previousHandler = currentHandler;
			currentHandler = NULL;
			mdi_audio_vr_trn_result_msg = msg_p;
			previousHandler( mdi_get_event(msg_p->result) );
			mdi_audio_vr_trn_result_msg = NULL;
		}
		mdi_start_background_timer();
	}
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_vr_rcg_session_id
* DESCRIPTION
*    This function is to get vr rcg session id
* PARAMETERS
*    void
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
U32 mdi_audio_vr_rcg_session_id( void )
{
	if( mdi_audio_vr_rcg_result_msg != NULL )
		return mdi_audio_vr_rcg_result_msg->session_id;
	return 0;
}

/******************************************************************
* FUNCTION
*    mdi_audio_vr_trn_session_id
* DESCRIPTION
*    This function is to get vr trn session id
* PARAMETERS
*    void
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
U32 mdi_audio_vr_trn_session_id( void )
{
	if( mdi_audio_vr_trn_result_msg != NULL )
		return mdi_audio_vr_trn_result_msg->session_id;
	return 0;
}

/******************************************************************
* FUNCTION
*    mdi_audio_vr_rcg_result_id_length
* DESCRIPTION
*    This function is to get vr rcg result id length
* PARAMETERS
*    void
* RETURNS
*    U16
* GLOBALS AFFECTED
*  
******************************************************************/
U16 mdi_audio_vr_rcg_result_id_length( void )
{
	if( mdi_audio_vr_rcg_result_msg != NULL )
		return mdi_audio_vr_rcg_result_msg->id_length;
	return 0;
}

/******************************************************************
* FUNCTION
*    mdi_audio_vr_rcg_result_id_array
* DESCRIPTION
*    This function is to get vr rcg result id array
* PARAMETERS
*    void
* RETURNS
*    U16*
* GLOBALS AFFECTED
*  
******************************************************************/
U16* mdi_audio_vr_rcg_result_id_array( void )
{
	if( mdi_audio_vr_rcg_result_msg != NULL )
		return mdi_audio_vr_rcg_result_msg->id_array;
	return NULL;
}

/******************************************************************
* FUNCTION
*    mdi_audio_start_vr_voice_record
* DESCRIPTION
*    This function is to start vr voice record
* PARAMETERS
*    U8 mode
*    U8 lang
*    U8 group_id
*    U16 word_id
*    mdi_handle* handle_p
*    mdi_callback handler
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_start_vr_voice_record( U32 session_id, U8 seq_no, mdi_handle* handle_p, mdi_callback handler )
{
	mdi_result  result;

	//PRINT_INFORMATION(("mdi_audio_record_vr_voice, session id=%d, seq no=%d, state=%d\n",session_id,seq_no,mdi_audio_state));

	if( !mdi_audio_check_states_and_handlers( &result ) )
		return result;

	mdi_stop_background_timer();	

#ifdef MMI_ON_HARDWARE_P
	if( (result = media_aud_vr_voice_in( MOD_MMI, session_id, seq_no )) == MED_RES_OK )
	{
		currentHandler = handler;
		mdi_audio_state = MDI_AUDIO_RECORD;
		mdi_audio_mode = MDI_AUDIO_RECORD_VR_VOICE;
		SetProtocolEventHandler( mdi_audio_vr_rcg_result_ind, MSG_ID_MEDIA_VR_RCG_RESULT_IND );
		SetProtocolEventHandler( mdi_audio_vr_trn_result_ind, MSG_ID_MEDIA_VR_TRN_RESULT_IND );
		return MDI_AUDIO_SUCCESS;
	}
	else
	{
		currentHandler = NULL;
		mdi_audio_state = MDI_AUDIO_IDLE;
		mdi_start_background_timer();
		return mdi_get_event( (U8)result );
	}
#else
	currentHandler = handler;
	mdi_audio_state = MDI_AUDIO_RECORD;
	mdi_audio_mode = MDI_AUDIO_RECORD_VR_VOICE;
	return MDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_stop_vr_voice_record
* DESCRIPTION
*    This function is to stop vr voice record.
* PARAMETERS
*    void
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_stop_vr_voice_record(void)
{
	//PRINT_INFORMATION(("mdi_audio_stop_vr voice record, state=%d\n",mdi_audio_state));
#ifdef MMI_ON_HARDWARE_P
	media_aud_vr_abort( MOD_MMI );
#endif
   TRACE_EF(1,"chenhe,mdi_audio_stop_vr_voice_record");

	if (blocked)
		return MDI_AUDIO_BLOCKED;

#ifdef MMI_ON_HARDWARE_P
	SetProtocolEventHandler( NULL, MSG_ID_MEDIA_VR_RCG_RESULT_IND );
	SetProtocolEventHandler( NULL, MSG_ID_MEDIA_VR_TRN_RESULT_IND );
#endif

	mdi_audio_state = MDI_AUDIO_IDLE;
	if( currentHandler )
	{
		previousHandler = currentHandler;
		currentHandler = NULL;
		previousHandler( MDI_AUDIO_TERMINATED );
	}
	mdi_start_background_timer();

	return MDI_AUDIO_SUCCESS;
}

/******************************************************************
* FUNCTION
*    mdi_audio_vr_abort
* DESCRIPTION
*    This function is to abort vr.
* PARAMETERS
*    void
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_vr_abort( void )
{
	if( mdi_audio_state == MDI_AUDIO_RECORD && mdi_audio_mode == MDI_AUDIO_RECORD_VR_VOICE )
		mdi_audio_stop_vr_voice_record();
#ifdef MMI_ON_HARDWARE_P
	else
		media_aud_vr_abort( MOD_MMI );
#endif
}

#endif

#ifdef VRSI_ENABLE
/******************************************************************
* FUNCTION
*    mdi_audio_vrsi_trn_prompt_req_hdlr
* DESCRIPTION
*    This function is to inform MMI to prompt user training stage.
* PARAMETERS
*    void* inMsg
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_vrsi_trn_prompt_req_hdlr(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   media_vrsi_trn_prompt_req_struct* msg_p=(media_vrsi_trn_prompt_req_struct*)inMsg;

   //PRINT_INFORMATION(("mdi_audio_vrsi_trn_prompt_req_hdlr, state=%d\n",mdi_audio_state));
  
   switch(msg_p->pmp_mode)
   {
      case MED_VRSI_1ST_TRN_PROMPT:                            
         vrsiHandler(MED_VRSI_TRAIN_TAG_1ST_MPR);  
         break;
      case MED_VRSI_2ND_TRN_PROMPT: 
         vrsiHandler(MED_VRSI_TRAIN_TAG_2ND_MPR);
         break;
      case MED_VRSI_1ST_DGT_ADP_PROMPT:      
      case MED_VRSI_2ND_DGT_ADP_PROMPT:    
         vrsiHandler(MED_VRSI_DIGIT_ADAPT_MPR);
         break;
   }                   
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_vrsi_trn_result_ind
* DESCRIPTION
*    This function is to inform MMI train result.
* PARAMETERS
*    void* inMsg
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_vrsi_trn_result_ind(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   media_vrsi_trn_result_ind_struct* msg_p=(media_vrsi_trn_result_ind_struct*)inMsg;

   //PRINT_INFORMATION(("mdi_audio_vrsi_trn_result_ind, state=%d\n",mdi_audio_state));
   
   if(msg_p->tag_id == 0)             
      vrsiHandler(MED_VRSI_DIGIT_ADAPT_OK);            
   else 
      vrsiHandler(MED_VRSI_TRAIN_TAG_OK);    
   
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_vrsi_rcg_prompt_req_hdlr
* DESCRIPTION
*    This function is to inform MMI to prompt user recognition stage.
* PARAMETERS
*    void* inMsg
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_vrsi_rcg_prompt_req_hdlr(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   media_vrsi_rcg_prompt_req_struct* msg_p=(media_vrsi_rcg_prompt_req_struct*)inMsg;

   //PRINT_INFORMATION(("mdi_audio_vrsi_rcg_prompt_req_hdlr, state=%d\n",mdi_audio_state));
   
   switch(msg_p->pmp_mode)
   {
      case MED_VRSI_CMD_RCG_PROMPT:                            
         vrsiHandler(MED_VRSI_RECOG_MPR);  
         break;
      case MED_VRSI_DGT_RCG_PROMPT: 
         vrsiHandler(MED_VRSI_DIGIT_RECOG_MPR);
         break;         
   }                
  
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_vrsi_rcg_result_ind
* DESCRIPTION
*    This function is to inform MMI recognition result.
* PARAMETERS
*    void* inMsg
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_vrsi_rcg_result_ind(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   media_vrsi_rcg_result_ind_struct* msg_p=(media_vrsi_rcg_result_ind_struct*)inMsg;

   //PRINT_INFORMATION(("mdi_audio_vrsi_rcg_result_ind, state=%d\n",mdi_audio_state));
   
   if(msg_p->res_type == 0)                     
      vrsiHandler(MED_VRSI_DIGIT_RECOG_OK);            
   else 
      vrsiHandler(MED_VRSI_RECOG_OK);    

#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_vrsi_err_ind
* DESCRIPTION
*    This function is the error indication.
* PARAMETERS
*    void* inMsg
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_vrsi_err_ind(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   //media_vrsi_err_ind_struct* msg_p=(media_vrsi_err_ind_struct*)inMsg;

   //PRINT_INFORMATION(("mdi_audio_vrsi_play_tag_finish_ind, state=%d\n",mdi_audio_state));
               
   vrsiHandler(MED_VRSI_ERROR);        

#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_vrsi_init
* DESCRIPTION
*    This function is to init vrsi engine.
* PARAMETERS
*    mdi_callback handler
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_vrsi_init(mdi_callback handler) 
{
   mdi_result  result;
   
   //PRINT_INFORMATION(("mdi_audio_vrsi_init"));

   if((result = media_aud_vrsi_init(MOD_MMI))==MED_RES_OK)
   {
      vrsiHandler = handler;
      SetProtocolEventHandler(mdi_audio_vrsi_trn_prompt_req_hdlr, MSG_ID_MEDIA_VRSI_TRN_PROMPT_REQ);
      SetProtocolEventHandler(mdi_audio_vrsi_trn_result_ind, MSG_ID_MEDIA_VRSI_TRN_RESULT_IND);
      SetProtocolEventHandler(mdi_audio_vrsi_rcg_prompt_req_hdlr, MSG_ID_MEDIA_VRSI_RCG_PROMPT_REQ);
      SetProtocolEventHandler(mdi_audio_vrsi_rcg_result_ind, MSG_ID_MEDIA_VRSI_RCG_RESULT_IND);
      SetProtocolEventHandler(mdi_audio_vrsi_err_ind, MSG_ID_MEDIA_VRSI_ERR_IND);      
      return MDI_AUDIO_SUCCESS;
   }
   else 
   {
      mdi_audio_state=MDI_AUDIO_IDLE;
      mdi_start_background_timer();
      return mdi_get_event((U8)result);      
   }
}

/******************************************************************
* FUNCTION
*    mdi_audio_vrsi_new_session_id
* DESCRIPTION
*    This function is to get a new session id.
* PARAMETERS
*    void
* RETURNS
*    U32
* GLOBALS AFFECTED
*  
******************************************************************/
U32 mdi_audio_vrsi_new_session_id(void)
{
   return media_aud_vrsi_new_session_id();   
}

/******************************************************************
* FUNCTION
*    mdi_audio_vrsi_stop
* DESCRIPTION
*    This function is to stop a vrsi action.
* PARAMETERS
*    U32 session_id
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_vrsi_stop(U32 session_id)
{
   //PRINT_INFORMATION(("mdi_audio_vrsi_stop, session_id=%d, state=%d\n",session_id,mdi_audio_state));
   media_aud_vrsi_abort( MOD_MMI, session_id );
   if( mdi_audio_state == MDI_AUDIO_VRSI_PROCESS )
      mdi_audio_state = MDI_AUDIO_IDLE;
}

/******************************************************************
* FUNCTION
*    mdi_audio_vrsi_close
* DESCRIPTION
*    This function is to close vrsi engine.
* PARAMETERS
*    void
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_vrsi_close(void)
{
   //PRINT_INFORMATION(("mdi_audio_vrsi_close, state=%d\n",mdi_audio_state));
   media_aud_vrsi_close(MOD_MMI);
   if( mdi_audio_state == MDI_AUDIO_VRSI_PROCESS )
      mdi_audio_state = MDI_AUDIO_IDLE;
}

/******************************************************************
* FUNCTION
*    mdi_audio_vrsi_play_tag_finish_ind
* DESCRIPTION
*    This function is the play tag finish indication.
* PARAMETERS
*    void* inMsg
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_vrsi_play_tag_finish_ind(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   media_vrsi_play_tag_finish_ind_struct* msg_p=(media_vrsi_play_tag_finish_ind_struct*)inMsg;

   //PRINT_INFORMATION(("mdi_audio_vrsi_play_tag_finish_ind, state=%d\n",mdi_audio_state));
   if((mdi_audio_state==MDI_AUDIO_VRSI_PROCESS && mdi_audio_mode==MDI_AUDIO_VRSI_PLAY_TAG)
      && msg_p->session_id==identifier)
   {
      mdi_audio_state=MDI_AUDIO_IDLE;               
      vrsiHandler(MED_VRSI_PLAY_TAG_OK);      
      mdi_start_background_timer();
   }
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_vrsi_play_tag
* DESCRIPTION
*    This function is to play vrsi pre-record tag.
* PARAMETERS
*   U32 session_id,
*	U8 *grammar, 
*	U16 tag_id
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_vrsi_play_tag(U32 session_id, U8 *grammar, U16 tag_id, U8 volume, U8 output_path)
{   
   mdi_result  result;
   
   //PRINT_INFORMATION(("mdi_audio_vrsi_play_tag, session_id=%d, tag_id=%d, state=%d\n",session_id,tag_id,mdi_audio_state));

   if( !mdi_audio_check_states_and_handlers( &result ) )
      return result;

   identifier = session_id;
   mdi_stop_background_timer();
   
#ifdef MMI_ON_HARDWARE_P
   if((result = media_aud_vrsi_play_tag( MOD_MMI,  session_id, grammar, tag_id, volume ,output_path))==MED_RES_OK)
   {      
      mdi_audio_state=MDI_AUDIO_VRSI_PROCESS;
      mdi_audio_mode=MDI_AUDIO_VRSI_PLAY_TAG;
      SetProtocolEventHandler(mdi_audio_vrsi_play_tag_finish_ind, MSG_ID_MEDIA_VRSI_PLAY_TAG_FINISH_IND);
      return MDI_AUDIO_SUCCESS;
   }
   else
   {
      mdi_audio_state=MDI_AUDIO_IDLE;
      mdi_start_background_timer();
      return mdi_get_event((U8)result);
   }
#else   
   mdi_audio_state=MDI_AUDIO_VRSI_PROCESS;
   mdi_audio_mode=MDI_AUDIO_VRSI_PLAY_TAG;
#endif
      
}

/******************************************************************
* FUNCTION
*    mdi_audio_vrsi_play_tts_finish_ind
* DESCRIPTION
*    This function is the play TTS finish indication.
* PARAMETERS
*    void* inMsg
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_vrsi_play_tts_finish_ind(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   media_vrsi_play_tts_finish_ind_struct* msg_p=(media_vrsi_play_tts_finish_ind_struct*)inMsg;

   //PRINT_INFORMATION(("mdi_audio_vrsi_play_tag_finish_ind, state=%d\n",mdi_audio_state));
   if((mdi_audio_state==MDI_AUDIO_VRSI_PROCESS && mdi_audio_mode==MDI_AUDIO_VRSI_PLAY_TTS)
      && msg_p->session_id==identifier)
   {
      mdi_audio_state=MDI_AUDIO_IDLE;               
      vrsiHandler(MED_VRSI_TTS_OK);      
      mdi_start_background_timer();
   }
#endif
}

/******************************************************************
* FUNCTION
*    mdi_audio_vrsi_play_TTS
* DESCRIPTION
*    This function is to play vrsi TTS.
* PARAMETERS
*   U32 session_id, 
*	U16 *text, 
*	U8 lang
* RETURNS
*    mdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
mdi_result mdi_audio_vrsi_play_TTS(U32 session_id, U16 *text, U8 volume, U8 output_path, U8 lang) 
{
   mdi_result  result;
   
   //PRINT_INFORMATION(("mdi_audio_vrsi_play_TTS, session_id=%d, state=%d\n",session_id,mdi_audio_state));

   if( !mdi_audio_check_states_and_handlers( &result ) )
      return result;

   identifier = session_id;
   mdi_stop_background_timer();
   
#ifdef MMI_ON_HARDWARE_P
   if((result = media_aud_vrsi_play_TTS( MOD_MMI,  session_id, text,volume, output_path, lang))==MED_RES_OK)
   {      
      mdi_audio_state=MDI_AUDIO_VRSI_PROCESS;
      mdi_audio_mode=MDI_AUDIO_VRSI_PLAY_TTS;
      SetProtocolEventHandler(mdi_audio_vrsi_play_tts_finish_ind, MSG_ID_MEDIA_VRSI_PLAY_TTS_FINISH_IND);
      return MDI_AUDIO_SUCCESS;
   }
   else
   {
      mdi_audio_state=MDI_AUDIO_IDLE;
      mdi_start_background_timer();
      return mdi_get_event((U8)result);
   }
#else   
   mdi_audio_state=MDI_AUDIO_VRSI_PROCESS;
   mdi_audio_mode=MDI_AUDIO_VRSI_PLAY_TTS;
#endif
     
}

/******************************************************************
* FUNCTION
*    mdi_audio_vrsi_register_record
* DESCRIPTION
*    This function is to change vrsi state to record state.
* PARAMETERS
*    void
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_vrsi_register_record(void) 
{
   //PRINT_INFORMATION(("mdi_audio_vrsi_register_record, state=%d\n",mdi_audio_state));
   mdi_audio_state = MDI_AUDIO_VRSI_PROCESS;
   mdi_audio_mode = MDI_AUDIO_VRSI_RECORD;      
}

/******************************************************************
* FUNCTION
*    mdi_audio_vrsi_unregister_record
* DESCRIPTION
*    This function is to change vrsi state to idle state..
* PARAMETERS
*    void
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void mdi_audio_vrsi_unregister_record(void)
{
   //PRINT_INFORMATION(("mdi_audio_vrsi_unregister_record, state=%d\n",mdi_audio_state));
   mdi_audio_state = MDI_AUDIO_IDLE;
   mdi_audio_mode = MDI_AUDIO_VRSI_RECORD; 
}



#endif //  VRSI_ENABLE

enum{
   JDI_AUDIO_PLAY_KT,
   JDI_AUDIO_PLAY_TONE,
   JDI_AUDIO_PLAY_ID,
   JDI_AUDIO_PLAY_STRING,
   JDI_AUDIO_PLAY_FILE,
   JDI_AUDIO_RECORD_FILE,
   JDI_AUDIO_PLAY_FM_RADIO,
   JDI_AUDIO_RECORD_VR_VOICE,   
   JDI_AUDIO_VRSI_PLAY_TTS,
   JDI_AUDIO_VRSI_PLAY_TAG, 
   JDI_AUDIO_VRSI_RECORD,  
   JDI_NO_AUDIO_MODE  
};

static jdi_callback jdi_current_handler;
static jdi_callback jdi_previous_handler;

static U16 jdi_audio_state;
static U16 jdi_audio_mode;

/******************************************************************
* FUNCTION
*    jdi_audio_init
* DESCRIPTION
*    This function is to initialize the mdi audio interface.
* PARAMETERS
*    None
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
void jdi_audio_init(void)
{
   jdi_audio_state=JDI_AUDIO_IDLE;
}

/******************************************************************
* FUNCTION
*    jdi_get_event
* DESCRIPTION
*    This function is to convert event to jdi result.
* PARAMETERS
*    U8 result
* RETURNS
*    jdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
jdi_result jdi_get_event(U8 result)
{
   return (jdi_result)result;
}

/******************************************************************
* FUNCTION
*    jdi_audio_play_file
* DESCRIPTION
*    This function is to play audio file, which will be executed in Java context
* PARAMETERS
*    void* file_name, jdi_callback handler
* RETURNS
*    jdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
jdi_result jdi_audio_play_file(void* file_name, int java_player, jdi_callback handler)
{
#ifdef MMI_ON_HARDWARE_P
   aud_play_file_struct file_param;
   jdi_result  result;
#endif

//   //PRINT_INFORMATION(("mdi_audio_play_file, state=%d\n",mdi_audio_state));

//   ++identifier;
      
#ifdef MMI_ON_HARDWARE_P
   file_param.file_name_p = (UI_string_type)file_name;
   file_param.play_style = DEVICE_AUDIO_PLAY_ONCE; //play_style;
   file_param.volume=GetRingVolumeLevel();
   file_param.output_path=MDI_DEVICE_SPEAKER2; // <----------------------
   file_param.identifier= (U16) java_player; //identifier;
// set both start_offset and end_offset to 0 for backward compatible
   file_param.start_offset = file_param.end_offset = 0;
   if((result=media_aud_play_file(MOD_JASYN, &file_param))==MED_RES_OK)
   {
      jdi_current_handler=handler;
      jdi_audio_state=JDI_AUDIO_PLAY;
      jdi_audio_mode=JDI_AUDIO_PLAY_FILE;
//      SetProtocolEventHandler(mdi_audio_play_finish_ind1, MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND);
/* Handler for message MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND in j2me module is ....() */ // <----------------------
      return JDI_AUDIO_SUCCESS;
   }
   else
   {
      jdi_current_handler=NULL;
      jdi_audio_state=MDI_AUDIO_IDLE;
      return jdi_get_event((U8)result);
   }
#else
   jdi_current_handler=handler;
   jdi_audio_state=JDI_AUDIO_PLAY;
   jdi_audio_mode=JDI_AUDIO_PLAY_FILE;
   return JDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    jdi_audio_stop_file
* DESCRIPTION
*    This function is to stop audio file in Java context.
* PARAMETERS
*    None
* RETURNS
*    jdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
jdi_result jdi_audio_stop_file(void)
{
//   //PRINT_INFORMATION(("mdi_audio_stop_file, state=%d, mode=%d\n",mdi_audio_state, mdi_audio_mode));
//   if (blocked)
//      return JDI_AUDIO_BLOCKED;

#ifdef MMI_ON_HARDWARE_P
//   SetProtocolEventHandler(NULL, MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND);
/* Handler for message MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND in j2me module is ....() */  // <--------------------
   media_aud_stop_file(MOD_JASYN, SRC_LMMI);
#endif

   if (jdi_current_handler)
   {
      jdi_previous_handler=jdi_current_handler;
      jdi_current_handler=NULL;
      jdi_previous_handler(JDI_AUDIO_TERMINATED);

   }
   jdi_audio_state=JDI_AUDIO_IDLE;

   return JDI_AUDIO_SUCCESS;
}

/******************************************************************
* FUNCTION
*    jdi_audio_pause
* DESCRIPTION
*    This function is to pause audio play/record in Java context.
* PARAMETERS
*    jdi_callback handler
* RETURNS
*    jdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
jdi_result jdi_audio_pause(jdi_callback handler)
{
#ifdef MMI_ON_HARDWARE_P
   jdi_result  result;
#endif

//   //PRINT_INFORMATION(("mdi_audio_pause, state=%d\n",mdi_audio_state));
//   if (blocked)
//      return JDI_AUDIO_BLOCKED;

#ifdef MMI_ON_HARDWARE_P
   if((result=media_aud_pause(MOD_JASYN, SRC_LMMI))==MED_RES_OK)
   {
      if(jdi_audio_state==JDI_AUDIO_PLAY)
         jdi_audio_state=JDI_AUDIO_PLAY_PAUSED;
      else
         jdi_audio_state=JDI_AUDIO_RECORD_PAUSED;
      jdi_current_handler=handler;
      return JDI_AUDIO_SUCCESS;
   }
   else if(result == MED_RES_UNSUPPORTED_OPERATION)
   	  return JDI_AUDIO_UNSUPPORTED_OPERATION;
   else
   {
      jdi_current_handler=NULL;
      jdi_audio_state=JDI_AUDIO_IDLE;

      return jdi_get_event((U8)result);
   }
#else
   if(jdi_audio_state==JDI_AUDIO_PLAY)
      jdi_audio_state=JDI_AUDIO_PLAY_PAUSED;
   else
      jdi_audio_state=JDI_AUDIO_RECORD_PAUSED;
   jdi_current_handler=handler;
   return JDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    jdi_audio_pause
* DESCRIPTION
*    This function is to resume audio play/record in Java context.
* PARAMETERS
*    jdi_callback handler
* RETURNS
*    jdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
jdi_result jdi_audio_resume(jdi_callback handler)
{
#ifdef MMI_ON_HARDWARE_P
   jdi_result  result;
#endif

//   //PRINT_INFORMATION(("mdi_audio_resume, state=%d\n",mdi_audio_state));
//   if (blocked)
//      return JDI_AUDIO_BLOCKED;

#ifdef MMI_ON_HARDWARE_P
   if((result=media_aud_resume(MOD_JASYN, SRC_LMMI))==MED_RES_OK)
   {
      if(jdi_audio_state==JDI_AUDIO_PLAY_PAUSED)
         jdi_audio_state=JDI_AUDIO_PLAY;
      else
         jdi_audio_state=JDI_AUDIO_RECORD;
      jdi_current_handler=handler;
      return JDI_AUDIO_SUCCESS;
   }
   else
   {
      jdi_current_handler=NULL;
      jdi_audio_state=JDI_AUDIO_IDLE;

      return jdi_get_event((U8)result);
   }
#else
   if(jdi_audio_state==JDI_AUDIO_PLAY_PAUSED)
      jdi_audio_state=JDI_AUDIO_PLAY;
   else
      jdi_audio_state=JDI_AUDIO_RECORD;
   jdi_current_handler=handler;
   return JDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    jdi_audio_start_record
* DESCRIPTION
*    This function is to start audio record.
* PARAMETERS
*    void* file_name, U8 format, mdi_handle* handle_p, mdi_callback handler
* RETURNS
*    jdi_handle
* GLOBALS AFFECTED
*  
******************************************************************/
jdi_result jdi_audio_start_record(void* file_name, U8 format, jdi_handle* handle_p, jdi_callback handler)
{
#ifdef MMI_ON_HARDWARE_P
   mdi_result result;
#endif

//   //PRINT_INFORMATION(("mdi_audio_start_record, state=%d\n",mdi_audio_state));

//   if( blocked )
//   	return MDI_AUDIO_BLOCKED;
/*   else*/ if( jdi_audio_state == JDI_AUDIO_RECORD || jdi_audio_state == JDI_AUDIO_RECORD_PAUSED )
	return MDI_AUDIO_BUSY;
   else if( jdi_current_handler && (jdi_audio_state == JDI_AUDIO_PLAY || jdi_audio_state == JDI_AUDIO_PLAY_PAUSED
   ) )
   {
      jdi_current_handler( JDI_AUDIO_TERMINATED );
//      if( jdi_audio_state == JDI_AUDIO_PLAY && jdi_audio_mode == JDI_AUDIO_PLAY_FM_RADIO )
//         mdi_fmr_power_off();
   }

#ifdef MMI_ON_HARDWARE_P
   if((result=media_aud_start_record(MOD_JASYN, (kal_wchar *) file_name, format, 7,TRUE, 0))==MED_RES_OK)
   {
      jdi_current_handler=handler;
      jdi_audio_state=JDI_AUDIO_RECORD;
      jdi_audio_mode=JDI_AUDIO_RECORD_FILE;
//      SetProtocolEventHandler(mdi_audio_record_finish_ind, MSG_ID_L4AUD_MEDIA_RECORD_FINISH_IND); <-----
      return JDI_AUDIO_SUCCESS;
   }
   else
   {
      jdi_current_handler=NULL;
      jdi_audio_state=JDI_AUDIO_IDLE;
//      mdi_start_background_timer();
      return jdi_get_event((U8)result);
   }
#else
   jdi_current_handler=handler;
   jdi_audio_state=JDI_AUDIO_RECORD;
   jdi_audio_mode=JDI_AUDIO_RECORD_FILE;
   return JDI_AUDIO_SUCCESS;
#endif
}

/******************************************************************
* FUNCTION
*    jdi_audio_record_finish_ind
* DESCRIPTION
*    This function is to handle audio record finish indication.
* PARAMETERS
*    void* inMsg
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
void jdi_audio_record_finish_ind(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   l4aud_media_record_finish_ind_struct* msg_p=(l4aud_media_record_finish_ind_struct*)inMsg;

//   //PRINT_INFORMATION(("mdi_audio_record_finish_ind, state=%d\n",mdi_audio_state));
   if(jdi_audio_state==JDI_AUDIO_RECORD)
   {
      jdi_audio_state=JDI_AUDIO_IDLE;
      if (jdi_current_handler)
      {
         jdi_previous_handler=jdi_current_handler;
         jdi_current_handler=NULL;
         jdi_previous_handler(jdi_get_event(msg_p->result));
      }
//      mdi_start_background_timer();
   }   
#endif
}

/******************************************************************
* FUNCTION
*    jdi_audio_stop_record
* DESCRIPTION
*    This function is to stop audio record.
* PARAMETERS
*    void* inMsg
* RETURNS
*    jdi_result
* GLOBALS AFFECTED
*  
******************************************************************/
jdi_result jdi_audio_stop_record(void)
{
//   //PRINT_INFORMATION(("mdi_audio_stop_record, state=%d\n",mdi_audio_state));
//   if (blocked)
//      return MDI_AUDIO_BLOCKED;

#ifdef MMI_ON_HARDWARE_P
//   SetProtocolEventHandler(NULL, MSG_ID_L4AUD_MEDIA_RECORD_FINISH_IND);
   media_aud_stop_record(MOD_JASYN, SRC_LMMI);
#endif

   jdi_audio_state=JDI_AUDIO_IDLE; 
   if (jdi_current_handler)
   {
      jdi_previous_handler=jdi_current_handler;
      jdi_current_handler=NULL;
      jdi_previous_handler(JDI_AUDIO_TERMINATED);
   }

//   mdi_start_background_timer();

   return JDI_AUDIO_SUCCESS;
}

#ifdef MAGIC_AE_SUPPORT
//audio effect.sheen
void mdi_audio_ae_process(UINT32  *pHandle,  INT16 *pData, INT32 dataLen, void (*ae_callback)(void))
{
	   media_aud_ae_req_struct* msg_p;
	      
	   msg_p = OslConstructDataPtr(sizeof(media_aud_ae_req_struct));
	   msg_p->aeHandle=(kal_uint32*)pHandle;
	   msg_p->pData=pData;
	   msg_p->dataLen=dataLen;
	   msg_p->ae_callback=ae_callback;
	   mdi_send_msg(MSG_ID_MEDIA_AUD_AE_REQ, msg_p, NULL);
}
#endif

BOOL mdi_audio_is_paused(void)
{
return mdi_audio_state == MDI_AUDIO_PLAY_PAUSED;
}
#ifdef __GENERAL_TTS__
/*****************************************************************************
 * FUNCTION
 *  mdi_audio_tts_set_attr
 * DESCRIPTION
 *  This function is to set tts play attribute. 
 *  Note: This function need to call immediately before each play request.
 * PARAMETERS
 *  attr_id       [IN]  attribute id
 *  attr_value    [IN]  attribute value
 * RETURNS
 *  mdi_result
 *****************************************************************************/
mdi_result mdi_audio_tts_set_attr(U32 attr_id, U32 attr_value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mdi_result result;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION(("mdi_audio_tts_set_attr, state=[%d], id=[%d], value=[%d]\n", mdi_audio_state, attr_id, attr_value));

    result = media_aud_tts_set_attr_req(MOD_MMI, attr_id, attr_value);

    return result;
}


/*****************************************************************************
 * FUNCTION
 *  mdi_audio_tts_play_update_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  attr_id       [IN]  attribute id
 *  attr_value    [IN]  attribute value
 * RETURNS
 *  mdi_result
 *****************************************************************************/
void mdi_audio_tts_play_update_ind(void *inMsg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_aud_tts_update_info_ind_struct *msg_p = (media_aud_tts_update_info_ind_struct*) inMsg;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION(("mdi_audio_tts_play_update_ind"));
    
	//    free_ctrl_buffer(msg_p->data);
}


/*****************************************************************************
 * FUNCTION
 *  mdi_audio_tts_play
 * DESCRIPTION
 *  This function is to request TTS string.
 * PARAMETERS
 *  tts_param       [IN]
 * RETURNS
 *  mdi_result
 *****************************************************************************/
mdi_result mdi_audio_tts_play(mdi_audio_tts_struct tts_param)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    aud_tts_struct aud_tts;
    mdi_result result;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION(("mdi_audio_tts_play, state=%d\n", mdi_audio_state));

    if (!mdi_audio_check_states_and_handlers(&result))
    {
        return result;
    }
mdi_stop_background_timer();
///    mdi_audio_stop_background_timer();
    ++identifier;

    /* All sound shall go through bluetooth earphone */
#ifdef __MMI_BT_AUDIO_VIA_SCO__
    mmi_profiles_bt_connect_audio_via_sco();
#endif

    aud_tts.text_string = tts_param.text_string;
    aud_tts.str_type = tts_param.str_type;
    aud_tts.lang = tts_param.lang;
    aud_tts.app_type = tts_param.app_type;
    aud_tts.gander = tts_param.gander;
    aud_tts.volume = tts_param.volume;
    aud_tts.path = tts_param.path;
    aud_tts.pitch = tts_param.pitch;
    aud_tts.speed = tts_param.speed;
    aud_tts.identifier = identifier;
    
    if ((result = media_aud_tts_play_req(MOD_MMI, &aud_tts)) == MED_RES_OK)
    {
        currentHandler = tts_param.callback;
        mdi_audio_state = MDI_AUDIO_PLAY;
        mdi_audio_mode = MDI_AUDIO_PLAY_TTS;
        SetProtocolEventHandler(mdi_audio_play_finish_ind1, MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND);
        SetProtocolEventHandler(mdi_audio_tts_play_update_ind, MSG_ID_MEDIA_AUD_TTS_UPDATE_INFO_IND);
        return MDI_AUDIO_SUCCESS;
    }
    else
    {
        currentHandler = NULL;
        mdi_audio_state = MDI_AUDIO_IDLE;
      mdi_start_background_timer();
//        mdi_audio_start_background_timer();
        return mdi_get_event((U8) result);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mdi_audio_tts_stop
 * DESCRIPTION
 *  This function is to stop playing TTS string.
 * PARAMETERS
 *  void
 * RETURNS
 *  mdi_result
 *****************************************************************************/
mdi_result mdi_audio_tts_stop(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION(("mdi_audio_tts_stop, state=%d, mode=%d\n", mdi_audio_state, mdi_audio_mode));
    if (!mdi_check_device_ownership())
    {
        return MDI_AUDIO_BLOCKED;
    }

    /*
     * set pseudo play file callback to NULL before callback
     * bcoz we could set another callback during callback
    if (pseudo_play_file_callback)
    {
        mdi_callback old_pseudo_play_file_callback = pseudo_play_file_callback;
        pseudo_play_file_callback = NULL;
        old_pseudo_play_file_callback(MDI_AUDIO_TERMINATED);
    }
     */

    if (!((mdi_audio_state == MDI_AUDIO_PLAY || mdi_audio_state == MDI_AUDIO_PLAY_PAUSED)
          && mdi_audio_mode == MDI_AUDIO_PLAY_TTS))
    {
        return MDI_AUDIO_SUCCESS;
    }

    SetProtocolEventHandler(NULL, MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND);
    media_aud_tts_stop_req(MOD_MMI);

    if (currentHandler)
    {
        previousHandler = currentHandler;
        currentHandler = NULL;
        previousHandler(MDI_AUDIO_TERMINATED);
    }

    mdi_audio_state = MDI_AUDIO_IDLE;
 //   mdi_audio_start_background_timer();
      mdi_start_background_timer();

    return MDI_AUDIO_SUCCESS;
}


/*****************************************************************************
 * FUNCTION
 *  mdi_audio_tts_pause
 * DESCRIPTION
 *  This function is to pause playing TTS string.
 * PARAMETERS
 *  void
 * RETURNS
 *  mdi_result
 *****************************************************************************/
mdi_result mdi_audio_tts_pause(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mdi_result result;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION(("mdi_audio_tts_pause, state=%d\n", mdi_audio_state));
    if (!mdi_check_device_ownership())
    {
        return MDI_AUDIO_BLOCKED;
    }

    if ((result = media_aud_tts_pause_req(MOD_MMI)) == MED_RES_OK)
    {
        if (mdi_audio_state == MDI_AUDIO_PLAY)
        {
            mdi_audio_state = MDI_AUDIO_PLAY_PAUSED;
        }
        return MDI_AUDIO_SUCCESS;
    }
    else
    {
        currentHandler = NULL;
        mdi_audio_state = MDI_AUDIO_IDLE;
           mdi_start_background_timer();
//      mdi_start_background_timer();
      mdi_start_background_timer();
//  mdi_audio_start_background_timer();
        return mdi_get_event((U8) result);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mdi_audio_tts_resume
 * DESCRIPTION
 *  This function is to resume playing TTS string.
 * PARAMETERS
 *  void
 * RETURNS
 *  mdi_result
 *****************************************************************************/
mdi_result mdi_audio_tts_resume(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mdi_result result;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION(("mdi_audio_tts_resume, state=%d\n", mdi_audio_state));
    if (!mdi_check_device_ownership())
    {
        return MDI_AUDIO_BLOCKED;
    }

    if ((result = media_aud_tts_resume_req(MOD_MMI)) == MED_RES_OK)
    {
        if (mdi_audio_state == MDI_AUDIO_PLAY_PAUSED)
        {
            mdi_audio_state = MDI_AUDIO_PLAY;
        }
        return MDI_AUDIO_SUCCESS;
    }
    else
    {
        currentHandler = NULL;
        mdi_audio_state = MDI_AUDIO_IDLE;
          mdi_start_background_timer();
//  mdi_audio_start_background_timer();
        return mdi_get_event((U8) result);
    }
}
#endif /*__GENERAL_TTS__*/
