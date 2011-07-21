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
 *  Modification Notice:
 *  --------------------------
 *  This software is modified by COOLSAND Inc. and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of COOLSAND Inc. (C) 2001
 *
 *******************************************************************************/

 /*******************************************************************************
 * Filename:
 * ---------
 *   FileManagerSrc.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   File Manager Utility
 *
 * Author:
 * -------
 * -------
 *
 *------------------------------------------------------------------------------
 *
 *******************************************************************************/
#ifndef MMI_ON_HARDWARE_P
#undef __DRM_SUPPORT__
#undef __DRM_V01__
#endif

#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"

#include "globaldefs.h"
#include "unicodexdcl.h"
#include "commonscreens.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "protocolevents.h"
#include "wgui_categories.h"
#include "hardwareevents.h"
#include "debuginitdef.h"
#include "timerevents.h"
#include "callsetupenum.h"				/* str_seconds */

#include "nvramtype.h"					/* nvram data type */
#include "nvramprot.h"					/* nvram access fucntions */
#include "nvramenum.h"					/* nvram id define */

#include "settingprofile.h"
#include "phonesetupgprots.h"
#include "filesystemdef.h"
#include "fat_fs.h"
#include "conversions.h"
#include "lcd_sw_inc.h"

#ifdef __DRM_SUPPORT__
#include "drm_def.h"
#include "drm_gprot.h"
#endif

#include "filemanagerdef.h"
#include "filemanagergprot.h"
#include "filemgr.h"
#include "filemgrstruct.h"
#ifdef __MMI_VIDEO__
	#include "mdi_datatype.h"
	#include "mdi_audio.h"
	#include "mdi_video.h"
#endif /* __MMI_VIDEO__ */

#include "mmi_trace.h"
#undef __NEWSIMULATOR 

/*=============  Constants & Global Variables  ================ */
extern fmgr_filter_struct fmgr_filter[];
extern fmgr_context_struct *fmgr_p;

/*****************************************************************************
* Define
*****************************************************************************/
#define NO_FS_ERROR 55

#define FILE_LIMIT_ERROR_STR_LEN		(100)
#define FILE_LIMIT_ERROR_BUF_SIZE	((FILE_LIMIT_ERROR_STR_LEN+1)*ENCODING_LENGTH)

#define FILE_LIMIT_FALSE_RETURN(_str_id_) \
do{\
	pfnUnicodeStrncpy(g_fmgr_file_limit_error_str,\
							(PS8)(GetString(_str_id_)),\
							FILE_LIMIT_ERROR_STR_LEN);\
	g_fmgr_file_limit_error_str[FILE_LIMIT_ERROR_BUF_SIZE-1] = 0;\
	g_fmgr_file_limit_error_str[FILE_LIMIT_ERROR_BUF_SIZE-2] = 0;\
	return FALSE;\
} while(0);


/*
	size 		: Kbyte
	width 	: pixel
	height 	: pixel
*/
#define IMG_BMP_LIMIT_ITEM(size,width,height)  	GDI_IMAGE_TYPE_BMP_FILE,size,width,height
#define IMG_JPG_LIMIT_ITEM(size,width,height)  	GDI_IMAGE_TYPE_JPG_FILE,size,width,height
#define IMG_GIF_LIMIT_ITEM(size,width,height)  	GDI_IMAGE_TYPE_GIF_FILE,size,width,height
#define IMG_WBMP_LIMIT_ITEM(size,width,height)  GDI_IMAGE_TYPE_WBMP_FILE,size,width,height


/*
	size 		: Kbyte
	duration	: sec
	width 	: pixel
	height 	: pixel
*/
#define VDO_3GP_LIMIT_ITEM(size,duration,width,height)  	GDI_IMAGE_TYPE_3GP_FILE,size,duration,width,height
#define VDO_MP4_LIMIT_ITEM(size,duration,width,height)  	GDI_IMAGE_TYPE_MP4_FILE,size,duration,width,height
#define VDO_AVI_LIMIT_ITEM(size,duration,width,height)  	GDI_IMAGE_TYPE_AVI_FILE,size,duration,width,height

#define END_LIMIT_ITEM		GDI_IMAGE_TYPE_INVALID


/*****************************************************************************
* Typedef
*****************************************************************************/
typedef struct
{
   S32 errorNo;
   UI_string_ID_type stringId;
}
FsErrorEntryStruct;


/*****************************************************************************
* Global Variable
*****************************************************************************/


/*****************************************************************************
* Global Function
*****************************************************************************/


/*****************************************************************************
* Local Variable
*****************************************************************************/
const FsErrorEntryStruct FsErrorTable[]=
{
   {FS_ERROR_RESERVED,					FMGR_FS_ERROR_RESERVED_TEXT					},
   {FS_PARAM_ERROR,						FMGR_FS_PARAM_ERROR_TEXT						},
   {FS_INVALID_FILENAME,				FMGR_FS_INVALID_FILENAME_TEXT					},
   {FS_DRIVE_NOT_FOUND,					FMGR_FS_DRIVE_NOT_FOUND_TEXT 					},
   {FS_TOO_MANY_FILES,					FMGR_FS_TOO_MANY_FILES_TEXT					},
   {FS_NO_MORE_FILES,					FMGR_FS_NO_MORE_FILES_TEXT       			},
   {FS_WRONG_MEDIA,						FMGR_FS_WRONG_MEDIA_TEXT         			},
   {FS_INVALID_FILE_SYSTEM,			FMGR_FS_INVALID_FILE_SYSTEM_TEXT   			},
   {FS_FILE_NOT_FOUND,					FMGR_FS_FILE_NOT_FOUND_TEXT      			},
   {FS_INVALID_FILE_HANDLE,			FMGR_FS_INVALID_FILE_HANDLE_TEXT  			},
   {FS_UNSUPPORTED_DEVICE,				FMGR_FS_UNSUPPORTED_DEVICE_TEXT 				},
   {FS_UNSUPPORTED_DRIVER_FUNCTION,	FMGR_FS_UNSUPPORTED_DRIVER_FUNCTION_TEXT	},
   {FS_CORRUPTED_PARTITION_TABLE,	FMGR_FS_CORRUPTED_PARTITION_TABLE_TEXT		},
   {FS_TOO_MANY_DRIVES,					FMGR_FS_TOO_MANY_DRIVES_TEXT     			},
   {FS_INVALID_FILE_POS,				FMGR_FS_INVALID_FILE_POS_TEXT     			},
   {FS_ACCESS_DENIED,					FMGR_FS_ACCESS_DENIED_TEXT        			},
   {FS_STRING_BUFFER_TOO_SMALL,		FMGR_FS_STRING_BUFFER_TOO_SMALL_TEXT		},
   {FS_GENERAL_FAILURE,					FMGR_FS_GENERAL_FAILURE_TEXT      			},
   {FS_PATH_NOT_FOUND,					FMGR_FS_PATH_NOT_FOUND_TEXT      			},
   {FS_FAT_ALLOC_ERROR,					FMGR_FS_FAT_ALLOC_ERROR_TEXT   				},
   {FS_ROOT_DIR_FULL,					FMGR_FS_ROOT_DIR_FULL_TEXT       			},
   {FS_DISK_FULL,							FMGR_FS_DISK_FULL_TEXT            			},
   {FS_TIMEOUT,							FMGR_FS_TIMEOUT_TEXT               			},
   {FS_BAD_SECTOR,						FMGR_FS_BAD_SECTOR_TEXT           			},
   {FS_DATA_ERROR,						FMGR_FS_DATA_ERROR_TEXT          			},
   {FS_MEDIA_CHANGED,					FMGR_FS_MEDIA_CHANGED_TEXT       			},
   {FS_SECTOR_NOT_FOUND,				FMGR_FS_SECTOR_NOT_FOUND_TEXT   				},
   {FS_ADDRESS_MARK_NOT_FOUND,		FMGR_FS_ADDRESS_MARK_NOT_FOUND_TEXT 		},
   {FS_DRIVE_NOT_READY,					FMGR_FS_DRIVE_NOT_READY_TEXT      			},
   {FS_WRITE_PROTECTION,				FMGR_FS_WRITE_PROTECTION_TEXT      			},
   {FS_DMA_OVERRUN,						FMGR_FS_DMA_OVERRUN_TEXT           			},
   {FS_CRC_ERROR,							FMGR_FS_CRC_ERROR_TEXT							},
   {FS_DEVICE_RESOURCE_ERROR,			FMGR_FS_DEVICE_RESOURCE_ERROR_TEXT			},
   {FS_INVALID_SECTOR_SIZE,			FMGR_FS_INVALID_SECTOR_SIZE_TEXT   			},
   {FS_OUT_OF_BUFFERS,					FMGR_FS_OUT_OF_BUFFERS_TEXT      			},
   {FS_FILE_EXISTS,						FMGR_FS_FILE_EXISTS_TEXT           			},
   {FS_LONG_FILE_POS,					FMGR_FS_LONG_FILE_POS_TEXT        			},
   {FS_FILE_TOO_LARGE,					FMGR_FS_FILE_TOO_LARGE_TEXT       			},
   {FS_BAD_DIR_ENTRY,					FMGR_FS_BAD_DIR_ENTRY_TEXT       	  		},
   //{FS_ATTR_CONFLICT					,FMGR_FS_ATTR_CONFLICT_TEXT					},
   //{FS_CHECKDISK_RETRY				,FMGR_FS_CHECKDISK_RETRY_TEXT					},
   //{FS_LACK_OF_PROTECTION_SPACE	,FMGR_FS_LACK_OF_PROTECTION_SPACE_TEXT		},
   {FS_READ_ONLY_ERROR,					FMGR_FS_READ_ONLY_ERROR_TEXT       	  		},
   {FS_SYSTEM_CRASH,						FMGR_FS_SYSTEM_CRASH_TEXT						},
   {FS_FAIL_GET_MEM,						FMGR_FS_FAIL_GET_MEM_TEXT						},
   {FS_DEVICE_BUSY,						FMGR_FS_DEVICE_BUSY_TEXT						},
   {FS_ABORTED_ERROR,					FMGR_FS_ABORTED_ERROR_TEXT						},
   {FS_FLASH_MOUNT_ERROR,				FMGR_FS_FLASH_MOUNT_ERROR_TEXT				},
   {FS_PATH_OVER_LEN_ERROR,			FMGR_FS_PATH_OVER_LEN_ERROR_TEXT				},
   {FS_APP_QUOTA_FULL, 					FMGR_FS_APP_QUOTA_FULL_TEXT					},
   {FS_MSDC_MOUNT_ERROR,				FMGR_FS_MSDC_MOUNT_ERROR_TEXT					},
   {FS_MSDC_READ_SECTOR_ERROR,		FMGR_FS_MSDC_READ_SECTOR_ERROR_TEXT			},
   {FS_MSDC_WRITE_SECTOR_ERROR,		FMGR_FS_MSDC_WRITE_SECTOR_ERROR_TEXT		},
   {FS_MSDC_DISCARD_SECTOR_ERROR,	FMGR_FS_MSDC_DISCARD_SECTOR_ERROR_TEXT		},
   {FS_MSDC_PRESNET_NOT_READY, 		FMGR_FS_MSDC_PRESENT_NOT_READY_TEXT			},
   {FS_MSDC_NOT_PRESENT, 				FMGR_FS_MSDC_NOT_PRESENT_TEXT					},
   {FS_FLASH_ERASE_BUSY,				FMGR_FS_FLASH_ERASE_BUSY_TEXT					},
   {FS_LOCK_MUTEX_FAIL,					FMGR_FS_LOCK_MUTEX_FAIL_TEXT					},
   {0,0}
};


const S32 g_fmgr_image_viewer_limit_arry[]=
{
	IMG_JPG_LIMIT_ITEM(0,0,0),
	IMG_GIF_LIMIT_ITEM(0,0,0),
	END_LIMIT_ITEM
};

const S32 g_fmgr_wallpaper_limit_arry[]=
{
	IMG_BMP_LIMIT_ITEM(0,UI_DEVICE_WIDTH,UI_DEVICE_HEIGHT),
	IMG_JPG_LIMIT_ITEM(256,0,0),
	IMG_GIF_LIMIT_ITEM(0,UI_DEVICE_WIDTH,UI_DEVICE_HEIGHT),
	END_LIMIT_ITEM
};

#ifdef __MMI_SUB_WALLPAPER__
const S32 g_fmgr_sub_wallpaper_limit_arry[]=
{
	IMG_BMP_LIMIT_ITEM(0,SUBLCD_WIDTH,SUBLCD_HEIGHT),
	IMG_JPG_LIMIT_ITEM(256,0,0),
	IMG_GIF_LIMIT_ITEM(0,SUBLCD_WIDTH,SUBLCD_HEIGHT),
	END_LIMIT_ITEM
};
#endif
const S32 g_fmgr_screensaver_limit_arry[]=
{
	IMG_BMP_LIMIT_ITEM(0,UI_DEVICE_WIDTH,UI_DEVICE_HEIGHT),
	IMG_JPG_LIMIT_ITEM(256,0,0),
	IMG_GIF_LIMIT_ITEM(0,UI_DEVICE_WIDTH,UI_DEVICE_HEIGHT),
	END_LIMIT_ITEM
};

const S32 g_fmgr_power_on_off_disp_limit_arry[]=
{
	IMG_BMP_LIMIT_ITEM(200,UI_DEVICE_WIDTH,UI_DEVICE_HEIGHT),
	IMG_JPG_LIMIT_ITEM(200,0,0),
	IMG_GIF_LIMIT_ITEM(200,UI_DEVICE_WIDTH,UI_DEVICE_HEIGHT),
	END_LIMIT_ITEM
};

const S32 g_fmgr_caller_picture_limit_arry[] =
{
	IMG_BMP_LIMIT_ITEM(20,0,0),
	IMG_JPG_LIMIT_ITEM(20,0,0),
	IMG_GIF_LIMIT_ITEM(20,0,0),
	END_LIMIT_ITEM
};

const S32 g_fmgr_caller_video_limit_arry[] =
{
#ifdef __MMI_MAINLCD_240X320__
	VDO_3GP_LIMIT_ITEM(50*1024,0,240,180),
	VDO_MP4_LIMIT_ITEM(50*1024,0,240,180),
	VDO_AVI_LIMIT_ITEM(50*1024,0,240,180),
#elif defined (__MMI_MAINLCD_128X160__) || defined (__MMI_MAINLCD_128X128__)
	VDO_3GP_LIMIT_ITEM(50*1024,0,128,96),
	VDO_MP4_LIMIT_ITEM(50*1024,0,128,96),
	VDO_AVI_LIMIT_ITEM(50*1024,0,128,96),
#else
	VDO_3GP_LIMIT_ITEM(50*1024,0,0,0),
	VDO_MP4_LIMIT_ITEM(50*1024,0,0,0),
	VDO_AVI_LIMIT_ITEM(50*1024,0,0,0),
#endif
	END_LIMIT_ITEM
};

static S8 g_fmgr_file_limit_error_str[FILE_LIMIT_ERROR_BUF_SIZE];


/*****************************************************************************
* Local Function
*****************************************************************************/






/*****************************************************************************
* FUNCTION
*	GetFileSystemErrorString
* DESCRIPTION
*	map file system return error cause to error string id
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
UI_string_ID_type GetFileSystemErrorString(S32 error)
{
   S16 i;

   for(i=0;i<NO_FS_ERROR;i++)
      if(FsErrorTable[i].errorNo==error)
         return FsErrorTable[i].stringId;
   return (UI_string_ID_type)0;

}


/*****************************************************************************
* FUNCTION
*	mmi_fmgr_set_audio_filter
* DESCRIPTION
*	set the file type belongs to audio group
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_fmgr_set_audio_filter (FMGR_FILTER *file_type)
{
#ifndef TARGET_MIDI_SUPPORT_OFF
	FMGR_FILTER_SET(file_type,FMGR_TYPE_MID); 
#endif
	
	FMGR_FILTER_SET(file_type,FMGR_TYPE_WAV);
	FMGR_FILTER_SET(file_type,FMGR_TYPE_AMR);
#ifdef AAC_DECODE 
	FMGR_FILTER_SET(file_type,FMGR_TYPE_AAC);
#endif
#ifdef DAF_DECODE
	FMGR_FILTER_SET(file_type,FMGR_TYPE_DAF);
#endif

#ifdef VM_DECODE
	FMGR_FILTER_SET(file_type,FMGR_TYPE_VM);
#endif
	FMGR_FILTER_SET(file_type,FMGR_TYPE_IMY);
#ifdef AMRWB_DECODE
	FMGR_FILTER_SET(file_type,FMGR_TYPE_AWB);
#endif
	FMGR_FILTER_SET(file_type,FMGR_TYPE_AIF);
	FMGR_FILTER_SET(file_type,FMGR_TYPE_AIFF);
	FMGR_FILTER_SET(file_type,FMGR_TYPE_AIFC);
	FMGR_FILTER_SET(file_type,FMGR_TYPE_AU);
	FMGR_FILTER_SET(file_type,FMGR_TYPE_SND);
#ifdef MP4_CODEC
	FMGR_FILTER_SET(file_type,FMGR_TYPE_M4A);
#endif
#if defined( EXT_AUDIO_CHIP ) || defined( MMF_DECODE)
	FMGR_FILTER_SET(file_type,FMGR_TYPE_MMF);
#endif
#ifdef WMA_DECODE 
	FMGR_FILTER_SET(file_type,FMGR_TYPE_WMA);
#endif
}

/*****************************************************************************
* FUNCTION
*	mmi_fmgr_check_audio_filter
* DESCRIPTION
*	check if the file type belongs to audio group
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
BOOL mmi_fmgr_check_audio_filter (FMGR_FILTER *file_type)
{
	if  (
#ifndef TARGET_MIDI_SUPPORT_OFF
	FMGR_FILTER_IS_SET(file_type ,FMGR_TYPE_MID) ||	
#endif
	//FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_VM)  || //chenhe for jasper
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_IMY) ||
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_WAV) ||
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_AMR) ||
#ifdef AAC_DECODE 
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_AAC) ||
#endif
#ifdef DAF_DECODE
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_DAF) ||
#endif
#ifdef AMRWB_DECODE
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_AWB) ||
#endif
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_AIF) ||
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_AIFF) ||
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_AIFC)  ||
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_AU) ||
#ifdef MP4_CODEC
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_M4A) ||
#endif
#if defined( EXT_AUDIO_CHIP ) || defined( MMF_DECODE)
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_MMF) ||
#endif
#ifdef WMA_DECODE 
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_WMA) ||
#endif
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_SND) )

		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************
* FUNCTION
*	mmi_fmgr_check_image_filter
* DESCRIPTION
*	check if the file type belongs to image group
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
BOOL mmi_fmgr_check_image_filter(FMGR_FILTER *file_type)
{
	if (FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_BMP) ||
#ifdef JPG_DECODE
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_JPG) ||
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_JPEG) ||
#endif
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_GIF) ||
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_PNG) ||
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_WBMP) ||
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_WBM) ||
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_EMS) ||
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_ANM)||
 #if 1//gdm
	FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_PBM) )
  #endif
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************
* FUNCTION
*	mmi_fmgr_check_video_filter
* DESCRIPTION
*	check if the file type belongs to video group
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
BOOL mmi_fmgr_check_video_filter(FMGR_FILTER *file_type)
{

		if	(FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_3GP) ||
		FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_MP4) || FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_AVI) ||
		FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_RM)|| FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_RMVB))
			return TRUE;
		else
			return FALSE;		

#if 0
	#if defined(ISP_SUPPORT)
		if	(FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_3GP) ||
		FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_MP4))
			return TRUE;
		else
			return FALSE;
	#elif defined(CAMERA_MODULE)
		if	(FMGR_FILTER_IS_SET(file_type,FMGR_TYPE_AVI) )
			return TRUE;
		else
			return FALSE;
	#else
		return FALSE;
	#endif
#endif
}

/*****************************************************************************
* FUNCTION
*	mmi_fmgr_set_video_filter
* DESCRIPTION
*	set filter to all video types
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_fmgr_set_video_filter(FMGR_FILTER *file_type)
{
    mmi_trace(g_sw_FMGR, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	#if 1
		FMGR_FILTER_SET(file_type,FMGR_TYPE_MP4);
		FMGR_FILTER_SET(file_type,FMGR_TYPE_3GP);
		FMGR_FILTER_SET(file_type,FMGR_TYPE_AVI);
		FMGR_FILTER_SET(file_type,FMGR_TYPE_RM);
		FMGR_FILTER_SET(file_type,FMGR_TYPE_RMVB);

	#else   //guojian deleted for jassmine video 20080411
	#if defined(ISP_SUPPORT)
		FMGR_FILTER_SET(file_type,FMGR_TYPE_MP4);
		FMGR_FILTER_SET(file_type,FMGR_TYPE_3GP);
	#elif defined(CAMERA_MODULE)
		FMGR_FILTER_SET(file_type,FMGR_TYPE_AVI);
	#endif
	#endif
}
/*****************************************************************************
* FUNCTION
*	mmi_fmgr_set_audio_filter
* DESCRIPTION
*	set the file type belongs to audio group
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_fmgr_set_mms_attachment_filter (FMGR_FILTER *file_type)
{
	mmi_fmgr_set_video_filter(file_type);
	//the FMGR_TYPE_3G2,FMGR_TYPE_MPEG is just supported by mms ,not by platform
	FMGR_FILTER_SET(file_type, 	FMGR_TYPE_3G2);
	FMGR_FILTER_SET(file_type, 	FMGR_TYPE_MPEG);
	
	mmi_fmgr_set_audio_filter(file_type);
	FMGR_FILTER_SET_IMAGE(file_type);

	
	FMGR_FILTER_SET(file_type, 	FMGR_TYPE_DOC);
	FMGR_FILTER_SET(file_type, 	FMGR_TYPE_XLS);

	FMGR_FILTER_SET(file_type, 	FMGR_TYPE_PPT);
	FMGR_FILTER_SET(file_type, 	FMGR_TYPE_SMIL);
	FMGR_FILTER_SET(file_type, 	FMGR_TYPE_HTM);
	FMGR_FILTER_SET(file_type, 	FMGR_TYPE_TIFF);
	FMGR_FILTER_SET(file_type, 	FMGR_TYPE_QCP);
	FMGR_FILTER_SET(file_type, 	FMGR_TYPE_PDF);
	FMGR_FILTER_SET(file_type, 	FMGR_TYPE_XML);
	FMGR_FILTER_SET(file_type, 	FMGR_TYPE_VCF);
	FMGR_FILTER_SET(file_type, 	FMGR_TYPE_EBOOK_TXT);		
}

/*****************************************************************************
* FUNCTION
*	mmi_fmgr_append_file_name
* DESCRIPTION
*	append file name to file path, i.e. go down one level
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
BOOL mmi_fmgr_append_file_name(PS8 str1, PS8 str2)
{
   if (pfnUnicodeStrlen(str1) + pfnUnicodeStrlen(str2) > FMGR_MAX_PATH_LEN)
   {
   	PRINT_INFORMATION(("append file name too long\n"));

      DisplayPopup((PU8)GetString(FMGR_FS_PATH_OVER_LEN_ERROR_TEXT), IMG_GLOBAL_ERROR,
                     0, 1000, ERROR_TONE);

      return FALSE;
   }

   pfnUnicodeStrcat(str1, str2);
   return TRUE;
}

/*****************************************************************************
* FUNCTION
*	mmi_fmgr_get_last_dir_index
* DESCRIPTION
*	get pointer to the last directory in the file path
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
S16 mmi_fmgr_get_last_dir_index(PS8 path)
{
   	S16 i, length;

   	length = (S16)pfnUnicodeStrlen(path);
   	i = length - 1;

   	while (i>=0)
   	{
      	if (path[i<<1] == '/' && path[(i<<1)+1] == 0 && (i != length - 1))
         		break;
     	 	i--;
   	}

   	return (i+1)<<1;
}

/*****************************************************************************
* FUNCTION
*	mmi_fmgr_remove_last_dir
* DESCRIPTION
*	remove the last directory in the file path, i.e. go up one level
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_fmgr_remove_last_dir (PS8 path)
{
   	S16 index;

   	index = mmi_fmgr_get_last_dir_index (path);
   	/* terminate the string from the point of back slash */

   	if (index == 0)
   	{
   		/* can't delete over drive, always keep X:\ */
		path[6] = 0;
		path[7] = 0;
   	}
   	else
   	{
   		path[index] = 0;
   		path[index+1] = 0;
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_fmgr_convert_curr_file_name
* DESCRIPTION
*	convert file name encoding for display
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
BOOL mmi_fmgr_convert_curr_file_name (PS8 src, PS8 dest, S16 length)
{
   S32 index;

   index = mmi_fmgr_kernel_get_cur_idx();

   if (fmgr_p->file_cache[index].is_short == MMI_TRUE)
   {
      mmi_chset_mixed_text_to_ucs2_str ((U8*)dest, length, (U8*)src, (mmi_chset_enum)PhnsetGetDefEncodingType());
      return MMI_TRUE;
   }
   else
      return MMI_FALSE;
}

/*****************************************************************************
* FUNCTION
*	mmi_fmgr_hide_ext_name
* DESCRIPTION
*	hide extension file name
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_fmgr_hide_ext_name(S8* file_name)
{
	/* find ext name - by finding "." */
#ifndef __MMI_SHOW_FILE_EXT__
	S32	str_len;
	S32	index;

	str_len	= pfnUnicodeStrlen(file_name);
	index		= (str_len-1)*ENCODING_LENGTH;

	while(index >0)
	{
		if( pfnUnicodeStrncmp((PS8)&file_name[index], (PS8)L".", 1) == 0)
		{
			file_name[index]		= '\0';
			file_name[index+1]	= '\0';
			break;
		}

		file_name[index]		= '\0';
		file_name[index+1]	= '\0';
		index-=2;
	}
#endif
}


/*****************************************************************************
* FUNCTION
*	mmi_fmgr_get_default_folder_path
* DESCRIPTION
*	make default folder path
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
BOOL mmi_fmgr_get_default_folder_path(S8 *folder, S8 *path, U16 length, U8 drive)
{
	FS_HANDLE	handle = FS_ERROR_RESERVED;

	if (length < (pfnUnicodeStrlen((S8*)folder) + 4)*ENCODING_LENGTH)
		return FALSE;

	if (path != NULL)
	{
		memset (path, 0, 8);
		path[0] = drive;
		path[2] = ':';
		path[4] = '\\';
		pfnUnicodeStrcat(path, folder);
	}

	if ((handle = MMI_FS_Open((PU8)path, FS_READ_ONLY)) >=0)
		MMI_FS_Close(handle);
	else if (handle == FS_PATH_NOT_FOUND || handle == FS_FILE_NOT_FOUND)
	{
		if (MMI_FS_CreateDir((PU8)path) < 0)
			return FALSE;
	}

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*	mmi_fmgr_create_full_path_folder
* DESCRIPTION
*	create folder recursively
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
S32 mmi_fmgr_create_full_path_folder(S8 *filepath)
{
	S8				buf_createpath[128];
	S8				buf_filepath[(FMGR_MAX_PATH_LEN+1)*ENCODING_LENGTH];
	S32			fs_ret = -1;
	S32			str_len;
	S32			cmp_ret;
	S32			i;
	FS_HANDLE	file_handle = FS_ERROR_RESERVED;


	file_handle = MMI_FS_Open((UINT8 *)filepath, FS_OPEN_DIR | FS_READ_ONLY);

	/* path already exist */
	if(file_handle>=0 || file_handle == FS_FOLDER_HANDLE)
	{
		MMI_FS_Close(file_handle);
		return FS_NO_ERROR;
	}

	/* create directory if it is not there */
	pfnUnicodeStrcpy((PS8)buf_filepath, (PS8)filepath);

	str_len = pfnUnicodeStrlen((PS8)buf_filepath);
	str_len *= ENCODING_LENGTH;

	/* skip drive. "C:\" */
	for(i = 3*ENCODING_LENGTH ; i < str_len; i+=2)
	{
		cmp_ret = pfnUnicodeStrncmp((PS8)&buf_filepath[i], (PS8)L"\\", 1);
		if(cmp_ret == 0)
		{
			/* create dir */

			pfnUnicodeStrncpy((PS8)buf_createpath, (PS8)buf_filepath, i/ENCODING_LENGTH);
			buf_createpath[i]		= '\0';
			buf_createpath[i+1]	= '\0';
			
			fs_ret = MMI_FS_CreateDir((PU8)buf_createpath);	

		}
	}
	return fs_ret;
}

/*****************************************************************************
* FUNCTION
*	mmi_fmgr_extract_file_name
* DESCRIPTION
*	get pointer to the file name in the file path
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
PS8 mmi_fmgr_extract_file_name(S8 *path)
{
   	S16 i, length;

   	length = (S16)pfnUnicodeStrlen(path);
   	i = length - 1;

   	while (i>=0)
   	{
   	//TRACE_EF(g_sw_FMGR, "path[i] = %c",path[i<<1]);
      	if (path[i << 1] == '/' && path[(i << 1) + 1] == 0 &&(i != length - 1))
         		break;
     	 	i--;
   	}

	if (i<0)
		return NULL;

	return &path[(i+1)<<1];
}

/*****************************************************************************
* FUNCTION
*	mmi_fmgr_extract_ext_file_name
* DESCRIPTION
*	get pointer to the extension file name in the file path
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
PS8 mmi_fmgr_extract_ext_file_name(S8 *path)
{
   	S16 i, length;

   	length = (S16)pfnUnicodeStrlen(path);
   	i = length - 1;

   	while (i>=0)
   	{
   		if (path[i<<1] == '\\' && path[(i<<1)+1] == 0)
   			return NULL;
      	if (path[i<<1] == '.' && path[(i<<1)+1] == 0 &&(i != length - 1))
         		break;
     	 	i--;
   	}

	if (i<0)
		return NULL;

	return &path[(i+1)<<1];
}
/*****************************************************************************
* FUNCTION
*	mmi_fmgr_get_file_type
* DESCRIPTION
*	map extension file name to file type
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_fmgr_get_file_type (S8 *path, FMGR_FILTER *file_type)
{
	//UINT8 ext[32] = {0};

	S8		buf_file_ext[(FMGR_MAX_EXT_LEN+1)*ENCODING_LENGTH];
       S8           *ext_ptr = NULL;
	S32	i;
	FS_HANDLE handle = FS_ERROR_RESERVED;
	FS_FileInfo file_info;
		
	memset(buf_file_ext, 0, (FMGR_MAX_EXT_LEN+1)*ENCODING_LENGTH);
	handle = MMI_FS_Open((PU8)path, FS_READ_ONLY);
	
	MMI_FS_GetFileInfo(path, &file_info);
	
	FMGR_FILTER_INIT(file_type);
	FMGR_FILTER_SET(file_type, FMGR_TYPE_UNKNOW);

	/* get file extension - and make it upper case */

       ext_ptr = mmi_fmgr_extract_ext_file_name((PS8)path);
        if ( ext_ptr != NULL )
        {
            UnicodeNToAnsii((PS8)buf_file_ext, ext_ptr, FMGR_MAX_EXT_LEN<<1);
        }
        TRACE_EF(g_sw_FMGR, "mmi_fmgr_get_file_type: buf_file_ext = %s", buf_file_ext);

	for(i = 0 ; i < 3 ; i++)
	{
		if(buf_file_ext[i] >= 'a')
			buf_file_ext[i] += 'A' - 'a';
	}
	TRACE_FMGR_FUNCTION();

	/* use extension to check filter */
	for(i = 0 ; i < FMGR_MAX_FILTER_COUNT ; i++)
	{
		if((buf_file_ext[0] == fmgr_filter[i].ext[0]) &&
		  (buf_file_ext[1] == fmgr_filter[i].ext[1]) &&
		  (buf_file_ext[2] == fmgr_filter[i].ext[2]))
		{
			TRACE_EF(g_sw_FMGR, "I is %d .fmgr_filter[i].ext is %c%c%c %d",i,fmgr_filter[i].ext[0],fmgr_filter[i].ext[1],fmgr_filter[i].ext[2],fmgr_filter[i].type);
			FMGR_FILTER_SET(file_type, fmgr_filter[i].type);
			FMGR_FILTER_CLEAR(file_type, FMGR_TYPE_UNKNOW);
			break;
		}
	}
	
	MMI_FS_Close (handle);
}

/*****************************************************************************
* FUNCTION
*	mmi_fmgr_get_file_group
* DESCRIPTION
*	check file type belongs to which file group
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8 mmi_fmgr_get_file_group (S8 *path)
{
	FMGR_FILTER file_type;

	mmi_fmgr_get_file_type (path, &file_type);
	if (FMGR_FILTER_IS_SET_AUDIO(&file_type))
		return FMGR_GROUP_AUDIO;
	else if (FMGR_FILTER_IS_SET_IMAGE(&file_type))
		return FMGR_GROUP_IMAGE;
	else if (FMGR_FILTER_IS_SET_VIDEO(&file_type))
		return FMGR_GROUP_VIDEO;
	else
		return FMGR_GROUP_UNKNOWN;
}

/*****************************************************************************
* FUNCTION
*	mmi_fmgr_skip_space
* DESCRIPTION
*	skip space in a string
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
PS8 mmi_fmgr_skip_space(S8* line)
{
   S8 *ptr;
   S32 i=0, len;

   ptr = line;

   len = (pfnUnicodeStrlen(line)) << 1;

   while (ptr[i] == ' ' && ptr[i+1] == 0)
   {
      ptr += 2;
      if (i >= len)
         break;
   }
   return ptr;
}

/*WeiD Add Start For 6013 Ver: TBM780  on 2007-8-13 15:34 */
BOOL mmi_fmgr_judge_enter(S8* line)
{
	   S8 *ptr;
	   S32  len;

	   ptr = line;

	   len = (pfnUnicodeStrlen(line)) << 1;

	   while (len>0)
	   {
	   	if((*ptr == 0x0a) && (*(ptr+1) == 0))
	   	    return TRUE;
              ptr += 2;
	      	len -= 2;	       
	   }

	   return FALSE;
}
/*WeiD Add End  For 6013 Ver: TBM780  on 2007-8-13 15:34 */



#ifdef __DRM_SUPPORT__
/*****************************************************************************
* FUNCTION
*	mmi_fmgr_check_drm_right
* DESCRIPTION
*	check drm right for app
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
BOOL mmi_fmgr_check_drm_right(PU16 file_path, U8 permission, BOOL consume)
{
   FS_HANDLE handle;

   handle = DRM_open_file (file_path, FS_READ_ONLY, permission);
   if (handle >= FS_NO_ERROR)
   {
      DRM_RIGHTS_STRUCT *rights;

      rights = DRM_get_rights (handle);
      if (rights != NULL)
      {
         /* DRM file */
         if (DRM_check_rights(rights, permission) == FALSE)
         {
            DRM_close_file (handle);
            return FALSE;
         }
         if (consume)
            DRM_consume_rights(handle, permission);
      }
      DRM_close_file (handle);
      return TRUE;
   }
   return FALSE;
}

#endif /*__DRM_SUPPORT__*/

/******************************************************************************
 * Select drv
 ******************************************************************************/
static U8 fmgr_drv_hilite_id=0;
FmgrGetDrvfp g_fmgr_sel_drv_fp;
#include "profilesenum.h"

void mmi_fmgr_sel_drv_lsk_hdlr(void)
{
#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )

   //U16 selection = GetCurrHiliteID();

   /* User has been select certain dev, Clear exit handler prevent scr add into history */
   EntryNewScreen(SCR_FMGR_SEL_DRV, NULL, NULL, NULL);
   DeleteScreenIfPresent( SCR_FMGR_SEL_DRV );

   if( fmgr_drv_hilite_id /*selection*/ == FMGR_MENU_DRV_CARD )
   {
      if( g_fmgr_sel_drv_fp )
         (*g_fmgr_sel_drv_fp)(MMI_CARD_DRV);
   }
   else
   {
      if( g_fmgr_sel_drv_fp )
         (*g_fmgr_sel_drv_fp)(MMI_PUBLIC_DRV);
   }

#else

  if( g_fmgr_sel_drv_fp )
     (*g_fmgr_sel_drv_fp)((S16)MMI_PUBLIC_DRV);

#endif /* (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__)*/

   //fmgr_p->src_application = 0;
   g_fmgr_sel_drv_fp = NULL;

}

void highlight_fmgr_sel_drv(S32 nIndex)
{
	fmgr_drv_hilite_id = (U8) nIndex;
}

void mmi_fmgr_disp_sel_drv_scr(void)
{
   U8 *nStrItemList[FMGR_MENU_DRV_MAX];
   U8 nNumofItem = (U8)FMGR_MENU_DRV_MAX;
   U8* guiBuffer;

   nStrItemList[FMGR_MENU_DRV_PHONE] = (U8 *)GetString( STR_FMGR_SEL_DRV_PHONE );
   nStrItemList[FMGR_MENU_DRV_CARD] = (U8 *)GetString( STR_FMGR_SEL_DRV_CARD );

   EntryNewScreen(SCR_FMGR_SEL_DRV, NULL, mmi_fmgr_disp_sel_drv_scr, NULL);

   guiBuffer = GetCurrGuiBuffer (SCR_FMGR_SEL_DRV);

	//SetParentHandler(MENU_FMGR_SEL_DEV);

   RegisterHighlightHandler( highlight_fmgr_sel_drv );
   //RegisterHighlightHandler( ExecuteCurrHiliteHandler );

   ShowCategory56Screen (STR_FMGR_SEL_DRV_TITLE, 0,
   						    STR_GLOBAL_OK,IMG_GLOBAL_OK,
   						    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
   					       nNumofItem, nStrItemList, NULL, 0, guiBuffer);

   SetLeftSoftkeyFunction(mmi_fmgr_sel_drv_lsk_hdlr,KEY_EVENT_UP);
   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


BOOL mmi_fmgr_sel_drv_and_enter(U8 app_id, FmgrGetDrvfp callback)
{
   S16 CardDrv=-1;

   //fmgr_p->src_application = app_id;
   g_fmgr_sel_drv_fp = callback;

#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
    CardDrv = MMI_CARD_DRV;
#endif

    if(  CardDrv > 0 ) /* Card Storage is present */
    {
      mmi_fmgr_disp_sel_drv_scr();
      return MMI_TRUE;
    }
    else  /* Card Storage is not present */
    {
      (*callback)((S16) MMI_PUBLIC_DRV );
      //fmgr_p->src_application = 0;
      g_fmgr_sel_drv_fp = NULL;
      return MMI_FALSE;
    }
}



/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_util_load_filepath()
* DESCRIPTION
*   load file path from nvram path pool
* PARAMETERS
*	 path_ptr	IN 	filepath buffer pointer. [NOTIC! size must be FMGR_PATH_BUFFER_SIZE]
*	 id			IN 	storage id
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_fmgr_util_load_filepath(PS8 path_ptr, FMGR_PATH_ID_ENUM id)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S16	error;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	/* arg check */
	MMI_ASSERT(path_ptr != NULL);
	MMI_ASSERT(id < FMGR_PATH_ID_COUNT);

	/* if it is first time loaded form NVRAM, buffer will be all zero */

	/* load first 510 byte */
	ReadRecord(	NVRAM_EF_FMGR_FILEPATH_POOL_HEAD_LID,
					(U16)(id+1),
					(void*)path_ptr,
					NVRAM_EF_FMGR_FILEPATH_POOL_HEAD_SIZE,
					&error);

	/* load tail 10 byte */
	path_ptr += NVRAM_EF_FMGR_FILEPATH_POOL_HEAD_SIZE;

	ReadRecord(	NVRAM_EF_FMGR_FILEPATH_POOL_TAIL_LID,
					(U16)(id+1),
					(void*)path_ptr,
					NVRAM_EF_FMGR_FILEPATH_POOL_TAIL_SIZE,
					&error);

}



/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_util_store_filepath()
* DESCRIPTION
*   store file path to nvram path pool
* PARAMETERS
*	 path_ptr	IN 	filepath buffer pointer. [NOTIC! size must be FMGR_PATH_BUFFER_SIZE]
*	 id			IN 	storage id
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_fmgr_util_store_filepath(PS8 path_ptr, FMGR_PATH_ID_ENUM id)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	S16	error;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	/* arg check */
	MMI_ASSERT(path_ptr != NULL);
	MMI_ASSERT(id < FMGR_PATH_ID_COUNT);

	/* write first 510 byte */
	WriteRecord(NVRAM_EF_FMGR_FILEPATH_POOL_HEAD_LID,
					(U16)(id+1),
					(void*)path_ptr,
					NVRAM_EF_FMGR_FILEPATH_POOL_HEAD_SIZE,
					&error);

	/* write tail 10 byte */
	path_ptr += NVRAM_EF_FMGR_FILEPATH_POOL_HEAD_SIZE;

	WriteRecord(NVRAM_EF_FMGR_FILEPATH_POOL_TAIL_LID,
					(U16)(id+1),
					(void*)path_ptr,
					NVRAM_EF_FMGR_FILEPATH_POOL_TAIL_SIZE,
					&error);

}




/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_util_file_limit_check()
* DESCRIPTION
*   util function for some api to check file's limit
* 	 to gather all api's file decode limit here
* PARAMETERS
*   array_ptr			IN		array pointer
*	 filename	 		IN		filename
*	 error_str_pp		OUT 	error string pointer's ptr
* RETURNS
*   BOOL
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
extern U16 g_vdoply_width;
extern U16 g_vdoply_height;
extern U16 g_vdoply_totaltime;

BOOL mmi_fmgr_util_file_limit_check(const S32 *array_ptr, PS8 filename, PS8 *error_str_pp)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	INT32		file_size;
	S32			image_width;
	S32			image_height;
	S32		 	result;
	U8				res_group_type;
	FS_HANDLE	fs_handle = FS_ERROR_RESERVED;

//	S8				str_buf[50];
//	S8				unicode_buf[100];
	S32			*check_start_ptr;
	U16			resource_type;
	S16			jump;

	S32			check_file_size;
	S32			check_image_width;
	S32			check_image_height;

	#ifdef __MMI_VIDEO__
		mdi_video_info_struct	vdo_info;
		S8			ext_name[20];
		S32		check_duration;
		PS8		ext_ptr;
		S32		i;
	#endif /* __MMI_VIDEO__ */

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	mmi_trace(TRUE,"-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	check_start_ptr = (PS32)array_ptr;

	/* assign buffer */
	*error_str_pp = g_fmgr_file_limit_error_str;
	memset(g_fmgr_file_limit_error_str, 0, FILE_LIMIT_ERROR_BUF_SIZE);


	if(filename == NULL)
	{
		FILE_LIMIT_FALSE_RETURN(FMGR_FS_INVALID_FILENAME_TEXT);
	}

	/* source from file */
	res_group_type = mmi_fmgr_get_file_group(filename);

	if(res_group_type == FMGR_GROUP_IMAGE)
	{
		/* images */
		jump = 4;
		resource_type = gdi_image_get_type_from_file(filename);

	}
	else if(res_group_type == FMGR_GROUP_VIDEO)
	{
			/* video */
		#ifdef __MMI_VIDEO__
			jump = 5;
			ext_ptr = mmi_fmgr_extract_ext_file_name(filename);

			mmi_trace(TRUE,"-----+++++Func: %s;File: %s;Line: %d+++++-----res_group_type == FMGR_GROUP_VIDEO", __FUNCTION__, __FILE__, __LINE__);
			memset(ext_name, 0, sizeof(ext_name));
			UnicodeNToAnsii(ext_name, ext_ptr, 5*ENCODING_LENGTH);

			/* make upper */
			for(i = 0 ; i < 12 ; i++)
			{
				if(ext_name[i] != 0)
				{
					if(ext_name[i] >= 'a')
						ext_name[i] -= 'a' - 'A';
				}
			}

			if(strcmp(ext_name, "3GP") == 0)
			{
				resource_type = GDI_IMAGE_TYPE_3GP_FILE;
			}
			else if(strcmp(ext_name, "MP4") == 0)
			{
				resource_type = GDI_IMAGE_TYPE_MP4_FILE;
			}
			else if(strcmp(ext_name, "AVI") == 0)
			{
				resource_type = GDI_IMAGE_TYPE_AVI_FILE;
			}
			else
			{
				FILE_LIMIT_FALSE_RETURN(STR_FMGR_INVALID_FORMAT);
			}

		#endif /* __MMI_VIDEO__ */

	}
	else
	{
		/* do not have limit */
		return TRUE;
	}

	/* check if has limit */
	while(1)
	{
		if(*check_start_ptr == resource_type)
		{
			/* find limit */
			break;
		}

		check_start_ptr += jump;
		if(*check_start_ptr == END_LIMIT_ITEM)
		{
			/* all checked, no limitation */
			return TRUE;
		}
	};

	if(res_group_type == FMGR_GROUP_IMAGE)
	{
		check_start_ptr++;
		check_file_size 		= *check_start_ptr++;
		check_image_width 	= *check_start_ptr++;
		check_image_height 	= *check_start_ptr;


		/*******************************************/
		/* Image size checking 							 */
		/*******************************************/
		result = gdi_image_get_dimension_file(filename, &image_width, &image_height);
		mmi_trace(g_sw_FMGR, "result %d %d %d", result,image_width,image_height);
		mmi_trace(g_sw_FMGR, "check_image_width=%d check_image_height=%d", check_image_width, check_image_height);
		/*WUZC Add Start For SHOTCUTS Ver:    on 2007-1-25 16:2 */
		result =1;
		/*WUZC Add End  For SHOTCUTS Ver:    on 2007-1-25 16:2 */
		if(result < 0)
		{
			FILE_LIMIT_FALSE_RETURN(STR_FMGR_INVALID_FORMAT);
		}

		/* checking */
		if( ((check_image_width != 0) && (image_width > check_image_width)) ||
			 ((check_image_height != 0) && (image_height > check_image_height)))
		{
			pfnUnicodeStrncpy(g_fmgr_file_limit_error_str,
									(PS8)(GetString(STR_ID_FMGR_FILE_LIMIT_IMAGE_TOO_LARGE)),
									FILE_LIMIT_ERROR_STR_LEN);

			//sprintf(str_buf, " %dx%d", (U16)check_image_width, (U16)check_image_height);
			//AnsiiToUnicodeString(unicode_buf, str_buf);

			//pfnUnicodeStrcat(g_fmgr_file_limit_error_str, unicode_buf);

			/* avoid overflow - force have EOS */
			g_fmgr_file_limit_error_str[FILE_LIMIT_ERROR_BUF_SIZE-1] = 0;
			g_fmgr_file_limit_error_str[FILE_LIMIT_ERROR_BUF_SIZE-2] = 0;

			return FALSE;
		}

		/*******************************************/
		/* Image file size checking (width/height) */
		/*******************************************/
		fs_handle = MMI_FS_Open((U8*)filename, FS_READ_ONLY);

		if(fs_handle < 0)
		{
			FILE_LIMIT_FALSE_RETURN(FMGR_FS_FILE_NOT_FOUND_TEXT);
		}
		else
		{
			MMI_FS_GetFileSize(fs_handle, &file_size);

			if(file_size == 0)
			{
				FILE_LIMIT_FALSE_RETURN(STR_FMGR_INVALID_FORMAT);
			}
			
			MMI_FS_Close(fs_handle);
		}

		mmi_trace(g_sw_FMGR, "file_size and check_file_sze is %d %d %d", file_size,check_file_size,fs_handle);
		if( (check_file_size != 0) && (file_size > (check_file_size*1024) ))
		{

			pfnUnicodeStrncpy(g_fmgr_file_limit_error_str,
									(PS8)(GetString(STR_ID_FMGR_FILE_LIMIT_FILE_TOO_LARGE)),
									FILE_LIMIT_ERROR_STR_LEN);

			//sprintf(str_buf, " %dk", (U16)check_file_size);
			//AnsiiToUnicodeString(unicode_buf, str_buf);

			//pfnUnicodeStrcat(g_fmgr_file_limit_error_str, unicode_buf);

			/* avoid overflow - force have EOS */
			g_fmgr_file_limit_error_str[FILE_LIMIT_ERROR_BUF_SIZE-1] = 0;
			g_fmgr_file_limit_error_str[FILE_LIMIT_ERROR_BUF_SIZE-2] = 0;

			return FALSE;
		}
		TRACE_FMGR_FUNCTION();
		/* all pass */
		return TRUE;



	}
	else if(res_group_type == FMGR_GROUP_VIDEO)
	{
		/* video */
		#ifdef __MMI_VIDEO__

			check_start_ptr++;
			check_file_size 		= *check_start_ptr++;
			check_duration 		= *check_start_ptr++;
			check_image_width 	= *check_start_ptr++;
			check_image_height 	= *check_start_ptr;

			ext_ptr = mmi_fmgr_extract_ext_file_name(filename);

			/*******************************************/
			/* Video file size checking 					 */
			/*******************************************/
			fs_handle = MMI_FS_Open((U8*)filename, FS_READ_ONLY);

			if(fs_handle < 0)
			{
				FILE_LIMIT_FALSE_RETURN(FMGR_FS_FILE_NOT_FOUND_TEXT);
			}
			else
			{
				MMI_FS_GetFileSize(fs_handle, &file_size);

				if(file_size == 0)
				{
					FILE_LIMIT_FALSE_RETURN(STR_FMGR_INVALID_FORMAT);
				}
				
				MMI_FS_Close(fs_handle);
			}

			if( (check_file_size != 0) && (file_size > (check_file_size*1024)) )
			{
				pfnUnicodeStrncpy(g_fmgr_file_limit_error_str,
										(PS8)(GetString(STR_ID_FMGR_FILE_LIMIT_FILE_TOO_LARGE)),
										FILE_LIMIT_ERROR_STR_LEN);

				//sprintf(str_buf, " %dk", (U16)check_file_size);
				//AnsiiToUnicodeString(unicode_buf, str_buf);

				//pfnUnicodeStrcat(g_fmgr_file_limit_error_str, unicode_buf);

				/* avoid overflow - force have EOS */
				g_fmgr_file_limit_error_str[FILE_LIMIT_ERROR_BUF_SIZE-1] = 0;
				g_fmgr_file_limit_error_str[FILE_LIMIT_ERROR_BUF_SIZE-2] = 0;

				return FALSE;
			}
			
			/*******************************************/
			/* Vdieo info checking 	(width/height/dur) */
			/*******************************************/
			//guojian added for jassmine 20080617 B
			
			        /* suspend background audio */
  		      mdi_audio_suspend_background_play();
			
			//guojian added for jassmine 20080617 E

			
			result = mdi_video_ply_open_clip_file(filename, &vdo_info, FALSE);

			if(result < 0)
			{
				FILE_LIMIT_FALSE_RETURN(STR_FMGR_INVALID_FORMAT);
			}
			else
			{
				mdi_video_ply_close_clip_file();
			}

			//guojian added for jassmine 20080617 B
			
		        /* resume background audio */
		        mdi_audio_resume_background_play();
#ifdef NEVER
			COS_Sleep(300);
			vdo_info.height = g_vdoply_height;
			vdo_info.width= g_vdoply_width;
			vdo_info.total_time_duration = g_vdoply_totaltime;			
#endif /* NEVER */
			mmi_trace(1," vdo_info.height=%d,vdo_info.width=%d",vdo_info.height,vdo_info.width);
			//guojian added for jassmine 20080617 E

			if( (check_duration!= 0) && (vdo_info.total_time_duration/1000 > check_duration ) )
			{
				pfnUnicodeStrncpy(g_fmgr_file_limit_error_str,
										(PS8)(GetString(STR_ID_FMGR_FILE_LIMIT_VIDEO_TOO_LONG)),
										FILE_LIMIT_ERROR_STR_LEN);

				//s//printf(str_buf, " %d", (U16)check_duration);
				//AnsiiToUnicodeString(unicode_buf, str_buf);

				//pfnUnicodeStrcat(g_fmgr_file_limit_error_str, unicode_buf);
				//pfnUnicodeStrcat(g_fmgr_file_limit_error_str, (PS8)(GetString(STR_SECONDS)));

				/* avoid overflow - force have EOS */
				g_fmgr_file_limit_error_str[FILE_LIMIT_ERROR_BUF_SIZE-1] = 0;
				g_fmgr_file_limit_error_str[FILE_LIMIT_ERROR_BUF_SIZE-2] = 0;

				return FALSE;
			}

			if( ((check_image_width != 0) && (vdo_info.width > check_image_width)) ||
				 ((check_image_height != 0) && (vdo_info.height > check_image_height)))
			{
				pfnUnicodeStrncpy(g_fmgr_file_limit_error_str,
										(PS8)(GetString(STR_ID_FMGR_FILE_LIMIT_VIDEO_TOO_LARGE)),
										FILE_LIMIT_ERROR_STR_LEN);

				//sprintf(str_buf, " %dx%d", (U16)check_image_width, (U16)check_image_height);
				//AnsiiToUnicodeString(unicode_buf, str_buf);

				//pfnUnicodeStrcat(g_fmgr_file_limit_error_str, unicode_buf);

				/* avoid overflow - force have EOS */
				g_fmgr_file_limit_error_str[FILE_LIMIT_ERROR_BUF_SIZE-1] = 0;
				g_fmgr_file_limit_error_str[FILE_LIMIT_ERROR_BUF_SIZE-2] = 0;

				return FALSE;
			}

			/* pass all */
			return TRUE;

		#else
			FILE_LIMIT_FALSE_RETURN(STR_FMGR_INVALID_FORMAT);
			return FALSE;
		#endif /* __MMI_VIDEO__ */


	}

	return TRUE;

}


/*****************************************************************************
 * FUNCTION
 *  mmi_fmgr_get_file_type_by_filename
 * DESCRIPTION
 *  
 * PARAMETERS
 *  NULL
 * RETURNS
 *  void
 *****************************************************************************/
S32 mmi_fmgr_get_file_type_by_filename(S8* filename)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    PS8 ext_ptr;
    S8 ext_name[20] = {0};
	U8 i;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ext_ptr = mmi_fmgr_extract_ext_file_name(filename);

    memset(ext_name, 0, sizeof(ext_name));

    if (ext_ptr!=NULL)
    {
        UnicodeNToAnsii(ext_name, ext_ptr, 5 * ENCODING_LENGTH);
    }

    /* make upper */
    for (i = 0; i < 12; i++)
    {
        if (ext_name[i] != 0)
        {
            if (ext_name[i] >= 'a')
            {
                ext_name[i] -= 'a' - 'A';
            }
        }
    }

    for (i = 0; i < FMGR_MAX_FILTER_COUNT; i++)
    {
    	if (strcmp(ext_name, fmgr_filter[i].ext) == 0)
    	{
    		return fmgr_filter[i].type;
    	}
    }

	return FMGR_TYPE_UNKNOW;
}
