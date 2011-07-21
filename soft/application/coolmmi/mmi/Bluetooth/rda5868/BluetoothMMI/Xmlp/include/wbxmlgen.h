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
 *  wbxmlgen.h
 *
 * Project:
 * -------- 
 
 *
 * Description:
 * ------------
 *  wbxml generator extern header file
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
  
 *============================================================================
 ****************************************************************************/
#ifndef WBXMLGEN_H
#define WBXMLGEN_H
#ifdef __WBXML_SUPPORT__

#include "xmlgen_common.h"

typedef enum{
        WBXML_DATA_TYPE_STRING,
        WBXML_DATA_TYPE_OPAQUE
}wbxml_data_type_enum;

typedef enum{
    WBXML_CHARSET_ISO_8859_1,
    WBXML_CHARSET_ISO_8859_2,
    WBXML_CHARSET_ISO_8859_3,
    WBXML_CHARSET_ISO_8859_4,
    WBXML_CHARSET_ISO_8859_5,
    WBXML_CHARSET_ISO_8859_6,
    WBXML_CHARSET_ISO_8859_7,
    WBXML_CHARSET_ISO_8859_8,
    WBXML_CHARSET_ISO_8859_9,
    WBXML_CHARSET_SHIFT_JIS,
    WBXML_CHARSET_US_ASCII,
    WBXML_CHARSET_UTF_8
}wbxml_charset_type_enum;

typedef kal_int8 WG_HANDLE;

typedef kal_int32 (*wbxml_get_token_by_tagname)(const kal_char *tagname, kal_uint8 tagcp,  kal_uint8* token);
typedef kal_int32 (*wbxml_get_token_by_attrname)(const kal_char *attrname, kal_uint8 attrcp, kal_uint8* token, void *reserved);
typedef kal_int32 (*wbxml_get_token_by_attrvalue)(const kal_char *attrvalue, kal_uint8 attrcp, kal_uint8* token, kal_uint8 *reserved);


extern WG_HANDLE wbxml_new_generator(kal_char* dest, kal_uint32 length);
extern kal_int32 wbxml_close_generator(WG_HANDLE hd);
extern void wbxml_register_get_token_handler(WG_HANDLE  handle, wbxml_get_token_by_tagname get_token_by_tagname, 
        wbxml_get_token_by_attrname get_token_by_attrname, wbxml_get_token_by_attrvalue get_token_by_attrvalue);
extern kal_int32 wbxml_generate_header(WG_HANDLE hd, kal_uint8 mainver, kal_uint8 subver, const kal_char* publicid, wbxml_charset_type_enum charset);
extern kal_int32 wbxml_generate_stag(WG_HANDLE hd, const kal_char* el, xg_attr_struct* attrlist, kal_uint32 num_attr, kal_bool iscontent);
extern kal_int32 wbxml_generate_data(WG_HANDLE hd, kal_char* data, kal_uint32 datalen, wbxml_data_type_enum data_type);
extern kal_int32 wbxml_generate_empty_element(WG_HANDLE hd, const kal_char* el, xg_attr_struct* attrlist, kal_uint32 num_attr);
extern kal_int32 wbxml_generate_inline_element(WG_HANDLE hd, const kal_char* el, kal_char* data, kal_uint32 datalen, kal_uint8 datatype);
extern kal_int32 wbxml_generate_etag(WG_HANDLE hd, const kal_char* el);
#endif  /* __WBXML_SUPPORT__ */
#endif  /* WBXMLGEN_H */
