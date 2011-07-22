// //////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2002-2009, Coolsand Technologies, Inc.
// All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmission of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
// 
// FILENAME: cfw_sim_sms_pro.c
// 
// DESCRIPTION:
// TODO: ...
// 
// REVISION HISTORY:
// NAME              DATE                REMAKS
// Lixp                2009-3-20       Created initial version 1.0
// 
// //////////////////////////////////////////////////////////////////////////////

#include <cswtype.h>
#include <errorcode.h>
// #include <scl.h>
#include "api_msg.h"
#include <cfw.h>
#include <base.h>
#include <cfw_prv.h>
#include <event.h>
#include <sul.h>
#include <cos.h>
#include <ts.h>
#include <csw_mem_prv.h>
#include "csw_debug.h"
#if (CFW_SIM_DUMP_ENABLE==0) && (CFW_SIM_SRV_ENABLE==1)

#include "cfw_sim_sms.h"

#ifndef CFW_MULTI_SIM

UINT8 * gSimSMSStatusArray = NULL;

VOID _SimSMSStatusArrayInit(UINT32 TotalNum)
{
  gSimSMSStatusArray = (UINT8*)CSW_SIM_MALLOC(TotalNum + 1 );
  SUL_ZeroMemory8(gSimSMSStatusArray,TotalNum + 1);
  gSimSMSStatusArray[0] = TotalNum;
}

VOID _SimSMSStatusArrayAdd(UINT32 nRecordID)
{
  gSimSMSStatusArray[nRecordID] = 0x01;
}
VOID _SimSMSStatusArrayRemove(UINT32 nRecordID)
{
  gSimSMSStatusArray[nRecordID] = 0x00;
}
VOID _SimSMSStatusArraySeek(UINT32 *pRecordID)
{
  UINT8 i = 0x01;
  while(gSimSMSStatusArray[i++]);
  *pRecordID = i;
}
#else
UINT8 * gSimSMSStatusArray[CFW_SIM_COUNT] = {NULL,};

VOID _SimSMSStatusArrayInit(UINT32 TotalNum, CFW_SIM_ID nSimID)
{
  //UINT8 * p = NULL;
  gSimSMSStatusArray[nSimID] = (UINT8*)CSW_SIM_MALLOC(TotalNum + 1);
  hal_HstSendEvent(gSimSMSStatusArray[nSimID] );
  SUL_ZeroMemory8(gSimSMSStatusArray[nSimID],TotalNum + 1);
  gSimSMSStatusArray[nSimID][0] = TotalNum;
}

VOID _SimSMSStatusArrayAdd(UINT32 nRecordID, CFW_SIM_ID nSimID)
{
  gSimSMSStatusArray[nSimID][nRecordID] = 0x01;
}
VOID _SimSMSStatusArrayRemove(UINT32 nRecordID, CFW_SIM_ID nSimID)
{
  gSimSMSStatusArray[nSimID][nRecordID ] = 0x00;
}
VOID _SimSMSStatusArraySeek(UINT32 *pRecordID, CFW_SIM_ID nSimID)
{
  UINT8 i = 0x01;
  while((gSimSMSStatusArray[nSimID][i]) && (i <= gSimSMSStatusArray[nSimID][0]))
  {
    i ++;
  }
  *pRecordID = i;
}
#endif
UINT32 CFW_SimPatchProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32 nEvtId, nUTI, nErrCode;
	VOID* nEvParam = (VOID*)NULL;

	SIM_INFO_PATCH* pReadSmsInfo;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimPatchProc);
	if ((UINT32)pEvent != 0xFFFFFFFF)
	{
		nEvtId   = pEvent->nEventId;
		nEvParam = (VOID*)pEvent->nParam1;
	}

#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif

	pReadSmsInfo = CFW_GetAoUserData(hAo);

	switch (pReadSmsInfo->n_CurrStatus)
	{
		case SMS_STATUS_IDLE:
		{
			if ((UINT32)pEvent == 0xFFFFFFFF)
			{
				if (pReadSmsInfo->nTryCount >= 0x01)
				{
#ifdef CFW_MULTI_SIM
					nErrCode = SimReadRecordReq((UINT8)(pReadSmsInfo->nCurrentFile),
					(UINT8)(pReadSmsInfo->nIndex), SMS_MODE_ABSOLUTE, SMS_PDU_LEN, nSimID);
#else
					nErrCode = SimReadRecordReq((UINT8)(pReadSmsInfo->nCurrentFile),
					(UINT8)(pReadSmsInfo->nIndex), SMS_MODE_ABSOLUTE, SMS_PDU_LEN);
#endif
					if (ERR_SUCCESS != nErrCode)
					{

						CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", nErrCode);
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_PATCH_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_PATCH_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", nErrCode);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimPatchProc);
						return nErrCode;
					}

					CSW_TRACE(CFW_SIM_TS_ID,
					TSTXT("Currentfile %d,The index of sms %d\n"), pReadSmsInfo->nCurrentFile,
					pReadSmsInfo->nIndex);

					// CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);        
					pReadSmsInfo->n_PrevStatus = pReadSmsInfo->n_CurrStatus;
					pReadSmsInfo->n_CurrStatus = SMS_STATUS_READRECORD;

					// pReadSmsInfo->nTryCount --;
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
				}
				else
				{
					// Time out and TODO...
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}

			}
			else
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
		}
		break;
		case SMS_STATUS_READRECORD:
		{
			if (pEvent->nEventId == API_SIM_READRECORD_CNF)
			{
				api_SimReadRecordCnf_t* pSimReadRecordCnf = (api_SimReadRecordCnf_t*)nEvParam;

				if (((0x90 == pSimReadRecordCnf->Sw1) && (00 == pSimReadRecordCnf->Sw2)) || (0x91 == pSimReadRecordCnf->Sw1))
				{

#ifdef CFW_MULTI_SIM
		        nErrCode =SimUpdateRecordReq((UINT8)(pReadSmsInfo->nCurrentFile), 
		                                     (UINT8)(pReadSmsInfo->nIndex), 0x04,SMS_PDU_LEN, pSimReadRecordCnf->Data, nSimID);
#else
		        nErrCode = SimUpdateRecordReq((UINT8)(pReadSmsInfo->nCurrentFile), 
		                                      (UINT8)(pReadSmsInfo->nIndex), 0x04,SMS_PDU_LEN, pSimReadRecordCnf->Data);
#endif
		        if (ERR_SUCCESS != nErrCode)
		        {
		          CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
		          CFW_PostNotifyEvent(EV_CFW_SIM_PATCH_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xf0);
#else
		          CFW_PostNotifyEvent(EV_CFW_SIM_PATCH_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
		          CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n", nErrCode);
		          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
		          hAo = HNULL;
		          CSW_PROFILE_FUNCTION_EXIT(CFW_SimPatchProc);
		          return nErrCode;
		        }

					// CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);        
					pReadSmsInfo->n_PrevStatus = pReadSmsInfo->n_CurrStatus;
					pReadSmsInfo->n_CurrStatus = SMS_STATUS_UPDATERECORD;

					// pReadSmsInfo->nTryCount --;
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);

					return ERR_SUCCESS;
				}
				else
				{
#ifdef CFW_MULTI_SIM
					UINT32 ErrorCode = Sim_ParseSW1SW2((UINT8)(pSimReadRecordCnf->Sw1), (UINT8)(pSimReadRecordCnf->Sw2), nSimID);

					CFW_GetUTI(hAo, &nUTI);
					CFW_PostNotifyEvent(EV_CFW_SIM_PATCH_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xf0);
#else
					UINT32 ErrorCode = Sim_ParseSW1SW2((UINT8)(pSimReadRecordCnf->Sw1), (UINT8)(pSimReadRecordCnf->Sw2));

					CFW_GetUTI(hAo, &nUTI);
					CFW_PostNotifyEvent(EV_CFW_SIM_PATCH_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error Read Sim SMS 0x%x\n"), ErrorCode);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPatchProc);
					return ErrorCode;
				}

				// CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
			}
			else
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}
		}
		case SMS_STATUS_UPDATERECORD:
		{
		  if (pEvent->nEventId == API_SIM_UPDATERECORD_CNF)
		  {
				api_SimUpdateRecordCnf_t * pSimUpdateRecordCnf = (api_SimUpdateRecordCnf_t*)nEvParam;
				if (((0x90 == pSimUpdateRecordCnf->Sw1) && (00 == pSimUpdateRecordCnf->Sw2))
				|| (0x91 == pSimUpdateRecordCnf->Sw1))
				{
						CFW_GetUTI(hAo, &nUTI);

#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_PATCH_RSP, (UINT32)0, 0,
						nUTI | (nSimID << 24), 0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_PATCH_RSP, (UINT32)0, 0, nUTI, 0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_PATCH_RSP Success! \n"));
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo); hAo = HNULL;        
        }
        else
				{
#ifdef CFW_MULTI_SIM
					UINT32 ErrorCode = Sim_ParseSW1SW2((UINT8)(pSimUpdateRecordCnf->Sw1), (UINT8)(pSimUpdateRecordCnf->Sw2), nSimID);

					CFW_GetUTI(hAo, &nUTI);
					CFW_PostNotifyEvent(EV_CFW_SIM_PATCH_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xf0);
#else
					UINT32 ErrorCode = Sim_ParseSW1SW2((UINT8)(pSimUpdateRecordCnf->Sw1), (UINT8)(pSimUpdateRecordCnf->Sw2));

					CFW_GetUTI(hAo, &nUTI);
					CFW_PostNotifyEvent(EV_CFW_SIM_PATCH_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error update Sim SMS 0x%x\n"), ErrorCode);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPatchProc);
					return ErrorCode;
				}        

			}
			else
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}
		}
		break;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_SimPatchProc);
	return ERR_SUCCESS;
}
UINT32 CFW_SimReadMessageProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32 nEvtId, nUTI, nErrCode;
	VOID* nEvParam = (VOID*)NULL;

	SIM_INFO_READSMS* pReadSmsInfo;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimReadMessageProc);
	if ((UINT32)pEvent != 0xFFFFFFFF)
	{
		nEvtId   = pEvent->nEventId;
		nEvParam = (VOID*)pEvent->nParam1;
	}

#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif

	pReadSmsInfo = CFW_GetAoUserData(hAo);

	switch (pReadSmsInfo->n_CurrStatus)
	{
		case SMS_STATUS_IDLE:
		{
			if ((UINT32)pEvent == 0xFFFFFFFF)
			{
				if (pReadSmsInfo->nTryCount >= 0x01)
				{
#ifdef CFW_MULTI_SIM
					nErrCode = SimReadRecordReq((UINT8)(pReadSmsInfo->nCurrentFile),
					(UINT8)(pReadSmsInfo->nIndex), SMS_MODE_ABSOLUTE, SMS_PDU_LEN, nSimID);
#else
					nErrCode = SimReadRecordReq((UINT8)(pReadSmsInfo->nCurrentFile),
					(UINT8)(pReadSmsInfo->nIndex), SMS_MODE_ABSOLUTE, SMS_PDU_LEN);
#endif
					if (ERR_SUCCESS != nErrCode)
					{

						CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", nErrCode);
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_READ_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_READ_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", nErrCode);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadMessageProc);
						return nErrCode;
					}

					CSW_TRACE(CFW_SIM_TS_ID,
					TSTXT("Currentfile %d,The index of sms %d\n"), pReadSmsInfo->nCurrentFile,
					pReadSmsInfo->nIndex);

					// CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);        
					pReadSmsInfo->n_PrevStatus = pReadSmsInfo->n_CurrStatus;
					pReadSmsInfo->n_CurrStatus = SMS_STATUS_READRECORD;

					// pReadSmsInfo->nTryCount --;
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
				}
				else
				{
					// Time out and TODO...
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}

			}
			else
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
		}
		break;
		case SMS_STATUS_READRECORD:
		{
			if (pEvent->nEventId == API_SIM_READRECORD_CNF)
			{
				api_SimReadRecordCnf_t* pSimReadRecordCnf = (api_SimReadRecordCnf_t*)nEvParam;

				if (((0x90 == pSimReadRecordCnf->Sw1) && (00 == pSimReadRecordCnf->Sw2)) || (0x91 == pSimReadRecordCnf->Sw1))
				{
					UINT8* pGetSMSEntry = (UINT8*)CSW_SIM_MALLOC(SMS_PDU_LEN);

					if (pGetSMSEntry == NULL)
					{
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_READ_MESSAGE_RSP, (UINT32)ERR_NO_MORE_MEMORY,
						0, nUTI | (nSimID << 24), 0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_READ_MESSAGE_RSP, (UINT32)ERR_NO_MORE_MEMORY, 0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadMessageProc);
						return ERR_NO_MORE_MEMORY;
					}

					SUL_MemCopy8(pGetSMSEntry, pSimReadRecordCnf->Data, SMS_PDU_LEN);
					CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
					CSW_TRACE(CFW_SIM_TS_ID, "pGetSMSEntry 0x%x nSimID 0x%d\n",pGetSMSEntry,nSimID);
					CFW_PostNotifyEvent(EV_CFW_SIM_READ_MESSAGE_RSP, (UINT32)pGetSMSEntry, 0, nUTI | (nSimID << 24), 0x00);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_READ_MESSAGE_RSP, (UINT32)pGetSMSEntry, 0, nUTI, 0x00);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_READ_MESSAGE_RSP Success! \n"));
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadMessageProc);
					return ERR_SUCCESS;
				}
				else
				{
#ifdef CFW_MULTI_SIM
					UINT32 ErrorCode = Sim_ParseSW1SW2((UINT8)(pSimReadRecordCnf->Sw1), (UINT8)(pSimReadRecordCnf->Sw2), nSimID);

					CFW_GetUTI(hAo, &nUTI);
					CFW_PostNotifyEvent(EV_CFW_SIM_READ_MESSAGE_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xf0);
#else
					UINT32 ErrorCode = Sim_ParseSW1SW2((UINT8)(pSimReadRecordCnf->Sw1), (UINT8)(pSimReadRecordCnf->Sw2));

					CFW_GetUTI(hAo, &nUTI);
					CFW_PostNotifyEvent(EV_CFW_SIM_READ_MESSAGE_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error Read Sim SMS 0x%x\n"), ErrorCode);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadMessageProc);
					return ErrorCode;
				}

				// CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
			}
			else
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}
		}
		break;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadMessageProc);
	return ERR_SUCCESS;
}

UINT32 CFW_SimWriteMessageProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32 nEvtId, nUTI, ErrorCode;
	VOID* nEvParam = (VOID*)NULL;

	SIM_INFO_WRITESMS* pWriteSmsInfo;
	api_SimUpdateRecordCnf_t* pSimUpdateRecordCnf;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimWriteMessageProc);
	if ((UINT32)pEvent != 0xFFFFFFFF)
	{
		nEvtId   = pEvent->nEventId;
		nEvParam = (VOID*)pEvent->nParam1;
	}

#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif

	pWriteSmsInfo = CFW_GetAoUserData(hAo);

	switch (pWriteSmsInfo->n_CurrStatus)
	{
		case SMS_STATUS_IDLE:
		{
		  if ((UINT32)pEvent == 0xFFFFFFFF)
		  {
		    if (pWriteSmsInfo->nTryCount >= 0x01)
		    {
		      UINT32 nErrCode;

		      if (pWriteSmsInfo->nIndex == 0x00)
		      {
		        // Write a new sms;
#ifdef CFW_MULTI_SIM
		        nErrCode = SimSeekReq((UINT8)(pWriteSmsInfo->nCurrentFile),
		                              (UINT8)0x10, (UINT8)0x01, (pWriteSmsInfo->SeekPattern), nSimID);
#else
		        nErrCode = SimSeekReq((UINT8)(pWriteSmsInfo->nCurrentFile),
		                              (UINT8)0x10, (UINT8)0x01, (pWriteSmsInfo->SeekPattern));
#endif
		        if (ERR_SUCCESS != nErrCode)
		        {
		          CSW_TRACE(CFW_SIM_TS_ID, "Error SimSeekReq return 0x%x \n", nErrCode);
		          CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
		          CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xf0);
#else
		          CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
		          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
		          hAo = HNULL;
		          CSW_PROFILE_FUNCTION_EXIT(CFW_SimWriteMessageProc);
		          return nErrCode;
		        }

		        pWriteSmsInfo->n_PrevStatus = pWriteSmsInfo->n_CurrStatus;
		        pWriteSmsInfo->n_CurrStatus = SMS_STATUS_SEEKRECORD;

		        // pWriteSmsInfo->nTryCount --;
		      }
		      else
		      {
		        // Update the special sms
#ifdef CFW_MULTI_SIM
		        nErrCode =SimUpdateRecordReq((UINT8)(pWriteSmsInfo->nCurrentFile), 
		                                     (UINT8)(pWriteSmsInfo->nIndex), 0x04,SMS_PDU_LEN, pWriteSmsInfo->pData, nSimID);
#else
		        nErrCode = SimUpdateRecordReq((UINT8)(pWriteSmsInfo->nCurrentFile), 
		                                      (UINT8)(pWriteSmsInfo->nIndex), 0x04,SMS_PDU_LEN, pWriteSmsInfo->pData);
#endif
		        if (ERR_SUCCESS != nErrCode)
		        {
		          CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
		          CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xf0);
#else
		          CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
		          CSW_TRACE(CFW_SIM_TS_ID, "Error SimVerifyCHVReq return 0x%x \n", nErrCode);
		          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
		          hAo = HNULL;
		          CSW_PROFILE_FUNCTION_EXIT(CFW_SimWriteMessageProc);
		          return nErrCode;
		        }

		        pWriteSmsInfo->n_PrevStatus = pWriteSmsInfo->n_CurrStatus;
		        pWriteSmsInfo->n_CurrStatus = SMS_STATUS_UPDATERECORD;

		        // pWriteSmsInfo->nTryCount --;
		      }
		      CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
		    }
		    else
		    {
		      // Time out and TODO...
		      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
		      hAo = HNULL;
		    }

		    // CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
		  }
		  else
		    CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
		}
		break;
		case SMS_STATUS_SEEKRECORD:
		{
		  if (pEvent->nEventId == API_SIM_SEEK_CNF)
		  {
		    api_SimSeekCnf_t* pSimSeekCnf = (api_SimSeekCnf_t*)nEvParam;
        UINT32 nRecordID = 0x00;
#ifdef CFW_MULTI_SIM
        _SimSMSStatusArraySeek( &nRecordID,nSimID);
#else
        _SimSMSStatusArraySeek( &nRecordID);
#endif

		    if (((0x90 == pSimSeekCnf->Sw1) && (0x00 == pSimSeekCnf->Sw2)) || (0x9F == pSimSeekCnf->Sw1)
		        || (0x91 == pSimSeekCnf->Sw1) || (((0x94 == pSimSeekCnf->Sw1) && (0x04 == pSimSeekCnf->Sw2)) && nRecordID))
		    {
		      if (pWriteSmsInfo->nTryCount >= 0x01)
		      {
		        if (pWriteSmsInfo->nFlag == 0x01)
		        {
		          // 
		          // Update the record finished, and the cfw sms level 
		          // needs to get the Sms status(Full or not full).
		          // 

		          CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
		          CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP,
		                              (UINT32)(pWriteSmsInfo->nIndex), 0, nUTI | (nSimID << 24), 0x00);
#else
		          CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP, (UINT32)(pWriteSmsInfo->nIndex), 0, nUTI, 0x00);
#endif
		          CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_WRITE_MESSAGE_RSP Success! \n"));
		          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
		          hAo = HNULL;

		          // CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" ===================== 1 \n"));
		        }
		        else
		        {

					UINT32 nErrCode;

					pSimSeekCnf = (api_SimSeekCnf_t*)nEvParam;

					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" SIM seek Success! The record number %d\n"), pSimSeekCnf->RecordNb);
					pWriteSmsInfo->nIndex = pSimSeekCnf->RecordNb;

          if(((0x94 == pSimSeekCnf->Sw1) && (0x04 == pSimSeekCnf->Sw2)) && nRecordID)
          {
            pWriteSmsInfo->nIndex = (UINT8)nRecordID;
					  CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" nRecordID %d\n"), nRecordID);
          }         
#ifdef CFW_MULTI_SIM
					nErrCode = SimUpdateRecordReq((UINT8)(pWriteSmsInfo->nCurrentFile),
					                            (UINT8)(pWriteSmsInfo->nIndex), 0x04, SMS_PDU_LEN,pWriteSmsInfo->pData, nSimID);
#else
					nErrCode = SimUpdateRecordReq((UINT8)(pWriteSmsInfo->nCurrentFile),
					                            (UINT8)(pWriteSmsInfo->nIndex), 0x04, SMS_PDU_LEN, pWriteSmsInfo->pData);
#endif
					if (ERR_SUCCESS != nErrCode)
					{
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n", nErrCode);
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n", nErrCode);
						 CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL; 
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimWriteMessageProc); 
						return nErrCode;
					}

					pWriteSmsInfo->n_PrevStatus = pWriteSmsInfo->n_CurrStatus;
					pWriteSmsInfo->n_CurrStatus = SMS_STATUS_UPDATERECORD;

					// pWriteSmsInfo->nTryCount --;
					// CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" ===================== 2 \n"));
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);}
				}
				else
				{
					// Time out and TODO...
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo); hAo = HNULL;}
				}
				else if ((0x94 == pSimSeekCnf->Sw1) && (0x04 == pSimSeekCnf->Sw2))
				{
				// 
				// Maybe memory full
				// 
					if (pWriteSmsInfo->nFlag == 0x01)
					{
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP,
						(UINT32)(pWriteSmsInfo->nIndex), 1, nUTI | (nSimID << 24), 0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP,
						(UINT32)(pWriteSmsInfo->nIndex), 1, nUTI, 0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_WRITE_MESSAGE_RSP Success! \n"));
					}
					else
					{
						CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SIM seek record ERROR SW1 0x94 SW2 0x04\n"));
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP, (UINT32)ERR_CME_MEMORY_FULL,
						                  0, nUTI | (nSimID << 24), 0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP, (UINT32)ERR_CME_MEMORY_FULL,
						                  0, nUTI, 0xf0);
#endif
					}
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimWriteMessageProc); return ERR_CME_MEMORY_FULL;}
					else
					{
						// 
						// Failed for other reason
						// 
#ifdef CFW_MULTI_SIM
						ErrorCode = Sim_ParseSW1SW2(pSimSeekCnf->Sw1, pSimSeekCnf->Sw2, nSimID);
						CFW_GetUTI(hAo, &nUTI);
						CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP, (UINT32)ErrorCode,
						                  0, nUTI | (nSimID << 24), 0xf0);
#else
						ErrorCode = Sim_ParseSW1SW2(pSimSeekCnf->Sw1, pSimSeekCnf->Sw2);
						CFW_GetUTI(hAo, &nUTI);
						CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL; 
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimWriteMessageProc);
						return ErrorCode;
					}

		                          // CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
				}
				else
				{
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
				}
		}
		break; 
		case SMS_STATUS_UPDATERECORD:
		{
			if (pEvent->nEventId == API_SIM_UPDATERECORD_CNF)
			{
				pSimUpdateRecordCnf = (api_SimUpdateRecordCnf_t*)nEvParam;
				if (((0x90 == pSimUpdateRecordCnf->Sw1) && (00 == pSimUpdateRecordCnf->Sw2))
				|| (0x91 == pSimUpdateRecordCnf->Sw1))
				{
					if (pWriteSmsInfo->nTryCount >= 0x01)
					{
						if (pWriteSmsInfo->nReserveIndex == 0)
						{
							// 
							// 搜索是否有空的记录为下条存储。
							UINT32 nErrCode; pWriteSmsInfo->nFlag = 0x01;
#ifdef CFW_MULTI_SIM
							nErrCode =SimSeekReq((UINT8)(pWriteSmsInfo->nCurrentFile), (UINT8)0x10, (UINT8)0x01,
							(pWriteSmsInfo->SeekPattern), nSimID);
#else
							nErrCode =SimSeekReq((UINT8)(pWriteSmsInfo->nCurrentFile), (UINT8)0x10, (UINT8)0x01,
							(pWriteSmsInfo->SeekPattern)); if (ERR_SUCCESS != nErrCode)
#endif
							if (ERR_SUCCESS != nErrCode)
							{
								CSW_TRACE(CFW_SIM_TS_ID, "Error SimSeekReq return 0x%x \n", nErrCode);
								CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP, (UINT32)nErrCode,
								0, nUTI | (nSimID << 24), 0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
								CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
								hAo                         = HNULL; CSW_PROFILE_FUNCTION_EXIT(CFW_SimWriteMessageProc); return nErrCode;
							}
							pWriteSmsInfo->n_PrevStatus = pWriteSmsInfo->n_CurrStatus;
							pWriteSmsInfo->n_CurrStatus = SMS_STATUS_SEEKRECORD;

							// pWriteSmsInfo->nTryCount --;
							// CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" ===================== 6 \n"));
							CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
						}
						else
						{
						UINT8 nIndex = pWriteSmsInfo->nIndex; CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP, (UINT32)nIndex, 0,
						nUTI | (nSimID << 24), 0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP, (UINT32)nIndex, 0, nUTI, 0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_WRITE_MESSAGE_RSP Success! \n"));
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo); hAo = HNULL;

						// CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" ===================== 7 \n"));
						}
					}
					else
					{
						// Time out and TODO...
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo); hAo = HNULL;
					}
				}
			else
			{
#ifdef CFW_MULTI_SIM
				ErrorCode = Sim_ParseSW1SW2(pSimUpdateRecordCnf->Sw1, pSimUpdateRecordCnf->Sw2, nSimID);
				CFW_GetUTI(hAo, &nUTI);
				CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP, (UINT32)ErrorCode,
				                  0, nUTI | (nSimID << 24), 0xf0);
#else
				ErrorCode = Sim_ParseSW1SW2(pSimUpdateRecordCnf->Sw1, pSimUpdateRecordCnf->Sw2);
				CFW_GetUTI(hAo, &nUTI);
				CFW_PostNotifyEvent(EV_CFW_SIM_WRITE_MESSAGE_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
#endif
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SIM updata record 0x%x\n"), ErrorCode);
				CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo); hAo = HNULL;

				// CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" ===================== 8 \n"));
				CSW_PROFILE_FUNCTION_EXIT(CFW_SimWriteMessageProc); return ErrorCode;
			}

			// CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
		}
		else
		{
			CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);}
		}
		break;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_SimWriteMessageProc); 
	return ERR_SUCCESS;
}
#define SUPPORT_SR
UINT32 CFW_SimListMessageProc(HAO hAo, CFW_EV* pEvent) 
{
	UINT32 nEvtId, nUTI;
	VOID* nEvParam = (VOID*)NULL;
	SIM_INFO_LISTSMS * pListSmsInfo;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimListMessageProc);
	if ((UINT32)pEvent != 0xFFFFFFFF)
	{
		nEvtId = pEvent->nEventId; nEvParam = (VOID*)pEvent->nParam1;
	}
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID; nSimID = CFW_GetSimCardID(hAo);
#endif
	pListSmsInfo = CFW_GetAoUserData(hAo); 
	switch (pListSmsInfo->n_CurrStatus)
	{
		case SMS_STATUS_IDLE:
		{
			if ((UINT32)pEvent == 0xFFFFFFFF)
			{
				if (pListSmsInfo->nTryCount >= 0x01)
				{
					UINT32 nErrCode; 

          if (pListSmsInfo->nCount == 0)

					{
						pListSmsInfo->nCurrIndex = 0x01;
#ifdef CFW_MULTI_SIM
						nErrCode = SimReadRecordReq(pListSmsInfo->nCurrentFile,
													pListSmsInfo->nCurrIndex, 0x04, SMS_PDU_LEN, nSimID);
#else
						nErrCode = SimReadRecordReq(pListSmsInfo->nCurrentFile,
													pListSmsInfo->nCurrIndex, 0x04, SMS_PDU_LEN);
#endif
						if (ERR_SUCCESS != nErrCode)
						{
							CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", nErrCode);
							CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, (UINT32)nErrCode,
							0, nUTI | (nSimID << 24), 0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
							CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
							hAo = HNULL; 
							CSW_PROFILE_FUNCTION_EXIT(CFW_SimListMessageProc); return nErrCode;
						}
					}
					else
					{
#ifdef CFW_MULTI_SIM
						nErrCode = SimReadRecordReq(pListSmsInfo->nCurrentFile,
						(UINT8)(pListSmsInfo->nStartIndex), 0x04, SMS_PDU_LEN,nSimID);
#else
						nErrCode = SimReadRecordReq(pListSmsInfo->nCurrentFile,
						(UINT8)(pListSmsInfo->nStartIndex), 0x04, SMS_PDU_LEN);
#endif
						if (ERR_SUCCESS != nErrCode)
						{
							CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", nErrCode);
							CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
							CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
							hAo = HNULL;
							CSW_PROFILE_FUNCTION_EXIT(CFW_SimListMessageProc); return nErrCode;
						}
					}

					// CFW_SetAoState(hAo, CFW_SM_SIM_STATE_WRITE);

					pListSmsInfo->n_PrevStatus = pListSmsInfo->n_CurrStatus;
					pListSmsInfo->n_CurrStatus = SMS_STATUS_READRECORD;

					// pListSmsInfo->nTryCount --;
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
				}
				else
				{
					// Time out and TODO...
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo); hAo = HNULL;
				}

			}
			else
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
		}
		break; 
		case SMS_STATUS_READRECORD:
		{
			if (pEvent->nEventId == API_SIM_READRECORD_CNF)
			{
				if (pListSmsInfo->nTryCount >= 0x01)
				{
					api_SimReadRecordCnf_t* pSimReadRecordCnf = (api_SimReadRecordCnf_t*)nEvParam;
					if (((0x90 == pSimReadRecordCnf->Sw1) && (00 == pSimReadRecordCnf->Sw2))
					  || (0x91 == pSimReadRecordCnf->Sw1))
					{
#ifdef SUPPORT_SR
            UINT8   nStatus=0x0;
            UINT16  nPduRealSize=0x0; 
            UINT32  nPduOutData=0x0;
            UINT32 nRet = 0x00;
            CFW_SMS_MULTIPART_INFO sLongerMsg;

            //Zero memory
            SUL_ZeroMemory8(&sLongerMsg,SIZEOF(CFW_SMS_MULTIPART_INFO));
            //Just get the size ,status of each PDU
            nRet = sms_tool_DecomposePDU((PVOID)pSimReadRecordCnf->Data,8,&nPduOutData,&nStatus,&sLongerMsg);
            if(nRet == ERR_SUCCESS)
            {
                CSW_SIM_FREE((UINT8*)nPduOutData);
            }
            else// maybe Status report or EMS
            {
              if(nPduOutData)
                CSW_SIM_FREE((UINT8*)nPduOutData);
              pSimReadRecordCnf->Data[0] = 0x00;
            }
#endif

           
						UINT8* pGetSMSEntry = NULL; 
						UINT8 nState; 
						switch (pSimReadRecordCnf->Data[0])
						{
							case 0x03:
							nState = 0x01; break; case 0x01:
							nState = 0x02; break; case 0x07:
							nState = 0x04; break; case 0x05:
							nState = 0x08; break; case 0x0D:
							nState = 0x10; break; case 0x15:
							nState = 0x20; break; case 0x1D:
							nState = 0x40; break; case 0xA0:
							nState = 0x80; break; default:
							nState = pSimReadRecordCnf->Data[0];
						}
            //hal_HstSendEvent(nState | (pListSmsInfo->nCurrIndex<<8));
            if(nState)
              _SimSMSStatusArrayAdd(pListSmsInfo->nCurrIndex,nSimID);
            
						if (pListSmsInfo->nCount == 0x00)
						{
							// 
							// Get the sms storage info(only).
							// 
							UINT32 nErrCode; 
							if (((pListSmsInfo->nStatus) & CFW_SMS_STORED_STATUS_STORED_ALL) != 0x00)
							{
								pListSmsInfo->nCurrCount++;
							}
							else
							if (((pListSmsInfo->nStatus) & nState) != 0x00)
							{
								pListSmsInfo->nCurrCount++;
							}
							else
							{
							// Next do nothing
							}

							pListSmsInfo->nCurrIndex++;
#ifdef CFW_MULTI_SIM
							nErrCode = SimReadRecordReq(pListSmsInfo->nCurrentFile, pListSmsInfo->nCurrIndex, 0x04, SMS_PDU_LEN,nSimID);
#else
							nErrCode = SimReadRecordReq(pListSmsInfo->nCurrentFile, pListSmsInfo->nCurrIndex, 0x04, SMS_PDU_LEN);
#endif
							if (ERR_SUCCESS != nErrCode)
							{
								CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, (UINT32)nErrCode,
								                    0, nUTI | (nSimID << 24), 0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
								CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimReadRecordReq err,The ErrCode 0x%x\n"),nErrCode);
								CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo); hAo = HNULL;
								CSW_PROFILE_FUNCTION_EXIT(CFW_SimListMessageProc);
								return nErrCode;
							}
							pListSmsInfo->n_PrevStatus = pListSmsInfo->n_CurrStatus;
							pListSmsInfo->n_CurrStatus = SMS_STATUS_READRECORD;

							// pListSmsInfo->nTryCount --;    
						}
						else
						{
							if (((pListSmsInfo->nStatus) & CFW_SMS_STORED_STATUS_STORED_ALL) != 0x00)
							{
								pGetSMSEntry = (UINT8*)CSW_SIM_MALLOC(SMS_PDU_LEN); 
                if (pGetSMSEntry == NULL)
								{
									CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
									CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, (UINT32)ERR_NO_MORE_MEMORY,
									                  0, nUTI | (nSimID << 24), 0xf0);
#else
									CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, (UINT32)ERR_NO_MORE_MEMORY,
									                  0, nUTI, 0xf0);
#endif
									CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
									CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
									hAo = HNULL;
									CSW_PROFILE_FUNCTION_EXIT(CFW_SimListMessageProc);
									return ERR_NO_MORE_MEMORY;
								}
								SUL_MemCopy8(pGetSMSEntry, pSimReadRecordCnf->Data, SMS_PDU_LEN); CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, (UINT32)pGetSMSEntry,
								                  (UINT32)(pListSmsInfo->nCurrIndex), nUTI | (nSimID << 24), 0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, (UINT32)pGetSMSEntry,
								                  (UINT32)(pListSmsInfo->nCurrIndex), nUTI, 0);
#endif
								CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_LIST_MESSAGE_RSP Success! \n"));
								pListSmsInfo->nCurrCount++;
							}
							else if (((pListSmsInfo->nStatus) & nState) != 0x00)
							{
								pGetSMSEntry = (UINT8*)CSW_SIM_MALLOC(SMS_PDU_LEN);
								if (pGetSMSEntry == NULL)
								{
									CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
									CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, (UINT32)ERR_NO_MORE_MEMORY,
									                  0, nUTI | (nSimID << 24), 0xf0);
#else
									CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, (UINT32)ERR_NO_MORE_MEMORY,
									                  0, nUTI, 0xf0);
#endif
									CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
									CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
									hAo = HNULL;
									CSW_PROFILE_FUNCTION_EXIT(CFW_SimListMessageProc);
									return ERR_NO_MORE_MEMORY;
								}
								SUL_MemCopy8(pGetSMSEntry, pSimReadRecordCnf->Data, SMS_PDU_LEN); CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, (UINT32)pGetSMSEntry,
								                  (UINT32)(pListSmsInfo->nCurrIndex), nUTI | (nSimID << 24), 0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, (UINT32)pGetSMSEntry,
								                  (UINT32)(pListSmsInfo->nCurrIndex), nUTI, 0);
#endif
								CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_LIST_MESSAGE_RSP Success! \n"));
								pListSmsInfo->nCurrCount++;
							}
							else
							{
							// Next do nothing
							}
							if (pListSmsInfo->nCurrCount == pListSmsInfo->nCount)
							{
								CFW_GetUTI(hAo, &nUTI);

#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, 0, (0x01 << 16), nUTI | (nSimID << 24),
								                  0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, 0, (0x01 << 16), nUTI, 0);
#endif
								CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_LIST_MESSAGE_RSP Success! \n"));
								CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
								hAo = HNULL;
								CSW_PROFILE_FUNCTION_EXIT(CFW_SimListMessageProc);
								return ERR_SUCCESS;
							}
							else
							{
								UINT32 nErrCode; pListSmsInfo->nCurrIndex++;
#ifdef CFW_MULTI_SIM
								nErrCode = SimReadRecordReq(pListSmsInfo->nCurrentFile, pListSmsInfo->nCurrIndex, 0x04, SMS_PDU_LEN,nSimID);
#else
								nErrCode = SimReadRecordReq(pListSmsInfo->nCurrentFile, pListSmsInfo->nCurrIndex, 0x04, SMS_PDU_LEN);
#endif
								if (ERR_SUCCESS != nErrCode)
								{
									CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
									CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xf0);
#else
									CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
									CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimReadRecordReq err,The ErrCode %d\n"),
									            nErrCode);
									CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
									hAo = HNULL;
									CSW_PROFILE_FUNCTION_EXIT(CFW_SimListMessageProc);
									return nErrCode;
								}
								pListSmsInfo->n_PrevStatus = pListSmsInfo->n_CurrStatus;
								pListSmsInfo->n_CurrStatus = SMS_STATUS_READRECORD;

								// pListSmsInfo->nTryCount --;
								CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
							}
						}

					}
					else if ((0x94 == pSimReadRecordCnf->Sw1) && (0x02 == pSimReadRecordCnf->Sw2))
					{
						if (pListSmsInfo->nCount == 0x00)
						{
							CFW_GetUTI(hAo, &nUTI);

#ifdef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, 0, (pListSmsInfo->nCurrCount),
							                  nUTI | (nSimID << 24), 0);
#else
							CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, 0, (pListSmsInfo->nCurrCount), nUTI, 0);
#endif
							CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_LIST_MESSAGE_RSP Success! \n"));
							CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
							hAo = HNULL;
						}
						else
						{
							CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, 0, (0x01 << 16), nUTI | (nSimID << 24),
							                  0);
#else
							CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, 0, (0x01 << 16), nUTI, 0);
#endif
							CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_LIST_MESSAGE_RSP Success! \n"));
							CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
							hAo = HNULL;
						}
					}
					else
					{
#ifdef CFW_MULTI_SIM
						UINT32 ErrorCode =
						Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2, nSimID);
						CFW_GetUTI(hAo, &nUTI);

						CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, (UINT32)ErrorCode, 0,
						                  nUTI | (nSimID << 24), 0xf0);
#else
						UINT32 ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2);
						CFW_GetUTI(hAo, &nUTI);
						CFW_PostNotifyEvent(EV_CFW_SIM_LIST_MESSAGE_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SIM read listSMS ErrorCode 0x%x\n"), ErrorCode);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimListMessageProc);
						return ErrorCode;
					}
				}
				else
				{
					// Time out and TODO...
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo); hAo = HNULL;
				}

				// CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
			}
			else
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}
		}
		break;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_SimListMessageProc); 
	return ERR_SUCCESS;
}

UINT32 CFW_SimDeleMessageProc(HAO hAo, CFW_EV* pEvent) 
{
	UINT32 nEvtId, nUTI;
	VOID* nEvParam = (VOID*)NULL;
	SIM_INFO_DELESMS * pDeleteSmsInfo;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimDeleMessageProc);
	if ((UINT32)pEvent != 0xFFFFFFFF)
	{
		nEvtId = pEvent->nEventId;
		nEvParam = (VOID*)pEvent->nParam1;
	}
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif
	pDeleteSmsInfo = CFW_GetAoUserData(hAo);
	switch (pDeleteSmsInfo->n_CurrStatus)
	{
		case SMS_STATUS_IDLE:
		{
			if ((UINT32)pEvent == 0xFFFFFFFF)
			{
				if (pDeleteSmsInfo->nTryCount >= 0x01)
				{
					if (pDeleteSmsInfo->nIndex == 0)  // delete special type
					{
						if (pDeleteSmsInfo->nStatus == CFW_SMS_STORED_STATUS_STORED_ALL)
						{
#ifdef CFW_MULTI_SIM
							UINT32 nErrCode =
							SimUpdateRecordReq(pDeleteSmsInfo->nCurrentFile, pDeleteSmsInfo->nCurrInd, 0x04,
										       SMS_PDU_LEN, pDeleteSmsInfo->pData, nSimID );
              _SimSMSStatusArrayRemove( pDeleteSmsInfo->nCurrInd , nSimID);
#else
							UINT32 nErrCode =
							SimUpdateRecordReq(pDeleteSmsInfo->nCurrentFile, pDeleteSmsInfo->nCurrInd, 0x04,
									           SMS_PDU_LEN, pDeleteSmsInfo->pData);
              _SimSMSStatusArrayRemove( pDeleteSmsInfo->nCurrInd);

#endif
                if (ERR_SUCCESS != nErrCode)
                {
                  CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n", nErrCode);
                  CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
                  CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xf0);
#else
                  CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
                  hAo = HNULL;
								  CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeleMessageProc);
								  return nErrCode;
							  }

                pDeleteSmsInfo->n_PrevStatus = pDeleteSmsInfo->n_CurrStatus;
                pDeleteSmsInfo->n_CurrStatus = SMS_STATUS_UPDATERECORD;

                              // pDeleteSmsInfo->nTryCount --;
						}
						else
						{
#ifdef CFW_MULTI_SIM
							UINT32 nErrCode =
							SimReadRecordReq(pDeleteSmsInfo->nCurrentFile, pDeleteSmsInfo->nCurrInd, 0x04,
											 SMS_PDU_LEN, nSimID);
#else
							UINT32 nErrCode =
							SimReadRecordReq(pDeleteSmsInfo->nCurrentFile, pDeleteSmsInfo->nCurrInd, 0x04,
											 SMS_PDU_LEN);
#endif
							if (ERR_SUCCESS != nErrCode)
							{
								CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)nErrCode,
													0, nUTI | (nSimID << 24), 0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
								CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", nErrCode);
								CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
								hAo = HNULL;
								CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeleMessageProc);
								return nErrCode;
							}

							pDeleteSmsInfo->n_PrevStatus = pDeleteSmsInfo->n_CurrStatus;
							pDeleteSmsInfo->n_CurrStatus = SMS_STATUS_READRECORD;

							// pDeleteSmsInfo->nTryCount --;
						}
						CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
					}
					else  // delete special record sms
					{
#ifdef CFW_MULTI_SIM
						UINT32 nErrCode =
						SimUpdateRecordReq(pDeleteSmsInfo->nCurrentFile, (UINT8)pDeleteSmsInfo->nIndex, 0x04,
										   SMS_PDU_LEN, pDeleteSmsInfo->pData, nSimID);
            _SimSMSStatusArrayRemove( pDeleteSmsInfo->nIndex , nSimID );

#else
						UINT32 nErrCode =
						SimUpdateRecordReq(pDeleteSmsInfo->nCurrentFile, (UINT8)pDeleteSmsInfo->nIndex, 0x04,
										   SMS_PDU_LEN, pDeleteSmsInfo->pData); 
            _SimSMSStatusArrayRemove( pDeleteSmsInfo->nIndex );

#endif
						if (ERR_SUCCESS != nErrCode)
						{
							CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)nErrCode,
												0, nUTI | (nSimID << 24), 0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
							CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n", nErrCode);
							CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
							hAo = HNULL;
							CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeleMessageProc);
							return nErrCode;
						}
						pDeleteSmsInfo->n_PrevStatus = pDeleteSmsInfo->n_CurrStatus;
						pDeleteSmsInfo->n_CurrStatus = SMS_STATUS_UPDATERECORD;

						// pDeleteSmsInfo->nTryCount --;
						CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
					}
				}
				else
				{
					// Time out and TODO...
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
			}
			else
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
		}
		break;
		case SMS_STATUS_READRECORD:
		{
			if (pEvent->nEventId == API_SIM_READRECORD_CNF)
			{
				if (pDeleteSmsInfo->nTryCount >= 0x01)
				{
					api_SimReadRecordCnf_t* pSimReadRecordCnf = (api_SimReadRecordCnf_t*)nEvParam;
					if (((0x90 == pSimReadRecordCnf->Sw1) && (00 == pSimReadRecordCnf->Sw2))
						|| (0x91 == pSimReadRecordCnf->Sw1))
					{
						UINT8 nState; switch (pSimReadRecordCnf->Data[0])

						{
							case 0x03:
							nState = 0x01; break; case 0x01:
							nState = 0x02; break; case 0x07:
							nState = 0x04; break; case 0x05:
							nState = 0x08; break; case 0x0D:
							nState = 0x10; break; case 0x15:
							nState = 0x20; break; case 0x1D:
							nState = 0x40; break; case 0xA0:
							nState = 0x80; break; default:
							nState = pSimReadRecordCnf->Data[0];
						}
						if (((pDeleteSmsInfo->nStatus) & CFW_SMS_STORED_STATUS_STORED_ALL) != 0x00)
						{
#ifdef CFW_MULTI_SIM
							UINT32 nErrCode =
							SimUpdateRecordReq(pDeleteSmsInfo->nCurrentFile, pDeleteSmsInfo->nCurrInd, 0x04,
											   SMS_PDU_LEN, pDeleteSmsInfo->pData, nSimID);
            _SimSMSStatusArrayRemove( pDeleteSmsInfo->nCurrInd , nSimID );

#else
							UINT32 nErrCode =
							SimUpdateRecordReq(pDeleteSmsInfo->nCurrentFile, pDeleteSmsInfo->nCurrInd, 0x04,
								               SMS_PDU_LEN, pDeleteSmsInfo->pData);
            _SimSMSStatusArrayRemove( pDeleteSmsInfo->nCurrInd );
#endif
							if (ERR_SUCCESS != nErrCode)
							{
								CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)nErrCode,
								0, nUTI | (nSimID << 24), 0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
								CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n", nErrCode);
								CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
								hAo = HNULL;
								CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeleMessageProc);
								return nErrCode;
							}

							pDeleteSmsInfo->n_PrevStatus = pDeleteSmsInfo->n_CurrStatus;
							pDeleteSmsInfo->n_CurrStatus = SMS_STATUS_UPDATERECORD;

							// pDeleteSmsInfo->nTryCount --;
						}
						else if (((pDeleteSmsInfo->nStatus) & nState) != 0x00)
						{
#ifdef CFW_MULTI_SIM
							UINT32 nErrCode =
							SimUpdateRecordReq(pDeleteSmsInfo->nCurrentFile, pDeleteSmsInfo->nCurrInd, 0x04,
											   SMS_PDU_LEN, pDeleteSmsInfo->pData, nSimID);
            _SimSMSStatusArrayRemove( pDeleteSmsInfo->nCurrInd , nSimID );

#else
							UINT32 nErrCode =
							SimUpdateRecordReq(pDeleteSmsInfo->nCurrentFile, pDeleteSmsInfo->nCurrInd, 0x04,
									           SMS_PDU_LEN, pDeleteSmsInfo->pData);
            _SimSMSStatusArrayRemove( pDeleteSmsInfo->nCurrInd );

#endif
							if (ERR_SUCCESS != nErrCode)
							{
								CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)nErrCode,
													0, nUTI | (nSimID << 24), 0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
								CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimUpdateRecordReq ErrCode 0x%x\n"), nErrCode);
								CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
								hAo = HNULL;
								CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeleMessageProc);
								return nErrCode;
							}

							pDeleteSmsInfo->n_PrevStatus = pDeleteSmsInfo->n_CurrStatus;
							pDeleteSmsInfo->n_CurrStatus = SMS_STATUS_UPDATERECORD;

							// pDeleteSmsInfo->nTryCount --;
						}
						else
						{
							// 
							// read
							// 
							pDeleteSmsInfo->nCurrInd++;
#ifdef CFW_MULTI_SIM
							UINT32 nErrCode =
							SimReadRecordReq(pDeleteSmsInfo->nCurrentFile, pDeleteSmsInfo->nCurrInd, 0x04,
								             SMS_PDU_LEN, nSimID);
#else
							UINT32 nErrCode =
							SimReadRecordReq(pDeleteSmsInfo->nCurrentFile, pDeleteSmsInfo->nCurrInd, 0x04,
								             SMS_PDU_LEN);
#endif
							if (ERR_SUCCESS != nErrCode)
							{
								CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)nErrCode,
													0, nUTI | (nSimID << 24), 0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
								CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimReadRecordReq ErrCode 0x%x\n"), nErrCode);
								CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
								hAo = HNULL;
								CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeleMessageProc);
								return nErrCode;
							}

							pDeleteSmsInfo->n_PrevStatus = pDeleteSmsInfo->n_CurrStatus;
							pDeleteSmsInfo->n_CurrStatus = SMS_STATUS_READRECORD;

							// pDeleteSmsInfo->nTryCount --;    
						}
						CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
					}
					else if ((0x94 == pSimReadRecordCnf->Sw1) && (02 == pSimReadRecordCnf->Sw2))
					{
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)((pDeleteSmsInfo->pattern[0])),
							                1, nUTI | (nSimID << 24), 0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)((pDeleteSmsInfo->pattern[0])),
						                    1, nUTI, 0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" CFW_SIM_DELETE_MESSAGE Finished! \n"));
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
					}
					else
					{

						CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error delete sms failed the index is %d \n"),
						pDeleteSmsInfo->nCurrInd);
						CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error delete sms failed Sw1 0x%x Sw2 0x%x \n"),
						pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2);
#ifdef CFW_MULTI_SIM
						UINT32 ErrorCode =
						Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2, nSimID);

						CFW_GetUTI(hAo, &nUTI);
						CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)ErrorCode, 0,
											nUTI | (nSimID << 24), 0xf0);
#else
						UINT32 ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2);
						CFW_GetUTI(hAo, &nUTI);
						CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
#endif
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
					}

				}
				else
				{
					// Time out and TODO...
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo); hAo = HNULL;
				}
			}
			else
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}
		}
		break;
		case SMS_STATUS_UPDATERECORD:
		{
			if (pEvent->nEventId == API_SIM_UPDATERECORD_CNF)
			{
				api_SimUpdateRecordCnf_t* pSimUpdateRecordCnf = (api_SimUpdateRecordCnf_t*)nEvParam;
				if (((0x90 == pSimUpdateRecordCnf->Sw1) && (00 == pSimUpdateRecordCnf->Sw2))
				|| (0x91 == pSimUpdateRecordCnf->Sw1))
				{
					if (pDeleteSmsInfo->nIndex == 0)
					{
						pDeleteSmsInfo->pattern[0]++;
						if (((pDeleteSmsInfo->nStatus) & CFW_SMS_STORED_STATUS_STORED_ALL) != 0x00)
						{
							CSW_TRACE(CFW_SIM_TS_ID, "Current Delete SMS's index %d\n", pDeleteSmsInfo->nCurrInd);
							pDeleteSmsInfo->nCurrInd++;
#ifdef CFW_MULTI_SIM
							UINT32 nErrCode =
							SimUpdateRecordReq(pDeleteSmsInfo->nCurrentFile, pDeleteSmsInfo->nCurrInd, 0x04,
											   SMS_PDU_LEN, pDeleteSmsInfo->pData, nSimID);
            _SimSMSStatusArrayRemove( pDeleteSmsInfo->nCurrInd , nSimID );

#else
							UINT32 nErrCode =
							SimUpdateRecordReq(pDeleteSmsInfo->nCurrentFile, pDeleteSmsInfo->nCurrInd, 0x04,
											   SMS_PDU_LEN, pDeleteSmsInfo->pData);
            _SimSMSStatusArrayRemove( pDeleteSmsInfo->nCurrInd );

#endif
							if (ERR_SUCCESS != nErrCode)
							{
								CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)nErrCode,
												    0, nUTI | (nSimID << 24), 0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
								CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimUpdateRecordReq ErrCode 0x%x\n"), nErrCode);
								CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
								hAo = HNULL;
								CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeleMessageProc);
								return nErrCode;
							}
							pDeleteSmsInfo->n_PrevStatus = pDeleteSmsInfo->n_CurrStatus;
							pDeleteSmsInfo->n_CurrStatus = SMS_STATUS_UPDATERECORD;
						}
						else
						{
							CSW_TRACE(CFW_SIM_TS_ID, "Current Delete SMS's index is %d\n",
							pDeleteSmsInfo->nCurrInd); pDeleteSmsInfo->nCurrInd++;
#ifdef CFW_MULTI_SIM
							UINT32 nErrCode =
							SimReadRecordReq(pDeleteSmsInfo->nCurrentFile, pDeleteSmsInfo->nCurrInd, 0x04,
											 SMS_PDU_LEN, nSimID);
#else
							UINT32 nErrCode =
							SimReadRecordReq(pDeleteSmsInfo->nCurrentFile, pDeleteSmsInfo->nCurrInd, 0x04,
											 SMS_PDU_LEN);
#endif
							if (ERR_SUCCESS != nErrCode)
							{
								CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)nErrCode,
								0, nUTI | (nSimID << 24), 0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
								CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimReadRecordReq ErrCode 0x%x\n"), nErrCode);
								CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
								hAo = HNULL; CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeleMessageProc); 
								return nErrCode;
							}
							pDeleteSmsInfo->n_PrevStatus = pDeleteSmsInfo->n_CurrStatus;
							pDeleteSmsInfo->n_CurrStatus = SMS_STATUS_READRECORD;
						}
					}
					else
					{
						// Delete the special record sms success!
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)0, 0, nUTI | (nSimID << 24),0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)0, 0, nUTI, 0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_DELETE_MESSAGE_RSP Success! \n"));
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo); hAo = HNULL;
					}
				}
				else if ((0x94 == pSimUpdateRecordCnf->Sw1) && (02 == pSimUpdateRecordCnf->Sw2))
				{
					CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)((pDeleteSmsInfo->pattern[0])),
										1, nUTI | (nSimID << 24), 0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)((pDeleteSmsInfo->pattern[0])),
						                1, nUTI, 0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" CFW_SIM_DELETE_MESSAGE Finished! \n"));
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
				else
				{
#ifdef CFW_MULTI_SIM
					UINT32 ErrorCode =
					Sim_ParseSW1SW2(pSimUpdateRecordCnf->Sw1, pSimUpdateRecordCnf->Sw2, nSimID);

					CFW_GetUTI(hAo, &nUTI);
					CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)ErrorCode, 0,
										nUTI | (nSimID << 24), 0xf0);
#else
					UINT32 ErrorCode = Sim_ParseSW1SW2(pSimUpdateRecordCnf->Sw1, pSimUpdateRecordCnf->Sw2);
					CFW_GetUTI(hAo, &nUTI);
					CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_MESSAGE_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeleMessageProc);
					return ErrorCode;
				}
			}
			else
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}
		}
		break;
	}
	return ERR_SUCCESS;
}

UINT32 CFW_SimReadBinaryProc(HAO hAo, CFW_EV* pEvent) 
{
	UINT32 nEvtId, nUTI, ErrorCode;
	VOID* nEvParam = (VOID*)NULL;
	SIM_INFO_READBINARY * pReadBinary;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimReadBinaryProc);
	CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadBinaryProc); 
	
	if ((UINT32)pEvent != 0xFFFFFFFF)
	{
		nEvtId = pEvent->nEventId;
		nEvParam = (VOID*)pEvent->nParam1;
	}
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif
	pReadBinary = CFW_GetAoUserData(hAo);
	switch (pReadBinary->n_CurrStatus)
	{
		case SMS_STATUS_IDLE:
		{
			if ((UINT32)pEvent == 0xFFFFFFFF)
			{
				if (pReadBinary->nTryCount >= 0x01)
				{
#ifdef CFW_MULTI_SIM
					UINT32 nErrCode =
					SimReadBinaryReq(pReadBinary->nFileId, pReadBinary->nOffset, pReadBinary->nBytesToRead,nSimID);
#else
					UINT32 nErrCode =
					SimReadBinaryReq(pReadBinary->nFileId, pReadBinary->nOffset, pReadBinary->nBytesToRead);
#endif
					if (ERR_SUCCESS != nErrCode)
					{
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_READ_BINARY_RSP, (UINT32)nErrCode,
											0, nUTI | (nSimID << 24), 0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_READ_BINARY_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n", nErrCode);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadBinaryProc);
						return nErrCode;
					}

					pReadBinary->n_PrevStatus = pReadBinary->n_CurrStatus;
					pReadBinary->n_CurrStatus = SMS_STATUS_READBINARY;

					// pReadBinary->nTryCount --;
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
				}
				else
				{
					// Time out and TODO...
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo); hAo = HNULL;
				}
			}
			else
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
		}
		break;
		case SMS_STATUS_READBINARY:
		{
			if (pEvent->nEventId == API_SIM_READBINARY_CNF)
			{
				api_SimReadBinaryCnf_t* pSimReadBinaryCnf = (api_SimReadBinaryCnf_t*)nEvParam;
				if (((0x90 == pSimReadBinaryCnf->Sw1) && (00 == pSimReadBinaryCnf->Sw2))
				|| (0x91 == pSimReadBinaryCnf->Sw1))
				{
					UINT8* pReadBinary_R = (UINT8*)CSW_SIM_MALLOC(pReadBinary->nBytesToRead);
					UINT32 P2; if (pReadBinary_R == NULL)
					{
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_READ_BINARY_RSP, (UINT32)ERR_NO_MORE_MEMORY,
											0, nUTI | (nSimID << 24), 0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_READ_BINARY_RSP, (UINT32)ERR_NO_MORE_MEMORY,
											0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadBinaryProc);
						return ERR_NO_MORE_MEMORY;
					}
					P2  = pReadBinary->nBytesToRead << 16;
					P2 += pReadBinary->nFileId;
					SUL_MemCopy8(pReadBinary_R, pSimReadBinaryCnf->Data, pReadBinary->nBytesToRead);
					CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_READ_BINARY_RSP, (UINT32)pReadBinary_R, P2,
										nUTI | (nSimID << 24), 0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_READ_BINARY_RSP, (UINT32)pReadBinary_R, P2, nUTI, 0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_READ_BINARY_RSP Success! \n"));
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
				else
				{
					CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
					ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2, nSimID);
					CFW_PostNotifyEvent(EV_CFW_SIM_READ_BINARY_RSP, (UINT32)ErrorCode, 0,
										nUTI | (nSimID << 24), 0xf0);
#else
					ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2);
					CFW_PostNotifyEvent(EV_CFW_SIM_READ_BINARY_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
#endif
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
					hAo = HNULL;
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadBinaryProc);
					return ErrorCode;
				}

				// CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
			}
			else
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}
		}
		break;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadBinaryProc);
	return ERR_SUCCESS;
}

UINT32 CFW_SimUpdateBinaryProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32 nEvtId, nUTI, ErrorCode;
	VOID* nEvParam = (VOID*)NULL;
	SIM_INFO_UPDATEBINARY * pUpdateBinary;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimUpdateBinaryProc);
	
	if ((UINT32)pEvent != 0xFFFFFFFF)
	{
		nEvtId = pEvent->nEventId;
		nEvParam = (VOID*)pEvent->nParam1;
	}
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif
	pUpdateBinary = CFW_GetAoUserData(hAo);
	switch (pUpdateBinary->n_CurrStatus)
	{
		case SMS_STATUS_IDLE:
		{
			if ((UINT32)pEvent == 0xFFFFFFFF)
			{
				if (pUpdateBinary->nTryCount >= 0x01)
				{
#ifdef CFW_MULTI_SIM
					UINT32 nErrCode =
					SimUpdateBinaryReq(pUpdateBinary->nFileId, pUpdateBinary->nOffset,
									   pUpdateBinary->nBytesToWrite, pUpdateBinary->pDate, nSimID);
#else
					UINT32 nErrCode =
					SimUpdateBinaryReq(pUpdateBinary->nFileId, pUpdateBinary->nOffset,
									   pUpdateBinary->nBytesToWrite, pUpdateBinary->pDate);
#endif
					if (ERR_SUCCESS != nErrCode)
					{
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_UPDATE_BINARY_RSP, (UINT32)nErrCode,
											0, nUTI | (nSimID << 24), 0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_UPDATE_BINARY_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateBinaryReq return 0x%x \n", nErrCode);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimUpdateBinaryProc);
						return nErrCode;
					}

					pUpdateBinary->n_PrevStatus = pUpdateBinary->n_CurrStatus;
					pUpdateBinary->n_CurrStatus = SMS_STATUS_UPDATEBINARY;

					// pUpdateBinary->nTryCount --;
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
				}
				else
				{
					// Time out and TODO...
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}

			}
			else
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
		}
		break;
		case SMS_STATUS_UPDATEBINARY:
		{
			if (pEvent->nEventId == API_SIM_UPDATEBINARY_CNF)
			{
				api_SimUpdateBinaryCnf_t* pSimUpdateBinaryCnf = (api_SimUpdateBinaryCnf_t*)nEvParam;
				if (((0x90 == pSimUpdateBinaryCnf->Sw1) && (00 == pSimUpdateBinaryCnf->Sw2))
				|| (0x91 == pSimUpdateBinaryCnf->Sw1))
				{
					UINT8 nFileId = pUpdateBinary->nFileId; CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_UPDATE_BINARY_RSP, (UINT32)0, nFileId,
										nUTI | (nSimID << 24), 0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_UPDATE_BINARY_RSP, (UINT32)0, nFileId, nUTI, 0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_UPDATE_BINARY_RSP Success! \n"));
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
				else
				{
					CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
					ErrorCode = Sim_ParseSW1SW2(pSimUpdateBinaryCnf->Sw1, pSimUpdateBinaryCnf->Sw2, nSimID);
					CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
					CFW_PostNotifyEvent(EV_CFW_SIM_UPDATE_BINARY_RSP, (UINT32)ErrorCode, 0,
					nUTI | (nSimID << 24), 0xf0);
#else
					ErrorCode = Sim_ParseSW1SW2(pSimUpdateBinaryCnf->Sw1, pSimUpdateBinaryCnf->Sw2);
					CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
					CFW_PostNotifyEvent(EV_CFW_SIM_UPDATE_BINARY_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
#  endif
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimUpdateBinaryProc);
					return ErrorCode;
				}

			}
			else
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}

		}
		break;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_SimUpdateBinaryProc);
	return ERR_SUCCESS;
}

UINT32 CFW_SimGetMRProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32 nEvtId, nUTI, ErrorCode;
	VOID* nEvParam = (VOID*)NULL;
	SIM_INFO_GET_MR * pGetMR;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetMRProc);

	if ((UINT32)pEvent != 0xFFFFFFFF)
	{
		nEvtId = pEvent->nEventId;
		nEvParam = (VOID*)pEvent->nParam1;
	}
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif

	pGetMR = CFW_GetAoUserData(hAo);
	switch (pGetMR->n_CurrStatus)
	{
		case SMS_STATUS_IDLE:
		{
			if ((UINT32)pEvent == 0xFFFFFFFF)
			{
				if (pGetMR->nTryCount >= 0x01)
				{
#ifdef CFW_MULTI_SIM
					UINT32 nErrCode = SimReadBinaryReq(pGetMR->nFileId, 0, 2, nSimID);
#else
					UINT32 nErrCode = SimReadBinaryReq(pGetMR->nFileId, 0, 2);
#endif
					if (ERR_SUCCESS != nErrCode)
					{
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_MR_RSP, (UINT32)nErrCode,
											0, nUTI | (nSimID << 24), 0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_MR_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", nErrCode);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetMRProc);
						return nErrCode;
					}

					pGetMR->n_PrevStatus = pGetMR->n_CurrStatus;
					pGetMR->n_CurrStatus = SMS_STATUS_READBINARY;

					// pGetMR->nTryCount --;
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
				}
				else
				{
					// Time out and TODO...
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}

			}
			else
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
		}
		break;
		case SMS_STATUS_READBINARY:
		{
			if (pEvent->nEventId == API_SIM_READBINARY_CNF)
			{
				api_SimReadBinaryCnf_t* pSimReadBinaryCnf = (api_SimReadBinaryCnf_t*)nEvParam;
				if (((0x90 == pSimReadBinaryCnf->Sw1) && (00 == pSimReadBinaryCnf->Sw2))
				|| (0x91 == pSimReadBinaryCnf->Sw1))
				{
					CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_MR_RSP, (UINT32)pSimReadBinaryCnf->Data[0], 0,
										nUTI | (nSimID << 24), 0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_MR_RSP, (UINT32)pSimReadBinaryCnf->Data[0], 0, nUTI,0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_GET_MR_RSP Success! \n"));
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
				else
				{
					CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
					ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2, nSimID);
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_MR_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24),
					0xf0);
#else
					ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2);
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_MR_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetMRProc);
					return ErrorCode;
				}

			}
			else
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}

		}
		break;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetMRProc);
	return ERR_SUCCESS;
}

UINT32 CFW_SimSetMRProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32 nEvtId, nUTI;
	VOID* nEvParam = (VOID*)NULL;
	SIM_INFO_SET_MR * pSetMR;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimSetMRProc);

	if ((UINT32)pEvent != 0xFFFFFFFF)
	{
	nEvtId = pEvent->nEventId;
	nEvParam = (VOID*)pEvent->nParam1;
	}
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID; 
	nSimID = CFW_GetSimCardID(hAo);
#endif
	pSetMR = CFW_GetAoUserData(hAo);
	switch (pSetMR->n_CurrStatus)
	{
		case SMS_STATUS_IDLE:
		{
			if ((UINT32)pEvent == 0xFFFFFFFF)
			{
				if (pSetMR->nTryCount >= 0x01)
				{
#ifdef CFW_MULTI_SIM
					UINT32 nErrCode = SimUpdateBinaryReq(pSetMR->nFileId, 0, 1, &(pSetMR->nMR), nSimID);
#else
					UINT32 nErrCode = SimUpdateBinaryReq(pSetMR->nFileId, 0, 1, &(pSetMR->nMR));
#endif
					if (ERR_SUCCESS != nErrCode)
					{
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_MR_RSP, (UINT32)nErrCode,
											0, nUTI | (nSimID << 24), 0xf0);
#else
						CFW_GetUTI(hAo, &nUTI);
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_MR_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateBinaryReq return 0x%x \n", nErrCode);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetMRProc);
						return nErrCode;
					}

					pSetMR->n_PrevStatus = pSetMR->n_CurrStatus;
					pSetMR->n_CurrStatus = SMS_STATUS_UPDATEBINARY;

					// pSetMR->nTryCount --;
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
				}
				else
				{
					// Time out and TODO...
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
			}
			else
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
		}
		break;
		case SMS_STATUS_UPDATEBINARY:
		{
			if (pEvent->nEventId == API_SIM_UPDATEBINARY_CNF)
			{
				api_SimUpdateBinaryCnf_t* pSimUpdateBinaryCnf = (api_SimUpdateBinaryCnf_t*)nEvParam;
				if (((0x90 == pSimUpdateBinaryCnf->Sw1) && (00 == pSimUpdateBinaryCnf->Sw2))
				|| (0x91 == pSimUpdateBinaryCnf->Sw1))
				{
					CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_SET_MR_RSP, (UINT32)0, 0, nUTI | (nSimID << 24), 0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_SET_MR_RSP, (UINT32)0, 0, nUTI, 0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_SET_MR_RSP Success! \n"));
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
				else
				{
					UINT32 ErrorCode;
					CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
					ErrorCode = Sim_ParseSW1SW2(pSimUpdateBinaryCnf->Sw1, pSimUpdateBinaryCnf->Sw2, nSimID);
					CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
					CFW_PostNotifyEvent(EV_CFW_SIM_SET_MR_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24),
					0xf0);
#else
					ErrorCode = Sim_ParseSW1SW2(pSimUpdateBinaryCnf->Sw1, pSimUpdateBinaryCnf->Sw2);
					CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
					CFW_PostNotifyEvent(EV_CFW_SIM_SET_MR_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_SET_MR_RSP Success! \n"));
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetMRProc);
					return ErrorCode;
				}
			}
			else
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}
		}
		break;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetMRProc);
	return ERR_SUCCESS;
}

UINT32 CFW_SimGetSmsTotalNumProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32 nEvtId, nUTI, ErrorCode;
	VOID* nEvParam = (VOID*)NULL;
	SIM_INFO_GETSMSINFO * pGetSmsInfo;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetSmsTotalNumProc);

	if ((UINT32)pEvent != 0xFFFFFFFF)
	{
		nEvtId = pEvent->nEventId;
		nEvParam = (VOID*)pEvent->nParam1;
	}
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID; 
	nSimID = CFW_GetSimCardID(hAo);
#endif
	pGetSmsInfo = CFW_GetAoUserData(hAo); 
	switch (pGetSmsInfo->n_CurrStatus)
	{
		case SMS_STATUS_IDLE:
		{
			if ((UINT32)pEvent == 0xFFFFFFFF)
			{
				if (pGetSmsInfo->nTryCount >= 0x01)
				{
#ifdef CFW_MULTI_SIM
					UINT32 nErrCode = SimElemFileStatusReq(pGetSmsInfo->nCurrentFile, nSimID);
#else
					UINT32 nErrCode = SimElemFileStatusReq(pGetSmsInfo->nCurrentFile);
#endif
					if (ERR_SUCCESS != nErrCode)
					{
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_TOTAL_NUM_RSP, (UINT32)nErrCode,
											0, nUTI | (nSimID << 24), 0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_TOTAL_NUM_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", nErrCode);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetSmsTotalNumProc);
						return nErrCode;
					}
					pGetSmsInfo->n_PrevStatus = pGetSmsInfo->n_CurrStatus;
					pGetSmsInfo->n_CurrStatus = SMS_STATUS_ELEMFILE;

					// pGetSmsInfo->nTryCount --;
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
				}
				else
				{
					// Time out and TODO...
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo); hAo = HNULL;
				}
			}
			else
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
		}
		break;
		case SMS_STATUS_ELEMFILE:
		{
			if (pEvent->nEventId == API_SIM_ELEMFILESTATUS_CNF)
			{
				UINT16 TotalBytes; 
				UINT32 TotalSMS;
				//UINT32 nErrCode;
				api_SimElemFileStatusCnf_t* pSimElemFileStatusCnf = (api_SimElemFileStatusCnf_t*)nEvParam;
				if (((0x90 == pSimElemFileStatusCnf->Sw1) && (00 == pSimElemFileStatusCnf->Sw2))
				|| (0x91 == pSimElemFileStatusCnf->Sw1))
				{
					SUL_MemCopy8(pGetSmsInfo->pattern, pSimElemFileStatusCnf->Data + 2, 2);
					TotalBytes = 0x00;
					TotalBytes = pGetSmsInfo->pattern[0];
					TotalBytes <<= 8;
					TotalBytes &= 0xFF00;
					TotalBytes += pGetSmsInfo->pattern[1]; 
          TotalSMS = TotalBytes / SMS_PDU_LEN;
          hal_HstSendEvent(0x1000000);
          hal_HstSendEvent(TotalSMS);
					CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
          _SimSMSStatusArrayInit(TotalSMS,nSimID);

					CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_TOTAL_NUM_RSP, (UINT32)TotalSMS, 0,
										nUTI | (nSimID << 24), 0);
#else
          _SimSMSStatusArrayInit(TotalSMS);

					CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_TOTAL_NUM_RSP, (UINT32)TotalSMS, 0, nUTI, 0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post ERR_CME_SIM_FAILURE Success! \n"));
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;

				}
				else
				{
					CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
					ErrorCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2, nSimID);
					CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_TOTAL_NUM_RSP, (UINT32)ErrorCode, 0,
					nUTI | (nSimID << 24), 0xf0);
#else
					ErrorCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2);
					CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_TOTAL_NUM_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
#endif
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetSmsTotalNumProc);
					return ErrorCode;
				}
			}
			else
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}

		}
		break; 

	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetSmsTotalNumProc);
	return ERR_SUCCESS;
}

UINT32 CFW_SimGetSmsStorageInfoProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32 nEvtId, nUTI, ErrorCode;
	VOID* nEvParam = (VOID*)NULL;
	SIM_INFO_GETSMSINFO * pGetSmsInfo;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetSmsStorageInfoProc);

	if ((UINT32)pEvent != 0xFFFFFFFF)
	{
		nEvtId = pEvent->nEventId;
		nEvParam = (VOID*)pEvent->nParam1;
	}
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID; 
	nSimID = CFW_GetSimCardID(hAo);
#endif
	pGetSmsInfo = CFW_GetAoUserData(hAo); 
	switch (pGetSmsInfo->n_CurrStatus)
	{
		case SMS_STATUS_IDLE:
		{
			if ((UINT32)pEvent == 0xFFFFFFFF)
			{
				if (pGetSmsInfo->nTryCount >= 0x01)
				{
#ifdef CFW_MULTI_SIM
					UINT32 nErrCode = SimElemFileStatusReq(pGetSmsInfo->nCurrentFile, nSimID);
#else
					UINT32 nErrCode = SimElemFileStatusReq(pGetSmsInfo->nCurrentFile);
#endif
					if (ERR_SUCCESS != nErrCode)
					{
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP, (UINT32)nErrCode,
											0, nUTI | (nSimID << 24), 0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", nErrCode);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetSmsStorageInfoProc);
						return nErrCode;
					}
					pGetSmsInfo->n_PrevStatus = pGetSmsInfo->n_CurrStatus;
					pGetSmsInfo->n_CurrStatus = SMS_STATUS_ELEMFILE;

					// pGetSmsInfo->nTryCount --;
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
				}
				else
				{
					// Time out and TODO...
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo); hAo = HNULL;
				}
			}
			else
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
		}
		break;
		case SMS_STATUS_ELEMFILE:
		{
			if (pEvent->nEventId == API_SIM_ELEMFILESTATUS_CNF)
			{
				UINT16 TotalBytes;
				UINT32 TotalSMS;
				UINT32 nErrCode;
				api_SimElemFileStatusCnf_t* pSimElemFileStatusCnf = (api_SimElemFileStatusCnf_t*)nEvParam;
				if (((0x90 == pSimElemFileStatusCnf->Sw1) && (00 == pSimElemFileStatusCnf->Sw2))
				|| (0x91 == pSimElemFileStatusCnf->Sw1))
				{
					SUL_MemCopy8(pGetSmsInfo->pattern, pSimElemFileStatusCnf->Data + 2, 2);
					TotalBytes = 0x00;
					TotalBytes = pGetSmsInfo->pattern[0];
					TotalBytes <<= 8;
					TotalBytes &= 0xFF00;
					TotalBytes += pGetSmsInfo->pattern[1]; TotalSMS = TotalBytes / SMS_PDU_LEN;

					// TotalSMS   <<=16;
					if (pGetSmsInfo->nCurrentFile == API_SIM_EF_SMS)
					{
						pGetSmsInfo->nTotalNum = (UINT16)TotalSMS;
					}
					else
					{
						// TODO..
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP, (UINT32)0, 0,
											nUTI | (nSimID << 24), 0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP, (UINT32)0, 0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post ERR_CME_SIM_FAILURE Success! \n"));
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
					}

					if (pGetSmsInfo->nStatus == 0xA0)
					{
						pGetSmsInfo->pattern[0] = 0x00;
					}
					else
					{
						pGetSmsInfo->pattern[0] = pGetSmsInfo->nStatus;
					}
#ifdef CFW_MULTI_SIM
					nErrCode =
					SimReadRecordReq(pGetSmsInfo->nCurrentFile, pGetSmsInfo->nCurrInd, 0x04, SMS_PDU_LEN,nSimID);
#else
					nErrCode =
					SimReadRecordReq(pGetSmsInfo->nCurrentFile, pGetSmsInfo->nCurrInd, 0x04, SMS_PDU_LEN);
#endif
					if (ERR_SUCCESS != nErrCode)
					{
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP, (UINT32)nErrCode,
						0, nUTI | (nSimID << 24), 0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", nErrCode);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetSmsStorageInfoProc);
						return nErrCode;
					}
					pGetSmsInfo->n_PrevStatus = pGetSmsInfo->n_CurrStatus;
					pGetSmsInfo->n_CurrStatus = SMS_STATUS_READRECORD;

					// pGetSmsInfo->nTryCount --;
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
				}
				else
				{
					CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP, (UINT32)0, 0,
										nUTI | (nSimID << 24), 0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP, (UINT32)0, 0, nUTI, 0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post ERR_CME_SIM_FAILURE Success! \n"));
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
			}
			else
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}

		}
		break; 
		case SMS_STATUS_READRECORD:
		{
			if (pEvent->nEventId == API_SIM_READRECORD_CNF)
			{
				if (pGetSmsInfo->nTryCount >= 0x01)
				{
					api_SimReadRecordCnf_t* pSimReadRecordCnf = (api_SimReadRecordCnf_t*)nEvParam;
					if (((0x90 == pSimReadRecordCnf->Sw1) && (00 == pSimReadRecordCnf->Sw2))
					|| (0x91 == pSimReadRecordCnf->Sw1))
					{
						if (pGetSmsInfo->pattern[0] == pSimReadRecordCnf->Data[0])
						{
							// 
							// update the number and read
							// 
							UINT32 nErrCode; pGetSmsInfo->nSpecialNum++; pGetSmsInfo->nCurrInd++;
#ifdef CFW_MULTI_SIM
							nErrCode =
							SimReadRecordReq(pGetSmsInfo->nCurrentFile, pGetSmsInfo->nCurrInd, 0x04, SMS_PDU_LEN,nSimID); 
#else
							nErrCode =
							SimReadRecordReq(pGetSmsInfo->nCurrentFile, pGetSmsInfo->nCurrInd, 0x04, SMS_PDU_LEN);
#endif
							if (ERR_SUCCESS != nErrCode)
							{
								CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP, (UINT32)nErrCode,
													0, nUTI | (nSimID << 24), 0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
								CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", nErrCode);
								CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
								hAo = HNULL;
								CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetSmsStorageInfoProc);
								return nErrCode;
							}
						}
						else
						{
							// read
							UINT32 nErrCode; pGetSmsInfo->nCurrInd++;
#ifdef CFW_MULTI_SIM
							nErrCode =
							SimReadRecordReq(pGetSmsInfo->nCurrentFile, pGetSmsInfo->nCurrInd, 0x04, SMS_PDU_LEN,nSimID);
#else
							nErrCode =
							SimReadRecordReq(pGetSmsInfo->nCurrentFile, pGetSmsInfo->nCurrInd, 0x04, SMS_PDU_LEN);
#endif
							if (ERR_SUCCESS != nErrCode)
							{
								CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP, (UINT32)nErrCode,
								0, nUTI | (nSimID << 24), 0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
								CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", nErrCode);
								CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
								hAo = HNULL;
								CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetSmsStorageInfoProc);
								return nErrCode;
							}
						}
						pGetSmsInfo->n_PrevStatus = pGetSmsInfo->n_CurrStatus;
						pGetSmsInfo->n_CurrStatus = SMS_STATUS_READRECORD;

							// pGetSmsInfo->nTryCount --;
						CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
					}
					else if ((0x94 == pSimReadRecordCnf->Sw1) && (02 == pSimReadRecordCnf->Sw2))
					{
						UINT32 SMSStorageInfo[2];
						SMSStorageInfo[0] = pGetSmsInfo->nSpecialNum + ((pGetSmsInfo->nTotalNum) << 16);
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP, (UINT32)(SMSStorageInfo[0]),
											pGetSmsInfo->nStatus, nUTI | (nSimID << 24), 0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP, (UINT32)(SMSStorageInfo[0]),
											pGetSmsInfo->nStatus, nUTI, 0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID,TSTXT(" post EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP. Success! \n"));
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo); hAo = HNULL;
					}
					else
					{
#ifdef CFW_MULTI_SIM
						ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2, nSimID);
						CFW_GetUTI(hAo, &nUTI);
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP, (UINT32)ErrorCode,
											0, nUTI | (nSimID << 24), 0xf0);
#else
						ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2);
						CFW_GetUTI(hAo, &nUTI);
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP, (UINT32)ErrorCode,
											0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetSmsStorageInfoProc);
						return ErrorCode;
					}
				}
				else
				{
					// Time out and TODO...
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
			}
			else
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}
		}
		break;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetSmsStorageInfoProc);
	return ERR_SUCCESS;
}

UINT32 CFW_SimGetSmsParametersProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32 nEvtId, nUTI, ErrorCode = 0;
	VOID* nEvParam = (VOID*)NULL;
	SIM_INFO_GetSMSP * pGetSmsParam;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetSmsParametersProc);

	if ((UINT32)pEvent != 0xFFFFFFFF)
	{
		nEvtId = pEvent->nEventId;
		nEvParam = (VOID*)pEvent->nParam1;
	}
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif
	pGetSmsParam = CFW_GetAoUserData(hAo);
	switch (pGetSmsParam->n_CurrStatus)
	{
		case SMS_STATUS_IDLE:
		{
			if ((UINT32)pEvent == 0xFFFFFFFF)
			{
				if (pGetSmsParam->nTryCount >= 0x01)
				{
#ifdef CFW_MULTI_SIM
					UINT32 nErrCode = SimElemFileStatusReq(pGetSmsParam->nFileId, nSimID);
#else
					UINT32 nErrCode = SimElemFileStatusReq(pGetSmsParam->nFileId);
#endif
					if (ERR_SUCCESS != nErrCode)
					{
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)nErrCode,
											0, nUTI | (nSimID << 24), 0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", nErrCode);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetSmsParametersProc);
						return nErrCode;
					}

					CFW_SetAoState(hAo, CFW_SM_SIM_STATE_READ);
					pGetSmsParam->n_PrevStatus = pGetSmsParam->n_CurrStatus;
					pGetSmsParam->n_CurrStatus = SMS_STATUS_ELEMFILE;

					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
				}
				else
				{
					// Time out and TODO...
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
			}
			else
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
		}
		break;
		case SMS_STATUS_ELEMFILE:
		{
			if (pEvent->nEventId == API_SIM_ELEMFILESTATUS_CNF)
			{
				UINT16 TotalBytes; UINT32 nErrCode;

				// UINT32 TotalSMS;
				api_SimElemFileStatusCnf_t* pSimElemFileStatusCnf = (api_SimElemFileStatusCnf_t*)nEvParam;
				if (((0x90 == pSimElemFileStatusCnf->Sw1) && (00 == pSimElemFileStatusCnf->Sw2))
				|| (0x91 == pSimElemFileStatusCnf->Sw1))
				{
					SUL_MemCopy8(pGetSmsParam->pattern, pSimElemFileStatusCnf->Data + 2, 2);
					TotalBytes = 0x00;
					TotalBytes = pGetSmsParam->pattern[0];
					TotalBytes <<= 8;
					TotalBytes &= 0xFF00;
					TotalBytes += pGetSmsParam->pattern[1];
					pGetSmsParam->nTotalRecordNumber = TotalBytes / pSimElemFileStatusCnf->Data[14];
					pGetSmsParam->RecordLength       = pSimElemFileStatusCnf->Data[14];
					if (pGetSmsParam->nIndex == 0x00)
					{
						// 
						// List all the SCA nunmber;
						// 
						pGetSmsParam->pNode =
						(CFW_SIM_SMS_PARAMETERS*)CSW_SIM_MALLOC(pGetSmsParam->nTotalRecordNumber *sizeof(CFW_SIM_SMS_PARAMETERS));
						if ((pGetSmsParam->pNode) == NULL)
						{
							CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)ERR_NO_MORE_MEMORY,
												0, nUTI | (nSimID << 24), 0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)ERR_NO_MORE_MEMORY,
												0, nUTI, 0xf0);
#endif
							CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
							CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
							hAo = HNULL;
							CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetSmsParametersProc);
							return ERR_NO_MORE_MEMORY;
						}
#ifdef CFW_MULTI_SIM
						nErrCode =
						SimReadRecordReq(API_SIM_EF_SMSP, pGetSmsParam->nCurrRecordNumber, 0x04,
										 pGetSmsParam->RecordLength, nSimID);
#else
						nErrCode =
						SimReadRecordReq(API_SIM_EF_SMSP, pGetSmsParam->nCurrRecordNumber, 0x04,
										 pGetSmsParam->RecordLength);
#endif
						if (ERR_SUCCESS != nErrCode)
						{
							CSW_SIM_FREE(pGetSmsParam->pNode);
							CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)nErrCode,
												0, nUTI | (nSimID << 24), 0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
							CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", nErrCode);
							CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
							hAo = HNULL;
							CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetSmsParametersProc);
							return ERR_NO_MORE_MEMORY;
						}

						pGetSmsParam->n_PrevStatus = pGetSmsParam->n_CurrStatus;
						pGetSmsParam->n_CurrStatus = SMS_STATUS_READRECORD;
						CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);

					}
					else if (pGetSmsParam->nIndex == 0xFF)
					{
						// Get the total number of SCA
						UINT8 nTmp = pGetSmsParam->nTotalRecordNumber; CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)0, nTmp,
											nUTI | (nSimID << 24), 0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)0, nTmp, nUTI, 0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post ERR_CME_SIM_FAILURE Success! \n"));
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
					}
					else
					{
						// Get the special SCA

#ifdef CFW_MULTI_SIM
						nErrCode =
						SimReadRecordReq(API_SIM_EF_SMSP, pGetSmsParam->nIndex, 0x04, pGetSmsParam->RecordLength,
						nSimID);
#else
						nErrCode =
						SimReadRecordReq(API_SIM_EF_SMSP, pGetSmsParam->nIndex, 0x04, pGetSmsParam->RecordLength);
#endif
						if (ERR_SUCCESS != nErrCode)
						{
							CSW_SIM_FREE(pGetSmsParam->pNode); CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)nErrCode,
												0, nUTI | (nSimID << 24), 0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
							CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimReadRecordReq The ErrCode is %d\n"),nErrCode); 
							CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
							hAo = HNULL;
							CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetSmsParametersProc);
							return nErrCode;
						}
						pGetSmsParam->n_PrevStatus = pGetSmsParam->n_CurrStatus;
						pGetSmsParam->n_CurrStatus = SMS_STATUS_READRECORD;

						// pGetSmsParam->nTryCount --;
						CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
					}
				}
				else
				{
#ifdef CFW_MULTI_SIM
					ErrorCode =
					Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2, nSimID);
					CFW_GetUTI(hAo, &nUTI);
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)ErrorCode, 0,
										nUTI | (nSimID << 24), 0xf0);
#else
					ErrorCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2);
					CFW_GetUTI(hAo, &nUTI);
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL; CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetSmsParametersProc);
					return ErrorCode;
				}
			}
			else
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}
		}
		break; 
		case SMS_STATUS_READRECORD:
		{
			if (pEvent->nEventId == API_SIM_READRECORD_CNF)
			{
				if (pGetSmsParam->nTryCount >= 0x01)
				{
					api_SimReadRecordCnf_t* pSimReadRecordCnf = (api_SimReadRecordCnf_t*)nEvParam;
					if (((0x90 == pSimReadRecordCnf->Sw1) && (00 == pSimReadRecordCnf->Sw2))
					|| (0x91 == pSimReadRecordCnf->Sw1))
					{
						UINT8 nTmp;
						nTmp = pGetSmsParam->RecordLength - 0x1c; if (pGetSmsParam->nIndex == 0x00)
						{
							// List all the Parameters;
							SUL_MemCopy8(pGetSmsParam->pNode[pGetSmsParam->nCurrRecordNumber - 1].nSMSCInfo,
							&(pSimReadRecordCnf->Data[nTmp + 13]), 12);
							pGetSmsParam->pNode[pGetSmsParam->nCurrRecordNumber - 1].nPID =
							pSimReadRecordCnf->Data[nTmp + 25];
							pGetSmsParam->pNode[pGetSmsParam->nCurrRecordNumber - 1].nDCS =
							pSimReadRecordCnf->Data[nTmp + 26];
							pGetSmsParam->pNode[pGetSmsParam->nCurrRecordNumber - 1].nVP =
							pSimReadRecordCnf->Data[nTmp + 27];
							if (pGetSmsParam->nCurrRecordNumber == pGetSmsParam->nTotalRecordNumber)
							{
								nTmp = pGetSmsParam->nTotalRecordNumber;
								CFW_SIM_SMS_PARAMETERS* pPostNode =
								(CFW_SIM_SMS_PARAMETERS*)CSW_SIM_MALLOC(nTmp * sizeof(CFW_SIM_SMS_PARAMETERS));
								if (pPostNode == NULL)
								{
									CSW_SIM_FREE(pGetSmsParam->pNode); CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
									CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)ERR_NO_MORE_MEMORY,
														0, nUTI | (nSimID << 24), 0xf0);
#else
									CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)ERR_NO_MORE_MEMORY,
														0, nUTI, 0xf0);
#endif
									CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
									CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
									hAo = HNULL;
									CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetSmsParametersProc);
									return ERR_NO_MORE_MEMORY;
								}
								SUL_MemCopy8(pPostNode, pGetSmsParam->pNode, nTmp * sizeof(CFW_SIM_SMS_PARAMETERS));
								CSW_SIM_FREE(pGetSmsParam->pNode);
								CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)(pPostNode), nTmp,
													nUTI | (nSimID << 24), 0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)(pPostNode), nTmp, nUTI,0);
#endif
								CSW_TRACE(CFW_SIM_TS_ID,
								TSTXT(" post EV_CFW_SIM_GET_SMS_PARAMETERS_RSP Success! \n"));
								CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
								hAo = HNULL;
							}
							else
							{
								pGetSmsParam->nCurrRecordNumber++;
#ifdef CFW_MULTI_SIM
								SimReadRecordReq(API_SIM_EF_SMSP, pGetSmsParam->nCurrRecordNumber, 0x04,
								pGetSmsParam->RecordLength, nSimID);
#else
								SimReadRecordReq(API_SIM_EF_SMSP, pGetSmsParam->nCurrRecordNumber, 0x04,
								pGetSmsParam->RecordLength);
#endif
								pGetSmsParam->n_PrevStatus = pGetSmsParam->n_CurrStatus;
								pGetSmsParam->n_CurrStatus = SMS_STATUS_READRECORD;

								// pGetSmsParam->nTryCount --;
								CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
							}
						}
						else if (pGetSmsParam->nIndex == 0xff)
						{
							CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)(0), 0,
							nUTI | (nSimID << 24), 0);
#else
							CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)(0), 0, nUTI, 0);
#endif
							CSW_TRACE(CFW_SIM_TS_ID,
							TSTXT(" post EV_CFW_SIM_GET_SMS_PARAMETERS_RSP Success! \n"));
							CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
							hAo = HNULL;
						}
						else
						{
							// Get the special Parameter;
							CFW_SIM_SMS_PARAMETERS* pPostNode =
							(CFW_SIM_SMS_PARAMETERS*)CSW_SIM_MALLOC(sizeof(CFW_SIM_SMS_PARAMETERS));
							if (pPostNode == NULL)
							{
								// CSW_SIM_FREE(pGetSmsParam->pNode);
								CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)ERR_NO_MORE_MEMORY,
													0, nUTI | (nSimID << 24), 0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)ERR_NO_MORE_MEMORY,
													0, nUTI, 0xf0);
#endif
								CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
								CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
								hAo = HNULL;
								CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetSmsParametersProc);
								return ERR_NO_MORE_MEMORY;
							}
							SUL_MemCopy8(pPostNode->nSMSCInfo, &(pSimReadRecordCnf->Data[nTmp + 13]), 12);
							pPostNode->nPID = pSimReadRecordCnf->Data[nTmp + 25];
							pPostNode->nDCS = pSimReadRecordCnf->Data[nTmp + 26];
							pPostNode->nVP  = pSimReadRecordCnf->Data[nTmp + 27];
							nTmp            = pGetSmsParam->nCurrRecordNumber; CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)(pPostNode), nTmp,
												nUTI | (nSimID << 24), 0);
#else
							CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)(pPostNode), nTmp, nUTI,0);
#endif
							CSW_TRACE(CFW_SIM_TS_ID,
							TSTXT(" post EV_CFW_SIM_GET_SMS_PARAMETERS_RSP Success! \n"));
							CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
							hAo = HNULL;
						}
					}
					else
					{
#ifdef CFW_MULTI_SIM
						ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2, nSimID);
						CFW_GetUTI(hAo, &nUTI);
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)ErrorCode,
						0, nUTI | (nSimID << 24), 0xf0);
#else
						ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2);
						CFW_GetUTI(hAo, &nUTI);
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_SMS_PARAMETERS_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetSmsParametersProc);
						return ErrorCode;
					}
				}
				else
				{
					// Time out and TODO...
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
			}
			else
			{
			CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}
		}
		break;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetSmsParametersProc);
	return ERR_SUCCESS;
}

UINT32 CFW_SimSetSmsParametersProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32 nEvtId, nUTI, ErrorCode;
	VOID* nEvParam = (VOID*)NULL;
	SIM_INFO_SETSMSP * pSetSmsParam;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimSetSmsParametersProc); 

	if ((UINT32)pEvent != 0xFFFFFFFF)
	{
		nEvtId = pEvent->nEventId;
		nEvParam = (VOID*)pEvent->nParam1;
	}
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif

	pSetSmsParam = CFW_GetAoUserData(hAo); switch (pSetSmsParam->n_CurrStatus)
	{
		case SMS_STATUS_IDLE:
		{
			if ((UINT32)pEvent == 0xFFFFFFFF)
			{
				if (pSetSmsParam->nTryCount >= 0x01)
				{
#ifdef CFW_MULTI_SIM
					UINT32 nErrCode = SimElemFileStatusReq(pSetSmsParam->nFileId, nSimID);
#else
					UINT32 nErrCode = SimElemFileStatusReq(pSetSmsParam->nFileId);
#endif
					if (ERR_SUCCESS != nErrCode)
					{
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)nErrCode,
											0, nUTI | (nSimID << 24), 0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n", nErrCode);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetSmsParametersProc);
						return nErrCode;
					}

					CFW_SetAoState(hAo, CFW_SM_SIM_STATE_READ);
					pSetSmsParam->n_PrevStatus = pSetSmsParam->n_CurrStatus;
					pSetSmsParam->n_CurrStatus = SMS_STATUS_ELEMFILE;

					// pSetSmsParam->nTryCount --;
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
				}
				else
				{
					// Time out and TODO...
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
			}
			else
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
		}
		break;
		case SMS_STATUS_ELEMFILE:
		{
			if (pEvent->nEventId == API_SIM_ELEMFILESTATUS_CNF)
			{
				UINT16 TotalBytes;

				// UINT32 TotalSMS;
				api_SimElemFileStatusCnf_t* pSimElemFileStatusCnf = (api_SimElemFileStatusCnf_t*)nEvParam;
				if (((0x90 == pSimElemFileStatusCnf->Sw1) && (00 == pSimElemFileStatusCnf->Sw2))
				|| (0x91 == pSimElemFileStatusCnf->Sw1))
				{
					SUL_MemCopy8(pSetSmsParam->pattern, pSimElemFileStatusCnf->Data + 2, 2);
					TotalBytes = 0x00;
					TotalBytes = pSetSmsParam->pattern[0];
					TotalBytes <<= 8;
					TotalBytes &= 0xFF00;
					TotalBytes += pSetSmsParam->pattern[1];
					pSetSmsParam->nTotalRecordNumber = TotalBytes / pSimElemFileStatusCnf->Data[14];
					pSetSmsParam->RecordLength       = pSimElemFileStatusCnf->Data[14];
					if (pSetSmsParam->nIndex == 0x00)
					{
						// 
						// Set the parameters in batch ;
						// 
						if (pSetSmsParam->nTotalRecordNumber < pSetSmsParam->nArrayCount)
						{
							// 
							// The count number is bigger than the total number of record
							// 
							CFW_GetUTI(hAo, &nUTI);
							CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)0, 0, nUTI, 0);
							CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post ERR_CME_SIM_FAILURE Success! \n"));
							CSW_TRACE(CFW_SIM_TS_ID,
							TSTXT(" Error,nArrayCount is bigger then total record number!. \n"));
							CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
							hAo = HNULL;
						}
						else
						{
#ifdef CFW_MULTI_SIM
						UINT32 nErrCode =
						SimReadRecordReq(API_SIM_EF_SMSP, pSetSmsParam->nCurrRecordNumber, 0x04,
										 pSetSmsParam->RecordLength, nSimID);
#else
						UINT32 nErrCode =
						SimReadRecordReq(API_SIM_EF_SMSP, pSetSmsParam->nCurrRecordNumber, 0x04,
										 pSetSmsParam->RecordLength);
#endif
							if (ERR_SUCCESS != nErrCode)
							{
								CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)nErrCode,
													0, nUTI | (nSimID << 24), 0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
								CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimReadRecordReq The ErrCode is %d\n"),nErrCode);
								CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
								hAo = HNULL;
								CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetSmsParametersProc);
								return nErrCode;
							}

							pSetSmsParam->n_PrevStatus = pSetSmsParam->n_CurrStatus;
							pSetSmsParam->n_CurrStatus = SMS_STATUS_READRECORD;
						}
					}
					else
					{
				// Set the special record parameter
#ifdef CFW_MULTI_SIM
						UINT32 nErrCode =
						SimReadRecordReq(API_SIM_EF_SMSP, pSetSmsParam->nIndex, 0x04, pSetSmsParam->RecordLength,nSimID);
#else
						UINT32 nErrCode =
						SimReadRecordReq(API_SIM_EF_SMSP, pSetSmsParam->nIndex, 0x04, pSetSmsParam->RecordLength);
#endif
						if (ERR_SUCCESS != nErrCode)
						{
							CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)nErrCode,
							0, nUTI | (nSimID << 24), 0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
							CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimReadRecordReq The ErrCode is %d\n"),nErrCode); 
							CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
							hAo = HNULL;
							CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetSmsParametersProc);
							return nErrCode;
						}

						pSetSmsParam->n_PrevStatus = pSetSmsParam->n_CurrStatus;
						pSetSmsParam->n_CurrStatus = SMS_STATUS_READRECORD;

						// pSetSmsParam->nTryCount --;
						CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
					}
				}
				else
				{
					CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)0, 0,
										nUTI | (nSimID << 24), 0xf0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)0, 0, nUTI, 0xf0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID,
					TSTXT(" post EV_CFW_SIM_SET_SMS_PARAMETERS_RSP Success! \n"));
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
			}
			else
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}
		}
		break;
		case SMS_STATUS_READRECORD:
		{
			if (pEvent->nEventId == API_SIM_READRECORD_CNF)
			{
				if (pSetSmsParam->nTryCount >= 0x01)
				{
					api_SimReadRecordCnf_t* pSimReadRecordCnf = (api_SimReadRecordCnf_t*)nEvParam;
					if (((0x90 == pSimReadRecordCnf->Sw1) && (00 == pSimReadRecordCnf->Sw2))
					|| (0x91 == pSimReadRecordCnf->Sw1))
					{
						UINT8 nTmp; UINT8* pTmp = (UINT8*)CSW_SIM_MALLOC(pSetSmsParam->RecordLength);

						// CFW_SIM_SMS_PARAMETERS **pR = NULL;    
						if (pTmp == NULL)
						{
							CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)ERR_NO_MORE_MEMORY,
												0, nUTI | (nSimID << 24), 0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)ERR_NO_MORE_MEMORY,
												0, nUTI, 0xf0);
#endif
							CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
							CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
							hAo = HNULL;
							CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetSmsParametersProc);
							return ERR_NO_MORE_MEMORY;
						}
						SUL_ZeroMemory8(pTmp, pSetSmsParam->RecordLength);
						SUL_MemCopy8(pTmp, pSimReadRecordCnf->Data, pSetSmsParam->RecordLength);
						nTmp = pSetSmsParam->RecordLength - 0x1c;
						if (pSetSmsParam->nIndex == 0x00)
						{
							// Set the parameters in batch
							UINT32 nErrCode;
							UINT8 nCRN = pSetSmsParam->nCurrRecordNumber;
							if ((pSetSmsParam->Node[nCRN - 1].nFlag & 0x01) != 0x00)
							{
								SUL_MemCopy8(&(pTmp[nTmp + 13]), pSetSmsParam->Node[nCRN - 1].nSMSCInfo, 12);
							}

							if ((pSetSmsParam->Node[nCRN - 1].nFlag & 0x02) != 0x00)
							{
								pTmp[nTmp + 25] = pSetSmsParam->Node[nCRN - 1].nPID;
							}

							if ((pSetSmsParam->Node[nCRN - 1].nFlag & 0x04) != 0x00)
							{
								pTmp[nTmp + 26] = pSetSmsParam->Node[nCRN - 1].nDCS;
							}

							if ((pSetSmsParam->Node[nCRN - 1].nFlag & 0x08) != 0x00)
							{
								pTmp[nTmp + 27] = pSetSmsParam->Node[nCRN - 1].nVP;
							}
#ifdef CFW_MULTI_SIM
							nErrCode =
							SimUpdateRecordReq(pSetSmsParam->nFileId, nCRN, 0x04, pSetSmsParam->RecordLength, pTmp,nSimID);
#else
							nErrCode =
							SimUpdateRecordReq(pSetSmsParam->nFileId, nCRN, 0x04, pSetSmsParam->RecordLength, pTmp);
#endif
							if (ERR_SUCCESS != nErrCode)
							{
								CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)nErrCode,
													0, nUTI | (nSimID << 24), 0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
								CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n", nErrCode);
								CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
								hAo = HNULL;
								CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetSmsParametersProc);
								return nErrCode;
							}

							pSetSmsParam->n_PrevStatus = pSetSmsParam->n_CurrStatus;
							pSetSmsParam->n_CurrStatus = SMS_STATUS_UPDATERECORD;

							CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
						}
						else
						{
							// Set the special record parameter
							UINT32 nErrCode;
							if ((pSetSmsParam->Node[0].nFlag & 0x01) != 0x00)
							{
								SUL_MemCopy8(&(pTmp[nTmp + 13]), pSetSmsParam->Node[0].nSMSCInfo, 12);
							}

							if ((pSetSmsParam->Node[0].nFlag & 0x02) != 0x00)
							{
								pTmp[nTmp + 25] = pSetSmsParam->Node[0].nPID;
							}

							if ((pSetSmsParam->Node[0].nFlag & 0x04) != 0x00)
							{
								pTmp[nTmp + 26] = pSetSmsParam->Node[0].nDCS;
							}

							if ((pSetSmsParam->Node[0].nFlag & 0x08) != 0x00)
							{
								pTmp[nTmp + 27] = pSetSmsParam->Node[0].nVP;
							}
#ifdef CFW_MULTI_SIM
							nErrCode =
							SimUpdateRecordReq(pSetSmsParam->nFileId, pSetSmsParam->nIndex, 0x04,
											   pSetSmsParam->RecordLength, pTmp, nSimID);
#else
							nErrCode =
							SimUpdateRecordReq(pSetSmsParam->nFileId, pSetSmsParam->nIndex, 0x04,
											   pSetSmsParam->RecordLength, pTmp);
#endif
							if (ERR_SUCCESS != nErrCode)
							{

								CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)nErrCode,
								0, nUTI | (nSimID << 24), 0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#  endif
								CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n", nErrCode);
								CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
								hAo = HNULL;
								CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetSmsParametersProc);
								return nErrCode;
							}

							pSetSmsParam->n_PrevStatus = pSetSmsParam->n_CurrStatus;
							pSetSmsParam->n_CurrStatus = SMS_STATUS_UPDATERECORD;

							// pSetSmsParam->nTryCount --;
							CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
						}

					}
					else
					{
#ifdef CFW_MULTI_SIM
						ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2, nSimID);
						CFW_GetUTI(hAo, &nUTI);
						CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)ErrorCode,
											0, nUTI | (nSimID << 24), 0xf0);
#else
						ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2);
						CFW_GetUTI(hAo, &nUTI);
						CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetSmsParametersProc);
						return ErrorCode;
					}
				}
				else
				{
					// Time out and TODO...
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
			}
			else
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}
		}
		break; 
		case SMS_STATUS_UPDATERECORD:
		{
			if (pSetSmsParam->nIndex == 0x00)
			{
				// Set the parameters in batch
				if (pSetSmsParam->nCurrRecordNumber < pSetSmsParam->nArrayCount)
				{
					UINT32 nErrCode; pSetSmsParam->nCurrRecordNumber++;
#ifdef CFW_MULTI_SIM
					nErrCode =
					SimReadRecordReq(API_SIM_EF_SMSP, pSetSmsParam->nCurrRecordNumber, 0x04,
									 pSetSmsParam->RecordLength, nSimID);
#else
					nErrCode =
					SimReadRecordReq(API_SIM_EF_SMSP, pSetSmsParam->nCurrRecordNumber, 0x04,
									 pSetSmsParam->RecordLength); 
#endif
					if (ERR_SUCCESS != nErrCode)
					{
						CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)nErrCode,
											0, nUTI | (nSimID << 24), 0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)nErrCode, 0, nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", nErrCode);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetSmsParametersProc);
						return nErrCode;
					}
				}
				else
				{
					CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)0, 0,
					nUTI | (nSimID << 24), 0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)0, 0, nUTI, 0);
#endif
					CSW_TRACE(33, TSTXT(" post EV_CFW_SIM_SET_SMS_PARAMETERS_RSP Success! \n"));
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
			}
			else
			{
				// Set the special record parameter
				CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)0, 0,
									nUTI | (nSimID << 24), 0);
#else
				CFW_PostNotifyEvent(EV_CFW_SIM_SET_SMS_PARAMETERS_RSP, (UINT32)0, 0, nUTI, 0);
#endif
				CSW_TRACE(33, TSTXT(" post EV_CFW_SIM_SET_SMS_PARAMETERS_RSP Success! \n"));
				CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
				hAo = HNULL;
			}
		}
		break;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetSmsParametersProc);
	return ERR_SUCCESS;
}

UINT32 CFW_SimSmsInitProc(HAO hAO, CFW_EV* pEvent)
{
	UINT32 nState, nUTI, Ret, TotalSMS, ErrorCode;
	UINT16 TotalBytes = 0x0;
	UINT8 pattern[2], nType;
	UINT8* pSMSStatus = NULL;
	BOOL Flag_UnknownSms = FALSE;
	UINT8 nSmsStatus = 0x0;
	api_SimElemFileStatusCnf_t* pSimElemFileStatusCnf = NULL;
	api_SimReadRecordCnf_t * pSimReadRecordCnf = NULL;
	SIM_SMS_INIT_INFO * pSimSMSInfo;
	CFW_SMS_MULTIPART_INFO LongerMsgInfo;
	//CFW_SMS_LONG_INFO * pLongSMSInfo = NULL;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimSmsInitProc);
	nType      = 0x00;
	pattern[0] = 0x00;
	pattern[1] = 0x00;
	TotalBytes = 0x0000;
	nUTI       = 0x0;
	Ret        = 0x0;
	nState     = 0x0;
	TotalSMS   = 0x0;
	ErrorCode  = 0x0;
	SUL_ZeroMemory8(&LongerMsgInfo, SIZEOF(pSimSMSInfo));
	pSMSStatus = &nType; 

	if (pEvent == (CFW_EV*)0xffffffff) 
		nState = CFW_SM_STATE_IDLE;
	else
		nState = CFW_GetAoState(hAO);

#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAO);
#endif

	pSimSMSInfo = CFW_GetAoUserData(hAO); 
	switch (nState)
	{
		case CFW_SM_STATE_IDLE:
		{
			// Get the SMS info ==> API_SIM_EF_SMS.
#ifdef CFW_MULTI_SIM

			SimElemFileStatusReq(API_SIM_EF_SMS, nSimID);
#else
			SimElemFileStatusReq(API_SIM_EF_SMS);
#endif
			CFW_SetAoState(hAO, CFW_SM_STATE_WAIT);
		}
		break;
		
		case CFW_SM_STATE_WAIT:
		{
			switch (pEvent->nEventId)
			{
				case API_SIM_ELEMFILESTATUS_CNF:

				pSimElemFileStatusCnf = (api_SimElemFileStatusCnf_t*)(pEvent->nParam1);
				if (((0x90 == pSimElemFileStatusCnf->Sw1) && (00 == pSimElemFileStatusCnf->Sw2))
				|| (0x91 == pSimElemFileStatusCnf->Sw1))
				{

					SUL_MemCopy8(pattern, pSimElemFileStatusCnf->Data + 2, 2);
					TotalBytes = 0x00;
					TotalBytes = pattern[0];
					TotalBytes <<= 8;
					TotalBytes &= 0xFF00;
					TotalBytes += (UINT16)pattern[1];
					TotalSMS = TotalBytes / 176; pSimSMSInfo->totalSlot = (UINT16)TotalSMS;          
#ifdef CFW_MULTI_SIM
          _SimSMSStatusArrayInit(TotalSMS,nSimID);
					SimReadRecordReq(API_SIM_EF_SMS, (UINT8)(pSimSMSInfo->nCurrentIndex), 0x04, 176, nSimID);
#else
          _SimSMSStatusArrayInit(TotalSMS);
					SimReadRecordReq(API_SIM_EF_SMS, (UINT8)(pSimSMSInfo->nCurrentIndex), 0x04, 176);
#endif
				}
				else
				{
					CFW_GetUTI(hAO, &nUTI);
#ifdef CFW_MULTI_SIM
					ErrorCode =
					Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2, nSimID);

					CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
					CFW_PostNotifyEvent(EV_CFW_SIM_INIT_SMS_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24),0xf0);
#else
					ErrorCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2);
					CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
					CFW_PostNotifyEvent(EV_CFW_SIM_INIT_SMS_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
#endif
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAO);
					hAO = HNULL;
				}
				break; 
				
				case API_SIM_READRECORD_CNF:

				pSimReadRecordCnf = (api_SimReadRecordCnf_t*)(pEvent->nParam1);
				if (((0x90 == pSimReadRecordCnf->Sw1) && (00 == pSimReadRecordCnf->Sw2))
				|| (0x91 == pSimReadRecordCnf->Sw1))
				{
          UINT8 i = 0x00;
					nSmsStatus = (UINT8)(0x1F & (pSimReadRecordCnf->Data[0])); 
          if (0x03 == nSmsStatus)
					{
            i = 0x01;
						// CFW_SMS_STORED_STATUS_UNREAD
						pSimSMSInfo->unReadRecords++;
						pSimSMSInfo->usedSlot++;
					}
					else if (0x01 == nSmsStatus)
					{
            i = 0x01;
						// CFW_SMS_STORED_STATUS_READ
						pSimSMSInfo->readRecords++;
						pSimSMSInfo->usedSlot++;
					}
					else if (0x07 == nSmsStatus)
					{
            i = 0x01;
						// CFW_SMS_STORED_STATUS_UNSENT
						pSimSMSInfo->unsentRecords++;
						pSimSMSInfo->usedSlot++;
					}
					else if (0x05 == nSmsStatus)
					{
            i = 0x01;
						// CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ
						pSimSMSInfo->sentRecords++;
						pSimSMSInfo->usedSlot++;
					}
					else if (0x0D == nSmsStatus)
					{
            i = 0x01;
						// CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV
						pSimSMSInfo->sentRecords++;
						pSimSMSInfo->usedSlot++;
					}
					else if (0x15 == nSmsStatus)
					{
            i = 0x01;
						// CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE
						pSimSMSInfo->sentRecords++;
						pSimSMSInfo->usedSlot++;
					}
					else if (0x1D == nSmsStatus)
					{
            i = 0x01;
						// CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE
						pSimSMSInfo->sentRecords++;
						pSimSMSInfo->usedSlot++;
					}
					else if (0x00 != nSmsStatus)
					{
						// unknownRecords
						pSimSMSInfo->unknownRecords++;
						Flag_UnknownSms = TRUE;
					}
#if CFW_SIM_SMS_SUPPORT_LONG_MSG
					if ((0x00 != nSmsStatus) && (Flag_UnknownSms != TRUE) && CFW_SIM_SMS_SUPPORT_LONG_MSG)
					{
						UINT32 nPduOutData = 0x0;;
						Ret = CFW_DecomposePDU(pSimReadRecordCnf->Data, 0, NULL, pSMSStatus, NULL);
						if (Ret != ERR_SUCCESS)
						{
							CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_DecomposePDU return 0x%x \n", Ret);
							CFW_GetUTI(hAO, &nUTI);
							CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAO);
							hAO = HNULL;
							CSW_PROFILE_FUNCTION_EXIT(CFW_SimSmsInitProc);
							return (Ret);
						}

						if ((*pSMSStatus == CFW_SMS_STORED_STATUS_UNREAD)
						|| (*pSMSStatus == CFW_SMS_STORED_STATUS_READ))
						{
							Ret = CFW_DecomposePDU(pSimReadRecordCnf->Data, 0x02, &nPduOutData, pSMSStatus, &LongerMsgInfo);
							CSW_SIM_FREE((UINT8*)nPduOutData);

						}
						else if ((*pSMSStatus == CFW_SMS_STORED_STATUS_UNSENT) ||
						(*pSMSStatus == CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ) ||
						(*pSMSStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) ||
						(*pSMSStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) ||
						(*pSMSStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE))
						{

							Ret = CFW_DecomposePDU(pSimReadRecordCnf->Data, 0x04, &nPduOutData, pSMSStatus, &LongerMsgInfo);
							CSW_SIM_FREE((UINT8*)nPduOutData);
						}
#if 0
						if (LongerMsgInfo.count != 0x00)
						{
							pSimSMSInfo->nLongStruCount++;
							pLongSMSInfo = (CFW_SMS_LONG_INFO*)CSW_SIM_MALLOC(sizeof(CFW_SMS_LONG_INFO));
							if (pLongSMSInfo == NULL)
							{
								// Unregist Ao
#ifdef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SIM_INIT_SMS_RSP, (UINT32)ERR_NO_MORE_MEMORY, 0,
													nUTI | (nSimID << 24), 0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SIM_INIT_SMS_RSP, (UINT32)ERR_NO_MORE_MEMORY, 0, nUTI, 0xf0);
#endif
								CFW_GetUTI(hAO, &nUTI);
								CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAO);
								hAO = HNULL;
								CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
								CSW_PROFILE_FUNCTION_EXIT(CFW_SimSmsInitProc);
								return (ERR_NO_MORE_MEMORY);
							}
							SUL_ZeroMemory8(pLongSMSInfo, sizeof(CFW_SMS_LONG_INFO)); pLongSMSInfo->count = LongerMsgInfo.count; pLongSMSInfo->current = LongerMsgInfo.current; pLongSMSInfo->id = LongerMsgInfo.id; pLongSMSInfo->index = (UINT8)(pSimSMSInfo->nCurrentIndex); pLongSMSInfo->nIEIa = 0x00; // Next version will change the value
							pLongSMSInfo->nStatus = *pSMSStatus; pLongSMSInfo->pNext = NULL;

							// Conncet the struct to the link
							if (pSimSMSInfo->pLongInfo != NULL)
							{
								CFW_SMS_LONG_INFO* pLink;
								pLink = pSimSMSInfo->pLongInfo;
								while (pLink->pNext != NULL)
								{
									pLink = (CFW_SMS_LONG_INFO*)(pLink->pNext);
								}

								pLink->pNext = (UINT32*)pLongSMSInfo;
							}
							else
							{
								pSimSMSInfo->pLongInfo = pLongSMSInfo;
							}
						}
#endif
					}
#endif

          if(i)
#ifdef CFW_MULTI_SIM
          _SimSMSStatusArrayAdd( pSimSMSInfo->nCurrentIndex,nSimID );
#else
          _SimSMSStatusArrayAdd( pSimSMSInfo->nCurrentIndex );
#endif
					if (pSimSMSInfo->nCurrentIndex < pSimSMSInfo->totalSlot)
					{
						// CSW_TRACE(CFW_SIM_TS_ID, "Initing sms The current index ==== 0x00%x\n",pSimSMSInfo->nCurrentIndex);
						pSimSMSInfo->nCurrentIndex++;
#ifdef CFW_MULTI_SIM
						SimReadRecordReq(API_SIM_EF_SMS, (UINT8)(pSimSMSInfo->nCurrentIndex), 0x04, 176, nSimID);
#else
						SimReadRecordReq(API_SIM_EF_SMS, (UINT8)(pSimSMSInfo->nCurrentIndex), 0x04, 176);
#endif
					}
					else
					{
						// Post Event
						CFW_SMS_INIT_INFO* pSimSMSInitInfo = NULL;
						pSimSMSInitInfo = (CFW_SMS_INIT_INFO*)CSW_SIM_MALLOC(sizeof(CFW_SMS_INIT_INFO));
						if (pSimSMSInitInfo == NULL)
						{
#ifdef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SIM_INIT_SMS_RSP, (UINT32)ERR_NO_MORE_MEMORY, 0,
												nUTI | (nSimID << 24), 0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SIM_INIT_SMS_RSP, (UINT32)ERR_NO_MORE_MEMORY, 0, nUTI, 0xf0);
#endif
							CFW_GetUTI(hAO, &nUTI);
							CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAO);
							hAO = HNULL;
							CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
							CSW_PROFILE_FUNCTION_EXIT(CFW_SimSmsInitProc);
							return (ERR_NO_MORE_MEMORY);
						}
						SUL_ZeroMemory8(pSimSMSInitInfo, sizeof(CFW_SMS_INIT_INFO));
						pSimSMSInitInfo->usedSlot       = pSimSMSInfo->usedSlot;
						pSimSMSInitInfo->totalSlot      = pSimSMSInfo->totalSlot;
						pSimSMSInitInfo->readRecords    = pSimSMSInfo->readRecords;
						pSimSMSInitInfo->unReadRecords  = pSimSMSInfo->unReadRecords;
						pSimSMSInitInfo->sentRecords    = pSimSMSInfo->sentRecords;
						pSimSMSInitInfo->unsentRecords  = pSimSMSInfo->unsentRecords;
						pSimSMSInitInfo->unknownRecords = pSimSMSInfo->unknownRecords;
						pSimSMSInitInfo->storageId      = pSimSMSInfo->storageId;
						pSimSMSInitInfo->nLongStruCount = pSimSMSInfo->nLongStruCount;
						pSimSMSInitInfo->pLongInfo      = pSimSMSInfo->pLongInfo;

						CFW_GetUTI(hAO, &nUTI);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_INIT_SMS_RSP, (UINT32)pSimSMSInitInfo, 0x00,
											nUTI | nSimID << 24, 0x00);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_INIT_SMS_RSP, (UINT32)pSimSMSInitInfo, 0x00, nUTI, 0x00);
#endif
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAO);
						hAO = HNULL;
						CSW_TRACE(CFW_SIM_TS_ID, "Initing sms over !\n");
						CSW_TRACE(CFW_SIM_TS_ID, "EV_CFW_SIM_INIT_SMS_RSP  usedSlot is %d\n",
						pSimSMSInitInfo->usedSlot);
						CSW_TRACE(CFW_SIM_TS_ID, "EV_CFW_SIM_INIT_SMS_RSP  totalSlot is %d\n",
						pSimSMSInitInfo->totalSlot);
						CSW_TRACE(CFW_SIM_TS_ID, "EV_CFW_SIM_INIT_SMS_RSP  readRecords is %d\n",
						pSimSMSInitInfo->readRecords);
						CSW_TRACE(CFW_SIM_TS_ID, "EV_CFW_SIM_INIT_SMS_RSP  unReadRecords is %d\n",
						pSimSMSInitInfo->unReadRecords);
						CSW_TRACE(CFW_SIM_TS_ID, "EV_CFW_SIM_INIT_SMS_RSP  sentRecords is %d\n",
						pSimSMSInitInfo->sentRecords);
						CSW_TRACE(CFW_SIM_TS_ID, "EV_CFW_SIM_INIT_SMS_RSP  unsentRecords is %d\n",
						pSimSMSInitInfo->unsentRecords);
						CSW_TRACE(CFW_SIM_TS_ID, "EV_CFW_SIM_INIT_SMS_RSP  unknownRecords is %d\n",
						pSimSMSInitInfo->unknownRecords);
						CSW_TRACE(CFW_SIM_TS_ID, "EV_CFW_SIM_INIT_SMS_RSP  storageId is %d\n",
						pSimSMSInitInfo->storageId);
						CSW_TRACE(CFW_SIM_TS_ID, "EV_CFW_SIM_INIT_SMS_RSP  nLongStruCount is %d\n",
						pSimSMSInitInfo->nLongStruCount);
					}
				}
				else
				{
					CFW_GetUTI(hAO, &nUTI);

					// ErrorCode = cfw_ErrorDeal( pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2);
#ifdef CFW_MULTI_SIM
					ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2, nSimID);
					CFW_PostNotifyEvent(EV_CFW_SIM_INIT_SMS_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24),0xf0);
#else
					ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2);
					CFW_PostNotifyEvent(EV_CFW_SIM_INIT_SMS_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAO);
					hAO = HNULL;
				}
				break;
			}
			break;
		}
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_SimSmsInitProc);
	return ERR_SUCCESS;
}


#endif

