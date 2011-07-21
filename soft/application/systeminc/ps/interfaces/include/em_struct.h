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
 *   em_struct.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Engineer mode related structures.
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

 
#ifndef _EM_STRUCT_H
#define _EM_STRUCT_H

#ifdef MMI_ON_WIN32 /* zhuoxz,2009-5-18 */
#include "kal_non_specific_general_types.h"
#include "stack_config.h"
#endif

typedef kal_uint32 request_info_type;

typedef enum
{
   /* RR */ 
   RR_EM_CELL_SELECT_PARA_INFO   = 0x00000001 ,
   RR_EM_CHANNEL_DESCR_INFO      = 0x00000002 ,
   RR_EM_CTRL_CHANNEL_DESCR_INFO = 0x00000004 ,
   RR_EM_RACH_CTRL_PARA_INFO     = 0x00000008 ,
   RR_EM_LAI_INFO                = 0x00000010 ,
   RR_EM_RADIO_LINK_COUNTER_INFO = 0x00000020 ,
   RR_EM_MEASUREMENT_REPORT_INFO = 0x00000040 ,
   
   /* CC */ 
   CC_EM_CHANNEL_INFO            = 0x00000080 ,
   CC_EM_CALL_INFO               = 0x00000100 ,
   
   /* CB */
   CB_EM_INFO                    = 0x00000200 ,
   
   /* SS */
   SS_EM_INFO                    = 0x00000400 ,
   
   /* MM */
   MM_EM_INFO                    = 0x00000800 ,
                                        
   /* UEM */                            
   UEM_EM_BATTERY_INFO           = 0x00001000 ,
   
   /* RR new structure */
   RR_EM_CONTROL_MSG_INFO        = 0x00002000 ,
   /* gprs em begins */
   RR_EM_TBF_INFO                = 0x00004000 ,
   RR_EM_GPRS_GENERAL_INFO       = 0x00008000 ,
   GMM_EM_INFO                   = 0x00010000 ,
   TCM_EM_EXT_PDP_INFO           = 0x00020000 ,
   TCM_EM_INT_PDP_INFO           = 0x00040000 ,
   SNDCP_EM_INFO                 = 0x00080000 ,
   LLC_EM_INFO                   = 0x00100000 ,
   PPP_EM_INFO                   = 0x00200000 ,
   SM_EM_INFO                    = 0x00400000 
   
} em_info_enum;

typedef enum                     
{
	EM_NOT_ACTIVE,
	EM_FROM_MMI,
	EM_FROM_CATCHER,
	EM_FROM_RMMI	/* tommy add for RMMI EM support */
} em_source_enum;

typedef enum
{
	EM_CAUSE_OK,
	EM_CAUSE_INVALID_INFO_TYPE
} em_err_cause_enum;

/* local parameter structures */
/* For L4CPS_EM_START_REQ, L4CPS_EM_STOP_REQ */
typedef struct
{
  kal_uint8 ref_count;
  kal_uint16 msg_len;
   request_info_type	info_request;
} l4cps_em_start_req_struct;

typedef l4cps_em_start_req_struct l4cps_em_stop_req_struct;

/* For L4CPS_EM_START_CNF, L4CPS_EM_STOP_CNF */
typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;
   kal_bool	result;
   kal_uint8	cause;
   request_info_type	info_request;
} l4cps_em_start_cnf_struct;

typedef l4cps_em_start_cnf_struct l4cps_em_stop_cnf_struct;

/* For L4CPS_EM_STATUS_IND */
typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;
   em_info_enum	em_info;
} l4cps_em_info_req_struct;

typedef l4cps_em_info_req_struct l4cps_em_status_ind_struct;

/* For TSTL4C_EM_START_REQ, TSTL4C_EM_STOP_REQ */
typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;
   module_type mod_id;
   request_info_type	info_request;
} tstl4c_em_start_req_struct;

typedef tstl4c_em_start_req_struct tstl4c_em_stop_req_struct;


/* peer buffer structures */
/* RR structure */		
typedef struct
{
	kal_uint8	crh;   //CELL-RESELECT-HYSTERESIS
	kal_uint8	ms_txpwr;   // 	maximum TX power level an MS may use when accessing on a CCH
	kal_uint8	rxlev_access_min;   //	minimum received signal level at the MS for which it is permitted to access the system
} rr_em_cell_select_para_info_struct;
			
typedef struct
{
	kal_uint8	channel_type;   //	channel type
	kal_uint8	tn;   //	timeslot number
	kal_uint8	tsc;   //	training sequence code
	kal_uint8	hopping_flag;   //	hopping or not
	kal_uint8	maio;   //	MAIO value
	kal_uint8	hsn;   //	HSN value
	kal_uint8	num_of_carriers;   //	number of carriers in the BA list(in the non-hopping case, this shall be set to 1)
	kal_uint16	arfcn[64];   //	ARFCN value in the MA
	kal_bool     is_BCCH_arfcn_valid; /*the BCCH_arfcn is only valid 
	                              when receives H.O cmd in DEDI-state*/
	kal_uint16	BCCH_arfcn;   //	Target_BCCH carrier
} rr_em_channel_descr_info_struct;	//This shall be used in IDLE and DEDI state
			
typedef struct
{	
	kal_uint8	mscr;   //	MSC release
	kal_uint8	att;   //	att flag
	kal_uint8	bs_ag_blks_res;   //	BS-AG-BLKS-RES
	kal_uint8	ccch_conf;   //	CCCH-CONF
	kal_uint8	cbq2;   //	Cell Bar Qualify 2
	kal_uint8	bs_pa_mfrms;   //	BS-PA-MFRMS
	kal_uint8	t3212;   //	T3212 timeout value
} rr_em_ctrl_channel_descr_info_struct;
			
typedef struct
{		
	kal_uint8	max_retrans;    
	kal_uint8	tx_integer;   //	TX integer
	kal_uint8	cba;   //	cell bar for access
	kal_uint8	re;   //	call reestablishment allowed
	kal_uint8	acc_class[2];   //	acces control class
} rr_em_rach_ctrl_para_info_struct;
			
typedef struct
{			
	kal_uint8	mcc[3];	//MCC
	kal_uint8	mnc[3];	//MNC
	kal_uint8	lac[2];	//LAC
	kal_uint16	cell_id;	 //cell ID
} rr_em_lai_info_struct;
					
typedef struct
{			
	kal_uint16	max_value;   //	timeout value for the DSF or RLF
	kal_int16	current_value;   //	current value in the DSF or RLF calculation
	kal_uint8	dtx_ind;	//DTX indicator
	kal_uint8	dtx_used;	//DTX used or not
	
} rr_em_radio_link_counter_info_struct;  //This shall be used in IDLE and DEDI state
			
typedef struct
{
	
	kal_uint8	rr_state;	//current RR state	1: IDLE, 2: ACCESS, 3: PKT_TRANSFER 4: DEDICATED
	kal_uint16	serving_arfcn;	//serving cell ARFCN
	kal_uint8	serving_bsic;	//serving cell BSIC
	kal_uint8	serv_gprs_supported;	//serving cell support GPRS or not		
	kal_int16	serv_rla_in_quarter_dbm;	/*rssi level for serving cell (BCCH
                   	                  in IDLE-state) & (TCH in DEDI-state) & (PDTCH in TRANSFER-state) */
    kal_bool     is_serv_BCCH_rla_valid;  /*To indicate if the serv_BCCH rssi is valid*/                   	                     
	kal_int16	serv_BCCH_rla_in_dedi_state;	/*rssi level for serving cell (BCCH) in DEDI-State*/
	kal_uint8	quality;	//serving cell -TCH measured quality	

   /* 2003-0919, add new C31/C32 cvalue information for GPRS service */
   kal_bool     gprs_pbcch_present; // To indicate if the current cell supports GPRS & PBCCH is present

   /* 2003-0919, add new C31/C32 cvalue information for GPRS service */
   kal_bool     gprs_c31_c32_enable; // To indicate if the current mode is GPRS_PMO_MODE or GPRS_PCCCH_MODE
	kal_int16	c31[32];	//C31 value for the neighbor cell	   

	kal_int16	c1_serv_cell;	//C1 value for the serving cell
	kal_int16	c2_serv_cell;	/* if gprs_c31_c32_enable is false, this field is shown as c2_value for serv_cell
	                              if gprs_c31_c32_enable is true, this field is shown as c32_value for serv_cell */
	kal_int16	c31_serv_cell;	/* if gprs_c31_c32_enable is false, this field is ignore
	                              if gprs_c31_c32_enable is true, this field shall be shown in based on report c31_value*/
	                                 

	kal_uint8	num_of_carriers;	//number of carriers in the BA list
	kal_uint16	nc_arfcn[32];	//ARFCN value in the BA list(The list will be sorted by the RSSI level)
	kal_int16	rla_in_quarter_dbm[32];	//rssi level for each carrier
	kal_uint8	nc_info_status[32];	/*Bit0 = 0: "nc_bsic","frame_offset","ebit_offset" is invalid
									Bit0 = 1: "nc_bsic","frame_offset","ebit_offset" is valid
             									Bit1 = 0: "c1","c2" is invalid
             									Bit1 = 1: "c1","c2" is valid 
             									Bit2 = 0: "gprs_status" is invalid
             									Bit2 = 1: "gprs_status" is valid */
	kal_uint8	nc_bsic[32];	//neighbor cell BSIC
	kal_int32	frame_offset[32];	//frame offset for each carrier
	kal_int32	ebit_offset[32];	//ebit offset for each carrier
	kal_int16	c1[32];	//C1 value for the neighbor cell
	kal_int16	c2[32];	/* if gprs_c31_c32_enable is false, this field is shown as c2_value for nbr_cell
                           if gprs_c31_c32_enable is true, this field is shown as c32_value for nbr_cell */

	
	//kal_uint8	nbr_gprs_supported[32];	//0: not supported, 1: supported
	kal_uint8	multiband_report;	//MULTIBAND_REPORT value
} rr_em_measurement_report_info_struct;	//This shall be used in IDLE and DEDI state;
			
typedef struct
{	
	kal_uint8	msg_type;	/*receives msg type from network in Dedi-State
	                          0: Assignment Command
	                          1: Channel Release
	                          2: Additional Assignment 
 
	                          4: Ciphering Mode Command
	                          5: Configure Change Command
	                          6: Handover Command
	                          7: Frequency Redefinition
	                          8: Channel Mode Modify
	                          9: Cell Selection Indicator*/
	                           	                           
} rr_em_control_msg_info_struct;

/* CC structure */
typedef struct
{
   kal_uint8   cause;	//channel sync cause
   kal_uint8   channel_type;	//channel type
   kal_uint8   channel_mode;	//channel mode
} cc_em_channel_info_struct;

typedef struct
{
	kal_uint8	call_id;	//call id
	kal_uint8	ti;	//transaction identifier
	kal_uint8	state;	//call state
	kal_uint8	aux_state;	//auxiliary state
   kal_uint8	pi; //progress indicator
	kal_uint16	cause;	//call disconnect cause
	kal_uint8	cause_location;	//cause originating locatioin   
} cc_em_call_info_struct;

/* CB structure */
typedef struct
{
	kal_uint16	sn;	//serial number
	kal_uint16	mi;	//message identifier
	kal_uint8	dcs;	//data code scheme
	kal_uint8	page;	//current number of this page
	kal_uint8	pages;	//total number of pages
} cb_em_info_struct;
     
/* SS structure */
typedef struct
{			
	kal_uint8	ss_state;	//ciss state
	kal_uint8	ti;	//transaction identifier
	kal_uint8	call_id;	//call id
	kal_uint8	invoke_id;	//invoke id
	kal_uint8	num_invoke;	//number of active invokes
	kal_uint8	op_code;	//operation code
	kal_uint8	ss_code;	//supplementary service code
	kal_uint8	bs_code;	//basic service code
} ss_em_info_struct;

/* MM structure */ 
typedef struct
{
	kal_uint8	t3212_val;	//T3212 timer value
	kal_uint8	att_flag;	//ATT flag
	kal_uint8	mm_cause;	//MM Reject cause, if any
	kal_uint8	mm_state;	//MM State
   kal_uint8	mcc[3];	//MCC
   kal_uint8	mnc[3];	//MNC
	kal_uint8   loc[2];	 //registered LAC
   kal_uint8   rac;     //registered RAC
	kal_uint8	tmsi[4];	//TMSI, if valid
} mm_em_info_struct;

/* UEM structure */
typedef struct
{
	kal_int32	adc_value;	//battery ADC value
	kal_uint8	battery_level;	//battery level
	kal_uint8	battery_status;	//battery status: battery_status_enum
} uem_em_battery_info;




typedef struct
{
   kal_uint8   flc_buffer_type;     // reference to gprs_flc_buffer_enum
   kal_uint8   pool_active;         // true or false
   kal_uint8	percentage_upp_thre; //percentage of upper threshold
   kal_uint8	percentage_low_thre; //	percentage of lower threshold
   kal_uint32	total_size;          //	total size of pol
   kal_uint32	curr_occ;            //	current occupied
   kal_uint16	hit_upp_thre_counts; //	upper threshold hit counter
   kal_uint16	hit_low_thre_counts; //	lower threshold hit counter
} flc_em_pool_status_struct;

extern kal_bool flc_em_get_pool_staus(kal_uint8 poolid,
                     flc_em_pool_status_struct *status);


typedef struct
{
	kal_uint8	ul_tbf_status;      //	access/active/inactive
	kal_uint8	ul_rel_cause;       //	if applicable
	kal_uint8	ul_ts_allocation;   // timeslot allocation  
	kal_uint8	ul_coding_scheme; 
	kal_uint8   ul_rlc_mode;
	kal_uint8   ul_mac_mode;
			
	kal_uint8	dl_tbf_status;     //	active/inactive/release pending
	kal_uint8	dl_rel_cause;      //	if applicable
	kal_uint8	dl_ts_allocation;  //	 timeslot allocation
	kal_uint8   dl_rlc_mode;
	kal_uint8   dl_mac_mode;
	kal_uint8   dl_coding_scheme;
	
}rr_em_tbf_status_struct;

typedef struct 
{
	kal_uint32	t3192;  // in msec
	kal_uint32	t3168;  // in msec
	kal_uint8   rp;     // radio priority
}rr_em_gprs_general_info_struct;

typedef struct
{
	kal_uint32	num_pkt_txed;    // number of packets transferred
	kal_uint32	num_pkt_rxed;    // number of packets received.
	kal_uint32	num_octets_txed;  // number of octets transferred
	kal_uint32	num_octets_rxed;	// number of octets received.
}tcm_em_pdp_info_struct;

typedef struct
{
   kal_uint8   nsapi; // NSAPI 
	kal_uint8	pdp_context_status; //	active/inactive
	kal_uint8	rej_cause;          //	if applicable
	kal_uint8	reliability_class;  //	reliabillity class of QoS
	kal_uint8	rp;  //radio priority
} sm_em_pdp_info_struct;

typedef struct{
   sm_em_pdp_info_struct pdp[2];
} sm_em_info_struct;

typedef struct
{
	kal_uint16	ready_timer; //	Ready timer value in SECONDS.
	kal_uint16	rau_timer;   //	Periodical Routing Area Updating timer value in deci-hours
	kal_uint8	ms_state;    //	    MS_NULL_STATE = 0,
                            //    MS_READY_STATE,
                            //    MS_STANDBY_STATE,
                            //    MS_FORCED_TO_STANDBY_STATE
}gmm_em_info_struct;



typedef struct
{
	kal_uint32	num_checksum_error;
	kal_uint32  requested_ip;
	kal_uint32  negotiated_ip;
	kal_uint32  requested_dns;
	kal_uint32  negotiated_dns;
} ppp_em_info_struct;


typedef struct
{
	kal_uint32	num_reasm_failure;
} sndcp_em_info_struct;

typedef struct
{
	kal_uint32	num_fcs_error;
} llc_em_info_struct;

#endif /* ~_EM_STRUCT_H */

