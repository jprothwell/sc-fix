///*************************************************************
//hameina 2006.8.21 modified
//
//notice: this version is based on reg systerm
//
//modify record:
//   2006.9.21 hameina [mod] 9 interfaces of REG, REG modified it's interface.
//   2006.9.22 hameina [mod] reg getvalue() should be called only once when read hex&str
//                         getvalue() the type of param datasize changed:UINT32->UINT8
//  2006.9.29 hameina [mod] cfg_readdefault() the type of function changed:bool->UINT32
//  2006.11.16 hameina[mod] REG_CloseKey()sometime returns err, so add a output trace sentence
///**************************************************************

#include "cfw_cfg.h"
#include "sul.h"
#include "csw_debug.h"

#define PUBLIC_DEFAULT_VALUE         0x0;
#define PHBK_DEFAULT_VALUE           0x534D;
#define NW_STATUS_DEFAULT_VALUE      0x1
#define ERR_REPORT_DEFAULT_VALUE     0x0
#define CHAR_SET_DEFAULT_VALUE       0x0
#define SMS_MODE_DEFAULT_VALUE       0x1A72300
#define SMS_STORE_DEFAULT_VALUE      0x534D

#define SMS_CENTER_1_DEFAULT_VALUE   0x0 
#define SMS_CENTER_2_DEFAULT_VALUE   0x0 
#define SMS_CENTER_3_DEFAULT_VALUE   0x0 
#define SMS_CENTER_4_DEFAULT_VALUE   0x0 
#define SMS_CENTER_5_DEFAULT_VALUE   0x0 
#define SMS_CENTER_6_DEFAULT_VALUE   0x0 

#define CFD_NUMBER_1_DEFAULT_VALUE   0x0
#define CFD_NUMBER_2_DEFAULT_VALUE   0x0
#define CFD_NUMBER_3_DEFAULT_VALUE   0x0
#define CFD_NUMBER_4_DEFAULT_VALUE   0x0
#define CFD_NUMBER_5_DEFAULT_VALUE   0x0
#define CFD_NUMBER_6_DEFAULT_VALUE   0x0
#define CFD_OTHERS_DEFAULT_VALUE     0x0
#define CFD_STATUS_DEFAULT_VALUE     0x0

#define FUN_LEVEL_DEFAULT_VALUE      0x1

#define CALL_WAIT_DEFAULT_VALUE      0x0
#define RTC_DEFAULT_VALUE            0x0
#define DISP_FORMAT_DEFAULT_VALUE    0x0

const  OPER_INFO OperatorInfo[] = {
    {{0x04, 0x06, 0x00, 0x00, 0x00, 0x0f}, "China Mobile"},
    {{0x04, 0x06, 0x00, 0x00, 0x01, 0x0f}, "China Unicom"},
    
    {{0x05, 0x00, 0x05, 0x00, 0x01, 0x0f}, "Telstra Corporation Ltd."},
    {{0x05, 0x00, 0x05, 0x00, 0x02, 0x0f}, "Optus Mobile Pty. Ltd."},
    {{0x05, 0x00, 0x05, 0x00, 0x03, 0x0f}, "Vodafone Network Pty. Ltd."},
    {{0x05, 0x00, 0x05, 0x00, 0x04, 0x0f}, "Department of Defence"},
    {{0x05, 0x00, 0x05, 0x00, 0x05, 0x0f}, "The Ozitel Network Pty. Ltd."},
    {{0x05, 0x00, 0x05, 0x00, 0x06, 0x0f}, "Hutchison 3G Australia Pty. Ltd."},
    {{0x05, 0x00, 0x05, 0x00, 0x07, 0x0f}, "Vodafone Network Pty. Ltd."},
    {{0x05, 0x00, 0x05, 0x00, 0x08, 0x0f}, "One.Tel GSM 1800 Pty. Ltd."},
    {{0x05, 0x00, 0x05, 0x00, 0x09, 0x0f}, "Airnet Commercial Australia Ltd."},
    {{0x05, 0x00, 0x05, 0x01, 0x01, 0x0f}, "Telstra Corporation Ltd."},
    {{0x05, 0x00, 0x05, 0x01, 0x02, 0x0f}, "Hutchison Telecommunications (Australia) Pty. Ltd."},
    {{0x05, 0x00, 0x05, 0x01, 0x04, 0x0f}, "AAPT Ltd."},
    {{0x05, 0x00, 0x05, 0x01, 0x05, 0x0f}, "3GIS Pty Ltd. (Telstra & Hutchison 3G)"},
    {{0x05, 0x00, 0x05, 0x02, 0x04, 0x0f}, "Advanced Communications Technologies Pty. Ltd."},
    {{0x05, 0x00, 0x05, 0x07, 0x01, 0x0f}, "Telstra Corporation Ltd."},
    {{0x05, 0x00, 0x05, 0x07, 0x02, 0x0f}, "Telstra Corporation Ltd."},
    {{0x05, 0x00, 0x05, 0x08, 0x08, 0x0f}, "Localstar Holding Pty. Ltd."},
    {{0x05, 0x00, 0x05, 0x09, 0x00, 0x0f}, "Optus Ltd."},
    {{0x05, 0x00, 0x05, 0x09, 0x09, 0x0f}, "One.Tel GSM 1800 Pty. Ltd."},

    {{0x03, 0x01, 0x00, 0x00, 0x01, 0x00}, "MCI"},
    {{0x03, 0x01, 0x00, 0x00, 0x01, 0x02}, "Verizon Wireless"},
    {{0x03, 0x01, 0x00, 0x00, 0x01, 0x03}, "Mobile Tel Inc."},
    {{0x03, 0x01, 0x00, 0x00, 0x01, 0x04}, "Testing"},
    {{0x03, 0x01, 0x00, 0x00, 0x01, 0x06}, "Cricket Communications"},
    {{0x03, 0x01, 0x00, 0x00, 0x01, 0x07}, "North Sight Communications Inc."},
    {{0x03, 0x01, 0x00, 0x00, 0x02, 0x00}, "Union Telephone Company"},
    {{0x03, 0x01, 0x00, 0x00, 0x03, 0x00}, "Centennial Communications"},
    {{0x03, 0x01, 0x00, 0x00, 0x03, 0x02}, "IT&E Overseas, Inc."},
    {{0x03, 0x01, 0x00, 0x00, 0x03, 0x03}, "Guam Telephone Authority"},
    {{0x03, 0x01, 0x00, 0x00, 0x03, 0x04}, "Nevada Wireless LLC"},

    {{0,},0}                                  
                                  };
 MEM_INFO_STORAGE MemoryInfo = {1, };

#ifdef CFW_MULTI_SIM
 CFW_IND_EVENT_INFO   IndEventInfo[CFW_SIM_COUNT];
#else
CFW_IND_EVENT_INFO   IndEventInfo;
#endif

 UINT32 gLastErrorNo = 0;


/*==========================================================================
Static internal function.
==========================================================================*/
static UINT32 CFW_Cfg_atoi( UINT32 *pVal, UINT8 *s)
{
	UINT32 err = 0;
	UINT8 c;
	UINT32 val = 0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_Cfg_atoi);
	while ( (c = *s++) != 0x00)
	{
        if (c >= '0' && c <= '9') 
        {
            c -= '0';
	    }
	    else
	    {
            err = 0; 
            break;
	    }
	    
        err = 1;     
        val = (val * 10 + c);
	}
    *pVal = val;
	CSW_PROFILE_FUNCTION_EXIT(CFW_Cfg_atoi);
    return err;
} 


/*=========================================================================


=========================================================================*/
static UINT32 GetNumber(UINT32 *pValue, UINT8 **pStr)
{        
    UINT8 *ptmp = 0;
    UINT8 *pBegin = 0;
    UINT8 len = 0;
    UINT8 strNum[10];
    UINT32 ret = 0;
	CSW_PROFILE_FUNCTION_ENTER(GetNumber);
    SUL_ZeroMemory8(strNum, 10);
    
    ptmp = (*pStr);
    pBegin = ptmp;
    while((*ptmp != ',') && (*ptmp != '-') && (*ptmp != 0))
    {    
        ptmp++;
        len++;
        (*pStr)++;
    }
    SUL_MemCopy8(strNum, pBegin, len);
    ret = CFW_Cfg_atoi (pValue, strNum);
	CSW_PROFILE_FUNCTION_EXIT(GetNumber);
    if (ret > 0)
        return 1;
    
    return 0;

}

/*=========================================================================
nInput : input parameter.
pString: string table.

If the nInput is in the string table, this function will return ERR_SUCCESS
Otherwise, ERR_PARAM_TABLE_ILLEGAL(means the string table is illegal)
or ERR_PARAM_ILLEGAL(means it is not found in string table).

EXAMPLE:
#define PARAM_EXAMPLE_TABLE  "2,3,8-12,128,230" 

switch ( CFW_CfgNumRangeCheck(9, PARAM_EXAMPLE_TABLE))
{
    case ERR_SUCCESS:

        break;
    case ERR_PARAM_TABLE_ILLEGAL:

        break;
    case ERR_PARAM_ILLEGAL:

        break;
}
=========================================================================*/
UINT32 CFW_CfgNumRangeCheck(UINT32 nInput, UINT8 *pString)
{
    LIMIT  lim;
    UINT32 nValue = 0;
    UINT8 *ptmp = pString;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgNumRangeCheck);
    while(*ptmp != 0) 
    {
        if(GetNumber(&nValue, &ptmp) == 0)  //ptempµÄÊıÖµ
        {
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNumRangeCheck);        
            return ERR_PARAM_TABLE_ILLEGAL;
        }
		
        if(*ptmp == ',')
        {
               if(nValue == nInput)
	        {
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNumRangeCheck);        
                     return ERR_SUCCESS;
               }
            
               if(*ptmp == 0)
	        {
		      CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNumRangeCheck);        
                    return ERR_CFG_PARAM_OUT_RANGR;
		 }
               ptmp++;
        }
		
        else if(*ptmp == '-')
        {
            lim.min = nValue;
            ptmp++;
            
            if(GetNumber(&nValue, &ptmp) == 0)
            {
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNumRangeCheck);        
                return ERR_PARAM_TABLE_ILLEGAL;
            }
            lim.MAX = nValue;

            if((nInput >= lim.min) && (nInput <= lim.MAX))
	        {
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNumRangeCheck);        
                return ERR_SUCCESS;
            }

            if(*ptmp == 0)
	        {
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNumRangeCheck);        
                return ERR_CFG_PARAM_OUT_RANGR;
			}

            ptmp++;
                    
        }
        else if(*ptmp == 0)
        {
            if(nValue == nInput)
            {
				CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNumRangeCheck);        
                return ERR_SUCCESS;
            }
        }
    }
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgNumRangeCheck);        
    return ERR_CFG_PARAM_OUT_RANGR;
}


/*=================================================================


=================================================================*/
static void GetString(UINT8 *pOUtStr, UINT8 **pStr)
{        
    UINT8 *ptmp = 0;
    UINT8 *pBegin = 0;
    UINT8 len = 0;
	CSW_PROFILE_FUNCTION_ENTER(GetString);
    ptmp = (*pStr);
    pBegin = ptmp;
    while((*ptmp != ',') && (*ptmp != 0))
    {    
        ptmp++;
        len++;
        (*pStr)++;
    }
    SUL_MemCopy8(pOUtStr, pBegin, len);
    (*pStr)++;
	CSW_PROFILE_FUNCTION_EXIT(GetString);
}


/*=================================================================


=================================================================*/
INT32 CFW_Cfg_strcmp(const UINT8 *s1, const UINT8 *s2)
{
    UINT8     c1 = 0;
    UINT8     c2 = 0;
    INT32    ret = 0;
	CSW_PROFILE_FUNCTION_ENTER(CFW_Cfg_strcmp);
    if(!s1 || !s2)
    {
		CSW_PROFILE_FUNCTION_EXIT(CFW_Cfg_strcmp);
        return 1;
    }
    
    do
    {
        while ( (c1 = *s1++) == ' ');

        while ( (c2 = *s2++) == ' ');

        ret = c1 - c2;

    }
    while ( ret == 0 && c1 != '\0' && c2 != '\0' );
	CSW_PROFILE_FUNCTION_EXIT(CFW_Cfg_strcmp);
    return ret;
}



/*=================================================================
pInputStr : input parameter.
pString   : string table.

If the pInputStr is found in the string table, this function will return 
ERR_SUCCESS. Otherwise, return ERR_PARAM_ILLEGAL(means it is not found in 
string table).

EXAMPLE:
#define PARAM_EXAMPLE_TABLE  "qwe,abc,xyz,123,ioppp,e,er,8u,aaaaa" 

switch ( CFW_CfgStrRangeCheck("xyz", PARAM_EXAMPLE_TABLE) )
{
    case ERR_SUCCESS:

        break;
 
    case ERR_PARAM_ILLEGAL:

        break;
}


=================================================================*/
UINT32 CFW_CfgStrRangeCheck(UINT8 *pInputStr, UINT8 *pString)
{
    INT32 ret = 0;
    UINT8 *ptmp = pString;
    UINT8  pGetStr[10];
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgStrRangeCheck);
    SUL_ZeroMemory8(pGetStr,10);

    while(*ptmp != 0) 
    {
        SUL_ZeroMemory8(pGetStr,10);
        GetString(pGetStr, &ptmp);
        
        ret = CFW_Cfg_strcmp(pGetStr, pInputStr);
        if(ret == 0)
        {
			CSW_PROFILE_FUNCTION_EXIT(CFW_CfgStrRangeCheck);
            return ERR_SUCCESS;
        }
    }
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgStrRangeCheck);
    return ERR_CFG_PARAM_OUT_RANGR;
}




typedef struct _REGISTER_DATA_TYPE
{
 UINT8  *KEY_NAME;   
 UINT32 Value;   
} REGISTER_DATA_TYPE;

const REGISTER_DATA_TYPE gRegisterData[] = {
{"PUBLIC", 0},
{"CALL_WAIT", 0},
{"TONE_DURA", 0},
{"AUDIO_MODE", 0},
{"SPEAK_V", 0},
{"FUN_LEVEL", 0},
{"BearServType", 0},
{"Clip_Value", 0},
{"CMER_param", 0},
{"CNMI_param", 0},
{"ConnectResultFormatCode", 0},
{"ErrorReport", 0},
{"IncomingCallResultMode", 0},
{"NewSmsIndication", 0},
{"NwStatus", 0},
{"ResultCodeFormatMode", 0},
{"ResultCodePresentMode", 0},
{"RLPType_iws", 0},
{"RLPType_mws", 0},
{"RLPType_N2", 0},
{"RLPType_T1", 0},
{"SelectSmsService", 0},
{"ServiceReportingMode", 0},
{"CHAR_SET", 0},
{"IND_EVENT", 0},
{"REG_DISP_FORMAT", 0},
{"PH_BK", 0},
{"SMS_STORAGE_1", 0},
{"SMS_STORAGE_2", 0},
{"SMS_STORAGE_3", 0},
{"SMS_OVER_IND", 0},
{"SMS_SHOW_MODE_PARAM", 0},
{"SMS_CB_MODE", 0},
{"SMS_CB_mids_0", 0},
{"SMS_CB_mids_1", 0},
{"SMS_CB_mids_2", 0},
{"SMS_CB_mids_3", 0},
{"SMS_CB_mids_4", 0},
{"SMS_CB_mids_5", 0},
{"SMS_CB_dcss_0", 0},
{"SMS_CB_dcss_1", 0},
{"SMS_CB_dcss_2", 0},
{"SMS_CB_dcss_3", 0},
{"SMS_CB_dcss_4", 0},
{"SMS_CB_dcss_5", 0},
{"SMS_ACK", 0},
{"SMS_MODE", 0},
{"SMS_CENTER_1", 0},
{"SMS_CENTER_2", 0},
{"SMS_CENTER_3", 0},
{"SMS_CENTER_4", 0},
{"SMS_CENTER_5", 0},
{"SMS_CENTER_6", 0},
{"SimCardSwitch", 0},  
{0,0}
};


//[[hameina[+]2006.8.22
UINT32 cfg_read_8(UINT8 *uSubkey,UINT8 *name, UINT8 *n)
{
	HKEY hkey;
	UINT32 data;
	UINT8 type;
	UINT8 size=1;
	INT32 err_code = 0;
	CSW_PROFILE_FUNCTION_ENTER(cfg_read_8);
	CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_8, subkey=%s, value name=%s\n", uSubkey, name);
	err_code = REG_OpenKey(REG_HKEY_CURRENT_COMM, TEXT(uSubkey), &hkey);
    if(ERR_SUCCESS == err_code)
    {
	    err_code = REG_GetValue(hkey, TEXT(name), &type, &data, &size);
        if(ERR_SUCCESS == err_code)
        {
	        err_code = REG_CloseKey(hkey);			

	        *n = data;
		 CSW_PROFILE_FUNCTION_EXIT(cfg_read_8);
	        return ERR_SUCCESS;
        }
        else
        {    
			CSW_TRACE(CFW_CFG_TS_ID, "REG_GetValue , error err_code = 0x%x\n",err_code);
			err_code = REG_CloseKey(hkey);

			CSW_PROFILE_FUNCTION_EXIT(cfg_read_8);
			return ERR_CFG_WRITE_REG_FAILURE;
        }
    }
    else
    {
		CSW_TRACE(CFW_CFG_TS_ID, "REG_OpenKey, error err_code=0x%x\n",err_code);
		CSW_PROFILE_FUNCTION_EXIT(cfg_read_8);
		return ERR_CFG_WRITE_REG_FAILURE;
    }
}

UINT32 cfg_write_8(UINT8 *uSubkey,UINT8 *name, UINT8 n)
{
	HKEY hkey;
	INT32 err_code = 0;
	CSW_PROFILE_FUNCTION_ENTER(cfg_write_8);
	CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_8, subkey=%s, value name=%s, n = 0x%x\n", uSubkey, name, n);
	err_code = REG_OpenKey(REG_HKEY_CURRENT_COMM, TEXT(uSubkey), &hkey);
    if(ERR_SUCCESS == err_code)
    {
	    err_code = REG_SetValue(hkey, TEXT(name), REG_TYPE_INT8, &n, 1);
        if(ERR_SUCCESS == err_code)
        {
	        err_code = REG_CloseKey(hkey);
			CSW_PROFILE_FUNCTION_EXIT(cfg_write_8);
	        return ERR_SUCCESS;
        }
        else
        {
			CSW_TRACE(CFW_CFG_TS_ID, "REG_SetValue , error err_code = 0x%x\n",err_code);
	        err_code = REG_CloseKey(hkey);

			CSW_PROFILE_FUNCTION_EXIT(cfg_write_8);
	        return ERR_CFG_WRITE_REG_FAILURE;
        }
    }
    else
    {
		//err_code = SUL_GetLastError();
		CSW_TRACE(CFW_CFG_TS_ID, "REG_OpenKey, error err_code=0x%x\n",err_code);
		CSW_PROFILE_FUNCTION_EXIT(cfg_write_8);
	    return ERR_CFG_WRITE_REG_FAILURE;
    }
}

UINT32 cfg_write_str(UINT8 *uSubkey,UINT8 *name, UINT8* data, UINT8 size)
{
	HKEY hkey;
	INT32 err_code = 0;
	CSW_PROFILE_FUNCTION_ENTER(cfg_write_str);
	CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_str, subkey=%s, value name=%s\n",uSubkey,name);
	err_code = REG_OpenKey(REG_HKEY_CURRENT_COMM, TEXT(uSubkey), &hkey);
    if(ERR_SUCCESS == err_code)
    {
		err_code = REG_SetValue(hkey, TEXT(name), REG_TYPE_STRING, data, size);
		if(ERR_SUCCESS == err_code)
        {
	        err_code = REG_CloseKey(hkey);
			CSW_PROFILE_FUNCTION_EXIT(cfg_write_str);
	        return ERR_SUCCESS;
        }
        else
        {
		 CSW_TRACE(CFW_CFG_TS_ID, "REG_SetValue , error err_code = 0x%x\n",err_code);
	        err_code = REG_CloseKey(hkey);
	        CSW_PROFILE_FUNCTION_EXIT(cfg_write_str);
	        return ERR_CFG_WRITE_REG_FAILURE;
        }
    }
    else
    {
		//err_code = SUL_GetLastError();
		CSW_TRACE(CFW_CFG_TS_ID, "REG_OpenKey, error err_code=0x%x\n",err_code);
		CSW_PROFILE_FUNCTION_EXIT(cfg_write_str);
	    return ERR_CFG_WRITE_REG_FAILURE;
    }
}


UINT32 cfg_read_str(UINT8 *uSubkey,UINT8 *name, UINT8* uData, UINT8 *uSize)
{
	HKEY hkey;
	INT32 err_code = 0;
	UINT8 type;
	UINT8 size=*uSize;
	//BOOL bret=FALSE;
	CSW_PROFILE_FUNCTION_ENTER(cfg_read_str);
	CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_str, subkey=%s, value name=%s\n",uSubkey,name);
	CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_str, usize=%d\n", *uSize);
	err_code = REG_OpenKey(REG_HKEY_CURRENT_COMM, TEXT(uSubkey), &hkey);
	if(ERR_SUCCESS != err_code)
	{
		//err_code = SUL_GetLastError();
		CSW_TRACE(CFW_CFG_TS_ID, "REG_OpenKey, error err_code=0x%x\n",err_code);
		CSW_PROFILE_FUNCTION_EXIT(cfg_read_str);
		return ERR_CFG_READ_REG_FAILURE;
	}
	
	err_code = REG_GetValue(hkey, TEXT(name), &type, uData, &size);
	if(ERR_SUCCESS == err_code)
	{
		err_code = REG_CloseKey(hkey); 
		*uSize = size;
		CSW_PROFILE_FUNCTION_EXIT(cfg_read_str);
		return ERR_SUCCESS;
	}
	else
	{
		//err_code = SUL_GetLastError();
		CSW_TRACE(CFW_CFG_TS_ID, "REG_GetValue, error err_code=0x%x\n",err_code);
		err_code = REG_CloseKey(hkey);

		CSW_PROFILE_FUNCTION_EXIT(cfg_read_str);
		return ERR_CFG_READ_REG_FAILURE;
	}
}


UINT32 cfg_read_32(UINT8 *uSubkey,UINT8 *name, UINT32 *n)
{

	HKEY hkey;
	UINT32 data;
	UINT8 type;
	UINT8 size=4;
	INT32 err_code = 0;
	CSW_PROFILE_FUNCTION_ENTER(cfg_read_32);
	CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_32, subkey=%s, value name=%s\n",uSubkey,name);
	err_code = REG_OpenKey(REG_HKEY_CURRENT_COMM, TEXT(uSubkey), &hkey);
    if(ERR_SUCCESS == err_code)
    {
	    err_code = REG_GetValue(hkey, TEXT(name), &type, &data, &size);
        if(ERR_SUCCESS == err_code)
        {
	        err_code = REG_CloseKey(hkey); 

	        *n = data;
			CSW_PROFILE_FUNCTION_EXIT(cfg_read_32);
	        return ERR_SUCCESS;
        }
        else
        {
			CSW_TRACE(CFW_CFG_TS_ID, "REG_GetValue, error err_code=0x%x\n",err_code);
			err_code = REG_CloseKey(hkey);

			CSW_PROFILE_FUNCTION_EXIT(cfg_read_32);
			return ERR_CFG_READ_REG_FAILURE;
        }
    }
    else
    {
		CSW_TRACE(CFW_CFG_TS_ID, "REG_OpenKey, error err_code=0x%x\n",err_code);
		CSW_PROFILE_FUNCTION_EXIT(cfg_read_32);
		return ERR_CFG_READ_REG_FAILURE;
    }
}

UINT32 cfg_write_32(UINT8 *uSubkey,UINT8 *name, UINT32 n)
{

	HKEY hkey;
	INT32 err_code = 0;
	CSW_PROFILE_FUNCTION_ENTER(cfg_write_32);
	CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_32, subkey=%s, value name=%s\n",uSubkey,name);
	err_code = REG_OpenKey(REG_HKEY_CURRENT_COMM, TEXT(uSubkey), &hkey);
    if(ERR_SUCCESS == err_code)
    {
	    err_code = REG_SetValue(hkey, TEXT(name), REG_TYPE_INT32, &n, 4);
        if(ERR_SUCCESS == err_code)
        {
	        err_code = REG_CloseKey(hkey);
		 CSW_PROFILE_FUNCTION_EXIT(cfg_write_32);
	        return ERR_SUCCESS;
        }
        else
        {
			//err_code = SUL_GetLastError();
			CSW_TRACE(CFW_CFG_TS_ID, "REG_SetValue, error err_code=0x%x\n",err_code);
			err_code = REG_CloseKey(hkey);
			CSW_PROFILE_FUNCTION_EXIT(cfg_write_32);
			return ERR_CFG_WRITE_REG_FAILURE;
        }
    }
    else
    {
		//err_code = SUL_GetLastError();
		CSW_TRACE(CFW_CFG_TS_ID, "REG_OpenKey, error err_code=0x%x\n",err_code);
		CSW_PROFILE_FUNCTION_EXIT(cfg_write_32);
	    return ERR_CFG_WRITE_REG_FAILURE;
    }
	return ERR_SUCCESS;
}

/*16 BITs*/
UINT32 cfg_read_hex(UINT8 *uSubkey,UINT8 *name, PVOID udata,UINT8 usize)
{
	HKEY hkey;
	UINT8 type;
	UINT8 size=usize;
	INT32 err_code = 0;
	//BOOL bRet;
	CSW_PROFILE_FUNCTION_ENTER(cfg_read_hex);
	CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, subkey=%s, value name=%s\n",uSubkey,name);

	err_code = REG_OpenKey(REG_HKEY_CURRENT_COMM, TEXT(uSubkey), &hkey);
	if (ERR_SUCCESS != err_code)
	{
		//err_code = SUL_GetLastError();
		CSW_TRACE(CFW_CFG_TS_ID, "REG_OpenKey, error err_code=0x%x,size=%d\n",err_code,size);
		CSW_PROFILE_FUNCTION_EXIT(cfg_read_hex);
		return ERR_CFG_READ_REG_FAILURE;
	}
	err_code = REG_GetValue(hkey, TEXT(name), &type, udata, &size);
	if(ERR_SUCCESS == err_code)
	{
		err_code = REG_CloseKey(hkey);
		if(size != usize)
		{
			CSW_TRACE(CFW_CFG_TS_ID, "in cfg_read_hex,1,call REG_OpenKey return success, but returned size is wrong, the right value:%d,returned:%d\n",usize, size);
			CSW_PROFILE_FUNCTION_EXIT(cfg_read_hex);
			return ERR_CFG_FAILURE;
		}
	}
	else
	{
		//err_code = SUL_GetLastError();
		CSW_TRACE(CFW_CFG_TS_ID, "REG_OpenKey, error err_code=0x%x\n",err_code);
		err_code = REG_CloseKey(hkey);
		CSW_PROFILE_FUNCTION_EXIT(cfg_read_hex);
		return ERR_CFG_READ_REG_FAILURE;
	}
    return ERR_SUCCESS;
}

UINT32 cfg_write_hex(UINT8 *uSubkey,UINT8 *name, PVOID data,UINT8 usize)
{

	HKEY hkey;
	INT32 err_code = 0;
	CSW_PROFILE_FUNCTION_ENTER(cfg_write_hex);
	CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_hex, subkey=%s, value name=%s\n",uSubkey,name);
	err_code = REG_OpenKey(REG_HKEY_CURRENT_COMM, TEXT(uSubkey), &hkey);
    if(ERR_SUCCESS == err_code)
    {
	    err_code = REG_SetValue(hkey, TEXT(name), REG_TYPE_BINARY, data, usize);
        if(ERR_SUCCESS == err_code)
        {
	        err_code = REG_CloseKey(hkey);
		 CSW_PROFILE_FUNCTION_EXIT(cfg_write_hex);
	        return ERR_SUCCESS;
        }
        else
        {
		//err_code = SUL_GetLastError();
		CSW_TRACE(CFW_CFG_TS_ID, "REG_SetValue, error err_code=0x%x\n",err_code);
		err_code = REG_CloseKey(hkey);
		CSW_PROFILE_FUNCTION_EXIT(cfg_write_hex);
		return ERR_CFG_WRITE_REG_FAILURE;
        }
    }
    else
    {
		//err_code = SUL_GetLastError();
		CSW_TRACE(CFW_CFG_TS_ID, "REG_OpenKey, error err_code=0x%x\n",err_code);
		CSW_PROFILE_FUNCTION_EXIT(cfg_write_hex);
	    return ERR_CFG_WRITE_REG_FAILURE;
    }
}

UINT32 cfg_readdefault(UINT8 *uSubkey,UINT8 *name, void *data)
{
	UINT32 size=0;
	//BOOL ret = 0;
	UINT8 utype =0;
	INT32 err_code = 0;
	CSW_PROFILE_FUNCTION_ENTER(cfg_readdefault);
	CSW_TRACE(CFW_CFG_TS_ID, "cfg_write_hex, subkey=%s, value name=%s\n",uSubkey,name);

	err_code = REG_QueryDefaultValue(REG_HKEY_CURRENT_COMM, uSubkey, name, &utype, NULL, &size);
	if(err_code == ERR_SUCCESS)
	{
		err_code = REG_QueryDefaultValue(REG_HKEY_CURRENT_COMM, uSubkey, name, &utype, data, &size);
		if(err_code != ERR_SUCCESS)
		{
			//err_code = SUL_GetLastError();
			CSW_TRACE(CFW_CFG_TS_ID, "REG_QueryDefaultValue data, error err_code=0x%x\n",err_code);
		}

	}
	else
	{
		//err_code = SUL_GetLastError();
		CSW_TRACE(CFW_CFG_TS_ID, "REG_QueryDefaultValue error err_code=0x%x\n",err_code);
	}
	CSW_PROFILE_FUNCTION_EXIT(cfg_readdefault);
	return (UINT32)err_code;
}
//]]hameina[+]2006.8.22

