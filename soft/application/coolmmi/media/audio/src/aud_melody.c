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
 *	aud_melody.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes voice memo related functions of audio manager task.
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/******************************************************************
 * MODULE
 *    AUD_VM
 * DESCRIPTION
 *    This module defines the voice memo related function for Audio Manager.
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
#include "gpio_sw.h"
//#include "alerter_sw.h"
#include "device.h"
#include "resource_audio.h"
#include "custom_equipment.h"
#include "nvram_struct.h"
#include "nvram_user_defs.h"
#include "nvram_data_items.h"
#include "custom_nvram_editor_data_item.h"
/* for FAT FS */
#include "fat_fs.h"
/* DRM_REPLACE */
#include "drm_gprot.h"

/* local includes */
#include "med_global.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "med_utility.h"
#include "med_main.h"
#include "aud_main.h"

#include "mmi_trace.h"
#include "mci.h"
#include "mdi_audio.h"
#ifdef __MMI_SUPPORT_BACKLIGHT_SYNC__
/* -- added for turn on/off backlight by MMI task -- */
#include "mmiapi_struct.h"
#endif

extern void wavSetAdpcmState( kal_bool realResume, kal_uint32 mode );

#include "aud_daf_parser.h"

#include "mmi_trace.h"

/*==== IMPORTS =======*/
#define AMR_HEADER   "#!AMR\n"
#define AWB_HEADER   "#!AMR-WB\n"

#define MAX_MELODY_DIR 20
#define MELODY_FULL_NAME (MAX_MELODY_FILE_NAME+MAX_MELODY_DIR)
#define CRESCENDO_STEPS  4

static kal_uint16 max_melody_volume1;
static kal_uint16 max_melody_volume2;
static kal_uint16 current_melody_volume1;
static kal_uint16 current_melody_volume2;
static kal_uint16 melody_volume_step1;
static kal_uint16 melody_volume_step2;

#ifdef __MMI_SUPPORT_BACKLIGHT_SYNC__
/* -- added for turn on/off backlight by MMI task -- */
#define CTRL_BACKLIGHT_REQ_IDLE 0
#define CTRL_BACKLIGHT_REQ_WAIT 1
#define CTRL_BACKLIGHT_REQ_WAIT_CLOSE 2
#define CTRL_BACKLIGHT_REQ_CLOSE 3
static kal_uint8 backlight_ctrl_state = CTRL_BACKLIGHT_REQ_IDLE;
static kal_bool backlight_ctrl_in_use = KAL_FALSE;
static volatile kal_mutexid  backlight_ctrl_mutex;
#endif

#ifdef __MMI_SUPPORT_LED_SYNC__
/* -- added for turn on/off led by MMI task -- */
static kal_uint8 wait_for_led_ctrl_rsp = 0;
static volatile kal_mutexid  led_ctrl_mutex;
#endif

kal_uint8 current_playing_melody_volume_level = 0; // this variable will keep track of current playing melody volume level

#ifdef __MED_BT_A2DP_MOD__
#if (defined(__BTCOOLSAND__)||defined(__BTRDA__))
extern void aud_bt_a2dp_open_codec(kal_int32 audio_format);
extern kal_bool aud_bt_a2dp_is_mute_phone(void);
 
#elif defined(__BTVCSR_HCI_BCHS__)
extern void aud_bt_a2dp_media_output_hdlr(kal_bool on);
extern void aud_bt_a2dp_media_play_hdlr(void);
extern void aud_bt_a2dp_media_pause_hdlr(void);
extern void aud_bt_a2dp_media_resume_hdlr(void);
extern void aud_bt_a2dp_media_stop_hdlr(void);
extern kal_bool aud_bt_a2dp_is_media_output_on(void);
//end
#endif
#endif /* __MED_BT_A2DP_MOD__ */ 

#ifdef __AUD_NEW_INTERFACE__
extern kal_bool aud_media_process_file_stream( Media_Event event );
extern kal_uint8 aud_media_play_byte_stream( kal_uint8* data, kal_uint32 size, kal_uint8 play_style );
#endif // __AUD_NEW_INTERFACE__

/******************************************************************
 * FUNCTION
 *    aud_melody_init
 * DESCRIPTION
 *    This function is used to init aud_melody module
 * PARAMETERS
 *    
 *    
 * RETURNS
 *
 * GLOBALS AFFECTED
 *  
 *  
 ******************************************************************/

kal_bool aud_melody_init(void)
{
   aud_context_p->vibrator_enabled = KAL_TRUE;
   aud_context_p->vibrator_on = KAL_FALSE;
   aud_context_p->led_on = KAL_FALSE;
   aud_context_p->backlight_on = KAL_TRUE;

   aud_context_p->vibrator_on_stored = KAL_FALSE;
   aud_context_p->led_on_stored = KAL_FALSE;
   aud_context_p->backlight_on_stored = KAL_TRUE;

   aud_context_p->source_type = AUD_UNKNOWN_SOURCE_TYPE;
   aud_context_p->media_playing = KAL_FALSE;
   aud_context_p->audio_id = 255;
   aud_context_p->string_p = NULL;
   

   custom_get_fixed_imelody_range(&aud_context_p->min_fixed_imy_id, &aud_context_p->max_fixed_imy_id);
   custom_get_fixed_midi_range(&aud_context_p->min_fixed_midi_id, &aud_context_p->max_fixed_midi_id);
   custom_get_fixed_sound_range(&aud_context_p->min_fixed_sound_id, &aud_context_p->max_fixed_sound_id);
   custom_get_fixed_message_sound_range(&aud_context_p->min_fixed_message_sound_id, &aud_context_p->max_fixed_message_sound_id);

   /* create the a semaphore */
   aud_context_p->media_sem = kal_create_sem("MEDIA SEM", 1);

#ifdef __MMI_SUPPORT_BACKLIGHT_SYNC__
   backlight_ctrl_mutex = kal_create_mutex( "backlight_ctrl_mutex" );
#endif
#ifdef __MMI_SUPPORT_LED_SYNC__
   led_ctrl_mutex = kal_create_mutex( "led_ctrl_mutex" );
#endif
   return KAL_TRUE;
}

void aud_free_melody_file_buffer(void)
{  
   if(aud_context_p->melody_string_p)
      med_free_ext_mem((void**)&aud_context_p->melody_string_p);
}

void aud_set_vibrator_enabled_req_hdlr( ilm_struct *ilm_ptr )
{
	media_aud_set_vibrator_enabled_req_struct* msg_p = (media_aud_set_vibrator_enabled_req_struct*)ilm_ptr->local_para_ptr;
	aud_context_p->vibrator_enabled = (kal_bool) msg_p->enabled;
}

#ifdef __MMI_SUPPORT_BACKLIGHT_SYNC__
/* -- added for turn on/off backlight by MMI task -- */
void aud_backlight_ctrl_rsp_hdlr(ilm_struct* ilm_ptr)
{
	mmiapi_backlight_control_rsp_struct* msg_p = (mmiapi_backlight_control_rsp_struct*)ilm_ptr->local_para_ptr;
	kal_bool on_off = (msg_p->on_off ? KAL_TRUE: KAL_FALSE);

     kal_take_mutex( backlight_ctrl_mutex );
	switch( backlight_ctrl_state )
	{
	case CTRL_BACKLIGHT_REQ_IDLE:
		ASSERT(0);
		break;
	case CTRL_BACKLIGHT_REQ_WAIT:
		if( on_off == aud_context_p->backlight_on )
			backlight_ctrl_state = CTRL_BACKLIGHT_REQ_IDLE;
		else
			aud_send_backlight_ctrl_req( MOD_MMI, (kal_uint8) aud_context_p->backlight_on, 1 );
		break;
	case CTRL_BACKLIGHT_REQ_WAIT_CLOSE:
		aud_send_backlight_ctrl_req( MOD_MMI, (kal_uint8) aud_context_p->backlight_on, 0 );
		backlight_ctrl_state = CTRL_BACKLIGHT_REQ_CLOSE;
		break;
	case CTRL_BACKLIGHT_REQ_CLOSE:
		if( on_off == aud_context_p->backlight_on )
			backlight_ctrl_state = CTRL_BACKLIGHT_REQ_IDLE;
		else
			ASSERT(0);
		break;
	}
	kal_give_mutex( backlight_ctrl_mutex );
}
#endif

#ifdef __MMI_SUPPORT_LED_SYNC__
/* -- added for turn on/off led by MMI task -- */
void aud_led_ctrl_rsp_hdlr(ilm_struct* ilm_ptr)
{
	mmiapi_led_control_rsp_struct* msg_p = (mmiapi_led_control_rsp_struct*)ilm_ptr->local_para_ptr;

	kal_take_mutex( led_ctrl_mutex );
	if( msg_p->on_off )
	{
		// MMI responses that it finishs turning on led request
		if( !aud_context_p->led_on )
		{
			aud_send_led_ctrl_req(MOD_MMI, 0); // request to turn off led
			wait_for_led_ctrl_rsp = 1;
		}
		else
			wait_for_led_ctrl_rsp = 0;
	}
	else
	{
		// MMI responses that it finishs turning off led request
		if( aud_context_p->led_on )
		{
			aud_send_led_ctrl_req(MOD_MMI, 1); // request to turn on led
			wait_for_led_ctrl_rsp = 1;
		}
		else
			wait_for_led_ctrl_rsp = 0;
	}
	kal_give_mutex( led_ctrl_mutex );
}
#endif

#ifdef __MMI_SUPPORT_BACKLIGHT_SYNC__
void aud_melody_imy_close_backlight_ctrl( void )
{
   kal_take_mutex( backlight_ctrl_mutex );
/* -- added for turn on/off backlight by MMI task -- */
   if( backlight_ctrl_state == CTRL_BACKLIGHT_REQ_IDLE )
   {
      if( backlight_ctrl_in_use )
      {
         aud_send_backlight_ctrl_req( MOD_MMI, 1, 0 );
         backlight_ctrl_state = CTRL_BACKLIGHT_REQ_CLOSE;
      }
   }
   else if( backlight_ctrl_state != CTRL_BACKLIGHT_REQ_CLOSE )
      backlight_ctrl_state = CTRL_BACKLIGHT_REQ_WAIT_CLOSE;
   backlight_ctrl_in_use = KAL_FALSE;
   aud_context_p->backlight_on = KAL_TRUE;
   kal_give_mutex( backlight_ctrl_mutex );
}
#endif

void aud_melody_stop_driver(void)
{
#if 0
#ifndef _WIN32
   if( aud_context_p->vibrator_enabled )
   {
      if (aud_context_p->vibrator_on)
      {
         custom_cfg_gpio_set_level( GPIO_DEV_VIBRATOR, VIBRATOR_OFF);
         aud_context_p->vibrator_on = KAL_FALSE;
      }
   }

   /* turn off led, if on */
#ifndef __MMI_SUPPORT_LED_SYNC__
   if (aud_context_p->led_on)
   {
      custom_cfg_gpio_set_level( GPIO_DEV_LED_STATUS_1, LED_LIGHT_LEVEL0);
      custom_cfg_gpio_set_level( GPIO_DEV_LED_STATUS_2, LED_LIGHT_LEVEL0);
      custom_cfg_gpio_set_level( GPIO_DEV_LED_STATUS_3, LED_LIGHT_LEVEL0);
      aud_context_p->led_on = KAL_FALSE;
   }
#else
   kal_take_mutex( led_ctrl_mutex );
   if (aud_context_p->led_on)
   {
/* -- added for turn on/off led by MMI task -- */
      if( !wait_for_led_ctrl_rsp )
      {
         aud_send_led_ctrl_req(MOD_MMI, 1);
         wait_for_led_ctrl_rsp = 1;
      }
      aud_context_p->led_on = KAL_FALSE;
   }
   kal_give_mutex( led_ctrl_mutex );
#endif
   
#ifndef __MMI_SUPPORT_BACKLIGHT_SYNC__
   if (!aud_context_p->backlight_on)
   {
      custom_cfg_gpio_set_level( GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL5 );
      custom_cfg_gpio_set_level( GPIO_DEV_LED_SUBLCD, LED_LIGHT_LEVEL5 );
      custom_cfg_gpio_set_level( GPIO_DEV_LED_KEY, LED_LIGHT_LEVEL5 );
   }
   aud_context_p->backlight_on = KAL_TRUE;
#else
   aud_melody_imy_close_backlight_ctrl();
#endif
#endif
#endif
}

void aud_melody_imy_vibrator_hdlr(kal_bool on)
{  
#ifndef _WIN32
   if( aud_context_p->vibrator_enabled )
   {
      if (on)
         custom_cfg_gpio_set_level( GPIO_DEV_VIBRATOR, VIBRATOR_ON);
      else
         custom_cfg_gpio_set_level( GPIO_DEV_VIBRATOR, VIBRATOR_OFF);
      aud_context_p->vibrator_on = on;
   }

#endif   

}

void aud_melody_imy_led_hdlr(kal_bool on)
{  
#ifndef _WIN32
#ifndef __MMI_SUPPORT_LED_SYNC__
   if (on)
   {
      custom_cfg_gpio_set_level( GPIO_DEV_LED_STATUS_1, LED_LIGHT_LEVEL5);
      custom_cfg_gpio_set_level( GPIO_DEV_LED_STATUS_2, LED_LIGHT_LEVEL5);
      custom_cfg_gpio_set_level( GPIO_DEV_LED_STATUS_3, LED_LIGHT_LEVEL5);
   }
   else
   {
      custom_cfg_gpio_set_level( GPIO_DEV_LED_STATUS_1, LED_LIGHT_LEVEL0);
      custom_cfg_gpio_set_level( GPIO_DEV_LED_STATUS_2, LED_LIGHT_LEVEL0);
      custom_cfg_gpio_set_level( GPIO_DEV_LED_STATUS_3, LED_LIGHT_LEVEL0);
   }
   aud_context_p->led_on = on;
#else
   kal_take_mutex( led_ctrl_mutex );
/* -- added for turn on/off led by MMI task -- */
   if( !wait_for_led_ctrl_rsp )
   {
      aud_send_led_ctrl_req(MOD_MMI, (kal_uint8)on);
      wait_for_led_ctrl_rsp = 1;
   }
   aud_context_p->led_on = on;
   kal_give_mutex( led_ctrl_mutex );
#endif
#endif   
}

void aud_melody_imy_backlight_hdlr(kal_bool on)
{  
#ifndef _WIN32
#ifndef __MMI_SUPPORT_BACKLIGHT_SYNC__
   if (on)
   {
      custom_cfg_gpio_set_level( GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL5);
      custom_cfg_gpio_set_level( GPIO_DEV_LED_SUBLCD, LED_LIGHT_LEVEL5);
      custom_cfg_gpio_set_level( GPIO_DEV_LED_KEY, LED_LIGHT_LEVEL5);
   }
   else
   {
      custom_cfg_gpio_set_level( GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL2);
      custom_cfg_gpio_set_level( GPIO_DEV_LED_SUBLCD, LED_LIGHT_LEVEL2);
      custom_cfg_gpio_set_level( GPIO_DEV_LED_KEY, LED_LIGHT_LEVEL0);
   }
   aud_context_p->backlight_on = on;
#else
   kal_take_mutex( backlight_ctrl_mutex );
/* -- added for turn on/off backlight by MMI task -- */
   if( backlight_ctrl_state == CTRL_BACKLIGHT_REQ_IDLE )
      aud_send_backlight_ctrl_req( MOD_MMI, (kal_uint8)on, 1 );
   backlight_ctrl_state = CTRL_BACKLIGHT_REQ_WAIT;
   backlight_ctrl_in_use = KAL_TRUE;
   aud_context_p->backlight_on = on;
   kal_give_mutex( backlight_ctrl_mutex );
#endif
#endif   
}

void aud_store_driver_status(void)
{
   aud_context_p->vibrator_on_stored=aud_context_p->vibrator_on;

#ifdef __MMI_SUPPORT_LED_SYNC__
   kal_take_mutex( led_ctrl_mutex );
   aud_context_p->led_on_stored=aud_context_p->led_on;
   kal_give_mutex( led_ctrl_mutex );
#else
   aud_context_p->led_on_stored=aud_context_p->led_on;
#endif

#ifdef __MMI_SUPPORT_BACKLIGHT_SYNC__
   kal_take_mutex( backlight_ctrl_mutex );
   aud_context_p->backlight_on_stored=aud_context_p->backlight_on;
   kal_give_mutex( backlight_ctrl_mutex );
#else
   aud_context_p->backlight_on_stored=aud_context_p->backlight_on;
#endif
}

void aud_restore_driver_status(void)
{
   aud_melody_imy_vibrator_hdlr(aud_context_p->vibrator_on_stored);
   aud_melody_imy_led_hdlr(aud_context_p->led_on_stored);
   aud_melody_imy_backlight_hdlr(aud_context_p->backlight_on_stored);      
}


void aud_melody_create_folder(void)
{
   FS_HANDLE file_handle;
   kal_int32   result;

   /* if the dir exists, open it ,otherwise create it */
   file_handle = MMI_FS_Open((UINT8*)L"z:\\def_sound", FS_OPEN_DIR | FS_READ_ONLY );
   if (file_handle < 0) {
      result = MMI_FS_CreateDir((UINT8*)L"z:\\def_sound");
      ASSERT(result >= 0);
   }
   else
      MMI_FS_Close(file_handle);

   /* if the dir exists, open it ,otherwise create it */
   file_handle = MMI_FS_Open((UINT8*)L"z:\\def_sound\\imelody", FS_OPEN_DIR | FS_READ_ONLY );
   if (file_handle < 0) {
      result = MMI_FS_CreateDir((UINT8*)L"z:\\def_sound\\imelody");
      ASSERT(result >= 0);
   }
   else
      MMI_FS_Close(file_handle);

   /* if the dir exists, open it ,otherwise create it */
   file_handle = MMI_FS_Open((UINT8*)L"z:\\def_sound\\other", FS_OPEN_DIR | FS_READ_ONLY );
   if (file_handle < 0) {
      result = MMI_FS_CreateDir((UINT8*)L"z:\\def_sound\\other");
      ASSERT(result >= 0);
   }
   else
      MMI_FS_Close(file_handle);

}

 
kal_bool aud_melody_startup(void)
{
   //aud_melody_create_folder();
   return KAL_TRUE;
}

void aud_melody_set_max_swing(kal_uint16 max_swing)
{
   Media_Control(MEDIA_CTRL_MAX_SWING, max_swing);

}


void aud_melody_set_output_device(kal_uint8 device)
{
   Media_SetOutputDevice(device);

}

void aud_melody_set_volume(kal_uint8 volume1, kal_uint8 volume2)
{
   Media_SetOutputVolume(volume1,volume2);

}

void aud_melody_set_output_volume(kal_uint8 volume1, kal_uint8 volume2)
{
   
   max_melody_volume1 = (kal_uint16)volume1;
   max_melody_volume2 = (kal_uint16)volume2;
   aud_melody_set_volume(volume1,volume2);
}

void aud_set_active_ring_tone_volume(void)
{
   /* incoming call ring, alarm, message use the same volume */
   kal_uint8 volume;
   kal_uint8 gain_audio_mode;

   /* store playing melody volume level */
   current_playing_melody_volume_level = aud_get_volume_level(aud_context_p->audio_mode, AUD_VOLUME_MEDIA);
   /* if it's headset mode and output to both earphone & loud speaker, use normal mode gain value */
   if( aud_context_p->audio_mode == AUD_MODE_HEADSET && aud_context_p->melody_output_device == AUDIO_DEVICE_SPEAKER_BOTH )
      gain_audio_mode = AUD_MODE_NORMAL;
   else
      gain_audio_mode = aud_context_p->audio_mode;
   volume = aud_get_volume_gain( gain_audio_mode, AUD_VOLUME_MEDIA, current_playing_melody_volume_level );
   aud_melody_set_output_volume( volume, volume );
}

void aud_set_active_sound_volume(void)
{
   /* power on/off, cover on/off use the same volume */
   /* the volume gain is for sound, but the volume level is tied with keytone's*/
   kal_uint8 volume;
   volume = aud_get_volume_gain(aud_context_p->audio_mode, 
                                 AUD_VOLUME_FMR, 
                                 aud_get_volume_level(aud_context_p->audio_mode, VOL_TYPE_GMI));
   aud_melody_set_volume(volume, volume);

}

void aud_melody_crescendo_hdlr(void* arg)
{
   
   current_melody_volume1+= melody_volume_step1;
   current_melody_volume2+= melody_volume_step2;

   current_melody_volume1 = MIN(current_melody_volume1, max_melody_volume1);
   current_melody_volume2 = MIN(current_melody_volume2, max_melody_volume2);

   aud_melody_set_volume((kal_uint8)current_melody_volume1, (kal_uint8)current_melody_volume2);


   if(current_melody_volume1<max_melody_volume1 ||
      current_melody_volume2<max_melody_volume2)
   {
      med_start_timer(AUD_TIMER_CRESCENDO, AUD_TIMER_PERIOD, aud_melody_crescendo_hdlr, 0);
   }
   else
      med_stop_timer(AUD_TIMER_CRESCENDO);

}

void aud_melody_stop_crescendo(void)
{
   med_stop_timer(AUD_TIMER_CRESCENDO);
   aud_melody_set_volume((kal_uint8)max_melody_volume1, (kal_uint8)max_melody_volume2);

}

kal_uint8 aud_melody_play_style_convert(kal_uint8 play_style)
{
   kal_uint8 l1sp_play_style = 1;//AUDIO_PLAY_ONCE;

   switch(play_style)
   {
      /* Play sound for crescendo */
   case DEVICE_AUDIO_PLAY_CRESCENDO:
      l1sp_play_style = 0;//AUDIO_PLAY_CRESCENDO;
      if(max_melody_volume1>=CRESCENDO_STEPS)
      {
         melody_volume_step1 = max_melody_volume1/CRESCENDO_STEPS;
         melody_volume_step2 = max_melody_volume2/CRESCENDO_STEPS;
         current_melody_volume1 = melody_volume_step1;
         current_melody_volume2 = melody_volume_step2;
         med_start_timer(AUD_TIMER_CRESCENDO, AUD_TIMER_PERIOD, aud_melody_crescendo_hdlr, 0);
      }
      else
      {
         melody_volume_step1 = 0;
         melody_volume_step2 = 0;
         current_melody_volume1 = max_melody_volume1;
         current_melody_volume2 = max_melody_volume2;
      }
      aud_melody_set_volume((kal_uint8)current_melody_volume1, (kal_uint8)current_melody_volume2);
      break;
      /* Play sound for repeat. */
   case DEVICE_AUDIO_PLAY_INFINITE:
      l1sp_play_style = 0;//AUDIO_PLAY_INFINITE;
      break;
      /* Play sound for once. */
   case DEVICE_AUDIO_PLAY_ONCE:
      l1sp_play_style = 1;//AUDIO_PLAY_ONCE;
      break;
   default:
      break;
   }
   
   return l1sp_play_style;

}

#ifdef __MED_MMA_MOD__
void aud_media_close_handle( void )
{
   if( aud_context_p->last_handle == NULL ) return;
   switch( aud_context_p->current_format )
   {
      case MED_TYPE_SMF:
      case MED_TYPE_SMF_SND:
         JSmf_Close( aud_context_p->last_handle );
         aud_context_p->last_handle = NULL;
         break;
      case MED_TYPE_IMELODY:
         JImy_Close( aud_context_p->last_handle );
         aud_context_p->last_handle = NULL;
         break;
#ifdef MMF_DECODE
      case MED_TYPE_MMF:
         JMmf_Close( aud_context_p->last_handle );
         aud_context_p->last_handle = NULL;
         break;
#endif
   }
}
#endif
/*Jinzh Add Start For 插着耳机快门音从扬声器听到 Ver: TBM780  on 2007-8-27 15:44 */
#ifdef __FF_AUDIO_SET__
extern BOOL cameraAppOnOff;
#endif
/*Jinzh Add End  For 插着耳机快门音从扬声器听到 Ver: TBM780  on 2007-8-27 15:44 */
extern BOOL HA_audio_StopStream(VOID);

void aud_media_play_event_hdlr(ilm_struct *ilm_ptr)
{

	TBM_ENTRY(0x2A14);
  aud_context_p->tone_playing = KAL_FALSE;
	TRACE_EF(g_sw_MP3, "aud_context_p->tone_playing =FALSE");
  //add by wuzc 2007062519
   //aud_send_audio_play_finish_ind(MED_RES_OK);

	HA_audio_StopStream();//chenhe add


/*Jinzh Add Start For 插着耳机快门音从扬声器听到 Ver: TBM780  on 2007-8-27 15:40 */
#ifdef __FF_AUDIO_SET__
	if(cameraAppOnOff == TRUE)
	{
		cameraAppOnOff = FALSE;
	}
#endif
/*Jinzh Add End  For 插着耳机快门音从扬声器听到 Ver: TBM780  on 2007-8-27 15:40 */
	/*JINZH Add End  For 6293 Ver: TBM780  on 2007-8-17 15:3 */
  #if 1
   aud_media_play_event_ind_struct* ind_p = (aud_media_play_event_ind_struct*)ilm_ptr->local_para_ptr;

   if( /*aud_context_p->last_seq_no != ind_p->seq_no ||*/ (aud_context_p->state != AUD_MEDIA_PLAY && aud_context_p->state != AUD_MEDIA_PLAY_PAUSED) )
   {
      return;
   }

#ifdef __AUD_NEW_INTERFACE__
   if( aud_media_process_file_stream( (Media_Event)ind_p->event ) )
      return;
#endif // __AUD_NEW_INTERFACE__
   
   switch(ind_p->event)
   {
      case MEDIA_END:
         /* enter AUD_MEDIA_IDLE state */
         AUD_ENTER_STATE(AUD_MEDIA_IDLE);
         aud_melody_stop_driver();
         if ((aud_context_p->source_type==AUD_ID||aud_context_p->source_type==AUD_STRING) && 
            aud_is_streaming_type(aud_context_p->current_format) &&
            aud_context_p->play_style!=DEVICE_AUDIO_PLAY_ONCE &&
            aud_context_p->string_p!=NULL)
         {
            kal_uint8 res;
            res=aud_replay_stream();
			mmi_trace(1,"chenhe,aud_media_play_event_hdlr,111");
            if (res!=MED_RES_OK)
            {
               /* stop volume crescendo timer */
            	 med_stop_timer(AUD_TIMER_CRESCENDO);
               
               aud_send_audio_play_finish_ind(res);
            }
	   TBM_EXIT(0x2A14);
         }   
         else /* AUD_FILE */
         {
#ifdef __MED_MMA_MOD__
            aud_media_close_handle();
#endif
            /* stop volume crescendo timer */
            med_stop_timer(AUD_TIMER_CRESCENDO);
            
            /* free file buffer */
            aud_free_melody_file_buffer();
            			mmi_trace(1,"chenhe,aud_media_play_event_hdlr,2222,aud_context_p->play_mode is %d",aud_context_p->play_mode);

            if (aud_context_p->play_mode==AUD_MEDIA_PLAY_AS_RING)
               aud_send_audio_play_finish_ind(MED_RES_END_OF_FILE);
            else
               aud_send_media_play_finish_ind(MED_RES_END_OF_FILE);

		TBM_EXIT(0x2A14);
         }
         break;
      case MEDIA_ERROR:

#ifdef __MED_MMA_MOD__
         aud_media_close_handle();
#endif
         /* enter AUD_MEDIA_IDLE state */
         AUD_ENTER_STATE(AUD_MEDIA_IDLE);

         /* stop volume crescendo timer */
         med_stop_timer(AUD_TIMER_CRESCENDO);

         /* not for iMelody, so ... */
         aud_melody_stop_driver();

         /* free file buffer */
         aud_free_melody_file_buffer();

         if(aud_context_p->source_type==AUD_FILE &&
            aud_context_p->play_mode==AUD_MEDIA_PLAY_AS_SONG)
            aud_send_media_play_finish_ind(MED_RES_ERROR);
         else
            aud_send_audio_play_finish_ind(MED_RES_ERROR);

	TBM_EXIT(0x2A14);
         break;
		 
	/*liwh Add Start For 5993 Ver: TBM780  on 2007-8-8 17:25 */
	case MEDIA_UNEXPECTED_FORMAT:
		#ifdef __MED_MMA_MOD__
         aud_media_close_handle();
		#endif
         /* enter AUD_MEDIA_IDLE state */
         AUD_ENTER_STATE(AUD_MEDIA_IDLE);

         /* stop volume crescendo timer */
         med_stop_timer(AUD_TIMER_CRESCENDO);

         /* not for iMelody, so ... */
         aud_melody_stop_driver();

         /* free file buffer */
         aud_free_melody_file_buffer();

         if(aud_context_p->source_type==AUD_FILE &&
            aud_context_p->play_mode==AUD_MEDIA_PLAY_AS_SONG)
            aud_send_media_play_finish_ind(MED_RES_INVALID_FORMAT);
         else
            aud_send_audio_play_finish_ind(MED_RES_INVALID_FORMAT);

	TBM_EXIT(0x2A14);
         break;
	/*liwh Add End  For 5993 Ver: TBM780  on 2007-8-8 17:25 */
	
      case MEDIA_TERMINATED:
         /* enter AUD_MEDIA_IDLE state */
         AUD_ENTER_STATE(AUD_MEDIA_IDLE);

         /* stop volume crescendo timer */
         med_stop_timer(AUD_TIMER_CRESCENDO);
         
         /* not for iMelody, so ... */
         aud_melody_stop_driver();

	TBM_EXIT(0x2A14);
         break;
      case MEDIA_DATA_REQUEST:
      case MEDIA_DATA_NOTIFICATION:
         break;
      default:
         /* not for iMelody, so ... */
         break;         
   }
   #endif

   return;
}

void aud_media_ievent_callback(Media_Handle *handle, Media_Event event)
{
	aud_media_event_callback( event );
}

void aud_media_event_callback(Media_Event event)
{
   aud_media_play_event_ind_struct* ind_p = NULL;
   
   switch(event)
   {
      case MEDIA_LED_ON:
         aud_melody_imy_led_hdlr(KAL_TRUE);
         return;
      case MEDIA_LED_OFF:
         aud_melody_imy_led_hdlr(KAL_FALSE);
         return;
      case MEDIA_VIBRATOR_ON:
         aud_melody_imy_vibrator_hdlr(KAL_TRUE);
         return;
      case MEDIA_VIBRATOR_OFF:
         aud_melody_imy_vibrator_hdlr(KAL_FALSE);
         return;
      case MEDIA_BACKLIGHT_ON:
         aud_melody_imy_backlight_hdlr(KAL_TRUE);
         return;
      case MEDIA_BACKLIGHT_OFF:
         aud_melody_imy_backlight_hdlr(KAL_FALSE);
         return;
      case MEDIA_REPEATED:
         aud_melody_stop_driver();
         return;
      case MEDIA_TERMINATED:
         return;
      default:
         break;
   }      
   ind_p = (aud_media_play_event_ind_struct*)
            construct_local_para(sizeof(aud_media_play_event_ind_struct), TD_CTRL);

   ind_p->src_id = aud_context_p->src_id;
   ind_p->event = event;
   ind_p->seq_no = aud_context_p->last_seq_no;
   
   aud_send_ilm(MOD_MED, 
               MSG_ID_AUD_MEDIA_PLAY_EVENT_IND, 
               ind_p, 
               NULL);

}


kal_int32 aud_melody_play_imy(kal_uint8* data, kal_uint32 len, kal_uint8 play_style)
{
#ifdef __MED_MMA_MOD__
   Media_iMelody_Param* imy_param_p =NULL;
   Media_Status status = 0;
               
   if( (aud_context_p->last_handle = JImy_Open( aud_media_ievent_callback, &imy_param_p )) != NULL )
   {
      imy_param_p->imyfile = (const kal_uint8*) data;
      imy_param_p->filesize = len;
      imy_param_p->repeats = aud_melody_play_style_convert(play_style);
      imy_param_p->default_instrument = IMY_DEFAULT_INSTRUMENT;

	      #ifdef __MED_BT_A2DP_MOD__
    #if (defined(__BTCOOLSAND__)||defined(__BTRDA__))
	    aud_bt_a2dp_open_codec(MED_TYPE_IMELODY);
    #endif
    #endif /* __MED_BT_A2DP_MOD__ */
	
      if( (status = JImy_Play( aud_context_p->last_handle )) == MEDIA_SUCCESS )
      {
         /* enter AUD_MEDIA_PLAY state */
         AUD_ENTER_STATE(AUD_MEDIA_PLAY);
      }
      else
      {
         JImy_Close( aud_context_p->last_handle );
         aud_context_p->last_handle = NULL;
         /* stop volume crescendo timer */
         med_stop_timer(AUD_TIMER_CRESCENDO);
      }
   }
   else
   {
      /* stop volume crescendo timer */
      med_stop_timer(AUD_TIMER_CRESCENDO);
   }
#else
   Media_iMelody_Param msp;
   Media_Status status;
   
   msp.imyfile = (const kal_uint8*)data;
   msp.filesize = len;
   msp.repeats = aud_melody_play_style_convert(play_style);
   msp.default_instrument = IMY_DEFAULT_INSTRUMENT;
   
   Media_SetBuffer(aud_context_p->ring_buf, AUD_RING_BUFFER_LEN);
   if ((status=Media_Play((Media_Format)MED_TYPE_IMELODY, aud_media_event_callback, &msp ))==MEDIA_SUCCESS)
   {
      /* enter AUD_MEDIA_PLAY state */
      AUD_ENTER_STATE(AUD_MEDIA_PLAY);
   }
   else
   {
      /* stop volume crescendo timer */
      med_stop_timer(AUD_TIMER_CRESCENDO);
   }
#endif

   return aud_get_res(status);
}


kal_int32 aud_melody_play_smf(kal_uint8* data, kal_uint32 len, kal_uint8 play_style)
{
#ifdef __MED_MMA_MOD__
   Media_SMF_Param* smf_param_p = NULL;
   Media_Status status = 0;
               
   if( (aud_context_p->last_handle = JSmf_Open( aud_media_ievent_callback, &smf_param_p )) != NULL )
   {
      smf_param_p->smffile = (const kal_uint8*) data;
      smf_param_p->filesize = len;
      smf_param_p->repeats = aud_melody_play_style_convert(play_style);

    #ifdef __MED_BT_A2DP_MOD__
    #if (defined(__BTCOOLSAND__)||defined(__BTRDA__))
	    aud_bt_a2dp_open_codec(MED_TYPE_SMF);
    #endif
    #endif /* __MED_BT_A2DP_MOD__ */ 

      if( (status = JSmf_Play( aud_context_p->last_handle )) == MEDIA_SUCCESS )
      {
         /* enter AUD_MEDIA_PLAY state */
         AUD_ENTER_STATE(AUD_MEDIA_PLAY);
      }
      else
      {
         JSmf_Close( aud_context_p->last_handle );
         aud_context_p->last_handle = NULL;
         /* stop volume crescendo timer */
         med_stop_timer(AUD_TIMER_CRESCENDO);
      }
   }
   else /* free file buffer */
   {
      /* stop volume crescendo timer */
      med_stop_timer(AUD_TIMER_CRESCENDO);
   }
#else
   Media_SMF_Param msp;
   Media_Status status;

   msp.smffile = (const kal_uint8*)data;
   msp.filesize = len;
   msp.repeats = aud_melody_play_style_convert(play_style);
   
   Media_SetBuffer(aud_context_p->ring_buf, AUD_RING_BUFFER_LEN);
   if ((status=Media_Play((Media_Format)MED_TYPE_SMF, aud_media_event_callback, &msp))==MEDIA_SUCCESS)
   {
      /* enter AUD_MEDIA_PLAY state */
      AUD_ENTER_STATE(AUD_MEDIA_PLAY);
   }
   else
   {
      /* stop volume crescendo timer */
      med_stop_timer(AUD_TIMER_CRESCENDO);
   }
#endif

   return aud_get_res(status);
}

kal_int32 aud_melody_play_mmf(kal_uint8* data, kal_uint32 len, kal_uint8 play_style)
{
#if defined( __MED_MMA_MOD__ ) && defined( MMF_DECODE )
   Media_MMF_Param* mmf_param_p;
   Media_Status status;
               
   if( (aud_context_p->last_handle = JMmf_Open( aud_media_ievent_callback, &mmf_param_p )) != NULL )
   {
      mmf_param_p->mmffile = (const kal_uint8*) data;
      mmf_param_p->filesize = len;
      mmf_param_p->repeats = aud_melody_play_style_convert(play_style);
      if( (status = JMmf_Play( aud_context_p->last_handle )) == MEDIA_SUCCESS )
      {
         /* enter AUD_MEDIA_PLAY state */
         AUD_ENTER_STATE(AUD_MEDIA_PLAY);
      }
      else
      {
         JMmf_Close( aud_context_p->last_handle );
         aud_context_p->last_handle = NULL;
         /* stop volume crescendo timer */
         med_stop_timer(AUD_TIMER_CRESCENDO);
      }
   }
   else /* free file buffer */
   {
      /* stop volume crescendo timer */
      med_stop_timer(AUD_TIMER_CRESCENDO);
   }
#else
   Media_MMF_Param msp;
   Media_Status status;

   msp.mmffile = (const kal_uint8*)data;
   msp.filesize = len;
   msp.repeats = aud_melody_play_style_convert(play_style);
   
//#ifdef SW_SYN_16K
   Media_SetBuffer(aud_context_p->ring_buf, AUD_RING_BUFFER_LEN);
//#endif
   if ((status=Media_Play((Media_Format)MED_TYPE_MMF, aud_media_event_callback, &msp))==MEDIA_SUCCESS)
   {
      /* enter AUD_MEDIA_PLAY state */
      AUD_ENTER_STATE(AUD_MEDIA_PLAY);
   }
   else
   {
      /* stop volume crescendo timer */
      med_stop_timer(AUD_TIMER_CRESCENDO);
   }
#endif

   return aud_get_res(status);
}



kal_int32 aud_melody_play_stream(kal_uint8* data, kal_uint32 len, kal_uint8 format, kal_uint8 play_style)
{
   Media_Status status;
   kal_uint32 offset;
   audio_input_stream_struct *input_stream;

   if( format == MED_TYPE_AMR )
   {
      data += (sizeof(AMR_HEADER)-1);
      len -= (sizeof(AMR_HEADER)-1);
   }
#ifdef DAF_DECODE
   else if( format == MED_TYPE_DAF )
   {
      offset = 0;
      input_stream = audio_byte_input_stream_alloc( data, len );
      if( !daf_seek( input_stream, &offset ) )
      {
         audio_input_stream_free( input_stream );
         return MED_RES_BAD_FORMAT;
      }
      audio_input_stream_free( input_stream );
      len -= (kal_uint32) offset;
      data += offset;
      if( ((kal_uint32)data) % 2 )
         len += 2;
   }
#endif
   len = len/2;

   aud_melody_play_style_convert(play_style);
   
   Media_SetBuffer((kal_uint16*)data, len+1);
   Media_WriteDataDone(len);
   Media_DataFinished();
   switch( format )
   {
   case MED_TYPE_WAV:
   case MED_TYPE_WAV_ALAW:
   case MED_TYPE_WAV_ULAW:
   case MED_TYPE_WAV_DVI_ADPCM:
      wavSetAdpcmState( KAL_FALSE, 1 );
      break;
   }
   if ((status=Media_Play((Media_Format)format, aud_media_event_callback, MEDIA_VMP_AS_RINGTONE))==MEDIA_SUCCESS)
   {
      /* enter AUD_MEDIA_PLAY state */
      AUD_ENTER_STATE(AUD_MEDIA_PLAY);
   }
   else
   {
      /* stop volume crescendo timer */
      med_stop_timer(AUD_TIMER_CRESCENDO);
   }
   return aud_get_res(status);
}

kal_int32 aud_replay_stream(void)
{
   Media_Status status;
   kal_uint32 len = aud_context_p->buf_len;
   kal_uint8* data = aud_context_p->string_p;
   kal_uint8 format = aud_context_p->current_format;
   kal_uint32 offset;
   audio_input_stream_struct *input_stream;

   if( format == MED_TYPE_AMR )
   {
      data += (sizeof(AMR_HEADER)-1);
      len -= (sizeof(AMR_HEADER)-1);
   }
#if defined (DAF_DECODE) && defined(__RICH_AUDIO_PROFILE__)
   else if( format == MED_TYPE_DAF )
   {
      offset = 0;
      input_stream = audio_byte_input_stream_alloc( data, len );
      if( !daf_seek( input_stream, &offset ) )
      {
         audio_input_stream_free( input_stream );
         return MED_RES_BAD_FORMAT;
      }
      audio_input_stream_free( input_stream );
      len -= (kal_uint32) offset;
      data += offset;
      if( ((kal_uint32)data) % 2 )
         len += 2;
   }
#endif
   len = len/2;

   Media_SetBuffer((kal_uint16*)data, len+1);
   Media_WriteDataDone(len);
   Media_DataFinished();
   switch( aud_context_p->current_format )
   {
   case MED_TYPE_WAV:
   case MED_TYPE_WAV_ALAW:
   case MED_TYPE_WAV_ULAW:
   case MED_TYPE_WAV_DVI_ADPCM:
      wavSetAdpcmState( KAL_FALSE, 1 );
      break;
   }
   if ((status=Media_Play((Media_Format)format, aud_media_event_callback, MEDIA_VMP_AS_RINGTONE))==MEDIA_SUCCESS)
   {
      /* enter AUD_MEDIA_PLAY state */
      AUD_ENTER_STATE(AUD_MEDIA_PLAY);
   }
   return (kal_int32)aud_get_res(status);      
}

void aud_melody_play_by_id(kal_uint8 audio_id, kal_uint8 play_style, kal_uint8 audio_out_device)
{
   kal_uint8 rel_audio_id;
   kal_int32 res;

   aud_context_p->source_type = AUD_ID;
   aud_context_p->audio_id = audio_id;
   aud_context_p->play_style= play_style;

   /* set to the volume of ringtone */
   aud_set_active_ring_tone_volume();
   /* long imelody for incoming call and alarm */
   if ((audio_id >= aud_context_p->min_fixed_imy_id) && 
      (audio_id <= aud_context_p->max_fixed_imy_id))
   {
      rel_audio_id = audio_id-aud_context_p->min_fixed_imy_id;
      aud_context_p->current_format=resource_imelodys[rel_audio_id].format;
      aud_context_p->string_p=(kal_uint8*)resource_imelodys[rel_audio_id].data;
      aud_context_p->buf_len=resource_imelodys[rel_audio_id].len;
   }
   /* long midi for incoming call and alarm */
   else if ((audio_id >= aud_context_p->min_fixed_midi_id) && 
      (audio_id <= aud_context_p->max_fixed_midi_id))
   {
      rel_audio_id = audio_id-aud_context_p->min_fixed_midi_id;
      aud_context_p->current_format=resource_midis[rel_audio_id].format;
      aud_context_p->string_p=(kal_uint8*)resource_midis[rel_audio_id].data;
      aud_context_p->buf_len=resource_midis[rel_audio_id].len;
   }
   /* short sound for cover open/close, power on/off */
   else if((audio_id >= aud_context_p->min_fixed_sound_id) && 
         (audio_id <= aud_context_p->max_fixed_sound_id))
   {
      rel_audio_id = audio_id-aud_context_p->min_fixed_sound_id;
      aud_context_p->current_format=resource_sounds[rel_audio_id].format;
      aud_context_p->string_p=(kal_uint8*)resource_sounds[rel_audio_id].data;
      aud_context_p->buf_len=resource_sounds[rel_audio_id].len;
   }
   /* short sound for message */
   else if((audio_id >= aud_context_p->min_fixed_message_sound_id) && 
         (audio_id <= aud_context_p->max_fixed_message_sound_id))
   {
      
      rel_audio_id = audio_id-aud_context_p->min_fixed_message_sound_id;
      aud_context_p->current_format=resource_message_sounds[rel_audio_id].format;
      aud_context_p->string_p=(kal_uint8*)resource_message_sounds[rel_audio_id].data;
      aud_context_p->buf_len=resource_message_sounds[rel_audio_id].len;
   }
   /* short sound for EMS */
   else if((audio_id >= MIN_EMS_SND_ID) && (audio_id <= MAX_EMS_SND_ID))
   {
      rel_audio_id = audio_id-MIN_EMS_SND_ID;
      aud_context_p->current_format=resource_ems_sounds[rel_audio_id].format;
      aud_context_p->string_p=(kal_uint8*)resource_ems_sounds[rel_audio_id].data;
      aud_context_p->buf_len=resource_ems_sounds[rel_audio_id].len;
   }
   else
   {
      aud_send_audio_play_finish_ind(MED_RES_ERROR);
      return;
   }
      
   switch (aud_context_p->current_format)
   {
      case MED_TYPE_SMF:
         res=aud_melody_play_smf(aud_context_p->string_p,
                              aud_context_p->buf_len,
                              play_style);
         break;                     
      case MED_TYPE_IMELODY:
         res=aud_melody_play_imy(aud_context_p->string_p,
                              aud_context_p->buf_len,
                              play_style);
         break;                     
      case MED_TYPE_MMF:
         res=aud_melody_play_mmf(aud_context_p->string_p,
                              aud_context_p->buf_len,
                              play_style);
         break;                     
      case MED_TYPE_GSM_FR:
      case MED_TYPE_GSM_HR:
      case MED_TYPE_GSM_EFR:
      case MED_TYPE_AMR:
      case MED_TYPE_AMR_WB:
      case MED_TYPE_DAF:
      case MED_TYPE_PCM_8K:
      case MED_TYPE_PCM_16K:
      case MED_TYPE_G711_ALAW:
      case MED_TYPE_G711_ULAW:
      case MED_TYPE_DVI_ADPCM:
      case MED_TYPE_WAV:
      case MED_TYPE_WAV_ALAW:
      case MED_TYPE_WAV_ULAW:
      case MED_TYPE_WAV_DVI_ADPCM:
      case MED_TYPE_AU:
      case MED_TYPE_AIFF:
         res=aud_melody_play_stream(aud_context_p->string_p,
                              aud_context_p->buf_len,
                              aud_context_p->current_format,
                              play_style);
         break;
#ifdef __AUD_NEW_INTERFACE__
#ifdef AAC_DECODE
      case MED_TYPE_AAC:
#endif // AAC_DECODE
#ifdef WMA_DECODE
      case MED_TYPE_WMA:
#endif // WMA_DECODE
         res = aud_media_play_byte_stream( aud_context_p->string_p, aud_context_p->buf_len, play_style );
         break;
#endif // __AUD_NEW_INTERFACE__
      default:
         res=MED_RES_INVALID_FORMAT;
         break;
   }
   if(res!=MED_RES_OK)
      aud_send_audio_play_finish_ind(MED_RES_OK);
}

#ifdef __MED_MMA_MOD__
kal_uint8 aud_melody_resume_by_name(kal_wchar* file_name, kal_uint8 play_style, kal_uint8 audio_out_device)
{
   /* open the file */
   kal_int16 format;
   

   aud_context_p->source_type = AUD_FILE;

   format = med_get_media_type(file_name);

   /* set to the volume of ringtone */
   aud_set_active_ring_tone_volume();
   
   /* DRM_REPLACE */   
   //aud_context_p->current_file_handle = FS_Open(file_name, FS_READ_ONLY|FS_OPEN_NO_DIR);
   aud_context_p->current_file_handle = DRM_open_file((UINT8*)file_name, FS_READ_ONLY|FS_OPEN_NO_DIR, 0);   

   if (aud_context_p->current_file_handle >= 0)
   {
      INT32 len;
      INT32 buf_len;
      INT32 result;
      
      aud_context_p->audio_end=KAL_FALSE;
      /* DRM_REPLACE */
      //result=FS_GetFileSize(aud_context_p->current_file_handle, &buf_len);
      result=DRM_file_size(aud_context_p->current_file_handle, &buf_len);      
      if(!(result==FS_NO_ERROR && buf_len>0))
      {
         /* DRM_REPLACE */         
         //FS_Close(aud_context_p->current_file_handle);
         DRM_close_file(aud_context_p->current_file_handle);
         return MED_RES_OPEN_FILE_FAIL;
      }
      else if(buf_len>AUD_MELODY_BUFFER_LEN)
      {
         /* DRM_REPLACE */         
         //FS_Close(aud_context_p->current_file_handle);
         DRM_close_file(aud_context_p->current_file_handle);      
         return MED_RES_MEM_INSUFFICIENT;
      }
      aud_context_p->melody_string_p=med_alloc_ext_mem(buf_len);
      if(aud_context_p->melody_string_p==NULL)
      {
         /* DRM_REPLACE */         
         //FS_Close(aud_context_p->current_file_handle);
         DRM_close_file(aud_context_p->current_file_handle);
         return MED_RES_MEM_INSUFFICIENT;
      }
      
      /* read data from file to buffer */
      /* DRM_REPLACE */
      /*FS_Read(aud_context_p->current_file_handle, 
               aud_context_p->melody_string_p, 
               buf_len,
               &len);*/
      DRM_read_file(aud_context_p->current_file_handle, 
               aud_context_p->melody_string_p, 
               buf_len,
               &len);

      /* close the file */
      /* DRM_REPLACE */         
      //FS_Close(aud_context_p->current_file_handle);
      DRM_close_file(aud_context_p->current_file_handle);

      if (len == 0)
      {
         /* free file buffer */
         aud_free_melody_file_buffer();
         return MED_RES_OPEN_FILE_FAIL;
      }
      else
      {
         switch(format)
         {
            case MED_TYPE_IMELODY:
            {
               Media_iMelody_Param* imy_param_p = NULL;
               Media_Status status = 0;
               
               if( (aud_context_p->last_handle = JImy_Open( aud_media_ievent_callback, &imy_param_p )) != NULL )
               {
                  imy_param_p->imyfile = (const kal_uint8*)aud_context_p->melody_string_p;
                  imy_param_p->filesize = len;
                  imy_param_p->repeats = aud_melody_play_style_convert(play_style);
                  imy_param_p->default_instrument = IMY_DEFAULT_INSTRUMENT;
                  JImy_SetStartTime( aud_context_p->last_handle, aud_context_p->last_time );
                  if( (status = JImy_Play( aud_context_p->last_handle )) == MEDIA_SUCCESS )
                  {
                     /* enter AUD_MEDIA_PLAY state */
                     AUD_ENTER_STATE(AUD_MEDIA_PLAY);
                  }
                  else
                  {
                     JImy_Close( aud_context_p->last_handle );
                     aud_context_p->last_handle = NULL;
                     /* free file buffer */
                     aud_free_melody_file_buffer();
                     /* stop volume crescendo timer */
                     med_stop_timer(AUD_TIMER_CRESCENDO);
                  }
               }
               else
               {
                  /* free file buffer */
                  aud_free_melody_file_buffer();
                  /* stop volume crescendo timer */
                  med_stop_timer(AUD_TIMER_CRESCENDO);
               }
               return aud_get_res(status);      
            }
            case MED_TYPE_SMF:
            {
               Media_SMF_Param* smf_param_p = NULL;
               Media_Status status = 0;
               
               if( (aud_context_p->last_handle = JSmf_Open( aud_media_ievent_callback, &smf_param_p )) != NULL )
               {
                  smf_param_p->smffile = (const kal_uint8*)aud_context_p->melody_string_p;
                  smf_param_p->filesize = len;
                  smf_param_p->repeats = aud_melody_play_style_convert(play_style);
                  JSmf_SetStartTime( aud_context_p->last_handle, aud_context_p->last_time );
                  if( (status = JSmf_Play( aud_context_p->last_handle )) == MEDIA_SUCCESS )
                  {
                     /* enter AUD_MEDIA_PLAY state */
                     AUD_ENTER_STATE(AUD_MEDIA_PLAY);
                  }
                  else
                  {
                     JSmf_Close( aud_context_p->last_handle );
                     aud_context_p->last_handle = NULL;
                     /* free file buffer */
                     aud_free_melody_file_buffer();
                     /* stop volume crescendo timer */
                     med_stop_timer(AUD_TIMER_CRESCENDO);
                  }
               }
               else
               {
                  /* free file buffer */
                  aud_free_melody_file_buffer();
                  /* stop volume crescendo timer */
                  med_stop_timer(AUD_TIMER_CRESCENDO);
               }
               return aud_get_res(status);  
            }
#ifdef MMF_DECODE
            case MED_TYPE_MMF:
            {
               Media_MMF_Param* mmf_param_p;
               Media_Status status;
               
               if( (aud_context_p->last_handle = JMmf_Open( aud_media_ievent_callback, &mmf_param_p )) != NULL )
               {
                  mmf_param_p->mmffile = (const kal_uint8*)aud_context_p->melody_string_p;
                  mmf_param_p->filesize = len;
                  mmf_param_p->repeats = aud_melody_play_style_convert(play_style);
                  JMmf_SetStartTime( aud_context_p->last_handle, aud_context_p->last_time );
                  if( (status = JMmf_Play( aud_context_p->last_handle )) == MEDIA_SUCCESS )
                  {
                     /* enter AUD_MEDIA_PLAY state */
                     AUD_ENTER_STATE(AUD_MEDIA_PLAY);
                  }
                  else
                  {
                     JMmf_Close( aud_context_p->last_handle );
                     aud_context_p->last_handle = NULL;
                     /* free file buffer */
                     aud_free_melody_file_buffer();
                     /* stop volume crescendo timer */
                     med_stop_timer(AUD_TIMER_CRESCENDO);
                  }
               }
               else
               {
                  /* free file buffer */
                  aud_free_melody_file_buffer();
                  /* stop volume crescendo timer */
                  med_stop_timer(AUD_TIMER_CRESCENDO);
               }
               return aud_get_res(status);  
            }
#endif
            default:
            {
               /* free file buffer */
               aud_free_melody_file_buffer();
               return MED_RES_INVALID_FORMAT;
            }
         }
         
      }
       
   }
   else
   {
      return MED_RES_OPEN_FILE_FAIL;
   }
}
#endif

kal_uint8 aud_melody_play_by_name(kal_wchar* file_name, kal_uint8 play_style, kal_uint8 audio_out_device)
{
   /* open the file */
   kal_int16 format;
   
   aud_context_p->source_type = AUD_FILE;

   format = med_get_media_type(file_name);

   /* set to the volume of ringtone */
   aud_set_active_ring_tone_volume();
   
   /* DRM_REPLACE */   
   //aud_context_p->current_file_handle = FS_Open(file_name, FS_READ_ONLY|FS_OPEN_NO_DIR);
   aud_context_p->current_file_handle = DRM_open_file((UINT8*)file_name, FS_READ_ONLY|FS_OPEN_NO_DIR, 0);   

   if (aud_context_p->current_file_handle >= 0)
   {
      INT32 len;
      INT32 buf_len;
      INT32 result;
      
      aud_context_p->audio_end=KAL_FALSE;
      /* DRM_REPLACE */
      //result=FS_GetFileSize(aud_context_p->current_file_handle, &buf_len);
      result=DRM_file_size(aud_context_p->current_file_handle, &buf_len);      
      if(!(result==FS_NO_ERROR && buf_len>0))
      {
         /* DRM_REPLACE */         
         //FS_Close(aud_context_p->current_file_handle);
         DRM_close_file(aud_context_p->current_file_handle);
         return MED_RES_BAD_FORMAT;
      }
      else if(buf_len>AUD_MELODY_BUFFER_LEN)
      {
         /* DRM_REPLACE */         
         //FS_Close(aud_context_p->current_file_handle);
         DRM_close_file(aud_context_p->current_file_handle);      
         return MED_RES_MEM_INSUFFICIENT;
      }
      aud_context_p->melody_string_p=med_alloc_ext_mem(buf_len);
      if(aud_context_p->melody_string_p==NULL)
      {
         /* DRM_REPLACE */         
         //FS_Close(aud_context_p->current_file_handle);
         DRM_close_file(aud_context_p->current_file_handle);
         return MED_RES_MEM_INSUFFICIENT;
      }
      
      /* read data from file to buffer */
      /* DRM_REPLACE */
      /*FS_Read(aud_context_p->current_file_handle, 
               aud_context_p->melody_string_p, 
               buf_len,
               &len);*/
      DRM_read_file(aud_context_p->current_file_handle, 
               aud_context_p->melody_string_p, 
               buf_len,
               &len);

      /* close the file */
      /* DRM_REPLACE */         
      //FS_Close(aud_context_p->current_file_handle);
      DRM_close_file(aud_context_p->current_file_handle);

      if (len == 0)
      {
         /* free file buffer */
         aud_free_melody_file_buffer();
         return MED_RES_BAD_FORMAT;
      }
      else
      {
         switch(format)
         {
            case MED_TYPE_IMELODY:
            {
#ifdef __MED_MMA_MOD__
               Media_iMelody_Param* imy_param_p = NULL;
               Media_Status status = 0;
               
               if( (aud_context_p->last_handle = JImy_Open( aud_media_ievent_callback, &imy_param_p )) != NULL )
               {
                  imy_param_p->imyfile = (const kal_uint8*)aud_context_p->melody_string_p;
                  imy_param_p->filesize = len;
                  imy_param_p->repeats = aud_melody_play_style_convert(play_style);
                  imy_param_p->default_instrument = IMY_DEFAULT_INSTRUMENT;
                    #ifdef __MED_BT_A2DP_MOD__
                    #if (defined(__BTCOOLSAND__)||defined(__BTRDA__))
                	    aud_bt_a2dp_open_codec((kal_int32)format);
                    #endif
                    #endif /* __MED_BT_A2DP_MOD__ */ 
                  if( (status = JImy_Play( aud_context_p->last_handle )) == MEDIA_SUCCESS )
                  {
                     /* enter AUD_MEDIA_PLAY state */
                     AUD_ENTER_STATE(AUD_MEDIA_PLAY);
                  }
                  else
                  {
                     JImy_Close( aud_context_p->last_handle );
                     aud_context_p->last_handle = NULL;
                     /* free file buffer */
                     aud_free_melody_file_buffer();
                     /* stop volume crescendo timer */
                     med_stop_timer(AUD_TIMER_CRESCENDO);
                  }
               }
               else
               {
                  /* free file buffer */
                  aud_free_melody_file_buffer();
                  
                  /* stop volume crescendo timer */
                  med_stop_timer(AUD_TIMER_CRESCENDO);
               }
#else
               Media_iMelody_Param msp;
               Media_Status status;

               msp.imyfile = (const kal_uint8*)aud_context_p->melody_string_p;
               msp.filesize = len;
               msp.repeats = aud_melody_play_style_convert(play_style);
               msp.default_instrument = IMY_DEFAULT_INSTRUMENT;
               Media_SetBuffer(aud_context_p->ring_buf, AUD_RING_BUFFER_LEN);
               if((status=Media_Play((Media_Format)MED_TYPE_IMELODY, aud_media_event_callback, &msp))==MEDIA_SUCCESS)
               {
                  /* enter AUD_MEDIA_PLAY state */
                  AUD_ENTER_STATE(AUD_MEDIA_PLAY);
               }
               else
               {
                  /* free file buffer */
                  aud_free_melody_file_buffer();
                  
                  /* stop volume crescendo timer */
                  med_stop_timer(AUD_TIMER_CRESCENDO);
               }
#endif

               return aud_get_res(status);      
            }
            case MED_TYPE_SMF:
            {
#ifdef __MED_MMA_MOD__
               Media_SMF_Param* smf_param_p = NULL;
               Media_Status status = 0;
               
               if( (aud_context_p->last_handle = JSmf_Open( aud_media_ievent_callback, &smf_param_p )) != NULL )
               {
                  smf_param_p->smffile = (const kal_uint8*)aud_context_p->melody_string_p;
                  smf_param_p->filesize = len;
                  smf_param_p->repeats = aud_melody_play_style_convert(play_style);
                    #ifdef __MED_BT_A2DP_MOD__
                    #if (defined(__BTCOOLSAND__)||defined(__BTRDA__))
                	    aud_bt_a2dp_open_codec((kal_int32)format);

                    #endif
                    #endif /* __MED_BT_A2DP_MOD__ */ 
                  if( (status = JSmf_Play( aud_context_p->last_handle )) == MEDIA_SUCCESS )
                  {
                     /* enter AUD_MEDIA_PLAY state */
                     AUD_ENTER_STATE(AUD_MEDIA_PLAY);
                  }
                  else
                  {
                     JSmf_Close( aud_context_p->last_handle );
                     aud_context_p->last_handle = NULL;
                     /* free file buffer */
                     aud_free_melody_file_buffer();
                     /* stop volume crescendo timer */
                     med_stop_timer(AUD_TIMER_CRESCENDO);
                  }
               }
               else
               {
                  /* free file buffer */
                  aud_free_melody_file_buffer();
                  
                  /* stop volume crescendo timer */
                  med_stop_timer(AUD_TIMER_CRESCENDO);
               }
#else
               Media_SMF_Param msp;
               Media_Status status;

               msp.smffile = (const kal_uint8*)aud_context_p->melody_string_p;
               msp.filesize = len;
               msp.repeats = aud_melody_play_style_convert(play_style);
               Media_SetBuffer(aud_context_p->ring_buf, AUD_RING_BUFFER_LEN);
               if((status=Media_Play((Media_Format)format, aud_media_event_callback, &msp))==MEDIA_SUCCESS)
               {
                  /* enter AUD_MEDIA_PLAY state */
                  AUD_ENTER_STATE(AUD_MEDIA_PLAY);
               }
               else
               {
                  /* free file buffer */
                  aud_free_melody_file_buffer();
                  
                  /* stop volume crescendo timer */
                  med_stop_timer(AUD_TIMER_CRESCENDO);
               }
#endif

               return aud_get_res(status);  
            }
            case MED_TYPE_MMF:
            {
#if defined( __MED_MMA_MOD__ ) && defined( MMF_DECODE )
               Media_MMF_Param* mmf_param_p;
               Media_Status status;
               
               if( (aud_context_p->last_handle = JMmf_Open( aud_media_ievent_callback, &mmf_param_p )) != NULL )
               {
                  mmf_param_p->mmffile = (const kal_uint8*)aud_context_p->melody_string_p;
                  mmf_param_p->filesize = len;
                  mmf_param_p->repeats = aud_melody_play_style_convert(play_style);
                  if( (status = JMmf_Play( aud_context_p->last_handle )) == MEDIA_SUCCESS )
                  {
                     /* enter AUD_MEDIA_PLAY state */
                     AUD_ENTER_STATE(AUD_MEDIA_PLAY);
                  }
                  else
                  {
                     JMmf_Close( aud_context_p->last_handle );
                     aud_context_p->last_handle = NULL;
                     /* free file buffer */
                     aud_free_melody_file_buffer();
                     /* stop volume crescendo timer */
                     med_stop_timer(AUD_TIMER_CRESCENDO);
                  }
               }
               else
               {
                  /* free file buffer */
                  aud_free_melody_file_buffer();
                  
                  /* stop volume crescendo timer */
                  med_stop_timer(AUD_TIMER_CRESCENDO);
               }
#else
            	 Media_MMF_Param mmp;
               Media_Status status;

               mmp.mmffile = (const kal_uint8*)aud_context_p->melody_string_p;
               mmp.filesize = len;
               mmp.repeats = aud_melody_play_style_convert(play_style);
               Media_SetBuffer(aud_context_p->ring_buf, AUD_RING_BUFFER_LEN);
               if((status=Media_Play((Media_Format)format, aud_media_event_callback, &mmp))==MEDIA_SUCCESS)
               {
                  /* enter AUD_MEDIA_PLAY state */
                  AUD_ENTER_STATE(AUD_MEDIA_PLAY);
               }
               else
               {
                  /* free file buffer */
                  aud_free_melody_file_buffer();
                  
                  /* stop volume crescendo timer */
                  med_stop_timer(AUD_TIMER_CRESCENDO);
               }
#endif

               return aud_get_res(status);  
            }
            default:
            {
               /* free file buffer */
               aud_free_melody_file_buffer();
               return MED_RES_INVALID_FORMAT;
            }
         }
         
      }
       
   }
   else
   {
      return MED_RES_OPEN_FILE_FAIL;
   }
}

#if 1 //use csd mci play api
extern VOID aud_media_Ring_play_ind_hdlr(MCI_ERR_T result );

kal_int32 aud_melody_play_by_string(kal_uint8* string, kal_uint8 play_style, kal_uint32 string_len, kal_uint8 format)
{
	UINT32 mciFormat = 0;
		if(DEVICE_AUDIO_PLAY_ONCE==play_style||0==play_style) 
		{
			play_style = FALSE;
		}
		else
		{
			play_style = TRUE;
		}
		
		switch(format)
		{
			case    MDI_FORMAT_AMR:
			case   MDI_FORMAT_AMR_WB:
				mciFormat = MCI_PLAY_MODE_AMR_RING;
				break;
			case   MDI_FORMAT_DAF:
				mciFormat = MCI_PLAY_MODE_MP3;
				break;
			case MDI_FORMAT_AAC:
				mciFormat = MCI_PLAY_MODE_AAC;
				break;
			case MDI_FORMAT_PCM_8K:
			case MDI_FORMAT_PCM_16K  :
				mciFormat = MCI_PLAY_MODE_PCM;
				break;
			case MDI_FORMAT_WAV:
				mciFormat = MCI_PLAY_MODE_WAV;
				break;
			case MDI_FORMAT_MID:
				mciFormat =MCI_PLAY_MODE_MID;				
				break;
			default:
				mmi_trace(1,"chenhe,aud_melody_play_by_string,wrong format is %d",format);
				return -1;
				break;
		}
	int32 result=MCI_AudioPlayBuffer((UINT32 *)string,string_len,play_style,aud_media_Ring_play_ind_hdlr,mciFormat,0);
	mmi_trace(1,"chenhe,aud_melody_play_by_string,result is %d",result);
	return result;
}
#else

kal_int32 aud_melody_play_by_string(kal_uint8* string, kal_uint8 play_style, kal_uint32 string_len, kal_uint8 format)
{
   kal_int32 res;
   
   aud_context_p->source_type = AUD_STRING;
   aud_context_p->current_format = format;
   aud_context_p->string_p = string;
   aud_context_p->buf_len = string_len;

   /* set to the volume of ringtone */
   aud_set_active_ring_tone_volume();

   switch (aud_context_p->current_format)
   {
      case MED_TYPE_SMF:
         res=aud_melody_play_smf(aud_context_p->string_p,
                              aud_context_p->buf_len,
                              play_style);
         break;                     
      case MED_TYPE_IMELODY:
         res=aud_melody_play_imy(aud_context_p->string_p,
                              aud_context_p->buf_len,
                              play_style);
         break;                     
      case MED_TYPE_MMF:
         res=aud_melody_play_mmf(aud_context_p->string_p,
                              aud_context_p->buf_len,
                              play_style);
         break;                     
      case MED_TYPE_GSM_FR:
      case MED_TYPE_GSM_HR:
      case MED_TYPE_GSM_EFR:
      case MED_TYPE_AMR:
      case MED_TYPE_AMR_WB:
      case MED_TYPE_DAF:
      case MED_TYPE_PCM_8K:
      case MED_TYPE_PCM_16K:
      case MED_TYPE_G711_ALAW:
      case MED_TYPE_G711_ULAW:
      case MED_TYPE_DVI_ADPCM:
      case MED_TYPE_WAV:
      case MED_TYPE_WAV_ALAW:
      case MED_TYPE_WAV_ULAW:
      case MED_TYPE_WAV_DVI_ADPCM:
      case MED_TYPE_AU:
      case MED_TYPE_AIFF:
         res=aud_melody_play_stream(aud_context_p->string_p,
                              aud_context_p->buf_len,
                              aud_context_p->current_format,
                              play_style);
         break;
#ifdef __AUD_NEW_INTERFACE__
#ifdef AAC_DECODE
      case MED_TYPE_AAC:
#endif
#ifdef WMA_DECODE
      case MED_TYPE_WMA:
#endif // WMA_DECODE
         res = aud_media_play_byte_stream( aud_context_p->string_p, aud_context_p->buf_len, play_style );
         break;
#endif // __AUD_NEW_INTERFACE__
      default:
         res=MED_RES_ERROR;
         break;
   }
   return res;
}
#endif //endif use csd audio play





#endif

