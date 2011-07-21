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
 *  SMSFunc.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef _MMI_SMSFUNC_H
#define _MMI_SMSFUNC_H

void mmi_frm_sms_new_sms (void);
U8 mmi_frm_sms_check_sms_complete (U8 type, U16 index);
U8 mmi_frm_sms_set_sms_status (U8 type, U16 index, U8 newtype);
U8 mmi_frm_sms_get_sms_status (U8 type, U16 index);
U8 mmi_frm_sms_get_sms_dcs (U8 type, U16 index);
U8* mmi_frm_sms_get_sms_address (U8 type, U16 index);
U8* mmi_frm_sms_get_sms_timestamp (U8 type, U16 index);
U8 mmi_frm_sms_read_sms_seg (void);
void mmi_frm_sms_read_sms_req (void* inMsg);
void mmi_frm_sms_get_sms_req (void* inMsg);
void mmi_frm_sms_save_sms_seg (void);
void mmi_frm_sms_save_sms_req (void* inMsg);
void mmi_frm_sms_delete_sms_req (void* inMsg);

#ifdef __MMI_MESSAGES_COPY__
void mmi_frm_sms_pre_copy_sms_req(void* dummy);
void mmi_frm_sms_pre_copy_sms_rsp (void* inMsg);
void mmi_frm_sms_copy_sms_seg_req(void);
void mmi_frm_sms_copy_sms_seg_rsp (void* inMsg);
void mmi_frm_sms_copy_sms_move_rsp (void* inMsg);
void mmi_frm_sms_copy_sms_rollback_rsp (void* inMsg);
void mmi_frm_sms_copy_sms_callback(U16 result, U8 cause);
U8 mmi_frm_sms_get_sms_storage(U8 type, U16 index);
void mmi_frm_sms_copy_sms_abort(void);
#endif

void mmi_frm_sms_abort_sms_req (void* inMsg);
void mmi_frm_sms_send_sms_seg (void);
void mmi_frm_sms_pre_send_sms (void* inMsg);
void mmi_frm_sms_send_sms_req (void);
void mmi_frm_sms_set_sc_addr_for_send_req (void* inMsg);
void mmi_frm_sms_reg_port_req (void* inMsg);
void mmi_frm_sms_get_profile_num_req (void* dummy);
void mmi_frm_sms_get_profile_name_req (void* inMsg);
void mmi_frm_sms_get_sc_addr_req (void* inMsg);
void mmi_frm_sms_set_sc_addr_req (void* inMsg);
void mmi_frm_sms_get_profile_detail_req (void* inMsg);
void mmi_frm_sms_set_profile_detail_req (void* inMsg);
void mmi_frm_sms_get_common_settings_req (void* dummy);
void mmi_frm_sms_set_common_settings_req (void* inMsg);
void mmi_msg_inform_init_done(void);
#ifdef __MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__
void mmi_frm_sms_get_memory_status_req (void* dummy);
void mmi_frm_sms_get_preferred_storage_req (void* dummy);
void mmi_frm_sms_set_preferred_storage_req (void* inMsg);
#endif
#ifdef __MMI_GPRS_FEATURES__
void mmi_frm_sms_get_preferred_bearer_req (void *dummy);
void mmi_frm_sms_set_preferred_bearer_req (void* inMsg);
#endif
void mmi_frm_sms_get_mailbox_info_req (void *dummy);
void mmi_frm_sms_set_mailbox_info_req(void* inMsg);

void mmi_frm_sms_entry_abort (void);
void mmi_frm_sms_entry_send (void);
void mmi_frm_sms_entry_sc (void);
void mmi_frm_sms_entry_number (void);
void mmi_frm_sms_entry_number_done (void);

void mmi_frm_sms_abort_sms (void);
void mmi_frm_sms_send_message(module_type MOD_SRC, module_type MOD_DEST, U8 MSG_SAP, const U32/*U16*/ MSG_ID, void* LOCAL1, void* PEER);
U8 mmi_frm_sms_write_action (PsFuncPtrU16 callback, module_type mod_src, void* data, PsFuncPtr function);
U8 mmi_frm_sms_read_action (void);
void mmi_frm_sms_callback_action (void* data, U16 result);

void mmi_frm_sms_reg_interrupt_handler(module_type mod,  /*U16*/U32 msgid, PsFuncPtrU16 callback);   
void mmi_frm_sms_convert_time_stamp (U8* scts, U8* timestamp);
void mmi_frm_sms_free_awaited(U8 index);
void mmi_frm_sms_free_data(U8 index);
U8 mmi_frm_sms_add_data_entry (void);
void mmi_frm_sms_free_new_data (void);
void mmi_frm_sms_new_data (void);
void mmi_frm_sms_indicate_sms (U16 index);


void mmi_frm_sms_create_sms_list (void);
U8 mmi_frm_sms_check_concatenate_sms (U16* index);
U8 mmi_frm_sms_check_data_complete(U8 index);
U8 mmi_frm_sms_check_concatenate_data (U8* index);
U8 mmi_frm_sms_get_awaited_sms (U8 index, mmi_frm_sms_deliver_msg_struct* entry, U8* content);
void mmi_frm_sms_add_new_sms (mmi_frm_sms_deliver_msg_struct* entry, U16 length, U8* data);
void mmi_frm_sms_add_new_data (mmi_frm_sms_deliver_data_struct* entry, U16 length, U8* data);
void mmi_frm_sms_free_new_sms (void);
void mmi_frm_sms_convert_new_data (MMI_FRM_SMS_APP_DATA_IND_STRUCT* data, mmi_frm_sms_deliver_data_struct* entry);
void mmi_frm_sms_convert_new_sms (MMI_FRM_SMS_DELIVER_MSG_IND_STRUCT* data, mmi_frm_sms_deliver_msg_struct* entry);
void mmi_frm_sms_convert_read_to_entry (MMI_FRM_SMS_GET_MSG_RSP_STRUCT* data, mmi_frm_sms_msgbox_struct* entry, U16 index);
void mmi_frm_sms_convert_startup_read_to_entry (MMI_FRM_SMS_STARTUP_READ_MSG_IND_STRUCT* data, mmi_frm_sms_msgbox_struct* entry);
void mmi_frm_sms_convert_mt_to_entry (mmi_frm_sms_deliver_msg_struct* data, mmi_frm_sms_msgbox_struct* entry);
#if defined (__MMI_MESSAGES_COPY__)||defined (__MMI_MESSAGE_DIFFERENTIATE_STORAGE__)
void mmi_frm_sms_convert_mo_to_entry (mmi_frm_sms_send_struct* data, mmi_frm_sms_msgbox_struct* entry, 
															U16 index, U8 status, U8 totalsegment, U16 reference, U8 storagetype);
#else
void mmi_frm_sms_convert_mo_to_entry (mmi_frm_sms_send_struct* data, mmi_frm_sms_msgbox_struct* entry, 
															U16 index, U8 status, U8 totalsegment, U16 reference);
#endif
U16 mmi_frm_sms_add_sms_to_msgbox (mmi_frm_sms_msgbox_struct* entry, U16 index, U8 thisseg);
void mmi_frm_sms_get_sms_list_index (U8* type, U16* index, U16 msgbox_index);
void mmi_frm_sms_get_sms_index (mmi_frm_sms_msgbox_enum type, U16 index, U16* l4_index);
void mmi_frm_sms_get_sms_msgbox_index (U16 l4_index, U16* msgbox_index);
U8 mmi_frm_sms_get_sms_bitmap (mmi_frm_sms_msgbox_enum type, U16 index, U8* l4_index);
void mmi_frm_sms_remove_sms_from_msgbox (U8* data);
void mmi_frm_sms_remove_sms_entry (U16 index);
void mmi_frm_sms_new_msg_ind (U16 index);
U8 mmi_frm_sms_get_data_index (module_type mod, U16 port, U8 startindex);
U8 mmi_frm_sms_get_data(U8 index, mmi_frm_sms_deliver_data_struct* data, U8* content);
void mmi_frm_sms_remove_data (module_type mod, U16 port);

#ifdef __MMI_MESSAGES_SINGLE_DELIVERY_REPORT__
void mmi_frm_sms_check_delivery_report_awaited(MMI_FRM_SMS_SEND_MSG_RSP_STRUCT *inMsg);
U16 mmi_frm_sms_get_delivery_report_index_by_mr(U8 msg_ref);
U16 mmi_frm_sms_get_delivery_report_index_by_ref(U16 ref);
void mmi_frm_sms_remove_delivery_report_awaited(U16 index);
void mmi_frm_sms_set_delivery_report_awaited_check(U16 index, U8 msg_ref, U8 status_type);
BOOL mmi_frm_sms_check_delivery_report_awaited_complete(U16 index);
void mmi_frm_sms_add_delivery_report_awaited(U16 reference_no, U8 total_seg, U8 box_index, U8 msg_ref);
void mmi_frm_sms_set_delivery_report_awaited_mr(U16 delivery_report_awaited_index, U8 msg_ref, U8 seq_number);
U8 mmi_frm_sms_get_delivery_report_awaited_result(U16 index);
#endif 

void mmi_frm_sms_set_protocol_event_handler (void);
void mmi_frm_sms_handle_extension_char(U16* dstBuffer, U16* srcBuffer,U16 charNum );
BOOL mmi_frm_sms_check_UCS2(U16* buffer , U16 charNum);

void playShortMessageTone(U8 audioId) ;

#ifdef __MMI_MESSAGES_USE_URL__
void mmi_frm_sms_startup_wap_browser_req(void *URL);
#endif	/* __MMI_MESSAGES_USE_URL__ */

#ifdef __MMI_MULTI_SIM__
extern U8 mmi_frm_sms_record_get_sim_id(U16 box, U16 index);
extern void mmi_frm_sms_set_active_simid(U8 nSimID);
extern U8 mmi_frm_sms_get_active_simid(void);
extern void mmi_msg_set_msg_need_select_sim_when_send(BOOL isNeed);
extern void mmi_msg_entry_select_save_option_screen(void);
extern void mmi_msg_highlight_send_from_SIM_Select(void);
extern void mmi_msg_highlight_save_from_SIM_select(void);
extern U8 mmi_frm_sms_get_multi_sms_bitmap(U16 type, U16 index, U8 *l4_index, U8 nSimID);
#endif

#endif
