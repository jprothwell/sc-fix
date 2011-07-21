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
 *	vObjects.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for common screen and function for vCard and vCalendar
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
#ifndef _MMI_VOBJECTS_H
#define _MMI_VOBJECTS_H

#include "mmi_data_types.h"
#include "globalconstants.h"
#include "filemgr.h"
#include "vobjectsresdef.h"
/* if some header should be include before this one */
/* 
** Define
*/
#define VOBJ_DRV              MMI_PUBLIC_DRV
#define VOBJ_MAX_TEMP_FILE       6      /* Maximum 254 */
#define VOBJ_FILE_PATH           "Received"
#define VOBJ_VCARD_FILE_EXT         ".vcf"
#define VOBJ_VCLNDR_FILE_EXT     ".vcs"
#define VOBJ_VCARD_OUTLOOK_FILE_EXT         ".csv"
#define VOBJ_TMP_FILE_NAME       "tmp"
#define VOBJ_VCARD_DEFAULT_FILE_NAME   "~vcard.vcf"
#define VOBJ_VCLNDR_DEFAULT_FILE_NAME  "~vcalendar.vcs"
#define VOBJ_VCLNDR_DEFAULT_FILE_NAME_FOR_AT  "~at_vcs.vcs"
#define VOBJ_MAX_PATH_LENGTH     FMGR_MAX_PATH_LEN*ENCODING_LENGTH
#define VOBJ_MAX_FILE_EXT_LEN    FMGR_MAX_EXT_LEN*ENCODING_LENGTH
#define VOBJ_MAX_FILENAME_LENGTH (FMGR_MAX_INPUT_FILE_LEN)*ENCODING_LENGTH

#define VOBJ_SRC_PORT            0x0000 /* For sending SMS */
#define VOBJ_VCARD_PORT       0x23f4
#define VOBJ_VCLNDR_PORT         0x23f5
#ifdef __MMI_VBOOKMARK__
#define VOBJ_VBKM_PORT         0x2000 /* temp number need to be change later*/
#endif
#define VOBJ_READ_BUFF_LEN       512
#define VOBJ_WRITE_BUFF_LEN      1024

#define  VOBJ_CR_CHAR         0x0D
#define  VOBJ_LF_CHAR         0x0A
#define  VOBJ_NULL            '\0'
#define  VOBJ_VALUE_SEP_STR   ":"
#define  VOBJ_VALUE_SEP_SEMI_COLON    ";"
#define  VOBJ_QP_CRLF_STR     "=0D=0A="
#define  VOBJ_CRLF_STR     "\r\n"
#define  VOBJ_LF_STR       "\n"

/* 
 * Typedef 
 */

/* vObject type enum */
typedef enum
{
    MMI_VOBJ_UNRECOGNIZED,         /* Unrecognized */
    MMI_VOBJ_VCARD,                /* vCard */
    MMI_VOBJ_VCLNDR,               /* vCalendar */
#ifdef __MMI_VBOOKMARK__
    MMI_VOBJ_VBKM,                  
#endif
    MMI_VOBJ_TOTAL
} vobj_object_type_enum;

#define VOBJ_TYPE_NUM   (MMI_VOBJ_TOTAL - 1)   /* define vobject type */

/* Save vCalendar or vCard type with IMPS */
typedef enum
{
    MMI_VOBJ_SAVE_DISCARD,             /* Revieve vObject but not save, discard */

    MMI_VOBJ_SAVE_TO_SIM,              /* Save to SIM (SIM1) card success */
#ifdef __MMI_MULTI_SIM__
    MMI_VOBJ_SAVE_TO_SIM2,             /* Dual sim, save to SIM2 success */
    MMI_VOBJ_SAVE_TO_SIM3,
    MMI_VOBJ_SAVE_TO_SIM4,
#endif
    MMI_VOBJ_SAVE_TO_PHONE,            /* Save to phone success */
    MMI_VOBJ_SAVE_TO_FILEMGR,          /* Save to file manager success */
    MMI_VOBJ_SAVE_TO_SIM_FAIL,         /* Save to SIM card failed */
#ifdef __MMI_MULTI_SIM__
    MMI_VOBJ_SAVE_TO_SIM2_FAIL,        /* Save to SIM2 card failed */
    MMI_VOBJ_SAVE_TO_SIM3_FAIL, 
    MMI_VOBJ_SAVE_TO_SIM4_FAIL, 
#endif
    MMI_VOBJ_SAVE_TO_PHONE_FAIL,       /* Save to phone failed */
    MMI_VOBJ_SAVE_TO_FILEMGR_FAIL,     /* Save to file manager failed */
    
    MMI_VOBJ_SAVE_TOTAL
} vobj_save_info_enum;


typedef void(* mmi_vobj_recv_from_other_app_callback)(vobj_save_info_enum, S8*);

/* vObject global context */
typedef struct
{
    U16 curr_sms_port;                  /* Port number of the message */
    U8 buffer_begin;                    /* Sequence number of the object, used to generate temp file */
    U8 buffer_end;                      /* Buffer index together with buffer_begin, to store temp files */
#ifdef __MMI_VBOOKMARK__
    U8 obj_list[VOBJ_MAX_TEMP_FILE];    /* Temp file list */
#else
    U8 obj_list;                        /* Temp file bit buffer */
#endif
    U16 sms_port_reg[VOBJ_TYPE_NUM];    /* Sms port list */
    U8 sms_port_reg_count;              /* Sms port num */
    S8 *AppFilePathPtr;                 /* Temp file path to store vObject from other apps, 
                                           such as imps, network */
    vobj_save_info_enum info;           /* vObject save result */
    mmi_vobj_recv_from_other_app_callback recv_callback;    /* Reveive from other app callback handler */
} vobj_context_struct;

/* vObject save as file result */
typedef enum
{
    VOBJ_ERR_NO_ERROR = 0,            /* Save success */
    VOBJ_ERR_EMPTY_NAME,              /* Name empty, save failed */
    VOBJ_ERR_DUPLICATE_NAME,          /* Duplicated name, save failed */
    VOBJ_ERR_MEMORY_FULL,             /* Memory full, save failed */
    VOBJ_ERR_INVALID_FILENAME,        /* Invalid filename, save failed */
    VOBJ_ERR_SELECT_PATH_TOO_LONG,    /* Selected path too long, save failed */
    VOBJ_ERR_UNSUPPORT_FORMAT,        /* Unsupport format, save failed */
    VOBJ_ERR_UNKNOWN
} vobj_file_err_enum;

/* vObject parse descriptive property parameters, 
   used together with 'g_vobj_paras' defined in vObjects.c */
typedef enum
{
    VOBJ_PARA_LANGUAGE,               /* Not used */
    VOBJ_PARA_CHARSET,                /* Charset for parsing */
    VOBJ_PARA_ENCODING,               /* Encoding for parsing */
    VOBJ_PARA_TOTAL
} vobj_desc_para_type_enum;

typedef enum
{
    VOBJ_EN_7BIT = 0,                 /* 7-bit encoding */
    VOBJ_EN_8BIT,                     /* 8-bit encoding */
    VOBJ_EN_QP,                       /* Quoted-Printable */
    VOBJ_EN_BASE64,                   /* Base64 */
    VOBJ_EN_OTHERS,                   /* X-xxxx */
    VOBJ_EN_TOTAL
} vobj_ecoding_type_enum;

typedef enum
{
    VOBJ_CH_UTF8,
    VOBJ_CH_BIG5,
    VOBJ_CH_UTF7,
    VOBJ_CH_TOTAL
} vobj_charset_type_enum;


/******************************************************************************* 
 * Extern Global Variable
 *******************************************************************************/
 
/* vObject global context */
extern vobj_context_struct g_vobj_cntx;


/******************************************************************************* 
 * Extern Global Function
 *******************************************************************************/
 
/*****************************************************************************
 * FUNCTION
 *  mmi_vobj_init
 * DESCRIPTION
 *  Initialization of vobject application.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 * RETURN VALUES
 *  void
 *****************************************************************************/
extern void mmi_vobj_init(void);


/*****************************************************************************
 * FUNCTION
 *  mmi_vobj_deinit
 * DESCRIPTION
 *  Deinitialize function for vObjects. Free allocated buffer.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 * RETURN VALUES
 *  void
 *****************************************************************************/
extern void mmi_vobj_deinit(void);


/*****************************************************************************
 * FUNCTION
 *  mmi_vobj_is_pending_object
 * DESCRIPTION
 *  Check if there any pending objects.
 *  Criterion: if buffer size > 0 and rename succesfully.
 *  If there is any pending obejct, rename the temporary file.
 * PARAMETERS
 *  void
 * RETURNS
 *  If there is any pending object
 * RETURN VALUES
 *  TRUE : If there is any pending object
 *  FALSE : IF there is not any
 *****************************************************************************/
extern BOOL mmi_vobj_is_pending_object(void);


/*****************************************************************************
 * FUNCTION
 *  mmi_vobj_entry_recv_object
 * DESCRIPTION
 *  Display indication screen whe receiving vobjects.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vobj_entry_recv_object(void);


/*****************************************************************************
 * FUNCTION
 *  mmi_vobj_clear_one_file_buffer
 * DESCRIPTION
 *  remove current indexing file from file buffer.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vobj_clear_one_file_buffer(void);


/*****************************************************************************
 * FUNCTION
 *  mmi_vobj_get_file_buffer_size
 * DESCRIPTION
 *  return number of current pending objects
 * PARAMETERS
 *  void
 * RETURNS
 *  number of objects
 * RETURN VALUES
 *  size : number of objects
 *****************************************************************************/
extern U8 mmi_vobj_get_file_buffer_size(void);


/*****************************************************************************
 * FUNCTION
 *  mmi_vobj_fs_check_folder
 * DESCRIPTION
 *  check if vObject folder exists, if not, create the folder
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern S32 mmi_vobj_fs_check_folder(void);


/*****************************************************************************
 * FUNCTION
 *  mmi_vobj_entry_irda_processing_screen
 * DESCRIPTION
 *  Display progressing screen when sending vobject via irda.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vobj_entry_irda_processing_screen(void);


/*****************************************************************************
 * FUNCTION
 *  mmi_vobj_get_full_path_name
 * DESCRIPTION
 *  This function is to generate full file path and name to store
 *  vCard and vCalendar objects from the network.
 * PARAMETERS
 *  index : [IN]Sequence number of the object
 *  result : [IN/OUT]String buffer to store the result path and name
 *  type : [IN]Type of the object, vCard/vCalendar
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vobj_get_full_path_name(U8 index, S8 *result, U8 type);


/*****************************************************************************
 * FUNCTION
 *  mmi_vobj_irda_receive_ind
 * DESCRIPTION
 *  Indication handler when receiving a vObject message from IrDA.
 *  If it's in Idle screen, process the object immediatelly,
 *  otherwise, insert into the queue.
 * PARAMETERS
 *  path_src : [IN/OUT]File path name of the vobject received from irda
 *  type : [IN]Type of the object (vCard or vCalendar)
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vobj_irda_receive_ind(void *path_src, U32 type);


/*****************************************************************************
 * FUNCTION
 *  mmi_vobj_opp_receive_ind
 * DESCRIPTION
 *  Indication handler when receiving a vObject message from BT.
 *  If it's in Idle screen, process the object immediatelly,
 *  otherwise, insert into the queue.
 * PARAMETERS
 *  path_src : [IN/OUT]File path name of the vobject received from opp
 *  type : [IN]Type of the object (vCard or vCalendar)
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vobj_opp_receive_ind(void *path_src, U32 type);


/*****************************************************************************
 * FUNCTION
 *  mmi_vobj_opp_receive_ind
 * DESCRIPTION
 *  Indication handler when receiving a vObject message from BT.
 *  If it's in Idle screen, process the object immediatelly,
 *  otherwise, insert into the queue.
 * PARAMETERS
 *  path_src : [IN/OUT]File path name of the vobject received from opp
 *  type : [IN]Type of the object (vCard or vCalendar)
 *  call_back : [IN]Callback handler for receiving vObject
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vobj_imps_receive_ind(void *path_src, U32 type, mmi_vobj_recv_from_other_app_callback call_back);


/*****************************************************************************
 * FUNCTION
 *  mmi_vobj_uti_strnicmp
 * DESCRIPTION
 *  Convert a charcater to lower case
 * PARAMETERS
 *  src : [IN]String pointer to be compared
 *  dest : [IN]String pointer to be compared
 *  maxlen : [IN]Lengh to be compared
 * RETURNS
 *  If two string is identical
 * RETURN VALUES
 *  Ture : If two string is identical
 *  False : If two string is different
 *****************************************************************************/
extern S8 mmi_vobj_uti_strnicmp(S8*, S8*, S32);


/*****************************************************************************
 * FUNCTION
 *  mmi_vobj_uti_cmp_filed
 * DESCRIPTION
 *  Check if the string contains a field.
 * PARAMETERS
 *  srcString : [IN]Source string to be compared.
 *  refString : [IN]Header of the filed
 * RETURNS
 *  If the string contains a field
 * RETURN VALUES
 *  TRUE : If string exists
 *  FALSE : Not contains a field
 *****************************************************************************/
extern U8 mmi_vobj_uti_cmp_filed(S8 *srcString, const S8 *refString);


/*****************************************************************************
 * FUNCTION
 *  mmi_vobj_util_eliminate_qp_CRLF
 * DESCRIPTION
 *  To eliminate quoted printable CRLF sequence.
 * PARAMETERS
 *  string : [IN/OUT]String to be parsed
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_vobj_util_eliminate_qp_CRLF(S8 *string);


/*****************************************************************************
 * FUNCTION
 *  mmi_vobj_uti_get_value
 * DESCRIPTION
 *  Get the string after ":"
 * PARAMETERS
 *  strString : [IN]Input string.
 * RETURNS
 *  Point to the field value
 * RETURN VALUES
 *  pValue : Pointer to the starting point after ":"
 *****************************************************************************/
extern S8 *mmi_vobj_uti_get_value(S8 *strString);


extern void mmi_vobj_register_sms_req(U16);
extern void mmi_vobj_sms_receive_ind(void *info);
extern void mmi_vobj_sms_get_incoming_msg_req(U16 port);
extern void mmi_vobj_sms_get_incoming_msg_rsp(void *info);
extern void mmi_vobj_receive_ind_prompt(MMI_BOOL, U8, U16);
extern void mmi_vboj_entry_recv_popup(U8 type);
extern void mmi_vobj_entry_recv_object_abort(void);
extern void mmi_vobj_abort_object(void);
extern void mmi_vobj_increase_file_buffer_counter(U8);
extern BOOL mmi_vobj_decrease_file_buffer_counter(void);
extern vobj_object_type_enum mmi_vobj_check_curr_file_type(void);
extern void mmi_vobj_get_full_tmp_path_name(U8 index, S8 *result, U8 type);
extern U8 mmi_vobj_util_append_qp_CRLF(S8 *string);
extern U8 mmi_vobj_get_file_buffer_slot(void);
extern void mmi_vobj_increase_sms_register(U16 port);
extern void mmi_vobj_set_sms_register(void);
extern void mmi_vobj_register_sms_rsp(void *info);
extern BOOL mmi_vobj_is_pending_object(void);
extern void mmi_vobj_entry_recv_object(void);
extern void mmi_vobj_clear_one_file_buffer(void);
extern U8 mmi_vobj_get_file_buffer_size(void);
extern S32 mmi_vobj_fs_check_folder(void);
extern void mmi_vobj_entry_irda_processing_screen(void);
extern void mmi_vobj_get_full_path_name(U8 index, S8* result, U8 type);
extern void mmi_vobj_irda_receive_ind(void* path_src, U32 type);
extern void mmi_vobj_irda_receive_ind(void* path_src, U32 type);
extern S8* mmi_vobj_uti_get_value(S8* strString);

extern S8 mmi_vobj_uti_strnicmp(S8*, S8*,S32 );
//extern S32 mmi_vobj_uti_get_line(S8* strOutput,  FS_HANDLE fh);
extern U8 mmi_vobj_uti_cmp_filed(S8* srcString, const S8* refString);
extern void mmi_vobj_util_eliminate_qp_CRLF(S8 *string);
#endif /* _MMI_VOBJECTS_H */ /* END #define _MMI_VOBJECTS_H */

