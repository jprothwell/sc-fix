/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
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
 * L4C2ABM_STRUCT.H
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file describes the common struct of ABM and L4C.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * removed!
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


#ifndef _L4C2ABM_STRUCT_H
#define _L4C2ABM_STRUCT_H

typedef struct {
   kal_uint8 ref_count;
   kal_uint16 msg_len;
   kal_bool result;
   kal_uint16 err_cause;
   kal_uint8 profile_id;
   kal_uint8 ip_addr[4];
   kal_uint8 pri_dns_addr[4];
   kal_uint8 sec_dns_addr[4];    
} l4c_ps_abm_setup_rsp_struct;

typedef struct {
   kal_uint8 ref_count;
   kal_uint16 msg_len;
   kal_bool result;
   kal_uint16 err_cause;
   kal_uint8 profile_id;
} l4c_ps_abm_disc_rsp_struct;

typedef struct {
   kal_uint8 ref_count;
   kal_uint16 msg_len;  
   kal_uint8 profile_id;
   kal_uint16 err_cause;
} l4c_ps_abm_disc_ind_struct;

typedef struct {
   kal_uint8 ref_count;
   kal_uint16 msg_len;
   kal_bool result;
   kal_uint16 err_cause;   
   kal_uint8 ip_addr[4];
   kal_uint8 pri_dns_addr[4];
   kal_uint8 sec_dns_addr[4];
   kal_uint16 mtu;    
} l4c_cc_abm_setup_rsp_struct;

typedef struct {
   kal_uint8 ref_count;
   kal_uint16 msg_len;
   kal_bool result;
   kal_uint16 err_cause;
} l4c_cc_abm_disc_rsp_struct;   

typedef struct {
   kal_uint8 ref_count;
   kal_uint16 msg_len;  
   kal_uint8 csd_profile_id;
   kal_uint16 err_cause;
} l4c_cc_abm_disc_ind_struct;   
   
typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 src_id;
} l4cabm_start_req_struct;

typedef nvram_ef_abm_gprs_profile_struct l4c_gprs_account_info_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 src_id;
    l4c_gprs_account_info_struct gprs_account_info;
} l4cabm_set_gprs_account_info_req_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 src_id;
    kal_uint8 profile_id;
} l4cabm_get_gprs_account_info_req_struct;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 src_id;
    l4c_gprs_account_info_struct gprs_account_info;
} l4cabm_get_gprs_account_info_rsp_struct;

#endif /* ~_L4C2ABM_STRUCT_H */


