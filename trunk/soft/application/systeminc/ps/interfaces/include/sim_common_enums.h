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
 *	sim_common.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file defines the data structure, enum and constant for SIM.
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

/********************************************************************************
*  Copyright Statement:
*  --------------------
*
*  This product has been developed using a protocol stack
*  developed by Sasken Communication Technologies Limited.
*
********************************************************************************/

#ifndef SIM_COMMON_ENUM_H
#define SIM_COMMON_ENUM_H


//#define __CPHS__
//#define SAT
//#define SIMTEST
/* +2 to support Rel.4 SIM card */
#ifdef __CPHS__
#ifdef __HOMEZONE_SUPPORT__
#define SIM_TOT_FILES            (108+2)  /* default 102, for supporting HomeZome */
#else
#define SIM_TOT_FILES            (102 +2)
#endif
#define MAX_SST_LEN              15
#else
#ifdef __HOMEZONE_SUPPORT__
#define SIM_TOT_FILES            (84+2) /* default 78, for supporting HomeZome */
#else
#define SIM_TOT_FILES            (78+2)
#endif
#define MAX_SST_LEN              13
#endif
#define MAX_PROFILE_LEN          18
#define MAX_SIM_TIMER_NUM         1
#define MAX_DATA_LEN            256
#define NUM_OF_BYTE_ECC           3
#define MAX_ECC_NUM               5
#define NUM_OF_BYTE_LP            2
#define MAX_LP_NUM                5
#define MAX_EF_HEADER_LEN        15
#define MAX_DF_HEADER_LEN        22
#define NUM_GID1                 20
#define NUM_GID2                 20
#define LEN_OF_OPNAME            20
#define LEN_OF_SOPNAME           10
#define LEN_OF_SPN               17
#define SIZE_OF_LOCATION_INFO     7

typedef enum
{
   TYPE_NONE=0,
   TYPE_ADN=0x2,
   TYPE_FDN=0x4,
   TYPE_BDN=0x8,
   TYPE_ADN_BDN=0x0A,
   TYPE_FDN_BDN=0x0C
}sim_dn_enum;

typedef enum
{
   SIM_CHV_1=1,
   SIM_CHV_2=2
}sim_chv_type_enum;

typedef enum
{
   RESET,
   INIT,
   PUK1
}sim_start_mode_enum;

typedef enum
{
   SIM_PHASE_0,
   SIM_PHASE_2=2,
   SIM_PHASE_3=3
}sim_phase_enum;

typedef enum
{
   ELP,
   LP
}sim_lp_file_type_enum;



typedef enum
{
   SIM_FILE_SIZE,
   SIM_REC_NUM,
   SIM_REC_SIZE
}sim_info_type_enum;


typedef enum
{
   SERVICE_CHV_DISABLE_FUNCTION=1,
   SERVICE_ADN,
   SERVICE_FDN,
   SERVICE_SMS,
   SERVICE_AOC,
   SERVICE_CCP,
   SERVICE_PLMNSEL,
   SERVICE_RFU1,
   SERVICE_MSISDN,
   SERVICE_EXT1,
   SERVICE_EXT2,
   SERVICE_SMSP,
   SERVICE_LND,
   SERVICE_CBMI,
   SERVICE_GID1,
   SERVICE_GID2,
   SERVICE_SPN,
   SERVICE_SDN,
   SERVICE_EXT3,
   SERVICE_RFU2,
   SERVICE_VGCS,
   SERVICE_VBS,
   SERVICE_EMLPP,
   SERVICE_AAEM,
   SERVICE_DATA_DOWNLOAD_SMS_CB,
   SERVICE_DATA_DOWNLOAD_SMS_PP,
   SERVICE_MENU_SELECTION,
   SERVICE_CALL_CONTROL,
   SERVICE_PROACTIVE_SIM,
   SERVICE_CBMIR,
   SERVICE_BDN,
   SERVICE_EXT4,
   SERVICE_DCK,
   SERVICE_CNL,
   SERVICE_SMSR,
   SERVICE_NW_IND_ALERT,
   SERVICE_MO_SMS,
   SERVICE_GPRS,
   SERVICE_IMAGE,
   SERVICE_SOLSA,
   SERVICE_USSD,
   SERVICE_RUN_AT_CMD,
   SERVICE_PLMNWACT,
   SERVICE_OPLMNWACT,
   SERVICE_HPLMNWACT,
   SERVICE_CPBCCH,
   SERVICE_INVSCAN,
   SERVICE_ECCP,
   SERVICE_MEXE,
   SERVICE_CSP=53,
   SERVICE_SST,
   SERVICE_MAILBOX_NUM,
   SERVICE_OPNAME_SHORTFORM,
   SERVICE_INFO_NUM,
   SERVICE_END
}sim_service_enum;

typedef enum
{
   SIM_CARD_REMOVED,
   SIM_ACCESS_ERROR,
   SIM_REFRESH,
   SIM_PUK1
}sim_error_cause_enum;

typedef enum
{
   SIM_SEEK_TYPE1_FROM_BEG=0x00,
   SIM_SEEK_TYPE1_FROM_BACK=0x01,
   SIM_SEEK_TYPE1_FROM_NEXT=0x02,
   SIM_SEEK_TYPE1_FROM_PREV=0x03,
   SIM_SEEK_TYPE2_FROM_BEG=0x10,
   SIM_SEEK_TYPE2_FROM_BACK=0x11,
   SIM_SEEK_TYPE2_FROM_NEXT=0x12,
   SIM_SEEK_TYPE2_FROM_PREV=0x13
}sim_seek_type_mode_enum;

typedef enum
{
   SIM_CMD_SUCCESS        = 2560,
   SIM_CMD_FAIL           = 2561,
   SIM_FATAL_ERROR        = 2562,
   SIM_NO_INSERTED        = 2563,
   SIM_CHV_NOT_INIT       = 2564,
   SIM_CHV_RETRY_ALLOW    = 2565,
   SIM_CHV_BLOCK          = 2566,
   SIM_ACCESS_NOT_ALLOW   = 2567,
   SIM_SAT_CMD_BUSY       = 2568,
   SIM_DL_ERROR           = 2569,
   SIM_MEMORY_PROBLEM     = 2570,
   SIM_TECHNICAL_PROBLEM  = 2571,
   SIM_PUK_UNLOCK   = 2572, /* add new enum value to distinguish whether normal poweron or PUK-unlock-poweron */   
   SIM_EF_RECORD_FULL = 2573 /*add new enum value to represent no free record in the file*/
}sim_cmd_result_enum;

typedef enum
{
   SIM_REPORT_MEMORY_PROBLEM,	
   SIM_ACCESS_NOT_FULFILLED,
   SIM_SECORD_CODE_BLOCK	
}sim_status_report_enum;


typedef enum
{
   VERIFY,
   DISABLE,
   ENABLE,
   CHANGE,
   UNBLOCK
}sim_security_operation_enum;

typedef enum
{
   SERVICE_NOT_SUPPORT,
   SERVICE_SUPPORT
}sim_service_state_enum;

typedef enum
{
   FILE_MF_IDX,
   FILE_GSM_IDX,
   FILE_TELECOM_IDX,
   FILE_IRIDIUM_IDX,
   FILE_GLOBST_IDX,
   FILE_ICO_IDX,
   FILE_ACES_IDX,
   FILE_EIA_IDX,
   FILE_CTS_IDX,
   FILE_SOLSA_IDX,
   FILE_MEXE_IDX,
   FILE_GRAPHICS_IDX,
   FILE_ICCID_IDX,
   FILE_ELP_IDX,
   FILE_SAI_IDX,
   FILE_SLL_IDX,
   FILE_MEXE_ST_IDX,
   FILE_ORPK_IDX,
   FILE_ARPK_IDX,
   FILE_TPRPK_IDX,
   FILE_IMG_IDX,/*20*/
   FILE_LP_IDX,
   FILE_IMSI_IDX,
   FILE_KC_IDX,
   FILE_PLMNSEL_IDX,
   FILE_HPLMN_IDX,
   FILE_ACMAX_IDX,
   FILE_SST_IDX,
   FILE_ACM_IDX,
   FILE_GID1_IDX,
   FILE_GID2_IDX,/*30*/
   FILE_SPN_IDX,
   FILE_PUCT_IDX,
   FILE_CBMI_IDX,
   FILE_BCCH_IDX,
   FILE_ACC_IDX,
   FILE_FPLMN_IDX,
   FILE_LOCI_IDX,
   FILE_AD_IDX,
   FILE_PHASE_IDX,
   FILE_VGCS_IDX,
   FILE_VGCSS_IDX,
   FILE_VBS_IDX,
   FILE_VBSS_IDX,
   FILE_EMLPP_IDX,
   FILE_AAEM_IDX,
   FILE_CBMID_IDX,
   FILE_ECC_IDX,
   FILE_CBMIR_IDX,
   FILE_DCK_IDX,
   FILE_CNL_IDX,/*50*/
   FILE_NIA_IDX,
   FILE_KCGPRS_IDX,
   FILE_LOCIGPRS_IDX,
   FILE_SUME_IDX,
   FILE_PLMNWACT_IDX,
   FILE_OPLMNWACT_IDX,
   FILE_HPLMNACT_IDX,
   FILE_CPBCCH_IDX,
   FILE_INVSCAN_IDX,
   FILE_VM_WAIT_IDX,/*60*/
   FILE_CPHS_SST_IDX,
   FILE_CF_FLAG_IDX,
   FILE_OP_STRING_IDX,
   FILE_CSP_IDX,
   FILE_CPHS_INFO_IDX,
   FILE_MAILBOX_NUM_IDX,
   FILE_OP_SHORTFORM_IDX,
   FILE_INFO_NUM_IDX,
   FILE_ADN_IDX,
   FILE_FDN_IDX,
   FILE_SMS_IDX,/*71*/
   FILE_CCP_IDX,
   FILE_ECCP_IDX,
   FILE_MSISDN_IDX,
   FILE_SMSP_IDX,
   FILE_SMSS_IDX,
   FILE_LND_IDX,
   FILE_SDN_IDX,
   FILE_EXT1_IDX,
   FILE_EXT2_IDX,
   FILE_EXT3_IDX,
   FILE_BDN_IDX,
   FILE_EXT4_IDX,
   FILE_SMSR_IDX,
   FILE_CMI_IDX,
   FILE_IMG_1_IDX,
   FILE_IMG_2_IDX,
   FILE_IMG_3_IDX,
   FILE_IMG_4_IDX,
   FILE_IMG_5_IDX,
   FILE_IMG_6_IDX,
   FILE_IMG_7_IDX,
   FILE_IMG_8_IDX,
   FILE_IMG_9_IDX,
   FILE_IMG_A_IDX,
   FILE_IMG_B_IDX,
   FILE_IMG_C_IDX,
   FILE_IMG_D_IDX,
   FILE_IMG_E_IDX,
   FILE_IMG_F_IDX,
   #ifdef __HOMEZONE_SUPPORT__
   /* For HomeZone support */
   FILE_VID_IDX, /* 101 */
   FILE_HZ_IDX,
   FILE_HZ_CACHE1_IDX,
   FILE_HZ_CACHE2_IDX,
   FILE_HZ_CACHE3_IDX,
   FILE_HZ_CACHE4_IDX,
   #endif /* __HOMEZONE_SUPPORT__ */
   /* Support Rel.4 SIM files in 51.011 */
   FILE_PNN_IDX,
   FILE_OPL_IDX,
   FILE_NONE=0xFF
}sim_file_index_enum;

typedef enum
{
   SIM_NO_FILE_ID     = 0x0000,
   MASTER_FILE_ID     = 0x3f00, /* Master File */
   /* Dedicated File under Master File */
   DF_GSM_ID          = 0x7f20,
   DF_TELECOM_ID      = 0x7f10,

   /* Dedicated files under DF_GSM */
   DF_IRIDIUM_ID      = 0x5f30,
   DF_GLOBST_ID       = 0x5f31,
   DF_ICO_ID          = 0x5f32,
   DF_ACeS_ID         = 0x5f33,
   DF_EIA_ID          = 0x5f40,
   DF_CTS_ID          = 0x5f60,
   DF_SoLSA_ID        = 0x5f70,
   DF_MExE_ID         = 0x5f3C,
   /* Dedicated File under DF_TELECOM */
   DF_GRAPHICS_ID     = 0x5f50,
   /* Elementary Files under Master File */
   EF_ICCID_ID        = 0x2fE2,
   EF_ELP_ID          = 0x2f05,
   /* Elementary Files under DF_SoLSA */
   EF_SAI_ID          = 0x4f30,
   EF_SLL_ID          = 0x4f31,
   /* Elementary Files under DF_MExE */
   EF_MExE_ST_ID      = 0x4f40,
   EF_ORPK_ID         = 0x4f41,
   EF_ARPK_ID         = 0x4f42,
   EF_TPRPK_ID        = 0x4f43,
   /* Elementary Files at DF_GRAPHICS */
   EF_IMG_ID          = 0x4f20,

   EF_IMG_1_ID        = 0x4f01,
   EF_IMG_2_ID        = 0x4f02,
   EF_IMG_3_ID        = 0x4f03,
   EF_IMG_4_ID        = 0x4f04,
   EF_IMG_5_ID        = 0x4f05,
   EF_IMG_6_ID        = 0x4f06,
   EF_IMG_7_ID        = 0x4f07,
   EF_IMG_8_ID        = 0x4f08,
   EF_IMG_9_ID        = 0x4f09,
   EF_IMG_A_ID        = 0x4f0a,
   EF_IMG_B_ID        = 0x4f0b,
   EF_IMG_C_ID        = 0x4f0c,
   EF_IMG_D_ID        = 0x4f0d,
   EF_IMG_E_ID        = 0x4f0e,
   EF_IMG_F_ID        = 0x4f0f,

   /* Elementary Files under DF_GSM */
   EF_LP_ID           = 0x6f05,
   EF_IMSI_ID         = 0x6f07,
   EF_Kc_ID           = 0x6f20,
   EF_PLMNsel_ID      = 0x6f30,
   EF_HPLMN_ID        = 0x6f31,
   EF_ACMax_ID        = 0x6f37,
   EF_SST_ID          = 0x6f38,
   EF_ACM_ID          = 0x6f39,
   EF_GID1_ID         = 0x6f3E,
   EF_GID2_ID         = 0x6f3F,
   EF_SPN_ID          = 0x6f46,
   EF_PUCT_ID         = 0x6f41,
   EF_CBMI_ID         = 0x6f45,
   EF_BCCH_ID         = 0x6f74,
   EF_ACC_ID          = 0x6f78,
   EF_FPLMN_ID        = 0x6f7B,
   EF_LOCI_ID         = 0x6f7E,
   EF_AD_ID           = 0x6fAD,
   EF_PHASE_ID        = 0x6fAE,
   EF_VGCS_ID         = 0x6fB1,
   EF_VGCSS_ID        = 0x6fB2,
   EF_VBS_ID          = 0x6fB3,
   EF_VBSS_ID         = 0x6fB4,
   EF_eMLPP_ID        = 0x6fB5,
   EF_AAeM_ID         = 0x6fB6,
   EF_CBMID_ID        = 0x6f48,
   EF_ECC_ID          = 0x6fB7,
   EF_CBMIR_ID        = 0x6f50,
   EF_DCK_ID          = 0x6f2C,
   EF_CNL_ID          = 0x6f32,
   EF_NIA_ID          = 0x6f51,
   EF_KcGPRS_ID       = 0x6f52,
   EF_LOCIGPRS_ID     = 0x6f53,
   EF_SUME_ID         = 0x6f54,
   EF_PLMNwACT_ID     = 0x6f60,
   EF_OPLMNwACT_ID    = 0x6f61,
   EF_HPLMNACT_ID     = 0x6f62,
   EF_CPBCCH_ID       = 0x6f63,
   EF_INVSCAN_ID      = 0x6f64,
   EF_VM_WAIT_ID      = 0x6F11,
   EF_CPHS_SST_ID     = 0x6F12,
   EF_CF_FLAG_ID      = 0x6F13,
   EF_OP_STRING_ID    = 0x6F14,
   EF_CSP_ID          = 0x6F15,
   EF_CPHS_INFO_ID    = 0x6F16,
   EF_MAILBOX_NUM_ID  = 0x6F17,
   EF_OP_SHORTFORM_ID = 0x6F18,
   EF_INFO_NUM_ID     = 0x6F19,
   /* Elementary Files at DF_TELECOM Level */
   EF_ADN_ID          = 0x6f3A,
   EF_FDN_ID          = 0x6f3B,
   EF_SMS_ID          = 0x6f3C,
   EF_CCP_ID          = 0x6f3D,
   EF_ECCP_ID         = 0x6f4F,
   EF_MSISDN_ID       = 0x6f40,
   EF_SMSP_ID         = 0x6f42,
   EF_SMSS_ID         = 0x6f43,
   EF_LND_ID          = 0x6f44,
   EF_SDN_ID          = 0x6f49,
   EF_EXT1_ID         = 0x6f4A,
   EF_EXT2_ID         = 0x6f4B,
   EF_EXT3_ID         = 0x6f4C,
   EF_BDN_ID          = 0x6f4D,
   EF_EXT4_ID         = 0x6f4E,
   EF_SMSR_ID         = 0x6f47,
   EF_CMI_ID          = 0x6f58,
#ifdef __HOMEZONE_SUPPORT__   
   DF_VID_ID                = 0x7F43,
   EF_HZ_ID                  = 0x6F60,
   EF_HZ_CACHE1_ID   = 0x6F61,
   EF_HZ_CACHE2_ID   = 0x6F62,
   EF_HZ_CACHE3_ID   = 0x6F63,
   EF_HZ_CACHE4_ID   = 0x6F64,
#endif /* __HOMEZONE_SUPPORT__ */   
   /* Support Rel.4 SIM files in 51.011 */
   EF_PNN_ID = 0x6FC5,
   EF_OPL_ID = 0x6FC6,
   EF_NULL            = 0xFFFF
}sim_file_id_enum;

typedef enum
{
   SIM_NOT_READY, /* initial state */
   SIM_CARD_NOT_INSERTED, /* No SIM found after initialization */
   NORMAL_SIM_PRESENCE, /* Normal SIM found after initialization */
   TEST_SIM_PRESENCE /* Test SIM found after initialization */
}sim_card_status_enum;

#ifdef __SAT__
typedef enum
{
   SAT_MM_NO_SERVICE,
   SAT_MM_LIMITED_SERVICE,
   SAT_MM_NORMAL_SERVICE
}sat_mm_status_enum;

typedef enum
{
   SAT_CALL_DISCONNECT,
   SAT_CALL_CONNECT
}sat_call_status_enum;

typedef enum
{
   TYPE_SIM_INIT_AND_FULL_FILE_CHANGE,
   TYPE_FILE_CHANGE,
   TYPE_SIM_INIT_AND_FILE_CHANGE,
   TYPE_SIM_INIT,
   TYPE_SIM_RESET    
}sat_refresh_type_enum;
#endif

typedef struct 
{
   /* This field gives the actual file id of EF */ 
   sim_file_id_enum           file_id;

   /* This field gives the id of the parent of the EF */ 
   sim_file_index_enum        parent_file_idx;

   /* This field gives the id of the grandparent of the EF */ 
   sim_file_index_enum        grand_parent_file_idx;

}sim_ef_info_struct;

#define SIM_EF_INFO(sim_file_index_enum, X)  sim_ef_info[sim_file_index_enum].X
extern sim_service_state_enum sim_service_table_query(sim_service_enum service_id);
extern sim_file_index_enum sim_id_map_to_index(kal_uint16 file_id);
extern kal_bool is_test_sim(void);
extern sim_card_status_enum check_sim_card_status(void);
#endif


