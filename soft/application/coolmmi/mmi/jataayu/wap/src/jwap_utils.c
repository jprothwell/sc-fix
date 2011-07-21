#define MODULE_NAME "JWAP"
#define FILE_NAME "jwap_utils.c"
 /***************************************************************************
 *
 * File Name : jwap_utils.c
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
 * @file		jwap_utils.c
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

#include "filemgr.h"
#include "fmt_def.h"
#include "fs.h"


#include "jdi_displayinfo.h"
#include "jdi_mimeinfo.h"
#include "jdi_constants.h"
#include "jwap_mmi_cb.h"
#include "jdi_cutils.h"
#include <jdi_urlformation.h>
#include <jdi_constants.h>
#include "jmms_init.h"
#include "commonscreens.h"
#include "ts.h"
#include "jdi_cutils_uriparse.h"
#include "wgui_status_icons.h"
#include "jwap_push.h"

 /***************************************************************************
 * Macros
 **************************************************************************/

#define WWW_AUTH_HDR    "WWW-Authenticate"
#define DIGEST_STR		"Digest" 			/* One space is present at the end */
#define BASIC_STR		"Basic " 			/* One space is present at the end */
#define REALAM_STR      "realm"
#define DOMAIN_STR      "domain"
#define NONCE_STR       "nonce"
#define QOP_STR         "qop"
#define ALGORITHM_STR   "algorithm"
#define OPAQUE_STR      "opaque"

 /***************************************************************************
 * Global variables declarations
 **************************************************************************/

	
#define EOS					'\0'
#define TOTAL_VALID_SCHEME   3
#define LOWER_CASE_ALPHA_BEGIN				97
#define LOWER_CASE_ALPHA_END				122
#define UPPER_CASE_ALPHA_BEGIN				65
#define UPPER_CASE_ALPHA_END				90
#define NUMERIC_BEGIN						48
#define NUMERIC_END							57
#define IPADDRESS_NUMERIC_LENGTH			4

 static const JC_INT8 * const pmaValidSchemeList [TOTAL_VALID_SCHEME] = {"file","http","https"} ;


 /***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

 /***************************************************************************
 * All Local Function Definitions
 ***************************************************************************/

void jWap_getDigestAuthContentInfo (JC_INT8 *pcSourceContent, JC_INT8 *pcDestContent,  
                  JC_INT8 **ppcHeaders)
{
  JC_UINT8  ucCount ;
  JC_INT8   *pcHeaders ;
  JC_BOOLEAN  bQuotes = E_FALSE ;

  pcHeaders = *ppcHeaders ;
  pcHeaders = pcHeaders + jc_strlen(pcSourceContent);
  ucCount = 0 ;
  while (EOS_TOKEN != *pcHeaders && EQUAL_TOKEN != *pcHeaders)
  {
    pcHeaders++ ;
  }
  if (EOS != *pcHeaders)
  {
    pcHeaders++ ;
  }
  while (EOS != *pcHeaders && SPACE_TOKEN == *pcHeaders)
  {
    pcHeaders++ ;
  }
  if (EOS != *pcHeaders && QUOTE_TOKEN == *pcHeaders)
  {
    bQuotes = E_TRUE ;
    pcHeaders++ ;
  }
  while (EOS != *pcHeaders && COMMA_TOKEN != *pcHeaders && SEMICOLON_TOKEN != *pcHeaders)
  {
    pcDestContent [ucCount] = *pcHeaders ;
    pcHeaders++ ;
    ucCount++ ;
    if (COMMA_TOKEN == *pcHeaders || SEMICOLON_TOKEN == *pcHeaders)
    {
      if (E_TRUE == bQuotes)
      {
        pcDestContent [ucCount] = *pcHeaders ;
        pcHeaders++ ;
        ucCount++ ;
      }
    }
    if (QUOTE_TOKEN == *pcHeaders)
    {
      bQuotes = E_FALSE ;
      pcHeaders++ ;
    }
  }
  *ppcHeaders = pcHeaders ;
  return ;
}


void jWap_copyDigestAuthInfo (ST_AUTH_HEADERS *pstAuthInfo, JC_INT8 *pcHeaders)
{

  if (JC_NULL != pcHeaders)
  {
	while (EOS_TOKEN != *pcHeaders)
	{
	  if (0 == jc_strncasecmp (QOP_STR, pcHeaders, jc_strlen (QOP_STR)))
	  {
		jWap_getDigestAuthContentInfo (QOP_STR, pstAuthInfo->acQOP, &pcHeaders) ;
	  }
	  if (0 == jc_strncasecmp (ALGORITHM_STR, pcHeaders, jc_strlen (ALGORITHM_STR)))
	  {
		jWap_getDigestAuthContentInfo (ALGORITHM_STR, pstAuthInfo->acAlgorithm, &pcHeaders) ;
	  }
	  if (0 == jc_strncasecmp (REALAM_STR, pcHeaders, jc_strlen(REALAM_STR)))
	  {
		jWap_getDigestAuthContentInfo (REALAM_STR, pstAuthInfo->acRealm, &pcHeaders) ;
	  }
	  if (0 == jc_strncasecmp (DOMAIN_STR , pcHeaders, jc_strlen(DOMAIN_STR)))
	  {
		jWap_getDigestAuthContentInfo (DOMAIN_STR, pstAuthInfo->acDomain, &pcHeaders) ;
	  }
	  if (0 == jc_strncasecmp (NONCE_STR , pcHeaders, jc_strlen(NONCE_STR)))
	  {
		jWap_getDigestAuthContentInfo (NONCE_STR, pstAuthInfo->acNOnce, &pcHeaders) ;
	  }
	  if (0 == jc_strncasecmp (OPAQUE_STR , pcHeaders, jc_strlen (OPAQUE_STR)))
	  {
		jWap_getDigestAuthContentInfo (OPAQUE_STR, pstAuthInfo->acOpaque, &pcHeaders) ;
	  }
	 
	  if (EOS != *pcHeaders)
	  {
		pcHeaders++ ;
	  }
	  
	}
  }
  return ;
}



void jWap_freePostData (ST_BE_POST_DATA  *pstPostData)
{
	if (pstPostData)
	{
		if (pstPostData->bIsFileData)
		{
			jdd_MemFree (pstPostData->uPostData.pmFileName) ;
		}
		else
		{
			jdd_MemFree (pstPostData->uPostData.pcPostData) ;
		}

		jdd_MemFree (pstPostData) ;
	}
}	
	
	
void jWap_freeMimeHeaders (ST_MIME_HEADERS *pstMimeHeaders)
{
	ST_MIME_HEADERS 		*pstTempMimeHeaders ;
	ST_PARAMETER_LIST 		*pstTempParamList, *pstParamList ;
	
		
	while (pstMimeHeaders) 
	{
		if (pstMimeHeaders->pcName)
		{
			jdd_MemFree (pstMimeHeaders->pcName) ;
		}

		if (E_HEADER_TYPE_STRING == pstMimeHeaders->eHeaderType)
		{
			if (pstMimeHeaders->uHeaderValue.pcValue)
				jdd_MemFree (pstMimeHeaders->uHeaderValue.pcValue) ;	
		}
		else if (E_HEADER_TYPE_TCHAR == pstMimeHeaders->eHeaderType)
		{
			if (pstMimeHeaders->uHeaderValue.pmValue)
				jdd_MemFree (pstMimeHeaders->uHeaderValue.pmValue) ;	
		}

		pstParamList = pstMimeHeaders->pstParameterList ;
		while (pstParamList)
		{
			if (pstParamList->pcName)
			{
				jdd_MemFree (pstParamList->pcName) ;
			}

			if (E_HEADER_TYPE_STRING == pstParamList->eType)
			{
				if (pstParamList->uHeaderValue.pcValue)
					jdd_MemFree (pstParamList->uHeaderValue.pcValue) ;	
			}
			else if (E_HEADER_TYPE_TCHAR == pstParamList->eType)
			{
				if (pstParamList->uHeaderValue.pmValue)
					jdd_MemFree (pstParamList->uHeaderValue.pmValue) ;	
			}

			pstTempParamList = pstParamList ;
			pstParamList = pstParamList->pstNext ;
			jdd_MemFree (pstTempParamList) ;
		}

		pstTempMimeHeaders = pstMimeHeaders ;
		pstMimeHeaders = pstMimeHeaders->pstNext ;
		jdd_MemFree (pstTempMimeHeaders) ;
	}
	
}


void jWap_freeFetchInfo (ST_BE_FETCH_REQ* pStFetchInfo)
{
	if (!g_pstJWapAppInfo->pstFetchReq)
	{
		goto ERROR_HANDLER ;
	}

	if (g_pstJWapAppInfo->pstFetchReq->pmURI)
	{
		jdd_MemFree (g_pstJWapAppInfo->pstFetchReq->pmURI) ;
	}

	if (g_pstJWapAppInfo->pstFetchReq->pstPostData)
	{
		jWap_freePostData (g_pstJWapAppInfo->pstFetchReq->pstPostData) ;
	}

	if (g_pstJWapAppInfo->pstFetchReq->pstAuthInfo)
	{
		jdd_MemFree (g_pstJWapAppInfo->pstFetchReq->pstAuthInfo) ;
	}

	
	if (g_pstJWapAppInfo->pstFetchReq->pstMimeHeaders)
	{
		jWap_freeMimeHeaders (g_pstJWapAppInfo->pstFetchReq->pstMimeHeaders) ;
	}
	
	jdd_MemFree(g_pstJWapAppInfo->pstFetchReq);
	/* Default error action is to return silently */
ERROR_HANDLER:
	return ;
}


 /***************************************************************************
 * All Global Function Definitions
 **************************************************************************/
 void jWap_setSoftKeys ()
{
	switch (jWAP_GET_STATE())
		{
		case E_WAP_END_DISPLAY_RECVD:
			if (!jWap_isBack ())
			{
				//ChangeRightSoftkey (STR_GLOBAL_QUIT, 0) ;
				/* This is the first page. Remove LSK */	
				ClearKeyHandler (KEY_RSK, KEY_EVENT_DOWN) ;
				ChangeRightSoftkey (0, 0) ;

				ChangeLeftSoftkey (STR_GLOBAL_OPTIONS, 0) ;
				SetLeftSoftkeyFunction (jWap_options_handler, KEY_EVENT_UP) ;
			}
			else
			{
				/* There are more than 1 page. Set to "Back"  */
				ChangeRightSoftkey (STR_GLOBAL_BACK, 0) ;	
				SetRightSoftkeyFunction (jWap_back, KEY_EVENT_UP);

				ChangeLeftSoftkey (STR_GLOBAL_OPTIONS, 0) ;
				SetLeftSoftkeyFunction (jWap_options_handler, KEY_EVENT_UP) ;
			}
		break ;

		default:
		case E_WAP_BEGIN_DISPLAY_RECVD:
			/* There are more than 1 page. Set to "Back"  */
			ChangeRightSoftkey (STR_GLOBAL_CANCEL, 0) ;	
			SetRightSoftkeyFunction (jWap_priMmi_cancel_handler, KEY_EVENT_UP) ;

			ChangeLeftSoftkey (0, 0) ;
			SetLeftSoftkeyFunction (0, KEY_EVENT_UP) ;
			
			break ;
		
		}
}
 
void jWap_freePageInfo (ST_BE_PAGE_INFO* pstPageInfo)
{
	if (pstPageInfo)
	{
		if (pstPageInfo->pmURI)
		{
			jdd_MemFree (pstPageInfo->pmURI) ;
		}

		if (pstPageInfo->pmTitle)
		{
			jdd_MemFree (pstPageInfo->pmTitle) ;
		}

		if (pstPageInfo->pcPostData)
		{
			jdd_MemFree (pstPageInfo->pcPostData) ;
		}

		jWap_freeMimeHeaders (pstPageInfo->pstHeaders) ;

		jdd_MemFree (pstPageInfo) ;
	}
}


/* This function copies the page info sent to WWW authentication callback. 
    This function can also be used to copy Proxy authenticatoin info by checking
    the header type when checking the mime headers for proxy authentication header
*/
   
INT jWap_copyAuthInfo (ST_BE_PAGE_INFO* pstPageInfo)
{
	INT	iErr ;
	ST_MIME_HEADERS *pstHeaders ; 
	JC_INT8			*pcHeadVal ;

	ASSERT (pstPageInfo) ;

	if (g_pstJWapAppInfo->pstFetchReq)
	{
		jWap_freeFetchInfo (g_pstJWapAppInfo->pstFetchReq) ;
		g_pstJWapAppInfo->pstFetchReq = JC_NULL ;
	}

	g_pstJWapAppInfo->pstFetchReq = (ST_BE_FETCH_REQ*)jdd_MemAlloc (1, sizeof (ST_BE_FETCH_REQ)) ;	
	if (!g_pstJWapAppInfo->pstFetchReq)
	{
		iErr = JC_ERR_MEMORY_ALLOCATION ;
		goto ERROR_HANDLER ;
	}
	else
	{
		iErr = JC_OK ;
	}
	jc_memset (g_pstJWapAppInfo->pstFetchReq, 0, sizeof (ST_BE_FETCH_REQ)) ;

	/*	Copy URL if present. Otherwise, exit */
	if (!pstPageInfo->pmURI)
	{
		iErr = JC_ERR_INVALID_PARAMETER ;
		goto ERROR_HANDLER ;
	}

	g_pstJWapAppInfo->pstFetchReq->pmURI = (JC_CHAR*)jdd_MemAlloc (sizeof (JC_CHAR), \
		jc_wstrlen ( pstPageInfo->pmURI) + 1) ;
	jc_wstrcpy (g_pstJWapAppInfo->pstFetchReq->pmURI, pstPageInfo->pmURI) ;

	{
		JC_INT8  *pcFName = JC_NULL ;

		jdi_CUtilsTcharToChar (JC_NULL, g_pstJWapAppInfo->pstFetchReq->pmURI, &pcFName) ;
		if (pcFName) 
		{
			mmi_trace (1, "@@@ Auth URL: %s @@@", pcFName) ;
			jdd_MemFree (pcFName) ;
		}
	}
	
	/* Copy Post Data if present */
	if (pstPageInfo->pcPostData && pstPageInfo->uiPostLen)
	{
		g_pstJWapAppInfo->pstFetchReq->pstPostData = (ST_BE_POST_DATA*)jdd_MemAlloc (1, sizeof (ST_BE_POST_DATA)) ;	
		if (!g_pstJWapAppInfo->pstFetchReq->pstPostData)
		{
			iErr = JC_ERR_MEMORY_ALLOCATION ;
			goto ERROR_HANDLER ;
		}

		jc_memset (g_pstJWapAppInfo->pstFetchReq->pstPostData, 0, sizeof (ST_BE_POST_DATA)) ;

		g_pstJWapAppInfo->pstFetchReq->pstPostData->uiPostLen = pstPageInfo->uiPostLen ;
		g_pstJWapAppInfo->pstFetchReq->pstPostData->bIsFileData = E_FALSE ;
		g_pstJWapAppInfo->pstFetchReq->pstPostData->uPostData.pcPostData = \
			(JC_INT8*)jdd_MemAlloc (sizeof (JC_INT8), pstPageInfo->uiPostLen) ;
		if (!g_pstJWapAppInfo->pstFetchReq->pstPostData->uPostData.pcPostData)
		{
			iErr = JC_ERR_MEMORY_ALLOCATION ;
			goto ERROR_HANDLER ;
		}
		
		jc_memcpy (g_pstJWapAppInfo->pstFetchReq->pstPostData->uPostData.pcPostData, \
			pstPageInfo->pcPostData, pstPageInfo->uiPostLen) ;
	}

	/* Copy Authentication Headers */
	/* Only WWW Authentication is taken care here. If Proxy Authentication is required, need to 
		check for the Proxy authentication header */
	pstHeaders = pstPageInfo->pstHeaders ;
	while (JC_NULL != pstHeaders && JC_NULL != pstHeaders->pcName 
         && 0 != jc_strcasecmp (WWW_AUTH_HDR, pstHeaders->pcName))
    {
    	pstHeaders = pstHeaders->pstNext ;
    }

	if (!pstHeaders || !pstHeaders->uHeaderValue.pcValue)
	{
		iErr = JC_ERR_INVALID_PARAMETER ;
		mmi_trace (1, "@@@ Auth Header Not Present @@@") ;
		goto ERROR_HANDLER ;
	}

	g_pstJWapAppInfo->pstFetchReq->pstAuthInfo = (ST_BE_AUTH_INFO *)jdd_MemAlloc (1, sizeof (ST_BE_AUTH_INFO)) ;

	if (!g_pstJWapAppInfo->pstFetchReq->pstAuthInfo)
	{
		iErr = JC_ERR_MEMORY_ALLOCATION ;
		goto ERROR_HANDLER ;
	}

	jc_memset (g_pstJWapAppInfo->pstFetchReq->pstAuthInfo, 0, sizeof (ST_BE_AUTH_INFO)) ;
	pcHeadVal = pstHeaders->uHeaderValue.pcValue ;

	mmi_trace (1, "@@@ WWW-Authenticate: %s", pcHeadVal) ;
	if (!jc_strncmp (DIGEST_STR, pcHeadVal, jc_strlen (DIGEST_STR)))
	{
		mmi_trace (1, "@@@ Digest Auth @@@ ") ;
		pcHeadVal = pcHeadVal + jc_strlen(DIGEST_STR) ;
		jWap_copyDigestAuthInfo (g_pstJWapAppInfo->pstFetchReq->pstAuthInfo, pcHeadVal) ;
		g_pstJWapAppInfo->pstFetchReq->pstAuthInfo->eAuthType = E_AUTH_DIGEST ;
	}
	else// if (!jc_strncmp (BASIC_STR, pcHeadVal, jc_strlen (BASIC_STR)))  //Code commented as problem in wap gateway sending data with different pcHeadVal
	{
		mmi_trace (1, "@@@ Basic Auth @@@") ;
		g_pstJWapAppInfo->pstFetchReq->pstAuthInfo->eAuthType = E_AUTH_BASIC ;
	}
	//Code commented as problem in wap gateway sending data with different pcHeadVal
	/*
	else
	{
		mmi_trace (1, "@@@ Invalid HTTP Header Value @@@") ;
		iErr = JC_ERR_BE_CONTENT_HANDLED ;
		goto ERROR_HANDLER ;
	}
	*/
	return (iErr) ;

ERROR_HANDLER:

	jWap_freeFetchInfo (g_pstJWapAppInfo->pstFetchReq) ;
	g_pstJWapAppInfo->pstFetchReq = JC_NULL ;
	return (iErr) ;
}


/***************************************************************************
 * All Global Function Definitions
 **************************************************************************/
void	jWap_Register_KeyHandler_Funcs ()
{
	SetKeyHandler(jWap_keyUpArrHandler, KEY_UP_ARROW, KEY_EVENT_UP) ;
	SetKeyHandler(jWap_keyDownArrHandler, KEY_DOWN_ARROW, KEY_EVENT_UP) ;
	SetKeyHandler(jWap_keyLeftArrHandler, KEY_LEFT_ARROW, KEY_EVENT_UP) ;
	SetKeyHandler(jWap_keyRightArrHandler, KEY_RIGHT_ARROW, KEY_EVENT_UP) ;
	SetKeyHandler(jWap_keyEnterHandler, KEY_ENTER, KEY_EVENT_UP) ;
	//SetKeyHandler(jWap_keyEndHandler, KEY_END, KEY_EVENT_DOWN) ;
#if defined(__MMI_TOUCH_SCREEN__)
	mmi_wgui_register_pen_down_handler (jWap_PenDownHandler) ;
	//mmi_wgui_register_pen_up_handler (jWap_PenUpHandler) ;
	mmi_wgui_register_pen_move_handler (jWap_PenMoveHandler) ;
#endif
	//SetCenterSoftkeyFunction (jWap_keyEnterHandler, KEY_EVENT_UP) ;
	//set_softkey_function(jWap_keyEnterHandler, KEY_EVENT_UP, MMI_CENTER_SOFTKEY) ;
}

#if defined(__MMI_TOUCH_SCREEN__)
void	jWap_PenDownHandler (mmi_pen_point_struct stPenCoordinates)
{
	jWap_Generic_Pen_Handler (KEY_STAR, &stPenCoordinates) ;
}
void jWap_PenUpHandler (mmi_pen_point_struct stPenCoordinates)
{
	jWap_Generic_Pen_Handler (KEY_ENTER, &stPenCoordinates) ;
}
void jWap_PenMoveHandler (mmi_pen_point_struct stPenCoordinates)
{
	jWap_Generic_Pen_Handler (KEY_POUND, &stPenCoordinates) ;
}
#endif
/**
* @brief <appCheckValidScheme : Checking for valid Scheme.
* @param[in] <ParameterName> <Description>
* @param[in] <ParameterName> <Description>
* @param[in,out] <ParameterName> <Description>
* @param[out] <ParameterName> <Description>
* @return <Description about the return value>.
*
* <Here give detail description about the function>
*/

JC_BOOLEAN appCheckValidScheme (JC_INT8 *pcURLText, JC_INT32 iSchemeIndex)
{
	int i ;
	JC_BOOLEAN  bValid = E_TRUE ;
	
	if (iSchemeIndex > 0)
	{
		if (iSchemeIndex < 4)
		{
			bValid = E_FALSE ;
		}
		else
		{
			bValid = E_FALSE ;
			for (i = 0; i < TOTAL_VALID_SCHEME; i++)
			{
				if (0 == jc_strncmp (pcURLText, pmaValidSchemeList[i], iSchemeIndex))
				{
					bValid = E_TRUE ;
					i = TOTAL_VALID_SCHEME ;
				}
			}
			i = iSchemeIndex++ ;
			if (pcURLText [i] != ':' && pcURLText [++i] != '/' && pcURLText [++i] != '/')
			{
				bValid = E_FALSE ;
			}
		}
	}
	else
	{
		bValid = E_TRUE ;
	}
	return bValid ;
}

/**
* @brief <Brief description about the function.
* @param[in] <ParameterName> <Description>
* @param[in] <ParameterName> <Description>
* @param[in,out] <ParameterName> <Description>
* @param[out] <ParameterName> <Description>
* @return <Description about the return value>.
*
* <Here give detail description about the function>
*/


JC_BOOLEAN appCheckValidPort (JC_INT8 *pcURLText, JC_INT32 iStartIndex, JC_INT32 iEndIndex)
{
	JC_INT32 i = 0 ;
	JC_BOOLEAN bIsValid = E_TRUE ;
	if (iStartIndex > iEndIndex)
	{
		bIsValid = E_FALSE ;
	}
	else if (iStartIndex > 0 && iEndIndex > 0)
	{
		for (i = iStartIndex; i <= iEndIndex; i++)
		{
			if (! (pcURLText [i] >= NUMERIC_BEGIN && pcURLText [i] <= NUMERIC_END))
			{
				bIsValid = E_FALSE ;
			}
		}
	}
	return bIsValid ;
}

/**
* @brief <Brief description about the function.
* @param[in] <ParameterName> <Description>
* @param[in] <ParameterName> <Description>
* @param[in,out] <ParameterName> <Description>
* @param[out] <ParameterName> <Description>
* @return <Description about the return value>.
*
* <Here give detail description about the function>
*/

JC_BOOLEAN appCheckValidIpAddress (JC_INT8 *pcURLText, JC_INT32 iStartIndex, JC_INT32 iEndIndex)
{
	JC_INT32 i ;
	JC_UINT8 numericValue [IPADDRESS_NUMERIC_LENGTH] ;
	JC_INT32 iIndex ;
	JC_INT32 iDotsCount = 0 ;
	JC_INT32 iActual_value ;
	JC_BOOLEAN bIsValid = E_TRUE ;
	
	iIndex = 0 ;
	jc_memset (numericValue, 0, IPADDRESS_NUMERIC_LENGTH) ; 
	for (i=iStartIndex; i <= iEndIndex; i++)
	{
		if (pcURLText [i] == '.')
		{
			iDotsCount++ ;
			iActual_value = jc_atoi ((JC_INT8 *)numericValue) ;  
			if (iActual_value > 255 || iActual_value < 0)
			{
				bIsValid = E_FALSE ;
			}
			if (iDotsCount == 1)
			{
				if (iActual_value == 0)              
				{
					bIsValid = E_FALSE ;
				}
			}
			jc_memset (numericValue, 0, IPADDRESS_NUMERIC_LENGTH) ; 
			iIndex = 0 ;
		}
		else if ((pcURLText [i] >= NUMERIC_BEGIN && pcURLText [i] <= NUMERIC_END))
		{
			if (iIndex == IPADDRESS_NUMERIC_LENGTH)
			{
				bIsValid = E_FALSE ;
			}
			numericValue [iIndex++] = (JC_UINT8) pcURLText[i] ;
		}
	}
	if (iDotsCount != 3)
	{
		bIsValid = E_FALSE ;
	}
	return bIsValid ;
}

/**
* @brief <Brief description about the function.
* @param[in] <ParameterName> <Description>
* @param[in] <ParameterName> <Description>
* @param[in,out] <ParameterName> <Description>
* @param[out] <ParameterName> <Description>
* @return <Description about the return value>.
*
* <Here give detail description about the function>
*/

JC_BOOLEAN appCheckValidHostname (JC_INT8 *pcURLText, JC_INT32 iStartIndex, JC_INT32 iEndIndex)
{
	JC_INT16 isIpAddress ;
	JC_INT32 iDotsCount ;
	JC_BOOLEAN bIsValid = E_TRUE ;
	int i ;
	
	if (iStartIndex >= 0 && iEndIndex >= 0)
	{
		isIpAddress = 1 ;
		iDotsCount = 0 ;
		if (pcURLText [iStartIndex] == '.' || pcURLText [iStartIndex] == '-')
		{
			bIsValid = E_FALSE ;
		}
		if (pcURLText [iEndIndex] == '.' || pcURLText [iEndIndex] == '-')
		{
			bIsValid = E_TRUE ;
		}
		for (i = iStartIndex; i <= iEndIndex; i++)
		{
			if (pcURLText [i] != '.' && pcURLText [i] != '-')
			{
				if (! (pcURLText [i] >= NUMERIC_BEGIN && pcURLText [i] <= NUMERIC_END))
				{
					isIpAddress = 0 ;
					if (! ((pcURLText [i] >= LOWER_CASE_ALPHA_BEGIN && pcURLText [i] <= LOWER_CASE_ALPHA_END) || (pcURLText [i] >= UPPER_CASE_ALPHA_BEGIN && pcURLText [i] <= UPPER_CASE_ALPHA_END)) )
					{
						bIsValid = E_FALSE ;
					}
				}
			}
			else 
			{
				if (pcURLText [i] == '.')
				{
					iDotsCount++ ;
				}
				if (pcURLText [i + 1] == '.' || pcURLText [i + 1] == '-')
				{
					bIsValid = E_FALSE ;
				}
			}
		}
		if (isIpAddress == 1)
		{
			if (iDotsCount == 3)
			{
				bIsValid = appCheckValidIpAddress (pcURLText, iStartIndex, iEndIndex) ;
			}
			else
			{
				bIsValid = E_FALSE ;
			}
		}
	}
	else
	{
		bIsValid = E_FALSE ;
	}
	return bIsValid ;
}

/**
* @brief <Brief description about the function.
* @param[in] <ParameterName> <Description>
* @param[in] <ParameterName> <Description>
* @param[in,out] <ParameterName> <Description>
* @param[out] <ParameterName> <Description>
* @return <Description about the return value>.
*
* <Here give detail description about the function>
*/

JC_BOOLEAN IsValidUrl (JC_INT8 *pcURLText, JC_BOOLEAN *pbIsFileScheme)
{
	JC_INT32 iLength ;
	JC_INT32 iQueryIndex ;
	JC_INT32 iSchemeIndex ;
	JC_INT32 iHostIndex ;
	JC_INT32 iHostEndIndex ;
	JC_INT32 iPortIndex ;
	JC_INT32 iPasswordIndex ;
	JC_INT32 iPathIndex ;
	JC_INT32 iAtIndex ;
	JC_INT32 i ;
	JC_BOOLEAN bisValid = E_TRUE ;
	JC_BOOLEAN bIsFileScheme = E_FALSE ;
	
	iQueryIndex = iSchemeIndex = iHostIndex = iPortIndex = iAtIndex = iPathIndex = iPasswordIndex = -1 ;
	iHostEndIndex = 0 ;
	iLength = jc_strlen ((const JC_INT8*)pcURLText) ;
	for (i=0; i < iLength; i++)
	{
		switch (pcURLText[i])
		{
			case '@':
				if ((iPasswordIndex > 0) && (iPathIndex < 0))
				{
					iHostIndex = i + 1 ;
					iAtIndex = i ;
				}
				break ;

			case '?':
				iQueryIndex = i + 1 ;
				break ;

			case ':':
				if (iPathIndex < 0)
				{
					if (pcURLText [i + 1] == '/' && pcURLText [i + 2] == '/')
					{
						if (iSchemeIndex < 0)
						{
							iSchemeIndex = i ;
						}
						i += 2 ;
					}
					else 
					{
						if (iPasswordIndex < 0)
						{
							iPasswordIndex = i + 1 ;
						}
						else
						{
							iPortIndex = i + 1 ;
						}
					}
				}
				break ;

			case '/':
				if (iQueryIndex < 0 && iPathIndex < 0)
				{
					iPathIndex = i ;
				}
				break ;

			default:
				break ;
		}
		if (iQueryIndex > 0)
		{
			break ;
		}
	}
	if (iPortIndex < 0)
	{
		if (iPasswordIndex > 0 && iAtIndex < 0)
		{
			iPortIndex = iPasswordIndex ;
		}
	}
	if (iHostIndex < 0) 
	{
		if (iSchemeIndex > 0)
		{
			iHostIndex = iSchemeIndex + 3 ;
		}
		else
		{
			iHostIndex = 0 ;
		}
	}
	
	if (iPathIndex < 0)
	{
		iPathIndex = iLength - 1 ;
		iHostEndIndex = (iQueryIndex > 0)? (iQueryIndex - 2): (iLength - 1) ;
	}
	else
	{
		iHostEndIndex = iPathIndex - 1 ;
	}

	if ((0 <= iSchemeIndex) &&
		(jc_strncasecmp ((const JC_INT8*)pcURLText, "http", iSchemeIndex - 1) != 0) &&
		(jc_strncasecmp ((const JC_INT8*)pcURLText, "https", iSchemeIndex - 1) != 0) &&
		(jc_strncasecmp ((const JC_INT8*)pcURLText, "file", iSchemeIndex - 1) != 0))
	{
		if (pcURLText [iSchemeIndex] != ':' && pcURLText [++iSchemeIndex] != '/' && pcURLText [++iSchemeIndex] != '/')
		{
			bisValid = E_FALSE ;
		}
		else
		{
			bisValid =  E_TRUE ;
			bIsFileScheme = E_TRUE ;
			if (JC_NULL != pbIsFileScheme)
			{
				*pbIsFileScheme = E_TRUE ;
			}
		}
	}
	if (E_FALSE == bIsFileScheme)
	{
		if (appCheckValidScheme (pcURLText, iSchemeIndex) == 0)
		{
			bisValid = E_FALSE ;
		}
		if (iPortIndex > 0)
		{
			if (appCheckValidPort (pcURLText, iPortIndex, iPathIndex - 1) == 0)
			{
				bisValid =  E_FALSE ;
			}
		}
		if (iPortIndex < 0)
		{
			if (iHostIndex < iHostEndIndex)
			{
				if (appCheckValidHostname (pcURLText, iHostIndex, iHostEndIndex) == 0)
				{
					bisValid =  E_FALSE ;
				}
			}
		}
		else
		{
			if (appCheckValidHostname (pcURLText, iHostIndex, iPortIndex - 2) == 0)
			{
				bisValid =  E_FALSE ;
			}
		}
	}
		
	return bisValid  ;

}

/**
 * @brief This function Remove leading and trailing spaces of a word in JC_INT8 format
 * @param[in] pucSourceWord : Source Word in JC_INT8 format
 * @param[oot] ppucDestWord : Desination Word in JC_INT8 format
 
 * @return JC_OK for successful operation else get error in memory allocation.
 *
 * This function checks the validation of Phone number.
 */
 
JC_RETCODE TruncateLeadingAndTrailingSpaces (JC_CHAR *pucSourceWord, JC_CHAR **ppucDestWord)
{
	JC_RETCODE rCode   = JC_OK ;
	JC_CHAR  *pucSourceWord1 = pucSourceWord ;
	JC_CHAR  *pucDestWord = NULL ;
	JC_UINT32 uiWordLen  = 0;
	JC_UINT32 uiLeadingSpaces  = 0;
		

	if (JC_NULL != pucSourceWord)
	{
		pucSourceWord1 = pucSourceWord ;
		while (' ' == *pucSourceWord1)
		{
			pucSourceWord1++ ;
			uiLeadingSpaces ++;
		}
		uiWordLen = 0 ;
		
		uiWordLen = jc_wstrlen(pucSourceWord);
  
		while (' ' == *(pucSourceWord + uiWordLen - 1) && uiWordLen > 0 )
		{
			uiWordLen-- ;
		}

		if( 0 != uiWordLen)
		{
			uiWordLen = uiWordLen - uiLeadingSpaces;
			pucDestWord = (JC_CHAR*) jdd_MemAlloc (uiWordLen + 1, sizeof (JC_CHAR)) ;
			if(NULL == pucDestWord)
			{
				rCode = JC_ERR_MEMORY_ALLOCATION ;
			}
		}
		else
		{
			pucDestWord = NULL;
			rCode = JC_OK ;
		}

		if (JC_NULL != pucDestWord)
		{
			jc_wstrncpy (pucDestWord, pucSourceWord1, uiWordLen) ;
		}

		*ppucDestWord = pucDestWord ;
	}
	return rCode ;
}

/**
 * @brief This function Remove Duplicate Character in Shortcut
 * @param[in] ppmStringText : Specifies String
 * @param[in] ucSeparateText : Specifies Separtion of Text
 * @return Success : E_TRUE else E_FALSE
 *
 * This function checks the validation of Phone number.
 */

JC_RETCODE appRemoveDuplicateWord (JC_CHAR **ppmStringText, JC_UINT8 ucSeparateText)
{
	JC_RETCODE	rCode			= JC_OK ;
	JC_UINT32	uiStrLen ;
	JC_UINT32	uiStrSize		= 0;
	JC_UINT32	uiWordCount		= 0 ;
	JC_CHAR		*pmSourceText	= *ppmStringText ;
	JC_INT8		*pcText ;
	JC_INT8		*pcTempText		= JC_NULL ;
	JC_INT8		*pcMemText		= JC_NULL ;
	JC_INT8		*pcSourceWord ;
	JC_INT8		*pcDestWord ;
	JC_BOOLEAN	bDuplicateWord ;
	typedef struct stwordlist
	{
		JC_INT8		*pcWord ;
		struct stwordlist  *pstNext ;
	} ST_WORD_LIST ;
	ST_WORD_LIST *pstWordList, *pstStartWordList, *pstTempWordList ;
	pstWordList = JC_NULL ;
	pstStartWordList = JC_NULL ;
	pstTempWordList = JC_NULL ;
	rCode = jdi_CUtilsTcharToChar (JC_NULL, pmSourceText, &pcText) ;
	if (JC_OK == rCode && JC_NULL != pcText)
	{
		pcTempText = pcText ;
		pcMemText = pcText ;
		uiStrLen = jc_strlen (pcText) ;
		while (0 != uiStrLen)
		{
			if (ucSeparateText != *pcText)
			{
				if (32 != *pcText)
				{
					uiStrSize++ ;
				}
				else
				{
					pcTempText++ ;
				}
				pcText++ ;	
			}
			else
			{
				pcText++ ;
				if (0 != uiStrSize)
				{
					pcSourceWord = (JC_INT8*) jdd_MemAlloc (uiStrSize + 1, sizeof (JC_INT8)) ;
					if (JC_NULL != pcSourceWord)
					{
						jc_strncpy (pcSourceWord , pcTempText, uiStrSize) ;
						rCode = appRemoveLeadTrailSpacesWord (pcSourceWord, &pcDestWord) ; 
						if (JC_NULL == pstStartWordList && JC_NULL != pcDestWord 
							&& 0 != jc_strlen(pcDestWord))
						{
							pstWordList = (ST_WORD_LIST *)jdd_MemAlloc (1, sizeof (ST_WORD_LIST)) ;
							if (JC_NULL != pstWordList)
							{
								pstWordList->pcWord = pcDestWord ;
								pstWordList->pstNext = JC_NULL ;
								pstStartWordList = pstWordList ;
							}
							else
							{
								rCode = JC_ERR_MEMORY_ALLOCATION ;
								break;
							}
						}
						else
						{
							pstTempWordList = pstStartWordList ;
							bDuplicateWord = E_FALSE ;
							while (JC_NULL != pstTempWordList && E_FALSE == bDuplicateWord)
							{
								if (0 == jc_strcmp (pcDestWord, pstTempWordList->pcWord))
									bDuplicateWord = E_TRUE ;
								else
								{
									pstWordList = pstTempWordList ;
									pstTempWordList = pstTempWordList->pstNext ;
								}
							}
							if (E_TRUE == bDuplicateWord)
							{
								jdd_MemFree (pcDestWord) ;
								pcDestWord = JC_NULL ;
							}
							else
							{
								pstWordList->pstNext = (ST_WORD_LIST *)jdd_MemAlloc (1, sizeof (ST_WORD_LIST)) ;
								if (JC_NULL != pstWordList->pstNext)
								{
									pstWordList = pstWordList->pstNext ;
									pstWordList->pcWord = pcDestWord ;
									pstWordList->pstNext = JC_NULL ;
								}
								else
								{
									rCode = JC_ERR_MEMORY_ALLOCATION ;
									break;
								}
							}
						}
						jdd_MemFree (pcSourceWord) ;
						pcSourceWord = JC_NULL ;
					}
					else
					{
						rCode = JC_ERR_MEMORY_ALLOCATION ;
						break;
					}
				}
				pcTempText = pcText ; 
				uiStrSize = 0 ;
			}
			--uiStrLen ;
		}
		if (rCode == JC_OK && 0 != uiStrSize)
		{
			pcSourceWord = (JC_INT8 *) jdd_MemAlloc (uiStrSize + 1, sizeof (JC_INT8)) ;
			if (JC_NULL != pcSourceWord)
			{
				bDuplicateWord = E_FALSE ;
				jc_strncpy (pcSourceWord , pcTempText, uiStrSize) ;
				rCode = appRemoveLeadTrailSpacesWord (pcSourceWord, &pcDestWord) ;
				if (JC_NULL == pstStartWordList)
				{
					pstWordList = (ST_WORD_LIST *)jdd_MemAlloc (1, sizeof (ST_WORD_LIST)) ;
					pstStartWordList = pstWordList ;
				}
				else
				{
					pstTempWordList = pstStartWordList ;
					bDuplicateWord = E_FALSE ;
					while (JC_NULL != pstTempWordList && E_FALSE == bDuplicateWord)
					{
						if (0 == jc_strcmp (pcDestWord, pstTempWordList->pcWord))
							bDuplicateWord = E_TRUE ;
						else
						{
							pstWordList = pstTempWordList ;
							pstTempWordList = pstTempWordList->pstNext ;
						}
					}
					if (E_TRUE == bDuplicateWord)
					{
						jdd_MemFree (pcDestWord) ;
						pcDestWord = JC_NULL ;
					}
					else
					{
						pstWordList->pstNext = (ST_WORD_LIST *)jdd_MemAlloc (1, sizeof (ST_WORD_LIST)) ;
						pstWordList = pstWordList->pstNext ;
					}
				}
				if (E_FALSE == bDuplicateWord)
				{
					if (JC_NULL != pstWordList)
					{
						pstWordList->pcWord = pcDestWord ;
						pstWordList->pstNext = JC_NULL ;
					}
					else
					{
						rCode = JC_ERR_MEMORY_ALLOCATION ;
					}
				}
				jdd_MemFree (pcSourceWord) ;
				pcSourceWord = JC_NULL ;
			}
			else
			{
				rCode = JC_ERR_MEMORY_ALLOCATION ;
			}
		}
		jdd_MemFree (pcMemText) ;
		pcMemText = JC_NULL ;
	}
	if (JC_OK == rCode && JC_NULL != pmSourceText)
	{
		pstTempWordList = pstStartWordList ;
		uiStrLen = 0 ;
		while  (JC_NULL != pstTempWordList)
		{
			uiStrLen += jc_strlen (pstTempWordList->pcWord) ;
			pstTempWordList = pstTempWordList->pstNext ;
			uiWordCount++ ;
		}
		pcText = (JC_INT8 *)jdd_MemAlloc (uiStrLen + uiWordCount + 1, sizeof (JC_INT8)) ;
		if (JC_NULL != pcText)
		{
			pstTempWordList = pstStartWordList ;
			uiStrLen = 0 ;
			while  (JC_NULL != pstTempWordList)
			{
				pstWordList = pstTempWordList ;
				jc_sprintf (pcText + jc_strlen(pcText), "%s%c",pstWordList->pcWord, ucSeparateText) ;
				pstTempWordList = pstTempWordList->pstNext ;
				jdd_MemFree (pstWordList->pcWord) ;
				pstWordList->pcWord = JC_NULL ;
				jdd_MemFree (pstWordList) ;
				pstWordList = JC_NULL ;
			}
			jdd_MemFree (pmSourceText) ;
			pmSourceText = JC_NULL ;
			rCode = jdi_CUtilsCharToTchar (JC_NULL, pcText, &pmSourceText) ;
			*ppmStringText = pmSourceText ;
			jdd_MemFree (pcText) ;
			pcText = JC_NULL ;
		}	
		
	}
	
	if(JC_ERR_MEMORY_ALLOCATION==rCode)
	{
		pstTempWordList = pstStartWordList ;
		while  (JC_NULL != pstTempWordList)
		{
			pstWordList = pstTempWordList ;
			pstTempWordList = pstTempWordList->pstNext ;
			jdd_MemFree (pstWordList->pcWord) ;
			pstWordList->pcWord = JC_NULL ;
			jdd_MemFree (pstWordList) ;
			pstWordList = JC_NULL ;
		}
	}
	return rCode ;
}

/**
 * @brief This function Remove leading and trailing spaces of a word in JC_INT8 format
 * @param[in] pucSourceWord : Source Word in JC_INT8 format
 * @param[oot] ppucDestWord	: Desination Word in JC_INT8 format

 * @return JC_OK for successful operation else get error in memory allocation.
 *
 * This function checks the validation of Phone number.
 */

JC_RETCODE appRemoveLeadTrailSpacesWord (JC_INT8 *pucSourceWord, JC_INT8 **ppucDestWord)
{
	JC_RETCODE	rCode			= JC_OK ;
	JC_INT8		*pucSourceWord1 = pucSourceWord ;
	JC_INT8		*pucSourceWord2 ;
	JC_INT8		*pucDestWord	= NULL ;
	JC_UINT32	uiWordLen		= 0;
	
	if (JC_NULL != pucSourceWord)
	{
		pucSourceWord1 = pucSourceWord ;
		while (' ' == *pucSourceWord1)
		{
			pucSourceWord1++ ;
		}
		uiWordLen = 0 ;
		pucSourceWord2 = pucSourceWord1 ;
		while (' ' != *pucSourceWord2 && uiWordLen <= jc_strlen(pucSourceWord))
		{
			uiWordLen++ ;
			pucSourceWord2++ ;
		}
		pucDestWord = (JC_INT8 *) jdd_MemAlloc (uiWordLen + 1, sizeof (JC_INT8)) ;
		if (JC_NULL != pucDestWord)
		{
			jc_strncpy (pucDestWord, pucSourceWord1, uiWordLen) ;
		}
		else
		{
			rCode = JC_ERR_MEMORY_ALLOCATION ;
		}
	}
	*ppucDestWord = pucDestWord ;
	return rCode ;
}

JC_BOOLEAN IsStrHavingSpecialChars(JC_CHAR* pmStr)
{
	JC_BOOLEAN rCode = E_FALSE;
	JC_INT32 iIndex  ;
/*	JC_BOOLEAN bisUpperCase = E_FALSE;
	JC_BOOLEAN bisLowerCase = E_FALSE;
	JC_BOOLEAN bisDigit = E_FALSE;
	JC_BOOLEAN bisSpace = E_FALSE;*/

	for( iIndex = 0 ; iIndex < jc_wstrlen(pmStr); iIndex ++)
	{
	/*	if(IsLower(pmStr[iIndex])) bisLowerCase = E_TRUE;
		if(IsUpper(pmStr[iIndex])) bisLowerCase = E_TRUE;
		if(IsLower(pmStr[iIndex])) bisLowerCase = E_TRUE;
		if(IsDigit(pmStr[iIndex])) bisDigit = E_TRUE;
		if(IsSpace(pmStr[iIndex])) bisSpace = E_TRUE;*/

		if(32 < pmStr[iIndex] && pmStr[iIndex] < 47) 
		{	if(44 != pmStr[iIndex] && 46 != pmStr[iIndex])	{	rCode = E_TRUE;	}	}
		if(58 < pmStr[iIndex] && pmStr[iIndex] < 65) 
		{	rCode = E_TRUE;	}
		if(90 < pmStr[iIndex] && pmStr[iIndex] < 97) 
		{	rCode = E_TRUE;	}
		if(122 < pmStr[iIndex] && pmStr[iIndex] < 127) 
		{	rCode = E_TRUE;	}
	}
	return rCode;
}

/*End Push Settings Utils */
/*Start Shortcut Settings UTILS*/
/**
 * @brief This function Check the validataion of Scheme
 * @param[in] pmSchemeText : Specifies the Scheme Character

 * @return Success : E_TRUE else E_FALSE... *
 *
 * This function checks the validation of  Scheme of Shortcuts settings from Shortcuts. .
 */
JC_BOOLEAN appIsValidScheme (JC_INT8 *pucSchemeText) 
{
	JC_INT8		*pucTempSchemeText ;
	JC_UINT8	ucCheck	;
	JC_UINT32	uiStrLen ;
	JC_BOOLEAN	bIsValid = E_TRUE ;
		
		pucTempSchemeText = pucSchemeText ;
		uiStrLen = jc_strlen (pucSchemeText) ;
		ucCheck = *pucTempSchemeText ;
		while (0 != uiStrLen && E_TRUE == bIsValid)
		{
			if (*pucTempSchemeText == ' ')
			{
				while ((*pucTempSchemeText == ' ') && (0 != uiStrLen))
				{
					(*pucTempSchemeText)++ ;
					--uiStrLen ;
				}
				if (((ucCheck >= 'a') && (ucCheck <= 'z')) && 
					 ((*pucTempSchemeText >= 'a') && (*pucTempSchemeText <= 'z')))

				{
					bIsValid = E_FALSE ;
				}
			}
			if (bIsValid && 0 != uiStrLen)
			{
				if (((*pucTempSchemeText >= 'a') && (*pucTempSchemeText <= 'z')) || 
					(*pucTempSchemeText == ',') || 
					(*pucTempSchemeText == ':') || 
					(*pucTempSchemeText == '/'))
				{
					ucCheck = *pucTempSchemeText ;
					(*pucTempSchemeText)++ ;
				}
				else
				{
					bIsValid = E_FALSE ;
				}
				--uiStrLen ;
			}
		}
		pucSchemeText = NULL ;
	
	return bIsValid ;
}


/*End Push Settings Utils */
/*Start Shortcut Settings UTILS*/
/**
 * @brief This function Check the validataion of Scheme
 * @param[in] pmSchemeText : Specifies the Scheme Character

 * @return Success : E_TRUE else E_FALSE... *
 *
 * This function checks the validation of  Scheme of Shortcuts settings from Shortcuts. .
 */
JC_BOOLEAN appIsValidKeyword (JC_INT8 *pucSchemeText) 
{
	JC_INT8		*pucTempSchemeText ;
	JC_UINT8	ucCheck	;
	JC_UINT32	uiStrLen ;
	JC_BOOLEAN	bIsValid = E_TRUE ;
		
		pucTempSchemeText = pucSchemeText ;
		uiStrLen = jc_strlen (pucSchemeText) ;
		ucCheck = *pucTempSchemeText ;
		while (0 != uiStrLen && E_TRUE == bIsValid)
		{
			if (*pucTempSchemeText == ' ')
			{
				while ((*pucTempSchemeText == ' ') && (0 != uiStrLen))
				{
					(*pucTempSchemeText)++ ;
					--uiStrLen ;
				}
				if (((ucCheck >= 'a') && (ucCheck <= 'z')) 
					&& ((*pucTempSchemeText >= 'a') && (*pucTempSchemeText <= 'z')))
				{
					bIsValid = E_FALSE ;
				}
			}
			if (bIsValid && 0 != uiStrLen)
			{
				if (((*pucTempSchemeText >= 'a') && (*pucTempSchemeText <= 'z')) || 
					((*pucTempSchemeText == ',') || (*pucTempSchemeText == '.')))
				{
					ucCheck = *pucTempSchemeText ;
					(*pucTempSchemeText)++ ;
				}
				else
				{
					bIsValid = E_FALSE ;
				}
				--uiStrLen ;
			}
		}
		pucSchemeText = NULL ;
	
	return bIsValid ;
}


/**
 * @brief  : Get URL form Enter URL Page
 * @param[in] pstBrowserApp		: Contains the structure of Browser Applicaton
 * @return Success : JC_OK else Appropriate error code... *
 *
 * This function get the URL Text from Enter URL page. It is also checking the validation of 
    URL. It should send error message for invalid URL
 */

JC_BOOLEAN aapIsValidTcharUrl (JC_CHAR	*pURLText, JC_CHAR **ppEditedURL, \
				JC_BOOLEAN *pbIsFileScheme)
{
	JC_INT8			*pcURLText	= NULL ;
	JC_BOOLEAN		bValidURL	= E_TRUE ;
	JC_CHAR			*pmNextURLText	= NULL			 ;
	JC_CHAR			*pmTempURLText	= NULL			 ;
	JC_UINT32		uiBegin						= 0 ;
	JC_UINT32		uiEnd						= 0 ;
	JC_UINT32		uiStrLen					= 0 ;
	JC_UINT32		uiCount						= 0 ;
	JC_RETCODE		rCode ;

	if( JC_NULL != pURLText)
	{
		pmTempURLText = pURLText ;
		uiStrLen = jc_tcslen (pmTempURLText) ;
		while (32 == *pmTempURLText && uiCount < uiStrLen && NULL != pmTempURLText)
		{
			uiBegin++ ;
			pmTempURLText++ ;
			uiCount++ ;
		}
		while (32 != *pmTempURLText && 13 != *pmTempURLText && NULL != pmTempURLText && uiCount < uiStrLen)
		{
			uiEnd++ ;
			pmTempURLText++ ;
			uiCount++ ;
		}
		if (uiCount < uiStrLen)
		{
			bValidURL = E_FALSE ;
		}
		else if ((0 != uiEnd) && (uiEnd != uiStrLen))
		{
			pmTempURLText = pURLText ;
			pURLText = jdi_CUtilsTcsCopyIndexNum (JC_NULL, pmTempURLText, uiBegin, uiEnd) ;

		}
		if (E_TRUE == bValidURL)
		{
			if (NULL != pURLText)
			{
				rCode = jdi_CUtilsTcharToChar (JC_NULL, pURLText , &pcURLText) ;
				 if (IsValidUrl (pcURLText, pbIsFileScheme))
				{
					rCode = jdi_URLFormNext (NULL, pURLText, NULL, &pmNextURLText) ; 
					*ppEditedURL = pmNextURLText ;
				}
				else
				{
					bValidURL = E_FALSE ;
				}
				if (JC_NULL != pcURLText)
				{
					jdd_MemFree (pcURLText) ;
					pcURLText = NULL ;
				}
			}
			else
			{
				rCode = JC_ERR_MEMORY_ALLOCATION ;
			}
			if(0 != uiEnd && uiEnd != uiStrLen && NULL != pURLText)
			 {
				  jdd_MemFree (pURLText) ;
				  pURLText = NULL ;
			 }
		}
	}
	return bValidURL ;
}

/**
 * @brief This function Check the validataion of shortcut options.
 * @param[in] pmText : Specifies the Shortcut option text
 * @param[in] bIsScheme : Specfies whether the text to be validated is the Scheme
 *						  or the KeyWord text.
 * @return Success : E_TRUE else E_FALSE... *
 *
 * This function checks the validation of  Scheme and KeyWords of Shortcuts settings. .
 */
JC_BOOLEAN appIsValidShortcut (JC_CHAR *pmText, JC_BOOLEAN bIsScheme) 
{
	JC_RETCODE	rCode ;
	JC_INT8		*pucText = JC_NULL ;
	JC_INT8		*pucTempText ;
	JC_BOOLEAN	bIsValid = E_FALSE ;
	JC_BOOLEAN	bIsStart = E_TRUE ;

	rCode = jdi_CUtilsTcharToChar (JC_NULL, pmText, &pucText) ;
	if (JC_OK == rCode)
	{
		bIsValid = E_TRUE ;
		pucTempText = pucText ;

		while ((EOS != *pucTempText) && (E_TRUE == bIsValid))
		{
			if (E_TRUE == bIsStart)
			{
				while ((' ' == *pucTempText) && (EOS != *pucTempText))
				{
					pucTempText++ ;
				}
				bIsStart = E_FALSE ;
			}
			if (EOS != *pucTempText)
			{
				if (bIsScheme && ((*pucTempText >= 'a' && *pucTempText <= 'z') || *pucTempText == ',' || *pucTempText == '/' ))
				{
					if (',' == *pucTempText)
					{
						bIsStart = E_TRUE ;
					}
				}
				else if(bIsScheme && *pucTempText == ':' )
				{
					if(*(pucTempText+1) == '/') 
					{
						if(*(pucTempText+2) != '/')
						{
							bIsValid = E_FALSE ;
						}
					}
					else
					{
						bIsValid = E_FALSE ;
					}
				}
				else if ((E_FALSE == bIsScheme) && (' ' != *pucTempText))
				{
					if (',' == *pucTempText)
					{
						bIsStart = E_TRUE ;
					}
				}
				else
				{
					bIsValid = E_FALSE ;
				}
				pucTempText++ ;
			}
		}
		jdd_MemFree (pucText) ;	
	}

	return bIsValid ;
}

void jWap_showErrorPopup (JC_RETCODE iErr)
{
	U16 			StringId ;
	
	if (iErr)
	{
		switch (iErr)
		{
			case JC_ERR_FILE_NOT_FOUND:
				StringId = STR_ID_WAP_NO_FILE_NAME ;
				break ;

			case JC_ERR_FILE_DIR_ALREADY_EXIST:
				StringId = STR_ID_WAP_HTTPDWN_FILE_EXISTS ;
				break ;

			default:
				StringId = STR_GLOBAL_ERROR ;
				break ;
		}

		DisplayPopup((PU8)GetString(StringId), \
				IMG_GLOBAL_ERROR, 1, 2000, 1);	
	}
}

JC_CHAR	*jWap_GetUniqueFileName (JC_INT8 *pcPath)
{
	JC_INT8			TempStr[11] ;
	JC_UINT32		uiUniqueID ;
	JC_UINT8		ucOffset ;
	JC_CHAR			*pFileName ;

	pFileName = (JC_CHAR *)jdd_MemAlloc (JDOWNLOAD_INPUTBOX_FILENAME_MAX, sizeof(JC_CHAR)) ;
	if (JC_NULL != pFileName)
	{
		ucOffset = (JC_UINT8)jc_strlen (pcPath) ;
		uiUniqueID = jdd_MiscGetUniqueValue () ;
		jdi_CUtilsTcsStrCpy (pcPath, pFileName) ;

		jc_sprintf(TempStr, "%d", uiUniqueID);
		jdi_CUtilsTcsStrCpy (TempStr, pFileName + ucOffset) ;
		while (E_TRUE == jdd_FSIsFileExist (JC_NULL, pFileName))
		{
			uiUniqueID++ ;
			jc_sprintf(TempStr, "%d", uiUniqueID);
			jdi_CUtilsTcsStrCpy (TempStr, pFileName + ucOffset) ;
		}
		mmi_trace (1, "JDD_LOG: jWap_GetUniqueFileName >> uiUniqueID is [%d]", uiUniqueID) ;
	}
	return pFileName ;
}

/*****************************************************************************
*
* @fn 			: jWap_SaveToFile
*
* @brief		: This function save the given buffer to the file specified. 
*					If the file already exists, it will append.
*
* @param[in]	: pmFileName		File name.
*
* @param[in]	: uiBufLen			Length of the buffer.
*
* @param[in]	: pucBuffer			Buffer.
*
* @retval		: JC_OK On Success, On failure specific error code.
*
******************************************************************************/
JC_RETCODE jWap_SaveToFile (JC_CHAR *pmFileName, JC_UINT32 uiBufLen, JC_UINT8 *pucBuffer)
{ 
	JDD_FILE			vFileHandle    = JC_NULL ;  
	JC_RETCODE			rCode = JC_OK ;
	JC_INT32			iBytesWritten = 0 ;
	
	if (JC_NULL == pmFileName)
	{
		rCode = JC_ERR_INVALID_PARAMETER ;
	}
	else if(0 == uiBufLen || JC_NULL == pucBuffer)
	{
		rCode = JC_OK ;
	}
	else
	{
		vFileHandle = jdd_FSOpen  (JC_NULL, pmFileName, E_CREATE_APPEND_WRITE_MODE) ;
		if (JC_NULL != vFileHandle)
		{
			if (uiBufLen > 0)
			{
				iBytesWritten = jdd_FSWrite ((void *)pucBuffer, sizeof (JC_UINT8), uiBufLen, vFileHandle) ;
				if (iBytesWritten != (JC_INT32)uiBufLen)
				{
					rCode = JC_ERR_FILE_WRITE ;
				}
			}
			jdd_FSClose(vFileHandle);
		}
	}
	return rCode ;
}

/*****************************************************************************
 *
 * @fn 			: jWap_GetFileNameFromURL
 *
 * @brief		: This function gives the file name from the URL.
 *
 * @param[in]	: pmURL				URL.
 *
 * @param[in, out]	: pmFileName		Buffer in which the file name needs to be filled.
 *
 * @param[in]	: uiFileNameLen	Length of the file name buffer.
 *
 * @retval		: JC_OK On Success, On failure specific error code.
 *
 ******************************************************************************/
void jWap_GetFileNameFromURL (JC_CHAR  *pmURL, JC_CHAR  *pmFileName, JC_UINT32 uiFileNameLen)
{
	JC_INT32			iUrlLen = 0 ;
	JC_UINT32			uiIndex = 0 ;
	JC_INT32			iSlashIndex = 0 ;
	JC_INT32			iDotIndex = 0 ;
	JC_UINT16 			uhSpecialCharIndex = 0;
		
	if (JC_NULL != pmURL && JC_NULL != pmFileName && 0 < uiFileNameLen)
	{
		iUrlLen = jc_tcslen (pmURL);
		if(0 < iUrlLen)
		{
			iDotIndex = jdi_CUtilsTcsLastIndexOf ( pmURL,(JC_CHAR)'.',iUrlLen - 1) ;			
			iSlashIndex = jdi_CUtilsTcsLastIndexOf ( pmURL ,(JC_CHAR)'/', iUrlLen - 1) ;
			if(0 >  iSlashIndex)
			{
				iSlashIndex = 0 ;
			}
			else
			{
				iSlashIndex += 1 ;
			}

			if( iDotIndex <= 0 || iDotIndex < iSlashIndex)
			{
				iDotIndex = iUrlLen ;
			}			
			for (uiIndex = 0; (uiIndex + iSlashIndex) < (JC_UINT32)iDotIndex && uiFileNameLen > uiIndex; uiIndex++)
			{
				pmFileName[uiIndex] = pmURL[iSlashIndex + uiIndex];
			}			
			pmFileName[uiIndex] = (JC_CHAR)'\0' ;

			if (E_FALSE == jWap_IsValidFileName (pmFileName, &uhSpecialCharIndex))
			{				
				pmFileName[uhSpecialCharIndex] = (JC_CHAR)'\0' ;
			}
			else
			{
				pmFileName[uiIndex] = (JC_CHAR)'\0' ;
				jdi_CUtilsTcsURIUnescape (pmFileName) ;
			}			
		}
	}
}

/**
 * @fn 			: jWap_IsValidFileName
 * @brief This Function checks wheatear the file name is valid or not (Excluding the path).
 * @param[in]	pmFileName Specifies the file name.
 * @retval		E_TRUE Specifies the file name is valid.
 * @retval		E_FALSE Specifies the file name is invalid.
 */
JC_BOOLEAN jWap_IsValidFileName (const JC_CHAR *pmFileName, JC_UINT16 *uhSpecialCharIndex)
{
	JC_UINT16	uhFileNameLen = 0 ;
	JC_UINT16	uhIndex = 0 ;
	JC_BOOLEAN	rCode = E_TRUE ;
	*uhSpecialCharIndex = 0;
	
	if (JC_NULL != pmFileName)
	{
		uhFileNameLen = jc_tcslen(pmFileName);
		if (0 != uhFileNameLen)
		{
			for(uhIndex = 0; uhIndex < uhFileNameLen; uhIndex++)
			{
				if(	pmFileName[uhIndex] == '/'	||
					pmFileName[uhIndex] == '\\' ||
					pmFileName[uhIndex] == '*'	||
					pmFileName[uhIndex] == '<'	||
					pmFileName[uhIndex] == '>'	||
					pmFileName[uhIndex] == ':'	||
					pmFileName[uhIndex] == '"'	||
					pmFileName[uhIndex] == '?'	||
					pmFileName[uhIndex] == '|')
				{
					*uhSpecialCharIndex = uhIndex;
					rCode = E_FALSE;
					break ;
				}
			}
		}
		else
		{
			rCode = E_FALSE ;
		}
	}
	else
	{
		rCode = E_FALSE ;
	}
	mmi_trace(1, "JDD_LOG: IsValidFileNam[%d]", rCode) ;
	return rCode ;
}
JC_RETCODE jWap_CopyToFile (JC_CHAR *pmSrcFileName, JC_CHAR *pmDestFileName)
{ 
	JDD_FILE			vFileHandle    = JC_NULL ;  
	JC_RETCODE			rCode = JC_OK ;
	JC_INT32			iBytesWritten = 0 ;
	JC_UINT32			uiBytesRead = 0 ;
	JC_INT8			*pcBuffer = JC_NULL ;

	if (JC_NULL == pmSrcFileName || JC_NULL == pmDestFileName)
	{
		rCode = JC_ERR_INVALID_PARAMETER ;
	}
	else
	{
		vFileHandle = jdd_FSOpen  (JC_NULL, pmSrcFileName, E_OPEN_READ_MODE) ;
		if (JC_NULL != vFileHandle)
		{
			rCode = jdd_FSGetSize (vFileHandle, &uiBytesRead) ;
			if (JC_OK == rCode && 0 < uiBytesRead)
			{
				pcBuffer = (JC_INT8 *)jdd_MemAlloc (sizeof (JC_INT8), uiBytesRead) ;
				if (pcBuffer)
				{
					iBytesWritten = jdd_FSRead (pcBuffer, sizeof (JC_INT8), uiBytesRead, vFileHandle) ;
					if (iBytesWritten != (JC_INT32)uiBytesRead)
					{
						rCode = JC_ERR_FILE_READ ;
					}
				}
				else
				{
					rCode = JC_ERR_MEMORY_ALLOCATION ;
				}
			}
			jdd_FSClose (vFileHandle) ;
		}
		else
		{
			rCode = JC_ERR_FILE_NOT_FOUND ;
		}
		if (JC_OK == rCode && 0 < uiBytesRead && pcBuffer)
		{
			vFileHandle = jdd_FSOpen  (JC_NULL, pmDestFileName, E_CREATE_WRITE_MODE) ;
			if (JC_NULL != vFileHandle)
			{
				iBytesWritten = jdd_FSWrite ((void *)pcBuffer, sizeof (JC_INT8), uiBytesRead, vFileHandle) ;
				if (iBytesWritten != (JC_INT32)uiBytesRead)
				{
					rCode = JC_ERR_FILE_WRITE ;
				}
				jdd_FSClose (vFileHandle) ;
			}
			else
			{
				rCode = JC_ERR_FILE_OPEN ;
			}
		}
		jdd_MemFree (pcBuffer) ;
	}
	return rCode ;
}

void jwap_UpdatePushIcon()
{	
#ifdef	JWAP_PUSH_ENABLE	

	JC_UINT32		uiUnreadMsg , uiTotalMsg;
	uiUnreadMsg = jwap_getUnreadPushMsg(&uiTotalMsg);
	
	HideStatusIcon(STATUS_ICON_WAP);
	if (uiTotalMsg >= JWAP_MAX_PUSH_MSGS)
	{
		BlinkStatusIcon(STATUS_ICON_WAP);
	}
	else if((uiUnreadMsg > 0) && (uiUnreadMsg < JWAP_MAX_PUSH_MSGS))
	{
		ShowStatusIcon(STATUS_ICON_WAP);
	}
	UpdateStatusIcons();
	mmi_trace (1, "jwap_log: jwap_UpdatePushIcon rCode[%d] total[%d]", uiUnreadMsg, uiTotalMsg);
#endif	
}

void jwap_showPushIcon(JC_UINT32 uiUnreadMsg, JC_UINT32 uiTotalMsg)
{
	#ifdef	JWAP_PUSH_ENABLE	

	HideStatusIcon(STATUS_ICON_WAP);
	if (uiTotalMsg >= JWAP_MAX_PUSH_MSGS)
	{
		BlinkStatusIcon(STATUS_ICON_WAP);
	}
	else if((uiUnreadMsg > 0) && (uiUnreadMsg < JWAP_MAX_PUSH_MSGS))
	{
		ShowStatusIcon(STATUS_ICON_WAP);
	}
	UpdateStatusIcons();
	mmi_trace (1, "jwap_log: jwap_UpdatePushIcon showIcon[%d] total[%d]", uiUnreadMsg, uiTotalMsg);
	#endif
}

JC_RETCODE jwap_GetOfflinePageCountFromFolder()
{
	FMGR_FILTER filter;
	U16			file_count;
	JC_UINT8	ucSavedPageFolder[64] = {0, } ;
	
	FMGR_FILTER_INIT(&filter);	
	FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER_DOT);	
	FMGR_FILTER_SET_ALL(&filter);		
	
	AnsiiToUnicodeString (ucSavedPageFolder, CS_WAP_SAVED_PAGE_PATH_WITH_END_SLASH) ;
	mmi_fmgr_kernel_init(ucSavedPageFolder, filter,  &file_count);
	mmi_trace (1, "jwap_log: PageCountFromFolder[%d]", file_count);
	return file_count;
}

#if 0
JC_CHAR* jwap_MapFileNameToDrive(const JC_CHAR *pmFilePath)
 {
 	JC_CHAR *pmFileName = JC_NULL;

	 if ( (*pmFilePath == 'c' || *pmFilePath == 'C') &&
		   (*(pmFilePath +1) == ':') &&
		   ((*(pmFilePath + 2) == '/') || (*(pmFilePath + 2)== '\\')))
	 {
	 	pmFileName = jdd_MemAlloc(jc_wstrlen(pmFilePath)+1 , sizeof(JC_CHAR));
	 	jc_wstrcpy(pmFileName, pmFilePath+2);
	 }
	 else if ( (*pmFilePath == 'D' || *pmFilePath == 'd') &&
			   (*(pmFilePath +1) == ':') &&
			   ((*(pmFilePath + 2) == '/') || (*(pmFilePath + 2)== '\\')))	 
	 {
		pmFileName = jdd_MemAlloc(jc_wstrlen(pmFilePath)+1, sizeof(JC_CHAR));
	 	jc_wstrcpy(pmFileName, pmFilePath);
		*pmFileName = '/' ;
		 *(pmFileName+1) = 't' ;
		 *(pmFileName+2) = '/' ;
		
	 }
	 else if ( (*pmFilePath == 'e' || *pmFilePath == 'E') &&
		   (*(pmFilePath +1) == ':') &&
		   ((*(pmFilePath + 2) == '/') || (*(pmFilePath + 2)== '\\')))
	 {
	 	pmFileName = jdd_MemAlloc(jc_wstrlen(pmFilePath) + strlen("/home")+ 1, sizeof(JC_CHAR));
	 	jdi_CUtilsTcsStrCpy("/home", pmFileName);
		jc_wstrcat(pmFileName, pmFilePath+2);
	 }
	  return pmFileName;
}


/*	this function will check file existance and invlind file name 
	(special characters like :<>? are not supported in coolsand file system)
*/
	
JC_RETCODE jwap_CheckFileStatus(JC_BOOLEAN *bIsFileExist, \
							  JC_CHAR *pmFileName )
{
	JC_RETCODE	rCode = JC_OK;
	JC_CHAR* 	pmFName = JC_NULL;	
	JDD_FILE	vFileHandle = JC_NULL ;
	JC_INT32 	iFlag = 0;
	JC_INT32 	iFd =-1;

	iFlag = FS_O_RDONLY ;//read mode

	mmi_trace (1, "CheckFileStatus: Entry") ;
	pmFName = jwap_MapFileNameToDrive(pmFileName) ;
	jdd_AssertOnFail ((JC_NULL != pmFileName), "jdd_FSIsFileExist: File name is NULL") ;

	if (JC_NULL == pmFileName)
	{
		mmi_trace (1, "JDD_LOG: File Open for NULL file") ;
		rCode = JC_ERR_INVALID_PARAMETER;
	}
	
	if(pmFName)
	{
		iFd = FS_Open ((PCSTR)pmFName, iFlag, 0);/*Last Parameter is not used*/
	}
	else
	{
		iFd = FS_Open ((PCSTR)pmFileName, iFlag, 0);/*Last Parameter is not used*/
	}
	
	if(0 <= iFd)
	{
		*bIsFileExist = E_TRUE ;	
		jdd_FSClose (vFileHandle) ;
	}
	else if(ERR_FS_INVALID_PARAMETER == iFd)
	{
		*bIsFileExist = E_FALSE;
		rCode = JC_ERR_INVALID_PARAMETER;
	}
	
	jdd_MemFree(pmFName);	
	mmi_trace (1, "JDD_LOG: CheckFileStatus hd[%d] rCode[%d] exist[%d]", \
				iFd, rCode, *bIsFileExist) ;
	return (rCode) ;	
}
#endif

#endif //__JATAAYU_APP__

