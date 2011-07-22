#ifdef CFW_GPRS_SUPPORT

#include <csw.h>



#ifndef _CFW_GPRS_DATA_H_
#define _CFW_GPRS_DATA_H_



#include <cswtype.h>
#include <cfw.h>

#define READ_NUMBER_SETTING_8(WHICH, X, FLAG)\
{HKEY hkey;INT32 data;UINT8 type;UINT32 size;\
    if(REG_OpenKey(REG_HKEY_CURRENT_COMM, TEXT(FLAG), &hkey))\
    {\
        if(REG_QueryValue(hkey, TEXT(WHICH), &type, (void*)&data, &size))\
        {REG_CloseKey(hkey); X = (UINT8)data;}\
        else{REG_CloseKey(hkey);return ERR_CFG_READ_REG_FAILURE;}\
    }else{return ERR_CFG_READ_REG_FAILURE;}\
}
 

#define WRITE_NUMBER_SETTING_8(WHICH, X, FLAG)\
{HKEY hkey;\
    if(REG_OpenKey(REG_HKEY_CURRENT_COMM, TEXT(FLAG), &hkey))\
    {\
        if(REG_SetValue(hkey, TEXT(WHICH), REG_TYPE_INT8, (void*)&X, sizeof(X)))\
        {REG_CloseKey(hkey);}\
        else{REG_CloseKey(hkey);return ERR_CFG_WRITE_REG_FAILURE;}\
    }else{return ERR_CFG_WRITE_REG_FAILURE;}\
}


#define WRITE_STRING_SETTING(WHICH, X, FLAG)\
{HKEY hkey;\
    if(REG_OpenKey(REG_HKEY_CURRENT_COMM, TEXT(FLAG), &hkey))\
    {\
        if(REG_SetValue(hkey, TEXT(WHICH), REG_TYPE_STRING, (void*)X, SUL_Strlen(X)))\
        {REG_CloseKey(hkey);}\
        else{REG_CloseKey(hkey);return ERR_CFG_WRITE_REG_FAILURE;}\
    }else{return ERR_CFG_WRITE_REG_FAILURE;}\
}


#define WRITE_BIN_SETTING(WHICH, X, FLAG, LEN)\
{HKEY hkey;\
    if(REG_OpenKey(REG_HKEY_CURRENT_COMM, TEXT(FLAG), &hkey))\
    {\
        if(REG_SetValue(hkey, TEXT(WHICH), REG_TYPE_BINARY, (void*)X, LEN))\
        {REG_CloseKey(hkey);}\
        else{REG_CloseKey(hkey);return ERR_CFG_WRITE_REG_FAILURE;}\
    }else{return ERR_CFG_WRITE_REG_FAILURE;}\
}


#define READ_STRING_SETTING(WHICH, X, FLAG)\
{HKEY hkey;UINT8 type;UINT32 size;\
    if(REG_OpenKey(REG_HKEY_CURRENT_COMM, TEXT(FLAG), &hkey))\
    {\
        if(REG_QueryValue(hkey, TEXT(WHICH), &type, X, &size))\
        {REG_CloseKey(hkey); }\
        else{REG_CloseKey(hkey);return ERR_CFG_READ_REG_FAILURE;}\
    }else{return ERR_CFG_READ_REG_FAILURE;}\
}

#define READ_BIN_SETTING(WHICH, X, FLAG, len)\
{HKEY hkey;UINT8 type;UINT32 size;\
    if(REG_OpenKey(REG_HKEY_CURRENT_COMM, TEXT(FLAG), &hkey))\
    {\
        if(REG_QueryValue(hkey, TEXT(WHICH), &type, X, &size))\
        {REG_CloseKey(hkey); len = (UINT8)size;}\
        else{REG_CloseKey(hkey);return ERR_CFG_READ_REG_FAILURE;}\
    }else{return ERR_CFG_READ_REG_FAILURE;}\
}

typedef struct _CID2INDEX {
    UINT8 nCid;
    UINT8 nNsapi;
    UINT8 nSmIndex;
    UINT8 padding;
}CID2INDEX ;

typedef struct _GPRS_SM_INFO {
    UINT32 nMsgId;           // message id 
    VOID *pMsgBody;          // message body
    CID2INDEX Cid2Index;     // cid smindex, nsapi 
    CFW_GPRS_QOS ModifyQos;  // modify qos???
    UINT8 nModifyCid;        // ...???
    UINT8 nActState;         // activated?     
    UINT8 nPDPAddrSize;   // PDP address size , only if the flash error;
    BOOL  bAttManualAcc;  // manual acc ?
	
    UINT8 nPDPAddr[4];        // used for ip address only if the flash is error ;// ip v4
    //CFW_GPRS_PDPCONT_INFO PdpCXT;
    CFW_GPRS_QOS Qos;    
    UINT8 nFlowCtr;              // 1 uplink congestion or low memory, 2 uplink congestion or low memory disappear	
    
}GPRS_SM_INFO;


#endif //
#endif

