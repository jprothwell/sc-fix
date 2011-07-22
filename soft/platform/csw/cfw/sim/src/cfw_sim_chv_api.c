// //////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2002-2010, Coolsand Technologies, Inc.
// All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmission of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
// 
// FILENAME: cfw_sim_chv_api.c
// 
// DESCRIPTION:
// TODO: ...
// 
// REVISION HISTORY:
// NAME              DATE                REMAKS
// Administrator      2010-8-28       Created initial version 1.0
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
#include <cos.h>
#include "csw_debug.h"

#include "cfw_sim.h"
#include "cfw_sim_chv.h"

#ifdef CFW_MULTI_SIM
UINT32 CFW_SimGetAuthenticationStatus(UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimGetAuthenticationStatus(UINT16 nUTI)
#endif
{
  HAO hSimGetAuthAo = 0;

  CFW_GETAUTH* pSimGetAuthData = NULL;
  CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetAuthenticationStatus);

#ifdef CFW_MULTI_SIM
  UINT32 ret = ERR_SUCCESS;

  ret = CFW_CheckSimId(nSimID);
  if (ERR_SUCCESS != ret)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_CheckSimId return 0x%x \n", ret);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetAuthenticationStatus);
    return ret;
  }
#endif

  if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetAuthenticationStatus);
    return ERR_CFW_UTI_EXIST;
  }

  pSimGetAuthData = (CFW_GETAUTH*)CSW_SIM_MALLOC(SIZEOF(CFW_GETAUTH));
  if (pSimGetAuthData == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetAuthenticationStatus);
    return ERR_NO_MORE_MEMORY;
  }
#ifdef CFW_MULTI_SIM
  hSimGetAuthAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimGetAuthData, CFW_SimGetAuthenStatusProc, nSimID);
#else
  hSimGetAuthAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimGetAuthData, CFW_SimGetAuthenStatusProc);
#endif
  CFW_SetUTI(hSimGetAuthAo, nUTI, 0);

  pSimGetAuthData->nSm_GetAuth_prevStatus = SM_GETAUTH_INIT;
  pSimGetAuthData->nSm_GetAuth_currStatus = SM_GETAUTH_IDLE;
  CFW_SetAoProcCode(hSimGetAuthAo, CFW_AO_PROC_CODE_CONTINUE);
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetAuthenticationStatus);
  return (ERR_SUCCESS);
}

#ifdef CFW_MULTI_SIM
UINT32 CFW_SimEnterAuthentication(UINT8* pPin,
                                  UINT8 nPinSize,
                                  UINT8* pNewPin, UINT8 nNewPinSize, UINT8 nOption, UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimEnterAuthentication(UINT8* pPin,
                                  UINT8 nPinSize, UINT8* pNewPin, UINT8 nNewPinSize, UINT8 nOption, UINT16 nUTI)
#endif
{

  UINT8 i             = 0;
  HAO hSimEnterAuthAo = 0;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimEnterAuthentication);
  CSW_TRACE(CFW_SIM_TS_ID, "Invoking CFW_SimEnterAuthentication \n");

#ifdef CFW_MULTI_SIM
  UINT32 ret = ERR_SUCCESS;

  ret = CFW_CheckSimId(nSimID);
  if (ERR_SUCCESS != ret)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_CheckSimId return 0x%x \n", ret);

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthentication);
    return ret;
  }
#endif

  CFW_ENTERAUTH* pSimEnterAuthData = NULL;

  if ((pNewPin != NULL) && (nNewPinSize != 0))
  {
    if ((pPin == NULL) || (nPinSize != 8) || (nNewPinSize > 8) || (nNewPinSize < 4))
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error Parameter pPin %x,nPinSize %d,nNewPinSize %d\n", pPin, nPinSize, nNewPinSize);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthentication);
      return ERR_CFW_INVALID_PARAMETER;
    }
  }
  else
  {
    if ((pPin == NULL) || (nPinSize > 8) || (nPinSize < 4))
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error Parameter pPin %x,nPinSize %d,nNewPinSize %d\n", pPin, nPinSize, nNewPinSize);
      CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthentication);
      return ERR_CFW_INVALID_PARAMETER;
    }
  }
  if (nOption > 2)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error nOption 0x%x \n", nOption);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthentication);
    return ERR_CFW_INVALID_PARAMETER;
  }
  if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthentication);
    return ERR_CFW_UTI_EXIST;
  }
  pSimEnterAuthData = (CFW_ENTERAUTH*)CSW_SIM_MALLOC(SIZEOF(CFW_ENTERAUTH));
  if (pSimEnterAuthData == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthentication);
    return ERR_NO_MORE_MEMORY;
  }
  pSimEnterAuthData->nOption = nOption;

  SUL_MemCopy8(pSimEnterAuthData->pPinPwd, pPin, nPinSize);
  for (i = nPinSize; i < 8; i++)
    pSimEnterAuthData->pPinPwd[i] = 0xFF;

  if (nNewPinSize != 0)
  {
    SUL_MemCopy8(pSimEnterAuthData->pNewPwd, pNewPin, nNewPinSize);
    for (i = nNewPinSize; i < 8; i++)
      pSimEnterAuthData->pNewPwd[i] = 0xFF;
  }
  else
    pSimEnterAuthData->pNewPwd[0] = 0xFF;

#ifdef CFW_MULTI_SIM
  hSimEnterAuthAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimEnterAuthData, CFW_SimEnterAuthenticationProc, nSimID);
#else
  hSimEnterAuthAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimEnterAuthData, CFW_SimEnterAuthenticationProc);
#endif
  CFW_SetUTI(hSimEnterAuthAo, nUTI, 0);

  pSimEnterAuthData->nSm_EntAuth_prevStatus = SM_ENTAUTH_INIT;
  pSimEnterAuthData->nSm_EntAuth_currStatus = SM_ENTAUTH_IDLE;
  CFW_SetAoProcCode(hSimEnterAuthAo, CFW_AO_PROC_CODE_CONTINUE);
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthentication);

  return (ERR_SUCCESS);
}

#ifdef CFW_MULTI_SIM
UINT32 CFW_SimChangePassword(UINT16 nFac,
                             UINT8* pOldPwd,
                             UINT8 nOldPwdSize, UINT8* pNewPwd, UINT8 nNewPwdSize, UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SimChangePassword(UINT16 nFac,
                             UINT8* pOldPwd, UINT8 nOldPwdSize, UINT8* pNewPwd, UINT8 nNewPwdSize, UINT16 nUTI)
#endif
{
  UINT8 i = 0;
  HAO hSimChangePwAo;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimChangePassword);

#ifdef CFW_MULTI_SIM
  UINT32 ret = ERR_SUCCESS;

  ret = CFW_CheckSimId(nSimID);
  if (ERR_SUCCESS != ret)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_CheckSimId return 0x%x \n", ret);

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimChangePassword);
    return ret;
  }
#endif

  CFW_CHANGEPW* pSimChangePwData = NULL;

  if ((pOldPwd == NULL) || (pNewPwd == NULL) || (nOldPwdSize < 4) ||
      (nOldPwdSize > 8) || (nNewPwdSize > 8) || (nNewPwdSize < 4))
  {
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimChangePassword);
    return ERR_CFW_INVALID_PARAMETER;
  }

  if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimChangePassword);

    return ERR_CFW_UTI_EXIST;
  }
  pSimChangePwData = (CFW_CHANGEPW*)CSW_SIM_MALLOC(SIZEOF(CFW_CHANGEPW));
  if (pSimChangePwData == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimChangePassword);
    return ERR_NO_MORE_MEMORY;
  }
  pSimChangePwData->nFac = nFac;

  SUL_MemCopy8(pSimChangePwData->pOldPwd, pOldPwd, nOldPwdSize);
  for (i = nOldPwdSize; i < 8; i++)
    pSimChangePwData->pOldPwd[i] = 0xFF;

  SUL_MemCopy8(pSimChangePwData->pNewPwd, pNewPwd, nNewPwdSize);
  for (i = nNewPwdSize; i < 8; i++)
    pSimChangePwData->pNewPwd[i] = 0xFF;

  if (nFac == CFW_STY_FAC_TYPE_SC)
    pSimChangePwData->nCHVNumber = CHV1;
  else if (nFac == CFW_STY_FAC_TYPE_P2)
    pSimChangePwData->nCHVNumber = CHV2;
  else
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error nFac value 0x%x\n", nFac);
    CSW_SIM_FREE(pSimChangePwData);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimChangePassword);
    return ERR_CFW_INVALID_PARAMETER;
  }

#ifdef CFW_MULTI_SIM
  hSimChangePwAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimChangePwData, CFW_SimChangePasswordProc, nSimID);
#else
  hSimChangePwAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimChangePwData, CFW_SimChangePasswordProc);
#endif
  CFW_SetUTI(hSimChangePwAo, nUTI, 0);

  pSimChangePwData->nSm_ChgPw_prevStatus = SM_CHGPW_INIT;
  pSimChangePwData->nSm_ChgPw_currStatus = SM_CHGPW_IDLE;
  CFW_SetAoProcCode(hSimChangePwAo, CFW_AO_PROC_CODE_CONTINUE); // state change to read state.
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimChangePassword);

  return (ERR_SUCCESS);
}

#ifdef CFW_MULTI_SIM
UINT32 CFW_SimGetFacilityLock(UINT16 nFac, UINT16 nUTI, CFW_SIM_ID nSimID)
#else

UINT32 CFW_SimGetFacilityLock(UINT16 nFac, UINT16 nUTI)
#endif
{
  HAO hSimGetFacilityAo;

  CFW_GETFACILITY* pSimGetFAcilityData;
  CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetFacilityLock);

#ifdef CFW_MULTI_SIM
  UINT32 ret = ERR_SUCCESS;

  ret = CFW_CheckSimId(nSimID);
  if (ERR_SUCCESS != ret)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_CheckSimId return 0x%x \n", ret);

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetFacilityLock);
    return ret;
  }
#endif

  if ((nFac != CFW_STY_FAC_TYPE_SC) && (nFac != CFW_STY_FAC_TYPE_FD))
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error nFac value 0x%x\n", nFac);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetFacilityLock);
    return ERR_CFW_INVALID_PARAMETER;
  }
  if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetFacilityLock);
    return ERR_CFW_UTI_EXIST;
  }
  pSimGetFAcilityData = (CFW_GETFACILITY*)CSW_SIM_MALLOC(SIZEOF(CFW_GETFACILITY));
  if (pSimGetFAcilityData == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetFacilityLock);
    return ERR_NO_MORE_MEMORY;
  }
  pSimGetFAcilityData->nFac = nFac;

#ifdef CFW_MULTI_SIM
  hSimGetFacilityAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimGetFAcilityData, CFW_SimGetFacilityProc, nSimID);
#else
  hSimGetFacilityAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimGetFAcilityData, CFW_SimGetFacilityProc);
#endif
  CFW_SetUTI(hSimGetFacilityAo, nUTI, 0);

  pSimGetFAcilityData->nSm_GetFac_prevStatus = SM_GETFAC_INIT;
  pSimGetFAcilityData->nSm_GetFac_currStatus = SM_GETFAC_IDLE;
  CFW_SetAoProcCode(hSimGetFacilityAo, CFW_AO_PROC_CODE_CONTINUE);
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetFacilityLock);

  return (ERR_SUCCESS);
}

#ifdef CFW_MULTI_SIM
UINT32 CFW_SimSetFacilityLock(UINT16 nFac, UINT8* pBufPwd, UINT8 nPwdSize, UINT8 nMode, // 0 for unlock(=Disable); 1 for lock(=Eanble)
                              UINT16 nUTI, CFW_SIM_ID nSimID)
#else

UINT32 CFW_SimSetFacilityLock(UINT16 nFac, UINT8* pBufPwd, UINT8 nPwdSize, UINT8 nMode, // 0 for unlock(=Disable); 1 for lock(=Eanble)
                              UINT16 nUTI)
#endif
{
  UINT8 i = 0;
  HAO hSimSetFacilityAo;

  CFW_SETFACILITY* pSimSetFAcilityData = NULL;
  CSW_PROFILE_FUNCTION_ENTER(CFW_SimSetFacilityLock);

#ifdef CFW_MULTI_SIM
  UINT32 ret = ERR_SUCCESS;

  ret = CFW_CheckSimId(nSimID);
  if (ERR_SUCCESS != ret)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_CheckSimId return 0x%x \n", ret);

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetFacilityLock);
    return ret;
  }
#endif

  if ((nFac != CFW_STY_FAC_TYPE_SC) && (nFac != CFW_STY_FAC_TYPE_FD))
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error nFac value 0x%x\n", nFac);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetFacilityLock);
    return ERR_CFW_INVALID_PARAMETER;
  }
  if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error UTI Busy\n");

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetFacilityLock);
    return ERR_CFW_UTI_EXIST;
  }
  if ((nMode > 1) || (nPwdSize > 8) || (nPwdSize < 4))
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Parameter nMode %x,nPwdSize %d,nPwdSize %d\n", nMode, nPwdSize, nPwdSize);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetFacilityLock);
    return ERR_CFW_INVALID_PARAMETER;
  }
  pSimSetFAcilityData = (CFW_SETFACILITY*)CSW_SIM_MALLOC(SIZEOF(CFW_SETFACILITY));
  if (pSimSetFAcilityData == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetFacilityLock);
    return ERR_NO_MORE_MEMORY;
  }
  pSimSetFAcilityData->nFac     = nFac;
  pSimSetFAcilityData->nMode    = nMode;
  pSimSetFAcilityData->nPwdSize = nPwdSize;

  SUL_MemCopy8(pSimSetFAcilityData->pBufPwd, pBufPwd, nPwdSize);
  for (i = nPwdSize; i < 8; i++)
    pSimSetFAcilityData->pBufPwd[i] = 0xFF;

#ifdef CFW_MULTI_SIM
  hSimSetFacilityAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimSetFAcilityData, CFW_SimSetFacilityProc, nSimID);
#else
  hSimSetFacilityAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimSetFAcilityData, CFW_SimSetFacilityProc);
#endif
  CFW_SetUTI(hSimSetFacilityAo, nUTI, 0);

  pSimSetFAcilityData->nSm_SetFac_prevStatus = SM_SETFAC_INIT;
  pSimSetFAcilityData->nSm_SetFac_currStatus = SM_SETFAC_IDLE;
  CFW_SetAoProcCode(hSimSetFacilityAo, CFW_AO_PROC_CODE_CONTINUE);
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimSetFacilityLock);
  return (ERR_SUCCESS);
}
 
