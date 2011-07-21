 /***************************************************************************
 *
 * File Name : jwap_interface.h
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
 * @file		jwap_interface.h
 * @ingroup	jWAP
 * @brief		
 *
 **/
 
#ifndef __JWAP_INTERFACE
#define __JWAP_INTERFACE

#ifdef __cplusplus
extern "C"
{
#endif


 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/
 

 /***************************************************************************
 * Macros
 **************************************************************************/

#ifdef __JATAAYU_APP__

#define	jWAP_MAX_INPUT_SIZE	 (255)


#include <jdi_cutils_time.h>
#include <jdi_browserapp.h>
#include <jdi_bookmark.h>
#include <jwap_mms_common.h>
#include "jwap_httpdownload.h"
#include "jdi_browserengine.h"
#include "jdi_cutils_uriparse.h"
#include "jdi_downloadclient.h"

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif
#include "wgui_touch_screen.h"

/*	This is the identifier for the filesystem used by wap download (i.e, .\\t\\) */
#define	CS_FS_WAP_TCARD_IDENTIFIER	"MMC0"
/*	This is the configuration for the minimum space (in Bytes) to be available always in
	the filesystem for it to work properly */
#define	CS_FS_WAP_THRESHOLD_LIMIT	(20 * 1024)

/***************************************************************************
 * Global variables declarations
 **************************************************************************/
typedef enum _e_jwap_state
{
	E_WAP_STATE_IDLE,
	E_WAP_BEGIN_DISPLAY_RECVD,
	E_WAP_END_DISPLAY_RECVD,
} E_JWAP_STATE ;

typedef enum _e_wap_connectionState
{
	E_WAP_CONN_IDLE,
	E_WAP_CONN_CONNECTING,
	E_WAP_CONN_CONNECTED,
	E_WAP_CONN_DISCONNECTING,
	E_WAP_CONN_DISCONNECTED
} E_WAP_CONNECTION_STATE ;

typedef enum _e_wap_exitKeyPressed
{
	E_WAP_EXIT_KEY_NONE,
	E_WAP_EXIT_KEY_QUIT,
	E_WAP_EXIT_KEY_END
} E_WAP_EXIT_KEY_PRESSED ;

typedef enum _e_wap_LaunchMode
{
	E_WAP_LAUNCH_NORMAL,
	E_WAP_LAUNCH_URL,
	E_WAP_LAUNCH_PUSH_ITEM,
	E_WAP_LAUNCH_PUSH_INBOX
} E_WAP_LAUNCH_MODE ;

typedef enum _e_wap_filename_editor_state
{
	E_WAP_EDIT_PAGE_FILE_NAME,
	E_WAP_EDIT_IMAGE_FILE_NAME
} E_WAP_FILENAME_EDITOR_STATE ;

typedef struct __st_wap_state
{
	E_WAP_CONNECTION_STATE	eTrConnState ;
	E_WAP_EXIT_KEY_PRESSED	eWapExitKeyPressed ;
	E_WAP_FILENAME_EDITOR_STATE	eEditorState ;
} ST_WAP_STATE ;


/* This structure is used to maintain settings information. As this is written 
     to file as whole, no pointers should be used in this structure and its 
     member structures */
typedef struct _jwap_settings_options
{
	JC_UINT16		uiTimeout ;
	JC_BOOLEAN		bEnableGraphics ;
	JC_BOOLEAN		bEnableCache ;
	JC_BOOLEAN		bEnableCookie ;
}ST_WAP_SETTINGS_OPTIONS ;


/* This structure is used to maintain settings information. As this is written 
     to file as whole, no pointers should be used in this structure and its 
     member structures */
typedef struct _jwap_settings
{
	ST_WAP_SETTINGS_OPTIONS	stWAPOptions ;
}ST_WAP_SETTINGS ;

#ifdef __MMI_MULTI_SIM__
typedef enum mmi_wap_simtype_enum
{
	WAP_SIMTYPE_ALWAYSASK,
	WAP_SIMTYPE_SIM1,
	WAP_SIMTYPE_SIM2,
	WAP_SIMTYPE_SIM3,
	WAP_SIMTYPE_SIM4
} MMI_WAP_SIMTYPE_ENUM;
#endif

#ifdef CS_DC_ENABLE
typedef enum _e_download_type
{
	E_APP_DOWNLOAD_CONTENT,
	E_APP_DOWNLOAD_CLIENT
} E_APP_DOWNLOAD_TYPE ;

typedef struct _st_download_content
{
	JC_BOOLEAN			bUserResponse ;
	JC_UINT32			uiResponseCode ;
	JC_UINT32			uiRequestID ;
	JC_UINT32			uiHeaderLength ; /**< Specifies the length of the header */
	JC_UINT32			uiContentLength ; /**< Specifies the length of the content to be downloaded */
	E_HTTP_STATUS		eHTTPStatus ; /**< Specifies the lower layer status */
	JC_BOOLEAN			bIsBinary ; /**< Specifies whether the header information is in binary format or not */
	JC_INT8				*pcContentType ; /**< Specifies the content type */
	JC_UINT8			*pucHeader ; /**<Specifies the header */
	JC_CHAR				*pmURL ; /**< Specifies the URL for which the header indication is sent */
	JC_UINT32			uiCurrentLength ; /**< Specifies the length of the buffer sent in the current indication */
	JC_BOOLEAN			bHasMoreData ; /**< Specifies whether more indication are to be received */
	JC_UINT8			*pucBuffer ; /**< Specifies the fetch indication (buffer) */
	JC_BOOLEAN			bFileCreated ; /**< Specifies whether the file for the Media Object being downloaded has been created or not */
	JC_BOOLEAN			bOverWriteConfirm ; /**< Specifies whether the OverWrite Confirmation message has been displayed or not */
	JC_BOOLEAN			bFileWait ; /**< Specifies the time between the user cancelling the confirmation for overwriting and confirming the new file name */
	JC_BOOLEAN			bStopRequest ; /**< Specifies whether a particular download request has been stopped or not */
} ST_DOWNLOAD_CONTENT;


typedef struct _st_download_client
{
	void						*pvDCCallbackArg ;
	JC_HANDLE					vDCHandle ;
	JC_HANDLE					vSessionHandle ;
	ST_DC_USER_CONFIRMATION		*pstDCUserConfirmation ;
} ST_DOWNLOAD_CLIENT;

#endif // CS_DC_ENABLE

typedef struct _st_authentication_info
{
	JC_CHAR					*pmUserName ;
	JC_CHAR					*pmPassword ;
} ST_AUTHENTICATION_INFO ;


typedef struct _jwap_handle
{
	JC_HANDLE		 		hBECore ;
	JC_UINT32		 		uiWinId ;
	JC_UINT32		 		uiScrollBarWinId ;
	E_JWAP_STATE	 		eJwapState ;
	U8 			 			*pcInputBuff ;
	U8						*pc2ndInputBuff ;
	JC_BOOLEAN		 		bIsDCAvailable ;
	ST_BE_FETCH_REQ 		*pstFetchReq ;
	JC_HANDLE				vBookMarkHandle ;
	ST_WAP_STATE			stWapState ;
	ST_WAP_SETTINGS			stWapSettings ;
	JC_CHAR					amHomePage [JWAP_MAX_URL_LEN] ;
   	JC_HANDLE				pvHistory ;
	JC_CHAR					*pszBookMarkURL ;
	JC_CHAR					*pszBookMarkTitle ;
	JC_CHAR					*pszTitleString ;
	ST_BE_WMLS_PROMPT 		*g_pstWMLPrompt ;
	ST_BE_WMLS_CONFIRM		*g_pstWMLConfirm ;
	JC_BOOLEAN				bIsPriMmiActive ;
	JC_UINT32				uiNumOptItems ;
	JC_CHAR					*pmPushURL ;
	E_WAP_LAUNCH_MODE		eWapLaunchMode ;
	JC_CHAR					amFileExt [JWAP_MAX_FILE_EXT_LEN] ;
	JC_BOOLEAN				bNeedNewPushPopOnIdle;

	//For Download Client
#ifdef 	CS_DC_ENABLE
	ST_DOWNLOAD_CLIENT		*pstDownloadClient ;
	ST_DOWNLOAD_CONTENT		stDownloadContent ;
	E_APP_DOWNLOAD_TYPE		eDownloadType ;
	JC_CHAR					*pmDownloadFileName;
	JC_CHAR					*pmFetchURL;
	JC_INT8					*pcMediaType ;
	ST_AUTHENTICATION_INFO	stAuthenticationInfo ;
	JC_BOOLEAN				bIsDownloadCompleted;
#endif 

#ifdef __MMI_MULTI_SIM__
	MMI_WAP_SIMTYPE_ENUM 	simType;
	MMI_SIM_ID_T 			simid;
	MMI_SIM_ID_T 			ph_edit_simid;
	MMI_SIM_ID_T 			original_simid ; /* This is added for handling push. */
#endif	
	
} JWAP_APP_INFO ;
 
 
extern JWAP_APP_INFO *g_pstJWapAppInfo ;

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

/***************************************************************************
 * All Local Function Definitions
 ***************************************************************************/

/***************************************************************************
 * All Global Function Declarations
 **************************************************************************/
#ifdef MMI_ON_HARDWARE_P

U8* jWap_getInputBuff () ;
U8* jWap_get2ndInputBuff () ;

void 		jWAP_SET_STATE (E_JWAP_STATE X) ;
E_JWAP_STATE jWAP_GET_STATE () ;

#else

U8* jWap_getInputBuff () ;
U8* jWap_get2ndInputBuff () ;
void jWAP_SET_STATE (E_JWAP_STATE X) ;
E_JWAP_STATE jWAP_GET_STATE () ;

#endif /* #ifdef MMI_ON_HARDWARE_P */

INT		jWap_init_engine () ;
void 	jWap_Generic_Key_Handler (U16 keyCode) ;

#if defined(__MMI_TOUCH_SCREEN__)
/* mmi_keypads_enum should be replaced with pen event type */
void 	jWap_Generic_Pen_Handler (mmi_keypads_enum keyCode, mmi_pen_point_struct *stPenCooridinates) ;
#endif
INT 	jWap_send_fetch_request (U16	*pmDest) ;
INT 	jWap_init_priMmi () ;
void 	jWap_init_context () ;
INT 	jWap_config_dispSettings () ;
INT 	jWap_config_connProfile () ;
INT		jWap_refreshPriMmi () ;
INT 	jWap_exitApplication () ;
void 	jWap_back () ;
INT 	jWap_deInit_priMmi () ;
void 	jWap_deInit_context () ;
INT 	jWap_stop () ;
INT 	jWap_next () ;
JC_RETCODE jWap_LaunchFromIdle (E_WAP_LAUNCH_MODE eLaunchMode, JC_UINT8 eSimID, JC_CHAR *pmURL) ;
void jWap_processAppEvent(JC_EVENT_DATA *pstEventData) ;

void 	jWap_options_handler () ;
void 	jWap_initAppSettingsInfo () ;
void 	jWap_readUpdateDataConnInfo () ;
void	jWap_updateDataConnSettings (ST_BE_PROFILE *pstProxySett, \
			ST_GENERIC_DATA_CONN *pstGprsSett, JC_CHAR *pmHomePage) ;

void 	jWap_setUpdateCore (ST_WAP_SETTINGS	*pstWapSettInfo) ;

void* 	jWap_Get_Brw_Handle();
INT		jWap_deInit_engine () ;
INT 	jWap_refresh () ;
INT 	jWap_clearCache () ;
INT 	jWap_clearCookie () ;

JC_BOOLEAN jWap_isBack () ;
JC_BOOLEAN jWap_isNext () ;

JC_RETCODE	jWap_Init_Cookie() ;
JC_RETCODE	jWap_Init_Cache() ;
JC_UINT32 jwap_GetPrimaryWindow();
JC_BOOLEAN jMMS_IsWapConnected (void);

#endif //__JATAAYU_APP__

#ifdef __cplusplus
}
#endif

#endif //__JWAP_INTERFACE

