#include "dsm_cf.h"
#include "error_id.h"
#include "sms_dm_cd.h"
#include "sms_dm_local.h"
#include "dsm_dbg.h"

UINT32 g_tstSmsSize = 0;
/**************************************************************************************/
// Function: This function is initaliazed the sms database.
//           if the sector is unsigned.will been erasured the secors,signed the sectors,and 
//           set the global variables,else, will been set the global variable.
// Parameter: 
//    None.
// Return value: If the function succeeds, the return value is ERR_SUCCESS.
// If the function fails, the return value as follows:
//    ERR_SMS_DM_NOT_ENOUGH_MEDIA_SPACE: check media space failed.
//    ERR_SMS_DM_MEDIA_WRITE_FAILED: write the medium failed. 
//    ERR_SMS_DM_NOT_ENOUGH_MEMORY: alloc memory failed.
/***************************************************************************************/

INT32 SMS_DM_Init(VOID)
{
  INT32 iRet;
  BOOL bResult; 
  INT8 iResult;
  
     
  bResult = SMS_DM_Existed();
  if(FALSE == bResult)
  {            
    iRet = SMS_DM_Create();
    ERR_RETURN(iRet != ERR_SUCCESS, iRet, "SMS_DM_Create() return failed.");
  }
  else
  {
    iRet = SMS_DM_VersionCompare(&iResult);
    ERR_RETURN(iRet != ERR_SUCCESS, iRet, "SMS_DM_VersionCompare() return failed.");
    
    if(iResult != 0)
    {
      iRet = SMS_DM_Create();
      ERR_RETURN(iRet != ERR_SUCCESS, iRet, "SMS_DM_Create() return failed.");
    }
    else
    {
      iRet = SMS_DM_IsLogActive(&bResult);
      ERR_RETURN(iRet != ERR_SUCCESS, iRet, "SMS_DM_IsLogActive() return failed.");
      
      if (TRUE == bResult)
      {
        iRet = SMS_DM_LogRecove();
        ERR_RETURN(iRet != ERR_SUCCESS, iRet, "SMS_DM_LogRecove() return failed.");
      }
    }
  }

  if(0 == g_SMS_DM_Sem)
  {
    SMS_DM_SemInit();
  }
  g_tstSmsSize = SMS_DM_SZ_MEDIA_USE;
  g_SMS_DM_InitFlag = SMS_INITIALIZED;
  return ERR_SUCCESS;
  
}



/**************************************************************************************/
// Function: This function is add a record to the sms database.
// Parameter: 
// pIndex: [out]Output the index in the index table,it's range is from 0 to SMS_DM_MAX_REC_COUNT - 1.
// pData:  [in]The data of the record,the pData size equal to SMS_DM_SZ_RECORD_DATA.
// Return value: 
// If the function succeeds, the return value is ERR_SUCCESS.
// If the function fails, the return value as follows:
//     ERR_SMS_DM_UNINIT:Uninitialize the sms database.
//     ERR_SMS_DM_INVALID_PARAMETER: parameter is invalid.
//     ERR_SMS_DM_WRITEMEDIA_FAILED: write the medium failed.       
//     ERR_SMS_DM_SPACE_FULL: the space is full,maximal record count is SMS_DM_MAX_REC_COUNT.
/***************************************************************************************/
#ifndef CFW_MULTI_SIM
INT32 SMS_DM_Add(UINT16* pIndex, PVOID pData)
#else
INT32 SMS_DM_Add(UINT16* pIndex, PVOID pData, CFW_SIM_ID nSimId)
#endif
{
  INT32 iRet;
  UINT16 iIndex;
  UINT8 iDataStatus;
  
  if(g_SMS_DM_InitFlag != SMS_INITIALIZED)   
  {
    return ERR_SMS_DM_UNINIT;
  }
  
  if((NULL == pIndex) || (NULL == pData))
  {
    return ERR_SMS_DM_INVALID_PARAMETER;
  }

  iDataStatus = *((UINT8*)pData);
  if (SMS_REC_IVALID_STATUS == iDataStatus)
  {
    return ERR_SMS_DM_INVALID_PARAMETER;
  }
  
  SMS_DM_Down();
#ifndef CFW_MULTI_SIM
  iRet = SMS_DM_GetFreeIndex(&iIndex);
#else
  iRet = SMS_DM_GetFreeIndex(&iIndex, nSimId);
#endif
  if (iRet != ERR_SUCCESS)
  {
      goto failed;
  }
  
  iRet = SMS_DM_MakeLog(iIndex, SMS_DM_STATUS_REC_VALID, pData);
  ERR_GOTO(iRet != ERR_SUCCESS, iRet, failed, "SMS_DM_MakeLog() return failed.");
  
  iRet = SMS_DM_WriteRecordData(iIndex, pData);
  ERR_GOTO(iRet != ERR_SUCCESS, iRet, failed, "SMS_DM_WriteRecordData() return failed.");

  iRet = SMS_DM_SetRecordStatus(iIndex, SMS_DM_STATUS_REC_VALID);
  ERR_GOTO(iRet != ERR_SUCCESS, iRet, failed, "SMS_DM_SetRecordStatus() return failed.");

  iRet = SMS_DM_ClearLog();
  ERR_GOTO(iRet != ERR_SUCCESS, iRet, failed, "SMS_DM_ClearLog() return failed.");
  
  *pIndex = iIndex;
  iRet = ERR_SUCCESS;

failed:
  SMS_DM_Up();
  return iRet;
  
}


/**************************************************************************************/
// Function: This function is read a record from the sms database.
// Parameter: 
// iIndex: [in] Specified the index of the record,it's range is from 0 to SMS_DM_MAX_REC_COUNT - 1.
// pData:  [out]Output The data of the record,the pData size equal to SMS_DM_SZ_RECORD_DATA.
// Return value: 
// If the function succeeds, the return value is ERR_SUCCESS.
// If the function fails, the return value as follows:
//     ERR_SMS_DM_UNINIT:Uninitialize the sms database.
//     ERR_SMS_DM_INVALID_PARAMETER: parameter is invalid.
//     ERR_SMS_DM_MEDIA_READ_FAILED: read the medium failed.       
//	 ERR_SMS_DM_INVALID_INDEX: the index is ivalid.
/***************************************************************************************/
INT32 SMS_DM_Read(UINT16 iIndex, PVOID pData)
{
  UINT16 iStatus;
  INT32 iRet;
   
  if(g_SMS_DM_InitFlag != SMS_INITIALIZED)   
  {
    return ERR_SMS_DM_UNINIT;
  }
  
  if (iIndex >= SMS_DM_MAX_REC_COUNT || NULL == pData)
  {
    return ERR_SMS_DM_INVALID_PARAMETER;
  }
  
  SMS_DM_Down();
  
  iRet = SMS_DM_GetRecordStatus(iIndex, &iStatus);
  ERR_GOTO(iRet != ERR_SUCCESS, iRet, failed, "SMS_DM_GetRecordStatus() return failed.")

  ERR_GOTO(SMS_DM_STATUS_REC_FREE == iStatus, ERR_SMS_DM_INVALID_INDEX, failed, "status of record is free.");
  ERR_GOTO(iStatus != SMS_DM_STATUS_REC_VALID, ERR_SMS_DM_ERROR, failed, "status of record is unknow.");

  iRet = SMS_DM_ReadRecordData(iIndex, pData);

failed:
    SMS_DM_Up();
    return iRet;
    
}


/**************************************************************************************/
// Function: This function is write a specified index record to the sms database.
//           if the specified index record is invalid.will been return ERR_SMS_DM_INVALID_INDEX.
// Parameter: 
// iIndex: [in] Specified the index of the record,it's range is from 0 to SMS_DM_MAX_REC_COUNT - 1.
// pData:  [in] The data of the record,the pData size equal to SMS_REC_DATA_SIZE.
// Return value: If the function succeeds, the return value is ERR_SUCCESS.
// If the function fails, the return value as follows:
//    ERR_SMS_DM_UNINIT:Uninitialize the sms database.
//    ERR_SMS_DM_INVALID_PARAMETER: parameter is invalid.
//    ERR_SMS_DM_MEDIA_WRITE_FAILED: write the medium failed.       
//    ERR_SMS_DM_INVALID_INDEX: the index is invalid.
/***************************************************************************************/
INT32 SMS_DM_OverWrite(UINT16 iIndex, PVOID pData)
{
  UINT16 iStatus;
  INT32 iRet;

  if(g_SMS_DM_InitFlag != SMS_INITIALIZED)   
  {
    return ERR_SMS_DM_UNINIT;
  }
  
  if (iIndex >= SMS_DM_MAX_REC_COUNT || NULL == pData)
  {
    return ERR_SMS_DM_INVALID_PARAMETER;
  }
      
  SMS_DM_Down();
  
  iRet = SMS_DM_GetRecordStatus(iIndex, &iStatus);
  ERR_GOTO(iRet != ERR_SUCCESS, iRet, failed, "SMS_DM_GetRecordStatus() return failed.")

  ERR_GOTO(SMS_DM_STATUS_REC_FREE == iStatus, ERR_SMS_DM_INVALID_INDEX, failed, "status of record is free.");
  ERR_GOTO(iStatus != SMS_DM_STATUS_REC_VALID, ERR_SMS_DM_ERROR, failed, "status of record is unknow.");
  
  iRet = SMS_DM_MakeLog(iIndex, SMS_DM_STATUS_REC_VALID, pData);
  ERR_GOTO(iRet != ERR_SUCCESS, iRet, failed, "SMS_DM_MakeLog() return failed.");
  
  iRet = SMS_DM_WriteRecordData(iIndex, pData);
  ERR_GOTO(iRet != ERR_SUCCESS, iRet, failed, "SMS_DM_WriteRecordData() return failed.");

  iRet = SMS_DM_ClearLog();
  

failed:
  SMS_DM_Up();
  return iRet;
}


/**************************************************************************************/
// Function: This function is delete a specified index record from the sms database.
//           if the specified index record is invalid.will been return ERR_SMS_DM_INVALID_INDEX.
// Parameter: 
// iIndex: [in] Specified the index of the record,it's range is from 0 to SMS_DM_MAX_REC_COUNT - 1.
// Return value: If the function succeeds, the return value is ERR_SUCCESS.
// If the function fails, the return value as follows:
//    ERR_SMS_DM_UNINIT:Uninitialize the sms database.
//    ERR_SMS_DM_INVALID_PARAMETER: parameter is invalid.
//    ERR_SMS_DM_MEDIA_WRITE_FAILED: write the medium failed.       
//    ERR_SMS_DM_INVALID_INDEX: the index is ivalid.
/***************************************************************************************/
INT32 SMS_DM_Delete(UINT16 iIndex)
{
  UINT16 iStatus;
  INT32 iRet;

  if(g_SMS_DM_InitFlag != SMS_INITIALIZED)   
  {
    return ERR_SMS_DM_UNINIT;
  }
  
  if (iIndex >= SMS_DM_MAX_REC_COUNT)
  {
    return ERR_SMS_DM_INVALID_PARAMETER;
  }
      
  SMS_DM_Down();
  
  iRet = SMS_DM_GetRecordStatus(iIndex, &iStatus);
  ERR_GOTO(iRet != ERR_SUCCESS, iRet, failed, "SMS_DM_GetRecordStatus() return failed.")

  ERR_GOTO(SMS_DM_STATUS_REC_FREE == iStatus, ERR_SMS_DM_INVALID_INDEX, failed, "status of record is free.")
  ERR_GOTO(iStatus != SMS_DM_STATUS_REC_VALID, ERR_SMS_DM_ERROR, failed, "status of record is unknow.")

  iRet = SMS_DM_MakeLog(iIndex, SMS_DM_STATUS_REC_FREE, NULL);
  ERR_GOTO(iRet != ERR_SUCCESS, iRet, failed, "SMS_DM_MakeLog() return failed.")
  
  iRet = SMS_DM_SetRecordStatus(iIndex, (UINT16)SMS_DM_STATUS_REC_FREE);
  ERR_GOTO(iRet != ERR_SUCCESS, iRet, failed, "SMS_DM_SetRecordStatus() return failed.")

  iRet = SMS_DM_ClearLog();

failed:
  SMS_DM_Up();
  return iRet;

}


/**************************************************************************************/
// Function: This function is rebuied the sms database.
// Parameter: 
//    None.
// Return value: If the function succeeds, the return value is ERR_SUCCESS.
// If the function fails, the return value as follows:
//    ERR_SMS_DM_NOT_ENOUGH_MEDIA_SPACE: check media space failed.
//    ERR_SMS_DM_MEDIA_WRITE_FAILED: write the medium failed. 
/***************************************************************************************/
INT32 SMS_DM_Clear(VOID)
{
  INT32 iRet;

  if (SMS_INITIALIZED == g_SMS_DM_InitFlag)
  {
    g_SMS_DM_InitFlag = SMS_UNINIT;
  }
  
  iRet = SMS_DM_Create();
  ERR_RETURN(iRet != ERR_SUCCESS, iRet, "SMS_DM_Create() return failed.");

  if(0 == g_SMS_DM_Sem)
  {
    SMS_DM_SemInit();
  }

  g_SMS_DM_InitFlag = SMS_INITIALIZED;
  return ERR_SUCCESS;
  
}


/**************************************************************************************/
// Function: This function is list the record of specified status from the sms database.
// Parameter: 
// pIndex: [out] output the list of index.
// iStatus: [in]Specified the status for the list of record.
// Return value: return the count of the index list.
/***************************************************************************************/
#ifndef CFW_MULTI_SIM
UINT16 SMS_DM_GetIndexByStatus(UINT16* pIndex, UINT8 iStatus)
#else
UINT16 SMS_DM_GetIndexByStatus(UINT16* pIndex, UINT8 iStatus, CFW_SIM_ID nSimId)
#endif
{
  UINT16 i = 0;
  UINT16 iRecordIndex;
  UINT16 iRecordStatus;
  UINT8   iDataStatus;
  INT32   iRet;
  UINT16 nStartIndex=0,  nEndIndex=0;
  if((NULL == pIndex) || (SMS_REC_IVALID_STATUS == iStatus))
  {
    return 0;
  }   
  
  if(g_SMS_DM_InitFlag != SMS_INITIALIZED)   
  {
    return 0;
  }
     
  SMS_DM_Down();
  
  //[[hameina[mod] 2009.1.22
#ifdef CFW_MULTI_SIM
  iRet = SMS_DM_GetRange(nSimId,&nStartIndex, &nEndIndex) ;
#else
  iRet = SMS_DM_GetRange( &nStartIndex, &nEndIndex) ;
#endif
  for(iRecordIndex = nStartIndex; iRecordIndex < nEndIndex; iRecordIndex++)
  //for(iRecordIndex = 0; iRecordIndex < SMS_DM_MAX_REC_COUNT; iRecordIndex++)
  //]]hameina[mod] 2009.1.22
  {
    iRet = SMS_DM_GetRecordStatus(iRecordIndex, &iRecordStatus);
    if (iRet != (INT32)ERR_SUCCESS)
    {
      continue;
    }

    if (iRecordStatus != SMS_DM_STATUS_REC_VALID)
    {
      continue;
    }
    
    iRet = SMS_DM_GetDatadStatus(iRecordIndex, &iDataStatus);
    if (iRet != (INT32)ERR_SUCCESS)
    {
      continue;
    }
    
    if(0 != (iStatus & iDataStatus))
    {
      *(pIndex + i) = iRecordIndex;
      i ++;
    }       
  }
  
  SMS_DM_Up();
  
  return i;
}


/**************************************************************************************/
// Function: This function get the  version of sms_dm module.
// Parameter: 
// None.
// Return value: return the version of sms_dm module..
/***************************************************************************************/
UINT32 SMS_DM_GetVersion(VOID)
{
    return SMS_DM_VERSION;
}


