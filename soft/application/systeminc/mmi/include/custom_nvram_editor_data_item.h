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
 * custom_nvram_editor_data_item.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *    This file is for customers to config/customize their parameters to NVRAM Layer and
 *    Driver Layer.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _CUSTOM_NVRAM_EDTIOR_DATA_ITEM_H
#define _CUSTOM_NVRAM_EDTIOR_DATA_ITEM_H

#ifndef __L1_STANDALONE__

/*****
  **
  ** This is structure for NVRAM_EF_CSUT_ACOUSTIC_DATA_LID
  **
  *****/
#include "device.h"
#include "nvram_default_audio.h"
#include "mcd_l4_common.h"

#ifndef GEN_FOR_PC
	#include "l4c2uem_struct.h"
#endif

#ifdef MMI_ON_WIN32
#include "kal_non_specific_general_types.h"
#endif

#define MAX_GPRS_USER_NAME_LEN 32   
#define MAX_GPRS_PASSWORD_LEN  32
#define MAX_GPRS_APN_LEN       100

/* Definations below is defined at  "nvram_user_config.h" before 04.40*/
#define NVRAM_EDITOR_NUM_OF_BYTE_KEY	8
#define NVRAM_EDITOR_NUM_OF_BYTE_NP		3
#define NVRAM_EDITOR_NUM_NP				3
#define NVRAM_EDITOR_NUM_OF_BYTE_NSP	5
#define NVRAM_EDITOR_NUM_NSP			   3
#define NVRAM_EDITOR_NUM_GID			   20
#define NVRAM_EDITOR_NUM_OF_BYTE_IMSI	9
#define NVRAM_EDITOR_NUM_PIN1			   8

//fengzb add for jataayu profile begin
#ifdef JATAAYU_SUPPORT
#define NVRAM_WAP_PROFILE_MAX             10
#define NVRAM_MMS_PROFILE_MAX             10
#define NVRAM_WAP_PROFILE_PROFILE_NAME_LEN 41
#define NVRAM_WAP_PROFILE_NAME_LEN  41
#define NVRAM_MMS_PROFILE_NAME_LEN  41
#define NVRAM_WAP_PROFILE_HOMEPAGE_URL_LEN 256
#define NVRAM_MMS_PROFILE_MMSC_URL_LEN    256
#define NVRAM_WAP_PROFILE_USERNAME_LEN 31
#define NVRAM_WAP_PROFILE_PASSWORD_LEN 31
#define NVRAM_MMS_PROFILE_USERNAME_LEN 31
#define NVRAM_MMS_PROFILE_PASSWORD_LEN 31
typedef struct
{
    U8 editable;
    U8 empty;
    U16 proxy_port;
    U8 homepage_url[NVRAM_WAP_PROFILE_HOMEPAGE_URL_LEN];
    U8 data_account;
    U8 conn_type;
    U8 proxy_ip[4];
    U8 username[NVRAM_WAP_PROFILE_USERNAME_LEN];
    U8 password[NVRAM_WAP_PROFILE_PASSWORD_LEN];
} nvram_wap_profile_content_struct;

typedef struct
{
    U8 editable;
    U8 empty;
    U16 proxy_port;
    U8 mmsc_url[NVRAM_MMS_PROFILE_MMSC_URL_LEN];
    U8 data_account;
    U8 conn_type;
    U8 proxy_ip[4];
    U8 username[NVRAM_MMS_PROFILE_USERNAME_LEN];
    U8 password[NVRAM_MMS_PROFILE_PASSWORD_LEN];
} nvram_mms_profile_content_struct;

typedef struct
{
    U8 conn_type;
    U8 dcs;
    U8 profile_name[NVRAM_WAP_PROFILE_NAME_LEN];
} nvram_wap_profile_name_struct;

typedef struct
{
    U8 conn_type;
    U8 dcs;
    U8 profile_name[NVRAM_MMS_PROFILE_NAME_LEN];
} nvram_mms_profile_name_struct;

typedef struct
{
    nvram_wap_profile_name_struct nvram_wap_profile_name_array[NVRAM_WAP_PROFILE_MAX];
} nvram_wap_profile_name_array_struct;

typedef struct
{
    nvram_mms_profile_name_struct nvram_mms_profile_name_array[NVRAM_MMS_PROFILE_MAX];
} nvram_mms_profile_name_array_struct;

/*for Browser profile*/
#define NVRAM_EF_WAP_PROFILE_CONTENT_SIZE		sizeof(nvram_wap_profile_content_struct)
#define NVRAM_EF_WAP_PROFILE_CONTENT_TOTAL		10
#define NVRAM_EF_WAP_PROFILE_NAMES_SIZE			sizeof(nvram_wap_profile_name_array_struct)
#define NVRAM_EF_WAP_PROFILE_NAMES_TOTAL		1

/*for MMS profile*/
#define NVRAM_EF_MMS_PROFILE_CONTENT_SIZE		sizeof(nvram_mms_profile_content_struct)
#define NVRAM_EF_MMS_PROFILE_CONTENT_TOTAL		10
#define NVRAM_EF_MMS_PROFILE_NAMES_SIZE			sizeof(nvram_mms_profile_name_array_struct)
#define NVRAM_EF_MMS_PROFILE_NAMES_TOTAL		1

 
#define TITLES_PER_RECORD  20
#define URLS_PER_RECORD    5

#define NVRAM_BRW_MAX_BOOKMARKS                    20
#define NVRAM_BRW_BKM_MAX_TITLE_LENGTH             41
#define NVRAM_BRW_BKM_MAX_URL_LENGTH               256
#endif
//fengzb add for jataayu profile end
 
typedef struct
{
   kal_uint8                als_line_id[2];
}nvram_ef_als_line_id_struct;
 



typedef struct {
	kal_uint8 text_length;
	kal_uint8 text_dcs;
	kal_uint8 text[UEM_GREETING_LEN];
} rmi_greeting_text_struct;

 
typedef struct 
{
   kal_uint8                        volume_gain[MAX_VOL_CATE][MAX_VOL_TYPE][MAX_VOL_LEVEL];
   kal_uint8                        volume[MAX_VOL_CATE][MAX_VOL_TYPE];
} custom_acoustic_struct;
//uem_context_acoustic_struct

typedef struct
{
   kal_int16  Audio_FIR_Input_Coeff_Tbl[30];
   kal_int16  Audio_FIR_Output_Coeff_Tbl[30];
   kal_int16  Audio_FIR_Output_Coeff_Tbl_a[30];
   kal_int16  Audio_FIR_Output_Coeff_Tbl_b[30];
   kal_int16  Audio_FIR_Output_Coeff_Tbl_c[30];
   kal_int16  Audio_FIR_Output_Coeff_Tbl_d[30];
   kal_int16  Audio_FIR_Output_Coeff_Tbl_e[30];
   kal_int16  Audio_FIR_Input_Coeff_Tbl2[30];
   kal_int16  Audio_FIR_Output_Coeff_Tbl2[30];
   kal_uint16  selected_FIR_output_index;
   kal_uint16 ES_TimeConst;
   kal_uint16 ES_VolConst;
   kal_uint16 ES_TimeConst2;
   kal_uint16 ES_VolConst2;
   kal_uint16 Media_Playback_Maximum_Swing;
   kal_int16  Speech_FIR_Input_Coeff_16k_Tbl1[62];
   kal_int16  Speech_FIR_Output_Coeff_16k_Tbl1[62];
   kal_int16  Speech_FIR_Input_Coeff_16k_Tbl2[62];
   kal_int16  Speech_FIR_Output_Coeff_16k_Tbl2[62];
   kal_int16  Melody_FIR_Output_Coeff_32k_Tbl1[45];
   kal_int16  Melody_FIR_Output_Coeff_32k_Tbl2[45];
   kal_uint8  Reserved[72];  /*to prevent human mistake (size mismatch with nvram_data_items.c) */
}nvram_ef_audio_param_struct;

/*****
  **
  ** This is structure for NVRAM_EF_CUST_HW_LEVEL_TBL_LID
  **
  *****/
 
#include "custom_hw_default.h"

typedef struct 
{
   kal_uint32   PWM1[PWM_MAX_LEVEL][2];  /* freq, duty */
   kal_uint32   PWM2[PWM_MAX_LEVEL][2];  /* freq, duty */
   kal_uint32   PWM3[PWM_MAX_LEVEL][2];  /* freq, duty */
   kal_uint32   MainLCD_Contrast[LCD_CONTRAST_MAX_LEVEL];
   kal_uint32   MainLCD_Bias[LCD_PARAM_MAX_LEVEL];
   kal_uint32   MainLCD_Linerate[LCD_PARAM_MAX_LEVEL];
   kal_uint32   MainLCD_Temp[LCD_PARAM_MAX_LEVEL];
   kal_uint32   SubLCD_Contrast[LCD_CONTRAST_MAX_LEVEL];
   kal_uint32   SubLCD_Bias[LCD_PARAM_MAX_LEVEL];
   kal_uint32   SubLCD_Linerate[LCD_PARAM_MAX_LEVEL];
   kal_uint32   SubLCD_Temp[LCD_PARAM_MAX_LEVEL];
   kal_uint32   BatteryLevel[BATTERY_MAX_LEVEL];
} custom_hw_level_struct;

/*****
  **
  ** This is structure for NVRAM_EF_CSD_PROFILE_LID
  **
  *****/
typedef struct {
   l4_addr_bcd_struct addr;   
   kal_uint8   csd_ur;
   kal_uint8   csd_type;
   kal_uint8   csd_module;
   kal_uint8   auth_type;
   kal_uint8   user_id[32];
   kal_uint8   user_pwd[32];
   kal_uint8   dns_addr[4];
   l4_name_struct name;
} nvram_ef_csd_profile_struct;

typedef struct {
    kal_uint8 qos_length; /* QOS identifer bit*/
    kal_uint8 unused1; /* Unused Bit*/
    kal_uint8 delay_class;
    kal_uint8 reliability_class;
    kal_uint8 peak_throughput;
    kal_uint8 unused2; /* Unused Bit*/
    kal_uint8 precedence_class;
    kal_uint8 unused3; /* Unused Bit*/
    kal_uint8 mean_throughput;
    kal_uint8 traffic_class;
    kal_uint8 delivery_order;
    kal_uint8 delivery_of_err_sdu;
    kal_uint8 max_sdu_size;
    kal_uint8 max_bitrate_up_lnk;
    kal_uint8 max_bitrate_down_lnk;
    kal_uint8 residual_bit_err_rate;
    kal_uint8 sdu_err_ratio;
    kal_uint8 transfer_delay;
    kal_uint8 traffic_hndl_priority;
    kal_uint8 guarntd_bit_rate_up_lnk;
    kal_uint8 guarntd_bit_rate_down_lnk;
} nvram_editor_qos_struct;

/*****
  **
  ** This is structure for NVRAM_EF_ABM_GPRS_PROFILE_LID
  **
  *****/
typedef struct
{
   kal_uint8               context_id;
   kal_uint8 	    name_length;
   kal_uint8 	   name_dcs;
   kal_uint8               authentication_type;
   kal_uint8 	   name[32];      
   kal_uint8               user_name[MAX_GPRS_USER_NAME_LEN];
   kal_uint8               password[MAX_GPRS_PASSWORD_LEN];
   kal_uint8               dns[4];
   kal_uint8               apn [MAX_GPRS_APN_LEN];
   kal_uint8               apn_length;
   nvram_editor_qos_struct     req_qos;
} nvram_ef_abm_gprs_profile_struct;

typedef struct {
  nvram_ef_abm_gprs_profile_struct DataAccount1;
  nvram_ef_abm_gprs_profile_struct DataAccount2;
  nvram_ef_abm_gprs_profile_struct DataAccount3;
  nvram_ef_abm_gprs_profile_struct DataAccount4;
  nvram_ef_abm_gprs_profile_struct DataAccount5;
  nvram_ef_abm_gprs_profile_struct DataAccount6;
  nvram_ef_abm_gprs_profile_struct DataAccount7;
  nvram_ef_abm_gprs_profile_struct DataAccount8;
  nvram_ef_abm_gprs_profile_struct DataAccount9;
  nvram_ef_abm_gprs_profile_struct DataAccount10;
  
} nvram_meta_abm_gprs_profile_struct;

 
typedef struct
{
   kal_uint8                reg_ps_key[NVRAM_EDITOR_NUM_OF_BYTE_KEY];
   kal_uint8                security_indication;
   kal_uint8                auto_lock_item;
   kal_uint8                np_code[NVRAM_EDITOR_NUM_OF_BYTE_NP * NVRAM_EDITOR_NUM_NP];
   kal_uint8                np_key[NVRAM_EDITOR_NUM_OF_BYTE_KEY];
   kal_uint8                nsp_code[NVRAM_EDITOR_NUM_OF_BYTE_NSP * NVRAM_EDITOR_NUM_NSP];
   kal_uint8                nsp_key[NVRAM_EDITOR_NUM_OF_BYTE_KEY];
   kal_uint8                np_of_sp[NVRAM_EDITOR_NUM_OF_BYTE_NP];
   kal_uint8                np_of_cp[NVRAM_EDITOR_NUM_OF_BYTE_NP];   
   kal_uint8                gid1[NVRAM_EDITOR_NUM_GID];
   kal_uint8                gid2[NVRAM_EDITOR_NUM_GID];
   kal_uint8                sp_key[NVRAM_EDITOR_NUM_OF_BYTE_KEY];
   kal_uint8                cp_key[NVRAM_EDITOR_NUM_OF_BYTE_KEY];
   kal_uint8                imsi_code[NVRAM_EDITOR_NUM_OF_BYTE_IMSI];
   kal_uint8                imsi_key[NVRAM_EDITOR_NUM_OF_BYTE_KEY];
   kal_uint8                phone_key[NVRAM_EDITOR_NUM_OF_BYTE_KEY];
   kal_uint8                last_imsi[NVRAM_EDITOR_NUM_OF_BYTE_IMSI];
   kal_uint8                pin1[NVRAM_EDITOR_NUM_PIN1];
   kal_uint8                pin1_valid;
   kal_uint8                phone_lock_verified;
}smu_security_info_struct;

typedef struct
{
	kal_uint16	       tst_port;
	kal_uint16             ps_port;
	kal_uint32             tst_baudrate;
	kal_uint32             ps_baudrate;
	kal_bool                high_speed_sim_enabled;
//Lisen 05032005	Begin
	kal_uint8			swdbg;
//Lisen 05032005	End
} nvram_ef_port_setting_struct;

/*****
  **
  ** This is structure for NVRAM_EF_PHB_LN_ENTRY_LID
  **
  *****/
#define NVRAM_EF_PHB_LN_SIZE 10

typedef struct {
    kal_uint8 name_length;
    kal_uint8 name_dcs;
    kal_uint8 name[32];
    kal_uint8 count;
    kal_uint8 addr_length;
    rtc_format_struct time;
    kal_uint8 addr_bcd[41];
} phb_ln_entry_struct;

typedef  struct { 
   kal_uint8 no_entry;
   kal_uint8 padding;
   phb_ln_entry_struct array[NVRAM_EF_PHB_LN_SIZE];
} nvram_ef_phb_ln_struct;

#define NVRAM_EF_PHB_LN_TYPE_SEQ_SIZE  150

typedef  struct {
   kal_uint8 type_seq[NVRAM_EF_PHB_LN_TYPE_SEQ_SIZE];
} nvram_ef_phb_ln_type_seq_struct;

/*****
  **
  ** This is structure for NVRAM_EF_UEM_MANUFACTURE_LID
  **
  *****/

 
typedef struct
{
   kal_uint8   me_manufacture_id[MAX_ME_ID_NUM][MAX_ME_ID_LEN];
} uem_context_manufacture_struct;

typedef struct 
{

   kal_uint8                        audio_mute_flag; /* AUDIO_DEVICE_SPEAKER,... */

   kal_uint8                        gpio_status[MAX_GPIO_DEVICE_NUM];
   kal_uint8                        silent_mode;

   kal_uint8                        alert_mode;

   kal_uint8                        country_code;
   kal_uint8                        lang_id[2];

   kal_uint8                        date_mode;
   kal_uint8                        time_mode;

   kal_uint8                        greeting_mode;
   //kal_uint8                        greeting_text[UEM_GREETING_LEN];
   rmi_greeting_text_struct  greeting_text; 

   rtc_alarm_info_struct            alarm_info[MAX_ALARM_NUM];
} uem_context_rmi_struct;


typedef struct _nvram_smsal_common_param_struct 
{
   kal_uint8	        bearer_service;			      
   kal_uint8	        status_report;			
   kal_uint8	        reply_path;			
   kal_uint8           vp_format;
   kal_uint8           prefer_mem1;
   kal_uint8           prefer_mem2;
   kal_uint8           prefer_mem3;
   kal_uint8           cbch_req;
   kal_uint8           all_lang_on;
   kal_uint8           rmi_act_pfile_id;
   kal_uint8	        fo;    
	kal_uint8	        sms_fdn_off;
} nvram_ef_smsal_common_param_struct;

/*****
  **
  ** This is structure for NVRAM_EF_TCM_PDP_PROFILE_LID
  **
  *****/
 

 
typedef struct {
   kal_uint8                   context_id;
   kal_uint8                   nsapi;
   kal_uint8                   pdp_addr_type;
   kal_uint8                   pdp_addr_len;
   kal_uint8                   addr_val[4];
   nvram_editor_qos_struct     req_qos;
   nvram_editor_qos_struct     min_qos;
   kal_uint8                   apn_len;
   kal_uint8                   apn[100];
   kal_uint8                   pcomp_algo;
   kal_uint8                   dcomp_algo;
   kal_uint8                   context_type;
   kal_uint8                   primary_context_id;
   kal_uint8                   is_sib_defined;
   kal_uint8                   dns_val[4];
   kal_uint8                   is_chap;
   kal_uint8                   name_length;
   kal_uint8                   name_dcs;
   kal_uint8                   name[32];
   kal_uint8                   user_name[16];
   kal_uint8                   password[16];
   kal_uint8                   reserved;
} nvram_ef_tcm_PDP_profile_record_struct;

/*****
  **
  ** This is structure for NVRAM_EF_AUTOTEST_LID
  **
  *****/
typedef struct {
   kal_uint8   testcount;
   kal_uint8   testlist[229];
   kal_uint8   rtc_sec;
   kal_uint8   rtc_min;
   kal_uint8   rtc_hour;
   kal_uint8   rtc_day;
   kal_uint8   rtc_mon;
   kal_uint8   rtc_wday;
   kal_uint8   rtc_year;
   kal_uint8   spare[18];
} nvram_ef_autotest_struct;

typedef struct 
{
	kal_uint16	  cbmi[10];
} nvram_ef_cb_default_ch_struct ;

#endif /* __L1_STANDALONE__ */

#define VER_LID(lid)	lid##_VERNO lid
#define VER(lid)	lid##_VERNO

#define NVRAM_EF_CUST_ACOUSTIC_DATA_LID_VERNO		"000" 
#define NVRAM_EF_AUDIO_PARAM_LID_VERNO                  "003" 
#define NVRAM_EF_CUST_HW_LEVEL_TBL_LID_VERNO           	"001" 
#define NVRAM_EF_CACHE_BYTE_LID_VERNO                  	"007" 
#define NVRAM_EF_CACHE_SHORT_LID_VERNO                 	"004" 
#define NVRAM_EF_CACHE_DOUBLE_LID_VERNO                	"001" 
#define NVRAM_EF_IP_NUMBER_LID_VERNO                   	"002" 
#define NVRAM_EF_SHORTCUTS_LID_VERNO                   	"001" 
#define NVRAM_EF_SETTING_LID_VERNO                     	"000" 
#define NVRAM_EF_SMS_LID_VERNO                         	"001" 
#define NVRAM_EF_SMS_CB_CHNL_LID_VERNO                 	"004" 
#define NVRAM_EF_SMS_CB_SMS_LID_VERNO                  	"003" 
#define NVRAM_EF_SMS_CB_SMS_INFO_LID_VERNO             	"001" 
#define NVRAM_EF_CHAT_ROOM_INFO_LID_VERNO              	"000" 
#define NVRAM_EF_EMS_MY_PICTURE_NAME_LID_VERNO         	"000" 
#define NVRAM_EF_EMS_MY_ANIMATION_NAME_LID_VERNO       	"000" 
#define NVRAM_EF_EMS_MY_MELODY_NAME_LID_VERNO          	"000" 
#define NVRAM_EF_PHB_IDS_LID_VERNO                     	"004" 
#define NVRAM_EF_PHB_FIELDS_LID_VERNO                  	"002" 
#define NVRAM_EF_PHB_SETTINGS_LID_VERNO                	"001" 
#define NVRAM_EF_PHB_CALLER_GROUPS_LID_VERNO           	"003" 
#define NVRAM_EF_CALORIE_DATA_LID_VERNO                	"000" 
#define NVRAM_EF_PHONEBOOK_SOS_LID_VERNO               	"001" 
#define NVRAM_EF_PHONEBOOK_SPEEDDIAL_LID_VERNO         	"001" 
#define NVRAM_EF_PROFILES_LID_VERNO                    	"004" 
#define NVRAM_EF_DOWNLOAD_IMAGE_LID_VERNO              	"000" 
#define NVRAM_EF_THEMES_VALUES_VERNO                   	"001" 
#define NVRAM_EF_TODO_LIST_LID_VERNO                   	"002" 
#ifdef __MMI_NOTEPAD__
#define NVRAM_EF_NOTEPAD_LIST_LID_VERNO                "001"
#define NVRAM_EF_NOTEPAD_RECORD_LID_VERNO           "001"
#endif
#define NVRAM_EF_GROUPLIST_LID_VERNO                   	"000" 
#define NVRAM_EF_COMPOSE_RINGTONE_LID_VERNO            	"000" 
#define NVRAM_EF_DOWNLOAD_TONE_LID_VERNO               	"000" 
#define NVRAM_EF_DOWNLOAD_MELODY_LID_VERNO             	"000" 
#define NVRAM_EF_CALL_TIME_LID_VERNO                   	"000" 

//added by zhoumn @2006/11/29>>>start
#define	NVRAM_EF_CALL_COST_GET_MAX_LID_VERNO			"000"
#define	NVRAM_EF_CALL_COST_GET_ALL_COST_LID_VERNO       "001"
#define	NVRAM_EF_CALL_COST_GET_LAST_COST_LID_VERNO      "002"
#define	NVRAM_EF_CALL_COST_GET_CURRENCY_LID_VERNO   	"003"
#define	NVRAM_EF_CALL_COST_GET_PPU_LID_VERNO   			"004"
//added by zhoumn @2006/11/29<<<end

#define NVRAM_EF_ROOT_CA_LID_VERNO                     	"000" 
#define NVRAM_EF_CSD_PROFILE_LID_VERNO                 	"001" 
#define NVRAM_EF_ABM_GPRS_PROFILE_LID_VERNO            	"003" 
#define NVRAM_EF_EXT_MELODY_INFO_LID_VERNO             	"001" 
#define NVRAM_EF_IMEI_IMEISV_LID_VERNO                 	"000" 
#define NVRAM_EF_ALM_QUEUE_LID_VERNO                   	"000" 
#define NVRAM_EF_ALM_SPOF_DATA_LID_VERNO               	"001" 
#define NVRAM_EF_CLASSMARK_RACAP_LID_VERNO             	"000" 
#define NVRAM_EF_MS_SECURITY_LID_VERNO                 	"001" 
#define NVRAM_EF_ADC_LID_VERNO                         	"000" 
#define NVRAM_EF_BARCODE_NUM_LID_VERNO                 	"000" 
#define NVRAM_EF_PHB_VCARD_LID_VERNO                    "000" 
#define NVRAM_EF_SYS_CACHE_OCTET_LID_VERNO             	"000" 
#define NVRAM_EF_PHB_LID_VERNO                         	"001" 
#define NVRAM_EF_PHB_LN_ENTRY_LID_VERNO                	"000" 
#define NVRAM_EF_UEM_MANUFACTURE_DATA_LID_VERNO        	"000" 
#define NVRAM_EF_UEM_RMI_DATA_LID_VERNO                	"000" 
#define NVRAM_EF_SMSAL_COMMON_PARAM_LID_VERNO          	"001" 
#define NVRAM_EF_SMSAL_MWIS_LID_VERNO                  	"000" 
#define NVRAM_EF_SMSAL_MAILBOX_ADDR_LID_VERNO          	"000" 
#define NVRAM_EF_SMSAL_SMS_LID_VERNO                   	"000" 
#define NVRAM_EF_RAC_PREFERENCE_LID_VERNO		"001"
#define NVRAM_EF_TCM_PDP_PROFILE_LID_VERNO             	"000" 
#define NVRAM_EF_CAMERA_PARA_LID_VERNO                 	"000" 
#define NVRAM_EF_WPSS_FILENAME_LID_VERNO               	"001" 
#define NVRAM_EF_AUTOTEST_LID_VERNO                    	"000" 
//Lisen 05032005	Begin
#define NVRAM_EF_PORT_SETTING_LID_VERNO                 "003" 
//Lisen 05032005	End
#define NVRAM_EF_STOPWATCH_LID_VERNO                   	"000" 
#define NVRAM_PICTURE_EDITOR_LID_VERNO                 	"000" 
#define NVRAM_EF_MSG_CLUB_LID_VERNO                    	"000" 
#define NVRAM_EF_MSG_CLUB_NUM_LID_VERNO                	"000" 
#define NVRAM_EF_FMGR_FILEPATH_POOL_HEAD_LID_VERNO		   "000"
#define NVRAM_EF_FMGR_FILEPATH_POOL_TAIL_LID_VERNO		   "000"
#define NVRAM_EF_IMPS_LID_VERNO   "001"			

#define NVRAM_EF_BLACK_LIST_LID_VERNO		"001"
#define NVRAM_EF_PHB_BIRTHDAY_LID_VERNO		"000"
#define NVRAM_EF_AUDIO_EQUALIZER_LID_VERNO		"001"

#define NVRAM_EF_MESSAGES_BLACK_LIST_LID_VERNO	"000"
#if defined(__MMI_SAVE_CURRENT_TIME__)
#define NVRAM_EF_SAVE_CURRENT_TIME_LID_VERNO	"000"
#endif
#define NVRAM_EF_PHB_LN_TYPE_SEQ_LID_VERNO      "001" 
#define NVRAM_EF_PHB_COMPARE_DIGIT_LID_VERNO      "000" 
#define NVRAM_EF_CUSTPACK_VERNO_LID_VERNO	"000"  
 
#define	NVRAM_EF_MSCAP_LID_VERNO                        "000" 
#define	NVRAM_EF_ALS_LINE_ID_LID_VERNO                  "000" 
#define  NVRAM_EF_CB_DEFAULT_CH_LID_VERNO                "000" 
 

 
#define	NVRAM_EF_EONS_INFO_LID_VERNO			"000"
 

//wufasong added 2007.01.09
#define NVRAM_EF_CAMERA_SETTING_LID_VERNO		"001"

//wufasong added 2007.01.09
#define NVRAM_EF_CAMERA_SETTING_LID_VERNO		"001"
#define NVRAM_EF_FM_RADIO_LID_VERNO  "001"
#define NVRAM_EF_FMSR_SETTING_LID_VERNO  "001"
#define NVRAM_EF_PHB_VIDEO_LID_VERNO  "001"
#define NVRAM_EF_VIDEO_SETTING_LID_VERNO  "001"
#define NVRAM_EF_PHOTO_EDITOR_SETTING_LID_VERNO  "001"
#define NVRAM_EF_EBOOK_SETTINGS_LID_VERNO  "001"
//fengzb add for wap/mms profile begin
#define NVRAM_EF_WAP_PROFILE_CONTENT_LID_VERNO "001"
#define NVRAM_EF_MMS_PROFILE_CONTENT_LID_VERNO "001"
#define NVRAM_EF_WAP_PROFILE_NAMES_LID_VERNO "001"
#define NVRAM_EF_MMS_PROFILE_NAMES_LID_VERNO "001"
//fengzb add for wap/mms profile end

#ifdef __MMI_MULTI_SIM__
#define NVRAM_EF_PHONEBOOK_SIM2_SOS_LID_VERNO               	"001" 
#define NVRAM_EF_PHONEBOOK_SIM3_SOS_LID_VERNO               	"001" 
#define NVRAM_EF_PHONEBOOK_SIM4_SOS_LID_VERNO               	"001" 

#define NVRAM_EF_WAP_PROFILE_CONTENT_SIM2_LID_VERNO               	"001" 
#define NVRAM_EF_MMS_PROFILE_CONTENT_SIM2_LID_VERNO               	"001" 
#define NVRAM_EF_WAP_PROFILE_NAMES_SIM2_LID_VERNO               	"001" 
#define NVRAM_EF_MMS_PROFILE_NAMES_SIM2_LID_VERNO               	"001" 
#define NVRAM_EF_WAP_PROFILE_CONTENT_SIM3_LID_VERNO               	"001" 
#define NVRAM_EF_MMS_PROFILE_CONTENT_SIM3_LID_VERNO               	"001" 
#define NVRAM_EF_WAP_PROFILE_NAMES_SIM3_LID_VERNO               	"001" 
#define NVRAM_EF_MMS_PROFILE_NAMES_SIM3_LID_VERNO               	"001" 
#define NVRAM_EF_WAP_PROFILE_CONTENT_SIM4_LID_VERNO               	"001" 
#define NVRAM_EF_MMS_PROFILE_CONTENT_SIM4_LID_VERNO               	"001" 
#define NVRAM_EF_WAP_PROFILE_NAMES_SIM4_LID_VERNO               	"001" 
#define NVRAM_EF_MMS_PROFILE_NAMES_SIM4_LID_VERNO               	"001" 
#endif

#ifdef __MMI_VIP_FUNCTION__
#define NVRAM_EF_VIP_FUNCTION_LID_VERNO                "000"
#endif

/*Don't remove this line: insert version number definition above*/
#ifdef GEN_FOR_PC
#include "nvram_user_defs.h"
#include "custom_mmi_default_value.h"

 
typedef  struct { 
   kal_uint8 speech_version;
   kal_uint8 data_cap1;
   kal_uint8 data_cap2;
   kal_uint8 channel_coding;   
} nvram_ef_mscap_struct;
 


typedef struct
{
   kal_uint8  cust[NVRAM_EF_CUST_SIZE];
}nvram_ef_cust_struct;

typedef struct
{
   kal_uint8  CacheByte[NVRAM_CACHE_SIZE];
}nvram_cache_byte_struct;

typedef struct
{
   kal_uint16  CacheShort[NVRAM_CACHE_SIZE/2];
}nvram_cache_short_struct;

typedef struct
{
   kal_uint64 Cachedouble[NVRAM_CACHE_SIZE/8];
}nvram_cache_double_struct;

typedef struct
{
   kal_uint16  shortcuts[NVRAM_SHORTCUTS_SIZE/2];
}nvram_shortcuts_struct;

typedef struct
{
   kal_uint8  setting[NVRAM_SETTING_SIZE];
}nvram_setting_struct;

typedef struct
{
   kal_uint8  ext_melody[NVRAM_EF_EXT_MELODY_INFO_SIZE];
}nvram_meta_ext_melody_struct;

typedef  struct { 
   kal_uint8 imei[8];
   kal_uint8 svn;
   kal_uint8 pad;
} nvram_ef_imei_imeisv_struct;

//CSD Joy added for NVRAM data, 1129
/*********************** SMS use **************************/


typedef struct
{
	kal_char	smsTemplate[NVRAM_SMS_TEMPL_RECORD_SIZE];	
}nvram_sms_struct;

typedef struct
{
	CB_CHANNEL_INDEX	l4MidIndex;	
	CB_CHANNEL_NAME 	chnlNames1;
	CB_CHANNEL_NAME 	chnlNames2;
	CB_CHANNEL_NAME 	chnlNames3;
	CB_CHANNEL_NAME 	chnlNames4;
	CB_CHANNEL_NAME 	chnlNames5;
	CB_CHANNEL_NAME 	chnlNames6;
	CB_CHANNEL_NAME 	chnlNames7;
	CB_CHANNEL_NAME 	chnlNames8;
	CB_CHANNEL_NAME 	chnlNames9;
	CB_CHANNEL_NAME 	chnlNames10;	
}nvram_sms_cb_chnl_struct;

typedef struct
{
	kal_uint8 MsgContent[NVRAM_SMS_CB_SMS_PAGE_SIZE];
}nvram_sms_cb_sms_struct;

typedef struct
{
	kal_uint8 MsgDCS[MAX_STORE_CB_MSG];
	kal_uint8 MsgPage[MAX_STORE_CB_MSG];
	kal_uint16 MsgMI[MAX_STORE_CB_MSG];
}nvram_sms_cb_sms_info_struct;

typedef struct
{
	kal_char nickName[(8 +1)*2];
	kal_uint8 destinationNo[(20 +2)];
}nvram_chat_room_info_struct;

typedef struct
{
	EMS_OBJECT_NAME 	emsPictureNames1;
	EMS_OBJECT_NAME 	emsPictureNames2;
	EMS_OBJECT_NAME 	emsPictureNames3;
	EMS_OBJECT_NAME 	emsPictureNames4;
	EMS_OBJECT_NAME 	emsPictureNames5;	
}nvram_ems_my_picture_name_struct;

typedef struct
{
	EMS_OBJECT_NAME 	emsAnimationNames1;
	EMS_OBJECT_NAME 	emsAnimationNames2;
	EMS_OBJECT_NAME 	emsAnimationNames3;
	EMS_OBJECT_NAME 	emsAnimationNames4;
	EMS_OBJECT_NAME 	emsAnimationNames5;	
}nvram_ems_my_animation_name_struct;

typedef struct
{
	EMS_OBJECT_NAME 	emsMelodyNames1;
	EMS_OBJECT_NAME 	emsMelodyNames2;
	EMS_OBJECT_NAME 	emsMelodyNames3;
	EMS_OBJECT_NAME 	emsMelodyNames4;
	EMS_OBJECT_NAME 	emsMelodyNames5;
}nvram_ems_my_melody_name_struct;

/*********************** SMS end **************************/
//CSD end

//CSD 2003-11-30 Wilson Begin
typedef struct
{
  PHB_OPTIONAL_IDS_STRUCT  phb_ids[NVRAM_PHB_IDS_RECORD_COUNT];
}nvram_phb_ids_struct;

typedef struct
{
  kal_uint8	homeNumber[MAX_PB_NUMBER_LENGTH+1+1];
  kal_uint8 	companyName[(MAX_PB_COMPANY_LENGTH+1)*ENCODING_LENGTH];
  kal_uint8 	emailAddress[MAX_PB_EMAIL_LENGTH+1];
  kal_uint8 	officeNumber[MAX_PB_NUMBER_LENGTH+1+1];
  kal_uint8 	faxNumber[MAX_PB_NUMBER_LENGTH+1+1];
}nvram_phb_fields_struct;

typedef struct
{
  kal_uint8	viewfieldStates[NVRAM_PHB_SETTINGS_SIZE];
}nvram_phb_settings_struct;

typedef struct
{
  PHB_CALLER_GROUP_STRUCT Friends;
  PHB_CALLER_GROUP_STRUCT Family;
  PHB_CALLER_GROUP_STRUCT VIP;
  PHB_CALLER_GROUP_STRUCT Business;
  PHB_CALLER_GROUP_STRUCT Others;        
}nvram_phb_caller_groups_struct;
//CSD 2003-11-30 Wilson End


typedef struct
{
	ActivityDetails Monday;
	ActivityDetails Tuesday;
	ActivityDetails Wednesday;
	ActivityDetails Thursday;
	ActivityDetails Friday;
	ActivityDetails Saturday;
	ActivityDetails Sunday;
}nvram_calorie_data_struct;

typedef struct
{
   kal_uint8  data_account_record[NVRAM_DATA_ACCOUNT_RECORD_SIZE];
}nvram_data_account_record_struct;

//CSD 2003-11-30 Wilson Begin
typedef struct
{
  kal_uint8	type;	//129-default, 145-international
  kal_uint8	length;
  kal_uint8	number[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
}nvram_phonebook_sos_record_struct;

typedef struct
{
SpeedDialInfo	Number2;
SpeedDialInfo	Number3;
SpeedDialInfo	Number4;
SpeedDialInfo	Number5;
SpeedDialInfo	Number6;
SpeedDialInfo	Number7;
SpeedDialInfo	Number8;
SpeedDialInfo	Number9;

}nvram_phonebook_speeddial_record_struct;
//CSD 2003-11-30 Wilson End


 
typedef struct
{
   DYNIMAGEINFO download_image1;
   DYNIMAGEINFO download_image2;   
   DYNIMAGEINFO download_image3;
   DYNIMAGEINFO download_image4;
   DYNIMAGEINFO download_image5;
   DYNIMAGEINFO download_image6;
   DYNIMAGEINFO download_image7;
   DYNIMAGEINFO download_image8;
   DYNIMAGEINFO download_image9;
   DYNIMAGEINFO download_image10;
   DYNIMAGEINFO download_image11;
   DYNIMAGEINFO download_image12;
   DYNIMAGEINFO download_image13;
   DYNIMAGEINFO download_image14;
   DYNIMAGEINFO download_image15;
}nvram_download_image_record_struct;

 
typedef struct{
	kal_uint8 freq_type;
	kal_uint8 current_theme_index; 
	kal_int16 set_order_flag;
	kal_uint8 themes_order[4];
}nvram_themes_values_struct;

// MAXXXXX
typedef struct
{
	kal_uint8 nvram_todo_list_record[NVRAM_TODO_LIST_RECORD_SIZE];
}nvram_todo_list_record_struct;
// MAXXXXX


typedef struct
{
   kal_uint8  name[34];
   kal_uint8  index;
   kal_uint8  dummy;
}nvram_group_lists_struct;

 
typedef struct
{
   DYNAUDIOINFO compose_ringtone1;
   DYNAUDIOINFO compose_ringtone2;   
   DYNAUDIOINFO compose_ringtone3;
   DYNAUDIOINFO compose_ringtone4;
   DYNAUDIOINFO compose_ringtone5;
}nvram_compose_ringtone_struct;

 
typedef struct
{
   DYNAUDIOINFO download_tone1;
   DYNAUDIOINFO download_tone2;   
   DYNAUDIOINFO download_tone3;
   DYNAUDIOINFO download_tone4;
   DYNAUDIOINFO download_tone5;
   DYNAUDIOINFO download_tone6;
   DYNAUDIOINFO download_tone7;
   DYNAUDIOINFO download_tone8;
   DYNAUDIOINFO download_tone9;
   DYNAUDIOINFO download_tone10;
   DYNAUDIOINFO download_tone11;
   DYNAUDIOINFO download_tone12;
   DYNAUDIOINFO download_tone13;
   DYNAUDIOINFO download_tone14;
   DYNAUDIOINFO download_tone15;
}nvram_download_tone_struct;

 
typedef struct
{

   DYNAUDIOINFO download_melody1;
   DYNAUDIOINFO download_melody2;   
   DYNAUDIOINFO download_melody3;
   DYNAUDIOINFO download_melody4;
   DYNAUDIOINFO download_melody5;
   DYNAUDIOINFO download_melody6;
   DYNAUDIOINFO download_melody7;
   DYNAUDIOINFO download_melody8;
   DYNAUDIOINFO download_melody9;
   DYNAUDIOINFO download_melody10;
   DYNAUDIOINFO download_melody11;
   DYNAUDIOINFO download_melody12;
   DYNAUDIOINFO download_melody13;
   DYNAUDIOINFO download_melody14;
   DYNAUDIOINFO download_melody15;
}nvram_download_melody_struct;

typedef struct
{
   kal_uint8  call_time[NVRAM_CALL_TIME_RECORD_SIZE]; 
}nvram_call_time_struct;




/* File Manager Filepath Pool Head [Bear] */
typedef struct
{
   kal_uint8  nvram_ef_fmgr_filepath_pool_head[NVRAM_EF_FMGR_FILEPATH_POOL_HEAD_SIZE]; 
}nvram_ef_fmgr_filepath_pool_head_struct;


/* File Manager Filepath Pool Tail [Bear] */
typedef struct
{
   kal_uint8  nvram_ef_fmgr_filepath_pool_tail[NVRAM_EF_FMGR_FILEPATH_POOL_TAIL_SIZE]; 
}nvram_ef_fmgr_filepath_pool_tail_struct;
typedef struct
{
   kal_uint8  nvram_ef_imps_prof[NVRAM_EF_IMPS_SIZE]; 
}nvram_ef_imps_struct;


typedef struct
{
   PROFILE General;
   PROFILE Meeting;
   PROFILE Care;
   PROFILE Outdoor;
   PROFILE Indoor;
   PROFILE Reserve1;
   PROFILE Reserve2;
}nvram_meta_profile_struct;


typedef  struct { 
   kal_uint8 nvram_ef_root_ca[NVRAM_EF_ROOT_CA_SIZE];
} nvram_ef_root_ca_struct;

typedef struct {
  nvram_ef_csd_profile_struct DataAccount1;
  nvram_ef_csd_profile_struct DataAccount2;
  nvram_ef_csd_profile_struct DataAccount3;
  nvram_ef_csd_profile_struct DataAccount4;
  nvram_ef_csd_profile_struct DataAccount5;
  nvram_ef_csd_profile_struct DataAccount6;
  nvram_ef_csd_profile_struct DataAccount7;
  nvram_ef_csd_profile_struct DataAccount8;
  nvram_ef_csd_profile_struct DataAccount9;
  nvram_ef_csd_profile_struct DataAccount10;  
} nvram_meta_csd_profile_struct;

typedef  struct { 
   kal_uint8 nvram_ef_ip_number[NVRAM_IP_NUMBER_SIZE];
} nvram_ef_ip_number_struct;


// MAXXXXX
typedef struct
{
	kal_uint8 nvram_alm_queue[NVRAM_ALM_QUEUE_SIZE];
} nvram_alm_queue_struct;


typedef struct 
{
	kal_uint8 nvram_alm_spof_data[NVRAM_ALM_SPOF_DATA_SIZE];
} nvram_alm_spof_data_struct;

// MAXXXXX


typedef  struct { 
   kal_uint8 byte1;
   kal_uint8 byte2;
   kal_uint8 byte3;
   kal_uint8 byte4;
   kal_uint16 byte5_byte6;
   kal_uint8 byte7;
   kal_uint8 byte8;
   kal_uint8 byte9;
   kal_uint8 byte10;
   kal_uint8 byte11;
   kal_uint8 byte12;
   } nvram_ef_classmark_racap_struct;

typedef  struct { 
   kal_uint8 nvram_ef_barcode_num[NVRAM_EF_BARCODE_NUM_SIZE];
} nvram_ef_barcode_num_struct;

typedef struct
{
   kal_uint8  CacheByte[NVRAM_EF_SYS_CACHE_OCTET_SIZE];
}nvram_sys_cache_octet_struct;

typedef  struct { 
   kal_uint8 alpha_id[62];
   kal_uint8 BCD_length;
   kal_uint8 TON_NPI;
   kal_uint8 DialNum[20];
   kal_uint8 cc_ident;
   kal_uint8 pad;
} nvram_ef_phb_struct;



typedef struct 
{
		kal_uint8	message_waiting_indication_status[5];
		kal_uint8	pad;
} nvram_ef_smsal_mwis_struct;

typedef struct 
{
   kal_uint8 alpha_id[10];
   kal_uint8 BCD_length;
   kal_uint8 TON_NPI;
   kal_uint8 DialNum[10];
   kal_uint8 cc_ident;
   kal_uint8 pad;
} nvram_ef_smsal_mailbox_addr_struct;

typedef struct 
{
	kal_uint8 status;
	kal_uint8 remainder[175];
} nvram_ef_smsal_sms_struct;


typedef struct 
{
   kal_uint8	band;
   kal_uint8	arg1;
} nvram_ef_rac_preference_struct;

typedef struct
{
	kal_uint8 nvram_camera_para[NVRAM_EF_CAMERA_PARA_SIZE];
} nvram_camera_para_struct;

typedef struct
{
	kal_uint8 nvram_wpss_filename[NVRAM_EF_WPSS_FILENAME_SIZE];
} nvram_wpss_filename_struct;

typedef struct
{
	kal_uint16 channel_name[9][21];
	kal_uint16 channel_freq[9];
} nvram_fm_radio_struct;

typedef struct
{
	kal_uint8 stopwatch_recored[NVRAM_EF_STOPWATCH_RECORD_SIZE];
} nvram_ef_stopwatch_struct;


typedef struct
{
	kal_uint8 nvram_picture_editor_file[NVRAM_PICTURE_EDITOR_SIZE];
} nvram_picture_editor_struct;


typedef struct
{
	kal_uint8 nvram_msg_club_phone_code[NVRAM_MSG_CLUB_RECORD_SIZE];
}nvram_msg_club_struct;

typedef struct
{
	kal_uint8 nvram_msg_club_number_code[NVRAM_MSG_CLUB_NUM_RECORD_SIZE];
}nvram_msg_club_num_struct;


typedef struct 
{ 
   kal_uint8 nvram_ef_black_list_number[NVRAM_EF_BLACK_LIST_SIZE];
}nvram_ef_black_list_struct;

typedef struct 
{ 
   kal_uint8 nvram_ef_phb_birthday_field[NVRAM_EF_PHB_BIRTHDAY_SIZE];
}nvram_ef_phb_birthday_struct;


/*Don't remove this line: insert structure definition above*/
#if 0
#ifndef __L1_STANDALONE__

BEGIN_NVRAM_DATA
 
/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_MSCAP_LID
  ***
  ***  Module: 
  ***
  ***  Description:
  ***
  ***  Maintainer:
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_MSCAP_LID) nvram_ef_mscap_struct * NVRAM_EF_MSCAP_TOTAL
{
  speech_version: "Supported speech version"
   {
        FR: 1 "FR"
        {
           0x0: "Not supported";
           0x1: "Supported";
        };

        HR: 1 "HR"
        {
           0x0: "Not supported";
           0x1: "Supported";
        };

        EFR: 1 "EFR"
        {
           0x0: "Not supported";
           0x1: "Supported";
        };

        sv3_FR: 1 "Speech version 3 FR"
        {
           0x0: "Not supported";
           0x1: "Supported";
        };

        sv3_HR: 1 "Speech version 3 HR"
        {
           0x0: "Not supported";
           0x1: "Supported";
        };

        AMR_FR: 1 "AMR FR"
        {
           0x0: "Not supported";
           0x1: "Supported";
        };

        AMR_HR: 1 "AMR HR"
        {
           0x0: "Not supported";
           0x1: "Supported";
        };
   };
   
   data_cap1: "Supported speech version"
   {
        CSD: 1 "CSD"
        {
           0x0: "Not Supported";
           0x1: "Supported";
        };
        
        trans_fax: 1 "Transparent Fax"
        {
           0x0: "Not Supported";
           0x1: "Supported";
        };

        non_trans_fax: 1 "Non-transparent Fax"
        {
           0x0: "Not Supported";
           0x1: "Supported";
        };

        trans_data_cs: 1 "Transparent data circuit synchronous"
        {
           0x0: "Not Supported";
           0x1: "Supported";
        };

        non_trans_data_cs: 1 "Non-transparent data circuit synchronous"
        {
           0x0: "Not Supported";
           0x1: "Supported";
        };

        trans_data_acs: 1 "Transparent data circuit asynchronous"
        {
           0x0: "Not Supported";
           0x1: "Supported";
        };

        non_trans_data_acs: 1 "Non-transparent data circuit asynchronous"
        {
           0x0: "Not Supported";
           0x1: "Supported";
        };
        
        trans_PAD_aa: 1 "Transparent PAD access asynchronous"
        {
           0x0: "Not Supported";
           0x1: "Supported";
        };
   };

   data_cap2: "Supported speech version"
   {
        non_trans_PAD_aa: 1 "Non-transparent PAD access asynchronous"
        {
           0x0: "Not Supported";
           0x1: "Supported";
        };

        trans_data_ps: 1 "Transparent data packet synchronous"
        {
           0x0: "Not Supported";
           0x1: "Supported";
        };

        non_trans_PAD_pas: 1 "Non-transparent data packet asynchronous"
        {
           0x0: "Not Supported";
           0x1: "Supported";
        };

        HSCSD_multislot: 5 "HSCSD multislot class"
        {
        };

   };

   channel_coding: "Supported speech version"   
   {
        tch_f48:1 "TCH/F4.8"
        {
           0x0: "Not Supported";
           0x1: "Supported";
        };

        tch_f96:1 "TCH/F9.6"
        {
           0x0: "Not Supported";
           0x1: "Supported";
        };
        
        tch_f144:1 "TCH/F14.4"
        {
           0x0: "Not Supported";
           0x1: "Supported";
        };
        
        tch_f288:1 "TCH/F28.8"
        {
           0x0: "Not Supported";
           0x1: "Supported";
        };
        
        tch_f32:1 "TCH/F32"
        {
           0x0: "Not Supported";
           0x1: "Supported";
        };
        
        tch_f432:1 "TCH/F43.2"
        {
           0x0: "Not Supported";
           0x1: "Supported";
        };
   };        
};

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_ALS_LINE_ID_LID
  ***
  ***  Module: 
  ***
  ***  Description:  
  ***
 
  ***
  ***********************************************************************/
LID_BIT VER_LID(NVRAM_EF_ALS_LINE_ID_LID) nvram_ef_als_line_id_struct * NVRAM_EF_ALS_LINE_ID_TOTAL
{
   als_line_id: "als_line_id" {};
};

 

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_CUST_ACOUSTIC_DATA_LID
  ***
  ***  Module: UEM
  ***
  ***  Description:  
  ***
 
  ***
  ***********************************************************************/
LID_BIT VER_LID(NVRAM_EF_CUST_ACOUSTIC_DATA_LID) custom_acoustic_struct * NVRAM_EF_CUST_ACOUSTIC_DATA_TOTAL
{
   volume_gain: "Volume Gain:[Mode (Normal, Headset, LoudSpk)][Tone type (Tone, Keytone, MIC, Sound, Speech, SideTone, Melody) ][Level (1~7)]" ;
   volume: "Current Volume Level[Mode (Normal, Headset, LoudSpk)][Tone type (Tone, Keytone, MIC, Sound, Speech, SideTone, Melody) ]" ;
};

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_ALS_LINE_ID_LID
  ***
  ***  Module: 
  ***
  ***  Description:  
  ***
 
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_AUDIO_PARAM_LID) nvram_ef_audio_param_struct * NVRAM_EF_AUDIO_PARAM_TOTAL
{
   Audio_FIR_Input_Coeff_Tbl: "Audio_FIR_Input_Coeff" {};
   Audio_FIR_Output_Coeff_Tbl: "Audio_FIR_Output_Coeff" {};
   Audio_FIR_Output_Coeff_Tbl_a: "Audio_FIR_Output_Coeff_a" {};
   Audio_FIR_Output_Coeff_Tbl_b: "Audio_FIR_Output_Coeff_b" {};
   Audio_FIR_Output_Coeff_Tbl_c: "Audio_FIR_Output_Coeff_c" {};
   Audio_FIR_Output_Coeff_Tbl_d: "Audio_FIR_Output_Coeff_d" {};
   Audio_FIR_Output_Coeff_Tbl_e: "Audio_FIR_Output_Coeff_e" {};
   Audio_FIR_Input_Coeff_Tbl2: "Audio_FIR_Input_Coeff2" {};
   Audio_FIR_Output_Coeff_Tbl2: "Audio_FIR_Output_Coeff2" {};
   selected_FIR_output_index: "selected_FIR_output_index" {};
   ES_TimeConst: "ES_TimeConst" {};
   ES_VolConst: "ES_VolConst" {};
   ES_TimeConst2: "ES_TimeConst2" {};
   ES_VolConst2: "ES_VolConst2" {};
   Media_Playback_Maximum_Swing: "Media_Playback_Maximum_Swing" {};
   Speech_FIR_Input_Coeff_16k_Tbl1: "Speech_FIR_Input_Coeff_16k_Tbl1" {};
   Speech_FIR_Output_Coeff_16k_Tbl1: "Speech_FIR_Output_Coeff_16k_Tbl1" {};
   Speech_FIR_Input_Coeff_16k_Tbl2: "Speech_FIR_Input_Coeff_16k_Tbl2" {};
   Speech_FIR_Output_Coeff_16k_Tbl2: "Speech_FIR_Output_Coeff_16k_Tbl2" {};
   Melody_FIR_Output_Coeff_32k_Tbl1: "Melody_FIR_Output_Coeff_32k_Tbl1" {};
   Melody_FIR_Output_Coeff_32k_Tbl2: "Melody_FIR_Output_Coeff_32k_Tbl2" {};
   Reserved: "Reserved" {};
};

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_CUST_ACOUSTIC_DATA_LID
  ***
  ***  Module: UEM
  ***
  ***  Description:  
  ***
 
  ***
  ***********************************************************************/
LID_BIT VER_LID(NVRAM_EF_CUST_HW_LEVEL_TBL_LID) custom_hw_level_struct * NVRAM_EF_CUST_HW_LEVEL_TBL_TOTAL
{
   PWM1: " PWM1[Level][0]: freq, PWM1[Level][1]: duty" ;
   PWM2: " PWM2[Level][0]: freq, PWM2[Level][1]: duty" ;
   PWM3: " PWM3[Level][0]: freq, PWM3[Level][1]: duty" ;

   MainLCD_Contrast: " MainLCD Contrast [Level 1 ~ 15]" ;
   /* 
   MainLCD_Bias[LCD_PARAM_MAX_LEVEL];
   MainLCD_Linerate[LCD_PARAM_MAX_LEVEL];
   MainLCD_Temp[LCD_PARAM_MAX_LEVEL];
   */
   SubLCD_Contrast: " SubLCD Contrast [Level 1 ~ 15]" ;
   /*
   SubLCD_Bias[LCD_PARAM_MAX_LEVEL];
   SubLCD_Linerate[LCD_PARAM_MAX_LEVEL];
   SubLCD_Temp[LCD_PARAM_MAX_LEVEL];
   */
   BatteryLevel: " Battery Level [Level 1 ~ 10 ] " ;

};

LID_BIT VER_LID(NVRAM_EF_CACHE_BYTE_LID) nvram_cache_byte_struct * NVRAM_CACHE_TOTAL
{
   CacheByte: "One-byte parameter setting" ;
   CacheByte[14]
   {
      default_lang: 8 "Default Language"  {};
   };
   
   CacheByte[24]
   {
      time_zone: 8 "Time Zone (Current City)"  {};
   };

   CacheByte[25]
   {
      date_format: 8 "Time Format" {};
   };

   CacheByte[26]
   {
      date_format: 8 "Date Format" {};
   };   
};

LID_BIT VER_LID(NVRAM_EF_CACHE_SHORT_LID) nvram_cache_short_struct * NVRAM_CACHE_TOTAL
{
   CacheShort: "Two-byte parameter setting" ;
   CacheShort[13]
   {
      Wallpaper: 16 "Wallpaper"  {};
   };   
};

LID_BIT VER_LID(NVRAM_EF_CACHE_DOUBLE_LID) nvram_cache_double_struct * NVRAM_CACHE_TOTAL{};

LID_BIT VER_LID(NVRAM_EF_IP_NUMBER_LID) nvram_ef_ip_number_struct* NVRAM_IP_NUMBER_TOTAL{};

 
LID_BIT VER_LID(NVRAM_EF_SHORTCUTS_LID) nvram_shortcuts_struct * NVRAM_SHORTCUTS_TOTAL
{
   shortcuts: "List of default shortcut menu IDs";
   shortcuts[0]
   {
   		Shortcut1: 16 "Menu ID of default shortcut 1" {};
   };
   shortcuts[1]
   {
   		Shortcut2: 16 "Menu ID of default shortcut 2" {};
   };
   shortcuts[2]
   {
   		Shortcut3: 16 "Menu ID of default shortcut 3" {};
   };
   shortcuts[3]
   {
   		Shortcut4: 16 "Menu ID of default shortcut 4" {};
   };
   shortcuts[4]
   {
   		Shortcut5: 16 "Menu ID of default shortcut 5" {};
   };
   shortcuts[5]
   {
   		Shortcut6: 16 "Menu ID of default shortcut 6" {};
   };
   shortcuts[6]
   {
   		Shortcut7: 16 "Menu ID of default shortcut 7" {};
   };
   shortcuts[7]
   {
   		Shortcut8: 16 "Menu ID of default shortcut 8" {};
   } ;  
   shortcuts[8]
   {
   		Shortcut9: 16 "Menu ID of default shortcut 9" {};
   };
   shortcuts[9]
   {
   		Shortcut10: 16 "Menu ID of default shortcut 10" {};
   };
};

LID_BIT VER_LID(NVRAM_EF_SETTING_LID) nvram_setting_struct * NVRAM_SETTING_TOTAL
{
   setting: "Welcome text" ;
};

//CSD Joy added for NVRAM data, 1129
/*********************** SMS use **************************/

LID_BIT VER_LID(NVRAM_EF_SMS_LID) nvram_sms_struct * NVRAM_SMS_RECORD_TOTAL
{
	smsTemplate: "SMS Template" {};
};

LID_BIT VER_LID(NVRAM_EF_SMS_CB_CHNL_LID) nvram_sms_cb_chnl_struct * NVRAM_SMS_CB_CHNL_RECORD_TOTAL
{
	l4MidIndex:				"L4 Index" {};

	chnlNames1:				"Channel 1" {};
	chnlNames1.NameDCS:		"Channel 1 Name DCS" {};
	chnlNames1.Name:		"Channel 1 Name" {};
	
	chnlNames2:				"Channel 2" {};	
	chnlNames2.NameDCS:		"Channel 2 Name DCS" {};
	chnlNames2.Name:		"Channel 2 Name" {};

	chnlNames3:				"Channel 3" {};	
	chnlNames3.NameDCS:		"Channel 3 Name DCS" {};
	chnlNames3.Name:		"Channel 3 Name" {};	

	chnlNames4:				"Channel 4" {};
	chnlNames4.NameDCS:		"Channel 4 Name DCS" {};
	chnlNames4.Name:		"Channel 4 Name" {};	

	chnlNames5:				"Channel 5" {};
	chnlNames5.NameDCS:		"Channel 5 Name DCS" {};
	chnlNames5.Name:		"Channel 5 Name" {};	

	chnlNames6:				"Channel 6" {};
	chnlNames6.NameDCS:		"Channel 6 Name DCS" {};
	chnlNames6.Name:		"Channel 6 Name" {};	

	chnlNames7:				"Channel 7" {};
	chnlNames7.NameDCS:		"Channel 7 Name DCS" {};
	chnlNames7.Name:		"Channel 7 Name" {};	

	chnlNames8:				"Channel 8" {};
	chnlNames8.NameDCS:		"Channel 8 Name DCS" {};
	chnlNames8.Name:		"Channel 8 Name" {};	

	chnlNames9:				"Channel 9" {};
	chnlNames9.NameDCS:		"Channel 9 Name DCS" {};
	chnlNames9.Name:		"Channel 9 Name" {};

	chnlNames10:			"Channel 10" {};
	chnlNames10.NameDCS:	"Channel 10 Name DCS" {};
	chnlNames10.Name:		"Channel 10 Name" {};		
};

LID_BIT VER_LID(NVRAM_EF_SMS_CB_SMS_LID) nvram_sms_cb_sms_struct * NVRAM_SMS_CB_SMS_PAGE_NUM_TOTAL
{
        MsgContent:       "Contents of CB Messages" {};
};

LID_BIT VER_LID(NVRAM_EF_SMS_CB_SMS_INFO_LID) nvram_sms_cb_sms_info_struct * NVRAM_SMS_CB_SMS_INFO_TOTAL
{
	MsgDCS:			"DCS of CB Messages" {};
	MsgPage:			"Page number of CB Messages" {};
	MsgMI:			"MI of CB Messages" {};
};

LID_BIT VER_LID(NVRAM_EF_CHAT_ROOM_INFO_LID) nvram_chat_room_info_struct * NVRAM_CHAT_ROOM_INFO_RECORD_TOTAL
{
	nickName:		"nickname" {};
	destinationNo:		"destinationNo" {};
};

LID_BIT VER_LID(NVRAM_EF_EMS_MY_PICTURE_NAME_LID) nvram_ems_my_picture_name_struct * NVRAM_EMS_MY_PICTURE_NAME_RECORD_TOTAL
{
	emsPictureNames1: "EMS Picture 1 Name" {};
	emsPictureNames2: "EMS Picture 2 Name" {};
	emsPictureNames3: "EMS Picture 3 Name" {};
	emsPictureNames4: "EMS Picture 4 Name" {};
	emsPictureNames5: "EMS Picture 5 Name" {};	
};

LID_BIT VER_LID(NVRAM_EF_EMS_MY_ANIMATION_NAME_LID) nvram_ems_my_animation_name_struct * NVRAM_EMS_MY_ANIMATION_NAME_RECORD_TOTAL
{
	emsAnimationNames1: "EMS Animation 1 Name" {};
	emsAnimationNames2: "EMS Animation 2 Name" {};
	emsAnimationNames3: "EMS Animation 3 Name" {};
	emsAnimationNames4: "EMS Animation 4 Name" {};
	emsAnimationNames5: "EMS Animation 5 Name" {};	
};
	
LID_BIT VER_LID(NVRAM_EF_EMS_MY_MELODY_NAME_LID) nvram_ems_my_melody_name_struct * NVRAM_EMS_MY_MELODY_NAME_RECORD_TOTAL
{
	emsMelodyNames1: "EMS Melody 1 Name" {};
	emsMelodyNames2: "EMS Melody 2 Name" {};
	emsMelodyNames3: "EMS Melody 3 Name" {};
	emsMelodyNames4: "EMS Melody 4 Name" {};
	emsMelodyNames5: "EMS Melody 5 Name" {};	
};

/*********************** SMS use **************************/
//CSD end


LID_BIT VER_LID(NVRAM_EF_PHB_IDS_LID) nvram_phb_ids_struct * NVRAM_PHB_IDS_TOTAL{
  phb_ids: "phonebook IDs" {};
  phb_ids.pictureTagID: "Picture tag ID" {};
  phb_ids.ringToneID: "Ring tone ID" {};
  phb_ids.callerGroupID: "Caller group ID" {};  
};

LID_BIT VER_LID(NVRAM_EF_PHB_FIELDS_LID) nvram_phb_fields_struct * NVRAM_PHB_FIELDS_TOTAL{
  homeNumber: "Home Number" {};
  companyName: "Company Name" {};
  emailAddress: "Email Address" {};
  officeNumber: "Office Number" {};
  faxNumber: "Fax Number" {};
};

LID_BIT VER_LID(NVRAM_EF_PHB_SETTINGS_LID) nvram_phb_settings_struct * NVRAM_PHB_SETTINGS_TOTAL{
  viewfieldStates: "Home # / Company Name / Email / Business # / Fax / Picture / Ring Tone / Caller Group" {};
  
  viewfieldStates[0]
  {
      Home: 8 "Home # Setting" 
      {
         0x00: "Set indication inactive" ;
         0x01: "Set indication active" ;
      };
  };

  viewfieldStates[1]
  {
      Company: 8 "Company Name Setting" 
      {
         0x00: "Set indication inactive" ;
         0x01: "Set indication active" ;
      };
  };

  viewfieldStates[2]
  {
      Email: 8 "Email Setting" 
      {
         0x00: "Set indication inactive" ;
         0x01: "Set indication active" ;
      };
  };

  viewfieldStates[3]
  {
      Business: 8 "Business # Setting" 
      {
         0x00: "Set indication inactive" ;
         0x01: "Set indication active" ;
      };
  };

  viewfieldStates[4]
  {
      Fax: 8 "Fax Setting" 
      {
         0x00: "Set indication inactive" ;
         0x01: "Set indication active" ;
      };
  };
  
  viewfieldStates[5]
  {
      Picture: 8 "Picture Setting" 
      {
         0x00: "Set indication inactive" ;
         0x01: "Set indication active" ;
      };
  };
  
  viewfieldStates[6]
  {
      RingTone: 8 "Ring Tone Setting" 
      {
         0x00: "Set indication inactive" ;
         0x01: "Set indication active" ;
      };
  };

  viewfieldStates[7]
  {
      CallerGroup: 8 "Caller Group Setting" 
      {
         0x00: "Set indication inactive" ;
         0x01: "Set indication active" ;
      };
  };
};

LID_BIT VER_LID(NVRAM_EF_PHB_CALLER_GROUPS_LID) nvram_phb_caller_groups_struct * NVRAM_PHB_CALLER_GROUPS_TOTAL{
  Friends: "Friends Caller Group" {};
  Friends.groupName: "Name" {};
  Friends.alertType: "Alert Type" {};
  Friends.ringToneID: "Ring Tone ID" {};		
  Friends.pictureTagID: "Picture ID" {};
  Friends.LEDPatternId: "LED Pattern ID" {};
  Friends.VideoID: "VideoID" {};
  Friends.is_modified: "Is record modified" {};

  Family: "Family Caller Group" {};
  Family.groupName: "Name" {};
  Family.alertType: "Alert Type" {};
  Family.ringToneID: "Ring Tone ID" {};		
  Family.pictureTagID: "Picture ID" {};
  Family.LEDPatternId: "LED Pattern ID" {};
  Family.VideoID: "VideoID" {};
  Family.is_modified: "Is record modified" {};

  VIP: "VIP Caller Group" {};
  VIP.groupName: "Name" {};
  VIP.alertType: "Alert Type" {};
  VIP.ringToneID: "Ring Tone ID" {};		
  VIP.pictureTagID: "Picture ID" {};
  VIP.LEDPatternId: "LED Pattern ID" {};
  VIP.VideoID: "VideoID" {};
  VIP.is_modified: "Is record modified" {};

  Business: "Business Caller Group" {};
  Business.groupName: "Name" {};
  Business.alertType: "Alert Type" {};
  Business.ringToneID: "Ring Tone ID" {};		
  Business.pictureTagID: "Picture ID" {};
  Business.LEDPatternId: "LED Pattern ID" {};
  Business.VideoID: "VideoID" {};
  Business.is_modified: "Is record modified" {};

  Others: "Others Caller Group" {};
  Others.groupName: "Name" {};
  Others.alertType: "Alert Type" {};
  Others.ringToneID: "Ring Tone ID" {};		
  Others.pictureTagID: "Picture ID" {};
  Others.LEDPatternId: "LED Pattern ID" {};
  Others.VideoID: "VideoID" {};
  Others.is_modified: "Is record modified" {};

};
//CSD 2003-11-30 Wilson End

 
LID_BIT VER_LID(NVRAM_EF_CALORIE_DATA_LID) nvram_calorie_data_struct * NVRAM_CALORIE_DATA_TOTAL
{
	Monday: "Activity information of Monday" {};
	Monday.ActivityId: "List of activity IDs" {};
	Monday.CaloriePerActivity: "List of calorie consumption" {};
	Monday.MinutesPerActivity: "List of minutes of each activity" {};

	Tuesday: "Activity information of Tuesday" {};
	Tuesday.ActivityId: "List of activity IDs" {};
	Tuesday.CaloriePerActivity: "List of calorie consumption" {};
	Tuesday.MinutesPerActivity: "List of minutes of each activity" {};
		
	Wednesday: "Activity information of Wednesday" {};
	Wednesday.ActivityId: "List of activity IDs" {};
	Wednesday.CaloriePerActivity: "List of calorie consumption" {};
	Wednesday.MinutesPerActivity: "List of minutes of each activity" {};

	Thursday: "Activity information of Thursday" {};
	Thursday.ActivityId: "List of activity IDs" {};
	Thursday.CaloriePerActivity: "List of calorie consumption" {};
	Thursday.MinutesPerActivity: "List of minutes of each activity" {};

	Friday: "Activity information of Friday" {};
	Friday.ActivityId: "List of activity IDs" {};
	Friday.CaloriePerActivity: "List of calorie consumption" {};
	Friday.MinutesPerActivity: "List of minutes of each activity" {};

	Saturday: "Activity information of Saturday" {};
	Saturday.ActivityId: "List of activity IDs" {};
	Saturday.CaloriePerActivity: "List of calorie consumption" {};
	Saturday.MinutesPerActivity: "List of minutes of each activity" {};

	Sunday: "Activity information of Sunday" {};
	Sunday.ActivityId: "List of activity IDs" {};
	Sunday.CaloriePerActivity: "List of calorie consumption" {};
	Sunday.MinutesPerActivity: "List of minutes of each activity" {};
};

//CSD 2003-11-30 Wilson Begin
LID_BIT VER_LID(NVRAM_EF_PHONEBOOK_SOS_LID) nvram_phonebook_sos_record_struct * NVRAM_PHONEBOOK_SOS_RECORD_TOTAL{
  type: "Number Type: 129-default, 145-international" {};
  length: "Number Length" {};
  number: "Number" {};
};

#ifdef __MMI_MULTI_SIM__
LID_BIT VER_LID(NVRAM_EF_PHONEBOOK_SIM2_SOS_LID) nvram_phonebook_sos_record_struct * NVRAM_PHONEBOOK_SOS_RECORD_TOTAL{
  type: "Number Type: 129-default, 145-international" {};
  length: "Number Length" {};
  number: "Number" {};
};
LID_BIT VER_LID(NVRAM_EF_PHONEBOOK_SIM3_SOS_LID) nvram_phonebook_sos_record_struct * NVRAM_PHONEBOOK_SOS_RECORD_TOTAL{
  type: "Number Type: 129-default, 145-international" {};
  length: "Number Length" {};
  number: "Number" {};
};
LID_BIT VER_LID(NVRAM_EF_PHONEBOOK_SIM4_SOS_LID) nvram_phonebook_sos_record_struct * NVRAM_PHONEBOOK_SOS_RECORD_TOTAL{
  type: "Number Type: 129-default, 145-international" {};
  length: "Number Length" {};
  number: "Number" {};
};
#endif

LID_BIT VER_LID(NVRAM_EF_PHONEBOOK_SPEEDDIAL_LID) nvram_phonebook_speeddial_record_struct * NVRAM_PHONEBOOK_SPEEDDIAL_RECORD_TOTAL{
  
  Number2: "Speed Dial Number Accociate with Key 2" {};	
  Number2.type: "Number Type: 129-default, 145-international" {};
  Number2.storage: "Storage Location: 1-SIM 2-PHONE" {};
  Number2.record_index: "Record Index" {};
  Number2.number: "Number" {};
  Number2.name: "Name" {};

  Number3: "Speed Dial Number Accociate with Key 3" {};	
  Number3.type: "Number Type: 129-default, 145-international" {};
  Number3.storage: "Storage Location: 1-SIM 2-PHONE" {};
  Number3.record_index: "Record Index" {};
  Number3.number: "Number" {};
  Number3.name: "Name" {};
  
  Number4: "Speed Dial Number Accociate with Key 4" {};	
  Number4.type: "Number Type: 129-default, 145-international" {};
  Number4.storage: "Storage Location: 1-SIM 2-PHONE" {};
  Number4.record_index: "Record Index" {};
  Number4.number: "Number" {};
  Number4.name: "Name" {};  

  Number5: "Speed Dial Number Accociate with Key 5" {};	
  Number5.type: "Number Type: 129-default, 145-international" {};
  Number5.storage: "Storage Location: 1-SIM 2-PHONE" {};
  Number5.record_index: "Record Index" {};
  Number5.number: "Number" {};
  Number5.name: "Name" {};  

  Number6: "Speed Dial Number Accociate with Key 6" {};	
  Number6.type: "Number Type: 129-default, 145-international" {};
  Number6.storage: "Storage Location: 1-SIM 2-PHONE" {};
  Number6.record_index: "Record Index" {};
  Number6.number: "Number" {};
  Number6.name: "Name" {};  

  Number7: "Speed Dial Number Accociate with Key 7" {};	
  Number7.type: "Number Type: 129-default, 145-international" {};
  Number7.storage: "Storage Location: 1-SIM 2-PHONE" {};
  Number7.record_index: "Record Index" {};
  Number7.number: "Number" {};
  Number7.name: "Name" {};

  Number8: "Speed Dial Number Accociate with Key 8" {};	
  Number8.type: "Number Type: 129-default, 145-international" {};
  Number8.storage: "Storage Location: 1-SIM 2-PHONE" {};
  Number8.record_index: "Record Index" {};
  Number8.number: "Number" {};
  Number8.name: "Name" {};  

  Number9: "Speed Dial Number Accociate with Key 9" {};	
  Number9.type: "Number Type: 129-default, 145-international" {};
  Number9.storage: "Storage Location: 1-SIM 2-PHONE" {};
  Number9.record_index: "Record Index" {};
  Number9.number: "Number" {};
  Number9.name: "Name" {};    
};
//CSD 2003-11-30 Wilson End

LID_BIT VER_LID(NVRAM_EF_PROFILES_LID) nvram_meta_profile_struct * NVRAM_PROFILES_RECORD_TOTAL
{
   General: "General Profile" {}; 
   General.ringVolumeLevel: "Ring Volume Level" {};
   General.keypadVolumeLevel: "Keypad Volume Level " {};
   General.loudSpeakerVolumeLevel: "Loud Speaker Volume Level" {};
   General.mtCallAlertTypeEnum: "MT Call Alert Type" {};
   General.light: "Light" {};
   General.ringTypeEnum: "Ring type" {};
   General.toneSetup: "Tone Setup" {};
   General.answeringMode: "Answering mode" {};
   General.fontSizeEnum: "Font Size" {};
   General.intelligentCallAlert: "Intelligent Call Alert" {};
   General.extraTone: "Extra Tone" {};

   Meeting: "Meeting Profile" {}; 
   Meeting.ringVolumeLevel: "Ring Volume Level" {};
   Meeting.keypadVolumeLevel: "Keypad Volume Level " {};
   Meeting.loudSpeakerVolumeLevel: "Loud Speaker Volume Level" {};
   Meeting.mtCallAlertTypeEnum: "MT Call Alert Type" {};
   Meeting.light: "Light" {};
   Meeting.ringTypeEnum: "Ring type" {};
   Meeting.toneSetup: "Tone Setup" {};
   Meeting.answeringMode: "Answering mode" {};
   Meeting.fontSizeEnum: "Font Size" {};
   Meeting.intelligentCallAlert: "Intelligent Call Alert" {};
   Meeting.extraTone: "Extra Tone" {};

   Care: "Care Profile" {}; 
   Care.ringVolumeLevel: "Ring Volume Level" {};
   Care.keypadVolumeLevel: "Keypad Volume Level " {};
   Care.loudSpeakerVolumeLevel: "Loud Speaker Volume Level" {};
   Care.mtCallAlertTypeEnum: "MT Call Alert Type" {};
   Care.light: "Light" {};
   Care.ringTypeEnum: "Ring type" {};
   Care.toneSetup: "Tone Setup" {};
   Care.answeringMode: "Answering mode" {};
   Care.fontSizeEnum: "Font Size" {};
   Care.intelligentCallAlert: "Intelligent Call Alert" {};
   Care.extraTone: "Extra Tone" {};

   Outdoor: "Outdoor Profile" {}; 
   Outdoor.ringVolumeLevel: "Ring Volume Level" {};
   Outdoor.keypadVolumeLevel: "Keypad Volume Level " {};
   Outdoor.loudSpeakerVolumeLevel: "Loud Speaker Volume Level" {};
   Outdoor.mtCallAlertTypeEnum: "MT Call Alert Type" {};
   Outdoor.light: "Light" {};
   Outdoor.ringTypeEnum: "Ring type" {};
   Outdoor.toneSetup: "Tone Setup" {};
   Outdoor.answeringMode: "Answering mode" {};
   Outdoor.fontSizeEnum: "Font Size" {};
   Outdoor.intelligentCallAlert: "Intelligent Call Alert" {};
   Outdoor.extraTone: "Extra Tone" {};

   Indoor: "Indoor Profile" {}; 
   Indoor.ringVolumeLevel: "Ring Volume Level" {};
   Indoor.keypadVolumeLevel: "Keypad Volume Level " {};
   Indoor.loudSpeakerVolumeLevel: "Loud Speaker Volume Level" {};
   Indoor.mtCallAlertTypeEnum: "MT Call Alert Type" {};
   Indoor.light: "Light" {};
   Indoor.ringTypeEnum: "Ring type" {};
   Indoor.toneSetup: "Tone Setup" {};
   Indoor.answeringMode: "Answering mode" {};
   Indoor.fontSizeEnum: "Font Size" {};
   Indoor.intelligentCallAlert: "Intelligent Call Alert" {};
   Indoor.extraTone: "Extra Tone" {};

   Reserve1: "Reserve1 Profile" {}; 
   Reserve1.ringVolumeLevel: "Ring Volume Level" {};
   Reserve1.keypadVolumeLevel: "Keypad Volume Level " {};
   Reserve1.loudSpeakerVolumeLevel: "Loud Speaker Volume Level" {};
   Reserve1.mtCallAlertTypeEnum: "MT Call Alert Type" {};
   Reserve1.light: "Light" {};
   Reserve1.ringTypeEnum: "Ring type" {};
   Reserve1.toneSetup: "Tone Setup" {};
   Reserve1.answeringMode: "Answering mode" {};
   Reserve1.fontSizeEnum: "Font Size" {};
   Reserve1.intelligentCallAlert: "Intelligent Call Alert" {};
   Reserve1.extraTone: "Extra Tone" {};

   Reserve2: "Reserve2 Profile" {}; 
   Reserve2.ringVolumeLevel: "Ring Volume Level" {};
   Reserve2.keypadVolumeLevel: "Keypad Volume Level " {};
   Reserve2.loudSpeakerVolumeLevel: "Loud Speaker Volume Level" {};
   Reserve2.mtCallAlertTypeEnum: "MT Call Alert Type" {};
   Reserve2.light: "Light" {};
   Reserve2.ringTypeEnum: "Ring type" {};
   Reserve2.toneSetup: "Tone Setup" {};
   Reserve2.answeringMode: "Answering mode" {};
   Reserve2.fontSizeEnum: "Font Size" {};
   Reserve2.intelligentCallAlert: "Intelligent Call Alert" {};
   Reserve2.extraTone: "Extra Tone" {};
};

 
LID_BIT VER_LID(NVRAM_EF_DOWNLOAD_IMAGE_LID) nvram_download_image_record_struct * NVRAM_DOWNLOAD_IMAGE_RECORD_TOTAL
{
   download_image1:  "Downloaded image 1" {};
   download_image1.ImageName: "Name of downloaded image" {};
   download_image1.ImagesId: "File index of downloaded image" {};
   
   download_image2:  "Downloaded image 2" {};
   download_image2.ImageName: "Name of downloaded image" {};
   download_image2.ImagesId: "File index of downloaded image" {};
   
   download_image3:  "Downloaded image 3" {};
   download_image3.ImageName: "Name of downloaded image" {};
   download_image3.ImagesId: "File index of downloaded image" {};
   
   download_image4:  "Downloaded image 4" {};
   download_image4.ImageName: "Name of downloaded image" {};
   download_image4.ImagesId: "File index of downloaded image" {};

   download_image5:  "Downloaded image 5" {};
   download_image5.ImageName: "Name of downloaded image" {};
   download_image5.ImagesId: "File index of downloaded image" {};

   download_image6:  "Downloaded image 6" {};
   download_image6.ImageName: "Name of downloaded image" {};
   download_image6.ImagesId: "File index of downloaded image" {};

   download_image7:  "Downloaded image 7" {};   
   download_image7.ImageName: "Name of downloaded image" {};
   download_image7.ImagesId: "File index of downloaded image" {};
  
   download_image8:  "Downloaded image 8" {};   
   download_image8.ImageName: "Name of downloaded image" {};
   download_image8.ImagesId: "File index of downloaded image" {};

   download_image9:  "Downloaded image 9" {};   
   download_image9.ImageName: "Name of downloaded image" {};
   download_image9.ImagesId: "File index of downloaded image" {};

   download_image10:  "Downloaded image 10" {};
   download_image10.ImageName: "Name of downloaded image" {};
   download_image10.ImagesId: "File index of downloaded image" {};

   download_image11:  "Downloaded image 11" {};   
   download_image11.ImageName: "Name of downloaded image" {};
   download_image11.ImagesId: "File index of downloaded image" {};
   
   download_image12:  "Downloaded image 12" {};   
   download_image12.ImageName: "Name of downloaded image" {};
   download_image12.ImagesId: "File index of downloaded image" {};
 
   download_image13:  "Downloaded image 13" {};   
   download_image13.ImageName: "Name of downloaded image" {};
   download_image13.ImagesId: "File index of downloaded image" {};
   
   download_image14:  "Downloaded image 14" {};   
   download_image14.ImageName: "Name of downloaded image" {};
   download_image14.ImagesId: "File index of downloaded image" {};
  
   download_image15:  "Downloaded image 15" {};   
   download_image15.ImageName: "Name of downloaded image" {};
   download_image15.ImagesId: "File index of downloaded image" {};

};

 
LID_BIT VER_LID(NVRAM_EF_THEMES_VALUES) nvram_themes_values_struct * NVRAM_THEMES_VALUES_RECORDS
{
	freq_type: "The update frequency of theme" {};
	current_theme_index: "Current activated theme" {};
	set_order_flag: "Current index of Set Order option" {};
	themes_order: "The list of theme in Set Order option" {};
};

 
LID_BIT VER_LID(NVRAM_EF_TODO_LIST_LID) nvram_todo_list_record_struct * NVRAM_TODO_LIST_RECORD_TOTAL
{
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
};

LID_BIT VER_LID(NVRAM_EF_GROUPLIST_LID) nvram_group_lists_struct * NVRAM_MAX_GROUP_NAME_TOTAL
{
   name: "CUG Name" {};
   index: "CUG Index" {};
};

 
LID_BIT VER_LID(NVRAM_EF_COMPOSE_RINGTONE_LID) nvram_compose_ringtone_struct * NVRAM_COMPOSE_RINGTONE_TOTAL
{
   compose_ringtone1:  "Composed ringtone 1" {};
   compose_ringtone1.AudioName: "Name of composed tone" {};
   compose_ringtone1.AudioId: "File index of composed tone" {};
   
   compose_ringtone2:  "Composed ringtone 2" {};
   compose_ringtone2.AudioName: "Name of composed tone" {};
   compose_ringtone2.AudioId: "File index of composed tone" {};

   compose_ringtone3:  "Composed ringtone 3" {};
   compose_ringtone3.AudioName: "Name of composed tone" {};
   compose_ringtone3.AudioId: "File index of composed tone" {};

   compose_ringtone4:  "Composed ringtone 4" {};
   compose_ringtone4.AudioName: "Name of composed tone" {};
   compose_ringtone4.AudioId: "File index of composed tone" {};

   compose_ringtone5:  "Composed ringtone 5" {};
   compose_ringtone5.AudioName: "Name of composed tone" {};
   compose_ringtone5.AudioId: "File index of composed tone" {};
};

 
LID_BIT VER_LID(NVRAM_EF_DOWNLOAD_TONE_LID) nvram_download_tone_struct * NVRAM_DOWNLOAD_TONE_RECORD_TOTAL
{
   download_tone1:  "Downloaded tone 1" {};
   download_tone1.AudioName: "Name of downloaded tone" {};
   download_tone1.AudioId: "File index of downloaded tone" {};
   
   download_tone2:  "Downloaded tone 2" {}; 
   download_tone2.AudioName: "Name of downloaded tone" {};
   download_tone2.AudioId: "File index of downloaded tone" {}; 

   download_tone3:  "Downloaded tone 3" {};
   download_tone3.AudioName: "Name of downloaded tone" {};
   download_tone3.AudioId: "File index of downloaded tone" {}; 

   download_tone4:  "Downloaded tone 4" {}; 
   download_tone4.AudioName: "Name of downloaded tone" {};
   download_tone4.AudioId: "File index of downloaded tone" {}; 

   download_tone5:  "Downloaded tone 5" {};
   download_tone5.AudioName: "Name of downloaded tone" {};
   download_tone5.AudioId: "File index of downloaded tone" {}; 

   download_tone6:  "Downloaded tone 6" {}; 
   download_tone6.AudioName: "Name of downloaded tone" {};
   download_tone6.AudioId: "File index of downloaded tone" {}; 

   download_tone7:  "Downloaded tone 7" {}; 
   download_tone7.AudioName: "Name of downloaded tone" {};
   download_tone7.AudioId: "File index of downloaded tone" {}; 

   download_tone8:  "Downloaded tone 8" {};
   download_tone8.AudioName: "Name of downloaded tone" {};
   download_tone8.AudioId: "File index of downloaded tone" {};

   download_tone9:  "Downloaded tone 9" {};
   download_tone9.AudioName: "Name of downloaded tone" {};
   download_tone9.AudioId: "File index of downloaded tone" {};

   download_tone10:  "Downloaded tone 10" {}; 
   download_tone10.AudioName: "Name of downloaded tone" {};
   download_tone10.AudioId: "File index of downloaded tone" {}; 

   download_tone11:  "Downloaded tone 11" {}; 
   download_tone11.AudioName: "Name of downloaded tone" {};
   download_tone11.AudioId: "File index of downloaded tone" {}; 

   download_tone12:  "Downloaded tone 12" {};
   download_tone12.AudioName: "Name of downloaded tone" {};
   download_tone12.AudioId: "File index of downloaded tone" {}; 

   download_tone13:  "Downloaded tone 13" {}; 
   download_tone13.AudioName: "Name of downloaded tone" {};
   download_tone13.AudioId: "File index of downloaded tone" {};

   download_tone14:  "Downloaded tone 14" {};
   download_tone14.AudioName: "Name of downloaded tone" {};
   download_tone14.AudioId: "File index of downloaded tone" {};

   download_tone15:  "Downloaded tone 15" {};
   download_tone15.AudioName: "Name of downloaded tone" {};
   download_tone15.AudioId: "File index of downloaded tone" {};
};

 
LID_BIT VER_LID(NVRAM_EF_DOWNLOAD_MELODY_LID) nvram_download_melody_struct * NVRAM_DOWNLOAD_MELODY_RECORD_TOTAL
{
   download_melody1:  "Downloaded melody 1" {};
   download_melody1.AudioName: "Name of downloaded melody" {};
   download_melody1.AudioId: "File index of downloaded melody" {};
   
   download_melody2:  "Downloaded melody 2" {};
   download_melody2.AudioName: "Name of downloaded melody" {};
   download_melody2.AudioId: "File index of downloaded melody" {};

   download_melody3:  "Downloaded melody 3" {};
   download_melody3.AudioName: "Name of downloaded melody" {};
   download_melody3.AudioId: "File index of downloaded melody" {};

   download_melody4:  "Downloaded melody 4" {};
   download_melody4.AudioName: "Name of downloaded melody" {};
   download_melody4.AudioId: "File index of downloaded melody" {};

   download_melody5:  "Downloaded melody 5" {};
   download_melody5.AudioName: "Name of downloaded melody" {};
   download_melody5.AudioId: "File index of downloaded melody" {};
 
   download_melody6:  "Downloaded melody 6" {};
   download_melody6.AudioName: "Name of downloaded melody" {};
   download_melody6.AudioId: "File index of downloaded melody" {};

   download_melody7:  "Downloaded melody 7" {};
   download_melody7.AudioName: "Name of downloaded melody" {};
   download_melody7.AudioId: "File index of downloaded melody" {};
 
   download_melody8:  "Downloaded melody 8" {};
   download_melody8.AudioName: "Name of downloaded melody" {};
   download_melody8.AudioId: "File index of downloaded melody" {};

   download_melody9:  "Downloaded melody 9" {};
   download_melody9.AudioName: "Name of downloaded melody" {};
   download_melody9.AudioId: "File index of downloaded melody" {};

   download_melody10:  "Downloaded melody 10" {};
   download_melody10.AudioName: "Name of downloaded melody" {};
   download_melody10.AudioId: "File index of downloaded melody" {};

   download_melody11:  "Downloaded melody 11" {};
   download_melody11.AudioName: "Name of downloaded melody" {};
   download_melody11.AudioId: "File index of downloaded melody" {};

   download_melody12:  "Downloaded melody 12" {};
   download_melody12.AudioName: "Name of downloaded melody" {};
   download_melody12.AudioId: "File index of downloaded melody" {};

   download_melody13:  "Downloaded melody 13" {};
   download_melody13.AudioName: "Name of downloaded melody" {};
   download_melody13.AudioId: "File index of downloaded melody" {};

   download_melody14:  "Downloaded melody 14" {};
   download_melody14.AudioName: "Name of downloaded melody" {};
   download_melody14.AudioId: "File index of downloaded melody" {};

   download_melody15:  "Downloaded melody 15" {};
   download_melody15.AudioName: "Name of downloaded melody" {};
   download_melody15.AudioId: "File index of downloaded melody" {};
};

LID_BIT VER_LID(NVRAM_EF_CALL_TIME_LID) nvram_call_time_struct * NVRAM_CALL_TIME_RECORD_TOTAL{};

LID_BIT VER_LID(NVRAM_EF_ROOT_CA_LID)   nvram_ef_root_ca_struct * NVRAM_EF_ROOT_CA_TOTAL{};

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_CSD_PROFILE_LID
  ***
  ***  Module: 
  ***
  ***  Description:  
  ***
 
  ***
  ***********************************************************************/
LID_BIT VER_LID(NVRAM_EF_CSD_PROFILE_LID) nvram_meta_csd_profile_struct * NVRAM_EF_CSD_PROFILE_TOTAL
{
   DataAccount1: "Data Account 1" {}; 
   DataAccount1.addr: "dial-up addr struct" {};
       DataAccount1.addr.addr_length: "dial-up phone number length" {};
       DataAccount1.addr.addr_bcd:"dial-up phone number in BCD" {};
   DataAccount1.csd_ur: "data call user rate" {};
   DataAccount1.csd_type: "data call type" {};
   DataAccount1.csd_module: "data module to be used" {};
   DataAccount1.auth_type: "authentication type" {};
   DataAccount1.user_id: "user authentication ID" {};
   DataAccount1.user_pwd: "user authentication password" {};
   DataAccount1.dns_addr: "DNS server address" {};
   DataAccount1.name: "Profile name struct" {};
      DataAccount1.name.name_length: "Profile name length" {};
      DataAccount1.name.name_dcs: "name_dcs" {};
      DataAccount1.name.name: "Profile name";

   DataAccount2: "Data Account 2" {}; 
   DataAccount2.addr: "dial-up addr struct" {};
       DataAccount2.addr.addr_length: "dial-up phone number length" {};
       DataAccount2.addr.addr_bcd:"dial-up phone number in BCD" {};
   DataAccount2.csd_ur: "data call user rate" {};
   DataAccount2.csd_type: "data call type" {};
   DataAccount2.csd_module: "data module to be used" {};
   DataAccount2.auth_type: "authentication type" {};
   DataAccount2.user_id: "user authentication ID" {};
   DataAccount2.user_pwd: "user authentication password" {};
   DataAccount2.dns_addr: "DNS server address" {};
   DataAccount2.name: "Profile name struct" {};
      DataAccount2.name.name_length: "Profile name length" {};
      DataAccount2.name.name_dcs: "name_dcs" {};
      DataAccount2.name.name: "Profile name";

   DataAccount3: "Data Account 3" {}; 
   DataAccount3.addr: "dial-up addr struct" {};
       DataAccount3.addr.addr_length: "dial-up phone number length" {};
       DataAccount3.addr.addr_bcd:"dial-up phone number in BCD" {};
   DataAccount3.csd_ur: "data call user rate" {};
   DataAccount3.csd_type: "data call type" {};
   DataAccount3.csd_module: "data module to be used" {};
   DataAccount3.auth_type: "authentication type" {};
   DataAccount3.user_id: "user authentication ID" {};
   DataAccount3.user_pwd: "user authentication password" {};
   DataAccount3.dns_addr: "DNS server address" {};
   DataAccount3.name: "Profile name struct" {};
      DataAccount3.name.name_length: "Profile name length" {};
      DataAccount3.name.name_dcs: "name_dcs" {};
      DataAccount3.name.name: "Profile name";

   DataAccount4: "Data Account 4" {}; 
   DataAccount4.addr: "dial-up addr struct" {};
       DataAccount4.addr.addr_length: "dial-up phone number length" {};
       DataAccount4.addr.addr_bcd:"dial-up phone number in BCD" {};
   DataAccount4.csd_ur: "data call user rate" {};
   DataAccount4.csd_type: "data call type" {};
   DataAccount4.csd_module: "data module to be used" {};
   DataAccount4.auth_type: "authentication type" {};
   DataAccount4.user_id: "user authentication ID" {};
   DataAccount4.user_pwd: "user authentication password" {};
   DataAccount4.dns_addr: "DNS server address" {};
   DataAccount4.name: "Profile name struct" {};
      DataAccount4.name.name_length: "Profile name length" {};
      DataAccount4.name.name_dcs: "name_dcs" {};
      DataAccount4.name.name: "Profile name";

   DataAccount5: "Data Account 5" {}; 
   DataAccount5.addr: "dial-up addr struct" {};
       DataAccount5.addr.addr_length: "dial-up phone number length" {};
       DataAccount5.addr.addr_bcd:"dial-up phone number in BCD" {};
   DataAccount5.csd_ur: "data call user rate" {};
   DataAccount5.csd_type: "data call type" {};
   DataAccount5.csd_module: "data module to be used" {};
   DataAccount5.auth_type: "authentication type" {};
   DataAccount5.user_id: "user authentication ID" {};
   DataAccount5.user_pwd: "user authentication password" {};
   DataAccount5.dns_addr: "DNS server address" {};
   DataAccount5.name: "Profile name struct" {};
      DataAccount5.name.name_length: "Profile name length" {};
      DataAccount5.name.name_dcs: "name_dcs" {};
      DataAccount5.name.name: "Profile name";
      
   DataAccount6: "Data Account 6" {}; 
   DataAccount6.addr: "dial-up addr struct" {};
       DataAccount6.addr.addr_length: "dial-up phone number length" {};
       DataAccount6.addr.addr_bcd:"dial-up phone number in BCD" {};
   DataAccount6.csd_ur: "data call user rate" {};
   DataAccount6.csd_type: "data call type" {};
   DataAccount6.csd_module: "data module to be used" {};
   DataAccount6.auth_type: "authentication type" {};
   DataAccount6.user_id: "user authentication ID" {};
   DataAccount6.user_pwd: "user authentication password" {};
   DataAccount6.dns_addr: "DNS server address" {};
   DataAccount6.name: "Profile name struct" {};
      DataAccount6.name.name_length: "Profile name length" {};
      DataAccount6.name.name_dcs: "name_dcs" {};
      DataAccount6.name.name: "Profile name";
      
   DataAccount7: "Data Account 7" {}; 
   DataAccount7.addr: "dial-up addr struct" {};
       DataAccount7.addr.addr_length: "dial-up phone number length" {};
       DataAccount7.addr.addr_bcd:"dial-up phone number in BCD" {};
   DataAccount7.csd_ur: "data call user rate" {};
   DataAccount7.csd_type: "data call type" {};
   DataAccount7.csd_module: "data module to be used" {};
   DataAccount7.auth_type: "authentication type" {};
   DataAccount7.user_id: "user authentication ID" {};
   DataAccount7.user_pwd: "user authentication password" {};
   DataAccount7.dns_addr: "DNS server address" {};
   DataAccount7.name: "Profile name struct" {};
      DataAccount7.name.name_length: "Profile name length" {};
      DataAccount7.name.name_dcs: "name_dcs" {};
      DataAccount7.name.name: "Profile name";
      
   DataAccount8: "Data Account 8" {}; 
   DataAccount8.addr: "dial-up addr struct" {};
       DataAccount8.addr.addr_length: "dial-up phone number length" {};
       DataAccount8.addr.addr_bcd:"dial-up phone number in BCD" {};
   DataAccount8.csd_ur: "data call user rate" {};
   DataAccount8.csd_type: "data call type" {};
   DataAccount8.csd_module: "data module to be used" {};
   DataAccount8.auth_type: "authentication type" {};
   DataAccount8.user_id: "user authentication ID" {};
   DataAccount8.user_pwd: "user authentication password" {};
   DataAccount8.dns_addr: "DNS server address" {};
   DataAccount8.name: "Profile name struct" {};
      DataAccount8.name.name_length: "Profile name length" {};
      DataAccount8.name.name_dcs: "name_dcs" {};
      DataAccount8.name.name: "Profile name";
      
   DataAccount9: "Data Account 9" {}; 
   DataAccount9.addr: "dial-up addr struct" {};
       DataAccount9.addr.addr_length: "dial-up phone number length" {};
       DataAccount9.addr.addr_bcd:"dial-up phone number in BCD" {};
   DataAccount9.csd_ur: "data call user rate" {};
   DataAccount9.csd_type: "data call type" {};
   DataAccount9.csd_module: "data module to be used" {};
   DataAccount9.auth_type: "authentication type" {};
   DataAccount9.user_id: "user authentication ID" {};
   DataAccount9.user_pwd: "user authentication password" {};
   DataAccount9.dns_addr: "DNS server address" {};
   DataAccount9.name: "Profile name struct" {};
      DataAccount9.name.name_length: "Profile name length" {};
      DataAccount9.name.name_dcs: "name_dcs" {};
      DataAccount9.name.name: "Profile name";
      
   DataAccount10: "Data Account 10" {}; 
   DataAccount10.addr: "dial-up addr struct" {};
       DataAccount10.addr.addr_length: "dial-up phone number length" {};
       DataAccount10.addr.addr_bcd:"dial-up phone number in BCD" {};
   DataAccount10.csd_ur: "data call user rate" {};
   DataAccount10.csd_type: "data call type" {};
   DataAccount10.csd_module: "data module to be used" {};
   DataAccount10.auth_type: "authentication type" {};
   DataAccount10.user_id: "user authentication ID" {};
   DataAccount10.user_pwd: "user authentication password" {};
   DataAccount10.dns_addr: "DNS server address" {};
   DataAccount10.name: "Profile name struct" {};
      DataAccount10.name.name_length: "Profile name length" {};
      DataAccount10.name.name_dcs: "name_dcs" {};
      DataAccount10.name.name: "Profile name";            
};
	
/* File Manager Filepath Pool */
LID_BIT VER_LID(NVRAM_EF_FMGR_FILEPATH_POOL_HEAD_LID) nvram_ef_fmgr_filepath_pool_head_struct * NVRAM_EF_FMGR_FILEPATH_POOL_HEAD_TOTAL{};
LID_BIT VER_LID(NVRAM_EF_FMGR_FILEPATH_POOL_TAIL_LID) nvram_ef_fmgr_filepath_pool_tail_struct * NVRAM_EF_FMGR_FILEPATH_POOL_TAIL_TOTAL{};
LID_BIT VER_LID(NVRAM_EF_IMPS_LID) nvram_ef_imps_struct * NVRAM_EF_IMPS_TOTAL{};
	
LID_BIT VER_LID(NVRAM_EF_ABM_GPRS_PROFILE_LID) nvram_meta_abm_gprs_profile_struct * NVRAM_EF_ABM_GPRS_PROFILE_TOTAL{};
	
LID_BIT VER_LID(NVRAM_EF_EXT_MELODY_INFO_LID) nvram_meta_ext_melody_struct * NVRAM_EF_EXT_MELODY_INFO_TOTAL{};
	
/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_IMEI_IMEISV_LID
  ***
  ***  Module: 
  ***
  ***  Description:
  ***
  ***  Maintainer:
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_IMEI_IMEISV_LID) nvram_ef_imei_imeisv_struct * NVRAM_EF_IMEI_IMEISV_TOTAL
{
   imei: "The IMEI is 15 digit" {};
   svn: "Software version number" {};
   pad: "Padding Byte" {};
};

LID_BIT VER_LID(NVRAM_EF_ALM_QUEUE_LID)   nvram_alm_queue_struct * NVRAM_ALM_QUEUE_TOTAL{};

LID_BIT VER_LID(NVRAM_EF_ALM_SPOF_DATA_LID)   nvram_alm_spof_data_struct * NVRAM_ALM_SPOF_DATA_TOTAL{};

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
 
  ***
  ***  Module: 
  ***
  ***  Description:
  ***
 
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_CLASSMARK_RACAP_LID) nvram_ef_classmark_racap_struct * NVRAM_EF_CLASSMARK_RACAP_TOTAL
{
   byte1: ""
   {
      a54: 1 "A5/4" {};
      a55: 1 "A5/5" {};
      a56: 1 "A5/6" {};
      a57: 1 "A5/7" {};
      band3: 1 "Band 3" {};
      band2: 1 "Band 2" {};
      band1: 1 "Band 1" {};
      spare: 1 "Spare" {};
   };

   byte2: ""
   {
      radio_cap1: 4 {};
      radio_cap2: 4 {};
   };

   byte3: ""
   {
      ucs2: 1 "UCS2 treatment" {};
      hscsd_mtslot_cap: 5 "HSCSD Multi slot capability" {};
      hscsd_mtslot_cap_p: 1 "HSCSD Multi slot capability present" {};
      rsupport: 1 "R-GSM support" {};
   };

   byte4: ""
   {
      gsm400_p: 1 "GSM400 present" {};
      ms_posi_method: 5 "MS Positioning method" {};
      ms_posi_method_p: 1 "MS Positioning method present"{};
      em_cap : 1 "Extended Measurement Capability" {};
   };

   byte5_byte6 : ""
   {
      gsm1900_r_cap: 4 "GSM1900 Radio Capability" {};
      gsm1900_r_cap_p: 1 "GSM1900 Radio Capability present" {};
      gsm850_r_cap: 4 "GSM850 Radio Capability" {};
      gsm850_r_cap_p: 1 "GSM850 Radio Capability present" {};
      gsm400_r_cap: 4 "GSM400 Radio Capability" {};
      gsm400_supp: 2 "GSM400 Bands Supported" {};     
   };

   byte7: "Octet3 of Mobile Station Classmark2 information element"
   {
      spare: 3 "Spare" {};
      a51: 1 "A5/1" {};
      es_ind: 1 "ES IND" {};
      rev_level: 2 "Revision level" {};
      spare: 1 "Spare" {};
   };
   
   /* Default: 0x78, PS capability present, ss=3, MT-PP SMS supported, on support E-GSM */
   byte8: "Octet4 of Mobile Station Classmark2 information element" 
   {
      spare: 1 "Spare" {};
      vgcs: 1 "VGCS" {};
      vbs: 1 "VBS" {};
      sm_cap: 1 "SM capability" {};
      SSScr_ind: 2 "SS Screen Indicator" {};
      ps_cap: 1 "PS capability" {};
      spare: 1 "Spare" {};
   };
   
    
   byte9: "Octet5 of Mobile Station Classmark2 information element"
   {
      a52: 1 "A5/2" {};
      a53: 1 "A5/3" {};
      cmsp: 1 "CMSP" {};
      solsa: 1 "SoLSA" {};
      spare: 1 "Spare" {};
      lcsva_cap: 1 "LCSVA CAP" {};
      spare: 1 "Spare" {};
      cm3: 1 "Support options in Classmark 3" {};
   };

   byte10: "GPRS related parameters used in Mobile Station Radio Access Capability IE"
   {
      rev_level_ind: 1 "Revision Level Indicator" {};
      gprs_ext_dyna_cap: 1 "GPRS Extended Dynamic Allocation Capability" {};
      gprs_mtslot_cap: 5 "GPRS Multi slot capability" {};
      gprs_mtslot_cap_p: 1 "GPRS Multi slot capability present" {};      
   };

   byte11: "GPRS related parameters used in Mobile network capability IE"
   {
      gea_1: 1 "GEA1" {};
      gea_2: 1 "GEA2" {};
      gea_3: 1 "GEA3" {};
      gea_4: 1 "GEA4" {};
      gea_5: 1 "GEA5" {};
      gea_6: 1 "GEA6" {};      
      gea_7: 1 "GEA7" {};
      sm_cap_gprs: 1 "SM capabilities via GPRS channels" {};
   };

   byte12: "GPRS related parameters used in Mobile network capability IE"
   {
      pfc: 1 "BSS packet flow proedure" {};
      spare: 7 "Spare" {};
   };

};

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_MS_SECURITY_LID
  ***
  ***  Module: 
  ***
  ***  Description:
  ***
  ***  Maintainer:
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_MS_SECURITY_LID) smu_security_info_struct * NVRAM_EF_MS_SECURITY_TOTAL
{
   reg_ps_key: "Registeration personalization key" {};
   
   security_indication: "Security indication"
   {
      pl: 1 "Phone lock" 
      {
         0x0: "Not Supported";
         0x1: "Supported";
      };
      
      nl: 1 "Network lock" 
      {
         0x0: "Not Supported";
         0x1: "Supported";
      };
      
      nsbl: 1 "Network subset lock" 
      {
         0x0: "Not Supported";
         0x1: "Supported";
      };
      
      spl: 1 "Service provide lock" 
      {
         0x0: "Not Supported";
         0x1: "Supported";
      };
      
      cl: 1 "Corporate lock" 
      {
         0x0: "Not Supported";
         0x1: "Supported";
      };
      
      il: 1 "IMSI lock" 
      {
         0x0: "Not Supported";
         0x1: "Supported";
      };
      
      al: 1 "Auto lock" 
      {
         0x0: "Not Supported";
         0x1: "Supported";
      };
      
      res: 1 "Reserved" 
      {
         /*
         0x0: "Not Supported";
         0x1: "Supported";
         */
      };

   };
   
   auto_lock_item: "Auto lock item" 
   {
      nl: 1 "Network lock" 
      {
         0x0: "Not Supported";
         0x1: "Supported";
      };
      
      nsbl: 1 "Network subset lock" 
      {
         0x0: "Not Supported";
         0x1: "Supported";
      };
      
      spl: 1 "Service provide lock" 
      {
         0x0: "Not Supported";
         0x1: "Supported";
      };
      
      cl: 1 "Corporate lock" 
      {
         0x0: "Not Supported";
         0x1: "Supported";
      };
      
      il: 1 "IMSI lock" 
      {
         0x0: "Not Supported";
         0x1: "Supported";
      };

      res: 3 "Reserved" 
      {
         /*
         0x0: "Not Supported";
         0x1: "Supported";
         */
      };
     
   };
   
   np_code: "Network personalization code" {};
   np_key: "Network personalization key" {};
   nsp_code: "Network subset personalization code" {};
   nsp_key: "Network subset personalization key" {} ;
   np_of_sp: "NP of SP" {};
   np_of_cp: "NP of CP" {} ;
   gid1: "EF_GID 1" {} ;
   gid2: "EF_GID 2" {} ;
   sp_key: "Service provide personalization key" {} ;
   cp_key: "Corporate personalization key" {} ;
   imsi_code: "IMSI personalization code" {} ;
   imsi_key: "IMSI personalization key" {} ;
   phone_key: "Phone key" {} ;
   last_imsi: "Last IMSI code" {} ;

   pin1: "Last IMSI code" {} ;
   pin1_valid: "Last IMSI code" {} ;
   phone_lock_verified: "Phone Lock verified" {} ;
};

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_ADC_LID
  ***
  ***  Module: L1
  ***
  ***  Description:
  ***
  ***  Maintainer:
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_ADC_LID) ADC_CALIDATA * NVRAM_EF_ADC_TOTAL
{
   ADCSlope: "ADC Slop" ;
   ADCOffset: "ADC Offset" ;
};

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_BARCODE_NUM_LID
  ***
  ***  Module: FT
  ***
  ***  Description:  
  ***
 
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_BARCODE_NUM_LID) nvram_ef_barcode_num_struct * NVRAM_EF_BARCODE_NUM_TOTAL
{
   nvram_ef_barcode_num: "Barcode Serial Number" {};
};

LID_BIT VER_LID(NVRAM_EF_PHB_VCARD_LID) kal_uint8 * NVRAM_PHB_VCARD_LID_SIZE {};
LID_BIT VER_LID(NVRAM_EF_SYS_CACHE_OCTET_LID) nvram_sys_cache_octet_struct * NVRAM_EF_SYS_CACHE_OCTET_TOTAL {};

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_PHB_LID
  ***
  ***  Module: PHB
  ***
  ***  Description:  
  ***
 
  ***
  ***********************************************************************/
LID_BIT VER_LID(NVRAM_EF_PHB_LID) nvram_ef_phb_struct * NVRAM_EF_PHB_TOTAL
{
   alpha_id: "Alpha identifier" {};
   BCD_length: "Length of BCD number" {};
   TON_NPI: "TON and NPI" {};
   DialNum: "Dailling number" {};
   cc_ident: "Capability/ configuration identifier" {};
   pad: "Extension 1 record identifier" {};
};


/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_PHB_LN_ENTRY_LID
  ***
  ***  Module: L4C
  ***
  ***  Description:  
  ***
 
  ***
  ***********************************************************************/
LID_BIT VER_LID(NVRAM_EF_PHB_LN_ENTRY_LID) nvram_ef_phb_ln_struct * NVRAM_EF_PHB_LN_ENTRY_TOTAL
{
   no_entry: "Numbers of entries" {};
   array: "Last Number information" {};
      array.name_length: "name length" {};
      array.name_dcs: "name dcs" {};
      array.name: "name" {};
      array.count: "count" {};
      array.addr_length: "addr length" {};
      array.addr_bcd: "addr bcd" {};

      array.time: "Call time" {};
         array.time.rtc_sec: "Second" {};
         array.time.rtc_min: "Minute" {};
         array.time.rtc_hour: "Hour" {};
         array.time.rtc_day: "Day" {};
         array.time.rtc_mon: "Month" {};
         array.time.rtc_wday: "Week day" {};
         array.time.rtc_year: "Year" {};
};



/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_UEM_MANUFACTURE_DATA_LID
  ***
  ***  Module: UEM
  ***
  ***  Description:  
  ***
 
  ***
  ***********************************************************************/
LID_BIT VER_LID(NVRAM_EF_UEM_MANUFACTURE_DATA_LID) uem_context_manufacture_struct * NVRAM_EF_UEM_MANUFACTURE_DATA_TOTAL
{
   me_manufacture_id: "" {};
};



/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_UEM_RMI_DATA_LID
  ***
  ***  Module: UEM
  ***
  ***  Description:  
  ***
 
  ***
  ***********************************************************************/
LID_BIT VER_LID(NVRAM_EF_UEM_RMI_DATA_LID) uem_context_rmi_struct * NVRAM_EF_UEM_RMI_DATA_TOTAL
{

   audio_mute_flag: "Audio mute flag" {};
   gpio_status: "GPIO status" {};
   silent_mode: "Silent mode ON/OFF" {};
   alert_mode: "Alert mode" {};
   country_code: "Country code" {};
   lang_id: "Language identifier" {};
   date_mode : "Date mode" {};
   time_mode : "Time mode" {};
   greeting_mode : "Greeting mode" {};
   greeting_text : "Greeting Text" {};
      greeting_text.text_dcs : "dcs" {};
      greeting_text.text_length : "Length" {};
      greeting_text.text : "context" {};

   alarm_info: "Alarm Information" {};
      alarm_info.alarm_format: "Alarm format" {};
      alarm_info.alarm_index: "Alarm index" {};
      alarm_info.type: "Alarm Type" {};
      alarm_info.text: "Alarm Text" {};
      alarm_info.recurr: "Alarm recurrence" {};
    
      alarm_info.data_time: "Alarm date and time" {};
         alarm_info.data_time.rtc_sec: "Second" {};
         alarm_info.data_time.rtc_min: "Minute" {};
         alarm_info.data_time.rtc_hour: "Hour" {};
         alarm_info.data_time.rtc_mon: "Month" {};
         alarm_info.data_time.rtc_wday: "Week day" {};
         alarm_info.data_time.rtc_year: "Year" {};
};



/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_SMSAL_COMMON_PARAM_LID
  ***
  ***  Module: SMSAL
  ***
  ***  Description:  
  ***
  ***  Maintainer: Keven Chien
  ***
  ***********************************************************************/
LID_BIT VER_LID(NVRAM_EF_SMSAL_COMMON_PARAM_LID) nvram_ef_smsal_common_param_struct * NVRAM_EF_SMSAL_COMMON_PARAM_TOTAL
{
   bearer_service: " Preferred Service" 
      {
         bearer: 8 "Prefered band" 
         {
            0x0: "GPRS only";
            0x1: "GSM only";
            0x2: "GPRS prefer";
            0x3: "GSM prefer";
         };
      };

   status_report: " Status Report" 
      {
         status_rep: 8 "Status Report" 
         {
            0x0: "Not Supported (or request)";
            0x1: "Supported (or request)";
         };
      };

   reply_path:    " Reply Path" 
      {
         reply_p: 8 "Reply Path" 
         {
            0x0: "Not Supported (or request)";
            0x1: "Supported (or request)";
         };
      };

   vp_format:     " VP Format" 
      {
         vp_f: 8 "VP Format" 
         {
            0x0: "VP not present";
            0x1: "Enhanced format";
            0x2: "Relative format (default)";
            0x3: "Absolute format";
         };
      };

   prefer_mem1:   " Preferred Memory 1" 
      {
         p_mem1: 8 "Preferred Memory 1" 
         {
            0x0: "SIM";
            0x1: "ME";
            0x2: "SR";
            0x3: "BM";
            0x4: "TA";
            0x5: "SM_PREFER";
            0x6: "ME_PREFER";
            0x7: "Not Specified (default)";
         };
      };
   prefer_mem2:   " Preferred Memory 2" 
      {
         p_mem2: 8 "Preferred Memory 2" 
         {
            0x0: "SIM";
            0x1: "ME";
            0x2: "SR";
            0x3: "BM";
            0x4: "TA";
            0x5: "SM_PREFER";
            0x6: "ME_PREFER";
            0x7: "Not Specified (default)";
         };
      };
   prefer_mem3:   " Preferred Memory 3" 
      {
         p_mem3: 8 "Preferred Memory 3" 
         {
            0x0: "SIM";
            0x1: "ME";
            0x2: "SR";
            0x3: "BM";
            0x4: "TA";
            0x5: "SM_PREFER";
            0x6: "ME_PREFER";
            0x7: "Not Specified (default)";
         };
      };

   cbch_req:     "CB status" {};
   all_lang_on:  "CB all language ON " {};
   rmi_act_pfile_id: "Activated profile ID for AT command" {};
   fo:   "First Octet of TP Header" {}; 
   sms_fdn_off: "Turn Off SMS FDN" {};
};




/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_SMSAL_MWIS_LID
  ***
  ***  Module: SMSAL
  ***
  ***  Description:  
  ***
  ***  Maintainer: Keven Chien
  ***
  ***********************************************************************/
LID_BIT VER_LID(NVRAM_EF_SMSAL_MWIS_LID) nvram_ef_smsal_mwis_struct * NVRAM_EF_SMSAL_MWIS_TOTAL
{
   message_waiting_indication_status: "Message waiting indication status" ;
   message_waiting_indication_status[0]
   {
      vm: 1 "Voice mail" 
      {
         0x0: "Set indication active" ;
         0x1: "Set indication inactive" ;
      };

      fax: 1 "FAX" 
      {
         0x0: "Set indication active" ;
         0x1: "Set indication inactive" ;
      };

      email: 1 "Email" 
      {
         0x0: "Set indication active" ;
         0x1: "Set indication inactive" ;
      };

      other: 1 "Other" 
      {
         0x0: "Set indication active" ;
         0x1: "Set indication inactive" ;
      };
      
      RFU: 4 "RFU" 
      {
         0x0: "Set indication active" ;
         0x1: "Set indication inactive" ;
      };
   };
   
   message_waiting_indication_status[1]
   {
      vm: 8 "Number of Voicemail message waiting"  {};
   };

   message_waiting_indication_status[2]
   {
      fm: 8 "Number of fax mail message waiting" {};
   };

   message_waiting_indication_status[3]
   {
      em: 8 "Number of email message waiting" {};
   };

   message_waiting_indication_status[4]
   {
      om: 8 "Number of other message waiting" {};
   };

   pad: "Padding" {};
};



/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_SMSAL_MAILBOX_ADDR_LID
  ***
  ***  Module: SMSAL
  ***
  ***  Description:  
  ***
  ***  Maintainer: Keven Chien
  ***
  ***********************************************************************/
LID_BIT VER_LID(NVRAM_EF_SMSAL_MAILBOX_ADDR_LID) nvram_ef_smsal_mailbox_addr_struct * NVRAM_EF_SMSAL_MAILBOX_ADDR_TOTAL
{
   alpha_id: "Alpha identifier" {};
   BCD_length: "Length of BCD number" {};
   TON_NPI: "TON and NPI" {};
   DialNum: "Dailling number" {};
   cc_ident: "Capability/ configuration identifier" {};
   pad: "Extension 1 record identifier" {};
};


/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_SMSAL_SMS_LID
  ***
  ***  Module: SMSAL
  ***
  ***  Description:  
  ***
  ***  Maintainer: Keven Chien
  ***
  ***********************************************************************/
LID_BIT VER_LID(NVRAM_EF_SMSAL_SMS_LID) nvram_ef_smsal_sms_struct * NVRAM_EF_SMSAL_SMS_TOTAL
{
   status: "Status" {};
   remainder: "Reminder" {};

};

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_RAC_PREFERENCE_LID
  ***
  ***  Module: 
  ***
  ***  Description:  
  ***
  ***  Maintainer: 
  ***
  ***********************************************************************/
LID_BIT VER_LID(NVRAM_EF_RAC_PREFERENCE_LID) nvram_ef_rac_preference_struct * NVRAM_EF_RAC_PREFERENCE_TOTAL
{
   band: "Prefered band" 
   {
      pf_band: 8 "Prefered band" 
      {
         0x0: "GSM 900";
         0x1: "DCS 1800";
         0x2: "PCS 1900";
         0x3: "Dual mode, GSM 900 and DCS 1800";
         0x4: "Dual mode, GSM 900 and PCS 1900";
      };
   };
   
   
   arg1: "PLMN selection mode / Preferred Service in NMO III"
   {
      plmn_sel_mode: 1 "PLMN select mode"
      {
         0x0: "Automatic";
         0x1: "Manual";
      };
      
      pf_serv: 2 "Preferred service"
      {
         0x0: "CS_ONLY";
         0x1: "PS_ONLY";
         0x2: "BOTH_PREFER_CS";
         0x3: "BOTH_PREFER_PS";
      };

      unused: 5 "Unused" {};
   };

};

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_TCM_PDP_PROFILE_LID
  ***
  ***  Module: TCM
  ***
  ***  Description:  
  ***
  ***  Maintainer: Danny Liu
  ***
  ***********************************************************************/

LID_BIT VER_LID(NVRAM_EF_TCM_PDP_PROFILE_LID) nvram_ef_tcm_PDP_profile_record_struct * NVRAM_EF_TCM_PDP_PROFILE_TOTAL
{
   context_id: "Context identifier" {};
   nsapi: "NSAPI" {};
   pdp_addr_type: "PDP address type" {};
   pdp_addr_len: "PDP address length" {};
   addr_val: "Static PDP address value, in MSB" {};

   req_qos: "Requested QoS" ;
      req_qos.qos_length: "Length of Qos" {};
      req_qos.unused1: "Unused Byte" {};
      req_qos.delay_class: "Delay class" {};
      req_qos.reliability_class: "Reliability class" {};
      req_qos.peak_throughput: "Peak throughput" {};
      req_qos.unused2: "Unused Byte" {};
      req_qos.precedence_class: "Precedence class" {};
      req_qos.unused3: "Unused Byte" {};
      req_qos.mean_throughput: "Mean throughput" {};
      req_qos.traffic_class: "Traffic class" {};
      req_qos.delivery_order: "Delivery order" {};
      req_qos.delivery_of_err_sdu: "Delivery of error SDU" {};
      req_qos.max_sdu_size: "Maximum SDU size" {};
      req_qos.max_bitrate_up_lnk: "Max bitrate for uplink" {};
      req_qos.max_bitrate_down_lnk: "Max bitrate for downlink" {};
      req_qos.residual_bit_err_rate: "Residual bit error rate" {};
      req_qos.sdu_err_ratio: "SDU error ratio" {};
      req_qos.transfer_delay: "Transfer delay" {};
      req_qos.traffic_hndl_priority: "Traffic handle priority" {};
      req_qos.guarntd_bit_rate_up_lnk: "Guaranteed bitrate for uplink" {};
      req_qos.guarntd_bit_rate_down_lnk: "Guaranteed bitrate for downlink" {};

   min_qos: "Minimum QoS requirement" {};
      min_qos.qos_length: "Length of Qos" {};
      min_qos.unused1: "Unused Byte" {};
      min_qos.delay_class: "Delay class" {};
      min_qos.reliability_class: "Reliability class" {};
      min_qos.peak_throughput: "Peak throughput" {};
      min_qos.unused2: "Unused Byte" {};
      min_qos.precedence_class: "Precedence class" {};
      min_qos.unused3: "Unused Byte" {};
      min_qos.mean_throughput: "Mean throughput" {};
      min_qos.traffic_class: "Traffic class" {};
      min_qos.delivery_order: "Delivery order" {};
      min_qos.delivery_of_err_sdu: "Delivery of error SDU" {};
      min_qos.max_sdu_size: "Maximum SDU size" {};
      min_qos.max_bitrate_up_lnk: "Max bitrate for uplink" {};
      min_qos.max_bitrate_down_lnk: "Max bitrate for downlink" {};
      min_qos.residual_bit_err_rate: "Residual bit error rate" {};
      min_qos.sdu_err_ratio: "SDU error ratio" {};
      min_qos.transfer_delay: "Transfer delay" {};
      min_qos.traffic_hndl_priority: "Traffic handle priority" {};
      min_qos.guarntd_bit_rate_up_lnk: "Guaranteed bitrate for uplink" {};
      min_qos.guarntd_bit_rate_down_lnk: "Guaranteed bitrate for downlink" {};


   apn_len: "APN length" {};
   apn: "APN string";
   pcomp_algo: "Protocol (header) compression algorithm" {};
   dcomp_algo: "Data compression algorithm" {};
   context_type: "Padding" {};
   primary_context_id: "Primary context identifier" {};
   is_sib_defined: "SIB defination flag" {};
   dns_val: "DNS" {};
   is_chap: "CHAP Authentication" {};
   name_length: "Name Length" {};
   name_dcs: "Name DCS" {};
   name: "Name" {};
   user_name: "Username" {};
   password: "Password" {};
   reserved: "Reserved" {};

};

LID_BIT VER_LID(NVRAM_EF_CAMERA_PARA_LID)   nvram_camera_para_struct * NVRAM_EF_CAMERA_PARA_TOTAL{};
	
LID_BIT VER_LID(NVRAM_EF_WPSS_FILENAME_LID)   nvram_wpss_filename_struct * NVRAM_EF_WPSS_FILENAME_TOTAL{};

LID_BIT VER_LID(NVRAM_EF_AUTOTEST_LID) nvram_ef_autotest_struct * NVRAM_EF_AUTOTEST_TOTAL {};
	
// Lisen 0813
LID_BIT VER_LID(NVRAM_EF_PORT_SETTING_LID) nvram_ef_port_setting_struct * NVRAM_EF_PORT_SETTING_TOTAL
{
   tst_port: "TST port";
   ps_port: "PS port";
   tst_baudrate: "TST baudrate";
   ps_baudrate: "PS baudrate";   
   high_speed_sim_enabled: "high_speed_sim_enabled";
//Lisen 05032005	Begin   
   swdbg: "SW Debug setting";
//Lisen 05032005	End   
};

LID_BIT VER_LID(NVRAM_EF_STOPWATCH_LID) nvram_ef_stopwatch_struct * NVRAM_EF_STOPWATCH_RECORD_TOTAL {};

LID_BIT VER_LID(NVRAM_PICTURE_EDITOR_LID) nvram_picture_editor_struct * NVRAM_PICTURE_EDITOR_TOTAL{};

LID_BIT VER_LID(NVRAM_EF_MSG_CLUB_LID) nvram_msg_club_struct * NVRAM_MSG_CLUB_RECORD_TOTAL {};

LID_BIT VER_LID(NVRAM_EF_MSG_CLUB_NUM_LID) nvram_msg_club_num_struct * NVRAM_MSG_CLUB_NUM_RECORD_TOTAL {};



LID_BIT VER_LID(NVRAM_EF_BLACK_LIST_LID) nvram_ef_black_list_struct* NVRAM_EF_BLACK_LIST_TOTAL {};

LID_BIT VER_LID(NVRAM_EF_AUDIO_EQUALIZER_LID) nvram_ef_audio_equalizer_struct* NVRAM_EF_AUDIO_EQUALIZER_TOTAL
{
   count: "Number of Equalizer Settings";
   index: "Selected Equalizer Settings Index";
   magnitude: "Equalizer Settings Data";
};

LID_BIT VER_LID(NVRAM_EF_PHB_LN_TYPE_SEQ_LID) nvram_ef_phb_ln_type_seq_struct* NVRAM_EF_PHB_LN_TYPE_SEQ_TOTAL {};
LID_BIT VER_LID(NVRAM_EF_PHB_COMPARE_DIGIT_LID) kal_uint8 * NVRAM_EF_PHB_COMPARE_DIGIT_TOTAL {};

LID_BIT VER_LID(NVRAM_EF_PHB_BIRTHDAY_LID) nvram_ef_phb_birthday_struct* NVRAM_EF_PHB_BIRTHDAY_TOTAL {};
LID_BIT VER_LID(NVRAM_EF_CB_DEFAULT_CH_LID) nvram_ef_cb_default_ch_struct * NVRAM_EF_CB_DEFAULT_CH_TOTAL
{
   cbmi: "Default CB Channel ID";
};


 
LID_BIT VER_LID(NVRAM_EF_EONS_INFO_LID) kal_uint8* NVRAM_EF_EONS_INFO_TOTAL {};
 

	
END_NVRAM_DATA


#endif /* !__L1_STANDALONE__ */
#endif
#endif /* GEN_FOR_PC */

#endif /* _CUSTOM_NVRAM_EDTIOR_DATA_ITEM_H */

