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

/*****************************************************************************
 *
 * Filename:
 * ---------
 * SmsPsHandler.H
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for MMI SMS APP.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: SmsPsHandler.h

  	PURPOSE		: ---

 

	AUTHOR		: Magesh k, Hiran

	DATE		: ---

**************************************************************/

#ifndef _PIXCOM_SMS_PRT_MSG_HANDLER_H
#define _PIXCOM_SMS_PRT_MSG_HANDLER_H

#include "mmi_data_types.h"
#include "messagesl4def.h"
#include "messagesmiscell.h"
#include "emsmiscell.h"
//
#define MSG_INVALID_TIMER						0xff
#define MSG_MAX_TIMER							12

//
typedef enum
{
	MSG_LIST_AWAITED,
	MSG_LIST_INBOX,
	MSG_LIST_OUTBOX,
	MSG_LIST_DRAFTBOX
}msgbox_list_type_enum;



/*******************************************

			new functions

*******************************************/
void mmi_msg_handle_ready_ind (void* dummy, module_type mod, U16 result);
void mmi_msg_handle_ready_ind_after_init (void* dummy);
void mmi_msg_callback_init_msg_process (void* dummy);
void mmi_msg_sync_msgbox_with_at (void* dummy, module_type mod, U16 result);
void mmi_msg_handle_msg_waiting_ind (void *inMsg, module_type mod, U16 result);
void mmi_msg_handle_deliver_report_ind (void* inMsg, module_type mod, U16 result);
void mmi_msg_handle_mem_full_ind(void* dummy, module_type mod, U16 result);
void mmi_msg_handle_mem_exceed_ind(void* dummy, module_type mod, U16 result);
void mmi_msg_handle_mem_avail_ind(void* dummy, module_type mod, U16 result);
U8 mmi_msg_handle_class0_msg_ind(void* inMsg, void* content);
void mmi_msg_handle_new_msg_ind(void* data, module_type mod, U16 index);
U8 mmi_msg_check_new_msg_ind (U8 type, U8 len, U8* number, U8* timestamp, U16 ref, U8 totalseg, U8 currseg, U16 l4index, U8 mti);
#ifdef __MMI_MESSAGES_CHAT__
void mmi_msg_check_chat_msg(U16 index);
#endif
void mmi_msg_get_msg_new (void);

void mmi_msg_check_fdn_req (void);
void mmi_msg_check_fdn_rsp (void* inMsg);
void mmi_msg_send_msg_req (U8* number, U16 replyindex, U8 hidenumscreen);
void mmi_msg_send_and_save_msg_req (U8* number, U16 replyindex);
void mmi_msg_save_msg_after_send_req (U8 status, void* number);
void mmi_msg_send_msg_to_many_req (U8* number);

void mmi_msg_abort_msg_req (void);
void mmi_msg_abort_msg_rsp (void* inMsg);

U8 mmi_msg_pack_msg (void);
void mmi_msg_send_msg (void);
void mmi_msg_save_msg (U8 status);
void mmi_msg_abort_msg (void);
void mmi_msg_search_phb (void);
void mmi_msg_send_msg_to_entry (void);
void mmi_msg_send_msg_to_entry_next (void);
#ifdef __MMI_MESSAGES_SEND_TO_MANY__
void mmi_msg_send_msg_to_many (void);
#endif
#ifdef __MMI_MESSAGES_SEND_BY_GROUP__
void mmi_msg_send_msg_to_group_entry (U16 index, U8 isfirst);
void mmi_msg_send_msg_to_group (U16 cnt, U16* PhbIndex);
void mmi_msg_send_msg_by_group (void);
#endif
void mmi_msg_send_msg_to_only_entry (void);
#ifdef __MMI_SEND_BY_SEARCH_NAME__	
void mmi_msg_send_msg_to_only_name_entry (void);
#endif
void mmi_msg_save_msg_to_entry (void);
void mmi_msg_send_and_save_to_entry (void);
void mmi_msg_reply_msg (void);
void mmi_msg_reply_msg_to_entry (void);
void mmi_msg_get_number_from_phb (U8 name[], U8 number[]);
void mmi_msg_get_number_from_clg(U8 number[]);//wangwb @2007.06.12    send sms form call log (interface)
void mmi_msg_edit_msg_from_view (void);
void mmi_msg_delete_msg (void);
void mmi_msg_pre_delete_msg_all_inbox (void);
void mmi_msg_delete_msg_all_inbox (void);
void mmi_msg_pre_delete_msg_all_outbox (void);
void mmi_msg_delete_msg_all_outbox (void);

#ifdef __MMI_MESSAGES_COPY__
void mmi_msg_copy_msg(void);
void mmi_msg_copy_msg_all_abort(void);
void mmi_msg_copy_msg_all_from_sim_inbox(void);
void mmi_msg_copy_msg_all_from_phone_inbox(void);
void mmi_msg_copy_msg_all_from_sim_outbox(void);
void mmi_msg_copy_msg_all_from_phone_outbox(void);
void mmi_msg_copy_msg_all_from_sim_draftbox(void);
void mmi_msg_copy_msg_all_from_phone_draftbox(void);
void mmi_msg_move_msg(void);
void mmi_msg_move_msg_all_abort(void);
void mmi_msg_move_msg_all_from_sim_inbox(void);
void mmi_msg_move_msg_all_from_phone_inbox(void);
void mmi_msg_move_msg_all_from_sim_outbox(void);
void mmi_msg_move_msg_all_from_phone_outbox(void);
void mmi_msg_move_msg_all_from_sim_draftbox(void);
void mmi_msg_move_msg_all_from_phone_draftbox(void);
#endif // __MMI_MESSAGES_COPY__

#ifdef __MMI_MESSAGES_DRAFT_BOX__
void mmi_msg_pre_delete_msg_all_draftbox (void);
void mmi_msg_delete_msg_all_draftbox (void);
void mmi_msg_delete_msg_rsp_draftbox (void *inMsg);
#endif
#ifdef __MMI_MESSAGES_DELETE_ALL_MENU__
void mmi_msg_delete_msg_all (void);
void mmi_msg_pre_delete_msg_all (void);
#endif

U16 mmi_msg_get_msg_list_avai_index (U8 list);
U16 mmi_msg_get_msg_list_start_index (U8 list);
U16 mmi_msg_get_msg_list_end_index (U8 list);
void mmi_msg_add_msg_new_entry (U8 status, U8 type, U8 len, U8* number, U8* timestamp, U16 ref, U8 totalseg, U8 currseg, U16 l4index, U8 mti);
void mmi_msg_add_msg_list_entry (U16 index, U8 status, U8 type, U8 len, U8* number, U8* timestamp, U16 ref, U8 totalseg, U8 currseg, U16 l4index, U8 mti);
void mmi_msg_change_msg_list_entry_status (U16 index, U8 status);
void mmi_msg_remove_msg_list_entry (U8 list, U16 index);
void mmi_msg_copy_msg_list_entry(U16 toindex, U16 fromindex);
void mmi_msg_add_msg_list_entry_seg (U16 index, U8 seg, U16 l4index);
U16 mmi_msg_search_msg_list_entry (U8 list, U8 type, U8 len, U8* number, U16 ref, U8 totalseg, U8 currseg, U16 l4index);
U8 mmi_msg_check_msg_complete (U16 index);
void mmi_msg_start_msg_timer (U16 index);
void mmi_msg_stop_msg_timer (U16 index);
void mmi_msg_callback_msg_timer (void);

void mmi_msg_get_msg_list_req(smsal_status_enum status, U16 startindex, U8 msgnum);
void mmi_msg_get_msg_list_rsp_inbox(void* inMsg);		
void mmi_msg_get_msg_list_rsp_outbox (void* inMsg)	;	
#ifdef __MMI_MESSAGES_DRAFT_BOX__
void mmi_msg_get_msg_list_rsp_draftbox (void* inMsg)	;	
#endif
void mmi_msg_reset_msg_list(U8 list);

void mmi_msg_get_msg_num_rsp_inbox (void* inMsg);
void mmi_msg_get_msg_num_rsp_outbox (void* inMsg);
#ifdef __MMI_MESSAGES_DRAFT_BOX__
void mmi_msg_get_msg_num_rsp_draftbox (void* inMsg);
#endif
void mmi_msg_reget_msg_list_inbox (void* dummy);

void mmi_msg_get_msg_list_inbox (void);
void mmi_msg_get_msg_list_outbox (void);
#ifdef __MMI_MESSAGES_DRAFT_BOX__
void mmi_msg_get_msg_list_draftbox (void);
#endif
void mmi_msg_get_msg_num_rsp_unread (void* inMsg);
void mmi_msg_get_msg_req (U8 type, U16 index);
void mmi_msg_get_msg_rsp_inbox (void *inMsg);
void mmi_msg_get_msg_rsp_outbox (void *inMsg);
void mmi_msg_get_msg (U16 index);
void mmi_msg_get_msg_inbox (void);
void mmi_msg_get_msg_outbox (void);
#ifdef __MMI_MESSAGES_DRAFT_BOX__
void mmi_msg_get_msg_draftbox (void);
#endif
#ifdef __MMI_MESSAGES_CHAT__
void mmi_msg_get_msg_chat (void);
#endif
void mmi_msg_reset_msg (void);
void mmi_msg_unpack_msg (U8 dcs, U8 fo);
void mmi_msg_unpack_class0_msg (void);

void mmi_msg_set_protocol_event_handler (void);
void mmi_msg_set_protocol_event_handler_after_init (void);

#endif	// _PIXCOM_SMS_PRT_MSG_HANDLER_H

