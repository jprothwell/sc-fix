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
 *	PhoneBookGprot.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   PhoneBook global prototypes 
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
#ifndef _PHONEBOOKGPROT_H
#define _PHONEBOOKGPROT_H


#include "mmi_data_types.h"
#include "phonebooktypes.h"

/* 
** Typedef 
*/
/* address filed struct */
typedef struct
{
    U8 pobox[(MAX_PB_ADDRESS_LENGTH + 1) *ENCODING_LENGTH];         /* pobox field */
    U8 extension[(MAX_PB_ADDRESS_LENGTH + 1) *ENCODING_LENGTH];     /* extension field */
    U8 street[(MAX_PB_ADDRESS_LENGTH + 1) *ENCODING_LENGTH];        /* street field */
    U8 city[(MAX_PB_ADDRESS_LENGTH + 1) *ENCODING_LENGTH];          /* city field */
    U8 state[(MAX_PB_ADDRESS_LENGTH + 1) *ENCODING_LENGTH];         /* state field */
    U8 postalcode[(MAX_PB_ADDRESS_LENGTH + 1) *ENCODING_LENGTH];    /* postalcode field */
    U8 country[(MAX_PB_ADDRESS_LENGTH + 1) *ENCODING_LENGTH];	    /* country field */
} MMI_PHB_ADDRESS_STRUCT;

typedef struct
{
    DOUBLE longitude;
    DOUBLE latitude;
} MMI_PHB_GEOGRAPHIC_FIELD_STRUCT;

/* info filed struct */
typedef struct
{
    U8 title[(MAX_PB_TITLE_LENGTH + 1) *ENCODING_LENGTH];       /* title field */
    U8 url[MAX_PB_URL_LENGTH + 1];                              /* url field */
    MMI_PHB_ADDRESS_STRUCT address;                             /* address field */
    U8 note[(MAX_PB_NOTE_LENGTH + 1) *ENCODING_LENGTH];         /* note field */
} PHB_INFO_FIELDS_STRUCT;

typedef void (*FuncPhbListCallback)(MMI_PHB_VCARD_STRUCT* resut_entry);

/* 
** Extern Global Function
*/
extern PS8 lookUpName (PS8);						/* Search for the number entry for corresponding name input */
extern PS8 lookUpNumber (PS8);						/* Search for the name entry for corresponding number input */
extern void mmi_phb_generic_enter_list(FuncPhbListCallback func_list);
extern void mmi_phb_generic_enter_list_result(void);


extern void mmi_phb_idle_enter_phb_list(void);
extern void SaveNumberFromIdleScrn (PS8, U8);			/* Saves a number from the idle screen */
extern void mmi_phb_get_sim_entry_by_location(U16);

#ifdef __MMI_MULTI_SIM__
extern PHB_CM_INTERFACE mmi_phb_call_get_data_for_call_mgnt(PU8 number, UINT8 nSimId);
#else
extern PHB_CM_INTERFACE mmi_phb_call_get_data_for_call_mgnt(PU8);
#endif
extern UINT8 mmi_phb_get_callergroup_by_number(PUINT8 number);
extern UINT8 mmi_phb_get_black_list_flag_by_group(UINT8 group_id);
extern UINT8 mmi_phb_get_red_list_flag_by_group(UINT8 group_id);
extern UINT8 mmi_phb_group_in_black_list(PUINT8 number);
extern UINT8 mmi_phb_group_in_red_list(PUINT8 number);
extern void mmi_phb_call_set_dial_from_list(U8 value);
extern S8* mmi_phb_image_get_path_from_id(U16 record_index);
extern S8* mmi_phb_video_get_path_from_id(U16 record_index);
extern void mmi_phb_call_enter_from_active_call(void);
extern void mmi_phb_call_enter_from_deflect_option(void);
extern void mmi_phb_call_enter_from_divert(void);
extern void mmi_phb_call_send_data_for_divert(void);
extern void mmi_phb_call_send_data_for_deflect(void);
extern void mmi_phb_list_enter_first_from_active_call(void);
extern void mmi_phb_list_enter_last_from_active_call(void);


extern void mmi_phb_sms_enter_send_entry(void);
extern void mmi_phb_frm_enter_send_entry(void) ;
extern void mmi_phb_sms_enter_send_by_group (void);
extern void mmi_phb_sms_send_data_choose_entry(void);
extern void mmi_phb_sms_send_data_for_entry_name_only(void);
extern void mmi_phb_sms_send_data_for_entry_list(void);
extern void mmi_phb_sms_send_data_for_entry (S8* number);
extern void mmi_phb_sms_send_data_for_group(void);

extern BOOL mmi_phb_callergroup_restore_default(void);
extern void mmi_phb_speed_dial_enter_phb(void);

extern void mmi_phb_update_delete_image(U16);
extern void mmi_phb_update_delete_image_caller_group(U16);
extern void mmi_phb_update_delete_audio(U16);
extern void mmi_phb_update_delete_audio_caller_group(U16);
extern U8 mmi_phb_check_valid_sos_number(S8*);

extern void mmi_phb_fdn_get_list_to_ram(void);
/*****************************************************************************
 * FUNCTION
 *  mmi_phb_truncate_pbname_buffer
 * DESCRIPTION
 
 * PARAMETERS
 *  storage     :      [IN]          storage of select contact    
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_phb_truncate_pbname_buffer(U8 storage);
/*****************************************************************************
 * FUNCTION
 *  mmi_phb_calculate_number_valid_length
 * DESCRIPTION
 
 * PARAMETERS
 *  in_number         :         [IN]          input number
 * RETURNS 
 *  U8
 * RETURN VALUES
 *  length            :                       valid number length
 *****************************************************************************/
extern U8 mmi_phb_calculate_number_valid_length(S8 *in_number);
/*****************************************************************************
 * FUNCTION
 *  mmi_phb_name_remove_invalid_character
 * DESCRIPTION
 *  remove invalid characters in name
 * PARAMETERS
 *  name    :   [IN] Input name string       
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_phb_name_remove_invalid_character(S8 *name);
/*****************************************************************************
 * FUNCTION
 *  mmi_phb_entry_op_remove_invalid_name_and_number
 * DESCRIPTION
 *  remove invalid char before add entry.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_phb_entry_op_remove_invalid_name_and_number(void);

/*****************************************************************************
 * FUNCTION
 *  mmi_phb_convert_to_digit_by_store_index
 * DESCRIPTION
 *  convert BCD string to ASCII
 * PARAMETERS
 *  dest              :     [OUT]           An ASCII encoding string.
 *  source            :     [IN]            An BCD encoding string.
 *  max_dest_len      :     [IN]            The byte size of dest array.(include null terminate '\0')
 *  store_index       :     [IN]            contact's store index
 * RETURNS
 *  U8
 * RETURN VALUES
 *  count             :                     byte number after convert.
 *****************************************************************************/
extern U8 mmi_phb_convert_to_digit_by_store_index(U8 *dest, U8 *source, U8 max_dest_len, U16 store_index);

#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
/*****************************************************************************
 * FUNCTION
 *  mmi_phb_retrieve_bday
 * DESCRIPTION
 *  parse birthday field of a vcard object.s
 * PARAMETERS
 *  b_string       :     [IN]       input date buffer  
 *  pb_year        :     [OUT]      output year buffer
 *  pb_mon         :     [OUT]      output month buffer
 *  pb_day         :     [OUT]      output day buffer
 *  change_flag    :     [IN]       bday changed or not 
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_phb_retrieve_bday(S8* b_string, S8* pb_year, S8* pb_mon, S8* pb_day, BOOL change_flag);
#endif

#if 1 //def MMS_SUPPORT  //fengzb 2009-3-3
void EntryPhbFromMMSNumber(void) ;
void EntryPhbFromMMSEmail(void) ;
void EntryPhbFromMMSAll(void);
#ifdef __MMI_PHB_SEND_MMS_FROM_PHB__
extern void mmi_phb_mms_send_data_choose_entry(void);
#endif
#endif /* MMS_SUPPORT */

extern void mmi_phb_fmgr_from_file_mgr(S8* path);
extern U32 mmi_phb_util_convert_number_to_int(PU8);

PS8 BigEndianToLittleEndian(PS8, PS8);
pBOOL GetUCS2Flag(PS8);
void PhbAsciiToGSM7Bit(PU8);
void PhbGSM7BitToAscii(PU8, U8);

#if defined(__MMI_EMAIL__)
extern void mmi_phb_email_enter_list(void);
extern void mmi_phb_email_choose_entry(void);
extern void mmi_phb_email_save_entry_to_phb (S8* ucs2_name, S8* ucs2_email);
#endif

#if defined(__MMI_VRSD_DIAL__)
extern void mmi_phb_vrsddial_enter_tag_list(void);
extern void mmi_phb_vrsddial_get_tag_list_index(S32 nIndex);
extern void mmi_phb_vrsddial_playback_by_store_index(void);
extern void mmi_phb_vrsddial_enter_no_tag_list(void);
extern void mmi_phb_highlight_op_voice_dial(void);
extern void mmi_phb_vrsddial_auto_dial(U16 store_index);
#endif

#if defined(__MMI_VRSI__) && defined(__MMI_VRSI_TRAIN_TAG__)
extern void mmi_phb_vrsi_pre_entry_list(void);
extern void mmi_phb_vrsi_entry_list_callback(void);
#endif

extern BOOL mmi_phb_util_check_entry_exist(U16 store_index);
#ifdef __FT_IDLE_SMART_DIAL__
extern S32 mmi__smartdial_search_list_get_hint ( S32 item_index, UI_string_type *hint_array );
extern pBOOL mmi__smartdial_search_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask );
extern S32 OnChangeEdit(U8* KeyWord);
extern int   IsSubStr(char *motherStr, char *sonStr, int subLen);
#endif

#endif //_PHONEBOOKGPROT_H


