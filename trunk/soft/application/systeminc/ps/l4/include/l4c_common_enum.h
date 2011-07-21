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
 *	l4c_common_enum.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for �K.
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

/********************************************************************************
*  Copyright Statement:
*  --------------------
*
*  This product has been developed using a protocol stack
*  developed by Sasken Communication Technologies Limited.
*
********************************************************************************/



#ifndef _L4C_COMMON_ENUM_H
#define _L4C_COMMON_ENUM_H

#define L4C_MAX_NAME_LEN         10
#define L4C_MAX_CALL_LIST_LEN    7

#define MAX_PIN_LENGTH           8  /* to be removed */
#define MAX_DIGITS_USSD          183

#define MAX_CC_ADDR_LEN          41
#define MAX_CC_ADDR_BCD_LEN      21
#define MAX_CC_SUB_ADDR_LEN      41
#define MAX_CC_SUB_ADDR_BCD_LEN  21


#define MAX_SS_PASSWD_LEN        9
#define MAX_SS_PASSWD_BCD_LEN    5

#define MAX_PLMN_LEN             6
#define MAX_PLMN_BCD_LEN         3
#define MAX_PLMN_LIST_LEN        16

#define MAX_SIM_PASSWD_LEN       9
#define MAX_SIM_PASSWD_BCD_LEN   5

#define INVALID_BS_CODE          0

#define L4C_USE_DEFAULT_VALUE    255
#define L4C_MAX_RETRY_COUNT      10

#define MAX_CREE_STR_LEN         75



#define L4C_LONGPRESS_TIMER  20 /* 2 sec */
#define L4C_REPEATED_TIMER  10 /* 2 sec */

#define L4C_DOWNLOAD_ERROR_TIMER  100 /* 10 sec */
#define L4C_DOWNLOAD_SLEEP_TIMER 10 /* 1 sec */  
#define L4C_GPRS_CONNECT_TIEMR   50 /* 10 sec */

enum
{
   GSM_ONLY,
   GPRS_ONLY,
   GSM_AND_GPRS
};

typedef enum
{
   TCM_PDP_DEACTIVATE,
   TCM_PDP_ACTIVATE
}l4c_tcm_pdp_action_enum;

typedef enum
{
   TCM_INVALID_L2P,
   TCM_PPP_L2P
}l4c_tcm_l2p_enum;

typedef enum
{
   TCM_REJECT_PDP_ACT,
   TCM_ACCEPT_PDP_ACT
}l4c_tcm_manual_answer_enum;

typedef enum
{
   LMMI_SRC,
   //RMMI_SRC, /*UART or CMUX virtual port 1*/	
   SAT_SRC,
   WAP_SRC,
   RMMI_SRC, /*UART or CMUX virtual port 1*/	   
   RMMI_SRC2, /*UART or BT*/
   RMMI_TST, /*TST inject AT*/
   RMMI_SAT, /*SAT inject AT*/
   RMMI_2, 	/*CMUX virtual port 2*/
   RMMI_3, 	/*CMUX virtual prot 3*/
   RMMI_ALL, /*for rmmi indication*/
   L4C_NO_USER, /*no user register action*/
   INVALID_SRC, /*Invalid source for MT*/
   MAX_SOURCE_NUM
} l4c_source_id_enum;

#if defined (__CMUX_SUPPORT__)  && defined ( __WINCE__)
#define RMMI_COMMAND RMMI_SRC;
#define RMMI_UNSOLICITED RMMI_2;
#define RMMI_DATA_CHANNEL RMMI_3;
#else
#define RMMI_COMMAND RMMI_SRC;
#define RMMI_UNSOLICITED RMMI_ALL;
#define RMMI_DATA_CHANNEL RMMI_SRC;
#endif

typedef enum
{
   L4C_COMMAND_STATE = 0,
   L4C_ONLINE_COMMAND_STATE,
   L4C_ONLINE_DATA_STATE,
   L4C_ONLINE_DATA_GPRS_STATE
} l4c_state_enum;

/*
typedef enum
{
   L4C_IDLE_STATE = 0,
   L4C_CMD_FORWARDED_FOR_ACTION_STATE,
   L4C_INTERMEDIATE_RESULT_CODE_SENT_STATE
} l4c_sub_state_enum;
*/

typedef enum
{
	L4C_CC,
	L4C_SS,
	L4C_SMS,
	L4C_RAC,
	L4C_TCM,
	L4C_PHB
}l4c_module_id;

typedef enum
{
   L4C_NO_ACTION,
   L4C_FIRST_ACTION,                /*1*/
   NVM_R_EXE = L4C_FIRST_ACTION,
   NVM_W_EXE,
   UART_READ_EXE,//CSD00468 modify for canno release buffer fail in set uart in EM mode
   UART_WRITE_EXE,
   UART_PORT_READ,
   SIM_R_EXE,
   SIM_W_EXE,
   CCSP_EXE,
   SIM_FILE,
//#ifdef __TCPIP__
   CSD_PROF_SET,
   ABM_ACT_APP,
   ABM_ACT,
   ABM_ACT_ECC,
   PPP_ACT,     						/*10*/
   ABM_DEACT,
   ABM_DISC,
//#endif
   FIRST_CC_ACTION,
   CHLD_EXE = FIRST_CC_ACTION,
#ifdef __MOD_SMU__
   CACM_EXE,
   CAMM_EXE,
#endif /* __MOD_SMU */
   CHUP_EXE,
   CTFR_EXE,                      
   ATA_EXE,
   ATD_APP,
   ATD_EXE,								/*20*/
   ATD_ECC,
   CMOD_EXE,
   ATH_EXE,
   PARSE_SS_EXE,
   CPHS_ALS_UPDATE,
   CHLD_CSD_DEACT,        /* this action is to handle CSD active and incoming call coming */
   CLCC_READ,
   LAST_CC_ACTION = CLCC_READ,
   FIRST_SS_ACTION,                 
   CAEMLPP_EXE = FIRST_SS_ACTION,
   CCFC_REPEAT,
   CCFC_EXE,
   CCWA_REPEAT,
   CCWA_EXE,
   ITRG_EXE,							/*30*/
   CDIP_EXE,
   CLCK_SS_REPEAT,
   CLCK_SS_EXE,
   CLIP_EXE,
   CLIR_EXE,
   CNAP_EXE,
   COLP_EXE,
   COLR_EXE,
   CPWD_SS_EXE,                     
   CB_FAC_IND,
   CCBS_EXE,						/* 40 */
   CUSD_EXE,
   USSR_FAC_IND,
   LAST_SS_ACTION = USSR_FAC_IND,
#ifdef CSD_SUPPORT
   //FIRST_DATA_ACTION,
   //LAST_DATA_ACTION,
#endif
   FIRST_EQ_ACTION,
   VOL_SET = FIRST_EQ_ACTION,
   AUDI_NAME_PLAY, 
   AUDI_NAME_STOP,/* CSD00468 add for play audio by name*/
   AUDI_STRING_PLAY, 
   AUDI_STRING_STOP, /* CSD00468 add for play audio by string */
   AUDI_SET, /* for audio profile set 030627 CSD00468*/
   AUDI_PARAM_SET,
   GAIN_SET,
   CMUT_SET,							/*50*/
   CSIL_SET,
   CALM_SET,   /* alert, removed later */
   CKPD_EXE,                       
   CIND_SET,
   CALA_SET,
   CALD_EXE,
   CSDF_SET,
   CSTF_SET,
   CLAN_SET,
   GPIO_SET,							/*60*/
   CSGT_SET,
   CGCI_SET,
   CVIB_SET,
   HW_PROF_SET,
   MS_SN_READ,
   MS_IMEI_READ,
   MS_SN_SET,
   MS_IMEI_SET, //030923 CSD00468 add for IMEI set 
   LOCK_NVRAM,
   TEST_REPORT_READ,  //CSD00714 add for autotest report read
#ifdef VM_SUPPORT
   VM_GET_EXE,
   VM_REC_EXE,
   VM_PLAY_EXE,
   VM_DEL_EXE,
   VM_STOP_EXE,
   VM_ABT_EXE,
   VM_PAU_EXE,
   VM_RES_EXE,
   VM_APP_EXE,
   VM_REN_EXE,
#endif

   POWERON_EXE,          					/*80*/                  
   POWEROFF_EXE,
   LAST_EQ_ACTION = POWEROFF_EXE,
   
#ifdef __IRDA_SUPPORT__
   FIRST_IRDA_ACTION,
   PPP_DEACT_EXE = FIRST_IRDA_ACTION, /* add to keep PPP deactive */
   LAST_IRDA_ACTION = PPP_DEACT_EXE,
#endif /*__IRDA_SUPPORT__*/

#ifdef __USB_ENABLE__
   FIRST_USB_ACTION,
   USB_CONFIG_EXE = FIRST_USB_ACTION, /* configure usb */
   LAST_USB_ACTION = USB_CONFIG_EXE,
#endif /* __USB_ENABLE__ */

#ifdef __MOD_TCM__
   FIRST_GPRS_ACTION,
   CGACT_EXE = FIRST_GPRS_ACTION,
   CGACT_TEST,
   CGANS_EXE,							
   CGCMOD_EXE,
   CGDCONT_EXE,
   GPRS_COUNTER_RESET,
   GPRS_ACCOUNT_SET,
   CGDSCONT_EXE,
   CGQMIN_EXE,
   CGQREQ_EXE,
   CGTFT_EXE,
   EGPAU_EXE, // CSD00468 add for ppp auth info set  031015
   ATT_DEFAULT_SET,
   CGDATA_EXE,
   LAST_GPRS_ACTION = CGDATA_EXE,
#endif
#ifdef __MOD_RAC__
   FIRST_NW_ACTION,
   CGATT_EXE = FIRST_NW_ACTION,       		/*90*/
   CGCLASS_SET,
  // SET_GPRS_CON_TYPE,			/* '04/01/07 CSD00468 add for set gprs connect type*/
   CBND_SET,				
   COPS_EXE,
   PWROFF_DETACH,
   RFOFF_DETACH,
   RFON_ATTACH,
   COPS_TEST,
   LAST_NW_ACTION = COPS_TEST,
#endif
   FIRST_PHB_ACTION,
   CPBF_EXE= FIRST_PHB_ACTION,
   PHB_FDN_CHECK,//action for fdn check for sms sca and da
   CPBR_EXE,
   CPBR_RANGE_EXE,
   CNUM_EXE,
   CPBW_EXE,
   CPBD_EXE,                     
   CPBR_LN,								/*100*/
   CPBW_LN,
   CPBD_LN,							
   LAST_PHB_ACTION = CPBD_LN,
   FIRST_SIM_ACTION,
   CLCK_EXE = FIRST_SIM_ACTION,
   CMAR_EXE,
   CPIN_EXE,
   CPOL_EXE,
   CPUC_EXE,
   CPWD_EXE,
   XDN_EXE,
   CRSM_EXE,
   LAST_SIM_ACTION = CRSM_EXE,
   FIRST_SMS_ACTION,               
   CGSMS_SET = FIRST_SMS_ACTION,
   CMGC_EXE,						/*100*/
   CMGD_EXE,
   CMGL_EXE,
   CMGR_EXE,
   CMGS_EXE,
   CMGS_PDU_EXE,
   CMGW_EXE,
   CMSS_EXE,
   CPMS_SET,
   /*CRES_EXE,
   CSAS_EXE,*/
   CSCA_SET,
   CNMI_SET,                     
   MAILBOX_SET, 					/* 030617 CSD00468 add for set mail box address*/
   CSMP_SET,
   CSAS_SET,
   LAST_SMS_ACTION = CSAS_SET,
#ifdef __CB__
   CBMI_SET,
#endif
#ifdef __SAT__
   SAT_ATD_EXE,
   SAT_USD_EXE,
   MENU_SEL_EXE,
   SAT_CMGS_EXE,
   SAT_DTMF_EXE,
#ifdef __SATCE__   
   SAT_CSD_EXE,
   SAT_OPEN_CHANNEL_EXE,
   SAT_SEND_DATA_EXE,
   SAT_RECV_DATA_EXE,
#endif /* __SATCE__ */   
#endif

#ifdef __CSD_FAX__
	FDT_EXE,
	FDR_EXE,
	FIP_EXE,
	FET_EXE,
	FTM_EXE,
	FRM_EXE,
	FTH_EXE,
	FRH_EXE,
	FTS_EXE,
	FRS_EXE,
	FCC_EXE,
	FDCC_EXE,
	FIS_EXE,
	FDIS_EXE,
	FCS_EXE,
	FDCS_EXE,
	FLI_EXE,
	FLID_EXE,
	FPI_EXE,
	FCIG_EXE,
	FLP_EXE,
	FLPL_EXE,
	FSP_EXE,
	FSPL_EXE,
	FNR_EXE,
	FIE_EXE,
	FPS_EXE,
	FPTS_EXE,
	FRQ_EXE,
	FBADLIN_EXE,
	FBADMUL_EXE,
	FCQ_EXE,
	FIT_EXE,
	FPP_EXE,
	FBO_EXE,
	FBOR_EXE,
	FEA_EXE,
	FREL_EXE,
	FNS_EXE,
	FCR_EXE,
	FBU_EXE,
	FBUG_EXE,
	FAP_EXE,
	FSA_EXE,
	FPA_EXE,
	FPW_EXE,
	FCT_EXE,
	FPHCTO_EXE,
	FHS_EXE,
	FMS_EXE,
	FMINSP_EXE,
	FRY_EXE,
	FBS_EXE,
	FFC_EXE,
	FVRFC_EXE,
	FDFFC_EXE,
	FLNFC_EXE,
	FWDFC_EXE,

	
#endif /* __CSD_FAX__ */

   ATO_EXE,                      
#ifdef __WAP_SUPPORT					/* 030528 CSD00468 add for WAP profile set and query*/
   CWAP_ACCESS,
   //L4C_LAST_ACTION = CWAP_ACCESS
//#else
   //L4C_LAST_ACTION = ATO_EXE
#endif 

	PREFER_BAND_READ,
	PREFER_BAND_SET,
	L4C_LAST_ACTION = PREFER_BAND_SET
} l4c_action_enum;

typedef enum
{
   L4C_FIRST_IND,                /*0*/
   INIT_IND = L4C_FIRST_IND,
   NVM_READY_IND,
   SIM_FAIL_IND,
   PHB_FIN_IND,
   LN_FIN_IND,
   
   FIRST_CC_IND,
   CAOC_IND = FIRST_CC_IND,
   SPEECH_IND,
   ALERT_IND,
   CCWA_IND,
   CRING_IND,
   DISC_IND,
   REL_IND,                       /* 10 */
   CMOD_IND,                     
   CONN_IND,
   NOTIFY_SS_IND,
   NS_CUG_IND,
   NS_ECT_IND,
   NS_CNAP_IND,
   NS_CCBS_IND,
   CPHS_ALS_IND,//CSD00468 add for ripple CPHS feature
   START_DTMF,
   STOP_DTMF,						/* 20 */
   PARSE_SS_IND,
   LAST_CC_IND = PARSE_SS_IND,
   
   FIRST_SS_IND,
   CB_PWD_IND = FIRST_SS_IND,
   CUSD_R_IND,
   CUSD_N_IND,                 
   CPHS_CFU_IND,  
   LAST_SS_IND = CPHS_CFU_IND,
   
   FIRST_EQ_IND,
   KEY_IND = FIRST_EQ_IND,
   POWERON_IND,
   BAT_IND,
#ifdef VM_SUPPORT
   VM_REC_IND,
   VM_PLAY_IND,						/*30*/
#endif

//#ifdef DATA_DOWNLOAD
   FS_DIR_QUERY_IND,
   FS_FILE_MODIFY_IND,
   FS_FILE_EXECUTE_IND,
//#endif /*DATA_DOWNLOAD*/
   RTC_IND,
   ALARM_IND,
   ADC_CHAN_IND,
   EM_SET_LCM_IND,//CSD00468 add for factory mode test
   EM_KEYPAD_ACT_IND,//CSD00468 add for factory mode test
   EM_LCM_SLEEP_IND,//CSD00468 add for LCM backlight turn on/off
   FACTORY_IND, //CSD00468 1107 add for factory test inform MMI
   MMI_PROF_SET_IND, //CSD00468 add 1117 for mmi's profile setting indication
   AUDI_STOP_IND,
   GPIO_IND,
   LAST_EQ_IND = GPIO_IND,
   
#ifdef __IRDA_SUPPORT__
   FIRST_IRDA_IND,
   IRDA_DISCON_IND = FIRST_IRDA_IND,
   LAST_IRDA_IND = IRDA_DISCON_IND,
#endif /* __IRDA_SUPPORT__ */

#ifdef __MOD_TCM__
   FIRST_GPRS_IND,
   GACT_IND = FIRST_GPRS_IND,
   GPRS_STATUS_IND,
   LAST_GPRS_IND = GPRS_STATUS_IND,				
#endif

#ifdef __MOD_RAC__
   FIRST_NW_IND,
   REG_IND = FIRST_NW_IND,				/*40*/
   ATTACH_IND,
   CSQ_IND,
   CTZR_IND,                     
   LAST_NW_IND = CTZR_IND,
#endif

	/* CSD0046803050315 add enum IND so that in l4c_sat_msg
	 * we can use mmi_ind_ft to use it. modified direct call the 
	 * funciton handler and more systemlize
	*/
#ifdef __SAT__
   FIRST_SAT_IND, 
   SAT_DSPL_TEXT_IND = FIRST_SAT_IND, 
   SAT_GET_INKEY_IND,
   SAT_GET_INPUT_IND,
   SAT_SETUP_MENU_IND,
   SAT_SELECT_ITEM_IND,
   SAT_PLAY_TONE_IND,
   SAT_SETUP_CALL_IND,					/*50 */
   SAT_SEND_SMS_IND,
   SAT_SEND_SS_IND,
   SAT_SEND_USSD_IND,
   SAT_MMI_INFO_IND,
   SAT_SETUP_IDLE_DSPL_IND,  
   SAT_RUN_AT_COMMAND_IND,
   SAT_SEND_DTMF_IND,
   SAT_LANG_NOTIFY_IND,
#ifdef __SATCE__   
   SAT_OPEN_CHANNEL_IND,
   SAT_CLOSE_CHANNEL_IND,
   SAT_SEND_DATA_IND,
   SAT_RECV_DATA_IND,
#endif /* __SATCE__ */   
   SAT_LAUNCH_BROWSER_IND,	
   SAT_NO_OTHER_CMD_IND,
   LAST_SAT_IND = SAT_NO_OTHER_CMD_IND, 
#endif /* __SAT__ */

   FIRST_PHB_IND,
   PHB_FULL_IND = FIRST_PHB_IND,			/* 60 */
   PHB_UPDATE_IND,       /* CSD00468 add for sync AT and MMI with phb write ande delete*/
   PHB_STARTUP_BEGIN_IND,
   PHB_STARTUP_READ_IND,
   LAST_PHB_IND = PHB_STARTUP_READ_IND,

   FIRST_SIM_IND,
   PASSWD_IND = FIRST_SIM_IND,
   SIM_INFO_IND,
   PLMN_LEN_IND,
   CPHS_MMI_INFO, //CSD00468 030610 add for l4csmu_mmi_info_ind handle
   LAST_SIM_IND = CPHS_MMI_INFO,

#ifdef __USB_ENABLE__
   FIRST_USB_IND,
   USB_DETECT_IND = FIRST_USB_IND,
   LAST_USB_IND = USB_DETECT_IND,
 #endif /* __USB_ENABLE__ */
   
   FIRST_SMSAL_IND,
   SMS_FULL_IND = FIRST_SMSAL_IND,
   SMS_EXC_IND,
   SMS_AVA_IND,
   SMS_WAIT_IND, //030613 CSD00468 add for SMS CPHS feature
   SMS_INDX_IND,
   SMS_TEXT_IND,								/* 70 */
   SMS_PDU_IND,
   SMS_READY_IND, //CSD00468 add in 040229
   SMS_APP_DATA_IND, //CSD00468 add in 040312 for WMA
   SMS_SYNC_MSG_IND,
   CMGD_IND,
   CMGW_IND,
   CMGS_ABORT_START_IND,
   CMGS_ABORT_FINISH_IND,
   CB_MSG_IND,  //for LMMI & text mode of RMMI.
   CB_PDU_IND, //for RMMI pdu mode
   CB_DL_IND,
   LAST_SMSAL_IND = CB_DL_IND,
   L4C_LAST_IND = LAST_SMSAL_IND
} l4c_ind_enum;


typedef enum
{
   PAS_READY,
   PAS_UNAVAILABLE,
   PAS_CALL_RINGING = 3,
   PAS_CALL_IN_PROGRESS

} l4ccsm_phone_act_enum;


typedef enum
{
   SINGLE_VOICE_CALL,
   SINGLE_DATA_CALL,
   SINGLE_FAX_CALL,
   FOLLOW_VOICE_CALL,
   ALT_DATA_VOICE_CALL,
   ALT_FAX_VOICE_CALL,
   FOLLOW_DATA_CALL,
   ALT_DATA_DATA_CALL,
   ALT_FAX_FAX_CALL,
   UNKNOWN_CALL

} l4ccsm_cc_call_mode_enum;

typedef enum
{
   L4C_ASYNC_T,  /* ASYNC */
   L4C_SYNC_T,    /* SYNC */
   L4C_ASYNC_NT,   /* REL ASYNC */
   L4C_SYNC_NT      /* REL SYNC */
} l4ccsm_cc_data_call_type_enum;


enum
{
   L4C_CISS_NOT_ACTIVE,
   L4C_CISS_ACTIVE
};

enum
{
   L4C_SMU_INACTIVE,
   L4C_SMU_ACTIVE,
   L4C_SMU_ACTIVATE_FAIL
};

typedef enum
{
   L4C_SUCCESS,
   L4C_SIM_FAIL,
   L4C_NVRAM_FAIL,
   L4C_GENERAL_ERROR
} l4c_poweron_result_enum;

typedef enum
{
	L4C_PHB_UPDATE,
	L4C_PHB_DELETE,
	L4C_PHBD_ELETE_ALL
}l4c_phb_update_enum;

typedef enum
{
	L4C_PHB_NO_ERROR,
	L4C_PHB_FAIL,
	L4C_PHB_SCA_FDN,
	L4C_PHB_DA_FDN_ONLY
}l4c_phb_sms_fdn_result_enum;

typedef enum
{
   L4C_NONE_DATA,
   L4C_CSD_DATA,
   L4C_GPRS_DATA
} l4c_data_mode_enum;

typedef enum
{
   L4C_MMI_PROF_QUERY,
   L4C_MMI_PROF_SET
} l4c_mmi_set_prof_op_enum;


typedef enum
{
   L4C_WHEN_NEEDED,
   L4C_ALWAYS,
   L4C_GPRS_CONN_TYPE_UNKNOWN
} l4c_gprs_connect_type_enum;

typedef enum
{
   L4C_GPRS_ATTACHED, //0
   L4C_NONE_GPRS, // 1
   L4C_GPRS_DETACHED, // 2
   L4C_INVALID_SIM,// 3
   L4C_GPRS_ATTEMPT_ATTACH,// 4
   L4C_GPRS_COVERAGE,//5 
   L4C_PDP_ATTEMPT_ACT,//6
   L4C_PDP_DEACTIVED,//7
   L4C_PDP_ACTIVED//8
} l4c_gprs_status_enum;

typedef enum
{
   L4C_IMELODY,
   L4C_SOUND_OTHER,
   L4C_IMAGE,
   L4C_APP_UNKNOWN
} l4c_app_type_enum;

typedef enum
{
   L4C_IMAGE_STD,
   L4C_AUDIO_IMELODY,
   L4C_AUDIO_OTHER
} l4c_data_dl_folder_enum;

typedef enum
{
   L4C_FILE_ADD,
   L4C_FILE_DELETE
}l4c_data_dl_file_action_enum;

typedef enum
{
   L4C_FILE_EXECUTE_PLAY,        /*play audio*/
   L4C_FILE_EXECUTE_STOP        /*stop play audio*/
}l4c_data_dl_execute_action_enum;

typedef enum
	{
		L4C_DISCONNECT_NONE,
		L4C_DISCONNECT_MO,
		L4C_DISCONNECT_MT
	} l4c_ath_req_enum;

typedef enum
	{
		L4C_COPS_UNKNOWN,
		L4C_COPS_AVAILABLE,
		L4C_COPS_CURRENT,
		L4C_COPS_FORBIDDEN
	} l4c_cops_status_enum; /*27.007 7.3 +COPS*/

typedef enum
{
	L4C_AT_SMS_ADD,
	L4C_AT_SMS_READ,
	L4C_AT_SMS_DELETE,
	L4C_SMS_SIM_REFRESH

}l4c_at_sms_action_enum;

typedef enum
{
   CBS0_GERMAN = 0,
   CBS0_ENGLISH,
   CBS0_ITALIAN,
   CBS0_FRENCH,
   CBS0_SPANISH,
   CBS0_DUTCH,
   CBS0_SWEDISH,
   CBS0_DANISH,
   CBS0_PORTUGUESE,
   CBS0_FINNISH,
   CBS0_NORWEGIAN,
   CBS0_GREEK,
   CBS0_TURKISH,
   CBS0_HUNGARIAN,
   CBS0_POLISH,
   CBS0_LANG_UNSPECIFIED,

   CBS1_CZECH = 16,
   CBS1_HEBREW,
   CBS1_ARABIC,
   CBS1_RUSSIAN,


   CBS2_AA, // AFAR, 20
   CBS2_AB, // ABKHAZIAN
   CBS2_AF, // AFRIKAANS
   CBS2_AM, // AMHARIC
   //CBS2_AR, // ARABIC
   CBS2_AS, // ASSAMESE
   CBS2_AY, // AYMARA
   CBS2_AZ, // AZERBAIJANI

   CBS2_BA, // BASHKIR
   CBS2_BE, // BYELORUSSIAN
   CBS2_BG, // BULGARIAN, 30
   CBS2_BH, // BIHARI
   CBS2_BI, // BISLAMA
   CBS2_BN, // BENGALI; BANGLA
   CBS2_BO, // TIBETAN
   CBS2_BR, // BRETON

   CBS2_CA, // CATALAN
   CBS2_CO, // CORSICAN
   //CBS2_CS, // CZECH
   CBS2_CY, // WELSH

   //CBS2_DA, // DANISH, 40
   //CBS2_DE, // GERMAN
   CBS2_DZ, // BHUTANI

   //CBS2_EL, // GREEK
   //CBS2_EN, // ENGLISH
   CBS2_EO, // ESPERANTO
   //CBS2_ES, // SPANISH
   CBS2_ET, // ESTONIAN
   CBS2_EU, // BASQUE

   CBS2_FA, // PERSIAN
   //CBS2_FI, // FINNISH, 50
   CBS2_FJ, // FIJI
   CBS2_FO, // FAROESE
   //CBS2_FR, // FRENCH
   CBS2_FY, // FRISIAN

   CBS2_GA, // IRISH
   CBS2_GD, // SCOTS GAELIC
   CBS2_GL, // GALICIAN
   CBS2_GN, // GUARANI
   CBS2_GU, // GUJARATI

   CBS2_HA, // HAUSA, 60
   //CBS2_HE, // HEBREW (FORMERLY IW)
   CBS2_HI, // HINDI
   CBS2_HR, // CROATIAN
   //CBS2_HU, // HUNGARIAN
   CBS2_HY, // ARMENIAN

   CBS2_IA, // INTERLINGUA
   CBS2_ID, // INDONESIAN (FORMERLY IN)
   CBS2_IE, // INTERLINGUE
   CBS2_IK, // INUPIAK
   CBS2_IS, // ICELANDIC, 70
   //CBS2_IT, // ITALIAN
   CBS2_IU, // INUKTITUT

   CBS2_JA, // JAPANESE
   CBS2_JW, // JAVANESE

   CBS2_KA, // GEORGIAN
   CBS2_KK, // KAZAKH
   CBS2_KL, // GREENLANDIC
   CBS2_KM, // CAMBODIAN
   CBS2_KN, // KANNADA
   CBS2_KO, // KOREAN, 80
   CBS2_KS, // KASHMIRI
   CBS2_KU, // KURDISH
   CBS2_KY, // KIRGHIZ

   CBS2_LA, // LATIN
   CBS2_LN, // LINGALA
   CBS2_LO, // LAOTHIAN
   CBS2_LT, // LITHUANIAN
   CBS2_LV, // LATVIAN, LETTISH

   CBS2_MG, // MALAGASY
   CBS2_MI, // MAORI, 90
   CBS2_MK, // MACEDONIAN
   CBS2_ML, // MALAYALAM
   CBS2_MN, // MONGOLIAN
   CBS2_MO, // MOLDAVIAN
   CBS2_MR, // MARATHI
   CBS2_MS, // MALAY
   CBS2_MT, // MALTESE
   CBS2_MY, // BURMESE

   CBS2_NA, // NAURU
   CBS2_NE, // NEPALI, 100
   //CBS2_NL, // DUTCH
   //CBS2_NO, // NORWEGIAN

   CBS2_OC, // OCCITAN
   CBS2_OM, // (AFAN) OROMO
   CBS2_OR, // ORIYA

   CBS2_PA, // PUNJABI
   //CBS2_PL, // POLISH
   CBS2_PS, // PASHTO, PUSHTO
   //CBS2_PT, // PORTUGUESE

   CBS2_QU, // QUECHUA, 110

   CBS2_RM, // RHAETO-ROMANCE
   CBS2_RN, // KIRUNDI
   CBS2_RO, // ROMANIAN
   //CBS2_RU, // RUSSIAN
   CBS2_RW, // KINYARWANDA

   CBS2_SA, // SANSKRIT
   CBS2_SD, // SINDHI
   CBS2_SG, // SANGHO
   CBS2_SH, // SERBO-CROATIAN
   CBS2_SI, // SINHALESE, 120
   CBS2_SK, // SLOVAK
   CBS2_SL, // SLOVENIAN
   CBS2_SM, // SAMOAN
   CBS2_SN, // SHONA
   CBS2_SO, // SOMALI
   CBS2_SQ, // ALBANIAN
   CBS2_SR, // SERBIAN
   CBS2_SS, // SISWATI
   CBS2_ST, // SESOTHO
   CBS2_SU, // SUNDANESE, 130
    
   CBS2_SW, // SWAHILI

   CBS2_TA, // TAMIL
   CBS2_TE, // TELUGU
   CBS2_TG, // TAJIK
   CBS2_TH, // THAI
   CBS2_TI, // TIGRINYA
   CBS2_TK, // TURKMEN
   CBS2_TL, // TAGALOG
   CBS2_TN, // SETSWANA, 140
   CBS2_TO, // TONGA
   //CBS2_TR, // TURKISH
   CBS2_TS, // TSONGA
   CBS2_TT, // TATAR
   CBS2_TW, // TWI

   CBS2_UG, // UIGHUR
   CBS2_UK, // UKRAINIAN
   CBS2_UR, // URDU
   CBS2_UZ, // UZBEK

   CBS2_VI, // VIETNAMESE, 150
   CBS2_VO, // VOLAPUK

   CBS2_WO, // WOLOF

   CBS2_XH, // XHOSA

   CBS2_YI, // YIDDISH (FORMERLY JI)
   CBS2_YO, // YORUBA

   CBS2_ZA, // ZHUANG
   CBS2_ZH, // CHINESE
   CBS2_ZU, // ZULU, 158
   CBS_INVALID = 255
} l4c_cbs_language_enum;
#endif /* l4c_common_enum.h */


