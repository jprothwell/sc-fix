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
 * MessagesExDcl.H
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for MMI SMS APP.
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: MessagesExDcl.h

  	PURPOSE		: ---

 

	AUTHOR		: Hiran and Magesh k

	DATE		: ---

**************************************************************/

#ifndef _PIXCOM_MESSAGES_EXTERN_DCL_H
#define _PIXCOM_MESSAGES_EXTERN_DCL_H

#include "mmi_data_types.h"
//

typedef enum {
	MMI_SEND_ONLY,
	MMI_SEND_BY_NAME,
	MMI_SEND_TO_MANY,
	MMI_CALLER_GROUP_NAMES,
	MMI_CALLER_GROUP_DATA,
	MMI_VOICEMAIL_NUMBERS,
	MMI_SEND_SMS_FROM_PHB,	//CSD 2004-02-03 Wilson.
	MMI_SEND_VCARD_FROM_PHB, //CSD 2004-03-25 Wilson
	MMI_SEND_CHAT,
	MMI_SAVE_CHAT,
	MMI_SEND_SMS_FROM_CLG,//wangwb 2007.06.12
	MMI_PHB_SMS_INTERFACE_NONE
} MMI_PHB_SMS_INTERFACE;
#ifdef __MMI_MESSAGES_EMS__
typedef enum
{
	EMS_OBJECT_PICTURE_STATE=1,
	EMS_OBJECT_MY_PICTURE_STATE,
	EMS_OBJECT_PREDEFINED_ANIMATION_STATE,
	EMS_OBJECT_MY_ANIMATION_STATE,
	EMS_OBJECT_MELODY_STATE,
	EMS_OBJECT_MY_MELODY_STATE,
	EMS_OBJECT_PREDEFINED_SOUND_STATE,
	EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE,
	EMS_OBJECT_MY_MELODY_DOWNLOAD_APP_STATE
} EMS_OBJECT_DISPLAY_STATE;
#endif


#ifdef __MMI_MESSAGES_CLUB__
#define MSG_CLUB_SERVICE_CODE_LENGTH	6
#define MSG_CLUB_EXPLAIN_CODE_LENGTH	14
#define MSG_CLUB_CODE_LENGTH			30

#define MSG_CLUB_VOICE_CODE_LENGTH	16
#define MSG_CLUB_LONG_CODE_LENGTH	12
#define MSG_CLUB_SHORT_CODE_LENGTH	4


typedef enum
{
	MSG_CLUB_CT,
	MSG_CLUB_CU,
	MSG_CLUB_NETWORK_TOTAL
}msg_club_network_enum;


typedef enum
{
	MSG_CLUB_SERVICE_SUBSCRIBE,
	MSG_CLUB_SERVICE_ORDER,
	MSG_CLUB_SERVICE_EXPLAIN,
	
	MSG_CLUB_SERVICE_CANCEL,
	MSG_CLUB_SERVICE_MORE
}msg_club_service_enum;


typedef enum
{
	MSG_CLUB_MENU			=	0x01,
	MSG_CLUB_DY_DB_ZF_TD	=	0x02,
	MSG_CLUB_DY_DB_ZF	=	0x04,
	MSG_CLUB_DY_DB		=	0x08,
	MSG_CLUB_TD			=	0x10,
	MSG_CLUB_INFO			=	0x20,
	MSG_CLUB_VOICE_MENU	=	0x40,
	MSG_CLUB_CT_ONLY		=	0x80
}msg_club_menu_enum;

typedef enum
{
	PHONE_PRE		=	0x0001,
	PHONE_END		=	0x0002,
	NUMBER_PRE	=	0x0004,
	NUMBER_END	=	0x0008,
	CODE_PRE		=	0x0010,
	CODE_END		=	0x0020,
	SERVICE_PRE	=	0x0040,
	SER_END		=	0x0080,
	INPUT_NUM		=	0x0100,
	INPUT_ENG		=	0x0200,
	INPUT_CHI		=	0x0400,
	INPUT_CODE		=	0x0800,
	INPUT_DA		=	0x1000,
	INPUT_NAME	=	0x2000,
	INPUT_SMS		=	0x4000,
	INPUT_PWD		=	0x8000
}msg_club_code_enum;


typedef struct
{
	U16 Level2String;
	S8 CodeSubscribe[MSG_CLUB_NETWORK_TOTAL][MSG_CLUB_SERVICE_CODE_LENGTH];
	S8 CodeOrder[MSG_CLUB_NETWORK_TOTAL][MSG_CLUB_SERVICE_CODE_LENGTH];
	S8 ExplainCode[MSG_CLUB_EXPLAIN_CODE_LENGTH];
}msg_club_submenu_struct;


typedef struct
{
	U8 Level1Menu;
	U8 NumOfLevel2Menu;
	U16 Level1String;
	const msg_club_submenu_struct* SubMenu;
}msg_club_menu_struct;


typedef struct
{
	U16 stringID;
	U16 childNum;
	void* childEntry;
}msg_club_entry_struct;

typedef struct
{
	U16 codeType;
	U8 codeNum[MSG_CLUB_LONG_CODE_LENGTH];
}msg_club_long_code_struct;

typedef struct
{
	U8 codeNum[MSG_CLUB_SHORT_CODE_LENGTH];
}msg_club_short_code_struct;

typedef struct
{
	U16 codeType;
	U8 codeNum[MSG_CLUB_LONG_CODE_LENGTH];
}msg_club_service_code_struct;

typedef struct
{
	U8 codeNum[MSG_CLUB_VOICE_CODE_LENGTH];
}msg_club_voice_code_struct;

typedef struct
{
	U16	stringID;
	U8	childNum;
	U8	childType;
	const void*	childEntry;
}msg_club_entry_menu_struct;

typedef struct
{
	U16	stringID;
	U8	serverType;
	U8	codeCU;
	U8 strInput_codeCU;
	msg_club_long_code_struct	codeDY;
	msg_club_long_code_struct	codeDB;
	msg_club_long_code_struct	codeZF;
	msg_club_short_code_struct	codeTD;
}msg_club_entry_dy_db_zf_struct;

typedef struct
{
	U16	stringID;
	U8	serverType;
	U8	codeCU;
	msg_club_short_code_struct	codeTD;
}msg_club_entry_td_struct;

typedef struct
{
	U16	stringID;
	U8	serverType;
	U8	codeCU;
	msg_club_long_code_struct	codeInfo;
}msg_club_entry_info_struct;

typedef struct
{
	U16	stringID;
	U8	codeType;
	U8	codeCU;
	msg_club_voice_code_struct	codeVoice;
}msg_club_entry_voice_struct;


typedef struct
{
	msg_club_service_code_struct	serverDY[MSG_CLUB_NETWORK_TOTAL];
	msg_club_service_code_struct	serverDB[MSG_CLUB_NETWORK_TOTAL];
	msg_club_service_code_struct	serverZF[MSG_CLUB_NETWORK_TOTAL];
	msg_club_service_code_struct	serverTD[MSG_CLUB_NETWORK_TOTAL];
}msg_club_service_num_struct;

#endif

#if defined(__MMI_SMART_MESSAGE_MT__) || ( defined(__MMI_SMART_MESSAGE_MO__)&& !defined(__MMI_MESSAGES_EMS__) )
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

#if defined(__MMI_SMART_MESSAGE_MT__)
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
#endif /*__MMI_SMART_MESSAGE_MT__*/
//#endif
#if defined(__MMI_SMART_MESSAGE_MT__) || ( defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
#endif

extern void PreEntryScrMessagesMenuList (void);
extern void EntryScrMessagesMenuList (void);
extern void InitMessagesApp (void);
extern void InitMessageProtocolEventHandlers (void);
extern void InitCBNVRAMData (void);
#if defined(__MMI_MESSAGES_TEMPLATE__)
extern U8	InitTemplates				(void) ;
#endif
extern void DeInitMessagesApp (void);
extern void RestoreMessagesSettings (void);	
extern U16 GetMessagesCurrScrnID (void);
extern void SetMessagesCurrScrnID (U16 scrnID);
extern void SetMessagesScrnIdToDelHistoryNodes (U16 ScrnId);
extern void SetMessagesScrnIdDelUpToHistoryNodes (U16 StartScrnId);
	
extern void mmi_msg_get_number_from_phb(U8[], U8[]);
extern void mmi_msg_set_phb_send_case (U8 PhbSmsInterfaceState);
extern void mmi_msg_save_msg (U8 status);
extern void DialVoiceMail (void);
extern S32 GetChildMenuIDIndexByParentMenuID (U16 parentMenuId, U16 childMenuId);
#ifdef __MMI_MESSAGES_CHAT__
extern U16 ObtainChatNumbersFromMessageTextWrapper(char * phNumber, S16);
extern PU8 GetNewChatSmsIndDisplayStr (void);
extern void DeleteChatMsg(S32 index);
extern void GetConcatenatedMessage(void);
#endif
#if defined(__MMI_MESSAGES_EMS__)
extern void EnableDisableAudioPlayback (void);
extern void DisplayUserDefinedImageList (void);
#ifdef __MMI_IMELODY_SUPPORT__
extern U8* GetEmsMelodyNameWithPathInFileSystem (U16 AudioId);
extern U8 DeleteEmsMelodyObject (U16 emsMelodyID);
extern void SendMelodiesByEMS (void);
extern void UpdateIMelodyDataForSyncDownloadFileSystemFunc (void);
#endif
#ifdef __MMI_FILE_MANAGER__
#define CATEGORY144_FMGR_PREVIEW		0x1000
extern void ShowCategory144Screen(U8* title,U16 title_icon,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,U32 object_type,S32 n_items,U8** object_titles,U16 *predefined_objects,MMI_BOOL is_short_name,U8** userdefined_objects,S32 *index,U8* history_buffer);
extern void mmi_ems_fm_send_by_ems (S8* emsObjectFileNameBuf, EMS_OBJECT_DISPLAY_STATE emsObjectType);
extern void mmi_ems_add_object_from_fm(void);
extern void mmi_ems_preview_object_from_fm (PU16 filePath, int is_short);
#endif // __MMI_FILE_MANAGER__
#endif // defined(__MMI_MESSAGES_EMS__)
#if defined (__MMI_VCARD__) || defined (__MMI_VCALENDAR__)
void mmi_ems_vobject_send_by_ems (U16 scrport, U16 dstport, S8* filepath, PsFuncPtr callback);
#endif
void mmi_msg_handle_call_ind (void);
U8 mmi_msg_check_interrupt(void);
U8 mmi_msg_need_clear_action_queue (void);
U8 mmi_msg_need_new_msg_ind (void);
U8 mmi_msg_need_new_msg_ind_call_end (void);
void mmi_msg_reset_new_msg_ind_call_end (void);
U8 mmi_msg_need_class0_msg_ind (void);
U8 mmi_msg_need_mem_full_ind (void);
BOOL mmi_msg_need_mem_exceed_ind (void);
#ifdef __MMI_MULTI_SIM__
U8 mmi_msg_is_mem_full_for_simid(U8 nSimID);
#endif
U8 mmi_msg_need_delivery_report_ind (void);
U8 mmi_msg_need_waiting_ind (void);

void mmi_msg_init_msg_data (void);
void mmi_msg_init_msg_process (void);
void mmi_msg_deinit_msg_data (void);


void mmi_msg_entry_msg_waiting_ind (void);
void mmi_msg_get_deliver_report_status (U32 *status, U16 *imgID, U16 *strID);
U8* mmi_msg_get_deliver_report_msg (void);
void mmi_msg_entry_deliver_report_ind (void);
void mmi_msg_entry_mem_exceed_ind (void);
void mmi_msg_entry_class0_msg_ind (void);
U8* mmi_msg_get_new_msg_ind_string (void);
void mmi_msg_entry_new_msg_ind (void);
void mmi_msg_entry_class0_msg_ind_when_kpl (U16 LSKStringtID, U16 LSKIconID, U16 RSKStringID, U16 RSKIconID);
void mmi_msg_entry_new_cb_ind_when_kpl (U16 LSKStringtID, U16 LSKIconID, U16 RSKStringID, U16 RSKIconID);
void mmi_msg_entry_new_cb_ind (void);
U8 mmi_msg_need_new_cb_ind (void);
U8* mmi_msg_get_msg_waiting_msg (void);
#ifdef __MMI_MESSAGES_CLUB__
void EntryMsgClub(void);
void mmi_msg_club_entry_level_1(void);
void mmi_msg_club_send_msg_finish (void);
extern U8 mmi_msg_club_get_item_total(void);
extern U8 mmi_msg_club_get_voice_item_index(void);
extern S8* mmi_msg_club_get_number_ct(void);
extern S8* mmi_msg_club_get_number_cu(void);
extern S8* mmi_msg_club_get_number_begin(void);
extern S8* mmi_msg_club_get_number_end(void);
extern S8* mmi_msg_club_get_number_other_ct(void);
extern S8* mmi_msg_club_get_number_other_cu(void);
extern S8* mmi_msg_club_get_code_sms(void);
extern S8* mmi_msg_club_get_code_dummy(void);
extern S8* mmi_msg_club_get_code_devide(void);
extern S8* mmi_msg_club_get_code_subscribe(void);
extern S8* mmi_msg_club_get_code_order(void);
extern S8* mmi_msg_club_get_code_explain(void);
extern S8* mmi_msg_club_get_code_cancel(void);
extern S8* mmi_msg_club_get_code_more(void);
#endif


#if defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
#if defined (MMI_ON_WIN32)
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
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
#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)) || defined(__MMI_SMART_MESSAGE_MT__)
/* under construction !*/
/* under construction !*/
#endif

#endif	/* _PIXCOM_MESSAGES_EXTERN_DCL_H */


