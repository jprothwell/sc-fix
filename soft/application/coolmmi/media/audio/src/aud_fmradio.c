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
 *	aud_fmradio.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes FM Radio service functions
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
 
#ifndef MED_NOT_PRESENT

/*==== INCLUDES =========*/
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
#include "fat_fs.h"
#include "nvram_struct.h"
#include "nvram_user_defs.h"
#include "nvram_data_items.h"
#include "custom_nvram_editor_data_item.h"
#include "custom_equipment.h"

/* local includes */
#include "med_global.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "med_main.h"
#include "aud_main.h"

#include "media_others.h"

#include "hal_timers.h"     // added by kecx for fixing bug 13021 on 20090710
#include "cos.h"
#include "mci.h"
#include "mmi_trace.h"	


#ifdef __MED_FMR_MOD__
//extern unsigned char const FM_RADIO_INPUT_LEVEL_THRESHOLD; // defined in custom folder
uint8 const FM_RADIO_INPUT_LEVEL_THRESHOLD = 5;
#endif


extern U8 GetHandsetInPhone();  
extern void ToSetHandSetMode() ;
extern void ToSetEarpieceMode();
#if defined(__MMI_NOKIA_STYLE_N800__)
extern void MusicStartAudioMode() ;
extern void MusicEndAudioMode()  ;
#endif

extern void GpioSpeakerModeStart() ;
extern void GpioSpeakerModeEnd()  ;
extern void sxr_Sleep (u32 Period);
    
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE)
//beginning-- added by kecx for fixing bug 13021 on 20090710
static void FM_Delay(u16 iMs)
{
//    sxr_Sleep(iMs * HAL_TICK1S / 1000);
    COS_Sleep(iMs);
}
//end-- added by kecx for fixing bug 13021 on 20090710
#endif // (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE)

void aud_fmr_power_on_req_hdlr( ilm_struct *ilm_ptr )
{
#ifdef __MED_FMR_MOD__
	media_fmr_power_on_req_struct* msg_p = (media_fmr_power_on_req_struct*)ilm_ptr->local_para_ptr;
	aud_context_p->melody_output_device=msg_p->output_path;
		
	FMR_PowerOn();

//	if( aud_context_p->audio_mode == AUD_MODE_HEADSET )
//		aud_fmr_set_output_device( custom_cfg_hw_aud_output_path( msg_p->output_path ) );
	aud_fmr_set_output_device(AUDIO_DEVICE_SPEAKER2);
	if(custom_cfg_hw_aud_output_path( msg_p->output_path ) ==AUDIO_DEVICE_SPEAKER_BOTH)//chenhe fix bug 11449 
	{
		aud_fmr_set_output_device(AUDIO_DEVICE_SPEAKER_BOTH);
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE)
		#if (CSW_EXTENDED_API_AUDIO_VIDEO == 0)
		MCI_FmPlay();//i2s
		#endif
		FM_Delay(200);     // added by kecx for fixing bug 13021 on 20090710
		#ifdef __FF_AUDIO_SET__
		GpioSpeakerModeStart();
		#endif	
#endif // (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE)
	}

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
#if (CSW_EXTENDED_API_AUDIO_VIDEO == 0)
	MCI_FmPlay();
#endif // (CSW_EXTENDED_API_AUDIO_VIDEO == 0)
#if defined(ANALOG_TV_SUPPORT) && defined(__FF_AUDIO_SET__)
    GpioSpeakerModeStart();
#endif	
#endif // (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)

#endif
	AUD_SET_EVENT( AUD_EVT_FMR_ON );
}

void aud_fmr_power_off_req_hdlr( ilm_struct *ilm_ptr )
{
#ifdef __MED_FMR_MOD__
	#if (CSW_EXTENDED_API_AUDIO_VIDEO == 0)
	MCI_FmStop();
	#endif

	FMR_PowerOff(); 
	#ifdef __FF_AUDIO_SET__
	GpioSpeakerModeEnd();
	#endif	
#endif
	AUD_SET_EVENT( AUD_EVT_FMR_OFF );
}

void aud_fmr_set_freq_req_hdlr( ilm_struct *ilm_ptr )
{
#ifdef __MED_FMR_MOD__
	media_fmr_set_freq_req_struct* msg_p = (media_fmr_set_freq_req_struct*)ilm_ptr->local_para_ptr;
	FMR_SetFreq( msg_p->freq );
#endif
	AUD_SET_EVENT( AUD_EVT_FMR_SET );
}

void aud_fmr_mute_req_hdlr( ilm_struct *ilm_ptr )
{
#ifdef __MED_FMR_MOD__
	media_fmr_mute_req_struct* msg_p = (media_fmr_mute_req_struct*)ilm_ptr->local_para_ptr;
	FMR_Mute( msg_p->mute );
#endif
	AUD_SET_EVENT( AUD_EVT_FMR_MUTE );
}

void aud_fmr_check_is_valid_stop_req_hdlr( ilm_struct *ilm_ptr )
{
#ifdef __MED_FMR_MOD__
	media_fmr_check_is_valid_stop_req_struct* msg_p = (media_fmr_check_is_valid_stop_req_struct*)ilm_ptr->local_para_ptr;
	kal_uint8 result = FMR_ValidStop( msg_p->freq, (uint8)FM_RADIO_INPUT_LEVEL_THRESHOLD, msg_p->is_step_up );
	aud_context_p->src_mod = ilm_ptr->src_mod_id;
	aud_send_fmr_check_is_valid_stop_cnf( result );
#else
	aud_send_fmr_check_is_valid_stop_cnf( 1 );
#endif
}

void aud_fmr_get_signal_level_req_hdlr( ilm_struct *ilm_ptr )
{
#ifdef __MED_FMR_MOD__
	media_fmr_get_signal_level_req_struct* msg_p = (media_fmr_get_signal_level_req_struct*)ilm_ptr->local_para_ptr;
	kal_uint8 result;
	if( FMR_ValidStop( msg_p->freq, (uint8)FM_RADIO_INPUT_LEVEL_THRESHOLD, msg_p->is_step_up ) ) 
	{
		result = FMR_GetSignalLevel( msg_p->freq ) + 1;
		ASSERT(result != 0); // when it's 0, that means the result is overflow
	}
	else
		result = 0;
	aud_context_p->src_mod = ilm_ptr->src_mod_id;
	aud_send_fmr_get_signal_level_cnf( result );
#else
	aud_send_fmr_get_signal_level_cnf( 5 );
#endif
}

void aud_fmr_set_volume( kal_uint8 volume1 )
{
#ifdef __MED_FMR_MOD__
	FMR_SetOutputVolume( volume1, volume1 );
#endif
}

void aud_fmr_set_output_device( kal_uint8 device )
{
#ifdef __MED_FMR_MOD__
	FMR_SetOutputDevice( device );
#endif
}

#endif


