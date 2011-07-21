#define MODULE_NAME "JDD_MESSAGINGCONFIG" 
#define FILE_NAME "jdd_messagingconfig.c"
 /***************************************************************************
 *
 * File Name jdd_messagingconfig.c
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
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY 
 * KIND AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
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
 ***************************************************************************/
 
 /** 
 * @file		jdd_messagingconfig.c
 * @ingroup	jdd_messagingconfig
 * @brief	Describes the Messaging Configuration related API's.
 **/

#ifdef JATAAYU_SUPPORT
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/
#include <ddl.h>
#include <jcal.h>
#include <jdi_cutils.h>
#include <jdd_messagingconfig.h>
#include <jdi_cutils_tcsstr.h>
#include <jwap_mms_common.h>
#ifdef ENABLE_MSG_HTML_DISPLAY
#include <jdi_de.h>
#include <jdi_arr.h>
#endif
#include "jmms_str_id.h"
#include "custdatares.h"

 /***************************************************************************
 * Macros
 **************************************************************************/
#ifdef JDD_LOG_ENABLED
#define __MODULE_ID__					CONFIG_MODULE
#else
#define __MODULE_ID__					0
#endif

#define	MMS_FOLDER_NAMES_FILE				"MMS_Folders.bin"
#define	EMAIL_FOLDER_NAMES_FILE				"EMAIL_Folders.bin"
#define EMS_FOLDER_NAMES_FILE				"EMS_Folders.bin"

#define VIEW_DEFAULT_TEXT_CONTROL_SPACING	(1)
#define VIEW_DEFAULT_SCROLL_BAR_WIDTH		jwap_GetScrollbarWidth()
#define VIEW_DEFAULT_MULTIPART_PAGE_TIME	(5000)
#define VIEW_DEFAULT_SMIL_PAGE_TIME			(5*1000)
#define VIEW_DEFAULT_FONT_SIZE				(11)
#define VIEW_DEFAULT_FOCUS_LINE_WIDTH		(1)
#define VIEW_DEFAULT_MEDIA_SPACING			(4)
#define VIEW_DEFAULT_FOCUS_LINE_SPACING		(2)
#define VIEW_DEFAULT_RENDERING_RECT_SPACING	(4)
#define VIEW_DEFAULT_BG_COLOR				"#ffffff"
#define VIEW_DEFAULT_FOCUS_COLOR			"black"
#define VIEW_DEFAULT_FONT_NAME				"Arial"
#define VIEW_DEFAULT_TEXT_COLOR				"black"
#define VIEW_REGION_1_NAME					"Text"
#define VIEW_REGION_2_NAME					"Image"
#define VIEW_CORRUPTED_IMAGE_PATH			CFG_MMS_ROOT_FODER_PATH "unsupported.jpg"
#define VIEW_UNKNOWN_IMAGE_PATH				CFG_MMS_ROOT_FODER_PATH "unsupported.jpg"
#define VIEW_DRM_IMAGE_PATH					".\\drm.bmp"
#define VIEW_CORRUPTED_IMAGE_TYPE			"image/jpg"
#define VIEW_UNKNOWN_IMAGE_TYPE				"image/jpg"
#define VIEW_DRM_IMAGE_TYPE					"image/jpg"
#define VIEW_DEFAULT_FORWARD_STRING			"Fw:"
#define VIEW_DEFAULT_REPLY_STRING			"Re:"
#define VIEW_DEFAULT_TAB_SPACING			(8)

#define DEVICE_CHAR_WIDTH                   (200)
#define DEVICE_CHAR_HEIGHT                  (270)

#define	DEFAULT_FILE_CHUNK_SIZE				(5120)
#define DE_LISTENER_TIMER					(1000)

/**
 * @brief	Specifies the path in which core can create its temporary files for
 *			Email Application
 **/
#define	EMAIL_TEMP_FILE_PATH	"EmailTempFiles"

 /***************************************************************************
 * Global Variable declarations and definitions
 **************************************************************************/
/**
 * @ingroup		jdd_messagingconfig
 * @brief		gets the file extension associated with a content type.
 * @note			Application to add more known content type in the list to 
 *				get expected extension.
 * @see			#jdd_MsgConfigGetFileNameWithExt
 */
 static const JC_INT8 *pFileNameExtension []= 
 {
	"text/html",".htm",
	"text/plain",	".txt",
	"text/x-vcalendar",	".vcs",
	"text/x-vcard",	".vcf",	
	"image/gif",".gif",
	"image/jpeg",	".jpg",
	"image/jpeg",	".jpeg",
	"image/tiff",	".tiff",
	"image/png",	".png",
	"image/bmp",	".bmp",
	"image/x-bmp", ".bmp",
	"image/vnd.wap.wbmp",".wbmp",
	"image/wbmp", ".wbmp",	
	"text/xml",".xml",	
	"audio/amr",".amr",
	"audio/mid",".mid",
	"audio/midi", ".mid",
	"audio/sp-midi", ".mid",
	"audio/wav",".wav",
	"audio/mp3",".mp3",
	"audio/mpeg", ".mp3",
	"audio/vnd.qcelp", ".qcp",
    "application/vnd.smaf", ".mmf",
	"audio/mmf", ".mmf",
	"audio/imy", ".imy",
	"text/x-imelody",".imy",	
	"video/3gpp", ".3gp",
	"video/3gpp2", ".3g2",
	"video/mpeg", ".mpeg",
	"video/avi", ".avi",
	"video/mp4", ".mp4",	
	"message/disposition-notification" , ".unk",
	"application/octet-stream",	".unk",
	"application/msword",			".doc",
	"application/vnd.ms-excel",	".xls",
	"application/vnd.ms-powerpoint",	".ppt",
	"application/smil",	".smil",
	"application/pdf",	".pdf",
	"\0","\0"
} ;

/**
 * @ingroup		jdd_messagingconfig
 * @brief		List of all content type supported by application.
 * @note			Application to add more content type in the list according  
 *				to platform Content support.
 * @see			#jdd_MsgConfigIsSupportedContentType
 */
 
static const JC_INT8 *pSupportedContentTypes [] = 
{
	"text/plain",
	"image/bmp",
	"image/gif",
	"image/jpeg",
	"image/jpg",
	"image/vnd.wap.wbmp",
	"image/wbmp",
	"image/png",
	"audio/amr",
	"audio/mp3",
       "audio/mpeg",
	"audio/wav",
	"audio/aac",
	"audio/mid",
	"audio/midi",
	"audio/sp-midi",
	"video/avi",
	"video/mp4",	
	"video/3gpp",
	JC_NULL
} ;

static const JC_INT8 *gpURLSchemes [] =
{
	"http://",
	"https://",
	"www.", 
	"rtp://", 
	"rtsp://", 
	JC_NULL
} ;


static JDD_FSHANDLE g_vMsgCfgFileSys = JC_NULL ;

 /***************************************************************************
 * Local Function Prototypes
 **************************************************************************/
 static JC_RETCODE MsgConfigGetImageInfo(JC_OBJECT_INFO **ppImageInfo, 
				EMsgConfigIconType eCfgIcon, EMsgConfigDRMError eCfgDRMErr) ;
 
 /***************************************************************************
 * All Local Function Definitions
 **************************************************************************/
 
 /**
 * @ingroup		jdd_messagingconfig
 * @brief		Retrieves Image presentation information.
 * @param[out]	ppstImageInfo Specifies Image presentation information
 * @param[in]	eCfgIcon Specifies Icon type information.
 * @param[in]	eCfgDRMErr Specifies DRM Error information.
 * @retval		JC_OK On Success.
 * @retval		JC_ERR_FILE_NOT_FOUND for file not found.
 * @retval		JC_ERR_MEMORY_ALLOCATION for Insufficient memory.
 * @see			#jdd_MsgConfigGetDefaultErrorInfo
 */

const u8 unsupportedBuffer[]={
	0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46, 0x00, 0x01, 0x01, 0x01, 0x00, 0x60, 
    0x00, 0x60, 0x00, 0x00, 0xFF, 0xDB, 0x00, 0x43, 0x00, 0x08, 0x06, 0x06, 0x07, 0x06, 0x05, 0x08, 
    0x07, 0x07, 0x07, 0x09, 0x09, 0x08, 0x0A, 0x0C, 0x14, 0x0D, 0x0C, 0x0B, 0x0B, 0x0C, 0x19, 0x12, 
    0x13, 0x0F, 0x14, 0x1D, 0x1A, 0x1F, 0x1E, 0x1D, 0x1A, 0x1C, 0x1C, 0x20, 0x24, 0x2E, 0x27, 0x20, 
    0x22, 0x2C, 0x23, 0x1C, 0x1C, 0x28, 0x37, 0x29, 0x2C, 0x30, 0x31, 0x34, 0x34, 0x34, 0x1F, 0x27, 
    0x39, 0x3D, 0x38, 0x32, 0x3C, 0x2E, 0x33, 0x34, 0x32, 0xFF, 0xDB, 0x00, 0x43, 0x01, 0x09, 0x09, 
    0x09, 0x0C, 0x0B, 0x0C, 0x18, 0x0D, 0x0D, 0x18, 0x32, 0x21, 0x1C, 0x21, 0x32, 0x32, 0x32, 0x32, 
    0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 
    0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 
    0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0xFF, 0xC0, 
    0x00, 0x11, 0x08, 0x00, 0x3F, 0x00, 0x7D, 0x03, 0x01, 0x22, 0x00, 0x02, 0x11, 0x01, 0x03, 0x11, 
    0x01, 0xFF, 0xC4, 0x00, 0x1F, 0x00, 0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 
    0x0A, 0x0B, 0xFF, 0xC4, 0x00, 0xB5, 0x10, 0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 0x05, 
    0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7D, 0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 
    0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07, 0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xA1, 0x08, 0x23, 
    0x42, 0xB1, 0xC1, 0x15, 0x52, 0xD1, 0xF0, 0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0A, 0x16, 0x17, 
    0x18, 0x19, 0x1A, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 
    0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 
    0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 
    0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 
    0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 
    0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 
    0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF1, 
    0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFF, 0xC4, 0x00, 0x1F, 0x01, 0x00, 0x03, 
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
    0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0xFF, 0xC4, 0x00, 0xB5, 0x11, 0x00, 
    0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77, 0x00, 
    0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71, 0x13, 
    0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xA1, 0xB1, 0xC1, 0x09, 0x23, 0x33, 0x52, 0xF0, 0x15, 
    0x62, 0x72, 0xD1, 0x0A, 0x16, 0x24, 0x34, 0xE1, 0x25, 0xF1, 0x17, 0x18, 0x19, 0x1A, 0x26, 0x27, 
    0x28, 0x29, 0x2A, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 
    0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 
    0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 
    0x89, 0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 
    0xA7, 0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 
    0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE2, 
    0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 
    0xFA, 0xFF, 0xDA, 0x00, 0x0C, 0x03, 0x01, 0x00, 0x02, 0x11, 0x03, 0x11, 0x00, 0x3F, 0x00, 0xF6, 
    0xF8, 0xED, 0xD6, 0xE6, 0xEE, 0xF4, 0xC9, 0x24, 0xFF, 0x00, 0x24, 0xC1, 0x54, 0x24, 0xEE, 0xA0, 
    0x0F, 0x2D, 0x0F, 0x40, 0x40, 0xEA, 0x4D, 0x4B, 0xFD, 0x9D, 0x07, 0xF7, 0xEE, 0xBF, 0xF0, 0x2A, 
    0x5F, 0xFE, 0x2A, 0x8B, 0x3F, 0xF8, 0xFA, 0xBF, 0xFF, 0x00, 0xAE, 0xE3, 0xFF, 0x00, 0x45, 0xA5, 
    0x60, 0x48, 0xBA, 0x4D, 0x86, 0x93, 0x3E, 0xA7, 0x7D, 0xA4, 0x25, 0xD2, 0x0B, 0xC9, 0xFC, 0xF9, 
    0x52, 0xD9, 0x24, 0x68, 0xD3, 0xCE, 0x70, 0x5D, 0xB3, 0xC9, 0x55, 0x1D, 0x71, 0x92, 0x07, 0x6C, 
    0x0A, 0x96, 0xD2, 0x57, 0x65, 0xC2, 0x12, 0x9C, 0x94, 0x63, 0xBB, 0x37, 0xFF, 0x00, 0xB3, 0xA0, 
    0xFE, 0xFD, 0xD7, 0xFE, 0x05, 0x4B, 0xFF, 0x00, 0xC5, 0x51, 0xFD, 0x9D, 0x07, 0xF7, 0xEE, 0xBF, 
    0xF0, 0x2A, 0x5F, 0xFE, 0x2A, 0xB8, 0xBD, 0x65, 0xF4, 0xDB, 0x9D, 0x0F, 0x4A, 0xD4, 0xF4, 0xCD, 
    0x33, 0xEC, 0x4A, 0xFA, 0xD5, 0x8A, 0x45, 0x37, 0x90, 0xB1, 0x34, 0xB1, 0x9B, 0x98, 0xC6, 0xE5, 
    0xC7, 0x21, 0x18, 0x13, 0xD7, 0x04, 0x8E, 0xD8, 0x35, 0xDF, 0xD3, 0x56, 0x6A, 0xE8, 0x53, 0x84, 
    0xA1, 0x27, 0x19, 0x68, 0xD1, 0x53, 0xFB, 0x3A, 0x0F, 0xEF, 0xDD, 0x7F, 0xE0, 0x54, 0xBF, 0xFC, 
    0x55, 0x1F, 0xD9, 0xD0, 0x7F, 0x7E, 0xEB, 0xFF, 0x00, 0x02, 0xA5, 0xFF, 0x00, 0xE2, 0xAA, 0xDD, 
    0x14, 0xEC, 0x49, 0x53, 0xFB, 0x3A, 0x0F, 0xEF, 0xDD, 0x7F, 0xE0, 0x54, 0xBF, 0xFC, 0x55, 0x1F, 
    0xD9, 0xD0, 0x7F, 0x7E, 0xEB, 0xFF, 0x00, 0x02, 0xA5, 0xFF, 0x00, 0xE2, 0xAA, 0xDD, 0x14, 0x58, 
    0x0A, 0x9F, 0xD9, 0xD0, 0x7F, 0x7E, 0xEB, 0xFF, 0x00, 0x02, 0xA5, 0xFF, 0x00, 0xE2, 0xA8, 0xFE, 
    0xCE, 0x83, 0xFB, 0xF7, 0x5F, 0xF8, 0x15, 0x2F, 0xFF, 0x00, 0x15, 0x56, 0xE8, 0xA2, 0xC0, 0x54, 
    0xFE, 0xCE, 0x83, 0xFB, 0xF7, 0x5F, 0xF8, 0x15, 0x2F, 0xFF, 0x00, 0x15, 0x47, 0xF6, 0x74, 0x1F, 
    0xDF, 0xBA, 0xFF, 0x00, 0xC0, 0xA9, 0x7F, 0xF8, 0xAA, 0xB7, 0x45, 0x16, 0x02, 0xA7, 0xF6, 0x74, 
    0x1F, 0xDF, 0xBA, 0xFF, 0x00, 0xC0, 0xA9, 0x7F, 0xF8, 0xAA, 0x3F, 0xB3, 0xA0, 0xFE, 0xFD, 0xD7, 
    0xFE, 0x05, 0x4B, 0xFF, 0x00, 0xC5, 0x55, 0xBA, 0x28, 0xB0, 0x15, 0x3F, 0xB3, 0xA0, 0xFE, 0xFD, 
    0xD7, 0xFE, 0x05, 0x4B, 0xFF, 0x00, 0xC5, 0x55, 0x88, 0x97, 0x6C, 0x41, 0x41, 0x24, 0x2E, 0x40, 
    0xDC, 0x49, 0x3D, 0x7D, 0x4F, 0x5A, 0x7D, 0x35, 0x3E, 0xE9, 0xFA, 0x9F, 0xE7, 0x48, 0x0A, 0xD6, 
    0x7F, 0xF1, 0xF5, 0x7F, 0xFF, 0x00, 0x5D, 0xC7, 0xFE, 0x8B, 0x4A, 0xE3, 0xE6, 0xF1, 0x37, 0x82, 
    0x5E, 0xDA, 0x7D, 0x2B, 0x5B, 0xD6, 0xF4, 0xD0, 0xD0, 0x6A, 0x13, 0xB4, 0x96, 0xD2, 0xDD, 0x05, 
    0xF9, 0x96, 0x77, 0x20, 0x3A, 0x83, 0xF3, 0x0E, 0x87, 0x6B, 0x64, 0x74, 0xC8, 0xE2, 0xBB, 0x0B, 
    0x3F, 0xF8, 0xFA, 0xBF, 0xFF, 0x00, 0xAE, 0xE3, 0xFF, 0x00, 0x45, 0xA5, 0x5B, 0xA2, 0xC9, 0xAB, 
    0x32, 0xA3, 0x29, 0x42, 0x4A, 0x51, 0x76, 0x68, 0xF3, 0x0D, 0x5B, 0x5F, 0xF0, 0x74, 0x9A, 0x7E, 
    0x99, 0xA5, 0x78, 0x7B, 0x58, 0xD3, 0xE5, 0x79, 0x35, 0xDB, 0x19, 0x96, 0xD6, 0x0B, 0xA0, 0xF8, 
    0xCD, 0xD4, 0x65, 0xB6, 0x2E, 0x7E, 0x51, 0xD4, 0xED, 0x5C, 0x01, 0xC9, 0xC7, 0x5A, 0xF4, 0xFA, 
    0x28, 0xA1, 0x24, 0x95, 0x90, 0x4A, 0x52, 0x9B, 0x72, 0x93, 0xBB, 0x61, 0x45, 0x14, 0x53, 0x24, 
    0x28, 0xA2, 0x8A, 0x00, 0x28, 0xA2, 0x8A, 0x00, 0x28, 0xA2, 0x8A, 0x00, 0x28, 0xA2, 0x8A, 0x00, 
    0x29, 0xA9, 0xF7, 0x4F, 0xD4, 0xFF, 0x00, 0x3A, 0x75, 0x35, 0x3E, 0xE9, 0xFA, 0x9F, 0xE7, 0x40, 
    0x15, 0xAC, 0xFF, 0x00, 0xE3, 0xEA, 0xFF, 0x00, 0xFE, 0xBB, 0x8F, 0xFD, 0x16, 0x95, 0x6E, 0xAA, 
    0x59, 0xFF, 0x00, 0xC7, 0xD5, 0xFF, 0x00, 0xFD, 0x77, 0x1F, 0xFA, 0x2D, 0x2A, 0xDD, 0x08, 0x02, 
    0x8A, 0x28, 0xA0, 0x02, 0x8A, 0x28, 0xA0, 0x02, 0x8A, 0x28, 0xA0, 0x02, 0x8A, 0x28, 0xA0, 0x02, 
    0x8A, 0x28, 0xA0, 0x02, 0x8A, 0x28, 0xA0, 0x02, 0x9A, 0x9F, 0x74, 0xFD, 0x4F, 0xF3, 0xA7, 0x53, 
    0x53, 0xEE, 0x9F, 0xA9, 0xFE, 0x74, 0x01, 0x5A, 0xCF, 0xFE, 0x3E, 0xAF, 0xFF, 0x00, 0xEB, 0xB8, 
    0xFF, 0x00, 0xD1, 0x69, 0x56, 0xEA, 0x08, 0xA1, 0x68, 0xA6, 0xB8, 0x70, 0x43, 0x09, 0x64, 0x0F, 
    0xCF, 0x18, 0xF9, 0x55, 0x71, 0xFF, 0x00, 0x8E, 0xFE, 0xB5, 0x2E, 0x5F, 0xFB, 0xAB, 0xFF, 0x00, 
    0x7D, 0x7F, 0xF5, 0xA9, 0x00, 0xEA, 0x29, 0xB9, 0x7F, 0xEE, 0xAF, 0xFD, 0xF5, 0xFF, 0x00, 0xD6, 
    0xA3, 0x2F, 0xFD, 0xD5, 0xFF, 0x00, 0xBE, 0xBF, 0xFA, 0xD4, 0xC0, 0x75, 0x14, 0xDC, 0xBF, 0xF7, 
    0x57, 0xFE, 0xFA, 0xFF, 0x00, 0xEB, 0x51, 0x97, 0xFE, 0xEA, 0xFF, 0x00, 0xDF, 0x5F, 0xFD, 0x6A, 
    0x00, 0x75, 0x14, 0xDC, 0xBF, 0xF7, 0x57, 0xFE, 0xFA, 0xFF, 0x00, 0xEB, 0x51, 0x97, 0xFE, 0xEA, 
    0xFF, 0x00, 0xDF, 0x5F, 0xFD, 0x6A, 0x00, 0x75, 0x14, 0xDC, 0xBF, 0xF7, 0x57, 0xFE, 0xFA, 0xFF, 
    0x00, 0xEB, 0x51, 0x97, 0xFE, 0xEA, 0xFF, 0x00, 0xDF, 0x5F, 0xFD, 0x6A, 0x00, 0x75, 0x14, 0xDC, 
    0xBF, 0xF7, 0x57, 0xFE, 0xFA, 0xFF, 0x00, 0xEB, 0x51, 0x97, 0xFE, 0xEA, 0xFF, 0x00, 0xDF, 0x5F, 
    0xFD, 0x6A, 0x00, 0x75, 0x14, 0xDC, 0xBF, 0xF7, 0x57, 0xFE, 0xFA, 0xFF, 0x00, 0xEB, 0x51, 0x97, 
    0xFE, 0xEA, 0xFF, 0x00, 0xDF, 0x5F, 0xFD, 0x6A, 0x00, 0x75, 0x35, 0x3E, 0xE9, 0xFA, 0x9F, 0xE7, 
    0x46, 0x5F, 0xFB, 0xAB, 0xFF, 0x00, 0x7D, 0x7F, 0xF5, 0xA8, 0x40, 0x42, 0xF3, 0xD7, 0x24, 0xD2, 
    0x03, 0xFF, 0xD9,
};
JC_RETCODE MsgConfigGetImageInfo (JC_OBJECT_INFO **ppstImageInfo, 
								 EMsgConfigIconType eCfgIcon, 
								 EMsgConfigDRMError eCfgDRMErr) 
{
	JC_RETCODE		rCode = JC_ERR_INVALID_PARAMETER ;
	JC_UINT32		uiFileSize = 0 ;
	JDD_FSHANDLE	vFileSysHandle =JC_NULL ;
	JC_OBJECT_INFO	*pstImageInfo = JC_NULL ;
	JC_CHAR			*pmFileName = JC_NULL ;
	JC_INT8			*pcFilePath = JC_NULL ;
	JDD_FILE 		hFile  = JC_NULL;
	
	JDD_LOG_INFO0 ("MsgConfigGetImageInfo: Entry") ;
	
	jdd_AssertOnFail ((JC_NULL != ppstImageInfo) ,\
		"MsgConfigGetImageInfo: ppstImageInfo is NULL") ;
	
	PARAM_INTENTIONALLY_NOT_USED (eCfgDRMErr) ;
	
	vFileSysHandle =jdd_MsgConfigGetFSHandle () ;
	
	switch(eCfgIcon)
	{
		case E_CONFIG_UNSUPPORTED_ICON:
			pcFilePath = VIEW_UNKNOWN_IMAGE_PATH ;
			break;
		case E_CONFIG_CORRUPTED_ICON:
			pcFilePath = VIEW_CORRUPTED_IMAGE_PATH ;
			break;
		case E_CONFIG_DRM_ICON:
			pcFilePath = VIEW_DRM_IMAGE_PATH ;
			break;
		default:
			break;
	}
	if(pcFilePath)
	{
		jdi_CUtilsCharToTchar (JC_NULL, pcFilePath, &pmFileName) ;
		if(pmFileName)
		{
			if (E_FALSE == jdd_FSIsFileExist(vFileSysHandle, pmFileName))
			{
				//rCode = JC_ERR_FILE_NOT_FOUND ;
				JDD_LOG_INFO1 ("MsgConfigGetImageInfo: Returns [%d]", rCode) ;
				// write const buffer to  file
				hFile = jdd_FSOpen (JC_NULL, pmFileName, E_CREATE_WRITE_MODE) ;
				if(hFile)
				{					
					rCode = jdd_FSWrite (unsupportedBuffer, sizeof (unsupportedBuffer), 1, hFile);
					rCode = jdd_FSClose(hFile);
				}
				else
				{
					rCode = JC_ERR_FILE_NOT_FOUND ;
				}
				uiFileSize = sizeof(unsupportedBuffer);

			}
			else
			{
				jdd_FSGetFileSize (vFileSysHandle, pmFileName, &uiFileSize) ;
				rCode = JC_OK ;
			}
		}		
	}

	if(JC_OK == rCode)
	{
		pstImageInfo = (JC_OBJECT_INFO*) jdd_MemAlloc \
											(sizeof(JC_OBJECT_INFO), 1) ;
		if(JC_NULL != pstImageInfo)
		{
			pstImageInfo->bIsFileContent = E_TRUE ;
			pstImageInfo->uBuffer.pmFileName = pmFileName ;
			pstImageInfo->uiObjectLen = uiFileSize ;
			pstImageInfo->psMimeType = jdi_CUtilsStrDuplicate (JC_NULL, pcFilePath) ;
			*ppstImageInfo = pstImageInfo ;
		}
		else
		{
			rCode = JC_ERR_MEMORY_ALLOCATION ;
			JDD_LOG_INFO1 ("MsgConfigGetImageInfo: Returns [%d]", rCode) ;
		}
	}
	else
	{
		//free filename
		jdd_MemFree(pmFileName);
	}
	
	JDD_LOG_INFO0 ("MsgConfigGetImageInfo: Exit") ;
	return (rCode) ;
}
 
 /***************************************************************************
 * All Global Function Definitions
 **************************************************************************/

/**
 * @ingroup		jdd_messagingconfig
 * @brief		Retrieves default presentation information.
 * @param[out]	pstPresentationInfo Specifies presentation information
 * @param[in]	uiConfigArg Pointer to user-defined information
 * @retval		JC_OK On Success
 *
 * Application can set its own presentation information
 * using this API implementation.
 *
 * @note
 *    - Memory for pstPresentationInfo is allocated and freed by the Core Module.
 *    - This API is applicable only to MMS
 */
JC_RETCODE jdd_MsgConfigGetDefaultPresentationInfo ( \
				MSG_CONFIG_PRESENTATION_INFO *pstPresentationInfo, \
				JC_UINT32 uiConfigArg) 
{
	JDD_LOG_INFO0 ("jdd_MsgConfigGetDefaultPresentationInfo: Entry") ;

	jdd_AssertOnFail ((JC_NULL != pstPresentationInfo) , \
		"jdd_MsgConfigGetDefaultPresentationInfo: \
		pstPresentationInfo is NULL") ;

	PARAM_INTENTIONALLY_NOT_USED (uiConfigArg) ;	
	
	pstPresentationInfo->eDefaultPresentation = E_CONFIG_PRESENTATION_SMIL ;
	pstPresentationInfo->uiDefaultMultipartPageTime = \
								(JC_UINT32)VIEW_DEFAULT_MULTIPART_PAGE_TIME ;
	pstPresentationInfo->uiDefaultSMILPageTime = VIEW_DEFAULT_SMIL_PAGE_TIME ;
	pstPresentationInfo->bTimerUnit = E_TRUE ; /* for Milliseconds */

	JDD_LOG_INFO0 ("jdd_MsgConfigGetDefaultPresentationInfo : Exit") ;

	return	(JC_OK) ;
}

/**
 * @ingroup		jdd_messagingconfig
 * @brief		Retrieves default text information.
 * @param[out]	pstTextInfo Specifies default text information
 * @param[in]	uiConfigArg Points to user-defined information
 * @retval		JC_OK On Success
 *
 * Application can set its own text information
 * using this API implementation.
 *
 * @note
 *    - Memory for pstTextInfo is allocated and freed by the Core Module.
 *    - This API is applicable only to MMS
 */
JC_RETCODE jdd_MsgConfigGetDefaultTextInfo
				(MSG_CONFIG_TEXT_INFO *pstTextInfo, JC_UINT32 uiConfigArg)
{

	JDD_LOG_INFO0 ("jdd_MsgConfigGetDefaultTextInfo: Entry") ;

	jdd_AssertOnFail ((JC_NULL != pstTextInfo) , \
		"jdd_MsgConfigGetDefaultTextInfo: pstTextInfo is NULL") ;

	PARAM_INTENTIONALLY_NOT_USED (uiConfigArg) ;
	
	jc_strcpy (pstTextInfo->pFontName , VIEW_DEFAULT_FONT_NAME) ;
	jc_strcpy (pstTextInfo->pTextColor , VIEW_DEFAULT_TEXT_COLOR) ;
	jc_strcpy (pstTextInfo->pActiveLinkColor , "red") ;
	jc_strcpy (pstTextInfo->pInactiveLinkColor , "blue") ;
	
	pstTextInfo->uiTextSize = VIEW_DEFAULT_FONT_SIZE ;

	JDD_LOG_INFO0 ("jdd_MsgConfigGetDefaultTextInfo : Exit") ;
	
	return	(JC_OK) ;
}


/**
 * @ingroup		jdd_messagingconfig
 * @brief		Retrieves default focus information.
 * @param[out]	pstFocusInfo Specifies focusing information
 * @param[in]	uiConfigArg Pointer to user-defined information
 * @retval		JC_OK On Success
 *
 * Application can set its own focus information for focused media object,
 * using this API implementation.
 *
 * @note
 *    - Memory for pstFocusInfo is allocated and freed by the Core Module
 *    - This API is applicable only to MMS
 */
JC_RETCODE jdd_MsgConfigGetDefaultFocusingInfo ( \
				MSG_CONFIG_FOCUS_INFO *pstFocusInfo, JC_UINT32 uiConfigArg)
{
	JDD_LOG_INFO0 ("jdd_MsgConfigGetDefaultFocusingInfo: Entry") ;

	jdd_AssertOnFail ((JC_NULL != pstFocusInfo) , \
		"jdd_MsgConfigGetDefaultFocusingInfo: pstFocusInfo is NULL") ;

	PARAM_INTENTIONALLY_NOT_USED (uiConfigArg) ;
	
	pstFocusInfo->eFocusLineStyle = E_DOTTED_BORDER ;
	jc_strcpy (pstFocusInfo->pFocusLineColor , VIEW_DEFAULT_FOCUS_COLOR) ;
	pstFocusInfo->uiFocusLineWidth = VIEW_DEFAULT_FOCUS_LINE_WIDTH ;

	JDD_LOG_INFO0 ("jdd_MsgConfigGetDefaultFocusingInfo : Exit") ;

	return (JC_OK) ;
}

/**
 * @ingroup		jdd_messagingconfig
 * @brief		Retrieves default view setting information.
 * @param[out]	pstViewSettingsInfo Specifies view settings information
 * @param[in]	uiConfigArg Points to user-defined information
 * @retval		JC_OK On Success
 *
 * Application can set its own view settings
 * for rendering the message, using this API implementation.
 *
 * @note
 *    - Memory for pstViewSettingsInfo is allocated and freed by the Core Module.
 *    - This API is applicable only to MMS
 */
JC_RETCODE jdd_MsgConfigGetDefaultViewSettingsInfo ( \
						MSG_CONFIG_VIEW_SETTINGS_INFO *pstViewSettingsInfo, 
						JC_UINT32 uiConfigArg) 
{
	JDD_LOG_INFO0 ("jdd_MsgConfigGetDefaultViewSettingsInfo: Entry") ;

	jdd_AssertOnFail ((JC_NULL != pstViewSettingsInfo) ,\
		"jdd_MsgConfigGetDefaultViewSettingsInfo: \
		pstViewSettingsInfo is NULL") ;
	PARAM_INTENTIONALLY_NOT_USED (uiConfigArg) ;

	pstViewSettingsInfo->bPreferMediaTime = E_FALSE ;
	pstViewSettingsInfo->bUseConfigSMIL = E_TRUE ;
	pstViewSettingsInfo->eImageAlign = E_CONFIG_CENTRE_ALIGNMENT ;
	pstViewSettingsInfo->eTextAlign = E_CONFIG_LEFT_ALIGNMENT ;
	pstViewSettingsInfo->bValidateLayout = E_FALSE ;
	pstViewSettingsInfo->bValidateTimer = E_TRUE ;
	pstViewSettingsInfo->eLayout = E_IMAGE_TOP_TEXT_BOTTOM ;	
	pstViewSettingsInfo->uiCacheSize = 20; // will cache the slide text

	JDD_LOG_INFO0 ("jdd_MsgConfigGetDefaultViewSettingsInfo : Exit") ;

	return	(JC_OK) ;
}

/**
 * @ingroup		jdd_messagingconfig
 * @brief		Retrieves default error information.
 * @param[out]	pstErrorInfo Specifies default error information
 * @param[in]	uiConfigArg Pointer to user-defined information
 * @retval		JC_OK On Success
 * @retval		JC_ERR_MEMORY_ALLOCATION If memory allocation fails
 * @retval		JC_ERR_FILE_NOT_FOUND if File handling errors.
 *
 * Application can set its own error info to be used for handling
 * corrupted or unknown content, using this API implementation.
 *
 * @note
 *    - Memory for pstErrorInfo is allocated and freed by the Core Module.
 *    - This API is applicable only to MMS
 */
JC_RETCODE jdd_MsgConfigGetDefaultErrorInfo  ( \
				MSG_CONFIG_ERROR_INFO *pstErrorInfo, JC_UINT32 uiConfigArg)
{
	JC_OBJECT_INFO	*pstImageInfo = JC_NULL ;
	JC_RETCODE		rCode = JC_OK ;
	
	JDD_LOG_INFO0 ("jdd_MsgConfigGetDefaultErrorInfo: Entry") ;
	
	jdd_AssertOnFail ((JC_NULL != pstErrorInfo) ,\
		"jdd_MsgConfigGetDefaultErrorInfo: pstErrorInfo is NULL") ;
	PARAM_INTENTIONALLY_NOT_USED (uiConfigArg) ;
	
	rCode = MsgConfigGetImageInfo (&pstImageInfo, pstErrorInfo->eIconType, \
									pstErrorInfo->eDRMError) ;
	if(JC_OK == rCode)
	{
		pstErrorInfo->pImageInfo = pstImageInfo ;
	}
	else
	{
		if (pstImageInfo && pstImageInfo->psMimeType)
		{
			jdd_MemFree(pstImageInfo->psMimeType) ;
		}
		if (pstImageInfo)
		{
			jdd_MemFree(pstImageInfo) ;
		}
		JDD_LOG_INFO1 ("jdd_MsgConfigGetDefaultErrorInfo: Returns [%d]", rCode) ;
	}

	JDD_LOG_INFO0 ("jdd_MsgConfigGetDefaultErrorInfo : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_messagingconfig
 * @brief		Retrieves default spacing information.
 * @param[out]	pstSpacingInfo Specifies default spacing information
 * @param[in]	uiConfigArg Pointer to user-defined information
 * @retval		JC_OK On Success
 *
 * Application can set its own spacing information, for media objects,
 * for rendering the message, using this API implementation.
 *
 * @note
 *    - Memory for pstSpacingInfo is allocated and freed by the Core Module.
 *    - This API is applicable only to MMS
 */
 extern  JC_UINT32 jwap_GetScrollbarWidth();
JC_RETCODE jdd_MsgConfigGetDefaultSpacingInfo ( \
			MSG_CONFIG_SPACING_INFO *pstSpacingInfo, \
			JC_UINT32 uiConfigArg) 
{
	JDD_LOG_INFO0 ("jdd_MsgConfigGetDefaultSpacingInfo: Entry") ;
	
	jdd_AssertOnFail ((JC_NULL != pstSpacingInfo), \
		"jdd_MsgConfigGetDefaultSpacingInfo: pstSpacingInfo is NULL") ;
	
	PARAM_INTENTIONALLY_NOT_USED (uiConfigArg) ;	
	
	pstSpacingInfo->uiTabLength		= VIEW_DEFAULT_TAB_SPACING ;
	pstSpacingInfo->uiFocusLineSpacing = VIEW_DEFAULT_FOCUS_LINE_SPACING ;
	pstSpacingInfo->uiControlSpacing = VIEW_DEFAULT_TEXT_CONTROL_SPACING ;
	pstSpacingInfo->uiMediaSpacing = VIEW_DEFAULT_MEDIA_SPACING ;
	pstSpacingInfo->uiRenderingRectSpacing = \
					VIEW_DEFAULT_RENDERING_RECT_SPACING ;
	pstSpacingInfo->uiScrollBarWidth = VIEW_DEFAULT_SCROLL_BAR_WIDTH ;

	JDD_LOG_INFO0 ("jdd_MsgConfigGetDefaultSpacingInfo : Exit") ;

	return (JC_OK) ;
}

/**
 * @ingroup		jdd_messagingconfig
 * @brief		Retrieves default text value
 * @param[out]	pstTextValueInfo Specifies default text value
 * @param[in]	uiConfigArg Pointer to user-defined information
 * @retval		JC_OK On Success
 *
 * Application can set its own texts for pre appending to reply
 * or forward subject fields, using this API implementation.
 *
 * @note
 *    - Memory for pstTextValueInfo is allocated and freed by the Core Module.
 *    - This API is applicable only to MMS
 */
JC_RETCODE jdd_MsgConfigGetDefaultTextValueInfo ( \
				MSG_CONFIG_TEXT_VALUE_INFO *pstTextValueInfo, \
				JC_UINT32 uiConfigArg)
{
	/*
	JC_CHAR *pmForwardString = JC_NULL ;
	JC_CHAR	*pmReplyString = JC_NULL ;
	*/
	JC_INT8 pmForwardString[64];
	JC_INT8	pmReplyString[64];
	JDD_LOG_INFO0 ("jdd_MsgConfigGetDefaultTextValueInfo: Entry") ;
	
	jdd_AssertOnFail ((JC_NULL != pstTextValueInfo) , \
		"jdd_MsgConfigGetDefaultTextValueInfo: pstTextValueInfo is NULL") ;
	PARAM_INTENTIONALLY_NOT_USED (uiConfigArg) ;

	memset(pmForwardString, 0, sizeof(pmForwardString));
	memset(pmReplyString, 0, sizeof(pmReplyString));
	/*
	jdi_CUtilsCharToTchar (JC_NULL, VIEW_DEFAULT_FORWARD_STRING, &pmForwardString) ;
	*/
	pfnUnicodeStrncat(pmForwardString, (PS8)GetString(STR_ID_MMS_INBOX_FORWARD), 16);
	jc_tcscpy	 (pstTextValueInfo->pForwardString , pmForwardString); 
	
	/*
	jdi_CUtilsCharToTchar (JC_NULL, VIEW_DEFAULT_REPLY_STRING, &pmReplyString) ;
	*/
	pfnUnicodeStrncat(pmReplyString, (PS8)GetString(STR_ID_MMS_INBOX_REPLY), 16);
	jc_tcscpy (pstTextValueInfo->pReplyString , pmReplyString);

	JDD_LOG_INFO0 ("jdd_MsgConfigGetDefaultTextValueInfo : Exit") ;
	/*
	jdd_MemFree(pmForwardString);
	jdd_MemFree(pmReplyString);	
	*/
	return (JC_OK) ;
}

/**
 * @ingroup		jdd_messagingconfig
 * @brief		Retrieves default SMIL information for rendering the message
 * @param[out]	pstSMILInfo Specifies view settings information
 * @param[in]	uiRootLayoutHeight Specifies root layout height
 * @param[in]	uiRootLayoutWidth Specifies root layout width
 * @param[in]	uiConfigArg Points to user-defined information
 * @retval		JC_OK On Success
 *
 * Application can set its own SMIL information for rendering the message,
 * using this API implementation.
 *
 * @note
 *    - Memory for pstSMILInfo is allocated and freed by the Core Module.
 *    - uiRootLayoutHeight and uiRootLayoutWidth are the root layout height
 *      and width passed by the core and to be assigned to the pstSMILInfo
 *      structure. If any one of these are zero then need to get these
 *      values from the device characteristics and assigned to the pstSMILInfo
 *      structure.
 *    - This API is applicable only to MMS
 */
JC_RETCODE jdd_MsgConfigGetDefaultSMILInfo ( \
				MSG_CONFIG_SMIL_INFO *pstSMILInfo, \
				JC_UINT32 uiRootLayoutHeight, \
				JC_UINT32 uiRootLayoutWidth, \
				JC_UINT32 uiConfigArg)
{
	DeviceCharacteristics	stDeviceChar = {0, } ;
	JC_RETCODE rCode = JC_OK ;
	JC_CHAR *pmString = JC_NULL ;

	JDD_LOG_INFO0 ("jdd_MsgConfigGetDefaultSMILInfo: Entry") ;
	
	jdd_AssertOnFail ((JC_NULL != pstSMILInfo), \
		"jdd_MsgConfigGetDefaultSMILInfo: pstSMILInfo is NULL") ;
	
	PARAM_INTENTIONALLY_NOT_USED (uiConfigArg) ;
	
	if(0 == uiRootLayoutHeight || 0 == uiRootLayoutWidth)
	{
		rCode = jdd_MMIGetDeviceCharacteristics (&stDeviceChar) ;
		if(JC_OK == rCode)
		{
			pstSMILInfo->uiRootLayoutWidth = stDeviceChar.uiRenderingWidth ;
			pstSMILInfo->uiRootLayoutHeight = stDeviceChar.uiRenderingHeight ;
		}
		else
		{
            if(JC_ERR_NOT_IMPLEMENTED == rCode)
            {
                pstSMILInfo->uiRootLayoutWidth = DEVICE_CHAR_WIDTH ;
                pstSMILInfo->uiRootLayoutHeight = DEVICE_CHAR_HEIGHT ;
				rCode = JC_OK ;
            }            
		}
	}
	else
	{
		pstSMILInfo->uiRootLayoutWidth = uiRootLayoutWidth ;
		pstSMILInfo->uiRootLayoutHeight = uiRootLayoutHeight ;
	}

	if(JC_OK == rCode)
	{
		jc_strcpy(pstSMILInfo->pBGColor, VIEW_DEFAULT_BG_COLOR) ;
			
		jdi_CUtilsCharToTchar (JC_NULL, VIEW_REGION_1_NAME, \
										&pmString);
		if(pmString)
		{
			jc_tcscpy (pstSMILInfo->pRegion_1_Name, pmString) ;
			jdd_MemFree (pmString) ;
		}
		pstSMILInfo->uiRegion_1_Top = 0 ;
		pstSMILInfo->uiRegion_1_Left = 0 ;
		pstSMILInfo->uiRegion_1_Width = pstSMILInfo->uiRootLayoutWidth ;
		pstSMILInfo->uiRegion_1_Height = \
						pstSMILInfo->uiRootLayoutHeight * 0.4 ;
		pstSMILInfo->eRegion_1_Fit = E_CONFIG_REGION_MEET ;

		jdi_CUtilsCharToTchar (JC_NULL, VIEW_REGION_2_NAME, &pmString);
		if (pmString)
		{
			jc_tcscpy (pstSMILInfo->pRegion_2_Name, pmString) ;
			jdd_MemFree (pmString) ;
		}
		pstSMILInfo->uiRegion_2_Top = pstSMILInfo->uiRegion_1_Height \
										+ VIEW_DEFAULT_MEDIA_SPACING ;
		pstSMILInfo->uiRegion_2_Left = 0 ;
		pstSMILInfo->uiRegion_2_Width =  pstSMILInfo->uiRootLayoutWidth ;
		pstSMILInfo->uiRegion_2_Height = pstSMILInfo->uiRootLayoutHeight * 0.6 ;
		pstSMILInfo->eRegion_2_Fit = E_CONFIG_REGION_MEET ;
	}

	JDD_LOG_INFO0 ("jdd_MsgConfigGetDefaultSMILInfo : Exit") ;	
	return (JC_OK) ;
}

/**
 * @ingroup			jdd_messagingconfig
 * @brief			Retrieves filename with extension
 * @param[in,out]		ppmFileName Specifies the file name
 * @param[in]		pcContentType Specifies the content type
 * @param[in]		uiConfigArg	Points to user defined information
 * @retval			JC_OK On Success.
 * @retval			JC_ERR_MEMORY_ALLOCATION if Memory Allocation fails.
 *
 * Core will invoke this function while parsing. Application can check the 
 * file name and optionally pass back the reformatted file name (for example
 * if extension is present).
 *
 * @note
 *	ppmFileName - If modified, application should free and allocate new 
 *  resource which will be freed by core
 */
JC_RETCODE jdd_MsgConfigGetFileNameWithExt (JC_CHAR **ppFileName, \
											const JC_INT8 *pcContentType, \
											JC_UINT32 uiConfigArg)
{
	JC_RETCODE  rCode    = JC_OK;
    JC_INT32	iCount    = 0;
    JC_INT32	iLen    = 0;
    JC_CHAR		*pFileName   = NULL;
    JC_CHAR		*pTempFileName  = NULL;
    JC_INT8     *pTempContentType = NULL;
    JC_INT8		*pExtension   = NULL ; 
    
    PARAM_INTENTIONALLY_NOT_USED (uiConfigArg) ;
    
    if(ppFileName == NULL || pcContentType == NULL)
    {
        rCode = JC_ERR_INVALID_PARAMETER;
        goto error;
    }
    pTempContentType = jdi_CUtilsStrDuplicate (JC_NULL, pcContentType) ;
    jdi_CUtilsStrToLower(pTempContentType) ;

	while(pFileNameExtension[iCount] != NULL)
    {
        if(jc_strstr(pTempContentType, (const JC_INT8 *)pFileNameExtension[iCount]) != 0) 
        {
            iCount += 1;
            break;
        }
        iCount+=2;  
    }	
    if(pFileNameExtension[iCount] == NULL)
    {
        pExtension = (JC_INT8 *)"unk" ;
    }
    else
    {
        pExtension = (JC_INT8 *)pFileNameExtension[iCount] ;
    }
	
    if(*ppFileName == NULL)
    {
    	JC_INT8 pTempName[32];		
		jc_sprintf(pTempName, "File%u%s", (JC_UINT32)jdd_MiscGetUniqueValue(),pExtension);		
		jdi_CUtilsCharToTchar(NULL, pTempName, ppFileName);		
    }
    else
    {    	
        pTempFileName = jdi_CUtilsTcsDuplicate (JC_NULL, *ppFileName) ;
		if (E_FALSE == jdi_CUtilsTcsStrEndsWith (*ppFileName, pExtension))
		{
            iLen = jc_tcslen (*ppFileName) + jc_strlen(pExtension);
            pFileName = (JC_CHAR *)jdd_MemAlloc(sizeof(JC_CHAR), iLen + 1);
            if(pFileName == NULL)
            {
                rCode = JC_ERR_MEMORY_ALLOCATION;
                goto error;
            }
            jc_tcscpy(pFileName,*ppFileName);
            jdd_MemFree(*ppFileName);
			jdi_CUtilsTcsStrCat (pExtension, pFileName) ;
            *ppFileName = pFileName;
        }
    }    
error :
    if(pTempFileName)
        jdd_MemFree(pTempFileName);
    if(pTempContentType)
        jdd_MemFree(pTempContentType);
    return rCode;
}

/**
 * @ingroup		jdd_messagingconfig
 * @brief		Retrieves the storage settings
 * @param[out]  	pstStoreSettings	On success, filled with default settings
 *				information.
 * @retval		JC_OK On Success
 * @retval		JC_ERR_MEMORY_ALLOCATION if Memory Allocation fails.
 *
 * Application can return default settings for the storage. These information
 * will be used by the messaging core for maintaining storage related files
 *
 * @note
 *		- Caller should allocate and free pstStoreSettings
 *		- On success, caller should free resources for pstStoreSettings members
 */
JC_RETCODE jdd_MsgConfigGetDefaultStoreSettings ( \
				STORE_CONFIG_SETTINGS *pstStoreSettings) 
{
	JC_RETCODE rCode = JC_OK ;
	
	JDD_LOG_INFO0 ("jdd_MsgConfigGetDefaultStoreSettings: Entry") ;
	
	jdd_AssertOnFail ((JC_NULL != pstStoreSettings), \
		"jdd_MsgConfigGetDefaultStoreSettings: pstStoreSettings is NULL") ;
	
	if (0 == pstStoreSettings->uiMsgType)
	{
		rCode = jdi_CUtilsCharToTchar (JC_NULL, MMS_FOLDER_NAMES_FILE, \
										&pstStoreSettings->pRootFile) ;
		rCode = jdi_CUtilsCharToTchar (JC_NULL, CFG_MMS_ROOT_FODER_PATH, &pstStoreSettings->pRootPath) ;
		//rCode = jdi_CUtilsCharToTchar (JC_NULL, ".\\t\\jmms\\", &pstStoreSettings->pRootPath) ;
	}
	else if (1 == pstStoreSettings->uiMsgType)
	{
		rCode = jdi_CUtilsCharToTchar (JC_NULL, EMAIL_FOLDER_NAMES_FILE, \
										&pstStoreSettings->pRootFile);
		rCode = jdi_CUtilsCharToTchar (JC_NULL, ".\\", \
										&pstStoreSettings->pRootPath) ;
	}
	else if (2 == pstStoreSettings->uiMsgType)
	{
		rCode = jdi_CUtilsCharToTchar (JC_NULL, EMS_FOLDER_NAMES_FILE, \
										&pstStoreSettings->pRootFile);
		rCode = jdi_CUtilsCharToTchar (JC_NULL, ".\\", \
										&pstStoreSettings->pRootPath) ;
	}

	JDD_LOG_INFO0 ("jdd_MsgConfigGetDefaultStoreSettings: Exit") ;

	return (rCode) ;
}

/**
 * @ingroup		jdd_messagingconfig
 * @brief		Checks if the content type is supported by the application
 * @param[in]	pcContentType Pointer to the content type to be checked
 * @retval		E_TRUE If content type is supported
 * @retval		E_FALSE If content type is not supported
 *
 * Messaging core invokes this function to validate whether the content is 
 * supported (rendering)
 *
 */
JC_BOOLEAN jdd_MsgConfigIsSupportedContentType
                                (JC_INT8 *pcContentType)
{
	JC_UINT8 ucIndex = 0 ;
	JC_BOOLEAN bIsSupportedContentType = E_FALSE ;

	
	if (JC_NULL != pcContentType)
	{
		while (JC_NULL != pSupportedContentTypes[ucIndex])
		{
			if (0 == jc_strcmp(pcContentType, pSupportedContentTypes[ucIndex]))
			{
				bIsSupportedContentType = E_TRUE ;
				break ;
			}
			ucIndex++;
		}
	}
	else
	{
		bIsSupportedContentType = E_FALSE ;
	}
	mmi_trace(12, "jdd_log: MsgConfigIsSupportedContentType[%s][%d]", pcContentType, bIsSupportedContentType);
	return bIsSupportedContentType ;
}

/**
 * @ingroup		jdd_messagingconfig
 * @brief		Retrieves media type from the content type
 * @param[in]	pcContentType Pointer to the content type to be checked
 * @retval		E_CONFIG_MEDIA_TYPE associated with the content type
 *
 * Messaging core invokes this function to get the media type from the content
 * type (rendering)
 *
 */
E_CONFIG_MEDIA_TYPE   jdd_MsgConfigGetMediaType (JC_INT8 *pcContentType) 
{
	E_CONFIG_MEDIA_TYPE	eMediaType = E_CONFIG_MEDIA_OTHERS ;
	
	if (JC_NULL != pcContentType)
	{
		if (JC_NULL != jdi_CUtilsStrCaseStr (pcContentType, "text/plain"))
		{
			eMediaType = E_CONFIG_MEDIA_TEXT ;
		}
		else if (JC_NULL != jdi_CUtilsStrCaseStr(pcContentType, "image/"))
		{
			eMediaType = E_CONFIG_MEDIA_IMAGE ;
		}
		else if ( \
		JC_NULL != jdi_CUtilsStrCaseStr (pcContentType, "audio/") || \
		JC_NULL != jdi_CUtilsStrCaseStr (pcContentType, "application/vnd.smaf")|| \
		JC_NULL != jdi_CUtilsStrCaseStr (pcContentType, "application/x-smaf") || \
		JC_NULL != jdi_CUtilsStrCaseStr (pcContentType, "text/x-imelody") || \
		JC_NULL != jdi_CUtilsStrCaseStr (pcContentType, "text/x-wave"))
		{
			eMediaType = E_CONFIG_MEDIA_AUDIO ;
		}
		else if (JC_NULL != jdi_CUtilsStrCaseStr(pcContentType, "video/"))
		{
			eMediaType = E_CONFIG_MEDIA_VIDEO ;
		}
	}
	return eMediaType ;
}

/**
 * @ingroup		jdd_messagingconfig
 * @brief		Retrieves the region to be used for the media type.
 * @param[in]	eMediaType Specifies the media type to be checked
 * @retval		Region Name associated with the media type.
 *
 * Messaging core invokes this function to get the region to be used while 
 * converting a multipart/mixed content to SMIL (rendering).
 *
 * @note
 * Application should allocate the resource returned and caller will free 
 * the resource.
 */
JC_CHAR	* jdd_MsgConfigGetSMILRegion (E_CONFIG_MEDIA_TYPE eMediaType)
{
	JC_CHAR *pmRegionName = JC_NULL ;
	
	JDD_LOG_INFO0 ("jdd_MsgConfigGetSMILRegion: Entry") ;	
	
	if (E_CONFIG_MEDIA_TEXT == eMediaType)
	{
		jdi_CUtilsCharToTchar (JC_NULL, VIEW_REGION_1_NAME, &pmRegionName) ;
	}
	else if (E_CONFIG_MEDIA_IMAGE  == eMediaType|| \
				E_CONFIG_MEDIA_VIDEO == eMediaType)
	{
		jdi_CUtilsCharToTchar(JC_NULL, VIEW_REGION_2_NAME, &pmRegionName) ;
	}	
	
	JDD_LOG_INFO0 ("jdd_MsgConfigGetSMILRegion: Exit") ;
	
	return (pmRegionName) ;
}

/**
 * @ingroup    config_module
 * @brief      Retrieves the icon to be displayed for audio/video control.
 * @param[in]  pstIconInfo Specifies the pointer to MSG_CONFIG_ICON_INFO structure
 * @param[in]  uiConfigArg Points to user defined information
 * @return
 *		- JC_OK on success
 *
 * Messaging core invokes this function to get the icon buffer to be displayed for audio/video controls.
 * This icon will be displayed only when presentation is set to E_PRESENTATION_NONE.
 *
 */
JC_RETCODE jdd_MsgConfigGetDefaultIcon (MSG_CONFIG_ICON_INFO *pstIconInfo, JC_UINT32	uiConfigArg)
{
	if (pstIconInfo)
	{
		pstIconInfo->pstImageInfo = JC_NULL ;
	}
	return JC_OK ;
}
/**
 * @ingroup		jdd_messagingconfig
 * @brief		Initializes the File System Handle
 * @retval		JC_OK	On success
 */
JC_RETCODE jdd_MsgConfigFSHandleInit (void)
{
	JDD_LOG_INFO0 ("jdd_MsgConfigFSHandleInit: Entry") ;
	
	if (JC_NULL == g_vMsgCfgFileSys)
	{
		jdd_FSInitialize (&g_vMsgCfgFileSys);
	}
	
	JDD_LOG_INFO0 ("jdd_MsgConfigFSHandleInit: Exit") ;
	return (JC_OK) ;
}

/**
 * @ingroup		jdd_messagingconfig
 * @brief		Deinitializes the File System Handle
 * @retval		JC_OK On success
 */
JC_RETCODE jdd_MsgConfigFSHandleDeInit (void)
{
	JDD_LOG_INFO0 ("jdd_MsgConfigFSHandleDeInit: Entry") ;

	if (JC_NULL != g_vMsgCfgFileSys )
	{
		jdd_FSDeinitialize (g_vMsgCfgFileSys) ;
		g_vMsgCfgFileSys = JC_NULL ;
	}

	JDD_LOG_INFO0 ("jdd_MsgConfigFSHandleDeInit: Exit") ;

	return (JC_OK) ;
}

/**
 * @ingroup		jdd_messagingconfig
 * @brief		Retrieves File System Handle
 * @retval		File System Handle
 *
 * File system handle can be used by the caller for performing any file
 * stream related operations.
 */
JDD_FSHANDLE jdd_MsgConfigGetFSHandle (void)
{
	JDD_LOG_INFO0 ("jdd_MsgConfigGetFSHandle: Entry") ;
	if (JC_NULL == g_vMsgCfgFileSys)
	{
		jdd_FSInitialize (&g_vMsgCfgFileSys) ;
	}
	
	JDD_LOG_INFO0 ("jdd_MsgConfigGetFSHandle: Exit") ;
	
	return (g_vMsgCfgFileSys) ;
}

JC_INT8	*	jdd_MsgConfigGetContentTypeForExt (JC_CHAR *pFileName)
{
	JC_UINT16	uhCount = 0 ;
	JC_INT8		*pContentType = JC_NULL ;

	while (pFileNameExtension[uhCount] != NULL)
	{
		if (E_TRUE == jdi_CUtilsTcsStrEndsWith (pFileName, pFileNameExtension[uhCount + 1]))
		{
			pContentType = jdi_CUtilsStrDuplicate (JC_NULL, pFileNameExtension[uhCount]) ;
			break;
		}
		uhCount += 2 ;
	}
	return pContentType ;
}
/**
 * @ingroup		jdd_messagingconfig
 * @brief		Retrieves Folder Name (with Path) for keeping temporary 
 *				files created by core (Parsing)
 * @param[in]	eMessagingType Specifies messaging type
 * @retval		Folder Name
 *
 * Messaging core will generate all the temporary files (parsed mime objects) 
 * in the specified folder. Application can provide different paths based on 
 * eMessagingType.
 **/
JC_CHAR	*	jdd_MsgConfigGetTempFolder (E_CONFIG_MSG_TYPE eMessagingType) 
{
	JC_CHAR	*pmFolderName = JC_NULL ;
	
	JDD_LOG_INFO0 ("jdd_MsgConfigGetTempFolder: Entry") ;
		
	if (E_CONFIG_MSG_MMS == eMessagingType)
	{
		jdi_CUtilsCharToTchar (JC_NULL, MMS_TEMP_FILE_PATH, &pmFolderName) ;
	}
	else if (E_CONFIG_MSG_EMAIL == eMessagingType)
	{
		jdi_CUtilsCharToTchar (JC_NULL, EMAIL_TEMP_FILE_PATH, &pmFolderName) ;
	}

	if(pmFolderName)
	{
		if (E_FALSE == jdd_FSIsDirExist (JC_NULL, pmFolderName))
		{
			jdd_FSMakeDir (JC_NULL, pmFolderName) ;
		}
	}

	JDD_LOG_INFO0 ("jdd_MsgConfigGetTempFolder: Exit") ;

	return (pmFolderName) ;
}

/**
 * @ingroup		jdd_messagingconfig
 * @brief		Specifies the default data size (chunk size) that MM1 can 
 *				read from file stream during Decode operation.
 * @retval		JC_UINT32 value specifying Chunk Size
 *
 * Messaging core will do file read of size returned from this function 
 * while decoding in chunks.
 *
 * @note			Applicable to File stream based decoding 
 * (i.e., application passes message data in file).
 */
JC_UINT32 jdd_MsgConfigGetFileChunkSize (void) 
{
	return (DEFAULT_FILE_CHUNK_SIZE) ;
}


/**
 * @ingroup		jdd_messagingconfig
 * @brief		Checks whether DRM content type is supported by application
 * @param[in]	pcContentType Points to the content type to be checked
 * @retval		E_TRUE If content type is supported
 * @retval		E_FALSE If content type is not supported
 *
 * Messaging core invokes this function to validate if the DRM content type
 * is supported by the application.
 */
JC_BOOLEAN jdd_MsgConfigIsDRMTypeSupported (JC_INT8 *pcContentType)
{
	JC_BOOLEAN bIsDRMSupported = E_FALSE ;

	JDD_LOG_INFO0 ("jdd_MsgConfigIsDRMTypeSupported: Entry") ;

	if (0 == jc_strcmp(pcContentType, "application/vnd.oma.drm.message"))
	{
		bIsDRMSupported = E_TRUE ;
	}
	
	JDD_LOG_INFO0 ("jdd_MsgConfigIsDRMTypeSupported: Exit") ;
	return (bIsDRMSupported) ;
}

/**
 * @ingroup		jdd_messagingconfig
 * @brief		Cleans up the temporary files directory.
 * @param[in]	uiConfigArg	Pointer to user defined information
 * @retval		JC_OK On Success
 *
 * Core calls this function to clear the temporary file generated.
 */
JC_RETCODE jdd_MsgConfigCleanUpTempFiles (JC_UINT32 uiConfigArg)
{
	JC_RETCODE rCode = JC_ERR_NOT_IMPLEMENTED ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/
	
	JDD_LOG_INFO0 ("jdd_MsgConfigCleanUpTempFiles: Entry") ;
	/*--- Platform Specific Implementation Start ---*/
	/*
	if ((Platform API succeeds))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = <platformErrValue> ;
		JDD_LOG_INFO1 ("jdd_MsgConfigCleanUpTempFiles: \
					Returns [%d]", rCode) ;
	}
	*/
	/*--- Platform Specific Implementation End ---*/
	
	JDD_LOG_INFO0 ("jdd_MsgConfigCleanUpTempFiles: Exit") ;
	return (rCode) ;
}

const JC_INT8 ** jdd_MsgConfigGetURLSchemes (void)
{
	return (gpURLSchemes) ;
}


#ifdef ENABLE_MSG_HTML_DISPLAY

/**
 * @ingroup		jdd_messagingconfig
 * @brief		Checks whether to display as Rich Text or Plain Text 
 *				(text/html or text/plain)
 * @param[in]	uiConfigArg	Pointer to user defined information
 * @param[out]	pbIsRichText Specifies whether to display Rich Text (E_TRUE)
 * @retval		JC_OK On Success
 *
 * Core calls this function if message is multipart/alternate to check 
 * whether to render the message as Plain Text or Rich Text
 */
JC_RETCODE jdd_MsgConfigGetRichTextInfo (JC_BOOLEAN *pbIsRichText, 
										JC_UINT32 uiConfigArg)
{
	JDD_LOG_INFO0 ("jdd_MsgConfigGetRichTextInfo: Entry") ;
	jdd_AssertOnFail ((JC_NULL != pbIsRichText) , \
		"jdd_MsgConfigGetRichTextInfo: pbIsRichText is NULL") ;
	
	PARAM_INTENTIONALLY_NOT_USED(uiConfigArg) ;
	/*--- Platform Specific Implementation Start ---*/
	*pbIsRichText = E_TRUE  ;	
	/*--- Platform Specific Implementation End ---*/
	
	JDD_LOG_INFO0 ("jdd_MsgConfigGetRichTextInfo: Exit") ;

	return (JC_OK) ;
}

/**
 * @ingroup		jdd_messagingconfig
 * @brief		To get timer value to be used in Display Engine Transport
 *				Listener callback (to set content)
 * @param[in]	uiConfigArg	Pointer to user defined information
 * @retval		Returns the timer value (milliseconds)
 */
JC_UINT32	jdd_MsgConfigGetDEListenerTimer (JC_UINT32 uiConfigArg)
{
	return (DE_LISTENER_TIMER) ;
}

/**
 * @ingroup			jdd_messagingconfig
 * @brief			Gets DE Settings
 * @param[in]		eDESetting	Specifies the settings type for which
 *					request comes.
 * @param[in,out]		pstDESettingInfo	Pointer to settings information. 
 *					On success information will be filled.
 * @param[in]		uiConfigArg	Pointer to user defined information
 * @retval			JC_OK On Success
 *
 * Core calls this function to get the HTML Display Engine Settings.
 */
JC_RETCODE	jdd_MsgConfigGetDESettings (E_CONFIG_DE_SETTING eDESetting, \
										void *pstDESettingInfo, \
										JC_UINT32 uiConfigArg) 
{
	PARAM_INTENTIONALLY_NOT_USED (uiConfigArg) ;
	
	JDD_LOG_INFO0 ("jdd_MsgConfigGetDESettings: Entry") ;
	
	if(E_CONFIG_ARR_RULES == eDESetting)
	{
		ST_ARR_RULES *pstArrRules ;
		pstArrRules = (ST_ARR_RULES *)pstDESettingInfo ;
		
		pstArrRules->eButton = E_ARR_BUTTON_TRUNCATE_TEXT ;
		pstArrRules->eFontSize = E_ARR_FONTSIZE_IGNORE ;
		pstArrRules->eForm = E_ARR_FORM_FIT_ON_EXCEED ;
		pstArrRules->eFrame = E_ARR_FRAME_LINKS ;
		pstArrRules->eGeneral = E_ARR_GENERAL_FIT_ON_EXCEED ;
		pstArrRules->eIFrame = E_ARR_IFRAME_LINK ;
		pstArrRules->eImage = E_ARR_IMAGE_ASPECT_FIT ;
		pstArrRules->eImageMap = E_ARR_IMAGEMAP_SAME_WINDOW ;
		pstArrRules->eIndentation = E_ARR_INDENTATION_MAX_LEVEL ;
		pstArrRules->eLineHeight = E_ARR_LINEHEIGHT_OVERFLOW ;
		pstArrRules->eLink = E_ARR_LINK_IGNORE ;
		pstArrRules->eMargin = E_ARR_MARGIN_SCALE_RATIO ;
#ifdef UNSUPPORTED_ARR_RULES
		pstArrRules->eMouseClick = E_ARR_MOUSECLICK_PAGE_INST ;
		pstArrRules->eMouseOver = E_ARR_MOUSEOVER_ASSOCIATE_ACTION ;
		pstArrRules->eOverflow = E_ARR_OVERFLOW_FULL_BOX ;
		pstArrRules->eFloatText = E_ARR_FLOATTEXT_ALONGSIDE ;
#endif/* UNSUPPORTED_ARR_RULES */
		pstArrRules->eNoframe = E_ARR_NOFRAME_IGNORE ;
		pstArrRules->ePadding = E_ARR_PADDING_SCALE_RATIO ;
		pstArrRules->ePercentage = E_ARR_PERCENTAGE_IGNORE ;
		pstArrRules->ePosition = E_ARR_POSITION_IGNORE ;
		pstArrRules->eTable = E_ARR_TABLE_2D_FIT_SCREEN ;
		pstArrRules->eWrapText = E_ARR_WRAPTEXT_WRAP ;
		
	}
	else if(E_CONFIG_DISPLAY_MODE_INFO == eDESetting)
	{
		ST_DE_DISPLAY_MODE_INFO *pstDisplayModeInfo  ;
		pstDisplayModeInfo = (ST_DE_DISPLAY_MODE_INFO *)pstDESettingInfo ;
		
		pstDisplayModeInfo->bDisplayAHref = E_TRUE ;
		pstDisplayModeInfo->bDisplayDirectImage = E_TRUE ;
		pstDisplayModeInfo->bTextOnly = E_FALSE ;
		pstDisplayModeInfo->eDisplayMode = E_DISPLAY_NORMAL_MODE ; 
	}
	else if(E_CONFIG_OPTION_MAGNIFICATION == eDESetting)
	{
		ST_BE_OPTION_MAGNIFICATION	*pstMagnification ;
		
		pstMagnification = (ST_BE_OPTION_MAGNIFICATION *)pstDESettingInfo ;
		pstMagnification->uhMagnification = 100 ;
	}
	else if(E_CONFIG_SCROLL_MODE == eDESetting)
	{
		ST_DE_OPTION_SCROLL_MODE	*pstScrollMode ;
		pstScrollMode = (ST_DE_OPTION_SCROLL_MODE *)pstDESettingInfo ;
		
		pstScrollMode->eMode = E_SCROLL_LEVEL ;
		pstScrollMode->uiLevel = 10 ;
	}
	
	JDD_LOG_INFO0 ("jdd_MsgConfigGetDESettings: Exit") ;
	
	return (JC_OK) ;	
}

#endif	/* ENABLE_MSG_HTML_DISPLAY */

#ifdef EMAIL_CORE_WITH_INDICATION_HANDLING
/**
 * @ingroup		jdd_messagingconfig
 * @brief		Gets UIDL File Size Limit
 * @param[out]	puiFileSize Pointer to the Size Limit Configured 
 *				on the UIDL File Size
 * @retval		JC_OK On Success
 *
 * Core calls this function to get the configured Size limit for 
 * the UIDL File. If the Configured File Size is Set to zero, no limitation
 * would be set to the File Size.
 **/
 JC_RETCODE jdd_EmailConfigGetUIDLFileSizeLimit (JC_UINT32 *puiFileSize) 
 {
	 JDD_LOG_INFO0 ("jdd_EmailConfigGetUIDLFileSizeLimit: Entry") ;
	 jdd_AssertOnFail ((JC_NULL != puiFileSize), \
		 "jdd_EmailConfigGetUIDLFileSizeLimit: puiFileSize is NULL") ;
	 /*--- Platform Specific Implementation Start ---*/
	 *puiFileSize = 0 ; 
	 /*--- Platform Specific Implementation End ---*/
	 
	 JDD_LOG_INFO0 ("jdd_EmailConfigGetUIDLFileSizeLimit: Exit") ;
	 return (JC_OK) ;
 }
#endif /* EMAIL_CORE_WITH_INDICATION_HANDLING */

/**
 * @ingroup		jdd_messagingconfig
 * @brief		Gets the temporary file path for MMS
 * @retval		the temporary file path for MMS
 *
 * Messaging core will generate all the temporary files (parsed mime objects) 
 * in the specified folder. Application can provide different paths based on 
 * eMessagingType. This is used only in the application to check the existence
 * and creation of the folder.
 **/
JC_INT8 *jdd_CFGGetMMSTempFilePath (void)
{
	return CFG_MMS_CONFIG_PATH ;
}


JC_INT8 *jdd_MsgConfigGetContentTypeFromFileName (JC_INT8 *pcFileName)
{
	JC_INT32 iLen, iExtLen ;	
	JC_INT32 iCount = 0 ;	
	if ( pcFileName )	
	{		
		if ( 0 != ( iLen = jc_strlen( pcFileName ) ) )		
		{			
			while (pFileNameExtension[iCount] != NULL)			
			{				
				iExtLen = jc_strlen ( pFileNameExtension[iCount + 1] ) ;	
				if ( iLen > iExtLen )				
				{					
					if ( 0 == jc_strcasecmp ( pcFileName + ( iLen-iExtLen), pFileNameExtension[iCount + 1] ) )					
					{						
						return (JC_INT8 *)pFileNameExtension[iCount] ;					
					}				}				
					iCount += 2 ;			
				}		
			}	
		}	
	return NULL ;
}

/* End of File */
#endif
