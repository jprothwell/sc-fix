///*************************************************************
//hameina 2006.8.21 modified
//
//notice: this version is based on reg systerm
//
//modify record:
//2006.9.14 hameina [mod] ERR_CFW_NOTSUPPORT->ERR_CFW_NOT_SUPPORT, who define this
//    wrong err code murdered hameina.
///**************************************************************

#include <cfw.h>
#include "cfw_cfg.h"
#include "cfw_prv.h"
/*==========================================================================
Select the current phonebook memory storage.

KEY: "PH_BK"
==========================================================================*/
#if 0
UINT16 g_Pbk_nStorage1 = 0;
UINT16 g_Pbk_nStorage2 = 0;
UINT16 g_Pbk_nStorage3 = 0;
#endif

#ifdef CFW_MULTI_SIM
UINT8 g_PbkStorage[CFW_SIM_COUNT] ={ 2,};

#else
UINT8 g_PbkStorage = 2;//hameina [+] 2006.8.23
#endif

#ifdef CFW_MULTI_SIM
UINT32 CFW_CfgSetPbkStorage (UINT8 nStorage, CFW_SIM_ID nSimID)
#else
UINT32 CFW_CfgSetPbkStorage (UINT8 nStorage)
#endif
{
	UINT32 ret = ERR_SUCCESS;
#ifdef CFW_MULTI_SIM
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetPbkStorage, nStorage=0x%x, nSimID=0x%x\n", nStorage, nSimID);
#else
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetPbkStorage, nStorage=0x%x\n", nStorage);
#endif
#ifdef CFW_MULTI_SIM
	ret = CFW_CheckSimId(nSimID);	
	if( ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "CFW_CheckSimId, error ret =0x%x\n", ret);
		return ret;
	}
#endif
	ret = CFW_CfgNumRangeCheck(nStorage, PBK_STORAGE_RANGE);
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret =0x%x\n", ret);
		ret = ERR_CFG_PARAM_OUT_RANGR;
	}
	else
	{
#ifdef CFW_MULTI_SIM
           if((nSimID>=CFW_SIM_0)&&(nSimID<CFW_SIM_COUNT))
           {
		
		    g_PbkStorage[nSimID] = nStorage;
		
             }else
		{
			ret = ERR_CFW_INVALID_PARAMETER;
		}

#else
		g_PbkStorage = nStorage;
#endif
	}
    return ret;
        
}


/*==========================================================================
Get the current phonebook memory storage.

KEY: "PH_BK"
==========================================================================*/
#ifdef CFW_MULTI_SIM
UINT32 CFW_CfgGetPbkStorage (UINT8 *nStorage, CFW_SIM_ID nSimID)
#else
UINT32 CFW_CfgGetPbkStorage (UINT8 *nStorage)
#endif
{
	UINT32 ret = ERR_SUCCESS;

#ifdef CFW_MULTI_SIM
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetPbkStorage,  nSimID=0x%x\n", nSimID);
#else
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetPbkStorage, enter\n");
#endif
#ifdef CFW_MULTI_SIM

	ret = CFW_CheckSimId(nSimID);	
	if( ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "CFW_CheckSimId, error ret =0x%x\n", ret);
		return ret;
	}
       if((nSimID>=CFW_SIM_0)&&(nSimID<CFW_SIM_COUNT))
       {
	
	    *nStorage = g_PbkStorage[nSimID];
	
	}
	else
	{
		ret = ERR_CFW_INVALID_PARAMETER;
	}
#else
    *nStorage = g_PbkStorage;
#endif

    return ret;        
}


/*==========================================================================
typedef struct _CFW_PBK_STRORAGE_INFO { 
 UINT16 index;
 UINT8 iNumberLen;
 UINT8 txtLen;
} CFW_PBK_STRORAGE_INFO;
==========================================================================*/
UINT32 SRVAPI CFW_CfgSetPbkStrorageInfo (
  CFW_PBK_STRORAGE_INFO* pStorageInfo
)
{
#if 0
    UINT32 *pInteger = 0;
    pInteger = (UINT32*)pStorageInfo;

    #ifndef MF32_BASE_REG_DISABLE
    WRITE_NUMBER_SETTING("BK_INFO", *pInteger, "pbk");
    #endif 

    return ERR_SUCCESS;
#endif
	return ERR_CFW_NOT_SUPPORT;
}


UINT32 SRVAPI CFW_CfgGetPbkStrorageInfo (
  CFW_PBK_STRORAGE_INFO* pStorageInfo
  )
{
#if 0
    INT32 integer = 0;

    #ifndef MF32_BASE_REG_DISABLE
    READ_NUMBER_SETTING("BK_INFO", integer, "pbk");   
    #endif 
    
    SUL_MemCopy32((void*)pStorageInfo, &integer, 4);
    
    return ERR_SUCCESS;
#endif
	return ERR_CFW_NOT_SUPPORT;

}

//PRIVATE CFW_PBK_CAPACITY_INFO g_PbkCapInfo[2] = {{0,}, };

UINT32 CFW_CfgSetPbkCapacityInfo (
  UINT8 nStorageId,
  CFW_PBK_CAPACITY_INFO* pStorageInfo
)
{
#if 0
    UINT8 n = 0;

    if(nStorageId == CFW_PBK_ME)
        n = 0;
    else  if(nStorageId == CFW_PBK_SIM)
        n = 1;
    else
        return ERR_INVALID_PARAMETER;

    SUL_MemCopy32(&g_PbkCapInfo[n], pStorageInfo, SIZEOF(CFW_PBK_CAPACITY_INFO));

    return ERR_SUCCESS;
#endif
	return ERR_CFW_NOT_SUPPORT;

}

//PRIVATE CFW_PBK_CAPACITY_INFO g_CalllogCapInfo[7] = {{0, },};

UINT32 CFW_CfgSetCalllogCapacityInfo (
  UINT8 nStorageId,
  CFW_PBK_CAPACITY_INFO* pStorageInfo
)
{
#if 0
    UINT8 n = 0;

    if(nStorageId == CFW_PBK_ME_MISSED)
        n = 0;
    else  if(nStorageId == CFW_PBK_ME_RECEIVED)
        n = 1;
    else  if(nStorageId == CFW_PBK_SIM_FIX_DIALLING)
        n = 2;
    else  if(nStorageId == CFW_PBK_SIM_LAST_DIALLING)
        n = 3;
    else  if(nStorageId == CFW_PBK_ME_LAST_DIALLING)
        n = 4;
    else  if(nStorageId == CFW_PBK_ON)
        n = 5;   
    else  if(nStorageId == CFW_PBK_EN)
        n = 6;      
    else
        return ERR_INVALID_PARAMETER;

    SUL_MemCopy32(&g_PbkCapInfo[n], pStorageInfo, SIZEOF(CFW_PBK_CAPACITY_INFO));

    return ERR_SUCCESS;
#endif
	return ERR_CFW_NOT_SUPPORT;

}

