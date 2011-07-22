// //////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2002-2009, Coolsand Technologies, Inc.
// All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmission of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
// 
// FILENAME: cfw_sim_pbk_pro.c
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

#include "cfw_sim.h"
#include "cfw_sim_pbk.h"
//-------------------add by wulc for smoke test ------------------
//make CFW_SimPBKAddProc( ) can return right parameter2 which used by smoketest demo
UINT8 _GetPBKStorage(UINT32 nStackFileID)
{
	if( API_SIM_EF_ADN == nStackFileID )
		return CFW_PBK_SIM;
 	else if(API_SIM_EF_FDN == nStackFileID)
		return CFW_PBK_SIM_FIX_DIALLING;
	else if(API_SIM_EF_LND == nStackFileID)
		return CFW_PBK_SIM_LAST_DIALLING;
	else if(API_SIM_EF_BDN == nStackFileID)
		return CFW_PBK_SIM_BARRED_DIALLING;
	else if(API_SIM_EF_MSISDN == nStackFileID)
		return CFW_PBK_ON;
	else
		return 0x00;
 	 
}
//------------------------------------------------------------
UINT32 cfw_SimParsePBKRecData(UINT8* pData,
							  CFW_SIM_PBK_ENTRY_INFO * pGetPBKEntry, 
							  void* proc, 
							  UINT8 txtlength,
                              UINT8 nApiNum
                             );
#ifdef CFW_MULTI_SIM
UINT32 Sim_ParseSW1SW2(UINT8 SW1, UINT8 SW2, CFW_SIM_ID nSimID);
#else
UINT32 Sim_ParseSW1SW2(UINT8 SW1, UINT8 SW2);
#endif

UINT32 CFW_SimPBKAddProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32 nUTI      = 0x00000000;
	UINT32 nEvtId    = 0x00000000;
	UINT32 ErrorCode = 0x00000000;
	UINT32 result    = 0x00000000;
	UINT8 txtlength  = 0x00;
	VOID* nEvParam   = NULL;
	CFW_EV ev;

	SIM_SM_INFO_ADDPBK* pAddPBK = NULL;
	UINT8* pPadFData = NULL;

	SIM_CHV_PARAM* pSimCHVPara = NULL;
	SIM_PBK_PARAM* pSimPBKPara = NULL;
	api_SimUpdateRecordCnf_t* pSimUpdateRecordCnf = NULL;
	api_SimSeekCnf_t* pSimSeekCnf = NULL;
	api_SimReadRecordCnf_t* pSimReadRecordCnf = NULL;
	CFW_SIM_PBK_ENTRY_INFO* pGetPBKEntry = NULL;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimPBKAddProc);

#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif
	pAddPBK = CFW_GetAoUserData(hAo);

	if ((UINT32)pEvent == 0xffffffff)
	{
		SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
		ev.nParam1                 = 0xff;
		pEvent                     = &ev;
		pAddPBK->nState.nNextState = CFW_SIM_PBK_IDLE;
	}
	else
	{
		nEvtId   = pEvent->nEventId;
		nEvParam = (VOID*)pEvent->nParam1;
	}

	switch (pAddPBK->nState.nNextState)
	{
		case CFW_SIM_PBK_IDLE:

#ifdef CFW_MULTI_SIM
		CFW_CfgSimGetPbkParam(&pSimPBKPara, nSimID);
#else
		CFW_CfgSimGetPbkParam(&pSimPBKPara);
#endif
		switch (pAddPBK->nCurrentFile)
		{
			case API_SIM_EF_ADN:
			  pAddPBK->nDataSize = pSimPBKPara->nADNRecordSize;
			  break;
			case API_SIM_EF_FDN:
			  pAddPBK->nDataSize = pSimPBKPara->nFDNRecordSize;
			  break;
			case API_SIM_EF_LND:
			  pAddPBK->nDataSize = pSimPBKPara->nLNDRecordSize;
			  break;
			case API_SIM_EF_BDN:
			  pAddPBK->nDataSize = pSimPBKPara->nBDNRecordSize - 1;
			  break;
			case API_SIM_EF_MSISDN:
			  pAddPBK->nDataSize = pSimPBKPara->nMSISDNRecordSize;
			  break;
			default:
			  break;
		}

		if (pAddPBK->nCurrentFile == API_SIM_EF_LND)
		{
			SUL_MemSet8(pAddPBK->pData + pAddPBK->nNameSize, 0xFF,
			(UINT16)(pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH - pAddPBK->nNameSize));
			pAddPBK->pData[pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH] = pAddPBK->nNumSize + 1;
			pAddPBK->pData[pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH + 1] = pAddPBK->nNumType;
			if (pAddPBK->nNumSize > 0)
			{
				SUL_MemCopy8(pAddPBK->pData + pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH + 2, pAddPBK->pNum, pAddPBK->nNumSize);
			}
			SUL_MemSet8(pAddPBK->pData + pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH + 2 + pAddPBK->nNumSize, 0xFF,
						(UINT16)(10 - pAddPBK->nNumSize));
			SUL_MemSet8(pAddPBK->pData + pAddPBK->nDataSize - 2, 0xFF, 2);

#ifdef CFW_MULTI_SIM
			result = SimUpdateRecordReq(pAddPBK->nCurrentFile, 0x00, 0x03, pAddPBK->nDataSize, pAddPBK->pData, nSimID);
#else
			result = SimUpdateRecordReq(pAddPBK->nCurrentFile, 0x00, 0x03, pAddPBK->nDataSize, pAddPBK->pData);
#endif

			if (ERR_SUCCESS != result)
			{
				CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n", result);
				CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKAddProc);
				CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
				hAo = HNULL;
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)result, pAddPBK->nAddRecInd, nUTI | (nSimID << 24), 0xF0);
#else
				CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)result, pAddPBK->nAddRecInd, nUTI, 0xF0);
#endif
			}
			pAddPBK->nState.nCurrState = CFW_SIM_PBK_IDLE;
			pAddPBK->nState.nNextState = CFW_SIM_PBK_UPDATERECORD;
		}
		
		else
		{
			if ((pAddPBK->nCurrentFile == API_SIM_EF_FDN) || (pAddPBK->nCurrentFile == API_SIM_EF_BDN))
			{
#ifdef CFW_MULTI_SIM
				CFW_CfgSimGetChvParam(&pSimCHVPara, nSimID);
#else
				CFW_CfgSimGetChvParam(&pSimCHVPara);
#endif
				if (pSimCHVPara->nPin2Status == CHV_NOVERIFY)
				pSimCHVPara->bCHV2VerifyReq = TRUE;

				// pSimCHVPara->nPin2puk2=TRUE;
			}
			if (pAddPBK->nAddRecInd == 0)
			{

				UINT8 size = pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH + 1;

				pPadFData = (UINT8*)CSW_SIM_MALLOC(size);
				if (pPadFData == NULL)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, ERR_NO_MORE_MEMORY, pAddPBK->nAddRecInd, nUTI | (nSimID << 24), 0xF0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, ERR_NO_MORE_MEMORY, pAddPBK->nAddRecInd, nUTI, 0xF0);
#endif
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKAddProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					return ERR_SUCCESS;
				}
				SUL_MemSet8(pPadFData, 0xFF, (UINT16)(size));
#ifdef CFW_MULTI_SIM
				result = SimSeekReq(pAddPBK->nCurrentFile, 0x10, (UINT8)(size), pPadFData, nSimID);
#else
				result = SimSeekReq(pAddPBK->nCurrentFile, 0x10, (UINT8)(size), pPadFData);
#endif
				// CSW_TRACE(CFW_SIM_TS_ID, TSTXT("@@@@@2 pAddPBK->nDataSize=%x\r\n"), pAddPBK->nDataSize);

				if (ERR_SUCCESS != result)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error SimSeekReq return 0x%x \n", result);
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)result, pAddPBK->nAddRecInd, nUTI | (nSimID << 24), 0xF0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)result, pAddPBK->nAddRecInd, nUTI, 0xF0);
#endif
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKAddProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
				CSW_SIM_FREE(pPadFData);
				pAddPBK->nState.nCurrState = CFW_SIM_PBK_IDLE;
				pAddPBK->nState.nNextState = CFW_SIM_PBK_SEEK;
			}
			else if (pAddPBK->nAddRecInd != 0)
			{
				SUL_MemSet8(pAddPBK->pData + pAddPBK->nNameSize, 0xFF,
				(UINT16)(pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH - pAddPBK->nNameSize));
				pAddPBK->pData[pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH] = pAddPBK->nNumSize + 1;
				pAddPBK->pData[pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH + 1] = pAddPBK->nNumType;
				if (pAddPBK->nNumSize > 0)
				SUL_MemCopy8(pAddPBK->pData + pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH + 2, pAddPBK->pNum,
				pAddPBK->nNumSize);
				SUL_MemSet8(pAddPBK->pData + pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH + 2 + pAddPBK->nNumSize, 0xFF,
				(UINT16)(10 - pAddPBK->nNumSize));
				SUL_MemSet8(pAddPBK->pData + pAddPBK->nDataSize - 2, 0xFF, 2);
#ifdef CFW_MULTI_SIM
				result =
				SimUpdateRecordReq(pAddPBK->nCurrentFile, pAddPBK->nAddRecInd, 0x04, pAddPBK->nDataSize, pAddPBK->pData,nSimID);
#else
				result =
				SimUpdateRecordReq(pAddPBK->nCurrentFile, pAddPBK->nAddRecInd, 0x04, pAddPBK->nDataSize, pAddPBK->pData);
#endif
				if (ERR_SUCCESS != result)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n", result);
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)result, pAddPBK->nAddRecInd, nUTI | (nSimID << 24), 0xF0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)result, pAddPBK->nAddRecInd, nUTI, 0xF0);
#endif

					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKAddProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;

				}
				pAddPBK->nState.nCurrState = CFW_SIM_PBK_IDLE;
				pAddPBK->nState.nNextState = CFW_SIM_PBK_UPDATERECORD;
			}
		}
		CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE | CFW_AO_PROC_CODE_CONSUMED);
		break;

		case CFW_SIM_PBK_SEEK:
		{
			// Add by lixp for TBM780(PBK Init failed)at 20080121
			// Because a sim fetch Ind event is coming.But the event isnot filtrated
			if (API_SIM_SEEK_CNF != nEvtId)
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
				return ERR_SUCCESS;
			}

			// CSW_TRACE(CFW_SIM_TS_ID, TSTXT("@@@@@3 pAddPBK->nDataSize=%x\r\n"), pAddPBK->nDataSize);

			pSimSeekCnf = (api_SimSeekCnf_t*)nEvParam;

			// CSW_TRACE(CFW_SIM_TS_ID, TSTXT("@@@@@3pAddPBK->nNumSize=%x\r\n"), pAddPBK->nNumSize);
			if (((0x90 == pSimSeekCnf->Sw1) && (0x00 == pSimSeekCnf->Sw2)) || (0x9F == pSimSeekCnf->Sw1)
			|| (0x91 == pSimSeekCnf->Sw1))
			{
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" SIM seek Success! \n"));
				pAddPBK->nAddRecInd = pSimSeekCnf->RecordNb;
				SUL_MemSet8(pAddPBK->pData + pAddPBK->nNameSize, 0xFF,
				(UINT16)(pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH - pAddPBK->nNameSize));
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT("pAddPBK->nDataSize=%x\r\n"), pAddPBK->nDataSize);

				pAddPBK->pData[pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH] = pAddPBK->nNumSize + 1;

				pAddPBK->pData[pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH + 1] = pAddPBK->nNumType;
				SUL_MemCopy8(pAddPBK->pData + pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH + 2, pAddPBK->pNum, pAddPBK->nNumSize);

				CSW_TRACE(CFW_SIM_TS_ID, TSTXT("pAddPBK->nDataSize=%x\r\n"), pAddPBK->nDataSize);
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT("pAddPBK->nNumSize=%x\r\n"), pAddPBK->nNumSize);

				SUL_MemSet8(pAddPBK->pData + pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH + 2 + pAddPBK->nNumSize, 0xFF,
				(UINT16)(10 - pAddPBK->nNumSize));
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT("pAddPBK->nDataSize=%x\r\n"), pAddPBK->nDataSize);

				SUL_MemSet8(pAddPBK->pData + pAddPBK->nDataSize - 2, 0xFF, 2);
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT("pAddPBK->nAddRecInd=%x\r\n"), pAddPBK->nAddRecInd);
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT("pAddPBK->nDataSize=%x\r\n"), pAddPBK->nDataSize);
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT("pAddPBK->pData=%x\r\n"), pAddPBK->pData);
				
#ifdef CFW_MULTI_SIM
				result =
				SimUpdateRecordReq(pAddPBK->nCurrentFile, pAddPBK->nAddRecInd, 0x04, pAddPBK->nDataSize, pAddPBK->pData,nSimID);
#else
				result = SimUpdateRecordReq(pAddPBK->nCurrentFile, pAddPBK->nAddRecInd, 0x04, pAddPBK->nDataSize, pAddPBK->pData);
#endif
				if (ERR_SUCCESS != result)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n", result);
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)result, pAddPBK->nAddRecInd, nUTI | (nSimID << 24), 0xF0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)result, pAddPBK->nAddRecInd, nUTI, 0xF0);
#endif
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKAddProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE | CFW_AO_PROC_CODE_CONSUMED);
				pAddPBK->nState.nCurrState = CFW_SIM_PBK_SEEK;
				pAddPBK->nState.nNextState = CFW_SIM_PBK_UPDATERECORD;
			}
			else
			{
				if (0x67 == pSimSeekCnf->Sw1)
				{
#ifdef CFW_MULTI_SIM
					result = SimReadRecordReq(pAddPBK->nCurrentFile, 0x01, 0x04, pAddPBK->nDataSize, nSimID);
#else
					result = SimReadRecordReq(pAddPBK->nCurrentFile, 0x01, 0x04, pAddPBK->nDataSize);
#endif
					if (ERR_SUCCESS != result)
					{
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", result);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)result, 0, nUTI, 0xF0);
#endif
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKAddProc);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;

					}
					pAddPBK->nCoutOfAdd = 1;
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE | CFW_AO_PROC_CODE_CONSUMED);
					pAddPBK->nState.nCurrState = CFW_SIM_PBK_SEEK;
					pAddPBK->nState.nNextState = CFW_SIM_PBK_READRECORD;
				}
				else
				{
#ifdef CFW_MULTI_SIM
					ErrorCode = Sim_ParseSW1SW2(pSimSeekCnf->Sw1, pSimSeekCnf->Sw2, nSimID);
					CFW_GetUTI(hAo, &nUTI);
					CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)ErrorCode, pAddPBK->nAddRecInd, nUTI | (nSimID << 24),0xF0);
#else
					ErrorCode = Sim_ParseSW1SW2(pSimSeekCnf->Sw1, pSimSeekCnf->Sw2);
					CFW_GetUTI(hAo, &nUTI);
					CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)ErrorCode, pAddPBK->nAddRecInd, nUTI, 0xF0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);

					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKAddProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
			}
		}
		break;
		case CFW_SIM_PBK_READRECORD:
		{
			// Add by lixp for TBM780(PBK Init failed)at 20080121
			// Because a sim fetch Ind event is coming.But the event isnot filtrated
			if (API_SIM_READRECORD_CNF != nEvtId)
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
				return ERR_SUCCESS;
			}

			pSimReadRecordCnf = (api_SimReadRecordCnf_t*)nEvParam;
			if (((0x90 == pSimReadRecordCnf->Sw1) && (0x00 == pSimReadRecordCnf->Sw2)) || (0x91 == pSimReadRecordCnf->Sw1))
			{
				pGetPBKEntry = (CFW_SIM_PBK_ENTRY_INFO*)CSW_SIM_MALLOC(SIZEOF(CFW_SIM_PBK_ENTRY_INFO) + SIM_PBK_EXTR_SIZE);
				if (pGetPBKEntry == NULL)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKAddProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					return ERR_SUCCESS;

				}
				pGetPBKEntry->pNumber   = &pGetPBKEntry->iFullNameSize + (UINT32)4;
				pGetPBKEntry->pFullName = &pGetPBKEntry->iFullNameSize + (UINT32)(4 + SIM_PBK_NUMBER_SIZE);
				txtlength               = pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH;
				cfw_SimParsePBKRecData(pSimReadRecordCnf->Data, pGetPBKEntry, pAddPBK, txtlength, API_GETPBK);

				if ((pGetPBKEntry->iFullNameSize == 0) && (pGetPBKEntry->nNumberSize == 0) && (pGetPBKEntry->nType != CFW_TELNUMBER_TYPE_INTERNATIONAL))  // updated on 20070618 to read "+" entry
				{
					pAddPBK->nAddRecInd = pAddPBK->nCoutOfAdd;
					SUL_MemSet8(pAddPBK->pData + pAddPBK->nNameSize, 0xFF,
					(UINT16)(pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH - pAddPBK->nNameSize));
					pAddPBK->pData[pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH] = pAddPBK->nNumSize + 1;
					pAddPBK->pData[pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH + 1] = pAddPBK->nNumType;
					SUL_MemCopy8(pAddPBK->pData + pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH + 2, pAddPBK->pNum, pAddPBK->nNumSize);
					SUL_MemSet8(pAddPBK->pData + pAddPBK->nDataSize - CFW_SIM_REMAINLENGTH + 2 + pAddPBK->nNumSize, 0xFF,
					(UINT16)(10 - pAddPBK->nNumSize));
					SUL_MemSet8(pAddPBK->pData + pAddPBK->nDataSize - 2, 0xFF, 2);

#ifdef CFW_MULTI_SIM
					result =
					SimUpdateRecordReq(pAddPBK->nCurrentFile, pAddPBK->nAddRecInd, 0x04, pAddPBK->nDataSize, pAddPBK->pData,nSimID);
#else
					result =
					SimUpdateRecordReq(pAddPBK->nCurrentFile, pAddPBK->nAddRecInd, 0x04, pAddPBK->nDataSize, pAddPBK->pData);
#endif
					if (ERR_SUCCESS != result)
					{
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n", result);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)result, pAddPBK->nAddRecInd, nUTI | (nSimID << 24), 0xF0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)result, pAddPBK->nAddRecInd, nUTI, 0xF0);
#endif
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKAddProc);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
					}
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE | CFW_AO_PROC_CODE_CONSUMED);
					pAddPBK->nState.nCurrState = CFW_SIM_PBK_READRECORD;
					pAddPBK->nState.nNextState = CFW_SIM_PBK_UPDATERECORD;
				}
				else
				{
#ifdef CFW_MULTI_SIM
					result = SimReadRecordReq(pAddPBK->nCurrentFile, pAddPBK->nCoutOfAdd + 1, 0x04, pAddPBK->nDataSize, nSimID);
#else
					result = SimReadRecordReq(pAddPBK->nCurrentFile, pAddPBK->nCoutOfAdd + 1, 0x04, pAddPBK->nDataSize);
#endif
					if (ERR_SUCCESS != result)
					{
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", result);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)result, 0, nUTI, 0xF0);
#endif
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKAddProc);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
					}
					pAddPBK->nCoutOfAdd++;
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE | CFW_AO_PROC_CODE_CONSUMED);
					pAddPBK->nState.nCurrState = CFW_SIM_PBK_READRECORD;
					pAddPBK->nState.nNextState = CFW_SIM_PBK_READRECORD;
				}

			}
			else
			{
#ifdef CFW_MULTI_SIM
				ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2, nSimID);
				CFW_GetUTI(hAo, &nUTI);
				CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)ErrorCode, pAddPBK->nAddRecInd, nUTI | (nSimID << 24), 0xF0);
#else
				ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2);
				CFW_GetUTI(hAo, &nUTI);
				CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)ErrorCode, pAddPBK->nAddRecInd, nUTI, 0xF0);
#endif
				CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);

				CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKAddProc);
				CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
				hAo = HNULL;
			}
		}
		break;
		case CFW_SIM_PBK_UPDATERECORD:
		{
			// Add by lixp for TBM780(PBK Init failed)at 20080121
			// Because a sim fetch Ind event is coming.But the event isnot filtrated
			if (API_SIM_UPDATERECORD_CNF != nEvtId)
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
				return ERR_SUCCESS;
			}
			pSimUpdateRecordCnf = (api_SimUpdateRecordCnf_t*)nEvParam;
			if (((0x90 == pSimUpdateRecordCnf->Sw1) && (0x00 == pSimUpdateRecordCnf->Sw2))
			|| (0x91 == pSimUpdateRecordCnf->Sw1))
			{
				CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)pAddPBK->nAddRecInd,_GetPBKStorage(pAddPBK->nCurrentFile), nUTI | (nSimID << 24), 0);
#else
				CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)pAddPBK->nAddRecInd, _GetPBKStorage(pAddPBK->nCurrentFile), nUTI, 0);
#endif
				// g_PbkIndex = (UINT32)pAddPBK->nAddRecInd;
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_ADD_PBK_RSP Success!nAddRecInd = %d \n"),
				(UINT32)pAddPBK->nAddRecInd);
			}
			else
			{
#ifdef CFW_MULTI_SIM
				ErrorCode = Sim_ParseSW1SW2(pSimUpdateRecordCnf->Sw1, pSimUpdateRecordCnf->Sw2, nSimID);
				CFW_GetUTI(hAo, &nUTI);
				CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)ErrorCode, pAddPBK->nAddRecInd, nUTI | (nSimID << 24), 0xF0);
#else
				ErrorCode = Sim_ParseSW1SW2(pSimUpdateRecordCnf->Sw1, pSimUpdateRecordCnf->Sw2);
				CFW_GetUTI(hAo, &nUTI);
				CFW_PostNotifyEvent(EV_CFW_SIM_ADD_PBK_RSP, (UINT32)ErrorCode, pAddPBK->nAddRecInd, nUTI, 0xF0);
#endif
			}

			CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);

			CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKAddProc);
			CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
			hAo = HNULL;
		}
		break;

		default:
			CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKAddProc);
			CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
			hAo = HNULL;
		break;
	}

	return ERR_SUCCESS;
}

UINT32 CFW_SimPBKDelProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32 nUTI      = 0x00000000;
	UINT32 nEvtId    = 0x00000000;
	UINT32 ErrorCode = 0x00000000;
	UINT32 result    = 0x00000000;
	VOID* nEvParam   = NULL;
	CFW_EV ev;

	SIM_SM_INFO_DELEPBK* pDelePBK = NULL;
	UINT8* pPadFData = NULL;

	SIM_PBK_PARAM* pSimPBKPara = NULL;
	SIM_CHV_PARAM* pSimCHVPara = NULL;
	api_SimUpdateRecordCnf_t* pSimUpdateRecordCnf = NULL;

	CSW_PROFILE_FUNCTION_ENTER(CFW_SimPBKDelProc);

#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif

	pDelePBK = CFW_GetAoUserData(hAo);
	if ((UINT32)pEvent == 0xffffffff)
	{
		SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
		ev.nParam1                  = 0xff;
		pEvent                      = &ev;
		pDelePBK->nState.nNextState = CFW_SIM_PBK_IDLE;
	}
	else
	{
		nEvtId   = pEvent->nEventId;
		nEvParam = (VOID*)pEvent->nParam1;
	}

	switch (pDelePBK->nState.nNextState)
	{
		case CFW_SIM_PBK_IDLE:
		{
#ifdef CFW_MULTI_SIM
			CFW_CfgSimGetPbkParam(&pSimPBKPara, nSimID);
#else
			CFW_CfgSimGetPbkParam(&pSimPBKPara);
#endif
			switch (pDelePBK->nCurrentFile)
			{

				case API_SIM_EF_ADN:
				  pDelePBK->nPadFDataSize = pSimPBKPara->nADNRecordSize;
				  break;
				case API_SIM_EF_FDN:
				  pDelePBK->nPadFDataSize = pSimPBKPara->nFDNRecordSize;
				  break;
				case API_SIM_EF_BDN:
				  pDelePBK->nPadFDataSize = pSimPBKPara->nBDNRecordSize - 1;
				  break;
				case API_SIM_EF_MSISDN:
				  pDelePBK->nPadFDataSize = pSimPBKPara->nMSISDNRecordSize;
				  break;
				case API_SIM_EF_LND:
				  pDelePBK->nPadFDataSize = pSimPBKPara->nLNDRecordSize;
				  break;
				default:
				  break;
			}
			if ((pDelePBK->nCurrentFile == API_SIM_EF_FDN) || (pDelePBK->nCurrentFile == API_SIM_EF_BDN))
			{
#ifdef CFW_MULTI_SIM
				CFW_CfgSimGetChvParam(&pSimCHVPara, nSimID);
#else
				CFW_CfgSimGetChvParam(&pSimCHVPara);
#endif
				if (pSimCHVPara->nPin2Status == CHV_NOVERIFY)
				pSimCHVPara->bCHV2VerifyReq = TRUE;
				// pSimCHVPara->nPin2puk2=TRUE;
			}
			pPadFData = (UINT8*)CSW_SIM_MALLOC(pDelePBK->nPadFDataSize);
#ifdef CFW_MULTI_SIM
			if (pPadFData == NULL)
			{
				CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
				CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_PBK_ENTRY_RSP, ERR_NO_MORE_MEMORY, pDelePBK->nDeleRecInd,
									nUTI | (nSimID << 24), 0xF0);
				CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKDelProc);
				CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
				hAo = HNULL;
				return ERR_SUCCESS;

			}
			SUL_MemSet8(pPadFData, 0xFF, (UINT16)(pDelePBK->nPadFDataSize));
			if (pDelePBK->nCurrentFile == API_SIM_EF_LND)
			{
				result = SimUpdateRecordReq(pDelePBK->nCurrentFile, 0x00, 0x03, pDelePBK->nPadFDataSize, pPadFData, nSimID);
				if (ERR_SUCCESS != result)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n", result);

					CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_PBK_ENTRY_RSP, (UINT32)result, pDelePBK->nDeleRecInd,
										nUTI | (nSimID << 24), 0xF0);
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKDelProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_PBK_ENTRY_RSP, (UINT32)result, pDelePBK->nDeleRecInd,
										nUTI | (nSimID << 24), 0xF0);
				}
			}
			else
			{
				result =
				SimUpdateRecordReq(pDelePBK->nCurrentFile, pDelePBK->nDeleRecInd, 0x04, pDelePBK->nPadFDataSize, pPadFData,nSimID);
				if (ERR_SUCCESS != result)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n", result);

					CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_PBK_ENTRY_RSP, (UINT32)result, pDelePBK->nDeleRecInd,
					nUTI | (nSimID << 24), 0xF0);

					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKDelProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;

				}
			}

#else
			if (pPadFData == NULL)
			{
				CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
				CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_PBK_ENTRY_RSP, ERR_NO_MORE_MEMORY, pDelePBK->nDeleRecInd, nUTI, 0xF0);
				CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKDelProc);
				CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
				hAo = HNULL;
			}
			SUL_MemSet8(pPadFData, 0xFF, (UINT16)(pDelePBK->nPadFDataSize));
			if (pDelePBK->nCurrentFile == API_SIM_EF_LND)
			{

				result = SimUpdateRecordReq(pDelePBK->nCurrentFile, 0x00, 0x03, pDelePBK->nPadFDataSize, pPadFData);
				if (ERR_SUCCESS != result)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n", result);
					CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_PBK_ENTRY_RSP, (UINT32)result, pDelePBK->nDeleRecInd, nUTI, 0xF0);
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKDelProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_PBK_ENTRY_RSP, (UINT32)result, pDelePBK->nDeleRecInd, nUTI, 0xF0);
				}
			}
			else
			{
				result =
				SimUpdateRecordReq(pDelePBK->nCurrentFile, pDelePBK->nDeleRecInd, 0x04, pDelePBK->nPadFDataSize, pPadFData);
				if (ERR_SUCCESS != result)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n", result);
					CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_PBK_ENTRY_RSP, (UINT32)result, pDelePBK->nDeleRecInd, nUTI, 0xF0);


					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKDelProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;

				}
			}
#endif
			CSW_SIM_FREE(pPadFData);
			CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE | CFW_AO_PROC_CODE_CONSUMED);
			pDelePBK->nState.nCurrState = CFW_SIM_PBK_IDLE;
			pDelePBK->nState.nNextState = CFW_SIM_PBK_UPDATERECORD;
		}
		break;

		case CFW_SIM_PBK_UPDATERECORD:
		{
			// Add by lixp for TBM780(PBK Init failed)at 20080121
			// Because a sim fetch Ind event is coming.But the event isnot filtrated
			if (API_SIM_UPDATERECORD_CNF != nEvtId)
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
				return ERR_SUCCESS;
			}

			pSimUpdateRecordCnf = (api_SimUpdateRecordCnf_t*)nEvParam;
			if (((0x90 == pSimUpdateRecordCnf->Sw1) && (0x00 == pSimUpdateRecordCnf->Sw2))
			    || (0x91 == pSimUpdateRecordCnf->Sw1))
			{
			  CFW_GetUTI(hAo, &nUTI);

#ifdef CFW_MULTI_SIM
			  CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_PBK_ENTRY_RSP, (UINT32)pDelePBK->nDeleRecInd, 0, nUTI | (nSimID << 24), 0);
#else
			  CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_PBK_ENTRY_RSP, (UINT32)pDelePBK->nDeleRecInd, 0, nUTI, 0);
#endif
			  CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_DELETE_PBK_ENTRY_RSP Success! \n"));
			}
			else
			{
#ifdef CFW_MULTI_SIM
				ErrorCode = Sim_ParseSW1SW2(pSimUpdateRecordCnf->Sw1, pSimUpdateRecordCnf->Sw2, nSimID);
				CFW_GetUTI(hAo, &nUTI);
				CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_PBK_ENTRY_RSP, (UINT32)ErrorCode, pDelePBK->nDeleRecInd,
				                  nUTI | (nSimID << 24), 0xF0);
#else
				ErrorCode = Sim_ParseSW1SW2(pSimUpdateRecordCnf->Sw1, pSimUpdateRecordCnf->Sw2);
				CFW_GetUTI(hAo, &nUTI);
				CFW_PostNotifyEvent(EV_CFW_SIM_DELETE_PBK_ENTRY_RSP, (UINT32)ErrorCode, pDelePBK->nDeleRecInd, nUTI, 0xF0);
#endif
			}

			CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKDelProc);
			CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
			hAo = HNULL;
		}
		break;

		default:
			CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
			hAo = HNULL;
		break;
	}
	return ERR_SUCCESS;
}

UINT32 CFW_SimPBKEntryProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32 nUTI      = 0x00000000;
	UINT32 nEvtId    = 0x00000000;
	UINT32 ErrorCode = 0x00000000;
	UINT8 txtlength  = 0x00;
	UINT32 result    = 0x00000000;
	VOID* nEvParam   = NULL;
	CFW_EV ev;

	CFW_SIM_PBK_ENTRY_INFO* pGetPBKEntry = NULL;
	SIM_SM_INFO_GETPBK* pGetPBK = NULL;
	SIM_PBK_PARAM* pSimPBKPara = NULL;
	api_SimReadRecordCnf_t* pSimReadRecordCnf = NULL;
	api_SimReadBinaryCnf_t* pSimReadBinaryCnf = NULL;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimPBKEntryProc);

#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif

	pGetPBK = CFW_GetAoUserData(hAo);
	if ((UINT32)pEvent == 0xffffffff)
	{
		SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
		ev.nParam1                 = 0xff;
		pEvent                     = &ev;
		pGetPBK->nState.nNextState = CFW_SIM_PBK_IDLE;
	}
	else
	{
		nEvtId   = pEvent->nEventId;
		nEvParam = (VOID*)pEvent->nParam1;
	}

	switch (pGetPBK->nState.nNextState)
	{
		case CFW_SIM_PBK_IDLE:
		{
#ifdef CFW_MULTI_SIM
			CFW_CfgSimGetPbkParam(&pSimPBKPara, nSimID);
#else
			CFW_CfgSimGetPbkParam(&pSimPBKPara);
#endif
			switch (pGetPBK->nCurrentFile)
			{

				case API_SIM_EF_ADN:
				  pGetPBK->nRecordSize = pSimPBKPara->nADNRecordSize;
				  break;
				case API_SIM_EF_FDN:
				  pGetPBK->nRecordSize = pSimPBKPara->nFDNRecordSize;
				  break;
				case API_SIM_EF_BDN:
				  pGetPBK->nRecordSize = pSimPBKPara->nBDNRecordSize - 1;
				  break;
				case API_SIM_EF_SDN:
				  pGetPBK->nRecordSize = pSimPBKPara->nSDNRecordSize;
				  break;
				case API_SIM_EF_MSISDN:
				  pGetPBK->nRecordSize = pSimPBKPara->nMSISDNRecordSize;
				  break;
				case API_SIM_EF_LND:
				  pGetPBK->nRecordSize = pSimPBKPara->nLNDRecordSize;
				  break;
				case API_SIM_EF_ECC:
				  pGetPBK->nRecordSize = pSimPBKPara->nECCRecordSize;
				  break;
				default:
				  break;
			}
			if (pGetPBK->nCurrentFile == API_SIM_EF_ECC)
			{
#ifdef CFW_MULTI_SIM
				result = SimReadBinaryReq(API_SIM_EF_ECC, 0, pGetPBK->nRecordSize, nSimID);
#else
				result = SimReadBinaryReq(API_SIM_EF_ECC, 0, pGetPBK->nRecordSize);
#endif
				if (ERR_SUCCESS != result)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n", result);
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, (UINT32)result, 0, nUTI, 0xF0);
#endif
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKEntryProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;

				}
				pGetPBK->nState.nCurrState = CFW_SIM_PBK_IDLE;
				pGetPBK->nState.nNextState = CFW_SIM_PBK_READBINARY;
			}
			else
			{
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimPBKEntryProc()nCurrentFile=%x\r\n"), pGetPBK->nCurrentFile);
#ifdef CFW_MULTI_SIM
				result = SimReadRecordReq(pGetPBK->nCurrentFile, pGetPBK->nCurrRecInd, 0x04, pGetPBK->nRecordSize, nSimID);
#else
				result = SimReadRecordReq(pGetPBK->nCurrentFile, pGetPBK->nCurrRecInd, 0x04, pGetPBK->nRecordSize);
#endif
				if (ERR_SUCCESS != result)
				{
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, (UINT32)result, 0, nUTI, 0xF0);
#endif
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKEntryProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
				pGetPBK->nState.nCurrState = CFW_SIM_PBK_IDLE;
				pGetPBK->nState.nNextState = CFW_SIM_PBK_READRECORD;
			}
			CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE | CFW_AO_PROC_CODE_CONSUMED);
		}
		break;

		case CFW_SIM_PBK_READRECORD:
		{
			// Add by lixp for TBM780(PBK Init failed)at 20080121
			// Because a sim fetch Ind event is coming.But the event isnot filtrated
			// 
			if (API_SIM_READRECORD_CNF != nEvtId)
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
				return ERR_SUCCESS;
			}

			pSimReadRecordCnf = (api_SimReadRecordCnf_t*)nEvParam;
			if (((0x90 == pSimReadRecordCnf->Sw1) && (0x00 == pSimReadRecordCnf->Sw2)) || (0x91 == pSimReadRecordCnf->Sw1))
			{
				pGetPBKEntry = (CFW_SIM_PBK_ENTRY_INFO*)CSW_SIM_MALLOC(SIZEOF(CFW_SIM_PBK_ENTRY_INFO) + SIM_PBK_EXTR_SIZE);
				if (pGetPBKEntry == NULL)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKEntryProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					return ERR_SUCCESS;

				}
				SUL_MemSet8(pGetPBKEntry, 0, (SIZEOF(CFW_SIM_PBK_ENTRY_INFO) + SIM_PBK_EXTR_SIZE)); // hameina[+] 20081006, zero memery
				pGetPBKEntry->pNumber   = &pGetPBKEntry->iFullNameSize + (UINT32)4;
				pGetPBKEntry->pFullName = &pGetPBKEntry->iFullNameSize + (UINT32)(4 + SIM_PBK_NUMBER_SIZE);
				txtlength               = pGetPBK->nRecordSize - CFW_SIM_REMAINLENGTH;
				cfw_SimParsePBKRecData(pSimReadRecordCnf->Data, pGetPBKEntry, pGetPBK, txtlength, API_GETPBK);
				CFW_GetUTI(hAo, &nUTI);

#ifdef CFW_MULTI_SIM
				if ((pGetPBKEntry->iFullNameSize == 0) && (pGetPBKEntry->nNumberSize == 0) && (pGetPBKEntry->nType != CFW_TELNUMBER_TYPE_INTERNATIONAL))  // updated on 20070618 to read "+" entry
				{
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, (UINT32)NULL, pGetPBK->nCurrRecInd, nUTI | (nSimID << 24), 0);
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_GET_PBK_ENTRY_NOTIFY Success and record is NULL! \n"));
					CSW_SIM_FREE(pGetPBKEntry);

				}
				else
				{
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, (UINT32)pGetPBKEntry,
					((UINT16)(SIZEOF(CFW_SIM_PBK_ENTRY_INFO) + SIM_PBK_EXTR_SIZE) << 16) +
					(UINT16)pGetPBK->nCurrRecInd, nUTI | (nSimID << 24), 0);
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_GET_PBK_ENTRY_NOTIFY Success! \n"));
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" NameSize = %d,NumberSize = %d\n"),
					(UINT32)pGetPBKEntry->iFullNameSize, (UINT32)pGetPBKEntry->nNumberSize);

				}
#else
				if ((pGetPBKEntry->iFullNameSize == 0) && (pGetPBKEntry->nNumberSize == 0) && (pGetPBKEntry->nType != CFW_TELNUMBER_TYPE_INTERNATIONAL))  // updated on 20070618 to read "+" entry
				{
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, (UINT32)NULL, pGetPBK->nCurrRecInd, nUTI, 0);
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_GET_PBK_ENTRY_NOTIFY Success and record is NULL! \n"));
					CSW_SIM_FREE(pGetPBKEntry);

				}
				else
				{
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, (UINT32)pGetPBKEntry,
					((UINT16)(SIZEOF(CFW_SIM_PBK_ENTRY_INFO) + SIM_PBK_EXTR_SIZE) << 16) +
					(UINT16)pGetPBK->nCurrRecInd, nUTI, 0);
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_GET_PBK_ENTRY_NOTIFY Success! \n"));
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT("NameSize = %d,NumberSize = %d \n"),
					(UINT32)pGetPBKEntry->iFullNameSize, (UINT32)pGetPBKEntry->nNumberSize);

				}	 
#endif
				CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
				hAo = HNULL;
			}
			else
			{
#ifdef CFW_MULTI_SIM
				ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2, nSimID);
				CFW_GetUTI(hAo, &nUTI);
				CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, (UINT32)ErrorCode, pGetPBK->nCurrRecInd, nUTI | (nSimID << 24),
				0xF0);

#else

				ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2);
				CFW_GetUTI(hAo, &nUTI);
				CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, (UINT32)ErrorCode, pGetPBK->nCurrRecInd, nUTI, 0xF0);
#endif
			}

			CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKEntryProc);
			CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
			hAo = HNULL;
		}
		break;

		case CFW_SIM_PBK_READBINARY:
		{
			// Add by lixp for TBM780(PBK Init failed)at 20080121
			// Because a sim fetch Ind event is coming.But the event isnot filtrated
			if (API_SIM_READBINARY_CNF != nEvtId)
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
				return ERR_SUCCESS;
			}

			pSimReadBinaryCnf = (api_SimReadBinaryCnf_t*)nEvParam;
			CFW_GetUTI(hAo, &nUTI);
			if (((0x90 == pSimReadBinaryCnf->Sw1) && (0x00 == pSimReadBinaryCnf->Sw2)) || (0x91 == pSimReadBinaryCnf->Sw1))
			{
				if (pGetPBK->nCurrRecInd > 5)
				{
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, ERR_CFW_INVALID_PARAMETER, 0, nUTI | (nSimID << 24), 0xF0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, ERR_CFW_INVALID_PARAMETER, 0, nUTI, 0xF0);
#endif
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKEntryProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;

				}

				pGetPBKEntry = (CFW_SIM_PBK_ENTRY_INFO*)CSW_SIM_MALLOC(SIZEOF(CFW_SIM_PBK_ENTRY_INFO) + SIM_PBK_EXTR_SIZE);
				if (pGetPBKEntry == NULL)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKEntryProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					return ERR_SUCCESS;

				}
				SUL_ZeroMemory32(pGetPBKEntry, SIZEOF(CFW_SIM_PBK_ENTRY_INFO) + SIM_PBK_EXTR_SIZE);
				pGetPBKEntry->pNumber   = &pGetPBKEntry->iFullNameSize + (UINT32)4;
				pGetPBKEntry->pFullName = &pGetPBKEntry->iFullNameSize + (UINT32)(4 + SIM_PBK_NUMBER_SIZE);
				SUL_MemCopy8(pGetPBKEntry->pNumber, pSimReadBinaryCnf->Data + (pGetPBK->nCurrRecInd) * 3, 3);
				pGetPBKEntry->nNumberSize   = 3;
				pGetPBKEntry->iFullNameSize = 0;
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, (UINT32)pGetPBKEntry,
									((UINT16)(SIZEOF(CFW_SIM_PBK_ENTRY_INFO) + SIM_PBK_EXTR_SIZE) << 16) +
									(UINT16)pGetPBK->nCurrRecInd, nUTI | (nSimID << 24), 0);
#else
				CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, (UINT32)pGetPBKEntry,
				                    ((UINT16)(SIZEOF(CFW_SIM_PBK_ENTRY_INFO) + SIM_PBK_EXTR_SIZE) << 16) +
				                    (UINT16)pGetPBK->nCurrRecInd, nUTI, 0);
#endif
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_GET_PBK_ENTRY_RSP Success! \n"));
			}
			else
			{
#ifdef CFW_MULTI_SIM
			  ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2, nSimID);
			  CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xF0);
#else
			  ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2);
			  CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_ENTRY_RSP, (UINT32)ErrorCode, 0, nUTI, 0xF0);
#endif
			}
			CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKEntryProc);
			CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
			hAo = HNULL;
		}
		break;

		default:
			CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKEntryProc);
			CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
			hAo = HNULL;
		break;
	}
	return ERR_SUCCESS;
}

UINT32 CFW_SimPBKListEntryProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32 nUTI      = 0x00000000;
	UINT32 nEvtId    = 0x00000000;
	UINT32 ErrorCode = 0x00000000;
	UINT8 txtlength  = 0x00;
	UINT32 result    = 0x00000000;
	VOID* nEvParam   = NULL;
	CFW_EV ev;

	CFW_SIM_PBK_ENTRY_INFO* pGetPBKEntry = NULL;
	SIM_SM_INFO_LISTPBK* pListPBK = NULL;
	SIM_PBK_PARAM* pSimPBKPara = NULL;
	api_SimReadRecordCnf_t* pSimReadRecordCnf = NULL;
	api_SimReadBinaryCnf_t* pSimReadBinaryCnf = NULL;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimPBKListEntryProc);

#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif
	pListPBK = CFW_GetAoUserData(hAo);
	if ((UINT32)pEvent == 0xffffffff)
	{
		SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
		ev.nParam1                  = 0xff;
		pEvent                      = &ev;
		pListPBK->nState.nNextState = CFW_SIM_PBK_IDLE;
	}
	else
	{
		nEvtId   = pEvent->nEventId;
		nEvParam = (VOID*)pEvent->nParam1;
	}

	switch (pListPBK->nState.nNextState)
	{
		case CFW_SIM_PBK_IDLE:
		{
#ifdef CFW_MULTI_SIM
			CFW_CfgSimGetPbkParam(&pSimPBKPara, nSimID);
#else
			CFW_CfgSimGetPbkParam(&pSimPBKPara);
#endif
			switch (pListPBK->nCurrentFile)
			{
				case API_SIM_EF_ADN:
				  pListPBK->nRecordSize = pSimPBKPara->nADNRecordSize;
				  break;
				case API_SIM_EF_FDN:
				  pListPBK->nRecordSize = pSimPBKPara->nFDNRecordSize;
				  break;
				case API_SIM_EF_BDN:
				  pListPBK->nRecordSize = pSimPBKPara->nBDNRecordSize - 1;
				  break;
				case API_SIM_EF_SDN:
				  pListPBK->nRecordSize = pSimPBKPara->nSDNRecordSize;
				  break;
				case API_SIM_EF_MSISDN:
				  pListPBK->nRecordSize = pSimPBKPara->nMSISDNRecordSize;
				  break;
				case API_SIM_EF_LND:
				  pListPBK->nRecordSize = pSimPBKPara->nLNDRecordSize;
				  break;
				case API_SIM_EF_ECC:
				  pListPBK->nRecordSize = pSimPBKPara->nECCRecordSize;
				  break;
				default:
				  break;
			}
			if (pListPBK->nCurrentFile == API_SIM_EF_ECC)
			{
#ifdef CFW_MULTI_SIM
				result = SimReadBinaryReq(API_SIM_EF_ECC, 0, pListPBK->nRecordSize, nSimID);
				if (ERR_SUCCESS != result)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n", result);
					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_PBK_ENTRY_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0);
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListEntryProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;

				}
#else
				result = SimReadBinaryReq(API_SIM_EF_ECC, 0, pListPBK->nRecordSize);
				if (ERR_SUCCESS != result)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n", result);

					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_PBK_ENTRY_RSP, (UINT32)result, 0, nUTI, 0);
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListEntryProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;

				}
#endif
				pListPBK->nState.nCurrState = CFW_SIM_PBK_IDLE;
				pListPBK->nState.nNextState = CFW_SIM_PBK_READBINARY;
			}
			else
			{
#ifdef CFW_MULTI_SIM
				result = SimReadRecordReq(pListPBK->nCurrentFile, pListPBK->nStartRecInd, 0x04, pListPBK->nRecordSize, nSimID);
#else
				result = SimReadRecordReq(pListPBK->nCurrentFile, pListPBK->nStartRecInd, 0x04, pListPBK->nRecordSize);
#endif
				if (ERR_SUCCESS != result)
				{
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_PBK_ENTRY_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_PBK_ENTRY_RSP, (UINT32)result, 0, nUTI, 0);
#endif
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListEntryProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;

				}
				pListPBK->nState.nCurrState = CFW_SIM_PBK_IDLE;
				pListPBK->nState.nNextState = CFW_SIM_PBK_READRECORD;
			}
			CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE | CFW_AO_PROC_CODE_CONSUMED);
		}
		break;

		case CFW_SIM_PBK_READRECORD:
		{
			// Add by lixp for TBM780(PBK Init failed)at 20080121
			// Because a sim fetch Ind event is coming.But the event isnot filtrated
			if (API_SIM_READRECORD_CNF != nEvtId)
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
				return ERR_SUCCESS;
			}

			pSimReadRecordCnf = (api_SimReadRecordCnf_t*)nEvParam;
			if (((0x90 == pSimReadRecordCnf->Sw1) && (0x00 == pSimReadRecordCnf->Sw2)) || (0x91 == pSimReadRecordCnf->Sw1))
			{
				UINT32 nStructSize = 0x00;

				nStructSize = SIZEOF(CFW_SIM_PBK_ENTRY_INFO);
				if (pListPBK->nStartRecInd == pListPBK->nCurrRecInd)
				{
					UINT16 size =
					(pListPBK->nEndRecInd - pListPBK->nStartRecInd + 1) * (SIZEOF(CFW_SIM_PBK_ENTRY_INFO) + SIM_PBK_EXTR_SIZE);
					pGetPBKEntry = (CFW_SIM_PBK_ENTRY_INFO*)CSW_SIM_MALLOC(size);
					if (pGetPBKEntry == NULL)
					{
						CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_LIST_PBK_ENTRY_RSP, (UINT32)ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_LIST_PBK_ENTRY_RSP, (UINT32)ERR_NO_MORE_MEMORY, 0, nUTI, 0);
#endif
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListEntryProc);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						return ERR_SUCCESS;

					}
					pListPBK->nCurrentP = (UINT32)pGetPBKEntry;
				}
				else
					pGetPBKEntry          = (CFW_SIM_PBK_ENTRY_INFO*)(pListPBK->nCurrentP);

				
				pGetPBKEntry->pNumber   = &pGetPBKEntry->iFullNameSize + (UINT32)4;
				pGetPBKEntry->pFullName = &pGetPBKEntry->iFullNameSize + (UINT32)(4 + SIM_PBK_NUMBER_SIZE);
				txtlength               = pListPBK->nRecordSize - CFW_SIM_REMAINLENGTH;

				cfw_SimParsePBKRecData(pSimReadRecordCnf->Data, pGetPBKEntry, pListPBK, txtlength, API_LISTPBK);
				CFW_GetUTI(hAo, &nUTI);
				if ((pGetPBKEntry->iFullNameSize != 0) || (pGetPBKEntry->nNumberSize != 0) || (pGetPBKEntry->nType == CFW_TELNUMBER_TYPE_INTERNATIONAL))  // updated on 20070618 to read out "+" entry
				{
					pListPBK->nStructNum++;
					pListPBK->nCurrentP = (UINT32)(&pGetPBKEntry->iFullNameSize - 8 + nStructSize + SIM_PBK_EXTR_SIZE);
				}
				else if ((pGetPBKEntry->iFullNameSize == 0) && (pGetPBKEntry->nNumberSize == 0) && (pGetPBKEntry->nType != CFW_TELNUMBER_TYPE_INTERNATIONAL)) // updated on 20070618 to read out "+" entry
				{
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" record is NULL\n"));
				}
				if (pListPBK->nCurrRecInd < pListPBK->nEndRecInd)
				{
					pListPBK->nCurrRecInd += 1;
#ifdef CFW_MULTI_SIM
					result = SimReadRecordReq(pListPBK->nCurrentFile, pListPBK->nCurrRecInd, 0x04, pListPBK->nRecordSize, nSimID);
					if (ERR_SUCCESS != result)
					{
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", result);

						CFW_PostNotifyEvent(EV_CFW_SIM_LIST_PBK_ENTRY_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0);
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListEntryProc);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
					}
#else
				    result = SimReadRecordReq(pListPBK->nCurrentFile, pListPBK->nCurrRecInd, 0x04, pListPBK->nRecordSize);
				    if (ERR_SUCCESS != result)
					{
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", result);

						CFW_PostNotifyEvent(EV_CFW_SIM_LIST_PBK_ENTRY_RSP, (UINT32)result, 0, nUTI, 0);
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListEntryProc);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;

					}
#endif
					pListPBK->nState.nNextState = CFW_SIM_PBK_READRECORD;
					pListPBK->nState.nCurrState = CFW_SIM_PBK_READRECORD;
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE | CFW_AO_PROC_CODE_CONSUMED);
				}

				else if (pListPBK->nCurrRecInd == pListPBK->nEndRecInd)
				{
					pGetPBKEntry =
					(CFW_SIM_PBK_ENTRY_INFO*)(pListPBK->nCurrentP -(pListPBK->nStructNum - 1) * (nStructSize + SIM_PBK_EXTR_SIZE));
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_PBK_ENTRY_RSP, (UINT32)pGetPBKEntry, pListPBK->nStructNum - 1,
										nUTI | (nSimID << 24), 0);
#else
				    CFW_PostNotifyEvent(EV_CFW_SIM_LIST_PBK_ENTRY_RSP, (UINT32)pGetPBKEntry, pListPBK->nStructNum - 1, nUTI, 0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_LIST_PBK_ENTRY_NOTIFY Success! \n"));
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListEntryProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
			}
			else
			{
#ifdef CFW_MULTI_SIM
				ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2, nSimID);
				CFW_GetUTI(hAo, &nUTI);
				CFW_PostNotifyEvent(EV_CFW_SIM_LIST_PBK_ENTRY_RSP, (UINT32)ErrorCode, pListPBK->nCurrRecInd,
				nUTI | (nSimID << 24), 0xF0);
#else

				ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2);
				CFW_GetUTI(hAo, &nUTI);
				CFW_PostNotifyEvent(EV_CFW_SIM_LIST_PBK_ENTRY_RSP, (UINT32)ErrorCode, pListPBK->nCurrRecInd, nUTI, 0xF0);
#endif
				CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListEntryProc);
				CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
				hAo = HNULL;
			}
		}
		break;

		case CFW_SIM_PBK_READBINARY:
		{
			// Add by lixp for TBM780(PBK Init failed)at 20080121
			// Because a sim fetch Ind event is coming.But the event isnot filtrated
			if (API_SIM_READBINARY_CNF != nEvtId)
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
				return ERR_SUCCESS;
			}

			pSimReadBinaryCnf = (api_SimReadBinaryCnf_t*)nEvParam;
			CFW_GetUTI(hAo, &nUTI);
			if (((0x90 == pSimReadBinaryCnf->Sw1) && (0x00 == pSimReadBinaryCnf->Sw2)) || (0x91 == pSimReadBinaryCnf->Sw1))
			{
				UINT32 nStructSize = 0x00000000;
				UINT8 i            = 0x00;

				nStructSize = SIZEOF(CFW_SIM_PBK_ENTRY_INFO);
				if (pListPBK->nStructNum == 1)
				{
					pGetPBKEntry =
					(CFW_SIM_PBK_ENTRY_INFO*)CSW_SIM_MALLOC((pListPBK->nEndRecInd - pListPBK->nStartRecInd + 1) *
					                              (SIZEOF(CFW_SIM_PBK_ENTRY_INFO) + SIM_PBK_EXTR_SIZE));
					if (pGetPBKEntry == NULL)
					{
						CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_LIST_PBK_ENTRY_RSP, (UINT32)ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_LIST_PBK_ENTRY_RSP, (UINT32)ERR_NO_MORE_MEMORY, 0, nUTI, 0);
#endif
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListEntryProc);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
						return ERR_SUCCESS;
	
					}
					SUL_ZeroMemory32(pGetPBKEntry,
								     (pListPBK->nEndRecInd - pListPBK->nStartRecInd + 1) * (SIZEOF(CFW_SIM_PBK_ENTRY_INFO) +SIM_PBK_EXTR_SIZE));
				}
				else
					pGetPBKEntry = (CFW_SIM_PBK_ENTRY_INFO*)(pListPBK->nCurrentP);
				for (i = pListPBK->nStartRecInd; i < pListPBK->nEndRecInd + 1; i++)
				{

					if ((pSimReadBinaryCnf->Data[i* 3] & 0x0F)!=0x0F)
					{
						pGetPBKEntry->pNumber   = &pGetPBKEntry->iFullNameSize + (UINT32)4;
						pGetPBKEntry->pFullName = &pGetPBKEntry->iFullNameSize + (UINT32)(4 + SIM_PBK_NUMBER_SIZE);
						SUL_MemCopy8(pGetPBKEntry->pNumber, pSimReadBinaryCnf->Data + i * 3, 3);
						pGetPBKEntry->nNumberSize   = 3;
						pGetPBKEntry->iFullNameSize = 0;
						pListPBK->nStructNum += 1;
					}
				}
				pGetPBKEntry =
				(CFW_SIM_PBK_ENTRY_INFO*)(pListPBK->nCurrentP -(UINT32)((pListPBK->nStructNum - 2) * (nStructSize + SIM_PBK_EXTR_SIZE)));

#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SIM_LIST_PBK_ENTRY_RSP, (UINT32)pGetPBKEntry, pListPBK->nStructNum - 1,
									nUTI | (nSimID << 24), 0);
#else
				CFW_PostNotifyEvent(EV_CFW_SIM_LIST_PBK_ENTRY_RSP, (UINT32)pGetPBKEntry, pListPBK->nStructNum - 1, nUTI, 0);
#endif
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_LIST_PBK_ENTRY_RSP Success! \n"));
			}
			else
			{
#ifdef CFW_MULTI_SIM
				ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2, nSimID);
				CFW_PostNotifyEvent(EV_CFW_SIM_LIST_PBK_ENTRY_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xF0);
#else
				ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2);
				CFW_PostNotifyEvent(EV_CFW_SIM_LIST_PBK_ENTRY_RSP, (UINT32)ErrorCode, 0, nUTI, 0xF0);
#endif
			}
			CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListEntryProc);
			CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
			hAo = HNULL;
		}
		break;

		default:
			CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListEntryProc);
			CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
			hAo = HNULL;
		break;
	}
	return ERR_SUCCESS;
}

UINT32 CFW_SimPBKListCountEntryProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32 nUTI        = 0x00000000;
	UINT32 nEvtId      = 0x00000000;
	UINT32 ErrorCode   = 0x00000000;
	UINT32 nStructSize = 0x00000000;
	UINT8 txtlength    = 0x00;
	UINT32 result      = 0x00000000;
	VOID* nEvParam     = NULL;
	CFW_EV ev;

	CFW_SIM_PBK_ENTRY_INFO* pGetPBKEntry = NULL;
	SIM_SM_INFO_LISTCOUNTPBK* pListCountPBK = NULL;
	SIM_PBK_PARAM* pSimPBKPara = NULL;
	api_SimReadRecordCnf_t* pSimReadRecordCnf = NULL;
	api_SimReadBinaryCnf_t* pSimReadBinaryCnf = NULL;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimPBKListCountEntryProc);

#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif
	pListCountPBK = CFW_GetAoUserData(hAo);
	if ((UINT32)pEvent == 0xffffffff)
	{
		SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
		ev.nParam1                       = 0xff;
		pEvent                           = &ev;
		pListCountPBK->nState.nNextState = CFW_SIM_PBK_IDLE;
	}
	else
	{
		nEvtId   = pEvent->nEventId;
		nEvParam = (VOID*)pEvent->nParam1;
	}

	switch (pListCountPBK->nState.nNextState)
	{
		case CFW_SIM_PBK_IDLE:
		{
#ifdef CFW_MULTI_SIM
			CFW_CfgSimGetPbkParam(&pSimPBKPara, nSimID);
#else
			CFW_CfgSimGetPbkParam(&pSimPBKPara);
#endif

			switch (pListCountPBK->nCurrentFile)
			{
				case API_SIM_EF_ADN:
				pListCountPBK->nRecordSize = pSimPBKPara->nADNRecordSize;
				break;
				case API_SIM_EF_FDN:
				pListCountPBK->nRecordSize = pSimPBKPara->nFDNRecordSize;
				break;
				case API_SIM_EF_BDN:
				pListCountPBK->nRecordSize = pSimPBKPara->nBDNRecordSize - 1;
				break;
				case API_SIM_EF_SDN:
				pListCountPBK->nRecordSize = pSimPBKPara->nSDNRecordSize;
				break;
				case API_SIM_EF_MSISDN:
				pListCountPBK->nRecordSize = pSimPBKPara->nMSISDNRecordSize;
				break;
				case API_SIM_EF_LND:
				pListCountPBK->nRecordSize = pSimPBKPara->nLNDRecordSize;
				break;
				case API_SIM_EF_ECC:
				pListCountPBK->nRecordSize = pSimPBKPara->nECCRecordSize;
				break;
				default:
				break;
			}
			if (pListCountPBK->nCurrentFile == API_SIM_EF_ECC)
			{
#ifdef CFW_MULTI_SIM
				result = SimReadBinaryReq(API_SIM_EF_ECC, 0, pListCountPBK->nRecordSize, nSimID);
				if (ERR_SUCCESS != result)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n", result);
					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0);
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListCountEntryProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0);
				}
#else
				result = SimReadBinaryReq(API_SIM_EF_ECC, 0, pListCountPBK->nRecordSize);
				if (ERR_SUCCESS != result)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n", result);
					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, (UINT32)result, 0, nUTI, 0);
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListCountEntryProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, (UINT32)result, 0, nUTI, 0);
				}
#endif
				pListCountPBK->nState.nCurrState = CFW_SIM_PBK_IDLE;
				pListCountPBK->nState.nNextState = CFW_SIM_PBK_READBINARY;
			}
			else
			{
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT("nCurrentFile=%x\r\n"), pListCountPBK->nCurrentFile);
#ifdef CFW_MULTI_SIM
				result =
				SimReadRecordReq(pListCountPBK->nCurrentFile, pListCountPBK->nStartRecInd, 0x04, pListCountPBK->nRecordSize, nSimID);
#else
				result =
				SimReadRecordReq(pListCountPBK->nCurrentFile, pListCountPBK->nStartRecInd, 0x04, pListCountPBK->nRecordSize);
#endif
				if (ERR_SUCCESS != result)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", result);
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, (UINT32)result, 0, nUTI, 0);
#endif
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListCountEntryProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;

				}
				pListCountPBK->nState.nCurrState = CFW_SIM_PBK_IDLE;
				pListCountPBK->nState.nNextState = CFW_SIM_PBK_READRECORD;
			}
			CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE | CFW_AO_PROC_CODE_CONSUMED);
		}
		break;

		case CFW_SIM_PBK_READRECORD:
	 	{
			// Add by lixp for TBM780(PBK Init failed)at 20080121
			// Because a sim fetch Ind event is coming.But the event isnot filtrated
			if (API_SIM_READRECORD_CNF != nEvtId)
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
				return ERR_SUCCESS;
			}

			pSimReadRecordCnf = (api_SimReadRecordCnf_t*)nEvParam;
			nStructSize       = SIZEOF(CFW_SIM_PBK_ENTRY_INFO);
			if (((0x90 == pSimReadRecordCnf->Sw1) && (0x00 == pSimReadRecordCnf->Sw2)) || (0x91 == pSimReadRecordCnf->Sw1))
			{
				if (pListCountPBK->nStartRecInd == pListCountPBK->nCurrRecInd)
				{
					UINT16 size = (pListCountPBK->nCount) * (SIZEOF(CFW_SIM_PBK_ENTRY_INFO) + SIM_PBK_EXTR_SIZE);

					pGetPBKEntry = (CFW_SIM_PBK_ENTRY_INFO*)CSW_SIM_MALLOC(size);

						if (pGetPBKEntry == NULL)
						{
							CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
#ifdef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xf0);
#endif
							CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListCountEntryProc);
							CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
							hAo = HNULL;
					              return ERR_SUCCESS;

						}
					SUL_ZeroMemory32(pGetPBKEntry, size);
					pListCountPBK->nCurrentP = (UINT32)pGetPBKEntry;
				}
				else
					pGetPBKEntry = (CFW_SIM_PBK_ENTRY_INFO*)(pListCountPBK->nCurrentP);

				pGetPBKEntry->pNumber   = &pGetPBKEntry->iFullNameSize + (UINT32)4;
				pGetPBKEntry->pFullName = &pGetPBKEntry->iFullNameSize + (UINT32)(4 + SIM_PBK_NUMBER_SIZE);
				txtlength               = pListCountPBK->nRecordSize - CFW_SIM_REMAINLENGTH;
				cfw_SimParsePBKRecData(pSimReadRecordCnf->Data, pGetPBKEntry, pListCountPBK, txtlength, API_LISTPBK);
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT("pGetPBKEntry->nNumberSize=%x\r\n"), pGetPBKEntry->nNumberSize);
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT("pListCountPBK->nStructNum is %d \n"), pListCountPBK->nStructNum);

				CFW_GetUTI(hAo, &nUTI);
				if ((pGetPBKEntry->iFullNameSize != 0) || (pGetPBKEntry->nNumberSize != 0) || (pGetPBKEntry->nType == CFW_TELNUMBER_TYPE_INTERNATIONAL))  // updated on 20070618 to read out "+" entry
				{
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT("pListCountPBK->nStructNum is %d \n"), pListCountPBK->nStructNum);
					pListCountPBK->nStructNum++;
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT("pListCountPBK->nStructNum is %d \n"), pListCountPBK->nStructNum);
					pListCountPBK->nCurrentP = (UINT32)(&pGetPBKEntry->iFullNameSize - 8 + nStructSize + SIM_PBK_EXTR_SIZE);
				}
				else if ((pGetPBKEntry->iFullNameSize == 0) && (pGetPBKEntry->nNumberSize == 0) && (pGetPBKEntry->nType != CFW_TELNUMBER_TYPE_INTERNATIONAL)) // updated on 20070618 to read out "+" entry
				{
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT("record is NULL! \n"));
				}
				
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT("pListCountPBK->nStructNum is %d \n"), pListCountPBK->nStructNum);

				if (pListCountPBK->nStructNum < pListCountPBK->nCount)
				{
					pListCountPBK->nCurrRecInd += 1;
#ifdef CFW_MULTI_SIM
					result =
					SimReadRecordReq(pListCountPBK->nCurrentFile, pListCountPBK->nCurrRecInd, 0x04, pListCountPBK->nRecordSize,nSimID);
#else
					result =
					SimReadRecordReq(pListCountPBK->nCurrentFile, pListCountPBK->nCurrRecInd, 0x04, pListCountPBK->nRecordSize);
#endif
					if (ERR_SUCCESS != result)
					{
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, (UINT32)result, 0, nUTI, 0xf0);
#endif
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListCountEntryProc);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;

					}
					pListCountPBK->nState.nNextState = CFW_SIM_PBK_READRECORD;
					pListCountPBK->nState.nCurrState = CFW_SIM_PBK_READRECORD;
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE | CFW_AO_PROC_CODE_CONSUMED);
				}

				else if (pListCountPBK->nStructNum == pListCountPBK->nCount)
				{
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" pListCountPBK->nStructNum is %d \n"), pListCountPBK->nStructNum);
					pGetPBKEntry =
					(CFW_SIM_PBK_ENTRY_INFO*)(pListCountPBK->nCurrentP -
					                        (pListCountPBK->nStructNum) * (nStructSize + SIM_PBK_EXTR_SIZE));

#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, (UINT32)pGetPBKEntry, pListCountPBK->nStructNum,
					                nUTI | (nSimID << 24), 0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, (UINT32)pGetPBKEntry, pListCountPBK->nStructNum, nUTI,0);
#endif
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP Success! \n"));
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListCountEntryProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
			}
			else
			{
#ifdef CFW_MULTI_SIM
				ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2, nSimID);
#else
				ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2);
#endif
				if (ErrorCode == ERR_CME_INVALID_INDEX)
				{
					pGetPBKEntry =
					(CFW_SIM_PBK_ENTRY_INFO*)(pListCountPBK->nCurrentP -
					    (pListCountPBK->nStructNum) * (nStructSize + SIM_PBK_EXTR_SIZE));
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, (UINT32)pGetPBKEntry, pListCountPBK->nStructNum,
					nUTI | (nSimID << 24), 0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, (UINT32)pGetPBKEntry, pListCountPBK->nStructNum, nUTI,
					0);
#endif
				}

				// [[hameina[mod] for bug 9438
				else
				{
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, (UINT32)ErrorCode, pListCountPBK->nCurrRecInd,
										nUTI | (nSimID << 24), 0xF0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, (UINT32)ErrorCode, pListCountPBK->nCurrRecInd, nUTI,
										0xF0);
#endif
				}
				// ]]hameina [mod] for bug 9438
				CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListCountEntryProc);
				CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
				hAo = HNULL;
			}
		}
		break;

		case CFW_SIM_PBK_READBINARY:
		{
			// Add by lixp for TBM780(PBK Init failed)at 20080121
			// Because a sim fetch Ind event is coming.But the event isnot filtrated
			if (API_SIM_READBINARY_CNF != nEvtId)
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
				return ERR_SUCCESS;
			}

			pSimReadBinaryCnf = (api_SimReadBinaryCnf_t*)nEvParam;
			CFW_GetUTI(hAo, &nUTI);
			if (((0x90 == pSimReadBinaryCnf->Sw1) && (0x00 == pSimReadBinaryCnf->Sw2)) || (0x91 == pSimReadBinaryCnf->Sw1))
			{
				nStructSize = 0x00000000;
				UINT8 i = 0x00;

				nStructSize  = SIZEOF(CFW_SIM_PBK_ENTRY_INFO);
				pGetPBKEntry =
				(CFW_SIM_PBK_ENTRY_INFO*)CSW_SIM_MALLOC((pListCountPBK->nCount) *
				                                    (SIZEOF(CFW_SIM_PBK_ENTRY_INFO) + SIM_PBK_EXTR_SIZE));
				if (pGetPBKEntry == NULL)
				{
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0);
#endif
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListCountEntryProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					return ERR_SUCCESS;

				}
				SUL_ZeroMemory32(pGetPBKEntry, (pListCountPBK->nCount) * (SIZEOF(CFW_SIM_PBK_ENTRY_INFO) + SIM_PBK_EXTR_SIZE));

				for (i = 0; i < pListCountPBK->nCount; i++)
				{

					if ((pSimReadBinaryCnf->Data[i* 3] & 0x0F)!=0x0F)
					{
						pGetPBKEntry->pNumber   = &pGetPBKEntry->iFullNameSize + (UINT32)4;
						pGetPBKEntry->pFullName = &pGetPBKEntry->iFullNameSize + (UINT32)(4 + SIM_PBK_NUMBER_SIZE);
						SUL_MemCopy8(pGetPBKEntry->pNumber, pSimReadBinaryCnf->Data + i * 3, 3);
						pGetPBKEntry->nNumberSize   = 3;
						pGetPBKEntry->iFullNameSize = 0;
						pListCountPBK->nStructNum += 1;
						pGetPBKEntry = (CFW_SIM_PBK_ENTRY_INFO*)(&pGetPBKEntry->iFullNameSize - 8 + nStructSize + SIM_PBK_EXTR_SIZE);

					}
				}
				pGetPBKEntry =
				(CFW_SIM_PBK_ENTRY_INFO*)(&pGetPBKEntry->iFullNameSize - 8 -
				                      (UINT32)((pListCountPBK->nStructNum) * (nStructSize + SIM_PBK_EXTR_SIZE)));
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, (UINT32)pGetPBKEntry, pListCountPBK->nStructNum - 1,
									nUTI | (nSimID << 24), 0);
#else
				CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, (UINT32)pGetPBKEntry, pListCountPBK->nStructNum - 1,
									nUTI, 0);
#endif
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP Success! \n"));
			}
			else
			{
#ifdef CFW_MULTI_SIM
			ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2, nSimID);
			CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xF0);
#else
			ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2);
			CFW_PostNotifyEvent(EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP, (UINT32)ErrorCode, 0, nUTI, 0xF0);
			CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
#endif
			}
			CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListCountEntryProc);
			CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
			hAo = HNULL;
		}
		break;

		default:
			CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKListCountEntryProc);
			CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
			hAo = HNULL;
		break;
	}
	return ERR_SUCCESS;
}

UINT32 CFW_SimPBKStorageInfoProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32 nUTI      = 0x00000000;
	UINT32 nEvtId    = 0x00000000;
	UINT32 ErrorCode = 0x00000000;
	UINT32 result    = 0x00000000;
	VOID* nEvParam   = NULL;
	CFW_EV ev;
	UINT16 TotalBytes = 0x0000;

	CFW_PBK_STRORAGE_INFO* pPBKStorageInfo = NULL;
	SIM_SM_INFO_STORAGEINFO* pStorageInfo = NULL;
	api_SimElemFileStatusCnf_t* pSimElemFileStatusCnf = NULL;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimPBKStorageInfoProc);

#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif
	pStorageInfo = CFW_GetAoUserData(hAo);
	if ((UINT32)pEvent == 0xffffffff)
	{
		SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
		ev.nParam1                      = 0xff;
		pEvent                          = &ev;
		pStorageInfo->nState.nNextState = CFW_SIM_PBK_IDLE;
	}
	else
	{
		nEvtId   = pEvent->nEventId;
		nEvParam = (VOID*)pEvent->nParam1;
	}

	switch (pStorageInfo->nState.nNextState)
	{
		case CFW_SIM_PBK_IDLE:
		{
#ifdef CFW_MULTI_SIM
			result = SimElemFileStatusReq(pStorageInfo->nCurrentFile, nSimID);
#else
			result = SimElemFileStatusReq(pStorageInfo->nCurrentFile);
#endif
			if (ERR_SUCCESS != result)
			{
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP, (UINT32)result, 0, nUTI | (nSimID), 0xF0);
#else
				CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP, (UINT32)result, 0, nUTI, 0xF0);
#endif
				CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKStorageInfoProc);
				CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
				hAo = HNULL;
			}
			pStorageInfo->nState.nCurrState = CFW_SIM_PBK_IDLE;
			pStorageInfo->nState.nNextState = CFW_SIM_PBK_READELEMENT;
		}
		break;

		case CFW_SIM_PBK_READELEMENT:
	 	{
		// Add by lixp for TBM780(PBK Init failed)at 20080121
		// Because a sim fetch Ind event is coming.But the event isnot filtrated
			if (API_SIM_ELEMFILESTATUS_CNF != nEvtId)
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
				return ERR_SUCCESS;
			}
			pSimElemFileStatusCnf = (api_SimElemFileStatusCnf_t*)nEvParam;
			if (((0x90 == pSimElemFileStatusCnf->Sw1) && (0x00 == pSimElemFileStatusCnf->Sw2))
			    || (0x91 == pSimElemFileStatusCnf->Sw1))
			{
				pPBKStorageInfo = (CFW_PBK_STRORAGE_INFO*)CSW_SIM_MALLOC(SIZEOF(CFW_PBK_STRORAGE_INFO));
				if (pPBKStorageInfo == NULL)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKStorageInfoProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					return ERR_SUCCESS;

				}
				pStorageInfo->nDataSize = pSimElemFileStatusCnf->Data[14];
				SUL_MemCopy8(&pStorageInfo->nTotalSize, pSimElemFileStatusCnf->Data + 2, 2);
				TotalBytes = pStorageInfo->nTotalSize[0];
				TotalBytes <<= 8;
				TotalBytes &= 0xFF00;
				TotalBytes += pStorageInfo->nTotalSize[1];
				pPBKStorageInfo->index      = TotalBytes / (pStorageInfo->nDataSize);
				pPBKStorageInfo->iNumberLen = SIM_PBK_NUMBER_SIZE;
				if (pStorageInfo->nCurrentFile == API_SIM_EF_BDN)
				{
					pPBKStorageInfo->txtLen = pStorageInfo->nDataSize - CFW_SIM_REMAINLENGTH - 1;
				}
				else
					pPBKStorageInfo->txtLen = pStorageInfo->nDataSize - CFW_SIM_REMAINLENGTH;
				
				CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP, (UINT32)pPBKStorageInfo, SIZEOF(CFW_PBK_STRORAGE_INFO),
									nUTI | (nSimID << 24), 0);
#else
				CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP, (UINT32)pPBKStorageInfo, SIZEOF(CFW_PBK_STRORAGE_INFO),
			                        nUTI, 0);
#endif

				CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_GET_PBK_STRORAGE_INFO_NOTIFY Success! \n"));
				CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKStorageInfoProc);
				CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
				hAo = HNULL;
			}
			else
			{
#ifdef CFW_MULTI_SIM
				ErrorCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2, nSimID);
				CFW_GetUTI(hAo, &nUTI);
				CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xF0);
#else
				ErrorCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2);
				CFW_GetUTI(hAo, &nUTI);
				CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP, (UINT32)ErrorCode, 0, nUTI, 0xF0);
#endif
				CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
				CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKStorageInfoProc);
				CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
				hAo = HNULL;
			}
		}
		break;

		default:
			CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKStorageInfoProc);
			CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
			hAo = HNULL;
		break;
	}
	return ERR_SUCCESS;
}

UINT32 CFW_SimPBKStorageProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32 nUTI      = 0x00000000;
	UINT32 nEvtId    = 0x00000000;
	UINT32 ErrorCode = 0x00000000;
	UINT8 txtlength  = 0x00;
	UINT32 result    = 0x00000000;
	UINT8 i          = 0x00;

	VOID* nEvParam = NULL;
	CFW_EV ev;

	CFW_PBK_STORAGE* pPBKStorage = NULL;
	CFW_SIM_PBK_ENTRY_INFO* pGetPBKEntry = NULL;
	SIM_SM_INFO_STORAGE* pStorage = NULL;
	SIM_PBK_PARAM* pSimPBKPara = NULL;
	api_SimReadRecordCnf_t* pSimReadRecordCnf = NULL;
	api_SimReadBinaryCnf_t* pSimReadBinaryCnf = NULL;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SimPBKStorageProc);

#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif

	pStorage = CFW_GetAoUserData(hAo);
	if ((UINT32)pEvent == 0xffffffff)
	{
		SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
		ev.nParam1                  = 0xff;
		pEvent                      = &ev;
		pStorage->nState.nNextState = CFW_SIM_PBK_IDLE;
	}
	else
	{
		nEvtId   = pEvent->nEventId;
		nEvParam = (VOID*)pEvent->nParam1;
	}

	switch (pStorage->nState.nNextState)
	{
		case CFW_SIM_PBK_IDLE:
		{
#ifdef CFW_MULTI_SIM
			CFW_CfgSimGetPbkParam(&pSimPBKPara, nSimID);
#else
			CFW_CfgSimGetPbkParam(&pSimPBKPara);
#endif
			switch (pStorage->nCurrentFile)
			{
				case API_SIM_EF_ADN:
				pStorage->nDataSize   = pSimPBKPara->nADNRecordSize;
				pStorage->nTotalIndex = pSimPBKPara->nADNTotalNum;
				txtlength             = pSimPBKPara->nADNRecordSize - CFW_SIM_REMAINLENGTH;
				break;
				case API_SIM_EF_FDN:
				pStorage->nDataSize   = pSimPBKPara->nFDNRecordSize;
				pStorage->nTotalIndex = pSimPBKPara->nFDNTotalNum;
				txtlength             = pSimPBKPara->nFDNRecordSize - CFW_SIM_REMAINLENGTH;
				break;
				case API_SIM_EF_MSISDN:
				pStorage->nDataSize   = pSimPBKPara->nMSISDNRecordSize;
				pStorage->nTotalIndex = pSimPBKPara->nMSISDNTotalNum;
				txtlength             = pSimPBKPara->nMSISDNRecordSize - CFW_SIM_REMAINLENGTH;
				break;
				case API_SIM_EF_LND:
				pStorage->nDataSize   = pSimPBKPara->nLNDRecordSize;
				pStorage->nTotalIndex = pSimPBKPara->nLNDTotalNum;
				txtlength             = pSimPBKPara->nLNDRecordSize - CFW_SIM_REMAINLENGTH;
				break;
				case API_SIM_EF_ECC:
				pStorage->nDataSize   = 3;
				pStorage->nTotalIndex = pSimPBKPara->nECCTotalNum;
				txtlength             = 0;
				break;
				default:
				break;
			}

			if (pStorage->nCurrentFile == API_SIM_EF_ECC)
			{
#ifdef CFW_MULTI_SIM
				result = SimReadBinaryReq(API_SIM_EF_ECC, 0, 3 * pStorage->nTotalIndex, nSimID);
#else
				result = SimReadBinaryReq(API_SIM_EF_ECC, 0, 3* pStorage->nTotalIndex);
#endif
				if (ERR_SUCCESS != result)
				{
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_RSP, (UINT32)result, 0, nUTI, 0xF0);
#endif
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKStorageProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;

				}
				pStorage->nState.nCurrState = CFW_SIM_PBK_IDLE;
				pStorage->nState.nNextState = CFW_SIM_PBK_READBINARY;
			}
			else
			{
				pStorage->nCurrentIndex = 1;
				pStorage->nNotUsedIndex = 0;

#ifdef CFW_MULTI_SIM
				result = SimReadRecordReq(pStorage->nCurrentFile, 0x01, 0x04, pStorage->nDataSize, nSimID);
#else
				result = SimReadRecordReq(pStorage->nCurrentFile, 0x01, 0x04, pStorage->nDataSize);
#endif
				if (ERR_SUCCESS != result)
				{
					CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", result);
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_RSP, (UINT32)result, 0, nUTI, 0xF0);
#endif
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKStorageProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;

				}
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE | CFW_AO_PROC_CODE_CONSUMED);
				pStorage->nState.nCurrState = CFW_SIM_PBK_IDLE;
				pStorage->nState.nNextState = CFW_SIM_PBK_READRECORD;
			}
			CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE | CFW_AO_PROC_CODE_CONSUMED);
		}
		break;

		case CFW_SIM_PBK_READRECORD:
		{
			pSimReadRecordCnf = (api_SimReadRecordCnf_t*)nEvParam;
			// Add by lixp for TBM780(PBK Init failed)at 20080121
			// Because a sim fetch Ind event is coming.But the event isnot filtrated
			if (API_SIM_READRECORD_CNF != nEvtId)
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
				return ERR_SUCCESS;
			}

			if (((0x90 == pSimReadRecordCnf->Sw1) && (0x00 == pSimReadRecordCnf->Sw2)) || (0x91 == pSimReadRecordCnf->Sw1))
			{
				pGetPBKEntry = (CFW_SIM_PBK_ENTRY_INFO*)CSW_SIM_MALLOC(SIZEOF(CFW_SIM_PBK_ENTRY_INFO) + SIM_PBK_EXTR_SIZE);
				if (pGetPBKEntry == NULL)
				{
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKStorageProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
					return ERR_SUCCESS;
				}

				pGetPBKEntry->pNumber   = &pGetPBKEntry->iFullNameSize + (UINT32)4;
				pGetPBKEntry->pFullName = &pGetPBKEntry->iFullNameSize + (UINT32)(4 + SIM_PBK_NUMBER_SIZE);
				txtlength               = pStorage->nDataSize - CFW_SIM_REMAINLENGTH;
				cfw_SimParsePBKRecData(pSimReadRecordCnf->Data, pGetPBKEntry, pStorage, txtlength, API_GETSTORAGE);
				if ((pGetPBKEntry->iFullNameSize == 0) && (pGetPBKEntry->nNumberSize == 0) && (pGetPBKEntry->nType != CFW_TELNUMBER_TYPE_INTERNATIONAL))  // updated on 20070618 to read out "+" entry
				{
					pStorage->nNotUsedIndex += 1;
				}
				CSW_TRACE(CFW_SIM_TS_ID, "NotUsed RecNum %d \n", pStorage->nNotUsedIndex);
				if (pStorage->nCurrentIndex < pStorage->nTotalIndex)
				{
					pStorage->nCurrentIndex += 1;
					CSW_SIM_FREE(pGetPBKEntry);
#ifdef CFW_MULTI_SIM
					result = SimReadRecordReq(pStorage->nCurrentFile, pStorage->nCurrentIndex, 0x04, pStorage->nDataSize, nSimID);
#else
					result = SimReadRecordReq(pStorage->nCurrentFile, pStorage->nCurrentIndex, 0x04, pStorage->nDataSize);
#endif
					if (ERR_SUCCESS != result)
					{
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_RSP, (UINT32)result, 0, nUTI, 0xF0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n", result);

						CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKStorageProc);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;

					}
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE | CFW_AO_PROC_CODE_CONSUMED);
				}
				else
				{
					CFW_GetUTI(hAo, &nUTI);
					pPBKStorage = (CFW_PBK_STORAGE*)CSW_SIM_MALLOC(SIZEOF(CFW_PBK_STORAGE));
					if (pPBKStorage == NULL)
					{
						CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
						CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
						CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKStorageProc);
						CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
						hAo = HNULL;
					       return ERR_SUCCESS;

					}
					pPBKStorage->totalSlot = pStorage->nTotalIndex;
					pPBKStorage->usedSlot  = pStorage->nTotalIndex - pStorage->nNotUsedIndex;
					switch (pStorage->nCurrentFile)
					{
						case API_SIM_EF_ADN:
						pPBKStorage->storageId = CFW_PBK_SIM;
						break;
						case API_SIM_EF_FDN:
						pPBKStorage->storageId = CFW_PBK_SIM_FIX_DIALLING;
						break;
						case API_SIM_EF_LND:
						pPBKStorage->storageId = CFW_PBK_SIM_LAST_DIALLING;
						break;
						case API_SIM_EF_MSISDN:
						pPBKStorage->storageId = CFW_PBK_ON;
						break;
						case API_SIM_EF_ECC:
						pPBKStorage->storageId = CFW_PBK_EN;
						break;
						default:
						break;
					}
					pPBKStorage->padding[0] = 0xFF;
					pPBKStorage->padding[1] = 0xFF;
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_RSP, (UINT32)pPBKStorage, SIZEOF(CFW_PBK_STORAGE),
										nUTI | (nSimID << 24), 0);
#else
					CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_RSP, (UINT32)pPBKStorage, SIZEOF(CFW_PBK_STORAGE), nUTI, 0);
#endif
					CSW_SIM_FREE(pGetPBKEntry);
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_GET_PBK_STRORAGE_RSP Success! \n"));
					CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKStorageProc);
					CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
					hAo = HNULL;
				}
			}
			else
			{
#ifdef CFW_MULTI_SIM
				ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2, nSimID);
				CFW_GetUTI(hAo, &nUTI);
				CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_RSP, (UINT32)ErrorCode, pStorage->nCurrentIndex,
									nUTI | (nSimID << 24), 0xF0);
#else
				ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2);
				CFW_GetUTI(hAo, &nUTI);
				CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_RSP, (UINT32)ErrorCode, pStorage->nCurrentIndex, nUTI, 0xF0);
#endif
				CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKStorageProc);
				CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
				hAo = HNULL;
			}
		}
		break;

		case CFW_SIM_PBK_READBINARY:
		{
			pSimReadBinaryCnf = (api_SimReadBinaryCnf_t*)nEvParam;

			// 
			// Add by lixp for TBM780(PBK Init failed)at 20080121
			// Because a sim fetch Ind event is coming.But the event isnot filtrated
			// 
			if (API_SIM_READBINARY_CNF != nEvtId)
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
				return ERR_SUCCESS;
			}

			CFW_GetUTI(hAo, &nUTI);
			if (((0x90 == pSimReadBinaryCnf->Sw1) && (0x00 == pSimReadBinaryCnf->Sw2)) || (0x91 == pSimReadBinaryCnf->Sw1))
			{

				for (i = 0; i < (pStorage->nTotalIndex); i++)
				{

					if (((pSimReadBinaryCnf->Data[i* 3])&0x0F) != 0x0F)
					{
						pPBKStorage->usedSlot += 1;
					}

				}
				if (pStorage->nCurrentFile == API_SIM_EF_ECC)
				{
					pPBKStorage->storageId = CFW_PBK_EN;
				}
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_RSP, (UINT32)pPBKStorage, SIZEOF(CFW_PBK_STORAGE),
				nUTI | (nSimID << 24), 0);
#else
				CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_RSP, (UINT32)pPBKStorage, SIZEOF(CFW_PBK_STORAGE), nUTI, 0);
#endif
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_GET_PBK_STRORAGE_RSP Success! \n"));
			}
			else
			{
#ifdef CFW_MULTI_SIM
				ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2, nSimID);
				CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xF0);
#else
				ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2);
				CFW_PostNotifyEvent(EV_CFW_SIM_GET_PBK_STRORAGE_RSP, (UINT32)ErrorCode, 0, nUTI, 0xF0);
#endif
				CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 ErrorCode 0x%x \n", ErrorCode);
			}
			CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKStorageProc);
			CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
			hAo = HNULL;
		}
		break;

		default:
			CSW_PROFILE_FUNCTION_EXIT(CFW_SimPBKStorageProc);
			CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
			hAo = HNULL;
		break;
	}
	
	return ERR_SUCCESS;
}

UINT32 cfw_SimParsePBKRecData(UINT8* pData, 
							  CFW_SIM_PBK_ENTRY_INFO * pGetPBKEntry,
							  void* proc,
							  UINT8 txtlength,
                              UINT8 nApiNum
                             )
{
	UINT8 i = 0x00;
	WORD wNameStep;
	WORD wBasePoint;
	WORD wConverted;
	UINT16 uLen = 0x0000;
	UINT8 pName[SIM_PBK_NAME_SIZE];

	CSW_PROFILE_FUNCTION_ENTER(cfw_SimParsePBKRecData);

	// CSW_TC_MEMBLOCK(CFW_SIM_TS_ID, pData, txtlength, 16);
	if (pData[0] < 0x7F)
	{

		for (i = 0; i < txtlength + 1; i++)
		{
			if (pData[i] == 0xFF)
			break;
		}

		// [[hameina[mod] 2008-10-09, bug 9565
		// pGetPBKEntry->iFullNameSize = i + 1;

		if (i > txtlength)
		pGetPBKEntry->iFullNameSize = txtlength;
		else
		pGetPBKEntry->iFullNameSize = i;

		// ]]hameina[mod]

		if (pGetPBKEntry->iFullNameSize > SIM_PBK_NAME_SIZE)
		{
			pGetPBKEntry->iFullNameSize = SIM_PBK_NAME_SIZE;
		}

		SUL_MemCopy8(pGetPBKEntry->pFullName, pData, pGetPBKEntry->iFullNameSize);
	}

	// convert to standard UCS2 if necessary
	else if (pData[0] == (BYTE)0x81)
	{
		// UINT8 data[SIM_PBK_NAME_SIZE] = {0x00};

		uLen = pData[1] * 2 + 1;

		// add by wanghb 2007.12.13 begin
		if (uLen > SIM_PBK_NAME_SIZE)
		{
			uLen = SIM_PBK_NAME_SIZE;
		}

		SUL_MemSet8(pName, 0xFF, SIM_PBK_NAME_SIZE);

		pName[0]   = (BYTE)0x80;
		wNameStep  = (WORD) pData[2];
		wBasePoint = (wNameStep << 7) & 0x7F80;

		for (wNameStep = 0; wNameStep < (WORD) ((uLen - 1) / 2); wNameStep++)
		{
			if (0x80 == (pData[3 + wNameStep] & 0x80))
			{
				wConverted = wBasePoint + (WORD) (pData[3 + wNameStep] & 0x7F);
			}
			else
			{
				wConverted = 0x0000 + (WORD) pData[3 + wNameStep];
			}

			pName[1 + wNameStep* 2] = (BYTE)((wConverted & 0xFF00) >> 8);
			pName[1 + wNameStep * 2 + 1] = (BYTE)(wConverted & 0x00FF);
		}

		// SUL_MemCopy8(data,pName,uLen);

		if (pData[1] == 0x00 || 0xFF == pData[1])
		{
			pGetPBKEntry->iFullNameSize = 0;
		}

		// Add by wanghb at 20071214
		// For TD136 sim pbk issue 
		else
		{
			pGetPBKEntry->iFullNameSize = uLen;

		}

		SUL_MemCopy8(pGetPBKEntry->pFullName, pName, pGetPBKEntry->iFullNameSize);

	}
	else if (pData[0] == (BYTE)0x82)
	{

		// add by wanghb 2007.12.13 begin
		// UINT8 data[SIM_PBK_NAME_SIZE] = {0x00};
		uLen = pData[1] * 2 + 1;

		if (uLen > SIM_PBK_NAME_SIZE)
		uLen = SIM_PBK_NAME_SIZE;

		SUL_MemSet8(pName, 0xFF, SIM_PBK_NAME_SIZE);

		pName[0]   = (BYTE)0x80;
		wBasePoint = (WORD) pData[2];
		wBasePoint <<= 8;
		wBasePoint += (BYTE)pData[3];

		for (wNameStep = 0; wNameStep < (WORD) ((uLen - 1) / 2); wNameStep++)
		{
			if (0x80 == (pData[4 + wNameStep] & 0x80))
			{
				wConverted = wBasePoint + (WORD) (pData[4 + wNameStep] & 0x7F);
			}
			else
			{
				wConverted = (WORD) pData[4 + wNameStep];
			}

			pName[1 + wNameStep * 2] = (BYTE)((wConverted & 0xFF00) >> 8);
			pName[1 + wNameStep * 2 + 1] = (BYTE)(wConverted & 0x00FF);
		}

		// SUL_MemCopy8(data, pName, uLen);

		if (pData[1] == 0xFF)
		{
			pGetPBKEntry->iFullNameSize = 0;
		}
		else
		{
			pGetPBKEntry->iFullNameSize = uLen;
		}

		// add by wanghb 2007.12.13 end

		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("0x80 Name size %d\n"), pGetPBKEntry->iFullNameSize);

		// Add by wanghb at 20071214
		// For Td136issue 
		// SUL_MemCopy8(pGetPBKEntry->pFullName, pData, pGetPBKEntry->iFullNameSize);
		SUL_MemCopy8(pGetPBKEntry->pFullName, pName, pGetPBKEntry->iFullNameSize);
	}

	// end conversion    
	else if (pData[0] == 0x80)
	{
		if (pData[1] == 0xFF)
		{
			pGetPBKEntry->iFullNameSize = 0;
		}
		for (i = 0; i < (txtlength - 1) / 2; i++)
		{
			if ((pData[2 * i + 1] == 0xFF) && (pData[2 * i + 2] == 0xFF))
			break;

			pGetPBKEntry->iFullNameSize = 2 * (i + 1) + 1;
		}

		if (pGetPBKEntry->iFullNameSize > SIM_PBK_NAME_SIZE)
		{
			pGetPBKEntry->iFullNameSize = SIM_PBK_NAME_SIZE;
		}

		// 
		// modify by lixp for TBM780 bug at 20080121
		// (!(pGetPBKEntry->iFullNameSize & 0x01))
		if ((!(pGetPBKEntry->iFullNameSize & 0x01)) && (pGetPBKEntry->iFullNameSize))
		{
			pGetPBKEntry->iFullNameSize--;
		}

		SUL_MemCopy8(pGetPBKEntry->pFullName, pData, pGetPBKEntry->iFullNameSize);
	}
	else
	{
		pGetPBKEntry->iFullNameSize = 0;
		//CSW_TRACE(CFW_SIM_TS_ID, TSTXT("REC_Name NULL!\n"));
	}

	if (nApiNum == API_GETPBK)
	{
		if (pData[(((SIM_SM_INFO_GETPBK*)proc)->nRecordSize - CFW_SIM_REMAINLENGTH)] > SIM_PBK_NAME_SIZE
		|| (pData[(((SIM_SM_INFO_GETPBK*)proc)->nRecordSize - CFW_SIM_REMAINLENGTH)] - 1 <= 0))
		{
			pGetPBKEntry->nNumberSize = 0;
		}
		else
		{
			pGetPBKEntry->nNumberSize = pData[(((SIM_SM_INFO_GETPBK*)proc)->nRecordSize - CFW_SIM_REMAINLENGTH)] - 1;
			if (pGetPBKEntry->nNumberSize > 10)
			{
				pGetPBKEntry->nNumberSize = 10;
			}
			else
			{
				SUL_MemCopy8(pGetPBKEntry->pNumber, pData + ((SIM_SM_INFO_GETPBK*)proc)->nRecordSize - 12,
					         (UINT8)(pData[(((SIM_SM_INFO_GETPBK*)proc)->nRecordSize - CFW_SIM_REMAINLENGTH)] - 1));
			}
		}
		pGetPBKEntry->phoneIndex = ((SIM_SM_INFO_GETPBK*)proc)->nCurrRecInd;
		pGetPBKEntry->nType      = pData[((SIM_SM_INFO_GETPBK*)proc)->nRecordSize - 13];
	}
	else if (nApiNum == API_LISTPBK)
	{
		if ((pData[(((SIM_SM_INFO_LISTPBK*)proc)->nRecordSize - CFW_SIM_REMAINLENGTH)] > SIM_PBK_NAME_SIZE)
		|| (pData[(((SIM_SM_INFO_LISTPBK*)proc)->nRecordSize - CFW_SIM_REMAINLENGTH)] - 1 <= 0))
		{
			pGetPBKEntry->nNumberSize = 0;
		}
		else
		{
			pGetPBKEntry->nNumberSize = pData[(((SIM_SM_INFO_LISTPBK*)proc)->nRecordSize - CFW_SIM_REMAINLENGTH)] - 1;
			if (pGetPBKEntry->nNumberSize > 10)
			{
				pGetPBKEntry->nNumberSize = 10;
			}
			else
			{
				SUL_MemCopy8(pGetPBKEntry->pNumber, pData + ((SIM_SM_INFO_LISTPBK*)proc)->nRecordSize - 12,
					         (UINT8)(pData[(((SIM_SM_INFO_LISTPBK*)proc)->nRecordSize - CFW_SIM_REMAINLENGTH)] - 1));
			}
		}
		pGetPBKEntry->phoneIndex = ((SIM_SM_INFO_LISTPBK*)proc)->nCurrRecInd;
		pGetPBKEntry->nType      = pData[((SIM_SM_INFO_LISTPBK*)proc)->nRecordSize - 13];
	}
	else if (nApiNum == API_GETSTORAGE)
	{
		if (pData[(((SIM_SM_INFO_STORAGE*)proc)->nDataSize - CFW_SIM_REMAINLENGTH)] > SIM_PBK_NAME_SIZE
		|| (pData[(((SIM_SM_INFO_STORAGE*)proc)->nDataSize - CFW_SIM_REMAINLENGTH)] - 1 <= 0))
		{
			pGetPBKEntry->nNumberSize = 0;
		}
		else
		{
			pGetPBKEntry->nNumberSize = pData[(((SIM_SM_INFO_STORAGE*)proc)->nDataSize - CFW_SIM_REMAINLENGTH)] - 1;
			if (pGetPBKEntry->nNumberSize > 10)
			{
				pGetPBKEntry->nNumberSize = 10;
			}
			else
			{
				SUL_MemCopy8(pGetPBKEntry->pNumber, pData + ((SIM_SM_INFO_STORAGE*)proc)->nDataSize - 12,
					         (UINT8)(pData[(((SIM_SM_INFO_STORAGE*)proc)->nDataSize - CFW_SIM_REMAINLENGTH)] - 1));
			}
		}
		pGetPBKEntry->phoneIndex = ((SIM_SM_INFO_STORAGE*)proc)->nCurrentIndex;
		pGetPBKEntry->nType      = pData[((SIM_SM_INFO_STORAGE*)proc)->nDataSize - 13];

	}
	CSW_PROFILE_FUNCTION_EXIT(cfw_SimParsePBKRecData);
	return 0;
}

#endif // 
 
