#define MODULE_NAME "JMMS"
#define FILE_NAME "jmms_composeutils.c"
 /***************************************************************************
 *
 * File Name : jmms_composeutils.c
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
 * @file		jmms_composeutils.c
 * @ingroup		jMMS
 * @brief		Describes the Compose related API's.
 *
 **/
 
 #ifdef JATAAYU_SUPPORT
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/
#include "jmms_inc.h"


#include "jdi_trfetch.h"
#include "jmms_cb.h"
#include "jmms_storedlg.h"
#include "jmms_composedlg.h"
#include "jmms_composedlg.h"
#include "jmms_settings.h"
#include "jmms_addrdlg.h"
#include "messagesresourcedata.h"
/***************************************************************************
* Macros
**************************************************************************/

#define TEXT_CONTENT_TYPE			"text/plain"
#define jMMS_MAX_MMS_SIZE			100*1024

 /***************************************************************************
 * Global variables declarations
 **************************************************************************/
static const JC_INT8 *g_pContentTypesList[][2] =
{
		{"txt",		"text/plain"},
		{"gif" ,	"image/gif"},
		{"jpeg",	"image/jpeg"},
		{"jpg",		"image/jpeg"},
		{"jpe",		"image/jpeg"},
		{"png",		"image/png"},
		{"wbmp",	"image/vnd.wap.wbmp"},
		{"bmp" ,	"image/bmp"},
		{"wav", 	"audio/wav"},
		{"imy",		"text/x-imelody"},
		{"mid",		"audio/midi"},
		{"mp3",		"audio/mp3"},
		{"midi",	"audio/midi"},
		{"amr",		"audio/amr"},
		{"aac",		"audio/aac"},
		{"qcp",		"audio/vnd.qcelp"},
		{"qcelp",   "audio/qcelp"},
		{"mp4",		"video/mp4"},
		{"3gp",		"video/3gpp"},
		{"3g2",		"video/3gpp2"},
		{"mpg",		"video/mpeg"},
		{"mpeg",	"video/mpeg"},
		{"mpe",		"video/mpeg"},
		{"wmv",		"video/wmv"},
		{"dat",		"video/dat"},
		{"avi",		"video/avi"},		
		{"vcs",		"text/x-vcalendar"},
		{"vcf",		"text/x-vcard"},
		{"c",		"text/plain"},
		{"c++",		"text/plain"},
		{"h",		"text/plain"},
		{"pl",		"text/plain"},
		{"html",	"text/html"},
		{"htm",		"text/html"},
		{"tiff",	"image/tiff"},
		{"tif",		"image/tiff"},
		{"pdf", 	"application/pdf"}	,
		{"doc",		"application/msword"},
		{".xls",	"application/vnd.ms-excel"},
		{"ppt",		"application/ms-powerpoint"},
		{"smil",	"application/smil"},
		{"zip", 	"application/zip"},
		{"eml",		"message/rfc822"},
		{NULL,		"application/octet-stream"}
} ;

 /***************************************************************************
 * All Global Function Definitions 
**************************************************************************/

JC_INT8* GetContentTypeFromFile (JC_CHAR *szFileName)
{
	JC_INT8	*pContentType = NULL ;

	if(szFileName)
	{
		JC_INT32	iIndex = 0 ;

		iIndex = jdi_CUtilsTcsLastIndexOf(szFileName, '.', jc_tcslen(szFileName) - 1) ;
		if(iIndex > 0 && jc_tcslen(szFileName) != (JC_UINT32)iIndex)
		{
			JC_UINT16	uhCounter = 0;

			iIndex++ ;
			while(g_pContentTypesList[uhCounter][0])
			{
                if(jdi_CUtilsTcsStrCaseCmp(szFileName + iIndex, g_pContentTypesList[uhCounter][0]) == 0)
				{
					pContentType = jdi_CUtilsStrDuplicate(JC_NULL, g_pContentTypesList[uhCounter][1]) ;
					break ;
				}
				uhCounter++ ;
			}
		}
	}
	if(pContentType == NULL)
	{
		pContentType = jdi_CUtilsStrDuplicate(JC_NULL, "application/octet-stream") ;
	}
	return pContentType ;
}
 
void jmms_GoToDisplaySlideScreen()
{
	DeleteNHistory(2);
	GoBackHistory();
}
void jMMS_HandleTextMediaError(JC_RETCODE rCode)
{
	switch(rCode)
	{	
	case JC_ERR_MSG_MAX_REACHED:		
		DisplayPopup((PU8)GetString(STR_ID_MMS_MESSAGE_SIZE_EXCEED), IMG_GLOBAL_INFO, 1,2000, 1);
		DeleteNHistory(2);
		break ;
	case JC_ERR_MSG_MMS_MEDIA_TYPE_EXISTS:
		DisplayPopup((PU8)GetString(STR_ID_MMS_CNF_MEDIA_TYPE), IMG_GLOBAL_INFO, 1,2000, 1);
		DeleteNHistory(2);
		break ;
	case JC_OK:
	case JC_ERR_MSG_MULTIPLE_REFERENCE:	
		jmms_GoToDisplaySlideScreen();				
		break;
	default:
		DisplayPopup((PU8)GetString(STR_ID_MMS_ADD_MEDIA_FAILED), IMG_GLOBAL_INFO, 1,2000, 1);
		DeleteNHistory(2);
		break ;		
	}
}
void finished_AddMedia()
{
	/* Modified by Praveen for Attachments */
	
	if (IsScreenPresent (SCR_ID_MMS_ATTACHMENT))
	{
		GoBackToHistory(SCR_ID_MMS_ATTACHMENT);
	}
	else
	{
		GoBackToHistory(SCR_ID_MMS_COMPOSE_NEWMSG_SCREEN);
	}
	
}
extern UINT8 entry_from_image_view;
void jMMS_HandleMediaError(JC_RETCODE rCode)
{
	if (E_FALSE == g_pstJMmsAppInfo->bIsViolatingContentClass)
	{
		switch(rCode)
		{	
		case JC_ERR_MSG_MAX_REACHED:			
			DisplayPopup((PU8)GetString(STR_ID_MMS_MESSAGE_SIZE_EXCEED), IMG_GLOBAL_INFO, 1,2000, 1);
#if 0			
			if (IsScreenPresent (SCR_ID_MMS_ATTACHMENT))
			{
				DeleteUptoScrID(SCR_ID_MMS_ATTACHMENT);
			}
			else
			{
				DeleteUptoScrID(SCR_ID_MMS_COMPOSE_NEWMSG_SCREEN);
			}
#endif			
			break ;
		case JC_ERR_MSG_MMS_MEDIA_TYPE_EXISTS:			
			DisplayPopup((PU8)GetString(STR_ID_MMS_CNF_MEDIA_TYPE), IMG_GLOBAL_INFO, 1,2000, 1);
			if (IsScreenPresent (SCR_ID_MMS_ATTACHMENT))
			{
				DeleteUptoScrID(SCR_ID_MMS_ATTACHMENT);
			}
			else
			{
				DeleteUptoScrID(SCR_ID_MMS_COMPOSE_NEWMSG_SCREEN);
			}
			break ;
		case JC_OK:
		case JC_ERR_MSG_MULTIPLE_REFERENCE:
			if(entry_from_image_view==0)
			finished_AddMedia();
			
			break ;
		case JC_ERR_MSG_MEDIA_RESTRICTED:
			/* Display proper message */
			DisplayPopup((PU8)GetString(STR_ID_MMS_CNF_RESTRICTED_MEDIA),IMG_GLOBAL_INFO, 1,2000, 1);
			//if (IsScreenPresent (SCR_ID_MMS_ATTACHMENT))
			//{
			//	DeleteUptoScrID(SCR_ID_MMS_ATTACHMENT);
			//}
			//else
			//{
			//	DeleteUptoScrID(SCR_ID_MMS_COMPOSE_NEWMSG_SCREEN);
			//}
			break ;
		case JC_ERR_FILE_EMPTY:
			DisplayPopup((PU8)GetString(STR_ID_MMS_FILE_EMPTY), IMG_GLOBAL_INFO, 1,2000, 1);
#if 0
			if (IsScreenPresent (SCR_ID_MMS_ATTACHMENT))
			{
				DeleteUptoScrID(SCR_ID_MMS_ATTACHMENT);
			}
			else
			{
				DeleteUptoScrID(SCR_ID_MMS_COMPOSE_NEWMSG_SCREEN);
			}
#endif
			break ;
		default:			
			DisplayPopup((PU8)GetString(STR_ID_MMS_ADD_MEDIA_FAILED), IMG_GLOBAL_INFO, 1,2000, 1);
#if 0
			if (IsScreenPresent (SCR_ID_MMS_ATTACHMENT))
			{
				DeleteUptoScrID(SCR_ID_MMS_ATTACHMENT);
			}
			else
			{
				DeleteUptoScrID(SCR_ID_MMS_COMPOSE_NEWMSG_SCREEN);
			}
#endif
			break ;		
		}
	}
}

JC_RETCODE AddTextMedia(JC_CHAR *psTextBuffer,JC_UINT8 ucSlideNo)
{
	JC_RETCODE	eRet		= JC_OK ;
	JC_UINT32	uiMediaID		= 0 ;
	
	
	MEDIA_MIME_INFO *pstMediaMimeInfo =JC_NULL ;
	MEDIA_INFO 		stCurrentMediaInfo;

	jc_memset(&stCurrentMediaInfo, 0, sizeof(MEDIA_INFO));
	if(psTextBuffer != NULL)
	{
		pstMediaMimeInfo = (MEDIA_MIME_INFO *) jdd_MemAlloc(1, sizeof(MEDIA_MIME_INFO));
		pstMediaMimeInfo->MimeHeaders.pContentType  = jdi_CUtilsStrDuplicate(JC_NULL, TEXT_CONTENT_TYPE) ;		
		pstMediaMimeInfo->MimeHeaders.bIsAttachment = E_FALSE;
		pstMediaMimeInfo->ContentData.uiTotalSize   = jc_tcslen(psTextBuffer) * sizeof(JC_CHAR);
		pstMediaMimeInfo->ContentData.uiCurSize     = jc_tcslen(psTextBuffer) * sizeof(JC_CHAR);
		pstMediaMimeInfo->ContentData.bIsStream     = E_FALSE;
		pstMediaMimeInfo->ContentData.pBuffer       =(JC_UINT8*)psTextBuffer;
		pstMediaMimeInfo->ContentData.eCopyMemory   = E_FALSE;
		
		stCurrentMediaInfo.pMimeInfo                   = pstMediaMimeInfo;
		stCurrentMediaInfo.eMediaType                  = E_MEDIA_TEXT;
		stCurrentMediaInfo.uiMediaID                   = 0;
		
		jdi_CUtilsCharToTchar(JC_NULL, "Text", &(stCurrentMediaInfo.PresentationInfo.pRegionName)) ;
	
		stCurrentMediaInfo.PresentationInfo.uiHeight   = 0;
		stCurrentMediaInfo.PresentationInfo.uiWidth    = 0;
		
		eRet = jdi_MsgAddMedia (g_pstJMmsAppInfo->hCurrMsg, &stCurrentMediaInfo,
								ucSlideNo, &uiMediaID);
		if (eRet == JC_ERR_ASYNC_CALL)
		{			
			eRet = JC_OK ;
		}
			
		if(eRet != JC_OK && eRet != JC_ERR_MSG_MULTIPLE_REFERENCE)
		{			
			//Free Allocated Buffer
			jdd_MemFree (stCurrentMediaInfo.pMimeInfo->ContentData.pBuffer) ;
			jdd_MemFree (stCurrentMediaInfo.pMimeInfo->ContentData.pStream) ;
			jdd_MemFree (stCurrentMediaInfo.pMimeInfo->MimeHeaders.pContentType) ;
			jdd_MemFree (stCurrentMediaInfo.pMimeInfo) ;
		}		

		if ( JC_OK == eRet || JC_ERR_MSG_MULTIPLE_REFERENCE  == eRet)
		{
			jc_tcscpy(g_pstJMmsAppInfo->pActiveTextBuffer, (JC_CHAR*)psTextBuffer );
			g_pstJMmsAppInfo->ActivePageMediaIds[0] = uiMediaID ;		
		}
		else
    	{        
			mmi_trace(1,"AddTextMedia : unexpected Error [%d]",eRet) ;
		}		
	}
	return eRet ;
}

static JC_CHAR * jmms_UpdateFileName(JC_CHAR *pmSrc)
{
	JC_CHAR *pmResult = JC_NULL ;
	JC_CHAR *pmTemp = JC_NULL ;
	
	while ('\0' != *pmSrc)
	{
		if (*pmSrc == '/')
		{
			pmResult = (JC_CHAR *)pmSrc + 1;
		}
		pmSrc++ ;
	}
	if(pmResult)
	{
		pmTemp = jdi_CUtilsTcsDuplicate(JC_NULL,pmResult) ;	
	}
	return pmTemp ;
}
extern U32 Get_Audio_duration(JC_CHAR *pFileName) ;

JC_RETCODE AddObjectMedia(JC_CHAR *pFileName,EMediaType eMediaType,JC_UINT8 ucSlideNo,JC_BOOLEAN bIsAttachment)
{
	JC_RETCODE 		rStatus = JC_OK;
	JC_UINT32 		uiFileSize = 0;
	JC_UINT32 		uiMediaID = 0;
	JC_CHAR        *pmRegionName = JC_NULL;
	JC_INT8        *pcContentType = JC_NULL;
	MEDIA_INFO 		stCurrentMediaInfo;
	MEDIA_MIME_INFO *pstMediaMimeInfo = JC_NULL;
	
	jc_memset(&stCurrentMediaInfo, 0, sizeof(MEDIA_INFO));
	if(JC_NULL != pFileName )
	{
		if (E_TRUE == jdd_FSIsFileExist (JC_NULL, pFileName))
		{
			rStatus = jdd_FSGetFileSize(JC_NULL, pFileName, &uiFileSize);
			if(JC_OK != rStatus)
			{
				rStatus = JC_ERR_FILE_NOT_FOUND;
			}
			else if(0 == uiFileSize)
			{
				rStatus = JC_ERR_FILE_EMPTY;
			}
			else
			{				
				pcContentType = GetContentTypeFromFile(pFileName);				
				if (JC_NULL != pcContentType)
				{
					pstMediaMimeInfo = (MEDIA_MIME_INFO *) jdd_MemAlloc(1, sizeof(MEDIA_MIME_INFO));
					if (JC_NULL != pstMediaMimeInfo)
					{
						JC_CHAR *pmActualFileName = JC_NULL;
						rStatus = JC_OK;
						pstMediaMimeInfo->MimeHeaders.pContentType  = pcContentType;
						pstMediaMimeInfo->MimeHeaders.bIsAttachment = bIsAttachment;
						pstMediaMimeInfo->ContentData.uiTotalSize   = uiFileSize;
						pstMediaMimeInfo->ContentData.uiCurSize     = 0 ; 
						pstMediaMimeInfo->ContentData.bIsStream     = E_TRUE;
						pstMediaMimeInfo->ContentData.pFileName     = pFileName;
						pstMediaMimeInfo->ContentData.pStream       = pFileName;
						pmActualFileName = jmms_UpdateFileName(pFileName) ;						

						if(E_TRUE == jmms_IsFileNameAscii ( (JC_CHAR*)pmActualFileName))
						{
							mmi_trace(1," jmms_IsFileNameAscii true");							
							pstMediaMimeInfo->MimeHeaders.pObjectName   = pmActualFileName;
						}
						else
						{
							JC_CHAR *pmUpDatedFileName = JC_NULL;
							
							mmi_trace(1," jmms_IsFileNameAscii false");
							rStatus = jdd_MsgConfigGetFileNameWithExt (&pmUpDatedFileName, pcContentType, 0) ;
							pstMediaMimeInfo->MimeHeaders.pObjectName   = pmUpDatedFileName;
							jdd_MemFree(pmActualFileName);							
						}						
						
						pstMediaMimeInfo->ContentData.uiStartOffset = 0;
						pstMediaMimeInfo->ContentData.pBuffer       = JC_NULL;
						pstMediaMimeInfo->ContentData.eCopyMemory = E_FALSE;
						pstMediaMimeInfo->ContentData.cbReadCallback = FileStreamReadCB ;
						
						stCurrentMediaInfo.eMediaType = eMediaType;
						stCurrentMediaInfo.pMimeInfo = pstMediaMimeInfo;
					}
				}			
				
				if((E_MEDIA_OTHERS != eMediaType) || (E_FALSE == bIsAttachment) )
				{	
					if (eMediaType == E_MEDIA_IMAGE || eMediaType == E_MEDIA_VIDEO)
					{
						jdi_CUtilsCharToTchar(JC_NULL, "Image", &pmRegionName) ;
						jdi_MsgSetPresentationType(g_pstJMmsAppInfo->hCurrMsg, E_PRESENTATION_SMIL); /*Presentation mode is set to SMIL*/
					}				
					stCurrentMediaInfo.PresentationInfo.pRegionName = pmRegionName;

					if(eMediaType == E_MEDIA_AUDIO)
					{

						JC_INT32 uiAudioDuration = Get_Audio_duration(pFileName); // get the audio from platform interface the duration audio file pFileName 
						JC_UINT32 uiSlidetime;

					    rStatus = jdi_MsgGetPageTimer(g_pstJMmsAppInfo->hCurrMsg,g_pstJMmsAppInfo->ActivePageIndex, \
					           &uiSlidetime);
					 
					    if( uiAudioDuration > uiSlidetime )  
					          uiSlidetime = uiAudioDuration ;
					    if(60000 < uiSlidetime)
					            uiSlidetime = 60000;
					    
						if(JC_OK == rStatus)
						{
							rStatus = jdi_MsgSetPageTimer(g_pstJMmsAppInfo->hCurrMsg,g_pstJMmsAppInfo->ActivePageIndex, \
								uiSlidetime);
							mmi_trace(1, "jdd_log: jdi_MsgSetPageTimer eststus [%d] uiSlidetime[%d]", rStatus, uiSlidetime);
						}
					 
					}
				}			
				rStatus = jdi_MsgAddMedia(g_pstJMmsAppInfo->hCurrMsg, &stCurrentMediaInfo, ucSlideNo, &uiMediaID);						
				mmi_trace(1, "jdi_MsgAddMedia [%d] ",rStatus) ;
					
				if (rStatus == JC_ERR_ASYNC_CALL)
				{
					rStatus = JC_OK ;
				}
				if (rStatus != JC_OK  )
				{						
					//Free Allocated Buffer
					jdd_MemFree (stCurrentMediaInfo.pMimeInfo->ContentData.pBuffer) ;					
					jdd_MemFree (stCurrentMediaInfo.pMimeInfo->ContentData.pStream) ;
					jdd_MemFree (stCurrentMediaInfo.pMimeInfo->MimeHeaders.pContentType) ;
					jdd_MemFree (stCurrentMediaInfo.pMimeInfo->MimeHeaders.pObjectName) ;		
					jdd_MemFree (stCurrentMediaInfo.pMimeInfo) ;

					if (rStatus != JC_ERR_MSG_MULTIPLE_REFERENCE)
					{
						/* In case of rStatus as JC_ERR_MSG_MULTIPLE_REFERENCE,
							stCurrentMediaInfo.PresentationInfo.pRegionName, will be freed inside
							jdi_MsgDeInit () */
						jdd_MemFree(pmRegionName);
					}
				}
				if (JC_OK == rStatus || (JC_ERR_MSG_MULTIPLE_REFERENCE == rStatus)
							|| rStatus == JC_ERR_MSG_MMS_MEDIA_TYPE_EXISTS)
				{
					g_pstJMmsAppInfo->ActivePageMediaIds[eMediaType] = uiMediaID ;		
				}
				else
				{
					mmi_trace(1,"AddObjectMedia : unexpected Error [%d]",rStatus);
				}				
			}
			if(rStatus == JC_ERR_FILE_NOT_FOUND || rStatus == JC_ERR_FILE_EMPTY)
			{
				jdd_MemFree(pFileName);
			}
		}
	}	
	return rStatus;
}

JC_RETCODE  jMms_setMsgProperties ()
{
	JC_RETCODE rCode;
	JC_UINT8  ucTotalPages = 0;
	JC_UINT8 ucMaxPages = jMMS_MAX_SLIDE_NUMBER;
	
	rCode = jdi_MsgSetMessagingType (g_pstJMmsAppInfo->hCurrMsg, E_TYPE_MMS) ;

	if (JC_OK == rCode)
	{
		rCode = jdi_MsgSetEncodingType (g_pstJMmsAppInfo->hCurrMsg, E_FALSE) ;//From Setting .
	}

	if(JC_OK == rCode)
	{
		rCode = jdi_MsgSetMaxSize (g_pstJMmsAppInfo->hCurrMsg, jMMS_MAX_MMS_SIZE) ;//Default presently 
	}

	if ( rCode == JC_OK )
	{
		rCode = jdi_MsgSetPresentationType(g_pstJMmsAppInfo->hCurrMsg,E_PRESENTATION_SMIL) ;
		jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &ucTotalPages) ;
		ucMaxPages = ucTotalPages>ucMaxPages? ucTotalPages:jMMS_MAX_SLIDE_NUMBER;
		rCode = jdi_MsgSetMaxPages (g_pstJMmsAppInfo->hCurrMsg, ucMaxPages) ; //Default presently 	
	}	 
	return rCode;
}

JC_RETCODE jmms_UpdateMessageHeaders()
{
	JC_RETCODE rStatus = JC_ERR_INVALID_PARAMETER ;
	MESSAGE_HEADERS *pMsgHdr = JC_NULL ;
	MESSAGE_SEND   *pSendHeader = JC_NULL ;
	JC_BOOLEAN bNewMessageHeaders = E_FALSE ;

	rStatus = jdi_MsgGetMessageHeaders(g_pstJMmsAppInfo->hCurrMsg, &pMsgHdr) ;
	
	mmi_trace(1,"jdd_log: MsgGetMessageHeaders[%d]", rStatus);
	if (JC_OK == rStatus || JC_ERR_MSG_HEADERS_NOT_SET == rStatus)
	{
		if (JC_NULL == pMsgHdr)
		{
			rStatus = JC_ERR_MSG_HEADERS_NOT_SET ;
		}
		if (JC_ERR_MSG_HEADERS_NOT_SET == rStatus)
		{
			pMsgHdr = (MESSAGE_HEADERS *) jdd_MemAlloc(sizeof(MESSAGE_HEADERS), 1) ;
			if (JC_NULL != pMsgHdr)
			{
				rStatus = jdi_MsgSetMessageHeaders(g_pstJMmsAppInfo->hCurrMsg, pMsgHdr) ;
				if (JC_OK == rStatus)
				{
					pSendHeader = (MESSAGE_SEND *) jdd_MemAlloc(sizeof(MESSAGE_SEND), 1) ;
					if (JC_NULL != pSendHeader)
					{
						pMsgHdr->eMsgType = E_MSG_TYPE_SEND ;
						pSendHeader->pVersion = jdi_CUtilsStrDuplicate(NULL,"1.2");
						pMsgHdr->pHeaders = pSendHeader ;
						bNewMessageHeaders = E_TRUE ;
						rStatus = JC_OK;
					}
				}
			}
		}
		else	// If the message headers already set
		{
			if (JC_NULL != pMsgHdr->pHeaders)
			{
				pSendHeader = (MESSAGE_SEND *) pMsgHdr->pHeaders ;
			}
		}
		if(JC_OK == rStatus )
		{
			if (E_MSG_TYPE_SEND == pMsgHdr->eMsgType)
			{
				if (E_FALSE == bNewMessageHeaders)
				{
					if (JC_NULL != pSendHeader->pTo)
					{
						jdd_MemFree(pSendHeader->pTo) ;
						pSendHeader->pTo = JC_NULL ;
					}
					if (JC_NULL != pSendHeader->pCc)
					{
						jdd_MemFree(pSendHeader->pCc) ;
						pSendHeader->pCc = JC_NULL ;
					}
					if (JC_NULL != pSendHeader->pBcc)
					{
						jdd_MemFree(pSendHeader->pBcc) ;
						pSendHeader->pBcc = JC_NULL ;
					}
					if (JC_NULL != pSendHeader->pSubject)
					{
						jdd_MemFree(pSendHeader->pSubject) ;
						pSendHeader->pSubject = JC_NULL ;
					}
				}

				if(g_pstJMmsAppInfo)
					{
						// fill new to,cc,bcc,subject fileds here
						if(g_pstJMmsAppInfo->stMmsHeader.pTo )
						{
							pSendHeader->pTo = jdi_CUtilsTcsDuplicate(NULL, g_pstJMmsAppInfo->stMmsHeader.pTo);							
							jdd_MemFree(g_pstJMmsAppInfo->stMmsHeader.pTo);
							g_pstJMmsAppInfo->stMmsHeader.pTo = NULL;
						}
						if(g_pstJMmsAppInfo->stMmsHeader.pCc)
						{
							pSendHeader->pCc = jdi_CUtilsTcsDuplicate(NULL, g_pstJMmsAppInfo->stMmsHeader.pCc);							
							jdd_MemFree(g_pstJMmsAppInfo->stMmsHeader.pCc);
							g_pstJMmsAppInfo->stMmsHeader.pCc = NULL;							
						}
						if(g_pstJMmsAppInfo->stMmsHeader.pBcc)
						{
							pSendHeader->pBcc = jdi_CUtilsTcsDuplicate(NULL, g_pstJMmsAppInfo->stMmsHeader.pBcc);							
							jdd_MemFree(g_pstJMmsAppInfo->stMmsHeader.pBcc);
							g_pstJMmsAppInfo->stMmsHeader.pBcc = NULL;							
						}
						if(g_pstJMmsAppInfo->stMmsHeader.pSubject)
						{
							pSendHeader->pSubject = jdi_CUtilsTcsDuplicate(NULL, g_pstJMmsAppInfo->stMmsHeader.pSubject);							
							jdd_MemFree(g_pstJMmsAppInfo->stMmsHeader.pSubject);
							g_pstJMmsAppInfo->stMmsHeader.pSubject = NULL;
						}
					}
				
				pSendHeader->eMsgPriority    = jmms_GetPriorityValue();
				pSendHeader->bReadReport     = g_pstJMmsAppInfo->stMMSSetting.bReadReport;
				pSendHeader->bDeliveryReport = g_pstJMmsAppInfo->stMMSSetting.bDeliveryReport ;
				pSendHeader->uiExpiryTime = jmms_GetExpiryTimeValue();
				pSendHeader->uiDeliveryTime = jmms_GetDeliveryTimeValue(); 
			}
		}
		else
		{
			if (E_TRUE == bNewMessageHeaders)
			{
				if (JC_NULL != pMsgHdr)
				{
					jdd_MemFree(pMsgHdr) ;
					pMsgHdr = JC_NULL;
				}
				if (JC_NULL != pSendHeader)
				{
					jdd_MemFree(pSendHeader) ;
					pSendHeader = JC_NULL;
				}
			}
		}
	}
	return rStatus ;
}

// This function gets the input value for the input box control. 
JC_RETCODE jMMS_MMIGetInputBoxValue (JC_CHAR **ppsInputValue) 
{
	JC_RETCODE rCode ;	
	{
		*ppsInputValue = jdi_CUtilsTcsDuplicate(JC_NULL, (const JC_CHAR *)g_pstJMmsAppInfo->pcInputBuff) ;		
		if(*ppsInputValue)
		{
			jc_memset(g_pstJMmsAppInfo->pcInputBuff,0,jMMS_MAX_INPUT_SIZE) ;
			rCode = JC_OK ;
		}
		else
		{
			rCode = JC_ERR_INVALID_PARAMETER ;
		}
	}	
	return rCode ;
}

//#define PUSH_WRITE_TO_FILE

#ifdef PUSH_WRITE_TO_FILE
#include "jdd_fileapi.h"
static void dumpPushBufferToFile (JC_INT8 *pcBuffer, JC_UINT32 uiBufLen)
{
	/* To be used for logging push buffer*/
	JDD_FILE *vfLogHandle = JC_NULL ;
	JDD_FSHANDLE    hFSHandle = JC_NULL ;
	JC_CHAR			amPath [255] ;

	jdi_CUtilsTcsStrCpy ("/t/pushBuff.dat", amPath) ;
	if (JC_OK == jdd_FSInitialize (&hFSHandle))
	{
		vfLogHandle = (JDD_FILE*)jdd_FSOpen(hFSHandle, amPath, \
								E_CREATE_APPEND_WRITE_MODE) ;
	}	
	if(vfLogHandle)
	{
		jdd_FSWrite ((void*)pcBuffer, sizeof( JC_UINT8 ), uiBufLen, vfLogHandle) ;
	}	
	jdd_FSClose (vfLogHandle) ;
	return ;
}
#endif

#ifdef __MMI_MULTI_SIM__
JC_RETCODE MMS_SendPushMsgToTransport (INT8 *pcBuffer, INT32 ulBuffrLen, UINT8 nSimID)
#else
JC_RETCODE MMS_SendPushMsgToTransport (INT8 *pcBuffer, INT32 ulBuffrLen)
#endif
{
	JC_RETCODE			retCode = JC_ERR_UNKNOWN ;
	JC_EVENT_DATA			stEventData;	
	JC_UINT32 temp= 0;
	mmi_trace(1,"MMS_SendPushMsgToTransport dual sim Buffer Len[%d]",ulBuffrLen) ;	

	#ifdef PUSH_WRITE_TO_FILE
	dumpPushBufferToFile (pcBuffer, ulBuffrLen) ;
	#endif
		
	stEventData.uiEventInfoSize = sizeof (ST_TR_PUSH_SMS_CONTENT) + ulBuffrLen + 1;
	stEventData.destTaskInfo.iAppID = (JC_INT32) E_TASK_TRANSPORT ;
	stEventData.destTaskInfo.iTaskID = (JC_INT32) E_TASK_TRANSPORT ;
	stEventData.srcTaskInfo.iAppID = (JC_INT32) E_TASK_TRANSPORT ;
	stEventData.srcTaskInfo.iTaskID = (JC_INT32) E_TASK_TRANSPORT ;
	stEventData.iEventType = E_TR_SMS_PUSH_MSG ;
	retCode = jdd_QueueAlloc (&stEventData) ;
	if (retCode != JC_OK)
	{
		retCode = JC_ERR_MEMORY_ALLOCATION ;
	}
	else
	{
		ST_TR_PUSH_SMS_CONTENT	*pstPushContent ;
		
		pstPushContent = (ST_TR_PUSH_SMS_CONTENT *) stEventData.pEventInfo;
		jc_memset (pstPushContent, 0x00, stEventData.uiEventInfoSize);
		pstPushContent->iPushLen = ulBuffrLen ;

		#ifdef __MMI_MULTI_SIM__
		jc_sprintf(pstPushContent->acSmeAddr,"%d" , nSimID);
		mmi_trace(1,"JDD_LOG: MMS_SendPushMsgToTransport dual sim nSimID[%s] ",pstPushContent->acSmeAddr) ;
		#endif
		
		jc_memcpy (&pstPushContent->ucPushMsg, pcBuffer, ulBuffrLen) ;
		for(temp=0; temp < ulBuffrLen; temp++ )
		{				
			mmi_trace(1,"JDD_LOG: buffer value is [%x-%c-%d]", pstPushContent->ucPushMsg[temp], 
				pstPushContent->ucPushMsg[temp], temp);
		}		
		retCode = jdd_QueueSend (&stEventData) ;
	}	
	mmi_trace(1,"JDD_LOG: MMS_SendPushMsgToTransport dual sim End") ;	
	return retCode;
}

void jMms_DeletePage_Msg()
{
	JC_RETCODE rCode = JC_OK;
	
	rCode = jdi_MsgDeletePage(g_pstJMmsAppInfo->hCurrMsg, g_pstJMmsAppInfo->ActivePageIndex) ;
	
	if(rCode == JC_OK)
	{
		jc_memset ( g_pstJMmsAppInfo->ActivePageMediaIds , 0 , sizeof (g_pstJMmsAppInfo->ActivePageMediaIds) );		
		jc_memset(g_pstJMmsAppInfo->pActiveTextBuffer, 0,sizeof(JC_CHAR)*(JMMS_MAX_TEXT_PER_SLIDE+1));
		if (g_pstJMmsAppInfo->ActivePageIndex > 1)
		{
			g_pstJMmsAppInfo->ActivePageIndex--;
		}
		DisplayPopup(GetString (STR_ID_MMS_SLIDE_DELETED), IMG_GLOBAL_INFO,
								1, 2000, 1);
		Go_to_slide_screen_cb();
	}
	else
	{
		DisplayPopup(GetString (STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR,
								1, 2000, 1);
		Go_to_slide_screen_cb();
	}
}

void jMms_DeleteImage_Msg()
{
	JC_RETCODE rCode ;
	
	rCode = jdi_MsgDeleteMedia (g_pstJMmsAppInfo->hCurrMsg, g_pstJMmsAppInfo->ActivePageIndex, 
								g_pstJMmsAppInfo->ActivePageMediaIds[1], 0) ;
	if(JC_OK == rCode)
	{
		g_pstJMmsAppInfo->ActivePageMediaIds[1] = 0 ;		
		DisplayPopup(GetString (STR_ID_MMS_IMAGE_DELETED), IMG_GLOBAL_INFO,
								1, 2000, 1);
		Go_to_slide_screen_cb();
	}
	else
	{
		GoBackToHistory(SCR_ID_MMS_COMPOSE_NEWMSG_SCREEN);
	}
}

void jMms_DeleteAudio_Msg()
{
	JC_RETCODE rCode ;
	
	rCode = jdi_MsgDeleteMedia(g_pstJMmsAppInfo->hCurrMsg, g_pstJMmsAppInfo->ActivePageIndex, 
							   g_pstJMmsAppInfo->ActivePageMediaIds[2], 0) ;
	if(JC_OK == rCode)
	{		
		DisplayPopup(GetString (STR_ID_MMS_AUDIO_DELETED), IMG_GLOBAL_INFO,
								1, 2000, 1);
		Go_to_slide_screen_cb();
		g_pstJMmsAppInfo->ActivePageMediaIds[2] = 0 ;
	}	
	else
	{
		GoBackToHistory(SCR_ID_MMS_COMPOSE_NEWMSG_SCREEN);	
	}
}

void jMms_DeleteVideo_Msg()
{
	JC_RETCODE rCode ;
	
	rCode = jdi_MsgDeleteMedia (g_pstJMmsAppInfo->hCurrMsg, g_pstJMmsAppInfo->ActivePageIndex, 
								g_pstJMmsAppInfo->ActivePageMediaIds[3], 0) ;
	if(JC_OK == rCode)
	{
		g_pstJMmsAppInfo->ActivePageMediaIds[3] = 0 ;		
		DisplayPopup(GetString (STR_ID_MMS_VIDEO_DELETED), IMG_GLOBAL_INFO,
								1, 2000, 1);
		Go_to_slide_screen_cb();
	}
	else
	{
		GoBackToHistory(SCR_ID_MMS_COMPOSE_NEWMSG_SCREEN);
	}
}


JC_RETCODE jMms_ComposeAutoSignature(void)
{
	JC_RETCODE rCode = JC_OK;
	JC_UINT8  ucTotalPages;
	JC_CHAR*  pszSignBuff;
	
	/*Only be used from Compose/reply forward */
	if(g_pstJMmsAppInfo->stMMSSetting.bIsAutoSignatureOn && !g_pstJMmsAppInfo->bIsAutoSignatureAdded)
	{		
		rCode = jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &ucTotalPages) ;
		if(JC_OK == rCode)
		{
			rCode = jdi_MsgAddPage(g_pstJMmsAppInfo->hCurrMsg,ucTotalPages+1) ;
			if(JC_OK == rCode)
			{
				pszSignBuff = jdi_CUtilsTcsDuplicate(JC_NULL, (JC_CHAR*)g_pstJMmsAppInfo->stMMSSetting.pszAutoSignature);				
				jdi_MsgSetPageTimer(g_pstJMmsAppInfo->hCurrMsg,ucTotalPages+1,g_pstJMmsAppInfo->stMMSSetting.uiDefaultSlideTime); 
				//Add MediaText
				rCode = AddTextMedia(pszSignBuff, ucTotalPages+1) ;
				if(JC_OK == rCode)
				{
					g_pstJMmsAppInfo->bIsAutoSignatureAdded = E_TRUE;
				}
				else
				{
					jdi_MsgDeletePage(g_pstJMmsAppInfo->hCurrMsg,ucTotalPages+1);
				}
			}
		}
	}
	return rCode;
}


void jMms_composecreateMode_HiliteHanlder (S32 index)
{
	g_pstJMmsAppInfo->iHilitedItem = index ;
}

void jMms_compose_createModeDone ()
{
	INT iErr ;

	iErr = jdi_MsgSetCreationMode (g_pstJMmsAppInfo->hCurrMsg, \
		g_pstJMmsAppInfo->iHilitedItem) ;

	mmi_trace (1, "@@@ Creat Mode Set to: %u=%d @@@", g_pstJMmsAppInfo->hCurrMsg, \
		g_pstJMmsAppInfo->iHilitedItem) ;
	
	if (!iErr)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, \
			1, 2000, 13);
		g_pstJMmsAppInfo->iHilitedItem = 0 ;
		DeleteUptoScrID(SCR_ID_MMS_ADDRESS_SCREEN);
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, \
			1, 2000, 1);
	}
}


void jMms_compose_createMode_handler()
{
	/* This list is symmetric to ECreationMode */	
	U16 item_list [] = {
		STR_ID_MMS_FREE,
		STR_ID_MMS_WARNING,
		STR_ID_MMS_RESTRICTED
		} ;
	U8* guiBuffer;
	ECreationMode	eCreateMode	= E_CREATION_MODE_FREE ;

	g_pstJMmsAppInfo->iHilitedItem = 0 ;
	jdi_MsgGetCreationMode (g_pstJMmsAppInfo->hCurrMsg, &eCreateMode) ;
	guiBuffer = GetCurrGuiBuffer(SCR_ID_MMS_CREATE_MODE);
	EntryNewScreen (SCR_ID_MMS_CREATE_MODE, NULL, jMms_compose_createMode_handler, NULL) ;	
	RegisterHighlightHandler (jMms_composecreateMode_HiliteHanlder) ;
	ShowCategory11Screen(STR_ID_MMS_CREATE_MODE, IMG_SMS_ENTRY_SCRN_CAPTION, \
						STR_GLOBAL_OK, 0, \
						STR_GLOBAL_BACK, 0, \
						(sizeof (item_list)/sizeof (U16)), item_list,\
						eCreateMode, guiBuffer) ;

	SetLeftSoftkeyFunction (jMms_compose_createModeDone, KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP) ;		
}

void jMms_composeContClass_HiliteHanlder (S32 index)
{
	g_pstJMmsAppInfo->iHilitedItem = index ;
}

void jMms_compose_contClassDone ()
{
	INT iErr ;

	mmi_trace (1, "@@@ Cont Class Set to: %u=%d @@@", g_pstJMmsAppInfo->hCurrMsg, \
				g_pstJMmsAppInfo->iHilitedItem) ;
	
	iErr = jdi_MsgSetContentClass (g_pstJMmsAppInfo->hCurrMsg, \
									g_pstJMmsAppInfo->iHilitedItem) ;

	if (!iErr)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, \
			1, 2000, 13);
		g_pstJMmsAppInfo->iHilitedItem = 0 ;
		DeleteUptoScrID(SCR_ID_MMS_ADDRESS_SCREEN);
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, \
			1, 2000, 1);
	}
}

void jMms_compose_contentClass_handler () 
{
	/* This list is symmetric to EContentClass */	
	U16 item_list [] = {STR_ID_MMS_CONTENT_CLASS_TEXT,		
						STR_ID_MMS_CONTENT_CLASS_IMAGE_BASIC,
						STR_ID_MMS_CONTENT_CLASS_IMAGE_RICH,
						STR_ID_MMS_CONTENT_CLASS_VIDEO_BASIC,
						STR_ID_MMS_CONTENT_CLASS_VIDEO_RICH
						};
	U8 *guiBuffer;
	EContentClass	eContClass	= E_MMS_CONTENT_CLASS_VIDEO_RICH ;

	g_pstJMmsAppInfo->iHilitedItem = 0 ;
	jdi_MsgGetContentClass (g_pstJMmsAppInfo->hCurrMsg, &eContClass) ;
	guiBuffer = GetCurrGuiBuffer(SCR_ID_MMS_CONTENT_CLASS);
	EntryNewScreen (SCR_ID_MMS_CONTENT_CLASS, NULL, jMms_compose_contentClass_handler, NULL) ;	
	RegisterHighlightHandler (jMms_composeContClass_HiliteHanlder) ;
	ShowCategory11Screen(STR_ID_MMS_CONTENT_CLASS, IMG_SMS_ENTRY_SCRN_CAPTION, \
						STR_GLOBAL_OK, 0, \
						STR_GLOBAL_BACK, 0, \
						(sizeof (item_list)/sizeof (U16)), item_list,\
						eContClass, guiBuffer) ;

	SetLeftSoftkeyFunction (jMms_compose_contClassDone, KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP) ;		
}

JC_RETCODE jmms_MMISetComposeCallback (MSG_HANDLE hMsgHandle)
{
	JC_RETCODE eRet = JC_OK ;

	eRet = jdi_MsgRegisterCallbacks(hMsgHandle, E_CB_ERROR_IND, (void *)jMms_cbError);
	if(eRet != JC_OK )
	{
		return eRet ;
	}
	//eRet = jdi_MsgRegisterCallbacks(hMsgHandle, E_CB_DRM_INFO_LIST, (void *)jmms_DRMMediaListIndicatorCB);
	if(eRet != JC_OK )
	{
		return eRet ;
	}
	return eRet ;
}

#endif //__JATAAYU_APP__

