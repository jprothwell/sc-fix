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
 *	ThemeArchiverDef.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file defines string, screen, and image ids for Theme Archiver application.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/Framework/include/ThemeArchiverMsgStruct.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:57:40   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:38:10   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:24:18   zhangxb
 * Initial revision.
 * 
 *    Rev 1.2   Sep 19 2006 18:59:12   zhoumn
 * No change.
 * 
 *    Rev 1.1   Aug 28 2006 17:19:44   zhangxb
 * 更新了PSI和Simon修改的一些东西.
 * Revision 1.1.1.1  2006/06/08 12:42:59  liuruijin
 * no message
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: ThemeArchiverMsgStruct.h

  	PURPOSE		: Theme Archiver

 

 

	DATE		: April 14,05

**************************************************************/
#ifndef _MMI_THEME_ARCHIVER_MSG_STRUCT_H
#define _MMI_THEME_ARCHIVER_MSG_STRUCT_H


#include "mmi_features.h" 
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__


typedef enum
{
	DA_THEME_DATA=1,
	DA_DESCRIPTOR_DATA,
	NON_DESCRIPTOR_THEME_DATA
} DA_CONTENT_TYPE;

typedef enum
{
	DLT_NO_ERROR,
	DLT_ERROR_CREATE_FILE,
	DLT_ERROR_DOWNLOAD_FILE,
	DLT_ERROR_CONNECTION_ABORT,
	DLT_ERROR_USER_CANCEL
} DLT_ERROR_TYPE;

typedef struct {
	LOCAL_PARA_HDR
	U32 content_type;
	U32 size;
	U32 error_code;
	U16* download_url;
	U16* file_path_name;
}da_content_received;




typedef struct  {
	LOCAL_PARA_HDR
	kal_uint32			session_id;
	kal_wchar*			download_url;
	kal_wchar*			save_file_path;
} da_wap_get_file_req_struct;


#endif//#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
#endif// #ifndef _MMI_THEME_ARCHIVER_MSG_STRUCT_H


