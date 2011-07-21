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
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/Framework/include/ThemeArchiverProt.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:57:42   lin
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

	FILENAME	: ThemeArchiverDef.h

  	PURPOSE		: Theme Archiver

 

 

	DATE		: April 14,05

**************************************************************/
#ifndef _MMI_THEME_ARCHIVER_PROT_H
#define _MMI_THEME_ARCHIVER_PROT_H

#include "mmi_features.h" 
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__

void mmi_init_themearchiver(void);
void da_content_recvd_hdlr(void *inMsg);
void mmi_handle_download_response(void *inMsg);
void mmi_theme_download_complete_notification(void);
void mmi_handle_abort_req(void *inMsg);
void EntryScrSelectStorageScreen(void);
void EntryDownloadInfoScreen(void);

void SelectThmDownloadStorageType(S32 index);
void SetThmDownloadStroageType(void);

void da_handle_non_OMA_content(da_content_received *msg);
void da_handle_descriptor(da_content_received *msg);
void da_parse_descriptor(U8*);

void da_compare_theme_compatibility_withphone(void);
void SetDLTasActivated(void);
void SendGetThemeContentReqToWap(S8 *filepath);
void descriptor_data_handler( void *data, const char *el, const char *value, kal_int32 len);
PS8 mmi_parse_url_extract_file_name(S8 *path);
void GoBackHistoryThemeArchiver(void);
void FreeDynamicMemoryThemeArchiver(void);
void EndKeyHandlerThemeArchiver(void);
void DeleteIntermediateDLTFiles(U16*);
void CheckIfFileAlreadyExistInFM(PS8,PS8);
U16 mmi_check_theme_file_version_compatibility(float32 theme_version);
U16 mmi_check_theme_file_lcd_dimension_compatibiltiy(U32 lcd_width, U32 lcd_height);
U16 mmi_check_theme_type_compatibility(S8* input_type );
void mmi_thmgr_hide_ext_name(S8* file_name);


#endif// #ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__

#endif// #ifndef _MMI_THEME_ARCHIVER_PROT_H


