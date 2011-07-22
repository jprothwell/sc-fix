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

#ifdef CFW_MULTI_SIM

//CFW_SIM_STATUS gSimStatus[CFW_SIM_COUNT] = { CFW_SIM_STATUS_END, CFW_SIM_STATUS_END };
CFW_SIM_STATUS gSimStatus[CFW_SIM_COUNT];
VOID CFW_SetSimStatus(CFW_SIM_ID nSimID, CFW_SIM_STATUS nSimStatus)
{
  if (CFW_SIM_END > nSimID)
  {
    gSimStatus[nSimID] = nSimStatus;
  }
}

CFW_SIM_STATUS CFW_GetSimStatus(CFW_SIM_ID nSimID)
{
  if (CFW_SIM_END > nSimID)
  {
    return gSimStatus[nSimID];
  }
  return CFW_SIM_STATUS_END;
}


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

UINT32 CFW_SimInitStage3Proc(HAO hAo, CFW_EV* pEvent)
{
  HAO hSimInitializeAo = hAo;

  UINT32 nEvtId  = 0;
  VOID* nEvParam = 0;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimInitStage3Proc);

  CFW_SIMINITS3* pSimInitS3Data = NULL;


  if ((UINT32)pEvent != 0xFFFFFFFF)
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }

#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hSimInitializeAo);
#endif


  pSimInitS3Data = CFW_GetAoUserData(hSimInitializeAo);

  switch (pSimInitS3Data->nSm_SimInitS3_currStatus)
  {
  case SM_SIMINITS1_IDLE:
    {
      if ((UINT32)pEvent == 0xFFFFFFFF)
      {
        UINT32 Result = 0;

        pSimInitS3Data->nEF = API_SIM_EF_SPN;
#ifdef CFW_MULTI_SIM
        Result = SimReadBinaryReq(API_SIM_EF_SPN, 0x00, 17, nSimID);
        if (ERR_SUCCESS != Result)
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq EF_SPN return 0x%x \n", Result);
          CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);
          return ERR_SUCCESS;
        }

#else
        Result = SimReadBinaryReq(API_SIM_EF_SPN, 0x00, 17);
        if (ERR_SUCCESS != Result)
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq EF_SPN return 0x%x \n", Result);
          CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);
          return ERR_SUCCESS;
        }

#endif
        pSimInitS3Data->nSm_SimInitS3_prevStatus = pSimInitS3Data->nSm_SimInitS3_currStatus;
        pSimInitS3Data->nSm_SimInitS3_currStatus = SM_SIMINITS3_READBINARY;
        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_CONSUMED);
      }
      else
        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_SIMINITS3_ELEMENTARY:
    {
      if (nEvtId == API_SIM_ELEMFILESTATUS_CNF)
      {
        SIM_PBK_PARAM* pG_Pbk_Param;
        api_SimElemFileStatusCnf_t* pSimElemFileStatusCnf = (api_SimElemFileStatusCnf_t*)nEvParam;

#ifdef CFW_MULTI_SIM
        CFW_CfgSimGetPbkParam(&pG_Pbk_Param, nSimID);
#else
        CFW_CfgSimGetPbkParam(&pG_Pbk_Param);
#endif
        CSW_TRACE(CFW_SIM_TS_ID, "ELEMFILESTATUS SW1[0x%x] nEF[0x%x]\n", pSimElemFileStatusCnf->Sw1,pSimInitS3Data->nEF);
          CSW_TRACE(CFW_SIM_TS_ID, "pSimInitS3Data->nEF %d\n", pSimInitS3Data->nEF);

        if (((0x90 == pSimElemFileStatusCnf->Sw1) && (00 == pSimElemFileStatusCnf->Sw2))
            || (0x91 == pSimElemFileStatusCnf->Sw1))
        {
          pSimInitS3Data->nFileSize   = ((UINT16)pSimElemFileStatusCnf->Data[2] << 8) | (pSimElemFileStatusCnf->Data[3]);
          pSimInitS3Data->nRecordSize = pSimElemFileStatusCnf->Data[14];
          CSW_TRACE(CFW_SIM_TS_ID, "nRecordSize 0x%x\n", pSimInitS3Data->nRecordSize);
          CSW_TRACE(CFW_SIM_TS_ID, "pSimInitS3Data->nEF %d\n", pSimInitS3Data->nEF);

          if ((pSimInitS3Data->nEF == API_SIM_EF_AD) || (pSimInitS3Data->nEF == API_SIM_EF_SST))
          {
            //if (pSimInitS3Data->nFileSize != 0)
            {
              
#ifdef CFW_MULTI_SIM
              UINT32 Result = SimReadBinaryReq(pSimInitS3Data->nEF, 0, (UINT8)pSimInitS3Data->nFileSize, nSimID);

              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n", Result);
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);
                return ERR_SUCCESS;
              }
#else
              UINT32 Result = SimReadBinaryReq(pSimInitS3Data->nEF, 0, (UINT8)pSimInitS3Data->nFileSize);

              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n", Result);
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);
                return ERR_SUCCESS;
              }
#endif


              pSimInitS3Data->nSm_SimInitS3_prevStatus = pSimInitS3Data->nSm_SimInitS3_currStatus;
              pSimInitS3Data->nSm_SimInitS3_currStatus = SM_SIMINITS3_READBINARY; // updated by wanghb-20070711
              CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_CONSUMED);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);

              return ERR_SUCCESS;
            }
          }
          else if (pSimInitS3Data->nEF == API_SIM_EF_LOCI)
          {
#ifdef CFW_MULTI_SIM
            CSW_TRACE(CFW_SIM_TS_ID, "Data[11] & 0x01 = 0x%x \n", (pSimElemFileStatusCnf->Data[11] & 0x01));

            if ((pSimElemFileStatusCnf->Data[11] & 0x01) == 1)
            {
              UINT32 Result = 0;

              pSimInitS3Data->nEF = API_SIM_EF_IMSI;
              Result            = SimElemFileStatusReq(API_SIM_EF_IMSI, nSimID);
              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);
                return ERR_SUCCESS;
              }
              else
              {

              }
            }
            else
            {
              UINT32 Result = 0;

              CFW_PROFILE* pMeProfile = NULL;
              CFW_SimGetMeProfile(&pMeProfile, nSimID);
              CSW_TRACE(CFW_SIM_TS_ID, "pMeProfile->nStart_Profile= 0x%x \n", pMeProfile->nStart_Profile);
              if ((pMeProfile->nStart_Profile & 0x04) == 0x04)  // ME support CC.
              {
                if ((pMeProfile->nStart_Profile & 0x03) == 0x03)
                {
                  pSimInitS3Data->nMEStatus = 1;  // 1: ALL SERVICES ENABLED
                }
                else
                {
                  CSW_TRACE(CFW_SIM_TS_ID, "Error SIM Card Abnormity\n");
                  SIM_MISC_PARAM* pG_Misc_Param = NULL;
                  CFW_CfgSimGetMicParam(&pG_Misc_Param, nSimID);
                  pG_Misc_Param->nSimMode = CFW_SIM_CARD_ABNORMITY;
                  CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_ABNORMITY, 0xFF,
                                      GENERATE_SHELL_UTI() | (nSimID << 24), SIM_INIT_EV_STAGE3);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                  return ERR_SUCCESS;
                }
              }
              else
              {
                if (((pMeProfile->nStart_Profile & 0x01) == 0x01) && (pG_Pbk_Param->bADNStatus == EF_DISABLE))
                {
                  pSimInitS3Data->nMEStatus = 0;
                }
                else
                {
                  SIM_MISC_PARAM* pG_Misc_Param = NULL;
                  CFW_CfgSimGetMicParam(&pG_Misc_Param, nSimID);
                  pG_Misc_Param->nSimMode = CFW_SIM_CARD_ABNORMITY;
                  CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_ABNORMITY, 0xFF,
                                      GENERATE_SHELL_UTI() | (nSimID << 24), SIM_INIT_EV_STAGE3);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                  return ERR_SUCCESS;
                }
              }
              Result = SimRehabilitateReq(API_SIM_EF_LOCI, nSimID);
              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimRehabilitateReq return 0x%x \n", Result);
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);

                return ERR_SUCCESS;
              }
              else
              {

              }
              pSimInitS3Data->nSm_SimInitS3_prevStatus = pSimInitS3Data->nSm_SimInitS3_currStatus;
              pSimInitS3Data->nSm_SimInitS3_currStatus = SM_SIMINITS3_REHABILITATE;
            }
#else
            if ((pSimElemFileStatusCnf->Data[11] & 0x01) == 1)
            {
              UINT32 Result = 0;

              pSimInitS3Data->nEF = API_SIM_EF_IMSI;
              Result              = SimElemFileStatusReq(API_SIM_EF_IMSI);
              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);
                return ERR_SUCCESS;
              }
            }
            else
            {
              UINT32 Result = 0;

              CFW_PROFILE* pMeProfile = NULL;
              CFW_SimGetMeProfile(&pMeProfile);
              if ((pMeProfile->nStart_Profile & 0x04) == 0x04)  // ME support CC.
              {
                if ((pMeProfile->nStart_Profile & 0x03) == 0x03)
                {
                  pSimInitS3Data->nMEStatus = 1;  // 1: ALL SERVICES ENABLED
                }
                else
                {
                  CSW_TRACE(CFW_SIM_TS_ID, "Error SIM Card Abnormity\n");
                  SIM_MISC_PARAM* pG_Misc_Param = NULL;
                  CFW_CfgSimGetMicParam(&pG_Misc_Param);
                  pG_Misc_Param->nSimMode = CFW_SIM_CARD_ABNORMITY;
                  CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_ABNORMITY, 0xFF, GENERATE_SHELL_UTI(),
                                      SIM_INIT_EV_STAGE3);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                  return ERR_SUCCESS;
                }
              }
              else
              {
                if (((pMeProfile->nStart_Profile & 0x01) == 0x01) && (pG_Pbk_Param->bADNStatus == EF_DISABLE))
                {
                  pSimInitS3Data->nMEStatus = 0;
                }
                else
                {
                  CSW_TRACE(CFW_SIM_TS_ID, "Error SIM Card Abnormity\n");
                  SIM_MISC_PARAM* pG_Misc_Param = NULL;
                  CFW_CfgSimGetMicParam(&pG_Misc_Param);
                  pG_Misc_Param->nSimMode = CFW_SIM_CARD_ABNORMITY;
                  CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_ABNORMITY, 0xFF, GENERATE_SHELL_UTI(),
                                      SIM_INIT_EV_STAGE3);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                  return ERR_SUCCESS;
                }
              }
              Result = SimRehabilitateReq(API_SIM_EF_LOCI);
              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimRehabilitateReq return 0x%x \n", Result);
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);

                return ERR_SUCCESS;
              }
              else
              {

              }
              pSimInitS3Data->nSm_SimInitS3_prevStatus = pSimInitS3Data->nSm_SimInitS3_currStatus;
              pSimInitS3Data->nSm_SimInitS3_currStatus = SM_SIMINITS3_REHABILITATE;
            }
#endif


            CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_CONSUMED);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);

            return ERR_SUCCESS;
          }
          else if (pSimInitS3Data->nEF == API_SIM_EF_IMSI)
          {
#ifdef CFW_MULTI_SIM

            if ((pSimElemFileStatusCnf->Data[11] & 0x01) == 1)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "IMSI[11] 0x%x\n",pSimElemFileStatusCnf->Data[11]);
              if (CFW_SimGetPS() == TRUE)
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_ME_PS, 0, GENERATE_SHELL_UTI() | (nSimID << 24),
                                    SIM_INIT_EV_STAGE3);
              else if (CFW_SimGetPF() == TRUE)
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_ME_PF, 0, GENERATE_SHELL_UTI() | (nSimID << 24),
                                    SIM_INIT_EV_STAGE3);
              else
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Normal Sim Card %d\n",nSimID);
                SIM_MISC_PARAM* pG_Mic_Param;
                CFW_CfgSimGetMicParam(&pG_Mic_Param, nSimID);
                CFW_RegisterCreateAoProc(API_SIM_STATUSERROR_IND, SimSpySimCard);
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, pG_Mic_Param->nSimMode,  // modify by wanghb from CFW_SIM_CARD_NORMAL to pG_Mic_Param->nSimMode
                                    pG_Pbk_Param->bBDNStatus << 1 | pG_Pbk_Param->bADNStatus,
                                    GENERATE_SHELL_UTI() | (nSimID << 24), SIM_INIT_EV_GSM);
              }
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
            }
            else
            {
              UINT32 Result = SimRehabilitateReq(API_SIM_EF_IMSI, nSimID);

              if (ERR_SUCCESS != Result)
              {

                CSW_TRACE(CFW_SIM_TS_ID, "Error SimRehabilitateReq return 0x%x \n", Result);
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);

                return ERR_SUCCESS;
              }
              else
              {

              }
              pSimInitS3Data->nSm_SimInitS3_prevStatus = pSimInitS3Data->nSm_SimInitS3_currStatus;
              pSimInitS3Data->nSm_SimInitS3_currStatus = SM_SIMINITS3_REHABILITATE;

            }
#else
            if ((pSimElemFileStatusCnf->Data[11] & 0x01) == 1)
            {
              // CFW_SendSclMessage(API_SIM_OPEN_IND, NULL);
              if (CFW_SimGetPS() == TRUE)
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_ME_PS, 0, GENERATE_SHELL_UTI(), SIM_INIT_EV_STAGE3);
              else if (CFW_SimGetPF() == TRUE)
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_ME_PF, 0, GENERATE_SHELL_UTI(), SIM_INIT_EV_STAGE3);
              else
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Normal Sim Card\n");

                // add by wanghb for special card 2007.8.22 begin
                SIM_MISC_PARAM* pG_Mic_Param;
                CFW_CfgSimGetMicParam(&pG_Mic_Param);

                // add by wanghb for special card 2007.8.22 end

                CFW_RegisterCreateAoProc(API_SIM_STATUSERROR_IND, SimSpySimCard);

                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, pG_Mic_Param->nSimMode,  // modify by wanghb from CFW_SIM_CARD_NORMAL to pG_Mic_Param->nSimMode
                                    pG_Pbk_Param->bBDNStatus << 1 | pG_Pbk_Param->bADNStatus,
                                    GENERATE_SHELL_UTI(), SIM_INIT_EV_GSM);
              }

              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
            }
            else
            {
              UINT32 Result = SimRehabilitateReq(API_SIM_EF_IMSI);

              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimRehabilitateReq return 0x%x \n", Result);
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);

                return ERR_SUCCESS;
              }
              else
              {

              }

              pSimInitS3Data->nSm_SimInitS3_prevStatus = pSimInitS3Data->nSm_SimInitS3_currStatus;
              pSimInitS3Data->nSm_SimInitS3_currStatus = SM_SIMINITS3_REHABILITATE;

            }
#endif
            CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_CONSUMED);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);

            return ERR_SUCCESS;
          }
          else if (pSimInitS3Data->nRecordSize != 0)
            pSimInitS3Data->nFileSize = pSimInitS3Data->nFileSize / pSimInitS3Data->nRecordSize;
          else
            pSimInitS3Data->nFileSize = 0;

          CSW_TRACE(CFW_SIM_TS_ID, "pSimInitS3Data->nEF 0x%x\n", pSimInitS3Data->nEF);
          if (pSimInitS3Data->nEF == API_SIM_EF_ADN)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "ADN nRecordSize 0x%x\n", pSimInitS3Data->nRecordSize);

            pG_Pbk_Param->nADNRecordSize = pSimInitS3Data->nRecordSize;
            pG_Pbk_Param->nADNTotalNum   = (UINT8)pSimInitS3Data->nFileSize;
            pG_Pbk_Param->bADNStatus     = pSimElemFileStatusCnf->Data[11] & 0x01;  // 1: EF_ADN 0 :EF_FDN
            CSW_TRACE(CFW_SIM_TS_ID, "FDN Status 0x%x\n", pG_Pbk_Param->bADNStatus);

            pSimInitS3Data->nEF = API_SIM_EF_FDN;
          }
          else if (pSimInitS3Data->nEF == API_SIM_EF_FDN)
          {
            pG_Pbk_Param->nFDNRecordSize = pSimInitS3Data->nRecordSize;
            pG_Pbk_Param->nFDNTotalNum   = (UINT8)pSimInitS3Data->nFileSize;
            pSimInitS3Data->nEF          = API_SIM_EF_BDN;
          }
          else if (pSimInitS3Data->nEF == API_SIM_EF_BDN)
          {
            // 
            // Modify by wangxj at 20060819
            // 
            pG_Pbk_Param->nBDNRecordSize = pSimInitS3Data->nRecordSize;

            pG_Pbk_Param->nBDNTotalNum = (UINT8)pSimInitS3Data->nFileSize;
            CSW_TRACE(CFW_SIM_TS_ID, "API_SIM_EF_BDN nBDNTotalNum %d\n", pG_Pbk_Param->nBDNTotalNum);
            pG_Pbk_Param->bBDNStatus = pSimElemFileStatusCnf->Data[11] & 0x01;  // 1: enable 0 :disable
            pSimInitS3Data->nEF      = API_SIM_EF_LND;
          }
          else if (pSimInitS3Data->nEF == API_SIM_EF_LND)
          {
            // 
            // Modify by wangxj at 20060819
            // 
            pG_Pbk_Param->nLNDRecordSize = pSimInitS3Data->nRecordSize;
            pG_Pbk_Param->nLNDTotalNum   = (UINT8)pSimInitS3Data->nFileSize;
            pSimInitS3Data->nEF          = API_SIM_EF_SDN;
          }
          else if (pSimInitS3Data->nEF == API_SIM_EF_SDN)
          {
            pG_Pbk_Param->nSDNRecordSize = pSimInitS3Data->nRecordSize;
            pG_Pbk_Param->nSDNTotalNum   = (UINT8)pSimInitS3Data->nFileSize;
            CSW_TRACE(CFW_SIM_TS_ID, "API_SIM_EF_SDN nSDNTotalNum %d\n", pG_Pbk_Param->nSDNTotalNum);
            pSimInitS3Data->nEF = API_SIM_EF_MSISDN;
          }
          else if (pSimInitS3Data->nEF == API_SIM_EF_MSISDN)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "EF_MSISDN\n");
            pG_Pbk_Param->nMSISDNRecordSize = pSimInitS3Data->nRecordSize;
            pG_Pbk_Param->nMSISDNTotalNum   = (UINT8)pSimInitS3Data->nFileSize;
            pSimInitS3Data->nEF             = API_SIM_EF_ECC;
          }
          else if (pSimInitS3Data->nEF == API_SIM_EF_ECC)
          {
            pG_Pbk_Param->nECCRecordSize = 3; // pSimInitS3Data->nRecordSize;
            pG_Pbk_Param->nECCTotalNum   = pSimInitS3Data->nFileSize;
            pSimInitS3Data->nEF          = API_SIM_EF_LOCI;
          }
        }
        else if ((pSimInitS3Data->nEF == API_SIM_EF_AD) ||
                 (pSimInitS3Data->nEF == API_SIM_EF_SST) ||
                 (pSimInitS3Data->nEF == API_SIM_EF_LOCI) || (pSimInitS3Data->nEF == API_SIM_EF_IMSI))
        {

          SIM_MISC_PARAM* pG_Mic_Param;

#ifdef CFW_MULTI_SIM
          CFW_CfgSimGetMicParam(&pG_Mic_Param, nSimID);
          pG_Mic_Param->nSimMode = CFW_SIM_CARD_BAD;
          CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_BAD, 0, GENERATE_SHELL_UTI() | (nSimID << 24), SIM_INIT_EV_STAGE3); // need to add error code to param2.
#else
          CFW_CfgSimGetMicParam(&pG_Mic_Param);
          pG_Mic_Param->nSimMode = CFW_SIM_CARD_BAD;
          CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_BAD, 0, GENERATE_SHELL_UTI(), SIM_INIT_EV_STAGE3);  // need to add error code to param2.
#endif
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);

          return ERR_SUCCESS;
        }

// else if(pSimInitS3Data->nEF == API_SIM_EF_AD)
// {
// pSimInitS3Data->nEF = API_SIM_EF_SST;
// }
// else if(pSimInitS3Data->nEF == API_SIM_EF_SST)
// {
// pSimInitS3Data->nEF = API_SIM_EF_ADN;
// }
        else if (pSimInitS3Data->nEF == API_SIM_EF_ADN)
        {
          pG_Pbk_Param->nADNRecordSize = 0;
          pG_Pbk_Param->nADNTotalNum   = 0;
          pG_Pbk_Param->bADNStatus     = EF_DISABLE;
          pSimInitS3Data->nEF          = API_SIM_EF_FDN;
        }
        else if (pSimInitS3Data->nEF == API_SIM_EF_FDN)
        {
          pG_Pbk_Param->nFDNRecordSize = 0;
          pG_Pbk_Param->nFDNTotalNum   = 0;
          pSimInitS3Data->nEF          = API_SIM_EF_BDN;
        }
        else if (pSimInitS3Data->nEF == API_SIM_EF_BDN)
        {
          pG_Pbk_Param->bBDNStatus = EF_DISABLE;
          pSimInitS3Data->nEF      = API_SIM_EF_LND;
        }
        else if (pSimInitS3Data->nEF == API_SIM_EF_LND)
        {
          pG_Pbk_Param->nLNDRecordSize = 0;
          pG_Pbk_Param->nLNDTotalNum   = 0;

          // 
          // Modify by wangxj at 20060819
          // 
          pSimInitS3Data->nEF = API_SIM_EF_SDN;
        }
        else if (pSimInitS3Data->nEF == API_SIM_EF_SDN)
        {
          pG_Pbk_Param->nSDNRecordSize = 0;
          pG_Pbk_Param->nSDNTotalNum   = 0;
          pSimInitS3Data->nEF          = API_SIM_EF_MSISDN;
        }
        else if (pSimInitS3Data->nEF == API_SIM_EF_MSISDN)
        {
          pG_Pbk_Param->nMSISDNRecordSize = 0;
          pG_Pbk_Param->nMSISDNTotalNum   = 0;
          pSimInitS3Data->nEF             = API_SIM_EF_ECC; // This will be remove.
        }
        else if (pSimInitS3Data->nEF == API_SIM_EF_ECC)
        {
          pG_Pbk_Param->nECCRecordSize = 0;
          pG_Pbk_Param->nECCTotalNum   = 0;
          pSimInitS3Data->nEF          = API_SIM_EF_LOCI;  // This will be remove.
        }

#ifdef CFW_MULTI_SIM
        UINT32 Result = SimElemFileStatusReq(pSimInitS3Data->nEF, nSimID);

        if (ERR_SUCCESS != Result)
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
          CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);

          return ERR_SUCCESS;
        }
#else
        UINT32 Result = SimElemFileStatusReq(pSimInitS3Data->nEF);

        if (ERR_SUCCESS != Result)
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);

          CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);

          return ERR_SUCCESS;
        }
#endif


        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_CONSUMED);
      }
      else
        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_SIMINITS3_READBINARY:
    {

      if (nEvtId == API_SIM_READBINARY_CNF)
      {
        api_SimReadBinaryCnf_t* pSimReadBinaryCnf = (api_SimReadBinaryCnf_t*)nEvParam;
        CSW_TRACE(CFW_SIM_TS_ID, "api_SimReadBinaryCnf_t: SW1 %x,SW2 %x \n", pSimReadBinaryCnf->Sw1,
                      pSimReadBinaryCnf->Sw2);
        if ((0x90 == pSimReadBinaryCnf->Sw1) && (0x00 == pSimReadBinaryCnf->Sw2))
        {
          SIM_MISC_PARAM* pG_Mic_Param;
#ifdef CFW_MULTI_SIM
          CFW_CfgSimGetMicParam(&pG_Mic_Param, nSimID);
          //CSW_TRACE(CFW_SIM_TS_ID, "Stage3 SM_SIMINITS3_READBINARY nEF(%s)\n", TS_GetEFFileName(pSimInitS3Data->nEF));

          if (pSimInitS3Data->nEF == API_SIM_EF_SPN)
          {
            
            CFW_SPN_INFO nSPNInfo = {0x00,0x00,{0x00,}};
            nSPNInfo.nSPNameLen = CFW_GetSPNLen(&(pSimReadBinaryCnf->Data[1]));
            nSPNInfo.nSPNDisplayFlag = pSimReadBinaryCnf->Data[0]&0x01;
            
            SUL_MemCopy8(nSPNInfo.nSpnName,&(pSimReadBinaryCnf->Data[1]),nSPNInfo.nSPNameLen);
            CFW_SetSimSPN(nSimID, nSPNInfo);
            
            UINT32 Result = 0;

            pSimInitS3Data->nEF = API_SIM_EF_AD;
            #ifdef CFW_MULTI_SIM
              Result = SimElemFileStatusReq(API_SIM_EF_AD, nSimID);
              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq EF_AD return 0x%x \n", Result);
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);
                return ERR_SUCCESS;
              }

            #else
              Result = SimElemFileStatusReq(API_SIM_EF_AD);
              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq EF_AD return 0x%x \n", Result);
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);
                return ERR_SUCCESS;
              }

            #endif
            pSimInitS3Data->nSm_SimInitS3_prevStatus = pSimInitS3Data->nSm_SimInitS3_currStatus;
            pSimInitS3Data->nSm_SimInitS3_currStatus = SM_SIMINITS3_ELEMENTARY;
            CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_CONSUMED);
          }
          else if (pSimInitS3Data->nEF == API_SIM_EF_AD)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "pSimReadBinaryCnf->Data[0] 0x%x \n", pSimReadBinaryCnf->Data[0]);
#ifdef CFW_MULTI_SIM
            if( pSimInitS3Data->nFileSize >= 4 )
            {
              if((pSimReadBinaryCnf->Data[3] == 2) || (pSimReadBinaryCnf->Data[3] == 3))
                CFW_SetMNCLen(pSimReadBinaryCnf->Data[3],nSimID);  
              else
                CFW_SetMNCLen(2,nSimID);  
            }
            else
            {
              CFW_SetMNCLen(2,nSimID);  
            }
#else
            if( pSimInitS3Data->nFileSize >= 4 )
            {
              if((pSimReadBinaryCnf->Data[3] == 2) || (pSimReadBinaryCnf->Data[3] == 3))
                CFW_SetMNCLen(pSimReadBinaryCnf->Data[3]);  
              else
                CFW_SetMNCLen(2);  
            }
            else
            {
              CFW_SetMNCLen(2);  
            }
#endif            
            if ((0x00 == pSimReadBinaryCnf->Data[0]) || (0xFF == pSimReadBinaryCnf->Data[0]) || (0x01 == pSimReadBinaryCnf->Data[0]))
            {
              UINT32 Result = 0;

              pG_Mic_Param->nSimMode = CFW_SIM_CARD_NORMAL;

#ifdef CFW_MULTI_SIM
              CFW_SetSimStatus(nSimID, CFW_SIM_NORMAL);
#endif

              pSimInitS3Data->nEF = API_SIM_EF_SST;
              Result              = SimElemFileStatusReq(API_SIM_EF_SST, nSimID);

              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);

                return ERR_SUCCESS;
              }
              pSimInitS3Data->nSm_SimInitS3_prevStatus = pSimInitS3Data->nSm_SimInitS3_currStatus;
              pSimInitS3Data->nSm_SimInitS3_currStatus = SM_SIMINITS3_ELEMENTARY;
            }
            else if ((pSimReadBinaryCnf->Data[0] == 0x80) || (pSimReadBinaryCnf->Data[0] == 0x81))
            {
              pG_Mic_Param->nSimMode = CFW_SIM_CARD_TEST;
#ifdef CFW_MULTI_SIM
              CFW_SetSimStatus(nSimID, CFW_SIM_TEST);
#endif

              UINT32 Result = 0;

              pSimInitS3Data->nEF = API_SIM_EF_SST;
              Result              = SimElemFileStatusReq(API_SIM_EF_SST, nSimID);
              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);
                return ERR_SUCCESS;
              }
              pSimInitS3Data->nSm_SimInitS3_prevStatus = pSimInitS3Data->nSm_SimInitS3_currStatus;
              pSimInitS3Data->nSm_SimInitS3_currStatus = SM_SIMINITS3_ELEMENTARY;
            }
            else
            {
#  ifdef CFW_MULTI_SIM
              CFW_SetSimStatus(nSimID, CFW_SIM_ABNORMAL);
#  endif
              pG_Mic_Param->nSimMode = CFW_SIM_CARD_ABNORMITY;
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_ABNORMITY, 0,
                                  GENERATE_SHELL_UTI() | (nSimID << 24), SIM_INIT_EV_STAGE3);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
            }
          }
          else if (pSimInitS3Data->nEF == API_SIM_EF_SST)
          {
            UINT8 i       = 0;
            UINT32 Result = 0;

            if (pSimInitS3Data->nFileSize > SERVICES_NUM)
              pSimInitS3Data->nFileSize = SERVICES_NUM;
            for (i = 0; i < pSimInitS3Data->nFileSize; i++)
              pG_Mic_Param->nServices[i] = pSimReadBinaryCnf->Data[i];

            if(0x03 == ( pSimReadBinaryCnf->Data[4] & 0x03 ))
            {
              CFW_SPN_INFO nSPNInfo = {0x00,0x00,{0x00,}};
              
              CFW_GetSimSPN( nSimID, &nSPNInfo );
              
              if( nSPNInfo.nSPNameLen > 0x00 )
              {
                nSPNInfo.nSPNDisplayFlag = 0x01;
              }  
              
              CFW_SetSimSPN( nSimID, nSPNInfo );
            }
            pSimInitS3Data->nEF = API_SIM_EF_ADN;
            Result              = SimElemFileStatusReq(API_SIM_EF_ADN, nSimID);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);
              return ERR_SUCCESS;
            }
            pSimInitS3Data->nSm_SimInitS3_prevStatus = pSimInitS3Data->nSm_SimInitS3_currStatus;
            pSimInitS3Data->nSm_SimInitS3_currStatus = SM_SIMINITS3_ELEMENTARY;
          }
#else
          CFW_CfgSimGetMicParam(&pG_Mic_Param);
          if (pSimInitS3Data->nEF == API_SIM_EF_SPN)
          {
            
            CFW_SPN_INFO nSPNInfo = {0x00,0x00,{0x00,}};
            nSPNInfo.nSPNameLen = CFW_GetSPNLen(&(pSimReadBinaryCnf->Data[1]));
            nSPNInfo.nSPNDisplayFlag = pSimReadBinaryCnf->Data[0]&0x01;
            SUL_MemCopy8(nSPNInfo.nSpnName,&(pSimReadBinaryCnf->Data[1]),nSPNInfo.nSPNameLen);
            CFW_SetSimSPN(nSPNInfo);
            
            UINT32 Result = 0;

            pSimInitS3Data->nEF = API_SIM_EF_AD;
              Result = SimElemFileStatusReq(API_SIM_EF_AD);
              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq EF_AD return 0x%x \n", Result);
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);
                return ERR_SUCCESS;
              }

            pSimInitS3Data->nSm_SimInitS3_prevStatus = pSimInitS3Data->nSm_SimInitS3_currStatus;
            pSimInitS3Data->nSm_SimInitS3_currStatus = SM_SIMINITS3_ELEMENTARY;
            CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_CONSUMED);
          }
          else if (pSimInitS3Data->nEF == API_SIM_EF_AD)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "pSimReadBinaryCnf->Data[0] 0x%x \n", pSimReadBinaryCnf->Data[0]);
            
            if ((0x00 == pSimReadBinaryCnf->Data[0]) || (0xFF == pSimReadBinaryCnf->Data[0]))
            {
              UINT32 Result = 0;

              pG_Mic_Param->nSimMode = CFW_SIM_CARD_NORMAL;
              pSimInitS3Data->nEF    = API_SIM_EF_SST;
              Result                 = SimElemFileStatusReq(API_SIM_EF_SST);

              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);

                return ERR_SUCCESS;
              }

              pSimInitS3Data->nSm_SimInitS3_prevStatus = pSimInitS3Data->nSm_SimInitS3_currStatus;
              pSimInitS3Data->nSm_SimInitS3_currStatus = SM_SIMINITS3_ELEMENTARY;
            }
            else if ((pSimReadBinaryCnf->Data[0] == 0x80) || (pSimReadBinaryCnf->Data[0] == 0x81))
            {
              pG_Mic_Param->nSimMode = CFW_SIM_CARD_TEST;

              // ------------------------------------------------------------
              // Add by lixp for special card 2007.8.16 start
              // ------------------------------------------------------------
              // CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              UINT32 Result = 0;

              pSimInitS3Data->nEF = API_SIM_EF_SST;
              Result              = SimElemFileStatusReq(API_SIM_EF_SST);

              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);

                return ERR_SUCCESS;
              }

              pSimInitS3Data->nSm_SimInitS3_prevStatus = pSimInitS3Data->nSm_SimInitS3_currStatus;
              pSimInitS3Data->nSm_SimInitS3_currStatus = SM_SIMINITS3_ELEMENTARY;

              // ------------------------------------------------------------
              // Add by lixp for special card 2007.8.16 end
              // ------------------------------------------------------------

            }
            else
            {
              pG_Mic_Param->nSimMode = CFW_SIM_CARD_ABNORMITY;
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_ABNORMITY, 0, GENERATE_SHELL_UTI(),
                                  SIM_INIT_EV_STAGE3);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
            }
          }
          else if (pSimInitS3Data->nEF == API_SIM_EF_SST)
          {
            UINT8 i       = 0;
            UINT32 Result = 0;

            if (pSimInitS3Data->nFileSize > SERVICES_NUM)
              pSimInitS3Data->nFileSize = SERVICES_NUM;
            for (i = 0; i < pSimInitS3Data->nFileSize; i++)
              pG_Mic_Param->nServices[i] = pSimReadBinaryCnf->Data[i];

            pSimInitS3Data->nEF = API_SIM_EF_ADN;
            Result              = SimElemFileStatusReq(API_SIM_EF_ADN);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);
              return ERR_SUCCESS;
            }

            pSimInitS3Data->nSm_SimInitS3_prevStatus = pSimInitS3Data->nSm_SimInitS3_currStatus;
            pSimInitS3Data->nSm_SimInitS3_currStatus = SM_SIMINITS3_ELEMENTARY;
          }
#endif
        }
        else
        {
          if(pSimInitS3Data->nEF == API_SIM_EF_SPN)
          {
            UINT32 Result = 0;

            pSimInitS3Data->nEF = API_SIM_EF_AD;
            #ifdef CFW_MULTI_SIM
              Result = SimElemFileStatusReq(API_SIM_EF_AD, nSimID);
              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq EF_AD return 0x%x \n", Result);
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);
                return ERR_SUCCESS;
              }

            #else
              Result = SimElemFileStatusReq(API_SIM_EF_AD);
              if (ERR_SUCCESS != Result)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq EF_AD return 0x%x \n", Result);
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);
                return ERR_SUCCESS;
              }

            #endif
            pSimInitS3Data->nSm_SimInitS3_prevStatus = pSimInitS3Data->nSm_SimInitS3_currStatus;
            pSimInitS3Data->nSm_SimInitS3_currStatus = SM_SIMINITS3_ELEMENTARY;
            CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_CONSUMED);
          }
        }
        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_CONSUMED);
      }
      else
        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_SIMINITS3_REHABILITATE:
    {
      if (nEvtId == API_SIM_REHABILITATE_CNF)
      {
        api_SimRehabilitateCnf_t* SimRehabilitateCnf = (api_SimRehabilitateCnf_t*)nEvParam;
        CSW_TRACE(CFW_SIM_TS_ID, "SimRehabilitateCnf: SW1 %x,SW2 %x \n", SimRehabilitateCnf->Sw1,
              SimRehabilitateCnf->Sw2);

        
#ifdef CFW_MULTI_SIM
        if ((0x90 == SimRehabilitateCnf->Sw1) && (0x00 == SimRehabilitateCnf->Sw2))
        {
          if (pSimInitS3Data->nEF == API_SIM_EF_LOCI)
          {
            UINT32 Result = 0;

            pSimInitS3Data->nEF = API_SIM_EF_IMSI;
            Result              = SimElemFileStatusReq(API_SIM_EF_IMSI, nSimID);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);

              return ERR_SUCCESS;
            }
            pSimInitS3Data->nSm_SimInitS3_prevStatus = pSimInitS3Data->nSm_SimInitS3_currStatus;
            pSimInitS3Data->nSm_SimInitS3_currStatus = SM_SIMINITS3_ELEMENTARY;
          }
          else  // API_SIM_EF_IMSI
          {
            SIM_PBK_PARAM* pG_Pbk_Param;
            CFW_CfgSimGetPbkParam(&pG_Pbk_Param, nSimID);

            if (CFW_SimGetPS() == TRUE)
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_ME_PS, 0, GENERATE_SHELL_UTI() | (nSimID << 24),
                                  SIM_INIT_EV_STAGE3);
            else if (CFW_SimGetPF() == TRUE)
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_ME_PF, 0, GENERATE_SHELL_UTI() | (nSimID << 24),
                                  SIM_INIT_EV_STAGE3);
            else
            {
              UINT8 nFDNStatus = 0;

              SIM_MISC_PARAM* pG_Misc_Param = NULL;
              CFW_CfgSimGetMicParam(&pG_Misc_Param, nSimID);
              pG_Misc_Param->nSimMode = CFW_SIM_CARD_NORMAL;

              CSW_TRACE(CFW_SIM_TS_ID, "Normal Sim Card\n");

              CFW_RegisterCreateAoProc(API_SIM_STATUSERROR_IND, SimSpySimCard);
              if (pSimInitS3Data->nMEStatus == 0)
              {
                nFDNStatus = 1;
              }
              else
              {
                nFDNStatus = pG_Pbk_Param->bADNStatus;
              }
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_NORMAL,
                                  pG_Pbk_Param->bBDNStatus << 1 | nFDNStatus, GENERATE_SHELL_UTI() | (nSimID << 24),
                                  SIM_INIT_EV_GSM);
            }
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
          }
        }
        else
        {
          CSW_TRACE(CFW_SIM_TS_ID, "ABNormal Sim Card\n");
          SIM_MISC_PARAM* pG_Misc_Param = NULL;
          CFW_CfgSimGetMicParam(&pG_Misc_Param, nSimID);
          pG_Misc_Param->nSimMode = CFW_SIM_CARD_ABNORMITY;
          CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_ABNORMITY, 0xFF,
                              GENERATE_SHELL_UTI() | (nSimID << 24), SIM_INIT_EV_STAGE3);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
        }
#else
        if ((0x90 == SimRehabilitateCnf->Sw1) && (0x00 == SimRehabilitateCnf->Sw2))
        {
          if (pSimInitS3Data->nEF == API_SIM_EF_LOCI)
          {
            UINT32 Result = 0;

            pSimInitS3Data->nEF = API_SIM_EF_IMSI;
            Result              = SimElemFileStatusReq(API_SIM_EF_IMSI);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);

              return ERR_SUCCESS;
            }

            pSimInitS3Data->nSm_SimInitS3_prevStatus = pSimInitS3Data->nSm_SimInitS3_currStatus;
            pSimInitS3Data->nSm_SimInitS3_currStatus = SM_SIMINITS3_ELEMENTARY;
          }
          else  // API_SIM_EF_IMSI
          {
            SIM_PBK_PARAM* pG_Pbk_Param;
            CFW_CfgSimGetPbkParam(&pG_Pbk_Param);

            // CFW_SendSclMessage(API_SIM_OPEN_IND, NULL);

            if (CFW_SimGetPS() == TRUE)
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_ME_PS, 0, GENERATE_SHELL_UTI(), SIM_INIT_EV_STAGE3);
            else if (CFW_SimGetPF() == TRUE)
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_ME_PF, 0, GENERATE_SHELL_UTI(), SIM_INIT_EV_STAGE3);
            else
            {
              UINT8 nFDNStatus = 0;

              SIM_MISC_PARAM* pG_Misc_Param = NULL;
              CFW_CfgSimGetMicParam(&pG_Misc_Param);
              pG_Misc_Param->nSimMode = CFW_SIM_CARD_NORMAL;

              CSW_TRACE(CFW_SIM_TS_ID, "CFW_SIM_CARD_NORMAL\n");

              CFW_RegisterCreateAoProc(API_SIM_STATUSERROR_IND, SimSpySimCard);
              if (pSimInitS3Data->nMEStatus == 0)
              {
                nFDNStatus = 1;

                // CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND,CFW_SIM_CARD_NORMAL,
                // pG_Pbk_Param->bBDNStatus<<1|0x01,GENERATE_SHELL_UTI(), SIM_INIT_EV_GSM);
              }
              else
              {
                nFDNStatus = pG_Pbk_Param->bADNStatus;

                // CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND,CFW_SIM_CARD_NORMAL,
                // pG_Pbk_Param->bBDNStatus<<1|pG_Pbk_Param->bADNStatus, 
                // GENERATE_SHELL_UTI(), SIM_INIT_EV_GSM);
              }
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_NORMAL,
                                  pG_Pbk_Param->bBDNStatus << 1 | nFDNStatus, GENERATE_SHELL_UTI(), SIM_INIT_EV_GSM);

            }
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
          }
        }
        else
        {
          SIM_MISC_PARAM* pG_Misc_Param = NULL;
          CFW_CfgSimGetMicParam(&pG_Misc_Param);
          pG_Misc_Param->nSimMode = CFW_SIM_CARD_ABNORMITY;
          CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_ABNORMITY, 0xFF, GENERATE_SHELL_UTI(),
                              SIM_INIT_EV_STAGE3);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
        }
#endif
      }
      else
        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_SIMINITS3_MMI:
    break;
  case SM_SIMINITS3_STACK:
    break;
  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage3Proc);
  return ERR_SUCCESS;
}


#define CFW_CHV_ET /*CHV Early treatment*/
#ifdef CFW_MULTI_SIM
extern CFW_SIM_OC_STATUS SIMOCStatus[]  ;
#else
extern CFW_SIM_OC_STATUS SIMOCStatus ;
#endif


UINT32 CFW_SimInitStageBootProc(HAO hAo, CFW_EV* pEvent)
{
  HAO hSimInitializeAo = hAo;

  UINT32 nEvtId  = 0;
  VOID* nEvParam = 0;

  CFW_SIMINITS* pSimInitData = NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimInitStageBootProc);
  if ((UINT32)pEvent != 0xFFFFFFFF)
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }

#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hSimInitializeAo);
#endif
  pSimInitData = CFW_GetAoUserData(hSimInitializeAo);

  switch (pSimInitData->nSm_SimInit_currStatus)
  {
  case SM_SIMINITS1_IDLE:
    {
      if ((UINT32)pEvent == 0xFFFFFFFF)
      {
        UINT32 Result = 0;

#ifdef CFW_MULTI_SIM
        Result = SimResetReq(nSimID);
        if (ERR_SUCCESS != Result)
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error Fail to send REST message,erro:0x%x \n", Result);
          CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);

          return ERR_SUCCESS;
        }

#else
        Result = SimResetReq();
        if (ERR_SUCCESS != Result)
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Error Fail to send REST message,erro:0x%x \n", Result);
          CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);

          return ERR_SUCCESS;
        }

#endif
        pSimInitData->nResetNum                = 0;
        pSimInitData->nSm_SimInit_prevStatus = pSimInitData->nSm_SimInit_currStatus;
        pSimInitData->nSm_SimInit_currStatus = SM_SIMINITS1_RESET;
        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_CONSUMED);
      }
      else
        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_SIMINITS1_RESET:
    {
      if (nEvtId == API_SIM_RESET_CNF)
      {
        api_SimResetCnf_t* pSimResetCnf = (api_SimResetCnf_t*)nEvParam;
#ifdef CFW_MULTI_SIM
        if (((pSimResetCnf->Sw1 == 0x90) && (pSimResetCnf->Sw2 == 0x00)) || (pSimResetCnf->Sw1 == 0x91))
        {
          UINT32 Result = 0;
          
          CSW_TRACE(CFW_SIM_TS_ID, "Reset Success[Sim%d]\n", nSimID);
          CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_INSERTED, 0x0, GENERATE_SHELL_UTI() | (nSimID << 24),
                              SIM_INIT_EV_STAGE1);
          
          CFW_SetSimOpenStatus(SOS_NO_SEND_SIM_OPEN,nSimID);

#ifdef CFW_CHV_ET

          {
            Result               = SimDedicFileStatusReq(1, nSimID);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimDedicFileStatusReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);
              return ERR_SUCCESS;
            }
            pSimInitData->nSm_SimInit_prevStatus = pSimInitData->nSm_SimInit_currStatus;
            pSimInitData->nSm_SimInit_currStatus = SM_SIMINITS1_DEDICATED;
          }

#else

          pSimInitData->nEF = API_SIM_EF_ECC;
          Result              = SimElemFileStatusReq(API_SIM_EF_ECC, nSimID);
          if (ERR_SUCCESS != Result)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);
            return ERR_SUCCESS;
          }
          pSimInitData->nSm_SimInit_prevStatus = pSimInitData->nSm_SimInit_currStatus;
          pSimInitData->nSm_SimInit_currStatus = SM_SIMINITS1_ELEMENTARY;
#endif          
        }
        else
        {
          if (pSimInitData->nResetNum == 1) // here we may need a discussion
          {
            SIM_MISC_PARAM* pG_Misc_Param = NULL;
            CFW_CfgSimGetMicParam(&pG_Misc_Param, nSimID);
            CSW_TRACE(CFW_SIM_TS_ID, "CFW_SIM_CARD_REMOVED: SW1 %x,SW2 %x \n", pSimResetCnf->Sw1,
                          pSimResetCnf->Sw2);
            pG_Misc_Param->nSimMode = CFW_SIM_CARD_REMOVED;
            CFW_SetSimStatus(nSimID, CFW_SIM_ABSENT);

            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_REMOVED, 0xFF,
                                GENERATE_SHELL_UTI() | (nSimID << 24), SIM_INIT_EV_STAGE1);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
          }
          else
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Num %x\n", pSimInitData->nResetNum);
            pSimInitData->nResetNum++;
            SimResetReq(nSimID);
          }
        }
#else
        if (((pSimResetCnf->Sw1 == 0x90) && (pSimResetCnf->Sw2 == 0x00))|| (pSimResetCnf->Sw1 == 0x91))
        {
          UINT32 Result = 0;
          
          CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_INSERTED, 0x0, GENERATE_SHELL_UTI(),
                              SIM_INIT_EV_STAGE1);
          
          CFW_SetSimOpenStatus(SOS_NO_SEND_SIM_OPEN);
          typedef struct _NW_STORELIST_INFO{
            CFW_StoredPlmnList p_StoreList;
          }CFW_STORELIST_INFO;
          
          CFW_STORELIST_INFO *pStoreList = CFW_MemAllocMessageStructure(sizeof(CFW_STORELIST_INFO));

          CFW_CfgGetStoredPlmnList(&pStoreList->p_StoreList);
          UINT32 Result = 0;

    			if (ERR_SUCCESS != (Result = CFW_SendSclMessage(API_SIM_OPEN_IND, pStoreList)))
    			{
    				//return (ret | BAL_PROC_DONT_FREE_MEM | BAL_PROC_COMPLETE);
    			}

          pSimInitData->nEF = API_SIM_EF_ECC;
          Result              = SimElemFileStatusReq(API_SIM_EF_ECC);
          if (ERR_SUCCESS != Result)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);
            return ERR_SUCCESS;
          }

          pSimInitData->nSm_SimInit_prevStatus = pSimInitData->nSm_SimInit_currStatus;
          pSimInitData->nSm_SimInit_currStatus = SM_SIMINITS1_ELEMENTARY;
       
        }
        else
        {
          if (pSimInitData->nResetNum == 3)
          {
            SIM_MISC_PARAM* pG_Misc_Param = NULL;
            CFW_CfgSimGetMicParam(&pG_Misc_Param);

            CSW_TRACE(CFW_SIM_TS_ID, "CFW_SIM_CARD_REMOVED: SW1 =%x,SW2 = %x \n", pSimResetCnf->Sw1,
                          pSimResetCnf->Sw2);
            pG_Misc_Param->nSimMode = CFW_SIM_CARD_REMOVED;
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_REMOVED, 0xFF, GENERATE_SHELL_UTI(),
                                SIM_INIT_EV_STAGE1);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
          }
          else
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Num = %x\n", pSimInitData->nResetNum);
            pSimInitData->nResetNum++;
            SimResetReq();
          }
        }
#endif
        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_CONSUMED);
      }
      else
        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_SIMINITS1_DEDICATED:
    {
      if (nEvtId == API_SIM_DEDICFILESTATUS_CNF)
      {

        
        api_SimDedicFileStatusCnf_t* pSimDedicFileStatusCnf = (api_SimDedicFileStatusCnf_t*)nEvParam;
        CSW_TRACE(CFW_SIM_TS_ID, "API_SIM_DEDICFILESTATUS_CNF  Sw1 0x%x, sw2 0x%x \n",
                      pSimDedicFileStatusCnf->Sw1, pSimDedicFileStatusCnf->Sw2);
        
    if (((0x90 == pSimDedicFileStatusCnf->Sw1) && (0x00 == pSimDedicFileStatusCnf->Sw2))
        || (0x91 == pSimDedicFileStatusCnf->Sw1))        
        {
          UINT32 Remains = 0;

          SIM_CHV_PARAM* pG_Chv_Param;
#ifdef CFW_MULTI_SIM
          Remains = SimParseDedicatedStatus(pSimDedicFileStatusCnf->Data, nSimID);
          CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
          if (pG_Chv_Param->nCHVAuthen == CFW_SIM_CARD_PIN1DISABLE)
          {
            SIM_MISC_PARAM* pG_Mic_Param = NULL;
            CFW_CfgSimGetMicParam(&pG_Mic_Param, nSimID);
            pG_Mic_Param->bStartPeriod = FALSE;
            CSW_TRACE(CFW_SIM_TS_ID, "CFW_SIM_CARD_PIN1DISABLE!\n");
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_PIN1DISABLE, Remains,
                                GENERATE_SHELL_UTI() | (nSimID << 24), SIM_INIT_EV_STAGE2);
             typedef struct _NW_STORELIST_INFO{
              CFW_StoredPlmnList p_StoreList[CFW_SIM_COUNT];
            }CFW_STORELIST_INFO;
            
            CFW_STORELIST_INFO *pStoreList = CFW_MemAllocMessageStructure(sizeof(CFW_STORELIST_INFO));

            UINT32 Result = 0;

            CFW_SIM_ID s = CFW_SIM_0;

            for( ; s < CFW_SIM_COUNT; s++ )
            {
              CFW_CfgGetStoredPlmnList(&pStoreList->p_StoreList[s], s);
            }

            CSW_TRACE(CFW_SIM_TS_ID, "Reset Success[Sim%d]\n", nSimID);
            CSW_TRACE(0, "Send Sim Open to Stack[sim%d]\n", nSimID);
      			if (ERR_SUCCESS != (Result = CFW_SendSclMessage(API_SIM_OPEN_IND, pStoreList, nSimID)))
      			{
      				//return (ret | BAL_PROC_DONT_FREE_MEM | BAL_PROC_COMPLETE);
      			}

          }
          else
          {
            SIMOCStatus[nSimID] = CFW_SIM_OC_IDLE;
            CSW_TRACE(CFW_SIM_TS_ID, "CFW_SIM_CARD_pin REQ!G_Chv_Param.nCHVAuthen %x\n", pG_Chv_Param->nCHVAuthen);
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, pG_Chv_Param->nCHVAuthen,
                                Remains, GENERATE_SHELL_UTI() | (nSimID << 24), SIM_INIT_EV_STAGE1);

          }
#else
          Remains = SimParseDedicatedStatus(pSimDedicFileStatusCnf->Data);
          CFW_CfgSimGetChvParam(&pG_Chv_Param);
          if (pG_Chv_Param->nCHVAuthen == CFW_SIM_CARD_PIN1DISABLE)
          {
            SIM_MISC_PARAM* pG_Mic_Param = NULL;
            CFW_CfgSimGetMicParam(&pG_Mic_Param);
            pG_Mic_Param->bStartPeriod = FALSE;
            CSW_TRACE(CFW_SIM_TS_ID, "CFW_SIM_CARD_PIN1DISABLE!\n");
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_PIN1DISABLE, Remains,
                                GENERATE_SHELL_UTI(), SIM_INIT_EV_STAGE2);
          }
          else
          {

            CSW_TRACE(CFW_SIM_TS_ID, "CFW_SIM_CARD_pin REQ!G_Chv_Param.nCHVAuthen=%x!!\n",
                          pG_Chv_Param->nCHVAuthen);
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, pG_Chv_Param->nCHVAuthen, Remains, GENERATE_SHELL_UTI(),
                                SIM_INIT_EV_STAGE1);


          }
#endif
        }

        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);
        return ERR_SUCCESS;


      }
      else
        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_NOT_CONSUMED);

    }
    break;

  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);
  return ERR_SUCCESS;
}


UINT32 CFW_SimInitStage1Proc(HAO hAo, CFW_EV* pEvent)
{
  HAO hSimInitializeAo = hAo;

  UINT32 nEvtId  = 0;
  VOID* nEvParam = 0;

  CFW_SIMINITS1* pSimInitS1Data = NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SimInitStage1Proc);
  if ((UINT32)pEvent != 0xFFFFFFFF)
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }


#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hSimInitializeAo);
#endif
  pSimInitS1Data = CFW_GetAoUserData(hSimInitializeAo);

  switch (pSimInitS1Data->nSm_SimInitS1_currStatus)
  {
  case SM_SIMINITS1_IDLE:
    {
      if ((UINT32)pEvent == 0xFFFFFFFF)
      {

   /*
          pSimInitS1Data->nEF = API_SIM_EF_ECC;
          Result              = SimElemFileStatusReq(API_SIM_EF_ECC, nSimID);
          if (ERR_SUCCESS != Result)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);
            return ERR_SUCCESS;
          }
          pSimInitS1Data->nSm_SimInitS1_prevStatus = pSimInitS1Data->nSm_SimInitS1_currStatus;
          pSimInitS1Data->nSm_SimInitS1_currStatus = SM_SIMINITS1_ELEMENTARY;

   */     
        UINT32 Result = 0;

#ifdef CFW_MULTI_SIM
          pSimInitS1Data->nEF = API_SIM_EF_ECC;
          Result              = SimElemFileStatusReq(API_SIM_EF_ECC, nSimID);
          if (ERR_SUCCESS != Result)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);
            return ERR_SUCCESS;
          }

#else
          pSimInitS1Data->nEF = API_SIM_EF_ECC;
          Result              = SimElemFileStatusReq(API_SIM_EF_ECC);
          if (ERR_SUCCESS != Result)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI()  , 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);
            return ERR_SUCCESS;
          }

#endif
        pSimInitS1Data->nSm_SimInitS1_prevStatus = pSimInitS1Data->nSm_SimInitS1_currStatus;
        pSimInitS1Data->nSm_SimInitS1_currStatus = SM_SIMINITS1_ELEMENTARY;
        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_CONSUMED);
      }
      else
        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_SIMINITS1_ELEMENTARY:
    {
      if (nEvtId == API_SIM_ELEMFILESTATUS_CNF)
      {
        api_SimElemFileStatusCnf_t* pSimElemFileStatusCnf = (api_SimElemFileStatusCnf_t*)nEvParam;
#ifdef CFW_MULTI_SIM
        CSW_TRACE(CFW_SIM_TS_ID, "API_SIM_ELEMFILESTATUS_CNF nSimID :0x%x,Sw1=0x%x, sw2=0x%x  \n",
                      nSimID, pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2);
#else
        CSW_TRACE(CFW_SIM_TS_ID, "API_SIM_ELEMFILESTATUS_CNF Sw1=0x%x, sw2=0x%x \n",
                      pSimElemFileStatusCnf->Sw1, pSimElemFileStatusCnf->Sw2);
#endif
        if (((0x90 == pSimElemFileStatusCnf->Sw1) && (00 == pSimElemFileStatusCnf->Sw2))
            || (0x91 == pSimElemFileStatusCnf->Sw1))
        {
          pSimInitS1Data->nFileSize   = ((UINT16)pSimElemFileStatusCnf->Data[2] << 8) | (pSimElemFileStatusCnf->Data[3]);
          pSimInitS1Data->nRecordSize = pSimElemFileStatusCnf->Data[14];

          //if (pSimInitS1Data->nFileSize != 0)
          {
#ifdef CFW_MULTI_SIM
            UINT32 Result = SimReadBinaryReq(pSimInitS1Data->nEF, 0, (UINT8)(pSimInitS1Data->nFileSize), nSimID);

            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);

              return ERR_SUCCESS;
            }
#else
            UINT32 Result = SimReadBinaryReq(pSimInitS1Data->nEF, 0, (UINT8)(pSimInitS1Data->nFileSize));

            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimReadBinaryReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);

              return ERR_SUCCESS;
            }
#endif


            pSimInitS1Data->nSm_SimInitS1_prevStatus = pSimInitS1Data->nSm_SimInitS1_currStatus;
            pSimInitS1Data->nSm_SimInitS1_currStatus = SM_SIMINITS1_READBINARY;
          }
        }
        else if (pSimInitS1Data->nEF == API_SIM_EF_ECC)
        {
          UINT32 Result = 0;

          // SET ecc is null;
          CFW_SimSetECC(NULL);

          // Read EFLP file.This should read EFELP file,but this file cannot be support by StackCom.
          pSimInitS1Data->nEF = API_SIM_EF_LP;
#ifdef CFW_MULTI_SIM
          Result = SimElemFileStatusReq(API_SIM_EF_LP, nSimID);
          if (ERR_SUCCESS != Result)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);

            return ERR_SUCCESS;
          }
#else
          Result = SimElemFileStatusReq(API_SIM_EF_LP);
          if (ERR_SUCCESS != Result)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);

            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);

            return ERR_SUCCESS;
          }
#endif
        }
        else if (pSimInitS1Data->nEF == API_SIM_EF_LP)
        {
          UINT32 Result = 0;

          // Set LP code is NULL
          CFW_SimSetLP(NULL, 0);

          // Read EFPHASE
          pSimInitS1Data->nEF = API_SIM_EF_PHASE;
#ifdef CFW_MULTI_SIM
          Result = SimElemFileStatusReq(API_SIM_EF_PHASE, nSimID);
          if (ERR_SUCCESS != Result)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);

            return ERR_SUCCESS;
          }

#else
          Result = SimElemFileStatusReq(API_SIM_EF_PHASE);
          if (ERR_SUCCESS != Result)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);

            return ERR_SUCCESS;
          }

#endif
        }
        else
        {
          UINT32 Result = 0;

          SIM_MISC_PARAM* pG_Mic_Param;
#ifdef CFW_MULTI_SIM
          CFW_CfgSimGetMicParam(&pG_Mic_Param, nSimID);
          pG_Mic_Param->nPhase = SIM_PHASE1;  // default phase1
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_PHASE, 0,
                                GENERATE_SHELL_UTI() | (nSimID << 24), SIM_INIT_EV_STAGE1);

          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);
          return ERR_SUCCESS;


#else
          CFW_CfgSimGetMicParam(&pG_Mic_Param);
          pG_Mic_Param->nPhase = SIM_PHASE1;  // default phase1
          Result               = SimDedicFileStatusReq(1);
          if (ERR_SUCCESS != Result)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimDedicFileStatusReq return 0x%x \n", Result);

            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);
            return ERR_SUCCESS;
          }

#endif
          pSimInitS1Data->nSm_SimInitS1_prevStatus = pSimInitS1Data->nSm_SimInitS1_currStatus;
          pSimInitS1Data->nSm_SimInitS1_currStatus = SM_SIMINITS1_DEDICATED;
        }
        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_CONSUMED);
      }
      else
        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_SIMINITS1_READBINARY:
    {
      if (nEvtId == API_SIM_READBINARY_CNF)
      {

        api_SimReadBinaryCnf_t* pSimReadBinaryCnf = (api_SimReadBinaryCnf_t*)nEvParam;
#ifdef CFW_MULTI_SIM
        CSW_TRACE(CFW_SIM_TS_ID, "API_SIM_READBINARY_CNF nSimID :0x%x\n", nSimID);
#endif
        CSW_TRACE(CFW_SIM_TS_ID, "API_SIM_READBINARY_CNF  Sw1 0x%x, sw2 0x%x EF[0x%x]\n",
                      pSimReadBinaryCnf->Sw1, pSimReadBinaryCnf->Sw2,pSimInitS1Data->nEF);
    if (((0x90 == pSimReadBinaryCnf->Sw1) && (0x00 == pSimReadBinaryCnf->Sw2))
        || (0x91 == pSimReadBinaryCnf->Sw1))

    {
          if (pSimInitS1Data->nEF == API_SIM_EF_ECC)
          {
            UINT32 Result = 0;

            CFW_SimSetECC(pSimReadBinaryCnf->Data);

            // Read EFLP file.This should read EFELP file,but this file cannot be support by StackCom.
            pSimInitS1Data->nEF = API_SIM_EF_LP;
#ifdef CFW_MULTI_SIM
            Result = SimElemFileStatusReq(API_SIM_EF_LP, nSimID);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);

              return ERR_SUCCESS;
            }

#else
            Result = SimElemFileStatusReq(API_SIM_EF_LP);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);

              return ERR_SUCCESS;
            }

#endif

            pSimInitS1Data->nSm_SimInitS1_prevStatus = pSimInitS1Data->nSm_SimInitS1_currStatus;
            pSimInitS1Data->nSm_SimInitS1_currStatus = SM_SIMINITS1_ELEMENTARY;
          }
          else if (pSimInitS1Data->nEF == API_SIM_EF_LP)
          {
            UINT32 Result = 0;
              CSW_TRACE(CFW_SIM_TS_ID, "pSimReadBinaryCnf->Data[0] 0x%x \n", pSimReadBinaryCnf->Data[0]);

            // process lp file
            if (pSimReadBinaryCnf->Data[0] != 0xFF)
            {
              UINT8* pLangPreferred = NULL;

              pLangPreferred = (UINT8*)CSW_SIM_MALLOC(pSimInitS1Data->nFileSize);
              if (pLangPreferred == NULL)
              {
                CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_MEMORY_FULL \n");
#ifdef CFW_MULTI_SIM
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, ERR_NO_MORE_MEMORY, 0xFF,
                                    GENERATE_SHELL_UTI() | (nSimID << 24), SIM_INIT_EV_STAGE2);
#else
                CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, ERR_NO_MORE_MEMORY, 0xFF, GENERATE_SHELL_UTI(),
                                    SIM_INIT_EV_STAGE2);
#endif
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);

                return ERR_NO_MORE_MEMORY;
              }
              SUL_MemCopy8(pLangPreferred, pSimReadBinaryCnf->Data, pSimInitS1Data->nFileSize);
              CFW_SimSetLP(pLangPreferred, pSimInitS1Data->nFileSize);
            }
            else
              CFW_SimSetLP(NULL, 0);

            // Read EFPHASE file.
            pSimInitS1Data->nEF = API_SIM_EF_PHASE;
            CSW_TRACE(CFW_SIM_TS_ID, "pSimReadBinaryCnf->Data[0] 0x%x \n", pSimReadBinaryCnf->Data[0]);
            
#ifdef CFW_MULTI_SIM
            Result = SimElemFileStatusReq(API_SIM_EF_PHASE, nSimID);
            if (ERR_SUCCESS != Result)
            {
              
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);

              return ERR_SUCCESS;
            }
#else
            Result = SimElemFileStatusReq(API_SIM_EF_PHASE);
            if (ERR_SUCCESS != Result)
            {
              
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);

              return ERR_SUCCESS;
            }
#endif


            pSimInitS1Data->nSm_SimInitS1_prevStatus = pSimInitS1Data->nSm_SimInitS1_currStatus;
            pSimInitS1Data->nSm_SimInitS1_currStatus = SM_SIMINITS1_ELEMENTARY;
          }
          else  // API_SIM_EF_PHASE
          {
            UINT32 Result = 0;


            SIM_MISC_PARAM* pG_Mic_Param = NULL;
#ifdef CFW_MULTI_SIM
            CFW_CfgSimGetMicParam(&pG_Mic_Param, nSimID);
            pG_Mic_Param->nPhase = pSimReadBinaryCnf->Data[0];
            CSW_TRACE(CFW_SIM_TS_ID, "EF_PHASE Data[0] 0x%x\r\n", pSimReadBinaryCnf->Data[0]);
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_PHASE, 0,
                                GENERATE_SHELL_UTI() | (nSimID << 24), SIM_INIT_EV_STAGE1);

            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);
            return ERR_SUCCESS;

#else
            CFW_CfgSimGetMicParam(&pG_Mic_Param);
            pG_Mic_Param->nPhase = pSimReadBinaryCnf->Data[0];

            CSW_TRACE(CFW_SIM_TS_ID, "EF_PHASE Data[0] 0x%x\r\n", pSimReadBinaryCnf->Data[0]);

            // Read DedicatedFile
            Result = SimDedicFileStatusReq(1);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimDedicFileStatusReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);

              return ERR_SUCCESS;
            }
#endif


            pSimInitS1Data->nSm_SimInitS1_prevStatus = pSimInitS1Data->nSm_SimInitS1_currStatus;
            pSimInitS1Data->nSm_SimInitS1_currStatus = SM_SIMINITS1_DEDICATED;
          }

// CFW_SetAoProcCode(hSimInitializeAo,CFW_AO_PROC_CODE_CONSUMED);
// return ERR_SUCCESS;
        }
        else
        {
          if (pSimInitS1Data->nEF == API_SIM_EF_ECC)
          {
            UINT32 Result = 0;

            CFW_SimSetECC(NULL);  // SET ecc is null;
            pSimInitS1Data->nEF = API_SIM_EF_LP;
#ifdef CFW_MULTI_SIM
            Result = SimElemFileStatusReq(API_SIM_EF_LP, nSimID);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);
              return ERR_SUCCESS;
            }
#else
            Result = SimElemFileStatusReq(API_SIM_EF_LP);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);
              return ERR_SUCCESS;
            }
#endif


            pSimInitS1Data->nSm_SimInitS1_prevStatus = pSimInitS1Data->nSm_SimInitS1_currStatus;
            pSimInitS1Data->nSm_SimInitS1_currStatus = SM_SIMINITS1_ELEMENTARY;
          }
          else if (pSimInitS1Data->nEF == API_SIM_EF_LP)
          {
            UINT32 Result = 0;

            CFW_SimSetLP(NULL, 0);
            pSimInitS1Data->nEF = API_SIM_EF_PHASE;
#ifdef CFW_MULTI_SIM
            Result = SimElemFileStatusReq(API_SIM_EF_PHASE, nSimID);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);

              return ERR_SUCCESS;
            }
#else
            Result = SimElemFileStatusReq(API_SIM_EF_PHASE);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);

              return ERR_SUCCESS;
            }
#endif


            pSimInitS1Data->nSm_SimInitS1_prevStatus = pSimInitS1Data->nSm_SimInitS1_currStatus;
            pSimInitS1Data->nSm_SimInitS1_currStatus = SM_SIMINITS1_ELEMENTARY;
          }
          else  // API_SIM_EF_PHASE
          {
            UINT32 Result = 0;

            SIM_MISC_PARAM* pG_Mic_Param;
#ifdef CFW_MULTI_SIM
            CFW_CfgSimGetMicParam(&pG_Mic_Param, nSimID);
            pG_Mic_Param->nPhase = SIM_PHASE1;  // default phase1

#ifdef CFW_CHV_ET
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);
            return ERR_SUCCESS;

#else
            Result               = SimDedicFileStatusReq(1, nSimID);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimDedicFileStatusReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);

              return ERR_SUCCESS;
            }
#endif            
#else
            CFW_CfgSimGetMicParam(&pG_Mic_Param);
            pG_Mic_Param->nPhase = SIM_PHASE1;  // default phase1
            // Read DedicatedFile
            Result = SimDedicFileStatusReq(1);
            if (ERR_SUCCESS != Result)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Error SimDedicFileStatusReq return 0x%x \n", Result);
              CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI(), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);

              return ERR_SUCCESS;
            }
#endif


            pSimInitS1Data->nSm_SimInitS1_prevStatus = pSimInitS1Data->nSm_SimInitS1_currStatus;
            pSimInitS1Data->nSm_SimInitS1_currStatus = SM_SIMINITS1_DEDICATED;
          }

// CFW_SetAoProcCode(hSimInitializeAo,CFW_AO_PROC_CODE_CONSUMED);
// return ERR_SUCCESS;
        }
        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_CONSUMED);
      }
      else
        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_SIMINITS1_DEDICATED:
    {
      if (nEvtId == API_SIM_DEDICFILESTATUS_CNF)
      {
     
        api_SimDedicFileStatusCnf_t* pSimDedicFileStatusCnf = (api_SimDedicFileStatusCnf_t*)nEvParam;
        CSW_TRACE(CFW_SIM_TS_ID, "API_SIM_DEDICFILESTATUS_CNF  Sw1 0x%x, sw2 0x%x \n",
                      pSimDedicFileStatusCnf->Sw1, pSimDedicFileStatusCnf->Sw2);
        
    if (((0x90 == pSimDedicFileStatusCnf->Sw1) && (0x00 == pSimDedicFileStatusCnf->Sw2))
        || (0x91 == pSimDedicFileStatusCnf->Sw1))        
        {
          UINT32 Remains = 0;

          SIM_CHV_PARAM* pG_Chv_Param;
#ifdef CFW_MULTI_SIM
          Remains = SimParseDedicatedStatus(pSimDedicFileStatusCnf->Data, nSimID);
          CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
          if (pG_Chv_Param->nCHVAuthen == CFW_SIM_CARD_PIN1DISABLE)
          {
            SIM_MISC_PARAM* pG_Mic_Param = NULL;
            CFW_CfgSimGetMicParam(&pG_Mic_Param, nSimID);
            pG_Mic_Param->bStartPeriod = FALSE;
            CSW_TRACE(CFW_SIM_TS_ID, "CFW_SIM_CARD_PIN1DISABLE!\n");
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_PIN1DISABLE, Remains,
                                GENERATE_SHELL_UTI() | (nSimID << 24), SIM_INIT_EV_STAGE2);
          }
          else
          {
            SIMOCStatus[nSimID] = CFW_SIM_OC_IDLE;
            CSW_TRACE(CFW_SIM_TS_ID, "CFW_SIM_CARD_pin REQ!G_Chv_Param.nCHVAuthen %x\n", pG_Chv_Param->nCHVAuthen);
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, pG_Chv_Param->nCHVAuthen,
                                Remains, GENERATE_SHELL_UTI() | (nSimID << 24), SIM_INIT_EV_STAGE1);

          }
#else
          Remains = SimParseDedicatedStatus(pSimDedicFileStatusCnf->Data);
          CFW_CfgSimGetChvParam(&pG_Chv_Param);
          if (pG_Chv_Param->nCHVAuthen == CFW_SIM_CARD_PIN1DISABLE)
          {
            SIM_MISC_PARAM* pG_Mic_Param = NULL;
            CFW_CfgSimGetMicParam(&pG_Mic_Param);
            pG_Mic_Param->bStartPeriod = FALSE;
            CSW_TRACE(CFW_SIM_TS_ID, "CFW_SIM_CARD_PIN1DISABLE!\n");
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, CFW_SIM_CARD_PIN1DISABLE, Remains,
                                GENERATE_SHELL_UTI(), SIM_INIT_EV_STAGE2);
          }
          else
          {

            CSW_TRACE(CFW_SIM_TS_ID, "CFW_SIM_CARD_pin REQ!G_Chv_Param.nCHVAuthen=%x!!\n",
                          pG_Chv_Param->nCHVAuthen);
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, pG_Chv_Param->nCHVAuthen, Remains, GENERATE_SHELL_UTI(),
                                SIM_INIT_EV_STAGE1);


          }
#endif
        }

#ifdef CFW_MULTI_SIM    
#ifdef CFW_CHV_ET

          pSimInitS1Data->nEF = API_SIM_EF_ECC;
          UINT32 Result              = SimElemFileStatusReq(API_SIM_EF_ECC, nSimID);
          if (ERR_SUCCESS != Result)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "Error SimElemFileStatusReq return 0x%x \n", Result);
            CFW_PostNotifyEvent(EV_CFW_SIM_STATUS_IND, Result, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);
            return ERR_SUCCESS;
          }
          pSimInitS1Data->nSm_SimInitS1_prevStatus = pSimInitS1Data->nSm_SimInitS1_currStatus;
          pSimInitS1Data->nSm_SimInitS1_currStatus = SM_SIMINITS1_ELEMENTARY;
#else
        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);
        return ERR_SUCCESS;
#endif
#else
        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimInitializeAo);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);
        return ERR_SUCCESS;
#endif
      }
      else
        CFW_SetAoProcCode(hSimInitializeAo, CFW_AO_PROC_CODE_NOT_CONSUMED);

    }
    break;
  case SM_SIMINITS1_MMI:
    break;
  case SM_SIMINITS1_STACK:
    break;
  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SimInitStage1Proc);
  return ERR_SUCCESS;
}


UINT32 CFW_SimResetProc(HAO hAo, CFW_EV* pEvent)
{
  UINT32 nUTI      = 0x00000000;
  UINT32 nEvtId    = 0x00000000;
  UINT32 ErrorCode = 0x00000000;
  UINT32 result    = 0x00000000;
  VOID* nEvParam   = NULL;
  CFW_EV ev;

  SIM_SM_RESET* pReset = NULL;
  api_SimResetCnf_t* pSimResetCnf = NULL;

#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hAo);
#endif
  pReset = CFW_GetAoUserData(hAo);
  if ((UINT32)pEvent == 0xffffffff)
  {
    SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
    ev.nParam1                = 0xff;
    pEvent                    = &ev;
    pReset->nState.nNextState = CFW_SIM_MIS_IDLE;
  }
  else
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }

  switch (pReset->nState.nNextState)
  {
  case CFW_SIM_MIS_IDLE:

#ifdef CFW_MULTI_SIM
    result = CFW_SendSclMessage(API_SIM_RESET_REQ, NULL, nSimID);
    if (ERR_SUCCESS != result)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_SendSclMessage return 0x%x \n", result);
      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
      hAo = HNULL;
      CFW_PostNotifyEvent(EV_CFW_SIM_RESET_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
    }
#else
    result = CFW_SendSclMessage(API_SIM_RESET_REQ, NULL);
    if (ERR_SUCCESS != result)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_SendSclMessage return 0x%x \n", result);

      CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
      hAo = HNULL;
      CFW_PostNotifyEvent(EV_CFW_SIM_RESET_RSP, (UINT32)result, 0, nUTI, 0xF0);
    }
#endif
    pReset->nState.nCurrState = CFW_SIM_MIS_IDLE;
    pReset->nState.nNextState = CFW_SIM_MIS_RESET;
    break;

  case CFW_SIM_MIS_RESET:
    pSimResetCnf = (api_SimResetCnf_t*)nEvParam;
    CFW_GetUTI(hAo, &nUTI);

#ifdef CFW_MULTI_SIM
    //if ((pSimResetCnf->ResetResult == 0x02) || (pSimResetCnf->ResetResult == 0x03))
    if (((pSimResetCnf->Sw1 == 0x90) && (pSimResetCnf->Sw2 == 0x00)) || (pSimResetCnf->Sw1 == 0x91))
    {
      CFW_PostNotifyEvent(EV_CFW_SIM_RESET_RSP, (UINT32)0x00, 0x00, nUTI | (nSimID << 24), 0x00);  // 2006-11-13
    }
    else
    {
      CFW_SendSclMessage(API_SIM_CLOSE_IND, NULL, nSimID);  
      ErrorCode = Sim_ParseSW1SW2(pSimResetCnf->Sw1, pSimResetCnf->Sw2, nSimID);
      CFW_PostNotifyEvent(EV_CFW_SIM_RESET_RSP, (UINT32)result, 0, nUTI | (nSimID << 24), 0xF0);
    }
#else
    //if ((pSimResetCnf->ResetResult == 0x02) || (pSimResetCnf->ResetResult == 0x03))
    if (((pSimResetCnf->Sw1 == 0x90) && (pSimResetCnf->Sw2 == 0x00)) || (pSimResetCnf->Sw1 == 0x91))

    {
      CFW_PostNotifyEvent(EV_CFW_SIM_RESET_RSP, (UINT32)0x00, 0x00, nUTI, 0x00); 

    }
    else
    {
      CFW_SendSclMessage(API_SIM_CLOSE_IND, NULL);  
      ErrorCode = Sim_ParseSW1SW2(pSimResetCnf->Sw1, pSimResetCnf->Sw2);
      CFW_PostNotifyEvent(EV_CFW_SIM_RESET_RSP, (UINT32)result, 0, nUTI, 0xF0);
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
  return ERR_SUCCESS;
}







