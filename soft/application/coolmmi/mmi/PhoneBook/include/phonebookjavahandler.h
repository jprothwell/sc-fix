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

/*******************************************************************************
 * Filename:
 * ---------
 *	PhoneBookHandlerJava.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for providing phonebook interface for java package
 *
 * Author:
 * -------
 *	Wilson Lien (CSD00676)
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/PhoneBook/include/PhoneBookJavaHandler.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 11:02:38   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:43:00   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:28:04   zhangxb
 * Initial revision.
 * 
 *    Rev 1.0   Nov 06 2006 17:33:00   zhangxb
 * Initial revision.
 * 
 *    Rev 1.1   Sep 19 2006 19:11:04   zhoumn
 * No change.
 * 
 *    Rev 1.0   Jul 06 2006 17:43:02   zhangxb
 * Initial revision.
 * 
 *    Rev 1.0   Jul 04 2006 23:03:44   zhangx
 * Initial revision.
 * Revision 1.1.1.1  2006/06/08 12:43:23  liuruijin
 * no message
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef _MMI_PHONEBOOKJAVAHANDLER_H
#define _MMI_PHONEBOOKJAVAHANDLER_H


/* if some header should be include before this one */
//#ifndef _PREINC_H
//#error "preinc.h should be included before header.h"
//#endif

/* 
** Define
*/
#define MMI_PHB_JAVA_SEARCH_LENGTH ((MAX_PB_EMAIL_LENGTH+1)*ENCODING_LENGTH)

/* 
** Typedef 
*/
typedef enum
{
   MMI_PHB_NO_SORT,
   MMI_PHB_BY_NAME,
   MMI_PHB_BY_EMAIL,
   MMI_PHB_BY_NUMBER
} mmi_phb_sort_type_enum;

typedef enum
{
   MMI_PHB_JAVA_ERROR,
   MMI_PHB_JAVA_SUCCESS,
   MMI_PHB_JAVA_NOT_READY,
   MMI_PHB_JAVA_NOT_SUPPORT,
   MMI_PHB_JAVA_NOT_FOUND,
   MMI_PHB_JAVA_STORAGE_FULL,   
   MMI_PHB_JAVA_NUMBER_TOO_LONG,
   MMI_PHB_JAVA_FDN_ON,
   MMI_PHB_JAVA_OUT_OF_INDEX
} mmi_phb_java_error_enum;


typedef struct 
{
   S8	name[(MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH];
   S8	number[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
   S8	homeNumber[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
   S8	officeNumber[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
   S8	faxNumber[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
   S8	emailAddress[(MAX_PB_EMAIL_LENGTH+1)*ENCODING_LENGTH];
} mmi_phb_handler_entry_struct;


typedef struct
{
   LOCAL_PARA_HDR
   U8 sort_type;
   U8 storage;
   U16 index;
   mmi_phb_handler_entry_struct* entry_ptr;
} mmi_phb_handler_set_entry_req_struct;

typedef struct
{
   LOCAL_PARA_HDR
   U8 result;
   U16 index;
} mmi_phb_handler_set_entry_rsp_struct;

typedef struct
{
   LOCAL_PARA_HDR
   U8 sort_type;
   U8 storage;
   U16 index;
   U16 count;
   mmi_phb_handler_entry_struct* entry_ptr;
} mmi_phb_handler_get_entry_req_struct;

typedef struct
{
   LOCAL_PARA_HDR
   U8 result;
   U16 count;
} mmi_phb_handler_get_entry_rsp_struct;

typedef struct
{
   LOCAL_PARA_HDR
   S8 pattern[MMI_PHB_JAVA_SEARCH_LENGTH];
   U8 sort_type;
   U8 storage;
   U8 search_type;
   mmi_phb_handler_entry_struct* entry_ptr;
} mmi_phb_handler_search_entry_req_struct;

typedef struct
{
   LOCAL_PARA_HDR
   U8 result;
   U16 index;
} mmi_phb_handler_search_entry_rsp_struct;

/* 
** Extern Global Variable
*/

/* 
** Extern Global Function
*/
extern void mmi_phb_java_handler_init(void);
extern void mmi_phb_java_write_req(void* info);
extern void mmi_phb_java_write_rsp(U16 result, U16 store_index);
extern void mmi_phb_java_read_req(void* info);
extern void mmi_phb_java_read_rsp(U16 result, U16 count);
extern void mmi_phb_java_search_req(void* info);
extern void mmi_phb_java_search_rsp(U16 result, U16 store_index);
extern void mmi_phb_java_util_get_record_index_by_id(U8 sort_type, U8 storage, U16 index, U16* record_index, U8* record_storage);
extern U16 mmi_phb_java_util_get_index_by_store_index(U8 sort_type, U8 storage, U16 in_store_index);
extern void mmi_phb_java_util_copy_entry_from_phb(U16 store_index, mmi_phb_handler_entry_struct* entry);
extern void mmi_phb_java_util_rebuild_email_sorting(void);
extern void mmi_phb_java_entry_processing_email(void) ;
extern void mmi_phb_java_exit_processing_email(void) ;

#endif /*_MMI_PHONEBOOKJAVAHANDLER_H*/


