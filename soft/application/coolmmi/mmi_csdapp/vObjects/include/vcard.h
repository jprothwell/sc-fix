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
 *	vCard.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for sending and receving vcard object.
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
 #include "mmi_features.h"

#if defined(__MMI_VCARD__)

#ifndef _MMI_VCARD_H
#define _MMI_VCARD_H

/* if some header should be include before this one */
#include "phonebooktypes.h"
#include "phonebookgprot.h"
#include "conversions.h"
#include "vobjects.h"

/* call log number type info */
typedef enum
{
    CALL_NUM_TYPE_NONE,         /* none */
    CALL_NUM_TYPE_DIALED,       /* dialed number */
    CALL_NUM_TYPE_MISSED,       /* missed number */
    CALL_NUM_TYPE_RECEIVED      /* received number */
}call_num_type_enum;

/* vcard field enum */
typedef enum
{
    VCARD_FIELD_BEGIN,      /* vcard begin feild */
    VCARD_FIELD_VERSION,    /* vcard version feild */
    VCARD_FIELD_FN,         /* vcard FN feild */
    VCARD_FIELD_N,          /* vcard name feild */
    VCARD_FIELD_TEL,        /* vcard tel feild */
    VCARD_FIELD_EMAIL,      /* vcard eamil feild */
    VCARD_FIELD_ORG,        /* vcard org feild */
    VCARD_FIELD_BDAY,       /* vcard birthday feild */
    VCARD_FIELD_TITLE,      /* vcard title feild */
    VCARD_FIELD_URL,        /* vcard url feild */
    VCARD_FIELD_NOTE,       /* vcard note feild */
    VCARD_FIELD_ADR,        /* vcard address feild */
    VCARD_FIELD_PHOTO,      /* vcard photo feild */
    VCARD_FIELD_REV,        /* vcard rev feild */
    VCARD_FIELD_LABEL,      /* vcard label feild */
    VCARD_FIELD_NICKNAME,   /* vcard nike name feild */
    VCARD_FIELD_MAILER,     /* vcard mailer feild */
    VCARD_FIELD_TZ,         /* vcard tz feild */
    VCARD_FIELD_GEO,        /* vcard geo feild */
    VCARD_FIELD_ROLE,       /* vcard role feild */
    VCARD_FIELD_LOGO,       /* vcard logo feild */
    VCARD_FIELD_AGENT,      /* vcard agent feild */
    VCARD_FIELD_CATEGORIES, /* vcard categories feild */
    VCARD_FIELD_PRODID,     /* vcard prodid feild */
    VCARD_FIELD_SORT_STRING,/* vcard sort string feild */
    VCARD_FIELD_SOUND,      /* vcard sound feild */
    VCARD_FIELD_UID,        /* vcard UID feild */
    VCARD_FIELD_CLASS,      /* vcard class feild */
    VCARD_FIELD_KEY,        /* vcard key feild */
    VCARD_FIELD_X,          /* vcard X feild */
    VCARD_FIELD_END,        /* vcard end feild */
    VCARD_FIELD_TOTAL       /* enum total */
} vcard_filed_enum;

/* vcard parse result enum */
typedef enum
{
    MMI_VCARD_PARSE_NO_ERROR,                   /* no error */    
    MMI_VCARD_PARSE_BEGIN_ERROR,                /* begin field check error */
    MMI_VCARD_PARSE_VERSION_ERROR,              /* version field check error */
    MMI_VCARD_PARSE_FILE_SIZE_TOO_LARGE,        /* vcard file too large */
    MMI_VCARD_PARSE_UNKNOW_ERROR,               /* other error */
    MMI_VCARD_PARSE_VERSION_21,                 /* vcard version 2.1 */
    MMI_VCARD_PARSE_VERSION_30,                 /* vcard version 3.0 */
    MMI_VCARD_PARSE_FORMAT_ERROR,
    MMI_VCARD_PARSE_FILE_OPEN_ERROR,  
    MMI_VCARD_PARSE_FILE_READ_ERROR,
    MMI_VCARD_PARSE_FILE_END_ERROR,
    MMI_VCARD_PARSE_ENUM_TATOL                  /* enmu total */
} vcard_parse_enum;

/* send vcard location enum */
typedef enum
{
    MMI_VCARD_NO_TYPE,                          /* none */
    MMI_VCARD_SEND_MY_CARD,                     /* send from my number */
    MMI_VCARD_SEND_SELECT_CARD,                 /* send from phonebook list */
    MMI_VCARD_SEND_SELECT_CARD_SECOND_LEVEL,    /* send from imps */
    MMI_VCARD_SEND_VCARD_TATOL                  /* enum total */
} vcard_card_type_enum;

/* parse location enum */
typedef enum
{
    MMI_VCARD_PARSE_FROM_FILE_NONE,          
    MMI_VCARD_PARSE_FROM_FILE_MANAGER,          /* from file manager */
    MMI_VCARD_PARSE_FROM_VOBJ_BUFFER,           /* from receive */
    MMI_VCARD_PARSE_FROM_OTHER_APPS             /* from imps */
} vcard_parse_location_enum;

/* vcard file from location enmu */
typedef enum
{
    MMI_VCARD_SAVE_TO_FILE_NONE,
    MMI_VCARD_SAVE_TO_FILE_FROM_PHB,    /* vcard file build from phonebook */
    MMI_VCARD_SAVE_TO_FILE_FROM_RECV    /* vcard file from recv */
} vcard_file_location_enum;

/* vcard file import from location enmu */
typedef enum
{
    MMI_VCARD_IMPORT_FROM_NONE,
    MMI_VCARD_IMPORT_FROM_VCF,    /* vcard file build from *.vcf */
    MMI_VCARD_IMPORT_FROM_OUTLOOK    /* vcard file from *.csv */
} vcard_file_Import_location_enum;

/* vcard context struct */
typedef struct
{
    S8 *FileNamePtr;            /* vcard file name point */
    S8 *FilePathPtr;            /* vcard file path point */
    U8 StorageLoc;              /* phonebook storage to save vcard */
    U8 FileLoc;                 /* vcard_file_location_enum */
    U8 ParseLoc;                /* vcard_parse_location_enum */
    U8 ExportLoc;              /* phonebook export to save vcard */
    U8 ImportLoc;              /* phonebook export to save vcard */
    U8 CardType;                /* vcard_card_type_enum */
    U8 processing;
    mmi_chset_enum CharSet;     /* char set used to decode when there is no encoding type in vcard file 
	                            assign before mmi_vcard_reader_parse_line() */
} vcard_context_struct;

/* vcard filed struct */
typedef struct
{
    S8 *ucs2_name;                              /* usc2 name field */
    S8 *number;                                 /* mobile number field*/
    S8 *home_num;                               /* home number field */
    S8 *office_num;                             /* offfice number field */
    S8 *fax_num;                                /* fax number field */
    S8 *email_addr;                             /* email address field */
    S8* email_addr2;                            /* email address 2 field */
    S8 *ucs2_company;                           /* company name field */
    S8 *b_day;                                  /* birthday field */
    PHB_INFO_FIELDS_STRUCT *info_fields;        /* info fields */
    PHB_OPTIONAL_IDS_STRUCT *optional_ids;      /* optional fields */
    MMI_PHB_GEOGRAPHIC_FIELD_STRUCT *geo_fields;/* geo fields */    
    S8 *nick_name;                              /* nick name field */
    MYTIME *start_time;                         /* start time for call log */
    U8 call_num_type;                           /* call_num_type_enum for call log */
}vcard_field_struct;



#ifdef __MMI_PHB_INFO_FIELD__
#define VCARD_WRITE_BUFF_LEN      3500          /* vcard write buffer size */
#define VCARD_READ_BUFF_LEN        2048         /* vcard read buffer size */
#else /* __MMI_PHB_INFO_FIELD__ */
#define VCARD_WRITE_BUFF_LEN      1024          /* vcard write buffer size */
#define VCARD_READ_BUFF_LEN          512        /* vcard read buffer size */
#endif /* __MMI_PHB_INFO_FIELD__ */


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_init
 * DESCRIPTION
 *  Initialize function of vcard application.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vcard_init(void);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_deinit
 * DESCRIPTION
 *  Deinitialize function of vcard application.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vcard_deinit(void);

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_option_select_card
 * DESCRIPTION
 *  Pre - Entry function of vcard forward option, will call by phonebook application.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vcard_forward_option_select_card(void);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_option_my_card
 * DESCRIPTION
 *  Pre - Entry function of vcard forward option, will call by phonebook application.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vcard_forward_option_my_card(void);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_menu_check
 * DESCRIPTION
 *  send vcard option menu check function
 * PARAMETERS
 *  void
 * RETURNS
 *  MMI_BOOL
 * RETURN VALUES
 *  MMI_TRUE               :                    need hide send vcard menuy
 *  MMI_FALSE              :                    need unhide send vcard menuy
 *****************************************************************************/
extern MMI_BOOL mmi_vcard_forward_menu_check(void);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_recv_set_result
 * DESCRIPTION
 *  set receive vcard result
 * PARAMETERS
 *  result                 :      [IN]           save vcard result
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vcard_recv_set_result(U8 result);


#if defined(__MMI_IMPS__) && defined(IMPS_WV13_ENHANCEMENT)
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_imps
 * DESCRIPTION
 *  forward vcard to imps app.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vcard_forward_imps(void);
#endif

#if defined(__MMI_FILE_MANAGER__)
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_copy_forward_fmgr
 * DESCRIPTION
 *  entry function for save vcard to file manager.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vcard_copy_forward_fmgr(void);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_fmgr_sms_forward
 * DESCRIPTION
 *  Forward vcard to SMS from file manager.
 *  This is a intermediate function between file manager and SMS app.
 * PARAMETERS
 *  path        :     [IN]      vcard file path
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vcard_fmgr_sms_forward(S8 *path);
#endif /* defined(__MMI_FILE_MANAGER__) */ 


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_recv_from_idle_screen
 * DESCRIPTION
 *  A pre-entry function of receiving vcard functuion. This vcard is displayed when in idle screen.
 *  If vcard is received from IrDA or SMS, it will enter from this path.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vcard_recv_from_idle_screen(void);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_recv_from_other_apps
 * DESCRIPTION
 *  A pre entry function of receiving vcard from imps
 * PARAMETERS
 *  path             :     [IN]      vcard file path
 *  call_back        :     [IN]      call back function
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vcard_recv_from_other_apps(S8 *path, mmi_vobj_recv_from_other_app_callback call_back);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_recv_from_other_apps_callback
 * DESCRIPTION
 *  call back function for recv vcard from imps
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vcard_recv_from_other_apps_callback(void);


#if defined(__MMI_FILE_MANAGER__)
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_recv_from_file_mgr
 * DESCRIPTION
 *  A pre-entry function of receiving vcard functuion. This vcard is forwarded from file manager.
 * PARAMETERS
 *  path        :     [IN]      vcard file path
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vcard_recv_from_file_mgr(S8 *path);
#endif 


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_writer_v21_mycard_to_file
 * DESCRIPTION
 *  write my vcard entry into vcard format into destination file.
 * PARAMETERS
 *  path                 :     [IN]        Destination path
 * RETURNS
 *  S32
 * RETURN VALUES
 *  fs_error             :                fs op error info
 *****************************************************************************/
extern S32 mmi_vcard_writer_v21_mycard_to_file(S8 *path);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_writer_v21_select_card_to_file
 * DESCRIPTION
 *  write select phonebook entry into vcard format into destination file.
 * PARAMETERS
 *  path        :     [IN]        Destination path
 * RETURNS
 *  S32
 * RETURN VALUES
 *  fs_error             :                fs op error info
 *****************************************************************************/
extern S32 mmi_vcard_writer_v21_select_card_to_file(S8 *path);


/*****************************************************************************
 * FUNCTION
 *  mmi_phb_vcard_req_writer_v21_to_file
 * DESCRIPTION
 *  write select phonebook entry into vcard format into destination file.
 * PARAMETERS
 *  path                :           [IN]        Destination path
 *  field_content       :           [IN]        vcard filed content
 * RETURNS
 *  S32
 * RETURN VALUES
 *  fs_error             :                fs op error info
 *****************************************************************************/
extern S32 mmi_vcard_writer_v21_to_file(S8 *path, vcard_field_struct *field_content);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_writer_to_file
 * DESCRIPTION
 *  write entry into vcard format into destination file.
 * PARAMETERS
 *  path               :        [IN]        Destination path
 *  field_content      :        [IN]        vcard field data   
 * RETURNS
 *  S32
 * RETURN VALUES
 *  fs_error             :                fs op error info
 *****************************************************************************/
extern S32 mmi_vcard_writer_to_file(S8 *path, vcard_field_struct *field_content);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_recv_view_detail_pre_entry_check
 * DESCRIPTION
 *  entry function of recv vcard view detial
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vcard_recv_view_detail_pre_entry_check(void);


#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_get_bday_to_buff
 * DESCRIPTION
 *  read a phonebook contact's birthday to a buffer
 * PARAMETERS
 *  store_index     :    [IN]       phonebook contact's store index 
 *  b_day           :    [OUT]      output birthday data         
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vcard_get_bday_to_buff(U16 store_index, S8 *b_day);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_retrive_bday
 * DESCRIPTION
 *  parse birthday field of a vcard object.s
 * PARAMETERS
 *  String         :      [IN]        String read from file for one line
 *  pb_year        :      [OUT]       output birthday year field
 *  pb_mon         :      [OUT]       output birthday month field
 *  pb_day         :      [OUT]       output birthday day field
 *  change_flag    :      [IN]        birthday change flag
 * RETURNS
 *  U8
 * RETURN VALUES
 *  MMI_VCARD_PARSE_NO_ERROR   :   always return no error
 *****************************************************************************/
extern U8 mmi_vcard_reader_retrive_bday(S8 *String, S8 *pb_year, S8 *pb_mon, S8 *pb_day, BOOL change_flag);
#endif /* defined(__MMI_PHB_BIRTHDAY_FIELD__) */ 


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_uti_get_line
 * DESCRIPTION
 *  Get a line from file system, excluding the escape characters for vcard.
 * PARAMETERS
 *  strOutput      :     [OUT]           Storage location for data
 *  fh             :     [IN]            File handle
 * RETURNS
 *  S32
 * RETURN VALUES
 *  length         :                     output string length
 *****************************************************************************/
extern S32 mmi_vcard_uti_get_line(S8 *strOutput, FS_HANDLE fh);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_uti_get_single_line
 * DESCRIPTION
 *  Get a line from file system, excluding the escape characters for vcard.
 * PARAMETERS
 *  strOutput      :     [IN/OUT]        Storage location for data
 *  fh             :     [IN]            File handle
 * RETURNS
 *  S32
 * RETURN VALUES
 *  length         :                     output string length
 *****************************************************************************/
extern S32 mmi_vcard_uti_get_single_line(S8 *strOutput, FS_HANDLE fh);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_parse_line
 * DESCRIPTION
 *  Read one line to parse for each filed.
 * PARAMETERS
 *  fh                       :      [IN]        file handle ready to read and parse.
 * RETURNS
 *  U8
 * RETURN VALUES
 *  vcard_parse_enum         :                parse result
 *****************************************************************************/
extern U8 mmi_vcard_reader_parse_line(FS_HANDLE fh);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_get_field
 * DESCRIPTION
 *  parse field name of a vcard string.
 * PARAMETERS
 *  String                  :       [IN]        String read from file for one line
 * RETURNS
 *  U8
 * RETURN VALUES
 *  vcard_filed_enum        :                   vcard field enum
 *****************************************************************************/
extern U8 mmi_vcard_reader_get_field(S8 *String);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_from_file_path
 * DESCRIPTION
 *  Clear buffer and open the file to parse.
 * PARAMETERS
 *  path                    :      [IN]        The vcard file location in file system.
 * RETURNS
 *  S32
 * RETURN VALUES
 *  <0                      :                 fs op error
 *  vcard_parse_enum        :                 vcard parse result
 *****************************************************************************/
extern S32 mmi_vcard_reader_from_file_path(S8 *path);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_check_header
 * DESCRIPTION
 *  Check the VCARD header.
 * PARAMETERS
 *  String      :      [IN]        String read from file for one line
 * RETURNS
 *  BOOL
 * RETURN VALUES
 *  FALSE       :                  vcard file heard correct
 *  TRUE        :                  vcard file heard incorrect
 *****************************************************************************/
extern BOOL mmi_vcard_reader_check_header(S8 *String);



/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_check_version
 * DESCRIPTION
 *  Check the version field of a vcard object.
 * PARAMETERS
 *  String      :     [IN]        String read from file for one line
 * RETURNS
 *  BOOL
 * RETURN VALUES
 *  FALSE       :                  vcard file version correct
 *  TRUE        :                  vcard file version incorrect
 *****************************************************************************/
extern BOOL mmi_vcard_reader_check_version(S8 *String);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_retrive_name
 * DESCRIPTION
 *  parse the name field of a vcard object.
 * PARAMETERS
 *  String                      :    [IN]        String read from file for one line
 * RETURNS
 *  U8
 * RETURN VALUES
 *  MMI_VCARD_PARSE_NO_ERROR    :                always return no error
 *****************************************************************************/
extern U8 mmi_vcard_reader_retrive_name(S8 *String);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_retrive_number
 * DESCRIPTION
 *  parse number field of a vcard object.s
 * PARAMETERS
 *  String              :       [IN]        String read from file for one line
 *  number              :       [OUT]       output mobile number data
 *  home_number         :       [OUT]       output home number data 
 *  office_number       :       [OUT]       output office number data 
 *  fax_number          :       [OUT]       output fax number data
 * RETURNS
 *  U8
 * RETURN VALUES
 *  MMI_VCARD_PARSE_NO_ERROR    :                always return no error
 *****************************************************************************/
extern U8 mmi_vcard_reader_retrive_number(
            S8 *String,
            S8 *number,
            S8 *home_number,
            S8 *office_number,
            S8 *fax_number);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_retrive_email
 * DESCRIPTION
 *  parse email field of a vcard object.s
 * PARAMETERS
 *  String              :       [IN]        String read from file for one line
 *  email_address       :       [OUT]       output email address data
 * RETURNS
 *  U8
 * RETURN VALUES
 *  MMI_VCARD_PARSE_NO_ERROR    :                always return no error
 *****************************************************************************/
extern U8 mmi_vcard_reader_retrive_email(S8 *String, S8 *email_address);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_retrive_org
 * DESCRIPTION
 *  parse org field of a vcard object.s
 * PARAMETERS
 *  String              :      [IN]        String read from file for one line
 *  company_name        :      [OUT]       output company name data 
 * RETURNS
 *  U8
 * RETURN VALUES
 *  MMI_VCARD_PARSE_NO_ERROR    :                always return no error
 *****************************************************************************/
extern U8 mmi_vcard_reader_retrive_org(S8 *String, S8 *company_name);


#if defined(__MMI_PHB_INFO_FIELD__)
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_retrive_title
 * DESCRIPTION
 *  parse title field of a vcard object.s
 * PARAMETERS
 *  String              :       [IN]        String read from file for one line
 *  title               :       [OUT]       output title data  
 * RETURNS
 *  U8
 * RETURN VALUES
 *  MMI_VCARD_PARSE_NO_ERROR    :                always return no error
 *****************************************************************************/
extern U8 mmi_vcard_reader_retrive_title(S8 *String, S8 *title);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_retrive_url
 * DESCRIPTION
 *  parse url field of a vcard object.s
 * PARAMETERS
 *  String              :       [IN]        String read from file for one line
 *  url                 :       [OUT]       output url data  
 * RETURNS
 *  U8
 * RETURN VALUES
 *  MMI_VCARD_PARSE_NO_ERROR    :                always return no error
 *****************************************************************************/
extern U8 mmi_vcard_reader_retrive_url(S8 *String, S8 *url);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_retrive_address
 * DESCRIPTION
 *  parse address field of a vcard object.s
 * PARAMETERS
 *  String                  :       [IN]        String read from file for one line
 *  pobox                   :       [OUT]       output pobox data
 *  extension               :       [OUT]       output extension data    
 *  street                  :       [OUT]       output street data    
 *  city                    :       [OUT]       output city data    
 *  state                   :       [OUT]       output state data
 *  postalcode              :       [OUT]       output postalcode data
 *  country                 :       [OUT]       output country data   
 * RETURNS
 *  U8
 * RETURN VALUES
 *  MMI_VCARD_PARSE_NO_ERROR    :                always return no error
 *****************************************************************************/
extern U8 mmi_vcard_reader_retrive_address(S8 *String, S8 *pobox, S8 *extension, S8 *street, S8 *city, S8 *state, S8 *postalcode, S8 *country);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_retrive_note
 * DESCRIPTION
 *  parse note field of a vcard object.s
 * PARAMETERS
 *  String                  :       [IN]        String read from file for one line
 *  note                    :       [OUT]       output note data  
 * RETURNS
 *  U8
 * RETURN VALUES
 *  MMI_VCARD_PARSE_NO_ERROR    :                always return no error
 *****************************************************************************/
extern U8 mmi_vcard_reader_retrive_note(S8 *String, S8 *note);
#endif /* defined(__MMI_PHB_INFO_FIELD__) */


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_util_set_default_charset
 * DESCRIPTION
 *  set default charset of vcard
 * PARAMETERS
 *  default_charset       :      [IN]     default charset           
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vcard_util_set_default_charset(U8 default_charset);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reset_fmagr_flag
 * DESCRIPTION
 *  reset fmgr flag of vcard
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vcard_reset_fmagr_flag(void);


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_get_version
 * DESCRIPTION
 *  get a vcard file version
 * PARAMETERS
 *  path                :     [IN]        The vcard file location in file system.
 * RETURNS
 *  S32
 * RETURN VALUES
 *  <0                  :                 fs op error
 *  vcard_parse_enum    :                 vcard parse result
 *****************************************************************************/
extern S32 mmi_vcard_get_version(S8 *path);

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_outlook_recv_parse_one_record
 * DESCRIPTION
 *  parse outlook one by one and add to phb
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern BOOL mmi_vcard_outlook_recv_parse_one_by_one(void);

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_import_free_buffer
 * DESCRIPTION
 *  Free allocate file path buffer when exit import application.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vcard_import_free_buffer(void);

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_outlook_import_done_del_src
 * DESCRIPTION
 * del all useless src
 * PARAMETERS
 * RETURNS
 *  void
 ****************************************************************************/
extern void mmi_vcard_outlook_import_done_del_src(void);

extern void mmi_vcard_forward_option_my_card(void);

#if 1
extern void mmi_vcard_highlight_forward_sms(void);
#endif

#if defined(JATAAYU_SUPPORT)
extern void mmi_vcard_highlight_forward_mms(void) ;
#endif

#if  defined(__MMI_IRDA_SUPPORT__)
extern void mmi_vcard_highlight_forward_irda(void);
extern void mmi_vcard_forward_irda(void);
extern void mmi_vcard_forward_irda_callback(void *buf_p, U32 result );
#endif

//Renwy add begine 2008/08/06
#ifdef __MMI_ACCEL_OPP_SUPPORT__
static void mmi_vcard_highlight_forward_bt(void);
static void mmi_vcard_forward_bt(void);
#endif /* __MMI_ACCEL_OPP_SUPPORT__ */ 
//Renwy add end 2008/08/06
#if defined(EMAIL_APP_SUPPORT)
extern void mmi_vcard_highlight_forward_email(void);
extern void mmi_vcard_forward_email(void);
#endif

#if defined(__MMI_FILE_MANAGER__)
extern void mmi_vcard_highlight_forward_fmgr(void);
extern void mmi_vcard_highlight_enter_filename_done(void);
extern void mmi_vcard_highlight_enter_filename_input_method(void);
extern void mmi_vcard_recv_from_file_mgr(S8* path);
#endif

extern void mmi_vcard_highlight_recv_save_to_sim(void);
extern void mmi_vcard_highlight_recv_save_to_phone(void);
extern void mmi_vcard_highlight_recv_save_to_fmgr(void);

extern void mmi_vcard_recv_from_idle_screen(void);

extern S32 mmi_vcard_writer_v21_mycard_to_file(S8* path);
extern S32 mmi_vcard_writer_v21_select_card_to_file(S8* path);

extern  U8 mmi_vcard_reader_parse_line(FS_HANDLE fh);
extern U8 mmi_vcard_reader_get_field(S8* String);
extern U8 mmi_vcard_reader_retrive_name(S8* String);

/*
*export vCard
*/
extern void mmi_phb_highlight_export(void);
extern void mmi_phb_highlight_export_from_phone(void);
extern void mmi_phb_highlight_export_from_sim(void);

/*
*load vCard
*/
extern void mmi_phb_highlight_import(void);
extern void mmi_phb_highlight_import_to_phone(void);
extern void mmi_phb_highlight_import_to_sim1(void);
extern void mmi_phb_highlight_import_from_vcf(void);
extern void mmi_phb_highlight_import_from_outlook(void);

  
#endif	/*END #define _MMI_VCARD_H*/
#endif /* _MMI_VCARD_H */

