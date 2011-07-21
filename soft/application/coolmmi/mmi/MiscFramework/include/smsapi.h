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
 *  SMSApi.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#include "messagesmiscell.h"
typedef enum
{
	MMI_FRM_SMS_APP_NOBOX	=	0x00,
	MMI_FRM_SMS_APP_UNREAD	=	0x01,
	MMI_FRM_SMS_APP_INBOX	=	0x02,
	MMI_FRM_SMS_APP_OUTBOX	=	0x04,
	MMI_FRM_SMS_APP_DRAFTS	=	0x08,
	MMI_FRM_SMS_APP_AWAITS	=	0x10,
	MMI_FRM_SMS_APP_DATA	=	0x20
}mmi_frm_sms_app_msgbox_enum;



typedef enum
{
	MMI_FRM_SMS_OK,
	MMI_FRM_SMS_ERROR,
	MMI_FRM_SMS_RETURN,
	MMI_FRM_SMS_END,
	MMI_FRM_SMS_ABORT,
	MMI_FRM_SMS_FDLFAIL,
	MMI_FRM_SMS_NUMEMPTY,
	MMI_FRM_SMS_DATAEMPTY,
	MMI_FRM_SMS_DATAINVALID,
	MMI_FRM_SMS_DATAEXCEED,
	MMI_FRM_SMS_MEMFULL,
	MMI_FRM_SMS_NOTREADY
}mmi_frm_sms_result;

typedef enum
{
	MMI_FRM_SMS_SC		=	0x01,
	MMI_FRM_SMS_DA		=	0x02,
	MMI_FRM_SMS_FDL		=	0x04,
	MMI_FRM_SMS_SCR		=	0x08
}mmi_frm_sms_action_enum;

typedef enum
{
	MMI_FRM_SMS_PORT		=	0x0001,
	MMI_FRM_SMS_CLASS	=	0x0002,
	MMI_FRM_SMS_REPLY	=	0x0004,
	MMI_FRM_SMS_SAVE		=	0x0008,
	MMI_FRM_SMS_SAVE_AFTER_FAIL	=	0x0010,
	MMI_FRM_SMS_RETURN_STRING	=	0x0020,
	MMI_FRM_SMS_MORE		=	0x0040,
	MMI_FRM_SMS_NUM_SCR	=	0x0080,
	#ifdef __MMI_MULTI_SIM__
	MMI_FRM_SMS_DISP_SIM_OPT = 0x0200,
	#endif
}mmi_frm_sms_send_enum;


typedef struct
{
	U16 srcport;
	U16 desport;
	U16 replyindex;
	U16 stringlength;

	U8 dcs;
	U8 dcs_class;
	U8 profile;
	U8 status;
	
	mmi_frm_sms_action_enum sendcheck;
	mmi_frm_sms_send_enum sendrequire;
	
	U8 number[MAX_DIGITS_SMS];
	S8* filename;
	S8* string;
}mmi_frm_sms_send_struct;

typedef void (*PsFuncPtrU16)(void*, module_type, U16); 
typedef U8 (*PsFuncPtrFuncPtr)(void*, void*); 

#define MMI_FRM_SMS_INVALID_INDEX	0xffff
#define MMI_FRM_SMS_INVALID_NUM		0xff
#define MMI_FRM_SMS_LMS_TIME_OUT		1000*10
#define MMI_FRM_SMS_MSG_LEN			160

#if defined (__MMI_MESSAGES_COPY__)||defined (__MMI_MESSAGE_DIFFERENTIATE_STORAGE__)
/*if segments of one concatenated message have different storage, 
	the storage of this message is determined by this value*/
#define MMI_FRM_SMS_PREFER_STORAGE	SMSAL_SM
#endif

void mmi_frm_sms_get_phb_number (U8 name[], U8 number[]);
extern BOOL mmi_frm_sms_check_dst_number(U8 *num, U8 numLen);

void mmi_frm_sms_reg_interrupt_check (module_type mod,  /*U16*/U32 msgid, PsFuncPtrU16 callback);   

void mmi_frm_sms_reg_msg_check (PsExtPeerIntFuncPtr action, PsFuncPtrFuncPtr callback);
void mmi_frm_sms_read_sms (PsFuncPtrU16 callback, module_type mod_src, U8 type, U16 index);
void mmi_frm_sms_send_sms (PsFuncPtrU16 callback, module_type mod_src, mmi_frm_sms_send_struct* sendData);
void mmi_frm_sms_get_sms (PsFuncPtrU16 callback, module_type mod_src, U16 index);
void mmi_frm_sms_save_sms (PsFuncPtrU16 callback, module_type mod_src, mmi_frm_sms_send_struct* sendData);
void mmi_frm_sms_delete_sms (PsFuncPtrU16 callback, module_type mod_src, U8 type, U16 index);
void mmi_frm_sms_reg_port (PsFuncPtrU16 callback, module_type mod_src, U16 port_num, BOOL enable, BOOL withdata, PsFuncPtrU16 regcallback);

U8 mmi_frm_sms_check_action_pending(void);
void mmi_frm_sms_delete_action_pending(void);
void mmi_frm_sms_delete_screen_history(void);
U8 mmi_frm_sms_check_complete(U8 type, U16 index);
U8 mmi_frm_sms_get_status(U8 type, U16 index);
U8 mmi_frm_sms_get_dcs (U8 type, U16 index);
U8* mmi_frm_sms_get_address (U8 type, U16 index);
U8* mmi_frm_sms_get_timestamp (U8 type, U16 index);
void mmi_frm_sms_get_list_index(U8* type, U16* index, U16 msgbox_index);
U8 mmi_frm_sms_get_sms_size (U8 type, U16 index);
U8 mmi_frm_sms_get_sms_received_size (U8 type, U16 index);
U16 mmi_frm_sms_get_sms_list_size (U8 type);
U8 mmi_frm_sms_get_data_check (U8 index);
void mmi_frm_sms_set_data_check (U8 index, U8 set);
void mmi_frm_sms_swap_list_index(U8 type, U16 index1, U16 index2);
#if defined (__MMI_MESSAGES_COPY__) ||defined(__MMI_MESSAGE_DIFFERENTIATE_STORAGE__)
U8 mmi_frm_sms_get_storage(U8 type, U16 index);
#endif
#ifdef __MMI_MESSAGES_COPY__ 
void mmi_frm_sms_copy_abort(void);
#endif
U8 mmi_frm_sms_check_awaited_complete_msg  (U8 index);
U16 mmi_frm_sms_get_index (U16 index, U8 *index_bitmap);
void mmi_frm_sms_init_msgbox (void);
void mmi_frm_sms_set_profile_active (PsFuncPtrU16 callback, module_type mod_src, U8* index);
void mmi_frm_sms_get_profile_list (PsFuncPtrU16 callback, module_type mod_src, U8 nSimID);
void mmi_frm_sms_get_sc_addr (PsFuncPtrU16 callback, module_type mod_src, U8 index);
void mmi_frm_sms_set_sc_addr (PsFuncPtrU16 callback, module_type mod_src, U8 index, S8 scAddr[]);
void mmi_frm_sms_get_profile_detail (PsFuncPtrU16 callback, module_type mod_src, U8 index);
void mmi_frm_sms_set_profile_detail (PsFuncPtrU16 callback, module_type mod_src, void* profile);
void mmi_frm_sms_get_common_settings (PsFuncPtrU16 callback, module_type mod_src);
void mmi_frm_sms_set_common_settings (PsFuncPtrU16 callback, module_type mod_src, U8* setting);
#ifdef __MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__
void mmi_frm_sms_get_memory_status (PsFuncPtrU16 callback, module_type mod_src);
void mmi_frm_sms_get_preferred_storage (PsFuncPtrU16 callback, module_type mod_src);
void mmi_frm_sms_set_preferred_storage (PsFuncPtrU16 callback, module_type mod_src, U8 storage);
#endif
#ifdef __MMI_GPRS_FEATURES__
void mmi_frm_sms_get_preferred_bearer (PsFuncPtrU16 callback, module_type mod_src);
void mmi_frm_sms_set_preferred_bearer (PsFuncPtrU16 callback, module_type mod_src, U8 bearer);
#endif
void mmi_frm_sms_get_mailbox_info (PsFuncPtrU16 callback, module_type mod_src);
void mmi_frm_sms_set_mailbox_info (PsFuncPtrU16 callback, module_type mod_src, void* mailbox);
#ifdef __MMI_MESSAGES_COPY__
void mmi_frm_sms_copy_sms(PsFuncPtrU16 callback, module_type mod_src, U8 type, U16 index, U8 action,U8 dst_storage);
#endif

void mmi_frm_sms_init (void);

void             InitEMSDataStruct              (void) ;
void             DeInitEMSDataStruct            (void) ;
extern void      ReleaseEMSViewBuffer           (void) ;
extern void      ReleaseEMSEditBuffer           (void) ;
extern U8        CopyEMSViewToEditBuffer        (void) ;
extern BOOL mmi_frm_sms_check_SC_number(U8 *num, U8 numLen);
extern void mmi_frm_sms_sc_empty (void);
extern void mmi_frm_sms_go_back_from_send (void);

#ifdef __MMI_MULTI_SIM__
extern U8 mmi_frm_msg_get_simid_by_l4_index(U16 l4_index);
extern void mmi_msg_set_msg_sim_id(U8 nSimID);
extern BOOL mmi_frm_msg_get_simid_by_list_index(U16 list_index);
extern void mmi_msg_delete_dm_msg_req(U8 side, U16 type, U16 index);
extern void mmi_frm_sms_dm_delete_sms(U8 side, PsFuncPtrU16 callback, module_type mod_src, U16 type, U16 index);
#endif


