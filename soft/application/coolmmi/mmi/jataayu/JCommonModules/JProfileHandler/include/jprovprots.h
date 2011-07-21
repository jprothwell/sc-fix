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
 * JProvProts.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file declares the function prototypes for OTA client provisioning.
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

#ifndef _JPROVPROTS_H
#define _JPROVPROTS_H

#include "mmi_features.h"

#if defined (JATAAYU_SUPPORT)

#include <ddl.h>
#include <jcal.h>
#include <jdi_xml.h>
#include <jdi_wbxml.h>
#include <xmltag.h>
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "custom_nvram_editor_data_item.h"
#include "ucs2prot.h"
#include "keybrd.h"
#include "idleappprot.h"
#include "commonscreens.h"
#include "wgui_categories_popup.h"
#include "globaldefs.h"
#include "protocolevents.h"
#include "settingprofile.h"
#include "wap_ps_struct.h"
#include "l4a.h"

extern void mmi_prov_deinit(void);

extern void mmi_prov_handle_ok(void);
extern void mmi_prov_handle_cancel(void);
extern void mmi_prov_handle_back(void);
extern void mmi_prov_handle_accept(void);
extern void mmi_prov_handle_terminate(void);

extern void mmi_prov_handle_error_state(void);
extern void mmi_prov_handle_error_state_callback(void);
extern void mmi_prov_get_dataaccount_data(void);
extern void mmi_prov_get_gprs_account_req(U8 ItemNum);
extern void mmi_prov_get_gprs_account_rsp(void *info);
extern void mmi_prov_get_csd_account_req(U8 ItemNum);
extern void mmi_prov_get_csd_account_rsp(void *info);

extern void mmi_prov_delete_queue_entry(void);
extern void mmi_prov_add_to_queue(S8 *provBuffer, U32 provLength);
extern void mmi_prov_confirm_abort_setting_set_up(void);
extern void mmi_prov_show_install_settings_dialog(void);
extern void mmi_prov_receive_da_file(
                S32 session_id,
                S32 mime_type,
                S32 mime_subtype,
                S32 action_type,
                PU16 filename,
                PS8 download_url,
                PS8 mime_type_str_string);
extern kal_int32 mmi_prov_handle_provisioning_data(kal_int8 *contentType, kal_wchar *filePath);
extern void mmi_prov_handle_provisioning_data_ext(S8 *provBuffer, U32 provLength, S8 *contentType);
extern void mmi_prov_update_NVRAM_data(void);

extern void mmi_prov_show_profile_list_menu(void);
extern void mmi_prov_handle_get_next_prov_data(void);

extern U8 mmi_prov_check_and_install_new_settings(void);
extern void mmi_prov_show_new_settings_popup(void);

extern S32 mmi_prov_check_if_substring(const S8 *string, const S8 *sub_string);
extern U8 mmi_prov_convert_ip_string_to_array(S8 *characteristic_element, U8 *proxy_ip);

/* data account related API's */
extern void mmi_prov_entry_csd_data_acct(void);
extern void mmi_prov_entry_gprs_data_acct(void);
extern void mmi_prov_prof_save_gprs_data_acct(void);
extern void mmi_prov_prof_save_csd_data_acct(void);
extern void mmi_prov_set_csd_req(void);
extern void mmi_prov_set_csd_rsp(void *info);
extern void mmi_prov_set_gprs_req(void);
extern void mmi_prov_set_gprs_rsp(void *info);
extern void mmi_prov_hilite_data_account(S32 index);

extern void mmi_prov_wap_get_profile_list_req(void);
extern void mmi_prov_wap_get_profile_list_rsp(void *);

 
extern void mmi_prov_handle_proxy_ip(ATTRIBUTE *pAttr);
extern void mmi_prov_handle_nap_auth_type(ATTRIBUTE *pAttr);
extern void mmi_prov_handle_nap_auth_name(ATTRIBUTE *pAttr);
extern void mmi_prov_handle_nap_auth_pw(ATTRIBUTE *pAttr);
extern void mmi_prov_handle_profile_name(ATTRIBUTE *pAttr);
extern void mmi_prov_handle_homepage_url(ATTRIBUTE *pAttr);
extern void mmi_prov_handle_mmsc_url(ATTRIBUTE *attr_p);

extern void mmi_prov_handle_update_nvram_wap_profile(void);
extern void mmi_prov_handle_update_nvram_mms_profile(void);

extern void mmi_prov_handle_data_account_mem_allocate(void);
extern void mmi_prov_handle_data_account_mem_free(void);
extern void mmi_prov_handle_profile_mem_allocate(void);
extern void mmi_prov_handle_profile_mem_free(void);
extern void mmi_prov_entry_show_new_settings_popup(void);
extern void mmi_prov_set_current_instal_stringID(void);

extern void mmi_prov_handle_data_account_set_rsp(U8 dataAccountType);

extern void mmi_prov_get_next_appl_or_new_doc_callback(void);

extern void mmi_prov_handle_error_code(U16 error_code);

/* Function to check null for input parameter */
extern MMI_BOOL mmi_prov_check_not_null(void *inp_p) ; 

#endif /* defined (JATAAYU_SUPPORT) */ // #if defined (JATAAYU_SUPPORT)
#endif /* _JPROVPROTS_H */ 

