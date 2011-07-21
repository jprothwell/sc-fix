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
 *
 * Filename:
 * ---------
 * custom_ems_context.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is used to define the EMS maximum segment number.
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

#include "kal_release.h"
#include "kal_non_specific_general_types.h"
#include "string.h"

#include "ems.h"
#include "ems_defs.h"

#include "customer_ps_inc.h"
#include "custom_ems_context.h"

const kal_uint8 ems_miss_seg_str[] = EMS_MISS_SEG_DELIMIT_STR ;

#ifndef __SLIM_EMS__
kal_uint32  ems_mem_pool_buff[EMS_MAX_EMS_DATA][(MMI_SMS_MAX_MSG_SEG * EMS_OBJ_BUFF_SIZE_PER_SEG)/4];

#ifdef __EMS_REL5__
#define EMS_PDU_POOL_SIZE  (MMI_SMS_MAX_MSG_SEG * 140 * EMS_RATIO_OF_OBJ_PDU_BUFF)
kal_uint32  ems_mem_pdu_pool_buff[EMS_MAX_EMS_DATA][EMS_PDU_POOL_SIZE/4];
#endif
#endif
kal_uint8  ems_textBuffer[EMS_MAX_EMS_DATA][MMI_SMS_MAX_MSG_SEG *153*2+4]; /* text buffer */
kal_uint8  ems_TPUDLen[EMS_MAX_EMS_DATA][MMI_SMS_MAX_MSG_SEG ]; 
kal_uint8  ems_UDHL[EMS_MAX_EMS_DATA][MMI_SMS_MAX_MSG_SEG];


/*****************************************************************************
* FUNCTION
*  ems_max_seg_num 
* DESCRIPTION
*   This function is used to define the 
*   EMS maximum segment number.
*
* PARAMETERS
*
* RETURNS
*  kal_uint8
* GLOBALS AFFECTED
*   None
*****************************************************************************/
kal_uint8 ems_max_seg_num (void)
{
   return (MMI_SMS_MAX_MSG_SEG);
}


kal_uint16 ems_obj_buff_size_per_seg (void)
{
   return (EMS_OBJ_BUFF_SIZE_PER_SEG);
}


kal_uint8* ems_get_miss_seg_str (kal_uint16 *str_len)
{
   if(str_len != NULL)
      *str_len = strlen((char *)ems_miss_seg_str);

   return ((kal_uint8 *)ems_miss_seg_str);
}


#if ( !defined(__L1_STANDALONE__) && !defined(__MAUI_BASIC__) )
#ifndef __SLIM_EMS__

#ifdef __EMS_REL5__
void *ems_get_pdu_mem_pool_info (kal_uint8 ems_id, kal_uint16 *size)
{
   if (ems_id < EMS_MAX_EMS_DATA)
   {
      if(size!= NULL)
         *size = ((EMS_PDU_POOL_SIZE >> 2) << 2);

      return (void *)ems_mem_pdu_pool_buff[ems_id];
   }
   else
      return NULL;
}
#endif


void *ems_get_mem_pool_ptr (kal_uint8 ems_id)
{
   if (ems_id < EMS_MAX_EMS_DATA)
      return (void *)ems_mem_pool_buff[ems_id];
   else
      return NULL;

}
#endif

kal_uint8 *ems_get_text_buff_ptr (kal_uint8 ems_id)
{
   if (ems_id < EMS_MAX_EMS_DATA)
      return (void *)ems_textBuffer[ems_id];
   else
      return NULL;

}
kal_uint8 *ems_get_TPUDLen_ptr (kal_uint8 ems_id)
{
   if (ems_id < EMS_MAX_EMS_DATA)
      return (void *)ems_TPUDLen[ems_id];
   else
      return NULL;

}

kal_uint8 *ems_get_UDHL_ptr (kal_uint8 ems_id)
{
   if (ems_id < EMS_MAX_EMS_DATA)
      return (void *)ems_UDHL[ems_id];
   else
      return NULL;

}
#endif 


