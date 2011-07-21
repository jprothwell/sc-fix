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
 *  wbxmlp.h
 *
 * Project:
 * -------- 
 
 *
 * Description:
 * ------------
 *  wbxml parser extern header file
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
  
 *============================================================================
 ****************************************************************************/
#ifndef WBXMLP_H
#define WBXMLP_H
#ifdef __WBXML_SUPPORT__

typedef void (*wbxml_stag_hdlr)(void *resv, const kal_char *tagname, const kal_char **attrlist);
typedef void (*wbxml_end_elem_hdlr)(void *resv, const kal_char  *tagname);
typedef void (*wbxml_header_hdlr)(void *resv, kal_char *publicid, kal_uint32 charset, kal_uint8 mainver, kal_uint8 subver);
typedef void (*wbxml_data_hdlr)(void *resv, const kal_char  *tagname, const kal_char *data, kal_uint32 len, kal_uint8 datatype);
typedef kal_int32 (*wbxml_get_tagname_by_token)(kal_char *ptr, kal_uint32 tagcp,  kal_uint8 token);
typedef kal_int32 (*wbxml_get_attrname_by_token)(kal_char *ptr, kal_char *attrprefix, kal_uint32 attrcp, kal_uint8 token);

typedef void *(*wbxml_malloc_buffer_ptr)(kal_int32 len);
typedef void (*wbxml_free_buffer_ptr)(void *ptr);

typedef enum
{
    WBXML_E_MISMATCH_TAG = -5,
    WBXML_E_UNKNOWN_TAG = -4,
    WBXML_E_UNKNOWN_CHARSET = -3,
    WBXML_E_NO_MEMORY = -2,
    WBXML_RESULT_FAIL = -1,
    WBXML_RESULT_OK = 0
}wbxml_error_enum;

typedef kal_int8 WP_HANDLE; /* WBXML PARSER HANDLE */

#define WBXML_EXT_START_MARK    "$("
#define WBXML_EXT_END_MARK_0    ":escape)"
#define WBXML_EXT_END_MARK_1    ":unesc)"
#define WBXML_EXT_END_MARK_2    ":noesc)"

/* API for parsing */
extern WP_HANDLE wbxml_new_parser(void);
extern void wbxml_close_parser(WP_HANDLE handle);
extern kal_int32 wbxml_parse(WP_HANDLE handle, kal_char* src, kal_int32 length);
extern void wbxml_stop_parser(WP_HANDLE handle);
extern kal_char *wbxml_get_error_tag(WP_HANDLE handle);

/* API for registering callback functions */
extern void wbxml_register_header_handler(WP_HANDLE handle,  wbxml_header_hdlr header_hdlr);
extern void wbxml_register_data_handler(WP_HANDLE handle, wbxml_data_hdlr data_hdlr);
extern void wbxml_register_element_handler(WP_HANDLE handle,
                               wbxml_stag_hdlr stag_hdlr, wbxml_end_elem_hdlr end_hdlr);
extern void wbxml_register_get_name_handler(WP_HANDLE  handle,
                            wbxml_get_tagname_by_token get_tagname_by_token,
                            wbxml_get_attrname_by_token get_attrname_by_token);
extern void wbxml_configure_memory(WP_HANDLE  handle,
            wbxml_malloc_buffer_ptr get_buffer,  wbxml_free_buffer_ptr free_buffer);

/* API for getting extension data*/
extern kal_char *wbxml_get_ext_0(const kal_char* data, kal_uint32 *len);
extern kal_char *wbxml_get_ext_1(const kal_char* data, kal_uint32 *len);
extern kal_char *wbxml_get_ext_2(const kal_char* data, kal_uint32 *len);



#endif  /* __WBXML_SUPPORT__ */
#endif  /* WBXMLP_H */
