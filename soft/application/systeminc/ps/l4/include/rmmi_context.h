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
 *	rmmi_context.h
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



#ifndef _RMMI_CONTEXT_H
#define _RMMI_CONTEXT_H

#ifndef _RMMI_COMMON_ENUM_H
#error "rmmi_common_enum.h shall be included before rmmi_context.h"
#endif
#include "kal_release.h" /* in dir. kal */

#include "stacklib.h"
#include "event_shed.h"
#include "fat_fs.h" //bt_debug

/* CONSTANTS */

#define NULL_TERM '\0' /* string termination */
#define INVALID_ENUM 255  /* invalid enum value */




#define rmmi_skip_spaces(source_string_ptr)                               \
      while( source_string_ptr->string_ptr[ source_string_ptr->index ]    \
                                 == RMMI_SPACE )                          \
      {                                                                   \
        source_string_ptr->index++;                                          \
      }
//030617 CSD00468 add for skip unused character before character a 
#define rmmi_skip_none_a(source_string_ptr, length)                               \
      while( (source_string_ptr->index < length) && !((source_string_ptr->string_ptr[ source_string_ptr->index ] == RMMI_CHAR_A) ||\
             (source_string_ptr->string_ptr[ source_string_ptr->index ] == rmmi_char_a)) ) \
      {                                                                   \
        source_string_ptr->index++;                                          \
      }

#define RMMI_IS_LOWER( alpha_char )   \
  ( ( (alpha_char >= rmmi_char_a) && (alpha_char <= rmmi_char_z) ) ?  1 : 0 )

#define RMMI_IS_UPPER( alpha_char )   \
   ( ( (alpha_char >= RMMI_CHAR_A) && (alpha_char <= RMMI_CHAR_Z) ) ? 1 : 0 )

#define RMMI_IS_NUMBER( alpha_char )   \
   ( ( (alpha_char >= RMMI_CHAR_0) && (alpha_char <= RMMI_CHAR_9) ) ? 1 : 0 )

#define RMMI_IS_ALPHA( alpha_char )   \
   ( ( RMMI_IS_UPPER(alpha_char) || RMMI_IS_LOWER(alpha_char) ) ? 1 : 0 )

#define RMMI_IS_SYMBOL( alpha_char )   \
   ( ( (alpha_char == RMMI_CHAR_PLUS) || (alpha_char == RMMI_CHAR_STAR) || \
       (alpha_char == RMMI_CHAR_POUND) || (alpha_char == RMMI_COMMA) || \
       (alpha_char == RMMI_CHAR_DOT) || (alpha_char == RMMI_FORWARD_SLASH) || \
       (alpha_char == RMMI_COLON) || (alpha_char == RMMI_HAT) || \
       (alpha_char == RMMI_CHAR_MINUS) || \
       (alpha_char == RMMI_L_SQ_BRACKET) || \
       (alpha_char == RMMI_R_SQ_BRACKET) || \
       (alpha_char == RMMI_L_ANGLE_BRACKET) || \
        (alpha_char == RMMI_CHAR_ULINE) || \
         (alpha_char == RMMI_SPACE) || \
           (alpha_char == RMMI_SEMICOLON) || \
       (alpha_char == RMMI_R_ANGLE_BRACKET) \
     ) ? 1 : 0 )

typedef kal_bool (*UART_TRANS_FUNC_PTR)(kal_uint8);

typedef struct rmmi_report_mode_struct
{
   kal_uint8      ccm;           /* rmmi_report_mode_enum */
   kal_uint8      acm_warning;   /* rmmi_report_mode_enum */
   kal_uint8      call_wait;     /* rmmi_report_mode_enum */
   kal_uint8      cdip;          /* rmmi_report_mode_enum */
   kal_uint8      clip;          /* rmmi_report_mode_enum */
   kal_uint8      clir;          /* rmmi_report_mode_enum */
   kal_uint8      cnap;          /* rmmi_report_mode_enum */
   kal_uint8      colp;          /* rmmi_report_mode_enum */
   kal_uint8      ss_mo;         /* rmmi_report_mode_enum */
   kal_uint8      ss_mt;         /* rmmi_report_mode_enum */
   kal_uint8      ussd;          /* rmmi_report_mode_enum */
   kal_uint8      compression;   /* rmmi_report_mode_enum */
   kal_uint8      ps_reg;        /* rmmi_nw_reg_report_mode_enum */
   kal_uint8      cs_reg;        /* rmmi_nw_reg_report_mode_enum */
   kal_uint8      timezone;      /* rmmi_report_mode_enum */
   kal_uint8	crc;			 /* rmmi_report_mode_enum, for +CRC */
   kal_uint8	cr;			 /* rmmi_report_mode_enum, for +CR */
   kal_uint8	cmer_mode;	 
   kal_uint8	cmer_keyp;   
   kal_uint8	cmer_ind; 	
   kal_uint8	cmer_bfr; 	
   kal_uint8	ecpi; 	/*proprietary +ECPI: call progress information */
   kal_uint8	eind; 	/*proprietary +EIND: general indication */
   					/* Bit 0(SMS ready), Bit 1(PHB ready), Bit 7(AT ready), others reserved*/
   kal_uint8	ecsq;
} rmmi_report_mode_struct;


typedef struct rmmi_rsp_mode_struct
{
   kal_uint8   echo_flag;        /* rmmi_cmd_echo_enum */
   kal_uint8   suppress_flag;    /* rmmi_result_code_suppress_enum */
   kal_uint8   format;           /* rmmi_result_code_format */   
} rmmi_rsp_mode_struct;

typedef struct rmmi_single_cmd_struct
{
   kal_uint16      cmd_code;     /* rmmi_extended_cmd_id_enum */
   kal_uint8      cmd_mode;   /* rmmi_cmd_mode_enum */
} rmmi_single_cmd_struct;


typedef struct basic_cmd_struct
{
   kal_uint8      cmd_code;     /* rmmi_basic_cmd_id_enum */
   kal_uint8      cmd_mode;   /* rmmi_cmd_mode_enum */

   kal_bool       is_this_node_used;
   kal_uint8      value;

   struct basic_cmd_struct  *next_ptr;
} rmmi_multiple_basic_cmd_struct;

typedef struct rmmi_cmd_struct
{
   kal_uint8                  which_present; /* rmmi_cmd_type_enum */

   rmmi_single_cmd_struct     single_cmd;
   rmmi_multiple_basic_cmd_struct   multiple_cmd; //not used now
} rmmi_cmd_struct;

typedef struct
{
   kal_uint8   s0;   /* auto answer */
   kal_uint8   s3;   /* CR */
   kal_uint8   s4;   /* LF */
   kal_uint8   s5;   /* backspace */
   kal_uint8   s6;   /* pause before dialing */
   kal_uint8   s7;   /* connect timeout */
   kal_uint8   s8;   /* , pause time */
   kal_uint8   s10;  /* disconnect timeout */

} rmmi_s_register_struct;

typedef struct uart_queue_struct
{
   kal_uint16  head;
   kal_uint16  length;
   kal_uint8   data[MAX_DATA_QUEUE_LENGTH];

} rmmi_uart_queue_struct;

typedef struct
{
   
   kal_bool     sat_in_menu;
   kal_bool     sat_in_process;
   kal_bool     sat_normal_standby;
   kal_bool     sat_ready;
   kal_uint8    sat_input_type;
   kal_uint8    sat_input_reveal;
   kal_uint8    sat_input_min_len;
   kal_uint8    sat_input_max_len;
   kal_bool     sat_redial;
   kal_bool     sat_redial_count;
   kal_uint32   sat_duration;

}rmmi_SAT_info_struct;

typedef struct rmmi_cind_struct
{
   kal_uint8 battchg;
   kal_uint8 signal;
   kal_uint8 service;
   kal_uint8 message;
   kal_uint8 call;
   kal_uint8 roam;
   kal_uint8 smsfull;
   kal_uint8 callsetup;
} rmmi_cind_struct;

typedef struct rmmi_reg_state_struct
{
   kal_uint8				gsm_state; // for +CREG
   kal_uint8				gprs_state; //for +CGREG
   kal_uint16				cell_id;  // for +CREG, +CGREG
   kal_uint16				lac_value;	// for +CREG, +CGREG
} rmmi_reg_state_struct;

/* init by l4cphb_start_cnf_hdlr */
typedef struct rmmi_phb_info_struct 
{
   kal_uint8 sim_phb_len;      
} rmmi_phb_info_struct;

typedef struct rmmi_clip_info_struct 
{
   kal_uint8 number[41];//MAX_CC_ADDR_LEN
   kal_uint8 type;
   kal_uint8 subaddr[41];//MAX_CC_ADDR_LEN
   kal_uint8 satype;
   kal_uint8 cli_validity;
} rmmi_clip_info_struct;

typedef struct rmmi_context_struct
{
   rmmi_report_mode_struct    	report_mode;
   rmmi_rsp_mode_struct       	rsp_mode;
   rmmi_cmd_struct            	cmd_info;
   rmmi_s_register_struct     	s_reg;

   kal_bool				cmux_enable;
   
#ifdef __CMUX_SUPPORT__
/* Begin for CMUX */
   kal_uint8				source[RMMI_MAX_CHANNEL_NUMBER];
   kal_uint8				port[RMMI_MAX_CHANNEL_NUMBER];
   rmmi_uart_queue_struct     	uart_input_queue[RMMI_MAX_CHANNEL_NUMBER];
   rmmi_uart_queue_struct     	uart_output_queue[RMMI_MAX_CHANNEL_NUMBER];
   kal_uint8 					multi_cmd_string[RMMI_MAX_CHANNEL_NUMBER][MAX_MULTIPLE_CMD_INFO_LEN];
   rmmi_uart_queue_struct		CNMI_ResultCodeBuffer; //for +CNMI, SMSAL_ONE_MSG_LEN = 160
/* End for CMUX */

#elif defined (__ACCEL_BTVCSR_HP__)
/* Begin for bluetooth */
   rmmi_uart_queue_struct     	uart_input_queue[RMMI_MAX_CHANNEL_NUMBER];
   rmmi_uart_queue_struct     	uart_output_queue[RMMI_MAX_CHANNEL_NUMBER];
   kal_uint8 					multi_cmd_string[2][MAX_MULTIPLE_CMD_INFO_LEN];
   rmmi_uart_queue_struct		CNMI_ResultCodeBuffer; //for +CNMI, SMSAL_ONE_MSG_LEN = 160
   /* some context for BT */
   kal_uint8 	BT_ctrl_src; //LMMI(User) or RMMI(PC Tool) 
   kal_bool 	BT_port;
   kal_uint8	*BT_string;
   kal_uint8	BT_action;
   kal_uint8	BT_err_act;
   kal_uint8	BT_err_reason;
   kal_uint8    BT_cancel_act;
   kal_uint8	BT_addr[6];
   kal_uint8	BT_name[40];
   kal_bool	BT_as_serial_port;
   kal_bool	BT_visible;
   kal_uint8	BT_auth;
   eventid		BT_guard_timer;
   kal_bool BT_rmmi_rsp_flag;
   FS_HANDLE BT_file_handle; //BT_DEBUG
   kal_bool      cmd_from_BT;
/* End for bluetooth */
#else
/* Begin of normal case: only one channel for physical UART*/
   rmmi_uart_queue_struct     	uart_input_queue[1];
   rmmi_uart_queue_struct     	uart_output_queue[1];
#if defined (__SLIM_AT__) || defined (__EM_AT_ONLY__)
	kal_uint8					CNMI_ResultCodeBuffer; //dummy 
	kal_uint8 				multi_cmd_string[1][1];	//dummy
#else
   	rmmi_uart_queue_struct	CNMI_ResultCodeBuffer; //for +CNMI, SMSAL_ONE_MSG_LEN = 160
	kal_uint8 				multi_cmd_string[1][MAX_MULTIPLE_CMD_INFO_LEN];
#endif
/* End of normal case */
#endif


   kal_uint8                   	uart_stop_send_flag;
   void*                       	arg_list[RMMI_MAX_ARG_NUM];
      
/* for parsing */   
	kal_uint8			is_multi_cmd;
	kal_uint8			multi_cmd_error; // this flag using to verify if error occur , we should abort command processor
   
   kal_bool     		cmd_present;  //for A/ use
   kal_bool     		cmd_from_tst; //command is from Catcher
   kal_uint8 			is_sms_cmd; //using to cehck string is sms command and send " > " to DTE
   kal_uint8			isAtdAction_flag; /* CSD00714 20040412 : remember ATD this action after setup_cnf */

/* use to check if sim verified */
   kal_bool			command_allow;// using to keep if password required, only some AT command can access when pin puk or ph_sim pending
   kal_uint16			cmd_error_cause;// SMU saves command not allow cause for +cme error value to TA 

/* settings */
   kal_uint8		 	em_enable; //CSD00468 add for using to keep the em enable or disable engineer mode information
   kal_uint8			ccsr_enable; //CSD00468 add for turn on/off call state indication report 
   kal_uint8    		date_aux_mode; // this variable using for +CSDF to keep in local
   kal_uint8			cring_type; //CSD00714 add for +CRC (remember cring_type for rmmi_ring_repeat_hdlr)
   kal_uint8    		error_report_mode; ///* +CMEE <mode>: rmmi_error_report_mode_enum */
   kal_uint8                  	plmn_format;
   kal_uint8                   plmn_oper[7]; //CSD00924 2005_0426: remember the plmn selected 
   kal_uint8			cops_mode;
   kal_uint8			CPOL_plmn_format;
   kal_uint8                  	smsal_bfr;  /*+CNMI : <bfr> */
   kal_uint8			CNMI_mode;  // +CNMI: <mode>: result code mode 
   kal_uint8                  	char_set;	//+CSCS
   kal_uint16                 	max_plmn_list_len;  //+CPOL ?
   kal_bool			cb_mode;
   kal_bool			csdh_show;		//CSD00714: for +CSDH , hide/show detail Text mode parameters.
   kal_bool			clae_mode; // we save clae mode in rmmi context
   kal_bool			cmut_mode; // we save clae mode in rmmi context
   kal_uint8			preferred_lang;

/* some necessary global variables */   
   kal_bool			in_call;
   kal_bool                   	uart_in_data_mode;
   kal_uint8 			cpin_status;//for cpin query when verify pin error
   kal_uint8			ring_count; // for ATS0
   rmmi_reg_state_struct  	register_status; //registration status for +CREG, +CGREG
   rmmi_phb_info_struct		phb_info;
   rmmi_clip_info_struct   	clip_info;
   kal_uint8			casp_soundID; //for +CASP proprietary command , timeout hdlr parameter.
   rmmi_cind_struct		indicators ; // for +CIND and +CMER 
   kal_bool                 	is_gprs_data_dialing; /*ATH is not allowed to hangup existed voice call after dialup failed*/
   kal_uint8			cgatt_last_action ; /* CSD00714 20040611 add to avoid continuos at+cgatt=0 or at+cgatt=1 */
   kal_uint8			phb_storage_type;
   kal_uint16			cmee_err; // error value for +CME:<err>
   kal_uint8			online_cmd_state;
   
#ifdef __IRDA_SUPPORT__
   kal_uint8			ppp_nsapi;
#endif /* __IRDA_SUPPORT__ */
#ifndef __MMI_FMI__ 
#ifdef __SAT__
   rmmi_SAT_info_struct	     	rmmi_sat_info; //CSD00468 add for SAT using   
#endif /* __SAT__ */
#ifdef __RSAT__
  kal_bool is_sat_call_setup_ind;//CSD00758 - add for sat call setup 
#endif /*__RSAT__*/
#endif /* #ifndef __MMI_FMI__ */

#ifdef __SATC3__ 
   rmmi_uart_queue_struct	sat_output_queue; //CSD00714 add for SAT using
   kal_bool			IsSATcommand;	//check if SATCommand 
   kal_bool			IsSATresponse; //use to check if SAT response
   void * 			SAT_rsp_ptr;
#endif/* __SATC3__  */   

#ifndef MED_NOT_PRESENT
  kal_uint32   vr_session_id;
#endif

   kal_uint8				current_src;
	kal_uint8		imei_string[8]; //for EGMR=1,7,<imei>
	kal_uint8		imei_action; //rmmi_imei_action_enum
	kal_uint8		prefer_band;
	kal_bool         invalid_sim; //CSD00924 2005_0418 : when sim invalid there are some cmd not allowed
	kal_bool		rmmi_atd_mem;//CSD00758 - atd>
	kal_bool         abort_download_data; //CSD00924: for about +EFSR when USB plugoput
	kal_uint8        ringer_vol_level;//CSD00924 050704 : saving vol_level for multi cmd 
	kal_uint8        loudspeaker_vol_level;//CSD00924 050704 : saving vol_level for multi cmd 
	kal_uint16      write_data_size;	
	kal_uint8        transfered_to_dt_port;	
#ifdef __DT_SUPPORT__
       UART_TRANS_FUNC_PTR  trans_to_dt_func;
#endif /*__DT_SUPPORT__*/	

} rmmi_context_struct;



typedef struct rmmi_string_struct
{
   kal_uint16   index;
   kal_uint8   *string_ptr;
   kal_uint8   *ip_string; 
   kal_uint8	src_id;
   kal_uint8	cmd_mode;
   kal_uint16	cmd_index; //rmmi_extended_cmd_id_enum
} rmmi_string_struct;



#ifdef __SAT__

typedef enum
{
   RMMI_TIM_MOD_NONE,
   RMMI_TIM_MOD_SAT_DSPL_TEXT,
   RMMI_TIM_MOD_SAT_PLAY_TONE,
   RMMI_TIM_MOD_SAT_REDIAL,
   RMMI_TIM_MOD_SAT_NOTICE,
   RMMI_TIM_MOD_SAT_MMI_INFO,
   RMMI_NUM_OF_TIMER_MOD
} 
rmmi_timer_mod_enum;


typedef struct
{
   eventid timer_id;
   kal_uint8 time;
   kal_bool timer_on;
}rmmi_sat_timer_struct;

/* SAT */
#define SAT_SIG_SETUP_MENU_SHOW          70
#define SAT_SIG_SELECT_ITEM_SHOW          71
#define SAT_SIG_DSPL_TXT_SHOW             72
#define SAT_SIG_GET_INKEY_SHOW            73
#define SAT_SIG_GET_INPUT_SHOW            74
#define SAT_SIG_PLAY_TONE_SHOW            75
#define SAT_SIG_SETUP_CALL_SHOW           76
//#define SAT_SIG_SETUP_CALL_REQ_SHOW      77
//#define SAT_SIG_SETUP_CALL_RES_SHOW      78
//#define SAT_SIG_REDIAL_SHOW                79
#define SAT_SIG_SEND_SMS_SHOW            80
#define SAT_SIG_SEND_SS_SHOW              81
#define SAT_SIG_SEND_USSD_SHOW           82
#define SAT_SIG_MMI_INFO_SHOW             83
//#define SAT_SIG_NOTICE_SHOW                85


#define RMMI_SAT_SIG_SETUP_CALL_REQ      		16
#define RMMI_SAT_SIG_SEND_SS_REQ         		17
#define RMMI_SAT_SIG_SEND_USSD_REQ       		18
#define RMMI_SAT_SIG_SEND_SMS_REQ        		19
#define RMMI_SAT_SIG_PLAY_TONE_RES       		32
#define RMMI_SAT_SIG_DSPL_TEXT_RES       		33
#define RMMI_SAT_SIG_GET_INKEY_RES       		34
#define RMMI_SAT_SIG_GET_INPUT_RES       		35
#define RMMI_SAT_SIG_SELECT_ITEM_RES     		36
#define RMMI_SAT_SIG_MENU_SELECT_REQ    		37
#define RMMI_SAT_SIG_START_SAT           		80
#define RMMI_SAT_SIG_SETUP_CALL_REDIAL   		88




typedef struct
{
   kal_uint8 item_len;
   kal_uint8 item_dcs;
   kal_uint8 item_id;
   kal_uint8* item_data;
}
rmmi_sat_menu_item_struct;

typedef struct
{
   kal_uint8 is_help_info_available;
   kal_uint8 cmd_detail[5];
   kal_uint8 alpha_id_len;
   kal_uint8 *alpha_id;
   kal_uint8 dcs_of_alpha_id;
   kal_uint8 num_of_item;
   rmmi_sat_menu_item_struct *menu_items;
}
rmmi_sat_main_menu_struct;

typedef struct
{
   kal_uint8 cmd_detail[5];
   kal_uint8 is_help_info_available;
   kal_uint8 is_item_id_available;
   kal_uint8 item_id;
   kal_uint8 alpha_id_len;
   kal_uint8 *alpha_id;
   kal_uint8 dcs_of_alpha_id;
   kal_uint8 num_of_item;
   rmmi_sat_menu_item_struct *menu_items;
}
rmmi_sat_sub_menu_struct;

typedef struct
{
   kal_uint8 cmd_detail[5];
   kal_uint8 priority;
   kal_uint8 clear_text_type;
   kal_uint8 immediate_res;
   kal_uint16 no_text_string;
   kal_uint8 *text_string;
   kal_uint8 dcs_of_text_string;
}
rmmi_sat_dspl_text_struct;

typedef struct
{
   kal_uint8 cmd_detail[5];
   kal_uint8 is_help_info_available;
   kal_uint8 type_of_input;
   kal_uint16 no_text_string;
   kal_uint8 *text_string;
   kal_uint8 dcs_of_text_string;
}
rmmi_sat_get_inkey_struct;

typedef struct
{
   kal_uint8 cmd_detail[5];
   kal_uint8 is_help_info_available;
   kal_uint8 type_of_input;
   kal_uint8 is_input_revealed_to_user;
   kal_uint8 len_of_min_input;
   kal_uint8 len_of_max_input;

   kal_uint16 no_text_string;
   kal_uint8 *text_string;
   kal_uint8 dcs_of_text_string;
   kal_uint16 no_default_text;
   kal_uint8 *default_text;
   kal_uint8 dcs_of_default_text;
}
rmmi_sat_get_input_struct;

typedef struct
{
   kal_uint8 cmd_detail[5];
   kal_uint8 tone_id;
   kal_uint32 duration;
   kal_uint8 no_alpha_id;
   kal_uint8 *alpha_id;
   kal_uint8 dcs_of_alpha_id;
   
}
rmmi_sat_play_tone_struct;


typedef struct
{
   kal_uint8 cmd_detail[5];
   kal_uint8 mmi_info_type;
   kal_uint8 no_alpha_id;
   kal_uint8 *alpha_id;
   kal_uint8 dcs_of_alpha_id;
}
rmmi_sat_mmi_info_struct;

typedef struct {
   kal_uint8 cmd_detail[5];
   kal_uint8 condition;
   kal_uint32 duration;
   
   kal_uint8 no_alpha_id1;
   kal_uint8 *alpha_id1;
   kal_uint8 dcs_of_alpha_id1;
   kal_uint8 no_alpha_id2;
   kal_uint8 *alpha_id2;
   kal_uint8 dcs_of_alpha_id2;
   
   kal_uint8 size_of_ccp;
   kal_uint8 ccp[13];
   kal_uint8 size_of_subaddr;
   kal_uint8 subaddr[21];

   kal_uint8 no_addr;
   kal_uint8 *addr;
} 
rmmi_sat_setup_call_struct;

typedef struct {
   kal_uint8 cmd_detail[5];
   kal_uint8 is_sms_packed_required;
   kal_uint8 size_of_addr;
   kal_uint8 addr[41];
   kal_uint8 no_alpha_id;
   kal_uint8 *alpha_id;
   kal_uint8 dcs_of_alpha_id;
   kal_uint8 no_sms_tpdu;
   kal_uint8 *sms_tpdu;
} 
rmmi_sat_send_sms_struct;

typedef struct {
   kal_uint8 cmd_detail[5];
   kal_uint8 no_alpha_id;
   kal_uint8 *alpha_id;
   kal_uint8 dcs_of_alpha_id;
   kal_uint8 no_ss_string;
   kal_uint8 *ss_string;
} 
rmmi_sat_send_ss_struct;

typedef struct {
   kal_uint8 cmd_detail[5];
   kal_uint8 no_alpha_id;
   kal_uint8 *alpha_id;
   kal_uint8 dcs_of_alpha_id;
   kal_uint8 no_ussd_string;
   kal_uint8 *ussd_string;
   kal_uint8 dcs_of_ussd_string;
} 
rmmi_sat_send_ussd_struct;


/* TYPES */

typedef enum
{
  RMMI_VTS_SPEECH_IN  = 0,
  RMMI_VTS_SPEECH_OUT = 1,
  RMMI_VTS_KEY_BEEP  = 2,
  RMMI_VTS_DTMF_TONE = 3,
  RMMI_VTS_RINGER    = 4,
  RMMI_VTS_MEDIA    = 5,
  RMMI_NUM_OF_VTS_TYPE
} 
rmmi_vts_type_enum;

typedef enum
{
   RMMI_VTS_PLAY_CRESCENDO = 0, /* Play sound for crescendo. */
   RMMI_VTS_PLAY_INFINITE = 1, /* Play sound for infinite. */
   RMMI_VTS_PLAY_ONCE = 2, /* Play sound for once. */
   RMMI_VTS_PLAY_DESCENDO = 3, /* Play sound for descendo. */
   RMMI_VTS_PLAY_OFF = 4, /* stop sound */
   RMMI_NUM_OF_VTS_PLAY_MODE
} 
rmmi_vts_play_mode_enum;


#endif /* __SAT__*/


typedef struct
{
   kal_uint8   err_string[RMMI_MAX_ERR_STR_LEN];
} rmmi_error_msg_struct;

typedef void (*RMMI_EXT_CMD_FUNCTION)
                           (rmmi_string_struct *source_string_ptr);

typedef kal_bool (*RMMI_BASIC_CMD_FUNCTION)
                           (rmmi_string_struct *source_string_ptr,
                            rmmi_multiple_basic_cmd_struct *head_node_ptr,
                            rmmi_err_id_enum *err_id);

typedef void (*RMMI_IND_FUNCTION) ();


















#endif /* rmmi_context.h */


