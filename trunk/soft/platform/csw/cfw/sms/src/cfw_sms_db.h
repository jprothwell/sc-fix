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
#ifndef _SMS_DB_H
#define _SMS_DB_H

#include <errorcode.h>
#include <tm.h>
#include <event.h>
#include <cfw.h>

#define SMSEX
//Extern 
//#define SMS_ME_DEFAULT_DB_SIZE   95248 //500  SMS_ME_MAX_NUM(db_size) ((db_size-48)/190 - 1) 
//#define SMS_ME_DEFAULT_DB_SIZE   38248 //200  SMS_ME_MAX_NUM(db_size) ((db_size-48)/190 - 1) 
//#define SMS_ME_DEFAULT_DB_SIZE   9738 //50  SMS_ME_MAX_NUM(db_size) ((db_size-48)/190 - 1) 
//#define SMS_ME_DEFAULT_DB_SIZE   57238 //300  SMS_ME_MAX_NUM(db_size) ((db_size-48)/190 - 1) 
//#define SMS_ME_DEFAULT_DB_SIZE   9738 //50  SMS_ME_MAX_NUM(db_size) ((db_size-48)/190 - 1) 


#define ERR_CME_ME_FAILURE                 2007    //DB interface error code for DB operation failuer
#define ERR_CME_ME_SPACE_FULL              2008

//
#define SMS_DB_NAME "sms.db"
#define SMS_DB_FIELD_NUM     0x06

#define ID_SMS_DEL_FLAG      0x01           // 0x00-deleted, 0xff-not deleted
#define FT_SMS_DEL_FLAG      DB_UINT8       // FT-Field Type
#define FS_SMS_DEL_FLAG      1              // FS-Field Size
      
#define ID_SMS_INDEX         0x02           // based index is 1.
#define FT_SMS_INDEX         DB_UINT32
#define FS_SMS_INDEX         4

#define ID_SMS_STATUS        0x03           //8 status
#define FT_SMS_STATUS        DB_UINT8
#define FS_SMS_STATUS        1

#define ID_SMS_DATA          0x04           //Short message content
#define FT_SMS_DATA          DB_BINARY
#define FS_SMS_DATA          175

#define ID_SMS_TIME          0x05           //Short message content
#define FT_SMS_TIME          DB_UINT32
#define FS_SMS_TIME          4

#define ID_SMS_APPINT32      0x06           //Short message content
#define FT_SMS_APPINT32      DB_UINT32
#define FS_SMS_APPINT32      4

#define SMS_DEFAULT_SIZE    176 
#define SMS_DEFAULT_SIZE_EX (176+4+4) 
//
//Modify by nyf
//at 20060725 for 
//
/*
DB_Size �� 24 �� 4��FieldsCntPerRec �� RecCnt����RecSize��1��

DB_Size����������ݿ�ռ�
FieldsCntPerRec��ÿ����¼���ֶ���
RecCnt�����ݿ�������ɵ��ܼ�¼����
RecSize��ÿ����¼�Ĵ�С����λ���ֽ���
*/
#define DB_HEAD_FIXED_SIZE	24
#define SMS_DB_FIELDS_SIZE  SMS_DB_FIELD_NUM*4
#define SMS_DB_HEAD_SIZE    (DB_HEAD_FIXED_SIZE + SMS_DB_FIELDS_SIZE)
#define SMS_DB_RECORD_SIZE  ((1+4+1+175+4+4) + 1)

extern UINT32 g_cfw_sms_max_num;
#ifndef CFW_MULTI_SIM
#define SMS_ME_MAX_INDEX   ( g_cfw_sms_max_num)//((SMS_ME_DB_SIZE-SMS_DB_HEAD_SIZE)/SMS_DB_RECORD_SIZE)
#else
#define SMS_ME_MAX_INDEX   ( g_cfw_sms_max_num)//((SMS_ME_DB_SIZE-SMS_DB_HEAD_SIZE)/SMS_DB_RECORD_SIZE)
#endif
#define SMS_DB_DEL_FLAG     0x00
#define SMS_DB_EXIST_FALG   0xf0

#define SMS_ME_MAX_NUM(db_size) ((db_size-SMS_DB_HEAD_SIZE)/SMS_DB_RECORD_SIZE - 1)
#ifndef SMSEX
INT32 Create_SMS_DB();
INT32 Remove_SMS_DB();

INT32 SMS_DeleteFromMeByStatus(const UINT8 nStatus, UINT16 nLocation);
INT32 SMS_DeleteFromMeByIndex(const UINT16 nIndex, UINT16 nLocation);
INT32 SMS_Append(UINT8* pData, UINT16 nLocation, UINT16 *nIndex, UINT32 nTime, UINT32 nAppInt32);
INT32 SMS_Write(UINT16 nIndex, UINT8* pData, UINT16 nLocation, UINT32 nTime, UINT32 nAppInt32);
INT32 SMS_Read(UINT16 nIndex, UINT8* pData, UINT16 nLocation);
INT32 SMS_Read2(UINT16 nIndex, UINT8* nStatus, UINT8* pData, UINT16 nLocation, HDB hdb);

INT32 SMS_HaveFreeSlot(UINT16 nLocation);
INT32 SMS_SetStorageInfo(UINT16 nUsedSlot, UINT16 nLocation);
INT32 SMS_GetStorageInfo(UINT16 *pUsedSlot, UINT16 *pMaxSlot, UINT16 nLocation,UINT8 nStatus);
INT32 SMS_GetStorageInfo2(UINT16 *pUsedSlot, UINT16 *pMaxSlot);
INT32 SMS_UpdateStorageInfo(BOOL bInc, UINT16 nLocation);
UINT8 SMS_SFSim2User(UINT8 state);
UINT8 SMS_SFUser2Sim(UINT8 state);
#endif
#define SMS_NO_FREE_SLOT -1
#define ERR_SMS_DB_INVALID_INDEX -1
#define ERR_SMS_DB_INVALID_STATUS -1
#define ERR_SMS_DB_RECORD_NOT_EXIST -1
#define ERR_SMS_DB_SPACE_NULL 0x05


///////////////////////////////////////////////////////////////////////////////
//                         Local ME DB Operation API                         //
///////////////////////////////////////////////////////////////////////////////
UINT32 CFW_MeReadMessage   (UINT16 nLocation, UINT16 nIndex, CFW_EV* pEvent);
#ifndef CFW_MULTI_SIM
UINT32 CFW_MeDeleteMessage (UINT16 nLocation, UINT16 nIndex, UINT8 nStatus,CFW_EV* pEvent);
UINT32 CFW_MeWriteMessage  (UINT16 nLocation, UINT16 nIndex, UINT8* pData, UINT8 nDataSize,UINT32 nTime,UINT32 nAppInt32,CFW_EV* pEvent);
UINT32 CFW_MeListMessage   (UINT16 nLocation, UINT8 nStatus, UINT16 nCount,UINT16 nListStartIndex, CFW_EV* pEvent); 
UINT32 CFW_MeGetStorageInfo(UINT16 nLocation, UINT8 nStatus,CFW_EV* pEvent);
#else
UINT32 CFW_MeDeleteMessage (CFW_SIM_ID nSimId, UINT16 nIndex, UINT8 nStatus,CFW_EV* pEvent);
UINT32 CFW_MeWriteMessage  (CFW_SIM_ID nSimId, UINT16 nIndex, UINT8* pData, UINT8 nDataSize,UINT32 nTime,UINT32 nAppInt32,CFW_EV* pEvent);
UINT32 CFW_MeListMessage   (CFW_SIM_ID nSimId, UINT8 nStatus, UINT16 nCount,UINT16 nListStartIndex, CFW_EV* pEvent); 
UINT32 CFW_MeGetStorageInfo(CFW_SIM_ID nSimId, UINT8 nStatus,CFW_EV* pEvent);
#endif
UINT32 sms_db_InitEX(void);

UINT8 SMS_SFUser2SimEX(UINT8 state);
UINT8 SMS_SFSim2UserEX(UINT8 state);

#endif //_SMS_DB_H
