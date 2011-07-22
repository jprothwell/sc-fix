// //////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2002-2009, Coolsand Technologies, Inc.
// All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmission of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
// 
// FILENAME: cfw_sim_pbk_api.c
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
#include <cos.h>
#include <sul.h>
#include <ts.h>
#include <csw_mem_prv.h>
#include "csw_debug.h"

#include "cfw_sim.h"
#include "cfw_sim_pbk.h"

// 
// main steps will be done in this function:
// 1. check input parameters
// 2. call CFW_CfgGetPbkStorage() to get the current phonebook
// 3. call cfw_setUTI(pTransId) to inform system
// 4. check pEntryInfo->phoneIndex is zero or not
// 5. if =0, store pEntryInfo to context and call _SimSeekReq(nStorage) to 
// search for the 1st free record.
// 6. else call _SimUpdateRecordReq(phoneIndex,Name+Type+Number)
// 
#ifdef CFW_MULTI_SIM
UINT32 CFW_SimAddPbkEntry(UINT8 nStorage, CFW_SIM_PBK_ENTRY_INFO* pEntryInfo, UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimAddPbkEntry(UINT8 nStorage, CFW_SIM_PBK_ENTRY_INFO* pEntryInfo, UINT16 nUTI)
#endif
{
  HAO hAo = 0x00000000;

  SIM_SM_INFO_ADDPBK* pAddPBK = NULL;
  SIM_PBK_PARAM* pSimPBKPara = NULL;
  CSW_PROFILE_FUNCTION_ENTER(CFW_SimAddPbkEntry);
  if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimAddPbkEntry);
    return ERR_CFW_UTI_IS_BUSY;
  }

#ifdef CFW_MULTI_SIM
  UINT32 Ret = ERR_SUCCESS;

  Ret = CFW_CheckSimId(nSimID);
  if (Ret != ERR_SUCCESS)
  {
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimAddPbkEntry);
    return Ret;
  }
  if (nStorage == 0)
    CFW_CfgGetPbkStorage(&nStorage, nSimID);

  CFW_CfgSimGetPbkParam(&pSimPBKPara, nSimID);
#else
  if (nStorage == 0)
    CFW_CfgGetPbkStorage(&nStorage);

  CFW_CfgSimGetPbkParam(&pSimPBKPara);
#endif
  pAddPBK = (SIM_SM_INFO_ADDPBK*)CSW_SIM_MALLOC(SIZEOF(SIM_SM_INFO_ADDPBK));
  if (pAddPBK == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimAddPbkEntry);
    return ERR_NO_MORE_MEMORY;  // should be updated by macro   
  }
  SUL_ZeroMemory32(pAddPBK, SIZEOF(SIM_SM_INFO_ADDPBK));

  switch (nStorage)
  {
  case CFW_PBK_SIM:

    pAddPBK->nCurrentFile = API_SIM_EF_ADN;
    if (pEntryInfo->phoneIndex > pSimPBKPara->nADNTotalNum)
    {
      CSW_SIM_FREE(pAddPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimAddPbkEntry);
      CSW_TRACE(CFW_SIM_TS_ID, "Error pEntryInfo->phoneIndex>pSimPBKPara->nADNTotalNum\n");
      return ERR_CFW_INVALID_PARAMETER;
    }
    if ((pEntryInfo->nNumberSize > SIM_PBK_NUMBER_SIZE)
        || (pEntryInfo->iFullNameSize > (pSimPBKPara->nADNRecordSize - CFW_SIM_REMAINLENGTH)))
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error pEntryInfo->nNumberSize 0x%x pEntryInfo->iFullNameSize 0x%x\n",
                    pEntryInfo->nNumberSize, pEntryInfo->iFullNameSize);
      CSW_SIM_FREE(pAddPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimAddPbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    break;

  case CFW_PBK_SIM_FIX_DIALLING:
    pAddPBK->nCurrentFile = API_SIM_EF_FDN;
    if (pEntryInfo->phoneIndex > pSimPBKPara->nFDNTotalNum)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error pEntryInfo->phoneIndex>pSimPBKPara->nFDNTotalNum\n");
      CSW_SIM_FREE(pAddPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimAddPbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    if ((pEntryInfo->nNumberSize > SIM_PBK_NUMBER_SIZE)
        || (pEntryInfo->iFullNameSize > (pSimPBKPara->nFDNRecordSize - CFW_SIM_REMAINLENGTH)))
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error pEntryInfo->nNumberSize 0x%x pEntryInfo->iFullNameSize 0x%x\n",
                    pEntryInfo->nNumberSize, pEntryInfo->iFullNameSize);
      CSW_SIM_FREE(pAddPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimAddPbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    break;

  case CFW_PBK_SIM_LAST_DIALLING:
    pAddPBK->nCurrentFile = API_SIM_EF_LND;
    if (pEntryInfo->phoneIndex > pSimPBKPara->nLNDTotalNum)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error pEntryInfo->phoneIndex>pSimPBKPara->nLNDTotalNum\n");
      CSW_SIM_FREE(pAddPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimAddPbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    if ((pEntryInfo->nNumberSize > SIM_PBK_NUMBER_SIZE)
        || (pEntryInfo->iFullNameSize > (pSimPBKPara->nLNDRecordSize - CFW_SIM_REMAINLENGTH)))
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error pEntryInfo->nNumberSize 0x%x pEntryInfo->iFullNameSize 0x%x\n",
                    pEntryInfo->nNumberSize, pEntryInfo->iFullNameSize);

      CSW_SIM_FREE(pAddPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimAddPbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    break;

  case CFW_PBK_SIM_BARRED_DIALLING:
    pAddPBK->nCurrentFile = API_SIM_EF_BDN;
    if (pEntryInfo->phoneIndex > pSimPBKPara->nBDNTotalNum)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error pEntryInfo->phoneIndex>pSimPBKPara->nLNDTotalNum\n");
      CFW_FREE(pAddPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimAddPbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    if ((pEntryInfo->nNumberSize > SIM_PBK_NUMBER_SIZE)
        || (pEntryInfo->iFullNameSize > (pSimPBKPara->nBDNRecordSize - CFW_SIM_REMAINLENGTH)))
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error pEntryInfo->nNumberSize 0x%x pEntryInfo->iFullNameSize 0x%x\n",
                    pEntryInfo->nNumberSize, pEntryInfo->iFullNameSize);
      CFW_FREE(pAddPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimAddPbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    break;

  case CFW_PBK_ON:
    pAddPBK->nCurrentFile = API_SIM_EF_MSISDN;
    if (pEntryInfo->phoneIndex > pSimPBKPara->nMSISDNTotalNum)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error pEntryInfo->phoneIndex>pSimPBKPara->nMSISDNTotalNum\n");
      CSW_SIM_FREE(pAddPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimAddPbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    if ((pEntryInfo->nNumberSize > SIM_PBK_NUMBER_SIZE)
        || (pEntryInfo->iFullNameSize > (pSimPBKPara->nMSISDNRecordSize - CFW_SIM_REMAINLENGTH)))
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error pEntryInfo->nNumberSize 0x%x pEntryInfo->iFullNameSize 0x%x\n",
                    pEntryInfo->nNumberSize, pEntryInfo->iFullNameSize);
      CSW_SIM_FREE(pAddPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimAddPbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    break;

  default:
    CSW_SIM_FREE(pAddPBK);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimAddPbkEntry);
    CSW_TRACE(CFW_SIM_TS_ID, "Error nStorage 0x%x \n", nStorage);
    return ERR_CFW_INVALID_PARAMETER;
  }

  // [[hameina[mod] 2008.10.07:bug 10006, move here from above
  // 对于sim卡上的电话簿，统一改为：仅当number type=CFW_TELNUMBER_TYPE_INTERNATIONAL时
  // 才允许numbersize=0，namesize=0.其它条件均不允许numbersize,namesize同时为0
  if ((pEntryInfo->iFullNameSize == 0) && (pEntryInfo->nNumberSize == 0) && (pEntryInfo->nType != 145)) // niej 080503 number is only '+'
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error NULL\n");
    CSW_SIM_FREE(pAddPBK);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimAddPbkEntry);
    return ERR_CFW_INVALID_PARAMETER;
  }

  // ]]hameina[mod]2008.10.07

#ifdef CFW_MULTI_SIM
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pAddPBK, CFW_SimPBKAddProc, nSimID);
#else
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pAddPBK, CFW_SimPBKAddProc);
#endif
  CFW_SetUTI(hAo, nUTI, 0);
  if (pEntryInfo->nNumberSize > 0)
    SUL_MemCopy8(pAddPBK->pNum, pEntryInfo->pNumber, pEntryInfo->nNumberSize);
  if (pEntryInfo->iFullNameSize > 0)
    SUL_MemCopy8(pAddPBK->pData, pEntryInfo->pFullName, pEntryInfo->iFullNameSize);
  pAddPBK->nNumType  = pEntryInfo->nType;
  pAddPBK->nNameSize = pEntryInfo->iFullNameSize;
  pAddPBK->nNumSize  = pEntryInfo->nNumberSize;
  if (pEntryInfo->phoneIndex != 0)
  {
    pAddPBK->nAddRecInd = pEntryInfo->phoneIndex;
  }
  CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimAddPbkEntry);
  return (ERR_SUCCESS);
}

// 
// main steps will be done in this function:
// 1. check input parameters to make sure nIndex != 0
// 2. call CFW_CfgGetPbkStorage() to get the current phonebook
// 3. call cfw_setUTI(pTransId)
// 4. call _SimUpdateRecordReq(nIndex, "0xFFFFFF....")
// 
#ifdef CFW_MULTI_SIM
UINT32 CFW_SimDeletePbkEntry(UINT8 nStorage, UINT8 nIndex, UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimDeletePbkEntry(UINT8 nStorage, UINT8 nIndex, UINT16 nUTI)
#endif
{
  HAO hAo = 0x00000000;

  SIM_SM_INFO_DELEPBK* pDeletPBK = NULL;
  SIM_PBK_PARAM* pSimPBKPara = NULL;
  CSW_PROFILE_FUNCTION_ENTER(CFW_SimDeletePbkEntry);
#ifdef CFW_MULTI_SIM
  UINT32 Ret = ERR_SUCCESS;

  Ret = CFW_CheckSimId(nSimID);
  if (Ret != ERR_SUCCESS)
  {
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeletePbkEntry);
    return Ret;
  }
  if (nStorage == CFW_PBK_AUTO)
    CFW_CfgGetPbkStorage(&nStorage, nSimID);
  CFW_CfgSimGetPbkParam(&pSimPBKPara, nSimID);
#else
  if (nStorage == CFW_PBK_AUTO)
    CFW_CfgGetPbkStorage(&nStorage);
  CFW_CfgSimGetPbkParam(&pSimPBKPara);
#endif

  if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeletePbkEntry);
    return ERR_CFW_UTI_IS_BUSY;
  }
  pDeletPBK = (SIM_SM_INFO_DELEPBK*)CSW_SIM_MALLOC(SIZEOF(SIM_SM_INFO_DELEPBK));
  if (pDeletPBK == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeletePbkEntry);
    return ERR_NO_MORE_MEMORY;  // should be updated by macro
  }

  SUL_ZeroMemory32(pDeletPBK, SIZEOF(SIM_SM_INFO_DELEPBK));
  switch (nStorage)
  {

  case CFW_PBK_SIM:
    pDeletPBK->nCurrentFile = API_SIM_EF_ADN;
    if ((0 >= nIndex) || (nIndex > pSimPBKPara->nADNTotalNum))
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter 0x%x\n", nIndex);
      CSW_SIM_FREE(pDeletPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeletePbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    break;
  case CFW_PBK_SIM_FIX_DIALLING:
    pDeletPBK->nCurrentFile = API_SIM_EF_FDN;
    if ((0 >= nIndex) || (nIndex > pSimPBKPara->nFDNTotalNum))
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter 0x%x\n", nIndex);
      CSW_SIM_FREE(pDeletPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeletePbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    break;
  case CFW_PBK_SIM_LAST_DIALLING:
    pDeletPBK->nCurrentFile = API_SIM_EF_LND;
    if ((0 >= nIndex) || (nIndex > pSimPBKPara->nLNDTotalNum))
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter 0x%x\n", nIndex);
      CSW_SIM_FREE(pDeletPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeletePbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    break;
  case CFW_PBK_SIM_BARRED_DIALLING:
    pDeletPBK->nCurrentFile = API_SIM_EF_BDN;
    if ((0 >= nIndex) || (nIndex > pSimPBKPara->nBDNTotalNum))
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter 0x%x\n", nIndex);
      CFW_FREE(pDeletPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeletePbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    break;
  case CFW_PBK_ON:
    pDeletPBK->nCurrentFile = API_SIM_EF_MSISDN;
    if ((0 >= nIndex) || (nIndex > pSimPBKPara->nMSISDNTotalNum))
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter 0x%x\n", nIndex);
      CSW_SIM_FREE(pDeletPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeletePbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    break;
  default:
    CSW_SIM_FREE(pDeletPBK);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeletePbkEntry);
    return ERR_CFW_INVALID_PARAMETER;
  }

#ifdef CFW_MULTI_SIM
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pDeletPBK, CFW_SimPBKDelProc, nSimID);
#else
  hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pDeletPBK, CFW_SimPBKDelProc);
#endif
  CFW_SetUTI(hAo, nUTI, 0);
  pDeletPBK->nDeleRecInd = (UINT8)nIndex;
  CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimDeletePbkEntry);
  return ERR_SUCCESS;
}

// 
// main steps will be done in this function:
// 1. check input parameters to make sure nIndex != 0
// 2. call CFW_CfgGetPbkStorage() to get the current phonebook
// 3. call cfw_setUTI(pTransId)
// 4. call _SimReadRecordReq(nIndex)
// 

#ifdef CFW_MULTI_SIM
UINT32 CFW_SimGetPbkEntry(UINT8 nStorage, UINT8 nIndex, UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimGetPbkEntry(UINT8 nStorage, UINT8 nIndex, UINT16 nUTI)
#endif
{
  HAO hAo = 0x0000000000;

  SIM_SM_INFO_GETPBK* pGetPBK = NULL;
  SIM_PBK_PARAM* pSimPBKPara = NULL;
  CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetPbkEntry);

#ifdef CFW_MULTI_SIM
  UINT32 Ret = ERR_SUCCESS;

  Ret = CFW_CheckSimId(nSimID);
  if (Ret != ERR_SUCCESS)
  {
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkEntry);
    return Ret;
  }
  if (nStorage == CFW_PBK_AUTO)
    CFW_CfgGetPbkStorage(&nStorage, nSimID);

  CFW_CfgSimGetPbkParam(&pSimPBKPara, nSimID);
#else
  if (nStorage == CFW_PBK_AUTO)
    CFW_CfgGetPbkStorage(&nStorage);

  CFW_CfgSimGetPbkParam(&pSimPBKPara);
#endif

  if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkEntry);
    return ERR_CFW_UTI_IS_BUSY;
  }
  pGetPBK = (SIM_SM_INFO_GETPBK*)CSW_SIM_MALLOC(SIZEOF(SIM_SM_INFO_GETPBK));
  if (pGetPBK == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkEntry);
    return ERR_NO_MORE_MEMORY;  // should be updated by macro
  }

  SUL_ZeroMemory32(pGetPBK, SIZEOF(SIM_SM_INFO_GETPBK));

  switch (nStorage)
  {

  case CFW_PBK_SIM:
    pGetPBK->nCurrentFile = API_SIM_EF_ADN;
    if ((0 >= nIndex) || (nIndex > pSimPBKPara->nADNTotalNum))
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter 0x%x\n", nIndex);

      CSW_SIM_FREE(pGetPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    break;
  case CFW_PBK_SIM_FIX_DIALLING:
    pGetPBK->nCurrentFile = API_SIM_EF_FDN;
    if ((0 >= nIndex) || (nIndex > pSimPBKPara->nFDNTotalNum))
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter 0x%x\n", nIndex);

      CSW_SIM_FREE(pGetPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    break;
  case CFW_PBK_SIM_LAST_DIALLING:
    pGetPBK->nCurrentFile = API_SIM_EF_LND;
    if ((0 >= nIndex) || (nIndex > pSimPBKPara->nLNDTotalNum))
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter 0x%x\n", nIndex);

      CSW_SIM_FREE(pGetPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    break;
  case CFW_PBK_ON:
    pGetPBK->nCurrentFile = API_SIM_EF_MSISDN;
    if ((0 >= nIndex) || (nIndex > pSimPBKPara->nMSISDNTotalNum))
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter 0x%x\n", nIndex);

      CSW_SIM_FREE(pGetPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    break;
  case CFW_PBK_EN:
    pGetPBK->nCurrentFile = API_SIM_EF_ECC;
    if ((0 >= nIndex) || (nIndex > pSimPBKPara->nECCTotalNum))
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter 0x%x\n", nIndex);

      CSW_SIM_FREE(pGetPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkEntry);
      return ERR_CFW_INVALID_PARAMETER;
    }
    break;
  default:
    CSW_SIM_FREE(pGetPBK);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkEntry);
     CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid nStorage 0x%x\n", nStorage);

      return ERR_CFW_INVALID_PARAMETER;

    }

#ifdef CFW_MULTI_SIM
    hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pGetPBK, CFW_SimPBKEntryProc, nSimID);
#else
    hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pGetPBK, CFW_SimPBKEntryProc);
#endif
    CFW_SetUTI(hAo, nUTI, 0);
    pGetPBK->nCurrRecInd = (UINT8)nIndex;
    CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkEntry);
    return (ERR_SUCCESS);
  }

// 
// main steps will be done in this function:
// 1. check input parameters
// 2. call CFW_CfgGetPbkStorage() to get the current phonebook
// 3. call cfw_setUTI(pTransId)
// 4. call _SimReadRecordReq(nIndexStart)
// 

#ifdef CFW_MULTI_SIM
  UINT32 CFW_SimListCountPbkEntries(UINT8 nStorage, UINT8 nIndexStart, UINT8 nCount, UINT16 nUTI, CFW_SIM_ID nSimID)
#else
  UINT32 CFW_SimListCountPbkEntries(UINT8 nStorage, UINT8 nIndexStart, UINT8 nCount, UINT16 nUTI)
#endif
  {
    HAO hAo = 0x00000000;

    SIM_SM_INFO_LISTCOUNTPBK* pListCountPBK = NULL;
    SIM_PBK_PARAM* pSimPBKPara = NULL;
    CSW_PROFILE_FUNCTION_ENTER(CFW_SimListCountPbkEntries);

#ifdef CFW_MULTI_SIM
    UINT32 Ret = ERR_SUCCESS;

    Ret = CFW_CheckSimId(nSimID);
    if (Ret != ERR_SUCCESS)
    {
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimListCountPbkEntries);
      return Ret;
    }
    if (nStorage == CFW_PBK_AUTO)
      CFW_CfgGetPbkStorage(&nStorage, nSimID);

    CFW_CfgSimGetPbkParam(&pSimPBKPara, nSimID);
#else
    if (nStorage == CFW_PBK_AUTO)
      CFW_CfgGetPbkStorage(&nStorage);

    CFW_CfgSimGetPbkParam(&pSimPBKPara);
#endif

    if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
    {CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimListCountPbkEntries);
      return ERR_CFW_UTI_IS_BUSY;
    }
    pListCountPBK = (SIM_SM_INFO_LISTCOUNTPBK*)CSW_SIM_MALLOC(SIZEOF(SIM_SM_INFO_LISTCOUNTPBK));
    if (pListCountPBK == NULL)
    {CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

      CSW_PROFILE_FUNCTION_EXIT(CFW_SimListCountPbkEntries);
      return ERR_NO_MORE_MEMORY;  // should be updated by macro
    }

    SUL_ZeroMemory32(pListCountPBK, SIZEOF(SIM_SM_INFO_LISTPBK));
    switch (nStorage)
    {
    case CFW_PBK_SIM:
      pListCountPBK->nCurrentFile = API_SIM_EF_ADN;
      if ((0 >= nIndexStart) || (nCount < 1) || (nCount > pSimPBKPara->nADNTotalNum))
      {
        CSW_SIM_FREE(pListCountPBK);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimListCountPbkEntries);
        return ERR_CFW_INVALID_PARAMETER;
      }
      break;
    case CFW_PBK_SIM_FIX_DIALLING:
      pListCountPBK->nCurrentFile = API_SIM_EF_FDN;
      if ((0 >= nIndexStart) || (nCount < 1) || (nCount > pSimPBKPara->nFDNTotalNum))
      {
              CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter nIndexStart 0x%x nCount %d\n", nIndexStart,nCount);

        CSW_SIM_FREE(pListCountPBK);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimListCountPbkEntries);
        return ERR_CFW_INVALID_PARAMETER;
      }
      break;
    case CFW_PBK_SIM_LAST_DIALLING:
      CSW_TRACE(CFW_SIM_TS_ID, TSTXT("SIM  TEST =====>CFW_SimListCountPbkEntries--LND\n"));
      pListCountPBK->nCurrentFile = API_SIM_EF_LND;
      if ((0 >= nIndexStart) || (nCount < 1) || (nCount > pSimPBKPara->nLNDTotalNum))
      {
                      CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter nIndexStart 0x%x nCount %d\n", nIndexStart,nCount);

        CSW_SIM_FREE(pListCountPBK);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimListCountPbkEntries);
        return ERR_CFW_INVALID_PARAMETER;
      }
      break;
    case CFW_PBK_SIM_BARRED_DIALLING:
      CSW_TRACE(CFW_SIM_TS_ID, TSTXT("SIM  TEST =====>CFW_SimListCountPbkEntries--BDN\n"));
      pListCountPBK->nCurrentFile = API_SIM_EF_BDN;
      if ((0 >= nIndexStart) || (nCount < 1) || (nCount > pSimPBKPara->nBDNTotalNum))
      {
                      CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter nIndexStart 0x%x nCount %d\n", nIndexStart,nCount);

        CFW_FREE(pListCountPBK);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimListCountPbkEntries);
        return ERR_CFW_INVALID_PARAMETER;
      }
      break;

    case CFW_PBK_SIM_SERVICE_DIALLING:
      CSW_TRACE(CFW_SIM_TS_ID, TSTXT("SIM  TEST =====>CFW_SimListCountPbkEntries--SDN\n"));
      pListCountPBK->nCurrentFile = API_SIM_EF_SDN;
      if ((0 >= nIndexStart) || (nCount < 1) || (nCount > pSimPBKPara->nSDNTotalNum))
      {
                      CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter nIndexStart 0x%x nCount %d\n", nIndexStart,nCount);

        CFW_FREE(pListCountPBK);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimListCountPbkEntries);
        return ERR_CFW_INVALID_PARAMETER;
      }
      break;
    case CFW_PBK_ON:
      pListCountPBK->nCurrentFile = API_SIM_EF_MSISDN;
      if ((0 >= nIndexStart) || (nCount < 1) || (nCount > pSimPBKPara->nMSISDNTotalNum))
      {
                      CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter nIndexStart 0x%x nCount %d\n", nIndexStart,nCount);

        CSW_SIM_FREE(pListCountPBK);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimListCountPbkEntries);
        return ERR_CFW_INVALID_PARAMETER;
      }
      break;
    case CFW_PBK_EN:
      pListCountPBK->nCurrentFile = API_SIM_EF_ECC;
      if ((0 >= nIndexStart) || (nCount < 1) || (nCount > pSimPBKPara->nECCTotalNum))
      {
                      CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter nIndexStart 0x%x nCount %d\n", nIndexStart,nCount);

        CSW_SIM_FREE(pListCountPBK);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimListCountPbkEntries);
        return ERR_CFW_INVALID_PARAMETER;
      }
      break;
    default:
      CSW_SIM_FREE(pListCountPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimListCountPbkEntries);
      return ERR_CFW_INVALID_PARAMETER;

    }

#ifdef CFW_MULTI_SIM
    hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pListCountPBK, CFW_SimPBKListCountEntryProc, nSimID);
#else
    hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pListCountPBK, CFW_SimPBKListCountEntryProc);
#endif
    CFW_SetUTI(hAo, nUTI, 0);
    pListCountPBK->nStartRecInd = (UINT8)nIndexStart;
    pListCountPBK->nCurrRecInd  = (UINT8)nIndexStart;
    pListCountPBK->nCount       = (UINT8)nCount;
    pListCountPBK->nStructNum   = 0;
    CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimListCountPbkEntries);
    return (ERR_SUCCESS);
  }

#ifdef CFW_MULTI_SIM
  UINT32 CFW_SimListPbkEntries(UINT8 nStorage, UINT8 nIndexStart, UINT8 nIndexEnd, UINT16 nUTI, CFW_SIM_ID nSimID)
#else
  UINT32 CFW_SimListPbkEntries(UINT8 nStorage, UINT8 nIndexStart, UINT8 nIndexEnd, UINT16 nUTI)
#endif
  {
    HAO hAo = 0x00000000;

    SIM_SM_INFO_LISTPBK* pListPBK = NULL;
    SIM_PBK_PARAM* pSimPBKPara = NULL;
    CSW_PROFILE_FUNCTION_ENTER(CFW_SimListPbkEntries);

#ifdef CFW_MULTI_SIM
    UINT32 Ret = ERR_SUCCESS;

    Ret = CFW_CheckSimId(nSimID);
    if (Ret != ERR_SUCCESS)
    {
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimListPbkEntries);
      return Ret;
    }
    if (nStorage == CFW_PBK_AUTO)
      CFW_CfgGetPbkStorage(&nStorage, nSimID);

    CFW_CfgSimGetPbkParam(&pSimPBKPara, nSimID);
#else
    if (nStorage == CFW_PBK_AUTO)
      CFW_CfgGetPbkStorage(&nStorage);

    CFW_CfgSimGetPbkParam(&pSimPBKPara);
#endif

    if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimListPbkEntries);
      return ERR_CFW_UTI_IS_BUSY;
    }
    if (nIndexEnd < nIndexStart)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error nIndexEnd < nIndexStart\n");
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimListPbkEntries);
      return ERR_CFW_INVALID_PARAMETER;
    }

    pListPBK = (SIM_SM_INFO_LISTPBK*)CSW_SIM_MALLOC(SIZEOF(SIM_SM_INFO_LISTPBK));
    if (pListPBK == NULL)
    {
CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
      return ERR_NO_MORE_MEMORY;  // should be updated by macro

    }

    SUL_ZeroMemory32(pListPBK, SIZEOF(SIM_SM_INFO_LISTPBK));

    switch (nStorage)
    {

    case CFW_PBK_SIM:
      pListPBK->nCurrentFile = API_SIM_EF_ADN;
      if ((0 >= nIndexStart) || (nIndexEnd > pSimPBKPara->nADNTotalNum))
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter nIndexStart 0x%x nIndexEnd %d\n", nIndexStart,nIndexEnd);
        CSW_SIM_FREE(pListPBK);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimListPbkEntries);
        return ERR_CFW_INVALID_PARAMETER;
      }
      break;
    case CFW_PBK_SIM_FIX_DIALLING:
      pListPBK->nCurrentFile = API_SIM_EF_FDN;
      if ((0 >= nIndexStart) || (nIndexEnd > pSimPBKPara->nFDNTotalNum))
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter nIndexStart 0x%x nIndexEnd %d\n", nIndexStart,nIndexEnd);
        
        CSW_SIM_FREE(pListPBK);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimListPbkEntries);
        return ERR_CFW_INVALID_PARAMETER;
      }
      break;
    case CFW_PBK_SIM_LAST_DIALLING:

      pListPBK->nCurrentFile = API_SIM_EF_LND;
      if ((0 >= nIndexStart) || (nIndexEnd > pSimPBKPara->nLNDTotalNum))
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter nIndexStart 0x%x nIndexEnd %d\n", nIndexStart,nIndexEnd);
        
        CSW_SIM_FREE(pListPBK);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimListPbkEntries);
        return ERR_CFW_INVALID_PARAMETER;
      }
      break;
    case CFW_PBK_SIM_BARRED_DIALLING:

      pListPBK->nCurrentFile = API_SIM_EF_BDN;
      if ((0 >= nIndexStart) || (nIndexEnd > pSimPBKPara->nBDNTotalNum))
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter nIndexStart 0x%x nIndexEnd %d\n", nIndexStart,nIndexEnd);
        
        CFW_FREE(pListPBK);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimListPbkEntries);
        return ERR_CFW_INVALID_PARAMETER;
      }
      break;

    case CFW_PBK_SIM_SERVICE_DIALLING:

      pListPBK->nCurrentFile = API_SIM_EF_SDN;
      if ((0 >= nIndexStart) || (nIndexEnd > pSimPBKPara->nSDNTotalNum))
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter nIndexStart 0x%x nIndexEnd %d\n", nIndexStart,nIndexEnd);
        CFW_FREE(pListPBK);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimListPbkEntries);
        return ERR_CFW_INVALID_PARAMETER;
      }
      break;
    case CFW_PBK_ON:
      pListPBK->nCurrentFile = API_SIM_EF_MSISDN;
      if ((0 >= nIndexStart) || (nIndexEnd > pSimPBKPara->nMSISDNTotalNum))
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter nIndexStart 0x%x nIndexEnd %d\n", nIndexStart,nIndexEnd);
        CSW_SIM_FREE(pListPBK);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimListPbkEntries);
        return ERR_CFW_INVALID_PARAMETER;
      }
      break;
    case CFW_PBK_EN:
      pListPBK->nCurrentFile = API_SIM_EF_ECC;
      if ((0 >= nIndexStart) || (nIndexEnd > pSimPBKPara->nECCTotalNum))
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error Invalid paramter nIndexStart 0x%x nIndexEnd %d\n", nIndexStart,nIndexEnd);
        CSW_SIM_FREE(pListPBK);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimListPbkEntries);
        return ERR_CFW_INVALID_PARAMETER;
      }
      break;
    default:
      CSW_TRACE(CFW_SIM_TS_ID, TSTXT("SIM  TEST =====>CFW_SimListPbkEntries--NO\n"));
      CSW_SIM_FREE(pListPBK);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimListPbkEntries);
      return ERR_CFW_INVALID_PARAMETER;
    }

#ifdef CFW_MULTI_SIM
    hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pListPBK, CFW_SimPBKListEntryProc, nSimID);
#else
    hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pListPBK, CFW_SimPBKListEntryProc);
#endif
    CFW_SetUTI(hAo, nUTI, 0);
    pListPBK->nStartRecInd = (UINT8)nIndexStart;
    pListPBK->nEndRecInd   = (UINT8)nIndexEnd;
    pListPBK->nCurrRecInd  = (UINT8)nIndexStart;
    pListPBK->nStructNum   = 1;
    CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimListPbkEntries);
    return (ERR_SUCCESS);
  }

// 
// main steps will be done in this function:
// 1. call CFW_CfgGetPbkStorage() to get the current phonebook
// 2. call cfw_setUTI(pTransId)
// 3. call _SimSeekReq(nStorage) to search for the 1st free record
// 

#ifdef CFW_MULTI_SIM
  UINT32 CFW_SimGetPbkStrorageInfo(UINT8 nStorage, UINT16 nUTI, CFW_SIM_ID nSimID)
#else
  UINT32 CFW_SimGetPbkStrorageInfo(UINT8 nStorage, UINT16 nUTI)
#endif
  {
    HAO hAo = 0x00000000;

    SIM_SM_INFO_STORAGEINFO* pStorageInfo = NULL;
    CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetPbkStrorageInfo);


#ifdef CFW_MULTI_SIM
    UINT32 Ret = ERR_SUCCESS;

    Ret = CFW_CheckSimId(nSimID);
    if (Ret != ERR_SUCCESS)
    {
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkStrorageInfo);
      return Ret;
    }
    if (nStorage == CFW_PBK_AUTO)
      CFW_CfgGetPbkStorage(&nStorage, nSimID);
#else
    if (nStorage == CFW_PBK_AUTO)
      CFW_CfgGetPbkStorage(&nStorage);
#endif
    if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
    {CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkStrorageInfo);
      return ERR_CFW_UTI_IS_BUSY;
    }
    pStorageInfo = (SIM_SM_INFO_STORAGEINFO*)CSW_SIM_MALLOC(SIZEOF(SIM_SM_INFO_STORAGEINFO));
    if (pStorageInfo == NULL)
    {CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkStrorageInfo);
      return ERR_NO_MORE_MEMORY;  // should be updated by macro
    }
    SUL_ZeroMemory32(pStorageInfo, SIZEOF(SIM_SM_INFO_STORAGEINFO));

    switch (nStorage)
    {
    case CFW_PBK_SIM:
      pStorageInfo->nCurrentFile = API_SIM_EF_ADN;
      break;
    case CFW_PBK_SIM_FIX_DIALLING:
      pStorageInfo->nCurrentFile = API_SIM_EF_FDN;
      break;
    case CFW_PBK_SIM_LAST_DIALLING:
      pStorageInfo->nCurrentFile = API_SIM_EF_LND;
      break;
    case CFW_PBK_SIM_BARRED_DIALLING:
      pStorageInfo->nCurrentFile = API_SIM_EF_BDN;
      break;
    case CFW_PBK_SIM_SERVICE_DIALLING:
      pStorageInfo->nCurrentFile = API_SIM_EF_SDN;
      break;
    case CFW_PBK_ON:
      pStorageInfo->nCurrentFile = API_SIM_EF_MSISDN;
      break;
    case CFW_PBK_EN:
      pStorageInfo->nCurrentFile = API_SIM_EF_ECC;
      break;
    default:
      CSW_SIM_FREE(pStorageInfo);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkStrorageInfo);
       CSW_TRACE(CFW_SIM_TS_ID, "Error nStorage %d\n",nStorage);

      return ERR_CFW_INVALID_PARAMETER;
    }


#ifdef CFW_MULTI_SIM
    hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pStorageInfo, CFW_SimPBKStorageInfoProc, nSimID);
#else
    hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pStorageInfo, CFW_SimPBKStorageInfoProc);
#endif
    CFW_SetUTI(hAo, nUTI, 0);
    CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkStrorageInfo);
    return (ERR_SUCCESS);
  }

#ifdef CFW_MULTI_SIM
  UINT32 CFW_SimGetPbkStorage(UINT8 nStorage, UINT16 nUTI, CFW_SIM_ID nSimID)
#else
  UINT32 CFW_SimGetPbkStorage(UINT8 nStorage, UINT16 nUTI)
#endif
  {
    HAO hAo = 0x00000000;

    SIM_SM_INFO_STORAGE* pStorage = NULL;
    CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetPbkStorage);

#ifdef CFW_MULTI_SIM
    UINT32 Ret = ERR_SUCCESS;

    Ret = CFW_CheckSimId(nSimID);
    if (Ret != ERR_SUCCESS)
    {
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkStorage);
      return Ret;
    }
    if (nStorage == CFW_PBK_AUTO)
      CFW_CfgGetPbkStorage(&nStorage, nSimID);
#else
    if (nStorage == CFW_PBK_AUTO)
      CFW_CfgGetPbkStorage(&nStorage);
#endif

    if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
    {
    	CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkStorage);
      return ERR_CFW_UTI_IS_BUSY;

    }
    pStorage = (SIM_SM_INFO_STORAGE*)CSW_SIM_MALLOC(SIZEOF(SIM_SM_INFO_STORAGE));
    if (pStorage == NULL)
    {
    	CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkStorage);
      return ERR_NO_MORE_MEMORY;  // should be updated by macro
    }
    SUL_ZeroMemory32(pStorage, SIZEOF(SIM_SM_INFO_STORAGE));
    switch (nStorage)
    {
    case CFW_PBK_SIM:
      pStorage->nCurrentFile = API_SIM_EF_ADN;
      break;
    case CFW_PBK_SIM_FIX_DIALLING:
      pStorage->nCurrentFile = API_SIM_EF_FDN;
      break;
    case CFW_PBK_SIM_LAST_DIALLING:
      pStorage->nCurrentFile = API_SIM_EF_LND;
      break;
    case CFW_PBK_SIM_BARRED_DIALLING:
      pStorage->nCurrentFile = API_SIM_EF_BDN;
      break;
    case CFW_PBK_SIM_SERVICE_DIALLING:
      pStorage->nCurrentFile = API_SIM_EF_SDN;
      break;
    case CFW_PBK_ON:
      pStorage->nCurrentFile = API_SIM_EF_MSISDN;
      break;
    case CFW_PBK_EN:
      pStorage->nCurrentFile = API_SIM_EF_ECC;
      break;
    default:
      CSW_SIM_FREE(pStorage);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkStorage);
      CSW_TRACE(CFW_SIM_TS_ID, "Error nStorage %d\n",nStorage);

      return ERR_CFW_INVALID_PARAMETER;
    }
    CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SimGetPbkStrorage()\r\n"));

#ifdef CFW_MULTI_SIM
    hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pStorage, CFW_SimPBKStorageProc, nSimID);
#else
    hAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pStorage, CFW_SimPBKStorageProc);
#endif
    CFW_SetUTI(hAo, nUTI, 0);
    CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetPbkStorage);
    return (ERR_SUCCESS);
  }

#if 0 // Remove by lixp at 20090204
// *****************************************
// add by wanghb  for temp use begin  2007.7.27
// *****************************************
  UINT32 CFW_SimPBKListCount_TimeOut()  // Stack_MsgMalloc
  {
    api_SimUpdateRecordCnf_t* pSimUpdaterecordCnf = CSW_SIM_MALLOC(SIZEOF(api_SimUpdateRecordCnf_t));
    SUL_ZeroMemory8(pSimUpdaterecordCnf, sizeof(api_SimUpdateRecordCnf_t));

    pSimUpdaterecordCnf->Sw1 = 0x93;
    pSimUpdaterecordCnf->Sw2 = 0x00;

    UINT32 nRet =
      CFW_BalSendMessage(11, API_SIM_UPDATERECORD_CNF, pSimUpdaterecordCnf, sizeof(api_SimUpdateRecordCnf_t));
    return nRet;
  }

// *****************************************
// add by wanghb  for temp use end  2007.7.27
// *****************************************
#endif
 
