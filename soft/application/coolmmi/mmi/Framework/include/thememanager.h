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
 *	ThemeManager.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This is the header file for Theme Manager
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef _THEME_MANAGER_H
#define _THEME_MANAGER_H

#include "custthemesres.h"
#include "thememanager.h"

/***************************************************************************** 
* Define
*****************************************************************************/
#define		THEME_IMAGE_INFO_HEADER		(8)
#define		THEME_IMAGE_OFFSET			(4)
#define		THEME_FILE_HANDLE			(4)

#define		MAX_MMI_IMAGEID_IMAGES		(1393) //temp

#define		MAX_SYSTEM_THEMES				(3)
#define		MAX_DOWNLOADED_THEMES			(20)
#define		TOTAL_THEMES_COUNT				(MAX_SYSTEM_THEMES+MAX_DOWNLOADED_THEMES)
#define		MAX_THEME_NAME_LENGTH			(21)
#define		MAX_THEME_IMAGE_HEADER_LENGTH	(THEME_IMAGE_INFO_HEADER+THEME_IMAGE_OFFSET+THEME_FILE_HANDLE)
#define		MAX_THEME_IMAGEID_IMAGES		(150)
#define		MAX_THEME_NON_IMAGEID_IMAGES	(50)
#define		MAX_DWNL_THM_PATH_LEN			(30)
#define		MAX_THEME_SEARCH_STR_LEN		(10)

#define		DOWNLOADED_THEMES_PATH				":\\Themes\\"
#define		THEME_SEARCH_STR					"*.med"
#define		THEME_AUDIO_FILE				"ThemeAud.wav"
#define		THEME_AUDIO_MID_FILE				"ThemeAud.mid"
#define		THEME_AUDIO_FILE_PATH			"c:\\Audio\\"
#define		THEME_CHECKSUM_HEADER_SIZE			(4)

#define THEME_AUDIO  0   //ritesh
#define THEME_MIDI  1  //ritesh

#define THEME_ID	U32 	
#define MAX_SUPPORTED_VERSION_NUM	1




/***************************************************************************** 
* Typedef 
*****************************************************************************/
typedef enum 
{
	THEME_TYPE_SYSTEM = 1,
	THEME_TYPE_DOWNLOADED_PHONE,
	THEME_TYPE_DOWNLOADED_CARD
} theme_type_enum;

typedef enum
{
	THEME_ERROR_SUCCESS,
	THEME_ERROR_SPACE_NOT_AVAILABLE,
	THEME_ERROR_THEME_NOT_AVAILABLE,
	THEME_ERROR_FILE_FORMAT_BAD,
	THEME_ERROR_DELETE_FAILURE,
	THEME_ERROR_FAILURE,
	THEME_ERROR_ALREADY_ACTIVATED,
	THEME_ERROR_MAX_COUNT_REACHED,
	THEME_ERROR_INCOMING_CALL,
	THEME_ERROR_PROCESS_QUEUE_SUCESS,
	THEME_ERROR_VERSION_FAILURE
} theme_error_enum;

typedef enum
{
	CHECKSUM_NOT_VERIFIED,
	CHECKSUM_VERIFIED_OK,
	CHECKSUM_VERIFIED_BAD
} theme_checksum_enum;

typedef struct 
{
	U32		theme_id;
	U16		theme_checksum;
	U8		theme_checksum_verified;
	U8		theme_name[MAX_THEME_NAME_LENGTH*ENCODING_LENGTH];
	U8		theme_type;
} theme_list_struct;
/*
typedef struct 
{
	U16	image_id;
	U32 image_offset;
} theme_image_detail_struct;

typedef struct 
{
	MMI_theme*					theme;
	theme_image_detail_struct* theme_image_details_list;
	U8**						system_image_list;
} theme_details_struct;*/

typedef struct 
{
	U8*	mempool;
	U32	current_offset;
	U32	total_pool_size;
	U16	unit_size;
} theme_image_header_struct;

typedef struct 
{
	S16	nOffset;
}theme_image_cache_struct;

typedef struct 
{
	U8 imageid_images_headers_pool[MAX_THEME_IMAGE_HEADER_LENGTH*MAX_THEME_IMAGEID_IMAGES];
	U8 non_imageid_images_headers_pool[MAX_THEME_IMAGE_HEADER_LENGTH*MAX_THEME_NON_IMAGEID_IMAGES];
	U8  theme_imageid_image_count;
	U8	total_thm_cnt;
	U8  sys_thm_cnt;
	U32 curr_activated_theme_id;
	U8 curr_activated_theme_type;
	U8	theme_has_wallpaper;
	U8  theme_has_screensaver;
	U8  theme_has_ringtone;
	U8	theme_has_alternate_image;
	S32						theme_file_handle;
	theme_image_header_struct imageid_images_headers;
	theme_image_header_struct non_imageid_images_headers;
	theme_image_detail_struct theme_image_list[MAX_THEME_IMAGEID_IMAGES];
	theme_image_cache_struct image_cache[MAX_THEME_IMAGEID_IMAGES];
} mmi_theme_manager_context_struct;

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/

void	mmi_theme_manager_init(void); /*Theme Manager intialization*/

void	tm_create_image_header_pool(theme_image_header_struct* ds, void* mempool, U16 unit_size, U32 total_pool_size);
void*	tm_add_image_header_to_pool(theme_image_header_struct* ds, void* buff);
void	tm_reset_image_header_pool(theme_image_header_struct* ds);
void*	tm_get_downloaded_theme_image(U32 image_offset, S32);  
void*	tm_get_system_theme_image(U32 image_index);  
void	tm_reset_imageid_image_header_pool(void);
void	tm_reset_non_imageid_image_header_pool(void);
void	tm_set_imageid(U16 image_id, U32 image_offset);	
void	tm_handle_audio(U32 audio_offset, U32 audio_size);
void	tm_sort_theme_image_table(void);
void	tm_image_table_quick_sort(U16, U16);
void	tm_image_table_insertion_sort(U16, U16);
void	tm_image_table_swap_node(U16, U16);
U8		tm_get_themes(theme_list_struct** theme_list);
U8		tm_get_downloaded_themes(theme_type_enum theme_type);
theme_error_enum tm_verify_theme_checksum(U8* file_name); //temp
U32		tm_get_current_theme_id(void);		
U8		tm_does_theme_have_alt_image(void);
U8		tm_is_theme_dirty_bit_set(U16 image_index);
U32		tm_get_theme_image_offset_or_index(U16 image_id, S32* nArrayIndex);
U8		tm_get_curr_theme_type(void);
U8		tm_get_total_themes_count(void);
void	tm_activate_theme_at_bootup(void);
pBOOL	tm_get_current_theme_wallapaper(void);
pBOOL	tm_get_current_theme_screensaver(void);
pBOOL	tm_get_current_theme_ringtone(void);
const float32* tm_get_theme_version_table(void);
#ifdef __DLT_ENABLE_PROCESS_QUEUE__
theme_error_enum theme_process_queue(void);
#endif

UI_image_type tm_create_non_imageid_image(U32 image_offset);
theme_error_enum	tm_activate_theme(THEME_ID theme_id);
theme_error_enum	tm_activate_system_theme(theme_list_struct* theme_struct);
theme_error_enum	tm_activate_downloaded_theme(theme_list_struct* theme_struct);
theme_error_enum	tm_activate_theme_from_fmgr(theme_type_enum theme_type, U8* theme_name);
theme_error_enum	tm_delete_theme(THEME_ID theme_id, PS32 error_type);
theme_error_enum	tm_delete_theme_from_fmgr(theme_type_enum theme_type, U8* theme_name, PS32 error_type);

BOOL theme_version_and_deviceinfo_check(theme_list_struct* theme_struct);
void HandleThemeActivationErrorStatus(U16 status);


#endif
