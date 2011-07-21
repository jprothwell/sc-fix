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
 *	med_utility.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is the header file of utiltiy of media task.
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

#ifndef _MED_UTILITY_H
#define _MED_UTILITY_H
#include "timerevents.h"

//#define MED_BASE_TIMER_ID 0x0000

typedef enum{
	MED_BASE_TIMER_ID=0,   //multimedia

   AUD_TIMER_CRESCENDO = MED_BASE_TIMER_ID,
   CAM_POWER_DOWN_DELAY_TIMER ,
   CAM_POWER_UP_TIMER ,
   CAM_READY_POLLING_TIMER,
   IMAGE_CODEC_RETRY_TIMER ,
   VID_FRAME_TIMER,
   RECORDER_GET_DATA,
   AMR_PLAY_TIMER,
   CAM_MD_TIMER,
   MAX_NUM_OF_MED_TIMER,
   MED_END_TIMER_ID  = MAX_NUM_OF_MED_TIMER,



}med_timer_enum;

/* aud timer periods */
#define AUD_TIMER_PERIOD 2000

/*==== Functions =====*/

kal_bool med_utility_init(void);
kal_int32 med_util_utf8_to_ucs2( kal_uint8 *dest, kal_uint32 dest_size, kal_uint8* src, kal_uint32 src_size );
kal_int32 med_util_ucs2_to_utf8( kal_uint8 *dest, kal_uint32 dest_size, kal_uint8* src, kal_uint32 src_size );
void med_get_buffer_req_hdlr(ilm_struct *ilm_ptr);
void med_free_buffer_req_hdlr(ilm_struct *ilm_ptr);
void med_start_base_timer(void *base_timer_ptr, kal_uint32 time_out) ;
void med_stop_base_timer(void *base_timer_ptr);
void med_start_timer(kal_uint8 timer_id, kal_uint32 period, kal_timer_func_ptr timer_expiry, void *arg);
void med_stop_timer(kal_uint8 timer_id);
void* med_alloc_int_mem(kal_int32 size);
void med_free_int_mem(void** pointer);
void* med_alloc_ext_mem(kal_int32 size);
void med_free_ext_mem(void** pointer);
kal_int32 med_check_disc_space(kal_wchar* dir_name, kal_int32 free_byte);
kal_int16 med_get_media_type(kal_wchar* file_name);
kal_bool med_retrieve_file_path(kal_wchar *file_name, kal_wchar *path);
kal_bool med_remove_file_name(kal_wchar *file_name);

/* crc */
kal_uint32 med_crc_calculate(kal_uint8 const *data, kal_uint32 length);
void med_timer_expiry_callback(void *arg);

#endif /* _MED_UTILITY_H */

#endif /* MED_NOT_PRESENT */

