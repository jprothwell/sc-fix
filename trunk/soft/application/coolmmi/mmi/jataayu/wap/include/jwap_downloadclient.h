/***************************************************************************
 *
 * File Name : jwap_download.h
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
 * @file		jwap_download.h
 * @ingroup	jWAP
 * @brief
 *
 **/
#ifndef __JWAP_DOWNLOAD_CLIENT_H
#define __JWAP_DOWNLOAD_CLIENT_H
 
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/

 /***************************************************************************
 * Macros
 **************************************************************************/
#ifdef JDD_LOG_ENABLED
#define __MODULE_ID__					JWAP_APP
#else
#define __MODULE_ID__					0
#endif
#include "commonscreens.h"

 /***************************************************************************
 * Global variables declarations
 **************************************************************************/

 /***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

 /***************************************************************************
 * All Local Function Definitions
 ***************************************************************************/

 /***************************************************************************
 * All Global Function Definitions
 **************************************************************************/
	
#ifdef __JATAAYU_APP__
#if 0

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
	ST_DOWNLOAD_CONTENT			*pstDownloadContent ;
	ST_DC_USER_CONFIRMATION		*pstDCUserConfirmation ;	
	JC_CHAR						*pmFetchURL ;
	E_APP_DOWNLOAD_TYPE			eDownloadType;
	JC_CHAR						*pmDownloadFileName ;
	JC_CHAR						*pmTempDCFileName ;
	//JC_INT8						*pcFileContentType; 
} ST_DOWNLOAD_CLIENT;
#endif
JC_RETCODE jwap_DCCallback  (void *pvArg, void	*pvDCArg, E_DC_CALLBACK_TYPE eType,void	*vCallbackInfo) ;
JC_RETCODE jwap_appDCFetchRequest (ST_DC_FETCH_REQUEST *pstFetchRequest) ;
JC_RETCODE jwap_appDCUserConfirmation (ST_DC_USER_CONFIRMATION *pstUserConfirmation) ;
JC_RETCODE jwap_appDCNotifyUser (ST_DC_NOTIFY_USER	*pstNotifyUser) ;
JC_RETCODE jwap_appFetchNextUrl (ST_DC_NEXT_URL	*pstNextUrl) ;
JC_RETCODE jwap_appDCAuthHandler (ST_DC_AUTHORIZATION_HANDLER	*pstAuthorizationHandler) ;
JC_RETCODE jwap_appDCMemoryCheck (ST_DC_MEMORY_IND	*pstMemoryInd);
JC_RETCODE jwap_appDCCapabilityCheck ( ST_DC_CAPABILITY_IND *pstCapabilityInd);
JC_RETCODE jwap_appDCHeaderIndication (ST_DC_HEADER_IND *pstHeaderInd) ;
JC_RETCODE jwap_HandleDCHeaderIndication(ST_BE_URL_IND *pstBeURLIn) ;
JC_RETCODE jwap_HandleDCBufferIndication(ST_BE_URL_IND *pstBeURLIn) ;
JC_RETCODE jwap_appDCResponseIndication (ST_DC_FETCH_IND *pstFetchInd);
JC_RETCODE jwap_appDCErrorIndication (ST_DC_FETCH_ERROR_INFO *pstFetchErrorInfo);
JC_RETCODE jwap_appDCInstallationStatus (ST_DC_INSTALLATION_STATUS *pstInstallationStatus);
JC_RETCODE jwap_ConcatenateDownloadContent (ST_BE_URL_IND *pstBeURLInd) ;
JC_RETCODE jwap_BrowserDownloadClientInitialize ();
JC_RETCODE jwap_isOMAInitialized();
JC_RETCODE jwap_CreateDownloadConfirmationPage();
JC_RETCODE jwap_appGetFileNameForDC (JC_INT8 *pcBaseContentType, JC_CHAR **ppmFileName) ;
JC_RETCODE jwap_DCDeInit ();
void jwap_FreeDownloadContent() ;
void jwap_DCProcessUserConfirmation();
void jwap_HandleDCUserconfLSKYes();
void jwap_HandleDCUserconfLSKNo();
void jwap_DCProcessUserConfirmationExit();


#endif //__JATAAYU_APP__
#endif //__JWAP_DOWNLOAD_CLIENT_H
