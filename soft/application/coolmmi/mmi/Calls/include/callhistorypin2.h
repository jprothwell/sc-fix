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
 *  CallHistoryPin2.h
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
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/Calls/include/CallHistoryPin2.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:57:04   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:37:30   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:23:48   zhangxb
 * Initial revision.
 * 
 *    Rev 1.0   Nov 06 2006 17:32:22   zhangxb
 * Initial revision.
 * 
 *    Rev 1.2   Sep 19 2006 19:10:24   zhoumn
 * 更新了部分代码、文档
 * Revision 1.1.1.1  2006/06/08 12:40:18  liuruijin
 * no message
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: Calls_Defs.h

  	PURPOSE		: Call application 

 

	DATE		: Jan 15,02

**************************************************************/

#ifndef _COOLSAND_CALLHISTORYPIN2_H
#define _COOLSAND_CALLHISTORYPIN2_H



#define MAX_SIM_PASSWD_LEN       9

#ifdef __UCS2_ENCODING
#define SIM_PASS_LEN	9//((ENCODING_LENGTH * MAX_SIM_PASSWD_LEN	) + 2)
#else
#define SIM_PASS_LEN	MAX_SIM_PASSWD_LEN
#endif 

#ifndef MMI_ON_HARDWARE_P

#ifndef __NEWSIMULATOR
typedef enum
{
   TYPE_NO_REQUIRED,
   TYPE_PHONELOCK,
   TYPE_CHV1,
   TYPE_CHV2,
   TYPE_UBCHV1,
   TYPE_UBCHV2,
   TYPE_NP,
   TYPE_NSP,
   TYPE_SP,
   TYPE_CP,
   TYPE_IMSI_LOCK,
   TYPE_PERSONALIZATION_KEY,
   TYPE_CHV1_BLOCKED,
   TYPE_CHV2_BLOCKED,
   TYPE_SIM_CARD_BLOCKED,
   TYPE_UBCHV2_BLOCKED,
   TYPE_UBCHV1_CHV1_NOT_LOCK,
   TYPE_UNSPECIFIED
}MSG_MMI_SMU_SECURITY_TYPE_ENUM;


typedef struct
{
	MSG_MMI_SMU_SECURITY_TYPE_ENUM	pin_type; /* smu_security_type_enum */
	U8	pin[MAX_SIM_PASSWD_LEN];
	U8	new_pin[MAX_SIM_PASSWD_LEN];
} MSG_MMI_SMU_VERIFY_PIN_REQ_STRUCT ;

typedef struct
{
	pBOOL		result;
	U8			next_type; /* smu_security_type_enum */
} MSG_MMI_SMU_VERIFY_PIN_RSP_STRUCT ;

#endif

#endif 

#define	MSG_MMI_GET_REQ_PIN_TYPE_RSP_STRUCT		mmi_smu_get_required_pin_type_rsp_struct
#define MSG_MMI_SMU_SECURITY_TYPE_ENUM		mmi_smu_security_type_enum 
#define	MSG_MMI_SMU_VERIFY_PIN_REQ_STRUCT    mmi_smu_verify_pin_req_struct
#define	MSG_MMI_SMU_VERIFY_PIN_RSP_STRUCT	mmi_smu_verify_pin_rsp_struct

void EntryScrPin2(void);
void ExitScrPin2(void);
void VerifyPin2(void);
void EntryScrInvalidPin2(void);
void ExitScrInvalidPin2(void);
void GoToCallHistoryMain(void);
#endif

