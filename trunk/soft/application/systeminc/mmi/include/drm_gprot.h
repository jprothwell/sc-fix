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
 *	drm_gprot.h
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
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *******************************************************************************/
#ifndef _DRM_GPROT_H
#define _DRM_GPROT_H

#if (defined (__DRM_SUPPORT__) && !defined(MMI_ON_WIN32))

#include "filesystemdef.h"
#include "drm_def.h"

/* interface for applications */
FS_HANDLE DRM_open_file (kal_wchar *file_path, kal_uint32 flags, kal_uint8 permission);
kal_int32 DRM_close_file (FS_HANDLE object);
kal_int32 DRM_read_file (FS_HANDLE source, void *buffer, kal_uint32 size, kal_uint32 *length);
kal_int32 DRM_seek_file (FS_HANDLE source, kal_int32 offset, kal_uint8 ref);
kal_int32 DRM_file_size (FS_HANDLE source, kal_uint32 *size);
kal_int32 DRM_file_info (FS_HANDLE source, FS_FileInfo *file_info);

kal_uint8 DRM_process_file (kal_uint8 *mime_type, kal_wchar *file_path, kal_bool temp_use);
void DRM_consume_rights(FS_HANDLE source, kal_uint8 permission);
kal_uint8 *DRM_get_content_type (FS_HANDLE input);
kal_uint8 DRM_get_object_method (FS_HANDLE input);
DRM_RIGHTS_STRUCT *DRM_get_rights(FS_HANDLE input);
kal_bool DRM_check_rights(DRM_RIGHTS_STRUCT *rights, kal_uint8 permission);
kal_bool DRM_validate_permission (FS_HANDLE source, kal_uint8 permission);

#else

#define DRM_open_file(a,b,c)     	MMI_FS_Open(a,b)
#define DRM_close_file(a)           MMI_FS_Close(a)
#define DRM_read_file(a,b,c,d)      MMI_FS_Read(a,b,c,d)
#define DRM_seek_file(a,b,c)        MMI_FS_Seek(a,b,c)
#define DRM_file_size               MMI_FS_GetFileSize
#define DRM_file_pos(a,b)			MMI_FS_GetFilePosition(a, b)
//#define DRM_file_info               FS_GetFileInfo

#endif

#endif

