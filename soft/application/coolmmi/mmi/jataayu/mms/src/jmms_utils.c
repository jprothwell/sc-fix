#define MODULE_NAME "JMMS"
#define FILE_NAME "jmms_utils.c"
 /***************************************************************************
 *
 * File Name : jmms_utils.c
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
 * @file		jmms_utils.c
 * @ingroup		jMMS
 * @brief		Describes the Task related API's.
 *
 **/
 
 #ifdef JATAAYU_SUPPORT
 /***************************************************************************
 * System Include Files
 **************************************************************************/
#include "stdc.h"
#include "profilegprots.h"
#include "datetimegprot.h"
#include "phonesetupgprots.h"
#include "messagesmiscell.h"
#include "phonebookgprot.h"
#include "fs.h"
#include "gpioinc.h" 
#include "dual_sim.h"
/***************************************************************************
* User Include Files
**************************************************************************/

#include "jmms_inc.h"
#include "jmms_cb.h"
#include "jmms_storedlg.h"
#include "jmms_communicator.h"
#include "jmms_composedlg.h"
#include "jmms_composeutils.h"
#include "jmms_addrdlg.h"
#include "jmms_objectpicker.h"

 /***************************************************************************
 * Macros
 **************************************************************************/

#define JMMS_RESERVED_MMS_FILE_NAME			"jMMSReserved"
#define JMMS_RESERVED_MMS_FILE_CHUNK_SIZE	1024
#define JMMS_RESERVED_MMS_FILE_CHUNK_NUM	5

 /***************************************************************************
 * Global variables declarations
 **************************************************************************/
U8 gNeedNewMMSPopOnIdle = E_FALSE;
U32 uiMsgIDofNewMsgPop = 0;
static FuncPtr endKeyDownHanler=NULL;
/***************************************************************************
* Local Function Prototypes
***************************************************************************/

/***************************************************************************
* All Local Function Definitions
***************************************************************************/

/***************************************************************************
* All Global Function Definitions 
**************************************************************************/
 /* Explicitly declared here, but it should be declared in Worldclock.h */
S32 GetTimeZone (U8 cityIndex) ;

static uint8* pFolderName[] = 
{
    "Inbox",
    "Outbox",
    "Sent",
    "Draft"
};

uint8* jmms_GetFolderName(E_JMMS_FOLDER_ID eFolderID)
{
	return (pFolderName[eFolderID]);
}

void jmms_FreeContentData (CONTENT_DATA *pContentData, JC_RETCODE eRet)
{   
    if (pContentData != JC_NULL)
	{
        if (pContentData->bIsStream)
		{
		    if (eRet != JC_OK)
            {
                if (pContentData->pStream != JC_NULL)
		        {
		            jdd_FSDelete(JC_NULL, pContentData->pStream) ;
                }
            }
            jdd_MemFree (pContentData->pStream);
        }
		else
		{
		    if (pContentData->pBuffer)
            {      
		        jdd_MemFree(pContentData->pBuffer) ;
                pContentData->pBuffer = JC_NULL ;
            }
        }
    }
}

JC_RETCODE jmms_MMISetMsgContentStream(E_JMMS_FOLDER_ID eFolderID,
												JC_UINT32 MsgId,CONTENT_DATA* pViewContent)
{
    JC_RETCODE      eRet = -1 ;
    JC_INT32       ContentSize = 0;
    
    ContentSize = jmms_MMIGetMsgSize(eFolderID, MsgId);

    if (ContentSize != -1) 
    {
        //Get Msg Content
        if(ContentSize > JMMS_MAX_USE_BUFFER_SIZE)    
        {
            pViewContent->bIsStream = E_TRUE ;
        }
        else
        {
            pViewContent->bIsStream = E_FALSE ;
        }
        eRet = JC_OK;
    }
        
    return eRet;
    
}

JC_INT32 jmms_MMIGetMsgSize(E_JMMS_FOLDER_ID eFolderID,JC_UINT32 uiMsgIndex)
{
    JC_RETCODE          eRet = JC_OK ;
    MESSAGE_LIST		*pMessageList = JC_NULL;    

    eRet = jmms_GetMsgInfoByID(eFolderID, uiMsgIndex, &pMessageList);
	mmi_trace (1, "JDD_LOG: jmms_GetMsgInfoByID >> Folder ID [%d], MSG Index [%d] and rCode is [%d]",
		eFolderID, uiMsgIndex, -eRet) ;
    if ( eRet != JC_OK )
    {
        return -1;
    }  

    if (pMessageList == JC_NULL)
    {
        return 0;
    }

    if (pMessageList->pMessageHeaders->eMsgType == E_MSG_TYPE_NOTIFICATION)
    {
        MESSAGE_NOTIFICATION *pTempHeaders = (MESSAGE_NOTIFICATION*)pMessageList->pMessageHeaders->pHeaders ;
        //here return the size of msg to be downloaded
        return pTempHeaders->uiMessageSize;
    }
    else
    {
        return pMessageList->uiBufferLen;    
    }
}

JC_RETCODE jmms_GetMsgInfoByID (E_JMMS_FOLDER_ID eFolderID, JC_UINT32 uiMsgId, MESSAGE_LIST **ppstMsgInfo)
{
    MESSAGE_LIST		*pMessageList = JC_NULL, *pTempMsgList = JC_NULL ;
	RETRIEVE_FILTERS	stStoreRetFilter = {0 } ;
    JC_RETCODE			eRet ;
    
    *ppstMsgInfo = JC_NULL ;
	eRet = jmms_SetActiveFolder (eFolderID) ;
	if (JC_OK == eRet)
	{
	    stStoreRetFilter.eRetrieve = E_RETRIEVE_BY_INDEX ;
	    stStoreRetFilter.uiStartIndex = 1 ;
		eRet = jmms_GetMsgList (g_pstJMmsAppInfo->hMsgStore, &pMessageList, &stStoreRetFilter) ;
	}
	if (JC_OK == eRet)
	{
		pTempMsgList = pMessageList ;
	    while (pTempMsgList)
	    { 
	        if (pTempMsgList->uiMessageID == uiMsgId)
	        {
	            *ppstMsgInfo = pTempMsgList ;
	            break;
	        }
	        pTempMsgList = pTempMsgList->pNext ;
	    }
	}
	if (JC_OK != eRet)
	{
		mmi_trace (1, "JDD_LOG: jmms_GetMsgInfoByID >> Folder ID [%d], MSG ID [%d], rCode [%X]",
			eFolderID, uiMsgId, -eRet) ;
	}
    return eRet;
}


JC_RETCODE jmms_SetActiveFolder (E_JMMS_FOLDER_ID eFolderID)
{
	JC_RETCODE	rCode ;
	JC_CHAR	pszFolderName [JMMS_MAX_FOLDER_NAME_LEN] ;
	if (E_JMMS_NONE_FOLDER != eFolderID)
	{
		jdi_CUtilsCharToTcharMem(pFolderName[eFolderID],JMMS_MAX_FOLDER_NAME_LEN,pszFolderName) ;
		rCode = jdi_StoreSetActiveFolder (g_pstJMmsAppInfo->hMsgStore, pszFolderName) ;
		mmi_trace (1, "JDD_LOG: jdi_StoreSetActiveFolder () returned [%X] set folder %d", rCode, eFolderID) ;
	}
	else
	{
		rCode = JC_ERR_INVALID_PARAMETER ;
		mmi_trace (1, "JDD_LOG: ERROR >> jmms_SetActiveFolder () is called with eFolderID as E_JMMS_NONE_FOLDER") ;
	}
	return rCode ;
}

JC_RETCODE jmms_MoveMmsByID(E_JMMS_FOLDER_ID eSrcFolderID, E_JMMS_FOLDER_ID eDesFolderID, 
				JC_UINT32 msgId)
{
    MESSAGE_LIST		*pMessageList = JC_NULL;
    RETRIEVE_FILTERS	 retFilter = {0, } ;
    JC_RETCODE           eRet = JC_OK;
	//JC_CHAR pszSrcFolderName[JMMS_MAX_FOLDER_NAME_LEN];
	JC_CHAR pszDesFolderName[JMMS_MAX_FOLDER_NAME_LEN];

	

	jdi_CUtilsCharToTcharMem(pFolderName[eDesFolderID],JMMS_MAX_FOLDER_NAME_LEN,pszDesFolderName);
    eRet = jmms_SetActiveFolder(eSrcFolderID);
	if (JC_OK == eRet)
	{
		retFilter.eRetrieve = E_RETRIEVE_BY_INDEX ;
		retFilter.uiStartIndex = 1 ;
		retFilter.uiNumMessage = 0 ;
		eRet = jmms_GetMsgList (g_pstJMmsAppInfo->hMsgStore, &pMessageList, &retFilter) ;
		mmi_trace(1, "JDD_LOG: jmms_MoveMmsByID >> jmms_GetMsgList rCode value is [%d]", eRet) ;
	}

    if (eRet == JC_OK)      
    {
        eRet = jdi_StoreMoveMsg(g_pstJMmsAppInfo->hMsgStore, msgId, 
					pszDesFolderName);
		mmi_trace(1, "JDD_LOG: jdi_StoreMoveMsg eRet %d", eRet);
        if (eRet != JC_OK)
        {
            if(eRet == JC_ERR_STORE_FILE_CORRUPTED)
		    {
			    jdi_StoreRestoreFolderInfo(g_pstJMmsAppInfo->hMsgStore);
		    }
        }
        else
        {
            eRet = jdi_StoreUpdateMsgList(g_pstJMmsAppInfo->hMsgStore, E_FALSE);            
        }
    }
	if (JC_OK != eRet)
	{
		mmi_trace (1, "JDD_LOG: jmms_MoveMmsByID >> eFolderID [%d], msgId [%d]", eSrcFolderID, msgId) ;
	}
    return eRet;
}

JC_RETCODE jmms_GetMsgExtraBytes (STORE_HANDLE hStoreHandle, E_JMMS_FOLDER_ID eFolderID,
	JC_UINT32 uiMsgId, JC_UINT32 *uiExtraBytes)
{
	JC_RETCODE			eRetCode ;
	RETRIEVE_FILTERS	stMsgFilter = {0, } ;
	MESSAGE_LIST		*pstMessageList = JC_NULL ;

	eRetCode = jmms_SetActiveFolder (eFolderID) ;
	if (JC_OK == eRetCode)
	{
		stMsgFilter.eRetrieve	 = E_RETRIEVE_BY_INDEX ;
		stMsgFilter.uiStartIndex = 1;
		stMsgFilter.uiNumMessage = 0 ;
		eRetCode = jmms_GetMsgList (hStoreHandle, &pstMessageList, &stMsgFilter) ;
		if (JC_OK == eRetCode)
		{
			while (pstMessageList)
			{
				if (pstMessageList->uiMessageID == uiMsgId)
				{
					break ;
				}
				pstMessageList = pstMessageList->pNext ;
			}			
			if (pstMessageList == JC_NULL)
			{			
				eRetCode = JC_ERR_UNKNOWN ;
				*uiExtraBytes = 0 ;
			}
			else
			{
				*uiExtraBytes = pstMessageList->uiExtraBytes ;
			}
		}
	}
	if (JC_OK != eRetCode)
	{
		mmi_trace (1, "JDD_LOG: jmms_GetMsgExtraBytes () failed with eRetCode as [-%x]", eRetCode) ;
	}
	return eRetCode ;
}


JC_RETCODE jmms_GetMsgDetailsByID (STORE_HANDLE hStoreHandle, E_JMMS_FOLDER_ID eFolderID,
	JC_UINT32 ucMsgId, MESSAGE_LIST **ppstMsgDetail)
{
	JC_RETCODE			eRetCode ;
	RETRIEVE_FILTERS	stMsgFilter = {0, } ;
	MESSAGE_LIST		*pstMessageList = NULL ;

	eRetCode = jmms_SetActiveFolder (eFolderID) ;
	if (JC_OK == eRetCode)
	{
		stMsgFilter.eRetrieve	 = E_RETRIEVE_BY_INDEX ;
		stMsgFilter.uiStartIndex = 1;
		stMsgFilter.uiNumMessage = 0 ;
		eRetCode = jmms_GetMsgList (hStoreHandle, &pstMessageList, &stMsgFilter) ;
		if (JC_OK == eRetCode)
		{
			while (pstMessageList)
			{
				if (pstMessageList->uiMessageID == ucMsgId)
				{
					break ;
				}
				pstMessageList = pstMessageList->pNext ;
			}
			if (pstMessageList == JC_NULL)
			{
				*ppstMsgDetail = JC_NULL ;
				eRetCode = JC_ERR_UNKNOWN ;
			}
			*ppstMsgDetail = pstMessageList ;
		}
		if (JC_OK != eRetCode)
		{
			mmi_trace (1, "JDD_LOG: jMMS_GetMsgFromStoreByMsgID::jmms_GetMsgList ([%d] failed with rCode [%x]", -eRetCode) ;
		}
	}
	return eRetCode ;
}

JC_RETCODE jMMS_GetMsgFromStoreByMsgID (STORE_HANDLE hStoreHandle, E_JMMS_FOLDER_ID eFolderID,
	JC_UINT32 uiMsgId, CONTENT_DATA *pstContentData)
{
	JC_RETCODE			rCode ;
	RETRIEVE_FILTERS	stMsgFilter = {0, } ;
	MESSAGE_LIST		*pstMessageList = JC_NULL ;
	
	rCode = jmms_SetActiveFolder (eFolderID) ;
	if (JC_OK == rCode)
	{
		stMsgFilter.eRetrieve = E_RETRIEVE_BY_INDEX ;
		stMsgFilter.uiStartIndex= 1 ;
		stMsgFilter.uiNumMessage = 0 ;
		rCode = jmms_GetMsgList (g_pstJMmsAppInfo->hMsgStore, &pstMessageList, &stMsgFilter) ;
	}   
	if (JC_OK == rCode)
	{
		rCode = jdi_StoreGetMsgFromStore (hStoreHandle, uiMsgId, pstContentData) ;
		if (JC_OK != rCode)
		{
			mmi_trace (1, "JDD_LOG: jMMS_GetMsgFromStoreByMsgID::jdi_StoreGetMsgFromStore ([%d]) failed with rCode [%x]", uiMsgId, -rCode) ;
		}
	}
	return rCode ;
}

JC_RETCODE jmms_GetMsgDetailsByIndex (STORE_HANDLE hStoreHandle, E_JMMS_FOLDER_ID eFolderID,
						JC_UINT32 ucMsgIndex, MESSAGE_LIST **ppstMsgDetail)
{
	JC_UINT8			ucLoopCnt = 0 ;
	JC_RETCODE			eRetCode ;
	RETRIEVE_FILTERS	stMsgFilter = {0, } ;
	MESSAGE_LIST		*pstMessageList = JC_NULL ;

	eRetCode = jmms_SetActiveFolder (eFolderID) ;
	if (JC_OK == eRetCode)
	{
		stMsgFilter.eRetrieve	 = E_RETRIEVE_BY_INDEX ;
		stMsgFilter.uiStartIndex = 1;
		stMsgFilter.uiNumMessage = 0 ;
		eRetCode = jmms_GetMsgList (hStoreHandle, &pstMessageList, &stMsgFilter) ;	
		if (JC_OK == eRetCode)
		{
			while (pstMessageList)
			{
				if (ucLoopCnt == ucMsgIndex)
				{
					break ;
				}
				ucLoopCnt++ ;
				pstMessageList = pstMessageList->pNext ;
			}			
			if (pstMessageList == NULL)
			{
				*ppstMsgDetail = JC_NULL ;
				eRetCode = JC_ERR_UNKNOWN ;
			}
			*ppstMsgDetail = pstMessageList ;
		}
	}
	if (JC_OK != eRetCode)
	{
		mmi_trace (1, "JDD_LOG: jmms_GetMsgDetailsByIndex >> Folder ID [%d], MSG Index [%d], rCode [%X]",
			eFolderID, ucMsgIndex, -eRetCode) ;
	}
	return eRetCode ;
}

JC_RETCODE jmms_GetMsgList (STORE_HANDLE hStore, MESSAGE_LIST **ppMessageList, 
						   RETRIEVE_FILTERS *pRetrieveFilters)
{
	JC_RETCODE retCode ;

	retCode = jdi_StoreGetMsgList (hStore, ppMessageList, pRetrieveFilters) ;
	if (retCode == JC_ERR_STORE_FILE_CORRUPTED)
	{
		retCode = jdi_StoreRestoreFolderInfo (hStore);
		if (retCode == JC_OK)
		{
			retCode = jdi_StoreGetMsgList (hStore, ppMessageList, pRetrieveFilters) ;			
		}
	}
	mmi_trace (1, "JDD_LOG: jmms_GetMsgList >> rCode [%X]", -retCode) ;
	return retCode ;
}

JC_BOOLEAN jmms_GetMsgID (E_JMMS_FOLDER_ID eFolderID, JC_UINT8 ucMsgIndex, JC_UINT32 *pucMsgId)
{
	MESSAGE_LIST		*pstMessageList ;	
	RETRIEVE_FILTERS	stMsgFilter = {0, } ;
	JC_BOOLEAN			bIsValid = E_FALSE ;
	JC_UINT8			ucLoopCnt = 0 ;
	JC_RETCODE			rCode ;
	
	rCode = jmms_SetActiveFolder (eFolderID) ;
	if (JC_OK == rCode)
	{
		stMsgFilter.eRetrieve	 = E_RETRIEVE_BY_INDEX ;
		stMsgFilter.uiStartIndex = 1;
		stMsgFilter.uiNumMessage = 0 ;
		rCode = jmms_GetMsgList (g_pstJMmsAppInfo->hMsgStore, &pstMessageList, &stMsgFilter) ;
		if (JC_OK == rCode)
		{
			while (pstMessageList)
			{
				if (ucLoopCnt == ucMsgIndex)
				{
					break ;
				}
				ucLoopCnt++ ;
				pstMessageList = pstMessageList->pNext ;
			}
			if (JC_NULL != pstMessageList)
			{
				*pucMsgId = pstMessageList->uiMessageID ;
				bIsValid = E_TRUE ;
			}
		}
	}
	if (JC_OK != rCode)
	{
		mmi_trace (1, "JDD_LOG: jmms_GetMsgID >> rCode [%X]", -rCode) ;
	}
	return bIsValid ;
}

JC_RETCODE FileStreamReadCB (void *pStream, JC_INT32 uiOffset, JC_UINT32 uiSize, JC_UINT8 *pBuffer, JC_UINT32 *pSizeRead)
{
	JC_RETCODE	eRet = JC_OK ;
	JDD_FILE	hFile = NULL ;

	mmi_trace(1, "@@@ FileStreamReadCB entry");
	if(pStream == NULL || pBuffer == NULL)
	{
		eRet = JC_ERR_INVALID_PARAMETER ;
		goto error ;
	}
	
	hFile = jdd_FSOpen(NULL, (JC_CHAR *)pStream, E_OPEN_READ_MODE) ;
	if(hFile == NULL)
	{
		eRet = JC_ERR_FILE_OPEN ;
		goto error ;
	}
	if(uiOffset > 0)
	{
		eRet = jdd_FSSeek (hFile, uiOffset, E_FILE_SEEK_SET) ;
		if(eRet != JC_OK)
		{
			goto error ;
		}
	}
	*pSizeRead = 0 ;
	*pSizeRead = jdd_FSRead (pBuffer, sizeof(JC_INT8), uiSize, hFile) ;
	if(*pSizeRead != uiSize)
	{
		eRet = JC_ERR_FILE_READ ;
		goto error ;
	}

error :
	if(hFile != NULL)
	{
		jdd_FSClose(hFile) ;
	}
	return eRet ;
	mmi_trace(1, " FileStreamReadCB exit rCode [%d]", eRet);
}

JC_RETCODE jmms_FileStreamWriteCB (void *pStream, JC_UINT8 *pBuffer, JC_UINT32 uiSize)
{
	JC_RETCODE	eRet = JC_OK ;
	JC_INT32	iBytesWritten = 0 ;
	JDD_FILE	hFile = JC_NULL ;
	
	mmi_trace(1, "@@@ jmms_FileStreamWriteCB entry");
	if(pStream == JC_NULL || pBuffer == JC_NULL)
	{
		return JC_ERR_NULL_POINTER ;
	}
    
	if(uiSize == 0)
	{
		return JC_ERR_INVALID_PARAMETER ;
	}

	hFile = jdd_FSOpen(JC_NULL, (JC_CHAR *)pStream, E_CREATE_APPEND_WRITE_MODE) ;
    
	if(hFile != JC_NULL)
	{
    	iBytesWritten = jdd_FSWrite (pBuffer, 1, uiSize, hFile) ;
    	if(iBytesWritten != (JC_INT32)uiSize)
    	{
    		eRet = JC_ERR_FILE_WRITE ;
    	}
    }
    else
    {
        eRet = JC_ERR_FILE_OPEN;
    }

	if(hFile != JC_NULL)
	{
		jdd_FSClose(hFile) ;
	}
	mmi_trace(1, " jmms_FileStreamWriteCB exit rCode [%d]", eRet);
	return eRet ;
} 


// this function is updated the new mms icon on idle screen based read/unread messages in inboc
void jmms_UpdateMmsIcon()
{
	JC_UINT32 ucNUnreadMsg = 0 , ucNTotMsg = 0;
	
	if (jMMS_CheckJMMSFull())
	{
		BlinkStatusIcon(STATUS_ICON_MMS_UNREAD);
	}
	else
	{
		jmms_GetReadMsgStatus(&ucNUnreadMsg, &ucNTotMsg, E_JMMS_INBOX_FOLDER);
		mmi_trace(1, "ucNUnreadMsg[%d],ucNTotMsg [%d]",ucNUnreadMsg, ucNTotMsg);

		HideStatusIcon(STATUS_ICON_MMS_UNREAD);
		if(ucNUnreadMsg > 0)
		{
			ShowStatusIcon(STATUS_ICON_MMS_UNREAD);
		}
	}
	UpdateStatusIcons();
}

// this function is invoked by platform when set to flight mode . this function  forcibly
// hides the new mms icon on idle screen
void jmms_HideMmsIcon()
{
	HideStatusIcon(STATUS_ICON_MMS_UNREAD);
	UpdateStatusIcons();
}

// this function is invoked by platform changed from flight mode to normal mode . this function  checks
// if any messages in inbox and updates idle screen new mms icon accordingly

void jmms_RestoreMmsICon()
{
	JC_RETCODE			rCode = JC_OK;
	//return if MMS application not initliased
	if(!g_pstJMmsAppInfo)
		return ;
	if (JC_NULL == g_pstJMmsAppInfo->hMsgStore)
	{
		rCode = jdi_StoreInit (&g_pstJMmsAppInfo->hMsgStore, 0) ;
		mmi_trace (1, "JDD_LOG: jmms_RestoreMmmsICon::jdi_StoreInit[%X]", -rCode) ;
		if(rCode == JC_OK)
		{
			jmms_UpdateMmsIcon();
			jdi_StoreDeinit(g_pstJMmsAppInfo->hMsgStore);
			g_pstJMmsAppInfo->hMsgStore = NULL;
			mmi_trace (1, "JDD_LOG: jmms_RestoreMmsICon store deinit[%X]", -rCode) ;
		}
	}
	else
	{
		jmms_UpdateMmsIcon();
	}
}

JC_RETCODE jmms_GetReadMsgStatus(JC_UINT32 *pucNUnreadMsg,JC_UINT32 *pucNTotMsg,
						E_JMMS_FOLDER_ID eFolderID)
{
	JC_RETCODE			eRet = JC_OK;
	MESSAGE_LIST		*pMessageList = JC_NULL, *pTempMsgList = JC_NULL ;
	RETRIEVE_FILTERS	 StoreRetFilter = {0, } ;

	eRet = jmms_SetActiveFolder (eFolderID) ;
	if (eRet != JC_OK) return eRet;

	StoreRetFilter.eRetrieve = E_RETRIEVE_BY_INDEX ;
	StoreRetFilter.uiStartIndex = 1 ;
	
	eRet = jdi_StoreGetMsgList(g_pstJMmsAppInfo->hMsgStore, &pMessageList, &StoreRetFilter) ;

	if(eRet == JC_ERR_STORE_FILE_CORRUPTED)
	{
		jdi_StoreRestoreFolderInfo (g_pstJMmsAppInfo->hMsgStore) ;
	}
	
	if (eRet != JC_OK) return eRet;

	*pucNUnreadMsg = 0 ;
	*pucNTotMsg = 0 ;
	pTempMsgList = pMessageList ;
	while(pTempMsgList)
	{	
		if ( !pTempMsgList->bReadFlag )
		{
		   *pucNUnreadMsg = *pucNUnreadMsg + 1 ;
		}
		*pucNTotMsg = *pucNTotMsg + 1 ;
		pTempMsgList = pTempMsgList->pNext;
	}	
	return eRet ;	
}

#ifdef __MMI_MULTI_SIM__
JC_RETCODE jmms_GetReadMsgStatusBySimID(JC_UINT32 *pucNUnreadMsg,JC_UINT32 *pucNTotMsg,
						E_JMMS_FOLDER_ID eFolderID, UINT8 nSimID)
{
	JC_RETCODE			eRet = JC_OK;
	MESSAGE_LIST		*pMessageList = JC_NULL, *pTempMsgList = JC_NULL ;
	RETRIEVE_FILTERS	 StoreRetFilter = {0, } ;

	eRet = jmms_SetActiveFolder (eFolderID) ;
	if (eRet != JC_OK) return eRet;

	StoreRetFilter.eRetrieve = E_RETRIEVE_BY_INDEX ;
	StoreRetFilter.uiStartIndex = 1 ;
	
	eRet = jdi_StoreGetMsgList(g_pstJMmsAppInfo->hMsgStore, &pMessageList, &StoreRetFilter) ;

	if(eRet == JC_ERR_STORE_FILE_CORRUPTED)
	{
		jdi_StoreRestoreFolderInfo (g_pstJMmsAppInfo->hMsgStore) ;
	}
	
	if (eRet != JC_OK) return eRet;

	*pucNUnreadMsg = 0 ;
	*pucNTotMsg = 0 ;
	pTempMsgList = pMessageList ;
	while(pTempMsgList)
	{	
		if ( !pTempMsgList->bReadFlag  && nSimID== pTempMsgList->uiExtraBytes)
		{
		   *pucNUnreadMsg = *pucNUnreadMsg + 1 ;
		}
		*pucNTotMsg = *pucNTotMsg + 1 ;
		pTempMsgList = pTempMsgList->pNext;
	}	
	return eRet ;	
}

#endif
BOOL jMMS_CheckJMMSFull()
{
	JC_UINT32 free_space;
	free_space = jMMS_GetMMSFreeSpace(TRUE);

	if (free_space > 0)
	{
		return FALSE;	
	}
	return TRUE;
}
JC_RETCODE jMMS_SaveOrUpdateMsg (E_JMMS_FOLDER_ID eFolderID, JC_UINT32 *puiMessageID,
				CONTENT_DATA *pstContentData, MESSAGE_HEADERS *pMsgHeaders, 
				JC_UINT32 uiExtraBytes)
{
	JC_UINT32			uiMessagID = *puiMessageID ;
	RETRIEVE_FILTERS	stRetFilter = {0, } ;
	MESSAGE_LIST		*pstMsgList =JC_NULL ;
	JC_RETCODE			rCode = JC_OK;
	JC_CHAR				pmcFolderName [JMMS_MAX_FOLDER_NAME_LEN] ;
	JC_UINT32			uiMMSFreeSpace = jMMS_GetMMSFreeSpace (FALSE) ;

	/*	Even if it is for update Msg, then also it is checked for pstContentData->uiTotalSize
		space availability in the file system and it is intentional as store module may need this */

	mmi_trace (1, "JDD_LOG: jMMS_SaveOrUpdateMsg >> eFolderID [%d], MSG id [%d], MMS Required Space [%d] and Available Space [%d]",
				eFolderID, *puiMessageID, pstContentData->uiTotalSize, uiMMSFreeSpace) ;
	if (uiMMSFreeSpace < pstContentData->uiTotalSize)
	{
		rCode = JC_ERR_FILE_WRITE ;
	}
	if (JC_OK == rCode)
	{
		if (JC_NULL == g_pstJMmsAppInfo->hMsgStore)
		{
			rCode = jdi_StoreInit (&g_pstJMmsAppInfo->hMsgStore, 0) ;
			mmi_trace (1, "JDD_LOG: jMMS_SaveOrUpdateMsg::jdi_StoreInit[%X]", -rCode) ;
		}
		rCode = jmms_SetActiveFolder (eFolderID) ;
		if (JC_OK == rCode)
		{
			stRetFilter.uiStartIndex = 1 ;
			stRetFilter.eRetrieve = E_RETRIEVE_BY_INDEX ;
			rCode = jmms_GetMsgList (g_pstJMmsAppInfo->hMsgStore, &pstMsgList, &stRetFilter) ;
		}
		if (JC_OK == rCode)
		{
			if (0 == uiMessagID)
			{
				jdi_CUtilsCharToTcharMem (pFolderName[eFolderID], JMMS_MAX_FOLDER_NAME_LEN, pmcFolderName) ;
				if ((pMsgHeaders->eMsgType == E_MSG_TYPE_SEND || pMsgHeaders->eMsgType == E_MSG_TYPE_RECEIVE))  
				{
					if (pstContentData->uiCurSize < jMMS_GetMMSFreeSpace (TRUE)) //when send or retrieve, need check the space
					{
						rCode = jdi_StoreSaveMsg (g_pstJMmsAppInfo->hMsgStore, pmcFolderName, 0,
							pMsgHeaders, pstContentData, &uiMessagID, uiExtraBytes) ;
					}
					else
					{
						rCode = JC_ERR_FILE_WRITE;	
					}
				}
				else
				{
						rCode = jdi_StoreSaveMsg (g_pstJMmsAppInfo->hMsgStore, pmcFolderName, 0,
							pMsgHeaders, pstContentData, &uiMessagID, uiExtraBytes) ;
				}
				mmi_trace(1,"JDD_LOG: jdi_StoreSaveMsg start [%X] MSG ID is [%d]", -rCode, uiMessagID) ;
			}
			else
			{
				rCode = jdi_StoreUpdateMsg (g_pstJMmsAppInfo->hMsgStore, uiMessagID, pMsgHeaders, pstContentData) ;
				mmi_trace(1,"JDD_LOG: jdi_StoreUpdateMsg start [%X] MSG ID is [%d]", -rCode, uiMessagID) ;
			}
		}

		if (GetActiveScreenId() == SCR_ID_MMS_INBOX_SCREEN)
		{
			//DeleteUptoScrID(SCR_ID_MMS_INBOX_SCREEN);	
			jMms_inbox_entry_handler();
			DeleteScreenIfPresent(SCR_ID_MMS_INBOX_SCREEN);
		}
		*puiMessageID = uiMessagID ;
	}
	return rCode;
}

JC_RETCODE jMMS_DeleteMsg (E_JMMS_FOLDER_ID eFolderID, JC_UINT32 uiMessagID, JC_BOOLEAN bCheckNodeIsBusy)
{
	JC_RETCODE			rCode ;
	JC_CHAR			*pmMMSFullFileName = JC_NULL ;
	JDD_FILE			vFileHandle ;
	RETRIEVE_FILTERS	stMsgFilter = {0, } ;
	MESSAGE_LIST		*pstMessageList = JC_NULL ;
	JC_INT32				uiRet;
	JC_UINT32			uiMMSFreeSpace = jMMS_GetMMSFreeSpace (FALSE) ;
	
	rCode = jmms_SetActiveFolder (eFolderID) ;
	if (JC_OK == rCode)
	{
		stMsgFilter.eRetrieve = E_RETRIEVE_BY_INDEX ;
		stMsgFilter.uiStartIndex= 1 ;
		stMsgFilter.uiNumMessage = 0 ;
		rCode = jmms_GetMsgList (g_pstJMmsAppInfo->hMsgStore, &pstMessageList, &stMsgFilter) ;
	}   
	if (JC_OK == rCode)
	{
		pmMMSFullFileName = jMMS_GetMMSFullFileName (jmms_GetFolderName (eFolderID)) ;
		if (pmMMSFullFileName)
		{
			vFileHandle = jdd_FSOpen (JC_NULL, pmMMSFullFileName, E_OPEN_READ_MODE) ;
			if (vFileHandle)
			{
				rCode = jdd_FSGetSize (vFileHandle, &uiRet) ;
				if (JC_OK == rCode)
				{
					mmi_trace (1, "JDD_LOG: Folder ID [%d], size [%d], Available Space [%d]",
						eFolderID, uiRet, uiMMSFreeSpace) ;
					if (uiRet > uiMMSFreeSpace) //?? 
					{
						jdd_MemFree (pmMMSFullFileName) ;
						pmMMSFullFileName = jMMS_GetMMSFullFileName (JMMS_RESERVED_MMS_FILE_NAME) ;
						if (JC_NULL != pmMMSFullFileName)
						{
							rCode = jdd_FSDelete (JC_NULL, pmMMSFullFileName) ;
						}
					}
				}
				rCode = jdd_FSClose (vFileHandle) ;
			}
			else
			{
				mmi_trace (1, "JDD_LOG: jMMS_DeleteMsg >> Folder MMS Files does not exist") ;
			}
			jdd_MemFree (pmMMSFullFileName) ;
		}
		if (0 == uiMessagID)
		{
		    if( !bCheckNodeIsBusy || jMMS_FindMsgInGetReqRespNode(pstMessageList) == E_FALSE)
		    {
				rCode = jdi_StoreDeleteAllMsgs (g_pstJMmsAppInfo->hMsgStore) ;
		    }
			else
			{
				rCode = JC_ERR_GENERAL_FAILURE;
			}
			mmi_trace (1, "JDD_LOG: jdi_StoreDeleteAllMsgs returned rCode [%X] for Folder [%d]", -rCode, eFolderID) ;
		}
		else
		{
			if( !bCheckNodeIsBusy || jMMS_IsMsgIdInGetReqRespNode(uiMessagID) == E_FALSE)
		    {
				rCode = jdi_StoreDeleteMsg (g_pstJMmsAppInfo->hMsgStore, uiMessagID) ;
			}
			else
			{
				rCode = JC_ERR_GENERAL_FAILURE;
			}			
			mmi_trace (1, "JDD_LOG: jdi_StoreDeleteMsg returned rCode [%X] for Folder [%d] and message [%d]", -rCode, eFolderID, uiMessagID) ;
		}
		if (JC_OK == rCode)
		{
			rCode = jdi_StoreUpdateMsgList (g_pstJMmsAppInfo->hMsgStore, E_FALSE) ;
			mmi_trace (1, "JDD_LOG: jMMS_DeleteMsg::jdi_StoreUpdateMsgList returned rCode [%X]", -rCode) ;
		}
		jMMS_CreateReservedMMSFile () ;

	}
	return rCode ;
}

JC_UINT32 jMMS_GetTotalNumberOfMsgsInFolder (E_JMMS_FOLDER_ID eFolderID)
{
	JC_RETCODE rCode = JC_OK;
	FOLDER_INFO *pFolderInfo ;
	FOLDER_LIST *pFolderList;
	JC_UINT32 uiMsgListCount = 0;
	MESSAGE_LIST		*pstMessageList = JC_NULL ;
	RETRIEVE_FILTERS	stMsgFilter = {0, } ;
	
	rCode = jmms_SetActiveFolder (eFolderID) ;
	if (JC_OK == rCode)
	{
		stMsgFilter.eRetrieve = E_RETRIEVE_BY_INDEX ;
		stMsgFilter.uiStartIndex= 1 ;
		stMsgFilter.uiNumMessage = 0 ;
		rCode = jmms_GetMsgList (g_pstJMmsAppInfo->hMsgStore, &pstMessageList, &stMsgFilter) ;
	}   
	rCode = jdi_StoreGetFolders (g_pstJMmsAppInfo->hMsgStore, &pFolderInfo) ;
	if (JC_OK == rCode && pFolderInfo)
	{
		pFolderList = pFolderInfo->pFolderList;
		while (pFolderList)
		{
			if (jdi_CUtilsTcsStrCmp (pFolderList->pFolderName, jmms_GetFolderName (eFolderID)) == 0)
			{
				uiMsgListCount = pFolderList->uiTotalMessages ;
				break ;
			}
			pFolderList = pFolderList->pNext ;
		}
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jMMS_GetTotalNumberOfMsgsInFolder >> jdi_StoreGetFolders returned [%X] and value of pFolderInfo is [%d]",
			-rCode, pFolderInfo) ;
	}
	return uiMsgListCount ;
}

JC_UINT32 jMMS_GetTotalNumberOfMsgs (void)
{
	JC_RETCODE rCode = JC_OK;
	FOLDER_INFO *pFolderInfo=NULL ;
	FOLDER_LIST *pFolderList=NULL;
	JC_UINT32 uiMsgListCount = 0;
	if (JC_NULL == g_pstJMmsAppInfo->hMsgStore)
	{
		rCode = JC_ERR_NULL_POINTER ;
	}
	else
	{
		rCode = jdi_StoreGetFolders (g_pstJMmsAppInfo->hMsgStore, &pFolderInfo) ;
	}
	if (JC_OK == rCode && pFolderInfo)
	{
		pFolderList = pFolderInfo->pFolderList;
		while (pFolderList)
		{
			uiMsgListCount += pFolderList->uiTotalMessages ;
			pFolderList = pFolderList->pNext ;
		}
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jMMS_GetTotalNumberOfMsgs >> jdi_StoreGetFolders returned [%X] and value of pFolderInfo is [%d]",
			-rCode, pFolderInfo) ;
	}
	return uiMsgListCount ;
}

void jMMS_FormDateTimeStringFromSecs (JC_UINT32 uiSeconds, JC_CHAR *pmcFormattedDateTimeString)
{
	MYTIME		stTime = {0, } ;
	JC_CHAR	pmcFormatString [64] = {0,} ;
	kal_bool	eRet ;
	
	eRet = mmi_dt_utc_sec_2_mytime (uiSeconds, &stTime) ;
	mmi_trace (1, "JDD_LOG: jMMS_FormDateTimeStringFromSecs >> UTC seconds is [%d], timezone diff is [%d], eRet [%d])",
		uiSeconds, GetTimeZone(PhnsetGetHomeCity()) * 60 * 60, eRet) ;
	
	date_string (&stTime, pmcFormatString, DT_IDLE_SCREEN) ;
//	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, (JC_CHAR*)GetString (STR_ID_MMS_MSG_DATE)) ;
	jc_tcscat (pmcFormattedDateTimeString, pmcFormatString) ;
	jc_memset (pmcFormatString, 0 , 64);

	time_string (&stTime, pmcFormatString, DT_IDLE_SCREEN) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, (JC_CHAR*)GetString (STR_ID_MMS_MSG_TIME)) ;
	jc_tcscat (g_pstJMmsAppInfo->pszScreeBuff, pmcFormatString) ;
}

void jMMS_NetErrorHandler (JC_RETCODE rCode, FuncPtr pfCBFPtr)
{
	switch (rCode)
	{
	case JC_ERR_NET_INIT:
		{
			DisplayPopup((PU8)GetString(STR_ERR_NET_INIT), IMG_GLOBAL_ERROR, 1,	2000, 1) ;			
		}
	case JC_ERR_NET_CONNECT:
		{
			DisplayPopup((PU8)GetString(STR_ERR_NET_CONNECT), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		}
		break ;
	case JC_ERR_NET_DNS_RESOLVE:
		{
			DisplayPopup((PU8)GetString(STR_ERR_NET_DNS_RESOLVE), IMG_GLOBAL_ERROR, 1,	2000, 1) ;
		}
		break ;
	case JC_ERR_NET_TIMEOUT:
		{
			DisplayPopup((PU8)GetString(STR_ERR_NET_TIMEOUT), IMG_GLOBAL_ERROR, 1,	2000, 1) ;
		}
		break ;
	case JC_ERR_NET_SEND:
		{
			DisplayPopup((PU8)GetString(STR_ERR_NET_SEND), IMG_GLOBAL_ERROR, 1,	2000, 1) ;
		}
		break ;
	case JC_ERR_NET_RECV:
		{
			DisplayPopup((PU8)GetString(STR_ERR_NET_RECV), IMG_GLOBAL_ERROR, 1,	2000, 1) ;
		}
		break ;
	case JC_ERR_NET_SOCKET:
		{
			DisplayPopup((PU8)GetString(STR_ERR_NET_SOCKET), IMG_GLOBAL_ERROR, 1,	2000, 1) ;
		}
		break ;
	case JC_ERR_NET_SOCKET_NOT_READY:
		{
			DisplayPopup((PU8)GetString(STR_ERR_NET_SOCKET_NOT_READY), IMG_GLOBAL_ERROR, 1,	2000, 1) ;
		}
		break ;
	case JC_ERR_NET_SET_SOCKOPT:
		{
			DisplayPopup((PU8)GetString(STR_ERR_NET_SET_SOCKOPT), IMG_GLOBAL_ERROR, 1,	2000, 1) ;
		}
		break ;
	case JC_ERR_NET_GET_SOCKOPT:
		{
			DisplayPopup((PU8)GetString(STR_ERR_NET_GET_SOCKOPT), IMG_GLOBAL_ERROR, 1,	2000, 1) ;
		}
		break ;
	case JC_ERR_NET_WOULDBLOCK:
		{
			DisplayPopup((PU8)GetString(STR_ERR_NET_WOULDBLOCK), IMG_GLOBAL_ERROR, 1,	2000, 1) ;
		}
		break ;
	case JC_ERR_NET_ACCEPT:
		{
			DisplayPopup((PU8)GetString(STR_ERR_NET_ACCEPT), IMG_GLOBAL_ERROR, 1,	2000, 1) ;
		}
		break ;
	case JC_ERR_NET_LISTEN:
		{
			DisplayPopup((PU8)GetString(STR_ERR_NET_LISTEN), IMG_GLOBAL_ERROR, 1,	2000, 1) ;
		}
		break ;
	case JC_ERR_NET_CONN_ABORT:
		{
			DisplayPopup((PU8)GetString(STR_ERR_NET_CONN_ABORT), IMG_GLOBAL_ERROR, 1,	2000, 1) ;
		}
		break ;
	case JC_ERR_NET_DNS_WOULDBLOCK:
		{
			DisplayPopup((PU8)GetString(STR_ERR_NET_DNS_WOULDBLOCK), IMG_GLOBAL_ERROR, 1,	2000, 1) ;
		}
		break ;
	case JC_ERR_NET_SOCKET_FULL:
		{
			DisplayPopup((PU8)GetString(STR_ERR_NET_SOCKET_FULL), IMG_GLOBAL_ERROR, 1,	2000, 1) ;
		}
		break ;
	default:
		DisplayPopup((PU8)GetString(STR_ERR_DATA_CONN_UNKNOWN), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		break ;
	}
}

void jMMS_DataconnectionErrorHandler (JC_RETCODE rCode, FuncPtr pfCBFPtr)
{
	switch (rCode)
	{
	case JC_ERR_DATA_CONN_CANCEL:
		{
			DisplayPopup((PU8)GetString(STR_ERR_DATA_CONN_CANCEL), IMG_GLOBAL_ERROR, 1,	2000, 1) ;
		}
		break ;
	case JC_ERR_DATA_CONN_CONTINUE:
		{
			DisplayPopup((PU8)GetString(STR_ERR_DATA_CONN_CONTINUER), IMG_GLOBAL_ERROR, 1,	2000, 1) ;
		}
		break ;
	case JC_ERR_DATA_CONN_LINE_BUSY:
		{
			DisplayPopup((PU8)GetString(STR_ERR_DATA_CONN_LINE_BUSY), IMG_GLOBAL_ERROR, 1,	2000, 1) ;
		}
		break ;
	case JC_ERR_DATA_CONN_INVALID_USERNAME:
		{
			DisplayPopup((PU8)GetString(STR_ERR_DATA_CONN_INVALID_USERNAME), IMG_GLOBAL_ERROR, 1,	2000, 1) ;
		}
		break ;
	case JC_ERR_DATA_CONN_INVALID_PASSWORD:
		{
			DisplayPopup((PU8)GetString(STR_ERR_DATA_CONN_INVALID_PASSWORD), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		}
		break ;
	case JC_ERR_DATA_CONN_NO_DIALTONE:
		{
			DisplayPopup((PU8)GetString(STR_ERR_DATA_CONN_NO_DIALTONE), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		}
		break ;
	case JC_ERR_DATA_CONN_INVALID_APN:
		{
			DisplayPopup((PU8)GetString(STR_ERR_DATA_CONN_INVALID_APN), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		}
		break ;
	case JC_ERR_DATA_CONN_TIMEOUT:
		{
			DisplayPopup((PU8)GetString(STR_ERR_DATA_CONN_TIMEOUT), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		}
		break ;
	case JC_ERR_DATA_CONN_UNKNOWN:
		{
			DisplayPopup((PU8)GetString(STR_ERR_DATA_CONN_UNKNOWN), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		}
		break ;
	default:
		DisplayPopup((PU8)GetString(STR_ERR_DATA_CONN_UNKNOWN), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		break ;
	}
}
// for debug
typedef struct {
	JC_RETCODE err_id;
	char *err_str;
} ERR_STRUCT;

const static ERR_STRUCT err_net_module_array[] =
{
	{JC_ERR_NET_INIT, "JC_ERR_NET_INIT"},
	{JC_ERR_NET_CONNECT, "JC_ERR_NET_CONNECT"},
	{JC_ERR_NET_DNS_RESOLVE, "JC_ERR_NET_DNS_RESOLVE"},
	{JC_ERR_NET_TIMEOUT, "JC_ERR_NET_TIMEOUT"},
	{JC_ERR_NET_SEND, "JC_ERR_NET_SEND"},
	{JC_ERR_NET_RECV, "JC_ERR_NET_RECV"},
	{JC_ERR_NET_SOCKET, "JC_ERR_NET_SOCKET"},
	{JC_ERR_NET_SOCKET_NOT_READY, "JC_ERR_NET_SOCKET_NOT_READY"},
	{JC_ERR_NET_SET_SOCKOPT, "JC_ERR_NET_SET_SOCKOPT"},
	{JC_ERR_NET_GET_SOCKOPT, "JC_ERR_NET_GET_SOCKOPT"},
	{JC_ERR_NET_WOULDBLOCK, "JC_ERR_NET_WOULDBLOCK"},
	{JC_ERR_NET_ACCEPT, "JC_ERR_NET_ACCEPT"},
	{JC_ERR_NET_LISTEN, "JC_ERR_NET_LISTEN"},
	{JC_ERR_NET_CONN_ABORT, "JC_ERR_NET_CONN_ABORT"},
	{JC_ERR_NET_DNS_WOULDBLOCK, "JC_ERR_NET_DNS_WOULDBLOCK"},
	{JC_ERR_NET_SOCKET_FULL, "JC_ERR_NET_SOCKET_FULL"}
};
const static ERR_STRUCT err_data_conn_module_array[] =
{
	{JC_ERR_DATA_CONN_CANCEL, "JC_ERR_DATA_CONN_CANCEL"},
	{JC_ERR_DATA_CONN_CONTINUE, "JC_ERR_DATA_CONN_CONTINUE"},
	{JC_ERR_DATA_CONN_LINE_BUSY, "JC_ERR_DATA_CONN_LINE_BUSY"},
	{JC_ERR_DATA_CONN_INVALID_USERNAME, "JC_ERR_DATA_CONN_INVALID_USERNAME"},
	{JC_ERR_DATA_CONN_INVALID_PASSWORD, "JC_ERR_DATA_CONN_INVALID_PASSWORD"},
	{JC_ERR_DATA_CONN_NO_DIALTONE, "JC_ERR_DATA_CONN_NO_DIALTONE"},
	{JC_ERR_DATA_CONN_INVALID_APN, "JC_ERR_DATA_CONN_INVALID_APN"},
	{JC_ERR_DATA_CONN_TIMEOUT, "JC_ERR_DATA_CONN_TIMEOUT"},
	{JC_ERR_DATA_CONN_UNKNOWN, "JC_ERR_DATA_CONN_UNKNOWN"},

};

const static ERR_STRUCT err_file_module_array[] =
{
	{JC_ERR_FILE_CREATEDIR, "JC_ERR_FILE_CREATEDIR"},
	{JC_ERR_FILE_DIR_ALREADY_EXIST, "JC_ERR_FILE_DIR_ALREADY_EXIST"},
	{JC_ERR_FILE_CHANGEDIR, "JC_ERR_FILE_CHANGEDIR"},
	{JC_ERR_FILE_RENAME, "JC_ERR_FILE_RENAME"},
	{JC_ERR_FILE_WRITE, "JC_ERR_FILE_WRITE"},
	{JC_ERR_FILE_FLUSH, "JC_ERR_FILE_FLUSH"},
	{JC_ERR_FILE_OPEN, "JC_ERR_FILE_OPEN"},
	{JC_ERR_FILE_CLOSE, "JC_ERR_FILE_CLOSE"},
	{JC_ERR_FILE_SIZE, "JC_ERR_FILE_SIZE"},
	{JC_ERR_FILE_DELETE, "JC_ERR_FILE_DELETE"},
	{JC_ERR_FILE_SEEK, "JC_ERR_FILE_SEEK"},
	{JC_ERR_FILE_READ, "JC_ERR_FILE_READ"},
	{JC_ERR_FILE_SYS_INIT, "JC_ERR_FILE_SYS_INIT"},
	{JC_ERR_FILE_SYS_DEINIT, "JC_ERR_FILE_SYS_DEINIT"},
	{JC_ERR_DIR_DELETE, "JC_ERR_DIR_DELETE"},
	{JC_ERR_DIR_RENAME, "JC_ERR_DIR_RENAME"},
	{JC_ERR_FILE_NOT_FOUND, "JC_ERR_FILE_NOT_FOUND"},
	{JC_ERR_FILE_EOF, "JC_ERR_FILE_EOF"},

};
const static ERR_STRUCT err_other_module_array[] =
{
	{JC_ERR_MEM_INIT, "JC_ERR_MEM_INIT"},
	{JC_ERR_MMI_INIT, "JC_ERR_MMI_INIT"},
	{JC_ERR_CFG_INSUFFICIENT_MEMORY, "JC_ERR_CFG_INSUFFICIENT_MEMORY"},
	{JC_ERR_CFG_NOT_FOUND, "JC_ERR_CFG_NOT_FOUND"},
	{JC_ERR_NOT_IMPLEMENTED, "JC_ERR_NOT_IMPLEMENTED"},
	{JC_ERR_INVALID_PARAMETER, "JC_ERR_INVALID_PARAMETER"},
	{JC_ERR_NULL_POINTER, "JC_ERR_NULL_POINTER"},
	{JC_ERR_MEMORY_ALLOCATION, "JC_ERR_MEMORY_ALLOCATION"},
	{JC_ERR_INVALID_STATE, "JC_ERR_INVALID_STATE"},
	{JC_ERR_NOT_SUPPORTED, "JC_ERR_NOT_SUPPORTED"},
	{JC_ERR_GENERAL_FAILURE, "JC_ERR_GENERAL_FAILURE"},
	{JC_ERR_UNKNOWN, "JC_ERR_UNKNOWN"},
	{JC_ERR_IMG_FORMAT, "JC_ERR_IMG_FORMAT"},
	{JC_ERR_USER_CANCELLED, "JC_ERR_USER_CANCELLED"},
	{JC_ERR_MAILTO_UNSUPPORTED, "JC_ERR_MAILTO_UNSUPPORTED"},
	{JC_ERR_HTTP_MULTIPART, "JC_ERR_HTTP_MULTIPART"},
	{JC_ERR_WAIT_USER_ACTION, "JC_ERR_WAIT_USER_ACTION"},
	{JC_ERR_ASYNC_CALL, "JC_ERR_ASYNC_CALL"},
	{JC_ERR_MMI_FONT_NOT_AVAILABLE, "JC_ERR_MMI_FONT_NOT_AVAILABLE"},
	{JC_ERR_MMI_NO_CACHING, "JC_ERR_MMI_NO_CACHING"},

};
void jdd_TraceErrCode(JC_RETCODE rCode, JC_INT32	uiModule)
{
	JC_INT32 length = 0;
	const ERR_STRUCT * err_array = JC_NULL;
	int i = 0;
	char *pModuleName = NULL;
	BOOL bFindIt = FALSE;
	switch (uiModule)
	{
		case NET_MODULE:
		{
			err_array = err_net_module_array;
			length = sizeof(err_net_module_array) / sizeof(ERR_STRUCT);
			pModuleName = "NET_MODULE";
		}
		break;
		
		case DATA_CONN_MODULE:
		{
			err_array = err_data_conn_module_array;
			length = sizeof(err_data_conn_module_array) / sizeof(ERR_STRUCT);
			pModuleName = "DATA_CONN_MODULE";
		}
		break;
		case FILE_MODULE:
		{
			err_array = err_file_module_array;
			length = sizeof(err_file_module_array) / sizeof(ERR_STRUCT);
			pModuleName = "FILE_MODULE";
		}
		break;
		case OTHER_MODULE:
		{
			err_array = err_other_module_array;
			length = sizeof(err_other_module_array) / sizeof(ERR_STRUCT);
			pModuleName = "OTHER_MODULE";
		}
		break;
			

	}
	if (JC_NULL != err_array)
	{
		for (i=0; i< length; i++)
		{
			if (rCode == err_array[i].err_id)
			{
				bFindIt = TRUE;
				mmi_trace(TRUE,"JDD_LOG: ERROR Func: %s  MODULE:%s CODE:%s", __FUNCTION__, pModuleName, err_array[i].err_str);
			}
		}

		if (!bFindIt)
		{
				mmi_trace(TRUE,"JDD_LOG: ERROR Func: %s  Cant't find CODE:%s in MODULE:%s ", __FUNCTION__, rCode, pModuleName);
		}
	}
	else 
	{
		mmi_trace(TRUE,"JDD_LOG: ERROR NO THIS MODULE in jdd_TraceErrCode");
	}
}

void jMMS_ErrorHandler (JC_RETCODE rCode, FuncPtr pfCBFPtr)
{
	JC_UINT32 uiModuleNum = (-rCode) & 0x00FFFF00 ;
	mmi_trace(g_sw_MMS,"JDD_LOG: Func: %s ERROR rCode:0x%x pfCBFPtr:0x%p", __FUNCTION__, -rCode, pfCBFPtr);
	jdd_TraceErrCode(rCode, uiModuleNum);
	switch (uiModuleNum)
	{
		case NET_MODULE:
			jMMS_NetErrorHandler (rCode, pfCBFPtr) ;
			break ;
		case DATA_CONN_MODULE:
			jMMS_DataconnectionErrorHandler (rCode, pfCBFPtr) ;
			break ;
		case FILE_MODULE:
			if(JC_ERR_FILE_OPEN == rCode)
			{
				DisplayPopup((PU8)GetString(STR_ID_MMS_FILE_OPEN_ERROR), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
			}
			else if (JC_ERR_FILE_NOT_FOUND != rCode)
			{
				DisplayPopup((PU8)GetString(STR_ID_MMS_FILE_SYSTEM_FULL), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
			}
			else
			{
				DisplayPopup((PU8)GetString(STR_ID_MMS_INF_FAIL), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
			}
			break ;
		case MSG_STORE_MODULE:
			mmi_trace(g_sw_MMS,"JDD_LOG: Func: %s ERROR MSG_STORE_MODULE STR_ID_MMS_INF_FAIL", __FUNCTION__);
			DisplayPopup((PU8)GetString(STR_ID_MMS_INF_FAIL), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
			break ;
		default:
			mmi_trace(g_sw_MMS,"JDD_LOG: Func: %s ERROR Default STR_ID_MMS_INF_FAIL", __FUNCTION__);
			DisplayPopup((PU8)GetString(STR_ID_MMS_INF_FAIL), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
			break ;
	}
}

JC_CHAR* jMMS_GetMMSFullFileName (const JC_INT8 *pcFileName)
{
	JC_CHAR					*pmMMSFullFileName = JC_NULL ;
	JC_UINT32					uiFileNameLen ;
	JC_UINT32					uiRootPathLen ;
	STORE_CONFIG_SETTINGS	stStoreSettings = {0, } ;

	stStoreSettings.uiMsgType = 0 ;
	jdd_MsgConfigGetDefaultStoreSettings (&stStoreSettings) ;
	if (stStoreSettings.pRootPath)
	{
		uiRootPathLen = jc_tcslen (stStoreSettings.pRootPath) ;
		uiFileNameLen =  uiRootPathLen + jc_strlen (pcFileName) ;
		pmMMSFullFileName = jdd_MemAlloc (uiFileNameLen + 1, sizeof (JC_CHAR)) ;
		if (pmMMSFullFileName)
		{
			jc_tcscpy (pmMMSFullFileName, stStoreSettings.pRootPath) ;
			jdi_CUtilsCharToTcharMem (pcFileName,
				uiFileNameLen - uiRootPathLen + 1, pmMMSFullFileName + uiRootPathLen) ;
		}
		else
		{
			mmi_trace (1, "JDD_LOG: jMMS_GetReservedMMSFileNameAndExistence >> Memory failed") ;
		}
		jdd_MemFree (stStoreSettings.pRootPath) ;
		jdd_MemFree (stStoreSettings.pRootFile) ;
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jMMS_GetReservedMMSFileNameAndExistence >> jdd_MsgConfigGetDefaultStoreSettings () failed") ;
	}
	return pmMMSFullFileName ;
}

JC_RETCODE jMMS_CreateReservedMMSFile (void)
{
	JC_CHAR					*pmMMSReservedFileName ;
	JC_INT8					pcReservedArr [JMMS_RESERVED_MMS_FILE_CHUNK_SIZE] = {0, } ;
	JC_RETCODE					rCode = JC_OK ;
	JDD_FILE					vFileHandle ;
	JC_UINT32					uiNumChunks = JMMS_RESERVED_MMS_FILE_CHUNK_NUM ;

	pmMMSReservedFileName = jMMS_GetMMSFullFileName (JMMS_RESERVED_MMS_FILE_NAME) ;
	if (pmMMSReservedFileName)			
	{
		if (E_FALSE == jdd_FSIsFileExist (JC_NULL, pmMMSReservedFileName))
		{
			vFileHandle = jdd_FSOpen (JC_NULL, pmMMSReservedFileName, E_CREATE_APPEND_WRITE_MODE) ;
			if (vFileHandle)
			{
				while (uiNumChunks > 0)
				{
					if (-1 == jdd_FSWrite (pcReservedArr, JMMS_RESERVED_MMS_FILE_CHUNK_SIZE, 1, vFileHandle))
					{
						uiNumChunks = 0 ;
						rCode = JC_ERR_FILE_WRITE ;
					}
					else
					{
						uiNumChunks-- ;
					}
				}
				jdd_FSClose (vFileHandle) ;
			}
			else
			{
				rCode = JC_ERR_FILE_OPEN ;
			}
		}
		jdd_MemFree (pmMMSReservedFileName) ;
	}
	else
	{
		rCode = JC_ERR_MEMORY_ALLOCATION ;
	}
	if (JC_OK != rCode)
	{
		mmi_trace (1, "JDD_LOG: jMMS_CreateReservedMMSFile () failed with rCode as [%X]", -rCode) ;
	}
	return rCode ;
}

/*****************************************************************************
*
* @fn 			: mmsUtilIsFileNameAscii
*
* @brief		: This function checks whether file name is ascii
* @param[in]	: pFileName	File name
*
* @param[out]	: No Parameter.
*
* @retval		: E_TRUE/E_FALSE, E_TRUE when file name is ascii
*
******************************************************************************/

JC_BOOLEAN jmms_IsFileNameAscii (JC_CHAR *pFileName)
{
	JC_BOOLEAN	bIsASCII = E_TRUE ;

	while (*pFileName)
	{
		if(*pFileName > (JC_UINT16)0x7f)
		{
			bIsASCII = E_FALSE ;
			break ;
		}
		pFileName++ ;
	}
	return bIsASCII ;
}
extern void playShortMessageTone(U8 audioId) ;
extern U16 GetTotalCallCount(void);

void jmms_exit_inbox_list_dummy (void)
{
	history_t currHistory;
	
	memset (& currHistory, 0, sizeof (currHistory));
	currHistory.scrnID = SCR_ID_MMS_MAIN_SCREEN;

	currHistory.entryFuncPtr = jMms_mainMenu_entry_handler;
	memset ((S8*)currHistory.inputBuffer, 0, ENCODING_LENGTH);
	
	AddHistory (currHistory);
	
	SetDelScrnIDCallbackHandler(SCR_ID_MMS_MAIN_SCREEN,jmms_ApplicationExitHandler);
	
	memset (& currHistory, 0, sizeof (currHistory));
	currHistory.scrnID = SCR_ID_MMS_INBOX_SCREEN;

	currHistory.entryFuncPtr = jMms_inbox_entry_handler;
	memset ((S8*)currHistory.inputBuffer, 0, ENCODING_LENGTH);
	
	AddHistory (currHistory);
}


void jmms_get_msg_new (void)
{

	JC_RETCODE rCode ;
	
	gNeedNewMMSPopOnIdle = E_FALSE;

	mmi_trace(g_sw_MMS,"JDD_LOG: jmms_get_msg_new, g_pstJMmsAppInfo->uiHighlightedMsgId =%d", g_pstJMmsAppInfo->uiHighlightedMsgId);
	if (JC_NULL == g_pstJMmsAppInfo->hMsgStore)
	{
		rCode = jdi_StoreInit (&g_pstJMmsAppInfo->hMsgStore, 0) ;
	}
	jMms_createMmsViewWindow();

	jMMS_SetMMSAppStatus(E_TRUE);	
	jmms_ResetGlobalVal();

	g_pstJMmsAppInfo->eCurrentFolderID = E_JMMS_INBOX_FOLDER ;
	g_pstJMmsAppInfo->uiHighlightedMsgId = uiMsgIDofNewMsgPop;
	
	jmms_exit_inbox_list_dummy();

	jmms_ViewMsgFromFolder();	
}

U8* jmms_get_new_msg_ind_string (void)
{

	JC_RETCODE rCode ;
	static S8 buffer [180*ENCODING_LENGTH];
	JC_CHAR *phName = NULL;
	MESSAGE_RETRIEVAL	*pRetrieveHeader  = NULL;
	MESSAGE_LIST		*pstMsgDetail = NULL ;	
	
	memset(buffer, 0, sizeof(buffer));
	
	if (JC_NULL == g_pstJMmsAppInfo->hMsgStore)
	{
		rCode = jdi_StoreInit (&g_pstJMmsAppInfo->hMsgStore, 0) ;
	}
	jMms_createMmsViewWindow();

	jMMS_SetMMSAppStatus(E_TRUE);	
	jmms_ResetGlobalVal();

	g_pstJMmsAppInfo->eCurrentFolderID = E_JMMS_INBOX_FOLDER ;
	g_pstJMmsAppInfo->uiHighlightedMsgId = uiMsgIDofNewMsgPop;

		
	jmms_GetMsgDetailsByID (g_pstJMmsAppInfo->hMsgStore, g_pstJMmsAppInfo->eCurrentFolderID,
							g_pstJMmsAppInfo->uiHighlightedMsgId, &pstMsgDetail) ;
	
	 if (pstMsgDetail!=NULL && pstMsgDetail->pMessageHeaders->pHeaders)
	{
		if(E_MSG_TYPE_DELIVERY_REPORT  == pstMsgDetail->pMessageHeaders->eMsgType)
		{
			jc_tcscpy(buffer, (JC_CHAR*)GetString (STR_ID_MMS_DELIVERY_REPORT));
		}
		else if(E_MSG_TYPE_READ_REPORT_RESPONSE  == pstMsgDetail->pMessageHeaders->eMsgType)
		{
			jc_tcscpy(buffer, (JC_CHAR*)GetString (STR_ID_MMS_READ_REPORT));
		}
		else if(E_MSG_TYPE_NOTIFICATION == pstMsgDetail->pMessageHeaders->eMsgType)
		{
			
		}
		else
		{
			pRetrieveHeader = (MESSAGE_RETRIEVAL *)pstMsgDetail->pMessageHeaders->pHeaders;
			if(pRetrieveHeader->pFrom)
			{
				jc_tcscpy(buffer, (JC_CHAR*)GetString (STR_ID_MMS_MSG_FROM)) ;
				phName = (JC_CHAR *)lookUpNumber((PS8)pRetrieveHeader->pFrom);
				if (phName != NULL)
				{			
					jc_tcscat((JC_CHAR*)buffer, phName) ;
				}
				else
				{
					jc_tcscat((JC_CHAR*)buffer, pRetrieveHeader->pFrom) ;
				}
			}
			/*
			if(pRetrieveHeader->pSubject)
			{
				jc_wstrcat ((JC_CHAR*)buffer, (JC_CHAR*)GetString (STR_ID_JMMS_SUBJECT)) ;
				jc_tcscat((JC_CHAR*)buffer, pRetrieveHeader->pSubject) ;
			}
			*/
		}
		
	}
	jmms_ApplicationExitHandler(NULL);
	return (U8*)buffer;
}
void jmms_end_key_from_new_msg_ind(void)
{
	gNeedNewMMSPopOnIdle = E_FALSE;
	if(NULL != endKeyDownHanler)
	{
		(*endKeyDownHanler)();
	}
	
}
void jmms_back_from_new_msg_ind(void)
{
	gNeedNewMMSPopOnIdle = E_FALSE;
	GoBackHistory();
}
void jmms_entry_new_msg_ind (void)
{
	EntryNewScreen(SCR_ID_MMS_NEW_MSG_IND, NULL, NULL, NULL);
	mmi_trace(1,"JDD_LOG: jmms_entry_new_msg_ind");
	if(!IsKeyPadLockState ())
	{
		ShowCategory154Screen(0, 0,STR_ID_MMS_VIEW_MSG, 0, STR_GLOBAL_BACK, 0,
										(PU8)GetString (STR_ID_MMS_MESSAGE_RECIEVED), (PU8)jmms_get_new_msg_ind_string(), 
										IMG_GLOBAL_ACTIVATED, NULL);
		SetRightSoftkeyFunction(jmms_back_from_new_msg_ind, KEY_EVENT_UP);
		SetLeftSoftkeyFunction(jmms_get_msg_new, KEY_EVENT_UP);
		endKeyDownHanler = GetKeyHandler (KEY_END, KEY_EVENT_DOWN);
		SetKeyHandler(jmms_end_key_from_new_msg_ind, KEY_END, KEY_EVENT_DOWN);
	}
	else // keypad is locked
	{
           #ifdef __MMI_KEYPAD_LOCK_PATTERN_1__
			ShowCategory154Screen(0, 0, 0, 0, KEYPAD_LOCKED_RSK_TEXT, 0,
										(PU8)GetString (STR_ID_MMS_MESSAGE_RECIEVED), (PU8)jmms_get_new_msg_ind_string(),
										IMG_GLOBAL_ACTIVATED, NULL);
	   #else // __MMI_KEYPAD_LOCK_PATTERN_2__
			ShowCategory154Screen(0, 0, KEYPAD_LOCKED_RSK_TEXT, 0, 0, 0,
										(PU8)GetString (STR_ID_MMS_MESSAGE_RECIEVED), (PU8)jmms_get_new_msg_ind_string(), 
										IMG_GLOBAL_ACTIVATED, NULL);
	   #endif
	   	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	}
}

BOOL jmms_NeedNewMMSPopupOnIdle(void)
{
	return gNeedNewMMSPopOnIdle;
}

extern kal_bool vid_is_audio_channel_available(void);
extern BOOL mmi_vdorec_is_in_app(void);
extern BOOL mmi_vdoply_is_in_app(void);
void jmms_ShowNewMMSPopup(JC_UINT32 uiMsgID)
{	

	MMI_ALERT_TYPE alertType;
	MESSAGE_TONE_ENUM messagetonetype;//liuxl add the variavle to save message tone setting
	U16 pp_flag = 0;
	U8 audioPlayId=0;
	mmi_trace(1,"JDD_LOG, jmms_ShowNewMMSPopup");
       pp_flag = GetInterruptPopupDisplay();
       if( (pp_flag & POPUP_DSPL_ON) == 0 )
       {
	       PlaySmsVibratorOnce();
      		return;
       }


#if defined( __MED_VID_MOD__) || defined (__MED_MJPG_MOD__)
	   if(!vid_is_audio_channel_available()||mmi_vdorec_is_in_app()||mmi_vdoply_is_in_app()
	   	)
   	{
		PlaySmsVibratorOnce();
   		return;
   	}
#endif
    	mmi_trace(1,"JDD_LOG, jmms_ShowNewMMSPopup,entry play tone");
	alertType = GetMtCallAlertTypeEnum();
	messagetonetype = GetMessageToneType();


	TurnOnBacklight(1);

  	#ifdef __NATIVE_JAVA__

	if(checkVMOn())
	{
		PlaySmsVibratorOnce();
		DisplayPopup(GetString(STR_ID_MMS_MESSAGE_RECIEVED), IMG_GLOBAL_ACTIVATED, \
					1, 2000, 13);

   		return;
       }
	
     	#endif

	gNeedNewMMSPopOnIdle = E_TRUE;
	uiMsgIDofNewMsgPop = uiMsgID;
   	if (IsMMIInIdleState()||GetActiveScreenId()==SCR_ID_MMS_NEW_MSG_IND) 
	{
		if(IsKeyPadLockState ())
		{
			EntryIdleScreen (); 
		}
		else
		{
			jmms_entry_new_msg_ind();
		}
	}
	else
	{
		DisplayPopup(GetString(STR_ID_MMS_MESSAGE_RECIEVED), IMG_GLOBAL_ACTIVATED, \
					1, 2000, 13);
	}

  
	if (messagetonetype !=MESSAGE_SILENT) //liuxl modify for fix bug-13164
	{	
		if (alertType != MMI_VIBRATION_ONLY) //if alertType is MMI_VIBRATION_ONLY,then can't play tone,so no need to get audioPlayId
		{
			if (!GetTotalCallCount())
			{
				#ifndef __MMI_MULTI_SIM__
				audioPlayId=(U8)MESSAGE_TONE;
				#else
				audioPlayId = (U8) MESSAGE_TONE;
				#endif
			}
			else
				audioPlayId=(U8)SMS_IN_CALL_TONE;
		}
	}
	if ((alertType == MMI_VIBRATION_ONLY) || (alertType == MMI_VIBRATION_AND_RING)
					|| (alertType == MMI_VIBRATION_THEN_RING))
	{
		if(!AlmIsTonePlaying())
			PlaySmsVibratorOnce();
	}	

	if(audioPlayId)
	{
		#ifdef __MMI_SUSPEND_AUDIO_PLAY_WHEN_POPUP_NEW_MESSAGE_IND__
		if((audioPlayId==(U8)MESSAGE_TONE)&&!(mdi_audio_is_background_play_suspended()))
		{
			mdi_audio_suspend_background_play();
			backgroundaudioresume=TRUE;
		}
		#endif
		playShortMessageTone(audioPlayId);
	}

}

void jmms_DownloadCompletePopup()
{
	DisplayPopup(GetString (STR_ID_JMMS_MMS_DOWNLAOD_SUCCESS), IMG_GLOBAL_ACTIVATED,  \
				1, 2000, 1) ;
	jmms_CallbackGoBackHistory();
}

JC_INT32 JMMS_ReservedWAPSpace()
{
	JC_INT32		iWapUsedFixedSpace, iWapReservedSpace = 0;
	JC_INT32		iWapMinRequiredFixedSpace = CS_WAP_CACHE_SIZE_MAX ;
	JC_INT32		iWapMaxRequiredFixedSpace = CS_WAP_CACHE_SIZE_MAX +
		(CS_WAP_COOKIE_DEFAULT_MAX * CS_WAP_COOKIE_MAX_SIZE) +
		(CS_WAP_HISTORY_MAX_HOST * 1024) + (BOOKMARK_COUNT * 1024);
	JC_UINT8 acTmpSuffix[10];
	JC_UINT8		acTmpbuffer [64] ;

	AnsiiToUnicodeString(acTmpSuffix,"/*");


	jc_memset (acTmpbuffer, 0, 64) ;
	AnsiiToUnicodeString (acTmpbuffer, CS_WAP_PERSISTENT_PATH) ;
	pfnUnicodeStrcat (acTmpbuffer, acTmpSuffix) ;
	iWapUsedFixedSpace = MMI_FS_GetDirSize (acTmpbuffer) ;

	if (iWapUsedFixedSpace < iWapMinRequiredFixedSpace)
	{
		/* WAP has not been launched. So need to ensure, that there is enough space for
			WAP to launch */
		iWapReservedSpace = iWapMinRequiredFixedSpace ;
	}
	else
	{
		/* WAP has been launched once. So no need to reserve space for it */
		iWapReservedSpace = 0;
	}
	mmi_trace (1, "JDD_LOG: JMMS_ReservedWAPSpace >>WAP Fixed Used [%d], Min [%d], Max [%d], iWapReservedSpace[%d] ",
					iWapUsedFixedSpace, iWapMinRequiredFixedSpace, iWapMaxRequiredFixedSpace, iWapReservedSpace) ;

	return iWapReservedSpace;


}

JC_UINT32 jMMS_GetMMSTotalSpace (void)
{
	FS_INFO 		stFSInfo = {0, } ;
	JC_UINT32	uiRet = FS_GetFSInfo (CS_FS_MMS_IDENTIFIER, &stFSInfo) ;
	JC_INT32		mms_totalSpace = 0 ;
	JC_INT32		wap_usedSpace = 0 ;
	JC_UINT8		acTmpbuffer [64] = {0, } ;
	JC_INT32 iWapReservedSpace = 0;
	JC_UINT8 acTmpSuffix[10];

	AnsiiToUnicodeString(acTmpSuffix,"/*");

	iWapReservedSpace = JMMS_ReservedWAPSpace();

	AnsiiToUnicodeString (acTmpbuffer, CFG_DE_CONFIG_PATH) ;
	pfnUnicodeStrcat (acTmpbuffer, acTmpSuffix) ;
	wap_usedSpace = MMI_FS_GetDirSize (acTmpbuffer) ;
	mmi_trace (1, "JDD_LOG: jMMS_GetMMSTotalSpace () >> [%s] folder size is [%d]", CFG_DE_CONFIG_PATH, wap_usedSpace) ;

	jc_memset (acTmpbuffer, 0, 64) ;
	AnsiiToUnicodeString (acTmpbuffer, CS_WAP_PERSISTENT_PATH) ;
	pfnUnicodeStrcat (acTmpbuffer, acTmpSuffix) ;
	wap_usedSpace += MMI_FS_GetDirSize (acTmpbuffer) ;
	mmi_trace (1, "JDD_LOG: jMMS_GetMMSTotalSpace () >>[%s] Total used wap space is [%d]",CS_WAP_PERSISTENT_PATH, wap_usedSpace) ;
	if (uiRet == ERR_SUCCESS)
	{
		mms_totalSpace = (UINT32)stFSInfo.iTotalSize - wap_usedSpace - iWapReservedSpace - CS_FS_THRESHOLD_LIMIT;
	}
	else
	{
		/* Just to return 2 * CS_FS_THRESHOLD_LIMIT */
		mms_totalSpace= 4 * CS_FS_THRESHOLD_LIMIT ;
	}
	mmi_trace (1, "JDD_LOG: jMMS_GetMMSTotalSpace >> ret [%d], Total [%d] and Used [%d], mms_totalSpace[%d]",
					uiRet, (UINT32)stFSInfo.iTotalSize, (UINT32)stFSInfo.iUsedSize, mms_totalSpace );
	return mms_totalSpace ;
}
JC_UINT32 jMMS_GetMMSFreeSpace (JC_BOOLEAN bIsTOConsiderThreshold)
{
	JC_INT32		mms_freeSpace = 0 , iWapReservedSpace = 0;
#if 0
	JC_UINT8		acTmpbuffer [64] ;
	AnsiiToUnicodeString (acTmpbuffer, JMMS_DEFAULT_STORAGE_PATH) ;
	pfnUnicodeStrcat (acTmpbuffer, "*") ;
	mms_freeSpace = CS_MMS_MAX_SPACE - CS_FS_THRESHOLD_LIMIT - MMI_FS_GetDirSize (acTmpbuffer) ;
	if (0 > mms_freeSpace)
	{
		mms_freeSpace = 0 ;
	}
#else
	FS_INFO 		stFSInfo = {0, } ;
	JC_UINT32	uiRet = FS_GetFSInfo (CS_FS_MMS_IDENTIFIER, &stFSInfo) ;
	iWapReservedSpace = JMMS_ReservedWAPSpace();
	if (uiRet == ERR_SUCCESS)
	{		
		if (bIsTOConsiderThreshold)
		{
			mms_freeSpace = stFSInfo.iTotalSize - stFSInfo.iUsedSize - iWapReservedSpace - CS_FS_THRESHOLD_LIMIT ;
		}
		else
			mms_freeSpace = stFSInfo.iTotalSize - iWapReservedSpace - stFSInfo.iUsedSize ;

		if (mms_freeSpace < 0)
			mms_freeSpace = 0;

	}
	else
	{
		/* Just to return CS_FS_THRESHOLD_LIMIT */
		mms_freeSpace = 2 * CS_FS_THRESHOLD_LIMIT ;
	}
	mmi_trace (1, "JDD_LOG: jMMS_GetMMSFreeSpace >> ret [%d], Total [%d], Used [%d] returned free space is [%d]",
					uiRet, (UINT32)stFSInfo.iTotalSize, (UINT32)stFSInfo.iUsedSize, mms_freeSpace) ;

#endif
	return mms_freeSpace ;
}
void jMMS_NewMMSAutoRetiveFailPopup(JC_RETCODE rCode)
{
	JC_UINT32 uiModuleNum = (-rCode) & 0x00FFFF00 ;
	mmi_trace(g_sw_MMS,"JDD_LOG: Func: %s ERROR rCode:0x%x", __FUNCTION__, -rCode);
	jdd_TraceErrCode(rCode, uiModuleNum);
	switch (uiModuleNum)
	{
		case FILE_MODULE:
			if (JC_ERR_FILE_NOT_FOUND != rCode)
			{
				DisplayPopup((PU8)GetString(STR_ID_MMS_AUTO_RETRIVE_FAIL_MEMORY_FULL), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
			}
			else
			{
				DisplayPopup((PU8)GetString(STR_ID_MMS_AUTO_RETRIVE_FAIL), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
			}
			break ;
		default:
			DisplayPopup((PU8)GetString(STR_ID_MMS_AUTO_RETRIVE_FAIL), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
			break ;
	}
}


char* jmms_UtilsUnicodeToUtf8( const JC_CHAR* wstr , int *outLen)
{
    const JC_CHAR* w;
	unsigned char* szOut = JC_NULL;
	int i = 0;
    // Convert unicode to utf8
    int len = 0;
    for ( w = wstr; *w; w++ ) 
	{
        if (*w < (JC_CHAR)0x0080)
        {
			len++;
        }
        else if ( *w < (JC_CHAR)0x0800 )
			len += 2;
        else
			len += 3;
    }

    szOut = ( unsigned char* )jdd_MemAlloc( len+1, 1 );
	*outLen = len;

    if ( szOut == JC_NULL )
        return JC_NULL;

    
    for ( w = wstr; *w; w++ ) 
		{
	        if ( *w < 0x0080 )
	            szOut[i++] = ( unsigned char ) *w;
	        else if ( *w < 0x0800 ) {
	            szOut[i++] = 0xc0 | (( *w ) >> 6 );
	            szOut[i++] = 0x80 | (( *w ) & 0x3f );
	        }
	        else {
	            szOut[i++] = 0xe0 | (( *w ) >> 12 );
	            szOut[i++] = 0x80 | (( ( *w ) >> 6 ) & 0x3f );
	            szOut[i++] = 0x80 | (( *w ) & 0x3f );
	        }    
		}

    szOut[i] = '\0';
    return ( char* )szOut;
}
#endif //__JATAAYU_APP__

