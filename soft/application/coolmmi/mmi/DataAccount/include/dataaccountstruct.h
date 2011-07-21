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
 * DataAccountStruct.h
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
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#ifndef _COOLSAND_DATA_ACCOUNT_STRUCT_H
#define _COOLSAND_DATA_ACCOUNT_STRUCT_H

typedef struct _service_data_account_node
{
    U8 Present;                                             /* If this item is presented */
    U8 DataAccountName[(MAX_DATA_ACCOUNT_NAME_LEN + 1) *ENCODING_LENGTH];      /* Name of a Data Account */
    U8 DialNumber[(MAX_DIAL_NUM_LEN + 1) *ENCODING_LENGTH]; /* Dial Number of a Data Account */
    U8 UserName[MAX_USER_LEN + 1];                          /* User of a Data Account */
    U8 Passwd[MAX_PASSWD_LEN + 1];                          /* Password for a Data Account */
    U8 DialType;                                            /* Boolean : 0.Analoue or 1. SDN */
    U8 DataRate;                                            /* Boolean : 0.9.6 kbit/s or 1. 14.4 Kbit/s */
    U8 DnsAddress[MAX_PART_IP_ADDRESS];                     /* DNS IP Address */
    U8 iFdcs;                                               /* DCS indicator for account name. ascii:0, ucs2:length */
} DataAccountNode;

typedef struct _service_data_account_node_display
{
    S32 DialType;
    S32 DataRate;
#ifdef __MMI_GPRS_FEATURES__
    S32 AuthType;   /* pdp_auth_enum */
    U8 APN[(MAX_GPRS_MMI_APN_LEN + 1) *ENCODING_LENGTH];
    U8 *AuthTypeOption[GPRS_DATA_ACCOUNT_INLINE_AUTH_TYPE_OPTION];
    U8 GPRSCurrGetIndex;
    U8 GPRSCurrSelectItem;
    /* micha0112 */
    U8 GPRSCurrAppSelectItem;
    FuncPtrShort GPRSAppCallBack;
#endif /* __MMI_GPRS_FEATURES__ */ 
    U8 DataAccountName[(MAX_DATA_ACCOUNT_NAME_LEN + 1) *ENCODING_LENGTH];
    U8 DialNumber[(MAX_DIAL_NUM_LEN + 1) *ENCODING_LENGTH];
#ifdef MAX_GPRS_USERNAME
#if ( MAX_USER_LEN > MAX_GPRS_USERNAME)
    U8 UserName[(MAX_USER_LEN + 1) *ENCODING_LENGTH];
#else 
    U8 UserName[(MAX_GPRS_USERNAME + 1) *ENCODING_LENGTH];
#endif 
#else /* MAX_GPRS_USERNAME */ 
    U8 UserName[(MAX_USER_LEN + 1) *ENCODING_LENGTH];
#endif /* MAX_GPRS_USERNAME */ 

#ifdef MAX_GPRS_USERNAME
#if (MAX_PASSWD_LEN > MAX_GPRS_USERNAME)
    U8 Passwd[(MAX_PASSWD_LEN + 1) *ENCODING_LENGTH];
#else 
    U8 Passwd[(MAX_GPRS_USERNAME + 1) *ENCODING_LENGTH];
#endif 
#else /* MAX_GPRS_USERNAME */ 
    U8 Passwd[(MAX_PASSWD_LEN + 1) *ENCODING_LENGTH];
#endif /* MAX_GPRS_USERNAME */ 

    U8 IP1[MAX_IPADDRES_LEN *ENCODING_LENGTH];
    U8 IP2[MAX_IPADDRES_LEN *ENCODING_LENGTH];
    U8 IP3[MAX_IPADDRES_LEN *ENCODING_LENGTH];
    U8 IP4[MAX_IPADDRES_LEN *ENCODING_LENGTH];
    U16 IconList[MAX_INLINE_ITEM_DATA_ACCOUNT_LIST];
    U8 *DialTypeOption[DATA_ACCOUNT_INLINE_DIAL_TYPE_OPTION];
    U8 *DialRateOption[DATA_ACCOUNT_INLINE_DATA_RATE_OPTION];
    U8 CurrGetIndex;
    U8 CurrSelectItem;
    /* micha0112 */
    U8 CurrAppMainSelectItem;
    U8 CurrAppSelectItem;
    FuncPtrShort AppCallBack;
    U16 AppMenuID;
    FuncPtrShort app_callback;
    FuncPtr bearer_callbacks[MAX_SUB_MENUS];
    U8 bearers;
} DataAccountNodeDisplay;

#ifdef __MMI_GPRS_FEATURES__

typedef struct _gprs_service_data_account_node
{
    U8 Present;                                             /* If this item is presented */
    U8 AuthType; 							/* Authentication Type */
    U8 DataAccountName[(MAX_GPRS_DATAACCOUNT_NAME + 1) *ENCODING_LENGTH];      /* Name of a Data Account */
	
    U8 APN[(MAX_GPRS_MMI_APN_LEN + 1) *ENCODING_LENGTH];    /* GPRS APN */
                                               
    U8 UserName[MAX_GPRS_USERNAME + 1];                     /* User of a Data Account */
    U8 Passwd[MAX_GPRS_USERPASSWD + 1];                     /* Password for a Data Account */
    U8 DnsAddress[MAX_PART_IP_ADDRESS];                     /* DNS IP Address */
    U8 iFdcs;                                               /* DCS indicator for account name. ascii:0, ucs2:length */
    U8 reserved; /*For odd number address verify*/
} GPRSDataAccountNode;

#endif /* __MMI_GPRS_FEATURES__ */ 
#endif /* _COOLSAND_DATA_ACCOUNT_STRUCT_H */ 

