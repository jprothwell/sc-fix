// //////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2002-2010, Coolsand Technologies, Inc.
// All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmission of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
// 
// FILENAME: cfw_sim_sat_pro.c
// 
// DESCRIPTION:
// TODO: ...
// 
// REVISION HISTORY:
// NAME              DATE                REMAKS
// Lixp      2010-4-19       Created initial version 1.0
// 
// //////////////////////////////////////////////////////////////////////////////

#include "api_msg.h"
#include <cfw.h>
#include <base.h>
#include <cfw_prv.h>
#include <sul.h>
#include <ts.h>
#include "csw_debug.h"

#include "cfw_sim.h"
#include "cfw_sim_sat.h"

const UINT8 TerminalResponse[RESPONSEBASE] = {
// Command details
  0x81, // Command Detail tag            Tag=0x01 or 0x81
  0x03, // Length                          0x03
  // Command Number Modify by lixp at 20081005 for SIM SAT bug.
  // 协议说明3GPP 11.14 6.5.1  Command numbers may take any hexadecimal value between '01' and 'FE'.
  // 目前经验证有些卡，必须是0x00才能正常执行Response的操作。
  // 所以目前定义的是0x00 和 0x01.通常首先尝试0x00,如果失败则尝试0x01.
  // 按照协议，应该是由SIM卡指定Command numbers，ME记录该值，不过实践发现，有些情况使用SIM卡返回的Command numbers
  // 会出现异常，所以仍然按照0x00,0x01来尝试。
  0x00, // 0x01,    
  0x00, // Type of command               This byte need to be updated.???
  0x00, // Command Qualifier             ???

// Device identities
  0x82, // Device identities tag         Tag=0x01 or 0x81
  0x02, // Length                        0x02
  0x82, // Source device identity        ME=0x82
  0x81, // Destination device identity   SIM=0x81

// Result
  0x83, // Result tag                    tag=0x03 or 0x83
  0x01, // Length                        0x01,    ??????
  0x00  // General result                 0x0n or 0x1n successful.???
    // 0x2n or 0x3n fail.
/*    0x00,    //Additional information
            // on result
*/
};
const UINT8 TerminalResponse1[RESPONSEBASE] = {
// Command details
  0x81, // Command Detail tag            Tag=0x01 or 0x81
  0x03, // Length                          0x03

  // Command Number Modify by lixp at 20081005 for SIM SAT bug.
  // 协议说明3GPP 11.14 6.5.1  Command numbers may take any hexadecimal value between '01' and 'FE'.
  // 目前经验证有些卡，必须是0x00才能正常执行Response的操作。
  // 所以目前定义的是0x00 和 0x01.通常首先尝试0x00,如果失败则尝试0x01.
  0x01, // Command Number 
  0x00, // Type of command               This byte need to be updated.???
  0x00, // Command Qualifier             ???

// Device identities
  0x82, // Device identities tag         Tag=0x01 or 0x81
  0x02, // Length                        0x02
  0x82, // Source device identity        ME=0x82
  0x81, // Destination device identity   SIM=0x81

// Result
  0x83, // Result tag                    tag=0x03 or 0x83
  0x01, // Length                        0x01,    ??????
  0x00  // General result                 0x0n or 0x1n successful.???
    // 0x2n or 0x3n fail.
/*    0x00,    //Additional information
            // on result
*/
};  // #define ComType          0x03

// #define ComType          0x03
// #define ComResult        0x0B
// 

const UINT8 ResponseDuration[RESPONSEDURA] = {
  // Duration
  0x84, // Duration tag                  tag=0x04 or 0x84
  0x02, // Length                        0x02
  0x00, // Time unit                     00-Minutes,01-Seconds,02-Tenths of seconds
  0x00  // Time innterval                 00-Reserved,01-1 unit,02-2 units...FF-255
};

const UINT8 ResponseText[RESPONSETEXT] = {
  // Text string
  0x0D, // Text string tag                0x0d or 0x8d
  0x00, // Length                         00
  0x00  // Data coding scheme              00
};

const UINT8 ResponseUSSD[RESPONSEUSSD] = {
  // USSD 
  0x0d, // USSD string tag                0x0d or 0x8d
  0x00, // Length                         00
  0x00  // Data coding scheme             00
};
const UINT8 ResponseUSSDEX[RESPONSEUSSD+1] = {
  // USSD 
  0x0d, // USSD string tag                0x0d or 0x8d
  0x81,
  0x00, // Length                         00
  0x00  // Data coding scheme             00
};
// #define TextLength        0x01
// #define TextScheme        0x02

const UINT8 ResponseItem[RESPONSEITEM] = {
  // Item identifier
  0x90, // Item identifier tag            tag=0x10 or 0x90
  0x01, // Length                         0x01
  0x00  // Identifier of item chosen       This byte need to be updated.???
    // 0x2n or 0x3n fail.
};

// #define ItemSelection    0x02

const UINT8 MenuSelection[] = {
  0xD3, // Menu Selection Tag             Tag=0xD3
  0x07, // Length                         This byte need to be updated.???
  // This can occupy the two byte.
  // Device identities
  0x82, // Device identities tag          Tag=0x02 or 0x82
  0x02, // Length                         0x02
  0x01, // Source device identity         Keypad=0x82
  0x81, // Destination device identity    SIM=0x81

  // Item identifier
  0x90, // Item identifier tag            tag=0x10 or 0x90
  0x01, // Length                         0x01
  0x00  // Identifier of item chosen       This byte need to be updated.???
    // 0x2n or 0x3n fail.
};
const UINT8 EventDownload[] = {
  0xD6, // Event download tag             Tag=0xD6
  0x07, // Length                         This byte need to be updated.???
  // This can occupy the two byte.
  // Device identities
  0x19, // Event list tag          
  0x01, // Length                         User activity length
  0x04, // User activity          
  0x82, // Destination device identity 
  0x02, // Length = '02'
  0x82, // Source device identity         ME
  0x81  // Destination device identity    SIM
};
const UINT8 SMPPDownSelection[] = {
  0xD1, // SMS TAG Selection Tag             Tag=0xD1
  0x00, // Length                         This byte need to be updated.???
  // This can occupy the two byte.
// Device identities
  0x02, // Device identities tag          Tag=0x02 or 0x82
  0x02, // Length                         0x02
  0x83, // Source device identity         Network=0x83
  0x81, // Destination device identity    SIM=0x81

// Address identifier
  0x06, // Address identifier tag            tag=0x06 or 0x86
};
const UINT8 SMPPDownSelectionEx[] = {
  0xD1, // SMS TAG Selection Tag             Tag=0xD1
  0x81,
  0x00, // Length                         This byte need to be updated.???
  // This can occupy the two byte.
// Device identities
  0x02, // Device identities tag          Tag=0x02 or 0x82
  0x02, // Length                         0x02
  0x83, // Source device identity         Network=0x83
  0x81, // Destination device identity    SIM=0x81

// Address identifier
  0x06, // Address identifier tag            tag=0x06 or 0x86
};


// #define ItemNumber    0x08        //if Length occupy two byte,this value should add one.

const UINT8 ResponseLocalInfo[RESPONSELOCAL] = {
  0x93, // Local Information Tag           Tag=0x13 or 0x93
  0x07, // Length
  0x00, // Mobile country&Network codes    Three octets
  0x00, // (MCC & MNC)
  0x00,
  0x00, // Location Area code(LAC)         Two octets
  0x00,
  0x00, // Cell Identify Value(Cell ID)    Two octets
  0x00
};

// #define MCC_MNC        0x02
// #define LAC            0x05
// #define CELLID        0x07

const UINT8 ResponseIMEI[RESPONSEIMEI] = {
  0x94, // IMEI Tag                        Tag=0x14 or 0x94
  0x08, // Length
  0x00, // IMEI                            eight octets
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
};

// #define IMEI        0x02

const UINT8 ResponseMeasure[RESPONSEMEASURE] = {
  0x96, // Network measurement Tag    Tag=0x16 or 0x96
  0x10, // Length
  0x00, // Measurement results        sixteen octets
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
};

// #define MEASUREMENT        0x02

const UINT8 ResponseDateTime[RESPONSEDATETIME] = {
  0xA6, // Date-Time and Zone tag     Tag=0x26 or 0xA6
  0x07, // Length
  0x00, // Date Time and Time zone    seven octets
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
};

// #define DateTime    0x02

const UINT8 ResponseLanguage[RESPONSELANGUAGE] = {
  0xAD, // Language tag            Tag=0x2D or 0xAD
  0x02, // Length
  0x00, // Language                Two octets
  0x00
};

// #define Language    0x02

const UINT8 ResponseTimingAdvance[RESPONSETADVANCE] = {
  0xAE, // Timing Advance tag     Tag=0x2E or 0xAE
  0x02, // Length
  0x00, // ME Status              00:ME is in the idle status,01:ME is not in the idle status
  0x00  // Timing Advance          Value
};

// #define MEStatus    0x02
// #define TimingAdv    0x03

const UINT8 ResponseChannel[RESPONSECHANNEL] = {
  0xB8, // Channel status Tag            Tag=0x38 or 0xB8
  0x02, // Length
  0x00, // Channel status                Two octets
  0x00
};

// #define CHANNELSTATUS        0x02

const UINT8 ResponseCardID[RESPONSECARDID] = {
  0xBA, // Card reader identify tag    Tag=0x3A or 0xBA
  0x00, // Length,the ID need to be dynamic allocation
  0x00
};

// #define CardID_Length    0x01
// #define CardID            0x02
static UINT32 nTRTryNum = 0x00; // TerminalResponse
static UINT32 nTETryNum = 0x00; // TerminalEnvelop

#ifdef CFW_MULTI_SIM
UINT8 nCommondNum[68][CFW_SIM_COUNT]               = { {0x00,}, {0x00,} };
#else
UINT8 nCommondNum[68] ={0x00,};
#endif
const UINT8 ResponseCardStatus[RESPONSECARDSTATUS] = {
  0xA0, // Card reader status tag    Tag=0x20 or 0xA0
  0x01, // Length
  0x00
};

#define CardStatus    0x02

#ifndef MEM_ALIGNMENT
#define MEM_ALIGNMENT                   4
#endif
#ifndef MEM_ALIGN_SIZE
#define MEM_ALIGN_SIZE(size) (((size) + MEM_ALIGNMENT - 1) & ~(MEM_ALIGNMENT-1))
#endif

void CFW_ValidatePDU(UINT8* pTPDU)
{
  UINT8 offSet = 0;

  offSet = (pTPDU[2] % 2 == 0) ? (pTPDU[2] / 2) : (pTPDU[2] / 2 + 1);

  if (pTPDU[5 + offSet] == 0x00)
  {
    pTPDU[5 + offSet] = 0x04;
  }
}
UINT32 CFW_SatResponseProc(HAO hAo, CFW_EV* pEvent)
{
  HAO hSimSatResponseAo = hAo;

  UINT32 nEvtId  = 0;
  VOID* nEvParam = NULL;
  UINT32 nUTI    = 0;

  SIM_SAT_PARAM* pG_SimSat = NULL;
  CFW_SAT_RESPONSE* pSimSatResponseData = NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SatResponseProc);
  if ((UINT32)pEvent != 0xFFFFFFFF)
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }

#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;
  nSimID = CFW_GetSimCardID(hSimSatResponseAo);
  CFW_CfgSimGetSatParam(&pG_SimSat, nSimID);
  pSimSatResponseData = CFW_GetAoUserData(hSimSatResponseAo);

  
  CSW_TRACE(CFW_SIM_TS_ID, "CFW_SatResponseProc nSimID[%d] CmdType[0x%x] nEvtId[0x%d]\n", nSimID,
                pSimSatResponseData->nSAT_CmdType,nEvtId);

#else

  CFW_CfgSimGetSatParam(&pG_SimSat);
  pSimSatResponseData = CFW_GetAoUserData(hSimSatResponseAo);
  CSW_TRACE(CFW_SIM_TS_ID, "CFW_SatResponseProc  CmdType[0x%x]\n", pSimSatResponseData->nSAT_CmdType);

#endif

  CFW_GetUTI(hSimSatResponseAo, &nUTI);
  switch (pSimSatResponseData->nSm_Resp_currStatus)
  {
  case SM_RESP_IDLE:
    {
      if ((UINT32)pEvent == 0xFFFFFFFF)
      {
        nTRTryNum = 0x00;
        nTETryNum = 0x00;

        if ( SIM_SAT_SETUP_MENU_IND == pSimSatResponseData->nSAT_CmdType )  // response the envolope
        {
          api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
            (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SimTerminalResponseReq_t));
          CFW_SAT_MENU_RSP* pMenu = (CFW_SAT_MENU_RSP*)(pG_SimSat->pSetupMenu);

          if ((pSimTerminalResponesReq == NULL) || (pMenu == NULL))
          {
            CSW_TRACE(CFW_SIM_TS_ID, "NO_MORE_MEMORY nSAT_CmdType %x\n", pSimSatResponseData->nSAT_CmdType);
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

          pSimTerminalResponesReq->NbByte = 12;

          // 首先尝试Command number为0x00.'
#ifdef CFW_MULTI_SIM
          CSW_TRACE(CFW_SIM_TS_ID, " nCommondNum %x\n", nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID]);
#else
          CSW_TRACE(CFW_SIM_TS_ID, " nCommondNum %x\n", nCommondNum[pSimSatResponseData->nSAT_CmdType - 1]);
#endif
          SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, pSimTerminalResponesReq->NbByte);

#ifdef CFW_MULTI_SIM
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID];
#else
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatResponseData->nSAT_CmdType - 1];
#endif
          pSimTerminalResponesReq->TerminalRespData[ComType]   = 0x25;
          pSimTerminalResponesReq->TerminalRespData[ComQual]   = pMenu->nComQualifier;
          pSimTerminalResponesReq->TerminalRespData[ComResult] = pSimSatResponseData->nSAT_Status;
#ifdef CFW_MULTI_SIM
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
#else
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
#endif

        }
        else if ( SIM_SAT_SELECT_ITEM_IND == pSimSatResponseData->nSAT_CmdType ) // Item selection
        {
          api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
            (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
          CFW_SAT_MENU_RSP* pMenu = (CFW_SAT_MENU_RSP*)(pG_SimSat->pSatComData);

          if ((pSimTerminalResponesReq == NULL) || (pMenu == NULL))
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse1, RESPONSEBASE);
#ifdef CFW_MULTI_SIM
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID];
#else
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatResponseData->nSAT_CmdType - 1];
#endif
          pSimTerminalResponesReq->TerminalRespData[ComType]   = SIM_SAT_SELECT_ITEM_IND;
          pSimTerminalResponesReq->TerminalRespData[ComQual]   = pMenu->nComQualifier;
          pSimTerminalResponesReq->TerminalRespData[ComResult] = pSimSatResponseData->nSAT_Status;

          if( 0x00 == pSimSatResponseData->nSAT_Status )
          {
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseItem, RESPONSEITEM);
            
            *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + ItemSelection) =
              pSimSatResponseData->nSAT_ItemID;

            pSimTerminalResponesReq->NbByte = RESPONSEBASE + RESPONSEITEM;
          }
          else
          {
            pSimTerminalResponesReq->NbByte = RESPONSEBASE;
          }

#ifdef CFW_MULTI_SIM
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
#else
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
#endif

        }
        else if ( SIM_SAT_GET_INPUT_IND == pSimSatResponseData->nSAT_CmdType ) // get input
        {
          api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
            (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
          CFW_SAT_INPUT_RSP* pGetInput = (CFW_SAT_INPUT_RSP*)(pG_SimSat->pSatComData);

          if ((pSimTerminalResponesReq == NULL) || (pGetInput == NULL))
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          pSimTerminalResponesReq->NbByte = RESPONSEBASE + RESPONSETEXT + pSimSatResponseData->nSAT_InputLen - 1; // remove the sch byte
#ifdef CFW_MULTI_SIM
          if (0x00 == nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID])
#else
          if (0x00 == nCommondNum[pSimSatResponseData->nSAT_CmdType - 1])
#endif
          {
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, pSimTerminalResponesReq->NbByte);
          }
          else
          {
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse1, pSimTerminalResponesReq->NbByte);
          }

#ifdef CFW_MULTI_SIM
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID];
#else
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
                  nCommondNum[pSimSatResponseData->nSAT_CmdType - 1];
#endif
          pSimTerminalResponesReq->TerminalRespData[ComType]   = pSimSatResponseData->nSAT_CmdType;
          pSimTerminalResponesReq->TerminalRespData[ComQual]   = pGetInput->nComQualifier;
          pSimTerminalResponesReq->TerminalRespData[ComResult] = pSimSatResponseData->nSAT_Status;

          SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseText, RESPONSETEXT);

          *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + TextLength) = pSimSatResponseData->nSAT_InputLen;  // add scheme
          *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + TextScheme) = pSimSatResponseData->nSAT_InputSch;

          if (pSimSatResponseData->nSAT_InputLen > 1)
          {
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + RESPONSETEXT,
                         pSimSatResponseData->nSAT_InputStr, pSimSatResponseData->nSAT_InputLen - 1);
          }

#ifdef CFW_MULTI_SIM
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
#else
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
#endif
        }
        else if ( SIM_SAT_SMSPP_DOWNLOAD == pSimSatResponseData->nSAT_CmdType ) // SMS Profile download data
        {
          api_SimTerminalEnvelopeReq_t* pSimTerminalEnvelopeReq =
            (api_SimTerminalEnvelopeReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalEnvelopeReq_t));
          if (pSimTerminalEnvelopeReq == NULL)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "SIM_SAT_SMSPP_DOWNLOAD[0xD1] command ERR_NO_MORE_MEMORY \n");
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          api_SmsPPReceiveInd_t* pSMSPP = (api_SmsPPReceiveInd_t*)(pSimSatResponseData->nSAT_InputStr);
          CSW_TRACE(CFW_SIM_TS_ID, "pSMSPP->DataLen %x,pSMSPP->AddressLen %x\n", pSMSPP->DataLen,
                        pSMSPP->AddressLen);
          if(( 4 + 2 + 2 + pSMSPP->AddressLen + pSMSPP->DataLen ) < 0x80)
          {
            SUL_MemCopy8(pSimTerminalEnvelopeReq->EnvelopeData, SMPPDownSelection, 7);

            //Device identities tag Length: 4
              /*
                Byte(s)	Description	Length
                1	Device identities tag	1
                2	Length = '02'	1
                3 	Source device identity	1
                4	Destination device identity	1

              */

            //Address Length: 2
              /*
                Byte(s)	Description	Length
                1	Address tag	1
                2 to (Y 1)+2	Length (X)	Y

              */
              
           //SMS TPDU tag: 2
             /*
                Byte(s)	Description	Length
                1	SMS TPDU tag	1
                2 to (Y 1)+2	Length (X)	Y

              */
            pSimTerminalEnvelopeReq->EnvelopeData[1] = 4 + 2 + 2 + pSMSPP->AddressLen + pSMSPP->DataLen;
            CSW_TRACE(CFW_SIM_TS_ID, "pSMSPP->AddressLen %x pSMSPP->DataLen %x\n", pSMSPP->AddressLen,
                          pSMSPP->DataLen);

            //SMS-PP download tag: 2
              /*
                Description	Section	M/O	Min	Length
                SMS-PP download tag	13.1	M	Y	1
                Length (A+B+C)	-	M	Y	1 or 2

              */
            pSimTerminalEnvelopeReq->NbByte = pSimTerminalEnvelopeReq->EnvelopeData[1] + 2;

            /* 
               A: Device identities
               B: Address
               C: SMS TPDU (SMS-DELIVER)
             */
            pSimTerminalEnvelopeReq->EnvelopeData[7] = pSMSPP->AddressLen;
            SUL_MemCopy8(&(pSimTerminalEnvelopeReq->EnvelopeData[8]), pSMSPP->Address, pSMSPP->AddressLen);

            pSimTerminalEnvelopeReq->EnvelopeData[8 + pSMSPP->AddressLen] = 0x0B;
            pSimTerminalEnvelopeReq->EnvelopeData[8 + pSMSPP->AddressLen + 1] = pSMSPP->DataLen;
            SUL_MemCopy8(&(pSimTerminalEnvelopeReq->EnvelopeData[8 + pSMSPP->AddressLen + 2]), pSMSPP->Data,
                         pSMSPP->DataLen);
          }
          else
          {
            SUL_MemCopy8(pSimTerminalEnvelopeReq->EnvelopeData, SMPPDownSelectionEx, 8);

            //Device identities tag Length: 4
              /*
                Byte(s)	Description	Length
                1	Device identities tag	1
                2	Length = '02'	1
                3 	Source device identity	1
                4	Destination device identity	1

              */

            //Address Length: 2
              /*
                Byte(s)	Description	Length
                1	Address tag	1
                2 to (Y 1)+2	Length (X)	Y

              */
              
           //SMS TPDU tag: 2
             /*
                Byte(s)	Description	Length
                1	SMS TPDU tag	1
                2 to (Y 1)+2	Length (X)	Y

              */
            if( pSMSPP->DataLen < 0x80 )  
              pSimTerminalEnvelopeReq->EnvelopeData[2] = 4 + 2 + 2 + pSMSPP->AddressLen + pSMSPP->DataLen;
            else
              pSimTerminalEnvelopeReq->EnvelopeData[2] = 4 + 2 + 2 + pSMSPP->AddressLen + pSMSPP->DataLen + 1;

            CSW_TRACE(CFW_SIM_TS_ID, "pSMSPP->AddressLen %x pSMSPP->DataLen %x\n", pSMSPP->AddressLen,
                          pSMSPP->DataLen);

            //SMS-PP download tag: 2
              /*
                Description	Section	M/O	Min	Length
                SMS-PP download tag	13.1	M	Y	1
                Length (A+B+C)	-	M	Y	1 or 2

              */
            pSimTerminalEnvelopeReq->NbByte = pSimTerminalEnvelopeReq->EnvelopeData[2] + 3;

            /* 
               A: Device identities
               B: Address
               C: SMS TPDU (SMS-DELIVER)
             */
            pSimTerminalEnvelopeReq->EnvelopeData[8] = pSMSPP->AddressLen;
            SUL_MemCopy8(&(pSimTerminalEnvelopeReq->EnvelopeData[9]), pSMSPP->Address, pSMSPP->AddressLen);

            pSimTerminalEnvelopeReq->EnvelopeData[9 + pSMSPP->AddressLen] = 0x0B;

            if(pSMSPP->DataLen < 0x80)
            {
              pSimTerminalEnvelopeReq->EnvelopeData[9 + pSMSPP->AddressLen + 1] = pSMSPP->DataLen;
              SUL_MemCopy8(&(pSimTerminalEnvelopeReq->EnvelopeData[9 + pSMSPP->AddressLen + 2]), pSMSPP->Data,
                           pSMSPP->DataLen);
            }
            else
            {
              pSimTerminalEnvelopeReq->EnvelopeData[9 + pSMSPP->AddressLen + 1] = 0x81;
              pSimTerminalEnvelopeReq->EnvelopeData[9 + pSMSPP->AddressLen + 2] = pSMSPP->DataLen;
              SUL_MemCopy8(&(pSimTerminalEnvelopeReq->EnvelopeData[9 + pSMSPP->AddressLen + 3]), pSMSPP->Data,
                           pSMSPP->DataLen);
            }

          }

          
          CSW_TRACE(CFW_SIM_TS_ID, "0xD1 CMD TERMINALENVELOPE_REQ \n");

          // CSW_TC_MEMBLOCK(CFW_SIM_TS_ID, (UINT8 *) pSimTerminalEnvelopeReq->EnvelopeData, (UINT16) pSimTerminalEnvelopeReq->NbByte, 16);
#ifdef CFW_MULTI_SIM
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALENVELOPE_REQ, pSimTerminalEnvelopeReq, nSimID))
          {
            CSW_TRACE(CFW_SIM_TS_ID, "0xD1 command ERR_NO_MORE_MEMORY \n");
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#else
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALENVELOPE_REQ, pSimTerminalEnvelopeReq))
          {
            CSW_TRACE(CFW_SIM_TS_ID, "0xD1 command ERR_NO_MORE_MEMORY \n");
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#endif

          CSW_TRACE(CFW_SIM_TS_ID, "0xD1 command OK!!! \n");
          pSimSatResponseData->nSm_Resp_prevStatus = pSimSatResponseData->nSm_Resp_currStatus;
          pSimSatResponseData->nSm_Resp_currStatus = SM_RESP_TERMINALENVELOPE;
          CFW_SetAoProcCode(hSimSatResponseAo, CFW_AO_PROC_CODE_CONSUMED);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
          return ERR_SUCCESS;
        }
        else if ( SIM_SAT_MENU_SELECTION == pSimSatResponseData->nSAT_CmdType ) // menu selection
        {
          api_SimTerminalEnvelopeReq_t* pSimTerminalEnvelopeReq =
            (api_SimTerminalEnvelopeReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalEnvelopeReq_t));
          if (pSimTerminalEnvelopeReq == NULL)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "SIM_SAT_MENU_SELECTION[0xD3] command ERR_NO_MORE_MEMORY \n");
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          pSimTerminalEnvelopeReq->NbByte = 0x09;

          SUL_MemCopy8(pSimTerminalEnvelopeReq->EnvelopeData, MenuSelection, pSimTerminalEnvelopeReq->NbByte);
          pSimTerminalEnvelopeReq->EnvelopeData[ItemNumber] = pSimSatResponseData->nSAT_ItemID; // Select the second item
          CSW_TRACE(CFW_SIM_TS_ID, "SIM_SAT_MENU_SELECTION[0xD3] CMD TERMINALENVELOPE_REQ \n");
#ifdef CFW_MULTI_SIM
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALENVELOPE_REQ, pSimTerminalEnvelopeReq, nSimID))
          {
            CSW_TRACE(CFW_SIM_TS_ID, "SIM_SAT_MENU_SELECTION[0xD3] command ERR_NO_MORE_MEMORY \n");
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#else
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALENVELOPE_REQ, pSimTerminalEnvelopeReq))
          {
            CSW_TRACE(CFW_SIM_TS_ID, "SIM_SAT_MENU_SELECTION[0xD3] command ERR_NO_MORE_MEMORY \n");
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#endif
          CSW_TRACE(CFW_SIM_TS_ID, "SIM_SAT_MENU_SELECTION[0xD3] command OK!!! \n");
          pSimSatResponseData->nSm_Resp_prevStatus = pSimSatResponseData->nSm_Resp_currStatus;
          pSimSatResponseData->nSm_Resp_currStatus = SM_RESP_TERMINALENVELOPE;
          CFW_SetAoProcCode(hSimSatResponseAo, CFW_AO_PROC_CODE_CONSUMED);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
          return ERR_SUCCESS;
        }
        else if ( SIM_SAT_EVENT_DOWNLOAD == pSimSatResponseData->nSAT_CmdType ) // menu selection
        {
          api_SimTerminalEnvelopeReq_t* pSimTerminalEnvelopeReq =
            (api_SimTerminalEnvelopeReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalEnvelopeReq_t));
          if (pSimTerminalEnvelopeReq == NULL)
          {
            CSW_TRACE(CFW_SIM_TS_ID, "SIM_SAT_EVENT_DOWNLOAD[0xD6] command ERR_NO_MORE_MEMORY \n");
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          pSimTerminalEnvelopeReq->NbByte = 0x09;

          SUL_MemCopy8(pSimTerminalEnvelopeReq->EnvelopeData, EventDownload, pSimTerminalEnvelopeReq->NbByte);
          pSimTerminalEnvelopeReq->EnvelopeData[ItemNumber] = pSimSatResponseData->nSAT_ItemID; // Select the second item
          CSW_TRACE(CFW_SIM_TS_ID, "SIM_SAT_EVENT_DOWNLOAD[0xD6] CMD TERMINALENVELOPE_REQ \n");
#ifdef CFW_MULTI_SIM
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALENVELOPE_REQ, pSimTerminalEnvelopeReq, nSimID))
          {
            CSW_TRACE(CFW_SIM_TS_ID, "SIM_SAT_EVENT_DOWNLOAD[0xD6] command ERR_NO_MORE_MEMORY \n");
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#else
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALENVELOPE_REQ, pSimTerminalEnvelopeReq))
          {
            CSW_TRACE(CFW_SIM_TS_ID, "SIM_SAT_EVENT_DOWNLOAD[0xD6] command ERR_NO_MORE_MEMORY \n");
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#endif
          CSW_TRACE(CFW_SIM_TS_ID, "SIM_SAT_EVENT_DOWNLOAD[0xD6] command OK!!! \n");
          pSimSatResponseData->nSm_Resp_prevStatus = pSimSatResponseData->nSm_Resp_currStatus;
          pSimSatResponseData->nSm_Resp_currStatus = SM_RESP_TERMINALENVELOPE;
          CFW_SetAoProcCode(hSimSatResponseAo, CFW_AO_PROC_CODE_CONSUMED);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
          return ERR_SUCCESS;
        }
        else if (SIM_SAT_POLL_INTERVAL_IND == pSimSatResponseData->nSAT_CmdType)
        {
          api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
            (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
          SAT_BASE_RSP* pBase = (SAT_BASE_RSP*)(pG_SimSat->pSatComData);

          if ((pSimTerminalResponesReq == NULL) || (pBase == NULL))
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

          pSimTerminalResponesReq->NbByte = RESPONSEBASE + 4;

          // 次数有些特别，目前发现一张特别的卡会上该Fetch Ind,单独先进行Command number 0x01的处理
          // 如果失败再进行0x00的处理          
          SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse1, RESPONSEBASE);

#ifdef CFW_MULTI_SIM
    pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID];
#else
    pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatResponseData->nSAT_CmdType - 1];
#endif
      
          pSimTerminalResponesReq->TerminalRespData[ComType]   = pSimSatResponseData->nSAT_CmdType;
          pSimTerminalResponesReq->TerminalRespData[ComQual]   = pBase->nComQualifier;
          pSimTerminalResponesReq->TerminalRespData[ComResult] = pSimSatResponseData->nSAT_Status;

          pSimTerminalResponesReq->TerminalRespData[0x0c] = 0x84;
          pSimTerminalResponesReq->TerminalRespData[0x0d] = 0x02;
          pSimTerminalResponesReq->TerminalRespData[0x0e] = 0x02;
          pSimTerminalResponesReq->TerminalRespData[0x0f] = 0x01;

          CSW_TRACE(CFW_SIM_TS_ID, "nSAT_CmdType = %x\n", pSimSatResponseData->nSAT_CmdType);
          CSW_TRACE(CFW_SIM_TS_ID, "nComQualifier = %x\n", pBase->nComQualifier);
          CSW_TRACE(CFW_SIM_TS_ID, "nSAT_Status = %x\n", pSimSatResponseData->nSAT_Status);
#ifdef CFW_MULTI_SIM
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_SCL_QUEUE_FULL, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#else
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_SCL_QUEUE_FULL, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#endif
        }
        else if( SIM_SAT_SEND_USSD_IND == pSimSatResponseData->nSAT_CmdType ) 
        {
          // USSD String

          api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
            (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
          SAT_BASE_RSP* pBase = (SAT_BASE_RSP*)(pG_SimSat->pSatComData);

          if ((pSimTerminalResponesReq == NULL) || (pBase == NULL))
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

          pSimTerminalResponesReq->NbByte = RESPONSEBASE + RESPONSEUSSD + pSimSatResponseData->nSAT_InputLen - 1;

          // SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, RESPONSEBASE);
#ifdef CFW_MULTI_SIM
          if (0x00 == nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID])
#else
          if (0x00 == nCommondNum[pSimSatResponseData->nSAT_CmdType - 1])
#endif
          SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, pSimTerminalResponesReq->NbByte);
          else
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse1, pSimTerminalResponesReq->NbByte);

#ifdef CFW_MULTI_SIM
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
          nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID];
#else
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
          nCommondNum[pSimSatResponseData->nSAT_CmdType - 1];
#endif     

          // pSimTerminalResponesReq->TerminalRespData[ComType -1]   = 0x02;

          pSimTerminalResponesReq->TerminalRespData[ComType]   = pSimSatResponseData->nSAT_CmdType;
          pSimTerminalResponesReq->TerminalRespData[ComQual]   = pBase->nComQualifier;
          pSimTerminalResponesReq->TerminalRespData[ComResult] = pSimSatResponseData->nSAT_Status;

          CSW_TRACE(CFW_SIM_TS_ID, "nSAT_CmdType    %x\n", pSimSatResponseData->nSAT_CmdType);
          CSW_TRACE(CFW_SIM_TS_ID, "nComQualifier   %x\n", pBase->nComQualifier);
          CSW_TRACE(CFW_SIM_TS_ID, "nSAT_Status     %x\n", pSimSatResponseData->nSAT_Status);

          if(pSimSatResponseData->nSAT_InputLen < 0x7F)
          {
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseUSSD, RESPONSEUSSD);

            *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + TextLength) = pSimSatResponseData->nSAT_InputLen;  // add Len

            if (pSimSatResponseData->nSAT_InputLen > 1)
            {
              SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + 2,
                           pSimSatResponseData->nSAT_InputStr, pSimSatResponseData->nSAT_InputLen);
            }
          }
          else
          {
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseUSSDEX, RESPONSEUSSD+1);

            *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + TextLength+1) = pSimSatResponseData->nSAT_InputLen;  // add Len

            if (pSimSatResponseData->nSAT_InputLen > 1)
            {
              SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + 3,
                           pSimSatResponseData->nSAT_InputStr, pSimSatResponseData->nSAT_InputLen);
            }
            pSimTerminalResponesReq->NbByte ++;
          }

          // CSW_TC_MEMBLOCK(CFW_SIM_TS_ID, (UINT8 *) pSimTerminalResponesReq, sizeof(api_SimTerminalResponseReq_t), 16);
#ifdef CFW_MULTI_SIM
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_SCL_QUEUE_FULL, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#else
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_SCL_QUEUE_FULL, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#endif

        }
        else if ( SIM_SAT_PROVIDE_LOCAL_INFO_IND == pSimSatResponseData->nSAT_CmdType ) // location infomation
        {
          api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
            (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
          SAT_BASE_RSP* pBase = (SAT_BASE_RSP*)(pG_SimSat->pSatComData);

          SUL_MemSet8(pSimTerminalResponesReq, 0x00, sizeof(api_SimTerminalResponseReq_t));
          if ((pSimTerminalResponesReq == NULL) || (pBase == NULL))
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          pSimTerminalResponesReq->NbByte = RESPONSEBASE;
          SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse1, RESPONSEBASE);

#ifdef CFW_MULTI_SIM
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID];

#else
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatResponseData->nSAT_CmdType - 1];

#endif

          pSimTerminalResponesReq->TerminalRespData[ComType]   = pSimSatResponseData->nSAT_CmdType;
          pSimTerminalResponesReq->TerminalRespData[ComQual]   = pBase->nComQualifier;
          pSimTerminalResponesReq->TerminalRespData[ComResult] = pSimSatResponseData->nSAT_Status;
          CSW_TRACE(CFW_SIM_TS_ID, "pBase->nComQualifier   %x\n", pBase->nComQualifier);
          if (pBase->nComQualifier == 0x00)
          {
            SAT_LOCALINFO_RSP* pLocalInfo = NULL;
            pSimTerminalResponesReq->NbByte += RESPONSELOCAL;
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseLocalInfo, RESPONSELOCAL);
            pLocalInfo = (SAT_LOCALINFO_RSP*)pSimSatResponseData->nSAT_InputStr;
            if (pLocalInfo == NULL)
            {
#ifdef CFW_MULTI_SIM
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
              return ERR_SUCCESS;
            }
      
            *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + MCC_MNC) =
              (pLocalInfo->nMCC_MNC_Code[0] & 0x0F) + ((pLocalInfo->nMCC_MNC_Code[1] << 4) & 0xF0);;
            *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + MCC_MNC + 1) =
              (pLocalInfo->nMCC_MNC_Code[2] & 0x0F) + ((pLocalInfo->nMCC_MNC_Code[5] << 4) & 0xF0);
            *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + MCC_MNC + 2) =
              (pLocalInfo->nMCC_MNC_Code[3] & 0x0F) + ((pLocalInfo->nMCC_MNC_Code[4] << 4) & 0xF0);

            *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + LAC) =
              (UINT8)(pLocalInfo->nLocalAreaCode >> 8);
            *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + LAC + 1) =
              (UINT8)(pLocalInfo->nLocalAreaCode & 0xFF);

            *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + CellID) =
              (UINT8)(pLocalInfo->nCellIdentify >> 8);
            *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + CellID + 1) =
              (UINT8)(pLocalInfo->nCellIdentify & 0xFF);
          }
          else if (pBase->nComQualifier == 0x01)
          {
            SAT_IMEI_RSP* pIMEI = NULL;
            pSimTerminalResponesReq->NbByte += RESPONSEIMEI;
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseIMEI, RESPONSEIMEI);
            pIMEI = (SAT_IMEI_RSP*)pSimSatResponseData->nSAT_InputStr;
            if (pIMEI == NULL)
            {
#ifdef CFW_MULTI_SIM
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
              return ERR_SUCCESS;
            }
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + IMEI, pIMEI->nInterMobileId, 8);
          }
          else if (pBase->nComQualifier == 0x02)
          {
            SAT_MEASURE_RSP* pMeasure = NULL;
            pSimTerminalResponesReq->NbByte += RESPONSEMEASURE;
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseMeasure, RESPONSEMEASURE);
            pMeasure = (SAT_MEASURE_RSP*)pSimSatResponseData->nSAT_InputStr;
            if (pMeasure == NULL)
            {
#ifdef CFW_MULTI_SIM
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
              return ERR_SUCCESS;
            }
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + Measurement, pMeasure->nMeasure,
                         16);
          }
          else if (pBase->nComQualifier == 0x03)
          {
            SAT_DATETIME_RSP* pDatetime = NULL;
            pSimTerminalResponesReq->NbByte += RESPONSEDATETIME;
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseDateTime, RESPONSEDATETIME);
            pDatetime = (SAT_DATETIME_RSP*)pSimSatResponseData->nSAT_InputStr;
            if (pDatetime == NULL)
            {
#ifdef CFW_MULTI_SIM
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
              return ERR_SUCCESS;
            }
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + DateTime, pDatetime->nDateTimeZone,
                         7);
          }
          else if (pBase->nComQualifier == 0x04)
          {
            SAT_LANGUAGE_RSP* pLanguage = NULL;
            pSimTerminalResponesReq->NbByte += RESPONSELANGUAGE;
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseLanguage, RESPONSELANGUAGE);
            pLanguage = (SAT_LANGUAGE_RSP*)pSimSatResponseData->nSAT_InputStr;
            if (pLanguage == NULL)
            {
#ifdef CFW_MULTI_SIM
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
              return ERR_SUCCESS;
            }
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + Language, pLanguage->nLanguageCode,
                         2);
          }
          else if (pBase->nComQualifier == 0x05)
          {
            SAT_TIMINGADV_RSP* pTimingAdv = NULL;
            pSimTerminalResponesReq->NbByte += RESPONSETADVANCE;
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE,
                         ResponseTimingAdvance, RESPONSETADVANCE);
            pTimingAdv = (SAT_TIMINGADV_RSP*)pSimSatResponseData->nSAT_InputStr;
            if (pTimingAdv == NULL)
            {
#ifdef CFW_MULTI_SIM
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
              return ERR_SUCCESS;
            }
            *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + MEStatus) = pTimingAdv->nME_Status;
            *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + TimingAdvance) = pTimingAdv->nTimingAdv;
          }

          // CSW_TC_MEMBLOCK(CFW_SIM_TS_ID, (UINT8 *) pSimTerminalResponesReq, sizeof(api_SimTerminalResponseReq_t), 16);
#ifdef CFW_MULTI_SIM
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#else
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#endif
        }

        // else if(pSimSatResponseData->nSAT_CmdType == 0x31)    // power on card
        // {
        // StackCom does not support!
        // }
        else if( SIM_SAT_GET_READER_STATUS_IND == pSimSatResponseData->nSAT_CmdType ) // Get Reader status
        {
          api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
            (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
          SAT_BASE_RSP* pBase = (SAT_BASE_RSP*)(pG_SimSat->pSatComData);

          if ((pSimTerminalResponesReq == NULL) || (pBase == NULL))
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          pSimTerminalResponesReq->NbByte = RESPONSEBASE;

          // SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, RESPONSEBASE);
#ifdef CFW_MULTI_SIM
          if (0x00 == nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID])
#else
          if (0x00 == nCommondNum[pSimSatResponseData->nSAT_CmdType - 1])
#endif
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, pSimTerminalResponesReq->NbByte);
          else
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse1, pSimTerminalResponesReq->NbByte);

#ifdef CFW_MULTI_SIM
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID];
#else
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatResponseData->nSAT_CmdType - 1];
#endif

          pSimTerminalResponesReq->TerminalRespData[ComType]   = pSimSatResponseData->nSAT_CmdType;
          pSimTerminalResponesReq->TerminalRespData[ComQual]   = pBase->nComQualifier;
          pSimTerminalResponesReq->TerminalRespData[ComResult] = pSimSatResponseData->nSAT_Status;

          if (pBase->nComQualifier == 0x00) // Card reader status
          {
            pSimTerminalResponesReq->NbByte += RESPONSECARDSTATUS;
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE,
                         ResponseCardStatus, RESPONSECARDSTATUS);
            *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + CardStatus) =
              pSimSatResponseData->nSAT_InputLen;
          }
          else if (pBase->nComQualifier == 0x01)  // Card reader identify
          {
            pSimTerminalResponesReq->NbByte += RESPONSECARDID;
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseCardStatus, RESPONSECARDID);

            *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + CardID_Length) =
              pSimSatResponseData->nSAT_InputLen;
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + CardID,
                         pSimSatResponseData->nSAT_InputStr, pSimSatResponseData->nSAT_InputLen);
          }
          else
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#ifdef CFW_MULTI_SIM

          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#else

          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#endif

        }
        else if ( SIM_SAT_GET_CHANNEL_STATUS_IND == pSimSatResponseData->nSAT_CmdType ) // Get channel status
        {
          api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
            (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
          SAT_BASE_RSP* pBase = (SAT_BASE_RSP*)(pG_SimSat->pSatComData);
          //SAT_CHSTATUS_RSP* pChannelStatus = NULL;

          if ((pSimTerminalResponesReq == NULL) || (pBase == NULL))
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          pSimTerminalResponesReq->NbByte = RESPONSEBASE + RESPONSELOCAL;

#ifdef CFW_MULTI_SIM
          if (0x00 == nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID])
#else
          if (0x00 == nCommondNum[pSimSatResponseData->nSAT_CmdType - 1])
#endif
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, pSimTerminalResponesReq->NbByte);
          else
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse1, pSimTerminalResponesReq->NbByte);

#ifdef CFW_MULTI_SIM
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID];
#else
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatResponseData->nSAT_CmdType - 1];
#endif
    
          pSimTerminalResponesReq->TerminalRespData[ComType]   = pSimSatResponseData->nSAT_CmdType;
          pSimTerminalResponesReq->TerminalRespData[ComQual]   = pBase->nComQualifier;
          pSimTerminalResponesReq->TerminalRespData[ComResult] = pSimSatResponseData->nSAT_Status;

          SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseLocalInfo, RESPONSECHANNEL);
         // *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + RESPONSECHANNEL) =
         //  (pChannelStatus->nChannelID) | (pChannelStatus->nStatus << 7);
         // *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + RESPONSECHANNEL + 1) = pChannelStatus->nType;
#ifdef CFW_MULTI_SIM
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
#else
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
#endif
        }
        else
        { 
          api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
            (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
          SAT_BASE_RSP* pBase = (SAT_BASE_RSP*)(pG_SimSat->pSatComData);

          if ((pSimTerminalResponesReq == NULL) || (pBase == NULL))
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

          pSimTerminalResponesReq->NbByte = RESPONSEBASE;

          // SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, RESPONSEBASE);
#ifdef CFW_MULTI_SIM
          if (0x00 == nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID])
#else
          if (0x00 == nCommondNum[pSimSatResponseData->nSAT_CmdType - 1])
#endif
          SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, pSimTerminalResponesReq->NbByte);
          else
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse1, pSimTerminalResponesReq->NbByte);

#ifdef CFW_MULTI_SIM
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
          nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID];
#else
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
          nCommondNum[pSimSatResponseData->nSAT_CmdType - 1];
#endif     

          // pSimTerminalResponesReq->TerminalRespData[ComType -1]   = 0x02;

          pSimTerminalResponesReq->TerminalRespData[ComType]   = pSimSatResponseData->nSAT_CmdType;
          pSimTerminalResponesReq->TerminalRespData[ComQual]   = pBase->nComQualifier;
          pSimTerminalResponesReq->TerminalRespData[ComResult] = pSimSatResponseData->nSAT_Status;

          CSW_TRACE(CFW_SIM_TS_ID, "nSAT_CmdType    %x\n", pSimSatResponseData->nSAT_CmdType);
          CSW_TRACE(CFW_SIM_TS_ID, "nComQualifier   %x\n", pBase->nComQualifier);
          CSW_TRACE(CFW_SIM_TS_ID, "nSAT_Status     %x\n", pSimSatResponseData->nSAT_Status);

          // CSW_TC_MEMBLOCK(CFW_SIM_TS_ID, (UINT8 *) pSimTerminalResponesReq, sizeof(api_SimTerminalResponseReq_t), 16);
#ifdef CFW_MULTI_SIM 
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_SCL_QUEUE_FULL, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#else
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_SCL_QUEUE_FULL, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#endif

        }

        CFW_SetAoProcCode(hSimSatResponseAo, CFW_AO_PROC_CODE_CONSUMED);
        pSimSatResponseData->nSm_Resp_prevStatus = pSimSatResponseData->nSm_Resp_currStatus;
        pSimSatResponseData->nSm_Resp_currStatus = SM_RESP_TERMINALRESPONSE;
      }
      else
        CFW_SetAoProcCode(hSimSatResponseAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_RESP_TERMINALRESPONSE:
    {
      if (nEvtId == API_SIM_TERMINALRESPONSE_CNF)
      {
        api_SimTerminalResponseCnf_t* pSimTerminalResponeCnf = (api_SimTerminalResponseCnf_t*)nEvParam;
        CSW_TRACE(CFW_SIM_TS_ID, "Sw1[0x%x]Sw2[0x%x] nTRTryNum[0x%x]\n", pSimTerminalResponeCnf->Sw1,
                      pSimTerminalResponeCnf->Sw2, nTRTryNum);

        CFW_SetAoProcCode(hSimSatResponseAo, CFW_AO_PROC_CODE_CONSUMED);
        pSimTerminalResponeCnf = (api_SimTerminalResponseCnf_t*)nEvParam;
        if (pSimTerminalResponeCnf->Sw1 != 0x91)
        {
          if (pSimTerminalResponeCnf->Sw1 != 0x90)
          {
            if (nTRTryNum == 0x00)
            {
              nTRTryNum = 0x01;
            }
            else
            {
              UINT32 nErrorCode = 0x00;

#ifdef CFW_MULTI_SIM
              nErrorCode = Sim_ParseSW1SW2(pSimTerminalResponeCnf->Sw1, pSimTerminalResponeCnf->Sw2, nSimID);
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, nErrorCode, pSimTerminalResponeCnf->Sw1,
                                  nUTI | (nSimID << 24), 0xF0);
#else
              nErrorCode = Sim_ParseSW1SW2(pSimTerminalResponeCnf->Sw1, pSimTerminalResponeCnf->Sw2);
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, nErrorCode, pSimTerminalResponeCnf->Sw1, nUTI, 0xF0);
#endif
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
              return ERR_SUCCESS;
            }

            // 当错误为0x6f的时候，按照3Gpp的协议应为technical problem with no diagnostic given，
            // 这个时候进行对Command Number的重新尝试。如下代码可以提取到一个函数中，有待优化。
            // 另外目前只发现返回错误码为0x6f的时候，需要重新尝试，其他情况，我验证的各种卡没有发现。
            // 如果发现，需要将错误码加入
            if ((pSimTerminalResponeCnf->Sw1 == 0x6f) & (pSimTerminalResponeCnf->Sw1 == 0x93))
            {
              CSW_TRACE(CFW_SIM_TS_ID, "nSAT_CmdType %x\n", pSimSatResponseData->nSAT_CmdType);
              if (pSimSatResponseData->nSAT_CmdType == 0x25)  // response the envolope
              {
                api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
                  (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SimTerminalResponseReq_t));
                CFW_SAT_MENU_RSP* pMenu = (CFW_SAT_MENU_RSP*)(pG_SimSat->pSetupMenu);

                if ((pSimTerminalResponesReq == NULL) || (pMenu == NULL))
                {
#ifdef CFW_MULTI_SIM
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }

                pSimTerminalResponesReq->NbByte = 12;
                SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse1,
                             pSimTerminalResponesReq->NbByte);
                pSimTerminalResponesReq->TerminalRespData[ComType]   = 0x25;
                pSimTerminalResponesReq->TerminalRespData[ComQual]   = pMenu->nComQualifier;
                pSimTerminalResponesReq->TerminalRespData[ComResult] = pSimSatResponseData->nSAT_Status;
#ifdef CFW_MULTI_SIM
                if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
                {
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }

#else
                if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
                {
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }

#endif
              }
              else if (pSimSatResponseData->nSAT_CmdType == 0x24) // Item selection
              {
                api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
                  (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
                CFW_SAT_MENU_RSP* pMenu = (CFW_SAT_MENU_RSP*)(pG_SimSat->pSatComData);

                if ((pSimTerminalResponesReq == NULL) || (pMenu == NULL))
                {
#ifdef CFW_MULTI_SIM
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }
                SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, RESPONSEBASE);
                pSimTerminalResponesReq->TerminalRespData[ComType]   = 0x24;
                pSimTerminalResponesReq->TerminalRespData[ComQual]   = pMenu->nComQualifier;
                pSimTerminalResponesReq->TerminalRespData[ComResult] = pSimSatResponseData->nSAT_Status;

                SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseItem, RESPONSEITEM);
                *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + ItemSelection) =
                  pSimSatResponseData->nSAT_ItemID;

                pSimTerminalResponesReq->NbByte = RESPONSEBASE + RESPONSEITEM;
#ifdef CFW_MULTI_SIM
                if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
                {
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }

#else
                if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
                {
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }
#endif

              }
              else if (pSimSatResponseData->nSAT_CmdType == 0x23)
              {
                api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
                  (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
                CFW_SAT_INPUT_RSP* pGetInput = (CFW_SAT_INPUT_RSP*)(pG_SimSat->pSatComData);

                if ((pSimTerminalResponesReq == NULL) || (pGetInput == NULL))
                {
#ifdef CFW_MULTI_SIM
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }
                pSimTerminalResponesReq->NbByte = RESPONSEBASE + RESPONSETEXT + pSimSatResponseData->nSAT_InputLen;
                SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, RESPONSEBASE);
                pSimTerminalResponesReq->TerminalRespData[ComType]   = pSimSatResponseData->nSAT_CmdType;
                pSimTerminalResponesReq->TerminalRespData[ComQual]   = pGetInput->nComQualifier;
                pSimTerminalResponesReq->TerminalRespData[ComResult] = pSimSatResponseData->nSAT_Status;

                SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseText, RESPONSETEXT);
                *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + TextLength) = pSimSatResponseData->nSAT_InputLen;  // add scheme
                *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + TextScheme) =
                  pSimSatResponseData->nSAT_InputSch;

                SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + RESPONSETEXT,
                             pSimSatResponseData->nSAT_InputStr, pSimSatResponseData->nSAT_InputLen);
#ifdef CFW_MULTI_SIM
                if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
                {
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }
#else
                if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
                {
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }
#endif
              }
              else if (pSimSatResponseData->nSAT_CmdType == 0x03)
              {
                api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
                  (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
                SAT_BASE_RSP* pBase = (SAT_BASE_RSP*)(pG_SimSat->pSatComData);

                if ((pSimTerminalResponesReq == NULL) || (pBase == NULL))
                {
#ifdef CFW_MULTI_SIM
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }

                pSimTerminalResponesReq->NbByte = RESPONSEBASE + 4;
                SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse1, RESPONSEBASE);
                pSimTerminalResponesReq->TerminalRespData[ComType]   = pSimSatResponseData->nSAT_CmdType;
                pSimTerminalResponesReq->TerminalRespData[ComQual]   = pBase->nComQualifier;
                pSimTerminalResponesReq->TerminalRespData[ComResult] = pSimSatResponseData->nSAT_Status;

                // pSimTerminalResponesReq->TerminalRespData[ComResult -1] = 0x05;         
                pSimTerminalResponesReq->TerminalRespData[0x0c] = 0x84;
                pSimTerminalResponesReq->TerminalRespData[0x0d] = 0x02;
                pSimTerminalResponesReq->TerminalRespData[0x0e] = 0x02;
                pSimTerminalResponesReq->TerminalRespData[0x0f] = 0x01;

                CSW_TRACE(CFW_SIM_TS_ID, "nSAT_CmdType  %x\n", pSimSatResponseData->nSAT_CmdType);
                CSW_TRACE(CFW_SIM_TS_ID, "nComQualifier %x\n", pBase->nComQualifier);
                CSW_TRACE(CFW_SIM_TS_ID, "nSAT_Status = %x\n", pSimSatResponseData->nSAT_Status);
#ifdef CFW_MULTI_SIM
                if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
                {
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_SCL_QUEUE_FULL, 0, nUTI | (nSimID << 24), 0xF0);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }
#else
                if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
                {
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_SCL_QUEUE_FULL, 0, nUTI, 0xF0);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }
#endif
              }
              else if ((pSimSatResponseData->nSAT_CmdType == 0x13) || (pSimSatResponseData->nSAT_CmdType == 0x21)
                       || (pSimSatResponseData->nSAT_CmdType == 0x10) || (pSimSatResponseData->nSAT_CmdType == 0x02)
                       || (pSimSatResponseData->nSAT_CmdType == 0x04) || (pSimSatResponseData->nSAT_CmdType == 0x32))

              { // SMS or Display Text or setup call

                api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
                  (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
                SAT_BASE_RSP* pBase = (SAT_BASE_RSP*)(pG_SimSat->pSatComData);

                if ((pSimTerminalResponesReq == NULL) || (pBase == NULL))
                {
#ifdef CFW_MULTI_SIM
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }

                pSimTerminalResponesReq->NbByte = RESPONSEBASE;
                SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse1, RESPONSEBASE);
                pSimTerminalResponesReq->TerminalRespData[ComType]   = pSimSatResponseData->nSAT_CmdType;
                pSimTerminalResponesReq->TerminalRespData[ComQual]   = pBase->nComQualifier;
                pSimTerminalResponesReq->TerminalRespData[ComResult] = pSimSatResponseData->nSAT_Status;

                CSW_TRACE(CFW_SIM_TS_ID, "nSAT_CmdType  %x\n", pSimSatResponseData->nSAT_CmdType);
                CSW_TRACE(CFW_SIM_TS_ID, "nComQualifier %x\n", pBase->nComQualifier);
                CSW_TRACE(CFW_SIM_TS_ID, "nSAT_Status   %x\n", pSimSatResponseData->nSAT_Status);
#ifdef CFW_MULTI_SIM
                if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
                {
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_SCL_QUEUE_FULL, 0, nUTI | (nSimID << 24), 0xF0);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }

#else
                if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
                {
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_SCL_QUEUE_FULL, 0, nUTI, 0xF0);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }
#endif
              }
              else if (pSimSatResponseData->nSAT_CmdType == 0x26)
              {
                api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
                  (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
                SAT_BASE_RSP* pBase = (SAT_BASE_RSP*)(pG_SimSat->pSatComData);

                if ((pSimTerminalResponesReq == NULL) || (pBase == NULL))
                {
#ifdef CFW_MULTI_SIM
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }
                pSimTerminalResponesReq->NbByte = RESPONSEBASE;
                SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse1, RESPONSEBASE);
                pSimTerminalResponesReq->TerminalRespData[ComType]   = pSimSatResponseData->nSAT_CmdType;
                pSimTerminalResponesReq->TerminalRespData[ComQual]   = pBase->nComQualifier;
                pSimTerminalResponesReq->TerminalRespData[ComResult] = pSimSatResponseData->nSAT_Status;

                if (pBase->nComQualifier == 0x00)
                {
                  SAT_LOCALINFO_RSP* pLocalInfo = NULL;
                  pSimTerminalResponesReq->NbByte += RESPONSELOCAL;
                  SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseLocalInfo,
                               RESPONSELOCAL);
                  pLocalInfo = (SAT_LOCALINFO_RSP*)pSimSatResponseData->nSAT_InputStr;
                  if (pLocalInfo == NULL)
                  {
#ifdef CFW_MULTI_SIM
                    CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
                    CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
                    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                    CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                    return ERR_SUCCESS;
                  }

                  // modify by wulc for bug 13366        
                  *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + MCC_MNC) =
                    (pLocalInfo->nMCC_MNC_Code[0] << 4) | (pLocalInfo->nMCC_MNC_Code[1]);
                  *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + MCC_MNC + 1) =
                    (pLocalInfo->nMCC_MNC_Code[2] << 4) | (pLocalInfo->nMCC_MNC_Code[3]);
                  *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + MCC_MNC + 2) =
                    (pLocalInfo->nMCC_MNC_Code[4] << 4) | (pLocalInfo->nMCC_MNC_Code[5]);

                  *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + LAC) =
                    (UINT8)(pLocalInfo->nLocalAreaCode >> 8);
                  *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + LAC + 1) =
                    (UINT8)(pLocalInfo->nLocalAreaCode & 0xFF);

                  *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + CellID) =
                    (UINT8)(pLocalInfo->nCellIdentify >> 8);
                  *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + CellID + 1) =
                    (UINT8)(pLocalInfo->nCellIdentify & 0xFF);
                }
                else if (pBase->nComQualifier == 0x01)
                {
                  SAT_IMEI_RSP* pIMEI = NULL;
                  pSimTerminalResponesReq->NbByte += RESPONSEIMEI;
                  SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseIMEI, RESPONSEIMEI);
                  pIMEI = (SAT_IMEI_RSP*)pSimSatResponseData->nSAT_InputStr;
                  if (pIMEI == NULL)
                  {
#ifdef CFW_MULTI_SIM
                    CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
                    CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
                    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                    CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                    return ERR_SUCCESS;
                  }
                  SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + IMEI, pIMEI->nInterMobileId,
                               8);
                }
                else if (pBase->nComQualifier == 0x02)
                {
                  SAT_MEASURE_RSP* pMeasure = NULL;
                  pSimTerminalResponesReq->NbByte += RESPONSEMEASURE;
                  SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseMeasure,
                               RESPONSEMEASURE);
                  pMeasure = (SAT_MEASURE_RSP*)pSimSatResponseData->nSAT_InputStr;
                  if (pMeasure == NULL)
                  {
#ifdef CFW_MULTI_SIM
                    CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
                    CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
                    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                    CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                    return ERR_SUCCESS;
                  }
                  SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + Measurement,
                               pMeasure->nMeasure, 16);
                }
                else if (pBase->nComQualifier == 0x03)
                {
                  SAT_DATETIME_RSP* pDatetime = NULL;
                  pSimTerminalResponesReq->NbByte += RESPONSEDATETIME;
                  SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseDateTime,
                               RESPONSEDATETIME);
                  pDatetime = (SAT_DATETIME_RSP*)pSimSatResponseData->nSAT_InputStr;
                  if (pDatetime == NULL)
                  {
#ifdef CFW_MULTI_SIM
                    CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
                    CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
                    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                    CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                    return ERR_SUCCESS;
                  }
                  SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + DateTime,
                               pDatetime->nDateTimeZone, 7);
                }
                else if (pBase->nComQualifier == 0x04)
                {
                  SAT_LANGUAGE_RSP* pLanguage = NULL;
                  pSimTerminalResponesReq->NbByte += RESPONSELANGUAGE;
                  SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseLanguage,
                               RESPONSELANGUAGE);
                  pLanguage = (SAT_LANGUAGE_RSP*)pSimSatResponseData->nSAT_InputStr;
                  if (pLanguage == NULL)
                  {
#ifdef CFW_MULTI_SIM
                    CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
                    CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
                    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                    CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                    return ERR_SUCCESS;
                  }
                  SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + Language,
                               pLanguage->nLanguageCode, 2);
                }
                else if (pBase->nComQualifier == 0x05)
                {
                  SAT_TIMINGADV_RSP* pTimingAdv = NULL;
                  pSimTerminalResponesReq->NbByte += RESPONSETADVANCE;
                  SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE,
                               ResponseTimingAdvance, RESPONSETADVANCE);
                  pTimingAdv = (SAT_TIMINGADV_RSP*)pSimSatResponseData->nSAT_InputStr;
                  if (pTimingAdv == NULL)
                  {
#ifdef CFW_MULTI_SIM
                    CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
                    CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
                    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                    CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                    return ERR_SUCCESS;
                  }
                  *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + MEStatus) = pTimingAdv->nME_Status;
                  *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + TimingAdvance) = pTimingAdv->nTimingAdv;
                }
#ifdef CFW_MULTI_SIM
                CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID);
#else
                CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq);
#endif
              }

              // else if(pSimSatResponseData->nSAT_CmdType == 0x31)    // power on card
              // {
              // StackCom does not support!
              // }
              else if (pSimSatResponseData->nSAT_CmdType == 0x33) // Get Reader status
              {
                api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
                  (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
                SAT_BASE_RSP* pBase = (SAT_BASE_RSP*)(pG_SimSat->pSatComData);

                if ((pSimTerminalResponesReq == NULL) || (pBase == NULL))
                {
#ifdef CFW_MULTI_SIM
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }
                pSimTerminalResponesReq->NbByte = RESPONSEBASE;
                SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse1, RESPONSEBASE);
                pSimTerminalResponesReq->TerminalRespData[ComType]   = pSimSatResponseData->nSAT_CmdType;
                pSimTerminalResponesReq->TerminalRespData[ComQual]   = pBase->nComQualifier;
                pSimTerminalResponesReq->TerminalRespData[ComResult] = pSimSatResponseData->nSAT_Status;

                if (pBase->nComQualifier == 0x00) // Card reader status
                {
                  pSimTerminalResponesReq->NbByte += RESPONSECARDSTATUS;
                  SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE,
                               ResponseCardStatus, RESPONSECARDSTATUS);
                  *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + CardStatus) =
                    pSimSatResponseData->nSAT_InputLen;
                }
                else if (pBase->nComQualifier == 0x01)  // Card reader identify
                {
                  pSimTerminalResponesReq->NbByte += RESPONSECARDID;
                  SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseCardStatus,
                               RESPONSECARDID);

                  *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + CardID_Length) =
                    pSimSatResponseData->nSAT_InputLen;
                  SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + CardID,
                               pSimSatResponseData->nSAT_InputStr, pSimSatResponseData->nSAT_InputLen);
                }
                else
                {
#ifdef CFW_MULTI_SIM
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }

#ifdef CFW_MULTI_SIM
                if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
                {
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }
#else
                if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
                {
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }

#endif
              }
              else if (pSimSatResponseData->nSAT_CmdType == 0x44) // Get channel status
              {
                api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
                  (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
                SAT_BASE_RSP* pBase = (SAT_BASE_RSP*)(pG_SimSat->pSatComData);
                //SAT_CHSTATUS_RSP* pChannelStatus = NULL;

                if ((pSimTerminalResponesReq == NULL) || (pBase == NULL))
                {
#ifdef CFW_MULTI_SIM
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }
                pSimTerminalResponesReq->NbByte = RESPONSEBASE + RESPONSELOCAL;
                SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse1, RESPONSEBASE);
                pSimTerminalResponesReq->TerminalRespData[ComType]   = pSimSatResponseData->nSAT_CmdType;
                pSimTerminalResponesReq->TerminalRespData[ComQual]   = pBase->nComQualifier;
                pSimTerminalResponesReq->TerminalRespData[ComResult] = pSimSatResponseData->nSAT_Status;

                SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseLocalInfo,
                             RESPONSECHANNEL);
                //*(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + RESPONSECHANNEL) =
                //  (pChannelStatus->nChannelID) | (pChannelStatus->nStatus << 7);
                // *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + RESPONSECHANNEL + 1) =
                //   pChannelStatus->nType;
#ifdef CFW_MULTI_SIM
                if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
                {
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }
#else
                if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
                {
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }
#endif
              }
              else
              {
                CSW_TRACE(CFW_SIM_TS_ID, "TerminalRespone Sw1 %x\n", pSimTerminalResponeCnf->Sw1);
                UINT32 nErrorCode = 0;

#ifdef CFW_MULTI_SIM
                nErrorCode = Sim_ParseSW1SW2(pSimTerminalResponeCnf->Sw1, pSimTerminalResponeCnf->Sw2, nSimID);
                CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, nErrorCode, pSimTerminalResponeCnf->Sw1,
                                    nUTI | (nSimID << 24), 0xF0);
#else
                nErrorCode = Sim_ParseSW1SW2(pSimTerminalResponeCnf->Sw1, pSimTerminalResponeCnf->Sw2);
                CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, nErrorCode, pSimTerminalResponeCnf->Sw1, nUTI, 0xF0);
#endif
                CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                return ERR_SUCCESS;
              }
            }

            else
            {
              CSW_TRACE(CFW_SIM_TS_ID, "TerminalRespone Sw1 %x\n", pSimTerminalResponeCnf->Sw1);
              UINT32 nErrorCode = 0;

#ifdef CFW_MULTI_SIM
              nErrorCode = Sim_ParseSW1SW2(pSimTerminalResponeCnf->Sw1, pSimTerminalResponeCnf->Sw2, nSimID);
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, nErrorCode, pSimTerminalResponeCnf->Sw1,
                                  nUTI | (nSimID << 24), 0xF0);
#else
              nErrorCode = Sim_ParseSW1SW2(pSimTerminalResponeCnf->Sw1, pSimTerminalResponeCnf->Sw2);
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, nErrorCode, pSimTerminalResponeCnf->Sw1, nUTI, 0xF0);
#endif
              CFW_SetAoProcCode(hSimSatResponseAo, CFW_AO_PROC_CODE_CONSUMED);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
              return ERR_SUCCESS;
            }
          }
          else
          {
            if (0)  // (nTRTryNum == 0x00)
            {
              nTRTryNum = 0x01;
              CSW_TRACE(CFW_SIM_TS_ID, "nSAT_CmdType %x, Sw1 90 retry\n", pSimSatResponseData->nSAT_CmdType);

              if (pSimSatResponseData->nSAT_CmdType == 0x24)  // Item selection
              {
                api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
                  (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
                CFW_SAT_MENU_RSP* pMenu = (CFW_SAT_MENU_RSP*)(pG_SimSat->pSatComData);

                if ((pSimTerminalResponesReq == NULL) || (pMenu == NULL))
                {
#ifdef CFW_MULTI_SIM
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }
                SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, RESPONSEBASE);
                pSimTerminalResponesReq->TerminalRespData[ComType]   = 0x24;
                pSimTerminalResponesReq->TerminalRespData[ComQual]   = pMenu->nComQualifier;
                pSimTerminalResponesReq->TerminalRespData[ComResult] = pSimSatResponseData->nSAT_Status;

                SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE, ResponseItem, RESPONSEITEM);
                *(pSimTerminalResponesReq->TerminalRespData + RESPONSEBASE + ItemSelection) =
                  pSimSatResponseData->nSAT_ItemID;

                pSimTerminalResponesReq->NbByte = RESPONSEBASE + RESPONSEITEM;

#ifdef CFW_MULTI_SIM
                if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
                {
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }
#else
                if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
                {
                  CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
                  CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
                  return ERR_SUCCESS;
                }
#endif
                return ERR_SUCCESS;

              }
            }
            else
            {
              CSW_TRACE(CFW_SIM_TS_ID, "nSAT_CmdType 24, trynum 0x01\n");

            }
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, pSimSatResponseData->nSAT_CmdType, pSimTerminalResponeCnf->Sw1, nUTI | (nSimID << 24), 0);

            // CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, 0, pSimTerminalResponeCnf->Sw1, GENERATE_SHELL_UTI()|(nSimID<<24), 0);

#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, pSimSatResponseData->nSAT_CmdType, pSimTerminalResponeCnf->Sw1, nUTI, 0);
#endif
            CFW_SetAoProcCode(hSimSatResponseAo, CFW_AO_PROC_CODE_CONSUMED);
            CSW_TRACE(CFW_SIM_TS_ID, "TerminalRespone Sw1 %x\n", pSimTerminalResponeCnf->Sw1);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);

            return ERR_SUCCESS;

          }

          // CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP,0,pSimTerminalResponeCnf->Sw1,nUTI,0);
          // CSW_TRACE(CFW_SIM_TS_ID, "pSimTerminalResponeCnf->Sw1 = %x-OK\n",pSimTerminalResponeCnf->Sw1);
          // CFW_UnRegisterAO(CFW_SIM_SRV_ID,hSimSatResponseAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
          return ERR_SUCCESS;
        }
        else
        {
          if(TRUE != pSimSatResponseData->nSAT_InnerProcess)
          {
#ifdef CFW_MULTI_SIM
          CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, pSimSatResponseData->nSAT_CmdType, pSimTerminalResponeCnf->Sw1, nUTI | (nSimID << 24), 0);
          CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, pSimSatResponseData->nSAT_CmdType, pSimTerminalResponeCnf->Sw1,
                              GENERATE_SHELL_UTI() | (nSimID << 24), 0);
#else
          CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, pSimSatResponseData->nSAT_CmdType, pSimTerminalResponeCnf->Sw1, nUTI, 0);
#endif
  
          }
          pSimSatResponseData->nSAT_InnerProcess = FALSE;
          pSimSatResponseData->nSm_Resp_prevStatus = pSimSatResponseData->nSm_Resp_currStatus;
          pSimSatResponseData->nSm_Resp_currStatus = SM_RESP_FETCH;
        }
      }
      else
        CFW_SetAoProcCode(hSimSatResponseAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_RESP_TERMINALENVELOPE:
    {
      if (nEvtId == API_SIM_TERMINALENVELOPE_CNF)
      {
        api_SimTerminalEnvelopeCnf_t* pSimTerminalEnvelopeCnf = (api_SimTerminalEnvelopeCnf_t*)nEvParam;

        if (pSimTerminalEnvelopeCnf->Sw1 != 0x91)
        {
          if (pSimTerminalEnvelopeCnf->Sw1 == 0x90)
          {
            CSW_TRACE(CFW_SIM_TS_ID, TSTXT("API_SIM_TERMINALENVELOPE_CNF 0x90()\r\n"));
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, pSimSatResponseData->nSAT_CmdType, pSimTerminalEnvelopeCnf->Sw1, nUTI | (nSimID << 24), 0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, pSimSatResponseData->nSAT_CmdType, pSimTerminalEnvelopeCnf->Sw1, nUTI, 0);
#endif

          }
          else
          {
            if (nTETryNum == 0x00)
            {
              nTETryNum = 0x01;
            }
            else
            {
              UINT32 nErrorCode = 0x00;

#ifdef CFW_MULTI_SIM
              nErrorCode = Sim_ParseSW1SW2(pSimTerminalEnvelopeCnf->Sw1, pSimTerminalEnvelopeCnf->Sw2, nSimID);
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, nErrorCode, pSimTerminalEnvelopeCnf->Sw1,
                                  nUTI | (nSimID << 24), 0xF0);
#else
              nErrorCode = Sim_ParseSW1SW2(pSimTerminalEnvelopeCnf->Sw1, pSimTerminalEnvelopeCnf->Sw2);
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, nErrorCode, pSimTerminalEnvelopeCnf->Sw1, nUTI, 0xF0);
#endif
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
              return ERR_SUCCESS;
            }
            CSW_TRACE(CFW_SIM_TS_ID, TSTXT("TerminalEnvelope Sw1 0x%x\r\n"), pSimTerminalEnvelopeCnf->Sw1);

            api_SimTerminalEnvelopeReq_t* pSimTerminalEnvelopeReq =
              (api_SimTerminalEnvelopeReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalEnvelopeReq_t));
            if (pSimTerminalEnvelopeReq == NULL)
            {
              CSW_TRACE(CFW_SIM_TS_ID, "0xD3 command ERR_NO_MORE_MEMORY \n");
#ifdef CFW_MULTI_SIM
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
              return ERR_SUCCESS;
            }
            pSimTerminalEnvelopeReq->NbByte = 0x09;

            SUL_MemCopy8(pSimTerminalEnvelopeReq->EnvelopeData, MenuSelection, pSimTerminalEnvelopeReq->NbByte);
            pSimTerminalEnvelopeReq->EnvelopeData[ItemNumber] = pSimSatResponseData->nSAT_ItemID; // Select the second item
            CSW_TRACE(CFW_SIM_TS_ID, "0xD3 command API_SIM_TERMINALENVELOPE_REQ \n");
#ifdef CFW_MULTI_SIM
            if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALENVELOPE_REQ, pSimTerminalEnvelopeReq, nSimID))
            {
              CSW_TRACE(CFW_SIM_TS_ID, "0xD3 command ERR_NO_MORE_MEMORY \n");
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
              return ERR_SUCCESS;
            }
#else
            if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALENVELOPE_REQ, pSimTerminalEnvelopeReq))
            {
              CSW_TRACE(CFW_SIM_TS_ID, "0xD3 command ERR_NO_MORE_MEMORY \n");
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
              return ERR_SUCCESS;
            }
#endif

            CSW_TRACE(CFW_SIM_TS_ID, "0xD3 command OK!!! \n");
            pSimSatResponseData->nSm_Resp_prevStatus = pSimSatResponseData->nSm_Resp_currStatus;
            pSimSatResponseData->nSm_Resp_currStatus = SM_RESP_TERMINALENVELOPE;
            CFW_SetAoProcCode(hSimSatResponseAo, CFW_AO_PROC_CODE_CONSUMED);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
        }
        else
        {
          CSW_TRACE(CFW_SIM_TS_ID, TSTXT("API_SIM_TERMINALENVELOPE_CNF 0x91()\r\n"));
#ifdef CFW_MULTI_SIM
          CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, pSimSatResponseData->nSAT_CmdType, pSimTerminalEnvelopeCnf->Sw1, nUTI | (nSimID << 24), 0);
#else
          CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, pSimSatResponseData->nSAT_CmdType, pSimTerminalEnvelopeCnf->Sw1, nUTI, 0);
#endif
          pSimSatResponseData->nSm_Resp_prevStatus = pSimSatResponseData->nSm_Resp_currStatus;
          pSimSatResponseData->nSm_Resp_currStatus = SM_RESP_FETCH;
          CFW_SetAoProcCode(hSimSatResponseAo, CFW_AO_PROC_CODE_CONSUMED);
        }
      }
      else
        CFW_SetAoProcCode(hSimSatResponseAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_RESP_FETCH:
    {
      if (nEvtId == API_SIM_FETCH_IND)
      {
        UINT8 nIndex        = 0;
        UINT8 nLength       = 0;
        UINT8 nComQualifier = 0;

        api_SimFetchInd_t* pSimFetchInd = (api_SimFetchInd_t*)nEvParam;

        // 
        UINT8* pOffset = pSimFetchInd->Data;

        if (pOffset[1] == 0x81)
          nIndex = 2;
        else
          nIndex = 1;
        nLength  = pOffset[nIndex] + nIndex; // Update the length

        pSimSatResponseData->nSAT_CmdType = pOffset[nIndex + 4];
        nComQualifier                     = pOffset[nIndex + 5];
        pG_SimSat->nCurCmd                = pOffset[nIndex + 4];
        pG_SimSat->nCmdNum                = pOffset[nIndex + 3];

#ifdef CFW_MULTI_SIM
        nCommondNum[pG_SimSat->nCurCmd - 1][nSimID] = pG_SimSat->nCmdNum;
#else
        nCommondNum[pG_SimSat->nCurCmd - 1] = pG_SimSat->nCmdNum;
#endif

        CSW_TRACE(CFW_SIM_TS_ID, "Got API_SIM_FETCH_IND from Sim[0x%x]\n", pSimSatResponseData->nSAT_CmdType);
        CSW_TRACE(CFW_SIM_TS_ID, "nCmdNum : %x\n", pG_SimSat->nCmdNum);

        if (pOffset[1] == 0x81)
          CSW_TC_MEMBLOCK(CFW_SIM_TS_ID,pOffset,pOffset[2]+2,16);
        else
          CSW_TC_MEMBLOCK(CFW_SIM_TS_ID,pOffset,pOffset[1]+2,16);
        if ( SIM_SAT_SETUP_MENU_IND == pSimSatResponseData->nSAT_CmdType )
        {
          cfw_SatProCmdSetupMenu(hSimSatResponseAo, pOffset + nIndex + 5, nLength - nIndex - 5);
        }
        else if ( SIM_SAT_SELECT_ITEM_IND == pSimSatResponseData->nSAT_CmdType )
        {
          CFW_SAT_MENU_RSP* pMenuItemList = NULL;

          if (SIM_SAT_PROACTIVE_SIM_COMMAND != pOffset[0] )
          {
            CSW_TRACE(CFW_SIM_TS_ID, "ERR_PARAM_ILLEGAL error SAT: %x\n", pEvent->nParam1);
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          else
          {
            UINT8 nAlphaLen = 0;
            UINT32 nItemNum = 0;
            UINT8 i         = 0;
            UINT16 nMemsize = 0;  // memory size

            CFW_SAT_ITEM_LIST* pItemList = NULL;

            nIndex = nIndex + 10;
            if ((pOffset[nIndex] == 0x05) || (pOffset[nIndex] == 0x85))
            {
              if (pOffset[++nIndex] == 0x81)
                nIndex++;
              nAlphaLen = pOffset[nIndex++];
              i         = nIndex + nAlphaLen;
              nMemsize  = SIZEOF(CFW_SAT_MENU_RSP) + nAlphaLen + SIZEOF(CFW_SAT_ITEM_LIST);
            }
            else
            {
              nAlphaLen = 0;
              i         = nIndex;
              nMemsize  = SIZEOF(CFW_SAT_MENU_RSP) + nAlphaLen;
            }

            while (i < nLength)
            {
              if ((pOffset[i] != 0x0f) && (pOffset[i] != 0x8f))
                break;
              nItemNum++;
              nMemsize += SIZEOF(CFW_SAT_ITEM_LIST);
              if (pOffset[++i] == 0x81)
                i++;
              nMemsize += pOffset[i] - 1;
              i = i + pOffset[i];
              i++;
            }
            nMemsize += SIZEOF(CFW_SAT_ITEM_LIST);

            pMenuItemList = (CFW_SAT_MENU_RSP*)CSW_SIM_MALLOC(MEM_ALIGN_SIZE(nMemsize));
            if (pMenuItemList == NULL)
            {
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("pMenuItemList == NULL\r\n"));
#ifdef CFW_MULTI_SIM
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
              return ERR_SUCCESS;
            }
            SUL_ZeroMemory8(pMenuItemList, MEM_ALIGN_SIZE(nMemsize));
            pMenuItemList->nComID        = pG_SimSat->nCurCmd;
            pMenuItemList->nComQualifier = nComQualifier;
            pMenuItemList->nAlphaLen     = nAlphaLen;
            nItemNum                     = SIZEOF(CFW_SAT_ITEM_LIST) * nItemNum;

            if (nAlphaLen != 0)
            {
              pMenuItemList->pAlphaStr =
                SIZEOF(CFW_SAT_MENU_RSP) + (UINT8*)pMenuItemList + nItemNum + SIZEOF(CFW_SAT_ITEM_LIST);
              SUL_MemCopy8(pMenuItemList->pAlphaStr, (pOffset + nIndex), nAlphaLen);
              nItemNum = (UINT32)(pMenuItemList->pAlphaStr) + (UINT32)nAlphaLen;
            }
            else
            {
              pMenuItemList->pAlphaStr = NULL;
              nItemNum                 = SIZEOF(CFW_SAT_MENU_RSP) + (UINT32)pMenuItemList + nItemNum + SIZEOF(CFW_SAT_ITEM_LIST);
            }
            pMenuItemList->pItemList = (CFW_SAT_ITEM_LIST*)(SIZEOF(CFW_SAT_ITEM_LIST) + (UINT8*)pMenuItemList);
            pItemList                = pMenuItemList->pItemList; // pointer point to the byte after alpha string

            i = nIndex + nAlphaLen;

            for (;;)
            {
              if ((pOffset[i] != 0x0f) && (pOffset[i] != 0x8f))
              {
                CSW_TRACE(CFW_SIM_TS_ID, "pOffset[i] issue %x\n", pOffset[i]);
                pItemList->pNextItem = NULL;

                // pItemList  = NULL;
                pItemList->nItemLen  = 0x00;
                pItemList->pItemStr  = NULL;
                pItemList->pNextItem = NULL;
                break;
              }

              if (pOffset[++i] == 0x81)
                i++;
              pItemList->nItemLen = pOffset[i] - 1; // reduce the item's id
              CSW_TRACE(CFW_SIM_TS_ID, "pItemList->nItemLen %x\n", pItemList->nItemLen);

              pItemList->nItemID  = pOffset[++i];
              pItemList->pItemStr = (UINT8*)nItemNum;
              nItemNum += pItemList->nItemLen;

              SUL_MemCopy8(pItemList->pItemStr, (pOffset + i + 1), pItemList->nItemLen);
              pItemList->pNextItem = (CFW_SAT_ITEM_LIST*)(SIZEOF(CFW_SAT_ITEM_LIST) + (UINT8*)pItemList);

              i = i + pItemList->nItemLen;
              if (++i < nLength)
                pItemList = pItemList->pNextItem;
              else
              {
                pItemList->pNextItem = NULL;
                break;
              }
            }
            if (pG_SimSat->pSatComData != NULL)
            {
              CSW_SIM_FREE(pG_SimSat->pSatComData);

              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("pG_SimSat->pSatComData != NULL\r\n"));

            }

            pG_SimSat->pSatComData = pMenuItemList;
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI | (nSimID << 24), 0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI, 0);
#endif
            CSW_TRACE(CFW_SIM_TS_ID, "EV_CFW_SAT_CMDTYPE_IND = 0x24\n");
          }

        }
        else if ( SIM_SAT_SEND_SMS_IND == pSimSatResponseData->nSAT_CmdType )
        {
          UINT8 nAlphaLen  = 0;
          UINT8 nAddrLen   = 0;
          UINT8 nAddrType  = 0;
          UINT8 nTPDULen   = 0;
          UINT8* nAlphaStr = NULL;
          UINT8* nAddrStr  = NULL;
          UINT8* nTPDUStr  = NULL;

          CFW_SAT_SMS_RSP* pSmsResp = NULL;
          UINT16 nMemsize = SIZEOF(CFW_SAT_SMS_RSP);

          CSW_TRACE(CFW_SIM_TS_ID, "SMS CFW_SAT_SMS_RSP size %d\n", nMemsize);

          nIndex = nIndex + 10;
          if ((pOffset[nIndex] == 0x05) || (pOffset[nIndex] == 0x85))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength   = pOffset[nIndex++];
            nAlphaLen = nLength;
            nAlphaStr = pOffset + nIndex;
          }
          else
            nLength = 0;
          nIndex    = nIndex + nLength;
          nMemsize += nLength;

          if ((pOffset[nIndex] == 0x06) || (pOffset[nIndex] == 0x86))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            if(pOffset[nIndex] > 0)
              nLength   = pOffset[nIndex++] - 1;
            nAddrLen  = nLength;
            nAddrType = pOffset[nIndex++];

            if((0x81 == nAddrType) && (0x00 == pOffset[ nIndex ]))
            {
              nIndex++;
              if( nLength > 0 ) nLength--;
              if( nAddrLen > 0 ) nAddrLen-- ;
            }
            nAddrStr  = pOffset + nIndex;

          }
          else
            nLength = 0;
          nIndex    = nIndex + nLength;
          nMemsize += nLength;
          if ((pOffset[nIndex] == 0x0B) || (pOffset[nIndex] == 0x8B))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength  = pOffset[nIndex++];
            nTPDULen = nLength;
            nTPDUStr = (UINT8*)pOffset + nIndex;
          }
          else
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          nMemsize += nLength;

          pSmsResp = (CFW_SAT_SMS_RSP*)CSW_SIM_MALLOC(nMemsize);
          CSW_TRACE(CFW_SIM_TS_ID, "SMS PDUString Mem size=%d\n", nMemsize);

          if (pSmsResp == NULL)
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(ERR_PARAM_ILLEGAL, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(ERR_PARAM_ILLEGAL, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_ZeroMemory8(pSmsResp, nMemsize);
          pSmsResp->nComID        = pG_SimSat->nCurCmd;
          pSmsResp->nComQualifier = nComQualifier;

          pSmsResp->nAlphaLen = nAlphaLen;
          pSmsResp->pAlphaStr = (UINT8*)pSmsResp + SIZEOF(CFW_SAT_SMS_RSP);
          SUL_MemCopy8(pSmsResp->pAlphaStr, nAlphaStr, nAlphaLen);

          pSmsResp->nAddrLen  = nAddrLen;
          pSmsResp->nAddrType = nAddrType;

          pSmsResp->pAddrStr = pSmsResp->pAlphaStr + nAlphaLen;
          if (nAddrLen != 0)
            SUL_MemCopy8(pSmsResp->pAddrStr, nAddrStr, nAddrLen);

          pSmsResp->nPDULen = nTPDULen;
          pSmsResp->pPDUStr = pSmsResp->pAddrStr + nAddrLen;
          CSW_TRACE(CFW_SIM_TS_ID, "-->EV_CFW_SAT_CMDTYPE_IND SMS addr Length=%d,alphabet length = %d\n", nAddrLen,
                        nAlphaLen);
          CSW_TRACE(CFW_SIM_TS_ID, "-->EV_CFW_SAT_CMDTYPE_IND SMS PDUString Length=%d\n", nTPDULen);
          SUL_MemCopy8(pSmsResp->pPDUStr, nTPDUStr, nTPDULen);

          // hameina [+] 2008.1SAT
          // check the DCS and tpdu, if dcs is 7 bits and all the tpdu bit7 is 0, it means the DCS is not equal to the data,
          // we need to transfer the 8 bit data to 7 bit.

          UINT8 uDCSOffsize;
          UINT8 uVPLen;
          UINT8 uVPF;
          UINT8 UdataOff   = 0;
          UINT8 buff7[140] = { 0, };
          UINT8 buff7len   = 0;

          // MTI+MR+DALEN/2+DATYPE+PID + DCS
          uDCSOffsize = 2 + (pSmsResp->pPDUStr[2] + 1) / 2 + 1 + 2;

          // bit3,4 of MTI is vpf
          uVPF = (pSmsResp->pPDUStr[0] >> 3) & 3;
          if (!uVPF)  // NOT present
            uVPLen = 0;
          else if (uVPF == 2) // relative format
            uVPLen = 1;
          else
            uVPLen = 7;

          CSW_TRACE(CFW_SIM_TS_ID, "uDCSOffsize=%d,pPDUPacketTmp[uDCSOffsize]=0x%02x, uVPLen=%d\n",
                        uDCSOffsize, pSmsResp->pPDUStr[uDCSOffsize], uVPLen);

          // dcs offsize + udl+1-->userdata
          // cause VPF is 0, it means vp field is not present, so when test, we don't count this, othersize VP will take 1~7 octs.
          UdataOff = uDCSOffsize + 2 + uVPLen;

          // check if match the condition 1, DCS is 0
          if (!pSmsResp->pPDUStr[uDCSOffsize])
          {
            UINT8 uLoop = 0;

            // here need to check whether all the bit7 of userdata is 0,
            for (uLoop = 0; uLoop < pSmsResp->pPDUStr[UdataOff - 1]; uLoop++)
            {
              if ((pSmsResp->pPDUStr[UdataOff + uLoop] >> 7) & 1)
                break;
            }
            CSW_TRACE(CFW_SIM_TS_ID, "uLoop=%d, pSmsResp->pPDUStr[UdataOff-1]=%d\n", uLoop,
                          pSmsResp->pPDUStr[UdataOff - 1]);

            // check if match the condition 2: all the bit 7 of user data is 0.
            if (uLoop && (uLoop == pSmsResp->pPDUStr[UdataOff - 1]))
            {
              buff7len = SUL_Encode7Bit(&pSmsResp->pPDUStr[UdataOff], buff7, pSmsResp->pPDUStr[UdataOff - 1]);
              SUL_MemCopy8(&pSmsResp->pPDUStr[UdataOff], buff7, buff7len);
              CSW_TRACE(CFW_SIM_TS_ID, "UdataOff=%d, buff7len= %d\n", UdataOff, buff7len);
            }
          }

          // 8 bit data, change to 7 bit data
          //else if (4 == (pSmsResp->pPDUStr[uDCSOffsize] & 0xf)) // padding for UMCC mobile mailbox
          else if ((4 == (pSmsResp->pPDUStr[uDCSOffsize] & 0x0f))||(0xf2 == (pSmsResp->pPDUStr[uDCSOffsize]))) // padding for UMCC mobile mailbox
          {
            //if ((2 == pSmsResp->pPDUStr[UdataOff - 1] && pSmsResp->pPDUStr[UdataOff] == 0x4d) ||(0xf2 == (pSmsResp->pPDUStr[uDCSOffsize])))  // && pSmsResp->pPDUStr[UdataOff+1]== 0x50
            if ((2 == pSmsResp->pPDUStr[UdataOff - 1] && pSmsResp->pPDUStr[UdataOff] == 0x4d) )  // && pSmsResp->pPDUStr[UdataOff+1]== 0x50
            {
              // change DCS to 0, 7  bit decode
              pSmsResp->pPDUStr[uDCSOffsize] = 0;

              // encode user data to 7 bit data
              buff7len = SUL_Encode7Bit(&pSmsResp->pPDUStr[UdataOff], buff7, pSmsResp->pPDUStr[UdataOff - 1]);
              SUL_MemCopy8(&pSmsResp->pPDUStr[UdataOff], buff7, buff7len);
              CSW_TRACE(CFW_SIM_TS_ID, "UdataOff=%d, buff7len= %d\n", UdataOff, buff7len);
            }
          }

          // CSW_TC_MEMBLOCK(CFW_SIM_TS_ID, (UINT8 *) pSmsResp->pPDUStr, (UINT16)nTPDULen, 16);

          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);

          pG_SimSat->pSatComData = pSmsResp;

          // CFW_GetUTI(hSimSatResponseAo, &nUTI);
#ifdef CFW_MULTI_SIM
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI | (nSimID << 24), 0x0);
#else
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI, 0x0);
#endif
        }
        else if ( SIM_SAT_DISPLAY_TEXT_IND == pSimSatResponseData->nSAT_CmdType )
        {
          UINT8 nCodeSch  = 0;
          UINT8* pTextStr = NULL;

          CFW_SAT_DTEXT_RSP* pDTextResp = NULL;

          nIndex = nIndex + 10;
          if ((pOffset[nIndex] == 0x0D) || (pOffset[nIndex] == 0x8D))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength  = pOffset[nIndex++] - 1;
            nCodeSch = pOffset[nIndex++];
            pTextStr = pOffset + nIndex;
          }
          else
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          pDTextResp = (CFW_SAT_DTEXT_RSP*)(CSW_SIM_MALLOC(SIZEOF(CFW_SAT_DTEXT_RSP) + nLength));
          if (pDTextResp == NULL)
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_ZeroMemory8(pDTextResp, (SIZEOF(CFW_SAT_DTEXT_RSP) + nLength));
          pDTextResp->nComID        = pG_SimSat->nCurCmd;
          pDTextResp->nComQualifier = nComQualifier;
          pDTextResp->nTextSch      = nCodeSch;
          pDTextResp->nTextLen      = nLength;
          pDTextResp->pTextStr      = (SIZEOF(CFW_SAT_DTEXT_RSP) + (UINT8*)pDTextResp);
          SUL_MemCopy8(pDTextResp->pTextStr, pTextStr, nLength);

          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);
          pG_SimSat->pSatComData = pDTextResp;
#ifdef CFW_MULTI_SIM
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI | (nSimID << 24), 0x0);
#else
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI, 0x0);
#endif
        }
        else if ( SIM_SAT_GET_INPUT_IND == pSimSatResponseData->nSAT_CmdType ) // GetInput
        {
          UINT8 nTextLen  = 0;
          UINT8 nTextSch  = 0;
          UINT8* nTextStr = NULL;

          UINT8 nMaxLen = 0;
          UINT8 nMinLen = 0;

          UINT8 nDefTextLen  = 0;
          UINT8 nDefTextSch  = 0;
          UINT8* nDefTextStr = NULL;

          CFW_SAT_INPUT_RSP* pInputResp = NULL;

          nIndex = nIndex + 10;
          if ((pOffset[nIndex] == 0x0D) || (pOffset[nIndex] == 0x8D))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength  = pOffset[nIndex++] - 1;
            nTextLen = nLength;
            nTextSch = pOffset[nIndex++];
            nTextStr = pOffset + nIndex;
          }
          else
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          nIndex = nIndex + nLength;
          if ((pOffset[nIndex] == 0x11) || (pOffset[nIndex] == 0x91))
          {
            nMinLen = pOffset[nIndex + 2];
            nMaxLen = pOffset[nIndex + 3];
          }
          else
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          nIndex = nIndex + 4;
          if ((pOffset[nIndex] == 0x17) || (pOffset[nIndex] == 0x97))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nDefTextLen = pOffset[nIndex++] - 1;
            nDefTextSch = pOffset[nIndex++];
            nDefTextStr = pOffset + nIndex;
          }
          pInputResp = (CFW_SAT_INPUT_RSP*)(CSW_SIM_MALLOC(SIZEOF(CFW_SAT_INPUT_RSP) + nTextLen + nDefTextLen));
          if (pInputResp == NULL)
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(ERR_NO_MORE_MEMORY, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(ERR_NO_MORE_MEMORY, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_ZeroMemory8(pInputResp, (SIZEOF(CFW_SAT_INPUT_RSP) + nTextLen + nDefTextLen));
          pInputResp->nComID        = pG_SimSat->nCurCmd;
          pInputResp->nComQualifier = nComQualifier;
          pInputResp->nResMinLen    = nMinLen;
          pInputResp->nResMaxLen    = nMaxLen;
          pInputResp->nTextSch      = nTextSch;
          pInputResp->nTextLen      = nLength;
          pInputResp->pTextStr      = (SIZEOF(CFW_SAT_INPUT_RSP) + (UINT8*)pInputResp);
          SUL_MemCopy8(pInputResp->pTextStr, nTextStr, nTextLen);
          if (nDefTextLen != 0)
          {
            pInputResp->nDefTextLen = nDefTextLen;
            pInputResp->nDefTextSch = nDefTextSch;
            pInputResp->pDefTextStr = pInputResp->pTextStr + nTextLen;
            SUL_MemCopy8(pInputResp->pDefTextStr, nDefTextStr, nDefTextLen);
          }
          else
            pInputResp->pDefTextStr = NULL;

          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);
          pG_SimSat->pSatComData = pInputResp;
#ifdef CFW_MULTI_SIM
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI | (nSimID << 24), 0x0);
#else
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI, 0x0);
#endif
        }
        else if ( SIM_SAT_CALL_SETUP_IND == pSimSatResponseData->nSAT_CmdType ) // set up call
        {
          UINT8 nAlphaConfirmLen  = 0;
          UINT8* pAlphaConfirmStr = NULL;

          UINT8 nAlphaSetupLen  = 0;
          UINT8* pAlphaSetupStr = NULL;

          UINT8 nAddrLen  = 0;
          UINT8 nAddrType = 0;
          UINT8* pAddrStr = NULL;

          UINT8 nSubAddrLen  = 0;
          UINT8* pSubAddrStr = NULL;

          UINT8 nCapabilityCfgLen  = 0;
          UINT8* pCapabilityCfgStr = NULL;

          UINT8 nTuint     = 0;
          UINT8 nTinterval = 0;

          CFW_SAT_CALL_RSP* pCallResp = NULL;
          UINT16 nMemsize = SIZEOF(CFW_SAT_CALL_RSP);

          nIndex = nIndex + 10;
          if ((pOffset[nIndex] == 0x05) || (pOffset[nIndex] == 0x85))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength          = pOffset[nIndex++];
            nAlphaConfirmLen = nLength;
            pAlphaConfirmStr = pOffset + nIndex;
          }
          else
            nLength = 0;

          nIndex = nIndex + nLength;
          nMemsize += nLength;

          if ((pOffset[nIndex] == 0x06) || (pOffset[nIndex] == 0x86))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength   = pOffset[nIndex++] - 1;
            nAddrLen  = nLength;
            nAddrType = pOffset[nIndex++];
            pAddrStr  = pOffset + nIndex;
          }
          else
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          nIndex = nIndex + nLength;
          nMemsize += nLength;
          if ((pOffset[nIndex] == 0x07) || (pOffset[nIndex] == 0x87))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength           = pOffset[nIndex++];
            nCapabilityCfgLen = nLength;
            pCapabilityCfgStr = (UINT8*)pOffset + nIndex;
          }
          else
            nLength = 0;

          nIndex = nIndex + nLength;
          nMemsize += nLength;
          if ((pOffset[nIndex] == 0x08) || (pOffset[nIndex] == 0x88)) // SubAddress
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength     = pOffset[nIndex++];
            nSubAddrLen = nLength;
            pSubAddrStr = (UINT8*)pOffset + nIndex;
          }
    	    else
    	      nLength = 0;

          nIndex = nIndex + nLength;
          nMemsize += nLength;
          if ((pOffset[nIndex] == 0x04) || (pOffset[nIndex] == 0x84)) // Duration
          {
            nTuint     = pOffset[nIndex + 2];
            nTinterval = pOffset[nIndex + 3];
            nLength    = 4;
          }
    	    else
    		    nLength = 0;
          
          nIndex = nIndex + nLength;
          nMemsize += nLength;
          if ((pOffset[nIndex] == 0x1E) || (pOffset[nIndex] == 0x9E))
            nIndex = nIndex + 4;

          if ((pOffset[nIndex] == 0x05) || (pOffset[nIndex] == 0x85))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength        = pOffset[nIndex++];
            nAlphaSetupLen = nLength;
            pAlphaSetupStr = pOffset + nIndex;
          }
    	    else
    		  nLength = 0;

          nMemsize += nLength;

          pCallResp = (CFW_SAT_CALL_RSP*)CSW_SIM_MALLOC(nMemsize);
          if (pCallResp == NULL)
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_ZeroMemory8(pCallResp, nMemsize);
          pCallResp->nComID        = pG_SimSat->nCurCmd;
          pCallResp->nComQualifier = nComQualifier;

          pCallResp->nAlphaConfirmLen = nAlphaConfirmLen;
          pCallResp->pAlphaConfirmStr = (UINT8*)pCallResp + SIZEOF(CFW_SAT_CALL_RSP);
          SUL_MemCopy8(pCallResp->pAlphaConfirmStr, pAlphaConfirmStr, nAlphaConfirmLen);

          pCallResp->nAddrLen  = nAddrLen;
          pCallResp->nAddrType = nAddrType;
          pCallResp->pAddrStr  = pCallResp->pAlphaConfirmStr + nAlphaConfirmLen;
          SUL_MemCopy8(pCallResp->pAddrStr, pAddrStr, nAddrLen);

          pCallResp->nCapabilityCfgLen = nCapabilityCfgLen;
          pCallResp->pCapabilityCfgStr = pCallResp->pAddrStr + nAddrLen;
          SUL_MemCopy8(pCallResp->pCapabilityCfgStr, pCapabilityCfgStr, nCapabilityCfgLen);

          pCallResp->nSubAddrLen = nSubAddrLen;
          pCallResp->pSubAddrStr = pCallResp->pCapabilityCfgStr + nCapabilityCfgLen;
          SUL_MemCopy8(pCallResp->pSubAddrStr, pSubAddrStr, nSubAddrLen);

          pCallResp->nTuint     = nTuint;
          pCallResp->nTinterval = nTinterval;

          pCallResp->nAlphaSetupLen = nAlphaSetupLen;
          pCallResp->pAlphaSetupStr = pCallResp->pSubAddrStr + nSubAddrLen;
          SUL_MemCopy8(pCallResp->pAlphaSetupStr, pAlphaSetupStr, nAlphaSetupLen);
          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);
          pG_SimSat->pSatComData = pCallResp;
#ifdef CFW_MULTI_SIM
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI | (nSimID << 24), 0x0);
#else
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI, 0x0);
#endif
        }
      
        else if ( SIM_SAT_SETUP_EVENT_LIST_IND == pSimSatResponseData->nSAT_CmdType ) 
        {
          SAT_BASE_RSP* pBaseResp = (SAT_BASE_RSP*)CSW_SIM_MALLOC(SIZEOF(SAT_BASE_RSP));
          if (pBaseResp == NULL)
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_ZeroMemory8(pBaseResp, (SIZEOF(SAT_BASE_RSP)));
          pBaseResp->nComID        = pG_SimSat->nCurCmd;
          pBaseResp->nComQualifier = nComQualifier;
          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);
          pG_SimSat->pSatComData = pBaseResp;

          UINT8 nRUti = 0x00;

          CFW_GetFreeUTI(0, &nRUti);

          // #ifdef CFW_MULTI_SIM
          // CFW_SatResponse(pSimSatResponseData->nSAT_CmdType, 0x00, 0x00, NULL, 0x00, nRUti,nSimID);
          api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
            (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
          SAT_BASE_RSP* pBase = (SAT_BASE_RSP*)(pG_SimSat->pSatComData);

          if ((pSimTerminalResponesReq == NULL) || (pBase == NULL))
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

          pSimTerminalResponesReq->NbByte = RESPONSEBASE;

          // SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, RESPONSEBASE);
#ifdef CFW_MULTI_SIM
          if (0x00 == nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID])
#else
          if (0x00 == nCommondNum[pSimSatResponseData->nSAT_CmdType - 1])
#endif
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, pSimTerminalResponesReq->NbByte);
          else
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse1, pSimTerminalResponesReq->NbByte);
#ifdef CFW_MULTI_SIM
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID];
#else
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatResponseData->nSAT_CmdType - 1];
#endif

          // pSimTerminalResponesReq->TerminalRespData[ComType -1]   = 0x02;

          pSimTerminalResponesReq->TerminalRespData[ComType]   = 0x05;
          pSimTerminalResponesReq->TerminalRespData[ComQual]   = 0x00;
          pSimTerminalResponesReq->TerminalRespData[ComResult] = 0x00;

          CSW_TRACE(CFW_SIM_TS_ID, "nSAT_CmdType    %x\n", pSimSatResponseData->nSAT_CmdType);
          CSW_TRACE(CFW_SIM_TS_ID, "nComQualifier   %x\n", pBase->nComQualifier);
          CSW_TRACE(CFW_SIM_TS_ID, "nSAT_Status     %x\n", pSimSatResponseData->nSAT_Status);

          // CSW_TC_MEMBLOCK(CFW_SIM_TS_ID, (UINT8 *) pSimTerminalResponesReq, sizeof(api_SimTerminalResponseReq_t), 16);
#ifdef CFW_MULTI_SIM
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_SCL_QUEUE_FULL, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#else
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_SCL_QUEUE_FULL, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#endif
          CFW_SetAoProcCode(hSimSatResponseAo, CFW_AO_PROC_CODE_CONSUMED);
          pSimSatResponseData->nSm_Resp_prevStatus = pSimSatResponseData->nSm_Resp_currStatus;
          pSimSatResponseData->nSm_Resp_currStatus = SM_RESP_TERMINALRESPONSE;
          pSimSatResponseData->nSAT_InnerProcess = TRUE;
          // CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI|(nSimID<<24), 0x0);
          // #else
          // CFW_SatResponse(pSimSatResponseData->nSAT_CmdType, 0x00, 0x00, NULL, 0x00, nRUti);
          // CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI, 0x0);
          // #endif
          return ERR_SUCCESS;
        }
        else if ( SIM_SAT_FRESH_IND == pSimSatResponseData->nSAT_CmdType ) // Refresh
        {
          SAT_BASE_RSP* pBaseResp = (SAT_BASE_RSP*)CSW_SIM_MALLOC(SIZEOF(SAT_BASE_RSP));
          if (pBaseResp == NULL)
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_ZeroMemory8(pBaseResp, (SIZEOF(SAT_BASE_RSP)));
          pBaseResp->nComID        = pG_SimSat->nCurCmd;
          pBaseResp->nComQualifier = nComQualifier;
          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);
          pG_SimSat->pSatComData = pBaseResp;

          UINT8 nRUti = 0x00;

          CFW_GetFreeUTI(0, &nRUti);

          // #ifdef CFW_DUAL_SIM
          // CFW_SatResponse(pSimSatResponseData->nSAT_CmdType, 0x00, 0x00, NULL, 0x00, nRUti,nSimID);
          api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
            (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
          SAT_BASE_RSP* pBase = (SAT_BASE_RSP*)(pG_SimSat->pSatComData);

          if ((pSimTerminalResponesReq == NULL) || (pBase == NULL))
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

          pSimTerminalResponesReq->NbByte = RESPONSEBASE;

          // SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, RESPONSEBASE);
#ifdef CFW_MULTI_SIM 
          if (0x00 == nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID])
#else
          if (0x00 == nCommondNum[pSimSatResponseData->nSAT_CmdType - 1])
#endif
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, pSimTerminalResponesReq->NbByte);
          else
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse1, pSimTerminalResponesReq->NbByte);
#ifdef CFW_MULTI_SIM
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatResponseData->nSAT_CmdType - 1][nSimID];
#else
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatResponseData->nSAT_CmdType - 1];
#endif

          // pSimTerminalResponesReq->TerminalRespData[ComType -1]   = 0x02;

          pSimTerminalResponesReq->TerminalRespData[ComType]   = 0x01;
          pSimTerminalResponesReq->TerminalRespData[ComQual]   = 0x00;
          pSimTerminalResponesReq->TerminalRespData[ComResult] = 0x00;

          CSW_TRACE(CFW_SIM_TS_ID, "nSAT_CmdType    %x\n", pSimSatResponseData->nSAT_CmdType);
          CSW_TRACE(CFW_SIM_TS_ID, "nComQualifier   %x\n", pBase->nComQualifier);
          CSW_TRACE(CFW_SIM_TS_ID, "nSAT_Status     %x\n", pSimSatResponseData->nSAT_Status);

          // CSW_TC_MEMBLOCK(CFW_SIM_TS_ID, (UINT8 *) pSimTerminalResponesReq, sizeof(api_SimTerminalResponseReq_t), 16);
#ifdef CFW_MULTI_SIM
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_SCL_QUEUE_FULL, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#else
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_SCL_QUEUE_FULL, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#endif
          CFW_SetAoProcCode(hSimSatResponseAo, CFW_AO_PROC_CODE_CONSUMED);
          pSimSatResponseData->nSm_Resp_prevStatus = pSimSatResponseData->nSm_Resp_currStatus;
          pSimSatResponseData->nSm_Resp_currStatus = SM_RESP_TERMINALRESPONSE;
          pSimSatResponseData->nSAT_InnerProcess = TRUE;
          // CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI|(nSimID<<24), 0x0);
          // #else
          // CFW_SatResponse(pSimSatResponseData->nSAT_CmdType, 0x00, 0x00, NULL, 0x00, nRUti);
          // CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI, 0x0);
          // #endif
          return ERR_SUCCESS;
        }        
        else if (SIM_SAT_POLL_INTERVAL_IND == pSimSatResponseData->nSAT_CmdType )
        {
          // UINT8 nCodeSch      = 0x00;
          UINT8 nLen          = 0x00;
          UINT8 nTimeUnit     = 0x00;
          UINT8 nTimeInterval = 0x00;

          // UINT8* pTextStr =NULL;
          CFW_SAT_POLL_INTERVAL_RSP* pPIResp = NULL;

          nIndex = nIndex + 10;

          if ((pOffset[nIndex] == 0x04) || (pOffset[nIndex] == 0x84))
          {
            nLen          = pOffset[nIndex++];
            nTimeUnit     = pOffset[nIndex++];
            nTimeInterval = pOffset[nIndex++];
          }
          else
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          pPIResp = (CFW_SAT_POLL_INTERVAL_RSP*)(CSW_SIM_MALLOC(SIZEOF(CFW_SAT_POLL_INTERVAL_RSP)));
          if (pPIResp == NULL)
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_ZeroMemory8(pPIResp, (SIZEOF(CFW_SAT_POLL_INTERVAL_RSP)));
          pPIResp->nComID        = pG_SimSat->nCurCmd;
          pPIResp->nComQualifier = nComQualifier;
          pPIResp->nTimeUnit     = nTimeUnit;
          pPIResp->nTimeInterval = nTimeInterval;
          CSW_TRACE(CFW_SIM_TS_ID, "pG_SimSat->nCurCmd 0x%x,nComQualifier 0x%x,nTimeUnit 0x%x,nTimeInterval 0x%x\n",
                        pG_SimSat->nCurCmd, nComQualifier, nTimeUnit, nTimeInterval);

          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);

          pG_SimSat->pSatComData = pPIResp;
#ifdef CFW_MULTI_SIM
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI | (nSimID << 24),
                              0x00);
#else
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI, 0x0);
#endif

        }
        else if( SIM_SAT_SEND_USSD_IND == pSimSatResponseData->nSAT_CmdType ) // Send USSD
        {
          UINT8   nAlphaLen = 0x00;
          UINT8*  pAlphaStr = NULL;           
          UINT8   nSSLen    = 0x00;
          UINT8   nSSType   = 0x00;
          UINT8*  pSSStr    = NULL; 
          UINT8   nMemsize  = 0x00;
          UINT8   nIconQual = 0x00;
          UINT8   nIconIdentifer = 0x00;
          CFW_SAT_SS_RSP* pSSRsp = NULL;

          nMemsize += SIZEOF(CFW_SAT_SS_RSP);
          nIndex = nIndex + 10;
          if ((pOffset[nIndex] == 0x05) || (pOffset[nIndex] == 0x85))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength   = pOffset[nIndex++];
            nAlphaLen = nLength;
            pAlphaStr = pOffset + nIndex;
          }
          else
            nLength = 0;
          nIndex    = nIndex + nLength;
          nMemsize += nLength;

          if ((pOffset[nIndex] == 0x0a) || (pOffset[nIndex] == 0x8a))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength   = pOffset[nIndex++] - 1;
            nSSLen  = nLength;
            nSSType = pOffset[nIndex++];
            pSSStr  = pOffset + nIndex;
          }
          else
            nLength = 0;
          nIndex    = nIndex + nLength;
          nMemsize += nLength;
          if ((pOffset[nIndex] == 0x1E) || (pOffset[nIndex] == 0x9E))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength  = pOffset[nIndex++];
            nIconQual = pOffset[nIndex++];
            nIconIdentifer = pOffset[nIndex];
          }
          else
          {
            nLength = 0;
#if 0            
            CSW_TRACE(CFW_SIM_TS_ID, "SS Send Mem size=%d\n", nMemsize);
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
#endif            
          }
          nMemsize += nLength;

          pSSRsp = (CFW_SAT_SS_RSP*)CSW_SIM_MALLOC(nMemsize);
          CSW_TRACE(CFW_SIM_TS_ID, "SS Send Mem size=%d\n", nMemsize);

          if (pSSRsp == NULL)
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(ERR_PARAM_ILLEGAL, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(ERR_PARAM_ILLEGAL, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_ZeroMemory8(pSSRsp, nMemsize);
          pSSRsp->nComID        = pG_SimSat->nCurCmd;
          pSSRsp->nComQualifier = nComQualifier;

          pSSRsp->nAlphaLen = nAlphaLen;
          pSSRsp->pAlphaStr = (UINT8*)pSSRsp + SIZEOF(CFW_SAT_SS_RSP);
          SUL_MemCopy8(pSSRsp->pAlphaStr, pAlphaStr, nAlphaLen);

          pSSRsp->nSSLen = nSSLen;
          pSSRsp->pSSStr = pSSRsp->pAlphaStr + nAlphaLen;
          pSSRsp->nSSType = nSSType;
          SUL_MemCopy8(pSSRsp->pSSStr, pSSStr, nSSLen);

          pSSRsp->nIconQual = nIconQual;
          pSSRsp->nIconIdentifer = nIconIdentifer;
          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);

          pG_SimSat->pSatComData = pSSRsp;
#ifdef CFW_MULTI_SIM
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI | (nSimID << 24),
                              0x0);
#else
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI, 0x0);
#endif
          
        }
        else if( SIM_SAT_LAUNCH_BROWSER_IND == pSimSatResponseData->nSAT_CmdType ) // Launch Browser
        {
          CSW_TRACE(CFW_SIM_TS_ID, "Got SIM_SAT_LAUNCH_BROWSER_IND Data Len %d\n",pSimFetchInd->Length);
          UINT8* pCurrentAdd = NULL;
          
          CFW_SAT_BROWSER_RSP* pBrowserRsp = (CFW_SAT_BROWSER_RSP *)CSW_SIM_MALLOC(pSimFetchInd->Length*2);
          SUL_ZeroMemory8(pBrowserRsp,pSimFetchInd->Length*2);
          pCurrentAdd =  (UINT8*)(pBrowserRsp + SIZEOF(CFW_SAT_BROWSER_RSP));
          
          nIndex = nIndex + 10;
          if (((pOffset[nIndex] == 0x30) || (pOffset[nIndex] == 0xb0)) && ( nIndex <= pSimFetchInd->Length ))
          {
            pBrowserRsp->nBrowserIdentity = pOffset[nIndex + 2];
            nIndex    = nIndex + 3;
            CSW_TRACE(CFW_SIM_TS_ID, "Got pBrowserRsp->nBrowserIdentity[%d]\n",pBrowserRsp->nBrowserIdentity);
            
          }
          
          if (((pOffset[nIndex] == 0x31) || (pOffset[nIndex] == 0xb1)) && ( nIndex <= pSimFetchInd->Length ))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            pBrowserRsp->nURLLength = pOffset[nIndex++];
            pBrowserRsp->pURL = pCurrentAdd;
            if(!SUL_StrNCaselessCompare(&pOffset[nIndex],"http://", 7))
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Got pBrowserRsp->nURLLength We neednot add http flag\n");
              SUL_MemCopy8(pBrowserRsp->pURL, &pOffset[nIndex], pBrowserRsp->nURLLength);
              pCurrentAdd = pBrowserRsp->pURL + pBrowserRsp->nURLLength;
            }
            else
            {
              CSW_TRACE(CFW_SIM_TS_ID, "Got pBrowserRsp->nURLLength We add http flag\n");
              SUL_MemCopy8(pBrowserRsp->pURL,"http://",7);
              //pBrowserRsp->pURL;
              SUL_MemCopy8(pBrowserRsp->pURL + 7, &pOffset[nIndex], pBrowserRsp->nURLLength);
              pBrowserRsp->nURLLength += 7;
              pCurrentAdd = pBrowserRsp->pURL + 7 + pBrowserRsp->nURLLength;
            }
            
            CSW_TRACE(CFW_SIM_TS_ID, "Got pBrowserRsp->nURLLength[%d]\n",pBrowserRsp->nURLLength);
            CSW_TC_MEMBLOCK(CFW_SIM_TS_ID,pBrowserRsp->pURL,pBrowserRsp->nURLLength,16);
            nIndex += pBrowserRsp->nURLLength;
          }


          if (((pOffset[nIndex] == 0x35) || (pOffset[nIndex] == 0xB5)) && ( nIndex <= pSimFetchInd->Length ))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            pBrowserRsp->nBearerLength = pOffset[nIndex++];
            CSW_TRACE(CFW_SIM_TS_ID, "Got pBrowserRsp->nBearerLength[%d]\n",pBrowserRsp->nBearerLength);
            pBrowserRsp->pBearer = pCurrentAdd;
            SUL_MemCopy8(pBrowserRsp->pBearer, &pOffset[nIndex], pBrowserRsp->nBearerLength);
            CSW_TC_MEMBLOCK(CFW_SIM_TS_ID,pBrowserRsp->pBearer,pBrowserRsp->nBearerLength,16);
            pCurrentAdd = pBrowserRsp->pBearer + pBrowserRsp->nBearerLength;
            nIndex += pBrowserRsp->nBearerLength;
          }
          
          UINT8 nPFRLen = 0x00;
          UINT8 *pPFR = NULL;
          for(;nIndex < pSimFetchInd->Length;)
          {
            if (((pOffset[nIndex] == 0x33) || (pOffset[nIndex] == 0xB3)) && ( nIndex <= pSimFetchInd->Length ))
            {
              pBrowserRsp->nProvisioningFileReferenceNum ++;

              nPFRLen = 0x00;
              if (pOffset[++nIndex] == 0x81)
                nIndex++;
              nPFRLen = pOffset[nIndex];
              CSW_TRACE(CFW_SIM_TS_ID, "Got ProvisioningFileReference Len[%d]\n",nPFRLen);
              if( 1 == pBrowserRsp->nProvisioningFileReferenceNum )
              {
                pPFR = pBrowserRsp->pProvisioningFileReference =  pCurrentAdd;
              }
              SUL_MemCopy8(pPFR, &pOffset[nIndex],nPFRLen + 1);
              CSW_TC_MEMBLOCK(CFW_SIM_TS_ID,pPFR,nPFRLen,16);

              nIndex += (1 + nPFRLen);
              pPFR += (1 + nPFRLen);
            }
            else
            {
              if(pPFR)
              pCurrentAdd = pPFR + nPFRLen;
              break;
            }
            
          }

          if (((pOffset[nIndex] == 0x0D) || (pOffset[nIndex] == 0x8D)) && ( nIndex <= pSimFetchInd->Length ))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            pBrowserRsp->nTextLength = pOffset[nIndex++];
            pBrowserRsp->nTextType   = pOffset[nIndex++];
            pBrowserRsp->pText = pCurrentAdd;
            SUL_MemCopy8(pBrowserRsp->pText, &pOffset[nIndex], pBrowserRsp->nTextLength);
            CSW_TRACE(CFW_SIM_TS_ID, "Got pBrowserRsp->nTextLength Len[%d] nTextType[%d]\n",pBrowserRsp->nTextLength,pBrowserRsp->nTextType);
            CSW_TC_MEMBLOCK(CFW_SIM_TS_ID,pBrowserRsp->pText,pBrowserRsp->nTextLength,16);
            pCurrentAdd = pBrowserRsp->pText + pBrowserRsp->nTextLength;
            nIndex += pBrowserRsp->nTextLength;
          }

          if (((pOffset[nIndex] == 0x05) || (pOffset[nIndex] == 0x85)) && ( nIndex <= pSimFetchInd->Length ))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            pBrowserRsp->nAlphaLenLength = pOffset[nIndex++];
            pBrowserRsp->pAlpha = pCurrentAdd;
            SUL_MemCopy8(pBrowserRsp->pAlpha, &pOffset[nIndex], pBrowserRsp->nAlphaLenLength);
            nIndex += pBrowserRsp->nAlphaLenLength;
            CSW_TRACE(CFW_SIM_TS_ID, "Got pBrowserRsp->nAlphaLenLength[%d]\n",pBrowserRsp->nAlphaLenLength);
            CSW_TC_MEMBLOCK(CFW_SIM_TS_ID,pBrowserRsp->pAlpha,pBrowserRsp->nAlphaLenLength,16);
          }          

          if (((pOffset[nIndex] == 0x1e) || (pOffset[nIndex] == 0x9e)) && ( nIndex <= pSimFetchInd->Length ))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            pBrowserRsp->nIconQualifier = pOffset[nIndex++];
            pBrowserRsp->nIconIdentifier = pOffset[nIndex++];
            CSW_TRACE(CFW_SIM_TS_ID, "Got pBrowserRsp->nIconQualifier[%d]\n",pBrowserRsp->nIconQualifier);
            CSW_TRACE(CFW_SIM_TS_ID, "Got pBrowserRsp->nIconIdentifier[%d]\n",pBrowserRsp->nIconIdentifier);
            
          }             

          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);

          pG_SimSat->pSatComData = pBrowserRsp;
#ifdef CFW_MULTI_SIM
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI | (nSimID << 24),
                              0x0);
#else
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI, 0x0);
#endif
          
        }        
        else
        {
          SAT_BASE_RSP* pBaseResp = (SAT_BASE_RSP*)CSW_SIM_MALLOC(SIZEOF(SAT_BASE_RSP));
          if (pBaseResp == NULL)
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_ZeroMemory8(pBaseResp, (SIZEOF(SAT_BASE_RSP)));
          pBaseResp->nComID        = pG_SimSat->nCurCmd;
          pBaseResp->nComQualifier = nComQualifier;
          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);
          pG_SimSat->pSatComData = pBaseResp;
#ifdef CFW_MULTI_SIM
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI | (nSimID << 24), 0x0);
#else
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, nUTI, 0x0);
#endif
        }        
        
        CFW_SetAoProcCode(hSimSatResponseAo, CFW_AO_PROC_CODE_CONSUMED);
        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatResponseAo);
      }
    }
    break;
  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
  return ERR_SUCCESS;
}

// static UINT32 nACTNUM = 0x00;
UINT32 CFW_SatActivationProc(HAO hAo, CFW_EV* pEvent)
{
  HAO hSimSatActivateAo = hAo;

  CFW_SAT_RESPONSE* pSimSatActivateData = NULL;
  UINT32 nUTI   = 0;
  UINT32 nEvtId = 0;
  VOID* nEvParam = NULL;

  CSW_PROFILE_FUNCTION_ENTER(CFW_SatActivationProc);

  if ((UINT32)pEvent != 0xFFFFFFFF)
  {
    nEvtId   = pEvent->nEventId;
    nEvParam = (VOID*)pEvent->nParam1;
  }
   SIM_SAT_PARAM* pG_SimSat = NULL;


#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hSimSatActivateAo);
#endif
  pSimSatActivateData = CFW_GetAoUserData(hSimSatActivateAo);
#ifdef CFW_MULTI_SIM 
          CFW_CfgSimGetSatParam(&pG_SimSat, nSimID);
#else
          CFW_CfgSimGetSatParam(&pG_SimSat);
#endif
  CFW_GetUTI(hSimSatActivateAo, &nUTI);
  switch (pSimSatActivateData->nSm_Act_currStatus)
  {
  case SM_ACT_IDLE:
    {
      if ((UINT32)pEvent == 0xFFFFFFFF)
      {
        api_SimToolkitActivateReq_t* pSimToolkitActivate = NULL;
        pSimToolkitActivate = (api_SimToolkitActivateReq_t*)CFW_MemAllocMessageStructure
          (sizeof(api_SimToolkitActivateReq_t));

#ifdef CFW_MULTI_SIM
        if (pSimToolkitActivate == NULL)
        {
          CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SatActivationProc);
          return ERR_SUCCESS;
        }
        if (pSimSatActivateData->nSAT_Mode == 0x00)
        {
          CFW_PROFILE* pMeProfile;

          // activate the fetch function
          api_SimTerminalProfileReq_t* pSimTerminalProfile = NULL;

          pSimToolkitActivate->Activate = TRUE;
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_STKACTIVATE_REQ, pSimToolkitActivate, nSimID))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatActivationProc);
            return ERR_SUCCESS;
          }

          // ProfileDownload function    
          pSimTerminalProfile = (api_SimTerminalProfileReq_t*)CFW_MemAllocMessageStructure
            (sizeof(api_SimTerminalProfileReq_t));
          if (pSimTerminalProfile == NULL)
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatActivationProc);
            return ERR_SUCCESS;
          }

          CFW_SimGetMeProfile(&pMeProfile, nSimID);
          pSimTerminalProfile->NbByte = pMeProfile->nSat_ProfileLen;
          CSW_TRACE(CFW_SIM_TS_ID, "pMeProfile->nSat_ProfileLen = 0x%x\n", pMeProfile->nSat_ProfileLen);

          SUL_MemCopy8(pSimTerminalProfile->ProfileData, pMeProfile->pSat_ProfileData, pMeProfile->nSat_ProfileLen);
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALPROFILE_REQ, pSimTerminalProfile, nSimID))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatActivationProc);
            return ERR_SUCCESS;
          }

          CFW_SetAoProcCode(hSimSatActivateAo, CFW_AO_PROC_CODE_CONSUMED);
          pSimSatActivateData->nSm_Act_prevStatus = pSimSatActivateData->nSm_Act_currStatus;
          pSimSatActivateData->nSm_Act_currStatus = SM_ACT_TERMINALPROFILE;
        }
        else if (pSimSatActivateData->nSAT_Mode == 0x01)
        {
          pSimToolkitActivate->Activate = TRUE;
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_STKACTIVATE_REQ, pSimToolkitActivate, nSimID))
            CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
          else
            CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, 0, 0, nUTI | (nSimID << 24), 0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
        }
        else if (pSimSatActivateData->nSAT_Mode == 0x02)
        {
          pSimToolkitActivate->Activate = FALSE;
          CFW_GetUTI(hSimSatActivateAo, &nUTI);
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_STKACTIVATE_REQ, pSimToolkitActivate, nSimID))
            CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
          else
            CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, 0, 0, nUTI | (nSimID << 24), 0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
        }

#else
        if (pSimToolkitActivate == NULL)
        {
          CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SatActivationProc);
          return ERR_SUCCESS;
        }
        if (pSimSatActivateData->nSAT_Mode == 0x00)
        {
          CFW_PROFILE* pMeProfile;

          // activate the fetch function
          api_SimTerminalProfileReq_t* pSimTerminalProfile = NULL;

          pSimToolkitActivate->Activate = TRUE;
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_STKACTIVATE_REQ, pSimToolkitActivate))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatActivationProc);
            return ERR_SUCCESS;
          }

          // ProfileDownload function    
          pSimTerminalProfile = (api_SimTerminalProfileReq_t*)CFW_MemAllocMessageStructure
            (sizeof(api_SimTerminalProfileReq_t));
          if (pSimTerminalProfile == NULL)
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatActivationProc);
            return ERR_SUCCESS;
          }

          CFW_SimGetMeProfile(&pMeProfile);
          pSimTerminalProfile->NbByte = pMeProfile->nSat_ProfileLen;
          CSW_TRACE(CFW_SIM_TS_ID, "----->pMeProfile->nSat_ProfileLen = 0x%x\n", pMeProfile->nSat_ProfileLen);

          SUL_MemCopy8(pSimTerminalProfile->ProfileData, pMeProfile->pSat_ProfileData, pMeProfile->nSat_ProfileLen);
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALPROFILE_REQ, pSimTerminalProfile))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatActivationProc);
            return ERR_SUCCESS;
          }

          CFW_SetAoProcCode(hSimSatActivateAo, CFW_AO_PROC_CODE_CONSUMED);
          pSimSatActivateData->nSm_Act_prevStatus = pSimSatActivateData->nSm_Act_currStatus;
          pSimSatActivateData->nSm_Act_currStatus = SM_ACT_TERMINALPROFILE;

          // set SAT status to SAT_INIT
        }
        else if (pSimSatActivateData->nSAT_Mode == 0x01)
        {
          // set SAT status to  SAT_IDLE;
          pSimToolkitActivate->Activate = TRUE;
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_STKACTIVATE_REQ, pSimToolkitActivate))
            CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
          else
            CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, 0, 0, nUTI, 0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
        }
        else if (pSimSatActivateData->nSAT_Mode == 0x02)
        {
          // set SAT status to SAT_NULL;
          pSimToolkitActivate->Activate = FALSE;
          CFW_GetUTI(hSimSatActivateAo, &nUTI);
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_STKACTIVATE_REQ, pSimToolkitActivate))
            CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
          else
            CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, 0, 0, nUTI, 0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
        }
#endif
      }
      else
        CFW_SetAoProcCode(hSimSatActivateAo, CFW_AO_PROC_CODE_NOT_CONSUMED);

    }
    break;
  case SM_ACT_TERMINALPROFILE:
    {
      if (pEvent->nEventId == API_SIM_TERMINALPROFILE_CNF)
      {
        api_SimTerminalProfileCnf_t* pSimTerminalProfileCnf = (api_SimTerminalProfileCnf_t*)pEvent->nParam1;

#ifdef CFW_MULTI_SIM
        if (pSimTerminalProfileCnf->Sw1 != 0x91)
        {
          UINT32 nErrorCode = 0;

          if (pSimTerminalProfileCnf->Sw1 != 0x90)
          {
            nErrorCode = Sim_ParseSW1SW2(pSimTerminalProfileCnf->Sw1, pSimTerminalProfileCnf->Sw2, nSimID);
          }
          else
          {
            nErrorCode = ERR_CME_SIM_PROFILE_ERROR;
          }
          CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, nErrorCode, pSimTerminalProfileCnf->Sw1, nUTI | (nSimID << 24),
                              0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
        }
        else
        {
          CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, 0, pSimTerminalProfileCnf->Sw1, nUTI | (nSimID << 24), 0);
          CFW_SetAoProcCode(hSimSatActivateAo, CFW_AO_PROC_CODE_CONSUMED);
          pSimSatActivateData->nSm_Act_prevStatus = pSimSatActivateData->nSm_Act_currStatus;
          pSimSatActivateData->nSm_Act_currStatus = SM_ACT_FETCH;
        }

#else
        if (pSimTerminalProfileCnf->Sw1 != 0x91)
        {
          UINT32 nErrorCode = 0;

          if (pSimTerminalProfileCnf->Sw1 != 0x90)
          {
            nErrorCode = Sim_ParseSW1SW2(pSimTerminalProfileCnf->Sw1, pSimTerminalProfileCnf->Sw2);
          }
          else
          {
            nErrorCode = ERR_CME_SIM_PROFILE_ERROR;
          }
          CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, nErrorCode, pSimTerminalProfileCnf->Sw1,
                              nUTI /* GENERATE_SHELL_UTI() */ , 0xF0);
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
        }
        else
        {
          CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, 0, pSimTerminalProfileCnf->Sw1, nUTI, 0);
          CFW_SetAoProcCode(hSimSatActivateAo, CFW_AO_PROC_CODE_CONSUMED);
          pSimSatActivateData->nSm_Act_prevStatus = pSimSatActivateData->nSm_Act_currStatus;
          pSimSatActivateData->nSm_Act_currStatus = SM_ACT_FETCH;
        }

#endif

        CSW_TRACE(CFW_SIM_TS_ID, "API_SIM_TERMINALPROFILE_CNF status sw1[0x%x]\n", pSimTerminalProfileCnf->Sw1);
      }
      else
        CFW_SetAoProcCode(hSimSatActivateAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;
  case SM_ACT_FETCH:
    {
      if (nEvtId == API_SIM_FETCH_IND)
      {
        UINT8 nIndex        = 0;
        UINT8 nLength       = 0;
        UINT8 nComQualifier = 0;

        api_SimFetchInd_t* pSimFetchInd = (api_SimFetchInd_t*)nEvParam;

        // 
        UINT8* pOffset = pSimFetchInd->Data;

        if (pOffset[1] == 0x81)
          nIndex = 2;
        else
          nIndex = 1;
        nLength  = pOffset[nIndex] + nIndex; // Update the length

        pSimSatActivateData->nSAT_CmdType = pOffset[nIndex + 4];
        nComQualifier                     = pOffset[nIndex + 5];
        pG_SimSat->nCurCmd                = pOffset[nIndex + 4];
        pG_SimSat->nCmdNum                = pOffset[nIndex + 3];

#ifdef CFW_MULTI_SIM
          nCommondNum[pG_SimSat->nCurCmd - 1][nSimID] = pG_SimSat->nCmdNum;
#else
          nCommondNum[pG_SimSat->nCurCmd - 1] = pG_SimSat->nCmdNum;
#endif

        CSW_TRACE(CFW_SIM_TS_ID, "Got API_SIM_FETCH_IND from Sim[0x%x]\n", pSimSatActivateData->nSAT_CmdType);
        CSW_TRACE(CFW_SIM_TS_ID, "nCmdNum : %x\n", pG_SimSat->nCmdNum);

        if (pOffset[1] == 0x81)
          CSW_TC_MEMBLOCK(CFW_SIM_TS_ID,pOffset,pOffset[2]+2,16);
        else
          CSW_TC_MEMBLOCK(CFW_SIM_TS_ID,pOffset,pOffset[1]+2,16);
        if ( SIM_SAT_SETUP_MENU_IND == pSimSatActivateData->nSAT_CmdType )
        {
          cfw_SatProCmdSetupMenu(hSimSatActivateAo, pOffset + nIndex + 5, nLength - nIndex - 5);
        }
        else if ( SIM_SAT_SELECT_ITEM_IND == pSimSatActivateData->nSAT_CmdType )
        {
          CFW_SAT_MENU_RSP* pMenuItemList = NULL;

          if (SIM_SAT_PROACTIVE_SIM_COMMAND != pOffset[0] )
          {
            CSW_TRACE(CFW_SIM_TS_ID, "ERR_PARAM_ILLEGAL error SAT: %x\n", pEvent->nParam1);
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          else
          {
            UINT8 nAlphaLen = 0;
            UINT32 nItemNum = 0;
            UINT8 i         = 0;
            UINT16 nMemsize = 0;  // memory size

            CFW_SAT_ITEM_LIST* pItemList = NULL;

            nIndex = nIndex + 10;
            if ((pOffset[nIndex] == 0x05) || (pOffset[nIndex] == 0x85))
            {
              if (pOffset[++nIndex] == 0x81)
                nIndex++;
              nAlphaLen = pOffset[nIndex++];
              i         = nIndex + nAlphaLen;
              nMemsize  = SIZEOF(CFW_SAT_MENU_RSP) + nAlphaLen + SIZEOF(CFW_SAT_ITEM_LIST);
            }
            else
            {
              nAlphaLen = 0;
              i         = nIndex;
              nMemsize  = SIZEOF(CFW_SAT_MENU_RSP) + nAlphaLen;
            }

            while (i < nLength)
            {
              if ((pOffset[i] != 0x0f) && (pOffset[i] != 0x8f))
                break;
              nItemNum++;
              nMemsize += SIZEOF(CFW_SAT_ITEM_LIST);
              if (pOffset[++i] == 0x81)
                i++;
              nMemsize += pOffset[i] - 1;
              i = i + pOffset[i];
              i++;
            }
            nMemsize += SIZEOF(CFW_SAT_ITEM_LIST);

            pMenuItemList = (CFW_SAT_MENU_RSP*)CSW_SIM_MALLOC(MEM_ALIGN_SIZE(nMemsize));
            if (pMenuItemList == NULL)
            {
              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("pMenuItemList == NULL\r\n"));
#ifdef CFW_MULTI_SIM
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
              CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
              CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
              CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
              return ERR_SUCCESS;
            }
            SUL_ZeroMemory8(pMenuItemList, MEM_ALIGN_SIZE(nMemsize));
            pMenuItemList->nComID        = pG_SimSat->nCurCmd;
            pMenuItemList->nComQualifier = nComQualifier;
            pMenuItemList->nAlphaLen     = nAlphaLen;
            nItemNum                     = SIZEOF(CFW_SAT_ITEM_LIST) * nItemNum;

            if (nAlphaLen != 0)
            {
              pMenuItemList->pAlphaStr =
                SIZEOF(CFW_SAT_MENU_RSP) + (UINT8*)pMenuItemList + nItemNum + SIZEOF(CFW_SAT_ITEM_LIST);
              SUL_MemCopy8(pMenuItemList->pAlphaStr, (pOffset + nIndex), nAlphaLen);
              nItemNum = (UINT32)(pMenuItemList->pAlphaStr) + (UINT32)nAlphaLen;
            }
            else
            {
              pMenuItemList->pAlphaStr = NULL;
              nItemNum                 = SIZEOF(CFW_SAT_MENU_RSP) + (UINT32)pMenuItemList + nItemNum + SIZEOF(CFW_SAT_ITEM_LIST);
            }
            pMenuItemList->pItemList = (CFW_SAT_ITEM_LIST*)(SIZEOF(CFW_SAT_ITEM_LIST) + (UINT8*)pMenuItemList);
            pItemList                = pMenuItemList->pItemList; // pointer point to the byte after alpha string

            i = nIndex + nAlphaLen;

            for (;;)
            {
              if ((pOffset[i] != 0x0f) && (pOffset[i] != 0x8f))
              {
                CSW_TRACE(CFW_SIM_TS_ID, "pOffset[i] issue %x\n", pOffset[i]);
                pItemList->pNextItem = NULL;

                // pItemList  = NULL;
                pItemList->nItemLen  = 0x00;
                pItemList->pItemStr  = NULL;
                pItemList->pNextItem = NULL;
                break;
              }

              if (pOffset[++i] == 0x81)
                i++;
              pItemList->nItemLen = pOffset[i] - 1; // reduce the item's id
              CSW_TRACE(CFW_SIM_TS_ID, "pItemList->nItemLen %x\n", pItemList->nItemLen);

              pItemList->nItemID  = pOffset[++i];
              pItemList->pItemStr = (UINT8*)nItemNum;
              nItemNum += pItemList->nItemLen;

              SUL_MemCopy8(pItemList->pItemStr, (pOffset + i + 1), pItemList->nItemLen);
              pItemList->pNextItem = (CFW_SAT_ITEM_LIST*)(SIZEOF(CFW_SAT_ITEM_LIST) + (UINT8*)pItemList);

              i = i + pItemList->nItemLen;
              if (++i < nLength)
                pItemList = pItemList->pNextItem;
              else
              {
                pItemList->pNextItem = NULL;
                break;
              }
            }
            if (pG_SimSat->pSatComData != NULL)
            {
              CSW_SIM_FREE(pG_SimSat->pSatComData);

              CSW_TRACE(CFW_SIM_TS_ID, TSTXT("pG_SimSat->pSatComData != NULL\r\n"));

            }

            pG_SimSat->pSatComData = pMenuItemList;
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI | (nSimID << 24), 0);
            CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, 1 | (nSimID << 24), 0);

#else
            CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI, 0);
            CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, 1, 0);
#endif
            CSW_TRACE(CFW_SIM_TS_ID, "EV_CFW_SAT_CMDTYPE_IND = 0x24\n");
          }

        }
        else if ( SIM_SAT_SEND_SMS_IND == pSimSatActivateData->nSAT_CmdType )
        {
          UINT8 nAlphaLen  = 0;
          UINT8 nAddrLen   = 0;
          UINT8 nAddrType  = 0;
          UINT8 nTPDULen   = 0;
          UINT8* nAlphaStr = NULL;
          UINT8* nAddrStr  = NULL;
          UINT8* nTPDUStr  = NULL;

          CFW_SAT_SMS_RSP* pSmsResp = NULL;
          UINT16 nMemsize = SIZEOF(CFW_SAT_SMS_RSP);

          CSW_TRACE(CFW_SIM_TS_ID, "SMS CFW_SAT_SMS_RSP size %d\n", nMemsize);

          nIndex = nIndex + 10;
          if ((pOffset[nIndex] == 0x05) || (pOffset[nIndex] == 0x85))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength   = pOffset[nIndex++];
            nAlphaLen = nLength;
            nAlphaStr = pOffset + nIndex;
          }
          else
            nLength = 0;
          nIndex    = nIndex + nLength;
          nMemsize += nLength;

          if ((pOffset[nIndex] == 0x06) || (pOffset[nIndex] == 0x86))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength   = pOffset[nIndex++] - 1;
            nAddrLen  = nLength;
            nAddrType = pOffset[nIndex++];
            if((0x81 == nAddrType) && (0x00 == pOffset[nIndex]))
            {
              nIndex++;
              if( nLength > 0 ) nLength--;
              if( nAddrLen > 0 ) nAddrLen -- ;
            }
            nAddrStr  = pOffset + nIndex;
          }
          else
            nLength = 0;
          nIndex    = nIndex + nLength;
          nMemsize += nLength;
          if ((pOffset[nIndex] == 0x0B) || (pOffset[nIndex] == 0x8B))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength  = pOffset[nIndex++];
            nTPDULen = nLength;
            nTPDUStr = (UINT8*)pOffset + nIndex;
          }
          else
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          nMemsize += nLength;

          pSmsResp = (CFW_SAT_SMS_RSP*)CSW_SIM_MALLOC(nMemsize);
          CSW_TRACE(CFW_SIM_TS_ID, "SMS PDUString Mem size=%d\n", nMemsize);

          if (pSmsResp == NULL)
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(ERR_PARAM_ILLEGAL, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(ERR_PARAM_ILLEGAL, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_ZeroMemory8(pSmsResp, nMemsize);
          pSmsResp->nComID        = pG_SimSat->nCurCmd;
          pSmsResp->nComQualifier = nComQualifier;

          pSmsResp->nAlphaLen = nAlphaLen;
          pSmsResp->pAlphaStr = (UINT8*)pSmsResp + SIZEOF(CFW_SAT_SMS_RSP);
          SUL_MemCopy8(pSmsResp->pAlphaStr, nAlphaStr, nAlphaLen);

          pSmsResp->nAddrLen  = nAddrLen;
          pSmsResp->nAddrType = nAddrType;

          pSmsResp->pAddrStr = pSmsResp->pAlphaStr + nAlphaLen;
          if (nAddrLen != 0)
            SUL_MemCopy8(pSmsResp->pAddrStr, nAddrStr, nAddrLen);

          pSmsResp->nPDULen = nTPDULen;
          pSmsResp->pPDUStr = pSmsResp->pAddrStr + nAddrLen;
          CSW_TRACE(CFW_SIM_TS_ID, "-->EV_CFW_SAT_CMDTYPE_IND SMS addr Length=%d,alphabet length = %d\n", nAddrLen,
                        nAlphaLen);
          CSW_TRACE(CFW_SIM_TS_ID, "-->EV_CFW_SAT_CMDTYPE_IND SMS PDUString Length=%d\n", nTPDULen);
          SUL_MemCopy8(pSmsResp->pPDUStr, nTPDUStr, nTPDULen);

          // hameina [+] 2008.1SAT
          // check the DCS and tpdu, if dcs is 7 bits and all the tpdu bit7 is 0, it means the DCS is not equal to the data,
          // we need to transfer the 8 bit data to 7 bit.

          UINT8 uDCSOffsize;
          UINT8 uVPLen;
          UINT8 uVPF;
          UINT8 UdataOff   = 0;
          UINT8 buff7[140] = { 0, };
          UINT8 buff7len   = 0;

          // MTI+MR+DALEN/2+DATYPE+PID + DCS
          uDCSOffsize = 2 + (pSmsResp->pPDUStr[2] + 1) / 2 + 1 + 2;

          // bit3,4 of MTI is vpf
          uVPF = (pSmsResp->pPDUStr[0] >> 3) & 3;
          if (!uVPF)  // NOT present
            uVPLen = 0;
          else if (uVPF == 2) // relative format
            uVPLen = 1;
          else
            uVPLen = 7;

          CSW_TRACE(CFW_SIM_TS_ID, "uDCSOffsize=%d,pPDUPacketTmp[uDCSOffsize]=0x%02x, uVPLen=%d\n",
                        uDCSOffsize, pSmsResp->pPDUStr[uDCSOffsize], uVPLen);

          // dcs offsize + udl+1-->userdata
          // cause VPF is 0, it means vp field is not present, so when test, we don't count this, othersize VP will take 1~7 octs.
          UdataOff = uDCSOffsize + 2 + uVPLen;

          // check if match the condition 1, DCS is 0
          if (!pSmsResp->pPDUStr[uDCSOffsize])
          {
            UINT8 uLoop = 0;

            // here need to check whether all the bit7 of userdata is 0,
            for (uLoop = 0; uLoop < pSmsResp->pPDUStr[UdataOff - 1]; uLoop++)
            {
              if ((pSmsResp->pPDUStr[UdataOff + uLoop] >> 7) & 1)
                break;
            }
            CSW_TRACE(CFW_SIM_TS_ID, "uLoop=%d, pSmsResp->pPDUStr[UdataOff-1]=%d\n", uLoop,
                          pSmsResp->pPDUStr[UdataOff - 1]);

            // check if match the condition 2: all the bit 7 of user data is 0.
            if (uLoop && (uLoop == pSmsResp->pPDUStr[UdataOff - 1]))
            {
              buff7len = SUL_Encode7Bit(&pSmsResp->pPDUStr[UdataOff], buff7, pSmsResp->pPDUStr[UdataOff - 1]);
              SUL_MemCopy8(&pSmsResp->pPDUStr[UdataOff], buff7, buff7len);
              CSW_TRACE(CFW_SIM_TS_ID, "UdataOff=%d, buff7len= %d\n", UdataOff, buff7len);
            }
          }

          // 8 bit data, change to 7 bit data
          else if (4 == (pSmsResp->pPDUStr[uDCSOffsize] & 0xf)) // padding for UMCC mobile mailbox
          {
            //if (2 == pSmsResp->pPDUStr[UdataOff - 1] && pSmsResp->pPDUStr[UdataOff] == 0x4d)  // && pSmsResp->pPDUStr[UdataOff+1]== 0x50

            {
              // change DCS to 0, 7  bit decode
              pSmsResp->pPDUStr[uDCSOffsize] = 0;

              // encode user data to 7 bit data
              buff7len = SUL_Encode7Bit(&pSmsResp->pPDUStr[UdataOff], buff7, pSmsResp->pPDUStr[UdataOff - 1]);
              SUL_MemCopy8(&pSmsResp->pPDUStr[UdataOff], buff7, buff7len);
              CSW_TRACE(CFW_SIM_TS_ID, "UdataOff=%d, buff7len= %d\n", UdataOff, buff7len);
            }
          }

          // CSW_TC_MEMBLOCK(CFW_SIM_TS_ID, (UINT8 *) pSmsResp->pPDUStr, (UINT16)nTPDULen, 16);

          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);

          pG_SimSat->pSatComData = pSmsResp;

          // CFW_GetUTI(hSimSatActivateAo, &nUTI);
#ifdef CFW_MULTI_SIM
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI | (nSimID << 24), 0x0);
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, 1 | (nSimID << 24), 0x0);
#else
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI, 0x0);
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, 1, 0x0);
#endif
        }
        else if ( SIM_SAT_DISPLAY_TEXT_IND == pSimSatActivateData->nSAT_CmdType )
        {
          UINT8 nCodeSch  = 0;
          UINT8* pTextStr = NULL;

          CFW_SAT_DTEXT_RSP* pDTextResp = NULL;

          nIndex = nIndex + 10;
            if ((pOffset[nIndex] == 0x0D) || (pOffset[nIndex] == 0x8D))
            {
              if (pOffset[++nIndex] == 0x81)
                nIndex++;
              nLength  = pOffset[nIndex++] - 1;
              nCodeSch = pOffset[nIndex++];
              pTextStr = pOffset + nIndex;
            }
            else
            {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          pDTextResp = (CFW_SAT_DTEXT_RSP*)(CSW_SIM_MALLOC(SIZEOF(CFW_SAT_DTEXT_RSP) + nLength));
          if (pDTextResp == NULL)
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_ZeroMemory8(pDTextResp, (SIZEOF(CFW_SAT_DTEXT_RSP) + nLength));
          pDTextResp->nComID        = pG_SimSat->nCurCmd;
          pDTextResp->nComQualifier = nComQualifier;
          pDTextResp->nTextSch      = nCodeSch;
          pDTextResp->nTextLen      = nLength;
          pDTextResp->pTextStr      = (SIZEOF(CFW_SAT_DTEXT_RSP) + (UINT8*)pDTextResp);
          SUL_MemCopy8(pDTextResp->pTextStr, pTextStr, nLength);

          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);
          pG_SimSat->pSatComData = pDTextResp;
#ifdef CFW_MULTI_SIM
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI | (nSimID << 24), 0x0);
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, 1 | (nSimID << 24), 0x0);
#else
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI, 0x0);
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, 1, 0x0);
#endif
        }
        else if ( SIM_SAT_GET_INPUT_IND == pSimSatActivateData->nSAT_CmdType ) // GetInput
        {
          UINT8 nTextLen  = 0;
          UINT8 nTextSch  = 0;
          UINT8* nTextStr = NULL;

          UINT8 nMaxLen = 0;
          UINT8 nMinLen = 0;

          UINT8 nDefTextLen  = 0;
          UINT8 nDefTextSch  = 0;
          UINT8* nDefTextStr = NULL;

          CFW_SAT_INPUT_RSP* pInputResp = NULL;

          nIndex = nIndex + 10;
          if ((pOffset[nIndex] == 0x0D) || (pOffset[nIndex] == 0x8D))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength  = pOffset[nIndex++] - 1;
            nTextLen = nLength;
            nTextSch = pOffset[nIndex++];
            nTextStr = pOffset + nIndex;
          }
          else
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          nIndex = nIndex + nLength;
          if ((pOffset[nIndex] == 0x11) || (pOffset[nIndex] == 0x91))
          {
            nMinLen = pOffset[nIndex + 2];
            nMaxLen = pOffset[nIndex + 3];
          }
          else
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          nIndex = nIndex + 4;
          if ((pOffset[nIndex] == 0x17) || (pOffset[nIndex] == 0x97))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nDefTextLen = pOffset[nIndex++] - 1;
            nDefTextSch = pOffset[nIndex++];
            nDefTextStr = pOffset + nIndex;
          }
          pInputResp = (CFW_SAT_INPUT_RSP*)(CSW_SIM_MALLOC(SIZEOF(CFW_SAT_INPUT_RSP) + nTextLen + nDefTextLen));
          if (pInputResp == NULL)
          {
#ifdef CFW_MULTI_SIM 
            CFW_PostNotifyEvent(ERR_NO_MORE_MEMORY, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(ERR_NO_MORE_MEMORY, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_ZeroMemory8(pInputResp, (SIZEOF(CFW_SAT_INPUT_RSP) + nTextLen + nDefTextLen));
          pInputResp->nComID        = pG_SimSat->nCurCmd;
          pInputResp->nComQualifier = nComQualifier;
          pInputResp->nResMinLen    = nMinLen;
          pInputResp->nResMaxLen    = nMaxLen;
          pInputResp->nTextSch      = nTextSch;
          pInputResp->nTextLen      = nLength;
          pInputResp->pTextStr      = (SIZEOF(CFW_SAT_INPUT_RSP) + (UINT8*)pInputResp);
          SUL_MemCopy8(pInputResp->pTextStr, nTextStr, nTextLen);
          if (nDefTextLen != 0)
          {
            pInputResp->nDefTextLen = nDefTextLen;
            pInputResp->nDefTextSch = nDefTextSch;
            pInputResp->pDefTextStr = pInputResp->pTextStr + nTextLen;
            SUL_MemCopy8(pInputResp->pDefTextStr, nDefTextStr, nDefTextLen);
          }
          else
            pInputResp->pDefTextStr = NULL;

          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);
          pG_SimSat->pSatComData = pInputResp;
#ifdef CFW_MULTI_SIM 
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI | (nSimID << 24), 0x0);
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, 1 | (nSimID << 24), 0x0);
#else
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI, 0x0);
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, 1, 0x0);
#endif
        }
        else if ( SIM_SAT_CALL_SETUP_IND == pSimSatActivateData->nSAT_CmdType ) // set up call
        {
          UINT8 nAlphaConfirmLen  = 0;
          UINT8* pAlphaConfirmStr = NULL;

          UINT8 nAlphaSetupLen  = 0;
          UINT8* pAlphaSetupStr = NULL;

          UINT8 nAddrLen  = 0;
          UINT8 nAddrType = 0;
          UINT8* pAddrStr = NULL;

          UINT8 nSubAddrLen  = 0;
          UINT8* pSubAddrStr = NULL;

          UINT8 nCapabilityCfgLen  = 0;
          UINT8* pCapabilityCfgStr = NULL;

          UINT8 nTuint     = 0;
          UINT8 nTinterval = 0;

          CFW_SAT_CALL_RSP* pCallResp = NULL;
          UINT16 nMemsize = SIZEOF(CFW_SAT_CALL_RSP);

          nIndex = nIndex + 10;
          if ((pOffset[nIndex] == 0x05) || (pOffset[nIndex] == 0x85))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength          = pOffset[nIndex++];
            nAlphaConfirmLen = nLength;
            pAlphaConfirmStr = pOffset + nIndex;
          }
          else
            nLength = 0;

          nIndex = nIndex + nLength;
          nMemsize += nLength;

          if ((pOffset[nIndex] == 0x06) || (pOffset[nIndex] == 0x86))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength   = pOffset[nIndex++] - 1;
            nAddrLen  = nLength;
            nAddrType = pOffset[nIndex++];
            pAddrStr  = pOffset + nIndex;
          }
          else
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          nIndex = nIndex + nLength;
          nMemsize += nLength;
          if ((pOffset[nIndex] == 0x07) || (pOffset[nIndex] == 0x87))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength           = pOffset[nIndex++];
            nCapabilityCfgLen = nLength;
            pCapabilityCfgStr = (UINT8*)pOffset + nIndex;
          }
          else
            nLength = 0;

          nIndex = nIndex + nLength;
          nMemsize += nLength;
          if ((pOffset[nIndex] == 0x08) || (pOffset[nIndex] == 0x88)) // SubAddress
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength     = pOffset[nIndex++];
            nSubAddrLen = nLength;
            pSubAddrStr = (UINT8*)pOffset + nIndex;
          }
    	    else
    	      nLength = 0;

          nIndex = nIndex + nLength;
          nMemsize += nLength;
          if ((pOffset[nIndex] == 0x04) || (pOffset[nIndex] == 0x84)) // Duration
          {
            nTuint     = pOffset[nIndex + 2];
            nTinterval = pOffset[nIndex + 3];
            nLength    = 4;
          }
    	    else
    		    nLength = 0;
          
          nIndex = nIndex + nLength;
          nMemsize += nLength;
          if ((pOffset[nIndex] == 0x1E) || (pOffset[nIndex] == 0x9E))
            nIndex = nIndex + 4;

          if ((pOffset[nIndex] == 0x05) || (pOffset[nIndex] == 0x85))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength        = pOffset[nIndex++];
            nAlphaSetupLen = nLength;
            pAlphaSetupStr = pOffset + nIndex;
          }
    	    else
    		  nLength = 0;

          nMemsize += nLength;

          pCallResp = (CFW_SAT_CALL_RSP*)CSW_SIM_MALLOC(nMemsize);
          if (pCallResp == NULL)
          {
#ifdef CFW_MULTI_SIM 
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_ZeroMemory8(pCallResp, nMemsize);
          pCallResp->nComID        = pG_SimSat->nCurCmd;
          pCallResp->nComQualifier = nComQualifier;

          pCallResp->nAlphaConfirmLen = nAlphaConfirmLen;
          pCallResp->pAlphaConfirmStr = (UINT8*)pCallResp + SIZEOF(CFW_SAT_CALL_RSP);
          SUL_MemCopy8(pCallResp->pAlphaConfirmStr, pAlphaConfirmStr, nAlphaConfirmLen);

          pCallResp->nAddrLen  = nAddrLen;
          pCallResp->nAddrType = nAddrType;
          pCallResp->pAddrStr  = pCallResp->pAlphaConfirmStr + nAlphaConfirmLen;
          SUL_MemCopy8(pCallResp->pAddrStr, pAddrStr, nAddrLen);

          pCallResp->nCapabilityCfgLen = nCapabilityCfgLen;
          pCallResp->pCapabilityCfgStr = pCallResp->pAddrStr + nAddrLen;
          SUL_MemCopy8(pCallResp->pCapabilityCfgStr, pCapabilityCfgStr, nCapabilityCfgLen);

          pCallResp->nSubAddrLen = nSubAddrLen;
          pCallResp->pSubAddrStr = pCallResp->pCapabilityCfgStr + nCapabilityCfgLen;
          SUL_MemCopy8(pCallResp->pSubAddrStr, pSubAddrStr, nSubAddrLen);

          pCallResp->nTuint     = nTuint;
          pCallResp->nTinterval = nTinterval;

          pCallResp->nAlphaSetupLen = nAlphaSetupLen;
          pCallResp->pAlphaSetupStr = pCallResp->pSubAddrStr + nSubAddrLen;
          SUL_MemCopy8(pCallResp->pAlphaSetupStr, pAlphaSetupStr, nAlphaSetupLen);
          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);
          pG_SimSat->pSatComData = pCallResp;
#ifdef CFW_MULTI_SIM 
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI | (nSimID << 24), 0x0);
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, 1 | (nSimID << 24), 0x0);
#else
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI, 0x0);
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, 1, 0x0);
#endif
        }
      
        else if ( SIM_SAT_SETUP_EVENT_LIST_IND == pSimSatActivateData->nSAT_CmdType ) 
        {
          SAT_BASE_RSP* pBaseResp = (SAT_BASE_RSP*)CSW_SIM_MALLOC(SIZEOF(SAT_BASE_RSP));
          if (pBaseResp == NULL)
          {
#ifdef CFW_MULTI_SIM 
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_ZeroMemory8(pBaseResp, (SIZEOF(SAT_BASE_RSP)));
          pBaseResp->nComID        = pG_SimSat->nCurCmd;
          pBaseResp->nComQualifier = nComQualifier;
          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);
          pG_SimSat->pSatComData = pBaseResp;

          UINT8 nRUti = 0x00;

          CFW_GetFreeUTI(0, &nRUti);

          // #ifdef CFW_DUAL_SIM
          // CFW_SatResponse(pSimSatActivateData->nSAT_CmdType, 0x00, 0x00, NULL, 0x00, nRUti,nSimID);
          api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
            (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
          SAT_BASE_RSP* pBase = (SAT_BASE_RSP*)(pG_SimSat->pSatComData);

          if ((pSimTerminalResponesReq == NULL) || (pBase == NULL))
          {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

          pSimTerminalResponesReq->NbByte = RESPONSEBASE;

          // SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, RESPONSEBASE);
#ifdef CFW_MULTI_SIM 
          if (0x00 == nCommondNum[pSimSatActivateData->nSAT_CmdType - 1][nSimID])
#else
          if (0x00 == nCommondNum[pSimSatActivateData->nSAT_CmdType - 1])
#endif
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, pSimTerminalResponesReq->NbByte);
          else
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse1, pSimTerminalResponesReq->NbByte);
#ifdef CFW_MULTI_SIM 
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatActivateData->nSAT_CmdType - 1][nSimID];
#else
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatActivateData->nSAT_CmdType - 1];
#endif

          // pSimTerminalResponesReq->TerminalRespData[ComType -1]   = 0x02;

          pSimTerminalResponesReq->TerminalRespData[ComType]   = 0x05;
          pSimTerminalResponesReq->TerminalRespData[ComQual]   = 0x00;
          pSimTerminalResponesReq->TerminalRespData[ComResult] = 0x00;

          CSW_TRACE(CFW_SIM_TS_ID, "nSAT_CmdType    %x\n", pSimSatActivateData->nSAT_CmdType);
          CSW_TRACE(CFW_SIM_TS_ID, "nComQualifier   %x\n", pBase->nComQualifier);
          CSW_TRACE(CFW_SIM_TS_ID, "nSAT_Status     %x\n", pSimSatActivateData->nSAT_Status);

          // CSW_TC_MEMBLOCK(CFW_SIM_TS_ID, (UINT8 *) pSimTerminalResponesReq, sizeof(api_SimTerminalResponseReq_t), 16);
#ifdef CFW_MULTI_SIM
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_SCL_QUEUE_FULL, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#else
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_SCL_QUEUE_FULL, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#endif
          CFW_SetAoProcCode(hSimSatActivateAo, CFW_AO_PROC_CODE_CONSUMED);
          pSimSatActivateData->nSm_Resp_prevStatus = pSimSatActivateData->nSm_Resp_currStatus;
          pSimSatActivateData->nSm_Resp_currStatus = SM_RESP_TERMINALRESPONSE;
          pSimSatActivateData->nSAT_InnerProcess = TRUE;
          // CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI|(nSimID<<24), 0x0);
          // #else
          // CFW_SatResponse(pSimSatActivateData->nSAT_CmdType, 0x00, 0x00, NULL, 0x00, nRUti);
          // CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI, 0x0);
          // #endif
          return ERR_SUCCESS;
        }
        else if ( SIM_SAT_FRESH_IND == pSimSatActivateData->nSAT_CmdType ) // Refresh
        {
          SAT_BASE_RSP* pBaseResp = (SAT_BASE_RSP*)CSW_SIM_MALLOC(SIZEOF(SAT_BASE_RSP));
          if (pBaseResp == NULL)
          {
#ifdef CFW_MULTI_SIM 
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_ZeroMemory8(pBaseResp, (SIZEOF(SAT_BASE_RSP)));
          pBaseResp->nComID        = pG_SimSat->nCurCmd;
          pBaseResp->nComQualifier = nComQualifier;
          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);
          pG_SimSat->pSatComData = pBaseResp;

          UINT8 nRUti = 0x00;

          CFW_GetFreeUTI(0, &nRUti);

          // #ifdef CFW_DUAL_SIM
          // CFW_SatResponse(pSimSatActivateData->nSAT_CmdType, 0x00, 0x00, NULL, 0x00, nRUti,nSimID);
          api_SimTerminalResponseReq_t* pSimTerminalResponesReq =
            (api_SimTerminalResponseReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimTerminalResponseReq_t));
          SAT_BASE_RSP* pBase = (SAT_BASE_RSP*)(pG_SimSat->pSatComData);

          if ((pSimTerminalResponesReq == NULL) || (pBase == NULL))
          {
#ifdef CFW_MULTI_SIM 
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

          pSimTerminalResponesReq->NbByte = RESPONSEBASE;

          // SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, RESPONSEBASE);
#ifdef CFW_MULTI_SIM 
          if (0x00 == nCommondNum[pSimSatActivateData->nSAT_CmdType - 1][nSimID])
#else
          if (0x00 == nCommondNum[pSimSatActivateData->nSAT_CmdType - 1])
#endif
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse, pSimTerminalResponesReq->NbByte);
          else
            SUL_MemCopy8(pSimTerminalResponesReq->TerminalRespData, TerminalResponse1, pSimTerminalResponesReq->NbByte);
#ifdef CFW_MULTI_SIM 
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatActivateData->nSAT_CmdType - 1][nSimID];
#else
          pSimTerminalResponesReq->TerminalRespData[ComType - 1] =
            nCommondNum[pSimSatActivateData->nSAT_CmdType - 1];
#endif

          // pSimTerminalResponesReq->TerminalRespData[ComType -1]   = 0x02;

          pSimTerminalResponesReq->TerminalRespData[ComType]   = 0x01;
          pSimTerminalResponesReq->TerminalRespData[ComQual]   = 0x00;
          pSimTerminalResponesReq->TerminalRespData[ComResult] = 0x00;

          CSW_TRACE(CFW_SIM_TS_ID, "nSAT_CmdType    %x\n", pSimSatActivateData->nSAT_CmdType);
          CSW_TRACE(CFW_SIM_TS_ID, "nComQualifier   %x\n", pBase->nComQualifier);
          CSW_TRACE(CFW_SIM_TS_ID, "nSAT_Status     %x\n", pSimSatActivateData->nSAT_Status);

          // CSW_TC_MEMBLOCK(CFW_SIM_TS_ID, (UINT8 *) pSimTerminalResponesReq, sizeof(api_SimTerminalResponseReq_t), 16);
#ifdef CFW_MULTI_SIM 
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq, nSimID))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_SCL_QUEUE_FULL, 0, nUTI | (nSimID << 24), 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#else
          if (ERR_SUCCESS != CFW_SendSclMessage(API_SIM_TERMINALRESPONSE_REQ, pSimTerminalResponesReq))
          {
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_SCL_QUEUE_FULL, 0, nUTI, 0xF0);
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }

#endif
          CFW_SetAoProcCode(hSimSatActivateAo, CFW_AO_PROC_CODE_CONSUMED);
          pSimSatActivateData->nSm_Resp_prevStatus = pSimSatActivateData->nSm_Resp_currStatus;
          pSimSatActivateData->nSm_Resp_currStatus = SM_RESP_TERMINALRESPONSE;
          pSimSatActivateData->nSAT_InnerProcess = TRUE;
          // CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI|(nSimID<<24), 0x0);
          // #else
          // CFW_SatResponse(pSimSatActivateData->nSAT_CmdType, 0x00, 0x00, NULL, 0x00, nRUti);
          // CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI, 0x0);
          // #endif
          return ERR_SUCCESS;
        }        
        else if (SIM_SAT_POLL_INTERVAL_IND == pSimSatActivateData->nSAT_CmdType )
        {
          // UINT8 nCodeSch      = 0x00;
          UINT8 nLen          = 0x00;
          UINT8 nTimeUnit     = 0x00;
          UINT8 nTimeInterval = 0x00;

          // UINT8* pTextStr =NULL;
          CFW_SAT_POLL_INTERVAL_RSP* pPIResp = NULL;

          nIndex = nIndex + 10;

          if ((pOffset[nIndex] == 0x04) || (pOffset[nIndex] == 0x84))
          {
            nLen          = pOffset[nIndex++];
            nTimeUnit     = pOffset[nIndex++];
            nTimeInterval = pOffset[nIndex++];
          }
          else
          {
#ifdef CFW_MULTI_SIM 
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          pPIResp = (CFW_SAT_POLL_INTERVAL_RSP*)(CSW_SIM_MALLOC(SIZEOF(CFW_SAT_POLL_INTERVAL_RSP)));
          if (pPIResp == NULL)
          {
#ifdef CFW_MULTI_SIM 
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_ZeroMemory8(pPIResp, (SIZEOF(CFW_SAT_POLL_INTERVAL_RSP)));
          pPIResp->nComID        = pG_SimSat->nCurCmd;
          pPIResp->nComQualifier = nComQualifier;
          pPIResp->nTimeUnit     = nTimeUnit;
          pPIResp->nTimeInterval = nTimeInterval;
          CSW_TRACE(CFW_SIM_TS_ID, "pG_SimSat->nCurCmd 0x%x,nComQualifier 0x%x,nTimeUnit 0x%x,nTimeInterval 0x%x\n",
                        pG_SimSat->nCurCmd, nComQualifier, nTimeUnit, nTimeInterval);

          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);

          pG_SimSat->pSatComData = pPIResp;
#ifdef CFW_MULTI_SIM 
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI | (nSimID << 24),
                              0x00);
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, 1 | (nSimID << 24),
                              0x00);
#else
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI, 0x0);
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, 1, 0x0);
#endif

        }
        
        else if( SIM_SAT_SEND_USSD_IND == pSimSatActivateData->nSAT_CmdType ) // Send USSD
        {
          UINT8   nAlphaLen = 0x00;
          UINT8*  pAlphaStr = NULL;           
          UINT8   nSSLen    = 0x00;
          UINT8   nSSType   = 0x00;
          UINT8*  pSSStr    = NULL; 
          UINT8   nMemsize  = 0x00;
          UINT8   nIconQual = 0x00;
          UINT8   nIconIdentifer = 0x00;
          CFW_SAT_SS_RSP* pSSRsp = NULL;

          nMemsize += SIZEOF(CFW_SAT_SS_RSP);
          nIndex = nIndex + 10;
          if ((pOffset[nIndex] == 0x05) || (pOffset[nIndex] == 0x85))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength   = pOffset[nIndex++];
            nAlphaLen = nLength;
            pAlphaStr = pOffset + nIndex;
          }
          else
            nLength = 0;
          nIndex    = nIndex + nLength;
          nMemsize += nLength;

          if ((pOffset[nIndex] == 0x0a) || (pOffset[nIndex] == 0x8a))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength   = pOffset[nIndex++] - 1;
            nSSLen  = nLength;
            nSSType = pOffset[nIndex++];
            pSSStr  = pOffset + nIndex;
          }
          else
            nLength = 0;
          nIndex    = nIndex + nLength;
          nMemsize += nLength;
          if ((pOffset[nIndex] == 0x1E) || (pOffset[nIndex] == 0x9E))
          {
            if (pOffset[++nIndex] == 0x81)
              nIndex++;
            nLength  = pOffset[nIndex++];
            nIconQual = pOffset[nIndex++];
            nIconIdentifer = pOffset[nIndex];
          }
          else
          {
            nLength = 0;
#if 0            
            CSW_TRACE(CFW_SIM_TS_ID, "SS Send Mem size=%d\n", nMemsize);
#ifdef CFW_MULTI_SIM 
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
#endif            
          }
          nMemsize += nLength;

          pSSRsp = (CFW_SAT_SS_RSP*)CSW_SIM_MALLOC(nMemsize);
          CSW_TRACE(CFW_SIM_TS_ID, "SS Send Mem size=%d\n", nMemsize);

          if (pSSRsp == NULL)
          {
#ifdef CFW_MULTI_SIM 
            CFW_PostNotifyEvent(ERR_PARAM_ILLEGAL, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(ERR_PARAM_ILLEGAL, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_ZeroMemory8(pSSRsp, nMemsize);
          pSSRsp->nComID        = pG_SimSat->nCurCmd;
          pSSRsp->nComQualifier = nComQualifier;

          pSSRsp->nAlphaLen = nAlphaLen;
          pSSRsp->pAlphaStr = (UINT8*)pSSRsp + SIZEOF(CFW_SAT_SS_RSP);
          SUL_MemCopy8(pSSRsp->pAlphaStr, pAlphaStr, nAlphaLen);

          pSSRsp->nSSLen = nSSLen;
          pSSRsp->pSSStr = pSSRsp->pAlphaStr + nAlphaLen;
          pSSRsp->nSSType = nSSType;
          SUL_MemCopy8(pSSRsp->pSSStr, pSSStr, nSSLen);

          pSSRsp->nIconQual = nIconQual;
          pSSRsp->nIconIdentifer = nIconIdentifer;
          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);

          pG_SimSat->pSatComData = pSSRsp;
#ifdef CFW_MULTI_SIM 
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI | (nSimID << 24),
                              0x0);
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, 1 | (nSimID << 24),
                              0x0);
#else
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI, 0x0);
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, 1, 0x0);
#endif
          
        }
        else 
        {
          SAT_BASE_RSP* pBaseResp = (SAT_BASE_RSP*)CSW_SIM_MALLOC(SIZEOF(SAT_BASE_RSP));
          if (pBaseResp == NULL)
          {
#ifdef CFW_MULTI_SIM 
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
            CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
            CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
            return ERR_SUCCESS;
          }
          SUL_ZeroMemory8(pBaseResp, (SIZEOF(SAT_BASE_RSP)));
          pBaseResp->nComID        = pG_SimSat->nCurCmd;
          pBaseResp->nComQualifier = nComQualifier;
          if (pG_SimSat->pSatComData != NULL)
            CSW_SIM_FREE(pG_SimSat->pSatComData);
          pG_SimSat->pSatComData = pBaseResp;
#ifdef CFW_MULTI_SIM 
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI | (nSimID << 24), 0x0);
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, 1 | (nSimID << 24), 0x0);
#else
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, nUTI, 0x0);
          CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatActivateData->nSAT_CmdType, 0, 1, 0x0);
#endif
        }        
        
        CFW_SetAoProcCode(hSimSatActivateAo, CFW_AO_PROC_CODE_CONSUMED);
               // DBG_ASSERT(0,"sat activetion");

        CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
      }
    }
    break;
  case SM_ACT_TERMINALRESPONSE:
    {
      if (pEvent->nEventId == API_SIM_TERMINALRESPONSE_CNF)
      {
        api_SimTerminalResponseCnf_t* pSimTerminalResponseCnf = (api_SimTerminalResponseCnf_t*)pEvent->nParam1;
        if ((pSimTerminalResponseCnf->Sw1 == 0x90) && (pSimTerminalResponseCnf->Sw2 == 0x00))
        {
#ifdef CFW_MULTI_SIM
          CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, 0x00, pSimTerminalResponseCnf->Sw1, nUTI | (nSimID << 24),
                              0x00);
#else
          CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, 0x00, pSimTerminalResponseCnf->Sw1, nUTI, 0x00);
#endif
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SatActivationProc);
          return ERR_SUCCESS;
        }
        else if (pSimTerminalResponseCnf->Sw1 == 0x91)
        {

          CSW_TRACE(CFW_SIM_TS_ID, "ERMINALPROFILE_CNF sw1 0x%x\n", pSimTerminalResponseCnf->Sw1);
          pSimSatActivateData->nSm_Act_prevStatus = pSimSatActivateData->nSm_Act_currStatus;
          pSimSatActivateData->nSm_Act_currStatus = SM_ACT_FETCH;
        }
        else
        {
          UINT32 nErrorCode = 0x00;

#ifdef CFW_MULTI_SIM
          nErrorCode = Sim_ParseSW1SW2(pSimTerminalResponseCnf->Sw1, pSimTerminalResponseCnf->Sw2, nSimID);
          CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, nErrorCode, 0, nUTI | (nSimID << 24), 0xF0);
#else
          nErrorCode = Sim_ParseSW1SW2(pSimTerminalResponseCnf->Sw1, pSimTerminalResponseCnf->Sw2);
          CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, nErrorCode, 0, nUTI, 0xF0);
#endif
          CFW_UnRegisterAO(CFW_SIM_SRV_ID, hSimSatActivateAo);
          CSW_PROFILE_FUNCTION_EXIT(CFW_SatActivationProc);
          return ERR_SUCCESS;

          // Return;
        }
        CSW_TRACE(CFW_SIM_TS_ID, "API_SIM_TERMINALPROFILE_CNF status sw1 0x%x\n", pSimTerminalResponseCnf->Sw1);
      }
      else
        CFW_SetAoProcCode(hSimSatActivateAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
    }
    break;

  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_SatActivationProc);
  return ERR_SUCCESS;
}

// =============================================================================
UINT32 cfw_SatProCmdProccess(HAO hAo, UINT8 nCmdType, UINT8* pData, UINT8 nLen)
{
  UINT32 ret = ERR_SUCCESS;

  if (nCmdType == 0x26)
  {
    ret = cfw_SatProCmdLocal(hAo, pData);
  }
  else if (nCmdType == 0x25)
  {
    ret = cfw_SatProCmdSetupMenu(hAo, pData, nLen);
  }
  return ret;
}
// =============================================================================
UINT32 cfw_SatProCmdSetupMenu(HAO hAo, UINT8* pData, UINT8 nLength)
{
  UINT8 i         = 0;
  UINT8 nIndex    = 0;
  UINT16 nMemsize = 0;  // memory size
  UINT8 nAlphaLen = 0;
  UINT32 nItemNum = 0;
  UINT8* pOffset  = pData;

  CFW_SAT_MENU_RSP* pMenuItemList = NULL;
  CFW_SAT_ITEM_LIST* pItemList = NULL;
  SIM_SAT_PARAM* pG_SimSat = NULL;
  UINT32 nUTI;


  CSW_TC_MEMBLOCK(CFW_SIM_TS_ID,pData,nLength,16);
  //COS_Sleep(100);
  
 // DBG_ASSERT(0,"setupmenu");
#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hAo);
  CFW_CfgSimGetSatParam(&pG_SimSat, nSimID);
#else
  CFW_CfgSimGetSatParam(&pG_SimSat);
#endif
  nIndex = 5;
  CFW_GetUTI(hAo, &nUTI);

  if ((pData[nIndex] == 0x05) || (pData[nIndex] == 0x85))
  {
    if (pData[++nIndex] == 0x81)
      nIndex++;
    nAlphaLen = pData[nIndex++];
    i         = nIndex + nAlphaLen;
  }
  else
  {
#ifdef CFW_MULTI_SIM
    CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, ERR_PARAM_ILLEGAL, 0, nUTI | (nSimID << 24), 0xF0);
#else
    CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, ERR_PARAM_ILLEGAL, 0, nUTI, 0xF0);
#endif
    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SatActivationProc);
    CSW_TRACE(CFW_SIM_TS_ID, "Get Fetch Data err 0x%x\n",pData[nIndex]);
    return ERR_SUCCESS;
  }
  nMemsize = SIZEOF(CFW_SAT_MENU_RSP) + nAlphaLen;

  //nLength = nLength - i;
  while (i < nLength)
  {
    if ((pOffset[i] != 0x0F) && (pOffset[i] != 0x8F))
      break;
    nItemNum++;
    nMemsize += SIZEOF(CFW_SAT_ITEM_LIST);
    //nMemsize += pOffset[++i] - 1;
    nMemsize += pOffset[++i] ;
    i = i + pOffset[i];
    i++;
  }

  // ??
  CSW_TRACE(CFW_SIM_TS_ID, "nMemsize: 0x%x\n", nMemsize);

  pMenuItemList = (CFW_SAT_MENU_RSP*)CSW_SIM_MALLOC(nMemsize);
  if (pMenuItemList == NULL)
  {
#ifdef CFW_MULTI_SIM
    CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
    CFW_PostNotifyEvent(EV_CFW_SAT_ACTIVATION_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SatActivationProc);
    return ERR_SUCCESS;
  }
  SUL_ZeroMemory8(pMenuItemList, (nMemsize));
  pMenuItemList->nComID = 0x25;

  pMenuItemList->nComQualifier = *pData;
  pMenuItemList->nAlphaLen     = nAlphaLen;
  nItemNum                     = SIZEOF(CFW_SAT_ITEM_LIST) * nItemNum;

  pMenuItemList->pAlphaStr = SIZEOF(CFW_SAT_MENU_RSP) + (UINT8*)pMenuItemList + nItemNum;
  SUL_MemCopy8(pMenuItemList->pAlphaStr, (pOffset + nIndex), nAlphaLen);
  nItemNum                 = (UINT32)(pMenuItemList->pAlphaStr) + nAlphaLen;
  pMenuItemList->pItemList = (CFW_SAT_ITEM_LIST*)(SIZEOF(CFW_SAT_MENU_RSP) + (UINT8*)pMenuItemList);
  
  pItemList = pMenuItemList->pItemList;
  i         = nIndex + nAlphaLen;

  CFW_SAT_ITEM_LIST* pTemp = NULL;
  for (;;)
  {
    if ((pOffset[i] != 0x0F) && (pOffset[i] != 0x8F))
    {
      pTemp->pNextItem = NULL;
      break;
    }
    if(pOffset[i + 1])
      pItemList->nItemLen = pOffset[++i] - 1; // reduce the item's id
    else
      pItemList->nItemLen = 0x00;
    
    pItemList->nItemID  = pOffset[++i];
    pItemList->pItemStr = (UINT8*)nItemNum;
    nItemNum += pItemList->nItemLen;

    SUL_MemCopy8(pItemList->pItemStr, (pOffset + i + 1), pItemList->nItemLen);
    pItemList->pNextItem = (CFW_SAT_ITEM_LIST*)(SIZEOF(CFW_SAT_ITEM_LIST) + (UINT8*)pItemList);
    //CSW_TRACE(CFW_SIM_TS_ID, "SM_ACT_FETCH\n");
    i = i + pItemList->nItemLen;
    if (++i < nLength)
    {
      pTemp     = pItemList;
      pItemList = pItemList->pNextItem;
    }
    else
    {
      pItemList->pNextItem = NULL;
      break;
    }
  }

  if (pG_SimSat->pSetupMenu != NULL)
    CSW_SIM_FREE(pG_SimSat->pSetupMenu);
  pG_SimSat->pSetupMenu = pMenuItemList;
  g_SAT_INIT_COMPLETE   = TRUE; // g__SAT_INIT_COMPLETE  to determine whether init is complete?? 
#ifdef CFW_MULTI_SIM
  CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pG_SimSat->pSetupMenu->nComID, 0, nUTI | (nSimID << 24), 0x0);
#else
  CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pG_SimSat->pSetupMenu->nComID, 0, nUTI, 0x0);
#endif
  return ERR_SUCCESS; // [+ ] dingmx 20080908 remove warning
}

// =============================================================================
UINT32 cfw_SatProCmdLocal(HAO hAo, UINT8* pData)
{
  UINT32 nUTI;

  SIM_SAT_PARAM* pG_SimSat = NULL;

  CFW_GetUTI(hAo, &nUTI);
  SAT_BASE_RSP* pBaseResp = (SAT_BASE_RSP*)CSW_SIM_MALLOC(SIZEOF(SAT_BASE_RSP));

#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID;

  nSimID = CFW_GetSimCardID(hAo);
  CFW_CfgSimGetSatParam(&pG_SimSat, nSimID);
#else
  CFW_CfgSimGetSatParam(&pG_SimSat);
#endif

  if (pBaseResp == NULL)
  {
#ifdef CFW_MULTI_SIM
    CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI | (nSimID << 24), 0xF0);
#else
    CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, nUTI, 0xF0);
#endif
    CFW_UnRegisterAO(CFW_SIM_SRV_ID, hAo);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
    return ERR_SUCCESS;
  }

  pBaseResp->nComID        = 0x26;
  pBaseResp->nComQualifier = pData[0];
  if (pG_SimSat->pSatComData != NULL)
    CSW_SIM_FREE(pG_SimSat->pSatComData);
  pG_SimSat->pSatComData = pBaseResp;
#ifdef CFW_MULTI_SIM
  CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, 0x26, 0, nUTI | (nSimID << 24), 0xF0);
#else
  CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, 0x26, 0, nUTI, 0x0);
#endif
  CSW_TRACE(CFW_SIM_TS_ID, "EV_CFW_SAT_CMDTYPE_IND ComID= 0x%x\n", 0x26);
  return ERR_SUCCESS;
}
 
