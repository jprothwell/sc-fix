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
 *	vCalendar.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for ¡K.
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
#ifndef _MMI_VCALENDAR_H
#define _MMI_VCALENDAR_H
#include "mmi_features.h"
#if defined(__MMI_VCALENDAR__)

#include "mmi_data_types.h"
#include "datetimetype.h"
/* if some header should be include before this one */

/* 
** Define
*/


/* 
** Typedef 
*/
typedef struct
{
	S8 	*FilePathPtr;
	S8	*FileNamePtr;
	U8	SrcType;
}vclndr_context_struct;

typedef struct
{
	U8	Days;
	U8	Repeat;
}vclndr_recurr_struct;

typedef enum
{
	VCLNDR_TYPE_EVENT = 0,
	VCLNDR_TYPE_TODO
}vclndr_type;

typedef enum
{
	VCLNDR_BEGIN,
	VCLNDR_VERSION,
	VCLNDR_VEVENT_BEGIN,
	VCLNDR_VTODO_BEGIN,
	VCLNDR_DTSTART,
	VCLNDR_DTEND,
	VCLNDR_DUE,	
	VCLNDR_SUMMARY,
	VCLNDR_DESCRIPTION,
	VCLNDR_RRULE,
	VCLNDR_DALARM,
	VCLNDR_AALARM,
	VCLNDR_VTODO_END,
	VCLNDR_VEVENT_END,
	VCLNDR_END,
	VCLNDR_TOTAL_FILEDS
}vclndr_filed_type;


typedef enum
{
	VCLNDR_SAVE_FROM_RECV,
	VCLNDR_SAVE_FROM_TDL
}vclndr_src_type;

typedef	enum
{
	VCLNDR_DAILY,
	VCLNDR_WEEKLY,
	VCLNDR_MONTHLY,
	VCLNDR_RECURR_TOTAL
}vclndr_recurr_type;



/* 
** Extern Global Variable
*/


/* 
** Extern Global Function
*/
extern void mmi_vclndr_init(void);
extern void mmi_vclndr_deinit(void);
extern void mmi_vclndr_free_buffer(void);


// entry function 
extern void mmi_vclndr_entry_filename(void);
extern void mmi_vclndr_entry_filename_option(void);
extern void mmi_vclndr_highlight_opt_done(void);
extern void mmi_vclndr_highlight_opt_input_method(void);
extern void mmi_vclndr_entry_recv_option(void);

extern void mmi_vclndr_highlight_forward(void);
extern void mmi_vclndr_entry_forward(void);


extern void mmi_vclndr_save_file(void);
extern void mmi_vclndr_save_file_done(S32 result);
extern S32 mmi_vclndr_save_obj_to_file(void);
extern void mmi_vclndr_save_obj_to_file_callback(void* info);
extern S32 mmi_vclndr_save_tdl_to_file(void);


// for tdl
extern void mmi_vclndr_highlight_recv_opt_save_to_file(void);
extern void mmi_vclndr_recv_save_to_tdl(void);
extern void mmi_vclndr_highlight_recv_opt_save_to_tdl(void);
extern void mmi_vclndr_goback_1_history(void);

// for sms
#if !defined(__MMI_MESSAGES_NO_SEND_BY_SMS_OPTION__)
extern void mmi_vclndr_sms_highlight_forward(void);
extern void mmi_vclndr_sms_forward(void);
extern void mmi_vclndr_sms_forward_callback(void* flag);
#endif
// for irda
extern void mmi_vclndr_irda_highlight_forward(void);
extern void mmi_vclndr_irda_forward(void);
extern void mmi_vclndr_irda_forward_callback(void *buf_p, U32 result );

/* for Bluetooth */
extern void mmi_vclndr_opp_highlight_forward(void);
extern void mmi_vclndr_opp_forward(void);
extern void mmi_vclndr_opp_forward_callback(void *buf_p, U32 result);
// for mms
extern void mmi_vclndr_mms_highlight_forward(void);
extern void mmi_vclndr_mms_forward(void);

// for email
extern void mmi_vclndr_email_highlight_forward(void);

// for fmgr
extern void mmi_vclndr_fmgr_highlight_forward(void);
extern void mmi_vclndr_fmgr_forward(void);
extern void mmi_vclndr_fmgr_select_path_done(void *path, int is_short);
extern void mmi_vclndr_fmgr_recv(S8* file_path_name);
extern void mmi_vclndr_fmgr_sms_forward(S8 *path);

// utilities 
extern void mmi_vclndr_popup_not_support(void);
extern MYTIME mmi_vclndr_uti_get_time(S8* strTime);
//extern S8* mmi_vclndr_uti_get_value(S8* strString);
extern U8 mmi_vclndr_uti_get_recurr(S8* strRRule, vclndr_recurr_struct* recurr);
extern U8 mmi_vclndr_uti_get_days(S8* strDays);
extern U8 mmi_vclndr_uti_check_field(S8* string);
//extern U8 mmi_vclndr_uti_cmp_filed(S8* srcString, const S8* refString);
extern U8 mmi_vclndr_uti_get_param(S8 *source, S8 *para_name, S8 *result, S32 max_len);
extern U8 mmi_vclndr_uti_get_encoding_type(S8* string);
extern U8 mmi_vclndr_uti_is_file_exist(S8* file_path_name);
extern void mmi_vclndr_uti_del_default_file(void);
extern void mmi_vclndr_uti_get_default_filename(S8* result);
extern void mmi_vclndr_uti_remove_ws(S8*, S8*);
extern U8 mmi_vclndr_uti_get_charset(S8* string);


// temporary usage
extern S32 utf8_to_ucs2 (U16 *ucs2, U8 *utf8);
extern S32 ucs2_to_utf8 (U8 *utf8, U16 ucs2);
extern S32 ucs2_to_utf8_len(U16 ucs2);
extern S32 utf8_to_ucs2_string(U8 *dest,int dest_size,U8* src);
extern S32 ucs2_to_utf8_string(U8 *dest,S32 dest_size,U8* src);
extern S32 utf8_strlen(const U8 *s);
	
#endif
#endif /*END #ifndef _MMI_VCALENDAR_H*/


