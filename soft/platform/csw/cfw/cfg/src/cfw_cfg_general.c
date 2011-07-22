///*************************************************************
//hameina 2006.8.21 modified
//
//notice: this version is based on reg systerm
//
//modify record:
// 2006.9.1 hameina [+] CFW_CfgSetIndicatorEventInfo should check input param
//                   CFW_CfgSetIndicatorEvent 
//                   CFW_CfgGetIndicatorEvent  do the same
//2006.9.13  hameina[mod] CFW_CfgGetIndicatorEvent, cause this  function is used
//           so frequently, I'll use a global(IND_EVENT_general) to save the value from reg.
//2006.9.13  hameina[mod] CFW_CfgSetIndicatorEvent, this function set to invalid status.
//2006.9.14 hameina [mod] ERR_CFW_NOTSUPPORT->ERR_CFW_NOT_SUPPORT, who define this
//    wrong err code murdered hameina.
//2006.9.22 hameina [mod] getvalue() the type of param datasize changed:UINT32->UINT8
//2006.9.29 hameina [mod] cfg_readdefault() the type of function changed:bool->UINT32
//2007.8.17 hameina[+] UINT32 CFW_CfgGetIMSI(UINT8 * pIMSI)
//2007.8.17 hameina[+] UINT32 CFW_CfgSetIMSI(UINT8 * pIMSI)
///**************************************************************

#include <cfw.h>
#include "cfw_cfg.h"
#include "csw_debug.h"
#include <string.h>
/*==========================================================================
Set the level of functionality.

KEY: "FUN_LEVEL"
==========================================================================*/
//static UINT32 FUN_LEVEL_general = 0;
UINT32 CFW_CfgSetPhoneFunctionality( UINT8 nLevel, UINT8 nRst)
{
    UINT32 ret = ERR_SUCCESS;
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetPhoneFunctionality, nLevel = 0x%x, nRst = 0x%x\n", nLevel,nRst);
    //ret = CFW_CfgNumRangeCheck(nLevel, AT_PARAM_CFUN_LEVEL_RANGE);
    //if(ret != ERR_SUCCESS)
   //     return ret;
    if(nLevel > 127)
	return ERR_CFG_PARAM_OUT_RANGR;
   
   // ret = CFW_CfgNumRangeCheck(nRst, AT_PARAM_CFUN_RESULT_RANGE);
    //if(ret != ERR_SUCCESS)
   //     return ret;
    if(nRst > 1)
	return ERR_CFG_PARAM_OUT_RANGR;
	
    MemoryInfo.CFUN_Level = nLevel;
    MemoryInfo.CFUN_Result = nRst;
    return ret;
#if 0
    #ifdef MF32_BASE_REG_DISABLE
	FUN_LEVEL_general = nLevel;
    #else
    WRITE_NUMBER_SETTING_8("FUN_LEVEL", nLevel, "general");
	#endif 

    /*
        Set the Functionality...
    */

    if(nRst)
    {
      /*RESET...*/
    }

    return ret; 
	return ERR_CFW_NOT_SUPPORT;
#endif
}

/*==========================================================================
Get the level of functionality.

KEY: "FUN_LEVEL"
==========================================================================*/
UINT32 SRVAPI CFW_CfgGetPhoneFunctionality(UINT8* pLevel)
{
#if 0
    UINT32 ret = ERR_SUCCESS;
    UINT32 nPubVal= 0;
      
    INT32 integer = 0;

    #ifdef MF32_BASE_REG_DISABLE
	integer = FUN_LEVEL_general;
    #else
    READ_NUMBER_SETTING_8("FUN_LEVEL", integer, "general");
    #endif 
    
    *pLevel = (UINT8)integer;

    return ret;
	return ERR_CFW_NOT_SUPPORT;
#endif
	*pLevel = MemoryInfo.CFUN_Level;
       CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetPhoneFunctionality, *pLevel = 0x%x\n", *pLevel);
	return ERR_SUCCESS;
}

/*==========================================================================
AT&W

==========================================================================*/
UINT32 CFW_CfgStoreUserProfile ( VOID)
{
       UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgStoreUserProfile);
       CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgStoreUserProfile  enter\n");
        ret = cfg_write_hex("general", "MEMORYINFO", &MemoryInfo, sizeof(MEM_INFO_STORAGE));
#if 0
    WRITE_NUMBER_SETTING_8("BearServType", MemoryInfo.BearServType, "general" );

    WRITE_NUMBER_SETTING_8("Clip_Value", MemoryInfo.Clip_Value, "general" );

    WRITE_NUMBER_SETTING("CMER_param", MemoryInfo.CMER_parameter, "general");

    WRITE_NUMBER_SETTING("CNMI_param", MemoryInfo.CNMI_parameter, "general");
    
    WRITE_NUMBER_SETTING_8("CtRtFtCe", MemoryInfo.ConnectResultFormatCode, "general" );

    WRITE_NUMBER_SETTING_8("EchoMode", MemoryInfo.EchoMode, "general");
    
    WRITE_NUMBER_SETTING_8("ErrorReport", MemoryInfo.ErrorReport, "general");

    WRITE_NUMBER_SETTING_8("IgClRtMe", MemoryInfo.IncomingCallResultMode, "general");

    WRITE_NUMBER_SETTING_8("NewSmsInd", MemoryInfo.NewSmsIndication, "general");
    
    WRITE_NUMBER_SETTING_8("NwStatus", MemoryInfo.NwStatus, "general");    
    
    WRITE_NUMBER_SETTING_8("RtCeFtMe", MemoryInfo.ResultCodeFormatMode, "general");
    
    WRITE_NUMBER_SETTING_8("RtCePtMe", MemoryInfo.ResultCodePresentMode, "general");

    WRITE_NUMBER_SETTING_8("RLPType_iws", MemoryInfo.RLPType_iws, "general");
    WRITE_NUMBER_SETTING_8("RLPType_mws", MemoryInfo.RLPType_mws, "general");
    WRITE_NUMBER_SETTING_8("RLPType_N2", MemoryInfo.RLPType_N2, "general");
    WRITE_NUMBER_SETTING_8("RLPType_T1", MemoryInfo.RLPType_T1, "general");

    WRITE_NUMBER_SETTING_8("StSsSe", MemoryInfo.SelectSmsService, "general");
    
    WRITE_NUMBER_SETTING_8("SeRgMe", MemoryInfo.ServiceReportingMode, "general");
#endif
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_hex ret=0x%x\n",ret);
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgStoreUserProfile);
    return ret;
}

/*==========================================================================
AT&F

==========================================================================*/
UINT32 CFW_CfgRestoreFactoryProfile ( VOID)
{
#if 0
    MemoryInfo.BearServType = AT_BST_DEDAULT;

    MemoryInfo.Clip_Value = AT_CLIP_DEDAULT;

    MemoryInfo.CMER_parameter = CMER_DEFAULT_VALUE;

    MemoryInfo.CNMI_parameter = CNMI_DEFAULT_VALUE;

    MemoryInfo.ConnectResultFormatCode = AT_X_DEDAULT;

    MemoryInfo.EchoMode = AT_E_DEDAULT;
    
    MemoryInfo.ErrorReport = AT_CMEE_DEDAULT;

    MemoryInfo.IncomingCallResultMode = AT_CRC_DEDAULT;

    MemoryInfo.NewSmsIndication = NEW_SMS_DEFAULT_VALUE;
    
    MemoryInfo.NwStatus = AT_REG_DEDAULT;    
    
    MemoryInfo.ResultCodeFormatMode = AT_V_DEDAULT;
    
    MemoryInfo.ResultCodePresentMode = AT_Q_DEDAULT;

    MemoryInfo.RLPType_iws = AT_RLP_IWS_DEDAULT;
    MemoryInfo.RLPType_mws = AT_RLP_MWS_DEDAULT;
    MemoryInfo.RLPType_N2 = AT_RLP_M2_DEDAULT;
    MemoryInfo.RLPType_T1 = AT_RLP_T1_DEDAULT;

    MemoryInfo.SelectSmsService = AT_SMS_SERV_SELECT_DEDAULT;
    
    MemoryInfo.ServiceReportingMode = AT_CR_DEDAULT;
#endif
	UINT32 result;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgRestoreFactoryProfile);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgRestoreFactoryProfile \n");
	result = cfg_readdefault("general", "MEMORYINFO", & MemoryInfo);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgRestoreFactoryProfile);	
	if(ERR_SUCCESS == result)
	    return ERR_SUCCESS;
       else
	    return ERR_CFG_READ_REG_FAILURE;

}

/*==========================================================================
AT&Z

==========================================================================*/
UINT32 CFW_CfgGetUserProfile ( VOID)
{
    UINT32 ret = ERR_SUCCESS,ret1=ERR_SUCCESS, ret2 = ERR_SUCCESS,ret3=ERR_SUCCESS;
#if 0
    READ_NUMBER_SETTING_8("BearServType", MemoryInfo.BearServType, "general" );

    READ_NUMBER_SETTING_8("Clip_Value", MemoryInfo.Clip_Value, "general" );

    READ_NUMBER_SETTING_16("CMER_param", MemoryInfo.CMER_parameter, "general");

    READ_NUMBER_SETTING_16("CNMI_param", MemoryInfo.CNMI_parameter, "general");
    
    READ_NUMBER_SETTING_8("CtRtFtCe", MemoryInfo.ConnectResultFormatCode, "general" );

    READ_NUMBER_SETTING_8("EchoMode", MemoryInfo.EchoMode, "general");
    
    READ_NUMBER_SETTING_8("ErrorReport", MemoryInfo.ErrorReport, "general");

    READ_NUMBER_SETTING_8("IgClRtMe", MemoryInfo.IncomingCallResultMode, "general");

    READ_NUMBER_SETTING_8("NewSmsInd", MemoryInfo.NewSmsIndication, "general");
    
    READ_NUMBER_SETTING_8("NwStatus", MemoryInfo.NwStatus, "general");    
    
    READ_NUMBER_SETTING_8("RtCeFtMe", MemoryInfo.ResultCodeFormatMode, "general");
    
    READ_NUMBER_SETTING_8("RtCePtMe", MemoryInfo.ResultCodePresentMode, "general");

    READ_NUMBER_SETTING_8("RLPType_iws", MemoryInfo.RLPType_iws, "general");
    READ_NUMBER_SETTING_8("RLPType_mws", MemoryInfo.RLPType_mws, "general");
    READ_NUMBER_SETTING_8("RLPType_N2", MemoryInfo.RLPType_N2, "general");
    READ_NUMBER_SETTING_8("RLPType_T1", MemoryInfo.RLPType_T1, "general");

    READ_NUMBER_SETTING_8("StSsSe", MemoryInfo.SelectSmsService, "general");
    
    READ_NUMBER_SETTING_8("SeRgMe", MemoryInfo.ServiceReportingMode, "general");
#endif
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetUserProfile);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetUserProfile enter\n");
	ret = cfg_read_hex("general", "MEMORYINFO", &MemoryInfo, (UINT8)sizeof(MEM_INFO_STORAGE));
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret=0x%x\n",ret);
		ret = CFW_CfgRestoreFactoryProfile();
		if(ERR_SUCCESS != ret)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgRestoreFactoryProfile, error ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetUserProfile);
			return ret;
		}
	}
	else
	{
#ifdef CFW_MULTI_SIM	
		ret = CFW_CfgNumRangeCheck(MemoryInfo.Clip_Value0, CM_CLIP_RANGE);
		ret1 = CFW_CfgNumRangeCheck(MemoryInfo.NwStatus0, NW_REG_RANGE);
		ret2 = CFW_CfgNumRangeCheck(MemoryInfo.Clip_Value1, CM_CLIP_RANGE);
		ret3 = CFW_CfgNumRangeCheck(MemoryInfo.NwStatus1, NW_REG_RANGE);
#else
		ret = CFW_CfgNumRangeCheck(MemoryInfo.Clip_Value, CM_CLIP_RANGE);
		ret1 = CFW_CfgNumRangeCheck(MemoryInfo.NwStatus, NW_REG_RANGE);
#endif
		if(ERR_SUCCESS !=(ret + ret1 + ret2 + ret3))
		{
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret=0x%x, ret1=0x%x, ret2=0x%x, ret3=0x%x\n", ret, ret1, ret2, ret3);
			ret = CFW_CfgRestoreFactoryProfile();
			if(ERR_SUCCESS != ret)
			{
				CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgRestoreFactoryProfile, error ret = 0x%x\n",ret);
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetUserProfile);
				return ret;
			}
		}
	}
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetUserProfile exit\n");
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetUserProfile);
       return ret;
}

/*==========================================================================
NAME    FUNCTION                            PARAMETER   RANGE  DEFAULT
AT+CR	CFW_CfgSetServiceReportingMode	    nMode	    0-1	   0


==========================================================================*/
UINT32 CFW_CfgSetServiceReportingMode(UINT8 nMode)
{
#if 0
    UINT32 ret = ERR_SUCCESS;
    
    /*Check the range*/
    ret = CFW_CfgNumRangeCheck(nMode, AT_CR_RANGE);
    if(ret != ERR_SUCCESS)
        return ret;

    MemoryInfo.ServiceReportingMode = nMode;

    return ret;
#endif
	return ERR_CFW_NOT_SUPPORT;
}

UINT32 CFW_CfgGetServiceReportingMode(UINT8 *nMode)
{
    //*nMode = MemoryInfo.ServiceReportingMode;
	return ERR_CFW_NOT_SUPPORT;

}

/*==========================================================================
NAME        FUNCTION            PARAMETER   RANGE   DEFAULT
AT+CRLP	    CFW_CfgSetRLPType	iws	        0-61	61
		                        mws	        0-61	61
    		                    T1	        48-255	78
		                        N2	        1-255	6
==========================================================================*/
UINT32 CFW_CfgSetRLPType(UINT8 iws, UINT8 mws, UINT8 T1, UINT8 N2)
{
#if 0
    UINT32 ret = ERR_SUCCESS;
    
    /*Check the range*/
    ret = CFW_CfgNumRangeCheck(iws, AT_RLP_IWS_RANGE);
    if(ret != ERR_SUCCESS)
        return ret;
    
    MemoryInfo.RLPType_iws = iws;

    ret = CFW_CfgNumRangeCheck(mws, AT_RLP_MWS_RANGE);
    if(ret != ERR_SUCCESS)
        return ret;
    
    MemoryInfo.RLPType_mws = mws;

    ret = CFW_CfgNumRangeCheck(T1, AT_RLP_T1_RANGE);
    if(ret != ERR_SUCCESS)
        return ret;
    
    MemoryInfo.RLPType_T1 = T1;

    ret = CFW_CfgNumRangeCheck(N2, AT_RLP_N2_RANGE);
    if(ret != ERR_SUCCESS)
        return ret;
    
    MemoryInfo.RLPType_N2 = N2;
    return ret;    
#endif
	return ERR_CFW_NOT_SUPPORT;
}

UINT32 CFW_CfgGetRLPType(UINT8 *iws, UINT8 *mws, UINT8 *T1, UINT8 *N2)
{
#if 0
    *iws = MemoryInfo.RLPType_iws;
    *mws = MemoryInfo.RLPType_mws;
    *T1 = MemoryInfo.RLPType_T1;
    *N2 = MemoryInfo.RLPType_N2;

    return ERR_SUCCESS;
#endif
	return ERR_CFW_NOT_SUPPORT;
}

/*==========================================================================
Get the character sets. 

KEY: "CHAR_SET"
==========================================================================*/
//UINT8 CharSet[10] = {0};

UINT32 CFW_CfgGetCharacterSet (UINT8* chset)
{   
#if 0
	UINT32 ret = ERR_SUCCESS;
	READ_NUMBER_SETTING_8("CHAR_SET", *chset, "general");
	return ret;
#endif
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetCharacterSet, *chset = 0x%x\n",*chset);
	*chset = MemoryInfo.CharacterSet;
	return ERR_SUCCESS;

}

/*==========================================================================
Set the character sets.

KEY: "CHAR_SET"
==========================================================================*/
UINT32 CFW_CfgSetCharacterSet (UINT8 chset)
{
#if 0
    UINT32 ret = ERR_SUCCESS;
    
    if(chset >= 0) /*Need to verify the range*/
    {
        WRITE_NUMBER_SETTING_8("CHAR_SET", chset, "general");
    }
    else
    {
        ret = ERR_CFG_PARAM_OUT_RANGR;
    }

    return ret;
    //return ERR_CFW_NOT_SUPPORT;
    UINT32 ret = ERR_SUCCESS;
    UINT8 Str_GSM[5] = "GSM";
    UINT8 Str_UCS2[5] = "UCS2";

    /*Check the range*/
    //ret = CFW_CfgStrRangeCheck(chset, AT_CSCS_RANGE);
    //if(ret != ERR_SUCCESS)
    //    return ret;
    if(SUL_StrCaselessCompare((CONST TCHAR *)chset, (CONST TCHAR *) Str_GSM) || SUL_StrCaselessCompare((CONST TCHAR *)chset, (CONST TCHAR *) Str_UCS2))
        return ERR_CFG_PARAM_OUT_RANGR;

   SUL_MemCopy8(CharSet, chset, SUL_Strlen(chset));
    MemoryInfo.CharacterSet = CharSet;

    return ret;
#endif
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetCharacterSet, chset = 0x%x\n",chset);
	if((chset != CFW_CHSET_UCS2) && (chset != CFW_CHSET_GSM))
	    return ERR_CFG_PARAM_OUT_RANGR;

	MemoryInfo.CharacterSet = chset;
	return ERR_SUCCESS;
}

/*==========================================================================
The CFW_CfgGetEventReport function get the setting.

==========================================================================*/
UINT32 CFW_CfgGetEventReport(UINT16* pMer)
{
#if 0
    UINT32 ret = ERR_SUCCESS;
        
    *pMer = MemoryInfo.CMER_parameter;
    
    return ret;
#endif
	return ERR_CFW_NOT_SUPPORT;
}

/*==========================================================================
The CFW_CfgSetEventReport function controls the presentation of the event reporting.

==========================================================================*/
UINT32 CFW_CfgSetEventReport(UINT16 Mer)
{
#if 0
    UINT32 ret = ERR_SUCCESS;
    
    UINT8 nBfr =0, nDisp= 0, nInd=0, nKeyp = 0, nMode =0;

    nMode = CFW_CFG_CMER_MODE(Mer);
    nKeyp = CFW_CFG_CMER_KEYP(Mer);
    nDisp = CFW_CFG_CMER_DISP(Mer);
    nInd = CFW_CFG_CMER_IND(Mer);
    nBfr = CFW_CFG_CMER_BFR(Mer);

    /*Check the range*/
    ret = CFW_CfgNumRangeCheck(nBfr, AT_PARAM_CMER_BFR);
    if(ret != ERR_SUCCESS)
        return ret;

    ret = CFW_CfgNumRangeCheck(nDisp, AT_PARAM_CMER_DISP);
    if(ret != ERR_SUCCESS)
        return ret;

    ret = CFW_CfgNumRangeCheck(nInd, AT_PARAM_CMER_IND);
    if(ret != ERR_SUCCESS)
        return ret;

    ret = CFW_CfgNumRangeCheck(nKeyp, AT_PARAM_CMER_KEYP);
    if(ret != ERR_SUCCESS)
        return ret;

    ret = CFW_CfgNumRangeCheck(nMode, AT_PARAM_CMER_MODE);
    if(ret != ERR_SUCCESS)
        return ret;
    
    /*Real setting*/
    MemoryInfo.CMER_parameter  = Mer;
    return ret;
#endif
	return ERR_CFW_NOT_SUPPORT;
}


/*==========================================================================
NAME    FUNCTION                        PARAMETER   RANGE    DEFAULT
ATV	    CFW_CfgSetResultCodeFormatMode	nMode	    0-1	     1


==========================================================================*/
UINT32 CFW_CfgSetResultCodeFormatMode( UINT8 nMode )
{
    UINT32 ret = ERR_SUCCESS;
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetResultCodeFormatMode, nMode = 0x%x\n", nMode);
    /*Check the range*/
    //ret = CFW_CfgNumRangeCheck(nMode, AT_V_RANGE);
    //if(ret != ERR_SUCCESS)
    //    return ret;
    if(nMode > 1)
	return ERR_CFG_PARAM_OUT_RANGR;
    
    MemoryInfo.ResultCodeFormatMode = nMode;

    return ret;
	//return ERR_CFW_NOT_SUPPORT;
}

/*==========================================================================
NAME    FUNCTION                        PARAMETER   RANGE    DEFAULT
ATQ	    CFW_CfgSetResultCodePresentMode	nMode	    0-1 	 0

==========================================================================*/
UINT32 CFW_CfgSetResultCodePresentMode( UINT8 nMode )
{
    UINT32 ret = ERR_SUCCESS;
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetResultCodePresentMode, nMode = 0x%x\n", nMode);
    /*Check the range*/
    //ret = CFW_CfgNumRangeCheck(nMode, AT_Q_RANGE);
   // if(ret != ERR_SUCCESS)
   //     return ret;
    if(nMode > 1)
	return ERR_CFG_PARAM_OUT_RANGR;
    MemoryInfo.ResultCodePresentMode = nMode;
    return ret;
	//return ERR_CFW_NOT_SUPPORT;
}

UINT32 CFW_CfgGetResultCodePresentMode( UINT8* pMode )
{
    UINT32 ret = ERR_SUCCESS;
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetResultCodePresentMode, *pMode = 0x%x\n", *pMode);
    
    *pMode = MemoryInfo.ResultCodePresentMode;

    return ret;
	//return ERR_CFW_NOT_SUPPORT;
}

UINT32 CFW_CfgGetResultCodeFormatMode( UINT8* pMode )
{
    UINT32 ret = ERR_SUCCESS;
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetResultCodeFormatMode, *pMode = 0x%x\n", *pMode);
    
    *pMode = MemoryInfo.ResultCodeFormatMode;

    return ret;
	//return ERR_CFW_NOT_SUPPORT;
}

/*==========================================================================
NAME    FUNCTION            PARAMETER   RANGE    DEFAULT
ATE 	CFW_CfgSetEchoMode	nMode	    0-1	     1
==========================================================================*/
UINT32 CFW_CfgSetEchoMode(UINT8 nMode)
{
    UINT32 ret = ERR_SUCCESS;
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetEchoMode, nMode = 0x%x\n", nMode);
    /*Check the range*/
    //ret = CFW_CfgNumRangeCheck(nMode, AT_E_RANGE);
    //if(ret != ERR_SUCCESS)
    //    return ret;
    if(nMode > 1)
	return ERR_CFG_PARAM_OUT_RANGR;
    MemoryInfo.EchoMode = nMode;
    return ret;    
	//return ERR_CFW_NOT_SUPPORT;
}

UINT32 CFW_CfgGetEchoMode(UINT8 *nMode)
{
    UINT32 ret = ERR_SUCCESS;
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetEchoMode, *nMode = 0x%x\n", *nMode);
    
    *nMode = MemoryInfo.EchoMode;
    return ret;

	//return ERR_CFW_NOT_SUPPORT;
}


/*=================================================================================
The CFW_CfgGetProductIdentification function retrieves product identification 
information.
=================================================================================*/
UINT32 CFW_CfgGetProductIdentification( UINT8* pIdText )
{
#if 0
    UINT32 ret = ERR_SUCCESS;

    if( SUL_Strlen(PRODUCTION_IDENTIFICATION) > SUL_Strlen(pIdText))
        return ERR_CFG_PARAM_OUT_RANGR;
    
    SUL_StrCopy(pIdText, PRODUCTION_IDENTIFICATION);    
    
    return ret;
#endif
	CSW_TRACE(CFW_CFG_TS_ID, "cfg version R1.3, this version based on reg\n");
	return ERR_CFW_NOT_SUPPORT;
}


/*===================================================================
The CFW_CfgGetExtendedErrorV0 function retrieves an extended error 
report of the reason for the last call release and location.
===================================================================*/
UINT32 CFW_CfgGetExtendedErrorV0 ( PSTR* pErrReport)
{
#if 0
    UINT32 ret = ERR_SUCCESS;

    #ifndef MF32_BASE_REG_DISABLE
    *pErrReport = CEER_INFO_TABlE[gLastErrorNo];
    #endif 
    
    return ret;
#endif
	return ERR_CFW_NOT_SUPPORT;
}


UINT32 CFW_CfgSetExtendedErrorV0 ( UINT32 nErrorCode)
{
#if 0
    UINT32 ret = ERR_SUCCESS;
    
    if(nErrorCode > CEER_INFO_TABlE_SIZE - 1)
        return ERR_CFG_PARAM_OUT_RANGR;
               
    gLastErrorNo = nErrorCode;   
    return ret;
#endif
	return ERR_CFW_NOT_SUPPORT;
}

UINT32 CFW_CfgGetExtendedErrorV1 (
  CFW_EXTENDED_ERROR* pExtendedError
)
{
	UINT32 ret = ERR_CFW_NOT_SUPPORT;

	return ret;
}

UINT32 CFW_CfgGetErrorReport(UINT8 *nLevel)
{
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetErrorReport, *nLevel = 0x%x\n", *nLevel);
    *nLevel = MemoryInfo.ErrorReport;
    return ERR_SUCCESS;
	//return ERR_CFW_NOT_SUPPORT;
}

/*==========================================================================
NAME    FUNCTION                PARAMETER   RANGE  DEFAULT
AT+CMEE	CFW_CfgSetErrorReport	nLevel	    0-2	   0

==========================================================================*/
UINT32 CFW_CfgSetErrorReport(UINT8 nLevel)
{
    UINT32 ret = ERR_SUCCESS;
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetErrorReport, nLevel = 0x%x\n", nLevel);
    /*Check the range*/
    //ret = CFW_CfgNumRangeCheck(nLevel, AT_CMEE_RANGE);
    //if(ret != ERR_SUCCESS)
    //    return ret;
    if(nLevel > 2)
	return ERR_CFG_PARAM_OUT_RANGR;

    MemoryInfo.ErrorReport = nLevel;

    return ret;
}

/*==============================================================================
function name:CFW_CfgSetSoftReset
cause[out] This parameter can be stored one of the following value:
0:normal reset
1:soft reset
===============================================================================*/
UINT32 CFW_CfgSetSoftReset(UINT8 cause)
{
	UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER( CFW_CfgSetSoftReset);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSoftReset, cause=0x%x\n",cause);

	ret = CFW_CfgNumRangeCheck(cause, SOFT_RESETNUM_RANGE);
	if (ERR_SUCCESS == ret)
	{
	    ret = cfg_write_8("sim", "SoftReset", cause);
	}
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetSoftReset exit\n");
	CSW_PROFILE_FUNCTION_EXIT( CFW_CfgSetSoftReset);
	return ret;
}

UINT32 CFW_CfgGetSoftReset(UINT8 *cause)
{
    UINT32 ret    = ERR_SUCCESS;
    BOOL bDefault = FALSE;
  
    CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetSoftReset);
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSoftReset exter\n");
	
    ret = cfg_read_8("sim", "SoftReset", cause);
    if (ERR_SUCCESS != ret)
    {
	CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret=0x%x\n",ret);
        ret = cfg_readdefault("sim", "SoftReset", cause);
        if (ERR_SUCCESS != ret)
        {
		CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSoftReset);
		return ret;
        }
        bDefault = TRUE;
    }
    else
    {
        ret = CFW_CfgNumRangeCheck(*cause, SOFT_RESETNUM_RANGE);
        if (ERR_SUCCESS != ret)
        {
		CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret=0x%x\n",ret);
              ret = cfg_readdefault("sim", "SoftReset", cause);
              if (ERR_SUCCESS != ret)
              {
			CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSoftReset);
			return ret;
              }
              bDefault = TRUE;
        }
    }
  
    if (bDefault)
    {
        cfg_write_8("sim", "SoftReset", *cause);
    }
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetSoftReset , *cause = 0x%x\n", cause);
    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSoftReset);
    return ERR_SUCCESS;

}



/*=========================================================================
AT capability
=========================================================================*/
#define AT_FULL        0
#define AT_LIMITED     1
#define AT_DISABLE     2

/*==========================================================================
AT APP only obtains the AT capability level. 
That info is enough for APP.
==========================================================================*/
UINT32 CFW_GetAtCapability(UINT8 *nLevel)
{
   // UINT32 ret = ERR_SUCCESS;
   //*nLevel = AT_FULL;
	return ERR_CFW_NOT_SUPPORT;
}

/*==============================================================================
function name:CFW_CfgSetFirstPowerOnFlag
cause[in] This parameter can be stored one of the following value:
1: First Power on
0: Not First Power on
===============================================================================*/
UINT32 CFW_CfgSetFirstPowerOnFlag(BOOL bFPO)
{
	UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER( CFW_CfgSetFirstPowerOnFlag);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetFirstPowerOnFlag, bFPO[0x%x]\n",bFPO);

	ret = CFW_CfgNumRangeCheck( bFPO, IS_FIRST_POWER_ON_RANGE );
	if (ERR_SUCCESS == ret)
	{
	    ret = cfg_write_8("sim", "IsFirstPowerOn", bFPO);
	}
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetFirstPowerOnFlag exit\n");
	CSW_PROFILE_FUNCTION_EXIT( CFW_CfgSetFirstPowerOnFlag);
	return ret;
}

/*==============================================================================
function name:CFW_CfgCheckFirstPowerOnFlag
cause[out] This parameter can be stored one of the following value:
1: First Power on
0: Not First Power on
===============================================================================*/

UINT32 CFW_CfgCheckFirstPowerOnFlag(BOOL *pbFPO)
{
    UINT32 ret    = ERR_SUCCESS;
    BOOL bDefault = FALSE;
  
    CSW_PROFILE_FUNCTION_ENTER(CFW_CfgCheckFirstPowerOnFlag);
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgCheckFirstPowerOnFlag exter\n");
	
    ret = cfg_read_8("sim", "IsFirstPowerOn", pbFPO);
    if (ERR_SUCCESS != ret)
    {
      	CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, error ret=0x%x\n",ret);
        ret = cfg_readdefault("sim", "IsFirstPowerOn", pbFPO);
        if (ERR_SUCCESS != ret)
        {
      		CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
      		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgCheckFirstPowerOnFlag);
      		return ret;
        }
        bDefault = TRUE;
    }
    else
    {
        ret = CFW_CfgNumRangeCheck(*pbFPO, IS_FIRST_POWER_ON_RANGE);
        if (ERR_SUCCESS != ret)
        {
          CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret=0x%x\n",ret);
          ret = cfg_readdefault("sim", "IsFirstPowerOn", pbFPO);
          if (ERR_SUCCESS != ret)
          {
            CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
            CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSoftReset);
            return ret;
          }
          bDefault = TRUE;
        }
    }
  
    if (bDefault)
    {
        cfg_write_8("sim", "IsFirstPowerOn", *pbFPO);
    }
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgCheckFirstPowerOnFlag , *cause = 0x%x\n", pbFPO);
    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetSoftReset);
    return ERR_SUCCESS;

}



#ifdef CFW_MULTI_SIM
UINT32 CFW_CfgGetIMSI(UINT8 * pIMSI, CFW_SIM_ID nSimID)
{
    UINT32 ret = ERR_SUCCESS;
    CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetIMSI);
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetIMSI, nSimID=0x%x\n", nSimID);
    if(pIMSI != NULL)
    {

	 INT8 simid_c[2]  = {0x30,0x00};
	INT8 name_s[11] = {0x00,};
	strcpy(name_s,"IMSI_SIM");
	
	if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
	{
		simid_c[0] = simid_c[0] + nSimID;
		strcat(name_s, simid_c);
	       ret = cfg_read_hex("general", name_s, pIMSI,10);
	}else{
	       ret = ERR_CFW_INVALID_PARAMETER;
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
		return ret;
	}

	if(ERR_SUCCESS != ret)
	{
	        CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret=0x%x\n",ret);
	        ret = cfg_readdefault("general", name_s, pIMSI);
	        if(ERR_SUCCESS != ret)
	        {
		        CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
		        CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetIMSI);
		        return ret;
	        }
	}
   
    }

    CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetIMSI,2, pImsi=%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x\n", 
		pIMSI[0], pIMSI[1], pIMSI[2], pIMSI[3], pIMSI[4], pIMSI[5], pIMSI[6], pIMSI[7], pIMSI[8], pIMSI[9]);
    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetIMSI);
    return ret;	
}


UINT32 CFW_CfgSetIMSI(UINT8 * pIMSI, CFW_SIM_ID nSimID)
{
    UINT32 ret = ERR_SUCCESS;
    CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetIMSI);
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetIMSI, nSimID=0x%x\n", nSimID);
    if(pIMSI != NULL)
    {
      INT8 simid_c[2]  = {0x30,0x00};
	INT8 name_s[11] = {0x00,};
	strcpy(name_s,"IMSI_SIM");
	
	if((CFW_SIM_0<=nSimID)&&(nSimID<CFW_SIM_COUNT))
	{
		simid_c[0] = simid_c[0] + nSimID;
		strcat(name_s, simid_c);
		 ret = cfg_write_hex("general", name_s, pIMSI, 10);

	}else{
	       ret = ERR_CFW_INVALID_PARAMETER;
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetCallWaiting);
		return ret;
	}


	
        if(ERR_SUCCESS != ret)
        {
	        CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_hex, error ret=0x%x\n",ret);
	        CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetIMSI);
	        return ret;
        }
    }

    CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgSetIMSI,2, pImsi=%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x\n", 
		pIMSI[0], pIMSI[1], pIMSI[2], pIMSI[3], pIMSI[4], pIMSI[5], pIMSI[6], pIMSI[7], pIMSI[8], pIMSI[9]);
    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetIMSI);
    return ret;	
}

UINT16   IND_EVENT_general[CFW_SIM_COUNT] = {0xff,0xff};
/*==========================================================================
The CFW_CfgSetIndicatorEvent function set the specified
indicator event to report.
==========================================================================*/
UINT32 CFW_CfgSetIndicatorEvent ( UINT16 n , CFW_SIM_ID nSimID)
{	   
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetIndicatorEvent);
       CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetIndicatorEvent, n=0x%x,nSimID=0x%x\n", n,nSimID);

	if(nSimID >= CFW_SIM_COUNT)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetIndicatorEvent);
		return ERR_CFW_INVALID_PARAMETER;
	}
	
	IND_EVENT_general[nSimID] = n;
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetIndicatorEvent exit\n");
 	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetIndicatorEvent);
	return ERR_SUCCESS;
}


UINT32 CFW_CfgGetIndicatorEvent ( UINT16* pind, CFW_SIM_ID nSimID)
{
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetIndicatorEvent);
       CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetIndicatorEvent, nSimID=0x%x\n", nSimID);
	if(nSimID >= CFW_SIM_COUNT)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetIndicatorEvent);
		return ERR_CFW_INVALID_PARAMETER;
	}

	
	* pind = IND_EVENT_general[nSimID];
       CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetIndicatorEvent, * pind =0x%x\n", * pind);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetIndicatorEvent);
	return ERR_SUCCESS;
}

/*==========================================================================
typedef struct _CFW_IND_EVENT_INFO { 
  UINT32 bc:3;
  UINT32 sq:3;
  UINT32 nw_srv:1;
  UINT32 sms_receive:1;
  UINT32 call:1;
  UINT32 roam:1;
  UINT32 sms_full:1;
  UINT32 sounder :1;
  UINT32 bit_err:8;
} CFW_IND_EVENT_INFO;

==========================================================================*/
UINT32 CFW_CfgGetIndicatorEventInfo (CFW_IND_EVENT_INFO* pIndInfo, CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetIndicatorEventInfo);
       CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetIndicatorEventInfo, nSimID=0x%x\n", nSimID);
	ret = CFW_CheckSimId(nSimID);
	if (ERR_SUCCESS != ret)
	{
	       CSW_TRACE(CFW_CFG_TS_ID, "CFW_CheckSimId, ret=0x%x\n", ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetIndicatorEventInfo);
		return ret;
	}
	pIndInfo->bc          = IndEventInfo[nSimID].bc;
	pIndInfo->call        = IndEventInfo[nSimID].call;
	pIndInfo->nw_srv      = IndEventInfo[nSimID].nw_srv;
	pIndInfo->roam        = IndEventInfo[nSimID].roam;
	pIndInfo->sms_full    = IndEventInfo[nSimID].sms_full;
	pIndInfo->sms_receive = IndEventInfo[nSimID].sms_receive;
	pIndInfo->sq          = IndEventInfo[nSimID].sq;
	pIndInfo->bit_err     = IndEventInfo[nSimID].bit_err;
	pIndInfo->sounder     = IndEventInfo[nSimID].sounder;

       CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetIndicatorEventInfo exit\n");
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetIndicatorEventInfo);
	return ret;
}


UINT32 CFW_CfgSetIndicatorEventInfo ( CFW_IND_EVENT_INFO* pIndInfo, CFW_SIM_ID nSimID )
{
	UINT32 ret = ERR_SUCCESS;
	UINT16  param = 0;
	BOOL bErr = FALSE;
	
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetIndicatorEventInfo);
       CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetIndicatorEventInfo, nSimID=0x%x\n", nSimID);
	ret = CFW_CfgNumRangeCheck(pIndInfo->bc, GE_EV_INFO_BC_RANGE);
	if(ERR_SUCCESS != ret)
	{
		bErr = TRUE;
		CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck error ret=0x%x, pIndInfo->bc = 0x%x\n",ret, pIndInfo->bc);
	}
	
	if(!bErr)
	{
		ret = CFW_CfgNumRangeCheck(pIndInfo->sq, GE_EV_INFO_BC_RANGE);
		if(ERR_SUCCESS != ret)
		{
			bErr = TRUE;
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck error ret=0x%x, pIndInfo->sq = 0x%x\n",ret, pIndInfo->sq);
		}
	}
	
	if(!bErr)
	{
		ret = CFW_CfgNumRangeCheck(pIndInfo->nw_srv , GE_EV_INFO_NWSRV_RANGE);
		if(ERR_SUCCESS != ret)
		{
			bErr = TRUE;
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck error ret=0x%x, pIndInfo->nw_srv = 0x%x\n",ret, pIndInfo->nw_srv);
		}
	}
	
	if(!bErr)
	{
		ret = CFW_CfgNumRangeCheck(pIndInfo->sms_receive , GE_EV_INFO_NWSRV_RANGE);
		if(ERR_SUCCESS != ret)
		{
			bErr = TRUE;
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck error ret=0x%x, pIndInfo->sms_receive = 0x%x\n",ret, pIndInfo->sms_receive);
		}
	}

	if(!bErr)
	{
		ret = CFW_CfgNumRangeCheck(pIndInfo->call , GE_EV_INFO_NWSRV_RANGE);
		if(ERR_SUCCESS != ret)
		{
			bErr = TRUE;
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck error ret=0x%x, pIndInfo->call = 0x%x\n",ret, pIndInfo->call);
		}
	}

	if(!bErr)
	{
		ret = CFW_CfgNumRangeCheck(pIndInfo->roam  , GE_EV_INFO_NWSRV_RANGE);
		if(ERR_SUCCESS != ret)
		{
			bErr = TRUE;
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck error ret=0x%x, pIndInfo->roam = 0x%x\n",ret, pIndInfo->roam);
		}
	}
	
	if(!bErr)
	{
		ret = CFW_CfgNumRangeCheck(pIndInfo->sms_full , GE_EV_INFO_NWSRV_RANGE);
		if(ERR_SUCCESS != ret)
		{
			bErr = TRUE;
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck error ret=0x%x, pIndInfo->sms_full = 0x%x\n",ret, pIndInfo->sms_full);
		}
	}

	if(!bErr)
	{
		ret = CFW_CfgNumRangeCheck(pIndInfo->sounder  , GE_EV_INFO_NWSRV_RANGE);
		if(ERR_SUCCESS != ret)
		{
			bErr = TRUE;
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck error ret=0x%x, pIndInfo->sounder = 0x%x\n",ret, pIndInfo->sounder);
		}
	} 

	if(!bErr)
	{
		ret = CFW_CfgNumRangeCheck(pIndInfo->bit_err , GE_EV_INFO_NWBITER_RANGE);
		if(ERR_SUCCESS != ret)
		{
			bErr = TRUE;
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck error ret=0x%x, pIndInfo->bit_err = 0x%x\n",ret, pIndInfo->bit_err);
		}
	}
	if(bErr)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetIndicatorEventInfo);
		return ret;
	}
	//param check end

	ret = CFW_CfgGetIndicatorEvent(&param, nSimID);

	if(ERR_SUCCESS != ret)
	{
	       CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetIndicatorEvent, error ret =0x%x\n",ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetIndicatorEventInfo);
		return ret;
	}
	
    if( (param & CFW_CFG_IND_BC_EVENT) == CFW_CFG_IND_BC_EVENT )
        IndEventInfo[nSimID].bc = pIndInfo->bc;
    
    if( ( param & CFW_CFG_IND_SQ_EVENT ) == CFW_CFG_IND_SQ_EVENT )
        IndEventInfo[nSimID].sq = pIndInfo->sq;
    
    if( ( param & CFW_CFG_IND_NW_EVENT ) == CFW_CFG_IND_NW_EVENT)
        IndEventInfo[nSimID].nw_srv = pIndInfo->nw_srv;
    
    if( ( param & CFW_CFG_IND_SMS_RECEIVE_EVENT ) == CFW_CFG_IND_SMS_RECEIVE_EVENT )
        IndEventInfo[nSimID].sms_receive = pIndInfo->sms_receive;

    if( ( param & CFW_CFG_IND_CALL_EVENT ) == CFW_CFG_IND_CALL_EVENT )
        IndEventInfo[nSimID].call = pIndInfo->call;

    if( ( param & CFW_CFG_IND_ROAM_EVENT ) == CFW_CFG_IND_ROAM_EVENT )
        IndEventInfo[nSimID].roam = pIndInfo->roam;

    if( ( param & CFW_CFG_IND_SMS_FULL_EVENT ) == CFW_CFG_IND_SMS_FULL_EVENT )
        IndEventInfo[nSimID].sms_full = pIndInfo->sms_full;

    if( ( param & CFW_CFG_IND_SOUNDER_EVENT ) == CFW_CFG_IND_SOUNDER_EVENT )
        IndEventInfo[nSimID].sounder = pIndInfo->sounder;
    
    if( ( param & CFW_CFG_IND_BIT_ERR_EVENT ) == CFW_CFG_IND_BIT_ERR_EVENT )
        IndEventInfo[nSimID].bit_err = pIndInfo->bit_err;

    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetIndicatorEventInfo exit\n");
    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetIndicatorEventInfo);
    return ret;
}


#else

UINT32 CFW_CfgGetIMSI(UINT8 * pIMSI)
{
    UINT32 ret = ERR_SUCCESS;
    CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetIMSI);
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetIMSI enter\n");
    if(pIMSI != NULL)
    {
        ret = cfg_read_hex("general", "IMSI", pIMSI, 10);
        if(ERR_SUCCESS != ret)
        {
	        CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret=0x%x\n",ret);
	        ret = cfg_readdefault("general", "IMSI", pIMSI);
	        if(ERR_SUCCESS != ret)
	        {
		        CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret=0x%x\n",ret);
		        CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetIMSI);
		        return ret;
	        }
        }
    }

    CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgGetIMSI,2, pImsi=%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x\n", 
		pIMSI[0], pIMSI[1], pIMSI[2], pIMSI[3], pIMSI[4], pIMSI[5], pIMSI[6], pIMSI[7], pIMSI[8], pIMSI[9]);
    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetIMSI);
    return ret;	
}


UINT32 CFW_CfgSetIMSI(UINT8 * pIMSI)
{
    UINT32 ret = ERR_SUCCESS;
    CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetIMSI);
    CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetIMSI enter\n");
    if(pIMSI != NULL)
    {
        ret = cfg_write_hex("general", "IMSI", pIMSI, 10);
        if(ERR_SUCCESS != ret)
        {
	        CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_hex, error ret=0x%x\n",ret);
	        CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetIMSI);
	        return ret;
        }
    }

    CSW_TRACE(CFW_CFG_TS_ID, "in CFW_CfgSetIMSI,2, pImsi=%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x\n", 
		pIMSI[0], pIMSI[1], pIMSI[2], pIMSI[3], pIMSI[4], pIMSI[5], pIMSI[6], pIMSI[7], pIMSI[8], pIMSI[9]);
    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetIMSI);
    return ret;	
}

UINT16   IND_EVENT_general;
/*==========================================================================
The CFW_CfgSetIndicatorEvent function set the specified
indicator event to report.
==========================================================================*/
UINT32 CFW_CfgSetIndicatorEvent ( UINT16 n )
{
    UINT32 ret = ERR_SUCCESS;
   /* 
    UINT16 ind = 0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetIndicatorEvent);
    ret = CFW_CfgGetIndicatorEvent(&ind);
    if(ERR_SUCCESS != ret)
    {
	    return ret;
    }

    ind |= n;  // Set to 1 (bit)
	IND_EVENT_general = ind;
	ret = cfg_write_hex("general", "IND_EVENT", &ind, 2);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetIndicatorEvent);
	*/
    return ret;
}


UINT32 CFW_CfgGetIndicatorEvent ( UINT16* pind )
{
/*
    UINT32 ret = ERR_SUCCESS, param=0;
    ret = cfg_read_hex("general", "IND_EVENT", &param, 2);
    if(ERR_SUCCESS != ret)
    {
	    ret = cfg_readdefault("general", "IND_EVENT", pind);
	    if(ERR_SUCCESS != ret)
	    {
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetIndicatorEvent);
		    return ret;
	    }
    }
    else
    {
	    *pind = (UINT16)param;
    }
*/
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetIndicatorEvent);
	* pind = IND_EVENT_general;
	
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetIndicatorEvent);
    return ERR_SUCCESS;
}

/*==========================================================================
typedef struct _CFW_IND_EVENT_INFO { 
  UINT32 bc:3;
  UINT32 sq:3;
  UINT32 nw_srv:1;
  UINT32 sms_receive:1;
  UINT32 call:1;
  UINT32 roam:1;
  UINT32 sms_full:1;
  UINT32 sounder :1;
  UINT32 bit_err:8;
} CFW_IND_EVENT_INFO;

==========================================================================*/
UINT32 CFW_CfgGetIndicatorEventInfo (CFW_IND_EVENT_INFO* pIndInfo)
{
	UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetIndicatorEventInfo);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetIndicatorEventInfo enter\n");
	pIndInfo->bc          = IndEventInfo.bc;
	pIndInfo->call        = IndEventInfo.call;
	pIndInfo->nw_srv      = IndEventInfo.nw_srv;
	pIndInfo->roam        = IndEventInfo.roam;
	pIndInfo->sms_full    = IndEventInfo.sms_full;
	pIndInfo->sms_receive = IndEventInfo.sms_receive;
	pIndInfo->sq          = IndEventInfo.sq;
	pIndInfo->bit_err     = IndEventInfo.bit_err;
	pIndInfo->sounder     = IndEventInfo.sounder;
	
       CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetIndicatorEventInfo exit\n");
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetIndicatorEventInfo);
	return ret;
}

UINT32 CFW_CfgSetIndicatorEventInfo ( CFW_IND_EVENT_INFO* pIndInfo )
{
	UINT32 ret = ERR_SUCCESS;
	UINT16  param = 0;
	BOOL bErr = FALSE;
	
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetIndicatorEventInfo);
       CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetIndicatorEventInfo enter\n");
	ret = CFW_CfgNumRangeCheck(pIndInfo->bc, GE_EV_INFO_BC_RANGE);
	if(ERR_SUCCESS != ret)
	{
		bErr = TRUE;
		CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck error ret=0x%x, pIndInfo->bc = 0x%x\n",ret, pIndInfo->bc);
	}
	
	if(!bErr)
	{
		ret = CFW_CfgNumRangeCheck(pIndInfo->sq, GE_EV_INFO_BC_RANGE);
		if(ERR_SUCCESS != ret)
		{
			bErr = TRUE;
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck error ret=0x%x, pIndInfo->sq = 0x%x\n",ret, pIndInfo->sq);
		}
	}
	
	if(!bErr)
	{
		ret = CFW_CfgNumRangeCheck(pIndInfo->nw_srv , GE_EV_INFO_NWSRV_RANGE);
		if(ERR_SUCCESS != ret)
		{
			bErr = TRUE;
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck error ret=0x%x, pIndInfo->nw_srv = 0x%x\n",ret, pIndInfo->nw_srv);
		}
	}
	
	if(!bErr)
	{
		ret = CFW_CfgNumRangeCheck(pIndInfo->sms_receive , GE_EV_INFO_NWSRV_RANGE);
		if(ERR_SUCCESS != ret)
		{
			bErr = TRUE;
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck error ret=0x%x, pIndInfo->sms_receive = 0x%x\n",ret, pIndInfo->sms_receive);
		}
	}

	if(!bErr)
	{
		ret = CFW_CfgNumRangeCheck(pIndInfo->call , GE_EV_INFO_NWSRV_RANGE);
		if(ERR_SUCCESS != ret)
		{
			bErr = TRUE;
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck error ret=0x%x, pIndInfo->call = 0x%x\n",ret, pIndInfo->call);
		}
	}

	if(!bErr)
	{
		ret = CFW_CfgNumRangeCheck(pIndInfo->roam  , GE_EV_INFO_NWSRV_RANGE);
		if(ERR_SUCCESS != ret)
		{
			bErr = TRUE;
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck error ret=0x%x, pIndInfo->roam = 0x%x\n",ret, pIndInfo->roam);
		}
	}
	
	if(!bErr)
	{
		ret = CFW_CfgNumRangeCheck(pIndInfo->sms_full , GE_EV_INFO_NWSRV_RANGE);
		if(ERR_SUCCESS != ret)
		{
			bErr = TRUE;
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck error ret=0x%x, pIndInfo->sms_full = 0x%x\n",ret, pIndInfo->sms_full);
		}
	}

	if(!bErr)
	{
		ret = CFW_CfgNumRangeCheck(pIndInfo->sounder  , GE_EV_INFO_NWSRV_RANGE);
		if(ERR_SUCCESS != ret)
		{
			bErr = TRUE;
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck error ret=0x%x, pIndInfo->sounder = 0x%x\n",ret, pIndInfo->sounder);
		}
	} 

	if(!bErr)
	{
		ret = CFW_CfgNumRangeCheck(pIndInfo->bit_err , GE_EV_INFO_NWBITER_RANGE);
		if(ERR_SUCCESS != ret)
		{
			bErr = TRUE;
			CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck error ret=0x%x, pIndInfo->bit_err = 0x%x\n",ret, pIndInfo->bit_err);
		}
	}
	if(bErr)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetIndicatorEventInfo);
		return ret;
	}
	//param check end

	ret = CFW_CfgGetIndicatorEvent(&param);

	if(ERR_SUCCESS != ret)
	{
	       CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetIndicatorEvent, error ret =0x%x\n",ret);
		CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetIndicatorEventInfo);
		return ret;
	}
	
    if( (param & CFW_CFG_IND_BC_EVENT) == CFW_CFG_IND_BC_EVENT )
        IndEventInfo.bc = pIndInfo->bc;
    
    if( ( param & CFW_CFG_IND_SQ_EVENT ) == CFW_CFG_IND_SQ_EVENT )
        IndEventInfo.sq = pIndInfo->sq;
    
    if( ( param & CFW_CFG_IND_NW_EVENT ) == CFW_CFG_IND_NW_EVENT)
        IndEventInfo.nw_srv = pIndInfo->nw_srv;
    
    if( ( param & CFW_CFG_IND_SMS_RECEIVE_EVENT ) == CFW_CFG_IND_SMS_RECEIVE_EVENT )
        IndEventInfo.sms_receive = pIndInfo->sms_receive;

    if( ( param & CFW_CFG_IND_CALL_EVENT ) == CFW_CFG_IND_CALL_EVENT )
        IndEventInfo.call = pIndInfo->call;

    if( ( param & CFW_CFG_IND_ROAM_EVENT ) == CFW_CFG_IND_ROAM_EVENT )
        IndEventInfo.roam = pIndInfo->roam;

    if( ( param & CFW_CFG_IND_SMS_FULL_EVENT ) == CFW_CFG_IND_SMS_FULL_EVENT )
        IndEventInfo.sms_full = pIndInfo->sms_full;

    if( ( param & CFW_CFG_IND_SOUNDER_EVENT ) == CFW_CFG_IND_SOUNDER_EVENT )
        IndEventInfo.sounder = pIndInfo->sounder;
    
    if( ( param & CFW_CFG_IND_BIT_ERR_EVENT ) == CFW_CFG_IND_BIT_ERR_EVENT )
        IndEventInfo.bit_err = pIndInfo->bit_err;
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetIndicatorEventInfo);
    return ret;
}


#endif

