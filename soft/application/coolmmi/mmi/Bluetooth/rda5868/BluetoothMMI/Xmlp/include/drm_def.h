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
 * drm_def.h
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
 *==============================================================================
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
#ifndef DRM_DEF_H
#define DRM_DEF_H

#include "drm_errcode.h"

#define DRM_PERMISSION_NONE         0x00
#define DRM_PERMISSION_PLAY         0x01
#define DRM_PERMISSION_DISPLAY      0x02
#define DRM_PERMISSION_EXECUTE      0x04
#define DRM_PERMISSION_PRINT        0x08
#define DRM_PERMISSION_EXPORT       0x10
#define DRM_PERMISSION_ALL          0x80

#ifdef __DRM_SUPPORT__
#include "app_mine.h"
#define DRM_MAX_PATH_LENGTH     261
#define DRM_MAX_KEY_LENGTH      36
#define DRM_MAX_ID_LENGTH       255
#define DRM_MAX_URL_LENGTH      128          
#define DRM_MAX_LINE_LENGTH     128
#define DRM_MAX_FILE_HANDLE     32
#define DRM_MAX_ODKM_SIZE       2048

#define DRM_DEFAULT_WAIT 25 /* seconds */



#define DRM_CONSTRAINT_UNINIT       0x00
#define DRM_CONSTRAINT_COUNT        0x01
#define DRM_CONSTRAINT_DATETIME     0x02
#define DRM_CONSTRAINT_INTERVAL     0x04
#define DRM_CONSTRAINT_NONE         0x08
/* <2.0> */
#define DRM_CONSTRAINT_TIMEDCOUNT   0x10
#define DRM_CONSTRAINT_ACCUMULATED  0X20
#define DRM_CONSTRAINT_INDIVIDUAL   0X40
#define DRM_CONSTRAINT_SYSTEM       0X80

#define DRM_MIMETYPE_MESSAGE            "application/vnd.oma.drm.message"
#define DRM_MIMETYPE_CONTENT            "application/vnd.oma.drm.content"
#define DRM_MIMETYPE_RIGHTS             "application/vnd.oma.drm.rights+xml"
#define DRM_MIMETYPE_RIGHTS_WB          "application/vnd.oma.drm.rights+wbxml"

/* <2.0> */
#define DRM_MIMETYPE_ROAP_PDU           "application/vnd.oma.drm.roap-pdu+xml"
#define DRM_MIMETYPE_ROAP_TRIGGER       "application/vnd.oma.drm.roap-trigger+xml"

typedef enum
{
    DRM_METHOD_NONE = 0,
    DRM_METHOD_FORWARD_LOCK = 1,
    DRM_METHOD_COMBINED_DELIVERY = 2,
    DRM_METHOD_SEPARATE_DELIVERY = 4,
    DRM_METHOD_V2 = 8,
    DRM_METHOD_PDCF = 16
} drm_method_enum;

typedef enum
{
    DRM_PROCESS_TYPE_NORMAL,
    DRM_PROCESS_TYPE_ASYNC,
    DRM_PROCESS_TYPE_VIRTUAL,
    DRM_PROCESS_TYPE_BUFFER,
    DRM_PROCESS_TYPE_INVLID
} drm_process_type;


/* Meta data */
typedef enum
{
    DRM_META_CONTENTURI = 0,        /* 1.0: "ContentURI" field */
    DRM_META_RIGHTS_ISSUER,         /* 1.0: "Rights-Issuer" header */
    DRM_META_CONTENT_LOCATION,      /* 1.0: "Content-Location" header *//* not standard */
    DRM_META_CONTENT_NAME,          /* 1.0: "Content-Name" header */
    DRM_META_CONTENT_DESCRIPTION,   /* 1.0: "Content-Description" header */
    DRM_META_CONTENT_VENDOR,         
    DRM_META_ICON_URI,              /* 1.0: "Icon URI" header */
    DRM_META_CONTENT_URL,           /* 2.0: Content URL */
    DRM_META_CONTENT_VERSION,       /* 2.0: Content Version */
    DRM_META_COPYRIGHT,             /* 2.0: Copyright */
    DRM_META_INFO_URL,              /* 2.0: Info URL */
    DRM_META_PREVIEW_URI,           /* 2.0: Preview URI */
    DRM_META_MAX
}drm_meta_enum;

#define DRM_META_01_TOTAL DRM_META_CONTENT_URL

#define DRM_PATH L"Z:\\@DRM\\"

#define DRM_VERNO_REC L"z:\\@DRM\\verno.txt"

#define DRM_MMS_PATH L"Z:\\@MMS_DRM\\"

#define DRM_DEFAULT_APP_PATH "z:\\@DRM\\"
#define DRM_DEFAULT_RP_PATH DRM_DEFAULT_APP_PATH"rp\\"
#define DRM_DEFAULT_CER_PATH DRM_DEFAULT_APP_PATH"bscidrm2\\"
#define DRM_DEFAULT_TEMP_PATH DRM_DEFAULT_APP_PATH"tmp\\"

#define DRM_MAX_XML_SIZE (3*1024)

typedef enum
{
    DRM_STATUS_NO_RIGHTS,
    DRM_STATUS_RIGHTS_PRESENT,
    DRM_STATUS_RIGHTS_IN_FUTURE,
    DRM_STATUS_RIGHTS_PENDING,
    DRM_STATUS_UNTRUSTED_TIME,
    DRM_STATUS_CONTENT_PENDING,
    DRM_STATUS_RIGHTS_TRIAL_ONLY,
    DRM_STATUS_NOT_PROTECTED
} drm_rights_status_enum;

typedef struct
{
    kal_int32 id_len;
    kal_char id[DRM_MAX_ID_LENGTH];
}drm_cid_struct;

#define DRM_MSG_BINARY_ENCODING (0)
#define DRM_MSG_BASE64_ENCODING (1)

typedef struct
{
    drm_cid_struct cid;
    kal_int32 plaintext_offset;
    kal_int32 plaintext_len;
    applib_mime_type_struct *mime_type;
    kal_uint8 encoding;
} drm_message_info_struct;

typedef struct
{
    kal_int32 status;
    kal_int32 used_count;
    kal_int32 total_count;
    kal_int32 start_time;
    kal_int32 end_time;
    kal_int32 interval;
    kal_int32 start_intv;
    kal_int32 end_intv;
#ifdef __DRM_V02__
    kal_int32 timed_count;
    kal_int32 total_timed_count;
    kal_uint32 period;
    kal_int32 accum_dur;
    kal_int32 total_accum_dur;
    kal_int32 is_individual;
    kal_uint32 sys_num;
#endif /* __DRM_V02__ */ 
    kal_uint8 type;
} drm_constraint_struct;

typedef void (*drm_general_callback) (kal_uint8 serial, kal_int32 result);

typedef void (*drm_install_object_callback)(kal_uint8 serial,
                 kal_int32 result,
                 applib_mime_type_struct *mime_type);

typedef void (*drm_parse_dm_callback)(kal_uint8 serial,
                     kal_int32 result,
                     drm_message_info_struct *info);

typedef void (*drm_ro_expiry_callback) (kal_int32 result, kal_int32 id);


#endif /* __DRM_SUPPORT__ */ 
#endif /* DRM_DEF_H */ 

