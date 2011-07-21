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
 * SmsGuiInterfaceProt.H
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

	FILENAME	: SmsGuiInterfaceProt.h

  	PURPOSE		: ???

 

	AUTHOR		: Manu Vikram and Magesh k, Hiran

	DATE		: ???

**************************************************************/

#ifndef _PIXCOM_SMS_GUI_INTERFACE_PROT_H
#define _PIXCOM_SMS_GUI_INTERFACE_PROT_H

#include "l4dr.h"

#include  "mmi_data_types.h"
#include "messagesl4def.h"
#include "messagesmiscell.h"
#include "gui_data_types.h"
#include "ems.h"


void InitMessagesApp (void);
void DeInitMessagesApp (void);
void HighlightVoiceMailHandler (void);
void HighlightCBHandler (void);
void HighlightWMessageHandler (void);
void HighlightInboxHandler (void);

#if defined(__MMI_MESSAGES_TEMPLATE__)
void HighlightTemplatesHandler (void);
#endif // defined(__MMI_MESSAGES_TEMPLATE__)

void HighlightOutboxHandler (void);
void HintInboxHandler (U16 index);
void HintOutboxHandler (U16 index);
 #ifdef __MMI_MESSAGES_DRAFT_BOX__
void HighlightDraftboxHandler (void);
void HintDraftboxHandler (U16 index);
#endif
void EntryScrMessagesMenuList (void);
void ExitScrnMessagesMenuList (void);
#ifdef __MMI_MESSAGES_MULTI_APP__
void EntrySMSMainMenuList (void);
void ExitScrSMSMainMenuList(void);
#endif
void mmi_msg_set_highlight_handler (void);
void mmi_msg_set_msg_menu_highlight_handler(void);
#ifdef __MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__
void mmi_msg_refresh_memory_status (void);
#endif
void RefreshMessagesMenuList (void);

void GetHiliteIndex (S32 nIndex);
void GetMsgIndex (S32 nIndex);

int IsAPhoneNumber (S8 * number);


/********************************************************

					new functions

********************************************************/
void mmi_msg_get_mailbox_address_req (void);
void mmi_msg_get_mailbox_address_rsp_display (void* inMsg);
void mmi_msg_get_mailbox_address_rsp_dial (void* inMsg);
void mmi_msg_set_mailbox_address_req (U8 mailboxNum, U8* number);
void mmi_msg_set_mailbox_address_rsp (void* inMsg);

void mmi_msg_save_sc_address_for_send (void);
void mmi_msg_save_sc_address (void);
void mmi_msg_save_profile_itmes (void);
void mmi_msg_save_common_settings (void);
void mmi_msg_save_mailbox_address (void);
void mmi_msg_erase_mailbox_address (void);
void mmi_msg_dial_mailbox_address (void);

void mmi_msg_get_profile_num_req (void);
void mmi_msg_get_profile_highlight_index (S32 nIndex);
void mmi_msg_entry_msg_settings (void);
void mmi_msg_pre_entry_msg_settings (void);
void mmi_msg_entry_profile_edit (void);
void mmi_msg_exit_profile_edit (void);
void mmi_msg_entry_profile_settings (void);
void mmi_msg_entry_common_settings (void);
#ifdef __MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__
void mmi_msg_entry_memory_status (void);
void mmi_msg_entry_preferred_storage (void);
#endif //__MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__
#ifdef __MMI_GPRS_FEATURES__
void mmi_msg_entry_bearer_settings (void);
#endif
void mmi_msg_go_back_from_voice_mail_server (void);
void mmi_msg_end_key_from_voice_mail_server (void);
void mmi_msg_pre_entry_voice_mail_server (void);
void mmi_msg_entry_voice_mail_server (void);
pBOOL mmi_msg_voice_mail_server_get_item (S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask);
S32 mmi_msg_voice_mail_server_get_hint ( S32 item_index, UI_string_type *hint_array );
void mmi_msg_entry_voice_mail_server_option (void);
void mmi_msg_entry_voice_mail_server_edit (void);

void mmi_msg_highlight_msg_settings (void);
void mmi_msg_highlight_profile_settings (void);
void mmi_msg_highlight_common_settings (void);
#ifdef __MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__
void mmi_msg_highlight_memory_status (void);
void mmi_msg_highlight_preferred_storage (void);
void mmi_msg_hint_preferred_storage (U16 index);
#endif
#ifdef __MMI_GPRS_FEATURES__
void mmi_msg_highlight_bearer_settings (void);
void mmi_msg_hint_bearer_settings (U16 index);
#endif
void mmi_msg_highlight_voice_mail_server_edit (void);
void mmi_msg_highlight_voice_mail_server_erase (void);
void mmi_msg_highlight_voice_mail_server_dial (void);
U8* ConvertL4NumberStructToDisplay (U8 *number, U8 length, U8 type);
void DialVoiceMail(void);
void FreeVoiceMailData (void);
void DeInitVoiceMailApp(void);

void mmi_msg_profile_edit_sc_address_empty_handler(void);

/*******************************************

			smsmomtguiinterface.c

********************************************/
void mmi_msg_set_msg_icon (U8 blink, U8 unblink);
void mmi_msg_set_msg_waiting_icon(U8 line, U8 type, U8 set);
void mmi_msg_set_funcptr (FuncPtr funcptr);
void mmi_msg_exec_funcptr (void);
void mmi_msg_highlight_generic (U16 lskstr, U16 lskimg, U16 rskstr, U16 rskimg, FuncPtr lskfunc, FuncPtr rskfunc);
void mmi_msg_exit_generic (void);
void mmi_msg_set_confirm_screen (U16 id, FuncPtr lskfunc, FuncPtr rskfunc);
void mmi_msg_entry_confirm_generic (void);
void mmi_msg_exit_confirm_generic (void);
void mmi_msg_set_processing_screen (U16 captionStrId, U16 bodyStrId, U16 animationImageId, U16 rskStrId);
void mmi_msg_entry_processing_generic (void);
void mmi_msg_exit_processing_generic (void);
void mmi_msg_time_out_processing_generic (void);
void mmi_msg_set_popup_generic (U16 strID, U16 imgID, U8 toneID);
void mmi_msg_display_popup_generic (void);
void mmi_msg_entry_popup_generic(U8 * string, U16 imageId, U8 imageOnBottom, U32 popupDuration, U8 toneId);
void mmi_msg_exit_popup_generic (void);
void mmi_msg_inline_generic_custom_function (void);
void mmi_msg_popup_timeout (void);
void mmi_msg_highlight_inline_generic_done (void);
void mmi_msg_entry_new_msg_popup (U8 type);
void mmi_msg_exit_new_msg_popup (void);


void mmi_msg_entry_sending (void);
void mmi_msg_set_msg_num_hint(U16 totalInbox, U16 totalOutbox, U16 totalDraftbox);

void mmi_msg_entry_write_msg (void);
void mmi_msg_entry_option_write (void);


void mmi_msg_phone_number_empty (void);
void mmi_msg_phone_number_not_empty (void);
void mmi_msg_entry_phone_number (void);
void mmi_msg_sc_number_not_empty (void);
void mmi_msg_entry_sc_number (void);

void mmi_msg_entry_send_option (void);
void mmi_msg_entry_option_inbox (void);
void mmi_msg_entry_option_outbox (void);
void mmi_msg_entry_option_default (void);
void mmi_msg_entry_send_option_outbox (void);

#ifdef __MMI_SEND_BY_SEARCH_NAME__	
void mmi_msg_entry_send_only (void);
#endif
#ifdef __MMI_MESSAGES_SEND_TO_MANY__
void mmi_msg_entry_send_to_many (void);
void mmi_msg_entry_send_to_many_list (void);
pBOOL mmi_msg_send_to_many_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask );
S32 mmi_msg_send_to_many_list_get_hint ( S32 item_index, UI_string_type *hint_array );
void mmi_msg_entry_phone_number_from_send_to_many(void);
void mmi_msg_save_phone_number(void);
#endif
void mmi_msg_get_msg_date_time (S8* dateStr, S8* timeStr, U8 scts[]);
U8 mmi_msg_handle_ready_ind_inbox(void * dummy);
void mmi_msg_pre_entry_inbox_list (void);
void mmi_msg_go_back_to_inbox_list (void);
void mmi_msg_entry_inbox_list (void);
pBOOL mmi_msg_inbox_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask );
S32 mmi_msg_inbox_list_get_hint ( S32 item_index, UI_string_type *hint_array );
U8 mmi_msg_handle_ready_ind_outbox(void * dummy);
void mmi_msg_pre_entry_outbox_list (void);
void mmi_msg_entry_outbox_list (void);
pBOOL mmi_msg_outbox_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask );
#ifdef __MMI_MESSAGES_DRAFT_BOX__
U8 mmi_msg_handle_ready_ind_draftbox(void * dummy);
void mmi_msg_pre_entry_draftbox_list (void);
void mmi_msg_entry_draftbox_list (void);
pBOOL mmi_msg_draftbox_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask );
#endif
void mmi_msg_entry_inbox_msg (void);
void mmi_msg_entry_outbox_msg (void);
void mmi_msg_entry_default_msg (void);
void mmi_msg_msg_waiting_ind (U8 type);
U8* mmi_msg_get_msg_waiting_msg (void);
void mmi_msg_entry_msg_waiting_ind (void);
void mmi_msg_deliver_report_ind (void);
void mmi_msg_go_back_from_deliver_report_ind(void);
void mmi_msg_end_key_from_deliver_report_ind(void);
void mmi_msg_get_deliver_report_status (U32 *status, U16 *imgID, U16 *strID);
U8* mmi_msg_get_deliver_report_msg (void);
void mmi_msg_entry_deliver_report_ind (void);
void mmi_msg_go_back_from_mem_exceed_ind (void);
void mmi_msg_end_key_from_mem_exceed_ind (void);
void mmi_msg_entry_mem_exceed_ind (void);
void mmi_msg_class0_msg_ind (void);
void mmi_msg_entry_class0_msg_ind (void);
EMSData* mmi_msg_get_class0_msg (void);
U8* mmi_msg_get_class0_header (void);

void mmi_msg_new_msg_ind (U16 index);
U8* mmi_msg_get_new_msg_ind_string (void);
void mmi_msg_go_back_from_new_msg_ind (void);
void mmi_msg_end_key_from_new_msg_ind (void);
void mmi_msg_entry_new_msg_ind (void);
void mmi_msg_stop_msg_sub_anm (void);
void mmi_msg_exit_inbox_list_dummy (void);

void mmi_msg_start_msg_sub_anm (void);
void mmi_msg_stop_msg_sub_anm (void);
void mmi_msg_callback_msg_sub_anm (void);


void mmi_msg_highlight_done (void);
void mmi_msg_highlight_send_only (void);
#ifdef __MMI_SEND_BY_SEARCH_NAME__	
void mmi_msg_highlight_send_only_number (void);
void mmi_msg_highlight_send_only_name (void);
#endif
void mmi_msg_highlight_send_and_save (void);
void mmi_msg_highlight_save (void);
void mmi_msg_highlight_send_to_many (void);
#ifdef __MMI_MESSAGES_SEND_TO_MANY__
void mmi_msg_highlight_send_to_many_list (void);
void mmi_msg_highlight_send_to_many_now (void);
#endif
#ifdef __MMI_MESSAGES_SEND_BY_GROUP__
void mmi_msg_highlight_send_by_group (void);
#endif
void mmi_msg_highlight_reply (void);
void mmi_msg_highlight_send_outbox (void);
void mmi_msg_highlight_edit (void);
void mmi_msg_highlight_delete (void);
void mmi_msg_highlight_delete_all_inbox (void);
void mmi_msg_highlight_delete_all_outbox (void);

#ifdef __MMI_MESSAGES_COPY__
void mmi_msg_highlight_copy(void);
void mmi_msg_highlight_copy_all_inbox(void);
void mmi_msg_highlight_copy_all_from_sim_inbox(void);
void mmi_msg_highlight_copy_all_from_phone_inbox(void);
void mmi_msg_highlight_copy_all_outbox(void);
void mmi_msg_highlight_copy_all_from_sim_outbox(void);
void mmi_msg_highlight_copy_all_from_phone_outbox(void);
void mmi_msg_entry_copy_all_inbox(void);
void mmi_msg_entry_copy_all_outbox (void);

void mmi_msg_highlight_move(void);
void mmi_msg_highlight_move_all_inbox(void);
void mmi_msg_highlight_move_all_from_sim_inbox(void);
void mmi_msg_highlight_move_all_from_phone_inbox(void);
void mmi_msg_highlight_move_all_outbox(void);
void mmi_msg_highlight_move_all_from_sim_outbox(void);
void mmi_msg_highlight_move_all_from_phone_outbox(void);
void mmi_msg_entry_move_all_inbox(void);
void mmi_msg_entry_move_all_outbox (void);
#endif  //__MMI_MESSAGES_COPY__

void mmi_msg_save_use_number (void);
void mmi_msg_dial_use_number (void);
void mmi_msg_entry_use_number (void);
void mmi_msg_entry_use_number_option (void);
void mmi_msg_highlight_save_use_number (void);
void mmi_msg_highlight_dial_use_number (void);
void mmi_msg_highlight_use_number (void);

#if defined(__MMI_MESSAGES_USE_URL__)
extern void mmi_msg_highlight_use_url(void);
extern void mmi_msg_entry_use_url(void);
extern void mmi_msg_entry_use_url_option(void);
extern void HighlightUseURLOptConnectHandler(void);
extern void mmi_msg_entry_open_url(void);
extern void mmi_msg_end_key_from_use_url(void);
extern void mmi_msg_go_back_use_url_list(void);
#endif /* __MMI_MESSAGES_USE_URL__ */

#if defined(__MMI_MESSAGES_TEMPLATE__)
void mmi_msg_insert_template (void);
void mmi_msg_save_template (void);
void mmi_msg_erase_template (void);
void mmi_msg_pre_erase_template (void);
void mmi_msg_entry_template_list (void);
pBOOL mmi_msg_template_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask );
void mmi_msg_entry_template_option (void);
void mmi_msg_entry_template_edit (void);
void mmi_msg_entry_template_edit_option (void);
void mmi_msg_highlight_save_template (void);
void mmi_msg_highlight_erase_template (void);
void mmi_msg_highlight_edit_template (void);
#endif

#ifdef __MMI_MESSAGES_DELETE_ALL_MENU__
void HighlightSMSDeleteHandler (void);
void mmi_msg_entry_delete(void);
void mmi_msg_highlight_delete_all (void);
#ifdef __MMI_MESSAGES_DRAFT_BOX__
void mmi_msg_highlight_delete_all_draftbox (void);
#endif
#endif
#endif /* _PIXCOM_SMS_GUI_INTERFACE_PROT_H */


