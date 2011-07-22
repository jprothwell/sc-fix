///*************************************************************
//hameina 2006.8.21 modified
//
//notice: this version is based on reg systerm
//
//modify record:
//2006.8.31 hameina[mod] CFW_CfgSimGetSmsParam & CFW_CfgSimGetChvParam reused
///**************************************************************

#include <sul.h>
#include <cfw.h>
#include "cfw_cfg.h"
#include "drv.h"
#include "dm.h"

#ifdef SIM_SWITCH_ENABLE
#include "drv.h"
#include "dm.h"
#endif

#include "csw_debug.h"

//
// Volatile global variables in RAM.
// You don't need to save to flash.
//
#ifdef CFW_MULTI_SIM
PRIVATE SIM_SMS_PARAM  g_SimSmsParam[CFW_SIM_COUNT];
PRIVATE SIM_CHV_PARAM  g_SimChvParam[CFW_SIM_COUNT];
PRIVATE SIM_PBK_PARAM  g_SimPbkParam[CFW_SIM_COUNT];
PRIVATE SIM_MISC_PARAM g_SimMiscParam[CFW_SIM_COUNT];
PRIVATE SIM_SAT_PARAM  g_SimSatParam[CFW_SIM_COUNT];
#else
PRIVATE SIM_SMS_PARAM  g_SimSmsParam;
PRIVATE SIM_CHV_PARAM  g_SimChvParam;
PRIVATE SIM_PBK_PARAM  g_SimPbkParam;// this value is used, confirmed by wangxiaojin hameina[+]2006.8.23
PRIVATE SIM_MISC_PARAM g_SimMiscParam;
PRIVATE SIM_SAT_PARAM  g_SimSatParam;
#endif
UINT32 CFW_CfgSimParamsInit()
{
#ifdef CFW_MULTI_SIM
    SUL_ZeroMemory8(g_SimSmsParam,  CFW_SIM_COUNT*SIZEOF(SIM_SMS_PARAM));
    SUL_ZeroMemory8(g_SimChvParam,  CFW_SIM_COUNT*SIZEOF(SIM_CHV_PARAM));
    SUL_ZeroMemory8(g_SimPbkParam,  CFW_SIM_COUNT*SIZEOF(SIM_PBK_PARAM));
    SUL_ZeroMemory8(g_SimMiscParam, CFW_SIM_COUNT*SIZEOF(SIM_MISC_PARAM));
    SUL_ZeroMemory8(g_SimSatParam,  CFW_SIM_COUNT*SIZEOF(SIM_SAT_PARAM));	
#else
    SUL_ZeroMemory8(&g_SimSmsParam,  SIZEOF(SIM_SMS_PARAM));
    SUL_ZeroMemory8(&g_SimChvParam,  SIZEOF(SIM_CHV_PARAM));
    SUL_ZeroMemory8(&g_SimPbkParam,  SIZEOF(SIM_PBK_PARAM));
    SUL_ZeroMemory8(&g_SimMiscParam, SIZEOF(SIM_MISC_PARAM));
    SUL_ZeroMemory8(&g_SimSatParam,  SIZEOF(SIM_SAT_PARAM));	
#endif

    return ERR_SUCCESS;
}
#ifdef CFW_MULTI_SIM

UINT32 CFW_CfgSimGetSmsParam(SIM_SMS_PARAM** pSimSmsParam,CFW_SIM_ID nSimID)
{
	UINT32 nRet = ERR_SUCCESS;
	
	nRet = CFW_CheckSimId(nSimID);
	if(ERR_SUCCESS == nRet)
	{
	    *pSimSmsParam = &(g_SimSmsParam[nSimID]);
	}
	else
	{
		CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSimGetSmsParam, error ret =0x%x\n", nRet);
	}

    return nRet;
}

UINT32 CFW_CfgSimGetChvParam(SIM_CHV_PARAM** pSimChvParam,
	                         CFW_SIM_ID nSimID)
{
	UINT32 nRet = ERR_SUCCESS;
	
	nRet = CFW_CheckSimId(nSimID);
	if(ERR_SUCCESS == nRet)
	{
	    *pSimChvParam = &(g_SimChvParam[nSimID]);
	}
	else
	{
		CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSimGetChvParam, error ret =0x%x\n", nRet);
	}
    return nRet;
}

UINT32 CFW_CfgSimGetPbkParam(SIM_PBK_PARAM** pSimPbkParam,
	                         CFW_SIM_ID nSimID)
{
	UINT32 nRet = ERR_SUCCESS;
	
	nRet = CFW_CheckSimId(nSimID);
	if(ERR_SUCCESS == nRet)
	{
	    *pSimPbkParam = &(g_SimPbkParam[nSimID]);
	}
	else
	{
		CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSimGetPbkParam, error ret =0x%x\n", nRet);
	}
    return nRet;
}


UINT32 CFW_CfgSimGetMicParam(SIM_MISC_PARAM** pSimMicParam,
	                         CFW_SIM_ID nSimID)
{
	UINT32 nRet = ERR_SUCCESS;
	
	nRet = CFW_CheckSimId(nSimID);
	if(ERR_SUCCESS == nRet)
	{
	    *pSimMicParam = &(g_SimMiscParam[nSimID]);
	}
	else
	{
		CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSimGetMicParam, error ret =0x%x\n", nRet);
	}
    return nRet;
}


UINT32 CFW_CfgSimGetSatParam(SIM_SAT_PARAM** pSimSatParam,
	                         CFW_SIM_ID nSimID)
{
	UINT32 nRet = ERR_SUCCESS;
	
	nRet = CFW_CheckSimId(nSimID);
	if(ERR_SUCCESS == nRet)
	{
	    *pSimSatParam = &(g_SimSatParam[nSimID]);
	}
	else
	{
		CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSimGetSatParam, error ret =0x%x\n", nRet);
	}
    return nRet;
}

#else

UINT32 CFW_CfgSimGetSmsParam(SIM_SMS_PARAM** pSimSmsParam)
{
    *pSimSmsParam = &g_SimSmsParam;
    return ERR_SUCCESS;
}

UINT32 CFW_CfgSimGetChvParam(SIM_CHV_PARAM** pSimChvParam)
{
    *pSimChvParam = &g_SimChvParam;
    
    return ERR_SUCCESS;
}

UINT32 CFW_CfgSimGetPbkParam(SIM_PBK_PARAM** pSimPbkParam)
{
    *pSimPbkParam = &g_SimPbkParam;
    
    return ERR_SUCCESS;
}


UINT32 CFW_CfgSimGetMicParam(SIM_MISC_PARAM** pSimMicParam)
{
    *pSimMicParam = &g_SimMiscParam;
    
    return ERR_SUCCESS;
}


UINT32 CFW_CfgSimGetSatParam(SIM_SAT_PARAM** pSim_SatParam)
{
    *pSim_SatParam = &g_SimSatParam;
    return(ERR_SUCCESS);

}

#endif

#ifdef SIM_SWITCH_ENABLE
//==============================================================================
//Sim Switch
//==============================================================================
UINT32 SRVAPI CFW_CfgSetSimSwitch(UINT8 n)
{
  UINT32 ret = ERR_SUCCESS;

  CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSimSwitch);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSimSwitch,  n=0x%x\n", n);
  ret = CFW_CfgNumRangeCheck(n, SIMCARD_SWITCH_RABGE);
  if (ERR_SUCCESS == ret)
  {

    ret = cfg_write_8("sim", "SimCardSwitch", n);

  }
  CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSimSwitch);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSimSwitch,  ret=0x%x\n", ret);
  return ret;
}

UINT32 SRVAPI CFW_CfgGetSimSwitch(UINT8* n)
{
  UINT32 ret    = ERR_SUCCESS;
  BOOL bDefault = FALSE;

  CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetSimSwitch);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSimSwitch enter\n");
  ret = cfg_read_8("sim", "SimCardSwitch", n);
  if (ERR_SUCCESS != ret)
  {
	CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8,  ret=0x%x\n", ret);
    ret = cfg_readdefault("sim", "SimCardSwitch", n);
    if (ERR_SUCCESS != ret)
    {
	CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault,  ret=0x%x\n", ret);
      CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetClir);
      return ret;
    }
    bDefault = TRUE;
  }
  else
  {
    ret = CFW_CfgNumRangeCheck(*n, SIMCARD_SWITCH_RABGE);
    if (ERR_SUCCESS != ret)
    {
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck,  ret=0x%x\n", ret);
      ret = cfg_readdefault("sim", "SimCardSwitch", n);
      if (ERR_SUCCESS != ret)
      {
	CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault,  ret=0x%x\n", ret);
        CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetClir);
        return ret;
      }
      bDefault = TRUE;
    }
  }

  if (bDefault)
    cfg_write_8("sim", "SimCardSwitch", *n);
  CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSimSwitch);
  	CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault,  *n=0x%x\n", *n);
  return ERR_SUCCESS;

}

#if 0
#define SIM_1 1 //simcard 1
#define SIM_2 2 //simcard 2
VOID SimSwitch(UINT8 SimNum)
{
  DRV_GpioSetMode(DM_DEVID_GPIO_7,1);
  DRV_GpioSetDirection(DM_DEVID_GPIO_7,DRV_GPIO_OUT);
  	CSW_TRACE(CFW_CFG_TS_ID, "SimSwitch,  SimNum=0x%x\n", SimNum);
  {
    DRV_GpioSetLevel(DM_DEVID_GPIO_7,DRV_GPIO_LOW);
  }
  if (SIM_2 == SimNum)
  {
    DRV_GpioSetLevel(DM_DEVID_GPIO_7,DRV_GPIO_HIGH);
  }
}
#endif
 #endif


 
