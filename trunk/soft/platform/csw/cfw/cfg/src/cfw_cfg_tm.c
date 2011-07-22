///*************************************************************
//hameina 2006.8.21 modified
//
//notice: this version is based on reg systerm
//
//modify record:
//2006.10.13 hameina add 6 new functions for emod
///**************************************************************

#include "sul.h"
#include "cfw.h"
#include "cfw_cfg.h"
#include "csw_debug.h"
//PRIVATE TM_PARAMETERS g_Cfg_Tm_Params;

//[[hameina[+]2006.8.23 :EMOD ADD TWO NEW FUNCTIONS
UINT32 CFW_CfgGetEmodParam(EMOD_PARAMETERS*  pEmodParam)
{
	UINT32 ret = ERR_SUCCESS;
	if(NULL == pEmodParam)
		return ERR_CFW_INVALID_PARAMETER;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetEmodParam);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetEmodParam enter\n");
    ret = cfg_read_hex("emod", "EMOD_PARAM", (PVOID) pEmodParam, (UINT8)sizeof(EMOD_PARAMETERS));
    if(ERR_SUCCESS != ret)
    {
	    CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetEmodParam,1, call cfg_read_hex failed,ret = 0x%x, zero memory here\n",ret);
	    SUL_ZeroMemory8(pEmodParam, sizeof(EMOD_PARAMETERS));
    }
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetEmodParam);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetEmodParam exit ret= 0x%x\n", ret);
    return ret;
}

UINT32 CFW_CfgSetEmodParam(EMOD_PARAMETERS*  pEmodParam)
{
	UINT32 ret = ERR_SUCCESS;
	if(NULL == pEmodParam)
		return ERR_CFW_INVALID_PARAMETER;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetEmodParam);	
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetEmodParam enter\n");
    ret = cfg_write_hex("emod", "EMOD_PARAM", (PVOID) pEmodParam, sizeof(EMOD_PARAMETERS));
    if(ERR_SUCCESS != ret)
    {
	    CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgSetEmodParam,1, call cfg_write_hex failed,ret = 0x%x\n",ret);
    }
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetEmodParam);	
    return ret;
}
//]]hameina[+]2006.8.23

//[[hameina [+] 2006.10.13
#if 0
typedef struct _BIND_SIM_NM
{
	UINT8 simNo[90];
	UINT8 pad[2];
}BIND_SIM_NM;

typedef struct _BIND_NW_NM
{
	UINT8 nwNo[15];  
	UINT8 pad;
}BIND_NW_NM;
#endif
UINT32 Cfg_Emod_ReadSIMNm(BIND_SIM_NM*simnm)
{
	UINT32 ret = ERR_SUCCESS;
    if(NULL == simnm)
	    return ERR_CFG_PARAM_OUT_RANGR;
//    CSW_PROFILE_FUNCTION_ENTER(Cfg_Emod_ReadSIMNm);
	ret = cfg_read_hex("emod", "EMOD_SIMNM_1", simnm, 48);
	if(ERR_SUCCESS == ret)
	{
		ret = cfg_read_hex("emod", "EMOD_SIMNM_2", &(simnm->simNo[48]), 42);
		if(ERR_SUCCESS == ret)
		{
//			CSW_PROFILE_FUNCTION_EXIT(Cfg_Emod_ReadSIMNm);
			return ret;
		}
	}

	//read data failed!, call read default begin
	ret = cfg_readdefault("emod", "EMOD_SIMNM_1", simnm);
	if(ERR_SUCCESS == ret)
	{
		ret = cfg_readdefault("emod", "EMOD_SIMNM_2", &(simnm->simNo[48]));
	}
	
	if(ERR_SUCCESS != ret)
	{
	    CSW_TRACE(CFW_CFG_TS_ID, "Cfg_Emod_ReadSIMNm error ret = 0x%x\n",ret);
		SUL_ZeroMemory8(simnm,sizeof( BIND_SIM_NM));
	}
		
//	CSW_PROFILE_FUNCTION_EXIT(Cfg_Emod_ReadSIMNm);
	return ret;
}


UINT32 Cfg_Emod_UpdateSIMNm(BIND_SIM_NM*simnm)
{
	UINT32 ret = ERR_SUCCESS;
    if(NULL == simnm)
	    return ERR_CFG_PARAM_OUT_RANGR;
//    CSW_PROFILE_FUNCTION_ENTER(Cfg_Emod_UpdateSIMNm);
	ret = cfg_write_hex("emod", "EMOD_SIMNM_1", simnm, 48);
	if(ERR_SUCCESS == ret)
	{
		ret = cfg_write_hex("emod", "EMOD_SIMNM_2", &(simnm->simNo[48]), 42);	
	}
//	CSW_PROFILE_FUNCTION_EXIT(Cfg_Emod_UpdateSIMNm);
	return ret;
}


UINT32 Cfg_Emod_ReadNWNm(BIND_NW_NM*nwnm)
{
	UINT32 ret = ERR_SUCCESS;
    if(NULL == nwnm)
	    return ERR_CFG_PARAM_OUT_RANGR;
	    
//    CSW_PROFILE_FUNCTION_ENTER(Cfg_Emod_ReadNWNm);
    ret = cfg_read_hex("emod", "EMOD_NWNM", nwnm, 16);
    if(ERR_SUCCESS != ret)
    {
	    CSW_TRACE(CFW_CFG_TS_ID, "in Cfg_Emod_ReadNWNm,1, call cfg_read_hex failed,ret = 0x%x, zero memory here\n",ret);
	    ret = cfg_readdefault("emod", "EMOD_NWNM", nwnm);
        if(ERR_SUCCESS != ret)
	    {
		    CSW_TRACE(CFW_CFG_TS_ID, "in Cfg_Emod_ReadNWNm,2, call cfg_read_hex failed,ret = 0x%x, zero memory here\n",ret);
		    SUL_ZeroMemory8(nwnm, sizeof(BIND_NW_NM));
		}
    }
//	CSW_PROFILE_FUNCTION_EXIT(Cfg_Emod_ReadNWNm);
    return ret;
}


UINT32 Cfg_Emod_UpdateNWNm(BIND_NW_NM*nwnm)
{
	UINT32 ret = ERR_SUCCESS;
    if(NULL == nwnm)
	    return ERR_CFG_PARAM_OUT_RANGR;
	    
//    CSW_PROFILE_FUNCTION_ENTER(Cfg_Emod_UpdateNWNm);
    ret = cfg_write_hex("emod", "EMOD_NWNM", nwnm, 16);
    if(ERR_SUCCESS != ret)
    {
	    CSW_TRACE(CFW_CFG_TS_ID, "in Cfg_Emod_UpdateNWNm,1,write hex err!, ret = 0x%x\n", ret);
    }
//    CSW_PROFILE_FUNCTION_EXIT(Cfg_Emod_UpdateNWNm);
    return ret;
}


UINT32 Cfg_Emod_ReadSign(BOOL* sim_Sign,BOOL* nw_Sign)
{
    UINT32 ret = ERR_SUCCESS;
	UINT8 value=0;
//    CSW_PROFILE_FUNCTION_ENTER(Cfg_Emod_ReadSign);
	ret = cfg_read_8("emod", "EMOD_SIGN", &value);
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "in Cfg_Emod_ReadSign,1, call cfg_read_8 falsed, ret =0x%x\n",ret);
		ret = cfg_readdefault("emod", "EMOD_SIGN", &value);
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in Cfg_Emod_ReadSign,1, call cfg_readdefault falsed, ret =0x%x\n",ret);
//		    CSW_PROFILE_FUNCTION_EXIT(Cfg_Emod_ReadSign);
			return ret;
		}
	}
	*sim_Sign = value & 1;
	*nw_Sign = (value & 2)?1:0;
//    CSW_PROFILE_FUNCTION_EXIT(Cfg_Emod_ReadSign);

	return ret;
}


UINT32 Cfg_Emod_UpdateSign(BOOL sim_Sign,BOOL nw_Sign)
{
    UINT32 ret = ERR_SUCCESS;
	UINT8 value=0;
//    CSW_PROFILE_FUNCTION_ENTER(Cfg_Emod_UpdateSign);
    value = (sim_Sign&1) + ((nw_Sign&1)<<1);
    CSW_TRACE(CFW_CFG_TS_ID, "in Cfg_Emod_UpdateSign,1, input sim_Sign=%d, nw_Sign=%d,value=0x%x\n",sim_Sign,nw_Sign,value);
	ret = cfg_write_8("emod", "EMOD_SIGN", value);
	if(ERR_SUCCESS != ret)
		CSW_TRACE(CFW_CFG_TS_ID, "in Cfg_Emod_UpdateSign,2, call cfg_write_8 falsed, ret =0x%x\n",ret);

//    CSW_PROFILE_FUNCTION_EXIT(Cfg_Emod_UpdateSign);
	return ret;
}


//]]hameina[+]2006.10.13

