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
 *	CallsCost.h
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: Calls_Defs.h

  	PURPOSE		: Call application 

 

	DATE		: Jan 15,02

**************************************************************/

#ifndef _COOLSAND_CALLSCOST_H
#define _COOLSAND_CALLSCOST_H

#define MSG_MMI_CC_GET_CCM_REQ_STRUCT        mmi_cc_get_ccm_req_struct
#define MSG_MMI_CC_GET_CCM_RSP_STRUCT        mmi_cc_get_ccm_rsp_struct
#define MSG_MMI_CC_GET_ACM_REQ_STRUCT        mmi_cc_get_acm_req_struct
#define MSG_MMI_CC_GET_ACM_RSP_STRUCT        mmi_cc_get_acm_rsp_struct
#define MSG_MMI_CC_GET_MAX_ACM_REQ_STRUCT    mmi_cc_get_max_acm_req_struct
#define MSG_MMI_CC_GET_MAX_ACM_RSP_STRUCT    mmi_cc_get_max_acm_rsp_struct

#define MSG_MMI_CC_RESET_ACM_REQ_STRUCT      mmi_cc_reset_acm_req_struct
#define MSG_MMI_CC_RESET_ACM_RSP_STRUCT      mmi_cc_reset_acm_rsp_struct
#define MSG_MMI_CC_SET_MAX_ACM_REQ_STRUCT	   mmi_cc_set_max_acm_req_struct
#define MSG_MMI_CC_SET_MAX_ACM_RSP_STRUCT	   mmi_cc_set_max_acm_rsp_struct

#define MSG_MMI_SMU_GET_PUC_REQ_STRUCT	      mmi_smu_get_puc_req_struct
#define MSG_MMI_SMU_GET_PUC_RSP_STRUCT	      mmi_smu_get_puc_rsp_struct
#define MSG_MMI_SMU_SET_PUC_REQ_STRUCT	      mmi_smu_set_puc_req_struct
#define MSG_MMI_SMU_SET_PUC_RSP_STRUCT	      mmi_smu_set_puc_rsp_struct


void HighlightCHISTCallCostItem( S32 index );
void HighlightCHISTCallCost( void );
void HighlightCHISTLastCost( void );
void HighlightCHISTAllCost( void );
void HighlightCHISTResetCost( void );
void HighlightCHISTMaxCost( void );
void HighlightCHISTPriceAndUnit( void );
void HighlightPriceAndUnitHandler(S32 index);

void HintCHISTLastCost(U16 index);
void HintCHISTAllCost(U16 index);
void HintCHISTMaxCost(U16 index);
void HintCHISTPriceAndUnit(U16 index);

void EntryCHISTCallCost( void );
void ExitCHISTCallCost( void );
void EntryCHISTResetCost( void );
void EntryCHISTSetMaxCost( void );
void ExitCHISTSetMaxCost( void );
void EntryCHISTSetPriceUnit( void );
void ExitCHISTSetPriceUnit( void );
void EntryCHISTSetPriceUnitConfirm( void );
void ExitCHISTSetPriceUnitConfirm( void );

void CHISTGetCallCost( void );
void CHISTValidateMaxCost( void );
void CHISTValidatePriceUnit( void );

void RspPin2ForCallCostSuccess( void );
void ReqGetLastCallCost( void );
void RspGetLastCallCost( void* info );
void ReqGetAllCallCost( void );
void RspGetAllCallCost( void* info );
void ReqGetMaxCallCost( void );
void RspGetMaxCallCost( void* info );
void ReqGetPriceAndUnit( void );
void RspGetPriceAndUnit( void* info );

void RspResetAllCallCost( void* info );
void ReqSetMaxCallCost( void );
void RspSetMaxCallCost( void* info );
void ReqSetPriceAndUnit( void );
void RspSetPriceAndUnit( void* info );

void CHISTConvertPPU( S8 *reqBuf, S8 *inputBuf );
void CHISTGoBackCallCostHistory( void );

#endif


