// //////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2002-2010, Coolsand Technologies, Inc.
// All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmission of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
// 
// FILENAME: cfw_sim.c
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
#include <cfw.h>
#include <base.h>
#include <cfw_prv.h>
#include <event.h>
#include <ts.h>
#include <sul.h>

#include "cfw_sim.h"
#include "csw_debug.h"
#include "api_msg.h"

#ifdef CFW_MULTI_SIM
// The same data as MTK Trace info
CFW_PROFILE nMEProfile = {
  0x07,
  16,
  {
   0x0FF,
   0x0FF,
   0x0FF,
   0x0FF,
   0x07F,
   0x00F,
   0x000,
   0x05F,
   0x07F,

   0x000,
   0x000,
   0x01F,
   0x023,
   0x009,
   0x009,
   0x006,
   0x0FF,
   0x0FF,

   0x0FF}
};

#else

CFW_PROFILE nMEProfile = {
  0x07,
  19,
#  if 0
  {0x0F,  // Profile download-------->1
   // SMS-PP data download------>1
   // Cell Broadcast data download-------->1
   // Menu selection-------->1
   // '9EXX' response code for SIM data download error------->0
   // Timer expiration--------->0
   // USSD string data object supported in Call Control------->0
   // Envelope Call Control always sent to the SIM during automatic redial mode------->0

   0x6B,  // Command result----------->1
   // Call Control by SIM----------->1
   // Cell identity included in Call Control by SIM----------->0
   // MO short message control by SIM----------->1
   // Handling of the alpha identifier according to subclause 9.1.3----------->0
   // UCS2 Entry supported----------->1
   // UCS2 Display supported----------->1
   // Display of the extension text----------->0

   0x4D,  // Proactive SIM: DISPLAY TEXT----------->1
   // Proactive SIM: GET INKEY----------->0
   // Proactive SIM: GET INPUT----------->1
   // Proactive SIM: MORE TIME----------->1
   // Proactive SIM: PLAY TONE----------->0
   // Proactive SIM: POLL INTERVAL----------->0
   // Proactive SIM: POLLING OFF----------->1
   // Proactive SIM: REFRESH----------->0

   0xF3,  // Proactive SIM: SELECT ITEM ----------->1
   // Proactive SIM: SEND SHORT MESSAGE ----------->1
   // Proactive SIM: SEND SS ----------->0
   // Proactive SIM: SEND USSD----------->0
   // Proactive SIM: SET UP CALL----------->1
   // Proactive SIM: SET UP MENU----------->1
   // Proactive SIM: PROVIDE LOCAL INFORMATION (MCC, MNC, LAC, Cell ID & IMEI)----------->1
   // Proactive SIM: PROVIDE LOCAL INFORMATION (NMR)----------->1

   0x0E,  // Proactive SIM: SET UP EVENT LIST ----------->0
   // Event: MT call----------->1
   // Event: Call connected----------->1
   // Event: Call disconnected----------->1
   // Event: Location status----------->0
   // Event: User activity----------->0
   // Event: Idle screen available----------->0
   // Event: Card reader status----------->0

   0x09,  // Event: Language selection----------->1
   // Event: Browser Termination----------->0
   // Event: Data available----------->0
   // Event: Channel status----------->1
   // RFU, bit = 0

   0x0A,  // Proactive SIM: POWER ON CARD----------->0
   // Proactive SIM: POWER OFF CARD----------->1
   // Proactive SIM: PERFORM CARD APDU ----------->0
   // Proactive SIM: GET READER STATUS (Card reader status)----------->1
   // Proactive SIM: GET READER STATUS (Card reader identifier)----------->0
   // RFU, bit = 0

   0x04,  // Proactive SIM: TIMER MANAGEMENT (start, stop)----------->0
   // Proactive SIM: TIMER MANAGEMENT (get current value)----------->0
   // Proactive SIM: PROVIDE LOCAL INFORMATION (date, time and time zone)----------->1
   // Binary choice in GET INKEY----------->0
   // SET UP IDLE MODE TEXT----------->0
   // RUN AT COMMAND (i.e. class "b" is supported)----------->0
   // 2nd alpha identifier in SET UP CALL----------->0
   // 2nd capability configuration parameter (see 9.1.6)----------->0

   0x3D,  // Sustained DISPLAY TEXT (see 6.4.1)----------->1
   // SEND DTMF command (see 6.4.24)----------->0
   // Proactive SIM: PROVIDE LOCAL INFORMATION - BCCH Channel List coding as in subclause 12.29)----------->1
   // Proactive SIM: PROVIDE LOCAL INFORMATION (language)----------->1
   // Proactive SIM: PROVIDE LOCAL INFORMATION (Timing Advance)----------->1
   // Proactive SIM: LANGUAGE NOTIFICATION----------->1
   // Proactive SIM: LAUNCH BROWSER----------->0
   // RFU, bit = 0

   0x00,  // Soft keys support for SELECT ITEM (see 6.4.9)----------->0
   // Soft Keys support for SET UP MENU (see 6.4.8)----------->0
   // RFU, bit = 0
   // RFU, bit = 0
   // RFU, bit = 0
   // RFU, bit = 0
   // RFU, bit = 0
   // RFU, bit = 0

   0x02,  // Maximum number of soft keys available
   // 'FF' value is reserved for future use

   0x00,

   // Proactive SIM: OPEN CHANNEL----------->0
   // Proactive SIM: CLOSE CHANNEL ----------->0
   // Proactive SIM: RECEIVE DATA----------->0
   // Proactive SIM: SEND DATA----------->0 
   // Proactive SIM: GET CHANNEL STATUS----------->0 
   // RFU, bit = 0
   // RFU, bit = 0
   // RFU, bit = 0
   0x03,

   // CSD supported by ME----------->1
   // GPRS supported by ME----------->1
   // RFU, bit = 0
   // RFU, bit = 0
   // RFU, bit = 0
   // Number of channels supported by ME 

   0x0C,

   // bit0  - bit4 :Number of characters supported down the ME display as defined in 5.3.1
   // bit 5 - bit 6 :RFU, bit = 0
   // bit 7 :Screen Sizing Parameters supported as defined in section 5.3

   0x08,

   // bit 0 - bit 6 :Number of characters supported across the ME display as defined in 5.3.2
   // bit 7 :Variable size fonts Supported

   0x06,  // bit 0 :Display can be resized as defined in 5.3.3----------->0
   // bit 1 :Text Wrapping supported as defined in 5.3.4----------->1
   // bit 2 :Text Scrolling supported as defined in 5.3.5----------->1
   // bit 3 :RFU
   // bit 4 :RFU
   // bit 5 - bit 6 :Width reduction when in a menu as defined in 5.3.6

   0x03,  // bit 0 :TCP
   // bit 1 :UDP
   // bit 2 - bit 7 :RFU, bit = 0

   0x00,  // RFU, bit = 0

   0x00,  // bit 0 - bit 3 :Protocol Version, coded as indicated in TIA/EIA-136-123 [35]
   // bit 4 - bit 7 :bit = 0
   }
#  else
  {
   0x0FF,
   0x0FF,
   0x0FF,
   0x0FF,
   0x0FF,
   0x0FF,
   0x0FF,
   0x0FF,
   0x0FF,

   0x0FF,
   0x0FF,
   0x0FF,
   0x0FF,
   0x0FF,
   0x0FF,
   0x0FF,
   0x0FF,
   0x0FF,

   0x0FF}
#  endif
};

#endif

// b0  == FDN
// b1  == BDN
// b2  == CC
// 0: Not Support,1: Support

#ifdef CFW_MULTI_SIM
UINT32 CFW_SimGetMeProfile(CFW_PROFILE** pMeProfile, CFW_SIM_ID nSimID)
{
#  if 0
  if (CFW_SIM_0 == nSimID)
    *pMeProfile = &nMEProfileSim0;
  else if (CFW_SIM_1 == nSimID)
    *pMeProfile = &nMEProfileSim1;

  else
    return ERR_CFW_INVALID_PARAMETER;
#  endif
  *pMeProfile = &nMEProfile;
  return ERR_SUCCESS;
}
#else
UINT32 CFW_SimGetMeProfile(CFW_PROFILE** pMeProfile)
{
  *pMeProfile = &nMEProfile;
  return ERR_SUCCESS;
}
#endif

UINT32 Sim_ParseSW1SW2(UINT8 SW1, UINT8 SW2
#ifdef CFW_MULTI_SIM
                       , CFW_SIM_ID nSimID
#endif
  )
{
  CSW_TRACE(CFW_SIM_TS_ID, "Function:Sim_ParseSW1SW2\n");

  SIM_CHV_PARAM* pG_Chv_Param = NULL;
  CSW_PROFILE_FUNCTION_ENTER(Sim_ParseSW1SW2);

#ifdef CFW_MULTI_SIM
  CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
#else
  CFW_CfgSimGetChvParam(&pG_Chv_Param);
#endif

  CSW_TRACE(CFW_SIM_TS_ID, "SIM parse SW1(2) 0x%x,0x%x\n", SW1, SW2);

  if ((SW1 == 0x92) && (SW2 == 0x40))
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_MEMORY_FAILURE memory problem\n");
    CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
    return ERR_CME_MEMORY_FAILURE;
  }
  else if ((SW1 == 0x93) && (SW2 == 0x00))
  {
    CSW_TRACE(CFW_SIM_TS_ID,
              "Error ERR_CME_SIM_STK_BUSY SIM Application Toolkit is busy. Command cannot be executed at present, further normal commands are allowed.\n");
    CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
    return ERR_CME_SIM_STK_BUSY;
  }
  else if (SW1 == 0x94)
  {
    if (SW2 == 0x00)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_NOEF_SELECTED no EF selected\n");
      CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
      return ERR_CME_SIM_NOEF_SELECTED;
    }
    else if (SW2 == 0x02)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_INVALID_INDEX out of range (invalid address)\n");
      CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
      return ERR_CME_INVALID_INDEX;

    }
    else if (SW2 == 0x04)
    {
      if (pG_Chv_Param->nFunID == SIM_SEEK)
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_PATTERN_NOT_FOUNDpattern not found\n");
        CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
        return ERR_CME_SIM_PATTERN_NOT_FOUND;
      }
      else
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_PATTERN_NOT_FOUND file ID not found\n");
        CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
        return ERR_CME_SIM_FILEID_NOT_FOUND;
      }
    }
    else if (SW2 == 0x08)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_FILE_UNMATCH_COMMAND file is inconsistent with the command\n");
      CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
      return ERR_CME_SIM_FILE_UNMATCH_COMMAND;
    }
  }
  else if (SW1 == 0x98)
  {
    if (SW2 == 0x02)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_CHV_UNINIT no CHV initialized\n");
      CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
      return ERR_CME_SIM_CHV_UNINIT;  // ERR_CME_SIM_PUK_REQUIRED;
    }
    else if (SW2 == 0x04)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error  	access condition not fulfilled \n");

      if (pG_Chv_Param->nFunID == SIM_UNBLOCKCHV)
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_UNBLOCK_FAIL\n");
        CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
        return ERR_CME_SIM_UNBLOCK_FAIL;
      }
      else if (pG_Chv_Param->nFunID == SIM_VERIFYCHV)
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_VERIFY_FAIL\n");
        CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
        return ERR_CME_SIM_VERIFY_FAIL;
      }
      else
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_CONDITION_NO_FULLFILLED\n");
        CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
        return ERR_CME_SIM_CONDITION_NO_FULLFILLED;
      }
    }
    else if (SW2 == 0x08)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_CONTRADICTION_CHV in contradiction with CHV status\n");
      CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
      return ERR_CME_SIM_CONTRADICTION_CHV;
    }
    else if (SW2 == 0x10)
    {
      CSW_TRACE(CFW_SIM_TS_ID,
                "Error ERR_CME_SIM_CONTRADICTION_INVALIDATION in contradiction with invalidation status\n");
      CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
      return ERR_CME_SIM_CONTRADICTION_INVALIDATION;
    }
    else if (SW2 == 0x40)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error  	unsuccessful CHV verification, no attempt left \n");

      if (pG_Chv_Param->nFunID == SIM_UNBLOCKCHV)
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_UNBLOCK_FAIL_NO_LEFT\n");
        CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);

// pG_Chv_Param->nPin1Status = CHV_BLOCKED;
        return ERR_CME_SIM_UNBLOCK_FAIL_NO_LEFT;
      }
      else if (pG_Chv_Param->nFunID == SIM_VERIFYCHV)
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_VERIFY_FAIL_NO_LEFT\n");
        CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
        return ERR_CME_SIM_VERIFY_FAIL_NO_LEFT;
      }
      else
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_CHV_NEED_UNBLOCK\n");
        CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
        return ERR_CME_SIM_CHV_NEED_UNBLOCK;
      }
    }
    else if (SW2 == 0x50)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_MAXVALUE_REACHED increase cannot be performed, Max value reached\n");
      CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
      return ERR_CME_SIM_MAXVALUE_REACHED;
    }
    else
    {
      CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_UNKNOW\n");
      CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
      return ERR_CME_SIM_UNKNOW;
    }
  }
  else if (SW1 == 0X67)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_INVALID_PARAMETER incorrect parameter P3\n");
    CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
    return ERR_CME_SIM_INVALID_PARAMETER;
  }
  else if (SW1 == 0X6B)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_INVALID_PARAMETER incorrect parameter P1 or P2\n");
    CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
    return ERR_CME_SIM_INVALID_PARAMETER;
  }
  else if (SW1 == 0X6D)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_UNKNOW_COMMAND unknown instruction code given in the command\n");
    CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
    return ERR_CME_SIM_UNKNOW_COMMAND;
  }
  else if (SW1 == 0X6E)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_WRONG_CLASS wrong instruction class given in the command\n");
    CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
    return ERR_CME_SIM_WRONG_CLASS;
  }
  else if (SW1 == 0X6F)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error ERR_CME_SIM_TECHNICAL_PROBLEM technical problem with no diagnostic given\n");
    CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
    return ERR_CME_SIM_TECHNICAL_PROBLEM;
  }
  else
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error No process Sim_ParseSW1SW2 SW1 0x%x, SW2 0x%x\n", SW1, SW2);

    // TODO..
  }

  CSW_TRACE(CFW_SIM_TS_ID, "Error Sim_ParseSW1SW2 SW1 0x%x, SW2 0x%x\n", SW1, SW2);

  CSW_PROFILE_FUNCTION_EXIT(Sim_ParseSW1SW2);
  return ERR_CME_SIM_UNKNOW;
}

UINT32 SimParseDedicatedStatus(UINT8* pDedicatedData
#ifdef CFW_MULTI_SIM
                               , CFW_SIM_ID nSimID
#endif
  )
{
  UINT8 nPin1Remain = 0;
  UINT8 nPuk1Remain = 0;
  UINT8 nPin2Remain = 0;
  UINT8 nPuk2Remain = 0;

  UINT8* pData = pDedicatedData;

  SIM_CHV_PARAM* pG_Chv_Param;

  CSW_PROFILE_FUNCTION_ENTER(SimParseDedicatedStatus);

#ifdef CFW_MULTI_SIM
  CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
#else
  CFW_CfgSimGetChvParam(&pG_Chv_Param);
#endif
  // Update Remain number of CHV
  nPin1Remain = pData[18] & 0x0F;
  nPuk1Remain = pData[19] & 0x0F;
  nPin2Remain = pData[20] & 0x0F;
  nPuk2Remain = pData[21] & 0x0F;

  // Set the status of the PIN1
  if ((pData[18] & 0x80) == 0)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "CHV Uninit\n");
    pG_Chv_Param->nPin1Status = CHV_UNINIT;
  }
  else if ((pData[13] & 0x80) == 0)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "CHV NOVERIFY\n");
    if (pG_Chv_Param->nPin1Status == CHV_UNKNOW)
      pG_Chv_Param->nPin1Status = CHV_NOVERIFY;
  }
  else
  {
    CSW_TRACE(CFW_SIM_TS_ID, "CHV DISABLE\n");
    pG_Chv_Param->nPin1Status = CHV_DISABLE;
  }

  // Set the status of the PUK1
  CSW_TRACE(CFW_SIM_TS_ID, "pData[19]&0x80 Value %x\n", pData[19] & 0x80);

  if ((pData[19] & 0x80) == 0)
    pG_Chv_Param->nPuk1Status = CHV_UNINIT;
  else if (pG_Chv_Param->nPuk1Status == CHV_UNKNOW)
    pG_Chv_Param->nPuk1Status = CHV_INIT;

  // Set the status of the PIN2
  CSW_TRACE(CFW_SIM_TS_ID, "pData[20]&0x80 Value %x\n", pData[19] & 0x80);
  if ((pData[20] & 0x80) == 0)
    pG_Chv_Param->nPin2Status = CHV_UNINIT;
  else if (pG_Chv_Param->nPin2Status == CHV_UNKNOW)
    pG_Chv_Param->nPin2Status = CHV_NOVERIFY;

  // Set teh status of the PUK2
  CSW_TRACE(CFW_SIM_TS_ID, "pData[21]&0x80 Value %x\n", pData[19] & 0x80);
  if ((pData[21] & 0x80) == 0)
    pG_Chv_Param->nPuk2Status = CHV_UNINIT;
  else if (pG_Chv_Param->nPuk2Status == CHV_UNKNOW)
    pG_Chv_Param->nPuk2Status = CHV_INIT;

  if (pG_Chv_Param->nPin1Status == CHV_UNINIT)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "CHV_UNINIT\n");
    pG_Chv_Param->nCHVAuthen = CFW_SIM_CARD_PIN1DISABLE;
  }
  else if (pG_Chv_Param->nPin1Status == CHV_NOVERIFY)
  {
    if (pG_Chv_Param->nPuk1Status == CHV_UNINIT)
    {
      CSW_TRACE(CFW_SIM_TS_ID, "nPin1Status: CHV_NOVERIFY, nPuk1Status: CHV_UNINIT\n");
      CSW_TRACE(CFW_SIM_TS_ID, "nPin1Remain: %d\n", nPin1Remain);
      if (nPin1Remain == 0)
        pG_Chv_Param->nCHVAuthen = CFW_SIM_CARD_PIN1BLOCK;
      else
        pG_Chv_Param->nCHVAuthen = CFW_SIM_CARD_PIN1; // need PIN1.
    }
    else
    {
      CSW_TRACE(CFW_SIM_TS_ID, "nPin1Status: CHV_NOVERIFY, nPuk1Status: %d\n", pG_Chv_Param->nPuk1Status);
      CSW_TRACE(CFW_SIM_TS_ID, "nPin1Remain: %d\n", nPuk1Remain);
      if (nPuk1Remain == 0)
        pG_Chv_Param->nCHVAuthen = CFW_SIM_CARD_PIN1BLOCK;  // PIN1 block.
      else if (nPin1Remain == 0)
        pG_Chv_Param->nCHVAuthen = CFW_SIM_CARD_PUK1; // need PUK1.
      else
      {
        CSW_TRACE(CFW_SIM_TS_ID, "Need input PIN1\n");
        pG_Chv_Param->nCHVAuthen = CFW_SIM_CARD_PIN1; // need PIN1
      }
    }
  }
  else if (pG_Chv_Param->nPin1Status == CHV_VERIFY)
  {
    // This process to report puk1 status after power on
    CSW_TRACE(CFW_SIM_TS_ID, "nPin1Status: CHV_VERIFY, nPuk1Remain: %d\n", nPuk1Remain);
    if (nPuk1Remain == 0)
      pG_Chv_Param->nCHVAuthen = CFW_SIM_CARD_PIN1BLOCK;  // PIN1 block.
    else if (nPin1Remain == 0)
      pG_Chv_Param->nCHVAuthen = CFW_SIM_CARD_PUK1; // need PUK1.
    else
      pG_Chv_Param->nCHVAuthen = CFW_SIM_CARD_PIN1READY;  // PIN1 ready.
  }
  else if (pG_Chv_Param->nPin1Status == CHV_DISABLE)
  {
    // This process to report puk1 status after power on
    CSW_TRACE(CFW_SIM_TS_ID, "nPin1Status: CHV_DISABLE, nPuk1Remain: %d\n", nPuk1Remain);

    if (nPuk1Remain == 0)
      pG_Chv_Param->nCHVAuthen = CFW_SIM_CARD_PIN1BLOCK;  // PIN1 block.
    else if (nPin1Remain == 0)
      pG_Chv_Param->nCHVAuthen = CFW_SIM_CARD_PUK1; // need PUK1.
    else
    {
      CSW_TRACE(CFW_SIM_TS_ID, "PIN1 Disable\n");
      pG_Chv_Param->nCHVAuthen = CFW_SIM_CARD_PIN1DISABLE;  // PIN1 disable.
    }
  }

  if (((pG_Chv_Param->nPin1Status == CHV_VERIFY) ||
       (pG_Chv_Param->nPin1Status == CHV_UNINIT) ||
       (pG_Chv_Param->nPin1Status == CHV_DISABLE)) && (pG_Chv_Param->bCHV2VerifyReq == TRUE))
  {
    CSW_TRACE(CFW_SIM_TS_ID, "nPuk2Status: %d, nPin2Remain: %d nPuk1Remain: %d\n", pG_Chv_Param->nPuk2Status,
              nPin2Remain, nPuk2Remain);

    if (pG_Chv_Param->nPuk2Status == CHV_UNINIT)
    {
      if (nPin2Remain == 0)
        pG_Chv_Param->nCHVAuthen = CFW_SIM_CARD_PIN2BLOCK;
      else
        pG_Chv_Param->nCHVAuthen = CFW_SIM_CARD_PIN2; // need PIN2.
    }
    else
    {
      if (nPuk2Remain == 0)
        pG_Chv_Param->nCHVAuthen = CFW_SIM_CARD_PIN2BLOCK;  // PIN2 block.
      else if (nPin2Remain == 0)
        pG_Chv_Param->nCHVAuthen = CFW_SIM_CARD_PUK2; // need PUK2.
      else
        pG_Chv_Param->nCHVAuthen = CFW_SIM_CARD_PIN2; // need PIN2
    }

  }
  CSW_PROFILE_FUNCTION_EXIT(SimParseDedicatedStatus);

  CSW_TRACE(CFW_SIM_TS_ID, "nPuk2Remain %d nPin2Remain %d nPuk1Remain %d nPin1Remain %d\n",
            nPuk2Remain, nPin2Remain, nPuk1Remain, nPin1Remain);

  return (((UINT16)nPuk2Remain << 12) | ((UINT16)nPin2Remain << 8) | (nPuk1Remain << 4) | nPin1Remain);
}

#ifdef CFW_MULTI_SIM
UINT32 SimResetReq(CFW_SIM_ID nSimID)
{
  return CFW_SendSclMessage(API_SIM_RESET_REQ, 0, nSimID);
}
#else
UINT32 SimResetReq(VOID)
{
  return CFW_SendSclMessage(API_SIM_RESET_REQ, 0);
}
#endif

UINT32 SimReadBinaryReq(UINT8 nFile, UINT8 nOffset, UINT8 nLen
#ifdef CFW_MULTI_SIM
                        , CFW_SIM_ID nSimID
#endif
  )
{
  api_SimReadBinaryReq_t* pSimReadBinaryReq = NULL;
  SIM_CHV_PARAM* pG_Chv_Param = NULL;
  CSW_PROFILE_FUNCTION_ENTER(SimReadBinaryReq);

#ifdef CFW_MULTI_SIM
  UINT32 nRet = CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);

  if (nRet != ERR_SUCCESS)
  {
    return nRet;
  }
#else
  CFW_CfgSimGetChvParam(&pG_Chv_Param);
#endif

  pG_Chv_Param->nFunID = SIM_READBINARY;

  pSimReadBinaryReq = (api_SimReadBinaryReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SimReadBinaryReq_t));
  if (pSimReadBinaryReq == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");
    CSW_PROFILE_FUNCTION_EXIT(SimReadBinaryReq);
    return ERR_NO_MORE_MEMORY;
  }
  pSimReadBinaryReq->File    = nFile;
  pSimReadBinaryReq->Offset  = nOffset;
  pSimReadBinaryReq->NbBytes = nLen;
  CSW_PROFILE_FUNCTION_EXIT(SimReadBinaryReq);
#ifdef CFW_MULTI_SIM
  return CFW_SendSclMessage(API_SIM_READBINARY_REQ, pSimReadBinaryReq, nSimID);
#else
  return CFW_SendSclMessage(API_SIM_READBINARY_REQ, pSimReadBinaryReq);
#endif
}

UINT32 SimUpdateBinaryReq(UINT8 nFile, UINT8 nOffset, UINT8 nLen, UINT8* pData
#ifdef CFW_MULTI_SIM
                          , CFW_SIM_ID nSimID
#endif
  )
{
  api_SimUpdateBinaryReq_t* pSimUpdateBinaryReq = NULL;
  SIM_CHV_PARAM* pG_Chv_Param = NULL;
  CSW_PROFILE_FUNCTION_ENTER(SimUpdateBinaryReq);
#ifdef CFW_MULTI_SIM
  CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
#else
  CFW_CfgSimGetChvParam(&pG_Chv_Param);
#endif
  pG_Chv_Param->nFunID = SIM_UPDATEBINARY;

  pSimUpdateBinaryReq = (api_SimUpdateBinaryReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SimUpdateBinaryReq_t));
  if (pSimUpdateBinaryReq == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(SimUpdateBinaryReq);
    return ERR_NO_MORE_MEMORY;
  }
  pSimUpdateBinaryReq->NbBytes = nLen;
  pSimUpdateBinaryReq->Offset  = nOffset;
  pSimUpdateBinaryReq->File    = nFile;
  SUL_MemCopy8(pSimUpdateBinaryReq->Data, pData, nLen);
  CSW_PROFILE_FUNCTION_EXIT(SimUpdateBinaryReq);
#ifdef CFW_MULTI_SIM
  return CFW_SendSclMessage(API_SIM_UPDATEBINARY_REQ, pSimUpdateBinaryReq, nSimID);
#else
  return CFW_SendSclMessage(API_SIM_UPDATEBINARY_REQ, pSimUpdateBinaryReq);
#endif
}

UINT32 SimElemFileStatusReq(UINT8 nFile
#ifdef CFW_MULTI_SIM
                            , CFW_SIM_ID nSimID
#endif
  )
{
  api_SimElemFileStatusReq_t* pSimElemFileStatusReq = NULL;
  CSW_PROFILE_FUNCTION_ENTER(SimElemFileStatusReq);

  pSimElemFileStatusReq = (api_SimElemFileStatusReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SimElemFileStatusReq_t));
  if (pSimElemFileStatusReq == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(SimElemFileStatusReq);
    return ERR_NO_MORE_MEMORY;
  }
  pSimElemFileStatusReq->File = nFile;
  CSW_PROFILE_FUNCTION_EXIT(SimElemFileStatusReq);
#ifdef CFW_MULTI_SIM
  return CFW_SendSclMessage(API_SIM_ELEMFILESTATUS_REQ, pSimElemFileStatusReq, nSimID);
#else
  return CFW_SendSclMessage(API_SIM_ELEMFILESTATUS_REQ, pSimElemFileStatusReq);
#endif
}

UINT32 SimDedicFileStatusReq(UINT8 nFile
#ifdef CFW_MULTI_SIM
                             , CFW_SIM_ID nSimID
#endif
  )
{
  CSW_TRACE(CFW_SIM_TS_ID, "Invoking SimDedicFileStatusReq\n");

  api_SimDedicFileStatusReq_t* pSimDedicFileStatusReq = NULL;
  CSW_PROFILE_FUNCTION_ENTER(SimDedicFileStatusReq);
  pSimDedicFileStatusReq =
    (api_SimDedicFileStatusReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SimDedicFileStatusReq_t));
  if (pSimDedicFileStatusReq == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(SimDedicFileStatusReq);
    return ERR_NO_MORE_MEMORY;
  }
  pSimDedicFileStatusReq->File = nFile;
  CSW_PROFILE_FUNCTION_EXIT(SimDedicFileStatusReq);
#ifdef CFW_MULTI_SIM
  return CFW_SendSclMessage(API_SIM_DEDICFILESTATUS_REQ, pSimDedicFileStatusReq, nSimID);
#else
  return CFW_SendSclMessage(API_SIM_DEDICFILESTATUS_REQ, pSimDedicFileStatusReq);
#endif
}

#ifdef CFW_MULTI_SIM
UINT32 SimReadRecordReq(UINT8 nFile, UINT8 nRecordNb, UINT8 nMode, UINT8 nLen, CFW_SIM_ID nSimID)
#else
UINT32 SimReadRecordReq(UINT8 nFile, UINT8 nRecordNb, UINT8 nMode, UINT8 nLen)
#endif
{
  CSW_TRACE(CFW_SIM_TS_ID, TSTXT("ReadRecordReq nFile[%s]nMode[%d]nRecordNb[%d]nLen[%d]\n"), TS_GetSIMFileName(nFile), nMode, nRecordNb,
            nLen);

  api_SimReadRecordReq_t* pSimReadRecordReq = NULL;
  SIM_CHV_PARAM* pG_Chv_Param = NULL;
  CSW_PROFILE_FUNCTION_ENTER(SimReadRecordReq);
#ifdef CFW_MULTI_SIM
  CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
#else
  CFW_CfgSimGetChvParam(&pG_Chv_Param);
#endif
  pG_Chv_Param->nFunID = SIM_READRECORD;

  pSimReadRecordReq = (api_SimReadRecordReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SimReadRecordReq_t));
  if (pSimReadRecordReq == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(SimReadRecordReq);
    return ERR_NO_MORE_MEMORY;
  }
  pSimReadRecordReq->File     = nFile;
  pSimReadRecordReq->Mode     = nMode;
  pSimReadRecordReq->NbByte   = nLen;
  pSimReadRecordReq->RecordNb = nRecordNb;
  CSW_PROFILE_FUNCTION_EXIT(SimReadRecordReq);

#ifdef CFW_MULTI_SIM
  return CFW_SendSclMessage(API_SIM_READRECORD_REQ, pSimReadRecordReq, nSimID);
#else
  return CFW_SendSclMessage(API_SIM_READRECORD_REQ, pSimReadRecordReq);
#endif
}

#ifdef CFW_MULTI_SIM
UINT32 SimUpdateRecordReq(UINT8 nFile, UINT8 nRecordNb, UINT8 nMode, UINT8 nLen, UINT8* pData, CFW_SIM_ID nSimID)
#else
UINT32 SimUpdateRecordReq(UINT8 nFile, UINT8 nRecordNb, UINT8 nMode, UINT8 nLen, UINT8* pData)
#endif
{
  CSW_TRACE(CFW_SIM_TS_ID, TSTXT("UpdateRecordReq nFile[%s]nMode[%d]nRecordNb[%d]nLen[%d]\n"), TS_GetSIMFileName(nFile), nMode, nRecordNb,
            nLen);
  CSW_TC_MEMBLOCK(CFW_API_TS_ID, (UINT8*)pData, nLen, 16);

  api_SimUpdateRecordReq_t* pSimUpdateRecordReq = NULL;
  SIM_CHV_PARAM* pG_Chv_Param = NULL;
  CSW_PROFILE_FUNCTION_ENTER(SimUpdateRecordReq);
#ifdef CFW_MULTI_SIM
  CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
#else
  CFW_CfgSimGetChvParam(&pG_Chv_Param);
#endif
  pG_Chv_Param->nFunID = SIM_UPDATERECORD;

  pSimUpdateRecordReq = (api_SimUpdateRecordReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SimUpdateRecordReq_t));
  if (pSimUpdateRecordReq == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(SimUpdateRecordReq);
    return ERR_NO_MORE_MEMORY;
  }
  pSimUpdateRecordReq->File     = nFile;
  pSimUpdateRecordReq->Mode     = nMode;
  pSimUpdateRecordReq->NbByte   = nLen;
  pSimUpdateRecordReq->RecordNb = nRecordNb;
  SUL_MemCopy8(pSimUpdateRecordReq->Data, pData, nLen);
  CSW_PROFILE_FUNCTION_EXIT(SimUpdateRecordReq);
#ifdef CFW_MULTI_SIM
  return CFW_SendSclMessage(API_SIM_UPDATERECORD_REQ, pSimUpdateRecordReq, nSimID);
#else
  return CFW_SendSclMessage(API_SIM_UPDATERECORD_REQ, pSimUpdateRecordReq);
#endif
}

#ifdef CFW_MULTI_SIM
UINT32 SimSeekReq(UINT8 nFile, UINT8 nTypeMode, UINT8 nLen, UINT8* pData, CFW_SIM_ID nSimID)
#else
UINT32 SimSeekReq(UINT8 nFile, UINT8 nTypeMode, UINT8 nLen, UINT8* pData)
#endif
{
  api_SimSeekReq_t* pSimSeekReq = NULL;
  SIM_CHV_PARAM* pG_Chv_Param = NULL;
  UINT32 nRet = ERR_SUCCESS;
  
  CSW_TRACE(CFW_SIM_TS_ID, TSTXT("SeekReq nFile[%s]nTypeMode[%d]nLen[%d]\n"), TS_GetSIMFileName(nFile), nTypeMode, nLen);
  CSW_TC_MEMBLOCK(CFW_API_TS_ID, (UINT8*)pData, nLen, 16);

  CSW_PROFILE_FUNCTION_ENTER(SimSeekReq);

#ifdef CFW_MULTI_SIM
  nRet=CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
#else
  nRet=CFW_CfgSimGetChvParam(&pG_Chv_Param);
#endif
  if(nRet != ERR_SUCCESS)
  {
  	return nRet;
  }
  pG_Chv_Param->nFunID = SIM_SEEK;

  pSimSeekReq = (api_SimSeekReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SimSeekReq_t));
  if (pSimSeekReq == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(SimSeekReq);
    return ERR_NO_MORE_MEMORY;
  }
  pSimSeekReq->File          = nFile;
  pSimSeekReq->TypeMode      = nTypeMode;
  pSimSeekReq->PatternNbByte = nLen;
  SUL_MemCopy8(pSimSeekReq->Patern, pData, nLen);
  CSW_PROFILE_FUNCTION_EXIT(SimSeekReq);
#ifdef CFW_MULTI_SIM
  return CFW_SendSclMessage(API_SIM_SEEK_REQ, pSimSeekReq, nSimID);
#else
  return CFW_SendSclMessage(API_SIM_SEEK_REQ, pSimSeekReq);
#endif
}

#ifdef CFW_MULTI_SIM
UINT32 SimVerifyCHVReq(UINT8* pCHVValue, UINT8 nCHVNumber, CFW_SIM_ID nSimID)
#else
UINT32 SimVerifyCHVReq(UINT8* pCHVValue, UINT8 nCHVNumber)
#endif
{
  CSW_TRACE(CFW_SIM_TS_ID, TSTXT("VerifyCHVReq nCHVNumber[%d]\n"), nCHVNumber);
  CSW_TC_MEMBLOCK(CFW_API_TS_ID, (UINT8*)pCHVValue, 8, 16);

  api_SimVerifyCHVReq_t* pSimVerifyCHVReq = NULL;
  SIM_CHV_PARAM* pG_Chv_Param = NULL;

  CSW_PROFILE_FUNCTION_ENTER(SimVerifyCHVReq);
#ifdef CFW_MULTI_SIM
  CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
#else
  CFW_CfgSimGetChvParam(&pG_Chv_Param);
#endif
  pG_Chv_Param->nFunID = SIM_VERIFYCHV;

  pSimVerifyCHVReq = (api_SimVerifyCHVReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SimVerifyCHVReq_t));
  if (pSimVerifyCHVReq == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(SimVerifyCHVReq);
    return ERR_NO_MORE_MEMORY;
  }
  pSimVerifyCHVReq->CHVNumber = nCHVNumber;
  SUL_MemCopy8(pSimVerifyCHVReq->CHVValue, pCHVValue, 8);
  CSW_PROFILE_FUNCTION_EXIT(SimVerifyCHVReq);
#ifdef CFW_MULTI_SIM
  return CFW_SendSclMessage(API_SIM_VERIFYCHV_REQ, pSimVerifyCHVReq, nSimID);
#else
  return CFW_SendSclMessage(API_SIM_VERIFYCHV_REQ, pSimVerifyCHVReq);
#endif
}

#ifdef CFW_MULTI_SIM
UINT32 SimChangeCHVReq(UINT8* pOldCHVValue, UINT8* pNewCHVValue, UINT8 nCHVNumber, CFW_SIM_ID nSimID)
#else
UINT32 SimChangeCHVReq(UINT8* pOldCHVValue, UINT8* pNewCHVValue, UINT8 nCHVNumber)
#endif
{
  api_SimChangeCHVReq_t* pSimChangeCHVReq = NULL;
  SIM_CHV_PARAM* pG_Chv_Param = NULL;

  CSW_TRACE(CFW_SIM_TS_ID, TSTXT("ChangeCHVReq nCHVNumber[%d]\n"), nCHVNumber);
  CSW_TC_MEMBLOCK(CFW_API_TS_ID, (UINT8*)pOldCHVValue, 8, 16);
  CSW_TC_MEMBLOCK(CFW_API_TS_ID, (UINT8*)pNewCHVValue, 8, 16);

  CSW_PROFILE_FUNCTION_ENTER(SimChangeCHVReq);
#ifdef CFW_MULTI_SIM
  CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
#else
  CFW_CfgSimGetChvParam(&pG_Chv_Param);
#endif
  pG_Chv_Param->nFunID = SIM_CHANGECHV;

  pSimChangeCHVReq = (api_SimChangeCHVReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SimChangeCHVReq_t));
  if (pSimChangeCHVReq == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(SimChangeCHVReq);
    return ERR_NO_MORE_MEMORY;
  }
  pSimChangeCHVReq->CHVNumber = nCHVNumber;
  SUL_MemCopy8(pSimChangeCHVReq->OldCHVValue, pOldCHVValue, 8);
  SUL_MemCopy8(pSimChangeCHVReq->NewCHVValue, pNewCHVValue, 8);
  CSW_PROFILE_FUNCTION_EXIT(SimChangeCHVReq);
#ifdef CFW_MULTI_SIM
  return CFW_SendSclMessage(API_SIM_CHANGECHV_REQ, pSimChangeCHVReq, nSimID);
#else
  return CFW_SendSclMessage(API_SIM_CHANGECHV_REQ, pSimChangeCHVReq);
#endif
}

#ifdef CFW_MULTI_SIM
UINT32 SimDisableCHVReq(UINT8* pCHVValue, CFW_SIM_ID nSimID)
#else
UINT32 SimDisableCHVReq(UINT8* pCHVValue)
#endif
{
  api_SimDisableCHVReq_t* pSimDisableCHVReq = NULL;
  SIM_CHV_PARAM* pG_Chv_Param = NULL;

  CSW_TC_MEMBLOCK(CFW_API_TS_ID, (UINT8*)pCHVValue, 8, 16);

  CSW_PROFILE_FUNCTION_ENTER(SimDisableCHVReq);

#ifdef CFW_MULTI_SIM
  CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
#else
  CFW_CfgSimGetChvParam(&pG_Chv_Param);
#endif
  pG_Chv_Param->nFunID = SIM_DISABLECHV;

  pSimDisableCHVReq = (api_SimDisableCHVReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SimDisableCHVReq_t));
  if (pSimDisableCHVReq == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(SimDisableCHVReq);
    return ERR_NO_MORE_MEMORY;
  }
  SUL_MemCopy8(pSimDisableCHVReq->CHVValue, pCHVValue, 8);
  CSW_PROFILE_FUNCTION_EXIT(SimDisableCHVReq);
#ifdef CFW_MULTI_SIM
  return CFW_SendSclMessage(API_SIM_DISABLECHV_REQ, pSimDisableCHVReq, nSimID);
#else
  return CFW_SendSclMessage(API_SIM_DISABLECHV_REQ, pSimDisableCHVReq);
#endif
}

#ifdef CFW_MULTI_SIM
UINT32 SimEnableCHVReq(UINT8* pCHVValue, CFW_SIM_ID nSimID)
#else
UINT32 SimEnableCHVReq(UINT8* pCHVValue)
#endif
{
  api_SimEnableCHVReq_t* pSimEnableCHVReq = NULL;
  SIM_CHV_PARAM* pG_Chv_Param = NULL;

  CSW_PROFILE_FUNCTION_ENTER(SimEnableCHVReq);

  CSW_TC_MEMBLOCK(CFW_API_TS_ID, (UINT8*)pCHVValue, 8, 16);

#ifdef CFW_MULTI_SIM
  CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
#else
  CFW_CfgSimGetChvParam(&pG_Chv_Param);
#endif
  pG_Chv_Param->nFunID = SIM_ENABLECHV;

  pSimEnableCHVReq = (api_SimEnableCHVReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SimEnableCHVReq_t));
  if (pSimEnableCHVReq == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(SimEnableCHVReq);
    return ERR_NO_MORE_MEMORY;
  }
  SUL_MemCopy8(pSimEnableCHVReq->CHVValue, pCHVValue, 8);
  CSW_PROFILE_FUNCTION_EXIT(SimEnableCHVReq);
#ifdef CFW_MULTI_SIM
  return CFW_SendSclMessage(API_SIM_ENABLECHV_REQ, pSimEnableCHVReq, nSimID);
#else
  return CFW_SendSclMessage(API_SIM_ENABLECHV_REQ, pSimEnableCHVReq);
#endif
}

#ifdef CFW_MULTI_SIM
UINT32 SimUnblockCHVReq(UINT8* pUnblockCHVValue, UINT8* pNewCHVValue, UINT8 nCHVNumber, CFW_SIM_ID nSimID)
#else
UINT32 SimUnblockCHVReq(UINT8* pUnblockCHVValue, UINT8* pNewCHVValue, UINT8 nCHVNumber)
#endif
{
  api_SimUnblockCHVReq_t* pSimUnblockCHVReq = NULL;
  SIM_CHV_PARAM* pG_Chv_Param = NULL;

  CSW_PROFILE_FUNCTION_ENTER(SimUnblockCHVReq);

  CSW_TC_MEMBLOCK(CFW_API_TS_ID, (UINT8*)pUnblockCHVValue, 8, 16);
  CSW_TC_MEMBLOCK(CFW_API_TS_ID, (UINT8*)pNewCHVValue, 8, 16);
  CSW_TRACE(CFW_SIM_TS_ID, TSTXT("UnblockCHVReq nCHVNumber[%d]\n"), nCHVNumber);

#ifdef CFW_MULTI_SIM
  CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
#else
  CFW_CfgSimGetChvParam(&pG_Chv_Param);
#endif
  pG_Chv_Param->nFunID = SIM_UNBLOCKCHV;

  pSimUnblockCHVReq = (api_SimUnblockCHVReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SimUnblockCHVReq_t));
  if (pSimUnblockCHVReq == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(SimUnblockCHVReq);
    return ERR_NO_MORE_MEMORY;
  }
  pSimUnblockCHVReq->CHVNumber = nCHVNumber;
  SUL_MemCopy8(pSimUnblockCHVReq->UnblockCHVValue, pUnblockCHVValue, 8);
  SUL_MemCopy8(pSimUnblockCHVReq->NewCHVValue, pNewCHVValue, 8);
  CSW_PROFILE_FUNCTION_EXIT(SimUnblockCHVReq);

#ifdef CFW_MULTI_SIM
  return CFW_SendSclMessage(API_SIM_UNBLOCKCHV_REQ, pSimUnblockCHVReq, nSimID);
#else
  return CFW_SendSclMessage(API_SIM_UNBLOCKCHV_REQ, pSimUnblockCHVReq);
#endif
}

#ifdef CFW_MULTI_SIM
UINT32 SimInvalidateReq(UINT8 nFile, CFW_SIM_ID nSimID)
#else
UINT32 SimInvalidateReq(UINT8 nFile)
#endif
{
  api_SimInvalidateReq_t* pSimInvalidateReq = NULL;
  SIM_CHV_PARAM* pG_Chv_Param = NULL;
  CSW_TRACE(CFW_SIM_TS_ID, TSTXT("InvalidateReq nFile[%s]\n"), TS_GetSIMFileName(nFile));

  CSW_PROFILE_FUNCTION_ENTER(SimInvalidateReq);
#ifdef CFW_MULTI_SIM
  CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
#else
  CFW_CfgSimGetChvParam(&pG_Chv_Param);
#endif

  pG_Chv_Param->nFunID = SIM_INVALIDATE;

  pSimInvalidateReq = (api_SimInvalidateReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SimInvalidateReq_t));
  if (pSimInvalidateReq == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(SimInvalidateReq);
    return ERR_NO_MORE_MEMORY;
  }
  pSimInvalidateReq->File = nFile;
  CSW_PROFILE_FUNCTION_EXIT(SimInvalidateReq);

#ifdef CFW_MULTI_SIM
  return CFW_SendSclMessage(API_SIM_INVALIDATE_REQ, pSimInvalidateReq, nSimID);
#else
  return CFW_SendSclMessage(API_SIM_INVALIDATE_REQ, pSimInvalidateReq);
#endif
}

#ifdef CFW_MULTI_SIM
UINT32 SimRehabilitateReq(UINT8 nFile, CFW_SIM_ID nSimID)
#else
UINT32 SimRehabilitateReq(UINT8 nFile)
#endif
{
  api_SimRehabilitateReq_t* SimReHabilitateReq = NULL;
  SIM_CHV_PARAM* pG_Chv_Param = NULL;
  CSW_TRACE(CFW_SIM_TS_ID, TSTXT("RehabilitateReq nFile[%s]\n"), TS_GetSIMFileName(nFile));
  CSW_PROFILE_FUNCTION_ENTER(SimRehabilitateReq);

#ifdef CFW_MULTI_SIM
  CFW_CfgSimGetChvParam(&pG_Chv_Param, nSimID);
#else
  CFW_CfgSimGetChvParam(&pG_Chv_Param);
#endif

  pG_Chv_Param->nFunID = SIM_REHABILITATE;

  SimReHabilitateReq = (api_SimRehabilitateReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SimRehabilitateReq_t));
  if (SimReHabilitateReq == NULL)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error Malloc failed\n");

    CSW_PROFILE_FUNCTION_EXIT(SimReHabilitateReq);
    return ERR_NO_MORE_MEMORY;
  }
  SimReHabilitateReq->File = nFile;
  CSW_PROFILE_FUNCTION_EXIT(SimRehabilitateReq);

#ifdef CFW_MULTI_SIM
  return CFW_SendSclMessage(API_SIM_REHABILITATE_REQ, SimReHabilitateReq, nSimID);
#else
  return CFW_SendSclMessage(API_SIM_REHABILITATE_REQ, SimReHabilitateReq);
#endif
}

VOID cfw_IMSItoASC(UINT8* InPut, UINT8* OutPut, UINT8* OutLen)
{
  UINT8 i;

  CSW_PROFILE_FUNCTION_ENTER(cfw_IMSItoASC);

  OutPut[0] = ((InPut[1] & 0xF0) >> 4) + 0x30;
  for (i = 2; i < InPut[0] + 1; i++)
  {
    OutPut[2* (i - 1) - 1] = (InPut[i] & 0x0F) + 0x30;
    OutPut[2 * (i - 1)] = ((InPut[i] & 0xF0) >> 4) + 0x30;
  }
  OutPut[2 * InPut[0] - 1] = 0x00;
  *OutLen = 2 * InPut[0] - 1;
  CSW_PROFILE_FUNCTION_EXIT(cfw_IMSItoASC);
}

VOID cfw_PLMNtoBCD(UINT8* pIn, UINT8* pOut, UINT8* nInLength)
{
  UINT8 i = 0;
  UINT8 j = 0;

  CSW_PROFILE_FUNCTION_ENTER(cfw_PLMNtoBCD);

  for (i = 0; i <* nInLength; i += 3)
  {
    if (0xFF != pIn[i])
    {
      j++;
      pOut[2 * i] = pIn[i] & 0x0F;  // MCC digit 1
      pOut[2 * i + 1] = pIn[i] >> 4;  // MCC digit 2
      pOut[2 * i + 2] = pIn[i + 1] & 0x0F;  // MNC digit 3
      pOut[2 * i + 5] = pIn[i + 1] >> 4;  // MCC digit 3
      pOut[2 * i + 3] = pIn[i + 2] & 0x0F;  // MNC digit 2
      pOut[2 * i + 4] = pIn[i + 2] >> 4;  // MNC digit 1
    }
  }
  *nInLength = j;

  CSW_PROFILE_FUNCTION_EXIT(cfw_PLMNtoBCD);
}

VOID cfw_BCDtoPLMN(UINT8* pIn, UINT8* pOut, UINT8 nInLength)
{
  UINT8 i;

  CSW_PROFILE_FUNCTION_ENTER(cfw_BCDtoPLMN);

  for (i = 0; i < nInLength / 2; i++)
  {
    if (i % 3 == 0)
    {
      pOut[i] = (UINT8)(pIn[2* i] & 0x0F)+(UINT8)((pIn[2* i + 1] & 0x0F)<<4);
    }
    else if (i % 3 == 1)
    {
      pOut[i] = (UINT8)(pIn[2* i] & 0x0F)+(UINT8)((pIn[2* i + 3] & 0x0F)<<4);
    }
    else
    {
      pOut[i] = (UINT8)(pIn[2* i - 1] & 0x0F)+(UINT8)(((pIn[2* i] & 0x0F)<<4));
    }
  }
  CSW_PROFILE_FUNCTION_EXIT(cfw_BCDtoPLMN);

}

#ifdef CFW_MULTI_SIM
CFW_SIM_ID CFW_GetSimCardID(HAO hAO)
{
  CFW_SIM_ID nSimID;

  UINT32 nRet = CFW_GetSimID(hAO, &nSimID);

  if (ERR_SUCCESS != nRet)
  {
    CSW_TRACE(CFW_SIM_TS_ID, "Error CFW_GetSimID return 0x%x \n", nRet);

    COS_SetLastError(nRet);
  }

  return nSimID;

}

#endif
/*
================================================================================
  Function   : cfw_PLMNtoBCDEX
--------------------------------------------------------------------------------
  
  Scope      : Change PLMN to BCD
  Parameters :
  Return     :
================================================================================
*/
VOID cfw_PLMNtoBCDEX(UINT8* pIn, UINT8* pOut, UINT8* nInLength)
{
  UINT8 i = 0x00;
  UINT8 j = 0x00;

  CSW_PROFILE_FUNCTION_ENTER(cfw_PLMNtoBCD);

  for (i = 0; i <* nInLength; i += 3)
  {
    j++;
    if (0xFF != pIn[i])
    {
      pOut[2 * i] = pIn[i] & 0x0F;  // MCC digit 1
      pOut[2 * i + 1] = pIn[i] >> 4;  // MCC digit 2
      pOut[2 * i + 2] = pIn[i + 1] & 0x0F;  // MNC digit 3
      pOut[2 * i + 5] = pIn[i + 1] >> 4;  // MCC digit 3
      pOut[2 * i + 3] = pIn[i + 2] & 0x0F;  // MNC digit 2
      pOut[2 * i + 4] = pIn[i + 2] >> 4;  // MNC digit 1
    }
    else
    {
      pOut[2 * i] = 0xff; // MCC digit 1
      pOut[2 * i + 1] = 0xff; // MCC digit 2
      pOut[2 * i + 2] = 0xff; // MNC digit 3
      pOut[2 * i + 5] = 0xff; // MCC digit 3
      pOut[2 * i + 3] = 0xff; // MNC digit 2
      pOut[2 * i + 4] = 0xff; // MNC digit 1
    }
  }
  *nInLength = j;

  CSW_PROFILE_FUNCTION_EXIT(cfw_PLMNtoBCD);
}
 
