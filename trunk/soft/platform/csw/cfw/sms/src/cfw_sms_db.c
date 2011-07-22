/******************************************************************************/
/*              Copyright (C) 2005, CII Technologies, Inc.                    */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    cfw_sms_db.c                                                  */
/*                                                                            */
/* Description:                                                               */
/*           SMS DB APIS                                                      */
/*                                                                            */
/* Revision :                                                                 */
/*   1.0.0 : 2006-03-20, Nie YongFeng,nieyf@ciitec.com                        */
/*                                                                            */
/* Remark: Search "???" for more current problems reserved                    */
/*                                                                            */
/******************************************************************************/
#include <sul.h>
#include <ts.h>
#include <cfw.h>
#include <cfw_prv.h>
#include "csw_debug.h"

#include "sms_dm.h"
#include "cfw_sms_db.h"
//=================================================================

INT32 SMS_DM_GetRange(
#ifdef CFW_MULTI_SIM
			CFW_SIM_ID nSimId, 
#endif			
			UINT16*nStartIndex, 
			UINT16*nEndIndex
			);
UINT32  g_cfw_sms_max_num = CFW_SMS_MAX_REC_COUNT; 
//UINT32  g_cfw_sms_max_num = SMS_MAX_NUMBER;

typedef struct
{
   UINT8  nStatus;				
   UINT8  nDate[175];		
   UINT32 nTime;
   UINT32 nAppInt32;
   //UINT8  nPading[CFW_SMS_REC_DATA_SIZE - 184];
} CFW_ME_SMS_PARAM;


//=================================================================
//Create_SMS_DBEX 
//
//=================================================================

INT32 Create_SMS_DBEX()
{
    return ERR_SUCCESS ;
}


//=================================================================
//Remove_SMS_DBEX 
//
//=================================================================

INT32 Remove_SMS_DBEX()
{
    return SMS_DM_Clear();
}


//=================================================================
//SMS_DeleteFromMeByStatusEX 
//
//=================================================================
INT32 SMS_DeleteFromMeByStatusEX ( 
		const UINT8 nStatus,
#ifdef CFW_MULTI_SIM
		 CFW_SIM_ID nSimId
#else
		UINT16 nLocation
#endif
		)
{
  UINT16 nIndexBuf[CFW_SMS_MAX_REC_COUNT];
  UINT16 nSMSNumber = 0x0000;
  UINT16 nTmp       = 0x0000;
  INT32 nRet;

  SUL_ZeroMemory32(nIndexBuf, SIZEOF(nIndexBuf));
#ifndef CFW_MULTI_SIM
  nSMSNumber = SMS_DM_GetIndexByStatus(nIndexBuf, (UINT8)nStatus);
#else
  nSMSNumber = SMS_DM_GetIndexByStatus(nIndexBuf, (UINT8)nStatus, nSimId);
#endif

  for (nTmp = 0x0000; nTmp < nSMSNumber; nTmp++)
  {
    CSW_TRACE(CFW_SMS_TS_ID, "The nIndexBuf[nTmp] is %d\r\n", nIndexBuf[nTmp]);
    if (ERR_SUCCESS != (nRet = SMS_DM_Delete(nIndexBuf[nTmp])))
    {
      CSW_TRACE(CFW_SMS_TS_ID, "The nIndexBuf[nTmp] %d nRet 0x%x\r\n", nIndexBuf[nTmp], nRet);
      return nRet;
    }
  }

  return ERR_SUCCESS;
}


//=================================================================
//SMS_DeleteFromMeByIndexEX 
//
//=================================================================
INT32 SMS_DeleteFromMeByIndexEX(const UINT16 nIndex, UINT16 nLocation)
{
    return( SMS_DM_Delete( nIndex-1));
}


//=================================================================
//SMS_AppendEX 
//
//=================================================================
INT32 SMS_AppendEX(
		UINT8* pData, 
#ifdef CFW_MULTI_SIM
		CFW_SIM_ID nSimId, 
#else	
		UINT16 nLocation, 
#endif
		UINT16 *nIndex,
		UINT32 nTime, 
		UINT32 nAppInt32
		)
{
    //
    //pData 第一个字节是状态
    //
    CFW_ME_SMS_PARAM nME_SMS_PARAM;
    UINT32 nRet;

    
#ifndef CFW_MULTI_SIM
    nLocation = nLocation;
#endif
    nME_SMS_PARAM.nStatus   = *pData;
    nME_SMS_PARAM.nTime     = nTime;
    nME_SMS_PARAM.nAppInt32 = nAppInt32;
    
    SUL_MemCopy8( nME_SMS_PARAM.nDate, &(pData[1]), 175);
#ifndef CFW_MULTI_SIM
    if(ERR_SUCCESS != ( nRet = SMS_DM_Add( nIndex,(PVOID)( &nME_SMS_PARAM))))
#else
    if(ERR_SUCCESS != ( nRet = SMS_DM_Add( nIndex,(PVOID)( &nME_SMS_PARAM), nSimId)))
#endif
    {
	    CSW_TRACE(CFW_SMS_TS_ID,"SMS_AppendEX nret: %d\r\n",nRet);
        return nRet;
    }

    (*nIndex)++;  
    CSW_TRACE(CFW_SMS_TS_ID,"in SMS_AppendEX, output nIndex is %d\n",(*nIndex));

    return ERR_SUCCESS; 	
}				 


//=================================================================
//SMS_WriteEX 
//
//=================================================================

INT32 SMS_WriteEX(UINT16 nIndex, UINT8* pData, UINT16 nLocation, 
				UINT32 nTime, UINT32 nAppInt32)
{
    //
    //pData 第一个字节是状态
    //
    CFW_ME_SMS_PARAM nME_SMS_PARAM;
    nLocation = nLocation;

    SUL_ZeroMemory32( &nME_SMS_PARAM,SIZEOF(CFW_ME_SMS_PARAM));
    
    nME_SMS_PARAM.nStatus    =  *pData;
    nME_SMS_PARAM.nTime      =  nTime;
    nME_SMS_PARAM.nAppInt32  =  nAppInt32;
    
    SUL_MemCopy8( nME_SMS_PARAM.nDate, &(pData[1]), 175);
    return SMS_DM_OverWrite( nIndex-1,(PVOID)(&nME_SMS_PARAM));
}


//=================================================================
//SMS_ReadEX 
//
//=================================================================

INT32 SMS_ReadEX(UINT16 nIndex, UINT8* pData, UINT16 nLocation)
{
    UINT8   nDate[CFW_SMS_REC_DATA_SIZE];
    UINT32  nRet;
    SUL_ZeroMemory8( nDate, SIZEOF(nDate));
    
    if(ERR_SUCCESS != ( nRet = SMS_DM_Read( nIndex-1, nDate)))
    {
        return nRet;
    }
    
    SUL_MemCopy8( pData, nDate, CFW_SMS_REC_DATA_SIZE);

    *pData = SMS_SFUser2SimEX( *pData );
    return ERR_SUCCESS;
   
}


//=================================================================
//SMS_Read2EX 
//
//=================================================================

INT32 SMS_Read2EX(UINT16 nIndex, UINT8* nStatus, UINT8* pData, 
                UINT16 nLocation)
{
    UINT8   Date[CFW_SMS_REC_DATA_SIZE];
    UINT32  nRet;
    SUL_ZeroMemory8( Date, SIZEOF(Date));
    
    if(ERR_SUCCESS != ( nRet = SMS_DM_Read( nIndex-1, Date)))
    {
        return nRet;
    }   
    
    *nStatus = Date[0];
    *nStatus = SMS_SFUser2SimEX( *nStatus );
    SUL_MemCopy8( pData, &(Date[1]), (CFW_SMS_REC_DATA_SIZE - 1));
    return ERR_SUCCESS;
}

#ifndef CFW_MULTI_SIM
//=================================================================
//SMS_GetStorageInfo2EX 
//
//=================================================================

INT32 SMS_GetStorageInfo2EX(UINT16 *pUsedSlot, UINT16 *pMaxSlot)
{
    UINT16  nIndexBuf[CFW_SMS_MAX_REC_COUNT];
    UINT16  nSMSNumber = 0x0000;
    SUL_ZeroMemory32( nIndexBuf, SIZEOF( nIndexBuf));

    nSMSNumber = SMS_DM_GetIndexByStatus( nIndexBuf, 
            (CFW_SMS_STORED_STATUS_UNREAD  \
            | CFW_SMS_STORED_STATUS_READ   \
            | CFW_SMS_STORED_STATUS_UNSENT \
            | CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ \
            | CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV \
            | CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE \
            | CFW_SMS_STORED_STATUS_STORED_ALL));
    
    *pUsedSlot = nSMSNumber;
    *pMaxSlot  = CFW_SMS_MAX_REC_COUNT;
    return ERR_SUCCESS;
}
#endif

//=================================================================
//SMS_AppendEX 
//
//=================================================================

INT32 SMS_GetStorageInfoEX(UINT16 *pUsedSlot, UINT16 *pMaxSlot, 
#ifndef CFW_MULTI_SIM
                            UINT16 nLocation,
#else
				CFW_SIM_ID nSimId,
#endif
                            UINT8 nStatus)
{
    UINT16  nIndexBuf[CFW_SMS_MAX_REC_COUNT];
    UINT16  nSMSNumber = 0x0000;
    SUL_ZeroMemory32( nIndexBuf, SIZEOF(nIndexBuf));
#ifndef CFW_MULTI_SIM
    nSMSNumber = SMS_DM_GetIndexByStatus( nIndexBuf, nStatus);
#else
    nSMSNumber = SMS_DM_GetIndexByStatus( nIndexBuf, nStatus, nSimId);
#endif

    *pUsedSlot = nSMSNumber;
#ifndef CFW_MULTI_SIM
    *pMaxSlot  = CFW_SMS_MAX_REC_COUNT;
#else
    *pMaxSlot  = CFW_SMS_MAX_REC_COUNT/CFW_SIM_COUNT;
#endif
    CSW_TRACE(CFW_SMS_TS_ID,"used=%d, total is %d\r\n",nSMSNumber,CFW_SMS_MAX_REC_COUNT);

    return ERR_SUCCESS;
}


//=================================================================
//SMS_HaveFreeSlotEX 
//
//=================================================================
INT32 SMS_HaveFreeSlotEX(
#ifdef CFW_MULTI_SIM
		CFW_SIM_ID nSimId
#else
		UINT16 nLocation
#endif
)
{
    UINT16 nIndexBuf[CFW_SMS_MAX_REC_COUNT];
    UINT16 nSMSNumber = 0x0000;
    SUL_ZeroMemory32( nIndexBuf, SIZEOF(nIndexBuf));
#ifndef CFW_MULTI_SIM
    nSMSNumber = SMS_DM_GetIndexByStatus( nIndexBuf, (CFW_SMS_STORED_STATUS_UNREAD \
            | CFW_SMS_STORED_STATUS_READ | CFW_SMS_STORED_STATUS_UNSENT  \
            | CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ | CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV \
            | CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE | CFW_SMS_STORED_STATUS_STORED_ALL) );
#else
    nSMSNumber = SMS_DM_GetIndexByStatus( nIndexBuf, 0xff, nSimId);

#endif
    
    if( nSMSNumber != CFW_SMS_MAX_REC_COUNT) 
    {
        return ERR_SUCCESS;
    }
    else
    {
        CSW_TRACE(CFW_SMS_TS_ID,"SMS_HaveFreeSlotEX, error nSMSNumber= 0x%x\n", nSMSNumber);

        return SMS_NO_FREE_SLOT;
    }
}


//=================================================================
//SMS_AppendEX 
//
//=================================================================

UINT32 sms_db_InitEX(void)
{
	CSW_PROFILE_FUNCTION_ENTER( sms_db_Init);    
	CSW_PROFILE_FUNCTION_EXIT( sms_db_Init);

	return ERR_SUCCESS ;//(UINT32)SMS_DM_Init();
}

UINT8 SMS_SFSim2UserEX(UINT8 state)
{

       UINT8 nState;
	CSW_PROFILE_FUNCTION_ENTER(SMS_SFSim2UserEX);
       CSW_TRACE(CFW_SMS_TS_ID,"SMS_SFSim2UserEX, state = 0x%x\n", state);
	switch(state)
	{
		case 0x00: nState = 0x00; break;
		case 0x03: nState = 0x01; break; 
		case 0x01: nState = 0x02; break; 
		case 0x07: nState = 0x04; break; 
		case 0x05: nState = 0x08; break; 
		case 0x0D: nState = 0x10; break; 
		case 0x15: nState = 0x20; break; 
		case 0x1D: nState = 0x40; break; 
		case 0xA0: nState = 0x80; break; 
		default:   nState = 0xff; break;
	}

	CSW_PROFILE_FUNCTION_EXIT(SMS_SFSim2UserEX);
	return nState;
}

UINT8 SMS_SFUser2SimEX(UINT8 state)
{
	UINT8 nState;
	CSW_PROFILE_FUNCTION_ENTER(SMS_SFUser2SimEX);
       CSW_TRACE(CFW_SMS_TS_ID,"SMS_SFUser2SimEX, state = 0x%x\n", state);
	switch(state)
	{
		case 0x00: nState = 0x00; break;
		case 0x01: nState = 0x03; break; 
		case 0x02: nState = 0x01; break; 
		case 0x04: nState = 0x07; break; 
		case 0x08: nState = 0x05; break; 
		case 0x10: nState = 0x0D; break; 
		case 0x20: nState = 0x15; break; 
		case 0x40: nState = 0x1D; break; 
		case 0x80: nState = 0xA0; break; 
		default:   nState = 0xff; break;
	}

	CSW_PROFILE_FUNCTION_EXIT(SMS_SFUser2SimEX);
	return nState;
}





///////////////////////////////////////////////////////////////////////////////
//                         Local ME DB Operation API                         //
///////////////////////////////////////////////////////////////////////////////

// Return success, but invalid index and Flash operation exception.
UINT32 CFW_MeDeleteMessage ( 
#ifndef CFW_MULTI_SIM
									UINT16 nLocation,
#else
									CFW_SIM_ID nSimId,
#endif
									UINT16 nIndex,
									UINT8 nStatus,
									CFW_EV* pEvent
									)
{
	UINT32 err_code;
	CSW_PROFILE_FUNCTION_ENTER(CFW_MeDeleteMessage);

	pEvent->nEventId = EV_CFW_SMS_DELETE_MESSAGE_RSP;
	pEvent->nType = 0;
	pEvent->nParam2 = nIndex;
#ifndef CFW_MULTI_SIM
	if(nLocation!=CFW_SMS_STORAGE_ME 
		&& nLocation!=CFW_SMS_STORAGE_BM
		&& nLocation!=CFW_SMS_STORAGE_ME_SR)
	{
		pEvent->nType = 0xf0;
		pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
		CSW_PROFILE_FUNCTION_EXIT(CFW_MeDeleteMessage);
		return ERR_SUCCESS;
	}
#endif

	if(nIndex > SMS_ME_MAX_INDEX)
	{
		pEvent->nType = 0xf0;
		pEvent->nParam1 = ERR_CME_INVALID_INDEX;
		CSW_PROFILE_FUNCTION_EXIT(CFW_MeDeleteMessage);
		return ERR_SUCCESS;
	}
	else if(nIndex == 0) //Delete the short message(s) pointed by status
	{
#ifndef CFW_MULTI_SIM
        if(ERR_SUCCESS != (err_code=SMS_DeleteFromMeByStatusEX(nStatus, nLocation)))
#else
        if(ERR_SUCCESS != (err_code=SMS_DeleteFromMeByStatusEX(nStatus, nSimId)))
#endif
		{
			if(ERR_DB_NOT_ACCURATE_LOCATE == err_code)
			{
				pEvent->nType = 0x0;
				//pEvent->nParam1 = ERR_CME_ME_FAILURE;
			}
			else
			{
				pEvent->nType = 0xf0;
				pEvent->nParam1 = ERR_CME_ME_FAILURE;
			}
			CSW_PROFILE_FUNCTION_EXIT(CFW_MeDeleteMessage);
			return ERR_SUCCESS;
		}
	}
	else //Delete the short the message pointed by index
	{
	        if(ERR_SUCCESS != (err_code=SMS_DeleteFromMeByIndexEX(nIndex, CFW_SMS_STORAGE_ME)))
		{
		        CSW_TRACE(CFW_SMS_TS_ID,"CFW_MeDeleteMessage, error err_code= 0x%x\n", err_code);
			if(ERR_DB_NOT_ACCURATE_LOCATE == err_code)
			{
				pEvent->nType = 0x0;
				//pEvent->nParam1 = ERR_CME_ME_FAILURE;
			}
			else
			{
				pEvent->nType = 0xf0;
				pEvent->nParam1 = ERR_CME_ME_FAILURE;
			}
			CSW_PROFILE_FUNCTION_EXIT(CFW_MeDeleteMessage);
			return ERR_SUCCESS;
		}
	}

	CSW_PROFILE_FUNCTION_EXIT(CFW_MeDeleteMessage);
	return ERR_SUCCESS;
}

UINT32 CFW_MeWriteMessage ( 
#ifndef CFW_MULTI_SIM
					UINT16 nLocation,
#else
					CFW_SIM_ID nSimId,
#endif
					UINT16 nIndex,
					UINT8* pData,
					UINT8 nDataSize,
					UINT32 nTime,
					UINT32 nAppInt32,
					CFW_EV* pEvent
								   )
{
	UINT32 err_code=0x0;
	UINT16 index=nIndex;
	CSW_PROFILE_FUNCTION_ENTER(CFW_MeWriteMessage);
       CSW_TRACE(CFW_SMS_TS_ID,"CFW_MeWriteMessage nIndex= 0x%x\n", nIndex);
	nDataSize = nDataSize;

	pEvent->nEventId = EV_CFW_SMS_WRITE_MESSAGE_RSP;
	pEvent->nType = 0;
	pEvent->nParam2 = nIndex;
#ifndef CFW_MULTI_SIM
	if(nLocation!=CFW_SMS_STORAGE_ME 
		&& nLocation!=CFW_SMS_STORAGE_BM
		&& nLocation!=CFW_SMS_STORAGE_ME_SR)
	{
		pEvent->nType = 0xf0;
		pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
		CSW_PROFILE_FUNCTION_EXIT(CFW_MeWriteMessage);
		return ERR_SUCCESS;
	}
#endif
	if(nIndex > SMS_ME_MAX_INDEX)
	{
		pEvent->nType = 0xf0;
		pEvent->nParam1 = ERR_CME_INVALID_INDEX;
		CSW_PROFILE_FUNCTION_EXIT(CFW_MeWriteMessage);
		return ERR_SUCCESS;
	}
	else if(nIndex == 0) //Find a free space to save the short message
	{
#ifndef CFW_MULTI_SIM
		if(ERR_SUCCESS != (err_code=SMS_AppendEX(pData, CFW_SMS_STORAGE_SM, &index, nTime, nAppInt32)))
#else
		if(ERR_SUCCESS != (err_code=SMS_AppendEX(pData, nSimId, &index, nTime, nAppInt32)))
#endif			
		{
		       CSW_TRACE(CFW_SMS_TS_ID,"CFW_MeWriteMessage, error err_code= 0x%x\n", err_code);
			if(ERR_SMS_DB_SPACE_NULL == err_code)
			{
				pEvent->nType = 0xf0;
				pEvent->nParam1 = ERR_CME_ME_SPACE_FULL;
			}
			else
			{
				pEvent->nType = 0xf0;
				pEvent->nParam1 = ERR_CME_ME_FAILURE;
			}
			CSW_PROFILE_FUNCTION_EXIT(CFW_MeWriteMessage);
			return ERR_SUCCESS;
		}
#ifndef CFW_MULTI_SIM
		if(ERR_SUCCESS == (err_code = SMS_HaveFreeSlotEX(nLocation)))
#else
		if(ERR_SUCCESS == (err_code = SMS_HaveFreeSlotEX(nSimId)))
#endif
		{
			pEvent->nParam2 = 0;
		}
		else
		{
		       CSW_TRACE(CFW_SMS_TS_ID,"SMS_HaveFreeSlotEX, error err_code= 0x%x\n", err_code);
			pEvent->nParam2 = 1;
		}
	}
	else //Write the short the message pointed by index
	{
 		if(ERR_SUCCESS != SMS_WriteEX(nIndex, pData, CFW_SMS_STORAGE_ME, nTime, nAppInt32))
 		{
			pEvent->nType = 0xf0;
			pEvent->nParam1 = ERR_CME_ME_FAILURE;
			CSW_PROFILE_FUNCTION_EXIT(CFW_MeWriteMessage);
			return ERR_SUCCESS;
		}
#ifndef CFW_MULTI_SIM
		if(ERR_SUCCESS == (err_code = SMS_HaveFreeSlotEX(nLocation)))
#else
		if(ERR_SUCCESS == (err_code = SMS_HaveFreeSlotEX(nSimId)))
#endif
 		{
			pEvent->nParam2 = 0;
		}
		else
		{
		       CSW_TRACE(CFW_SMS_TS_ID,"SMS_HaveFreeSlotEX, error err_code= 0x%x\n", err_code);
			pEvent->nParam2 = 1;
		}
	}

	pEvent->nParam1 = index;
	CSW_PROFILE_FUNCTION_EXIT(CFW_MeWriteMessage);
	return ERR_SUCCESS;
}

UINT32 CFW_MeReadMessage ( 
								  UINT16 nLocation,
								  UINT16 nIndex,
								  CFW_EV* pEvent
								  )
{
	CSW_PROFILE_FUNCTION_ENTER(CFW_MeReadMessage);
	pEvent->nEventId = EV_CFW_SMS_READ_MESSAGE_RSP;
	pEvent->nType = 0;
	//pEvent->nParam2 = nIndex;
#ifndef CFW_MULTI_SIM
	if(nLocation!=CFW_SMS_STORAGE_ME 
		&& nLocation!=CFW_SMS_STORAGE_BM
		&& nLocation!=CFW_SMS_STORAGE_ME_SR)
	{
		pEvent->nType = 0xf0;
		pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
		CSW_PROFILE_FUNCTION_EXIT(CFW_MeReadMessage);
		return ERR_SUCCESS;
	}
#endif
	if(nIndex <= 0 || nIndex > SMS_ME_MAX_INDEX)
	{
		pEvent->nType = 0xf0;
		pEvent->nParam1 = ERR_CME_INVALID_INDEX;
		CSW_PROFILE_FUNCTION_EXIT(CFW_MeReadMessage);
		return ERR_SUCCESS;
	}
	if(ERR_SUCCESS != SMS_ReadEX(nIndex, (UINT8*)pEvent->nParam1, nLocation))
	{
		pEvent->nType = 0xf0;
		pEvent->nParam1 = ERR_CME_ME_FAILURE;
		CSW_PROFILE_FUNCTION_EXIT(CFW_MeReadMessage);
		return ERR_SUCCESS;
	}
	
	CSW_PROFILE_FUNCTION_EXIT(CFW_MeReadMessage);
	return ERR_SUCCESS;
}

//----------------------------------------------------------------------------
//parameter: 
//            nCount:[in]	 0, 		only return the count filtered by the status and nSimId
//					others,   	SMS data will be saved in the last parameter pEvent.nParam1
//		 nStatus:[in]	 this value can be conbinated
//		 pEvent:[out]   nParam1 is used to save the output PDU, note that nParam1[0] is the status of
//					 SMS, it's value same as the input param nStatus. this is the difference between the SIM SMS
//-------------------------------------------------------------------------
UINT32 CFW_MeListMessage ( 
#ifndef CFW_MULTI_SIM
								  UINT16 nLocation, 
#else
								  CFW_SIM_ID nSimId,
#endif
								  UINT8 nStatus, 
								  UINT16 nCount, 
								  UINT16 nIndex,
								  CFW_EV* pEvent 
								  )
{
	UINT8  status=0x0;
	UINT8  count = 0;
	UINT16 index = 0;
	UINT8  data[SMS_DEFAULT_SIZE_EX];
	UINT8  *outp=NULL;

	CSW_PROFILE_FUNCTION_ENTER(CFW_MeListMessage);

    outp =  (UINT8 *)pEvent->nParam1;
	pEvent->nEventId = EV_CFW_SMS_LIST_MESSAGE_RSP;
	pEvent->nType = 0;
#ifndef CFW_MULTI_SIM
	if(nLocation!=CFW_SMS_STORAGE_ME 
		&& nLocation!=CFW_SMS_STORAGE_BM
		&& nLocation!=CFW_SMS_STORAGE_ME_SR)
	{
		pEvent->nType = 0xf0;
		pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
		CSW_PROFILE_FUNCTION_EXIT(CFW_MeListMessage);
		return ERR_SUCCESS;
	}
#else
	pEvent->nParam2= 0;

#endif
	if(nCount>SMS_ME_MAX_INDEX)
	{
		pEvent->nType = 0xf0;
		pEvent->nParam1 = ERR_CMS_INVALID_STATUS;
		CSW_PROFILE_FUNCTION_EXIT(CFW_MeListMessage);
		return ERR_SUCCESS;
	}

	index = nIndex;

//#ifdef CFW_MULTI_SIM
//		UINT16 tmpIndex[CFW_SMS_MAX_REC_COUNT]={0,};
//		count = SMS_DM_GetIndexByStatus(tmpIndex, nStatus, nSimId);
//#endif
  UINT16 nStartIndex=0,  nEndIndex=0;
#ifdef CFW_MULTI_SIM
  SMS_DM_GetRange(nSimId,&nStartIndex, &nEndIndex) ;
 // index += nStartIndex;
#else
  SMS_DM_GetRange(&nStartIndex, &nEndIndex) ;

#endif
	if(nCount == 0)
	{
//#ifndef CFW_MULTI_SIM
		UINT8 i=nStartIndex;
		for(i=nStartIndex; i < nEndIndex; i++)
		{
			if(ERR_SUCCESS == SMS_Read2EX((UINT16)(i+1), &status, &data[1], CFW_SMS_STORAGE_ME))
			{
				//status = SMS_SFSim2User(status);
				if((UINT8)0 != (status&nStatus)) // is the required status
				{
					count++;
				}
			}
		}
//#endif
		pEvent->nType = 0;
		//pEvent->nParam1 = 0;
		pEvent->nParam2 = count;
		CSW_PROFILE_FUNCTION_EXIT(CFW_MeListMessage);
		return ERR_SUCCESS;
	}
	else
	{
#ifndef CFW_MULTI_SIM
		CFW_SMS_STORAGE_INFO  sStorageInfo;


		SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
		CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_ME);
		if(sStorageInfo.usedSlot == 0)
		{
			return ERR_CONTINUE;
		}
#endif
		while((count<nCount) && (index <= nEndIndex))
		{
			if(ERR_SUCCESS == SMS_Read2EX(index, &status, &data[1], CFW_SMS_STORAGE_ME))
			{
				UINT8 nTmpStatus=0x0;

				nTmpStatus = status;
				status = SMS_SFSim2UserEX(status);
				if((UINT8)0 != (status&nStatus)) // is the required status
				{
					data[0] = nTmpStatus;
					SUL_MemCopy8((UINT8 *)((UINT8*)outp+SMS_DEFAULT_SIZE_EX*count), data, SMS_DEFAULT_SIZE_EX);
					count++;
				}
			}
			index++;
		}

		if(index > SMS_ME_MAX_INDEX)
		{
			pEvent->nType = 0;
			//pEvent->nParam1 = 0;
			pEvent->nParam2 = 0x00010000 | count;
			CSW_PROFILE_FUNCTION_EXIT(CFW_MeListMessage);
			return ERR_SUCCESS;
		}
		else
		{
			pEvent->nType = 0;
			// pEvent->nParam1  has been set at the beginning of the function
			// with the code: outp = (UINT8 *)pEvent->nParam1;
			pEvent->nParam2 = index-1;
			CSW_PROFILE_FUNCTION_EXIT(CFW_MeListMessage);
			return ERR_SUCCESS;
		}
	}
}


UINT32  CFW_MeGetStorageInfo ( 
#ifndef CFW_MULTI_SIM
						UINT16 nLocation,
#else
						CFW_SIM_ID nSimId,
#endif						
						UINT8 nStatus,
						CFW_EV* pEvent
								  )
{
	CFW_SMS_STORAGE_INFO *storageinfo = (CFW_SMS_STORAGE_INFO *)pEvent->nParam1;
	UINT16 usedSlot=0x0;
	UINT16 maxSlot=0x0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_MeGetStorageInfo);

	pEvent->nEventId = EV_CFW_SMS_GET_STORAGE_RSP;
	pEvent->nType = 0;

    if(CFW_SMS_STORED_STATUS_STORED_ALL == nStatus)
    {
        nStatus = 0xff; // List all of the records.
    }
#ifndef CFW_MULTI_SIM
	if(nLocation!=CFW_SMS_STORAGE_ME 
		&& nLocation!=CFW_SMS_STORAGE_BM
		&& nLocation!=CFW_SMS_STORAGE_ME_SR)
	{
		pEvent->nType = 0xf0;
		pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
		CSW_PROFILE_FUNCTION_EXIT(CFW_MeGetStorageInfo);
		return ERR_SUCCESS;
	}
    if(ERR_SUCCESS != SMS_GetStorageInfoEX(&usedSlot, &maxSlot, CFW_SMS_STORAGE_ME,nStatus))
#else
    if(ERR_SUCCESS != SMS_GetStorageInfoEX(&usedSlot, &maxSlot, nSimId, nStatus))
#endif
	{
		pEvent->nType = 0xf0;
		pEvent->nParam1 = ERR_CMS_UNKNOWN_ERROR;
	}
	else
	{
		storageinfo->storageId = CFW_SMS_STORAGE_ME;
		storageinfo->totalSlot = maxSlot;
		storageinfo->usedSlot = usedSlot;
		pEvent->nType = 0x0;
	}

	CSW_PROFILE_FUNCTION_EXIT(CFW_MeGetStorageInfo);
	return ERR_SUCCESS;
}

//#endif //#if USING_FLASH_API

