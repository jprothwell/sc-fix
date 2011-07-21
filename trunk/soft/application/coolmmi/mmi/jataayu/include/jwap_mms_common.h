 /***************************************************************************
 *
 * File Name : jwap_mms_common.h
 *
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software Ltd., Bangalore ("Jataayu") 
 * or Jataayu's suppliers as the case may be. The Software is protected by 
 * copyright, including without limitation by applicable copyright laws, 
 * international treaty provisions, other intellectual property laws and 
 * applicable laws in the country in which the Software is being used. 
 * You shall not modify, adapt or translate the Software, without prior 
 * express written consent from Jataayu. You shall not reverse engineer, 
 * decompile, disassemble or otherwise alter the Software, except and 
 * only to the extent that such activity is expressly permitted by 
 * applicable law notwithstanding this limitation. Unauthorized reproduction 
 * or redistribution of this program or any portion of it may result in severe 
 * civil and criminal penalties and will be prosecuted to the maximum extent 
 * possible under the law. Jataayu reserves all rights not expressly granted. 
 * 
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND 
 * AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, AND/OR FITNESS 
 * FOR A PARTICULAR PURPOSE OR USE, TITLE OR INFRINGEMENT ARE EXPRESSLY 
 * DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. YOU ASSUME THE 
 * ENTIRE RISK AS TO THE ACCURACY AND THE USE OF THIS SOFTWARE. JATAAYU 
 * SHALL NOT BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT, 
 * EXEMPLARY, SPECIAL OR PUNITIVE DAMAGES INCLUDING WITHOUT LIMITATION 
 * ANY LOSS OF DATA, OR; LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL 
 * OR OTHER SIMILAR LOSS RESULTING FROM OR OUT OF THE USE OR INABILITY 
 * TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 *
 ***************************************************************************/
 
 /** 
 * @file		jwap_mms_common.h
 * @ingroup		jCommonModules
 * @brief		
 *
 **/ 
#ifndef __JWAP_MMS_COMMON
#define __JWAP_MMS_COMMON

 /***************************************************************************
 * System Include Files
 **************************************************************************/

 /***************************************************************************
 * User Include Files
 **************************************************************************/

/***************************************************************************
* Macros Definitions
**************************************************************************/


#define __JATAAYU_APP__ //?

#ifdef __JATAAYU_APP__

/***************************************************************************
 * Global variables declarations
 **************************************************************************/
/*	Configuration related to DE */
#define	CFG_DE_CONFIG_PATH			"/_wap_mms/wap_tempfiles"	
#define CFG_PATH_SEPARATOR				"/"

/*	Configuration related to MMS */
/*	This is the identifier for the filesystem used by mms (i.e, .\\) */
//#define	CS_FS_MMS_IDENTIFIER		"vds1"
//#define	CS_FS_MMS_IDENTIFIER		"MMC0"
#define	CS_FS_MMS_IDENTIFIER		"WAP_MMS"
#define WAPMMS_REGION_PATH_NAME "/_wap_mms"

/**
 * @brief	Specifies the path in which core can create its temporary files for 
 *			MMS Application
 **/
#define	CFG_MMS_CONFIG_PATH			"/_wap_mms/mms_tempfiles"	
#define	MMS_TEMP_FILE_PATH			"/_wap_mms/mms_tempfiles/"

#define	CFG_MMS_ROOT_FODER_PATH	"/_wap_mms/jmms/"
#define JMMS_SETTINGS_FILE				"/_wap_mms/jmms/jmms_settings.dat"
/*  This is the wap downlaod folder configuration */
#define	CS_WAP_DOWLOAD_PATH	"/t/wap_download"
#define	CS_WAP_DOWLOAD_TEMP_PATH	"/t/wap_download/"

#define	CS_WAP_SAVED_PAGE_PATH						"/t/wap_saved_page"
#define	CS_WAP_SAVED_PAGE_PATH_WITH_END_SLASH	"/t/wap_saved_page/"
/*	Configuration related to jB5 Core */
#define CS_WAP_PERSISTENT_PATH			"/_wap_mms/wap_persistent"
#define CS_WAP_BOOKMARK_FILE_NAME		"/_wap_mms/wap_persistent/jBookMark.dat"
#define CS_WAP_HISTORY_FILE_NAME		"/_wap_mms/wap_persistent/jHistory.dat"
#define CS_WAP_SETTINGS_FILE_NAME		"/_wap_mms/wap_persistent/jWapSettings.dat"
#define CS_WAP_CACHE_FILE_NAME			"/_wap_mms/wap_persistent/jCache.dat"
#define CS_WAP_COOKIE_FILE_NAME			"/_wap_mms/wap_persistent/jCookie.dat"
#define	CS_WAP_PUSH_FILE_NAME			"/_wap_mms/wap_persistent/jPush.dat"

#define BOOKMARK_COUNT					10
#define	JWAP_MAX_URL_LEN				255
#define CS_WAP_CACHE_SIZE_MAX			(100 * 1024)
#define CS_WAP_COOKIE_DEFAULT_MAX		25
#define CS_WAP_COOKIE_MAX_SIZE			1024
#define CS_WAP_HISTORY_MAX_DOMAIN		0	
#define CS_WAP_HISTORY_MAX_HOST			10
#define	JWAP_MAX_PUSH_MSGS				20


#define CS_MMS_CONTENT_TYPE							"application/vnd.wap.mms-message"
#define CS_WAP_SI_CONTENT_TYPE						"text/vnd.wap.si"
#define CS_WAP_SIC_CONTENT_TYPE						"application/vnd.wap.sic"
#define CS_WAP_SL_CONTENT_TYPE						"text/vnd.wap.sl"
#define CS_WAP_SLC_CONTENT_TYPE						"application/vnd.wap.slc"

#ifdef JOTA_PROVISION_APP_ENABLE
#define CS_OTA_CONTENT_TYPE1	"application/vnd.wap.connectivity-wbxml"
#define CS_OTA_CONTENT_TYPE2	"text/vnd.wap.connectivity-xml"
#endif

#define JDOWNLOAD_INPUTBOX_FILENAME_MAX			41
#define	JWAP_MAX_FILE_EXT_LEN		10
#define	JWAP_SAVE_PAGE_FILE_EXT		".mrt"

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

#endif // __JATAAYU_APP__

#endif // __JWAP_MMS_COMMON
