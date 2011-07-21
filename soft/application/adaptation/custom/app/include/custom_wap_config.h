/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2001
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   custom_wap_config.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   The file contains WAP/MMS configuration constant. 
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
#ifndef _CUSTOM_WAP_CONFIG_H
#define _CUSTOM_WAP_CONFIG_H
 
#include "kal_release.h"      	/* basic data type */

// __CUSTPACK_MULTIBIN     Johnnie   BEGIN
#define CUSTOM_WAP_PROFILE_PROXY_PORT 9
#define CUSTOM_WAP_PROFILE_PROFILE_NAME 0
#define CUSTOM_WAP_PROFILE_HOMEPAGE_URL 24
#define CUSTOM_WAP_PROFILE_DATA_ACCOUNT 1
#define CUSTOM_WAP_PROFILE_CONN_TYPE 4
#define CUSTOM_WAP_PROFILE_PROXY_IP 7
#define CUSTOM_WAP_PROFILE_USERNAME 12
#define CUSTOM_WAP_PROFILE_PASSWORD 8

#define CUSTOM_WAP_PROFILE_PROFILE_NAME_LEN 41
#define CUSTOM_WAP_PROFILE_HOMEPAGE_URL_LEN 129
#define CUSTOM_WAP_PROFILE_USERNAME_LEN 31
#define CUSTOM_WAP_PROFILE_PASSWORD_LEN 31
#define CUSTOM_WAP_ROOT_CA_CONTENT_LEN 1024
#define CUSTOM_WAP_UA_HEADER_LEN 257
#define CUSTOM_WAP_UA_PROF_URL_LEN 257
#define CUSTOM_MMS_UA_HEADER_LEN 257
#define CUSTOM_WAP_BKM_TITLE_LEN 41
#define CUSTOM_WAP_BKM_URL_LEN 129

#define WAP_CUSTOM_CFG_N_WAP_PROFILES  10
#define WAP_CUSTOM_CFG_N_MMS_PROFILES  10 
#define WAP_CUSTOM_CFG_N_NBR_BOOKMARKS 20
#define WAP_CUSTOM_CFG_N_ROOT_CA       3

#define WAP_CUSTOM_CFG_MAX_CACHE_SIZE  (20*1024)
#define WAP_CUSTOM_CFG_DEFAULT_UA_HEADER  "MAUI WAP Browser"
#define WAP_CUSTOM_CFG_DEFAULT_UA_PROF_URL   ""
#define WAP_CUSTOM_CFG_DEFAULT_BROWSE_TIMEOUT   300
#define WAP_CUSTOM_CFG_PUSH_MAX_NO_OF_MSG       15
#define WAP_CUSTOM_CFG_DEFAULT_MMS_UA_HEADER "MAUI MMS User Agent"
#define WAP_CUSTOM_CFG_MAX_NBR_MMS_MSG 101
#define WAP_CUSTOM_CFG_MAX_MMS_MSG_SIZE   50000
#define WAP_CUSTOM_CFG_MAX_MMS_WARNING_MSG 95
#define WAP_CUSTOM_CFG_MAX_MMS_GET_RETRY   2
#define WAP_CUSTOM_CFG_MAX_MMS_POST_MSG_RETRY   0 /* Customer can reduce this value to zero, if they don't want resend MMS when error occurs. */
#define WAP_CUSTOM_CFG_MAX_MMS_POST_PDU_RETRY   2 /* This value isn't recommeded to reduce. */
#define WAP_CUSTOM_CFG_MMS_GET_TIMEOUT   400 /* 40 seconds */
#define WAP_CUSTOM_CFG_MMS_POST_TIMEOUT   1200 /* 120 seconds, this value isn't recommeded to reduce.*/
#define WAP_CUSTOM_CFG_MMS_SLIDE_DURATION 8000 // ms (unused)
#define WAP_CUSTOM_CFG_MMS_IMMED_RETRIEVAL_MODE 1 // 0: auto, 1: deferred, 2: restricted (unused)
#define WAP_CUSTOM_CFG_MMS_ROAM_RETRIEVAL_MODE 1 // 0: auto, 1: deferred, 2: restricted (unused)
#define WAP_CUSTOM_CFG_MMS_ALLOW_ADVERTIMSEMENT 1 // 0: no, 1: yes (unused)
#define WAP_CUSTOM_CFG_MMS_ALLOW_ANONYMOUS 1 // 0: no, 1: yes (unused)
#define WAP_CUSTOM_CFG_MMS_REQ_DELIVERY_REPORT 1 // 0: no, 1: yes (unused)
#define WAP_CUSTOM_CFG_MMS_REQ_READ_REPORT 1 // 0: no, 1: yes (unused)
#define WAP_CUSTOM_CFG_MMS_SEND_DELIVERY_REPORT 1 // 0: no, 1: yes (unused)
#define WAP_CUSTOM_CFG_MMS_SEND_READ_REPORT 1 // 0: no, 1: yes (unused)
#define WAP_CUSTOM_CFG_MMS_VERSION 1 // 0: default, 1: 1.0, 2: 1.1, 3: latest version (unused)

/* the following 2 constant are deserved from constant above and unnecessary to to modify */
#define WAP_CUSTOM_CFG_PROFILE_FILE_SIZE  ((WAP_CUSTOM_CFG_N_WAP_PROFILES+WAP_CUSTOM_CFG_N_MMS_PROFILES)*256)
#define WAP_CUSTOM_CFG_BOOKMARK_FILE_SIZE (WAP_CUSTOM_CFG_N_NBR_BOOKMARKS*256)

/* Define WAP_GLOBAL_MEM_SIZE for WAP memory pool size */
#define WAP_GLOBAL_MEM_SIZE (64*1024)

/* the following are macro to access wap default and unnecessary to modify */
#define CUST_WAP_PROFILE_DEFAULT (custpack_wap_default.wap_profile_default)
#define CUST_MMS_PROFILE_DEFAULT (custpack_wap_default.mms_profile_default)
#define CUST_WAP_BKM_DEFAULT     (custpack_wap_default.wap_bkm_default)
#define CUST_WAP_ROOT_CA_DEFAULT (custpack_wap_default.wap_root_ca_default)
#define CUST_WAP_CONFIG_DEFAULT  (custpack_wap_default.wap_config_default)
#define CUST_MMS_CONFIG_DEFAULT  (custpack_wap_default.mms_config_default)

typedef struct
{
   kal_uint8 editable;
   kal_uint8 empty;
   kal_uint16 proxy_port;            
   kal_uint8 profile_name[CUSTOM_WAP_PROFILE_PROFILE_NAME_LEN];
   kal_uint8 homepage_url[CUSTOM_WAP_PROFILE_HOMEPAGE_URL_LEN];
   kal_uint8 data_account;
   kal_uint8 conn_type;
   kal_uint8 proxy_ip[4];
   kal_uint8 username[CUSTOM_WAP_PROFILE_USERNAME_LEN];
   kal_uint8 password[CUSTOM_WAP_PROFILE_PASSWORD_LEN];
}  custom_profile_content_struct; 
   

typedef struct
{
   kal_uint8 n_wap_profiles;
   custom_profile_content_struct profile[WAP_CUSTOM_CFG_N_WAP_PROFILES];
} custom_wap_profile_struct;


typedef struct
{
   kal_uint8 n_mms_profiles;         
   custom_profile_content_struct profile[WAP_CUSTOM_CFG_N_MMS_PROFILES];
} custom_mms_profile_struct;

typedef struct
{
   kal_uint8 editable;
   kal_uint8 empty;
   kal_uint8 bkm_name[CUSTOM_WAP_BKM_TITLE_LEN];
   kal_uint8 bkm_url[CUSTOM_WAP_BKM_URL_LEN];
} custom_bkm_content_struct;

typedef struct
{
   kal_uint16 max_n_bkms;      
   custom_bkm_content_struct bkm[WAP_CUSTOM_CFG_N_NBR_BOOKMARKS];
} custom_wap_bkm_struct;

typedef struct
{
   kal_uint8 editable;
   kal_uint8 empty;
   kal_uint8 root_ca_type;
   kal_uint8 root_ca_content[CUSTOM_WAP_ROOT_CA_CONTENT_LEN];
   kal_uint32 root_ca_content_len;
} custom_root_ca_content_struct;
   
typedef struct
{
   kal_uint8 n_root_ca;
   custom_root_ca_content_struct root_ca[WAP_CUSTOM_CFG_N_ROOT_CA];
} custom_wap_root_ca_struct;

typedef struct
{
   kal_uint32 max_cache_size;
   kal_uint32 browse_timeout;
   kal_uint16 max_n_push_msg;
   kal_uint8 ua_header[CUSTOM_WAP_UA_HEADER_LEN];
   kal_uint8 ua_prof_url[CUSTOM_WAP_UA_PROF_URL_LEN];
} custom_wap_config_struct;

typedef struct
{
   kal_uint32 max_mms_size;
   kal_uint16 max_n_mms_msg;
   kal_uint16 max_n_msg_warning;   
   kal_uint8 ua_header[CUSTOM_MMS_UA_HEADER_LEN];
   kal_uint8 max_n_get_retry;
   kal_uint8 max_n_post_msg_retry;
   kal_uint8 max_n_post_pdu_retry;
   kal_uint32 get_timeout;
   kal_uint32 post_timeout;  
   kal_uint32 slide_duration;
   kal_uint8 immed_retrieval_mode;
   kal_uint8 roam_retrieval_mode;
   kal_uint8 allow_advertimsement;
   kal_uint8 allow_anonymous;
   kal_uint8 req_delivery_report;
   kal_uint8 req_read_report;
   kal_uint8 send_delivery_report;
   kal_uint8 send_read_report;
   kal_uint8 mms_version;   
} custom_mms_config_struct;

typedef struct
{
   kal_uint8 version;
   custom_wap_profile_struct const * wap_profile_default;
   custom_mms_profile_struct const * mms_profile_default;
   custom_wap_bkm_struct     const * wap_bkm_default;
   custom_wap_root_ca_struct const * wap_root_ca_default;
   custom_wap_config_struct  const * wap_config_default;
   custom_mms_config_struct  const * mms_config_default;
} wap_custpack_header;

/*******************************************************************************
 *   Global function prototype                                                 *
 ******************************************************************************/
extern kal_uint8 wap_custom_get_max_wap_prof_num(void);
extern kal_uint8 wap_custom_get_max_mms_prof_num(void);
extern kal_uint32 wap_custom_get_max_cache_size(void);
extern kal_uint16 wap_custom_get_max_bookmark_num(void);
extern const kal_uint8* wap_custom_get_ua_header(void);
extern const kal_uint8* wap_custom_get_ua_prof_url(void);
extern kal_uint16  wap_custom_get_max_push_msg_num(void);
extern kal_uint32 wap_custom_get_default_browse_timeout(void);
extern const kal_uint8* wap_custom_get_mms_user_agent(void);
extern kal_uint16 wap_custom_get_max_mms_num(void);
extern kal_uint32 wap_custom_get_max_mms_msg_size(void);
extern kal_uint16 wap_custom_get_max_mms_warning_num(void);
extern kal_uint32 wap_custom_get_max_mms_get_retry_timeout(void);
extern kal_uint32 wap_custom_get_max_mms_get_retry_nums(void);
extern kal_uint32 wap_custom_get_max_mms_post_retry_timeout(void);
extern kal_uint32 wap_custom_get_max_mms_post_msg_retry_nums(void);
extern kal_uint32 wap_custom_get_max_mms_post_pdu_retry_nums(void);
extern kal_uint32 wap_custom_get_mms_slide_duration(void);
extern kal_uint8 wap_custom_get_mms_immed_retrieval_mode(void);
extern kal_uint8 wap_custom_get_mms_roam_retrieval_mode(void);
extern kal_uint8 wap_custom_get_mms_allow_advertimsement(void);
extern kal_uint8 wap_custom_get_mms_allow_anonymous(void);
extern kal_uint8 wap_custom_get_mms_req_delivery_report(void);
extern kal_uint8 wap_custom_get_mms_req_read_report(void);
extern kal_uint8 wap_custom_get_mms_send_delivery_report(void);
extern kal_uint8 wap_custom_get_mms_send_read_report(void);
extern kal_uint8 wap_custom_get_mms_version(void);

extern const custom_wap_profile_struct wap_profile_default;
extern const custom_mms_profile_struct mms_profile_default;
extern const custom_wap_bkm_struct wap_bkm_default;
extern const custom_wap_root_ca_struct  wap_root_ca_default;
extern const custom_wap_config_struct wap_config_default;
extern const custom_mms_config_struct mms_config_default;
extern const wap_custpack_header custpack_wap_ptr;
extern wap_custpack_header custpack_wap_default;
// __CUSTPACK_MULTIBIN     Johnnie   END

#endif /* _CUSTOM_WAP_CONFIG_H */


