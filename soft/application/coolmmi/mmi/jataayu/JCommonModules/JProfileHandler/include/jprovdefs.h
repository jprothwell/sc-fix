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

/*******************************************************************************
 * Filename:
 * ---------
 * JProvDefs.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file defines the string ,screen & Image ID  used for the OTA provisioning.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * removed!
 * removed!
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
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
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
 * removed!
 * removed!
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
 
 *==============================================================================
 *******************************************************************************/

#ifndef _JPROVDEFS_H
#define _JPROVDEFS_H

#include "mmi_features.h"
#include "mmi_data_types.h"

#if defined (JATAAYU_SUPPORT)
typedef enum
{
    STR_ID_PROV_INSTALL_SETTINGS_1 = JATAAYU_PROVISIONING_BASE + 1,
    STR_ID_PROV_INVALID_BEARER,
    STR_ID_PROV_MAX_QUE_SIZE,
    STR_ID_PROV_INVALID_SETTINGS,
    STR_ID_PROV_NEW_SETTINGS_1,
    STR_ID_PROV_DATAACCOUNT_CAPTION,
    STR_ID_PROV_ENTER_PIN_CAPTION,
    STR_ID_PROV_AUTHENTICATION_PASSED,
    STR_ID_PROV_BOOKMARK_INVALID,
    STR_ID_PROV_BOOKMARK_STORED,
    STR_ID_PROV_INSTALL_BOOKMARK,
    STR_ID_PROV_NEW_BOOKMARK,
    STR_ID_PROV_ABORT_SETTINGS_SETUP,
    STR_ID_PROV_UNSUPPORTED_SETTING
} str_prov_list_enum;

typedef enum
{
    SCR_ID_PROV_PROF_LIST = JATAAYU_PROVISIONING_BASE + 1,
    SCR_ID_PROV_PROF_GPRS_DATA_ACCT,
    SCR_ID_PROV_PROF_GSM_DATA_ACCT,
    SCR_ID_PROV_PROF_NEW_SETTINGS_POPUP,
    SCR_ID_PROV_OMA_ENTER_PIN,
    SCR_ID_PROV_INSTALL_NEW_SETTINGS,
    SCR_ID_PROV_ABORT_SETTINGS_SETUP
} scr_prov_list_enum;

typedef enum
{
    IMG_ID_PROV_CANCEL = JATAAYU_PROVISIONING_BASE + 1,
    IMG_ID_PROV_MAX_QUE_SIZE,
    IMG_ID_PROV_INVALID_BEARER,
    IMG_ID_PROV_INVALID_SETTINGS,
    //  sandeep for unused resource removal
    //  IMG_ID_PROV_NEW_NOTIFICATION_MSG,
    IMG_ID_PROV_AUTHENTICATION_PASSED
} img_prov_list_enum;

#endif /* defined (JATAAYU_SUPPORT) */ // #if defined (JATAAYU_SUPPORT)

#endif /* _JPROVDEFS_H */ 

