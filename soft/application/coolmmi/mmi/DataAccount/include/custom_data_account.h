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
 * customer_data_account.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for include file of PS customization parameters. 
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#ifndef _CUSTOM_DATA_ACCOUNT_H
#define _CUSTOM_DATA_ACCOUNT_H

#define CUSTOM_DEFINED_MAX_CSD_PROFILE_NUM  (10)
#define MAX_GPRS_PROFILE_NUM (10)

/*
 * we only support 10 profiles per bearer due to following reasons
 
 * 2) default profile name string only define for 1~10.
 */
#if (CUSTOM_DEFINED_MAX_CSD_PROFILE_NUM > 10)
#error currently, we only support 10 CSD profiles for each bearer
#endif

extern kal_uint8 custom_get_gprs_profile_num(void);
extern kal_uint8 custom_get_csd_profile_num(void);
extern kal_int32 custom_get_gprs_profile_size(void);

#ifndef __L1_STANDALONE__

#ifdef __GPRS_MODE__
extern nvram_ef_abm_gprs_profile_struct  abm_gprs_profile[];
extern kal_uint8 abm_gprs_state[];
extern kal_uint8 abm_gprs_bearer_id[];   
#endif

//#ifdef __TCPIP__
extern nvram_ef_csd_profile_struct  csd_prof[CUSTOM_DEFINED_MAX_CSD_PROFILE_NUM];
//#endif /* __TCPIP__ */

#endif /* __L1_STANDALONE__ */


#endif /* _CUSTOM_DATA_ACCOUNT_H */

