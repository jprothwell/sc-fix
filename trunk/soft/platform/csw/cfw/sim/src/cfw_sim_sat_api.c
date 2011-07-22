////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2010, Coolsand Technologies, Inc.
//                       All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmission of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: cfw_sim_sat_api.c
// 
// DESCRIPTION:
// TODO: ...
// 
// REVISION HISTORY:
//    NAME              DATE                REMAKS
// Administrator      2010-8-30       Created initial version 1.0
//
////////////////////////////////////////////////////////////////////////////////
 
 

#include <cswtype.h>
#include <errorcode.h>
//#include <scl.h>
#include "api_msg.h"
#include <cfw.h>
#include <base.h>
#include <cfw_prv.h>
#include <event.h>
#include <sul.h>
#include <ts.h>
#include "csw_debug.h"

#include "cfw_sim.h"
#include "cfw_sim_sat.h"





UINT32 CFW_SatResponseProc (HAO hAo,CFW_EV* pEvent);
UINT32 CFW_SatActivationProc (HAO hAo,CFW_EV* pEvent);


#ifdef CFW_MULTI_SIM
UINT32 CFW_SatActivation(UINT8 nMode,UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SatActivation(UINT8 nMode,UINT16 nUTI)
#endif
{

    CSW_TRACE(CFW_SIM_TS_ID, "nMode = 0x%x\n",nMode);

    HAO hSimSatActivationAo;
    CFW_SAT_RESPONSE* pSatActivationData;

    CSW_PROFILE_FUNCTION_ENTER(CFW_SatActivation);
    if(nMode > 2)
    {
        CSW_PROFILE_FUNCTION_EXIT(CFW_SatActivation);
        return ERR_CFW_INVALID_PARAMETER;
    }
    if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
    {
        CSW_PROFILE_FUNCTION_EXIT(CFW_SatActivation);
        return ERR_CFW_UTI_EXIST;
    }

    pSatActivationData = (CFW_SAT_RESPONSE*)CSW_SIM_MALLOC(SIZEOF(CFW_SAT_RESPONSE));
    if(pSatActivationData == NULL)
    {
        CSW_PROFILE_FUNCTION_EXIT(CFW_SatActivation);
        return ERR_NO_MORE_MEMORY;
    }
    pSatActivationData->nSAT_Mode = nMode;

#ifdef CFW_MULTI_SIM
    hSimSatActivationAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSatActivationData, CFW_SatActivationProc, nSimID);
#else
    hSimSatActivationAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSatActivationData, CFW_SatActivationProc);
#endif
    CFW_SetUTI(hSimSatActivationAo, nUTI, 0);
//    if((nCharacter == 0)||(nCharacter == 1))
//        pShareData->SAT_Code = nCharacter;
//    else
//        return ERR_CFW_INVALID_PARAMETER;
    pSatActivationData->nSm_Act_prevStatus = SM_ACT_INIT;
    pSatActivationData->nSm_Act_currStatus = SM_ACT_IDLE;

    CFW_SetAoProcCode(hSimSatActivationAo,CFW_AO_PROC_CODE_CONTINUE);           
    CSW_PROFILE_FUNCTION_EXIT(CFW_SatActivation);
    return(ERR_SUCCESS);
}


#ifdef CFW_MULTI_SIM
UINT32 CFW_SatResponse(UINT8 nCmdType,UINT8 nStatus,UINT8 nItemId,
                        VOID* pInputStr,UINT8 InputStrLen,UINT16 nUTI, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SatResponse(UINT8 nCmdType,UINT8 nStatus,UINT8 nItemId,
                        VOID* pInputStr,UINT8 InputStrLen,UINT16 nUTI)
#endif
{   
    HAO hSimSatResponseAo;
    CFW_SAT_RESPONSE* pSimSatResponseData;
#ifdef CFW_MULTI_SIM
    CSW_TRACE(CFW_SIM_TS_ID, "CFW_SatResponse nCmdType %x nStatus %x nItemId %x InputStrLen %x nSimID[%d]\n",nCmdType,nStatus,nItemId,InputStrLen,nSimID);
#else
    CSW_TRACE(CFW_SIM_TS_ID, "CFW_SatResponse nCmdType %x nStatus %x nItemId %x InputStrLen %x\n",nCmdType,nStatus,nItemId,InputStrLen);
#endif
    CSW_PROFILE_FUNCTION_ENTER(CFW_SatResponse);
    if((nCmdType == 0x00) || ((nCmdType >= 0x05) && (nCmdType < 0x10)) || ((nCmdType > 0x15)
        && (nCmdType < 0x20)) || (nCmdType == 0x29) || ((nCmdType > 0x35)
        && (nCmdType < 0x40)) || ((nCmdType > 0x44) && (!((nCmdType == 0xD3) ||(nCmdType == 0xD1)||(nCmdType == 0xD6)))))
    {
        CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponse);
        return ERR_CFW_INVALID_PARAMETER;
    }
       CSW_TRACE(CFW_SIM_TS_ID, "CFW_SatResponse nCmdType %x\n",nCmdType);
	 //  CSW_TC_MEMBLOCK(CFW_SIM_TS_ID, pInputStr, InputStrLen, 16);
    if (CFW_IsFreeUTI(nUTI, CFW_SIM_SRV_ID) != ERR_SUCCESS)
    {
       CSW_TRACE(CFW_SIM_TS_ID, "CFW_IsFreeUTI\n");
        CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponse);
        return ERR_CFW_UTI_EXIST;
    }
    pSimSatResponseData = (CFW_SAT_RESPONSE*)CSW_SIM_MALLOC(SIZEOF(CFW_SAT_RESPONSE)+InputStrLen);
    if(pSimSatResponseData == NULL)
    {
        CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponse);
        return ERR_NO_MORE_MEMORY;
    }
	SUL_MemSet8(pSimSatResponseData, 0, (SIZEOF(CFW_SAT_RESPONSE)+InputStrLen));
	
    pSimSatResponseData->nSAT_CmdType = nCmdType;

#ifdef CFW_MULTI_SIM
    hSimSatResponseAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimSatResponseData, CFW_SatResponseProc, nSimID);
#else
    hSimSatResponseAo = CFW_RegisterAo(CFW_SIM_SRV_ID, pSimSatResponseData, CFW_SatResponseProc);
#endif
    CFW_SetUTI(hSimSatResponseAo, nUTI, 0);

    pSimSatResponseData->nSAT_Status = nStatus;
    pSimSatResponseData->nSAT_ItemID = nItemId;
    pSimSatResponseData->nSAT_InputLen = InputStrLen;
	if(nCmdType == 0x23)
	{
		pSimSatResponseData->nSAT_InputSch = *(UINT8*)pInputStr;
		if(InputStrLen != 0)
   		{
			SUL_MemCopy8(pSimSatResponseData->nSAT_InputStr,(UINT8*)pInputStr+1,InputStrLen-1);
	//		CSW_TC_MEMBLOCK(CFW_SIM_TS_ID, pSimSatResponseData->nSAT_InputStr, pSimSatResponseData->nSAT_InputLen, 16);
   		}
	}
	else if(nCmdType == 0x26)
	{
		SUL_MemCopy8(pSimSatResponseData->nSAT_InputStr,(UINT8*)pInputStr,InputStrLen);
	}
  else if(nCmdType == 0xD1)
	{
    CSW_TRACE(CFW_SIM_TS_ID, "InputStrLen %x\n",InputStrLen);
    pSimSatResponseData->nSAT_InputLen = InputStrLen;
		SUL_MemCopy8(pSimSatResponseData->nSAT_InputStr,(UINT8*)pInputStr,InputStrLen);
	}
  else if(nCmdType == 0x12)
	{
    CSW_TRACE(CFW_SIM_TS_ID, "InputStrLen %x\n",InputStrLen);
    pSimSatResponseData->nSAT_InputLen = InputStrLen;
		SUL_MemCopy8(pSimSatResponseData->nSAT_InputStr,(UINT8*)pInputStr,InputStrLen);
	}
    pSimSatResponseData->nSm_Resp_prevStatus = SM_RESP_INIT;
    pSimSatResponseData->nSm_Resp_currStatus = SM_RESP_IDLE;

    CFW_SetAoProcCode(hSimSatResponseAo,CFW_AO_PROC_CODE_CONTINUE);

    CSW_PROFILE_FUNCTION_EXIT(CFW_SatResponse);
    
    return(ERR_SUCCESS);
}

#ifdef CFW_MULTI_SIM
UINT32 CFW_SatGetInformation( UINT8 nCmdType, VOID** CmdData ,CFW_SIM_ID nSimID)
#else
UINT32 CFW_SatGetInformation( UINT8 nCmdType, VOID** CmdData )
#endif
{
    SIM_SAT_PARAM* pSatGetInfo = NULL;

    CSW_PROFILE_FUNCTION_ENTER(CFW_SatGetInformation);

#ifdef CFW_MULTI_SIM
    UINT32 ret = ERR_SUCCESS;
    ret = CFW_CheckSimId(nSimID);
    if( ERR_SUCCESS != ret)
    {
        CSW_PROFILE_FUNCTION_EXIT(CFW_SatGetInformation);
        return ret;
    }
    CFW_CfgSimGetSatParam(&pSatGetInfo, nSimID);
#else
    CFW_CfgSimGetSatParam(&pSatGetInfo);
#endif

    switch(nCmdType)
    {
        case 0x25:
        {
            if(pSatGetInfo->pSetupMenu == NULL)
            {
               CSW_PROFILE_FUNCTION_EXIT(CFW_SatGetInformation);
               return ERR_CFW_INVALID_COMMAND;
            }
            *CmdData = pSatGetInfo->pSetupMenu;
        }
        break;
        case 0x02:
        case 0x04:            
        case 0x10:
        case 0x12:
        case 0x13:
        case 0x21:
        case 0x24:
        case 0x23:
        case 0x26:
        case 0x32:
        case 0x33:
        case 0x44:
        case 0x15:
        {
            if(pSatGetInfo->pSatComData == NULL)
            {
               CSW_PROFILE_FUNCTION_EXIT(CFW_SatGetInformation);
               return ERR_CFW_INVALID_COMMAND;
            }
            if(nCmdType == pSatGetInfo->nCurCmd)
                *CmdData = pSatGetInfo->pSatComData;            
            else
            {
                CSW_PROFILE_FUNCTION_EXIT(CFW_SatGetInformation);
                return ERR_CFW_INVALID_PARAMETER;
            }
        }
        break;
        case 0x01:
        case 0x03:
        case 0x05:

        case 0x11:
        //case 0x12:
        case 0x14:
        

        case 0x20:
        case 0x22:

        case 0x27:
        case 0x28:

        case 0x30:
        case 0x31:
        case 0x34:
        case 0x35:

        case 0x40:
        case 0x41:
        case 0x42:
        case 0x43:
        {
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatGetInformation);
            return ERR_CFW_NOT_SUPPORT;
        }
        default:
        {
            CSW_PROFILE_FUNCTION_EXIT(CFW_SatGetInformation);
            return ERR_CFW_INVALID_PARAMETER;
        }
    }
    CSW_PROFILE_FUNCTION_EXIT(CFW_SatGetInformation);
    return ERR_SUCCESS;
}

 
//
// FUNCTION: CSW_SatGetCurCMD
//
// DESCRIPTION:
//   #This function is ... #
//
// ARGUMENTS: 
//   #  arg1# [in] #...#
//                 #...#
//   #  arg2# [in,out] 
//   #  arg3# [out] 
//
// RETURN:
//   #TODO...#
//
// COMMENTS:
//   1> #...#
//   2> #...#
//
// EXAMPLE:
//   #... #
//
#ifdef CFW_MULTI_SIM
UINT8 CFW_SatGetCurCMD(CFW_SIM_ID nSimID)
#else
UINT8 CFW_SatGetCurCMD(VOID)
#endif
{
  SIM_SAT_PARAM* pSatGetInfo = NULL;

#ifdef CFW_MULTI_SIM
    UINT32 ret = ERR_SUCCESS;
    ret = CFW_CheckSimId(nSimID);
    if( ERR_SUCCESS != ret)
    {
        return ret;
    }
    CFW_CfgSimGetSatParam(&pSatGetInfo, nSimID);
#else
    CFW_CfgSimGetSatParam(&pSatGetInfo);
#endif

  return pSatGetInfo->nCurCmd;
}

//b0  == FDN
//b1  == BDN
//b2  == CC
//0: Not Support,1: Support
#ifdef CFW_MULTI_SIM
extern CFW_PROFILE nMEProfile;
UINT32 CFW_CfgSimGetMeProfile(CFW_PROFILE* pMeProfile, CFW_SIM_ID nSimID)
{
    UINT32 ret = ERR_SUCCESS;
    ret = CFW_CheckSimId(nSimID);
    if( ERR_SUCCESS != ret)
    {
        return ret;
    }

    SUL_MemCopy8(pMeProfile,&nMEProfile,SIZEOF(CFW_PROFILE));

    return ERR_SUCCESS;
}

UINT32 CFW_CfgSimSetMeProfile(CFW_PROFILE* pMeProfile, CFW_SIM_ID nSimID)
{
    UINT32 ret = ERR_SUCCESS;
    ret = CFW_CheckSimId(nSimID);
    if( ERR_SUCCESS != ret)
    {
        return ret;
    }
    SUL_MemCopy8(&nMEProfile,pMeProfile,SIZEOF(CFW_PROFILE));

    return ERR_SUCCESS;
}

#else

UINT32 CFW_CfgSimGetMeProfile(CFW_PROFILE* pMeProfile)
{
    SUL_MemCopy8(pMeProfile,&nMEProfile,SIZEOF(CFW_PROFILE));
    return ERR_SUCCESS;
}

UINT32 CFW_CfgSimSetMeProfile(CFW_PROFILE* pMeProfile)
{
    SUL_MemCopy8(&nMEProfile,pMeProfile,SIZEOF(CFW_PROFILE));
    return ERR_SUCCESS;
}
#endif

