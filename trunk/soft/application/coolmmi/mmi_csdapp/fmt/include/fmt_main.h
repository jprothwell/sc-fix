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
 *	fmt_main.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for ¡K.
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
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef _FMT_MAIN_H
#define _FMT_MAIN_H

#ifndef FMT_NOT_PRESENT

#ifdef __SXS_NVRAM_RW__  //add by chenzy20091109
#include "Sxs_io.h"
#include "Sxs_rmc.h"
#include "Nvramprot.h"
#include "stdio.h"
#endif

#if 0
kal_bool fmt_create(comptask_handler_struct **handle);
#endif 
kal_bool fmt_init( task_indx_type task_indx );
kal_bool fmt_reset( task_indx_type task_indx );
void fmt_init_drive (void);
void fmt_main( ilm_struct *ilm_ptr);
#if 0
void fmt_task_main( task_entry_struct *task_entry_ptr);
#endif 

void fmt_copy_hdlr(local_para_struct *local_para_ptr,
                                   			peer_buff_struct *peer_buff_ptr);
void fmt_delete_hdlr(local_para_struct *local_para_ptr,
                                   			peer_buff_struct *peer_buff_ptr);
void fmt_format_hdlr(local_para_struct *local_para_ptr,
                                   			peer_buff_struct *peer_buff_ptr);
void fmt_format_done_hdlr(local_para_struct *local_para_ptr,
                                   			peer_buff_struct *peer_buff_ptr);
void fmt_check_drive_hdlr(local_para_struct *local_para_ptr,
                                   			peer_buff_struct *peer_buff_ptr);
void fmt_get_msdc_status_hdlr(local_para_struct *local_para_ptr,
                                   			peer_buff_struct *peer_buff_ptr);

kal_int32 fmt_delete_by_filter_hdlr(kal_uint8 *path, FMGR_FILTER *filter_mask);
int fmt_copy_rsp_hdlr(int action, UINT total, UINT completed, UINT handle);
void fmt_abort_copy (void);
kal_bool fmt_get_abort_copy (void);

#ifdef __SXS_NVRAM_RW__	// add by chenzy 20091109
typedef struct
{
  u8  Lid;
  u8  Recorder;  
  u16  Size;
  u8  *Data;
} sxs_FlwNvram_t;
void fmt_read_nvram_hdlr(local_para_struct *local_para_ptr,
                                   			peer_buff_struct *peer_buff_ptr);
void fmt_write_nvram_hdlr(local_para_struct *local_para_ptr,
                                   			peer_buff_struct *peer_buff_ptr);
void fmt_send_nvram_msg(u8 *data,u8 type);
#endif


#ifdef __FS_CARD_SUPPORT__
void fmt_card_detect_hdlr(local_para_struct *local_para_ptr,
                                   			peer_buff_struct *peer_buff_ptr);
void fmt_unmount_hdlr(local_para_struct *local_para_ptr,
                                   			peer_buff_struct *peer_buff_ptr);
#endif

#ifdef __OTG_ENABLE__
void fmt_otg_card_detect_hdlr(local_para_struct *local_para_ptr,
                                   			peer_buff_struct *peer_buff_ptr);
void fmt_otg_remove_hdlr(local_para_struct *local_para_ptr,
									peer_buff_struct *peer_buff_ptr);
void fmt_otg_insert_hdlr(local_para_struct *local_para_ptr,
								peer_buff_struct *peer_buff_ptr);                                   			
void fmt_otg_display_hdlr(local_para_struct *local_para_ptr,
								peer_buff_struct *peer_buff_ptr);                                   			
#endif
#endif

#endif

