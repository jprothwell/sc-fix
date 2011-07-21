#define MODULE_NAME "JWAP"
#define FILE_NAME "jwap_dc.c"
/***************************************************************************
*
* File Name : jdd_taskapi.c
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
* @file		jwap_dc.c
* @ingroup	jWAP
* @brief		Describes the Task related API's.
*
**/
#ifdef JATAAYU_SUPPORT
/***************************************************************************
* System Include Files
**************************************************************************/
#include "commonscreens.h"
#include "wgui_categories_popup.h"
#include "fs.h"
#include "errorcode.h"
/***************************************************************************
* User Include Files
**************************************************************************/
#include "jwap_inc.h"

#ifdef CS_DC_ENABLE

#include "jdi_cutils.h"
#include "jwap_interface.h"
#include "jwap_mmi_cb.h"
#include "jwap_httpdownload.h"
#include "jdi_browserengine.h"
#include "jdi_cutils_uriparse.h"
#include "jdi_downloadclient.h"
#include "jwap_downloadclient.h"

/***************************************************************************
* Global
**************************************************************************/
static JC_INT8 *gpacDCBrowserContentType [] = 
{
	"text/vnd.wap.wml; q=0.5",
	"application/xhtml+xml; q=0.99",
	"application/vnd.oma.dd+xml",
	"application/vnd.wap.multipart.related", 
	"application/vnd.wap.multipart.mixed", 
	"application/vnd.wap.mms-message",
	"application/vnd.oma.drm.message",
	"multipart/*",
	"image/bmp",
	"image/gif",
	"image/jpeg",
	"image/vnd.wap.wbmp",
	"image/png",
	"image/jpg",
	"text/css", 
	"text/html",
	"text/plain",
	"application/wml+xml",
	"application/vnd.wap.wml+xml",
	"application/vnd.wap.xhtml+xml; q=0.99",
	"application/vnd.wap.wmlc",
	"application/vnd.wap.wmlscriptc",
	"application/vnd.wap.wtls-ca-certificate", 
	"application/x-x509-ca-cert",
	"application/vnd.wap.sic",
	"application/vnd.wap.slc",
	"application/vnd.wap.coc",
	"text/vnd.wap.si",
	"text/vnd.wap.sl",
	"text/vnd.wap.co",
	"text/x-vCalendar", 
	"text/x-vCard", 
	"application/java-archive", 
	"text/vnd.sun.j2me.app-descriptor", 
	"audio/sp-midi",
	"audio/midi", 
	"audio/wav",
	"audio/x-wav",
	"audio/mpeg",
	"audio/aac",
	"audio/mp3",
	"audio/amr",
	"video/mp4",
	"video/3gpp",
	"video/avi",
	JC_NULL
} ;
	
JC_INT8 **jwap_DCappGetContentType ()
{
	return gpacDCBrowserContentType ;
}

#define APPLICATION_VND_OMA_DRM_MESSAGE "application/vnd.oma.drm.message"

#ifdef __MMI_GB__
	extern void GBSetInputboxLSKFunction(FuncPtr f); 
#endif

JC_RETCODE	jwap_BrowserDownloadClientInitialize ()
{
	JC_RETCODE	rCode = JC_OK ;
	if (JC_NULL != g_pstJWapAppInfo)
	{
		g_pstJWapAppInfo->pstDownloadClient = jdd_MemAlloc(1, sizeof(ST_DOWNLOAD_CLIENT)) ;
		//some website were sending net close after download complete so handle that condition
		g_pstJWapAppInfo->bIsDownloadCompleted = E_FALSE;
		if(JC_NULL != g_pstJWapAppInfo->pstDownloadClient)
		{
			rCode = jdi_DCInitialize (jwap_DCCallback, NULL, JC_NULL, 
						   &g_pstJWapAppInfo->pstDownloadClient->vDCHandle) ;
		}
		jc_memset (&g_pstJWapAppInfo->stDownloadContent, 0, sizeof (ST_DOWNLOAD_CONTENT)) ;
		mmi_trace(12,"jdc_log: DC init rcode[%x] handle[%x]", -rCode, g_pstJWapAppInfo->pstDownloadClient->vDCHandle);
	}		
	return rCode ;
}

JC_RETCODE jwap_DCCallback(void *pvArg, void *pvDCArg,
						   E_DC_CALLBACK_TYPE eType, void *vCallbackInfo)
{
	JC_RETCODE				rCode = JC_OK;	
	g_pstJWapAppInfo->pstDownloadClient->pvDCCallbackArg = pvDCArg ;	
	mmi_trace(12, "jdc_log: jwap_DCCallback start eType[%d]", eType);
	switch (eType)
	{
		case E_DC_FETCH_REQUEST :
				rCode = jwap_appDCFetchRequest ((ST_DC_FETCH_REQUEST *) vCallbackInfo) ;
				break;

		case E_DC_USER_CONFIRMATION :
				rCode = jwap_appDCUserConfirmation ((ST_DC_USER_CONFIRMATION *) vCallbackInfo) ;
				break;

		case E_DC_NOTIFY_USER :
				rCode = jwap_appDCNotifyUser  ((ST_DC_NOTIFY_USER *)vCallbackInfo) ;
				break;

		case E_DC_NEXT_URL :
				rCode = jwap_appFetchNextUrl ((ST_DC_NEXT_URL *) vCallbackInfo) ;
				break ;

		case E_DC_AUTHORIZATION_HANDLER :
				rCode = jwap_appDCAuthHandler ((ST_DC_AUTHORIZATION_HANDLER *) vCallbackInfo) ;
				break ;

		case E_DC_MEMORY_CHECK :
				rCode = jwap_appDCMemoryCheck ((ST_DC_MEMORY_IND *)vCallbackInfo) ;
				break;

		case E_DC_CAPABILITY_CHECK:		
				rCode = jwap_appDCCapabilityCheck ( (ST_DC_CAPABILITY_IND *)vCallbackInfo) ;
				break ;

		case E_DC_HEADER_INDICATION:
				rCode = jwap_appDCHeaderIndication ( (ST_DC_HEADER_IND *)vCallbackInfo) ;
				break ;

		case E_DC_RESPONSE_INDICATION:
				rCode = jwap_appDCResponseIndication ( (ST_DC_FETCH_IND *)vCallbackInfo) ;
				break ;
		case E_DC_ERROR_INDICATION:
				rCode = jwap_appDCErrorIndication ( (ST_DC_FETCH_ERROR_INFO *)vCallbackInfo) ;
				break ;

		case E_DC_OBJECT_INSTALLATION_STATUS :
				rCode = jwap_appDCInstallationStatus ( (ST_DC_INSTALLATION_STATUS *)vCallbackInfo) ;
				break ;
		default:
				rCode = JC_OK ;
				break;
	}
	mmi_trace(12, "jdc_log: jwap_DCCallback rCode [%X]", -rCode);
	return rCode ;
}

JC_RETCODE	jwap_appDCFetchRequest (ST_DC_FETCH_REQUEST *pstFetchRequest)
{
	JC_RETCODE				rCode		= JC_OK ;
	ST_BE_FETCH_REQ			stDownloadReq ;
	ST_BE_POST_DATA			stPostData ;

	mmi_trace(12,"jdc_log: DCFetchRequest begin ");
	jc_memset (&stDownloadReq, 0x00, sizeof (ST_BE_FETCH_REQ)) ;
	if (JC_NULL != pstFetchRequest->pcPostData)
	{
		stPostData.bIsFileData = E_FALSE ;
		stPostData.uiPostLen = pstFetchRequest->uiPostDataLen ;
		stPostData.uPostData.pcPostData = pstFetchRequest->pcPostData ;
		stDownloadReq.pstPostData = &stPostData ;
	}
	stDownloadReq.pmURI = pstFetchRequest->pmUrl ;
	if (JC_NULL != pstFetchRequest->pstAuthHeaders)
	{
		stDownloadReq.pstAuthInfo = (ST_BE_AUTH_INFO * )jdd_MemAlloc (1, sizeof(ST_BE_AUTH_INFO)) ; 
		if (JC_NULL != stDownloadReq.pstAuthInfo)
		{
			if (JC_NULL != pstFetchRequest->pstAuthHeaders->amPassword)
			{
				jc_tcscpy (stDownloadReq.pstAuthInfo->amPassword, 
						   pstFetchRequest->pstAuthHeaders->amPassword) ;
			}
			if (JC_NULL != pstFetchRequest->pstAuthHeaders->amName)
			{
				jc_tcscpy (stDownloadReq.pstAuthInfo->amName, 
						   pstFetchRequest->pstAuthHeaders->amName) ;
			}
			if (JC_NULL != pstFetchRequest->pstAuthHeaders->acRealm)
			{
				jc_strcpy (stDownloadReq.pstAuthInfo->acRealm, 
						   pstFetchRequest->pstAuthHeaders->acRealm) ;
			}
			if (JC_NULL != pstFetchRequest->pstAuthHeaders->acDomain)
			{
				jc_strcpy (stDownloadReq.pstAuthInfo->acDomain, 
						   pstFetchRequest->pstAuthHeaders->acDomain) ;
			}
			if (JC_NULL != pstFetchRequest->pstAuthHeaders->acNonce)
			{
				jc_strcpy (stDownloadReq.pstAuthInfo->acNOnce, 
						   pstFetchRequest->pstAuthHeaders->acNonce ) ;
			}
			if (E_DC_AUTHORIZATION_BASIC == pstFetchRequest->pstAuthHeaders->eAuthType)
			{
				stDownloadReq.pstAuthInfo->eAuthType = E_AUTH_BASIC ;
			}
			else
			{
				stDownloadReq.pstAuthInfo->eAuthType = E_AUTH_DIGEST ;
			}
		}
		else
		{
			rCode = JC_ERR_MEMORY_ALLOCATION ;
		}
	}
	if (JC_OK == rCode)
	{
		stDownloadReq.pstMimeHeaders = pstFetchRequest->pstHeaders ;
		rCode = jdi_BEDownload (g_pstJWapAppInfo->hBECore, &stDownloadReq, \
						&g_pstJWapAppInfo->stDownloadContent.uiRequestID) ;
		jdd_MemFree (stDownloadReq.pstAuthInfo) ;
		stDownloadReq.pstAuthInfo = JC_NULL ;
	}
	mmi_trace(12,"jdc_log: DCFetchRequest end id[%d] rCode[%d]", \
				g_pstJWapAppInfo->stDownloadContent.uiRequestID,
				rCode);
	return rCode ;

}

JC_RETCODE	jwap_appDCUserConfirmation (ST_DC_USER_CONFIRMATION	*pstUserConfirmation)
{
	JC_RETCODE	rCode = JC_OK ;
	g_pstJWapAppInfo->pstDownloadClient->pstDCUserConfirmation = pstUserConfirmation ;
	g_pstJWapAppInfo->eDownloadType = E_APP_DOWNLOAD_CLIENT ;
	rCode = jwap_CreateDownloadConfirmationPage() ;

	if (JC_OK == rCode)
	{
		rCode = JC_ERR_DC_CONTINUE ;
	}
	mmi_trace(12,"jdc_log: UserConfirmation rCode[%x]", -rCode);
	return rCode ;
}

JC_RETCODE	jwap_appDCNotifyUser (ST_DC_NOTIFY_USER		*pstNotifyUser)
{
	JC_RETCODE	rCode = JC_OK ;
	JC_UINT16	uhStrId = STR_ID_DC_UNSUPPORTED_TYPE ;
	U16			uImgID = IMG_GLOBAL_WARNING;
	U16			currid = 0;
	jc_memset (g_pstJWapAppInfo->pcInputBuff, 0, jWAP_MAX_INPUT_SIZE) ;
	
	if (JC_NULL != g_pstJWapAppInfo->pmFetchURL)
	{
		jdd_MemFree(g_pstJWapAppInfo->pmFetchURL) ;
		g_pstJWapAppInfo->pmFetchURL = JC_NULL;
	}
	jwap_FreeDownloadContent () ;

	if(JC_NULL != g_pstJWapAppInfo->stAuthenticationInfo.pmUserName)
	{
		jdd_MemFree(g_pstJWapAppInfo->stAuthenticationInfo.pmUserName);
		g_pstJWapAppInfo->stAuthenticationInfo.pmUserName = JC_NULL;
	}

	if(JC_NULL != g_pstJWapAppInfo->stAuthenticationInfo.pmPassword)
	{
		jdd_MemFree(g_pstJWapAppInfo->stAuthenticationInfo.pmPassword);
		g_pstJWapAppInfo->stAuthenticationInfo.pmPassword = JC_NULL;
	}

	if(g_pstJWapAppInfo->pcMediaType)
	{
		jdd_MemFree(g_pstJWapAppInfo->pcMediaType);
		g_pstJWapAppInfo->pcMediaType = JC_NULL;
	}

	if (JC_NULL != g_pstJWapAppInfo->pmDownloadFileName)
	{
		jdd_MemFree (g_pstJWapAppInfo->pmDownloadFileName) ;
		g_pstJWapAppInfo->pmDownloadFileName = JC_NULL ;
	}

	if (JC_NULL != g_pstJWapAppInfo->pmFetchURL)
	{
		jdd_MemFree (g_pstJWapAppInfo->pmFetchURL) ;
		g_pstJWapAppInfo->pmFetchURL = JC_NULL ;
	}

	//test code  for screen present
	currid = GetActiveScreenId();	
	mmi_trace(1, "jdc_log: DCNotifyUser curr[%d] HMpage[%d] showid[%d] DLID[%d]", \
				currid, SCR_ID_WAP_HOMEPAGE_SCREEN, 
				SCR_ID_WAP_DC_SHOW_DOWNLOADING,
				SCR_ID_WAP_DL_CONFIRMATION);
	//test code end	
	mmi_trace(12,"jdc_log: DCNotifyUser rCode[%x] type[%d]", -pstNotifyUser->rRetCode, \
							pstNotifyUser->eType);	
	if(SCR_ID_WAP_DC_SHOW_DOWNLOADING == currid)
	{
		mmi_trace(12,"jdc_log: DCNotifyUser delete present[%d]", currid);
		DeleteNHistory(1);
		GoBackHistory();		
	}	
	
	if (E_DC_ERROR == pstNotifyUser->eType)
	{
		if (JC_ERR_USER_CANCELLED == pstNotifyUser->rRetCode)
		{
			GoBackHistory();
			uhStrId = STR_ID_DC_DOWNLOAD_CANCELLED;
		}
		else if (JC_ERR_DC_CAPABILITY_ERROR == pstNotifyUser->rRetCode)
		{
			uhStrId = STR_ID_DC_UNSUPPORTED_TYPE ;
		}
		else if (JC_ERR_DC_LOW_MEMORY == pstNotifyUser->rRetCode)
		{
			uhStrId = STR_ID_DC_LOW_MEMORY_TYPE ;
		}
		else if (JC_ERR_DC_UNSUPPORTED_DD_VERSION == pstNotifyUser->rRetCode)
		{
			uhStrId = STR_ID_DC_UNSUPPORTED_TYPE ;
		}
		else if (JC_ERR_DC_INVALID_DD == pstNotifyUser->rRetCode)
		{
			uhStrId = STR_ID_DC_UNSUPPORTED_TYPE ;
		}
		else if (JC_ERR_DC_AUTHENTICATION == pstNotifyUser->rRetCode)
		{
			uhStrId = STR_ID_DC_UNSUPPORTED_TYPE ;
		}
		else if (JC_ERR_DC_UNSUPPORTED_TYPE == pstNotifyUser->rRetCode)
		{
			uhStrId = STR_ID_DC_UNSUPPORTED_TYPE ;
		}
		else if (JC_ERR_DC_HTTP_STATUS == pstNotifyUser->rRetCode)
		{
			uhStrId = STR_ID_WAP_UNKNOWN ;
		}
		else if (JC_ERR_NET_TIMEOUT == pstNotifyUser->rRetCode)
		{
			uhStrId = STR_ERR_NET_TIMEOUT ;
		}
		//some website were sending net close after download complete so handle that condition
		else if((JC_ERR_NET_CONN_ABORT == pstNotifyUser->rRetCode)&&
				(E_TRUE == g_pstJWapAppInfo->bIsDownloadCompleted))
		{
			uhStrId = STR_ID_WAP_HTTPDWN_DWNLOAD_SUCESS;
			uImgID = IMG_GLOBAL_ACTIVATED;
		}
		else
		{
			uhStrId = STR_ID_DC_FAILD_TO_DOWNLOAD;
		}
		DisplayPopup ((PU8)GetString(uhStrId), uImgID, 1, 2000, 1) ;
	}
	else if (E_DC_INFORMATION == pstNotifyUser->eType)
	{
		mmi_trace(1, "jdc_log: DCNotifyUser E_DC_INFORMATION called");
	}	
	return rCode ;
}

JC_RETCODE	jwap_appFetchNextUrl (ST_DC_NEXT_URL		*pstNextUrl)
{
	JC_RETCODE	rCode = JC_OK ;
	if (JC_NULL == g_pstJWapAppInfo->pmFetchURL)
	{
		g_pstJWapAppInfo->pmFetchURL = (JC_CHAR *) jdd_MemAlloc (jc_tcslen (pstNextUrl->pmNextUrl) + 1,
														 sizeof (JC_CHAR)) ;
	}

	if (g_pstJWapAppInfo->pmFetchURL)
	{
		jc_tcscpy (g_pstJWapAppInfo->pmFetchURL, pstNextUrl->pmNextUrl) ;
	}
	else
	{
		rCode = JC_ERR_MEMORY_ALLOCATION ;
	}
	return rCode ;
}

void jwap_HandleAuthorizationDone ()
{
	ST_DC_AUTHORIZATION_HANDLER stAuthHandler ;
	
	DeleteNHistory(1);
	GoBackHistory () ;
	jWap_setConnectingTitle((JC_CHAR*)GetString(STR_ID_WAP_CONNECTING)) ;	

	jc_memset (&stAuthHandler, 0x00, sizeof (ST_DC_AUTHORIZATION_HANDLER)) ;

	stAuthHandler.pmUserName = g_pstJWapAppInfo->stAuthenticationInfo.pmUserName;
	stAuthHandler.pmPassword = g_pstJWapAppInfo->stAuthenticationInfo.pmPassword ;
	stAuthHandler.rRetCode = JC_OK ;
	jdi_DCSetEvent (g_pstJWapAppInfo->pstDownloadClient->vDCHandle, 
					E_DC_AUTHORIZATION_RESPONSE,
					g_pstJWapAppInfo->pstDownloadClient->vSessionHandle, &stAuthHandler) ;
	return ;
}

void jwap_DCMMIAuthPasswordScreen ()
{
	S16 required_input_modes[] = {
								INPUT_MODE_MULTITAP_UPPERCASE_ABC ,
								INPUT_MODE_MULTITAP_LOWERCASE_ABC,
								INPUT_MODE_123,
								-1
								} ;
	U8* guiBuffer ; 
	
	mmi_trace (1, "jdc_log: jwap_DCShowPwdScreen entry") ;
	
	EntryNewScreen (SCR_ID_WAP_DC_AUTH_PASSWORD, NULL, jwap_DCMMIAuthPasswordScreen, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_DC_AUTH_PASSWORD) ;
	ShowCategory5Screen_ext(
		STR_ID_WAP_PASSWORD, 0,
		STR_GLOBAL_OK, IMG_GLOBAL_OK,
		STR_GLOBAL_CANCEL, IMG_GLOBAL_BACK,
		INPUT_TYPE_ALPHANUMERIC_PASSWORD,
		(U8*)g_pstJWapAppInfo->stAuthenticationInfo.pmPassword, MAX_AUTH_PASSWORD_SIZE,
		guiBuffer,0, required_input_modes, NULL);
	
#ifdef __MMI_GB__
	GBSetInputboxLSKFunction (jwap_HandleAuthorizationDone);
#endif
	SetLeftSoftkeyFunction (jwap_HandleAuthorizationDone, KEY_EVENT_UP) ;
	SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP) ;
}

void jwap_DCHandleAuthCancel ()
{
	jdd_MemFree(g_pstJWapAppInfo->stAuthenticationInfo.pmUserName);
	g_pstJWapAppInfo->stAuthenticationInfo.pmUserName = JC_NULL;
	jdd_MemFree(g_pstJWapAppInfo->stAuthenticationInfo.pmPassword);
	g_pstJWapAppInfo->stAuthenticationInfo.pmPassword = JC_NULL;	
	GoBackHistory () ;	
}

void jwap_DCMMIAuthUserNameScreen ()
{
	S16 required_input_modes[] = {
								INPUT_MODE_MULTITAP_UPPERCASE_ABC ,
								INPUT_MODE_MULTITAP_LOWERCASE_ABC,
								INPUT_MODE_123,
								-1
								} ;
	U8* guiBuffer ; 
	mmi_trace (1, "jdc_log: jwap_HandleAuthorization entry") ;

	DeleteScreenIfPresent(SCR_ID_WAP_DL_CONFIRMATION);
	EntryNewScreen (SCR_ID_WAP_DC_AUTH_USERNAME, NULL, jwap_DCMMIAuthUserNameScreen, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_DC_AUTH_USERNAME) ;

	ShowCategory5Screen_ext(STR_ID_WAP_USER_NAME,
		0,
		STR_GLOBAL_OK, IMG_GLOBAL_OK,
		STR_GLOBAL_CANCEL, IMG_GLOBAL_BACK,
		INPUT_TYPE_ALPHANUMERIC_UPPERCASE|INPUT_TYPE_ALPHANUMERIC_LOWERCASE|INPUT_TYPE_NUMERIC,
		(U8*)g_pstJWapAppInfo->stAuthenticationInfo.pmUserName, MAX_AUTH_NAME_SIZE,
		guiBuffer,0, required_input_modes, NULL);

#ifdef __MMI_GB__
	GBSetInputboxLSKFunction (jwap_DCMMIAuthPasswordScreen);
#endif
	SetLeftSoftkeyFunction (jwap_DCMMIAuthPasswordScreen, KEY_EVENT_UP) ;
	SetCategory5RightSoftkeyFunction (jwap_DCHandleAuthCancel, KEY_EVENT_UP) ;	
}

void jwap_HandleAuthorization()
{
	g_pstJWapAppInfo->eDownloadType = E_APP_DOWNLOAD_CLIENT ;
	jc_memset (&g_pstJWapAppInfo->stAuthenticationInfo, 0, sizeof (ST_WAP_SETTINGS)) ;

	mmi_trace (1, "jdc_log: HandleAuthorization entry") ;
	if(JC_NULL != g_pstJWapAppInfo->stAuthenticationInfo.pmUserName)
	{
		jdd_MemFree(g_pstJWapAppInfo->stAuthenticationInfo.pmUserName);
		g_pstJWapAppInfo->stAuthenticationInfo.pmUserName = JC_NULL;
	}

	g_pstJWapAppInfo->stAuthenticationInfo.pmUserName = (JC_CHAR*)jdd_MemAlloc(
							MAX_AUTH_NAME_SIZE+1, sizeof(JC_CHAR));
	
	if(JC_NULL != g_pstJWapAppInfo->stAuthenticationInfo.pmPassword)
	{
		jdd_MemFree(g_pstJWapAppInfo->stAuthenticationInfo.pmPassword);
		g_pstJWapAppInfo->stAuthenticationInfo.pmPassword = JC_NULL;	
	}
	g_pstJWapAppInfo->stAuthenticationInfo.pmPassword = (JC_CHAR*)jdd_MemAlloc(
							MAX_AUTH_PASSWORD_SIZE+1, sizeof(JC_CHAR));
	mmi_trace (1, "jdc_log: HandleAuthorization entry screen") ;
	if(g_pstJWapAppInfo->stAuthenticationInfo.pmUserName &&
		g_pstJWapAppInfo->stAuthenticationInfo.pmPassword)
	{
		mmi_trace (1, "jdc_log: AuthHandler entry") ;
		jwap_DCMMIAuthUserNameScreen();
	}
	mmi_trace (1, "jdc_log: HandleAuthorization end") ;
	return ;
}

JC_RETCODE	jwap_appDCAuthHandler (ST_DC_AUTHORIZATION_HANDLER *pstAuthorizationHandler) 
{
	JC_RETCODE rCode = JC_OK;
	DisplayPopupCallBack((PU8)GetString(STR_ID_WAP_AUTHORIZATION), IMG_GLOBAL_INFO, 1, \
				2000, 1, jwap_HandleAuthorization);
	if (JC_OK == rCode)
	{
		rCode = JC_ERR_DC_CONTINUE ;
	}
	return rCode ;
}

JC_RETCODE jwap_HandleDCHeaderIndication (ST_BE_URL_IND *pstBeURLInd)
{	
	JC_RETCODE rCode = JC_OK;
	ST_DOWNLOAD_CONTENT		stDownloadContent ;

	jwap_BrowserDownloadClientInitialize ();	
	if(JC_NULL == g_pstJWapAppInfo)
	{
		rCode = JC_ERR_INVALID_PARAMETER;
		return rCode;
	}

	mmi_trace(12, "jdc_log: DCHeaderIndication appid[%d] coreid[%d] stopreq[%d]", \
					g_pstJWapAppInfo->stDownloadContent.uiRequestID, 
					pstBeURLInd->uiRequestID,
					g_pstJWapAppInfo->stDownloadContent.bStopRequest);
	
	stDownloadContent = g_pstJWapAppInfo->stDownloadContent ;
	if (0 == stDownloadContent.uiRequestID)
	{
		if (E_FALSE == stDownloadContent.bStopRequest)
		{
			stDownloadContent.uiRequestID = pstBeURLInd->uiRequestID ;
			stDownloadContent.bIsBinary = pstBeURLInd->uBeInd.stBeHeaderInd.bIsBinary ;
			stDownloadContent.eHTTPStatus = pstBeURLInd->uBeInd.stBeHeaderInd.eHTTPStatus ;
			stDownloadContent.uiContentLength = pstBeURLInd->uBeInd.stBeHeaderInd.uiContentLength ;
			stDownloadContent.uiHeaderLength = pstBeURLInd->uBeInd.stBeHeaderInd.uiHeaderLength ;
			stDownloadContent.bHasMoreData = E_TRUE ;
			if (JC_NULL != pstBeURLInd->uBeInd.stBeHeaderInd.pmURL)
			{
				if (JC_NULL != stDownloadContent.pmURL)
				{
					jdd_MemFree (stDownloadContent.pmURL) ;
				}
				stDownloadContent.pmURL = (JC_CHAR *)jdd_MemAlloc (jc_tcslen (pstBeURLInd->uBeInd.stBeHeaderInd.pmURL) 
																+ 1, sizeof (JC_CHAR)) ;
				if (JC_NULL != stDownloadContent.pmURL)
				{
					jc_tcscpy (stDownloadContent.pmURL, pstBeURLInd->uBeInd.stBeHeaderInd.pmURL) ;
				}
				else
				{
					rCode = JC_ERR_MEMORY_ALLOCATION ;
				}
			}
			if (JC_NULL != pstBeURLInd->uBeInd.stBeHeaderInd.pucHeader && JC_OK == rCode)
			{
				if (JC_NULL != stDownloadContent.pucHeader)
				{
					jdd_MemFree (stDownloadContent.pucHeader) ;
				}
				stDownloadContent.pucHeader = (JC_UINT8*) jdd_MemAlloc (jc_strlen ((JC_INT8 *) \
						pstBeURLInd->uBeInd.stBeHeaderInd.pucHeader)  + 1, sizeof (JC_UINT8)) ;
				
				if (JC_NULL != stDownloadContent.pucHeader)
				{
					jc_memcpy (stDownloadContent.pucHeader, pstBeURLInd->uBeInd.stBeHeaderInd.pucHeader,
								jc_strlen ((JC_INT8 *) pstBeURLInd->uBeInd.stBeHeaderInd.pucHeader)) ;
				}
				else
				{
					rCode = JC_ERR_MEMORY_ALLOCATION ;
				}
			}
			if (JC_NULL != pstBeURLInd->uBeInd.stBeHeaderInd.pcContentType && JC_OK == rCode)
			{
				if (JC_NULL != stDownloadContent.pcContentType)
				{
					jdd_MemFree (stDownloadContent.pcContentType) ;
				}
				stDownloadContent.pcContentType = (JC_INT8*)jdd_MemAlloc (jc_strlen (pstBeURLInd->uBeInd.stBeHeaderInd.pcContentType) 
																+ 1, sizeof (JC_INT8)) ;
				if (JC_NULL != stDownloadContent.pcContentType)
				{
					jc_memcpy (stDownloadContent.pcContentType, pstBeURLInd->uBeInd.stBeHeaderInd.pcContentType,
								jc_strlen (pstBeURLInd->uBeInd.stBeHeaderInd.pcContentType)) ;
				}
				else
				{
					rCode = JC_ERR_MEMORY_ALLOCATION ;
				}
			}
			g_pstJWapAppInfo->stDownloadContent = stDownloadContent ;
		}
		else
		{
			g_pstJWapAppInfo->stDownloadContent.uiRequestID = pstBeURLInd->uiRequestID ;
		}
	}
	return rCode;
}

JC_RETCODE jwap_HandleDCBufferIndication (ST_BE_URL_IND *pstBeURLInd)
{
	JC_RETCODE rCode = JC_OK;
	ST_DOWNLOAD_CONTENT		stDownloadContent ;	

	stDownloadContent = g_pstJWapAppInfo->stDownloadContent ;

	mmi_trace(12, "jdc_log: DCBufferIndication appID[%x] coreID[%d]", \
					g_pstJWapAppInfo->stDownloadContent.uiRequestID, 
					pstBeURLInd->uiRequestID);
	
	if (stDownloadContent.uiRequestID == pstBeURLInd->uiRequestID)
	{
		g_pstJWapAppInfo->stDownloadContent.bHasMoreData = pstBeURLInd->uBeInd.stBeFetchInd.bHasMoreData ;
		
		rCode = jwap_ConcatenateDownloadContent (pstBeURLInd) ;
		mmi_trace(12, "jdc_log: ConcatenateDownloadContent rCode[%x] hasmoredata[%d]", \
					-rCode, g_pstJWapAppInfo->stDownloadContent.bHasMoreData);
		if ((JC_OK == rCode) && (E_FALSE == g_pstJWapAppInfo->stDownloadContent.bHasMoreData))
		{
			ST_DC_DD_INFO	stDDInfo ;
			mmi_trace(12, "jdc_log: HandleDCBufferIndication  in if" );
			stDDInfo.pcContentType = g_pstJWapAppInfo->stDownloadContent.pcContentType ;
			stDDInfo.pmUrl = g_pstJWapAppInfo->stDownloadContent.pmURL ;
			stDDInfo.pucBuffer = g_pstJWapAppInfo->stDownloadContent.pucBuffer ;
			stDDInfo.uiBufferLen = g_pstJWapAppInfo->stDownloadContent.uiCurrentLength ;
			//test code
			{
				JC_INT8	pcTemp [64] = {0, } ;					
				jdi_CUtilsStrTcsNCpy (stDDInfo.pmUrl, 64, pcTemp) ;
				mmi_trace(12,"jdc_log: ST_DC_DD_INFO pcContentType[%s] pmurl[%s] \
						uiBufferLen[%d]", \
						stDDInfo.pcContentType, pcTemp, \
						stDDInfo.uiBufferLen);
			}
			//end
			rCode = jdi_DCProcessContent (g_pstJWapAppInfo->pstDownloadClient->vDCHandle, 
										  &stDDInfo,
										  &g_pstJWapAppInfo->pstDownloadClient->vSessionHandle) ;
			mmi_trace(12, "jdc_log: jdi_DCProcessContent handle [%x] rCode [%x]", \
						g_pstJWapAppInfo->pstDownloadClient->vSessionHandle, -rCode);
			if ((JC_ERR_DC_INVALID_DD == rCode) || (JC_ERR_DC_UNSUPPORTED_TYPE == rCode))
			{
				mmi_trace(12, "jdc_log: HandleDCBufferIndication  in if error" );
			}
			jwap_FreeDownloadContent () ;			
		}			
	}
	return rCode;
}

JC_RETCODE jwap_ConcatenateDownloadContent (ST_BE_URL_IND		*pstBeURLInd)
{
	JC_UINT8		*pucBuffer ;
	JC_UINT8		*pucTemp ;
	JC_RETCODE		rCode = JC_OK ;

	mmi_trace(12,"jdc_log: ConcatenateDownloadContent buff[%x] core buff[%x]", \
				g_pstJWapAppInfo->stDownloadContent.pucBuffer,
				pstBeURLInd->uBeInd.stBeFetchInd.pucBuffer);
	
	if (JC_NULL != g_pstJWapAppInfo->stDownloadContent.pucBuffer)
	{
		pucBuffer = (JC_UINT8 *) jdd_MemAlloc (g_pstJWapAppInfo->stDownloadContent.uiCurrentLength + 1, 
											   sizeof (JC_UINT8)) ;
		if (JC_NULL != pucBuffer)
		{
			jc_memcpy (pucBuffer, g_pstJWapAppInfo->stDownloadContent.pucBuffer,
					   g_pstJWapAppInfo->stDownloadContent.uiCurrentLength) ;
			jdd_MemFree (g_pstJWapAppInfo->stDownloadContent.pucBuffer) ;
			g_pstJWapAppInfo->stDownloadContent.pucBuffer = (JC_UINT8 *) jdd_MemAlloc (
									g_pstJWapAppInfo->stDownloadContent.uiCurrentLength +
									pstBeURLInd->uBeInd.stBeFetchInd.uiCurrentLength + 1,
															sizeof (JC_UINT8)) ;
			if (JC_NULL == g_pstJWapAppInfo->stDownloadContent.pucBuffer)
			{
				rCode = JC_ERR_MEMORY_ALLOCATION ;
			}
			else
			{
				pucTemp = g_pstJWapAppInfo->stDownloadContent.pucBuffer ;
				jc_memcpy (g_pstJWapAppInfo->stDownloadContent.pucBuffer, pucBuffer,
						   g_pstJWapAppInfo->stDownloadContent.uiCurrentLength) ;
				g_pstJWapAppInfo->stDownloadContent.pucBuffer += g_pstJWapAppInfo->stDownloadContent.uiCurrentLength ;
				jc_memcpy (g_pstJWapAppInfo->stDownloadContent.pucBuffer, \
						   pstBeURLInd->uBeInd.stBeFetchInd.pucBuffer,
						   pstBeURLInd->uBeInd.stBeFetchInd.uiCurrentLength) ;
				g_pstJWapAppInfo->stDownloadContent.pucBuffer = pucTemp ;
				g_pstJWapAppInfo->stDownloadContent.uiCurrentLength += pstBeURLInd->uBeInd.stBeFetchInd.uiCurrentLength ;
			}
			jdd_MemFree (pucBuffer) ;
		}
		else
		{
			rCode = JC_ERR_MEMORY_ALLOCATION ;
		}
	}
	else if (JC_NULL != pstBeURLInd->uBeInd.stBeFetchInd.pucBuffer)
	{
		mmi_trace(12,"jdc_log: ConcatenateDownloadContent else case currlen[%d] sentlen[%d]", \
				g_pstJWapAppInfo->stDownloadContent.uiCurrentLength,
				pstBeURLInd->uBeInd.stBeFetchInd.uiCurrentLength);
		g_pstJWapAppInfo->stDownloadContent.pucBuffer = (JC_UINT8 *) jdd_MemAlloc (
									pstBeURLInd->uBeInd.stBeFetchInd.uiCurrentLength + 1,
									sizeof (JC_UINT8)) ;
		if (JC_NULL != g_pstJWapAppInfo->stDownloadContent.pucBuffer)
		{
			jc_memcpy (g_pstJWapAppInfo->stDownloadContent.pucBuffer, pstBeURLInd->uBeInd.stBeFetchInd.pucBuffer,
					   pstBeURLInd->uBeInd.stBeFetchInd.uiCurrentLength) ;
			g_pstJWapAppInfo->stDownloadContent.uiCurrentLength += pstBeURLInd->uBeInd.stBeFetchInd.uiCurrentLength ;
		}
		else
		{
			rCode = JC_ERR_MEMORY_ALLOCATION ;
		}
	}	
	return rCode ;
}

JC_RETCODE jwap_appDCMemoryCheck (ST_DC_MEMORY_IND	*pstMemoryInd)
{
	JC_RETCODE	rCode = JC_OK ;
	extern BOOL g_hasMountedTFlashFS;

	mmi_trace(12,"jdd_log: DCMemoryCheck size[%d] card status[%d]", pstMemoryInd->iSize, g_hasMountedTFlashFS); 
	if (FALSE == g_hasMountedTFlashFS)
	{		
		rCode = JC_ERR_FILE_SYS_INIT;
	}
	else
	{
		/* if the target filesystem does not have enough memory, then show error */
		FS_INFO			stFSInfo = {0, } ;
		JC_UINT32		uiRet = FS_GetFSInfo (CS_FS_WAP_TCARD_IDENTIFIER, &stFSInfo) ;
		
		if ((uiRet == ERR_SUCCESS) &&	(0 < pstMemoryInd->iSize))
		{
			uiRet = (UINT32)stFSInfo.iTotalSize - (UINT32)stFSInfo.iUsedSize ;
			mmi_trace (1, "jdc_log: MemoryCheck Space[%d] and Avail Space[%d] in [%s]",
						pstMemoryInd->iSize, uiRet, CS_FS_WAP_TCARD_IDENTIFIER) ;
			
			if (uiRet < (CS_FS_WAP_THRESHOLD_LIMIT + pstMemoryInd->iSize))
			{
				mmi_trace (1, "jdc_log: return system full");
				rCode = JC_ERR_DC_LOW_MEMORY;				
			}
		}
	}
	return rCode;
}

JC_RETCODE jwap_appDCCapabilityCheck (ST_DC_CAPABILITY_IND *pstCapabilityInd)
{
	JC_RETCODE			rCode = JC_OK ;
	JC_BOOLEAN			bLoop = E_TRUE ;
	JC_INT8				**ppcContentType ;
	JC_UINT32			uiIndex ;
	JC_INT8				*pcContentType ;

	ppcContentType = jwap_DCappGetContentType() ;	
	uiIndex = 0 ;	
	
	while (JC_NULL != ppcContentType[uiIndex] && E_TRUE == bLoop)
	{
		pcContentType = ppcContentType[uiIndex] ;
		if (0 == jc_strncmp (pstCapabilityInd->pcMediaType, pcContentType,
							 jc_strlen (pstCapabilityInd->pcMediaType)))
		{
			rCode = JC_OK ;
			bLoop = E_FALSE ;
		}
		uiIndex++ ;
	}
	if (E_FALSE == bLoop)
	{	
		rCode = JC_OK ; 
	}
	else
	{
		rCode = JC_ERR_DC_CAPABILITY_ERROR ;
	}
	return rCode ;
}

void jwap_FreeDownloadContent ()
{
	if (JC_NULL != g_pstJWapAppInfo->stDownloadContent.pucBuffer)
	{
		jdd_MemFree (g_pstJWapAppInfo->stDownloadContent.pucBuffer) ;		
	}
	if (JC_NULL != g_pstJWapAppInfo->stDownloadContent.pucHeader)
	{
		jdd_MemFree (g_pstJWapAppInfo->stDownloadContent.pucHeader) ;		
	}
	if (JC_NULL != g_pstJWapAppInfo->stDownloadContent.pmURL)
	{
		jdd_MemFree (g_pstJWapAppInfo->stDownloadContent.pmURL) ;		
	}
	if (JC_NULL != g_pstJWapAppInfo->stDownloadContent.pcContentType)
	{
		jdd_MemFree (g_pstJWapAppInfo->stDownloadContent.pcContentType) ;
	}
	jc_memset (&g_pstJWapAppInfo->stDownloadContent, 0x00, sizeof (ST_DOWNLOAD_CONTENT)) ;
	mmi_trace(12,"jdc_log: FreeDownloadContent [%x]", g_pstJWapAppInfo->stDownloadContent);
	return ;
}

JC_RETCODE jwap_isOMAInitialized()
{
	if(JC_NULL != g_pstJWapAppInfo->pstDownloadClient)
		return E_TRUE;
	else
		return E_FALSE;
}

JC_RETCODE jwap_CreateDownloadConfirmationPage()
{	
	jwap_DCProcessUserConfirmation();
	return (0);
}

JC_RETCODE jwap_PrepareDCDownloadableFile ()
{
	JC_CHAR 		*pmFolder = JC_NULL ;
	JC_CHAR 		*pmFileName = JC_NULL, *pmTempFileName = JC_NULL;
	JC_RETCODE		rCode ;
	JC_INT32		iFileSize ;
	JC_INT8			*pcContentType = JC_NULL;
	
	if(0 == jc_wstrlen (jWap_getInputBuff ())||*jWap_getInputBuff () == '.')
	{
		rCode = JC_ERR_FILE_NOT_FOUND ;
	}
	else
	{		
		rCode = jdi_CUtilsCharToTchar (JC_NULL, CS_WAP_DOWLOAD_PATH, &pmFolder) ;
		if (JC_OK == rCode)
		{
			if (E_FALSE == jdd_FSIsDirExist (JC_NULL, pmFolder))
			{
				rCode = jdd_FSMakeDir (JC_NULL, pmFolder) ;
			}
		}
		
		if (JC_OK == rCode)
		{
			iFileSize = jc_wstrlen (pmFolder) + jc_strlen (CFG_PATH_SEPARATOR) + jc_wstrlen ((JC_CHAR *)g_pstJWapAppInfo->pcInputBuff) ;			
		
			pmFileName = jdd_MemAlloc (sizeof (JC_CHAR), iFileSize + 1) ;
			if (pmFileName)
			{
				rCode = JC_OK ;				
				jc_wstrcpy (pmFileName, pmFolder) ;
				jdi_CUtilsTcsStrCat (CFG_PATH_SEPARATOR, pmFileName) ;
				jc_wstrcat (pmFileName, (JC_CHAR*)g_pstJWapAppInfo->pcInputBuff) ;			

				if(JC_NULL != g_pstJWapAppInfo->pmDownloadFileName)
				{					
					mmi_trace(1,"jdc_log: extn is [%s]", g_pstJWapAppInfo->pcMediaType);
					
					jdd_MiscGetExtForContentType (g_pstJWapAppInfo->pcMediaType, \
													(const JC_INT8**)&pcContentType) ;

					mmi_trace(1,"jdc_log: otput extn is [%s]", pcContentType);				
				}
			}
			else
			{
				rCode = JC_ERR_MEMORY_ALLOCATION ;
			}
		}
		jdd_MemFree (pmFolder) ;
	}

	if (!rCode)
	{
		iFileSize = (jc_wstrlen (pmFileName) + \
						(jc_strlen(pcContentType)*sizeof(JC_CHAR))) ;
		pmTempFileName = jdd_MemAlloc (sizeof (JC_CHAR), iFileSize + 1) ;

		if(pmTempFileName)
		{
			jc_tcscpy(pmTempFileName, pmFileName);
		}
			
		if (pcContentType && (jdi_CUtilsTcsStrEndsWith (pmTempFileName,pcContentType) == E_FALSE) )
		{
			jdi_CUtilsTcsStrCat(pcContentType, pmTempFileName);
		}		
		else
		{
			mmi_trace(12,"jdc_log: extension already existing ");	

		}
		{
			JC_INT8 cNameext[64]= {0,};
			jdi_CUtilsStrTcsNCpy (pmTempFileName, 64, cNameext) ;
			mmi_trace(12,"jdc_log: UserconfLSKYes checkfile[%s]", cNameext);
		}
		
		if (jdd_FSIsFileExist (JC_NULL, pmTempFileName))
		{
			rCode = JC_ERR_FILE_DIR_ALREADY_EXIST ;
		}
		else
		{			
			if(g_pstJWapAppInfo->pmDownloadFileName)
			{
				jdd_MemFree(g_pstJWapAppInfo->pmDownloadFileName);
				g_pstJWapAppInfo->pmDownloadFileName = jdi_CUtilsTcsDuplicate(JC_NULL, pmFileName);
			}			
		}
		jdd_MemFree(pcContentType);
		jdd_MemFree(pmTempFileName);
		jdd_MemFree(pmFileName);
	}
	
	return (rCode) ;
}

void jwap_DCHandleProgressCancel()
{
	JC_RETCODE rCode = JC_ERR_GENERAL_FAILURE;
	g_pstJWapAppInfo->stDownloadContent.bStopRequest = E_TRUE ;
	if (0 != g_pstJWapAppInfo->stDownloadContent.uiRequestID)
	{
		rCode = jdi_BEStopDownload (g_pstJWapAppInfo->hBECore,
									g_pstJWapAppInfo->stDownloadContent.uiRequestID) ;
	}
	mmi_trace(12, "jdc_log: HandleProgressCancel rCode[%x] id[%d]", \
				-rCode, g_pstJWapAppInfo->stDownloadContent.uiRequestID);

	if(g_pstJWapAppInfo->pmDownloadFileName)
	{
		jdd_MemFree(g_pstJWapAppInfo->pmDownloadFileName);
		g_pstJWapAppInfo->pmDownloadFileName = JC_NULL;
	}
	
	if(g_pstJWapAppInfo->pcMediaType)
	{
		jdd_MemFree(g_pstJWapAppInfo->pcMediaType);
		g_pstJWapAppInfo->pcMediaType = JC_NULL;
	}	
	DeleteNHistory(1);
	GoBackHistory();
	mmi_show_in_progress(STR_GLOBAL_PLEASE_WAIT, IMG_GLOBAL_PROGRESS);
}


void jwap_DCShowDownloading(void)
{	
	U8* guiBuffer = JC_NULL;
	EntryNewScreen (SCR_ID_WAP_DC_SHOW_DOWNLOADING, 0, jwap_DCShowDownloading, 0) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_DC_SHOW_DOWNLOADING);
	SetParentHandler (0);

	ShowCategory119Screen (	0, 0,
							STR_GLOBAL_CANCEL, 0, 
							(U8*)get_string (STR_ID_WAP_HTTPDWN_DWNLD_PROGRESS),
							IMG_GLOBAL_INFO, guiBuffer);

	SetRightSoftkeyFunction (jwap_DCHandleProgressCancel, KEY_EVENT_UP ) ;
}

void jwap_DCDownloadStart()
{
	JC_RETCODE		rCode = JC_OK;
	ST_DC_USER_RESP		stUserResp ;

	DeleteScreenIfPresent(SCR_ID_WAP_DL_CONFIRMATION);
	
	rCode = jwap_PrepareDCDownloadableFile () ;
	mmi_trace(12, "jdc_log: DCDownloadStart rCode[%x]",	-rCode);
	
	if (JC_OK == rCode)
	{		
		stUserResp.eResponse = E_DC_PROCEED ;		
		jdi_DCSetEvent (g_pstJWapAppInfo->pstDownloadClient->vDCHandle, E_DC_USER_INDICATION, \
							g_pstJWapAppInfo->pstDownloadClient->pvDCCallbackArg, &stUserResp) ;
		
		jwap_DCShowDownloading();		
	}
	else if (JC_ERR_FILE_DIR_ALREADY_EXIST == rCode)
	{
		GoBackToHistory (SCR_ID_WAP_INPUT_BOX_BK) ;
		jWap_showErrorPopup (rCode) ;
	}
	else
	{
		GoBackToHistory (SCR_ID_WAP_INPUT_BOX_BK) ;
		jWap_showErrorPopup (rCode) ;
	}
}

void jwap_DCOptioninputBox_highlightHanlder (S32 index)
{
	switch(index)
	{
		case 0:
			SetLeftSoftkeyFunction (jwap_DCDownloadStart, KEY_EVENT_UP ) ;
			break;
		case 1:
			SetLeftSoftkeyFunction (EntryInputMethodScreen, KEY_EVENT_UP ) ;
			break ;
	}	
}

void jwap_DCOptioninputBoxLskHandler ()
{
	U16 item_texts[] = {STR_GLOBAL_DONE, \
						//STR_GLOBAL_CANCEL,
						STR_GLOBAL_INPUT_METHOD} ;
	
	U16 item_icons[] = {IMG_GLOBAL_L1, \
						IMG_GLOBAL_L2, \
						//IMG_GLOBAL_L3
						} ;
	U8* guiBuffer = JC_NULL;
	
	EntryNewScreen (SCR_ID_WIDGET_INPUTBOX_OPTION, 0, 0, 0) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_INPUT_BOX_BK);
	SetParentHandler (SCR_ID_WAP_INPUT_BOX_BK);
	
	RegisterHighlightHandler (jwap_DCOptioninputBox_highlightHanlder);

	ShowCategory52Screen (STR_GLOBAL_OPTIONS, 0, 
		STR_GLOBAL_OK, 0,
		STR_GLOBAL_BACK, 0, 
		2, item_texts, item_icons,
		0, 0, 0, guiBuffer);
	
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP ) ;
}

void jwap_DCPrepareFilenameFromUser ()
{
	U8*			guiBuffer = JC_NULL;
	JC_INT8		*pcFileName ;
	JC_CHAR		*pmFileName = (JC_CHAR *)jWap_getInputBuff() ;
	JC_UINT32	uiFileNameLength ;
	JC_INT32	iIndexOfDotInOrginalName ;
	JC_INT32	iIndexOfDotInFinalName ;

	uiFileNameLength = jc_tcslen (pmFileName) ;
	jdi_CUtilsTcharToChar (JC_NULL, pmFileName, &pcFileName) ;
	if (pcFileName)
	{
		//mmi_trace (1, "jdc_log: DCPrepareFilenameFromUser File Name[%s] len[%d]",
			//				pcFileName, uiFileNameLength) ;
		mmi_trace (1, "jdc_log: DCPrepareFilenameFromUser [%d]", uiFileNameLength) ;	
		if (uiFileNameLength >= JDOWNLOAD_INPUTBOX_FILENAME_MAX)
		{
			iIndexOfDotInOrginalName = jdi_CUtilsStrLastIndexOf (pcFileName, '.', uiFileNameLength) ;
			if (-1 != iIndexOfDotInOrginalName)
			{
				iIndexOfDotInFinalName = (JDOWNLOAD_INPUTBOX_FILENAME_MAX - 1) -
											(uiFileNameLength - iIndexOfDotInOrginalName) ;
				if (0 < iIndexOfDotInFinalName)
				{
					jc_tcscpy (pmFileName + iIndexOfDotInFinalName,
						pmFileName + iIndexOfDotInOrginalName) ;
				}
			}
			jc_memset (pmFileName + JDOWNLOAD_INPUTBOX_FILENAME_MAX - 1, 0, \
						uiFileNameLength - JDOWNLOAD_INPUTBOX_FILENAME_MAX + 1) ;
		}
		else
		{
			/* The file name length is well within the platform limit. */
		}
	}
	
	EntryNewScreen (SCR_ID_WAP_INPUT_BOX_BK, JC_NULL, \
					jwap_DCPrepareFilenameFromUser, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_INPUT_BOX_BK);		
	SetParentHandler (SCR_ID_WAP_DL_CONFIRMATION);		

	ShowCategory5Screen (STR_ID_WAP_DC_FILENAME, 0,
						STR_GLOBAL_OPTIONS, 0,
						STR_GLOBAL_BACK, 0,
						INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
						(U8*)pmFileName, JDOWNLOAD_INPUTBOX_FILENAME_MAX, 
						guiBuffer);

	if (pcFileName)
	{
		jdi_CUtilsTcharToCharMem (pmFileName, uiFileNameLength + 1, pcFileName) ;
		mmi_trace (1, "jdc_log: DCPrepareFilenameFromUser FName[%s]", pcFileName) ;
		jdd_MemFree (pcFileName) ;
	}
#ifdef __MMI_GB__
		GBSetInputboxLSKFunction (jwap_DCOptioninputBoxLskHandler);
#endif
	
	SetLeftSoftkeyFunction (jwap_DCOptioninputBoxLskHandler, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction (jwap_HandleDCUserconfLSKNo, KEY_EVENT_UP);
	RegisterInputMethodScreenCloseFunction (jwap_HandleDCUserconfLSKNo);
}	


extern JC_RETCODE jWap_IsFileDownloadPossible (JC_BOOLEAN bOnlyFeasibilityChack);


void jWap_DCGetFileNameFromUrl (JC_CHAR *pmDestFileName)
{
	JC_INT8*	pcFileName = NULL , *pcTmp = NULL, *pcTmp2 = NULL ;
	JC_INT32	aLength ;
	
	mmi_trace (0,"jWap_DC %d",g_pstJWapAppInfo->pstDownloadClient->pstDCUserConfirmation);
	if(g_pstJWapAppInfo->pstDownloadClient->pstDCUserConfirmation)
		mmi_trace (0,"jWap_DC name %d",g_pstJWapAppInfo->pstDownloadClient->pstDCUserConfirmation->pstDD);

	if(g_pstJWapAppInfo->pstDownloadClient->pstDCUserConfirmation&&
		g_pstJWapAppInfo->pstDownloadClient->pstDCUserConfirmation->pstDD&&
		g_pstJWapAppInfo->pstDownloadClient->pstDCUserConfirmation->pstDD->pmName)
	{
		mmi_trace (0, "jdc_log: jWap_getFileNameFromUrl %s", g_pstJWapAppInfo->pstDownloadClient->pstDCUserConfirmation->pstDD->pmName) ;
		
		jdi_CUtilsTcharToChar(NULL, \
			g_pstJWapAppInfo->pstDownloadClient->pstDCUserConfirmation->pstDD->pmName, &pcFileName) ;
		if(pcFileName)
			mmi_trace (0, "jdc_log: pcFileName %s", pcFileName) ;
		else
			mmi_trace (0, "jdc_log: pcFileName is NULL") ;
	}
	if(NULL != pcFileName)
	{
		pcTmp = pcFileName ;
		aLength = jc_strlen(pcFileName) ;
		pcFileName = pcFileName + (aLength-1) ;
		while((*pcFileName != '\0') && (aLength != 0)  )
		{
			if(*pcFileName == '/')
			{
				pcFileName++ ;
				break ;
			}
			pcFileName-- ;
			aLength-- ;
		}

		pcTmp2 = pcFileName ;
				
		while (*pcTmp2)
		{
			if ('?' == *pcTmp2)  //Search for Query String
			{
				*pcTmp2 = '\0' ;
				break ;
			}
			pcTmp2++ ;
		}

		mmi_trace (1, "jdc_log: jWap_getFileNameFromUrl: %s", pcFileName) ;
		jdi_CUtilsCharToTcharNMem  (pcFileName, jc_strlen (pcFileName), \
			JDOWNLOAD_INPUTBOX_FILENAME_MAX, pmDestFileName) ;
		jdi_CUtilsTcsURIUnescape (pmDestFileName) ;
		jdd_MemFree (pcTmp) ;
	}
}


void jwap_HandleDCUserconfLSKYes()
{
	JC_RETCODE			rCode = JC_OK ;
	jc_memset (g_pstJWapAppInfo->pcInputBuff, 0, jWAP_MAX_INPUT_SIZE) ;
	
    //rCode = jWap_IsFileDownloadPossible (E_FALSE) ;	
	{
		JC_INT8 cName[64]= {0,};
		jdi_CUtilsStrTcsNCpy (g_pstJWapAppInfo->pmDownloadFileName, 64, cName) ;
		mmi_trace(12,"jdc_log: UserconfLSKYes cName[%s]", cName);
	}
	if (JC_OK == rCode)
	{
		if (!g_pstJWapAppInfo->pmDownloadFileName)
		{
			mmi_trace(1, "jdc_log: press yes DownloadPossible if");
			// cannot the us this as data is already freed
			//jWap_DCGetFileNameFromUrl ((JC_CHAR*)jWap_getInputBuff()) ;
		}
		else
		{
			mmi_trace(1, "jdc_log: press yes DownloadPossible else");
			jc_wstrncpy ((JC_CHAR*)jWap_getInputBuff(), \
				g_pstJWapAppInfo->pmDownloadFileName, \
				JDOWNLOAD_INPUTBOX_FILENAME_MAX) ;
		}
		mmi_trace(1, "jdc_log: jwap_DCPrepareFilenameFromUser start");
		jwap_DCPrepareFilenameFromUser () ;
		mmi_trace(1, "jdc_log: jwap_DCPrepareFilenameFromUser end");
	}	
}

void jwap_HandleDCUserconfLSKNo()
{
	ST_DC_USER_RESP		stUserResp ;
	stUserResp.eResponse = E_DC_CANCELED;
	jc_memset (g_pstJWapAppInfo->pcInputBuff, 0, jWAP_MAX_INPUT_SIZE) ;
	DeleteScreenIfPresent (SCR_ID_WAP_DL_CONFIRMATION) ;
	GoBackHistory();
	mmi_show_in_progress(STR_GLOBAL_PLEASE_WAIT, IMG_GLOBAL_PROGRESS);	
	jdi_DCSetEvent (g_pstJWapAppInfo->pstDownloadClient->vDCHandle, 
				E_DC_USER_INDICATION,
				g_pstJWapAppInfo->pstDownloadClient->pvDCCallbackArg, 
				&stUserResp) ;
	
	if (JC_NULL != g_pstJWapAppInfo->pmDownloadFileName)
	{
		mmi_trace(12,"jdc_log: pmDownloadFileName free");
		jdd_MemFree (g_pstJWapAppInfo->pmDownloadFileName) ;
		g_pstJWapAppInfo->pmDownloadFileName = JC_NULL ;
	}
	if(g_pstJWapAppInfo->pcMediaType)
	{
		jdd_MemFree(g_pstJWapAppInfo->pcMediaType);
		g_pstJWapAppInfo->pcMediaType = JC_NULL;
	}
	jwap_FreeDownloadContent () ;
}

JC_INT8			*pszDCScreenBuffer = NULL ;

void jwap_DCProcessUserConfirmation()
{
	
	U8 				*guiBuffer;	
	JC_CHAR			*pmContentType	= JC_NULL;
	JC_CHAR			*pmContentSize	= JC_NULL;
	ST_DC_USER_CONFIRMATION	*pstDCUserConfirmation = JC_NULL;
	JC_CHAR			cDCTemPBuff[128] = {0, } ;

	JC_INT32 Len = 0;
		
	pstDCUserConfirmation = g_pstJWapAppInfo->pstDownloadClient->pstDCUserConfirmation ;

	if (pstDCUserConfirmation->pstDD->iSize)
	{		
		if (pstDCUserConfirmation->pstDD->iSize < 1024)
		{			
			pmContentSize = jdi_CUtilsItoT (0, pstDCUserConfirmation->pstDD->iSize) ;
			jc_wstrncat ( cDCTemPBuff, pmContentSize, 10) ;
			jdi_CUtilsTcsStrCat ("Bytes", cDCTemPBuff) ;
		}
		else
		{			
			pmContentSize = jdi_CUtilsItoT (0, (pstDCUserConfirmation->pstDD->iSize/1024));
			jc_wstrncat ( cDCTemPBuff, pmContentSize, 10) ;	
			jdi_CUtilsTcsStrCat ("KB", cDCTemPBuff) ;
		}
		jdd_MemFree(pmContentSize);
	}	


	Len = jc_wstrlen((JC_CHAR*)GetString (STR_ID_WAP_DL_CONTENT_SIZE))+ 2 /* for ": " */ 
	       + jc_wstrlen(cDCTemPBuff)+ 1 /* for '\n' */;
	
	       
	Len += jc_wstrlen((JC_CHAR*)GetString (STR_ID_WAP_DL_CONTENT_TYPE))+ 2 /* for ": '"*/;
	       
	if(pstDCUserConfirmation->pstDD->pstTypeList->pcMediaType)
	{
		Len += jc_strlen(pstDCUserConfirmation->pstDD->pstTypeList->pcMediaType);
	}
	
	Len += 1 /* for '\n' */ ;
	
	Len += jc_wstrlen((JC_CHAR*)GetString (STR_ID_WAP_DC_FILENAME))+ 2 /* for ': ' */ ;
	
	
	if(pstDCUserConfirmation->pstDD->pmName)
	{
		Len += jc_wstrlen(pstDCUserConfirmation->pstDD->pmName);
	}
	Len += 1 /* for '\n' */ ;
	Len += jc_wstrlen((JC_CHAR*)GetString (STR_ID_WAP_DC_FILE_DESCRIPTION))+ 2 /* for ': ' */; 
	
	
	if(pstDCUserConfirmation->pstDD->pmDescription)
	{
		Len += jc_wstrlen(pstDCUserConfirmation->pstDD->pmDescription);
	}

	Len += 1 /* for '\n' */ ;
	
	mmi_trace (0, "jdc_log: GetUserConfirmation begin %d", Len);

	pszDCScreenBuffer = jdd_MemAlloc(Len+1,sizeof(JC_CHAR));

	
	
	jc_wstrcpy (pszDCScreenBuffer, GetString (STR_ID_WAP_DL_CONTENT_SIZE)) ;
	jdi_CUtilsTcsStrCat (": ", (JC_CHAR*) pszDCScreenBuffer) ;

	if(JC_NULL == g_pstJWapAppInfo)
		mmi_trace (0, "jdc_log: GetUserConfirmation begin2");

	if(JC_NULL == g_pstJWapAppInfo->pstDownloadClient)
		mmi_trace (0, "jdc_log: GetUserConfirmation begin3");
	
	if(JC_NULL == g_pstJWapAppInfo->pstDownloadClient->pstDCUserConfirmation)
		mmi_trace (0, "jdc_log: GetUserConfirmation begin4");
	
	

	jc_wstrcat((JC_CHAR*)pszDCScreenBuffer, cDCTemPBuff);
	
	jdi_CUtilsTcsStrCat ("\n",  (JC_CHAR*)pszDCScreenBuffer) ;
		
	jc_wstrcat ( (JC_CHAR*)pszDCScreenBuffer,  (JC_CHAR*)GetString (STR_ID_WAP_DL_CONTENT_TYPE)) ;
	jdi_CUtilsTcsStrCat (": ", (JC_CHAR*) pszDCScreenBuffer) ;

	
	if(pstDCUserConfirmation->pstDD->pstTypeList->pcMediaType)
	{
		jdi_CUtilsCharToTchar(JC_NULL, \
				pstDCUserConfirmation->pstDD->pstTypeList->pcMediaType, \
				&pmContentType);	
		jc_wstrcat((JC_CHAR*)pszDCScreenBuffer, (JC_CHAR*)pmContentType);
		jdi_CUtilsTcsStrCat ("\n",  (JC_CHAR*)pszDCScreenBuffer) ;
	}

	if(g_pstJWapAppInfo->pcMediaType)
	{
		jdd_MemFree(g_pstJWapAppInfo->pcMediaType);
		g_pstJWapAppInfo->pcMediaType = JC_NULL;
	}
	g_pstJWapAppInfo->pcMediaType = jdi_CUtilsStrDuplicate(JC_NULL, \
				pstDCUserConfirmation->pstDD->pstTypeList->pcMediaType);

	mmi_trace(1, "jdc_log: content type", g_pstJWapAppInfo->pcMediaType);
		
	jdd_MemFree(pmContentType);
	jc_wstrcat ( (JC_CHAR*)pszDCScreenBuffer,  (JC_CHAR*)GetString (STR_ID_WAP_DC_FILENAME)) ;
	jdi_CUtilsTcsStrCat (": ", (JC_CHAR*) pszDCScreenBuffer) ;
	
	if(pstDCUserConfirmation->pstDD->pmName)
	{
		jc_wstrcat ((JC_CHAR*) pszDCScreenBuffer , pstDCUserConfirmation->pstDD->pmName) ;
		
	}
	jdi_CUtilsTcsStrCat ("\n",  (JC_CHAR*)pszDCScreenBuffer) ;

	if(g_pstJWapAppInfo->pmDownloadFileName)
	{
		jdd_MemFree(g_pstJWapAppInfo->pmDownloadFileName);
		g_pstJWapAppInfo->pmDownloadFileName = JC_NULL;
	}
	if(pstDCUserConfirmation->pstDD->pmName)
	{
		mmi_trace(12, "jdc_log: pmDownloadFileName size%d", jc_tcslen(pstDCUserConfirmation->pstDD->pmName));
		g_pstJWapAppInfo->pmDownloadFileName = jdi_CUtilsTcsDuplicate(JC_NULL , \
						pstDCUserConfirmation->pstDD->pmName) ;
	}
	
	jc_wstrcat ( (JC_CHAR*)pszDCScreenBuffer,  (JC_CHAR*)GetString (STR_ID_WAP_DC_FILE_DESCRIPTION)) ;
	jdi_CUtilsTcsStrCat (": ", (JC_CHAR*) pszDCScreenBuffer) ;
	if(pstDCUserConfirmation->pstDD->pmDescription)
	{
		jc_wstrcat ((JC_CHAR*) pszDCScreenBuffer , pstDCUserConfirmation->pstDD->pmDescription) ;
	}
	
	jdi_CUtilsTcsStrCat ("\n",  (JC_CHAR*)pszDCScreenBuffer) ;
	
	mmi_trace (0, "jdc_log: GetUserConfirmation begin6");
	EntryNewScreen (SCR_ID_WAP_DL_CONFIRMATION, jwap_DCProcessUserConfirmationExit, jwap_DCProcessUserConfirmation, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_DL_CONFIRMATION) ; 
	SetParentHandler (SCR_ID_WAP_HOMEPAGE_SCREEN) ;

	ShowCategory78Screen(STR_ID_WAP_HTTPDWN_DWNLOAD_CONFIRM,0,
						STR_GLOBAL_YES, IMG_GLOBAL_YES,
						STR_GLOBAL_NO, IMG_GLOBAL_NO,
						NULL,
						(U8*) pszDCScreenBuffer,
						jc_wstrlen(pszDCScreenBuffer),
						NULL);

	SetLeftSoftkeyFunction (jwap_HandleDCUserconfLSKYes, KEY_EVENT_UP);
	SetRightSoftkeyFunction (jwap_HandleDCUserconfLSKNo, KEY_EVENT_UP);
	
	mmi_trace (0, "jdc_log: GetUserConfirmation. end");
}

void jwap_DCProcessUserConfirmationExit()
{
	mmi_trace (1, "jdc_log: jwap_DCProcessUserConfirmationExit");

	if(pszDCScreenBuffer)
		jdd_MemFree(pszDCScreenBuffer);

	pszDCScreenBuffer = NULL;
}


JC_BOOLEAN jwap_appIsDRMContent (JC_INT8 *pcContentType)
{
	JC_INT8		*pcType ;
	JC_BOOLEAN	bIsDRM = E_FALSE;

	pcType = pcContentType ;
	if ((E_TRUE == jdi_CUtilsStrStartsWith (pcContentType, APPLICATION_VND_OMA_DRM_MESSAGE)))
	{
		bIsDRM = E_TRUE ;
	}

	mmi_trace(12, "jdc_log: jwap_appIsDRMContent return %d", bIsDRM);
	return bIsDRM ;
}

JC_RETCODE	jwap_appDCHeaderIndication (ST_DC_HEADER_IND *pstHeaderInd)
{
	JC_RETCODE				rCode = JC_OK ;
	ST_DOWNLOAD_CONTENT		stDownloadContent ;
	stDownloadContent = g_pstJWapAppInfo->stDownloadContent ;
	stDownloadContent.uiRequestID = pstHeaderInd->uiRequestID ;
	stDownloadContent.bIsBinary = pstHeaderInd->bIsBinary ;
	stDownloadContent.eHTTPStatus = pstHeaderInd->eHttpStatus ;
	stDownloadContent.uiContentLength = pstHeaderInd->uiContentLength ;
	stDownloadContent.uiHeaderLength = pstHeaderInd->uiHeaderLength ;

	mmi_trace(12, "jdc_log: DCHeaderIndication begin");
	if (JC_NULL != pstHeaderInd->pcContentType && JC_OK == rCode)
	{
		mmi_trace(12, "jdc_log: DCHeaderIndication contenttype[%s]", pstHeaderInd->pcContentType);

		if (E_TRUE == jwap_appIsDRMContent (pstHeaderInd->pcContentType))
		{
			rCode = JC_ERR_DC_UNSUPPORTED_DD_VERSION;
			goto EXIT_LABEL;
		}
		if (JC_NULL != stDownloadContent.pcContentType)
		{
			jdd_MemFree (stDownloadContent.pcContentType) ;
		}
		stDownloadContent.pcContentType = (JC_INT8*)jdd_MemAlloc (jc_strlen (pstHeaderInd->pcContentType) 
														+ 1, sizeof (JC_INT8)) ;
		if (JC_NULL != stDownloadContent.pcContentType)
		{
			jc_memcpy (stDownloadContent.pcContentType, pstHeaderInd->pcContentType,
						jc_strlen (pstHeaderInd->pcContentType)) ;
		}
		else
		{
			rCode = JC_ERR_MEMORY_ALLOCATION ;
		}
	}
	if (JC_NULL != pstHeaderInd->pmUrl)
	{
		if (JC_NULL != stDownloadContent.pmURL)
		{
			jdd_MemFree (stDownloadContent.pmURL) ;
		}
		stDownloadContent.pmURL = (JC_CHAR *)jdd_MemAlloc (jc_tcslen (pstHeaderInd->pmUrl) 
														+ 1, sizeof (JC_CHAR)) ;
		if (JC_NULL != stDownloadContent.pmURL)
		{
			jc_tcscpy (stDownloadContent.pmURL, pstHeaderInd->pmUrl) ;
		}
		else
		{
			rCode = JC_ERR_MEMORY_ALLOCATION ;
		}
	}
	if (JC_NULL != pstHeaderInd->pucHeader && JC_OK == rCode)
	{
		if (JC_NULL != stDownloadContent.pucHeader)
		{
			jdd_MemFree (stDownloadContent.pucHeader) ;
		}
		stDownloadContent.pucHeader = (JC_UINT8*) jdd_MemAlloc (jc_strlen ((JC_INT8 *) pstHeaderInd->pucHeader)  + 1, sizeof (JC_UINT8)) ;
		if (JC_NULL != stDownloadContent.pucHeader)
		{
			jc_memcpy (stDownloadContent.pucHeader, pstHeaderInd->pucHeader,
						jc_strlen ((JC_INT8 *) pstHeaderInd->pucHeader)) ;
		}
		else
		{
			rCode = JC_ERR_MEMORY_ALLOCATION ;
		}
	}	

EXIT_LABEL:	
	if (JC_NULL != stDownloadContent.pucBuffer)
	{
		jdd_MemFree (stDownloadContent.pucBuffer) ;
		stDownloadContent.pucBuffer = JC_NULL ;
	}
	stDownloadContent.uiCurrentLength = 0 ;
	if(JC_OK == rCode)
	{
		g_pstJWapAppInfo->eDownloadType= E_APP_DOWNLOAD_CLIENT ;
		g_pstJWapAppInfo->stDownloadContent = stDownloadContent ;
	}
	mmi_trace(12, "jdc_log: DCHeaderIndication rCode[%d]", rCode);
	return rCode ;
}

JC_RETCODE	jwap_appDCResponseIndication (ST_DC_FETCH_IND *pstFetchInd)
{
	ST_DOWNLOAD_CONTENT		stDownloadContent ;
	ST_BE_PAGE_PROGRESS		stPageProgress ;
	JC_UINT32				uiRemainLen ;
	JC_INT8					*pcFile ;
	JC_UINT8				*pucTempBuff ;
	JC_UINT8				*pucTemp ;
	JC_RETCODE				rCode ;
	JC_BOOLEAN				bFileExists ;

	rCode = JC_OK ;
	pcFile = JC_NULL ;
	stDownloadContent = g_pstJWapAppInfo->stDownloadContent ;
	bFileExists = E_TRUE ;

	mmi_trace(12,"jdc_log: ResponseIndication hasmoredata[%d]", \
						pstFetchInd->bHasMoreData);
	
	if (stDownloadContent.uiRequestID == pstFetchInd->uiRequestID)
	{
		stDownloadContent.bHasMoreData = pstFetchInd->bHasMoreData;		
		if (E_TRUE == stDownloadContent.bOverWriteConfirm)
		{
			if (JC_NULL != stDownloadContent.pucBuffer)
			{
				pucTempBuff = (JC_UINT8 *) jdd_MemAlloc (stDownloadContent.uiCurrentLength + 1,
														 sizeof (JC_UINT8)) ;
				if (JC_NULL == pucTempBuff)
				{
					rCode = JC_ERR_MEMORY_ALLOCATION ;
				}
				else
				{
					jc_memcpy (pucTempBuff, stDownloadContent.pucBuffer, \
								stDownloadContent.uiCurrentLength) ;
					jdd_MemFree (stDownloadContent.pucBuffer) ;
					
					stDownloadContent.pucBuffer = (JC_UINT8 *) jdd_MemAlloc (stDownloadContent.uiCurrentLength +
																			pstFetchInd->uiCurrentLength + 1,
																			sizeof (JC_UINT8)) ;
					pucTemp = stDownloadContent.pucBuffer ;
					if (JC_NULL == stDownloadContent.pucBuffer)
					{
						rCode = JC_ERR_MEMORY_ALLOCATION ;
					}
					else
					{
						jc_memcpy (stDownloadContent.pucBuffer, pucTempBuff, \
									stDownloadContent.uiCurrentLength) ;
						stDownloadContent.pucBuffer += stDownloadContent.uiCurrentLength ;
						jc_memcpy (stDownloadContent.pucBuffer, pstFetchInd->pucBuffer, pstFetchInd->uiCurrentLength) ;
						stDownloadContent.uiCurrentLength += pstFetchInd->uiCurrentLength;
						stDownloadContent.pucBuffer = pucTemp ;
					}
					jdd_MemFree (pucTempBuff) ;
				}
			}
		}
		else
		{	
			mmi_trace(12,"jdc_log: ResponseIndication contenttype[%s] filestatus[%d]", \
						stDownloadContent.pcContentType,
						stDownloadContent.bFileCreated);			
			if(E_FALSE == stDownloadContent.bFileCreated)
			{
				rCode = jwap_appGetFileNameForDC (stDownloadContent.pcContentType, \
										&g_pstJWapAppInfo->pmDownloadFileName) ;

				if(rCode == JC_OK)
				{
					bFileExists = jdd_FSIsFileExist(JC_NULL, g_pstJWapAppInfo->pmDownloadFileName);
				}
			}
			mmi_trace(12,"jdc_log: rCode[%d] isfile[%d] isexist[%d]", \
						rCode, stDownloadContent.bFileCreated,
						bFileExists);
			if ((JC_OK == rCode) && 
				((E_TRUE == stDownloadContent.bFileCreated) || 
				(E_FALSE == bFileExists)))
			{
				uiRemainLen = pstFetchInd->uiCurrentLength ;
				mmi_trace(12,"jdc_log: ResponseIndication uiRemainLen[%d]", uiRemainLen);
				pucTempBuff = pstFetchInd->pucBuffer ;
				stDownloadContent.uiCurrentLength += pstFetchInd->uiCurrentLength ;
				stDownloadContent.bFileCreated = E_TRUE;
				if (JC_NULL != pstFetchInd->pucBuffer && 0 != uiRemainLen)
				{						
					rCode = jWap_SaveToFile (g_pstJWapAppInfo->pmDownloadFileName,
												uiRemainLen, pucTempBuff) ;
					mmi_trace(12,"jdc_log: jWap_SaveToFile rCode[%d]", rCode);
				}
			}
		}

		g_pstJWapAppInfo->stDownloadContent = stDownloadContent ;
		jc_memset (&stPageProgress, 0x00, sizeof (ST_BE_PAGE_PROGRESS)) ;
		stPageProgress.uiBytesRecvd = stDownloadContent.uiCurrentLength ;

		mmi_trace(12,"jdc_log: rcvdata[%d] totallen[%d] len[%d]", \
				stPageProgress.uiBytesRecvd,
				stDownloadContent.uiContentLength,
				stDownloadContent.uiCurrentLength);
		//some website were sending net close after download complete so handle that condition
		if(stPageProgress.uiBytesRecvd == stDownloadContent.uiContentLength)
		{
			g_pstJWapAppInfo->bIsDownloadCompleted = E_TRUE;
		}
		if(SCR_ID_WAP_DC_SHOW_DOWNLOADING == GetActiveScreenId())
		{
			JC_CHAR	Title[32] = {0,};
			JC_INT8 title1[16] = {0,};
			JC_INT8 title2[16] = {0,};
			JC_INT8 Log[32] = {0,};

			if(0 >= stDownloadContent.uiContentLength )
			{
				jdi_CUtilsTcharToCharMem((JC_CHAR*)GetString(STR_ID_WAP_UNKNOWN_SIZE), 16, title2);		
			}
			else if (1024 > stDownloadContent.uiContentLength)
			{

				jc_sprintf(title2, "%d B", \
							stDownloadContent.uiContentLength);

			}
			else if(1024 <= stDownloadContent.uiContentLength )
			{
		    
				jc_sprintf(title2, "%d KB", stDownloadContent.uiContentLength/1024);

			}

			if (1024 > stPageProgress.uiBytesRecvd)
			{
				jc_sprintf(title1, "%d B", \
							stPageProgress.uiBytesRecvd);
			}
			else if(1024 <= stPageProgress.uiBytesRecvd )
			{
				jc_sprintf(title1, "%d KB", stPageProgress.uiBytesRecvd/1024);

			}
			csd_wsprintf(Title,"%s/%s", \
							title1, title2);


			jdi_CUtilsTcharToCharMem(Title, 32, Log);		
			mmi_trace(1, "jdc_log: title[%s]", Log);
			ChangeTitleString ((U8*)Title) ;
			draw_title();
		}
	}

	if (E_FALSE == stDownloadContent.bHasMoreData)
	{		
		if (E_TRUE == stDownloadContent.bOverWriteConfirm)
		{
			rCode = JC_ERR_DC_CONTINUE ;
		}		
	}
	mmi_trace(12,"jdc_log: ResponseIndication rCode[%d]", rCode);
	return rCode ;
}

JC_RETCODE jwap_appGetFileNameForDC (JC_INT8 *pcBaseContentType, JC_CHAR **ppmFileName)
{
	JC_RETCODE		rCode ;
	JC_INT8			*pcFileName ;
	JC_INT8			*pcFile ;
	JC_INT8			*pcTempExt, *pcTemp ;
	JC_INT8			*pcContentType ;
	JC_INT8			*pcBase, *pcTempBase ;
	JC_INT32		iIndex ;
	JC_UINT32		uiIndex ;
	JC_CHAR			*pmFileName ;
	JC_CHAR			*pmTemp ;
	JC_BOOLEAN		bIsBreak ;
	JC_BOOLEAN		bIsAlreadyPresent ;

	rCode = JC_OK ;
	iIndex = 0 ;
	uiIndex = 0 ;
	pcTempExt = JC_NULL ;
	pcTemp = JC_NULL ;
	pmTemp = JC_NULL ;
	pcTempBase = JC_NULL ;
	bIsBreak = E_FALSE ;
	bIsAlreadyPresent = E_FALSE ;
	pcContentType = JC_NULL ;
	pcFile = JC_NULL;

	mmi_trace(12, "jdc_log: GetFileNameForDC begin base contype[%s]", \
				pcBaseContentType);
	if (JC_NULL != pcBaseContentType)
	{
		jdd_MiscGetExtForContentType (pcBaseContentType, (const JC_INT8**)&pcContentType) ;
		iIndex = jdi_CUtilsStrIndexOf (pcBaseContentType, ';', 0) ;
		mmi_trace(12, "jdc_log: StrIndexOf ;[%d]", iIndex);
		if (0 < iIndex)
		{
			pcTempBase = jdi_CUtilsStrCopyIndex (JC_NULL, pcBaseContentType, 0, (iIndex - 1)) ;
			iIndex = 0 ;
			pcBase = pcTempBase ;
		}
		else
		{
			pcBase = pcBaseContentType ;
		}
		iIndex = jdi_CUtilsStrLastIndexOf (pcBase, '/', jc_strlen (pcBase) - 1) ;
		mmi_trace(12, "jdc_log: StrIndexOf /[%d]", iIndex);
		if (0 < iIndex)
		{
			pcTempExt = pcBase + iIndex + 1 ;
			iIndex = 0 ;
			iIndex = jdi_CUtilsStrLastIndexOf (pcTempExt, '.', jc_strlen (pcTempExt) - 1) ;
			if (0 < iIndex)
			{
				pcTempExt = pcTempExt + iIndex + 1 ;
			}
		}
	}
	mmi_trace(12, "jdc_log: GetFileNameForDC ext[%s] ", pcTempExt);
	pmFileName = *ppmFileName ;
	{
		JC_INT8 cFileName[64]={0,};
		jdi_CUtilsStrTcsNCpy(pmFileName, 64, cFileName);
		mmi_trace(1,"jdc_log: cFileName[%s]", cFileName);
	}

	
	iIndex = 0 ;
	iIndex = jdi_CUtilsTcsLastIndexOf (pmFileName, '.', jc_tcslen (pmFileName) - 1) ;
	mmi_trace(1,"jdc_log: dot index in file name[%d]", iIndex);
	if (0 < iIndex)
	{
		if ((iIndex + 1) < (JC_INT32) (jc_tcslen (pmFileName)))
		{
			pmTemp = jdi_CUtilsTcsCopyIndex (JC_NULL, pmFileName, iIndex,
											 jc_tcslen (pmFileName) - 1) ;
			{
				JC_INT8 cFileName[64]={0,};
				jdi_CUtilsStrTcsNCpy(pmTemp, 64, cFileName);
				mmi_trace(1,"jdc_log: pmTemp[%s]", cFileName);
			}
			if (JC_NULL != pmTemp)
			{
				INT	iCmpVal = 0;				
				rCode = jdi_CUtilsTcharToChar (JC_NULL, pmTemp, &pcTemp) ;
				if(JC_NULL != pcContentType)
				{
					iCmpVal = jc_strcasecmp(pcTemp, pcContentType);
				}
				else
				{
					if(JC_NULL != g_pstJWapAppInfo->pcMediaType)
					{
						jdd_MiscGetExtForContentType (g_pstJWapAppInfo->pcMediaType, \
													(const JC_INT8**)&pcContentType) ;
						iCmpVal = jc_strcasecmp(pcTemp, pcContentType);
					}
				}
				mmi_trace(12, "jdc_log: cmp value[%d] pcTemp[%s] basetype[%s] base[%x]", \
							iCmpVal, pcTemp, pcContentType, pcContentType);
				if(0 == iCmpVal)
					bIsAlreadyPresent = E_TRUE ;
				else
					bIsAlreadyPresent = E_FALSE ;
			}
		}
		else
		{
			bIsAlreadyPresent = E_FALSE ;
		}
	}
	
	if (JC_OK == rCode)
	{
		if (E_FALSE == bIsAlreadyPresent)
		{
			rCode = jdi_CUtilsTcharToChar (JC_NULL, pmFileName, &pcFile) ;
			mmi_trace(12, "jdc_log: rCode pcFile [%d] filename[%s]", rCode, pcFile);
			if (JC_OK == rCode)
			{
				/*
				if (E_TRUE == bIsBreak)
				{
					pcFileName = (JC_INT8 *) jdd_MemAlloc (jc_strlen (pcFile) +
														   jc_strlen (pcContentType) + 1,
														   sizeof (JC_INT8)) ;
					if (JC_NULL == pcFileName)
					{
						rCode = JC_ERR_MEMORY_ALLOCATION ;
					}
					else
					{
						jc_strcpy (pcFileName, pcFile) ;
						jc_strcat (pcFileName, pcContentType) ;
						jdd_MemFree (*ppmFileName) ;
						rCode = jdi_CUtilsCharToTchar (JC_NULL, pcFileName, ppmFileName) ;
						mmi_trace(0, "jdc_log: rCode ppmFileName [%d]", rCode);
						jdd_MemFree (pcFileName) ;
					}
				}
				else */if (JC_NULL != pcTempExt)
				{
					if ('.' != pcFile [jc_strlen (pcFile) - 1])
					{
						pcFileName = (JC_INT8 *) jdd_MemAlloc (jc_strlen (pcFile) +
															   jc_strlen (pcTempExt) + 2,
															   sizeof (JC_INT8)) ;
						if (JC_NULL == pcFileName)
						{
							rCode = JC_ERR_MEMORY_ALLOCATION ;
						}
						else
						{
							jc_strcpy (pcFileName, pcFile) ;
							jc_strcat (pcFileName, ".") ;
							jc_strcat (pcFileName, pcTempExt) ;
							jdd_MemFree (*ppmFileName) ;
							rCode = jdi_CUtilsCharToTchar (JC_NULL, pcFileName, ppmFileName) ;
							{
								JC_INT8 cName[128]= {0,};
								jdi_CUtilsStrTcsNCpy (*ppmFileName, 128, cName) ;
								mmi_trace(12,"jdc_log: GetFileNameForDC FileName[%s]", cName);
							}							
							mmi_trace(12, "jdc_log: rCode ppmFileName [%d]", rCode);
							jdd_MemFree (pcFileName) ;
						}
					}
					else
					{
						pcFileName = (JC_INT8 *) jdd_MemAlloc (jc_strlen (pcFile) +
															   jc_strlen (pcTempExt) + 1,
															   sizeof (JC_INT8)) ;
						if (JC_NULL == pcFileName)
						{
							rCode = JC_ERR_MEMORY_ALLOCATION ;
						}
						else
						{
							jc_strcpy (pcFileName, pcFile) ;
							jc_strcat (pcFileName, pcTempExt) ;
							jdd_MemFree (*ppmFileName) ;
							rCode = jdi_CUtilsCharToTchar (JC_NULL, pcFileName, ppmFileName) ;
							mmi_trace(12, "jdc_log: rCode ppmFileName [%d]", rCode);
							jdd_MemFree (pcFileName) ;
						}
					}
				}
				if (JC_NULL != pcFile)
				{
					jdd_MemFree (pcFile) ;
				}
			}
		}
	}
	
	if (JC_NULL != pmTemp)
	{
		jdd_MemFree (pmTemp) ;
	}
	if (JC_NULL != pcTemp)
	{
		jdd_MemFree (pcTemp) ;
	}	
	if (JC_NULL != pcTempBase)
	{
		jdd_MemFree (pcTempBase) ;
	}
	mmi_trace(12, "jdc_log: GetFileNameForDC rcode[%d]", rCode);
	return rCode ;
}

JC_RETCODE	jwap_appDCErrorIndication (ST_DC_FETCH_ERROR_INFO *pstFetchErrorInfo)
{
	JC_RETCODE	rCode = JC_OK ;
	mmi_trace(12, "jdc_log: DCErrorIndication called %d", pstFetchErrorInfo->rRetCode);
	return rCode ;	
}

void jwap_DCFetchHeader (ST_DC_HEADER_IND		*pstHeaderInd)
{
	jdi_DCSetEvent (g_pstJWapAppInfo->pstDownloadClient->vDCHandle, 
					E_DC_FETCH_HEADER_INFO,
					g_pstJWapAppInfo->pstDownloadClient->pvDCCallbackArg, 
					pstHeaderInd) ;
	return ;
	
}

void jwap_DCFetchIndication (ST_DC_FETCH_IND		*pstFetchInd)
{
	jdi_DCSetEvent (g_pstJWapAppInfo->pstDownloadClient->vDCHandle, 
					E_DC_FETCH_INDICATION_INFO,
					g_pstJWapAppInfo->pstDownloadClient->pvDCCallbackArg, 
					pstFetchInd) ;
	return ;
	
}

void jwap_DCFetchError(ST_DC_ERR_IND *pstErrorInd)
{
	jdi_DCSetEvent (g_pstJWapAppInfo->pstDownloadClient->vDCHandle, 
					E_DC_FETCH_ERROR_INFO,
					g_pstJWapAppInfo->pstDownloadClient->pvDCCallbackArg, 
					pstErrorInd) ;
	return ;
	
}

JC_RETCODE jwap_DCHandleDownloadListener (ST_BE_URL_IND *pstBeURLInd)
{
	JC_RETCODE		rCode = JC_OK ;
	mmi_trace(12, "jdc_log: HandleDownloadListener type[%d]", pstBeURLInd->eIndType);
	
	if (JC_NULL != pstBeURLInd)
	{
		if (E_BE_HEADER_INDICATION == pstBeURLInd->eIndType)
		{
			ST_DC_HEADER_IND		stHeaderInd ;
			jc_memset (&stHeaderInd, 0x00, sizeof (ST_DC_HEADER_IND)) ;

			stHeaderInd.bIsBinary = pstBeURLInd->uBeInd.stBeHeaderInd.bIsBinary ;
			stHeaderInd.eHttpStatus = pstBeURLInd->uBeInd.stBeHeaderInd.eHTTPStatus ;
			stHeaderInd.pcContentType = pstBeURLInd->uBeInd.stBeHeaderInd.pcContentType ;
			stHeaderInd.pmUrl = pstBeURLInd->uBeInd.stBeHeaderInd.pmURL ;
			stHeaderInd.pucHeader = pstBeURLInd->uBeInd.stBeHeaderInd.pucHeader ;
			stHeaderInd.uiContentLength = pstBeURLInd->uBeInd.stBeHeaderInd.uiContentLength ;
			stHeaderInd.uiHeaderLength = pstBeURLInd->uBeInd.stBeHeaderInd.uiHeaderLength ;
			stHeaderInd.uiRequestID = pstBeURLInd->uiRequestID ;
			jwap_DCFetchHeader (&stHeaderInd) ;
			mmi_trace(12, "jdc_log: HandleDownloadListener called  E_BE_HEADER_INDICATION end");

		}
		if (E_BE_BUFFER_INDICATION == pstBeURLInd->eIndType)
		{
			ST_DC_FETCH_IND		stFetchUrlIndResp ;
			jc_memset (&stFetchUrlIndResp, 0x00, sizeof (ST_DC_FETCH_IND)) ;

			stFetchUrlIndResp.bHasMoreData = pstBeURLInd->uBeInd.stBeFetchInd.bHasMoreData ;
			stFetchUrlIndResp.pucBuffer = pstBeURLInd->uBeInd.stBeFetchInd.pucBuffer ;
			stFetchUrlIndResp.uiCurrentLength = pstBeURLInd->uBeInd.stBeFetchInd.uiCurrentLength ;
			stFetchUrlIndResp.uiRequestID = pstBeURLInd->uiRequestID ;
			jwap_DCFetchIndication (&stFetchUrlIndResp) ;
			mmi_trace(12, "jdc_log: HandleDownloadListener called E_BE_BUFFER_INDICATION end");
		}
		if (E_BE_ERROR_INDICATION == pstBeURLInd->eIndType)
		{
			ST_DC_ERR_IND		stErrorInd ;
			jc_memset (&stErrorInd , 0x00, sizeof (ST_DC_ERR_IND)) ;

			stErrorInd.eErrType = pstBeURLInd->eIndType ;
			stErrorInd.rCode = pstBeURLInd->rErrorCode ;
			stErrorInd.uiRequestID = pstBeURLInd->uiRequestID ;

			jwap_DCFetchError (&stErrorInd) ;
			mmi_trace(12, "jdc_log: HandleDownloadListener called E_BE_ERROR_INDICATION end");
		}
	}
	return rCode ;
}

JC_RETCODE	jwap_appDCInstallationStatus (ST_DC_INSTALLATION_STATUS *pstInstallationStatus)
{
	JC_RETCODE	rCode = JC_OK ;
	mmi_trace(12,"jdc_log: appDCInstallationStatus entry");
	jc_memset (g_pstJWapAppInfo->pcInputBuff, 0, jWAP_MAX_INPUT_SIZE) ;
	if (JC_OK == pstInstallationStatus->rRetCode)
	{		
		jWap_PopupCallBack () ;
		DisplayPopup ((PU8)GetString(STR_ID_WAP_HTTPDWN_DWNLOAD_SUCESS), \
			IMG_GLOBAL_ACTIVATED, 1, 2000, 1) ;
	}
	else if (JC_ERR_DC_NOTIFICATION_FAILURE == pstInstallationStatus->rRetCode)
	{
		DisplayPopup(GetString (STR_GLOBAL_OK),	IMG_GLOBAL_INFO, 1, 2000, 1) ;
		if (JC_NULL != g_pstJWapAppInfo->pmDownloadFileName)
		{
			rCode = jdd_FSDelete (JC_NULL, g_pstJWapAppInfo->pmDownloadFileName) ;
			mmi_trace (1, "jdc_log: Deleted the download temporary file rCode[%d]", rCode) ;
		}
	}
	jwap_FreeDownloadContent () ;
	if (JC_NULL != g_pstJWapAppInfo->pmDownloadFileName)
	{
		jdd_MemFree (g_pstJWapAppInfo->pmDownloadFileName) ;
		g_pstJWapAppInfo->pmDownloadFileName = JC_NULL ;
	}

	if (JC_NULL != g_pstJWapAppInfo->pmFetchURL)
	{
		jdd_MemFree (g_pstJWapAppInfo->pmFetchURL) ;
		g_pstJWapAppInfo->pmFetchURL = JC_NULL ;
	}

	if(g_pstJWapAppInfo->pcMediaType)
	{
		jdd_MemFree(g_pstJWapAppInfo->pcMediaType);
		g_pstJWapAppInfo->pcMediaType = JC_NULL;
	}

	if(JC_NULL != g_pstJWapAppInfo->stAuthenticationInfo.pmUserName)
	{
		jdd_MemFree(g_pstJWapAppInfo->stAuthenticationInfo.pmUserName);
		g_pstJWapAppInfo->stAuthenticationInfo.pmUserName = JC_NULL;
	}

	if(JC_NULL != g_pstJWapAppInfo->stAuthenticationInfo.pmPassword)
	{
		jdd_MemFree(g_pstJWapAppInfo->stAuthenticationInfo.pmPassword);
		g_pstJWapAppInfo->stAuthenticationInfo.pmPassword = JC_NULL;
	}	
	mmi_trace(1,"jdc_log: jwap_appDCInstallationStatus rcode[%d]",rCode);	
	return rCode ;
}

JC_RETCODE jwap_DCDeInit ()
{
	JC_RETCODE	rCode ;
	mmi_trace(12,"jdc_log: jwap_DCDeInit called");
	rCode =  jdi_DCDeInitialize (g_pstJWapAppInfo->pstDownloadClient->vDCHandle);

	if(JC_NULL != g_pstJWapAppInfo->pstDownloadClient)
	{
		jdd_MemFree (g_pstJWapAppInfo->pstDownloadClient) ;
		g_pstJWapAppInfo->pstDownloadClient = JC_NULL;
	}		
	return rCode ;								
}

void jwap_DCNonFeasibilityCallback (void)
{
	jwap_HandleDCUserconfLSKNo() ;
}
#endif// CS_DC_ENABLE


#endif // __JATAAYU_APP__

