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
 * JProvWap.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This is common file for OMA & NE OTA provisioning settings.
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

#ifndef _JPROVWAP_H
#define _JPROVWAP_H

#include "mmi_features.h"
#if defined (JATAAYU_SUPPORT)
#if 0
#include <ddl.h>
#include <jcal.h>
#include <jdi_xml.h>
#include <jdi_wbxml.h>
#include <xmltag.h>
#endif
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"

#include "custom_nvram_editor_data_item.h"
#include "ucs2prot.h"
#include "keybrd.h"
#include "idleappprot.h"
#include "dataaccountcommon.h"
#include "commonscreens.h"
#include "wgui_categories_popup.h"
#include "globaldefs.h"
#include "protocolevents.h"
//#include "settingprofile.h"
#include "wap_ps_struct.h"
//#include "jprovdefs.h"
//#include "jprovprots.h"

#include "l4a.h"

#include "dataaccountdef.h"

#define TEXT_VND_WAP_CONNECTIVITY_XML           "text/vnd.wap.connectivity-xml" /* Presently not supported */
#define APPLICATION_VND_WAP_CONNECTIVITY_WBXML     "application/vnd.wap.connectivity-wbxml"

/* Defines the content type used for Nokia\Ericsson provisioning. */
// #define TEXT_X_WAP_PROV_BROWSER_SETTINGS                     "text/x-wap-prov.browser-settings"/*Presently not supported*/
#define APPLICATION_X_WAP_PROV_BROWSER_SETTINGS    "application/x-wap-prov.browser-settings"

 
#define APPLICATION_X_WAP_PROV_BROWSER_BOOKMARKS   "application/x-wap-prov.browser-bookmarks"

#define WSP_PUSH_HEADER             (0x06)

#define PARM      "parm"
#define NAME      "name"
#define VALUE     "value"
#define TYPE      "type"
#define PAP       "PAP"
#define CHAP      "CHAP"

#define PROV_MAX_NBR_DOCUMENTS            (5)
#define PROV_MAX_NBR_BRS_PROFILE       (NVRAM_WAP_PROFILE_MAX)
#define PROV_MAX_NBR_MMS_PROFILE       (NVRAM_MMS_PROFILE_MAX

#define PROV_MAX_QUE_SIZE_TIMEOUT         (1000)
#define PROV_INV_BEARER_TIMEOUT           (1000)
#define PROV_INVALID_SETTINGS_TIMEOUT     (1000)
#define PROV_DATA_ACCOUNT_NOTIFY_TIMEOUT  (1000)
#define PROV_AUTHENTICATION_DONE_TIMEOUT  (1000)

#define PROV_PROFILE_PROFILE_NAME_LEN     (NVRAM_WAP_PROFILE_PROFILE_NAME_LEN)
#define PROV_PROFILE_HOMEPAGE_URL_LEN     (NVRAM_WAP_PROFILE_HOMEPAGE_URL_LEN)
#define PROV_PROFILE_MMSC_URL_LEN         (NVRAM_MMS_PROFILE_MMSC_URL_LEN)
#define PROV_PROFILE_USERNAME_LEN         (NVRAM_WAP_PROFILE_USERNAME_LEN)
#define PROV_PROFILE_PASSWORD_LEN         (NVRAM_WAP_PROFILE_PASSWORD_LEN)

#define PROV_MAX_GPRS_DATAACCOUNT_NAME    (MAX_GPRS_DATAACCOUNT_NAME)
#define PROV_MAX_GPRS_USERPASSWD       (MAX_GPRS_USERPASSWD)
#define PROV_MAX_GPRS_USERNAME            (MAX_GPRS_USERNAME)
#define PROV_MAX_GPRS_MMI_APN_LEN         (MAX_GPRS_MMI_APN_LEN)

#define PROV_MAX_DATA_ACCOUNT_NAME_LEN    (MAX_DATA_ACCOUNT_NAME_LEN)
#define PROV_MAX_USER_LEN              (MAX_USER_LEN)
#define PROV_MAX_PASSWD_LEN               (MAX_PASSWD_LEN)
#define PROV_MAX_DIAL_NUM_LEN          (MAX_DIAL_NUM_LEN - 1)   /* As the screen allow only 20 chars to input */

#define PROV_BKM_TITLE_LEN             (NVRAM_BRW_BKM_MAX_TITLE_LENGTH)
#define PROV_BKM_URL_LEN               (NVRAM_BRW_BKM_MAX_URL_LENGTH)

#define PROV_TAG_NAME_MAX_LENGHT       (30)

#define PROV_MIN_SIZE_REQ              (64)

#define  PROV_BRS_SETTINGS             (0x01)
#define  PROV_MMS_SETTINGS             (0x02)
#define  PROV_BOOKMARK_SETTINGS           (0x04)
#define  PROV_NOKIA_SETTINGS              (0x08)
#define  PROV_OMA_SETTINGS             (0x10)

#define PROV_DEFAULT_HTTP_PORT            (8080)

#define PROV_IP_PART_MAX_VALUE            (255)

typedef S8 PROV_RETCODE;

typedef enum
{
    PROV_OK,
    PROV_BKM_FULL,
    PROV_BKM_INVALID,
    PROV_BKM_OK,
    PROV_BRW_MMS_SETTING,
    PROV_MEM_FULL,
    PROV_ERR,
    PROV_ERR_UNSUPPORTED_SETTING
} prov_ret_code_enum;

typedef enum
{
    PROV_GPRS_BEARER,
    PROV_CSD_BEARER,
    PROV_INVALID_BEARER
} prov_bearer_enum;

 
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* 0 */ 
typedef enum
{
    PROV_AUTHTYPE_PAP,
    PROV_AUTHTYPE_CHAP
} prov_nap_authtype_enum;

typedef enum
{
    PROV_CONN_TYPE_HTTP = 1,
    PROV_CONN_TYPE_CO,
    PROV_CONN_TYPE_CO_SEC
} prov_conn_type_enum;

typedef struct
{
    S8 *pbuffer;
    U32 plength;
    U16 instal_stringID;
} prov_queue_data_struct;


/*****************************************************************************/

extern S8 g_prov_data_queue_size;
extern U8 g_prov_bearer;
extern U8 g_prov_setting_type;
extern U8 g_prov_appl_flag;
extern U8 *g_prov_profile_name_p;
extern FuncPtr g_prov_timeout_hdlr;
extern U16 g_prov_newsetting_stringID;
extern mmi_cc_set_csd_profile_req_struct *g_prov_set_csd_req_p;

#ifdef __GPRS_MODE__
extern mmi_ps_set_gprs_data_account_req_struct *g_prov_set_gprs_req_p;
#endif 

extern pBOOL isInCall(void);

extern U8 g_prov_pend_set;

#endif /* defined (JATAAYU_SUPPORT) */ // #if defined (JATAAYU_SUPPORT)
#endif /* _JPROVWAP_H */ 

