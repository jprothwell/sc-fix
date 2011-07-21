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
 *	PhoneBookProt.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   phone book application 
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
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
 #ifndef _PHONEBOOKPROT_H
#define _PHONEBOOKPROT_H

#include "mmi_data_types.h"

/* 
** Typedef 
*/
typedef	pBOOL (*GetItemPtr)(S32, UI_string_type, UI_image_type*, U8);
typedef	S32 (*GetHintPtr)(S32,UI_string_type*);
typedef void (*FunctionPtr)(void);

/* 
** Define
*/

/* 
** Extern Global Variable
*/

/* 
** Extern Global Function
*/

/*
*  Initialization Application
*/
extern void mmi_phb_search_init(void);
extern void mmi_phb_operate_single_init(void);
extern void mmi_phb_caller_group_init(void);
extern void mmi_phb_operate_mass_init(void);
extern void mmi_phb_extra_number_init(void);
extern void mmi_phb_setting_init(void);
extern void mmi_phb_sos_list_init(void);

#define IN_MMI_PHONEBOOKMAIN_C
/* 
 * Startup procedure -  Indication and initialization
 */
extern void mmi_phb_ind_startup_begin(void*);
#if defined(MMI_ON_HARDWARE_P)
extern void mmi_phb_ind_startup_read(void*);
extern void mmi_phb_ind_startup_read_next_req(U16 access_id,  U8 storage);
#endif
extern void mmi_phb_ind_startup_finish(void*);
extern void mmi_phb_ind_update(void*); 
extern void mmi_phb_init_get_data_from_nvram(void);
extern void mmi_phb_init_build_lookup_table(void);
extern void mmi_phb_init_populate_lookup_table(void);
#if !defined(MMI_ON_HARDWARE_P)
extern void mmi_phb_init_refresh_phonebook(void);
extern void mmi_phb_get_entry_by_index_req(void);			/* Asks stack to load all entries during bootup */
extern void mmi_phb_get_entry_by_index_rsp(void* );		/* Response from stack after loading all the entries */
#endif
#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__)
extern BOOL mmi_phb_init_check_sim_change(void);
extern void mmi_phb_init_entry_sim_refresh_confirm(void);
extern void mmi_phb_init_delete_caller_group_in_sim(void);
#endif
/*
*  Common Screen
*/
//extern void mmi_phb_entry_main_menu(void);	/*Move to AllAppGProt.h*/
extern void mmi_phb_exit_main_menu(void);			/* Exit function for the Phone Book main menu */
extern void mmi_phb_entry_not_ready(void);
extern void mmi_phb_exit_not_ready(void);
extern void mmi_phb_wait_and_go_back(void);
extern void mmi_phb_show_in_progress(U16, U16);
extern void mmi_phb_entry_in_progress(void);
extern void mmi_phb_show_mass_processing(U16, U16);
extern void mmi_phb_entry_mass_processing(void);
extern void mmi_phb_exit_mass_processing(void);

/*
*Phonebook List Screen
*/
extern void mmi_phb_list_allocate_filter(void);
extern void mmi_phb_list_free_filter(void);
extern void mmi_phb_list_pre_entry(void);
extern void mmi_phb_list_pre_entry_second_level(void);
extern void mmi_phb_list_pre_entry_for_fmgr_in_nvram(void);
extern void mmi_phb_list_pre_entry_for_mms_number_and_email(void);
extern void mmi_phb_list_pre_delete_one_by_one(void);
extern void mmi_phb_entry_list(U16 EntryCount, U16 STR_TITLE,U16 IMG_TITLE,U16 STR_LSK, U16 IMG_LSK, void (*HighlightPtr)(S32), FunctionPtr LSKPtr, 
   FunctionPtr SendKeyPtr, GetItemPtr ItemPtr, GetHintPtr HintPtr, FunctionPtr EntryPtr, BOOL alpha_index, BOOL right_arrow);
extern void mmi_phb_exit_list(void);
extern pBOOL mmi_phb_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask );
extern S32 mmi_phb_list_get_hint ( S32 item_index, UI_string_type *hint_array );
extern pBOOL mmi_phb_filter_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask );
extern S32 mmi_phb_filter_list_get_hint ( S32 item_index, UI_string_type *hint_array );
extern S32 mmi_phb_list_get_null_hint ( S32 item_index, UI_string_type *hint_array );
extern pBOOL mmi_phb_member_list_get_item(S32, UI_string_type, UI_image_type*, U8);
extern S32 mmi_phb_member_list_get_hint (S32, UI_string_type* );
extern void mmi_phb_choose_number_normal(void);
extern void mmi_phb_choose_number_ip_number(void);
extern void mmi_phb_list_build_alpha_index(U16 *filter_list, U16 filter_value, BOOL is_equal, U8 storage);
extern void mmi_phb_list_alpha_index(U16);
extern void mmi_phb_list_alpha_index_input_callback(void);
extern void mmi_phb_list_pre_choose_number(void);
extern void mmi_phb_entry_list_choose_number(U16 store_index, void (*EntryFunc)(void), void (*RSKFunc)(void), void(*ActionFunc)(S8*), U16 empty_str_id, BOOL dial_list);
extern void mmi_phb_exit_list_choose_number(void);
extern void mmi_phb_entry_choose_field(
        U16 store_index,
        U16 field_type,
        FunctionPtr entry_func,
        FunctionPtr lsk_func,
        FunctionPtr rsk_func,
        FunctionPtr send_key_func,
        MMI_BOOL show_if_empty);
extern void mmi_phb_exit_choose_field(void);
extern void mmi_phb_list_make_call (void);

#ifdef __MMI_MULTI_SIM__
extern void EntryScrForPHBCopyChoose(void);
extern void ExitScrForPHBCopyChoose(void);
extern void EntryScrForPHBMoveChoose(void);
extern void ExitScrForPHBMoveChoose(void);
#endif
/*
* Generic Utilities Function
*/
#if defined( __MMI_INTELLIGENT_CALL_ALERT__ )
extern void mmi_phb_get_index_speak_name(S32 nIndex);
extern void mmi_phb_list_speak_name(void);
#endif
extern void mmi_phb_get_index(S32 nIndex);		/* Gets the index of highlighted list item */
extern void mmi_phb_get_index_second_level(S32 nIndex);	/* Gets the index of highlighted list item in second level*/
extern void mmi_phb_get_index_third_level(S32 nIndex) ;	/* Gets the index of highlighted list item in third level*/
extern void mmi_phb_get_index_by_store_location(S32 nIndex);
extern void mmi_phb_get_index_by_store_location_second(S32 nIndex);
extern void mmi_phb_util_clear_buffer(BOOL);
extern void mmi_phb_go_back_2_history(void);
extern void mmi_phb_go_back_3_history_and_clear(void);
extern void mmi_phb_set_full_scr_lsk(void);
extern void mmi_phb_clear_old_history(void);
extern void mmi_phb_reset_scr_id(void);

/*
*	Image List Functions
*/
extern void mmi_phb_image_list1_draw_title_info(S32 item_idx, S32 x1, S32 y1, S32 x2, S32 y2);
extern void mmi_phb_entry_image_list1(void);
extern void mmi_phb_exit_image_list1(void);
extern U8 PHBGetStorageByStoreIndex(U16 nStoreIndex);
extern U16 PHBGetADPIndexByStorage(U8 storage, U16 storeindex);

#define IN_MMI_PHONEBOOKOPERATESINGLE_C
/*
*Entry Option - Entry Function
*/
extern void mmi_phb_entry_op_option(void);		/* Displays view options for an entry */
extern void mmi_phb_exit_op_option(void);

/*
*Add New Entry
*/
extern void mmi_phb_highlight_op_add_entry(void);		/* Highlight handler for NEW ENTRY option*/
extern void mmi_phb_highlight_op_add_entry_to_sim(void);
extern void mmi_phb_highlight_op_add_entry_to_phone(void);
extern void mmi_phb_hint_op_add_entry_to_sim(U16);
extern void mmi_phb_hint_op_add_entry_to_phone(U16);
extern void mmi_phb_op_add_pre_entry(void);		/* Entry function for adding an entry */
extern void mmi_phb_entry_op_add_choose_storage(void);
extern void mmi_phb_exit_op_add_choose_storage(void);
extern void mmi_phb_op_fill_inline_item(U8 field_index, U8 item_index,U16 store_index, U16*string_list, U16* image_list);
extern void mmi_phb_entry_op_add_entry(void);
extern void mmi_phb_exit_op_add_entry(void);
extern void mmi_phb_entry_op_add_entry_confirm(void);
extern BOOL mmi_phb_op_check_valid_number(S8* number);
extern BOOL mmi_phb_op_check_pre_save(void);
extern void mmi_phb_op_add_pre_save(void);
extern void mmi_phb_op_add_entry_req(void);		/* Send information to stack to store an entry */
extern void mmi_phb_op_add_entry_Directly_req(S8* in_name, S8* in_num);
extern void mmi_phb_op_add_entry_rsp(void*);	/* Response from stack after storing an entry */
extern void mmi_phb_highlight_import_and_export(void);

/* 
*Entry Option - View entry
*/
extern void mmi_phb_highlight_op_view_entry(void);
extern void mmi_phb_entry_op_view_entry(void);
extern void mmi_phb_exit_op_view_entry(void);
extern void mmi_phb_entry_op_view_picture(void);
extern void mmi_phb_view_entry_highlight_hdlr(s32);


/*
*  Entry Option - Edit entry
*/
extern void mmi_phb_highlight_op_edit_entry(void);
extern void mmi_phb_op_edit_pre_entry(void);
extern void mmi_phb_entry_op_edit_entry(void);	/* Displays an entry in detail for edit*/
extern void mmi_phb_exit_op_edit_entry(void);
extern void mmi_phb_op_edit_highlight_hdlr(S32);
extern void mmi_phb_entry_op_edit_view_picture(void);
extern void mmi_phb_exit_op_edit_view_picture(void);
extern void mmi_phb_entry_op_edit_save_confirm(void);
extern void mmi_phb_op_edit_pre_save(void);
extern void mmi_phb_op_edit_entry_req(void);
extern void mmi_phb_op_edit_entry_rsp(void*);

/* 
*  Entry Option - Delete entry
*/
extern void mmi_phb_highlight_op_delete_entry(void);
extern void mmi_phb_entry_op_delete_confirm(void);		/* Entry function for delete confirmation */	
extern void mmi_phb_op_delete_entry_req(void);					/* Deletes a phone book entry */
extern void mmi_phb_op_delete_entry_rsp(void*);	/* Response from stack after deleting */
extern void mmi_phb_op_delete_update_index(U16 store_index);

/* 
*  Entry Option - Copy, Move, Duplicate
*/
extern void mmi_phb_highlight_op_copy_entry(void);
extern void mmi_phb_highlight_op_move_entry(void);
extern void mmi_phb_highlight_op_duplicate_entry(void);
#ifdef __MMI_MULTI_SIM__
extern void mmi_phb_highlight_op_copy_sim1_phone(void);
extern void mmi_phb_highlight_op_move_sim1_phone(void);
#endif
extern void mmi_phb_entry_op_copy_confirm(void);
extern void mmi_phb_entry_op_move_confirm(void);
extern void mmi_phb_entry_op_dulicate_confirm(void);
#if defined(__MMI_PHB_COMBINE_COPY_DUPLICATE__)	/*Combine copy and duplicate into one option.*/
extern void mmi_phb_highlight_op_copy_duplicate(void);
extern void mmi_phb_entry_op_copy_duplicate(void);
extern void mmi_phb_exit_op_copy_duplicate(void);
#endif
extern void mmi_phb_op_copy_move_dup_req(void);		/* Send information to stack to edit an entry */
extern void mmi_phb_op_copy_move_dup_rsp(void*);
extern void mmi_phb_op_move_req(void);
extern void mmi_phb_op_move_rsp(void*);

/*
*  Extra Operations
*/
extern void mmi_phb_highlight_op_dial_number(void);
#if defined(__MMI_PHB_IP_DIAL_FROM_PHB__)       /* IP Dial from phonebook entry option menu. */
extern void mmi_phb_highlight_op_ip_number(void);
#endif 

#if defined(__MMI_PHB_SEND_SMS_FROM_PHB__)
void mmi_phb_highlight_op_send_sms(void);		/*Send SMS from phonebook entry option menu.*/
#endif
#if defined(__MMI_PHB_SEND_MMS_FROM_PHB__)
void mmi_phb_highlight_op_send_mms(void);
#endif

/*
*Entry Option - Process Image From File Manager.
*/
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
extern void mmi_phb_image_highlight_hdlr(S32 index);
extern void mmi_phb_black_list_highlight_hdlr(S32 index);
extern void mmi_phb_red_list_highlight_hdlr(S32 index);
extern void mmi_phb_image_pre_entry_option(void);
extern void mmi_phb_entry_image_option(U8 type);
extern void mmi_phb_exit_image_option(void);
extern void mmi_phb_highlight_image_view(void);
extern void mmi_phb_highlight_image_select(void);
extern void mmi_phb_highlight_image_delete(void);
extern void mmi_phb_image_view_from_phb_folder(void);
extern void mmi_phb_image_select_from_file_mamager(void);
extern void mmi_phb_image_select_from_file_mamager_done(void *path, int is_short);
extern void mmi_phb_image_delete_from_phb_folder(void);
extern BOOL mmi_phb_image_update_by_index(U16);
extern void mmi_phb_image_delete_by_index(U16);
extern BOOL mmi_phb_image_duplicate_by_index(U16 dst_record, U16 src_record);
extern BOOL mmi_phb_image_copy(S8* dst, S8* src);
#endif
#if defined(__MMI_FILE_MANAGER__) 
extern BOOL mmi_phb_image_check_valid_image(S8* path);
extern void mmi_phb_image_get_full_path_by_index(U16 record_index, S8* path);
#endif
extern S32 mmi_phb_fs_check_image_folder(S8* path);

#if defined(__MMI_INCOMING_CALL_VIDEO__)
extern void mmi_phb_video_init_data_store(void);
extern void mmi_phb_video_highlight_hdlr(S32 index);
extern void mmi_phb_highlight_video_default_view(void);
extern void mmi_phb_highlight_video_setting(void);
extern void mmi_phb_hint_video_setting(U16 index);
extern void mmi_phb_highlight_video_file_view(void);
extern void mmi_phb_highlight_video_file_select(void);
extern void mmi_phb_highlight_video_file_delete(void);
extern void mmi_phb_entry_video_default_option(void);
extern void mmi_phb_video_pre_entry_file_option(void);
extern void mmi_phb_entry_video_file_option(U8 type);
extern void mmi_phb_video_view_from_default(void);
extern void mmi_phb_video_view_from_file(void);
extern void mmi_phb_video_select_from_file_mamager(void);
extern void mmi_phb_video_select_from_file_mamager_done(void* path, int is_short);
extern void mmi_phb_video_delete_file_reference(void);
extern void mmi_phb_video_toggle_setting(void);
extern U16 mmi_phb_video_get_id_by_index(U16 store_index);
extern BOOL mmi_phb_video_get_path_by_index(U16 store_index);
extern S8* mmi_phb_caller_video_get_path_by_index(U16 video_id);
extern void mmi_phb_video_delete_by_index(U16 store_index);
extern BOOL mmi_phb_video_duplicate_by_index(U16 dest_index, U16 src_index);
extern BOOL mmi_phb_video_update_by_index(U16 store_index);
extern U16 mmi_phb_video_update_by_index_for_path(U16 original_id, BOOL *update_result);
extern BOOL mmi_phb_video_check_data_space(S8* path);
#endif

#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
extern void mmi_phb_util_clear_bday_buffer(void);
extern void mmi_phb_bday_set_changed(void);
extern void mmi_phb_bday_editor_callback(U8 *DateStr, U8 *day,U8 *month,U8 *year);
extern void mmi_phb_bday_update_field(U16 store_index);
extern void mmi_phb_bday_read_data_to_buff(U16 store_index);
extern void mmi_phb_bday_delete_field(U16 store_index);
extern void mmi_phb_bday_duplicate_field(U16 store_index,  U16 src_store_index);
#endif

#define IN_MMI_PHONEBOOKOPERATEMASS_C
/*
* Copy All
*/
extern void mmi_phb_highlight_copy_all(void);
extern void mmi_phb_highlight_copy_all_from_sim(void);
extern void mmi_phb_highlight_copy_all_from_phone(void);
extern void mmi_phb_hint_copy_all_from_sim(U16);
extern void mmi_phb_hint_copy_all_from_phone(U16);
extern void mmi_phb_entry_copy_all(void);
extern void mmi_phb_exit_copy_all(void);
extern void mmi_phb_entry_op_copy_all_confirm(void);
extern void mmi_phb_op_copy_all_pre_req(void);
extern void mmi_phb_op_copy_all_one_after_one_req(U8 from_storage);
extern void mmi_phb_op_copy_all_one_after_one_rsp(void* info);
extern void mmi_phb_op_copy_all_done(void);

/*
*  Delete All
*/
extern void mmi_phb_highlight_delete_all(void);	
extern void mmi_phb_highlight_delete_all_from_sim(void);
extern void mmi_phb_highlight_delete_all_from_phone(void);
extern void mmi_phb_hint_delete_all_from_sim(U16);
extern void mmi_phb_hint_delete_all_from_phone(U16);
extern void mmi_phb_highlight_delete_all_one_by_one(void);
extern void mmi_phb_entry_delete_all(void);
extern void mmi_phb_exit_delete_all(void);
extern void mmi_phb_entry_delete_all_confirm(void);
extern void mmi_phb_entry_delete_all_phone_lock(void);
extern void mmi_phb_delete_all_verify_phone_lock(void);
extern void mmi_phb_op_delete_all_update_index(U8 storage);
extern void mmi_phb_op_delete_all_update_lookup_table(U8 storage);
extern void mmi_phb_op_delete_all_update_optioal_id(void);
extern void mmi_phb_op_delete_all_done(U8 storage);
extern void mmi_phb_op_delete_all_pre_req(void);
extern void mmi_phb_op_delete_all_one_after_one_req(U8 delete_storage);
extern void mmi_phb_op_delete_all_one_after_one_rsp(void* info);

/*
* AT command
*/
extern void mmi_phb_at_cmd_edit(void*);
extern void mmi_phb_at_cmd_add(void*);
extern void mmi_phb_at_cmd_delete(void*);
extern void mmi_phb_at_cmd_delete_all(void*);

/*
*  Entry Option - Common Util
*/
extern void mmi_phb_mass_processing_abort(void);
extern void mmi_phb_op_update_optional_field(U16 store_index, U8 storage, U8 type) ;
extern void mmi_phb_read_optional_ids(U16 store_index);
extern void mmi_phb_clear_optional_ids(U16 store_index);
#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
extern void mmi_phb_clear_optional_ids_for_sim(U16 store_index);
#endif
extern void mmi_phb_copy_optional_ids(U16 dst_store_index, U16 src_store_index);
extern void mmi_phb_op_set_entry_prepare_name_number(void* dst_struct, S8* in_name, S8* in_num);
extern void mmi_phb_op_set_entry_prepare_name_number_by_index(void* dst_struct, U16 store_index);
extern void mmi_phb_op_set_entry_rsp_name_number(void* rsp_struct, U16 store_index);
extern U16 mmi_phb_op_get_store_index_and_increase_counter(U8 storage, U16 record_index);
extern U16 mmi_phb_op_clear_lookup_table_by_index(U16 store_index);
extern U8 mmi_phb_op_increase_lookup_table(U16 store_index, S8* number, S8* fax, S8* home, S8* office);
extern void mmi_phb_op_update_field_flag(U16 store_index, S8* number, S8* home, S8* office, S8* fax, S8* email, BOOL is_new);



#define IN_MMI_PHONEBOOKSEARCH_C
/*
* Sort Name
*/
extern void mmi_phb_sort_build_name_index(void);
extern void mmi_phb_sort_compare_name_index(U16 first, U16 second, S8* string1, S8* string2,U8 *same_count, S32 *distance);
extern void mmi_phb_sort_compare_name_index_by_encoding(U16 first, U16 second,  U8 *same_count, S32 *distance);
#if defined(__MMI_PHB_PINYIN_SORT__)
extern void mmi_phb_sort_compare_name_index_by_pinyin_cache(U16 first, U16 second,  U8 *same_count, S32 *distance);
extern void mmi_phb_sort_compare_name_index_by_pinyin(U16 first, U16 second,  U8 *same_count, S32 *distance);
#endif
extern U16 mmi_phb_sort_insert_name_index(U16 store_index);
extern U16 mmi_phb_sort_insert_name_index_find_pos(S32 low, S32 high, U16 store_index);

/*
* Normail Search Entry
*/
extern void mmi_phb_highlight_search_name(void);	/* Highlight handler for SEARCH ENTRY option */
extern void mmi_phb_highlight_search_name_option_search(void);
extern void mmi_phb_highlight_search_name_option_input_method(void);
extern void mmi_phb_search_name_pre_entry(void);	/* Entry screen for NAME */
extern void mmi_phb_entry_search_name(void);		/* Displays the editor to enter the NAME */
extern void mmi_phb_exit_search_name(void);
extern void mmi_phb_entry_search_name_option(void);
extern void mmi_phb_exit_search_name_option(void);
extern void mmi_phb_search_name(void);
extern U16 mmi_phb_search_name_find_closet(void);
extern U16 mmi_phb_search_name_binary_search(S32 low, S32 high, S8* ucs2_name);
extern U16 mmi_phb_search_name_code_search(S8* ucs2_name);
//void PhbSearchNameReq(void);		/* Invokes search operation */
//void PhbSearchNameRsp(void*);	/* Response from stack after SEARCH operation is completed */

/*
* Quick Search
*/
#if defined(__MMI_PHB_QUICK_SEARCH__)
extern void mmi_phb_highlight_quick_search_list(void);
extern void mmi_phb_quick_search_list_pre_entry(void);
extern void mmi_phb_entry_quick_search_list(void);
extern void mmi_phb_exit_quick_search_list(void);
extern pBOOL mmi_phb_quick_search_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask );
extern S32 mmi_phb_quick_search_list_get_hint ( S32 item_index, UI_string_type *hint_array );
extern void mmi_phb_quick_search_change_input_mode(void);

#if defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
extern void mmi_phb_quick_search_go_back_editor(void);
extern void mmi_phb_highlight_quick_search_input_method(void);
extern void mmi_phb_highlight_quick_search_op_option(void);
extern void mmi_phb_entry_quick_search_option(void);
extern void mmi_phb_exit_quick_search_option(void) ;
#endif

#endif

/*
*Compare Functions Utilities
*/
extern S32 mmi_phb_compare_chars(U16 char1, U16 char2);
extern void mmi_phb_compare_ucs2_strings(S8* string1, S8* string2, U8* same_count, S32* distance);
extern void mmi_phb_compare_ascii_strings(S8* string1, S8* string2, U16* same_count, S32* distance);
#if defined(__MMI_PHB_PINYIN_SORT__)
extern void mmi_phb_util_make_pinyin_cache(U16 store_index);
extern void mmi_phb_util_get_pinyin_cache(U16 store_index, U8** pinyin_ptr);
extern U16 mmi_phb_util_convert_to_pinyin(U8* inputString, U8* outputString, U16 out_length);
#endif

/* 
* Sort / Search Number Lookup Table
*/
extern void mmi_phb_lookup_table_sort(void);
extern void mmi_phb_lookup_table_quicksort(U16, U16);
extern void mmi_phb_lookup_table_insertionsort(U16, U16);
extern void mmi_phb_lookup_table_swap_node(U16, U16);
extern U16 mmi_phb_lookup_table_search(U32, S16, S16, S8*);
extern U16 mmi_phb_lookup_table_n_search(U32 number, S16 low, S16 high, S8* number_ASCII, S16 *pfirstindex);
extern U16 mmi_phb_number_has_extension(S8* number);
extern 	BOOL mmi_phb_number_compare_extension(S8* number1, S8* number2);
extern BOOL mmi_phb_number_compare_before_extension(S8 * number1, S8 * number2);
extern S16 mmi_phb_number_find_first_without_extension(S16 mid, U32 number, S8* number_in);
extern 	S16 mmi_phb_number_find_first_with_extension(S16 mid, U32 number, S8* number_in);

/*
*  email sorting
*/
#if defined(__MMI_PHB_EMAIL_SORT__)
extern void mmi_phb_sort_email_insert_index(U16 store_index);
extern U16 mmi_phb_sort_email_find_insert_pos(S32 low, S32 high, U16 store_index, U16* mapping_table);
extern void mmi_phb_sort_email_compare_by_encoding(U16 first, U16 second,  U8 *same_count, S32 *distance);
extern void mmi_phb_sort_email_delete_index(U16 store_index);
extern void mmi_phb_sort_email_delete_all_index(void);
extern U16 mmi_phb_search_email_binary_search(S32 low, S32 high, S8* ucs2_pattern);
#endif


#define IN_MMI_PHONEBOOKSETTING_C
/*
*  Settings Main Menu
*/
extern void mmi_phb_highlight_setting(void);		/* Highlight handler for Phonebook Settings option */
extern void mmi_phb_entry_setting(void);
extern void mmi_phb_exit_setting(void);

/*
*  Memory Status
*/
extern void mmi_phb_highlight_memory_status(void);
extern void mmi_phb_entry_memory_status(void);
extern void mmi_phb_exit_memory_status(void);
extern void mmi_phb_memory_status_get_sim(S8* ucs2_status, U8 nSimID);
extern void mmi_phb_memory_status_get_phone(S8* ucs2_status);

/*
*  Storage Location
*/
extern void mmi_phb_highlight_storage_location(void);
extern void mmi_phb_hint_storage_location(U16);
extern void mmi_phb_highlight_storage_location_in_sim(void);
extern void mmi_phb_highlight_storage_location_in_phone(void);
extern void mmi_phb_hint_storage_location_in_sim(U16);
extern void mmi_phb_hint_storage_location_in_phone(U16);
extern void mmi_phb_entry_storage_location(void);
extern void mmi_phb_exit_storage_location(void);
extern void mmi_phb_storage_location_set_req(void);

/*
*  View Fields
*/
extern void mmi_phb_highlight_view_field(void);
extern void mmi_phb_entry_view_field(void);
extern void mmi_phb_exit_view_field(void);
extern void mmi_phb_entry_view_field_save_confirm(void);
extern void mmi_phb_view_field_save(void);

#if defined(__MMI_VCARD__) && defined(__MMI_VCARD_V30__)
extern void mmi_phb_highlight_vcard_version(void);
extern void mmi_phb_hint_vcard_version(U16 index);
extern void mmi_phb_highlight_vcard_version_21(void);
extern void mmi_phb_highlight_vcard_version_30(void);
extern void mmi_phb_entry_vcard_version(void);
extern void mmi_phb_exit_vcard_version(void);
extern void mmi_phb_vcard_version_set_req(void);
#endif /* defined(__MMI_VCARD__)  && defined(__MMI_VCARD_V30__) */

/*
* My vCard
*/
extern void mmi_phb_highlight_vcard_send_select_card(void);
extern void mmi_phb_highlight_vcard_mycard(void);
extern void mmi_phb_highlight_vcard_edit_mycard(void);
extern void mmi_phb_highlight_vcard_send_mycard(void) ;
extern void mmi_phb_entry_mycard(void);
extern void mmi_phb_exit_mycard(void);
extern void mmi_phb_entry_edit_mycard(void);
extern void mmi_phb_exit_edit_mycard(void);
extern void mmi_phb_entry_edit_mycard_confirm_save(void) ;
extern void mmi_phb_vcard_save_mycard(void);

/*
* Speak Name in Phonebook list
*/
#if defined( __MMI_INTELLIGENT_CALL_ALERT__ )
extern void mmi_phb_init_speak_name(void);
extern void mmi_phb_highlight_speak_name(void) ;
extern void mmi_phb_hint_speak_name(U16 index);
extern void mmi_phb_speak_name_toggle(void);
#endif

/*
*   Forward Res To Phb Entry
*/

#if defined(__MMI_PHB_CALLER_RES_SETTING__)
extern void mmi_phb_highlight_caller_pic(void);
extern void mmi_phb_caller_pic_select_from_file_mamager(void);
extern void mmi_phb_caller_pic_select_from_file_mamager_done(void *path, int is_short);
extern void mmi_phb_highlight_caller_ring(void);
extern void mmi_phb_caller_ring_select_profile(void);
extern void mmi_phb_caller_ring_select_profile_done(void);
extern void mmi_phb_caller_ring_get_id_callback(U8 ExtMelodyFlag, U16 ringToneId);
extern void mmi_phb_caller_ring_forward_to_entry(U16 ringToneId);
#if defined(__MMI_INCOMING_CALL_VIDEO__)
extern void mmi_phb_highlight_caller_video(void);
extern void mmi_phb_caller_video_select_from_file_mamager(void);
extern void mmi_phb_caller_video_select_from_file_mamager_done(void *path, int is_short);
#endif
#endif

#define IN_MMI_PHONEBOOKCALLERGROUP_C
/*
*   Caller Group Settings
*/
extern void mmi_phb_highlight_callergroup(void);
extern void mmi_phb_entry_callergroup(void);
extern void mmi_phb_exit_callergroup(void);
extern void mmi_phb_entry_callergroup_detail(void);
extern void mmi_phb_exit_callergroup_detail(void);
extern void mmi_phb_callergroup_highlight_hdlr(S32);
extern void mmi_phb_callergroup_detail_pre_save(void);
extern void mmi_phb_entry_callergroup_detail_save_confirm(void);
extern void mmi_phb_callergroup_detail_save(void);

/* 
*  Caller Group Member List
*/
#if defined(__MMI_PHB_CALLERGROUP_MEMBER_LIST__)
extern void mmi_phb_highlight_callergroup_member_add(void);
extern void mmi_phb_highlight_callergroup_member_delete(void);
extern void mmi_phb_entry_callergroup_member_list(void);
extern void mmi_phb_exit_callergroup_member_list(void);
extern pBOOL mmi_phb_callergroup_member_list_get_item(S32, UI_string_type, UI_image_type*, U8);
extern S32 mmi_phb_callergroup_member_list_get_hint (S32, UI_string_type*);
extern void mmi_phb_entry_callergroup_member_option(void);
extern void mmi_phb_exit_callergroup_member_option(void) ;
extern void mmi_phb_entry_callergroup_member_add(void);
extern void mmi_phb_entry_callergroup_member_delete(void);
extern void mmi_phb_callergroup_member_pre_enter_list(void);
extern void mmi_phb_callergroup_member_pre_enter_add(void);
extern void mmi_phb_callergroup_member_delete_member(void);
extern void mmi_phb_callergroup_member_add_member(void);
extern U16 mmi_phb_callergroup_member_count_number(U8 list_enum);
extern void mmi_phb_callergroup_member_make_group_id_cache(void);
extern U16 mmi_phb_callergroup_member_get_index(U8 list_enum, U16 item_index);
#endif

/* 
*  Inline Editor Selection List - Ring Tone, Image List, LED, Alert.
*/
extern void mmi_phb_ring_tone_highlight_hdlr(S32);
extern void mmi_phb_play_ring_tone_id(void);
extern void mmi_phb_stop_ring_tone_id(void);
extern void mmi_phb_led_pattern_highlight_hdlr(S32);
extern void mmi_phb_play_led_pattern_id(void);
extern void mmi_phb_stop_led_pattern_id(void);
extern void mmi_phb_build_led_and_alert_list(void);
extern void mmi_phb_build_ring_tone_list(void);
extern void mmi_phb_build_image_list(void);
extern void mmi_phb_build_caller_group_list(void);
#if defined(__MMI_INCOMING_CALL_VIDEO__)
extern void mmi_phb_build_video_list(void) ;
extern U8 mmi_phb_get_video_index(U16 video_id);
#endif
extern void mmi_phb_build_black_list_list(void);
extern void mmi_phb_build_red_list_list(void);
extern U8 mmi_phb_get_ring_tone_index(U16);
extern U8 mmi_phb_get_image_index(U16);
extern U8 mmi_phb_get_black_list_index(U16 black_list_Id);
extern U8 mmi_phb_get_red_list_index(U16 black_list_Id);

#define IN_MMI_PHONEBOOKEXTRANUMBER_C
/*
*Extra Number Group, including Owner number, SOS number and Service Dial Number
*/
extern void mmi_phb_highlight_extra_number(void) ;
extern void mmi_phb_entry_extra_number(void);
extern void mmi_phb_exit_extra_number(void);
extern U8 GetUCS2ExtendedNum(PS8 buffer, U8 max_buffer_len);
/*
* Owner Number
*/
extern void mmi_phb_highlight_owner_number(void);
extern void mmi_phb_highlight_owner_number_erase(void);
extern void mmi_phb_highlight_owner_number_edit(void);
extern void mmi_phb_owner_number_pre_req(void);
extern void mmi_phb_owner_number_get_entry_req(void);
extern void mmi_phb_owner_number_get_entry_rsp(void*);
extern void mmi_phb_get_disp_owner_num();
extern void mmi_phb_idle_disp_owner_number_get_entry_req(U8 nSimID) ;
extern void mmi_phb_idle_disp_owner_number_get_entry_rsp(void* info) ;
extern void mmi_phb_entry_owner_number(void);
extern void mmi_phb_exit_owner_number(void);
extern void mmi_phb_entry_onwer_number_option(void);
extern void mmi_phb_exit_onwer_number_option(void);
extern void mmi_phb_entry_owner_number_erase_confirm(void);
extern void mmi_phb_owner_number_pre_erase(void);
extern void mmi_phb_owner_number_erase_req(void);
extern void mmi_phb_owner_number_erase_rsp(void*);
extern void mmi_phb_owner_number_pre_edit(void);
extern void mmi_phb_entry_owner_number_edit(void);
extern void mmi_phb_exit_owner_number_edit(void);
extern void mmi_phb_entry_owner_number_edit_confirm(void);
extern void mmi_phb_owner_number_edit_req(void);
extern void mmi_phb_owner_number_edit_rsp(void*);

/*
* Service Dial Number List
*/
extern void mmi_phb_highlight_sdn_list(void);
extern void mmi_phb_sdn_list_pre_entry(void);
extern void mmi_phb_sdn_get_entry_req(void);
extern void mmi_phb_sdn_get_entry_rsp(void *);
extern void mmi_phb_entry_sdn_list(void);
extern void mmi_phb_exit_sdn_list(void);
extern void mmi_phb_sdn_list_make_call(void) ;

/*
* Emergency(SOS) Number
*/
extern void mmi_phb_highlight_sos_list(void);
extern void mmi_phb_sos_list_pre_entry(void);
extern void mmi_phb_sos_list_get_entry_req(void);
extern void mmi_phb_sos_list_get_entry_rsp(void *);
extern void mmi_phb_entry_sos_list(void);
extern void mmi_phb_exit_sos_list(void);
extern void mmi_phb_sos_list_get_index(S32 );
#if defined(__MMI_PHB_SOS_NUM_EDIT__)
extern void mmi_phb_sos_list_pre_entry_number(void);
extern void mmi_phb_entry_sos_list_enter_number(void);
extern void mmi_phb_exit_sos_list_enter_number(void);
extern void mmi_phb_sos_list_update_entry(void);
#endif
extern void mmi_phb_sos_list_pre_confirm_call(void);
extern void mmi_phb_entry_sos_list_confirm_call(void);
extern void mmi_phb_sos_list_make_call(void);

#define MMI_PHB_FDN_SEARCH_LIST
extern void mmi_phb_fdn_get_list_to_ram_req(void);
extern void mmi_phb_fdn_get_list_to_ram_rsp(void *info);
extern U16 mmi_phb_fdn_search_by_number(S8* number);

#define IN_MMI_PHONEBOOKSTUBS_C
/*
* Interface for MMS
*/
#if 1//defined(MMS_SUPPORT)//fengzb 2009-3-3
extern void mmi_phb_entry_mms_choose_number(void);
extern void mmi_phb_exit_mms_choose_number(void);
extern void mmi_phb_mms_pre_send_data_to_mms(void);
extern void mmi_phb_mms_send_data_to_mms(S8*, S8*);
extern pBOOL mmi_phb_mms_email_list_get_item(S32,UI_string_type,UI_image_type*,U8);
extern S32 mmi_phb_mms_email_list_get_hint ( S32 item_index, UI_string_type *hint_array );
#endif

/* 
* Save Image To Phb from File Manager
*/
#if defined(__MMI_FILE_MANAGER__)
extern void mmi_phb_entry_fmgr_save_option(void);
extern void mmi_phb_exit_fmgr_save_option(void);
extern void mmi_phb_highlight_fmgr_add_entry(void);
extern void mmi_phb_highlight_fmgr_edit_entry(void);
extern void mmi_phb_highlight_edit_callergroup(void);
extern void mmi_phb_fmgr_pre_add_entry(void);
extern void mmi_phb_fmgr_pre_edit_entry(void);
extern pBOOL mmi_phb_fmgr_edit_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask );
extern S32 mmi_phb_fmgr_edit_list_get_hint ( S32 item_index, UI_string_type *hint_array );
extern void mmi_phb_fmgr_pre_edit_callergrp(void);
#endif

extern void mmi_phb_number_remove_invalid_character(S8 *num);

/*
* Pubic Utilities- Convert Functions
*/
extern U8 mmi_phb_convert_to_bcd_get_byte (U8);
extern U8 mmi_phb_convert_to_bcd (U8*, U8*,  U8);
extern U8 mmi_phb_convert_to_digit_get_byte (U8);
extern U8 mmi_phb_convert_to_digit (U8*, U8*, U8 );
extern void mmi_phb_convert_get_ucs2_number(S8* ,U16);
extern U8 mmi_phb_convert_to_0x81(S8* input_name, BOOL is_convert);
extern U16 mmi_phb_convert_to_0x81_get_base(U16 code);
extern U8 mmi_phb_convert_to_0x81_get_offset(U16 base, U16 code);

extern void mmi_phb_list_enter_from_active_call(U16 highlight_pos);

extern U16 mmi_phb_get_email_address_id(U16 index);

#endif /* _PHONEBOOKPROT_H */


