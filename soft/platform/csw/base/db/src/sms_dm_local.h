#ifndef _SMS_DM_LOCAL_H_
#define _SMS_DM_LOCAL_H_

//#include "sms_dm_cd.h"

#define SMS_DM_VERSION                     0x090220      // the version number

extern UINT16 g_SMS_DM_InitFlag;
extern HANDLE g_SMS_DM_Sem;

#ifdef _MS_VC_VER_
#define SMS_DM_SemInit() 
#define SMS_DM_Down() 
#define SMS_DM_Up() 
extern UINT32 g_CSW_Dm_MaxRecCount;
extern UINT32 g_CSW_Dm_RecDataSize;

#else

#define SMS_DM_SemInit() do { g_SMS_DM_Sem = COS_CreateSemaphore(1); } while(0)
#define SMS_DM_Down()    do { COS_WaitForSemaphore(g_SMS_DM_Sem, COS_WAIT_FOREVER); } while(0)
#define SMS_DM_Up()        do { COS_ReleaseSemaphore(g_SMS_DM_Sem); } while(0)

#endif

INT32 DM_Read(UINT32 ulAddrOffset,UINT8* pBuffer, UINT32 ulBytesToRead, UINT32* pBytesRead);    
INT32 DM_Write(UINT32 ulAddrOffset,CONST UINT8* pBuffer,UINT32 ulBytesToWrite,UINT32*  pBytesWritten);


BOOL  SMS_DM_Existed();
INT32 SMS_DM_Create(VOID);
INT32 SMS_DM_VersionCompare();
#ifndef CFW_MULTI_SIM
INT32 SMS_DM_GetFreeIndex(UINT16 *pIndex);
//Hameina[+] for 221 project, but the normal project will also need this function.
INT32 SMS_DM_GetRange(UINT16*nStartIndex, UINT16*nEndIndex);
#else
INT32 SMS_DM_GetFreeIndex(UINT16 *pIndex, CFW_SIM_ID nSimId);
//Hameina[+] for 221 project, but the normal project will also need this function.
INT32 SMS_DM_GetRange(CFW_SIM_ID nSimId, UINT16*nStartIndex, UINT16*nEndIndex);
#endif

INT32 SMS_DM_SetRecordStatus(UINT16 iIndex, UINT16 iStatus );
INT32 SMS_DM_GetRecordStatus(UINT16 iIndex, UINT16 *pStatus );
INT32 SMS_DM_GetDatadStatus(UINT16 iIndex, UINT8 *pStatus);
INT32 SMS_DM_ReadRecordData(UINT16 iIndex, PVOID pData);
INT32 SMS_DM_WriteRecordData(UINT16 iIndex, PVOID pData);
INT32 SMS_DM_ReadHeadInfo(SMS_DM_H_INFO *psHeadInfo);
INT32 SMS_DM_WriteHeadInfo(SMS_DM_H_INFO *psHeadInfo);
INT32 SMS_DM_GetFreeRecord(UINT16 *pIndex);
INT32 SMS_DM_MakeLog(UINT16 iIndex, UINT16 iStatus, PVOID pData);
INT32 SMS_DM_ClearLog(VOID);
INT32 SMS_DM_IsLogActive(BOOL *pResult);
INT32 SMS_DM_LogRecove(VOID);

#endif
