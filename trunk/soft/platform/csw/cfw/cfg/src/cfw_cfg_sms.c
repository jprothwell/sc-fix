///*************************************************************
//hameina 2006.8.21 modified
//
//notice: this version is based on reg systerm
//
//modify record:
//2006.9.1 hameina CFW_CfgGetSmsStorageInfo() add param verify
//2006.9.13 hameina CFW_CfgGetNewSmsOption , [mod] cause the param range is 0~255,
//        so delete the param check code
//2006.9.14 hameina [mod] ERR_CFW_NOTSUPPORT->ERR_CFW_NOT_SUPPORT, who define this
//    wrong err code murdered hameina.
//2006.9.29 hameina [mod] cfg_readdefault() the type of function changed:bool->UINT32
///**************************************************************

#include "cfw.h"
#include "sul.h"

#include "cfw_cfg.h"
#include "csw_debug.h"
#include <string.h>
//
// SMS Configuration
// 
#ifndef CFW_MULTI_SIM
UINT8 g_SmsFormat = 0; 
UINT8 g_SmsShowTextMode = 0;
#else
UINT8 g_SmsFormat_Sim0 = 0; 
UINT8 g_SmsFormat_Sim1 = 0; 
UINT8 g_SmsFormat_Sim2 = 0; 
UINT8 g_SmsFormat_Sim3 = 0;
UINT8 g_SmsShowTextMode_Sim0 = 0;
UINT8 g_SmsShowTextMode_Sim1 = 0;
UINT8 g_SmsShowTextMode_Sim2 = 0;
UINT8 g_SmsShowTextMode_Sim3 = 0;

#endif
static UINT32 sms_SetParamAoProc(HAO hAo, CFW_EV* pEvent);

#ifdef CFW_MULTI_SIM

UINT32 CFW_CfgSetSmsStorage (UINT8 nStorage, 
                             CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	//UINT32 ret2 = ERR_SUCCESS  ;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSmsStorage);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsStorage  nStorage = 0x%x, nSimID =0x%x\n", nStorage, nSimID);
	ret = CFW_CfgNumRangeCheck(nStorage, SMS_STORAGE_RANGE);
	if(ERR_SUCCESS == ret)
	{
		INT8 simid_c[2]  = {0x30,0x00};
		INT8 name_s[14] = {0x00,};
		strcpy(name_s,"SMS_STORAGE_");
		
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			ret = cfg_write_8("sms", name_s, nStorage);

		}else{
		       ret = ERR_CFW_INVALID_PARAMETER;
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
			//return ret;
		}
	}else
	{
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret =0x%x\n", ret);
			ret = ERR_CFG_PARAM_OUT_RANGR;
	}

	

	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsStorage);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsStorage exit\n");
	return ret;    
}

UINT32 CFW_CfgGetSmsStorage ( UINT8* pStorage, 
                              CFW_SIM_ID nSimID)
{
    
	UINT32 ret = ERR_SUCCESS;
	BOOL bDef = FALSE;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSmsStorage);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsStorage  nSimID =0x%x\n", nSimID);

	INT8 simid_c[2]  = {0x30,0x00};
		INT8 name_s[14] = {0x00,};
		strcpy(name_s,"SMS_STORAGE_");
		
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			ret = cfg_read_8("sms", name_s, pStorage);

			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsStorage,1, call cfg_read_8, error ret=0x%x\n",ret);
				ret = cfg_readdefault("sms", name_s, pStorage);
				if(ERR_SUCCESS != ret)
				{
					CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsStorage,1, call cfg_readdefault, error ret=0x%x\n",ret);
					CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsStorage);
					return ret;
				}
				bDef = TRUE;
			}
			else
			{
				ret = CFW_CfgNumRangeCheck(*pStorage, SMS_STORAGE_RANGE);
				if(ERR_SUCCESS != ret)
				{
					CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsStorage,2, call CFW_CfgNumRangeCheck, error ret=0x%x\n",ret);
					ret = cfg_readdefault("sms", name_s, pStorage);
					if(ERR_SUCCESS != ret)
					{
						CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsStorage,2, call cfg_readdefault, error ret=0x%x\n",ret);
						CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsStorage);
						return ret;
					}
					bDef = TRUE;
				}
			}

		}else{
		       ret = ERR_CFW_INVALID_PARAMETER;
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
			//return ret;
		}
	
	
	if(bDef)
	{
		
		cfg_write_8("sms", name_s, *pStorage);
		
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsStorage);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsStorage  *pStorage =0x%x\n", *pStorage);
	return ret;	
}

UINT32 CFW_CfgSetSmsOverflowInd ( UINT8 nMode, CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSmsOverflowInd);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsOverflowInd  nMode = 0x%x, nSimID =0x%x\n", nMode, nSimID);
	ret = CFW_CfgNumRangeCheck(nMode, SMS_OVERFLOW_RANGE);
	if(ERR_SUCCESS == ret)
	{
	       INT8 simid_c[2]  = {0x30,0x00};
	       INT8 name_s[20] = {0x00,};
		INT8 name_s1[11] = {0x00,};

		strcpy(name_s,"SMS_SIM");
		strcpy(name_s1,"_OVER_IND");
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			strcat(name_s, name_s1);
			ret = cfg_write_8("sms", name_s, nMode);

		}else{
		       ret = ERR_CFW_INVALID_PARAMETER;
		}
		
	}
	else
		ret = ERR_CFG_PARAM_OUT_RANGR;
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsOverflowInd);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsOverflowInd  ret =0x%x\n", ret);
	return ret;
}

UINT32 CFW_CfgGetSmsOverflowInd ( UINT8* pMode, CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	BOOL bDef = FALSE;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetSmsOverflowInd);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsOverflowInd, nSimID =0x%x\n", nSimID);

    INT8 simid_c[2]  = {0x30,0x00};
    INT8 name_s[20] = {0x00,};
    INT8 name_s1[11] = {0x00,};

		strcpy(name_s,"SMS_SIM");
		strcpy(name_s1,"_OVER_IND");
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			strcat(name_s, name_s1);
			ret = cfg_read_8("sms", name_s, pMode);

		}else{
		       ret = ERR_CFW_INVALID_PARAMETER;
		}

	

	if(ret == ERR_SUCCESS)
	{
		ret = CFW_CfgNumRangeCheck(*pMode, SMS_OVERFLOW_RANGE);
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, ret =0x%x\n", ret);

			if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
			{
			
				ret = cfg_readdefault("sms", name_s, pMode);

			}else{
		       	ret = ERR_CFW_INVALID_PARAMETER;
			}
		
			
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsOverflowInd,1,call cfg_readdefault falsed, ret=0x%x\n",ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSmsOverflowInd);
				return ret;
			}
			bDef = TRUE;
		}
	}
	else
	{
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
			{
			
				ret = cfg_readdefault("sms", name_s, pMode);

			}else{
		       	ret = ERR_CFW_INVALID_PARAMETER;
			}	
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsOverflowInd,1,call cfg_readdefault falsed, ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSmsOverflowInd);
			return ret;
		}
		bDef = TRUE;
	}
	if(bDef)
	{
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
			{
			
				ret = cfg_write_8("sms", name_s, *pMode);

			}else{
		       	ret = ERR_CFW_INVALID_PARAMETER;
			}	
		
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSmsOverflowInd);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsOverflowInd, *pMode =0x%x\n", *pMode);
	return ret;
}

UINT32 CFW_CfgSetSmsFormat(UINT8 nFormat, CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSmsFormat);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsOverflowInd, nFormat = 0x%x,nSimID =0x%x\n", nFormat, nSimID);
	if(nFormat == 0 || nFormat == 1)
	{
		if(CFW_SIM_0==nSimID)
		{
			g_SmsFormat_Sim0 = nFormat;
		}
		else if(CFW_SIM_1==nSimID)
		{
			g_SmsFormat_Sim1 = nFormat;
		}else if(CFW_SIM_1+1==nSimID)
		{
			g_SmsFormat_Sim2 = nFormat;
		}else if(CFW_SIM_1+2==nSimID)
		{
			g_SmsFormat_Sim3 = nFormat;
		}
		else
		{
			ret = ERR_CFW_INVALID_PARAMETER;
		}
	}
	else
	{
		ret = ERR_CFG_PARAM_OUT_RANGR;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsFormat);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsFormat exit\n");
	return ret;
}

UINT32 CFW_CfgGetSmsFormat(UINT8 *nFormat , CFW_SIM_ID nSimID)
{
	if(CFW_SIM_0==nSimID)
	{
		*nFormat = g_SmsFormat_Sim0;
	}
	else if(CFW_SIM_1==nSimID)
	{
		*nFormat = g_SmsFormat_Sim1;
	}else if(CFW_SIM_1+1==nSimID)
	{
		*nFormat = g_SmsFormat_Sim2;
	}else if(CFW_SIM_1+2==nSimID)
	{
		*nFormat = g_SmsFormat_Sim3;
	}
	else
	{
		return ERR_CFW_INVALID_PARAMETER;
	}
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSmsFormat, *nFormat = 0x%x,nSimID =0x%x\n", *nFormat, nSimID);
	return ERR_SUCCESS;
    
}

UINT32 CFW_CfgSetSmsShowTextModeParam ( UINT8 nShow , CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;

	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsShowTextModeParam, nShow = 0x%x,nSimID =0x%x\n", nShow, nSimID);
	if((nShow == 0) || (nShow == 1))
	{  
		if(CFW_SIM_0==nSimID)
		{
			g_SmsShowTextMode_Sim0 = nShow;
		}
		else if (CFW_SIM_1==nSimID)
		{
			g_SmsShowTextMode_Sim1 = nShow;
		}else if (CFW_SIM_1+1==nSimID)
		{
			g_SmsShowTextMode_Sim2 = nShow;
		}else if (CFW_SIM_1+2==nSimID)
		{
			g_SmsShowTextMode_Sim3 = nShow;
		}
		else
		{
			ret = ERR_CFW_INVALID_PARAMETER;
		}
	}
	else
		ret = ERR_CFG_PARAM_OUT_RANGR;

	return ret;
    
}

UINT32 CFW_CfgGetSmsShowTextModeParam ( UINT8* pShow , CFW_SIM_ID nSimID)
{
	if(CFW_SIM_0==nSimID)
	{
	        * pShow = g_SmsShowTextMode_Sim0;
	}
	else if (CFW_SIM_1==nSimID)
	{
	        * pShow = g_SmsShowTextMode_Sim1;
	}else if (CFW_SIM_1+1==nSimID)
	{
	        * pShow = g_SmsShowTextMode_Sim2;
	}else if (CFW_SIM_1+2==nSimID)
	{
	        * pShow = g_SmsShowTextMode_Sim3;
	}
	else
	{
	        return ERR_CFW_INVALID_PARAMETER;
	}
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSmsShowTextModeParam, *pShow = 0x%x,nSimID =0x%x\n", *pShow, nSimID);
	return ERR_SUCCESS;
}

UINT32 CFW_CfgSetSmsFullForNew ( UINT8 n, CFW_SIM_ID nSimID)
{

	UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSmsFullForNew);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsFullForNew, n = 0x%x,nSimID =0x%x\n", n, nSimID);
	ret = CFW_CfgNumRangeCheck(n, SMS_FULLFORNEW_RANGE);
	if(ERR_SUCCESS == ret)
	{
    INT8 simid_c[2]  = {0x30,0x00};
		INT8 name_s[18] = {0x00,};
		strcpy(name_s,"FULL_FOR_NEW_SIM");
		
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			ret = cfg_write_8("sms", name_s, n);

		}else{
		       ret = ERR_CFW_INVALID_PARAMETER;
		}
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsFullForNew);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsFullForNew, ret =0x%x\n", ret);
	return ret;
}


UINT32 CFW_CfgGetSmsFullForNew ( UINT8 *pn, CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	BOOL bDef = FALSE;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetSmsFullForNew);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsFullForNew, nSimID =0x%x\n", nSimID);

		      INT8 simid_c[2]  = {0x30,0x00};
		INT8 name_s[18] = {0x00,};
		strcpy(name_s,"FULL_FOR_NEW_SIM");
		
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			ret = cfg_read_8("sms", name_s, pn);

		}else{
		       ret = ERR_CFW_INVALID_PARAMETER;
		}
	
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret =0x%x\n", ret);
             if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			ret = cfg_readdefault("sms", name_s, pn);

		}else{
		       ret = ERR_CFW_INVALID_PARAMETER;
		}
		
		
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsFullForNew,1, call cfg_readdefault falsed, ret =0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSmsFullForNew);
			return ret;
		}
		bDef = TRUE;
	}
	else
	{
		ret = CFW_CfgNumRangeCheck(*pn, SMS_FULLFORNEW_RANGE);
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsFullForNew,2, call CFW_CfgNumRangeCheck falsed, ret =0x%x\n",ret);
	    		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
			{
				simid_c[0] = simid_c[0] + nSimID;
				strcat(name_s, simid_c);
				ret = cfg_readdefault("sms", name_s, pn);

			}else{
			       ret = ERR_CFW_INVALID_PARAMETER;
			}
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsFullForNew,2, call cfg_readdefault falsed, ret =0x%x\n",ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSmsFullForNew);
				return ret;
			}
		}
		bDef = TRUE;
	}
	if(bDef)
	{
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			ret = cfg_write_8("sms", name_s, *pn);

		}else{
		       ret = ERR_CFW_INVALID_PARAMETER;
		}
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSmsFullForNew);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSmsFullForNew, *pn =0x%x\n", *pn);
	return ERR_SUCCESS;
}

PUBLIC UINT32 SRVAPI CFW_CfgSetSmsParam (
  CFW_SMS_PARAMETER* pInfo,
  UINT16 nUTI,
  CFW_SIM_ID nSimID
)
{
	UINT32 ret = ERR_SUCCESS;
	UINT8 nIndex;
	if(pInfo == NULL)
	return ERR_CFG_PARAM_OUT_RANGR;
	if(pInfo->nIndex>4)
	return ERR_CFG_PARAM_OUT_RANGR;


	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSmsParam);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsParam, nSimID =0x%x\n", nSimID);
	nIndex = pInfo->nIndex;
	INT8 simid_c[2]  = {0x30,0x00};
	INT8 name_s[REG_VALUENAME_LEN_MAX+1] = {0x00,};

	if (0 == pInfo->nSaveToSimFlag)
	{
		if(pInfo->bDefault)
		{

			strcpy(name_s,"DEF_PARAM_IND_SIM");
			
			if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
			{
				simid_c[0] = simid_c[0] + nSimID;
				strcat(name_s, simid_c);
				ret = cfg_write_8("sms", name_s, nIndex);

			}else{
			       ret = ERR_CFW_INVALID_PARAMETER;
			}
			
			
			if(ERR_SUCCESS != ret)
			{
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsParam);
				return ret;
			}
		}
		if(pInfo)
		{
			UINT8 name[REG_VALUENAME_LEN_MAX+1];
			if(CFW_SIM_0==nSimID)
			{
				SUL_StrPrint(name, "SMS_SIM0_PARAM%d",nIndex+1);
			}
			else if (CFW_SIM_1==nSimID)
			{
				SUL_StrPrint(name, "SMS_SIM1_PARAM%d",nIndex+1);
			}else if (CFW_SIM_1+1==nSimID)
			{
				SUL_StrPrint(name, "SMS_SIM2_PARAM%d",nIndex+1);
			}else if (CFW_SIM_1+2==nSimID)
			{
				SUL_StrPrint(name, "SMS_SIM3_PARAM%d",nIndex+1);
			}
			else
			{
				ret = ERR_CFW_INVALID_PARAMETER;
			}
			ret = cfg_write_hex("sms", name, pInfo,sizeof(CFW_SMS_PARAMETER));
		}
    		CFW_RelaseUsingUTI( CFW_APP_SRV_ID , nUTI );
	}
	else 
	{
		CFW_SMS_PARAMETER *pParamInfo = NULL;
		CFW_SMS_PARAMETER *pStoredParam = NULL;
		HAO hAo = 0;

		ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);

		if(ret != ERR_SUCCESS)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_IsFreeUTI, error ret =0x%x\n", ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsWriteMessage);
			return ret;
		}

		pParamInfo = (CFW_SMS_PARAMETER*)CFW_MALLOC(SIZEOF(CFW_SMS_PARAMETER) * 2);
		if (NULL == pParamInfo)
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsParam);
			return ERR_CMS_MEMORY_FULL;
		}
		SUL_ZeroMemory8(pParamInfo, SIZEOF(CFW_SMS_PARAMETER) * 2);
		SUL_MemCopy8(pParamInfo, pInfo, SIZEOF(CFW_SMS_PARAMETER)); 

		pStoredParam = pParamInfo + SIZEOF(CFW_SMS_PARAMETER);
		ret = CFW_CfgGetSmsParam(pStoredParam, pInfo->nIndex, nSimID);

		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSmsParam, error ret =0x%x\n", ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsParam);
			CFW_FREE(pParamInfo);
			return ret;
		}

		if(pInfo->bDefault)
		{
			if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
			{
				simid_c[0] = simid_c[0] + nSimID;
				
				strcpy(name_s,"DEF_PARAM_IND_SIM");
				strcat(name_s, simid_c);
				
				ret = cfg_write_8("sms", name_s, nIndex);

			}else{
			       ret = ERR_CFW_INVALID_PARAMETER;
			}
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_8, error ret =0x%x\n", ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsParam);
				CFW_FREE(pParamInfo);
				return ret;
			}
		}
		if(pInfo)
		{
			UINT8 name[REG_VALUENAME_LEN_MAX+1] ={ 0x00, };
			if(CFW_SIM_0==nSimID)
			{
				SUL_StrPrint(name, "SMS_SIM0_PARAM%d",nIndex+1);
			}
			else if (CFW_SIM_1==nSimID)
			{
				SUL_StrPrint(name, "SMS_SIM1_PARAM%d",nIndex+1);
			}else if (CFW_SIM_1+1==nSimID)
			{
				SUL_StrPrint(name, "SMS_SIM2_PARAM%d",nIndex+1);
			}else if (CFW_SIM_1+2==nSimID)
			{
				SUL_StrPrint(name, "SMS_SIM3_PARAM%d",nIndex+1);
			}
			else
			{
				ret = ERR_CFW_INVALID_PARAMETER;
			}
			ret = cfg_write_hex("sms", name, pInfo,sizeof(CFW_SMS_PARAMETER));
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_hex, error ret =0x%x\n", ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsParam);
				CFW_FREE(pParamInfo);
				return ret;
			}
		}
		hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pParamInfo, sms_SetParamAoProc, nSimID);
		CFW_SetUTI(hAo,nUTI,0);
		CFW_SetAoProcCode(hAo,CFW_AO_PROC_CODE_CONTINUE);
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsParam);
		return ERR_SUCCESS;
	}

	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsParam);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsParam, exit ret =0x%x\n", ret);
	return ret;
}

UINT32 CFW_CfgGetSmsParam (
  CFW_SMS_PARAMETER* pInfo,
  UINT8 nIndex,
  CFW_SIM_ID nSimID
)
{
	UINT32 ret = ERR_SUCCESS;
	UINT8 name[REG_VALUENAME_LEN_MAX+1];

	if((nIndex>4)||(pInfo==NULL))
		return ERR_CFG_PARAM_OUT_RANGR;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetSmsParam);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsParam, nIndex= 0x%x, nSimID =0x%x\n", nIndex, nSimID);
	if(CFW_SIM_0==nSimID)
	{
		SUL_StrPrint(name, "SMS_SIM0_PARAM%d",nIndex+1);
	}
	else if (CFW_SIM_1==nSimID)
	{
		SUL_StrPrint(name, "SMS_SIM1_PARAM%d",nIndex+1);
	}else if (CFW_SIM_1+1==nSimID)
	{
		SUL_StrPrint(name, "SMS_SIM2_PARAM%d",nIndex+1);
	}else if (CFW_SIM_1+2==nSimID)
	{
		SUL_StrPrint(name, "SMS_SIM3_PARAM%d",nIndex+1);
	}
	else
	{
		ret = ERR_CFW_INVALID_PARAMETER;
	}
	ret = cfg_read_hex("sms", name,  pInfo, (UINT8)sizeof(CFW_SMS_PARAMETER));
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret=0x%x\n", ret);
		SUL_ZeroMemory8(pInfo, sizeof(CFW_SMS_PARAMETER));
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSmsParam);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSmsParam, exit \n");
	return ret;
}

UINT32 CFW_CfgGetDefaultSmsParam (
  CFW_SMS_PARAMETER* pInfo,
  CFW_SIM_ID nSimID
)
{
	UINT32 ret = ERR_SUCCESS;
	UINT8 name[REG_VALUENAME_LEN_MAX+1];
	UINT8 index =0;
	if(pInfo==NULL)
	return ERR_CFG_PARAM_OUT_RANGR;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetDefaultSmsParam);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetDefaultSmsParam, nSimID=0x%x\n", nSimID);

         INT8 simid_c[2]  = {0x30,0x00};
	INT8 name_s[REG_VALUENAME_LEN_MAX+1] = {0x00,};
	strcpy(name_s,"DEF_PARAM_IND_SIM");
	
	if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
	{
		simid_c[0] = simid_c[0] + nSimID;
		strcat(name_s, simid_c);
		ret = cfg_read_8("sms", name_s,  &index);

	}else{
	       ret = ERR_CFW_INVALID_PARAMETER;
	}
	
	if(ERR_SUCCESS !=ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret=0x%x\n", ret);
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			ret = cfg_readdefault("sms", name_s,  &index);

		}else{
		       ret = ERR_CFW_INVALID_PARAMETER;
		}
		
		
		if(ERR_SUCCESS !=ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetDefaultSmsParam,1, call cfg_readdefault falsed, ret =0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetDefaultSmsParam);
			return ret;
		}
		
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			ret = cfg_write_8("sms", name_s, index);

		}else{
		       ret = ERR_CFW_INVALID_PARAMETER;
		}
	}
	if(CFW_SIM_0==nSimID)
	{
		SUL_StrPrint(name, "SMS_SIM0_PARAM%d",index+1);
	}
	else if (CFW_SIM_1==nSimID)
	{
		SUL_StrPrint(name, "SMS_SIM1_PARAM%d",index+1);
	}else if (CFW_SIM_1+1==nSimID)
	{
		SUL_StrPrint(name, "SMS_SIM2_PARAM%d",index+1);
	}else if (CFW_SIM_1+2==nSimID)
	{
		SUL_StrPrint(name, "SMS_SIM3_PARAM%d",index+1);
	}
	else
	{
		ret = ERR_CFW_INVALID_PARAMETER;
	}

	ret = cfg_read_hex("sms", name, (PVOID) pInfo, (UINT8)sizeof(CFW_SMS_PARAMETER));
	if(ERR_SUCCESS != ret)
	{
		SUL_ZeroMemory8(pInfo, sizeof(CFW_SMS_PARAMETER));
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetDefaultSmsParam);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetDefaultSmsParam, exit \n");
	return ERR_SUCCESS;
}

UINT32 CFW_CfgSetDefaultSmsParam (
  CFW_SMS_PARAMETER* pInfo,
  CFW_SIM_ID nSimID
)
{
	UINT32 ret = ERR_SUCCESS;
	UINT8 name[REG_VALUENAME_LEN_MAX+1];
	UINT8 index =0;
	if(pInfo==NULL)
	return ERR_CFG_PARAM_OUT_RANGR;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetDefaultSmsParam);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetDefaultSmsParam, nSimID=0x%x\n", nSimID);
       INT8 simid_c[2]  = {0x30,0x00};
	INT8 name_s[REG_VALUENAME_LEN_MAX+1] = {0x00,};
	strcpy(name_s,"DEF_PARAM_IND_SIM");
	
	if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
	{
		simid_c[0] = simid_c[0] + nSimID;
		strcat(name_s, simid_c);
		ret = cfg_read_8("sms", name_s,  &index);

	}else{
	       ret = ERR_CFW_INVALID_PARAMETER;
	}
	
	

	if(ERR_SUCCESS !=ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret=0x%x\n", ret);

		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
	{
		simid_c[0] = simid_c[0] + nSimID;
		strcat(name_s, simid_c);
		ret = cfg_readdefault("sms", name_s,  &index);

	}else{
	       ret = ERR_CFW_INVALID_PARAMETER;
	}
		

		if(ERR_SUCCESS !=ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetDefaultSmsParam,1, call cfg_readdefault falsed, ret =0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetDefaultSmsParam);
			return ret;
		}
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			ret = cfg_write_8("sms", name_s, index);

		}else{
		       ret = ERR_CFW_INVALID_PARAMETER;
		}
	}

	if(CFW_SIM_0==nSimID)
	{
		SUL_StrPrint(name, "SMS_SIM0_PARAM%d",index+1);
	}
	else if (CFW_SIM_1==nSimID)
	{
		SUL_StrPrint(name, "SMS_SIM1_PARAM%d",index+1);
	}else if (CFW_SIM_1+1==nSimID)
	{
		SUL_StrPrint(name, "SMS_SIM2_PARAM%d",index+1);
	}else if (CFW_SIM_1+2==nSimID)
	{
		SUL_StrPrint(name, "SMS_SIM3_PARAM%d",index+1);
	}
	else
	{
		ret = ERR_CFW_INVALID_PARAMETER;
	}

	ret = cfg_write_hex("sms", name, (PVOID )pInfo,sizeof(CFW_SMS_PARAMETER));
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetDefaultSmsParam);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetDefaultSmsParam, exit \n");
	return ret;
}

UINT32 CFW_CfgSetNewSmsOption(
  UINT8 nOption, 
  UINT8 nNewSmsStorage,
  CFW_SIM_ID nSimID
)
{
	UINT32 ret = ERR_SUCCESS,ret1 = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetNewSmsOption);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetNewSmsOption, IN[%d,%d],nSimID=0x%x\n",nOption,nNewSmsStorage, nSimID);
	ret = CFW_CfgNumRangeCheck(nOption, SMS_OPTION_RANGE);
	ret1 = CFW_CfgNumRangeCheck(nNewSmsStorage, SMS_OPT_STORAGE_RABGE);

	
	if ((ERR_SUCCESS == ret)&&(ERR_SUCCESS == ret1))
	{	
		 INT8 simid_c[2]  = {0x30,0x00};
	       INT8 name_s[REG_VALUENAME_LEN_MAX+1] = {0x00,};
		INT8 name_s1[13] = {0x00,};

		strcpy(name_s,"SMS_SIM");
		strcpy(name_s1,"_OPTION");
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			strcat(name_s, name_s1);
			ret = cfg_write_8("sms", name_s, nOption);
			memset(name_s,0x00,sizeof(name_s));
			memset(name_s1,0x00,sizeof(name_s1));
			strcpy(name_s,"SMS_SIM");
		      strcpy(name_s1,"_OPTSTORAGE");
			strcat(name_s, simid_c);
			strcat(name_s, name_s1);

			ret1 = cfg_write_8("sms",name_s, nNewSmsStorage);


		}else{
		       ret = ERR_CFW_INVALID_PARAMETER;
		}

	
	
		
		if ((ERR_SUCCESS != ret)||(ERR_SUCCESS != ret1))
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in cfg_write_8, error IN[%d,%d],RET[0x%x,ret=0x%x]\n",nOption,nNewSmsStorage, ret,ret1);
			ret = ERR_CFG_WRITE_REG_FAILURE ;
		}
	}
	else
	{	
		CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgNumRangeCheck, error IN[%d,%d],RET[0x%x,ret=0x%x]\n",nOption,nNewSmsStorage, ret,ret1);
		ret = ERR_CFG_PARAM_OUT_RANGR ;
	}

	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetNewSmsOption);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetDefaultSmsParam, exit \n");
	return ret;

}


UINT32 CFW_CfgGetNewSmsOption(  UINT8* pOption,   UINT8* pNewSmsStorage, CFW_SIM_ID nSimID)
{
	if(pOption==NULL ||pNewSmsStorage==NULL) 
	return ERR_CFW_INVALID_PARAMETER;
	UINT32 ret = ERR_SUCCESS;
	BOOL bDef = FALSE;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetNewSmsOption);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetNewSmsOption, nSimID=0x%x\n", nSimID);

	 INT8 simid_c[2]  = {0x30,0x00};
	       INT8 name_s[REG_VALUENAME_LEN_MAX+1] = {0x00,};
		INT8 name_s1[13] = {0x00,};

		strcpy(name_s,"SMS_SIM");
		strcpy(name_s1,"_OPTION");
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			strcat(name_s, name_s1);
		        ret = cfg_read_8("sms", name_s, pOption);
			
		}else{
		      return ERR_CFW_INVALID_PARAMETER;
		}
	
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret=0x%x\n", ret);
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
		        ret = cfg_readdefault("sms", name_s, pOption);
			
		}else{
		       ret = ERR_CFW_INVALID_PARAMETER;
		}
		
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetNewSmsOption,1, call cfg_readdefault error ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetNewSmsOption);
			return ret;
		}
		bDef = TRUE;
	}
	if(bDef)
	{
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			ret = cfg_write_8("sms", name_s, *pOption);
		}
		else
		{
			return ERR_CFW_INVALID_PARAMETER;
		}
		bDef = FALSE;
	}
	memset(name_s,0x00,sizeof(name_s));
			memset(name_s1,0x00,sizeof(name_s1));
			strcpy(name_s,"SMS_SIM");
		      strcpy(name_s1,"_OPTSTORAGE");
			strcat(name_s, simid_c);
			strcat(name_s, name_s1);

	if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
	{
		ret = cfg_read_8("sms", name_s, pNewSmsStorage);
	}
	else
	{
		return ERR_CFW_INVALID_PARAMETER;
	}
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret=0x%x\n", ret);
	      if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			ret = cfg_readdefault("sms", name_s, pNewSmsStorage);
		}
		else
		{
			return ERR_CFW_INVALID_PARAMETER;
		}

		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetNewSmsOption,3, call cfg_readdefault ret=0x%x\n", ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetNewSmsOption);
			return ret;
		}
		bDef = TRUE;
	}
	if(bDef)
	{
	      if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			cfg_write_8("sms", name_s, *pNewSmsStorage);
		}
		else
		{
			return ERR_CFW_INVALID_PARAMETER;
		}
		bDef = FALSE;
	}
	CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetNewSmsOption,5, *pNewSmsStorage=%d, *pOption=%d\n", *pNewSmsStorage,*pOption);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetNewSmsOption);
	return ERR_SUCCESS;
}


PRIVATE CFW_SMS_STORAGE_INFO g_storageInfo[CFW_SIM_COUNT*2]={{0,},};

UINT32 CFW_CfgSetSmsStorageInfo (
  CFW_SMS_STORAGE_INFO* pStorageInfo,
  UINT16 nStorage,
  CFW_SIM_ID nSimID
)
{
	//UINT32 ret= ERR_SUCCESS;
	CFW_SMS_STORAGE_INFO* p;
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsStorageInfo  nStorage = 0x%x, nSimID =0x%x\n", nStorage, nSimID);

	if(pStorageInfo == NULL)
		return ERR_CFW_INVALID_PARAMETER;

	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSmsStorageInfo);
	if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
	{
		if(nStorage == CFW_SMS_STORAGE_ME)
		{
		      p = &g_storageInfo[nSimID];
		}
		else if(nStorage == CFW_SMS_STORAGE_SM)
		{
			p = &g_storageInfo[CFW_SIM_COUNT+nSimID];
		}
		else
		{
			p = g_storageInfo;
		}
	}else
	{
		return ERR_CFW_INVALID_PARAMETER;

	}
	SUL_MemCopy8(p, pStorageInfo, sizeof(CFW_SMS_STORAGE_INFO));
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsStorageInfo);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsStorageInfo, exit \n");
	return ERR_SUCCESS;
}

UINT32 CFW_CfgGetSmsStorageInfo (
  CFW_SMS_STORAGE_INFO* pStorageInfo,
  UINT16 nStorage,
  CFW_SIM_ID nSimID
)
{
	UINT32 ret= ERR_SUCCESS;
	CFW_SMS_STORAGE_INFO* p;

	if(pStorageInfo == NULL)
	return ERR_CFW_INVALID_PARAMETER;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetSmsStorageInfo);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSmsStorageInfo  nStorage = 0x%x, nSimID =0x%x\n", nStorage, nSimID);
	if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
	{
		if(nStorage == CFW_SMS_STORAGE_ME)
		{
		      p = &g_storageInfo[nSimID];
		}
		else if(nStorage == CFW_SMS_STORAGE_SM)
		{
			p = &g_storageInfo[CFW_SIM_COUNT+nSimID];
		}
		else
		{
			p = g_storageInfo;
		}
	}else
	{
		return ERR_CFW_INVALID_PARAMETER;

	}
	ret= (UINT32)SUL_MemCopy8(pStorageInfo, p, sizeof(CFW_SMS_STORAGE_INFO));

	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSmsStorageInfo);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSmsStorageInfo, exit \n");
	return ERR_SUCCESS;
}


#else

PRIVATE CFW_SMS_STORAGE_INFO g_storageInfo[2]={{0,},};

UINT32 CFW_CfgSetSmsStorageInfo (
  CFW_SMS_STORAGE_INFO* pStorageInfo,
  UINT16 nStorage
)
{
    //UINT8 name[17];
    UINT32 ret= ERR_SUCCESS;
    CFW_SMS_STORAGE_INFO* p;
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsStorageInfo  nStorage = 0x%x\n", nStorage);

    if(pStorageInfo == NULL)
	    return ERR_CFW_INVALID_PARAMETER;
	    
    CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSmsStorageInfo);
    if(nStorage == CFW_SMS_STORAGE_ME)
    {
        p = g_storageInfo;
	    //SUL_StrCopy(name, "SMS_STORAGEINFO1");
    }
    else if(nStorage == CFW_SMS_STORAGE_SM)
    {
        p = &g_storageInfo[1];
	    //SUL_StrCopy(name, "SMS_STORAGEINFO2");
    }
    else if(nStorage == CFW_SMS_STORAGE_BM)
    {
        p = &g_storageInfo[2];
	    //SUL_StrCopy(name, "SMS_STORAGEINFO3");
    }
    else if(nStorage == CFW_SMS_STORAGE_SM_SR ||nStorage == CFW_SMS_STORAGE_ME_SR )
    {
        p = &g_storageInfo[3];
	    //SUL_StrCopy(name, "SMS_STORAGEINFO4");
    }
    else
    {
        p = g_storageInfo;
	    //SUL_StrCopy(name, "SMS_STORAGEINFO1");
    }

    SUL_MemCopy8(p, pStorageInfo, sizeof(CFW_SMS_STORAGE_INFO));
	//ret = cfg_write_hex("sms", name, (PVOID) pStorageInfo, sizeof(CFW_SMS_STORAGE_INFO));
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsStorageInfo);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsStorageInfo, exit ret = 0x%x \n", ret);
    return ret;
}

UINT32 CFW_CfgGetSmsStorageInfo (
  CFW_SMS_STORAGE_INFO* pStorageInfo,
  UINT16 nStorage
)
{
    //UINT8 name[17];
    UINT32 ret= ERR_SUCCESS;
    CFW_SMS_STORAGE_INFO* p;
    
    if(pStorageInfo == NULL)
	    return ERR_CFW_INVALID_PARAMETER;
    CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetSmsStorageInfo);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSmsStorageInfo  nStorage = 0x%x\n", nStorage);
	if(nStorage == CFW_SMS_STORAGE_ME)
    {
        p = g_storageInfo;
    }
    else if(nStorage == CFW_SMS_STORAGE_SM)
    {
        p = &g_storageInfo[1];
    }
    else if(nStorage == CFW_SMS_STORAGE_BM)
    {
        p = &g_storageInfo[2];
    }
    else if(nStorage == CFW_SMS_STORAGE_SM_SR ||nStorage == CFW_SMS_STORAGE_ME_SR )
    {
        p = &g_storageInfo[3];
    }
    else
    {
        p = g_storageInfo;
    }

	ret= (UINT32)SUL_MemCopy8(pStorageInfo, p, sizeof(CFW_SMS_STORAGE_INFO));

	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSmsStorageInfo);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSmsStorageInfo, exit ret = 0x%x \n", ret);
    return ERR_SUCCESS;
}

UINT32 CFW_CfgSetSmsStorage (UINT8 nStorage)
{
	UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSmsStorage);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsStorage  nStorage = 0x%x\n", nStorage);
	ret = CFW_CfgNumRangeCheck(nStorage, SMS_STORAGE_RANGE);

	if(ERR_SUCCESS == ret)
	{   
		ret = cfg_write_8("sms", "SMS_STORAGE", nStorage);
	}
	else
	{ 
		CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret =0x%x\n", ret);
		ret = ERR_CFG_PARAM_OUT_RANGR;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsStorage);    
	return ret;    
}


UINT32 CFW_CfgGetSmsStorage ( UINT8* pStorage)
{
    
    UINT32 ret = ERR_SUCCESS;
    BOOL bDef = FALSE;
    CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSmsStorage);
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSmsStorage  enter\n");
    if(pStorage != 0)
    {
        ret = cfg_read_8("sms", "SMS_STORAGE", pStorage);
        if(ERR_SUCCESS != ret)
        {
	        CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsStorage, call cfg_read_8 false, ret=0x%x\n",ret);
	        ret = cfg_readdefault("sms", "SMS_STORAGE", pStorage);
	        if(ERR_SUCCESS != ret)
	        {
		        CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsStorage, call cfg_readdefault false, ret=0x%x\n",ret);
		        CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsStorage);
		        return ret;
	        }
	        bDef = TRUE;
        }
        else
        {
	        ret = CFW_CfgNumRangeCheck(*pStorage, SMS_STORAGE_RANGE);
	        if(ERR_SUCCESS != ret)
			{
		        CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsStorage,2, call CFW_CfgNumRangeCheck false, ret=0x%x\n",ret);
		        ret = cfg_readdefault("sms", "SMS_STORAGE", pStorage);
		        if(ERR_SUCCESS != ret)
		        {
			        CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsStorage,2, call cfg_readdefault false, ret=0x%x\n",ret);
					CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsStorage);
			        return ret;
		        }
		        bDef = TRUE;
	        }
        }
    }
	
    if(bDef)
    {
	    cfg_write_8("sms", "SMS_STORAGE", *pStorage);
    }
    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsStorage);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSmsStorage  *pStorage = 0x%x\n", *pStorage);
    return ret;	
}

/*===================================================================

===================================================================*/

UINT32 CFW_CfgSetSmsOverflowInd ( UINT8 nMode )
{
    UINT32 ret = ERR_SUCCESS;
    CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSmsOverflowInd);
	
    ret = CFW_CfgNumRangeCheck(nMode, SMS_OVERFLOW_RANGE);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsOverflowInd  nMode = 0x%x, ret =0x%x\n", nMode, ret);
    if(ERR_SUCCESS == ret)
    {
        //WRITE_NUMBER_SETTING_8("SMS_OVER_IND", nMode, "sms" );
        ret = cfg_write_8("sms", "SMS_OVER_IND", nMode);
    }
    else
        ret = ERR_CFG_PARAM_OUT_RANGR;
    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsOverflowInd);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsOverflowInd  ret =0x%x\n", ret);
    return ret;
}

UINT32 CFW_CfgGetSmsOverflowInd ( UINT8* pMode )
{
    UINT32 ret = ERR_SUCCESS;
    BOOL bDef = FALSE;
    CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetSmsOverflowInd);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsOverflowInd enter\n");
    ret = cfg_read_8("sms", "SMS_OVER_IND", pMode);
    if(ret == ERR_SUCCESS)
    {
	    ret = CFW_CfgNumRangeCheck(*pMode, SMS_OVERFLOW_RANGE);
	    if(ERR_SUCCESS != ret)
	    {
		    ret = cfg_readdefault("sms", "SMS_OVER_IND", pMode);
		    if(ERR_SUCCESS != ret)
		    {
			    CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsOverflowInd,1,call cfg_readdefault falsed, ret=0x%x\n",ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSmsOverflowInd);
				return ret;
		    }
		    bDef = TRUE;
	    }
    }
    else
	{
	    ret = cfg_readdefault("sms", "SMS_OVER_IND", pMode);
	    if(ERR_SUCCESS != ret)
	    {
		    CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsOverflowInd,1,call cfg_readdefault falsed, ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSmsOverflowInd);
			return ret;
	    }
	    bDef = TRUE;
	}
	if(bDef)
		cfg_write_8("sms", "SMS_OVER_IND", *pMode);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSmsOverflowInd);	
    return ERR_SUCCESS;
}


/*===========================================================================
Set the SMS format.

  nFormat :  0 ~ 1 b2 
  
    KEY: "PUBLIC"
===========================================================================*/
UINT32 CFW_CfgSetSmsFormat(UINT8 nFormat)
{
	UINT32 ret = ERR_SUCCESS;
	//    UINT32 nPubVal= 0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSmsFormat);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsOverflowInd, nFormat = 0x%x\n", nFormat);

	if(nFormat == 0 || nFormat == 1)
	{
	g_SmsFormat = nFormat;
	}
	else
	{
		ret = ERR_CFG_PARAM_OUT_RANGR;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsFormat);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsFormat exit\n");
	return ret;
}

/*===========================================================================
Get the SMS format.

  nFormat :  0 ~ 1 b2 
  
    KEY: "PUBLIC"
===========================================================================*/
UINT32 CFW_CfgGetSmsFormat(UINT8 *nFormat)
{
	*nFormat = g_SmsFormat;
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSmsFormat, *nFormat = 0x%x\n", *nFormat);
	return ERR_SUCCESS;
    
}

/*===================================================================

===================================================================*/
UINT32 CFW_CfgSetSmsShowTextModeParam ( UINT8 nShow )
{
	UINT32 ret = ERR_SUCCESS;
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsShowTextModeParam, nShow = 0x%x\n", nShow);
	if((nShow == 0) || (nShow == 1))
	{       
		g_SmsShowTextMode = nShow;
	}
	else
		ret = ERR_CFG_PARAM_OUT_RANGR;

	return ret;
    
}

UINT32 CFW_CfgGetSmsShowTextModeParam ( UINT8* pShow )
{
    
	* pShow = g_SmsShowTextMode;
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSmsShowTextModeParam, *pShow = 0x%x\n", *pShow);
	return ERR_SUCCESS;
}


/*============================================================================

  
============================================================================*/
UINT32 CFW_CfgSetSmsFullForNew ( UINT8 n)
{
    
	UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSmsFullForNew);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsFullForNew, n = 0x%x\n", n);
	ret = CFW_CfgNumRangeCheck(n, SMS_FULLFORNEW_RANGE);
	if(ERR_SUCCESS == ret)
	{
		ret = cfg_write_8("sms", "FULL_FOR_NEW", n);
	}
	else
	{
		CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsFullForNew, error ret =0x%x\n", ret);
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsFullForNew);
	return ret;
}


UINT32 CFW_CfgGetSmsFullForNew ( UINT8 *pn)
{
	UINT32 ret = ERR_SUCCESS;
	BOOL bDef = FALSE;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetSmsFullForNew);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsFullForNew enter\n");
	ret = cfg_read_8("sms", "FULL_FOR_NEW", pn);
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsFullForNew,1, call cfg_read_8 falsed, ret =0x%x\n",ret);
		ret = cfg_readdefault("sms", "FULL_FOR_NEW", pn);
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsFullForNew,1, call cfg_readdefault falsed, ret =0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSmsFullForNew);
			return ret;
		}
		bDef = TRUE;
	}
	else
	{
		ret = CFW_CfgNumRangeCheck(*pn, SMS_FULLFORNEW_RANGE);
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsFullForNew,2, call CFW_CfgNumRangeCheck falsed, ret =0x%x\n",ret);
			ret = cfg_readdefault("sms", "FULL_FOR_NEW", pn);
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsFullForNew,2, call cfg_readdefault falsed, ret =0x%x\n",ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSmsFullForNew);
				return ret;
			}
		}
		bDef = TRUE;
	}
	if(bDef)
		cfg_write_8("sms", "FULL_FOR_NEW", *pn);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSmsFullForNew);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSmsFullForNew, *pn =0x%x\n", *pn);
    return ERR_SUCCESS;
}


PUBLIC UINT32 SRVAPI CFW_CfgSetSmsParam (
  CFW_SMS_PARAMETER* pInfo,
  UINT16 nUTI
)
{
	UINT32 ret = ERR_SUCCESS;
	UINT8 nIndex;
	if(pInfo == NULL)
	    return ERR_CFG_PARAM_OUT_RANGR;
       if(pInfo->nIndex>4)
	    return ERR_CFG_PARAM_OUT_RANGR;
	
	    
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSmsParam);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsParam enter\n");
	nIndex = pInfo->nIndex;
	if (0 == pInfo->nSaveToSimFlag)//added by fengwei 20080707 for save sca to sim
	{
		if(pInfo->bDefault)
		{
			ret = cfg_write_8("sms", "DEF_PARAM_IND", nIndex);
			if(ERR_SUCCESS != ret)
			{
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsParam);
				return ret;
			}
		}
		if(pInfo)
		{
			UINT8 name[REG_VALUENAME_LEN_MAX+1];
			SUL_StrPrint(name, "SMS_PARAM%d",nIndex+1);
			ret = cfg_write_hex("sms", name, pInfo,sizeof(CFW_SMS_PARAMETER));
		}
	}
	//added by fengwei 20080707 for save sca to sim begin
	else 
	{
		CFW_SMS_PARAMETER *pParamInfo = NULL;
		CFW_SMS_PARAMETER *pStoredParam = NULL;
		HAO hAo = 0;

		//Verify input parameters
		ret = CFW_IsFreeUTI(nUTI,CFW_APP_SRV_ID);

		if(ret != ERR_SUCCESS)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_IsFreeUTI, error ret =0x%x\n", ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsParam);
			return ret;
		}

		pParamInfo = (CFW_SMS_PARAMETER*)CFW_MALLOC(SIZEOF(CFW_SMS_PARAMETER) * 2);
		if (NULL == pParamInfo)
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsParam);
			return ERR_CMS_MEMORY_FULL;
		}
		SUL_ZeroMemory8(pParamInfo, SIZEOF(CFW_SMS_PARAMETER) * 2);
		SUL_MemCopy8(pParamInfo, pInfo, SIZEOF(CFW_SMS_PARAMETER)); 

		//store the param stored in ME in void of setting param to sim failure, then restore param in ME. 
		pStoredParam = pParamInfo + SIZEOF(CFW_SMS_PARAMETER);
		ret = CFW_CfgGetSmsParam(pStoredParam, pInfo->nIndex);

		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSmsParam, error ret =0x%x\n", ret);
			CFW_FREE(pParamInfo);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsParam);
			return ret;
		}

		//first of all we updata param in ME. 
		if(pInfo->bDefault)
		{
			ret = cfg_write_8("sms", "DEF_PARAM_IND", nIndex);
			if(ERR_SUCCESS != ret)
			{
				CFW_FREE(pParamInfo);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsParam);
				return ret;
			}
		}
		if(pInfo)
		{
			UINT8 name[REG_VALUENAME_LEN_MAX+1];
			SUL_StrPrint(name, "SMS_PARAM%d",nIndex+1);
			ret = cfg_write_hex("sms", name, pInfo,sizeof(CFW_SMS_PARAMETER));
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_hex, error ret =0x%x\n", ret);
				CFW_FREE(pParamInfo);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsParam);
				return ret;
			}
		}

		//then set param to sim
		hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pParamInfo, sms_SetParamAoProc);
		CFW_SetUTI(hAo,nUTI,0);
		CFW_SetAoProcCode(hAo,CFW_AO_PROC_CODE_CONTINUE);
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsParam);
		return ERR_SUCCESS;
	}
	//added by fengwei 20080707 for save sca to sim end

	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsParam);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsParam, exit ret =0x%x\n", ret);
	return ret;
}

UINT32 CFW_CfgGetSmsParam (
  CFW_SMS_PARAMETER* pInfo,
  UINT8 nIndex
)
{
	UINT32 ret = ERR_SUCCESS;
	UINT8 name[14];

	if((nIndex>4)||(pInfo==NULL))
	return ERR_CFG_PARAM_OUT_RANGR;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetSmsParam);
	SUL_StrPrint(name, "SMS_PARAM%d",nIndex+1);
	CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetSmsParam,1,struct size=%d\n",sizeof(CFW_SMS_PARAMETER));
	ret = cfg_read_hex("sms", name,  pInfo, (UINT8)sizeof(CFW_SMS_PARAMETER));
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret =0x%x\n", ret);
		SUL_ZeroMemory8(pInfo, sizeof(CFW_SMS_PARAMETER));
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSmsParam);
	return ERR_SUCCESS;
}

UINT32 CFW_CfgGetDefaultSmsParam (
  CFW_SMS_PARAMETER* pInfo
)
{
	UINT32 ret = ERR_SUCCESS;
	UINT8 name[11];
	UINT8 index =0;
	if(pInfo==NULL)
	return ERR_CFG_PARAM_OUT_RANGR;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetDefaultSmsParam);
	ret = cfg_read_8("sms", "DEF_PARAM_IND", &index);
	if(ERR_SUCCESS !=ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetDefaultSmsParam,1, call cfg_read_8 falsed, ret =0x%x\n",ret);
		ret = cfg_readdefault("sms", "DEF_PARAM_IND", &index);
		if(ERR_SUCCESS !=ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetDefaultSmsParam,1, call cfg_readdefault falsed, ret =0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetDefaultSmsParam);
			return ret;
		}
		cfg_write_8("sms", "DEF_PARAM_IND", index);
	}

	SUL_StrPrint(name, "SMS_PARAM%d",index+1);

	ret = cfg_read_hex("sms", name, (PVOID) pInfo, (UINT8)sizeof(CFW_SMS_PARAMETER));
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetDefaultSmsParam,1, call cfg_read_hex falsed, ret =0x%x\n",ret);
		SUL_ZeroMemory8(pInfo, sizeof(CFW_SMS_PARAMETER));
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetDefaultSmsParam);
	return ERR_SUCCESS;
}

UINT32 CFW_CfgSetDefaultSmsParam (
  CFW_SMS_PARAMETER* pInfo
)
{
	UINT32 ret = ERR_SUCCESS;
	UINT8 name[11];
	UINT8 index =0;
	if(pInfo==NULL)
	return ERR_CFG_PARAM_OUT_RANGR;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetDefaultSmsParam);
	ret = cfg_read_8("sms", "DEF_PARAM_IND", &index);
	if(ERR_SUCCESS !=ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgSetDefaultSmsParam,1, call cfg_read_8 falsed, ret =0x%x\n",ret);
		ret = cfg_readdefault("sms", "DEF_PARAM_IND", &index);
		if(ERR_SUCCESS !=ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgSetDefaultSmsParam,1, call cfg_readdefault falsed, ret =0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetDefaultSmsParam);
			return ret;
		}
		cfg_write_8("sms", "DEF_PARAM_IND", index);
	}

	SUL_StrPrint(name, "SMS_PARAM%d",index+1);
	ret = cfg_write_hex("sms", name, (PVOID )pInfo,sizeof(CFW_SMS_PARAMETER));
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetDefaultSmsParam);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetDefaultSmsParam exit ret =0x%x\n",ret);
	return ret;
}

//PRIVATE UINT8 g_nOption = CFW_SMS_MULTIPART_OPTION_ALL;
//PRIVATE UINT8 g_nNewSmsStorage = CFW_CFG_RECEIVE_STORAGE(CFW_SMS_STORAGE_SM_ME,CFW_SMS_STORAGE_BM,CFW_SMS_STORAGE_ME_SR);

UINT32 CFW_CfgSetNewSmsOption(
  UINT8 nOption, 
  UINT8 nNewSmsStorage
)
{
	UINT32 ret = ERR_SUCCESS,ret1 = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetNewSmsOption);
	
	ret = CFW_CfgNumRangeCheck(nOption, SMS_OPTION_RANGE);
	ret1 = CFW_CfgNumRangeCheck(nNewSmsStorage, SMS_OPT_STORAGE_RABGE);
	if(ERR_SUCCESS == ret)
	{
		ret = cfg_write_8("sms", "SMS_OPTION", nOption);
	}

	if(ERR_SUCCESS == ret1)
	{
		ret1 = cfg_write_8("sms", "SMS_OPT_STORAGE", nNewSmsStorage);
	}
	CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgSetNewSmsOption,1, input option=%d, NewSmsStorage=%d, ret1=0x%x,ret=0x%x\n",nOption,nNewSmsStorage, ret1,ret);

	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetNewSmsOption);
	if((ret+ret1) == ERR_SUCCESS)
	{
		return ERR_SUCCESS;
	}
	else
	{
		return (ret?ret:ret1);
	}
}

UINT32 CFW_CfgGetNewSmsOption(  UINT8* pOption,   UINT8* pNewSmsStorage)
{
	if(pOption==NULL ||pNewSmsStorage==NULL) 
	return ERR_CFW_INVALID_PARAMETER;
	UINT32 ret = ERR_SUCCESS;
	BOOL bDef = FALSE;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetNewSmsOption);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetNewSmsOption enter\n");
	ret = cfg_read_8("sms", "SMS_OPTION", pOption);
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetNewSmsOption,1, call cfg_read_8 error ret=0x%x\n",ret);
		ret = cfg_readdefault("sms", "SMS_OPTION", pOption);
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetNewSmsOption,1, call cfg_readdefault error ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetNewSmsOption);
			return ret;
		}
		bDef = TRUE;
	}
	if(bDef)
	{
		cfg_write_8("sms", "SMS_OPTION", *pOption);
		bDef = FALSE;
	}

	// here begin to work with pNewSmsStorage
	ret = cfg_read_8("sms", "SMS_OPT_STORAGE", pNewSmsStorage);
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetNewSmsOption,3, call cfg_read_8, error ret=0x%x\n",ret);
		if(!cfg_readdefault("sms", "SMS_OPT_STORAGE", pNewSmsStorage))
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetNewSmsOption,3, call cfg_readdefault error ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetNewSmsOption);
			return ret;
		}
		bDef = TRUE;
	}
	if(bDef)
	{
		cfg_write_8("sms", "SMS_OPT_STORAGE", *pNewSmsStorage);
		bDef = FALSE;
	}
	CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetNewSmsOption,5, *pNewSmsStorage=%d, *pOption=%d\n", *pNewSmsStorage,*pOption);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetNewSmsOption);
	return ERR_SUCCESS;
}

#endif

#ifdef CFW_MULTI_SIM
extern UINT32 CFW_GetSimID(HAO hAO, CFW_SIM_ID *pSimID);
#endif
//added by fengwei 20080707 for save sca to sim begin.
static UINT32 sms_SetParamAoProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32  nState=0x0, nGetUti=0x0; // [-] dingmx20080908 delete unused ret
	CFW_SMS_PARAMETER* pSmsParamInfo = NULL; //Point to private data.
	CFW_SIM_SMS_PARAMETERS pSimSmsParam;
	CSW_PROFILE_FUNCTION_ENTER(sms_SetParamAoProc);
#ifdef CFW_MULTI_SIM	
	CFW_SIM_ID nSimID = CFW_SIM_COUNT;
	CFW_GetSimID(hAo, &nSimID);
#endif
	CSW_TRACE(CFW_SMS_TS_ID, "sms_SetParamAoProc() enter\n");
	//Get private date
	pSmsParamInfo = CFW_GetAoUserData(hAo);
	//Verify memory
	if(!pSmsParamInfo)
	{
		CFW_GetUTI(hAo, &nGetUti);
#ifdef CFW_MULTI_SIM	
		CFW_PostNotifyEvent(EV_CFW_SMS_SET_PARAM_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti|(nSimID<<24),0xf0);
#else
		CFW_PostNotifyEvent(EV_CFW_SMS_SET_PARAM_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti,0xf0);
#endif
		CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
		CSW_PROFILE_FUNCTION_EXIT(sms_SetParamAoProc);
		return ERR_NO_MORE_MEMORY;
	}

	//Get current State
	if(pEvent == (CFW_EV*)0xffffffff)
		nState = CFW_SM_STATE_IDLE;
	else 
		nState = CFW_GetAoState(hAo);

	//SMS MO State machine process
	CSW_TRACE(CFW_CFG_TS_ID, "sms_SetParamAoProc, nState =0x%x\n",nState);
	switch(nState)
	{
		case CFW_SM_STATE_IDLE:
		{
			UINT32 uRet;
			SUL_ZeroMemory8(&pSimSmsParam, SIZEOF(CFW_SIM_SMS_PARAMETERS));
			pSimSmsParam.nDCS = pSmsParamInfo->dcs;
			pSimSmsParam.nPID = pSmsParamInfo->pid;
			pSimSmsParam.nVP = pSmsParamInfo->vp;
			pSimSmsParam.nFlag = pSmsParamInfo->nSaveToSimFlag;
			SUL_MemCopy8(pSimSmsParam.nSMSCInfo, pSmsParamInfo->nNumber, 12);
			
			CFW_GetUTI(hAo, &nGetUti);
			//we set one set of CFW_SIM_SMS_PARAMETERS each time.
			#ifdef CFW_MULTI_SIM
			uRet = CFW_SimSetSmsParameters(&pSimSmsParam, 1, pSmsParamInfo->nIndex, (UINT16)(CFW_APP_UTI_MIN + nGetUti),nSimID);
			#else
			uRet = CFW_SimSetSmsParameters(&pSimSmsParam, 1, pSmsParamInfo->nIndex, (UINT16)(CFW_APP_UTI_MIN + nGetUti));
			#endif
			if (ERR_SUCCESS != uRet)
			{
				//set sms fail, we think it's memory malloc fail.
			#ifdef CFW_MULTI_SIM				
				CFW_PostNotifyEvent(EV_CFW_SMS_SET_PARAM_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti|(nSimID<<24),0xf0);
			#else
				CFW_PostNotifyEvent(EV_CFW_SMS_SET_PARAM_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti ,0xf0);
			#endif
				CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
				CSW_PROFILE_FUNCTION_EXIT(sms_SetParamAoProc);
				return ERR_NO_MORE_MEMORY;
			}
			else
			{
				CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
			}
		}
		break;
		case CFW_SM_STATE_WAIT:
		{
			CSW_TRACE(CFW_CFG_TS_ID, "sms_SetParamAoProc, pEvent->nEventId =0x%x, pEvent->nType  =0x%x\n",pEvent->nEventId, pEvent->nType );
			if(pEvent->nEventId == EV_CFW_SIM_SET_SMS_PARAMETERS_RSP)
			{
				
				if (pEvent->nType == 0)
				{
				       CFW_GetUTI(hAo, &nGetUti);
			#ifdef CFW_MULTI_SIM				
					CFW_PostNotifyEvent(EV_CFW_SMS_SET_PARAM_RSP, 0, 0, nGetUti|(nSimID<<24), 0);
			#else
					CFW_PostNotifyEvent(EV_CFW_SMS_SET_PARAM_RSP, 0, 0, nGetUti , 0);
			#endif
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
					CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
					return ERR_SUCCESS;
				}
				else//set fails 
				{
					UINT32 uRet = 0;
					//restore param in ME.
					CFW_SMS_PARAMETER* pStoredParam =(CFW_SMS_PARAMETER*)( (UINT32)pSmsParamInfo + SIZEOF(CFW_SMS_PARAMETER));  // [mod] dingmx20080908 add type cast with (CFW_SMS_PARAMETER*)

					//set default to the value fo setting param.
					pStoredParam->bDefault = pSmsParamInfo->bDefault;
					//restore param in ME, so don't need to restore param in SIM.
					pStoredParam->nSaveToSimFlag = 0;
					CFW_GetUTI(hAo, &nGetUti);
					#ifdef CFW_MULTI_SIM
					uRet = CFW_CfgSetSmsParam(pStoredParam, nGetUti, nSimID);
					#else
					uRet = CFW_CfgSetSmsParam(pStoredParam, nGetUti);
					#endif

					if (ERR_SUCCESS != uRet)
					{
						CSW_TRACE(CFW_SMS_TS_ID, "Set Param to Sim fails, and restore param in ME fails, WRONING!!!\n");
					}
			#ifdef CFW_MULTI_SIM				
					CFW_PostNotifyEvent(EV_CFW_SMS_SET_PARAM_RSP, pEvent->nParam1, 0, nGetUti|(nSimID<<24), 0xf0);
			#else
					CFW_PostNotifyEvent(EV_CFW_SMS_SET_PARAM_RSP, pEvent->nParam1, 0, nGetUti , 0xf0);
			#endif
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
					CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
					return ERR_CMS_UNKNOWN_ERROR;
				}
			}
		}
		break;
	}
	CSW_PROFILE_FUNCTION_EXIT(sms_SetParamAoProc);
	CSW_TRACE(CFW_SMS_TS_ID, "sms_SetParamAoProc() exit\n");
	return ERR_SUCCESS;
}
PRIVATE UINT8 g_mids[6]={0,};
PRIVATE UINT8 g_dcss[6]={0,};
PRIVATE UINT8 g_mode = 0;
/*===================================================================

===================================================================*/
UINT32 CFW_CfgSetSmsCB ( UINT8 mode, UINT8 mids[6], UINT8 dcss[6])
{
    UINT32 ret = ERR_SUCCESS;
    UINT8 i = 0;
    if(mode > 1)
	    return ERR_CFG_PARAM_OUT_RANGR;
	    
    for(i = 0; i< SIZEOF(g_mids); i++) 
        g_mids[i] = mids[i];
    for(i = 0; i< SIZEOF(g_dcss); i++) 
        g_dcss[i] = dcss[i]; 
    g_mode = mode;

    return ret;
}


UINT32 CFW_CfgGetSmsCB (
  UINT8* pMode,
  UINT8 mids[6],
  UINT8 dcss[6]
)
{
    UINT8 i = 0;
    for(i = 0; i< SIZEOF(g_mids); i++) 
        mids[i] = g_mids[i];
    for(i = 0; i< SIZEOF(g_dcss); i++) 
        dcss[i] = g_dcss[i];
    if(pMode) 
        *pMode = g_mode;

    return ERR_SUCCESS;
}


/*======================================================================================
NAME    FUNCTION                PARAMETER   RANGE   DEFAULT
AT+CSMS	CFW_CfgSelectSmsService	nService	0-1	    0
pMTType	    0-1	    1
pMOType     0-1	    1
pBMType	    0-1	    1
=======================================================================================*/
UINT32 CFW_CfgSelectSmsService( UINT8 nService, UINT8* pSupportType)
{
#if 0
    UINT32 ret = ERR_SUCCESS;

    /*Check the range*/
    ret = CFW_CfgNumRangeCheck(nService, SMS_SERV_SELECT_RANGE);
    if(ret != ERR_SUCCESS)
        return ret;
    
    MemoryInfo.SelectSmsService = nService;
    
    if(nService)
    /*GSM 03.40 and 03.41 (the syntax of SMS AT commands is compatible with GSM 07.05 
    Phase 2+ version; the requirement of <service> setting 1 is mentioned under corresponding
    command descriptions).
    */
    {
        CFW_SET_BIT(*pSupportType, 0); //*pMTType = 1;
        CFW_SET_BIT(*pSupportType, 1); //*pMOType = 1;
        CFW_SET_BIT(*pSupportType, 2); //*pBMType = 1;
    }
    /*GSM 03.40 and 03.41 (the syntax of SMS AT commands is compatible with GSM 07.05 
    Phase 2 version 4.7.0; Phase 2+ features which do not require new command syntax 
    may be supported, e.g. correct routing of messages with new Phase 2+ data coding schemes)
    */
    else
    {
        CFW_SET_BIT(*pSupportType, 0); //*pMTType = 1;
        CFW_SET_BIT(*pSupportType, 1); //*pMOType = 1;
        CFW_SET_BIT(*pSupportType, 2); //*pBMType = 1;
    }
    
    return ret;
#endif
	return ERR_CFW_NOT_SUPPORT;
}

/*==========================================================================
The CFW_CfgQuerySmsService function select the current SMS service.

==========================================================================*/
UINT32 CFW_CfgQuerySmsService( UINT8* pService )
{
#if 0
    *pService = 0;
    
    CFW_SET_BIT(*pService, 0); 
    CFW_SET_BIT(*pService, 1); 
    CFW_SET_BIT(*pService, 2); 
    CFW_SET_BIT(*pService, 3); 
    
    return ERR_SUCCESS;
#endif
	return ERR_CFW_NOT_SUPPORT;
}
//added by fengwei 20080707 for save sca to sim end
/*===================================================================
The CFW_CfgSetNewSmsAck function set New SMS message acknowledge to 
ME/TE, only phase 2+.
===================================================================*/
UINT32 CFW_CfgSetNewSmsAck ( BOOL bAck )
{
#if 0
    UINT32 ret = ERR_SUCCESS;
    
    #ifndef MF32_BASE_REG_DISABLE    
    WRITE_NUMBER_SETTING_8("SMS_ACK", bAck, "sms" );  
    #endif 
    
    return ret;
#endif
	return ERR_CFW_NOT_SUPPORT;
}

UINT32 CFW_CfgGetNewSmsAck ( UINT8* pAck)
{
#if 0
    UINT32 ret = ERR_SUCCESS;
    READ_NUMBER_SETTING_8("SMS_ACK", *pAck, "sms" );    
    return ret;
#endif
	return ERR_CFW_NOT_SUPPORT;
}

#ifdef CFW_CUSTOM_IMEI_COUNT
UINT32 CFW_CfgSetCustomSendIMEIFlag (UINT8 nFlag, 
                             CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSmsStorage);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetCustomSendIMEIFlag nStorage[0x%x] nSimID[0x%x]\n", nStorage, nSimID);
	ret = CFW_CfgNumRangeCheck(nFlag, SMS_IMEI_RANGE);
	if(ERR_SUCCESS == ret)
	{
		INT8 simid_c[2]  = {0x30,0x00};
		INT8 name_s[14] = {0x00,};
		strcpy(name_s,"SMS_IMEI_");
		
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			ret = cfg_write_8("sms", name_s, nFlag);

		}else{
			ret = ERR_CFW_INVALID_PARAMETER;
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
		}
	}else
	{
			CSW_TRACE(CFW_CFG_TS_ID, "Error CFW_CfgSetCustomSendIMEIFlag  ret[0x%x]\n", ret);
			ret = ERR_CFG_PARAM_OUT_RANGR;
	}

	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsStorage);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSmsStorage Exit\n");
	return ret;    
}

UINT32 CFW_CfgGetCustomSendIMEIFlag ( UINT8* pFlag, 
                              CFW_SIM_ID nSimID)
{
    
	UINT32 ret = ERR_SUCCESS;
	BOOL bDef = FALSE;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSmsStorage);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetCustomSendIMEIFlag nSimID[0x%x]\n", nSimID);

	INT8 simid_c[2]  = {0x30,0x00};
		INT8 name_s[14] = {0x00,};
		strcpy(name_s,"SMS_IMEI_");
		
		if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
		{
			simid_c[0] = simid_c[0] + nSimID;
			strcat(name_s, simid_c);
			ret = cfg_read_8("sms", name_s, pFlag);

			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetCustomSendIMEIFlag->cfg_read_8 error ret[0x%x]\n",ret);
				ret = cfg_readdefault("sms", name_s, pFlag);
				if(ERR_SUCCESS != ret)
				{
					CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetCustomSendIMEIFlag->cfg_readdefault error ret[0x%x]\n",ret);
					CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsStorage);
					return ret;
				}
				bDef = TRUE;
			}
			else
			{
				ret = CFW_CfgNumRangeCheck(*pFlag, SMS_STORAGE_RANGE);
				if(ERR_SUCCESS != ret)
				{
					CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetCustomSendIMEIFlag->CFW_CfgNumRangeCheck error ret[0x%x]\n",ret);
					ret = cfg_readdefault("sms", name_s, pFlag);
					if(ERR_SUCCESS != ret)
					{
						CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetCustomSendIMEIFlag->cfg_readdefault error ret[0x%x]\n",ret);
						CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsStorage);
						return ret;
					}
					bDef = TRUE;
				}
			}

		}else
		{
			ret = ERR_CFW_INVALID_PARAMETER;
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
		}
	
	if(bDef)
	{
		cfg_write_8("sms", name_s, *pFlag);
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSmsStorage);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetCustomSendIMEIFlag  *pStorage[0x%x]\n", *pFlag);
	return ret;	
}
#endif

/*============================================================================
Set the SMS Service Center telephone number.

  KEY: "SMS_CENTER_1" "SMS_CENTER_2" "SMS_CENTER_3"
  "SMS_CENTER_4" "SMS_CENTER_5" "SMS_CENTER_6"
============================================================================*/
UINT32 CFW_CfgSetSmsCenter (CFW_TELNUMBER* pSmsCenterNumber)
{
#if 0
    UINT32 ret = ERR_SUCCESS;
    UINT32 *pInt = 0;
    
    #ifndef MF32_BASE_REG_DISABLE
    
    pInt = (UINT32*)pSmsCenterNumber;
    
    WRITE_NUMBER_SETTING("SMS_CENTER_1", *pInt, "sms");
    pInt++;
    
    WRITE_NUMBER_SETTING("SMS_CENTER_2", *pInt, "sms");
    pInt++;
    
    WRITE_NUMBER_SETTING("SMS_CENTER_3", *pInt, "sms");
    pInt++;
    
    WRITE_NUMBER_SETTING("SMS_CENTER_4", *pInt, "sms");
    pInt++;
    
    WRITE_NUMBER_SETTING("SMS_CENTER_5", *pInt, "sms");
    pInt++;
    
    WRITE_NUMBER_SETTING("SMS_CENTER_6", *pInt, "sms");
    #endif 
    
    return ret;
#endif
	return ERR_CFW_NOT_SUPPORT;

}

/*============================================================================
Get the SMS Service Center telephone number.

  KEY: "SMS_CENTER_1" "SMS_CENTER_2" "SMS_CENTER_3"
  "SMS_CENTER_4" "SMS_CENTER_5" "SMS_CENTER_6"
============================================================================*/
UINT32 CFW_CfgGetSmsCenter (CFW_TELNUMBER* pSmsCenterNumber)
{
#if 0
UINT32 ret = ERR_SUCCESS;

    #ifndef MF32_BASE_REG_DISABLE
    
    UINT32 integer = 0;
    
    READ_NUMBER_SETTING("SMS_CENTER_1", integer, "sms");
    SUL_MemCopy32(&(pSmsCenterNumber->nTelNumber[0]), &integer, 4);
    
    READ_NUMBER_SETTING("SMS_CENTER_2", integer, "sms");
    SUL_MemCopy32(&(pSmsCenterNumber->nTelNumber[4]), &integer, 4);
    
    READ_NUMBER_SETTING("SMS_CENTER_3", integer, "sms");
    SUL_MemCopy32(&(pSmsCenterNumber->nTelNumber[8]), &integer, 4);
    
    READ_NUMBER_SETTING("SMS_CENTER_4", integer, "sms");
    SUL_MemCopy32(&(pSmsCenterNumber->nTelNumber[12]), &integer, 4);
    
    READ_NUMBER_SETTING("SMS_CENTER_5", integer, "sms");
    SUL_MemCopy32(&(pSmsCenterNumber->nTelNumber[16]), &integer, 4);
    
    READ_NUMBER_SETTING("SMS_CENTER_6", integer, "sms");
    pSmsCenterNumber->nTelNumber[20] = (UINT8)((integer & 0xff000000)>>24);
    pSmsCenterNumber->nTelNumber[21] = (UINT8)((integer & 0x00ff0000)>>16);
    pSmsCenterNumber->nTelNumber[22] = (UINT8)((integer & 0x0000ff00)>>8);
    pSmsCenterNumber->nType =  (UINT8)(integer & 0x000000ff);
    #endif 
    
    return ret;
#endif
	return ERR_CFW_NOT_SUPPORT;

}

UINT32 CFW_CfgSmsFlush (
                       VOID
)
{
    //TODO...

    return ERR_CFW_NOT_SUPPORT;
}
