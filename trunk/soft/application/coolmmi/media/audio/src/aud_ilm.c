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
 *	aud_ilm.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes send-ilm related funcions of audio manager task.
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef MED_NOT_PRESENT


/*==== INCLUDES ===========*/

/* system includes */
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

/* local includes */
#include "med_global.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "aud_main.h"
#include "oslmemory.h"

#ifdef __MMI_SUPPORT_BACKLIGHT_SYNC__
/* -- added for turn on/off backlight by MMI task -- */
#include "mmiapi_struct.h"
#endif
#include "mmi_trace.h"
#define aud_send_msg_to_med( src_id, msg_id, local_param_ptr ) aud_send_msg(src_id, MOD_MED, msg_id, local_param_ptr)
#define aud_send_msg_from_med( dst_id, msg_id, local_param_ptr ) aud_send_msg(MOD_MED, dst_id, msg_id, local_param_ptr)
#define kal_mem_cpy(a,b,c) memcpy(a,b,c)

extern HANDLE GetMmiTaskHandle(module_type mod);

extern kal_int32 aud_result;
/******************************************************************
 * FUNCTION
 *    aud_send_ilm
 * DESCRIPTION
 *    This function is used to send ilm.
 * PARAMETERS
 *    
 *    
 * RETURNS
 *
 * GLOBALS AFFECTED
 *  
 *  
 ******************************************************************/
void aud_send_ilm(module_type dest_id, 
                     kal_uint32 msg_id, 
                     void *local_param_ptr, 
                     void *peer_buf_ptr)
{
   ilm_struct       *ilm_ptr = NULL;

   switch(dest_id)
   {
      case MOD_MED:
         ilm_ptr = allocate_ilm(MOD_L1SP);
         ilm_ptr->src_mod_id = MOD_L1SP;
         ilm_ptr->sap_id = MED_SAP;
         break;
      case MOD_UEM:
         ilm_ptr = allocate_ilm(MOD_MED);
         ilm_ptr->src_mod_id = MOD_MED;
         ilm_ptr->sap_id = MED_SAP;
         break;
      case MOD_NVRAM:
         ilm_ptr = allocate_ilm(MOD_MED);
         ilm_ptr->src_mod_id = MOD_MED;
         ilm_ptr->sap_id = PS_NVRAM_SAP;
         break;
      default:
         ilm_ptr = allocate_ilm(MOD_MED);
         ilm_ptr->src_mod_id = MOD_MED;
         ilm_ptr->sap_id = MED_SAP;
         break;
   }
   ilm_ptr->msg_id = (msg_type_t)msg_id;
   ilm_ptr->local_para_ptr = (local_para_struct*)local_param_ptr;
   ilm_ptr->peer_buff_ptr = (peer_buff_struct*) peer_buf_ptr;

   ilm_ptr->dest_mod_id = dest_id;

   msg_send_ext_queue(ilm_ptr);

} 

void aud_send_msg( module_type src_id, module_type dst_id, kal_uint32 msg_id, void *local_param_ptr )
{
   ilm_struct *ilm_ptr = NULL;
   ilm_ptr = allocate_ilm(src_id);
   ilm_ptr->src_mod_id = src_id;
   ilm_ptr->dest_mod_id = dst_id;
   ilm_ptr->sap_id = MED_SAP;
   ilm_ptr->msg_id = (msg_type_t)msg_id;
   ilm_ptr->local_para_ptr = (local_para_struct*)local_param_ptr;
   ilm_ptr->peer_buff_ptr = (peer_buff_struct*) NULL;
   msg_send_ext_queue(ilm_ptr);
}

void aud_send_audio_play_finish_ind(kal_uint8   result)
{
   l4aud_audio_play_finish_ind_struct *ind_p;
   

    
   ind_p = (l4aud_audio_play_finish_ind_struct*)
            construct_local_para(sizeof(l4aud_audio_play_finish_ind_struct), TD_CTRL);  

   ind_p->result = result;
   ind_p->identifier = aud_context_p->identifier;

  aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_AUDIO_PLAY_FINISH_IND, 
               ind_p, 
               NULL);
}


void aud_send_msg_to_nvram(msg_type_t msg_name, 
                                  kal_uint16 ef_id,
                                  void *data_ptr,
                                  kal_uint16 length)
{
   kal_bool ret_val    = KAL_TRUE;
   peer_buff_struct     *data_stream;
   local_para_struct    *parm_stream;
   kal_uint16           pdu_len;


   switch (msg_name)
   {
   case MSG_ID_NVRAM_WRITE_REQ:
      switch (ef_id)
      {
         case NVRAM_EF_CUST_ACOUSTIC_DATA_LID:
         case NVRAM_EF_AUDIO_PARAM_LID:
#ifdef ISP_SUPPORT
         case NVRAM_EF_CAMERA_PARA_LID:
   #if (!defined(MT6219))
         case NVRAM_EF_CAMERA_DEFECT_LID:
         case NVRAM_EF_CAMERA_LENS_LID:
      #if (!defined(MT6228))
         case NVRAM_EF_CAMERA_GAMMA_LID:
      #endif
   #endif
#endif
            parm_stream =  construct_local_para( sizeof(nvram_write_req_struct), TD_CTRL );
            data_stream =  construct_peer_buff( length, 0, 0, TD_CTRL );

            ((nvram_write_req_struct *)parm_stream) -> file_idx = (kal_uint8) ef_id;
            ((nvram_write_req_struct *)parm_stream) -> para = 1;
            /* keep the function id in NVRAM message from UEM */
            ((nvram_write_req_struct *)parm_stream) -> access_id = aud_context_p->func_id;

            pdu_len = length;
            kal_mem_cpy(get_pdu_ptr(data_stream, &pdu_len), data_ptr, length);

            aud_send_ilm(MOD_NVRAM, 
                  MSG_ID_NVRAM_WRITE_REQ, 
                  parm_stream, 
                  data_stream);
            break;
         default:
            /* error write */
            break;
      }
      break;
   case MSG_ID_NVRAM_READ_REQ:
      switch (ef_id)
      {
         case NVRAM_EF_CUST_ACOUSTIC_DATA_LID:
         case NVRAM_EF_AUDIO_PARAM_LID:
#ifdef ISP_SUPPORT
         case NVRAM_EF_CAMERA_PARA_LID:
   #if (!defined(MT6219))
         case NVRAM_EF_CAMERA_DEFECT_LID:
         case NVRAM_EF_CAMERA_LENS_LID:
      #if (!defined(MT6228))
         case NVRAM_EF_CAMERA_GAMMA_LID:
      #endif
   #endif
#endif
            parm_stream =  construct_local_para( sizeof(nvram_read_req_struct), TD_CTRL );

            ((nvram_read_req_struct *)parm_stream) -> file_idx = (kal_uint8)ef_id;
            ((nvram_read_req_struct *)parm_stream) -> para = 1;

            aud_send_ilm(MOD_NVRAM, 
                  MSG_ID_NVRAM_READ_REQ, 
                  parm_stream, 
                  NULL);
            break;
         default:
            /* error read */
            break;
      }
      break;
   default:
      ret_val = KAL_FALSE;
      break;
   } /* End Switch */

   return;
}

void aud_send_startup_cnf(kal_uint8 result)
{
   med_startup_cnf_struct* cnf_p = NULL;
   

   cnf_p = (med_startup_cnf_struct*)
            construct_local_para(sizeof(med_startup_cnf_struct), TD_CTRL);  
                            
    
   cnf_p->result = result;
   
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_MED_STARTUP_CNF, 
               cnf_p, 
               NULL);

}

void aud_send_set_audio_profile_cnf(kal_uint8 result)
{
   l4aud_set_audio_profile_cnf_struct* cnf_p = NULL;
   

   cnf_p = (l4aud_set_audio_profile_cnf_struct*)
            construct_local_para(sizeof(l4aud_set_audio_profile_cnf_struct), TD_CTRL);  
                            
    
   cnf_p->src_id = aud_context_p->src_id;
   cnf_p->result = (kal_bool)result;
   cnf_p->cause = result;
   
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_SET_AUDIO_PROFILE_CNF, 
               cnf_p, 
               NULL);

}

void aud_send_set_audio_param_cnf(kal_uint8 result)
{
   l4aud_set_audio_param_cnf_struct* cnf_p = NULL;
   

   cnf_p = (l4aud_set_audio_param_cnf_struct*)
            construct_local_para(sizeof(l4aud_set_audio_param_cnf_struct), TD_CTRL);  
                            
    
   cnf_p->src_id = aud_context_p->src_id;
   cnf_p->result = (kal_bool)result;
   cnf_p->cause = result;
   
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_SET_AUDIO_PARAM_CNF, 
               cnf_p, 
               NULL);

}


#ifdef VM_SUPPORT

void aud_send_vm_get_info_cnf(void)
{
   kal_uint8 i;
   l4aud_vm_get_info_cnf_struct* cnf_p = NULL;
   

   cnf_p = (l4aud_vm_get_info_cnf_struct*)
            construct_local_para(sizeof(l4aud_vm_get_info_cnf_struct), TD_CTRL);  
                            
    
   cnf_p->src_id = aud_context_p->src_id;
   cnf_p->vm_info.no_of_vm = aud_context_p->vm_info.num_of_vm;
   for(i=0; i<aud_context_p->vm_info.num_of_vm && i<MAX_NUM_OF_VM_LIST; i++)
   {
      kal_mem_cpy(cnf_p->vm_info.file_name_list[i], 
                  aud_context_p->vm_info.file_name_list[i],
                  MAX_VM_FILE_NAME*ENCODE_BYTE);
   }
   cnf_p->vm_info.more_entry = KAL_FALSE;
   cnf_p->vm_info.free_space = aud_context_p->vm_info.free_space;
   
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_VM_GET_INFO_CNF, 
               cnf_p, 
               NULL);

}
void aud_send_vm_record_cnf(kal_uint8   result)
{
   l4aud_vm_record_cnf_struct* cnf_p = NULL;
   

   cnf_p = (l4aud_vm_record_cnf_struct*)
            construct_local_para(sizeof(l4aud_vm_record_cnf_struct), TD_CTRL);  
                            
    
   cnf_p->src_id = aud_context_p->src_id;
   cnf_p->result = result;
                     
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_VM_RECORD_CNF, 
               cnf_p, 
               NULL);

}

void aud_send_vm_play_cnf(kal_uint8   result)
{
   l4aud_vm_play_cnf_struct* cnf_p = NULL;
   

   cnf_p = (l4aud_vm_play_cnf_struct*)
            construct_local_para(sizeof(l4aud_vm_play_cnf_struct), TD_CTRL);  
                            
    
   cnf_p->src_id = aud_context_p->src_id;
   cnf_p->result = result;
                     
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_VM_PLAY_CNF, 
               cnf_p, 
               NULL);

}

void aud_send_vm_stop_cnf(kal_uint8   result, kal_wchar    *file_name)
{
   l4aud_vm_stop_cnf_struct* cnf_p = NULL;
   

   cnf_p = (l4aud_vm_stop_cnf_struct*)
            construct_local_para(sizeof(l4aud_vm_stop_cnf_struct), TD_CTRL);  
                            
    
   cnf_p->src_id = aud_context_p->src_id;
   cnf_p->result = result;
   if (file_name)
      kal_mem_cpy(cnf_p->file_name, file_name, MAX_VM_FILE_NAME*ENCODE_BYTE);
   else
      cnf_p->file_name[0] = 0;
                     
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_VM_STOP_CNF, 
               cnf_p, 
               NULL);

}

void aud_send_vm_abort_cnf(kal_uint8   result)
{
   l4aud_vm_abort_cnf_struct* cnf_p = NULL;
   

   cnf_p = (l4aud_vm_abort_cnf_struct*)
            construct_local_para(sizeof(l4aud_vm_abort_cnf_struct), TD_CTRL);  
                            
    
   cnf_p->src_id = aud_context_p->src_id;
   cnf_p->result = result;
                     
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_VM_ABORT_CNF, 
               cnf_p, 
               NULL);

}

void aud_send_vm_pause_cnf(kal_uint8   result, kal_wchar    *file_name)
{
   l4aud_vm_pause_cnf_struct* cnf_p = NULL;
   

   cnf_p = (l4aud_vm_pause_cnf_struct*)
            construct_local_para(sizeof(l4aud_vm_pause_cnf_struct), TD_CTRL);  
                            
    
   cnf_p->src_id = aud_context_p->src_id;
   cnf_p->result = result;
   if (file_name)
      kal_mem_cpy(cnf_p->file_name, file_name, MAX_VM_FILE_NAME*ENCODE_BYTE);
   else
      cnf_p->file_name[0] = 0;
                     
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_VM_PAUSE_CNF, 
               cnf_p, 
               NULL);

}

void aud_send_vm_resume_cnf(kal_uint8   result)
{
   l4aud_vm_resume_cnf_struct* cnf_p = NULL;
   

   cnf_p = (l4aud_vm_resume_cnf_struct*)
            construct_local_para(sizeof(l4aud_vm_resume_cnf_struct), TD_CTRL);  
                            
    
   cnf_p->src_id = aud_context_p->src_id;
   cnf_p->result = result;
                     
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_VM_RESUME_CNF, 
               cnf_p, 
               NULL);

}

void aud_send_vm_append_cnf(kal_uint8   result)
{
   l4aud_vm_append_cnf_struct* cnf_p = NULL;
   

   cnf_p = (l4aud_vm_append_cnf_struct*)
            construct_local_para(sizeof(l4aud_vm_append_cnf_struct), TD_CTRL);  
                            
    
   cnf_p->src_id = aud_context_p->src_id;
   cnf_p->result = result;
                     
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_VM_APPEND_CNF, 
               cnf_p, 
               NULL);

}

void aud_send_vm_delete_cnf(kal_uint8   result)
{
   l4aud_vm_delete_cnf_struct* cnf_p = NULL;
   

   cnf_p = (l4aud_vm_delete_cnf_struct*)
            construct_local_para(sizeof(l4aud_vm_delete_cnf_struct), TD_CTRL);  
                            
    
   cnf_p->src_id = aud_context_p->src_id;
   cnf_p->result = result;
                     
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_VM_DELETE_CNF, 
               cnf_p, 
               NULL);

}

void aud_send_vm_rename_cnf(kal_uint8   result)
{
   l4aud_vm_rename_cnf_struct* cnf_p = NULL;
   

   cnf_p = (l4aud_vm_rename_cnf_struct*)
            construct_local_para(sizeof(l4aud_vm_rename_cnf_struct), TD_CTRL);  
                            
    
   cnf_p->src_id = aud_context_p->src_id;
   cnf_p->result = result;
                     
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_VM_RENAME_CNF, 
               cnf_p, 
               NULL);

}

void aud_send_vm_record_finish_ind(kal_uint8   result, kal_wchar    *file_name)
{
   l4aud_vm_record_finish_ind_struct *ind_p;
   

    
   ind_p = (l4aud_vm_record_finish_ind_struct*)
            construct_local_para(sizeof(l4aud_vm_record_finish_ind_struct), TD_CTRL);  

   ind_p->src_id = aud_context_p->src_id;
   ind_p->result = result;
   if (file_name)
      kal_mem_cpy(ind_p->file_name, file_name, MAX_VM_FILE_NAME*ENCODE_BYTE);
   else
      ind_p->file_name[0] = 0;

   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_VM_RECORD_FINISH_IND, 
               ind_p, 
               NULL);
}

void aud_send_vm_play_finish_ind(kal_uint8   result)
{
   l4aud_vm_play_finish_ind_struct *ind_p;
   

    
   ind_p = (l4aud_vm_play_finish_ind_struct*)
            construct_local_para(sizeof(l4aud_vm_play_finish_ind_struct), TD_CTRL);  

   ind_p->src_id = aud_context_p->src_id;
   ind_p->result = result;

   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_VM_PLAY_FINISH_IND, 
               ind_p, 
               NULL);
}


#endif


void aud_send_media_record_cnf(kal_uint8   result)
{
   l4aud_media_record_cnf_struct* cnf_p = NULL;
   

   cnf_p = (l4aud_media_record_cnf_struct*)
            construct_local_para(sizeof(l4aud_media_record_cnf_struct), TD_CTRL);  
                            
    
   cnf_p->src_id = aud_context_p->src_id;
   cnf_p->result = result;
                     
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_MEDIA_RECORD_CNF, 
               cnf_p, 
               NULL);

}

void aud_send_media_play_cnf(kal_uint8   result)
{
   l4aud_media_play_cnf_struct* cnf_p = NULL;
   

   cnf_p = (l4aud_media_play_cnf_struct*)
            construct_local_para(sizeof(l4aud_media_play_cnf_struct), TD_CTRL);  
                            
    
   cnf_p->src_id = aud_context_p->src_id;
   cnf_p->result = result;
                     
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_MEDIA_PLAY_CNF, 
               cnf_p, 
               NULL);

}

void aud_send_media_stop_cnf(kal_uint8   result, kal_wchar    *file_name)
{
   l4aud_media_stop_cnf_struct* cnf_p = NULL;
   

   cnf_p = (l4aud_media_stop_cnf_struct*)
            construct_local_para(sizeof(l4aud_media_stop_cnf_struct), TD_CTRL);  
                            
    
   cnf_p->src_id = aud_context_p->src_id;
   cnf_p->result = result;
                      
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_MEDIA_STOP_CNF, 
               cnf_p, 
               NULL);

}


void aud_send_media_pause_cnf(kal_uint8   result, kal_wchar    *file_name)
{
   l4aud_media_pause_cnf_struct* cnf_p = NULL;
   

   cnf_p = (l4aud_media_pause_cnf_struct*)
            construct_local_para(sizeof(l4aud_media_pause_cnf_struct), TD_CTRL);  
                            
    
   cnf_p->src_id = aud_context_p->src_id;
   cnf_p->result = result;
                     
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_MEDIA_PAUSE_CNF, 
               cnf_p, 
               NULL);

}

void aud_send_media_resume_cnf(kal_uint8   result)
{
   l4aud_media_resume_cnf_struct* cnf_p = NULL;
   

   cnf_p = (l4aud_media_resume_cnf_struct*)
            construct_local_para(sizeof(l4aud_media_resume_cnf_struct), TD_CTRL);  
                            
    
   cnf_p->src_id = aud_context_p->src_id;
   cnf_p->result = result;
                     
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_MEDIA_RESUME_CNF, 
               cnf_p, 
               NULL);

}

void aud_send_media_record_finish_ind2(ilm_struct *ilm_ptr)
{
   
	l4aud_media_record_finish_ind_struct *ind_p = NULL;
	l4aud_media_record_finish_ind_struct *msg_p = NULL;
	 
	ind_p = (l4aud_media_record_finish_ind_struct*)
			 construct_local_para(sizeof(l4aud_media_record_finish_ind_struct), TD_CTRL);  
	
   msg_p = (l4aud_media_record_finish_ind_struct*)ilm_ptr->local_para_ptr;
   
	ind_p->src_id = msg_p->src_id;
	ind_p->result = msg_p->result;
	mmi_trace(1,"jiashuo aud_send_media_record_finish_ind2->msg_p->result:%d",msg_p->result);
   AUD_ENTER_STATE(AUD_MEDIA_IDLE);
   
	aud_send_ilm(MOD_MMI, 
				MSG_ID_L4AUD_MEDIA_RECORD_FINISH_IND, 
				ind_p, 
				NULL);

}


void aud_send_media_record_finish_ind(kal_uint8   result)
{
   l4aud_media_record_finish_ind_struct *ind_p;
   

    
   ind_p = (l4aud_media_record_finish_ind_struct*)
            construct_local_para(sizeof(l4aud_media_record_finish_ind_struct), TD_CTRL);  

   ind_p->src_id = aud_context_p->src_id;
   ind_p->result = result;

   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_MEDIA_RECORD_FINISH_IND, 
               ind_p, 
               NULL);
}


void aud_send_media_play_finish_ind2(ilm_struct *ilm_ptr)
{
   l4aud_media_play_finish_ind_struct *ind_p = NULL;
   l4aud_media_play_finish_ind_struct *msg_p = NULL;
  mmi_trace(1," aud_send_media_play_finish_ind2");
    
   ind_p = (l4aud_media_play_finish_ind_struct*)
            construct_local_para(sizeof(l4aud_media_play_finish_ind_struct), TD_CTRL);  
   
  msg_p = (l4aud_media_play_finish_ind_struct*)ilm_ptr->local_para_ptr;
  
   ind_p->src_id = msg_p->src_id;
   ind_p->result = msg_p->result;
   ind_p->identifier = msg_p->identifier;
   //MMI_FS_Close(aud_context_p->current_file_handle);
   if(aud_context_p->current_file_handle>=0)
   	MMI_FS_Seek(aud_context_p->current_file_handle,0x0,FS_FILE_BEGIN);
  AUD_ENTER_STATE(AUD_MEDIA_IDLE);
  
   aud_send_ilm(MOD_MMI, 
               MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND, 
               ind_p, 
               NULL);
}


void aud_send_media_play_finish_ind(kal_uint8   result)
{
   l4aud_media_play_finish_ind_struct *ind_p;
   

    
   ind_p = (l4aud_media_play_finish_ind_struct*)
            construct_local_para(sizeof(l4aud_media_play_finish_ind_struct), TD_CTRL);  
  
   ind_p->src_id = aud_context_p->src_id;
   ind_p->result = result;
   ind_p->identifier = aud_context_p->identifier;
  
   aud_send_ilm(aud_context_p->src_mod, 
               MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND, 
               ind_p, 
               NULL);
}

/* for aud_api.c, to send message to media task */


void aud_send_play_id_req(module_type src_mod_id, void* id_param)
{
   aud_play_id_struct *data_p=(aud_play_id_struct*)id_param;
   l4aud_audio_play_req_struct* msg_p = (l4aud_audio_play_req_struct*)
            construct_local_para(sizeof(l4aud_audio_play_req_struct), TD_CTRL);  
                            
   msg_p->audio_id = data_p->audio_id;
   msg_p->play_style = data_p->play_style;
   msg_p->volume = data_p->volume;
   msg_p->output_path = data_p->output_path;
   msg_p->identifier = data_p->identifier;

   aud_send_msg_to_med( src_mod_id, MSG_ID_L4AUD_AUDIO_PLAY_REQ, msg_p );
}


void aud_send_stop_id_req(module_type src_mod_id, kal_uint8 audio_id)
{
   l4aud_audio_stop_req_struct* msg_p = (l4aud_audio_stop_req_struct*)
            construct_local_para(sizeof(l4aud_audio_stop_req_struct), TD_CTRL);  
                            
   msg_p->audio_id = audio_id;

   aud_send_msg_to_med( src_mod_id, MSG_ID_L4AUD_AUDIO_STOP_REQ, msg_p );
}

void aud_send_play_string_req(module_type src_mod_id, void* string_param)
{
   aud_play_string_struct *data_p=(aud_play_string_struct*)string_param;
   l4aud_audio_play_by_string_req_struct* msg_p = (l4aud_audio_play_by_string_req_struct*)
            construct_local_para(sizeof(l4aud_audio_play_by_string_req_struct), TD_CTRL);  
                            
   msg_p->src_id = 0;
   msg_p->melody = (const kal_uint8*)data_p->data_p;
   msg_p->format = data_p->format;
   msg_p->len = data_p->len;
   msg_p->play_style = data_p->play_style;
   msg_p->volume = data_p->volume;
   msg_p->output_path = data_p->output_path;
   msg_p->identifier = data_p->identifier;
   msg_p->blocking = data_p->blocking;

   aud_send_msg_to_med( src_mod_id, MSG_ID_L4AUD_AUDIO_PLAY_BY_STRING_REQ, msg_p );
}

void aud_send_stop_string_req(module_type src_mod_id, kal_uint8 src_id)
{
   l4aud_audio_stop_by_string_req_struct* msg_p = (l4aud_audio_stop_by_string_req_struct*)
            construct_local_para(sizeof(l4aud_audio_stop_by_string_req_struct), TD_CTRL);  
                            
   msg_p->src_id = src_id;

   aud_send_msg_to_med( src_mod_id, MSG_ID_L4AUD_AUDIO_STOP_BY_STRING_REQ, msg_p );
}
extern INT32 mdi_audio_file_playStyle;

void aud_send_play_file_req(module_type src_mod_id, void* file_param)
{
   aud_play_file_struct *data_p=(aud_play_file_struct*)file_param;
   l4aud_media_play_req_struct* msg_p = (l4aud_media_play_req_struct*)
            construct_local_para(sizeof(l4aud_media_play_req_struct), TD_CTRL);  

   if (NULL == msg_p)
   {
      AUD_SET_EVENT(AUD_EVT_PLAY);
      aud_result = 1;
      return;
   }

   msg_p->src_id = 0;
   /*WeiD Modify Start For 6208 Ver: TBM780  on 2007-8-17 15:2 */
   kal_mem_cpy(msg_p->file_name, data_p->file_name_p, MAX_MEDIA_ALL_FILE_NAME);
   /*WeiD Modify End  For 6208 Ver: TBM780  on 2007-8-17 15:2 */
   msg_p->play_style = data_p->play_style;
   mdi_audio_file_playStyle = msg_p->play_style ;//to record file play stye in mdi layer
   msg_p->volume = data_p->volume;
   msg_p->output_path = data_p->output_path;
   msg_p->identifier = data_p->identifier;
   msg_p->start_offset = data_p->start_offset;
   msg_p->end_offset = data_p->end_offset;
   /*WeiD Modify Start For 6208 Ver: TBM780  on 2007-8-17 15:2 */
   msg_p->file_name[MAX_MEDIA_ALL_FILE_NAME -1] = 0;
   /*WeiD Modify End  For 6208 Ver: TBM780  on 2007-8-17 15:2 */
   mmi_trace(g_sw_MED_Aud, "aud_send_play_file_req,send mp3 message success MSG_ID_L4AUD_MEDIA_PLAY_REQ = %d", MSG_ID_L4AUD_MEDIA_PLAY_REQ);
   aud_send_msg_to_med( src_mod_id, MSG_ID_L4AUD_MEDIA_PLAY_REQ, msg_p );
}


void aud_send_stop_file_req(module_type src_mod_id, kal_uint8 src_id)
{
   l4aud_media_stop_req_struct* msg_p = (l4aud_media_stop_req_struct*)
            construct_local_para(sizeof(l4aud_media_stop_req_struct), TD_CTRL);  
                            
   msg_p->src_id = src_id;

   aud_send_msg_to_med( src_mod_id, MSG_ID_L4AUD_MEDIA_STOP_REQ, msg_p );
}

//void aud_send_record_req(module_type src_mod_id, kal_wchar* file_name, kal_uint8 format, kal_bool default_input, kal_uint8 input_source)
void aud_send_record_req(  module_type src_mod_id,   kal_wchar *file_name, kal_uint8 format,  kal_uint8 quality,    kal_bool default_input,   kal_uint8 input_source)
{
   l4aud_media_record_req_struct* msg_p = (l4aud_media_record_req_struct*)
            construct_local_para(sizeof(l4aud_media_record_req_struct), TD_CTRL);  
                            
   kal_mem_cpy(msg_p->file_name, file_name, MAX_MEDIA_FILE_NAME*ENCODE_BYTE);
   msg_p->format = format;
   msg_p->default_input = default_input;
   msg_p->input_source = input_source;
   msg_p->quality = quality;
   aud_send_msg_to_med( src_mod_id, MSG_ID_L4AUD_MEDIA_RECORD_REQ, msg_p );
}

void aud_send_stop_record_req(module_type src_mod_id, kal_uint8 src_id)
{
   l4aud_media_stop_req_struct* msg_p = (l4aud_media_stop_req_struct*)
            construct_local_para(sizeof(l4aud_media_stop_req_struct), TD_CTRL);  

   msg_p->src_id = src_id;

   aud_send_msg_to_med( src_mod_id, MSG_ID_L4AUD_MEDIA_STOP_REQ, msg_p );
}

void aud_send_pause_req(module_type src_mod_id, kal_uint8 src_id)
{
   l4aud_media_pause_req_struct* msg_p = (l4aud_media_pause_req_struct*)
            construct_local_para(sizeof(l4aud_media_pause_req_struct), TD_CTRL);  
                            
   msg_p->src_id = src_id;

   aud_send_msg_to_med( src_mod_id, MSG_ID_L4AUD_MEDIA_PAUSE_REQ, msg_p );
}

void aud_send_resume_req(module_type src_mod_id, kal_uint8 src_id)
{
   l4aud_media_resume_req_struct* msg_p = (l4aud_media_resume_req_struct*)
            construct_local_para(sizeof(l4aud_media_resume_req_struct), TD_CTRL);  
                            
   msg_p->src_id = src_id;

   aud_send_msg_to_med( src_mod_id, MSG_ID_L4AUD_MEDIA_RESUME_REQ, msg_p );
}

void aud_send_store_req(module_type src_mod_id, kal_uint8 src_id)
{
   l4aud_media_store_req_struct* msg_p = (l4aud_media_store_req_struct*)
            construct_local_para(sizeof(l4aud_media_store_req_struct), TD_CTRL);  
                            
   msg_p->src_id = src_id;

   aud_send_msg_to_med( src_mod_id, MSG_ID_L4AUD_MEDIA_STORE_REQ, msg_p );
}


void aud_send_restore_req(module_type src_mod_id, void* file_param)
{
   aud_play_file_struct *data_p=(aud_play_file_struct*)file_param;
   l4aud_media_restore_req_struct* msg_p = (l4aud_media_restore_req_struct*)
            construct_local_para(sizeof(l4aud_media_restore_req_struct), TD_CTRL);  
                            
   msg_p->src_id = 0;
   kal_mem_cpy(msg_p->file_name, data_p->file_name_p, MAX_MEDIA_FILE_NAME*2);
   msg_p->play_style = data_p->play_style;
   msg_p->volume = data_p->volume;
   msg_p->output_path = data_p->output_path;
   msg_p->identifier = data_p->identifier;
   msg_p->start_offset = data_p->start_offset;
   msg_p->end_offset = data_p->end_offset;

   aud_send_msg_to_med( src_mod_id, MSG_ID_L4AUD_MEDIA_RESTORE_REQ, msg_p );
}

void aud_send_block_req(module_type src_mod_id, kal_uint16 mod_id, kal_uint8 level)
{
   media_aud_block_req_struct* msg_p = (media_aud_block_req_struct*)
            construct_local_para(sizeof(media_aud_block_req_struct), TD_CTRL);  
                            
   msg_p->mod_id = mod_id;
   msg_p->level = level;

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_AUD_BLOCK_REQ, msg_p );
}

void aud_send_set_headset_mode_output_path_req(module_type src_mod_id, kal_uint8 device)
{
   media_aud_set_headset_mode_output_path_struct* msg_p = (media_aud_set_headset_mode_output_path_struct*)
            construct_local_para(sizeof(media_aud_set_headset_mode_output_path_struct), TD_CTRL);  

   msg_p->device = device;

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_AUD_SET_HEADSET_MODE_OUTPUT_PATH, msg_p );
}

void aud_send_set_equalizer_req( module_type src_mod_id, kal_int8* magnitude )
{
	media_aud_set_equalizer_req_struct* msg_p = (media_aud_set_equalizer_req_struct*)
            construct_local_para(sizeof(media_aud_set_equalizer_req_struct), TD_CTRL);

	memcpy( msg_p->magnitude, magnitude, 8 );

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_AUD_SET_EQUALIZER_REQ, msg_p );
}

void aud_send_set_vibrator_enabled_req( module_type src_mod_id, kal_uint8 enable )
{
	media_aud_set_vibrator_enabled_req_struct* msg_p = (media_aud_set_vibrator_enabled_req_struct*)
            construct_local_para(sizeof(media_aud_set_vibrator_enabled_req_struct), TD_CTRL);
	msg_p->enabled = enable;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_AUD_SET_VIBRATOR_ENABLED_REQ, msg_p );
}

void aud_send_get_file_data_length_req( module_type src_mod_id, void* file_name,kal_int32 PlayProgress )
{
	media_aud_get_file_data_length_req_struct* msg_p = (media_aud_get_file_data_length_req_struct*)
            construct_local_para(sizeof(media_aud_get_file_data_length_req_struct), TD_CTRL);

	msg_p->progress = PlayProgress;
	kal_mem_cpy(msg_p->file_name, file_name, MAX_MEDIA_FILE_NAME*2);

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_AUD_GET_FILE_DATA_LENGTH_REQ, msg_p );
}

void aud_send_get_file_data_progress_req( module_type src_mod_id )
{
	media_aud_get_file_data_progress_req_struct* msg_p = (media_aud_get_file_data_progress_req_struct*)
            construct_local_para(sizeof(media_aud_get_file_data_progress_req_struct), TD_CTRL);

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_AUD_GET_FILE_DATA_PROGRESS_REQ, msg_p );
}

void aud_send_set_file_data_progress_req( module_type src_mod_id, kal_uint32 progress )
{
	media_aud_set_file_data_progress_req_struct* msg_p = (media_aud_set_file_data_progress_req_struct*)
            construct_local_para(sizeof(media_aud_set_file_data_progress_req_struct), TD_CTRL);

	msg_p->progress = progress;
	
	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_AUD_SET_FILE_DATA_PROGRESS_REQ, msg_p );
}

void aud_send_fmr_power_on_req(module_type src_mod_id, kal_uint8 output_path)
{
	media_fmr_power_on_req_struct* msg_p = (media_fmr_power_on_req_struct*)
            construct_local_para(sizeof(media_fmr_power_on_req_struct), TD_CTRL);

	msg_p->output_path = output_path;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_FMR_POWER_ON_REQ, msg_p );
}

void aud_send_fmr_power_off_req(module_type src_mod_id)
{
	media_fmr_power_off_req_struct* msg_p = (media_fmr_power_off_req_struct*)
            construct_local_para(sizeof(media_fmr_power_off_req_struct), TD_CTRL);

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_FMR_POWER_OFF_REQ, msg_p );
}

void aud_send_fmr_set_freq_req(module_type src_mod_id, kal_uint16 freq)
{
	media_fmr_set_freq_req_struct* msg_p = (media_fmr_set_freq_req_struct*)
            construct_local_para(sizeof(media_fmr_set_freq_req_struct), TD_CTRL);

	msg_p->freq = freq;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_FMR_SET_FREQ_REQ, msg_p );
}

void aud_send_fmr_mute_req(module_type src_mod_id, kal_uint8 mute)
{
	media_fmr_mute_req_struct* msg_p = (media_fmr_mute_req_struct*)
            construct_local_para(sizeof(media_fmr_mute_req_struct), TD_CTRL);

	msg_p->mute = mute;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_FMR_MUTE_REQ, msg_p );
}

void aud_send_fmr_check_is_valid_stop_cnf(kal_uint8 is_valid)
{
	media_fmr_check_is_valid_stop_cnf_struct* cnf_p = (media_fmr_check_is_valid_stop_cnf_struct*)
		construct_local_para(sizeof(media_fmr_check_is_valid_stop_cnf_struct), TD_CTRL);

	cnf_p->is_valid = is_valid;                     
	aud_send_msg_from_med( aud_context_p->src_mod, MSG_ID_MEDIA_FMR_CHECK_IS_VALID_STOP_CNF, cnf_p );
} 

void aud_send_fmr_get_signal_level_cnf(kal_uint8 sig_lvl)
{
	media_fmr_get_signal_level_cnf_struct* cnf_p = (media_fmr_get_signal_level_cnf_struct*)
		construct_local_para(sizeof(media_fmr_get_signal_level_cnf_struct), TD_CTRL);

	cnf_p->signal_level = sig_lvl;
	aud_send_msg_from_med( aud_context_p->src_mod, MSG_ID_MEDIA_FMR_GET_SIGNAL_LEVEL_CNF, cnf_p );
}

#ifdef __MED_MMA_MOD__
/*************** MMA  ************************/

/*************************************************************************
* FUNCTION
*	aud_send_mma_open_req
*
* DESCRIPTION
*	This function is to send mma open request. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint8 media_type
*  kal_int32 handler
*  kal_int32 param
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_open_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handler, kal_int32 param)
{
   media_mma_open_req_struct* msg_p = NULL;
   mma_player_struct* param_p=(mma_player_struct*)param;

   //if(stack_int_get_active_module_id()!=src_mod_id)
      //ASSERT(stack_int_get_active_module_id()!=src_mod_id);
      
   msg_p = (media_mma_open_req_struct*)
            construct_local_para(sizeof(media_mma_open_req_struct), TD_CTRL);  
                            
   msg_p->media_type = media_type;
   msg_p->handler = handler;
   msg_p->data = param_p->data;
   msg_p->size = param_p->size;
   msg_p->repeats = param_p->repeats;
   
   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_OPEN_REQ, msg_p );
}

/*************************************************************************
* FUNCTION
*	aud_send_mma_close_req
*
* DESCRIPTION
*	This function is to send mma close request. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint8 media_type
*  kal_int32 handle
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_close_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle)
{
   media_mma_close_req_struct* msg_p = NULL;

   //if(stack_int_get_active_module_id()!=src_mod_id)
      //ASSERT(stack_int_get_active_module_id()!=src_mod_id);

   msg_p = (media_mma_close_req_struct*)
            construct_local_para(sizeof(media_mma_close_req_struct), TD_CTRL);  
                            
   msg_p->media_type = media_type;
   msg_p->handle = handle;

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_CLOSE_REQ, msg_p );
}

/*************************************************************************
* FUNCTION
*	aud_send_mma_play_req
*
* DESCRIPTION
*	This function is to send mma play request. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint8 media_type
*  kal_int32 handle
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_play_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle)
{
   media_mma_play_req_struct* msg_p = NULL;

   //if(stack_int_get_active_module_id()!=src_mod_id)
      //ASSERT(stack_int_get_active_module_id()!=src_mod_id);

   msg_p = (media_mma_play_req_struct*)
            construct_local_para(sizeof(media_mma_play_req_struct), TD_CTRL);  
                            
   msg_p->media_type = media_type;
   msg_p->handle = handle;

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_PLAY_REQ, msg_p );
}

/*************************************************************************
* FUNCTION
*	aud_send_mma_stop_req
*
* DESCRIPTION
*	This function is to send mma stop request. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint8 media_type
*  kal_int32 handle
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_stop_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle)
{
   media_mma_stop_req_struct* msg_p = NULL;

   //if(stack_int_get_active_module_id()!=src_mod_id)
      //ASSERT(stack_int_get_active_module_id()!=src_mod_id);

   msg_p = (media_mma_stop_req_struct*)
            construct_local_para(sizeof(media_mma_stop_req_struct), TD_CTRL);  
                            
   msg_p->media_type = media_type;
   msg_p->handle = handle;

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_STOP_REQ, msg_p );
}

/*************************************************************************
* FUNCTION
*	aud_send_mma_pause_req
*
* DESCRIPTION
*	This function is to send mma pause request. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint8 media_type
*  kal_int32 handle
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_pause_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle)
{
   media_mma_pause_req_struct* msg_p = NULL;

   //if(stack_int_get_active_module_id()!=src_mod_id)
      //ASSERT(stack_int_get_active_module_id()!=src_mod_id);

   msg_p = (media_mma_pause_req_struct*)
            construct_local_para(sizeof(media_mma_pause_req_struct), TD_CTRL);  
                            
   msg_p->media_type = media_type;
   msg_p->handle = handle;

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_PAUSE_REQ, msg_p );
}

/*************************************************************************
* FUNCTION
*	aud_send_mma_resume_req
*
* DESCRIPTION
*	This function is to send mma resume request. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint8 media_type
*  kal_int32 handle
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_resume_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle)
{
   media_mma_resume_req_struct* msg_p = NULL;

   //if(stack_int_get_active_module_id()!=src_mod_id)
      //ASSERT(stack_int_get_active_module_id()!=src_mod_id);

   msg_p = (media_mma_resume_req_struct*)
            construct_local_para(sizeof(media_mma_resume_req_struct), TD_CTRL);  
                            
   msg_p->media_type = media_type;
   msg_p->handle = handle;

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_RESUME_REQ, msg_p );
}

/*************************************************************************
* FUNCTION
*	aud_send_mma_set_start_time_req
*
* DESCRIPTION
*	This function is to send mma set start time request. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint8 media_type
*  kal_int32 handle
*  kal_int32 start_time
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_set_start_time_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle, kal_int32 start_time)
{
   media_mma_set_start_time_req_struct* msg_p = NULL;

   msg_p = (media_mma_set_start_time_req_struct*)
            construct_local_para(sizeof(media_mma_set_start_time_req_struct), TD_CTRL);  
                            
   msg_p->media_type = media_type;
   msg_p->handle = handle;
   msg_p->start_time = start_time;

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_SET_START_TIME_REQ, msg_p );
}

/*************************************************************************
* FUNCTION
*	aud_send_mma_set_stop_time_req
*
* DESCRIPTION
*	This function is to send mma set stop time request. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint8 media_type
*  kal_int32 handle
*  kal_int32 stop_time
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_set_stop_time_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle, kal_int32 stop_time)
{
   media_mma_set_stop_time_req_struct* msg_p = NULL;

   msg_p = (media_mma_set_stop_time_req_struct*)
            construct_local_para(sizeof(media_mma_set_stop_time_req_struct), TD_CTRL);  
                            
   msg_p->media_type = media_type;
   msg_p->handle = handle;
   msg_p->stop_time = stop_time;

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_SET_STOP_TIME_REQ, msg_p );
}


/*************************************************************************
* FUNCTION
*	aud_send_mma_set_rate_req
*
* DESCRIPTION
*	This function is to send mma set rate request. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint8 media_type
*  kal_int32 handle
*  kal_int32 rate
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_set_rate_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle, kal_int32 rate)
{
   media_mma_set_rate_req_struct* msg_p = NULL;

   msg_p = (media_mma_set_rate_req_struct*)
            construct_local_para(sizeof(media_mma_set_rate_req_struct), TD_CTRL);  
                            
   msg_p->media_type = media_type;
   msg_p->handle = handle;
   msg_p->rate = rate;

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_SET_RATE_REQ, msg_p );
}

/*************************************************************************
* FUNCTION
*	aud_send_mma_set_tempo_req
*
* DESCRIPTION
*	This function is to send mma set tempo request. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint8 media_type
*  kal_int32 handle
*  kal_int32 tempo
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_set_tempo_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle, kal_int32 tempo)
{
   media_mma_set_tempo_req_struct* msg_p = NULL;

   msg_p = (media_mma_set_tempo_req_struct*)
            construct_local_para(sizeof(media_mma_set_tempo_req_struct), TD_CTRL);  
                            
   msg_p->media_type = media_type;
   msg_p->handle = handle;
   msg_p->tempo = tempo;

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_SET_TEMPO_REQ, msg_p );
}

/*************************************************************************
* FUNCTION
*	aud_send_mma_set_pitch_req
*
* DESCRIPTION
*	This function is to send mma set pitch request. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint8 media_type
*  kal_int32 handle
*  kal_int32 pitch
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_set_pitch_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle, kal_int32 pitch)
{
   media_mma_set_pitch_req_struct* msg_p = NULL;

   msg_p = (media_mma_set_pitch_req_struct*)
            construct_local_para(sizeof(media_mma_set_pitch_req_struct), TD_CTRL);  
                            
   msg_p->media_type = media_type;
   msg_p->handle = handle;
   msg_p->pitch = pitch;

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_SET_PITCH_REQ, msg_p );
}

/*************************************************************************
* FUNCTION
*	aud_send_mma_set_volume_req
*
* DESCRIPTION
*	This function is to send mma set volume request. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint8 media_type
*  kal_int32 handle
*  kal_int32 volume
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_set_volume_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle, kal_uint8 volume)
{
   media_mma_set_volume_req_struct* msg_p = NULL;

   msg_p = (media_mma_set_volume_req_struct*)
            construct_local_para(sizeof(media_mma_set_volume_req_struct), TD_CTRL);  
                            
   msg_p->media_type = media_type;
   msg_p->handle = handle;
   msg_p->volume = volume;

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_SET_VOLUME_REQ, msg_p );
}

/*************************************************************************
* FUNCTION
*	aud_send_mma_send_long_msg_req
*
* DESCRIPTION
*	This function is to send mma send long msg request. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_int32 handle
*  kal_uint8* buf, kal_uint16 len
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_send_long_msg_req(module_type src_mod_id, kal_int32 handle, kal_uint8* buf, kal_uint16 len)
{
   media_mma_send_long_msg_req_struct* msg_p = NULL;

   msg_p = (media_mma_send_long_msg_req_struct*)
            construct_local_para(sizeof(media_mma_send_long_msg_req_struct), TD_CTRL);  
                            
   msg_p->handle = handle;
   msg_p->buf = buf;
   msg_p->len = len;

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_SEND_LONG_MSG_REQ, msg_p );
}

/*************************************************************************
* FUNCTION
*	aud_send_mma_send_short_msg_req
*
* DESCRIPTION
*	This function is to send mma send short msg request. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_int32 handle
*  kal_uint8 type, kal_uint8 data1, kal_uint8 data2
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_send_short_msg_req(module_type src_mod_id, kal_int32 handle, kal_uint8 type, kal_uint8 data1, kal_uint8 data2)
{
   media_mma_send_short_msg_req_struct* msg_p = NULL;

   msg_p = (media_mma_send_short_msg_req_struct*)
            construct_local_para(sizeof(media_mma_send_short_msg_req_struct), TD_CTRL);  
                            
   msg_p->handle = handle;
   msg_p->type = type;
   msg_p->data1 = data1;
   msg_p->data2 = data2;

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_SEND_SHORT_MSG_REQ, msg_p );
}

/*************************************************************************
* FUNCTION
*	aud_send_mma_open_device_req
*
* DESCRIPTION
*	This function is to send mma open device request. 
*
* PARAMETERS
*	module_type src_mod_id
*  
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_open_device_req(module_type src_mod_id)
{
   media_mma_open_device_req_struct* msg_p = NULL;

   //if(stack_int_get_active_module_id()!=src_mod_id)
      //ASSERT(stack_int_get_active_module_id()!=src_mod_id);

   msg_p = (media_mma_open_device_req_struct*)
            construct_local_para(sizeof(media_mma_open_device_req_struct), TD_CTRL);  

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_OPEN_DEVICE_REQ, msg_p );
}

/*************************************************************************
* FUNCTION
*	aud_send_mma_close_device_req
*
* DESCRIPTION
*	This function is to send mma close device request. 
*
* PARAMETERS
*	module_type src_mod_id
*  
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_close_device_req(module_type src_mod_id, kal_int32 handle)
{
   media_mma_close_device_req_struct* msg_p = NULL;

   //if(stack_int_get_active_module_id()!=src_mod_id)
      //ASSERT(stack_int_get_active_module_id()!=src_mod_id);

   msg_p = (media_mma_close_device_req_struct*)
            construct_local_para(sizeof(media_mma_close_device_req_struct), TD_CTRL);  
                            
   msg_p->handle = handle;

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_CLOSE_DEVICE_REQ, msg_p );
}

/*************************************************************************
* FUNCTION
*	aud_send_mma_set_channel_volume_req
*
* DESCRIPTION
*	This function is to send mma set channel volume request. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint8 media_type
*  kal_int32 handle
*  kal_uint8 channel, kal_uint8 level
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_set_channel_volume_req(module_type src_mod_id, kal_int32 handle, kal_uint8 channel, kal_uint8 level)
{
   media_mma_set_channel_volume_req_struct* msg_p = NULL;

   msg_p = (media_mma_set_channel_volume_req_struct*)
            construct_local_para(sizeof(media_mma_set_channel_volume_req_struct), TD_CTRL);  
                            
   msg_p->handle = handle;
   msg_p->channel = channel;
   msg_p->level = level;

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_SET_CHANNEL_VOLUME_REQ, msg_p );
}

/*************************************************************************
* FUNCTION
*	aud_send_mma_set_program_req
*
* DESCRIPTION
*	This function is to send mma set program request. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint8 media_type
*  kal_int32 handle
*  kal_uint8 channel, kal_uint8 bank, kal_uint8 program
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_set_program_req(module_type src_mod_id, kal_int32 handle, kal_uint8 channel, kal_uint8 bank, kal_uint8 program)
{
   media_mma_set_program_req_struct* msg_p = NULL;

   msg_p = (media_mma_set_program_req_struct*)
            construct_local_para(sizeof(media_mma_set_program_req_struct), TD_CTRL);  
                            
   msg_p->handle = handle;
   msg_p->channel = channel;
   msg_p->bank = bank;
   msg_p->program = program;

   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_SET_PROGRAM_REQ, msg_p );
}

/*************************************************************************
* FUNCTION
*	aud_send_mma_set_data_req
*
* DESCRIPTION
*	This function is to send mma set data request. 
*
* PARAMETERS
*	module_type src_mod_id
*  kal_uint8 media_type
*  kal_int32 handle
*  kal_int32 param
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_send_mma_set_data_req(module_type src_mod_id, kal_uint8 media_type, kal_int32 handle, kal_int32 param)
{
   media_mma_set_data_req_struct* msg_p = NULL;
   mma_player_struct* param_p=(mma_player_struct*)param;

   //if(stack_int_get_active_module_id()!=src_mod_id)
      //ASSERT(stack_int_get_active_module_id()!=src_mod_id);

   msg_p = (media_mma_set_data_req_struct*)
            construct_local_para(sizeof(media_mma_set_data_req_struct), TD_CTRL);  
                            
   msg_p->media_type = media_type;
   msg_p->handle = handle;
   msg_p->data = param_p->data;
   msg_p->size = param_p->size;
   msg_p->repeats = param_p->repeats;
   
   aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_MMA_SET_DATA_REQ, msg_p );
}

#endif /* __MED_MMA_MOD__ */

#ifdef __MED_VR_MOD__
void aud_send_vr_get_version_req( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang )
{
	media_vr_get_version_req_struct* msg_p = (media_vr_get_version_req_struct*)
            construct_local_para(sizeof(media_vr_get_version_req_struct), TD_CTRL);

	msg_p->mode = mode;
	msg_p->language = lang;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VR_GET_VERSION_REQ, msg_p );
}

void aud_send_vr_get_param_req( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_int16* sim, kal_int16* diff, kal_int16* rej )
{
	media_vr_get_param_req_struct* msg_p = (media_vr_get_param_req_struct*)
            construct_local_para(sizeof(media_vr_get_param_req_struct), TD_CTRL);

	msg_p->mode = mode;
	msg_p->language = lang;
	msg_p->sim = sim;
	msg_p->diff = diff;
	msg_p->rej = rej;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VR_GET_PARAM_REQ, msg_p );
}

void aud_send_vr_set_param_req( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_int16 sim, kal_int16 diff, kal_int16 rej )
{
	media_vr_set_param_req_struct* msg_p = (media_vr_set_param_req_struct*)
            construct_local_para(sizeof(media_vr_set_param_req_struct), TD_CTRL);

	msg_p->mode = mode;
	msg_p->language = lang;
	msg_p->sim = sim;
	msg_p->diff = diff;
	msg_p->rej = rej;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VR_SET_PARAM_REQ, msg_p );
}

void aud_send_vr_init_rcg_req( module_type src_mod_id, kal_uint32 session_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id )
{
	media_vr_init_rcg_req_struct* msg_p = (media_vr_init_rcg_req_struct*)
            construct_local_para(sizeof(media_vr_init_rcg_req_struct), TD_CTRL);

	msg_p->session_id = session_id;
	msg_p->mode = mode;
	msg_p->language = lang;
	msg_p->group_id = group_id;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VR_INIT_RCG_REQ, msg_p );
}

void aud_send_vr_init_trn_req( module_type src_mod_id, kal_uint32 session_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 word_id )
{
	media_vr_init_trn_req_struct* msg_p = (media_vr_init_trn_req_struct*)
            construct_local_para(sizeof(media_vr_init_trn_req_struct), TD_CTRL);

	msg_p->session_id = session_id;
	msg_p->mode = mode;
	msg_p->language = lang;
	msg_p->group_id = group_id;
	msg_p->word_id = word_id;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VR_INIT_TRN_REQ, msg_p );
}

void aud_send_vr_voice_in_req( module_type src_mod_id, kal_uint32 session_id, kal_uint8 seq_no )
{
	media_vr_voice_in_req_struct* msg_p = (media_vr_voice_in_req_struct*)
            construct_local_para(sizeof(media_vr_voice_in_req_struct), TD_CTRL);

	msg_p->session_id = session_id;
	msg_p->seq_no = seq_no;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VR_VOICE_IN_REQ, msg_p );
}

void aud_send_vr_rcg_result_ind( module_type dst_mod_id, kal_uint32 session_id, kal_int32 result, kal_uint16 id_length, kal_uint16* id_array )
{
	media_vr_rcg_result_ind_struct* msg_p = (media_vr_rcg_result_ind_struct*)
            construct_local_para(sizeof(media_vr_rcg_result_ind_struct), TD_CTRL);

	msg_p->session_id = session_id;
	msg_p->result = result;
	msg_p->id_length = id_length;
	kal_mem_cpy( msg_p->id_array, id_array, VR_MAX_RCG_RESULT_NUM*2 );

	aud_send_msg_from_med( dst_mod_id, (kal_uint32)MSG_ID_MEDIA_VR_RCG_RESULT_IND, msg_p );
}

void aud_send_vr_trn_result_ind( module_type dst_mod_id, kal_uint32 session_id, kal_int32 result )
{
	media_vr_rcg_result_ind_struct* msg_p = (media_vr_rcg_result_ind_struct*)
            construct_local_para(sizeof(media_vr_rcg_result_ind_struct), TD_CTRL);

	msg_p->session_id = session_id;
	msg_p->result = result;

	aud_send_msg_from_med( dst_mod_id, (kal_uint32)MSG_ID_MEDIA_VR_TRN_RESULT_IND, msg_p );
}

void aud_send_vr_del_tag_req( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 word_id )
{
	media_vr_del_tag_req_struct* msg_p = (media_vr_del_tag_req_struct*)
            construct_local_para(sizeof(media_vr_del_tag_req_struct), TD_CTRL);

	msg_p->mode = mode;
	msg_p->language = lang;
	msg_p->group_id = group_id;
	msg_p->word_id = word_id;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VR_DEL_TAG_REQ, msg_p );
}

void aud_send_vr_check_tag_req( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 word_id )
{
	media_vr_check_tag_req_struct* msg_p = (media_vr_check_tag_req_struct*)
            construct_local_para(sizeof(media_vr_check_tag_req_struct), TD_CTRL);

	msg_p->mode = mode;
	msg_p->language = lang;
	msg_p->group_id = group_id;
	msg_p->word_id = word_id;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VR_CHECK_TAG_REQ, msg_p );
}

void aud_send_vr_sync_db_req( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16* id_length_p, kal_uint16* id_array )
{
	media_vr_sync_db_req_struct* msg_p = (media_vr_sync_db_req_struct*)
            construct_local_para(sizeof(media_vr_sync_db_req_struct), TD_CTRL);

	msg_p->mode = mode;
	msg_p->language = lang;
	msg_p->group_id = group_id;
	msg_p->id_length_p = id_length_p;
	msg_p->id_array = id_array;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VR_SYNC_DB_REQ, msg_p );
}

void aud_send_vr_play_tag_req( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 word_id, kal_uint16 identifier )
{
	media_vr_play_tag_req_struct* msg_p = (media_vr_play_tag_req_struct*)
            construct_local_para(sizeof(media_vr_play_tag_req_struct), TD_CTRL);

	msg_p->mode = mode;
	msg_p->language = lang;
	msg_p->group_id = group_id;
	msg_p->word_id = word_id;
	msg_p->identifier = identifier;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VR_PLAY_TAG_REQ, msg_p );
}

void aud_send_vr_get_dir_req( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16* folder_name )
{
	media_vr_get_dir_req_struct* msg_p = (media_vr_get_dir_req_struct*)
            construct_local_para(sizeof(media_vr_get_dir_req_struct), TD_CTRL);

	msg_p->mode = mode;
	msg_p->language = lang;
	msg_p->group_id = group_id;
	msg_p->folder_name = folder_name;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VR_GET_DIR_REQ, msg_p );
}

void aud_send_vr_add_tag_req( module_type src_mod_id, kal_uint8 mode, kal_uint8 lang, kal_uint8 group_id, kal_uint16 id_length, kal_uint16* id_array )
{
	media_vr_add_tag_req_struct* msg_p = (media_vr_add_tag_req_struct*)
            construct_local_para(sizeof(media_vr_add_tag_req_struct), TD_CTRL);

	msg_p->mode = mode;
	msg_p->language = lang;
	msg_p->group_id = group_id;
	msg_p->id_length = id_length;
	kal_mem_cpy( msg_p->id_array, id_array, VR_MAX_ADD_ONCE_NUM*2 );

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VR_ADD_TAG_REQ, msg_p );
}

void aud_send_vr_abort_req( module_type src_mod_id )
{
	media_vr_abort_req_struct* msg_p = (media_vr_abort_req_struct*)
            construct_local_para(sizeof(media_vr_abort_req_struct), TD_CTRL);

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VR_ABORT_REQ, msg_p );
}

void aud_send_vr_abort_ind( module_type dst_mod_id, kal_uint32 session_id )
{
	media_vr_abort_ind_struct* msg_p = (media_vr_abort_ind_struct*)
            construct_local_para(sizeof(media_vr_abort_ind_struct), TD_CTRL);

	msg_p->session_id = session_id;

	aud_send_msg_from_med( dst_mod_id, (kal_uint32)MSG_ID_MEDIA_VR_ABORT_IND, msg_p );
}
#endif // __MED_VR_MOD__

#ifdef __MMI_SUPPORT_BACKLIGHT_SYNC__
void aud_send_backlight_ctrl_req(module_type dst_mod_id, kal_uint8 on_off, kal_uint8 disable_timer)
{
	mmiapi_backlight_control_req_struct* msg_p = (mmiapi_backlight_control_req_struct*)
            construct_local_para(sizeof(mmiapi_backlight_control_req_struct), TD_CTRL);
	module_type src_mod_id;
	
	msg_p->on_off = on_off;
	msg_p->disable_timer = disable_timer;
	src_mod_id = stack_int_get_active_module_id();

	aud_send_msg( src_mod_id, dst_mod_id, MSG_ID_MMIAPI_BACKLIGHT_CONTROL_REQ, msg_p );
}
#endif

#ifdef __MMI_SUPPORT_LED_SYNC__
void aud_send_led_ctrl_req(module_type dst_mod_id, kal_uint8 on_off)
{
	mmiapi_led_control_req_struct* msg_p = (mmiapi_led_control_req_struct*)
            construct_local_para(sizeof(mmiapi_led_control_req_struct), TD_CTRL);
	module_type src_mod_id;
	
	msg_p->on_off = on_off;
	src_mod_id = stack_int_get_active_module_id();
	
	aud_send_msg( src_mod_id, dst_mod_id, MSG_ID_MMIAPI_LED_CONTROL_REQ, msg_p );
}
#endif

#ifdef __MED_VRSI_MOD__

void aud_send_vrsi_init_req(module_type src_mod_id)
{   
	media_vrsi_init_req_struct* msg_p = (media_vrsi_init_req_struct*)
            construct_local_para(sizeof(media_vrsi_init_req_struct), TD_CTRL);

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VRSI_INIT_REQ, msg_p );
}

void aud_send_vrsi_add_tags_req(module_type src_mod_id, kal_uint32 session_id, kal_uint8 *grammar, kal_uint16 tag_num, kal_uint16** name_list, kal_int32 *long_list, kal_uint8 *is_name_list, kal_uint16 *id_list)
{
	media_vrsi_add_tags_req_struct* msg_p = (media_vrsi_add_tags_req_struct*)
            construct_local_para(sizeof(media_vrsi_add_tags_req_struct), TD_CTRL);

	msg_p->session_id = session_id;
	msg_p->grammar = grammar;
	msg_p->tag_num = tag_num;
	msg_p->name_list = name_list;
	msg_p->long_list = long_list;
	msg_p->is_name_list = is_name_list;
	msg_p->id_list = id_list;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VRSI_ADD_TAGS_REQ, msg_p );
} 

void aud_send_vrsi_start_cmd_trn_req(module_type src_mod_id, kal_uint32 session_id, kal_uint8 *grammar, kal_uint16 *tag_name, kal_int32 tag_long)
{
	media_vrsi_start_trn_req_struct* msg_p = (media_vrsi_start_trn_req_struct*)
            construct_local_para(sizeof(media_vrsi_start_trn_req_struct), TD_CTRL);

	msg_p->session_id = session_id;
	msg_p->grammar = grammar;
	msg_p->tag_name = tag_name;
	msg_p->tag_long = tag_long;
	msg_p->lang = NULL;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VRSI_START_TRN_REQ, msg_p );
}  

void aud_send_vrsi_start_cmd_rcg_req(module_type src_mod_id, kal_uint32 session_id, kal_uint8 *grammar)
{
	media_vrsi_start_rcg_req_struct* msg_p = (media_vrsi_start_rcg_req_struct*)
            construct_local_para(sizeof(media_vrsi_start_rcg_req_struct), TD_CTRL);

	msg_p->session_id = session_id;
	msg_p->grammar = grammar;	
	msg_p->lang = NULL;
	msg_p->limit = NULL;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VRSI_START_RCG_REQ, msg_p );
}  

void aud_send_vrsi_start_digit_rcg_req(module_type src_mod_id, kal_uint32 session_id, kal_uint8 lang, kal_uint16 limit)
{
	media_vrsi_start_rcg_req_struct* msg_p = (media_vrsi_start_rcg_req_struct*)
            construct_local_para(sizeof(media_vrsi_start_rcg_req_struct), TD_CTRL);

	msg_p->session_id = session_id;
	msg_p->grammar = NULL;	
	msg_p->lang = lang;
	msg_p->limit = limit;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VRSI_START_RCG_REQ, msg_p );
} 
   
void aud_send_vrsi_start_digit_adapt_req(module_type src_mod_id, kal_uint32 session_id, kal_uint8 lang)
{
	media_vrsi_start_trn_req_struct* msg_p = (media_vrsi_start_trn_req_struct*)
            construct_local_para(sizeof(media_vrsi_start_trn_req_struct), TD_CTRL);

	msg_p->session_id = session_id;
	msg_p->grammar = NULL;
	msg_p->tag_name = NULL;
	msg_p->tag_long = NULL;
	msg_p->lang = lang;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VRSI_START_TRN_REQ, msg_p );
}

void aud_send_vrsi_play_tag_req(module_type src_mod_id, kal_uint32 session_id, kal_uint8 *grammar, kal_uint16 tag_id ,kal_uint8 volume, kal_uint8 output_path)
{
	media_vrsi_play_tag_req_struct* msg_p = (media_vrsi_play_tag_req_struct*)
            construct_local_para(sizeof(media_vrsi_play_tag_req_struct), TD_CTRL);

	msg_p->session_id = session_id;
	msg_p->grammar = grammar;
	msg_p->tag_id = tag_id;	
	msg_p->volume = volume;
	msg_p->output_path = output_path;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VRSI_PLAY_TAG_REQ, msg_p );
}

void aud_send_vrsi_play_TTS_req(module_type src_mod_id, kal_uint32 session_id, kal_uint16 *text, kal_uint8 volume, kal_uint8 output_path,  kal_uint8 lang)
{
	media_vrsi_play_tts_req_struct* msg_p = (media_vrsi_play_tts_req_struct*)
            construct_local_para(sizeof(media_vrsi_play_tts_req_struct), TD_CTRL);

	msg_p->session_id = session_id;
	msg_p->text = text;	
	msg_p->volume = volume;
	msg_p->output_path = output_path;
	msg_p->lang = lang;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VRSI_PLAY_TTS_REQ, msg_p );
}

  
void aud_send_vrsi_get_tag_num_req(module_type src_mod_id, kal_uint8 *grammar, kal_int32 *tag_num)
{
	media_vrsi_get_tagnum_req_struct* msg_p = (media_vrsi_get_tagnum_req_struct*)
            construct_local_para(sizeof(media_vrsi_get_tagnum_req_struct), TD_CTRL);

	msg_p->grammar = grammar;
	msg_p->tag_num = tag_num;	

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VRSI_GET_TAGNUM_REQ, msg_p );
}

  
void aud_send_vrsi_get_tag_info_req(module_type src_mod_id, kal_uint8 *grammar, kal_int32 tag_num, kal_uint16 *id_list, kal_uint16 **name_list, kal_int32 *long_list)
{
	media_vrsi_get_taginfo_req_struct* msg_p = (media_vrsi_get_taginfo_req_struct*)
            construct_local_para(sizeof(media_vrsi_get_taginfo_req_struct), TD_CTRL);

	msg_p->grammar = grammar;
	msg_p->tag_num = tag_num;	
	msg_p->id_list = id_list;
	msg_p->name_list = name_list;	
	msg_p->long_list = long_list;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VRSI_GET_TAGINFO_REQ, msg_p );
}

  
void aud_send_vrsi_del_tags_req(module_type src_mod_id, kal_uint8 *grammar, kal_int32 tag_num, kal_uint16 *id_list)
{
	media_vrsi_del_tags_req_struct* msg_p = (media_vrsi_del_tags_req_struct*)
            construct_local_para(sizeof(media_vrsi_del_tags_req_struct), TD_CTRL);

	msg_p->grammar = grammar;
	msg_p->tag_num = tag_num;	
	msg_p->id_list = id_list;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VRSI_DEL_TAGS_REQ, msg_p );
}

  
void aud_send_vrsi_reset_tags_req(module_type src_mod_id, kal_uint8 *grammar)
{
	media_vrsi_reset_tags_req_struct* msg_p = (media_vrsi_reset_tags_req_struct*)
            construct_local_para(sizeof(media_vrsi_reset_tags_req_struct), TD_CTRL);

	msg_p->grammar = grammar;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VRSI_RESET_TAGS_REQ, msg_p );
}

  
void aud_send_vrsi_sync_db_req(module_type src_mod_id, kal_uint8 *grammar, kal_int32 tag_num, kal_uint16 *id_list, kal_uint16 **name_list, kal_uint8 *sync_flag)
{
	media_vrsi_sync_db_req_struct* msg_p = (media_vrsi_sync_db_req_struct*)
            construct_local_para(sizeof(media_vrsi_sync_db_req_struct), TD_CTRL);

	msg_p->grammar = grammar;
	msg_p->tag_num = tag_num;
	msg_p->id_list = id_list;
	msg_p->name_list = name_list;
	msg_p->sync_flag = sync_flag;		

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VRSI_SYNC_DB_REQ, msg_p );
}

void aud_send_vrsi_abort_req(module_type src_mod_id, kal_uint32 session_id)
{
	media_vrsi_abort_req_struct* msg_p = (media_vrsi_abort_req_struct*)
            construct_local_para(sizeof(media_vrsi_abort_req_struct), TD_CTRL);

	msg_p->session_id = session_id;

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VRSI_ABORT_REQ, msg_p );
}
 
void aud_send_vrsi_close_req(module_type src_mod_id)
{
	media_vrsi_close_req_struct* msg_p = (media_vrsi_close_req_struct*)
            construct_local_para(sizeof(media_vrsi_close_req_struct), TD_CTRL);

	aud_send_msg_to_med( src_mod_id, MSG_ID_MEDIA_VRSI_CLOSE_REQ, msg_p );
}


// MED -> MED 
void aud_send_vrsi_process_ind(module_type dst_mod_id, kal_uint32 session_id, kal_uint8 proc_type)
{
   ilm_struct *ilm_ptr;
	media_vrsi_process_ind_struct* msg_p = (media_vrsi_process_ind_struct*)
            construct_local_para(sizeof(media_vrsi_process_ind_struct), TD_CTRL);

	msg_p->session_id = session_id;
	msg_p->proc_type = proc_type; 
	
	ilm_ptr = allocate_ilm(MOD_L1SP);
	ilm_ptr->src_mod_id = MOD_L1SP;
	ilm_ptr->dest_mod_id = dst_mod_id;
	ilm_ptr->sap_id = MED_SAP;
	ilm_ptr->msg_id = (msg_type_t) MSG_ID_MEDIA_VRSI_PROCESS_IND;
	ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
	ilm_ptr->peer_buff_ptr = NULL;
	msg_send_ext_queue(ilm_ptr);   
}

// MED -> MMI 
void aud_send_vrsi_trn_prompt_req(module_type dst_mod_id, kal_uint32 session_id, kal_uint8 pmp_mode)
{
	media_vrsi_trn_prompt_req_struct* msg_p = (media_vrsi_trn_prompt_req_struct*)
            construct_local_para(sizeof(media_vrsi_trn_prompt_req_struct), TD_CTRL);

	msg_p->session_id = session_id;
	msg_p->pmp_mode = pmp_mode;	

	aud_send_msg_from_med( dst_mod_id, (kal_uint32)MSG_ID_MEDIA_VRSI_TRN_PROMPT_REQ, msg_p );
}

void aud_send_vrsi_trn_result_ind(module_type dst_mod_id, kal_uint32 session_id, kal_uint16 tag_id)
{
	media_vrsi_trn_result_ind_struct* msg_p = (media_vrsi_trn_result_ind_struct*)
            construct_local_para(sizeof(media_vrsi_trn_result_ind_struct), TD_CTRL);

	msg_p->session_id = session_id;
	msg_p->tag_id = tag_id;

	aud_send_msg_from_med( dst_mod_id, (kal_uint32)MSG_ID_MEDIA_VRSI_TRN_RESULT_IND, msg_p );
}

void aud_send_vrsi_rcg_prompt_req(module_type dst_mod_id, kal_uint32 session_id, kal_uint8 pmp_mode)
{
	media_vrsi_rcg_prompt_req_struct* msg_p = (media_vrsi_rcg_prompt_req_struct*)
            construct_local_para(sizeof(media_vrsi_rcg_prompt_req_struct), TD_CTRL);

	msg_p->session_id = session_id;
	msg_p->pmp_mode = pmp_mode;	

	aud_send_msg_from_med( dst_mod_id, (kal_uint32)MSG_ID_MEDIA_VRSI_RCG_PROMPT_REQ, msg_p );
}

void aud_send_vrsi_rcg_result_ind(module_type dst_mod_id, kal_uint32 session_id, kal_uint16 res_type)
{
	media_vrsi_rcg_result_ind_struct* msg_p = (media_vrsi_rcg_result_ind_struct*)
            construct_local_para(sizeof(media_vrsi_rcg_result_ind_struct), TD_CTRL);

	msg_p->session_id = session_id;
	msg_p->res_type = res_type;	

	aud_send_msg_from_med( dst_mod_id, (kal_uint32)MSG_ID_MEDIA_VRSI_RCG_RESULT_IND, msg_p );
}
 
void aud_send_vrsi_err_ind(module_type dst_mod_id, kal_uint32 session_id)
{
	media_vrsi_err_ind_struct* msg_p = (media_vrsi_err_ind_struct*)
            construct_local_para(sizeof(media_vrsi_err_ind_struct), TD_CTRL);

	msg_p->session_id = session_id;	

	aud_send_msg_from_med( dst_mod_id, (kal_uint32)MSG_ID_MEDIA_VRSI_ERR_IND, msg_p );
}

void aud_send_vrsi_play_tag_finish_ind(module_type dst_mod_id, kal_uint32 session_id)
{
	media_vrsi_play_tag_finish_ind_struct* msg_p = (media_vrsi_play_tag_finish_ind_struct*)
            construct_local_para(sizeof(media_vrsi_play_tag_finish_ind_struct), TD_CTRL);

	msg_p->session_id = session_id;

	aud_send_msg_from_med( dst_mod_id, (kal_uint32)MSG_ID_MEDIA_VRSI_PLAY_TAG_FINISH_IND, msg_p );
}

void aud_send_vrsi_play_tts_finish_ind(module_type dst_mod_id, kal_uint32 session_id)
{
	media_vrsi_play_tts_finish_ind_struct* msg_p = (media_vrsi_play_tts_finish_ind_struct*)
            construct_local_para(sizeof(media_vrsi_play_tts_finish_ind_struct), TD_CTRL);

	msg_p->session_id = session_id;	

	aud_send_msg_from_med( dst_mod_id, (kal_uint32)MSG_ID_MEDIA_VRSI_PLAY_TTS_FINISH_IND, msg_p );
}

#endif  // __MED_VRSI_MOD__


#endif

#ifdef __MED_GENERAL_TTS__
/*****************************************************************************
 * FUNCTION
 *  aud_send_tts_set_attr_req
 * DESCRIPTION
 *  
 * PARAMETERS
 *  src_mod_id      [IN]        
 *
 * RETURNS
 *  void
 *****************************************************************************/
void aud_send_tts_set_attr_req(module_type src_mod_id, kal_uint32 attr_id, kal_uint32 attr_value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_aud_tts_set_attr_req_struct *msg_p = (media_aud_tts_set_attr_req_struct*)
        construct_local_para(sizeof(media_aud_tts_set_attr_req_struct), TD_CTRL);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    msg_p->attr_id = attr_id;
    msg_p->attr_value = attr_value;

    aud_send_msg_to_med(src_mod_id, (kal_uint16) MSG_ID_MEDIA_AUD_TTS_SET_ATTR_REQ, msg_p);
}


/*****************************************************************************
 * FUNCTION
 *  aud_send_play_file_req
 * DESCRIPTION
 *  
 * PARAMETERS
 *  src_mod_id      [IN]        
 *  file_param      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void aud_send_tts_play_req(module_type src_mod_id, void *tts_param)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    aud_tts_struct *data_p = (aud_tts_struct*) tts_param;
    media_aud_tts_play_req_struct *msg_p = (media_aud_tts_play_req_struct*)
        construct_local_para(sizeof(media_aud_tts_play_req_struct), TD_CTRL);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    msg_p->tts_string = data_p->text_string;
    msg_p->str_type = data_p->str_type;
    msg_p->lang = data_p->lang;
    msg_p->app_type = data_p->app_type;
    msg_p->gander = data_p->gander;
    msg_p->identifier = data_p->identifier;
    msg_p->volume = data_p->volume;
    msg_p->output_path = data_p->path;
    msg_p->pitch = data_p->pitch;
    msg_p->speed = data_p->speed;

    aud_send_msg_to_med(src_mod_id, (kal_uint16) MSG_ID_MEDIA_AUD_TTS_PLAY_REQ, msg_p);
}


/*****************************************************************************
 * FUNCTION
 *  aud_send_tts_update_info_ind
 * DESCRIPTION
 *  send message to MMI to invoke tts update infomation
 * PARAMETERS
 *       
 * RETURNS
 *  void
 *****************************************************************************/
void aud_send_tts_update_info_ind(void *data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ilm_struct *ilm_ptr = allocate_ilm(MOD_MED);
    media_aud_tts_update_info_ind_struct *ind_p;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    ind_p = (media_aud_tts_update_info_ind_struct*)
      construct_local_para(sizeof(media_aud_tts_update_info_ind_struct), TD_CTRL);

    kal_mem_cpy(ind_p->data, data, AUD_TTS_MAX_PROCESS_DATA_SIZE);
    
    ilm_ptr->src_mod_id = MOD_MED;
    ilm_ptr->dest_mod_id = MOD_MMI;
    ilm_ptr->sap_id = MED_SAP;
    ilm_ptr->msg_id = MSG_ID_MEDIA_AUD_TTS_UPDATE_INFO_IND;
    ilm_ptr->local_para_ptr = (local_para_struct*) ind_p;
    ilm_ptr->peer_buff_ptr = (peer_buff_struct*) NULL;

    msg_send_ext_queue(ilm_ptr);
}
#endif /*#ifdef __MED_GENERAL_TTS__*/
#ifdef __MED_BT_MOD__
/* open */


/*****************************************************************************
 * FUNCTION
 *  aud_send_bt_audio_open_req
 * DESCRIPTION
 *  
 * PARAMETERS
 *  src_mod_id      [IN]        
 *  profile         [IN]        
 *  mode            [IN]        
 *  connect_id      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void aud_send_bt_audio_open_req(module_type src_mod_id, kal_uint8 profile, kal_uint8 mode, kal_uint16 connect_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_audio_open_req_struct *msg_p = (media_bt_audio_open_req_struct*)
        construct_local_para(sizeof(media_bt_audio_open_req_struct), TD_CTRL);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    msg_p->profile = profile;
    msg_p->mode = mode;
    msg_p->connect_id = connect_id;
//    mmi_trace(1," Send msg MSG_ID_MEDIA_BT_AUDIO_OPEN_REQ to media!! mode = %d  ",mode);
    aud_send_msg_to_med(src_mod_id, (kal_uint32) MSG_ID_MEDIA_BT_AUDIO_OPEN_REQ, msg_p);
}


/*****************************************************************************
 * FUNCTION
 *  aud_send_bt_audio_open_cnf
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dst_mod_id      [IN]        
 *  profile         [IN]        
 *  result          [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void aud_send_bt_audio_open_cnf(module_type dst_mod_id, kal_uint8 profile, kal_uint16 result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_audio_open_cnf_struct *msg_p = (media_bt_audio_open_cnf_struct*)
        construct_local_para(sizeof(media_bt_audio_open_cnf_struct), TD_CTRL);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    msg_p->profile = profile;
    msg_p->result = result;

    aud_send_msg_from_med(dst_mod_id, (kal_uint32) MSG_ID_MEDIA_BT_AUDIO_OPEN_CNF, msg_p);
}


/*****************************************************************************
 * FUNCTION
 *  aud_send_bt_audio_open_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dst_mod_id      [IN]        
 *  profile         [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void aud_send_bt_audio_open_ind(module_type dst_mod_id, kal_uint8 profile)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_audio_open_ind_struct *msg_p = (media_bt_audio_open_ind_struct*)
        construct_local_para(sizeof(media_bt_audio_open_ind_struct), TD_CTRL);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    msg_p->profile = profile;

    aud_send_msg_from_med(dst_mod_id, (kal_uint32) MSG_ID_MEDIA_BT_AUDIO_OPEN_IND, msg_p);
}

/* close */


/*****************************************************************************
 * FUNCTION
 *  aud_send_bt_audio_close_req
 * DESCRIPTION
 *  
 * PARAMETERS
 *  src_mod_id      [IN]        
 *  profile         [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void aud_send_bt_audio_close_req(module_type src_mod_id, kal_uint8 profile)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_audio_close_req_struct *msg_p = (media_bt_audio_close_req_struct*)
        construct_local_para(sizeof(media_bt_audio_close_req_struct), TD_CTRL);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    msg_p->profile = profile;

    aud_send_msg_to_med(src_mod_id, (kal_uint32) MSG_ID_MEDIA_BT_AUDIO_CLOSE_REQ, msg_p);
}


/*****************************************************************************
 * FUNCTION
 *  aud_send_bt_audio_close_cnf
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dst_mod_id      [IN]        
 *  profile         [IN]        
 *  result          [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void aud_send_bt_audio_close_cnf(module_type dst_mod_id, kal_uint8 profile, kal_uint16 result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_audio_close_cnf_struct *msg_p = (media_bt_audio_close_cnf_struct*)
        construct_local_para(sizeof(media_bt_audio_close_cnf_struct), TD_CTRL);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    msg_p->profile = profile;
    msg_p->result = result;

    aud_send_msg_from_med(dst_mod_id, (kal_uint32) MSG_ID_MEDIA_BT_AUDIO_CLOSE_CNF, msg_p);
}

/* close */


/*****************************************************************************
 * FUNCTION
 *  aud_send_bt_audio_close_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dst_mod_id      [IN]        
 *  profile         [IN]        
 *  cause           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void aud_send_bt_audio_close_ind(module_type dst_mod_id, kal_uint8 profile, kal_uint16 cause)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_audio_close_ind_struct *msg_p = (media_bt_audio_close_ind_struct*)
        construct_local_para(sizeof(media_bt_audio_close_ind_struct), TD_CTRL);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    msg_p->profile = profile;
    msg_p->cause = cause;

    aud_send_msg_from_med(dst_mod_id, (kal_uint32) MSG_ID_MEDIA_BT_AUDIO_CLOSE_IND, msg_p);
}


/*****************************************************************************
 * FUNCTION
 *  aud_send_bt_audio_turn_on_req
 * DESCRIPTION
 *  
 * PARAMETERS
 *  src_mod_id      [IN]        
 *  profile         [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void aud_send_bt_audio_turn_on_req(module_type src_mod_id, kal_uint8 profile)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_audio_turn_on_req_struct *msg_p = (media_bt_audio_turn_on_req_struct*)
        construct_local_para(sizeof(media_bt_audio_turn_on_req_struct), TD_CTRL);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    msg_p->profile = profile;

    aud_send_msg_to_med(src_mod_id, (kal_uint32) MSG_ID_MEDIA_BT_AUDIO_TURN_ON_REQ, msg_p);
}


/*****************************************************************************
 * FUNCTION
 *  aud_send_bt_audio_turn_off_req
 * DESCRIPTION
 *  
 * PARAMETERS
 *  src_mod_id      [IN]        
 *  profile         [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void aud_send_bt_audio_turn_off_req(module_type src_mod_id, kal_uint8 profile)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_audio_turn_off_req_struct *msg_p = (media_bt_audio_turn_off_req_struct*)
        construct_local_para(sizeof(media_bt_audio_turn_off_req_struct), TD_CTRL);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    msg_p->profile = profile;

    aud_send_msg_to_med(src_mod_id, (kal_uint32) MSG_ID_MEDIA_BT_AUDIO_TURN_OFF_REQ, msg_p);
}

void aud_send_bt_open_codec_req(module_type src_mod_id, bt_a2dp_audio_cap_struct *cfg, 
    kal_uint8 stream_handle, kal_bool immediate, kal_uint32 seq_no)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_open_codec_req_struct *msg_p = (media_bt_open_codec_req_struct*)
        construct_local_para(sizeof(media_bt_open_codec_req_struct), TD_CTRL);
//mmi_trace(g_sw_BT,"aud_send_bt_open_codec_req is called cfg.codec_type=%d",cfg.codec_type);
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    memcpy(&msg_p->cfg, cfg, sizeof(bt_a2dp_audio_cap_struct));
    msg_p->stream_handle = stream_handle;
    msg_p->immediate = immediate;
    msg_p->seq_no = seq_no;

    aud_send_msg_to_med(src_mod_id, (kal_uint32) MSG_ID_MEDIA_BT_OPEN_CODEC_REQ, msg_p);
}

void aud_send_bt_close_codec_req(module_type src_mod_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    aud_send_msg_to_med(src_mod_id, (kal_uint32) MSG_ID_MEDIA_BT_CLOSE_CODEC_REQ, NULL);
}

void aud_send_bt_close_codec_ind(module_type dst_mod_id, kal_uint8 stream_handle, kal_uint32 seq_no)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_close_codec_ind_struct *msg_p = (media_bt_close_codec_ind_struct*)
        construct_local_para(sizeof(media_bt_close_codec_ind_struct), TD_CTRL);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    msg_p->seq_no = seq_no;
    msg_p->stream_handle = stream_handle;

    aud_send_msg_from_med(dst_mod_id, (kal_uint32) MSG_ID_MEDIA_BT_CLOSE_CODEC_IND, msg_p);
}
#endif /* __MED_BT_MOD__ */ 
