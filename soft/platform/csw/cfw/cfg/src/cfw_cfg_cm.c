///*************************************************************
//hameina 2006.8.21 modified
//
//notice: this version is based on reg systerm
//
//modify record:
//2006.9.14 hameina [mod] ERR_CFW_NOTSUPPORT->ERR_CFW_NOT_SUPPORT, who define this
//    wrong err code murdered hameina.
//2006.9.29 hameina [mod] cfg_readdefault() the type of function changed:bool->UINT32
///**************************************************************

#include "cfw.h"
#include "cfw_cfg.h"
#include "csw_debug.h"
#include <string.h>
#ifdef CFW_MULTI_SIM
/*===========================================================================
Configure Supplementary service notifications.
  
bCSSI: 0 ~ 1  b0
bCSSU: 0 ~ 1  b1

KEY: "PUBLIC"
===========================================================================*/
UINT32 CFW_CfgSetSSN(UINT8 bCSSI, UINT8 bCSSU,CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	UINT32 nPubVal= 0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSSN);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSSN, bCSSI = 0x%x, bCSSU =0x%x,  nSimID =0x%x\n", bCSSI, bCSSU, nSimID);
	ret = cfg_read_32("cm", "PUBLIC", &nPubVal);
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_32, Error ret =0x%x\n", ret);
		ret = cfg_readdefault("cm", "PUBLIC", &nPubVal);
		if(ERR_SUCCESS!=ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, Error ret =0x%x\n", ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSSN);
			return ERR_CFG_READ_REG_FAILURE;
		}
	}
	if((CFW_SIM_0<=nSimID)&&(nSimID< CFW_SIM_COUNT))
	{
	    	if((( bCSSI == 0)||(bCSSI == 1))&&
	    	  (( bCSSU == 0)||(bCSSU == 1)))
	    	{
	    	    SET_BIT_VALUE(nPubVal, (nSimID*2), bCSSI);
	    	    SET_BIT_VALUE(nPubVal, (nSimID*2+1), bCSSU);
	    	    
	    	}
	    	else if((bCSSI == 0xFF) && (( bCSSU == 0)||(bCSSU == 1)))
	    	{
	    	    SET_BIT_VALUE(nPubVal, (nSimID*2+1), bCSSU);
	    		
	    	}
	    	else if((bCSSU == 0xFF) && (( bCSSI == 0)||(bCSSI == 1)))
	    	{
	    	    SET_BIT_VALUE(nPubVal, (nSimID*2), bCSSI);
	    		
	    	}
	    	else if((bCSSU == 0xFF) && (bCSSI == 0xFF))
	    	{
	    	    ret = ERR_SUCCESS;
	    	}
	    	else
	    	{
	    	    ret = ERR_CFG_PARAM_OUT_RANGR;
	    	}   
	}
	else
	{
	    	 ret = ERR_CFW_INVALID_PARAMETER;
	}
	if(ERR_SUCCESS == ret)
		ret = cfg_write_32("cm", "PUBLIC", nPubVal);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSSN);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSSN exit");
	return ret;
}

/*===========================================================================
Get the SSN value.

bCSSI: 0 ~ 1  b0
bCSSU: 0 ~ 1  b1

KEY: "PUBLIC"
===========================================================================*/
UINT32 CFW_CfgGetSSN (UINT8* bCSSI, UINT8* bCSSU, CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	UINT32 nPubVal= 0;
	BOOL bDefault = FALSE;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetSSN);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSSN  nSimID =0x%x\n", nSimID);
	ret = cfg_read_32("cm", "PUBLIC", &nPubVal);
	if(ERR_SUCCESS!= ret)
	{
	   CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_32, Error ret =0x%x\n", ret);
	   ret = cfg_readdefault("cm", "PUBLIC", &nPubVal);
	   if(ERR_SUCCESS!=ret)
	   {
		   CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, Error ret =0x%x\n", ret);
	          CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSSN);
		   return ERR_CFG_READ_REG_FAILURE;
	   }
	   bDefault = TRUE;
	}
	if((CFW_SIM_0<=nSimID)&&(nSimID< CFW_SIM_COUNT))
	{
	    GET_BIT_VALUE(nPubVal, (nSimID*2), *bCSSI);
	    GET_BIT_VALUE(nPubVal, (nSimID*2 + 1), *bCSSU);
	}
	else
		ret= ERR_CFW_INVALID_PARAMETER;
	
	if(bDefault)
	    cfg_write_8("cm", "PUBLIC", nPubVal);
	
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSSN);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSSN  *bCSSI = 0x%x, *bCSSU =0x%x\n", *bCSSI, *bCSSU);

	return ERR_SUCCESS;

}

UINT32 CFW_CfgSetCallWaiting (UINT8 n,CFW_SIM_ID nSimID)
{

	UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetCallWaiting);

	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetCallWaiting  n = 0x%x, nSimID =0x%x\n", n,nSimID);
	ret = CFW_CfgNumRangeCheck(n, CM_CCWA_RANGE);
	if(ERR_SUCCESS == ret)
	{
	    if( (CFW_SIM_0<=nSimID)&&(CFW_SIM_COUNT>nSimID))
	    {
	    	char sims[2] = {(nSimID+0x30),0x00};
		char nameSim[10] = {0x00,};
		strcpy(nameSim, "CCWA_SIM");
	      strcat(nameSim, sims);

		ret = cfg_write_8("cm",nameSim, n);	    
	   }else
			ret = ERR_CFW_INVALID_PARAMETER;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetCallWaiting);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetCallWaiting exit");
	return ret;
}


UINT32 CFW_CfgGetCallWaiting (UINT8* n,CFW_SIM_ID nSimID)
{

	UINT32 ret = ERR_SUCCESS;
	BOOL bDefault = FALSE;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetCallWaiting);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetCallWaiting  nSimID =0x%x\n", nSimID);
      INT8 simid_c[2]  = {0x30,0x00};
	INT8 name_s[11] = {0x00,};
	strcpy(name_s,"CCWA_SIM");
	
	if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
	{
		simid_c[0] = simid_c[0] + nSimID;
		strcat(name_s, simid_c);
	       ret = cfg_read_8("cm", name_s, n);
	}else{
	       ret = ERR_CFW_INVALID_PARAMETER;
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
		return ret;
	}
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret = 0x%x\n", ret);
		
		ret = cfg_readdefault("cm", name_s, n);
		
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret = 0x%x\n", ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
			return ERR_CFG_READ_REG_FAILURE;
		}
		bDefault = TRUE;
	}
	else
	{
		ret = CFW_CfgNumRangeCheck(*n, CM_CCWA_RANGE);
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret = 0x%x\n", ret);
			
			ret = cfg_readdefault("cm", name_s, n);
			
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret = 0x%x\n", ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
				return ERR_CFG_READ_REG_FAILURE;
			}
			bDefault = TRUE;
		}
	}
	if(bDefault)
	{
	
		ret = cfg_write_8("cm", name_s, *n);
		
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetCallWaiting  *n =0x%x\n", *n);
	return ERR_SUCCESS;

}

UINT32 SRVAPI CFW_CfgSetClir ( UINT8 nClir,
                               CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetClir);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetClir  nClir = 0x%x, nSimID =0x%x\n", nClir, nSimID);
	ret = CFW_CfgNumRangeCheck(nClir, CM_CLIR_RANGE);

	if(ERR_SUCCESS == ret)
	{
	    if( (CFW_SIM_0<=nSimID)&&(CFW_SIM_COUNT>nSimID))
	    {
	    	char sims[2] = {(nSimID+0x30),0x00};
		char nameSim[10] = {0x00,};
		strcpy(nameSim, "CLIR_SIM");
	      strcat(nameSim, sims);

		ret = cfg_write_8("cm",nameSim, nClir);	    
	   }else
			ret = ERR_CFW_INVALID_PARAMETER;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetClir);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetClir exit");
	return ret;
}

UINT32 SRVAPI CFW_CfgGetClir ( UINT8 *n,
	                           CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	BOOL bDefault = FALSE;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetClir);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetClir  nSimID =0x%x\n", nSimID);
	 INT8 simid_c[2]  = {0x30,0x00};
	INT8 name_s[11] = {0x00,};
	strcpy(name_s,"CLIR_SIM");
	
	if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
	{
		simid_c[0] = simid_c[0] + nSimID;
		strcat(name_s, simid_c);
	       ret = cfg_read_8("cm", name_s, n);
	}else{
	       ret = ERR_CFW_INVALID_PARAMETER;
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
		return ret;
	}
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret = 0x%x\n", ret);
		
		ret = cfg_readdefault("cm", name_s, n);
		
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret = 0x%x\n", ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
			return ERR_CFG_READ_REG_FAILURE;
		}
		bDefault = TRUE;
	}
	else
	{
		ret = CFW_CfgNumRangeCheck(*n, CM_CCWA_RANGE);
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret = 0x%x\n", ret);
			
			ret = cfg_readdefault("cm", name_s, n);
			
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret = 0x%x\n", ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
				return ERR_CFG_READ_REG_FAILURE;
			}
			bDefault = TRUE;
		}
	}
	if(bDefault)
	{
	
		ret = cfg_write_8("cm", name_s, *n);
		
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetClir);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetClir  *n =0x%x\n", *n);
	return ERR_SUCCESS;

}

/*==========================================================================
NAME        FUNCTION                PARAMETER   RANGE   DEFAULT
AT+CLIP	    CFW_CfgSetClip 	        N	        0-1	    1


KEY: "PUBLIC"
==========================================================================*/
UINT32 CFW_CfgSetClip ( UINT8 nEnable, CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetClip);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetClip  nEnable = 0x%x, nSimID =0x%x\n", nEnable, nSimID);
	ret = CFW_CfgNumRangeCheck(nEnable, CM_CLIP_RANGE);
	if(ret == ERR_SUCCESS)
	{
		if(CFW_SIM_0==nSimID)
			MemoryInfo.Clip_Value0= nEnable;
		else if(CFW_SIM_1==nSimID)
			MemoryInfo.Clip_Value1= nEnable;
		else if((CFW_SIM_1+1)==nSimID)
			MemoryInfo.Clip_Value2= nEnable;
		else if((CFW_SIM_1+2)==nSimID)
			MemoryInfo.Clip_Value3= nEnable;
		else
			ret=ERR_CFW_INVALID_PARAMETER;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetClip);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetClip exit");
	return ret;
}


UINT32 CFW_CfgGetClip (UINT8 *pEnable,CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetClip);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetClip  nSimID =0x%x\n", nSimID);
	if(CFW_SIM_0==nSimID)
		*pEnable = MemoryInfo.Clip_Value0;
	else if(CFW_SIM_1==nSimID)
		*pEnable = MemoryInfo.Clip_Value1;
	else if((CFW_SIM_1+1)==nSimID)
		*pEnable = MemoryInfo.Clip_Value2;
	else if((CFW_SIM_1+2)==nSimID)
		*pEnable = MemoryInfo.Clip_Value3;
	else
		ret= ERR_CFW_INVALID_PARAMETER;
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetClip);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetClip *pEnable = 0x%x", *pEnable);
	return ret;
}

/*=========================================================================
These functions enable or disable a calling subscriber to get the connected 
line identity (COL) of the called party after setting up a mobile originated 
call. See 3GPP TS 22.081 [3] for more

=========================================================================*/
UINT32 CFW_CfgSetColp ( UINT8 n,
                        CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetColp);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetColp  n = 0x%x, nSimID =0x%x\n", n,nSimID);
	ret = CFW_CfgNumRangeCheck(n, CM_COLP_RANGE);
	if(ERR_SUCCESS == ret)
	{
	    if( (CFW_SIM_0<=nSimID)&&(CFW_SIM_COUNT>nSimID))
	    {
	    	char sims[2] = {(nSimID+0x30),0x00};
		char nameSim[10] = {0x00,};
		strcpy(nameSim, "COLP_SIM");
	      strcat(nameSim, sims);

		ret = cfg_write_8("cm",nameSim, n);	    
	   }else
			ret = ERR_CFW_INVALID_PARAMETER;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetColp);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetColp exit");
	return ret;
}


UINT32 CFW_CfgGetColp ( UINT8 *n,
	                    CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	BOOL bDefault = FALSE;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetColp);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetColp  nSimID =0x%x\n", nSimID);
	 INT8 simid_c[2]  = {0x30,0x00};
	INT8 name_s[11] = {0x00,};
	strcpy(name_s,"COLP_SIM");
	
	if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
	{
		simid_c[0] = simid_c[0] + nSimID;
		strcat(name_s, simid_c);
	       ret = cfg_read_8("cm", name_s, n);
	}else{
	       ret = ERR_CFW_INVALID_PARAMETER;
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
		return ret;
	}
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret = 0x%x\n", ret);
		
		ret = cfg_readdefault("cm", name_s, n);
		
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret = 0x%x\n", ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
			return ERR_CFG_READ_REG_FAILURE;
		}
		bDefault = TRUE;
	}
	else
	{
		ret = CFW_CfgNumRangeCheck(*n, CM_CCWA_RANGE);
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret = 0x%x\n", ret);
			
			ret = cfg_readdefault("cm", name_s, n);
			
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret = 0x%x\n", ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
				return ERR_CFG_READ_REG_FAILURE;
			}
			bDefault = TRUE;
		}
	}
	if(bDefault)
	{
	
		ret = cfg_write_8("cm", name_s, *n);
		
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetColp);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetColp  *n =0x%x\n", *n);
	return ERR_SUCCESS;
}
#else
/*===========================================================================
Configure Supplementary service notifications.
  
bCSSI: 0 ~ 1  b0
bCSSU: 0 ~ 1  b1

KEY: "PUBLIC"
===========================================================================*/
//static UINT32 PUBLIC_cm  = 0;
UINT32 CFW_CfgSetSSN(UINT8 bCSSI, UINT8 bCSSU)
{
	UINT32 ret = ERR_SUCCESS;
	UINT32 nPubVal= 0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetSSN);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSSN, bCSSI = 0x%x, bCSSU =0x%x\n", bCSSI, bCSSU);
	ret = cfg_read_32("cm", "PUBLIC", &nPubVal);
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_32, Error ret =0x%x\n", ret);
		ret = cfg_readdefault("cm", "PUBLIC", &nPubVal);
		if(ERR_SUCCESS!=ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, Error ret =0x%x\n", ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSSN);
			return ERR_CFG_READ_REG_FAILURE;
		}
	}

	if((( bCSSI == 0)||(bCSSI == 1))&&
	  (( bCSSU == 0)||(bCSSU == 1)))
	{
	    /*Set the bit 0 of the nPubVal*/
	    SET_BIT_VALUE(nPubVal, 0, bCSSI);
	    /*Set the bit 1 of the nPubVal*/
	    SET_BIT_VALUE(nPubVal, 1, bCSSU);
	    
	}
	else if((bCSSI == 0xFF) && (( bCSSU == 0)||(bCSSU == 1)))
	{
	    SET_BIT_VALUE(nPubVal, 1, bCSSU);
		
	}
	else if((bCSSU == 0xFF) && (( bCSSI == 0)||(bCSSI == 1)))
	{
	    SET_BIT_VALUE(nPubVal, 0, bCSSI);
		
	}
	else if((bCSSU == 0xFF) && (bCSSI == 0xFF))
	{
	    ret = ERR_SUCCESS;
	}
	else
	{
	    ret = ERR_CFG_PARAM_OUT_RANGR;
	}   

	if(ERR_SUCCESS == ret)
		ret = cfg_write_32("cm", "PUBLIC", nPubVal);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetSSN);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSSN exit");
	return ret;
}

/*===========================================================================
Get the SSN value.

bCSSI: 0 ~ 1  b0
bCSSU: 0 ~ 1  b1

KEY: "PUBLIC"
===========================================================================*/
UINT32 CFW_CfgGetSSN (UINT8* bCSSI, UINT8* bCSSU)
{
	UINT32 ret = ERR_SUCCESS;
	UINT32 nPubVal= 0;
	BOOL bDefault = FALSE;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetSSN);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSSN  enter\n");
	ret = cfg_read_32("cm", "PUBLIC", &nPubVal);
	if(ERR_SUCCESS!= ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_32, Error ret =0x%x\n", ret);
		ret = cfg_readdefault("cm", "PUBLIC", &nPubVal);
		if(ERR_SUCCESS!=ret)
		{
		   CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, Error ret =0x%x\n", ret);
		      CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSSN);
		   return ERR_CFG_READ_REG_FAILURE;
		}
		bDefault = TRUE;
	}
	/*Get the bit 0 of the nPubVal*/
	GET_BIT_VALUE(nPubVal, 0, *bCSSI);
	/*Get the bit 1 of the nPubVal*/
	GET_BIT_VALUE(nPubVal, 1, *bCSSU);

	if(bDefault)
	    cfg_write_8("cm", "PUBLIC", nPubVal);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSSN);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSSN  *bCSSI = 0x%x, *bCSSU =0x%x\n", *bCSSI, *bCSSU);
	return ERR_SUCCESS;

}

/*==============================================================================
Set the call Waiting value.

===============================================================================*/
//static UINT32 CALL_WAIT_cm = 0;
UINT32 CFW_CfgSetCallWaiting (UINT8 n)
{

    UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetCallWaiting);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetCallWaiting  n = 0x%x\n", n);
	ret = CFW_CfgNumRangeCheck(n, CM_CCWA_RANGE);
	if(ERR_SUCCESS == ret)
	{
		ret = cfg_write_8("cm","CCWA", n);
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetCallWaiting);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetCallWaiting exit");
	return ret;
}


UINT32 CFW_CfgGetCallWaiting (UINT8* n)
{
    
    UINT32 ret = ERR_SUCCESS;
    BOOL bDefault = FALSE;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetCallWaiting);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetCallWaiting  enter\n");
    ret = cfg_read_8("cm", "CCWA", n);
    if(ERR_SUCCESS != ret)
    {
	    CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret = 0x%x\n", ret);
	    ret = cfg_readdefault("cm", "CCWA", n);
	    if(ERR_SUCCESS != ret)
	    {
		   CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret = 0x%x\n", ret);
		   CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
		   return ERR_CFG_READ_REG_FAILURE;
	    }
	    bDefault = TRUE;
    }
    else
    {
	    ret = CFW_CfgNumRangeCheck(*n, CM_CCWA_RANGE);
	    if(ERR_SUCCESS != ret)
	    {
		    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret = 0x%x\n", ret);
		    ret = cfg_readdefault("cm", "CCWA", n);
		    if(ERR_SUCCESS != ret)
		    {
				CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret = 0x%x\n", ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
				return ERR_CFG_READ_REG_FAILURE;
		    }
		    bDefault = TRUE;
	    }
    }
    if(bDefault)
	    cfg_write_8("cm", "CCWA", *n);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetCallWaiting  *n =0x%x\n", *n);
	return ERR_SUCCESS;

}
/*==============================================================================

==============================================================================*/
UINT32 SRVAPI CFW_CfgSetClir ( UINT8 nClir )
{
    UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetClir);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetClir  nClir = 0x%x\n", nClir);
    ret = CFW_CfgNumRangeCheck(nClir, CM_CLIR_RANGE);
    if(ERR_SUCCESS == ret)
    {
	    //WRITE_NUMBER_SETTING_8("CLIR", nClir, "cm" );
	    ret = cfg_write_8("cm", "CLIR", nClir);
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetClir);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetClir exit");
    return ret;
}

UINT32 SRVAPI CFW_CfgGetClir ( UINT8 *n )
{
    UINT32 ret = ERR_SUCCESS;
    BOOL bDefault = FALSE;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetClir);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetClir  enter\n");
    ret = cfg_read_8("cm", "CLIR", n);
    if(ERR_SUCCESS != ret)
    {
	    CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret = 0x%x\n", ret);
	    //here need to get default value
	    ret = cfg_readdefault("cm", "CLIR", n);
	    if(ERR_SUCCESS != ret)
	    {
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret = 0x%x\n", ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetClir);
			return ret;
	    }
	    bDefault = TRUE;
    }
    else
    {
	    ret = CFW_CfgNumRangeCheck(*n, CM_CLIR_RANGE);
	    if(ERR_SUCCESS != ret)
	    {
		    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret = 0x%x\n", ret);
		    ret = cfg_readdefault("cm", "CLIR", n);
		    if(ERR_SUCCESS != ret)
		    {
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret = 0x%x\n", ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetClir);
			return ret;
		    }
		    bDefault = TRUE;
	    }
    }
    
    if(bDefault)
	    cfg_write_8("cm", "CLIR", *n);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetClir);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetClir  *n =0x%x\n", *n);
    return ERR_SUCCESS;

}

/*==========================================================================
NAME        FUNCTION                PARAMETER   RANGE   DEFAULT
AT+CLIP	    CFW_CfgSetClip 	        N	        0-1	    1


KEY: "PUBLIC"
==========================================================================*/
UINT32 CFW_CfgSetClip ( UINT8 nEnable )
{
    UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetClip);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetClip  nEnable = 0x%x\n", nEnable);
    /*Check the range*/
    ret = CFW_CfgNumRangeCheck(nEnable, CM_CLIP_RANGE);
    if(ret == ERR_SUCCESS)
	    MemoryInfo.Clip_Value = nEnable;
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetClip);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetClip exit");
    return ret;
}


UINT32 CFW_CfgGetClip (UINT8 *pEnable)
{
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetClip);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetClip  enter\n");
    *pEnable = MemoryInfo.Clip_Value;
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetClip);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetClip *pEnable = 0x%x", *pEnable);
    return ERR_SUCCESS;
}


/*=========================================================================
These functions enable or disable a calling subscriber to get the connected 
line identity (COL) of the called party after setting up a mobile originated 
call. See 3GPP TS 22.081 [3] for more

=========================================================================*/
UINT32 CFW_CfgSetColp ( UINT8 n )
{
    UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetColp);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetColp  n = 0x%x\n", n);
    ret = CFW_CfgNumRangeCheck(n, CM_COLP_RANGE);
    if(ERR_SUCCESS != ret)
    {
 	 CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret = 0x%x\n", ret);
 	 CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetColp);
        return ERR_CFG_PARAM_OUT_RANGR;
    }
    ret = cfg_write_8("cm", "COLP", n);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetColp);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetColp exit");
    return ret;
}


UINT32 CFW_CfgGetColp ( UINT8 *n )
{
    UINT32 ret = ERR_SUCCESS;
    BOOL bDefault = FALSE;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetColp);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetColp  entern");
    ret = cfg_read_8("cm", "COLP", n);
    if(ERR_SUCCESS != ret)
    {
 	    CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret = 0x%x\n", ret);
	    ret = cfg_readdefault("cm", "COLP", n);
	    if(ERR_SUCCESS != ret)
	    {
	       CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret = 0x%x\n", ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetColp);
		return ERR_CFG_READ_REG_FAILURE;
	    }
	    bDefault = TRUE;
    }
    else
    {
	    ret = CFW_CfgNumRangeCheck(*n, CM_COLP_RANGE);
	    if(ERR_SUCCESS != ret)
	    {
	           CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret = 0x%x\n", ret);
		    ret = cfg_readdefault("cm", "COLP", n);
		    if(ERR_SUCCESS != ret)
		    {
	                 CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret = 0x%x\n", ret);
			   CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetColp);
			   return ERR_CFG_READ_REG_FAILURE;
		    }
		    bDefault = TRUE;
	    }
    }
    if(bDefault)
	    cfg_write_8("cm", "COLP", *n);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetColp);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetColp  *n =0x%x\n", *n);
    return ERR_SUCCESS;
}

#endif

/*===========================================================================
nToneDuration 
[in] Specify the tone duration(1-255) in 100ms(for example, the value 4 means 
400 milliseconds). The default is zero the tone duration is 300ms.
===========================================================================*/
UINT32 CFW_CfgSetToneDuration ( UINT8  nToneDuration )
{
#if 0
    UINT32 ret = ERR_SUCCESS;
    WRITE_NUMBER_SETTING_8("TONE_DURA", nToneDuration, "cm" );    
    return ret;
#endif
	return ERR_CFW_NOT_SUPPORT;
}

UINT32 CFW_CfgGetToneDuration ( UINT8* pToneDuration )
{
#if 0    
	UINT32 ret = ERR_SUCCESS;
    READ_NUMBER_SETTING_8("TONE_DURA", *pToneDuration, "cm" ); 
    return ret;
#endif     
	return ERR_CFW_NOT_SUPPORT;

}


/*==============================================================================
Set the call forwarding number and conditions.

KEY: "CFD_NUMBER_1" "CFD_NUMBER_2" "CFD_NUMBER_3"
     "CFD_NUMBER_4" "CFD_NUMBER_5" "CFD_NUMBER_6"
     "CFD_OTHERS" "CFD_STATUS"
===============================================================================*/
UINT32 CFW_CfgSetCallForwarding ( CFW_CFG_CALLFORWARDING* pCallForwarding)
{
#if 0
    UINT32 ret = ERR_SUCCESS;
	#ifndef MF32_BASE_REG_DISABLE
    UINT32 *pInt = 0; UINT32 integer = 0;
    
    UINT8 reason = 0;
    UINT8 mode = 0;
    UINT8 class = 0;
    UINT8 time = 0;
  
    pInt = (UINT32*)(&(pCallForwarding->number));
    
    /*Call fording number*/    
    WRITE_NUMBER_SETTING("CFD_NUMBER_1", *pInt, "cm");
    pInt++;
    
    WRITE_NUMBER_SETTING("CFD_NUMBER_2", *pInt, "cm");
    pInt++;
    
    WRITE_NUMBER_SETTING("CFD_NUMBER_3", *pInt, "cm");
    pInt++;
    
    WRITE_NUMBER_SETTING("CFD_NUMBER_4", *pInt, "cm");
    pInt++;

    WRITE_NUMBER_SETTING("CFD_NUMBER_5", *pInt, "cm");
    pInt++;
    
    WRITE_NUMBER_SETTING("CFD_NUMBER_6", *pInt, "cm");

    /*Call fording other parameters*/    
    reason = pCallForwarding->reason;    mode = pCallForwarding->mode;
    class = pCallForwarding->class;      time = pCallForwarding->time;

    if(reason >= 0)/* etc...*/
    {
        integer = (reason<<24) | (mode<<16) | (class<<8) | (time) ;
        WRITE_NUMBER_SETTING("CFD_OTHERS", integer, "cm");
        WRITE_NUMBER_SETTING("CFD_STATUS", pCallForwarding->status, "cm");
    }
    else
    {
        ret = ERR_CFG_PARAM_OUT_RANGR;
    }

    #endif 
    
    return ret;
#endif
	return ERR_CFW_NOT_SUPPORT;
}

/*================================================================================
Get the call forwarding number and conditions.

KEY: "CFD_NUMBER_1" "CFD_NUMBER_2" "CFD_NUMBER_3"
     "CFD_NUMBER_4" "CFD_NUMBER_5" "CFD_NUMBER_6"
     "CFD_OTHERS" "CFD_STATUS"
================================================================================*/
UINT32 CFW_CfgGetCallForwarding (CFW_CFG_CALLFORWARDING* pCallForwardingArray,
                                 UINT8* pCount
                                 )  /*Need to verify the parameters...*/
{
#if 0
    UINT32 ret = ERR_SUCCESS;
    UINT32 integer = 0;

    UINT8 reason = 0;
    UINT8 mode = 0;
    UINT8 class = 0;
    UINT8 time = 0;

    #ifndef MF32_BASE_REG_DISABLE
    READ_NUMBER_SETTING("CFD_NUMBER_1", integer, "cm");
    SUL_MemCopy32(&(pCallForwardingArray->number.nTelNumber[0]), &integer, 4);

    READ_NUMBER_SETTING("CFD_NUMBER_2", integer, "cm");
    SUL_MemCopy32(&(pCallForwardingArray->number.nTelNumber[4]), &integer, 4);

    READ_NUMBER_SETTING("CFD_NUMBER_3", integer, "cm");
    SUL_MemCopy32(&(pCallForwardingArray->number.nTelNumber[8]), &integer, 4);

    READ_NUMBER_SETTING("CFD_NUMBER_4", integer, "cm");
    SUL_MemCopy32(&(pCallForwardingArray->number.nTelNumber[12]), &integer, 4);

    READ_NUMBER_SETTING("CFD_NUMBER_5", integer, "cm");
    SUL_MemCopy32(&(pCallForwardingArray->number.nTelNumber[16]), &integer, 4);

    READ_NUMBER_SETTING("CFD_NUMBER_6", integer, "cm");
    pCallForwardingArray->number.nTelNumber[20] = (UINT8)((integer & 0xff000000)>>24);
    pCallForwardingArray->number.nTelNumber[21] = (UINT8)((integer & 0x00ff0000)>>16);
    pCallForwardingArray->number.nTelNumber[22] = (UINT8)((integer & 0x0000ff00)>>8);
    pCallForwardingArray->number.nType =  (UINT8)((integer & 0x000000ff));


    READ_NUMBER_SETTING("CFD_OTHERS", integer, "cm");

    reason = (UINT8)((integer & 0xFF000000) >> 24); mode = (UINT8)((integer & 0xFF0000) >> 16);
    class = (UINT8)((integer & 0xFF00) >> 8);  time = (UINT8)(integer & 0xFF);
    
    pCallForwardingArray->reason = reason;   pCallForwardingArray->mode = mode;
    pCallForwardingArray->class = class;     pCallForwardingArray->time = time;

    READ_NUMBER_SETTING("CFD_STATUS", integer, "cm");
    pCallForwardingArray->status = (UINT8)integer;

    /*Need someone to fill out...*/
    if( pCallForwardingArray->number.nTelNumber[0] == 0)
        pCount = pCount;

	#endif 
    return ret;
#endif
	return ERR_CFW_NOT_SUPPORT;

}

/*==========================================================================
NAME    FUNCTION                          PARAMETER   RANGE  DEFAULT
AT+CRC	CFW_CfgSetIncomingCallResultMode  nMode	      0-1	 0

==========================================================================*/
UINT32 CFW_CfgSetIncomingCallResultMode(UINT8 nMode)
{
#if 0
    UINT32 ret = ERR_SUCCESS;
    
	#ifndef MF32_BASE_REG_DISABLE
    /*Check the range*/
    ret = CFW_CfgNumRangeCheck(nMode, AT_CRC_RANGE);
    if(ret != ERR_SUCCESS)
        return ret;

    MemoryInfo.IncomingCallResultMode = nMode;
	#endif 

    return ret;
	return ERR_CFW_NOT_SUPPORT;
#endif

    //UINT32 ret = CFW_CfgNumRangeCheck(nMode, AT_CRC_RANGE);
    //if(ret != ERR_SUCCESS)
    //    return ret;
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetIncomingCallResultMode  nMode =0x%x\n", nMode);
    if(nMode > 1)
        return ERR_CFG_PARAM_OUT_RANGR;
	
    MemoryInfo.IncomingCallResultMode = nMode;
    return ERR_SUCCESS;
}

UINT32 CFW_CfgGetIncomingCallResultMode(UINT8 *nMode)
{
#if 0
	return ERR_CFW_NOT_SUPPORT;
#endif
    *nMode = MemoryInfo.IncomingCallResultMode;

    return ERR_SUCCESS;
}


/*==========================================================================
NAME        FUNCTION                PARAMETER   RANGE                   DEFAULT
AT+CBST	    CFW_CfgSetBearServType	nSpeed	    0,4,6,7,14,68,70,71,75	7

==========================================================================*/
UINT32 CFW_CfgSetBearServType(UINT8 nSpeed)
{
#if 0
    UINT32 ret = ERR_SUCCESS;
    
    /*Check the range*/
    ret = CFW_CfgNumRangeCheck(nSpeed, AT_BST_RANGE);
    if(ret != ERR_SUCCESS)
        return ret;
    
    MemoryInfo.BearServType = nSpeed;

    return ret;
#endif
	return ERR_CFW_NOT_SUPPORT;
}

UINT32 CFW_CfgGetBearServType(UINT8 *nSpeed)
{
#if 0
    *nSpeed = MemoryInfo.BearServType;
    return ERR_SUCCESS;
#endif
	return ERR_CFW_NOT_SUPPORT;

}

/*==========================================================================
NAME    FUNCTION                            PARAMETER   RANGE    DEFAULT
ATX 	CFW_CfgSetConnectResultFormatCode	nValue	    0-4	     4

==========================================================================*/
UINT32 CFW_CfgSetConnectResultFormatCode(UINT8 nValue)
{
#if 1
    //UINT32 ret = ERR_SUCCESS;
    
    /*Check the range*/
    //ret = CFW_CfgNumRangeCheck(nValue, AT_X_RANGE);
    //if(ret != ERR_SUCCESS)
    //    return ret;
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetConnectResultFormatCode  nValue =0x%x\n", nValue);
    if(nValue > 4)
	return ERR_CFG_PARAM_OUT_RANGR;
    
    MemoryInfo.ConnectResultFormatCode = nValue;

    return ERR_SUCCESS;
#endif
	//return ERR_CFW_NOT_SUPPORT;
}

UINT32 CFW_CfgGetConnectResultFormatCode(UINT8 *nValue)
{
#if 1
    //UINT32 ret = ERR_SUCCESS;
    *nValue = MemoryInfo.ConnectResultFormatCode;
    return ERR_SUCCESS;
#endif
	//return ERR_CFW_NOT_SUPPORT;
}


