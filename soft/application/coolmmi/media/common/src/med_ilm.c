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
 *	med_ilm.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes send-ilm related funcions of media task common module.
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


extern HANDLE GetMmiTaskHandle(module_type mod);

void med_send_get_buffer_req(module_type src_mod_id, void** buffer_p, kal_uint32 buffer_size, kal_uint8 location)
{
   media_get_buffer_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;

   msg_p = (media_get_buffer_req_struct*)
            construct_local_para(sizeof(media_get_buffer_req_struct), TD_CTRL);  
                            
   msg_p->location=location;
   msg_p->buffer_p=buffer_p;
   msg_p->buffer_size=buffer_size;

   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_GET_BUFFER_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}


void med_send_free_buffer_req(module_type src_mod_id, void** buffer_p, kal_uint8 location)
{
   media_free_buffer_req_struct* msg_p = NULL;
   ilm_struct       *ilm_ptr = NULL;

   msg_p = (media_free_buffer_req_struct*)
            construct_local_para(sizeof(media_free_buffer_req_struct), TD_CTRL);  
                            
   msg_p->location=location;
   msg_p->buffer_p=buffer_p;

   ilm_ptr = allocate_ilm(src_mod_id);
   ilm_ptr->src_mod_id = src_mod_id;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type_t)MSG_ID_MEDIA_FREE_BUFFER_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);

}


#endif /* MED_NOT_PRESENT */

