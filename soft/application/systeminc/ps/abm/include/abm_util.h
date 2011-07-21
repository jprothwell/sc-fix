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
 * ABM_UTIL.H
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file contains the utilities functions of ABM.
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
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _ABM_UTIL_H
#define _ABM_UTIL_H

/* Minimum network account ID */
#define ABM_MIN_NWACC_ID 0

extern kal_uint8 abm_get_total_bearer(void);

INLINE kal_bool abm_nw2bearer_id(
                    kal_uint8 nw_acc_id,
                    /* IN */ kal_uint8 *bearer,
                    /* OUT */ kal_uint8 *bearer_id /* OUT */ )
{
	mmi_trace(1,"abm_nw2bearer_id>>>>>>>>>>>>>>>nw_acc_id:%d",nw_acc_id);
	if(CUSTOM_DEFINED_MAX_CSD_PROFILE_NUM > nw_acc_id)
	{
		*bearer = ABM_E_SOCKET_BEARER_GSM_CSD;
		*bearer_id = nw_acc_id;
	}
	else if((CUSTOM_DEFINED_MAX_CSD_PROFILE_NUM + MAX_GPRS_PROFILE_NUM) > nw_acc_id)
	{
		*bearer = ABM_E_SOCKET_BEARER_GSM_GPRS;
		*bearer_id = nw_acc_id - 9;
	}
	return KAL_TRUE;
}

INLINE kal_bool abm_bearer2nw_id(
                    kal_uint8 bearer,
                    /* IN */ kal_uint8 bearer_id,
                    /* IN */ kal_uint8 *nw_acc_id /* OUT */ )
{
	mmi_trace(1,"abm_bearer2nw_id>>>>>>bearer:%d,bearer_id:%d",bearer,bearer_id);
	if(bearer == ABM_E_SOCKET_BEARER_GSM_CSD)
	{
		*nw_acc_id = bearer_id - 1;
	}
	else if(bearer == ABM_E_SOCKET_BEARER_GSM_GPRS)
	{
		*nw_acc_id = bearer_id + 9;
	}
	
	return KAL_TRUE;
}

extern kal_bool abm_check_msg(ilm_struct *ilm_ptr);

extern void abm_send_msg2l4c(msg_type_t msg_id, local_para_struct *p_local_para);

extern void abm_send_cnf2soc(
                msg_type_t msg_id,
                kal_uint8 bearer,
                kal_uint8 nw_acc_id,
                kal_uint8 *ip_addr,
                kal_uint8 *pri_dns_addr,
                kal_uint8 *sec_dns_addr,
                kal_uint16 mtu);

extern void abm_send_rej2soc(msg_type_t msg_id, kal_uint8 bearer, kal_uint16 err_cause, kal_uint8 nw_acc_id);

#ifdef __GPRS_MODE__

extern void abm_send_req2nvram(msg_type_t msg_id, local_para_struct *p_local_para, peer_buff_struct *p_peer_buf);

extern kal_bool l4cabm_get_gprs_account_info(l4c_gprs_account_info_struct *info);

extern void abm_restore_sib_to_nvram(kal_uint16 record_no, kal_uint8 access_id);

extern void abm_send_get_gprs_acct_cnf(
                kal_uint8 src_id,
                kal_uint8 result,
                kal_uint8 err_cause,
                l4c_gprs_account_info_struct *account_info);

extern kal_bool abm_get_gprs_slot(kal_uint8 state, kal_uint8 *id);
extern kal_bool abm_get_gprs_slot_by_acct_id(kal_uint8 nw_acct_id, kal_uint8 *id);

#endif /* __GPRS_MODE__ */ 

#endif /* _ABM_UTIL_H */ 

