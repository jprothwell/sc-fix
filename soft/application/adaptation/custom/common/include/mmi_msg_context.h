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
 *	mmi_msg_context.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for MMI message application context.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef _MMI_MSG_CONTEXT_H
#define _MMI_MSG_CONTEXT_H


/* if some header should be include before this one */
#ifndef _CUSTOMER_PS_INC_H
#error "customer_ps_inc.h should be included before mmi_msg_context.h"
#endif
#include "mmi_features.h"

#ifdef __MMI_MULTI_SIM__
#define MMI_SMS_MAX_MSG_NUM	 (( PHONE_SMS_ENTRY_COUNT/MMI_SIM_NUMBER + SIM_SMS_ENTRY_COUNT ) * MMI_SIM_NUMBER)
#else
#define MMI_SMS_MAX_MSG_NUM	 ( PHONE_SMS_ENTRY_COUNT/MMI_SIM_NUMBER + SIM_SMS_ENTRY_COUNT )
#endif

#ifdef SMS_FOR_NEPTUNE
typedef struct
{
	unsigned short ref;
	unsigned char totalseg;
	unsigned char startseg;
}msg_concat_info_struct;

typedef struct
{
	unsigned char status;	
	unsigned char type;
	unsigned char timer;
	unsigned char mti;
	unsigned char number[20];
	unsigned char timestamp[6];
	unsigned short index[MMI_SMS_MAX_MSG_SEG];
	msg_concat_info_struct concatinfo;
}msg_msgbox_entry_struct;

typedef struct
{
	unsigned short totalinbox;
	unsigned short totaloutbox;
	unsigned short totaldraftbox;
	unsigned short totalawaited;
	msg_msgbox_entry_struct msgentry[MMI_SMS_MAX_MSG_NUM];
}msg_msgbox_info_struct;
#endif


typedef struct 
{
	unsigned char		totalSegments ;
	unsigned char		presentSegments ;	//use for send waiting response
	unsigned char		nextSegment ;		//use for save one by one
	unsigned char		dcs;
	unsigned char		udhi;
	unsigned char		mti;
	unsigned char		st;

	// Params for Sending only
	char*			phNumber ;
	unsigned char		number[(20+2)*2];
	unsigned char		status ;
	unsigned char		timestamp[6];

	//prepare for new interface, 0416
	unsigned short		msgboxindex;
	
    	// Take a contiguous buffer so that we can re-use!
	unsigned char		TPUD[MMI_SMS_MAX_MSG_SEG * 160];
	unsigned char*		TPUD_p[MMI_SMS_MAX_MSG_SEG];
	unsigned char		TPUDLen[MMI_SMS_MAX_MSG_SEG];

	//CSD Joy added for error SIM handler, 10/07
	unsigned char		TPUD_udhi[MMI_SMS_MAX_MSG_SEG];
	unsigned short		l4index[MMI_SMS_MAX_MSG_SEG];

}PendingSaveSendDataStruct;

#ifdef SMS_FOR_NEPTUNE
extern msg_msgbox_info_struct msgbox_info;
extern msg_msgbox_entry_struct msgbox_newmsg;
#else
typedef struct
{
	/*mti and status*/
	unsigned char msgtype;
	unsigned char storageType;

	unsigned char startseg;
	unsigned char totalseg;

	unsigned char timestamp[7];
	unsigned char number[21];

	unsigned short ref;
	unsigned short startindex;
}mmi_frm_sms_msgbox_struct;


typedef struct
{
	unsigned char segment;
	unsigned char prevtype;

	/*next non-empty segment, 0xffff for last segment*/
	unsigned short nextindex;
	unsigned short previndex;
}mmi_frm_sms_index_struct;

extern unsigned short mmi_frm_sms_inbox_list[MMI_SMS_MAX_MSG_NUM];
extern unsigned short mmi_frm_sms_outbox_list[MMI_SMS_MAX_MSG_NUM];
extern unsigned short mmi_frm_sms_drafts_list[MMI_SMS_MAX_MSG_NUM];

extern mmi_frm_sms_msgbox_struct mmi_frm_sms_msg_box[MMI_SMS_MAX_MSG_NUM];
extern mmi_frm_sms_index_struct mmi_frm_sms_index_list[MMI_SMS_MAX_MSG_NUM];
#endif
extern PendingSaveSendDataStruct PendingSaveSendData;
extern unsigned char	mmi_msg_get_seg_num(void);
extern unsigned short	mmi_msg_get_msgbox_size(void);
#ifdef SMS_FOR_NEPTUNE
extern unsigned char	mmi_msg_get_msgbox_entry_size(void);
#endif
#endif

