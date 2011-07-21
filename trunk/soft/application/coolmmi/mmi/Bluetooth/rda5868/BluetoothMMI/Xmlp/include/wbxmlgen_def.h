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
 *  wbxmlgen_def.h
 *
 * Project:
 * -------- 
 
 *
 * Description:
 * ------------
 *  wbxml generator internal header file
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
  
 *============================================================================
 ****************************************************************************/
#ifndef WBXMLGEN_DEF
#define WBXMLGEN_DEF
#ifdef __WBXML_SUPPORT__



#include "wbxmlp.h"
#include "wbxmlgen.h"

#define     WG_MAX_HANDLE_NUM   10
#define     WG_MAX_ELEM_DEPTH   20
#define     WG_BUFFER_SIZE      512

typedef struct
{
    FS_HANDLE file;             /* file to save xml text generated */
    kal_char  *buf;             /* buffer to save xml text generated */
    wbxml_get_token_by_tagname  get_token_by_tagname;  /* Get tagtoken through tagtname */
    wbxml_get_token_by_attrname get_token_by_attrname;  /* Get attrstart and attrvalue token through attribute */   
    wbxml_get_token_by_attrvalue get_token_by_attrvalue;  /* Get attrstart and attrvalue token through attribute */   
    kal_char  *tempbuf;         /* tempbuf is used to load temp string*/
    kal_uint32 buflen;          /* If  application uses buffer to store xml text, length is the length of buffer */
    kal_uint32 curlen;          /* Current length of buffer. it is used to avoid over-buffer, should be less than length*/
    kal_uint8   tagcp;          /* tag space code page */
    kal_uint8   attrcp;         /* attribute space code page */
    kal_uint8   curdepth;       /* Current depth */    
    kal_int8    istagcpchange;   
    kal_int8    isattrcpchange;      
    kal_uint8   occupy;         /* 1: such generator is be occupied;  0: not occupied */  
}wbxml_generator_struct;

extern kal_int32 xml_generate_write_filetobuf (char * dest, FS_HANDLE src, kal_uint32 *len);
extern kal_int32 xml_generate_write_filetofile(FS_HANDLE dest, FS_HANDLE src, kal_uint32 *len);

static kal_int32 wbxml_default_get_tagtoken_hdlr(const kal_char *tagname, kal_uint8 tagcp,  kal_uint8* token);
static kal_int32 wbxml_default_get_attrnametoken_hdlr(const kal_char *attrname, kal_uint8 attrcp, kal_uint8* token, void *reserved);
static kal_int32 wbxml_default_get_attrvaluetoken_hdlr(const kal_char *attrname, kal_uint8 attrcp, kal_uint8* token, kal_uint8 *datatype);
static kal_int32 wbxml_generate_flash_text(kal_int8 hd, char *text, kal_uint32 textlen);
static kal_int32 wbxml_trans_int_to_mb(kal_int32 value, kal_uint8 *pbuf, kal_uint32 buflen);



#endif	/* __WBXML_SUPPORT__ */
#endif  /* WBXMLGEN_DEF */
