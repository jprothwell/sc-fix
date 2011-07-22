/******************************************************************************/
/*              Copyright (C) 2006~2007, CoolSand Technologies, Inc.          */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    sms_dm.h                                                         */
/*                                                                            */
/* Description:                                                               */
/*    sms dm declarations, constant definitions and macros               */
/******************************************************************************/

#ifndef __SMS_DM_H__
#define __SMS_DM_H__


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
INT32 SMS_DM_Add(UINT16* pIndex, PVOID pData);
#else
INT32 SMS_DM_Add(UINT16* pIndex, PVOID pData,CFW_SIM_ID nSimId);
#endif


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
//     ERR_SMS_DM_INVALID_INDEX: the index is ivalid.
/***************************************************************************************/
INT32 SMS_DM_Read(UINT16 iIndex, PVOID pData);


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
INT32 SMS_DM_OverWrite(UINT16 iIndex, PVOID pData);


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
INT32 SMS_DM_Delete( UINT16 iIndex);


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
INT32 SMS_DM_Init(VOID);


/**************************************************************************************/
// Function: This function is rebuied the sms database.
// Parameter: 
//    None.
// Return value: If the function succeeds, the return value is ERR_SUCCESS.
// If the function fails, the return value as follows:
//    ERR_SMS_DM_NOT_ENOUGH_MEDIA_SPACE: check media space failed.
//    ERR_SMS_DM_MEDIA_WRITE_FAILED: write the medium failed. 
/***************************************************************************************/
INT32 SMS_DM_Clear(VOID);


/**************************************************************************************/
// Function: This function is list the record of specified status from the sms database.
// Parameter: 
// pIndex: [out] output the list of index.
// iStatus: [in]Specified the status for the list of record.
// Return value: return the count of the index list.
/***************************************************************************************/
#ifndef CFW_MULTI_SIM
UINT16 SMS_DM_GetIndexByStatus(UINT16* pIndex, UINT8 iStatus);
#else
UINT16 SMS_DM_GetIndexByStatus(UINT16* pIndex, UINT8 iStatus, CFW_SIM_ID nSimId);
#endif

/**************************************************************************************/
// Function: This function get the  version of sms_dm module.
// Parameter: 
// None.
// Return value: return the version of sms_dm module..
/***************************************************************************************/
UINT32 SMS_DM_GetVersion(VOID);



//#define ERR_SUCCESS                    0
#endif //__SMS_DM_H__

