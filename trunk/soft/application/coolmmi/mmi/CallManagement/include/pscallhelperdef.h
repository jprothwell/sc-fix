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
 *  PsCallHelperDef.h
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: PsCallHelperDef.h

  	PURPOSE		: Call Management application

 

	 

	DATE		: Nov 16,02

**************************************************************/
#ifndef _PIXCOM_PSCALLHELPER_DEF_H
#define _PIXCOM_PSCALLHELPER_DEF_H

//using MMI specific coding standards
#define MMI_CC_CHLD_REQ						mmi_cc_chld_req_struct
#define MMI_CHLD_RSP							mmi_cc_chld_rsp_struct
#define MMI_DTMF_DIGIT						mmi_cc_start_dtmf_req_struct
#define MMI_INCOMING							mmi_cc_call_ring_ind_struct
#define MMI_SPEECH_IND						mmi_cc_speech_ind_struct
#define MMI_SPEECH_MODE_REQ				mmi_eq_set_speech_mode_req_struct
#define MMI_CALL_LIST_RSP					mmi_cc_get_call_list_rsp_struct
#define MMI_CALL_LIST						l4c_call_list_struct
#define MMI_CALL_LIST_ENTRY				l4c_call_list_entry_struct
#define MMI_RELEASE_IND						mmi_cc_call_release_ind_struct
#define MMI_CCM_STRUCT						mmi_cc_ccm_ind_struct
#define MMI_ATH_REQ							mmi_cc_ath_req_struct
 
#define MMI_RELCOMP_REQ					mmi_cc_rel_comp_req_struct
 
#define MMI_ATA_RSP							mmi_cc_ata_rsp_struct
#define MMI_PARSE_STR						mmi_ss_parsing_string_req_struct
#define MMI_PARSE_STR_RSP					mmi_ss_parsing_string_rsp_struct
#define MMI_CALL_WAIT						mmi_cc_call_wait_ind_struct
#define MMI_DIAL_IND							mmi_cc_dial_ind_struct
#define MMI_NOTIFY_SS						mmi_cc_notify_ss_ind_struct
#define MMI_CC_INTERNAL_ALERT_IND		mmi_cc_internal_alert_ind_struct
#define MMI_SS_USSN_IND_STRUCT			mmi_ss_ussn_ind_struct
#define MMI_SS_USSR_IND_STRUCT			mmi_ss_ussr_ind_struct
#define MMI_CALL_DEFLECT_REQ				mmi_cc_call_deflect_req_struct
#define MMI_CALL_DEFLECT_RSP				mmi_cc_call_deflect_rsp_struct
#define MMI_SS_USSD_RSP						mmi_ss_ussd_rsp_struct
#define MMI_SS_ABORT_RSP					mmi_ss_abort_rsp_struct
#define MMI_CONNECT_IND						mmi_cc_call_connect_ind_struct
#define MMI_CC_CALL_DISCONNECT_IND		mmi_cc_call_disconnect_ind_struct
#define MMI_DTMF_CONFIRM_IND				mmi_cc_start_auto_dtmf_ind_struct
#define MMI_DTMF_CONFIRM_REQ				mmi_cc_start_auto_dtmf_req_struct


#endif //_PIXCOM_PSCALLHELPER_DEF_H

