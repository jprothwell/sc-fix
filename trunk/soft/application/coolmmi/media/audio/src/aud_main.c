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
 *   aud_main.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes primary functions of audio manager task.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef MED_NOT_PRESENT


/*==== INCLUDES =========*/

/* system includes */
#include "kal_release.h" 
#include "stack_common.h"  
#include "stack_msgs.h"
#include "app_ltlcom.h"          /* task message communiction */
#include "stacklib.h"           /* basic type for dll, evshed, stacktimer */
#include "event_shed.h"          /* event scheduler */
#include "stack_timer.h"         /* stack timer */

/* global includes */

#include "l1audio.h"
#include "device.h"
#include "resource_audio.h"
#include "nvram_struct.h"
#include "nvram_user_defs.h"
#include "nvram_data_items.h"
#include "custom_nvram_editor_data_item.h"

/* local includes */
#include "med_global.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "med_main.h"
#include "aud_main.h"

#include "mmi_trace.h"

aud_context_struct*   aud_context_p = &(med_context.aud_context);

/* ring buffer for VM recording/playback or DAF playing */

//kal_uint8  melody_buffer[AUD_MELODY_BUFFER_LEN];

kal_uint16 ring_buffer[1];//AUD_RING_BUFFER_LEN

extern void aud_send_media_play_finish_ind2(ilm_struct *ilm_ptr);
extern void aud_send_media_record_finish_ind2(ilm_struct *ilm_ptr);

#ifdef __MED_BT_MOD__
extern kal_bool aud_bt_main(ilm_struct *ilm_ptr);
extern void aud_bt_init(void);
#endif /* __MED_BT_MOD__ */ 



/*==== FUNCTIONS ===========*/

/******************************************************************
* FUNCTION
*    aud_startup_hdlr
* DESCRIPTION
*    This function is handle startup procedure of audio module.
* PARAMETERS
*    
*    
* RETURNS
*
* GLOBALS AFFECTED
*  
*  
******************************************************************/
void aud_startup_hdlr( ilm_struct *ilm_ptr)
{
   aud_context_p->startup = KAL_TRUE;

   aud_context_p->src_mod = ilm_ptr->src_mod_id;
   aud_context_p->aud_event = kal_create_event_group("aud_events");
   aud_context_p->mma_event = kal_create_event_group("mma_events");

   aud_melody_startup();
   
#if defined( VM_SUPPORT ) && defined( VM_LOG )
   aud_vm_startup();
#endif

#ifdef __MED_MMA_MOD__
   aud_mma_startup();
#endif   

#ifdef __MED_VR_MOD__
   aud_vr_startup();
#endif

#ifdef __MED_VRSI_MOD__
  aud_vrsi_startup();	
#endif 

#ifdef __MED_GENERAL_TTS__
    aud_tts_startup();
#endif 
}


/*************************************************************************
* FUNCTION
*	aud_set_startup_param
*
* DESCRIPTION
*	This function is to set audio parameters when startup. 
*
* PARAMETERS
*	None
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_set_startup_param(void)
{
   aud_mic_mute(KAL_TRUE);
   aud_set_aud_path_volume(aud_context_p->audio_mode);
   aud_melody_set_max_swing(aud_context_p->audio_param.max_swing);
}

/*************************************************************************
* FUNCTION
*	aud_startup_read_nvram_data
*
* DESCRIPTION
*	This function is to read audio nvram data when startup. 
*
* PARAMETERS
*	None
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_startup_read_nvram_data(void)
{
 
   med_context_p->nvram_state=MED_POWERON_READING_NVRAM;
   aud_send_msg_to_nvram(MSG_ID_NVRAM_READ_REQ, NVRAM_EF_CUST_ACOUSTIC_DATA_LID, 0, 0);
}


/******************************************************************
* FUNCTION
*    aud_main
* DESCRIPTION
*    This function is main message dispatching function of audio module.
* PARAMETERS
*    
*    
* RETURNS
*
* GLOBALS AFFECTED
*  
*  
******************************************************************/
void aud_main( ilm_struct *ilm_ptr)
{
#ifdef __MED_BT_MOD__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (aud_bt_main(ilm_ptr))
    {
        return;
    }
#endif /* __MED_BT_MOD__ */ 
   switch( ilm_ptr->msg_id )
   {
      case MSG_ID_L4AUD_SP_SET_MODE_REQ:
         aud_speech_set_mode_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_L4AUD_AUDIO_SET_VOLUME_REQ:
         aud_audio_set_volume_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_L4AUD_AUDIO_PLAY_REQ:
         aud_play_by_id_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_L4AUD_AUDIO_STOP_REQ:
         aud_stop_by_id_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_L4AUD_AUDIO_PLAY_BY_NAME_REQ:
         aud_play_by_name_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_L4AUD_AUDIO_STOP_BY_NAME_REQ:
         aud_stop_by_name_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_L4AUD_AUDIO_PLAY_BY_STRING_REQ:
         aud_play_by_string_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_L4AUD_AUDIO_STOP_BY_STRING_REQ:
         aud_stop_by_string_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_L4AUD_AUDIO_PLAY_TONE_SEQ_REQ:
         break;
      case MSG_ID_L4AUD_AUDIO_STOP_TONE_SEQ_REQ:
         break;
      case MSG_ID_L4AUD_AUDIO_DEMO_IMY_REQ:
         break;
      /* daf, xwav, amr */
      case MSG_ID_L4AUD_MEDIA_RECORD_REQ:
         aud_media_record_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_L4AUD_MEDIA_PLAY_REQ:
         aud_media_play_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_L4AUD_MEDIA_STOP_REQ:
         aud_media_stop_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_L4AUD_MEDIA_PAUSE_REQ:
         aud_media_pause_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_L4AUD_MEDIA_RESUME_REQ:
         aud_media_resume_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_AUD_MEDIA_READ_DATA_IND:
         aud_media_read_data_ind_hdlr(ilm_ptr);
         break;
      case MSG_ID_AUD_MEDIA_WRITE_DATA_IND:
         aud_media_write_data_ind_hdlr(ilm_ptr);
         break;
      case MSG_ID_AUD_MEDIA_PLAY_EVENT_IND:
         aud_media_play_event_hdlr(ilm_ptr);
         break;
      case MSG_ID_MEDIA_AUD_BLOCK_REQ:
         aud_block_req_hdlr(ilm_ptr);
         break;
 	case MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND:
		aud_send_media_play_finish_ind2(ilm_ptr); 
		break;
	  case MSG_ID_L4AUD_MEDIA_RECORD_FINISH_IND:
	  	aud_send_media_record_finish_ind2(ilm_ptr);
		break;
      case MSG_ID_L4AUD_MEDIA_STORE_REQ:
         aud_media_store_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_L4AUD_MEDIA_RESTORE_REQ:
         aud_media_restore_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_MEDIA_AUD_SET_MUTE_REQ:
         aud_set_mute_req_hdlr(ilm_ptr);
         break;
      case MSG_ID_MEDIA_AUD_SET_VIBRATOR_ENABLED_REQ:
      	  aud_set_vibrator_enabled_req_hdlr(ilm_ptr);
      	  break;

#ifdef __MMI_SUPPORT_BACKLIGHT_SYNC__
/* -- added for turn on/off backlight by MMI task -- */
	case MSG_ID_MMIAPI_BACKLIGHT_CONTROL_RSP:
		aud_backlight_ctrl_rsp_hdlr( ilm_ptr );
		break;
#endif
	case MSG_ID_MEDIA_AUD_SET_HEADSET_MODE_OUTPUT_PATH:
		aud_set_headset_mode_output_path_req_hdlr( ilm_ptr );
		break;

#ifdef __MMI_SUPPORT_LED_SYNC__
/* -- added for turn on/off backlight by MMI task -- */
	case MSG_ID_MMIAPI_LED_CONTROL_RSP:
		aud_led_ctrl_rsp_hdlr( ilm_ptr );
		break;
#endif
		
	case MSG_ID_MEDIA_AUD_SET_EQUALIZER_REQ:
		aud_set_equalizer_req_hdlr( ilm_ptr );
		break;

// play file portion related function
	case MSG_ID_MEDIA_AUD_GET_FILE_DATA_LENGTH_REQ:
		aud_get_file_data_length_req_hdlr( ilm_ptr );
		break;
	case MSG_ID_MEDIA_AUD_GET_FILE_DATA_PROGRESS_REQ:
		aud_get_file_data_progress_req_hdlr( ilm_ptr );
		break;
	case MSG_ID_MEDIA_AUD_SET_FILE_DATA_PROGRESS_REQ:
		aud_set_file_data_progress_req_hdlr( ilm_ptr );
		break;

 
	case MSG_ID_MEDIA_FMR_POWER_ON_REQ:
		aud_fmr_power_on_req_hdlr( ilm_ptr );
		break;
	case MSG_ID_MEDIA_FMR_POWER_OFF_REQ:
		aud_fmr_power_off_req_hdlr( ilm_ptr );
		break;
	case MSG_ID_MEDIA_FMR_SET_FREQ_REQ:
		aud_fmr_set_freq_req_hdlr( ilm_ptr );
		break;
	case MSG_ID_MEDIA_FMR_MUTE_REQ:
		aud_fmr_mute_req_hdlr( ilm_ptr );
		break;
	case MSG_ID_MEDIA_FMR_CHECK_IS_VALID_STOP_REQ:
		aud_fmr_check_is_valid_stop_req_hdlr( ilm_ptr );
		break;
	case MSG_ID_MEDIA_FMR_GET_SIGNAL_LEVEL_REQ:
		aud_fmr_get_signal_level_req_hdlr( ilm_ptr );
		break;
   #if defined(__MED_AUD_EQUALIZER__)
        case MSG_ID_MEDIA_AUD_EQ_SET_MAGNITUDE_REQ:
            aud_eq_set_magnitude_req_hdlr(ilm_ptr);
            break;
        case MSG_ID_MEDIA_AUD_EQ_TURN_ON_REQ:
            aud_eq_turn_on_req_hdlr();
            break;
        case MSG_ID_MEDIA_AUD_EQ_TURN_OFF_REQ:
            aud_eq_turn_off_req_hdlr();
            break;
    #endif /* #ifdef defined(__MED_AUD_EQUALIZER__) */ 
    #ifdef __MED_GENERAL_TTS__
        case MSG_ID_MEDIA_AUD_TTS_SET_ATTR_REQ:
            aud_tts_set_attr_req_hdlr(ilm_ptr);
            break;
        case MSG_ID_MEDIA_AUD_TTS_PLAY_EVENT_IND:
            aud_tts_process_event_ind_hdlr(ilm_ptr);
            break;
        case MSG_ID_MEDIA_AUD_TTS_PLAY_REQ:
            aud_tts_play_req_hdlr(ilm_ptr);
            break;
        case MSG_ID_MEDIA_AUD_TTS_STOP_REQ:
            aud_tts_stop_req_hdlr(ilm_ptr);
            break;
        case MSG_ID_MEDIA_AUD_TTS_PAUSE_REQ:
            aud_tts_pause_req_hdlr(ilm_ptr);
            break;
        case MSG_ID_MEDIA_AUD_TTS_RESUME_REQ:
            aud_tts_resume_req_hdlr(ilm_ptr);
            break;
    #endif /*#ifdef __MED_GENERAL_TTS__*/
 

#ifdef MAGIC_AE_SUPPORT
	//audio effect.sheen
	case MSG_ID_MEDIA_AUD_AE_REQ:
		aud_audio_ae_req_hdlr( ilm_ptr );
		break;
#endif

      default:
         /* assert */
         break;
   }
   return;

}



/******************************************************************
 * FUNCTION
 *    aud_init
 * DESCRIPTION
 *    This function is used to init audio task.
 * PARAMETERS
 *    
 *    
 * RETURNS
 *
 * GLOBALS AFFECTED
 *  
 *  
 ******************************************************************/
kal_bool aud_init(void)
{

	mmi_trace(TRUE, "Excute func : aud_init()");
   //aud_context_p->melody_string_p = &melody_buffer[0];
   aud_context_p->melody_string_p = NULL;
   aud_context_p->ring_buf = &ring_buffer[0];
   aud_context_p->buf_p = aud_context_p->ring_buf;
   aud_context_p->buf_len = 0;
   aud_context_p->offset = 0;
   aud_context_p->access_mod = MOD_MMI;
   aud_context_p->block_level = AUD_BLOCK_ALL;
   
   if (!aud_speech_init())
      return KAL_FALSE;
      
   if (!aud_audio_init())
      return KAL_FALSE;
      
   if(!aud_keytone_init())
      return KAL_FALSE;
      
   if (!aud_tone_init())
      return KAL_FALSE;
      
   if (!aud_melody_init())
      return KAL_FALSE;
   
#if defined( VM_SUPPORT ) && defined( VM_LOG )
   if (!aud_vm_init())
      return KAL_FALSE;
#endif

   if (!aud_media_init())
      return KAL_FALSE;

#ifdef __MED_BT_MOD__
    aud_bt_init();
#endif 
   aud_context_p->my_mod_id = MOD_MED;
   aud_context_p->state = AUD_IDLE;
   aud_context_p->startup = KAL_FALSE;
   return KAL_TRUE;
}





#endif

