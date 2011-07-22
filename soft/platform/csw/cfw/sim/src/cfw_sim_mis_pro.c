// //////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2002-2009, Coolsand Technologies, Inc.
// All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmission of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
// 
// FILENAME: cfw_sim_mis_pro.c
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
#include <ts.h>
#include "csw_debug.h"

#include <base_prv.h>

#include "cfw_sim.h"
#include "cfw_sim_boot.h"
#if 0
#ifdef CFW_MULTI_SIM

//CFW_SIM_STATUS gSimStatus[CFW_SIM_COUNT] = { CFW_SIM_STATUS_END, CFW_SIM_STATUS_END };


CFW_SPN_INFO gSPNInfo[CFW_SIM_COUNT] = {{0x00,0x00,{0x00,}},{0x00,0x00,{0x00,}}};
UINT32 CFW_GetSimSPN(CFW_SIM_ID nSimID, CFW_SPN_INFO *pSPNInfo)
{  
  SUL_MemCopy8(pSPNInfo,&(gSPNInfo[nSimID]),SIZEOF(CFW_SPN_INFO));
  CSW_TRACE(CFW_SIM_TS_ID, "pSPNInfo.nSPNDisplayFlag 0x%x \n", pSPNInfo->nSPNDisplayFlag);
  CSW_TRACE(CFW_SIM_TS_ID, "pSPNInfo.nSPNameLen 0x%x \n", pSPNInfo->nSPNameLen);
  CSW_TC_MEMBLOCK(CFW_SIM_TS_ID,pSPNInfo->nSpnName,pSPNInfo->nSPNameLen,16);
  CSW_TRACE(CFW_SIM_TS_ID, "nSpnName %s \n", pSPNInfo->nSpnName);
  return ERR_SUCCESS;
}
UINT32 CFW_SetSimSPN(CFW_SIM_ID nSimID, CFW_SPN_INFO nSPNInfo)
{
  SUL_MemCopy8(&(gSPNInfo[nSimID]),&nSPNInfo,SIZEOF(CFW_SPN_INFO));
  return ERR_SUCCESS;
}
#else
CFW_SPN_INFO gSPNInfo = {0x00,0x00,{0x00,}};
UINT32 CFW_GetSimSPN(CFW_SPN_INFO *pSPNInfo)
{  
  SUL_MemCopy8(pSPNInfo,&gSPNInfo,SIZEOF(CFW_SPN_INFO));
  return ERR_SUCCESS;
}
UINT32 CFW_SetSimSPN(CFW_SPN_INFO nSPNInfo)
{
  SUL_MemCopy8(&(gSPNInfo),&nSPNInfo,SIZEOF(CFW_SPN_INFO));
  return ERR_SUCCESS;
}
#endif
#endif
static UINT32 CFW_GetSPNLen(UINT8 nSPN[16])
{
  UINT8 i = 0x00;
  if(nSPN[0] > 0x7F)
  {
    //UCS2
    UINT16 *p = (UINT16 *)nSPN;
    for(;i < 8; i++)
    {
      if(*p == 0xFFFF)
        break;
    }
  }
  else
  {
    for(;i < 16; i++)
    {
      if(nSPN[i] == 0xFF)
        break;
    }
  }
  return i;
}
#if 0
#ifdef CFW_MULTI_SIM
static UINT8 gMNCLen[CFW_SIM_COUNT] = {0x00,};
VOID CFW_SetMNCLen(UINT8 nLen,CFW_SIM_ID nSimID)
{
   gMNCLen[nSimID] = nLen;
}
VOID CFW_GetMNCLen(UINT8 *pLen,CFW_SIM_ID nSimID)
{
   *pLen = gMNCLen[nSimID] ;
}
#else
static UINT8 gMNCLen = 0x00;
VOID CFW_SetMNCLen(UINT8 nLen )
{
   gMNCLen = nLen;
}
VOID CFW_GetMNCLen(UINT8 *pLen )
{
   *pLen = gMNCLen ;
}
#endif
#endif



#define CFW_CHV_ET /*CHV Early treatment*/
#ifdef CFW_MULTI_SIM
extern CFW_SIM_OC_STATUS SIMOCStatus[]  ;
#else
extern CFW_SIM_OC_STATUS SIMOCStatus ;
#endif


#ifdef CFW_MULTI_SIM
BOOL g_GETIMSI[CFW_SIM_COUNT] = {FALSE,FALSE};
#else
BOOL g_GETIMSI = FALSE;
#endif


UINT32 CFW_SimMisSetPrefListProc(HAO hAo, CFW_EV* pEvent)
{
  UINT32 nUTI      = 0x00000000;
  UINT32 nEvtId    = 0x00000000;
  UINT32 ErrorCode = 0x00000000;
  UINT32 result    = 0x00000000;
  VOID* nEvParam   = NULL;
  CFW_EV ev;

  SIM_SM_INFO_SETPREFOPREATORLIST* pMisSetPrefList = NULL;
  api_SimUpdateBinaryCnf_t* pSimUpdateBinaryCnf = NULL;
  api_SimElemFileStatusCnf_t* pSimElemFileStatusCnf = NULL;
  api_SimReadBinaryCnf_t * pSimReadBinaryCnf = NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimMisSetPrefListProc);
#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hAo);
#endif
  pMisSetPrefList = CFW_GetAoUserData(hAo);
  if ((UINT32)pEvent == 0xffffffff)
  {
    SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
    ev.nParam1                         = 0xff;
    pEvent                             = &ev;
    pMisSetPrefList->nState.nNextState = CFW_SIM_MIS_IDLE;
  }
  else
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }

  switch (pMisSetPrefList->nState.nNextState)
  {
  case CFW_SIM_MIS_IDLE:
#ifdef CFW_MULTI_SIM
    result = SimElemFileStatusReq(API_SIM_EF_PLMNSEL, nSimID);
    if (ERR_SUCCESS != result)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", result);

      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
      hAo = HNULL;
      CFW_PostNotifyEvent(EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
    }
#else
    result = SimElemFileStatusReq(API_SIM_EF_PLMNSEL);
    if (ERR_SUCCESS != result)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", result);

      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
      hAo = HNULL;
      CFW_PostNotifyEvent(EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP, (UINT32)result, 0, nUTI, 0xF0);
    }
#endif
    pMisSetPrefList->nState.nCurrState = CFW_SIM_MIS_IDLE;
    pMisSetPrefList->nState.nNextState = CFW_SIM_MIS_READELEMENT;
    break;

  case CFW_SIM_MIS_READELEMENT:
    pSimElemFileStatusCnf = (api_SimElemFileStatusCnf_t*)nEvParam;
    CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
    if (((0x90 == pSimElemFileStatusCnf->Sw1) && (0x00 == pSimElemFileStatusCnf->Sw2))
        || (0x91 == pSimElemFileStatusCnf->Sw1))

    {
      pMisSetPrefList->nSize = ((pSimElemFileStatusCnf->Data[2] << 8) & 0xFF00) + pSimElemFileStatusCnf->Data[3];
      CSW_TRACE(CFW_SIM_TS_ID, "Prelist Size 0x%x \n", pMisSetPrefList->nSize);
      //CSW_TC_MEMBLOCK(CFW_SIM_TS_ID,pMisSetPrefList->nOperatorList,pMisSetPrefList->nSize,16);

      result                 =
        SimUpdateBinaryReq(API_SIM_EF_PLMNSEL, 0, pMisSetPrefList->nSize, pMisSetPrefList->nOperatorList, nSimID);
      if (ERR_SUCCESS != result)
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", result);

        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
        hAo = HNULL;
        CFW_PostNotifyEvent(EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
      }
      pMisSetPrefList->nState.nCurrState = CFW_SIM_MIS_IDLE;
      pMisSetPrefList->nState.nNextState = CFW_SIM_MIS_UPDATEBINARY;
    }
    else
    {
      ErrorCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2, nSimID);
      CFW_PostNotifyEvent(EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xF0);
      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
      hAo = HNULL;
    }

#else
    if (((0x90 == pSimElemFileStatusCnf->Sw1) && (0x00 == pSimElemFileStatusCnf->Sw2))
        || (0x91 == pSimElemFileStatusCnf->Sw1))

    {
      pMisSetPrefList->nSize = ((pSimElemFileStatusCnf->Data[2] << 8) & 0xFF00) + pSimElemFileStatusCnf->Data[3];
      result                 = SimUpdateBinaryReq(API_SIM_EF_PLMNSEL, 0, pMisSetPrefList->nSize, pMisSetPrefList->nOperatorList);
      if (ERR_SUCCESS != result)
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateBinaryReq return 0x%x \n", result);
        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
        hAo = HNULL;
        CFW_PostNotifyEvent(EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP, (UINT32)result, 0, nUTI, 0xF0);
      }
      pMisSetPrefList->nState.nCurrState = CFW_SIM_MIS_IDLE;
      pMisSetPrefList->nState.nNextState = CFW_SIM_MIS_UPDATEBINARY;
    }
    else
    {
      ErrorCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2);
      CFW_PostNotifyEvent(EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP, (UINT32)ErrorCode, 0, nUTI, 0xF0);
      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
      hAo = HNULL;
    }

#endif
    break;

  case CFW_SIM_MIS_UPDATEBINARY:
    pSimUpdateBinaryCnf = (api_SimUpdateBinaryCnf_t*)nEvParam;
    CFW_GetUTI(hAo, &nUTI);

#ifdef CFW_MULTI_SIM
    if (((0x90 == pSimUpdateBinaryCnf->Sw1) && (0x00 == pSimUpdateBinaryCnf->Sw2))
        || (0x91 == pSimUpdateBinaryCnf->Sw1))
    {

      result = SimReadBinaryReq(API_SIM_EF_PLMNSEL, 0, pMisSetPrefList->nSize, nSimID);
      if (ERR_SUCCESS != result)
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n",result);
        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
        hAo = HNULL;
        CFW_PostNotifyEvent(EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
      }
    }
    else
    {
      ErrorCode = Sim_ParseSW1SW2(pSimUpdateBinaryCnf->Sw1, pSimUpdateBinaryCnf->Sw2, nSimID);
      CFW_PostNotifyEvent(EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xF0);
      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
      hAo = HNULL;
    }

#else
    if (((0x90 == pSimUpdateBinaryCnf->Sw1) && (0x00 == pSimUpdateBinaryCnf->Sw2))
        || (0x91 == pSimUpdateBinaryCnf->Sw1))
    {

      result = SimReadBinaryReq(API_SIM_EF_PLMNSEL, 0, pMisSetPrefList->nSize);
      if (ERR_SUCCESS != result)
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n",result);
        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
        hAo = HNULL;
        CFW_PostNotifyEvent(EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP, (UINT32)result, 0, nUTI, 0xF0);
      }
    }
    else
    {
      ErrorCode = Sim_ParseSW1SW2(pSimUpdateBinaryCnf->Sw1, pSimUpdateBinaryCnf->Sw2);
      CFW_PostNotifyEvent(EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP, (UINT32)ErrorCode, 0, nUTI, 0xF0);
      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
      hAo = HNULL;
    }

#endif
    pMisSetPrefList->nState.nCurrState = CFW_SIM_MIS_UPDATEBINARY;
    pMisSetPrefList->nState.nNextState = CFW_SIM_MIS_READBINARY;
    break;

  case CFW_SIM_MIS_READBINARY:
    pSimReadBinaryCnf = (api_SimReadBinaryCnf_t*)nEvParam;;
#ifdef CFW_MULTI_SIM
    if (((0x90 == pSimReadBinaryCnf->Sw1) && (0x00 == pSimReadBinaryCnf->Sw2))
        || (0x91 == pSimReadBinaryCnf->Sw1))
    {      
      CSW_TRACE(CFW_SIM_TS_ID, "Save Data Size 0x%x \n", pMisSetPrefList->nSize);
      CSW_TC_MEMBLOCK(CFW_SIM_TS_ID,pMisSetPrefList->nOperatorList,pMisSetPrefList->nSize,16);
      CSW_TRACE(CFW_SIM_TS_ID, "Sim Data Size 0x%x \n", pMisSetPrefList->nSize);
      CSW_TC_MEMBLOCK(CFW_SIM_TS_ID,pSimReadBinaryCnf->Data,pMisSetPrefList->nSize,16);

      
      UINT32 nCompResult = 0x00;
      nCompResult = SUL_MemCompare(pSimReadBinaryCnf->Data, pMisSetPrefList->nOperatorList, pMisSetPrefList->nSize);
      if( 0x00 == nCompResult )
      {
        CFW_PostNotifyEvent(EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP, (UINT32)0, 0, nUTI | (nSimID << 24), 0x00);
        CSW_TRACE(CFW_SIM_TS_ID, TSTXT("EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP Success\n"));

      }
      else
      {
        CFW_PostNotifyEvent(EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP, (UINT32)0, 0, nUTI | (nSimID << 24), 0xF0);
        CSW_TRACE(CFW_SIM_TS_ID, TSTXT("EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP Failed Write failed\n"));

      }
      
    }
    else
    {
      ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2, nSimID);
      CFW_PostNotifyEvent(EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xF0);
      CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP ErrorCode %x\n"),ErrorCode);
    }
#else
    if (((0x90 == pSimReadBinaryCnf->Sw1) && (0x00 == pSimReadBinaryCnf->Sw2))
        || (0x91 == pSimReadBinaryCnf->Sw1))
    {      
      UINT32 nCompResult = 0x00;
      nCompResult = SUL_MemCompare(pSimReadBinaryCnf->Data, pMisSetPrefList->nOperatorList, pMisSetPrefList->nSize);
      if( 0x00 == nCompResult )
      {
        CFW_PostNotifyEvent(EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP, (UINT32)0, 0, nUTI, 0x00);
        CSW_TRACE(CFW_SIM_TS_ID, TSTXT("EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP Success\n"));

      }
      else
      {
        CFW_PostNotifyEvent(EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP, (UINT32)0, 0, nUTI, 0xF0);
        CSW_TRACE(CFW_SIM_TS_ID, TSTXT("EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP Failed Write failed\n"));

      }
    }
    else
    {
      ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2);
      CFW_PostNotifyEvent(EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP, (UINT32)ErrorCode, 0, nUTI, 0xF0);
      CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP ErrorCode %x\n"),ErrorCode);
    }
#endif

    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
    hAo = HNULL;
    break;

  default:
    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
    hAo = HNULL;
    break;
  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimMisSetPrefListProc);
  return ERR_SUCCESS;
}

UINT32 CFW_SimMisGetPrefListProc(HAO hAo, CFW_EV* pEvent)
{
  UINT32 nUTI      = 0x00000000;
  UINT32 nEvtId    = 0x00000000;
  UINT32 ErrorCode = 0x00000000;
  UINT32 result    = 0x00000000;
  VOID* nEvParam   = NULL;
  CFW_EV ev;

  SIM_SM_INFO_GETPREFOPREATORLIST* pMisGetPreflist = NULL;
  api_SimReadBinaryCnf_t* pSimReadBinaryCnf = NULL;
  api_SimElemFileStatusCnf_t* pSimElemFileStatusCnf = NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimMisGetPrefListProc);
#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hAo);
#endif
  pMisGetPreflist = CFW_GetAoUserData(hAo);
  if ((UINT32)pEvent == 0xffffffff)
  {
    SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
    ev.nParam1                         = 0xff;
    pEvent                             = &ev;
    pMisGetPreflist->nState.nNextState = CFW_SIM_MIS_IDLE;
  }
  else
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }

  switch (pMisGetPreflist->nState.nNextState)
  {
  case CFW_SIM_MIS_IDLE:
#ifdef CFW_MULTI_SIM
    result = SimElemFileStatusReq(API_SIM_EF_PLMNSEL, nSimID);

    if (ERR_SUCCESS != result)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n",result);      
      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
      hAo = HNULL;
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
    }
#else
    result = SimElemFileStatusReq(API_SIM_EF_PLMNSEL);
    if (ERR_SUCCESS != result)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n",result);      
      
      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
      hAo = HNULL;
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP, (UINT32)result, 0, nUTI, 0xF0);
    }
#endif
    pMisGetPreflist->nState.nCurrState = CFW_SIM_MIS_IDLE;
    pMisGetPreflist->nState.nNextState = CFW_SIM_MIS_READELEMENT;
    break;

  case CFW_SIM_MIS_READBINARY:
    pSimReadBinaryCnf = (api_SimReadBinaryCnf_t*)nEvParam;
    CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
    if (((0x90 == pSimReadBinaryCnf->Sw1) && (0x00 == pSimReadBinaryCnf->Sw2)) || (0x91 == pSimReadBinaryCnf->Sw1))
    {
      // ------------------------------------------------------------
      // Add by lixp for Plmn raise 2007-06-25 begin
      // ------------------------------------------------------------

      pMisGetPreflist->nSize = pMisGetPreflist->nSize / 3 * 3;

      // ------------------------------------------------------------
      // Add by lixp for Plmn raise 2007-06-25 endSS
      // ------------------------------------------------------------

      UINT8* pPLMN = (UINT8*)CSW_SIM_MALLOC(2 * (pMisGetPreflist->nSize));


      if (pPLMN == NULL)
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");        
        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
        hAo = HNULL;

        CFW_PostNotifyEvent(EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
      }
      cfw_PLMNtoBCD(pSimReadBinaryCnf->Data, pPLMN, &(pMisGetPreflist->nSize));
      CSW_TRACE(CFW_SIM_TS_ID, "pMisGetPreflist->nSize 0x%x \n", pMisGetPreflist->nSize);

      CSW_TC_MEMBLOCK(CFW_SIM_TS_ID,pSimReadBinaryCnf->Data,pMisGetPreflist->nSize,16);
      // CSW_TC_MEMBLOCK(CFW_SIM_TS_ID, pPLMN, 6*pMisGetPreflist->nSize, 16);
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP, (UINT32)pPLMN, pMisGetPreflist->nSize,
                          nUTI | (nSimID << 24), 0);
      CSW_TRACE(CFW_SIM_TS_ID,
                    TSTXT("EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP Success data nSize=%d\n"),
                    pMisGetPreflist->nSize);
    }
    else
    {
      ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2, nSimID);
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xF0);
      CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP ErrorCode 0x%x\n"),ErrorCode);
    }

#else
    if (((0x90 == pSimReadBinaryCnf->Sw1) && (0x00 == pSimReadBinaryCnf->Sw2)) || (0x91 == pSimReadBinaryCnf->Sw1))
    {
      // ------------------------------------------------------------
      // Add by lixp for Plmn raise 2007-06-25 begin
      // ------------------------------------------------------------

      pMisGetPreflist->nSize = pMisGetPreflist->nSize / 3 * 3;

      // ------------------------------------------------------------
      // Add by lixp for Plmn raise 2007-06-25 endSS
      // ------------------------------------------------------------

      UINT8* pPLMN = (UINT8*)CSW_SIM_MALLOC(2 * (pMisGetPreflist->nSize));


      if (pPLMN == NULL)
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");        
        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
        hAo = HNULL;

        CFW_PostNotifyEvent(EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
      }
      cfw_PLMNtoBCD(pSimReadBinaryCnf->Data, pPLMN, &(pMisGetPreflist->nSize));
      CSW_TC_MEMBLOCK(CFW_SIM_TS_ID, pPLMN, 6 * pMisGetPreflist->nSize, 16);
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP, (UINT32)pPLMN, pMisGetPreflist->nSize, nUTI, 0);
      CSW_TRACE(CFW_SIM_TS_ID,
                    TSTXT("EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP Success data nSize %d\n"),
                    pMisGetPreflist->nSize);
    }
    else
    {
      ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2);
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP, (UINT32)ErrorCode, 0, nUTI, 0xF0);
      CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP ErrorCode 0x%x\n"),ErrorCode);
    }
#endif
    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
    hAo = HNULL;
    break;
  case CFW_SIM_MIS_READELEMENT:
    pSimElemFileStatusCnf = (api_SimElemFileStatusCnf_t*)nEvParam;
    CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
    if (((0x90 == pSimElemFileStatusCnf->Sw1) && (0x00 == pSimElemFileStatusCnf->Sw2))
        || (0x91 == pSimElemFileStatusCnf->Sw1))
    {
      pMisGetPreflist->nSize = ((pSimElemFileStatusCnf->Data[2] << 8) & 0xFF00) + pSimElemFileStatusCnf->Data[3];

      result = SimReadBinaryReq(API_SIM_EF_PLMNSEL, 0, pMisGetPreflist->nSize, nSimID);
      if (ERR_SUCCESS != result)
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n",result);
        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
        hAo = HNULL;
        CFW_PostNotifyEvent(EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
      }
    }
    else
    {
      ErrorCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2, nSimID);
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xF0);
      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
      hAo = HNULL;
    }

#else
    if (((0x90 == pSimElemFileStatusCnf->Sw1) && (0x00 == pSimElemFileStatusCnf->Sw2))
        || (0x91 == pSimElemFileStatusCnf->Sw1))
    {
      pMisGetPreflist->nSize = ((pSimElemFileStatusCnf->Data[2] << 8) & 0xFF00) + pSimElemFileStatusCnf->Data[3];

      result = SimReadBinaryReq(API_SIM_EF_PLMNSEL, 0, pMisGetPreflist->nSize);
      if (ERR_SUCCESS != result)
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n",result);
        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
        hAo = HNULL;
        CFW_PostNotifyEvent(EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP, (UINT32)result, 0, nUTI, 0xF0);
      }
    }
    else
    {
      ErrorCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2);
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP, (UINT32)ErrorCode, 0, nUTI, 0xF0);
      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
      hAo = HNULL;
    }

#endif
    pMisGetPreflist->nState.nCurrState = CFW_SIM_MIS_READELEMENT;
    pMisGetPreflist->nState.nNextState = CFW_SIM_MIS_READBINARY;
    break;
  default:
    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
    hAo = HNULL;
    break;
  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimMisGetPrefListProc);
  return ERR_SUCCESS;
}

// ------------------------------------------------------------
// Add by wulc  2009-10-25 begin
// ------------------------------------------------------------

#define CFW_SIM_SPN_LEN  16
UINT32 CFW_SimGetServiceProviderNameProc(HAO hAo, CFW_EV* pEvent)
{
  UINT32 nUTI      = 0x00000000;
  UINT32 nEvtId    = 0x00000000;
  UINT32 ErrorCode = 0x00000000;
  UINT32 result    = 0x00000000;
  VOID* nEvParam   = NULL;
  CFW_EV ev;

  SIM_SM_INFO_GETSERVERPROVIDERNAME* pMisGetSPN = NULL;
  api_SimReadBinaryCnf_t* pSimReadBinaryCnf = NULL;
  api_SimElemFileStatusCnf_t* pSimElemFileStatusCnf = NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetServiceProviderNameProc);
#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hAo);
#endif
  pMisGetSPN = CFW_GetAoUserData(hAo);
  if ((UINT32)pEvent == 0xffffffff)
  {
    SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
    ev.nParam1                         = 0xff;
    pEvent                             = &ev;
    pMisGetSPN->nState.nNextState = CFW_SIM_MIS_IDLE;
  }
  else
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }

  switch (pMisGetSPN->nState.nNextState)
  {
  case CFW_SIM_MIS_IDLE:
#ifdef CFW_MULTI_SIM
    result = SimElemFileStatusReq(API_SIM_EF_SPN, nSimID);

    if (ERR_SUCCESS != result)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n",result);      
      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
      hAo = HNULL;
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_SPN_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
      return ERR_SUCCESS;

    }
#else
    result = SimElemFileStatusReq(API_SIM_EF_SPN);
    if (ERR_SUCCESS != result)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n",result);      
      
      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
      hAo = HNULL;
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_SPN_RSP, (UINT32)result, 0, nUTI, 0xF0);
      return ERR_SUCCESS;
    }
#endif
    pMisGetSPN->nState.nCurrState = CFW_SIM_MIS_IDLE;
    pMisGetSPN->nState.nNextState = CFW_SIM_MIS_READELEMENT;
    break;

  case CFW_SIM_MIS_READBINARY:
    pSimReadBinaryCnf = (api_SimReadBinaryCnf_t*)nEvParam;
    CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
    if (((0x90 == pSimReadBinaryCnf->Sw1) && (0x00 == pSimReadBinaryCnf->Sw2)) || (0x91 == pSimReadBinaryCnf->Sw1))
    {     
    	CFW_SPN *SPN = (CFW_SPN*)CSW_SIM_MALLOC(SIZEOF(CFW_SPN));
    	SPN->flag = pSimReadBinaryCnf->Data[0];
    	SUL_MemCopy8(SPN->spn, pSimReadBinaryCnf->Data+1, CFW_SIM_SPN_LEN);

      CFW_PostNotifyEvent(EV_CFW_SIM_GET_SPN_RSP, (UINT32)SPN, pMisGetSPN->nSize, nUTI | (nSimID << 24), 0);
      CSW_TRACE(CFW_SIM_TS_ID, TSTXT("pSimReadBinaryCnf->Data Success data nSize=%d\n"),pSimReadBinaryCnf->Data);
    }
    else
    {
      ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2, nSimID);
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_SPN_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xF0);
      CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error EV_CFW_SIM_GET_SPN_RSP ErrorCode 0x%x\n"),ErrorCode);
    }

#else
    if (((0x90 == pSimReadBinaryCnf->Sw1) && (0x00 == pSimReadBinaryCnf->Sw2)) || (0x91 == pSimReadBinaryCnf->Sw1))
    {     
    	CFW_SPN *SPN = (CFW_SPN*)CSW_SIM_MALLOC(SIZEOF(CFW_SPN));
    	SPN->flag = pSimReadBinaryCnf->Data[0];
    	SUL_MemCopy8(SPN->spn, pSimReadBinaryCnf->Data+1, CFW_SIM_SPN_LEN);

      CFW_PostNotifyEvent(EV_CFW_SIM_GET_SPN_RSP,(UINT32)SPN, pMisGetSPN->nSize, nUTI, 0);
      CSW_TRACE(CFW_SIM_TS_ID, TSTXT("EV_CFW_SIM_GET_SPN_RSP Success data nSize %d\n"), pMisGetSPN->nSize);
      CSW_TRACE(CFW_SIM_TS_ID, TSTXT("pSimReadBinaryCnf->Data Success data nSize=%d\n"),pSimReadBinaryCnf->Data);
    }
    else
    {
      ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2);
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_SPN_RSP, (UINT32)ErrorCode, 0, nUTI, 0xF0);
      CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error EV_CFW_SIM_GET_SPN_RSP ErrorCode 0x%x\n"),ErrorCode);
    }

#endif


    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
    hAo = HNULL;
    break;
  case CFW_SIM_MIS_READELEMENT:
    pSimElemFileStatusCnf = (api_SimElemFileStatusCnf_t*)nEvParam;
    CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
    if (((0x90 == pSimElemFileStatusCnf->Sw1) && (0x00 == pSimElemFileStatusCnf->Sw2))
        || (0x91 == pSimElemFileStatusCnf->Sw1))
    {
      pMisGetSPN->nSize = ((pSimElemFileStatusCnf->Data[2] << 8) & 0xFF00) + pSimElemFileStatusCnf->Data[3];

      result = SimReadBinaryReq(API_SIM_EF_SPN, 0, pMisGetSPN->nSize, nSimID);
      if (ERR_SUCCESS != result)
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n",result);
        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
        hAo = HNULL;
        CFW_PostNotifyEvent(EV_CFW_SIM_GET_SPN_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
        return ERR_SUCCESS;

      }
    }
    else
    {
      ErrorCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2, nSimID);
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_SPN_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xF0);
      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
      hAo = HNULL;
      return ERR_SUCCESS;

    }
#else
    if (((0x90 == pSimElemFileStatusCnf->Sw1) && (0x00 == pSimElemFileStatusCnf->Sw2))
        || (0x91 == pSimElemFileStatusCnf->Sw1))
    {
      pMisGetSPN->nSize = ((pSimElemFileStatusCnf->Data[2] << 8) & 0xFF00) + pSimElemFileStatusCnf->Data[3];

      result = SimReadBinaryReq(API_SIM_EF_SPN, 0, pMisGetSPN->nSize);
      if (ERR_SUCCESS != result)
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n",result);
        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
        hAo = HNULL;
        CFW_PostNotifyEvent(EV_CFW_SIM_GET_SPN_RSP, (UINT32)result, 0, nUTI, 0xF0);
        return ERR_SUCCESS;

      }
    }
    else
    {
      ErrorCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2);
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_SPN_RSP, (UINT32)ErrorCode, 0, nUTI, 0xF0);
      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
      hAo = HNULL;
      return ERR_SUCCESS;

    }
#endif
    pMisGetSPN->nState.nCurrState = CFW_SIM_MIS_READELEMENT;
    pMisGetSPN->nState.nNextState = CFW_SIM_MIS_READBINARY;
    break;
  default:
    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
    hAo = HNULL;
    break;
  }
}

// ------------------------------------------------------------
// Add by wulc  2009-10-25 end
// ------------------------------------------------------------

UINT32 CFW_SimMisGetPrefListMAXNumProc(HAO hAo, CFW_EV* pEvent)
{
  UINT32 nUTI      = 0x00000000;
  UINT32 nEvtId    = 0x00000000;
  UINT32 ErrorCode = 0x00000000;
  UINT32 result    = 0x00000000;
  VOID* nEvParam   = NULL;
  CFW_EV ev;

  SIM_SM_INFO_GETPREFOPREATORLISTMAXNUM* pMisGetPreflistMax = NULL;
  api_SimElemFileStatusCnf_t* pSimElemFileStatusCnf = NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimMisGetPrefListMAXNumProc);
#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hAo);
#endif
  pMisGetPreflistMax = CFW_GetAoUserData(hAo);
  if ((UINT32)pEvent == 0xffffffff)
  {
    SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
    ev.nParam1                            = 0xff;
    pEvent                                = &ev;
    pMisGetPreflistMax->nState.nNextState = CFW_SIM_MIS_IDLE;
  }
  else
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }

  switch (pMisGetPreflistMax->nState.nNextState)
  {
  case CFW_SIM_MIS_IDLE:
#ifdef CFW_MULTI_SIM
    result = SimElemFileStatusReq(API_SIM_EF_PLMNSEL, nSimID);
    if (ERR_SUCCESS != result)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n",result);      
      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
      hAo = HNULL;
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_PREF_OPT_LIST_MAXNUM_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
    }
#else
    result = SimElemFileStatusReq(API_SIM_EF_PLMNSEL);
    if (ERR_SUCCESS != result)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n",result);      
      
      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
      hAo = HNULL;
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_PREF_OPT_LIST_MAXNUM_RSP, (UINT32)result, 0, nUTI, 0xF0);
    }
#endif
    pMisGetPreflistMax->nState.nCurrState = CFW_SIM_MIS_IDLE;
    pMisGetPreflistMax->nState.nNextState = CFW_SIM_MIS_READELEMENT;
    break;

  case CFW_SIM_MIS_READELEMENT:
    pSimElemFileStatusCnf = (api_SimElemFileStatusCnf_t*)nEvParam;
    CFW_GetUTI(hAo, &nUTI);
    if (((0x90 == pSimElemFileStatusCnf->Sw1) && (0x00 == pSimElemFileStatusCnf->Sw2))
        || (0x91 == pSimElemFileStatusCnf->Sw1))
    {
      UINT16 Totalsize = ((pSimElemFileStatusCnf->Data[2] << 8) & 0xFF00) + pSimElemFileStatusCnf->Data[3];
      UINT32 nNumber   = Totalsize / 0x03;

#ifdef CFW_MULTI_SIM
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_PREF_OPT_LIST_MAXNUM_RSP, (UINT32)nNumber, 0, nUTI | (nSimID << 24), 0);
      CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_GET_PREF_OPT_LIST_MAXNUM_RSP Success\n"));
    }
    else
    {
      ErrorCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2, nSimID);
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_PREF_OPT_LIST_MAXNUM_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xF0);
#else
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_PREF_OPT_LIST_MAXNUM_RSP, (UINT32)nNumber, 0, nUTI, 0);
      CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_GET_PREF_OPT_LIST_MAXNUM_RSP Success! \n"));
    }
    else
    {
      ErrorCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2);
      CFW_PostNotifyEvent(EV_CFW_SIM_GET_PREF_OPT_LIST_MAXNUM_RSP, (UINT32)ErrorCode, 0, nUTI, 0xF0);
#endif
    }
    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
    hAo = HNULL;
    break;
  default:
    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
    hAo = HNULL;
    break;
  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimMisGetPrefListMAXNumProc);
  return ERR_SUCCESS;
}

UINT32 CFW_SimMisGetACMProc(HAO hAo, CFW_EV* pEvent)
{
  UINT32 nUTI      = 0x00000000;
  UINT32 nEvtId    = 0x00000000;
  UINT32 ErrorCode = 0x00000000;
  UINT32 result    = 0x00000000;
  UINT32 nValue    = 0x00000000;
  VOID* nEvParam   = NULL;
  CFW_EV ev;

  SIM_SM_INFO_GETACM* pMisGetACM = NULL;
  api_SimReadRecordCnf_t* pSimReadRecordCnf = NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimMisGetACMProc);
#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hAo);
#endif
  pMisGetACM = CFW_GetAoUserData(hAo);

  if ((UINT32)pEvent == 0xffffffff)
  {
    SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
    ev.nParam1                    = 0xff;
    pEvent                        = &ev;
    pMisGetACM->nState.nNextState = CFW_SIM_MIS_IDLE;
  }
  else
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }

  switch (pMisGetACM->nState.nNextState)
  {
  case CFW_SIM_MIS_IDLE:
    {
      if ((UINT32)pEvent == 0xFFFFFFFF)
      {
#ifdef CFW_MULTI_SIM
        result = SimReadRecordReq(API_SIM_EF_ACM, 0x00, 0x04, 0x03, nSimID);
        if (ERR_SUCCESS != result)
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n",result);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
          hAo = HNULL;
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_ACM_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
        }
#else
        result = SimReadRecordReq(API_SIM_EF_ACM, 0x00, 0x04, 0x03);
        if (ERR_SUCCESS != result)
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadRecordReq return 0x%x \n",result);
          
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
          hAo = HNULL;
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_ACM_RSP, (UINT32)result, 0, nUTI, 0xF0);
        }
#endif
        pMisGetACM->nState.nCurrState = CFW_SIM_MIS_IDLE;
        pMisGetACM->nState.nNextState = CFW_SIM_MIS_READRECORD;
      }
      else
      {
        CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
      }
    }
    break;

  case CFW_SIM_MIS_READRECORD:
    {
      if ((UINT32)pEvent == API_SIM_READRECORD_CNF)
      {
        pSimReadRecordCnf = (api_SimReadRecordCnf_t*)nEvParam;
        CFW_GetUTI(hAo, &nUTI);

#ifdef CFW_MULTI_SIM
        if (((0x90 == pSimReadRecordCnf->Sw1) && (0x00 == pSimReadRecordCnf->Sw2)) || (0x91 == pSimReadRecordCnf->Sw1))
        {
          UINT32* pACMData = CSW_SIM_MALLOC(4);

          if (pACMData == NULL)
          {
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
            hAo = HNULL;

            CFW_PostNotifyEvent(EV_CFW_SIM_GET_ACM_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
          }
          SUL_MemSet8(pACMData, 0x00, 4);
          SUL_MemCopy8(pACMData, pSimReadRecordCnf->Data, 3);
          nValue = *pACMData;
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_ACM_RSP, (UINT32)(nValue >> 0x08), 0, nUTI | (nSimID << 24), 0);
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_GET_ACM_RSP Success! \n"));
          CSW_SIM_FREE(pACMData);
        }
        else
        {
          ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2, nSimID);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_ACM_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xF0);
        }
#else
        if (((0x90 == pSimReadRecordCnf->Sw1) && (0x00 == pSimReadRecordCnf->Sw2)) || (0x91 == pSimReadRecordCnf->Sw1))
        {
          UINT32* pACMData = CSW_SIM_MALLOC(4);

          if (pACMData == NULL)
          {
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
            hAo = HNULL;

            CFW_PostNotifyEvent(EV_CFW_SIM_GET_ACM_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
          }
          SUL_MemSet8(pACMData, 0x00, 4);
          SUL_MemCopy8(pACMData, pSimReadRecordCnf->Data, 3);
          nValue = *pACMData;
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_ACM_RSP, (UINT32)(nValue >> 0x08), 0, nUTI, 0);
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_GET_ACM_RSP Success! \n"));
          CSW_SIM_FREE(pACMData);
        }
        else
        {
          ErrorCode = Sim_ParseSW1SW2(pSimReadRecordCnf->Sw1, pSimReadRecordCnf->Sw2);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_ACM_RSP, (UINT32)ErrorCode, 0, nUTI, 0xF0);
        }
#endif
        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
        hAo = HNULL;
      }
      else
      {
        CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
      }
    }
    break;

  default:
    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
    hAo = HNULL;
    break;
  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimMisGetACMProc);

  return ERR_SUCCESS;
}

UINT32 CFW_SimMisSetACMProc(HAO hAo, CFW_EV* pEvent)
{
  UINT32 nUTI      = 0x00000000;
  UINT32 nEvtId    = 0x00000000;
  UINT32 ErrorCode = 0x00000000;
  UINT32 result    = 0x00000000;
  VOID* nEvParam   = NULL;
  UINT8 nValue[3];
  CFW_EV ev;

  SIM_SM_INFO_SETACM* pMisSetACM = NULL;
  api_SimElemFileStatusCnf_t* pSimElemFileStatusCnf = NULL;
  api_SimVerifyCHVCnf_t* pSimVerifyCHVCnf = NULL;
  api_SimUpdateRecordCnf_t* pSimUpdateRecordCnf = NULL;
  SIM_CHV_PARAM* pSimCHV = NULL;

// CSW_PROFILE_FUNCTION_ENTER(CFW_SimMisSetACMProc);
#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hAo);
  CFW_CfgSimGetChvParam(&pSimCHV, nSimID);
#else
  CFW_CfgSimGetChvParam(&pSimCHV);
#endif

  pMisSetACM = CFW_GetAoUserData(hAo);

  if ((UINT32)pEvent == 0xffffffff)
  {
    SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
    ev.nParam1                    = 0xff;
    pEvent                        = &ev;
    pMisSetACM->nState.nNextState = CFW_SIM_MIS_IDLE;
  }
  else
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }
  SUL_MemCopy8(nValue, &pMisSetACM->iACMValue, 0x03);

  switch (pMisSetACM->nState.nNextState)
  {
  case CFW_SIM_MIS_IDLE:
    {
      if ((UINT32)pEvent == 0xffffffff)
      {
#ifdef CFW_MULTI_SIM
        result = SimElemFileStatusReq(API_SIM_EF_ACM, nSimID);
        if (ERR_SUCCESS != result)
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n",result);
          
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
          hAo = HNULL;
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACM_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
        }
#else
        result = SimElemFileStatusReq(API_SIM_EF_ACM);
        if (ERR_SUCCESS != result)
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n",result);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
          hAo = HNULL;
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACM_RSP, (UINT32)result, 0, nUTI, 0xF0);
        }
#endif
        pMisSetACM->nState.nCurrState = CFW_SIM_MIS_IDLE;
        pMisSetACM->nState.nNextState = CFW_SIM_MIS_READELEMENT;
      }
      else
      {
        CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
      }
    }
    break;

  case CFW_SIM_MIS_READELEMENT:
    {
      if ((UINT32)pEvent == API_SIM_ELEMFILESTATUS_CNF)
      {
        pSimElemFileStatusCnf = (api_SimElemFileStatusCnf_t*)nEvParam;
        if (((0x90 == pSimElemFileStatusCnf->Sw1) && (0x00 == pSimElemFileStatusCnf->Sw2))
            || (0x91 == pSimElemFileStatusCnf->Sw1))
        {
          if (((pSimElemFileStatusCnf->Data[8]) & 0x0F) == 1)
          {
            pMisSetACM->nMark = 1;
#ifdef CFW_MULTI_SIM
            result = SimUpdateRecordReq(API_SIM_EF_ACM, 0x00, 0x03, 0x03, nValue, nSimID);
            if (ERR_SUCCESS != result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n",result);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
              hAo = HNULL;
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACM_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
            }
#else
            result = SimUpdateRecordReq(API_SIM_EF_ACM, 0x00, 0x03, 0x03, nValue);
            if (ERR_SUCCESS != result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n",result);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
              hAo = HNULL;
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACM_RSP, (UINT32)result, 0, nUTI, 0xF0);
            }
#endif
            pMisSetACM->nState.nCurrState = CFW_SIM_MIS_READELEMENT;
            pMisSetACM->nState.nNextState = CFW_SIM_MIS_UPDATERECORD;

          }
          else if (((pSimElemFileStatusCnf->Data[8]) & 0x0F) == 2)
          {
            pMisSetACM->nMark = 2;

            // 
            // CHV2 cannot be disable!
            // 
#if 0
            if (pSimCHV->nPin2Status == 2)
            {

              result = SimUpdateRecordReq(API_SIM_EF_ACM, 0x00, 0x03, 0x03, nValue);
              if (ERR_SUCCESS != result)
              {
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
                hAo = HNULL;
                CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACM_RSP, (UINT32)result, 0, nUTI, 0xF0);
              }
              pMisSetACM->nState.nCurrState = CFW_SIM_MIS_READELEMENT;
              pMisSetACM->nState.nNextState = CFW_SIM_MIS_UPDATERECORD;
            }
#endif
#ifdef CFW_MULTI_SIM
            result = SimVerifyCHVReq(pMisSetACM->nPIN2, 2, nSimID);
            if (ERR_SUCCESS != result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimVerifyCHVReq return 0x%x \n",result);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
              hAo = HNULL;
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACM_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
            }
#else
            result = SimVerifyCHVReq(pMisSetACM->nPIN2, 2);
            if (ERR_SUCCESS != result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimVerifyCHVReq return 0x%x \n",result);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
              hAo = HNULL;
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACM_RSP, (UINT32)result, 0, nUTI, 0xF0);
            }
#endif
            pMisSetACM->nState.nCurrState = CFW_SIM_MIS_READELEMENT;
            pMisSetACM->nState.nNextState = CFW_SIM_MIS_VERIFY;
          }
          else
          {
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
            hAo = HNULL;
          }
        }
        else
        {
          CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
          ErrorCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2, nSimID);
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACM_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xF0);
#else
          ErrorCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2);
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACM_RSP, (UINT32)ErrorCode, 0, nUTI, 0xF0);
#endif
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error EV_CFW_SIM_SET_ACM_RSP  ErrorCode 0x%x\n"),ErrorCode);
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

  case CFW_SIM_MIS_VERIFY:
    {
      if ((UINT32)pEvent == API_SIM_VERIFYCHV_CNF)
      {
        pSimVerifyCHVCnf = (api_SimVerifyCHVCnf_t*)nEvParam;
        CFW_GetUTI(hAo, &nUTI);
        if ((0x90 == pSimVerifyCHVCnf->Sw1) && (0x00 == pSimVerifyCHVCnf->Sw2))
        {
#ifdef CFW_MULTI_SIM
          result = SimUpdateRecordReq(API_SIM_EF_ACM, 0x00, 0x03, 0x03, nValue, nSimID);
          if (ERR_SUCCESS != result)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n",result);            
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
            hAo = HNULL;
            CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACM_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
          }
#else
          result = SimUpdateRecordReq(API_SIM_EF_ACM, 0x00, 0x03, 0x03, nValue);
          if (ERR_SUCCESS != result)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateRecordReq return 0x%x \n",result);            
            
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
            hAo = HNULL;
            CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACM_RSP, (UINT32)result, 0, nUTI, 0xF0);
          }
#endif
          pMisSetACM->nState.nCurrState = CFW_SIM_MIS_VERIFY;
          pMisSetACM->nState.nNextState = CFW_SIM_MIS_UPDATERECORD;
        }
        else
        {
#ifdef CFW_MULTI_SIM
          ErrorCode = Sim_ParseSW1SW2(pSimVerifyCHVCnf->Sw1, pSimVerifyCHVCnf->Sw2, nSimID);
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACM_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID), 0xF0);
#else
          ErrorCode = Sim_ParseSW1SW2(pSimVerifyCHVCnf->Sw1, pSimVerifyCHVCnf->Sw2);
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACM_RSP, (UINT32)ErrorCode, 0, nUTI, 0xF0);
#endif
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("EV_CFW_SIM_SET_ACM_RSP ErrorCode 0x%x \n"),ErrorCode);
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

  case CFW_SIM_MIS_UPDATERECORD:
    {
      if ((UINT32)pEvent == API_SIM_UPDATERECORD_CNF)
      {
        pSimUpdateRecordCnf = (api_SimUpdateRecordCnf_t*)nEvParam;
        CFW_GetUTI(hAo, &nUTI);
        if (((0x90 == pSimUpdateRecordCnf->Sw1) && (0x00 == pSimUpdateRecordCnf->Sw2))
            || (0x91 == pSimUpdateRecordCnf->Sw1))
        {
#ifdef CFW_MULTI_SIM
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACM_RSP, (UINT32)pMisSetACM->nMark, 0, nUTI | (nSimID << 24), 0);
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_SET_ACM_RSP Success! \n"));
        }
        else
        {

          ErrorCode = Sim_ParseSW1SW2(pSimUpdateRecordCnf->Sw1, pSimUpdateRecordCnf->Sw2, nSimID);
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACM_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xF0);
#else
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACM_RSP, (UINT32)pMisSetACM->nMark, 0, nUTI, 0);
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_SET_ACM_RSP Success! \n"));
        }
        else
        {

          ErrorCode = Sim_ParseSW1SW2(pSimUpdateRecordCnf->Sw1, pSimUpdateRecordCnf->Sw2);
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACM_RSP, (UINT32)ErrorCode, 0, nUTI, 0xF0);
#endif
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("EV_CFW_SIM_SET_ACM_RSP ErrorCode 0x%x \n"),ErrorCode);
        }

        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
        hAo = HNULL;
      }
      else
      {
        CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
      }
    }
    break;

  default:
    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
    hAo = HNULL;
    break;
  }

// CSW_PROFILE_FUNCTION_EXIT(CFW_SimMisSetACMProc);

  return ERR_SUCCESS;
}

UINT32 CFW_SimSetACMMaxProc(HAO hAo, CFW_EV* pEvent)
{
  UINT32 nEvtId, nUTI, nErrCode;
  VOID* nEvParam;

  ACM_INFO_SETACMMAX* pSetACMMax;

  nEvtId     = 0x00000000;
  nUTI       = 0x00000000;
  nErrCode   = 0x00000000;
  nEvParam   = (VOID*)NULL;
  pSetACMMax = (ACM_INFO_SETACMMAX*)NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimSetACMMaxProc);
  if ((UINT32)pEvent != 0xFFFFFFFF)
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }

#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hAo);
#endif
  pSetACMMax = CFW_GetAoUserData(hAo);

  switch (pSetACMMax->n_CurrStatus)
  {
  case CFW_SIM_MIS_IDLE:
    {
      if ((UINT32)pEvent == 0xFFFFFFFF)
      {
        if (pSetACMMax->nTryCount >= 0x01)
        {
#ifdef CFW_MULTI_SIM
          nErrCode = SimElemFileStatusReq(API_SIM_EF_ACMMAX, nSimID);
          if (ERR_SUCCESS != nErrCode)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n",nErrCode);
            CFW_GetUTI(hAo, &nUTI);
            CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACMMAX_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0XF0);
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n",nErrCode);

            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
            hAo = HNULL;
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACMMaxProc);
            return nErrCode;
          }

#else
          nErrCode = SimElemFileStatusReq(API_SIM_EF_ACMMAX);
          if (ERR_SUCCESS != nErrCode)
          {
            CFW_GetUTI(hAo, &nUTI);
            CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACMMAX_RSP, (UINT32)nErrCode, 0, nUTI, 0XF0);
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n",nErrCode);

            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
            hAo = HNULL;
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACMMaxProc);
            return nErrCode;
          }

#endif
        }
        else
        {
          // Time out and TODO...
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
          hAo = HNULL;
        }
        CFW_SetAoState(hAo, CFW_SM_SIM_STATE_WRITE);

        pSetACMMax->n_PrevStatus = pSetACMMax->n_CurrStatus;
        pSetACMMax->n_CurrStatus = CFW_SIM_MIS_READELEMENT;
      }
      else
      {
        CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
      }
    }
    break;

  case CFW_SIM_MIS_READELEMENT:
    {
      if (nEvtId == API_SIM_ELEMFILESTATUS_CNF)
      {
        api_SimElemFileStatusCnf_t* pSimElemFileStatusCnf = (api_SimElemFileStatusCnf_t*)nEvParam;
        if (((0x90 == pSimElemFileStatusCnf->Sw1) && (0x00 == pSimElemFileStatusCnf->Sw2))
            || (0x91 == pSimElemFileStatusCnf->Sw1))
        {
          if (0x02 == (UINT8)(pSimElemFileStatusCnf->Data[8] & 0x0F))
          {
#ifdef CFW_MULTI_SIM
            UINT32 Result = SimVerifyCHVReq(pSetACMMax->pPin2, CHV2, nSimID);

            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimVerifyCHVReq return 0x%x \n",Result);
              CFW_GetUTI(hAo, &nUTI);
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_PUCT_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPUCTProc);

              return ERR_SUCCESS;
            }


#else
            UINT32 Result = SimVerifyCHVReq(pSetACMMax->pPin2, CHV2);

            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimVerifyCHVReq return 0x%x \n",Result);
              
              CFW_GetUTI(hAo, &nUTI);
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_PUCT_RSP, Result, 0, nUTI, 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPUCTProc);

              return ERR_SUCCESS;
            }

#endif
            CFW_SetAoState(hAo, CFW_SM_SIM_STATE_WRITE);

            pSetACMMax->n_PrevStatus = pSetACMMax->n_CurrStatus;
            pSetACMMax->n_CurrStatus = CFW_SIM_MIS_VERIFY;
          }
          else
          {
            UINT8 nACMMax[3];

            nACMMax[0] = (UINT8)((pSetACMMax->iACMMaxValue) >> 16);
            nACMMax[1] = (UINT8)((pSetACMMax->iACMMaxValue) >> 8);
            nACMMax[2] = pSetACMMax->iACMMaxValue;
#ifdef CFW_MULTI_SIM
            nErrCode = SimUpdateBinaryReq(API_SIM_EF_ACMMAX, 0, 3, nACMMax, nSimID);
            if (ERR_SUCCESS != nErrCode)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimUpdateBinaryReq return 0x%x \n",nErrCode);
              
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACMMAX_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0XF0);
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimUpdateBinaryReq ErrCode 0x%x\n"), nErrCode);

              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
              hAo = HNULL;
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACMMaxProc);
              return nErrCode;
            }

#else
            nErrCode = SimUpdateBinaryReq(API_SIM_EF_ACMMAX, 0, 3, nACMMax);
            if (ERR_SUCCESS != nErrCode)
            {
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACMMAX_RSP, (UINT32)nErrCode, 0, nUTI, 0XF0);
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimUpdateBinaryReq ErrCode 0x%x\n"), nErrCode);

              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
              hAo = HNULL;
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACMMaxProc);
              return nErrCode;
            }

#endif

            CFW_SetAoState(hAo, CFW_SM_SIM_STATE_WRITE);

            pSetACMMax->n_PrevStatus = pSetACMMax->n_CurrStatus;
            pSetACMMax->n_CurrStatus = CFW_SIM_MIS_UPDATEBINARY;
          }
          CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
        }
        else
        {
#ifdef CFW_MULTI_SIM
          nErrCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2, nSimID);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_PUCT_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0);
#else
          nErrCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_PUCT_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0);
#endif
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
        }
      }
      else
      {
        CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
      }
    }
    break;
  case CFW_SIM_MIS_VERIFY:
    {
      if (nEvtId == API_SIM_VERIFYCHV_CNF)
      {
        api_SimVerifyCHVCnf_t* pSimVerifyCHVCnf = (api_SimVerifyCHVCnf_t*)nEvParam;
        CFW_GetUTI(hAo, &nUTI);

        if ((0x90 == pSimVerifyCHVCnf->Sw1) && (0x00 == pSimVerifyCHVCnf->Sw2))
        {
          UINT8 nACMMax[3];

          nACMMax[0] = (pSetACMMax->iACMMaxValue) >> 16;
          nACMMax[1] = (pSetACMMax->iACMMaxValue) >> 8;
          nACMMax[2] = pSetACMMax->iACMMaxValue;
#ifdef CFW_MULTI_SIM
          nErrCode = SimUpdateBinaryReq(API_SIM_EF_ACMMAX, 0, 3, nACMMax, nSimID);
          if (ERR_SUCCESS != nErrCode)
          {
            CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimUpdateBinaryReq ErrCode 0x%x\n"), nErrCode);
            
            CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACMMAX_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0XF0);
            CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimUpdateBinaryReq ErrCode 0x%x\n"), nErrCode);

            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
            hAo = HNULL;
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACMMaxProc);
            return nErrCode;
          }

#else
          nErrCode = SimUpdateBinaryReq(API_SIM_EF_ACMMAX, 0, 3, nACMMax);
          if (ERR_SUCCESS != nErrCode)
          {
            CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACMMAX_RSP, (UINT32)nErrCode, 0, nUTI, 0XF0);
            CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimUpdateBinaryReq ErrCode 0x%x\n"), nErrCode);

            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
            hAo = HNULL;
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACMMaxProc);
            return nErrCode;
          }

#endif
          CFW_SetAoState(hAo, CFW_SM_SIM_STATE_WRITE);

          pSetACMMax->n_PrevStatus = pSetACMMax->n_CurrStatus;
          pSetACMMax->n_CurrStatus = CFW_SIM_MIS_UPDATEBINARY;
        }
        else
        {
#ifdef CFW_MULTI_SIM
          nErrCode = Sim_ParseSW1SW2(pSimVerifyCHVCnf->Sw1, pSimVerifyCHVCnf->Sw2, nSimID);
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACMMAX_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0);
#else
          nErrCode = Sim_ParseSW1SW2(pSimVerifyCHVCnf->Sw1, pSimVerifyCHVCnf->Sw2);
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACMMAX_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0);
#endif
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimUpdateBinaryReq ErrCode 0x%x\n"), nErrCode);

          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
          hAo = HNULL;
        }

      }
      else
        CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case CFW_SIM_MIS_UPDATEBINARY:
    {
      if (pEvent->nEventId == API_SIM_UPDATEBINARY_CNF)
      {
        api_SimUpdateBinaryCnf_t* pSimUpdateBinaryCnf = (api_SimUpdateBinaryCnf_t*)nEvParam;
        CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
        if ((0x90 == pSimUpdateBinaryCnf->Sw1) && (00 == pSimUpdateBinaryCnf->Sw2))
        {
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACMMAX_RSP, (UINT32)0, 0, nUTI | (nSimID << 24), 0);
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_ACM_SET_ACMMAX_RSP Success! \n"));
        }
        else
        {
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACMMAX_RSP, (UINT32)0, 0, nUTI | (nSimID << 24), 0xf0);
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" Error EV_CFW_ACM_SET_ACMMAX_RSP SW1 0x%x SW2 0x%x\n"),pSimUpdateBinaryCnf->Sw1,pSimUpdateBinaryCnf->Sw2);
        }
#else
        if ((0x90 == pSimUpdateBinaryCnf->Sw1) && (00 == pSimUpdateBinaryCnf->Sw2))
        {
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACMMAX_RSP, (UINT32)0, 0, nUTI, 0);
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_ACM_SET_ACMMAX_RSP Success! \n"));
        }
        else
        {
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_ACMMAX_RSP, (UINT32)0, 0, nUTI, 0xf0);
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" Error EV_CFW_ACM_SET_ACMMAX_RSP SW1 0x%x SW2 0x%x\n"),pSimUpdateBinaryCnf->Sw1,pSimUpdateBinaryCnf->Sw2);
        }
#endif
        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
        hAo = HNULL;
      }
      else
      {
        CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
      }
    }
    break;
  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACMMaxProc);
  return ERR_SUCCESS;
}

UINT32 CFW_SimGetACMMaxProc(HAO hAo, CFW_EV* pEvent)
{
  UINT32 nEvtId, nUTI, ErrorCode;
  VOID* nEvParam;

  ACM_INFO_GETACMMAX* pGetACMMax;

  nEvtId     = 0x00000000;
  nUTI       = 0x00000000;
  ErrorCode  = 0x00000000;
  nEvParam   = (VOID*)NULL;
  pGetACMMax = (ACM_INFO_GETACMMAX*)NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetACMMaxProc);
  if ((UINT32)pEvent != 0xFFFFFFFF)
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }
#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hAo);
#endif
  pGetACMMax = CFW_GetAoUserData(hAo);

  switch (pGetACMMax->n_CurrStatus)
  {
  case CFW_SIM_MIS_IDLE:
    {
      if ((UINT32)pEvent == 0xFFFFFFFF)
      {
        if (pGetACMMax->nTryCount >= 0x01)
        {
#ifdef CFW_MULTI_SIM
          ErrorCode = SimReadBinaryReq(API_SIM_EF_ACMMAX, 0, 3, nSimID);
          if (ERR_SUCCESS != ErrorCode)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n",ErrorCode);            
            CFW_GetUTI(hAo, &nUTI);
            CFW_PostNotifyEvent(EV_CFW_SIM_GET_ACMMAX_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0XF0);
#else
          ErrorCode = SimReadBinaryReq(API_SIM_EF_ACMMAX, 0, 3);
          if (ERR_SUCCESS != ErrorCode)
          {
            CFW_GetUTI(hAo, &nUTI);
            CFW_PostNotifyEvent(EV_CFW_SIM_GET_ACMMAX_RSP, (UINT32)ErrorCode, 0, nUTI, 0XF0);
#endif
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n",ErrorCode);            

            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
            hAo = HNULL;
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetACMMaxProc);
            return ErrorCode;
          }

          CFW_SetAoState(hAo, CFW_SM_SIM_STATE_WRITE);

          pGetACMMax->n_PrevStatus = pGetACMMax->n_CurrStatus;
          pGetACMMax->n_CurrStatus = CFW_SIM_MIS_READBINARY;

          // pGetMR->nTryCount --;
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

  case CFW_SIM_MIS_READBINARY:
    {
      if (pEvent->nEventId == API_SIM_READBINARY_CNF)
      {
        api_SimReadBinaryCnf_t* pSimReadBinaryCnf = (api_SimReadBinaryCnf_t*)nEvParam;
        CFW_GetUTI(hAo, &nUTI);
        if ((0x90 == pSimReadBinaryCnf->Sw1) && (00 == pSimReadBinaryCnf->Sw2))
        {
          UINT32 nACMMaxValue;

          nACMMaxValue = (pSimReadBinaryCnf->Data[0]) * 0x400000 +
            (pSimReadBinaryCnf->Data[1]) * 0x100 + (pSimReadBinaryCnf->Data[2]);
#ifdef CFW_MULTI_SIM
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_ACMMAX_RSP, (UINT32)nACMMaxValue, 0, nUTI | (nSimID << 24), 0);
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_ACM_GET_ACMMAX_RSP Success! \n"));
        }
        else
        {
          ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2, nSimID);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_ACMMAX_RSP, (UINT32)ErrorCode, 0, nUTI | (nSimID << 24), 0xf0);
        }
#else
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_ACMMAX_RSP, (UINT32)nACMMaxValue, 0, nUTI, 0);
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_ACM_GET_ACMMAX_RSP Success! \n"));
        }
        else
        {
          ErrorCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_ACMMAX_RSP, (UINT32)ErrorCode, 0, nUTI, 0xf0);
        }
#endif
            CSW_TRACE(CFW_SIM_TS_ID, "Error Wrong SW return 0x%x \n",ErrorCode);            

        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
        hAo = HNULL;
      }
      else
      {
        CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
      }
    }
    break;
  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetACMMaxProc);
  return ERR_SUCCESS;
}

UINT32 CFW_SimGetPUCTProc(HAO hAo, CFW_EV* pEvent)
{
  HAO hSimGetPUCTAo = hAo;

  CFW_SIMGETPUCT* pSimGetPUCTData = NULL;

  UINT32 nEvtId  = 0;
  VOID* nEvParam = 0;
  UINT32 nUTI    = 0;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetPUCTProc);

  if ((UINT32)pEvent != 0xFFFFFFFF) // 不是第一次进入本函数
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }
#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hSimGetPUCTAo);
#endif
  pSimGetPUCTData = CFW_GetAoUserData(hSimGetPUCTAo);
  switch (pSimGetPUCTData->nSm_GetPUCT_currStatus)
  {
  case CFW_SIM_MIS_IDLE:
    {
      if ((UINT32)pEvent == 0xFFFFFFFF)
      {
        UINT32 Result = 0;

#ifdef CFW_MULTI_SIM
        Result = SimReadBinaryReq(API_SIM_EF_PUCT, 0, 5, nSimID);
        if (ERR_SUCCESS != Result)
        {
          CFW_GetUTI(hSimGetPUCTAo, &nUTI);
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("EV_CFW_SIM_GET_PUCT_RSP fail:0x%x\r\n"), Result);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_PUCT_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimGetPUCTAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPUCTProc);

          return ERR_SUCCESS;
        }

#else
        Result = SimReadBinaryReq(API_SIM_EF_PUCT, 0, 5);
        if (ERR_SUCCESS != Result)
        {
          CFW_GetUTI(hSimGetPUCTAo, &nUTI);
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("EV_CFW_SIM_GET_PUCT_RSP fail:0x%x\r\n"), Result);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_PUCT_RSP, Result, 0, nUTI, 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimGetPUCTAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPUCTProc);

          return ERR_SUCCESS;
        }

#endif

        pSimGetPUCTData->nSm_GetPUCT_prevStatus = pSimGetPUCTData->nSm_GetPUCT_currStatus;
        pSimGetPUCTData->nSm_GetPUCT_currStatus = CFW_SIM_MIS_READBINARY;
        CFW_SetAoProcCode(hSimGetPUCTAo, CFW_AO_PROC_CODE_CONSUMED);
      }
      else
        CFW_SetAoProcCode(hSimGetPUCTAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case CFW_SIM_MIS_READBINARY:
    {
      if (nEvtId == API_SIM_READBINARY_CNF) // 用event id来鉴别此event是否为本函数当前想要处理的event
      {
        api_SimReadBinaryCnf_t* pSimReadBinaryCnf = (api_SimReadBinaryCnf_t*)nEvParam;
        CFW_GetUTI(hSimGetPUCTAo, &nUTI);

        if ((0x90 == pSimReadBinaryCnf->Sw1) && (0x00 == pSimReadBinaryCnf->Sw2)) // 操作成功的状态
        {
          UINT16 iEPPU    = 0;
          UINT8 iEX       = 0;  // mabo changed from INT8 to UINT8
          UINT8 iSignOfEX = 0;  // mabo added

          // Data[3]存储EPPU的高八位(bit12~bit5),Data[4]存储EPPU的低四位(bit4~bit1)
          iEPPU = (((UINT16)pSimReadBinaryCnf->Data[3] << 4) | ((UINT16)(pSimReadBinaryCnf->Data[4] & 0x0F)));
          iEX   = (UINT8)(pSimReadBinaryCnf->Data[4] >> 5); // Data[4]的高三位存储EX
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("EV_CFW_SIM_GET_PUCT_PROC Data[3] = %x,Data[4] = %x\r\n"),
                        pSimReadBinaryCnf->Data[3], pSimReadBinaryCnf->Data[4]);
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("EV_CFW_SIM_GET_PUCT_PROC iEX = %d\r\n"), iEX);
          if (pSimReadBinaryCnf->Data[4] & 0x10)  // Data[4]的bit5为EX的符号位，1表示负号
          {
            iSignOfEX = 1;
          }
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("EV_CFW_SIM_GET_PUCT_PROC iSignOfEX = %d\r\n"), iSignOfEX);

          CFW_SIM_PUCT_INFO* pPUCTData = (CFW_SIM_PUCT_INFO*)CSW_SIM_MALLOC(SIZEOF(CFW_SIM_PUCT_INFO));
          if (pPUCTData == NULL)
          {
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
            hAo = HNULL;
            CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error ERR_NO_MORE_MEMORY\r\n"));
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SIM_GET_PUCT_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SIM_GET_PUCT_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
          }
          pPUCTData->iCurrency[0] = pSimReadBinaryCnf->Data[0];
          pPUCTData->iCurrency[1] = pSimReadBinaryCnf->Data[1];
          pPUCTData->iCurrency[2] = pSimReadBinaryCnf->Data[2];
          pPUCTData->iEPPU        = iEPPU;
          pPUCTData->iEX          = iEX;
          pPUCTData->iSign        = iSignOfEX;
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("EV_CFW_SIM_GET_PUCT_RSP success:Currency = %c%c%c\r\n"),
                        pPUCTData->iCurrency[0], pPUCTData->iCurrency[1], pPUCTData->iCurrency[2]);
          CSW_TRACE(CFW_SIM_TS_ID,
                        TSTXT("EV_CFW_SIM_GET_PUCT_RSP success:iEPPU = %d,iEX = %d,iSign = %d\r\n"),
                        pPUCTData->iEPPU, pPUCTData->iEX, pPUCTData->iSign);

#ifdef CFW_MULTI_SIM
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_PUCT_RSP, (UINT32)pPUCTData, SIZEOF(CFW_SIM_PUCT_INFO),
                              nUTI | (nSimID << 24), 0);
#else
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_PUCT_RSP, (UINT32)pPUCTData, SIZEOF(CFW_SIM_PUCT_INFO),
                              nUTI , 0);

#endif
        }
        else  // 操作失败的状态，向mmi返回error code
        {
          // UINT32 nErrCode = 0;
#ifdef CFW_MULTI_SIM
          UINT32 nErrCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2, nSimID);

          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error EV_CFW_SIM_GET_PUCT_RSP 0x%x\r\n"), nErrCode);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_PUCT_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0);
#else
          UINT32 nErrCode = Sim_ParseSW1SW2(pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2);

          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error EV_CFW_SIM_GET_PUCT_RSP 0x%x\r\n"), nErrCode);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_PUCT_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0);

#endif
          
        }

        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimGetPUCTAo);
      }

      else  // 此event不是本函数当前想要处理的event
      {
        CFW_SetAoProcCode(hSimGetPUCTAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
      }
    }
    break;

    // case SM_GETPUCT_MMI:
    // break;
    // case SM_GETPUCT_STACK:
    // break;
  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPUCTProc);

  return ERR_SUCCESS;
}

UINT32 CFW_SimSetPUCTProc(HAO hAo, CFW_EV* pEvent)
{
  HAO hSimSetPUCTAo = hAo;

  CFW_SIMSETPUCT* pSimSetPUCTData = NULL;

  UINT32 nUTI     = 0;
  UINT32 nEvtId   = 0;
  UINT32 nErrCode = 0;
  VOID* nEvParam  = 0;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimSetPUCTProc);

  if ((UINT32)pEvent != 0xFFFFFFFF) // 不是第一次进入本函数
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }
#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hSimSetPUCTAo);
#endif
  pSimSetPUCTData = CFW_GetAoUserData(hSimSetPUCTAo);
  switch (pSimSetPUCTData->nSm_SetPUCT_currStatus)
  {
  case CFW_SIM_MIS_IDLE:
    {
      if ((UINT32)pEvent == 0xFFFFFFFF) // 用event id来鉴别此event是否为本函数当前想要处理的event
      {
#ifdef CFW_MULTI_SIM
        UINT32 Result = SimElemFileStatusReq(API_SIM_EF_PUCT, nSimID);

        if (ERR_SUCCESS != Result)
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n",Result);
          CFW_GetUTI(hSimSetPUCTAo, &nUTI);
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_PUCT_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetPUCTAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPUCTProc);

          return ERR_SUCCESS;
        }
#else
        UINT32 Result = SimElemFileStatusReq(API_SIM_EF_PUCT);

        if (ERR_SUCCESS != Result)
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n",Result);
          
          CFW_GetUTI(hSimSetPUCTAo, &nUTI);
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_PUCT_RSP, Result, 0, nUTI, 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetPUCTAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPUCTProc);

          return ERR_SUCCESS;
        }
#endif
        pSimSetPUCTData->nSm_SetPUCT_prevStatus = CFW_SIM_MIS_IDLE;
        pSimSetPUCTData->nSm_SetPUCT_currStatus = CFW_SIM_MIS_READELEMENT;

        CFW_SetAoProcCode(hSimSetPUCTAo, CFW_AO_PROC_CODE_CONSUMED);
      }

      else  // 此event不是本函数当前想要处理的event
      {
        CFW_SetAoProcCode(hSimSetPUCTAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
      }
    }
    break;

  case CFW_SIM_MIS_READELEMENT:
    {
      if (nEvtId == API_SIM_ELEMFILESTATUS_CNF) // 用event id来鉴别此event是否为本函数当前想要处理的event
      {
        api_SimElemFileStatusCnf_t* pSimElemFileStatusCnf = (api_SimElemFileStatusCnf_t*)nEvParam;
        if (((0x90 == pSimElemFileStatusCnf->Sw1) && (0x00 == pSimElemFileStatusCnf->Sw2)) || (0x91 == pSimElemFileStatusCnf->Sw1)) // 操作成功的状态
        {
          if ((UINT8)(pSimElemFileStatusCnf->Data[8] & 0x0F) == 2)  // Update操作需要CHV2鉴权
          {
#ifdef CFW_MULTI_SIM
            UINT32 Result = SimVerifyCHVReq(pSimSetPUCTData->pPin2, CHV2, nSimID);

            if (ERR_SUCCESS != Result)
            {
              CFW_GetUTI(hSimSetPUCTAo, &nUTI);
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error EV_CFW_SIM_GET_PUCT_RSP 0x%x\r\n"), Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_PUCT_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetPUCTAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPUCTProc);

              return ERR_SUCCESS;
            }
#else
            UINT32 Result = SimVerifyCHVReq(pSimSetPUCTData->pPin2, CHV2);

            if (ERR_SUCCESS != Result)
            {
              CFW_GetUTI(hSimSetPUCTAo, &nUTI);
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error EV_CFW_SIM_GET_PUCT_RSP 0x%x\r\n"), Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_PUCT_RSP, Result, 0, nUTI, 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetPUCTAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPUCTProc);

              return ERR_SUCCESS;
            }
#endif
            pSimSetPUCTData->nSm_SetPUCT_prevStatus = CFW_SIM_MIS_READELEMENT;
            pSimSetPUCTData->nSm_SetPUCT_currStatus = CFW_SIM_MIS_VERIFY;
          }

          else  // Update操作需要CHV1鉴权，在此不必进行PIN1验证，因为如果CHV1在使能状态，开机时就已经通过验证了
          {
            UINT8 Ex      = 0;
            UINT32 Result = 0;
            UINT8 Data[5];

            Data[0] = pSimSetPUCTData->iCurrency[0];
            Data[1] = pSimSetPUCTData->iCurrency[1];
            Data[2] = pSimSetPUCTData->iCurrency[2];
            Data[3] = pSimSetPUCTData->iEPPU >> 4;  // 存储EPPU的高八位(bit5~bit12)
            Data[4] = (UINT8)(pSimSetPUCTData->iEPPU & 0x0F); // 存储EPPU的低四位(bit1~bit4)

            // Data[4]的结构如下:
            // |---bit8~bit6---|--- bit5 ---|---bit4~bit1---|
            // |---    EX   ---|-sign of EX-|---   EPPU  ---|

            if (pSimSetPUCTData->iEX < 0) // EX值为负数
            {
              Data[4] |= 0x10;  // 将bit5置为1(negative sign)
              Ex = -(pSimSetPUCTData->iEX); // Ex取iEX的绝对值
            }
            else  // EX值为非负数
            {
              Ex = pSimSetPUCTData->iEX;  // 直接赋值，无需转换
            }
            Data[4] |= Ex << 5; // 高三位存储EX的值

#ifdef CFW_MULTI_SIM
            Result = SimUpdateBinaryReq(API_SIM_EF_PUCT, 0, 5, Data, nSimID);
            if (ERR_SUCCESS != Result)
            {
              CFW_GetUTI(hSimSetPUCTAo, &nUTI);
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimUpdateBinaryReq Result 0x%x\r\n"), Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_PUCT_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetPUCTAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPUCTProc);
              return ERR_SUCCESS;
            }
#else
            Result = SimUpdateBinaryReq(API_SIM_EF_PUCT, 0, 5, Data);
            if (ERR_SUCCESS != Result)
            {
              CFW_GetUTI(hSimSetPUCTAo, &nUTI);
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimUpdateBinaryReq Result 0x%x\r\n"), Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_PUCT_RSP, Result, 0, nUTI, 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetPUCTAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPUCTProc);
              return ERR_SUCCESS;
            }
#endif
            pSimSetPUCTData->nSm_SetPUCT_prevStatus = pSimSetPUCTData->nSm_SetPUCT_currStatus;
            pSimSetPUCTData->nSm_SetPUCT_currStatus = CFW_SIM_MIS_UPDATEBINARY;
          }
          CFW_SetAoProcCode(hSimSetPUCTAo, CFW_AO_PROC_CODE_CONSUMED);
        }

        else  // 操作失败的状态，向mmi返回error code
        {
          CFW_GetUTI(hSimSetPUCTAo, &nUTI);
#ifdef CFW_MULTI_SIM
          nErrCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2, nSimID);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_PUCT_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0);
#else
          nErrCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_PUCT_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0);
#endif
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error EV_CFW_SIM_GET_PUCT_RSP 0x%x\r\n"), nErrCode);

          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetPUCTAo);
        }
      }
      else  // 此event不是本函数当前想要处理的event
      {
        CFW_SetAoProcCode(hSimSetPUCTAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
      }
    }
    break;

  case CFW_SIM_MIS_VERIFY:
    {
      if (nEvtId == API_SIM_VERIFYCHV_CNF)  // 用event id来鉴别此event是否为本函数当前想要处理的event
      {
        api_SimVerifyCHVCnf_t* pSimVerifyCHVCnf = (api_SimVerifyCHVCnf_t*)nEvParam;
        if ((0x90 == pSimVerifyCHVCnf->Sw1) && (0x00 == pSimVerifyCHVCnf->Sw2)) // 操作成功的状态
        {
          UINT8 Ex      = 0;
          UINT32 Result = 0;
          UINT8 Data[5];

          Data[0] = pSimSetPUCTData->iCurrency[0];
          Data[1] = pSimSetPUCTData->iCurrency[1];
          Data[2] = pSimSetPUCTData->iCurrency[2];
          Data[3] = pSimSetPUCTData->iEPPU >> 4;  // 存储EPPU的高八位(bit5~bit12)
          Data[4] = (UINT8)(pSimSetPUCTData->iEPPU & 0x0F); // 存储EPPU的低四位(bit1~bit4)

          // Data[4]的结构如下:
          // |---bit8~bit6---|--- bit5 ---|---bit4~bit1---|
          // |---    EX   ---|-sign of EX-|---   EPPU  ---|

          if (pSimSetPUCTData->iEX < 0) // EX值为负数
          {
            Data[4] |= 0x10;  // 将bit5置为1(negative sign)
            Ex = -(pSimSetPUCTData->iEX); // Ex取iEX的绝对值
          }
          else  // EX值为非负数
          {
            Ex = pSimSetPUCTData->iEX;  // 直接赋值，无需转换
          }
          Data[4] |= Ex << 5; // 高三位存储EX的值
#ifdef CFW_MULTI_SIM
          Result = SimUpdateBinaryReq(API_SIM_EF_PUCT, 0, 5, Data, nSimID);
          if (ERR_SUCCESS != Result)
          {
            CFW_GetUTI(hSimSetPUCTAo, &nUTI);
            CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimUpdateBinaryReq 0x%x\r\n"), Result);
            CFW_PostNotifyEvent(EV_CFW_SIM_SET_PUCT_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
#else
          Result = SimUpdateBinaryReq(API_SIM_EF_PUCT, 0, 5, Data);
          if (ERR_SUCCESS != Result)
          {
            CFW_GetUTI(hSimSetPUCTAo, &nUTI);
            CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error SimUpdateBinaryReq 0x%x\r\n"), Result);
            CFW_PostNotifyEvent(EV_CFW_SIM_SET_PUCT_RSP, Result, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetPUCTAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPUCTProc);

            return ERR_SUCCESS;
          }

          pSimSetPUCTData->nSm_SetPUCT_prevStatus = CFW_SIM_MIS_VERIFY;
          pSimSetPUCTData->nSm_SetPUCT_currStatus = CFW_SIM_MIS_UPDATEBINARY;
          CFW_SetAoProcCode(hSimSetPUCTAo, CFW_AO_PROC_CODE_CONSUMED);
        }

        else  // 操作失败的状态，向mmi返回error code
        {
          CFW_GetUTI(hSimSetPUCTAo, &nUTI);
#ifdef CFW_MULTI_SIM
          nErrCode = Sim_ParseSW1SW2(pSimVerifyCHVCnf->Sw1, pSimVerifyCHVCnf->Sw2, nSimID);
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error EV_CFW_SIM_GET_PUCT_RSP  0x%x\r\n"), nErrCode);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_PUCT_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0);
#else
          nErrCode = Sim_ParseSW1SW2(pSimVerifyCHVCnf->Sw1, pSimVerifyCHVCnf->Sw2);
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error EV_CFW_SIM_GET_PUCT_RSP  0x%x\r\n"), nErrCode);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_PUCT_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0);
#endif
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetPUCTAo);
        }
      }

      else  // 此event不是本函数当前想要处理的event
      {
        CFW_SetAoProcCode(hSimSetPUCTAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
      }
    }
    break;

  case CFW_SIM_MIS_UPDATEBINARY:
    {
      if (nEvtId == API_SIM_UPDATEBINARY_CNF) // 用event id来鉴别此event是否为本函数当前想要处理的event
      {
        api_SimUpdateBinaryCnf_t* pSimUpdateBinaryCnf = (api_SimUpdateBinaryCnf_t*)nEvParam;
        CFW_GetUTI(hSimSetPUCTAo, &nUTI);
#ifdef CFW_MULTI_SIM
        if ((0x90 == pSimUpdateBinaryCnf->Sw1) && (0x00 == pSimUpdateBinaryCnf->Sw2)) // 操作成功的状态
        {
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("EV_CFW_SIM_SET_PUCT_RSP success\r\n"));
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_PUCT_RSP, 0, 0, nUTI | (nSimID << 24), 0);
        }

        else  // 操作失败的状态，向mmi返回error code
        {
          nErrCode = Sim_ParseSW1SW2(pSimUpdateBinaryCnf->Sw1, pSimUpdateBinaryCnf->Sw2, nSimID);
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error EV_CFW_SIM_GET_PUCT_RSP  0x%x\r\n"), nErrCode);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_PUCT_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0);
        }
#else
        if ((0x90 == pSimUpdateBinaryCnf->Sw1) && (0x00 == pSimUpdateBinaryCnf->Sw2)) // 操作成功的状态
        {
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("EV_CFW_SIM_SET_PUCT_RSP success\r\n"));
          CFW_PostNotifyEvent(EV_CFW_SIM_SET_PUCT_RSP, 0, 0, nUTI, 0);
        }

        else  // 操作失败的状态，向mmi返回error code
        {
          nErrCode = Sim_ParseSW1SW2(pSimUpdateBinaryCnf->Sw1, pSimUpdateBinaryCnf->Sw2);
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error EV_CFW_SIM_GET_PUCT_RSP  0x%x\r\n"), nErrCode);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_PUCT_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0);
        }
#endif
        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetPUCTAo);
      }

      else  // 此event不是本函数当前想要处理的event
      {
        CFW_SetAoProcCode(hSimSetPUCTAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
      }
    }
    break;

    // case SM_SETPUCT_MMI:
    // break;

    // case SM_SETPUCT_STACK:
    // break;
  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPUCTProc);
  return ERR_SUCCESS;
}
  

UINT32 CFW_SimMisGetProviderIdProc(HAO hAo,CFW_EV* pEvent)
{
    UINT32 nUTI=0x00000000;    
    UINT32 nEvtId=0x00000000;
    UINT32 ErrorCode=0x00000000;
    UINT32 result=0x00000000;
    VOID*  nEvParam                                        =NULL;
    CFW_EV ev;
    UINT8 IMSIAscLen=0x00;
    UINT8 *pIMSIAscData=NULL;
    SIM_SM_INFO_GETPROVIDERID       *pMisGetProviderID    =NULL;
    api_SimReadBinaryCnf_t            *pSimReadBinaryCnf    =NULL;

    CSW_PROFILE_FUNCTION_ENTER(CFW_SimMisGetProviderIdProc);

#ifdef CFW_MULTI_SIM
    CFW_SIM_ID nSimID;
    nSimID = CFW_GetSimCardID(hAo);
#endif

    pMisGetProviderID = CFW_GetAoUserData(hAo);
    if((UINT32)pEvent == 0xffffffff)
    {
        SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
        ev.nParam1 = 0xff;
        pEvent = &ev;
        pMisGetProviderID->nState.nNextState=CFW_SIM_MIS_IDLE;
    }
    else
    {
        nEvtId = pEvent->nEventId;
        nEvParam = (VOID*)pEvent->nParam1;
    }
    CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimMisGetProviderIdProc()\r\n") );
    
    switch(pMisGetProviderID->nState.nNextState)
    {
        case CFW_SIM_MIS_IDLE:
#ifdef CFW_MULTI_SIM
		      result=SimReadBinaryReq(API_SIM_EF_IMSI,0, 0x09, nSimID);
            if(ERR_SUCCESS!=result)
            {
                CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                hAo = HNULL; 
                CFW_PostNotifyEvent (EV_CFW_SIM_GET_PROVIDER_ID_RSP,(UINT32)result,0,nUTI|(nSimID<<24), 0xF0);
            }
#else
             result=SimReadBinaryReq(API_SIM_EF_IMSI,0, 0x09);
            if(ERR_SUCCESS!=result)
            {
                CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                hAo = HNULL; 
                CFW_PostNotifyEvent (EV_CFW_SIM_GET_PROVIDER_ID_RSP,(UINT32)result,0,nUTI, 0xF0);
            }
#endif
            pMisGetProviderID->nState.nCurrState=CFW_SIM_MIS_IDLE;
            pMisGetProviderID->nState.nNextState=CFW_SIM_MIS_READBINARY;
            break;
            
        case CFW_SIM_MIS_READBINARY:
            if (nEvtId == API_SIM_READBINARY_CNF) // 用event id来鉴别此event是否为本函数当前想要处理的event
            {
              pSimReadBinaryCnf=(api_SimReadBinaryCnf_t *)nEvParam;
              CFW_GetUTI(hAo, &nUTI);
              if(((0x90==pSimReadBinaryCnf->Sw1)&&(0x00==pSimReadBinaryCnf->Sw2))||(0x91==pSimReadBinaryCnf->Sw1))
              {      
                  pIMSIAscData = (UINT8 *)CSW_SIM_MALLOC(16);
#ifdef CFW_MULTI_SIM
                  if(pIMSIAscData==NULL)
                  {
                      CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                      hAo = HNULL;
                      CFW_PostNotifyEvent (EV_CFW_SIM_GET_PROVIDER_ID_RSP,ERR_NO_MORE_MEMORY,0,nUTI|(nSimID<<24), 0xF0);  
                  }
                  if(pSimReadBinaryCnf->Data[0] > 9)
                  {
                      CSW_SIM_FREE(pIMSIAscData);
                      pIMSIAscData = NULL;	
                      CFW_PostNotifyEvent (EV_CFW_SIM_GET_PROVIDER_ID_RSP,ERR_CME_UNKNOWN,0,nUTI|(nSimID<<24), 0xf0);  
                  }
                  else
          				{
          					if(pSimReadBinaryCnf->Data[0] == 0)
          					{
          						
          						CSW_SIM_FREE(pIMSIAscData);
          						CFW_PostNotifyEvent (EV_CFW_SIM_GET_PROVIDER_ID_RSP,0,0,nUTI|(nSimID<<24), 0);  
          					}
          					else
          					{
          						if (FALSE == g_GETIMSI[nSimID])
          						{
          							UINT8 nPreIMSI[10];

          							SUL_ZeroMemory8(nPreIMSI, 10);
          							UINT32 nRet = CFW_CfgGetIMSI(nPreIMSI, nSimID);
          							
          							if (ERR_SUCCESS != nRet)
          							{
          								CSW_SIM_FREE(pIMSIAscData);
          								pIMSIAscData = NULL;
          								CSW_TRACE(CFW_SIM_TS_ID, "The CFW_CfgGetIMSI nRet  is 0x%x\n", nRet);
          								
          								CFW_PostNotifyEvent(EV_CFW_SIM_GET_PROVIDER_ID_RSP, ERR_CME_UNKNOWN, 0, nUTI|(nSimID<<24), 0xf0);

          							}
          							if (0x00 == SUL_MemCompare(pSimReadBinaryCnf->Data, nPreIMSI, 9))
          							{
          								nPreIMSI[9] = 0x00;
          								nRet        = CFW_CfgSetIMSI(nPreIMSI, nSimID);
          								if (ERR_SUCCESS != nRet)
          								{
          									CSW_TRACE(CFW_SIM_TS_ID, "The CFW_CfgSetIMSI nRet  is 0x%x\n", nRet);
          									

          									CSW_SIM_FREE(pIMSIAscData);
          									pIMSIAscData = NULL;

          									CFW_PostNotifyEvent(EV_CFW_SIM_GET_PROVIDER_ID_RSP, ERR_CME_UNKNOWN, 0, nUTI|(nSimID<<24), 0xf0);
          								}
          							}
          							else
          							{
          								

          								nPreIMSI[9] = 0x01;
          								SUL_MemCopy8(nPreIMSI, pSimReadBinaryCnf->Data, 9);
          								nRet = CFW_CfgSetIMSI(nPreIMSI, nSimID);
          								if (ERR_SUCCESS != nRet)
          								{
          									

          									CSW_SIM_FREE(pIMSIAscData);
          									pIMSIAscData = NULL;
          									
          									CFW_PostNotifyEvent(EV_CFW_SIM_GET_PROVIDER_ID_RSP, ERR_CME_UNKNOWN, 0, nUTI|(nSimID<<24), 0xf0);
          								}
          								CSW_TRACE(CFW_SIM_TS_ID, "The nPreIMSI[9] is %d \n", nPreIMSI[9]);
          							}
          							g_GETIMSI[nSimID] = TRUE;
          						}
          						else
          						{
          						// .....
          						}
          						if (pIMSIAscData != NULL)
          						{
          							cfw_IMSItoASC(pSimReadBinaryCnf->Data,pIMSIAscData, &IMSIAscLen);
          						}
          						else
          						{
          							IMSIAscLen = 0;
          						}
          						
          						CFW_PostNotifyEvent (EV_CFW_SIM_GET_PROVIDER_ID_RSP,(UINT32)pIMSIAscData,IMSIAscLen,nUTI|(nSimID<<24), 0); 
          					}
          				}
                  CSW_TRACE(33, TSTXT(" post EV_CFW_SIM_GET_PROVIDER_ID_RSP Success! \n"));
              }
              else 
              {
                  ErrorCode = Sim_ParseSW1SW2( pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2,nSimID);
                  CFW_PostNotifyEvent (EV_CFW_SIM_GET_PROVIDER_ID_RSP,(UINT32)ErrorCode,0,nUTI|(nSimID<<24), 0xF0); 
              }
  			
#else

                if(pIMSIAscData==NULL)
                {
                    CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                    hAo = HNULL;
                    CFW_PostNotifyEvent (EV_CFW_SIM_GET_PROVIDER_ID_RSP,ERR_NO_MORE_MEMORY,0,nUTI, 0xF0);  
                }
                            
                if(pSimReadBinaryCnf->Data[0] > 9)
                {
                    CSW_SIM_FREE(pIMSIAscData);
                    pIMSIAscData = NULL;	
                    
                    CFW_PostNotifyEvent (EV_CFW_SIM_GET_PROVIDER_ID_RSP,ERR_CME_UNKNOWN,0,nUTI, 0xf0);            
                }
                else
                {
                    //------------------------------------------------------------
                   //Add by wanghb for  special card 2007.8.10 begin
                   //------------------------------------------------------------
                    if(pSimReadBinaryCnf->Data[0] == 0)
                    {
                        
                        CSW_SIM_FREE(pIMSIAscData);
                        CFW_PostNotifyEvent (EV_CFW_SIM_GET_PROVIDER_ID_RSP,0,0,nUTI, 0); 
                    }
                    // ------------------------------------------------------------
                    // Add by wanghb for special card 2007.8.10 end
                    // ------------------------------------------------------------
                    else
                    {
                      // ------------------------------------------------------------
                      // Add by Lixp for  special card 2007.8.17 begin
                      // ------------------------------------------------------------
                      if (FALSE == g_GETIMSI)
                      {
                        UINT8 nPreIMSI[10];

                        SUL_ZeroMemory8(nPreIMSI, 10);
                        UINT32 nRet = CFW_CfgGetIMSI(nPreIMSI);

                        

                        if (ERR_SUCCESS != nRet)
                        {
                          CSW_SIM_FREE(pIMSIAscData);
                          pIMSIAscData = NULL;
                          CSW_TRACE(CFW_SIM_TS_ID, "The CFW_CfgGetIMSI nRet  is 0x%x\n", nRet);
                           CFW_PostNotifyEvent(EV_CFW_SIM_GET_PROVIDER_ID_RSP, ERR_CME_UNKNOWN, 0, nUTI, 0xf0);
                        }
                        CSW_TRACE(CFW_SIM_TS_ID, "The nPreIMSI[0]is %d,pSimReadBinaryCnf->Data[0] is %d\n", nPreIMSI[0],
                                      pSimReadBinaryCnf->Data[0]);
                        if (0x00 == SUL_MemCompare(pSimReadBinaryCnf->Data, nPreIMSI, 9))
                        {
                        

                          nPreIMSI[9] = 0x00;
                          nRet        = CFW_CfgSetIMSI(nPreIMSI);
                          if (ERR_SUCCESS != nRet)
                          {
                            CSW_TRACE(CFW_SIM_TS_ID, "The CFW_CfgSetIMSI nRet  is 0x%x\n", nRet);
                            

                            CSW_SIM_FREE(pIMSIAscData);
                            pIMSIAscData = NULL;
                          
                            CFW_PostNotifyEvent(EV_CFW_SIM_GET_PROVIDER_ID_RSP, ERR_CME_UNKNOWN, 0, nUTI, 0xf0);
                          }

                        }
                        else
                        {
                          nPreIMSI[9] = 0x01;
                          SUL_MemCopy8(nPreIMSI, pSimReadBinaryCnf->Data, 9);

                          nRet = CFW_CfgSetIMSI(nPreIMSI);
                          if (ERR_SUCCESS != nRet)
                          {
                            

                            CSW_SIM_FREE(pIMSIAscData);
                            pIMSIAscData = NULL;
                            
                            CFW_PostNotifyEvent(EV_CFW_SIM_GET_PROVIDER_ID_RSP, ERR_CME_UNKNOWN, 0, nUTI, 0xf0);
                          }

                          CSW_TRACE(CFW_SIM_TS_ID, "The nPreIMSI[9] is %d \n", nPreIMSI[9]);
                        }
                        g_GETIMSI = TRUE;
                      }
                      else
                      {
                        // .....
                      }

                      // ------------------------------------------------------------
                      // Add by Lixp for special card 2007.8.17 end
                      // ------------------------------------------------------------

                      // apparently pIMSIAscData can be null for CFW_PostNotifyEvent 
                      // however cfw_IMSItoASC will write to 0 so avoid it.
                      if (pIMSIAscData != NULL)
                      {
                          cfw_IMSItoASC(pSimReadBinaryCnf->Data,pIMSIAscData, &IMSIAscLen);
                      }
                      else
                      {
                          IMSIAscLen = 0;
                      }
                      CFW_PostNotifyEvent (EV_CFW_SIM_GET_PROVIDER_ID_RSP,(UINT32)pIMSIAscData,IMSIAscLen,nUTI, 0); 
                  }
                }
                  CSW_TRACE(33, TSTXT(" post EV_CFW_SIM_GET_PROVIDER_ID_RSP Success! \n"));
              }
              else 
              {
                  ErrorCode = Sim_ParseSW1SW2( pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2);
                      CFW_PostNotifyEvent (EV_CFW_SIM_GET_PROVIDER_ID_RSP,(UINT32)ErrorCode,0,nUTI, 0xF0); 
              } 
  			
#endif
  		
              CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
              hAo = HNULL;            

        }
        else
        {
          CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
          
        }
            break;
            
        default:
            CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
            hAo = HNULL;
            break;
    }
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimMisGetProviderIdProc);
    
    return ERR_SUCCESS;
}



#ifdef CFW_EXTENDED_API


// ================================================================================
//  Function   : cfw_SimBinaryDataParserDefault
// --------------------------------------------------------------------------------
//
//  Scope      : Read binary EF and send back the whole data content 
//  Parameters : hAo -> Ao
//             : pValue -> pointer to the returned data (CFW's event param1)
//             : pResult -> binary read result structure
//             : pUserData -> the user (auxiliary) data containing the file Id & size
//             : simId -> SIM Id (dual SIM only)
//  Return     : none
// ================================================================================

VOID cfw_SimBinaryDataParserDefault(HAO hAo, UINT8 * pValue, api_SimReadBinaryCnf_t * pResult, SIM_SM_INFO_EF * pUserData
                                            #ifdef CFW_MULTI_SIM
                                            , CFW_SIM_ID simId
                                            #endif
                                            )
{
    // copy and sent result
    SUL_ZeroMemory8(pValue, pUserData->fileSize);
    SUL_MemCopy8(pValue, pResult->Data, pUserData->fileSize);
    CFW_SIM_SEND_RESULT(pValue, pUserData->fileSize | (pUserData->fileId << 8), EV_CFW_SIM_READ_EF_RSP);
}


// ================================================================================
//  Function   : cfw_SimReadUpdateElementaryFileProc
// --------------------------------------------------------------------------------
//
//  Scope      : Generic binary EF read & update function 
//  Parameters : hAo -> Ao
//             : pEvent -> CFW event
//  Return     : CFW error
// ================================================================================

UINT32 cfw_SimReadUpdateElementaryFileProc(HAO hAo, CFW_EV* pEvent)
{
    SIM_SM_INFO_EF *pUserData   = CFW_GetAoUserData(hAo);
#ifdef CFW_MULTI_SIM
    CFW_SIM_ID      simId       = CFW_GetSimCardID(hAo);
#endif
    SIM_DATA_PARSER_EF_T simDataParser;

    switch (pUserData->fileId)
    {
        case API_SIM_EF_PUCT : simDataParser = cfw_SimBinaryDataParserDefault; break;
        default              : simDataParser = cfw_SimBinaryDataParserDefault; break;
    }

    switch (pUserData->nState.nNextState)
    {
        case CFW_SIM_MIS_IDLE:
        {
            if ((UINT32)pEvent == 0xFFFFFFFF)
            {  
                // check remaining number of tries
                CFW_SIM_ASSERT_ERROR((pUserData->nTryCount<1), ERR_CME_SIM_FAILURE, EV_CFW_SIM_READ_EF_RSP);

                // get file information
        		UINT32 errorCode = SimElemFileStatusReq(CFW_WITH_SIM_ID(pUserData->fileId,simId));
                CFW_SIM_ASSERT_ERROR(errorCode, errorCode, EV_CFW_SIM_READ_EF_RSP);
                
                pUserData->nState.nCurrState = pUserData->nState.nNextState;
                pUserData->nState.nNextState = CFW_SIM_MIS_READELEMENT;

                // update read state
                CFW_SetAoState(hAo, CFW_AO_PROC_CODE_CONSUMED);
            }
            else
            {
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
            }
        }
        break;
        
        case CFW_SIM_MIS_READELEMENT:
        {
            if (pEvent->nEventId == API_SIM_ELEMFILESTATUS_CNF)
            {
                api_SimElemFileStatusCnf_t * pResult = ((UINT32)pEvent != 0xFFFFFFFF) ? (api_SimElemFileStatusCnf_t *)pEvent->nParam1 : NULL;

                // check SIM read status
                UINT32 errorCode = Sim_ParseSW1SW2(pResult->Sw1, CFW_WITH_SIM_ID(pResult->Sw2,simId));
                CFW_SIM_ASSERT_ERROR(CFW_SIM_ERROR(pResult), errorCode, EV_CFW_SIM_READ_EF_RSP);

                // print file size
                pUserData->fileSize = ((pResult->Data[2] << 8) & 0xff00) + pResult->Data[3];
                CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW SIM : file size is %d"), pUserData->fileSize);

                // read SIM binary file
                errorCode = SimReadBinaryReq(pUserData->fileId,0,CFW_WITH_SIM_ID(pUserData->fileSize,simId)); 
                CFW_SIM_ASSERT_ERROR(errorCode, errorCode, EV_CFW_SIM_READ_EF_RSP);
                
                // goto update state if required 
                pUserData->nState.nCurrState = pUserData->nState.nNextState;
                pUserData->nState.nNextState = (pUserData->update) ? CFW_SIM_MIS_UPDATEBINARY : CFW_SIM_MIS_READBINARY;
            }
            else
            {
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
            }
        }
        break;

        case CFW_SIM_MIS_READBINARY:   
        {
            if(pEvent->nEventId == API_SIM_READBINARY_CNF)
            {
                api_SimReadBinaryCnf_t * pResult = ((UINT32)pEvent != 0xFFFFFFFF) ? (api_SimReadBinaryCnf_t *)pEvent->nParam1 : NULL;
               
                // check SIM read status
                UINT32 errorCode = Sim_ParseSW1SW2(pResult->Sw1, CFW_WITH_SIM_ID(pResult->Sw2,simId));
                CFW_SIM_ASSERT_ERROR(CFW_SIM_ERROR(pResult), errorCode, EV_CFW_SIM_READ_EF_RSP);
                
                // allocate result message
                UINT8 * pValue = CSW_SIM_MALLOC(pUserData->fileSize);
                CFW_SIM_ASSERT_ERROR((pValue==NULL), ERR_NO_MORE_MEMORY, EV_CFW_SIM_READ_EF_RSP);
                    
                // parse the SIM data and send the results
                simDataParser(hAo,pValue,pResult,CFW_WITH_SIM_ID(pUserData,simId));

                }
            else
            {
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);                
            }            
        }
        break;

        case CFW_SIM_MIS_UPDATEBINARY:   
        break;

        default:
        break;
    }

    return ERR_SUCCESS;
}


/*
================================================================================
  Function   : CFW_SimMisSetPrefListProcEX
--------------------------------------------------------------------------------

  Scope      : The process for setting the PLMNs in SIM 
  Parameters :
  Return     :
================================================================================
*/
UINT32 CFW_SimMisSetPrefListProcEX(HAO hAo,CFW_EV* pEvent)
{
    UINT32 nUTI       = 0x00;    
    UINT32 nEvtId     = 0x00;
    UINT32 ErrorCode  = 0x00;
    UINT32 result     = 0x00;
    VOID*  nEvParam   = NULL;
    
    CFW_EV ev;
    
    SIM_SM_INFO_SETPREFOPREATORLIST *pMisSetPrefList         =NULL;
    api_SimUpdateBinaryCnf_t        *pSimUpdateBinaryCnf     =NULL;
    api_SimElemFileStatusCnf_t      *pSimElemFileStatusCnf   =NULL;


    CSW_PROFILE_FUNCTION_ENTER(CFW_SimMisSetPrefListProc);
#ifdef CFW_MULTI_SIM
    CFW_SIM_ID nSimID;
    nSimID = CFW_GetSimCardID(hAo);
#endif
    pMisSetPrefList = CFW_GetAoUserData(hAo);
    if((UINT32)pEvent == 0xffffffff)
    {
        SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
        ev.nParam1 = 0xff;
        pEvent = &ev;
        pMisSetPrefList->nState.nNextState=CFW_SIM_MIS_IDLE;
    }
    else
    {
        nEvtId = pEvent->nEventId;
        nEvParam = (VOID*)pEvent->nParam1;
    }
    CSW_TRACE(33, TSTXT("CFW_SimMisSetPrefListProc()\r\n") );

    switch(pMisSetPrefList->nState.nNextState)
    {
        case CFW_SIM_MIS_IDLE:
#ifdef CFW_MULTI_SIM
		    result=SimElemFileStatusReq(API_SIM_EF_PLMNSEL,nSimID);
#else
            result=SimElemFileStatusReq(API_SIM_EF_PLMNSEL);
#endif
			if(ERR_SUCCESS!=result)
            {
                CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                hAo = HNULL; 
#ifdef CFW_MULTI_SIM
                CFW_PostNotifyEvent (EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP,(UINT32)result,0,nUTI|(nSimID<<24), 0xF0);
#else
                CFW_PostNotifyEvent (EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP,(UINT32)result,0,nUTI, 0xF0);
#endif
            }
            pMisSetPrefList->nState.nCurrState=CFW_SIM_MIS_IDLE;
            pMisSetPrefList->nState.nNextState=CFW_SIM_MIS_READELEMENT;
            break;
            
        case CFW_SIM_MIS_READELEMENT:
            pSimElemFileStatusCnf=(api_SimElemFileStatusCnf_t *)nEvParam;
            CFW_GetUTI(hAo, &nUTI);
            if(((0x90==pSimElemFileStatusCnf->Sw1)&&(0x00==pSimElemFileStatusCnf->Sw2))||(0x91==pSimElemFileStatusCnf->Sw1))
            {
                pMisSetPrefList->nSize=((pSimElemFileStatusCnf->Data[2]<<8)&0xFF00)+pSimElemFileStatusCnf->Data[3];
#ifdef CFW_MULTI_SIM
			result=SimUpdateBinaryReq(API_SIM_EF_PLMNSEL,0, pMisSetPrefList->nSize,pMisSetPrefList->nOperatorList,nSimID);
#else
			result=SimUpdateBinaryReq(API_SIM_EF_PLMNSEL,0, pMisSetPrefList->nSize,pMisSetPrefList->nOperatorList);
#endif
			if(ERR_SUCCESS!=result)
            {
                CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                hAo = HNULL; 
#ifdef CFW_MULTI_SIM
                CFW_PostNotifyEvent (EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP,(UINT32)result,0,nUTI|(nSimID<<24), 0xF0); 
#else
          		CFW_PostNotifyEvent (EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP,(UINT32)result,0,nUTI, 0xF0); 
#endif
            }
            pMisSetPrefList->nState.nCurrState=CFW_SIM_MIS_IDLE;
            pMisSetPrefList->nState.nNextState=CFW_SIM_MIS_UPDATEBINARY;
            }
            else
            {
#ifdef CFW_MULTI_SIM
          		ErrorCode = Sim_ParseSW1SW2( pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2,nSimID);
				CFW_PostNotifyEvent (EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP,(UINT32)ErrorCode,0,nUTI|(nSimID<<24), 0xF0);
#else
                ErrorCode = Sim_ParseSW1SW2( pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2);
				CFW_PostNotifyEvent (EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP,(UINT32)ErrorCode,0,nUTI, 0xF0);
#endif
                CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                hAo = HNULL;
            }
            break;
            
        case CFW_SIM_MIS_UPDATEBINARY:
            pSimUpdateBinaryCnf=(api_SimUpdateBinaryCnf_t *)nEvParam;
            CFW_GetUTI(hAo, &nUTI);
            if(((0x90==pSimUpdateBinaryCnf->Sw1)&&(0x00==pSimUpdateBinaryCnf->Sw2))||(0x91==pSimUpdateBinaryCnf->Sw1))
            {
#ifdef CFW_MULTI_SIM
                CFW_PostNotifyEvent (EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP,(UINT32)0,0,nUTI|(nSimID<<24),0);    
#else
          		CFW_PostNotifyEvent (EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP,(UINT32)0,0,nUTI,0);    
#endif
				CSW_TRACE(33, TSTXT(" post EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP Success! \n"));
            }
            else
            {
#ifdef CFW_MULTI_SIM
                ErrorCode = Sim_ParseSW1SW2( pSimUpdateBinaryCnf->Sw1, pSimUpdateBinaryCnf->Sw2,nSimID);
				CFW_PostNotifyEvent (EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP,(UINT32)ErrorCode,0,nUTI|(nSimID<<24), 0xF0); 
#else
	            ErrorCode = Sim_ParseSW1SW2( pSimUpdateBinaryCnf->Sw1, pSimUpdateBinaryCnf->Sw2);	
				CFW_PostNotifyEvent (EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP,(UINT32)ErrorCode,0,nUTI, 0xF0); 
#endif
                CSW_TRACE(33, TSTXT(" post EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP failure! \n"));    
            }
            
            CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
            hAo = HNULL; 
            break;
            
        default:
            CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
            hAo = HNULL;
            break;
    }
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimMisSetPrefListProc);
    return ERR_SUCCESS;
}


/*
================================================================================
  Function   : CFW_SimMisGetPrefListProcEX
--------------------------------------------------------------------------------

  Scope      : The process for Getting the PLMNs in SIM 
  Parameters :
  Return     :
================================================================================
*/
UINT32 CFW_SimMisGetPrefListProcEX(HAO hAo,CFW_EV* pEvent)
{
    UINT32 nUTI     = 0x00;    
    UINT32 nEvtId   = 0x00;
    UINT32 ErrorCode= 0x00;
    UINT32 result   = 0x00;
    VOID*  nEvParam = NULL;
    
    CFW_EV ev;
    
    SIM_SM_INFO_GETPREFOPREATORLIST * pMisGetPreflist       =  NULL;
    api_SimReadBinaryCnf_t          * pSimReadBinaryCnf     =  NULL;
    api_SimElemFileStatusCnf_t      * pSimElemFileStatusCnf =  NULL;


    CSW_PROFILE_FUNCTION_ENTER(CFW_SimMisGetPrefListProcEX);

#ifdef CFW_MULTI_SIM
    CFW_SIM_ID nSimID;
    nSimID = CFW_GetSimCardID(hAo);
#endif

    pMisGetPreflist = CFW_GetAoUserData(hAo);
    if((UINT32)pEvent == 0xffffffff)
    {
        SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
        ev.nParam1 = 0xff;
        pEvent = &ev;
        pMisGetPreflist->nState.nNextState=CFW_SIM_MIS_IDLE;
    }
    else
    {
        nEvtId = pEvent -> nEventId;
        nEvParam = (VOID*)pEvent -> nParam1;
    }
    CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimMisGetPrefListProcEX()\r\n") );

    switch(pMisGetPreflist->nState.nNextState)
    {
        case CFW_SIM_MIS_IDLE:          
#ifdef CFW_MULTI_SIM
    	    result=SimElemFileStatusReq(API_SIM_EF_PLMNSEL,nSimID);
#else
            result=SimElemFileStatusReq(API_SIM_EF_PLMNSEL);
#endif
			if(ERR_SUCCESS!=result)
            {
                CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimMisGetPrefListProcEX SimElemFileStatusReq failed \r\n") );

                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
                hAo = HNULL; 
#ifdef CFW_MULTI_SIM
                CFW_PostNotifyEvent (EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP, (UINT32)result, 0, nUTI|(nSimID<<24), 0xF0); 
#else
                CFW_PostNotifyEvent (EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP, (UINT32)result, 0, nUTI, 0xF0); 
#endif
            }
            
            pMisGetPreflist->nState.nCurrState = CFW_SIM_MIS_IDLE;
            pMisGetPreflist->nState.nNextState = CFW_SIM_MIS_READELEMENT;
            break;
            
        case CFW_SIM_MIS_READBINARY:
          
            pSimReadBinaryCnf = (api_SimReadBinaryCnf_t *)nEvParam;
            CFW_GetUTI(hAo, &nUTI);
            if(((0x90 == pSimReadBinaryCnf->Sw1) && (0x00 == pSimReadBinaryCnf->Sw2))
                                    ||(0x91 == pSimReadBinaryCnf->Sw1))
            {              
                //------------------------------------------------------------
                //Add by lixp for Plmn raise 2007-06-25 begin
                //------------------------------------------------------------
                
                pMisGetPreflist->nSize = pMisGetPreflist->nSize / 3 *3;
                
                //------------------------------------------------------------
                //Add by lixp for Plmn raise 2007-06-25 endSS
                //------------------------------------------------------------
                
                UINT8 * pPLMN=(UINT8 *)CSW_SIM_MALLOC(2 * ( pMisGetPreflist->nSize ));
                if(pPLMN == NULL)
                {
                    CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                    hAo = HNULL; 
#ifdef CFW_MULTI_SIM
                    CFW_PostNotifyEvent (EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP,ERR_NO_MORE_MEMORY,0,nUTI|(nSimID<<24), 0xF0); 
#else
	                CFW_PostNotifyEvent (EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP,ERR_NO_MORE_MEMORY,0,nUTI, 0xF0); 
#endif
				}
                
                cfw_PLMNtoBCDEX(pSimReadBinaryCnf->Data, pPLMN, &(pMisGetPreflist->nSize));    
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent (EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP,(UINT32)pPLMN,pMisGetPreflist->nSize,nUTI|(nSimID<<24),0);  
#else
        		CFW_PostNotifyEvent (EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP,(UINT32)pPLMN,pMisGetPreflist->nSize,nUTI,0);  
#endif
                CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" post EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP success! \n"));    
                
            }
            else 
            {    
#ifdef CFW_MULTI_SIM
                ErrorCode = Sim_ParseSW1SW2( pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2,nSimID);
				CFW_PostNotifyEvent (EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP,(UINT32)ErrorCode,0,nUTI|(nSimID<<24), 0xF0); 
#else
                ErrorCode = Sim_ParseSW1SW2( pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2);
				CFW_PostNotifyEvent (EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP,(UINT32)ErrorCode,0,nUTI, 0xF0); 
#endif
                
                CSW_TRACE(33, TSTXT(" post EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP failure! \n"));
            }      
            
            CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
            hAo = HNULL;
            break;
            
        case CFW_SIM_MIS_READELEMENT:
          
            pSimElemFileStatusCnf=(api_SimElemFileStatusCnf_t *)nEvParam;
            CFW_GetUTI(hAo, &nUTI);
            if(((0x90==pSimElemFileStatusCnf->Sw1)&&(0x00==pSimElemFileStatusCnf->Sw2))||(0x91==pSimElemFileStatusCnf->Sw1))
            {
                pMisGetPreflist->nSize = (( pSimElemFileStatusCnf->Data[2] << 8) & 0xFF00)
                  + pSimElemFileStatusCnf -> Data[3];
#ifdef CFW_MULTI_SIM
                result=SimReadBinaryReq(API_SIM_EF_PLMNSEL,0,pMisGetPreflist->nSize,nSimID);
#else
                result=SimReadBinaryReq(API_SIM_EF_PLMNSEL,0,pMisGetPreflist->nSize);
#endif
				if(ERR_SUCCESS!=result)
                {
                    CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                    hAo = HNULL; 
#ifdef CFW_MULTI_SIM
                    CFW_PostNotifyEvent (EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP,(UINT32)result,0,nUTI|(nSimID<<24), 0xF0); 
#else
              		CFW_PostNotifyEvent (EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP,(UINT32)result,0,nUTI, 0xF0); 
#endif
				}
            }
            else
            {
                CSW_TRACE(33, TSTXT(" SIM GET PREFOPREATORLIST ERROR! \n"));
#ifdef CFW_MULTI_SIM
                ErrorCode = Sim_ParseSW1SW2( pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2,nSimID);
				CFW_PostNotifyEvent (EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP,(UINT32)ErrorCode,0,nUTI|(nSimID<<24), 0xF0);
#else
                ErrorCode = Sim_ParseSW1SW2( pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2);
				CFW_PostNotifyEvent (EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP,(UINT32)ErrorCode,0,nUTI, 0xF0);
#endif
                CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                hAo = HNULL;
            }
            pMisGetPreflist->nState.nCurrState=CFW_SIM_MIS_READELEMENT;
            pMisGetPreflist->nState.nNextState=CFW_SIM_MIS_READBINARY;
            break;
            
        default:
            CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
            hAo = HNULL;
            break;
            
    }
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimMisGetPrefListProc);
    return ERR_SUCCESS;
}


/*
================================================================================
  Function   : CFW_SimGetFileStatusProc
--------------------------------------------------------------------------------

  Scope      : 
  Parameters :
  Return     :
================================================================================
*/
UINT32 CFW_SimGetFileStatusProc(HAO hAo, CFW_EV* pEvent)
{
    UINT32 nEvtId,nUTI,ErrorCode;
    VOID*  nEvParam;
    GET_FILE_STATUS_INFO * pGetFileStatus;

    nEvtId     = 0x00;
    nUTI       = 0x00;
    ErrorCode  = 0x00;
    nEvParam   = (VOID*)NULL;
    pGetFileStatus = (GET_FILE_STATUS_INFO *)NULL;

    CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetFileStatusProc);
    if((UINT32)pEvent != 0xFFFFFFFF)
    {
        nEvtId = pEvent->nEventId;
        nEvParam = (VOID*)pEvent->nParam1;
    }
#ifdef CFW_MULTI_SIM
    CFW_SIM_ID nSimID;
    nSimID = CFW_GetSimCardID(hAo);
#endif

    pGetFileStatus = CFW_GetAoUserData(hAo);
    
    switch(pGetFileStatus->n_CurrStatus)
    {
    case CFW_SIM_MIS_IDLE:
        {
            if((UINT32)pEvent == 0xFFFFFFFF)
            {
                if(pGetFileStatus->nTryCount >= 0x01)
                {
#ifdef CFW_MULTI_SIM
                    ErrorCode = SimElemFileStatusReq(pGetFileStatus->nFileID,nSimID);
#else
                    ErrorCode = SimElemFileStatusReq(pGetFileStatus->nFileID);
#endif
					if(ERR_SUCCESS != ErrorCode)
                    {
                        CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
                        CFW_PostNotifyEvent (EV_CFW_SIM_GET_FILE_STATUS_RSP,(UINT32)ErrorCode,0,nUTI|(nSimID<<24), 0XF0);            
#else
                        CFW_PostNotifyEvent (EV_CFW_SIM_GET_FILE_STATUS_RSP,(UINT32)ErrorCode,0,nUTI, 0XF0);            
#endif
						CSW_TRACE(CFW_SIM_TS_ID, TSTXT("SimElemFileStatusReq err,The ErrCode is %d\n"),ErrorCode);
                        
                        CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);  
                        hAo = HNULL;
                        CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetFileStatusProc);
                        return ErrorCode;
                    }

                    CFW_SetAoState(hAo, CFW_SM_SIM_STATE_WRITE); 
                    
                    pGetFileStatus->n_PrevStatus = pGetFileStatus->n_CurrStatus;
                    pGetFileStatus->n_CurrStatus = CFW_SIM_MIS_READELEMENT;
                    //pGetMR->nTryCount --;
                }
                else
                {
                    //Time out and TODO...
                    CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                    hAo = HNULL;
                }
            }
            else
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);        
        }
        break;
        
    case CFW_SIM_MIS_READELEMENT:   
        {
            if(pEvent->nEventId == API_SIM_ELEMFILESTATUS_CNF)
            {
                api_SimElemFileStatusCnf_t * pSimEFStatusCnf=(api_SimElemFileStatusCnf_t *)nEvParam;
                    CFW_GetUTI(hAo, &nUTI);
                if((0x90==pSimEFStatusCnf->Sw1)&&(00==pSimEFStatusCnf->Sw2))
                {   
                    /* 3gpp11.11 page50
                    Structure of file
                    '00'  transparent;
                    '01'  linear fixed;
                    '03'  cyclic.
                    
                    Type of File
                    '00'  RFU;
                    '01'  MF;
                    '02'  DF;
                    '04'  EF.

                    */
                    //
                    //参数P1低16位为文件长度，高16位为文件类型。
                    //
                    UINT32 p1 =((pSimEFStatusCnf->Data[2] << 8) & 0xFF00) + pSimEFStatusCnf->Data[3];
                    p1 += (UINT32)(pSimEFStatusCnf->Data[6] << 16);
                    //
                    //参数P2低三个字节是ACCESS CONDITIONS,高1个字节是文件状态。
                    //                    
                    UINT32 p2 = (UINT32) ((pSimEFStatusCnf->Data[8] << 16) + (pSimEFStatusCnf->Data[9] << 8)
                    + pSimEFStatusCnf->Data[10] + (pSimEFStatusCnf->Data[11] << 24));
#ifdef CFW_MULTI_SIM
                    CFW_PostNotifyEvent (EV_CFW_SIM_GET_FILE_STATUS_RSP,(UINT32)p1,p2,nUTI|(nSimID<<24), 0);            
#else
                    CFW_PostNotifyEvent (EV_CFW_SIM_GET_FILE_STATUS_RSP,(UINT32)p1,p2,nUTI, 0);            
#endif
					CSW_TRACE(33, TSTXT(" post EV_CFW_SIM_GET_FILE_STATUS_RSP Success! \n"));
                }
                else 
                {
#ifdef CFW_MULTI_SIM
                    ErrorCode = Sim_ParseSW1SW2( pSimEFStatusCnf->Sw1, pSimEFStatusCnf->Sw2,nSimID);
					CFW_PostNotifyEvent (EV_CFW_SIM_GET_FILE_STATUS_RSP,(UINT32)ErrorCode,0,nUTI|(nSimID<<24), 0xf0); 
#else
                    ErrorCode = Sim_ParseSW1SW2( pSimEFStatusCnf->Sw1, pSimEFStatusCnf->Sw2);
					CFW_PostNotifyEvent (EV_CFW_SIM_GET_FILE_STATUS_RSP,(UINT32)ErrorCode,0,nUTI, 0xf0); 
#endif
                }
                
                CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);  
                hAo = HNULL;
            }
            else
            {
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);                
            }            
        }
        break;          
    }
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetFileStatusProc);
    return ERR_SUCCESS;
}



/*
================================================================================
  Function   : CFW_SimReadRecordProc
--------------------------------------------------------------------------------

  Scope      : 
  Parameters :
  Return     :
================================================================================
*/

UINT32 CFW_SimReadRecordProc (HAO hAo,CFW_EV* pEvent)
{
    UINT32 nEvtId   = 0x00;
    UINT32 nUTI     = 0x00;
    UINT32 nErrCode = 0x00;
    VOID*  nEvParam = (VOID*)NULL;
    SIM_INFO_READRECORD *pReadSimInfo;
    CSW_PROFILE_FUNCTION_ENTER(CFW_SimReadRecordProc);
    if((UINT32)pEvent != 0xFFFFFFFF)
    {
        nEvtId = pEvent->nEventId;
        nEvParam = (VOID*)pEvent->nParam1;
    }
#ifdef CFW_MULTI_SIM
    CFW_SIM_ID nSimID;
    nSimID = CFW_GetSimCardID(hAo);
#endif


    pReadSimInfo = CFW_GetAoUserData(hAo);
    
    switch(pReadSimInfo->n_CurrStatus)
    {
     case SIM_STATUS_IDLE:
        {
            if((UINT32)pEvent == 0xFFFFFFFF)
            {
                if(pReadSimInfo->nTryCount >= 0x01)
                {
                  
    				CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
#ifdef CFW_MULTI_SIM
        			nErrCode = SimElemFileStatusReq((UINT8)(pReadSimInfo->nFileId),nSimID);
#else
					nErrCode = SimElemFileStatusReq((UINT8)(pReadSimInfo->nFileId));
#endif
					if(ERR_SUCCESS != nErrCode)
                    {
                        CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
                        CFW_PostNotifyEvent (EV_CFW_SIM_READ_RECORD_RSP ,(UINT32)nErrCode,
                            0,nUTI|(nSimID<<24), 0xF0);
#else
                        CFW_PostNotifyEvent (EV_CFW_SIM_READ_RECORD_RSP ,(UINT32)nErrCode,
                            0,nUTI, 0xF0);
#endif
                        CSW_TRACE(CFW_SIM_TS_ID, TSTXT("SimElemFileStatusReq Failed in CFW_SimReadRecordProc\n"));
                        CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                        hAo = HNULL;
                        CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadRecordProc);
                        return nErrCode;
                    }    
                    pReadSimInfo->n_PrevStatus = pReadSimInfo->n_CurrStatus;
                    pReadSimInfo->n_CurrStatus = SIM_STATUS_ELEMFILE;
                    CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
                }
                else
                {
                    CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                    hAo = HNULL;
                }    

            }
            else
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);        
        }
        break;
        
    case SIM_STATUS_ELEMFILE:
        {
            if(nEvtId == API_SIM_ELEMFILESTATUS_CNF)
            {
                api_SimElemFileStatusCnf_t*  pSimElemFileStatusCnf=(api_SimElemFileStatusCnf_t *)nEvParam;                
                if(((0x90==pSimElemFileStatusCnf->Sw1)&&(00==pSimElemFileStatusCnf->Sw2))||(0x91==pSimElemFileStatusCnf->Sw1))
                {
    				  CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
                      pReadSimInfo->nRecordSize =  pSimElemFileStatusCnf->Data[14];
#ifdef CFW_MULTI_SIM
                      UINT32 ErrorCode = SimReadRecordReq(pReadSimInfo->nFileId, pReadSimInfo->nRecordNum, SIM_MODE_ABSOLUTE, pReadSimInfo->nRecordSize,nSimID);
#else
                      UINT32 ErrorCode = SimReadRecordReq(pReadSimInfo->nFileId, pReadSimInfo->nRecordNum, SIM_MODE_ABSOLUTE, pReadSimInfo->nRecordSize);
#endif
                      if(ErrorCode != ERR_SUCCESS)
                      {
                          CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
                          CFW_PostNotifyEvent (EV_CFW_SIM_READ_RECORD_RSP ,(UINT32)nErrCode, 0,nUTI|(nSimID<<24), 0xF0);
#else
                          CFW_PostNotifyEvent (EV_CFW_SIM_READ_RECORD_RSP ,(UINT32)nErrCode, 0,nUTI, 0xF0);
#endif
						  CSW_TRACE(CFW_SIM_TS_ID, TSTXT("SimReadRecordReq Failed in CFW_SimReadRecordProc\n"));
                          CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                          hAo = HNULL;
                          CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadRecordProc);
                          return nErrCode;
                      }

                    pReadSimInfo->n_PrevStatus = pReadSimInfo->n_CurrStatus;
                    pReadSimInfo->n_CurrStatus = SIM_STATUS_READRECORD;
                    CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);                    
                }
                else
                {
#ifdef CFW_MULTI_SIM
	               UINT32 ErrorCode = Sim_ParseSW1SW2((UINT8)pSimElemFileStatusCnf->Sw1, (UINT8)pSimElemFileStatusCnf->Sw2,nSimID);
#else
                   UINT32 ErrorCode = Sim_ParseSW1SW2((UINT8)pSimElemFileStatusCnf->Sw1, (UINT8)pSimElemFileStatusCnf->Sw2);
#endif
                   CFW_GetUTI(hAo, &nUTI); 
#ifdef CFW_MULTI_SIM
                   CFW_PostNotifyEvent (EV_CFW_SIM_READ_RECORD_RSP ,(UINT32)ErrorCode, 0,nUTI|(nSimID<<24), 0xf0);
#else
                   CFW_PostNotifyEvent (EV_CFW_SIM_READ_RECORD_RSP ,(UINT32)ErrorCode, 0,nUTI, 0xf0);
#endif
                   CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" SIM read ReadRecord Error CFW_SimReadRecordProc !\n"));
                   CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                   hAo = HNULL;
                   CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadRecordProc);
                   return ErrorCode;
                }
            }
            else
            {
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
            }
        }          
        break;
        
    case SIM_STATUS_READRECORD:
        {
            if(pEvent->nEventId == API_SIM_READRECORD_CNF)
            {                
                api_SimReadRecordCnf_t *pSimReadRecordCnf = (api_SimReadRecordCnf_t *)nEvParam;         
                CSW_TRACE(CFW_SIM_TS_ID, "The sw1 is %d,    The sw2 is %d\n", pSimReadRecordCnf->Sw1,pSimReadRecordCnf->Sw2);
               
                if(((0x90==pSimReadRecordCnf->Sw1) && (00==pSimReadRecordCnf->Sw2)) || (0x91==pSimReadRecordCnf->Sw1))
                {
    				CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
                    UINT8 *pGetRecord = (UINT8 *)CSW_SIM_MALLOC(pReadSimInfo->nRecordSize);
                    if(pGetRecord == NULL)
                    {
                        CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
                        CFW_PostNotifyEvent (EV_CFW_SIM_READ_RECORD_RSP, (UINT32)ERR_NO_MORE_MEMORY,
                            0,nUTI|(nSimID<<24), 0xf0);
#else
                        CFW_PostNotifyEvent (EV_CFW_SIM_READ_RECORD_RSP, (UINT32)ERR_NO_MORE_MEMORY,
                            0,nUTI, 0xf0);
#endif
						CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimReadMessage-->SIM_STATUS_READRECORD----> ERR_NO_MORE_MEMORY! \n"));
                        CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                        hAo = HNULL;
                        CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadRecordProc);
                        return ERR_NO_MORE_MEMORY;
                    }
                    
                    SUL_MemCopy8(pGetRecord,pSimReadRecordCnf->Data,pReadSimInfo->nRecordSize);
                    
                    CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
                    CFW_PostNotifyEvent (EV_CFW_SIM_READ_RECORD_RSP,(UINT32)pGetRecord,pReadSimInfo->nRecordSize,nUTI|(nSimID<<24), 0x00);
#else
                    CFW_PostNotifyEvent (EV_CFW_SIM_READ_RECORD_RSP,(UINT32)pGetRecord,pReadSimInfo->nRecordSize,nUTI, 0x00);
#endif
                    CSW_TRACE(CFW_SIM_TS_ID, TSTXT("post EV_CFW_SIM_READ_RECORD_RSP Success! \n"));
                    CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                    hAo = HNULL;
                    CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadRecordProc);
                    return ERR_SUCCESS;
                }
                else
                {
    				CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
#ifdef CFW_MULTI_SIM
        			UINT32 ErrorCode = Sim_ParseSW1SW2((UINT8)(pSimReadRecordCnf->Sw1),(UINT8)(pSimReadRecordCnf->Sw2),nSimID);
#else
					UINT32 ErrorCode = Sim_ParseSW1SW2((UINT8)(pSimReadRecordCnf->Sw1),(UINT8)(pSimReadRecordCnf->Sw2));
#endif               
                    CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
                    CFW_PostNotifyEvent (EV_CFW_SIM_READ_RECORD_RSP ,(UINT32)ErrorCode,0,nUTI|(nSimID<<24), 0xf0);
#else
                    CFW_PostNotifyEvent (EV_CFW_SIM_READ_RECORD_RSP ,(UINT32)ErrorCode,0,nUTI, 0xf0);
#endif
                    CSW_TRACE(CFW_SIM_TS_ID, TSTXT("SIM read ReadRecord Error! \n"));
                    CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                    hAo = HNULL;
                    CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadRecordProc);
                    return ErrorCode;
                }                
                //CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
            }
            else
            {
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);        
            }            
        }
        break;   

        default:
            CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
            hAo = HNULL;
        break;  
    }
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadRecordProc);
    return ERR_SUCCESS;
}



/*
================================================================================
  Function   : CFW_SimUpdateRecordProc
--------------------------------------------------------------------------------

  Scope      : 
  Parameters :
  Return     :
================================================================================
*/

UINT32 CFW_SimUpdateRecordProc (HAO hAo,CFW_EV* pEvent)
{
    UINT32 nEvtId,nUTI; //[-]dingmx 20080908 remove warning nErrCode
    VOID*  nEvParam = (VOID*)NULL;
    SIM_INFO_UPDATERECORD *pUpdateRecord;
    CSW_PROFILE_FUNCTION_ENTER(CFW_SimUpdateRecordProc);
    if((UINT32)pEvent != 0xFFFFFFFF)
    {
        nEvtId = pEvent->nEventId;
        nEvParam = (VOID*)pEvent->nParam1;
    }
#ifdef CFW_MULTI_SIM
    CFW_SIM_ID nSimID;
    nSimID = CFW_GetSimCardID(hAo);
#endif


    pUpdateRecord = CFW_GetAoUserData(hAo);
    switch(pUpdateRecord->n_CurrStatus)
    {
     case SIM_STATUS_IDLE:
        {
            if((UINT32)pEvent == 0xFFFFFFFF)
            {
                if(pUpdateRecord->nTryCount >= 0x01)
                {
                  //Add by lixp at 2007-12-06
                  //Remove for At issue                  
                  /*
                  if(( CFW_SIM_FILE_FDN   == pUpdateRecord->nFileId) || 
                     ( CFW_SIM_FILE_EXT2  == pUpdateRecord->nFileId) || 
                     ( CFW_SIM_FILE_BDN   == pUpdateRecord->nFileId) || 
                     ( CFW_SIM_FILE_EXT4  == pUpdateRecord->nFileId) )//Need pin2
                  {
    CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
                    nErrCode = SimElemFileStatusReq((UINT8)(pUpdateRecord->nFileId));
                    if(ERR_SUCCESS != nErrCode)
                    {
                        CFW_GetUTI(hAo, &nUTI);
                        CFW_PostNotifyEvent (EV_CFW_SIM_UPDATE_RECORD_RSP ,(UINT32)nErrCode, 0,nUTI, 0xF0);
                        CSW_TRACE(CFW_SIM_TS_ID, TSTXT("SimElemFileStatusReq Failed in CFW_SimReadRecordProc\n"));
                        CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                        hAo = HNULL;
                        CSW_PROFILE_FUNCTION_EXIT(CFW_SimUpdateRecordProc );
                        return nErrCode;
                    }    
                    pUpdateRecord->n_PrevStatus = pUpdateRecord->n_CurrStatus;
                    pUpdateRecord->n_CurrStatus = SIM_STATUS_ELEMFILE;
                    CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
                  }*/
                  if(0)
                  {
                      ;
                  }
                  else
                  {
    				  CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
#ifdef CFW_MULTI_SIM
					  UINT32 result = SimUpdateRecordReq(pUpdateRecord->nFileId,pUpdateRecord->nRecordNum,pUpdateRecord->nMode,
                          pUpdateRecord->nRecordSize,pUpdateRecord->pData,nSimID);
#else
					  UINT32 result = SimUpdateRecordReq(pUpdateRecord->nFileId,pUpdateRecord->nRecordNum,pUpdateRecord->nMode,
                          pUpdateRecord->nRecordSize,pUpdateRecord->pData);
#endif
                      if(ERR_SUCCESS!=result)
                      {
                          CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
                          CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                          hAo = HNULL; 
#ifdef CFW_MULTI_SIM
                          CFW_PostNotifyEvent (EV_CFW_SIM_UPDATE_RECORD_RSP,(UINT32)result,0,nUTI|(nSimID<<24), 0xF0); 
#else
                          CFW_PostNotifyEvent (EV_CFW_SIM_UPDATE_RECORD_RSP,(UINT32)result,0,nUTI, 0xF0); 
#endif
                          CSW_PROFILE_FUNCTION_EXIT(CFW_SimUpdateRecordProc);
                      }
                      pUpdateRecord->n_PrevStatus = pUpdateRecord->n_CurrStatus;
                      pUpdateRecord->n_CurrStatus = SIM_STATUS_UPDATERECORD;                    
                  }

                }
                else
                {
                    CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                    hAo = HNULL;
                }    

            }
            else
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);        
        }
        break;
        
    case SIM_STATUS_ELEMFILE:
        {
            if(pEvent->nEventId == API_SIM_ELEMFILESTATUS_CNF)
            {
                api_SimElemFileStatusCnf_t  * pSimElemFileStatusCnf = (api_SimElemFileStatusCnf_t *)nEvParam;
                if(((0x90==pSimElemFileStatusCnf->Sw1)&&(0x00==pSimElemFileStatusCnf->Sw2))||(0x91==pSimElemFileStatusCnf->Sw1))
                {
    				CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
                    if(((pSimElemFileStatusCnf->Data[8])&0x0F) == 1)
                    {
    					CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
#ifdef CFW_MULTI_SIM
						UINT32 result = SimUpdateRecordReq(pUpdateRecord->nFileId,pUpdateRecord->nRecordNum,pUpdateRecord->nMode,
                            pUpdateRecord->nRecordSize,pUpdateRecord->pData,nSimID);
#else
						UINT32 result = SimUpdateRecordReq(pUpdateRecord->nFileId,pUpdateRecord->nRecordNum,pUpdateRecord->nMode,
                            pUpdateRecord->nRecordSize,pUpdateRecord->pData);
#endif
						if(ERR_SUCCESS!=result)
                        {
                            CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                            hAo = HNULL; 
#ifdef CFW_MULTI_SIM
                            CFW_PostNotifyEvent (EV_CFW_SIM_UPDATE_RECORD_RSP,(UINT32)result,0,nUTI|(nSimID<<24), 0xF0); 
#else
                            CFW_PostNotifyEvent (EV_CFW_SIM_UPDATE_RECORD_RSP,(UINT32)result,0,nUTI, 0xF0); 
#endif
							CSW_PROFILE_FUNCTION_EXIT(CFW_SimUpdateRecordProc);
                        }
                        pUpdateRecord->n_PrevStatus = pUpdateRecord->n_CurrStatus;
                        pUpdateRecord->n_CurrStatus = SIM_STATUS_UPDATERECORD;                    
                    }
                    else if (((pSimElemFileStatusCnf->Data[8])&0x0F)==2)
                    {
    					CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
                        //UINT32 result=SimVerifyCHVReq(pUpdateRecord->nPIN2,CHV2);
                        //Add by lixp at 2007-12-06
                        //Remove for At issue
                        /*
                        if(ERR_SUCCESS!=result)
                        {
                            CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                            hAo = HNULL; 
                            CFW_PostNotifyEvent (EV_CFW_SIM_UPDATE_RECORD_RSP,(UINT32)result,0,nUTI, 0xF0); 
                            CSW_PROFILE_FUNCTION_EXIT(CFW_SimUpdateRecordProc);
                        }*/
                        pUpdateRecord->n_PrevStatus = pUpdateRecord->n_CurrStatus;
                        pUpdateRecord->n_CurrStatus = SIM_STATUS_VERIFY;
                    }
                    else
                    {
    					CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
                      
                        CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                        hAo = HNULL;
                    //    CFW_PostNotifyEvent (EV_CFW_SIM_UPDATE_RECORD_RSP,(UINT32)11,0,nUTI, 0xF0); 
                                            }
                }
                else 
                {    
    				CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
#ifdef CFW_MULTI_SIM
        			UINT32  ErrorCode = Sim_ParseSW1SW2((UINT8) pSimElemFileStatusCnf->Sw1, (UINT8)pSimElemFileStatusCnf->Sw2,nSimID);
#else
					UINT32  ErrorCode = Sim_ParseSW1SW2((UINT8) pSimElemFileStatusCnf->Sw1, (UINT8)pSimElemFileStatusCnf->Sw2);
#endif
					CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
                    CFW_PostNotifyEvent (EV_CFW_SIM_UPDATE_RECORD_RSP,(UINT32)ErrorCode,0,nUTI|(nSimID<<24), 0xF0); 
#else
                    CFW_PostNotifyEvent (EV_CFW_SIM_UPDATE_RECORD_RSP,(UINT32)ErrorCode,0,nUTI, 0xF0); 
#endif
                    CSW_TRACE(33, TSTXT("post EV_CFW_SIM_UPDATE_RECORD_RSP! \n"));
                    CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                    hAo = HNULL;                    
                }    
            }
            else
            {
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);     
            }
        }    
       break;
       
    case SIM_STATUS_VERIFY:
        {
            if(pEvent->nEventId == API_SIM_VERIFYCHV_CNF)
            {
                api_SimVerifyCHVCnf_t  * pSimVerifyCHVCnf=(api_SimVerifyCHVCnf_t *)nEvParam;
                CFW_GetUTI(hAo, &nUTI);
                if((0x90==pSimVerifyCHVCnf->Sw1)&&(0x00==pSimVerifyCHVCnf->Sw2))
                {
    				CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
#ifdef CFW_MULTI_SIM
					UINT32  result = SimUpdateRecordReq(pUpdateRecord->nFileId,pUpdateRecord->nRecordNum,pUpdateRecord->nMode,
                            pUpdateRecord->nRecordSize,pUpdateRecord->pData,nSimID);
#else
					UINT32  result = SimUpdateRecordReq(pUpdateRecord->nFileId,pUpdateRecord->nRecordNum,pUpdateRecord->nMode,
                            pUpdateRecord->nRecordSize,pUpdateRecord->pData);
#endif
					if(ERR_SUCCESS!=result)
                    {
                        CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                        hAo = HNULL;
#ifdef CFW_MULTI_SIM
                        CFW_PostNotifyEvent (EV_CFW_SIM_UPDATE_RECORD_RSP,(UINT32)result,0,nUTI|(nSimID<<24), 0xF0);                        
#else
                  		CFW_PostNotifyEvent (EV_CFW_SIM_UPDATE_RECORD_RSP,(UINT32)result,0,nUTI, 0xF0);                        
#endif
					}    

                    pUpdateRecord->n_PrevStatus = pUpdateRecord->n_CurrStatus;
                    pUpdateRecord->n_CurrStatus = SIM_STATUS_UPDATERECORD;
                }
                else
                {
    				CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
#ifdef CFW_MULTI_SIM
                    UINT32 ErrorCode = Sim_ParseSW1SW2((UINT8)pSimVerifyCHVCnf->Sw1, (UINT8)pSimVerifyCHVCnf->Sw2,nSimID);
					CFW_PostNotifyEvent (EV_CFW_SIM_UPDATE_RECORD_RSP,(UINT32)ErrorCode,0,nUTI|(nSimID<<24), 0xF0); 
#else
                    UINT32 ErrorCode = Sim_ParseSW1SW2((UINT8)pSimVerifyCHVCnf->Sw1, (UINT8)pSimVerifyCHVCnf->Sw2);
					CFW_PostNotifyEvent (EV_CFW_SIM_UPDATE_RECORD_RSP,(UINT32)ErrorCode,0,nUTI, 0xF0); 
#endif
                    CSW_TRACE(33, TSTXT(" post EV_CFW_SIM_UPDATE_RECORD_RSP failure!\n"));
                    CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                    hAo = HNULL;                    
                }
            }
            else
            {
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);     
            }
        }
        break;

       case SIM_STATUS_UPDATERECORD:
        {
            if(pEvent->nEventId == API_SIM_UPDATERECORD_CNF)
            {
                api_SimUpdateRecordCnf_t * pSimUpdateRecordCnf=(api_SimUpdateRecordCnf_t *)nEvParam;
                CFW_GetUTI(hAo, &nUTI);
                if(((0x90==pSimUpdateRecordCnf->Sw1)&&(0x00==pSimUpdateRecordCnf->Sw2))||(0x91==pSimUpdateRecordCnf->Sw1))
                {
    				CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent (EV_CFW_SIM_UPDATE_RECORD_RSP,(UINT32)0, 0, nUTI|(nSimID<<24), 0);
#else
					CFW_PostNotifyEvent (EV_CFW_SIM_UPDATE_RECORD_RSP,(UINT32)0, 0, nUTI, 0);
#endif
					CSW_TRACE(33, TSTXT("  post EV_CFW_SIM_UPDATE_RECORD_RSP Success! \n"));
                }
                else
                {    
    				CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
#ifdef CFW_MULTI_SIM
                    UINT32 ErrorCode = Sim_ParseSW1SW2((UINT8) pSimUpdateRecordCnf->Sw1, (UINT8)pSimUpdateRecordCnf->Sw2,nSimID);
					CFW_PostNotifyEvent (EV_CFW_SIM_UPDATE_RECORD_RSP,(UINT32)ErrorCode,0,nUTI|(nSimID<<24), 0xF0); 
#else
                    UINT32 ErrorCode = Sim_ParseSW1SW2((UINT8) pSimUpdateRecordCnf->Sw1, (UINT8)pSimUpdateRecordCnf->Sw2);
					CFW_PostNotifyEvent (EV_CFW_SIM_UPDATE_RECORD_RSP,(UINT32)ErrorCode,0,nUTI, 0xF0); 
#endif
                    CSW_TRACE(33, TSTXT("  post EV_CFW_SIM_UPDATE_RECORD_RSP failure! \n"));    
                }
                
                CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
                hAo = HNULL;                 
            }
            else
            {
    			CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s \n",__LINE__,__FILE__);
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);     
            }
        }
        break;
            
        default:
            CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
            hAo = HNULL;
        break;   
    }
    
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimUpdateRecordProc);
    return ERR_SUCCESS;
}
#endif
