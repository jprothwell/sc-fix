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
 *	JavaProt.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for define prototypes of Java related routines.
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef _JAVA_PROT_H
#define _JAVA_PROT_H

#include "mmi_features.h" 

#ifdef __J2ME__

#include "kal_non_specific_general_types.h"

/*****************************************************************************
**	0. JAVA_BASIC feature:
**	1.JAVA_CONTENT_MANAGER feature:
**	1.1	JAVA_CM_BROWSER
**	1.2	JAVA_CM_LAUNCH
**	1.3	JAVA_CM_UPDATE
**	1.4	JAVA_CM_SETTING
**	1.5	JAVA_CM_INFORMATION
**	1.6	JAVA_CM_DELETE
**	3. JAVA_TCK
**	4. JAVA_INSTALL_UPDATE
**	5. JAVA_TIMEALARM_PUSH
**	6. JAVA_ABORT
*****************************************************************************/

/*****************************************************************************
**	0. JAVA_BASIC feature: (5)
**		InitJAVA
**		Highlight_JAVA_MENU
**		EntryScrJavaMenu
**		ExitScrJavaMenu
**		HighlightJavaEM
*****************************************************************************/

void InitJAVA(void);

extern void mmi_java_highlight_menu(void);
extern void mmi_fng_highlight_setting_java(void);

void EntryScrJavaMenu(void);
void ExitScrJavaMenu(void);

void HighlightJavaEM(void);

/*****************************************************************************
**	1.JAVA_CONTENT_MANAGER feature: (22)
**	1.1	JAVA_CM_BROWSER
**		mmi_java_get_mids_index
**		mmi_java_entry_mids_n_option
**		mmi_java_exit_mids_n_option
**		mmi_java_entry_mids_1_option
**		mmi_java_exit_mids_1_option
**		mmi_java_highlight_opt_select
**		mmi_java_highlight_opt_launch
**		mmi_java_highlight_opt_update
**		mmi_java_highlight_opt_setting
**		mmi_java_highlight_opt_info
**		mmi_java_highlight_opt_delete
**		mmi_java_send_mids_n_getlist_req
**		mmi_java_recv_mids_n_getlist_cnf
**		mmi_java_send_mids_1_getlist_req
**		mmi_java_recv_mids_1_getlist_cnf
**		mmi_java_get_mid_index
**		mmi_java_entry_opt_select
**		mmi_java_exit_opt_select
**		mmi_java_entry_opt_update
**		mmi_java_exit_opt_update
**		mmi_java_entry_opt_setting
**		mmi_java_exit_opt_setting
*****************************************************************************/

void mmi_java_get_mids_index(S32 nIndex);
void mmi_java_entry_mids_n_option(void);
void mmi_java_exit_mids_n_option(void);
void mmi_java_entry_mids_1_option(void);
void mmi_java_exit_mids_1_option(void);

void mmi_java_highlight_opt_select(void);
void mmi_java_highlight_opt_launch(void);
void mmi_java_highlight_opt_update(void);
void mmi_java_highlight_opt_setting(void);
void mmi_java_highlight_opt_info(void);
void mmi_java_highlight_opt_delete(void);

void mmi_java_send_mids_n_getlist_req(void);
void mmi_java_recv_mids_n_getlist_cnf(void* MsgStruct);
void mmi_java_send_mids_1_getlist_req(void);
void mmi_java_recv_mids_1_getlist_cnf(void* MsgStruct);

void mmi_java_get_mid_index(S32 nIndex);

void mmi_java_entry_opt_select(void);
void mmi_java_exit_opt_select(void);

void mmi_java_entry_opt_update(void);
void mmi_java_exit_opt_update(void);

void mmi_java_entry_opt_setting(void);
void mmi_java_exit_opt_setting(void);

/*****************************************************************************
**	1.JAVA_CONTENT_MANAGER feature: (3)
**	1.2	JAVA_CM_LAUNCH
**		mmi_java_send_mid_start_req
**		mmi_java_recv_mid_start_cnf
**		mmi_java_recv_mid_stop_ind
*****************************************************************************/

void mmi_java_send_mid_start_req(void);
void mmi_java_recv_mid_start_cnf(void* MsgStruct);
void mmi_java_recv_mid_stop_ind(void* MsgStruct);

/*****************************************************************************
**	1.JAVA_CONTENT_MANAGER feature: (4)
**	1.5	JAVA_CM_INFORMATION
**		mmi_java_send_mids_getinfo_req
**		mmi_java_recv_mids_getinfo_cnf
**		mmi_java_entry_opt_info
**		mmi_java_exit_opt_info
*****************************************************************************/

void mmi_java_send_mids_getinfo_req(void);
void mmi_java_recv_mids_getinfo_cnf(void* MsgStruct);
void mmi_java_entry_opt_info(void);
void mmi_java_exit_opt_info(void);

/*****************************************************************************
**	1.JAVA_CONTENT_MANAGER feature: (23)
**	1.4	JAVA_CM_SETTING
**		mmi_java_highlight_network_access
**		mmi_java_highlight_auto_invocation
**		mmi_java_highlight_messaging
**		mmi_java_highlight_recording
**		mmi_java_execute_opt_setting
**		mmi_java_send_mids_get_setting_req
**		mmi_java_recv_mids_get_setting_cnf
**		mmi_java_send_mids_set_setting_req
**		mmi_java_recv_mids_set_setting_cnf
**		mmi_java_set_mids_choice_index
**		mmi_java_get_choice_level
**		mmi_java_get_new_permission_level
**		mmi_java_update_mids_setting
**		mmi_java_entry_opt_setting
**		mmi_java_exit_opt_setting
**		mmi_java_entry_network_access
**		mmi_java_exit_network_access
**		mmi_java_entry_auto_invocation
**		mmi_java_exit_auto_invocation
**		mmi_java_entry_messaging
**		mmi_java_exit_messaging
**		mmi_java_entry_recording
**		mmi_java_exit_recording
*****************************************************************************/

void mmi_java_highlight_network_access(void);
void mmi_java_highlight_auto_invocation(void);
void mmi_java_highlight_messaging(void);
void mmi_java_highlight_recording(void);

void mmi_java_execute_opt_setting (void);
void mmi_java_send_mids_get_setting_req(kal_uint32 session_id,kal_uint32 mids_id);
void mmi_java_recv_mids_get_setting_cnf(void* MsgStruct);
void mmi_java_send_mids_set_setting_req(void);
void mmi_java_recv_mids_set_setting_cnf(void* MsgStruct);

void mmi_java_set_mids_choice_index(S32 nIndex);
S8 mmi_java_get_choice_level (S8 permisisonLevel);
S8 mmi_java_get_new_permission_level (S8 choiceLevel, S8 maxLevel, S8 current);
void mmi_java_update_mids_setting(void);
void mmi_java_entry_opt_setting(void);
void mmi_java_exit_opt_setting(void);
void mmi_java_entry_network_access(void);
void mmi_java_exit_network_access(void);
void mmi_java_entry_auto_invocation(void);
void mmi_java_exit_auto_invocation(void);
void mmi_java_entry_messaging(void);
void mmi_java_exit_messaging(void);
void mmi_java_entry_recording(void);
void mmi_java_exit_recording(void);

/*****************************************************************************
**	1.JAVA_CONTENT_MANAGER feature: (13)
**	1.6	JAVA_CM_DELETE
**		mmi_java_execute_delete
**		mmi_java_enter_mids_delete_requesting
**		mmi_java_exit_mids_delete_requesting
**		mmi_java_send_mids_delete_req
**		mmi_java_recv_mids_delete_confirm_ind
**		mmi_java_enter_mids_delete_confirm_ind
**		mmi_java_exit_mids_delete_confirm_ind
**		mmi_java_enter_mids_delete_confirmed
**		mmi_java_exit_mids_delete_confirmed
**		mmi_java_send_mids_delete_confirm_res_yes
**		mmi_java_send_mids_delete_confirm_res_no
**		mmi_java_send_mids_delete_confirm_res
**		mmi_java_recv_mids_delete_cnf
*****************************************************************************/

void mmi_java_execute_delete(void);
void mmi_java_enter_mids_delete_requesting(void);
void mmi_java_exit_mids_delete_requesting(void);
void mmi_java_send_mids_delete_req(void);
void mmi_java_recv_mids_delete_confirm_ind(void* MsgStruct);
void mmi_java_enter_mids_delete_confirm_ind(void);
void mmi_java_exit_mids_delete_confirm_ind(void);
void mmi_java_enter_mids_delete_confirmed(void);
void mmi_java_exit_mids_delete_confirmed(void);
void mmi_java_send_mids_delete_confirm_res_yes(void);
void mmi_java_send_mids_delete_confirm_res_no(void);
void mmi_java_send_mids_delete_confirm_res(kal_bool confirm);
void mmi_java_recv_mids_delete_cnf(void* MsgStruct);

/*****************************************************************************
**	3. JAVA_TCK: (14)
**		mmi_java_highlight_em_normal
**		mmi_java_highlight_em_cldc_tck
**		mmi_java_highlight_em_midp_tck
**		mmi_java_highlight_em_wma_tck
**		mmi_java_highlight_em_mmapi_tck
**		mmi_java_execute_set_mode
**		mmi_java_send_set_mode_req
**		mmi_java_recv_set_mode_cnf
**		mmi_java_terminate_tck_mode
**		mmi_java_execute_em_tck
**		EntryScrJavaEM
**		ExitScrJavaEM
**		mmi_java_entry_java_em_tck
**		mmi_java_exit_java_em_tck
*****************************************************************************/

void mmi_java_highlight_em_normal(void);
void mmi_java_highlight_em_cldc_tck(void);
void mmi_java_highlight_em_midp_tck(void);
void mmi_java_highlight_em_wma_tck(void);
void mmi_java_highlight_em_mmapi_tck(void);

void mmi_java_execute_set_mode(void);
void mmi_java_send_set_mode_req(void);
void mmi_java_recv_set_mode_cnf(void);

void mmi_java_terminate_tck_mode(void);
void mmi_java_execute_em_tck(void);

void EntryScrJavaEM(void);
void ExitScrJavaEM(void);

void mmi_java_entry_java_em_tck(void);
void mmi_java_exit_java_em_tck(void);

/*****************************************************************************
**	4. JAVA_INSTALL_UPDATE: (16)
**		mmi_java_recv_mids_install_ind
**		mmi_java_enter_mids_start_installing
**		mmi_java_exit_mids_start_installing
**		mmi_java_recv_mids_install_confirm_ind
**		mmi_java_enter_mids_install_confirm_ind
**		mmi_java_exit_mids_install_confirm_ind
**		mmi_java_send_mids_install_confirm_res_yes
**		mmi_java_send_mids_install_confirm_res_no
**		mmi_java_send_mids_install_confirm_res
**		mmi_java_recv_mids_install_cnf
**		mmi_java_recv_reset_rms_ind
**		mmi_java_entry_reset_rms_res
**		mmi_java_exit_reset_rms_res
**		mmi_java_send_reset_rms_res_yes
**		mmi_java_send_reset_rms_res_no
**		mmi_java_send_reset_rms_res
*****************************************************************************/

void mmi_java_recv_mids_install_ind(void* MsgStruct);
void mmi_java_enter_mids_start_installing(void);
void mmi_java_exit_mids_start_installing(void);

void mmi_java_recv_mids_install_confirm_ind(void* MsgStruct);
void mmi_java_enter_mids_install_confirm_ind(void);
void mmi_java_exit_mids_install_confirm_ind(void);

void mmi_java_send_mids_install_confirm_res_yes(void);
void mmi_java_send_mids_install_confirm_res_no(void);

void mmi_java_send_mids_install_confirm_res(kal_bool confirm);
void mmi_java_recv_mids_install_cnf(void* MsgStruct);

void mmi_java_recv_reset_rms_ind(void* MsgStruct);
void mmi_java_entry_reset_rms_res(void);
void mmi_java_exit_reset_rms_res(void);
void mmi_java_send_reset_rms_res_yes(void);
void mmi_java_send_reset_rms_res_no(void);
void mmi_java_send_reset_rms_res(S8 confirm);

/*****************************************************************************
**	1.JAVA_CONTENT_MANAGER feature: (15)
**	1.3	JAVA_CM_UPDATE
**		mmi_java_execute_update
**		mmi_java_enter_mids_update_req_asking
**		mmi_java_exit_mids_update_req_asking
**		mmi_java_send_mids_update_req_yes
**		mmi_java_send_mids_update_req_no
**		mmi_java_enter_mids_start_updating
**		mmi_java_exit_mids_start_updating
**		mmi_java_send_mids_update_req
**		mmi_java_recv_mids_update_cnf
**		mmi_java_recv_java_get_file_and_save_ind
**		mmi_java_entry_mids_vm_installing
**		mmi_java_exit_mids_vm_installing
**		mmi_java_send_java_get_file_and_sav_res
**		mmi_java_j2me_show_alert_user
**		mmi_java_j2me_play_tone
*****************************************************************************/

void mmi_java_execute_update(void);
void mmi_java_enter_mids_update_req_asking(void);
void mmi_java_exit_mids_update_req_asking(void);
void mmi_java_send_mids_update_req_yes(void);
void mmi_java_send_mids_update_req_no(void);
void mmi_java_enter_mids_start_updating(void);
void mmi_java_exit_mids_start_updating(void);
void mmi_java_send_mids_update_req(void);
void mmi_java_recv_mids_update_cnf(void* MsgStruct);

void mmi_java_recv_java_get_file_and_save_ind(void* MsgStruct);
void mmi_java_entry_mids_vm_installing(void);
void mmi_java_exit_mids_vm_installing(void);
void mmi_java_send_java_get_file_and_sav_res(void);

void mmi_java_j2me_show_alert_user(void* MsgStruct);
void mmi_java_j2me_play_tone(void *MsgStruct);

/*****************************************************************************
**	5. JAVA_TIMEALARM_PUSH (11)
**		mmi_java_recv_timealarm_push_ind
**		mmi_java_recv_timealarm_push_get_setting_cnf
**		mmi_java_send_timealarm_push_res
**		mmi_java_entry_timealarm_push_res
**		mmi_java_exit_timealarm_push_res
**		mmi_java_send_timealarm_push_res_yes
**		mmi_java_send_timealarm_push_res_no
**		mmi_java_entry_timealarm_push_launch
**		mmi_java_exit_timealarm_push_launch
**		mmi_java_process_timealarm_event
**		j2me_launch_time_alarm
*****************************************************************************/

void mmi_java_recv_timealarm_push_ind(void* MsgStruct);
void mmi_java_recv_timealarm_push_get_setting_cnf(void* MsgStruct);
void mmi_java_send_timealarm_push_res(kal_uint32 session_id,S8 permission, U8 execute);
void mmi_java_entry_timealarm_push_res(void);
void mmi_java_exit_timealarm_push_res(void);
void mmi_java_send_timealarm_push_res_yes(void);
void mmi_java_send_timealarm_push_res_no(void);
void mmi_java_entry_timealarm_push_launch(void);
void mmi_java_exit_timealarm_push_launch(void);
void mmi_java_process_timealarm_event(void *arg);
void j2me_launch_time_alarm(void);

/*****************************************************************************
**	6. JAVA_ABORT (6)
**		j2me_content_routing
**		mmi_java_recv_wap_abort_cnf
**		mmi_java_go_back_send_java_abort_req
**		mmi_java_send_java_abort_req
**		mmi_java_recv_java_abort_cnf
*****************************************************************************/

void j2me_content_routing(kal_uint8* file_path);
void mmi_java_recv_wap_abort_cnf(void* MsgStruct);
void mmi_java_go_back_send_java_abort_req(void);
void mmi_java_send_java_abort_req(void);
void mmi_java_recv_java_abort_cnf(void* MsgStruct);
void mmi_java_entry_launch_after_install_confirm(void);
void mmi_java_entry_launch_after_install_confirm_yes(void);
void mmi_java_entry_launch_after_install_confirm_no(void);
void mmi_java_launch_after_install_callback(void);
extern void mmi_java_local_install_content_routing(kal_uint8 *file_path);

/*****************************************************************************
**
**   User Permission dialog
*****************************************************************************/

extern void mmi_java_entry_get_permission(void);
extern void mmi_java_recv_get_permission_ind(void *MsgStruct);
extern kal_bool jam_usb_mode_check(void);
extern void jam_enter_usb_mode(void);
extern void j2me_send_alertuser(char *message, int len);
extern void mmi_java_recv_show_alert_user_req(void *MsgStruct);
extern void jvm_start_java_req(void);
extern void jvm_exit_java_req(void);
extern void jvm_launch_url_req(char* urlString);
extern void mmi_java_send_message(S32 src_id, S32 dst_id, S32 sap_id, S32 msg_id, void *local_para_ptr, void *peer_buff_ptr);

typedef struct
{
    PS8 system_folder_path;
    S32 last_error;
}java_context_struct;

#endif /* !__J2ME__ */
#endif /* !_JAVA_PROT_H */


