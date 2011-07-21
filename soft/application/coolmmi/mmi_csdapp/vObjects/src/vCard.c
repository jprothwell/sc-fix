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
 * vCard.C
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
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "mmi_features.h"
#ifndef _MMI_VCARD_C
#define _MMI_VCARD_C

#if defined(__MMI_VCARD__)

/*  Include: MMI header file */
#define __NEWSIMULATOR
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globaldefs.h"
#include "debuginitdef.h"				/* debug info */
#include "commonscreens.h"
#include "fat_fs.h"
#include "filesystemdef.h"
#include "vobjects.h"
#include "phonebookdef.h"
#include "phonebooktypes.h"
#include "phonebookprot.h"
#include "phonebookgprot.h"
#include "vcard.h"
#include "nvramenum.h"
#include "nvramprot.h"
#include "messagesexdcl.h"
#include "irdammigprots.h"
#include "conversions.h"
#include "wgui_categories_idlescreen.h"
#include "app_usedetails.h"
#include "callmanagementgprot.h"

#if defined(__MMI_FILE_MANAGER__)
#include "filemanagerdef.h"		/* error string id */
#include "filemanagergprot.h"
#endif
#include "fmt_struct.h"
#include "wgui_categories_inputs.h"

#ifdef JATAAYU_SUPPORT
#include "jmms_inc.h"
#include "jwap_inc.h"
#include "messagingapi.h"
#include "jdi_communicator.h"
#include "jmms_mmi_cb.h"
#include "jmms_settings.h"
#include "phonebookgprot.h"
#include "jmms_struct_list.h"
#include "custom_nvram_editor_data_item.h"
#include "jprofilehandlertypes.h"
#endif

#ifdef __MMI_OPP_SUPPORT__
#include "btmmiobexgprots.h"
#include "extdevicedefs.h"
#include "btmmiscrgprots.h"
#endif /* __MMI_OPP_SUPPORT__ */ 
#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h" /* mmi_wgui_register_list_item_selected_callback_all */
#endif
#include "mmi_trace.h"
//Renwy add begine 2008/08/06
//#include "btmmigprots.h"
#include "oppmmigprots.h"
#include "extdevicedefs.h"
//Renwy add end 2008/08/06
#if defined(__MMI_GB__)
#include "gbhmi.h"
#endif
#undef __NEWSIMULATOR
#include "app_qp.h"

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif
#include "nvram_data_items.h"
/*
** Typedef
*/
#define VCARD_WRITER_FIELD_BUFFER  500 /* Writer Buffer for single field. over 50 x 9 */
#define VCARD_LINE_BREAK  "\r\n"
#define VCARD_VERSION_21 "2.1"
#define VCARD_VERSION_30 "3.0"
#define VCARD_HEADER "VCARD"
#define VCARD_FILE_SIZE_LIMIT 20000
#define VCARD_ITEM_BREAK  ","

typedef enum
{
    VCARD_PARA_CELL,
    VCARD_PARA_HOME,
    VCARD_PARA_WORK,
    VCARD_PARA_VOICE,
    VCARD_PARA_FAX,
    VCARD_PARA_PAGER,
    VCARD_PARA_PREF,
    VCARD_PARA_TOTAL
} vcard_para_enum;


typedef enum
{
    VCARD_EMAIL_PARA_INTERNET,
    VCARD_EMAIL_PARA_TOTAL
} vcard_email_para_enum;

/* for vcard 3.0 number type */
typedef enum
{
    VCARD_TYPE_PARA_TYPE,
    VCARD_TYPE_PARA_TOTAL
}vcard_type_para_enum;


/*
 * Define
 */

/*
 * Local Variable
 */
vcard_context_struct g_vcard_cntx;

const S8 *g_vcard_call_time[4] =
{
    "",
    "X-IRMC-CALL-DATETIME;DIALED", //ITEM_DIALED_CALL
    "X-IRMC-CALL-DATETIME;MISSED",
    "X-IRMC-CALL-DATETIME;RECEIVED"
};

const S8 *g_vcard_field[VCARD_FIELD_TOTAL] = 
{
    "BEGIN",
    "VERSION",
    "FN",
    "N",
    "TEL",
    "EMAIL",
    "ORG",
    "BDAY",
    "TITLE",
    "URL",
    "NOTE",
    "ADR",
    "PHOTO",
    "REV",
    "LABEL",
    "NICKNAME",
    "MAILER",
    "TZ",
    "GEO",
    "ROLE",
    "LOGO",
    "AGENT",
    "CATEGORIES",
    "PRODID",
    "SORT-STRING",
    "SOUND",
    "UID",
    "CLASS",
    "KEY",
    "X-",
    "END"
};

const S8 *g_vcard_para[VCARD_PARA_TOTAL] = 
{
    "CELL",
    "HOME",
    "WORK",
    "VOICE",
    "FAX",
    "PAGER",
    "PREF"
};

const S8 *g_vcard_email_para[VCARD_EMAIL_PARA_TOTAL] = 
{
    "INTERNET"
};

/* for vcard 3.0 number type */
const S8 *g_vcard_type_para[VCARD_TYPE_PARA_TOTAL] = 
{
    "TYPE"
};

const S8 g_vcard_outlook_phone_head[] =
"Name,Number,Home Number,Company Name,Email Address,Office Number,Birthday";

const S8 g_vcard_outlook_sim_head[] =
"Name,Number";

static U8 g_vcard_number = 0;
static U32 g_vcard_fmgr_id = 0;

#ifdef __MMI_PHB_INFO_FIELD__
S8 g_vcard_write_buff[VCARD_WRITE_BUFF_LEN];
S8 g_vcard_read_buff[VCARD_READ_BUFF_LEN];
#endif /* __MMI_PHB_INFO_FIELD__ */
//else, use ctrl buffer
static U8 g_vcard_email_counter;

#define MAX_VCARD_OUTLOOK_READ_LEN    1024
U8 *g_parse_buff = NULL;
U8 *gp_buff = NULL;
FS_HANDLE g_parse_fh = -1;
BOOL g_is_file_end = FALSE;
U16 g_parse_cnt = 0;

/*
 * Local Function
 */
static void mmi_vcard_free_path_buffer(void);
static void mmi_vcard_entry_forward_option(void);
static void mmi_vcard_exit_forward_option(void);
static U8 mmi_vcard_forward_option_delete_cb(void);

#if !defined(__MMI_MESSAGES_NO_SEND_BY_SMS_OPTION__)
static void mmi_vcard_forward_sms(void);
static void mmi_vcard_forward_sms_call_back(void *flag);
#endif /* !defined(__MMI_MESSAGES_NO_SEND_BY_SMS_OPTION__) */ 

#if defined(__JATAAYU_APP__)
static void mmi_vcard_forward_mms(void);
static void mmi_vcard_forward_mms_call_back(void *param);
#endif /* defined(__JATAAYU_APP__) */ 

#if defined(__MMI_IRDA_SUPPORT__)
static void mmi_vcard_forward_irda(void);
static void mmi_vcard_forward_irda_callback(void *buf_p, U32 result);
#endif /* defined(__MMI_IRDA_SUPPORT__) */ 

#if defined(__MMI_EMAIL__)
static void mmi_vcard_forward_email(void);
static void mmi_vcard_forward_email_call_back(void *param);
#endif /* defined(__MMI_EMAIL__) */ 

#ifdef __MMI_OPP_SUPPORT__
static void mmi_vcard_forward_bt(void);
static void mmi_vcard_forward_bt_callback(U32 result, void *para);
#endif /* __MMI_OPP_SUPPORT__ */ 

#if defined(__MMI_FILE_MANAGER__)
static void mmi_vcard_entry_enter_filename(void);
static void mmi_vcard_entry_enter_filename_back(void);
static void mmi_vcard_exit_enter_filename(void);
static void mmi_vcard_entry_enter_filename_option(void);
static void mmi_vcard_exit_enter_filename_option(void);
static void mmi_vcard_forward_fmgr(void);
void mmi_vcard_forward_fmgr_select_path_done(void *path, int is_short);
static void mmi_vcard_forward_fmgr_pre_enter_filename(void);
static void mmi_vcard_forward_fmgr_enter_filename_done(void);
static S32 mmi_vcard_forward_fmgr_save_file(void);
static void mmi_vcard_forward_fmgr_save_file_done(S32 result);
#endif /* defined(__MMI_FILE_MANAGER__) */ 

static void mmi_vcard_entry_recv_option(void);

static void mmi_vcard_writer_v21_append_name(S8 *object, S8 *temp_buf, S8 *field_name, S8 *field_value);
static void mmi_vcard_writer_v21_append_number(S8 *object, U8 number_type, S8 *field_value, U8 preferred_num);
#ifdef __MMI_PHB_INFO_FIELD__
static void mmi_vcard_writer_v21_append_address(S8 *object, S8 *temp_buf, S8 *field_name, MMI_PHB_ADDRESS_STRUCT *field_value);
#endif

#if defined(__MMI_VCARD_V30__)
static S32 mmi_vcard_writer_v30_to_file(S8 *path, vcard_field_struct *field_content);
static void mmi_vcard_writer_v30_append_name(S8 *object, S8 *temp_buf, S8 *field_name, S8 *field_value);
static void mmi_vcard_writer_v30_append_number(S8 *object, U8 number_type, S8 *field_value, U8 preferred_num);
static void mmi_vcard_writer_v30_append_address(S8 *object, S8 *temp_buf, S8 *field_name, MMI_PHB_ADDRESS_STRUCT *field_value);
#endif /* defined(__MMI_VCARD_V30__) */

static void mmi_vcard_entry_recv_view_detail(void);
static void mmi_vcard_recv_view_detail_pre_entry(void);

static void mmi_vcard_recv_abort_vcard(void);

static BOOL mmi_vcard_recv_parse_file(void);
static void mmi_vcard_recv_save_obj_to_phb(void);
static S32 mmi_vcard_recv_move_obj_to_file(void);
static void mmi_vcard_recv_move_obj_to_file_callback(void *info);

static U8 mmi_vcard_reader_get_para_value(S8 *String, const S8 *para, U8 para_enum, U8 *value);
static U8 mmi_vcard_reader_get_para_number(S8 *String, U8 *type, U8 *pref);
static void mmi_vcard_util_remove_field_delimiter(S8 *str, U16 length);

static void mmi_vcard_util_get_default_file(S8 *path);
#if /*defined(__JATAAYU_APP__) || */defined (__MMI_EMAIL__) || defined (__MMI_IMPS__)
static void mmi_vcard_util_get_default_file_name(S8 *path);
#endif
#if defined(__MMI_OPP_SUPPORT__)
static void mmi_vcard_util_del_default_file(void);
#endif
static U8 mmi_vcard_util_convert_to_system_encoding(S8 *dest, S8 *src);
static void mmi_vcard_util_convert_to_ucs2(S8 *dst, U8 dst_size, S8 *src, U8 encode);

#ifdef __MMI_VCARD_NAME_IN_ENGLISH_FORMAT__
static void mmi_vcard_util_get_name(S8 *str, U16 length);
#else /* __MMI_VCARD_NAME_IN_ENGLISH_FORMAT__ */
static BOOL mmi_vcard_util_has_transform_name(S8 *str, U16 length);
static void mmi_vcard_util_transform_field_delimiter(S8 *str, U16 length);
#endif /* __MMI_VCARD_NAME_IN_ENGLISH_FORMAT__ */

#if defined(__MMI_VCARD_V30__)
static void mmi_vcard_util_v30_number_field_preprocess(S8 *str, U16 length);
#endif /* defined(__MMI_VCARD_V30__) */

/* csj */
static void mmi_vcard_delete_screen(U16 scr_id);
static void mmi_vcard_entry_recv_view_detail_back(void);
static void mmi_vcard_clear_fmagr_screen(void);
static void mmi_vcard_clear_old_receive_history(void);

static void mmi_vcard_entry_recv_option_back(void);

/* for build pbap */
#ifdef __MMI_PBAP_SUPPORT__
static void mmi_vcard_append_calllog_entry_time(S8 *object, MYTIME *start_time, U8 type);
#endif

static U8 mmi_vcard_get_version_parse_line(FS_HANDLE fh);
static U8 mmi_vcard_parse_version_line(S8 *String);
static void mmi_vcard_entry_export_option(void);
static void mmi_vcard_export_forward_fmgr(void);
static void mmi_vcard_entry_import_from_option(void);
static void mmi_vcard_import_from_fmgr(void);
static void mmi_vcard_import_from_fmgr_done(void *path, int is_short);
static void mmi_vcard_outlook_recv_parse_file(void);
static void mmi_vcard_outlook_get_head_line_buff(S8 *object, U8 location);
static void mmi_vcard_outlook_import_parse_file_done(UINT32 errorcode);
static S32 mmi_vcard_outlook_import_parse_one_element(UINT8 elementtype);
static void mmi_vcard_outlook_recv_from_file_mgr(S8 *path);

static S32 mmi_vcard_writer_all_card_to_csvfile(S8 *path);
static S32 mmi_vcard_write_head_to_csvfile(FS_HANDLE fh, S8 *object);
static S32 mmi_vcard_writer_one_record_to_csvfile(FS_HANDLE fh, S8 *object, S8 *temp_buf, vcard_field_struct *field_content);

void mmi_vcard_import_processing(void);
void mmi_vcard_import_exit_processing(void);
/*
 * Global Variable
 */

#ifdef __MMI_PBAP_SUPPORT__
extern MMI_BOOL is_build_for_pbap;
extern MMI_BOOL is_build_append;
extern PbapVcardFormat build_format;
extern PbapVcardFilter build_filter;
extern U16 g_pbap_writer_count;
#endif

extern const S8 *g_vobj_paras[];
extern const S8 *g_vobj_charset[];
extern const S8 *g_vobj_encoding[];

extern U16 g_phb_name_index[];
extern MMI_PHB_ENTRY_BCD_STRUCT PhoneBook[];
extern PHB_OPTIONAL_FIELDS_STRUCT PhoneBookOptionalFields;
extern S8 pbName[];
extern S8 pbNumber[];
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) || defined(__MMI_PHB_USIM_FIELD__)
extern S8 pbHomeNumber[];
extern S8 pbCompanyName[];
extern S8 pbEmailAddress[];
#if defined(__MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__)
extern S8 pbEmailAddress2[];
#endif
extern S8 pbOfficeNumber[];
extern S8 pbFaxNumber[];
#endif /* !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) || defined(__MMI_PHB_USIM_FIELD__) */

#ifdef __MMI_PHB_BIRTHDAY_FIELD__
extern const kal_uint8 g_dt_day_per_mon[];
extern S8 pbDay[];
extern S8 pbMon[];
extern S8 pbYear[];
extern S8 pbBday[];
#endif /* __MMI_PHB_BIRTHDAY_FIELD__ */ 

#if defined(__MMI_PHB_INFO_FIELD__)
extern PHB_INFO_FIELDS_STRUCT PhoneBookInfoFields;
extern S8 pbTitle[];
extern S8 pbUrl[];
extern S8 pbAdrPOBox[];
extern S8 pbAdrExtension[];
extern S8 pbAdrStreet[];
extern S8 pbAdrCity[];
extern S8 pbAdrState[];
extern S8 pbAdrPostalCode[];
extern S8 pbAdrCountry[];
extern S8 pbNote[];
#endif /* defined(__MMI_PHB_INFO_FIELD__) */ 

#if defined(__MMI_PHB_GEOGRAPHIC_FIELD__)
extern MMI_PHB_GEOGRAPHIC_FIELD_STRUCT PhoneBookGeoFields;
#endif

extern U16 TitleStrings[];

extern U8 g_phb_enter_from;
extern MMI_PHB_LIST_VIEW phbListView;
extern void mmi_phb_clear_old_history(void);
extern  INT16 DSM_UnicodeLen(UINT8* pszUnicode);

/*
 * Global Function
 */
#if defined(__MMI_IMPS__) && defined(IMPS_WV13_ENHANCEMENT)
mmi_vcard_send_by_imps_cbk_type g_vcard_send_by_imps_cb;
#endif
#if defined(__MMI_OPP_SUPPORT__)
static void mmi_vcard_highlight_forward_bt(void);
static void mmi_vcard_forward_bt(void);
#endif
/*****************************************************************************
 * FUNCTION
 *  mmi_ucs2str
 * DESCRIPTION
 *  Finds the first occurrence of string2 in string1
 * PARAMETERS
 *  str1        [IN]  string to search in.       
 *  str2        [IN]  string to search for.      
 * RETURNS
 *  returns a pointer to the first occurrence of string2 in
 *  string1, or NULL if string2 does not occur in string1
 *****************************************************************************/
S8 *mmi_ucs2str(const S8 *str1, const S8 *str2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *cp = (S8 *) str1;
    S8 *s1, *s2;
    
    if (!(*str2 || *(str2+1)))
    {    
        return((S8 *)str1);
    }
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (*cp || *(cp+1))
    {
        s1 = cp;
        s2 = (S8 *) str2;
        
        while ((*s1 || *(s1+1)) 
               && (*s2 || *(s2+1)) 
               && !((*s1-*s2) || (*(s1+1)-*(s2+1))))
        {
            s1 += 2;
            s2 += 2;
        }
        
        if (!(*s2 || *(s2+1)))
        {
            return(cp);
        }
        
        cp += 2;
    }
    
    return NULL;    
}

 BOOL mmi_fmgr_is_filename_valid(const S8* string)
{
    return TRUE;
}
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
void mmi_vcard_init(void)
{
    S16 pError;
    MMI_PHB_VCARD_STRUCT myCard;
    U8 i;
    mmi_trace(g_sw_PHB, "VCARD func: %s", __FUNCTION__);
    WriteRecord(NVRAM_EF_PHB_VCARD_LID, 1, (void*)&myCard, MMI_PHB_VCARD_RECORD_SIZE, &pError);
    g_vcard_cntx.processing = FALSE;
	/*vcard forward highlight handler.*/
#if !defined(__MMI_MESSAGES_NO_SEND_BY_SMS_OPTION__)
    SetHiliteHandler(MENU_ID_VCARD_FORWARD_SMS,		mmi_vcard_highlight_forward_sms);
#endif
#if defined(__JATAAYU_APP__)
    SetHiliteHandler(MENU_ID_VCARD_FORWARD_MMS,		mmi_vcard_highlight_forward_mms);
#endif
#if  defined(__MMI_IRDA_SUPPORT__)
    SetHiliteHandler(MENU_ID_VCARD_FORWARD_IRDA,		mmi_vcard_highlight_forward_irda);
#endif
#if  defined(EMAIL_APP_SUPPORT)
    SetHiliteHandler(MENU_ID_VCARD_FORWARD_EMAIL,		mmi_vcard_highlight_forward_email);
#endif

#if (defined(__MMI_ACCEL_OPP_SUPPORT__) ||  defined(__MMI_OPP_SUPPORT__)) 
    SetHiliteHandler(MENU_ID_VCARD_FORWARD_BT, mmi_vcard_highlight_forward_bt);
#endif 
    /*vcard save file enter file name option highlight handler*/
#if defined(__MMI_FILE_MANAGER__)
    SetHiliteHandler(MENU_ID_VCARD_FORWARD_FMGR, mmi_vcard_highlight_forward_fmgr);
    SetHiliteHandler(MENU_ID_VCARD_ENTER_FILENAME_OPT_DONE, mmi_vcard_highlight_enter_filename_done);
    SetHiliteHandler(MENU_ID_VCARD_ENTER_FILENAME_OPT_INPUT_METHOD, mmi_vcard_highlight_enter_filename_input_method);
#endif

    /*vcard receive option highlight handler.*/
for(i = 0; i < MMI_SIM_NUMBER; i++)
{
    SetHiliteHandler(MENU_ID_VCARD_RECV_OPTION_TO_SIM + i, mmi_vcard_highlight_recv_save_to_sim);
}
    SetHiliteHandler(MENU_ID_VCARD_RECV_OPTION_TO_PHONE, mmi_vcard_highlight_recv_save_to_phone);
#if defined(__MMI_FILE_MANAGER__) && defined(__FS_MOVE_SUPPORT__)
    SetHiliteHandler(MENU_ID_VCARD_RECV_OPTION_FMGR, mmi_vcard_highlight_recv_save_to_fmgr);
#endif

}

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
void mmi_vcard_deinit(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_vcard_free_path_buffer();
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_recv_set_result
 * DESCRIPTION
 *  set receive vcard result
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_recv_set_result(U8 result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_vobj_cntx.info = result;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_free_path_buffer
 * DESCRIPTION
 *  Free allocate file path buffer when exit vcard application.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_free_path_buffer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_vcard_cntx.FilePathPtr)
    {
        OslMfree(g_vcard_cntx.FilePathPtr);
        g_vcard_cntx.FilePathPtr = NULL;
    }
    if (g_vcard_cntx.FileNamePtr)
    {
        OslMfree(g_vcard_cntx.FileNamePtr);
        g_vcard_cntx.FileNamePtr = NULL;
    }
    if (g_vobj_cntx.AppFilePathPtr)
    {
        OslMfree(g_vobj_cntx.AppFilePathPtr);
        g_vobj_cntx.AppFilePathPtr = NULL;
    }
    mmi_vcard_import_free_buffer();
    g_vobj_cntx.recv_callback = NULL;
}

/*--------------------------------------------------BEGIN vCard Forward operation ---------------------------------------------------*/


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
void mmi_vcard_forward_option_my_card(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_vcard_cntx.CardType = MMI_VCARD_SEND_MY_CARD;
    mmi_vcard_entry_forward_option();
}


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
void mmi_vcard_forward_option_select_card(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_vcard_cntx.CardType = MMI_VCARD_SEND_SELECT_CARD;

    mmi_vcard_entry_forward_option();
    //DeleteScreenIfPresent(SCR_PBOOK_ENTRY_OPTIONS);
#ifdef __MMI_OP11_HOMESCREEN__
/* under construction !*/
#endif
}


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
MMI_BOOL mmi_vcard_forward_menu_check(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_BOOL is_hide = MMI_TRUE;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if !defined(__MMI_MESSAGES_NO_SEND_BY_SMS_OPTION__)
    if (mmi_frm_sms_is_hide_send_sms_menu_item() == TRUE)
    {
        mmi_frm_hide_menu_item(MENU_ID_VCARD_FORWARD_SMS);        
    }
    else
    {
        mmi_frm_unhide_menu_item(MENU_ID_VCARD_FORWARD_SMS);
        is_hide = MMI_FALSE;
    }
#endif

#if defined(MMS_SUPPORT)
    if (!mms_is_ready() && GetTotalCallCount())
    {
        mmi_frm_hide_menu_item(MENU_ID_VCARD_FORWARD_MMS);    
    }
    else
    {
        mmi_frm_unhide_menu_item(MENU_ID_VCARD_FORWARD_MMS);
        is_hide = MMI_FALSE;
    }
#endif

#if  defined(__MMI_EMAIL__)
    if (!mmi_email_is_email_can_forward())
    {
        mmi_frm_hide_menu_item(MENU_ID_VCARD_FORWARD_EMAIL);
    }
    else
    {
        mmi_frm_unhide_menu_item(MENU_ID_VCARD_FORWARD_EMAIL);
        is_hide = MMI_FALSE;
    }
#endif

#if  defined(__MMI_IRDA_SUPPORT__)
#if defined (__FLIGHT_MODE_SUPPORT__)
    if (mmi_bootup_get_active_flight_mode() == 1)
    {
        mmi_frm_hide_menu_item(MENU_ID_VCARD_FORWARD_IRDA);
    }
    else
#endif
    {
        mmi_frm_unhide_menu_item(MENU_ID_VCARD_FORWARD_IRDA);
        is_hide = MMI_FALSE;
    }
#endif

#if defined(__MMI_OPP_SUPPORT__)
    if (mmi_bt_is_to_display_bt_menu() == MMI_FALSE)
    {
        mmi_frm_hide_menu_item(MENU_ID_VCARD_FORWARD_BT);
    }
    else
    {
        mmi_frm_unhide_menu_item(MENU_ID_VCARD_FORWARD_BT);
        is_hide = MMI_FALSE;
    }
#endif
    return is_hide;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_entry_forward_option
 * DESCRIPTION
 *  Entry function of vcard forward option
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_entry_forward_option(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8* guiBuffer = NULL;
    U16 nStrItemList[5];
    U16 nNumofItem;

    MMI_PHB_VCARD_STRUCT myCard;
    S16 pError;   
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_PHB, "VCARD func: %s, CardType = %d", __FUNCTION__, g_vcard_cntx.CardType);

#ifdef __USB_IN_NORMAL_MODE__
    S8 path[VOBJ_MAX_PATH_LENGTH];
    mmi_vobj_get_full_path_name(0, path, MMI_VOBJ_UNRECOGNIZED);   
    if (mmi_usb_is_in_mass_storage_mode())
    {
        /* in mass storage mode */
        if (mmi_usb_check_path_exported((PS8) path))
        {
            /* phone drive is exported, cant use this app */
            mmi_usb_app_unavailable_popup(0);   /* pass 0 will show default string */
            return;
        }
    }
#endif /* __USB_IN_NORMAL_MODE__ */    

    /* Check if the name field is empy before send vCard */
    if (g_vcard_cntx.CardType == MMI_VCARD_SEND_MY_CARD)
    {
        ReadRecord(NVRAM_EF_PHB_VCARD_LID, 1, (void*)&myCard, MMI_PHB_VCARD_RECORD_SIZE, &pError);
        
        if(!pfnUnicodeStrlen(myCard.name))
        {
            DisplayPopup((PU8)GetString(STR_ID_VCARD_NAME_EMPTY), IMG_GLOBAL_ERROR, TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
            return;
        }
    }
    else
    {
        if(!pfnUnicodeStrlen((S8*)PhoneBook[g_phb_name_index[g_phb_cntx.active_index]].alpha_id.name))
        {
            DisplayPopup((PU8)GetString(STR_ID_VCARD_NAME_EMPTY), IMG_GLOBAL_ERROR, TRUE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
            return;
        }
    }
    
    mmi_vcard_clear_old_receive_history();
    mmi_vcard_init();   /* Regiser highlight handler again. */
    EntryNewScreen(SCR_ID_VCARD_FORWARD_OPTION, mmi_vcard_exit_forward_option, mmi_vcard_entry_forward_option, NULL);
    SetDelScrnIDCallbackHandler(SCR_ID_VCARD_FORWARD_OPTION, (HistoryDelCBPtr)mmi_vcard_forward_option_delete_cb);

    mmi_vcard_forward_menu_check();

    guiBuffer = GetCurrGuiBuffer(SCR_ID_VCARD_FORWARD_OPTION);
    nNumofItem = GetNumOfChild_Ext(MENU_ID_VCARD_FORWARD_OPTION);

    GetSequenceStringIds_Ext(MENU_ID_VCARD_FORWARD_OPTION, nStrItemList);
    SetParentHandler(MENU_ID_VCARD_FORWARD_OPTION);
    RegisterHighlightHandler(ExecuteCurrHiliteHandler);

    ShowCategory15Screen(STR_SCR_VIEW_OPTIONS_CAPTION, IMG_SCR_PBOOK_CAPTION,
    					  STR_GLOBAL_OK, IMG_GLOBAL_OK,
    					  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
    					  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
    					  LIST_MENU, 0, guiBuffer);

    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_exit_forward_option
 * DESCRIPTION
 *  Exit function of vcard forward option.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_exit_forward_option(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_phb_cntx.end_scr_id = SCR_ID_VCARD_FORWARD_OPTION;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_option_delete_cb
 * DESCRIPTION
 *  Delete cb function of vcard forward option.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern BOOL MMI_FS_IsExist(const UINT8 * FileName);
static U8 mmi_vcard_forward_option_delete_cb(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 path[VOBJ_MAX_PATH_LENGTH];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_vcard_util_get_default_file(path);   
    if(MMI_FS_IsExist(path))
    {
        MMI_FS_Delete(path);
    }    
    return MMI_FALSE;
}


#if !defined(__MMI_MESSAGES_NO_SEND_BY_SMS_OPTION__)
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_highlight_forward_sms
 * DESCRIPTION
 *  Highlight function of forward vcard through SMS
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_highlight_forward_sms(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(mmi_vcard_forward_sms, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

    SetKeyHandler(mmi_vcard_forward_sms, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_sms
 * DESCRIPTION
 *  forward vcard thorugh SMS, will create vcard object into file and call SMS function to send.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_forward_sms(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *path;
    S32 result;
    S32 check_result = mmi_vobj_fs_check_folder();

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Create file path and folder. */
    if (check_result == FS_NO_ERROR)
    {
        path = OslMalloc(VOBJ_MAX_PATH_LENGTH);
        mmi_vcard_util_get_default_file(path);

        if (g_vcard_cntx.CardType == MMI_VCARD_SEND_MY_CARD)
        {
            result = mmi_vcard_writer_v21_mycard_to_file(path);
        }
        else
        {
            result = mmi_vcard_writer_v21_select_card_to_file(path);
        }
        if (result != VOBJ_ERR_NO_ERROR)
        {
            MMI_FS_Delete((U8*) path);
            DisplayPopup((PU8)GetString(GetFileSystemErrorString(result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
            OslMfree(path);
            return;
        }
        mmi_ems_vobject_send_by_ems(VOBJ_SRC_PORT, VOBJ_VCARD_PORT, path, mmi_vcard_forward_sms_call_back);
        OslMfree(path);
    }
    else
    {
        /* Error Handling here. */
        DisplayPopup((PU8)GetString(GetFileSystemErrorString(check_result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
    }

}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_sms_call_back
 * DESCRIPTION
 *  call back function of send vcard through SMS, will get error cause from SMS
 *  should go back to proper screen when send finish.
 * PARAMETERS
 *  flag        [IN]        Result of send vcard through SMS
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_forward_sms_call_back(void *flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_vcard_util_del_default_file();
    DeleteScreenIfPresent(SCR_ID_VCARD_FORWARD_OPTION);

    //if(g_vcard_cntx.CardType == MMI_VCARD_SEND_MY_CARD)
    //DeleteUptoScrID(SCR_ID_PHB_VCARD_MYCARD);
    //else
    //DeleteUptoScrID(SCR_PBOOK_ENTRY_OPTIONS);
}
#endif /* !defined(__MMI_MESSAGES_NO_SEND_BY_SMS_OPTION__) */ // #if !defined(__MMI_MESSAGES_NO_SEND_BY_SMS_OPTION__)

#if defined(__JATAAYU_APP__)
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_highlight_forward_mms
 * DESCRIPTION
 *  Highlight function for send vcard through MMS
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_highlight_forward_mms(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(mmi_vcard_forward_mms, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

    SetKeyHandler(mmi_vcard_forward_mms, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_mms
 * DESCRIPTION
 *  This function is just for template.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_forward_mms(void)
{
#if 0
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *path;
    S32 result;
    S32 check_result = mmi_vobj_fs_check_folder();
    mms_content_entry_struct data;
    FS_HANDLE fh;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Create file path and folder. */
    if (check_result == FS_NO_ERROR)
    {
        path = OslMalloc(VOBJ_MAX_PATH_LENGTH);
        mmi_vcard_util_get_default_file_name(path);
        fh = MMI_FS_Open((U8*) path, FS_CREATE | FS_READ_WRITE);
        MMI_FS_Close(fh);
        if (fh == FS_INVALID_FILENAME)
        {
            mmi_vcard_util_get_default_file(path);
        }
        if (g_vcard_cntx.CardType == MMI_VCARD_SEND_MY_CARD)
        {
            result = mmi_vcard_writer_v21_mycard_to_file(path);
        }
        else
        {
            result = mmi_vcard_writer_v21_select_card_to_file(path);
        }
        if (result != VOBJ_ERR_NO_ERROR)
        {
            MMI_FS_Delete((U8*) path);
            DisplayPopup((PU8)GetString(GetFileSystemErrorString(result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
            OslMfree(path);
            return;
        }

        memset(&data, 0x00, sizeof(mms_content_entry_struct));
        data.mode = MMS_NEW_FILE;
        data.file_path = path;
        data.callback = mmi_vcard_forward_mms_call_back;
        data.callback_param = path;
        mms_content_insert_hdlr_with_callback(&data);
    }
    else
    {
        /* Error Handling here. */
        DisplayPopup((PU8)GetString(GetFileSystemErrorString(check_result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
    }
#endif 
    mmi_vcard_forward_mms_call_back(NULL);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_mms_call_back
 * DESCRIPTION
 *  call back function of send vcard through MMS, will get error cause from MMS
 *  should go back to proper screen when send finish.
 * PARAMETERS
 *  flag        [IN]        Result of send vcard through SMS
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_forward_mms_call_back(void *param)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_FS_Delete((U8*) param);
    OslMfree(param);
    DeleteScreenIfPresent(SCR_ID_VCARD_FORWARD_OPTION);

    //if(g_vcard_cntx.CardType == MMI_VCARD_SEND_MY_CARD)
    //DeleteUptoScrID(SCR_ID_PHB_VCARD_MYCARD);
    //else
    //DeleteUptoScrID(SCR_PBOOK_ENTRY_OPTIONS);
}
#endif /* defined(__JATAAYU_APP__) */ /* END #if defined(__JATAAYU_APP__) */

#if defined(__MMI_IRDA_SUPPORT__)


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_highlight_forward_irda
 * DESCRIPTION
 *  highlight function for forward vcard to IrDA
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_highlight_forward_irda(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(mmi_vcard_forward_irda, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

    SetKeyHandler(mmi_vcard_forward_irda, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_irda
 * DESCRIPTION
 *  Forward vcard thorugh IrDA, will create vcard object into file and call IrDA function to send.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_forward_irda(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *path;
    S32 result;
    S32 check_result = mmi_vobj_fs_check_folder();

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Create file path and folder. */
    if (check_result == FS_NO_ERROR)
    {
        path = OslMalloc(VOBJ_MAX_PATH_LENGTH);
        mmi_vcard_util_get_default_file(path);

		if(pfnUnicodeStrlen(path) > 0)
        {
            if (g_vcard_cntx.CardType == MMI_VCARD_SEND_MY_CARD)
            {
                result = mmi_vcard_writer_v21_mycard_to_file(path);
            }
            else
            {
                result = mmi_vcard_writer_v21_select_card_to_file(path);
            }

            if (result != VOBJ_ERR_NO_ERROR)
            {
                MMI_FS_Delete((U8*) path);
				DisplayPopup((PU8)GetString(GetFileSystemErrorString(result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
                OslMfree(path);
                return;
            }

            if (MMI_IRDA_ERR_SUCCESS == mmi_irda_send_file_obj(path, mmi_vcard_forward_irda_callback))
            {
                mmi_vobj_entry_irda_processing_screen();
            }
        }
        else
        {
        }

        OslMfree(path);
    }
    else
    {
        /* Error Handling Here. */
		DisplayPopup((PU8)GetString(GetFileSystemErrorString(check_result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_irda_callback
 * DESCRIPTION
 *  call back function for forward vcard through IrDA, will bring error cause.
 *  If send vcard through IrDA by using SRAM instead of file, also free buffer here.
 * PARAMETERS
 *  buf_p       [IN]        Buffer to release
 *  result      [IN]        Error cause
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_forward_irda_callback(void *buf_p, U32 result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_vcard_util_del_default_file();

    /* Free buffer if send to IrDA by using SRAM */
    if (buf_p != NULL)
    {
        OslMfree(buf_p);
        buf_p = NULL;
    }

    if (result == MMI_IRDA_ERR_SUCCESS)
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
	else
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED), IMG_GLOBAL_ERROR, TRUE, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);

    DeleteBetweenScreen(SCR_ID_VCARD_FORWARD_OPTION, SCR_ID_VOBJ_IRDA_FORWARD_PROCESSING);

    //if(g_vcard_cntx.CardType == MMI_VCARD_SEND_MY_CARD)
    //DeleteUptoScrID(SCR_ID_PHB_VCARD_MYCARD);
    //else
    //DeleteUptoScrID(SCR_PBOOK_ENTRY_OPTIONS);

}
#endif /* defined(__MMI_IRDA_SUPPORT__) */ /* END of #if  defined(__MMI_IRDA_SUPPORT__) */

#if defined(__MMI_OPP_SUPPORT__)


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_highlight_forward_bt
 * DESCRIPTION
 *  highlight function for forward vcard to Blue Tooth
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_highlight_forward_bt(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(mmi_vcard_forward_bt, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

    SetKeyHandler(mmi_vcard_forward_bt, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_bt
 * DESCRIPTION
 *  Forward vcard thorugh Blue Tooth, will create vcard object into file and call IrDA function to send.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_forward_bt(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *path;
    S32 result;
    S32 check_result = mmi_vobj_fs_check_folder();

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Create file path and folder. */
    if (check_result == FS_NO_ERROR)
    {
        path = OslMalloc(VOBJ_MAX_PATH_LENGTH);
        mmi_vcard_util_get_default_file(path);

        if (pfnUnicodeStrlen(path) > 0)
        {
            if (g_vcard_cntx.CardType == MMI_VCARD_SEND_MY_CARD)
            {
                result = mmi_vcard_writer_v21_mycard_to_file(path);
            }
            else
            {
                result = mmi_vcard_writer_v21_select_card_to_file(path);
            }

            if (result != VOBJ_ERR_NO_ERROR)
            {
                MMI_FS_Delete((U8*) path);
                DisplayPopup(
                    (PU8) GetString(GetFileSystemErrorString(result)),
                    IMG_GLOBAL_ERROR,
                    1,
                    UI_POPUP_NOTIFYDURATION_TIME,
                    (U8) ERROR_TONE);
                OslMfree(path);
                return;
            }
            
            {
                btsend_obj_struct obj;
                btsend_app_struct app;

                memset(&obj, 0, sizeof(obj));

                //obj.data.obj_type = BTSEND_USE_FILE;
                obj.data.u.file.path = (U16 *)path;

                app.id = (U8)APP_PHONEBOOK;
                app.callback = mmi_vcard_forward_bt_callback;
            
                //mmi_bt_send_object(&obj, &app, BTSEND_PROF_OPP, BTSEND_OPT_NORMAL);
            }
        }
        else
        {
            DisplayPopup(
                (PU8) GetString(STR_GLOBAL_ERROR),
                IMG_GLOBAL_ERROR,
                1,
                UI_POPUP_NOTIFYDURATION_TIME,
                (U8) ERROR_TONE);
        }

        OslMfree(path);
    }
    else
    {
        /* Error Handling Here. */
        DisplayPopup(
            (PU8) GetString(GetFileSystemErrorString(check_result)),
            IMG_GLOBAL_ERROR,
            1,
            UI_POPUP_NOTIFYDURATION_TIME,
            (U8) ERROR_TONE);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_bt_callback
 * DESCRIPTION
 *  call back function for forward vcard through Blue Tooth, will bring error cause.
 *  If send vcard through Blue Tooth by using SRAM instead of file, also free buffer here.
 * PARAMETERS
 *  result      [IN]        Error cause
 *  para        [IN]        parameter
 * RETURNS
 *  void
 *****************************************************************************/
/* Bluetooth sending API revise */
static void mmi_vcard_forward_bt_callback(U32 result, void *para)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_PHB, "VCARD func:%s", __FUNCTION__);
    mmi_vcard_util_del_default_file();
    DeleteScreenIfPresent(SCR_ID_VCARD_FORWARD_OPTION);
}
#endif /* defined(__MMI_OPP_SUPPORT__) */ 


#if defined(__MMI_EMAIL__)
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_highlight_forward_email
 * DESCRIPTION
 *  Highlight function for forward to email app.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_highlight_forward_email(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(mmi_vcard_forward_email, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

    SetKeyHandler(mmi_vcard_forward_email, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_email
 * DESCRIPTION
 *  This function is just for template.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_forward_email(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    S8 *path;
    S32 result;
    S32 check_result = mmi_vobj_fs_check_folder();
    FS_HANDLE fh;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Create file path and folder. */
    if (check_result == FS_NO_ERROR)
    {
        path = OslMalloc(VOBJ_MAX_PATH_LENGTH);
        mmi_vcard_util_get_default_file_name(path);
        fh = MMI_FS_Open((U8*) path, FS_CREATE | FS_READ_WRITE);
        MMI_FS_Close(fh);
        if (fh == FS_INVALID_FILENAME)
        {
            mmi_vcard_util_get_default_file(path);
        }
        if (g_vcard_cntx.CardType == MMI_VCARD_SEND_MY_CARD)
        {
            result = mmi_vcard_writer_v21_mycard_to_file(path);
        }
        else
        {
            result = mmi_vcard_writer_v21_select_card_to_file(path);
        }
        if (result != VOBJ_ERR_NO_ERROR)
        {
            MMI_FS_Delete((U8*) path);
            DisplayPopup((PU8)GetString(GetFileSystemErrorString(result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
            OslMfree(path);
            return;
        }
        mmi_email_app_send_attch_from_vobj((S8*) path, mmi_vcard_forward_email_call_back);
        ClearDelScrnIDCallbackHandler(SCR_ID_VCARD_FORWARD_OPTION, (HistoryDelCBPtr)mmi_vcard_forward_option_delete_cb);
        DeleteScreenIfPresent(SCR_ID_VCARD_FORWARD_OPTION);
    }
    else
    {
        /* Error Handling Here. */
        DisplayPopup((PU8)GetString(GetFileSystemErrorString(check_result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_email_call_back
 * DESCRIPTION
 *  call back function of send vcard through Email, delete the file and free the buffer
 *  should go back to proper screen when send finish.
 * PARAMETERS
 *  flag        [IN]        Result of send vcard through SMS
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_forward_email_call_back(void *param)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_FS_Delete((U8*) param);
    OslMfree(param);
}
#endif /* defined(__MMI_EMAIL__) */


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
void mmi_vcard_forward_imps(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *path;
    S32 result;
    S32 check_result = mmi_vobj_fs_check_folder();
    FS_HANDLE fh;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Create file path and folder. */
    if (check_result == FS_NO_ERROR)
    {
        path = OslMalloc(VOBJ_MAX_PATH_LENGTH);
        g_vcard_cntx.CardType = MMI_VCARD_SEND_SELECT_CARD_SECOND_LEVEL;
        mmi_vcard_util_get_default_file_name(path);
        fh = MMI_FS_Open((U8*) path, FS_CREATE | FS_READ_WRITE);
        MMI_FS_Close(fh);
        
        if (fh == FS_INVALID_FILENAME)
        {
            mmi_vcard_util_get_default_file(path);
        }

        result = mmi_vcard_writer_v21_select_card_to_file(path);
            
        if (result != VOBJ_ERR_NO_ERROR)
        {
            MMI_FS_Delete((U8*) path);
            DisplayPopup((PU8)GetString(GetFileSystemErrorString(result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
            OslMfree(path);
            return;
        }
        g_vcard_send_by_imps_cb(path);
        OslMfree(path);
        g_vcard_send_by_imps_cb = NULL;
    }
    else
    {
        /* Error Handling here. */
        DisplayPopup((PU8)GetString(GetFileSystemErrorString(check_result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
    }
}
#endif /* defined(__MMI_IMPS__) && defined(IMPS_WV13_ENHANCEMENT) */


#if defined(__MMI_FILE_MANAGER__)
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_highlight_forward_fmgr
 * DESCRIPTION
 *  highlight function for save vcard to file manager.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_highlight_forward_fmgr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(mmi_vcard_forward_fmgr, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

    SetKeyHandler(mmi_vcard_forward_fmgr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

    g_vcard_cntx.FileLoc = MMI_VCARD_SAVE_TO_FILE_FROM_PHB;

}

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
void mmi_vcard_copy_forward_fmgr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_vcard_cntx.FileLoc = MMI_VCARD_SAVE_TO_FILE_FROM_PHB;
    g_vcard_cntx.CardType = MMI_VCARD_SEND_SELECT_CARD;
    if (!pfnUnicodeStrlen((S8*) PhoneBook[g_phb_name_index[g_phb_cntx.active_index]].alpha_id.name))
    {
        DisplayPopup((PU8)GetString(GetFileSystemErrorString(STR_ID_VCARD_NAME_EMPTY)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
    }
    else
    {
        mmi_vcard_forward_fmgr();
    }
    DeleteNScrId(SCR_PBOOK_ENTRY_OPTIONS);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_highlight_enter_filename_done
 * DESCRIPTION
 *  highlight function for forward vcard to file manger enter name done.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_highlight_enter_filename_done(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(mmi_vcard_forward_fmgr_enter_filename_done, KEY_EVENT_UP);

    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_highlight_enter_filename_input_method
 * DESCRIPTION
 *  highlight function for save vcard to file manager enter name change input method.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_highlight_enter_filename_input_method(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetInputMethodAndDoneCaptionIcon(IMG_SCR_PBOOK_CAPTION);
    SetLeftSoftkeyFunction(EntryInputMethodScreen, KEY_EVENT_UP);

    SetKeyHandler(EntryInputMethodScreen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

#if defined(__MMI_GB__)
/*****************************************************************************
* FUNCTION
*  mmi_frm_set_highlight_handler
* DESCRIPTION
*   Register highlight generic handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
static void mmi_vcard_frm_set_highlight_handler (void)
{
    SetHiliteHandler(MENU_ID_VCARD_ENTER_FILENAME_OPT_DONE, mmi_vcard_highlight_enter_filename_done);
    SetHiliteHandler(MENU_ID_VCARD_ENTER_FILENAME_OPT_INPUT_METHOD, mmi_vcard_highlight_enter_filename_input_method);
}
#endif

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_entry_enter_filename
 * DESCRIPTION
 *  entry function for enter file name after select file path from file manager.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_entry_enter_filename(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* ExecuteCurrExitHandler(SCR_ID_VCARD_ENTER_FILENAME, mmi_vcard_exit_enter_filename, mmi_vcard_entry_enter_filename,NULL); */
    EntryNewScreen(SCR_ID_VCARD_ENTER_FILENAME, mmi_vcard_exit_enter_filename, mmi_vcard_entry_enter_filename, NULL);
    SetParentHandler(0);
    guiBuffer = GetCurrGuiBuffer(SCR_ID_VCARD_ENTER_FILENAME);

    ShowCategory5Screen(STR_ID_VOBJ_FILENAME, IMG_SCR_PBOOK_CAPTION,
    					  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
    					  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
    					  INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
    					  (U8*)g_vcard_cntx.FileNamePtr,
    					  VOBJ_MAX_FILENAME_LENGTH/ENCODING_LENGTH, /*Use same length as other application. */
    					  guiBuffer);					/*===> also change related size of g_vcard_cntx.FileNamePtr*/

#if defined(__MMI_GB__)
    mmi_vcard_frm_set_highlight_handler();
    GBSetInputboxLSKFunction(mmi_vcard_entry_enter_filename_option); 
#endif

    SetLeftSoftkeyFunction(mmi_vcard_entry_enter_filename_option, KEY_EVENT_UP);
    SetCategory5RightSoftkeyFunction(mmi_vcard_entry_enter_filename_back, KEY_EVENT_UP);
    /* SetGenericExitHandler(SCR_ID_VCARD_ENTER_FILENAME, mmi_vcard_exit_enter_filename, mmi_vcard_entry_enter_filename); */
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_entry_enter_filename_back
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_entry_enter_filename_back(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_OTHER_APPS)
    {
        mmi_vcard_recv_from_other_apps_callback();
        mmi_vcard_free_path_buffer();
        GoBackHistory();
    }
    else if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_FILE_MANAGER)
    {
        if(g_vcard_cntx.ImportLoc != MMI_VCARD_IMPORT_FROM_NONE)
        {
            GoBackToHistory(SCR_ID_VCARD_IMPORT_FROM_OPTION);
        }
    }
    else
    {
        if (g_vcard_cntx.FileLoc == MMI_VCARD_SAVE_TO_FILE_FROM_PHB)
        {
            if (g_vcard_cntx.CardType == MMI_VCARD_SEND_MY_CARD)
            {
                GoBackToHistory(SCR_ID_PHB_VCARD_MYCARD);
            }
            else if (g_vcard_cntx.CardType == MMI_VCARD_SEND_SELECT_CARD)
            {
                GoBackToHistory(SCR_PBOOK_ENTRY_OPTIONS);
            }
            else if (g_vcard_cntx.CardType == MMI_VCARD_SEND_VCARD_TATOL)
            {
                GoBackToHistory(SCR_ID_VCARD_EXPORT_OPTION);
            }
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_exit_enter_filename
 * DESCRIPTION
 *  Exit function for enter file name. Will keep screen ID only when enter from phonebook.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_exit_enter_filename(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(g_vcard_cntx.FileLoc == MMI_VCARD_SAVE_TO_FILE_FROM_PHB)
    {   
        g_phb_cntx.end_scr_id = SCR_ID_VCARD_ENTER_FILENAME;
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_entry_enter_filename_option
 * DESCRIPTION
 *  entry function of options for save vcard to file.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_entry_enter_filename_option(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    U16 ItemList[2];
    U16 nItems;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* ExecuteCurrExitHandler(); */
    EntryNewScreen(
        SCR_ID_VCARD_ENTER_FILENAME_OPT,
        mmi_vcard_exit_enter_filename_option,
        mmi_vcard_entry_enter_filename_option,
        NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_ID_VCARD_ENTER_FILENAME_OPT);
    SetParentHandler(MENU_ID_VCARD_ENTER_FILENAME_OPT);

    RegisterHighlightHandler(ExecuteCurrHiliteHandler);

    nItems = GetNumOfChild(MENU_ID_VCARD_ENTER_FILENAME_OPT);
    GetSequenceStringIds(MENU_ID_VCARD_ENTER_FILENAME_OPT, ItemList);
    RegisterHighlightHandler(ExecuteCurrHiliteHandler);

    ShowCategory52Screen(
        STR_GLOBAL_OPTIONS,
        IMG_SCR_PBOOK_CAPTION,
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        nItems,
        ItemList,
        (PU16) gIndexIconsImageList,
        0,
        0,
        0,
        guiBuffer);

        /* Register function for right soft key */
        SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
        SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    /* SetGenericExitHandler(SCR_ID_VCARD_ENTER_FILENAME_OPT, mmi_vcard_exit_enter_filename_option, mmi_vcard_entry_enter_filename_option); */
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_exit_enter_filename_option
 * DESCRIPTION
 *  exit function of options for save vcard to file, only keep screen id when enter from phonebook.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_exit_enter_filename_option(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(g_vcard_cntx.FileLoc == MMI_VCARD_SAVE_TO_FILE_FROM_PHB)
    {   
        g_phb_cntx.end_scr_id = SCR_ID_VCARD_ENTER_FILENAME_OPT;
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_fmgr
 * DESCRIPTION
 *  save vcard to file manager, file manager will take over screen after calling.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_forward_fmgr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FMGR_FILTER filter;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    FMGR_FILTER_INIT(&filter);
    FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);

    mmi_fmgr_select_path_and_enter(
        APP_PHONEBOOK,
        FMGR_SELECT_FOLDER,
        filter,
        (S8*) L"root",
        mmi_vcard_forward_fmgr_select_path_done);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_fmgr_select_path_done
 * DESCRIPTION
 *  call back function of save vcard to file manager, will bring back selected file path
 * PARAMETERS
 *  path            [IN]        Selected file path
 *  is_short        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_forward_fmgr_select_path_done(void *path, int is_short)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 ansiiTmp[VOBJ_MAX_PATH_LENGTH];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_PHB, "VCARD Func:%s, fileloc = %d, cardtype = %d",  __FUNCTION__, g_vcard_cntx.FileLoc, g_vcard_cntx.CardType);
    
    mmi_vcard_reset_fmagr_flag();
    if (path == NULL)   /* User does not select file and go back directly. Go Back To Upto forward option or recv option. */
    {
        if (g_vcard_cntx.FileLoc == MMI_VCARD_SAVE_TO_FILE_FROM_PHB)
        {
            if( g_vcard_cntx.CardType == MMI_VCARD_SEND_VCARD_TATOL)
            {
                DeleteScreenIfPresent(SCR_ID_VCARD_EXPORT_OPTION);
                GoBackHistory();
            }
            else
            {
                DeleteScreenIfPresent(SCR_ID_VCARD_FORWARD_OPTION);
                GoBackHistory();            
            }
        }
        else    /* MMI_VCARD_SAVE_TO_FILE_FROM_RECV */
        {
            GoBackToHistory(SCR_ID_VCARD_RECV_OPT);
        }
         return;
    }

    if (g_vcard_cntx.FilePathPtr == NULL)   /* Should free after save or free in deinit func. */
    {
        g_vcard_cntx.FilePathPtr = OslMalloc(VOBJ_MAX_PATH_LENGTH);
    }

    pfnUnicodeStrcpy(g_vcard_cntx.FilePathPtr, (S8*)path);
    
    UnicodeToAnsii(ansiiTmp, g_vcard_cntx.FilePathPtr);
    mmi_trace(g_sw_PHB, "VCARD Func:%s, filepath = %s", __FUNCTION__, ansiiTmp);
    
    /* Enter File Name before save */
    mmi_vcard_forward_fmgr_pre_enter_filename();

#if 0
    /* Delete all screens for selecting file path. Upto forward option or recv option. */
    if(g_vcard_cntx.FileLoc == MMI_VCARD_SAVE_TO_FILE_FROM_PHB)
    {
        
        DeleteUptoScrID(SCR_ID_VCARD_FORWARD_OPTION);
    }
    else /*MMI_VCARD_SAVE_TO_FILE_FROM_RECV*/
        DeleteUptoScrID(SCR_ID_VCARD_RECV_OPT);
#endif //#if 0    
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_fmgr_pre_enter_filename
 * DESCRIPTION
 *  create filename buffer for input,
 *  Also bring name field as default file name, if user forward a vcard from phonebook.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_forward_fmgr_pre_enter_filename(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_PHB_VCARD_STRUCT myCard;
    S16 pError;
    U16 length;
    S8* temp_filename;
    S8* temp_ptr;
    U16 temp_filename_length = (MAX_PB_NAME_LENGTH + 1) * ENCODING_LENGTH;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_PHB, "VCARD func:%s,FileLoc = %d", __FUNCTION__, g_vcard_cntx.FileLoc);

    if (g_vcard_cntx.FileNamePtr == NULL)   /* Should free after save or free in deinit func. */
    {
        g_vcard_cntx.FileNamePtr = OslMalloc(VOBJ_MAX_FILENAME_LENGTH);
    }
    
    memset(g_vcard_cntx.FileNamePtr, 0, VOBJ_MAX_FILENAME_LENGTH);
     
    /* Create default file name. */
    if (g_vcard_cntx.FileLoc == MMI_VCARD_SAVE_TO_FILE_FROM_PHB) 
    {
        if(g_vcard_cntx.CardType != MMI_VCARD_SEND_VCARD_TATOL)
        {
            if (VOBJ_MAX_FILENAME_LENGTH > (MAX_PB_NAME_LENGTH + 1) * ENCODING_LENGTH)
            {
                length = (MAX_PB_NAME_LENGTH + 1) * ENCODING_LENGTH;
            }
            else
            {
                length = VOBJ_MAX_FILENAME_LENGTH;
            }

            temp_filename = OslMalloc(temp_filename_length);
            if (g_vcard_cntx.CardType == MMI_VCARD_SEND_MY_CARD)
            {
                ReadRecord(NVRAM_EF_PHB_VCARD_LID, 1, (void*)&myCard, MMI_PHB_VCARD_RECORD_SIZE, &pError);
                memcpy(temp_filename, myCard.name, temp_filename_length);
            }
            else
            {
                memcpy(
                    temp_filename,
                    PhoneBook[g_phb_name_index[g_phb_cntx.active_index]].alpha_id.name,
                    temp_filename_length);
            }
            temp_ptr = mmi_fmgr_skip_space(temp_filename);
            pfnUnicodeStrcat(g_vcard_cntx.FileNamePtr, temp_ptr);
            OslMfree(temp_filename);
        }
    }

    mmi_vcard_entry_enter_filename();
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_fmgr_enter_filename_done
 * DESCRIPTION
 *  enter file name done, save vcard to destination path and handle the error message.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_forward_fmgr_enter_filename_done(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 result;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_PHB, "VCARD func:%s", __FUNCTION__);
    ClearKeyHandler(KEY_LSK, KEY_EVENT_UP);
    ClearKeyHandler(KEY_ENTER, KEY_EVENT_UP);
    
    if (g_vcard_cntx.FileLoc == MMI_VCARD_SAVE_TO_FILE_FROM_PHB)
    {
        result = mmi_vcard_forward_fmgr_save_file();
    }
    else
    {
        result = mmi_vcard_recv_move_obj_to_file();

        if (result == VOBJ_ERR_NO_ERROR)    /* Send message to copy, handle result in callback */
        {
            return;
        }
    }

    mmi_vcard_forward_fmgr_save_file_done(result);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_fmgr_save_file
 * DESCRIPTION
 *  save vcard to the destination file path
 * PARAMETERS
 *  void
 * RETURNS
 *  S32   error cause
 *****************************************************************************/
static S32 mmi_vcard_forward_fmgr_save_file(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    S8 filename[VOBJ_MAX_FILE_EXT_LEN];
    FS_HANDLE fh;
    U16 path_len;
    U16 name_len;
    S32 result;
    S8* temp_filename;
    S8* temp_ptr;
    U16 temp_filename_length = VOBJ_MAX_FILENAME_LENGTH;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_PHB, "VCARD func:%s", __FUNCTION__);
    
    MMI_ASSERT(g_vcard_cntx.FilePathPtr != NULL);
    path_len = pfnUnicodeStrlen(g_vcard_cntx.FilePathPtr);

    MMI_ASSERT(g_vcard_cntx.FileNamePtr != NULL);
    name_len = pfnUnicodeStrlen(g_vcard_cntx.FileNamePtr);

    temp_filename = OslMalloc(temp_filename_length);
    memset(temp_filename, 0, sizeof(temp_filename));
    temp_ptr = mmi_fmgr_skip_space(g_vcard_cntx.FileNamePtr);
    pfnUnicodeStrncpy(temp_filename, temp_ptr, (temp_filename_length - 1));

    /* if temp_ptr != original file name ptr, it means there is space in the front of filename
       so popup invalid filename */
    if (temp_ptr != g_vcard_cntx.FileNamePtr)
    {
        OslMfree(temp_filename);
        return VOBJ_ERR_INVALID_FILENAME;
    }

    /* Check if name empty */
    if (pfnUnicodeStrlen(temp_filename) == 0)
    {
        OslMfree(temp_filename);
        return VOBJ_ERR_EMPTY_NAME;
    }

    /* Check if file path is too long */
    if ((path_len + name_len + VOBJ_MAX_FILE_EXT_LEN / ENCODING_LENGTH) > VOBJ_MAX_PATH_LENGTH / ENCODING_LENGTH)
    {
        OslMfree(temp_filename);
        return VOBJ_ERR_SELECT_PATH_TOO_LONG;
    }

    /* Check if file name contain '/' or '\' */
    if (!mmi_fmgr_is_filename_valid(temp_filename))
    {
        OslMfree(temp_filename);
        return VOBJ_ERR_INVALID_FILENAME;
    }

    /* Concatenate file path */
    pfnUnicodeStrcat(g_vcard_cntx.FilePathPtr, g_vcard_cntx.FileNamePtr);
    if(g_vcard_cntx.CardType == MMI_VCARD_SEND_VCARD_TATOL)
    {
        AnsiiToUnicodeString(filename, VOBJ_VCARD_OUTLOOK_FILE_EXT);
    }
    else
    {
        AnsiiToUnicodeString(filename,VOBJ_VCARD_FILE_EXT);
    }
    pfnUnicodeStrcat(g_vcard_cntx.FilePathPtr, filename);

    /*Test if file already exists.*/
    fh = MMI_FS_Open((U8*)g_vcard_cntx.FilePathPtr, FS_READ_ONLY);
    if( fh >= 0) 
    {
        MMI_FS_Close(fh);
        memset((g_vcard_cntx.FilePathPtr + path_len * ENCODING_LENGTH), 0, 2);  /* Clear file name concatenate after path */
        return VOBJ_ERR_DUPLICATE_NAME;
    }

    if (g_vcard_cntx.CardType == MMI_VCARD_SEND_MY_CARD)
    {
        result = mmi_vcard_writer_v21_mycard_to_file(g_vcard_cntx.FilePathPtr);
    }
    else if (g_vcard_cntx.CardType == MMI_VCARD_SEND_VCARD_TATOL)
    {
        result = mmi_vcard_writer_all_card_to_csvfile(g_vcard_cntx.FilePathPtr);    
        DeleteScreenIfPresent(SCR_IN_PROGRESS_PHB);
    }
    else
    {
        result = mmi_vcard_writer_v21_select_card_to_file(g_vcard_cntx.FilePathPtr);
    }

    return result;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_forward_fmgr_save_file_done
 * DESCRIPTION
 *  save vcard to the destination file path
 * PARAMETERS
 *  result      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_forward_fmgr_save_file_done(S32 result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_PHB, "VCARD func:%s result = %d, fileloc = %d, cardtype= %d",
    __FUNCTION__, result, g_vcard_cntx.FileLoc, g_vcard_cntx.CardType);
    switch (result)
    {
        case VOBJ_ERR_NO_ERROR:
            DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);

            if (g_vcard_cntx.FileLoc == MMI_VCARD_SAVE_TO_FILE_FROM_PHB)
            {
                if (g_vcard_cntx.CardType == MMI_VCARD_SEND_MY_CARD)
                {
                    DeleteUptoScrID(SCR_ID_PHB_VCARD_MYCARD);
                }
                else if (g_vcard_cntx.CardType == MMI_VCARD_SEND_SELECT_CARD)
                {
                    DeleteUptoScrID(SCR_PBOOK_ENTRY_OPTIONS);
                }
                else if (g_vcard_cntx.CardType == MMI_VCARD_SEND_VCARD_TATOL)
                {
                    DeleteUptoScrID(SCR_ID_VCARD_EXPORT_OPTION);
                }
            }
            else if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_OTHER_APPS)
            {
                mmi_vcard_recv_set_result(MMI_VOBJ_SAVE_TO_FILEMGR);
                DeleteNScrId(SCR_ID_VCARD_RECV_OPT);
                mmi_vcard_recv_from_other_apps_callback();
            }
            else
            {
                mmi_vobj_clear_one_file_buffer();
                DeleteNScrId(SCR_ID_VCARD_RECV_OPT);
                DeleteScreenIfPresent(SCR_ID_VOBJ_RECV_OBJECT);
            }
            
            mmi_vcard_free_path_buffer();
            break;
        case VOBJ_ERR_EMPTY_NAME:
            DisplayPopup((PU8)GetString(STR_ID_VOBJ_EMPTY_FILENAME), IMG_GLOBAL_ERROR,  TRUE, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
            DeleteUptoScrID(SCR_ID_VCARD_ENTER_FILENAME);
            break;
        case VOBJ_ERR_DUPLICATE_NAME:
            DisplayPopup((PU8)GetString(STR_ID_VOBJ_DUPLICATE_FILENAME), IMG_GLOBAL_ERROR,  TRUE, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
            DeleteUptoScrID(SCR_ID_VCARD_ENTER_FILENAME);
            break;
        case VOBJ_ERR_INVALID_FILENAME:			
            DisplayPopup((PU8)GetString(STR_ID_VOBJ_INVALID_FILENAME), IMG_GLOBAL_ERROR,  TRUE, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
            DeleteUptoScrID(SCR_ID_VCARD_ENTER_FILENAME);
            break;
        case VOBJ_ERR_SELECT_PATH_TOO_LONG:
           DisplayPopup(
                (PU8) GetString(STR_ID_VOBJ_FILE_PATH_TOO_LONG),
                IMG_GLOBAL_ERROR,
                TRUE,
                UI_POPUP_NOTIFYDURATION_TIME,
                ERROR_TONE);
            if (g_vcard_cntx.FileLoc == MMI_VCARD_SAVE_TO_FILE_FROM_PHB)
            {
                if (g_vcard_cntx.CardType == MMI_VCARD_SEND_VCARD_TATOL)
                {
                    DeleteUptoScrID(SCR_ID_VCARD_EXPORT_OPTION);
                }            
                else
                {
                    DeleteUptoScrID(SCR_ID_VCARD_FORWARD_OPTION);
                }
            }
            else
            {
                DeleteUptoScrID(SCR_ID_VCARD_RECV_OPT);
            }
            break;
        case VOBJ_ERR_UNKNOWN:
            if (g_vcard_cntx.FilePathPtr)
            {
                MMI_FS_Delete((U8*) g_vcard_cntx.FilePathPtr);
            }
            DisplayPopup(
                (PU8) GetString(STR_GLOBAL_UNFINISHED),
                IMG_GLOBAL_UNFINISHED,
                TRUE,
                UI_POPUP_NOTIFYDURATION_TIME,
                ERROR_TONE);
            if (g_vcard_cntx.FileLoc == MMI_VCARD_SAVE_TO_FILE_FROM_PHB)
            {
                if (g_vcard_cntx.CardType == MMI_VCARD_SEND_VCARD_TATOL)
                {
                    DeleteUptoScrID(SCR_ID_VCARD_EXPORT_OPTION);
                }            
                else
                {
                    DeleteUptoScrID(SCR_ID_VCARD_FORWARD_OPTION);
                }
            }
            /* csj */
            else if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_OTHER_APPS)
            {
                mmi_vcard_recv_set_result(MMI_VOBJ_SAVE_TO_FILEMGR_FAIL);
                DeleteUptoScrID(SCR_ID_VCARD_RECV_OPT);
            }
            else
            {
                DeleteUptoScrID(SCR_ID_VCARD_RECV_OPT);
            }
            break;
        default:
            if (g_vcard_cntx.FilePathPtr)
            {
                MMI_FS_Delete((U8*) g_vcard_cntx.FilePathPtr);
            }
            DisplayPopup(
                (PU8) GetString(GetFileSystemErrorString(result)),
                IMG_GLOBAL_ERROR,
                1,
                UI_POPUP_NOTIFYDURATION_TIME,
                (U8) ERROR_TONE);
            if (g_vcard_cntx.FileLoc == MMI_VCARD_SAVE_TO_FILE_FROM_PHB)
            {
                if (g_vcard_cntx.CardType == MMI_VCARD_SEND_MY_CARD)
                {
                    DeleteUptoScrID(SCR_ID_PHB_VCARD_MYCARD);
                }
                else if (g_vcard_cntx.CardType == MMI_VCARD_SEND_SELECT_CARD)
                {
                    DeleteUptoScrID(SCR_PBOOK_ENTRY_OPTIONS);
                }
                else if (g_vcard_cntx.CardType == MMI_VCARD_SEND_VCARD_TATOL)
                {
                    DeleteUptoScrID(SCR_ID_VCARD_EXPORT_OPTION);
                }
            }
            else if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_OTHER_APPS)
            {
                mmi_vcard_recv_set_result(MMI_VOBJ_SAVE_TO_FILEMGR_FAIL);
                DeleteUptoScrID(SCR_ID_VCARD_RECV_OPT);
            }
            else
            {
                DeleteUptoScrID(SCR_ID_VCARD_RECV_OPT);
            }            
            break;
	}
}

#endif /* defined(__MMI_FILE_MANAGER__) */


/*--------------------------------------------------END vCard Forward operation ---------------------------------------------------*/



/*--------------------------------------------------BEGIN vCard Received operation ---------------------------------------------------*/
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_highlight_recv_save_to_sim
 * DESCRIPTION
 *  highlight function for options "save to SIM" of receiving a vcard.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_highlight_recv_save_to_sim(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	mmi_trace(1, "mmi_vcard_highlight_recv_save_to_sim");

	if(g_vcard_cntx.ImportLoc == MMI_VCARD_IMPORT_FROM_VCF)   
	{
		SetLeftSoftkeyFunction(mmi_vcard_recv_view_detail_pre_entry_check, KEY_EVENT_UP);
		SetKeyHandler(mmi_vcard_recv_view_detail_pre_entry_check, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}
	else
	{
		SetLeftSoftkeyFunction(mmi_vcard_outlook_recv_parse_file, KEY_EVENT_UP);
		SetKeyHandler(mmi_vcard_outlook_recv_parse_file, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);  
	}

	g_vcard_cntx.StorageLoc = MMI_SIM;
#ifdef __MMI_MULTI_SIM__
	g_vcard_cntx.StorageLoc += GetReqSrcModIndex();
#endif
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_highlight_recv_save_to_phone
 * DESCRIPTION
 *  highlight function for options "save to Phone" of receiving a vcard.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_highlight_recv_save_to_phone(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
        mmi_trace(1, "vcard mmi_vcard_highlight_recv_save_to_phone");

     if(g_vcard_cntx.ImportLoc == MMI_VCARD_IMPORT_FROM_VCF)   
    {   
        SetLeftSoftkeyFunction(mmi_vcard_recv_view_detail_pre_entry_check, KEY_EVENT_UP);
        SetKeyHandler(mmi_vcard_recv_view_detail_pre_entry_check, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    }
    else
    {
        SetLeftSoftkeyFunction(mmi_vcard_outlook_recv_parse_file, KEY_EVENT_UP);
        SetKeyHandler(mmi_vcard_outlook_recv_parse_file, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);  
    }
    
    g_vcard_cntx.StorageLoc = MMI_NVRAM;
}


#if defined(__MMI_FILE_MANAGER__)
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_recv_save_to_fmgr
 * DESCRIPTION
 *  highlight function for save vcard to file manager.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_recv_save_to_fmgr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_vcard_cntx.FileLoc = MMI_VCARD_SAVE_TO_FILE_FROM_RECV;
    
    mmi_vcard_forward_fmgr();
    DeleteNScrId(SCR_ID_VCARD_RECV_OPT);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_highlight_recv_save_to_fmgr
 * DESCRIPTION
 *  highlight function for options "save to file" of receiving a vcard.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_highlight_recv_save_to_fmgr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(mmi_vcard_recv_save_to_fmgr, KEY_EVENT_UP);
    SetKeyHandler(mmi_vcard_recv_save_to_fmgr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    g_vcard_cntx.FileLoc = MMI_VCARD_SAVE_TO_FILE_FROM_RECV;
}
#endif /* __MMI_FILE_MANAGER__ */


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_entry_recv_option
 * DESCRIPTION
 *  Entry screen of save a vcard option. It can save to SIM, NVRAM or File manager.
 *  If forward from file manager, there's no "Save to File" option.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_entry_recv_option(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8 *guiBuffer;
	U16 nStrItemList[5];
	U16 nNumofItem;
	U16 str_id = STR_GLOBAL_OPTIONS;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	mmi_vcard_init();   /* Regiser highlight handler again. */

	EntryNewScreen(SCR_ID_VCARD_RECV_OPT, NULL, mmi_vcard_entry_recv_option, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_VCARD_RECV_OPT);
#if defined(__MMI_FILE_MANAGER__)	    
	mmi_frm_unhide_menu_item(MENU_ID_VCARD_RECV_OPTION_FMGR);
	if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_FILE_MANAGER)
	{
		mmi_frm_hide_menu_item(MENU_ID_VCARD_RECV_OPTION_FMGR);
		str_id = STR_CHOOSE_STORAGE_CAPTION;
	}
#endif /* defined(__MMI_FILE_MANAGER__) */
	mmi_frm_unhide_menu_item(MENU_ID_VCARD_RECV_OPTION_TO_PHONE);

#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	MTPNP_AD_SetMenuHideUnhide(MENU_ID_VCARD_RECV_OPTION_TO_SIM);
#endif	
	nNumofItem = GetNumOfChild_Ext(MENU_ID_VCARD_RECV_OPTION);

	GetSequenceStringIds_Ext(MENU_ID_VCARD_RECV_OPTION, nStrItemList);
	SetParentHandler(MENU_ID_VCARD_RECV_OPTION);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory15Screen(STR_GLOBAL_OPTIONS, IMG_SCR_PBOOK_CAPTION,
					  STR_GLOBAL_OK, IMG_GLOBAL_OK,
					  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
					  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
					  LIST_MENU, 0, guiBuffer);

	SetRightSoftkeyFunction(mmi_vcard_entry_recv_option_back,KEY_EVENT_UP);
	SetKeyHandler(mmi_vcard_entry_recv_option_back, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


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
void mmi_vcard_recv_from_idle_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_vcard_cntx.ParseLoc = MMI_VCARD_PARSE_FROM_VOBJ_BUFFER;
    mmi_vcard_entry_recv_option();
}


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
void mmi_vcard_recv_from_other_apps_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_vobj_cntx.info == MMI_VOBJ_SAVE_TO_FILEMGR)
    {
        g_vobj_cntx.recv_callback(g_vobj_cntx.info, g_vcard_cntx.FilePathPtr);
    }
    else
    {
        g_vobj_cntx.recv_callback(g_vobj_cntx.info, NULL);
    }
    
    mmi_vcard_free_path_buffer();
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_entry_recv_option_back
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_entry_recv_option_back(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/    
    if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_OTHER_APPS)
    {
        mmi_vcard_recv_from_other_apps_callback();
        mmi_vcard_free_path_buffer();
        GoBackHistory();
    }
    else if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_FILE_MANAGER)
    {
        if(g_vcard_cntx.ImportLoc != MMI_VCARD_IMPORT_FROM_NONE)
        {
            GoBackToHistory(SCR_ID_VCARD_IMPORT_FROM_OPTION);
        }
    }
    else
    {
        if (g_vcard_cntx.FileLoc == MMI_VCARD_SAVE_TO_FILE_FROM_PHB)
        {
            if (g_vcard_cntx.CardType == MMI_VCARD_SEND_MY_CARD)
            {
                GoBackToHistory(SCR_ID_PHB_VCARD_MYCARD);
            }
            else if (g_vcard_cntx.CardType == MMI_VCARD_SEND_SELECT_CARD)
            {
                GoBackToHistory(SCR_PBOOK_ENTRY_OPTIONS);
            }
            else if (g_vcard_cntx.CardType == MMI_VCARD_SEND_VCARD_TATOL)
            {
                GoBackToHistory(SCR_ID_VCARD_EXPORT_OPTION);
            }
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_delete_screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_delete_screen(U16 scr_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (scr_id == GetActiveScreenId())
    {
        GoBackHistory();
    }
    else
    {
        DeleteScreenIfPresent(scr_id);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_recv_from_other_apps
 * DESCRIPTION
 *  A pre entry function of receiving vcard from imps
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_recv_from_other_apps(S8 *path, mmi_vobj_recv_from_other_app_callback call_back)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (path == NULL)
    {
        return;
    }

    if (g_vobj_cntx.AppFilePathPtr == NULL)
    {
        g_vobj_cntx.AppFilePathPtr = OslMalloc(VOBJ_MAX_PATH_LENGTH);
    }

    pfnUnicodeStrcat(g_vobj_cntx.AppFilePathPtr, path);
    
    g_vobj_cntx.recv_callback = call_back;

    g_vobj_cntx.info = MMI_VOBJ_SAVE_DISCARD;
    
    g_vcard_cntx.ParseLoc = MMI_VCARD_PARSE_FROM_OTHER_APPS;
    
    mmi_vcard_entry_recv_option();
}

#if defined(__MMI_FILE_MANAGER__)
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_recv_from_file_mgr
 * DESCRIPTION
 *  A pre-entry function of receiving vcard functuion. This vcard is forwarded from file manager.
 * PARAMETERS
 *  path        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_recv_from_file_mgr(S8 *path)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (path == NULL)
    {
        return;
    }

    if (g_vcard_cntx.FilePathPtr == NULL)
    {
        g_vcard_cntx.FilePathPtr = OslMalloc(VOBJ_MAX_PATH_LENGTH);
    }

    pfnUnicodeStrcpy(g_vcard_cntx.FilePathPtr, path);

    g_vcard_cntx.ParseLoc = MMI_VCARD_PARSE_FROM_FILE_MANAGER;

    mmi_vcard_entry_recv_option();
}

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
void mmi_vcard_fmgr_sms_forward(S8 *path)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if !defined(__MMI_MESSAGES_NO_SEND_BY_SMS_OPTION__)
    mmi_ems_vobject_send_by_ems(VOBJ_SRC_PORT, VOBJ_VCARD_PORT, path, NULL);
#endif 
}
#endif /* defined(__MMI_FILE_MANAGER__) */ 


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_entry_recv_view_detail
 * DESCRIPTION
 *  This is the detail message of a vcard after parsing. Show details to users before saving.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_entry_recv_view_detail(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *pbEntry[MAX_PB_FIELDS];
    U16 DetailPbNameImageList[MAX_PB_FIELDS];
    U8 *guiBuffer;
    U8 j = 0;
#if defined(__PHB_0x81_SUPPORT__)  
    U8 ucs2_0x81_name_length;
    S8 ucs2_0x81_name[(MAX_PB_NAME_LENGTH + 1) * ENCODING_LENGTH];
#endif /* defined(__PHB_0x81_SUPPORT__) */
    U8 pos;
    U8 ex_count = 0;
    U8 ex_len = 1; /* need second bytes */
    U8 buffer_len = pfnUnicodeStrlen(pbName) * ENCODING_LENGTH;
    U8 max_name_ln;
        
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* ExecuteCurrExitHandler(); */
    EntryNewScreen(SCR_ID_VCARD_RECV_VIEW_DETAIL, NULL, mmi_vcard_recv_view_detail_pre_entry_check, NULL);
    guiBuffer = GetCurrGuiBuffer(SCR_ID_VCARD_RECV_VIEW_DETAIL);

    mmi_phb_entry_op_remove_invalid_name_and_number();

    /* Truncated to fit buffer size */
#if defined(__PHB_0x81_SUPPORT__)       /* Support maximum length of 0x81 UCS2 */
    memcpy(ucs2_0x81_name, pbName, ((MAX_PB_NAME_LENGTH + 1) * ENCODING_LENGTH));
    if ((ucs2_0x81_name_length = mmi_phb_convert_to_0x81((S8*) ucs2_0x81_name, TRUE)) > 0)
    {
        if (g_vcard_cntx.StorageLoc == MMI_NVRAM || !g_phb_cntx.phb_ready)
        {
            pos = (MAX_PB_NAME_LENGTH - 3) * 2;
        }
        else    /* Need phonebook ready to get the Maximum SIM length */
        {
            pos = (g_phb_cntx.sim_name_len[g_vcard_cntx.StorageLoc - MMI_SIM] - 3) * 2;
        }

        memset(pbName + pos, 0, 2);
    }
    else
#endif /* defined(__PHB_0x81_SUPPORT__) */ 
    if (GetUCS2Flag(pbName))
    {
        if (g_vcard_cntx.StorageLoc == MMI_NVRAM || !g_phb_cntx.phb_ready)
        {
            pos = ((MAX_PB_NAME_LENGTH) / 2) * 2;
        }
        else    /* Need phonebook ready to get the Maximum SIM length */
        {
            pos = ((g_phb_cntx.sim_name_len[g_vcard_cntx.StorageLoc - MMI_SIM] - 1) / 2) * 2;
        }

        memset(pbName + pos, 0, 2);
    }
    else if (g_phb_cntx.phb_ready)
    {

        if (g_vcard_cntx.StorageLoc >= MMI_SIM && g_vcard_cntx.StorageLoc <= MMI_SIM4 && g_phb_cntx.sim_name_len[g_vcard_cntx.StorageLoc - MMI_SIM] < MAX_PB_NAME_LENGTH)
        {
            max_name_ln = g_phb_cntx.sim_name_len[g_vcard_cntx.StorageLoc - MMI_SIM];
        }
        else
        {
            max_name_ln = MAX_PB_NAME_LENGTH;
        }
        while (ex_count < max_name_ln && ex_len < buffer_len)
        {
            UI_character_type ch;
            ch = ((UI_character_type) pbName[ex_len] << 8) | ((UI_character_type) pbName[ex_len - 1]);
            if (UI_TEST_GSM_EXTENDED(ch))
            {
                ex_count++;
            }
            ex_count++;
            ex_len += 2;
        }
        ex_len--;
        if (max_name_ln < ex_count && ex_len > 2)
        {
            ex_len = ex_len - 2;
        }
        memset(pbName + ex_len, 0, 2);
    }

    /* begin to associate to inline editor */
    pbEntry[j] = pbName;
    DetailPbNameImageList[j] = IMG_NAME;
    TitleStrings[j++] = STR_ID_PHB_NAME;

    pbEntry[j] = pbNumber;
    DetailPbNameImageList[j] = IMG_MOBILE_NUMBER;
    TitleStrings[j++] = STR_ID_PHB_MOBILE_NUMBER;

    if (g_vcard_cntx.StorageLoc == MMI_NVRAM)
    {
    #if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)    	
        if(pfnUnicodeStrlen(pbHomeNumber))
        {
            pbEntry[j] = pbHomeNumber;
            TitleStrings[j] = STR_HOME_NUMBER;
            DetailPbNameImageList[j++] = IMG_HOME_NUMBER;
        }
    	if(pfnUnicodeStrlen(pbCompanyName))
    	{
            pbEntry[j] = pbCompanyName;
            TitleStrings[j] = STR_COMPNAY_NAME;
            DetailPbNameImageList[j++] = IMG_COMPANY_NAME;
        }
     	if(pfnUnicodeStrlen(pbEmailAddress))
    	{
            pbEntry[j] = pbEmailAddress;
            TitleStrings[j] = STR_EMAIL_ADRESS;
            DetailPbNameImageList[j++] = IMG_EMAIL_ADDRESS;
        }
    #ifdef __MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__
     	if(pfnUnicodeStrlen(pbEmailAddress2))
    	{
            pbEntry[j] = pbEmailAddress2;
            TitleStrings[j] = STR_EMAIL_ADRESS;
            DetailPbNameImageList[j++] = IMG_EMAIL_ADDRESS;
        }
    #endif /* __MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__ */
    	if(pfnUnicodeStrlen(pbOfficeNumber))
    	{
            pbEntry[j] = pbOfficeNumber;
            TitleStrings[j] = STR_OFFICE_NUMBER;
            DetailPbNameImageList[j++] = IMG_OFFICE_NUMBER;
        }
    	if(pfnUnicodeStrlen(pbFaxNumber))
    	{
            pbEntry[j] = pbFaxNumber;
            TitleStrings[j] = STR_FAX_NUMBER;
            DetailPbNameImageList[j++] = IMG_FAX_NUMBER;
        }
    #endif /* !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)*/

    #if defined(__MMI_PHB_BIRTHDAY_FIELD__)
        if (pfnUnicodeStrcmp((const S8*)pbYear, (const S8*)MMI_PHB_BDAY_YEAR_STR) != 0 ||
            pfnUnicodeStrcmp((const S8*)pbMon, (const S8*)MMI_PHB_BDAY_MON_STR) != 0 ||
            pfnUnicodeStrcmp((const S8*)pbDay, (const S8*)MMI_PHB_BDAY_DAY_STR) != 0)
        {
            mmi_phb_bday_editor_callback((U8*) pbBday, (U8*) pbDay, (U8*) pbMon, (U8*) pbYear);
            pbEntry[j] = pbBday;
            TitleStrings[j] = STR_ID_PHB_BIRTHDAY;
            DetailPbNameImageList[j++] = IMG_ID_PHB_BIRTHDAY;
        }
    #endif /* defined(__MMI_PHB_BIRTHDAY_FIELD__) */ 

    #if defined(__MMI_PHB_INFO_FIELD__)
    	if(pfnUnicodeStrlen(pbTitle))
    	{
            pbEntry[j] = pbTitle;
            TitleStrings[j] = STR_ID_PHB_TITLE;
            DetailPbNameImageList[j++] = IMG_ID_PHB_TITLE;
        }
     	if(pfnUnicodeStrlen(pbUrl))
    	{       
            pbEntry[j] = pbUrl;
            TitleStrings[j] = STR_ID_PHB_URL;
            DetailPbNameImageList[j++] = IMG_ID_PHB_URL;
        }
     	if(pfnUnicodeStrlen(pbAdrPOBox))
    	{  
            pbEntry[j] = pbAdrPOBox;
            TitleStrings[j] = STR_ID_PHB_ADR_POBOX;
            DetailPbNameImageList[j++] = IMG_ID_PHB_ADR_POBOX;
        }
     	if(pfnUnicodeStrlen(pbAdrExtension))
    	{  
            pbEntry[j] = pbAdrExtension;
            TitleStrings[j] = STR_ID_PHB_ADR_EXTENSION;
            DetailPbNameImageList[j++] = IMG_ID_PHB_ADR_EXTENSION;
        }
     	if(pfnUnicodeStrlen(pbAdrStreet))
    	{  
            pbEntry[j] = pbAdrStreet;
            TitleStrings[j] = STR_ID_PHB_ADR_STREET;
            DetailPbNameImageList[j++] = IMG_ID_PHB_ADR_STREET;
        }
     	if(pfnUnicodeStrlen(pbAdrCity))
    	{  
            pbEntry[j] = pbAdrCity;
            TitleStrings[j] = STR_ID_PHB_ADR_CITY;
            DetailPbNameImageList[j++] = IMG_ID_PHB_ADR_CITY;
        }
     	if(pfnUnicodeStrlen(pbAdrState))
    	{  
            pbEntry[j] = pbAdrState;
            TitleStrings[j] = STR_ID_PHB_ADR_STATE;
            DetailPbNameImageList[j++] = IMG_ID_PHB_ADR_STATE;
        }
     	if(pfnUnicodeStrlen(pbAdrPostalCode))
    	{  
            pbEntry[j] = pbAdrPostalCode;
            TitleStrings[j] = STR_ID_PHB_ADR_POSTALCODE;
            DetailPbNameImageList[j++] = IMG_ID_PHB_ADR_POSTALCODE;
        }
        if(pfnUnicodeStrlen(pbAdrCountry))
    	{  
            pbEntry[j] = pbAdrCountry;
            TitleStrings[j] = STR_ID_PHB_ADR_COUNTRY;
            DetailPbNameImageList[j++] = IMG_ID_PHB_ADR_COUNTRY;
        }
     	if(pfnUnicodeStrlen(pbNote))
    	{  
            pbEntry[j] = pbNote;
            TitleStrings[j] = STR_ID_PHB_NOTE;
            DetailPbNameImageList[j++] = IMG_ID_PHB_NOTE;
        }
    #endif /* defined(__MMI_PHB_INFO_FIELD__) */ 
    }

    ShowCategory84Screen(
        STR_ID_VOBJ_RECV_VCARD,
        IMG_SCR_PBOOK_CAPTION,
        STR_GLOBAL_SAVE,
        0,
        STR_GLOBAL_BACK,
        0,
        j,
        (U8 **) pbEntry,
        DetailPbNameImageList,
        0,
        0,
        guiBuffer);

    ChangeLeftSoftkey(STR_GLOBAL_SAVE, 0);
    SetLeftSoftkeyFunction(mmi_vcard_recv_save_obj_to_phb, KEY_EVENT_UP);
#ifdef __MMI_TOUCH_SCREEN__
    wgui_register_list_item_selected_callback_all(UI_dummy_function);
#endif 

    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
    SetRightSoftkeyFunction(mmi_vcard_entry_recv_view_detail_back, KEY_EVENT_UP);

    /* SetGenericExitHandler(SCR_ID_VCARD_RECV_VIEW_DETAIL, NULL, mmi_vcard_entry_recv_view_detail); */

    if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_VOBJ_BUFFER)      /* Clear vCard temp file before go back to Idle screen if enter from idle screen */
    {
        SetKeyHandler(mmi_vcard_recv_abort_vcard, KEY_END, KEY_EVENT_DOWN);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_entry_recv_view_detail_back
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_entry_recv_view_detail_back(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_OTHER_APPS)
    {
    	mmi_vcard_recv_from_other_apps_callback();
    }
    
    if(g_vcard_cntx.ImportLoc == MMI_VCARD_IMPORT_FROM_VCF)
    {
        GoBackToHistory(SCR_ID_VCARD_IMPORT_FROM_OPTION); 
    }
    else
    {
        mmi_vcard_delete_screen(SCR_ID_VCARD_RECV_VIEW_DETAIL);  
    }

}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_recv_view_detail_pre_entry
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        First variable, used as returns
 *  b(?)        [IN]            Second variable
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_recv_view_detail_pre_entry(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (mmi_vcard_recv_parse_file())
    {
        mmi_vcard_entry_recv_view_detail();
    }
}


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
void mmi_vcard_recv_view_detail_pre_entry_check(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 str_id = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if (!g_phb_cntx.phb_ready || g_phb_cntx.processing) /* Phonebook not ready */
    {
        str_id = STR_PROCESSING_PHONEBOOK;
    }
#ifdef __MMI_PHB_USIM_FIELD__
    else if (g_phb_cntx.is_usim && g_phb_cntx.usim_ready_stage != MMI_PHB_USIM_READY)
    {
        str_id = STR_ID_PHB_PROCESSING_USIM;
    }
#endif /* __MMI_PHB_USIM_FIELD__ */
#ifdef __SYNCML_SUPPORT__
    else if (mmi_syncml_is_phb_sync_now())
    {
        str_id = STR_ID_SYNC_PLEASE_WAIT;
    }
#endif /* __SYNCML_SUPPORT__ */
    if (str_id != 0)
    {
        mmi_phb_entry_not_ready();
        if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_OTHER_APPS)
        {
            if (g_vcard_cntx.StorageLoc >= MMI_SIM && g_vcard_cntx.StorageLoc <= MMI_SIM4)
            {
                mmi_vcard_recv_set_result(MMI_VOBJ_SAVE_TO_SIM_FAIL + g_vcard_cntx.StorageLoc - MMI_SIM);
            }
            else
            {
                mmi_vcard_recv_set_result(MMI_VOBJ_SAVE_TO_PHONE_FAIL);           
            }
            mmi_vcard_recv_from_other_apps_callback();
        }
    }
    else
    {
        mmi_phb_clear_old_history();
        mmi_vcard_recv_view_detail_pre_entry();
    }
    DeleteScreenIfPresent(SCR_ID_VCARD_RECV_OPT);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_recv_abort_vcard
 * DESCRIPTION
 *  abort receiving a vcard, clear buffer or screen before aborting.
 * PARAMETERS
 *  void
 *  a(?)        [IN/OUT]        First variable, used as returns
 *  b(?)        [IN]            Second variable
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_recv_abort_vcard(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_FILE_MANAGER)
    {
        DeleteNScrId(SCR_ID_VCARD_RECV_VIEW_DETAIL);
        GoBackHistory();
    }
    else if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_VOBJ_BUFFER)
    {

        mmi_vobj_clear_one_file_buffer();
        DisplayIdleScreen();
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_recv_parse_file
 * DESCRIPTION
 *  a pre- parsing vcard function. Create path and do some error handling before
 *  send file to parser.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static BOOL mmi_vcard_recv_parse_file(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 error_code = MMI_VCARD_PARSE_NO_ERROR;
    U16 str_id;
    S8 path[VOBJ_MAX_PATH_LENGTH];
    S32 check_result = mmi_vobj_fs_check_folder();
        
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_FILE_MANAGER)
    {
        error_code = mmi_vcard_reader_from_file_path(g_vcard_cntx.FilePathPtr);
        if (error_code == MMI_VCARD_PARSE_NO_ERROR)
        {
            return TRUE;
        }
    }
    /* csj */
    else if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_OTHER_APPS)
    {
        error_code = mmi_vcard_reader_from_file_path(g_vobj_cntx.AppFilePathPtr);
        if (error_code == MMI_VCARD_PARSE_NO_ERROR)
        {
            return TRUE;
        }
    }
    else
    {
        /* Create file path and folder. */
        if (check_result == FS_NO_ERROR)
        {
            if (mmi_vobj_get_file_buffer_size() > 0)
            {
                //sprintf(ascii_path, "%c:\\%s\\%d.vcf", VOBJ_DRV,VOBJ_FILE_PATH,g_vobj_cntx.buffer_begin);
                //AnsiiToUnicodeString(path, ascii_path);
                mmi_vobj_get_full_path_name(g_vobj_cntx.buffer_begin, path, MMI_VOBJ_VCARD);

                error_code = mmi_vcard_reader_from_file_path(path);
                if (error_code == MMI_VCARD_PARSE_NO_ERROR)
                {
                    return TRUE;
                }
            }
        }
	DisplayPopup((PU8)GetString(STR_GLOBAL_NOT_DONE), IMG_GLOBAL_ERROR, TRUE, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
	DeleteUptoScrID(SCR_ID_VCARD_RECV_OPT);
    }

    do
    {
        if (error_code == MMI_VCARD_PARSE_FILE_SIZE_TOO_LARGE)
        {
            str_id = STR_ID_VCARD_FILE_TO_LARGE;
            break;
        }
        if (error_code < 0)
        {
            str_id = STR_GLOBAL_NOT_DONE;
            break;

        }
        str_id = STR_GLOBAL_UNSUPPORTED_FORMAT;
        
    }while(0);

    DisplayPopup((PU8)GetString(str_id), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
    mmi_vcard_outlook_import_done_del_src();
    
    if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_OTHER_APPS)
    {
        if (g_vcard_cntx.StorageLoc >= MMI_SIM && g_vcard_cntx.StorageLoc <= MMI_SIM4)
        {
            mmi_vcard_recv_set_result(MMI_VOBJ_SAVE_TO_SIM_FAIL + g_vcard_cntx.StorageLoc - MMI_SIM);
        }
        else
        {
            mmi_vcard_recv_set_result(MMI_VOBJ_SAVE_TO_PHONE_FAIL);           
        }
        mmi_vcard_recv_from_other_apps_callback();
    }

    return FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_recv_save_obj_to_phb
 * DESCRIPTION
 *  save parsed vcard to phonebook after parsing.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_recv_save_obj_to_phb(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 str_id = 0;
    U8 str_len = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_PHB, "VCARD Func = %s, add to= %d", __FUNCTION__, g_vcard_cntx.StorageLoc );

    if(g_vcard_cntx.StorageLoc != MMI_NVRAM)
    {
        str_len = pfnUnicodeStrlen(pbName);
        while(str_len > 0)
        {
            if(pbName[str_len * 2 - 1] != 0)
                break;
            str_len --;
        }
        
        if(str_len == 0)
        {
            memset(pbName + MAX_SIM_NAME_LENGTH * 2, 0, 2);            
        }
        else
        {
            memset(pbName + MAX_SIM_NAME_CHINESE_LENGTH * 2, 0, 2);                  
        }

        memset(pbNumber + MAX_SIM_NUMBER_LENGTH * 2, 0, 2);
    }

    if (!g_phb_cntx.phb_ready || g_phb_cntx.processing) /* Phonebook not ready */
    {
        str_id = STR_PROCESSING_PHONEBOOK;
    }
#ifdef __MMI_PHB_USIM_FIELD__
    else if (g_phb_cntx.is_usim && g_phb_cntx.usim_ready_stage != MMI_PHB_USIM_READY)
    {
        str_id = STR_ID_PHB_PROCESSING_USIM;
    }
#endif /* __MMI_PHB_USIM_FIELD__ */
#ifdef __SYNCML_SUPPORT__
    else if (mmi_syncml_is_phb_sync_now())
    {
        str_id = STR_ID_SYNC_PLEASE_WAIT;
    }
#endif /* __SYNCML_SUPPORT__ */
#if defined(__MMI_PHB_NAME_AND_NUM_REQUIRE__)
    else if (!pfnUnicodeStrlen(pbName) && !mmi_phb_calculate_number_valid_length(pbNumber))
    {
        str_id = STR_ID_PHB_NAME_NUM_INVALID;
    }
    else if (!pfnUnicodeStrlen(pbName))
    {
        str_id = STR_ID_PHB_NAME_INVALID;
    }
    else if (!mmi_phb_calculate_number_valid_length(pbNumber))
    {
        str_id = STR_ID_PHB_NUM_INVALID;
    }
#else /* defined(__MMI_PHB_NAME_AND_NUM_REQUIRE__) */ /* Require Name "or" Number field */
    else if (!pfnUnicodeStrlen(pbName) && !mmi_phb_calculate_number_valid_length(pbNumber))
    {
        str_id = STR_ID_PHB_NAME_NUM_INVALID_ALL;
    }
#endif /* defined(__MMI_PHB_NAME_AND_NUM_REQUIRE__) */ 
    else if (g_vcard_cntx.StorageLoc == MMI_NVRAM && g_phb_cntx.phone_used >= g_phb_cntx.phone_total)   /* Phone storage full. */
    {
        str_id = STR_PHONE_FULL_MSG;
    }
    else if (g_vcard_cntx.StorageLoc >= MMI_SIM && g_vcard_cntx.StorageLoc < MMI_SIM4 && g_phb_cntx.sim_used[g_vcard_cntx.StorageLoc - MMI_SIM] >= g_phb_cntx.sim_total[g_vcard_cntx.StorageLoc - MMI_SIM]) /* SIM storage full. */
    {
    #ifdef __MMI_MULTI_SIM__
        str_id = STR_ID_PHB_SYSTEM_FULL;
    #else
        str_id = STR_SIM_FULL_MSG;
    #endif
    }
    else if ((g_vcard_cntx.StorageLoc == MMI_NVRAM) && (!mmi_phb_op_check_valid_number(pbNumber) || mmi_ucs2str(pbNumber, "?")/* Number format not correct */
    #if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
        || !mmi_phb_op_check_valid_number(pbHomeNumber)
        || !mmi_phb_op_check_valid_number(pbOfficeNumber)
        || !mmi_phb_op_check_valid_number(pbFaxNumber)
        || mmi_ucs2str(pbHomeNumber, "?")
        || mmi_ucs2str(pbOfficeNumber, "?")
        || mmi_ucs2str(pbFaxNumber, "?")
    #endif    
        ))
    {
        str_id = STR_ID_PHB_NUM_INVALID;
    }
    else if (g_vcard_cntx.StorageLoc != MMI_NVRAM && (!mmi_phb_op_check_valid_number(pbNumber)
        || mmi_ucs2str(pbNumber, "w") || mmi_ucs2str(pbNumber, "W") || mmi_ucs2str(pbNumber, "?"))) 
    {
        str_id = STR_ID_PHB_NUM_INVALID;
    }
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) || defined(__MMI_PHB_USIM_FIELD__)
    else if (g_vcard_cntx.StorageLoc == MMI_NVRAM)
    {
        if (pfnUnicodeStrlen(pbEmailAddress) && !applib_is_valid_email_address_unicode((U16*)pbEmailAddress))
        {
            memset(pbEmailAddress, 0, 2);
        }
    #if defined(__MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__)
        if (pfnUnicodeStrlen(pbEmailAddress2) && !applib_is_valid_email_address_unicode((U16*)pbEmailAddress2))
        {
            memset(pbEmailAddress2, 0, 2);
        }
    #endif /* __MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__ */
    }
#endif /* !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) || defined(__MMI_PHB_USIM_FIELD__) */

    if (str_id != 0)
    {
        if (str_id == STR_PROCESSING_PHONEBOOK 
        #ifdef __MMI_PHB_USIM_FIELD__
            || str_id == STR_ID_PHB_PROCESSING_USIM
        #endif /* __MMI_PHB_USIM_FIELD__ */
        #ifdef __SYNCML_SUPPORT__
            || str_id == STR_ID_SYNC_PLEASE_WAIT
        #endif /* __SYNCML_SUPPORT__ */
            )
        {
            mmi_phb_entry_not_ready();
        }
        else
        {
            DisplayPopup((PU8)GetString(str_id), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
        }
        if (g_vcard_cntx.ParseLoc >= MMI_VCARD_PARSE_FROM_OTHER_APPS)
        {
            if (g_vcard_cntx.StorageLoc >= MMI_SIM && g_vcard_cntx.StorageLoc <= MMI_SIM4)
            {
                mmi_vcard_recv_set_result(MMI_VOBJ_SAVE_TO_SIM_FAIL + g_vcard_cntx.StorageLoc - MMI_SIM);
            }
            else
            {
                mmi_vcard_recv_set_result(MMI_VOBJ_SAVE_TO_PHONE_FAIL);           
            }
            mmi_vcard_recv_from_other_apps_callback();
        }
        mmi_vcard_import_free_buffer();
        mmi_vcard_outlook_import_done_del_src();
        DeleteScreenIfPresent(SCR_ID_VCARD_RECV_VIEW_DETAIL);
    }
    else
    {
        if (g_vcard_cntx.StorageLoc >= MMI_SIM && g_vcard_cntx.StorageLoc <= MMI_SIM4)
        {
            g_phb_cntx.selected_storage = MMI_SIM + g_vcard_cntx.StorageLoc - MMI_SIM;
        }
        else
        {
            g_phb_cntx.selected_storage = MMI_NVRAM;
        }
        g_phb_cntx.image_location = MMI_PHB_IMAGE_NO_SELECT;    /* No Image. */
        mmi_phb_op_add_entry_req();
        
        DeleteScreenIfPresent(SCR_ID_VCARD_RECV_VIEW_DETAIL);

        /* be sure to set g_phb_enter_from value in every case*/
        if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_VOBJ_BUFFER)
        {
            g_phb_enter_from = MMI_PHB_ENTER_FROM_IDLE_SCREEN;
        }
        /* csj */
        else if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_OTHER_APPS)
        {
            g_phb_enter_from = MMI_PHB_ENTER_FROM_VCARD_RECV;   
        }
        else if(g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_FILE_MANAGER 
            && g_vcard_cntx.ImportLoc == MMI_VCARD_IMPORT_FROM_OUTLOOK)
        {
            g_phb_enter_from = MMI_PHB_ENTER_FROM_OUTLOOK_IMPORT;   
        }
        else if(g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_FILE_MANAGER 
            && g_vcard_cntx.ImportLoc == MMI_VCARD_IMPORT_FROM_VCF)
        {
            g_phb_enter_from = MMI_PHB_ENTER_FROM_VCF_IMPORT;   
        }            
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_recv_move_obj_to_file
 * DESCRIPTION
 *  move vcard to the path selected by user.
 * PARAMETERS
 *  void
 * RETURNS
 *  S32   error cause
 *****************************************************************************/
static S32 mmi_vcard_recv_move_obj_to_file(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 filename[VOBJ_MAX_FILE_EXT_LEN];
    FS_HANDLE fh;
    U16 path_len;
    U16 name_len;
    S32 result;
    S8 *src_path;
    S8* temp_filename;
    S8* temp_ptr;
    U16 temp_filename_length = VOBJ_MAX_FILENAME_LENGTH;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(g_vcard_cntx.FilePathPtr != NULL);
	path_len = pfnUnicodeStrlen(g_vcard_cntx.FilePathPtr);

	MMI_ASSERT(g_vcard_cntx.FileNamePtr != NULL);
	name_len = pfnUnicodeStrlen(g_vcard_cntx.FileNamePtr);

	/*Check if name empty*/
	if(pfnUnicodeStrlen(g_vcard_cntx.FileNamePtr) == 0)
    {
        return VOBJ_ERR_EMPTY_NAME;
    }

    /* Check if file path is too long */
    if ((path_len + name_len + VOBJ_MAX_FILE_EXT_LEN / ENCODING_LENGTH) > VOBJ_MAX_PATH_LENGTH / ENCODING_LENGTH)
    {
        return VOBJ_ERR_SELECT_PATH_TOO_LONG;
    }

    temp_filename = OslMalloc(temp_filename_length);
    temp_ptr = mmi_fmgr_skip_space(g_vcard_cntx.FileNamePtr);
    pfnUnicodeStrcat(temp_filename, temp_ptr);
    /* Check if file name contain '/' or '\' */
    if (!mmi_fmgr_is_filename_valid(temp_filename))
    {
        OslMfree(temp_filename);
        return VOBJ_ERR_INVALID_FILENAME;
    }

    /* Concatenate file path */
    pfnUnicodeStrcat(g_vcard_cntx.FilePathPtr, temp_filename);
    OslMfree(temp_filename);
    AnsiiToUnicodeString(filename, VOBJ_VCARD_FILE_EXT);
    pfnUnicodeStrcat(g_vcard_cntx.FilePathPtr, filename);

    /* Test if file already exists. */
    fh = MMI_FS_Open((U8*) g_vcard_cntx.FilePathPtr, FS_READ_ONLY);
    if (fh > 0)
    {
        MMI_FS_Close(fh);
        memset((g_vcard_cntx.FilePathPtr + path_len * ENCODING_LENGTH), 0, 2);  /* Clear file name concatenate after path */
        return VOBJ_ERR_DUPLICATE_NAME;
    }
    /* csj */
    /* Move the file to the destinatio. */
    if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_OTHER_APPS)
    {
        /* Use move instead of rename because user may save to card. */
        /* result = FS_Rename((U16*)src_path, (U16*)g_vcard_cntx.FilePathPtr); */
    #if defined(__MMI_FILE_MANAGER__)
        mmi_fmgr_send_copy_req(
            FMGR_ACTION_MOVE,
            (U8*) g_vobj_cntx.AppFilePathPtr,
            (U8*) g_vcard_cntx.FilePathPtr,
            mmi_vcard_recv_move_obj_to_file_callback);
        result = VOBJ_ERR_NO_ERROR;
    #endif /* defined(__MMI_FILE_MANAGER__) */ 
    }
    else if (mmi_vobj_get_file_buffer_size() > 0)
    {
        src_path = OslMalloc(VOBJ_MAX_PATH_LENGTH);
        mmi_vobj_get_full_path_name(g_vobj_cntx.buffer_begin, src_path, MMI_VOBJ_VCARD);

        /* Use move instead of rename because user may save to card. */
        /* result = FS_Rename((U16*)src_path, (U16*)g_vcard_cntx.FilePathPtr); */
    #if defined(__MMI_FILE_MANAGER__)
        mmi_fmgr_send_copy_req(
            FMGR_ACTION_MOVE,
            (U8*) src_path,
            (U8*) g_vcard_cntx.FilePathPtr,
            mmi_vcard_recv_move_obj_to_file_callback);
        result = VOBJ_ERR_NO_ERROR;
    #endif /* defined(__MMI_FILE_MANAGER__) */ 

        OslMfree(src_path);
    }
    else
    {
        result = VOBJ_ERR_UNKNOWN;
    }

    return result;
}


#if defined(__MMI_FILE_MANAGER__)
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_recv_move_obj_to_file_callback
 * DESCRIPTION
 *  move vcard to the path selected by user.
 * PARAMETERS
 *  info        [?]     
 * RETURNS
 *  S32   error cause(?)
 *****************************************************************************/
static void mmi_vcard_recv_move_obj_to_file_callback(void *info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mmi_fmt_copy_rsp_strcut *msgPtr;
    int attr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    msgPtr = (mmi_fmt_copy_rsp_strcut*) info;

    if (msgPtr->result >= 0)
    {
        attr = MMI_FS_GetAttributes((U8*) g_vcard_cntx.FilePathPtr);
        attr &= ~FS_ATTR_HIDDEN;
        MMI_FS_SetAttributes((U8*) g_vcard_cntx.FilePathPtr, (BYTE)attr);
        mmi_vcard_forward_fmgr_save_file_done(VOBJ_ERR_NO_ERROR);
    }
    else
    {
        mmi_vcard_forward_fmgr_save_file_done(msgPtr->result);
    }
}
#endif /* defined(__MMI_FILE_MANAGER__) */
/*--------------------------------------------------END vCard Received operation ---------------------------------------------------*/


/*--------------------------------------------------BEGIN vCard writer ---------------------------------------------------*/
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
void mmi_vcard_get_bday_to_buff(U16 store_index, S8 *b_day)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 bday_record, bday_index;
    S16 pError;
    MMI_PHB_BDAY_STRUCT bday_data[NVRAM_EF_PHB_BIRTHDAY_COUNT];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    bday_record = (store_index / NVRAM_EF_PHB_BIRTHDAY_COUNT) + 1;
    bday_index = store_index - (bday_record - 1) * NVRAM_EF_PHB_BIRTHDAY_COUNT;

    ReadRecord(NVRAM_EF_PHB_BIRTHDAY_LID, bday_record, (void*)bday_data, NVRAM_EF_PHB_BIRTHDAY_SIZE, &pError);

    /* Year */
    if (bday_data[bday_index].bYear <= MMI_PHB_BDAY_MAX_YEAR_INT)
    {
            snprintf(b_day, 5, "%04d", bday_data[bday_index].bYear);
    }
    else
    {
        strcpy(b_day, "0000");
    }

    /* Month */
    if (bday_data[bday_index].bMonth <= 12)
    {
            snprintf(b_day+4 , 3, "%02d", bday_data[bday_index].bMonth);
    }
    else
    {
        strcpy(b_day + 4, "00");
    }

    /* Day */
    if (bday_data[bday_index].bDay <= 31)
    {
            snprintf(b_day+6, 3, "%02d", bday_data[bday_index].bDay);
    }
    else
    {
        strcpy(b_day + 6, "00");
    }
}
#endif /* defined(__MMI_PHB_BIRTHDAY_FIELD__) */ 


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_writer_v21_mycard_to_file
 * DESCRIPTION
 *  write my vcard entry into vcard format into destination file.
 * PARAMETERS
 *  path        [IN]        Destination path
 * RETURNS
 *  void
 *****************************************************************************/
S32 mmi_vcard_writer_v21_mycard_to_file(S8 *path)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_PHB_VCARD_STRUCT myCard;
    vcard_field_struct field_content;
    S16 pError;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Read my vCard from NVRAM. */
    ReadRecord(NVRAM_EF_PHB_VCARD_LID, 1, (void*)&myCard, MMI_PHB_VCARD_RECORD_SIZE, &pError);
    memset((void*)&field_content, 0, sizeof(field_content));

    field_content.ucs2_name = (S8*) myCard.name;
    field_content.number = (S8*) myCard.number;
    field_content.home_num = (S8*) myCard.homeNumber;
    field_content.office_num = (S8*) myCard.officeNumber;
    field_content.fax_num = (S8*) myCard.faxNumber;
    field_content.email_addr = (S8*) myCard.emailAddress;
#ifdef __MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__
    field_content.email_addr2 = (S8*) myCard.emailAddress2;
#endif
    field_content.ucs2_company = (S8*) myCard.companyName;
    field_content.b_day = NULL;
    field_content.info_fields = NULL;
    field_content.optional_ids = NULL;
    field_content.geo_fields = NULL;
    field_content.nick_name = NULL;

    return mmi_vcard_writer_to_file(path, &field_content);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_writer_v21_select_card_to_file
 * DESCRIPTION
 *  write select phonebook entry into vcard format into destination file.
 * PARAMETERS
 *  path        [IN]        Destination path
 * RETURNS
 *  void
 *****************************************************************************/
S32 mmi_vcard_writer_v21_select_card_to_file(S8 *path)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 store_index;
    S8 number_buff[MAX_PB_NUMBER_LENGTH + 1 + 1];
    S8 number_buff2[MAX_PB_NUMBER_LENGTH + 1 + 1];
    PHB_OPTIONAL_IDS_STRUCT *opt_ids = NULL;
    vcard_field_struct field_content;
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
    S16 pError;
#endif

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_PHB, "VCARD func:%s", __FUNCTION__);
    if (phbListView == MMI_PHB_LIST_FOR_ALL_SHARED)
    {
        store_index = g_phb_name_index[g_phb_cntx.active_index_second];
    }
    else
    {
        store_index = g_phb_name_index[g_phb_cntx.active_index];
    }
//    phbListView = MMI_PHB_LIST_FOR_NONE;
    memset((void*)&field_content, 0, sizeof(field_content));
    
    /* Mobile Number */
    if (((PhoneBook[store_index].tel.type & 0x90) == 0x90) && (PhoneBook[store_index].tel.type != 0xFF))
    {
        mmi_phb_convert_to_digit_by_store_index((U8*) number_buff2, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1, store_index);
        /* now number is +**21*8123456# */
        /* call the third API to transfer + to place just after SS pattern */
        applib_decode_number((U8*)number_buff2, (U8*)number_buff, 0x91);
        /* now number is **21*+8123456#, so we can show the original data to user */
    }
    else
    {
        mmi_phb_convert_to_digit_by_store_index((U8*) number_buff, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1, store_index);
    }

    field_content.ucs2_name = (S8*) PhoneBook[store_index].alpha_id.name;
    field_content.number = (S8*) number_buff;
    
    if (store_index < MAX_PB_PHONE_ENTRIES) /* In NVRAM */
    {
    #if defined(__MMI_PHB_BIRTHDAY_FIELD__)
        S8 b_day[9];    /* Format: yyyymmdd */
        mmi_vcard_get_bday_to_buff(store_index, (S8*) b_day);

        field_content.b_day = b_day;
    #endif /* defined(__MMI_PHB_BIRTHDAY_FIELD__) */ 
    #if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
        ReadRecord(
            NVRAM_EF_PHB_SOMEFIELDS_LID,
            (U16) (store_index + 1),
            (void*)&PhoneBookOptionalFields,
            OPTIONAL_FIELDS_RECORD_SIZE,
            &pError);

        field_content.home_num = (S8*) PhoneBookOptionalFields.homeNumber;
        field_content.office_num = (S8*) PhoneBookOptionalFields.officeNumber;
        field_content.fax_num = (S8*) PhoneBookOptionalFields.faxNumber;
        field_content.email_addr = (S8*) PhoneBookOptionalFields.emailAddress;
    #ifdef __MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__
        field_content.email_addr2 = (S8*) PhoneBookOptionalFields.emailAddress2;
    #endif
        field_content.ucs2_company = (S8*) PhoneBookOptionalFields.companyName;
    #endif /* !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) */
    #ifdef __MMI_PHB_INFO_FIELD__
        mmi_phb_info_read_data_to_buff(store_index);
        field_content.info_fields = &PhoneBookInfoFields;
    #endif 
        mmi_phb_read_optional_ids(store_index);
        opt_ids = (PHB_OPTIONAL_IDS_STRUCT*) g_phb_cntx.optional_ids;
        field_content.optional_ids = opt_ids;

    #if defined(__MMI_PHB_GEOGRAPHIC_FIELD__)
        field_content.geo_fields = &PhoneBookGeoFields,
    #endif
    }

    return mmi_vcard_writer_to_file(path, &field_content);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_writer_v21_to_file
 * DESCRIPTION
 *  write entry into vcard format into destination file.
 * PARAMETERS
 *  path                [IN]        Destination path
 *  ucs2_name           [IN]         
 *  number              [IN]         
 *  home_num            [IN]         
 *  office_num          [IN]         
 *  fax_num             [IN]         
 *  email_addr          [IN]         
 *  ucs2_company        [IN]         
 *  b_day               [IN]         
 *  title               [?]         
 *  url                 [?]         
 *  note                [?]         
 * RETURNS
 *  void
 *****************************************************************************/
S32 mmi_vcard_writer_v21_to_file(S8 *path, vcard_field_struct *field_content)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FS_HANDLE fh;
    INT32 write_len;
    U32 object_len;
    U32 temp_len;
    S8 *object;
    S8 *temp_buf;
    S32 result;
    U8 preferred_num = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_PHB, "VCARD func:%s", __FUNCTION__);
#ifdef __MMI_PBAP_SUPPORT__
    if (is_build_for_pbap)
    {
        fh = MMI_FS_Open((U8*) path, FS_READ_WRITE);
    }
    else
#endif /* __MMI_PBAP_SUPPORT__ */
    fh = MMI_FS_Open((U8*)path, FS_CREATE |FS_READ_WRITE);

    if(fh >= 0)
    {
    #ifdef __MMI_PHB_INFO_FIELD__
        object = g_vcard_write_buff;
    #else /* __MMI_PHB_INFO_FIELD__ */
        /* Allocate buffer */
        object = OslMalloc(VCARD_WRITE_BUFF_LEN);
    #endif /* __MMI_PHB_INFO_FIELD__ */
        memset(object, 0, VCARD_WRITE_BUFF_LEN);
    
        temp_buf = OslMalloc(VCARD_WRITER_FIELD_BUFFER);
        memset(temp_buf, 0, 2);
        /* Header */
        strcpy(object, g_vcard_field[VCARD_FIELD_BEGIN]);
        strcat(object, VOBJ_VALUE_SEP_STR);
        strcat(object, VCARD_HEADER);
        strcat(object, VCARD_LINE_BREAK);

        /* Version */
        strcat(object, g_vcard_field[VCARD_FIELD_VERSION]);
        strcat(object, VOBJ_VALUE_SEP_STR);
        strcat(object, VCARD_VERSION_21);
        strcat(object, VCARD_LINE_BREAK);

    #ifdef __MMI_PBAP_SUPPORT__
        if (is_build_for_pbap)
        {
            mmi_vcard_writer_v21_append_name(object, temp_buf, (S8*) g_vcard_field[VCARD_FIELD_N], field_content->ucs2_name);
        }
        else
    #endif /* __MMI_PBAP_SUPPORT__ */
        /* Name Field, write N field. */
        if (pfnUnicodeStrlen(field_content->ucs2_name))
        {
            mmi_vcard_writer_v21_append_name(object, temp_buf, (S8*) g_vcard_field[VCARD_FIELD_N], field_content->ucs2_name);
            /* mmi_vcard_writer_v21_append_name(object, temp_buf, (S8*) g_vcard_field[VCARD_FIELD_FN], ucs2_name); */
        }

    #if defined(__MMI_PHB_PREFERRED_NUMBER_FIELD__)
        preferred_num = field_content->optional_ids->preferredNumber;
    #endif /* defined(__MMI_PHB_PREFERRED_NUMBER_FIELD__)  */
    
#ifdef __MMI_PBAP_SUPPORT__
        if (is_build_for_pbap)
        {
            mmi_vcard_writer_v21_append_number(object, VCARD_PARA_CELL, field_content->number, preferred_num);
        }
        else
#endif /* __MMI_PBAP_SUPPORT__ */

        /* Mobile Number */
        if (strlen(field_content->number))
        {
            mmi_vcard_writer_v21_append_number(object, VCARD_PARA_CELL, field_content->number, preferred_num);
        }

    #if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
        /* Home Number */
        if ((field_content->home_num != NULL) && strlen(field_content->home_num))
        {
            mmi_vcard_writer_v21_append_number(object, VCARD_PARA_HOME, field_content->home_num, preferred_num);			
        }

        /* Office Number */
        if ((field_content->office_num != NULL) && strlen(field_content->office_num))
        {
            mmi_vcard_writer_v21_append_number(object, VCARD_PARA_WORK, field_content->office_num, preferred_num);				
        }

        /* Fax Number */
        if ((field_content->fax_num != NULL) && strlen(field_content->fax_num))
        {
            mmi_vcard_writer_v21_append_number(object, VCARD_PARA_FAX,field_content->fax_num, preferred_num);							
        }

        /* Email */
        if ((field_content->email_addr != NULL) && strlen(field_content->email_addr))
        {
            temp_len = strlen(object);
            sprintf((object + temp_len), "%s;%s:%s%s", g_vcard_field[VCARD_FIELD_EMAIL], 
            g_vcard_email_para[VCARD_EMAIL_PARA_INTERNET], field_content->email_addr, VCARD_LINE_BREAK);
        }
        
    #ifdef __MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__
        /* Email */
        if ((field_content->email_addr2 != NULL) && strlen(field_content->email_addr2))
        {
            temp_len = strlen(object);
            sprintf((object + temp_len), "%s;%s:%s%s", g_vcard_field[VCARD_FIELD_EMAIL], 
            g_vcard_email_para[VCARD_EMAIL_PARA_INTERNET], field_content->email_addr2, VCARD_LINE_BREAK);
        }
    #endif /* __MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__ */

        /* Company Name */
        if ((field_content->ucs2_company != NULL) && pfnUnicodeStrlen(field_content->ucs2_company))
        {
            mmi_vcard_writer_v21_append_name(object, temp_buf, (S8*) g_vcard_field[VCARD_FIELD_ORG], field_content->ucs2_company);
        }
    #endif /* !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) */

    #ifdef __MMI_PHB_BIRTHDAY_FIELD__
        /* Birthday */
        if ((field_content->b_day != NULL) && strlen(field_content->b_day) && strcmp(field_content->b_day, "00000000") != 0)
        {
            temp_len = strlen(object);
            sprintf((object + temp_len), "%s:%s%s", g_vcard_field[VCARD_FIELD_BDAY], field_content->b_day, VCARD_LINE_BREAK);
        }
    #endif/* __MMI_PHB_BIRTHDAY_FIELD__ */

    #ifdef __MMI_PHB_INFO_FIELD__
        /* Info field */
        if (field_content->info_fields != NULL)
        {
            /* title */
            if (pfnUnicodeStrlen((S8*)field_content->info_fields->title))
            {
                mmi_vcard_writer_v21_append_name(object, temp_buf, (S8*) g_vcard_field[VCARD_FIELD_TITLE], (S8*)field_content->info_fields->title);
            }
            /* url */
            if (strlen((const char*)field_content->info_fields->url))
            {
                temp_len = strlen(object);
                sprintf((object + temp_len), "%s:%s%s", g_vcard_field[VCARD_FIELD_URL], field_content->info_fields->url, VCARD_LINE_BREAK);
            }
            /* address */
            if (pfnUnicodeStrlen((S8*)field_content->info_fields->address.pobox) || pfnUnicodeStrlen((S8*)field_content->info_fields->address.extension) || pfnUnicodeStrlen((S8*)field_content->info_fields->address.street) || pfnUnicodeStrlen((S8*)field_content->info_fields->address.city) ||
                 pfnUnicodeStrlen((S8*)field_content->info_fields->address.state) || pfnUnicodeStrlen((S8*)field_content->info_fields->address.postalcode) || pfnUnicodeStrlen((S8*)field_content->info_fields->address.country))
            {
                mmi_vcard_writer_v21_append_address(object, temp_buf, (S8*) g_vcard_field[VCARD_FIELD_ADR], &(field_content->info_fields->address));
            }
            /* note */
            if (pfnUnicodeStrlen((S8*)field_content->info_fields->note))
            {
                mmi_vcard_writer_v21_append_name(object, temp_buf, (S8*) g_vcard_field[VCARD_FIELD_NOTE], (S8*)field_content->info_fields->note);
            }
        }
    #endif /* __MMI_PHB_INFO_FIELD__ */
    #ifdef __MMI_PBAP_SUPPORT__
        if (is_build_for_pbap)
        {
            if (field_content->call_num_type != 0)
            {
                mmi_vcard_append_calllog_entry_time(object, field_content->start_time, field_content->call_num_type);    
            }        
        }
    #endif
        /* Footer */
        strcat(object, g_vcard_field[VCARD_FIELD_END]);
        strcat(object, VOBJ_VALUE_SEP_STR);
        strcat(object, VCARD_HEADER);
        strcat(object, VCARD_LINE_BREAK);

        /* Write to file. */
        object_len = strlen(object);

        MMI_ASSERT(object_len < VCARD_WRITE_BUFF_LEN);

    #ifdef __MMI_PBAP_SUPPORT__
        if (is_build_for_pbap)
        {
            if (is_build_append)
            {
                MMI_FS_Seek(fh, 0, FS_FILE_END);
            }
            result = MMI_FS_Write(fh, object, object_len, (U32*) & write_len);
            MMI_FS_Close(fh);
        }
        else
    #endif /* __MMI_PBAP_SUPPORT__ */
        {
		result = MMI_FS_Write(fh, object, object_len, (INT32 *)&write_len);
		MMI_FS_Close(fh);
        }

        OslMfree(temp_buf);
    #ifndef __MMI_PHB_INFO_FIELD__
        OslMfree(object);
    #endif
        return result;
    }

    return (S32) fh;

}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_writer_v21_append_name
 * DESCRIPTION
 *  
 * PARAMETERS
 *  object          [?]     
 *  temp_buf        [?]     
 *  field_name      [?]     
 *  field_value     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_writer_v21_append_name(S8 *object, S8 *temp_buf, S8 *field_name, S8 *field_value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 temp_len;
    U8 valueType = 0;
    U8 isUCS2 = 1;
    S8 *qp_buf;
    S8 *temp_ucs2_buff;
    U16 i = 0, j = 0, num_of_semicolon = 0;
    U16 field_length = pfnUnicodeStrlen(field_value);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Always use UTF-8 to encode. */
    if ((isUCS2 /*=GetUCS2Flag(field_value) */ ) > 0)
    {
        i = 0;
        while (i < field_length)
        {
            if ((field_value[2 * i] == ';' && field_value[2 * i + 1] == 0) || (field_value[2 * i] == '\\' && field_value[2 * i + 1] == 0))
            {
                num_of_semicolon++;
            }
            i++;
        }

        if (num_of_semicolon > 0)
        {
            temp_ucs2_buff = OslMalloc((field_length + num_of_semicolon + 1) * 2);
            memset(temp_ucs2_buff, 0, ((field_length + num_of_semicolon + 1) * 2));

            i = 0;
            j = 0;
            while (i < field_length)
            {
                if ((field_value[2 * i] == ';' && field_value[2 * i + 1] == 0) || (field_value[2 * i] == '\\' && field_value[2 * i + 1] == 0))
                {
                    temp_ucs2_buff[2 * j] = '\\';
                    temp_ucs2_buff[2 * j + 1] = '\0';
                    j++;
                }
                temp_ucs2_buff[2 * j] = field_value[2 * i];
                temp_ucs2_buff[2 * j + 1] = field_value[2 * i + 1];
                i++;
                j++;
            }        
       
            valueType = mmi_vcard_util_convert_to_system_encoding(temp_buf, temp_ucs2_buff);
            OslMfree(temp_ucs2_buff);       
        }
        else
        {
            valueType = mmi_vcard_util_convert_to_system_encoding(temp_buf, field_value);            
        }
    }
    else
    {
        UnicodeToAnsii(temp_buf, field_value);
    }

    qp_buf = OslMalloc(VCARD_WRITER_FIELD_BUFFER);
    memset(qp_buf, 0, VCARD_WRITER_FIELD_BUFFER);
    applib_qprint_encode_ext(temp_buf, strlen(temp_buf), qp_buf, VOBJ_WRITE_BUFF_LEN, TRUE);

    /* Append Field Name */
    strcat(object, field_name);

    /* Append Charset Type */
    if (isUCS2)
    {
        temp_len = strlen(object);
        sprintf((object + temp_len), ";%s=%s", g_vobj_paras[VOBJ_PARA_CHARSET], g_vobj_charset[valueType]);
    }

    /* Append Encoding Type */
    temp_len = strlen(object);
    sprintf((object + temp_len), ";%s=%s", g_vobj_paras[VOBJ_PARA_ENCODING], g_vobj_encoding[VOBJ_EN_QP]);

    strcat(object, VOBJ_VALUE_SEP_STR);
    strcat(object, qp_buf);
    strcat(object, VCARD_LINE_BREAK);
    OslMfree(qp_buf);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_writer_v21_append_number
 * DESCRIPTION
 *  
 * PARAMETERS
 *  object          [?]     
 *  temp_buf        [?]     
 *  field_name      [?]     
 *  field_value     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_writer_v21_append_number(S8 *object, U8 number_type, S8 *field_value, U8 preferred_num)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 temp_len;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    temp_len = strlen(object);
#if defined(__MMI_PHB_PREFERRED_NUMBER_FIELD__)
    if ((number_type == VCARD_PARA_CELL && preferred_num == MMI_PHB_PREFERRED_NO_MOBILE_NUMBER)
    #if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
        ||(number_type == VCARD_PARA_HOME && preferred_num == MMI_PHB_PREFERRED_NO_HOME_NUMBER)
        ||(number_type == VCARD_PARA_WORK && preferred_num == MMI_PHB_PREFERRED_NO_OFFICE_NUMBER)
        ||(number_type == VCARD_PARA_FAX && preferred_num == MMI_PHB_PREFERRED_NO_FAX_NUMBER)        
    #endif
        )
    {
        sprintf(
            (object + temp_len),
            "%s;%s;%s:%s%s",
            g_vcard_field[VCARD_FIELD_TEL],
            g_vcard_para[number_type],
            g_vcard_para[VCARD_PARA_PREF],
            field_value,
            VCARD_LINE_BREAK);
    }
    else
#endif /* defined(__MMI_PHB_PREFERRED_NUMBER_FIELD__) */
    {
        sprintf(
            (object + temp_len),
            "%s;%s:%s%s",
            g_vcard_field[VCARD_FIELD_TEL],
            g_vcard_para[number_type],
            field_value,
            VCARD_LINE_BREAK);
    }
}

#ifdef __MMI_PHB_INFO_FIELD__
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_writer_v21_append_address
 * DESCRIPTION
 *  
 * PARAMETERS
 *  object          [?]     
 *  temp_buf        [?]     
 *  field_name      [?]     
 *  field_value     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_writer_v21_append_address(S8 *object, S8 *temp_buf, S8 *field_name, MMI_PHB_ADDRESS_STRUCT *field_value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 temp_len;
    S8 *qp_buf;	

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Append Field Name */
    strcat(object, field_name);

    temp_len = strlen(object);
    sprintf((object + temp_len), ";%s=%s", g_vobj_paras[VOBJ_PARA_CHARSET], g_vobj_charset[VOBJ_CH_UTF8]);

    /* Append Encoding Type */
    temp_len = strlen(object);
    sprintf((object + temp_len), ";%s=%s", g_vobj_paras[VOBJ_PARA_ENCODING], g_vobj_encoding[VOBJ_EN_QP]);

    strcat(object, VOBJ_VALUE_SEP_STR);
    qp_buf = OslMalloc(VCARD_WRITER_FIELD_BUFFER);
    if (pfnUnicodeStrlen((S8*)(field_value->pobox)))
    {
        mmi_vcard_util_convert_to_system_encoding(temp_buf, (S8*)(field_value->pobox));
        applib_qprint_encode_ext(temp_buf, strlen(temp_buf), qp_buf, VOBJ_WRITE_BUFF_LEN, TRUE);
        strcat(object, qp_buf);
    }
    strcat(object, VOBJ_VALUE_SEP_SEMI_COLON);

    if (pfnUnicodeStrlen((S8*)(field_value->extension)))
    {
        mmi_vcard_util_convert_to_system_encoding(temp_buf, (S8*)(field_value->extension));
        applib_qprint_encode_ext(temp_buf, strlen(temp_buf), qp_buf, VOBJ_WRITE_BUFF_LEN, TRUE);
        strcat(object, qp_buf);
    }
    strcat(object, VOBJ_VALUE_SEP_SEMI_COLON);

    if (pfnUnicodeStrlen((S8*)(field_value->street)))
    {
        mmi_vcard_util_convert_to_system_encoding(temp_buf, (S8*)(field_value->street));
        applib_qprint_encode_ext(temp_buf, strlen(temp_buf), qp_buf, VOBJ_WRITE_BUFF_LEN, TRUE);
        strcat(object, qp_buf);
    }
    strcat(object, VOBJ_VALUE_SEP_SEMI_COLON);

    if (pfnUnicodeStrlen((S8*)(field_value->city)))
    {
        mmi_vcard_util_convert_to_system_encoding(temp_buf, (S8*)(field_value->city));
        applib_qprint_encode_ext(temp_buf, strlen(temp_buf), qp_buf, VOBJ_WRITE_BUFF_LEN, TRUE);
        strcat(object, qp_buf);
    }
    strcat(object, VOBJ_VALUE_SEP_SEMI_COLON);

    if (pfnUnicodeStrlen((S8*)(field_value->state)))
    {
        mmi_vcard_util_convert_to_system_encoding(temp_buf, (S8*)(field_value->state));
        applib_qprint_encode_ext(temp_buf, strlen(temp_buf), qp_buf, VOBJ_WRITE_BUFF_LEN, TRUE);
        strcat(object, qp_buf);
    }
    strcat(object, VOBJ_VALUE_SEP_SEMI_COLON);

    if (pfnUnicodeStrlen((S8*)(field_value->postalcode)))
    {
        mmi_vcard_util_convert_to_system_encoding(temp_buf, (S8*)(field_value->postalcode));
        applib_qprint_encode_ext(temp_buf, strlen(temp_buf), qp_buf, VOBJ_WRITE_BUFF_LEN, TRUE);
        strcat(object, qp_buf);
    }
    strcat(object, VOBJ_VALUE_SEP_SEMI_COLON);

    if (pfnUnicodeStrlen((S8*)(field_value->country)))
    {
        mmi_vcard_util_convert_to_system_encoding(temp_buf, (S8*)(field_value->country));
        applib_qprint_encode_ext(temp_buf, strlen(temp_buf), qp_buf, VOBJ_WRITE_BUFF_LEN, TRUE);
        strcat(object, qp_buf);
    }

    strcat(object, VCARD_LINE_BREAK);
    OslMfree(qp_buf);
}
#endif

#if defined(__MMI_VCARD_V30__)
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_util_transfer_capital_to_lowercase
 * DESCRIPTION
 *  
 * PARAMETERS
 *  object          [?]     
 *  temp_buf        [?]     
 *  field_name      [?]     
 *  field_value     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_util_transfer_capital_to_lowercase(S8* output, S8* input)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; *(input + i) != '\0'; i++)
    {
        *(output + i) = *(input + i) + 32;
    }
    *(output + i) = '\0';
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_writer_v30_to_file
 * DESCRIPTION
 *  write entry into vcard format into destination file.
 * PARAMETERS
 *  path                [IN]        Destination path
 *  ucs2_name           [IN]         
 *  number              [IN]         
 *  home_num            [IN]         
 *  office_num          [IN]         
 *  fax_num             [IN]         
 *  email_addr          [IN]         
 *  ucs2_company        [IN]         
 *  b_day               [IN]         
 *  info_fields         [IN]         
 *  nick_name           [IN]         
 * RETURNS
 *  void
 *****************************************************************************/
static S32 mmi_vcard_writer_v30_to_file(S8 *path, vcard_field_struct *field_content)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FS_HANDLE fh;
    U32 write_len;
    U32 object_len;
    U32 temp_len;
    S8 *object;
    S8 *temp_buf;
    S32 result;
    S8 para_buff[10];
    U8 preferred_num = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Open the file to write, overwrite if already exists. */
    
#ifdef __MMI_PBAP_SUPPORT__
    if (is_build_for_pbap)
    {
        fh = MMI_FS_Open((U8*) path, FS_READ_WRITE);
    }
    else
#endif /* __MMI_PBAP_SUPPORT__ */
    fh = MMI_FS_Open((U8*) path, FS_CREATE | FS_READ_WRITE);

    if (fh > 0)
    {
    #ifdef __MMI_PHB_INFO_FIELD__
        object = g_vcard_write_buff;
    #else /* __MMI_PHB_INFO_FIELD__ */
        /* Allocate buffer */
        object = OslMalloc(VCARD_WRITE_BUFF_LEN);
    #endif /* __MMI_PHB_INFO_FIELD__ */
        memset(object, 0, VCARD_WRITE_BUFF_LEN);

        temp_buf = OslMalloc(VCARD_WRITER_FIELD_BUFFER);

        /* Header */
        strcpy(object, g_vcard_field[VCARD_FIELD_BEGIN]);
        strcat(object, VOBJ_VALUE_SEP_STR);
        strcat(object, VCARD_HEADER);
        strcat(object, VCARD_LINE_BREAK);

        /* Version */
        strcat(object, g_vcard_field[VCARD_FIELD_VERSION]);
        strcat(object, VOBJ_VALUE_SEP_STR);
        strcat(object, VCARD_VERSION_30);
        strcat(object, VCARD_LINE_BREAK);
        
#ifdef __MMI_PBAP_SUPPORT__
        if (is_build_for_pbap)
        {
            mmi_vcard_writer_v30_append_name(object, temp_buf, (S8*) g_vcard_field[VCARD_FIELD_N], field_content->ucs2_name);
        }
        else
#endif /* __MMI_PBAP_SUPPORT__ */

        /* Name Field, write N field. */
        if (pfnUnicodeStrlen(field_content->ucs2_name))
        {
            mmi_vcard_writer_v30_append_name(object, temp_buf, (S8*) g_vcard_field[VCARD_FIELD_N], field_content->ucs2_name);
            /* mmi_vcard_writer_v21_append_name(object, temp_buf, (S8*) g_vcard_field[VCARD_FIELD_FN], ucs2_name); */
        }

    #if defined(__MMI_PHB_PREFERRED_NUMBER_FIELD__)
        preferred_num = field_content->optional_ids->preferredNumber;
    #endif /* defined(__MMI_PHB_PREFERRED_NUMBER_FIELD__)  */
    
#ifdef __MMI_PBAP_SUPPORT__
        if (is_build_for_pbap)
        {
            mmi_vcard_writer_v30_append_number(object, VCARD_PARA_CELL, field_content->number, preferred_num);
        }
        else
#endif /* __MMI_PBAP_SUPPORT__ */

        /* Mobile Number */
        if (strlen(field_content->number))
        {
            mmi_vcard_writer_v30_append_number(object, VCARD_PARA_CELL, field_content->number, preferred_num);
        }

    #if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
        /* Home Number */
        if ((field_content->home_num != NULL) && strlen(field_content->home_num))
        {
            mmi_vcard_writer_v30_append_number(object, VCARD_PARA_HOME, field_content->home_num, preferred_num);
        }

        /* Office Number */
        if ((field_content->office_num != NULL) && strlen(field_content->office_num))
        {
            mmi_vcard_writer_v30_append_number(object, VCARD_PARA_WORK, field_content->office_num, preferred_num);               
        }

        /* Fax Number */
        if ((field_content->fax_num != NULL) && strlen(field_content->fax_num))
        {
            mmi_vcard_writer_v30_append_number(object, VCARD_PARA_FAX, field_content->fax_num, preferred_num);               
        }

        /* Email */
        if ((field_content->email_addr != NULL) && strlen(field_content->email_addr))
        {
            mmi_vcard_util_transfer_capital_to_lowercase(para_buff, (S8*) g_vcard_email_para[VCARD_EMAIL_PARA_INTERNET]);
            temp_len = strlen(object);
            sprintf(
                (object + temp_len),
                "%s;%s=%s:%s%s",
                g_vcard_field[VCARD_FIELD_EMAIL],
                g_vcard_type_para[VCARD_TYPE_PARA_TYPE],
                para_buff,
                field_content->email_addr,
                VCARD_LINE_BREAK);
        }

    #ifdef __MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__
        /* Email */
        if ((field_content->email_addr2 != NULL) && strlen(field_content->email_addr2))
        {
            mmi_vcard_util_transfer_capital_to_lowercase(para_buff, (S8*) g_vcard_email_para[VCARD_EMAIL_PARA_INTERNET]);
            temp_len = strlen(object);
            sprintf(
                (object + temp_len),
                "%s;%s=%s:%s%s",
                g_vcard_field[VCARD_FIELD_EMAIL],
                g_vcard_type_para[VCARD_TYPE_PARA_TYPE],
                para_buff,
                field_content->email_addr2,
                VCARD_LINE_BREAK);
        }
    #endif /* __MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__ */

        /* Company Name */
        if ((field_content->ucs2_company != NULL) && pfnUnicodeStrlen(field_content->ucs2_company))
        {
            mmi_vcard_writer_v30_append_name(object, temp_buf, (S8*) g_vcard_field[VCARD_FIELD_ORG], field_content->ucs2_company);
        }
    #endif /* !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) */

    #ifdef __MMI_PHB_BIRTHDAY_FIELD__
        /* Birthday */
        if ((field_content->b_day != NULL) && strlen(field_content->b_day) && strcmp(field_content->b_day, "00000000") != 0)
        {
            temp_len = strlen(object);
            sprintf((object + temp_len), "%s:%s%s", g_vcard_field[VCARD_FIELD_BDAY], field_content->b_day, VCARD_LINE_BREAK);
        }
    #endif/* __MMI_PHB_BIRTHDAY_FIELD__ */

    #ifdef __MMI_PHB_INFO_FIELD__
        /* Info field */
        if (field_content->info_fields != NULL)
        {
            /* title */
            if (pfnUnicodeStrlen((S8*)field_content->info_fields->title))
            {
                mmi_vcard_writer_v30_append_name(object, temp_buf, (S8*) g_vcard_field[VCARD_FIELD_TITLE], (S8*)field_content->info_fields->title);
            }
            /* url */
            if (strlen((const char*)field_content->info_fields->url))
            {
                temp_len = strlen(object);
                sprintf((object + temp_len), "%s:%s%s", g_vcard_field[VCARD_FIELD_URL], field_content->info_fields->url, VCARD_LINE_BREAK);
            }
            /* address */
            if (pfnUnicodeStrlen((S8*)field_content->info_fields->address.pobox) || pfnUnicodeStrlen((S8*)field_content->info_fields->address.extension) || pfnUnicodeStrlen((S8*)field_content->info_fields->address.street) || pfnUnicodeStrlen((S8*)field_content->info_fields->address.city) ||
                 pfnUnicodeStrlen((S8*)field_content->info_fields->address.state) || pfnUnicodeStrlen((S8*)field_content->info_fields->address.postalcode) || pfnUnicodeStrlen((S8*)field_content->info_fields->address.country))
            {
                mmi_vcard_writer_v30_append_address(object, temp_buf, (S8*) g_vcard_field[VCARD_FIELD_ADR], &(field_content->info_fields->address));
            }
            /* note */
            if (pfnUnicodeStrlen((S8*)field_content->info_fields->note))
            {
                mmi_vcard_writer_v30_append_name(object, temp_buf, (S8*) g_vcard_field[VCARD_FIELD_NOTE], (S8*)field_content->info_fields->note);
            }
        }
    #endif /* def __MMI_PHB_INFO_FIELD__ */
    
    #ifdef __MMI_PBAP_SUPPORT__
        if (is_build_for_pbap)
        {
            if (field_content->call_num_type != 0)
            {
                mmi_vcard_append_calllog_entry_time(object, field_content->start_time, field_content->call_num_type);    
            }        
        }
    #endif

        /* Footer */
        strcat(object, g_vcard_field[VCARD_FIELD_END]);
        strcat(object, VOBJ_VALUE_SEP_STR);
        strcat(object, VCARD_HEADER);
        strcat(object, VCARD_LINE_BREAK);

        /* Write to file. */
        object_len = strlen(object);

        MMI_ASSERT(object_len < VCARD_WRITE_BUFF_LEN);

#ifdef __MMI_PBAP_SUPPORT__
        if (is_build_for_pbap)
        {
            if (is_build_append)
            {
                MMI_FS_Seek(fh, 0, FS_FILE_END);
            }
            result = FS_Write(fh, object, object_len, (U32*) & write_len);
            MMI_FS_Close(fh);
        }
        else
#endif /* __MMI_PBAP_SUPPORT__ */
        {
            result = FS_Write(fh, object, object_len, (U32*) & write_len);
            MMI_FS_Close(fh);
        }

        OslMfree(temp_buf);
    #ifndef __MMI_PHB_INFO_FIELD__
        OslMfree(object);
    #endif
        return result;
    }

    return (S32) fh;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_writer_v30_append_name
 * DESCRIPTION
 *  
 * PARAMETERS
 *  object          [?]     
 *  temp_buf        [?]     
 *  field_name      [?]     
 *  field_value     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_writer_v30_append_name(S8 *object, S8 *temp_buf, S8 *field_name, S8 *field_value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 temp_len;
    U8 valueType = 0;
    U8 isUCS2 = 1;    
    S8 *temp_ucs2_buff;
    U16 i = 0, j = 0, num_of_semicolon = 0;
    U16 field_length = pfnUnicodeStrlen(field_value);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Always use UTF-8 to encode. */
    if ((isUCS2 /* GetUCS2Flag(field_value) */ ) > 0)
    {
        i = 0;
        while (i < field_length)
        {
            if ((field_value[2 * i] == ';' && field_value[2 * i + 1] == 0) || (field_value[2 * i] == '\\' && field_value[2 * i + 1] == 0))
            {
                num_of_semicolon++;
            }
            i++;
        }

        if (num_of_semicolon > 0)
        {
            temp_ucs2_buff = OslMalloc((field_length + num_of_semicolon + 1) * 2);
            memset(temp_ucs2_buff, 0, ((field_length + num_of_semicolon + 1) * 2));

            i = 0;
            j = 0;
            while (i < field_length)
            {
                if ((field_value[2 * i] == ';' && field_value[2 * i + 1] == 0) || (field_value[2 * i] == '\\' && field_value[2 * i + 1] == 0))
                {
                    temp_ucs2_buff[2 * j] = '\\';
                    temp_ucs2_buff[2 * j + 1] = '\0';
                    j++;
                }
                temp_ucs2_buff[2 * j] = field_value[2 * i];
                temp_ucs2_buff[2 * j + 1] = field_value[2 * i + 1];
                i++;
                j++;
            }        
       
            valueType = mmi_vcard_util_convert_to_system_encoding(temp_buf, temp_ucs2_buff);
            OslMfree(temp_ucs2_buff);       
        }
        else
        {
            valueType = mmi_vcard_util_convert_to_system_encoding(temp_buf, field_value);            
        }
    }
    else
    {
        UnicodeToAnsii(temp_buf, field_value);
    }

    /* Append Field Name */
    strcat(object, field_name);

    /* Append Charset Type */
    if (isUCS2 /* GetUCS2Flag(field_value) */)
    {
        temp_len = strlen(object);
        sprintf((object + temp_len), ";%s=%s", g_vobj_paras[VOBJ_PARA_CHARSET], g_vobj_charset[valueType]);
    }

    /* Append Encoding Type */
    strcat(object, VOBJ_VALUE_SEP_STR);
    strcat(object, temp_buf);
    strcat(object, VCARD_LINE_BREAK);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_writer_v30_append_number
 * DESCRIPTION
 *  
 * PARAMETERS
 *  object          [?]     
 *  temp_buf        [?]     
 *  field_name      [?]     
 *  field_value     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_writer_v30_append_number(S8 *object, U8 number_type, S8 *field_value, U8 preferred_num)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 temp_len;
    S8 para_buff[10];
#if defined(__MMI_PHB_PREFERRED_NUMBER_FIELD__)
    S8 prefer_buff[10];]
#endif /* defined(__MMI_PHB_PREFERRED_NUMBER_FIELD__) */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    temp_len = strlen(object);
    mmi_vcard_util_transfer_capital_to_lowercase(para_buff, (S8*) g_vcard_para[number_type]);

#if defined(__MMI_PHB_PREFERRED_NUMBER_FIELD__)
    if ((number_type == VCARD_PARA_CELL && preferred_num == MMI_PHB_PREFERRED_NO_MOBILE_NUMBER)
    #if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
        ||(number_type == VCARD_PARA_HOME && preferred_num == MMI_PHB_PREFERRED_NO_HOME_NUMBER)
        ||(number_type == VCARD_PARA_WORK && preferred_num == MMI_PHB_PREFERRED_NO_OFFICE_NUMBER)
        ||(number_type == VCARD_PARA_FAX && preferred_num == MMI_PHB_PREFERRED_NO_FAX_NUMBER)        
    #endif
        )
    {
        mmi_vcard_util_transfer_capital_to_lowercase(prefer_buff, (S8*) g_vcard_para[VCARD_PARA_PREF]);
        sprintf(
            (object + temp_len),
            "%s;%s=%s,%s:%s%s",
            g_vcard_field[VCARD_FIELD_TEL],
            g_vcard_type_para[VCARD_TYPE_PARA_TYPE],
            para_buff,
            prefer_buff,
            field_value,
            VCARD_LINE_BREAK);
    }
    else
#endif /* defined(__MMI_PHB_PREFERRED_NUMBER_FIELD__) */
    {
        sprintf(
            (object + temp_len),
            "%s;%s=%s:%s%s",
            g_vcard_field[VCARD_FIELD_TEL],
            g_vcard_type_para[VCARD_TYPE_PARA_TYPE],
            para_buff,
            field_value,
            VCARD_LINE_BREAK);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_writer_v30_append_address
 * DESCRIPTION
 *  
 * PARAMETERS
 *  object          [?]     
 *  temp_buf        [?]     
 *  field_name      [?]     
 *  field_value     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_writer_v30_append_address(S8 *object, S8 *temp_buf, S8 *field_name, MMI_PHB_ADDRESS_STRUCT *field_value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 temp_len;	

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Append Field Name */
    strcat(object, field_name);
    temp_len = strlen(object);
    sprintf((object + temp_len), ";%s=%s", g_vobj_paras[VOBJ_PARA_CHARSET], g_vobj_charset[VOBJ_CH_UTF8]);

    strcat(object, VOBJ_VALUE_SEP_STR);
    if (pfnUnicodeStrlen((S8*)(field_value->pobox)))
    {
        mmi_vcard_util_convert_to_system_encoding(temp_buf, (S8*)(field_value->pobox));
        strcat(object, temp_buf);
    }
    strcat(object, VOBJ_VALUE_SEP_SEMI_COLON);

    if (pfnUnicodeStrlen((S8*)(field_value->extension)))
    {
        mmi_vcard_util_convert_to_system_encoding(temp_buf, (S8*)(field_value->extension));
        strcat(object, temp_buf);
    }
    strcat(object, VOBJ_VALUE_SEP_SEMI_COLON);

    if (pfnUnicodeStrlen((S8*)(field_value->street)))
    {
        mmi_vcard_util_convert_to_system_encoding(temp_buf, (S8*)(field_value->street));
        strcat(object, temp_buf);
    }
    strcat(object, VOBJ_VALUE_SEP_SEMI_COLON);

    if (pfnUnicodeStrlen((S8*)(field_value->city)))
    {
        mmi_vcard_util_convert_to_system_encoding(temp_buf, (S8*)(field_value->city));
        strcat(object, temp_buf);
    }
    strcat(object, VOBJ_VALUE_SEP_SEMI_COLON);

    if (pfnUnicodeStrlen((S8*)(field_value->state)))
    {
        mmi_vcard_util_convert_to_system_encoding(temp_buf, (S8*)(field_value->state));
        strcat(object, temp_buf);
    }
    strcat(object, VOBJ_VALUE_SEP_SEMI_COLON);

    if (pfnUnicodeStrlen((S8*)(field_value->postalcode)))
    {
        mmi_vcard_util_convert_to_system_encoding(temp_buf, (S8*)(field_value->postalcode));
        strcat(object, temp_buf);
    }
    strcat(object, VOBJ_VALUE_SEP_SEMI_COLON);

    if (pfnUnicodeStrlen((S8*)(field_value->country)))
    {
        mmi_vcard_util_convert_to_system_encoding(temp_buf, (S8*)(field_value->country));
        strcat(object, temp_buf);
    }

    strcat(object, VCARD_LINE_BREAK);
}
#endif /* defined(__MMI_VCARD_V30__) */

#ifdef __MMI_PBAP_SUPPORT__
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_append_calllog_entry_time
 * DESCRIPTION
 *  
 * PARAMETERS
 *   
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_append_calllog_entry_time(S8 *object, MYTIME *start_time, U8 type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 temp_len = strlen(object);
    U8 index;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (type)
    {
        case ITEM_DIALED_CALL:
            index = CALL_NUM_TYPE_DIALED;
            break;
        case ITEM_MISSED_CALL:
            index = CALL_NUM_TYPE_MISSED;
            break;
        case ITEM_RECVD_CALL:
            index = CALL_NUM_TYPE_RECEIVED;
            break;
        default:
            return;
    }
    
    sprintf(
        (object + temp_len),
        "%s:%04d%02d%02dT%02d%02d%02d%s",
        g_vcard_call_time[index],
        start_time->nYear,
        start_time->nMonth,
        start_time->nDay,
        start_time->nHour,
        start_time->nMin,
        start_time->nSec,
        VCARD_LINE_BREAK);
}
#endif /* __MMI_PBAP_SUPPORT__ */

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_writer_to_file
 * DESCRIPTION
 *  write entry into vcard format into destination file.
 * PARAMETERS
 *  path                [IN]        Destination path
 *  ucs2_name           [IN]         
 *  number              [IN]         
 *  home_num            [IN]         
 *  office_num          [IN]         
 *  fax_num             [IN]         
 *  email_addr          [IN]         
 *  ucs2_company        [IN]         
 *  b_day               [IN]         
 *  info_fields         [IN]         
 *  nick_name           [IN]         
 * RETURNS
 *  void
 *****************************************************************************/
S32 mmi_vcard_writer_to_file(S8 *path, vcard_field_struct *field_content)
{
#ifdef __MMI_PBAP_SUPPORT__
    if (is_build_for_pbap)
    {
        if (build_format == VCARD_FORMAT_30)
        {
        #if defined(__MMI_VCARD_V30__)
            return mmi_vcard_writer_v30_to_file(path, field_content);
        #else
            return PB_STATUS_NOT_SUPPORTED;
        #endif /* defined(__MMI_VCARD_V30__) */
        }
        else
        {
            return mmi_vcard_writer_v21_to_file(path, field_content);
        }
    }
    else
#endif /* __MMI_PBAP_SUPPORT__ */
#if defined(__MMI_VCARD_V30__)
    if (g_phb_cntx.vcard_version == MMI_PHB_VCARD_VERSION_30)
    {
        return mmi_vcard_writer_v30_to_file(path, field_content);
    }
    else
#endif /* defined(__MMI_VCARD_V30__) */
    {
        return mmi_vcard_writer_v21_to_file(path, field_content);
    }	
}

/*--------------------------------------------------END vCard writer ---------------------------------------------------*/

/*--------------------------------------------------Begin vcard parser ---------------------------------------------------*/


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_from_file_path
 * DESCRIPTION
 *  Clear buffer and open the file to parse.
 * PARAMETERS
 *  path        [IN]        The vcard file location in file system.
 * RETURNS
 *  parse result.
 *****************************************************************************/
S32 mmi_vcard_reader_from_file_path(S8 *path)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FS_HANDLE fh;
    UINT32 f_size;
    U8 error_code = MMI_VCARD_PARSE_UNKNOW_ERROR;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    fh = MMI_FS_Open((U8*)path, FS_READ_ONLY);

    if(fh >= 0)
    {
        MMI_FS_GetFileSize(fh, &f_size);
        mmi_phb_util_clear_buffer(TRUE);    /* Clear all phonebook temp buffer before parsing */

        error_code = mmi_vcard_reader_parse_line(fh);

    }
    else
    {
        error_code = fh;
    }

    MMI_FS_Close(fh);
    return error_code;
}

#ifdef __MMI_PHB_BIRTHDAY_FIELD__


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
U8 mmi_vcard_reader_retrive_bday(S8 *String, S8 *pb_year, S8 *pb_mon, S8 *pb_day, BOOL change_flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *cursor;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    cursor = mmi_vobj_uti_get_value(String);
    mmi_phb_retrieve_bday(cursor, pb_year, pb_mon, pb_day, change_flag);
    return MMI_VCARD_PARSE_NO_ERROR;
}
#endif /* __MMI_PHB_BIRTHDAY_FIELD__ */ 


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_uti_get_single_line
 * DESCRIPTION
 *  Get a line from file system, excluding the escape characters for vcard.
 * PARAMETERS
 *  strOutput       [IN/OUT]        Storage location for data
 *  fh              [IN]            File handle
 * RETURNS
 *  void
 *****************************************************************************/
S32 mmi_vcard_uti_get_single_line(S8 *strOutput, FS_HANDLE fh)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    INT32 nBytes = 0;
    S8 strCRLF[4];
    S8 strLF[2];
    S8 *currCursor;
    S8 str_photo_end[12];
    S8 *photo_end_cursor;
    S32 strLen = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    sprintf(strCRLF, "%c%c", VOBJ_CR_CHAR, VOBJ_LF_CHAR);
    sprintf(strLF, "%c", VOBJ_LF_CHAR);
    sprintf(str_photo_end, "//Z%s%s", strCRLF, strCRLF);
    MMI_FS_Read(fh, strOutput, VCARD_READ_BUFF_LEN - 1, &nBytes);

    strOutput[nBytes] = VOBJ_NULL;

    if (!nBytes)
    {
        return 0;
    }

    if ((currCursor = strstr(strOutput, strLF)) != NULL)  /* find out first LF */
    {
        if ((photo_end_cursor = strstr(strOutput, str_photo_end)) != NULL)  /* find out photo end */
        {
            if (currCursor > photo_end_cursor)
            {
                currCursor = photo_end_cursor + 6;
            }
        }
        strLen = (S32) (currCursor - strOutput);
        if (strOutput[strLen - 1] == VOBJ_CR_CHAR)
        {
            strOutput[strLen - 1] = VOBJ_NULL;
        }
        else
        {
            strOutput[strLen] = VOBJ_NULL;
        }


        MMI_FS_Seek(fh, (S32) (strLen - nBytes + 1), FS_FILE_CURRENT);
        nBytes = strLen;
    }
    else    /* CRLF not found, read until CRLF */
    {
        strOutput[nBytes] = VOBJ_NULL;
    }

    return nBytes;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_uti_get_line
 * DESCRIPTION
 *  Get a line from file system, excluding the escape characters for vcard.
 * PARAMETERS
 *  strOutput       [IN/OUT]        Storage location for data
 *  fh              [IN]            File handle
 * RETURNS
 *  void
 *****************************************************************************/
S32 mmi_vcard_uti_get_line(S8 *strOutput, FS_HANDLE fh)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    INT32 nBytes = 0;
#ifdef __MMI_PHB_INFO_FIELD__
    S8 *buffer = g_vcard_read_buff;
#else
    S8 *buffer = OslMalloc(VCARD_READ_BUFF_LEN);
#endif
    S32 i;
    S32 strLen;
    S8 *curr_cursor;
    S8 *cursor = NULL;
    U16 buffer_offset;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_FS_Read(fh, buffer, VCARD_READ_BUFF_LEN - 1, &nBytes);
    buffer[nBytes] = '\0';

    if (!nBytes)
    {
    #ifndef __MMI_PHB_INFO_FIELD__
        OslMfree(buffer);
    #endif /* __MMI_PHB_INFO_FIELD__ */
        return 0;
    }

    // only the VERSION and END are sure to be single line
    if ((strncmp(g_vcard_field[VCARD_FIELD_END], buffer, strlen(g_vcard_field[VCARD_FIELD_END])) == 0) ||
        (strncmp(g_vcard_field[VCARD_FIELD_VERSION], buffer, strlen(g_vcard_field[VCARD_FIELD_VERSION])) == 0))
    {
        MMI_FS_Seek(fh, 0 - nBytes, FS_FILE_CURRENT);
    #ifndef __MMI_PHB_INFO_FIELD__
        OslMfree(buffer);
    #endif /* __MMI_PHB_INFO_FIELD__ */
        return mmi_vcard_uti_get_single_line(strOutput, fh);
    }

    /* find next filed */
    for (i = VCARD_FIELD_FN; i < VCARD_FIELD_TOTAL; i++)
    {
        buffer_offset = 1;
        do
        {
            curr_cursor = strstr(buffer + buffer_offset, g_vcard_field[i]);
            if (curr_cursor != (S8*) NULL)
            {
                buffer_offset += (curr_cursor - (buffer + buffer_offset) + 1);//strlen(g_vcard_field[i]);
            }
        }while(curr_cursor != (S8*) NULL && *(curr_cursor - 1) != 0x0A);

        if (curr_cursor == (S8*) NULL)
        {
            continue;
        }
        else if (cursor == (S8*) NULL && (*(curr_cursor - 1) == 0x0A))
        {
            cursor = curr_cursor;
        }
        else if (((S32) cursor > (S32) curr_cursor) && (*(curr_cursor - 1) == 0x0A))
        {
            cursor = curr_cursor;
        }
    }
    if (cursor == NULL)
    {
        MMI_FS_Seek(fh, 0 - nBytes, FS_FILE_CURRENT);
    #ifndef __MMI_PHB_INFO_FIELD__
        OslMfree(buffer);
    #endif /* __MMI_PHB_INFO_FIELD__ */
        return mmi_vcard_uti_get_single_line(strOutput, fh);
        /* recursively to get single line to parse */
    }

    strLen = (S32) cursor - (S32) buffer;
    strncpy(strOutput, buffer, strLen);
    strOutput[strLen] = VOBJ_NULL;

    /* Remove end of line "\r\n" */
    if (strncmp((S8*) ((strOutput + strLen) - strlen(VOBJ_LF_STR)), (S8*) VOBJ_LF_STR, strlen(VOBJ_LF_STR)) ==
        0)
    {
        if (strOutput[strLen - strlen(VOBJ_LF_STR) - 1] == VOBJ_CR_CHAR)
        {
            strOutput[strLen - strlen(VOBJ_LF_STR) - 1] = VOBJ_NULL;
        }
        else
        {
            strOutput[strLen - strlen(VOBJ_LF_STR)] = VOBJ_NULL;
        }
    }

    MMI_FS_Seek(fh, (S32) (strLen - nBytes), FS_FILE_CURRENT);

#ifndef __MMI_PHB_INFO_FIELD__
    OslMfree(buffer);
#endif

    return strLen;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_parse_line
 * DESCRIPTION
 *  Read one line to parse for each filed.
 * PARAMETERS
 *  fh      [IN]        file handle ready to read and parse.
 * RETURNS
 *  parse  error cause
 *****************************************************************************/
U8 mmi_vcard_reader_parse_line(FS_HANDLE fh)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifdef __MMI_PHB_INFO_FIELD__
    S8 *inBuff = g_vcard_write_buff;
#else
    S8 inBuff[VCARD_WRITE_BUFF_LEN];
#endif
    U8 token;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* BEGIN:VCARD */
    if (mmi_vcard_uti_get_single_line(inBuff, fh) != 0)
    {
        if (mmi_vcard_reader_get_field(inBuff) != VCARD_FIELD_BEGIN)
        {
            return MMI_VCARD_PARSE_BEGIN_ERROR;
        }
        else if (!mmi_vcard_reader_check_header(inBuff))
        {
            return MMI_VCARD_PARSE_BEGIN_ERROR;
        }
    }
    else
    {
        return MMI_VCARD_PARSE_UNKNOW_ERROR;
    }

    #if defined(__MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__)
        g_vcard_email_counter = 0;
    #endif
    /* PARSE EACH FIELD */
    while (mmi_vcard_uti_get_line(inBuff, fh) != 0)
    {
        token = mmi_vcard_reader_get_field(inBuff);

        switch (token)
        {
            case VCARD_FIELD_END:
                if (!mmi_vcard_reader_check_header(inBuff))
                {
                    return MMI_VCARD_PARSE_UNKNOW_ERROR;
                }
                else
                {
                    return MMI_VCARD_PARSE_NO_ERROR;
                }
                break;                
                
            case VCARD_FIELD_FN:
            case VCARD_FIELD_REV:
            case VCARD_FIELD_PHOTO:
            case VCARD_FIELD_LABEL:
            case VCARD_FIELD_NICKNAME:
            case VCARD_FIELD_MAILER:
            case VCARD_FIELD_TZ:
            case VCARD_FIELD_GEO:
            case VCARD_FIELD_ROLE:
            case VCARD_FIELD_LOGO:
            case VCARD_FIELD_AGENT:
            case VCARD_FIELD_CATEGORIES:
            case VCARD_FIELD_PRODID:
            case VCARD_FIELD_SORT_STRING:
            case VCARD_FIELD_SOUND:
            case VCARD_FIELD_UID:
            case VCARD_FIELD_CLASS:
            case VCARD_FIELD_KEY:
            case VCARD_FIELD_X:
                break;

            /* VERSION */
            case VCARD_FIELD_VERSION:
                if (!mmi_vcard_reader_check_version(inBuff))
                {
                    return MMI_VCARD_PARSE_VERSION_ERROR;
                }
                break;

            case VCARD_FIELD_N:
                mmi_vcard_reader_retrive_name(inBuff);
                break;

            case VCARD_FIELD_TEL:
            #if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)           	
                mmi_vcard_reader_retrive_number(inBuff, pbNumber, pbHomeNumber, pbOfficeNumber, pbFaxNumber);
            #else
                mmi_vcard_reader_retrive_number(inBuff, pbNumber, NULL, NULL, NULL);            
            #endif
                break;

        #if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
            case VCARD_FIELD_EMAIL:
            #if defined(__MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__)
                if (g_vcard_email_counter > 0)
                {
                    mmi_vcard_reader_retrive_email(inBuff, pbEmailAddress2);
                    g_vcard_email_counter++;
                }
            #endif /* __MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__ */
                {
                    mmi_vcard_reader_retrive_email(inBuff, pbEmailAddress);
                    g_vcard_email_counter++;
                }
                break;

            case VCARD_FIELD_ORG:
                mmi_vcard_reader_retrive_org(inBuff, pbCompanyName);
                break;
        #endif /* !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) */

        #ifdef __MMI_PHB_BIRTHDAY_FIELD__
            case VCARD_FIELD_BDAY:
                mmi_vcard_reader_retrive_bday(inBuff, pbYear, pbMon, pbDay, TRUE);
                break;
        #endif /* __MMI_PHB_BIRTHDAY_FIELD__ */ 

        #ifdef __MMI_PHB_INFO_FIELD__
            case VCARD_FIELD_TITLE:
                mmi_vcard_reader_retrive_title(inBuff, pbTitle);
                break;
            case VCARD_FIELD_URL:
                mmi_vcard_reader_retrive_url(inBuff, pbUrl);
                break;
            case VCARD_FIELD_ADR:
                mmi_vcard_reader_retrive_address(inBuff, pbAdrPOBox, pbAdrExtension, pbAdrStreet, pbAdrCity, pbAdrState, pbAdrPostalCode, pbAdrCountry);
                break;
            case VCARD_FIELD_NOTE:
                mmi_vcard_reader_retrive_note(inBuff, pbNote);
                break;
        #endif /* __MMI_PHB_INFO_FIELD__ */ 

            default:
                break;
        }
    }
    return MMI_VCARD_PARSE_UNKNOW_ERROR;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_check_header
 * DESCRIPTION
 *  Check the VCARD header.
 * PARAMETERS
 *  String      [IN]        String read from file for one line
 * RETURNS
 *  parse error cause
 *****************************************************************************/
BOOL mmi_vcard_reader_check_header(S8 *String)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *cursor;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    cursor = (S8*)strstr(String, (S8*) VOBJ_VALUE_SEP_STR);
    if (!cursor || !mmi_vobj_uti_strnicmp((cursor + 1), VCARD_HEADER, strlen(VCARD_HEADER)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;    
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_check_version
 * DESCRIPTION
 *  Check the version field of a vcard object.
 * PARAMETERS
 *  String      [IN]        String read from file for one line
 * RETURNS
 *  parse error cause
 *****************************************************************************/
BOOL mmi_vcard_reader_check_version(S8 *String)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *cursor;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    cursor = mmi_vobj_uti_get_value(String);
    if (!mmi_vobj_uti_strnicmp(cursor, VCARD_VERSION_21, strlen(VCARD_VERSION_21)) ||
        !mmi_vobj_uti_strnicmp(cursor, VCARD_VERSION_30, strlen(VCARD_VERSION_30)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;    
    }
}


#if !defined(__MMI_VCARD_NAME_IN_ENGLISH_FORMAT__)
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_check_if_all_English
 * DESCRIPTION
 *  check if the field is all english.
 * PARAMETERS
 *  str      [IN]        String to be checked
 * RETURNS
 *  if all character is English or not
 *****************************************************************************/
static BOOL mmi_vcard_check_if_all_English(const char *str)
{
    BOOL ret = TRUE;
    while(*str)
    {
        if (*str & 0x80)
        {
            ret = FALSE;
            break;
        }
        str++;
    }
    return ret;
}
#endif /* __MMI_VCARD_NAME_IN_ENGLISH_FORMAT__ */


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_retrive_name
 * DESCRIPTION
 *  parse the name field of a vcard object.
 * PARAMETERS
 *  String      [IN]        String read from file for one line
 * RETURNS
 *  parse error cause
 *****************************************************************************/
U8 mmi_vcard_reader_retrive_name(S8 *String)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 temp_buf[MAX_PB_NAME_LENGTH * 3 + 4 + 1];    /* For UTF-8 Encoding Size and add 4 bytes more to store delimiters. */
    S8 *cursor;
    U8 value;
    S8 ucs2_name[(MAX_PB_NAME_LENGTH + 1) * ENCODING_LENGTH];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    cursor = mmi_vobj_uti_get_value(String);
    if (cursor != NULL)
    {
        memset(temp_buf, 0, sizeof(temp_buf));
        strncpy(temp_buf, cursor, (sizeof(temp_buf) - 1));

        /* Check if there is CRLF */
        if (mmi_vcard_reader_get_para_value(String, g_vobj_paras[VOBJ_PARA_ENCODING], VOBJ_PARA_ENCODING, &value) &&
            value == VOBJ_EN_QP)
        {
            /* mmi_vobj_util_eliminate_qp_CRLF(temp_buf); */
            memset(temp_buf, 0, sizeof(temp_buf));
            /* can only pass the dest buff length (total_len - 1); */
            applib_qprint_decode_basic(cursor, strlen(cursor), temp_buf, (MAX_PB_NAME_LENGTH * 3 + 4), QPOPT_DECODE_SKIP_LEADING_SPACE);
        }

    #ifdef __MMI_VCARD_NAME_IN_ENGLISH_FORMAT__
        mmi_vcard_util_get_name(temp_buf, sizeof(temp_buf));
    #else
        if (mmi_vcard_check_if_all_English((const char*)temp_buf))
        {
            if (!mmi_vcard_util_has_transform_name(temp_buf, sizeof(temp_buf)))
            {
                mmi_vcard_util_transform_field_delimiter(temp_buf, sizeof(temp_buf));    
            }
        }
        else
        {
            /* Remove delimiter. */
            mmi_vcard_util_remove_field_delimiter(temp_buf, sizeof(temp_buf));
        }
    #endif /* __MMI_VCARD_NAME_IN_ENGLISH_FORMAT__ */
        /* decode */
        if (mmi_vcard_reader_get_para_value(String, g_vobj_paras[VOBJ_PARA_CHARSET], VOBJ_PARA_CHARSET, &value))
        {
            mmi_vcard_util_convert_to_ucs2(ucs2_name, (MAX_PB_NAME_LENGTH + 1) * ENCODING_LENGTH, temp_buf, value);
        }
        else
        {
            mmi_chset_convert(
                g_vcard_cntx.CharSet,
                MMI_CHSET_UCS2,
                (char*)temp_buf,
                (char*)ucs2_name,
                (MAX_PB_NAME_LENGTH + 1) * ENCODING_LENGTH);
        }

        pfnUnicodeStrncpy(pbName, ucs2_name, MAX_PB_NAME_LENGTH);
        mmi_phb_name_remove_invalid_character(pbName);
        mmi_phb_truncate_pbname_buffer(MMI_NVRAM);
    }
    return MMI_VCARD_PARSE_NO_ERROR;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_retrive_number
 * DESCRIPTION
 *  parse number field of a vcard object.s
 * PARAMETERS
 *  String              [IN]        String read from file for one line
 *  number              [?]         
 *  home_number         [?]         
 *  office_number       [?]         
 *  fax_number          [?]         
 * RETURNS
 *  parse error cause
 *****************************************************************************/
U8 mmi_vcard_reader_retrive_number(S8 *String, S8 *number, S8 *home_number, S8 *office_number, S8 *fax_number)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* U8 result; */
    U8 type = VCARD_PARA_TOTAL;
    U8 pref;
    S8 temp_buf[MAX_PB_NUMBER_LENGTH + 1 + 1];
    S8 *cursor;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if defined(__MMI_VCARD_V30__)    
    mmi_vcard_util_v30_number_field_preprocess(String, (U16)strlen(String));
#endif /* defined(__MMI_VCARD_V30__) */

    cursor = mmi_vobj_uti_get_value(String);
    if (cursor != NULL && *cursor != '\0')
    {
        mmi_vcard_reader_get_para_number(String, &type, &pref);
        memset(temp_buf, 0, sizeof(temp_buf));
        strncpy(temp_buf, cursor, MAX_PB_NUMBER_LENGTH + 1);

        switch (type)
        {
            case VCARD_PARA_HOME:
                if (home_number && !pfnUnicodeStrlen(home_number))
                {
                    if (temp_buf[0] == '+')
                        AnsiiNToUnicodeString(home_number, (S8*) temp_buf, MAX_PB_NUMBER_LENGTH + 1);
                    else
                        AnsiiNToUnicodeString(home_number, (S8*) temp_buf, MAX_PB_NUMBER_LENGTH);
                }
                break;
            case VCARD_PARA_WORK:
                if (office_number && !pfnUnicodeStrlen(office_number))
                {
                    if (temp_buf[0] == '+')
                        AnsiiNToUnicodeString(office_number, (S8*) temp_buf, MAX_PB_NUMBER_LENGTH + 1);
                    else
                        AnsiiNToUnicodeString(office_number, (S8*) temp_buf, MAX_PB_NUMBER_LENGTH);
                }
                break;
            case VCARD_PARA_FAX:
                if (fax_number && !pfnUnicodeStrlen(fax_number))
                {
                    if (temp_buf[0] == '+')
                        AnsiiNToUnicodeString(fax_number, (S8*) temp_buf, MAX_PB_NUMBER_LENGTH + 1);
                    else
                        AnsiiNToUnicodeString(fax_number, (S8*) temp_buf, MAX_PB_NUMBER_LENGTH);
                }
                break;
            case VCARD_PARA_PAGER:
                break; /* Don't have pager field */
        #ifdef __MMI_VCARD_TEL_VOICE_CELL_DIFFERENT__
            case VCARD_PARA_CELL:
                if (number && !pfnUnicodeStrlen(number))
                {
                    if (temp_buf[0] == '+')
                        AnsiiNToUnicodeString(number, (S8*) temp_buf, MAX_PB_NUMBER_LENGTH + 1);
                    else
                        AnsiiNToUnicodeString(number, (S8*) temp_buf, MAX_PB_NUMBER_LENGTH);
                }
                break;
            default:
                break;
        #else /* __MMI_VCARD_TEL_VOICE_CELL_DIFFERENT__ */
            /* demo project not define the compile option */
            case VCARD_PARA_CELL:
            default:
                if (number && !pfnUnicodeStrlen(number))
                {
                    if (temp_buf[0] == '+')
                        AnsiiToUnicodeString(number, (S8*) temp_buf);
                    else
                        AnsiiToUnicodeString(number, (S8*) temp_buf);
                }
                break;
        #endif /* __MMI_VCARD_TEL_VOICE_CELL_DIFFERENT__ */
        }
    }
    return MMI_VCARD_PARSE_NO_ERROR;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_retrive_email
 * DESCRIPTION
 *  parse email field of a vcard object.s
 * PARAMETERS
 *  String              [IN]        String read from file for one line
 *  email_address       [?]         
 * RETURNS
 *  parse error cause
 *****************************************************************************/
U8 mmi_vcard_reader_retrive_email(S8 *String, S8 *email_address)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (email_address && !pfnUnicodeStrlen(email_address))
    {
        S8 temp_buf[MAX_PB_EMAIL_LENGTH + 1];
        S8 *cursor;
        U8 value;

        cursor = mmi_vobj_uti_get_value(String);
        if (cursor != NULL)
        {
            memset(temp_buf, 0, sizeof(temp_buf));
            strncpy(temp_buf, cursor, MAX_PB_EMAIL_LENGTH);

            /* Check if there is CRLF */
            if (mmi_vcard_reader_get_para_value(String, g_vobj_paras[VOBJ_PARA_ENCODING], VOBJ_PARA_ENCODING, &value) &&
                value == VOBJ_EN_QP)
            {
                memset(temp_buf, 0, sizeof(temp_buf));
                /* can only pass the dest buff length (total_len - 1); */
                applib_qprint_decode_basic(cursor, strlen(cursor), temp_buf, MAX_PB_EMAIL_LENGTH, QPOPT_DECODE_SKIP_LEADING_SPACE);
            }
            mmi_vcard_util_remove_field_delimiter(temp_buf, sizeof(temp_buf));

            /* decode */
            if (mmi_vcard_reader_get_para_value(String, g_vobj_paras[VOBJ_PARA_CHARSET], VOBJ_PARA_CHARSET, &value))
            {
                mmi_vcard_util_convert_to_ucs2(email_address, (MAX_PB_EMAIL_LENGTH + 1) * ENCODING_LENGTH, temp_buf, value);
            }
            else
            {
                mmi_chset_convert(
                    g_vcard_cntx.CharSet,
                    MMI_CHSET_UCS2,
                    (char*)temp_buf,
                    (char*)email_address,
                    (MAX_PB_EMAIL_LENGTH + 1) * ENCODING_LENGTH);
            }
        }
    }
    return MMI_VCARD_PARSE_NO_ERROR;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_retrive_org
 * DESCRIPTION
 *  parse org field of a vcard object.s
 * PARAMETERS
 *  String              [IN]        String read from file for one line
 *  company_name        [?]         
 * RETURNS
 *  parse error cause
 *****************************************************************************/
U8 mmi_vcard_reader_retrive_org(S8 *String, S8 *company_name)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *temp_buf = OslMalloc(VCARD_WRITER_FIELD_BUFFER);    /* For UTF-8 Encoding Size and add 4 bytes more to store delimiters. */
    S8 *cursor;
    U8 value;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    cursor = mmi_vobj_uti_get_value(String);
    if (cursor != NULL)
    {
        memset(temp_buf, 0, VCARD_WRITER_FIELD_BUFFER);
        strncpy(temp_buf, cursor, (VCARD_WRITER_FIELD_BUFFER - 1));

        /* Check if there is CRLF */
        if (mmi_vcard_reader_get_para_value(String, g_vobj_paras[VOBJ_PARA_ENCODING], VOBJ_PARA_ENCODING, &value) &&
            value == VOBJ_EN_QP)
        {
            memset(temp_buf, 0, VCARD_WRITER_FIELD_BUFFER);
            /* can only pass the dest buff length (total_len - 1); */
            applib_qprint_decode_basic(cursor, strlen(cursor), temp_buf, VCARD_WRITER_FIELD_BUFFER - 1, QPOPT_DECODE_SKIP_LEADING_SPACE);
        }   

        /* Remove delimiter. */
        mmi_vcard_util_remove_field_delimiter(temp_buf, VCARD_WRITER_FIELD_BUFFER);
    
        /* decode */
        if (mmi_vcard_reader_get_para_value(String, g_vobj_paras[VOBJ_PARA_CHARSET], VOBJ_PARA_CHARSET, &value))
        {
            mmi_vcard_util_convert_to_ucs2(company_name, (MAX_PB_COMPANY_LENGTH + 1) * ENCODING_LENGTH, temp_buf, value);
        }
        else
        {
            mmi_chset_convert(
                g_vcard_cntx.CharSet,
                MMI_CHSET_UCS2,
                (char*)temp_buf,
                (char*)company_name,
                (MAX_PB_COMPANY_LENGTH + 1) * ENCODING_LENGTH);
        }
    }
    OslMfree(temp_buf);
    return MMI_VCARD_PARSE_NO_ERROR;
}


#if defined(__MMI_PHB_INFO_FIELD__)
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_retrive_title
 * DESCRIPTION
 *  parse title field of a vcard object.s
 * PARAMETERS
 *  String              [IN]        String read from file for one line
 *  company_name        [?]         
 * RETURNS
 *  parse error cause
 *****************************************************************************/
U8 mmi_vcard_reader_retrive_title(S8 *String, S8 *title)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *temp_buf = OslMalloc(VCARD_WRITER_FIELD_BUFFER);    /* For UTF-8 Encoding Size and add 4 bytes more to store delimiters. */
    S8 *cursor;
    U8 value;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    cursor = mmi_vobj_uti_get_value(String);
    if (cursor != NULL)
    {
        memset(temp_buf, 0, VCARD_WRITER_FIELD_BUFFER);
        strncpy(temp_buf, cursor, (VCARD_WRITER_FIELD_BUFFER - 1));

        /* Check if there is CRLF */
        if (mmi_vcard_reader_get_para_value(String, g_vobj_paras[VOBJ_PARA_ENCODING], VOBJ_PARA_ENCODING, &value) &&
            value == VOBJ_EN_QP)
        {
            memset(temp_buf, 0, VCARD_WRITER_FIELD_BUFFER);
            /* can only pass the dest buff length (total_len - 1); */
            applib_qprint_decode_basic(cursor, strlen(cursor), temp_buf, VCARD_WRITER_FIELD_BUFFER - 1, QPOPT_DECODE_SKIP_LEADING_SPACE);
        }

        /* Remove delimiter. */
        mmi_vcard_util_remove_field_delimiter(temp_buf, VCARD_WRITER_FIELD_BUFFER);

        /* decode */
        if (mmi_vcard_reader_get_para_value(String, g_vobj_paras[VOBJ_PARA_CHARSET], VOBJ_PARA_CHARSET, &value))
        {
            mmi_vcard_util_convert_to_ucs2(title, (MAX_PB_TITLE_LENGTH + 1) * ENCODING_LENGTH, temp_buf, value);
        }
        else
        {
            mmi_chset_convert(
                g_vcard_cntx.CharSet,
                MMI_CHSET_UCS2,
                (char*)temp_buf,
                (char*)title,
                (MAX_PB_TITLE_LENGTH + 1) * ENCODING_LENGTH);
        }
    }
    OslMfree(temp_buf);
    return MMI_VCARD_PARSE_NO_ERROR;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_retrive_url
 * DESCRIPTION
 *  parse url field of a vcard object.s
 * PARAMETERS
 *  String              [IN]        String read from file for one line
 *  email_address       [?]         
 * RETURNS
 *  parse error cause
 *****************************************************************************/
U8 mmi_vcard_reader_retrive_url(S8 *String, S8 *url)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (url && !pfnUnicodeStrlen(url))
    {
        S8 temp_buf[MAX_PB_URL_LENGTH * 3 + 4 + 1];
        S8 *cursor;
        U8 value;

        cursor = mmi_vobj_uti_get_value(String);
        if (cursor != NULL)
        {
            memset(temp_buf, 0, sizeof(temp_buf));
            strncpy(temp_buf, cursor, MAX_PB_URL_LENGTH);

            /* Check if there is CRLF */
            if (mmi_vcard_reader_get_para_value(String, g_vobj_paras[VOBJ_PARA_ENCODING], VOBJ_PARA_ENCODING, &value) &&
                value == VOBJ_EN_QP)
            {
                memset(temp_buf, 0, sizeof(temp_buf));
                /* can only pass the dest buff length (total_len - 1); */
                applib_qprint_decode_basic(cursor, strlen(cursor), temp_buf, MAX_PB_URL_LENGTH * 3 + 4, QPOPT_DECODE_SKIP_LEADING_SPACE);
            }
            mmi_vcard_util_remove_field_delimiter(temp_buf, sizeof(temp_buf));

            /* decode */
            if (mmi_vcard_reader_get_para_value(String, g_vobj_paras[VOBJ_PARA_CHARSET], VOBJ_PARA_CHARSET, &value))
            {
                mmi_vcard_util_convert_to_ucs2(url, (U8)((MAX_PB_URL_LENGTH + 1) * ENCODING_LENGTH), temp_buf, value);
            }
            else
            {
                mmi_chset_convert(
                    g_vcard_cntx.CharSet,
                    MMI_CHSET_UCS2,
                    (char*)temp_buf,
                    (char*)url,
                    (MAX_PB_URL_LENGTH + 1) * ENCODING_LENGTH);
            }
        }
    }
    return MMI_VCARD_PARSE_NO_ERROR;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_retrive_address
 * DESCRIPTION
 *  parse title field of a vcard object.s
 * PARAMETERS
 *  String              [IN]        String read from file for one line
 *  company_name        [?]         
 * RETURNS
 *  parse error cause
 *****************************************************************************/
U8 mmi_vcard_reader_retrive_address(S8 *String, S8 *pobox, S8 *extension, S8 *street, S8 *city, S8 *state, S8 *postalcode, S8 *country)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *temp_buf = OslMalloc(VCARD_WRITER_FIELD_BUFFER);    /* For UTF-8 Encoding Size and add 4 bytes more to store delimiters. */
    S8 *temp_value_buf = OslMalloc(VCARD_WRITER_FIELD_BUFFER);
    S8 *cursor;
    S8 *adr_value_buf = NULL;
    U8 value;
    U16 p1, p2;
    MMI_BOOL is_qp_encode = MMI_FALSE;
    MMI_BOOL get_para_value = MMI_FALSE;
    U8 num_of_semicolon = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    cursor = mmi_vobj_uti_get_value(String);
    /* Check if there is CRLF */
    if (mmi_vcard_reader_get_para_value(String, g_vobj_paras[VOBJ_PARA_ENCODING], VOBJ_PARA_ENCODING, &value) &&
        value == VOBJ_EN_QP)
    {
        is_qp_encode = MMI_TRUE;
    }
    /* decode */
    if (mmi_vcard_reader_get_para_value(String, g_vobj_paras[VOBJ_PARA_CHARSET], VOBJ_PARA_CHARSET, &value))
    {
        get_para_value = MMI_TRUE;
    }

    if (cursor != NULL)
    {
        p1 = 0;
        while(*(cursor + p1) != '\0')
        {
            switch (num_of_semicolon)
            {
                case MMI_PHB_ADDRESS_POBOX:
                    adr_value_buf = pobox;
                    break;

                case MMI_PHB_ADDRESS_EXTENSION:
                    adr_value_buf = extension;
                    break;

                case MMI_PHB_ADDRESS_STREET:
                    adr_value_buf = street;
                    break;

                case MMI_PHB_ADDRESS_CITY:
                    adr_value_buf = city;
                    break;

                case MMI_PHB_ADDRESS_STATE:
                    adr_value_buf = state;
                    break;

                case MMI_PHB_ADDRESS_POSTALCODE:
                    adr_value_buf = postalcode;
                    break;

                case MMI_PHB_ADDRESS_COUNTRY:
                    adr_value_buf = country;
                    break;

                default:
                    break;                                                                            
            }

            if (adr_value_buf == NULL)
            {
                break;
            }
            memset(temp_buf, 0, VCARD_WRITER_FIELD_BUFFER);
            memset(temp_value_buf, 0, VCARD_WRITER_FIELD_BUFFER);
            p2 = 0;
            for (; ((*(cursor + p1) != ';') && (*(cursor + p1) != '\0')) ; p1++)
            {
                if ((*(cursor + p1) == '\\') && (*(cursor + p1 + 1) == ';'))
                {
                    p1++;
                }
                *(temp_buf + p2) = *(cursor + p1);
                if ((++p2) == (VCARD_WRITER_FIELD_BUFFER - 1))
                {
                    break;
                }
            }
            *(temp_buf + p2) = '\0';
            if (*(cursor + p1) == ';')
            {
                num_of_semicolon ++;
                p1++;
            }
            if (is_qp_encode == MMI_TRUE)
            {
                /* can only pass the dest buff length (total_len - 1); */
                applib_qprint_decode_basic(temp_buf, strlen(temp_buf), temp_value_buf, VCARD_WRITER_FIELD_BUFFER - 1, QPOPT_DECODE_SKIP_LEADING_SPACE);
            }
            else
            {
                memcpy(temp_value_buf, temp_buf, (VCARD_WRITER_FIELD_BUFFER - 1));
            }

            mmi_vcard_util_remove_field_delimiter(temp_value_buf, sizeof(temp_value_buf));
            /* decode */
            if (get_para_value == MMI_TRUE)
            {
                mmi_vcard_util_convert_to_ucs2(adr_value_buf, (MAX_PB_ADDRESS_LENGTH + 1) * ENCODING_LENGTH, temp_value_buf, value);
            }
            else
            {
                mmi_chset_convert(
                    g_vcard_cntx.CharSet,
                    MMI_CHSET_UCS2,
                    (char*)temp_value_buf,
                    (char*)adr_value_buf,
                    (MAX_PB_ADDRESS_LENGTH + 1) * ENCODING_LENGTH);
            }            
        }
    }
    OslMfree(temp_buf);
    OslMfree(temp_value_buf);
    return MMI_VCARD_PARSE_NO_ERROR;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_retrive_note
 * DESCRIPTION
 *  parse note field of a vcard object.s
 * PARAMETERS
 *  String              [IN]        String read from file for one line
 *  note        [?]         
 * RETURNS
 *  parse error cause
 *****************************************************************************/
U8 mmi_vcard_reader_retrive_note(S8 *String, S8 *note)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *temp_buf = OslMalloc(VCARD_WRITER_FIELD_BUFFER);    /* For UTF-8 Encoding Size and add 4 bytes more to store delimiters. */
    S8 *cursor;
    U8 value;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    cursor = mmi_vobj_uti_get_value(String);
    if (cursor != NULL)
    {
        memset(temp_buf, 0, VCARD_WRITER_FIELD_BUFFER);
        strncpy(temp_buf, cursor, (VCARD_WRITER_FIELD_BUFFER - 1));

        /* Check if there is CRLF */
        if (mmi_vcard_reader_get_para_value(String, g_vobj_paras[VOBJ_PARA_ENCODING], VOBJ_PARA_ENCODING, &value) &&
            value == VOBJ_EN_QP)
        {
            memset(temp_buf, 0, VCARD_WRITER_FIELD_BUFFER);
            applib_qprint_decode_basic(cursor, strlen(cursor), temp_buf, VCARD_WRITER_FIELD_BUFFER - 1, QPOPT_DECODE_SKIP_LEADING_SPACE);
        }

        /* Remove delimiter. */
        mmi_vcard_util_remove_field_delimiter(temp_buf, VCARD_WRITER_FIELD_BUFFER);

        /* decode */
        if (mmi_vcard_reader_get_para_value(String, g_vobj_paras[VOBJ_PARA_CHARSET], VOBJ_PARA_CHARSET, &value))
        {
            mmi_vcard_util_convert_to_ucs2(note, (MAX_PB_NOTE_LENGTH + 1) * ENCODING_LENGTH, temp_buf, value);
        }
        else
        {
            mmi_chset_convert(
                g_vcard_cntx.CharSet,
                MMI_CHSET_UCS2,
                (char*)temp_buf,
                (char*)note,
                (MAX_PB_NOTE_LENGTH + 1) * ENCODING_LENGTH);
        }
    }
    OslMfree(temp_buf);
    return MMI_VCARD_PARSE_NO_ERROR;
}
#endif /* defined(__MMI_PHB_INFO_FIELD__) */


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_get_field
 * DESCRIPTION
 *  parse field name of a vcard string.
 * PARAMETERS
 *  String      [IN]        String read from file for one line
 * RETURNS
 *  field enum
 *****************************************************************************/
U8 mmi_vcard_reader_get_field(S8 *String)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < VCARD_FIELD_TOTAL; i++)
    {
        if (mmi_vobj_uti_cmp_filed(String, g_vcard_field[i]))
        {
            break;
        }
    }
    /* Add check for tag "N" here. Because the above check will confuse tag "N" and "NOTE" */
    if ((i == VCARD_FIELD_N) && !((String[1] == ':') || (String[1] == ';')))
    {
    #if defined(__MMI_PHB_INFO_FIELD__)
        return VCARD_FIELD_NOTE;
    #else /* defined(__MMI_PHB_INFO_FIELD__) */
        return VCARD_FIELD_TOTAL;
    #endif /* defined(__MMI_PHB_INFO_FIELD__) */
    }

    return i;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_get_para_value
 * DESCRIPTION
 *  read the value of a vcard parameter
 * PARAMETERS
 *  String          [IN]        String read from file for one line
 *  para            [IN]        Parameter for getting value
 *  para_enum       [IN]        
 *  value           [OUT]       Value of parameter
 * RETURNS
 *  Success or Fail
 *****************************************************************************/
static U8 mmi_vcard_reader_get_para_value(S8 *String, const S8 *para, U8 para_enum, U8 *value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *para_pos;
    U16 i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (strstr(String, ":") == NULL)
    {
        return FALSE;
    }

    para_pos = strstr(String, para);

    if (para_pos != NULL)
    {
        para_pos = strstr(para_pos, "=") + 1;
        if (para_pos != NULL)
        {

            if (para_enum == VOBJ_PARA_CHARSET)
            {
                for (i = 0; i < VOBJ_CH_TOTAL; i++)
                {
                    if (mmi_vobj_uti_cmp_filed(para_pos, g_vobj_charset[i]))
                    {
                        break;
                    }
                }
                *value = (U8)i;
                return TRUE;
            }
            else if (para_enum == VOBJ_PARA_ENCODING)
            {
                for (i = 0; i < VOBJ_EN_TOTAL; i++)
                {
                    if (mmi_vobj_uti_cmp_filed(para_pos, g_vobj_encoding[i]))
                    {
                        break;
                    }
                }
                *value = (U8)i;
                return TRUE;
            }
        }
    }

    return FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_reader_get_para_number
 * DESCRIPTION
 *  parse number field. there are several kinds of number field in vcard.
 * PARAMETERS
 *  String      [IN]        String read from file for one line
 *  type        [OUT]       Number type, voice or fax
 *  pref        [OUT]       Is this field a prefer number
 * RETURNS
 *  number parameter enum
 *****************************************************************************/
static U8 mmi_vcard_reader_get_para_number(S8 *String, U8 *type, U8 *pref)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *sep_pos;
    U16 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (strstr(String, ":") == NULL)
    {
        return FALSE;
    }

    *pref = FALSE;
    sep_pos = String;
    while ((sep_pos = strstr(sep_pos, ";")) != NULL)
    {
        sep_pos++;
        for (i = 0; i < VCARD_PARA_TOTAL; i++)
            if (mmi_vobj_uti_cmp_filed(sep_pos, g_vcard_para[i]))
            {
                break;
            }

        switch (i)
        {
            case VCARD_PARA_CELL:
                *type = VCARD_PARA_CELL;
                break;
            case VCARD_PARA_HOME:
                *type = VCARD_PARA_HOME;
                break;
            case VCARD_PARA_WORK:
                *type = VCARD_PARA_WORK;
                break;
            case VCARD_PARA_FAX:
                *type = VCARD_PARA_FAX;
                break;
            case VCARD_PARA_PAGER:
                *type = VCARD_PARA_PAGER;
            case VCARD_PARA_PREF:
                *pref = TRUE;
                break;
            default:
                break;
        }
        if ((i == VCARD_PARA_CELL) && ((g_vcard_number & 0x01) != 0x01))
        {
            g_vcard_number |= 0x01;
            break;
        }
        else if ((i == VCARD_PARA_HOME) && ((g_vcard_number & 0x02) != 0x02))
        {
            g_vcard_number |= 0x02;
            break;
        }
        else if ((i == VCARD_PARA_WORK) && ((g_vcard_number & 0x04) != 0x04))
        {
            g_vcard_number |= 0x04;
            break;
        }
        else if ((i == VCARD_PARA_FAX) && ((g_vcard_number & 0x08) != 0x08))
        {
            g_vcard_number |= 0x08;
            break;
        }
        else if (i == VCARD_PARA_PAGER)
        {
            break;
        }
    }
    return TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_util_remove_field_delimiter
 * DESCRIPTION
 *  remove field delimiter in a string.
 * PARAMETERS
 *  str         [IN]        String to remove
 *  length      [IN]        Maximum length to parse
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_util_remove_field_delimiter(S8 *str, U16 length)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 i, j;
    BOOL has_delimiter = FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    i = 0;
    while (i < length)
    {
        if (str[i] == '\0') /* End of string */
        {
/*            if (str[0] == '\0' && has_delimiter)
            {
                str[0] = ';';
                str[1] = '\0';
            }*/
            return;
        }

        if (str[i] == ';')  /* Remove ';' and shift string forward. */
        {
            has_delimiter = TRUE;
            for (j = i; j < length && str[j] != '\0'; j++)
            {
                str[j] = str[j + 1];
            }
            str[j] = '\0';
            continue;
        }
        else if (str[i] == '\\' && (str[i+1] == ';' || str[i+1] == '\\'))  /* Remove '\' and shift string forward. */
        {
            for (j = i; j < length && str[j] != '\0'; j++)
            {
                str[j] = str[j + 1];
            }
            str[j] = '\0';
        }
        i++;
    }
}


#ifdef __MMI_VCARD_NAME_IN_ENGLISH_FORMAT__
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_util_get_name
 * DESCRIPTION
 *  Transform name in "First Name" "Last Name".
 * PARAMETERS
 *  str         [IN]        String to check
 *  length      [IN]        Maximum length to parse
 * RETURNS
 *  if character already transfrom or not
 *****************************************************************************/
static void mmi_vcard_util_get_name(S8 *str, U16 length)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8* last_name = OslMalloc(length + 1);
    S8* first_name = OslMalloc(length + 1);
    U16 i = 0, j = 0, num_of_semicolon = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    memset(last_name, 0, 1);
    memset(first_name, 0, 1);
 
    while (i < length)
    {
    	if (str[i] == '\0')
        {
            if (num_of_semicolon == 0)
            {
                last_name[j] = '\0';
            }
            else if (num_of_semicolon == 1)
            {
                first_name[j] = '\0';
            }
            break;
        }
    	else if (str[i] == ';' && str[i - 1] != '\\')
        {
            num_of_semicolon ++;
            if (num_of_semicolon == 1)
            {
                last_name[j] = '\0';
                j = 0;                    
            }
            else if (num_of_semicolon == 2)
            {
                first_name[j] = '\0';
                break;
            }
        }
        else
        {
            if (!(str[i] == '\\' && (str[i + 1] == ';' || str[i + 1] == ',' || str[i + 1] == '\\')))
            {
                if (num_of_semicolon == 0)
                {
                    last_name[j++] = str[i]; 
                }
                else if (num_of_semicolon == 1)
                {
                    first_name[j++] = str[i];
                }
            }
        }
        i++;
    }

    if (strlen(first_name))
    {
        strncpy(str, first_name, strlen(first_name));
		str[strlen(first_name)] = '\0';
        strcat(str, " ");
        strcat(str, last_name);
		str[strlen(first_name) + strlen(last_name) + 1] = '\0';        
    }
    else
    {
        strncpy(str, last_name, strlen(last_name));
		str[strlen(last_name)] = '\0';        
    }
    OslMfree(first_name);
    OslMfree(last_name);
}
#else /* __MMI_VCARD_NAME_IN_ENGLISH_FORMAT__ */


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_util_need_transform_name
 * DESCRIPTION
 *  Transform field delimiter in a string.
 * PARAMETERS
 *  str         [IN]        String to check
 *  length      [IN]        Maximum length to parse
 * RETURNS
 *  if character already transfrom or not
 *****************************************************************************/
static BOOL mmi_vcard_util_has_transform_name(S8 *str, U16 length)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 i, j;
    BOOL has_semicolon = FALSE;
    U8 first_character_count = 0;
    U8 last_character_count = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    i = 0;
    if (str[i] == ';')
    {
        return FALSE;
    }
    while (i < length - 1)
    {
        if (str[i] == '\0') /* End of string */
        {
            if (first_character_count > 0 && last_character_count > 0 && has_semicolon)
            {
                /* transform the first & last name */
                S8* family_name = OslMalloc(first_character_count + 1);
                memcpy(family_name, str, first_character_count);
                i = 0;
                //j = last_character_count;
                for (j = last_character_count; j > 0; j--)
                {
                    str[i] = str[first_character_count + 1 + i];
                    i++;
                }
                str[last_character_count] = ' ';
                memcpy(str + last_character_count + 1, family_name, first_character_count);
                OslMfree(family_name);
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        if (str[i] == '\\' && str[i + 1] == ';')  /* "\;" */
        {
            return FALSE;
        }
        else if (str[i] == ';' && str[i+1] != '\0')
        {
            if (has_semicolon)
            {
                return FALSE;
            }
            else
            {
                has_semicolon = TRUE;
            }
        }
        else if (has_semicolon)
        {
            last_character_count++;
        }
        else
        {
            first_character_count++;
        }
        i++;
    }
    return FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_util_transform_field_delimiter
 * DESCRIPTION
 *  Transform field delimiter in a string.
 * PARAMETERS
 *  str         [IN]        String to transform
 *  length      [IN]        Maximum length to parse
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_util_transform_field_delimiter(S8 *str, U16 length)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 i, j;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    i = 0;
    while (i < length - 1)
    {
        if (str[i] == '\0') /* End of string */
        {
            /* 1. More than one character, then remove ";" (if not "\;") */
            if (i > 1 && str[i-1] == ';' && str[i-2] != '\\')
            {
                str[i-1] = '\0';
            }
            break;
        }
        if (str[i] == ';' && str[i + 1] != '\0')
        {
            if (i == 0 || str[i+1] == ';')  /* Remove ';' and shift string forward. */
            {
                for (j = i; j < length && str[j] != '\0'; j++)
                {
                    str[j] = str[j + 1];
                }
                str[j] = '\0';
                continue;
            }
        }
        i++;
    }
    
    i = 0;
    while (i < length - 1)
    {
        if (str[i] == '\0') /* End of string */
        {
            return;
        }
        if (str[i] == '\\' && (str[i + 1] == ';' || str[i + 1] == '\\'))  /* Remove '\' and shift string forward. */
        {
            for (j = i; j < length && str[j] != '\0'; j++)
            {
                str[j] = str[j + 1];
            }
            str[j] = '\0';
            if (str[i + 2] == '\0')
            {
                break;
            }
        }
        else if (str[i] == ';' && str[i+1] != '\0')
        {
            /* change ';' to ', ' and shift string backward. */
            {
                str[i] = ',';
                for (j = length - 2; j > (i + 1); j--)
                {
                    str[j] = str[j - 1];
                }
                str[i+1] = ' ';
            }
            continue;
        }
        i++;
    }
}
#endif /* __MMI_VCARD_NAME_IN_ENGLISH_FORMAT__ */


#if defined(__MMI_VCARD_V30__)
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_util_v30_number_field_preprocess
 * DESCRIPTION
 *  Transform field delimiter in a string.
 * PARAMETERS
 *  str         [IN]        String to transform
 *  length      [IN]        Maximum length to parse
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_util_v30_number_field_preprocess(S8 *str, U16 length)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 i, j;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    i = 0;
    while (i < length) 
    {
        if (str[i] == ':') /* End of type */
            return;

        if ((str[i] == 'T' || str[i] == 't') && 
            (str[i + 1] == 'Y' || str[i + 1] == 'y') && 
            (str[i + 2] == 'P' || str[i + 2] == 'p') && 
            (str[i + 3] == 'E' || str[i + 3] == 'e') && 
            str[i + 4] == '=')
        {
            for (j = i; j < length && str[j] != '\0'; j++)
            {
                str[j] = str[j + 5];
            }
            str[j] = '\0';
        }
        else if (str[i] == ',')
        {
            str[i] = ';';
        }
        i++;
    }
}
#endif /* defined(__MMI_VCARD_V30__) */
/*--------------------------------------------------END vcard parser ---------------------------------------------------*/


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_util_get_default_file
 * DESCRIPTION
 *  Create default file path for send operation.
 * PARAMETERS
 *  path        [IN/OUT]        Path buffer
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_util_get_default_file(S8 *path)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *ascii_path;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ascii_path = OslMalloc(VOBJ_MAX_PATH_LENGTH / ENCODING_LENGTH);
    sprintf(ascii_path,"%c:/%s/%s",VOBJ_DRV,VOBJ_FILE_PATH,VOBJ_VCARD_DEFAULT_FILE_NAME);
    mmi_trace(g_sw_PHB, "VCARD func:%s, ascii_path=%s", __FUNCTION__, ascii_path);
    AnsiiToUnicodeString(path, ascii_path);
    OslMfree(ascii_path);
}

#if /*defined(__JATAAYU_APP__) || */defined (__MMI_EMAIL__) || defined (__MMI_IMPS__)
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_util_get_default_file_name
 * DESCRIPTION
 *  Create default file path for send operation.
 * PARAMETERS
 *  path        [IN/OUT]        Path buffer
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_util_get_default_file_name(S8 *path)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *ascii_path;
    S8 *file_name;
    S8 filename[VOBJ_MAX_FILE_EXT_LEN];
    MMI_PHB_VCARD_STRUCT myCard;
    S16 pError;
    U16 file_name_length;
    U16 path_length;
    S8* temp_filename;
    S8* temp_ptr;
    U16 temp_filename_length = (MAX_PB_NAME_LENGTH + 1) * ENCODING_LENGTH;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ascii_path = OslMalloc(VOBJ_MAX_PATH_LENGTH / ENCODING_LENGTH);
    sprintf(ascii_path, "%c:\\%s\\", VOBJ_DRV, VOBJ_FILE_PATH);
    AnsiiToUnicodeString(path, ascii_path);    
    path_length = pfnUnicodeStrlen(path);
    OslMfree(ascii_path);

    file_name = OslMalloc(VOBJ_MAX_FILENAME_LENGTH);

    temp_filename = OslMalloc(temp_filename_length);
    if (g_vcard_cntx.CardType == MMI_VCARD_SEND_MY_CARD)
    {
        ReadRecord(NVRAM_EF_PHB_VCARD_LID, 1, (void*)&myCard, MMI_PHB_VCARD_RECORD_SIZE, &pError);
        memcpy(temp_filename, myCard.name, temp_filename_length);
    }
    else if (g_vcard_cntx.CardType == MMI_VCARD_SEND_SELECT_CARD_SECOND_LEVEL)
    {
        memcpy(
            temp_filename,
            PhoneBook[g_phb_cntx.active_index_second].alpha_id.name,
            temp_filename_length);
    }
    else
    {
        memcpy(
            temp_filename,
            PhoneBook[g_phb_name_index[g_phb_cntx.active_index]].alpha_id.name,
            temp_filename_length);
    }
    temp_ptr = mmi_fmgr_skip_space(temp_filename);
    pfnUnicodeStrncpy(file_name, temp_ptr, (VOBJ_MAX_FILENAME_LENGTH / ENCODING_LENGTH - 1));
    file_name_length = pfnUnicodeStrlen(file_name);
    OslMfree(temp_filename);

    if (!mmi_fmgr_is_filename_valid(file_name))
    {
        AnsiiToUnicodeString(file_name, "~vcard");
        pfnUnicodeStrcat(path, file_name);
    }
    else
    {
        if ((path_length + file_name_length + VOBJ_MAX_FILE_EXT_LEN / ENCODING_LENGTH) > VOBJ_MAX_PATH_LENGTH / ENCODING_LENGTH)
        {
            pfnUnicodeStrncat(path, file_name, (VOBJ_MAX_PATH_LENGTH / ENCODING_LENGTH - path_length - VOBJ_MAX_FILE_EXT_LEN / ENCODING_LENGTH));
        }
        else
        {
            pfnUnicodeStrcat(path, file_name);
        }
    }
    OslMfree(file_name);
    AnsiiToUnicodeString(filename, VOBJ_VCARD_FILE_EXT);
    pfnUnicodeStrcat(path, filename);
}
#endif
#if defined(__MMI_OPP_SUPPORT__)

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_util_del_default_file
 * DESCRIPTION
 *  delete default file path after sending.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_util_del_default_file(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *path;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    path = OslMalloc(VOBJ_MAX_PATH_LENGTH);
    mmi_vcard_util_get_default_file(path);
    MMI_FS_Delete((U8*)path);
    OslMfree(path);
}
#endif

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_util_convert_to_system_encoding
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dst     [?]     
 *  src     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
static U8 mmi_vcard_util_convert_to_system_encoding(S8 *dst, S8 *src)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_chset_convert(MMI_CHSET_UCS2, MMI_CHSET_UTF8, (char*)src, (char*)dst, VCARD_WRITER_FIELD_BUFFER);

    return VOBJ_CH_UTF8;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_util_convert_to_ucs2
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dst             [?]         
 *  dst_size        [IN]        
 *  src             [?]         
 *  encode          [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_util_convert_to_ucs2(S8 *dst, U8 dst_size, S8 *src, U8 encode)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (encode == VOBJ_CH_UTF8)
    {
        mmi_chset_convert(MMI_CHSET_UTF8, MMI_CHSET_UCS2, (char*)src, (char*)dst, dst_size);
    }
#ifdef __MMI_CHSET_UTF7__	
    else if (encode == VOBJ_CH_UTF7)
    {
        mmi_chset_convert(MMI_CHSET_UTF7, MMI_CHSET_UCS2, (char*)src, (char*)dst, dst_size);
    }
#endif /* __MMI_CHSET_UTF7__ */
    else
    {
        mmi_chset_convert(g_vcard_cntx.CharSet, MMI_CHSET_UCS2, (char*)src, (char*)dst, dst_size);
    }
}


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
void mmi_vcard_util_set_default_charset(U8 default_charset)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_vcard_cntx.CharSet = (mmi_chset_enum) default_charset;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_clear_old_receive_history
 * DESCRIPTION
 *  Deletes old vcard receive screens from history if Phonebook is re-entered thru call
 * PARAMETERS
 *  void     
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_clear_old_receive_history(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_BOOL imps_exist = MMI_FALSE;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_vcard_clear_fmagr_screen();
    if (DeleteScreenIfPresent(SCR_ID_VCARD_RECV_OPT))
    {
        if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_OTHER_APPS)
		{
            mmi_vcard_recv_set_result(MMI_VOBJ_SAVE_DISCARD);
            imps_exist = MMI_TRUE;
		}
    }
    if (DeleteScreenIfPresent(SCR_ID_VCARD_RECV_VIEW_DETAIL))
    {
        if (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_OTHER_APPS)
		{
		    if (g_vcard_cntx.StorageLoc >= MMI_SIM && g_vcard_cntx.StorageLoc <= MMI_SIM4)
            {
                mmi_vcard_recv_set_result(MMI_VOBJ_SAVE_TO_SIM_FAIL + g_vcard_cntx.StorageLoc - MMI_SIM);
            }
            else
            {
                mmi_vcard_recv_set_result(MMI_VOBJ_SAVE_TO_PHONE_FAIL);           
            }
            imps_exist = MMI_TRUE;
		}
    }
    DeleteScreenIfPresent(SCR_ID_VCARD_ENTER_FILENAME_OPT);
    mmi_ime_delete_editor_common_scr(SCR_ID_VCARD_ENTER_FILENAME);
    if (DeleteScreenIfPresent(SCR_ID_VCARD_ENTER_FILENAME))
    {
	    if ((g_vcard_cntx.FileLoc == MMI_VCARD_SAVE_TO_FILE_FROM_RECV)
            && (g_vcard_cntx.ParseLoc == MMI_VCARD_PARSE_FROM_OTHER_APPS))
        {
            mmi_vcard_recv_set_result(MMI_VOBJ_SAVE_TO_FILEMGR_FAIL);
            imps_exist = MMI_TRUE;
        }
    }
    if (imps_exist)
    {
        mmi_vcard_recv_from_other_apps_callback();
    }
}

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
void mmi_vcard_reset_fmagr_flag(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_vcard_fmgr_id = 0;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_clear_fmagr_screen
 * DESCRIPTION
 *  get the version field of the version field line.
 * PARAMETERS
 *  String      [IN]        String read from file for one line
 * RETURNS
 *  version
 *****************************************************************************/
static void mmi_vcard_clear_fmagr_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_vcard_fmgr_id)
    {
        //mmi_fmgr_close_service(g_vcard_fmgr_id);
        g_vcard_fmgr_id = 0;
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_get_version
 * DESCRIPTION
 *  get a vcard file version
 * PARAMETERS
 *  path        [IN]        The vcard file location in file system.
 * RETURNS
 *  result of get version
 *****************************************************************************/
S32 mmi_vcard_get_version(S8 *path)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FS_HANDLE fh;
    INT32 f_size;
    S32 error_code = MMI_VCARD_PARSE_UNKNOW_ERROR;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    fh = MMI_FS_Open((U8*) path, FS_READ_ONLY);

    if (fh > 0)
    {
        MMI_FS_GetFileSize(fh, &f_size);
        if (f_size > VCARD_FILE_SIZE_LIMIT)
        {
            error_code = MMI_VCARD_PARSE_FILE_SIZE_TOO_LARGE;
        }
        else
        {
            error_code = mmi_vcard_get_version_parse_line(fh);
        }
    }
    else
    {
        error_code = fh;
    }

    MMI_FS_Close(fh);
    return error_code;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_get_version_parse_line
 * DESCRIPTION
 *  Read line one by one and to get the version
 * PARAMETERS
 *  fh      [IN]        file handle ready to read
 * RETURNS
 *  parse result
 *****************************************************************************/
static U8 mmi_vcard_get_version_parse_line(FS_HANDLE fh)
{
     /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifdef __MMI_PHB_INFO_FIELD__
    S8 *inBuff = g_vcard_write_buff;
#else
    S8 inBuff[VCARD_WRITE_BUFF_LEN];
#endif

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* BEGIN:VCARD */
    if (mmi_vcard_uti_get_single_line(inBuff, fh) != 0)
    {
        if (mmi_vcard_reader_get_field(inBuff) != VCARD_FIELD_BEGIN)
        {
            return MMI_VCARD_PARSE_BEGIN_ERROR;
        }
        else if (!mmi_vcard_reader_check_header(inBuff))
        {
            return MMI_VCARD_PARSE_BEGIN_ERROR;
        }
    }
    else
    {
        return MMI_VCARD_PARSE_UNKNOW_ERROR;
    }

    while (mmi_vcard_uti_get_single_line(inBuff, fh) != 0)
    {
        if (mmi_vcard_reader_get_field(inBuff) == VCARD_FIELD_VERSION)
        {
            return mmi_vcard_parse_version_line(inBuff);
        }
    }
    return MMI_VCARD_PARSE_UNKNOW_ERROR;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_parse_version_line
 * DESCRIPTION
 *  get the version field of the version field line.
 * PARAMETERS
 *  String      [IN]        String read from file for one line
 * RETURNS
 *  version
 *****************************************************************************/
static U8 mmi_vcard_parse_version_line(S8 *String)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 *cursor;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    cursor = mmi_vobj_uti_get_value(String);
    if (!mmi_vobj_uti_strnicmp(cursor, VCARD_VERSION_21, strlen(VCARD_VERSION_21)))
    {
        return MMI_VCARD_PARSE_VERSION_21;
    }
    else if (!mmi_vobj_uti_strnicmp(cursor, VCARD_VERSION_30, strlen(VCARD_VERSION_30)))
    {
        return MMI_VCARD_PARSE_VERSION_30;    
    }
    else
    {
        return MMI_VCARD_PARSE_VERSION_ERROR;
    }
}


/*--------------------------------------------------Begin vcard export & lod csv ---------------------------------------------------*/
/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_export()
* DESCRIPTION
* Highlight function to associate image from file manager to a new phonebook entry
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_export(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(mmi_vcard_entry_export_option, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

    SetKeyHandler(mmi_vcard_entry_export_option, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

    g_vcard_cntx.FileLoc = MMI_VCARD_SAVE_TO_FILE_FROM_PHB;
    g_vcard_cntx.CardType = MMI_VCARD_SEND_VCARD_TATOL;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_export_from_phone()
* DESCRIPTION
* Highlight function to associate image from file manager to a new phonebook entry
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_export_from_phone(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(mmi_vcard_export_forward_fmgr, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

    SetKeyHandler(mmi_vcard_export_forward_fmgr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

    g_vcard_cntx.ExportLoc = MMI_NVRAM;    
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_export_from_sim()
* DESCRIPTION
* Highlight function to associate image from file manager to a new sim entry
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_export_from_sim(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	U8 nSimID = 0;
	#ifdef __MMI_MULTI_SIM__
	nSimID = GetReqSrcModIndex();
	#endif
    SetLeftSoftkeyFunction(mmi_vcard_export_forward_fmgr, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

    SetKeyHandler(mmi_vcard_export_forward_fmgr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

    g_vcard_cntx.ExportLoc = MMI_SIM + nSimID;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_export_forward_fmgr
 * DESCRIPTION
 *  save vcard to file manager, file manager will take over screen after calling.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_export_forward_fmgr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FMGR_FILTER filter;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(!g_phb_cntx.phb_ready || g_phb_cntx.processing)
    {
        mmi_phb_entry_not_ready();
        return;
    }

    if((g_vcard_cntx.ExportLoc == MMI_NVRAM && g_phb_cntx.phone_used == 0)
        || (g_vcard_cntx.ExportLoc >= MMI_SIM &&g_vcard_cntx.ExportLoc <= MMI_SIM4 && g_phb_cntx.sim_used[g_vcard_cntx.ExportLoc - MMI_SIM] == 0))
    {
        DisplayPopup((PU8)GetString(STR_GLOBAL_EMPTY), IMG_GLOBAL_ERROR,  TRUE, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
        return;
    }

    FMGR_FILTER_INIT(&filter);
    FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);

    mmi_fmgr_select_path_and_enter(
        APP_PHONEBOOK,
        FMGR_SELECT_FOLDER,
        filter,
        (S8*) L"root",
        mmi_vcard_forward_fmgr_select_path_done);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_entry_export_option
 * DESCRIPTION
 *  Entry function of backup option
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_entry_export_option(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8* guiBuffer = NULL;
	U16 nStrItemList[5];
	U16 nNumofItem;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	mmi_trace(g_sw_PHB, "VCARD func: %s, CardType = %d", __FUNCTION__, g_vcard_cntx.CardType);

	EntryNewScreen(SCR_ID_VCARD_EXPORT_OPTION, NULL, mmi_vcard_entry_export_option, NULL);

#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	MTPNP_AD_SetMenuHideUnhide(MENU_ID_PHB_EXPORT_FROM_SIM1);
#endif
	guiBuffer = GetCurrGuiBuffer(SCR_ID_VCARD_EXPORT_OPTION);
	nNumofItem = GetNumOfChild_Ext(MENU_ID_PHB_VCARD_EXPORT);

	GetSequenceStringIds_Ext(MENU_ID_PHB_VCARD_EXPORT, nStrItemList);
	SetParentHandler(MENU_ID_PHB_VCARD_EXPORT);
	#ifdef __MMI_MULTI_SIM__
	RegisterHighlightHandler( MultiSimMenuHighLightHandler );
	#else
	RegisterHighlightHandler( ExecuteCurrHiliteHandler);
	#endif

	ShowCategory15Screen(STR_SCR_VIEW_OPTIONS_CAPTION, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_import()
* DESCRIPTION
* Highlight function to load phb from file manager 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_import(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(mmi_vcard_entry_import_from_option, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

    SetKeyHandler(mmi_vcard_entry_import_from_option, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_entry_import_from_option
 * DESCRIPTION
 *  Entry function of import option
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_entry_import_from_option(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8* guiBuffer = NULL;
    U16 nStrItemList[5];
    U16 nNumofItem;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_PHB, "VCARD func: %s", __FUNCTION__);

    EntryNewScreen(SCR_ID_VCARD_IMPORT_FROM_OPTION, NULL, mmi_vcard_entry_import_from_option, NULL);
    
    guiBuffer = GetCurrGuiBuffer(SCR_ID_VCARD_IMPORT_FROM_OPTION);
    nNumofItem = GetNumOfChild(MENU_ID_PHB_VCARD_IMPORT);

    GetSequenceStringIds(MENU_ID_PHB_VCARD_IMPORT, nStrItemList);
    SetParentHandler(MENU_ID_PHB_VCARD_IMPORT);
    RegisterHighlightHandler(ExecuteCurrHiliteHandler);

    ShowCategory15Screen(STR_SCR_VIEW_OPTIONS_CAPTION, IMG_SCR_PBOOK_CAPTION,
    					  STR_GLOBAL_OK, IMG_GLOBAL_OK,
    					  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
    					  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
    					  LIST_MENU, 0, guiBuffer);

    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
 
/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_import_from_vcf()
* DESCRIPTION
* Highlight function to load phb from file manager to sim1
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_import_from_vcf(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(mmi_vcard_import_from_fmgr, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

    SetKeyHandler(mmi_vcard_import_from_fmgr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

    g_vcard_cntx.ImportLoc = MMI_VCARD_IMPORT_FROM_VCF;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_highlight_import_from_outlook()
* DESCRIPTION
* Highlight function to load phb from file manager to sim1
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_highlight_import_from_outlook(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(mmi_vcard_import_from_fmgr, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

    SetKeyHandler(mmi_vcard_import_from_fmgr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

    g_vcard_cntx.ImportLoc = MMI_VCARD_IMPORT_FROM_OUTLOOK;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_export_forward_fmgr
 * DESCRIPTION
 *  save vcard to file manager, file manager will take over screen after calling.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_import_from_fmgr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FMGR_FILTER filter;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(!g_phb_cntx.phb_ready || g_phb_cntx.processing)
    {
        mmi_phb_entry_not_ready();
        return;
    }

    FMGR_FILTER_INIT(&filter);
    FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);
    FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER_DOT);
    
    if(g_vcard_cntx.ImportLoc == MMI_VCARD_IMPORT_FROM_VCF)
    {
        FMGR_FILTER_SET(&filter, 	FMGR_TYPE_VCF);
    }
    else if(g_vcard_cntx.ImportLoc == MMI_VCARD_IMPORT_FROM_OUTLOOK)
    {
        FMGR_FILTER_SET(&filter, 	FMGR_TYPE_CSV);    
    }

    mmi_fmgr_select_path_and_enter(
        APP_PHONEBOOK,
        FMGR_SELECT_FILE,
        filter,
        (S8*) L"root",
        mmi_vcard_import_from_fmgr_done);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_import_from_fmgr_done
 * DESCRIPTION
 *  call back function of save vcard to file manager, will bring back selected file path
 * PARAMETERS
 *  path            [IN]        Selected file path
 *  is_short        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_import_from_fmgr_done(void *path, int is_short)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_PHB, "VCARD Func:%s, ImportLoc = %d",  __FUNCTION__, g_vcard_cntx.ImportLoc);
    
    mmi_vcard_reset_fmagr_flag();
    if (path == NULL)   /* User does not select file and go back directly. Go Back To Upto forward option or recv option. */
    {
         GoBackToHistory(SCR_ID_VCARD_IMPORT_FROM_OPTION);
         return;
    }

    if(g_vcard_cntx.ImportLoc == MMI_VCARD_IMPORT_FROM_VCF)
    {
        mmi_vcard_recv_from_file_mgr(path);
    }
    else if(g_vcard_cntx.ImportLoc == MMI_VCARD_IMPORT_FROM_OUTLOOK)
    {
        mmi_vcard_outlook_recv_from_file_mgr(path);
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_outlook_recv_from_file_mgr
 * DESCRIPTION
 *  A pre-entry function of receiving vcard functuion. This vcard is forwarded from file manager.
 * PARAMETERS
 *  path        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_outlook_recv_from_file_mgr(S8 *path)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (path == NULL)
    {
        return;
    }

    if (g_vcard_cntx.FilePathPtr == NULL)
    {
        g_vcard_cntx.FilePathPtr = OslMalloc(VOBJ_MAX_PATH_LENGTH);
    }

    pfnUnicodeStrcpy(g_vcard_cntx.FilePathPtr, path);

    g_vcard_cntx.ParseLoc = MMI_VCARD_PARSE_FROM_FILE_MANAGER;

    mmi_vcard_entry_recv_option();
}

extern U8* PhbMassOperateBufPtr;
#define SCRMEM_PHBMASS_LAYER_POOL_SIZE (GDI_LCD_WIDTH *GDI_LCD_HEIGHT * (16>>3) + 3)
extern U8 mmi_phb_mass_process_history_hdlr( void* in_param );
/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_import_processing
 * DESCRIPTION
 *  show processing
 *  send file to parser.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_vcard_import_processing(void)
{
       gdi_handle HandleBase = 0;
	EntryNewScreen(SCR_COPY_ALL_PROGRESS, mmi_vcard_import_exit_processing, NULL, NULL);

        if(NULL !=PhbMassOperateBufPtr )
        {
            gdi_layer_get_active(&HandleBase );
            gdi_layer_lock_frame_buffer();
            memcpy(((gdi_layer_struct*)HandleBase)->buf_ptr,PhbMassOperateBufPtr ,SCRMEM_PHBMASS_LAYER_POOL_SIZE);
            gdi_layer_unlock_frame_buffer();
            OslMfree((void*)PhbMassOperateBufPtr);
            PhbMassOperateBufPtr = NULL;
        }   

	ShowCategory8Screen(STR_SCR_PBOOK_CAPTION,IMG_SCR_PBOOK_CAPTION,
		0, 0, 0, 0,
		STR_GLOBAL_SAVING, IMG_GLOBAL_SAVE, NULL);

	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
    
	SetDelScrnIDCallbackHandler( SCR_COPY_ALL_PROGRESS, (HistoryDelCBPtr)mmi_phb_mass_process_history_hdlr );
}

/*****************************************************************************
* FUNCTION
*  mmi_vcard_import_exit_processing()
* DESCRIPTION
*   Exit function to mmi_vcard_import_processing()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_vcard_import_exit_processing(void)
{
    history_t   h;
    S16		nHistory	=	0;
    gdi_handle HandleBase = 0;
    mmi_trace(1, "mmi_vcard_import_exit_processing g_vcard_cntx.processing = %d", g_vcard_cntx.processing);
    
    if(g_vcard_cntx.processing == TRUE)
    {
        gdi_layer_get_active(&HandleBase );
        PhbMassOperateBufPtr = (U8*)OslMalloc(SCRMEM_PHBMASS_LAYER_POOL_SIZE);
        if( PhbMassOperateBufPtr!= NULL)
        {
            gdi_layer_lock_frame_buffer();
            memset(PhbMassOperateBufPtr,0,SCRMEM_PHBMASS_LAYER_POOL_SIZE);
            memcpy(PhbMassOperateBufPtr , ((gdi_layer_struct*)HandleBase)->buf_ptr,SCRMEM_PHBMASS_LAYER_POOL_SIZE);
            gdi_layer_unlock_frame_buffer();
        }
        
        h.scrnID = SCR_COPY_ALL_PROGRESS;
        h.entryFuncPtr = mmi_vcard_import_processing;
        pfnUnicodeStrcpy((PS8)h.inputBuffer, (PS8)&nHistory);
        GetCategoryHistory(h.guiBuffer);
        AddHistory(h);
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_outlook_recv_parse_file
 * DESCRIPTION
 *  a pre- parsing vcard function. Create path and do some error handling before
 *  send file to parser.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vcard_outlook_recv_parse_file(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UINT32 read_size = 0;    
    S8 tmp_buff[256];    
    INT nret = 0;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_PHB, "VCARD Func: %s, g_vcard_cntx.StorageLoc = %d", __FUNCTION__, g_vcard_cntx.StorageLoc);
    g_vcard_cntx.processing = TRUE;
    mmi_vcard_import_processing();
    
    g_parse_fh = MMI_FS_Open((U8*)g_vcard_cntx.FilePathPtr, FS_READ_ONLY);
    g_is_file_end = FALSE;
    
    if(g_parse_fh >= 0)
    {
        if(g_parse_buff == NULL)
        {
            g_parse_buff = OslMalloc(MAX_VCARD_OUTLOOK_READ_LEN + 2);
        }
        memset(g_parse_buff, 0xFF, MAX_VCARD_OUTLOOK_READ_LEN + 2);
        gp_buff = g_parse_buff;
        
        memset(tmp_buff, 0, 256);

        /*read file*/
        nret = MMI_FS_Read(g_parse_fh, g_parse_buff, MAX_VCARD_OUTLOOK_READ_LEN, &read_size);
        mmi_trace(g_sw_PHB, "VCARD Func: %s, ret = %d, read_size = %d, lenoftmp = %d", __FUNCTION__, nret, read_size, DSM_UnicodeLen(tmp_buff));
        if(nret != FS_NO_ERROR)
        {
            mmi_vcard_outlook_import_parse_file_done(MMI_VCARD_PARSE_FILE_READ_ERROR);
            return;
        }        
        
        if(read_size < MAX_VCARD_OUTLOOK_READ_LEN)
        {
            g_is_file_end = TRUE;
        }  
        
        /*head*/
        mmi_vcard_outlook_get_head_line_buff(tmp_buff, MMI_NVRAM);
        if(pfnUnicodeStrncmp(gp_buff, tmp_buff, pfnUnicodeStrlen(tmp_buff)) != 0)
        {
            memset(tmp_buff, 0, 256);
            mmi_vcard_outlook_get_head_line_buff(tmp_buff, MMI_SIM);
            if(pfnUnicodeStrncmp(gp_buff, tmp_buff, pfnUnicodeStrlen(tmp_buff)) != 0)
            {            
                mmi_vcard_outlook_import_parse_file_done(MMI_VCARD_PARSE_FORMAT_ERROR);
                return;
            }
        }
        
        gp_buff += DSM_UnicodeLen(tmp_buff);
        mmi_vcard_outlook_recv_parse_one_by_one();

    }
    else
    {
        mmi_vcard_outlook_import_parse_file_done(MMI_VCARD_PARSE_FILE_OPEN_ERROR);
    }

    return;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_outlook_recv_parse_one_record
 * DESCRIPTION
 *  a pre- parsing vcard function. Create path and do some error handling before
 *  send file to parser.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
BOOL mmi_vcard_outlook_recv_parse_one_by_one(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    INT nret = 0;
    UINT32 read_size = 0;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/  
    mmi_trace(g_sw_PHB, "VCARD Func: %s", __FUNCTION__);
    if(gp_buff == NULL)
    {
        return FALSE;
    }
    mmi_phb_util_clear_buffer(TRUE);    /* Clear all phonebook temp buffer before parsing */

    /*stat from a new line */
    while(!((*gp_buff == 0xFF) && (*(gp_buff + 1) == 0xFF)))
    {
        if(pfnUnicodeStrncmp(gp_buff, (S8 *)L"\r\n" , pfnUnicodeStrlen((S8 *)L"\r\n")) == 0)
        {
            break;
        }
        gp_buff++;
    }

    if(*gp_buff == 0xFF && *(gp_buff + 1) == 0xFF)
    {
        if(!g_is_file_end)
        {
            memset(g_parse_buff, 0xFF, MAX_VCARD_OUTLOOK_READ_LEN + 2);
            nret = MMI_FS_Read(g_parse_fh, g_parse_buff, MAX_VCARD_OUTLOOK_READ_LEN, &read_size);
            if(nret != FS_NO_ERROR)
            {
                return MMI_VCARD_PARSE_UNKNOW_ERROR;
            }              
            if(read_size < MAX_VCARD_OUTLOOK_READ_LEN)
            {
                g_is_file_end = TRUE;
            }
            gp_buff = g_parse_buff;
            
            while(!((*gp_buff == 0xFF) && (*(gp_buff + 1) == 0xFF)))
            {
                if(pfnUnicodeStrncmp(gp_buff, (S8 *)L"\r\n" , pfnUnicodeStrlen((S8 *)L"\r\n")) == 0)
                {
                    break;
                }
                gp_buff++;
            }
        }
        else
        {
            mmi_vcard_outlook_import_parse_file_done(MMI_VCARD_PARSE_NO_ERROR);
            return FALSE;
        }
    }
    
    if(pfnUnicodeStrncmp(gp_buff, (S8 *)L"\r\n" , pfnUnicodeStrlen((S8 *)L"\r\n")) == 0)
    {
        gp_buff += DSM_UnicodeLen((U8 *)L"\r\n");
    }
    else
    {
        mmi_vcard_outlook_import_parse_file_done(MMI_VCARD_PARSE_FORMAT_ERROR);
        return FALSE;
    }
    
    /*name*/
    nret = mmi_vcard_outlook_import_parse_one_element(MMI_PHB_FIELD_NAME);
    if(nret != MMI_VCARD_PARSE_NO_ERROR)
    {
        mmi_vcard_outlook_import_parse_file_done(nret);
        return FALSE;
    }

    /*number*/
    nret = mmi_vcard_outlook_import_parse_one_element(MMI_PHB_FIELD_NUMBER);
    if(nret != MMI_VCARD_PARSE_NO_ERROR)
    {
        mmi_vcard_outlook_import_parse_file_done(nret);
        return FALSE;
    }    

    if(g_vcard_cntx.StorageLoc == MMI_NVRAM)
    {
        nret = mmi_vcard_outlook_import_parse_one_element(MMI_PHB_FIELD_HOME);
        if(nret != MMI_VCARD_PARSE_NO_ERROR)
        {
            mmi_vcard_outlook_import_parse_file_done(nret);
            return FALSE;
        }        
        
        nret = mmi_vcard_outlook_import_parse_one_element(MMI_PHB_FIELD_COMPANY_NAME);
        if(nret != MMI_VCARD_PARSE_NO_ERROR)
        {
            mmi_vcard_outlook_import_parse_file_done(nret);
            return FALSE;
        }        
        
        nret = mmi_vcard_outlook_import_parse_one_element(MMI_PHB_FIELD_EMAIL);
        if(nret != MMI_VCARD_PARSE_NO_ERROR)
        {
            mmi_vcard_outlook_import_parse_file_done(nret);
            return FALSE;
        }        
        
        nret = mmi_vcard_outlook_import_parse_one_element(MMI_PHB_FIELD_OFFICE);
        if(nret != MMI_VCARD_PARSE_NO_ERROR)
        {
            mmi_vcard_outlook_import_parse_file_done(nret);
            return FALSE;
        }        
        
        nret = mmi_vcard_outlook_import_parse_one_element(MMI_PHB_FIELD_BIRTHDAY);
        if(nret != MMI_VCARD_PARSE_NO_ERROR)
        {
            mmi_vcard_outlook_import_parse_file_done(nret);
            return FALSE;
        }        
    }
    mmi_vcard_recv_save_obj_to_phb();

    return TRUE;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_outlook_import_parse_one_element
 * DESCRIPTION
 *  a pre- parsing vcard function. Create path and do some error handling before
 *  send file to parser.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static S32 mmi_vcard_outlook_import_parse_one_element(UINT8 elementtype)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UINT32 read_size;    
    U16 ntmp_buff_offset = 0;    
    INT nret = 0;
    S8 tmp_buff[256];  
    S8 assic_tmp_buff[256];        
    U16 max_len = 0;
    S32 error_code = MMI_VCARD_PARSE_NO_ERROR;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/   
    mmi_trace(g_sw_PHB, "VCARD Func: %s,elementtype = %d", __FUNCTION__, elementtype);

    memset(tmp_buff, 0, 256);
    switch(elementtype)
    {
    case MMI_PHB_FIELD_NAME:
        max_len = MAX_PB_NAME_LENGTH;
        break;
    case MMI_PHB_FIELD_EMAIL:
        max_len = MAX_PB_EMAIL_LENGTH;
        break;    
    case MMI_PHB_FIELD_COMPANY_NAME:
        max_len = MAX_PB_COMPANY_LENGTH;
        break;
    case MMI_PHB_FIELD_NUMBER:
    case MMI_PHB_FIELD_HOME:
    case MMI_PHB_FIELD_OFFICE:
        max_len = MAX_PB_NUMBER_LENGTH;
        break;
     case MMI_PHB_FIELD_BIRTHDAY:
        max_len = 9;
        break;       
    }
        
     while((pfnUnicodeStrncmp(gp_buff, (S8 *)L",", pfnUnicodeStrlen((S8 *)L",")) != 0) 
        &&(pfnUnicodeStrncmp(gp_buff, (S8 *)L"\r\n" , pfnUnicodeStrlen((S8 *)L"\r\n")) != 0)
        &&  ntmp_buff_offset <= (max_len + 1) * ENCODING_LENGTH)  
    {
        if((*gp_buff == 0xFF && *(gp_buff + 1) == 0xFF)
            || ((pfnUnicodeStrncmp(gp_buff, (S8 *)L"\r" , pfnUnicodeStrlen((S8 *)L"\r")) == 0) && *(gp_buff + 2) == 0xFF && *(gp_buff + 3) == 0xFF))
        {
            if(!g_is_file_end)
            {                
                if(*gp_buff == 0xFF && *(gp_buff + 1) == 0xFF)
                {
                    memset(g_parse_buff, 0xFF, MAX_VCARD_OUTLOOK_READ_LEN + 2);
                    nret = MMI_FS_Read(g_parse_fh, g_parse_buff, MAX_VCARD_OUTLOOK_READ_LEN, &read_size);
                    if(read_size < MAX_VCARD_OUTLOOK_READ_LEN)
                    {
                        g_is_file_end = TRUE;
                    }                    
                }
                else
                {
                    memset(g_parse_buff, 0xFF, MAX_VCARD_OUTLOOK_READ_LEN + 2);
                    memcpy(g_parse_buff, L"\r", DSM_UnicodeLen((S8 *)L"\r"));
                    nret = MMI_FS_Read(g_parse_fh, g_parse_buff + DSM_UnicodeLen((S8 *)L"\r"), MAX_VCARD_OUTLOOK_READ_LEN - DSM_UnicodeLen((S8 *)L"\r"), &read_size);
                    if(read_size < MAX_VCARD_OUTLOOK_READ_LEN - DSM_UnicodeLen((S8 *)L"\r"))
                    {
                        g_is_file_end = TRUE;
                    }
                }
                
                if(nret != FS_NO_ERROR)
                {
                    return MMI_VCARD_PARSE_UNKNOW_ERROR;
                }              

                gp_buff = g_parse_buff;
                continue;
            }
            else
            {
                break;
            }
        }
        
        tmp_buff[ntmp_buff_offset] = *gp_buff;
        ntmp_buff_offset++;
        gp_buff++;
    }
    mmi_trace(g_sw_PHB, "VCARD Func: %s,elementtype = %d, ntmp_buff_offset = %d", __FUNCTION__, elementtype, ntmp_buff_offset);
    
    if(pfnUnicodeStrncmp(gp_buff, (S8 *)L",", pfnUnicodeStrlen((U8 *)L",")) == 0)
    {
        gp_buff += DSM_UnicodeLen(VCARD_ITEM_BREAK);
    }

    if(ntmp_buff_offset > 0)
    {
        switch(elementtype)
        {
        case MMI_PHB_FIELD_NAME:
            pfnUnicodeStrcpy(pbName, tmp_buff);
            break; 
        case MMI_PHB_FIELD_NUMBER:
            pfnUnicodeStrcpy(pbNumber, tmp_buff);
            break;             
        case MMI_PHB_FIELD_HOME:
            pfnUnicodeStrcpy(pbHomeNumber, tmp_buff);
            break;            
        case MMI_PHB_FIELD_COMPANY_NAME:
            pfnUnicodeStrcpy(pbCompanyName, tmp_buff);
            break;             
        case MMI_PHB_FIELD_OFFICE:
            pfnUnicodeStrcpy(pbOfficeNumber, tmp_buff);
            break;             
        case MMI_PHB_FIELD_EMAIL:
            pfnUnicodeStrcpy(pbEmailAddress, tmp_buff);
            break;     
        case MMI_PHB_FIELD_BIRTHDAY:
            UnicodeToAnsii(assic_tmp_buff, tmp_buff);
            mmi_phb_retrieve_bday(assic_tmp_buff, pbYear, pbMon, pbDay, TRUE);
            break;
        }    
    }
    
    return error_code;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_outlook_get_head_line_buff
 * DESCRIPTION
 *  write all phonebook entris into csv format into destination file.
 * PARAMETERS
 *  fh        [FS_HANDLE]        fs handle
 *  object              [S8*]        object buffer
 * RETURNS
 *  void
 ****************************************************************************/
static void mmi_vcard_outlook_get_head_line_buff(S8 *object, U8 location)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    //S8 itembreak[2];
    S8 tmp_buff[256];
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    object[0] = 0xFF;
    object[1] = 0xFE;

    memset(tmp_buff, 0, 256);

    if(location == MMI_NVRAM)
    {
        AnsiiToUnicodeString(tmp_buff, (S8 *)g_vcard_outlook_phone_head);
    }
    else
    {
        AnsiiToUnicodeString(tmp_buff, (S8 *)g_vcard_outlook_sim_head);
    }
    
    pfnUnicodeStrcat(object, tmp_buff);
    
#if 0
    AnsiiToUnicodeString(itembreak, VCARD_ITEM_BREAK);

    /*name*/
    pfnUnicodeStrcat(object, GetString(STR_ID_PHB_NAME));

     /*VCARD_ITEM_BREAK*/
    pfnUnicodeStrcat(object, itembreak);

    /* Mobile Number */
    pfnUnicodeStrcat(object, GetString(STR_ID_PHB_MOBILE_NUMBER));
    
    if(g_vcard_cntx.ExportLoc == MMI_NVRAM)
    {
    #if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
         /*VCARD_ITEM_BREAK*/
        pfnUnicodeStrcat(object, itembreak);    
    
        pfnUnicodeStrcat(object, GetString(STR_HOME_NUMBER));        

         /*VCARD_ITEM_BREAK*/
        pfnUnicodeStrcat(object, itembreak);
         
        pfnUnicodeStrcat(object, GetString(STR_COMPNAY_NAME));

         /*VCARD_ITEM_BREAK*/
        pfnUnicodeStrcat(object, itembreak);
         
        pfnUnicodeStrcat(object, GetString(STR_EMAIL_ADRESS));

         /*VCARD_ITEM_BREAK*/
        pfnUnicodeStrcat(object, itembreak);
         
        pfnUnicodeStrcat(object, GetString(STR_OFFICE_NUMBER));
    #endif   
    
    #ifdef __MMI_PHB_BIRTHDAY_FIELD__
         /*VCARD_ITEM_BREAK*/
        pfnUnicodeStrcat(object, itembreak);
         
        pfnUnicodeStrcat(object, GetString(STR_ID_PHB_BIRTHDAY));
    #endif/* __MMI_PHB_BIRTHDAY_FIELD__ */    
    }
#endif //0    
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_outlook_import_done_del_src
 * DESCRIPTION
 * del all useless src
  * PARAMETERS
  * RETURNS
 *  void
 ****************************************************************************/
void mmi_vcard_outlook_import_done_del_src(void)
{  
    g_vcard_cntx.processing = FALSE;

    fmgr_remove_all_screen();
    DeleteScreenIfPresent(SCR_ID_VCARD_RECV_OPT);
    DeleteScreenIfPresent(SCR_COPY_ALL_PROGRESS);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_outlook_import_parse_file_done
 * DESCRIPTION
 * pop outlook vcard import result
 * PARAMETERS
 * RETURNS
 *  void
 ****************************************************************************/
static void mmi_vcard_outlook_import_parse_file_done(UINT32 errorcode)
{
    mmi_trace(g_sw_PHB, "VCARD Func: %s, errorcode = %d", __FUNCTION__, errorcode);

    if(errorcode == MMI_VCARD_PARSE_NO_ERROR)
    {
        DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
    }
    else
    {
        DisplayPopup((PU8)GetString(STR_GLOBAL_NOT_DONE), IMG_GLOBAL_ERROR, TRUE, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
    }
    
    mmi_vcard_outlook_import_done_del_src();
    mmi_vcard_import_free_buffer();
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_write_head_to_csvfile
 * DESCRIPTION
 *  write all phonebook entris into csv format into destination file.
 * PARAMETERS
 *  fh        [FS_HANDLE]        fs handle
 *  object              [S8*]        object buffer
 * RETURNS
 *  void
 ****************************************************************************/
static S32 mmi_vcard_write_head_to_csvfile(FS_HANDLE fh, S8 *object)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    INT32 write_len = 0;
    INT32 object_len = 0;    
    S32 result = 0;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_vcard_outlook_get_head_line_buff(object, g_vcard_cntx.ExportLoc);

    object_len = DSM_UnicodeLen(object);
    MMI_ASSERT(object_len < VCARD_WRITE_BUFF_LEN);

    result = MMI_FS_Write(fh, object, object_len, (INT32 *)&write_len);   

    return result;   
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_writer_one_record_to_csvfile
 * DESCRIPTION
 *  write all phonebook entris into csv format into destination file.
 * PARAMETERS
 *  fh        [FS_HANDLE]        fs handle
 *  object              [S8*]        object buffer
 *  temp_buf        [S8*]        temp buffer
 *  field_content [vcard_field_struct] wirte content
 * RETURNS
 *  void
 ****************************************************************************/
static S32 mmi_vcard_writer_one_record_to_csvfile(FS_HANDLE fh, S8 *object, S8 *temp_buf, vcard_field_struct *field_content)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    INT32 write_len = 0;
    INT32 object_len = 0;    
    S32 result = 0;
    S8 itembreak[2];
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    /*head*/
    AnsiiToUnicodeString(itembreak, VCARD_ITEM_BREAK);
    
    AnsiiToUnicodeString(temp_buf, VCARD_LINE_BREAK);
    pfnUnicodeStrcpy(object, temp_buf);
    
    /* Name Field, write N field. */
    if(pfnUnicodeStrlen(field_content->ucs2_name))
    {
        pfnUnicodeStrcat(object, field_content->ucs2_name);
    }

    /*VCARD_ITEM_BREAK*/
    pfnUnicodeStrcat(object, VCARD_ITEM_BREAK);

    /* Mobile Number */
    if (strlen(field_content->number))
    {
        AnsiiToUnicodeString(temp_buf, field_content->number);
        pfnUnicodeStrcat(object, temp_buf);
    }

    if(g_vcard_cntx.ExportLoc == MMI_NVRAM)
    {
        #if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
        /*VCARD_ITEM_BREAK*/
        pfnUnicodeStrcat(object, VCARD_ITEM_BREAK);
        
        /* Home Number */
        if ((field_content->home_num != NULL) && strlen(field_content->home_num))
        {
            AnsiiToUnicodeString(temp_buf, field_content->home_num);
            pfnUnicodeStrcat(object, temp_buf);
        }
        
        /*VCARD_ITEM_BREAK*/
        pfnUnicodeStrcat(object, VCARD_ITEM_BREAK);
        
        /* Company Name */
        if ((field_content->ucs2_company != NULL) && pfnUnicodeStrlen(field_content->ucs2_company))
        {
            pfnUnicodeStrcat(object, field_content->ucs2_company);
        }      

        /*VCARD_ITEM_BREAK*/
        pfnUnicodeStrcat(object, VCARD_ITEM_BREAK);        

        /* Email */
        if ((field_content->email_addr != NULL) && strlen(field_content->email_addr))
        {
            AnsiiToUnicodeString(temp_buf, field_content->email_addr);
            pfnUnicodeStrcat(object, temp_buf);
        }

        /*VCARD_ITEM_BREAK*/
        pfnUnicodeStrcat(object, VCARD_ITEM_BREAK);  

        /* Office Number */
        if ((field_content->office_num != NULL) && strlen(field_content->office_num))
        {
            AnsiiToUnicodeString(temp_buf, field_content->office_num);
            pfnUnicodeStrcat(object, temp_buf);
        }
       
        #endif /* !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) */

        #ifdef __MMI_PHB_BIRTHDAY_FIELD__
        /*VCARD_ITEM_BREAK*/
        pfnUnicodeStrcat(object, VCARD_ITEM_BREAK);  
        
        /* Birthday */
        if ((field_content->b_day != NULL) && strlen(field_content->b_day) && strcmp(field_content->b_day, "00000000") != 0)
        {
            AnsiiToUnicodeString(temp_buf, field_content->b_day);
            pfnUnicodeStrcat(object, temp_buf);
        }
        #endif/* __MMI_PHB_BIRTHDAY_FIELD__ */
    }
    
    object_len = DSM_UnicodeLen(object);
    MMI_ASSERT(object_len < VCARD_WRITE_BUFF_LEN);

    result = MMI_FS_Write(fh, object, object_len, (INT32 *)&write_len);

    return result;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vcard_writer_all_card_to_csvfile
 * DESCRIPTION
 *  write all phonebook entris into csv format into destination file.
 * PARAMETERS
 *  path        [IN]        Destination path
 * RETURNS
 *  void
 *****************************************************************************/
extern U16 PhoneBookEntryCount;
static S32 mmi_vcard_writer_all_card_to_csvfile(S8 *path)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 store_index = 0;
    U16 totalcnt = 0;
    U16 item_cnt = 0;
    BOOL is_valid_item = FALSE;
    U16 i = 0;
    S32 result = 0;    
    FS_HANDLE fh;
    S8 *object;
    S8 *temp_buf;
    S8 number_buff[MAX_PB_NUMBER_LENGTH + 1 + 1];
    S8 number_buff2[MAX_PB_NUMBER_LENGTH + 1 + 1];
    PHB_OPTIONAL_IDS_STRUCT *opt_ids = NULL;
    vcard_field_struct field_content;
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
    PHB_OPTIONAL_FIELDS_STRUCT	PHBOptionalFields;
    S16 pError;
#endif
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
    S8 b_day[9];    /* Format: yyyymmdd */
#endif

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_PHB, "VCARD func:%s, ExportLoc = %d, path = %s", __FUNCTION__, g_vcard_cntx.ExportLoc, path);
    mmi_phb_show_in_progress(STR_GLOBAL_SAVING, IMG_GLOBAL_SAVE);

    fh = MMI_FS_Open((U8*)path, FS_CREATE |FS_READ_WRITE);
    if(fh < 0)
    {
        return fh;
    }
    
    switch(g_vcard_cntx.ExportLoc)
    {
        case MMI_NVRAM:
            totalcnt = g_phb_cntx.phone_used;
            break;
        case MMI_SIM:
	 case MMI_SIM2:
	 case MMI_SIM3:
	 case MMI_SIM4:
            totalcnt = g_phb_cntx.sim_used[g_vcard_cntx.ExportLoc - MMI_SIM];
            break;
    }
    /* Allocate buffer */
    object = OslMalloc(VCARD_WRITE_BUFF_LEN);
    if(object == NULL)
    {
        return 0;
    }
    memset(object, 0, VCARD_WRITE_BUFF_LEN);

    temp_buf = OslMalloc(VCARD_WRITER_FIELD_BUFFER);
    if(temp_buf == NULL)
    {
        return 0;
    }    
    memset(temp_buf, 0, VCARD_WRITER_FIELD_BUFFER);
    mmi_trace(g_sw_PHB, "VCARD func:%s, fh = %x, store_index = %d, totalcnt = %d", __FUNCTION__, fh, store_index, totalcnt);

    /*write head*/
    mmi_vcard_write_head_to_csvfile(fh, object);
    
    for(i = 0; i < PhoneBookEntryCount; i++)
    {
        memset((void*)&field_content, 0, sizeof(field_content));
        store_index = g_phb_name_index[i];
        is_valid_item = FALSE;
        
	#ifdef __MMI_MULTI_SIM__
 	if ((g_vcard_cntx.ExportLoc >= MMI_SIM)&&(g_vcard_cntx.ExportLoc <= MMI_SIM4) && (store_index >= MAX_PB_PHONE_ENTRIES + (g_vcard_cntx.ExportLoc - MMI_SIM)*MAX_PB_SIM_ENTRIES&& store_index < (MAX_PB_PHONE_ENTRIES+(g_vcard_cntx.ExportLoc - MMI_SIM + 1)*MAX_PB_SIM_ENTRIES)))
        {
        	is_valid_item = TRUE;			
        }
        else if ((g_vcard_cntx.ExportLoc == MMI_NVRAM) && (store_index < MAX_PB_PHONE_ENTRIES))
        {
        	is_valid_item = TRUE;			
        }	
	#else
        if ((g_vcard_cntx.ExportLoc == MMI_SIM) && (store_index >= MAX_PB_PHONE_ENTRIES))
        {
        	is_valid_item = TRUE;			
        }
        else if ((g_vcard_cntx.ExportLoc == MMI_NVRAM) && (store_index < MAX_PB_PHONE_ENTRIES))
        {
        	is_valid_item = TRUE;			
        }	    
	#endif

        if(is_valid_item && (item_cnt < totalcnt))
        {
            item_cnt ++;
            /* Mobile Number */
            if (((PhoneBook[store_index].tel.type & 0x90) == 0x90) && (PhoneBook[store_index].tel.type != 0xFF))
            {
                mmi_phb_convert_to_digit_by_store_index((U8*) number_buff2, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1, store_index);
                /* now number is +**21*8123456# */
                /* call the third API to transfer + to place just after SS pattern */
                applib_decode_number((U8*)number_buff2, (U8*)number_buff, 0x91);
                /* now number is **21*+8123456#, so we can show the original data to user */
            }
            else
            {
                mmi_phb_convert_to_digit_by_store_index((U8*) number_buff, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1, store_index);
            }

            field_content.ucs2_name = (S8*) PhoneBook[store_index].alpha_id.name;
            field_content.number = (S8*) number_buff;
            
            if (store_index < MAX_PB_PHONE_ENTRIES) /* In NVRAM */
            {
            #if defined(__MMI_PHB_BIRTHDAY_FIELD__)
                memset(b_day, 0, 9);
                mmi_vcard_get_bday_to_buff(store_index, (S8*) b_day);
                field_content.b_day = b_day;
            #endif /* defined(__MMI_PHB_BIRTHDAY_FIELD__) */ 
            
            #if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
                ReadRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 
                1, 
                (void*)&PHBOptionalFields, 
                store_index*OPTIONAL_FIELDS_RECORD_SIZE, 
                &pError);

                if(mmi_phb_op_check_valid_number((S8*) PHBOptionalFields.homeNumber))
                {
                    field_content.home_num = (S8*) PHBOptionalFields.homeNumber;
                }
                if(mmi_phb_op_check_valid_number((S8*) PHBOptionalFields.officeNumber))
                {
                    field_content.home_num = (S8*) PHBOptionalFields.officeNumber;
                }
                if(mmi_phb_op_check_valid_number((S8*) PHBOptionalFields.faxNumber))
                {
                    field_content.home_num = (S8*) PHBOptionalFields.faxNumber;
                }
                field_content.email_addr = (S8*) PHBOptionalFields.emailAddress;
            #ifdef __MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__
                field_content.email_addr2 = (S8*) PHBOptionalFields.emailAddress2;
            #endif
                field_content.ucs2_company = (S8*) PHBOptionalFields.companyName;
            #endif /* !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) */
            #ifdef __MMI_PHB_INFO_FIELD__
                mmi_phb_info_read_data_to_buff(store_index);
                field_content.info_fields = &PHBOptionalFields;
            #endif 
                mmi_phb_read_optional_ids(store_index);
                opt_ids = (PHB_OPTIONAL_IDS_STRUCT*) g_phb_cntx.optional_ids;
                field_content.optional_ids = opt_ids;
            }

            memset(object, 0, VCARD_WRITE_BUFF_LEN);
            result = mmi_vcard_writer_one_record_to_csvfile(fh, object, temp_buf, &field_content);
            mmi_trace(g_sw_PHB, "VCARD func:%s, result = %d", __FUNCTION__, result);
        }
    }

    OslMfree(temp_buf);
    OslMfree(object);
    MMI_FS_Close(fh);
    
    return result;
}

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
void mmi_vcard_import_free_buffer(void)
{
    if(g_parse_buff != NULL)
    {
        OslMfree(g_parse_buff);
        g_parse_buff = NULL;
    }
    if(g_parse_fh >= 0)
    {
    	MMI_FS_Close(g_parse_fh);  
    	g_parse_fh = -1;
    }
    g_phb_enter_from = MMI_PHB_ENTER_NONE;    
}
/*--------------------------------------------------End vcard csv ---------------------------------------------------*/

#endif /* __MMI_VCARD__ */
#endif /* _MMI_VCARD_C */

