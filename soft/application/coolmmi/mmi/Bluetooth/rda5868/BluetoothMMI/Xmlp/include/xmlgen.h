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
 *  xmlgen.h
 *
 * Project:
 * -------- 
 
 *
 * Description:
 * ------------
 *  xml generator extern header file
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
  
 *============================================================================
 ****************************************************************************/
#ifndef XMLGEN_H
#define XMLGEN_H
#ifdef __XML_SUPPORT__

#include "xml_def.h"
#include "xmlgen_common.h"

typedef enum
{
    XG_DOCTYPE_PUBLIC = 0,
    XG_DOCTYPE_SYSTEM = 1
}xg_doctype_mark_enum;

typedef kal_int8 XG_HANDLE;

extern XG_HANDLE xml_new_generator(kal_char* dest, kal_uint32 length);
extern kal_int32 xml_close_generator(XG_HANDLE hd);
extern kal_int32 xml_generate_xmldecl(XG_HANDLE hd, const kal_char *verstr, XML_ENCODING_TYPE_ENUM encode);
extern kal_int32 xml_generate_doctypedecl(XG_HANDLE hd, const kal_char* name, xg_doctype_mark_enum mark, const kal_char* externalid);
extern kal_int32 xml_generate_doctypedecl_ex(XG_HANDLE hd, const kal_char* name, kal_char* dtd, kal_uint32 len_dtd);
extern kal_int32 xml_generate_stag(XG_HANDLE hd, const kal_char* el, xg_attr_struct* attrlist, kal_uint32 num_attr);
extern kal_int32 xml_generate_data(XG_HANDLE hd, kal_char* data, kal_uint32 datalen);
extern kal_int32 xml_generate_cdata(XG_HANDLE hd, kal_char* cdata, kal_uint32 datalen);
extern kal_int32 xml_generate_etag(XG_HANDLE hd, const kal_char* el);
extern kal_int32 xml_generate_inline_element(XG_HANDLE hd, const kal_char* el, kal_char* data, kal_uint32 datalen);
extern kal_int32 xml_generate_empty_element(XG_HANDLE hd, const kal_char* el, xg_attr_struct* attrlist, kal_uint32 num_attr);


#endif  /* __XML_SUPPORT__ */
#endif  /* XMLGEN_H */
