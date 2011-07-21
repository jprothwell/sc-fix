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
 *	 FileManagerGProt.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  File Manager exposed functions.
 *
 * Author:
 * -------
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
#ifndef	__FILE_MANAGER_GPROT_H_
#define __FILE_MANAGER_GPROT_H_

#include "wgui.h"
#include "fmt_def.h"

#ifdef MMI_ON_HARDWARE_P
extern kal_wchar CUSTOM_AT_AUDIO_DOWNLOAD_FOLDER_NAME[];
extern kal_wchar CUSTOM_AT_IMAGE_DOWNLOAD_FOLDER_NAME[];
#endif

/*WUZC Modify Start For MMIFS Ver:    on 2007-1-12 16:49 */
extern S8 nPrintableStr[];
/*WUZC Modify End  For MMIFS Ver:    on 2007-1-12 16:49 */
/****************************************************************************
* Define                                                            
*****************************************************************************/
/*
	NOR flash:  Small size (e.g. 512K, 2Mb). 
		    Reliable.
	NAND flash: Large size (e.g 128Mb).
		    Unreliable. (It might read incorrect data from bad blocks without returning error code) 
		    I/O operations might be blocked for a longer period (10~20 seconds) in garbage collection.
		    User applications need to handle FS_BAD_SECTOR on writing a file.
		    It might contain bad blocks in FAT table and need to be low-level formated in worst case.
*/


/************* Drive names on phone ***************/
/* 
 * Note: the following MACRO need not necessarily map to distinct actual drive names.
 * For example, MMI_SYSTEM_DRV might be equal to MMI_PRIVATE_DRV
 */

/*
 * Invisble in USB mode, NOR flash, fast but (possibly) smaller.
 *
 * Typically used to store important system internal data
 */
#define MMI_SYSTEM_DRV  MMI_FS_GetDrive(FS_DRIVE_I_SYSTEM, 1, FS_NO_ALT_DRIVE )

/*
 * Invisible in USB mode
 * NAND flash or NOR flash
 * Large size if it is on NAND flash.
 *
 * Typically used to store data that should be invisible in USB mode. 
 * (e.g. DRM files, thumbnail image cache)
 */
#define MMI_PRIVATE_DRV  MMI_FS_GetDrive(FS_DRIVE_I_SYSTEM, 2, FS_ONLY_ALT_SERIAL )

/*
 * Visible in USB mode if the phone has visible partitions.
 * NAND flash or NOR flash
 * Large size if it is on NAND flash.
 *
 * Typically used to store large data such as Camera pictures, voice memo, ...
 */
#define MMI_PUBLIC_DRV  MMI_FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM)


/***************** Drive names on card ***************/
/* 
 * Memory Card 
 * Visible in USB mode
 * NAND flash
 *
 * MMI_CARD_DRV < 0 if the configuration does not contain any memory card.
 */
#define MMI_CARD_DRV  	MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE)
#define WAP_SYSTEM_DRV  MMI_SYSTEM_DRV	/* drive name for wap */

/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
#define MMI_SEC_CARD_DRV  	MMI_FS_GetDrive(FS_DRIVE_TFCARD_SECOND, 1, FS_NO_ALT_DRIVE)
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
/* default path */
#ifdef MMI_ON_HARDWARE_P
#define FMGR_DEFAULT_FOLDER_IMAGES		CUSTOM_AT_IMAGE_DOWNLOAD_FOLDER_NAME
//#define FMGR_DEFAULT_FOLDER_AUDIO		CUSTOM_AT_AUDIO_DOWNLOAD_FOLDER_NAME
#define FMGR_DEFAULT_FOLDER_AUDIO		"Audio/" //chenhe for jasper
#else
#define FMGR_DEFAULT_FOLDER_IMAGES		L"Images\\"
#define FMGR_DEFAULT_FOLDER_AUDIO		L"Audio\\"
#endif
//chenhe,070622+
#define FMGR_DEFAULT_RECORDER_FOLDER       "Audio/"
//chenhe,070622-

#ifdef __MMI_EBOOK_READER__
#define FMGR_DEFAULT_FOLDER_EBOOKS     "Ebook/"  //guojian added 20080521
#endif 

#define FMGR_DEFAULT_FOLDER_PHOTOS		L"Photos\\"
#define FMGR_DEFAULT_FOLDER_VIDEO		"Video/"
#define FMGR_DEFAULT_FOLDER_RECEIVED	L"Received/" //licheng changed it form L"Received\\"
 
#define FMGR_DEFAULT_FOLDER_THEMES		L"Themes\\"
 
#define FMGR_DEFAULT_FOLDER_PHBIMAGES	"Phbimage"
#define FMGR_DEFAULT_FILE_PHBVIDEO		"phb_video.dat"

/* temp filename */
#define FMGR_TEMP_FILE_IRDA						L"~received.tmp"
#define FMGR_TEMP_FILE_JAVA						L"~java.tmp"
#define FMGR_TEMP_FILE_VCARD						L"~vcard.tmp"
#define FMGR_TEMP_FILE_VCALENDAR					L"~vcalendar.tmp"
#define FMGR_TEMP_FILE_CAMERA						L"~camera.jpg"
#define FMGR_TEMP_FILE_MELODY						L"~rngc.imy"
#define FMGR_TEMP_FILE_EMAIL_NEW					L"~email.tmp"
#define FMGR_TEMP_FILE_EMAIL_SMTP				L"~smtp.tmp"
#define FMGR_TEMP_FILE_VIDEO_AUD					L"~video_aud.tmp"
#define FMGR_TEMP_FILE_VIDEO_AUD_SAMPLE		L"~video_aud_sample.tmp"
#define FMGR_TEMP_FILE_VIDEO_AUD_REAL			L"~video_aud_real.tmp"
#define FMGR_TEMP_FILE_VIDEO_VISUAL				L"~video_aud.tmp"
#define FMGR_TEMP_FILE_VIDEO_VISUAL_SAMPLE	L"~video_aud_sample.tmp"
#define FMGR_TEMP_FILE_VIDEO_VISUAL_REAL		L"~video_aud_real.tmp"


/* file limit */
#define FMGR_LIMIT_IMAGE_VIEWER 			g_fmgr_image_viewer_limit_arry
#define FMGR_LIMIT_SCREEN_SAVER 			g_fmgr_screensaver_limit_arry
#define FMGR_LIMIT_WALLPAPER 				g_fmgr_wallpaper_limit_arry
#ifdef __MMI_SUB_WALLPAPER__
#define FMGR_LIMIT_SUB_WALLPAPER			g_fmgr_sub_wallpaper_limit_arry
#endif
#define FMGR_LIMIT_POWER_ON_OFF_DSPL 	g_fmgr_power_on_off_disp_limit_arry
#define FMGR_LIMIT_CALLER_PICTURE 			g_fmgr_caller_picture_limit_arry
#define FMGR_LIMIT_CALLER_VIDEO 			g_fmgr_caller_video_limit_arry


/****************************************************************************
* Typedef                                                            
*****************************************************************************/
typedef void (*FmgrGetDrvfp)( S16 drv );

typedef enum{
	FMGR_PATH_ID_CAMERA = 0,
	FMGR_PATH_ID_VIDEO,
	FMGR_PATH_ID_PHOTO_EDITOR,

	/* number of id */
	FMGR_PATH_ID_COUNT
} FMGR_PATH_ID_ENUM;
	

/****************************************************************************
* Extern Global Variable
*****************************************************************************/
extern const S32 g_fmgr_image_viewer_limit_arry[];
extern const S32 g_fmgr_wallpaper_limit_arry[];
extern const S32 g_fmgr_sub_wallpaper_limit_arry[];
extern const S32 g_fmgr_screensaver_limit_arry[];
extern const S32 g_fmgr_power_on_off_disp_limit_arry[];
extern const S32 g_fmgr_caller_picture_limit_arry[];
extern const S32 g_fmgr_caller_video_limit_arry[];


/****************************************************************************
* Extern Global Function
*****************************************************************************/
extern UI_string_ID_type GetFileSystemErrorString(S32 error);
extern BOOL mmi_fmgr_append_file_name(PS8 str1, PS8 str2);
extern S16 	mmi_fmgr_get_last_dir_index(PS8 path);
extern void mmi_fmgr_remove_last_dir (PS8 path);
extern void mmi_fmgr_hide_ext_name(S8* file_name);
extern BOOL mmi_fmgr_convert_curr_file_name (PS8 src, PS8 dest, S16 length);
extern BOOL mmi_fmgr_get_default_folder_path(S8 *folder, S8 *path, U16 length, U8 drive);
extern BOOL mmi_fmgr_check_file_present(S8 *path);
extern PS8	mmi_fmgr_extract_file_name(S8 *path);
extern PS8	mmi_fmgr_extract_ext_file_name(S8 *path);
extern S32 mmi_fmgr_create_full_path_folder(S8 *filepath);
extern void mmi_fmgr_get_file_type (S8 *path, FMGR_FILTER *file_type);
extern U8 	mmi_fmgr_get_file_group (S8 *path);
extern PS8	mmi_fmgr_skip_space(S8* line);
extern BOOL mmi_fmgr_check_drm_right(PU16 file_path, U8 permission, BOOL consume);
extern BOOL mmi_fmgr_sel_drv_and_enter(U8 app_id, FmgrGetDrvfp callback);
extern S32 mmi_fmgr_get_file_type_by_filename(S8* filename);


/* fmgr util function */

/* load/store file path */
extern void mmi_fmgr_util_load_filepath(PS8 path_ptr, FMGR_PATH_ID_ENUM id);
extern void mmi_fmgr_util_store_filepath(PS8 path_ptr, FMGR_PATH_ID_ENUM id);

/* file limit checking */
extern BOOL mmi_fmgr_util_file_limit_check(const S32 *array_ptr, PS8 filename, PS8 *error_str_pp);

#endif

