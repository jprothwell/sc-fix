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
 * PS2SIM_STRUCT.H
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for SIM's message
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

/********************************************************************************
*  Copyright Statement:
*  --------------------
*
*  This product has been developed using a protocol stack
*  developed by Sasken Communication Technologies Limited.
*
********************************************************************************/

/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2002
*
*******************************************************************************
 *  FILENAME : ps2sim_struct.h
 *  SYNOPSIS : Functions for encoding/decoding.
 *
 *                      R E V I S I O N    H I S T O R Y
 *
 */
#ifndef _PS2SIM_STRUCT_H
#define _PS2SIM_STRUCT_H

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 mode;
    kal_uint8 access_id;
    kal_uint8 pin1[8];
} sim_start_req_struct;

typedef struct {
    kal_uint8 chv1_count;
    kal_uint8 chv2_count;
    kal_uint8 ubchv1_count;
    kal_uint8 ubchv2_count;
    kal_uint8 is_chv1_enabled;
    kal_uint8 is_chv1_verified;
} sim_chv_info_struct;

typedef struct {
    kal_uint8 chv1_status;
    kal_uint8 chv2_status;
    kal_uint8 ubchv1_status;
    kal_uint8 ubchv2_status;
} sim_chv_status_struct;

typedef struct {
    kal_uint8 num_lp;
    kal_uint8 lp_file_type;
    kal_uint8 lp[10];
} sim_lp_struct;

typedef struct {
    kal_uint8 num_ecc;
    kal_uint8 ecc[15];
} sim_ecc_struct;

typedef struct {
    unsigned int no_plmn_data;
    unsigned int plmn_data_size;
    kal_uint8 *plmn_data;
} sim_plmn_sel_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint16 result;
    sim_chv_info_struct chv_info;
    sim_lp_struct lp;
    sim_ecc_struct ecc;
    kal_uint8 access_id;
} sim_start_cnf_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 cause;
} sim_error_ind_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint16 result;
    kal_uint8 ef_imsi[9];
    kal_uint8 is_valid_gid1;
    kal_uint8 gid1[20];
    kal_uint8 is_valid_gid2;
    kal_uint8 gid2[20];
/* moved from mmi_ready_ind */
    kal_uint8 is_valid_plmnsel;
    unsigned int no_plmn_data;
    unsigned int plmn_data_size;
    kal_uint8 *plmn_data;
    kal_uint8 is_valid_dck;
    kal_uint8 dck[16];
} sim_mmi_security_ind_struct;

typedef struct {
   kal_uint8 alpha_id_size;
   kal_uint8 alpha_id[32];
   kal_uint8 bcd_len;
   kal_uint8 bcd_digit[41];
} sim_addr_struct;

typedef struct {
   kal_uint8 alpha_id_size;
   kal_uint8 alpha_id[21];
   kal_uint8 ton_npi;
   kal_uint8 digit_size;
   kal_uint8 digit[4];//[41];
   kal_uint8 index_level;
   kal_bool network_specific;
   kal_bool premium_rate;
} sim_info_num_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint16 result;
    kal_uint8 dn_type;
    kal_uint8 phase;
    kal_uint8 is_valid_puct;
    kal_uint8 ef_puct[5];
    kal_uint8 is_spn_valid;
    kal_uint8 spn[17];
    kal_uint8 is_opname_valid;
    kal_uint8 opname[20];
    kal_uint8 is_sopname_valid;
    kal_uint8 sopname[10]; 
   kal_uint8 no_msisdn;        // MSISDN
   sim_addr_struct msisdn[2];
   kal_uint8 no_info_num;      // Information Numbers
   sim_info_num_struct *info_num;
   kal_bool is_valid_csp;
   kal_uint8 csp[22];          // Custom Service Profile
} sim_mmi_ready_ind_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 ef_imsi[9];
    kal_uint8 is_valid_ef_kc;
    kal_uint8 ef_kc[9];
    kal_uint8 ef_acc[2];
    kal_uint8 is_valid_ef_loci;
    kal_uint8 ef_loci[11];
    kal_uint8 is_valid_ef_gkc;
    kal_uint8 ef_gkc[9];
    kal_uint8 is_valid_ef_gloci;
    kal_uint8 ef_gloci[14];
} sim_mm_ready_ind_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 is_valid_ms_op_mode;
    kal_uint8 ms_op_mode;
    kal_uint8 ef_imsi[9];
    kal_uint8 is_valid_hplmn_time;
    kal_uint8 ef_hplmn_time;
    kal_uint8 is_valid_ef_bcch;
    kal_uint8 ef_bcch[16];
    kal_uint8 ef_acc[2];
    kal_uint8 is_valid_ef_fplmn;
    kal_uint8 ef_fplmn[12];
    kal_uint8 is_valid_ef_loci;
    kal_uint8 ef_loci[11];
    kal_uint8 is_valid_ef_gloci;
    kal_uint8 ef_gloci[14];
    kal_uint8 is_valid_plmnsel;
    unsigned int no_plmn_data;
    unsigned int plmn_data_size;
    kal_uint8 *plmn_data;    
} sim_rr_ready_ind_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_bool    cipher_ind;   // 20050202 Benson add for display cipher indication
} sim_mmrr_ready_ind_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
} sim_cc_ready_ind_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
} sim_sms_ready_ind_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 random_val[16];
} sim_run_algo_req_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 cipher_key[8];
    kal_uint8 sres[4];
} sim_run_algo_cnf_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 file_idx;
    kal_uint16 para;
    kal_uint16 length;
    kal_uint8 access_id;
} sim_read_req_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 file_idx;
    kal_uint16 para;
    kal_uint16 length;
    kal_uint8 data[256];
    kal_uint16 result;
    kal_uint16 status_word;
    kal_uint8 access_id;
} sim_read_cnf_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 file_idx;
    kal_uint16 para;
    kal_uint16 length;
    kal_uint8 data[260];
    kal_uint8 access_id;
} sim_write_req_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 file_idx;
    kal_uint16 para;
    kal_uint16 result;
    kal_uint16 status_word;
    kal_uint8 access_id;
} sim_write_cnf_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 file_idx;
    kal_uint8 data[3];
} sim_increase_req_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 file_idx;
    kal_uint8 data[256];
    kal_uint16 result;
    kal_uint8 length;
} sim_increase_cnf_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 op;
    kal_uint8 which_chv;
    kal_uint8 old_value[8];
    kal_uint8 new_value[8];
    kal_uint8 access_id;
} sim_security_req_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint16 result;
    kal_uint8 op;
    kal_uint8 which_chv;
    kal_uint8 access_id;
    sim_chv_info_struct chv_info;
} sim_security_cnf_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 access_id;
    kal_uint8 file_idx;
    kal_uint8 info_type;
} sim_file_info_req_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 access_id;
    kal_uint8 file_idx;
    kal_uint16 length;
    kal_uint16 result;
    kal_uint16 file_size;
    kal_uint8 num_of_rec;
} sim_file_info_cnf_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 file_idx;
    kal_uint8 pattern[16];
    kal_uint8 length;
    kal_uint8 mode;
} sim_seek_req_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 file_idx;
    kal_uint8 length;
    kal_uint16 result;
    kal_uint8 data[256];
} sim_seek_cnf_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 access_id;
    kal_uint8 dn_type;
    kal_uint8 switch_on;
    
} sim_dial_mode_req_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 access_id;
    kal_uint16 result;
    kal_uint8 dn_type;
    kal_uint8 switch_on;    
} sim_dial_mode_cnf_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 access_id;
} sim_status_req_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 access_id;
    kal_uint16 result;
    kal_uint8 data[22];
    kal_uint16 status_word;
} sim_status_cnf_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 switch_mode;
} sim_error_test_req_struct;

#endif 


