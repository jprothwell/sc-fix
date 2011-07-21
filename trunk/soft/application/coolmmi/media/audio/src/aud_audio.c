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
 *	aud_audio.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes audio functions of audio manager task.
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
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/******************************************************************
 * MODULE
 *    AUD_AUDIO
 * DESCRIPTION
 *    This module defines the audio related function for Audio Manager.
 *  
 ******************************************************************/


#ifndef MED_NOT_PRESENT


/*==== INCLUDES =======*/

#include "kal_release.h" 
#include "stack_common.h"  
#include "stack_msgs.h"
#include "app_ltlcom.h"       	/* task message communiction */
#include "stacklib.h"        	/* basic type for dll, evshed, stacktimer */
#include "event_shed.h"       	/* event scheduler */
#include "stack_timer.h"      	/* stack timer */

/* global includes */
#include "l1audio.h"
#include "device.h"
#include "resource_audio.h"
#include "custom_equipment.h"
#include "nvram_struct.h"
#include "nvram_user_defs.h"
#include "nvram_data_items.h"
#include "custom_nvram_editor_data_item.h"

/* local includes */
#include "med_global.h"
//#include "med_status.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "med_main.h"
#include "aud_main.h"
#include "med_utility.h"


#include "nvram_data_items.h"
#include "nvram_struct.h"
#include "device.h"
#include "mmi_trace.h"
#include "keybrd.h"
#ifdef __MED_CAM_MOD__
#include "cam_main.h"
#endif
#if defined (__MED_VID_MOD__) || defined (__MED_MJPG_MOD__)
#include "vid_main.h"
#endif

#include "media_others.h"
#include "oslmemory.h"
#include "mmi_trace.h"
//jiashuo add for jessamine start,20080504
#include "gui_data_types.h"
#include "fmradiodef.h"
#include "fmradiotype.h"
#include "frameworkstruct.h"
#include "audioplayertype.h"
//jiashuo add for jessamine end
#ifdef MAGIC_AE_SUPPORT
#include "jtae.h"
#endif

/*==== VARIABLES =======*/
extern kal_uint8 current_playing_melody_volume_level; // defined in aud_melody.c
//extern audply_struct g_audply;

extern void SetAudioVolumeReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI);
extern void PlayKeyPadToneReq(l4aud_audio_play_req_struct *pAudioNode);
extern void PlaySpecialToneReq(l4aud_audio_play_req_struct *pAudioNode);
extern void PlayRingToneReq(l4aud_audio_play_req_struct *pAudioNode);
extern pBOOL isInCall(void);
extern BOOL mmi_vdoply_is_in_playing();
extern BOOL mmi_audply_is_playing( void );

/******************************************************************
 * FUNCTION
 *    aud_audio_init
 * DESCRIPTION
 *    This function is used to init audio module for audio manager.
 * PARAMETERS
 *    
 *    
 * RETURNS
 *
 * GLOBALS AFFECTED
 *  
 *  
 ******************************************************************/

kal_bool aud_audio_init(void)
{ 
   aud_context_p->audio_mode = AUD_MODE_NORMAL;
   aud_context_p->identifier = 0;
   aud_context_p->melody_output_device = AUDIO_DEVICE_SPEAKER2;
   custom_cfg_audio_ec_range(&aud_context_p->min_ec_level, 
                                    &aud_context_p->max_ec_level);
   aud_context_p->audio_mute=KAL_FALSE;
   return KAL_TRUE;
}

kal_uint8 aud_get_volume_gain(kal_uint8 mode, kal_uint8 type, kal_uint8 level)
{
	if ( (mode>=MAX_VOL_CATE) || (type>=MAX_VOL_TYPE) || (level>=MAX_VOL_LEVEL))
		return 0;
	return aud_context_p->acoustic_data.volume_gain[mode][type][level];
}

kal_uint8 aud_get_volume_level(kal_uint8 mode, kal_uint8 type)
{
	if ( (mode>=MAX_VOL_CATE) || (type>=MAX_VOL_TYPE) )
		return 0;
	return aud_context_p->acoustic_data.volume[mode][type];
}

void aud_set_aud_path_volume( kal_uint8 audio_mode )
{
   audio_param_struct *param_p;
   kal_uint8 volume;
   kal_uint8 gain_audio_mode;
   /* To slove mute flag cleared by speech off */
   kal_bool mute;

   param_p = &aud_context_p->audio_param;

   /* for all audio mode */
   Media_SetFilter(MEDIA_FIR_SPEECH_OUTPUT1_16K, 62, param_p->Speech_FIR_Output_Coeff_16k_Tbl1);
   Media_SetFilter(MEDIA_FIR_SPEECH_INPUT1_16K, 62, param_p->Speech_FIR_Input_Coeff_16k_Tbl1);
   Media_SetFilter(MEDIA_FIR_SPEECH_OUTPUT2_16K, 62, param_p->Speech_FIR_Output_Coeff_16k_Tbl2);
   Media_SetFilter(MEDIA_FIR_SPEECH_INPUT2_16K, 62, param_p->Speech_FIR_Input_Coeff_16k_Tbl2);
   Media_SetFilter(MEDIA_FIR_MELODY_OUTPUT1_32K, 45, param_p->Melody_FIR_Output_Coeff_32k_Tbl1);
   Media_SetFilter(MEDIA_FIR_MELODY_OUTPUT2_32K, 45, param_p->Melody_FIR_Output_Coeff_32k_Tbl2);
   
   switch(audio_mode)
   {
      case AUD_MODE_NORMAL:
      {
         kal_uint8 level;

         /* Set output device */
         aud_speech_set_output_device(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_SPEAKER));
         aud_keytone_set_output_device(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_SPEAKER));
         aud_tone_set_output_device(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_SPEAKER));
         aud_melody_set_output_device(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_LOUDSPEAKER));
         aud_fmr_set_output_device(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_LOUDSPEAKER));
         
         /*For FM record, not use default input device during record phase, so don't apply input path right away*/
         if((AUD_IN_STATE(AUD_MEDIA_RECORD) || AUD_IN_STATE(AUD_MEDIA_RECORD_PAUSED)) 
              && !aud_context_p->use_default_input)
         {
            aud_mic_set_device_no_apply(custom_cfg_hw_aud_input_path(AUDIO_DEVICE_MIC1));
         }
         else
         {
            aud_mic_set_device(custom_cfg_hw_aud_input_path(AUDIO_DEVICE_MIC1));
         }
         
         /* set speech volume */
         volume = aud_get_volume_gain(VOL_NORMAL, AUD_VOLUME_SPH, aud_get_volume_level(VOL_NORMAL, AUD_VOLUME_SPH));
         aud_speech_set_output_volume(volume, volume);
         /* set general tone volume, use ring tone volume level */
         volume = aud_get_volume_gain(VOL_NORMAL, AUD_VOLUME_CTN, aud_get_volume_level(VOL_NORMAL, AUD_VOLUME_MEDIA));
         aud_tone_set_output_volume(volume, volume);
         /* set key tone volume */
         volume = aud_get_volume_gain(VOL_NORMAL, AUD_VOLUME_KEY, aud_get_volume_level(VOL_NORMAL, AUD_VOLUME_KEY));
         aud_keytone_set_output_volume(volume, volume);
         /* set loopback volume */
         volume = aud_get_volume_gain(VOL_NORMAL, AUD_VOLUME_SID, aud_get_volume_level(VOL_NORMAL, AUD_VOLUME_SID));
         aud_side_tone_set_volume(volume, volume);
         /* set fm radio volume */
         volume = aud_get_volume_gain(VOL_NORMAL, AUD_VOLUME_FMR, aud_get_volume_level(VOL_NORMAL, AUD_VOLUME_FMR));
         aud_fmr_set_volume(volume);
         /* set media volume, use playing melody volume level */
         volume = aud_get_volume_gain(VOL_NORMAL, AUD_VOLUME_MEDIA, current_playing_melody_volume_level);
         aud_melody_set_output_volume(volume, volume);
         
         /* Set speech FIR coefficient */
         L1SP_Write_Audio_Coefficients(param_p->FIR_input_coeff[0], param_p->FIR_output_coeff[param_p->selected_FIR_output_index]);
         aud_speech_set_fir(KAL_TRUE);

         level=aud_get_volume_level(VOL_NORMAL, AUD_VOLUME_SPH);

         volume = aud_get_volume_gain(VOL_NORMAL, AUD_VOLUME_MIC, aud_get_volume_level(VOL_NORMAL, AUD_VOLUME_MIC));
#if !defined( MT6205B )
#ifdef AEC_ENABLE
         aud_mic_set_volume(volume, volume);
         
         if (level>=aud_context_p->min_ec_level && level<=aud_context_p->max_ec_level)
         {
            L1SP_NormalMode_SetAECParameter(param_p->es_time_const2, param_p->es_vol_const2);
            L1SP_NormalMode_SetAEC(KAL_TRUE);
         }
         else
            L1SP_NormalMode_SetAEC(KAL_FALSE);
#endif // AEC_ENABLE
#ifdef EES_ENABLE
         L1SP_LoudSpkMode_SetEES(KAL_FALSE);
#endif // EES_ENABLE
#endif  
#if defined(MT6205B) || (!defined(MT6205B) && !defined(AEC_ENABLE))  /* only echo suppression */
         if (level==MAX_VOL_LEVEL-1)
         {
            /* if volume is at max level in normal mode, mic gain -16 */
            ASSERT( volume >= 16 );
            volume -= 16;
            aud_mic_set_volume(volume, volume);
            
            /* Turn on echo suppression */
            L1SP_SetEchoSuppressionParameter(param_p->es_time_const2, param_p->es_vol_const2);
            L1SP_SetEchoSuppression(KAL_TRUE);
         }
         else
         {
            aud_mic_set_volume(volume, volume);
            
            /* Turn off echo suppression */
            L1SP_SetEchoSuppression(KAL_FALSE);
         }
#endif
         break;
      }   
      case AUD_MODE_HEADSET:

         /* Set output device */
         aud_speech_set_output_device(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_SPEAKER2));
         aud_keytone_set_output_device(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_SPEAKER2));
         aud_tone_set_output_device(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_SPEAKER2));
         aud_melody_set_output_device(custom_cfg_hw_aud_output_path(aud_context_p->melody_output_device));
         aud_fmr_set_output_device(custom_cfg_hw_aud_output_path(aud_context_p->melody_output_device));

         /*For FM record, not use default input device during record phase, so don't apply input path right away*/
         if((AUD_IN_STATE(AUD_MEDIA_RECORD) || AUD_IN_STATE(AUD_MEDIA_RECORD_PAUSED)) 
              && !aud_context_p->use_default_input)
         {
            aud_mic_set_device_no_apply(custom_cfg_hw_aud_input_path(AUDIO_DEVICE_MIC2));
         }
         else
         {
            aud_mic_set_device(custom_cfg_hw_aud_input_path(AUDIO_DEVICE_MIC2));         
         }

         /* set speech volume */
         volume = aud_get_volume_gain(VOL_HEADSET, AUD_VOLUME_SPH, aud_get_volume_level(VOL_HEADSET, AUD_VOLUME_SPH));
         aud_speech_set_output_volume(volume, volume);
         /* set general tone volume, use ring tone volume level */
         volume = aud_get_volume_gain(VOL_HEADSET, AUD_VOLUME_CTN, aud_get_volume_level(VOL_HEADSET, AUD_VOLUME_MEDIA));
         aud_tone_set_output_volume(volume, volume);
         /* set key tone volume */
         volume = aud_get_volume_gain(VOL_HEADSET, AUD_VOLUME_KEY, aud_get_volume_level(VOL_HEADSET, AUD_VOLUME_KEY));
         aud_keytone_set_output_volume(volume, volume);
         /* set volume when play */
         volume = aud_get_volume_gain(VOL_HEADSET, AUD_VOLUME_MIC, aud_get_volume_level(VOL_HEADSET, AUD_VOLUME_MIC));
         aud_mic_set_volume(volume, volume);
         /* set loopback volume */
         volume = aud_get_volume_gain(VOL_HEADSET, AUD_VOLUME_SID, aud_get_volume_level(VOL_HEADSET, AUD_VOLUME_SID));
         aud_side_tone_set_volume(volume, volume);
         /* set fm radio volume */
         volume = aud_get_volume_gain(VOL_HEADSET, AUD_VOLUME_FMR, aud_get_volume_level(VOL_HEADSET, AUD_VOLUME_FMR));
         aud_fmr_set_volume(volume);
         /* set media volume, use playing melody volume level */
         /* if it's headset mode and output to both earphone & loud speaker, use normal mode gain value */
         if( aud_context_p->melody_output_device == AUDIO_DEVICE_SPEAKER_BOTH )
            gain_audio_mode = VOL_NORMAL;
         else
            gain_audio_mode = VOL_HEADSET;
         volume = aud_get_volume_gain( gain_audio_mode, AUD_VOLUME_MEDIA, current_playing_melody_volume_level );
         aud_melody_set_output_volume( volume, volume );

         /* Set speech FIR coefficient */
         L1SP_Write_Audio_Coefficients(param_p->FIR_input_coeff[1], param_p->FIR_output_coeff[6]);
         aud_speech_set_fir(KAL_TRUE);

         /* Turn off echo suppression */
         L1SP_SetEchoSuppression(KAL_FALSE);

#if !defined(MT6205B)
#ifdef AEC_ENABLE
         L1SP_NormalMode_SetAEC(KAL_TRUE);
#endif // AEC_ENABLE
#ifdef EES_ENABLE
         L1SP_LoudSpkMode_SetEES(KAL_FALSE);
#endif // EES_ENABLE
#endif  
         break;
         
      case AUD_MODE_LOUDSPK: /* Loud Speaker */

         /* To slove mute flag cleared by speech off */
         mute = L1SP_IsMicrophoneMuted();
         L1SP_Speech_Off();
         kal_sleep_task(20);
         L1SP_MuteMicrophone(mute);
         
         /* Set output device */
         aud_speech_set_output_device(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_LOUDSPEAKER));
         aud_keytone_set_output_device(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_LOUDSPEAKER));
         aud_tone_set_output_device(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_LOUDSPEAKER));
         aud_melody_set_output_device(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_LOUDSPEAKER));
         aud_fmr_set_output_device(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_LOUDSPEAKER));

         /*For FM record, not use default input device during record phase, so don't apply input path right away*/
         if((AUD_IN_STATE(AUD_MEDIA_RECORD) || AUD_IN_STATE(AUD_MEDIA_RECORD_PAUSED)) 
              && !aud_context_p->use_default_input)
         {
            aud_mic_set_device_no_apply(custom_cfg_hw_aud_input_path(AUDIO_DEVICE_MIC1));
         }
         else
         {
            aud_mic_set_device(custom_cfg_hw_aud_input_path(AUDIO_DEVICE_MIC1));
         }

         /* set speech volume */
         volume = aud_get_volume_gain(VOL_HANDFREE, AUD_VOLUME_SPH, aud_get_volume_level(VOL_HANDFREE, AUD_VOLUME_SPH));
         aud_speech_set_output_volume(volume, volume);
         /* set general tone volume, use ring tone volume level */
         volume = aud_get_volume_gain(VOL_HANDFREE, AUD_VOLUME_CTN, aud_get_volume_level(VOL_HANDFREE, AUD_VOLUME_MEDIA));
         aud_tone_set_output_volume(volume, volume);
         /* set key tone volume */
         volume = aud_get_volume_gain(VOL_HANDFREE, AUD_VOLUME_KEY, aud_get_volume_level(VOL_HANDFREE, AUD_VOLUME_KEY));
         aud_keytone_set_output_volume(volume, volume);
         /* set volume when play */
         volume = aud_get_volume_gain(VOL_HANDFREE, AUD_VOLUME_MIC, aud_get_volume_level(VOL_HANDFREE, AUD_VOLUME_MIC));
         aud_mic_set_volume(volume, volume);
         /* set loopback volume */
         volume = aud_get_volume_gain(VOL_HANDFREE, AUD_VOLUME_SID, aud_get_volume_level(VOL_HANDFREE, AUD_VOLUME_SID));
         aud_side_tone_set_volume(volume, volume);
         /* set fm radio volume */
         volume = aud_get_volume_gain(VOL_HANDFREE, AUD_VOLUME_FMR, aud_get_volume_level(VOL_HANDFREE, AUD_VOLUME_FMR));
         aud_fmr_set_volume(volume);
         /* set media volume, use playing melody volume level */
         volume = aud_get_volume_gain(VOL_HANDFREE, AUD_VOLUME_MEDIA, current_playing_melody_volume_level);
         aud_melody_set_output_volume(volume, volume);

         /* Set speech FIR coefficient */
         //L1SP_Write_Audio_Coefficients(aud_context_p->FIR_input_coeff[0], aud_context_p->FIR_output_coeff[0]);
         aud_speech_set_fir(KAL_FALSE);

#if !defined(MT6205B)
#ifdef AEC_ENABLE
         L1SP_NormalMode_SetAEC(KAL_FALSE);
#endif // AEC_ENABLE
#ifdef EES_ENABLE
         L1SP_LoudSpkMode_SetEESParameter(param_p->es_time_const, param_p->es_vol_const);
         L1SP_LoudSpkMode_SetEES(KAL_TRUE);
#endif // EES_ENABLE
#endif  
#if defined(MT6205B) || (!defined(MT6205B) && !defined(EES_ENABLE))  /* only echo suppression */
         L1SP_SetEchoSuppressionParameter(param_p->es_time_const, param_p->es_vol_const);
         L1SP_SetEchoSuppression(KAL_TRUE);
#endif  

         // If the speech shall be turned on, turn it on
         if (aud_context_p->speech_on)
            L1SP_Speech_On();
         break;

   }
}


void aud_audio_set_volume_req_hdlr(ilm_struct *ilm_ptr)
{
	SetAudioVolumeReq((UINT32)ilm_ptr, 0, 0, 0);
}

void aud_audio_get_gain_req(kal_uint8 volume_type, kal_uint8 volume_level, kal_uint8 *gain)
{
   *gain = aud_context_p->acoustic_data.volume_gain[aud_context_p->audio_mode][volume_type][volume_level];
 
 }

void aud_set_mute_req_hdlr(ilm_struct *ilm_ptr)
{

   media_aud_set_mute_req_struct *req_p; 

   req_p = (media_aud_set_mute_req_struct*)ilm_ptr->local_para_ptr;

   aud_context_p->audio_mute=req_p->mute;
   if(req_p->mute)
   {
      switch (req_p->vol_type)
      {
         case AUD_VOLUME_CTN:
            aud_tone_set_output_volume(0,0);
            break;
         case AUD_VOLUME_KEY:
            aud_keytone_set_output_volume(0,0);
            break;
         case AUD_VOLUME_MIC:
            aud_mic_mute(req_p->mute);
            break;
         case AUD_VOLUME_SPH:
            aud_speech_set_output_volume(0,0);
            break;
         case AUD_VOLUME_MEDIA:
            aud_melody_set_output_volume(0,0);
            break;
         case AUD_VOLUME_FMR:
            aud_fmr_set_volume( 0 );
            break;
         case AUD_VOLUME_SID:
            break;
         default:
            break;
      }
   }
   else
   {
      kal_uint8 volume;
      volume = aud_get_volume_gain(aud_context_p->audio_mode,
         req_p->vol_type, aud_get_volume_level(aud_context_p->audio_mode, req_p->vol_type));
      switch (req_p->vol_type)
      {
         case AUD_VOLUME_CTN:
            aud_tone_set_output_volume(volume,volume);
            break;
         case AUD_VOLUME_KEY:
            aud_keytone_set_output_volume(volume,volume);
            break;
         case AUD_VOLUME_MIC:
            aud_mic_mute(req_p->mute);
            break;
         case AUD_VOLUME_SPH:
            aud_speech_set_output_volume(volume,volume);
            break;
         case AUD_VOLUME_MEDIA:
            /* if it's headset mode and output to both earphone & loud speaker, use normal mode gain value */
            if( aud_context_p->audio_mode == AUD_MODE_HEADSET && aud_context_p->melody_output_device == AUDIO_DEVICE_SPEAKER_BOTH )
               volume = aud_context_p->acoustic_data.volume_gain[AUD_MODE_NORMAL][AUD_VOLUME_MEDIA][current_playing_melody_volume_level];
            aud_melody_set_output_volume(volume,volume);
            break;
         case AUD_VOLUME_FMR:
            aud_fmr_set_volume( volume );
            break;
         case AUD_VOLUME_SID:
            break;
         default:
            break;
      }
   }

}


void aud_audio_set_fir_req_hdlr(ilm_struct *ilm_ptr)
{
   kal_uint32 cnf_msg_id;     //memssage ID is a large integer
   						//changed by elisa on 2008_10_9
   l4aud_audio_set_fir_cnf_struct* cnf_p = NULL;

   l4aud_audio_set_fir_req_struct *req_p;

   req_p = (l4aud_audio_set_fir_req_struct*)ilm_ptr->local_para_ptr;


   switch(req_p->audio_type)
   {
      case AUD_TYPE_SPEECH:
         aud_speech_set_fir(req_p->fir_on);
         break;
      default:
         break;
   }
      
   cnf_msg_id = MSG_ID_L4AUD_AUDIO_SET_FIR_CNF;
   cnf_p = (l4aud_audio_set_fir_cnf_struct*)
            construct_local_para(sizeof(l4aud_audio_set_fir_cnf_struct), TD_CTRL);  

   cnf_p->result = MED_RES_OK;
   
   aud_send_ilm(ilm_ptr->src_mod_id, 
               cnf_msg_id, 
               cnf_p, 
               NULL);
   
}

kal_bool aud_cfg_audio_out_device(kal_uint8 audio_sound_id, kal_uint8 *out_device_path)
{
   
   ASSERT(out_device_path!=NULL);

   /* if there is no change, it will use the default value at PS */
   /* specially for connect tone */
   if ( (audio_sound_id == TONE_CALL_CONNECT) ||
       (audio_sound_id == TONE_WARNING2) ||
       (audio_sound_id == TONE_ERROR2) )
   {
      switch(aud_context_p->audio_mode)
      {
         case AUD_MODE_NORMAL:
            *out_device_path = AUDIO_DEVICE_SPEAKER;
            break;
         case AUD_MODE_LOUDSPK:
            *out_device_path = AUDIO_DEVICE_LOUDSPEAKER;
            break;
         case AUD_MODE_HEADSET:
            *out_device_path = AUDIO_DEVICE_SPEAKER2;
            break;
         default:
            *out_device_path = AUDIO_DEVICE_LOUDSPEAKER;
            break;     
      }
   }
   else if ( audio_sound_id <= TONE_NEGATIVE_ACK )
   {
      switch(aud_context_p->audio_mode)
      {
         case AUD_MODE_NORMAL:
            *out_device_path = AUDIO_DEVICE_SPEAKER;
            break;
         case AUD_MODE_LOUDSPK:
            *out_device_path = AUDIO_DEVICE_LOUDSPEAKER;
            break;
         case AUD_MODE_HEADSET:
            *out_device_path = AUDIO_DEVICE_SPEAKER2;
            break;
         default:
            *out_device_path = AUDIO_DEVICE_SPEAKER;
            break;     
      }
   }
   else if ( (audio_sound_id >TONE_NEGATIVE_ACK) && (audio_sound_id <= MAX_TONE_ID) )
   {
      switch(aud_context_p->audio_mode)
      {
         case AUD_MODE_NORMAL:
            /* if speech on, tone shall output via speaker1 */
            if (aud_context_p->speech_on)
               *out_device_path = AUDIO_DEVICE_SPEAKER;
            else
               *out_device_path = AUDIO_DEVICE_LOUDSPEAKER;
            break;
         case AUD_MODE_LOUDSPK:
            *out_device_path = AUDIO_DEVICE_LOUDSPEAKER;
            break;
         case AUD_MODE_HEADSET:
            *out_device_path = AUDIO_DEVICE_SPEAKER2;
            break;
         default:
            /* if speech on, tone shall output via speaker1 */
            if (aud_context_p->speech_on)
               *out_device_path = AUDIO_DEVICE_SPEAKER;
            else
               *out_device_path = AUDIO_DEVICE_LOUDSPEAKER;
            break;     
      }
   }
   else if ( (audio_sound_id >= MIN_RING_TONE_ID) && (audio_sound_id <= MAX_RING_TONE_ID) )
   {
      switch(aud_context_p->audio_mode)
      {
         case AUD_MODE_NORMAL:
            *out_device_path = AUDIO_DEVICE_LOUDSPEAKER;
            break;
         case AUD_MODE_LOUDSPK:
            *out_device_path = AUDIO_DEVICE_LOUDSPEAKER;
            break;
         case AUD_MODE_HEADSET:
            *out_device_path = AUDIO_DEVICE_SPEAKER_BOTH;
            break;
         default:
            *out_device_path = AUDIO_DEVICE_LOUDSPEAKER;
            break;     
      }
   }
   else if ( (audio_sound_id >= MIN_MIDI_ID) && (audio_sound_id <= MAX_MIDI_ID) )
   {
      switch(aud_context_p->audio_mode)
      {
         case AUD_MODE_NORMAL:
            *out_device_path = AUDIO_DEVICE_LOUDSPEAKER;
            break;
         case AUD_MODE_LOUDSPK:
            *out_device_path = AUDIO_DEVICE_LOUDSPEAKER;
            break;
         case AUD_MODE_HEADSET:
            *out_device_path = AUDIO_DEVICE_SPEAKER_BOTH;
            break;
         default:
            *out_device_path = AUDIO_DEVICE_LOUDSPEAKER;
            break;     
      }
   }
   else
   {
      return KAL_FALSE;
   }
   

   return KAL_TRUE;
}
BOOL aud_is_in_state(INT32 state)
{
	return (aud_context_p->state == state);
}
extern BOOL g_IsVibrate;   //Added by jinzh:20070716
#ifdef FM_RADIO_ENABLE
extern 	mmi_fmrdo_struct g_fmrdo; // jiashuo add for jessamine,20080504
#endif
#ifdef __MMI_CM_BLACK_LIST__
 extern UINT8 g_in_idle_blackcall_flag;
#endif
#if defined(__MMI_ANALOGTV__)
extern unsigned char atvif_GetCurModule(void);
#endif
void aud_play_by_id_req_hdlr(ilm_struct *ilm_ptr)
{
#ifdef DUMMY_CNF   
   kal_uint16  cnf_msg_id;
   l4aud_audio_play_cnf_struct* cnf_p = NULL;
#endif
	TBM_ENTRY(0x2A00);
   kal_uint8   audio_id;

   l4aud_audio_play_req_struct *req_p;

   req_p = (l4aud_audio_play_req_struct*)ilm_ptr->local_para_ptr;

   audio_id = req_p->audio_id;

   mmi_trace(TRUE, "audio_id is %d,req_p->volume is %d", req_p->audio_id,req_p->volume);
   //aud_cfg_audio_out_device(audio_id, &audio_out_device);
   
//added by liwh
  // aud_context_p->identifier = req_p->identifier;
   aud_context_p->src_mod = ilm_ptr->src_mod_id;
   if (
#if defined( __MED_VID_MOD__) || defined (__MED_MJPG_MOD__)
       !vid_is_audio_channel_available()
#endif
      )
   {
   }
else  if ( audio_id <= TONE_KEY_CLICK )
   {
      mmi_trace(TRUE, "play keytone vol%d", req_p->volume);
      //aud_keytone_stop();
      //aud_keytone_play(audio_id, audio_out_device);
	  /*Jinzh Modify Start For 5563 Ver: TBM780  on 2007-8-1 17:26 */
	  if(   (aud_context_p->tone_playing == KAL_FALSE)
 		 &&(mmi_frm_kbd_get_key_tone_state()==MMI_KEY_TONE_ENABLED)
		 && (g_IsVibrate == FALSE)
		 #ifdef FM_RADIO_ENABLE
		 && (g_fmrdo.is_power_on == FALSE)//jiashuo add for jessamine,20080504
		 #endif
		 //jiashuo add start 20080626,在播放音乐时和在音乐播放器界面时不播放按键音 
		 && (mmi_audply_is_playing() == FALSE)
		 //jiashuo add end 20080626
		 && !AUD_IN_STATE(AUD_MEDIA_PLAY)
#if defined(__MMI_ANALOGTV__)
		 //qiff not in analogtv app
		&& (atvif_GetCurModule() == 0)
#endif

		 )
	  {
	  	mmi_trace(1,"keytone,play key tone");
/*qiff 08/10/8 del for greenstone:bug: 9513 etc.*/
	      if (!(aud_context_p->speech_on))
	      {
	         aud_keytone_stop();
	      }
/*qiff end*/
	  	PlayKeyPadToneReq(req_p);
	  }
	  /*Jinzh Modify End  For 5563 Ver: TBM780  on 2007-8-1 17:26 */
	  
/*Jinzh Del Start For 5563 Ver: TBM780  on 2007-8-1 17:13 */
      #if 0
      if ((aud_context_p->tone_playing)&&!(aud_context_p->speech_on))
      {
        // aud_keytone_stop();
        
        TBM_EXIT(0x2A00);
        return ;
      }
      else
      {
      	if(aud_context_p->speech_on)/*Added by jinzh:20070720*/
      	{
      	}
      	else if (g_IsVibrate==FALSE/*Added by jinzh:20070716*/)
		{
			PlayKeyPadToneReq(req_p);
		}
	//by liwh
	//aud_send_audio_play_finish_ind(MED_RES_OK);
	  }      
      #endif
/*Jinzh Del End  For 5563 Ver: TBM780  on 2007-8-1 17:13 */

   }
   else if ((audio_id >TONE_KEY_CLICK) && (audio_id <= MAX_TONE_ID) )
   {
      /* if keytone is playing, stop it */
      //aud_keytone_stop();

      //if (req_p->play_style == DEVICE_AUDIO_PLAY_INFINITE)
      //   aud_context_p->tone_playing = KAL_TRUE;         
      //aud_tone_play(audio_id, audio_out_device);
      mmi_trace(TRUE, "play PlaySpecialTone");
	  #ifdef FM_RADIO_ENABLE
	  if(g_fmrdo.is_power_on == TRUE)
	  {
	    AUD_SET_EVENT(AUD_EVT_PLAY);
	  	return;
	  }
	  #endif
#if defined(__MMI_VIDEO_PLAYER__)
	  //jiashuo add for greenstone,when play music,donot play warning tone,20081016
	  if((mmi_audply_is_playing() == TRUE)||( mmi_vdoply_is_in_playing() == TRUE))
	  {
	 	 AUD_SET_EVENT(AUD_EVT_PLAY);
	  	return;
	  }
#endif
/*qiff 08/10/8 del for greenstone:bug: 9513 etc.*/
#if 0
      if ((aud_context_p->tone_playing)&&!(aud_context_p->speech_on))
      {
        // aud_keytone_stop();
         TBM_EXIT(0x2A00);
        return ;
      }
      else
      {
        PlaySpecialToneReq(req_p);
      }
#else
      if ((aud_context_p->tone_playing)&&!(aud_context_p->speech_on))
      {
         aud_keytone_stop();
      }
	if(!AUD_IN_STATE(AUD_MEDIA_PLAY))
	{
		PlaySpecialToneReq(req_p);
	}
#endif
/*qiff end*/      
	
   }
   else 
   {


      if (aud_context_p->speech_on)
      	{
      		AUD_SET_EVENT(AUD_EVT_PLAY);
      		TBM_EXIT(0x2A00);
         	return;
      	}

#if defined( __MED_VID_MOD__) || defined (__MED_MJPG_MOD__)
      if(!vid_is_audio_channel_available())
      	{
      		AUD_SET_EVENT(AUD_EVT_PLAY);
      		TBM_EXIT(0x2A00);
         	return;
      	}
#endif
      /* if keytone is playing, stop it */
      aud_keytone_stop();

      /* if tone is playing, stop it */
      if (aud_context_p->tone_playing)
         aud_tone_stop();
#ifdef __MED_MMA_MOD__
      /* close all mma tasks */
      aud_mma_close_all();
#endif

      if (!(aud_context_p->state == AUD_MEDIA_RECORD ||
         aud_context_p->state == AUD_MEDIA_RECORD_PAUSED ||
         aud_context_p->state == AUD_VM_RECORD ||
         aud_context_p->state == AUD_VM_RECORD_PAUSED))
      {
      	  aud_context_p->identifier = req_p->identifier;
         aud_stop_unfinished_process();

         #if 0
                  
         aud_context_p->src_mod = ilm_ptr->src_mod_id;
         aud_context_p->identifier = req_p->identifier;

         /* set volume and path if from LMMI */
         if (aud_context_p->src_mod==MOD_MMI)
         {
            SET_CURRENT_VOLUME_LEVEL(AUD_VOLUME_MEDIA, req_p->volume);
            aud_context_p->melody_output_device=req_p->output_path;
            if (aud_context_p->audio_mode==AUD_MODE_HEADSET)
               aud_melody_set_output_device(custom_cfg_hw_aud_output_path(req_p->output_path));
         }
         else
         {
            aud_context_p->melody_output_device=AUDIO_DEVICE_SPEAKER_BOTH;
         }
         aud_context_p->play_mode = AUD_MEDIA_PLAY_AS_RING;

         #endif
         //aud_melody_play_by_id(audio_id, req_p->play_style, audio_out_device);
         mmi_trace(1, "b PlayRingToneReq");
         aud_context_p->tone_playing = KAL_TRUE;

		//add by pan
          AUD_ENTER_STATE(AUD_MEDIA_PLAY);
	   aud_context_p->play_mode = AUD_MEDIA_PLAY_AS_RING;
		aud_context_p->source_type = AUD_ID;

         PlayRingToneReq(req_p);
	
      }
   }
  
  	 AUD_SET_EVENT(AUD_EVT_PLAY);
 	TBM_EXIT(0x2A00);
	
#ifdef DUMMY_CNF   
   cnf_p = (l4aud_audio_play_cnf_struct*)
            construct_local_para(sizeof(l4aud_audio_play_cnf_struct), TD_CTRL);  
   cnf_p->result = MED_RES_OK;

   cnf_msg_id = MSG_ID_L4AUD_AUDIO_PLAY_CNF;

   aud_send_ilm(ilm_ptr->src_mod_id, 
               cnf_msg_id, 
               cnf_p, 
               NULL);
#endif
}

void aud_stop_by_id_req_hdlr(ilm_struct *ilm_ptr)
{
#ifdef DUMMY_CNF   
   kal_uint16  cnf_msg_id;
   l4aud_audio_stop_cnf_struct* cnf_p = NULL;
#endif

   TBM_ENTRY(0x2A01);
   kal_uint8   audio_id;

   l4aud_audio_stop_req_struct *req_p;
 
   aud_context_p->src_mod = ilm_ptr->src_mod_id;

   req_p = (l4aud_audio_stop_req_struct*)ilm_ptr->local_para_ptr;

   audio_id = req_p->audio_id;
   mmi_trace(TRUE, "stop_aud_id");

   if ( audio_id <= TONE_KEY_CLICK )
   {
      mmi_trace(TRUE, "Stop key pad tone!");
      //aud_keytone_stop();
   }
   else if ( (audio_id >TONE_KEY_CLICK) && (audio_id <= MAX_TONE_ID) )
   {
      mmi_trace(TRUE, "Stop special tone!");
      //aud_tone_stop();
   }
   else 
   {
      if (!(aud_context_p->state == AUD_MEDIA_RECORD ||
         aud_context_p->state == AUD_MEDIA_RECORD_PAUSED ||
         aud_context_p->state == AUD_VM_RECORD ||
         aud_context_p->state == AUD_VM_RECORD_PAUSED||
         aud_context_p->state == AUD_MEDIA_IDLE))
      {
         aud_stop_unfinished_process();
      }  
      mmi_trace(TRUE, "stop_aud_id_AudioStopStream");
      aud_tone_stop();
   }
   /* set event to let caller go */
   AUD_SET_EVENT(AUD_EVT_STOP);
   TBM_EXIT(0x2A01);
   
#ifdef DUMMY_CNF    
   cnf_p = (l4aud_audio_stop_cnf_struct*)
            construct_local_para(sizeof(l4aud_audio_stop_cnf_struct), TD_CTRL);  
 
   cnf_msg_id = MSG_ID_L4AUD_AUDIO_STOP_CNF;

   cnf_p->result = MED_RES_OK;

   aud_send_ilm(ilm_ptr->src_mod_id, 
               cnf_msg_id, 
               cnf_p, 
               NULL);
#endif

}


void aud_play_by_name_req_hdlr(ilm_struct *ilm_ptr)
{

   kal_uint32  cnf_msg_id;
   //kal_uint8   audio_out_device=0;
   l4aud_audio_play_by_name_cnf_struct* cnf_p = NULL;

   l4aud_audio_play_by_name_req_struct *req_p;

   aud_context_p->src_mod = ilm_ptr->src_mod_id;

   req_p = (l4aud_audio_play_by_name_req_struct*)ilm_ptr->local_para_ptr;

   cnf_p = (l4aud_audio_play_by_name_cnf_struct*)
            construct_local_para(sizeof(l4aud_audio_play_by_name_cnf_struct), TD_CTRL);  

   if (aud_context_p->speech_on 
#if defined( __MED_VID_MOD__) || defined (__MED_MJPG_MOD__)
      || !vid_is_audio_channel_available()
#endif
      )
   {
      cnf_p->result = MED_RES_BUSY;
   }
   else 
   {
      /* if keytone is playing, stop it */
      aud_keytone_stop();

      /* if tone is playing, stop it */
      if (aud_context_p->tone_playing)
         aud_tone_stop();
      
#ifdef __MED_MMA_MOD__
      /* close all mma tasks */
      aud_mma_close_all();
#endif

      if (!(aud_context_p->state == AUD_MEDIA_RECORD ||
         aud_context_p->state == AUD_MEDIA_RECORD_PAUSED ||
         aud_context_p->state == AUD_VM_RECORD ||
         aud_context_p->state == AUD_VM_RECORD_PAUSED))
      {
         aud_stop_unfinished_process();

         aud_context_p->play_mode = AUD_MEDIA_PLAY_AS_RING;

         aud_context_p->identifier = req_p->identifier;
         
         /* set volume and path if from LMMI */
         if (aud_context_p->src_mod==MOD_MMI)
         {
            SET_CURRENT_VOLUME_LEVEL(AUD_VOLUME_MEDIA, req_p->volume);
            aud_context_p->melody_output_device=req_p->output_path;
            if (aud_context_p->audio_mode==AUD_MODE_HEADSET)
               aud_melody_set_output_device(custom_cfg_hw_aud_output_path(req_p->output_path));
         }
         else
         {
            aud_context_p->melody_output_device=AUDIO_DEVICE_SPEAKER_BOTH;
         }

         aud_context_p->current_format = med_get_media_type(req_p->file_name);
         switch(aud_context_p->current_format)
         {
            case MED_TYPE_SMF:
            case MED_TYPE_MMF:
            case MED_TYPE_SMF_SND:
            case MED_TYPE_IMELODY:
               cnf_p->result = aud_melody_play_by_name(req_p->file_name, req_p->play_style, 0);
               break;
            case MED_TYPE_DAF:
            case MED_TYPE_AMR:
            case MED_TYPE_AMR_WB:
            case MED_TYPE_GSM_EFR:
            case MED_TYPE_PCM_8K:
            case MED_TYPE_PCM_16K:
            case MED_TYPE_DVI_ADPCM:
            case MED_TYPE_WAV:
            case MED_TYPE_WAV_DVI_ADPCM:
            case MED_TYPE_AIFF:
            case MED_TYPE_AU:
               cnf_p->result = aud_media_play(req_p->file_name, req_p->play_style, 1);
               break;
            default:
               cnf_p->result = MED_RES_INVALID_FORMAT;
               break;
         }
      }      
      else
         cnf_p->result = MED_RES_BUSY;
   }
      
   cnf_p->src_id = req_p->src_id;

   cnf_msg_id = MSG_ID_L4AUD_AUDIO_PLAY_BY_NAME_CNF;

   aud_send_ilm(ilm_ptr->src_mod_id, 
               cnf_msg_id, 
               cnf_p, 
               NULL);

}

void aud_stop_by_name_req_hdlr(ilm_struct *ilm_ptr)
{
   kal_uint32  cnf_msg_id;
   l4aud_audio_stop_by_name_cnf_struct* cnf_p = NULL;

   l4aud_audio_stop_by_name_req_struct *req_p;
 
   aud_context_p->src_mod = ilm_ptr->src_mod_id;

   req_p = (l4aud_audio_stop_by_name_req_struct*)ilm_ptr->local_para_ptr;

   if (!(aud_context_p->state == AUD_MEDIA_RECORD ||
      aud_context_p->state == AUD_MEDIA_RECORD_PAUSED ||
      aud_context_p->state == AUD_VM_RECORD ||
      aud_context_p->state == AUD_VM_RECORD_PAUSED))
   {
      aud_stop_unfinished_process();
   }
   
   /* set event to let caller go */
   AUD_SET_EVENT(AUD_EVT_STOP);

   cnf_msg_id = MSG_ID_L4AUD_AUDIO_STOP_BY_NAME_CNF;

   cnf_p = (l4aud_audio_stop_by_name_cnf_struct*)
            construct_local_para(sizeof(l4aud_audio_stop_by_name_cnf_struct), TD_CTRL);  

   cnf_p->src_id = req_p->src_id;
   cnf_p->result = MED_RES_OK;          

   aud_send_ilm(ilm_ptr->src_mod_id, 
               cnf_msg_id, 
               cnf_p, 
               NULL);

}



extern void MusicStartAudioMode(); 

void aud_play_by_string_req_hdlr(ilm_struct *ilm_ptr)
{
  // l4aud_audio_play_by_string_cnf_struct* cnf_p = NULL;
   l4aud_audio_play_by_string_req_struct *req_p =  NULL;
   kal_uint8 res=MED_RES_OK;

       			
    aud_play_string_struct *data_p = NULL;
    //ilm_struct *Message = NULL;
    //Message = (ilm_struct *)pCosEvent->nParam1;
    data_p = (aud_play_string_struct *)ilm_ptr->local_para_ptr;
	mmi_trace(1,"chenhe,aud_play_by_string_req_hdlr");

   aud_context_p->src_mod = ilm_ptr->src_mod_id;

    
  req_p = (l4aud_audio_play_by_string_req_struct*)ilm_ptr->local_para_ptr; 

   if (aud_context_p->speech_on 
#if defined( __MED_VID_MOD__) || defined (__MED_MJPG_MOD__)
      || !vid_is_audio_channel_available()
#endif
      )
   {
      res = MED_RES_BUSY;
   }
   else 
   {
      /* if keytone is playing, stop it */
      aud_keytone_stop();

      /* if tone is playing, stop it */
      if (aud_context_p->tone_playing)
         aud_tone_stop();
      
#ifdef __MED_MMA_MOD__
      /* close all mma tasks */
      aud_mma_close_all();
#endif
      if (!(aud_context_p->state == AUD_MEDIA_RECORD ||
         aud_context_p->state == AUD_MEDIA_RECORD_PAUSED ||
         aud_context_p->state == AUD_VM_RECORD ||
         aud_context_p->state == AUD_VM_RECORD_PAUSED))
      {
         aud_stop_unfinished_process();
         #if 1

         aud_context_p->play_mode = AUD_MEDIA_PLAY_AS_RING;
         aud_context_p->identifier = req_p->identifier;
         aud_context_p->current_format=req_p->format;
         aud_context_p->play_style=req_p->play_style;
		aud_context_p->source_type = AUD_STRING;
         /* set volume and path if from LMMI */
         if (aud_context_p->src_mod==MOD_MMI)
         {
            SET_CURRENT_VOLUME_LEVEL(AUD_VOLUME_MEDIA, req_p->volume);
            aud_context_p->melody_output_device=req_p->output_path;
            if (aud_context_p->audio_mode==AUD_MODE_HEADSET)
               aud_melody_set_output_device(custom_cfg_hw_aud_output_path(req_p->output_path));
         }
         else
         {
            aud_context_p->melody_output_device=AUDIO_DEVICE_SPEAKER_BOTH;
         }

         res=aud_melody_play_by_string((kal_uint8*)req_p->melody, req_p->play_style, req_p->len, req_p->format);
         #endif
      }
      else
         res = MED_RES_BUSY;
   }

	if(res == MED_RES_OK)
	{
		DM_SetAudioVolume(req_p->volume);
		MusicStartAudioMode();
		AUD_ENTER_STATE(AUD_MEDIA_PLAY);
	}
	else
	{
		 AUD_ENTER_STATE(AUD_MEDIA_IDLE);
	}
	aud_set_result((kal_int32)res);
	AUD_SET_EVENT(AUD_EVT_PLAY);
#if 0
   if(!(aud_context_p->src_mod==MOD_MMI && data_p->blocking))
   {
      cnf_p = (l4aud_audio_play_by_string_cnf_struct*)
               construct_local_para(sizeof(l4aud_audio_play_by_string_cnf_struct), TD_CTRL);  

      cnf_p->src_id = data_p->src_id;
      cnf_p->result = res;


      aud_send_ilm(ilm_ptr->src_mod_id, 
                  MSG_ID_L4AUD_AUDIO_PLAY_BY_STRING_CNF, 
                  cnf_p, 
                  NULL);
   }
#endif   
}

void aud_stop_by_string_req_hdlr(ilm_struct *ilm_ptr)
{
   l4aud_audio_stop_by_string_cnf_struct* cnf_p = NULL;

   l4aud_audio_stop_by_string_req_struct *req_p;
 
   aud_context_p->src_mod = ilm_ptr->src_mod_id;

   req_p = (l4aud_audio_stop_by_string_req_struct*)ilm_ptr->local_para_ptr;

   if (!(aud_context_p->state == AUD_MEDIA_RECORD ||
      aud_context_p->state == AUD_MEDIA_RECORD_PAUSED ||
      aud_context_p->state == AUD_VM_RECORD ||
      aud_context_p->state == AUD_VM_RECORD_PAUSED))
   {
      aud_stop_unfinished_process();
   }
   aud_context_p->source_type=AUD_UNKNOWN_SOURCE_TYPE;
   aud_context_p->current_format=MED_TYPE_NONE;
   aud_context_p->string_p=NULL;
   
   /* set event to let caller go */
   AUD_SET_EVENT(AUD_EVT_STOP);

   if(aud_context_p->src_mod!=MOD_MMI)
   {
   cnf_p = (l4aud_audio_stop_by_string_cnf_struct*)
            construct_local_para(sizeof(l4aud_audio_stop_by_string_cnf_struct), TD_CTRL);  
 
   cnf_p->src_id = req_p->src_id;
   cnf_p->result = MED_RES_OK;
 
   aud_send_ilm(ilm_ptr->src_mod_id, 
               MSG_ID_L4AUD_AUDIO_STOP_BY_STRING_CNF, 
               cnf_p, 
               NULL);
   }
}


void aud_block_req_hdlr(ilm_struct *ilm_ptr)
{
   media_aud_block_req_struct *req_p;
   kal_int16 result=MED_RES_OK;

   req_p = (media_aud_block_req_struct*)ilm_ptr->local_para_ptr;

   aud_context_p->access_mod=(module_type)req_p->mod_id;
   aud_context_p->block_level=req_p->level;
   
   aud_keytone_stop();

   /* if tone is playing, stop it */
   if (aud_context_p->tone_playing)
      aud_tone_stop();

      
   if (!(aud_context_p->state == AUD_MEDIA_RECORD ||
      aud_context_p->state == AUD_MEDIA_RECORD_PAUSED ||
      aud_context_p->state == AUD_VM_RECORD ||
      aud_context_p->state == AUD_VM_RECORD_PAUSED))
   {
      aud_stop_unfinished_process();
   }

#ifdef __MED_MMA_MOD__
   /* close all mma tasks */
   aud_mma_close_all();

   /* if midi device is opening, close it */
   if (aud_context_p->midi_device_opening == KAL_TRUE) 
   {
      if (req_p->mod_id == MOD_MMI) 
      { // MMI take control
         if (aud_context_p->midi_device_handle != (kal_int32)NULL) 
         {
             JMidi_CloseDevice((Media_Handle*)aud_context_p->midi_device_handle);
             aud_context_p->midi_device_handle =(kal_int32) NULL;
             aud_context_p->midi_device_opening = KAL_FALSE;   
         } 
         else
         {
            ASSERT(0);
         }
      }
   }
#endif
#ifdef __MED_VID_MOD__
   result=vid_abort_by_client_switching();
#endif
#ifdef __MED_CAM_MOD__
   cam_abort_to_idle();
#endif
   if(result==MED_RES_OK)
      AUD_SET_EVENT(AUD_EVT_BLOCK);
}

void aud_set_equalizer_req_hdlr( ilm_struct *ilm_ptr )
{
#ifdef   __L1_SUPPORT_AUDIO_EQUALIZER__
   media_aud_set_equalizer_req_struct *req_p = (media_aud_set_equalizer_req_struct*)ilm_ptr->local_para_ptr;
   AEQ_SetAudioEqualizer( req_p->magnitude );
#endif
}

void aud_set_headset_mode_output_path_req_hdlr( ilm_struct *ilm_ptr )
{
	media_aud_set_headset_mode_output_path_struct *msg_p;
	msg_p = (media_aud_set_headset_mode_output_path_struct*)ilm_ptr->local_para_ptr;

	aud_context_p->melody_output_device = msg_p->device;
	if( aud_context_p->audio_mode == AUD_MODE_HEADSET )
		aud_melody_set_output_device( custom_cfg_hw_aud_output_path(msg_p->device) );
}

#ifdef MAGIC_AE_SUPPORT
//for audio effect.sheen
void aud_audio_ae_req_hdlr(ilm_struct *ilm_ptr)
{
	jtAEErr res;
   	media_aud_ae_req_struct *req_p;

	req_p = (media_aud_ae_req_struct*)ilm_ptr->local_para_ptr;
	if(!req_p)return;
	if(!req_p->aeHandle || !req_p->pData)return; 
	if(*req_p->aeHandle!=0)
		res=jtAE_Progress((unsigned long)(*req_p->aeHandle), (short*)req_p->pData, (int)req_p->dataLen);//size be short uint.
	req_p->ae_callback();
}
#endif

#endif

