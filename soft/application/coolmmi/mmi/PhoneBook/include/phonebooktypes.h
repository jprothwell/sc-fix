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
 *	PhoneBookTypes.h
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
/************************************************************************
  Change Histroy
  Version      Date             Author    Description
  02.01.001  2007-06-13, Li Tao      add define MAX_SIM_NUMBER_LENGTH and MAX_SIM_Name_LENGTH
  02.01.002  2007-06-21, Li Tao      add define MAX_SIM_NAME_CHINESE_LENGTH 6
 ************************************************************************/
 
#ifndef _PHONEBOOKTYPES_H
#define _PHONEBOOKTYPES_H

#include "mmi_data_types.h"
#include "audioinc.h"
#include "settingprofile.h"
#include "mcd_l4_common.h"

/*The enum decide the order of phonebook fields*/
typedef enum
{
	MMI_PHB_FIELD_NAME,// 0			/*Don't change this order*/
	MMI_PHB_FIELD_NUMBER,// 1			/*Don't change this order*/
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
 	MMI_PHB_FIELD_HOME,// 2
 	MMI_PHB_FIELD_COMPANY_NAME,// 3
 	MMI_PHB_FIELD_EMAIL,// 4
 	MMI_PHB_FIELD_OFFICE,// 5
 	MMI_PHB_FIELD_FAX,// 6
#endif
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)	
 	MMI_PHB_FIELD_BIRTHDAY,// 7
#endif 	
 	MMI_PHB_FIELD_PIC,// 8
#if defined(__MMI_INCOMING_CALL_VIDEO__)	
 	MMI_PHB_FIELD_VIDEO,// 9
#endif 	
 	MMI_PHB_FIELD_RING,// 10
 	MMI_PHB_FIELD_GROUP,// 11
 	MMI_PHB_FIELD_TOTAL// 12
}MMI_PHB_FIELD_ORDER_ENUM;

/* 
** Define
*/
#if defined(__MMI_PHB_MAX_NAME_60__)
#define MAX_PS_NAME_LENGTH		62
#else
#define MAX_PS_NAME_LENGTH		32
#endif

#define MAX_PS_NUMBER_LENGTH	41
#define MAX_ENTRIES_IN_LIST		15
#define MAX_SIM_PASSWD_LEN		 9

#define MAX_PB_SIM_ENTRIES			MAX_PHB_SIM_ENTRY   /*value defined in option.mak*/
#define MAX_PB_PHONE_ENTRIES		PHONE_PHB_ENTRY_COUNT /*value defined in option.mak*/

#define MAX_PB_ENTRIES				(MAX_PB_SIM_ENTRIES*MMI_SIM_NUMBER+ MAX_PB_PHONE_ENTRIES)	/*Maximum number of entries in Phonebook*/

#define MAX_PB_FIELDS				MMI_PHB_FIELD_TOTAL
#define MAX_PB_MANDATORY_FIELDS	2
#define MAX_PB_VIEW_FIELDS			(MMI_PHB_FIELD_TOTAL - MAX_PB_MANDATORY_FIELDS)	/*Exclude mandatory field*/

#define MAX_PB_URL_LENGTH        128
#define MAX_PB_ADDRESS_LENGTH 25
#define MAX_PB_NOTE_LENGTH    30

#if defined(__MMI_INCOMING_CALL_VIDEO__)
#define MAX_PB_VIDEO_TAGS		30           
#define MAX_CALLER_GRP_ITEMS	7
#else
#define MAX_CALLER_GRP_ITEMS	6
#endif
#define MAX_PB_PICTURES_TAGS	30           
#define MAX_PB_OWNER_NUMBERS	 4

#define    MAX_PB_BLACK_LIST    2

#if defined(__MMI_PHB_MAX_NAME_60__)
#define MAX_PB_NAME_LENGTH		60
#else
#define MAX_PB_NAME_LENGTH		30			 
#endif
#define MAX_PB_NUMBER_LENGTH	40			 
#define MAX_PB_EMAIL_LENGTH		60
#define MAX_PB_TITLE_LENGTH		20
#define MAX_PB_COMPANY_LENGTH	20
#define MAX_GRP_NAME_LENGTH		20	/*Caller Group Name Length, Should smaller than PHB name Length. (Share temp buffer.)*/
#define MAX_SEARCH_NAME_LENGTH	10
#define MAX_PB_CALLER_GROUPS	 5

#define MAX_SIM_NUMBER_LENGTH 20			 
#define MAX_SIM_NAME_LENGTH 14			 
#define MAX_SIM_NAME_CHINESE_LENGTH 6			 

#if defined(MMI_ON_HARDWARE_P)
#define CALLER_GROUPS_RECORD_SIZE	 (MAX_PB_CALLER_GROUPS*sizeof(PHB_CALLER_GROUP_STRUCT))//260//Renwy modify for bug 8407
#else
#define CALLER_GROUPS_RECORD_SIZE	 270
#endif

#define OPTIONAL_IDS_RECORD_TOTAL 85
#if defined(MMI_ON_HARDWARE_P)
#define OPTIONAL_IDS_RECORD_SIZE	510	/*Store 85 entries in one NVRAM record, so total is 85 x 6 (6 bytes each) */
#else
#define OPTIONAL_IDS_RECORD_SIZE	425
#endif


#define OPTIONAL_FIELDS_RECORD_SIZE	229
#define MMI_PHB_VCARD_RECORD_SIZE	338

#define INVALID_NUMBER			2000000000
#define ALPHA_LIST_LENGTH			 26
#define MAX_PB_RING_TONES			 60
#if defined(__MMI_STATUS_LED__)
#define MAX_PB_ALERT_TYPES			  5
#else
#define MAX_PB_ALERT_TYPES			  4
#endif
#define MAX_PB_LED_PATTERNS			  6
#define	PHB_NOTIFY_TIMEOUT			  UI_POPUP_NOTIFYDURATION_TIME//UI_POPUP_NOTIFYDURATION_TIME_2000

//For FDl & BDL
#define MAX_SIM_ENTRIES			20 
#define MAX_NAME_LEN_FSB_LIST	MAX_PB_NAME_LENGTH
#define MAX_NUMBER_LEN_FSB_LIST	MAX_PB_NUMBER_LENGTH

#define MAX_SIM_SOS_ENTRIES			 5
#define MAX_NVRAM_SOS_ENTRIES		 5
#define NVRAM_SOS_RECORD_SIZE		86

//For File System and File Manager
#define MMI_PHB_MAX_PATH_LENGTH			64*ENCODING_LENGTH		 
#define MMI_PHB_DRV								MMI_PUBLIC_DRV
#define MMI_PHB_IMAGE_FOLDER				FMGR_DEFAULT_FOLDER_PHBIMAGES
#define MMI_PHB_VIDEO_DATA_FILE			FMGR_DEFAULT_FILE_PHBVIDEO

//For field filter
#define	MMI_PHB_ENTRY_FIELD_NAME		0x01
#define	MMI_PHB_ENTRY_FIELD_NUMBER	0x02
#define	MMI_PHB_ENTRY_FIELD_HOME		0x04
#define	MMI_PHB_ENTRY_FIELD_OFFICE		0x08
#define	MMI_PHB_ENTRY_FIELD_FAX			0x10
#define	MMI_PHB_ENTRY_FIELD_EMAIL		0x20
#define	MMI_PHB_ENTRY_FIELD_COMPANY	0x40

#define  MMI_PHB_ENTRY_FIELD_GSM_NUM    (MMI_PHB_ENTRY_FIELD_NUMBER | MMI_PHB_ENTRY_FIELD_HOME | MMI_PHB_ENTRY_FIELD_OFFICE | MMI_PHB_ENTRY_FIELD_FAX)
#define  MMI_PHB_ENTRY_FIELD_MMS        (MMI_PHB_ENTRY_FIELD_GSM_NUM | MMI_PHB_ENTRY_FIELD_EMAIL)
#define  MMI_PHB_ENTRY_FIELD_OPTIONAL   (MMI_PHB_ENTRY_FIELD_HOME | MMI_PHB_ENTRY_FIELD_OFFICE | MMI_PHB_ENTRY_FIELD_FAX | MMI_PHB_ENTRY_FIELD_EMAIL | MMI_PHB_ENTRY_FIELD_COMPANY)

//Birthday Field
#define MMI_PHB_BDAY_MAX_YEAR_INT	2030		 

#define MMI_PHB_BDAY_YEAR_STR	"0000"			/*Empty Birthday*/
#define MMI_PHB_BDAY_MON_STR	"00"
#define MMI_PHB_BDAY_DAY_STR	"00"
/* 
** Extern Global Function
*/
extern void playRequestedTone(ALL_TONE_ENUM);

/* 
** Typedef 
*/
typedef enum 
{
	MMI_PHB_LIST_FOR_PHB,
	MMI_PHB_LIST_FOR_FRM,
	MMI_PHB_LIST_FOR_SMS,
	MMI_PHB_LIST_FOR_SMS_INSERT,
	MMI_PHB_LIST_FOR_SMS_INSERT_NO_CHECK,
	MMI_PHB_LIST_FOR_SMS_SEARCH_NAME,
	MMI_PHB_LIST_FOR_CHAT,
	MMI_PHB_LIST_FOR_CHAT_NO_CHECK,
	MMI_PHB_LIST_FOR_SETTINGS,
	MMI_PHB_LIST_FOR_SPEED_DIAL,
	MMI_PHB_LIST_FOR_CM,
	MMI_PHB_LIST_FOR_MMS_NUMBER,	/*MMS*/
	MMI_PHB_LIST_FOR_MMS_EMAIL,		/*MMS*/
	MMI_PHB_LIST_FOR_MMS_ALL,		/*MMS*/
	MMI_PHB_LIST_FOR_VIEW_MEMBER_LIST,	/*Caller Group Member List*/
	MMI_PHB_LIST_FOR_ADD_MEMBER_LIST,	/*Caller Group Member List*/
	MMI_PHB_LIST_FOR_SAVE_FROM_FILE_MGR,
	MMI_PHB_LIST_FOR_DELETE_ONE_BY_ONE,
	MMI_PHB_LIST_FOR_EMAIL_APP,
	MMI_PHB_LIST_FOR_ALL_SHARED
} MMI_PHB_LIST_VIEW;

typedef enum
{
	MMI_PHB_IMAGE_NO_SELECT,		/*No picture, use default.*/
	MMI_PHB_IMAGE_FOR_ENTRY,				/*picture for phonebook entry..*/
	MMI_PHB_IMAGE_SELECT_PATH,	/*picture from select path for phb entry.*/
	MMI_PHB_IMAGE_CALLER_GROUP	/*picture for caller group.*/
} mmi_phb_image_location_enum;

typedef enum
{
	MMI_PHB_RECV_RES_IMAGE,
	MMI_PHB_RECV_RES_AUDIO,
	MMI_PHB_RECV_RES_VIDEO
} mmi_phb_receive_resource_enum;

typedef struct 
{
	U8		name[(MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH];
	U8		number[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
	U8		name_dcs;
	U8		alertType;		/*MMI_ALERT_TYPE*/
	U16	pictureId;
	U16	ringtoneId;
	U16	backlightId;
	U16	record_index;	/* For Query Image Path*/
	U16	videoId;	
	U16	video_record_index;
	U8		dialInList;
} PHB_CM_INTERFACE;

/* Optional IDs in a phonebook entry */
typedef struct 
{
	U16		pictureTagID;
	U16		ringToneID;
	U8 		callerGroupID;
} PHB_OPTIONAL_IDS_STRUCT;

/* Optional field in a phonebook entry */
typedef struct 
{
	U8  homeNumber[MAX_PB_NUMBER_LENGTH+1+1];
	U8 	companyName[(MAX_PB_COMPANY_LENGTH+1)*ENCODING_LENGTH];
	U8 	emailAddress[MAX_PB_EMAIL_LENGTH+1];
	U8 	officeNumber[MAX_PB_NUMBER_LENGTH+1+1];
	U8 	faxNumber[MAX_PB_NUMBER_LENGTH+1+1];
} PHB_OPTIONAL_FIELDS_STRUCT;

typedef struct 
{
    U8    groupName[(MAX_GRP_NAME_LENGTH+1)*ENCODING_LENGTH];
    U16    ringToneID;			
    U16    pictureTagID;
    U16    LEDPatternId;
    U16    VideoID;	/*Originally "entryCount" is obselete, so change to store VideoID*/
    kal_uint16    black_list_flag_Id;
    kal_uint16    red_list_flag_Id;
    MMI_ALERT_TYPE    alertType;
    U8    is_modified;
} PHB_CALLER_GROUP_STRUCT;

typedef struct 
{
    U8	name_length;	/* Name Length*/
    U8	name_dcs;		/* Name Data Coding Scheme*/
    U8	name[(MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH];
} MMI_PHB_NAME_STRUCT;

typedef struct 
{
	U8	type;			/*129-default, 145-international*/
	U8	length;
	U8	number[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];		/*1 for + Sign and 1 for Null Termination.*/
} MMI_PHB_NUMBER_STRUCT;

typedef struct 
{
	U8		storage;		/*0-none, 1-SIM, 2-ME*/
	U8		type;
	U16		index;
	U16		record_index;
	MMI_PHB_NUMBER_STRUCT	tel;
	MMI_PHB_NAME_STRUCT	alpha_id;
} MMI_PHB_ENTRY_STRUCT;

/*Use BCD format to store number to save memory.*/
typedef struct
{
	U8	type;	/*129-default; 145-international, begin with '+' */
	U8	length;
	U8	number[(MAX_PB_NUMBER_LENGTH + 1) / 2];	/*half space to store BCD format.*/
} MMI_PHB_NUMBER_BCD_STRUCT;

/*Use array index to decide storage and record_index.*/
typedef struct 
{
	//U8		storage;
	//U8		type;
	//U16		index;
	//U16		record_index;
	MMI_PHB_NUMBER_BCD_STRUCT		tel;
	MMI_PHB_NAME_STRUCT			alpha_id;
	U8		field;		/*Indicate if an entry has the field*/	
	U8		dummy;	/*Make sure each phb structure is two-bytes aligned.*/
} MMI_PHB_ENTRY_BCD_STRUCT;

/*Owner number associate with Line ID*/
typedef struct 
{
    U8	name[(MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH];
    U8	number[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
} MMI_OWNER_NUMBER_STRUCT;

typedef struct 
{
	U16 store_index; /* Store Index of Phonebook, Begin from 0*/
	U32 number;
} MMI_PHB_LOOKUP_NODE_STRUCT;

typedef struct 
{
	U8	type;			/*129-default, 145-international*/
	U8	length;
	U8	number[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
} MMI_SOS_NUMBER_STRUCT;

typedef struct 
{
	MMI_SOS_NUMBER_STRUCT g_phb_nvram_list[MAX_NVRAM_SOS_ENTRIES];
	U8 g_phb_sos_nvram_read;
	U8 simId;
}MMI_EXTRA_NUMBER_CONTEXT_STRUCT;

typedef struct 
{
	S8	name[(MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH];
	S8	number[MAX_PB_NUMBER_LENGTH+1+1];		/*	1 for + Sign and 1 for Null Termination.*/
	S8	homeNumber[MAX_PB_NUMBER_LENGTH+1+1];
	S8	companyName[(MAX_PB_COMPANY_LENGTH+1)*ENCODING_LENGTH];
	S8	emailAddress[MAX_PB_EMAIL_LENGTH+1];
	S8	officeNumber[MAX_PB_NUMBER_LENGTH+1+1];
	S8	faxNumber[MAX_PB_NUMBER_LENGTH+1+1];
	U16	pictureTagID;
	U16	ringToneID;
} MMI_PHB_VCARD_STRUCT;

typedef struct
{
	U16	bYear;
	U8	bMonth;
	U8	bDay;
}MMI_PHB_BDAY_STRUCT;

typedef enum 
{
   MMI_PHB_BCD = 0x01,
   MMI_PHB_ASCII = 0x00,
   MMI_PHB_UCS2 = 0x08
#if defined(__PHB_0x81_SUPPORT__)	/*Support maximum length of 0x81 UCS2*/
   ,
   MMI_PHB_UCS2_81 = 0x09,
   MMI_PHB_UCS2_82 = 0x10
#endif
} MMI_PHB_CHARSET_ENUM;

typedef enum 
{
	MMI_STORAGE_NONE,
	MMI_SIM,
	MMI_SIM2,
	MMI_SIM3,
	MMI_SIM4,
	MMI_NVRAM,
	MMI_STORAGE_BOTH
} MMI_PHB_STORAGE_LOCATION;

typedef enum 
{
   MMI_CSMCC_DEFAULT_ADDR_TYPE = 129,
   MMI_CSMCC_INTERNATIONAL_ADDR = 145
} MMI_CSMCC_ADDR_TYPE;

typedef enum 
{
   MMI_PHB_NONE,
   MMI_PHB_ECC,
   MMI_PHB_FDN,
   MMI_PHB_BDN,
   MMI_PHB_MSISDN,
   MMI_PHB_SDN,
   MMI_PHB_PHONEBOOK
} MMI_PHB_TYPE;

typedef enum 
{
   MMI_PHB_NO_CAUSE,
   MMI_PHB_CAUSE_STORAGE_FULL,
   MMI_PHB_CAUSE_CAPACITY_EXCEEDED
} MMI_PHB_CAUSE_ENUM;

typedef enum 
{
	MMI_PHB_UPDATE,
	MMI_PHB_DELETE,
	MMI_PHB_DELETE_ALL
} MMI_PHB_UPDATE_ENUM;

typedef enum 
{
	MMI_PHB_ENTER_NONE,
	MMI_PHB_ENTER_FROM_IDLE_SCREEN,
	MMI_PHB_ENTER_FROM_CALL_LOG,
	MMI_PHB_ENTER_FROM_CALL_MGNT,
	MMI_PHB_ENTER_FROM_SMS,
	MMI_PHB_ENTER_FROM_JAVA,
	MMI_PHB_ENTER_FROM_VCARD_RECV,              /* enter from recv vcard */
	MMI_PHB_ENTER_FROM_OUTLOOK_IMPORT,              /* enter from recv IMPORT  */
	MMI_PHB_ENTER_FROM_VCF_IMPORT              /* enter from recv IMPORT  */	
}MMI_PHB_ENTER_LOCATION_ENUM;

/* See if the phonebook list screen data need reload.*/
typedef enum
{
	MMI_PHB_ENTRY_NO_CHANGE,
	MMI_PHB_ENTRY_REFRESH,
	MMI_PHB_ENTRY_QSEARCH
}MMI_PHB_ENTRY_UPDATE_ENUM;

typedef enum
{
	MMI_PHB_SORT_STORAGE,
	MMI_PHB_SORT_ENCODING,
	MMI_PHB_SORT_PINYIN
}MMI_PHB_SORT_METHOD_ENUM;

/* 
** Define for Messages
*/
#define PHB_NAME_STRUCT								l4_name_struct
#define PHB_NUMBER_STRUCT							l4c_number_struct
#define PHB_ENTRY_STRUCT							l4c_phb_entry_struct
#define PHB_RESULT_STRUCT							l4c_result_struct
#define MSG_PHB_GET_ENTRY_BY_NAME_REQ_STRUCT		mmi_phb_get_entry_by_name_req_struct
#define MSG_PHB_GET_ENTRY_BY_NAME_RSP_STRUCT		mmi_phb_get_entry_by_name_rsp_struct
#define MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT		mmi_phb_get_entry_by_index_req_struct
#define MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT		mmi_phb_get_entry_by_index_rsp_struct
#define MSG_PHB_SET_ENRTY_REQ_STRUCT				mmi_phb_set_entry_req_struct
#define MSG_PHB_SET_ENRTY_RSP_STRUCT				mmi_phb_set_entry_rsp_struct
#define MSG_PHB_DELETE_ENTRY_REQ_STRUCT				mmi_phb_del_entry_req_struct
#define MSG_PHB_DELETE_ENTRY_RSP_STRUCT				mmi_phb_del_entry_rsp_struct
#define MSG_PHB_GET_PREFERRED_STORAGE_RSP_STRUCT	mmi_phb_get_preferred_storage_rsp_struct
#define MSG_PHB_SET_PREFERRED_STORAGE_REQ_STRUCT	mmi_phb_set_preferred_storage_req_struct
#define MSG_PHB_SET_PREFERRED_STORAGE_RSP_STRUCT	mmi_phb_set_preferred_storage_rsp_struct
#define	MSG_PHB_GET_ALPHA_LIST_REQ_STRUCT			mmi_phb_get_alpha_list_req_struct
#define MSG_PHB_GET_ALPHA_LIST_RSP_STRUCT			mmi_phb_get_alpha_list_rsp_struct 
#define	MSG_PHB_STARTUP_FINISH_IND_STRUCT			mmi_phb_startup_finish_ind_struct
#define	MSG_PHB_STARTUP_READ_IND_STRUCT		mmi_phb_startup_read_ind_struct
#define MSG_PHB_STARTUP_READ_NEXT_REQ_STRUCT	mmi_phb_startup_read_next_req_struct
#define	MSG_PHB_UPDATE_IND_STRUCT					mmi_phb_update_ind_struct

#ifdef MMS_SUPPORT
#define MSG_PHB_MEA_ADDR_INPUT_STRUCT	mea_addr_input_struct		/*For MMS*/
#endif

/*Global context*/
typedef struct 
{
/*wangrui Add for fixbug 10195 start on 2008.10.28*/
	BOOL phb_ReadyFinishi;
/*wangrui Add for fixbug 10195 end*/

	//Global variables - Keep phonebook state flag.
	BOOL phb_ready;
	BOOL processing;
	BOOL nvram_data_init;
	U8 refresh_list;	
	U16 start_scr_id ;
	U16 end_scr_id;
	U8 sim_name_len[MMI_SIM_NUMBER];
	U8 fdn_name_len;
	U8 bdn_name_len;
	U8 owner_name_len;
	U16 sim_total[MMI_SIM_NUMBER];
	U16 sim_used[MMI_SIM_NUMBER];
	U16 phone_total;
	U16 phone_used;
	U16 populate_count; //physical index for populating option field. 0xffff if sort complete
	U16 lookup_table_count;

	//Pointer to global array
	void*	optional_ids;
	void* caller_group;
	U16* list_filter;

	//Index for list screen.
	U16 active_index;
	U16 active_index_second;
	U16 active_index_third;

	//Global variables - passing parameter between functions.
	U8* number_to_dial[5];
	U16 highlight_entry;
	U16 new_highlight_entry;
	U8 selected_storage;
	U8 image_location;
	S32 selected_pic_index;
	S32 selected_ring_index;
	S32 selected_grp_index;
	S32 selected_pattern_index;
	S32 selected_alert_index;
	S32 selected_pic_in_view;
	U8 dial_from_list;
	U8 searched_number_type; /* for mmi_phb_call_get_data_for_call_mgnt */
	U8 sort_type;

	//Inline Selection List
	U16 ring_tone_list[MAX_PB_RING_TONES];
	U16 image_id_list[MAX_PB_PICTURES_TAGS];
	U8* image_name_list[MAX_PB_PICTURES_TAGS];
	U8* ring_tone_name_list[MAX_PB_RING_TONES];
	U8* alert_name_list[MAX_PB_ALERT_TYPES+1];
	U8* led_name_list[MAX_PB_LED_PATTERNS+1];
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	S32 selected_video_index;
	U16 video_id_list[MAX_PB_VIDEO_TAGS];
	U16 selected_video_id;
	U8* video_name_list[MAX_PB_VIDEO_TAGS];
	U8 total_video_id;
	U8 init_video;
	U8 video_location;
	U8 video_audio;
#endif

    U8    total_black_list_id; // list item count
    U8*    black_list_name_list[MAX_PB_BLACK_LIST]; // list show string
    U16    black_list_id_list[MAX_PB_BLACK_LIST]; // list id
    S32    selected_black_list_index; // select 

    U8    total_red_list_id; // list item count
    U8*    red_list_name_list[MAX_PB_BLACK_LIST]; // list show string
    U16    red_list_id_list[MAX_PB_BLACK_LIST]; // list id
    S32    selected_red_list_index; // select 

	U16 recv_res_id;	/*receive resource and associate to phb entry*/
	U8 recv_res_from;	/*receive resource and associate to phb entry*/
	U8 set_done_flag;
	U8 total_ring_tone;
	U8 total_image_id;	
	U8 speak_name;

	//System setting
	U8 prefer_storage;
	U8	view_field[MAX_PB_VIEW_FIELDS];	
}mmi_phb_context_struct;

/**************************************************************
 * Type Definitions
**************************************************************/
/* item's infomation */
typedef struct _CFW_PBK_ENTRY_INFO {
	UINT16 nAppIndex;
	l4_name_struct alpha_id;
	l4_addr_bcd_struct tel;
} CFW_PBK_ENTRY_INFO;

extern mmi_phb_context_struct g_phb_cntx;
#if (MAX_PB_ENTRIES >= 1000)
extern U16 g_phb_list_filter[MAX_PB_ENTRIES];
#endif /* MAX_PB_ENTRIES >= 1000 */
#ifdef __FT_IDLE_SMART_DIAL__ 
typedef struct{
     U16 store_index;
     U8   matchCount;
}SmartDialMatch_STRUCT;
#endif


#endif /*_PHONEBOOKTYPES_H*/


