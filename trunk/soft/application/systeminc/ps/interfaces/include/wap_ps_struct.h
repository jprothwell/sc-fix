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
 *	wap_ps_struct.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for local parameter declaration about WAP sap
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

/********************************************************************************
*  Copyright Statement:
*  --------------------
*
*  This product has been developed using a protocol stack
*  developed by Sasken Communication Technologies Limited.
*
********************************************************************************/

#ifndef _WAP_PS_STRUCT_H
#define _WAP_PS_STRUCT_H 

#define WPS_MAX_AUTH_USERNAME_LENGTH	128
#define WPS_MAX_AUTH_PASSWORD_LENGTH	30

typedef enum
{
   WAP_PROF_EWIL, /* WAP Image Load */
	WAP_PROF_EWHP, /* WAP Homepage */
	WAP_PROF_EWPR, /* WAP Profiles */
	WAP_PROF_EWPN, /* WAP Profile Name */
	WAP_PROF_EWDT, /* WAP Download Timeout */
	WAP_PROF_EWLI, /* WAP Login */
	WAP_PROF_EWPB, /* WAP Preferred Bearer, non-support */
	WAP_PROF_EWCG, /* WAP CSD Gateway, non-support */
	WAP_PROF_EWBA,  
	WAP_PROF_EWBR,  
	WAP_PROF_EWCT, /* WAP Connection Timeout, non-support */
	WAP_PROF_EWNI  /* WAP Current Network Account Id */
} wap_prof_access_enum;

typedef enum
{
   WAP_PROF_SET,
	WAP_PROF_READ,
	WAP_PROF_TEST
} wap_prof_cmd_enum;

typedef struct wap_prof_access_req_struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;
   kal_uint8 src_id;
   kal_uint8 query;
   kal_uint8 cmd;
   kal_uint8 param[5][16];
}wap_prof_access_req_struct;

//for search pbook address- Jo 031203 
typedef struct mea_addr_input_struct 
{
	kal_uint8	ref_count; 
    kal_uint16	msg_len;  //LOCAL_PARA_HDR
	kal_uint8*	name;
	kal_uint8*	addr;

} mea_addr_input_struct;

typedef struct mea_addr_input_struct wap_phb_addr_input_struct;

typedef struct 
{
	LOCAL_PARA_HDR
	kal_uint16 keycode;
	kal_uint16 keytype;
} wap_mmi_key_indication_struct;

typedef struct mea_new_mms_req_struct
{
   kal_uint8 ref_count;
    kal_uint16	msg_len;  //LOCAL_PARA_HDR
	kal_uint8	mode;
	kal_uint8	commandInfo[522];	/* FMGR_MAX_PATH_LEN(260) * ENCODING_LENGTH(2) + ENCODING_LENGTH(2) */
} mea_new_mms_req_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;
   kal_uint8 browserMode;
   kal_uint8 browserIdentity;
   kal_uint16 noUrl;
   kal_uint8 *url;
   kal_uint16 noBearer;
   kal_uint8 *bearer;
   kal_uint16 noGateway;
   kal_uint8 *gateway;
} wap_mmi_lauch_browser_req_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;
   kal_uint8 res;
   kal_uint8 additionalInfo;
} wap_mmi_lauch_browser_rsp_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;
   kal_uint8 cause;
} wap_mmi_browser_termination_ind_struct;

typedef struct
{
   kal_wchar *midlet_path;
} wap_mmi_java_install_req_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;
   kal_uint8 type;
   kal_uint8 url[256];
} wap_browser_startup_req_struct;

typedef struct
{
   kal_uint8 len;
   kal_uint8 dcs;
   kal_uint8 value[40];
} wap_prof_name_struct;

typedef struct
{
   kal_uint8 id;
   wap_prof_name_struct name;
   kal_uint8 conn_type;
} wap_prof_entry_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 result;
   kal_uint8 number;
   wap_prof_entry_struct *prof_list;
} wap_get_prof_list_rsp_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 id;
} wap_get_prof_content_req_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;
   kal_uint8 result;   
   kal_uint8 id;
   kal_uint8 conn_type;
   kal_uint8 prx_ip[4];
   kal_uint16 prx_port;
   kal_uint8 username[30];
   kal_uint8 password[30];
   kal_uint8 net_id;
} wap_get_prof_content_rsp_struct;

typedef struct
{
   kal_uint8  ref_count;
   kal_uint16 msg_len;   
   kal_int32  prof_id;
   kal_uint8 net_id;
} wap_get_bra_prof_id_rsp_struct;

/* WPS SAP Begin */
typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_bool use_default;
   kal_uint8 conn_type;
   kal_uint8 net_id;
   kal_uint32 timeout;
   kal_bool use_proxy;
   kal_uint8 prx_ip[4];
   kal_uint16 prx_port;
   kal_uint8 username[30];
   kal_uint8 password[30];
   kal_uint8 server_auth_username[WPS_MAX_AUTH_USERNAME_LENGTH];
   kal_uint8 server_auth_password[WPS_MAX_AUTH_PASSWORD_LENGTH];
} wps_set_channel_req_struct;

typedef struct
{
   kal_uint32 static_header_len;
   kal_uint8* static_header;
} wps_set_channel_req_var_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 channel_id;
   kal_uint8 result;
} wps_set_channel_rsp_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 channel_id;
} wps_unset_channel_req_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 channel_id;
   kal_uint8 result;
} wps_unset_channel_rsp_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 channel_id;   
   kal_uint8 request_id;
   kal_uint8 method;
   kal_uint16 option;
   kal_uint8 reply_type;
   kal_wchar reply_path[256];
   kal_uint32 reply_segment_len;
   kal_uint8 post_type;
   kal_wchar post_path[256];
   kal_uint32 post_total_len;
   kal_bool   more_post;
} wps_http_req_struct; 

typedef struct
{
   kal_uint32 request_url_len;
   kal_uint8* request_url;
   kal_uint32 request_header_len;
   kal_uint8* request_header;
   kal_uint32 post_segment_len;
   kal_uint8* post_segment;
} wps_http_req_var_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 request_id;
   kal_uint8 result;
   kal_uint16 status;      
   kal_uint8 protocol;
   kal_uint8 content_type[256];
   kal_uint32 content_length;
   kal_bool more;
} wps_http_rsp_struct; 

typedef struct
{
   kal_uint32 new_url_len;
   kal_uint8* new_url;
   kal_uint32 reply_header_len;
   kal_uint8* reply_header;
   kal_uint32 reply_segment_len;
   kal_uint8* reply_segment;
} wps_http_rsp_var_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 channel_id;
   kal_uint8 request_id;
   kal_uint8 seq_num;
   kal_uint32 read_segnemtn_len;
} wps_read_content_req_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 request_id;
   kal_uint8 seq_num;
   kal_uint8 result;
   kal_bool more;
} wps_read_content_rsp_struct;

typedef struct
{
   kal_uint32 reply_segment_len;
   kal_uint8* reply_segment;
} wps_read_content_rsp_var_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 channel_id;
   kal_uint8 request_id;
   kal_uint8 seq_num;
   kal_bool more;
} wps_post_content_res_struct;

typedef struct
{
   kal_uint32 post_segment_len;
   kal_uint8* post_segment;
} wps_post_content_res_var_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 request_id;
   kal_uint8 seq_num;
   kal_uint8 result;
} wps_post_content_ind_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 request_id;
   kal_uint8 seq_num;
   kal_uint8 status;
   kal_uint8 acc_size;
} wps_dl_progress_ind_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;  
   kal_uint8 channel_id; 
   kal_uint8 request_id;
} wps_cancel_req_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 request_id;
   kal_uint8 result;
} wps_cancel_rsp_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 content_type[256];
   kal_uint32 content_length;
   kal_wchar data_path[256];
} wps_push_ind_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 channel_id;
   kal_uint8 request_id;
   kal_uint8 operation;
} wps_clear_req_struct;

typedef struct
{
   kal_uint32 cache_url_len;
   kal_uint8  *cache_url;
} wps_clear_req_var_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 request_id;
   kal_uint8 result;
} wps_clear_rsp_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 channel_id;
} wps_sec_get_session_info_req_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 channel_id;
   kal_uint8 result;
   kal_uint16 key_exchange_alg;
   kal_uint8 blk_encrypt_alg;
   kal_uint8 hash_alg;
   kal_uint8 sec_conn_type;
} wps_sec_get_session_info_rsp_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 channel_id;
} wps_sec_get_current_cert_req_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 channel_id;
   kal_uint8 result;
   kal_uint8 issuer[128];
   kal_uint16 issuer_len;
   kal_uint32 not_after;
   kal_uint32 not_before;
   kal_uint8 serial_num[128];
   kal_uint16 serial_num_len;
   kal_uint8 sig_alg_name[128];
   kal_uint16 sig_alg_name_len;
   kal_uint8 subject[128];
   kal_uint16 subject_len;
   kal_uint8 type;
   kal_uint8 version;
} wps_sec_get_current_cert_rsp_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 channel_id;
   kal_uint8 request_id;
   kal_uint8 auth_type;
   kal_uint8 realm_len;
   kal_uint8 realm[256];
} wps_http_auth_ind_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 channel_id;
   kal_uint8 request_id;
   kal_uint8 username[WPS_MAX_AUTH_USERNAME_LENGTH];
   kal_uint8 password[WPS_MAX_AUTH_PASSWORD_LENGTH];
} wps_http_auth_res_struct;

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;   
   kal_uint8 status;
} wps_status_query_rsp_struct;
/* WPS SAP end */

#endif


