/***************************************************************************************/
/*              Copyright (C) 2005, CII Technologies, Inc.                             */
/*                            All Rights Reserved                                      */
/***************************************************************************************/
/* Filename:    cfw_sms_mmi.c                                                          */
/*                                                                                     */
/* Description:                                                                        */
/*         All SMS MO API defined in CFW doc                                           */
/*                                                                                     */
/* Revision :                                                                          */
/*   1.0.0 : 2006-04-15, Wang Dahai, dahai_wang@ciitec.com                             */
/*                       Nie YongFeng,nieyf@ciitec.com                                 */
/*                       Li XingPeng,lixp@ciitec.com                                   */
/*                                                                                     */
/* Remark: Search "???" for more current problems reserved                             */
/*                                                                                     */
/* //modify record                                                                     */
/* XueWW[+] 2007/01/09                                                                 */
/* CFW_SmsWriteMessage add protection of null msg                                      */
/*                                                                                   */
/*xueww[-]2007.01.22 fix bug 3794                                                      */
/*support send NULL SMS, delete the verify parameter of datasize in CFW_SmsSendMessage */
/*                                                                                 */
/*xueww[mod] 2007.01.30                          */
/*in CFW_SmsCopyMessage, fix bug 3843, if else-->if         */
/*                                                                */
/*xueww[+] 2007.01.30              */
/*in CFW_SmsSendMessage, fix bug 3794,support send null SMS              */
/***************************************************************************************/
#include "cfw_sms_prv.h"
#include <csw_mem_prv.h>
#include "csw_debug.h"
//Here, define a golobal variable for longer msg
UINT32* gpLongerMsgPara = NULL;
UINT32* gpLongerMsgParaIndex = NULL;
CFW_SMS_LONG_INFO* gpWriteLongerMsgPara = NULL;
UINT8   gnWriteLongerMsgParaCount = 0x0;
//for long sms
PSTR pSMSDir      = "C:/System";
PSTR pSIMFilename = "C:/System/SimLong.txt";
PSTR pMEFilename  = "C:/System/MeLong.txt";

#if (CFW_SMS_DUMP_ENABLE==0) && (CFW_SMS_SRV_ENABLE==1)

///////////////////////////////////////////////////////////////////////////////
//                         Public APIs For SMS MO                            //
///////////////////////////////////////////////////////////////////////////////
#ifdef CFW_MULTI_SIM

PUBLIC UINT32 SRVAPI CFW_SmsSendMessage(CFW_DIALNUMBER* pNumber, UINT8* pData, UINT16 nDataSize, UINT16 nUTI, CFW_SIM_ID nSimId)
{
	CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
	UINT8             nFormat=0x00;
	UINT32            ret=0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsSendMessage);
	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSendMessage enter \n ");
	ret = CFW_CheckSimId(nSimId);
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_CheckSimId error ret=0x%x\n ", ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}

	ret = CFW_IsFreeUTI(nUTI,CFW_SMS_MO_SRV_ID);
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_IsFreeUTI error ret=0x%x\n ", ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSendMessage);
		return ret;
	}

	//Get SMS format from user setting
	CFW_CfgGetSmsFormat(&nFormat, nSimId);
	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSendMessage nFormat=0x%x\n ", nFormat);
	if(nFormat)//Text
	{
		UINT8 sNumber[2*TEL_NUMBER_MAX_LEN], uTmpNumberSize;//added by fengwei 20080709;

		SUL_ZeroMemory8(&sNumber, 2*TEL_NUMBER_MAX_LEN);
		
		//Check the user input data, do more later
		if((!pNumber) || (!pNumber->pDialNumber))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSendMessage);
			return ERR_CFW_INVALID_PARAMETER ;
		}
		if((pNumber->nDialNumberSize <= 0) ||(pNumber->nDialNumberSize > TEL_NUMBER_MAX_LEN ))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSendMessage);
			return ERR_CFW_INVALID_PARAMETER ;
		}
		if((pNumber->nClir!=0)&&(pNumber->nClir!=1)&&(pNumber->nClir!=2))
		{
			pNumber->nClir = 0;
		}
		if((pNumber->nType!=CFW_TELNUMBER_TYPE_INTERNATIONAL)&&  \
			(pNumber->nType!=CFW_TELNUMBER_TYPE_NATIONAL)&&      \
			(pNumber->nType!=CFW_TELNUMBER_TYPE_UNKNOWN))
		{
			pNumber->nType = CFW_TELNUMBER_TYPE_UNKNOWN;
		}

		uTmpNumberSize = SUL_GsmBcdToAscii(pNumber->pDialNumber, pNumber->nDialNumberSize, sNumber);
//hameina[-] sul function can return number size correctly		
#if 0		
		if(0xf0 == (*(pNumber->pDialNumber+pNumber->nDialNumberSize-1) & 0xf0))
			uTmpNumberSize = (UINT8)(pNumber->nDialNumberSize*2 - 1);
		else
			uTmpNumberSize = (UINT8)(pNumber->nDialNumberSize*2);
#endif
		//Malloc for Handle
		//modified by fengwei 20080709
		//pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO) + pNumber->nDialNumberSize + nDataSize);
		pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO) + uTmpNumberSize + nDataSize);
		if(!pSmsMOInfo)
		{
			CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSendMessage pSmsMOInfo malloc error!!! \n ");
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSendMessage);
			return ERR_CMS_MEMORY_FULL;
		}
		SUL_ZeroMemory8(pSmsMOInfo, (SIZEOF(CFW_SMS_MO_INFO) + uTmpNumberSize + nDataSize));//modified by fengwei 20080710

		//Set user input data to handle
		pSmsMOInfo->sNumber.nClir           = pNumber->nClir;
		pSmsMOInfo->sNumber.nType           = pNumber->nType;
		pSmsMOInfo->sNumber.nDialNumberSize = uTmpNumberSize;
		pSmsMOInfo->sNumber.pDialNumber     = (UINT8*)((UINT8*)pSmsMOInfo + SIZEOF(CFW_SMS_MO_INFO));
		SUL_MemCopy8((UINT8*)pSmsMOInfo->sNumber.pDialNumber,&sNumber,uTmpNumberSize);

		pSmsMOInfo->nDataSize = nDataSize;
		
		//[[hameina[mod]2007-3-22
		if(nDataSize)
			pSmsMOInfo->pData = (UINT8*)((UINT8*)pSmsMOInfo + SIZEOF(CFW_SMS_MO_INFO) + uTmpNumberSize);
		else
			pSmsMOInfo->pData = NULL;
		//]]hameina[mod]2007-3-22

		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSendMessage, send a text sms!nDataSize is %d \n ",nDataSize);
	}
	else //PDU
	{
		//Malloc for Handle
		pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO) + nDataSize);
		if(!pSmsMOInfo || nDataSize ==0)//hameina[mod] :add the last condition
		{
			CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSendMessage pSmsMOInfo malloc error!!! \n ");
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSendMessage);
			return ERR_CMS_MEMORY_FULL;
		}
		SUL_ZeroMemory8(pSmsMOInfo, (SIZEOF(CFW_SMS_MO_INFO) + nDataSize));

		pSmsMOInfo->nDataSize = nDataSize;
		pSmsMOInfo->pData = (UINT8*)((UINT8*)pSmsMOInfo + SIZEOF(CFW_SMS_MO_INFO));

		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSendMessage, send a PDU sms! nDataSize is %d\n ",nDataSize);
	}

	SUL_MemCopy8(pSmsMOInfo->pData,pData,nDataSize);
	pSmsMOInfo->nFormat = nFormat;
	pSmsMOInfo->nUTI_UserInput = nUTI;
	pSmsMOInfo->nUTI_Internal  = (UINT16)(SMS_MO_UTI_MIN + nUTI); 

	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_SMS_MO_SRV_ID, pSmsMOInfo, sms_SendProc, nSimId);
	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);
	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSendMessage);
	return ERR_SUCCESS;
}


PUBLIC UINT32 CFW_SmsDeleteMessage( UINT16 nIndex, UINT8 nStatus,UINT8 nStorage, UINT8 nType, UINT16 nUTI, CFW_SIM_ID nSimId)
{
	CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
	UINT32 ret=0x0;
	CFW_SMS_STORAGE_INFO  sStorageInfo;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsDeleteMessage);

	//Verify input parameters
	ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);
	if(ret != ERR_SUCCESS)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsDeleteMessage);
		return ret;
	}
	//check simid
	ret = CFW_CheckSimId(nSimId);
	if(ERR_SUCCESS != ret)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if((nStorage != CFW_SMS_STORAGE_ME)       && \
		(nStorage != CFW_SMS_STORAGE_SM)       && \
		(nStorage != CFW_SMS_STORAGE_AUTO))
	{
		CSW_PROFILE_FUNCTION_ENTER(CFW_SmsDeleteMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}
	//check msg type, currently only support pp msg
	if(nType != CFW_SMS_TYPE_PP)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}

	if(!nIndex && !nStatus)//delete by status=0
	{
		CSW_PROFILE_FUNCTION_ENTER(CFW_SmsDeleteMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}
	
	SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
	//Get max slot in SIM
	CFW_CfgGetSmsStorageInfo(&sStorageInfo,nStorage, nSimId);

	//Verify Storage
	if(nStorage == CFW_SMS_STORAGE_AUTO)
	{
		UINT8  nSmsStorageId=0x0;
		//Get SMS StorageID
		ret = CFW_CfgGetSmsStorage(&nSmsStorageId, nSimId);
		if (ret == ERR_SUCCESS)
		{
			nStorage = nSmsStorageId; 
		}
		else
		{
			CSW_PROFILE_FUNCTION_ENTER(CFW_SmsDeleteMessage);
			return ret;
		}
	}

	if(nStorage == CFW_SMS_STORAGE_SM)
	{
		if(nIndex > sStorageInfo.totalSlot )
		{ 
			CSW_PROFILE_FUNCTION_ENTER(CFW_SmsDeleteMessage);
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}
	else if(nStorage == CFW_SMS_STORAGE_ME)
	{
		if(nIndex > SMS_MAX_ENTRY_IN_ME)
		{
			CSW_PROFILE_FUNCTION_ENTER(CFW_SmsDeleteMessage);
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}

	if(CFW_SMS_STORED_STATUS_STORED_ALL == nStatus )
	{
		if(nStorage == CFW_SMS_STORAGE_ME)
			nStatus = 0x7f;
	}

	//Malloc for Handle
	pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
	if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsDeleteMessage pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_ENTER(CFW_SmsDeleteMessage);
		return ERR_CMS_MEMORY_FULL;
	}
	SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));


	//Set value to the handle
	pSmsMOInfo->nLocation = nStorage; 
	pSmsMOInfo->nIndex  = nIndex;
	pSmsMOInfo->nStatus = nStatus;
	pSmsMOInfo->nUTI_UserInput = nUTI;
	pSmsMOInfo->nUTI_Internal  = (UINT16)(CFW_APP_UTI_MIN + nUTI); 

	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_DleteAoProc, nSimId);
	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);

	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsDeleteMessage, nIndex is %d, nStatus is %d, nStorage is %d\n ",nIndex,nStatus,nStorage);

	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsDeleteMessage);
	return ERR_SUCCESS;
}


PUBLIC UINT32 CFW_SmsSetUnSent2Sent(UINT8 nStorage, UINT8 nType, UINT16 nIndex, UINT8 nSendStatus, UINT16 nUTI, CFW_SIM_ID nSimId)
{   
	CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
	UINT32 ret=0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsSetUnSent2Sent);

	//Verify input parameters
	ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);
	if(ret != ERR_SUCCESS)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2Sent);
		return ret;
	}
	//check simid
	ret = CFW_CheckSimId(nSimId);
	if(ERR_SUCCESS != ret)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(nType != CFW_SMS_TYPE_PP) 
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}

	if((nStorage != CFW_SMS_STORAGE_ME)     && \
		(nStorage != CFW_SMS_STORAGE_SM)     && \
		(nStorage != CFW_SMS_STORAGE_AUTO))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2Sent);
		return ERR_CFW_INVALID_PARAMETER;
	}
	
	if(nStorage == CFW_SMS_STORAGE_AUTO)
	{
		UINT8  nSmsStorageId=0x0;
		//Get SMS StorageID
		ret = CFW_CfgGetSmsStorage(&nSmsStorageId, nSimId);
		if (ret == ERR_SUCCESS)
		{
			if((nSmsStorageId != CFW_SMS_STORAGE_ME)  && \
				(nSmsStorageId != CFW_SMS_STORAGE_SM))
			{
				CSW_SMS_FREE(pSmsMOInfo);
				CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2Sent);
				return ERR_CFW_INVALID_PARAMETER;
			}
			nStorage = nSmsStorageId; 
		}
		else
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2Sent);
			return ERR_CFW_INVALID_PARAMETER;
		}
	}

	if(nStorage == CFW_SMS_STORAGE_SM)
	{
		CFW_SMS_STORAGE_INFO  sStorageInfo;
		SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
		//Get max slot in SIM
		
		CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM, nSimId);
		if((nIndex > sStorageInfo.totalSlot) || (nIndex <= 0))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2Sent); 
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}
	else if(nStorage == CFW_SMS_STORAGE_ME)
	{
		if((nIndex > SMS_MAX_ENTRY_IN_ME) || (nIndex <= 0))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2Sent); 
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}
	if((nSendStatus != CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)        && \
		(nSendStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV)   && \
		(nSendStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE)  && \
		(nSendStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2Sent);
		return ERR_CFW_INVALID_PARAMETER;
	}

	//Malloc for Handle
	pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
	if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSetUnSent2Sent pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2Sent);
		return ERR_CMS_MEMORY_FULL;
	}
	SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));


	//Set value to the handle
	pSmsMOInfo->nLocation = nStorage; 
	pSmsMOInfo->nIndex         = nIndex;
	pSmsMOInfo->nUTI_UserInput = nUTI;
	pSmsMOInfo->nUTI_Internal  = (UINT16)(CFW_APP_UTI_MIN + nUTI); 
	pSmsMOInfo->nStatus        = nSendStatus;

	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_SetUnSent2SentProc, nSimId);
	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);

	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSetUnSent2Sent, nStorage is %d, nIndex is %d, nSendStatus is %d\n ",nStorage,nIndex,nSendStatus);

	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2Sent);
	return ERR_SUCCESS;
}


PUBLIC UINT32 CFW_SmsSetUnRead2Read(UINT8 nStorage, UINT8 nType, UINT16 nIndex,UINT16 nUTI, CFW_SIM_ID nSimId)
{   
	CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
	UINT32 ret=0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsSetUnRead2Read);

	//Verify input parameters
	ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);
	if(ret != ERR_SUCCESS)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2Read);
		return ret;
	}
	//check simid
	ret = CFW_CheckSimId(nSimId);
	if(ERR_SUCCESS != ret)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(nType != CFW_SMS_TYPE_PP) 
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if((nStorage != CFW_SMS_STORAGE_ME)     && \
		(nStorage != CFW_SMS_STORAGE_SM)     && \
		(nStorage != CFW_SMS_STORAGE_AUTO))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2Read);
		return ERR_CFW_INVALID_PARAMETER;
	}
	
	if(nStorage == CFW_SMS_STORAGE_AUTO)
	{
		UINT8  nSmsStorageId=0x0;
		//Get SMS StorageID
		ret = CFW_CfgGetSmsStorage(&nSmsStorageId, nSimId);
		if (ret == ERR_SUCCESS)
		{
			if((nSmsStorageId != CFW_SMS_STORAGE_ME)  && \
				(nSmsStorageId != CFW_SMS_STORAGE_SM))
			{
				CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2Read);
				return ERR_CFW_INVALID_PARAMETER;
			}
			nStorage = nSmsStorageId; 
		}
		else
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2Read);
			return ERR_CFW_INVALID_PARAMETER;
		}
	}

	if(nStorage == CFW_SMS_STORAGE_SM)
	{
		CFW_SMS_STORAGE_INFO  sStorageInfo;
		SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
		//Get max slot in SIM
		CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM, nSimId);
		if((nIndex > sStorageInfo.totalSlot) || (nIndex <= 0))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2Read); 
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}
	else if(nStorage == CFW_SMS_STORAGE_ME)
	{
		if((nIndex > SMS_MAX_ENTRY_IN_ME) || (nIndex <= 0))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2Read); 
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}

	//Malloc for Handle
	pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
	if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSetUnRead2Read pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2Read);
		return ERR_CMS_MEMORY_FULL;
	}
	SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));


	//Set value to the handle
	pSmsMOInfo->nLocation = nStorage; 
	pSmsMOInfo->nIndex         = nIndex;
	pSmsMOInfo->nUTI_UserInput = nUTI;
	pSmsMOInfo->nUTI_Internal  = (UINT16)(CFW_APP_UTI_MIN + nUTI); 

	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_SetUnRead2ReadProc, nSimId);
	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);

	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSetUnRead2Read, nStorage is %d, nIndex is %d\n ",nStorage,nIndex );

	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2Read);
	return ERR_SUCCESS;
}

PUBLIC UINT32 CFW_SmsSetRead2UnRead(UINT8 nStorage, UINT8 nType, UINT16 nIndex, UINT16 nUTI, CFW_SIM_ID nSimId)
{   
	CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
	UINT32 ret=0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsSetRead2UnRead);

	//Verify input parameters
	ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);
	if(ret != ERR_SUCCESS)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetRead2UnRead);
		return ret;
	}
	//check simid
	ret = CFW_CheckSimId(nSimId);
	if(ERR_SUCCESS != ret)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(nType != CFW_SMS_TYPE_PP) 
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}
	
	if((nStorage != CFW_SMS_STORAGE_ME)     && \
		(nStorage != CFW_SMS_STORAGE_SM)     && \
		(nStorage != CFW_SMS_STORAGE_AUTO))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetRead2UnRead);
		return ERR_CFW_INVALID_PARAMETER;
	}
	
	if(nStorage == CFW_SMS_STORAGE_AUTO)
	{
		UINT8  nSmsStorageId=0x0;
		//Get SMS StorageID
		ret = CFW_CfgGetSmsStorage(&nSmsStorageId, nSimId);
		if (ret == ERR_SUCCESS)
		{
			if((nSmsStorageId != CFW_SMS_STORAGE_ME)  && \
				(nSmsStorageId != CFW_SMS_STORAGE_SM))
			{
				CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetRead2UnRead);
				return ERR_CFW_INVALID_PARAMETER;
			}
			nStorage = nSmsStorageId; 
		}
		else
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetRead2UnRead);
			return ERR_CFW_INVALID_PARAMETER;
		}
	}

	if(nStorage == CFW_SMS_STORAGE_SM)
	{
		CFW_SMS_STORAGE_INFO  sStorageInfo;
		SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
		//Get max slot in SIM
		CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM, nSimId);
		if((nIndex > sStorageInfo.totalSlot) || (nIndex <= 0))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetRead2UnRead); 
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}
	else if(nStorage == CFW_SMS_STORAGE_ME)
	{
		if((nIndex > SMS_MAX_ENTRY_IN_ME) || (nIndex <= 0))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetRead2UnRead); 
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}

	//Malloc for Handle
	pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
	if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSetRead2UnRead pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetRead2UnRead);
		return ERR_CMS_MEMORY_FULL;
	}
	SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));


	//Set value to the handle
	pSmsMOInfo->nLocation = nStorage; 
	pSmsMOInfo->nIndex         = nIndex;
	pSmsMOInfo->nUTI_UserInput = nUTI;
	pSmsMOInfo->nUTI_Internal  = (UINT16)(CFW_APP_UTI_MIN + nUTI); 

	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_SetRead2UnReadProc, nSimId);
	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);

	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSetRead2UnRead, nStorage is %d, nIndex is %d\n ",nStorage,nIndex );

	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetRead2UnRead);
	return ERR_SUCCESS;
}

PUBLIC UINT32 CFW_SmsCopyMessages(CFW_SMS_COPY * pCopyInfo, UINT16 nUTI, CFW_SIM_ID nSimId)
{
	CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
	UINT32            ret=0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsCopyMessages);

	//Verify input parameters
	ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);
	if(ret != ERR_SUCCESS)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsCopyMessages);
		return ret;
	}
	//check simid
	ret = CFW_CheckSimId(nSimId);
	if(ERR_SUCCESS != ret)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}

	if(pCopyInfo->nStartIndex <= 0)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsCopyMessages);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(pCopyInfo->nStatus == CFW_SMS_STORED_STATUS_STORED_ALL)
		pCopyInfo->nStatus = 0x7f;
	if((pCopyInfo->nOption != CFW_SMS_COPY_ME2SM) && (pCopyInfo->nOption != CFW_SMS_COPY_SM2ME))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsCopyMessages);
		return ERR_CFW_INVALID_PARAMETER;
	}

	//Malloc for Handle
	pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
	if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsCopyMessages pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsCopyMessages);
		return ERR_CMS_MEMORY_FULL;
	}
	SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));

	//Set value to the handle
	pSmsMOInfo->i               = 0x00;
	pSmsMOInfo->nListCount      = pCopyInfo->nCount;
	pSmsMOInfo->nStatus         = pCopyInfo->nStatus;
	pSmsMOInfo->nListStartIndex = pCopyInfo->nStartIndex;
	pSmsMOInfo->nListOption     = pCopyInfo->nOption;
	pSmsMOInfo->nUTI_UserInput  = nUTI;
	pSmsMOInfo->nUTI_Internal   = (UINT16)(CFW_APP_UTI_MIN + nUTI); 

	//
	//add for (listcount == 0x00)
	//
	if( 0x00 == pSmsMOInfo->nListCount )
	{
		CFW_SMS_STORAGE_INFO nStorageInfo;
		if( pCopyInfo->nOption == 0x00 )
		{
			CFW_CfgGetSmsStorageInfo(&nStorageInfo,CFW_SMS_STORAGE_ME, nSimId);
		}
		else
		{
			CFW_CfgGetSmsStorageInfo(&nStorageInfo,CFW_SMS_STORAGE_SM,nSimId);
		}

		if(pCopyInfo->nStatus & CFW_SMS_STORED_STATUS_UNREAD)
		{
			pSmsMOInfo->nListCount +=  nStorageInfo.unReadRecords;
		}
		//[[xueww[mod] 2007.01.30 fix bug 3843, if else-->if
		if(pCopyInfo->nStatus & CFW_SMS_STORED_STATUS_READ)
		{
			pSmsMOInfo->nListCount +=  nStorageInfo.readRecords;
		}
		if(pCopyInfo->nStatus & CFW_SMS_STORED_STATUS_UNSENT)
		{
			pSmsMOInfo->nListCount +=  nStorageInfo.unsentRecords;
		}
		//
		//这里有隐患；因为DOC和storageinfo有冲突
		//
		if((pCopyInfo->nStatus & CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)
			||(pCopyInfo->nStatus & CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV)
			||(pCopyInfo->nStatus & CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE)
			||(pCopyInfo->nStatus & CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE))
		{
			pSmsMOInfo->nListCount +=  nStorageInfo.sentRecords;
		} 
		//]]xueww[mod] 2007.01.30
	}
	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_CopyAoProc, nSimId);
	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);

	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsCopyMessages, nStatus is %d,  nCount is %d,  nStartIndex is %d, nOption is %d\n ",
		pCopyInfo->nStatus,pCopyInfo->nCount,pCopyInfo->nStartIndex,pCopyInfo->nOption );

	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsCopyMessages);
	return ERR_SUCCESS;
}


 UINT32 CFW_SmsReadMessage(UINT8 nStorage, UINT8 nType, UINT16 nIndex, UINT16 nUTI, CFW_SIM_ID nSimId)
{
	CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
	UINT32            ret=0x0;
	UINT8             nFormat=0x0,nShow=0x0;
	UINT8 nStorage1=0;
	CFW_SMS_STORAGE_INFO  sStorageInfo;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsReadMessage);

	//Verify input parameters
	ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);
	if(ret != ERR_SUCCESS)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ret;
	}
	//check storageid
	if((nStorage != CFW_SMS_STORAGE_ME)     && \
		(nStorage != CFW_SMS_STORAGE_SM)     && \
		(nStorage != CFW_SMS_STORAGE_AUTO) )
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}
	//check msg type, currently only support pp msg
	if(nType != CFW_SMS_TYPE_PP)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}
		
	//check simid
	ret = CFW_CheckSimId(nSimId);
	if(ERR_SUCCESS != ret)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}
	//check index
	if(CFW_SMS_STORAGE_AUTO == nStorage)
	{
		ret = CFW_CfgGetSmsStorage(&nStorage1, nSimId);
		nStorage = nStorage1;
	}
	
	//Get max slot in SIM
	SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
	CFW_CfgGetSmsStorageInfo(&sStorageInfo,nStorage, nSimId);
	if(nStorage == CFW_SMS_STORAGE_ME)	
	{
		if((nIndex>SMS_MAX_ENTRY_IN_ME) || (nIndex == 0))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage); 
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}
	else//sim
	{
		if((nIndex > sStorageInfo.totalSlot ) || (nIndex == 0))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage); 
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}

	//Malloc for Handle
	pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
	if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsReadMessage pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CMS_MEMORY_FULL;
	}
	SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));


	//Set value to the handle
	pSmsMOInfo->nLocation = nStorage; 
	pSmsMOInfo->nIndex         = nIndex;
	pSmsMOInfo->nUTI_UserInput = nUTI;
	pSmsMOInfo->nUTI_Internal  = (UINT16)(CFW_APP_UTI_MIN + nUTI); 

	CFW_CfgGetSmsFormat(&nFormat, nSimId);
	CFW_CfgGetSmsShowTextModeParam(&nShow, nSimId);
	pSmsMOInfo->nFormat  = nFormat; 
	pSmsMOInfo->nPath    = nShow;    //use nPath to save nShow,hh

	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_ReadAoProc, nSimId);
	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);
	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsReadMessage, nStorage is %d,  nIndex is %d \n ",nStorage,nIndex);

	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
	return ERR_SUCCESS;
}


PUBLIC UINT32 CFW_SmsWriteMessage(CFW_SMS_WRITE * pSMSWrite, UINT16 nUTI, CFW_SIM_ID nSimId)
{
	CFW_SMS_MO_INFO*   pSmsMOInfo = NULL;
	CFW_DIALNUMBER     sNumber;
	UINT8              nFormat=0x00;
	UINT32             ret=0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsWriteMessage);

	SUL_ZeroMemory8(&sNumber, SIZEOF(CFW_DIALNUMBER));
	//check simid
	ret = CFW_CheckSimId(nSimId);
	if(ERR_SUCCESS != ret)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}

	//Verify input parameters
	ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);

	if(ret != ERR_SUCCESS)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
		return ret;
	}

	//Verify Storage
	if(pSMSWrite->nStorage == CFW_SMS_STORAGE_AUTO)
	{
		UINT8  nSmsStorageId=0x0;
		//Get SMS StorageID
		
		ret = CFW_CfgGetSmsStorage(&nSmsStorageId, nSimId);
		if (ret == ERR_SUCCESS)
			pSMSWrite->nStorage = nSmsStorageId; 
		else
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
			return ERR_CFW_INVALID_PARAMETER;
		}
	}

	if(pSMSWrite->nStorage == CFW_SMS_STORAGE_SM)
	{
		CFW_SMS_STORAGE_INFO  sStorageInfo;
		SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
		//Get max slot in SIM
		CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM, nSimId);
		if(pSMSWrite->nIndex > sStorageInfo.totalSlot )
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage); 
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}
	else if(pSMSWrite->nStorage == CFW_SMS_STORAGE_ME)
	{
		if(pSMSWrite->nIndex > SMS_MAX_ENTRY_IN_ME)
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}
	else
	{

		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}
	//check msg type, currently only support pp msg
	if(pSMSWrite->nType != CFW_SMS_TYPE_PP)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}

	if((pSMSWrite->nStatus != CFW_SMS_STORED_STATUS_UNREAD)                && \
		(pSMSWrite->nStatus != CFW_SMS_STORED_STATUS_READ)                  && \
		(pSMSWrite->nStatus != CFW_SMS_STORED_STATUS_UNSENT)                && \
		(pSMSWrite->nStatus != CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)       && \
		(pSMSWrite->nStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV)  && \
		(pSMSWrite->nStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE) && \
		(pSMSWrite->nStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}

	//Get SMS format from user setting
	CFW_CfgGetSmsFormat(&nFormat, nSimId);
	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsWriteMessage nFormat=0x%x\n ", nFormat);
	if(nFormat)//Text
	{
		UINT8 sTmpNumber[2*TEL_NUMBER_MAX_LEN], uTmpNumberSize;//added by fengwei 20080709

		SUL_ZeroMemory8(&sTmpNumber, 2*TEL_NUMBER_MAX_LEN);
		//Check the user input data, do more later
		if(!pSMSWrite->pData) 
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
			return ERR_CFW_INVALID_PARAMETER ;
		}
		
		if(!pSMSWrite->sNumber.pDialNumber)
			pSMSWrite->sNumber = sNumber;

		if((pSMSWrite->sNumber.nClir!=0)&&(pSMSWrite->sNumber.nClir!=1)&&(pSMSWrite->sNumber.nClir!=2))
		{
			//default value
			pSMSWrite->sNumber.nClir = 0; 
		}
		if((pSMSWrite->sNumber.nType!=CFW_TELNUMBER_TYPE_INTERNATIONAL)&&  \
			(pSMSWrite->sNumber.nType!=CFW_TELNUMBER_TYPE_NATIONAL)&&      \
			(pSMSWrite->sNumber.nType!=CFW_TELNUMBER_TYPE_UNKNOWN))
		{
			//default value
			pSMSWrite->sNumber.nType = CFW_TELNUMBER_TYPE_UNKNOWN;
		}
		if(pSMSWrite->sNumber.nDialNumberSize > TEL_NUMBER_MAX_LEN )
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
			return ERR_CFW_INVALID_PARAMETER ;
		}

		uTmpNumberSize = SUL_GsmBcdToAscii(pSMSWrite->sNumber.pDialNumber, pSMSWrite->sNumber.nDialNumberSize, sTmpNumber);
//hameina delete the codes below, I think the sul_** can return the size correctly		
#if 0
		if(0xf0 == (*(pSMSWrite->sNumber.pDialNumber+pSMSWrite->sNumber.nDialNumberSize-1) & 0xf0))
			uTmpNumberSize = (UINT8)(pSMSWrite->sNumber.nDialNumberSize*2 - 1);
		else
			uTmpNumberSize = (UINT8)(pSMSWrite->sNumber.nDialNumberSize*2);
#endif
		//Malloc for Handle
		pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO)+uTmpNumberSize+pSMSWrite->nDataSize);
		if(!pSmsMOInfo)
		{
			CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsWriteMessage pSmsMOInfo malloc error!!! \n ");
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
			return ERR_CMS_MEMORY_FULL;
		}
		SUL_ZeroMemory8(pSmsMOInfo, (SIZEOF(CFW_SMS_MO_INFO)+uTmpNumberSize+pSMSWrite->nDataSize));//modified by fengwei 20080710 for bug 8698

		//Set user input data to handle
		pSmsMOInfo->sNumber.nClir           = pSMSWrite->sNumber.nClir;
		pSmsMOInfo->sNumber.nType           = pSMSWrite->sNumber.nType;
		pSmsMOInfo->sNumber.nDialNumberSize = uTmpNumberSize;
		pSmsMOInfo->sNumber.pDialNumber     = (UINT8*)((UINT8*)pSmsMOInfo + SIZEOF(CFW_SMS_MO_INFO));
		SUL_MemCopy8((UINT8*)pSmsMOInfo->sNumber.pDialNumber,&sTmpNumber,uTmpNumberSize);

		pSmsMOInfo->nDataSize = pSMSWrite->nDataSize;

		if(pSMSWrite->nDataSize)
			pSmsMOInfo->pData = (UINT8*)((UINT8*)pSmsMOInfo + SIZEOF(CFW_SMS_MO_INFO) + uTmpNumberSize);
		else
			pSmsMOInfo->pData = NULL;

		//CSW_TRACE(CFW_SMS_TS_ID, "---CFW_SmsWriteMessage, write a text sms,nDataSize is %d, nStorage is %d , nIndex is %d , nStatus is %d\n ",nDataSize,nStorage,nIndex,nStatus);
	}  
	else// Write a PDU
	{
		//Malloc for Handle
		pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO)+pSMSWrite->nDataSize);
		if(!pSmsMOInfo)
		{
			CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsWriteMessage pSmsMOInfo malloc error!!! \n ");
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
			return ERR_CMS_MEMORY_FULL;
		}
		SUL_ZeroMemory8(pSmsMOInfo, (SIZEOF(CFW_SMS_MO_INFO)+pSMSWrite->nDataSize));

		pSmsMOInfo->nDataSize = pSMSWrite->nDataSize;
		pSmsMOInfo->pData = (UINT8*)((UINT8*)pSmsMOInfo + SIZEOF(CFW_SMS_MO_INFO));;

		//CSW_TRACE(CFW_SMS_TS_ID, "---CFW_SmsWriteMessage, write a PDU sms,nDataSize is %d, nStorage is %d , nIndex is %d , nStatus is %d\n ",nDataSize,nStorage,nIndex,nStatus);
	}

	SUL_MemCopy8(pSmsMOInfo->pData,pSMSWrite->pData,pSMSWrite->nDataSize);

	pSmsMOInfo->nLocation = pSMSWrite->nStorage; 
	pSmsMOInfo->nFormat = nFormat;
	pSmsMOInfo->nIndex  = pSMSWrite->nIndex;
	pSmsMOInfo->nStatus = pSMSWrite->nStatus;
	pSmsMOInfo->nUTI_UserInput = nUTI;
	pSmsMOInfo->nUTI_Internal  = (UINT16)(CFW_APP_UTI_MIN + nUTI); 

	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_WriteAoProc, nSimId);

	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);
	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
	return ERR_SUCCESS;
}



PUBLIC UINT32 CFW_SmsListMessages(CFW_SMS_LIST * pListInfo, UINT16 nUTI, CFW_SIM_ID nSimId)
{
	CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
	UINT32            ret=0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsListMessages);

	//Verify input parameters
	ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);
	if(ret != ERR_SUCCESS)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages);
		return ret;
	}
	//check simid
	ret = CFW_CheckSimId(nSimId);
	if(ERR_SUCCESS != ret)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}
	//check msg type, currently only support pp msg
	if(pListInfo->nType != CFW_SMS_TYPE_PP)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}
	
	if((pListInfo->nStorage != CFW_SMS_STORAGE_ME)     && \
		(pListInfo->nStorage != CFW_SMS_STORAGE_SM)     && \
		(pListInfo->nStorage != CFW_SMS_STORAGE_AUTO))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(pListInfo->nStartIndex <= 0)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages);
		return ERR_CFW_INVALID_PARAMETER;
	}
	
	//Verify Storage
	if(pListInfo->nStorage == CFW_SMS_STORAGE_AUTO)
	{
		UINT8  nSmsStorageId=0x0;
		//Get SMS StorageID
		ret = CFW_CfgGetSmsStorage(&nSmsStorageId, nSimId);
		if (ret == ERR_SUCCESS)
		{
			if((nSmsStorageId != CFW_SMS_STORAGE_ME)  && \
				(nSmsStorageId != CFW_SMS_STORAGE_SM))
			{
				CSW_SMS_FREE(pSmsMOInfo);
				CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages);
				return ERR_CFW_INVALID_PARAMETER;
			}
			pListInfo->nStorage = nSmsStorageId; 
		}
		else
		{
			CSW_SMS_FREE(pSmsMOInfo);
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages);
			return ERR_CFW_INVALID_PARAMETER;
		}
	}

	if(pListInfo->nStorage == CFW_SMS_STORAGE_SM)
	{
		CFW_SMS_STORAGE_INFO  sStorageInfo;
		SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
		//Get max slot in SIM
		CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM, nSimId);
		if(pListInfo->nStartIndex > sStorageInfo.totalSlot )
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages); 
			return ERR_CFW_INVALID_PARAMETER ;
		}
	}
	else if(pListInfo->nStorage == CFW_SMS_STORAGE_ME)
	{
		if(pListInfo->nStartIndex > SMS_MAX_ENTRY_IN_ME)
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages);
			CSW_TRACE(CFW_SMS_TS_ID, "The SMS_MAX_ENTRY_IN_ME is %d \n",SMS_MAX_ENTRY_IN_ME);
			return ERR_CFW_INVALID_PARAMETER ;
		}
	}
	if(pListInfo->nStatus == 0xff)
		pListInfo->nStatus = CFW_SMS_STORED_STATUS_UNREAD;
	if(pListInfo->nStatus == CFW_SMS_STORED_STATUS_STORED_ALL)
		pListInfo->nStatus = 0x7f;
	if(!(pListInfo->nOption & CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(!(pListInfo->nOption & CFW_SMS_MULTIPART_OPTION_ALL) && !(pListInfo->nOption & CFW_SMS_MULTIPART_OPTION_FIRST))
		pListInfo->nOption = (UINT8)(pListInfo->nOption & CFW_SMS_MULTIPART_OPTION_ALL);

	//Malloc for Handle
	pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
	if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsListMessages pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages);
		return ERR_CMS_MEMORY_FULL;
	}
	SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));

	//Set value to the handle
	pSmsMOInfo->nLocation = pListInfo->nStorage; 
	pSmsMOInfo->i               = 0x00;
	pSmsMOInfo->nListCount      = pListInfo->nCount;
	pSmsMOInfo->nStatus         = pListInfo->nStatus;
	pSmsMOInfo->nListStartIndex = pListInfo->nStartIndex;
	pSmsMOInfo->nListOption     = pListInfo->nOption;
	pSmsMOInfo->nUTI_UserInput  = nUTI;
	pSmsMOInfo->nUTI_Internal   = (UINT16)(CFW_APP_UTI_MIN + nUTI); 

	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_ListAoProc, nSimId);
	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);

	//CSW_TRACE(CFW_SMS_TS_ID, "---CFW_SmsListMessages ,nStorage is %d, nStatus is %d , nCount is %d , nStartIndex is, nOption is %d\n ",nStorage,nStatus,nCount,nStartIndex,nOption);

	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages);
	return ERR_SUCCESS;
}


PUBLIC UINT32 SRVAPI CFW_SmsGetMessageNode(PVOID pListResult, UINT16 nIndex,CFW_SMS_NODE **pNode, CFW_SIM_ID nSimId)
{
	UINT8   nFormat=0x0,nShow=0x0;
	UINT32  sDecomposeOutData=0x0,ret=0x0;
	UINT16  nListAddrCount=0x0;
	CFW_SMS_NODE* pListAddr=NULL;
	CFW_SMS_MULTIPART_INFO sLongerMsg;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsGetMessageNode);

	CFW_CfgGetSmsFormat(&nFormat, nSimId);
	CFW_CfgGetSmsShowTextModeParam(&nShow, nSimId);

	//Zero memory
	SUL_ZeroMemory8(&sLongerMsg,SIZEOF(CFW_SMS_MULTIPART_INFO));

	if(!pListResult)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsGetMessageNode);
		return ERR_CFW_INVALID_PARAMETER ;
	}
	if(!pNode)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsGetMessageNode);
		return ERR_CFW_INVALID_PARAMETER ;
	}
	//Set value  
	nListAddrCount = *((UINT16*)pListResult);
	if(nListAddrCount == 0)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsGetMessageNode);
		return ERR_CFW_INVALID_PARAMETER ;
	}
	if((nIndex + 1) > nListAddrCount)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsGetMessageNode);
		return ERR_CFW_INVALID_PARAMETER ;
	}

	pListAddr = (CFW_SMS_NODE*)(*((UINT32*)pListResult + 1 + nIndex));
	if(!pListAddr)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsGetMessageNode);
		return ERR_CFW_INVALID_PARAMETER ;
	}

	ret = sms_mo_Parse_PDU(nFormat,nShow,(UINT8)pListAddr->nStatus,pListAddr->nStorage,pListAddr->pNode,&pListAddr->nType,&sDecomposeOutData,&sLongerMsg);
	if((ret != ERR_SUCCESS) || (sDecomposeOutData == 0))
	{
		CFW_SMS_NODE* pTmpNode=NULL;
		pTmpNode   = (CFW_SMS_NODE *)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO) + SIZEOF(CFW_SMS_NODE));
		if(!pTmpNode)
		{
			CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsGetMessageNode pSmsMOInfo malloc error!!! \n ");
			CSW_SMS_FREE(pListAddr);
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsGetMessageNode);
			return ERR_NO_MORE_MEMORY ;
		}
		SUL_ZeroMemory8(pTmpNode,(SIZEOF(CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO) + SIZEOF(CFW_SMS_NODE)));
		pTmpNode->pNode = (PVOID)((UINT8*)pTmpNode + sizeof(CFW_SMS_NODE));
		sDecomposeOutData = (UINT32)pTmpNode;
	}
	//Set value
	((CFW_SMS_NODE*)sDecomposeOutData)->nConcatPrevIndex     = pListAddr->nConcatPrevIndex;
	((CFW_SMS_NODE*)sDecomposeOutData)->nConcatCurrentIndex  = pListAddr->nConcatCurrentIndex;
	((CFW_SMS_NODE*)sDecomposeOutData)->nConcatNextIndex     = pListAddr->nConcatNextIndex;
	((CFW_SMS_NODE*)sDecomposeOutData)->nStatus  = pListAddr->nStatus;
	((CFW_SMS_NODE*)sDecomposeOutData)->nType    = pListAddr->nType;
	((CFW_SMS_NODE*)sDecomposeOutData)->nStorage = pListAddr->nStorage;
	CSW_SMS_FREE(pListAddr);
	*pNode = (CFW_SMS_NODE*)sDecomposeOutData;
	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsGetMessageNode);
	return ERR_SUCCESS;
}


#else
PUBLIC UINT32 SRVAPI CFW_SmsSendMessage(CFW_DIALNUMBER* pNumber, UINT8* pData, UINT16 nDataSize, UINT16 nUTI)
{
	CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
	UINT8             nFormat=0x00;
	UINT32            ret=0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsSendMessage);

	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSendMessage enter \n ");
	CFW_CfgGetSmsFormat(&nFormat);
	ret = CFW_IsFreeUTI(nUTI,CFW_SMS_MO_SRV_ID);
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_IsFreeUTI error ret=0x%x\n ", ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSendMessage);
		return ret;
	}
	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSendMessage nFormat=0x%x\n ", nFormat);
	if(nFormat)//Text
	{
		UINT8 sNumber[2*TEL_NUMBER_MAX_LEN], uTmpNumberSize;//added by fengwei 20080709;

		SUL_ZeroMemory8(&sNumber, 2*TEL_NUMBER_MAX_LEN);
		
		//Check the user input data, do more later
		if((!pNumber) || (!pNumber->pDialNumber))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSendMessage);
			return ERR_CFW_INVALID_PARAMETER ;
		}
		if((pNumber->nDialNumberSize <= 0) ||(pNumber->nDialNumberSize > TEL_NUMBER_MAX_LEN ))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSendMessage);
			return ERR_CFW_INVALID_PARAMETER ;
		}
		if((pNumber->nClir!=0)&&(pNumber->nClir!=1)&&(pNumber->nClir!=2))
		{
			pNumber->nClir = 0;
		}
		if((pNumber->nType!=CFW_TELNUMBER_TYPE_INTERNATIONAL)&&  \
			(pNumber->nType!=CFW_TELNUMBER_TYPE_NATIONAL)&&      \
			(pNumber->nType!=CFW_TELNUMBER_TYPE_UNKNOWN))
		{
			pNumber->nType = CFW_TELNUMBER_TYPE_UNKNOWN;
		}

		//[[xueww[mod] 2007.07.27 sms_tool_BCD2ASCII--->SUL_GsmBcdToAscii
		//sms_tool_BCD2ASCII(sNumber, pNumber->pDialNumber, pNumber->nDialNumberSize);
		SUL_GsmBcdToAscii(pNumber->pDialNumber, pNumber->nDialNumberSize, sNumber);
		//]]xueww[mod] 2007.07.27
		
		if(0xf0 == (*(pNumber->pDialNumber+pNumber->nDialNumberSize-1) & 0xf0))
			uTmpNumberSize = (UINT8)(pNumber->nDialNumberSize*2 - 1);
		else
			uTmpNumberSize = (UINT8)(pNumber->nDialNumberSize*2);

		//Malloc for Handle
		//modified by fengwei 20080709
		//pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO) + pNumber->nDialNumberSize + nDataSize);
		pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO) + uTmpNumberSize + nDataSize);
		if(!pSmsMOInfo)
		{
			CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSendMessage pSmsMOInfo malloc error!!! \n ");
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSendMessage);
			return ERR_CMS_MEMORY_FULL;
		}
		SUL_ZeroMemory8(pSmsMOInfo, (SIZEOF(CFW_SMS_MO_INFO) + uTmpNumberSize + nDataSize));//modified by fengwei 20080710

		//Set user input data to handle
		pSmsMOInfo->sNumber.nClir           = pNumber->nClir;
		pSmsMOInfo->sNumber.nType           = pNumber->nType;
		pSmsMOInfo->sNumber.nDialNumberSize = uTmpNumberSize;
		pSmsMOInfo->sNumber.pDialNumber     = (UINT8*)((UINT8*)pSmsMOInfo + SIZEOF(CFW_SMS_MO_INFO));
		SUL_MemCopy8((UINT8*)pSmsMOInfo->sNumber.pDialNumber,&sNumber,uTmpNumberSize);

		pSmsMOInfo->nDataSize = nDataSize;
		
		//[[hameina[mod]2007-3-22
		if(nDataSize)
			pSmsMOInfo->pData = (UINT8*)((UINT8*)pSmsMOInfo + SIZEOF(CFW_SMS_MO_INFO) + uTmpNumberSize);
		else
			pSmsMOInfo->pData = NULL;
		//]]hameina[mod]2007-3-22

		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSendMessage, send a text sms!nDataSize is %d \n ",nDataSize);
	}
	else //PDU
	{
		//Malloc for Handle
		pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO) + nDataSize);
		if(!pSmsMOInfo || nDataSize ==0)//hameina[mod] :add the last condition
		{
			CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSendMessage pSmsMOInfo malloc error!!! \n ");
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSendMessage);
			return ERR_CMS_MEMORY_FULL;
		}
		SUL_ZeroMemory8(pSmsMOInfo, (SIZEOF(CFW_SMS_MO_INFO) + nDataSize));

		pSmsMOInfo->nDataSize = nDataSize;
		pSmsMOInfo->pData = (UINT8*)((UINT8*)pSmsMOInfo + SIZEOF(CFW_SMS_MO_INFO));

		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSendMessage, send a PDU sms! nDataSize is %d\n ",nDataSize);
	}

	SUL_MemCopy8(pSmsMOInfo->pData,pData,nDataSize);
	pSmsMOInfo->nFormat = nFormat;
	pSmsMOInfo->nUTI_UserInput = nUTI;
	pSmsMOInfo->nUTI_Internal  = (UINT16)(SMS_MO_UTI_MIN + nUTI); 

	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_SMS_MO_SRV_ID, pSmsMOInfo, sms_SendProc);
	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);

	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSendMessage);
	return ERR_SUCCESS;
}


PUBLIC UINT32 SRVAPI CFW_SmsDeleteMessage( UINT16 nIndex, UINT8 nStatus,UINT16 nStorage, UINT16 nUTI)
{
	CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
	UINT32 ret=0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsDeleteMessage);

	//Verify input parameters
	ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);
	if(ret != ERR_SUCCESS)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsDeleteMessage);
		return ret;
	}
	if((nStorage != CFW_SMS_STORAGE_ME)       && \
		(nStorage != CFW_SMS_STORAGE_SM)       && \
		(nStorage != CFW_SMS_STORAGE_SM_ME)    && \
		(nStorage != CFW_SMS_STORAGE_AUTO))
	{
		CSW_PROFILE_FUNCTION_ENTER(CFW_SmsDeleteMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(!nIndex && !nStatus)//delete by status=0
		return ERR_CFW_INVALID_PARAMETER;

	if(nStorage == CFW_SMS_STORAGE_SM)
	{
		CFW_SMS_STORAGE_INFO  sStorageInfo;
		SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
		//Get max slot in SIM
		CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM);
		if(nIndex > sStorageInfo.totalSlot )
		{ 
			CSW_PROFILE_FUNCTION_ENTER(CFW_SmsDeleteMessage);
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}
	if(nStorage == CFW_SMS_STORAGE_ME)
	{
		if(nIndex > SMS_MAX_ENTRY_IN_ME)
		{
			CSW_PROFILE_FUNCTION_ENTER(CFW_SmsDeleteMessage);
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}

	if(CFW_SMS_STORED_STATUS_STORED_ALL == nStatus )
	{
		if((nStorage == CFW_SMS_STORAGE_ME) || (nStorage == CFW_SMS_STORAGE_SM_ME))
			nStatus = 0x7f;
	}

	//Malloc for Handle
	pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
	if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsDeleteMessage pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_ENTER(CFW_SmsDeleteMessage);
		return ERR_CMS_MEMORY_FULL;
	}
	SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));

	//Verify Storage
	if(nStorage == CFW_SMS_STORAGE_AUTO)
	{
		UINT8  nSmsStorageId=0x0;
		//Get SMS StorageID
		ret = CFW_CfgGetSmsStorage(&nSmsStorageId);
		if (ret == ERR_SUCCESS)
		{
			if((nSmsStorageId != CFW_SMS_STORAGE_ME) && \
				(nSmsStorageId != CFW_SMS_STORAGE_SM))
			{
				CSW_SMS_FREE(pSmsMOInfo);
				CSW_PROFILE_FUNCTION_ENTER(CFW_SmsDeleteMessage);
				return ERR_CFW_INVALID_PARAMETER;
			}
			pSmsMOInfo->nLocation = nSmsStorageId; 
		}
		else
		{
			CSW_SMS_FREE(pSmsMOInfo);
			CSW_PROFILE_FUNCTION_ENTER(CFW_SmsDeleteMessage);
			return ERR_CFW_INVALID_PARAMETER;
		}
	}
	else
		pSmsMOInfo->nLocation = nStorage; 

	//Set value to the handle
	pSmsMOInfo->nIndex  = nIndex;
	pSmsMOInfo->nStatus = nStatus;
	pSmsMOInfo->nUTI_UserInput = nUTI;
	pSmsMOInfo->nUTI_Internal  = (UINT16)(CFW_APP_UTI_MIN + nUTI); 

	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_DleteAoProc);
	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);

	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsDeleteMessage, nIndex is %d, nStatus is %d, nStorage is %d\n ",nIndex,nStatus,nStorage);

	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsDeleteMessage);
	return ERR_SUCCESS;
}

PUBLIC UINT32 SRVAPI CFW_SmsDeleteMessageSync( UINT16 nIndex, UINT8 nStatus,UINT16 nStorage, CFW_SMS_OUT_PARAM* pOutParam)
{
    CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
    UINT32 ret=0x0;
    CSW_PROFILE_FUNCTION_ENTER(CFW_SmsDeleteMessageSync);
    
    if(nStorage != CFW_SMS_STORAGE_ME){ 
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsDeleteMessageSync);
      return ERR_CFW_INVALID_PARAMETER;
      }
    if(!nIndex)//delete by status
    {
        if((nStatus != CFW_SMS_STORED_STATUS_UNREAD)                 && \
           (nStatus != CFW_SMS_STORED_STATUS_READ)                   && \
           (nStatus != CFW_SMS_STORED_STATUS_UNSENT)                 && \
           (nStatus != CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)        && \
           (nStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV)   && \
           (nStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE)  && \
           (nStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE) && \
           (nStatus != CFW_SMS_STORED_STATUS_STORED_ALL))
        {
        	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsDeleteMessageSync);
            return ERR_CFW_INVALID_PARAMETER;
        }
    }
    if(nStorage == CFW_SMS_STORAGE_SM)
    {
        CFW_SMS_STORAGE_INFO  sStorageInfo;
        SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
        //Get max slot in SIM
        CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM);
        if(nIndex > sStorageInfo.totalSlot ){ 
        	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsDeleteMessageSync);
            return ERR_CMS_INVALID_MEMORY_INDEX;
          }
    }
    if(nStorage == CFW_SMS_STORAGE_ME)
    {
        if(nIndex > SMS_MAX_ENTRY_IN_ME){
        	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsDeleteMessageSync);
          return ERR_CMS_INVALID_MEMORY_INDEX;
          }
    }

    if(CFW_SMS_STORED_STATUS_STORED_ALL == nStatus )
    {
        if((nStorage == CFW_SMS_STORAGE_ME) || (nStorage == CFW_SMS_STORAGE_SM_ME))
            nStatus = 0x7f;
    }

    //Malloc for Handle
    pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
    if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsDeleteMessageSync pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsDeleteMessageSync);
		return ERR_CMS_MEMORY_FULL;
      }
    SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));
    
    //Verify Storage
    if(nStorage == CFW_SMS_STORAGE_AUTO)
    {
        UINT8  nSmsStorageId=0x0;
        //Get SMS StorageID
        ret = CFW_CfgGetSmsStorage(&nSmsStorageId);
        if (ret == ERR_SUCCESS)
        {
            if((nSmsStorageId != CFW_SMS_STORAGE_ME) && \
               (nSmsStorageId != CFW_SMS_STORAGE_SM))
            {
                CSW_SMS_FREE(pSmsMOInfo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SmsDeleteMessageSync);
                return ERR_CFW_INVALID_PARAMETER;
            }
            pSmsMOInfo->nLocation = nSmsStorageId; 
        }
        else
        {
            CSW_SMS_FREE(pSmsMOInfo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SmsDeleteMessageSync);
            return ERR_CFW_INVALID_PARAMETER;
        }
    }
    else
        pSmsMOInfo->nLocation = nStorage; 
    //Set value to the handle
    pSmsMOInfo->nIndex  = nIndex;
    pSmsMOInfo->nStatus = nStatus;

    //Sync Api
    sms_Sync_Delete(pSmsMOInfo, pOutParam);
    if(pOutParam->nType == 0xF0)
    	{
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsDeleteMessageSync);
      return pOutParam->nParam1;
      }
    else
    	{
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsDeleteMessageSync);
      return ERR_SUCCESS;
      }
}

PUBLIC UINT32 SRVAPI CFW_SmsSetUnSent2Sent(UINT16 nStorage,UINT16 nIndex,UINT8 nSendStatus,UINT16 nUTI)
{   
	CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
	UINT32 ret=0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsSetUnSent2Sent);

	//Verify input parameters
	ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);
	if(ret != ERR_SUCCESS)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2Sent);
		return ret;
	}
	if((nStorage != CFW_SMS_STORAGE_ME)     && \
		(nStorage != CFW_SMS_STORAGE_SM)     && \
		(nStorage != CFW_SMS_STORAGE_AUTO))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2Sent);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(nStorage == CFW_SMS_STORAGE_SM)
	{
		CFW_SMS_STORAGE_INFO  sStorageInfo;
		SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
		//Get max slot in SIM
		CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM);
		if((nIndex > sStorageInfo.totalSlot) || (nIndex <= 0))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2Sent); 
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}
	if(nStorage == CFW_SMS_STORAGE_ME)
	{
		if((nIndex > SMS_MAX_ENTRY_IN_ME) || (nIndex <= 0))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2Sent); 
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}
	if((nSendStatus != CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)        && \
		(nSendStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV)   && \
		(nSendStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE)  && \
		(nSendStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2Sent);
		return ERR_CFW_INVALID_PARAMETER;
	}

	//Malloc for Handle
	pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
	if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSetUnSent2Sent pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2Sent);
		return ERR_CMS_MEMORY_FULL;
	}
	SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));

	if(nStorage == CFW_SMS_STORAGE_AUTO)
	{
		UINT8  nSmsStorageId=0x0;
		//Get SMS StorageID
		ret = CFW_CfgGetSmsStorage(&nSmsStorageId);
		if (ret == ERR_SUCCESS)
		{
			if((nSmsStorageId != CFW_SMS_STORAGE_ME)  && \
				(nSmsStorageId != CFW_SMS_STORAGE_SM))
			{
				CSW_SMS_FREE(pSmsMOInfo);
				CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2Sent);
				return ERR_CFW_INVALID_PARAMETER;
			}
			pSmsMOInfo->nLocation = nSmsStorageId; 
		}
		else
		{
			CSW_SMS_FREE(pSmsMOInfo);
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2Sent);
			return ERR_CFW_INVALID_PARAMETER;
		}
	}
	else
		pSmsMOInfo->nLocation = nStorage; 

	//Set value to the handle
	pSmsMOInfo->nIndex         = nIndex;
	pSmsMOInfo->nUTI_UserInput = nUTI;
	pSmsMOInfo->nUTI_Internal  = (UINT16)(CFW_APP_UTI_MIN + nUTI); 
	pSmsMOInfo->nStatus        = nSendStatus;

	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_SetUnSent2SentProc);
	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);

	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSetUnSent2Sent, nStorage is %d, nIndex is %d, nSendStatus is %d\n ",nStorage,nIndex,nSendStatus);

	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2Sent);
	return ERR_SUCCESS;
}

PUBLIC UINT32 SRVAPI CFW_SmsSetUnSent2SentSync(UINT16 nStorage,UINT16 nIndex,UINT8 nSendStatus)
{   
    CFW_SMS_OUT_PARAM sOutParam;
    CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
    UINT32 ret=0x0;
    CSW_PROFILE_FUNCTION_ENTER(CFW_SmsSetUnSent2SentSync);

    SUL_ZeroMemory8(&sOutParam,SIZEOF(CFW_SMS_OUT_PARAM));
    if(nStorage != CFW_SMS_STORAGE_ME){
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2SentSync); 
      return ERR_CFW_INVALID_PARAMETER;
      }
    if(nStorage == CFW_SMS_STORAGE_SM)
    {
        CFW_SMS_STORAGE_INFO  sStorageInfo;
        SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
        //Get max slot in SIM
        CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM);
        if((nIndex > sStorageInfo.totalSlot) || (nIndex <= 0)){
        	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2SentSync); 
          return ERR_CMS_INVALID_MEMORY_INDEX;
          }
    }
    if(nStorage == CFW_SMS_STORAGE_ME)
    {
        if((nIndex > SMS_MAX_ENTRY_IN_ME) || (nIndex <= 0)){
        	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2SentSync); 
          return ERR_CMS_INVALID_MEMORY_INDEX;
          }
    }
    if((nSendStatus != CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)        && \
       (nSendStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV)   && \
       (nSendStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE)  && \
       (nSendStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE)){
       	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2SentSync);
      return ERR_CFW_INVALID_PARAMETER;
      }

    //Malloc for Handle
    pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
    if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSetUnSent2SentSync pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2SentSync);
		return ERR_CMS_MEMORY_FULL;
      }
    SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));
    
    if(nStorage == CFW_SMS_STORAGE_AUTO)
    {
        UINT8  nSmsStorageId=0x0;
        //Get SMS StorageID
        ret = CFW_CfgGetSmsStorage(&nSmsStorageId);
        if (ret == ERR_SUCCESS)
        {
            if((nSmsStorageId != CFW_SMS_STORAGE_ME)  && \
               (nSmsStorageId != CFW_SMS_STORAGE_SM))
            {
                CSW_SMS_FREE(pSmsMOInfo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2SentSync);
                return ERR_CFW_INVALID_PARAMETER;
            }
            pSmsMOInfo->nLocation = nSmsStorageId; 
        }
        else
        {
            CSW_SMS_FREE(pSmsMOInfo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2SentSync);
            return ERR_CFW_INVALID_PARAMETER;
        }
    }
    else
        pSmsMOInfo->nLocation = nStorage; 
    
    //Set value to the handle
    pSmsMOInfo->nIndex  = nIndex;

    //Sync Api
    sms_Sync_SetUnSent2Sent(pSmsMOInfo, &sOutParam);
    if(sOutParam.nType == 0xF0){
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2SentSync);
      return sOutParam.nParam1;
      }
    else{
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnSent2SentSync);
      return ERR_SUCCESS;
      }
}


PUBLIC UINT32 SRVAPI CFW_SmsSetUnRead2Read(UINT16 nStorage,UINT16 nIndex,UINT16 nUTI)
{   
	CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
	UINT32 ret=0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsSetUnRead2Read);

	//Verify input parameters
	ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);
	if(ret != ERR_SUCCESS)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2Read);
		return ret;
	}
	if((nStorage != CFW_SMS_STORAGE_ME)     && \
		(nStorage != CFW_SMS_STORAGE_SM)     && \
		(nStorage != CFW_SMS_STORAGE_AUTO))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2Read);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(nStorage == CFW_SMS_STORAGE_SM)
	{
		CFW_SMS_STORAGE_INFO  sStorageInfo;
		SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
		//Get max slot in SIM
		CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM);
		if((nIndex > sStorageInfo.totalSlot) || (nIndex <= 0))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2Read); 
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}
	if(nStorage == CFW_SMS_STORAGE_ME)
	{
		if((nIndex > SMS_MAX_ENTRY_IN_ME) || (nIndex <= 0))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2Read); 
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}

	//Malloc for Handle
	pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
	if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSetUnRead2Read pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2Read);
		return ERR_CMS_MEMORY_FULL;
	}
	SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));

	if(nStorage == CFW_SMS_STORAGE_AUTO)
	{
		UINT8  nSmsStorageId=0x0;
		//Get SMS StorageID
		ret = CFW_CfgGetSmsStorage(&nSmsStorageId);
		if (ret == ERR_SUCCESS)
		{
			if((nSmsStorageId != CFW_SMS_STORAGE_ME)  && \
				(nSmsStorageId != CFW_SMS_STORAGE_SM))
			{
				CSW_SMS_FREE(pSmsMOInfo);
				CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2Read);
				return ERR_CFW_INVALID_PARAMETER;
			}
			pSmsMOInfo->nLocation = nSmsStorageId; 
		}
		else
		{
			CSW_SMS_FREE(pSmsMOInfo);
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2Read);
			return ERR_CFW_INVALID_PARAMETER;
		}
	}
	else
		pSmsMOInfo->nLocation = nStorage; 

	//Set value to the handle
	pSmsMOInfo->nIndex         = nIndex;
	pSmsMOInfo->nUTI_UserInput = nUTI;
	pSmsMOInfo->nUTI_Internal  = (UINT16)(CFW_APP_UTI_MIN + nUTI); 

	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_SetUnRead2ReadProc);
	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);

	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSetUnRead2Read, nStorage is %d, nIndex is %d\n ",nStorage,nIndex );

	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2Read);
	return ERR_SUCCESS;
}

PUBLIC UINT32 SRVAPI CFW_SmsSetRead2UnRead(UINT16 nStorage,UINT16 nIndex,UINT16 nUTI)
{   
	CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
	UINT32 ret=0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsSetRead2UnRead);

	//Verify input parameters
	ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);
	if(ret != ERR_SUCCESS)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetRead2UnRead);
		return ret;
	}
	if((nStorage != CFW_SMS_STORAGE_ME)     && \
		(nStorage != CFW_SMS_STORAGE_SM)     && \
		(nStorage != CFW_SMS_STORAGE_AUTO))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetRead2UnRead);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(nStorage == CFW_SMS_STORAGE_SM)
	{
		CFW_SMS_STORAGE_INFO  sStorageInfo;
		SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
		//Get max slot in SIM
		CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM);
		if((nIndex > sStorageInfo.totalSlot) || (nIndex <= 0))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetRead2UnRead); 
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}
	if(nStorage == CFW_SMS_STORAGE_ME)
	{
		if((nIndex > SMS_MAX_ENTRY_IN_ME) || (nIndex <= 0))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetRead2UnRead); 
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}

	//Malloc for Handle
	pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
	if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSetRead2UnRead pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetRead2UnRead);
		return ERR_CMS_MEMORY_FULL;
	}
	SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));

	if(nStorage == CFW_SMS_STORAGE_AUTO)
	{
		UINT8  nSmsStorageId=0x0;
		//Get SMS StorageID
		ret = CFW_CfgGetSmsStorage(&nSmsStorageId);
		if (ret == ERR_SUCCESS)
		{
			if((nSmsStorageId != CFW_SMS_STORAGE_ME)  && \
				(nSmsStorageId != CFW_SMS_STORAGE_SM))
			{
				CSW_SMS_FREE(pSmsMOInfo);
				CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetRead2UnRead);
				return ERR_CFW_INVALID_PARAMETER;
			}
			pSmsMOInfo->nLocation = nSmsStorageId; 
		}
		else
			{
			CSW_SMS_FREE(pSmsMOInfo);
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetRead2UnRead);
			return ERR_CFW_INVALID_PARAMETER;
		}
	}
	else
		pSmsMOInfo->nLocation = nStorage; 

	//Set value to the handle
	pSmsMOInfo->nIndex         = nIndex;
	pSmsMOInfo->nUTI_UserInput = nUTI;
	pSmsMOInfo->nUTI_Internal  = (UINT16)(CFW_APP_UTI_MIN + nUTI); 

	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_SetRead2UnReadProc);
	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);

	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSetRead2UnRead, nStorage is %d, nIndex is %d\n ",nStorage,nIndex );

	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetRead2UnRead);
	return ERR_SUCCESS;
}

PUBLIC UINT32 SRVAPI CFW_SmsSetUnRead2ReadSync(UINT16 nStorage,UINT16 nIndex)
{   
    CFW_SMS_OUT_PARAM sOutParam;
    CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
    UINT32 ret=0x0;
    CSW_PROFILE_FUNCTION_ENTER(CFW_SmsSetUnRead2ReadSync);

    SUL_ZeroMemory8(&sOutParam,SIZEOF(CFW_SMS_OUT_PARAM));
    if(nStorage != CFW_SMS_STORAGE_ME){
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2ReadSync);
      return ERR_CFW_INVALID_PARAMETER;
      }
    if(nStorage == CFW_SMS_STORAGE_SM)
    {
        CFW_SMS_STORAGE_INFO  sStorageInfo;
        SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
        //Get max slot in SIM
        CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM);
        if((nIndex > sStorageInfo.totalSlot) || (nIndex <= 0)){
        	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2ReadSync); 
          return ERR_CMS_INVALID_MEMORY_INDEX;
          }
    }
    if(nStorage == CFW_SMS_STORAGE_ME)
    {
        if((nIndex > SMS_MAX_ENTRY_IN_ME) || (nIndex <= 0)){
        	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2ReadSync); 
          return ERR_CMS_INVALID_MEMORY_INDEX;
          }
    }
    //Malloc for Handle
    pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
    if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsSetUnRead2ReadSync pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2ReadSync);
		return ERR_CMS_MEMORY_FULL;
      }
    SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));
    
    if(nStorage == CFW_SMS_STORAGE_AUTO)
    {
        UINT8  nSmsStorageId=0x0;
        //Get SMS StorageID
        ret = CFW_CfgGetSmsStorage(&nSmsStorageId);
        if (ret == ERR_SUCCESS)
        {
            if((nSmsStorageId != CFW_SMS_STORAGE_ME)  && \
               (nSmsStorageId != CFW_SMS_STORAGE_SM))
            {
                CSW_SMS_FREE(pSmsMOInfo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2ReadSync);
                return ERR_CFW_INVALID_PARAMETER;
            }
            pSmsMOInfo->nLocation = nSmsStorageId; 
        }
        else
        {
            CSW_SMS_FREE(pSmsMOInfo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2ReadSync);
            return ERR_CFW_INVALID_PARAMETER;
        }
    }
    else
        pSmsMOInfo->nLocation = nStorage; 
    
    //Set value to the handle
    pSmsMOInfo->nIndex   = nIndex;

    //Sync Api
    sms_Sync_SetUnRead2Read(pSmsMOInfo, &sOutParam);
    if(sOutParam.nType == 0xF0){
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2ReadSync);
      return sOutParam.nParam1;
      }
    else{
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsSetUnRead2ReadSync);
      return ERR_SUCCESS;
      }
}

PUBLIC UINT32 SRVAPI CFW_SmsCopyMessages(UINT8 nStatus, UINT16 nCount, UINT16 nStartIndex, UINT8 nOption, UINT16 nUTI)
{
	CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
	UINT32            ret=0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsCopyMessages);

	//Verify input parameters
	ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);
	if(ret != ERR_SUCCESS)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsCopyMessages);
		return ret;
	}
	if(nStartIndex <= 0)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsCopyMessages);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(nStatus == CFW_SMS_STORED_STATUS_STORED_ALL)
		nStatus = 0x7f;
	if((nOption != CFW_SMS_COPY_ME2SM) && (nOption != CFW_SMS_COPY_SM2ME))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsCopyMessages);
		return ERR_CFW_INVALID_PARAMETER;
	}

	//Malloc for Handle
	pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
	if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsCopyMessages pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsCopyMessages);
		return ERR_CMS_MEMORY_FULL;
	}
	SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));

	//Set value to the handle
	pSmsMOInfo->i               = 0x00;
	pSmsMOInfo->nListCount      = nCount;
	pSmsMOInfo->nStatus         = nStatus;
	pSmsMOInfo->nListStartIndex = nStartIndex;
	pSmsMOInfo->nListOption     = nOption;
	pSmsMOInfo->nUTI_UserInput  = nUTI;
	pSmsMOInfo->nUTI_Internal   = (UINT16)(CFW_APP_UTI_MIN + nUTI); 

	//
	//add for (listcount == 0x00)
	//
	if( 0x00 == pSmsMOInfo->nListCount )
	{
		CFW_SMS_STORAGE_INFO nStorageInfo;
		if( nOption == 0x00 )
		{
			CFW_CfgGetSmsStorageInfo(&nStorageInfo,CFW_SMS_STORAGE_ME);
		}
		else
		{
			CFW_CfgGetSmsStorageInfo(&nStorageInfo,CFW_SMS_STORAGE_SM);
		}

		if(nStatus & CFW_SMS_STORED_STATUS_UNREAD)
		{
			pSmsMOInfo->nListCount +=  nStorageInfo.unReadRecords;
		}
		//[[xueww[mod] 2007.01.30 fix bug 3843, if else-->if
		if(nStatus & CFW_SMS_STORED_STATUS_READ)
		{
			pSmsMOInfo->nListCount +=  nStorageInfo.readRecords;
		}
		if(nStatus & CFW_SMS_STORED_STATUS_UNSENT)
		{
			pSmsMOInfo->nListCount +=  nStorageInfo.unsentRecords;
		}
		//
		//这里有隐患；因为DOC和storageinfo有冲突
		//
		if((nStatus & CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)
			||(nStatus & CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV)
			||(nStatus & CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE)
			||(nStatus & CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE))
		{
			pSmsMOInfo->nListCount +=  nStorageInfo.sentRecords;
		} 
		//]]xueww[mod] 2007.01.30
	}
	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_CopyAoProc);
	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);

	CSW_TRACE(CFW_SMS_TS_ID, "-CFW_SmsCopyMessages, nStatus is %d,  nCount is %d,  nStartIndex is %d, nOption is %d\n ",nStatus,nCount,nStartIndex,nOption );

	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsCopyMessages);
	return ERR_SUCCESS;
}

PUBLIC UINT32 SRVAPI CFW_SmsReadMessage(UINT16 nStorage,UINT16 nIndex, UINT16 nUTI)
{
	CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
	UINT32            ret=0x0;
	UINT8             nFormat=0x0,nShow=0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsReadMessage);

	//Verify input parameters
	ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);
	if(ret != ERR_SUCCESS)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ret;
	}
	if((nStorage != CFW_SMS_STORAGE_ME)     && \
		(nStorage != CFW_SMS_STORAGE_SM)     && \
		(nStorage != CFW_SMS_STORAGE_AUTO))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(nStorage == CFW_SMS_STORAGE_SM)
	{
		CFW_SMS_STORAGE_INFO  sStorageInfo;
		SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
		//Get max slot in SIM
		CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM);
		if((nIndex > sStorageInfo.totalSlot ) || (nIndex <= 0))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage); 
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}
	if(nStorage == CFW_SMS_STORAGE_ME)
	{
		if((nIndex > SMS_MAX_ENTRY_IN_ME) || (nIndex <= 0))
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage); 
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}

	//Malloc for Handle
	pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
	if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsReadMessage pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
		return ERR_CMS_MEMORY_FULL;
	}
	SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));

	if(nStorage == CFW_SMS_STORAGE_AUTO)
	{
		UINT8  nSmsStorageId=0x0;
		//Get SMS StorageID
		ret = CFW_CfgGetSmsStorage(&nSmsStorageId);
		if (ret == ERR_SUCCESS)
		{
			if((nSmsStorageId != CFW_SMS_STORAGE_ME)  && \
				(nSmsStorageId != CFW_SMS_STORAGE_SM))
			{
				CSW_SMS_FREE(pSmsMOInfo);
				CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
				return ERR_CFW_INVALID_PARAMETER;
			}
			pSmsMOInfo->nLocation = nSmsStorageId; 
		}
		else
		{
			CSW_SMS_FREE(pSmsMOInfo);
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
			return ERR_CFW_INVALID_PARAMETER;
		}
	}
	else
		pSmsMOInfo->nLocation = nStorage; 

	//Set value to the handle
	pSmsMOInfo->nIndex         = nIndex;
	pSmsMOInfo->nUTI_UserInput = nUTI;
	pSmsMOInfo->nUTI_Internal  = (UINT16)(CFW_APP_UTI_MIN + nUTI); 

	CFW_CfgGetSmsFormat(&nFormat);
	CFW_CfgGetSmsShowTextModeParam(&nShow);
	pSmsMOInfo->nFormat  = nFormat; 
	pSmsMOInfo->nPath    = nShow;    //use nPath to save nShow,hh

	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_ReadAoProc);
	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);
	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsReadMessage, nStorage is %d,  nIndex is %d \n ",nStorage,nIndex);

	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessage);
	return ERR_SUCCESS;
}

PUBLIC UINT32 SRVAPI CFW_SmsReadMessageSync(UINT16 nStorage,UINT16 nIndex, CFW_SMS_OUT_PARAM* pOutParam)
{
    
    CFW_SMS_MO_INFO* pSmsMOInfo = NULL;
    UINT32           ret=0x0;
    UINT8            nFormat=0x0,nShow=0x0;
    CSW_PROFILE_FUNCTION_ENTER(CFW_SmsReadMessageSync);
                            
    if(nStorage != CFW_SMS_STORAGE_ME){
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessageSync);  
      return ERR_CFW_INVALID_PARAMETER;
      }
    if(nStorage == CFW_SMS_STORAGE_SM)
    {
        CFW_SMS_STORAGE_INFO  sStorageInfo;
        SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
        //Get max slot in SIM
        CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM);
        if((nIndex > sStorageInfo.totalSlot ) || (nIndex <= 0)){
        	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessageSync); 
          return ERR_CMS_INVALID_MEMORY_INDEX;
          }
    }
    if(nStorage == CFW_SMS_STORAGE_ME)
    {
        if((nIndex > SMS_MAX_ENTRY_IN_ME) || (nIndex <= 0)){
        	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessageSync); 
          return ERR_CMS_INVALID_MEMORY_INDEX;
          }
    }
    //Malloc for Handle
    pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
    if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsReadMessageSync pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessageSync);
		return ERR_CMS_MEMORY_FULL;
      }
    SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));
    if(nStorage == CFW_SMS_STORAGE_AUTO)
    {
        UINT8  nSmsStorageId=0x0;
        //Get SMS StorageID
        ret = CFW_CfgGetSmsStorage(&nSmsStorageId);
        if (ret == ERR_SUCCESS)
        {
            if((nSmsStorageId != CFW_SMS_STORAGE_ME)  && \
               (nSmsStorageId != CFW_SMS_STORAGE_SM))
            {
                CSW_SMS_FREE(pSmsMOInfo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessageSync);
                return ERR_CFW_INVALID_PARAMETER;
            }
            pSmsMOInfo->nLocation = nSmsStorageId; 
        }
        else
        {
            CSW_SMS_FREE(pSmsMOInfo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessageSync);
            return ERR_CFW_INVALID_PARAMETER;
        }
    }
    else
        pSmsMOInfo->nLocation = nStorage; 
    //Set value to the handle
    pSmsMOInfo->nIndex = nIndex;

    CFW_CfgGetSmsFormat(&nFormat);
    CFW_CfgGetSmsShowTextModeParam(&nShow);
    pSmsMOInfo->nFormat  = nFormat; 
    pSmsMOInfo->nPath    = nShow;    //use nPath to save nShow,hh

    //Sync Api
    sms_Sync_Read(pSmsMOInfo, pOutParam);
    if(pOutParam->nType == 0xF0){
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessageSync);
      return pOutParam->nParam1;
      }
    else{
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsReadMessageSync);
      return ERR_SUCCESS;
      }
}


PUBLIC UINT32 SRVAPI CFW_SmsWriteMessage(CFW_DIALNUMBER* pNumber, UINT8* pData,  UINT16 nDataSize, UINT16 nStorage, UINT16 nIndex, UINT8 nStatus,  UINT16 nUTI)
{
	CFW_SMS_MO_INFO*   pSmsMOInfo = NULL;
	CFW_DIALNUMBER     sNumber;
	UINT8              nFormat=0x00;
	UINT32             ret=0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsWriteMessage);

	SUL_ZeroMemory8(&sNumber, SIZEOF(CFW_DIALNUMBER));

	//Verify input parameters
	ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);

	if(ret != ERR_SUCCESS)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
		return ret;
	}

	//xueww[-] 2007.04.02 suport save NLL msg
	/*
	//[[xueww 2007/01/09[+] add protection of NULL msg
	if(nDataSize == 0)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}
	//]]xueww 2007/01/09[+]
	*/

	//added by fengwei 20080702 beging
	//at this moment we don't support SR and BM
	if((nStorage == CFW_SMS_STORAGE_BM)   ||
		(nStorage == CFW_SMS_STORAGE_SM_SR)  ||
		(nStorage == CFW_SMS_STORAGE_ME_SR)   ||
		(nStorage == CFW_SMS_STORAGE_SM_ME))
	{

			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage); 
		return ERR_CFW_INVALID_PARAMETER;
	}
	//added by fengwei 20080702 end.

	if(nStorage == CFW_SMS_STORAGE_SM)
	{
		CFW_SMS_STORAGE_INFO  sStorageInfo;
		SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
		//Get max slot in SIM
		CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM);
		if(nIndex > sStorageInfo.totalSlot )
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage); 
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}
	if(nStorage == CFW_SMS_STORAGE_ME)
	{
		if(nIndex > SMS_MAX_ENTRY_IN_ME)
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
			return ERR_CMS_INVALID_MEMORY_INDEX;
		}
	}
	if((nStorage != CFW_SMS_STORAGE_ME)   && \
		(nStorage != CFW_SMS_STORAGE_SM)   && \
		(nStorage != CFW_SMS_STORAGE_BM)   && \
		(nStorage != CFW_SMS_STORAGE_SM_SR)   && \
		(nStorage != CFW_SMS_STORAGE_ME_SR)   && \
		(nStorage != CFW_SMS_STORAGE_SM_ME)&& \
		(nStorage != CFW_SMS_STORAGE_AUTO))
	{

		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if((nStatus != CFW_SMS_STORED_STATUS_UNREAD)                && \
		(nStatus != CFW_SMS_STORED_STATUS_READ)                  && \
		(nStatus != CFW_SMS_STORED_STATUS_UNSENT)                && \
		(nStatus != CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)       && \
		(nStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV)  && \
		(nStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE) && \
		(nStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
		return ERR_CFW_INVALID_PARAMETER;
	}

	//Get SMS format from user setting
	CFW_CfgGetSmsFormat(&nFormat);
	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsWriteMessage nFormat= 0x%x \n ", nFormat);
	if(nFormat)//Text
	{
		UINT8 sTmpNumber[2*TEL_NUMBER_MAX_LEN], uTmpNumberSize;//added by fengwei 20080709

		SUL_ZeroMemory8(&sTmpNumber, 2*TEL_NUMBER_MAX_LEN);
		//Check the user input data, do more later
		if(!pData) 
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
			return ERR_CFW_INVALID_PARAMETER ;
		}
		if(!pNumber)
			pNumber = &sNumber;

		if((pNumber->nClir!=0)&&(pNumber->nClir!=1)&&(pNumber->nClir!=2))
		{
			//default value
			pNumber->nClir = 0; 
		}
		if((pNumber->nType!=CFW_TELNUMBER_TYPE_INTERNATIONAL)&&  \
			(pNumber->nType!=CFW_TELNUMBER_TYPE_NATIONAL)&&      \
			(pNumber->nType!=CFW_TELNUMBER_TYPE_UNKNOWN))
		{
			//default value
			pNumber->nType = CFW_TELNUMBER_TYPE_UNKNOWN;
		}
		if(pNumber->nDialNumberSize > TEL_NUMBER_MAX_LEN )
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
			return ERR_CFW_INVALID_PARAMETER ;
		}

		//[[xueww[mod] 2007.07.27 sms_tool_BCD2ASCII--->SUL_GsmBcdToAscii
		//sms_tool_BCD2ASCII(sTmpNumber, pNumber->pDialNumber, pNumber->nDialNumberSize);
		SUL_GsmBcdToAscii(pNumber->pDialNumber, pNumber->nDialNumberSize, sTmpNumber);
		//]]xueww[mod] 2007.07.27
		if(0xf0 == (*(pNumber->pDialNumber+pNumber->nDialNumberSize-1) & 0xf0))
			uTmpNumberSize = (UINT8)(pNumber->nDialNumberSize*2 - 1);
		else
			uTmpNumberSize = (UINT8)(pNumber->nDialNumberSize*2);

		//Malloc for Handle
		pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO)+uTmpNumberSize+nDataSize);
		if(!pSmsMOInfo)
		{
			CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsWriteMessage pSmsMOInfo malloc error!!! \n ");
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
			return ERR_CMS_MEMORY_FULL;
		}
		SUL_ZeroMemory8(pSmsMOInfo, (SIZEOF(CFW_SMS_MO_INFO)+uTmpNumberSize+nDataSize));//modified by fengwei 20080710 for bug 8698

		//Set user input data to handle
		pSmsMOInfo->sNumber.nClir           = pNumber->nClir;
		pSmsMOInfo->sNumber.nType           = pNumber->nType;
		pSmsMOInfo->sNumber.nDialNumberSize = uTmpNumberSize;
		pSmsMOInfo->sNumber.pDialNumber     = (UINT8*)((UINT8*)pSmsMOInfo + SIZEOF(CFW_SMS_MO_INFO));
		SUL_MemCopy8((UINT8*)pSmsMOInfo->sNumber.pDialNumber,&sTmpNumber,uTmpNumberSize);

		pSmsMOInfo->nDataSize = nDataSize;
		//[[xueww[mod]2007.04.02
		if(nDataSize)
			pSmsMOInfo->pData = (UINT8*)((UINT8*)pSmsMOInfo + SIZEOF(CFW_SMS_MO_INFO) + uTmpNumberSize);
		else
			pSmsMOInfo->pData = NULL;
		//]]xueww[mod]2007.04.02

		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsWriteMessage, write a text sms,nDataSize is %d, nStorage is %d , nIndex is %d , nStatus is %d\n ",nDataSize,nStorage,nIndex,nStatus);
	}  // Write a PDU
	else
	{
		//Malloc for Handle
		pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO)+nDataSize);
		if(!pSmsMOInfo)
		{
			CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsWriteMessage pSmsMOInfo malloc error!!! \n ");
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
			return ERR_CMS_MEMORY_FULL;
		}
		SUL_ZeroMemory8(pSmsMOInfo, (SIZEOF(CFW_SMS_MO_INFO)+nDataSize));

		pSmsMOInfo->nDataSize = nDataSize;
		pSmsMOInfo->pData = (UINT8*)((UINT8*)pSmsMOInfo + SIZEOF(CFW_SMS_MO_INFO));;

		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsWriteMessage, write a PDU sms,nDataSize is %d, nStorage is %d , nIndex is %d , nStatus is %d\n ",nDataSize,nStorage,nIndex,nStatus);
	}

	SUL_MemCopy8(pSmsMOInfo->pData,pData,nDataSize);

	//Verify Storage
	if(nStorage == CFW_SMS_STORAGE_AUTO)
	{
		UINT8  nSmsStorageId=0x0;
		//Get SMS StorageID
		ret = CFW_CfgGetSmsStorage(&nSmsStorageId);
		if (ret == ERR_SUCCESS)
			pSmsMOInfo->nLocation = nSmsStorageId; 
		else
		{
			CSW_SMS_FREE(pSmsMOInfo);
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
			return ERR_CFW_INVALID_PARAMETER;
		}
	}
	else
		pSmsMOInfo->nLocation = nStorage; 

	pSmsMOInfo->nFormat = nFormat;
	pSmsMOInfo->nIndex  = nIndex;
	pSmsMOInfo->nStatus = nStatus;
	pSmsMOInfo->nUTI_UserInput = nUTI;
	pSmsMOInfo->nUTI_Internal  = (UINT16)(CFW_APP_UTI_MIN + nUTI); 

	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_WriteAoProc);

	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);
	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
	return ERR_SUCCESS;
}

PUBLIC UINT32 SRVAPI CFW_SmsWriteMessageSync(CFW_DIALNUMBER* pNumber, UINT8* pData,  UINT16 nDataSize, UINT16 nStorage, UINT16 nIndex, UINT8 nStatus, CFW_SMS_OUT_PARAM* pOutParam)
{
    CFW_SMS_MO_INFO*   pSmsMOInfo = NULL;
    CFW_DIALNUMBER     sNumber;
    UINT8              nFormat=0x00;
    UINT32             ret=0x0;
    CSW_PROFILE_FUNCTION_ENTER(CFW_SmsWriteMessageSync);
    
    SUL_ZeroMemory8(&sNumber, SIZEOF(CFW_DIALNUMBER));
    //Verify input parameters
    if(nStorage != CFW_SMS_STORAGE_ME){
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessageSync);
      return ERR_CFW_INVALID_PARAMETER;
      }
    if(nStorage == CFW_SMS_STORAGE_SM)
    {
        CFW_SMS_STORAGE_INFO  sStorageInfo;
        SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
        //Get max slot in SIM
        CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM);
        if(nIndex > sStorageInfo.totalSlot ){
        	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessageSync); 
          return ERR_CMS_INVALID_MEMORY_INDEX;
          }
    }
    if(nStorage == CFW_SMS_STORAGE_ME)
    {
        if(nIndex > SMS_MAX_ENTRY_IN_ME){
        	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessageSync);
          return ERR_CMS_INVALID_MEMORY_INDEX;
          }
    }
    if((nStatus != CFW_SMS_STORED_STATUS_UNREAD)                && \
       (nStatus != CFW_SMS_STORED_STATUS_READ)                  && \
       (nStatus != CFW_SMS_STORED_STATUS_UNSENT)                && \
       (nStatus != CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)       && \
       (nStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV)  && \
       (nStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE) && \
       (nStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE))
    {
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessageSync);
        return ERR_CFW_INVALID_PARAMETER;
    }
    //Get SMS format from user setting
    CFW_CfgGetSmsFormat(&nFormat);
    CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsWriteMessageSync nFormat=0x%x\n ", nFormat);
    if(nFormat)//Text
    {
        UINT8 sTmpNumber[2*TEL_NUMBER_MAX_LEN];
        
        SUL_ZeroMemory8(&sTmpNumber, 2*TEL_NUMBER_MAX_LEN);
        //Check the user input data, do more later
        if(!pData) 
        {
        	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessageSync);
            return ERR_CFW_INVALID_PARAMETER ;
        }
        if(!pNumber)
            pNumber = &sNumber;

        if((pNumber->nClir!=0)&&(pNumber->nClir!=1)&&(pNumber->nClir!=2))
        {
            //default value
            pNumber->nClir = 0; 
        }
        if((pNumber->nType!=CFW_TELNUMBER_TYPE_INTERNATIONAL)&&  \
           (pNumber->nType!=CFW_TELNUMBER_TYPE_NATIONAL)&&      \
           (pNumber->nType!=CFW_TELNUMBER_TYPE_UNKNOWN))
        {
            //default value
            pNumber->nType = CFW_TELNUMBER_TYPE_UNKNOWN;
        }
        if(pNumber->nDialNumberSize > TEL_NUMBER_MAX_LEN ){
        	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessageSync);
          return ERR_CFW_INVALID_PARAMETER ;
          }

        sms_tool_BCD2ASCII(sTmpNumber, pNumber->pDialNumber, pNumber->nDialNumberSize);
        if(0xf0 == (*(pNumber->pDialNumber+pNumber->nDialNumberSize-1) & 0xf0))
            pNumber->nDialNumberSize = (UINT8)(pNumber->nDialNumberSize*2 - 1);
        else
            pNumber->nDialNumberSize = (UINT8)(pNumber->nDialNumberSize*2);

        //Malloc for Handle
        pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO)+pNumber->nDialNumberSize+nDataSize);
        if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsWriteMessageSync pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessageSync);
		return ERR_CMS_MEMORY_FULL;
	}
        SUL_ZeroMemory8(pSmsMOInfo, (SIZEOF(CFW_SMS_MO_INFO)+pNumber->nDialNumberSize+nDataSize));
    
        //Set user input data to handle
        pSmsMOInfo->sNumber.nClir           = pNumber->nClir;
        pSmsMOInfo->sNumber.nType           = pNumber->nType;
        pSmsMOInfo->sNumber.nDialNumberSize = pNumber->nDialNumberSize;
        pSmsMOInfo->sNumber.pDialNumber     = (UINT8*)((UINT8*)pSmsMOInfo + SIZEOF(CFW_SMS_MO_INFO));
        SUL_MemCopy8((UINT8*)pSmsMOInfo->sNumber.pDialNumber,&sTmpNumber,pNumber->nDialNumberSize);
    
        pSmsMOInfo->nDataSize = nDataSize;
        pSmsMOInfo->pData = (UINT8*)((UINT8*)pSmsMOInfo + SIZEOF(CFW_SMS_MO_INFO) + pNumber->nDialNumberSize);;
    }  // Write a PDU
    else
    {
        //Malloc for Handle
        pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO)+nDataSize);
        if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsWriteMessageSync pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessageSync);
		return ERR_CMS_MEMORY_FULL;
      }
        SUL_ZeroMemory8(pSmsMOInfo, (SIZEOF(CFW_SMS_MO_INFO)+nDataSize));
   
        pSmsMOInfo->nDataSize = nDataSize;
        pSmsMOInfo->pData = (UINT8*)((UINT8*)pSmsMOInfo + SIZEOF(CFW_SMS_MO_INFO));;

    }
    SUL_MemCopy8(pSmsMOInfo->pData,pData,nDataSize);
    //Verify Storage
    if(nStorage == CFW_SMS_STORAGE_AUTO)
    {
        UINT8  nSmsStorageId=0x0;
        //Get SMS StorageID
        ret = CFW_CfgGetSmsStorage(&nSmsStorageId);
        if (ret == ERR_SUCCESS)
            pSmsMOInfo->nLocation = nSmsStorageId; 
        else
        {
            CSW_SMS_FREE(pSmsMOInfo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessageSync);
            return ERR_CFW_INVALID_PARAMETER;
        }
    }
    else
        pSmsMOInfo->nLocation = nStorage; 
    
    pSmsMOInfo->nFormat = nFormat;
    pSmsMOInfo->nIndex  = nIndex;
    pSmsMOInfo->nStatus = nStatus;

    //Sync Api
    sms_Sync_Write(pSmsMOInfo, pOutParam);
    if(pOutParam->nType == 0xF0){
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessageSync);
      return pOutParam->nParam1;
      }
    else{
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessageSync);
      return ERR_SUCCESS;
      }
}

PUBLIC UINT32 SRVAPI CFW_SmsListMessages(UINT16 nStorage,UINT8 nStatus, UINT16 nCount ,UINT16 nStartIndex,  UINT8 nOption,  UINT16 nUTI)
{
	CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
	UINT32            ret=0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsListMessages);

	//Verify input parameters
	ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);
	if(ret != ERR_SUCCESS)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages);
		return ret;
	}
	if((nStorage != CFW_SMS_STORAGE_ME)     && \
		(nStorage != CFW_SMS_STORAGE_SM)     && \
		(nStorage != CFW_SMS_STORAGE_AUTO))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(nStartIndex <= 0)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(nStorage == CFW_SMS_STORAGE_SM)
	{
		CFW_SMS_STORAGE_INFO  sStorageInfo;
		SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
		//Get max slot in SIM
		CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM);
		if(nStartIndex > sStorageInfo.totalSlot )
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages); 
			return ERR_CFW_INVALID_PARAMETER ;
		}
	}
	if(nStorage == CFW_SMS_STORAGE_ME)
	{
		if(nStartIndex > SMS_MAX_ENTRY_IN_ME)
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages);
			CSW_TRACE(CFW_SMS_TS_ID, "The SMS_MAX_ENTRY_IN_ME is %d \n",SMS_MAX_ENTRY_IN_ME);
			return ERR_CFW_INVALID_PARAMETER ;
		}
	}
	if(nStatus == 0xff)
		nStatus = CFW_SMS_STORED_STATUS_UNREAD;
	if(nStatus == CFW_SMS_STORED_STATUS_STORED_ALL)
		nStatus = 0x7f;
	if(!(nOption & CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(!(nOption & CFW_SMS_MULTIPART_OPTION_ALL) && !(nOption & CFW_SMS_MULTIPART_OPTION_FIRST))
		nOption = (UINT8)(nOption & CFW_SMS_MULTIPART_OPTION_ALL);

	//Malloc for Handle
	pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
	if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsListMessages pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages);
		return ERR_CMS_MEMORY_FULL;
	}
	SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));

	//Verify Storage
	if(nStorage == CFW_SMS_STORAGE_AUTO)
	{
		UINT8  nSmsStorageId=0x0;
		//Get SMS StorageID
		ret = CFW_CfgGetSmsStorage(&nSmsStorageId);
		if (ret == ERR_SUCCESS)
		{
			if((nSmsStorageId != CFW_SMS_STORAGE_ME)  && \
				(nSmsStorageId != CFW_SMS_STORAGE_SM))
			{
				CSW_SMS_FREE(pSmsMOInfo);
				CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages);
				return ERR_CFW_INVALID_PARAMETER;
			}
			pSmsMOInfo->nLocation = nSmsStorageId; 
		}
		else
		{
			CSW_SMS_FREE(pSmsMOInfo);
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages);
			return ERR_CFW_INVALID_PARAMETER;
		}
	}
	else
		pSmsMOInfo->nLocation = nStorage; 

	//Set value to the handle
	pSmsMOInfo->i               = 0x00;
	pSmsMOInfo->nListCount      = nCount;
	pSmsMOInfo->nStatus         = nStatus;
	pSmsMOInfo->nListStartIndex = nStartIndex;
	pSmsMOInfo->nListOption     = nOption;
	pSmsMOInfo->nUTI_UserInput  = nUTI;
	pSmsMOInfo->nUTI_Internal   = (UINT16)(CFW_APP_UTI_MIN + nUTI); 

	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_ListAoProc);
	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);

	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsListMessages ,nStorage is %d, nStatus is %d , nCount is %d , nStartIndex is, nOption is %d\n ",nStorage,nStatus,nCount,nStartIndex,nOption);

	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessages);
	return ERR_SUCCESS;
}

PUBLIC UINT32 SRVAPI CFW_SmsListMessagesSync(UINT16 nStorage,UINT8 nStatus, UINT16 nCount ,UINT16 nStartIndex,  UINT8 nOption,  CFW_SMS_OUT_PARAM* pOutParam)
{
    CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
    UINT32            ret=0x0;
    CSW_PROFILE_FUNCTION_ENTER(CFW_SmsListMessagesSync);
    
    if(nStorage != CFW_SMS_STORAGE_ME){
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesSync); 
      return ERR_CFW_INVALID_PARAMETER;
      }
    if(nStartIndex <= 0){
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesSync);
      return ERR_CFW_INVALID_PARAMETER;
      }
    if(nStorage == CFW_SMS_STORAGE_SM)
    {
        CFW_SMS_STORAGE_INFO  sStorageInfo;
        SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
        //Get max slot in SIM
        CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM);
        if(nStartIndex > sStorageInfo.totalSlot ){
        	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesSync); 
          return ERR_CFW_INVALID_PARAMETER ;
          }
    }
    if(nStorage == CFW_SMS_STORAGE_ME)
    {
        if(nStartIndex > SMS_MAX_ENTRY_IN_ME){
        	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesSync);
          return ERR_CFW_INVALID_PARAMETER ;
          }
    }
    if(nStatus == CFW_SMS_STORED_STATUS_STORED_ALL)
        nStatus = 0x7f;
    if(!(nOption & CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER)){
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesSync);
      return ERR_CFW_INVALID_PARAMETER;
      }
    if(!(nOption & CFW_SMS_MULTIPART_OPTION_ALL) && !(nOption & CFW_SMS_MULTIPART_OPTION_FIRST))
        nOption = (UINT8)(nOption & CFW_SMS_MULTIPART_OPTION_ALL);
    //Malloc for Handle
    pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
    if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsListMessagesSync pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesSync);
		return ERR_CMS_MEMORY_FULL;
      }
    SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));
    //Verify Storage
    if(nStorage == CFW_SMS_STORAGE_AUTO)
    {
        UINT8  nSmsStorageId=0x0;
        //Get SMS StorageID
        ret = CFW_CfgGetSmsStorage(&nSmsStorageId);
        if (ret == ERR_SUCCESS)
        {
            if((nSmsStorageId != CFW_SMS_STORAGE_ME)  && \
               (nSmsStorageId != CFW_SMS_STORAGE_SM))
            {
                CSW_SMS_FREE(pSmsMOInfo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesSync);
                return ERR_CFW_INVALID_PARAMETER;
            }
            pSmsMOInfo->nLocation = nSmsStorageId; 
        }
        else
        {
            CSW_SMS_FREE(pSmsMOInfo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesSync);
            return ERR_CFW_INVALID_PARAMETER;
        }
    }
    else
        pSmsMOInfo->nLocation = nStorage; 

    //Set value to the handle
    pSmsMOInfo->i               = 0x00;
    pSmsMOInfo->nListCount      = nCount;
    pSmsMOInfo->nStatus         = nStatus;
    pSmsMOInfo->nListStartIndex = nStartIndex;
    pSmsMOInfo->nListOption     = nOption;

    //Sync Api
    sms_Sync_List(pSmsMOInfo, pOutParam);
    if(pOutParam->nType == 0xF0){
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesSync);
      return pOutParam->nParam1;
      }
    else{
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesSync);
      return ERR_SUCCESS;
      }
}

PUBLIC UINT32 SRVAPI CFW_SmsListMessagesHeader(UINT16 nStorage,UINT8 nStatus, UINT16 nCount ,UINT16 nStartIndex, UINT8 nOption, UINT16 nUTI)
{
	CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
	UINT32            ret=0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsListMessagesHeader);

	//    return ERR_CFW_INVALID_PARAMETER;

	//Verify input parameters
	ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);
	if(ret != ERR_SUCCESS)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeader);
		return ret;
	}
	if((nStorage != CFW_SMS_STORAGE_ME)     && \
		(nStorage != CFW_SMS_STORAGE_SM)     && \
		(nStorage != CFW_SMS_STORAGE_AUTO))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeader);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(nStartIndex <= 0)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeader);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(nStorage == CFW_SMS_STORAGE_SM)
	{
		CFW_SMS_STORAGE_INFO  sStorageInfo;
		SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
		//Get max slot in SIM
		CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM);
		if(nStartIndex > sStorageInfo.totalSlot)
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeader); 
			return ERR_CFW_INVALID_PARAMETER ;
		}
	}
	if(nStorage == CFW_SMS_STORAGE_ME)
	{
		if(nStartIndex > SMS_MAX_ENTRY_IN_ME)
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeader);
			return ERR_CFW_INVALID_PARAMETER ;
		}
	}
	if(nStatus == CFW_SMS_STORED_STATUS_STORED_ALL)
		nStatus = 0x7f;

	if(!(nOption & CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeader);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(!(nOption & CFW_SMS_MULTIPART_OPTION_ALL) && !(nOption & CFW_SMS_MULTIPART_OPTION_FIRST))
		nOption = (UINT8)(nOption & CFW_SMS_MULTIPART_OPTION_ALL);

	//Malloc for Handle
	pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
	if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsListMessagesHeader pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeader);
		return ERR_CMS_MEMORY_FULL;
	}
	SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));

	//Verify Storage
	if(nStorage == CFW_SMS_STORAGE_AUTO)
	{
		UINT8  nSmsStorageId=0x0;
		//Get SMS StorageID
		ret = CFW_CfgGetSmsStorage(&nSmsStorageId);
		if (ret == ERR_SUCCESS)
		{
			if((nSmsStorageId != CFW_SMS_STORAGE_ME)  && \
				(nSmsStorageId != CFW_SMS_STORAGE_SM))
			{
				CSW_SMS_FREE(pSmsMOInfo);
				CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeader);
				return ERR_CFW_INVALID_PARAMETER;
			}
			pSmsMOInfo->nLocation = nSmsStorageId; 
		}
		else
		{
			CSW_SMS_FREE(pSmsMOInfo);
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeader);
			return ERR_CFW_INVALID_PARAMETER;
		}
	}
	else
		pSmsMOInfo->nLocation = nStorage; 

	//Set value to the handle
	pSmsMOInfo->i               = 0x00;
	pSmsMOInfo->nUTI_UserInput  = nUTI;
	pSmsMOInfo->nListCount      = nCount;
	pSmsMOInfo->nStatus         = nStatus;
	pSmsMOInfo->nListStartIndex = nStartIndex;
	pSmsMOInfo->nListOption     = nOption;
	pSmsMOInfo->nUTI_Internal   = (UINT16)(CFW_APP_UTI_MIN + nUTI); 

	//Register CallBack into system 
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_ListHeaderAoProc);
	CFW_SetUTI(pSmsMOInfo->hAo,nUTI,0);
	CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);

	CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsListMessagesHeader ,nStorage is %d, nStatus is %d , nCount is %d , nStartIndex is, nOption is %d\n ",nStorage,nStatus,nCount,nStartIndex,nOption);

	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeader);
	return ERR_SUCCESS;
}

PUBLIC UINT32 SRVAPI CFW_SmsListMessagesHeaderSync(UINT16 nStorage,UINT8 nStatus, UINT16 nCount ,UINT16 nStartIndex, UINT8 nOption, CFW_SMS_OUT_PARAM* pOutParam)
{
    CFW_SMS_MO_INFO*  pSmsMOInfo = NULL;
    UINT32            ret=0x0;
    CSW_PROFILE_FUNCTION_ENTER(CFW_SmsListMessagesHeaderSync);
    if(nStorage != CFW_SMS_STORAGE_ME){
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeaderSync);  
      return ERR_CFW_INVALID_PARAMETER;
      }
    if(nStartIndex <= 0){
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeaderSync);
      return ERR_CFW_INVALID_PARAMETER;
      }
    if(nStorage == CFW_SMS_STORAGE_SM)
    {
        CFW_SMS_STORAGE_INFO  sStorageInfo;
        SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
        //Get max slot in SIM
        CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM);
        if(nStartIndex > sStorageInfo.totalSlot ){
        	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeaderSync); 
          return ERR_CFW_INVALID_PARAMETER ;
          }
    }
    if(nStorage == CFW_SMS_STORAGE_ME)
    {
        if(nStartIndex > SMS_MAX_ENTRY_IN_ME){
        	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeaderSync);
          return ERR_CFW_INVALID_PARAMETER ;
          }
    }
    if(nStatus == CFW_SMS_STORED_STATUS_STORED_ALL)
        nStatus = 0x7f;

    if(!(nOption & CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER)){
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeaderSync);
      return ERR_CFW_INVALID_PARAMETER;
      }
    if(!(nOption & CFW_SMS_MULTIPART_OPTION_ALL) && !(nOption & CFW_SMS_MULTIPART_OPTION_FIRST))
        nOption = (UINT8)(nOption & CFW_SMS_MULTIPART_OPTION_ALL);

    //Malloc for Handle
    pSmsMOInfo = (CFW_SMS_MO_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MO_INFO));
    if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsListMessagesHeaderSync pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeaderSync);
		return ERR_CMS_MEMORY_FULL;
      }
    SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_MO_INFO));
    
    //Verify Storage
    if(nStorage == CFW_SMS_STORAGE_AUTO)
    {
        UINT8  nSmsStorageId=0x0;
        //Get SMS StorageID
        ret = CFW_CfgGetSmsStorage(&nSmsStorageId);
        if (ret == ERR_SUCCESS)
        {
            if((nSmsStorageId != CFW_SMS_STORAGE_ME)  && \
               (nSmsStorageId != CFW_SMS_STORAGE_SM))
            {
                CSW_SMS_FREE(pSmsMOInfo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeaderSync);
                return ERR_CFW_INVALID_PARAMETER;
            }
            pSmsMOInfo->nLocation = nSmsStorageId; 
        }
        else
        {
            CSW_SMS_FREE(pSmsMOInfo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeaderSync);
            return ERR_CFW_INVALID_PARAMETER;
        }
    }
    else
        pSmsMOInfo->nLocation = nStorage; 

    //Set value to the handle
    pSmsMOInfo->i               = 0x00;
    pSmsMOInfo->nListCount      = nCount;
    pSmsMOInfo->nStatus         = nStatus;
    pSmsMOInfo->nListStartIndex = nStartIndex;
    pSmsMOInfo->nListOption     = nOption;

    //Sync Api
    sms_Sync_ListHeader(pSmsMOInfo, pOutParam);
    if(pOutParam->nType == 0xF0){
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeaderSync);
      return pOutParam->nParam1;
      }
    else{
    	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsListMessagesHeaderSync);
      return ERR_SUCCESS;
      }
}

PUBLIC UINT32 SRVAPI CFW_SmsGetMessageNode(PVOID pListResult, UINT16 nIndex,CFW_SMS_NODE **pNode)
{
	UINT8   nFormat=0x0,nShow=0x0;
	UINT32  sDecomposeOutData=0x0,ret=0x0;
	UINT16  nListAddrCount=0x0;
	CFW_SMS_NODE* pListAddr=NULL;
	CFW_SMS_MULTIPART_INFO sLongerMsg;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsGetMessageNode);

	CFW_CfgGetSmsFormat(&nFormat);
	CFW_CfgGetSmsShowTextModeParam(&nShow);

	//Zero memory
	SUL_ZeroMemory8(&sLongerMsg,SIZEOF(CFW_SMS_MULTIPART_INFO));

	if(!pListResult)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsGetMessageNode);
		return ERR_CFW_INVALID_PARAMETER ;
	}
	if(!pNode)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsGetMessageNode);
		return ERR_CFW_INVALID_PARAMETER ;
	}
	//Set value  
	nListAddrCount = *((UINT16*)pListResult);
	if(nListAddrCount == 0)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsGetMessageNode);
		return ERR_CFW_INVALID_PARAMETER ;
	}
	if((nIndex + 1) > nListAddrCount)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsGetMessageNode);
		return ERR_CFW_INVALID_PARAMETER ;
	}

	pListAddr = (CFW_SMS_NODE*)(*((UINT32*)pListResult + 1 + nIndex));
	if(!pListAddr)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsGetMessageNode);
		return ERR_CFW_INVALID_PARAMETER ;
	}

	ret = sms_mo_Parse_PDU(nFormat,nShow,(UINT8)pListAddr->nStatus,pListAddr->nStorage,pListAddr->pNode,&pListAddr->nType,&sDecomposeOutData,&sLongerMsg);
	if((ret != ERR_SUCCESS) || (sDecomposeOutData == 0))
	{
		CFW_SMS_NODE* pTmpNode=NULL;
		pTmpNode   = (CFW_SMS_NODE *)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO) + SIZEOF(CFW_SMS_NODE));
		if(!pTmpNode)
		{
			CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsGetMessageNode pSmsMOInfo malloc error!!! \n ");
			CSW_SMS_FREE(pListAddr);
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsGetMessageNode);
			return ERR_NO_MORE_MEMORY ;
		}
		SUL_ZeroMemory8(pTmpNode,(SIZEOF(CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO) + SIZEOF(CFW_SMS_NODE)));
		pTmpNode->pNode = (PVOID)((UINT8*)pTmpNode + sizeof(CFW_SMS_NODE));
		sDecomposeOutData = (UINT32)pTmpNode;
	}
	//Set value
	((CFW_SMS_NODE*)sDecomposeOutData)->nConcatPrevIndex     = pListAddr->nConcatPrevIndex;
	((CFW_SMS_NODE*)sDecomposeOutData)->nConcatCurrentIndex  = pListAddr->nConcatCurrentIndex;
	((CFW_SMS_NODE*)sDecomposeOutData)->nConcatNextIndex     = pListAddr->nConcatNextIndex;
	((CFW_SMS_NODE*)sDecomposeOutData)->nStatus  = pListAddr->nStatus;
	((CFW_SMS_NODE*)sDecomposeOutData)->nType    = pListAddr->nType;
	((CFW_SMS_NODE*)sDecomposeOutData)->nStorage = pListAddr->nStorage;
	CSW_SMS_FREE(pListAddr);
	*pNode = (CFW_SMS_NODE*)sDecomposeOutData;
	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsGetMessageNode);
	return ERR_SUCCESS;
}



//The CFW_SmsNewSmsAck function sets New SMS message acknowledge to ME/TE, only phase 2+.
PUBLIC UINT32 SRVAPI CFW_SmsNewSmsAck(UINT8 nCmd,  UINT8* pData,  UINT16 nSize) 
{
	UINT8 nSupportType = 0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsNewSmsAck);

#if 0
	//This function confirms successful receipt of a new message (SMS-DELIVER or SMSSTATUS-REPORT) which is routed directly to the TE. 
	//This function only work when CFW_CfgSelectSmsService(1). 
	//This function is applicable for SMS Text mode. 

	//The first parameter is 1 , it means as following:
	//GSM 03.40 and 03.41 (the syntax of SMS AT commands is compatible with GSM 07.05 Phase 2+ version; 
	//the requirement of <service> setting 1 is mentioned under corresponding command descriptions).
#endif
	CFW_CfgSelectSmsService(1,&nSupportType);
	if(CFW_GET_BIT(nSupportType, 1) == 1)//1 indicate doesn't support MT type
	{
	}
	else //0 indicate doesn't support MT type
	{
	}


	//0 command operates similarly as defined for the text mode. 
	if(nCmd == 0)
	{
	}
	//send RP-ACK (or buffered result code received correctly), only applicable for PDU mode.
	else if(nCmd == 1)
	{
	// sms_mt_SmsPPAckReq();
	}
	//send RP-ERROR (if PDU is not given, ME/TA shall send SMS-DELIVER-REPORT 
	//with 3G TS 23.040 [3] TP-FCS value set to ‘FF’ (unspecified error cause)), 
	//only applicable for PDU mode.
	else if(nCmd == 2)
	{
#if 0
	//Hex	Meanings 
	//81	Short message Type 0 not supported
	//82	Cannot replace short message
	//8F	Unspecified TP-PID error	
	//91	Message class not supported	
	//9F	Unspecified TP-DCS error	
	//B0	TPDU not supported	
	//D0	(U)SIM SMS storage full
	//D1	No SMS storage capability in (U)SIM	
	//D2	Error in MS	
	//D3	Memory Capacity Exceeded	
	//D4	(U)SIM Application Toolkit Busy	
	//D5	(U)SIM data download error	
	//E0 - FE	Values specific to an application
	//FF	Unspecified error cause	
#endif
	sms_mt_SmsPPErrorReq(*pData); //0xff: Unspecified error cause
	}
	else
	{
	}

	//if this parameter is NULL, this parameter is ignored, only applicable for PDU mode. 
	if(!pData)
	{
	}

	//if this parameter is 0, this parameter is ignored, only applicable for PDU mode. 
	if(nSize == 0)
	{
	}

	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsNewSmsAck);
	return ERR_SUCCESS;
	}

//??? Couldn't return *pStatus = 1, need a new condition
PUBLIC UINT32 SRVAPI CFW_SmsQueryOverflowInd(UINT8* pMode, UINT8* pStatus)
{
    UINT16 nEventInd  = 0x0;  
    CSW_PROFILE_FUNCTION_ENTER(CFW_SmsQueryOverflowInd);
            
    //Get the overflow presentation mode
    CFW_CfgGetSmsOverflowInd(pMode);
    CFW_CfgGetIndicatorEvent(&nEventInd);
    if((*pMode == 1) && (nEventInd & CFW_CFG_IND_SMS_RECEIVE_EVENT))
    {
        //SMS buffer full (SIM card or ME) 
        UINT8 ret, uStorageId = 0, uTemp = 0;
	CFW_SMS_STORAGE_INFO  sStorageInfo;
        SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
        ret = CFW_CfgGetNewSmsOption(&uTemp, &uStorageId);//now we support the inquiry of new sms storage. 
	if (ERR_SUCCESS == ret)
	{
            UINT8 nTmp=0x0;

            //Get short message storage
            nTmp = (UINT8)((uStorageId & CFW_SMS_STORAGE_ME)    | (uStorageId & CFW_SMS_STORAGE_SM) | \
                        (uStorageId & CFW_SMS_STORAGE_SM_ME) | (uStorageId & CFW_SMS_STORAGE_MT));
            if(nTmp)
                uStorageId = nTmp;
            else
                uStorageId = CFW_SMS_STORAGE_SM;
	    CFW_CfgGetSmsStorageInfo(&sStorageInfo,uStorageId);
	    if (sStorageInfo.usedSlot == sStorageInfo.totalSlot)
	    {
	         CFW_IND_EVENT_INFO sIndInfo;
		 if (nEventInd & CFW_CFG_IND_SMS_FULL_EVENT)
		 {
		      CFW_CfgGetIndicatorEventInfo(&sIndInfo);
		      if (1 == sIndInfo.sms_receive)
		      {
		            *pStatus = 2;
		      }
		      else
		      {
		           *pStatus = 1;
		      }
		 }
	    }
	    else
	    {
	        *pStatus = 0;
	    }
	}
	else
	{
	    CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsQueryOverflowInd Get Storage Id error!!!\t\n");
	    return ERR_CMS_UNKNOWN_ERROR;
	}
    }
    CSW_PROFILE_FUNCTION_EXIT(CFW_SmsQueryOverflowInd);

    return ERR_SUCCESS;
}


//just show long sms para when using flash api directly

 #endif
// This function is used by the MMI to send a notification to stack that 
// the user has canceled to send SMS.
#ifdef CFW_MULTI_SIM
UINT32 CFW_SmsAbortSendMessage(CFW_SIM_ID nSimID)
#else
UINT32 CFW_SmsAbortSendMessage(void)
#endif
{
    api_SmsPPAbortReq_t * pSmsPPAbortReq = NULL;
    pSmsPPAbortReq = (api_SmsPPAbortReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SmsPPAbortReq_t));
    if(!pSmsPPAbortReq)
    {
      CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsAbortMessage malloc error!!! \n ");
      return ERR_NO_MORE_MEMORY;
    }
    SUL_ZeroMemory8(pSmsPPAbortReq, SIZEOF(api_SmsPPSendMMAReq_t));
    
    //Set Value
    pSmsPPAbortReq->Path = 0; //0:GSM, 1:GPRS
    
    //Send MSG to SCL
#ifndef CFW_MULTI_SIM
    CFW_SendSclMessage(API_SMSPP_ABORT_REQ, pSmsPPAbortReq);
#else
    CFW_SendSclMessage(API_SMSPP_ABORT_REQ, pSmsPPAbortReq, nSimID);
#endif
    return ERR_SUCCESS;

}


#ifdef CFW_MULTI_SIM	
CFW_SMS_INIT_STATUS gNewSMSInd[CFW_SIM_COUNT] = {CFW_SMS_NOINITED,CFW_SMS_NOINITED};
UINT32 CFW_SmsInitComplete(UINT16 nUTI, CFW_SIM_ID nSimID)
#else
CFW_SMS_INIT_STATUS gNewSMSInd = CFW_SMS_NOINITED;
UINT32 CFW_SmsInitComplete(UINT16 nUTI)
#endif
{
	HAO hAo	= 0;
	CFW_SMS_INIT *pSmsInit = NULL;
	UINT32 ret=ERR_SUCCESS;
	
	CSW_TRACE(CFW_SMS_TS_ID, TSTXT("CFW_SmsInitComplete() start\r\n"));	
#ifdef CFW_MULTI_SIM	
	if( nSimID >= CFW_SIM_COUNT )
	{
		CSW_TRACE(CFW_SMS_TS_ID, TSTXT("ERROR ! SIM id error CFW_SmsInitComplete()\r\n"));	
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsInitComplete);
		return ERR_CFW_INVALID_PARAMETER;
	}
#endif		
	pSmsInit = (CFW_SMS_INIT*)CSW_SIM_MALLOC(SIZEOF(CFW_SMS_INIT));
	if(pSmsInit == NULL)
	{
		CSW_TRACE(CFW_SMS_TS_ID, TSTXT("ERROR !CFW_SmsInitComplete() data error\r\n"));	
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsInitComplete);
		return ERR_NO_MORE_MEMORY;
	}
	
	SUL_ZeroMemory32(pSmsInit, SIZEOF(CFW_SMS_INIT));
	pSmsInit->n_CurrStatus		= CFW_SM_STATE_IDLE;
	pSmsInit->n_PrevStatus		= CFW_SM_STATE_IDLE;

#ifdef CFW_MULTI_SIM	
	hAo=CFW_RegisterAo(CFW_APP_SRV_ID, pSmsInit, CFW_SmsInitCompleteProc,nSimID);
#else
	hAo=CFW_RegisterAo(CFW_APP_SRV_ID, pSmsInit, CFW_SmsInitCompleteProc);
#endif

	CFW_SetUTI(hAo, nUTI, 1);
	CFW_SetServiceId(CFW_APP_SRV_ID);
	CFW_SetAoProcCode(hAo,CFW_AO_PROC_CODE_CONTINUE);

	CSW_TRACE(CFW_SMS_TS_ID, TSTXT("CFW_SmsInitComplete() end\r\n"));
	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsInitComplete);
	return ret;  
}
#ifdef CFW_MULTI_SIM	
BOOL g_SMSConcat[CFW_SIM_COUNT] = {FALSE,};
UINT32 CFW_SetSMSConcat (BOOL bConcat, CFW_SIM_ID nSimID)
#else
BOOL g_SMSConcat = {FALSE};
UINT32 CFW_SetSMSConcat (BOOL bConcat)
#endif
{
#ifdef CFW_MULTI_SIM	
	if( nSimID >= CFW_SIM_COUNT )
	{
		CSW_TRACE(CFW_SMS_TS_ID, TSTXT("ERROR ! SIM id error CFW_SendMultSMS()\r\n"));	
		return ERR_CFW_INVALID_PARAMETER;
	}
	g_SMSConcat [nSimID] = bConcat;
	return g_SMSConcat [nSimID];
#else
	g_SMSConcat = bConcat;
	return g_SMSConcat;
#endif 
}
/**********************************************************************************/
/**********************************************************************************/

#endif // ENABLE 

