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
 *  CSP.h
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: CSP.h

  	PURPOSE		: Customer Service Profile

 

 

	DATE		: Aug 6,03

**************************************************************/
#ifndef _COOLSAND_CSP_H
#define _COOLSAND_CSP_H
#include "mmi_data_types.h"

typedef enum
{
	CSP_CT=3,
	CSP_CFNRc,
	CSP_CFNRy,
	CSP_CFB,
	CSP_CFU
}CSP_CALL_OFFERING_ENUM;

typedef enum
{
	CSP_BIC_Roam=3,
	CSP_BAIC,
	CSP_BOIC_exHC,
	CSP_BOIC,
	CSP_BOAC
}CSP_CALL_RESTRICTION_ENUM;

typedef enum
{
	CSP_CUG_OA=3,
	CSP_Pref_CUG,
	CSP_AoC,
	CSP_CUG,
	CSP_MPTY
}CSP_OTHER_SS_ENUM;

typedef enum
{
	CSP_User_Signalling=4,
	CSP_CCBS,
	CSP_CW,
	CSP_HOLD
}CSP_CALL_COMPLETION_ENUM;

typedef enum
{
	CSP_Validity_Period=1,
	CSP_Protocol_ID,
	CSP_Del_Conf,
	CSP_Reply_path,
	CSP_SM_CB,
	CSP_SM_MO,
	CSP_SM_MT
}CSP_TELESERVICES_ENUM;

typedef enum
{
	ALS=7
	
}CSP_CPHS_TELESERVICES_ENUM;

typedef enum
{
	CSP_SST=7
}CSP_CPHS_FEATURES_ENUM;

typedef enum
{
	CSP_CLI_block=0,
	CSP_CLI_send,
	CSP_MCI=3,
	CSP_CoLP,
	CSP_CoLR,
	CSP_CLIP=7
}CSP_NUMBER_IDENTIFICATION_ENUM;

typedef enum
{
	CSP_Multiple_Band=2,
	CSP_Multiple_Subscriber_Profile,
	CSP_Voice_Broadcast_Service,
	CSP_Voice_Group_call,
	CSP_HSCSD,
	CSP_GPRS
}CSP_PHASE_2_ENUM;

typedef enum
{
	CSP_Language=0,
	CSP_Data=2,
	CSP_Fax,
	CSP_SM_MO_EMAIL,
	CSP_SM_MO_PAGING,
	CSP_VPS,
	CSP_PLMN_MODE
}CSP_VALUE_ADDED_SERVICES_ENUM;

typedef enum
{
	CSP_CALL_OFFERING_SERVICE_GROUP=0,
	CSP_CALL_RESTRICTION_SERVICE_GROUP,
	CSP_OTHER_SS_SERVICE_GROUP,
	CSP_CALL_COMPLETION_SERVICE_GROUP,
	CSP_TELESERVICES_SERVICE_GROUP,
	CSP_CPHS_TELESERVICES_SERVICE_GROUP,
	CSP_CPHS_FEATURES_SERVICE_GROUP,
	CSP_NUMBER_IDENTIFICATION_SERVICE_GROUP,
	CSP_PHASE_2_SERVICE_GROUP,
	CSP_VALUE_ADDED_SERVICES_SERVICE_GROUP,
	CSP_INFORMATION_NO_SERVICE_GROUP,
	CSP_TOTAL_SERVICE_GROUP
}CSP_SERVICE_GROUP_CODE;

extern U8 cpsInfoArray[CSP_TOTAL_SERVICE_GROUP];

void SetCSPGroupService(U8 serviceGroup, U8 service);
void ResetCSPGroupService(U8 serviceGroup, U8 service);
U8 IsSetCSPGroupService(U8 serviceGroup, U8 service);
U8 IsResetCSPGroupService(U8 serviceGroup, U8 service);
void SaveCSPGroupServiceValue(U8 serviceGroup, U8 value);
void InitialiseCSPInfoArray(U8 *cspInfo);
U8 IsInformationNumberAlowed(void);
void CSPModifyHiliteHandlers(void);
void ClearCSPArray(void);
void RestorePreviousHiliteHandlers(U16 parentId);



#endif /* COOLSAND_CSP_H */

