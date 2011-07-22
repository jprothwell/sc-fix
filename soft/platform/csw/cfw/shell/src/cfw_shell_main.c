////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2009, Coolsand Technologies, Inc.
//                       All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmission of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: cfw_shell_main.c
// 
// DESCRIPTION:
// TODO: ...
// 
// REVISION HISTORY:
//    NAME              DATE                REMAKS
// Lixp                2009-3-19       Created initial version 1.0
//
////////////////////////////////////////////////////////////////////////////////
 
 
#include <base.h>
#include <cfw.h>
#include <cos.h>
#include <stdkey.h>
#include <sul.h>
#include "api_msg.h"
#include <ts.h>
#include <base_prv.h>
#include <cfw_prv.h>
#include <event_prv.h>
#include <shell.h>
#include "csw_debug.h"
#include "cfw_config_prv.h"
#include <cfw_sim_prv.h>
#include <stack_map.h>
#include "chip_id.h"
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
#  include <target.h>
#else
#  include "tgt_m.h"
#endif

// Include mandatorily that one after tgt_m.h (if inclusion needed)
// as the csw_csp.h change the meaning of the factory setting fields
// by expanding them with mobile_id., for example.
#include "csw_csp.h"

#ifdef CFW_MULTI_SIM
UINT32 CFW_SimPatch(UINT16 nUTI, CFW_SIM_ID nSimID);
#else
UINT32 CFW_SimPatch(UINT16 nUTI);
#endif


#ifdef CFW_MULTI_SIM

PRIVATE UINT8 g_nStatus[CFW_SIM_COUNT]  = { SHELL_STATE_WAIT,  };
BOOL g_NW_IND[CFW_SIM_COUNT]            = { FALSE,  };
BOOL g_IMSI_INIT[CFW_SIM_COUNT]            = { FALSE,  };
BOOL g_CFW_PowerOn          = FALSE;
BOOL g_SAT_IND[CFW_SIM_COUNT]           = { FALSE,  };
BOOL g_SAT_INIT_COMPLETE[CFW_SIM_COUNT] = { FALSE, };

UINT32 g_TestMode_AutoResponse[CFW_SIM_COUNT]   = { 0x00,  };
UINT32 g_TestMode_AutoEMCDial[CFW_SIM_COUNT]    = { 0x00,  };
UINT32 g_TestMode_AutoEMCDialNum[CFW_SIM_COUNT] = { 0x00,  };
PRIVATE UINT8 g_NW_DialEmcOK[CFW_SIM_COUNT]     = { 0x00, };
PRIVATE UINT8 g_State_No_SimCard[CFW_SIM_COUNT] = { 0x00,  };
PRIVATE UINT8 g_State_TestCard[CFW_SIM_COUNT]   = { 0x00,  };
PRIVATE CFW_SMS_PARAMETER g_Info[CFW_SIM_COUNT] = { {0,},  };
UINT32 g_SimInit1Status[CFW_SIM_COUNT]    = { 0x00,  };
UINT32 g_SimInit2Status[CFW_SIM_COUNT]    = { 0x00,  };
UINT32 g_SimInit3Status[CFW_SIM_COUNT]    = { 0x00,  };
UINT32 g_SimInit3Status_p2[CFW_SIM_COUNT] = { 0x00,  };
typedef struct _NW_STORELIST_INFO{
	CFW_StoredPlmnList p_StoreList[CFW_SIM_COUNT];
}CFW_STORELIST_INFO;
#else

PRIVATE UINT8 g_nStatus  = SHELL_STATE_WAIT;
BOOL g_NW_IND            = FALSE;
BOOL g_CFW_PowerOn       = FALSE;
BOOL g_SAT_IND           = FALSE;
BOOL g_SAT_INIT_COMPLETE = FALSE;

UINT32 g_TestMode_AutoResponse   = 0x00;
UINT32 g_TestMode_AutoEMCDial    = 0x00;
UINT32 g_TestMode_AutoEMCDialNum = 0x00;
PRIVATE UINT8 g_NW_DialEmcOK     = 0x00;
PRIVATE UINT8 g_State_No_SimCard = 0x00;
PRIVATE UINT8 g_State_TestCard   = 0x00;
PRIVATE CFW_SMS_PARAMETER g_Info = { 0, };

UINT32 g_SimInit1Status    = 0;
UINT32 g_SimInit2Status    = 0;
UINT32 g_SimInit3Status    = 0;
UINT32 g_SimInit3Status_p2 = 0;

#endif


#ifdef CFW_MULTI_SIM
UINT32 CFW_SimGetSmsTotalNum(UINT16 nUTI, CFW_SIM_ID nSimID);
#else
UINT32 CFW_SimGetSmsTotalNum(UINT16 nUTI);
#endif
#ifdef CFW_MULTI_SIM
static UINT8 nSimOpenStatus[CFW_SIM_COUNT] = {SOS_NO_SIM,};
#else
static UINT8 nSimOpenStatus = 0x00;
#endif

#ifdef CFW_MULTI_SIM
UINT32 CFW_GetSimOpenStatus(CFW_SIM_ID nSimID)
{
   return nSimOpenStatus[nSimID];
}
#else
UINT32 CFW_GetSimOpenStatus()
{
  return nSimOpenStatus;
}
#endif

#ifdef CFW_MULTI_SIM
VOID CFW_SetSimOpenStatus(UINT8 nStatus,CFW_SIM_ID nSimID)
{
  nSimOpenStatus[nSimID] = nStatus;
}
#else
VOID CFW_SetSimOpenStatus(UINT8 nStatus)
{
  nSimOpenStatus = nStatus;
}
#endif


// 
// Setup all CFW necessary Callback function as initialization step.
// This functions should be called by system or other task if you want to used CFW TASK.
// 
UINT32 BAL_CfwSetupCbs(CFW_PFN_KL_CBS* pKlFun, CFW_PFN_AUD_CBS* pHwFun)
{
  CSW_PROFILE_FUNCTION_ENTER(BAL_CfwSetupCbs);
  CFW_BalSetupHwCbs(pHwFun);
  CSW_PROFILE_FUNCTION_EXIT(BAL_CfwSetupCbs);
  return ERR_SUCCESS;
}

// 
// CFW command control.
// 
UINT32 CFW_ShellControl(UINT8 nCommand)
{
  CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("Entry CFW_ShellControl\r\n"));
  CSW_PROFILE_FUNCTION_ENTER(CFW_ShellControl);
  if (nCommand == CFW_CONTROL_CMD_POWER_ON)
  {
    if (!g_CFW_PowerOn)
    {
      CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("CFW_CONTROL_CMD_POWER_ON\r\n"));
      
      g_CFW_PowerOn = TRUE;

      // 
      // Instatll some callback functios to CSW-CFW when CFW task initialize.
      // You can Setupt this CBS in Usr-Task when initialize.
      // 
      CFW_PFN_AUD_CBS pfnAudCbs = { NULL, NULL };

      CFW_ServicesExit();

      CFW_BalSetupHwCbs(&pfnAudCbs);

      // 
      // Start Initialize CSW ATCOM module really.
      // 
#ifdef CFW_MULTI_SIM

      CFW_BalSendMessage(11, EV_CFW_INIT_REQ, NULL, 0, CFW_SIM_END);

#else
      CFW_BalSendMessage(11, EV_CFW_INIT_REQ, NULL, 0);
#endif

    }
    else
    {
      CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("Error Call CFW_CONTROL_CMD_POWER_ON More than one time\r\n"));
      CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("Error return ERR_CFW_REPEAT_TRANSFER\r\n"));

      return ERR_CFW_REPEAT_TRANSFER;
    }

  }
  else if (nCommand == CFW_CONTROL_CMD_POWER_OFF)
  {
    if (g_CFW_PowerOn)
    {
      CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("CFW_CONTROL_CMD_POWER_OFF\r\n"));
      g_CFW_PowerOn = FALSE;
      CFW_Exit();
    }
  }
  else if (nCommand == CFW_CONTROL_CMD_RESTART)
  {
      CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("CFW_CONTROL_CMD_RESTART Not Support\r\n"));
      return ERR_NOT_SUPPORT;
  }
  else if (nCommand == CFW_CONTROL_CMD_RESET)
  {
      CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("CFW_CONTROL_CMD_RESET Not Support\r\n"));
      return ERR_NOT_SUPPORT;
  }

  // 
  // Add at 20060914
  // 
  else
  {
    CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("CFW_ShellControl Input Invalid Parameter %x\r\n"),nCommand);
    
    return ERR_INVALID_PARAMETER;
  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_ShellControl);

  return ERR_SUCCESS;
}

// #define CFW_SIM_CARD_REMOVED   (SIM_BASE+1)  //No sim card is inserted.That just only is sent after API_SIM_RESET_CNF received.
#ifdef __221_UNIT_TEST__
UINT32 CFW_SimInit(CFW_SIM_ID nSimID);
#endif


// 
// CFW Shell Proc entry.
// 
UINT32 CFW_ShellProc(CFW_EV* pEvent)
	{
	UINT32 ret     = ERR_SUCCESS;
	UINT32 nEvId   = 0;
	UINT32 nParam1 = 0;
	UINT32 nParam2 = 0;
	UINT8 nType    = 0;
	UINT8 nTUTI    = 0;

	CSW_PROFILE_FUNCTION_ENTER(CFW_ShellProc);
	nEvId   = pEvent->nEventId;
	nParam1 = pEvent->nParam1;
	nParam2 = pEvent->nParam2;
	nType   = pEvent->nType;
	nTUTI   = pEvent->nTransId;
#ifdef CFW_MULTI_SIM

	CFW_SIM_ID nSimID;

	nSimID = pEvent->nFlag;

	CSW_TRACE(CFW_SHELL_TS_ID, "CFW_ShellProc [nEvId]0x%x,[nParam1]0x%x [nSimID]%d,[nType]0%x\n",nEvId,nParam1,nSimID,nType);
#endif
	if (nEvId == EV_CFW_SIM_STATUS_IND) // EV_CFW_SRV_STATUS_IND 
	{
		if (nParam1 == CFW_SIM_CARD_INSERTED) // sim init phase_1 
		{
			CSW_TRACE(CFW_SHELL_TS_ID, "Find SimCard\n");

			// g_SimInit1Status = CFW_INIT_SIM_INSERTED;
			// CFW_PostNotifyEvent(EV_CFW_INIT_IND, CFW_INIT_SIM_INSERTED, nParam2, 0, CFW_INIT_SMS_STATUS);
		}
		else if (nParam1 == CFW_SIM_CARD_REMOVED)  // sim init phase_1 
		{
			DM_KeyboardInit();
			CSW_TRACE(CFW_SHELL_TS_ID, "NO SimCard\n");
#ifdef CFW_MULTI_SIM
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, 0, 0, 0 | (nSimID << 24), CFW_INIT_STATUS_NO_SIM);
			g_SimInit1Status[nSimID] = 0;
			g_SimInit2Status[nSimID] = 0;
			g_SimInit3Status[nSimID] = 0;
			if (0x01 == CFW_GetAutoCallCmd(nSimID))
			{
				PM_ProLPMDisable();
			}
#else
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, 0, 0, 0, CFW_INIT_STATUS_NO_SIM);
			g_SimInit1Status = 0;
			g_SimInit2Status = 0;
			g_SimInit3Status = 0;
			if (0x01 == CFW_GetAutoCallCmd())
			{
				PM_ProLPMDisable();
			}
#endif

#ifdef CFW_MULTI_SIM
			if ((0x01 == CFW_GetAutoCallCmd(nSimID)) && g_NW_DialEmcOK[nSimID])
			{
			  CSW_TRACE(CFW_SHELL_TS_ID, "Auto Dail\n");
				CFW_PostNotifyEvent(EV_CFW_INIT_IND, 0x00, 0, 0 | (nSimID << 24), CFW_INIT_STATUS_AUTODAIL);
			}

#else
			if ((0x01 == CFW_GetAutoCallCmd()) && g_NW_DialEmcOK)
			{
			  CSW_TRACE(CFW_SHELL_TS_ID, "Auto Dail\n");
				CFW_PostNotifyEvent(EV_CFW_INIT_IND, 0x00, 0, 0, CFW_INIT_STATUS_AUTODAIL);
			}

#endif
#ifdef CFW_MULTI_SIM

			else if ((!g_NW_DialEmcOK[nSimID]))
#else
			else if ((!g_NW_DialEmcOK))
#endif
			{
#ifdef CFW_MULTI_SIM
				g_State_No_SimCard[nSimID] = 1;
#else
				g_State_No_SimCard = 1;
#endif

			}
			else
			{
				;
			}
		}
		else if (nParam1 == ERR_NO_MORE_MEMORY)
		{
#ifdef CFW_MULTI_SIM
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, 0, 0, 0 | (nSimID << 24), CFW_INIT_STATUS_NO_MEMORY);
#else
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, 0, 0, 0, CFW_INIT_STATUS_NO_MEMORY);
#endif
			CSW_TRACE(CFW_SHELL_TS_ID, "Error No More Memory\n");
		}
		else if (nParam1 == CFW_SIM_CARD_PIN1BLOCK) // sim init phase_1. sim card is block(pin1,or puk1?).
		{
#ifdef CFW_MULTI_SIM
			g_SimInit1Status[nSimID] |= CFW_INIT_SIM_CARD_BLOCK;
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit1Status[nSimID], nParam2, 0 | (nSimID << 24), CFW_INIT_STATUS_SIM);
			g_SimInit1Status[nSimID] = 0;
			CSW_TRACE(CFW_SHELL_TS_ID, "PIN1 Block\n");

#else
			g_SimInit1Status |= CFW_INIT_SIM_CARD_BLOCK;
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit1Status, nParam2, 0, CFW_INIT_STATUS_SIM);
			g_SimInit1Status = 0;
			CSW_TRACE(CFW_SHELL_TS_ID, "PIN1 Block\n");
#endif
		}
		else if (nParam1 == CFW_SIM_CARD_PUK1)  // sim init phase_1, need to input puk1
		{
			DM_KeyboardInit();
			CSW_TRACE(CFW_SHELL_TS_ID, "Need Input PUK1\n");
#ifdef CFW_MULTI_SIM
			g_SimInit1Status[nSimID] |= CFW_INIT_SIM_WAIT_PUK1;
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit1Status[nSimID], nParam2, 0 | (nSimID << 24), CFW_INIT_STATUS_SIM);
#else
			g_SimInit1Status |= CFW_INIT_SIM_WAIT_PUK1;
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit1Status, nParam2, 0, CFW_INIT_STATUS_SIM);
#endif
			// there should be send a message to MMI
			// CFW_SimEnterAuthentication("1234",4,"74074155",8,1,APP_UTI_SHELL);
		}
		else if (nParam1 == CFW_SIM_CARD_PIN1)  // sim init phase_1, need to input pin1
		{
			DM_KeyboardInit();
			CSW_TRACE(CFW_SHELL_TS_ID, "Need Input PIN1\n");
#ifdef CFW_MULTI_SIM
			g_SimInit1Status[nSimID] |= CFW_INIT_SIM_WAIT_PIN1;
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit1Status[nSimID], nParam2, 0 | (nSimID << 24), CFW_INIT_STATUS_SIM);

#else
			g_SimInit1Status |= CFW_INIT_SIM_WAIT_PIN1;
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit1Status, nParam2, 0, CFW_INIT_STATUS_SIM);
#endif
			// there should be send a message to MMI
			// CFW_SimEnterAuthentication("1234",4,NULL,0,1,APP_UTI_SHELL);
		}
		else if (nParam1 == CFW_SIM_CARD_PHASE) // sim init phase_1, pin1 ready 
		{
 #ifdef CFW_MULTI_SIM
			if (SAT_SUPPORT && (TRUE == CFW_SimInitStage2(nSimID)))
#else
			if (SAT_SUPPORT && (TRUE == CFW_SimInitStage2()))
#endif
			{
				CSW_TRACE(CFW_SHELL_TS_ID, "ProfileDownload OK\n");
#ifdef CFW_MULTI_SIM
				if (ERR_SUCCESS != (ret = CFW_SatActivation(0, APP_UTI_SHELL, nSimID)))
#else
				if (ERR_SUCCESS != (ret = CFW_SatActivation(0, APP_UTI_SHELL)))
#endif
				{
					CSW_TRACE(CFW_SHELL_TS_ID, "Error CFW_SatActivation failed,return 0x%x\n", ret);
#ifdef CFW_MULTI_SIM
					if (ERR_SUCCESS != (ret = CFW_SimInitStage3(nSimID)))
#else
					if (ERR_SUCCESS != (ret = CFW_SimInitStage3()))
#endif
					{
						CSW_TRACE(CFW_SHELL_TS_ID, "Error CFW_SimInitStage3 failed,return 0x%x\n", ret);
					}
				}
			}     
      else
      {
#ifdef CFW_MULTI_SIM
				if (ERR_SUCCESS != (ret = CFW_SimInitStage3(nSimID)))
#else
				if (ERR_SUCCESS != (ret = CFW_SimInitStage3()))
#endif
				{
					CSW_TRACE(CFW_SHELL_TS_ID, "Error CFW_SimInitStage3 failed,return 0x%x\n", ret);
				}
      }
		}
		else if (nParam1 == CFW_SIM_CARD_PIN1DISABLE) // sim init phase_1, pin1 ready 
		{
			CSW_TRACE(CFW_SHELL_TS_ID, "PIN1 Ready\n");
      
#ifdef CFW_MULTI_SIM
			g_SimInit1Status[nSimID] |= CFW_INIT_SIM_PIN1_READY;
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit1Status[nSimID], nParam2, 0 | (nSimID << 24), CFW_INIT_STATUS_SIM);

#else
			g_SimInit1Status |= CFW_INIT_SIM_PIN1_READY;
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit1Status, nParam2, 0, CFW_INIT_STATUS_SIM);
#endif
#ifdef CFW_MULTI_SIM
			if ( ERR_SUCCESS == CFW_SimInitStage1(nSimID))
#else
			if ( ERR_SUCCESS == CFW_SimInitStage1())
#endif
			{
				CSW_TRACE(CFW_SHELL_TS_ID, "CFW_SimInitStage1 OK\n");
				
			}
			else
			{
#ifdef CFW_MULTI_SIM
				CSW_TRACE(CFW_SHELL_TS_ID, "CFW_SimInitStage1 Error\n");

				if (ERR_SUCCESS != (ret = CFW_SimInitStage3(nSimID)))
#else
				if (ERR_SUCCESS != (ret = CFW_SimInitStage3()))

#endif
				{
					CSW_TRACE(CFW_SHELL_TS_ID, "Error CFW_SimInitStage3 failed,return 0x%x\n", ret);
				}
			}
		}
		else if (nParam1 == CFW_SIM_CARD_BAD) // sim init phase_3
		{
#ifdef CFW_MULTI_SIM
			g_SimInit3Status[nSimID] |= CFW_INIT_SIM_CARD_BAD;
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit3Status[nSimID], 0, 0 | (nSimID << 24), CFW_INIT_STATUS_SIMCARD);

#else
			g_SimInit3Status |= CFW_INIT_SIM_CARD_BAD;
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit3Status, 0, 0, CFW_INIT_STATUS_SIMCARD);
#endif
			CSW_TRACE(CFW_SHELL_TS_ID, "Bad SimCard\n");

		}
		else if (nParam1 == CFW_SIM_CARD_ABNORMITY)
		{
#ifdef CFW_MULTI_SIM
			g_SimInit3Status[nSimID] |= CFW_INIT_SIM_CARD_ABNORMITY;
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit3Status[nSimID], 0, 0 | (nSimID << 24), CFW_INIT_STATUS_SIMCARD);

#else
			g_SimInit3Status |= CFW_INIT_SIM_CARD_ABNORMITY;
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit3Status, 0, 0, CFW_INIT_STATUS_SIMCARD);
#endif
			CSW_TRACE(CFW_SHELL_TS_ID, "Abnormity Sim Card\n");

		}
		else if (nParam1 == CFW_SIM_CARD_TEST)  // sim init phase_3
		{
#ifdef CFW_MULTI_SIM
      CFW_STORELIST_INFO *pStoreList = CFW_MemAllocMessageStructure(sizeof(CFW_STORELIST_INFO));

      CFW_SIM_ID s = CFW_SIM_0;

      for( ; s < CFW_SIM_COUNT; s++ )
      {
        CFW_CfgGetStoredPlmnList(&pStoreList->p_StoreList[s], s);
      }
#endif
			CSW_TRACE(CFW_SHELL_TS_ID, "Sim Card Using for test\n");
      
#ifdef CFW_MULTI_SIM
			g_SimInit3Status[nSimID] |= CFW_INIT_SIM_TEST_CARD;
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit3Status[nSimID], nParam2, 0 | (nSimID << 24),
			  CFW_INIT_STATUS_SIMCARD);
			if (ERR_SUCCESS != (ret = CFW_SendSclMessage(API_SIM_OPEN_IND, pStoreList, nSimID)))
			{
				return (ret | BAL_PROC_DONT_FREE_MEM | BAL_PROC_COMPLETE);
			}
			g_State_TestCard[nSimID] = 0x01;
			g_nStatus[nSimID]        = SHELL_STATE_WAIT_NW_WITHSIM;
			if (0x02 == CFW_GetAutoCallCmd(nSimID))
#else
			g_SimInit3Status |= CFW_INIT_SIM_TEST_CARD;
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit3Status, nParam2, 0, CFW_INIT_STATUS_SIMCARD);
			if (ERR_SUCCESS != (ret = CFW_SendSclMessage(API_SIM_OPEN_IND, NULL)))
			{
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit3Status, ret, 0, CFW_INIT_STATUS_SIMCARD);
			return (ret | BAL_PROC_DONT_FREE_MEM | BAL_PROC_COMPLETE);
			}
			g_State_TestCard = 0x01;
			g_nStatus        = SHELL_STATE_WAIT_NW_WITHSIM;
			if (0x02 == CFW_GetAutoCallCmd())
#endif
			{
				PM_ProLPMDisable();
			}
      g_nStatus[nSimID] = SHELL_STATE_SIM_IMSI;

		}
		else if (nParam1 == CFW_SIM_CARD_NORMAL)  // sim init phase_3,  sent by Security APIs. last event of SIM init.
		{
#ifdef CFW_MULTI_SIM
			// Param2.b0 == 1:AND;Param2b0 == 0:FDN
			// Param2.b1= 1:BDN enable;Param2.b1=0:BDN disable.
			g_SimInit3Status[nSimID] &= ~CFW_INIT_SIM_WAIT_PS;
			g_SimInit3Status[nSimID] &= ~CFW_INIT_SIM_WAIT_PF;
			g_SimInit3Status[nSimID] |= CFW_INIT_SIM_NORMAL_CARD;
			g_SimInit3Status_p2[nSimID] = nParam2;
 
			CSW_TRACE(CFW_SHELL_TS_ID, "Nomal Sim Card\n");
      if(0x01 == (nParam2 & 0x01))
      {
			  CSW_TRACE(CFW_SHELL_TS_ID, "ADN mode\n");
      }
      else
      {
			  CSW_TRACE(CFW_SHELL_TS_ID, "FDN mode\n");
      }
      
      if(0x01 == ((nParam2 & 0x02) >> 1))
      {
			  CSW_TRACE(CFW_SHELL_TS_ID, "BDN Enable\n");
      }
      else
      {
			  CSW_TRACE(CFW_SHELL_TS_ID, "BDN Disable\n");
      }
      
			// Add by lixp at 20080703
			CFW_COMM_MODE nFM;

			UINT32 nRet = CFW_GetComm(&nFM, nSimID);
			if( ERR_SUCCESS != nRet )
			{
			  CSW_TRACE(CFW_SHELL_TS_ID, "Error CFW_GetComm failed return %x\n",nRet);
        
				nFM = CFW_ENABLE_COMM;
			}

			if (  CFW_ENABLE_COMM== nFM )
			{
        CFW_STORELIST_INFO *pStoreList = CFW_MemAllocMessageStructure(sizeof(CFW_STORELIST_INFO));
        CFW_SIM_ID s = CFW_SIM_0;

        for( ; s < CFW_SIM_COUNT; s++ )
        {
          CFW_CfgGetStoredPlmnList(&pStoreList->p_StoreList[s], s);
        }
				CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("Enable communciation nSimID[%d]\n"),nSimID);
				if (ERR_SUCCESS != (ret = CFW_SendSclMessage(API_SIM_OPEN_IND, pStoreList, nSimID)))
				{
					CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit3Status[nSimID], ret, 0 | (nSimID << 24),
					      CFW_INIT_STATUS_SIMCARD);
				}
				g_nStatus[nSimID] = SHELL_STATE_WAIT_NW_WITHSIM;
			}
			else if (( CFW_DISABLE_COMM == nFM ) || ( CFW_CHECK_COMM == nFM ))//
			{
				g_nStatus[nSimID] = SHELL_STATE_SIM_IMSI;
        if( CFW_DISABLE_COMM == nFM )
        {
				  CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("Disable communciation\n"));
        }
        else
        {
				  CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("Check status for communciation\n"));
        }
			}
			else
			{
				CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("Error Get communciation status,CommMode %d,nSimID %d\n"), nFM, nSimID);
			}			
      g_nStatus[nSimID] = SHELL_STATE_SIM_IMSI;
#else
			// Param2.b0 == 1:AND;Param2b0 == 0:FDN
			// Param2.b1= 1:BDN enable;Param2.b1=0:BDN disable.
			g_SimInit3Status &= ~CFW_INIT_SIM_WAIT_PS;
			g_SimInit3Status &= ~CFW_INIT_SIM_WAIT_PF;
			g_SimInit3Status |= CFW_INIT_SIM_NORMAL_CARD;
			g_SimInit3Status_p2 = nParam2;

			// Add by lixp at 20080703
			UINT8 bFM;

			CFW_GetFlightMode(&bFM);

			// bFM : 0 : nomal mode; 1: flight mode; 2: query mode
			if (0 == bFM)
			{
				CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("FlightMode Disable\n"));
				if (ERR_SUCCESS != (ret = CFW_SendSclMessage(API_SIM_OPEN_IND, NULL)))
				{
					CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit3Status, ret, 0, CFW_INIT_STATUS_SIMCARD);
				}
				g_nStatus = SHELL_STATE_WAIT_NW_WITHSIM;
			}
			else if ((1 == bFM) || (2 == bFM))
			{
				g_nStatus = SHELL_STATE_SIM_IMSI;
				CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("FlightMode Enable\n"));
			}
			else
			{
				CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("Error Get FlightMode status,CommMode %d\n"), bFM);
			}
      g_nStatus = SHELL_STATE_SIM_IMSI;
#endif

		}
		else if (nParam1 == CFW_SIM_ME_PS)
		{
#ifdef CFW_MULTI_SIM
			g_SimInit3Status[nSimID] |= CFW_INIT_SIM_WAIT_PS;
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit3Status[nSimID], 0, 0 | (nSimID << 24), CFW_INIT_STATUS_SIMCARD);

#else
			g_SimInit3Status |= CFW_INIT_SIM_WAIT_PS;
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit3Status, 0, 0, CFW_INIT_STATUS_SIMCARD);
#endif
			// Call Phone lock verify procedure
			// CFW_SimInitStage3();
		}
		else if (nParam1 == CFW_SIM_ME_PF)
		{
#ifdef CFW_MULTI_SIM
			g_SimInit3Status[nSimID] |= CFW_INIT_SIM_WAIT_PF;
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit3Status[nSimID], 0, 0 | (nSimID << 24), CFW_INIT_STATUS_SIMCARD);

#else
			g_SimInit3Status |= CFW_INIT_SIM_WAIT_PF;
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit3Status, 0, 0, CFW_INIT_STATUS_SIMCARD);
#endif
			// Call Phone lock verify procedure
			// CFW_SimInitStage3();
		}
	}
	else if (SAT_SUPPORT && (nEvId == EV_CFW_SAT_ACTIVATION_RSP))
	{
		if (nParam1 == 0)
		{
			CSW_TRACE(CFW_SHELL_TS_ID, "Get EV_CFW_SAT_ACTIVATION_RSP, Wait EV_CFW_SAT_CMDTYPE_IND message\n");

			// Wait EV_CFW_SAT_CMDTYPE_IND message
		}
		else
		{
			if (nParam1 == ERR_CME_SIM_PROFILE_ERROR) // when intit sat, some card may return 0x90, in this case there is a fetchind msg later
			{
#ifdef CFW_MULTI_SIM
				g_SAT_IND[nSimID] = TRUE;

#else
				g_SAT_IND = TRUE;
#endif
				CSW_TRACE(CFW_SHELL_TS_ID, "Without Fetch Data\n");
			}
			else
			{
#ifdef CFW_MULTI_SIM
				g_SimInit2Status[nSimID] &= ~CFW_INIT_SIM_SAT;  // call CFW_SatGetInformation to get sap info.
				CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit2Status[nSimID], 0, 0 | (nSimID << 24), CFW_INIT_STATUS_SAT);

#else
				g_SimInit2Status &= ~CFW_INIT_SIM_SAT;  // call CFW_SatGetInformation to get sap info.
				CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit2Status, 0, 0, CFW_INIT_STATUS_SAT);
#endif
			}
      
#ifdef CFW_MULTI_SIM
			if (ERR_SUCCESS != (ret = CFW_SimInitStage3(nSimID)))
			{
				CSW_TRACE(CFW_SHELL_TS_ID, "Error CFW_SimInitStage3 failed,return 0x%x nSimID %d\n", ret,nSimID);
			}

#else
			if (ERR_SUCCESS != (ret = CFW_SimInitStage3()))
			{
				CSW_TRACE(CFW_SHELL_TS_ID, "Error CFW_SimInitStage3 failed,return 0x%x\n", ret);
			}
#endif
		}
	}

// else if (SAT_SUPPORT && (nEvId == EV_CFW_SAT_CMDTYPE_IND))
	else if (SAT_SUPPORT && (nEvId == EV_CFW_SAT_CMDTYPE_IND))
	{
		if (pEvent->nParam1 == 0x25)  // to check whether is it a SETUP MENU command
		{
			CSW_TRACE(CFW_SHELL_TS_ID, "Get setUp menu command(Fetch Data)\n");
#ifdef CFW_MULTI_SIM
			g_SimInit2Status[nSimID] |= CFW_INIT_SIM_SAT; // call CFW_SatGetInformation to get sap info.
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit2Status[nSimID], (pEvent->nParam1), 0 | (nSimID << 24),
			CFW_INIT_STATUS_SAT);

#else
			g_SimInit2Status |= CFW_INIT_SIM_SAT; // call CFW_SatGetInformation to get sap info.
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit2Status, (pEvent->nParam1), 0, CFW_INIT_STATUS_SAT);
#endif
		}

#ifdef CFW_MULTI_SIM
	//	if (ERR_SUCCESS != (ret = CFW_SatResponse((UINT8)(pEvent->nParam1), 0, 0, 0, 0, APP_UTI_SHELL, nSimID)))

#else
	//	if (ERR_SUCCESS != (ret = CFW_SatResponse((UINT8)(pEvent->nParam1), 0, 0, 0, 0, APP_UTI_SHELL)))

#endif
		{
			CSW_TRACE(CFW_SHELL_TS_ID, "Error CFW_SatResponse failed, return 0x%x nCmdType 0x%x\n", ret,(pEvent->nParam1));
		}
#ifdef CFW_MULTI_SIM
		if (ERR_SUCCESS != (ret = CFW_SimInitStage3(nSimID)))
#else
		if (ERR_SUCCESS != (ret = CFW_SimInitStage3()))
#endif
		{
			CSW_TRACE(CFW_SHELL_TS_ID, "Error CFW_SimInitStage3 failed, return 0x%x\n", ret);
		}
	}

	else if (SAT_SUPPORT && (nEvId == EV_CFW_SAT_RESPONSE_RSP))
	{
#ifdef CFW_MULTI_SIM
		CSW_TRACE(CFW_SHELL_TS_ID, "g_SAT_IND %d, g_SAT_INIT_COMPLETE %d!\n", g_SAT_IND[nSimID],
			g_SAT_INIT_COMPLETE[nSimID]);
		if ((!g_SAT_IND[nSimID]) && (nParam1 == 0) && (nType == 0))
		{
			CSW_TRACE(CFW_SHELL_TS_ID, "SAT Init OK\n");
			g_SAT_INIT_COMPLETE[nSimID] = TRUE;
			if (ERR_SUCCESS != (ret = CFW_SimInitStage3(nSimID)))
			{
				CSW_TRACE(CFW_SHELL_TS_ID, "Error CFW_SimInitStage3 failed, return 0x%x\n", ret);
			}
			g_SAT_IND[nSimID] = TRUE;
		}
		else if (g_SAT_IND[nSimID] && g_SAT_INIT_COMPLETE[nSimID] && (nParam2 == 0x90) && (nType == 0)) // if init succeeds, to notify MMI
		{
			CSW_TRACE(CFW_SHELL_TS_ID, "SAT Init OK\n");
			g_SimInit2Status[nSimID] |= CFW_INIT_SIM_SAT; // call CFW_SatGetInformation to get sap info.
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit2Status[nSimID], (pEvent->nParam1), 0 | (nSimID << 24),
				CFW_INIT_STATUS_SAT);
		}
		else if(!g_SAT_IND[nSimID]  && !g_SAT_INIT_COMPLETE[nSimID]  && (0x00 != nType))
		{
			CSW_TRACE(CFW_SHELL_TS_ID, "g_SAT_INIT_COMPLETE***x%x\n", ret);

			g_SAT_INIT_COMPLETE[nSimID]  = TRUE;
			if (ERR_SUCCESS != (ret = CFW_SimInitStage3(nSimID)))
			{
				CSW_TRACE(CFW_SHELL_TS_ID, "CFW_SimInitStage3 failed,return 0x%x\n", ret);
			}
			g_SAT_IND[nSimID]  = TRUE;
		}
		else
		{
			// TODO
			g_SAT_INIT_COMPLETE[nSimID]  = TRUE;
			if (ERR_SUCCESS != (ret = CFW_SimInitStage3(nSimID)))
			{
				CSW_TRACE(CFW_SHELL_TS_ID, "CFW_SimInitStage3 failed,return 0x%x\n", ret);
			}
			g_SAT_IND[nSimID]  = TRUE;
			CSW_TRACE(CFW_SHELL_TS_ID, "SAT At TODO sect\n");
    }

#else
		CSW_TRACE(CFW_SHELL_TS_ID, "g_SAT_IND %d, g_SAT_INIT_COMPLETE %d!\n", g_SAT_IND,
			g_SAT_INIT_COMPLETE);
		if ((!g_SAT_IND) && (nParam1 == 0) && (nType == 0))
		{
			CSW_TRACE(CFW_SHELL_TS_ID, "SAT Init OK\n");
			g_SAT_INIT_COMPLETE = TRUE;
			if (ERR_SUCCESS != (ret = CFW_SimInitStage3()))
			{
				CSW_TRACE(CFW_SHELL_TS_ID, "CFW_SimInitStage3 failed,return 0x%x\n", ret);
			}
			g_SAT_IND = TRUE;
		}
		else if (g_SAT_IND && g_SAT_INIT_COMPLETE && (nParam2 == 0x90) && (nType == 0)) // if init succeeds, to notify MMI
		{
			CSW_TRACE(CFW_SHELL_TS_ID, "SAT Init OK\n");
			g_SimInit2Status |= CFW_INIT_SIM_SAT; // call CFW_SatGetInformation to get sap info.
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit2Status, (pEvent->nParam1), 0, CFW_INIT_STATUS_SAT);
		}
		else if(!g_SAT_IND && !g_SAT_INIT_COMPLETE && (0x00 != nType))
		{
			g_SAT_INIT_COMPLETE = TRUE;
			if (ERR_SUCCESS != (ret = CFW_SimInitStage3()))
			{
				CSW_TRACE(CFW_SHELL_TS_ID, "CFW_SimInitStage3 failed,return 0x%x\n", ret);
			}
			g_SAT_IND = TRUE;
		}
		else
		{
			CSW_TRACE(CFW_SHELL_TS_ID, "SAT At TODO sect\n");
			g_SAT_INIT_COMPLETE = TRUE;
			if (ERR_SUCCESS != (ret = CFW_SimInitStage3()))
			{
				CSW_TRACE(CFW_SHELL_TS_ID, "CFW_SimInitStage3 failed,return 0x%x\n", ret);
			}
			g_SAT_IND = TRUE;      

		}
#endif

	}

	else if (nEvId == EV_CFW_SIM_ENTER_AUTH_RSP)  // MMI input pin1 or puk1 OK --> EV_CFW_SIM_ENTER_AUTH_RSP
	{
		if (nParam1 == 0)
		{
			CSW_TRACE(CFW_SHELL_TS_ID, "Enter PIN OK\n");
      
#ifdef CFW_MULTI_SIM
			g_SimInit2Status[nSimID] &= ~CFW_INIT_SIM_WAIT_PIN1;
			g_SimInit2Status[nSimID] &= ~CFW_INIT_SIM_WAIT_PUK1;
			if (ERR_SUCCESS != CFW_SimInitStage1(nSimID))
			{
			  CSW_TRACE(CFW_SHELL_TS_ID, "CFW_SimInitStage1 Error \n");
				if (ERR_SUCCESS != (ret = CFW_SimInitStage3(nSimID)))
				{
					CSW_TRACE(CFW_SHELL_TS_ID, "Error CFW_SimInitStage3 failed, return 0x%x\n", ret);
				}
			}
#else
			g_SimInit2Status &= ~CFW_INIT_SIM_WAIT_PIN1;
			g_SimInit2Status &= ~CFW_INIT_SIM_WAIT_PUK1;
			if (SAT_SUPPORT && (ERR_SUCCESS == CFW_SimInitStage1()))
			{
				CSW_TRACE(CFW_SHELL_TS_ID, "ProfileDownload OK\n");
				if (ERR_SUCCESS != (ret = CFW_SatActivation(0, APP_UTI_SHELL)))
				{
					CSW_TRACE(CFW_SHELL_TS_ID, "CFW_SatActivation failed,return 0x%x\n", ret);
				}
			}
			else
			{
				if (ERR_SUCCESS != (ret = CFW_SimInitStage3()))
				{
					CSW_TRACE(CFW_SHELL_TS_ID, "CFW_SimInitStage3 failed,return 0x%x\n", ret);
				}
			}
#endif
		}
		else
			CSW_TRACE(CFW_SHELL_TS_ID, "Wrong PIN\n");
	}
	else if (nEvId == EV_CFW_SIM_GET_PROVIDER_ID_RSP)
	{
		if (nType == 0)
		{
			// ------------------------------------------------------------
			// Add by lixp for  special card 2007.8.16 begin
			// ------------------------------------------------------------
			UINT8 nIMSI[10];

			SUL_ZeroMemory8(nIMSI, 10);

#ifdef CFW_MULTI_SIM
			UINT32 nRet = CFW_CfgGetIMSI(nIMSI, nSimID);
#else
			UINT32 nRet = CFW_CfgGetIMSI(nIMSI);
#endif
			CSW_TRACE(CFW_SHELL_TS_ID, "CFW_CfgGetIMSI nIMSI[9] 0x%x \n", nIMSI[9]);
			if (ERR_SUCCESS != nRet)
			{
			  CSW_TRACE(CFW_SHELL_TS_ID, "Error CFW_CfgGetIMSI return 0x%x\n",nRet);
			}

			// ------------------------------------------------------------
			// Add by lixp for special card 2007.8.16 end
			// ------------------------------------------------------------

			if (nIMSI[9] == 0)
			{
#ifdef CFW_MULTI_SIM
				g_Info[nSimID].bSimChange = 0;
#else
				g_Info.bSimChange = 0;
#endif

			}
			else
			{
#ifdef CFW_MULTI_SIM
				g_Info[nSimID].bSimChange = 1;
				g_SimInit3Status[nSimID] |= CFW_INIT_SIM_CARD_CHANGED;

#else
				g_Info.bSimChange = 1;
				g_SimInit3Status |= CFW_INIT_SIM_CARD_CHANGED;

#endif

			}
      if(pEvent->nParam1)
      {
        CSW_SMS_FREE((UINT8*)(pEvent->nParam1));
        pEvent->nParam1 = (UINT32)NULL;
      }			
		}
    else
    {
			  CSW_TRACE(CFW_SHELL_TS_ID, "Error Get IMSI 0x%x\n",nParam1);
    }
#ifdef CFW_MULTI_SIM
		CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit3Status[nSimID], g_SimInit3Status_p2[nSimID], 0 | (nSimID << 24),
		                    CFW_INIT_STATUS_SIMCARD);
		g_nStatus[nSimID] = SHELL_STATE_SMS;

#else
		CFW_PostNotifyEvent(EV_CFW_INIT_IND, g_SimInit3Status, g_SimInit3Status_p2, 0, CFW_INIT_STATUS_SIMCARD);
		g_nStatus = SHELL_STATE_SMS;

#endif
	}
	else if (nEvId == EV_CFW_SIM_GET_SMS_PARAMETERS_RSP)
	{
		if (nType == 0x00)
		{
#if 1
			CFW_SIM_SMS_PARAMETERS* pSmsParam = (CFW_SIM_SMS_PARAMETERS*)pEvent->nParam1;

			// 
			// Get the SMS SCA number, 12 bytes.
			// 
#ifdef CFW_MULTI_SIM
			SUL_MemCopy8(g_Info[nSimID].nNumber, (UINT8*)pSmsParam->nSMSCInfo, SIZEOF(g_Info[nSimID].nNumber));

#else
			SUL_MemCopy8(g_Info.nNumber, (UINT8*)pSmsParam->nSMSCInfo, SIZEOF(g_Info.nNumber));
#endif

			// ------------------------------------------------------------
			// Add by lixp for Sim sca raise 2007-06-25 begin
			// ------------------------------------------------------------

			// if(g_Info.nNumber[0] && g_Info.nNumber[1] && g_Info.nNumber[2] )
#ifdef CFW_MULTI_SIM
			if (g_Info[nSimID].nNumber[0])
#else
			if (g_Info.nNumber[0])
#endif

			// ------------------------------------------------------------
			// Add by lixp for Sim sca raise 2007-06-25 end
			// ------------------------------------------------------------

			{
				// ------------------------------------------------------------
				// Add by wanghb for special sca  begin  2007.8.10
				// ------------------------------------------------------------
#ifdef CFW_MULTI_SIM
				if (0xFF == g_Info[nSimID].nNumber[0])
				{
					g_Info[nSimID].nNumber[0] = 0x00;
				}

				// ------------------------------------------------------------
				// Add by wanghb for special sca  end  2007.8.10
				// ------------------------------------------------------------ 
				CFW_CfgSetDefaultSmsParam(&g_Info[nSimID],nSimID);

				CSW_TC_MEMBLOCK(CFW_SHELL_TS_ID, (UINT8 *) g_Info[nSimID].nNumber, 12, 16);
        
#else
				if (0xFF == g_Info.nNumber[0])
				{
					g_Info.nNumber[0] = 0x00;
				}

				// ------------------------------------------------------------
				// Add by wanghb for special sca  end  2007.8.10
				// ------------------------------------------------------------ 
				CFW_CfgSetDefaultSmsParam(&g_Info);
				CSW_TC_MEMBLOCK(CFW_SHELL_TS_ID, (UINT8 *) g_Info.nNumber, 12, 16);

#endif
			}
      if(pEvent->nParam1)
      {
        CSW_SMS_FREE(((VOID*)(pEvent->nParam1)));
        pEvent->nParam1 = (UINT32)NULL;
      }
#endif
		}
    else
    {
				CSW_TRACE(CFW_SHELL_TS_ID, "Error Get Sms Parameter 0x%x",nParam1);
    }
#ifdef CFW_MULTI_SIM
  	CSW_TRACE(CFW_SHELL_TS_ID, "Start Patch");

    CFW_SimPatch(APP_UTI_SHELL, nSimID);

		//CFW_SmsMoInit(APP_UTI_SHELL, nSimID);
		//CFW_SimGetSmsTotalNum(APP_UTI_SHELL, nSimID);
		g_nStatus[nSimID] = SHELL_STATE_WAIT;
#else

    CFW_SimPatch(APP_UTI_SHELL);

		//CFW_SmsMoInit(APP_UTI_SHELL);
		//CFW_SimGetSmsTotalNum(APP_UTI_SHELL, nSimID);
		g_nStatus = SHELL_STATE_WAIT;
#endif

	}
  else if (nEvId == EV_CFW_SIM_PATCH_RSP)
  {
    CSW_TRACE(CFW_SHELL_TS_ID, "Got Patch Rsp");
#ifdef CFW_MULTI_SIM
    //CFW_SimPatch(APP_UTI_SHELL, nSimID);
    //CFW_SmsMoInit(APP_UTI_SHELL, nSimID);
    CFW_SimGetSmsTotalNum(APP_UTI_SHELL, nSimID);
		g_nStatus[nSimID] = SHELL_STATE_WAIT;
#else

    //CFW_SmsMoInit(APP_UTI_SHELL);
    CFW_SimGetSmsTotalNum(APP_UTI_SHELL);
		g_nStatus = SHELL_STATE_WAIT;
#endif
  }
  
	else if (nEvId == EV_CFW_NW_DEREGISTER_RSP)
	{
		if (nType == 0x00)
		{
#ifdef CFW_MULTI_SIM
			CFW_PostNotifyEvent(EV_CFW_EXIT_IND, CFW_EXIT_STATUS_NETWORK_DETACHMENT, 0, 0 | (nSimID << 24), 0x00);
#else
			CFW_PostNotifyEvent(EV_CFW_EXIT_IND, CFW_EXIT_STATUS_NETWORK_DETACHMENT, 0, 0, 0x00);
#endif
		}
		else
		{
		  CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("Error deregister failed 0x%x\n"),pEvent->nParam1);
      
#ifdef CFW_MULTI_SIM
			CFW_PostNotifyEvent(EV_CFW_EXIT_IND, CFW_EXIT_STATUS_NETWORK_DETACHMENT, pEvent->nParam1, 0 | (nSimID << 24),
			0xF0);
#else
			CFW_PostNotifyEvent(EV_CFW_EXIT_IND, CFW_EXIT_STATUS_NETWORK_DETACHMENT, pEvent->nParam1, 0, 0xF0);
#endif
		}
	}
  else if( nEvId == EV_CFW_SIM_GET_SMS_TOTAL_NUM_RSP)
  {
#ifdef CFW_MULTI_SIM
    CFW_SetSimOpenStatus(SOS_SENT_SIM_OPEN,nSimID);
#else
    CFW_SetSimOpenStatus(SOS_SENT_SIM_OPEN);
#endif
		if (nType == 0x00)
		{
			CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("Sim SMS GetTotalNum Success\n"));
#ifdef CFW_MULTI_SIM
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, 0, ((CFW_SMS_MAX_REC_COUNT/CFW_SIM_COUNT)<<16) | (nParam1 & 0xFFFF), 0 | (nSimID << 24) /*UTI*/, CFW_INIT_STATUS_SMS);

			g_nStatus[nSimID] = SHELL_STATE_PBK;
#else
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, 0, ((CFW_SMS_MAX_REC_COUNT)<<16) | (nParam1 & 0xFFFF), 0 /*UTI*/, CFW_INIT_STATUS_SMS);
			g_nStatus = SHELL_STATE_PBK;
#endif

		}
    else
    {
			CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("Error Sim SMS Init error\n"));
#ifdef CFW_MULTI_SIM
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, 1, 0, 0 | (nSimID << 24) /*UTI*/, CFW_INIT_STATUS_SMS);
			g_nStatus[nSimID] = SHELL_STATE_PBK;
#else
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, 1, 0, 0 /*UTI*/, CFW_INIT_STATUS_SMS);
			g_nStatus = SHELL_STATE_PBK;
#endif
		}
	}
	else if (nEvId == EV_CFW_SRV_STATUS_IND)
	{
		// SMS
		// 
		if (nType == SMS_INIT_EV_OK_TYPE)
		{
			CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("Sim SMS Init Success\n"));
#ifdef CFW_MULTI_SIM
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, 0, 0, 0 | (nSimID << 24) /*UTI*/, CFW_INIT_STATUS_SMS);

			g_nStatus[nSimID] = SHELL_STATE_PBK;
#else
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, 0, 0, 0 /*UTI*/, CFW_INIT_STATUS_SMS);
			g_nStatus = SHELL_STATE_PBK;
#endif

		}
		else if (nType == SMS_INIT_EV_ERR_TYPE)
		{

			CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("Error Sim SMS Init error\n"));
#ifdef CFW_MULTI_SIM
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, 1, 0, 0 | (nSimID << 24) /*UTI*/, CFW_INIT_STATUS_SMS);
			g_nStatus[nSimID] = SHELL_STATE_PBK;
#else
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, 1, 0, 0 /*UTI*/, CFW_INIT_STATUS_SMS);
			g_nStatus = SHELL_STATE_PBK;
#endif

		}

		// 
		// PBK 
		// 
		if (nType == PBK_INIT_EV_OK_TYPE)
		{
#ifdef CFW_MULTI_SIM
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, 0, 0, 0 | (nSimID << 24), CFW_INIT_STATUS_PBK);
			g_nStatus[nSimID] = SHELL_STATE_WAIT;
#else
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, 0, 0, 0, CFW_INIT_STATUS_PBK);
			g_nStatus = SHELL_STATE_WAIT;
#endif
		}
		else if (nType == PBK_INIT_EV_ERR_TYPE)
		{
#ifdef CFW_MULTI_SIM
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, 1, 0, 0 | (nSimID << 24), CFW_INIT_STATUS_PBK);
			g_nStatus[nSimID] = SHELL_STATE_WAIT;
#else
			CFW_PostNotifyEvent(EV_CFW_INIT_IND, 1, 0, 0, CFW_INIT_STATUS_PBK);
			g_nStatus = SHELL_STATE_WAIT;
#endif
		}
		else if (nType == PBK_EXIT_EV_OK_TYPE)
		{
		}
		else if (nType == PBK_EXIT_EV_ERR_TYPE)
		{
		}
	}
	else if (nEvId == EV_CFW_CC_INITIATE_SPEECH_CALL_RSP)
	{
		if (nType == 0xF0)
		{
			CSW_TRACE(CFW_SHELL_TS_ID,TSTXT("Call 112 failed 0x%x\r\n"), nParam1);
#ifdef CFW_MULTI_SIM
			CFW_EMCDial(nSimID);
#else
			CFW_EMCDial();
#endif

		}
		else
		{
			CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("Call 112 success\r\n"));
		}
	}
  #ifdef CFW_MULTI_SIM
	if (g_nStatus[nSimID] == SHELL_STATE_AUTO_RESP)
  #else
	if (g_nStatus == SHELL_STATE_AUTO_RESP)
  #endif
	{
		if ((nEvId == EV_CFW_CC_SPEECH_CALL_IND))
		{
			UINT32 nRet;

#ifdef CFW_MULTI_SIM
			nRet = CFW_CcAcceptSpeechCall(nSimID);
#else
			nRet = CFW_CcAcceptSpeechCall();
#endif

			if (nRet != ERR_SUCCESS)
			{
				// 
				// TO DO...
				// 
				CSW_TRACE(CFW_SHELL_TS_ID, "Error CFW_CcAcceptSpeechCall failed,return 0x%x", nRet);
			}

			// g_nStatus = SHELL_STATE_WAIT;
		}
	}

  // 
  // Add for nw init at 20060831
  // SHELL_STATE_WAIT_NW_WITHSIM
  // 
	if ((nEvId == EV_CFW_NW_REG_STATUS_IND) && (nType == 2))
	{
		DM_KeyboardInit();
#ifdef CFW_MULTI_SIM
		if (g_nStatus[nSimID] == SHELL_STATE_WAIT_NW_WITHSIM)
#else
		if (g_nStatus == SHELL_STATE_WAIT_NW_WITHSIM)
#endif
		{
			if (nParam1 == CFW_NW_STATUS_REGISTRATION_DENIED)
			{
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_INIT_IND, CFW_INIT_SIM_LIMITED_SERVER, 0, 0 | (nSimID << 24), CFW_INIT_STATUS_NET);
				CSW_TRACE(CFW_SHELL_TS_ID, "Limited Server\n");
				g_NW_DialEmcOK[nSimID] = 1;
#else
				CFW_PostNotifyEvent(EV_CFW_INIT_IND, CFW_INIT_SIM_LIMITED_SERVER, 0, 0, CFW_INIT_STATUS_NET);
				CSW_TRACE(CFW_SHELL_TS_ID, "Limited Server\n");
				g_NW_DialEmcOK = 1;
#endif
			}
			else if (nParam1 == CFW_NW_STATUS_REGISTERED_HOME)
			{
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_INIT_IND, CFW_INIT_SIM_FULL_SERVER, 0, 0 | (nSimID << 24), CFW_INIT_STATUS_NET);
				CSW_TRACE(CFW_SHELL_TS_ID, "Full Server\n");
				g_NW_DialEmcOK[nSimID] = 1;
#else
				CFW_PostNotifyEvent(EV_CFW_INIT_IND, CFW_INIT_SIM_FULL_SERVER, 0, 0, CFW_INIT_STATUS_NET);
				CSW_TRACE(CFW_SHELL_TS_ID, "Full Server\n");
				g_NW_DialEmcOK = 1;
#endif

			}
			else if (nParam1 == CFW_NW_STATUS_REGISTERED_ROAMING)
			{
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_INIT_IND, CFW_INIT_SIM_ROAMING_SERVER, 0, 0 | (nSimID << 24), CFW_INIT_STATUS_NET);
				CSW_TRACE(CFW_SHELL_TS_ID, "Roaming Server\n");
				g_NW_DialEmcOK[nSimID] = 1;
#else
				CFW_PostNotifyEvent(EV_CFW_INIT_IND, CFW_INIT_SIM_ROAMING_SERVER, 0, 0, CFW_INIT_STATUS_NET);
				CSW_TRACE(CFW_SHELL_TS_ID, "Roaming Server\n");
				g_NW_DialEmcOK = 1;
#endif

			}
			else
			{
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_INIT_IND, CFW_INIT_SIM_NO_SERVER, 0, 0 | (nSimID << 24), CFW_INIT_STATUS_NET);
				CSW_TRACE(CFW_SHELL_TS_ID, "No Server\n");
				g_NW_DialEmcOK[nSimID] = 0;
#else
				CFW_PostNotifyEvent(EV_CFW_INIT_IND, CFW_INIT_SIM_NO_SERVER, 0, 0, CFW_INIT_STATUS_NET);
				CSW_TRACE(CFW_SHELL_TS_ID, "No Server\n");
				g_NW_DialEmcOK = 0;
#endif

			}
			//CSW_TRACE(CFW_SHELL_TS_ID, "the value of is CFW_GetAutoCallCmd()  is %d\r\n", CFW_GetAutoCallCmd());

#ifdef CFW_MULTI_SIM
			if ((0x02 == CFW_GetAutoCallCmd(nSimID)) && g_NW_DialEmcOK[nSimID] && g_State_TestCard[nSimID])
			{
				g_TestMode_AutoResponse[nSimID] = 1;
				g_nStatus[nSimID]               = SHELL_STATE_AUTO_RESP;
			}
			else if ((0x02 == CFW_GetAutoCallCmd(nSimID)) && (!g_NW_DialEmcOK[nSimID]) && g_State_TestCard[nSimID])
			{
				g_nStatus[nSimID] = SHELL_STATE_WAIT_NW_WITHSIM;
			}
			else
			{
				//CSW_TRACE(CFW_SHELL_TS_ID, "No Server\n");

				if ((!g_NW_IND[nSimID]) && (g_SimInit3Status[nSimID] & CFW_INIT_SIM_NORMAL_CARD))
				{
					//g_nStatus[nSimID] = SHELL_STATE_SIM_IMSI;
					g_NW_IND[nSimID]  = TRUE;
				}
			}
#else
			if ((0x02 == CFW_GetAutoCallCmd()) && g_NW_DialEmcOK && g_State_TestCard)
			{
				g_TestMode_AutoResponse = 1;
				g_nStatus               = SHELL_STATE_AUTO_RESP;
			}
			else if ((0x02 == CFW_GetAutoCallCmd()) && (!g_NW_DialEmcOK) && g_State_TestCard)
			{
				g_nStatus = SHELL_STATE_WAIT_NW_WITHSIM;
			}
			else
			{
				if (!g_NW_IND)
				{
					//g_nStatus = SHELL_STATE_SIM_IMSI;
					g_NW_IND  = TRUE;
				}
			}
#endif

		}
		else
		{
			if (nParam1 == CFW_NW_STATUS_REGISTRATION_DENIED)
			{
				CSW_TRACE(CFW_SHELL_TS_ID, "Limited Server\n");
#ifdef CFW_MULTI_SIM
				g_NW_DialEmcOK[nSimID] = 1;
#else
				g_NW_DialEmcOK = 1;
#endif

			}
			else if (nParam1 == CFW_NW_STATUS_REGISTERED_HOME)
			{
#ifdef CFW_MULTI_SIM
				CSW_TRACE(CFW_SHELL_TS_ID, "Full Server\n");
				g_NW_DialEmcOK[nSimID] = 1;
#else
				CSW_TRACE(CFW_SHELL_TS_ID, "Full Server\n");
				g_NW_DialEmcOK = 1;
#endif

			}
			else if (nParam1 == CFW_NW_STATUS_REGISTERED_ROAMING)
			{
#ifdef CFW_MULTI_SIM
				CSW_TRACE(CFW_SHELL_TS_ID, "Roaming Server\n");
				g_NW_DialEmcOK[nSimID] = 1;
#else
				CSW_TRACE(CFW_SHELL_TS_ID, "Roaming Server\n");
				g_NW_DialEmcOK = 1;
#endif

			}
			else
			{
#ifdef CFW_MULTI_SIM
				CSW_TRACE(CFW_SHELL_TS_ID, "No Server\n");
				g_NW_DialEmcOK[nSimID] = 0;
#else
				CSW_TRACE(CFW_SHELL_TS_ID, "No Server\n");
				g_NW_DialEmcOK = 0;
#endif

			}

#ifdef CFW_MULTI_SIM
			if ((0x01 == CFW_GetAutoCallCmd(nSimID)) && g_State_No_SimCard[nSimID] && g_NW_DialEmcOK[nSimID])
			{
				CFW_PostNotifyEvent(EV_CFW_INIT_IND, 0x00, 0, 0 | (nSimID << 24), CFW_INIT_STATUS_AUTODAIL);

#else
				if ((0x01 == CFW_GetAutoCallCmd()) && g_State_No_SimCard && g_NW_DialEmcOK)
				{
					CFW_PostNotifyEvent(EV_CFW_INIT_IND, 0x00, 0, 0, CFW_INIT_STATUS_AUTODAIL);

#endif
#if 0
					// 
					// Auto dial 112
					// 
					if (ERR_SUCCESS == (ret = CFW_CcEmcDial(NULL, 0)))
					{
						CSW_TRACE(CFW_SHELL_TS_ID, "Auto dial 112 Successl!!!\n");
					}
					else
					{
						CSW_TRACE(CFW_SHELL_TS_ID, "Auto dial 112 failed,The errorcode is 0x%x!!!\n", ret);
						while (ERR_SUCCESS != ret)
						{
							// 
							// Maybe need timer
							// 
							COS_Sleep(500);
							ret = CFW_CcEmcDial(NULL, 0);
						}
					}
#endif

				}
				else
				{
					;
				}
			}

		}
#ifdef CFW_MULTI_SIM
		if (g_nStatus[nSimID] == SHELL_STATE_SIM_IMSI)
		{
			if(FALSE == g_IMSI_INIT[nSimID])
			{
				g_nStatus[nSimID] = SHELL_STATE_WAIT;
				CFW_SimGetProviderId(APP_UTI_SHELL, nSimID);
				g_IMSI_INIT[nSimID] = TRUE;
			}
			else
			{
				//Do nothing
			}
		}
#else
		if (g_nStatus == SHELL_STATE_SIM_IMSI)
		{
			g_nStatus = SHELL_STATE_WAIT;
			CFW_SimGetProviderId(APP_UTI_SHELL);
		}
#endif

#ifdef CFW_MULTI_SIM
		else if (g_nStatus[nSimID] == SHELL_STATE_SMS)
		{
			g_nStatus[nSimID] = SHELL_STATE_WAIT;

			CFW_CfgGetDefaultSmsParam(&g_Info[nSimID], nSimID);

			// 
			// Get the SCA 
			// 
			// TODO... need to check the default index in SIM card for SCA.
			// 
			ret = CFW_SimGetSmsParameters(0, APP_UTI_SHELL, nSimID);
			if (ret != ERR_SUCCESS)
			{
				CSW_TRACE(CFW_SHELL_TS_ID, "Error CFW_SimGetSmsParameters failed,ret 0x%x\n", ret);
				CFW_SmsMoInit(APP_UTI_SHELL, nSimID);
				g_nStatus[nSimID] = SHELL_STATE_WAIT;
			}
		}
#else
		else if (g_nStatus == SHELL_STATE_SMS)
		{
			g_nStatus = SHELL_STATE_WAIT;

			CFW_CfgGetDefaultSmsParam(&g_Info);

			// 
			// Get the SCA 
			// 
			// TODO... need to check the default index in SIM card for SCA.
			// 
			ret = CFW_SimGetSmsParameters(0, APP_UTI_SHELL);
			if (ret != ERR_SUCCESS)
			{
				CSW_TRACE(CFW_SHELL_TS_ID, "Error CFW_SimGetSmsParameters failed,ret 0x%x\n", ret);
#if 0
				CSW_PROFILE_FUNCTION_EXIT(CFW_ShellProc);
				return (ret | BAL_PROC_DONT_FREE_MEM | BAL_PROC_COMPLETE);
#else

				CFW_SmsMoInit(APP_UTI_SHELL);
				g_nStatus = SHELL_STATE_WAIT;
#endif
			}
		}
#endif
#ifdef CFW_MULTI_SIM
	else if (g_nStatus[nSimID] == SHELL_STATE_PBK)
#else
	else if (g_nStatus == SHELL_STATE_PBK)
#endif
	{
#ifdef CFW_MULTI_SIM
		CFW_PostNotifyEvent(EV_CFW_INIT_IND, 0, 0, 0 | (nSimID << 24), CFW_INIT_STATUS_PBK);
		g_nStatus[nSimID] = SHELL_STATE_WAIT;

#else
		CFW_PostNotifyEvent(EV_CFW_INIT_IND, 0, 0, 0, CFW_INIT_STATUS_PBK);
		g_nStatus = SHELL_STATE_WAIT;

#endif
		// SHL_PbkPowerOn((0x04)|(0x02));
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_ShellProc);

	return (BAL_PROC_DONT_FREE_MEM | BAL_PROC_COMPLETE);
}

/*
========================================================   
Flash的特性是擦除过后全部为1，所以这里认为如果标志位1
则是工位不成功，标志位为0，工位执行成功
另外两个结构体factory_settings_t，bind_info_t是连续存储
在FLASH中
========================================================
*/
// TODO I completely agree with that, we will fight in the shade.
BOOL _CheckValue(void)
{
  factory_settings_t* factory_settings_p = (factory_settings_t*)&_factory_start;
  CSW_TRACE(33, TSTXT("valid_marker 0x%x\r\n"), &(factory_settings_p->valid_marker));

  UINT32 nValue = (factory_settings_p->valid_marker);

  if ((nValue >> 24) == ((nValue & 0xFFFFFF) % 0xFF))
  {
    CSW_TRACE(CFW_SHELL_TS_ID, "Factory_Setting_CheckValue success\n");
    return TRUE;
  }
  else
  {
    CSW_TRACE(CFW_SHELL_TS_ID, "Factory_Setting_CheckValue failed\n");
    return FALSE;
  }

}

BOOL CFW_GetAutoCallFlagFromRam(UINT32 *flag_p)
{
    static BOOL gotStruct = FALSE;
    static STACK_AUTO_CALL_T autoCall = {0, };
    
    if (!gotStruct)
    {
        gotStruct = TRUE;
        STACK_AUTO_CALL_T *ramPtr = stack_GetAutoCallStruct();
        if (ramPtr != NULL)
        {
            autoCall = *ramPtr;
            // ram flag is valid for this boot only
            ramPtr->validity = 0;
        }
    }

    if (autoCall.validity != STACK_AUTO_CALL_VALID)
    {
        return FALSE;
    }

    *flag_p = autoCall.flag;
    return TRUE;
}

// 
// Return value: 
// 0x00:No test mode, 0x01: auto dial 112, 0x02:Auto response
// 

#ifdef CFW_MULTI_SIM
UINT8 CFW_GetAutoCallCmd(CFW_SIM_ID nSimID)
#else
UINT8 CFW_GetAutoCallCmd()
#endif
{
    UINT32 mark = 0;
    if (FALSE == CFW_GetAutoCallFlagFromRam(&mark))
    {
        return 0xFF;
    }

    mark = ((mark >> 16) & 0x03);
    if (AUTO_CALL_SETTIGN_DIAL == mark)
    {
        // Auto dial 112
        return 0x01;
    }
    else if (AUTO_CALL_SETTING_RESP == mark)
    {
        // Auto response
        return 0x02;
    }

    return 0x00;
}


#ifdef CFW_MULTI_SIM
VOID CFW_EMCDial(CFW_SIM_ID nSimID)
{
  UINT32 ret = 0x00000000;
  // 
  // Auto Dial 112
  // 
  g_TestMode_AutoEMCDial[nSimID] = 1;

  if (g_TestMode_AutoEMCDialNum[nSimID] < DIAL_EMC_MAXNUM)
  {
    g_TestMode_AutoEMCDialNum[nSimID]++;
    CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("Call 112 %x times\r\n"),
                  g_TestMode_AutoEMCDialNum[nSimID]);
  }
  else
  {
    return;
  }

  COS_Sleep(500);

  if (ERR_SUCCESS == (ret = CFW_CcEmcDial(NULL, 0, nSimID)))
  {
    CSW_TRACE(CFW_SHELL_TS_ID, "Auto Call 112 Success\n");
  }
  else
  {
    CSW_TRACE(CFW_SHELL_TS_ID, "Error Auto Call 112 failed,The errorcode 0x%x\n", ret);
    while (ERR_SUCCESS != ret)
    {
      // 
      // Maybe need timer
      // 
      COS_Sleep(500);
      ret = CFW_CcEmcDial(NULL, 0, nSimID);
    }
  }
}
#else
VOID CFW_EMCDial()
{
  UINT32 ret = 0x00000000;

  // 
  // Auto Dial 112
  // 
  g_TestMode_AutoEMCDial = 1;

  if (g_TestMode_AutoEMCDialNum < DIAL_EMC_MAXNUM)
  {
    g_TestMode_AutoEMCDialNum++;
    CSW_TRACE(CFW_SHELL_TS_ID, TSTXT("Call 112 %x times\r\n"), g_TestMode_AutoEMCDialNum);
  }
  else
  {
    return;
  }

  COS_Sleep(500);

  if (ERR_SUCCESS == (ret = CFW_CcEmcDial(NULL, 0)))
  {
    CSW_TRACE(CFW_SHELL_TS_ID, "Auto Call 112 Successl!\n");
  }
  else
  {
    CSW_TRACE(CFW_SHELL_TS_ID, "Auto Call 112 failed,The errorcode is 0x%x\n", ret);
    while (ERR_SUCCESS != ret)
    {
      // 
      // Maybe need timer
      // 
      COS_Sleep(500);
      ret = CFW_CcEmcDial(NULL, 0);
    }
  }
}
#endif


#ifdef CFW_MULTI_SIM
extern UINT8 nCommondNum[][CFW_SIM_COUNT];
#else
extern UINT8 nCommondNum[];
#endif
// COS_FreeEvent 
// 


BOOL CFW_GetAutoCallFreqBand(UINT8 *pBand)
{
    UINT32 mark = 0;
    if (FALSE == CFW_GetAutoCallFlagFromRam(&mark))
    {
        return FALSE;
    }
    
    mark = ((mark >> 18) & 0x0F);
    if( mark )
    {
        //第18位是GSM850 的功能位， 19位是GSM900的功能位，20是DCS1800的功能位，21是PCS1900的功能位。
        if(0x01 == ( mark & 0x01 ))
        {
          *pBand = API_GSM_850;
        }
        
        if(0x01 == ( ( mark >> 1 ) & 0x01 ))
        {
          *pBand |= ( API_GSM_900E | API_GSM_900P );
        }
        
        if(0x01 == ( ( mark >> 2 ) & 0x01 ))
        {
          *pBand |=  API_DCS_1800;
        }
        
        if(0x01 ==( ( mark >> 3 ) & 0x01 ))
        {
          *pBand |=  API_PCS_1900;
        }
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}
#ifdef CFW_MULTI_SIM
UINT32 cfw_SatProCmdSetupMenuEX(CFW_SIM_ID nSimID, UINT8* pData, UINT8 nLength)
#else
UINT32 cfw_SatProCmdSetupMenuEX(UINT8* pData, UINT8 nLength)
#endif
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
  UINT32 nUTI = GENERATE_SHELL_UTI();

#ifdef CFW_MULTI_SIM
  CFW_CfgSimGetSatParam(&pG_SimSat, nSimID);
#else
  CFW_CfgSimGetSatParam(&pG_SimSat);
#endif
  nIndex = 5;

  if ((pData[nIndex] == 0x05) || (pData[nIndex] == 0x85))
  {
    if (pData[++nIndex] == 0x81)
      nIndex++;
    nAlphaLen = pData[nIndex++];
    i         = nIndex + nAlphaLen;
  }
  else
  {
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
    pItemList->nItemLen = pOffset[++i] - 1; // reduce the item's id
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
#ifdef CFW_MULTI_SIM
  g_SAT_INIT_COMPLETE[nSimID]   = TRUE; // g__SAT_INIT_COMPLETE  to determine whether init is complete?? 

  CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pG_SimSat->pSetupMenu->nComID, 0, nUTI | (nSimID << 24), 0x0);
#else
  g_SAT_INIT_COMPLETE   = TRUE; // g__SAT_INIT_COMPLETE  to determine whether init is complete?? 

  CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pG_SimSat->pSetupMenu->nComID, 0, nUTI, 0x0);
#endif
  return ERR_SUCCESS; // [+ ] dingmx 20080908 remove warning
}
UINT32 CSW_SAT_InitEX(CFW_EV* pEV)
{

  SIM_SAT_PARAM* pG_SimSat = NULL;

#ifdef CFW_MULTI_SIM
  CFW_SIM_ID nSimID = pEV->nFlag;
  CFW_CfgSimGetSatParam(&pG_SimSat,nSimID);

#else
  CFW_CfgSimGetSatParam(&pG_SimSat);
#endif


  if (pEV->nEventId == API_SIM_FETCH_IND)
  {
    UINT8 nIndex        = 0;
    UINT8 nLength       = 0;
    UINT8 nComQualifier = 0;
    CFW_SAT_RESPONSE SimSatResponseData;

    CFW_SAT_RESPONSE* pSimSatResponseData = &SimSatResponseData;

    // api_SimFetchInd_t* pSimFetchInd = (api_SimFetchInd_t*)(&p[1]);
    CSW_TRACE(CFW_SHELL_TS_ID, "Get API_SIM_FETCH_IND in CSW_SAT_InitEX\n");
    UINT8* pOffset = (UINT8*)pEV->nParam1;

    pOffset += 2;
    

    if (pOffset[1] == 0x81)
      nIndex = 2;
    else
      nIndex = 1;
    nLength  = pOffset[nIndex] + nIndex; // Update the length

    CSW_TRACE(CFW_SHELL_TS_ID, "Fetch data lenght %d\n\n", nLength);

    pSimSatResponseData->nSAT_CmdType = pOffset[nIndex + 4];
    nComQualifier                     = pOffset[nIndex + 5];
    pG_SimSat->nCurCmd                = pOffset[nIndex + 4];
    pG_SimSat->nCmdNum                = pOffset[nIndex + 3];

#ifdef CFW_MULTI_SIM
    nCommondNum[pG_SimSat->nCurCmd - 1][nSimID] = pG_SimSat->nCmdNum;

#else
    nCommondNum[pG_SimSat->nCurCmd - 1] = pG_SimSat->nCmdNum;
#endif

    CSW_TRACE(CFW_SHELL_TS_ID, "nSAT_CmdType: %x\n", pSimSatResponseData->nSAT_CmdType);
    CSW_TRACE(CFW_SHELL_TS_ID, "nCmdNum : %x\n", pG_SimSat->nCmdNum);
	

    if (pSimSatResponseData->nSAT_CmdType == 0x25)
    {
       cfw_SatProCmdSetupMenuEX(nSimID, pOffset + nIndex + 5, nLength - nIndex - 5);

    }
    else if (pSimSatResponseData->nSAT_CmdType == 0x24)
    {
      CFW_SAT_MENU_RSP* pMenuItemList = NULL;

      if (pOffset[0] != 0xD0)
      {
        // CSW_TRACE(CFW_SHELL_TS_ID, "\nERR_PARAM_ILLEGAL error,in SAT: %x\n", pEvent->nParam1);
        CSW_TRACE(CFW_SHELL_TS_ID, "Error Invalid data Not 0xD0 But 0x%x",pOffset[0]);
        // CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, GENERATE_SHELL_UTI(), 0xF0);
        return ERR_INVALID_PARAMETER;
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
        }
        else
        {
          nAlphaLen = 0;
          i         = nIndex;
        }

        nMemsize = SIZEOF(CFW_SAT_MENU_RSP) + nAlphaLen;
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

        pMenuItemList = (CFW_SAT_MENU_RSP*)CSW_SIM_MALLOC(nMemsize);
        if (pMenuItemList == NULL)
        {
          CSW_TRACE(CFW_SHELL_TS_ID, "Error Malloc failed\n");
          // CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, GENERATE_SHELL_UTI(), 0xF0);
          return ERR_NO_MORE_MEMORY;
        }

        pMenuItemList->nComID        = pG_SimSat->nCurCmd;
        pMenuItemList->nComQualifier = nComQualifier;
        pMenuItemList->nAlphaLen     = nAlphaLen;
        nItemNum                     = SIZEOF(CFW_SAT_ITEM_LIST) * nItemNum;

        if (nAlphaLen != 0)
        {
          pMenuItemList->pAlphaStr = SIZEOF(CFW_SAT_MENU_RSP) + (UINT8*)pMenuItemList + nItemNum;
          SUL_MemCopy8(pMenuItemList->pAlphaStr, (pOffset + nIndex), nAlphaLen);
          nItemNum = (UINT32)(pMenuItemList->pAlphaStr) + (UINT32)nAlphaLen;
        }
        else
        {
          pMenuItemList->pAlphaStr = NULL;
          nItemNum                 = SIZEOF(CFW_SAT_MENU_RSP) + (UINT32)pMenuItemList + nItemNum;
        }
        pMenuItemList->pItemList = (CFW_SAT_ITEM_LIST*)(SIZEOF(CFW_SAT_ITEM_LIST) + (UINT8*)pMenuItemList);
        pItemList                = pMenuItemList->pItemList; // pointer point to the byte after alpha string

        i = nIndex + nAlphaLen;

        for (;;)
        {
          if ((pOffset[i] != 0x0f) && (pOffset[i] != 0x8f))
          {
            pItemList->pNextItem = NULL;
            break;
          }
          if (pOffset[++i] == 0x81)
            i++;
          pItemList->nItemLen = pOffset[i] - 1; // reduce the item's id

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
          CSW_SIM_FREE(pG_SimSat->pSatComData);

        pG_SimSat->pSatComData = pMenuItemList;
#ifdef CFW_MULTI_SIM
        CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0,
                            GENERATE_SHELL_UTI() | (nSimID << 24), 0);
#else
        CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, GENERATE_SHELL_UTI(), 0);
#endif
      }

    }
    else if (pSimSatResponseData->nSAT_CmdType == 0x03)
    {
      // UINT8 nCodeSch     = 0x00;
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
        CSW_TRACE(CFW_SHELL_TS_ID, "Error Invalid data Not 0x04 or 0x84 But 0x%x",pOffset[nIndex]);
#ifdef CFW_MULTI_SIM
        CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, GENERATE_SHELL_UTI() | (nSimID << 24), 0xF0);
#else
        CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, GENERATE_SHELL_UTI(), 0xF0);
#endif
        // CFW_UnRegisterAO(CFW_SIM_SRV_ID,hSimSatResponseAo);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
        return ERR_INVALID_PARAMETER;
      }
      pPIResp = (CFW_SAT_POLL_INTERVAL_RSP*)(CSW_SIM_MALLOC(SIZEOF(CFW_SAT_POLL_INTERVAL_RSP)));
      if (pPIResp == NULL)
      {
         CSW_TRACE(CFW_SHELL_TS_ID, "Error Malloc failed\n");
#ifdef CFW_MULTI_SIM
        CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, GENERATE_SHELL_UTI() | (nSimID << 24),
                            0xF0);
#else
        CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, GENERATE_SHELL_UTI(), 0xF0);
#endif
        // CFW_UnRegisterAO(CFW_SIM_SRV_ID,hSimSatResponseAo);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
        return ERR_NO_MORE_MEMORY;;
      }
      pPIResp->nComID        = pG_SimSat->nCurCmd;
      pPIResp->nComQualifier = nComQualifier;
      pPIResp->nTimeUnit     = nTimeUnit;
      pPIResp->nTimeInterval = nTimeInterval;
      CSW_TRACE(CFW_SHELL_TS_ID, "pG_SimSat->nCurCmd 0x%x,nComQualifier 0x%x,nTimeUnit 0x%x,nTimeInterval 0x%x\n",
                    pG_SimSat->nCurCmd, nComQualifier, nTimeUnit, nTimeInterval);

      if (pG_SimSat->pSatComData != NULL)
        CSW_SIM_FREE(pG_SimSat->pSatComData);
      pG_SimSat->pSatComData = pPIResp;
#ifdef CFW_MULTI_SIM
      CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, 1 | (nSimID << 24), 0x0);
#else
      CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, 1, 0x0);
#endif
    }
    else if (pSimSatResponseData->nSAT_CmdType == 0x13)
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
        CSW_TRACE(CFW_SHELL_TS_ID, "Error Invalid data Not 0x0B or 0x8B But 0x%x",pOffset[nIndex]);        
        // CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, GENERATE_SHELL_UTI(), 0xF0);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
        return ERR_INVALID_PARAMETER;
      }
      nMemsize += nLength;

      pSmsResp = (CFW_SAT_SMS_RSP*)CSW_SIM_MALLOC(nMemsize);
      if (pSmsResp == NULL)
      {
        CSW_TRACE(CFW_SHELL_TS_ID, "Error Malloc failed\n");        
        // CFW_PostNotifyEvent(ERR_PARAM_ILLEGAL, ERR_PARAM_ILLEGAL, 0, GENERATE_SHELL_UTI(), 0xF0);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
        return ERR_NO_MORE_MEMORY;
      }
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

      SUL_MemCopy8(pSmsResp->pPDUStr, nTPDUStr, nTPDULen);

      if (pG_SimSat->pSatComData != NULL)
        CSW_SIM_FREE(pG_SimSat->pSatComData);

      pG_SimSat->pSatComData = pSmsResp;

// CFW_GetUTI(hSimSatResponseAo, &nUTI);

      // CSW_TRACE(CFW_SHELL_TS_ID, "EV_CFW_SAT_CMDTYPE_IND 0x13 --in SAT---%x\n\n", pEvent->nParam1);
#ifdef CFW_MULTI_SIM
      CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0,
                          GENERATE_SHELL_UTI() | (nSimID << 24), 0x0);
      CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0,
                          1 | (nSimID << 24), 0x0);
#else
      CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, GENERATE_SHELL_UTI(), 0x0);
      CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, 1, 0x0);
#endif
    }
    else if (pSimSatResponseData->nSAT_CmdType == 0x21)
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
        CSW_TRACE(CFW_SHELL_TS_ID, "Error Invalid data Not 0x0D or 0x8D But 0x%x",pOffset[nIndex]);        
        // CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, GENERATE_SHELL_UTI(), 0xF0);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
        return ERR_INVALID_PARAMETER;;
      }
      pDTextResp = (CFW_SAT_DTEXT_RSP*)(CSW_SIM_MALLOC(SIZEOF(CFW_SAT_DTEXT_RSP) + nLength));
      if (pDTextResp == NULL)
      {
        CSW_TRACE(CFW_SHELL_TS_ID, "Error Malloc failed\n");
        // CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, GENERATE_SHELL_UTI(), 0xF0);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
        return ERR_NO_MORE_MEMORY;;
      }
      pDTextResp->nComID        = pG_SimSat->nCurCmd;
      pDTextResp->nComQualifier = nComQualifier;
      pDTextResp->nTextSch      = nCodeSch;
      pDTextResp->nTextLen      = nLength;
      pDTextResp->pTextStr      = (SIZEOF(CFW_SAT_DTEXT_RSP) + (UINT8*)pDTextResp);
      SUL_MemCopy8(pDTextResp->pTextStr, pTextStr, nLength);

      if (pG_SimSat->pSatComData != NULL)
	{
      		CSW_TRACE(CFW_SHELL_TS_ID, "pG_SimSat->pSatComData != NULL\n");
		CSW_SIM_FREE(pG_SimSat->pSatComData);
	}
      pG_SimSat->pSatComData = pDTextResp;
#ifdef CFW_MULTI_SIM
      CSW_TRACE(CFW_SHELL_TS_ID, "pSimSatResponseData->nSAT_CmdType == 0x21 2 %x nSimID %d\n",pDTextResp, nSimID);
      CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, 1 | (nSimID << 24), 0x0);
#else
      CSW_TRACE(CFW_SHELL_TS_ID, "pSimSatResponseData->nSAT_CmdType == 0x21 2 %x\n",pDTextResp);
      CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, 1, 0x0);
#endif
    }
    else if (pSimSatResponseData->nSAT_CmdType == 0x23) // GetInput
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
        // CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, GENERATE_SHELL_UTI(), 0xF0);
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
        CSW_TRACE(CFW_SHELL_TS_ID, "Error Invalid data Not 0x11 or 0x91 But 0x%x",pOffset[nIndex]);        
        // CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, GENERATE_SHELL_UTI(), 0xF0);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
        return ERR_INVALID_PARAMETER;;
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
        CSW_TRACE(CFW_SHELL_TS_ID, "Error Malloc failed\n");
        // CFW_PostNotifyEvent(ERR_NO_MORE_MEMORY, ERR_NO_MORE_MEMORY, 0, GENERATE_SHELL_UTI(), 0xF0);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
        return ERR_NO_MORE_MEMORY;;
      }
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

      // CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, GENERATE_SHELL_UTI(), 0x0);
    }
    else if (pSimSatResponseData->nSAT_CmdType == 0x10) // set up call
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
        CSW_TRACE(CFW_SHELL_TS_ID, "Error Invalid data Not 0x06 or 0x86 But 0x%x",pOffset[nIndex]);
        // CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, GENERATE_SHELL_UTI(), 0xF0);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
        return ERR_INVALID_PARAMETER;;
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

      nIndex = nIndex + nLength;
      nMemsize += nLength;
      if ((pOffset[nIndex] == 0x04) || (pOffset[nIndex] == 0x84)) // Duration
      {
        nTuint     = pOffset[nIndex + 2];
        nTinterval = pOffset[nIndex + 3];
        nLength    = 4;
      }
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

      pCallResp = (CFW_SAT_CALL_RSP*)CSW_SIM_MALLOC(nMemsize);
      if (pCallResp == NULL)
      {
        CSW_TRACE(CFW_SHELL_TS_ID, "Error Malloc failed\n");
        // CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, GENERATE_SHELL_UTI(), 0xF0);
        CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
        return ERR_NO_MORE_MEMORY;;
      }
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
      pCallResp->pAlphaSetupStr = pCallResp->pCapabilityCfgStr + nCapabilityCfgLen;
      SUL_MemCopy8(pCallResp->pAlphaSetupStr, pAlphaSetupStr, nAlphaSetupLen);
      if (pG_SimSat->pSatComData != NULL)
        CSW_SIM_FREE(pG_SimSat->pSatComData);
      pG_SimSat->pSatComData = pCallResp;
#ifdef CFW_MULTI_SIM
      CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0,
                          GENERATE_SHELL_UTI() | (nSimID << 24), 0x0);
#else
      CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, GENERATE_SHELL_UTI(), 0x0);
#endif
    }
    else if ((pSimSatResponseData->nSAT_CmdType == 0x26)  // Local information
             || (pSimSatResponseData->nSAT_CmdType == 0x02) // More time
             || (pSimSatResponseData->nSAT_CmdType == 0x04) // Polling off
             || (pSimSatResponseData->nSAT_CmdType == 0x32) // Power off card
// ||(pSimSatResponseData->nSAT_CmdType == 0x31)    //Power on card,StackCom not support
             || (pSimSatResponseData->nSAT_CmdType == 0x33) // Get reader status
             || (pSimSatResponseData->nSAT_CmdType == 0x44) // Get channel status
             || (pSimSatResponseData->nSAT_CmdType == 0x05))
    {
      SAT_BASE_RSP* pBaseResp = (SAT_BASE_RSP*)CSW_SIM_MALLOC(SIZEOF(SAT_BASE_RSP));
      if (pBaseResp == NULL)
      {
        // CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_NO_MORE_MEMORY, 0, GENERATE_SHELL_UTI(), 0xF0);
        CSW_TRACE(CFW_SHELL_TS_ID, "Error Malloc failed\n");
        CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
        return ERR_NO_MORE_MEMORY;;
      }
      pBaseResp->nComID        = pG_SimSat->nCurCmd;
      pBaseResp->nComQualifier = nComQualifier;
      if (pG_SimSat->pSatComData != NULL)
        CSW_SIM_FREE(pG_SimSat->pSatComData);
      pG_SimSat->pSatComData = pBaseResp;

#ifdef CFW_MULTI_SIM
      CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, 1 | (nSimID << 24), 0x0);
      CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0,
                          GENERATE_SHELL_UTI() | (nSimID << 24), 0x0);
#else
      CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, 1, 0x0);
      CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pSimSatResponseData->nSAT_CmdType, 0, GENERATE_SHELL_UTI(), 0x0);
#endif
    }

    else
    {
      // CFW_PostNotifyEvent(EV_CFW_SAT_RESPONSE_RSP, ERR_PARAM_ILLEGAL, 0, GENERATE_SHELL_UTI(), 0xF0);
      CSW_TRACE(CFW_SHELL_TS_ID, "Error Command Type 0x%x\n",pSimSatResponseData->nSAT_CmdType);

      CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponseProc);
      return ERR_INVALID_PARAMETER;
    }
  }
  return ERR_SUCCESS;
}
 
