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
 * mmi_msg_context.c
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
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _MMI_MSG_CONTEXT_C
#define _MMI_MSG_CONTEXT_C

#include "customer_ps_inc.h"
#include "mmi_msg_context.h" 

#ifdef SMS_FOR_NEPTUNE
msg_msgbox_info_struct msgbox_info;
msg_msgbox_entry_struct msgbox_newmsg;
#else
unsigned short mmi_frm_sms_inbox_list[MMI_SMS_MAX_MSG_NUM];
unsigned short mmi_frm_sms_outbox_list[MMI_SMS_MAX_MSG_NUM];
unsigned short mmi_frm_sms_drafts_list[MMI_SMS_MAX_MSG_NUM];

mmi_frm_sms_msgbox_struct mmi_frm_sms_msg_box[MMI_SMS_MAX_MSG_NUM];
mmi_frm_sms_index_struct mmi_frm_sms_index_list[MMI_SMS_MAX_MSG_NUM];
#endif

PendingSaveSendDataStruct PendingSaveSendData;

unsigned char mmi_msg_get_seg_num(void)
{
	return MMI_SMS_MAX_MSG_SEG;
}

#ifdef SMS_FOR_NEPTUNE
unsigned char mmi_msg_get_msgbox_entry_size(void)
{
	return sizeof(msg_msgbox_entry_struct);
}
#endif

unsigned short mmi_msg_get_msgbox_size(void)
{
	return MMI_SMS_MAX_MSG_NUM;
}



#endif

