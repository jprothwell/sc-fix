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
 *	nvram_user_defs.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is for customers to config/customize their parameters to NVRAM Layer and
 *   Driver Layer.
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
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/



#ifndef _NVRAM_USER_DEFS_H
#define _NVRAM_USER_DEFS_H
#ifndef MMI_ON_HARDWARE_P
#include "nvramenum.h"
#endif
#ifdef MMI_ON_HARDWARE_P

#ifdef __cplusplus
extern "C" {
#endif
#include "kal_release.h"
/* Header files should be include before this one */
#ifndef _KAL_RELEASE_H
#error "kal_release.h should be included"
#endif


#include "mmi_features.h"  //simon changed

#include "sms_common_setting.h" 

/**
 * Here user defines their constants for filling each data item into
 * `logical_data_item_table_cust' in `nvram_user_config.c'
 */

/**
 * Customized logical data item ID's.
 *
 * **** Lower boud is the definition: NVRAM_TOTAL_LID_VENDOR. ****
 * (DO NOT take away (), or precedence problem will happen.)
 */
#include "nvram_data_items.h"
#define NVRAM_LID_CUST_BEGIN     NVRAM_EF_LAST_LID_VENDOR

/**
 * Note:
 * This enumeration must begin from NVRAM_LID_CUST_BEGIN!!!
 * Besides, there must be at least one user-defined logical data item.
 */
typedef enum
{
    NVRAM_EF_SMS_LID = NVRAM_LID_CUST_BEGIN,
    NVRAM_EF_SMS_CB_CHNL_LID,
    NVRAM_EF_SMS_CB_SMS_LID,
    NVRAM_EF_SMS_CB_SMS_INFO_LID,
    NVRAM_EF_PHB_IDS_LID,
    NVRAM_EF_PHB_SETTINGS_LID,
    NVRAM_EF_PHB_CALLER_GROUPS_LID,
    NVRAM_EF_PROFILES_LID,
    NVRAM_EF_GROUPLIST_LID,
    NVRAM_EF_CALL_TIME_LID,
    NVRAM_EF_CALL_COST_GET_MAX_LID,
    NVRAM_EF_CALL_COST_GET_ALL_COST_LID,
    NVRAM_EF_CALL_COST_GET_LAST_COST_LID,
    NVRAM_EF_CALL_COST_GET_CURRENCY_LID,
    NVRAM_EF_CALL_COST_GET_PPU_LID,
    NVRAM_EF_BT_INFO_LID,
    NVRAM_EF_ALM_QUEUE_LID,
    NVRAM_EF_SYS_CACHE_OCTET_LID,
    NVRAM_EF_CAMERA_PARA_LID,
    NVRAM_EF_WPSS_FILENAME_LID,
    NVRAM_EF_AUTOTEST_LID,
    NVRAM_EF_FMGR_FILEPATH_POOL_HEAD_LID,
    NVRAM_EF_FMGR_FILEPATH_POOL_TAIL_LID,
    NVRAM_EF_ALM_SPOF_DATA_LID,
    NVRAM_EF_PHB_COMPARE_DIGIT_LID,
    NVRAM_EF_CAMERA_SETTING_LID,
    NVRAM_EF_VIDEO_SETTING_LID,
    NVRAM_EF_CUST_ACOUSTIC_DATA_LID,
    NVRAM_EF_AUDIO_PARAM_LID,
    NVRAM_EF_BARCODE_NUM_LID,
    
#ifdef __MMI_NOTEPAD__
    NVRAM_EF_NOTEPAD_LIST_LID,
    NVRAM_EF_NOTEPAD_RECORD_LID,
#endif

#ifdef __GPRS_MODE__
    NVRAM_EF_CSD_PROFILE_LID,
    NVRAM_EF_ABM_GPRS_PROFILE_LID,
#endif

#ifdef __IP_NUMBER__
    NVRAM_EF_IP_NUMBER_LID,
#endif

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
    NVRAM_EF_EXT_MELODY_INFO_LID,
#endif

#if defined(__MMI_VCARD__)
    NVRAM_EF_PHB_VCARD_LID,
#endif

#ifdef __MMI_STOPWATCH__
    NVRAM_EF_STOPWATCH_LID,
#endif

#ifdef __MMI_MESSAGES_CHAT__
    NVRAM_EF_CHAT_ROOM_INFO_LID,
#endif

#ifdef __MMI_MESSAGES_CLUB__
    NVRAM_EF_MSG_CLUB_NUM_LID,
#endif

#ifdef __MMI_CM_BLACK_LIST__
    NVRAM_EF_BLACK_LIST_LID,
#endif

#ifdef __MMI_SOUND_EFFECT__
    NVRAM_EF_AUDIO_EQUALIZER_LID,
#endif

#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
    NVRAM_EF_PHB_BIRTHDAY_LID,
#endif

#ifdef __MMI_FM_RADIO__
    NVRAM_EF_FM_RADIO_LID,
#endif

#if defined(__MMI_FM_RADIO_SCHEDULE_REC__)
    NVRAM_EF_FMSR_SETTING_LID,
#endif

#ifdef __MMI_PHOTOEDITOR__
    NVRAM_EF_PHOTO_EDITOR_SETTING_LID,
#endif

#ifdef __MMI_INCOMING_CALL_VIDEO__
    NVRAM_EF_PHB_VIDEO_LID,
#endif

#ifdef JATAAYU_SUPPORT
    NVRAM_EF_WAP_PROFILE_CONTENT_LID,

#ifdef __MMI_MULTI_SIM__
    NVRAM_EF_WAP_PROFILE_CONTENT_SIM2_LID,
    NVRAM_EF_WAP_PROFILE_CONTENT_SIM3_LID,
    NVRAM_EF_WAP_PROFILE_CONTENT_SIM4_LID,
#endif

    NVRAM_EF_MMS_PROFILE_CONTENT_LID,

#ifdef __MMI_MULTI_SIM__
    NVRAM_EF_MMS_PROFILE_CONTENT_SIM2_LID,
    NVRAM_EF_MMS_PROFILE_CONTENT_SIM3_LID,
    NVRAM_EF_MMS_PROFILE_CONTENT_SIM4_LID,
#endif

    NVRAM_EF_WAP_PROFILE_NAMES_LID,

#ifdef __MMI_MULTI_SIM__
    NVRAM_EF_WAP_PROFILE_NAMES_SIM2_LID,
    NVRAM_EF_WAP_PROFILE_NAMES_SIM3_LID,
    NVRAM_EF_WAP_PROFILE_NAMES_SIM4_LID,
#endif

    NVRAM_EF_MMS_PROFILE_NAMES_LID,

#ifdef __MMI_MULTI_SIM__
    NVRAM_EF_MMS_PROFILE_NAMES_SIM2_LID,
    NVRAM_EF_MMS_PROFILE_NAMES_SIM3_LID,
    NVRAM_EF_MMS_PROFILE_NAMES_SIM4_LID,
#endif

#endif/*JATAAYU_SUPPORT*/

#ifdef __MMI_EBOOK_READER__ 
    NVRAM_EF_EBOOK_SETTINGS_LID,
#endif

#if defined( __MMI_ANALOGTV__)	
    NVRAM_EF_ATV_PLAYER_SETTINGS_LID,
    NVRAM_EF_ATV_PLAYER_CHANNEL_LID,
#endif

#ifdef __MMI_NITZ__
    NVRAM_EF_NITZ_NW_NAME_LID,
#endif

#if defined(CSD_HW_LEVEL_NVRAM)
    NVRAM_EF_CUST_HW_LEVEL_TBL_LID,
#endif

#if defined(CSD_PHB_EXT_NVRAM)
    NVRAM_EF_PHB_LID,
    NVRAM_EF_PHB_LN_ENTRY_LID,
#endif

#if defined(SHORTCUTS_APP)
    NVRAM_EF_SHORTCUTS_LID,
#endif

#if defined(__DOWNLOAD__)    
    NVRAM_EF_EMS_MY_PICTURE_NAME_LID,
#endif

#if defined(__MMI_IMELODY_SUPPORT__)
    NVRAM_EF_EMS_MY_ANIMATION_NAME_LID,
    NVRAM_EF_EMS_MY_MELODY_NAME_LID,
#endif

#ifdef __MMI_TODOLIST__
    NVRAM_EF_TODO_LIST_LID,
#endif

#ifndef __MMI_THEMES_V2_SUPPORT__     
    NVRAM_EF_THEMES_VALUES,
#endif    
#if defined(__PHB_CALLER_IMAGE_SAVE_PATH_ONLY__)	
	NVRAM_EF_PHB_PIC_LID,
#endif	   
#ifdef __MMI_VIP_FUNCTION__
	NVRAM_EF_VIP_FUNCTION_LID,
#endif

#if defined(CSD_NO_SLIM_NVRAM)	
    NVRAM_EF_CALORIE_DATA_LID,
    NVRAM_EF_DOWNLOAD_IMAGE_LID,
    NVRAM_EF_COMPOSE_RINGTONE_LID,
    NVRAM_EF_DOWNLOAD_TONE_LID,
    NVRAM_EF_DOWNLOAD_MELODY_LID,
    NVRAM_EF_ROOT_CA_LID,
    NVRAM_EF_IMEI_IMEISV_LID,
    NVRAM_EF_CLASSMARK_RACAP_LID,
    NVRAM_EF_ADC_LID,
    NVRAM_EF_UEM_MANUFACTURE_DATA_LID,
    NVRAM_EF_UEM_RMI_DATA_LID,
    NVRAM_EF_SMSAL_MWIS_LID,
    NVRAM_EF_SMSAL_SMS_LID,
    NVRAM_EF_TCM_PDP_PROFILE_LID,
    NVRAM_EF_PORT_SETTING_LID,
    NVRAM_PICTURE_EDITOR_LID,
    NVRAM_EF_MSG_CLUB_LID,
    NVRAM_EF_IMPS_LID,
    NVRAM_EF_PHB_LN_TYPE_SEQ_LID,
    NVRAM_EF_EONS_INFO_LID,
#endif

    NVRAM_EF_LINESWITCH_LID,//man for lineswitch
    NVRAM_EF_LINEFORWARD_LID,//man for forward icon
    NVRAM_EF_PHB_BLACK_AND_RED_LIST,//chenhe for black and red list
    NVRAM_EF_BAND_SELECT_AUTO,
#ifdef __MMI_MULTI_SIM__
    NVRAM_EF_CARD2_BAND_SELECT_AUTO,
    NVRAM_EF_CARD3_BAND_SELECT_AUTO,
    NVRAM_EF_CARD4_BAND_SELECT_AUTO,
#endif
    NVRAM_EF_CALIBRATION_SET,
    NVRAM_EF_RDA_SETTING_LID,//licheng for rda bt
#ifdef __MMI_A2DP_SUPPORT__
    NVRAM_EF_AUDIO_PLAYER_BT_SETTINGS_LID,
#endif

#if defined(__MMI_MESSAGES_BLACK_LIST__)
	NVRAM_EF_MESSAGES_BLACK_LIST_LID,
#endif
#if defined(__MMI_SAVE_CURRENT_TIME__)
	NVRAM_EF_SAVE_CURRENT_TIME_LID,
#endif
#ifdef __MMI_TTS_FEATURES__
	NVRAM_SETTING_TTS_LID,
#endif

/*Don't remove this line: insert LID definition above*/
   /* DO NOT MODIFY THIS. */
   NVRAM_EF_LAST_LID_CUST
} nvram_LID_cust_enum;
/*MS unique ID*/
#define UNI_ID   NVRAM_EF_IMEI_IMEISV_LID

/**
 * For each logical data item, defines its constants of `size' and `number of records'
 * For linear-fixed, TOTAL is greater than 1, and SIZE is size of each record;
 * for transparent, TOTAL must be exaclty 1, and SIZE is size of entire data item.
 *
 * Each logical data item must be:
 * Size of default value must be equal to the logical data item's size.
 */

/************************************
 * Custom MMI Define Start
 ************************************/

#define NVRAM_EF_CUST_SIZE             16
#define NVRAM_EF_CUST_TOTAL            2

#define NVRAM_DOWNLOAD_IMAGE_RECORD_SIZE		40
#define NVRAM_DOWNLOAD_IMAGE_RECORD_TOTAL		1

#define NVRAM_DOWNLOAD_TONE_RECORD_SIZE		40
#define NVRAM_DOWNLOAD_TONE_RECORD_TOTAL		1

#define NVRAM_DOWNLOAD_MELODY_RECORD_SIZE		40
#define NVRAM_DOWNLOAD_MELODY_RECORD_TOTAL	1

#define NVRAM_EF_TEST_1_SIZE           		10
#define NVRAM_EF_TEST_1_TOTAL          		2

#define NVRAM_EF_PHONEBOOK_SIZE			16
#define NVRAM_EF_PHONEBOOK_TOTAL			2

#define NVRAM_PHB_SETTINGS_SIZE			20		/*Phonebook may use less setting fields than this value*/
#define NVRAM_PHB_SETTINGS_TOTAL			1

#define NVRAM_DOWNLOAD_RECORD_SIZE		(390+2)
#define NVRAM_DOWNLOAD_RECORD_TOTAL	2

#define NVRAM_THEMES_VALUES_SIZE			8
#define NVRAM_THEMES_VALUES_RECORDS		1

#define NVRAM_MAX_GROUP_NAME_SIZE		36
#define NVRAM_MAX_GROUP_NAME_TOTAL		12

#define NVRAM_COMPOSE_RINGTONE_TOTAL 	1
#define NVRAM_COMPOSE_RINGTONE_SIZE  	40*5

#define NVRAM_IP_NUMBER_TOTAL 			1
#define NVRAM_IP_NUMBER_SIZE 				22*3

#define NVRAM_CALORIE_DATA_SIZE			560
#define NVRAM_CALORIE_DATA_TOTAL			1

#define NVRAM_SHORTCUTS_SIZE				250
#define NVRAM_SHORTCUTS_TOTAL			2


#define NVRAM_SMS_CB_CHNL_RECORD_SIZE	180
#define NVRAM_SMS_CB_CHNL_RECORD_TOTAL	1

#define NVRAM_SMS_CB_SMS_RECORD_SIZE	826 // 824
#define NVRAM_SMS_CB_SMS_RECORD_TOTAL	1

#define MAX_STORE_CB_MSG						5
#define NVRAM_SMS_CB_SMS_PAGE_SIZE			94
#define NVRAM_SMS_CB_SMS_PAGE_NUM_TOTAL	30
#define NVRAM_SMS_CB_SMS_INFO_SIZE			20
#define NVRAM_SMS_CB_SMS_INFO_TOTAL			1

#define NVRAM_CHAT_ROOM_INFO_RECORD_SIZE 40
#define NVRAM_CHAT_ROOM_INFO_RECORD_TOTAL 2

#define NVRAM_MSG_CLUB_RECORD_SIZE 		8
#define NVRAM_MSG_CLUB_RECORD_TOTAL		1

#define NVRAM_MSG_CLUB_NUM_RECORD_SIZE	4
#define NVRAM_MSG_CLUB_NUM_RECORD_TOTAL	1


#define NVRAM_EMS_MY_PICTURE_NAME_RECORD_SIZE		140
#define NVRAM_EMS_MY_PICTURE_NAME_RECORD_TOTAL	1

#define NVRAM_EMS_MY_ANIMATION_NAME_RECORD_SIZE	 140
#define NVRAM_EMS_MY_ANIMATION_NAME_RECORD_TOTAL	 1

#define NVRAM_EMS_MY_MELODY_NAME_RECORD_SIZE		 140
#define NVRAM_EMS_MY_MELODY_NAME_RECORD_TOTAL	        1

// NVRAM for Data Account Application.
#define NVRAM_DATA_ACCOUNT_RECORD_SIZE		200
#define NVRAM_DATA_ACCOUNT_RECORD_TOTAL	4

// NVRAM for To Do List Application.
#define NVRAM_TODO_LIST_RECORD_SIZE			860
#define NVRAM_TODO_LIST_RECORD_TOTAL			1

#if defined( __MMI_ANALOGTV__)
#define NVRAM_EF_ATV_PLAYER_CHANNEL_TOTAL     30
#define NVRAM_EF_ATV_PLAYER_CHANNEL_SIZE      26 

#define NVRAM_EF_ATV_PLAYER_SETTINGS_TOTAL	1
#define NVRAM_EF_ATV_PLAYER_SETTINGS_SIZE	30
#endif

#define  NVRAM_NITZ_NW_NAME_SIZE      55

#ifdef __MMI_MULTI_SIM__
#define  NVRAM_NITZ_NW_NAME_TOTAL     MMI_SIM_NUMBER
#else /*__MMI_MULTI_SIM__*/
#define  NVRAM_NITZ_NW_NAME_TOTAL     1
#endif /*__MMI_MULTI_SIM__*/

//NVRAM for Notepad Application
#ifdef __MMI_NOTEPAD__
#define NVRAM_NOTEPAD_LIST_SIZE			220
#define NVRAM_NOTEPAD_LIST_TOTAL			1
#define NVRAM_NOTEPAD_RECORD_SIZE			500
#define NVRAM_NOTEPAD_RECORD_TOTAL			10
#endif
//NVRAM for Call Time
#define NVRAM_CALL_TIME_RECORD_SIZE  			8
//added by zhoumn @2006/12/05>>>start 
#define NVRAM_CALL_COST_GET_MAX_RECORD_SIZE	     4    			                                    
#define NVRAM_CALL_COST_GET_ALL_RECORD_SIZE		 6+1+2	                                      
#define NVRAM_CALL_COST_GET_LAST_RECORD_SIZE     4+1+2		                                      
#define NVRAM_CALL_COST_GET_CURRENCY_RECORD_SIZE	   4*ENCODING_LENGTH
#define NVRAM_CALL_COST_GET_PPU_RECORD_SIZE	   20*ENCODING_LENGTH
#define NVRAM_CALL_COST_GET_PPU_RECORD_TOTAL     MMI_SIM_NUMBER
#define NVRAM_CALL_COST_GET_CURRENCY_RECORD_TOTAL     MMI_SIM_NUMBER
#define NVRAM_CALL_COST_GET_LAST_RECORD_TOTAL     MMI_SIM_NUMBER
#define NVRAM_CALL_COST_GET_ALL_RECORD_TOTAL     MMI_SIM_NUMBER
#define NVRAM_CALL_COST_GET_MAX_RECORD_TOTAL     MMI_SIM_NUMBER
#define NVRAM_CALL_TIME_RECORD_TOTAL 		3*MMI_SIM_NUMBER

//added by zhoumn @2006/12/05<<<end
/**
 * ROOT_CA
 */
#define NVRAM_EF_ROOT_CA_SIZE        			1534
#define NVRAM_EF_ROOT_CA_TOTAL       			1

/**
 * CSD profile
 */
#define NVRAM_EF_CSD_PROFILE_SIZE          		1480   /* (148 * 10) */
#define NVRAM_EF_CSD_PROFILE_TOTAL         		1

/**
 * External Melody
 */
#if 0//modified by gdm 081128 fix bug10498,  sizeof(gPmgExtMelodyInfo)=2636bytes
#define NVRAM_EF_EXT_MELODY_INFO_SIZE      			1748 /*1696*/
#else
#define NVRAM_EF_EXT_MELODY_INFO_SIZE      			2636 
#endif
#define NVRAM_EF_EXT_MELODY_INFO_TOTAL    			1

#define NVRAM_PHONEBOOK_SOS_RECORD_SIZE				86
#define NVRAM_PHONEBOOK_SOS_RECORD_TOTAL			10

#define NVRAM_PHONEBOOK_SPEEDDIAL_RECORD_SIZE		1200
#define NVRAM_PHONEBOOK_SPEEDDIAL_RECORD_TOTAL	1

/**
 * File Manager FilePath Pool Settings
 */
#define NVRAM_EF_FMGR_FILEPATH_POOL_HEAD_TOTAL		4
#define NVRAM_EF_FMGR_FILEPATH_POOL_HEAD_SIZE  		510
#define NVRAM_EF_FMGR_FILEPATH_POOL_TAIL_TOTAL		4
#define NVRAM_EF_FMGR_FILEPATH_POOL_TAIL_SIZE  		10

/**
 * ABM_GPRS_PROFILE
 */

#define NVRAM_EF_ABM_GPRS_PROFILE_TOTAL 				10
#define NVRAM_EF_ABM_GPRS_PROFILE_SIZE  				226

//changes for the Greeting Text
#define NVRAM_SETTING_SIZE								60
#define NVRAM_SETTING_TOTAL							1

#ifdef	__MMI_PHB_NO_OPTIONAL_FIELD__	/*Use very small space for no optional field */
       #define NVRAM_PHB_FIELDS_SIZE				1
#else
       #define NVRAM_PHB_FIELDS_SIZE				229
#endif

#define NVRAM_PHB_FIELDS_TOTAL			PHONE_PHB_ENTRY_COUNT

#define NVRAM_PHB_SIM_TOTAL				MAX_PHB_SIM_ENTRY

#define NVRAM_PHB_IDS_RECORD_COUNT		85
#define NVRAM_PHB_IDS_SIZE					510	/*For 85 entries in one NVRAM record.*/
#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__)
#define NVRAM_PHB_IDS_TOTAL			 	((NVRAM_PHB_FIELDS_TOTAL+NVRAM_PHB_SIM_TOTAL*MMI_SIM_NUMBER -1) /  NVRAM_PHB_IDS_RECORD_COUNT + 1)
#else
#define NVRAM_PHB_IDS_TOTAL				 ((NVRAM_PHB_FIELDS_TOTAL-1) /  NVRAM_PHB_IDS_RECORD_COUNT + 1)
#endif

#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
#define NVRAM_EF_PHB_BIRTHDAY_COUNT	250	/*250 item in each record*/
#define NVRAM_EF_PHB_BIRTHDAY_SIZE		1000	/*4 bytes for each item, 4*250 = 1000*/
#define NVRAM_EF_PHB_BIRTHDAY_TOTAL		((NVRAM_PHB_FIELDS_TOTAL-1) /  NVRAM_EF_PHB_BIRTHDAY_COUNT + 1)
#else
#define NVRAM_EF_PHB_BIRTHDAY_COUNT	1
#define NVRAM_EF_PHB_BIRTHDAY_SIZE		2
#define NVRAM_EF_PHB_BIRTHDAY_TOTAL		1
#endif
#if defined(__PHB_CALLER_IMAGE_SAVE_PATH_ONLY__)
//Phb usr defiend pic path max 63 chars,it need 128*PHONE_PHB_ENTRY_COUNT bytes on NVRAM,example g800 200items need 25KB
#define      NVRAM_EF_PHB_PIC_SIZE             128*PHONE_PHB_ENTRY_COUNT                                        
#define      NVRAM_EF_PHB_PIC_TOTAL         1
#endif
#define NVRAM_PHB_CALLER_GROUPS_SIZE		(MAX_PB_CALLER_GROUPS*sizeof(PHB_CALLER_GROUP_STRUCT))//260 //Renwy modify for bug 8919
#define NVRAM_PHB_CALLER_GROUPS_TOTAL	 	1

/**
 * IMEI/IMEISV
 */
#define NVRAM_EF_IMEI_IMEISV_SIZE         10
#define NVRAM_EF_IMEI_IMEISV_TOTAL        1

#if defined(__MMI_FM_RADIO_SCHEDULE_REC__)
#define NVRAM_ALM_QUEUE_SIZE		230
#else
#define NVRAM_ALM_QUEUE_SIZE		190
#endif
#define	NVRAM_ALM_QUEUE_TOTAL		1

#define NVRAM_ALM_SPOF_DATA_SIZE        250
#define NVRAM_ALM_SPOF_DATA_TOTAL	2

/**
 * IMSI
 */

/**
 
 */
#define NVRAM_EF_CLASSMARK_RACAP_SIZE          12
#define NVRAM_EF_CLASSMARK_RACAP_TOTAL         1

/**
 * MS security
 */
#define NVRAM_EF_MS_SECURITY_SIZE         156
#define NVRAM_EF_MS_SECURITY_TOTAL        1

/**
 * ADC
 */
#ifdef MT6205B
#define NVRAM_EF_ADC_SIZE        40
#else

 #if ( defined(MT6208_CEVB) || defined(MT6208_EVB) )
  #define NVRAM_EF_ADC_SIZE        40
 #else
#define NVRAM_EF_ADC_SIZE        56
#endif
#endif

#define NVRAM_EF_ADC_TOTAL      1

/**
 * Serial Number
 */
#define NVRAM_EF_BARCODE_NUM_SIZE       64
#define NVRAM_EF_BARCODE_NUM_TOTAL    1

/**
 * NVRAM_EF_PHB_VCARD_LID
 */
#define NVRAM_PHB_VCARD_LID_SIZE        338
#define NVRAM_PHB_VCARD_LID_TOTAL       1


/**
 * System Cache OCTET : This is a special NVRAM data item used for storage purpose.
 *                                   Please note that the default value is ALWAYS 0x00
 */
#define NVRAM_EF_SYS_CACHE_OCTET_SIZE        8
#define NVRAM_EF_SYS_CACHE_OCTET_TOTAL     1

typedef enum
{
   NVRAM_SYS_AUTO_DETECT=1
} NVRAM_SYS_CACHE_OCTETDATA;

/**
 * PHB
 */
#define NVRAM_EF_PHB_SIZE                  86

#define NVRAM_EF_PHB_TOTAL                PHONE_PHB_ENTRY_COUNT


/**
 * PHB LN entry
 */
#if defined(__L4_MAX_NAME_60__)
#define NVRAM_EF_PHB_LN_ENTRY_SIZE     1142
#elif defined(__L4_MAX_NAME_20__)
#define NVRAM_EF_PHB_LN_ENTRY_SIZE      742
#else
#define NVRAM_EF_PHB_LN_ENTRY_SIZE      842
#endif
#if (MAX_PHB_LN_ENTRY > 20)
#define NVRAM_EF_PHB_LN_ENTRY_TOTAL    (((MAX_PHB_LN_ENTRY+9)/10)*3)
#else
#define NVRAM_EF_PHB_LN_ENTRY_TOTAL    6
#endif

// define NVRAM_EF_PHB_LN_TYPE_SEQ_SIZE in custom_nvram_editor_data_item.h
#define NVRAM_EF_PHB_LN_TYPE_SEQ_TOTAL 1

#define NVRAM_EF_PHB_COMPARE_DIGIT_SIZE 1
#define NVRAM_EF_PHB_COMPARE_DIGIT_TOTAL 1

/**
 * UEM Manufacturer data
 */
#define NVRAM_EF_UEM_MANUFACTURE_DATA_SIZE         210
#define NVRAM_EF_UEM_MANUFACTURE_DATA_TOTAL        1

/**
 * UEM RMI data
 */
#define NVRAM_EF_UEM_RMI_DATA_SIZE                 216 //182
#define NVRAM_EF_UEM_RMI_DATA_TOTAL                1

/**
 * SMSAL common parameters
 */
#define NVRAM_EF_SMSAL_COMMON_PARAM_SIZE           12
#define NVRAM_EF_SMSAL_COMMON_PARAM_TOTAL          1

/**
 * SMSAL message waiting indication status
 */
#define NVRAM_EF_SMSAL_MWIS_SIZE           6
#define NVRAM_EF_SMSAL_MWIS_TOTAL          2

/**
 * SMSAL mailbox addresses
 */
#define NVRAM_EF_SMSAL_MAILBOX_ADDR_SIZE           24
#define NVRAM_EF_SMSAL_MAILBOX_ADDR_TOTAL          2

/**
 * SMSAL short messages
 */
#define NVRAM_EF_SMSAL_SMS_SIZE              176
#define NVRAM_EF_SMSAL_SMS_TOTAL             50


/**
 * CB Default Channel Setting
 */
#define NVRAM_EF_CB_DEFAULT_CH_SIZE          20
#define NVRAM_EF_CB_DEFAULT_CH_TOTAL         1
/**
 * CHIS Default NUMBER Setting
 */
#define NVRAM_EF_CHIS_NUM_SIZE          1
#define NVRAM_EF_CHIS_NUM_TOTAL         1
/**
 * CHIS Unread missed call NUMBER Setting
 */
#define NVRAM_EF_UNREAD_MISSED_NUM_SIZE          8
#define NVRAM_EF_UNREAD_MISSED_NUM_TOTAL         2

/**
 * RAC preference
 */
#define NVRAM_EF_RAC_PREFERENCE_SIZE         2
#define NVRAM_EF_RAC_PREFERENCE_TOTAL        1

/**
 * TCM PDP profile
 */
#define NVRAM_EF_TCM_PDP_PROFILE_SIZE          228
#define NVRAM_EF_TCM_PDP_PROFILE_TOTAL       11

/**
 * Camera parameters for driver layers
 */
#define NVRAM_EF_CAMERA_PARA_SIZE		400
#define NVRAM_EF_CAMERA_PARA_TOTAL		1


/**
 * Wallpaper and Screensaver file name
 */
#define NVRAM_EF_WPSS_FILENAME_SIZE		1224
#define NVRAM_EF_WPSS_FILENAME_TOTAL		3  

#define   NVRAM_EF_AUTOTEST_SIZE           250
#define   NVRAM_EF_AUTOTEST_TOTAL        2

/**
 * Port Setting
 */
#define NVRAM_EF_PORT_SETTING_SIZE         16
#define NVRAM_EF_PORT_SETTING_TOTAL        1

//GPRS MMI NVRAM for Stopwatch
#ifdef __UCS2_ENCODING
	#define NVRAM_EF_STOPWATCH_RECORD_SIZE  1152
#else
	#define NVRAM_EF_STOPWATCH_RECORD_SIZE  980
#endif
#define NVRAM_EF_STOPWATCH_RECORD_TOTAL	1

#define NVRAM_PICTURE_EDITOR_TOTAL 1
#define NVRAM_PICTURE_EDITOR_SIZE  40*5

#define NVRAM_EF_IMPS_SIZE      922
#define NVRAM_EF_IMPS_TOTAL     1

/*
 * Audio Equalizer Settings
 */
#define NVRAM_EF_AUDIO_EQUALIZER_COUNT		8
#define NVRAM_EF_AUDIO_EQUALIZER_SIZE		68
#define NVRAM_EF_AUDIO_EQUALIZER_TOTAL		1

#define NVRAM_EF_BLACK_LIST_SIZE		42*5
#define NVRAM_EF_BLACK_LIST_TOTAL		1


#define MAX_MESSAGES_BLACK_LIST_NUMBER	5
#define MAX_MESSAGES_BLACK_LIST_NUMBER_LEN	20

#define NVRAM_EF_MESSAGES_BLACK_LIST_SIZE		((MAX_MESSAGES_BLACK_LIST_NUMBER_LEN + 1)*ENCODING_LENGTH*MAX_MESSAGES_BLACK_LIST_NUMBER)
#define NVRAM_EF_MESSAGES_BLACK_LIST_TOTAL		1

#define NVRAM_EF_SAVE_CURRENT_TIME_SIZE	7
#define NVRAM_EF_SAVE_CURRENT_TIME_TOTAL	1


 
#define NVRAM_EF_EONS_INFO_SIZE		96
#define NVRAM_EF_EONS_INFO_TOTAL		1

/*for fmradio setttings:shizh*/

#define NVRAM_EF_FM_RADIO_TOTAL 1
#define NVRAM_EF_FM_RADIO_SIZE  29*44  //change channel number from 23 to 29

/*
 * Schedule FM Radio Settings  :jiashuo add for fm schedule record
 */
#define NVRAM_EF_FMSR_SETTING_SIZE		300
#define NVRAM_EF_FMSR_SETTING_TOTAL		4


/*
  * Bluetooth status  //renwy add for BT 2008/07/04
  */
#define NVRAM_EF_BT_INFO_SIZE        2200
#define NVRAM_EF_BT_INFO_TOTAL     1

 #ifdef __MMI_TTS_FEATURES__
#define  MAX_TTS_CODE           		20 

#ifdef __ASCII
#define ENCODING_LENGTH       1
#endif 

#ifdef __UCS2_ENCODING
#define ENCODING_LENGTH       2
#endif 

typedef struct
{
//	kal_uint8 gttsRingKnownNumberCallCustom[(MAX_TTS_CODE + 1) * ENCODING_LENGTH];
//	kal_uint8 gttsRingUnKnownNumberCallCustom[(MAX_TTS_CODE + 1) * ENCODING_LENGTH];
//   kal_uint8 gttsRingKnownNumberMsgCustom[(MAX_TTS_CODE + 1) * ENCODING_LENGTH];
//	kal_uint8 gttsRingUnKnownNumberMsgCustom[(MAX_TTS_CODE + 1) * ENCODING_LENGTH];
/*这里是语音合成*/
//	kal_uint8 curTTSAlertMissCallStatus;
//	kal_uint8 curTTSAlertMissMsgStatus;

//	kal_uint8 curTTSAlertToDoListStatus;
	
	kal_uint8 curTTSAlertAlarmAutoStatus;
	kal_uint8 curTTSAlertHourStatus;
	kal_uint8 curTTSAlertWeekStatus;	
	kal_uint8 curTTSAlertDateStatus;
/*语音播报*/	
	kal_uint8 curTTSRingIncommingCallStatus;
//	kal_uint8 curTTSRingKnownNumberCallStatus;
	//kal_uint8 curTTSRingUnKnownNumberCallStatus;

//	kal_uint8 curTTSRingIncommingMsgStatus;
//	kal_uint8 curTTSRingKnownNumberMsgStatus;
//	kal_uint8 curTTSRingUnKnownNumberMsgStatus;
	
 /*菜单朗读*/
	kal_uint8 curTTSMenuReadStatus;	
	   
/*短信朗读*/	
 	kal_uint8 curTTSMsgReadStatus;

	kal_uint8 curTTSVolumeStatus;
	kal_uint8 curTTSSpeedStatus;
	kal_uint8 curTTSEffectStatus;
	kal_uint8 curTTSStyleStatus;
	kal_uint8 curTTSTuneStatus;


/*合成*/
//	kal_uint8 curASRStatus;
//	kal_uint8 curCallSim;        
//	kal_uint8 curAutoDibblerMusicStatus;


}nvram_tts_setting_struct;
#endif
#ifdef __MMI_TTS_FEATURES__
#define NVRAM_SETTING_TTS_SIZE     sizeof(nvram_tts_setting_struct)
#define NVRAM_SETTING_TTS_TOTAL    1
#define NVRAM_SETTING_TTS_LID_VERNO               "123"              
#endif

#ifdef __MMI_VIP_FUNCTION__
#define MAX_NUMBER_VIP  10
#define MIN_VIP_SETTING_PASSWD_LEN 6
#define MAX_VIP_SETTING_PASSWD_LEN 8
#ifndef MAX_PB_NUMBER_LENGTH
#define MAX_PB_NUMBER_LENGTH 40
#endif

typedef struct
{
	BOOL status;
	U8 VIPNumber[MAX_NUMBER_VIP][(MAX_PB_NUMBER_LENGTH+1)*ENCODING_LENGTH];
	U8 Password[(MAX_VIP_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH];
}vip_nvram_data_struct;

#define NVRAM_EF_VIP_FUNCTION_SIZE sizeof(vip_nvram_data_struct)
#define NVRAM_EF_VIP_FUNCTION_TOTAL 1
#endif
/*Don't remove this line: insert size and total definition above*/


 
 
#define NVRAM_EF_CUSTPACK_BASE 200
typedef enum
{
    NVRAM_EF_SETTING_LID = NVRAM_EF_CUSTPACK_BASE,
    NVRAM_EF_CACHE_BYTE_LID,
    NVRAM_EF_CACHE_SHORT_LID,
    NVRAM_EF_CACHE_DOUBLE_LID,
    NVRAM_EF_PHONEBOOK_SOS_LID,
#ifdef __MMI_MULTI_SIM__
    NVRAM_EF_PHONEBOOK_SIM2_SOS_LID,
    NVRAM_EF_PHONEBOOK_SIM3_SOS_LID,
    NVRAM_EF_PHONEBOOK_SIM4_SOS_LID,
#endif
    NVRAM_EF_PHONEBOOK_SPEEDDIAL_LID,
    NVRAM_EF_CHIS_NUM_LID,
    NVRAM_EF_UNREAD_MISSED_NUM_LID,
#if defined(CSD_NO_SLIM_NVRAM)	 
    NVRAM_EF_CUSTPACK_VERNO_LID,
    NVRAM_EF_SMSAL_MAILBOX_ADDR_LID,
    NVRAM_EF_MS_SECURITY_LID,
    NVRAM_EF_ALS_LINE_ID_LID,
    NVRAM_EF_MSCAP_LID,
    NVRAM_EF_RAC_PREFERENCE_LID,
    NVRAM_EF_SMSAL_COMMON_PARAM_LID,
    NVRAM_EF_CB_DEFAULT_CH_LID,
#endif

#ifdef __MMI_TOUCH_SCREEN__
    NVRAM_EF_TOUCH_PANEL_SETTING_LID,
#endif
    NVRAM_EF_LAST_LID_CUSTPACK
}NVRAM_LID_CUSTPACK_ENUM;
 


/************************************
 * Custom Pack Define Start
 ************************************/

/**
 *	Custom Pack
 **/
#define NVRAM_EF_CUSTPACK_VERNO_SIZE			34
#define NVRAM_EF_CUSTPACK_VERNO_TOTAL			1

 
typedef struct{
	kal_uint8 const* version;
	kal_uint8 const*
	custpack_default[NVRAM_EF_LAST_LID_CUSTPACK-NVRAM_EF_CUSTPACK_BASE-1];
}custpack_nvram_header;
 

//extern const custpack_nvram_header custpack_nvram_ptr;
 


//#ifdef __RESET_FACTORY__
#define NVRAM_MAX_CLEAN_FOLDERS 5
//#endif
/********************************************************
 * The following distiguish the target and the PC environment
 ********************************************************/

#ifdef MMI_ON_HARDWARE_P //target

#define NVRAM_CACHE_SIZE					256	/*For Cache file*/
#define NVRAM_CACHE_TOTAL					1	/*For Cache file*/

#define NVRAM_PROFILES_RECORD_SIZE		308
#define NVRAM_PROFILES_RECORD_TOTAL		2

#else  // ######### for windows #########

#define NVRAM_CACHE_SIZE					256	/*For Cache file*/
#define NVRAM_CACHE_TOTAL					3	/*For Cache file*/

#endif  //for ifdef MMI_ON_HARDWARE_P

//wufasong added 2007.01.08
/**
 * Camera Settings
 */
#define NVRAM_EF_CAMERA_SETTING_TOTAL 					1
#define NVRAM_EF_CAMERA_SETTING_SIZE  					84

#define NVRAM_EF_VIDEO_SETTING_SIZE				51
#define NVRAM_EF_VIDEO_SETTING_TOTAL				1


#define NVRAM_EF_PHB_VIDEO_COUNT					500
#define NVRAM_EF_PHB_VIDEO_SIZE					1000

//photo Editor
#define NVRAM_EF_PHOTO_EDITOR_SETTING_SIZE	200
#define NVRAM_EF_PHOTO_EDITOR_SETTING_TOTAL 	1

#define NVRAM_EF_EBOOK_SETTING_SIZE    93
#define NVRAM_EF_EBOOK_SETTING_TOTAL   1

#ifdef __cplusplus
}
#endif

#endif
#endif //_NVRAM_USER_DEFS_H


