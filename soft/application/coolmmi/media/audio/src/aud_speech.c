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
 *	aud_speech.c
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
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/******************************************************************
 * MODULE
 *    AUD_SPEECH
 * DESCRIPTION
 *    This module defines the speech related function for Audio Manager.
 *  
 ******************************************************************/


#ifndef MED_NOT_PRESENT


/*==== INCLUDES ===========*/


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
#include "med_status.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "aud_main.h"
#include "med_main.h"
#include "mmi_trace.h"
#ifdef __MED_VID_MOD__
#include "vid_main.h"
#endif


/******************************************************************
 * FUNCTION
 *    aud_speech_init
 * DESCRIPTION
 *    This function is used to init this module.
 * PARAMETERS
 *    
 *    
 * RETURNS
 *
 * GLOBALS AFFECTED
 *  
 *  
 ******************************************************************/

#ifdef MMI_ON_WIN32
aud_context_struct g_audcontext;
aud_context_struct*   aud_context_p = &g_audcontext;
#endif
kal_bool aud_speech_init(void)
{
   aud_context_p->speech_on = KAL_FALSE;
   return KAL_TRUE;
}

void aud_speech_set_output_device(kal_uint8 device)
{
}

void aud_speech_set_output_volume(kal_uint8 volume1, kal_uint8 volume2)
{
}

void aud_mic_set_device(kal_uint8 device)
{
}

/*Set input device to media task context only, apply to device later*/
void aud_mic_set_device_no_apply(kal_uint8 device)
{
}

void aud_mic_set_volume(kal_uint8 volume1, kal_uint8 volume2)
{
}

void aud_mic_mute(kal_bool mute)
{
}


void aud_side_tone_set_volume(kal_uint8 volume1, kal_uint8 volume2)
{
}



void aud_speech_set_fir(kal_bool fir_on)
{
}



void aud_speech_set_mode_req_hdlr(ilm_struct *ilm_ptr)
{
	l4aud_sp_set_mode_req_struct *sp_mode_req;

	sp_mode_req = ( l4aud_sp_set_mode_req_struct * )ilm_ptr->local_para_ptr ;
	mmi_trace(TRUE, "aud_speech_set_mode_req_hdlr %d",sp_mode_req->speech_on );
	
	if(sp_mode_req->speech_on)
	{
		aud_context_p->speech_on = KAL_TRUE ;
	}
	else
	{
		aud_context_p->speech_on = KAL_FALSE;
	}  
}

void  aud_speech_set_mode(kal_bool on)
{
	aud_context_p->speech_on = on;
}

/*JINZH Add Start For 5586 Ver: TBM780  on 2007-8-2 11:9 */
kal_bool aud_speech_get_mode()
{
	return aud_context_p->speech_on;
}
/*JINZH Add End  For 5586 Ver: TBM780  on 2007-8-2 11:9 */

#ifdef __MED_POC_MOD__

typedef  void (*media_handler)( Media_Handle *handle, Media_Event event );

kal_int32 aud_speech_amr_encode_start( kal_uint8 vocoder, void (*media_handler)(void), kal_int32 frame_threshold, kal_uint32 buffer_size )
{
   Media_Status result;

   ASSERT( buffer_size <= AUD_RING_BUFFER_LEN );
   
#ifdef CSD_KAL_MNT
   {
      extern kal_int32 poc_encode_start( void );
      status = poc_encode_start();
   }
#else
   PoC_AMR_SetBuffer( aud_context_p->ring_buf, buffer_size );
   result = PoC_AMR_EncodeStart( vocoder, media_handler, frame_threshold );
#endif

   return (kal_int32) result;
}

kal_int32 aud_speech_amr_encode_get_payload( kal_uint8 *buf, kal_int32 max_len, kal_int32* len_p )
{
   kal_int32 frames;
   
#ifdef CSD_KAL_MNT
   *len_p = 42;
   {
      const kal_uint8 test_rtp_payload[] = {		
                                 0x80, 0xed, 0x03, 0x0a, 0x00, 0x00, 0x49, 0xf4, 0x00, 0x00, 0x00, 0xff, 0x31, 0x32, 0x33, 0x34,
											0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
											0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30};

      memcpy( buf, test_rtp_payload, sizeof(test_rtp_payload) );
   }
#else
   frames = PoC_AMR_GetFrameCount();
   *len_p = PoC_AMR_GetPayload( buf, max_len, frames );
#endif

   return MED_SUCCESS;
}

kal_int32 aud_speech_amr_encode_stop( void )
{
   Media_Status result;

#ifdef CSD_KAL_MNT
   {
      extern kal_int32 poc_encode_stop( void );
      result = poc_encode_stop();
   }
#else
   result = PoC_AMR_EncodeStop();
#endif

   return (kal_int32) result;
}

kal_int32 aud_speech_amr_decode_start( kal_uint32 buffer_size )
{
   Media_Status result;

   ASSERT( buffer_size <= AUD_RING_BUFFER_LEN );
   
#ifdef CSD_KAL_MNT
   result = 0;
#else
   PoC_AMR_SetBuffer( aud_context_p->ring_buf, buffer_size );
   result = PoC_AMR_DecodeStart();
#endif

   return (kal_int32) result;
}

kal_int32 aud_speech_amr_decode_put_payload( kal_uint8 *buf, kal_int32 len )
{
   Media_Status result;

#ifdef CSD_KAL_MNT
   result = 0;
#else
   result = PoC_AMR_PutPayload( buf, len );
#endif

   return (kal_int32) result;
}

kal_int32 aud_speech_amr_decode_stop( void )
{
   Media_Status result;

#ifdef CSD_KAL_MNT
   {
      extern kal_int32 poc_decode_stop( void );
      result = poc_decode_stop();
   }
#else
   result = PoC_AMR_DecodeStop();
#endif

   return (kal_int32) result;
}

#endif // __MED_POC_MOD__

#endif

