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
 *	aud_utility.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes common used functions of audio manager task.
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
/* Buffer Management */
#include "stacklib.h"        	/* basic type for dll, evshed, stacktimer */
#include "event_shed.h"       	/* event scheduler */
#include "stack_timer.h"      	/* stack timer */
/* for tst inject message */
//#include "tst_sap.h" /* wufasong delete 2007.06.16 */


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
#include "fat_fs.h"
#include "mmi_trace.h"       //added by elisa for mem_cpy

extern const unsigned char  L1SP_SPEAKER1;
extern const unsigned char  L1SP_SPEAKER2;
extern const unsigned char  L1SP_LOUD_SPEAKER;

kal_uint8 aud_audio_out_dev_convert(kal_uint8 speaker_id)
{
   kal_uint8 audio_device_out = L1SP_SPEAKER1;

   switch(speaker_id)
   {
   case AUDIO_DEVICE_SPEAKER:
      audio_device_out = L1SP_SPEAKER1;
      break;
   case AUDIO_DEVICE_SPEAKER2:
      audio_device_out = L1SP_SPEAKER2;
      break;
   case AUDIO_DEVICE_LOUDSPEAKER:
      audio_device_out = L1SP_LOUD_SPEAKER;
      break;
   case AUDIO_DEVICE_SPEAKER_BOTH:
      audio_device_out = L1SP_LOUD_SPEAKER|L1SP_SPEAKER2;
      break;
   case AUDIO_DEVICE_BUZZER:
      audio_device_out = L1SP_LOUD_SPEAKER;
      break;
   default:
      break;
   }
   
   return audio_device_out;
}

kal_uint8 aud_get_res(kal_uint8 result)
{
   switch(result)
   {
      case MEDIA_SUCCESS:
         return MED_RES_OK;
      case MEDIA_FAIL:
         return MED_RES_FAIL;
      case MEDIA_BAD_FORMAT:
      case MEDIA_BAD_PARAMETER:
         return MED_RES_BAD_FORMAT;
      case MEDIA_BAD_COMMAND:
      case MEDIA_REENTRY:
         return MED_RES_BUSY;
      case MEDIA_NO_HANDLER:
         return MED_RES_NO_HANDLE;
      case MEDIA_UNSUPPORTED_CHANNEL:
         return MED_RES_UNSUPPORTED_CHANNEL;
      case MEDIA_UNSUPPORTED_FREQ:
         return MED_RES_UNSUPPORTED_FREQ;
      case MEDIA_UNSUPPORTED_TYPE:
         return MED_RES_UNSUPPORTED_TYPE;
      case MEDIA_UNSUPPORTED_OPERATION:
         return MED_RES_UNSUPPORTED_OPERATION;
      default:
         return MED_RES_FAIL;

   }
}

kal_uint8 aud_check_disc_space(kal_wchar* dir_name, kal_int32 free_byte)
{
   kal_int32 result;
   FS_DiskInfo disc_info;
   kal_wchar dir[3];

   kal_mem_set(dir, 0, 6);
   kal_mem_cpy(dir, dir_name, 4);
   /* check the available free space */
   result=MMI_FS_GetDiskInfo((UINT8 *)dir, &disc_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);

   if(result>0 && result&(FS_DI_BASIC_INFO|FS_DI_FREE_SPACE))
   {
      if((disc_info.FreeClusters*disc_info.SectorsPerCluster*disc_info.BytesPerSector)<free_byte)
         return MED_RES_DISC_FULL;
      else
         return MED_RES_OK;
   }
   else
   {
      return MED_RES_NO_DISC;
   }
}
 

/******************************************************************
* FUNCTION
*    aud_is_streaming_type
* DESCRIPTION
*    This function is used to check if the audio file type is streaming.
* PARAMETERS
*    kal_uint8 format
*    
* RETURNS
*    kal_bool
* GLOBALS AFFECTED
*  
*  
******************************************************************/
kal_bool aud_is_streaming_type(kal_uint8 format)
{

   switch(format)
   {
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
#ifdef MP4_CODEC
      case MED_TYPE_M4A:
#endif            
         return KAL_TRUE;
      default:
         return KAL_FALSE;
   
   }
}


/*****************************************************************************
* FUNCTION
*     aud_create_full_path_folder
* DESCRIPTION
*     check or make dir along the assigned path
* IMPACT 
*     
* PARAMETERS
*     kal_uint16 *filepath
* RETURNS
*     int
* GLOBALS AFFECTED
*     
*****************************************************************************/
int aud_create_full_path_folder( kal_uint16 *filepath )
{
	kal_uint16* filepath_p = filepath;
	kal_int32 fs_ret = FS_FILE_EXISTS;
	FS_HANDLE file_handle;

	file_handle = MMI_FS_Open( (UINT8 *)filepath, FS_OPEN_DIR | FS_READ_ONLY );

	/* path already exist */
	if( file_handle >= 0 )
	{
		MMI_FS_Close( file_handle );
		return FS_FILE_EXISTS;
	}
	
	/* skip drive, i.e. "C:\" */
	while( *filepath_p != (kal_uint16)_T_MMI'\\' )
	{
		if( *filepath_p != 0 )
			filepath_p++;
		else
			return FS_INVALID_FILENAME;
	}
	
	while( *(filepath_p++) )
	{
		if( *filepath_p == (kal_uint16)_T_MMI'\\' )
		{
			*filepath_p = 0;
			file_handle = MMI_FS_Open( (UINT8 *)filepath, FS_OPEN_DIR | FS_READ_ONLY );
			/* check if path exists */
			if( file_handle < 0 )
			{
				/* create dir */
				fs_ret = MMI_FS_CreateDir( (UINT8 *)filepath );
				*filepath_p = (kal_uint16)_T_MMI'\\';
				if( fs_ret != FS_NO_ERROR )
					break;
			}
			else
			{
				MMI_FS_Close( file_handle );
				*filepath_p = (kal_uint16)_T_MMI'\\';
			}
		}
	}
	
	return fs_ret;
}

#ifdef  TST_HANDLER

void aud_tst_handler(ilm_struct *ilm_ptr)
{
   tst_module_string_inject_struct* tst_inject = (tst_module_string_inject_struct*)ilm_ptr->local_para_ptr;
   

   ASSERT( tst_inject != NULL );

   switch(tst_inject->index)
   {
      case 13: /* audio play by name*/
      {
         l4aud_audio_play_by_name_req_struct msg;
         kal_wsprintf(msg.file_name, "%s", tst_inject->string);
         msg.play_style = DEVICE_AUDIO_PLAY_INFINITE;
         msg.src_id = 0;
         ilm_ptr->local_para_ptr = (local_para_struct*)&msg;
         aud_play_by_name_req_hdlr(ilm_ptr);
         break;
      }
      case 14: /* audio stop by name*/
      {
         l4aud_audio_stop_by_name_req_struct msg;
         kal_wsprintf(msg.file_name, "%s", tst_inject->string);
         ilm_ptr->local_para_ptr = (local_para_struct*)&msg;
         aud_stop_by_name_req_hdlr(ilm_ptr);
         break;
      }
      case 15: /* audio play by string*/
      {
         l4aud_audio_play_by_string_req_struct msg;
         msg.melody = resource_midis[tst_inject->string[0]-'0'].data;
         msg.format = AUD_MEDIA_SMF;
         msg.play_style = DEVICE_AUDIO_PLAY_INFINITE;
         msg.len = resource_midis[tst_inject->string[0]-'0'].len;
         msg.src_id = 0;
         ilm_ptr->local_para_ptr = (local_para_struct*)&msg;
         aud_play_by_string_req_hdlr(ilm_ptr);
         break;
      }
      case 16: /* audio stop by string*/
      {
         l4aud_audio_stop_by_string_req_struct msg;
         msg.src_id = 0;
         ilm_ptr->local_para_ptr = (local_para_struct*)&msg;
         aud_stop_by_string_req_hdlr(ilm_ptr);
         break;
      }
   }

}

#endif /* TST_HANDLER */

#endif

