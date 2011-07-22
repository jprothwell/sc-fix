// //////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2002-2009, Coolsand Technologies, Inc.
// All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmission of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
// 
// FILENAME: cfw_sim_mis_api.c
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
#include <ts.h>
#include <sul.h>
#include "csw_debug.h"

#include "cfw_sim.h"
#include "cfw_sim_mis.h"
#include <csw_mem_prv.h>
#include <base_prv.h>
#include <cos.h>

// 
// This variable is used to store ecc code,if the first byte of one code is 0xFF,
// and after this will be not any code.
UINT8 nECC_Code[5][3];

// The EFECC file contains up to 5 emergency call codes, and three bytes for one call number.

CFW_SIM_LP Sim_Lp = { 0, NULL };

UINT32 CFW_SimGetECC(UINT8* pBuffer, UINT8* nNum)
{
  UINT8 i     = 0;
  UINT8 nSize = 0;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetECC);
  for (i = 0; i < 5; i++)
  {
    if (nECC_Code[i][0] != 0xFF)
      nSize += 3;
    else
      break;
  }

  if (nSize <* nNum)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_SimGetECC *nNum 0x%x nSize 0x%x\n", *nNum, nSize);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetECC);
    return ERR_CFW_INVALID_PARAMETER; // The Buffer is not enough.
  }
  nSize = 0;
  for (i = 0; i < 5; i++)
  {
    if (nECC_Code[i][0] != 0xFF)
    {
      SUL_MemCopy8((pBuffer + (UINT32)nSize), nECC_Code[i], 3);
      nSize += 3;
    }
    else
      break;
  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetECC);
  return ERR_SUCCESS;
}

VOID CFW_SimSetECC(UINT8* pBuffer)
{
  UINT8 i      = 0;
  UINT8 Num    = 0;
  UINT8 Offset = 0;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimSetECC);
  if (pBuffer == NULL)
    nECC_Code[0][0] = 0xFF;
  else
  {
    for (i = 0; i < 5; i++)
    {
      if (*(pBuffer + Offset) == 0xFF)
      {
        SUL_MemCopy8(nECC_Code[Num], (pBuffer + Offset), 3);
        Num++;
      }
      Offset += 3;
    }
    if (Num != 5)
      nECC_Code[Num][0] = 0xFF;
  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetECC);
}

UINT32 CFW_SimGetLP(UINT8* pBuffer, UINT16* nNum)
{
  CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetLP);
  if (Sim_Lp.nLPSize > *nNum)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_SimGetLP *nNum 0x%x nSize 0x%x\n", *nNum, Sim_Lp.nLPSize);

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetLP);
    return ERR_INVALID_PARAMETER;
  }
  SUL_MemCopy8(pBuffer, Sim_Lp.pLP, Sim_Lp.nLPSize);
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetLP);
  return ERR_SUCCESS;
}

UINT32 CFW_SimSetLP(UINT8* pBuffer, UINT16 nNum)
{
  Sim_Lp.pLP     = pBuffer;
  Sim_Lp.nLPSize = nNum;
  return ERR_SUCCESS;
}

BOOL CFW_SimGetPS(VOID)
{
  return 0;
}

BOOL CFW_SimGetPF(VOID)
{
  return 0;
}

UINT32 CFW_SimSetPF(BOOL bValue)
{
  return 1;
}

UINT32 CFW_SimSetPS(BOOL bValue)
{
  return 1;
}
UINT32 CFW_SimErrorSpy(HAO hAo, CFW_EV* pEvent)
{					
       CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
	return ERR_SUCCESS;
}
HAO SimSpySimCard(CFW_EV* pEvent)
{
  HAO hSimAo;

  CSW_PROFILE_FUNCTION_ENTER(SimSpySimCard);
  CSW_TRACE(CFW_SIM_TS_ID, "Error SimSpySimCard\n");
#ifdef CFW_MULTI_SIM

  CFW_SIM_ID nSimID;

  nSimID = pEvent->nFlag;
  //CFW_SendSclMessage(API_SIM_CLOSE_IND, NULL, nSimID);  // added to 
  CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_REMOVED, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
  hSimAo = CFW_RegisterAo(CFW_SIM_SRV_ID, NULL,CFW_SimErrorSpy, nSimID);
#else

  //CFW_SendSclMessage(API_SIM_CLOSE_IND, NULL);  // added to 
  CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_REMOVED, 0, GENERATE_SHELL_UTI(), 0xF0);
  hSimAo = CFW_RegisterAo(CFW_SIM_SRV_ID, NULL, CFW_SimErrorSpy /* (PFN_AO_PORC)CFW_SimSpySimCardProc */ );
#endif

  CSW_PROFILE_FUNCTION_EXIT(SimSpySimCard);
  return ERR_SUCCESS;
}

#ifdef CFW_MULTI_SIM
static BOOL gSimStage3Finish[CFW_SIM_END] = { FALSE, FALSE };
#else
BOOL gSimStage3Finish = FALSE;
#endif
UINT32 CFW_SimInitStage3Proc(HAO hAo, CFW_EV* pEvent);
#if 0
#ifdef CFW_MULTI_SIM
UINT32 CFW_SimInitStage3(CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimInitStage3(VOID)
#endif
{
#ifdef CFW_MULTI_SIM
  if (FALSE == gSimStage3Finish[nSimID])
  {
    gSimStage3Finish[nSimID] = TRUE;
  }
  else
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_SimInitStage3 invoked more than one time\n");
    return ERR_CME_OPERATION_NOT_ALLOWED;
  }
#else
  if (FALSE == gSimStage3Finish)
  {
    gSimStage3Finish = TRUE;
  }
  else
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_SimInitStage3 invoked more than one time\n");

    return ERR_CME_OPERATION_NOT_ALLOWED;
  }
#endif
  HAO hSimInitializeAo = (HAO) 0;

  CFW_SIMINITS3* pSimInitS3Data = NULL;
  CSW_PROFILE_FUNCTION_ENTER(CFW_SimInitStage3);

  pSimInitS3Data = (CFW_SIMINITS3*)CSW_SIM_MALLOC(SIZEOF(CFW_SIMINITS3));
  if (pSimInitS3Data == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3);
    return ERR_NO_MORE_MEMORY;
  }
#ifdef CFW_MULTI_SIM
  hSimInitializeAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimInitS3Data, CFW_SimInitStage3Proc, nSimID);
#else
  hSimInitializeAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimInitS3Data, CFW_SimInitStage3Proc);
#endif
  pSimInitS3Data->nSm_SimInitS3_prevStatus = SM_SIMINITS3_INIT;
  pSimInitS3Data->nSm_SimInitS3_currStatus = SM_SIMINITS3_IDLE;

  CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_CONTINUE);
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3);
  return ERR_SUCCESS;
}
#endif
#if 0
// 同步函数,TRUE需要启动这ProfileDownload过程;FALSE要求SHELL启动Stage3过程。
#ifdef CFW_MULTI_SIM
BOOL CFW_SimInitStage2(CFW_SIM_ID nSimID)
#else
BOOL CFW_SimInitStage2(VOID)
#endif
{

  SIM_MISC_PARAM* pG_Mic_Param = NULL;
  CSW_PROFILE_FUNCTION_ENTER(CFW_SimInitStage2);

#ifdef CFW_MULTI_SIM
  CSW_TRACE(CFW_SIM_TS_ID, "CFW_SimInitStage2 nSimID %d\n", nSimID);

  CFW_CfgSimGetMicParam(&pG_Mic_Param, nSimID);
#else
  CSW_TRACE(CFW_SIM_TS_ID, "CFW_SimInitStage2 \n");

  CFW_CfgSimGetMicParam(&pG_Mic_Param);
#endif
  CSW_TRACE(CFW_SIM_TS_ID, "CFW_SimInitStage2 PHASE %d\n", pG_Mic_Param->nPhase);
  if (pG_Mic_Param->nPhase == SIM_PHASE2Plus)
  {
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage2);
    return TRUE;
  }
  else
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error pG_Mic_Param->nPhase 0x%x \n", pG_Mic_Param->nPhase);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage2);
    return FALSE;
  }
}
#endif
UINT32 CFW_SimInitStage1Proc(HAO hAo, CFW_EV* pEvent);
#if 0
#ifdef CFW_MULTI_SIM
extern CFW_SIM_STATUS gSimStatus[];

UINT32 CFW_SimInit(CFW_SIM_ID nSimID) // UINT32 CFW_SimInitStage1(VOID)
#else
UINT32 CFW_SimInit(VOID)  // UINT32 CFW_SimInitStage1(VOID)
#endif
{
  HAO hSimInitializeAo = (HAO) 0;

  SIM_SAT_PARAM* pG_Sat_Param;
  SIM_CHV_PARAM* pG_Chv_Param;
  SIM_PBK_PARAM* pG_Pbk_Param;
  SIM_MISC_PARAM* pG_Mic_Param;

  CFW_SIMINITS1* pSimInitS1Data = NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimInit);
#ifdef CFW_MULTI_SIM
  gSimStatus[nSimID] = CFW_SIM_STATUS_END;
#endif
  pSimInitS1Data = (CFW_SIMINITS1*)CSW_SIM_MALLOC(SIZEOF(CFW_SIMINITS1));
  if (pSimInitS1Data == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimInit);
    return ERR_NO_MORE_MEMORY;
  }

#ifdef CFW_MULTI_SIM
  CFW_CfgSimGetSatParam(&pG_Sat_Param, nSimID);
  CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
  CFW_CfgSimGetPbkParam(&pG_Pbk_Param, nSimID);
  CFW_CfgSimGetMicParam(&pG_Mic_Param, nSimID);
  hSimInitializeAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimInitS1Data, CFW_SimInitStage1Proc, nSimID);
#else
  CFW_CfgSimGetSatParam(&pG_Sat_Param);
  CFW_CfgSimGetChvParam(&pG_Chv_Param);
  CFW_CfgSimGetPbkParam(&pG_Pbk_Param);
  CFW_CfgSimGetMicParam(&pG_Mic_Param);
  hSimInitializeAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimInitS1Data, CFW_SimInitStage1Proc);
#endif

  // initialize sat parameter
  pG_Sat_Param->nCurCmd     = 0;
  pG_Sat_Param->pSatComData = NULL;
  pG_Sat_Param->pSetupMenu  = NULL;

  // initialize chv parameter
  pG_Chv_Param->bCHV2VerifyReq = FALSE;
  pG_Chv_Param->nFunID         = 0xFF;
  pG_Chv_Param->nCHVAuthen     = 0xFF;
  pG_Chv_Param->nPin1Status    = CHV_UNKNOW;
  pG_Chv_Param->nPin2Status    = CHV_UNKNOW;
  pG_Chv_Param->nPuk1Status    = CHV_UNKNOW;
  pG_Chv_Param->nPuk2Status    = CHV_UNKNOW;

  // initialize pbk parameter
  pG_Pbk_Param->nADNRecordSize    = 0;
  pG_Pbk_Param->nADNTotalNum      = 0;
  pG_Pbk_Param->nECCRecordSize    = 0;
  pG_Pbk_Param->nECCTotalNum      = 0;
  pG_Pbk_Param->nFDNRecordSize    = 0;
  pG_Pbk_Param->nFDNTotalNum      = 0;
  pG_Pbk_Param->nLNDRecordSize    = 0;
  pG_Pbk_Param->nLNDTotalNum      = 0;
  pG_Pbk_Param->nMSISDNRecordSize = 0;
  pG_Pbk_Param->nMSISDNTotalNum   = 0;

  // initialize mic parameter
  pG_Mic_Param->bImageDone   = 0;
  pG_Mic_Param->bStartPeriod = TRUE;
  pG_Mic_Param->nSimMode     = 0;

  pSimInitS1Data->nSm_SimInitS1_prevStatus = SM_SIMINITS1_INIT;
  pSimInitS1Data->nSm_SimInitS1_currStatus = SM_SIMINITS1_IDLE;
  CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_CONTINUE);
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimInit);

  return ERR_SUCCESS;
}

#ifdef CFW_MULTI_SIM
UINT32 CFW_SimReset(UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimReset(UINT16 nUTI)
#endif
{
  HAO hAo = 0x00000000;

  SIM_SM_RESET* pReset = NULL;

#ifdef CFW_MULTI_SIM
  UINT32 ret = ERR_SUCCESS;

  ret = CFW_CheckSimId(nSimID);
  if (ERR_SUCCESS != ret)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_CheckSimId return 0x%x \n", ret);
    return ret;
  }
#endif

  if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");
    return ERR_CFW_UTI_IS_BUSY;

  }

  // 
  // Modify by wangxj at 20060819
  // 
  pReset = (SIM_SM_RESET*)CFW_MALLOC(SIZEOF(SIM_SM_RESET));
  if (pReset == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
    return ERR_NO_MORE_MEMORY;  // should be updated by macro

  }
  SUL_ZeroMemory32(pReset, SIZEOF(SIM_SM_RESET));

#ifdef CFW_MULTI_SIM
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pReset, CFW_SimResetProc, nSimID);
#else
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pReset, CFW_SimResetProc);
#endif

  CFW_SetUTI(hAo, nUTI, 0);
  CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);
  return ERR_SUCCESS;
}
#endif
#ifdef CFW_MULTI_SIM
UINT32 CFW_SimGetProviderId(UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimGetProviderId(UINT16 nUTI)
#endif
{

  HAO hAo = 0x00000000;

  SIM_SM_INFO_GETPROVIDERID* pGetProviderID = NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetProviderId);
  if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");    
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetProviderId);
    return ERR_CFW_UTI_IS_BUSY;
  }
  pGetProviderID = (SIM_SM_INFO_GETPROVIDERID*)CSW_SIM_MALLOC(SIZEOF(SIM_SM_INFO_GETPROVIDERID));
  if (pGetProviderID == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
    
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetProviderId);
    return ERR_NO_MORE_MEMORY;  // should be updated by macro
  }
  SUL_ZeroMemory32(pGetProviderID, SIZEOF(SIM_SM_INFO_GETPROVIDERID));

#ifdef CFW_MULTI_SIM
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pGetProviderID, CFW_SimMisGetProviderIdProc, nSimID);
#else
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pGetProviderID, CFW_SimMisGetProviderIdProc);
#endif
  CFW_SetUTI(hAo, nUTI, 0);
  CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetProviderId);

  return (ERR_SUCCESS);
}

#ifdef CFW_MULTI_SIM
UINT32 CFW_SimSetPrefOperatorList(UINT8* pOperatorList, UINT8 nSize, UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimSetPrefOperatorList(UINT8* pOperatorList, UINT8 nSize, UINT16 nUTI)
#endif
{
  HAO hAo     = 0x00000000;
  UINT8* temp = NULL;

  SIM_SM_INFO_SETPREFOPREATORLIST* pSetPrefLsit = NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimSetPrefOperatorList);

#ifdef CFW_MULTI_SIM
  UINT32 ret = ERR_SUCCESS;

  ret = CFW_CheckSimId(nSimID);
  if (ERR_SUCCESS != ret)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_CheckSimId return 0x%x \n", ret);

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPrefOperatorList);
    return ret;
  }
#endif

  if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");    

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPrefOperatorList);
    return ERR_CFW_UTI_IS_BUSY;
  }
  pSetPrefLsit = (SIM_SM_INFO_SETPREFOPREATORLIST*)CSW_SIM_MALLOC(SIZEOF(SIM_SM_INFO_SETPREFOPREATORLIST));
  if (pSetPrefLsit == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
    
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPrefOperatorList);
    return ERR_NO_MORE_MEMORY;  // should be updated by macro
  }
  SUL_ZeroMemory32(pSetPrefLsit, SIZEOF(SIM_SM_INFO_SETPREFOPREATORLIST));
  temp = (UINT8*)CSW_SIM_MALLOC(nSize / 2);
  if (temp == NULL)
  {
    CSW_Free(pSetPrefLsit);
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");    
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPrefOperatorList);
    return ERR_NO_MORE_MEMORY;  // should be updated by macro
  }

#ifdef CFW_MULTI_SIM
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSetPrefLsit, CFW_SimMisSetPrefListProc, nSimID);
#else
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSetPrefLsit, CFW_SimMisSetPrefListProc);
#endif

  CFW_SetUTI(hAo, nUTI, 0);

  // CSW_TC_MEMBLOCK(CFW_SIM_TS_ID, pOperatorList, nSize, 16);

  pSetPrefLsit->nSize = nSize / 2;
  cfw_BCDtoPLMN(pOperatorList, temp, nSize);
  SUL_MemSet8(pSetPrefLsit->nOperatorList, 0xFF, 255);
  SUL_MemCopy8(pSetPrefLsit->nOperatorList, temp, pSetPrefLsit->nSize);
  CSW_SIM_FREE(temp);
  CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPrefOperatorList);

  return (ERR_SUCCESS);
}

      // ------------------------------------------------------------
      // Add by wulc  2009-10-25 begin
      // ------------------------------------------------------------

#ifdef CFW_MULTI_SIM
UINT32 CFW_SimGetServiceProviderName(UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimGetServiceProviderName(UINT16 nUTI)
#endif
{
  HAO hAo = 0x00000000;

  SIM_SM_INFO_GETSERVERPROVIDERNAME* pGetSPN = NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetServiceProviderName);
  if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");    
    
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetServiceProviderName);
    return ERR_CFW_UTI_IS_BUSY;
  }
  pGetSPN = (SIM_SM_INFO_GETSERVERPROVIDERNAME*)CSW_SIM_MALLOC(SIZEOF(SIM_SM_INFO_GETSERVERPROVIDERNAME));
  if (pGetSPN == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
    
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetServiceProviderName);
    return ERR_NO_MORE_MEMORY;  
  }
  SUL_ZeroMemory32(pGetSPN, SIZEOF(SIM_SM_INFO_GETSERVERPROVIDERNAME));

#ifdef CFW_MULTI_SIM
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pGetSPN, CFW_SimGetServiceProviderNameProc, nSimID);
#else
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pGetSPN, CFW_SimGetServiceProviderNameProc);
#endif
  CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimGetServiceProviderName()\r\n"));
  CFW_SetUTI(hAo, nUTI, 0);
  CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetServiceProviderName);

  return (ERR_SUCCESS);
}
      // ------------------------------------------------------------
      // Add by wulc  2009-10-25 end
      // ------------------------------------------------------------


#ifdef CFW_MULTI_SIM
UINT32 CFW_SimGetPrefOperatorList(UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimGetPrefOperatorList(UINT16 nUTI)
#endif
{
  HAO hAo = 0x00000000;

  SIM_SM_INFO_GETPREFOPREATORLIST* pGetPrefList = NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetPrefOperatorList);
  if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");    
    
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPrefOperatorList);
    return ERR_CFW_UTI_IS_BUSY;
  }
  pGetPrefList = (SIM_SM_INFO_GETPREFOPREATORLIST*)CSW_SIM_MALLOC(SIZEOF(SIM_SM_INFO_GETPREFOPREATORLIST));
  if (pGetPrefList == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
    
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPrefOperatorList);
    return ERR_NO_MORE_MEMORY;  // should be updated by macro
  }
  SUL_ZeroMemory32(pGetPrefList, SIZEOF(SIM_SM_INFO_GETPREFOPREATORLIST));

#ifdef CFW_MULTI_SIM
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pGetPrefList, CFW_SimMisGetPrefListProc, nSimID);
#else
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pGetPrefList, CFW_SimMisGetPrefListProc);
#endif
  CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimGetPrefOperatorList()\r\n"));
  CFW_SetUTI(hAo, nUTI, 0);
  CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPrefOperatorList);

  return (ERR_SUCCESS);
}

#ifdef CFW_MULTI_SIM
UINT32 CFW_SimGetPrefOperatorListMaxNum(UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimGetPrefOperatorListMaxNum(UINT16 nUTI)
#endif
{
  HAO hAo = 0x00000000;

  SIM_SM_INFO_GETPREFOPREATORLISTMAXNUM* pGetPrefListMaxNum = NULL;
  CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetPrefOperatorListMaxNum);

#ifdef CFW_MULTI_SIM
  UINT32 ret = ERR_SUCCESS;

  ret = CFW_CheckSimId(nSimID);
  if (ERR_SUCCESS != ret)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_CheckSimId return 0x%x \n", ret);

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPrefOperatorListMaxNum);
    return ret;
  }
#endif

  if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");    
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPrefOperatorListMaxNum);
    return ERR_CFW_UTI_IS_BUSY;
  }
  pGetPrefListMaxNum =
    (SIM_SM_INFO_GETPREFOPREATORLISTMAXNUM*)CSW_SIM_MALLOC(SIZEOF(SIM_SM_INFO_GETPREFOPREATORLISTMAXNUM));
  if (pGetPrefListMaxNum == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
    
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPrefOperatorListMaxNum);
    return ERR_NO_MORE_MEMORY;  // should be updated by macro
  }
  SUL_ZeroMemory32(pGetPrefListMaxNum, SIZEOF(SIM_SM_INFO_GETPREFOPREATORLISTMAXNUM));

#ifdef CFW_MULTI_SIM
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pGetPrefListMaxNum, CFW_SimMisGetPrefListMAXNumProc, nSimID);
#else
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pGetPrefListMaxNum, CFW_SimMisGetPrefListMAXNumProc);
#endif

  CFW_SetUTI(hAo, nUTI, 0);
  CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPrefOperatorListMaxNum);

  return (ERR_SUCCESS);
}

// =============================================================================
// ACM Reference 
// 
// =============================================================================

#ifndef SIM_ARG_CHK_EN
#  define SIM_ARG_CHK_EN (1 == 1)
#endif

#ifdef CFW_MULTI_SIM
UINT32 CFW_SimSetACMMax(UINT32 iACMMaxValue, UINT8* pPin2, UINT8 nPinSize, UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimSetACMMax(UINT32 iACMMaxValue, UINT8* pPin2, UINT8 nPinSize, UINT16 nUTI)
#endif
{
  HAO hAo;

  ACM_INFO_SETACMMAX* pSetAcmMax = NULL;
  CSW_PROFILE_FUNCTION_ENTER(CFW_SimSetACMMax);

#ifdef CFW_MULTI_SIM
  UINT32 ret = ERR_SUCCESS;

  ret = CFW_CheckSimId(nSimID);
  if (ERR_SUCCESS != ret)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_CheckSimId return 0x%x\n", ret);

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACMMax);
    return ret;
  }
#endif
  // 
  // Parameter validity check
  // 
#if SIM_ARG_CHK_EN > 0
  if (iACMMaxValue > 0x1000000)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error iACMMaxValue 0x%x\n", iACMMaxValue);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACMMax);
    return ERR_CFW_INVALID_PARAMETER;
  }

  if (pPin2 == (UINT8*)NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error pPin2 NULL\n");
    
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACMMax);
    return ERR_CFW_INVALID_PARAMETER;
  }

  if ((nPinSize < 4) || (nPinSize > 8))
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error nPinSize 0x%x\n", nPinSize);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACMMax);
    return ERR_CFW_INVALID_PARAMETER;
  }
#if 0
  if ((nUTI >= 255) || (nUTI <= 0))
  {
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACMMax);
    return ERR_CFW_INVALID_PARAMETER;
  }
#endif
  if (ERR_SUCCESS != CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID))
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");    
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACMMax);
    return ERR_CFW_NOT_EXIST_FREE_UTI;
  }

#endif
  pSetAcmMax = (ACM_INFO_SETACMMAX*)CSW_SIM_MALLOC(SIZEOF(ACM_INFO_SETACMMAX));

#if SIM_ARG_CHK_EN > 0
  if (pSetAcmMax == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
    
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACMMax);
    return ERR_NO_MORE_MEMORY;
  }
#endif

#ifdef CFW_MULTI_SIM
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSetAcmMax, CFW_SimSetACMMaxProc, nSimID);
#else
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSetAcmMax, CFW_SimSetACMMaxProc);
#endif
  CFW_SetUTI(hAo, nUTI, 1);

  SUL_ZeroMemory32(pSetAcmMax, SIZEOF(ACM_INFO_SETACMMAX));

  CFW_SetServiceId(CFW_SIM_SRV_ID);

  pSetAcmMax->nPinSize     = nPinSize;
  pSetAcmMax->iACMMaxValue = iACMMaxValue;
  SUL_MemSet8(pSetAcmMax->pPin2, 0xff, 8);
  SUL_MemCopy8(pSetAcmMax->pPin2, pPin2, nPinSize);

  pSetAcmMax->n_CurrStatus = CFW_SIM_MIS_IDLE;
  pSetAcmMax->n_PrevStatus = CFW_SIM_MIS_IDLE;
  pSetAcmMax->nTryCount    = 0x01;

  CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACMMax);
  return (ERR_SUCCESS);
}

#ifdef CFW_MULTI_SIM
UINT32 CFW_SimGetACMMax(UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimGetACMMax(UINT16 nUTI)
#endif
{
  HAO hAo;

  ACM_INFO_GETACMMAX* pGetAcmMax = NULL;
  CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetACMMax);
#ifdef CFW_MULTI_SIM
  UINT32 ret = ERR_SUCCESS;

  ret = CFW_CheckSimId(nSimID);
  if (ERR_SUCCESS != ret)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_CheckSimId return 0x%x \n", ret);

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetACMMax);
    return ret;
  }
#endif
  // 
  // Parameter validity check
  // 
#if SIM_ARG_CHK_EN > 0

  if (ERR_SUCCESS != CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID))
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");    
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetACMMax);
    return ERR_CFW_NOT_EXIST_FREE_UTI;
  }
#endif
  pGetAcmMax = (ACM_INFO_GETACMMAX*)CSW_SIM_MALLOC(SIZEOF(ACM_INFO_GETACMMAX));
#if SIM_ARG_CHK_EN > 0
  if (pGetAcmMax == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetACMMax);
    return ERR_NO_MORE_MEMORY;
  }
#endif

#ifdef CFW_MULTI_SIM
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pGetAcmMax, CFW_SimGetACMMaxProc, nSimID);
#else
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pGetAcmMax, CFW_SimGetACMMaxProc);
#endif
  CFW_SetUTI(hAo, nUTI, 1);

  SUL_ZeroMemory32(pGetAcmMax, SIZEOF(ACM_INFO_GETACMMAX));

  CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimGetACMMax()\r\n"));
  CFW_SetServiceId(CFW_SIM_SRV_ID);

  pGetAcmMax->n_CurrStatus = CFW_SIM_MIS_IDLE;
  pGetAcmMax->n_PrevStatus = CFW_SIM_MIS_IDLE;
  pGetAcmMax->nTryCount    = 0x01;
  CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetACMMax);
  return (ERR_SUCCESS);
}

#ifdef CFW_MULTI_SIM
UINT32 CFW_SimGetACM(UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimGetACM(UINT16 nUTI)
#endif
{
  HAO hAo = 0x00000000;

  SIM_SM_INFO_GETACM* pGetACM = NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetACM);
#ifdef CFW_MULTI_SIM
  UINT32 ret = ERR_SUCCESS;

  ret = CFW_CheckSimId(nSimID);
  if (ERR_SUCCESS != ret)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_CheckSimId return 0x%x \n", ret);

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetACM);
    return ret;
  }
#endif

  if (ERR_SUCCESS != CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID))
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetACM);
    return ERR_CFW_NOT_EXIST_FREE_UTI;
  }
  pGetACM = (SIM_SM_INFO_GETACM*)CSW_SIM_MALLOC(SIZEOF(SIM_SM_INFO_GETACM));
  if (pGetACM == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
    
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetACM);
    return ERR_NO_MORE_MEMORY;  // should be updated by macro
  }

#ifdef CFW_MULTI_SIM
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pGetACM, CFW_SimMisGetACMProc, nSimID);
#else
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pGetACM, CFW_SimMisGetACMProc);
#endif
  SUL_ZeroMemory32(pGetACM, SIZEOF(SIM_SM_INFO_GETACM));

  CFW_SetUTI(hAo, nUTI, 0);
  CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);

  CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetACM);

  return (ERR_SUCCESS);
}

#ifdef CFW_MULTI_SIM
UINT32 CFW_SimSetACM(UINT32 iCurValue, UINT8* pPIN2, UINT8 nPinSize, UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimSetACM(UINT32 iCurValue, UINT8* pPIN2, UINT8 nPinSize, UINT16 nUTI)
#endif
{
  HAO hAo = 0x00000000;

  SIM_SM_INFO_SETACM* pSetACM = NULL;

#ifdef CFW_MULTI_SIM
  UINT32 ret = ERR_SUCCESS;

  ret = CFW_CheckSimId(nSimID);
  if (ERR_SUCCESS != ret)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_CheckSimId return 0x%x\n", ret);

    return ret;
  }
#endif
// CSW_PROFILE_FUNCTION_ENTER(CFW_SimSetACM);

  if ((nPinSize > 8) || (iCurValue > 0x00FFFFFF))
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error nPinSize 0x%x,iCurValue 0x%x\n", nPinSize,iCurValue);
    
    // CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACM);
    return ERR_CFW_INVALID_PARAMETER;
  }

  if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
  {
// CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACM);
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");
    return ERR_CFW_NOT_EXIST_FREE_UTI;
  }
  pSetACM = (SIM_SM_INFO_SETACM*)CSW_SIM_MALLOC(SIZEOF(SIM_SM_INFO_SETACM));

  if (pSetACM == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
// CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACM);
    return ERR_NO_MORE_MEMORY;  // should be updated by macro
  }

#ifdef CFW_MULTI_SIM
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSetACM, CFW_SimMisSetACMProc, nSimID);
#else
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSetACM, CFW_SimMisSetACMProc);
#endif

  SUL_ZeroMemory32(pSetACM, SIZEOF(SIM_SM_INFO_SETACM));

  SUL_MemSet8(pSetACM->nPIN2, 0xff, 8);
  SUL_MemCopy8(pSetACM->nPIN2, pPIN2, nPinSize);
  pSetACM->nPIN2Size = nPinSize;

  pSetACM->iACMValue = iCurValue;

  CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimSetACM()\r\n"));

  CFW_SetUTI(hAo, nUTI, 0);
  CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);

// CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetACM);
  return (ERR_SUCCESS);
}

#ifdef CFW_MULTI_SIM
UINT32 CFW_SimGetPUCT(UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimGetPUCT(UINT16 nUTI)
#endif
{
  CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimGetPUCT()\r\n"));
  HAO hSimGetPUCTAo = 0;

  CFW_SIMGETPUCT* pSimGetPUCTData = NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetPUCT);
#ifdef CFW_MULTI_SIM
  UINT32 ret = ERR_SUCCESS;

  ret = CFW_CheckSimId(nSimID);
  if (ERR_SUCCESS != ret)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_CheckSimId return 0x%x \n", ret);

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPUCT);
    return ret;
  }
#endif

  if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPUCT);
    return ERR_CFW_NOT_EXIST_FREE_UTI;  // 替换掉了ERR_CFW_UTI_EXIST
  }

  pSimGetPUCTData = (CFW_SIMGETPUCT*)CSW_SIM_MALLOC(SIZEOF(CFW_SIMGETPUCT));
  if (pSimGetPUCTData == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPUCT);
    return ERR_NO_MORE_MEMORY;
  }

#ifdef CFW_MULTI_SIM
  hSimGetPUCTAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimGetPUCTData, CFW_SimGetPUCTProc, nSimID);
#else
  hSimGetPUCTAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimGetPUCTData, CFW_SimGetPUCTProc);
#endif

  CFW_SetUTI(hSimGetPUCTAo, nUTI, 0);
  pSimGetPUCTData->nSm_GetPUCT_prevStatus = CFW_SIM_MIS_IDLE;
  pSimGetPUCTData->nSm_GetPUCT_currStatus = CFW_SIM_MIS_IDLE;
  CFW_SetAoProcCode(hSimGetPUCTAo, CFW_AO_PROC_CODE_CONTINUE);
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPUCT);

  CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimGetPUCT() return ERR_SUCCESS\r\n"));
  return (ERR_SUCCESS);
}

#ifdef CFW_MULTI_SIM
UINT32 CFW_SimSetPUCT(CFW_SIM_PUCT_INFO* pPUCT, UINT8* pPin2, UINT8 nPinSize, UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimSetPUCT(CFW_SIM_PUCT_INFO* pPUCT, UINT8* pPin2, UINT8 nPinSize, UINT16 nUTI)
#endif
{
  CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimSetPUCT() begin\r\n"));
  UINT8 i           = 0;  // 用于for循环
  UINT16 iEPPU      = pPUCT->iEPPU;
  INT8 iEX          = pPUCT->iEX;  // iEX的值不大于7.
  HAO hSimSetPUCTAo = 0;

  CFW_SIMSETPUCT* pSimSetPUCTData = NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimSetPUCT);
#ifdef CFW_MULTI_SIM
  UINT32 ret = ERR_SUCCESS;

  ret = CFW_CheckSimId(nSimID);
  if (ERR_SUCCESS != ret)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_CheckSimId return 0x%x \n", ret);

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPUCT);
    return ret;
  }
#endif

  if ((iEPPU > 0xFFF) || (nPinSize < 4) || (nPinSize > 8) || (pPin2 == NULL)) // iEPPU占用12 bit
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error iEPPU 0x%x,nPinSize 0x%x,nPinSize 0x%x,pPin2 0x%x\n",iEPPU,nPinSize,nPinSize,pPin2);    
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPUCT);
    return ERR_CFW_INVALID_PARAMETER;
  }

  if (pPUCT->iSign == 1)  // EX为负数
  {
    iEX = -iEX;
  }

  if ((iEX < -7) || (iEX > 7))  // iEX占用三个bit，绝对值不应超过7
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error iEX 0x%x \n", iEX);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPUCT);
    return ERR_CFW_INVALID_PARAMETER;
  }

  if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPUCT);
    return ERR_CFW_NOT_EXIST_FREE_UTI;
  }
  pSimSetPUCTData = (CFW_SIMSETPUCT*)CSW_SIM_MALLOC(SIZEOF(CFW_SIMSETPUCT));
  if (pSimSetPUCTData == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPUCT);
    return ERR_NO_MORE_MEMORY;
  }

#ifdef CFW_MULTI_SIM
  hSimSetPUCTAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimSetPUCTData, CFW_SimSetPUCTProc, nSimID);
#else
  hSimSetPUCTAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimSetPUCTData, CFW_SimSetPUCTProc);
#endif
  CFW_SetUTI(hSimSetPUCTAo, nUTI, 0);

  pSimSetPUCTData->iCurrency[0] = pPUCT->iCurrency[0];
  pSimSetPUCTData->iCurrency[1] = pPUCT->iCurrency[1];
  pSimSetPUCTData->iCurrency[2] = pPUCT->iCurrency[2];

  pSimSetPUCTData->iEPPU = iEPPU;
  pSimSetPUCTData->iEX   = iEX;

  SUL_MemCopy8(pSimSetPUCTData->pPin2, pPin2, nPinSize);

  for (i = nPinSize; i < 8; i++)  // 将pPin2剩余部分置为0xFF
  {
    pSimSetPUCTData->pPin2[i] = 0xFF;
  }
  pSimSetPUCTData->nSm_SetPUCT_prevStatus = CFW_SIM_MIS_IDLE;
  pSimSetPUCTData->nSm_SetPUCT_currStatus = CFW_SIM_MIS_IDLE;

  CFW_SetAoProcCode(hSimSetPUCTAo, CFW_AO_PROC_CODE_CONTINUE);

  CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPUCT);

  CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimSetPUCT() return ERR_SUCCESS\r\n"));
  return (ERR_SUCCESS);
}


#ifdef CFW_EXTENDED_API

UINT32 cfw_SimReadElementaryFile(   UINT16 nUTI,
                                    UINT32 fileId
                                    #ifdef CFW_MULTI_SIM
                                    , CFW_SIM_ID nSimID
                                    #endif
                                    )
{
    HAO hAo;
    SIM_SM_INFO_EF *pUserData;
        
    if (ERR_SUCCESS != CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID))
    {
        return ERR_CFW_NOT_EXIST_FREE_UTI;
    }
    
    if (!(pUserData = (SIM_SM_INFO_EF *)CSW_SIM_MALLOC(SIZEOF(SIM_SM_INFO_EF))))
    {
        return ERR_NO_MORE_MEMORY;  
    }

    pUserData->nState.nNextState    = CFW_SIM_MIS_IDLE;
    pUserData->nState.nCurrState    = CFW_SIM_MIS_IDLE;
    pUserData->nTryCount            = 1;
    pUserData->fileSize             = 0;
    pUserData->fileId               = fileId;
    pUserData->update               = FALSE;

    hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pUserData, cfw_SimReadUpdateElementaryFileProc
                                    #ifdef CFW_MULTI_SIM
                                    ,nSimID
                                    #endif
                                    );
    
    CFW_SetUTI(hAo, nUTI, 0);
    CFW_SetAoProcCode(hAo,CFW_AO_PROC_CODE_CONTINUE);  
    
    return ERR_SUCCESS;
}

/*
================================================================================
  Function   : CFW_SimSetPrefOperatorListEX
--------------------------------------------------------------------------------

  Scope      : Set the PLMNs in SIM 
  Parameters :
  Return     :
================================================================================
*/
UINT32 CFW_SimSetPrefOperatorListEX(
                                              UINT8* pOperatorList,
                                              UINT8  nSize,
                                              UINT16 nUTI
#ifdef CFW_MULTI_SIM
                                              ,CFW_SIM_ID nSimID
#endif
)
{
    HAO hAo      = 0x00;
    UINT8 * temp = NULL;
    
    SIM_SM_INFO_SETPREFOPREATORLIST *pSetPrefLsit = NULL;

    CSW_PROFILE_FUNCTION_ENTER( CFW_SimSetPrefOperatorListEX );
#ifdef CFW_MULTI_SIM
	if( nSimID >=CFW_SIM_COUNT)
	{
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("ERROR ! SIM id error CFW_SimSetPrefOperatorListEX()\r\n"));	

		CSW_PROFILE_FUNCTION_EXIT( CFW_SimSetPrefOperatorList );
		return ERR_CFW_INVALID_PARAMETER;
	}
#endif
    if ( ERR_SUCCESS != CFW_IsFreeUTI( nUTI, CFW_SIM_SRV_ID ))
    {
        CSW_PROFILE_FUNCTION_EXIT( CFW_SimSetPrefOperatorList );
        return ERR_CFW_UTI_IS_BUSY;
    }
    pSetPrefLsit = ( SIM_SM_INFO_SETPREFOPREATORLIST * )CSW_SIM_MALLOC(SIZEOF(SIM_SM_INFO_SETPREFOPREATORLIST));
    if (pSetPrefLsit == NULL)
    {
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPrefOperatorList);
        return ERR_NO_MORE_MEMORY;  // should be updated by macro
    }
    SUL_ZeroMemory32(pSetPrefLsit, SIZEOF(SIM_SM_INFO_SETPREFOPREATORLIST));
    temp=( UINT8 * )CSW_SIM_MALLOC( nSize / 2 );
    if (temp == NULL)
    {
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPrefOperatorList);
        return ERR_NO_MORE_MEMORY;  // should be updated by macro
    }
#ifdef CFW_MULTI_SIM
    hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSetPrefLsit, CFW_SimMisSetPrefListProcEX,nSimID);    
#else
    hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSetPrefLsit, CFW_SimMisSetPrefListProcEX);    
#endif
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimSetPrefOperatorList()\r\n"));

    CFW_SetUTI(hAo, nUTI, 0);
    pSetPrefLsit->nSize = nSize/2;

    cfw_BCDtoPLMN(pOperatorList,temp, nSize);
	SUL_MemSet8(pSetPrefLsit->nOperatorList, 0xFF, 255);
    SUL_MemCopy8(pSetPrefLsit->nOperatorList, temp, pSetPrefLsit->nSize);
    CSW_SIM_FREE(temp);
    CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);                        
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetPrefOperatorList);
    
    return(ERR_SUCCESS);
}


/*
================================================================================
  Function   : CFW_SimGetPrefOperatorListEX
--------------------------------------------------------------------------------

  Scope      : Get the PLMNs in SIM 
  Parameters :
  Return     :
================================================================================
*/
UINT32 CFW_SimGetPrefOperatorListEX(
                                              UINT16 nUTI
#ifdef CFW_MULTI_SIM
											  ,CFW_SIM_ID nSimID
#endif
)
{
    HAO hAo = 0x00;
    
    SIM_SM_INFO_GETPREFOPREATORLIST *pGetPrefList = NULL;

    CSW_PROFILE_FUNCTION_ENTER( CFW_SimGetPrefOperatorListEX );
#ifdef CFW_MULTI_SIM
	if( nSimID >=CFW_SIM_COUNT)
	{
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("ERROR ! SIM id error CFW_SimGetPrefOperatorListEX()\r\n"));	

		CSW_PROFILE_FUNCTION_EXIT( CFW_SimGetPrefOperatorListEX );
		return ERR_CFW_INVALID_PARAMETER;
	}
#endif

    if (CFW_IsFreeUTI( nUTI, CFW_SIM_SRV_ID ) != ERR_SUCCESS)
    {
        CSW_PROFILE_FUNCTION_EXIT( CFW_SimGetPrefOperatorListEX );
        return ERR_CFW_UTI_IS_BUSY;
    }
    pGetPrefList = ( SIM_SM_INFO_GETPREFOPREATORLIST* ) CSW_SIM_MALLOC( 
                    SIZEOF( SIM_SM_INFO_GETPREFOPREATORLIST ));
    if ( NULL == pGetPrefList )
    {
        CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimGetPrefOperatorListEX()malloc failed\r\n"));
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPrefOperatorList);
        return( ERR_NO_MORE_MEMORY );  // should be updated by macro
    }
    
    SUL_ZeroMemory32( pGetPrefList, SIZEOF(SIM_SM_INFO_GETPREFOPREATORLIST ));
#ifdef CFW_MULTI_SIM
    hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pGetPrefList, CFW_SimMisGetPrefListProcEX,nSimID);  
#else
    hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pGetPrefList, CFW_SimMisGetPrefListProcEX);    
#endif
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimGetPrefOperatorListEX()\r\n"));
    
    CFW_SetUTI( hAo, nUTI, 0 );
    CFW_SetAoProcCode( hAo, CFW_AO_PROC_CODE_CONTINUE ); 
    
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPrefOperatorListEX);
    
    return(ERR_SUCCESS);
}



/*
================================================================================
  Function   : CFW_SimGetFileStatus
--------------------------------------------------------------------------------

  Scope      : Get the Sim File Status 
  Parameters :
  Return     :
================================================================================
*/
UINT32 CFW_SimGetFileStatus(
                        UINT8 nFileID,
                        UINT16 nUTI
#ifdef CFW_MULTI_SIM
						,CFW_SIM_ID nSimID
#endif
                        )
{
    HAO hAo;
    GET_FILE_STATUS_INFO *pGetFileStatus = NULL;
    CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetFileStatus);
    //
    //Parameter validity check
    //
#ifdef CFW_MULTI_SIM
	if( nSimID >=CFW_SIM_COUNT)
	{
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("ERROR ! SIM id error CFW_SimGetFileStatus()\r\n"));	

		CSW_PROFILE_FUNCTION_EXIT( CFW_SimGetFileStatus );
		return ERR_CFW_INVALID_PARAMETER;
	}
#endif
#if SIM_ARG_CHK_EN > 0
    if((nUTI >= 255) || (nUTI <= 0))
    {
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetFileStatus);
        return ERR_CFW_INVALID_PARAMETER;
    }
    
    if (ERR_SUCCESS != CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID))
    {
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetFileStatus);
        return ERR_CFW_NOT_EXIST_FREE_UTI;
    }
#endif
    pGetFileStatus = (GET_FILE_STATUS_INFO*)CSW_SIM_MALLOC(SIZEOF(GET_FILE_STATUS_INFO));
#if SIM_ARG_CHK_EN > 0
    if(pGetFileStatus == NULL)
    {
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetFileStatus);
        return ERR_NO_MORE_MEMORY;
    }
#endif
#ifdef CFW_MULTI_SIM
    hAo=CFW_RegisterAo(CFW_SIM_SRV_ID, pGetFileStatus, CFW_SimGetFileStatusProc,nSimID);
#else
    hAo=CFW_RegisterAo(CFW_SIM_SRV_ID, pGetFileStatus, CFW_SimGetFileStatusProc);
	CFW_SetUTI(hAo, nUTI, 1);
#endif
    
    SUL_ZeroMemory32(pGetFileStatus, SIZEOF(GET_FILE_STATUS_INFO));
    
    CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimGetFileStatus()\r\n"));
    CFW_SetServiceId(CFW_SIM_SRV_ID);
    

    pGetFileStatus->n_CurrStatus = CFW_SIM_MIS_IDLE;
    pGetFileStatus->n_PrevStatus = CFW_SIM_MIS_IDLE;
    pGetFileStatus->nFileID      = nFileID;
    pGetFileStatus->nTryCount    = 0x01;
    CFW_SetAoProcCode(hAo,CFW_AO_PROC_CODE_CONTINUE);   
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetFileStatus);
    return(ERR_SUCCESS);  
}



/*
================================================================================
  Function   : CFW_SimReadRecord
--------------------------------------------------------------------------------

  Scope      : 
  Parameters :
  Return     :
================================================================================
*/
UINT32 CFW_SimReadRecord (
                           UINT8 nFileID,
                           UINT8 nRecordNum,
                           UINT16 nUTI
#ifdef CFW_MULTI_SIM
						   ,CFW_SIM_ID nSimID
#endif
                           )
{
    HAO hAo;
    CSW_PROFILE_FUNCTION_ENTER(CFW_SimReadRecord);    
    SIM_INFO_READRECORD *pReadRecord = NULL;
#ifdef CFW_MULTI_SIM
	if( nSimID >=CFW_SIM_COUNT)
	{
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("ERROR ! SIM id error CFW_SimReadRecord()\r\n"));	
		CSW_PROFILE_FUNCTION_EXIT( CFW_SimReadRecord );
		return ERR_CFW_INVALID_PARAMETER;
	}
#endif

    if((nFileID >= 15 && nFileID != 22 && nFileID != 43 && nFileID != 55)  )
    {
    	CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadRecord);
        return ERR_INVALID_PARAMETER;
    }
    
    pReadRecord = (SIM_INFO_READRECORD*)CSW_SIM_MALLOC(SIZEOF(SIM_INFO_READRECORD));
    if(pReadRecord == NULL)
    {
    	CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadMessage);
        return ERR_NO_MORE_MEMORY;
    }

    if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
    {
    	CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadRecord);
        return ERR_CFW_NOT_EXIST_FREE_UTI;
    }
        
    SUL_ZeroMemory32(pReadRecord, SIZEOF(SIM_INFO_READRECORD));
#ifdef CFW_MULTI_SIM
	hAo=CFW_RegisterAo(CFW_SIM_SRV_ID, pReadRecord, CFW_SimReadRecordProc,nSimID);
#else
    hAo=CFW_RegisterAo(CFW_SIM_SRV_ID, pReadRecord, CFW_SimReadRecordProc);
#endif
	CFW_SetUTI(hAo, nUTI, 1);
    
    CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimReadRecord()\r\n"));
    CFW_SetServiceId(CFW_SIM_SRV_ID);
    
    pReadRecord->nFileId = nFileID;      
    pReadRecord->n_CurrStatus = SIM_STATUS_IDLE;     
    pReadRecord->n_PrevStatus = SIM_STATUS_IDLE;    
    pReadRecord->nTryCount = 0x01;                     
    pReadRecord->nMode = SIM_MODE_ABSOLUTE;   
    pReadRecord->nRecordNum = nRecordNum;
    
    CFW_SetAoProcCode(hAo,CFW_AO_PROC_CODE_CONTINUE);   
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimReadRecord);
    return(ERR_SUCCESS);  
}



/*
================================================================================
  Function   : CFW_SimUpdateRecord
--------------------------------------------------------------------------------

  Scope      : 
  Parameters :
  Return     :
================================================================================
*/
UINT32 CFW_SimUpdateRecord (
                             UINT8    nFileID,
                             UINT8    nRecordNum,
                             UINT8    nRecordSize,
                             //UINT8  * pPIN2,
                             //UINT8    nPinSize,//Remove by lixp for AT issue
                             UINT8  * pData,
                             UINT16   nUTI
#ifdef CFW_MULTI_SIM
						     ,CFW_SIM_ID nSimID
#endif
                             )
{
    HAO    hAo;    
    SIM_INFO_UPDATERECORD *pUpdateRecord= NULL;   
    CSW_PROFILE_FUNCTION_ENTER(CFW_SimUpdateRecord);   
#ifdef CFW_MULTI_SIM
	if( nSimID >=CFW_SIM_COUNT)
	{
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("ERROR ! SIM id error CFW_SimUpdateRecord()\r\n"));	
		CSW_PROFILE_FUNCTION_EXIT( CFW_SimUpdateRecord );
		return ERR_CFW_INVALID_PARAMETER;
	}
#endif
    if((nFileID >= 15 && nFileID != 22 && nFileID != 43 && nFileID != 55))
    {
    	CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimUpdateRecord);
        return ERR_INVALID_PARAMETER;
    }
    if((nFileID == 7) || (nFileID == 22)) 
    {
    	CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimUpdateRecord);
        return ERR_INVALID_PARAMETER;        
    }

     if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
    {
    	CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimUpdateRecord);
        return ERR_CFW_NOT_EXIST_FREE_UTI;
    }
    
    pUpdateRecord = (SIM_INFO_UPDATERECORD *)CSW_SIM_MALLOC(SIZEOF(SIM_INFO_UPDATERECORD));
    if(pUpdateRecord == NULL)
    {
    	CSW_TRACE(CFW_SIM_TS_ID, "The line is %d,the file is %s\n",__LINE__,__FILE__);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimUpdateRecord);
        return ERR_NO_MORE_MEMORY;
    }

    SUL_ZeroMemory32(pUpdateRecord, SIZEOF(SIM_INFO_UPDATERECORD));  
#ifdef CFW_MULTI_SIM
	hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pUpdateRecord, CFW_SimUpdateRecordProc,nSimID);
#else
    hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pUpdateRecord, CFW_SimUpdateRecordProc);
#endif
	CFW_SetUTI(hAo, nUTI, 1);
    
    CFW_SetServiceId(CFW_SIM_SRV_ID);
    CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimUpdateRecord()\r\n")); 
    
    pUpdateRecord->nRecordNum   = nRecordNum;
    pUpdateRecord->nRecordSize = nRecordSize;
    pUpdateRecord->nFileId = nFileID;  
    pUpdateRecord->nMode = SIM_MODE_ABSOLUTE;
    
    //Add by lixp 2007-12-06
    //Remove for AT issue
    //pUpdateRecord->nPIN2Size = nPinSize;
    //SUL_MemSet8(pUpdateRecord->nPIN2, 0xff, 8);	
    //SUL_MemCopy8(pUpdateRecord->nPIN2,pPIN2,nPinSize);    
    //SUL_MemCopy8(pUpdateRecord->pData,pData,nRecordSize);    
    
    pUpdateRecord->n_CurrStatus = SIM_STATUS_IDLE;
    pUpdateRecord->n_PrevStatus = SIM_STATUS_IDLE;
    pUpdateRecord->nTryCount    = 0x01;  
    
    CFW_SetAoProcCode(hAo,CFW_AO_PROC_CODE_CONTINUE);  
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimUpdateRecord);
    return ERR_SUCCESS;
}
#endif


