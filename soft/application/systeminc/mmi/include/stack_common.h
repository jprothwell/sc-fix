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
 *   stack_common.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file contains the following.
 *   1. All the module ID definitions.
 *   2. All the Stack SAPS.
 *   3. All the MSG code bases.
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/*********************************************************************
   (C) _____ (year of first publication) Sasken communication
   Technologies Limited, All rights reserved.
*   This file provides a template for .c files. This space
*   should be used to describe the file contents
*   Component-specific prefix : xxxx
*********************************************************************/

#ifndef _STACK_COMMON_H
#define _STACK_COMMON_H
#include "event.h"    

/*************************************************************************
 * Constant Definition
 *************************************************************************/
#define MAX_CUSTOM_MESSAGE_ID       1000   /* Maximum custom message ID */
#define MAX_CUSTOM_SAP_ID           10     /* Maximum custom SAP ID */

/*************************************************************************
* Macros Without Parameters
 *************************************************************************/
#define MAX_SAPS                    256
#define RPS_MASK_ARRAY_SIZE         4
#define RPS_ALLOCATE_ILM_MASK       31
#define RPS_ALLOCATE_ILM_SHIFT      5

/* NAS */
 
#define MSG_ID_MM_CODE_BEGIN        EV_MMI_EV_BASE    
#define MSG_ID_MM_CODE_RANGE        100

#define MSG_ID_CC_CODE_BEGIN        (MSG_ID_MM_CODE_BEGIN + MSG_ID_MM_CODE_RANGE)
#define MSG_ID_CC_CODE_RANGE        100

/*DT*/
#define MSG_ID_DT_CODE_BEGIN        (MSG_ID_CC_CODE_BEGIN + MSG_ID_CC_CODE_RANGE)
#define MSG_ID_DT_CODE_RANGE        10

#define MSG_ID_CISS_CODE_BEGIN      (MSG_ID_DT_CODE_BEGIN + MSG_ID_DT_CODE_RANGE)
#define MSG_ID_CISS_CODE_RANGE      100

#define MSG_ID_SMS_CODE_BEGIN       (MSG_ID_CISS_CODE_BEGIN + MSG_ID_CISS_CODE_RANGE)
#define MSG_ID_SMS_CODE_RANGE       100

#define MSG_ID_SIM_CODE_BEGIN       (MSG_ID_SMS_CODE_BEGIN + MSG_ID_SMS_CODE_RANGE)
#define MSG_ID_SIM_CODE_RANGE       150

 
#define MSG_ID_L4C_CODE_BEGIN       (MSG_ID_SIM_CODE_BEGIN + MSG_ID_SIM_CODE_RANGE)
#define MSG_ID_L4C_CODE_RANGE       300

#define MSG_ID_TCM_CODE_BEGIN       (MSG_ID_L4C_CODE_BEGIN + MSG_ID_L4C_CODE_RANGE)
#define MSG_ID_TCM_CODE_RANGE       100

#define MSG_ID_CSM_CODE_BEGIN       (MSG_ID_TCM_CODE_BEGIN + MSG_ID_TCM_CODE_RANGE)    
#define MSG_ID_CSM_CODE_RANGE       100

#define MSG_ID_SMSAL_CODE_BEGIN     (MSG_ID_CSM_CODE_BEGIN + MSG_ID_CSM_CODE_RANGE)  
#define MSG_ID_SMSAL_CODE_RANGE     100

#define MSG_ID_SMU_CODE_BEGIN       (MSG_ID_SMSAL_CODE_BEGIN + MSG_ID_SMSAL_CODE_RANGE)
#define MSG_ID_SMU_CODE_RANGE       100

#define MSG_ID_UEM_CODE_BEGIN       (MSG_ID_SMU_CODE_BEGIN + MSG_ID_SMU_CODE_RANGE)
#define MSG_ID_UEM_CODE_RANGE       100

#define MSG_ID_RAC_CODE_BEGIN       (MSG_ID_UEM_CODE_BEGIN + MSG_ID_UEM_CODE_RANGE)  
#define MSG_ID_RAC_CODE_RANGE       100

#define MSG_ID_USAT_CODE_BEGIN      (MSG_ID_RAC_CODE_BEGIN + MSG_ID_USAT_CODE_RANGE)   
#define MSG_ID_USAT_CODE_RANGE      100

#define MSG_ID_PHB_CODE_BEGIN       (MSG_ID_USAT_CODE_BEGIN + MSG_ID_USAT_CODE_RANGE)  
#define MSG_ID_PHB_CODE_RANGE       100

#define MSG_ID_ENG_CODE_BEGIN       (MSG_ID_PHB_CODE_BEGIN + MSG_ID_PHB_CODE_RANGE)
#define MSG_ID_ENG_CODE_RANGE       100

/* MMI */
#define MSG_ID_L4A_CODE_BEGIN       (MSG_ID_ENG_CODE_BEGIN + MSG_ID_ENG_CODE_RANGE)    
#define MSG_ID_L4A_CODE_RANGE       1300

/* RR */
#define MSG_ID_RLC_CODE_BEGIN  		(MSG_ID_L4A_CODE_BEGIN + MSG_ID_L4A_CODE_RANGE)    
#define MSG_ID_RLC_CODE_RANGE       100

#define MSG_ID_RRM_CODE_BEGIN       (MSG_ID_RLC_CODE_BEGIN + MSG_ID_RLC_CODE_RANGE)    
#define MSG_ID_RRM_CODE_RANGE       100

#define MSG_ID_RMPC_CODE_BEGIN      (MSG_ID_RRM_CODE_BEGIN + MSG_ID_RRM_CODE_RANGE)    
#define MSG_ID_RMPC_CODE_RANGE      200

#define MSG_ID_MAC_CODE_BEGIN  		(MSG_ID_RMPC_CODE_BEGIN + MSG_ID_RMPC_CODE_RANGE)  
#define MSG_ID_MAC_CODE_RANGE       100

#define MSG_ID_LAPDM_CODE_BEGIN     (MSG_ID_MAC_CODE_BEGIN + MSG_ID_MAC_CODE_RANGE)    
#define MSG_ID_LAPDM_CODE_RANGE     100

#define MSG_ID_REASM_CODE_BEGIN     (MSG_ID_LAPDM_CODE_BEGIN + MSG_ID_LAPDM_CODE_RANGE)
#define MSG_ID_REASM_CODE_RANGE     100

#define MSG_ID_MPAL_CODE_BEGIN      (MSG_ID_REASM_CODE_BEGIN + MSG_ID_REASM_CODE_RANGE)
#define MSG_ID_MPAL_CODE_RANGE      100

/* GPRS */
#define MSG_ID_LLC_CODE_BEGIN       (MSG_ID_MPAL_CODE_BEGIN + MSG_ID_MPAL_CODE_RANGE)
#define MSG_ID_LLC_CODE_RANGE       100

#define MSG_ID_SND_CODE_BEGIN       (MSG_ID_LLC_CODE_BEGIN + MSG_ID_LLC_CODE_RANGE)
#define MSG_ID_SND_CODE_RANGE       100

#define MSG_ID_SM_CODE_BEGIN        (MSG_ID_SND_CODE_BEGIN + MSG_ID_SND_CODE_RANGE)
#define MSG_ID_SM_CODE_RANGE        100

#define MSG_ID_PPP_CODE_BEGIN       (MSG_ID_SM_CODE_BEGIN + MSG_ID_SM_CODE_RANGE)
#define MSG_ID_PPP_CODE_RANGE       100

/* DATA */
#define MSG_ID_TDT_CODE_BEGIN       (MSG_ID_PPP_CODE_BEGIN + MSG_ID_PPP_CODE_RANGE)
#define MSG_ID_T30_CODE_BEGIN       (MSG_ID_TDT_CODE_BEGIN + 9)
#define MSG_ID_FA_CODE_BEGIN        (MSG_ID_T30_CODE_BEGIN + 50)
#define MSG_ID_TDT_CODE_RANGE       100

#define MSG_ID_L2R_CODE_BEGIN       (MSG_ID_TDT_CODE_BEGIN + MSG_ID_TDT_CODE_RANGE)
#define MSG_ID_L2R_CODE_RANGE       100

#define MSG_ID_RLP_CODE_BEGIN       (MSG_ID_L2R_CODE_BEGIN + MSG_ID_L2R_CODE_RANGE)
#define MSG_ID_RLP_CODE_RANGE       100

#define MSG_ID_MMIAPI_CODE_BEGIN    (MSG_ID_RLP_CODE_BEGIN + MSG_ID_RLP_CODE_RANGE)
#define MSG_ID_MMIAPI_CODE_RANGE    100

#define MSG_ID_MED_CODE_BEGIN       (MSG_ID_MMIAPI_CODE_BEGIN + MSG_ID_MMIAPI_CODE_RANGE)
#define MSG_ID_MED_CODE_RANGE       400

#define MSG_ID_ABM_CODE_BEGIN       (MSG_ID_MED_CODE_BEGIN + MSG_ID_MED_CODE_RANGE)
#define MSG_ID_ABM_CODE_RANGE       100

#define MSG_ID_SOC_CODE_BEGIN       (MSG_ID_ABM_CODE_BEGIN + MSG_ID_ABM_CODE_RANGE)
#define MSG_ID_SOC_CODE_RANGE       100

#define MSG_ID_TCPIP_CODE_BEGIN     (MSG_ID_SOC_CODE_BEGIN + MSG_ID_SOC_CODE_RANGE)
#define MSG_ID_TCPIP_CODE_RANGE     100

#define MSG_ID_WAP_CODE_BEGIN       (MSG_ID_TCPIP_CODE_BEGIN + MSG_ID_TCPIP_CODE_RANGE)
#define MSG_ID_WAP_CODE_RANGE       100

#define MSG_ID_EM_CODE_BEGIN        (MSG_ID_WAP_CODE_BEGIN + MSG_ID_WAP_CODE_RANGE)
#define MSG_ID_EM_CODE_RANGE        100

/* IRDA */
#define MSG_ID_IRDA_CODE_BEGIN      (MSG_ID_EM_CODE_BEGIN + MSG_ID_EM_CODE_RANGE)
#define MSG_ID_IRDA_CODE_RANGE      100

#define MSG_ID_OBEX_CODE_BEGIN      (MSG_ID_IRDA_CODE_BEGIN + MSG_ID_IRDA_CODE_RANGE)
#define MSG_ID_OBEX_CODE_RANGE      100

#define MSG_ID_IRCOMM_CODE_BEGIN    (MSG_ID_OBEX_CODE_BEGIN + MSG_ID_OBEX_CODE_RANGE)
#define MSG_ID_IRCOMM_CODE_RANGE    100

#define MSG_ID_MED_V_CODE_BEGIN     (MSG_ID_IRCOMM_CODE_BEGIN + MSG_ID_IRCOMM_CODE_RANGE)
#define MSG_ID_MED_V_CODE_RANGE     100

#define MSG_ID_J2ME_CODE_BEGIN      (MSG_ID_MED_V_CODE_BEGIN + MSG_ID_MED_V_CODE_RANGE)
#define MSG_ID_J2ME_CODE_RANGE      100

#define MSG_ID_EMAIL_CODE_BEGIN     (MSG_ID_J2ME_CODE_BEGIN + MSG_ID_J2ME_CODE_RANGE)
#define MSG_ID_EMAIL_CODE_RANGE     180

/* FMT */
#define MSG_ID_FMT_MSG_CODE_BEGIN   (MSG_ID_EMAIL_CODE_BEGIN + MSG_ID_EMAIL_CODE_RANGE)
#define MSG_ID_FMT_MSG_CODE_RANGE   120

#define DRIVER_MSG_CODE_BEGIN       (MSG_ID_FMT_MSG_CODE_BEGIN + MSG_ID_FMT_MSG_CODE_RANGE)
#define DRIVER_MSG_CODE_RANGE       100

#define MSG_ID_NVRAM_CODE_BEGIN     (DRIVER_MSG_CODE_BEGIN + DRIVER_MSG_CODE_RANGE)
#define MSG_ID_NVRAM_CODE_RANGE     100

/* DUMMY_PROTOCOL */
#define MSG_ID_DPS_MSG_START        (MSG_ID_NVRAM_CODE_BEGIN + MSG_ID_NVRAM_CODE_RANGE)
#define MSG_ID_DPS_MSG_RANGE        1000

/* L1 */
#define L1_MSG_CODE_BEGIN           (MSG_ID_DPS_MSG_START + MSG_ID_DPS_MSG_RANGE)
#define L1_MSG_CODE_RANGE           200

#define L1HISR_MSG_CODE_BEGIN       (L1_MSG_CODE_BEGIN + L1_MSG_CODE_RANGE)
#define L1HISR_MSG_CODE_RANGE       700

/* FT */
#define FT_MSG_CODE_BEGIN           (L1HISR_MSG_CODE_BEGIN + L1HISR_MSG_CODE_RANGE)
#define FT_MSG_CODE_RANGE           100

/* TST */
#define TST_MSG_CODE_BEGIN          (FT_MSG_CODE_BEGIN + FT_MSG_CODE_RANGE)
#define TST_MSG_CODE_RANGE          1200

/* SYSDEBUG */
#define SYSDEBUG_MSG_CODE_BEGIN     (TST_MSG_CODE_BEGIN + TST_MSG_CODE_RANGE)
#define SYSDEBUG_MSG_CODE_RANGE     100

/* CMUX */
#define CMUX_MSG_CODE_BEGIN         (SYSDEBUG_MSG_CODE_BEGIN + SYSDEBUG_MSG_CODE_RANGE)
#define CMUX_MSG_CODE_RANGE         1100

/* IMPS */
#define MSG_ID_IMPS_CODE_BEGIN      (CMUX_MSG_CODE_BEGIN + CMUX_MSG_CODE_RANGE)
#define MSG_ID_IMPS_CODE_RANGE      100

/* SIP */
#define SIP_MSG_CODE_BEGIN          (MSG_ID_IMPS_CODE_BEGIN + MSG_ID_IMPS_CODE_RANGE)
#define SIP_MSG_CODE_RANGE          50

/* POC */
#define POC_MSG_CODE_BEGIN          (SIP_MSG_CODE_BEGIN + SIP_MSG_CODE_RANGE)
#define POC_MSG_CODE_RANGE          50
#define BCHS_MSG_CODE_BEGIN    (POC_MSG_CODE_BEGIN + POC_MSG_CODE_RANGE)
#define BCHS_MSG_CODE_RANGE   1000
   
#define    BT_MSG_CODE_BEGIN   (BCHS_MSG_CODE_BEGIN + BCHS_MSG_CODE_RANGE)
#define    BT_MSG_CODE_RANGE  600


/* Maximum 1000 custom message */
#define CUSTOM_MSG_CODE_BEGIN       (BT_MSG_CODE_BEGIN + BT_MSG_CODE_RANGE + 1000)


/*************************************************************************
* Type Definitions
 *************************************************************************/
typedef void (*FSM_FUNCTION) (void*, void*);

typedef enum {
   INVALID_SAP = 0,
/* PS_BEGIN */
   /* NAS */
   /* TBD */
   GMMREG_SAP,
   /* GMM_REG_SAP = GMMREG_SAP, */
   /* GMM_TIMER_SAP, */
   MM_SMS_SAP,
   MM_CC_SAP,
   MM_SS_SAP,
   MM_AS_SAP,
   MM_SIM_SAP,
   GMM_SM_SAP,
   MNCC_SAP,
   MNSMS_SAP,
   MMREG_SAP,
   MNSS_SAP,
    
   CSM_TDT_SAP,
   CSM_L2R_SAP,
   CSM_T30_SAP,
   CSM_SIM_SAP,
   T30_L4C_SAP,
   CSM_L4C_SAP,
   SMSAL_L4C_SAP,
   UEM_L4C_SAP,
   PHB_L4C_SAP,
   SMU_L4C_SAP,
   TCM_L4C_SAP,
   RAC_L4C_SAP,
   UART_L4C_SAP,
   /* RR */
   RRM_RMPC_SAP,
   RMC_LAPDM_SAP,
   LAPDM_MPAL_SAP,
   RLC_MAC_SAP,
   RLC_RMPC_SAP,
   RLC_MPAL_SAP,
   RLC_REASM_SAP,
   MAC_RMPC_SAP,
   RRM_MPAL_SAP,
   RMPC_MPAL_SAP,
   MAC_MPAL_SAP,
   MMI_AS_SAP,
   SMSAL_AS_SAP,
   /* GPRS */
   LLC_GMM_SAP,
   LLC_RLC_SAP,
   LLC_SNDCP_SAP,
   LLC_SMS_SAP,
   GMM_RLC_SAP,
   SNDCP_REG_SAP,
   SM_SNDCP_SAP,
   SMREG_SAP,
   PPP_L4C_SAP,
   TCM_PPP_SAP,
   /* DATA */
   L2R_RLP_SAP,
   RLP_RA_SAP,
   T30_FA_SAP,
   FA_RA_SAP,
   SNDCP_PPP_SAP,
   DATA_MPAL_SAP,
   L2R_SMU_SAP,

   /* DRIVER */
   PS_NVRAM_SAP,
   PS_SIM_SAP,

   /* Media Task */
   MED_SAP,
   L4_AUD_SAP=MED_SAP,

#if (!defined(MED_V_NOT_PRESENT))
   /* Media-v Task */
   MED_V_SAP,
#endif /* !MED_V_NOT_PRESENT */

   /* MMI */
   MMI_L4C_SAP,

   /*WAP*/
   ABM_SOC_SAP,
   L4C_ABM_SAP,
   TCPIP_SOC_SAP,
   WAP_MMI_SAP,
   WPS_APP_SAP,
   TDT_PPP_SAP,
   L2R_PPP_SAP,
   PPP_TCPIP_SAP,
   TCM_TCPIP_SAP,
   SOC_APP_SAP, /* SOCKET to Applications */
   SMSAL_SOC_SAP,
   EM_PS_SAP,

   MMI_J2ME_SAP,
   J2ME_MMI_SAP,
   WAP_J2ME_SAP,
   J2ME_WAP_SAP,
   J2ME_JASYN_SAP,

   EMAIL_MMI_SAP,
   IMPS_MMI_SAP,   
   SIP_APP_SAP,
   POC_MMI_SAP,

   /* PS Unit Test */
   SME_SAP,
   SME_READER_SAP,
   SME_TIMER_SAP,
   /* PS_END */

   /* IrDA */
   OBEX_APP_SAP,
   IRCOMM_APP_SAP,
   IRDA_OBEX_SAP,
   IRDA_IRCOMM_SAP,   

   /* L1-L2, L1-MPAL */
   L1_MPAL_SAP,
   MPAL_L1_SAP,
   MAC_L1_SAP,
   L1_AS_SAP,
   L1HISR_AS_SAP,
   L1_LAPDM_SAP,

   L1_L1_SAP,

   DRIVER_PS_SAP,
	DRIVER_L1_SAP,
   FT_TST_SAP,
   STACK_TIMER_SAP,
   L4C_CMUX_SAP,
   CMUX_SAP,
   BCHS_L4C_SAP,
   BCHS_MMI_SAP,
   BT_A2DP_SAP,
   BT_APP_SAP,
   BT_AVRCP_SAP,
   BT_HFG_SAP,
   BT_BPP_SAP,
   BT_BIP_SAP,
   BT_PBAP_SAP,

   DT_L4C_SAP,

/*************************************************************************
 * Custom to add message sap id here, and maximum allow 10 sap id which 
 * limited by LAST_SAP_CODE 
 *************************************************************************/
   CUSTOM_SAP_BEGIN,
   
   LAST_SAP_CODE
} sap_type;

#define MOD_PH MOD_MPAL_READER

#endif /* _STACK_COMMON_H */

