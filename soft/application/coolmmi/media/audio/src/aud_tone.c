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
 *	aud_tone .c
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
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/******************************************************************
 * MODULE
 *    AUD_TONE
 * DESCRIPTION
 *    This module defines the tone related function for Audio Manager.
 *  
 ******************************************************************/


#ifndef MED_NOT_PRESENT


/*==== INCLUDES ==========*/


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
#include "aud_main.h"
#include "med_main.h"



/******************************************************************
 * FUNCTION
 *    aud_tone_init
 * DESCRIPTION
 *    This function is used to init tone module for audio manager.
 * PARAMETERS
 *    
 *    
 * RETURNS
 *
 * GLOBALS AFFECTED
 *  
 *  
 ******************************************************************/

kal_bool aud_tone_init(void)
{
   aud_context_p->tone_playing = KAL_FALSE;
   return KAL_TRUE;
}

void aud_tone_set_output_device(kal_uint8 device)
{
   TONE_SetOutputDevice(device);
   //if(device!=L1SP_LOUD_SPEAKER)
      //TONE_SetFIR(KAL_TRUE);
   //else
      TONE_SetFIR(KAL_FALSE);

}

void aud_tone_set_output_volume(kal_uint8 volume1, kal_uint8 volume2)
{
   TONE_SetOutputVolume(volume1,volume2);

}

void aud_set_active_tone_volume(void)
{
   kal_uint8 volume;
   kal_uint8 mode=aud_context_p->audio_mode;

   /* tone follow speech volume */
   volume = aud_get_volume_gain(mode, AUD_VOLUME_SPH, aud_get_volume_level(mode, AUD_VOLUME_SPH));
   aud_tone_set_output_volume(volume, volume);
}


void aud_tone_play(kal_uint8 audio_id, kal_uint8 audio_out_device)
{
   
   TONE_SetOutputDevice(aud_audio_out_dev_convert(audio_out_device));

   TONE_Play((const L1SP_Tones *) resource_tones[audio_id-MIN_TONE_ID]);


}

 extern BOOL AudioStopStream();

void aud_tone_stop(void)
{
   AudioStopStream();
   aud_context_p->tone_playing = KAL_FALSE;
}


#endif

