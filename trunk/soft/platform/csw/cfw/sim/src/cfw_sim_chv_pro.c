// //////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2002-2009, Coolsand Technologies, Inc.
// All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmission of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
// 
// FILENAME: cfw_sim_chv_pro.c
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

#include "cfw_sim.h"
#include "cfw_sim_chv.h"

UINT32 CFW_SimGetAuthenStatusProc(HAO hAo, CFW_EV* pEvent)
{
  HAO hSimGetAuthPwAo = hAo;

  CFW_GETAUTH* pSimGetAuthData = NULL;
  UINT32 nEvtId  = 0;
  VOID* nEvParam = 0;

  UINT32 nUTI = 0;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetAuthenStatusProc);
  if ((UINT32)pEvent != 0xFFFFFFFF)
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }

#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hSimGetAuthPwAo);
#endif

  pSimGetAuthData = CFW_GetAoUserData(hSimGetAuthPwAo);
  switch (pSimGetAuthData->nSm_GetAuth_currStatus)
  {
  case SM_GETAUTH_IDLE:
    {
      if ((UINT32)pEvent == 0xFFFFFFFF)
      {
#ifdef CFW_MULTI_SIM
        UINT32 Result = SimDedicFileStatusReq(API_DF_TELECOM, nSimID);

        if (ERR_SUCCESS != Result)
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error SimDedicFileStatusReq return 0x%x \n", Result);
          CFW_GetUTI(hSimGetAuthPwAo, &nUTI);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_AUTH_STATUS_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimGetAuthPwAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetAuthenStatusProc);
          return ERR_SUCCESS;
        }
#else
        UINT32 Result = SimDedicFileStatusReq(API_DF_TELECOM);

        if (ERR_SUCCESS != Result)
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error SimDedicFileStatusReq return 0x%x \n", Result);
          CFW_GetUTI(hSimGetAuthPwAo, &nUTI);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_AUTH_STATUS_RSP, Result, 0, nUTI, 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimGetAuthPwAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetAuthenStatusProc);
          return ERR_SUCCESS;
        }
#endif
        pSimGetAuthData->nSm_GetAuth_prevStatus = pSimGetAuthData->nSm_GetAuth_currStatus;
        pSimGetAuthData->nSm_GetAuth_currStatus = SM_GETAUTH_DEDICATED;
      }
      else
        CFW_SetAoProcCode(hSimGetAuthPwAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_GETAUTH_DEDICATED:
    {
      if (nEvtId == API_SIM_DEDICFILESTATUS_CNF)
      {
        api_SimDedicFileStatusCnf_t* pSimDedicFileStatusCnf = (api_SimDedicFileStatusCnf_t*)nEvParam;
        CSW_TRACE(CFW_SIM_TS_ID, "API_SIM_DEDICFILESTATUS_CNF SW1 0x%x SW2 0x%x \n", pSimDedicFileStatusCnf->Sw1,
                  pSimDedicFileStatusCnf->Sw2);

#ifdef CFW_MULTI_SIM
        if (((0x90 == pSimDedicFileStatusCnf->Sw1) && (00 == pSimDedicFileStatusCnf->Sw2))
            || (0x91 == pSimDedicFileStatusCnf->Sw1))
        {
          UINT32 Remains = 0;

          SIM_CHV_PARAM* pG_Chv_Param = NULL;
          Remains = SimParseDedicatedStatus(pSimDedicFileStatusCnf->Data, nSimID);
          CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
          CFW_GetUTI(hSimGetAuthPwAo, &nUTI);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_AUTH_STATUS_RSP, (UINT32)pG_Chv_Param->nCHVAuthen,
                              Remains, nUTI | (nSimID << 24), 0);
        }
        else
        {
          UINT32 nErrCode = 0;

          nErrCode = Sim_ParseSW1SW2(pSimDedicFileStatusCnf->Sw1, pSimDedicFileStatusCnf->Sw2, nSimID);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_AUTH_STATUS_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0);
        }
#else
        if (((0x90 == pSimDedicFileStatusCnf->Sw1) && (00 == pSimDedicFileStatusCnf->Sw2))
            || (0x91 == pSimDedicFileStatusCnf->Sw1))
        {
          UINT32 Remains = 0;

          SIM_CHV_PARAM* pG_Chv_Param = NULL;
          Remains = SimParseDedicatedStatus(pSimDedicFileStatusCnf->Data);
          CFW_CfgSimGetChvParam(&pG_Chv_Param);
          CFW_GetUTI(hSimGetAuthPwAo, &nUTI);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_AUTH_STATUS_RSP, (UINT32)pG_Chv_Param->nCHVAuthen, Remains, nUTI, 0);

        }
        else
        {
          UINT32 nErrCode = 0;

          nErrCode = Sim_ParseSW1SW2(pSimDedicFileStatusCnf->Sw1, pSimDedicFileStatusCnf->Sw2);
          CFW_PostNotifyEvent(EV_CFW_SIM_GET_AUTH_STATUS_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0);
        }
#endif

        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimGetAuthPwAo);

      }
      else
        CFW_SetAoProcCode(hSimGetAuthPwAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;

  case SM_GETAUTH_MMI:
    break;
  case SM_GETAUTH_STACK:
    break;
  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetAuthenStatusProc);
  return ERR_SUCCESS;
}

UINT32 CFW_SimEnterAuthenticationProc(HAO hAo, CFW_EV* pEvent)
{
  HAO hSimEnterAuthPwAo = hAo;

  CFW_ENTERAUTH* pSimEnterAuthData = NULL;

  UINT32 nEvtId  = 0;
  VOID* nEvParam = 0;

  UINT32 nUTI     = 0;
  UINT32 nErrCode = 0;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimEnterAuthenticationProc);

  if ((UINT32)pEvent != 0xFFFFFFFF)
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }

#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hSimEnterAuthPwAo);
#endif

  pSimEnterAuthData = CFW_GetAoUserData(hSimEnterAuthPwAo);
  switch (pSimEnterAuthData->nSm_EntAuth_currStatus)
  {
  case SM_ENTAUTH_IDLE:
    {
      if ((UINT32)pEvent == 0xFFFFFFFF)
      {
#ifdef CFW_MULTI_SIM
        UINT32 Result = SimDedicFileStatusReq(API_DF_TELECOM, nSimID);

        if (ERR_SUCCESS != Result)
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error SimDedicFileStatusReq return 0x%x \n", Result);

          CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);
          CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthenticationProc);
          return Result;
        }
#else
        UINT32 Result = SimDedicFileStatusReq(API_DF_TELECOM);

        if (ERR_SUCCESS != Result)
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error SimDedicFileStatusReq return 0x%x \n", Result);

          CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);
          CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, Result, 0, nUTI, 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthenticationProc);
          return Result;
        }
#endif

        pSimEnterAuthData->nSm_EntAuth_prevStatus = pSimEnterAuthData->nSm_EntAuth_currStatus;
        pSimEnterAuthData->nSm_EntAuth_currStatus = SM_ENTAUTH_DEDICATED;
      }
      else
        CFW_SetAoProcCode(hSimEnterAuthPwAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_ENTAUTH_DEDICATED:
    {
      api_SimDedicFileStatusCnf_t* pSimDedicFileStatusCnf = (api_SimDedicFileStatusCnf_t*)nEvParam;
      if (((0x90 == pSimDedicFileStatusCnf->Sw1) &&
           (00 == pSimDedicFileStatusCnf->Sw2)) || (0x91 == pSimDedicFileStatusCnf->Sw1))

#ifdef CFW_MULTI_SIM
      {
        UINT8 CHV = 0;

        SIM_CHV_PARAM* pG_Chv_Param;

        SimParseDedicatedStatus(pSimDedicFileStatusCnf->Data, nSimID);
        CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
        CSW_TRACE(CFW_SIM_TS_ID, "pSimEnterAuthData->nOption %d\n", pSimEnterAuthData->nOption);

        if (pSimEnterAuthData->nOption == 0)
        {
          UINT32 Result = 0;

          CSW_TRACE(CFW_SIM_TS_ID, "pG_Chv_Param->nCHVAuthen %d\n", pG_Chv_Param->nCHVAuthen);
          if ((pG_Chv_Param->nCHVAuthen == CFW_SIM_AUTH_PIN1BLOCK) ||
              (pG_Chv_Param->nCHVAuthen == CFW_SIM_AUTH_PIN2BLOCK))
          {
            CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, pG_Chv_Param->nCHVAuthen, 0, nUTI | (nSimID << 24), 0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
          }
          else if (pG_Chv_Param->nCHVAuthen == CFW_SIM_AUTH_PUK1)
          {
            pSimEnterAuthData->nOption = 1;
            Result                     = SimUnblockCHVReq(pSimEnterAuthData->pPinPwd, pSimEnterAuthData->pNewPwd, CHV1, nSimID);
            if (ERR_SUCCESS != Result)
            {
              CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimUnblockCHVReq return 0x%x \n", Result);

              CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthenticationProc);
              return ERR_SUCCESS;
            }

            pSimEnterAuthData->nSm_EntAuth_prevStatus = pSimEnterAuthData->nSm_EntAuth_currStatus;
            pSimEnterAuthData->nSm_EntAuth_currStatus = SM_ENTAUTH_UNBLOCKCHV;
          }
          else if (pG_Chv_Param->nCHVAuthen == CFW_SIM_AUTH_PUK2)
          {
            pSimEnterAuthData->nOption = 2;
            Result                     = SimUnblockCHVReq(pSimEnterAuthData->pPinPwd, pSimEnterAuthData->pNewPwd, CHV2, nSimID);
            if (ERR_SUCCESS != Result)
            {
              CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);

              CSW_TRACE(CFW_SIM_TS_ID, "Error SimUnblockCHVReq return 0x%x \n", Result);

              CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);

              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthenticationProc);
              return ERR_SUCCESS;
            }

            pSimEnterAuthData->nSm_EntAuth_prevStatus = pSimEnterAuthData->nSm_EntAuth_currStatus;
            pSimEnterAuthData->nSm_EntAuth_currStatus = SM_ENTAUTH_UNBLOCKCHV;
          }
          else if (pG_Chv_Param->nCHVAuthen == CFW_SIM_AUTH_PIN1)
          {
            pSimEnterAuthData->nOption = 1;
            Result                     = SimVerifyCHVReq(pSimEnterAuthData->pPinPwd, CHV1, nSimID);
            if (ERR_SUCCESS != Result)
            {
              CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimVerifyCHVReq return 0x%x \n", Result);

              CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthenticationProc);
              return ERR_SUCCESS;
            }

            pSimEnterAuthData->nSm_EntAuth_prevStatus = pSimEnterAuthData->nSm_EntAuth_currStatus;
            pSimEnterAuthData->nSm_EntAuth_currStatus = SM_ENTAUTH_VERIFYCHV;
          }
          else if (pG_Chv_Param->nCHVAuthen == CFW_SIM_AUTH_PIN2)
          {
            pSimEnterAuthData->nOption = 2;
            Result                     = SimVerifyCHVReq(pSimEnterAuthData->pPinPwd, CHV2, nSimID);
            if (ERR_SUCCESS != Result)
            {
              CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimVerifyCHVReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);

              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthenticationProc);
              return ERR_SUCCESS;
            }

            pSimEnterAuthData->nSm_EntAuth_prevStatus = pSimEnterAuthData->nSm_EntAuth_currStatus;
            pSimEnterAuthData->nSm_EntAuth_currStatus = SM_ENTAUTH_VERIFYCHV;
          }
          else
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error pG_Chv_Param->nCHVAuthen %d\n", pG_Chv_Param->nCHVAuthen);

            CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, pG_Chv_Param->nCHVAuthen, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
          }
        }
        else
        {
          CSW_TRACE(CFW_SIM_TS_ID, "pSimEnterAuthData->nOption %d\n", pSimEnterAuthData->nOption);
          if (pSimEnterAuthData->nOption == 1)
            CHV = CHV1;
          else
            CHV = CHV2;
          CSW_TRACE(CFW_SIM_TS_ID, "pSimEnterAuthData->pNewPwd[0] 0x%x\n", pSimEnterAuthData->pNewPwd[0]);
          if (pSimEnterAuthData->pNewPwd[0] != 0xFF)
          {
            UINT32 Result = SimUnblockCHVReq(pSimEnterAuthData->pPinPwd, pSimEnterAuthData->pNewPwd, CHV, nSimID);

            if (ERR_SUCCESS != Result)
            {
              CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimUnblockCHVReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthenticationProc);
              return ERR_SUCCESS;
            }

            pSimEnterAuthData->nSm_EntAuth_prevStatus = pSimEnterAuthData->nSm_EntAuth_currStatus;
            pSimEnterAuthData->nSm_EntAuth_currStatus = SM_ENTAUTH_UNBLOCKCHV;
          }
          else
          {
            UINT32 Result = SimVerifyCHVReq(pSimEnterAuthData->pPinPwd, CHV, nSimID);

            if (ERR_SUCCESS != Result)
            {
              CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimVerifyCHVReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);

              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthenticationProc);
              return ERR_SUCCESS;
            }
            pSimEnterAuthData->nSm_EntAuth_prevStatus = pSimEnterAuthData->nSm_EntAuth_currStatus;
            pSimEnterAuthData->nSm_EntAuth_currStatus = SM_ENTAUTH_VERIFYCHV;
          }
        }
        CFW_SetAoProcCode(hSimEnterAuthPwAo, CFW_AO_PROC_CODE_CONSUMED);
      }
#else
      {
        UINT8 CHV = 0;

        SIM_CHV_PARAM* pG_Chv_Param;
        SimParseDedicatedStatus(pSimDedicFileStatusCnf->Data);
        CFW_CfgSimGetChvParam(&pG_Chv_Param);

        CSW_TRACE(CFW_SIM_TS_ID, "pSimEnterAuthData->nOption %d\n", pSimEnterAuthData->nOption);

        if (pSimEnterAuthData->nOption == 0)
        {

          UINT32 Result = 0;

          CSW_TRACE(CFW_SIM_TS_ID, "pG_Chv_Param->nCHVAuthen %d\n", pG_Chv_Param->nCHVAuthen);
          if ((pG_Chv_Param->nCHVAuthen == CFW_SIM_AUTH_PIN1BLOCK) ||
              (pG_Chv_Param->nCHVAuthen == CFW_SIM_AUTH_PIN2BLOCK))
          {
            // The process will be implemented in the second version.
            CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, pG_Chv_Param->nCHVAuthen, 0, nUTI, 0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
          }
          else if (pG_Chv_Param->nCHVAuthen == CFW_SIM_AUTH_PUK1)
          {
            pSimEnterAuthData->nOption = 1;
            Result                     = SimUnblockCHVReq(pSimEnterAuthData->pPinPwd, pSimEnterAuthData->pNewPwd, CHV1);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimUnblockCHVReq return 0x%x \n", Result);
              CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);
              CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, Result, 0, nUTI, 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthenticationProc);
              return ERR_SUCCESS;
            }

            pSimEnterAuthData->nSm_EntAuth_prevStatus = pSimEnterAuthData->nSm_EntAuth_currStatus;
            pSimEnterAuthData->nSm_EntAuth_currStatus = SM_ENTAUTH_UNBLOCKCHV;
          }
          else if (pG_Chv_Param->nCHVAuthen == CFW_SIM_AUTH_PUK2)
          {
            pSimEnterAuthData->nOption = 2;
            Result                     = SimUnblockCHVReq(pSimEnterAuthData->pPinPwd, pSimEnterAuthData->pNewPwd, CHV2);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimUnblockCHVReq return 0x%x \n", Result);
              CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);
              CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, Result, 0, nUTI, 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthenticationProc);
              return ERR_SUCCESS;
            }

            pSimEnterAuthData->nSm_EntAuth_prevStatus = pSimEnterAuthData->nSm_EntAuth_currStatus;
            pSimEnterAuthData->nSm_EntAuth_currStatus = SM_ENTAUTH_UNBLOCKCHV;
          }
          else if (pG_Chv_Param->nCHVAuthen == CFW_SIM_AUTH_PIN1)
          {
            pSimEnterAuthData->nOption = 1;
            Result                     = SimVerifyCHVReq(pSimEnterAuthData->pPinPwd, CHV1);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimVerifyCHVReq return 0x%x \n", Result);
              CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);
              CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, Result, 0, nUTI, 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthenticationProc);
              return ERR_SUCCESS;
            }

            pSimEnterAuthData->nSm_EntAuth_prevStatus = pSimEnterAuthData->nSm_EntAuth_currStatus;
            pSimEnterAuthData->nSm_EntAuth_currStatus = SM_ENTAUTH_VERIFYCHV;
          }
          else if (pG_Chv_Param->nCHVAuthen == CFW_SIM_AUTH_PIN2)
          {
            pSimEnterAuthData->nOption = 2;
            Result                     = SimVerifyCHVReq(pSimEnterAuthData->pPinPwd, CHV2);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimVerifyCHVReq return 0x%x \n", Result);
              CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);
              CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, Result, 0, nUTI, 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthenticationProc);
              return ERR_SUCCESS;
            }

            pSimEnterAuthData->nSm_EntAuth_prevStatus = pSimEnterAuthData->nSm_EntAuth_currStatus;
            pSimEnterAuthData->nSm_EntAuth_currStatus = SM_ENTAUTH_VERIFYCHV;
          }
          else  // if(G_Chv_Param.nCHVAuthen == CFW_SIM_AUTH_PIN1DISABLE,
            // CFW_STY_AUTH_PIN2READY,CFW_SIM_AUTH_PIN1READY
          {
            CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, pG_Chv_Param->nCHVAuthen, 0, nUTI, 0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
          }
        }
        else
        {
          CSW_TRACE(CFW_SIM_TS_ID, "pSimEnterAuthData->nOption %d\n", pSimEnterAuthData->nOption);
          if (pSimEnterAuthData->nOption == 1)
            CHV = CHV1;
          else
            CHV = CHV2;

          CSW_TRACE(CFW_SIM_TS_ID, "pSimEnterAuthData->pNewPwd[0] 0x%x\n", pSimEnterAuthData->pNewPwd[0]);

          if (pSimEnterAuthData->pNewPwd[0] != 0xFF)
          {
            UINT32 Result = SimUnblockCHVReq(pSimEnterAuthData->pPinPwd, pSimEnterAuthData->pNewPwd, CHV);

            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimUnblockCHVReq return 0x%x \n", Result);
              CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);
              CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, Result, 0, nUTI, 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthenticationProc);
              return ERR_SUCCESS;
            }

            pSimEnterAuthData->nSm_EntAuth_prevStatus = pSimEnterAuthData->nSm_EntAuth_currStatus;
            pSimEnterAuthData->nSm_EntAuth_currStatus = SM_ENTAUTH_UNBLOCKCHV;
          }
          else
          {
            UINT32 Result = SimVerifyCHVReq(pSimEnterAuthData->pPinPwd, CHV);

            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimVerifyCHVReq return 0x%x \n", Result);
              CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);
              CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);
              CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, Result, 0, nUTI, 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthenticationProc);
              return ERR_SUCCESS;
            }
            pSimEnterAuthData->nSm_EntAuth_prevStatus = pSimEnterAuthData->nSm_EntAuth_currStatus;
            pSimEnterAuthData->nSm_EntAuth_currStatus = SM_ENTAUTH_VERIFYCHV;
          }
        }
        CFW_SetAoProcCode(hSimEnterAuthPwAo, CFW_AO_PROC_CODE_CONSUMED);
      }
#endif

      else
      {
        SIM_MISC_PARAM* pG_Mic_aram = NULL;

// if(pG_Mic_aram->bStartPeriod == TRUE)
// nUTI = GENERATE_SHELL_UTI();
// else
        CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);

#ifdef CFW_MULTI_SIM
        CFW_CfgSimGetMicParam(&pG_Mic_aram, nSimID);

        nErrCode = Sim_ParseSW1SW2(pSimDedicFileStatusCnf->Sw1, pSimDedicFileStatusCnf->Sw2, nSimID);
        if (ERR_CFW_QUEUE_FULL ==
            CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0));
#else
        CFW_CfgSimGetMicParam(&pG_Mic_aram);
        nErrCode = Sim_ParseSW1SW2(pSimDedicFileStatusCnf->Sw1, pSimDedicFileStatusCnf->Sw2);
        if (ERR_CFW_QUEUE_FULL == CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0))
#endif
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_PostNotifyEvent ERR_CFW_QUEUE_FULL\n");
          CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);

        }
        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
      }
    }
    break;
  case SM_ENTAUTH_UNBLOCKCHV:
    {
      if (nEvtId == API_SIM_UNBLOCKCHV_CNF)
      {
        SIM_MISC_PARAM* pG_Mic_Param;
        SIM_CHV_PARAM* pG_Chv_Param;

        api_SimUnblockCHVCnf_t* pSimUnblockCHVCnf = (api_SimUnblockCHVCnf_t*)nEvParam;
        CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);

#ifdef CFW_MULTI_SIM
        CFW_CfgSimGetMicParam(&pG_Mic_Param, nSimID);
        CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);

        if ((0x90 == pSimUnblockCHVCnf->Sw1) && (0x00 == pSimUnblockCHVCnf->Sw2))
        {
          CSW_TRACE(CFW_SIM_TS_ID, "API_SIM_UNBLOCKCHV_CNF OK\n");
          if (pSimEnterAuthData->nOption == 1)
          {
            pG_Chv_Param->nCHVAuthen  = CFW_SIM_AUTH_PIN1READY;
            pG_Chv_Param->nPin1Status = CHV_VERIFY;
            CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, 0, 0, nUTI | (nSimID << 24), 0);
            if (pG_Mic_Param->bStartPeriod == TRUE)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "SM_ENTAUTH_UNBLOCKCHV1 OK\n");
              CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, 0, 0, GENERATE_SHELL_UTI() | (nSimID << 24),
                                  SIM_INIT_EV_STAGE2);
              pG_Mic_Param->bStartPeriod = FALSE;
            }
          }
          else  // pSimEnterAuthData->nOption == 2
          {
            pG_Chv_Param->bCHV2VerifyReq = FALSE; // NEED VERIFY????
            pG_Chv_Param->nPin2Status    = CHV_VERIFY;
            pG_Chv_Param->nPin2Status    = CHV_VERIFY;
            CSW_TRACE(CFW_SIM_TS_ID, "SM_ENTAUTH_UNBLOCKCHV2 OK\n");

            CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, 0, 0, nUTI | (nSimID << 24), 0);
          }
        }
        else
        {
          CSW_TRACE(CFW_SIM_TS_ID, "API_SIM_UNBLOCKCHV_CNF  ERROR\n");
          nErrCode = Sim_ParseSW1SW2(pSimUnblockCHVCnf->Sw1, pSimUnblockCHVCnf->Sw2, nSimID);
          CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0);
        }
#else

        CFW_CfgSimGetMicParam(&pG_Mic_Param);
        CFW_CfgSimGetChvParam(&pG_Chv_Param);

        if ((0x90 == pSimUnblockCHVCnf->Sw1) && (0x00 == pSimUnblockCHVCnf->Sw2))
        {
          CSW_TRACE(CFW_SIM_TS_ID, "API_SIM_UNBLOCKCHV_CNF  OK\n");
          if (pSimEnterAuthData->nOption == 1)
          {
            pG_Chv_Param->nCHVAuthen  = CFW_SIM_AUTH_PIN1READY;
            pG_Chv_Param->nPin1Status = CHV_VERIFY;
            if (pG_Mic_Param->bStartPeriod == TRUE)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "SM_ENTAUTH_UNBLOCKCHV1 OK\n");
              CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, 0, 0, GENERATE_SHELL_UTI(), SIM_INIT_EV_STAGE2);
              pG_Mic_Param->bStartPeriod = FALSE;
            }

// else
// {
            CSW_TRACE(CFW_SIM_TS_ID, "SM_ENTAUTH_UNBLOCKCHV1 OK\n");
            CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, 0, 0, nUTI, 0);

// }
          }
          else  // pSimEnterAuthData->nOption == 2
          {
            pG_Chv_Param->bCHV2VerifyReq = FALSE; // NEED VERIFY????
            pG_Chv_Param->nPin2Status    = CHV_VERIFY;

// pG_Chv_Param.nCHVAuthen = CFW_STY_AUTH_PIN2READY;  //NEED VERIFY????
            pG_Chv_Param->nPin2Status = CHV_VERIFY;
            CSW_TRACE(CFW_SIM_TS_ID, "SM_ENTAUTH_UNBLOCKCHV2 OK\n");
            CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, 0, 0, nUTI, 0);
          }
        }
        else
        {
          CSW_TRACE(CFW_SIM_TS_ID, "API_SIM_UNBLOCKCHV_CNF  ERROR\n");
          nErrCode = Sim_ParseSW1SW2(pSimUnblockCHVCnf->Sw1, pSimUnblockCHVCnf->Sw2);

// if(pG_Mic_Param->bStartPeriod == TRUE)
// {
// CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP,(UINT32)nErrCode,0,GENERATE_SHELL_UTI(), SIM_INIT_EV_STAGE2);                  
// }
// else 
          CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0);
        }

#endif

        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
      }
      else
        CFW_SetAoProcCode(hSimEnterAuthPwAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_ENTAUTH_VERIFYCHV:
    {
      if (nEvtId == API_SIM_VERIFYCHV_CNF)
      {
        SIM_MISC_PARAM* pG_Mic_Param;
        SIM_CHV_PARAM* pG_Chv_Param;
        api_SimVerifyCHVCnf_t* pSimVerifyCHVCnf = (api_SimVerifyCHVCnf_t*)nEvParam;
        CFW_GetUTI(hSimEnterAuthPwAo, &nUTI);

#ifdef CFW_MULTI_SIM
        CFW_CfgSimGetMicParam(&pG_Mic_Param, nSimID);
        CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);

        if ((0x90 == pSimVerifyCHVCnf->Sw1) && (00 == pSimVerifyCHVCnf->Sw2))
        {
          CSW_TRACE(CFW_SIM_TS_ID, "API_SIM_VERIFYCHV_CNF  OK\n");
          if (pSimEnterAuthData->nOption == 1)
          {
            pG_Chv_Param->nCHVAuthen  = CFW_SIM_AUTH_PIN1READY;
            pG_Chv_Param->nPin1Status = CHV_VERIFY;
            CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, 0, 0, nUTI | (nSimID << 24), 0);
            if (pG_Mic_Param->bStartPeriod == TRUE)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "SM_ENTAUTH_VERIFYCHV1 OK\n");
              CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, 0, 0, GENERATE_SHELL_UTI() | (nSimID << 24),
                                  SIM_INIT_EV_STAGE2);
              pG_Mic_Param->bStartPeriod = FALSE;
            }
          }
          else  // pSimEnterAuthData->nOption == 2
          {
            pG_Chv_Param->bCHV2VerifyReq = FALSE; // NEED VERIFY????
            pG_Chv_Param->nPin2Status    = CHV_VERIFY;
            pG_Chv_Param->nPin2Status    = CHV_VERIFY;
            CSW_TRACE(CFW_SIM_TS_ID, "SM_ENTAUTH_VERIFYCHV2 OK\n");
            CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, 0, 0, nUTI | (nSimID << 24), 0);
          }
        }
        else
        {
          CSW_TRACE(CFW_SIM_TS_ID, "ERROR API_SIM_VERIFYCHV_CNF \n");
          nErrCode = Sim_ParseSW1SW2(pSimVerifyCHVCnf->Sw1, pSimVerifyCHVCnf->Sw2, nSimID);
          CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0);
        }

#else
        CFW_CfgSimGetMicParam(&pG_Mic_Param);
        CFW_CfgSimGetChvParam(&pG_Chv_Param);

        if ((0x90 == pSimVerifyCHVCnf->Sw1) && (00 == pSimVerifyCHVCnf->Sw2))
        {
          CSW_TRACE(CFW_SIM_TS_ID, "API_SIM_VERIFYCHV_CNF  OK\n");
          if (pSimEnterAuthData->nOption == 1)
          {
            pG_Chv_Param->nCHVAuthen  = CFW_SIM_AUTH_PIN1READY;
            pG_Chv_Param->nPin1Status = CHV_VERIFY;
            if (pG_Mic_Param->bStartPeriod == TRUE)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "SM_ENTAUTH_VERIFYCHV1 shell\n");
              CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, 0, 0, GENERATE_SHELL_UTI(), SIM_INIT_EV_STAGE2);
              pG_Mic_Param->bStartPeriod = FALSE;
            }

// else
// {
            CSW_TRACE(CFW_SIM_TS_ID, "SM_ENTAUTH_VERIFYCHV1 OK\n");
            CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, 0, 0, nUTI, 0);

// }
          }
          else  // pSimEnterAuthData->nOption == 2
          {
            pG_Chv_Param->bCHV2VerifyReq = FALSE; // NEED VERIFY????
            pG_Chv_Param->nPin2Status    = CHV_VERIFY;

// G_Chv_Param.nCHVAuthen = CFW_STY_AUTH_PIN2READY;  //NEED VERIFY????
            pG_Chv_Param->nPin2Status = CHV_VERIFY;
            CSW_TRACE(CFW_SIM_TS_ID, "SM_ENTAUTH_VERIFYCHV2 OK\n");
            CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, 0, 0, nUTI, 0);
          }
        }
        else
        {
          CSW_TRACE(CFW_SIM_TS_ID, "ERROR API_SIM_VERIFYCHV_CNF\n");
          nErrCode = Sim_ParseSW1SW2(pSimVerifyCHVCnf->Sw1, pSimVerifyCHVCnf->Sw2);

// if(pG_Mic_Param->bStartPeriod == TRUE)
// {
// CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP,(UINT32)nErrCode, 0, GENERATE_SHELL_UTI(), SIM_INIT_EV_STAGE2); 
// }
// else
          CFW_PostNotifyEvent(EV_CFW_SIM_ENTER_AUTH_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0);
        }

#endif

        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimEnterAuthPwAo);
      }
      else
        CFW_SetAoProcCode(hSimEnterAuthPwAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_ENTAUTH_MMI:
    break;
  case SM_ENTAUTH_STACK:
    break;
  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimEnterAuthenticationProc);
  return ERR_SUCCESS;
}

UINT32 CFW_SimChangePasswordProc(HAO hAo, CFW_EV* pEvent)
{
  HAO hSimChangePwAo = hAo;

  CFW_CHANGEPW* pSimChangePwData = NULL;
  SIM_CHV_PARAM* pG_Chv_Param;

  UINT32 nEvtId  = 0;
  VOID* nEvParam = 0;

  UINT32 nUTI     = 0;
  UINT32 nErrCode = 0;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimChangePasswordProc);

  if ((UINT32)pEvent != 0xFFFFFFFF)
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }

#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hSimChangePwAo);
  CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
#else
  CFW_CfgSimGetChvParam(&pG_Chv_Param);
#endif

  pSimChangePwData = CFW_GetAoUserData(hSimChangePwAo);
  switch (pSimChangePwData->nSm_ChgPw_currStatus)
  {
  case SM_CHGPW_IDLE:
    {
      if ((UINT32)pEvent == 0xFFFFFFFF)
      {
#ifdef CFW_MULTI_SIM
        UINT32 Result = SimChangeCHVReq(pSimChangePwData->pOldPwd, pSimChangePwData->pNewPwd,
                                        pSimChangePwData->nCHVNumber, nSimID);

        if (ERR_SUCCESS != Result)
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error SimChangeCHVReq return 0x%x\n", Result);
          CFW_GetUTI(hSimChangePwAo, &nUTI);
          CFW_PostNotifyEvent(EV_CFW_SIM_CHANGE_PWD_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimChangePwAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SimChangePasswordProc);
          return ERR_SUCCESS;
        }
#else
        UINT32 Result = SimChangeCHVReq(pSimChangePwData->pOldPwd, pSimChangePwData->pNewPwd,
                                        pSimChangePwData->nCHVNumber);

        if (ERR_SUCCESS != Result)
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error SimChangeCHVReq return 0x%x\n", Result);

          CFW_GetUTI(hSimChangePwAo, &nUTI);
          CFW_PostNotifyEvent(EV_CFW_SIM_CHANGE_PWD_RSP, Result, 0, nUTI, 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimChangePwAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SimChangePasswordProc);
          return ERR_SUCCESS;
        }
#endif

        pSimChangePwData->nSm_ChgPw_prevStatus = pSimChangePwData->nSm_ChgPw_currStatus;
        pSimChangePwData->nSm_ChgPw_currStatus = SM_CHGPW_CHANGECHV;
        CFW_SetAoProcCode(hSimChangePwAo, CFW_AO_PROC_CODE_CONSUMED);
      }
      else
        CFW_SetAoProcCode(hSimChangePwAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_CHGPW_CHANGECHV:
    {
      if (nEvtId == API_SIM_CHANGECHV_CNF)
      {
        api_SimChangeCHVCnf_t* pSimChangeCHVCnf = (api_SimChangeCHVCnf_t*)nEvParam;
        CFW_GetUTI(hSimChangePwAo, &nUTI);

#ifdef CFW_MULTI_SIM
        if ((0x90 == pSimChangeCHVCnf->Sw1) && (00 == pSimChangeCHVCnf->Sw2))
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Change CHV SUCCESS\n");

          if (ERR_CFW_QUEUE_FULL == CFW_PostNotifyEvent(EV_CFW_SIM_CHANGE_PWD_RSP,
                                                        (UINT32)0, 0, nUTI | (nSimID << 24), 0))
          {
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error QUEUE HAVE FULLED\n"));
          }
        }
        else
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error ChangeCHV\n");
          nErrCode = Sim_ParseSW1SW2(pSimChangeCHVCnf->Sw1, pSimChangeCHVCnf->Sw2, nSimID);
          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_CHANGE_PWD_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0))
          {
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error QUEUE HAVE FULLED\n"));
          }
        }

#else

        if ((0x90 == pSimChangeCHVCnf->Sw1) && (00 == pSimChangeCHVCnf->Sw2))
        {
          CSW_TRACE(CFW_SIM_TS_ID, "ChangeCHV SUCCESS\n");
          if (ERR_CFW_QUEUE_FULL == CFW_PostNotifyEvent(EV_CFW_SIM_CHANGE_PWD_RSP, (UINT32)0, 0, nUTI, 0))
          {
            // =====================================================
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error QUEUE HAVE FULLED\n"));

            // =====================================================
          }
        }
        else
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error ChangeCHV\n");
          nErrCode = Sim_ParseSW1SW2(pSimChangeCHVCnf->Sw1, pSimChangeCHVCnf->Sw2);
          if (ERR_CFW_QUEUE_FULL == CFW_PostNotifyEvent(EV_CFW_SIM_CHANGE_PWD_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0))
          {
            // =====================================================
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Error QUEUE HAVE FULLED\n"));

            // =====================================================
          }
        }

#endif

        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimChangePwAo);
      }
    }
    break;
  case SM_CHGPW_MMI:
    break;
  case SM_CHGPW_STACK:
    break;
  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimChangePasswordProc);
  return ERR_SUCCESS;
}

UINT32 CFW_SimGetFacilityProc(HAO hAo, CFW_EV* pEvent)
{
  HAO hSimGetFacilityAo = hAo;

  CFW_GETFACILITY* pSimGetFacilityData = NULL;

  UINT32 nEvtId  = 0;
  VOID* nEvParam = 0;

  UINT32 nUTI     = 0;
  UINT32 nErrCode = 0;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimGetFacilityProc);
  if ((UINT32)pEvent != 0xFFFFFFFF)
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }

#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hSimGetFacilityAo);
#endif
  pSimGetFacilityData = CFW_GetAoUserData(hSimGetFacilityAo);
  switch (pSimGetFacilityData->nSm_GetFac_currStatus)
  {

  case SM_GETFAC_IDLE:
    {
      UINT32 Result = 0;

      if ((UINT32)pEvent == 0xFFFFFFFF)
      {
#ifdef CFW_MULTI_SIM
        if (pSimGetFacilityData->nFac == CFW_STY_FAC_TYPE_SC)
        {
          Result = SimDedicFileStatusReq(API_DF_TELECOM, nSimID);
          if (ERR_SUCCESS != Result)
          {
            CFW_GetUTI(hSimGetFacilityAo, &nUTI);
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimDedicFileStatusReq return 0x%x \n", Result);
            CFW_PostNotifyEvent(EV_CFW_SIM_GET_FACILITY_LOCK_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimGetFacilityAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetFacilityProc);
            return ERR_SUCCESS;
          }

          pSimGetFacilityData->nSm_GetFac_prevStatus = pSimGetFacilityData->nSm_GetFac_currStatus;
          pSimGetFacilityData->nSm_GetFac_currStatus = SM_GETFAC_DEDICATED;
        }
        else if (pSimGetFacilityData->nFac == CFW_STY_FAC_TYPE_FD)
        {
          Result = SimElemFileStatusReq(API_SIM_EF_ADN, nSimID);
          if (ERR_SUCCESS != Result)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimDedicFileStatusReq return 0x%x \n", Result);
            CFW_GetUTI(hSimGetFacilityAo, &nUTI);

            CFW_PostNotifyEvent(EV_CFW_SIM_GET_FACILITY_LOCK_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimGetFacilityAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetFacilityProc);
            return ERR_SUCCESS;
          }

          pSimGetFacilityData->nSm_GetFac_prevStatus = pSimGetFacilityData->nSm_GetFac_currStatus;
          pSimGetFacilityData->nSm_GetFac_currStatus = SM_GETFAC_ELEMENTARY;
        }
#else
        if (pSimGetFacilityData->nFac == CFW_STY_FAC_TYPE_SC)
        {
          Result = SimDedicFileStatusReq(API_DF_TELECOM);
          if (ERR_SUCCESS != Result)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimDedicFileStatusReq return 0x%x \n", Result);
            CFW_GetUTI(hSimGetFacilityAo, &nUTI);
            CFW_PostNotifyEvent(EV_CFW_SIM_GET_FACILITY_LOCK_RSP, Result, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimGetFacilityAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetFacilityProc);
            return ERR_SUCCESS;
          }

          pSimGetFacilityData->nSm_GetFac_prevStatus = pSimGetFacilityData->nSm_GetFac_currStatus;
          pSimGetFacilityData->nSm_GetFac_currStatus = SM_GETFAC_DEDICATED;
        }
        else if (pSimGetFacilityData->nFac == CFW_STY_FAC_TYPE_FD)
        {
          Result = SimElemFileStatusReq(API_SIM_EF_ADN);
          if (ERR_SUCCESS != Result)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
            CFW_GetUTI(hSimGetFacilityAo, &nUTI);
            CFW_PostNotifyEvent(EV_CFW_SIM_GET_FACILITY_LOCK_RSP, Result, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimGetFacilityAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetFacilityProc);
            return ERR_SUCCESS;
          }

          pSimGetFacilityData->nSm_GetFac_prevStatus = pSimGetFacilityData->nSm_GetFac_currStatus;
          pSimGetFacilityData->nSm_GetFac_currStatus = SM_GETFAC_ELEMENTARY;
        }

#endif

        CFW_SetAoProcCode(hSimGetFacilityAo, CFW_AO_PROC_CODE_CONSUMED);
      }
      else
        CFW_SetAoProcCode(hSimGetFacilityAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_GETFAC_DEDICATED:
    {
      if (nEvtId == API_SIM_DEDICFILESTATUS_CNF)
      {
        UINT8 nStatus = 0;

        api_SimDedicFileStatusCnf_t* pSimDedicFileStatusCnf = (api_SimDedicFileStatusCnf_t*)nEvParam;

#ifdef CFW_MULTI_SIM
        if (((0x90 == pSimDedicFileStatusCnf->Sw1) && (00 == pSimDedicFileStatusCnf->Sw2))
            || (0x91 == pSimDedicFileStatusCnf->Sw1))
        {
          UINT32 Remains = 0;

          SIM_CHV_PARAM* pG_Chv_Param;
          Remains = SimParseDedicatedStatus(pSimDedicFileStatusCnf->Data, nSimID);
          CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);

          CFW_GetUTI(hSimGetFacilityAo, &nUTI);
          if (pG_Chv_Param->nPin1Status == CHV_DISABLE)
            nStatus = 0;
          else
            nStatus = 1;
          CSW_TRACE(CFW_SIM_TS_ID, "DedicatedStatus %x\n", nStatus);

          if (ERR_CFW_QUEUE_FULL == CFW_PostNotifyEvent(EV_CFW_SIM_GET_FACILITY_LOCK_RSP, (UINT32)nStatus,
                                                        Remains, nUTI | (nSimID << 24), 0))
          {
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));
          }
        }
        else
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error DedicatedStatus\n");

          nErrCode = Sim_ParseSW1SW2(pSimDedicFileStatusCnf->Sw1, pSimDedicFileStatusCnf->Sw2, nSimID);
          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_GET_FACILITY_LOCK_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0))
          {
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));
          }

        }
#else
        if (((0x90 == pSimDedicFileStatusCnf->Sw1) && (00 == pSimDedicFileStatusCnf->Sw2))
            || (0x91 == pSimDedicFileStatusCnf->Sw1))
        {
          UINT32 Remains = 0;

          SIM_CHV_PARAM* pG_Chv_Param;
          Remains = SimParseDedicatedStatus(pSimDedicFileStatusCnf->Data);
          CFW_CfgSimGetChvParam(&pG_Chv_Param);

          CFW_GetUTI(hSimGetFacilityAo, &nUTI);
          if (pG_Chv_Param->nPin1Status == CHV_DISABLE)
            nStatus = 0;
          else
            nStatus = 1;
          CSW_TRACE(CFW_SIM_TS_ID, "DedicatedStatus %x\n", nStatus);
          if (ERR_CFW_QUEUE_FULL == CFW_PostNotifyEvent(EV_CFW_SIM_GET_FACILITY_LOCK_RSP, (UINT32)nStatus,
                                                        Remains, nUTI, 0))
          {
            // =====================================================
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));

            // =====================================================
          }
        }
        else
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error DedicatedStatus\n");
          nErrCode = Sim_ParseSW1SW2(pSimDedicFileStatusCnf->Sw1, pSimDedicFileStatusCnf->Sw2);
          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_GET_FACILITY_LOCK_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0))
          {
            // =====================================================
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));

            // =====================================================
          }

        }

#endif

        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimGetFacilityAo);
      }
      else
        CFW_SetAoProcCode(hSimGetFacilityAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_GETFAC_ELEMENTARY:
    {
      UINT8 nStatus = 0;

      if (nEvtId == API_SIM_ELEMFILESTATUS_CNF)
      {
        api_SimElemFileStatusCnf_t* pSimElemFileStatusCnf = (api_SimElemFileStatusCnf_t*)nEvParam;
        CFW_GetUTI(hSimGetFacilityAo, &nUTI);

#ifdef CFW_MULTI_SIM
        if (((0x90 == pSimElemFileStatusCnf->Sw1) && (00 == pSimElemFileStatusCnf->Sw2))
            || (0x91 == pSimElemFileStatusCnf->Sw1))
        {
          if ((pSimElemFileStatusCnf->Data[11] & 0x01) == 0)
            nStatus = 1;
          else
            nStatus = 0;

          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_GET_FACILITY_LOCK_RSP, (UINT32)nStatus, 0, nUTI | (nSimID << 24), 0))
          {
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));
          }
        }
        else
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error Elementary\n");
          nErrCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2, nSimID);
          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_GET_FACILITY_LOCK_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0))
          {
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));
          }
        }
#else
        if (((0x90 == pSimElemFileStatusCnf->Sw1) && (00 == pSimElemFileStatusCnf->Sw2))
            || (0x91 == pSimElemFileStatusCnf->Sw1))
        {
          if ((pSimElemFileStatusCnf->Data[11] & 0x01) == 0)
            nStatus = 1;
          else
            nStatus = 0;
          if (ERR_CFW_QUEUE_FULL == CFW_PostNotifyEvent(EV_CFW_SIM_GET_FACILITY_LOCK_RSP, (UINT32)nStatus, 0, nUTI, 0))
          {
            // =====================================================
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));

            // =====================================================
          }
        }
        else
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error Elementary\n");
          nErrCode = Sim_ParseSW1SW2(pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2);
          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_GET_FACILITY_LOCK_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0))
          {
            // =====================================================
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));

            // =====================================================
          }
        }
#endif

        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimGetFacilityAo);
      }
      else
        CFW_SetAoProcCode(hSimGetFacilityAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_GETFAC_MMI:
    break;
  case SM_GETFAC_STACK:
    break;
  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimGetFacilityProc);
  return ERR_SUCCESS;
}

UINT32 CFW_SimSetFacilityProc(HAO hAo, CFW_EV* pEvent)
{
  HAO hSimSetFacilityAo = hAo;

  // CSW_TRACE(BASE_BAL_TS_ID, "Func %s Line %d 0x%x\n",__func__,__LINE__, pEvent->nEventId);

  CFW_SETFACILITY* pSimSetFacilityData = NULL;

  UINT32 nEvtId   = 0;
  VOID* nEvParam  = 0;
  UINT32 nStatus  = 0;
  UINT32 nUTI     = 0;
  UINT32 nErrCode = 0;

  if ((UINT32)pEvent != 0xFFFFFFFF)
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }

#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hSimSetFacilityAo);
#endif
  pSimSetFacilityData = CFW_GetAoUserData(hSimSetFacilityAo);
  switch (pSimSetFacilityData->nSm_SetFac_currStatus)
  {
  case SM_SETFAC_IDLE:
    {
      if ((UINT32)pEvent == 0xFFFFFFFF)
      {
#ifdef CFW_MULTI_SIM
        if (pSimSetFacilityData->nFac == CFW_STY_FAC_TYPE_SC)
        {
          UINT32 Result = 0;

          if (pSimSetFacilityData->nMode == 1)
          {
            Result = SimEnableCHVReq(pSimSetFacilityData->pBufPwd, nSimID);
            if (ERR_SUCCESS != Result)
            {
              CFW_GetUTI(hSimSetFacilityAo, &nUTI);
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimEnableCHVReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
              return ERR_SUCCESS;
            }

            pSimSetFacilityData->nSm_SetFac_prevStatus = pSimSetFacilityData->nSm_SetFac_currStatus;
            pSimSetFacilityData->nSm_SetFac_currStatus = SM_SETFAC_ENABLECHV;
          }
          else  // if(pSimSetFacilityData.nMode==0)
          {
            Result = SimDisableCHVReq(pSimSetFacilityData->pBufPwd, nSimID);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimDisableCHVReq return 0x%x \n", Result);

              CFW_GetUTI(hSimSetFacilityAo, &nUTI);
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
              return ERR_SUCCESS;
            }

            pSimSetFacilityData->nSm_SetFac_prevStatus = pSimSetFacilityData->nSm_SetFac_currStatus;
            pSimSetFacilityData->nSm_SetFac_currStatus = SM_SETFAC_DISABLECHV;
          }
        }
        else if (pSimSetFacilityData->nFac == CFW_STY_FAC_TYPE_FD)
        {
          SIM_CHV_PARAM* pG_Chv_Param = NULL;
          CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
          if (pG_Chv_Param->nPin2Status == CHV_NOVERIFY)
            pG_Chv_Param->bCHV2VerifyReq = TRUE;

          if (pSimSetFacilityData->nPwdSize == 0)
          {
            if (pSimSetFacilityData->nMode == 0)
            {
              UINT32 Result = SimRehabilitateReq(API_SIM_EF_ADN, nSimID);

              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimRehabilitateReq return 0x%x \n", Result);

                CFW_GetUTI(hSimSetFacilityAo, &nUTI);
                CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
                return ERR_SUCCESS;
              }

              pSimSetFacilityData->nSm_SetFac_prevStatus = pSimSetFacilityData->nSm_SetFac_currStatus;
              pSimSetFacilityData->nSm_SetFac_currStatus = SM_SETFAC_REHABILITATE;
            }
            else  // if(pSimSetFacilityData->nMode==1)
            {
              UINT32 Result = SimInvalidateReq(API_SIM_EF_ADN, nSimID);

              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimInvalidateReq return 0x%x \n", Result);

                CFW_GetUTI(hSimSetFacilityAo, &nUTI);
                CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
                return ERR_SUCCESS;
              }

              pSimSetFacilityData->nSm_SetFac_prevStatus = pSimSetFacilityData->nSm_SetFac_currStatus;
              pSimSetFacilityData->nSm_SetFac_currStatus = SM_SETFAC_INVALIDATE;
            }
          }
          else
          {
            UINT32 Result = 0;

            pG_Chv_Param = NULL;
            CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
            Result = SimVerifyCHVReq(pSimSetFacilityData->pBufPwd, CHV2, nSimID);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimVerifyCHVReq return 0x%x \n", Result);

              CFW_GetUTI(hSimSetFacilityAo, &nUTI);
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
              return ERR_SUCCESS;
            }

            pSimSetFacilityData->nSm_SetFac_prevStatus = pSimSetFacilityData->nSm_SetFac_currStatus;
            pSimSetFacilityData->nSm_SetFac_currStatus = SM_SETFAC_VERIFYCHV;
          }
        }
#else
        if (pSimSetFacilityData->nFac == CFW_STY_FAC_TYPE_SC)
        {
          UINT32 Result = 0;

          if (pSimSetFacilityData->nMode == 1)
          {
            Result = SimEnableCHVReq(pSimSetFacilityData->pBufPwd);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimEnableCHVReq return 0x%x \n", Result);

              CFW_GetUTI(hSimSetFacilityAo, &nUTI);
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, Result, 0, nUTI, 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
              return ERR_SUCCESS;
            }

            pSimSetFacilityData->nSm_SetFac_prevStatus = pSimSetFacilityData->nSm_SetFac_currStatus;
            pSimSetFacilityData->nSm_SetFac_currStatus = SM_SETFAC_ENABLECHV;
          }
          else  // if(pSimSetFacilityData.nMode==0)
          {
            Result = SimDisableCHVReq(pSimSetFacilityData->pBufPwd);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimDisableCHVReq return 0x%x \n", Result);

              CFW_GetUTI(hSimSetFacilityAo, &nUTI);
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, Result, 0, nUTI, 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
              return ERR_SUCCESS;
            }

            pSimSetFacilityData->nSm_SetFac_prevStatus = pSimSetFacilityData->nSm_SetFac_currStatus;
            pSimSetFacilityData->nSm_SetFac_currStatus = SM_SETFAC_DISABLECHV;
          }
        }
        else if (pSimSetFacilityData->nFac == CFW_STY_FAC_TYPE_FD)
        {
          SIM_CHV_PARAM* pG_Chv_Param = NULL;
          CFW_CfgSimGetChvParam(&pG_Chv_Param);
          if (pG_Chv_Param->nPin2Status == CHV_NOVERIFY)
            pG_Chv_Param->bCHV2VerifyReq = TRUE;

          if (pSimSetFacilityData->nPwdSize == 0)
          {
            if (pSimSetFacilityData->nMode == 0)
            {
              UINT32 Result = SimRehabilitateReq(API_SIM_EF_ADN);

              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimRehabilitateReq return 0x%x \n", Result);

                CFW_GetUTI(hSimSetFacilityAo, &nUTI);
                CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, Result, 0, nUTI, 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
                return ERR_SUCCESS;
              }

              pSimSetFacilityData->nSm_SetFac_prevStatus = pSimSetFacilityData->nSm_SetFac_currStatus;
              pSimSetFacilityData->nSm_SetFac_currStatus = SM_SETFAC_REHABILITATE;
            }
            else  // if(pSimSetFacilityData->nMode==1)
            {
              UINT32 Result = SimInvalidateReq(API_SIM_EF_ADN);

              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimInvalidateReq return 0x%x \n", Result);

                CFW_GetUTI(hSimSetFacilityAo, &nUTI);
                CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, Result, 0, nUTI, 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
                return ERR_SUCCESS;
              }

              pSimSetFacilityData->nSm_SetFac_prevStatus = pSimSetFacilityData->nSm_SetFac_currStatus;
              pSimSetFacilityData->nSm_SetFac_currStatus = SM_SETFAC_INVALIDATE;
            }
          }
          else
          {
            UINT32 Result = 0;

            pG_Chv_Param = NULL;

            CFW_CfgSimGetChvParam(&pG_Chv_Param);
            Result = SimVerifyCHVReq(pSimSetFacilityData->pBufPwd, CHV2);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimVerifyCHVReq return 0x%x \n", Result);

              CFW_GetUTI(hSimSetFacilityAo, &nUTI);
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, Result, 0, nUTI, 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
              return ERR_SUCCESS;
            }

            pSimSetFacilityData->nSm_SetFac_prevStatus = pSimSetFacilityData->nSm_SetFac_currStatus;
            pSimSetFacilityData->nSm_SetFac_currStatus = SM_SETFAC_VERIFYCHV;
          }
        }

#endif

        CFW_SetAoProcCode(hSimSetFacilityAo, CFW_AO_PROC_CODE_CONSUMED);
      }
      else
        CFW_SetAoProcCode(hSimSetFacilityAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
      nStatus = ERR_SUCCESS;
    }
    break;
  case SM_SETFAC_ENABLECHV:
    {
      if (nEvtId == API_SIM_ENABLECHV_CNF)
      {
        api_SimEnableCHVCnf_t* pSimEnableCHVCnf = (api_SimEnableCHVCnf_t*)nEvParam;
        CFW_GetUTI(hSimSetFacilityAo, &nUTI);

#ifdef CFW_MULTI_SIM
        if (((0x90 == pSimEnableCHVCnf->Sw1) && (00 == pSimEnableCHVCnf->Sw2)) || (0x91 == pSimEnableCHVCnf->Sw1))
        {
          SIM_CHV_PARAM* pG_Chv_Param;

          CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
          pG_Chv_Param->nPin1Status = CHV_VERIFY;

          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, (UINT32)0, 0, nUTI | (nSimID << 24), 0))
          {
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));
          }
        }
        else
        {
          nErrCode = Sim_ParseSW1SW2(pSimEnableCHVCnf->Sw1, pSimEnableCHVCnf->Sw2, nSimID);
          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0))
          {
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));
          }

        }
#else
        if (((0x90 == pSimEnableCHVCnf->Sw1) && (00 == pSimEnableCHVCnf->Sw2)) || (0x91 == pSimEnableCHVCnf->Sw1))
        {
          SIM_CHV_PARAM* pG_Chv_Param;
          CFW_CfgSimGetChvParam(&pG_Chv_Param);
          pG_Chv_Param->nPin1Status = CHV_VERIFY;
          if (ERR_CFW_QUEUE_FULL == CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, (UINT32)0, 0, nUTI, 0))
          {
            // =====================================================
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));

            // =====================================================
          }
        }
        else
        {
          nErrCode = Sim_ParseSW1SW2(pSimEnableCHVCnf->Sw1, pSimEnableCHVCnf->Sw2);
          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0))
          {
            // =====================================================
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));

            // =====================================================
          }

        }
#endif

        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
      }
      else
        CFW_SetAoProcCode(hSimSetFacilityAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
      nStatus = ERR_SUCCESS;
    }
    break;
  case SM_SETFAC_DISABLECHV:
    {
      if (nEvtId == API_SIM_DISABLECHV_CNF)
      {
        api_SimDisableCHVCnf_t* pSimDisableCHVCnf = (api_SimDisableCHVCnf_t*)nEvParam;
        CFW_GetUTI(hSimSetFacilityAo, &nUTI);

#ifdef CFW_MULTI_SIM
        if (((0x90 == pSimDisableCHVCnf->Sw1) && (00 == pSimDisableCHVCnf->Sw2)) || (0x91 == pSimDisableCHVCnf->Sw1))
        {
          SIM_CHV_PARAM* pG_Chv_Param;
          CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
          pG_Chv_Param->nPin1Status = CHV_DISABLE;
          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, (UINT32)0, 0, nUTI | (nSimID << 24), 0))
          {
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));
          }
        }
        else
        {
          nErrCode = Sim_ParseSW1SW2(pSimDisableCHVCnf->Sw1, pSimDisableCHVCnf->Sw2, nSimID);
          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0))
          {
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));
          }
        }
#else
        if (((0x90 == pSimDisableCHVCnf->Sw1) && (00 == pSimDisableCHVCnf->Sw2)) || (0x91 == pSimDisableCHVCnf->Sw1))
        {
          SIM_CHV_PARAM* pG_Chv_Param;
          CFW_CfgSimGetChvParam(&pG_Chv_Param);
          pG_Chv_Param->nPin1Status = CHV_DISABLE;

          if (ERR_CFW_QUEUE_FULL == CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, (UINT32)0, 0, nUTI, 0))
          {
            // =====================================================
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));

            // =====================================================
          }
        }
        else
        {
          nErrCode = Sim_ParseSW1SW2(pSimDisableCHVCnf->Sw1, pSimDisableCHVCnf->Sw2);
          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0))
          {
            // =====================================================
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));

            // =====================================================
          }
        }
#endif

        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
      }
      else
        CFW_SetAoProcCode(hSimSetFacilityAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
      nStatus = ERR_SUCCESS;
    }
    break;
  case SM_SETFAC_REHABILITATE:
    {
      if (nEvtId == API_SIM_REHABILITATE_CNF)
      {
        api_SimRehabilitateCnf_t* SimRehabilitateCnf = (api_SimRehabilitateCnf_t*)nEvParam;
        CFW_GetUTI(hSimSetFacilityAo, &nUTI);

#ifdef CFW_MULTI_SIM
        if (((0x90 == SimRehabilitateCnf->Sw1) && (00 == SimRehabilitateCnf->Sw2)) || (0x91 == SimRehabilitateCnf->Sw1))
        {
          // SIM_CHV_PARAM* pG_Chv_Param = NULL;
          SIM_PBK_PARAM* pG_Pbk_Param = NULL;
          CFW_CfgSimGetPbkParam(&pG_Pbk_Param, nSimID);
          pG_Pbk_Param->bADNStatus = EF_ENABLE;

          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, (UINT32)0, 0, nUTI | (nSimID << 24), 0))
          {
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));
          }
        }
        else
        {
          nErrCode = Sim_ParseSW1SW2(SimRehabilitateCnf->Sw1, SimRehabilitateCnf->Sw2, nSimID);
          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0))
          {
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));
          }
        }
#else
        if (((0x90 == SimRehabilitateCnf->Sw1) && (00 == SimRehabilitateCnf->Sw2)) || (0x91 == SimRehabilitateCnf->Sw1))
        {
          // SIM_CHV_PARAM* pG_Chv_Param = NULL;
          SIM_PBK_PARAM* pG_Pbk_Param = NULL;
          CFW_CfgSimGetPbkParam(&pG_Pbk_Param);
          pG_Pbk_Param->bADNStatus = EF_ENABLE;

          if (ERR_CFW_QUEUE_FULL == CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, (UINT32)0, 0, nUTI, 0))
          {
            // =====================================================
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));

            // =====================================================
          }
        }
        else
        {
          nErrCode = Sim_ParseSW1SW2(SimRehabilitateCnf->Sw1, SimRehabilitateCnf->Sw2);
          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0))
          {
            // =====================================================
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));

            // =====================================================
          }
        }
#endif

        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
      }
      else
        CFW_SetAoProcCode(hSimSetFacilityAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
      nStatus = ERR_SUCCESS;
    }
    break;
  case SM_SETFAC_INVALIDATE:
    {
      if (nEvtId == API_SIM_INVALIDATE_CNF)
      {
        api_SimInvalidateCnf_t* pSimInvalidateCnf = (api_SimInvalidateCnf_t*)nEvParam;
        CFW_GetUTI(hSimSetFacilityAo, &nUTI);

#ifdef CFW_MULTI_SIM
        if (((0x90 == pSimInvalidateCnf->Sw1) && (00 == pSimInvalidateCnf->Sw2)) || (0x91 == pSimInvalidateCnf->Sw1))
        {
          // SIM_CHV_PARAM* pG_Chv_Param = NULL;
          SIM_PBK_PARAM* pG_Pbk_Param = NULL;
          CFW_CfgSimGetPbkParam(&pG_Pbk_Param, nSimID);
          pG_Pbk_Param->bADNStatus = EF_DISABLE;

          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, (UINT32)0, 0, nUTI | (nSimID << 24), 0))
          {

            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));
          }
        }
        else
        {
          nErrCode = Sim_ParseSW1SW2(pSimInvalidateCnf->Sw1, pSimInvalidateCnf->Sw2, nSimID);
          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0))
          {
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));
          }
        }
#else
        if (((0x90 == pSimInvalidateCnf->Sw1) && (00 == pSimInvalidateCnf->Sw2)) || (0x91 == pSimInvalidateCnf->Sw1))
        {
          // SIM_CHV_PARAM* pG_Chv_Param = NULL;
          SIM_PBK_PARAM* pG_Pbk_Param = NULL;
          CFW_CfgSimGetPbkParam(&pG_Pbk_Param);
          pG_Pbk_Param->bADNStatus = EF_DISABLE;

          if (ERR_CFW_QUEUE_FULL == CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, (UINT32)0, 0, nUTI, 0))
          {
            // =====================================================
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));

            // =====================================================
          }
        }
        else
        {
          nErrCode = Sim_ParseSW1SW2(pSimInvalidateCnf->Sw1, pSimInvalidateCnf->Sw2);
          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0))
          {
            // =====================================================
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));

            // =====================================================
          }
        }
#endif

        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
      }
      else
        CFW_SetAoProcCode(hSimSetFacilityAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
      nStatus = ERR_SUCCESS;
    }
    break;
  case SM_SETFAC_VERIFYCHV:
    {
      if (nEvtId == API_SIM_VERIFYCHV_CNF)
      {
        SIM_CHV_PARAM* pG_Chv_Param;

        api_SimVerifyCHVCnf_t* pSimVerifyCHVCnf = (api_SimVerifyCHVCnf_t*)nEvParam;
        CFW_GetUTI(hSimSetFacilityAo, &nUTI);

#ifdef CFW_MULTI_SIM
        CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
        if (((0x90 == pSimVerifyCHVCnf->Sw1) && (0x00 == pSimVerifyCHVCnf->Sw2)) || (0x91 == pSimVerifyCHVCnf->Sw1))
        {
          pG_Chv_Param->bCHV2VerifyReq = FALSE;
          pG_Chv_Param->nPin2Status    = CHV_VERIFY;
          if (pSimSetFacilityData->nMode == 0)
          {
            UINT32 Result = SimRehabilitateReq(API_SIM_EF_ADN, nSimID);

            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimRehabilitateReq return 0x%x \n", Result);
              CFW_GetUTI(hSimSetFacilityAo, &nUTI);
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
              return ERR_SUCCESS;
            }
            pSimSetFacilityData->nSm_SetFac_prevStatus = pSimSetFacilityData->nSm_SetFac_currStatus;
            pSimSetFacilityData->nSm_SetFac_currStatus = SM_SETFAC_REHABILITATE;
          }
          else  // if(pSimSetFacilityData->nMode==1)
          {
            UINT32 Result = SimInvalidateReq(API_SIM_EF_ADN, nSimID);

            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimInvalidateReq return 0x%x \n", Result);
              CFW_GetUTI(hSimSetFacilityAo, &nUTI);
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, Result, 0, nUTI | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
              return ERR_SUCCESS;
            }

            pSimSetFacilityData->nSm_SetFac_prevStatus = pSimSetFacilityData->nSm_SetFac_currStatus;
            pSimSetFacilityData->nSm_SetFac_currStatus = SM_SETFAC_INVALIDATE;
          }
          pG_Chv_Param->nPin2Status = CHV_VERIFY;
          CFW_SetAoProcCode(hSimSetFacilityAo, CFW_AO_PROC_CODE_CONSUMED);
        }
        else
        {
          nErrCode = Sim_ParseSW1SW2(pSimVerifyCHVCnf->Sw1, pSimVerifyCHVCnf->Sw2, nSimID);
          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, (UINT32)nErrCode, 0, nUTI | (nSimID << 24), 0xF0))
          {
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));
          }
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
        }
#else
        CFW_CfgSimGetChvParam(&pG_Chv_Param);
        if (((0x90 == pSimVerifyCHVCnf->Sw1) && (0x00 == pSimVerifyCHVCnf->Sw2)) || (0x91 == pSimVerifyCHVCnf->Sw1))
        {
          pG_Chv_Param->bCHV2VerifyReq = FALSE;
          pG_Chv_Param->nPin2Status    = CHV_VERIFY;
          if (pSimSetFacilityData->nMode == 0)
          {
            UINT32 Result = SimRehabilitateReq(API_SIM_EF_ADN);

            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimRehabilitateReq return 0x%x \n", Result);
              CFW_GetUTI(hSimSetFacilityAo, &nUTI);
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, Result, 0, nUTI, 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
              return ERR_SUCCESS;
            }

            pSimSetFacilityData->nSm_SetFac_prevStatus = pSimSetFacilityData->nSm_SetFac_currStatus;
            pSimSetFacilityData->nSm_SetFac_currStatus = SM_SETFAC_REHABILITATE;
          }
          else  // if(pSimSetFacilityData->nMode==1)
          {
            UINT32 Result = SimInvalidateReq(API_SIM_EF_ADN);

            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimInvalidateReq return 0x%x \n", Result);
              CFW_GetUTI(hSimSetFacilityAo, &nUTI);
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, Result, 0, nUTI, 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
              return ERR_SUCCESS;
            }

            pSimSetFacilityData->nSm_SetFac_prevStatus = pSimSetFacilityData->nSm_SetFac_currStatus;
            pSimSetFacilityData->nSm_SetFac_currStatus = SM_SETFAC_INVALIDATE;
          }
          pG_Chv_Param->nPin2Status = CHV_VERIFY;
          CFW_SetAoProcCode(hSimSetFacilityAo, CFW_AO_PROC_CODE_CONSUMED);
        }
        else
        {
          nErrCode = Sim_ParseSW1SW2(pSimVerifyCHVCnf->Sw1, pSimVerifyCHVCnf->Sw2);
          if (ERR_CFW_QUEUE_FULL ==
              CFW_PostNotifyEvent(EV_CFW_SIM_SET_FACILITY_LOCK_RSP, (UINT32)nErrCode, 0, nUTI, 0xF0))
          {
            // =====================================================
            for (;;)
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("THE cfw QUEUE HAVE FULLED.\n"));

            // =====================================================
          }
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSetFacilityAo);
        }
#endif

      }
      else
        CFW_SetAoProcCode(hSimSetFacilityAo, CFW_AO_PROC_CODE_CONSUMED);
      nStatus = ERR_SUCCESS;
    }
    break;

  case SM_SETFAC_MMI:
    break;

  case SM_SETFAC_STACK:
    break;
  }
  return ERR_SUCCESS;
}
 
