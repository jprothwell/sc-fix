/***************************************************************************
 *
 * File Name : jwap_httpdownload.c
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
 * @file		jwap_httpdownload.c
 * @ingroup	jWAP
 * @brief	
 *
 **/

#define MODULE_NAME "JWAP"
#define FILE_NAME "jwap_httpdownload.c"
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
#include "jwap_mmi_cb.h"


#include "jdi_cutils.h"
#include "jdi_cutils_uriparse.h"
#include "jwap_interface.h"
#include "jwap_mmi_cb.h"
#include "jwap_httpdownload.h"
#include "jdi_browserengine.h"
#include "jdi_downloadclient.h"
#ifdef CS_DC_ENABLE
#include "jwap_downloadclient.h"
#endif

/***************************************************************************
* Macros
**************************************************************************/
#define CONT_DISPOSITION_HEADER "Content-Disposition"
ST_HTTP_DOWNLOAD_INFO *g_pstHTTP_DownLoad_info ;

/***************************************************************************
* Global variables declarations
**************************************************************************/

/***************************************************************************
* Local Function Prototypes
***************************************************************************/
#ifdef __MMI_GB__
	extern void GBSetInputboxLSKFunction(FuncPtr f); 
#endif

/***************************************************************************
* All Local Function Definitions
***************************************************************************/

JC_RETCODE jwap_DCHandleContentListener (ST_BE_URL_IND *pstBeURLInd)
{
	JC_RETCODE rCode = JC_OK ;

	mmi_trace(12, "jdc_log: appHandleContentListener  type %d", pstBeURLInd->eIndType);

	if(JC_NULL == pstBeURLInd)
	{
		rCode = JC_ERR_INVALID_PARAMETER;
		return rCode;
	}
	
	if(pstBeURLInd->eIndType == E_BE_HEADER_INDICATION)
	{
		
		mmi_trace (1, "jdc_LOG: appHandleContentListener:E_BE_HEADER_INDICATION uiRequestID \
						[%d], rErrorCode [%X], pcContentType [%s]",\
					pstBeURLInd->uiRequestID, -pstBeURLInd->rErrorCode, \
					pstBeURLInd->uBeInd.stBeHeaderInd.pcContentType) ;
#ifdef CS_DC_ENABLE

		if(E_TRUE == jwap_appIsOmaDD (pstBeURLInd->uBeInd.stBeHeaderInd.pcContentType))
		{
			mmi_trace(12, "jdc_log: E_BE_HEADER_INDICATION  begin DC");
			rCode = jwap_HandleDCHeaderIndication(pstBeURLInd) ;
			mmi_trace(12, "jdc_log: E_BE_HEADER_INDICATION  end DC rCode[%d]", rCode);			
		}
		else
#endif			
		{
			mmi_trace(12, "jdc_log: E_BE_HEADER_INDICATION  begin http");
			rCode = HandleHTTPDLHeaderIndication(pstBeURLInd) ;
		}
	}
	else if(pstBeURLInd->eIndType == E_BE_BUFFER_INDICATION)
	{		
		mmi_trace (1, "jdc_log: appHandleContentListener:E_BE_BUFFER_INDICATION uiRequestID \
					[%d], rErrorCode [%X], uiCurrentLength [%d], bHasMoreData [%d]",\
					pstBeURLInd->uiRequestID, -pstBeURLInd->rErrorCode,\
					pstBeURLInd->uBeInd.stBeFetchInd.uiCurrentLength, \
					pstBeURLInd->uBeInd.stBeFetchInd.bHasMoreData) ;

#ifdef CS_DC_ENABLE		
		if (E_TRUE == jwap_isOMAInitialized())
		{
			mmi_trace(12, "jdc_log: DC E_BE_BUFFER_INDICATION  begin");
			rCode = jwap_HandleDCBufferIndication(pstBeURLInd) ;
			mmi_trace(12, "jdc_log: E_BE_BUFFER_INDICATION  end rCode[%d]", rCode);
		}
		
		else
#endif //CS_DC_ENABLE			
		{
			mmi_trace(12, "jdc_log: E_BE_BUFFER_INDICATION  begin hhttp dl");
			rCode = HandleHTTPDLBufferIndication(pstBeURLInd) ;
		}
	}
#ifdef BROWSER_JUDGMENT_OF_CONTENT
	else if (E_BE_CONTENT_CAPABILITY_CHECK_INDICATION == pstBeURLInd->eIndType)
	{
#ifdef CS_DC_ENABLE	
		if (E_TRUE == jwap_appIsOmaDD (pstBeURLInd->uBeInd.pcContentType))
		{
			rCode = JC_OK ;
		}
		else
#endif // CS_DC_ENABLE			
		{
			rCode = JC_ERR_BE_UNSUPPORTED_CONTENT_TYPE ;
		}
	}
#endif
	else if(pstBeURLInd->eIndType == E_BE_ERROR_INDICATION)
	{
		mmi_trace (1, "jdc_log: appHandleContentListener:E_BE_ERROR_INDICATION uiRequestID [%d], rErrorCode [%X]",
			pstBeURLInd->uiRequestID, -pstBeURLInd->rErrorCode) ;
		rCode = HandleErrIndication() ;
	}
	else
	{
		mmi_trace (1, "jWap_HandleHTTPDownload:??? uiRequestID [%d], rErrorCode [%X]",
			pstBeURLInd->uiRequestID, -pstBeURLInd->rErrorCode) ;
	}
	return (rCode) ;
}


/* This function should validate scheme, length, content type etc */
JC_BOOLEAN jWap_isValidContent (ST_BE_URL_IND *pstBeURLInd)
{
	JC_BOOLEAN bRet ;
	ST_BE_HEADER_IND *pstHeaderInd = &pstBeURLInd->uBeInd.stBeHeaderInd ;

	if (!jdi_CUtilsTcsStrStartsWith (pstHeaderInd->pmURL, \
			"http://")) /* As of now, only http scheme is supported */
	{
		bRet = E_FALSE ;
	}
	else
	{
		bRet = E_TRUE ;
	}

	return (bRet) ;
}


JC_CHAR *jWap_httpDlGetFilenameFrmHdr (JC_INT8* pcHeaderInfo)
{
	JC_INT8 *pcTmp, *pcTmp1 ;
	JC_CHAR *pmOut ;

	pmOut = JC_NULL ;
	pcTmp = jdi_CUtilsStrCaseStr (pcHeaderInfo, CONT_DISPOSITION_HEADER) ;
	while (pcTmp && !jdi_CUtilsStrStartsWith (pcTmp, "\r\n"))
	{
		if (jdi_CUtilsStrStartsWith (pcTmp, "filename="))
		{
			pcTmp += jc_strlen ("filename=") ;

			while (*pcTmp == ' ' || *pcTmp == '\t' || *pcTmp == '"') pcTmp ++ ; //Increment pcTmp

			pcTmp1 = pcTmp ;
			while (!jdi_CUtilsStrStartsWith (pcTmp1, "\r\n") &&
					*pcTmp1 != '"' && *pcTmp1 != ';')
			{
				pcTmp1 ++ ;
			}

			jdi_CUtilsTcsStrNCpy (JC_NULL, pcTmp, (pcTmp1 - pcTmp), &pmOut) ;
			break ;
		}
		pcTmp ++ ;
	}

	return (pmOut) ;
}

void jWap_HTTPDownloadNonFeasibilityCallback (void)
{
	HandleNo () ;
}

#ifdef CS_DC_ENABLE
extern ST_DOWNLOAD_CLIENT	*pstDownloadClient ;
extern void jwap_DCNonFeasibilityCallback (void);
#endif//CS_DC_ENABLE

JC_RETCODE jWap_IsFileDownloadPossible (JC_BOOLEAN bOnlyFeasibilityChack)
{
	extern BOOL g_hasMountedTFlashFS ;
	JC_RETCODE rCode = JC_OK ;

	mmi_trace(1,"jdd_log: IsFileDownloadPossible boolean[%d] cardinfo[%d]", \
				bOnlyFeasibilityChack, g_hasMountedTFlashFS);
	if (FALSE == g_hasMountedTFlashFS)
	{
		rCode = JC_ERR_FILE_SYS_INIT ;
		if (E_FALSE == bOnlyFeasibilityChack)
		{
			DisplayPopup(GetString (STR_ID_WAP_NO_MEMORY_CARD),
							IMG_GLOBAL_ERROR, 1, 2000, 1) ;

			jWap_HTTPDownloadNonFeasibilityCallback() ;
		}

	}
	else
	{
		/* if the target filesystem does not have enough memory, then show error */
		FS_INFO			stFSInfo = {0, } ;
		JC_UINT32			uiRet = FS_GetFSInfo (CS_FS_WAP_TCARD_IDENTIFIER, &stFSInfo) ;

		if(g_pstHTTP_DownLoad_info)
		{
			mmi_trace(12,"jdd_log: IsFileDownloadPoss uiRet[%d] size[%d]", \
					uiRet, g_pstHTTP_DownLoad_info->uiTotalSize);
			if (uiRet == ERR_SUCCESS && g_pstHTTP_DownLoad_info && 
				(0 < g_pstHTTP_DownLoad_info->uiTotalSize))
			{
				uiRet = (UINT32)stFSInfo.iTotalSize - (UINT32)stFSInfo.iUsedSize ;
				mmi_trace (1, "JDD_LOG: HandleYes >> HTTP Download Requires Space [%d] and Available Space [%d] in [%s]",
					g_pstHTTP_DownLoad_info->uiTotalSize, uiRet, CS_FS_WAP_TCARD_IDENTIFIER) ;
				
				if (uiRet < (CS_FS_WAP_THRESHOLD_LIMIT + g_pstHTTP_DownLoad_info->uiTotalSize))
				{
					mmi_trace (1, "JDD_LOG: return system full");
					rCode = JC_ERR_FILE_SIZE ;
					if (E_FALSE == bOnlyFeasibilityChack)
					{
						DisplayPopup(GetString (STR_ID_WAP_FILE_SYSTEM_FULL),IMG_GLOBAL_ERROR, 1, 2000, 1) ;
						jWap_HTTPDownloadNonFeasibilityCallback() ;
					}
				}
			}
		}
		#ifdef CS_DC_ENABLE
		else if(g_pstJWapAppInfo->pstDownloadClient)
		{
			mmi_trace(12,"jdc_log: IsFileDownloadPoss uiRet[%d] size[%d]", \
					uiRet, g_pstJWapAppInfo->pstDownloadClient->pstDCUserConfirmation->pstDD->iSize);
			if (uiRet == ERR_SUCCESS && g_pstJWapAppInfo->pstDownloadClient && 
				(0 < g_pstJWapAppInfo->pstDownloadClient->pstDCUserConfirmation->pstDD->iSize))
			{
				uiRet = (UINT32)stFSInfo.iTotalSize - (UINT32)stFSInfo.iUsedSize ;
				mmi_trace (1, "jdc_log: HandleYes >> HTTP Download Requires Space [%d] and Available Space [%d] in [%s]",
					g_pstJWapAppInfo->pstDownloadClient->pstDCUserConfirmation->pstDD->iSize, uiRet, CS_FS_WAP_TCARD_IDENTIFIER) ;
				
				if (uiRet < (CS_FS_WAP_THRESHOLD_LIMIT + g_pstJWapAppInfo->pstDownloadClient->pstDCUserConfirmation->pstDD->iSize))
				{
					mmi_trace (1, "jdc_log: return system full");
					rCode = JC_ERR_FILE_SIZE ;
					if (E_FALSE == bOnlyFeasibilityChack)
					{
						DisplayPopup(GetString (STR_ID_WAP_FILE_SYSTEM_FULL),IMG_GLOBAL_ERROR, 1, 2000, 1) ;
						jwap_DCNonFeasibilityCallback() ;
					}
				}
			}
		}
		#endif
	}
	mmi_trace (1, "JDD_LOG: IsFileDownloadPossible rCode[%x]", -rCode);
	return rCode ;
}

void jWap_freeDownloadInfo ()
{
	mmi_trace (1, "JDD_LOG: Inside jWap_freeDownloadInfo ()") ;
	if (g_pstHTTP_DownLoad_info)
	{
		if (g_pstHTTP_DownLoad_info->pszURL)
		{
			jdd_MemFree (g_pstHTTP_DownLoad_info->pszURL) ;
		}
		if (g_pstHTTP_DownLoad_info->pmContentSize)
		{
			jdd_MemFree (g_pstHTTP_DownLoad_info->pmContentSize) ;
		}
		if (g_pstHTTP_DownLoad_info->pmContentType)
		{
			jdd_MemFree (g_pstHTTP_DownLoad_info->pmContentType) ;
		}
		if (JC_NULL != g_pstHTTP_DownLoad_info->pmTempFileName)
		{
			if (JC_OK != g_pstHTTP_DownLoad_info->rCode ||
					E_TRUE == g_pstHTTP_DownLoad_info->bHasUserCanceled ||
					E_WAP_EXIT_KEY_END == g_pstJWapAppInfo->stWapState.eWapExitKeyPressed)
			{
				jdd_FSDelete (JC_NULL, g_pstHTTP_DownLoad_info->pmTempFileName) ;
				mmi_trace (1, "JDD_LOG: Deleted the download temporary file") ;
			}
			jdd_MemFree (g_pstHTTP_DownLoad_info->pmTempFileName) ;
		}
		if (JC_NULL != g_pstHTTP_DownLoad_info->pmFinalFileName)
		{
			jdd_MemFree (g_pstHTTP_DownLoad_info->pmFinalFileName) ;
		}
		jdd_MemFree (g_pstHTTP_DownLoad_info) ;	
		g_pstHTTP_DownLoad_info = NULL ;
		jc_memset (g_pstJWapAppInfo->pcInputBuff, 0, jWAP_MAX_INPUT_SIZE) ;
	}
}


JC_RETCODE HandleHTTPDLHeaderIndication (ST_BE_URL_IND *pstBeURLInd)
{
	JC_RETCODE	rCode = JC_OK ;
	JC_INT32	iDelimLen ;
	
	mmi_trace (1, "JDD_LOG: HandleHTTPDLHeaderIndication. begin") ;
	if (!g_pstHTTP_DownLoad_info)
	{
		g_pstHTTP_DownLoad_info = (ST_HTTP_DOWNLOAD_INFO *)jdd_MemAlloc ( \
										1,sizeof(ST_HTTP_DOWNLOAD_INFO)) ;
		if (g_pstHTTP_DownLoad_info)
		{
			g_pstHTTP_DownLoad_info->rCode = JC_ERR_INVALID_STATE ;
			g_pstHTTP_DownLoad_info->bHasEndPageBeenCalled = E_FALSE ;
			g_pstHTTP_DownLoad_info->bHasUserCanceled = E_FALSE ;
			g_pstHTTP_DownLoad_info->bHasUserConfirmed = E_FALSE ;
			if (jWap_isValidContent (pstBeURLInd))
			{
				/* Copy Header info */
				g_pstHTTP_DownLoad_info->uiRequestID = pstBeURLInd->uiRequestID ;
				g_pstHTTP_DownLoad_info->uiTotalSize = \
					pstBeURLInd->uBeInd.stBeHeaderInd.uiContentLength ;
				g_pstHTTP_DownLoad_info->pszURL = jdi_CUtilsTcsDuplicate (0, \
					pstBeURLInd->uBeInd.stBeHeaderInd.pmURL) ;

				mmi_trace (1, "JDD_LOG: DL: ContentType %s", pstBeURLInd->uBeInd.stBeHeaderInd.pcContentType) ;
				/*jdi_CUtilsCharToTchar (0, pstBeURLInd->uBeInd.stBeHeaderInd.pcContentType, \
					&g_pstHTTP_DownLoad_info->pmContentType) ;*/
					
				/* Store only Content Type. Skip Charset, etc */
				iDelimLen =	jdi_CUtilsStrIndexOf (pstBeURLInd->uBeInd.stBeHeaderInd.pcContentType, \
								';', 0) ;
				if (0 > iDelimLen)
				{
					iDelimLen = jc_strlen (pstBeURLInd->uBeInd.stBeHeaderInd.pcContentType) ;
				}
				g_pstHTTP_DownLoad_info->pmContentType = jdd_MemAlloc (sizeof (JC_CHAR), \
					iDelimLen + 1) ;

				jdi_CUtilsCharToTcharNMem (pstBeURLInd->uBeInd.stBeHeaderInd.pcContentType, \
					jc_strlen (pstBeURLInd->uBeInd.stBeHeaderInd.pcContentType), \
					iDelimLen + 1, g_pstHTTP_DownLoad_info->pmContentType) ;

				g_pstHTTP_DownLoad_info->pmFinalFileName = jWap_httpDlGetFilenameFrmHdr ( \
					pstBeURLInd->uBeInd.stBeHeaderInd.pucHeader) ;

				rCode = jWap_IsFileDownloadPossible (E_TRUE) ;
				if (JC_OK == rCode)
				{
					g_pstHTTP_DownLoad_info->pmTempFileName = jWap_GetUniqueFileName (
						CS_WAP_DOWLOAD_TEMP_PATH) ;
					if (g_pstHTTP_DownLoad_info->pmTempFileName)
					{
						JC_CHAR *pmFolder ;
						mmi_trace (1, "JDD_LOG: HandleHTTPDLHeaderIndication temp download filename formed") ;
						rCode = jdi_CUtilsCharToTchar (JC_NULL, CS_WAP_DOWLOAD_PATH, &pmFolder) ;
						if (JC_OK == rCode)
						{
							if (E_FALSE == jdd_FSIsDirExist (JC_NULL, pmFolder))
							{
								rCode = jdd_FSMakeDir (JC_NULL, pmFolder) ;
								mmi_trace (1, "JDD_LOG: HandleHTTPDLHeaderIndication >> wap download folder created [%X]", rCode) ;
							}
							jdd_MemFree (pmFolder) ;
						}
					}
					g_pstHTTP_DownLoad_info->rCode = JC_ERR_WAIT_USER_ACTION ;
					GetUserConfirmation () ;
				}
				else
				{
					ShowUserConfirmation () ;
				}
			}
			else
			{
				rCode = JC_ERR_NOT_SUPPORTED ;
				DisplayPopup ((U8*)GetString (STR_GLOBAL_INVALID), IMG_GLOBAL_ERROR, 0, 2000, 1) ;
			}
		}
		else
		{
			rCode = JC_ERR_MEMORY_ALLOCATION ;
		}
	}
	else if (g_pstHTTP_DownLoad_info->uiRequestID == pstBeURLInd->uiRequestID)
	{
		/*	This case was found when we download one .txt file from an email id. The function
			jdi_CUtilsJudgeContent () is not able to judge the content type and is returning JC_NULL.
			Because of this bSendHeader is set to TRUE and the header indication is sent again after calling
			jdi_BEResumeRequest (). In jB5, it is checked that if one download url is set, then just JC_OK is
			returned. Here just one more validation is done comparing the request ID.	*/
			rCode = JC_OK ;
			mmi_trace (1, "JDD_LOG: Header Indication comes for the second time for uiRequestID [%d]",
				g_pstHTTP_DownLoad_info->uiRequestID) ;
	}
	else
	{
		/* Already Download is in progress. Send Stop to current one */
		/* This Condition should never arise as UI for downloading is blocking */
		mmi_trace (1, "JDD_LOG: MORE THAN ONE DOWNLOADS AT A TIME >> ASSERT Case") ;
		rCode = JC_ERR_NOT_SUPPORTED;		
	}
	mmi_trace (1, "JDD_LOG: HandleHTTPDLHeaderIndication end");

	return (rCode) ;
}


JC_RETCODE HandleHTTPDLBufferIndication(ST_BE_URL_IND *pstBeURLInd)
{
	JC_RETCODE rCode ;
	mmi_trace (1, "JDD_LOG: HHTp Buff Ind len[%d] size[%d] moredata[%d]", \
				pstBeURLInd->uBeInd.stBeFetchInd.uiCurrentLength,
				g_pstHTTP_DownLoad_info->uiTotalSize,
				pstBeURLInd->uBeInd.stBeFetchInd.bHasMoreData);

	rCode = JC_OK ;
	if (g_pstHTTP_DownLoad_info && (JC_NULL != g_pstHTTP_DownLoad_info->pmTempFileName))
	{
		if (pstBeURLInd->uBeInd.stBeFetchInd.uiCurrentLength)
		{
			rCode = jWap_SaveToFile (g_pstHTTP_DownLoad_info->pmTempFileName,
				pstBeURLInd->uBeInd.stBeFetchInd.uiCurrentLength,
				pstBeURLInd->uBeInd.stBeFetchInd.pucBuffer) ;
			
			mmi_trace (1, "JDD_LOG: jWap_SaveToFile rCode[%d]", rCode) ;
			g_pstHTTP_DownLoad_info->uiCurrSize += pstBeURLInd->uBeInd.stBeFetchInd.uiCurrentLength;	
		}
		if(E_FALSE == pstBeURLInd->uBeInd.stBeFetchInd.bHasMoreData)
		{
			mmi_trace (1, "jdc_log: HandleHTTPDLBufferIndication () >> g_pstHTTP_DownLoad_info->bHasUserConfirmed [%d]",
				g_pstHTTP_DownLoad_info->bHasUserConfirmed) ;
			g_pstHTTP_DownLoad_info->rCode = JC_OK ;
			ChangeTitleString (NULL) ;
			draw_title();
			if (E_TRUE == g_pstHTTP_DownLoad_info->bHasUserConfirmed)
			{
				jdd_FSRename (JC_NULL,
					g_pstHTTP_DownLoad_info->pmTempFileName,
					g_pstHTTP_DownLoad_info->pmFinalFileName) ;
				jWap_freeDownloadInfo () ;
				jWap_PopupCallBack () ;
				DisplayPopup ((PU8)GetString(STR_ID_WAP_HTTPDWN_DWNLOAD_SUCESS), \
					IMG_GLOBAL_ACTIVATED, 1, 2000, 1) ;
			}
			else
			{
				mmi_trace (1, "HandleHTTPDLBufferIndication () >> File Download Complte. Waiting for User Confirmation") ;
			}						
		}		 
	}	
	else
	{
		/* Show the Information screen showing that the file can not be downloaded */
	}

	{
		U16 currid;
		JC_BOOLEAN bispre = E_FALSE;
		currid = GetActiveScreenId();
		bispre = IsScreenPresent(SCR_ID_WAP_DL_SHOW_DOWNLOADING);
		mmi_trace(1, "jdc_log: DCNotifyUser curr[%d] currpage[%d] ispre[%d]", \
				currid, SCR_ID_WAP_DL_SHOW_DOWNLOADING, bispre);
		
	}
	if(SCR_ID_WAP_DL_SHOW_DOWNLOADING == GetActiveScreenId())
	{
		JC_CHAR	Title[32] = {0,};
		JC_INT8 cTempName[32] = {0,};
		JC_INT8 cUnknownStr[32] = {0,};
		
		if(1024 <= g_pstHTTP_DownLoad_info->uiTotalSize )
		{
			jc_sprintf(cTempName, "%d KB/%d KB", \
					(g_pstHTTP_DownLoad_info->uiCurrSize/1024),
					(g_pstHTTP_DownLoad_info->uiTotalSize/1024));
			jdi_CUtilsCharToTcharMem(cTempName, 32, Title);
		}
		else if ((0 < g_pstHTTP_DownLoad_info->uiTotalSize) && 
				(1024 > g_pstHTTP_DownLoad_info->uiTotalSize))
		{
			
			jc_sprintf(cTempName, "%d B/%d B", \
					(g_pstHTTP_DownLoad_info->uiCurrSize),
					(g_pstHTTP_DownLoad_info->uiTotalSize));
			jdi_CUtilsCharToTcharMem(cTempName, 32, Title);
		}
		
		else if(0 >= g_pstHTTP_DownLoad_info->uiTotalSize )
		{
			jdi_CUtilsTcharToCharMem((JC_CHAR*)GetString(STR_ID_WAP_UNKNOWN_SIZE), 128, cUnknownStr);		
			csd_wsprintf(Title,"%d B/%s",(g_pstHTTP_DownLoad_info->uiCurrSize), cUnknownStr );
			jdi_CUtilsCharToTcharMem(cTempName, 32, Title);
		}
		mmi_trace(1, "jdc_log: title[%s]", cTempName);
		ChangeTitleString ((U8*)Title) ;
		draw_title();
	}
	
	mmi_trace (1, "JDD_LOG: HandleHTTPDLBufferIndication. end") ;
	return (rCode) ;
}

void HandleProgressCancel()
{
	mmi_trace (1, "JDD_LOG: Inside HandleProgressCancel () with uiRequestID [%d]",
		g_pstHTTP_DownLoad_info->uiRequestID) ;
	if (0 != g_pstHTTP_DownLoad_info->uiRequestID)
	{
		jdi_BEStopRequest(g_pstJWapAppInfo->hBECore) ;
		g_pstHTTP_DownLoad_info->uiRequestID = 0 ;
		g_pstHTTP_DownLoad_info->bHasUserConfirmed = E_TRUE ;
		g_pstHTTP_DownLoad_info->bHasUserCanceled = E_TRUE ;
	}
}

JC_RETCODE HandleErrIndication()
{
	JC_RETCODE rCode = JC_OK;
	mmi_trace (1, "JDD_LOG: DL Error Indication g_pstHTTP_DownLoad_info [%X]", g_pstHTTP_DownLoad_info) ;
	if (g_pstHTTP_DownLoad_info)
	{
		if (E_TRUE == g_pstHTTP_DownLoad_info->bHasUserConfirmed)
		{
			jWap_freeDownloadInfo () ;
			jWap_PopupCallBack () ;
		}
		else
		{
			mmi_trace (1, "JDD_LOG: HandleErrIndication () while g_pstHTTP_DownLoad_info->rCode [%X]",
				-g_pstHTTP_DownLoad_info->rCode) ;
			g_pstHTTP_DownLoad_info->rCode = JC_ERR_GENERAL_FAILURE ;
		}
	}
	else
	{
		mmi_trace (1, "JDD_LOG: HandleErrIndication () >> jWap_freeDownloadInfo () and jWap_PopupCallBack () has already been called") ;
	}
	return (rCode) ;
}

void ShowUserConfirmation ()
{
	U8 *guiBuffer;
	S8				downloadMsg [200] = {0, } ;
	JC_RETCODE	rCode ;

	mmi_trace (1, "JDD_LOG: ShowUserConfirmation. begin");
	/* Copy "Size:" string. 5 Bytes */
	jc_wstrcpy (downloadMsg, GetString (STR_ID_WAP_DL_CONTENT_SIZE)) ;
	jdi_CUtilsTcsStrCat (": ", (JC_CHAR*) downloadMsg) ;

	 
	if (g_pstHTTP_DownLoad_info->uiTotalSize)
	{
		if (g_pstHTTP_DownLoad_info->uiTotalSize < 1024)
		{
			g_pstHTTP_DownLoad_info->pmContentSize = jdi_CUtilsItoT (0, \
				g_pstHTTP_DownLoad_info->uiTotalSize) ;
			jc_wstrncat ( (JC_CHAR*)downloadMsg, g_pstHTTP_DownLoad_info->pmContentSize, \
				10) ;
			jdi_CUtilsTcsStrCat ("B", (JC_CHAR*)downloadMsg) ;
		}
		else
		{
			g_pstHTTP_DownLoad_info->pmContentSize = jdi_CUtilsItoT (0, \
				(g_pstHTTP_DownLoad_info->uiTotalSize/1024)) ;
			jc_wstrncat ( (JC_CHAR*)downloadMsg, g_pstHTTP_DownLoad_info->pmContentSize, \
				10) ;	
			jdi_CUtilsTcsStrCat ("K", (JC_CHAR*)downloadMsg) ;
		}
	}
	
	jdi_CUtilsTcsStrCat ("\n",  (JC_CHAR*)downloadMsg) ;
		
	jc_wstrcat ( (JC_CHAR*)downloadMsg,  (JC_CHAR*)GetString (STR_ID_WAP_DL_CONTENT_TYPE)) ;
	jdi_CUtilsTcsStrCat (": ", (JC_CHAR*) downloadMsg) ;
	if (g_pstHTTP_DownLoad_info->pmContentType)
	{
		jc_wstrncat( (JC_CHAR*)downloadMsg, g_pstHTTP_DownLoad_info->pmContentType, 50) ;
	}	


	jdi_CUtilsTcsStrCat ("\n", (JC_CHAR*) downloadMsg) ;
	rCode = jWap_IsFileDownloadPossible (E_TRUE) ;
	if (JC_ERR_FILE_SYS_INIT == rCode)
	{
		jc_wstrcat ( (JC_CHAR*)downloadMsg,  (JC_CHAR*)GetString (STR_ID_WAP_NO_MEMORY_CARD)) ;
	}
	else if (JC_ERR_FILE_SIZE == rCode)
	{
		jc_wstrcat ( (JC_CHAR*)downloadMsg,  (JC_CHAR*)GetString (STR_ID_WAP_FILE_SYSTEM_FULL)) ;
	}

#if 0

	DisplayConfirm(STR_GLOBAL_OK,IMG_GLOBAL_OK,STR_GLOBAL_BACK,IMG_GLOBAL_BACK,(UI_string_type)downloadMsg,0,1);
	SetLeftSoftkeyFunction (HandleNo, KEY_EVENT_UP) ;
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
#else
	EntryNewScreen (SCR_ID_WAP_DL_CONFIRMATION, NULL, ShowUserConfirmation, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_DL_CONFIRMATION) ; 
	SetParentHandler (SCR_ID_WAP_HOMEPAGE_SCREEN) ;
	ShowCategory165Screen (STR_GLOBAL_OK, IMG_GLOBAL_OK, \
		0, 0, \
		(UI_string_type)downloadMsg, IMG_GLOBAL_UNFINISHED, guiBuffer) ;
	SetLeftSoftkeyFunction (HandleNo, KEY_EVENT_UP) ;
	
#endif	
	mmi_trace (1, "JDD_LOG: ShowUserConfirmation. end,rCode is %x",rCode);
}



void GetUserConfirmation()
{
	S8 downloadMsg [200] = {0, } ;
	U8 *guiBuffer;
	mmi_trace (1, "JDD_LOG: GetUserConfirmation. begin");
	/* Copy "Size:" string. 5 Bytes */
	jc_wstrcpy (downloadMsg, GetString (STR_ID_WAP_DL_CONTENT_SIZE)) ;
	jdi_CUtilsTcsStrCat (": ", (JC_CHAR*) downloadMsg) ;

	 
	if (g_pstHTTP_DownLoad_info->uiTotalSize)
	{
		if (g_pstHTTP_DownLoad_info->uiTotalSize < 1024)
		{
			g_pstHTTP_DownLoad_info->pmContentSize = jdi_CUtilsItoT (0, \
				g_pstHTTP_DownLoad_info->uiTotalSize) ;
			jc_wstrncat ( (JC_CHAR*)downloadMsg, g_pstHTTP_DownLoad_info->pmContentSize, \
				10) ;
			jdi_CUtilsTcsStrCat ("B", (JC_CHAR*)downloadMsg) ;
		}
		else
		{
			g_pstHTTP_DownLoad_info->pmContentSize = jdi_CUtilsItoT (0, \
				(g_pstHTTP_DownLoad_info->uiTotalSize/1024)) ;
			jc_wstrncat ( (JC_CHAR*)downloadMsg, g_pstHTTP_DownLoad_info->pmContentSize, \
				10) ;	
			jdi_CUtilsTcsStrCat ("K", (JC_CHAR*)downloadMsg) ;
		}
	}	
	else
	{
		g_pstHTTP_DownLoad_info->pmContentSize = (JC_CHAR*)jdd_MemAlloc(sizeof (JC_CHAR), \
													(jc_tcslen (GetString (STR_ID_WAP_UNKNOWN_SIZE))+1) );
		jc_wstrcpy(g_pstHTTP_DownLoad_info->pmContentSize, GetString (STR_ID_WAP_UNKNOWN_SIZE)); 		
		jc_wstrcat ( (JC_CHAR*)downloadMsg, g_pstHTTP_DownLoad_info->pmContentSize) ;
	}
	
	jdi_CUtilsTcsStrCat ("\n",  (JC_CHAR*)downloadMsg) ;
		
	jc_wstrcat ( (JC_CHAR*)downloadMsg,  (JC_CHAR*)GetString (STR_ID_WAP_DL_CONTENT_TYPE)) ;
	jdi_CUtilsTcsStrCat (": ", (JC_CHAR*) downloadMsg) ;
	if (g_pstHTTP_DownLoad_info->pmContentType)
	{
		jc_wstrncat( (JC_CHAR*)downloadMsg, g_pstHTTP_DownLoad_info->pmContentType, 50) ;
	}	


	jdi_CUtilsTcsStrCat ("\n", (JC_CHAR*) downloadMsg) ;
	jc_wstrcat ( (JC_CHAR*)downloadMsg,  (JC_CHAR*)GetString (STR_ID_WAP_HTTPDWN_DWNLOAD_CONFIRM)) ;

#if 0
	DisplayConfirm(STR_GLOBAL_YES,IMG_GLOBAL_YES,STR_GLOBAL_NO,IMG_GLOBAL_NO,(UI_string_type)downloadMsg,IMG_GLOBAL_QUESTION,1);
	SetLeftSoftkeyFunction (HandleYes, KEY_EVENT_UP);
	SetRightSoftkeyFunction (HandleNo, KEY_EVENT_UP);
#else
	EntryNewScreen (SCR_ID_WAP_DL_CONFIRMATION, NULL, GetUserConfirmation, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_DL_CONFIRMATION) ; 
	SetParentHandler (SCR_ID_WAP_HOMEPAGE_SCREEN) ;

	ShowCategory119Screen (STR_GLOBAL_YES, IMG_GLOBAL_YES, \
		STR_GLOBAL_NO, IMG_GLOBAL_NO, \
		(U8 *)downloadMsg, IMG_GLOBAL_QUESTION, guiBuffer) ;	

	SetLeftSoftkeyFunction (HandleYes, KEY_EVENT_UP);
	SetRightSoftkeyFunction (HandleNo, KEY_EVENT_UP);
#endif	
	mmi_trace (1, "JDD_LOG: GetUserConfirmation. end");
}

/* This functins handles very simple case very there is not query string, etc */
void jWap_getFileNameFromUrl (JC_CHAR *pmDestFileName)
{
	JC_INT8*	pcFileName, *pcTmp, *pcTmp2 ;
	JC_INT32	aLength ;
	
	if(g_pstHTTP_DownLoad_info && g_pstHTTP_DownLoad_info->pszURL)
	{
		jdi_CUtilsTcharToChar(NULL,g_pstHTTP_DownLoad_info->pszURL,&pcFileName) ;
	}
	#ifdef CS_DC_ENABLE
	else if(g_pstJWapAppInfo->pstDownloadClient->pstDCUserConfirmation->pstDD->pmName)
	{
		jdi_CUtilsTcharToChar(NULL, g_pstJWapAppInfo->pstDownloadClient->pstDCUserConfirmation->pstDD->pmName, &pcFileName) ;
	}
	#endif
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

		mmi_trace (1, "JDD_LOG: jWap_getFileNameFromUrl: %s", pcFileName) ;
		jdi_CUtilsCharToTcharNMem  (pcFileName, jc_strlen (pcFileName), \
			JDOWNLOAD_INPUTBOX_FILENAME_MAX, pmDestFileName) ;
		jdi_CUtilsTcsURIUnescape (pmDestFileName) ;
		jdd_MemFree (pcTmp) ;
	}
}


JC_RETCODE jWap_prepareDLFile ()
{
	JC_CHAR 		*pmFolder = JC_NULL ;
	JC_CHAR 		*pmFileName ;
	JC_RETCODE		rCode ;
	JC_INT8		*pcExt, *pcContType ;
	JC_INT32		iFileSize ;
	
	if(0 == jc_wstrlen (jWap_getInputBuff ())||*jWap_getInputBuff () == '.')
	{
		rCode = JC_ERR_FILE_NOT_FOUND ;
	}
	else
	{
		pcContType = JC_NULL ;
		pcExt = JC_NULL ;
		
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
		
			jdd_MiscGetContentTypeForExt ((JC_CHAR *)g_pstJWapAppInfo->pcInputBuff, \
				(const JC_INT8**)&pcContType) ;
	
			if (!pcContType) /* There is no valid extension */
			{
				jdi_CUtilsTcharToChar (0, g_pstHTTP_DownLoad_info->pmContentType, \
					&pcContType) ;
				if (pcContType)
				{
					jdd_MiscGetExtForContentType (pcContType, (const JC_INT8**)&pcExt) ;
					mmi_trace (1, "JDD_LOG: jWap_prepareDLFile >> Download content type is [%s]",
								pcContType) ;
					jdd_MemFree (pcContType) ;
					pcContType = JC_NULL ;
				}
				if (pcExt)
				{
					iFileSize += jc_strlen (pcExt) ;
				}
			}
			else
			{
				mmi_trace (1, "JDD_LOG: jWap_prepareDLFile >> From extention, the content type is [%s]",
					pcContType) ;
			}
			pmFileName = jdd_MemAlloc (sizeof (JC_CHAR), iFileSize + 1) ;
			if (pmFileName)
			{
				rCode = JC_OK ;
				jc_wstrcpy (pmFileName, pmFolder) ;
				jdi_CUtilsTcsStrCat (CFG_PATH_SEPARATOR, pmFileName) ;
				jc_wstrcat (pmFileName, (JC_CHAR*)g_pstJWapAppInfo->pcInputBuff) ; 
			}
			else
			{
				rCode = JC_ERR_MEMORY_ALLOCATION ;
			}
		}
	}

	if (!rCode)
	{
		if (pcExt)
		{
			jdi_CUtilsTcsStrCat (pcExt, pmFileName) ;
		}
		
		if (jdd_FSIsFileExist (JC_NULL, pmFileName))
		{
			rCode = JC_ERR_FILE_DIR_ALREADY_EXIST ;
		}
		else
		{
			if (g_pstHTTP_DownLoad_info->pmFinalFileName)
			{
				jdd_MemFree (g_pstHTTP_DownLoad_info->pmFinalFileName) ;
			}
			g_pstHTTP_DownLoad_info->pmFinalFileName = pmFileName ;
			g_pstHTTP_DownLoad_info->bHasUserConfirmed = E_TRUE ;
		}
	}
	jdd_MemFree (pmFolder) ;
	return (rCode) ;
}

void jWap_showDownloading(void)
{
	U8* guiBuffer = JC_NULL;
	EntryNewScreen (SCR_ID_WAP_DL_SHOW_DOWNLOADING, 0, jWap_showDownloading, 0) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_DL_SHOW_DOWNLOADING);
	SetParentHandler (SCR_ID_WAP_HOMEPAGE_SCREEN);

	ShowCategory119Screen ( 0, 0,
							STR_GLOBAL_CANCEL, 0, 
							(U8*)get_string (STR_ID_WAP_HTTPDWN_DWNLD_PROGRESS),
							IMG_GLOBAL_INFO, guiBuffer);

	SetRightSoftkeyFunction (HandleProgressCancel, KEY_EVENT_UP ) ;
}
void jWap_dlStartDownloading ()
{
	JC_RETCODE		iErr ;

	iErr = jWap_prepareDLFile () ;	

	if (!iErr)
	{	
		jWap_showDownloading();
		mmi_trace (1, "JDD_LOG: jWap_dlStartDownloading () >> rDode[%d]", \
					g_pstHTTP_DownLoad_info->rCode) ;
		
		if (JC_OK == g_pstHTTP_DownLoad_info->rCode)
		{
			mmi_trace (1, "JDD_LOG: jWap_dlStartDownloading () >> Download has already successful") ;
			jdd_FSRename (JC_NULL,
					g_pstHTTP_DownLoad_info->pmTempFileName,
					g_pstHTTP_DownLoad_info->pmFinalFileName) ;
				jWap_freeDownloadInfo () ;
				jWap_PopupCallBack () ;
				DisplayPopup ((PU8)GetString(STR_ID_WAP_HTTPDWN_DWNLOAD_SUCESS), \
					IMG_GLOBAL_ACTIVATED, 1, 2000, 1) ;
		}
		else if (JC_ERR_GENERAL_FAILURE == g_pstHTTP_DownLoad_info->rCode)
		{
			mmi_trace (1, "JDD_LOG: jWap_dlStartDownloading () >> Download has already finished with error") ;
			jWap_PopupCallBack () ;
			DisplayPopup(GetString (STR_ID_WAP_UNKNOWN), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		}
		else
		{
			mmi_trace (1, "JDD_LOG: jWap_dlStartDownloading () >> Download is in progress") ;
		}
	}
	else
	{
		GoBackToHistory (SCR_ID_WAP_INPUT_BOX_BK) ;
		jWap_showErrorPopup (iErr) ;
	}

}


void jWap_inputBox_highlightHanlder (S32 index)
{
	switch(index)
	{
		case 0:
			SetLeftSoftkeyFunction (jWap_dlStartDownloading, KEY_EVENT_UP ) ;
			break;
		case 1:
			SetLeftSoftkeyFunction (EntryInputMethodScreen, KEY_EVENT_UP ) ;
			break ;
	}	
}

void jWap_inputBoxLskHandler ()
{
	U16 item_texts[] = {STR_GLOBAL_DONE, \
						//STR_GLOBAL_CANCEL,
						STR_GLOBAL_INPUT_METHOD} ;
	
	U16 item_icons[] = {IMG_GLOBAL_L1, \
						IMG_GLOBAL_L2, \
						//IMG_GLOBAL_L3
						} ;
//	U8* guiBuffer = JC_NULL;
	
	EntryNewScreen (SCR_ID_WIDGET_INPUTBOX_OPTION, 0, 0, 0) ;
//	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_INPUT_BOX_BK);
	SetParentHandler (SCR_ID_WAP_INPUT_BOX_BK);
	
	RegisterHighlightHandler (jWap_inputBox_highlightHanlder);

	ShowCategory52Screen (STR_GLOBAL_OPTIONS, 0, 
		STR_GLOBAL_OK, 0,
		STR_GLOBAL_BACK, 0, 
		2, item_texts, item_icons,
		0, 0, 0, 0);
	
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP ) ;
}


void jWap_getFilenameFromUser ()
{
	U8*			guiBuffer = JC_NULL;
	JC_INT8		*pcFileName ;
	JC_CHAR		*pmFileName = (JC_CHAR *)jWap_getInputBuff() ;
	JC_UINT32	uiFileNameLength ;
	JC_INT32		iIndexOfDotInOrginalName ;
	JC_INT32		iIndexOfDotInFinalName ;

	uiFileNameLength = jc_tcslen (pmFileName) ;
	jdi_CUtilsTcharToChar (JC_NULL, pmFileName, &pcFileName) ;
	if (pcFileName)
	{
		mmi_trace (1, "JDD_LOG: jWap_getFilenameFromUser >> Original File Name is [%s] with Length [%d]",
							pcFileName, uiFileNameLength) ;
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
						(uiFileNameLength - JDOWNLOAD_INPUTBOX_FILENAME_MAX + 1)) ;
		}
		else
		{
			/* The file name length is well within the platform limit. */
		}
	}
	EntryNewScreen (SCR_ID_WAP_INPUT_BOX_BK, JC_NULL, \
		jWap_getFilenameFromUser, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_INPUT_BOX_BK);		
	SetParentHandler (SCR_ID_WAP_DL_CONFIRMATION);		

	ShowCategory5Screen (STR_ID_WAP_HTTPDWN_FILENAME, 0,
		STR_GLOBAL_OPTIONS, 0,
		STR_GLOBAL_BACK, 0,
		INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
		(U8*)pmFileName, JDOWNLOAD_INPUTBOX_FILENAME_MAX, 
		guiBuffer);

	if (pcFileName)
	{
		jdi_CUtilsTcharToCharMem (pmFileName, uiFileNameLength + 1, pcFileName) ;
		mmi_trace (1, "JDD_LOG: jWap_getFilenameFromUser >> Final File Name is [%s]", pcFileName) ;
		jdd_MemFree (pcFileName) ;
	}
#ifdef __MMI_GB__
		GBSetInputboxLSKFunction (jWap_inputBoxLskHandler);
#endif

	SetLeftSoftkeyFunction (jWap_inputBoxLskHandler, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	RegisterInputMethodScreenCloseFunction (GoBackHistory );
}	

void HandleYes ()
{
	JC_RETCODE rCode = JC_OK;
	
	DeleteScreenIfPresent (SCR_ID_WAP_DL_CONFIRMATION) ;
	rCode = jWap_IsFileDownloadPossible (E_FALSE) ;

	mmi_trace(1, "jdd_log: press yes DownloadPossible[%d]", rCode);
	if (JC_OK == rCode)
	{
		if (!g_pstHTTP_DownLoad_info->pmFinalFileName)
		{
			jWap_getFileNameFromUrl ((JC_CHAR*)jWap_getInputBuff()) ;
		}
		else
		{
			jc_wstrncpy ((JC_CHAR*)jWap_getInputBuff(), g_pstHTTP_DownLoad_info->pmFinalFileName, \
				jWAP_MAX_INPUT_SIZE) ;
		}
		jWap_getFilenameFromUser () ;
	}
}

void HandleNo ()
{
	DeleteScreenIfPresent (SCR_ID_WAP_DL_CONFIRMATION) ;
	SetLeftSoftkeyFunction (JC_NULL, KEY_EVENT_UP);
	g_pstHTTP_DownLoad_info->bHasUserCanceled = E_TRUE ;
	mmi_trace(1, "jdd_log: http download pressed no rCode[%d] endpage[%d]", \
				g_pstHTTP_DownLoad_info->rCode, 
				g_pstHTTP_DownLoad_info->bHasEndPageBeenCalled);
	
	if (JC_ERR_WAIT_USER_ACTION == g_pstHTTP_DownLoad_info->rCode)
	{
		if (0 != g_pstHTTP_DownLoad_info->uiRequestID)
		{
			jdi_BEStopRequest(g_pstJWapAppInfo->hBECore) ;
			g_pstHTTP_DownLoad_info->bHasUserConfirmed = E_TRUE ;
			/* To avoid jdi_BEStopRequest () again */
			g_pstHTTP_DownLoad_info->uiRequestID = 0 ;
		}
		mmi_trace (1, "JDD_LOG: HandleNo () >> Download is in progress. So, jdi_BEStopRequest () is called") ;
	}
	else if (E_TRUE == g_pstHTTP_DownLoad_info->bHasEndPageBeenCalled)
	{
		jWap_freeDownloadInfo () ;
		jWap_PopupCallBack () ;
		mmi_trace (1, "JDD_LOG: HandleNo () >> Download has been completed/cancelled, E_BE_END_PAGE_LISTENER had come") ;
	}
	else
	{
		mmi_trace (1, "JDD_LOG: HandleNo () >> Control should not come here") ;
		jWap_freeDownloadInfo () ;
		jWap_PopupCallBack () ;
	}
}

#ifdef CS_DC_ENABLE

JC_BOOLEAN jwap_appIsOmaDD (JC_INT8 *pcContentType)
{
	JC_INT8		*pcType ;
	JC_INT32	iIndex ;
	JC_BOOLEAN	bIsOmaDD ;

	pcType = pcContentType ;
	iIndex = 0 ;
	bIsOmaDD = E_FALSE ;

	mmi_trace(0 , "jdc_log: ContentType[%s]", pcContentType);
	if ((E_TRUE == jdi_CUtilsStrStartsWith (pcContentType, APPLICATION_VND_OMA_DD)))
	{
		bIsOmaDD = E_TRUE ;
	}
	mmi_trace(12, "jdc_log: jwap_appIsOmaDD return %d", bIsOmaDD);
	return bIsOmaDD ;
}
#endif//#ifdef CS_DC_ENABLE
#endif //__JATAAYU_APP__ 

