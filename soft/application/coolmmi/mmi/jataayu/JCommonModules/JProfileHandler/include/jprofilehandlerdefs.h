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
 * JProfileHandlerDefs.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This defines the String ,screen,& Image ID for profile handler module
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef _JPROFILEHANDLERDEFS_H
#define _JPROFILEHANDLERDEFS_H

#include "mmi_features.h"
#include "mmi_data_types.h"

#if defined (JATAAYU_SUPPORT)

typedef enum
{
    STR_ID_PROF_HDLR_SELECT_PROF_CAPTION = JATAAYU_PROVISIONING_BASE + 50,
    STR_ID_PROF_HDLR_ACTIVATE_PROFILE,
    STR_ID_PROF_HDLR_EDIT_PROFILE,
    STR_ID_PROF_HDLR_RENAME_PROFILE,
    STR_ID_PROF_HDLR_HOMEPAGE,
    STR_ID_PROF_HDLR_DATA_ACCOUNT,
    STR_ID_PROF_HDLR_CONN_TYPE,
    STR_ID_PROF_HDLR_USER_NAME,
    STR_ID_PROF_HDLR_PASSWORD,
    STR_ID_PROF_HDLR_SAVED,
    STR_ID_PROF_HDLR_CONN_TYPE_CONNECTION,
    STR_ID_PROF_HDLR_CONN_TYPE_HTTP,
    STR_ID_PROF_HDLR_IP_ADDRESS,
    STR_ID_PROF_HDLR_PROXY_ADDRESS,
    STR_ID_PROF_HDLR_SECURITY,
    STR_ID_PROF_HDLR_PORT,
    STR_ID_PROF_HDLR_OFF,
    STR_ID_PROF_HDLR_ON,
    STR_ID_PROF_HDLR_MMSC_URL,
    STR_ID_PROF_HDLR_PROFILE,
    STR_ID_PROF_HDLR_INVALID_INPUT,
    STR_ID_PROF_HDLR_INVALID_IP,
    STR_ID_PROF_HDLR_DEFAULT_PROFILE_NAME
} str_profile_handler_list_enum;

typedef enum
{
    SCR_ID_PROF_HDLR_PROF_LIST = JATAAYU_PROVISIONING_BASE + 50,
    SCR_ID_PROF_HDLR_SELECTED_PROFILE,
    SCR_ID_PROF_HDLR_EDIT_PROFILE,
    SCR_ID_PROF_HDLR_CONFIRMATION,
    SCR_ID_PROF_HDLR_CONN_TYPE,
    SCR_ID_EDIT_INPUT_METHOD_AND_DONE,
    SCR_ID_PROF_HDLR_EDIT_CONN_TYPE
} scr_profile_handler_list_enum;

typedef enum
{
    IMD_ID_PROF_HDLR_SELECT_PROF_CAPTION = JATAAYU_PROVISIONING_BASE + 50,
    IMG_ID_PROF_HDLR_SET_ACTIVATED_PROFILE,
    IMG_ID_PROF_HDLR_INVALID_INPUT
} img_profile_handler_list_enum;

#endif /* defined (JATAAYU_SUPPORT) */ // #if defined (JATAAYU_SUPPORT)
#endif /* _JPROFILEHANDLERDEFS_H */ 

