/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
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
 * JProfileHandlerProts.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This declares the function prototypes for the profile handler module
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef _JPROFILEHANDLERPROTS_H
#define _JPROFILEHANDLERPROTS_H

#include "mmi_features.h"
#include "mmi_data_types.h"

#if defined (JATAAYU_SUPPORT)
/* init ,deinit API's */
extern void mmi_ph_init(void);
extern void mmi_ph_deinit_profiles(void);

/* nvram related functions */
extern void mmi_ph_get_brw_profile_name_list(void);
extern void mmi_ph_get_mms_profile_name_list(void);

extern void mmi_ph_get_selected_profile_content_data(U8 g_profile_index);
extern void mmi_ph_get_profile_nvram_content_data(U8 index);

extern void mmi_ph_update_nvram_wap_profile(void);
extern void mmi_ph_update_nvram_mms_profile(void);

extern void mmi_ph_update_nvram_activated_wap_profile(void);

/* provisioing related API */
extern void mmi_ph_update_prov_nvram_wap_profile(void);
extern void mmi_ph_update_prov_nvram_mms_profile(void);

extern U8 mmi_ph_get_activated_profile_index(void);

/* general profile edit related API's */
extern void mmi_ph_activate_profile_highlight_hdlr(void);
extern void mmi_ph_edit_profile_highlight_hdlr(void);
extern void mmi_ph_pre_entry_edit_selected_profile(void);
extern void mmi_ph_pre_entry_show_profile_list_menu(ph_profile_enum profile);
extern void mmi_ph_entry_show_profile_list_menu(void);
extern void mmi_ph_entry_select_editable_profile(void);
extern void mmi_ph_entry_edit_selected_profile(void);
extern void mmi_ph_activate_selected_profile(void);
extern void mmi_ph_entry_activate_selected_profile(void);
extern void mmi_ph_selected_profile_fill_inline_struct(void);
extern void mmi_ph_save_selected_profile_changes(void);
extern void mmi_ph_editable_profile_save_changes(void);
extern void mmi_ph_editable_profile_full_screen_edit(void);
extern void mmi_ph_highlight_select_profile(S32 nIndex);
extern void mmi_ph_exit_edit_selected_profile(void);
extern void mmi_ph_editable_profile_reject_changes(void);

extern void mmi_ph_highlight_inline_item(S32 index);

/* connection type related API */
extern void mmi_ph_show_connection_type_options(void);
extern void mmi_ph_entry_edit_connection_type_details(void);
extern void mmi_ph_exit_edit_connection_type_details(void);
extern void mmi_ph_conn_type_highlight_hdlr(S32 index);
extern void mmi_ph_highlight_inline_item_conn_type(S32 index);
extern void mmi_ph_edit_conn_type_fill_inline_struct(void);
extern void mmi_ph_save_connection_type_changes(void);

/* data account related functions */
extern void mmi_ph_handle_get_data_account_name(void);
extern void mmi_ph_prof_callback(U16 index);
extern void mmi_ph_prof_list_data_acct(void);

extern void mmi_ph_get_csd_account_name_req(U8 ItemNum);
extern void mmi_ph_get_csd_account_name_rsp(void *info);
extern void mmi_ph_get_gprs_account_name_req(U8 ItemNum);
extern void mmi_ph_get_gprs_account_name_rsp(void *info);

extern void mmi_ph_change_data_account_name(void);

extern ph_data_account_enum mmi_ph_check_data_account_type(U8 accountIndex);

extern U8 mmi_ph_get_highlighted_menu_item(void);

/* general interfaces */

extern nvram_wap_profile_content_struct *mmi_ph_get_activated_wap_profile(void);
extern nvram_mms_profile_content_struct *mmi_ph_get_activated_mms_profile(void);

/* interface for browser */
extern void mmi_ph_get_activated_profile_data(ph_profile_enum profile, U8 index);
#ifdef __MMI_MULTI_SIM__
extern void 	MTPNP_mmi_ph_get_activated_profile_data(ph_profile_enum profile, U8 index, U8 simid);
#endif
extern U8 *mmi_ph_brw_get_activated_profile_homepage(void);
extern void mmi_ph_brw_set_activated_profile_homepage(U8 *homepage_url);

/* interface for MMS */
extern U8 *mmi_ph_mms_get_activated_profile_mmsc_url(void);
extern void mmi_ph_update_nvram_activated_mms_profile(void);

extern void mmi_ph_get_profile_name_list(void);
#if 0
//interface for java task
//void mmi_ph_get_prof_list_req_hdlr(void);
//void mmi_ph_get_prof_content_req_hdlr(void* msg);
extern void mmi_ph_get_prof_list_req_hdlr(module_type req_src_id);
extern void mmi_ph_get_prof_content_req_hdlr(module_type req_src_id, void *msg);
extern void mmi_ph_get_activated_prof_id_req_hdlr(module_type req_src_id);
#endif

void jmms_profileSettingsEndKeyHandler();
void jwap_profileSettingsEndKeyHandler();

#endif /* defined (JATAAYU_SUPPORT) */ // #if defined (JATAAYU_SUPPORT)

#endif /* _JPROFILEHANDLERPROTS_H */ 

