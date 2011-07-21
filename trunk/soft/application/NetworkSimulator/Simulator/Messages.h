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

/**************************************************************

	FILENAME	: Messages.h

  	PURPOSE		: Messages application simulator


	REMARKS		: nil

	AUTHOR		: Gurinder Singh Chhabra

	DATE		: July 1, 2003.

**************************************************************/

#ifndef _MESSAGES_H
#define _MESSAGES_H

typedef double 			DOUBLE;
typedef float			FLOAT;
typedef char			S8;			
typedef char			*PS8;		
typedef unsigned char   U8;			
typedef unsigned char	*PU8;	

typedef short			S16; 		
typedef short			*PS16; 		
typedef unsigned short  U16; 		
typedef unsigned short  *PU16;
typedef unsigned int	U32;

// Messages L4 defs -- definitions imported from L4
// This part to be moved to a separate file
// Begin -- L4 defs

#define CM_SMS_MAX_MSG_NUM 470  // Add by zhuoxz, 2009-10-12

#define SMSAL_MAX_MSG_LEN			(160)
#define MAX_CC_ADDR_LEN				41
#define SMSAL_MAX_SEG               (4)
#define MAX_MELODY_FILE_NAME 50

#define LOCAL_PARA_HDR \
   U8	ref_count; \
   U16	msg_len;

#define SMSAL_INVALID_INDEX		0xffff

#define kal_uint8  U8
#define kal_uint16 U16
#define kal_uint32 U32

typedef unsigned short          kal_wchar;

typedef enum {
   KAL_FALSE,
   KAL_TRUE
} kal_bool;

typedef struct l4c_result_struct
{
   kal_uint8   flag;    /*l4c_result_enum*/
   kal_uint16   cause;

} l4c_result_struct;

typedef struct {
    kal_uint16 ref; /* concat. message reference*/
    kal_uint8 total_seg; /* total segments*/
    kal_uint8 seg; /* indicate which segment*/
} smsal_concat_struct;

typedef struct
{
   kal_uint8 type;
   kal_uint8 length;
   kal_uint8 number[MAX_CC_ADDR_LEN];
} l4c_number_struct;

typedef struct {
		LOCAL_PARA_HDR
		l4c_result_struct	result;
		kal_uint8	mr;
		kal_uint8	scts[7];
		kal_uint32	concat_info[(sizeof(smsal_concat_struct)+3)/4];
} mmi_sms_send_command_rsp_struct;

typedef struct l4_name_struct
{
	kal_uint8 name_length;
	kal_uint8 name_dcs;
	kal_uint8 name[32];
} l4_name_struct;

	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		kal_uint16	cause;
		kal_uint8	mr;
		kal_uint8	scts[7];
		kal_uint8	seq_num;
	} mmi_sms_send_msg_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	profile;
		kal_uint16	reply_index;
		l4c_number_struct	dest;
		kal_uint8	para;
		l4c_number_struct	sc_addr;
		kal_uint8	vp;
		kal_uint8	pid; /* smsal_pid_enum */
		kal_uint8	udhi;
		kal_uint8	dcs; /* smsal_dcs_enum */
		kal_uint16	length;
		kal_uint8	msg[SMSAL_MAX_MSG_LEN];
	} mmi_sms_send_msg_req_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	profile;
		kal_uint8	status;
		l4c_number_struct	dest;
		kal_uint8	para;
		l4c_number_struct	sc_addr;
		kal_uint8	vp;
		kal_uint8	pid; /* smsal_pid_enum */
		kal_uint8	udhi;
		kal_uint8	dcs; /* smsal_dcs_enum */
		kal_uint16	length;
		kal_uint8	msg[SMSAL_MAX_MSG_LEN];
	} mmi_sms_set_msg_req_struct;

	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	pin[9];
		kal_uint8	currency[3];
		kal_uint32	ppu[18];
	} mmi_call_price_unit_req_struct;

	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint32	result;
		kal_uint8	currency[3];
		kal_uint32	ppu[18];
	} mmi_call_price_unit_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		kal_uint16	cause;
		kal_uint16	index;
		kal_uint8	status;
		kal_uint8	storage_type;
	} mmi_sms_set_msg_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint16	dest_port;
		kal_uint16	src_port;
		kal_uint16	indx;
		kal_uint8	storage;
		l4c_number_struct	oa_num;
		l4_name_struct	alpha_id;
		kal_uint8	mti;
		kal_uint8	scts[7];
		kal_uint32	concat_info[(sizeof(smsal_concat_struct)+3)/4];
	} mmi_sms_new_msg_index_ind_struct;

	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint16	index;
		kal_bool	change_status;
	} mmi_sms_get_msg_req_struct;

typedef struct l4_addr_bcd_struct
{
	kal_uint8 addr_length;
	kal_uint8 addr_bcd[41];
} l4_addr_bcd_struct;

	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		kal_uint16	cause;
		/* smsal_concat_struct concat_info; */ /* delete by zhuoxz, 2009-10-22 */
		kal_uint8	mti;
		kal_uint8	status;
		kal_uint8	storage_type;
		kal_uint8	fo;
		kal_uint8	scts[7];
		l4c_number_struct	sca_number;
		l4c_number_struct	addr_number;
		kal_uint8	pid;
		kal_uint8	dcs;
		kal_uint8	ori_dcs;
		kal_uint8	vp;
		kal_uint8	mr;
		kal_uint8	dt[7];
		kal_uint8	st;
		kal_uint16	no_msg_data;
		kal_uint8	msg_data[1];
	} mmi_sms_get_msg_rsp_struct;
	

/* For InBOX / OutBox Structure*/
typedef struct {
    kal_uint8 dcs;    
    kal_uint8 mti; /* smsal_mti_enum */
    kal_uint8 status; /* Status of message*/
    kal_uint8 storage_type;
    kal_uint16 index; /* Index of message*/
    //l4_addr_bcd_struct oa;
    union {
       l4c_number_struct  number;
       l4_addr_bcd_struct bcd;
    } oa;    /* Originating address*/
    kal_uint8 scts[7];
    smsal_concat_struct concat_info; /* Concatenaged Infomation*/
} smsal_inbox_struct;


typedef struct {
    kal_uint16 msg_count; /* Message Count*/
    smsal_inbox_struct inbox_list[10];
} smsal_inbox_list_struct;

typedef struct {
    kal_uint8 dcs;
    kal_uint8 status; /* Status of message*/
    kal_uint8 storage_type;
    kal_uint16 index; /* Index of message*/
    //l4_addr_bcd_struct da;
    union {
       l4c_number_struct  number;
       l4_addr_bcd_struct bcd;
    } da; /* Destination address*/    
    smsal_concat_struct concat_info; /* Concatenaged Infomation*/
} smsal_outbox_struct;

typedef struct {
    kal_uint16 msg_count; /* Message Count*/
    smsal_outbox_struct outbox_list[10];
} smsal_outbox_list_struct;

	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	concat_flag;
		kal_uint8	status; /* smsal_status_enum */
		kal_uint16	start_index;
		kal_uint8	no_msg;
	} mmi_sms_get_msg_list_req_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		smsal_inbox_list_struct	inbox;
		smsal_outbox_list_struct	outbox;
	} mmi_sms_get_msg_list_rsp_struct;

	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		kal_bool	is_sim_card_change;
		kal_uint16	in_sim_no;
		kal_uint16	in_me_no;
		kal_uint16	out_sim_no;
		kal_uint16	out_me_no;
		kal_uint16	unread_msg_num;
		kal_uint16	total_sim_num;
		kal_uint16	total_me_num;
	} mmi_sms_get_msg_num_rsp_struct;

	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	del_flag; /* smsal_del_flag_enum */
		kal_uint16	index;
		kal_uint8	del_bitmap[480];
	} mmi_sms_del_msg_req_struct;

	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		kal_uint16	cause;
		kal_uint8	del_flag; /* smsal_del_flag_enum */
		kal_uint16	index;
		kal_uint8	storage_type;
		kal_uint8	del_bitmap[480];
	} mmi_sms_del_msg_rsp_struct;

typedef struct {
    kal_uint8 mem1; /* Storage type for Memory1*/
    kal_uint8 mem2; /* Storage type for Memory2*/
    kal_uint8 mem3; /* Storage type for Memory3*/
    kal_uint8 used1;
    kal_uint8 used2;
    kal_uint8 used3;
    kal_uint8 total1;
    kal_uint8 total2;
    kal_uint8 total3;
} smsal_prefer_storage_struct;

	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		smsal_prefer_storage_struct	param;
	} mmi_sms_set_preferred_storage_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		smsal_prefer_storage_struct	info;
	} mmi_sms_get_preferred_storage_rsp_struct;

	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	mem1; /* smsal_storage_enum */
		kal_uint8	mem2; /* smsal_storage_enum */
		kal_uint8	mem3; /* smsal_storage_enum */
	} mmi_sms_set_preferred_storage_req_struct;

////////////////////////////////////////////////////////
/////////////audio related messages/////////////////////
////////////////////////////////////////////////////////
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	sound_id;
		kal_uint8	style; /* audio_play_style_enum */
	} mmi_eq_play_audio_req_struct;

	typedef struct
	{
		LOCAL_PARA_HDR
		kal_wchar	file_name[MAX_MELODY_FILE_NAME];
		kal_uint8	style;
		kal_uint16	identifier;
	} mmi_eq_play_audio_by_name_req_struct;


//abhi:start:voicemail
typedef struct {
   l4_name_struct name;
   l4c_number_struct number_info;
} l4c_sms_mailbox_info_struct;

	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		kal_uint8	mailbox_num;
		kal_uint8	alpha_length;
		l4c_sms_mailbox_info_struct	mailbox_info[2];
	} mmi_sms_get_mailbox_address_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	mailbox_num;
		l4c_number_struct	num_addr;
	} mmi_sms_set_mailbox_address_req_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		kal_uint16	cause;
	} mmi_sms_set_mailbox_address_rsp_struct;

	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	startup_seq_num;
		kal_uint8	mti;
		kal_uint8	status;
                kal_uint8	storage_type;
		kal_uint8	fo;
		kal_uint16	index;
		kal_uint8	scts[7];
		l4c_number_struct	addr_number;
		kal_uint8	pid;
		kal_uint8	dcs;
		kal_uint8	ori_dcs;
		kal_uint8	vp;
		kal_uint8	mr;
		kal_uint8	dt[7];
		kal_uint8	st;
		kal_uint32	concat_info;
		kal_uint16	no_msg_data;
		kal_uint8	msg_data[1];
	} mmi_sms_startup_read_msg_ind_struct;

//abhi:end
// End -- L4 defs

//#define MAX_MESG					10
#define INBOX_FILE_INDEX				0
#define OUTBOX_FILE_INDEX				1

#define INBOX_FILE					"inbox.txt"
#define OUTBOX_FILE					"outbox.txt"
#define MAX_MESSAGES_LIMIT			50


void readSMSFile(U8 flag );
void writeSMSFile(U8 flag );

//abhi:start:for edit of voicemail number
void ReadVoiceMailFile();
void WriteVoiceMailFile();
//abhi:end


//JP: 20050514 SMS Box implementation
#define SMS_BOX_FILE				"sms_box_file"
#define MAX_SMS_BOX_LIMIT			100
void write_sms_box_file(void);
void read_sms_box_file(void);

typedef struct tag_sms_box_entry
{
		kal_uint8	mti;
		kal_uint8	status;
		kal_uint8	fo;
		kal_uint8	scts[7];
		l4c_number_struct	sca_number;
		l4c_number_struct	addr_number;
		kal_uint8	pid;
		kal_uint8	dcs;
		kal_uint8	ori_dcs;
		kal_uint8	vp;
		kal_uint8	mr;
		kal_uint8	dt[7];
		kal_uint8	st;
		kal_uint32	concat_info;
		kal_uint16	length;
		kal_uint8	msg[500];		//JP
}sms_box_entry;
typedef struct tag_sms_box_struct
{
	kal_uint8		total_box_entries[2];
	sms_box_entry	sms_box_entries[MAX_SMS_BOX_LIMIT*2];
}sms_box_struct;

typedef struct
{
	LOCAL_PARA_HDR
	kal_uint8	fo;
	kal_uint8	scts[7];
	l4c_number_struct	sca_number;
	l4c_number_struct	oa_number;
	kal_uint8	pid; /* smsal_pid_enum */
	kal_uint8	dcs; /* smsal_dcs_enum */
	kal_uint16	index;
	kal_uint16	dest_port;
	kal_uint16	src_port;
	kal_uint8	mti;
	kal_uint8	display_type;
	kal_uint8	storage_type;
	kal_uint32	concat_info;
	kal_uint16	no_msg_data;
	kal_uint8	msg_data[1];
} mmi_sms_deliver_msg_ind_struct;

typedef struct
{
	LOCAL_PARA_HDR
	kal_uint16	sn;
	kal_uint16	mi;
	kal_uint8	dcs;
	kal_uint8	page;
	kal_uint8	pages;
	kal_uint16	msg_length;
	kal_uint8	msg_data[1];
} mmi_cb_msg_ind_struct;

typedef struct
{
	LOCAL_PARA_HDR
	kal_uint8	profile_no;
} mmi_sms_get_profile_params_req_struct;

typedef struct
{
	LOCAL_PARA_HDR
	kal_bool	result;
	kal_uint8	pid;
	kal_uint8	dcs;
	kal_uint8	vp;
	l4c_number_struct	sc_addr;
	l4_name_struct	profile_name;
} mmi_sms_get_profile_params_rsp_struct;

typedef struct
{
	LOCAL_PARA_HDR
	kal_uint16	para_ind;
	kal_uint8	profile_no;
	kal_uint8	pid;
	kal_uint8	dcs;
	kal_uint8	vp;
	l4c_number_struct	sc_addr;
	l4_name_struct	profile_name;
} mmi_sms_set_profile_params_req_struct;

typedef struct
{
	LOCAL_PARA_HDR
	kal_bool	result;
	kal_uint16	cause;
} mmi_sms_set_profile_params_rsp_struct;

typedef struct {
   kal_uint16 mid[40];
   kal_uint16 dcs[15];
   kal_uint16 mir[30];
   kal_uint8 mid_mask[40];
   kal_uint8 mir_mask[15];
   kal_uint8 dcs_mask[15];
   kal_uint8 para_ind; /* used in set_cb_mode_req only */ // lan, cha

   kal_bool is_all_language_on;

   /* used in get_cb_mode_cnf */
   kal_bool     is_cb_ready;
   kal_bool     is_sim_change;
} l4c_cb_info_struct;
typedef struct
{
	LOCAL_PARA_HDR
	kal_bool	result;
	kal_uint8	mode;
	kal_uint8	total_mid_num;
	kal_uint8	total_mir_num;
	kal_uint8	total_dcs_num;
	l4c_cb_info_struct	info;
} mmi_sms_get_cb_mode_rsp_struct;

typedef struct
{
	LOCAL_PARA_HDR
	l4c_cb_info_struct	info;
} mmi_sms_set_cb_mode_req_struct;

typedef enum
{
   SMSAL_CB_MI          = 0x01,
   SMSAL_CB_MIR         = 0x02,
   SMSAL_CB_DCS         = 0x04,
   SMSAL_CB_MI_MASK     = 0x08,
   SMSAL_CB_MIR_MASK    = 0x10,
   SMSAL_CB_DCS_MASK    = 0x20,
   SMSAL_CB_ALL_LANG    = 0x40
} smsal_cb_para_enum;

#endif	//	_MESSAGES_H
