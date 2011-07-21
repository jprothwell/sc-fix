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
 *   aud_api.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes primary exported functions of audio module.
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef MED_NOT_PRESENT


/*==== INCLUDES =========*/

/* system includes */
#include "kal_release.h" 
#include "stack_common.h"  
#include "stack_config.h"
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
#include "med_main.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "aud_main.h"
#include "med_main.h"

#include "media_others.h"
#include "mmi_trace.h"	

kal_int32 aud_result;
static kal_uint32 aud_value;
static kal_int32 mma_status;
static kal_int32 mma_handle;
HANDLE g_audioSem;//chenhe,070828
HANDLE g_TTSSem;//wangrui
AudioStateData Audiostate = {AUD_EVT_NOWAIT,1};
/*==== FUNCTIONS ===========*/


/*************************************************************************
* FUNCTION
*	aud_set_result
*
* DESCRIPTION
*	This function is to set the result of audio request. 
*
* PARAMETERS
*	kal_int32 result
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_set_result(kal_int32 result)
{
   aud_result=result;
}

/*************************************************************************
* FUNCTION
*	aud_set_pair_results
*
* DESCRIPTION
*	This function is to set the result and the value of audio request. 
*
* PARAMETERS
*	kal_int32 result
*	kal_uint32 value
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_set_pair_results(kal_int32 result, kal_uint32 value)
{
   aud_result = result;
   aud_value = value; 
}

/*************************************************************************
* FUNCTION
*	media_aud_play_id
*
* DESCRIPTION
*	This function is to play audio ID. 
*
* PARAMETERS
*	module_type src_mod_id, void *id_param
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_play_id(module_type src_mod_id, void *id_param)
{
   aud_result=MED_RES_OK;
   AUD_INIT_EVENT();
   aud_send_play_id_req(src_mod_id, id_param);
   AUD_WAIT_EVENT(AUD_EVT_PLAY);
   return aud_result;
}



/*************************************************************************
* FUNCTION
*	media_aud_stop_id
*
* DESCRIPTION
*	This function is to stop audio ID. 
*
* PARAMETERS
*	module_type src_mod_id, kal_uint8 audio_id
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_stop_id(module_type src_mod_id, kal_uint8 audio_id)
{
   aud_result=MED_RES_OK;
   AUD_INIT_EVENT();
   aud_send_stop_id_req(src_mod_id, audio_id);
   AUD_WAIT_EVENT(AUD_EVT_STOP);
   return aud_result;
}

/*************************************************************************
* FUNCTION
*	media_aud_play_string
*
* DESCRIPTION
*	This function is to play audio string. 
*
* PARAMETERS
*	module_type src_mod_id, void *string_param
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_play_string(module_type src_mod_id, void *string_param)
{
  // aud_play_string_struct *data_p=(aud_play_string_struct*)string_param;
   aud_result=MED_RES_OK;
  // if(data_p->blocking) //comment by chenhe,we always use semaphore,not block this wait
   	AUD_INIT_EVENT();
   aud_send_play_string_req(src_mod_id, string_param);
     // MMI_ASSERT(0);

  // if(data_p->blocking)//comment by chenhe,we always use semaphore,not block this wait
      AUD_WAIT_EVENT(AUD_EVT_PLAY);
   return aud_result;
}

/*************************************************************************
* FUNCTION
*	media_aud_stop_string
*
* DESCRIPTION
*	This function is to stop audio string. 
*
* PARAMETERS
*	module_type src_mod_id, kal_uint8 src_id
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_stop_string(module_type src_mod_id, kal_uint8 src_id)
{
   aud_result=MED_RES_OK;
   AUD_INIT_EVENT();
   aud_send_stop_string_req(src_mod_id, src_id);
   AUD_WAIT_EVENT(AUD_EVT_STOP);
   return aud_result;
}

/*************************************************************************
* FUNCTION
*	media_aud_play_file
*
* DESCRIPTION
*	This function is to play audio file. 
*
* PARAMETERS
*	module_type src_mod_id, void *file_param
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_play_file(module_type src_mod_id, void *file_param)
{
   aud_result=MED_RES_OK;
   AUD_INIT_EVENT();
   aud_send_play_file_req(src_mod_id, file_param);
   AUD_WAIT_EVENT(AUD_EVT_PLAY);
   return aud_result;
}

/*************************************************************************
* FUNCTION
*	media_aud_get_file_data_length
*
* DESCRIPTION
*	This function is to get the audio file data length. 
*
* PARAMETERS
*	module_type src_mod_id, void* file_name, kal_uint32* length
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_get_file_data_length( module_type src_mod_id, void* file_name,kal_uint32 PlayProgress,kal_uint32* length )
{
   aud_result = MED_RES_OK;
   *length = 0;
   AUD_INIT_EVENT();
   aud_send_get_file_data_length_req( src_mod_id, file_name,PlayProgress);
   AUD_WAIT_EVENT(AUD_EVT_GET_FILE_DATA_LEN);  
   *length = aud_value;
   return aud_result;
}

/*************************************************************************
* FUNCTION
*	media_aud_get_file_data_progress
*
* DESCRIPTION
*	This function is to get the progress of playing audio file
*
* PARAMETERS
*	module_type src_mod_id, kal_uint32* progress
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_get_file_data_progress( module_type src_mod_id, kal_uint32* progress )
{
   aud_result = MED_RES_OK;
   *progress = 0;
   AUD_INIT_EVENT();
   aud_send_get_file_data_progress_req( src_mod_id );
   AUD_WAIT_EVENT( AUD_EVT_GET_FILE_DATA_PROGRESS );
   *progress = aud_value;
   return aud_result;
}

/*************************************************************************
* FUNCTION
*	media_aud_set_file_data_progress
*
* DESCRIPTION
*	This function is to set the progress of playing audio file
*
* PARAMETERS
*	module_type src_mod_id, kal_uint32 progress
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_set_file_data_progress( module_type src_mod_id, kal_uint32 progress ,kal_uint32* getDuration)
{
   aud_result = MED_RES_OK;
   *getDuration = 0;
   AUD_INIT_EVENT();
   aud_send_set_file_data_progress_req( src_mod_id, progress );
   AUD_WAIT_EVENT( AUD_EVT_SET_FILE_DATA_PROGRESS );
   *getDuration = aud_value;
   return aud_result;
}

/*************************************************************************
* FUNCTION
*	media_aud_stop_file
*
* DESCRIPTION
*	This function is to stop audio file. 
*
* PARAMETERS
*	module_type src_mod_id, kal_uint8 src_id
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_stop_file(module_type src_mod_id, kal_uint8 src_id)
{
   aud_result=MED_RES_OK;
   AUD_INIT_EVENT();
   aud_send_stop_file_req(src_mod_id, src_id);
   AUD_WAIT_EVENT(AUD_EVT_STOP);
   return aud_result;
}

/*************************************************************************
* FUNCTION
*	media_aud_start_record
*
* DESCRIPTION
*	This function is to start recording audio. 
*
* PARAMETERS
*	module_type src_mod_id, kal_wchar* file_name, kal_uint8 format
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_start_record( module_type src_mod_id,kal_wchar *file_name,  kal_uint8 format,kal_uint8 quality,    kal_bool default_input,  kal_uint8 input_source)
//kal_int32 media_aud_start_record(module_type src_mod_id, kal_wchar* file_name, kal_uint8 format, kal_bool default_input,kal_uint8 input_source)
{
   aud_result=MED_RES_OK;
   AUD_INIT_EVENT();
   aud_send_record_req(src_mod_id, file_name, format,quality, default_input, input_source);
   AUD_WAIT_EVENT(AUD_EVT_RECORD);
   return aud_result;
}

/*************************************************************************
* FUNCTION
*	media_aud_stop_record
*
* DESCRIPTION
*	This function is to stop recording audio. 
*
* PARAMETERS
*	module_type src_mod_id, kal_uint8 src_id
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_stop_record(module_type src_mod_id, kal_uint8 src_id)
{
   aud_result=MED_RES_OK;
   AUD_INIT_EVENT();
   aud_send_stop_record_req(src_mod_id, src_id);
   AUD_WAIT_EVENT(AUD_EVT_STOP);
   return aud_result;
}

/*************************************************************************
* FUNCTION
*	media_aud_pause
*
* DESCRIPTION
*	This function is to pause audio. 
*
* PARAMETERS
*	module_type src_mod_id, kal_uint8 src_id
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_pause(module_type src_mod_id, kal_uint8 src_id)
{
   aud_result=MED_RES_OK;
   AUD_INIT_EVENT();
   aud_send_pause_req(src_mod_id, src_id);
   AUD_WAIT_EVENT(AUD_EVT_PAUSE);
   return aud_result;
}

/*************************************************************************
* FUNCTION
*	media_aud_resume
*
* DESCRIPTION
*	This function is to resume audio. 
*
* PARAMETERS
*	module_type src_mod_id, kal_uint8 src_id
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_resume(module_type src_mod_id, kal_uint8 src_id)
{
   aud_result=MED_RES_OK;
   AUD_INIT_EVENT();
   aud_send_resume_req(src_mod_id, src_id);
   AUD_WAIT_EVENT(AUD_EVT_RESUME);
   return aud_result;
}

/*************************************************************************
* FUNCTION
*	media_aud_store
*
* DESCRIPTION
*	This function is to pause audio and store play info. 
*
* PARAMETERS
*	module_type src_mod_id, kal_uint8 src_id
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_store(module_type src_mod_id, kal_uint8 src_id)
{
   aud_result=MED_RES_OK;
   AUD_INIT_EVENT();
   aud_send_store_req(src_mod_id, src_id);
   AUD_WAIT_EVENT(AUD_EVT_PAUSE);
   return aud_result;
}

/*************************************************************************
* FUNCTION
*	media_aud_restore
*
* DESCRIPTION
*	This function is to resume audio. 
*
* PARAMETERS
*	module_type src_mod_id, kal_uint8 src_id
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_restore(module_type src_mod_id, void *file_param)
{
   aud_result=MED_RES_OK;
   AUD_INIT_EVENT();
   aud_send_restore_req(src_mod_id, file_param);
   AUD_WAIT_EVENT(AUD_EVT_RESUME);
   return aud_result;
}


/*************************************************************************
* FUNCTION
*	media_aud_block
*
* DESCRIPTION
*	This function is to block access to audio functions from all module except the given one. 
*
* PARAMETERS
*	module_type src_mod_id, kal_uint16 mod_id, kal_uint8 level
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_block(module_type src_mod_id, kal_uint16 mod_id, kal_uint8 level)
{
   aud_result=MED_RES_OK;
   AUD_INIT_EVENT();
   aud_send_block_req(src_mod_id, mod_id, level);
   AUD_WAIT_EVENT(AUD_EVT_BLOCK);
   return aud_result;
}

/*************************************************************************
* FUNCTION
*	media_aud_set_headset_mode_output_path
*
* DESCRIPTION
*	This function is to set headset mode output path. 
*
* PARAMETERS
*	module_type src_mod_id, kal_uint8 device
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_set_headset_mode_output_path(module_type src_mod_id, kal_uint8 device)
{
   aud_result=MED_RES_OK;
   aud_send_set_headset_mode_output_path_req(src_mod_id, device);
   return aud_result;
}

/*************************************************************************
* FUNCTION
*	media_aud_set_vibrator_enabled
*
* DESCRIPTION
*	This function is to set vibrator enabled
*
* PARAMETERS
*	module_type src_mod_id, kal_uint8 enable
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_set_vibrator_enabled( module_type src_mod_id, kal_uint8 enable )
{
   aud_send_set_vibrator_enabled_req( src_mod_id, enable );
   return MED_RES_OK;
}

/*************************************************************************
* FUNCTION
*	media_aud_set_equalizer
*
* DESCRIPTION
*	This function is to set equalizer
*
* PARAMETERS
*	module_type src_mod_id, kal_uint8* magnitude
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_aud_set_equalizer(module_type src_mod_id, kal_int8 *magnitude)
{
   aud_send_set_equalizer_req( src_mod_id, magnitude );
   return MED_RES_OK;
}

kal_int32 media_aud_fmr_power_on(module_type src_mod_id, kal_uint8 output_path)
{
   aud_result=MED_RES_OK;
   AUD_INIT_EVENT();
   aud_send_fmr_power_on_req(src_mod_id, output_path);
   AUD_WAIT_EVENT(AUD_EVT_FMR_ON);
   return aud_result;
}

kal_int32 media_aud_fmr_power_off(module_type src_mod_id)
{
   aud_result=MED_RES_OK;
   AUD_INIT_EVENT();
   aud_send_fmr_power_off_req(src_mod_id);
   AUD_WAIT_EVENT(AUD_EVT_FMR_OFF);
   return aud_result;
}

kal_int32 media_aud_fmr_set_freq(module_type src_mod_id, kal_uint16 freq)
{
   aud_result=MED_RES_OK;
   AUD_INIT_EVENT();
   aud_send_fmr_set_freq_req(src_mod_id, freq);
   AUD_WAIT_EVENT(AUD_EVT_FMR_SET);
   return aud_result;
}

kal_int32 media_aud_fmr_mute(module_type src_mod_id, kal_uint8 mute)
{
   aud_result=MED_RES_OK;
   AUD_INIT_EVENT();
   aud_send_fmr_mute_req(src_mod_id, mute);
   AUD_WAIT_EVENT(AUD_EVT_FMR_MUTE);
   return aud_result;
}

#ifdef __MED_MMA_MOD__
/********** MMA *****************/

/*************************************************************************
* FUNCTION
*	mma_set_status
*
* DESCRIPTION
*	This function is set the mma status. 
*
* PARAMETERS
*	kal_int32 status
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void mma_set_status(kal_int32 status)
{
   mma_status=status;
}

/*************************************************************************
* FUNCTION
*	mma_set_handle
*
* DESCRIPTION
*	This function is set the mma media handle. 
*
* PARAMETERS
*	kal_int32 handle
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void mma_set_handle(kal_int32 handle)
{
   mma_handle=handle;
}


/******** MIDI *********/

/*************************************************************************
* FUNCTION
*	media_mma_midi_open
*
* DESCRIPTION
*	This function is to open midi. 
*
* PARAMETERS
*	kal_int32 handler
*  mma_player_struct* param
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_open(med_handler handler, mma_player_struct* param)
{
   aud_send_mma_open_req(stack_int_get_active_module_id(),
                              MMA_TYPE_MIDI, 
                              (kal_int32)handler, 
                              (kal_int32)param);
   MMA_WAIT_EVENT(MMA_EVT_OPEN);
   return mma_handle;
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_close
*
* DESCRIPTION
*	This function is to close midi. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_close(kal_int32 handle)
{
   aud_send_mma_close_req(stack_int_get_active_module_id(),
                              MMA_TYPE_MIDI, 
                              handle);
   MMA_WAIT_EVENT(MMA_EVT_CLOSE);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_play
*
* DESCRIPTION
*	This function is to play midi. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_play(kal_int32 handle)
{
   aud_send_mma_play_req(stack_int_get_active_module_id(),
                           MMA_TYPE_MIDI, 
                           handle);
   MMA_WAIT_EVENT(MMA_EVT_PLAY);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_stop
*
* DESCRIPTION
*	This function is to stop midi. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_stop(kal_int32 handle)
{
   aud_send_mma_stop_req(stack_int_get_active_module_id(),
                            MMA_TYPE_MIDI, handle);
   MMA_WAIT_EVENT(MMA_EVT_STOP);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_pause
*
* DESCRIPTION
*	This function is to pause midi. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_pause(kal_int32 handle)
{
   aud_send_mma_pause_req(stack_int_get_active_module_id(),
                            MMA_TYPE_MIDI, handle);
   MMA_WAIT_EVENT(MMA_EVT_PAUSE);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_resume
*
* DESCRIPTION
*	This function is to resume midi. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_resume(kal_int32 handle)
{
   aud_send_mma_resume_req(stack_int_get_active_module_id(),
                            MMA_TYPE_MIDI, handle);
   MMA_WAIT_EVENT(MMA_EVT_RESUME);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_set_start_time
*
* DESCRIPTION
*	This function is to set start time of midi. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_int32 start_time
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_set_start_time(kal_int32 handle, kal_int32 start_time)
{
   aud_send_mma_set_start_time_req(stack_int_get_active_module_id(),
                            MMA_TYPE_MIDI, handle, start_time);
   MMA_WAIT_EVENT(MMA_EVT_SET_START_TIME);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_set_stop_time
*
* DESCRIPTION
*	This function is to set stop time of midi. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_int32 stop_time
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_set_stop_time(kal_int32 handle, kal_int32 stop_time)
{
   aud_send_mma_set_stop_time_req(stack_int_get_active_module_id(), MMA_TYPE_MIDI, handle, stop_time);
   MMA_WAIT_EVENT(MMA_EVT_SET_STOP_TIME);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_get_current_time
*
* DESCRIPTION
*	This function is to get current time of midi. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_get_current_time(kal_int32 handle)
{
   return JSmf_GetCurrentTime((Media_Handle*)handle);
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_get_duration
*
* DESCRIPTION
*	This function is to get duration of midi. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_get_duration(kal_int32 handle)
{
   return JSmf_GetDuration((Media_Handle*)handle);
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_get_duration_by_string
*
* DESCRIPTION
*	This function is to get duration of midi by given midi data string. 
*
* PARAMETERS
*	const kal_uint8 *data
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_get_duration_by_string(const kal_uint8 *data)
{
   return JSmf_GetMidiDuration(data);
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_get_volume
*
* DESCRIPTION
*	This function is to get volume of midi. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_uint8
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_uint8 media_mma_midi_get_volume(kal_int32 handle)
{
   return JMidi_GetLevel((Media_Handle*)handle);
}


/*************************************************************************
* FUNCTION
*	media_mma_midi_set_volume
*
* DESCRIPTION
*	This function is to set volume of midi. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_uint8 volume
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_set_volume(kal_int32 handle, kal_uint8 volume)
{
   aud_send_mma_set_volume_req(stack_int_get_active_module_id(), MMA_TYPE_MIDI, handle, volume);
   MMA_WAIT_EVENT(MMA_EVT_SET_VOLUME);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_get_rate
*
* DESCRIPTION
*	This function is to get rate of midi. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_get_rate(kal_int32 handle)
{
   return JMidi_GetRate((Media_Handle*)handle);
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_set_rate
*
* DESCRIPTION
*	This function is to set rate of midi. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_int32 rate
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_set_rate(kal_int32 handle, kal_int32 rate)
{
   aud_send_mma_set_rate_req(stack_int_get_active_module_id(), MMA_TYPE_MIDI, handle, rate);
   MMA_WAIT_EVENT(MMA_EVT_SET_RATE);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_get_tempo
*
* DESCRIPTION
*	This function is to get tempo of midi. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_get_tempo(kal_int32 handle)
{
   return JMidi_GetTempo((Media_Handle*)handle);
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_set_tempo
*
* DESCRIPTION
*	This function is to set tempo of midi. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_int32 tempo
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_set_tempo(kal_int32 handle, kal_int32 tempo)
{
   aud_send_mma_set_tempo_req(stack_int_get_active_module_id(), MMA_TYPE_MIDI, handle, tempo);
   MMA_WAIT_EVENT(MMA_EVT_SET_TEMPO);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_get_pitch
*
* DESCRIPTION
*	This function is to get pitch of midi. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_get_pitch(kal_int32 handle)
{
   return JMidi_GetPitchTranspose((Media_Handle*)handle);
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_set_pitch
*
* DESCRIPTION
*	This function is to set pitch of midi. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_int32 pitch
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_set_pitch(kal_int32 handle, kal_int32 pitch)
{
   aud_send_mma_set_pitch_req(stack_int_get_active_module_id(), MMA_TYPE_MIDI, handle, pitch);
   MMA_WAIT_EVENT(MMA_EVT_SET_PITCH);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_open_device
*
* DESCRIPTION
*	This function is to open device of midi. 
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
kal_int32 media_mma_midi_open_device(void)
{
   aud_send_mma_open_device_req(MOD_J2ME);
   MMA_WAIT_EVENT(MMA_EVT_OPEN_DEVICE);
   return mma_handle;
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_close_device
*
* DESCRIPTION
*	This function is to close device of midi. 
*
* PARAMETERS
*	kal_int32 handle
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_close_device(kal_int32 handle)
{
   aud_send_mma_close_device_req(stack_int_get_active_module_id(), handle);
   MMA_WAIT_EVENT(MMA_EVT_CLOSE_DEVICE);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_send_long_msg
*
* DESCRIPTION
*	This function is to send long msg of midi. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_uint8* buf, kal_uint16 len
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_send_long_msg(kal_int32 handle, kal_uint8* buf, kal_uint16 len)
{
   aud_send_mma_send_long_msg_req(stack_int_get_active_module_id(), handle, buf, len);
   MMA_WAIT_EVENT(MMA_EVT_SEND_LONG_MSG);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_send_short_msg
*
* DESCRIPTION
*	This function is to send short msg of midi. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_uint8 type, kal_uint8 data1, kal_uint8 data2
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_send_short_msg(kal_int32 handle, kal_uint8 type, kal_uint8 data1, kal_uint8 data2)
{
   aud_send_mma_send_short_msg_req(stack_int_get_active_module_id(), handle, type, data1, data2);
   MMA_WAIT_EVENT(MMA_EVT_SEND_SHORT_MSG);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_get_channel_volume
*
* DESCRIPTION
*	This function is to get channel volume of midi. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_uint8 channel
* RETURNS
*	kal_uint8
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_uint8 media_mma_midi_get_channel_volume(kal_int32 handle, kal_uint8 channel)
{
   return JMidi_GetChannelVolume((Media_Handle*)handle, channel);
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_set_channel_volume
*
* DESCRIPTION
*	This function is to set channel volume of midi. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_uint8 channel, kal_uint8 level
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_set_channel_volume(kal_int32 handle, kal_uint8 channel, kal_uint8 level)
{
   aud_send_mma_set_channel_volume_req(stack_int_get_active_module_id(), handle, channel, level);
   MMA_WAIT_EVENT(MMA_EVT_SET_CHANNEL_VOLUME);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_get_program
*
* DESCRIPTION
*	This function is to get program of midi. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_uint8 channel, kal_int16* bank, kal_int8* program
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_mma_midi_get_program(kal_int32 handle, kal_uint8 channel, kal_int16* bank, kal_int8* program)
{
   JMidi_GetProgram((Media_Handle*)handle, channel, bank, program);
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_set_program
*
* DESCRIPTION
*	This function is to set program of midi. 
*
* PARAMETERS
*	kal_int32 handle
*  kal_uint8 channel, kal_uint8 bank, kal_uint8 program
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_midi_set_program(kal_int32 handle, kal_uint8 channel, kal_uint8 bank, kal_uint8 program)
{
   aud_send_mma_set_program_req(stack_int_get_active_module_id(), handle, channel, bank, program);
   MMA_WAIT_EVENT(MMA_EVT_SET_PROGRAM);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_bank_support
*
* DESCRIPTION
*	This function is to check if bank query supported. 
*
* PARAMETERS
*	kal_int32 handle
*  
* RETURNS
*	kal_bool
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_bool media_mma_midi_bank_support(kal_int32 handle)
{
   return JMidi_isBankQuerySupported();
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_get_bank_list
*
* DESCRIPTION
*	This function is to get the bank list of midi. 
*
* PARAMETERS
*	kal_int32 handle
*  kal_bool custom, const kal_int16 **buf, kal_int16 *len
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_mma_midi_get_bank_list(kal_int32 handle, kal_bool custom, const kal_int16 **buf, kal_int16 *len)
{
   JMidi_GetBankList(custom, buf, len);
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_get_program_list
*
* DESCRIPTION
*	This function is to get the program list of midi. 
*
* PARAMETERS
*	kal_int32 handle
*  kal_int16 bank, const kal_int8 **buf, kal_int16 *len
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void media_mma_midi_get_program_list(kal_int32 handle, kal_int16 bank, const kal_int8 **buf, kal_int16 *len)
{
   JMidi_GetProgramList(bank, buf, len);
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_get_program_name
*
* DESCRIPTION
*	This function is to get the program name of midi. 
*
* PARAMETERS
*	kal_int32 handle
*  kal_int16 bank, const kal_int8 **buf, kal_int16 *len
* RETURNS
*	const kal_uint8*
*
* GLOBALS AFFECTED
*
*************************************************************************/
const kal_uint8* media_mma_midi_get_program_name(kal_int32 handle, kal_int16 bank, kal_int8 prog)
{
   return JMidi_GetProgramName(bank, prog);
}

/*************************************************************************
* FUNCTION
*	media_mma_midi_get_key_name
*
* DESCRIPTION
*	This function is to get the key name of midi. 
*
* PARAMETERS
*	kal_int32 handle
*  kal_int16 bank, const kal_int8 **buf, kal_int16 *len
* RETURNS
*	const kal_uint8*
*
* GLOBALS AFFECTED
*
*************************************************************************/
const kal_uint8* media_mma_midi_get_key_name(kal_int32 handle, kal_int16 bank, kal_int8 prog, kal_int8 key)
{
   return JMidi_GetKeyName(bank, prog, key);
}

/******** TONE *********/

/*************************************************************************
* FUNCTION
*	media_mma_tone_open
*
* DESCRIPTION
*	This function is to open tone. 
*
* PARAMETERS
*	kal_int32 handler
*  mma_player_struct* param
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_tone_open(med_handler handler, mma_player_struct* param)
{
   aud_send_mma_open_req(stack_int_get_active_module_id(), MMA_TYPE_TONE, (kal_int32)handler, (kal_int32)param);
   MMA_WAIT_EVENT(MMA_EVT_OPEN);
   return mma_handle;
}

/*************************************************************************
* FUNCTION
*	media_mma_tone_close
*
* DESCRIPTION
*	This function is to close tone. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_tone_close(kal_int32 handle)
{
   aud_send_mma_close_req(stack_int_get_active_module_id(), MMA_TYPE_TONE, handle);
   MMA_WAIT_EVENT(MMA_EVT_CLOSE);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_tone_play
*
* DESCRIPTION
*	This function is to play tone. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_tone_play(kal_int32 handle)
{
   aud_send_mma_play_req(stack_int_get_active_module_id(), MMA_TYPE_TONE, handle);
   MMA_WAIT_EVENT(MMA_EVT_PLAY);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_tone_stop
*
* DESCRIPTION
*	This function is to stop tone. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_tone_stop(kal_int32 handle)
{
   aud_send_mma_stop_req(stack_int_get_active_module_id(), MMA_TYPE_TONE, handle);
   MMA_WAIT_EVENT(MMA_EVT_STOP);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_tone_pause
*
* DESCRIPTION
*	This function is to pause tone. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_tone_pause(kal_int32 handle)
{
   aud_send_mma_pause_req(stack_int_get_active_module_id(), MMA_TYPE_TONE, handle);
   MMA_WAIT_EVENT(MMA_EVT_PAUSE);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_tone_resume
*
* DESCRIPTION
*	This function is to resume tone. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_tone_resume(kal_int32 handle)
{
   aud_send_mma_resume_req(stack_int_get_active_module_id(), MMA_TYPE_TONE, handle);
   MMA_WAIT_EVENT(MMA_EVT_RESUME);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_tone_set_data
*
* DESCRIPTION
*	This function is to set data of tone. 
*
* PARAMETERS
*	kal_int32 handle
*  mma_player_struct* param
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_tone_set_data(kal_int32 handle, mma_player_struct* param)
{
   aud_send_mma_set_data_req(stack_int_get_active_module_id(), MMA_TYPE_TONE, handle, (kal_int32)param);
   MMA_WAIT_EVENT(MMA_EVT_SET_DATA);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_tone_set_start_time
*
* DESCRIPTION
*	This function is to set start time of tone. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_int32 start_time
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_tone_set_start_time(kal_int32 handle, kal_int32 start_time)
{
   aud_send_mma_set_start_time_req(stack_int_get_active_module_id(), MMA_TYPE_TONE, handle, start_time);
   MMA_WAIT_EVENT(MMA_EVT_SET_START_TIME);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_tone_set_stop_time
*
* DESCRIPTION
*	This function is to set stop time of tone. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_int32 stop_time
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_tone_set_stop_time(kal_int32 handle, kal_int32 stop_time)
{
   aud_send_mma_set_stop_time_req(stack_int_get_active_module_id(), MMA_TYPE_TONE, handle, stop_time);
   MMA_WAIT_EVENT(MMA_EVT_SET_STOP_TIME);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_tone_get_current_time
*
* DESCRIPTION
*	This function is to get current time of tone. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_tone_get_current_time(kal_int32 handle)
{
   return JTone_GetCurrentTime((Media_Handle*)handle);
}


/*************************************************************************
* FUNCTION
*	media_mma_tone_get_duration
*
* DESCRIPTION
*	This function is to get duration of tone. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_tone_get_duration(kal_int32 handle)
{
   return JTone_GetDuration((Media_Handle*)handle);
}


/*************************************************************************
* FUNCTION
*	media_mma_tone_get_volume
*
* DESCRIPTION
*	This function is to get volume of tone. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_uint8
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_uint8 media_mma_tone_get_volume(kal_int32 handle)
{
   return JTone_GetLevel((Media_Handle*)handle);
}


/*************************************************************************
* FUNCTION
*	media_mma_tone_set_volume
*
* DESCRIPTION
*	This function is to set volume of tone. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_tone_set_volume(kal_int32 handle, kal_uint8 volume)
{
   aud_send_mma_set_volume_req(stack_int_get_active_module_id(), MMA_TYPE_TONE, handle, volume);
   MMA_WAIT_EVENT(MMA_EVT_SET_VOLUME);
   return mma_status;
}


/*************************************************************************
* FUNCTION
*	media_mma_tone_get_pitch
*
* DESCRIPTION
*	This function is to get pitch of tone. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_int8
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int8 media_mma_tone_get_pitch(kal_int32 handle)
{
   return JTone_GetPitchShift((Media_Handle*)handle);
}


/*************************************************************************
* FUNCTION
*	media_mma_tone_set_pitch
*
* DESCRIPTION
*	This function is to set pitch of tone. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_int32 pitch
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_tone_set_pitch(kal_int32 handle, kal_int32 pitch)
{
   aud_send_mma_set_pitch_req(stack_int_get_active_module_id(), MMA_TYPE_TONE, handle, pitch);
   MMA_WAIT_EVENT(MMA_EVT_SET_PITCH);
   return mma_status;
}


/******** WAV *********/

/*************************************************************************
* FUNCTION
*	media_mma_wav_open
*
* DESCRIPTION
*	This function is to open wav. 
*
* PARAMETERS
*	kal_int32 handler
*  mma_player_struct* param
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_wav_open(med_handler handler, mma_player_struct* param)
{
   aud_send_mma_open_req(stack_int_get_active_module_id(), MMA_TYPE_WAV, (kal_int32)handler, (kal_int32)param);
   MMA_WAIT_EVENT(MMA_EVT_OPEN);
   return mma_handle;
}

/*************************************************************************
* FUNCTION
*	media_mma_wav_close
*
* DESCRIPTION
*	This function is to close wav. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_wav_close(kal_int32 handle)
{
   aud_send_mma_close_req(stack_int_get_active_module_id(), MMA_TYPE_WAV, handle);
   MMA_WAIT_EVENT(MMA_EVT_CLOSE);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_wav_play
*
* DESCRIPTION
*	This function is to play wav. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_wav_play(kal_int32 handle)
{
   aud_send_mma_play_req(stack_int_get_active_module_id(), MMA_TYPE_WAV, handle);
   MMA_WAIT_EVENT(MMA_EVT_PLAY);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_wav_stop
*
* DESCRIPTION
*	This function is to stop wav. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_wav_stop(kal_int32 handle)
{
   aud_send_mma_stop_req(stack_int_get_active_module_id(), MMA_TYPE_WAV, handle);
   MMA_WAIT_EVENT(MMA_EVT_STOP);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_wav_pause
*
* DESCRIPTION
*	This function is to pause wav. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_wav_pause(kal_int32 handle)
{
   aud_send_mma_pause_req(stack_int_get_active_module_id(), MMA_TYPE_WAV, handle);
   MMA_WAIT_EVENT(MMA_EVT_PAUSE);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_wav_resume
*
* DESCRIPTION
*	This function is to resume wav. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_wav_resume(kal_int32 handle)
{
   aud_send_mma_resume_req(stack_int_get_active_module_id(), MMA_TYPE_WAV, handle);
   MMA_WAIT_EVENT(MMA_EVT_RESUME);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_wav_set_start_time
*
* DESCRIPTION
*	This function is to set start time of wav. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_int32 start_time
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_wav_set_start_time(kal_int32 handle, kal_int32 start_time)
{
   aud_send_mma_set_start_time_req(stack_int_get_active_module_id(), MMA_TYPE_WAV, handle, start_time);
   MMA_WAIT_EVENT(MMA_EVT_SET_START_TIME);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_wav_set_stop_time
*
* DESCRIPTION
*	This function is to set stop time of wav. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_int32 stop_time
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_wav_set_stop_time(kal_int32 handle, kal_int32 stop_time)
{
   aud_send_mma_set_stop_time_req(stack_int_get_active_module_id(), MMA_TYPE_WAV, handle, stop_time);
   MMA_WAIT_EVENT(MMA_EVT_SET_STOP_TIME);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_wav_get_current_time
*
* DESCRIPTION
*	This function is to get current time of wave. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_wav_get_current_time(kal_int32 handle)
{
   return JWav_GetCurrentTime((Media_Handle*)handle);
}


/*************************************************************************
* FUNCTION
*	media_mma_wav_get_duration
*
* DESCRIPTION
*	This function is to get duration of wave. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_wav_get_duration(kal_int32 handle)
{
   return JWav_GetDuration((Media_Handle*)handle);
}


/*************************************************************************
* FUNCTION
*	media_mma_wav_get_volume
*
* DESCRIPTION
*	This function is to get volume of tone. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_uint8
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_uint8 media_mma_wav_get_volume(kal_int32 handle)
{
   return JWav_GetLevel((Media_Handle*)handle);
}



/*************************************************************************
* FUNCTION
*	media_mma_wav_set_volume
*
* DESCRIPTION
*	This function is to set volume of wav. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_wav_set_volume(kal_int32 handle, kal_uint8 volume)
{
   aud_send_mma_set_volume_req(stack_int_get_active_module_id(), MMA_TYPE_WAV, handle, volume);
   MMA_WAIT_EVENT(MMA_EVT_SET_VOLUME);
   return mma_status;
}

/******** IMY *********/

/*************************************************************************
* FUNCTION
*	media_mma_imy_open
*
* DESCRIPTION
*	This function is to open imy. 
*
* PARAMETERS
*	kal_int32 handler
*  mma_player_struct* param
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_imy_open(med_handler handler, mma_player_struct* param)
{
   aud_send_mma_open_req(stack_int_get_active_module_id(), MMA_TYPE_IMY, (kal_int32)handler, (kal_int32)param);
   MMA_WAIT_EVENT(MMA_EVT_OPEN);
   return mma_handle;
}

/*************************************************************************
* FUNCTION
*	media_mma_imy_close
*
* DESCRIPTION
*	This function is to close imy. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_imy_close(kal_int32 handle)
{
   aud_send_mma_close_req(stack_int_get_active_module_id(), MMA_TYPE_IMY, handle);
   MMA_WAIT_EVENT(MMA_EVT_CLOSE);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_imy_play
*
* DESCRIPTION
*	This function is to play imy. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_imy_play(kal_int32 handle)
{
   aud_send_mma_play_req(stack_int_get_active_module_id(), MMA_TYPE_IMY, handle);
   MMA_WAIT_EVENT(MMA_EVT_PLAY);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_imy_stop
*
* DESCRIPTION
*	This function is to stop imy. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_imy_stop(kal_int32 handle)
{
   aud_send_mma_stop_req(stack_int_get_active_module_id(), MMA_TYPE_IMY, handle);
   MMA_WAIT_EVENT(MMA_EVT_STOP);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_imy_pause
*
* DESCRIPTION
*	This function is to pause imy. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_imy_pause(kal_int32 handle)
{
   aud_send_mma_pause_req(stack_int_get_active_module_id(), MMA_TYPE_IMY, handle);
   MMA_WAIT_EVENT(MMA_EVT_PAUSE);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_imy_resume
*
* DESCRIPTION
*	This function is to resume imy. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_imy_resume(kal_int32 handle)
{
   aud_send_mma_resume_req(stack_int_get_active_module_id(), MMA_TYPE_IMY, handle);
   MMA_WAIT_EVENT(MMA_EVT_RESUME);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_imy_set_start_time
*
* DESCRIPTION
*	This function is to set start time of imy. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_int32 start_time
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_imy_set_start_time(kal_int32 handle, kal_int32 start_time)
{
   aud_send_mma_set_start_time_req(stack_int_get_active_module_id(), MMA_TYPE_IMY, handle, start_time);
   MMA_WAIT_EVENT(MMA_EVT_SET_START_TIME);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_imy_set_stop_time
*
* DESCRIPTION
*	This function is to set stop time of imy. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_int32 stop_time
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_imy_set_stop_time(kal_int32 handle, kal_int32 stop_time)
{
   aud_send_mma_set_stop_time_req(stack_int_get_active_module_id(), MMA_TYPE_IMY, handle, stop_time);
   MMA_WAIT_EVENT(MMA_EVT_SET_STOP_TIME);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_imy_get_current_time
*
* DESCRIPTION
*	This function is to get current time of imy. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_imy_get_current_time(kal_int32 handle)
{
   return JImy_GetCurrentTime((Media_Handle*)handle);
}

/*************************************************************************
* FUNCTION
*	media_mma_imy_get_duration
*
* DESCRIPTION
*	This function is to get duration of imy. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_imy_get_duration(kal_int32 handle)
{
   return JImy_GetDuration((Media_Handle*)handle);
}

/*************************************************************************
* FUNCTION
*	media_mma_imy_get_volume
*
* DESCRIPTION
*	This function is to get volume of imy. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_uint8
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_uint8 media_mma_imy_get_volume(kal_int32 handle)
{
   return JImy_GetLevel((Media_Handle*)handle);
}


/*************************************************************************
* FUNCTION
*	media_mma_imy_set_volume
*
* DESCRIPTION
*	This function is to set volume of imy. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_uint8 volume
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_imy_set_volume(kal_int32 handle, kal_uint8 volume)
{
   aud_send_mma_set_volume_req(stack_int_get_active_module_id(), MMA_TYPE_IMY, handle, volume);
   MMA_WAIT_EVENT(MMA_EVT_SET_VOLUME);
   return mma_status;
}

/******** AMR *********/

/*************************************************************************
* FUNCTION
*	media_mma_amr_open
*
* DESCRIPTION
*	This function is to open amr. 
*
* PARAMETERS
*	kal_int32 handler
*  mma_player_struct* param
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_amr_open(med_handler handler, mma_player_struct* param)
{
   aud_send_mma_open_req(stack_int_get_active_module_id(), MMA_TYPE_AMR, (kal_int32)handler, (kal_int32)param);
   MMA_WAIT_EVENT(MMA_EVT_OPEN);
   return mma_handle;
}

/*************************************************************************
* FUNCTION
*	media_mma_amr_close
*
* DESCRIPTION
*	This function is to close amr. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_amr_close(kal_int32 handle)
{
   aud_send_mma_close_req(stack_int_get_active_module_id(), MMA_TYPE_AMR, handle);
   MMA_WAIT_EVENT(MMA_EVT_CLOSE);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_amr_play
*
* DESCRIPTION
*	This function is to play amr. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_amr_play(kal_int32 handle)
{
   aud_send_mma_play_req(stack_int_get_active_module_id(), MMA_TYPE_AMR, handle);
   MMA_WAIT_EVENT(MMA_EVT_PLAY);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_amr_stop
*
* DESCRIPTION
*	This function is to stop amr. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_amr_stop(kal_int32 handle)
{
   aud_send_mma_stop_req(stack_int_get_active_module_id(), MMA_TYPE_AMR, handle);
   MMA_WAIT_EVENT(MMA_EVT_STOP);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_amr_pause
*
* DESCRIPTION
*	This function is to pause amr. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_amr_pause(kal_int32 handle)
{
   aud_send_mma_pause_req(stack_int_get_active_module_id(), MMA_TYPE_AMR, handle);
   MMA_WAIT_EVENT(MMA_EVT_PAUSE);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_amr_resume
*
* DESCRIPTION
*	This function is to resume amr. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_amr_resume(kal_int32 handle)
{
   aud_send_mma_resume_req(stack_int_get_active_module_id(), MMA_TYPE_AMR, handle);
   MMA_WAIT_EVENT(MMA_EVT_RESUME);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_amr_set_start_time
*
* DESCRIPTION
*	This function is to set start time of amr. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_int32 start_time
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_amr_set_start_time(kal_int32 handle, kal_int32 start_time)
{
   aud_send_mma_set_start_time_req(stack_int_get_active_module_id(), MMA_TYPE_AMR, handle, start_time);
   MMA_WAIT_EVENT(MMA_EVT_SET_START_TIME);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_amr_set_stop_time
*
* DESCRIPTION
*	This function is to set stop time of amr. 
*
* PARAMETERS
*	kal_int32 handler
*  kal_int32 stop_time
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_amr_set_stop_time(kal_int32 handle, kal_int32 stop_time)
{
   aud_send_mma_set_stop_time_req(stack_int_get_active_module_id(), MMA_TYPE_AMR, handle, stop_time);
   MMA_WAIT_EVENT(MMA_EVT_SET_STOP_TIME);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_amr_get_current_time
*
* DESCRIPTION
*	This function is to get current time of amr. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_amr_get_current_time(kal_int32 handle)
{
#ifdef MMA_AMR_SUPPORT
   return JAmr_GetCurrentTime((Media_Handle*)handle);
#else
   return 0;
#endif
}


/*************************************************************************
* FUNCTION
*	media_mma_amr_get_duration
*
* DESCRIPTION
*	This function is to get duration of amr. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_int32
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_amr_get_duration(const kal_uint8 *data, kal_int32 size, kal_int32 *duration)
{
#ifdef MMA_AMR_SUPPORT
   return aud_get_res(JAmr_GetDuration(data,size,duration));
#else
   return 0;
#endif   
}


/*************************************************************************
* FUNCTION
*	media_mma_amr_get_volume
*
* DESCRIPTION
*	This function is to get volume of tone. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	kal_uint8
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_uint8 media_mma_amr_get_volume(kal_int32 handle)
{
#ifdef MMA_AMR_SUPPORT
   return JAmr_GetLevel((Media_Handle*)handle);
#else
   return 0;
#endif
}



/*************************************************************************
* FUNCTION
*	media_mma_amr_set_volume
*
* DESCRIPTION
*	This function is to set volume of amr. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_amr_set_volume(kal_int32 handle, kal_uint8 volume)
{
   aud_send_mma_set_volume_req(stack_int_get_active_module_id(), MMA_TYPE_AMR, handle, volume);
   MMA_WAIT_EVENT(MMA_EVT_SET_VOLUME);
   return mma_status;
}

/******** DAF *********/

/*************************************************************************
* FUNCTION
*	media_mma_daf_open
*
* DESCRIPTION
*	This function is to open daf. 
*
* PARAMETERS
*	kal_int32 handler
*  mma_player_struct* param
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_daf_open(med_handler handler, mma_player_struct* param)
{
   aud_send_mma_open_req(stack_int_get_active_module_id(), MMA_TYPE_DAF, (kal_int32)handler, (kal_int32)param);
   MMA_WAIT_EVENT(MMA_EVT_OPEN);
   return mma_handle;
}

/*************************************************************************
* FUNCTION
*	media_mma_daf_close
*
* DESCRIPTION
*	This function is to close daf. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_daf_close(kal_int32 handle)
{
   aud_send_mma_close_req(stack_int_get_active_module_id(), MMA_TYPE_DAF, handle);
   MMA_WAIT_EVENT(MMA_EVT_CLOSE);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_daf_play
*
* DESCRIPTION
*	This function is to play daf. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_daf_play(kal_int32 handle)
{
   aud_send_mma_play_req(stack_int_get_active_module_id(), MMA_TYPE_DAF, handle);
   MMA_WAIT_EVENT(MMA_EVT_PLAY);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_daf_stop
*
* DESCRIPTION
*	This function is to stop daf. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_daf_stop(kal_int32 handle)
{
   aud_send_mma_stop_req(stack_int_get_active_module_id(), MMA_TYPE_DAF, handle);
   MMA_WAIT_EVENT(MMA_EVT_STOP);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_daf_pause
*
* DESCRIPTION
*	This function is to pause daf. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_daf_pause(kal_int32 handle)
{
   aud_send_mma_pause_req(stack_int_get_active_module_id(), MMA_TYPE_DAF, handle);
   MMA_WAIT_EVENT(MMA_EVT_PAUSE);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_daf_resume
*
* DESCRIPTION
*	This function is to resume daf. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_daf_resume(kal_int32 handle)
{
   aud_send_mma_resume_req(stack_int_get_active_module_id(), MMA_TYPE_DAF, handle);
   MMA_WAIT_EVENT(MMA_EVT_RESUME);
   return mma_status;
}

/********* AAC **********/

/*************************************************************************
* FUNCTION
*	media_mma_aac_open
*
* DESCRIPTION
*	This function is to open aac. 
*
* PARAMETERS
*	kal_int32 handler
*  mma_player_struct* param
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_aac_open(med_handler handler, mma_player_struct* param)
{
   aud_send_mma_open_req(stack_int_get_active_module_id(), MMA_TYPE_AAC, (kal_int32)handler, (kal_int32)param);
   MMA_WAIT_EVENT(MMA_EVT_OPEN);
   return mma_handle;
}

/*************************************************************************
* FUNCTION
*	media_mma_aac_close
*
* DESCRIPTION
*	This function is to close aac. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_aac_close(kal_int32 handle)
{
   aud_send_mma_close_req(stack_int_get_active_module_id(), MMA_TYPE_AAC, handle);
   MMA_WAIT_EVENT(MMA_EVT_CLOSE);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_aac_play
*
* DESCRIPTION
*	This function is to play aac. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_aac_play(kal_int32 handle)
{
   aud_send_mma_play_req(stack_int_get_active_module_id(), MMA_TYPE_AAC, handle);
   MMA_WAIT_EVENT(MMA_EVT_PLAY);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_aac_stop
*
* DESCRIPTION
*	This function is to stop aac. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_aac_stop(kal_int32 handle)
{
   aud_send_mma_stop_req(stack_int_get_active_module_id(), MMA_TYPE_AAC, handle);
   MMA_WAIT_EVENT(MMA_EVT_STOP);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_aac_pause
*
* DESCRIPTION
*	This function is to pause aac. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_aac_pause(kal_int32 handle)
{
   aud_send_mma_pause_req(stack_int_get_active_module_id(), MMA_TYPE_AAC, handle);
   MMA_WAIT_EVENT(MMA_EVT_PAUSE);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_aac_resume
*
* DESCRIPTION
*	This function is to resume aac. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_aac_resume(kal_int32 handle)
{
   aud_send_mma_resume_req(stack_int_get_active_module_id(), MMA_TYPE_AAC, handle);
   MMA_WAIT_EVENT(MMA_EVT_RESUME);
   return mma_status;
}


/************ AU **************/

/*************************************************************************
* FUNCTION
*	media_mma_au_open
*
* DESCRIPTION
*	This function is to open au. 
*
* PARAMETERS
*	kal_int32 handler
*  mma_player_struct* param
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_au_open(med_handler handler, mma_player_struct* param)
{
   aud_send_mma_open_req(stack_int_get_active_module_id(), MMA_TYPE_AU, (kal_int32)handler, (kal_int32)param);
   MMA_WAIT_EVENT(MMA_EVT_OPEN);
   return mma_handle;
}

/*************************************************************************
* FUNCTION
*	media_mma_au_close
*
* DESCRIPTION
*	This function is to close au. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_au_close(kal_int32 handle)
{
   aud_send_mma_close_req(stack_int_get_active_module_id(), MMA_TYPE_AU, handle);
   MMA_WAIT_EVENT(MMA_EVT_CLOSE);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_au_play
*
* DESCRIPTION
*	This function is to play au. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_au_play(kal_int32 handle)
{
   aud_send_mma_play_req(stack_int_get_active_module_id(), MMA_TYPE_AU, handle);
   MMA_WAIT_EVENT(MMA_EVT_PLAY);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_au_stop
*
* DESCRIPTION
*	This function is to stop au. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_au_stop(kal_int32 handle)
{
   aud_send_mma_stop_req(stack_int_get_active_module_id(), MMA_TYPE_AU, handle);
   MMA_WAIT_EVENT(MMA_EVT_STOP);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_au_pause
*
* DESCRIPTION
*	This function is to pause au. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_au_pause(kal_int32 handle)
{
   aud_send_mma_pause_req(stack_int_get_active_module_id(), MMA_TYPE_AU, handle);
   MMA_WAIT_EVENT(MMA_EVT_PAUSE);
   return mma_status;
}

/*************************************************************************
* FUNCTION
*	media_mma_au_resume
*
* DESCRIPTION
*	This function is to resume au. 
*
* PARAMETERS
*	kal_int32 handler
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_int32 media_mma_au_resume(kal_int32 handle)
{
   aud_send_mma_resume_req(stack_int_get_active_module_id(), MMA_TYPE_AU, handle);
   MMA_WAIT_EVENT(MMA_EVT_RESUME);
   return mma_status;
}


#endif /* __MED_MMA_MOD__ */

#ifdef __MED_VR_MOD__
kal_int32 media_aud_vr_get_version( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang )
{
	AUD_INIT_EVENT();
	aud_send_vr_get_version_req( src_mod_id, mode, lang );
	AUD_WAIT_EVENT( AUD_EVT_VR_GET_VER );
	return aud_result;
}

kal_int32 media_aud_vr_get_param( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_int16* sim, kal_int16* diff, kal_int16* rej )
{
	AUD_INIT_EVENT();
	aud_send_vr_get_param_req( src_mod_id, mode, lang, sim, diff, rej );
	AUD_WAIT_EVENT( AUD_EVT_VR_GET_PARAM );
	return aud_result;
}

kal_int32 media_aud_vr_set_param( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_int16 sim, kal_int16 diff, kal_int16 rej )
{
	AUD_INIT_EVENT();
	aud_send_vr_set_param_req( src_mod_id, mode, lang, sim, diff, rej );
	AUD_WAIT_EVENT( AUD_EVT_VR_SET_PARAM );
	return aud_result;
}

kal_int32 media_aud_vr_init_rcg( module_type src_mod_id, kal_uint32 session_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id )
{
	AUD_INIT_EVENT();
	aud_send_vr_init_rcg_req( src_mod_id, session_id, mode, lang, group_id );
	AUD_WAIT_EVENT( AUD_EVT_VR_INIT_RCG );
	return aud_result;
}

kal_int32 media_aud_vr_init_trn( module_type src_mod_id, kal_uint32 session_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 word_id )
{
	AUD_INIT_EVENT();
	aud_send_vr_init_trn_req( src_mod_id, session_id, mode, lang, group_id, word_id );
	AUD_WAIT_EVENT( AUD_EVT_VR_INIT_TRN );
	return aud_result;
}

kal_int32 media_aud_vr_voice_in( module_type src_mod_id, kal_uint32 session_id, kal_uint8 seq_no )
{
	AUD_INIT_EVENT();
	aud_send_vr_voice_in_req( src_mod_id, session_id, seq_no );
	AUD_WAIT_EVENT( AUD_EVT_VR_VOICE_IN );
	return aud_result;
}

kal_int32 media_aud_vr_del_tag( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 word_id )
{
	AUD_INIT_EVENT();
	aud_send_vr_del_tag_req( src_mod_id, mode, lang, group_id, word_id );
	AUD_WAIT_EVENT( AUD_EVT_VR_DEL_TAG );
	return aud_result;
}

kal_int32 media_aud_vr_check_tag( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 word_id )
{
	AUD_INIT_EVENT();
	aud_send_vr_check_tag_req( src_mod_id, mode, lang, group_id, word_id );
	AUD_WAIT_EVENT( AUD_EVT_VR_CHECK_TAG );
	return aud_result;
}

kal_int32 media_aud_vr_sync_db( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16* id_length_p, kal_uint16* id_array )
{
	AUD_INIT_EVENT();
	aud_send_vr_sync_db_req( src_mod_id, mode, lang, group_id, id_length_p, id_array );
	AUD_WAIT_EVENT( AUD_EVT_VR_SYNC_DB );
	return aud_result;
}

kal_int32 media_aud_vr_play_tag( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 word_id, kal_uint16 identifier)
{
	AUD_INIT_EVENT();
	aud_send_vr_play_tag_req( src_mod_id, mode, lang, group_id, word_id, identifier );
	AUD_WAIT_EVENT( AUD_EVT_VR_PLAY_TAG );
	return aud_result;
}

kal_int32 media_aud_vr_get_dir( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16* folder_name )
{
	AUD_INIT_EVENT();
	aud_send_vr_get_dir_req( src_mod_id, mode, lang, group_id, folder_name );
	AUD_WAIT_EVENT( AUD_EVT_VR_GET_DIR );
	return aud_result;
}

kal_int32 media_aud_vr_add_tag( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 id_length, kal_uint16* id_array )
{
	AUD_INIT_EVENT();
	aud_send_vr_add_tag_req( src_mod_id, mode, lang, group_id, id_length, id_array );
	AUD_WAIT_EVENT( AUD_EVT_VR_ADD_TAG );
	return aud_result;
}

void media_aud_vr_abort( module_type src_mod_id )
{
	AUD_INIT_EVENT();
	aud_send_vr_abort_req( src_mod_id );
	AUD_WAIT_EVENT( AUD_EVT_VR_ABORT );
}
#endif // __MED_VR_MOD__

#ifdef __MED_VRSI_MOD__

kal_int32 media_aud_vrsi_init(module_type src_mod_id)
{
   aud_send_vrsi_init_req(src_mod_id);
   VRSI_WAIT_EVENT(VRSI_EVT_INIT);
	return aud_result;        
}

//kal_uint32 media_aud_vrsi_new_session_id(void);

kal_int32 media_aud_vrsi_add_tags(module_type src_mod_id, kal_uint32 session_id, kal_uint8 *grammar, kal_uint16 tag_num, kal_uint16** name_list, kal_int32 *long_list, kal_uint8 *is_name_list, kal_uint16 *id_list)
{
   aud_send_vrsi_add_tags_req(src_mod_id, session_id, grammar, tag_num, name_list, long_list, is_name_list, id_list); 
   VRSI_WAIT_EVENT(VRSI_EVT_ADD_TAGS); 
   return aud_result;
} 

kal_int32 media_aud_vrsi_start_cmd_trn(module_type src_mod_id, kal_uint32 session_id, kal_uint8 *grammar, kal_uint16 *tag_name, kal_int32 tag_long)
{
   aud_send_vrsi_start_cmd_trn_req(src_mod_id, session_id, grammar, tag_name, tag_long);
   VRSI_WAIT_EVENT(VRSI_EVT_START_CMD_TRN);
   return aud_result;
}  

kal_int32 media_aud_vrsi_start_cmd_rcg(module_type src_mod_id, kal_uint32 session_id, kal_uint8 *grammar)
{
   aud_send_vrsi_start_cmd_rcg_req(src_mod_id, session_id, grammar);
   VRSI_WAIT_EVENT(VRSI_EVT_START_CMD_RCG);
   return aud_result;
}  

kal_int32 media_aud_vrsi_start_digit_rcg(module_type src_mod_id, kal_uint32 session_id, kal_uint8 lang, kal_uint16 limit)
{
   aud_send_vrsi_start_digit_rcg_req(src_mod_id, session_id, lang, limit);
   VRSI_WAIT_EVENT(VRSI_EVT_START_DIGIT_RCG);
   return aud_result;
} 
   
kal_int32 media_aud_vrsi_start_digit_adapt(module_type src_mod_id, kal_uint32 session_id, kal_uint8 lang)
{
   aud_send_vrsi_start_digit_adapt_req(src_mod_id, session_id, lang);
   VRSI_WAIT_EVENT(VRSI_EVT_START_DIGIT_ADAPT);  
   return aud_result;
}

// void media_aud_vrsi_prompt_cnf(module_type src_mod_id, kal_uint32 session_id);

kal_int32 media_aud_vrsi_play_tag(module_type src_mod_id, kal_uint32 session_id, kal_uint8 *grammar, kal_uint16 tag_id, kal_uint8 volume, kal_uint8 output_path)
{
   aud_send_vrsi_play_tag_req(src_mod_id, session_id, grammar, tag_id, volume, output_path);
   VRSI_WAIT_EVENT(VRSI_EVT_PLAY_TAG);  
   return aud_result;  
}


kal_int32 media_aud_vrsi_play_TTS(module_type src_mod_id, kal_uint32 session_id, kal_uint16 *text, kal_uint8 volume, kal_uint8 output_path, kal_uint8 lang)
{
   aud_send_vrsi_play_TTS_req(src_mod_id, session_id, text, volume, output_path, lang);
   VRSI_WAIT_EVENT(VRSI_EVT_PLAY_TTS);  
   return aud_result;     
}

kal_int32 media_aud_vrsi_get_tag_num(module_type src_mod_id, kal_uint8 *grammar, kal_int32 *tag_num)
{
   aud_send_vrsi_get_tag_num_req(src_mod_id, grammar, tag_num);
   VRSI_WAIT_EVENT(VRSI_EVT_GET_TAG_NUM);  
   return aud_result;  
}

kal_int32 media_aud_vrsi_get_tag_info(module_type src_mod_id, kal_uint8 *grammar, kal_int32 tag_num, kal_uint16 *id_list, kal_uint16 **name_list, kal_int32 *long_list)
{
   aud_send_vrsi_get_tag_info_req(src_mod_id, grammar, tag_num, id_list, name_list, long_list);
   VRSI_WAIT_EVENT(VRSI_EVT_GET_TAG_INFO);  
   return aud_result;  
}

kal_int32 media_aud_vrsi_del_tags(module_type src_mod_id, kal_uint8 *grammar, kal_int32 tag_num, kal_uint16 *id_list)
{
   aud_send_vrsi_del_tags_req(src_mod_id, grammar, tag_num, id_list);
   VRSI_WAIT_EVENT(VRSI_EVT_DEL_TAGS);  
   return aud_result;
}


kal_int32 media_aud_vrsi_reset_tags(module_type src_mod_id, kal_uint8 *grammar)
{
   aud_send_vrsi_reset_tags_req(src_mod_id, grammar);
   VRSI_WAIT_EVENT(VRSI_EVT_RESET_TAGS);  
   return aud_result;
}

kal_int32 media_aud_vrsi_sync_db(module_type src_mod_id, kal_uint8 *grammar, kal_int32 tag_num, kal_uint16 *id_list, kal_uint16 **name_list, kal_uint8 *sync_flag)
{
   aud_send_vrsi_sync_db_req(src_mod_id, grammar, tag_num, id_list, name_list, sync_flag);     
   VRSI_WAIT_EVENT(VRSI_EVT_SYNC_DB);  
   return aud_result;
}

void media_aud_vrsi_abort(module_type src_mod_id, kal_uint32 session_id)
{
   aud_send_vrsi_abort_req(src_mod_id, session_id);
   VRSI_WAIT_EVENT(VRSI_EVT_ABORT);  
   return;   
}

void media_aud_vrsi_close(module_type src_mod_id)
{
   aud_send_vrsi_close_req(src_mod_id);
   VRSI_WAIT_EVENT(VRSI_EVT_CLOSE);  
   return;         
}

kal_int32 media_aud_vrsi_get_error_msg(void) 
{
	return aud_vrsi_error();
}
#endif // __MED_VRSI_MOD__

#ifdef __MED_GENERAL_TTS__
/*****************************************************************************
 * FUNCTION
 *  media_aud_tts_set_attr_req()
 * DESCRIPTION
 *  
 * PARAMETERS
 *
 * RETURNS
 *  
 *****************************************************************************/
kal_int32 media_aud_tts_set_attr_req(module_type src_mod_id, kal_uint32 attr_id, kal_uint32 attr_value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    aud_send_tts_set_attr_req(src_mod_id, attr_id, attr_value);
    TTS_WAIT_EVENT(TTS_EVT_SET_ATTR);
    return aud_result;
}


/*****************************************************************************
 * FUNCTION
 *  media_aud_tts_play_req()
 * DESCRIPTION
 *  
 * PARAMETERS
 *
 * RETURNS
 *  
 *****************************************************************************/
kal_int32 media_aud_tts_play_req(module_type src_mod_id, void *tts_param)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    aud_send_tts_play_req(src_mod_id, tts_param);
    TTS_WAIT_EVENT(TTS_EVT_PLAY);
    return aud_result;
}


/*****************************************************************************
 * FUNCTION
 *  media_aud_tts_stop_req()
 * DESCRIPTION
 *  
 * PARAMETERS
 *
 * RETURNS
 *  
 *****************************************************************************/
kal_int32 media_aud_tts_stop_req(module_type src_mod_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    aud_send_msg_to_med(src_mod_id, (kal_uint16) MSG_ID_MEDIA_AUD_TTS_STOP_REQ, NULL);

    TTS_WAIT_EVENT(TTS_EVT_STOP);
    return aud_result;
}


/*****************************************************************************
 * FUNCTION
 *  media_aud_tts_pause_req()
 * DESCRIPTION
 *  
 * PARAMETERS
 *
 * RETURNS
 *  
 *****************************************************************************/
kal_int32 media_aud_tts_pause_req(module_type src_mod_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    aud_send_msg_to_med(src_mod_id, (kal_uint16) MSG_ID_MEDIA_AUD_TTS_PAUSE_REQ, NULL);

    TTS_WAIT_EVENT(TTS_EVT_PAUSE);
    return aud_result;
}


/*****************************************************************************
 * FUNCTION
 *  media_aud_tts_resume_req()
 * DESCRIPTION
 *  
 * PARAMETERS
 *
 * RETURNS
 *  
 *****************************************************************************/
kal_int32 media_aud_tts_resume_req(module_type src_mod_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    aud_send_msg_to_med(src_mod_id, (kal_uint16) MSG_ID_MEDIA_AUD_TTS_RESUME_REQ, NULL);

    TTS_WAIT_EVENT(TTS_EVT_RESUME);
    return aud_result;
}
#endif /*#ifdef __MED_GENERAL_TTS__*/

#ifdef __MED_BT_MOD__
/*****************************************************************************
 * FUNCTION
 *  media_aud_bt_audio_close
 * DESCRIPTION
 *  
 * PARAMETERS
 * 
 * RETURNS
 *  
 *****************************************************************************/
void media_aud_bt_audio_close(kal_uint8 profile)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	AUD_INIT_EVENT();
    aud_send_bt_audio_close_req(MOD_MMI/*stack_int_get_active_module_id()*/, profile);

    AUD_WAIT_EVENT(AUD_EVT_BT_CLOSE_AUDIO);
}


/*****************************************************************************
 * FUNCTION
 *  media_aud_bt_audio_close
 * DESCRIPTION
 *  
 * PARAMETERS
 * 
 * RETURNS
 *  
 *****************************************************************************/
void media_aud_bt_turn_off_audio(kal_uint8 profile)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	AUD_INIT_EVENT();
    aud_send_bt_audio_turn_off_req(MOD_MMI/*stack_int_get_active_module_id()*/, profile);

    AUD_WAIT_EVENT(AUD_EVT_BT_TURN_OFF_AUDIO);
}
#endif /*#ifdef __MED_BT_MOD__*/



#endif /* MED_NOT_PRESENT */



