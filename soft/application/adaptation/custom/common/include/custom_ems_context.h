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
 * custom_ems_context.h
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

#ifndef _CUSTOM_EMS_CONTEXT_H
#define _CUSTOM_EMS_CONTEXT_H


extern kal_uint8 ems_max_seg_num (void);
extern kal_uint16 ems_obj_buff_size_per_seg (void);
extern kal_uint8* ems_get_miss_seg_str (kal_uint16 *str_len);

#if ( !defined(__L1_STANDALONE__) && !defined(__MAUI_BASIC__) )

extern kal_uint8 *ems_get_text_buff_ptr (kal_uint8 ems_id);
extern kal_uint8 *ems_get_TPUDLen_ptr (kal_uint8 ems_id);
extern kal_uint8 *ems_get_UDHL_ptr (kal_uint8 ems_id);

/* EMS buffer pool                                            */
/* !! VERY IMPORTANT:  user pointer shall 4-byte alignment !! */
#ifndef __SLIM_EMS__
#ifdef __EMS_REL5__
extern void *ems_get_pdu_mem_pool_info (kal_uint8 ems_id, kal_uint16 *size);
#endif
extern void *ems_get_mem_pool_ptr (kal_uint8 ems_id);
extern kal_uint32  ems_mem_pool_buff[EMS_MAX_EMS_DATA][(MMI_SMS_MAX_MSG_SEG * EMS_OBJ_BUFF_SIZE_PER_SEG)/4];
#endif

extern kal_uint8  ems_textBuffer[EMS_MAX_EMS_DATA][MMI_SMS_MAX_MSG_SEG *153*2+4]; /* text buffer */
extern kal_uint8  ems_TPUDLen[EMS_MAX_EMS_DATA][MMI_SMS_MAX_MSG_SEG ]; 
extern kal_uint8  ems_UDHL[EMS_MAX_EMS_DATA][MMI_SMS_MAX_MSG_SEG];
#endif

#endif /* _CUSTOM_EMS_CONTEXT_H */

