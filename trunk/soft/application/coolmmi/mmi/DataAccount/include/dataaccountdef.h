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
 * DataAccountDef.h
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

#ifndef _COOLSAND_DATA_ACCOUNT_DEF_H
#define _COOLSAND_DATA_ACCOUNT_DEF_H

#ifndef MAX_DATA_ACCOUNT_NAME_LEN
#define MAX_DATA_ACCOUNT_NAME_LEN            31
#endif 
#define MAX_DIAL_NUM_LEN                  22
#define MAX_USER_LEN                   31
#define MAX_PASSWD_LEN                    31
#define MAX_IPADDRES_LEN                  4
#ifdef CUSTOM_DEFINED_MAX_CSD_PROFILE_NUM
#define MAX_DATA_ACCOUNT_GSM_LIMIT           CUSTOM_DEFINED_MAX_CSD_PROFILE_NUM
#else 
#define MAX_DATA_ACCOUNT_GSM_LIMIT           5
#endif 
#define MAX_PART_IP_ADDRESS                  4
#define DATA_ACCOUNT_HILITED_ITEM            0
#define DATA_ACCOUNT_UCS                  0x08
#define DATA_ACCOUNT_ASCII                0x00
#define MITEM_DATA_ACCOUNT_PARENT_ZERO       0
#define MAX_INLINE_ITEM_DATA_ACCOUNT_LIST    14
#define DATA_ACCOUNT_INLINE_DATA_RATE_OPTION 3
#define DATA_ACCOUNT_INLINE_DIAL_TYPE_OPTION 2
#define DATA_ACCOUNT_NOTIFY_TIMEOUT          UI_POPUP_NOTIFYDURATION_TIME

#ifdef __MMI_GPRS_FEATURES__
#define MAX_DATA_ACCOUNT_GPRS_LIMIT                10
#define MAX_INLINE_ITEM_GPRSDATA_ACCOUNT_LIST         10
#define GPRS_DATA_ACCOUNT_INLINE_AUTH_TYPE_OPTION     2
#define MAX_GPRS_DATAACCOUNT_NAME                  31
#ifdef MAX_GPRS_USER_NAME_LEN
#define MAX_GPRS_USERNAME                       MAX_GPRS_USER_NAME_LEN-1
#else 
#define MAX_GPRS_USERNAME                       15
#endif 
#ifdef MAX_GPRS_PASSWORD_LEN
#define MAX_GPRS_USERPASSWD                     MAX_GPRS_PASSWORD_LEN-1
#else 
#define MAX_GPRS_USERPASSWD                     15
#endif 
#ifdef MAX_GPRS_APN_LEN
#define MAX_GPRS_MMI_APN_LEN                    MAX_GPRS_APN_LEN-1
#else 
#define MAX_GPRS_MMI_APN_LEN                    99
#endif 

#endif /* __MMI_GPRS_FEATURES__ */ 

#endif /* _COOLSAND_DATA_ACCOUNT_DEF_H */ 

