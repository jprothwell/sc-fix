#ifndef CFW_SIM_MIS_H
#define CFW_SIM_MIS_H


#include <cfw_sim_prv.h>



//The EFLP file hold some bytes, and one byte for one language.
//The size of EFLP is variable.
typedef struct _CFW_SIM_LP
{
    UINT8 nLPSize;
    UINT8* pLP;
}CFW_SIM_LP;

EXPORT CFW_SIM_LP Sim_Lp;

//nNum is the size of Buffer as input,
//and as ouput it is the number of ECC
UINT32 CFW_SimGetECC(UINT8* pBuffer,UINT8* nNum);
VOID CFW_SimSetECC(UINT8* pBuffer);

UINT32 CFW_SimGetLP(UINT8* pBuffer,UINT16* nNum);
UINT32 CFW_SimSetLP(UINT8* pBuffer,UINT16 nNum);

UINT32 CFW_SimGetLP(UINT8* pBuffer,UINT16* nNum);
UINT32 CFW_SimSetLP(UINT8* pBuffer,UINT16 nNum);

BOOL CFW_SimGetPS(VOID);
BOOL CFW_SimGetPF(VOID);

UINT32 CFW_SimSetPF(BOOL bValue);
UINT32 CFW_SimSetPS(BOOL bValue);
#define SIM_SPYSIMCARD_INTERVAL      3000// 30000

//For Stage3
#define SM_SIMINITS3_INIT             0x00
#define SM_SIMINITS3_IDLE             0x01
#define SM_SIMINITS3_RESET            0x02
#define SM_SIMINITS3_MMI              0x03
#define SM_SIMINITS3_STACK            0x04
#define SM_SIMINITS3_ELEMENTARY       0x05
#define SM_SIMINITS3_READBINARY       0x06
#define SM_SIMINITS3_DEDICATED        0x07
#define SM_SIMINITS3_REHABILITATE     0x08

typedef struct _CFW_SIMINITS3
{ 
    UINT16 nFileSize;
    UINT8 nRecordSize;
    UINT8 nEF;
    UINT8 nMEStatus;
    UINT8 nSm_SimInitS3_prevStatus;
    UINT8 nSm_SimInitS3_currStatus;
	
    UINT8 pading;
} CFW_SIMINITS3;



//For Stage1
#define SM_SIMINITS1_INIT             0x00
#define SM_SIMINITS1_IDLE             0x01
#define SM_SIMINITS1_RESET            0x02
#define SM_SIMINITS1_MMI              0x03
#define SM_SIMINITS1_STACK            0x04
#define SM_SIMINITS1_ELEMENTARY       0x05
#define SM_SIMINITS1_READBINARY       0x06
#define SM_SIMINITS1_DEDICATED        0x07

typedef struct _CFW_SIMINITS1
{ 
    UINT8 nResetNum;
    UINT8 padding;
    UINT16 nFileSize;
    UINT8 nRecordSize;
    UINT8 nEF;
    
    UINT8 nSm_SimInitS1_prevStatus;
    UINT8 nSm_SimInitS1_currStatus;
} CFW_SIMINITS1;

typedef struct _CFW_SIMINITS
{ 
    UINT8 nResetNum;
    UINT8 padding;
    UINT16 nFileSize;
    UINT8 nRecordSize;
    UINT8 nEF;
    
    UINT8 nSm_SimInit_prevStatus;
    UINT8 nSm_SimInit_currStatus;
} CFW_SIMINITS;

//For Old Starting procedure
#define SM_SIMINIT_INIT             			0x00
#define SM_SIMINIT_IDLE             			0x01
#define SM_SIMINIT_RESET            			0x02
#define SM_SIMINIT_MMI              			0x03
#define SM_SIMINIT_STACK            		0x04
#define SM_SIMINIT_ELEMFILESTATUS   	0x05
#define SM_SIMINIT_CARDTYPE         		0x06
#define SM_SIMINIT_DEDICATED        		0x07
#define SM_SIMINIT_REHABILITATE        	0x08

typedef struct _CFW_SIMINITIALIZE
{ 
    UINT8 nEF;
    UINT8 nSm_SimInit_prevStatus;
    UINT8 nSm_SimInit_currStatus;
}CFW_SIMINITIALIZE;





//For GetPUCT
#define SM_GETPUCT_INIT             0x00
#define SM_GETPUCT_IDLE             0x01
#define SM_GETPUCT_READBINARY       0x02
#define SM_GETPUCT_MMI              0x03
#define SM_GETPUCT_STACK            0x04

typedef struct _CFW_SIMGETPUCT
{
    UINT8 nSm_GetPUCT_prevStatus;
    UINT8 nSm_GetPUCT_currStatus;
}CFW_SIMGETPUCT;



//For SetPUCT
#define SM_SETPUCT_INIT             0x00
#define SM_SETPUCT_IDLE             0x01
#define SM_SETPUCT_ELEMENTSTATUS    0x02
#define SM_SETPUCT_VERIFYCHV        0x03
#define SM_SETPUCT_UPDATEBINARY     0x04
#define SM_SETPUCT_MMI              0x05
#define SM_SETPUCT_STACK            0x06

typedef struct _CFW_SIMSETPUCT
{
    UINT8 iCurrency[3];
    UINT16 iEPPU;
    INT8 iEX;
    UINT8 pPin2[8];

    UINT8 nSm_SetPUCT_prevStatus;
    UINT8 nSm_SetPUCT_currStatus;
}CFW_SIMSETPUCT;


typedef struct _SIM_SM_STATE
{
    UINT8 nPreState;
    UINT8 nCurrState;
    UINT8 nNextState;
	UINT8 pad;
}SIM_SM_STATE;
//add by wulc 2009 10-25
typedef struct _SIM_SM_INFO_GETSERVERPROVIDERNAME
{   
       UINT8 nSize;
	   UINT8 pad[3];
       SIM_SM_STATE nState;
}SIM_SM_INFO_GETSERVERPROVIDERNAME;
//add by wulc 2009 10-25


// SimGetProviderId's struct
typedef struct _SIM_SM_RESET
{   
    SIM_SM_STATE nState;
}SIM_SM_RESET;
typedef struct _SIM_SM_INFO_GETPROVIDERID
{   
    SIM_SM_STATE nState;
}SIM_SM_INFO_GETPROVIDERID;

typedef struct _SIM_SM_INFO_SETPREFOPREATORLIST
{   
    UINT8  nOperatorList[255];
    UINT8  nSize;
    SIM_SM_STATE nState;
}SIM_SM_INFO_SETPREFOPREATORLIST;

typedef struct _SIM_SM_INFO_GETPREFOPREATORLIST
{   
    UINT8 nSize;
	UINT8 pad[3];
    SIM_SM_STATE nState;
}SIM_SM_INFO_GETPREFOPREATORLIST;

typedef struct _SIM_SM_INFO_GETPREFOPREATORLISTMAXNUM
{   
    SIM_SM_STATE nState;
}SIM_SM_INFO_GETPREFOPREATORLISTMAXNUM;

typedef struct _SIM_SM_INFO_INIT
{                  
    SIM_SM_STATE nState;    
}SIM_SM_INFO_INIT;
UINT32 CFW_SimResetProc (HAO hAo,CFW_EV* pEvent);

UINT32 CFW_SimMisGetProviderIdProc (HAO hAo,CFW_EV* pEvent);

UINT32 CFW_SimMisSetPrefListProc (HAO hAo,CFW_EV* pEvent);

UINT32 CFW_SimMisGetPrefListProc (HAO hAo,CFW_EV* pEvent);

UINT32 CFW_SimMisGetPrefListMAXNumProc (HAO hAo,CFW_EV* pEvent);
UINT32 CFW_SimGetServiceProviderNameProc(HAO hAo, CFW_EV* pEvent);


UINT32 CFW_SimInitProc (HAO hAo,CFW_EV* pEvent);

#define CFW_SIM_MIS_IDLE                     0x00
#define CFW_SIM_MIS_READBINARY               0x01
#define CFW_SIM_MIS_UPDATEBINARY             0x02
#define CFW_SIM_MIS_READELEMENT              0x03
#define CFW_SIM_MIS_RESET                    0x04
typedef struct _SIM_INFO_READBINARY{   
    UINT8  n_PrevStatus;
    UINT8  n_CurrStatus;
    UINT8  nTryCount;      

    UINT8  nFileId;
    UINT16  nOffset;
    UINT8  nBytesToRead;
    UINT8  pattern[2];   
}SIM_INFO_READBINARY;

typedef struct _SIM_INFO_UPDATEBINARY{  
    UINT8  n_PrevStatus;
    UINT8  n_CurrStatus;
    UINT8  nTryCount;     
    
    UINT8  nFileId;
    UINT16  nOffset;
    UINT8  nBytesToWrite;
    UINT8  pDate[256];
    UINT8  pattern[2];
}SIM_INFO_UPDATEBINARY;
typedef struct _SIM_INFO_READRECORD{  
    UINT8   n_PrevStatus;
    UINT8   n_CurrStatus; 
    UINT8   nTryCount;                  
    
    UINT8  nFileId;           
    UINT8  nRecordNum;
    UINT8  nMode;
    UINT8  nRecordSize;
    UINT8  pattern[1];
}SIM_INFO_READRECORD;


typedef struct _SIM_INFO_UPDATERECORD{  
    UINT8   n_PrevStatus;
    UINT8   n_CurrStatus; 
    UINT8   nTryCount;                  
    
    UINT8  nFileId;           
    UINT8  nRecordNum;
    UINT8  nMode;
    UINT8  nRecordSize;    
    //UINT8  nPIN2Size; Remove by lixp for at issue
    //UINT8  nPIN2[8];
    
    UINT8  pData[256];    
}SIM_INFO_UPDATERECORD;

#define SIM_MODE_NEXT_RECORD                 0x02
#define SIM_MODE_PRE_RECORD                  0x03
#define SIM_MODE_ABSOLUTE                    0x04

#define SIM_STATUS_IDLE                      0x01
#define SIM_STATUS_READRECORD                0x02
#define SIM_STATUS_UPDATERECORD              0x03
#define SIM_STATUS_SEEKRECORD                0x04
#define SIM_STATUS_READBINARY                0x05
#define SIM_STATUS_UPDATEBINARY              0x06
#define SIM_STATUS_ELEMFILE                  0x07
#define SIM_STATUS_VERIFY                    0x08

UINT32 CFW_SimReadRecordProc (HAO hAo,CFW_EV* pEvent);
UINT32 CFW_SimUpdateRecordProc (HAO hAo,CFW_EV* pEvent);

VOID cfw_PLMNtoBCDEX(UINT8 *pIn, UINT8* pOut, UINT8* nInLength);

#define CFW_SIM_MIS_VERIFY                   0x05
#define CFW_SIM_MIS_READRECORD               0x06
#define CFW_SIM_MIS_UPDATERECORD             0x07

typedef struct _SIM_SM_INFO_SETACM
{                 
    UINT32 iACMValue;
    UINT8  nPIN2[8];
    UINT8  nPIN2Size;
    UINT8  nMark;
	UINT8 pad[2];
    SIM_SM_STATE nState;    
}SIM_SM_INFO_SETACM;

typedef struct _SIM_SM_INFO_GETACM
{                  
    SIM_SM_STATE nState;    
}SIM_SM_INFO_GETACM;



#define ACM_STATUS_IDLE                     0x01
#define ACM_STATUS_READBINARY               0x02
#define ACM_STATUS_UPDATEBINARY             0x03
#define ACM_STATUS_VERIFYCHV                0x04
#define SMS_STATUS_GETACMMAX                0x01
#define SMS_STATUS_SETACMMAX                0x02
#define SMS_STATUS_VERIFYCHV                0x03

typedef struct _ACM_INFO_SETACMMAX
{
    UINT8  n_PrevStatus;
    UINT8  n_CurrStatus;
    UINT8  nTryCount;  
    
    UINT32 iACMMaxValue; 
    UINT8  pPin2[8];
    UINT8  nPinSize;
}ACM_INFO_SETACMMAX;

typedef struct _ACM_INFO_GETACMMAX
{
    UINT8  n_PrevStatus;
    UINT8  n_CurrStatus;
    UINT8  nTryCount; 
    
    UINT8  pattern[1]; 
}ACM_INFO_GETACMMAX;
typedef struct _GET_FILE_STATUS_INFO
{
    UINT8  n_PrevStatus;
    UINT8  n_CurrStatus;
    UINT8  nTryCount;     
    UINT8  nFileID; 
    
}GET_FILE_STATUS_INFO;
UINT32 CFW_SimSetACMMaxProc (HAO hAo,CFW_EV* pEvent);
UINT32 CFW_SimGetACMMaxProc(HAO hAo,CFW_EV* pEvent);
UINT32 CFW_SimMisGetACMProc(HAO hAo,CFW_EV* pEvent);
UINT32 CFW_SimMisSetACMProc(HAO hAo,CFW_EV* pEvent);
UINT32 CFW_SimGetPUCTProc(HAO hAo,CFW_EV* pEvent);
UINT32 CFW_SimSetPUCTProc(HAO hAo,CFW_EV* pEvent);


#ifdef CFW_MULTI_SIM
BOOL CFW_SimInitStage2(CFW_SIM_ID nSimID);
UINT32 CFW_SimInitStage3(CFW_SIM_ID nSimID);
UINT32 CFW_SimGetPUCT(UINT16 nUTI, CFW_SIM_ID nSimID);

UINT32 CFW_SimSetPUCT(
  CFW_SIM_PUCT_INFO  *pPUCT,
  UINT8 *pPin2, 
  UINT8 nPinSize, 
  UINT16 nUTI,
  CFW_SIM_ID nSimID
);

UINT32 CFW_SimReset(
  UINT16 nUTI, 
  CFW_SIM_ID nSimID
);

UINT32 CFW_SimSetACMMax(
  UINT32 iACMMaxValue,
  UINT8 *pPin2, 
  UINT8 nPinSize,
  UINT16 nUTI, 
  CFW_SIM_ID nSimID
);

UINT32 CFW_SimGetACMMax(
  UINT16 nUTI, 
  CFW_SIM_ID nSimID
);

UINT32 CFW_SimGetACM(UINT16 nUTI, CFW_SIM_ID nSimID);

UINT32  CFW_SimSetACM(
  UINT32 iCurValue,
  UINT8 *pPIN2,
  UINT8  nPinSize,
  UINT16 nUTI, 
  CFW_SIM_ID nSimID
);


#else
BOOL CFW_SimInitStage2(VOID);
UINT32 CFW_SimInitStage3(VOID);
UINT32 CFW_SimGetPUCT(UINT16 nUTI);

UINT32 CFW_SimSetPUCT(
            CFW_SIM_PUCT_INFO  *pPUCT,
            UINT8 *pPin2, 
            UINT8 nPinSize, 
            UINT16 nUTI
  );

UINT32 CFW_SimReset(
  UINT16 nUTI
);

UINT32 CFW_SimSetACMMax(
  UINT32 iACMMaxValue,
  UINT8 *pPin2, 
  UINT8 nPinSize,
  UINT16 nUTI
);

UINT32 CFW_SimGetACMMax(
  UINT16 nUTI
);

UINT32 CFW_SimGetACM(UINT16 nUTI);

UINT32  CFW_SimSetACM(
  UINT32 iCurValue,
  UINT8 *pPIN2,
  UINT8  nPinSize,
  UINT16 nUTI
);


#endif

#ifdef CFW_MULTI_SIM
UINT32 CFW_CfgGetIMSI(UINT8* pIMSI, CFW_SIM_ID nSimID);
UINT32 CFW_CfgSetIMSI(UINT8* pIMSI, CFW_SIM_ID nSimID);
#else
UINT32 CFW_CfgGetIMSI(UINT8* pIMSI);
UINT32 CFW_CfgSetIMSI(UINT8* pIMSI);
#endif
HAO SimSpySimCard(CFW_EV* pEvent);

#ifdef CFW_EXTENDED_API

// ---------------------
// MACROS & DEFINES
// ---------------------

#ifdef CFW_MULTI_SIM
 #define CFW_WITH_SIM_ID(a,b)    (a),(b)
 #define CFW_NOTIFY_SIM_ID(a,b)  (a)|((b)<<24)
#else
 #define CFW_WITH_SIM_ID(a,b)    (a)
 #define CFW_NOTIFY_SIM_ID(a,b)  (a)
#endif

#define CFW_SIM_ERROR(pStatus) (!(((0x90==pStatus->Sw1)&&(0x00==pStatus->Sw2))||(0x91==pStatus->Sw1)))

#define CFW_SIM_ASSERT_ERROR(isError, errorCode, event) \
    if (isError) \
    { \
        UINT32 nUTI; \
        CFW_GetUTI(hAo, &nUTI); \
        CFW_PostNotifyEvent (event,errorCode,0,CFW_NOTIFY_SIM_ID(nUTI,simId), 0XF0); \
        CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW SIM : ERROR n°0x%x, sending notification with event id 0x%x\n"),errorCode, event); \
        CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo); \
        hAo = HNULL; \
        return errorCode; \
    } \

#define CFW_SIM_SEND_RESULT(param1, param2, event) \
    { \
        UINT32 nUTI; \
        CFW_GetUTI(hAo, &nUTI); \
        CFW_PostNotifyEvent (event,(UINT32)(param1),(UINT32)(param2),CFW_NOTIFY_SIM_ID(nUTI,simId), 0); \
        CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW SIM : SUCCESS, sending result with event id 0x%x\n"), event); \
        CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo); \
        hAo = HNULL; \
    } \

// ---------------------
// TYPES 
// ---------------------

typedef struct 
{
    SIM_SM_STATE nState;    
    UINT8  nTryCount;
    UINT8  fileSize;
    UINT8  fileId;
    BOOL   update;

} SIM_SM_INFO_EF;

typedef VOID (*SIM_DATA_PARSER_EF_T)(   HAO,
                                        UINT8 *, 
                                        api_SimReadBinaryCnf_t *,
                                        SIM_SM_INFO_EF *
                                        #ifdef CFW_MULTI_SIM
                                        , CFW_SIM_ID
                                        #endif
                                    );

// ---------------------
// FUNCTION DECLARATIONS 
// ---------------------

UINT32 CFW_SimMisSetPrefListProcEX(HAO hAo,CFW_EV* pEvent);
UINT32 CFW_SimMisGetPrefListProcEX(HAO hAo,CFW_EV* pEvent);
UINT32 CFW_SimGetFileStatusProc(HAO hAo, CFW_EV* pEvent);
UINT32 CFW_SimReadRecordProc (HAO hAo,CFW_EV* pEvent);
UINT32 CFW_SimUpdateRecordProc (HAO hAo,CFW_EV* pEvent);
UINT32 cfw_SimReadUpdateElementaryFileProc(HAO hAo,CFW_EV* pEvent);


#endif //  CFW_EXTENDED_API


#endif // CFW_SIM_MIS_H       


