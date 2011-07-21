#define MODULE_NAME "JWAP"
#define FILE_NAME "jwap_cb.c"
 /***************************************************************************
 *
 * File Name : jwap_cb.c
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
 * @file		jwap_cb.c
 * @ingroup	jWAP
 * @brief		Describes the Task related API's.
 *
 **/

#ifdef JATAAYU_SUPPORT
 /***************************************************************************
 * System Include Files
 **************************************************************************/


 /***************************************************************************
 * User Include Files
 **************************************************************************/

#include "jwap_inc.h"


#include <callmanagementstruct.h>

#include <callbarrng.h>
#include <pscallhelpergprot.h>

#include "jdi_browserengine.h"
#include "jwap_cb.h"
#include "commonscreens.h"
#include "jdi_cutils_tcsstr.h"
#include "jwap_httpdownload.h"
#include "jdi_cutils_tcs.h"
#include "jddex_platform.h"
#include "phonebookprot.h"
#include "jwap_interface.h"
#include "wgui_categories_popup.h"
#include "jdi_transportmodule.h"
#include "jdi_wsp.h"
#include "satgprots.h"

 /***************************************************************************
 * Macros
 **************************************************************************/

#define WTAI_MC_SCHEME				"wtai://wp/mc"
#define WTAI_SD_SCHEME 				"wtai://wp/sd"
#define WTAI_AP_SCHEME 				"wtai://wp/ap"

#define  MAX_TEL_NUM		20
#define  MAX_TEL_NAME		14


 /***************************************************************************
 * Global variables declarations
 **************************************************************************/

 /***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

 /***************************************************************************
 * All Local Function Definitions
 ***************************************************************************/
void jWap_endDispHandler (ST_BE_END_PAGE_INFO* pstCallbackInfo)
{
	mmi_trace (1, "JDD_LOG: jWap_endDispHandler rCode [%X], eStatus [%d], rPageStatusCode [%X], bRequestPending [%d]",
		-pstCallbackInfo->rCode, pstCallbackInfo->eStatus,
		-pstCallbackInfo->rPageStatusCode, pstCallbackInfo->bRequestPending) ;

	/* See whether Authorization Info is set */
	if (g_pstJWapAppInfo->pstFetchReq)
	{
		DisplayPopupCallBack((PU8)GetString(STR_ID_WAP_AUTHORIZATION), IMG_GLOBAL_INFO, 1, \
				3000, 1, jWap_showAuthUserNameScreen); //Last argument is tone id. 1 = ERROR tone
	}
	else if (!pstCallbackInfo->stNavg.bBackNavigation && GetActiveScreenId() == SCR_ID_WAP_HOMEPAGE_SCREEN)
	{		
		/* This is the first page. Remove LSK */
		ClearKeyHandler (KEY_RSK, KEY_EVENT_DOWN) ;
		ChangeRightSoftkey (0, 0) ;

		ChangeLeftSoftkey (STR_GLOBAL_OPTIONS, 0) ;
		SetLeftSoftkeyFunction (jWap_options_handler, KEY_EVENT_UP) ;
		
		ChangeTitleIcon (IMG_ID_JATAAYU_WAP_END_ANIMATION_GIF) ;
		draw_title () ;
	}
	else if(GetActiveScreenId() == SCR_ID_WAP_HOMEPAGE_SCREEN)
	{
		/* There are more than 1 page. Set to "Back"  */
		ChangeRightSoftkey (STR_GLOBAL_BACK, 0) ;
		SetRightSoftkeyFunction (jWap_back, KEY_EVENT_UP);

		ChangeLeftSoftkey (STR_GLOBAL_OPTIONS, 0) ;
		SetLeftSoftkeyFunction (jWap_options_handler, KEY_EVENT_UP) ;		
		
		ChangeTitleIcon (IMG_ID_JATAAYU_WAP_END_ANIMATION_GIF) ;
		draw_title () ;
	}	
}

INT jWap_beginDispHandler ()
{
	/* Disable LSK when fetching is in progress */	
	if(GetActiveScreenId() == SCR_ID_WAP_HOMEPAGE_SCREEN)
	{
		ClearKeyHandler (KEY_LSK, KEY_EVENT_DOWN) ;
		ChangeLeftSoftkey (0, 0) ;

		/* Set RSK to cancel */
		ChangeRightSoftkey (STR_GLOBAL_CANCEL, KEY_EVENT_UP) ;
		SetRightSoftkeyFunction (jWap_priMmi_cancel_handler, KEY_EVENT_UP) ;

		ChangeTitleIcon (IMG_ID_JATAAYU_WAP_ANIMATION_GIF) ;
		draw_title();
	}	
	return (JC_OK) ;
}

void jWap_HTTPErrorHandler (E_HTTP_STATUS eHTTPStatus)
{
	JC_UINT16 uhStrId ;
	mmi_trace(g_sw_WAP,"JDD_LOG: Func: %s ERROR: %d", __FUNCTION__, eHTTPStatus);
	switch (eHTTPStatus)
	{
		case E_HTTP_NON_AUTHORITATIVE:
			uhStrId = STR_ID_WAP_NON_AUTHORITATIVE ;
			break ;

		case E_HTTP_NO_CONTENT:
			uhStrId = STR_ID_WAP_NO_CONTENT ;
			break ;

		case E_HTTP_MOVED_PERMANENTLY:
			uhStrId = STR_ID_WAP_MOVED_PERMANENTLY ;
			break ;

		case E_HTTP_MOVED_TEMPORARILY:
			uhStrId = STR_ID_WAP_MOVED_TEMPORARILY ;
			break ;

		case E_HTTP_BAD_REQUEST:
			uhStrId = STR_ID_WAP_BAD_REQUEST ;
			break ;

		case E_HTTP_UNAUTHORIZED:
			uhStrId = STR_ID_WAP_UNAUTHORIZED ;
			break ;

		case E_HTTP_PAYMENT:
			uhStrId = STR_ID_WAP_PAYMENT ;
			break ;

		case E_HTTP_FORBIDDEN:
			uhStrId = STR_ID_WAP_FORBIDDEN ;
			break ;

		case E_HTTP_NOT_FOUND:
			uhStrId = STR_ID_WAP_NOT_FOUND ;
			break ;
		
		case E_HTTP_METHOD_NOT_ALLOWED:
			uhStrId = STR_ID_WAP_METHOD_NOT_ALLOWED ;
			break ;

		case E_HTTP_NOT_ACCEPTABLE:
			uhStrId = STR_ID_WAP_NOT_ACCEPTABLE ;
			break ;

		case E_HTTP_REQUEST_TIMEOUT:
			uhStrId = STR_ID_WAP_REQUEST_TIMEOUT ;
			break ;

		case E_HTTP_CONFLICT:
			uhStrId = STR_ID_WAP_CONFLICT ;
			break ;

		case E_HTTP_REQUEST_ENTITY_TOO_LARGE:
			uhStrId = STR_ID_WAP_REQUEST_ENTITY_TOO_LARGE ;
			break ;

		case E_HTTP_REQUEST_URI_TOO_LARGE:
			uhStrId = STR_ID_WAP_REQUEST_URI_TOO_LARGE ;
			break ;

		case E_HTTP_UNSUPPORTED_MEDIA_TYPE:
			uhStrId = STR_ID_WAP_UNSUPPORTED_MEDIA_TYPE ;
			break ;

		case E_HTTP_INTERNAL_SERVER_ERROR:
			uhStrId = STR_ID_WAP_INTERNAL_SERVER_ERROR ;
			break ;

		case E_HTTP_NOT_IMPLEMENTED:
			uhStrId = STR_ID_WAP_NOT_IMPLEMENTED ;
			break ;

		case E_HTTP_BAD_GATEWAY:
			uhStrId = STR_ID_WAP_BAD_GATEWAY ;
			break ;

		case E_HTTP_SERVICE_UNAVAILABLE:
			uhStrId = STR_ID_WAP_SERVICE_UNAVAILABLE ;
			break ;

		case E_HTTP_GATEWAY_TIMEOUT:
			uhStrId = STR_ID_WAP_GATEWAY_TIMEOUT ;
			break ;
		
		case E_HTTP_BAD_VERSION:
			uhStrId = STR_ID_WAP_BAD_VERSION ;
			break ;
			
		default:
			uhStrId = STR_ID_WAP_HTTP_UNKNOWN ;
			break ;
	}
	DisplayPopupCallBack (GetString (uhStrId), IMG_GLOBAL_ERROR, 1,	2000, 1, jWap_PopupCallBack) ;
}

#ifdef JATAAYU_SUPPORT_MMS
	extern void jdd_TraceErrCode(JC_RETCODE rCode, JC_INT32	uiModule);
#endif

void  jWap_ErrorHandler (JC_RETCODE rCode)
{
	JC_UINT16	uhStrId = STR_ID_WAP_UNKNOWN ;
	JC_UINT16	uhImgId = IMG_GLOBAL_ERROR ;
	JC_INT32	uiModule = (-rCode) & 0x00FF0000 ;

	mmi_trace(12,"JDD_LOG: Func: %s ERROR rCode:0x%x uiModule:0x%x", __FUNCTION__, -rCode, uiModule);
	switch (uiModule)
	{
		case COMMON_APPLICATION:
			break ;
		case PROVISIONING_APPLICATION:
			break ;
		case TRANSPORT_APPLICATION:
			{
				uiModule = (-rCode) & 0x00FFFF00 ;				
				if(uiModule == WSP_MODULE)
				{
					switch (rCode)
					{							
						case JC_ERR_WSP_TIMED_OUT:
						{
							uhStrId = STR_ERR_NET_TIMEOUT ;
						}
						break;							
					}					
				}
				else
				{
					uhStrId = STR_ID_WAP_NETWORK_ERROR ;
				}
			}
			break ;
		case DOWNLOAD_CLIENT_APPLICATION:
			break ;
		case BROWSER_APPLICATION:
			uiModule = (-rCode) & 0x00FFFF00 ;
			if (uiModule == WMLSCRIPT_MODULE)
			{
				//uhStrId = "Script Processing Error" ;
			}
			else
			{
				switch (rCode) 
				{
					case JC_ERR_BE_UNSUPPORTED_CONTENT_TYPE:
					{
						uhStrId = STR_ID_WAP_UNSUPPORTED_CONTENT_TYPE ;
					}
					break ;

					case JC_ERR_BE_ACCESS_DENIED:
					{
						uhStrId = STR_ID_WAP_ACCESS_DENIED ;
					}
					break ;


					case JC_ERR_BE_PAGE_SIZE_EXCEEDS:
					{
						uhStrId = STR_ID_WAP_PAGE_SIZE_EXCEEDS ;
					}
					break ;

					case JC_ERR_BE_INVALID_INPUT:
					{
						uhStrId = STR_ID_WAP_INVALID_INPUT ;
					}
					break ;

					case JC_ERR_BE_EXCEEDS_MAX_REDIRECT:
					{
						uhStrId = STR_ID_WAP_MAX_REDIRECT ;
					}
					break ;

					case JC_ERR_BE_HISTORY_NOT_FOUND:
					{
						uhStrId = STR_ID_WAP_HISTORY_NOT_FOUND ;
					}
					break ;

					
					case JC_ERR_BE_HISTORY_FULL:
					{
						uhStrId = STR_ID_WAP_HISTORY_FULL ;
					}
					break ;

					case JC_ERR_BE_EMPTY_PAGE :
					{
						uhStrId = STR_ID_WAP_EMPTY_PAGE ;
					}
					break ;

					case JC_ERR_PUSH_LIST_FULL:
					break;
					default:
					break ;
				}
			}
			break ;
		case ABSTRACT_APPLICATION:
			uiModule = (-rCode) & 0x00FFFF00;
			#ifdef JATAAYU_SUPPORT_MMS
				jdd_TraceErrCode(rCode, uiModule);
			#endif
			switch (uiModule)
			{
				case NET_MODULE:
					switch (rCode)
					{
						case JC_ERR_NET_INIT:
						{
							uhStrId = STR_ERR_NET_INIT ;
						}
						break;
						case JC_ERR_NET_CONNECT:
						{
							uhStrId = STR_ERR_NET_CONNECT ;
						}
						break ;
						case JC_ERR_NET_DNS_RESOLVE:
						{
							uhStrId = STR_ERR_NET_DNS_RESOLVE ;
						}
						break ;
						case JC_ERR_NET_TIMEOUT:
						{
							uhStrId = STR_ERR_NET_TIMEOUT ;
						}
						break ;
						case JC_ERR_NET_CONN_ABORT:
						{
							uhStrId = STR_ERR_NET_CONN_ABORT ;
						}
						break ;
						default:
						{
							uhStrId = STR_ID_WAP_NETWORK_ERROR ;
						}
						break ;
					}
					break ;
				case FILE_MODULE:
					break ;
				case TIMER_MODULE:
					break ;
				case DATA_CONN_MODULE:
					switch (rCode)
					{
						case JC_ERR_DATA_CONN_TIMEOUT:
							{
								uhStrId = STR_ERR_DATA_CONN_TIMEOUT ;
							}
							break ;
						default:
							{
								uhStrId = STR_ERR_DATA_CONN_UNKNOWN ;
							}
							break ;
					}
					break ;
				case OTHER_MODULE:
					switch (rCode)
					{
						case JC_ERR_NOT_SUPPORTED:
							{
								uhStrId = STR_ID_WAP_UNSUPPORTED_MEDIA_TYPE ;
							}
							break ;
						default:
							break ;
					}
					break ;
			}
			break ;
		}
	if (uhStrId == STR_ID_WAP_EMPTY_PAGE)
	{
		mmi_trace (1, "JDD_LOG: jWap_ErrorHandler called with JC_ERR_BE_EMPTY_PAGE. Avoiding popup") ;
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jWap_ErrorHandler showing error popup") ;
		DisplayPopupCallBack (GetString (uhStrId), uhImgId, 1,	2000, 1, jWap_PopupCallBack) ;
	}
}

extern U16 GetCurrScrnId(void);
extern BOOL isInWapApp(void);
extern pBOOL isInCall(void);
void jWap_PopupCallBack()
{
	INT	 iErr ;
	ST_BE_PAGE_CONTENT 	 stPageCont ;

	jc_memset (&stPageCont, 0, sizeof (ST_BE_PAGE_CONTENT)) ;

	mmi_trace (1, "JDD_LOG: jWap_PopupCallBack starts") ;
	
	if((!isInWapApp())||isInCall())
	{
		GoBackHistory();
		mmi_trace (1, "JDD_LOG: go back screen ignoring") ;
		return;
	}
	
	if (E_WAP_EXIT_KEY_END == g_pstJWapAppInfo->stWapState.eWapExitKeyPressed)
	{
		mmi_trace (1, "JDD_LOG: aborting screen ignoring") ;
		return;
	}
	
	iErr = jdi_BEGetPageContent (g_pstJWapAppInfo->hBECore, &stPageCont) ;

	if (iErr)
	{
		// There is no content displayed on Primary MMI
		jWap_options_quit_handler () ;
	}
	else
	{
		if (!jWap_isBack ())
		{
			//ChangeRightSoftkey (STR_GLOBAL_QUIT, 0) ;
			//This is the first page. Remove RSK
			ClearKeyHandler (KEY_RSK, KEY_EVENT_DOWN) ;
		}
		else
		{
			// There are more than 1 page. Set to "Back"
			ChangeRightSoftkey (STR_GLOBAL_BACK, 0) ;
			SetRightSoftkeyFunction (jWap_back, KEY_EVENT_UP) ;
		}		
				
		ChangeLeftSoftkey (STR_GLOBAL_OPTIONS, 0) ;
		SetLeftSoftkeyFunction (jWap_options_handler, KEY_EVENT_UP) ;

		ChangeTitleIcon (IMG_ID_JATAAYU_WAP_END_ANIMATION_GIF) ;
		//setup_scrolling_title () ;
		draw_title () ;
		jmms_goto_primaryMMI();		
	}
	mmi_trace (1, "@@@ jWap_PopupCallBack end @@@") ;
}





void jWap_authHandler (ST_BE_PAGE_INFO* pstPageInfo, void *pvArg)
{
	INT		iErr ;

	mmi_trace (1, "JDD_LOG: jWap_authHandler @@@") ;

	PARAM_INTENTIONALLY_NOT_USED (pvArg) ;

	{
		JC_INT8  *pcFName = JC_NULL ;

		jdi_CUtilsTcharToChar (JC_NULL, pstPageInfo->pmURI, &pcFName) ;
		if (pcFName)
		{
			mmi_trace (1, "JDD_LOG: Auth URL: %s @@@", pcFName) ;
			jdd_MemFree (pcFName) ;
		}
	}

	iErr = jWap_copyAuthInfo (pstPageInfo) ;
	mmi_trace (1, "JDD_LOG: jWap_copyAuthInfo returns %X", JRET (iErr)) ;

	if (!iErr)
	{
		/* Screen displayed in End Page Listener */
		//jWap_showAuthUserNameScreen () ;

	}
	else
	{
		/* Display Popup for Error */
	}

	if (pstPageInfo->bFreeMemory)
	{
		jWap_freePageInfo (pstPageInfo) ;
	}

	return ;

}


void jWap_titleHandler (JC_CHAR *pmTitle)
{

	mmi_trace (1, "JDD_LOG: jWap_titleHandler @@@") ;


	if(!pmTitle)
	return;

	//PARAM_INTENTIONALLY_NOT_USED (pvArg) ;
	if(NULL != g_pstJWapAppInfo->pszTitleString)
	{
		jdd_MemFree(g_pstJWapAppInfo->pszTitleString) ;
		g_pstJWapAppInfo->pszTitleString = JC_NULL ;
		ChangeTitleString (NULL) ;
	}
	g_pstJWapAppInfo->pszTitleString = (JC_CHAR*)jdd_MemAlloc(jc_wstrlen(pmTitle)+1,sizeof(JC_CHAR)) ;

	jc_wstrcpy(g_pstJWapAppInfo->pszTitleString,pmTitle) ;

    if(g_pstJWapAppInfo->bIsPriMmiActive)
    {
		ChangeTitleString ((U8*)g_pstJWapAppInfo->pszTitleString) ;
		draw_title();
    }

}


void jWap_vscrollHandler (ST_BE_SCROLL_INFO * pstBeScrollInfo)
{
	SCROLL_INFO stVScrollInfo = {0,};

   stVScrollInfo.uiMinPos = 0;
   stVScrollInfo.uiPageSize = jddex_GetDeviceRenderHeight();


	if (pstBeScrollInfo->uiMaxAxis <= stVScrollInfo.uiPageSize)
	{
		stVScrollInfo.bIsScrollDown = 0;
		stVScrollInfo.bIsScrollUp = 0;
		stVScrollInfo.uiCurrPos = 0;
		stVScrollInfo.uiMaxPos = 0;
	}
	else
	{
		JC_DOUBLE dFactor;
		stVScrollInfo.bIsScrollDown = pstBeScrollInfo->bScrollDownRight;
		stVScrollInfo.bIsScrollUp = pstBeScrollInfo->bScrollUpLeft;
		stVScrollInfo.uiMaxPos = pstBeScrollInfo->uiMaxAxis;

		dFactor = (JC_DOUBLE) pstBeScrollInfo->uiMaxAxis / \
					(JC_DOUBLE)(pstBeScrollInfo->uiMaxAxis- stVScrollInfo.uiPageSize) ;

		stVScrollInfo.uiCurrPos = (JC_DOUBLE)pstBeScrollInfo->uiCurrAxis /dFactor ;


	}
	mmi_trace (1, "JDD_LOG: VScroll: Min=%d, Max=%d, PageSize=%d, currPos=%d", \
		stVScrollInfo.uiMinPos, stVScrollInfo.uiMaxPos, \
		stVScrollInfo.uiPageSize, stVScrollInfo.uiCurrPos) ;
	mmi_trace (1, "JDD_LOG: VScroll: UP=%d, DOWN=%d", \
		stVScrollInfo.bIsScrollUp, stVScrollInfo.bIsScrollDown) ;

	if (g_pstJWapAppInfo->uiWinId)
		jddex_MMIBrwSetInfo(g_pstJWapAppInfo->uiWinId, &stVScrollInfo);

}

void jWap_MakeCallHandler(void* pvCallbackInfo)
 	{

	JC_CHAR					*pszURLWithScheme ;
	ST_BE_SCHEME_LISTENER 	*pstSchemeListener ;
	JC_CHAR					*pszURL ;
	JC_CHAR				    	*pszFilterdURLWithScheme ;
	JC_CHAR 					*pszNumber ;
	JC_CHAR 					*pmNumPtr  ;
	JC_CHAR 					*pmNamePtr ;
	JC_CHAR					*pmNum ;
	JC_UINT8 					ucNumLen ;
	JC_UINT8 					ucNameLen ;
	JC_CHAR					*pmName ;

	pszURLWithScheme	= NULL ;
	pszURL				= NULL ;
	pstSchemeListener 	= (ST_BE_SCHEME_LISTENER *)pvCallbackInfo ;
	pszFilterdURLWithScheme = JC_NULL ;
	pmNamePtr = JC_NULL ;
	pmNum = JC_NULL ;
	pmName = NULL ;

	pszURL = jc_wstrchr(pstSchemeListener->pmURI, '!');

	if(NULL != pszURL)
	{
		mmi_trace (1, "@@@  pszurl not null @@@") ;

		pszFilterdURLWithScheme = (JC_CHAR *)  jdd_MemAlloc ((jc_wstrlen (pstSchemeListener->pmURI) - \
			jc_wstrlen(pszURL)) + 1, sizeof (JC_CHAR));

		if (JC_NULL != pszFilterdURLWithScheme)
		{
			mmi_trace (1, "@@@  alloc clear @@@") ;
		  jc_wstrncpy(pszFilterdURLWithScheme,pstSchemeListener->pmURI, jc_wstrlen(pstSchemeListener->pmURI)- jc_wstrlen(pszURL));
		}
	}
	else
	{
		pszFilterdURLWithScheme = (JC_CHAR *)  jdd_MemAlloc (jc_wstrlen (pstSchemeListener->pmURI)  + 1, sizeof (JC_CHAR)) ;
		jc_wstrcpy (pszFilterdURLWithScheme, pstSchemeListener->pmURI) ;
	}


	if (jdi_CUtilsTcsStrStartsWith (pszFilterdURLWithScheme, WTAI_MC_SCHEME))
	{
		mmi_trace (1, "@@@  WTAI_MC_SCHEME @@@")  ;
		pszNumber = pszFilterdURLWithScheme + 13 ;
		MakeCall((PS8) pszNumber) ;
		}
	else if (jdi_CUtilsTcsStrStartsWith (pszFilterdURLWithScheme, WTAI_SD_SCHEME))
	{
			/* Display Popup. DTMF NOT SUPPORTED */
	}
	else if (jdi_CUtilsTcsStrStartsWith (pszFilterdURLWithScheme, WTAI_AP_SCHEME))
	{
		pmNumPtr = pszFilterdURLWithScheme + 13 ;
		pmNamePtr = jdi_CUtilsTcsStrStr  (pmNumPtr, ";")  ;

		ucNumLen = pmNamePtr -  pmNumPtr ;

		if (pmNamePtr)
		{
			pmNamePtr ++ ;
		}

		ucNameLen = jc_wstrlen(pmNamePtr) ;

		if(MAX_TEL_NAME <= ucNameLen)
			{
				ucNameLen = MAX_TEL_NAME ;
			}

		pmName = jdd_MemAlloc (ucNameLen + 1, sizeof (JC_CHAR)) ;

		jc_wstrncpy (pmName, pmNamePtr, ucNameLen) ;


		if(MAX_TEL_NUM <= ucNumLen  )
			{
				ucNumLen = MAX_TEL_NUM ;
			}
		pmNum = jdd_MemAlloc (ucNumLen + 1, sizeof (JC_CHAR)) ;

		if (pmNum)
		{

			jc_wstrncpy (pmNum, pmNumPtr, ucNumLen) ;

			mmi_phb_op_add_entry_Directly_req((S8*)pmName, (S8*)pmNum) ;

			//jdd_MemFree(pmName) ;
			//jdd_MemFree(pmNum) ;
		}
	}

	if (pszFilterdURLWithScheme)
			{
		jdd_MemFree (pszFilterdURLWithScheme) ;
	}

	if (pmNum)
	{
		jdd_MemFree (pmNum) ;
	}
}

#if 0
				if(JC_OK == rCode)
					{
						bState = jdi_CUtilsTcsStartsWith(pszFilterdURLWithScheme, pszURLWithScheme);
						jdd_MemFree(pszURLWithScheme) ;pszURLWithScheme = NULL ;
						if (E_TRUE == bState)
						{
							pszNumber = pszFilterdURLWithScheme + 13 ;
							//MakePsSendDTMFTone(pszNumber) ;

							//iLength = jc_wstrlen(pszNumber) ;
							/*for(iIndex = 0;iIndex < iLength;iIndex++)
								{
									MakePsSendDTMFTone(*pszNumber) ;
									pszNumber++ ;
								}
								*/
								jdi_CUtilsTcharToChar(NULL,pszNumber,&pcDest) ;

								mmi_trace (1,"string destination is %s",pcDest) ;

								MakeCall((PS8) pszNumber) ;
						}
						else
						{
							rCode = jdi_CUtilsCharToTchar(NULL, WTAI_AP_SCHEME, &pszURLWithScheme);
							if(JC_OK == rCode)
								{
									bState = jdi_CUtilsTcsStartsWith(pszFilterdURLWithScheme, pszURLWithScheme);
									jdd_MemFree(pszURLWithScheme) ;pszURLWithScheme = NULL ;
									if (E_TRUE == bState)
									{
										pszURL = pszFilterdURLWithScheme + 13 ;
										/*//pszURL = jc_wstrchr(pszFilterdURLWithScheme, ';');
										mmi_trace (1, "@@@  pszName true  @@@") ;


										pszName = jc_wstrchr(pszURL, ';');
										pszName++ ;

										jdi_CUtilsTcharToChar(NULL,pszName,&pcDest) ;

										mmi_trace (1, "@@@  pszName is %s @@@",pcDest) ;
										//pszName++ ;

										pszNumber = jdd_MemAlloc(jc_wstrlen(pszURL)- jc_wstrlen(pszName)+1,sizeof(JC_CHAR)) ;

										//pszNumber = (JC_CHAR*)(pszURL - pszName) ;

										jc_wstrncpy(pszNumber,pszURL,jc_wstrlen(pszURL)- jc_wstrlen(pszName)) ;

										//pszName = pszFilterdURLWithScheme + pszURL + 1 ;	//+1 to move to name after taking the phone number
										// need to add the code for adding to phone book
										mmi_trace (1, "@@@  calc done @@@") ;
										jdi_CUtilsTcharToChar(NULL,pszNumber,&pcDest) ;
										jdi_CUtilsTcharToChar(NULL,pszName,&pcDest1) ;
										mmi_trace (1, "@@@  name is %s @@@",pcDest) ;
										mmi_trace (1, "@@@  phone number is %s @@@",pcDest1) ;
										*/

										while(pszURL)
											{

											}

										mmi_phb_op_add_entry_Directly_req((S8*)pszName, (S8*)pszNumber) ;
									}
								}
						}
					}
			}
	}
	 if(NULL != pszFilterdURLWithScheme)
	 	{
	 		jdd_MemFree(pszFilterdURLWithScheme) ;
			pszFilterdURLWithScheme = NULL ;
	 	}

}
#endif
void AlertCallBack()
{
	//GoBackHistory() ;
}

void jWap_HandleWmlAlertListener(ST_BE_WMLS_ALERT *pstWmlAlert)
{
	//beHandleWmlAlertListenerL (pstBeWMLSAlert->pmMessage) ;
	DisplayPopup((PU8)pstWmlAlert->pmMessage, IMG_GLOBAL_INFO, 1,4000, 1);
        AlertCallBack();
}



 void HandleWMLYes()
  {
  	//g_pstJWapAppInfo->g_pstWMLConfirm->bIsOkStatus = E_TRUE ;

	ST_BE_WMLS_CONFIRM_RESP stBeWmlsConfirmResp ;
	jc_memset (&stBeWmlsConfirmResp, 0, sizeof (ST_BE_WMLS_CONFIRM_RESP)) ;

	stBeWmlsConfirmResp.pvCbArg = g_pstJWapAppInfo->g_pstWMLConfirm->pvCbArg ;
	stBeWmlsConfirmResp.bIsOkStatus = E_TRUE ;

	(void)jdi_BESetEvent (g_pstJWapAppInfo->hBECore, E_BE_WMLS_CONFIRM_RESP, &stBeWmlsConfirmResp) ;

	GoBackHistory() ;
  }

  void HandleWMLNo()
  {
  //	g_pstJWapAppInfo->g_pstWMLConfirm->bIsOkStatus = E_FALSE ;

	ST_BE_WMLS_CONFIRM_RESP stBeWmlsConfirmResp ;
	jc_memset (&stBeWmlsConfirmResp, 0, sizeof (ST_BE_WMLS_CONFIRM_RESP)) ;

	stBeWmlsConfirmResp.pvCbArg =g_pstJWapAppInfo->g_pstWMLConfirm->pvCbArg ;
	stBeWmlsConfirmResp.bIsOkStatus = E_FALSE ;

	(void)jdi_BESetEvent (g_pstJWapAppInfo->hBECore, E_BE_WMLS_CONFIRM_RESP, &stBeWmlsConfirmResp) ;

	GoBackHistory() ;

  }

 void jWap_HandleWmlConfirmListener()
{
	mmi_trace (1, "@@@ GetUserConfirmation. begin");
	 DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, g_pstJWapAppInfo->g_pstWMLConfirm->pmMessage, 0, 1) ;
	 SetLeftSoftkeyFunction(HandleWMLYes,KEY_EVENT_UP) ;
	 SetRightSoftkeyFunction(HandleWMLNo,KEY_EVENT_UP) ;
	// g_pstJWapAppInfo->g_pstWMLConfirm->bIsOkStatus =  JC_ERR_WAIT_USER_ACTION ;
	 mmi_trace (1, "@@@ GetUserConfirmation. end");
}

void  jWap_ok_wml_msg()
{
	ST_BE_WMLS_PROMPT_RESP stBeWmlsPromptResp ;
	jc_memset (&stBeWmlsPromptResp, 0, sizeof (ST_BE_WMLS_PROMPT_RESP)) ;

	if(jc_wstrlen(jWap_getInputBuff) != 0)
		{
			stBeWmlsPromptResp.pmValue = jdd_MemAlloc(jc_wstrlen(jWap_getInputBuff)+1,sizeof(JC_CHAR)) ;

			jc_wstrncpy(stBeWmlsPromptResp.pmValue, jWap_getInputBuff, jc_wstrlen(jWap_getInputBuff) ) ;
		}


	stBeWmlsPromptResp.pvCbArg = g_pstJWapAppInfo->g_pstWMLPrompt->pvCbArg ;
	stBeWmlsPromptResp.bIsOkStatus = E_TRUE ;
	(void)jdi_BESetEvent (g_pstJWapAppInfo->hBECore, E_BE_WMLS_CONFIRM_RESP, &stBeWmlsPromptResp) ;
	GoBackHistory() ;
}

void  jWap_Cancel_wml_msg()
{
	ST_BE_WMLS_PROMPT_RESP stBeWmlsPromptResp ;
	jc_memset (&stBeWmlsPromptResp, 0, sizeof (ST_BE_WMLS_PROMPT_RESP)) ;

	if(jc_wstrlen(jWap_getInputBuff) != 0)
		{
			stBeWmlsPromptResp.pmValue = jdd_MemAlloc(jc_wstrlen(jWap_getInputBuff)+1,sizeof(JC_CHAR)) ;

			jc_wstrncpy(stBeWmlsPromptResp.pmValue, jWap_getInputBuff, jc_wstrlen(jWap_getInputBuff) ) ;
		}


	stBeWmlsPromptResp.pvCbArg = g_pstJWapAppInfo->g_pstWMLPrompt->pvCbArg ;
	stBeWmlsPromptResp.bIsOkStatus = E_FALSE ;
	(void)jdi_BESetEvent (g_pstJWapAppInfo->hBECore, E_BE_WMLS_CONFIRM_RESP, &stBeWmlsPromptResp) ;
	GoBackHistory() ;
}


void  jWap_HandleWmlPromptListener()
{	
	S16 required_input_modes[] = {INPUT_MODE_MULTITAP_UPPERCASE_ABC,
									INPUT_MODE_MULTITAP_LOWERCASE_ABC,
									INPUT_MODE_123,
									-1
									} ;
	U8* guiBuffer ;

	mmi_trace (1, "@@@ jWap_fetchUrl_entry_handler @@@") ;

	if(jc_wstrlen(g_pstJWapAppInfo->g_pstWMLPrompt->pmDefaultValue) != 0)
	{
		jc_wstrncpy(jWap_getInputBuff,g_pstJWapAppInfo->g_pstWMLPrompt->pmDefaultValue,jc_wstrlen(g_pstJWapAppInfo->g_pstWMLPrompt->pmDefaultValue)) ;
	}

	EntryNewScreen (SCR_ID_WAP_INPUT_BOX, NULL, jWap_HandleWmlPromptListener, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_INPUT_BOX) ;

	ShowCategory5Screen_ext(
		STR_ID_WAP_FETCH_URL, 0,
		STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
		INPUT_TYPE_ALPHANUMERIC_UPPERCASE|INPUT_TYPE_ALPHANUMERIC_LOWERCASE|INPUT_TYPE_NUMERIC,
		jWap_getInputBuff (), jWAP_MAX_INPUT_SIZE,
		NULL, 0, required_input_modes, NULL);
	ChangeTitle(0,(U8*)g_pstJWapAppInfo->g_pstWMLPrompt->pmMessage) ;
	SetLeftSoftkeyFunction (jWap_ok_wml_msg, KEY_EVENT_UP) ;
	SetCategory5RightSoftkeyFunction (jWap_Cancel_wml_msg, KEY_EVENT_UP) ;	
	draw_title() ;
}


void jWap_showDisconnectingScreen (void)
{
	U8 *guiBuffer;

	EntryNewScreen (SCR_ID_WAP_DISCONNECTING, JC_NULL, NULL, JC_NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_DISCONNECTING) ;


	ShowCategory165Screen (0, 0, \
		0, 0, \
		(UI_string_type)GetString (STR_GLOBAL_ABORTING), \
		IMG_GLOBAL_PROGRESS, NULL) ;


	ClearAllKeyHandler();	/* End key is still valid */
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
}

void jWap_exitDisconnectingScreen (void)
{
	mmi_trace(TRUE,"JDD_LOG: Func: %s eWapExitKeyPressed = %d", __FUNCTION__,
		g_pstJWapAppInfo->stWapState.eWapExitKeyPressed);
	if (E_WAP_EXIT_KEY_END == g_pstJWapAppInfo->stWapState.eWapExitKeyPressed)
	{
		DisplayIdleScreen () ;
	}
	else
	{
		GoBackHistory () ;
	}
	g_pstJWapAppInfo->stWapState.eWapExitKeyPressed = E_WAP_EXIT_KEY_NONE ;
}

JC_RETCODE	jwap_sendSelfEvent(JC_INT32 iEventType)
{    
	JC_RETCODE rCode = JC_OK ;
	JC_EVENT_DATA stEvent ;

	memset( &stEvent, 0, sizeof( JC_EVENT_DATA ) ) ;

	stEvent.srcTaskInfo.iTaskID   = 
	stEvent.srcTaskInfo.iAppID    = 
	stEvent.destTaskInfo.iTaskID  = 
	stEvent.destTaskInfo.iAppID   = E_TASK_BROWSER_UI ;
	stEvent.iEventType 		= iEventType ;

	rCode = jdd_QueueAlloc (&stEvent) ;
	if (JC_OK == rCode)
	{
		jdd_QueueSend (&stEvent) ;
		rCode = JC_OK;
	}
	else
	{
		jdd_QueueFree( &stEvent ) ;
	}
	return rCode ;
}

JC_RETCODE jWap_handleProgInd (ST_BE_PROGRESS_IND *pstProgInd)
{
	mmi_trace (1, "JDD_LOG: jWap_handleProgInd @@@") ;
	switch (pstProgInd->eDataConnectionStatus)
	{
		case E_DATA_CONN_CONNECTING_GPRS:
		case E_DATA_CONN_CONNECTING_CSD:
		case E_DATA_CONN_CONNECTING:
		case E_DATA_CONN_AUTHORIZING:
		case E_DATA_CONN_REGISTERING:
			g_pstJWapAppInfo->stWapState.eTrConnState = E_WAP_CONN_CONNECTING ;
			break ;
		case E_DATA_CONN_CONNECTED_GPRS:
		case E_DATA_CONN_CONNECTED_CSD:
		case E_DATA_CONN_CONNECTED:
			g_pstJWapAppInfo->stWapState.eTrConnState = E_WAP_CONN_CONNECTED ;
			break ;
		case E_DATA_CONN_FAIL_GPRS:
		case E_DATA_CONN_FAIL_CSD:
		case E_DATA_CONN_FAIL:
			g_pstJWapAppInfo->stWapState.eTrConnState = E_WAP_CONN_IDLE ;
			break ;
		case E_DATA_CONN_DISCONNECTING_GPRS:
		case E_DATA_CONN_DISCONNECTING_CSD:
		case E_DATA_CONN_DISCONNECTING:
		{
			mmi_trace (1, "JDD_LOG: DC: E_BE_STACK_DISCONNECTING @@@") ;
			/*g_pstJWapAppInfo->stWapState.eTrConnState = E_WAP_CONN_DISCONNECTING ;
			jWap_showDisconnectingScreen () ;*/
		}
		break ;

		case E_DATA_CONN_DISCONNECTED_GPRS:
		case E_DATA_CONN_DISCONNECTED_CSD:
		case E_DATA_CONN_DISCONNECTED:
		{
			mmi_trace (1, "JDD_LOG:  DC: E_BE_STACK_DISCONNECTED state [%d] stack status [%d]",
				pstProgInd->eDataConnectionStatus, pstProgInd->eStackStatus) ;
			/*if(g_pstJWapAppInfo->stWapState.eTrConnState == E_WAP_CONN_DISCONNECTING)
			{
				jWap_exitDisconnectingScreen () ;
			}
			g_pstJWapAppInfo->stWapState.eTrConnState = E_WAP_CONN_DISCONNECTED ;*/
		}
		break ;

		default:
		break ;
	}

	return (JC_OK) ;
}

 /***************************************************************************
 * All Global Function Definitions
 **************************************************************************/

JC_RETCODE jwap_app_callback (
						void						*pvArg,
						JC_UINT32				uiWindowID,
						E_BE_CALLBACK_TYPE 	eCallbackType,
						void 						*pvCallbackInfo)
{

	JC_RETCODE rRetCode ;
	rRetCode = JC_OK ;

	mmi_trace (0, "JDD_LOG: jwap_app_callback. Event: %d @@@", eCallbackType) ;

	switch (eCallbackType)
	{
		case E_BE_ERROR_LISTENER:
		{
			ST_BE_ERROR_LISTENER *pstErrorInfo = (ST_BE_ERROR_LISTENER *)pvCallbackInfo ;
			if (JC_NULL != pstErrorInfo)
			{
				mmi_trace (0, "JDD_LOG: E_BE_ERROR_LISTENER >> rCode is [%X] and eHttpStatus [%d]",
					-pstErrorInfo->rCode, pstErrorInfo->eHttpStatus) ;
				if (JC_ERR_BE_HTTP_STATUS == pstErrorInfo->rCode)
				{
					jWap_HTTPErrorHandler (pstErrorInfo->eHttpStatus) ;
				}
				else
				{
					jWap_ErrorHandler (pstErrorInfo->rCode) ;
				}
			}
		}
		break ;

		case E_BE_END_PAGE_LISTENER:
		{
			jWAP_SET_STATE (E_WAP_END_DISPLAY_RECVD) ;
			mmi_trace (0, "JDD_LOG: E_BE_END_PAGE_LISTENER and eWapExitKeyPressed [%d]",
				g_pstJWapAppInfo->stWapState.eWapExitKeyPressed) ;
			if (E_WAP_EXIT_KEY_END == g_pstJWapAppInfo->stWapState.eWapExitKeyPressed ||
					E_WAP_EXIT_KEY_QUIT == g_pstJWapAppInfo->stWapState.eWapExitKeyPressed)
			{
				//sending a self event as the de-init can not be done in this context
				jwap_sendSelfEvent(E_WAP_APP_EXIT);
				//WAPLaunchBrowserRsp(NULL);
			}
			else
			{
				extern ST_HTTP_DOWNLOAD_INFO *g_pstHTTP_DownLoad_info ;
				ST_BE_END_PAGE_INFO	*pstBEEndPageInfo = (ST_BE_END_PAGE_INFO *)pvCallbackInfo ;
				if (JC_NULL == g_pstHTTP_DownLoad_info)
				{
					jWap_endDispHandler (pstBEEndPageInfo) ;
				}
				else
				{
					jdd_AssertOnFail ((E_FALSE == g_pstHTTP_DownLoad_info->bHasEndPageBeenCalled), \
						"E_BE_END_PAGE_LISTENER: g_pstHTTP_DownLoad_info->bHasEndPageBeenCalled is E_TRUE") ;
					g_pstHTTP_DownLoad_info->bHasEndPageBeenCalled = E_TRUE ;
					
					if(JC_OK != g_pstHTTP_DownLoad_info->rCode)
					{						
						if (E_TRUE == g_pstHTTP_DownLoad_info->bHasUserConfirmed)
						{
							DisplayPopup ((PU8)GetString(STR_ID_WAP_UNKNOWN), \
										IMG_GLOBAL_WARNING, 1, 2000, 1) ;
						}
						HandleErrIndication();
					}					
				}
			}
		}
		break ;

		case E_BE_BEGIN_PAGE_LISTENER:
		{
			jWAP_SET_STATE (E_WAP_BEGIN_DISPLAY_RECVD) ;
			jWap_beginDispHandler () ;
		}
		break ;

		case E_BE_AUTHORIZATION_LISTENER:
		{
			jWap_authHandler ((ST_BE_PAGE_INFO *)pvCallbackInfo, pvArg) ;
		}
		break ;

		case E_BE_CONTENT_LISTENER:
		{
			mmi_trace (1, "JDD_LOG: E_BE_CONTENT_LISTENER begin@@@") ;
			rRetCode = jwap_DCHandleContentListener ((ST_BE_URL_IND*)pvCallbackInfo) ;
			mmi_trace (1, "JDD_LOG: E_BE_CONTENT_LISTENER END RetCode:%d", rRetCode) ;
		}
		break ;

		case E_BE_VSCROLL_LISTENER:
		{
			mmi_trace (1, "JDD_LOG: VSCROLL @@@") ;
			jWap_vscrollHandler ((ST_BE_SCROLL_INFO *) pvCallbackInfo) ;
		}
		break ;

		case E_BE_HSCROLL_LISTENER:
		{
			mmi_trace (1, "JDD_LOG: HSCROLL @@@") ;
		}
		break ;

		case E_BE_SCHEME_LISTENER:
		{
			ST_BE_SCHEME_LISTENER *pstScheme ;

			pstScheme = pvCallbackInfo ;
			mmi_trace (1, "JDD_LOG: E_BE_SCHEME_LISTENER begin") ;
			if (jdi_CUtilsTcsStrStartsWith(pstScheme->pmURI, "wtai://") ||
				jdi_CUtilsTcsStrStartsWith(pstScheme->pmURI, "WTAI://"))
			{
				jWap_MakeCallHandler(pvCallbackInfo) ;
			}
			else
			{
				DisplayPopupCallBack((PU8)GetString(STR_GLOBAL_INVALID), \
					IMG_GLOBAL_ERROR, 1, 2000, 1, jWap_PopupCallBack) ;
			}
			mmi_trace (1, "JDD_LOG: E_BE_SCHEME_LISTENER end") ;
		}
		break ;

		case E_BE_TITLE_LISTENER:
		{

			jWap_titleHandler ((JC_CHAR *) pvCallbackInfo) ;
		}
		break;
		case E_BE_WMLS_ALERT_LISTENER:
		{
			jWap_HandleWmlAlertListener((ST_BE_WMLS_ALERT*)pvCallbackInfo ) ;
		}
		break ;
		case E_BE_WMLS_CONFIRM_LISTENER:
		{
			g_pstJWapAppInfo->g_pstWMLConfirm = (ST_BE_WMLS_CONFIRM*)pvCallbackInfo ;
			jWap_HandleWmlConfirmListener() ;
			rRetCode = JC_ERR_WAIT_USER_ACTION  ;
		}
		break ;
		case E_BE_WMLS_PROMPT_LISTENER:
		{
			g_pstJWapAppInfo->g_pstWMLPrompt  = (ST_BE_WMLS_PROMPT*)pvCallbackInfo ;

			jWap_HandleWmlPromptListener() ;
			rRetCode = JC_ERR_WAIT_USER_ACTION  ;
		}
		break ;
		case E_BE_SECURITY_IND_LISTENER:
		{
			mmi_trace(1,"JDD_LOG: security ind received");	
		}
		break;

		case E_BE_TIMER_LISTENER:
		{
#if 0			
			JC_EVENT_DATA	stEventData ;

			jc_memset (&stEventData, 0, sizeof (JC_EVENT_DATA)) ;
			stEventData.destTaskInfo.iAppID = (JC_INT32) E_TASK_BROWSER_CORE ;
			stEventData.destTaskInfo.iTaskID = (JC_INT32) E_TASK_BROWSER_CORE ;
			stEventData.srcTaskInfo.iAppID = (JC_INT32) E_TASK_BROWSER_CORE ;
			stEventData.srcTaskInfo.iTaskID = (JC_INT32) E_TASK_BROWSER_CORE ;
			stEventData.iEventType = (JC_INT32) E_BE_TIMER ;
			stEventData.uiEventInfoSize = sizeof (ST_BE_TIMER_RESP) ;
			rRetCode = jdd_QueueAlloc (&stEventData) ;
			if (JC_OK == rRetCode)
			{
				jc_memcpy ((ST_BE_TIMER_RESP *) stEventData.pEventInfo, \
					(ST_BE_TIMER_RESP *) pvCallbackInfo, sizeof (ST_BE_TIMER_RESP)) ;
				jdd_QueueSend (&stEventData) ;
				rRetCode = JC_ERR_ASYNC_CALL ;
			}
#endif
			rRetCode = JC_OK;
		}
		break ;

		case E_BE_PROGRESS_LISTENER:
		{
			rRetCode = jWap_handleProgInd (pvCallbackInfo) ;
		}
		break ;
#ifdef CS_DC_ENABLE

		case E_BE_DOWNLOAD_LISTENER:
		{
			JC_RETCODE rCode= JC_OK;
			mmi_trace (1, "jdc_log: E_BE_DOWNLOAD_LISTENER called" );
			rCode = jwap_DCHandleDownloadListener ((ST_BE_URL_IND*)pvCallbackInfo) ;
			mmi_trace (1, "jdc_log: E_BE_DOWNLOAD_LISTENER call end rCode[%d]", rCode);
		}
		break;
#endif
		default:
		break ;
	}
	mmi_trace (1, "JDD_LOG: jwap_app_callback. Event: end@@@" );
	return rRetCode ;

}
 
#endif

