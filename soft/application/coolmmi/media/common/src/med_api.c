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
 *   med_api.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes primary exported functions of media task.
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
#include "med_main.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
//#include "tvo_main.h" /* wufasong delete 2007.06.13 */
#include "media_others.h"

extern void med_send_get_buffer_req(module_type src_mod_id, void** buffer_p, kal_uint32 buffer_size, kal_uint8 location);
extern void med_send_free_buffer_req(module_type src_mod_id, void** buffer_p, kal_uint8 location);

static kal_int32 med_result;

kal_int32 media_get_int_buffer(module_type src_mod_id, void** buffer_p, kal_uint32 buffer_size)
{
   med_result=MED_RES_OK;
   med_send_get_buffer_req(src_mod_id, buffer_p, buffer_size, 0);
   MED_WAIT_EVENT(MED_EVT_GET_BUFFER);
   return med_result;
}
kal_int32 media_free_int_buffer(module_type src_mod_id, void** buffer_p)
{
   med_result=MED_RES_OK;
   med_send_free_buffer_req(src_mod_id, buffer_p, 0);
   MED_WAIT_EVENT(MED_EVT_FREE_BUFFER);
   return med_result;
}


kal_int32 media_get_ext_buffer(module_type src_mod_id, void** buffer_p, kal_uint32 buffer_size)
{
   med_result=MED_RES_OK;
   med_send_get_buffer_req(src_mod_id, buffer_p, buffer_size, 1);
   MED_WAIT_EVENT(MED_EVT_GET_BUFFER);
   return med_result;
}
kal_int32 media_free_ext_buffer(module_type src_mod_id, void** buffer_p)
{
   med_result=MED_RES_OK;
   med_send_free_buffer_req(src_mod_id, buffer_p, 1);
   MED_WAIT_EVENT(MED_EVT_FREE_BUFFER);
   return med_result;
}



#endif /* MED_NOT_PRESENT */



