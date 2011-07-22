///*************************************************************
//hameina 2006.8.21 modified
//
//notice: this version is based on reg systerm
//
//modify record:
//2006.9.5 enabled CFW_CfgSetNwOperDispFormat, this value was saved in a globle
//2006.9.9 delete useless code in this file, add some trace
//2006.9.14 hameina [mod] ERR_CFW_NOTSUPPORT->ERR_CFW_NOT_SUPPORT, who define this
//    wrong err code murdered hameina.
//2006.9.22 hameina [mod] getvalue() the type of param datasize changed:UINT32->UINT8
//2006.9.29 hameina [mod] cfg_readdefault() the type of function changed:bool->UINT32
///**************************************************************

#include <sul.h>
#include <cfw.h>
#include "csw_debug.h"

#include "cfw_cfg.h"
#include <string.h>
UINT8 g_nNwDispFormat = 0;

#ifdef CFW_MULTI_SIM

UINT32 CFW_CfgSetNwStatus (UINT8 n,CFW_SIM_ID nSimID)
{   
	UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetNwStatus);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetNwStatus, n=0x%x, nSimID=0x%x\n", n, nSimID);
	ret = CFW_CfgNumRangeCheck(n, NW_REG_RANGE);
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, ret=0x%x\n", ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetNwStatus);
		return ret;
	}
	if(CFW_SIM_0==nSimID)
	{
		MemoryInfo.NwStatus0= n;
	}
	else if(CFW_SIM_1==nSimID)
	{
		MemoryInfo.NwStatus1= n;
	}	
	else if((CFW_SIM_1+1)==nSimID)
      {
		MemoryInfo.NwStatus2= n;
	}
	else if((CFW_SIM_1+2)==nSimID)
      {
		MemoryInfo.NwStatus3= n;
	}	
	else
	{
		ret=ERR_CFW_INVALID_PARAMETER;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetNwStatus);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetNwStatus exit\n");
	return ret;
}

UINT32 CFW_CfgGetNwStatus (UINT8 *n,CFW_SIM_ID nSimID)
{   
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetNwStatus, nSimID=0x%x\n", nSimID);
	if(CFW_SIM_0==nSimID)
	{
		*n = MemoryInfo.NwStatus0;
	}
	else if(CFW_SIM_1==nSimID)
	{
		*n = MemoryInfo.NwStatus1;
	}
	else if((CFW_SIM_1+1)==nSimID)
	{
		*n = MemoryInfo.NwStatus2;
	}
	else if((CFW_SIM_1+2)==nSimID)
	{
		*n = MemoryInfo.NwStatus3;
	}
	else
	{
	    return ERR_CFW_INVALID_PARAMETER;
	}
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetNwStatus, *n=0x%x\n", *n);
       return ERR_SUCCESS;
}
UINT32 CFW_CfgSetStoredPlmnList(CFW_StoredPlmnList* pPlmnL, CFW_SIM_ID nSimId)
{
  UINT32 ret = ERR_SUCCESS;

  CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetStoredPlmnList, nSimID=0x%x\n", nSimId);
  CSW_TRACE(CFW_CFG_TS_ID, "pPlmnL->nCount=0x%x\n", pPlmnL->nCount);
  CSW_TC_MEMBLOCK(CFW_CFG_TS_ID, (UINT8*)pPlmnL->ArfcnL, 64, 16);
  if (pPlmnL != NULL)
  {
    if (CFW_SIM_0 == nSimId)
    {
      u8 temp     = 0;
      u8 tPlmn[3] = { 0, };
      u16 tempL[32] = {0,}; 
      cfg_read_hex("nw", "NW_SIM0_PLMN", tPlmn, 3);
      cfg_read_hex("nw", "NW_SIM0_ARFCN_LIST", tempL, 64);
      cfg_read_8("nw", "NW_SIM0_BA_COUNT", &temp);
      ret = cfg_write_hex("nw", "NW_SIM0_PLMN", pPlmnL->Plmn, 3);
      if (ERR_SUCCESS != ret)
      {
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_wirte_hex, error ret=0x%x\n", ret);
        return ret;
      }
      ret = cfg_write_8("nw", "NW_SIM0_BA_COUNT", pPlmnL->nCount);
      if (ERR_SUCCESS != ret)
      {
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_wirte_8, error ret=0x%x\n", ret);
        cfg_write_hex("nw", "NW_SIM0_PLMN", tPlmn, 3);
        return ret;
      }
      ret = cfg_write_hex("nw", "NW_SIM0_ARFCN_LIST", pPlmnL->ArfcnL, 64);
      if (ERR_SUCCESS != ret)
      {
        cfg_write_hex("nw", "NW_SIM0_PLMN", tPlmn, 3);
        cfg_write_8("nw", "NW_SIM0_BA_COUNT", temp);
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_hex, error ret=0x%x\n", ret);
        return ret;
      }

      if(pPlmnL->nCount == CFW_STORE_MAX_COUNT)
      {
          ret = cfg_write_hex("nw", "NW_SIM0_STORE_LAST", &(pPlmnL->ArfcnL[CFW_STORE_MAX_COUNT-1]), 2);
          if (ERR_SUCCESS != ret)
          {
              cfg_write_hex("nw", "NW_SIM0_PLMN", tPlmn, 3);
              cfg_write_8("nw", "NW_SIM0_BA_COUNT", temp);
	       cfg_write_hex("nw", "NW_SIM0_ARFCN_LIST", tempL,64);
              CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_hex, error ret=0x%x\n", ret);
              return ret;
          }
      }
    }
    else if (CFW_SIM_1 == nSimId)
    {
      u8 temp     = 0;
      u8 tPlmn[3] = { 0, };
      u16 tempL[32] = {0,}; 
      cfg_read_hex("nw", "NW_SIM1_ARFCN_LIST", tempL, 64);
      cfg_read_hex("nw", "NW_SIM1_PLMN", tPlmn, 3);
      cfg_read_8("nw", "NW_SIM1_BA_COUNT", &temp);
      ret = cfg_write_hex("nw", "NW_SIM1_PLMN", pPlmnL->Plmn, 3);
      if (ERR_SUCCESS != ret)
      {
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_wirte_hex, error ret=0x%x\n", ret);
        return ret;
      }
      ret = cfg_write_8("nw", "NW_SIM1_BA_COUNT", pPlmnL->nCount);
      if (ERR_SUCCESS != ret)
      {
        cfg_write_hex("nw", "NW_SIM1_PLMN", tPlmn, 3);
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_8, error ret=0x%x\n", ret);
        return ret;
      }
      ret = cfg_write_hex("nw", "NW_SIM1_ARFCN_LIST", pPlmnL->ArfcnL, 64);
      if (ERR_SUCCESS != ret)
      {
        cfg_write_hex("nw", "NW_SIM1_PLMN", tPlmn, 3);
        cfg_write_8("nw", "NW_SIM1_BA_COUNT", temp);
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_hex, error ret=0x%x\n", ret);
        return ret;
      }

      if(pPlmnL->nCount == CFW_STORE_MAX_COUNT)
      {
          ret = cfg_write_hex("nw", "NW_SIM1_STORE_LAST", &(pPlmnL->ArfcnL[CFW_STORE_MAX_COUNT-1]), 2);
          if (ERR_SUCCESS != ret)
          {
              cfg_write_hex("nw", "NW_SIM1_PLMN", tPlmn, 3);
              cfg_write_8("nw", "NW_SIM1_BA_COUNT", temp);
	       cfg_write_hex("nw", "NW_SIM1_AFRCN_LIST", tempL,64);
              CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_hex, error ret=0x%x\n", ret);
              return ret;
          }
      }
    }
    else if (( CFW_SIM_1 + 1 ) == nSimId)
    {
      u8 temp     = 0;
      u8 tPlmn[3] = { 0, };
      u16 tempL[32] = {0,}; 
      cfg_read_hex("nw", "NW_SIM2_ARFCN_LIST", tempL, 64);
      cfg_read_hex("nw", "NW_SIM2_PLMN", tPlmn, 3);
      cfg_read_8("nw", "NW_SIM2_BA_COUNT", &temp);
      ret = cfg_write_hex("nw", "NW_SIM2_PLMN", pPlmnL->Plmn, 3);
      if (ERR_SUCCESS != ret)
      {
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_wirte_hex, error ret=0x%x\n", ret);
        return ret;
      }
      ret = cfg_write_8("nw", "NW_SIM2_BA_COUNT", pPlmnL->nCount);
      if (ERR_SUCCESS != ret)
      {
        cfg_write_hex("nw", "NW_SIM2_PLMN", tPlmn, 3);
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_8, error ret=0x%x\n", ret);
        return ret;
      }
      ret = cfg_write_hex("nw", "NW_SIM2_ARFCN_LIST", pPlmnL->ArfcnL, 64);
      if (ERR_SUCCESS != ret)
      {
        cfg_write_hex("nw", "NW_SIM2_PLMN", tPlmn, 3);
        cfg_write_8("nw", "NW_SIM2_BA_COUNT", temp);
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_hex, error ret=0x%x\n", ret);
        return ret;
      }

      if(pPlmnL->nCount == CFW_STORE_MAX_COUNT)
      {
          ret = cfg_write_hex("nw", "NW_SIM2_STORE_LAST", &(pPlmnL->ArfcnL[CFW_STORE_MAX_COUNT-1]), 2);
          if (ERR_SUCCESS != ret)
          {
              cfg_write_hex("nw", "NW_SIM2_PLMN", tPlmn, 3);
              cfg_write_8("nw", "NW_SIM2_BA_COUNT", temp);
	       cfg_write_hex("nw", "NW_SIM2_AFRCN_LIST", tempL,64);
              CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_hex, error ret=0x%x\n", ret);
              return ret;
          }
      }
    }
    else if (( CFW_SIM_1 + 2 ) == nSimId)
    {
      u8 temp     = 0;
      u8 tPlmn[3] = { 0, };
      u16 tempL[32] = {0,}; 
      cfg_read_hex("nw", "NW_SIM3_ARFCN_LIST", tempL, 64);
      cfg_read_hex("nw", "NW_SIM3_PLMN", tPlmn, 3);
      cfg_read_8("nw", "NW_SIM3_BA_COUNT", &temp);
      ret = cfg_write_hex("nw", "NW_SIM3_PLMN", pPlmnL->Plmn, 3);
      if (ERR_SUCCESS != ret)
      {
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_wirte_hex, error ret=0x%x\n", ret);
        return ret;
      }
      ret = cfg_write_8("nw", "NW_SIM3_BA_COUNT", pPlmnL->nCount);
      if (ERR_SUCCESS != ret)
      {
        cfg_write_hex("nw", "NW_SIM3_PLMN", tPlmn, 3);
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_8, error ret=0x%x\n", ret);
        return ret;
      }
      ret = cfg_write_hex("nw", "NW_SIM3_ARFCN_LIST", pPlmnL->ArfcnL, 64);
      if (ERR_SUCCESS != ret)
      {
        cfg_write_hex("nw", "NW_SIM3_PLMN", tPlmn, 3);
        cfg_write_8("nw", "NW_SIM1_BA_COUNT", temp);
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_hex, error ret=0x%x\n", ret);
        return ret;
      }

      if(pPlmnL->nCount == CFW_STORE_MAX_COUNT)
      {
          ret = cfg_write_hex("nw", "NW_SIM3_STORE_LAST", &(pPlmnL->ArfcnL[CFW_STORE_MAX_COUNT-1]), 2);
          if (ERR_SUCCESS != ret)
          {
              cfg_write_hex("nw", "NW_SIM3_PLMN", tPlmn, 3);
              cfg_write_8("nw", "NW_SIM3_BA_COUNT", temp);
	       cfg_write_hex("nw", "NW_SIM3_AFRCN_LIST", tempL,64);
              CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_hex, error ret=0x%x\n", ret);
              return ret;
          }
      }
    }
    else
    {
      ret = ERR_CFW_INVALID_PARAMETER;
    }
  }
  CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetStoredPlmnList Success\n");

  return ret;
}

UINT32 CFW_CfgGetStoredPlmnList(CFW_StoredPlmnList* pPlmnL, CFW_SIM_ID nSimId)
{
  UINT32 ret = ERR_SUCCESS;

  CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetStoredPlmnList, nSimID=0x%x\n", nSimId);
  if (pPlmnL != NULL)
  {
    if (CFW_SIM_0 == nSimId)
    {
      ret = cfg_read_hex("nw", "NW_SIM0_PLMN", pPlmnL->Plmn, 3);
      if (ERR_SUCCESS != ret)
      {
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret=0x%x\n", ret);
        ret = cfg_readdefault("nw", "NW_SIM0_PLMN", pPlmnL->Plmn);
        if (ERR_SUCCESS != ret)
        {
          CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n", ret);
          return ret;
        }
      }
      ret = cfg_read_8("nw", "NW_SIM0_BA_COUNT", &(pPlmnL->nCount));
      if (ERR_SUCCESS != ret)
      {
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret=0x%x\n", ret);
        ret = cfg_readdefault("nw", "NW_SIM0_BA_COUNT", &(pPlmnL->nCount));
        if (ERR_SUCCESS != ret)
        {
          CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n", ret);
          pPlmnL->nCount = 0;
          return ret;
        }
      }
	  
      if (pPlmnL->nCount > CFW_STORE_MAX_COUNT)
      {
           CSW_TRACE(CFW_CFG_TS_ID, "Sim 0 Store Monitor List Nb is %d, beyond 33\n", pPlmnL->nCount);
	    pPlmnL->nCount = 0;
	    return ERR_CFG_READ_REG_FAILURE;
      }
	  	
      ret = cfg_read_hex("nw", "NW_SIM0_ARFCN_LIST", pPlmnL->ArfcnL, 64);
      if (ERR_SUCCESS != ret)
      {
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret=0x%x\n", ret);
        ret = cfg_readdefault("nw", "NW_SIM0_ARFCN_LIST", pPlmnL->ArfcnL);

        if (ERR_SUCCESS != ret)
        {
          CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n", ret);
          pPlmnL->nCount = 0;
          return ret;
        }
      }

      if (pPlmnL->nCount == CFW_STORE_MAX_COUNT)
      {
          ret = cfg_read_hex("nw", "NW_SIM0_STORE_LAST",  &(pPlmnL->ArfcnL[CFW_STORE_MAX_COUNT-1]), 2);
          if (ERR_SUCCESS != ret)
          {
               CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret=0x%x\n", ret);
               ret = cfg_readdefault("nw", "NW_SIM0_STORE_LAST",  &(pPlmnL->ArfcnL[CFW_STORE_MAX_COUNT-1]));

               if (ERR_SUCCESS != ret)
               {
                   CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n", ret);
                   pPlmnL->nCount = 0;
                   return ret;
               }
           }
      }
    }
    else if (CFW_SIM_1 == nSimId)
    {
      ret = cfg_read_hex("nw", "NW_SIM1_PLMN", pPlmnL->Plmn, 3);
      if (ERR_SUCCESS != ret)
      {
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret=0x%x\n", ret);
        ret = cfg_readdefault("nw", "NW_SIM1_PLMN", pPlmnL->Plmn);
        if (ERR_SUCCESS != ret)
        {
          CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n", ret);
          return ret;
        }
      }
      ret = cfg_read_8("nw", "NW_SIM1_BA_COUNT", &(pPlmnL->nCount));
      if (ERR_SUCCESS != ret)
      {
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret=0x%x\n", ret);
        ret = cfg_readdefault("nw", "NW_SIM1_BA_COUNT", &(pPlmnL->nCount));
        if (ERR_SUCCESS != ret)
        {
          CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n", ret);
          pPlmnL->nCount = 0;
          return ret;
        }
      }

      if (pPlmnL->nCount > CFW_STORE_MAX_COUNT)
      {
           CSW_TRACE(CFW_CFG_TS_ID, "Sim 1 Store Monitor List Nb is %d, beyond 33\n", pPlmnL->nCount);
	    pPlmnL->nCount = 0;
	    return ERR_CFG_READ_REG_FAILURE;
      }
	  
      ret = cfg_read_hex("nw", "NW_SIM1_ARFCN_LIST", pPlmnL->ArfcnL, 64);
      if (ERR_SUCCESS != ret)
      {
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret=0x%x\n", ret);
        ret = cfg_readdefault("nw", "NW_SIM1_ARFCN_LIST", pPlmnL->ArfcnL);

        if (ERR_SUCCESS != ret)
        {
          CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n", ret);
          pPlmnL->nCount = 0;
          return ret;
        }
      }

       if (pPlmnL->nCount == CFW_STORE_MAX_COUNT)
       {
          ret = cfg_read_hex("nw", "NW_SIM1_STORE_LAST", &(pPlmnL->ArfcnL[CFW_STORE_MAX_COUNT-1]), 2);
          if (ERR_SUCCESS != ret)
          {
               CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret=0x%x\n", ret);
               ret = cfg_readdefault("nw", "NW_SIM1_STORE_LAST",  &(pPlmnL->ArfcnL[CFW_STORE_MAX_COUNT-1]));

               if (ERR_SUCCESS != ret)
               {
                   CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n", ret);
                   pPlmnL->nCount = 0;
                   return ret;
               }
           }
      }
	   
    }
    else if (( CFW_SIM_1 + 1 )== nSimId)
    {
      ret = cfg_read_hex("nw", "NW_SIM2_PLMN", pPlmnL->Plmn, 3);
      if (ERR_SUCCESS != ret)
      {
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret=0x%x\n", ret);
        ret = cfg_readdefault("nw", "NW_SIM2_PLMN", pPlmnL->Plmn);
        if (ERR_SUCCESS != ret)
        {
          CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n", ret);
          return ret;
        }
      }
      ret = cfg_read_8("nw", "NW_SIM2_BA_COUNT", &(pPlmnL->nCount));
      if (ERR_SUCCESS != ret)
      {
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret=0x%x\n", ret);
        ret = cfg_readdefault("nw", "NW_SIM2_BA_COUNT", &(pPlmnL->nCount));
        if (ERR_SUCCESS != ret)
        {
          CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n", ret);
          pPlmnL->nCount = 0;
          return ret;
        }
      }

      if (pPlmnL->nCount > CFW_STORE_MAX_COUNT)
      {
           CSW_TRACE(CFW_CFG_TS_ID, "Sim 2 Store Monitor List Nb is %d, beyond 33\n", pPlmnL->nCount);
	    pPlmnL->nCount = 0;
	    return ERR_CFG_READ_REG_FAILURE;
      }
	  
      ret = cfg_read_hex("nw", "NW_SIM2_ARFCN_LIST", pPlmnL->ArfcnL, 64);
      if (ERR_SUCCESS != ret)
      {
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret=0x%x\n", ret);
        ret = cfg_readdefault("nw", "NW_SIM2_ARFCN_LIST", pPlmnL->ArfcnL);

        if (ERR_SUCCESS != ret)
        {
          CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n", ret);
          pPlmnL->nCount = 0;
          return ret;
        }
      }

       if (pPlmnL->nCount == CFW_STORE_MAX_COUNT)
       {
          ret = cfg_read_hex("nw", "NW_SIM2_STORE_LAST", &(pPlmnL->ArfcnL[CFW_STORE_MAX_COUNT-1]), 2);
          if (ERR_SUCCESS != ret)
          {
               CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret=0x%x\n", ret);
               ret = cfg_readdefault("nw", "NW_SIM2_STORE_LAST",  &(pPlmnL->ArfcnL[CFW_STORE_MAX_COUNT-1]));

               if (ERR_SUCCESS != ret)
               {
                   CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n", ret);
                   pPlmnL->nCount = 0;
                   return ret;
               }
           }
      }
	   
    }
    else if (( CFW_SIM_1 + 2 )== nSimId)
    {
      ret = cfg_read_hex("nw", "NW_SIM3_PLMN", pPlmnL->Plmn, 3);
      if (ERR_SUCCESS != ret)
      {
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret=0x%x\n", ret);
        ret = cfg_readdefault("nw", "NW_SIM3_PLMN", pPlmnL->Plmn);
        if (ERR_SUCCESS != ret)
        {
          CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n", ret);
          return ret;
        }
      }
      ret = cfg_read_8("nw", "NW_SIM3_BA_COUNT", &(pPlmnL->nCount));
      if (ERR_SUCCESS != ret)
      {
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret=0x%x\n", ret);
        ret = cfg_readdefault("nw", "NW_SIM3_BA_COUNT", &(pPlmnL->nCount));
        if (ERR_SUCCESS != ret)
        {
          CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n", ret);
          pPlmnL->nCount = 0;
          return ret;
        }
      }

      if (pPlmnL->nCount > CFW_STORE_MAX_COUNT)
      {
           CSW_TRACE(CFW_CFG_TS_ID, "Sim 3 Store Monitor List Nb is %d, beyond 33\n", pPlmnL->nCount);
	    pPlmnL->nCount = 0;
	    return ERR_CFG_READ_REG_FAILURE;
      }
	  
      ret = cfg_read_hex("nw", "NW_SIM3_ARFCN_LIST", pPlmnL->ArfcnL, 64);
      if (ERR_SUCCESS != ret)
      {
        CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret=0x%x\n", ret);
        ret = cfg_readdefault("nw", "NW_SIM3_ARFCN_LIST", pPlmnL->ArfcnL);

        if (ERR_SUCCESS != ret)
        {
          CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n", ret);
          pPlmnL->nCount = 0;
          return ret;
        }
      }

       if (pPlmnL->nCount == CFW_STORE_MAX_COUNT)
       {
          ret = cfg_read_hex("nw", "NW_SIM3_STORE_LAST", &(pPlmnL->ArfcnL[CFW_STORE_MAX_COUNT-1]), 2);
          if (ERR_SUCCESS != ret)
          {
               CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret=0x%x\n", ret);
               ret = cfg_readdefault("nw", "NW_SIM3_STORE_LAST",  &(pPlmnL->ArfcnL[CFW_STORE_MAX_COUNT-1]));

               if (ERR_SUCCESS != ret)
               {
                   CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n", ret);
                   pPlmnL->nCount = 0;
                   return ret;
               }
           }
      }
	   
    }
    else
    {
      ret = ERR_CFW_INVALID_PARAMETER;
    }
  }
  CSW_TRACE(CFW_CFG_TS_ID, "pPlmnL->nCount=0x%x\n", pPlmnL->nCount);
  CSW_TC_MEMBLOCK(CFW_CFG_TS_ID, (UINT8*)pPlmnL->ArfcnL, 64, 16);

  CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetStoredPlmnList Success\n");

  return ret;
}

UINT32 CFW_CfgNwSetFrequencyBand ( UINT8 nBand, CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgNwSetFrequencyBand);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwSetFrequencyBand, nBand =0x%x, nSimID=0x%x\n", nBand, nSimID);
	if(nBand>0)
	{

		 INT8 simid_c[2]  = {0x30,0x00};
	       INT8 name_s[19] = {0x00,};
		INT8 name_s1[11] = {0x00,};

		strcpy(name_s,"NW_SIM");
		strcpy(name_s1,"_FREQ_BAND");
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			strcat(name_s, name_s1);
			ret = cfg_write_8("nw", name_s, nBand);

		}else{
		       ret = ERR_CFW_INVALID_PARAMETER;
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
			return ret;
		}
	    
	}
	else
		ret = ERR_CFG_PARAM_OUT_RANGR;
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwSetFrequencyBand);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwSetFrequencyBand exit\n");
	return ret;

}

UINT32 CFW_CfgNwGetFrequencyBand (
  UINT8* pBand,
  CFW_SIM_ID nSimID
)
{
	UINT32 ret = ERR_SUCCESS;
	BOOL bDef = FALSE;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgNwGetFrequencyBand);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwGetFrequencyBand, nSimID=0x%x\n", nSimID);
     INT8 simid_c[2]  = {0x30,0x00};
	       INT8 name_s[19] = {0x00,};
		INT8 name_s1[11] = {0x00,};
	if(pBand!=NULL)
	{

	      

		strcpy(name_s,"NW_SIM");
		strcpy(name_s1,"_FREQ_BAND");
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			strcat(name_s, name_s1);
			ret = cfg_read_8("nw", name_s, pBand);
		}else{
		       ret = ERR_CFW_INVALID_PARAMETER;
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
			return ret;
		}
	
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgNwGetFrequencyBand,1, call function cfg_read_8 failed,ret=0x%x\n",ret);
			
			ret = cfg_readdefault("nw", name_s, pBand);
			
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgNwGetFrequencyBand,1, call function cfg_readdefault failed,ret=0x%x\n",ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetFrequencyBand);
				return ret;
			}
			bDef = TRUE;
		}
		else
		{
			if(!(*pBand))
			{
				ret = cfg_readdefault("nw", name_s, pBand);

				if(ERR_SUCCESS != ret)
				{
					CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgNwGetFrequencyBand,2, call function cfg_readdefault failed,ret=0x%x\n",ret);
					CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetFrequencyBand);
					return ret;
				}
				bDef = TRUE;
			}
		}
	}

	if(bDef)
	{
		ret = cfg_read_8("nw", name_s, pBand);

	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetFrequencyBand);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetNwStatus, *pBand =0x%x\n", *pBand);
	return ret;
}

UINT32 CFW_CfgNwSetNetWorkMode(UINT8 nMode, CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgNwSetNetWorkMode);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwSetNetWorkMode, nMode=0x%x, nSimID=0x%x\n", nMode, nSimID);
	ret = CFW_CfgNumRangeCheck(nMode, NW_WORKMODE_RANGE);

	if(ERR_SUCCESS == ret)
  	{
              INT8 simid_c[2]  = {0x30,0x00};
	       INT8 name_s[19] = {0x00,};
		INT8 name_s1[11] = {0x00,};

		strcpy(name_s,"NW_SIM");
		strcpy(name_s1,"_WORK_MODE");
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			strcat(name_s, name_s1);
			ret = cfg_write_8("nw", name_s, nMode);

		}else{
		       ret = ERR_CFW_INVALID_PARAMETER;
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
			return ret;
		}
	    
	}
	else
	{
		ret = ERR_CFW_INVALID_PARAMETER;
	}
    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwSetNetWorkMode);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwSetNetWorkMode exit\n");
	return ret;
}

UINT32 CFW_CfgNwGetNetWorkMode(UINT8 *pMode, CFW_SIM_ID nSimID)
{
	UINT32 ret= ERR_SUCCESS;
	BOOL bDef = FALSE;
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwGetNetWorkMode, nSimID=0x%x\n", nSimID);
	 INT8 simid_c[2]  = {0x30,0x00};
	       INT8 name_s[19] = {0x00,};
		INT8 name_s1[11] = {0x00,};

	if(NULL == pMode)
	{
		return ERR_CFW_INVALID_PARAMETER;
	}
	else
	{
		CSW_PROFILE_FUNCTION_ENTER(CFW_CfgNwGetNetWorkMode);
		 
		strcpy(name_s,"NW_SIM");
		strcpy(name_s1,"_WORK_MODE");
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			strcat(name_s, name_s1);
			ret = cfg_read_8("nw", name_s, pMode);

		}else{
		       ret = ERR_CFW_INVALID_PARAMETER;
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
			return ret;
		}
		
		
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret=0x%x\n", ret);
			
			    ret = cfg_readdefault("nw", name_s, pMode);
			
			if(ERR_SUCCESS != ret)
			{
			    CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
			    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetNetWorkMode);
			    return ret;
			}
			bDef = TRUE;
		}
		else
		{
			ret = CFW_CfgNumRangeCheck(*pMode, NW_WORKMODE_RANGE);
			
			if(ERR_SUCCESS != ret)
			{
				ret = cfg_readdefault("nw", name_s, pMode);

			
				if(ERR_SUCCESS != ret)
				{
					CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
					CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetNetWorkMode);
					return ret;
				}
				bDef = TRUE;
			}
		}
	}
	if(bDef)
	{
		
			ret = cfg_write_8("nw", name_s, *pMode);
		
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetNetWorkMode);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwGetNetWorkMode, *pMode =0x%x\n", *pMode);
	return ret;
	
}
#ifdef CFW_GPRS_SUPPORT

UINT32 CFW_CfgGprsSetPdpCxt(UINT8 nCid, CFW_GPRS_PDPCONT_INFO *pPdpCont, CFW_SIM_ID nSimID)
{
	UINT32  ret = ERR_SUCCESS;
	UINT8 pdpType[REG_VALUENAME_LEN_MAX+1];
	UINT8 pdpDCmp[REG_VALUENAME_LEN_MAX+1];
	UINT8 pdpHCmp[REG_VALUENAME_LEN_MAX+1];
	UINT8 pdpAPN[REG_VALUENAME_LEN_MAX+1];
	UINT8 pdpAdr[REG_VALUENAME_LEN_MAX+1];
	UINT8 *Cid = 0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGprsSetPdpCxt);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsSetPdpCxt, nCid=0x%x, nSimID=0x%x\n", nCid, nSimID);
	ret = CFW_CfgNumRangeCheck(nCid, GPRS_CIND_RANGE);
	if(ret!= ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret=0x%x\n",ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetPdpCxt);
		return ERR_CFG_PARAM_OUT_RANGR;
	}
	if(pPdpCont == NULL)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetPdpCxt);
		return ERR_CFW_INVALID_PARAMETER;
	}
	if((pPdpCont->nPdpType > 2) 
		|| (pPdpCont->nDComp > 3) 
		|| (pPdpCont->nHComp > 4)
		|| (pPdpCont->nApnSize > 100)
		|| (pPdpCont->nPdpAddrSize > 18))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetPdpCxt);
		return ERR_CFG_PARAM_OUT_RANGR;
	}
	nCid = nCid + 0x30;
	Cid = &nCid;

	//PDP_TYPE 
	if(CFW_SIM_0==nSimID)
	{
		SUL_StrCopy(pdpType, "PDP_SIM0_TYPE");
	}
	else if (CFW_SIM_1==nSimID)
	{
		SUL_StrCopy(pdpType, "PDP_SIM1_TYPE");
	}
	else
	{
		return ERR_CFW_INVALID_PARAMETER;
	}
	SUL_StrCat(pdpType, Cid);
	pdpType[9] = '\0';

	//PDP_DATA_C 
	if(CFW_SIM_0==nSimID)
	{
		SUL_StrCopy(pdpDCmp, "PDP_SIM0_DATA_C");
	}
	else if (CFW_SIM_1==nSimID)
	{
		SUL_StrCopy(pdpDCmp, "PDP_SIM1_DATA_C");
	}
	else
	{
		return ERR_CFW_INVALID_PARAMETER;
	}
	SUL_StrCat(pdpDCmp, Cid);
	pdpDCmp[11] = '\0';

	//PDP_HEAD_C 
	if(CFW_SIM_0==nSimID)
	{
		SUL_StrCopy(pdpHCmp, "PDP_SIM0_HEAD_C");
	}
	else if (CFW_SIM_1==nSimID)
	{
		SUL_StrCopy(pdpHCmp, "PDP_SIM1_HEAD_C");
	}
	else
	{
		return ERR_CFW_INVALID_PARAMETER;
	}
	SUL_StrCat(pdpHCmp, Cid);
	pdpHCmp[11] = '\0';

	//PDP_APN 
	if(CFW_SIM_0==nSimID)
	{
		SUL_StrCopy(pdpAPN, "PDP_SIM0_APN");
	}
	else if (CFW_SIM_1==nSimID)
	{
		SUL_StrCopy(pdpAPN, "PDP_SIM1_APN");
	}
	else
	{
		return ERR_CFW_INVALID_PARAMETER;
	}
	SUL_StrCat(pdpAPN, Cid);
	pdpAPN[8] = '\0';
	//PDP_ADDRESS
	if(CFW_SIM_0==nSimID)
	{
		SUL_StrCopy(pdpAdr, "PDP_SIM0_ADR");
	}
	else if (CFW_SIM_1==nSimID)
	{
		SUL_StrCopy(pdpAdr, "PDP_SIM1_ADR");
	}
	else
	{
		return ERR_CFW_INVALID_PARAMETER;
	}
	SUL_StrCat(pdpAdr, Cid);
	pdpAdr[8] = '\0';

	ret = cfg_write_8("PDP", pdpType, pPdpCont->nPdpType);

	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_8 pdpType, error ret=0x%x\n",ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetPdpCxt);
		return ret;
	}
	ret = cfg_write_8("PDP", pdpDCmp, pPdpCont->nDComp);
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_8 pdpDCmp, error ret=0x%x\n",ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetPdpCxt);
		return ret;
	}
	ret = cfg_write_8("PDP", pdpHCmp, pPdpCont->nHComp);
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_8 pdpHCmp, error ret=0x%x\n",ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetPdpCxt);
		return ret;
	}
	ret = cfg_write_str("PDP", pdpAPN, pPdpCont->pApn, pPdpCont->nApnSize);
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_8 pdpAPN, error ret=0x%x\n",ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetPdpCxt);
		return ret;
	}

	ret = cfg_write_str("PDP", pdpAdr, pPdpCont->pPdpAddr, pPdpCont->nPdpAddrSize);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetPdpCxt);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsSetPdpCxt exit\n");
	if(ret != ERR_SUCCESS)
		return ret;
	return ERR_SUCCESS;
}

//=============================================================================
//[in]nCid[1 - 7], [out]pPdpCont
//=============================================================================
UINT32 CFW_CfgGprsGetPdpCxt(UINT8 nCid, CFW_GPRS_PDPCONT_INFO *pPdpCont, CFW_SIM_ID nSimID)
{
	UINT32  ret = ERR_SUCCESS;

	UINT8 pdpType[REG_VALUENAME_LEN_MAX+1];
	UINT8 pdpDCmp[REG_VALUENAME_LEN_MAX+1];
	UINT8 pdpHCmp[REG_VALUENAME_LEN_MAX+1];
	UINT8 pdpAPN[REG_VALUENAME_LEN_MAX+1];
	UINT8 pdpAdr[REG_VALUENAME_LEN_MAX+1];
	BOOL bDef = FALSE;
	UINT8 *Cid = 0;
	UINT8 datasize=0;
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwSetNetWorkMode, nCid=0x%x, nSimID=0x%x\n", nCid, nSimID);
	if(pPdpCont == NULL || pPdpCont->pApn ==NULL || pPdpCont->pPdpAddr == NULL)
	return ERR_CFW_INVALID_PARAMETER;

	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGprsGetPdpCxt);

	ret = CFW_CfgNumRangeCheck(nCid, GPRS_CIND_RANGE);
	if(ret!= ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret=0x%x\n",ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
		return ERR_CFG_PARAM_OUT_RANGR;
	}
	nCid = nCid + 0x30;
	Cid = &nCid;

	//PDP_TYPE
	if(CFW_SIM_0==nSimID)
	{
	 	SUL_StrCopy(pdpType, "PDP_SIM0_TYPE");
	}
	else if (CFW_SIM_1==nSimID)
	{
		SUL_StrCopy(pdpType, "PDP_SIM1_TYPE");
	}
	else
	{
		return ERR_CFW_INVALID_PARAMETER;
	}
	SUL_StrCat(pdpType, Cid);
	pdpType[9] = '\0';

	//PDP_DATA_C
	if(CFW_SIM_0==nSimID)
	{
		SUL_StrCopy(pdpDCmp, "PDP_SIM0_DATA_C");
	}
	else if (CFW_SIM_1==nSimID)
	{
		SUL_StrCopy(pdpDCmp, "PDP_SIM1_DATA_C");
	}
	else
	{
		return ERR_CFW_INVALID_PARAMETER;
	}
	SUL_StrCat(pdpDCmp, Cid);
	pdpDCmp[11] = '\0';

	//PDP_HEAD_C
	if(CFW_SIM_0==nSimID)
	{
		SUL_StrCopy(pdpHCmp, "PDP_SIM0_HEAD_C");
	}
	else if (CFW_SIM_1==nSimID)
	{
		SUL_StrCopy(pdpHCmp, "PDP_SIM1_HEAD_C");
	}
	else
	{
		return ERR_CFW_INVALID_PARAMETER;
	}
	SUL_StrCat(pdpHCmp, Cid);
	pdpHCmp[11] = '\0';

	//PDP_APN
	if(CFW_SIM_0==nSimID)
	{
		SUL_StrCopy(pdpAPN, "PDP_SIM0_APN");
	}
	else if (CFW_SIM_1==nSimID)
	{
		SUL_StrCopy(pdpAPN, "PDP_SIM1_APN");
	}
	else
	{
		return ERR_CFW_INVALID_PARAMETER;
	}
	SUL_StrCat(pdpAPN, Cid);
	pdpAPN[8] = '\0';

	//PDP_ADDRESS
	if(CFW_SIM_0==nSimID)
	{
		SUL_StrCopy(pdpAdr, "PDP_SIM0_ADR");
	}
	else if (CFW_SIM_1==nSimID)
	{
		SUL_StrCopy(pdpAdr, "PDP_SIM1_ADR");
	}
	else
	{
		return ERR_CFW_INVALID_PARAMETER;
	}
	SUL_StrCat(pdpAdr, Cid);
	pdpAdr[8] = '\0';

	// nPdpType
	ret = cfg_read_8("PDP", pdpType, &(pPdpCont->nPdpType));
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret=0x%x\n",ret);
		ret = cfg_readdefault("PDP", pdpType,&(pPdpCont->nPdpType));
		if( ERR_SUCCESS!= ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
			return ret;
		}
		bDef = TRUE;
	}
	else
	{
		if(pPdpCont->nPdpType > 2)
		{
			ret = cfg_readdefault("PDP", pdpType,&(pPdpCont->nPdpType));
			if( ERR_SUCCESS!= ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
				return ret;
			}
			bDef = TRUE;
		}
	}
	if(bDef)
	cfg_write_8("PDP", pdpType, pPdpCont->nPdpType);

	// nDComp
	ret = cfg_read_8("PDP", pdpDCmp, &(pPdpCont->nDComp));
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8 pdpDCmp, error ret=0x%x\n",ret);
		ret = cfg_readdefault("PDP", pdpDCmp, &(pPdpCont->nDComp));
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
			return ret;
		}
		bDef = TRUE;
	}
	else
	{
		if(pPdpCont->nDComp > 3)
		{
			ret = cfg_readdefault("PDP", pdpDCmp, &(pPdpCont->nDComp));
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
				return ret;
			}
			bDef = TRUE;
		}
	}
	if(bDef)
	cfg_write_8("PDP", pdpDCmp, pPdpCont->nDComp);

	// nHComp
	ret = cfg_read_8("PDP", pdpHCmp, &(pPdpCont->nHComp));
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8 pdpHCmp, error ret=0x%x\n",ret);
		ret = cfg_readdefault("PDP", pdpHCmp, &(pPdpCont->nHComp));
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
			return ret;
		}
		bDef = TRUE;
	}
	else
	{
		if(pPdpCont->nHComp > 4)
		{
			ret = cfg_readdefault("PDP", pdpHCmp, &(pPdpCont->nHComp));
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
				return ret;
			}
			bDef = TRUE;
		}
	}
	if(bDef)
	cfg_write_8("PDP", pdpHCmp, pPdpCont->nHComp);

	// 	pApn
	datasize = pPdpCont->nApnSize;
	ret = cfg_read_str("PDP", pdpAPN, (pPdpCont->pApn), &datasize);
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_str pdpAPN, error ret=0x%x\n",ret);
		ret = cfg_readdefault("PDP", pdpAPN, (pPdpCont->pApn));
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault pdpAPN, error ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
			return ret;
		}
		bDef = TRUE;
	}
	pPdpCont->nApnSize = datasize;

	if(bDef)
	{
		bDef = FALSE;
		cfg_write_str("PDP", pdpAPN, pPdpCont->pApn, pPdpCont->nApnSize);
	}

	// pPdpAddr
	datasize = pPdpCont->nPdpAddrSize;
	ret = cfg_read_str("PDP", pdpAdr, (pPdpCont->pPdpAddr), &datasize);
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_str pdpAdr, error ret=0x%x\n",ret);
		ret = cfg_readdefault("PDP", pdpAdr, (pPdpCont->pPdpAddr));
		if(ret != ERR_SUCCESS)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault pdpAdr, error ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
			return ret;
		}
		bDef = TRUE;
	}
	pPdpCont->nPdpAddrSize = (UINT8)datasize;

	if(bDef)
	cfg_write_str("PDP", pdpAdr, pPdpCont->pPdpAddr, pPdpCont->nPdpAddrSize);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsGetPdpCxt exit\n");
	return ERR_SUCCESS;

}


//=============================================================================================
//AT+CGQREQ
//This command allows the TE to specify a Quality of Service Profile 
//that is used when the MT sends an Activate PDP Context Request message 
///to the network.

//[in]nCid [1 - 7], [in]pQos

//=============================================================================================
UINT32 CFW_CfgGprsSetReqQos(UINT8 nCid, CFW_GPRS_QOS *pQos, CFW_SIM_ID nSimID)
{
    UINT32  ret = ERR_SUCCESS;
    UINT8 name[REG_VALUENAME_LEN_MAX+1];
    UINT8 *Cid = 0;
	if( pQos == NULL)
		return ERR_CFW_INVALID_PARAMETER;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGprsSetReqQos);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsSetReqQos, nCid=0x%x, nSimID=0x%x\n", nCid, nSimID);
    ret = CFW_CfgNumRangeCheck(nCid, GPRS_CIND_RANGE);
    if(ret!= ERR_SUCCESS)
    {
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret=0x%x\n",ret);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetReqQos);
	return ERR_CFG_PARAM_OUT_RANGR;
    }
        
    if((pQos->nDelay > 7) || (pQos->nMean > 31) || (pQos->nPeak > 15) 
		|| (pQos->nPrecedence > 7) || (pQos->nReliability > 7))
    {
	    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetReqQos);
         return ERR_CFW_INVALID_PARAMETER;
    }
    nCid = nCid + 0x30;
    Cid = &nCid;
    if(CFW_SIM_0==nSimID)
    {
	    SUL_StrCopy(name, "SIM0_QOS");
	}
	else if (CFW_SIM_1==nSimID)
	{
	    SUL_StrCopy(name, "SIM1_QOS");
	}
	else
	{
        return ERR_CFW_INVALID_PARAMETER;
	}
	SUL_StrCat(name, Cid);
    
	ret = cfg_write_hex("PDP", name, (PVOID) pQos, sizeof(CFW_GPRS_QOS));
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetReqQos);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsSetReqQos exit\n");
    return ret;
}


//=============================================================================================
// [in]nCid[1 - 7], [out]pQos
//=============================================================================================
UINT32 CFW_CfgGprsGetReqQos(UINT8 nCid, CFW_GPRS_QOS *pQos, CFW_SIM_ID nSimID)
{
    UINT32  ret = ERR_SUCCESS;
    UINT8 name[REG_VALUENAME_LEN_MAX+1];
    UINT8 *Cid = 0;
    
    if( pQos == NULL)
        return ERR_CFW_INVALID_PARAMETER;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGprsGetReqQos);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsGetReqQos, nCid=0x%x, nSimID=0x%x\n", nCid, nSimID);
    ret = CFW_CfgNumRangeCheck(nCid, GPRS_CIND_RANGE);
    if(ret!= ERR_SUCCESS)
    {
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret=0x%x\n",ret);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetReqQos);
	return ERR_CFG_PARAM_OUT_RANGR;
    }
    
    nCid = nCid + 0x30;
    Cid = &nCid;
    if(CFW_SIM_0==nSimID)
    {
	    SUL_StrCopy(name, "SIM0_QOS");
	}
	else if (CFW_SIM_1==nSimID)
	{
	    SUL_StrCopy(name, "SIM1_QOS");
	}
	else
	{
        return ERR_CFW_INVALID_PARAMETER;
	}
    SUL_StrCat(name, Cid);

	ret = cfg_read_hex("PDP", name, (PVOID) pQos, (UINT8)sizeof(CFW_GPRS_QOS));
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret=0x%x\n",ret);
		ret = cfg_readdefault("PDP", name, (PVOID) pQos);
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetReqQos);
			return ret;
		}
	}
	else
	{
	    if((pQos->nDelay > 7) || (pQos->nMean > 31) || (pQos->nPeak > 15) 
			|| (pQos->nPrecedence > 7) || (pQos->nReliability > 7))
		{
			ret = cfg_readdefault("PDP", name, (PVOID) pQos);
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetReqQos);
				return ret;
			}
		}
	}

    return ret;
}


//===========================================================================
//[in]nCid[1 - 7]
//===========================================================================
UINT32 CFW_CfgGprsSetMinQos(UINT8 nCid, CFW_GPRS_QOS *pQos, CFW_SIM_ID nSimID)
{
    UINT32  ret = ERR_SUCCESS;
    UINT8 name[REG_VALUENAME_LEN_MAX+1];
    UINT8 *Cid = 0;
    
    if( pQos == NULL)
        return ERR_CFW_INVALID_PARAMETER;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGprsSetMinQos);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsSetMinQos, nCid=0x%x, nSimID=0x%x\n", nCid, nSimID);
    ret = CFW_CfgNumRangeCheck(nCid, GPRS_CIND_RANGE);
    if(ret!= ERR_SUCCESS)
    {
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret=0x%x\n", ret);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetMinQos);
        return ERR_CFG_PARAM_OUT_RANGR;
    }
    if((pQos->nDelay > 7) || (pQos->nMean > 31) || (pQos->nPeak > 15) 
		|| (pQos->nPrecedence > 7) || (pQos->nReliability > 7))
    {
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetMinQos);
		return ERR_CFW_INVALID_PARAMETER;
    }
    
    nCid = nCid + 0x30;
    Cid = &nCid;
    if(CFW_SIM_0==nSimID)
    {
 	   SUL_StrCopy(name, "SIM0_MINQOS");
	}
	else if (CFW_SIM_1==nSimID)
	{
	    SUL_StrCopy(name, "SIM1_MINQOS");
	}
	else
	{
        return ERR_CFW_INVALID_PARAMETER;
	}
    SUL_StrCat(name, Cid);
    
	ret = cfg_write_hex("PDP", name, (PVOID) pQos, sizeof(CFW_GPRS_QOS));
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetMinQos);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsSetMinQos exit\n");
    return ret;
}

/*===========================================================================
//[in]nCid[1 - 7]
===========================================================================*/
UINT32 CFW_CfgGprsGetMInQos(UINT8 nCid, CFW_GPRS_QOS *pQos, CFW_SIM_ID nSimID)
{
    UINT32  ret = ERR_SUCCESS;
    UINT8 name[REG_VALUENAME_LEN_MAX+1];
    UINT8 *Cid = 0;
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsGetReqQos, nCid=0x%x, nSimID=0x%x\n", nCid, nSimID);
    if( pQos == NULL)
        return ERR_CFW_INVALID_PARAMETER;
        
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGprsGetMInQos);
	
    ret = CFW_CfgNumRangeCheck(nCid, GPRS_CIND_RANGE);
    if(ret!= ERR_SUCCESS)
    {
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret=0x%x\n",ret);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetMInQos);
	return ERR_CFG_PARAM_OUT_RANGR;
    }
    
    nCid = nCid + 0x30;
    Cid = &nCid;
    if(CFW_SIM_0==nSimID)
    {
    	SUL_StrCopy(name, "SIM0_MINQOS");
	}
	else if (CFW_SIM_1==nSimID)
	{
    	SUL_StrCopy(name, "SIM1_MINQOS");
	}
	else
	{
        return ERR_CFW_INVALID_PARAMETER;
	}
    SUL_StrCat(name, Cid);

	ret = cfg_read_hex("PDP", name, (PVOID) pQos, sizeof(CFW_GPRS_QOS));
	if(ERR_SUCCESS != ret)
	{
		ret = cfg_readdefault("PDP", name, (PVOID) pQos);
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGprsGetReqQos,1, call cfg_readdefault failed\n");
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetMInQos);
			return ret;
		}
	}
	else
	{
	    if((pQos->nDelay > 7) || (pQos->nMean > 31) || (pQos->nPeak > 15) 
			|| (pQos->nPrecedence > 7) || (pQos->nReliability > 7))
		{
			ret = cfg_readdefault("PDP", name, (PVOID) pQos);
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGprsGetReqQos,2, call cfg_readdefault failed\n");
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetMInQos);
				return ret;
			}
		}
	}

    return ret;
}
//add for gprs data statistics by wuys 2010-09-03
INT32 gprs_sum[CFW_SIM_COUNT][2] ={{0},};
UINT32 gprs_SetGprsSum(INT32 upsum,INT32 downsum,CFW_SIM_ID nSimID )
{
	UINT32  ret =ERR_SUCCESS;
        if( (CFW_SIM_0<=nSimID)&&(CFW_SIM_COUNT>nSimID))
	 {

		char sims[2] = {(nSimID+0x30),0x00};

		char nameSim[REG_VALUENAME_LEN_MAX+1] = {0x00,};
		strcpy(nameSim, "GPRS_UP_SUM_SIM");
	      strcat(nameSim, sims);
		ret = cfg_write_32("nw",nameSim, upsum);	    

		memset(nameSim,0x00,REG_VALUENAME_LEN_MAX+1);
		strcpy(nameSim, "GPRS_DWN_SUM_SIM");
	      strcat(nameSim, sims);

		ret = cfg_write_32("nw",nameSim, downsum);	    
	 
	     
	   }else
		ret = ERR_CFW_INVALID_PARAMETER;
	
       return ret;
}
UINT32 gprs_GetGprsSum(INT32* upsum,INT32* downsum,CFW_SIM_ID nSimID )
{
	UINT32  ret =ERR_SUCCESS;
        if( (CFW_SIM_0<=nSimID)&&(CFW_SIM_COUNT>nSimID))
	 {
	    	char sims[2] = {(nSimID+0x30),0x00};
		char nameSim[REG_VALUENAME_LEN_MAX+1] = {0x00,};
		strcpy(nameSim, "GPRS_UP_SUM_SIM");
	      strcat(nameSim, sims);
		ret = cfg_read_32("nw",nameSim, upsum);	    

		memset(nameSim,0x00,REG_VALUENAME_LEN_MAX+1);
		strcpy(nameSim, "GPRS_DWN_SUM_SIM");
	      strcat(nameSim, sims);

		ret = cfg_read_32("nw",nameSim, downsum);	    
	   }else
		ret = ERR_CFW_INVALID_PARAMETER;
	
       return ret;
}
UINT32 CFW_GetGprsSum(INT32* upsum,INT32* downsum,CFW_SIM_ID nSimID )
{
	
	*upsum = gprs_sum[nSimID][0];
      *downsum = gprs_sum[nSimID][1];
	  return ERR_SUCCESS;
 }

UINT32 CFW_ClearGprsSum(CFW_SIM_ID nSimID )
{
	gprs_SetGprsSum(0, 0,nSimID );
	gprs_sum[nSimID][0]= 0;
	gprs_sum[nSimID][1]= 0;
       return ERR_SUCCESS;

}

//add end
#endif


//=================================================================
//CFW_CfgNwSetFM 
//
//=================================================================
UINT32 CFW_CfgNwSetFM( CFW_COMM_MODE bMode, CFW_SIM_ID nSimID )
{
	UINT32 nRet = ERR_SUCCESS;

	INT8 simid_c[2]  = {0x30,0x00};
	INT8 name_s[25] = {0x00,};
	INT8 name_s1[15] = {0x00,};
	
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgNwSetFM);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwSetFM, bMode =0x%x, nSimID=0x%x\n", bMode, nSimID);

	strcpy(name_s,"NW_SIM");
	strcpy(name_s1,"_FLIGHT_MODE");
	if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
	{
		simid_c[0] = simid_c[0] + nSimID;
		strcat(name_s, simid_c);
		strcat(name_s, name_s1);
		nRet = cfg_write_32("nw", name_s, (UINT32)bMode);
	}
	else
	{
		nRet = ERR_CFW_INVALID_PARAMETER;
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
		return nRet;
	}
	
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwSetFM exit\n");
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwSetFM);
	return nRet;
}


//=================================================================
//CFW_CfgNwGetFM 
//
//=================================================================
UINT32 CFW_CfgNwGetFM(CFW_COMM_MODE *bMode, CFW_SIM_ID nSimID)
{
	UINT32 nRet= ERR_SUCCESS;
	BOOL bDef = FALSE;
	INT8 simid_c[2]  = {0x30,0x00};
	INT8 name_s[25] = {0x00,};
	INT8 name_s1[15] = {0x00,};

	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgNwGetFM);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsGetReqQos, nSimID=0x%x\n", nSimID);

	if( NULL == bMode )
	{
		return ERR_CFW_INVALID_PARAMETER;
	}
	else
	{
        strcpy(name_s,"NW_SIM");
		strcpy(name_s1,"_FLIGHT_MODE");
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			strcat(name_s, name_s1);
			nRet = cfg_read_32("nw", name_s, (UINT32*)bMode);
		}
		else
		{
		    nRet = ERR_CFW_INVALID_PARAMETER;
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
			return nRet;
		}
	

        if(ERR_SUCCESS != nRet)
        {
   	     	CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_32, error nRet=0x%x\n",nRet);
            
            nRet = cfg_readdefault("nw", name_s,bMode);
        	
            if(ERR_SUCCESS != nRet)
            {
		        CSW_TRACE(CFW_CFG_TS_ID, "in NW_FLIGHT_MODE,1, call cfg_readdefault false,ret=0x%x\n",nRet);
		        CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetNetWorkMode);
		        return nRet;
            }
            bDef = TRUE;
        }
        else
        {
            nRet = CFW_CfgNumRangeCheck(*bMode, NW_FLIGHTMODE_RANGE);
            if(ERR_SUCCESS != nRet)
            {
		  		CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error nRet=0x%x\n",nRet);
            	nRet = cfg_readdefault("nw", name_s, bMode);

                if(ERR_SUCCESS != nRet)
                {
                  CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgNwGetFM,1, call cfg_readdefault false,ret=0x%x\n",nRet);
                  CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetNetWorkMode);
                  return nRet;
                }
                bDef = TRUE;
            }
        }
    }
        
    if(bDef)
    {
        cfg_write_32("nw", name_s, *bMode);
    }
    
    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetFM);	
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwGetFM exit\n");
    return ERR_SUCCESS;
        
}

#else

/*==========================================================================
NAME        FUNCTION                PARAMETER   RANGE   DEFAULT
AT+CREG	    CFW_CfgSetNwStatus	    N	        0-2	    0

==========================================================================*/
UINT32 CFW_CfgSetNwStatus (UINT8 n)
{   
    UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetNwStatus);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetNwStatus, n=0x%x\n", n);
    /*Check the range*/
    ret = CFW_CfgNumRangeCheck(n, NW_REG_RANGE);
    if(ret != ERR_SUCCESS)
    {
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, ret=0x%x\n", ret);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetNwStatus);
	return ret;
    }

    MemoryInfo.NwStatus = n;
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetNwStatus);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetNwStatus exit\n");
    return ret;
}

UINT32 CFW_CfgGetNwStatus (UINT8 *n)
{   
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetNwStatus enter\n");

	*n = MemoryInfo.NwStatus;
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetNwStatus, *n=0x%x\n", *n);
    return ERR_SUCCESS;
}

//PRIVATE UINT8 g_FrequencyBand = CFW_NW_BAND_GSM_900P|CFW_NW_BAND_DCS_1800|CFW_NW_BAND_GSM_900E;
//**************************************************
//NOTICE:  the param of this function is a combination of several values, so the user should check the param by himself
UINT32 CFW_CfgNwSetFrequencyBand ( UINT8 nBand)
{
    UINT32 ret = ERR_SUCCESS;
   // g_FrequencyBand = nBand;
    CSW_PROFILE_FUNCTION_ENTER(CFW_CfgNwSetFrequencyBand);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwSetFrequencyBand, nBand =0x%x\n", nBand);
   
    if(nBand>0)
    {
	    ret = cfg_write_8("nw", "NW_FREQ_BAND", nBand);
    }
	else
		ret = ERR_CFG_PARAM_OUT_RANGR;
    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwSetFrequencyBand);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwSetFrequencyBand exit\n");
    return ret;

}

UINT32 CFW_CfgNwGetFrequencyBand (
  UINT8* pBand
)
{
    UINT32 ret = ERR_SUCCESS;
	BOOL bDef = FALSE;
    CSW_PROFILE_FUNCTION_ENTER(CFW_CfgNwGetFrequencyBand);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwGetFrequencyBand enter\n");
    if(pBand)
        //*pBand = g_FrequencyBand;
    {
	    ret = cfg_read_8("nw", "NW_FREQ_BAND", pBand);
	    if(ERR_SUCCESS != ret)
	    {
		    CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgNwGetFrequencyBand, call function cfg_read_8 failed,ret=0x%x\n",ret);
		    ret = cfg_readdefault("nw", "NW_FREQ_BAND", pBand);
		    if(ERR_SUCCESS != ret)
		    {
			    CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgNwGetFrequencyBand,1, call function cfg_readdefault failed,ret=0x%x\n",ret);
			    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetFrequencyBand);
			    return ret;
		    }
		    bDef = TRUE;
	    }
	    else
	    {
		    if(!(*pBand))
		    {
			    //*pBand = 0x4c;
			    ret = cfg_readdefault("nw", "NW_FREQ_BAND", pBand);
			    if(ERR_SUCCESS != ret)
			    {
				    CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgNwGetFrequencyBand,2, call function cfg_readdefault failed,ret=0x%x\n",ret);
					CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetFrequencyBand);
				    return ret;
			    }
			    bDef = TRUE;
		    }
	    }
    }

	if(bDef)
		cfg_write_8("nw", "NW_FREQ_BAND", *pBand);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetFrequencyBand);	
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetNwStatus, *pBand =0x%x\n", *pBand);
    return ERR_SUCCESS;
}


//description: set the network mode.
//param:nMode:the value of the network mode, it should be the following value:
//mode: [in]                Description                              
//  0        automatic mode, in this mode, nOperatorId can be ignored. In this     
//               case this function don't care this parameter at all.                  
//  1        manual operator selection.                                            
//  4        combination of manual/automatic mode; if manual selection fails, ME   
//              switches to automatic mode.                                           
//Return Values
//  ERR_SUCCESS
//  ERR_CFW_INVALID_PARAMETER   The input parameter is NULL. 
UINT32 CFW_CfgNwSetNetWorkMode(UINT8 nMode)
{
	UINT32 ret = ERR_SUCCESS;
    CSW_PROFILE_FUNCTION_ENTER(CFW_CfgNwSetNetWorkMode);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwSetNetWorkMode, nMode=0x%x\n", nMode);
	ret = CFW_CfgNumRangeCheck(nMode, NW_WORKMODE_RANGE);
	if(ERR_SUCCESS == ret)
	{
		ret = cfg_write_8("nw", "NW_WORK_MODE", nMode);
	}
	else
	{
		ret = ERR_CFW_INVALID_PARAMETER;
	}
    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwSetNetWorkMode);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwSetNetWorkMode exit\n");
	return ret;
}

//description: Get the network mode.
//param:nMode:the value of the network mode, it should be the following value:
//pMode:[out] output the nw registor mode                              
//  0        automatic mode, in this mode, nOperatorId can be ignored. In this     
//               case this function don't care this parameter at all.                  
//  1        manual operator selection.                                            
//  4        combination of manual/automatic mode; if manual selection fails, ME   
//              switches to automatic mode.                                           
//Return Values
//  ERR_SUCCESS
//  ERR_CFW_INVALID_PARAMETER   The input parameter is NULL. 
//Return Event:  None.
UINT32 CFW_CfgNwGetNetWorkMode(UINT8 *pMode)
{
	UINT32 ret= ERR_SUCCESS;
	BOOL bDef = FALSE;
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwGetNetWorkMode enter\n");
	if(NULL == pMode)
	{
		return ERR_CFW_INVALID_PARAMETER;
	}
	else
	{
	    CSW_PROFILE_FUNCTION_ENTER(CFW_CfgNwGetNetWorkMode);
       ret = cfg_read_8("nw", "NW_WORK_MODE", pMode);
       if(ERR_SUCCESS != ret)
       {
	       CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgNwGetNetWorkMode,1, call cfg_read_8 false,ret=0x%x\n",ret);
	       ret = cfg_readdefault("nw", "NW_WORK_MODE", pMode);
	       if(ERR_SUCCESS != ret)
	       {
		       CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgNwGetNetWorkMode,1, call cfg_readdefault false,ret=0x%x\n",ret);
			    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetNetWorkMode);
		       return ret;
	       }
	       bDef = TRUE;
       }
       else
       {
	       ret = CFW_CfgNumRangeCheck(*pMode, NW_WORKMODE_RANGE);
	       if(ERR_SUCCESS != ret)
	       {
		       CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgNwGetNetWorkMode,1, call CFW_CfgNumRangeCheck false,ret=0x%x\n",ret);
		       ret = cfg_readdefault("nw", "NW_WORK_MODE", pMode);
		       if(ERR_SUCCESS != ret)
		       {
			       CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgNwGetNetWorkMode,1, call cfg_readdefault false,ret=0x%x\n",ret);
			       CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetNetWorkMode);
			       return ret;
		       }
		       bDef = TRUE;
	       }
       }
	}
	if(bDef)
		cfg_write_8("nw", "NW_WORK_MODE", *pMode);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetNetWorkMode);	
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwGetNetWorkMode, *pMode =0x%x\n", *pMode);
	return ERR_SUCCESS;
	
}
#ifdef CFW_GPRS_SUPPORT


//=============================================================================================
//synchronization function
//AT+CGDCONT
//[in]nCid[1 - 7], [in]pPdpCont
//
//============================================================================================= 
UINT32 CFW_CfgGprsSetPdpCxt(UINT8 nCid, CFW_GPRS_PDPCONT_INFO *pPdpCont)
{
    UINT32  ret = ERR_SUCCESS;
    UINT8 pdpType[REG_VALUENAME_LEN_MAX+1];
    UINT8 pdpDCmp[REG_VALUENAME_LEN_MAX+1];
    UINT8 pdpHCmp[REG_VALUENAME_LEN_MAX+1];
    UINT8 pdpAPN[REG_VALUENAME_LEN_MAX+1];
    UINT8 pdpAdr[REG_VALUENAME_LEN_MAX+1];
    UINT8 *Cid = 0;
    CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGprsSetPdpCxt);
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsSetPdpCxt, nCid=0x%x\n", nCid);
    ret = CFW_CfgNumRangeCheck(nCid, GPRS_CIND_RANGE);
    if(ret!= ERR_SUCCESS)
    {
    	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret=0x%x\n",ret);
    	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetPdpCxt);
    	return ERR_CFG_PARAM_OUT_RANGR;
    }
    if(pPdpCont == NULL)
    {
	    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetPdpCxt);
	    return ERR_CFW_INVALID_PARAMETER;
    }
    if((pPdpCont->nPdpType > 2) 
		|| (pPdpCont->nDComp > 3) 
		|| (pPdpCont->nHComp > 4)
		|| (pPdpCont->nApnSize > 100)
		|| (pPdpCont->nPdpAddrSize > 18))
    {
	    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetPdpCxt);
        return ERR_CFG_PARAM_OUT_RANGR;
    }
    nCid = nCid + 0x30;
    Cid = &nCid;
    
    //PDP_TYPE 
    SUL_StrCopy(pdpType, "PDP_TYPE");
    SUL_StrCat(pdpType, Cid);
	pdpType[9] = '\0';

    //PDP_DATA_C 
    SUL_StrCopy(pdpDCmp, "PDP_DATA_C");
    SUL_StrCat(pdpDCmp, Cid);
	pdpDCmp[11] = '\0';

    //PDP_HEAD_C 
    SUL_StrCopy(pdpHCmp, "PDP_HEAD_C");
    SUL_StrCat(pdpHCmp, Cid);
	pdpHCmp[11] = '\0';
	
    //PDP_APN 
    SUL_StrCopy(pdpAPN, "PDP_APN");
    SUL_StrCat(pdpAPN, Cid);
	pdpAPN[8] = '\0';
    //PDP_ADDRESS 
    SUL_StrCopy(pdpAdr, "PDP_ADR");
    SUL_StrCat(pdpAdr, Cid);
	pdpAdr[8] = '\0';

	ret = cfg_write_8("PDP", pdpType, pPdpCont->nPdpType);
	
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_8 pdpType, error ret=0x%x\n",ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetPdpCxt);
		return ret;
	}
	ret = cfg_write_8("PDP", pdpDCmp, pPdpCont->nDComp);
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_8 pdpDCmp, error ret=0x%x\n",ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetPdpCxt);
		return ret;
	}
	ret = cfg_write_8("PDP", pdpHCmp, pPdpCont->nHComp);
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_8 pdpHCmp, error ret=0x%x\n",ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetPdpCxt);
		return ret;
	}
	ret = cfg_write_str("PDP", pdpAPN, pPdpCont->pApn, pPdpCont->nApnSize);
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_8 pdpAPN, error ret=0x%x\n",ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetPdpCxt);
		return ret;
	}
	ret = cfg_write_str("PDP", pdpAdr, pPdpCont->pPdpAddr, pPdpCont->nPdpAddrSize);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetPdpCxt);
	if(ret != ERR_SUCCESS)
		return ret;
	return ERR_SUCCESS;
}

//=============================================================================
//[in]nCid[1 - 7], [out]pPdpCont
//=============================================================================
UINT32 CFW_CfgGprsGetPdpCxt(UINT8 nCid, CFW_GPRS_PDPCONT_INFO *pPdpCont)
{
    UINT32  ret = ERR_SUCCESS;

    UINT8 pdpType[REG_VALUENAME_LEN_MAX+1];
    UINT8 pdpDCmp[REG_VALUENAME_LEN_MAX+1];
    UINT8 pdpHCmp[REG_VALUENAME_LEN_MAX+1];
    UINT8 pdpAPN[REG_VALUENAME_LEN_MAX+1];
    UINT8 pdpAdr[REG_VALUENAME_LEN_MAX+1];
    BOOL bDef = FALSE;
    UINT8 *Cid = 0;
    UINT8 datasize=0;
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwSetNetWorkMode, nCid=0x%x\n", nCid);
    if(pPdpCont == NULL || pPdpCont->pApn ==NULL || pPdpCont->pPdpAddr == NULL)
    return ERR_CFW_INVALID_PARAMETER;
    
    CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGprsGetPdpCxt);
    
    ret = CFW_CfgNumRangeCheck(nCid, GPRS_CIND_RANGE);
    if(ret!= ERR_SUCCESS)
    {
    	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret=0x%x\n",ret);
    	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
    	return ERR_CFG_PARAM_OUT_RANGR;
    }
    nCid = nCid + 0x30;
    Cid = &nCid;
    
    //PDP_TYPE
    SUL_StrCopy(pdpType, "PDP_TYPE");
    SUL_StrCat(pdpType, Cid);
	pdpType[9] = '\0';
	
    //PDP_DATA_C
    SUL_StrCopy(pdpDCmp, "PDP_DATA_C");
    SUL_StrCat(pdpDCmp, Cid);
	pdpDCmp[11] = '\0';
	
    //PDP_HEAD_C
    SUL_StrCopy(pdpHCmp, "PDP_HEAD_C");
    SUL_StrCat(pdpHCmp, Cid);
	pdpHCmp[11] = '\0';
	
    //PDP_APN
    SUL_StrCopy(pdpAPN, "PDP_APN");
    SUL_StrCat(pdpAPN, Cid);
	pdpAPN[8] = '\0';
	
    //PDP_ADDRESS
    SUL_StrCopy(pdpAdr, "PDP_ADR");
    SUL_StrCat(pdpAdr, Cid);
	pdpAdr[8] = '\0';
	
	// nPdpType
	ret = cfg_read_8("PDP", pdpType, &(pPdpCont->nPdpType));
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret=0x%x\n",ret);
		ret = cfg_readdefault("PDP", pdpType,&(pPdpCont->nPdpType));
		if( ERR_SUCCESS!= ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
			return ret;
		}
		bDef = TRUE;
	}
	else
	{
		if(pPdpCont->nPdpType > 2)
		{
			ret = cfg_readdefault("PDP", pdpType,&(pPdpCont->nPdpType));
			if( ERR_SUCCESS!= ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
				return ret;
			}
			bDef = TRUE;
		}
	}
	if(bDef)
	cfg_write_8("PDP", pdpType, pPdpCont->nPdpType);

	// nDComp
	ret = cfg_read_8("PDP", pdpDCmp, &(pPdpCont->nDComp));
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8 pdpDCmp, error ret=0x%x\n",ret);
		ret = cfg_readdefault("PDP", pdpDCmp, &(pPdpCont->nDComp));
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
			return ret;
		}
		bDef = TRUE;
	}
	else
	{
		if(pPdpCont->nDComp > 3)
		{
			ret = cfg_readdefault("PDP", pdpDCmp, &(pPdpCont->nDComp));
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
				return ret;
			}
			bDef = TRUE;
		}
	}
	if(bDef)
	cfg_write_8("PDP", pdpDCmp, pPdpCont->nDComp);

	// nHComp
	ret = cfg_read_8("PDP", pdpHCmp, &(pPdpCont->nHComp));
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8 pdpHCmp, error ret=0x%x\n",ret);
		ret = cfg_readdefault("PDP", pdpHCmp, &(pPdpCont->nHComp));
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
			return ret;
		}
		bDef = TRUE;
	}
	else
	{
		if(pPdpCont->nHComp > 4)
		{
			ret = cfg_readdefault("PDP", pdpHCmp, &(pPdpCont->nHComp));
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
				return ret;
			}
			bDef = TRUE;
		}
	}
	if(bDef)
	cfg_write_8("PDP", pdpHCmp, pPdpCont->nHComp);

	// 	pApn
	datasize = pPdpCont->nApnSize;
	ret = cfg_read_str("PDP", pdpAPN, (pPdpCont->pApn), &datasize);
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_str pdpAPN, error ret=0x%x\n",ret);
		ret = cfg_readdefault("PDP", pdpAPN, (pPdpCont->pApn));
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault pdpAPN, error ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
			return ret;
		}
		bDef = TRUE;
	}
	pPdpCont->nApnSize = datasize;

	if(bDef)
	{
		bDef = FALSE;
		cfg_write_str("PDP", pdpAPN, pPdpCont->pApn, pPdpCont->nApnSize);
	}

	// pPdpAddr
	datasize = pPdpCont->nPdpAddrSize;
	ret = cfg_read_str("PDP", pdpAdr, (pPdpCont->pPdpAddr), &datasize);
	if(ret != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_str pdpAdr, error ret=0x%x\n",ret);
		ret = cfg_readdefault("PDP", pdpAdr, (pPdpCont->pPdpAddr));
		if(ret != ERR_SUCCESS)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault pdpAdr, error ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
			return ret;
		}
		bDef = TRUE;
	}
	pPdpCont->nPdpAddrSize = (UINT8)datasize;

	if(bDef)
	cfg_write_str("PDP", pdpAdr, pPdpCont->pPdpAddr, pPdpCont->nPdpAddrSize);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetPdpCxt);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsGetPdpCxt exit\n");
	return ERR_SUCCESS;

}


//=============================================================================================
//AT+CGQREQ
//This command allows the TE to specify a Quality of Service Profile 
//that is used when the MT sends an Activate PDP Context Request message 
///to the network.

//[in]nCid [1 - 7], [in]pQos

//=============================================================================================
UINT32 CFW_CfgGprsSetReqQos(UINT8 nCid, CFW_GPRS_QOS *pQos)
{
    UINT32  ret = ERR_SUCCESS;
    UINT8 name[REG_VALUENAME_LEN_MAX+1];
    UINT8 *Cid = 0;
	if( pQos == NULL)
		return ERR_CFW_INVALID_PARAMETER;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGprsSetReqQos);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsSetReqQos, nCid=0x%x\n", nCid);
    ret = CFW_CfgNumRangeCheck(nCid, GPRS_CIND_RANGE);
    if(ret!= ERR_SUCCESS)
    {
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret=0x%x\n",ret);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetReqQos);
	return ERR_CFG_PARAM_OUT_RANGR;
    }
        
    if((pQos->nDelay > 7) || (pQos->nMean > 31) || (pQos->nPeak > 15) 
		|| (pQos->nPrecedence > 7) || (pQos->nReliability > 7))
    {
	    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetReqQos);
         return ERR_CFW_INVALID_PARAMETER;
    }
    nCid = nCid + 0x30;
    Cid = &nCid;
    SUL_StrCopy(name, "QOS");
    SUL_StrCat(name, Cid);
    
	ret = cfg_write_hex("PDP", name, (PVOID) pQos, sizeof(CFW_GPRS_QOS));
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetReqQos);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsSetReqQos exit\n");
    return ret;
}


//=============================================================================================
// [in]nCid[1 - 7], [out]pQos
//=============================================================================================
UINT32 CFW_CfgGprsGetReqQos(UINT8 nCid, CFW_GPRS_QOS *pQos)
{
    UINT32  ret = ERR_SUCCESS;
    UINT8 name[REG_VALUENAME_LEN_MAX+1];
    UINT8 *Cid = 0;
    
    if( pQos == NULL)
        return ERR_CFW_INVALID_PARAMETER;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGprsGetReqQos);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsGetReqQos, nCid=0x%x\n", nCid);
    ret = CFW_CfgNumRangeCheck(nCid, GPRS_CIND_RANGE);
    if(ret!= ERR_SUCCESS)
    {
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret=0x%x\n",ret);
	    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetReqQos);
        return ERR_CFG_PARAM_OUT_RANGR;
    }
    
    nCid = nCid + 0x30;
    Cid = &nCid;
    SUL_StrCopy(name, "QOS");
    SUL_StrCat(name, Cid);

	ret = cfg_read_hex("PDP", name, (PVOID) pQos, (UINT8)sizeof(CFW_GPRS_QOS));
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret=0x%x\n",ret);
		ret = cfg_readdefault("PDP", name, (PVOID) pQos);
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetReqQos);
			return ret;
		}
	}
	else
	{
	    if((pQos->nDelay > 7) || (pQos->nMean > 31) || (pQos->nPeak > 15) 
			|| (pQos->nPrecedence > 7) || (pQos->nReliability > 7))
		{
			ret = cfg_readdefault("PDP", name, (PVOID) pQos);
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetReqQos);
				return ret;
			}
		}
	}

    return ret;
}


//===========================================================================
//[in]nCid[1 - 7]
//===========================================================================
UINT32 CFW_CfgGprsSetMinQos(UINT8 nCid, CFW_GPRS_QOS *pQos)
{
    UINT32  ret = ERR_SUCCESS;
    UINT8 name[REG_VALUENAME_LEN_MAX+1];
    UINT8 *Cid = 0;
    
    if( pQos == NULL)
        return ERR_CFW_INVALID_PARAMETER;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGprsSetMinQos);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsSetMinQos, nCid=0x%x\n", nCid);
    ret = CFW_CfgNumRangeCheck(nCid, GPRS_CIND_RANGE);
    if(ret!= ERR_SUCCESS)
    {
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret=0x%x\n", ret);
	    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetMinQos);
        return ERR_CFG_PARAM_OUT_RANGR;
    }
    if((pQos->nDelay > 7) || (pQos->nMean > 31) || (pQos->nPeak > 15) 
		|| (pQos->nPrecedence > 7) || (pQos->nReliability > 7))
    {
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetMinQos);
		return ERR_CFW_INVALID_PARAMETER;
    }
    
    nCid = nCid + 0x30;
    Cid = &nCid;
    SUL_StrCopy(name, "MINQOS");
    SUL_StrCat(name, Cid);
    
	ret = cfg_write_hex("PDP", name, (PVOID) pQos, sizeof(CFW_GPRS_QOS));
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsSetMinQos);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsSetMinQos exit\n");
    return ret;
}

/*===========================================================================
//[in]nCid[1 - 7]
===========================================================================*/
UINT32 CFW_CfgGprsGetMInQos(UINT8 nCid, CFW_GPRS_QOS *pQos)
{
    UINT32  ret = ERR_SUCCESS;
    UINT8 name[REG_VALUENAME_LEN_MAX+1];
    UINT8 *Cid = 0;
	
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsGetReqQos, nCid=0x%x\n", nCid);
	if( pQos == NULL)
	    return ERR_CFW_INVALID_PARAMETER;
	    
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGprsGetMInQos);

	ret = CFW_CfgNumRangeCheck(nCid, GPRS_CIND_RANGE);
	if(ret!= ERR_SUCCESS)
	{
	    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret=0x%x\n",ret);
	    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetMInQos);
	    return ERR_CFG_PARAM_OUT_RANGR;
	}

    nCid = nCid + 0x30;
    Cid = &nCid;
    SUL_StrCopy(name, "MINQOS");
    SUL_StrCat(name, Cid);

	ret = cfg_read_hex("PDP", name, (PVOID) pQos, sizeof(CFW_GPRS_QOS));
	if(ERR_SUCCESS != ret)
	{
		ret = cfg_readdefault("PDP", name, (PVOID) pQos);
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGprsGetReqQos,1, call cfg_readdefault failed\n");
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetMInQos);
			return ret;
		}
	}
	else
	{
	    if((pQos->nDelay > 7) || (pQos->nMean > 31) || (pQos->nPeak > 15) 
			|| (pQos->nPrecedence > 7) || (pQos->nReliability > 7))
		{
			ret = cfg_readdefault("PDP", name, (PVOID) pQos);
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGprsGetReqQos,2, call cfg_readdefault failed\n");
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGprsGetMInQos);
				return ret;
			}
		}
	}

    return ret;
}
#endif


//=================================================================
//CFW_CfgNwSetFM 
//
//=================================================================
UINT32 CFW_CfgNwSetFM( UINT8 bMode )
{
	UINT32 nRet = ERR_SUCCESS;

  CSW_PROFILE_FUNCTION_ENTER(CFW_CfgNwSetFM);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwSetFM, bMode =0x%x\n", bMode);
	nRet = cfg_write_8("nw", "NW_FLIGHT_MODE", bMode);

  CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwSetFM);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwSetFM exit\n");
  
	return nRet;
}


//=================================================================
//CFW_CfgNwGetFM 
//
//=================================================================
UINT32 CFW_CfgNwGetFM(UINT8 *bMode)
{
	UINT32 nRet= ERR_SUCCESS;
	BOOL bDef = FALSE;
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGprsGetReqQos, enter\n");
	if( NULL == bMode )
	{
		return ERR_CFW_INVALID_PARAMETER;
	}
	else
	{
	    CSW_PROFILE_FUNCTION_ENTER(CFW_CfgNwGetFM);
      nRet = cfg_read_8("nw", "NW_FLIGHT_MODE", bMode);
      if(ERR_SUCCESS != nRet)
      {
   	     CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_32, error nRet=0x%x\n",nRet);
       nRet = cfg_readdefault("nw", "NW_FLIGHT_MODE", bMode);
       if(ERR_SUCCESS != nRet)
       {
         CSW_TRACE(CFW_CFG_TS_ID, "in NW_FLIGHT_MODE,1, call cfg_readdefault false,ret=0x%x\n",nRet);
         CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetNetWorkMode);
         return nRet;
       }
       bDef = TRUE;
      }
      else
      {
       nRet = CFW_CfgNumRangeCheck(*bMode, NW_FLIGHTMODE_RANGE);
       if(ERR_SUCCESS != nRet)
       {
         nRet = cfg_readdefault("nw", "NW_FLIGHT_MODE", bMode);
         if(ERR_SUCCESS != nRet)
         {
           CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgNwGetFM,1, call cfg_readdefault false,ret=0x%x\n",nRet);
           CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetNetWorkMode);
           return nRet;
         }
         bDef = TRUE;
       }
      }
	}
  
	if(bDef)
  {
		cfg_write_8("nw", "NW_FLIGHT_MODE", *bMode);
  }

  CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetFM);	
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwGetFM exit\n");
	return ERR_SUCCESS;
	
}

#endif

/*==============================================================================


==============================================================================*/
UINT32 CFW_CfgNwGetOperatorId (
  UINT8** pOperatorId,
  UINT8* pOperatorName
)
{
	//UINT32 ret = ERR_SUCCESS;
    UINT32 nIndex = 0;
    INT32  nCmpResult = 0;
    const UINT8 *pName = 0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgNwGetOperatorId);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwGetOperatorId enter\n");
    do
    {
        pName = OperatorInfo[nIndex].OperatorName;
        nCmpResult = CFW_Cfg_strcmp(pOperatorName, pName);
        
        if(!nCmpResult)
        {
            *pOperatorId = (UINT8*)OperatorInfo[nIndex].OperatorId;
            CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetOperatorId);
            return ERR_SUCCESS;
        }
        nIndex++;
        
    }while( pName != 0 );
    
    *pOperatorId = 0;
    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetOperatorId);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwGetOperatorId exit\n");
    return ERR_CFW_NOT_EXIST_OPERATOR_NAME;

}

UINT32 CFW_CfgNwGetOperatorName (
  UINT8 pOperatorId[6],
  UINT8** pOperatorName
)
{
   // UINT32 ret = ERR_SUCCESS;
    UINT32 nIndex = 0;
    INT32  nCmpResult = 0;
    UINT8 i = 0;
    const UINT8 *pOperID = 0;
    const UINT8 *pName = 0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgNwGetOperatorName);
	CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgNwGetOperatorName,input pOperatorId=%d%d%d%d%d%d\n",pOperatorId[0],pOperatorId[1],pOperatorId[2],pOperatorId[3],pOperatorId[4],pOperatorId[5]);
    do
    {
        pName = OperatorInfo[nIndex].OperatorName;
        pOperID = OperatorInfo[nIndex].OperatorId;
        
        if(pName == 0)
        {
            *pOperatorName = 0;
            CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetOperatorName);
            return ERR_CFW_NOT_EXIST_OPERATOR_ID;
        }
        
        nCmpResult = 0;
        for(i = 0; i < 6; i++)
        {   
            if( *pOperID == pOperatorId[i] )
            {
                pOperID++;
            }
            else
            {
                nCmpResult = 1;

                break;
            }
        }
        
        if(!nCmpResult)
        {
            *pOperatorName = OperatorInfo[nIndex].OperatorName; 
            CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetOperatorName);
            return ERR_SUCCESS;
        }
        nIndex++;
        
    }while( pName != 0 );
    
    *pOperatorName = 0;
    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetOperatorName);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwGetOperatorName exit\n");
    return ERR_CFW_NOT_EXIST_OPERATOR_ID;
}

UINT32 CFW_CfgNwGetOperatorInfo (UINT8** pOperatorId, UINT8** pOperatorName, UINT8 nIndex)
{
    //UINT32 ret = ERR_SUCCESS;
    UINT8 n = 0;
    UINT8 *pName = 0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgNwGetOperatorInfo);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwGetOperatorId enter\n");
    do
    {
        pName = OperatorInfo[n].OperatorName;
        
        if(pName == 0)
        {
            *pOperatorName = 0;
            CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetOperatorInfo);
            return ERR_CFW_NOT_EXIST_OPERATOR_ID;
        }
        
        if( n == nIndex ) 
        {
            *pOperatorId   =(UINT8*) OperatorInfo[nIndex].OperatorId;
            *pOperatorName =( UINT8*)OperatorInfo[nIndex].OperatorName;
            CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetOperatorInfo);
            return ERR_SUCCESS;
        }

        n++;
        
    }while( pName != 0 );
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNwGetOperatorInfo);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNwGetOperatorName exit\n");
    return ERR_CFW_NOT_EXIST_OPERATOR_ID;
}
/*=============================================================================
Get display format of Network, when register.
  
==============================================================================*/
UINT32 CFW_CfgSetNwOperDispFormat(UINT8 format)
{
    UINT32 ret = ERR_SUCCESS;

	ret = CFW_CfgNumRangeCheck(format, NW_DispFormat_RANGE);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetNwOperDispFormat, format = 0x%x,ret=0x%x\n", format,ret);
	if(ERR_SUCCESS == ret)
	{
		g_nNwDispFormat = format;
	}
	return ERR_CFW_NOT_SUPPORT;
}
       
UINT32 CFW_CfgGetNwOperDispFormat(UINT8 *format)
{
    	UINT32 ret = ERR_SUCCESS;  //mabo changed,20070726
    //READ_NUMBER_SETTING_8("REG_DISP_FORMAT", *format, "nw");
	*format = g_nNwDispFormat;
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetNwOperDispFormat, format=%d\n",*format);
    return ret;

}

//add for set APN for Jmetoc application by wuys, 2010-08-25
UINT32 CFW_SetJmetocApn(const INT8 *apn)
{
  UINT32 ret = ERR_SUCCESS;
  HKEY hkey; 
  REG_OpenKey(REG_HKEY_CURRENT_COMM, "JMETOC", &hkey);
  if(apn == NULL)
  {
        return ERR_EMPTY_PRI_DNSSERVER_IP;
  }
  CSW_TRACE(CFW_CFG_TS_ID, TSTXT(" CFW_SetJmetocApn apn\n"));
  ret = REG_SetValue(hkey, "APN", REG_TYPE_STRING,(PVOID) apn, strlen(apn));
  REG_CloseKey(REG_HKEY_CURRENT_COMM);
  return ret;
  
}
UINT32 CFW_SetJmetocUser(const INT8 *user)
{
  UINT32 ret = ERR_SUCCESS;
  HKEY hkey; 
  REG_OpenKey(REG_HKEY_CURRENT_COMM, "JMETOC", &hkey);
  if(user == NULL)
  {
        return ERR_EMPTY_PRI_DNSSERVER_IP;
  }
  CSW_TRACE(CFW_CFG_TS_ID, TSTXT(" CFW_SetJmetocApn apn\n"));
  ret = REG_SetValue(hkey, "USER", REG_TYPE_STRING,(PVOID) user, strlen(user));
  REG_CloseKey(REG_HKEY_CURRENT_COMM);
  return ret;
  
}
UINT32 CFW_SetJmetocPwd(INT8 * pwd)
{
  UINT32 ret = ERR_SUCCESS;
  HKEY hkey; 
  REG_OpenKey(REG_HKEY_CURRENT_COMM, "JMETOC", &hkey);
  if(pwd == NULL)
  {
        return ERR_EMPTY_PRI_DNSSERVER_IP;
  }
  CSW_TRACE(CFW_CFG_TS_ID, TSTXT(" CFW_SetJmetocApn apn\n"));
  ret = REG_SetValue(hkey, "PWD", REG_TYPE_STRING,(PVOID) pwd, strlen(pwd));
  REG_CloseKey(REG_HKEY_CURRENT_COMM);
  return ret;
  
  
}

UINT32 CFW_GetJmetocApn(INT8 * apn)
{
  UINT32 ret = ERR_SUCCESS;
  HKEY hkey=0; 
  UINT8 uType = REG_TYPE_STRING;
  UINT8 pApnSize = 63;
  
  REG_OpenKey(REG_HKEY_CURRENT_COMM, "JMETOC", &hkey);
  
  ret = REG_GetValue(hkey, "APN", &uType,(PVOID) apn, &pApnSize);
  
  REG_CloseKey(REG_HKEY_CURRENT_COMM);
  
  return ret;
  
}

UINT32 CFW_GetJmetocUser(INT8 * username)
{
  UINT32 ret = ERR_SUCCESS;
  HKEY hkey=0; 
  UINT8 uType = REG_TYPE_STRING;
  UINT8 pUserSize = 63;
  
  REG_OpenKey(REG_HKEY_CURRENT_COMM, "JMETOC", &hkey);
  
  ret = REG_GetValue(hkey, "USER", &uType,(PVOID) username, &pUserSize);
  
  REG_CloseKey(REG_HKEY_CURRENT_COMM);
  
  return ret;
  
}
UINT32 CFW_GetJmetocPwd(INT8 * pwd)
{
  UINT32 ret = ERR_SUCCESS;
  HKEY hkey=0; 
  UINT8 uType = REG_TYPE_STRING;
  UINT8 pPwdSize = 63;
  
  REG_OpenKey(REG_HKEY_CURRENT_COMM, "JMETOC", &hkey);
  
  ret = REG_GetValue(hkey, "PWD", &uType,(PVOID) pwd, &pPwdSize);
  
  REG_CloseKey(REG_HKEY_CURRENT_COMM);
  
  return ret;
  
}
//add end
