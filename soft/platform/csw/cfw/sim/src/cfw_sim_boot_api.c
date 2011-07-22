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
#if 0

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
#endif
#ifdef CFW_MULTI_SIM
static BOOL gSimStage3Finish[CFW_SIM_END] = { FALSE, FALSE };
#else
BOOL gSimStage3Finish = FALSE;
#endif
UINT32 CFW_SimInitStage3Proc(HAO hAo, CFW_EV* pEvent);

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
  if (( SIM_PHASE2Plus == pG_Mic_Param->nPhase ) || ( SIM_PHASE2 == pG_Mic_Param->nPhase ))
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

UINT32 CFW_SimInitStage1Proc(HAO hAo, CFW_EV* pEvent);

#ifdef CFW_MULTI_SIM
extern CFW_SIM_STATUS gSimStatus[];

UINT32 CFW_SimInitStage1(CFW_SIM_ID nSimID) // UINT32 CFW_SimInitStage1(VOID)
#else
UINT32 CFW_SimInitStage1(VOID)  // UINT32 CFW_SimInitStage1(VOID)
#endif
{
  HAO hSimInitializeAo = (HAO) 0;


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
  hSimInitializeAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimInitS1Data, CFW_SimInitStage1Proc, nSimID);
#else
  hSimInitializeAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimInitS1Data, CFW_SimInitStage1Proc);
#endif


  pSimInitS1Data->nSm_SimInitS1_prevStatus = SM_SIMINITS1_INIT;
  pSimInitS1Data->nSm_SimInitS1_currStatus = SM_SIMINITS1_IDLE;
  CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_CONTINUE);
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimInit);

  return ERR_SUCCESS;
}
UINT32 CFW_SimInitStageBootProc(HAO hAo, CFW_EV* pEvent);

#ifdef CFW_MULTI_SIM
extern CFW_SIM_STATUS gSimStatus[];

UINT32 CFW_SimInit(CFW_SIM_ID nSimID) // UINT32 CFW_SimInitStage1(VOID)
#else
UINT32 CFW_SimInit(VOID)  // UINT32 CFW_SimInitStage1(VOID)
#endif
{
  HAO hSimInitializeAo = (HAO) 0;

    CSW_TRACE(CFW_SIM_TS_ID, "CFW_SimInit[Sim %d]\n",nSimID);

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
  hSimInitializeAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimInitS1Data, CFW_SimInitStageBootProc, nSimID);
#else
  CFW_CfgSimGetSatParam(&pG_Sat_Param);
  CFW_CfgSimGetChvParam(&pG_Chv_Param);
  CFW_CfgSimGetPbkParam(&pG_Pbk_Param);
  CFW_CfgSimGetMicParam(&pG_Mic_Param);
  hSimInitializeAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimInitS1Data, CFW_SimInitStageBootProc);
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
#endif


