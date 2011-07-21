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
 *   mmi_sm_enums.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
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


#ifndef _MMI_SM_ENUMS_H
#define _MMI_SM_ENUMS_H

#define MAX_CONFIG_OPTION_LEN   251

#include "custom_nvram_editor_data_item.h"

/* Modified by: CH_Liang 2005.04.26
 * Description: the length of user_name and password are configurable.
 */
#define TCM_MAX(A, B)   (((A) > (B)) ? (A):(B))

/* Define the length of user_name, password and APN for internal PDP context */
#ifdef MAX_GPRS_USER_NAME_LEN
#define INT_MAX_GPRS_USER_NAME_LEN	MAX_GPRS_USER_NAME_LEN
#else
#define INT_MAX_GPRS_USER_NAME_LEN	16
#endif

#ifdef MAX_GPRS_PASSWORD_LEN
#define INT_MAX_GPRS_PASSWORD_LEN	MAX_GPRS_PASSWORD_LEN
#else
#define INT_MAX_GPRS_PASSWORD_LEN	16
#endif

#ifdef MAX_GPRS_APN_LEN
#define INT_MAX_GPRS_APN_LEN			MAX_GPRS_APN_LEN
#else
#define INT_MAX_GPRS_APN_LEN			100
#endif

#define MAX_NUM_CONFIG_OPTION_PACKETS   5
#define LIMIT_CONFIG_OPTION_CONTENT_LEN 128

#define TCM_MAX_GPRS_USER_NAME_LEN		TCM_MAX(INT_MAX_GPRS_USER_NAME_LEN, 16)
#define TCM_MAX_GPRS_PASSWORD_LEN		TCM_MAX(INT_MAX_GPRS_PASSWORD_LEN, 16)
#define TCM_MAX_GPRS_APN_LEN				TCM_MAX(INT_MAX_GPRS_APN_LEN, 100)
#define MAX_CONFIG_OPTION_CONTENT_LEN 	TCM_MAX(TCM_MAX_GPRS_USER_NAME_LEN + TCM_MAX_GPRS_PASSWORD_LEN + 6, 50)

/* Restrict the length of user_name, password and APN */
#if (MAX_CONFIG_OPTION_CONTENT_LEN > LIMIT_CONFIG_OPTION_CONTENT_LEN)
   #error GPRS_USER_NAME_LEN and GPRS_PASSWORD_LEN exceed the maximum
#endif

#if (TCM_MAX_GPRS_APN_LEN > 100)
   #error MAX_GPRS_APN_LEN exceed the maximum
#endif
/* End of Modification, 2005.04.26 Reviewed by: Joseph */

typedef enum
{
    MS_INITIATED = 0,
    NW_INITIATED = 1
}initiated_enum;

typedef enum
{
    NO_TEARDOWN  = 0,
    TEARDOWN_ALL = 1
}teardown_enum;

typedef enum
{
    PRIMARY_CONTEXT,
    SECONDARY_CONTEXT,
    UNKNOWN_CONTEXT
}pdp_context_type_enum;

typedef enum
{
    NO_OP_TFT = 0,
    CREATE_NEW_TFT = 1,
    DELETE_EXISTING_TFT = 2,
    ADD_PKT_FILTER_TO_TFT = 3,
    REPLACE_PKT_FILTER_IN_TFT = 4,
    DELETE_PKT_FILTER_FROM_TFT = 5
} tft_opcode_enum;

#endif


